#ifndef __STRING_H_
#define __STRING_H_

#include "types.h"

#define SPACE_SYMBOLS "\n\t "

class string
{
public:
	string();
	~string();
	string(LPCTSTR, int nSize = 0);
	string(const string&);
	string(ULONG cn, char symbol); // fill
		
	string(double);
	string(unsigned int);
	string(unsigned int, int);
	string(int);
	string(char c);
	string(BYTE c);

	void createFrom(LPCTSTR pSrcBuf, DWORD dwLen);
	void clear();
	bool empty() const {return !m_sBuf;}
	ULONG length() const {return m_len;}

	operator LPCTSTR() const {return buffer();}
	LPCTSTR buffer() const {return m_sBuf;}
	LPTSTR buffer() {return m_sBuf;}

	// return count of trimmed characters
	ULONG trim(); 
	ULONG trimLeft();
	ULONG trimRight();

	int toInt() const {if (!m_sBuf) {return 0;} return atoi(m_sBuf);}
	long  toLong() const {if (!m_sBuf) {return 0;} return atol(m_sBuf);}
	double toDouble() const {if (!m_sBuf) {return 0;} return atof(m_sBuf);}


	bool operator != (LPCTSTR) const;	
	bool operator == (LPCTSTR) const;	
	bool operator != (const string&) const;	
	bool operator == (const string&) const;	
	operator bool() const {return !empty();}
	bool operator < (const string&) const;	

	
	string& operator= (LPCTSTR s);
	string operator+ (LPCTSTR s) const;
	string& operator+= (LPCTSTR s);

		
	string& operator= (const string&);
	string operator+ (const string& str) const;
	string& operator+= (const string& str);

	TCHAR operator[] (int idx) const;
	
	friend string operator+ (LPCTSTR s, string& str);
		
	void toLower();
	void toUpper();

	// all pos must be in numbers of characters
	
	string& format(LPCTSTR sTempl, ...);
	bool insert(ULONG pos, LPCTSTR pszStr, int nSize = 0) throw();
	bool prepend(LPCTSTR sAddon, int nSize = 0) throw();
	bool append(LPCTSTR sAddon, int nSize = 0) throw();

	void setSize(int nSize);
	void setSizeRightSpaces(int nSize);

	string left(ULONG cnChars) const;
	string right(ULONG cnChars) const;
	string sub(ULONG pos, ULONG cnChars) const;

	bool compare(LPCTSTR s, bool bCase = true) const;
	bool isBeginAt(LPCTSTR s, bool bCase = true) const;

	void makeHexDump(LPBYTE pData, DWORD szData);

	ULONG tokenize(std::list<string>& list, LPCTSTR sTokens = SPACE_SYMBOLS) const;

	void out(int y, int x);
	static void outC(int y, LPCTSTR sTempl, ...); // выставляет m_bDisplayChanged в true (для нормального функ-ия меню все выводить ей)
	static void outC(int y, int x, LPCTSTR sTempl, ...); //? добавил, чтобы каждый Lprint не изменять (не пользоваться)
	static void outR(int y, LPCTSTR sTempl, ...);
	static void out(int y, int x, LPCTSTR sTempl, ...);

	static void setDisplayChangedTrue() { m_bDisplayChanged = true; }
	static void setDisplayChangedFalse() { m_bDisplayChanged = false; }
	static bool isDisplayChanged() { return m_bDisplayChanged; }
	
	

	/*friend string&     operator << ( string &str, LPCTSTR pszStr ) throw();
	friend string&     operator << ( string &str, int iValue ) throw();
	friend string&     operator << ( string &str, long lValue ) throw();
	friend string&     operator << ( string &str, unsigned long ulValue ) throw();
	friend string&     operator << ( string &str, double dValue ) throw();*/

	// functional object used for build of database of recived solutions
	class Comparator {public: bool operator() (const string& str1, const string& str2);};

private:
	void _init() {m_sBuf = 0; m_len = 0;}

	LPTSTR m_sBuf;
	ULONG m_len;

	static bool m_bDisplayChanged;
};

string operator+ (LPCTSTR s, string& str);

bool stringCompare(const string& str1, const string& str2);

/*
string& operator << (string &str, LPCTSTR pszStr) throw();
string& operator << (string &str, int iValue) throw();
string& operator << (string &str, long lValue) throw();
string& operator << (string &str, unsigned long ulValue) throw();
string& operator << (string &str, double dValue) throw();*/

int stricmp(const char* s1, const char* s2);
char rtoupper(unsigned char ch);
char rtolower(unsigned char ch);
void strtoupper(char* str);
void strtolower(char* str);
void skipsymbols(const char* src, string& dst, const char* symbols);

#endif

// end

