//---------------------------------------------------------------------------
/****************************************************************************
 * Модуль реализующий работу с динамически выделяемой памятью
 * История изменений:
 * $Log: mem.cpp,v $
 * Revision 1.3  2010/06/09 06:04:44  rakhmatulin
 * no message
 *
 *
 ***************************************************************************/
//---------------------------------------------------------------------------


// #include <sysdefs.h>
#include <string.h>
#include "cfg.h"
#include "../extern.h"
#include "tools.h"
//#include <iostream>

// #define PRINT_MEM_REST

extern "C" void __cxa_pure_virtual(void)
{
	// call to a pure virtual function happened ... wow, should never happen ... stop
	//Lprint(1, 0, "Internal Error");
	while(1);
}

#define HEAP_SIZE 1080000
#define HEAP_PAGE_CNT 45000
#define HEAP_PAGE_SIZE 24

// char gHEAP[HEAP_SIZE];
char * const gHEAP = (char*) 0x20000000UL;
size_t gPageTable[HEAP_PAGE_CNT];

#define MAX_MEM_BLOCK (HEAP_PAGE_SIZE << sizeof(gPageTable[0])*8)

int gHEAPInited = 0;

//---для дебага-----------------------------------
#ifdef MEM_DEBUG
namespace
{
	typedef struct _SMemInfo
	{
		void*		pPtr;
		const char*	pFile;
		long		nLine;
	} SMemInfo;

	SMemInfo	gMemInfoTable[HEAP_PAGE_CNT];
	size_t		gCurMemInfoIndx;

	int gFindMemInfoPtr(void* pPtr)
	{
		for (int i = 0; i < gCurMemInfoIndx; ++i)
		{
			if (gMemInfoTable[i].pPtr == pPtr)
				return i;
		}

		return -1;
	}

	void gDeleteMemInfoPtr(void* pPtr)
	{
		size_t nStartPos = gFindMemInfoPtr(pPtr);
		if (pPtr < 0 || nStartPos < 0)
		{
			DBGLine("gDeleteMemInfoPtr() - wtf?");
			while (true)
				;
		}
		for (size_t i = nStartPos; i < gCurMemInfoIndx - 1; ++i)
			gMemInfoTable[i] = gMemInfoTable[i + 1];

		--gCurMemInfoIndx;
	}

	struct Sentinel
	{
		~Sentinel();
	};

	Sentinel::~Sentinel()
	{
		if (gCurMemInfoIndx > 0)
		{
			DBGLine("Leaked memory at:");
			for (size_t i = 0; i < gCurMemInfoIndx; ++i)
			{
				DBGLine("    %d", (int)gMemInfoTable[i].pPtr);
				DBGLine("    %s", (char*)gMemInfoTable[i].pFile);
				DBGLine("    %d", (int)gMemInfoTable[i].nLine);
			}
		}
		else
		{
			DBGLine("No user memory leakes!");
		}
	}
	Sentinel gMemLeakesDbg;
};

void gDbgInitRAM()
{
	memset(gPageTable, 0, HEAP_PAGE_CNT);
	gHEAPInited = 1;
	
	memset(gMemInfoTable, 0, sizeof(gMemInfoTable));
	gCurMemInfoIndx = 0;
}
//------------------------------------------------
#endif

/// Инициализация модуля
void gInitRAM()
{
	memset(gPageTable, 0, HEAP_PAGE_CNT);
	gHEAPInited = 1;
}




	/// Получение минимально необходимого количества блоков памяти исходя из объема в байтах
	///
	/// \param [out] количество необходимых страниц памяти
	/// \param [in] необходимый объем в байтах
size_t getMinPages(size_t nReqSize)
{
	size_t nPages = nReqSize / HEAP_PAGE_SIZE;
	if(nReqSize > nPages) nPages++;
	return nPages;
}

	/// Получение объема сводобной памяти
	///
	/// \param [out] Размер свободной памяти
size_t getFreeMemSize()
{
	size_t nFreeMem = 0;
	for(int i = 0; i < HEAP_PAGE_CNT; i++)
	{
		if(gPageTable[i] == 0) nFreeMem += HEAP_PAGE_SIZE;
	}
	return nFreeMem;
}


	/// Поиск свободных блоков памяти в соответствии с необходимым размером
	///
	/// \param [out] адрес первого сводобного блока
	/// \param [in] необходимое количество блоков памяти
void *findFreePages(size_t nPages)
{
#ifdef PRINT_MEM_REST
	static size_t nLastFreeMemSize = 0;
	if (nLastFreeMemSize != getFreeMemSize())
	{
		DBGLine("Free memory: %d", getFreeMemSize());
		nLastFreeMemSize = getFreeMemSize();
	}
#endif

	int i,j;
	for(i = 0; i < HEAP_PAGE_CNT; i++)
	{
		if(gPageTable[i] == 0)
		{
			for (j = 0; j < nPages; j++)
			{
				if(gPageTable[i + j] != 0) break;
			}
			if(j == nPages) 
			{
				//we found mem block, now mark it as busy
				int nEndBlock = i + nPages;
				for (j = i; j < nEndBlock; j++)
				{
					gPageTable[j] = i + 1;
				}
				//std::cout << "GET page = "<<i << "; nPages = "<< nPages<<"\n";
				return (void*)(gHEAP + i * HEAP_PAGE_SIZE);
			}
		}
	}
	//can't find free block requested length
	DBGLine("ERROR: Memory empty!!!!!!!!!!!!");
	Wait1MS(3000);
	return NULL;
}

	/// Выделение блока памяти
	///
	/// \param [out] указатель на выделенную память
	/// \param [in] размер выделяемой памяти
void *getMemory(size_t nReqSize)
{
	//std::cout << "NEW["<<nReqSize<<"]\n";
	if (gHEAPInited == 0)
	{
#ifdef MEM_DEBUG
		gDbgInitRAM();
#else
		gInitRAM();
#endif
	}

	if (nReqSize == 0)
		return NULL;
	size_t nPages = getMinPages(nReqSize);
	return findFreePages(nPages);
}

	/// Удаление блока выделеной памяти
	///
	/// \param [in] указатель на выделенную память
void freeMemory(void *pMem)
{
	//std::cout << "FREE["<<(int)pMem<<"]\n";
	if(pMem >= gHEAP && pMem < (gHEAP + HEAP_PAGE_CNT * HEAP_PAGE_SIZE))
	{
		int i;
		char *pRam = gHEAP;
		for(i = 0; i < HEAP_PAGE_CNT; i++, pRam += HEAP_PAGE_SIZE)
		{
			if(pMem == pRam)
			{
				size_t nPage = i + 1;
				if(gPageTable[i] != nPage)
				{
					//error wrong pointer!!!
					//std::cout << "FREE: error wrong pointer!!!\n";
					return;
				}
				//mark memory as free
				
				while (i < HEAP_PAGE_CNT && gPageTable[i] == nPage)
				{
					gPageTable[i++] = 0;
				}
				return;
			}
		}
	}
	// pointer out of memory
	//std::cout << "FREE: error pointer out of memory!!!\n";

	return;
}


#ifdef MEM_DEBUG
//---операторы с дебагом---
void* operator new(size_t nReqSize, const char* strFile, long nLine)
{
	void*	pFreeMem = getMemory(nReqSize);
	if (gCurMemInfoIndx == HEAP_PAGE_CNT)
		DBGLine("Memory map too small");

	gMemInfoTable[gCurMemInfoIndx].pPtr		= pFreeMem;
	gMemInfoTable[gCurMemInfoIndx].pFile	= strFile;
	gMemInfoTable[gCurMemInfoIndx].nLine	= nLine;
	++gCurMemInfoIndx;
}

void* operator new[](size_t nReqSize, const char* strFile, long nLine)
{
	return operator new(nReqSize, strFile, nLine);
}

#endif

void operator delete(void* pMem)
{
#ifdef MEM_DEBUG
	if (gFindMemInfoPtr(pMem) >= 0)
	{
		freeMemory(pMem);
		if (gCurMemInfoIndx < 0)
		{
			DBGLine("operator delete() - wtf?");
			while (true)
				;
		}

		gDeleteMemInfoPtr(pMem);
	}
	else if (!pMem)
	{
		DBGLine("Attempt to delete unknown pointer");
		DBGLine("    %d", (int)pMem);
		while (true)
			;
	}
#else
	freeMemory(pMem);
#endif
}

void operator delete[](void* pMem)
{
	operator delete(pMem);
}


//---обычные операторы---
/// Перегруженный оператор выделения блока памяти new
///
/// \param [out] указатель на выделенную память
/// \param [in] размер выделяемой памяти
void* operator new (size_t nReqSize)
{
	return getMemory(nReqSize);
}

/// Перегруженный оператор выделения блока памяти new[]
///
/// \param [out] указатель на выделенную память
/// \param [in] размер выделяемой памяти
void* operator new[] (size_t nReqSize)
{
	return getMemory(nReqSize);
}



/*
/// Перегруженный оператор удаления блока выделеной памяти delete
///
/// \param [in] указатель на выделенную память
void operator delete (void *pMem)
{
	freeMemory(pMem);
}

/// Перегруженный оператор удаления блока выделеной памяти delete[]
///
/// \param [in] указатель на выделенную память
void operator delete[] (void *pMem)
{
	freeMemory(pMem);
}
*/

void* newmalloc(size_t n)
{
	return getMemory(n);
}

void newfree(void *p)
{
	freeMemory(p);
}

extern "C"
{

	void *XMALLOC(size_t n, void* heap)
	{
		return getMemory(n);
	}
	void *MYMALLOC(size_t n)
	{
		return getMemory(n);
	}

	void *XREALLOC(void *p, size_t n, void* heap)
	{
		void* pMem = getMemory(n);
		if (p != NULL)
		{
			memcpy(pMem, p, n);
			freeMemory(p);
		}
		return pMem;
	}

	extern void *XCALLOC(size_t n, size_t s, void* heap);


	void XFREE(void *p, void* heap)
	{
		freeMemory(p);
	}

	void MYFREE(void *p)
	{
		freeMemory(p);
	}

};
