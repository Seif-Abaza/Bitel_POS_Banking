
#ifndef __TOOLS_H
#define __TOOLS_H

#include "types.h"

//-----------------------------------------------------------------------------/
#define MAKEDWORD(l, h)     ((long)(((WORD)((DWORD)(l) & 0xffff)) | ((DWORD)((WORD)((DWORD)(h) & 0xffff))) << 16))
#define LOWORD(dw)           ((WORD)((DWORD)(dw) & 0xffff))
#define HIWORD(dw)           ((WORD)((DWORD)(dw) >> 16))

#define MAKEWORD(l, h)      ((WORD)(((BYTE)((DWORD)(l) & 0xff)) | ((WORD)((BYTE)((DWORD)(h) & 0xff))) << 8))
#define LOBYTE(w)           ((BYTE)((DWORD)(w) & 0xff))
#define HIBYTE(w)           ((BYTE)((DWORD)(w) >> 8))

#define ZM(stc) memset(&stc, 0, sizeof(stc))

#define Q(exp1, exp2, exp3) ((exp1)?(exp2):(exp3))
#define QTF(cond) Q(cond, true, false)

#ifndef min
#define min(X, Y) ((X) < (Y) ? (X) : (Y))
#endif

#ifndef max
#define max(X, Y) ((X) > (Y) ? (X) : (Y))
#endif

#define DELETE_POINTER(p) if (p) {delete p; p = 0;}
#define DELETE_POINTER_ARR(p) if (p) {delete[] p; p = 0;}

//-----------------------------------------------------------------------------/
#define AUTO_RELEASE(pIface) CIfaceAutoReleaser __releaseIface(pIface);
#define AUTO_DELETE(type, pPointer) TPointerKiller<type> __killPointer(pPointer, false);
#define AUTO_DELETE_ARR(type, pPointer) TPointerKiller<type> __killPointer_##pPointer(pPointer, true);
#define RELEASE(pIface) if (pIface) {pIface->release(); pIface = 0;}
#define ADDREF(pIface) if (pIface) {pIface->addRef();}
#define RELEASE_IUNK(pIface) if (pIface) {pIface->release();} pIface = NULL;
#define ADDREF_IUNK(pIface) if (pIface) {pIface->addRef();}

extern "C" int	Rprintf(const char *format, ...);
extern "C" void	RprintBuf(const char* bufName, const BYTE* buf, int nSize);


#ifdef MY_DEBUG
#define DBGPrt(...)		{ Rprintf(__VA_ARGS__); }
#else
#define DBGPrt(...)
#endif

#ifdef MY_DEBUG
#define DBGLine(...)	{ Rprintf(__VA_ARGS__); \
		Rprintf("\r\n"); }
#else
#define DBGLine(...)
#endif

#ifdef MY_DEBUG
#define DBGBuf(name, buf, nSize)	RprintBuf(name, buf, nSize)
#else
#define DBGBuf(name, buf, nSize)
#endif

DWORD	GetTickCount();
bool	IsExpired(DWORD dwStart, DWORD dwTimeout);
void	purgeChar(char* str, char ch);


//-----------------------------------------------------------------------------/
/// Подсчет CRC32 суммы
///
/// \param [out] Возвращает CRC32 сумму
/// \param [in] buffer данные
/// \param [in] len размер
/// \param [in] oldcrc32 предидущая CRC32 сумма, ели первый блок, то 0
DWORD	calcCRC32(const BYTE *buffer, DWORD len, DWORD oldcrc32);
ulong crc32(ulong crc1, const void *buf, size_t size);
unsigned long updcrc( unsigned char * s, unsigned n );


unsigned long crc32buffer(unsigned char *buffer, int len);

/// Подсчет CRC суммы
///
/// \param [out] Возвращает CRC сумму
/// \param [in] stream данные
/// \param [in] size размер
WORD	calcCRC(const BYTE *stream, int size);

BYTE calcLRC( BYTE *pBuf, int nLength );

#define CRC_INIT 0xFFFF
WORD calcCRC_SendFile(const LPBYTE message, ULONG length, WORD crc = CRC_INIT);

int convertFromHex( unsigned char hex );

WORD deltaTime(WORD wRef, WORD wCurrent);

void purge_char(char* str, char ch);
char* allocstrcpy(const char* src);

void bin2hexstr(unsigned char* src, char* dest, int len);
void bin2hexblk(BYTE* src, BYTE* dest, int len);
void hexblk2bin(BYTE* src, BYTE* dest, int len);
void newstrncpy(char * dest,char * src,int len);
void * memmem(const void *l, size_t l_len, const void *s, size_t s_len);
BYTE hexChar2num(char ch);
int hex2int(unsigned char hex);
unsigned long hex_long(unsigned char *ucIp,unsigned char len);
BYTE long_hex(unsigned char *ucDest,unsigned char len,unsigned long ulVal);

const int getLengthBCD(const int nLenSrc);
void str2BCDLeft(const BYTE* src, BYTE* dest, int len);
void str2BCDRight(const BYTE* src, BYTE* dest, int len);
void str2BinRight(const BYTE* src, BYTE* dest, int len);
void BCDLeft2str(const BYTE* src, BYTE* dest, int len);
void BCDRight2str(const BYTE* src, BYTE* dest, int len);
void vidIncrmntBcd(char* DstBcd, char* SrcBcd, ulong ulLen,ulong ulIncNo);
ulong bcd_to_long(char* SrcBcd, ulong ulLen);
void long_to_bcd(char* DstBcd,ulong ulSrc, ulong ulLen);

long days_from_civil(char* strYYYYMMDD);
void civil_from_days(long z, char* strYYYYMMDD);

int	 getDays(char *strYYYYMMDD);
int	 getDaysInMonth(char *strYYYYMMDD);
void getDate(char *strYYYYMMDD, bool bIsBCD);
void getDateTime(char* outDate, char* outTime,bool bIsBCD = false);

void getDDMMYY(char* outDDMMYY);
void getHHMM(char* outHHMM);
string getMounth(int nMonth,DWORD nlang);

void getDDMMYYHHMM(char* outDDMMYYHHMM);
void getTrmsnDateTime(char* outYYMMDDhhmm);
void getCaputeDate(char* outMMDD);
void getTime(char* outTime);
void getTrnLocalDateTime(char* outDateTime);
void getZZTime(char* outZZZZZHHmmss);
void addSecondsToCurrentTime(const char* seconds, char* outZZZZZHHmmss);
void setClock();

void makeDateTime(const char* inStr, char* outStr);
void makeDisplaySumm(const char* strIn, int nInLen, char* strOut);
void splitLeftZeros(const char* strIn, char* strOut);
int confirm(CParam& oParam);
int pressAnyButton(CParam& oParam);

void transformCheck(char* inCheck, char* outCheck);
bool extractCurrencyStr();
bool extractMoneyType(char* pTerminalNum);

unsigned int Length (const unsigned char s[]);
unsigned char WinToDos(unsigned char tok);
void WinToDos(unsigned char* str);

bool compareRegularStrings(const char *pStrExactly, const char *pStrRegular);
void setMessageMoneyType(const char* pStrMoneyType);

void showLogo();
void showVersion();
void showArabic(int first);
void changeMode();

bool useAutocancel(const char* strMTID, const char* strProcCode);


// возвращает указатель на начало следующего блока

bool checkMaskCardNumber(const char *strCardNumber, const char *strBegin, const char *strEnd);
bool checkExpireDate(char *strTrack2, char *strYYMM);
bool findCardInBinTable(const char* strCardNumber, char* outCardType, bool& outIsLoyal);
bool findCardInPinTable(bool& bPinNeedFind, bool& bPinNeedInput, const char* strCardType, const char* strMTID, const char* strProcCode);
bool findTermNumInCardTermTable(const char* strCardType, char* outTerminalNum);
bool findTermNumInTermNumTable(char* pIndexTMK, char* pBankId, int*& pEmvSection, int*& pHostType);
bool findCardTypeInFuelCards(const char* strCardType);


void resetTimer(CParam& oParam, DWORD& dwStartTime, DWORD& dwLastTime, DWORD dwTimeout);
void updateProgress(CParam& oParam, DWORD& dwLastTime, DWORD& dwTimeout, const DWORD dwPer = 1000);



template <typename T = void> class TPointerKiller
{
public:
	TPointerKiller(T* p, bool bArr) {m_p = p; m_bArr = bArr;}
	~TPointerKiller() {if (m_bArr) {DELETE_POINTER_ARR(m_p);} else {DELETE_POINTER(m_p);}}
private:
	T* m_p;
	bool m_bArr;
};

#endif
