
#include "fs_flex.h"
#include "CFileSystem.h"

CFileSystem	*g_pFileSystem = NULL;

DWORD fs_open(const char *strFileName, WORD nMode, WORD nLock /*= 0 */ )
{
	return g_pFileSystem->open(strFileName, nMode, nLock);
}

void fs_close( DWORD nHandle )
{
	return g_pFileSystem->close(nHandle);
}

bool fs_remove(const char *strFileName )
{
	return g_pFileSystem->remove(strFileName);
}

DWORD fs_read( DWORD nHandle, BYTE *pBuffer, DWORD nLength )
{
	return g_pFileSystem->read(nHandle, pBuffer, nLength);
}

DWORD fs_write( DWORD nHandle, BYTE *pBuffer, DWORD nLength )
{
	return g_pFileSystem->write(nHandle, pBuffer, nLength);
}

DWORD fs_size( DWORD nHandle )
{
	return g_pFileSystem->size(nHandle);
}

DWORD fs_lseek( DWORD nHandle, int nOffset, char nOrigin )
{
	return g_pFileSystem->lseek(nHandle, nOffset, nOrigin);
}

bool fs_eof( DWORD nHandle )
{
	return g_pFileSystem->eof(nHandle);

}


void fs_init()
{
	if (!g_pFileSystem)
	{
		g_pFileSystem = new CFileSystem;
	}
}

void fs_uninit()
{
	if (g_pFileSystem)
	{
		g_pFileSystem->sync();
		delete g_pFileSystem;
		g_pFileSystem = NULL;
	}
}

DWORD fs_tell( DWORD nHandle )
{
	return g_pFileSystem->tell(nHandle);
}


int fs_ls( char *strFiles )
{
	return g_pFileSystem->ls(strFiles);
}

void fs_sync()
{
	return g_pFileSystem->sync();
}

DWORD fs_freeSpace()
{
	return g_pFileSystem->freeSpace();
}

void fs_format()
{
	return g_pFileSystem->formatExtern();
}

bool fs_exists_size( const char *strFileName, DWORD *nSize )
{
	return g_pFileSystem->fileExistsSize(strFileName, nSize);

}
