#ifndef CARABICSTRING_H
#define CARABICSTRING_H

#include "string.h"


class CArabicString
{
	public:
		CArabicString(const string &str);
		string rightToLeft(int length) const;
		~CArabicString();

	private:
		string m_strText;
};

#endif // CARABICSTRING_H
