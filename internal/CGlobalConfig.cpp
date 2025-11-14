
#include "CGlobalConfig.h"
#include "tools.h"
#include "CTLVParser.h"
#include "../extern.h"
#include "../fs/CFile.h"
#include "../internal/CParam.h"
#include "../tasks/CTaskDisplay.h"


const char ZMK[]	= "13AED5DA1F32347523C708C11F2608FD";

#define	LENGTH_SESSION_EXPIRE_TIME	11

//------------------------------------------------------------------------------
// File names
#define FNAME_TMK		"tmk.key"
#define FNAME_MAC		"mac.key"
#define FNAME_TID		"tid.key"
#define FNAME_INFO		"info.dat"
#define FNAME_CONFIG	"miniConf.mcf"



//------------------------------------------------------------------------------
// Card types
#define	CT_POS			6
#define CT_FMPC			'1'
#define CT_FPC			'2'
#define CT_F3C			'3'

CGlobalConfig*	g_pConfig;

//------------------------------------------------------------------------------
CGlobalConfig::CGlobalConfig()
{
	// Инициализация файлухи
	fs_init();

	g_pConfig			= this;
	g_pTlvParser = new CTLVParser;
	loadInfo();
	loadConfig();

	m_nCrc = crc32buffer((BYTE*)&(sInfo.SConnectInfo),sizeof(_SConnectInfo) );
	

}

//------------------------------------------------------
void CGlobalConfig::saveInfo()
{
	CTaskDisplay::setDisplay(D_SAVE);
	CFile oFile;  
	bool bFOpen = oFile.open(FNAME_INFO, O_WRONLY);
	if (!bFOpen)
	{
		bool bFOpen1 = oFile.open(FNAME_INFO, O_WRONLY| O_CREAT);	
		if (!bFOpen1)
		{
			DBGLine("Error: CGlobalConfig::store info - can't open file");
			return;
		}
	}
	oFile.write((BYTE*)&sInfo,sizeof(sInfo));
	oFile.close();

	CFile::sync();
}

void CGlobalConfig::loadConfig()
{
	DWORD csize = 0;
	DWORD cbegin = 0;
	CFile fFile;
	if (!fFile.open("config.tlv",O_RDONLY))
		{
			fFile.close(); 
			DBGLine ("No config file" );

			m_bConfigOk = false;
		} 
	fFile.close(); 
	int res =  g_pTlvParser->parseTLV(0,0,&csize,&cbegin);

	m_bConfigOk = true;
	

	if (res != 1)
	{	
		ClearLCDScreen();
		CTaskDisplay::setDisplay(D_NO_CONFIG);
		Wait1MS(1000);
		changeMode();

		m_bConfigOk = false;
		DBGLine("Error load config");

		int nPort				= 8002;
		int nKeepAlive			= 45;
		int nPingAnswerMaxTime	= 90;

		m_stcConnInfo.wServerNode		=  22;
		m_stcConnInfo.wPort				= 8002;
		m_stcConnInfo.wKeepAlive			= 45;
		m_stcConnInfo.wPingAnswerMaxTime	= 90;
		strcpy(m_stcConnInfo.pIp, "79.132.105.17");

		ZM(m_stcConnStat);
		m_stcConnStat.ActSim = 0;
		m_stcConnStat.SimStates[0] = 0;
		m_stcConnStat.SimStates[1] = 0;

		SRASElement* pRASEl = new SRASElement;
		ZM(*pRASEl);

		pRASEl->nImsi	= 25002;
		strcpy(pRASEl->pAPN, "internet");
		m_stcConnStat.vecpstcRASTable.push_back(pRASEl);

		return ;
	}	
	else
	{
		m_bConfigOk = true;
		DBGLine("load config");
		int idx;
	/*	//AdminNodesCount //////////////////////////////////////////////////////////////////////////
		int* pAdminNodesCount= new int;
		*pAdminNodesCount=g_pTlvParser->getAdminNodesCount();
		oFile.write((BYTE*)pAdminNodesCount,sizeof(int));

		//AdminNodesItems
		int* pAdminNodesItems= new int;
		for (i=0;i<*pAdminNodesCount;i++)
		{
		
		}
		delete pAdminNodesCount;
		delete pAdminNodesItems;
*/

/*		//HostNodesCount(HostGatesCount) //////////////////////////////////////////////////////////////////////////
		int* pHostNodesCount= new int;
		*pHostNodesCount=g_pTlvParser->getHostNodesCount();
*/
/*		//HostGatesItems
		int* pHostNodes= new int;
		int* pHostInterface = new int;
		for (i=0;i<*pHostNodesCount;i++)
		{
			if (!g_pTlvParser->getHostNodesItem(i,pHostNodes,pHostInterface))
			{
				oFile.write((BYTE*)pHostNodes,sizeof(int));
				oFile.write((BYTE*)pHostInterface,sizeof(int));
			} else return 1;
		}
		delete pHostNodesCount;
		delete pHostNodes;
		delete pHostInterface;
*/

/*		//HostsCount //////////////////////////////////////////////////////////////////////////
		int* pHostsCount= new int;
		*pHostsCount=g_pTlvParser->getHostsCount();
		oFile.write((BYTE*)pHostsCount,sizeof(int));

		//HostsData
		int* pmainGate= new int;
		int* pbackupGate1= new int;
		int* pbackupGate2= new int;
		int* pbackupGate3= new int;
		int* pbackupGate4= new int;
		for (i=0;i<*pHostsCount;i++)
		{
			if (!g_pTlvParser->getHostsData(i,pmainGate,pbackupGate1,pbackupGate2,pbackupGate3,pbackupGate4))
			{
				oFile.write((BYTE*)pmainGate,sizeof(int));
				oFile.write((BYTE*)pbackupGate1,sizeof(int));
				oFile.write((BYTE*)pbackupGate2,sizeof(int));
				oFile.write((BYTE*)pbackupGate3,sizeof(int));
				oFile.write((BYTE*)pbackupGate4,sizeof(int));
			} else return 1;
		}
		delete pmainGate;
		delete pbackupGate1;
		delete pbackupGate2;
		delete pbackupGate3;
		delete pbackupGate4;
		delete pHostsCount;
*/


	
		//RASCount//////////////////////////////////////////////////////////////////////////
		int* pRASCount= new int;
		*pRASCount=g_pTlvParser->getRASCount();
	

		//RASData
		m_stcConnStat.ActSim = 0;
		m_stcConnStat.SimStates[0] = 0;
		m_stcConnStat.SimStates[1] = 0;
	
	
		for (idx = 0; idx < *pRASCount; idx++)
		{
			SRASElement* pRASEl = new SRASElement;
			ZM(*pRASEl);
			static const int nBufSize = 33;
			int		nImsi				= 0;
			char	pLogin[nBufSize]	= {0};
			char	pPassword[nBufSize]	= {0};
			char	pAPN[nBufSize]		= {0};
			char	pGateway[17]		= {0};
			int		nAutoDisc			= 0;
			if (g_pTlvParser->getRASData(idx + 1, &nImsi, pLogin, pPassword, pAPN, pGateway, &nAutoDisc))
				{
					pRASEl->nAutoDisc=nAutoDisc;
					pRASEl->nImsi=nImsi;
					strcpy(pRASEl->pLogin,pLogin);
					strcpy(pRASEl->pPassword,pPassword);
					strcpy(pRASEl->pAPN,pAPN);
					strcpy(pRASEl->pGateway,pGateway);
					m_stcConnStat.vecpstcRASTable.push_back(pRASEl);
				}	

		}
		



		//DNTCount //////////////////////////////////////////////////////////////////////////
/*		int* pDNTCount= new int;
		*pDNTCount=g_pTlvParser->getDNTCount();

		//DNTData
		
		for (idx=0;idx<*pDNTCount;idx++)
		{
			
				int nServerNode			= 0;
				int nPort				= 0;
				int nKeepAlive			= 0;
				int nPingAnswerMaxTime	= 0;
				memset(m_stcConnInfo.pIp,0,sizeof(m_stcConnInfo.pIp));
			
				if (g_pTlvParser->getDNTData(idx + 1, &nServerNode, m_stcConnInfo.pIp, &nPort, &nKeepAlive, &nPingAnswerMaxTime))
				{	
					
					m_stcConnInfo.wServerNode		= nServerNode;
					m_stcConnInfo.wPort				= nPort;
					m_stcConnInfo.wKeepAlive			= nKeepAlive;
					m_stcConnInfo.wPingAnswerMaxTime	= nPingAnswerMaxTime;
				}

			}		
		
		delete pDNTCount;

		*/

		int nPort				= 8002;
		int nKeepAlive			= 45;
		int nPingAnswerMaxTime	= 90;

		m_stcConnInfo.wServerNode		=  22;
		m_stcConnInfo.wPort				= 8002;
		m_stcConnInfo.wKeepAlive			= 45;
		m_stcConnInfo.wPingAnswerMaxTime	= 90;
		strcpy(m_stcConnInfo.pIp, "79.132.105.17");


		if (sInfo.SConnectInfo.port!=0 && sInfo.SConnectInfo.ip[0]!=0  ) 
		{
			sprintf(m_stcConnInfo.pIp , "%d.%d.%d.%d", sInfo.SConnectInfo.ip[0], sInfo.SConnectInfo.ip[1], sInfo.SConnectInfo.ip[2], sInfo.SConnectInfo.ip[3]);
    		 m_stcConnInfo.wPort	= sInfo.SConnectInfo.port;	
		}
	}
	
}
void CGlobalConfig::loadInfo()
{
	CFile oFile;  
	memset(&sInfo,0,sizeof(_SInfo));
	bool bFOpen = oFile.open(FNAME_INFO, O_RDONLY);
	if (!bFOpen)
	{	

			DBGLine("Error: CGlobalConfig::load info - can't open file");
			return;		
	}
	oFile.read((BYTE*)&sInfo,sizeof(sInfo));
	oFile.close();

}

void	CGlobalConfig::storePort(const char* sPort)
{
	sInfo.SConnectInfo.port=atoi(sPort);

}

bool	CGlobalConfig::getPort(string& strPort)
{
	strPort=sInfo.SConnectInfo.port; 
	return true;
}

void	CGlobalConfig::storeCardInfo(	SPublicInfo stcPublicInfo,SCardHolderInfo  stcCardHolderInfo, int nCardType)
	{
		m_sPublicInfo[nCardType] = stcPublicInfo;
		m_sCardHolderInfo[nCardType] = stcCardHolderInfo;





	}
bool	CGlobalConfig::getCardInfo(	SPublicInfo& stcPublicInfo,SCardHolderInfo & stcCardHolderInfo, int nCardType)
	{
		stcPublicInfo = m_sPublicInfo[nCardType];
		stcCardHolderInfo = m_sCardHolderInfo[nCardType];

		


		return true;

	}
void	CGlobalConfig::storeBatch(const char* sBatch)
{
	sInfo.batch=atoi(sBatch);
		saveInfo();
}

bool	CGlobalConfig::getBatch(string& strBatch)
{
	strBatch = sInfo.batch; 
	return true;
}

bool	CGlobalConfig::isEncUse()
{
	return false;	
	return sInfo.use_enc;
}

void CGlobalConfig::storeUseMac(bool use)
{
	sInfo.use_mac = use;
	g_pParam->storeParam(CParam::PARAM_USE_MAC, new bool(use),true);
}

void CGlobalConfig::storeUseEnc(bool use)
{
	sInfo.use_enc = use;
	if (use) storeUseMac(false);
	g_pParam->storeParam(CParam::PARAM_USE_ENC, new bool(use),true);

}


bool	CGlobalConfig::isMacUse()
{
	
	return sInfo.use_mac;
}


void	CGlobalConfig::storeTermNumb(const char* sTermNumb)
{
	sInfo.termid=atoi(sTermNumb);
}

bool	CGlobalConfig::getTermNumb(string& strTermNumb)
{
	strTermNumb=sInfo.termid; 
	return true;
}

bool	CGlobalConfig::storeIp(const char* sIp)
{
	if (strlen(sIp)<10) return  false;
	char strBuf[4];
	int nBuf=0;

	for(int ind=0;ind<4;ind++)
	{
		memset(strBuf,0,4);
		memcpy(strBuf,sIp+ind*3,3);
		nBuf=atoi(strBuf);
		if (nBuf>255)	return false;
		else sInfo.SConnectInfo.ip[ind] = nBuf;
	}

	memset(m_stcConnInfo.pIp ,0 ,sizeof(m_stcConnInfo.pIp));
	sprintf(m_stcConnInfo.pIp , "%d.%d.%d.%d", sInfo.SConnectInfo.ip[0], sInfo.SConnectInfo.ip[1], sInfo.SConnectInfo.ip[2], sInfo.SConnectInfo.ip[3]);	


	return true;


}

bool	CGlobalConfig::getIp(string& strIp)
{
	char strBuf[13];
	sprintf(strBuf, "%.3d%.3d%.3d%.3d", sInfo.SConnectInfo.ip[0], sInfo.SConnectInfo.ip[1], sInfo.SConnectInfo.ip[2], sInfo.SConnectInfo.ip[3]);
	strIp = strBuf;

	return true;
}

void	CGlobalConfig::storeKey(string strKey1, string strKey2)
{
	memcpy(sInfo.key1 , strKey1.buffer(),strKey1.length()); 
	memcpy(sInfo.key2 , strKey2.buffer(),strKey1.length());
	saveInfo();

	return ;
}

bool	CGlobalConfig::getKey(string& strKey1, string& strKey2)
{
	m_strKey1 = string((char*)sInfo.key1, 8);
	m_strKey2 = string((char*)sInfo.key2, 8);
	strKey1 = m_strKey1;
	strKey2 = m_strKey2;
	return true;
}

bool	CGlobalConfig::canCon()
{
	DBGLine("CAN CON %d %d",sInfo.SConnectInfo.port,sInfo.SConnectInfo.ip[0]);


	

	if (sInfo.SConnectInfo.port==0 || sInfo.SConnectInfo.ip[0]==0  ) 
	{
		//DNTCount //////////////////////////////////////////////////////////////////////////
		int* pDNTCount= new int;
		*pDNTCount=g_pTlvParser->getDNTCount();

		//DNTData
		sInfo.termid = 900281;
		sInfo.batch = 931;

			
		for (int idx=0;idx<*pDNTCount;idx++)
		{

			int nServerNode			= 0;
			int nPort				= 0;
			int nKeepAlive			= 0;
			int nPingAnswerMaxTime	= 0;
			memset(m_stcConnInfo.pIp,0,sizeof(m_stcConnInfo.pIp));

			if (g_pTlvParser->getDNTData(idx + 1, &nServerNode, m_stcConnInfo.pIp, &nPort, &nKeepAlive, &nPingAnswerMaxTime))
			{	

				sInfo.SConnectInfo.port = nPort;

				if (strlen(m_stcConnInfo.pIp)<6) break;
				
				char strBuf[4]={0};
				int nBuf = 0,nPos = 0,nBlock = 0;
				for (int ind = 0;ind < strlen(m_stcConnInfo.pIp); ind++)
				{
					if (m_stcConnInfo.pIp[ind]<='9' && m_stcConnInfo.pIp[ind]>='0') 
					{
						strBuf[nPos++] = m_stcConnInfo.pIp[ind];
					}

					if (m_stcConnInfo.pIp[ind] == '.' ) 
					{
						nBuf = atoi(strBuf);
						memset(strBuf, 0 ,4);
						nPos = 0;
						sInfo.SConnectInfo.ip[nBlock++]=nBuf;
					}

				}
				nBuf = atoi(strBuf);
				sInfo.SConnectInfo.ip[nBlock++]=nBuf;
			}

		}
		saveInfo();
		delete pDNTCount;
	}
	


	if (sInfo.termid==0 || sInfo.batch==0  ) 
	{
		g_pParam->storeParam(CParam::PARAM_NO_CFG,new bool(true));
		return false;
	}
	g_pParam->storeParam(CParam::PARAM_NEED_CON,new bool(true));
	g_pParam->storeParam(CParam::PARAM_NO_CFG,new bool(false));

	return true;
}

bool	CGlobalConfig::change()
{
	int nCrc = crc32buffer((BYTE*)&(sInfo.SConnectInfo),sizeof(_SConnectInfo) ); 
	if (m_nCrc != nCrc)
		{
			m_nCrc=nCrc;
			return true;
		}
	return false;
}

bool	CGlobalConfig::needConfig()
{	
	g_pParam->storeParam(CParam::PARAM_USE_ENC,new bool(sInfo.use_enc),true);
	g_pParam->storeParam(CParam::PARAM_USE_MAC,new bool(sInfo.use_mac),true);

	return !m_bConfigOk;
}





//------------------------------------------------------------------------------
long long CGlobalConfig::_atoll(const string& strAmountLeft)
{
	char	pLeft[7]	= {0};
	char	pRight[7]	= {0};
	memcpy(pLeft, strAmountLeft.buffer(), 6);
	memcpy(pRight, strAmountLeft.buffer() + 6, 6);

	int	nLeft	= atoi(pLeft);
	int	nRight	= atoi(pRight);
	return (nRight + nLeft * 1000000);
}
