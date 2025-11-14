
#include "CTaskConnect.h"
#include "../extern.h"
#include "../internal/tools.h"
#include "../internal/string.h"
#include "CGPRSConnection.h"

//------------------------------------------------
unsigned char	g_keys_ap3_auth[32];
DWORD			g_dwSelfNode;
//------------------------------------------------

CTaskConnect::~CTaskConnect()
{
	for (int i = 0; i < m_vecpServers.size(); ++i)
		DELETE_POINTER(m_vecpServers[i]);
}



void CTaskConnect::__loadRASTable()
{
	
	ZM(m_stcConnStat);
	m_stcConnStat = g_pConfig->getSConnStat();
	DBGLine("RAS element added.. IMSI: %u, APN: %s, login: %s, password: %s", m_stcConnStat.vecpstcRASTable[0]->nImsi, m_stcConnStat.vecpstcRASTable[0]->pAPN, m_stcConnStat.vecpstcRASTable[0]->pLogin, m_stcConnStat.vecpstcRASTable[0]->pPassword);
}

void CTaskConnect::__loadDNTTable()
{
	SConnInfo info;
	memset(&info, 0, sizeof(SConnInfo));
	info=g_pConfig->getSConnInfo();
	DBGLine("Server added.. ServerNode: %u, ServerIP: %s, ServerPort: %u", info.wServerNode, info.pIp, info.wPort);

	m_vecpServers.push_back(new CGPRSConnection());
	m_vecpServers[0]->initialize(info);

	m_dwActServer = m_vecpServers.size() - 1;
}

void CTaskConnect::start(CParam &oParam)
{

	m_dwActServer			= 0;
	m_dwCurBackupHost		= 1;
	m_dwCurHostIndx			= 1;
	m_dwSendBlockNumber		= 0;
	m_dwRcvBlockNumber		= 0;
	m_dwEncryptionType		= 0;
	m_dwNOfBytes			= 0;
	m_dwoldNOfBytes			= 0;
	m_dwKeepAliveRcvTime	= 0;
	m_dwKeepAliveSentTime	= 0;
	m_bKeepAliveSent		= false;
	m_bMCPlaced				= false;
	m_bConnectionImposible	= false;
	m_bHaveTLV				= true;
	DBGLine("1");

	memset(m_dwKeysAp3Imito, 0, sizeof(m_dwKeysAp3Imito));
	memset(m_BufIn, 0, sizeof(m_BufIn));


#if defined(NO_CRYPTO_CARD)
	g_dwSelfNode = 50050;
	BYTE key[32] =
	{
		0xC8, 0x7C, 0xD9, 0x63, 0xD7, 0x3B, 0x5B, 0xE0,
		0x72, 0x9C, 0x3E, 0xF4, 0x09, 0xF1, 0x40, 0x7C,
		0xCC, 0xB2, 0x99, 0xB1, 0xAB, 0x80, 0x9F, 0x69,
		0x56, 0x6A, 0x6B, 0x7A, 0x3D, 0xF7, 0xC3, 0x2E,
	};
	memcpy(g_keys_ap3_auth, key, 32);
		DBGLine("2");
#else
	//==============================================================================================
	// 1.
	// Считываем инфу с криптокарты
	// (если считать не удалось, дальнейшую работу с девайсом продолжить нельзя)
	if (!__processSAM())	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	{
		setCriticalError(ASD::CRER_SAM, oParam);
		return;
	}
	//==============================================================================================
#endif

	DBGLine("3");

	// Если есть тлв конфиг, забираем оттуда инфу
	if ((bool*)oParam.getParam(CParam::PARAM_NO_CFG, true))
		m_bHaveTLV = false;

		DBGLine("3");
		//---загружаем RAS таблицу (если конфиг обновился, обновляем файлик)--------------
		__loadRASTable();
		//--------------------------------------------------------------------------------

			DBGLine("4");


		//---достаем из конфига таймаут аутентификации------------------------------------
		int nAuthTimeout	= 0;
		m_dwAuthTimeout		= nAuthTimeout > 0 ? nAuthTimeout : 60;
		DBGLine("MSV3 auth timeout: %u", m_dwAuthTimeout);
		//--------------------------------------------------------------------------------
			DBGLine("5");
		//---загружаем DNTтаблицу---------------------------------------------------------
		__loadDNTTable();
		//--------------------------------------------------------------------------------
	
	
}

bool CTaskConnect::connect(CParam& oParam, SConnInfo* stcConnInfo /* = 0 */)
{
	if (stcConnInfo)
	{
		// если есть stcConnInfo, то не подключаемся, а только забираем настройки!!!
	
		m_vecpServers.push_back(new CGPRSConnection());
		m_vecpServers.front()->initialize(*stcConnInfo);

		manualRun();

		return true;
	}

	if (m_vecpServers[m_dwActServer]->isConnected())
	{
		DBGLine("Warning: CTaskConnect connect() - already connected");

		return true;
	}
	CTaskDisplay::setDisplay(D_CONNECTING);

	m_bConnectionImposible = false;	

	for (int i = 0; i < nTryConnectN; ++i)
	{
		m_dwActServer		= ++m_dwActServer % m_vecpServers.size();

		DBGLine("ActServer ind: %u, cur backup host ind: %u", m_dwActServer, m_dwCurBackupHost);

		if (m_vecpServers[m_dwActServer]->connect(m_stcConnStat, oParam))
		{
			DBGLine("connected");
			g_pParam->storeParam(CParam::PARAM_CRITICAL_ERROR, new bool (false),true);
			return true;

			if (checkCancelButtom(oParam))
				return false;			
		}
		else
		{
			DBGLine("Error: connect");
			if (checkCancelButtom(oParam))
			{
				g_pParam->storeParam(CParam::PARAM_CRITICAL_ERROR, new bool (true),true);
				return false;	
			}
		}
	}


	return false;
}


bool CTaskConnect::processInternalMsg(CParam& oParam)
{
	
	BYTE* ptr = (BYTE*)oParam.getParam(CParam::PARAM_MSG_SEND, true);


	if (ptr)
	{
		if (ptr[0] == '+')
		{		
				while (!m_vecpServers[m_dwActServer]->openSocket(oParam))
				{ 
					for (int i = 0; i < 4; ++i)
					{
						if (m_vecpServers[m_dwActServer]->connect(m_stcConnStat, oParam))  break;
						char key = oParam.lastKey();
							bool bCanceled = m_vecpServers[m_dwActServer]->isCanceled();
						if (key == BTKEY_SFUNC || key == BTKEY_SHARP || key == BTKEY_ASTERISK || key == BTKEY_CANCEL ||bCanceled)
						{
							BYTE* pBuf = new BYTE[3];
							memset(pBuf,0,3 );
							memcpy(pBuf, "no", 2);
							oParam.eraseParam(CParam::PARAM_MSG_SEND, true);
							oParam.storeParam(CParam::PARAM_MSG_RECEIVE, pBuf, true);
							DBGLine("Socket cansel");
							return true;
						}
					}

				}
				BYTE* pBuf = new BYTE[3];
				memset(pBuf,0,3 );
				memcpy(pBuf, "ye", 2);
				oParam.eraseParam(CParam::PARAM_MSG_SEND, true);
				oParam.storeParam(CParam::PARAM_MSG_RECEIVE, pBuf, true);
				DBGLine("Socket open");
				return true;
		}
		if (ptr[0] == '-')
		{		
			m_vecpServers[m_dwActServer]->closeSocket(oParam);
			BYTE* pBuf = new BYTE[3];
			memset(pBuf,0,3 );
			memcpy(pBuf, "--", 2);
			oParam.eraseParam(CParam::PARAM_MSG_SEND, true);
			//oParam.storeParam(CParam::PARAM_MSG_RECEIVE, pBuf, true);
			DBGLine("Socket close");
			return true;
		}


		DBGLine("SEND 2");		
		BYTE	pOutBuf[2 + DUT_MSG_MAX_SIZE];
		memset(pOutBuf, 0, sizeof(pOutBuf));
		DWORD nlen = hex2int(ptr[0])*256 + hex2int(ptr[1]);
		memcpy(pOutBuf, ptr, nlen + 2);	

		// отправка
		char pC[] ={0x38, 0x31};

		DBGLine(" SEND size  %d",nlen + 2);
		
		m_vecpServers[m_dwActServer]->writeData(pOutBuf, nlen + 2);			

		DBGLine(" SEND size ok ");

		oParam.eraseParam(CParam::PARAM_MSG_SEND, true);
		m_dwNOfBytes = 0 ;


	}


	return false;
}


int CTaskConnect::run(CParam &oParam)
{
	do 
	{
		//==========================================================================================
		// 1.
		// Проверяем, запущена 
	
		if (m_bManual)
			break;
	
		//==========================================================================================
		bool * bCanCon=(bool*)g_pParam->getParam(CParam::PARAM_NO_CFG);
		if (!bCanCon || *bCanCon) break;

		bool * bNeedCon =(bool*)g_pParam->getParam(CParam::PARAM_NEED_CON);

		if (bNeedCon && *bNeedCon) 
		{
			m_bConnectionImposible = false;
			g_pParam->eraseParam(CParam::PARAM_NEED_CON);
		}


		//==========================================================================================
		// 2.
		// Что-то 
		
		if (m_bMCPlaced)
			m_bMCPlaced = false;
		//==========================================================================================


		//==========================================================================================
		// 3.
		// Если не подключены к серверу, конектимся
		if (!m_vecpServers[m_dwActServer]->isConnected() && !m_bConnectionImposible)
		{
			if (!connect(oParam))
				DBGLine("Critical Error: connection impossible!");
			return 1;
			break;
		}
		//==========================================================================================



		//==========================================================================================

		processInternalMsg(oParam);		
		//==============================================================================================


		//==========================================================================================
		// 7.
		// Обрабатываем сообщение от сервера

	/*	WORD wNeedBytes = (m_dwNOfBytes < 2)								// расчитываем размер данных
			? 2 - m_dwNOfBytes												// которые следует
			: min(DUT_MSG_MAX_SIZE, 2 + *(WORD*)m_BufIn - m_dwNOfBytes);	// получить с сокета
*/
		if (m_vecpServers[m_dwActServer]->isSocketOpen()) m_dwNOfBytes += m_vecpServers[m_dwActServer]->readData(m_BufIn + m_dwNOfBytes,DUT_MSG_MAX_SIZE, oParam); // получаем данные с сокета (не более чем ровно на одно сообщение)




		WORD wMsgSize  = hex2int(m_BufIn[0])*100 + hex2int(m_BufIn[1]);
		if (m_dwNOfBytes == 0)
			break;

		if ( m_dwNOfBytes <wMsgSize + 2 && m_dwoldNOfBytes != m_dwNOfBytes && m_dwNOfBytes!=0 )
		{
			char cRec[17] = {0};

			sprintf(cRec, "Get %d/%d byte",m_dwNOfBytes,wMsgSize + 2 );

			ClearLCDLine(2);	// Надо вызывать, иначе мусор на экран
			Lprint(2,0,cRec);
			m_dwoldNOfBytes = m_dwNOfBytes; 
			DBGLine("No recive %d YET, %d",m_dwNOfBytes, wMsgSize + 2);

		}

		if ( m_dwNOfBytes >wMsgSize + 2)
		{
			char cRec[17] = {0};

			sprintf(cRec, "Invalid message" );
			ClearLCDLine(2);	// Надо вызывать, иначе мусор на экран
			Lprint(2,0,cRec);
			DBGBuf("TRASH",m_BufIn,m_dwNOfBytes);
			m_dwNOfBytes = 0;

			break;

		}

		if ( m_dwNOfBytes == wMsgSize + 2  )
		{
			DBGLine("No recive %d  %d",m_dwNOfBytes, wMsgSize + 2);
			DBGBuf("RECIVE ",m_BufIn,m_dwNOfBytes);
			BYTE* pBuf = new BYTE[wMsgSize - 5];
			memcpy(pBuf, m_BufIn + 7, wMsgSize);
			oParam.storeParam(CParam::PARAM_MSG_RECEIVE, pBuf, true);
			oParam.storeParam(CParam::PARAM_MSG_RECEIVE_SIZE, new int(wMsgSize - 5),true);
			m_dwNOfBytes = 0;


			
		}

		//==========================================================================================
	} while (false);

	return 0;
}

void CTaskConnect::stop(CParam &oParam)
{
}


#define DUTIFACE_MICROCONS				100

bool CTaskConnect::checkCancelButtom(CParam& oParam)
{
	if (m_bHaveTLV)
	{
		bool bCanceled = m_vecpServers[m_dwActServer]->isCanceled();
		if (!bCanceled)
			oParam.readKey();
		char key = oParam.lastKey();
		if (m_bConnectionImposible || key == BTKEY_SFUNC || key == BTKEY_SHARP || key == BTKEY_ASTERISK || bCanceled)
		{
			DBGLine("CTaskConnect: cancel pressed");
			m_bConnectionImposible = true;
			m_vecpServers[m_dwActServer]->disconnect(oParam);
			CTaskDisplay::setDisplay(D_CONNECT_CANC);
			while( (KEYSCAN()&0xff) == 0xff );

			return true;
		}
	}

	Wait1MS(30);

	return false;
}





void CTaskConnect::addRASElement(SRASElement* pRASElement)
{
	m_stcConnStat.vecpstcRASTable.push_back(pRASElement);
}

bool CTaskConnect::getIMSI(int* nImsi)
{
	if (m_vecpServers.size() > 0)
		return m_vecpServers.front()->getIMSI(nImsi); //??????? может надо m_vecpServers[m_dwActServer]->... ??????
	return false;
}

bool CTaskConnect::clearSMS(CParam& oParam)
{
	if (m_vecpServers.size() > 0)
	{
		m_vecpServers[m_dwActServer]->disconnect(oParam);
		return m_vecpServers[m_dwActServer]->clearSMS(oParam);
	}
	return false;
}
