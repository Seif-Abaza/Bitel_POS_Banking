
#ifndef	__FS_FLEX
#define	__FS_FLEX

#include "../internal/types.h"

#define FS_INVALID_HANDLE	0xFFFFFFFF
#define FS_MAX_PATH			32
#define FS_IO_ERROR			0xFFFFFFFF

#define FS_SEEK_SET			0
#define FS_SEEK_CUR			1
#define FS_SEEK_END			2


#define O_RDONLY    0x0001 /*Open for reading only        */
#define O_WRONLY    0x0002 /*Open for writing only        */
#define O_RDWR      0x0004 /*Open for reading and writing */
#define O_APPEND    0x0008 /*Append to end of file        */
#define O_CREAT     0x0010 /*Create and open file         */


void	fs_init();
void	fs_uninit();
DWORD	fs_open(const char *strFileName, WORD nMode, WORD nLock = 0);
void	fs_close(DWORD	nHandle);
bool	fs_remove(const char *strFileName);
DWORD	fs_read(DWORD nHandle, BYTE *pBuffer, DWORD nLength);
DWORD	fs_write(DWORD nHandle, BYTE *pBuffer, DWORD nLength);
DWORD	fs_size(DWORD nHandle);
DWORD	fs_lseek(DWORD nHandle, int nOffset, char nOrigin);
bool	fs_exists_size(const char *strFileName, DWORD *nSize);
DWORD	fs_tell(DWORD nHandle);
bool	fs_eof(DWORD nHandle);
int		fs_ls(char *strFiles);
void	fs_sync();
DWORD	fs_freeSpace();
void	fs_format();

#endif

