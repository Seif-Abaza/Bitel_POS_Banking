//---------------------------------------------------------------------------
/****************************************************************************
* История изменений:
* $Log: types.h,v $
* Revision 1.3  2010/06/09 06:04:44  rakhmatulin
* no message
*
*
***************************************************************************/
//---------------------------------------------------------------------------
#ifndef	___TYPES_H__
#define	___TYPES_H__

#include <map>
#include <list>
#include <vector>
#include <utility>
#include "../build_number.h"

#include "cfg.h"

#define BEEP_ERROR_TONE ERROR_TONE2

#ifndef UINT
    #define UINT unsigned int   /* UINT  defines two/four byte word       */
#endif
#ifndef BYTE
    #define BYTE unsigned char  /* BYTE  defines a unsigned_char          */
#endif
//typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned int	DWORD;
typedef unsigned long   ULONG;
//typedef unsigned int	UINT;
typedef BYTE*			LPBYTE;

typedef char TCHAR;
typedef TCHAR* LPTSTR;
typedef const TCHAR* LPCTSTR;

class string;
class CParam;
class CGlobalConfig;
class CMegaJournal;

typedef std::vector<string>	tVecStrings;

#endif
