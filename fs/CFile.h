#ifndef __CFILE_H
#define __CFILE_H

#include "../internal/string.h"
#include "../internal/tools.h"
#include "fs_flex.h"

class CFile
{
public:
	CFile();
	~CFile();

	void close();
	bool open(LPCTSTR sFile, DWORD dwMode = O_RDWR|O_CREAT, bool bLock = false);
	
	bool seek(DWORD dwOffset, DWORD dwOrigin = FS_SEEK_SET);
	DWORD write(BYTE* pData, DWORD szData);
	DWORD read(BYTE* pData, DWORD szData);
	
	DWORD size() const; 
	DWORD mode() const {return m_dwMode;}

	DWORD calcCrc32();

	
	const string& name() const {return m_strFileName;}
	bool fail() const {return (m_h == FS_INVALID_HANDLE);}
	bool isReadable() const {return ((m_dwMode&O_RDWR || m_dwMode&O_RDONLY) && !fail());}
	bool isWriteable() const {return ((m_dwMode&O_RDWR || m_dwMode&O_WRONLY) && !fail());}

	static bool remove(LPCTSTR sFile, bool bSync = false);
	static bool ls(std::list<string>& list);
	static void format();
	static void sync(); 

	// convenient functions
	static bool writeToFile(LPCTSTR sFile, LPBYTE pData, DWORD szData, bool bSync = false);
	static bool readFromFile(LPCTSTR sFile, LPBYTE pData, DWORD szData);
	static bool readAllFile( LPCTSTR sFile, LPBYTE pData);
	static bool readFromFileLoc(LPCTSTR sFile, DWORD SeekLoc, LPBYTE pData, DWORD szData);


	//Heba
	/*--------------------------------------------------------------------------
    ///search for the card number in the sorted file
	//<param><searchedCardNumber><the number to be searched for >
	///<return> true->if the Number in the file. else, false
	 * /*--------------------------------------------------------------------------
	 */
	bool searchForNumber(LPTSTR searchedCardNumber);
	/*--------------------------------------------------------------------------
	 *\Brief  Display the existed files with details
	 *\Param	N/A
	 *\Return   N/A
	 * --------------------------------------------------------------------------
	 */
	static void Dir(void);

private:
	DWORD m_h;
	DWORD m_dwMode;
	string m_strFileName;
	
	void _init();

};


#endif // __CFILE_H

// end

