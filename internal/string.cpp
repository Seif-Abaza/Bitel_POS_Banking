
#include "../extern.h"
#include "tools.h"
#include "string.h"
#include "../tasks/CTaskDisplay.h"


#include <stdio.h>
#include <stdarg.h>
#include <string.h>



////////////////////////////////////////////////////////////////////////////////
#define NUMBER_SIZE  32
#define FORMAT_SIZE	 4096

bool string::m_bDisplayChanged = true;

//-----------------------------------------------------------------------------/
string::string(void) {_init();}
//-----------------------------------------------------------------------------/
string::string(LPCTSTR s, int nSize /* = 0 */)
{
	_init();
	append(s, nSize);
}
//-----------------------------------------------------------------------------/
string::string(const string& str)
{
	_init();
	append(str, str.m_len);
}
//-----------------------------------------------------------------------------/
string::string(double f)
{
	_init();
	TCHAR* sBuf = new TCHAR[NUMBER_SIZE];
	memset(sBuf, 0, sizeof(TCHAR)*NUMBER_SIZE);
	sprintf(sBuf, "%f", f);
	append(sBuf);
	delete[] sBuf; 
}
//-----------------------------------------------------------------------------/
string::string(int n)
{
	_init();
	TCHAR* sBuf = new TCHAR[NUMBER_SIZE];
	memset(sBuf, 0, sizeof(TCHAR)*NUMBER_SIZE);
	sprintf(sBuf, "%d", n);
	append(sBuf);
	delete[] sBuf; 

}
//-----------------------------------------------------------------------------/
string::string(unsigned int un)
{
	_init();
	TCHAR* sBuf = new TCHAR[NUMBER_SIZE];
	memset(sBuf, 0, sizeof(TCHAR)*NUMBER_SIZE);
	sprintf(sBuf, "%u", un);
	append(sBuf);
	delete[] sBuf; 
}
//-----------------------------------------------------------------------------/
string::string(unsigned int un, int nSize)
{
	_init();
	TCHAR* sBuf = new TCHAR[NUMBER_SIZE + nSize];
	memset(sBuf, 0, (sizeof(TCHAR)*NUMBER_SIZE)+ nSize);
	sprintf(sBuf, "%0*u",nSize, un);
	append(sBuf);
	delete[] sBuf;
}
//-----------------------------------------------------------------------------/
string::string(char c)
{
	_init();
	TCHAR* sBuf = new TCHAR[NUMBER_SIZE];
	memset(sBuf, 0, sizeof(TCHAR)*NUMBER_SIZE);
	sprintf(sBuf, "%c", c);
	append(sBuf);
	delete[] sBuf; 
}
//-----------------------------------------------------------------------------/
string::string(BYTE c)
{
	_init();
	TCHAR* sBuf = new TCHAR[NUMBER_SIZE];
	memset(sBuf, 0, sizeof(TCHAR)*NUMBER_SIZE);
	sprintf(sBuf, "%02X", c);
	append(sBuf);
	delete[] sBuf; 
}
//-----------------------------------------------------------------------------/
string::string( ULONG cn, char symbol )
{
	_init();
	for (int i=0; i < cn; i++)
	{*this + string(symbol);}
}
//-----------------------------------------------------------------------------/
string::~string(void) {clear();}
//-----------------------------------------------------------------------------/
void string::clear()
{
	DELETE_POINTER_ARR(m_sBuf);
	m_len = 0;
}
//-----------------------------------------------------------------------------/
string string::sub(ULONG pos, ULONG cnChars) const
{
	if (empty()) {return string();}

	if (pos < 1) {pos = 1;}
	if (pos > m_len) {pos = m_len;}

	ULONG cnMax = m_len - pos + 1;
	if (cnChars > cnMax) {cnChars = cnMax;}
	if (cnChars < 1 || pos < 1) {return string();}
	
	LPTSTR pBuf = new TCHAR[cnChars + 1];
	memset(pBuf, 0, sizeof(TCHAR)*(cnChars + 1));
	memcpy(pBuf, m_sBuf + (pos - 1), cnChars*sizeof(TCHAR));
	
	string res(pBuf);
	DELETE_POINTER_ARR(pBuf);
	return res;
}
//-----------------------------------------------------------------------------/
string string::left(ULONG cnChars) const {return sub(0, cnChars);}
//-----------------------------------------------------------------------------/
string string::right(ULONG cnChars) const {return sub(length() - cnChars + 1, cnChars);}
//-----------------------------------------------------------------------------/
bool string::insert(ULONG pos, LPCTSTR sAddon, int nSize /* = 0 */) throw() /*pos = 0..len */
{
	if (!sAddon) {return false;}
	ULONG lenAddon;
	if (nSize == 0)
		lenAddon = strlen(sAddon);
	else
		lenAddon = nSize;
	if (!lenAddon) {return false;}
	
	ULONG lenBuf = lenAddon + m_len;
	LPTSTR pBuf = new TCHAR[lenBuf + 1];
	memset(pBuf, 0, sizeof(TCHAR)*(lenBuf + 1));

	if (pos > m_len) {pos = m_len;}
	
	ULONG cnLeft = pos;
	ULONG cnRight = m_len - cnLeft;

	memcpy(pBuf, m_sBuf, cnLeft*sizeof(TCHAR));
	memcpy(pBuf + cnLeft, sAddon, lenAddon*sizeof(TCHAR));
	memcpy(pBuf + cnLeft + lenAddon, m_sBuf + cnLeft, cnRight*sizeof(TCHAR));

	m_len = lenBuf;
	DELETE_POINTER_ARR(m_sBuf);
	m_sBuf = pBuf;

	return true;
}
//-----------------------------------------------------------------------------/
void string::createFrom(LPCTSTR pSrcBuf, DWORD dwLen)
{
	clear();
	if (!pSrcBuf || dwLen < 1)
		return;

	m_sBuf	= new TCHAR[dwLen + 1];
	memcpy(m_sBuf, pSrcBuf, dwLen);
	m_sBuf[dwLen]	= 0;
	m_len = dwLen;
}
//-----------------------------------------------------------------------------/
bool string::prepend( LPCTSTR sAddon, int nSize /* = 0 */ ) throw() {return insert(0, sAddon, nSize);}
//-----------------------------------------------------------------------------/
bool string::append( LPCTSTR sAddon, int nSize /* = 0 */ ) throw() {return insert(m_len, sAddon, nSize);}
//-----------------------------------------------------------------------------/
void string::setSize(int nSize)
{
	if (length() < nSize)
	{
		char* normalBuf = new char[nSize + 1];
		memset(normalBuf, 0, nSize + 1);		
		memset(normalBuf, '0', nSize - length());
		memcpy(normalBuf + nSize - length(), buffer(), length());
		clear();
		append(normalBuf);
		DELETE_POINTER_ARR(normalBuf);
	} else if (length() > nSize)
	{
		char* normalBuf = new char[nSize + 1];
		memset(normalBuf, 0, nSize + 1);
		memcpy(normalBuf, buffer(), nSize);
		clear();
		append(normalBuf);
		DELETE_POINTER_ARR(normalBuf);
	}
}
//-----------------------------------------------------------------------------/
void string::setSizeRightSpaces(int nSize)
{
	if (length() < nSize)
	{
		char* normalBuf = new char[nSize + 1];
		memset(normalBuf, 0, nSize + 1);		
		memcpy(normalBuf, buffer(), length());		
		memset(normalBuf + length(), ' ', nSize - length());
		clear();
		append(normalBuf);
		DELETE_POINTER_ARR(normalBuf);
	} else if (length() > nSize)
	{
		char* normalBuf = new char[nSize + 1];
		memset(normalBuf, 0, nSize + 1);
		memcpy(normalBuf, buffer(), nSize);
		clear();
		append(normalBuf);
		DELETE_POINTER_ARR(normalBuf);
	}
}
//-----------------------------------------------------------------------------/
string& string::format( LPCTSTR sTempl, ... )
{
	LPTSTR sBuf = new TCHAR[FORMAT_SIZE];	AUTO_DELETE_ARR(TCHAR, sBuf);
	memset(sBuf, 0, FORMAT_SIZE*sizeof(TCHAR));

	va_list arg;
	va_start(arg, sTempl);
	vsprintf(sBuf, sTempl, arg);

	clear();
	append(sBuf);

	return *this;
}
//-----------------------------------------------------------------------------/
void string::out(int y, int x) {if (m_sBuf) {out(y,x,m_sBuf);}}
//-----------------------------------------------------------------------------/
void string::outC(int y, LPCTSTR sTempl, ...)
{
	LPTSTR sBuf = new TCHAR[FORMAT_SIZE];
	memset(sBuf, 0, FORMAT_SIZE*sizeof(TCHAR));

	va_list arg;
	va_start(arg, sTempl);
	vsprintf(sBuf, sTempl, arg);

	int x = (DISPLAY_WIDTH - strlen(sBuf)) / 2;

	Lprint(y, x, sBuf);

	delete[] sBuf;

	m_bDisplayChanged = true;;
}
//-----------------------------------------------------------------------------/
void string::outR(int y, LPCTSTR sTempl, ...)
{
	LPTSTR sBuf = new TCHAR[FORMAT_SIZE];
	memset(sBuf, 0, FORMAT_SIZE * sizeof(TCHAR));

	va_list arg;
	va_start(arg, sTempl);
	vsprintf(sBuf, sTempl, arg);

	int x = DISPLAY_WIDTH - strlen(sBuf);

	Lprint(y, x, sBuf);

	delete[] sBuf;

	m_bDisplayChanged = true;
}
//-----------------------------------------------------------------------------/
void string::outC(int y, int x, LPCTSTR sTempl, ...)
{
	outC(y, sTempl);
}
//-----------------------------------------------------------------------------/
void string::out(int y, int x, LPCTSTR sTempl, ...)
{
	LPTSTR sBuf = new TCHAR[FORMAT_SIZE];
	memset(sBuf, 0, FORMAT_SIZE*sizeof(TCHAR));

	va_list arg;
	va_start(arg, sTempl);
	vsprintf(sBuf, sTempl, arg);
	
	Lprint(y,x, sBuf);

	delete[] sBuf;

	m_bDisplayChanged = true;
}
//-----------------------------------------------------------------------------/
bool string::compare(LPCTSTR s, bool bCase /* = true */) const
{
	if (m_sBuf == s) {return true;}
	if (!s || !m_sBuf) {return false;}

	if (bCase) {return  strcmp(m_sBuf, s) == 0;}
	else {return stricmp(m_sBuf, s) == 0;}
}
//-----------------------------------------------------------------------------/
string& string::operator= (LPCTSTR s) {clear(); append(s); return *this;}
//-----------------------------------------------------------------------------/
string& string::operator= (const string& str) {clear(); append(str, str.m_len); return *this;}
//-----------------------------------------------------------------------------/
string string::operator+(LPCTSTR s) const {string strCopy(*this); strCopy.append(s); return strCopy;}
//-----------------------------------------------------------------------------/
string string::operator+( const string& str ) const {string strCopy(*this); strCopy.append((LPCTSTR)str, str.m_len); return strCopy;}
//-----------------------------------------------------------------------------/
string& string::operator+=(LPCTSTR s) {append(s); return *this;}
//-----------------------------------------------------------------------------/
string& string::operator+=( const string& str ) {append(str, str.m_len); return *this;}
//-----------------------------------------------------------------------------/
bool string::operator==(const string& str) const {return compare(str);}
//-----------------------------------------------------------------------------/
bool string::operator != (const string& str) const {return !(*this == str);}
//-----------------------------------------------------------------------------/
bool string::operator < (const string& str) const
{
	if (m_len < str.m_len)
		return true;
	else if (m_len > str.m_len)
		return false;
	return strcmp(buffer(), str.buffer()) < 0;
}
//-----------------------------------------------------------------------------/
bool string::operator==(LPCTSTR s) const {return compare(s);}
//-----------------------------------------------------------------------------/
bool string::operator != (LPCTSTR s) const {return !(*this == s);}
//-----------------------------------------------------------------------------/
TCHAR string::operator[]( int idx ) const {if (idx < 0 || idx >= m_len || !m_sBuf) {return '\0';} return m_sBuf[idx];}
//-----------------------------------------------------------------------------/
bool string::isBeginAt(LPCTSTR s, bool bCase /* = true */) const
{
	if (!s) {return false;}
	
	string str(s);
	if (str.empty() || length() < str.length()) {return false;}

	return left(str.length()).compare(str, bCase);
}
//-----------------------------------------------------------------------------/
ULONG string::trimLeft()
{
	if (empty()) {return 0;}

	ULONG cn = 0;
	for (int i=0; i < m_len; i++)
	{
		if (strchr(SPACE_SYMBOLS, m_sBuf[i])) {cn++;} 
		else {break;}
	}

	if (cn > 0) {*this = right(length() - cn);}
	
	return cn;
}
//-----------------------------------------------------------------------------/
ULONG string::trimRight()
{
	if (empty()) {return 0;}

	ULONG cn = 0;
	for (int i = m_len - 1; i > -1; i--)
	{
		if (strchr(SPACE_SYMBOLS, m_sBuf[i])) {cn++;} 
		else {break;}
	}

	if (cn > 0) {*this = left(length() - cn);}

	return cn;
}
//-----------------------------------------------------------------------------/
ULONG string::trim() {return trimLeft() + trimRight();}
//-----------------------------------------------------------------------------/
ULONG string::tokenize(std::list<string>& list, LPCTSTR sTokens) const
{
	if (empty()) {return 0;}

	int posFirstSymbol = 1;
	bool bLastToken = true;
	for (int i=0; i < m_len; i++)
	{
		if (strchr(sTokens, m_sBuf[i]))
		{
			if (!bLastToken)
			{
				list.push_back(sub(posFirstSymbol, i + 1 - posFirstSymbol));	
			}

			posFirstSymbol = i + 2;
			bLastToken = true;
		}
		else
		{
			bLastToken = false;
		}
	}

	if (!bLastToken)
	{
		list.push_back(sub(posFirstSymbol, length() - posFirstSymbol + 1));	
	}

	return list.size();
}
//-----------------------------------------------------------------------------/
void string::makeHexDump( LPBYTE pData, DWORD szData )
{
	format("hex %u: ", szData); 

	for (int i=0; i < szData; i++) {*this + string(*(pData + i));}
}
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
string operator+( LPCTSTR s, string& str )
{
	string strCopy(str);
	strCopy.prepend(s);
	return strCopy;
}
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
char rtoupper(unsigned char ch)
{
	int val;

	val=ch;
	if ((val>='a' && val<='z') || (val>=0xA0 && val<=0xAF))
		val-=0x20;
	else if (val>=0xE0 && val<=0xEF)
		val-=0x50;
	else if (val==0xF1)
		val=0xF0;
	return(val);
}
//-----------------------------------------------------------------------------/
char rtolower(unsigned char ch)
{
	int val;

	val=ch;

	if ((val>='A' && val<='Z') || (val>=0x80 && val<=0x8F))
		val+=0x20;
	else if (val>=0x90 && val<=0x9F)
		val+=0x50;
	else if (val==0xF0)
		val=0xF1;
	return(val);
}
//-----------------------------------------------------------------------------/
void strtoupper(char* str)
{
	int i;              


	for(i=0;str[i]!=0;i++)
	{
		str[i]=rtoupper((BYTE)str[i]);
	}
}
//-----------------------------------------------------------------------------/
void strtolower(char* str)
{
	int i;
	for(i=0;str[i]!=0;i++)
	{
		str[i]=rtolower((BYTE)str[i]);
	}
}
//-----------------------------------------------------------------------------/
int stricmp( const char* s1, const char* s2 )
{
	if (s1 == s2) {return 0;}
	if (!s1 || !s2) {return 1;}
	
	const size_t l1 = strlen(s1);
	const size_t l2 = strlen(s1);

	char* st1 = new char[l1 + 1];
	char* st2 = new char[l2 + 1];
	memcpy(st1, s1, l1 + 1);
	memcpy(st2, s2, l2 + 1);
	
	strtolower(st1);
	strtolower(st2);

	int res = strcmp(st1, st2);
	DELETE_POINTER_ARR(st1);
	DELETE_POINTER_ARR(st2);

	return res;
}
//-----------------------------------------------------------------------------/
bool string::Comparator::operator() (const string& str1, const string& str2)
{
	if (str1 == str2) {return false;}
	return strcmp(str2, str1) >  0;
}
//-----------------------------------------------------------------------------/
void skipsymbols(const char* src, string& dst, const char* symbols)
{
	int		size = strlen(src);
	char*	pBuf = new char[size + 1];
	memset(pBuf, 0, size + 1);
	for (int i = 0, j = 0; i < size; ++i)
	{
		bool bFound = false;
		for (int k = 0; k < strlen(symbols); ++k)
		{
			if (src[i] == symbols[k])
			{
				bFound = true;
				break;
			}
		}

		if (!bFound)
			pBuf[j++] = src[i];
	}

	dst = pBuf;
	DELETE_POINTER_ARR(pBuf);
}
//-----------------------------------------------------------------------------/
////////////////////////////////////////////////////////////////////////////////

//end
