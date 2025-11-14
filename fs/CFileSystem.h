#ifndef	__CFILESYSTEM_H
#define	__CFILESYSTEM_H


#include "../internal/types.h"

// #define	MODE_MEMORY_RAM		// Устанавливается в случае работы с '0x50000000 2MB battery backup working ram with data saving area'

#define FS_MAGIC_STRING		"0000000000000052"

#define FS_BEGIN			((BYTE*)0x30440000UL)
#define FS_DATA_BEGIN		((BYTE*)0x30500000UL)
#define FS_CLUSTER_SIZE		512 // 800
#define	FS_FILES_COUNT		64
#define FS_CLUSTERS_COUNT	0x1000 //1366
#define FS_FREE_CLUSTER		0xFFFF
#define FS_BUSY_CLUSTER		0xFFFE


#define FS_WRONG_STRING		"FLEX WRONG FSSYS"
#define FS_BACKUP_BEGIN		((BYTE*)0x30700000UL)


#define FS_CALC_SIZE		(sizeof(SFSBoot) + FS_FILES_COUNT * sizeof(SFSFileRecord) + FS_CLUSTERS_COUNT * sizeof(WORD))

// sizeof(SFSBoot) = 36
// sizeof(SFSFileRecord) = 72

#define FS_RAM_SIZE			30000



typedef struct _SFSFileRecord  
{
	char  strFileName[32];
	BYTE  pCreateDate[12];
	BYTE  pChangeDate[12];
	WORD  attr;
	WORD  nCluster;
	DWORD nSize;
	DWORD nCrc32;
	DWORD nSeekPos;
} SFSFileRecord;




typedef struct _SFSBoot
{
	char			strMagicWord[18];
	WORD			nClusterSize;
	WORD			nClustersCount;
	WORD			nMaxFiles;
	SFSFileRecord*  pFiles;
	WORD*			pClusters;
	BYTE*			pData;
} SFSBoot;

typedef struct _SFSHandle
{
	WORD	nFSHandle;
	WORD	nMode;
	int		nSeekPos;
	WORD	nLock;
} SFSHandle;

typedef std::map < DWORD, SFSHandle* > tMapFSHandles;



#define FS_CLUSTER_BUFFER_SIZE	10000

class CClusterBuffer
{
public:
	CClusterBuffer();
	void	write(char *pDest, int nSize, char *pSource);
	void	flush();
private:
	char	*m_pDestAddr;
	char	m_pBuf[ FS_CLUSTER_BUFFER_SIZE ];
	int		m_nSize;
};


class CFileSystem
{
public:
	CFileSystem();
	~CFileSystem();
	DWORD	open(const char *strFileName, WORD nMode, WORD nLock = 0);
	void	close(DWORD	nHandle);
	bool	remove(const char *strFileName);
	bool	fileExistsSize(const char *strFileName, DWORD *nSize);
	DWORD	read(DWORD nHandle, BYTE *pBuffer, DWORD nLength);
	DWORD	write(DWORD nHandle, BYTE *pBuffer, DWORD nLength);
	DWORD	size(DWORD nHandle);
	DWORD	lseek(DWORD nHandle, int nOffset, char nOrigin);
	DWORD	tell(DWORD nHandle);
	DWORD	freeSpace();
	bool	eof(DWORD nHandle);
	void	dump();
	void	sync();
	void	formatExtern();
	

	int		ls(char *strFiles);

private:
	void	format();
	void	init();
	DWORD	createFile(const char *strFileName, WORD nMode, WORD nLock = 0);
	DWORD	write(SFSHandle	*pstcHandle, BYTE *pBuffer, DWORD nLength);
	WORD	findFreeCluster();
	DWORD	calcCRC32(DWORD nHandle);
	
	void	clearHandles();
	int		getBusyFiles();
	int		getBusyClusters();
	SFSBoot m_stcBoot;
	tMapFSHandles	m_mapFSHandles;
	DWORD	m_nCurHandle;
	WORD	m_arrnOpenedFiles[ FS_FILES_COUNT ];
	char	m_arrRAM [ FS_RAM_SIZE ];
	bool	m_bNeedSync;
	CClusterBuffer	m_objClusterBuf;
};





#endif

