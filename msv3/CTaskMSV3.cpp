
#include "CTaskMSV3.h"
#include "../extern.h"
#include "../internal/tools.h"
#include "../internal/string.h"
#include "acos.h"
#include "CGPRSConnection.h"

//------------------------------------------------
unsigned char	g_keys_ap3_auth[32];
DWORD			g_dwSelfNode;
//------------------------------------------------

CTaskMSV3::~CTaskMSV3()
{
	for (int i = 0; i < m_vecpServers.size(); ++i)
		DELETE_POINTER(m_vecpServers[i]);
}



void CTaskMSV3::__loadRASTable()
{
	ZM(m_stcConnStat);
	m_stcConnStat.ActSim = 1;

	SRASElement* pRASEl = new SRASElement;
	ZM(*pRASEl);

	pRASEl->nImsi	= 25002;
	strcpy(pRASEl->pAPN, "internet");

	addRASElement(pRASEl);
	DBGLine("RAS element added.. IMSI: %u, APN: %s, login: %s, password: %s", m_stcConnStat.vecpstcRASTable[0]->nImsi, m_stcConnStat.vecpstcRASTable[0]->pAPN, m_stcConnStat.vecpstcRASTable[0]->pLogin, m_stcConnStat.vecpstcRASTable[0]->pPassword);
}

void CTaskMSV3::__loadDNTTable()
{
	SConnInfo info;
	memset(&info, 0, sizeof(SConnInfo));

	int nServerNode			= 22;
	int nPort				= 1501;
	int nKeepAlive			= 45;
	int nPingAnswerMaxTime	= 90;

	info.wServerNode		= nServerNode;
	info.wPort				= nPort;
	info.wKeepAlive			= nKeepAlive;
	info.wPingAnswerMaxTime	= nPingAnswerMaxTime;
	strcpy(info.pIp, "87.118.199.10");

	DBGLine("Server added.. ServerNode: %u, ServerIP: %s, ServerPort: %u", info.wServerNode, info.pIp, info.wPort);


	m_vecpServers.push_back(new CGPRSConnection());
	m_vecpServers[0]->initialize(info);

	m_dwActServer = m_vecpServers.size() - 1;
}

void CTaskMSV3::start(CParam &oParam)
{
	m_dwActServer			= 0;
	m_dwCurBackupHost		= 1;
	m_dwCurHostIndx			= 1;
	m_dwSendBlockNumber		= 0;
	m_dwRcvBlockNumber		= 0;
	m_dwEncryptionType		= 0;
	m_dwNOfBytes			= 0;
	m_dwKeepAliveRcvTime	= 0;
	m_dwKeepAliveSentTime	= 0;
	m_bKeepAliveSent		= false;
	m_bMCPlaced				= false;
	m_bConnectionImposible	= false;
	m_bHaveTLV				= true;

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


	//==============================================================================================
	// 2.
	// Если есть тлв конфиг, забираем оттуда инфу
	if ((bool*)oParam.getParam(CParam::PARAM_NO_CFG, true))
		m_bHaveTLV = false;

	if (m_bHaveTLV)
	{
		//---загружаем RAS таблицу (если конфиг обновился, обновляем файлик)--------------
		__loadRASTable();
		//--------------------------------------------------------------------------------


		//---достаем из конфига номер ноды, проверяем с тем, что достали из криптокарты---
		//---(если ноды не совпали, дальше работу с девайсом продолжить невозможно)-------
#ifndef NO_CRYPTO_CARD
 		int nSelfNode		= g_pTlvParser->getPinPadNode();
		if ((DWORD)nSelfNode != g_dwSelfNode)
		{
			DBGLine("TLV node: %d, SAM node: %u", nSelfNode, g_dwSelfNode);
			setCriticalError(ASD::CRER_SELF_NODE, oParam);
			return;
		}
		g_dwSelfNode		= nSelfNode;
#endif
		//--------------------------------------------------------------------------------


		//---достаем из конфига таймаут аутентификации------------------------------------
		int nAuthTimeout	= 0;
		m_dwAuthTimeout		= nAuthTimeout > 0 ? nAuthTimeout : 60;
		DBGLine("MSV3 auth timeout: %u", m_dwAuthTimeout);
		//--------------------------------------------------------------------------------


		//---загружаем DNTтаблицу---------------------------------------------------------
		__loadDNTTable();
		//--------------------------------------------------------------------------------
	}
	else
	{
		m_dwAuthTimeout = 30;
	}

}

bool CTaskMSV3::connect(CParam& oParam, SConnInfo* stcConnInfo /* = 0 */)
{
	if (stcConnInfo)
	{
		// если есть stcConnInfo, то не подключаемся, а только забираем настройки!!!
		if (!oParam.getParam(CParam::PARAM_NO_CFG, true))
		{
			DBGLine("Error: CTaskMSV3::connect() with SConnInfo but without NO_CFG flag");
			return false;
		}


		m_vecpServers.push_back(new CGPRSConnection());
		m_vecpServers.front()->initialize(*stcConnInfo);

		manualRun();

		return true;
	}

	if (m_vecpServers[m_dwActServer]->isConnected())
	{
		DBGLine("Warning: CTaskMSV3 connect() - already connected");

		return true;
	}

	m_bConnectionImposible = false;	

	for (int i = 0; i < nTryConnectN; ++i)
	{
		m_dwActServer		= ++m_dwActServer % m_vecpServers.size();

		DBGLine("ActServer ind: %u, cur backup host ind: %u", m_dwActServer, m_dwCurBackupHost);

		if (m_vecpServers[m_dwActServer]->connect(m_stcConnStat, oParam))
		{
			DBGLine("connected");
			return true;

			if (checkCancelButtom(oParam))
				return false;			
		}
		else
		{
			DBGLine("Error: connect");
			if (checkCancelButtom(oParam))
				return false;			
		}
	}

	return false;
}


bool CTaskMSV3::processInternalMsg(CParam& oParam)
{
	return true;
}

int CTaskMSV3::run(CParam &oParam)
{
	do 
	{
		//==========================================================================================
		// 1.
		// Проверяем, запущена ли msv3
		if (m_bManual)
			break;
		//==========================================================================================


		//==========================================================================================
		// 2.
		// Что-то для конфигресивера
		
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
			break;
		}
		//==========================================================================================



		//==========================================================================================


		//==============================================================================================
		// 6.
		// Работаем с кипэлайвами
		if (m_bKeepAliveSent) // проверяем, может пора автодисконектиться
		{
			DWORD dwKeepaliveWaitMax = 1000 * m_vecpServers[m_dwActServer]->getPingAnswerMaxTime();

			if (IsExpired(m_dwKeepAliveRcvTime, dwKeepaliveWaitMax))
			{
				DBGLine("Error: run keep alive timeout");

				m_vecpServers[m_dwActServer]->disconnect(oParam);
			}
		}

		
		//==============================================================================================


		//==========================================================================================
		// 7.
		// Обрабатываем сообщение от сервера
		WORD wNeedBytes = (m_dwNOfBytes < 2)								// расчитываем размер данных
			? 2 - m_dwNOfBytes												// которые следует
			: Gmin(DUT_MSG_MAX_SIZE, 2 + *(WORD*)m_BufIn - m_dwNOfBytes);	// получить с сокета

		m_dwNOfBytes += m_vecpServers[m_dwActServer]->readData(m_BufIn + m_dwNOfBytes, wNeedBytes, oParam); // получаем данные с сокета (не более чем ровно на одно сообщение)

		if (m_dwNOfBytes < 2)
			break;

		WORD wMsgSize = *(WORD*)m_BufIn;

		if (wMsgSize == 0) // если получили keepalive..
		{
			// keepalive
			m_dwKeepAliveRcvTime = GetTickCount();
			m_bKeepAliveSent = false;

			// очищаем приёмный буфер
			m_dwNOfBytes = 0;

			DBGLine("keep alive received");
			break;
		}

		if (wMsgSize > DUT_MSG_MAX_SIZE) // неверный размер сообщения, разрываем подключение
		{
			DBGLine("Error: run bad msg size: %u", wMsgSize);

			

			m_dwNOfBytes = 0;
			m_vecpServers[m_dwActServer]->disconnect(oParam);
			break;
		}

		if (m_dwNOfBytes < 2 + wMsgSize) // сообщение принято не полностью
		{
		
			break;
		}

	

	
		m_dwNOfBytes = 0;
		//==========================================================================================
	} while (false);

	return 0;
}

void CTaskMSV3::stop(CParam &oParam)
{
}


#define DUTIFACE_MICROCONS				100

bool CTaskMSV3::checkCancelButtom(CParam& oParam)
{
	if (m_bHaveTLV)
	{
		bool bCanceled = m_vecpServers[m_dwActServer]->isCanceled();
		if (!bCanceled)
			oParam.readKey();
		char key = oParam.lastKey();
		if (m_bConnectionImposible || key == BTKEY_SFUNC || key == BTKEY_SHARP || key == BTKEY_ASTERISK)
		{
			DBGLine("CTaskMSV3: cancel pressed");
			m_bConnectionImposible = true;
			m_vecpServers[m_dwActServer]->disconnect(oParam);

			return true;
		}
	}

	Wait1MS(30);

	return false;
}





void CTaskMSV3::addRASElement(SRASElement* pRASElement)
{
	m_stcConnStat.vecpstcRASTable.push_back(pRASElement);
}

bool CTaskMSV3::getIMSI(int* nImsi)
{
	if (m_vecpServers.size() > 0)
		return m_vecpServers.front()->getIMSI(nImsi); //??????? может надо m_vecpServers[m_dwActServer]->... ??????
	return false;
}

bool CTaskMSV3::clearSMS(CParam& oParam)
{
	if (m_vecpServers.size() > 0)
	{
		m_vecpServers[m_dwActServer]->disconnect(oParam);
		return m_vecpServers[m_dwActServer]->clearSMS(oParam);
	}
	return false;
}
