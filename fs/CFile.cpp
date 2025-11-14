
#include "CFile.h"
#include "../extern.h"

CFile::CFile()
{
	_init();
}
//-----------------------------------------------------------------------------/
CFile::~CFile() {close();}
//-----------------------------------------------------------------------------/
void CFile::_init()
{
	m_h = FS_INVALID_HANDLE;
	m_dwMode = 0;
}
//-----------------------------------------------------------------------------/
bool CFile::open(LPCTSTR sFile, DWORD dwMode /* = O_RDWR|O_CREAT */, bool bLock /* = false */)
{
	m_dwMode = dwMode;
	m_strFileName = sFile;
	m_h = fs_open(sFile, dwMode, bLock);
	seek(0, FS_SEEK_SET);
	return m_h != FS_INVALID_HANDLE;
}
//-----------------------------------------------------------------------------/
void CFile::close()
{
	if (!fail()) {fs_close(m_h);}
	m_h = FS_INVALID_HANDLE;
	m_strFileName.clear();
}
//-----------------------------------------------------------------------------/
DWORD CFile::size() const
{
	if (fail()) {return 0;}
	return fs_size(m_h);
}
//-----------------------------------------------------------------------------/
bool CFile::seek( DWORD dwOffset, DWORD dwOrigin )
{
	if (fail()) {return false;}
	return fs_lseek(m_h, dwOffset, dwOrigin) != FS_IO_ERROR;
}
//-----------------------------------------------------------------------------/
DWORD CFile::write(BYTE* pData, DWORD szData)
{
	if (!isWriteable()) {return 0;}
	return fs_write(m_h, pData, szData);
}
//-----------------------------------------------------------------------------/
DWORD CFile::read( BYTE* pData, DWORD szData )
{
	if (!isReadable()) {return 0;}
	return fs_read(m_h, pData, szData);
}
//-----------------------------------------------------------------------------/
#define TILE_SIZE	4096
DWORD CFile::calcCrc32()
{
	if (!isReadable()) {return 0;}
	
	int cnTiles = size()/TILE_SIZE + Q(size()%TILE_SIZE, 1, 0);
	BYTE* pBuf = new BYTE[TILE_SIZE];
	DWORD crc32 = 0;

	for (int i=0; i < cnTiles; i++)
	{
		seek(i*TILE_SIZE, FS_SEEK_SET);
		DWORD dwReaded = read(pBuf, TILE_SIZE);
		crc32 = ::calcCRC32(pBuf, dwReaded, crc32);
	}

	DELETE_POINTER_ARR(pBuf);
	return crc32;
}
//-----------------------------------------------------------------------------/
bool CFile::remove(LPCTSTR sFile, bool bSync)
{
	if (!fs_remove(sFile)) {return false;}
	sync();
	return true;
}
//-----------------------------------------------------------------------------/
bool CFile::ls(std::list<string>& list)
{
	int cnb = fs_ls(NULL);
	if (cnb <= 0) {return false;}

	LPTSTR sBuf = new TCHAR[cnb];
	memset(sBuf, 0 , cnb);

	if (fs_ls(sBuf) <= 0) { DELETE_POINTER_ARR(sBuf); return false;}

	LPTSTR s = sBuf;
	bool bRetCode = false;

	while((*s) != '\0')
	{
		string str(s);
		ULONG len = str.length();
		if (!len) {break;}

		list.push_back(str);
		bRetCode = true;
		s += len + 1;
	}
	
	DELETE_POINTER_ARR(sBuf);
	return bRetCode;
}
//-----------------------------------------------------------------------------/
bool CFile::writeToFile(LPCTSTR sFile, LPBYTE pData, DWORD szData, bool bSync /* = false */)
{
	CFile file; 
	if (!file.open(sFile)) {return false;}
	bool bRetCode = (file.write(pData, szData) == szData);
	// Ant, 14.05.09. Не было close().
	file.close();
	if (bSync) {sync();}
	return bRetCode;
}
//-----------------------------------------------------------------------------/
bool CFile::readFromFile( LPCTSTR sFile, LPBYTE pData, DWORD szData )
{
	CFile file; 
	if (!file.open(sFile, O_RDONLY)) {return false;}
	DWORD dwReaded = file.read(pData, szData);
	// Ant, 14.05.09. Не было close().
	file.close();
	return (dwReaded == szData); 
}
bool CFile::readFromFileLoc( LPCTSTR sFile, DWORD SeekLoc, LPBYTE pData, DWORD szData )
{
	CFile file;
	if (!file.open(sFile, O_RDONLY)) {return false;}
	file.seek(SeekLoc, FS_SEEK_SET);
	DWORD dwReaded = file.read(pData, szData);
	// Ant, 14.05.09. Не было close().
	file.close();
	return (dwReaded == szData);
}
bool CFile::readAllFile( LPCTSTR sFile, LPBYTE pData)
{
	CFile file;
	DWORD szData;
	if (!file.open(sFile, O_RDONLY)) {return false;}
	szData = file.size();
	DBGLine("szData=%d",szData);
	DWORD dwReaded = file.read(pData, szData);
	// Ant, 14.05.09. Не было close().
	file.close();
	return (dwReaded == szData);
}
//-----------------------------------------------------------------------------/
void CFile::sync()
{
	fs_sync();
}
//-----------------------------------------------------------------------------/
void CFile::format()
{
	fs_format();
}
//-----------------------------------------------------------------------------/

//end

