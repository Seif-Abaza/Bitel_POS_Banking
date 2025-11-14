#ifndef CPINPAD_H
#define CPINPAD_H

#include "../../internal/types.h"


class CPinPad
{
	static const int m_cnBufSize = 2048;

public:
	CPinPad();
	~CPinPad(void);
	enum RCODE{
		PIN_TEST_FAIL = -9,
		PIN_MK3DES_FAIL,
		PIN_MK_SELECT_FAIL,
		PIN_SEND_UNK_CMD,
		PIN_SEND_ZERO_REPLY,
		PIN_TIMEOUT,
		PIN_MK_WRONG_LEN,
		PIN_MK_SET_FAIL,
		PIN_FAIL,
		PIN_OK,
		PIN_MK_EXIST,
		PIN_MK_EMPTY};
	enum PIN_CMD{
		PINPAD_CMD_TEST_CONN = 0,
		PINPAD_CMD_SET_3DES,
		PINPAD_CMD_SEL_MK,
		PINPAD_CMD_CHK_MK,
		PINPAD_CMD_CHK_MK_DATA,
		PINPAD_CMD_SET_MK,
		PINPAD_CMD_GET_PIN,
		PINPAD_CMD_GET_STATE,
		PINPAD_CMD_SET_SINGLEDES
	};

	RCODE setMasterKey(int nIdMK, const char *pMKey );
	RCODE checkMasterKey(int nIdMK);
	RCODE getPin(unsigned char nTMKIndex, int nTryCount, const char *strCardNumber, const char *strTPK, char *strPinBlock);
	RCODE testConnection();
	RCODE getMasterKey(int mkid, BYTE* bf);

	void	setLine(DWORD dwLine);

private:
	RCODE open(int nIdMK, int nLine = 3);
	RCODE send(PIN_CMD eCmd, const char *pData);
	int read(int nTimeout);
	CPinPad::RCODE readBuf(BYTE *pBuf, int nSize, int nTimeout);
	int LRC7Calc( BYTE *pStream, int nSize );
	int LRCCalc( BYTE *pStream, int nSize );
	void emptyBuf();

private:
	DWORD	m_dwLine;
	BYTE	m_pRxDBUF[m_cnBufSize];

};

#endif
