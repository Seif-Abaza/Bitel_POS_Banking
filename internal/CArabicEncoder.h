#ifndef CARABICENCODER_H
#define CARABICENCODER_H

#include "string.h"

class CArabicEncoder
{
	public:
	CArabicEncoder(const string &text);
	~CArabicEncoder();
	string CArabicEncoder::encode() const;

	private:
	string m_strText;
	int getPosition(int n) const;
	/*	unsigned char getEncodedSymbol(unsigned char ch, int pos)const;*/
};

#endif // CARABICENCODER_H
