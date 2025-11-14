
#ifndef __CGLOBAL_CONFIG_H
#define __CGLOBAL_CONFIG_H

#include "types.h"
#include "string.h"
#include "../gprs/common.h"
#include "../card/CReaderICC.h"

extern	CGlobalConfig*	g_pConfig;

enum FCARD_TYPE
{
	FCARD_FMPC,
	FCARD_FPC,
	FCARD_F3C
};
struct _SConnectInfo
{
	DWORD port;
	DWORD ip[4];
};


struct _SInfo
	{
		int version;
		_SConnectInfo SConnectInfo;
		DWORD termid;
		DWORD batch;
		bool use_mac;
		bool use_enc;
		BYTE key1[8];
		BYTE key2[8];
};
class CGlobalConfig
{
public:
	CGlobalConfig();
	void	storeTermNumb(const char* sTermNumb);
	bool	getTermNumb(string& strTermNumb);

	void	storePort(const char* sPort);
	bool	getPort(string& strPort);

	bool	storeIp(const char* sIp);
	bool	getIp(string& strIp);

	void	storeBatch(const char* sBatch);
	bool	getBatch(string& strBatch);

	void storeUseMac(bool use);
	bool isMacUse();

	void storeUseEnc(bool use);
	bool isEncUse();

	void	storeKey(string strKey1, string strKey2);
	bool	getKey(string& strKey1, string& strKey2);


	void	storeCardInfo(	SPublicInfo stcPublicInfo,SCardHolderInfo  stcCardHolderInfo, int nCardType);
	bool	getCardInfo(	SPublicInfo& stcPublicInfo,SCardHolderInfo & stcCardHolderInfo, int nCardType);
	
	DWORD	getPinPadNode()	const	{ return 50050; }
	WORD	getPinPadItf()	const	{ return 95; }
	DWORD	getServerNode()	const	{ return 204; }
	WORD	getServerItf()	const	{ return 110;}

	bool	canCon();
	bool	change();
	bool	needConfig();

	SConnInfo& getSConnInfo() { return m_stcConnInfo;}
	SConnStat& getSConnStat() { return m_stcConnStat;}

	void saveInfo();


private:
	long long	_atoll(const string& strAmountLeft);

private:
	void loadInfo();

	void loadConfig();
	SPublicInfo          m_sPublicInfo[3];
	SCardHolderInfo     m_sCardHolderInfo[3];
	_SInfo		sInfo;
	int			m_nCrc;
	bool		m_bConfigOk;
	SConnInfo	m_stcConnInfo;
	SConnStat	m_stcConnStat;
	string		m_strKey1;
	string		m_strKey2;

};

#endif
