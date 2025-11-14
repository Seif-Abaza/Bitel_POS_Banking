
#include "../extern.h"
#include "CGPRSConnection.h"
#include "CTaskConnect.h"
#include "../internal/string.h"

extern CTaskConnect* g_pConnTask;

bool	CGPRSConnection::m_bInitialized	= false;
DWORD	CGPRSConnection::m_dwDiscCount	= 0;
DWORD	CGPRSConnection::m_dwSmsCount	= 0;
DWORD	CGPRSConnection::m_dwLastSent	= 0;
DWORD	CGPRSConnection::m_dwLastRecv	= 0;
BYTE	CGPRSConnection::m_CSQ			= 0;
char	CGPRSConnection::m_pOperatorName[DISPLAY_WIDTH + 1];



void startHypTerm()
{
	while (g_pParam->lastKey() != BTKEY_CANCEL)
	{
		while (CheckRxInput(COM0))
		{
			char c = RdCom(COM0);
			WaitTxDone(COM0);
			WrCom(COM1, &c, 1);
			WaitTxDone(COM1);
		}

		while (CheckRxInput(COM1))
		{
			char c = RdCom(COM1);
			WaitTxDone(COM1);
			WrCom(COM0, &c, 1);
			WaitTxDone(COM0);
		}
		g_pParam->readKey();
	}
}

CGPRSConnection::CGPRSConnection()
	: m_bConnected(false)
	, m_dwLastCall(0)
	, m_wHelpBytesCount(0)
	, m_bIsCanceled(false)
	, m_nActRasInd(0)
	, m_bSocketOpen(false)
	, m_bOldModem(false)
{
	memset(&m_stcConnInfo, 0, sizeof(SConnInfo));
	memset(m_HelpBuf, 0, sizeof(m_HelpBuf));
}

CGPRSConnection::~CGPRSConnection()
{
	m_bConnected = false;
}

void CGPRSConnection::disconnect(CParam& oParam)
{	
	DBGLine("disconnect");

	m_bConnected = false;
	++m_dwDiscCount;
	oParam.eraseParam(CParam::PARAM_SERVER_CONNECTED, true);
}

bool CGPRSConnection::initialize(const SConnInfo& stcConnInfo)
{
	//"079.132.098.251"	

	memcpy(&m_stcConnInfo, &stcConnInfo, sizeof(SConnInfo));

	if (!m_bInitialized)
	{
		DBGLine("GPRS initializing..");

		CONTROL_POWER_SAVING_STATUS(2, 1);
		Wait1MS(1500);
// 		MODEM_SEL();
// 		DTR_HIGH();

		SET_BUFFER_SIZE_8K();
		OpenRS232Port(COM_GPRS, BPS115200, NONE8BIT, 1);

		m_bInitialized = true;
	}

	return true;
}

bool CGPRSConnection::getIMSI(int* nImsi)
{
	char *p;
	char	answer_buf[100];
	memset(answer_buf, 0, sizeof(answer_buf));

	if (!atPosfixExecute(2000, "OK", answer_buf, sizeof(answer_buf), "AT+CIMI\r\n")) 
	{
		DBGLine("Error: cannot get IMSI");
		return false;
	}

	char pBuf[6];
	memset(pBuf, 0, 6);
	memcpy(pBuf, answer_buf, 5);

	*nImsi = atoi(pBuf);

	return true;
}

BYTE CGPRSConnection::_getCSQ(CParam& oParam)
{
	char	answer_buf[100];
	memset(answer_buf, 0, sizeof(answer_buf));

	for (int i = 0; i < 5; ++i)
	{
		if (atExecute(oParam,1000, "\n+CSQ: ", answer_buf, sizeof(answer_buf), "AT+CSQ\r\n"))
			return atoi(answer_buf + 7);
		SLEEP(1000);
	}

	return 0;
}

bool CGPRSConnection::atExecute(CParam& oParam, DWORD timeout, char* prefix, char* ans_buf, WORD ans_buf_size, char* cmd, ...)
{
	Wait1MS(30);

	m_dwLastCall = GetTickCount();

	char	tx_buf[AT_CMD_BUF_SIZE];
	char	rx_buf[AT_ANS_BUF_SIZE];
	WORD	rx_cnt = 0;
	int		ii;

	memset(tx_buf, 0, AT_CMD_BUF_SIZE);
	memset(rx_buf, 0, AT_ANS_BUF_SIZE);

	// форматирование AT команды
	va_list arguments;
	va_start(arguments, cmd);
	vsprintf(tx_buf, cmd, arguments);
	va_end(arguments);

	// старые данные вычитываем
	while (CheckRxInput(COM_GPRS))
		RdCom(COM_GPRS);
	//ClearRS232Buffer(COM_GPRS);
	DBGLine("%s",tx_buf);

	// передача команды
	WrCom(COM_GPRS, tx_buf, strlen(tx_buf));
	WaitTxDone(COM_GPRS);

	// требуется ответ модема? если нет, то выход.
	if (!prefix)
		return true;

	// в течение отведённого времени ожидание ответа модема
	DWORD	tick_start = GetTickCount();
	char	*str_prefix = NULL;
	char	*str_eol = NULL;
	char	*str_eol_2 = NULL;
	char	*str_cme_error = NULL;
	while (!IsExpired(tick_start, timeout))
	{
		if (checkCancelButtom(oParam))
			return false;

		if (CheckRxInput(COM_GPRS))
		{
			rx_buf[rx_cnt++] = RdCom(COM_GPRS); // чтение 1 байта
		//	DBGLine("REad buffer %s",rx_buf);

			// поиск префикса желаемого ответа
			str_prefix = strstr(rx_buf, prefix);
			if (str_prefix)
			{
				// поиск перевода строки
				str_eol = strstr(str_prefix, "\r\n");
				if (str_eol)
				{
					// ответ модема получен
					if (ans_buf)
					{
						// копируем ответ модема в выходной буфер
						strncpy(ans_buf, str_prefix, Gmin(str_eol - str_prefix + 2, ans_buf_size - 1));
					}
					return true;
				}

				// удаление ненужных строк
				str_eol = strstr(rx_buf, "\r\n");
				if (str_eol)
				{
					str_eol_2 = strstr(str_eol + 2, "\r\n");
					if (str_eol_2)
					{
						strcpy(rx_buf, str_eol + 2);
						// memmove(rx_buf, str_eol + 2, strlen(str_eol + 2));
						// rx_buf[strlen(str_eol + 2)] = 0;

						rx_cnt = strlen(rx_buf);
						memset(rx_buf + rx_cnt, 0, sizeof(rx_buf) - rx_cnt);
					}
				}
			}

			if (strstr(rx_buf, "ERROR"))
			{
				break;
			}
		}
		else
		{
			Wait1MS(30);
		}
	}

	DBGLine("Modem answer: %s", rx_buf);

	// таймаут истёк, желаемый ответ модема не получен
	return false;
}

bool CGPRSConnection::atPosfixExecute(DWORD timeout, char* posfix, char* ans_buf, WORD ans_buf_size, char* cmd, ...)
{
	m_dwLastCall = GetTickCount();

	char	tx_buf[ AT_CMD_BUF_SIZE ];
	char	rx_buf[ AT_ANS_BUF_SIZE ];
	WORD	rx_cnt = 0;
	int		ii = 0;

	memset(tx_buf, 0, AT_CMD_BUF_SIZE);
	memset(rx_buf, 0, AT_ANS_BUF_SIZE);


	// форматирование AT команды
	va_list arguments;
	va_start(arguments, cmd);
	vsprintf(tx_buf, cmd, arguments);
	va_end(arguments);

	// старые данные вычитываем
	while (CheckRxInput(COM_GPRS))
		RdCom(COM_GPRS);

	// передача команды
	WrCom(COM_GPRS, tx_buf, strlen(tx_buf));

	// требуется ответ модема? если нет, то выход.
	if (!posfix)
		return true;

	// в течение отведённого времени ожидание ответа модема
	DWORD	tick_start = GetTickCount();
	char*	str_posfix = NULL;
	char*	str_eol = NULL;
	char*	str_eol_2 = NULL;
	char*	str_cme_error = NULL;
	
	while(!IsExpired(tick_start, timeout))
	{
		if (CheckRxInput(COM_GPRS))
		{
			rx_buf[rx_cnt++] = RdCom(COM_GPRS); // чтение 1 байта

			// поиск префикса желаемого ответа
			str_posfix = strstr(rx_buf, posfix);
			if (str_posfix)
			{
				*str_posfix=0;
				//                                  if ((str_eol=strstr(rx_buf,"AT+CIMI"))==NULL)
				if ((str_eol = strstr(rx_buf,tx_buf)) == NULL)
				{
					str_eol = rx_buf;
				}
				else
				{
					//	                                     str_eol+=strlen("AT+CIMI\n");
					str_eol += strlen(tx_buf);
				}
				if ((*str_eol) != 0)
				{
					strncpy(ans_buf, str_eol, Gmin(strlen(str_eol), ans_buf_size - 1));
					purgeChar(ans_buf, '\r');
					purgeChar(ans_buf, '\n');
					for (ii = 0; ii < strlen(ans_buf); ii++)
					{
						if (ans_buf[ii] > '9' || ans_buf[ii] <'0')
						{
							ans_buf[0] = 0;
							break;
						}
					}
				}
				else
				{
					ans_buf[0] = 0;
				}
				
				return true;
			}
		}
		else
		{
			Wait1MS(30);
		}
	}

	// таймаут истёк, желаемый ответ модема не получен
	return false;
}

bool CGPRSConnection::setupModem(CParam& oParam)
{
	DBGLine("modem setup..");

	// синхронизация скорости
	atExecute(oParam,1000, "\nOK", NULL, 0, "AT\r\n");
	SLEEP(100);
	atExecute(oParam,1000, "\nOK", NULL, 0, "AT\r\n");
	SLEEP(100);

	atExecute(oParam,1000, "\nasdasda", NULL, 0, "at+cgmr\r\n");
	SLEEP(100);

	atExecute(oParam,1000, "\nasdasdasda", NULL, 0, "at+wipcfg=3\r\n");
	SLEEP(100);

	atExecute(oParam,1000, "\nOK", NULL, 0, "AT\r\n");
	SLEEP(100);

	SLEEP(2000);
	ClearRS232Buffer(COM_GPRS);




	CHECK_AT(atExecute(oParam,1000, "\nOK", NULL, 0, "AT\r\n"));
	CHECK_AT(atExecute(oParam,1000, "\nOK", NULL, 0, "ATE0\r\n"));
	CHECK_AT(atExecute(oParam,1000, "\nOK", NULL, 0, "ATZ\r\n"));

	// запуск приложения
	atExecute(oParam,1000, NULL, NULL, 0, "AT+WOPEN=1\r\n");
	SLEEP(100);

	// синхронизация скорости (после запуска приложения модуль рестартует)
	atExecute(oParam,1000, "\nOK", NULL, 0, "AT\r\n");
	SLEEP(100);
	atExecute(oParam,1000, "\nOK", NULL, 0, "AT\r\n");
	SLEEP(100);

	CHECK_AT(atExecute(oParam,1000, "\nOK", NULL, 0, "AT\r\n"));
	CHECK_AT(atExecute(oParam,1000, "\nOK", NULL, 0, "AT+WBCM=0,0\r\n"));
	CHECK_AT(atExecute(oParam,1000, "\nOK", NULL, 0, "AT+CMEE=1\r\n"));
	CHECK_AT(atExecute(oParam,1000, "\nOK", NULL, 0, "AT+IFC=2,2\r\n"));
	CHECK_AT(atExecute(oParam,1000, "\nOK", NULL, 0, "AT+CREG=0\r\n"));

	return true;
}

bool CGPRSConnection::changeSim(SConnStat& connStat, CParam& oParam)
{
	BYTE newSim = (connStat.ActSim + 1) % 2;

	if (connStat.SimStates[newSim] == 0 || connStat.SimStates[newSim] == 1)
	{
		
		CONTROL_POWER_SAVING_STATUS(2, 0);
		SLEEP(1000);
		SIM_SELECT(newSim);
		// CONTROL_POWER_SAVING_STATUS(4, 1);
		CONTROL_POWER_SAVING_STATUS(2, 1);

		connStat.ActSim = newSim;

		DBGLine("sim card switched, new sim card: %u", (WORD)connStat.ActSim);

		return true;
	}

	connStat.ActSim = newSim;

	/*DBGLine("Warning: in sim card switch");*/

	return false;
}

bool CGPRSConnection::checkSim(char* strState, WORD size,CParam& oParam)
{
	DBGLine("checking sim card.. ");

	SLEEP(1000);

	char answer_buf[100];
	memset(answer_buf, 0, sizeof(answer_buf));

	if (!atExecute(oParam,1000, "\n+CPIN: ", answer_buf, sizeof(answer_buf), "AT+CPIN?\r\n")) 
		return false;

	// из ответа модема получаем состояние карты: "READY", "SIM PIN" и т.д.
	char*	str_start	= answer_buf + 8;
	WORD	str_len		= strlen(str_start) - 2;
	strncpy(strState, str_start, str_len);

	DBGLine("check sim: OK");

	return true;
}
bool CGPRSConnection::openSocket(CParam& oParam)
{
	bool res = false;
	CTaskDisplay::setDisplay(D_SOCET);
	for (int i= 0; i<3; i++)
	{
		SLEEP(1000);
		res = (atExecute(oParam,60000, "\n+WIPREADY: ", NULL, 0, "at+wipcreate=2,1,\"%s\",%u\r\n", m_stcConnInfo.pIp, m_stcConnInfo.wPort) && atExecute(oParam,10000, "\nCONNECT\r\n", NULL, 0, "at+wipdata=2,1,1\r\n") ) ;
		if (res) { SLEEP(1000);  m_bSocketOpen  = true; return true ;}
	}
	
	SLEEP(1000);
	return res;

}

bool CGPRSConnection::closeSocket(CParam& oParam)
{
	CTaskDisplay::setDisplay(D_CLOSE);
	// Один раз отправить ETX, ответ не проверять ответ
	if (atExecute(oParam, 2000, "\nSHUTDOWN", NULL, 0, "\x03"))
	{
		DBGLine("ETX answer successfully");
	}
	else
	{
		DBGLine("Error: ETX answer");
	}

	bool res;
	m_bSocketOpen  = false;
	// пытаемся перейти в командный режим
	for (int i = 0; i < 5; ++i)
	{		
		if ((res = atExecute(oParam,3000, "\nOK", NULL, 0, "+++")) == true)
			break;
	SLEEP(2000);
	}

	if (res)	//закрываем текущий сокет
	{
		if (atExecute(oParam,3000, "\nOK", NULL, 0, "at+wipclose=2,1\r\n"))
		{ DBGLine("disconnected successfully"); }
		else
		{ DBGLine("Error: cannot close socket"); res = false; }
	}
	else
	{
		DBGLine("Error: cannot switch to AT mode");
	}
	return res;

}

bool CGPRSConnection::connect(SConnStat& stcConnStat, CParam& oParam)
{
	m_bIsCanceled = false;

	if (checkCancelButtom(oParam))
		return false;

	CTaskDisplay::setDisplay(D_SIM);

	if (!changeSim(stcConnStat,oParam))
	{
		if (checkCancelButtom(oParam))
			return false;

		if (!changeSim(stcConnStat,oParam))
		{
			DBGLine("Error: both sim cards bad");
			CTaskDisplay::setDisplay(D_SIM_ERROR);
			m_bIsCanceled = true;
			while( (KEYSCAN()&0xff) == 0xff );			
			return false;
		}
	}

	CTaskDisplay::setDisplay(D_MODEM);
	if (setupModem(oParam))
	{
		DBGLine("modem ready");
	}
	else
	{
		DBGLine("Error: setup modem");
		//disconnect(oParam);

		return false;
	}

	if (checkCancelButtom(oParam))
		return false;

	SLEEP(3000);

 	if (checkCancelButtom(oParam))
 		return false;

// 	if (stcConnStat.SimStates[stcConnStat.ActSim] == 0)
// 	{
		SLEEP(7000);

		char str_cpin_ans[20] = {0};

 		if (checkCancelButtom(oParam))
 			return false;

		if (checkSim(str_cpin_ans, sizeof(str_cpin_ans),oParam) || checkSim(str_cpin_ans, sizeof(str_cpin_ans),oParam)) // два раза подаём команду при необходимости //twice to give a command if necessary
		{
			DBGLine("sim state: %s", str_cpin_ans);

			if (checkCancelButtom(oParam))
				return false;

			if (strcmp(str_cpin_ans, "READY") != 0)
			{
			  // Card is missing or not ready to use
				// карта отсутствует или не готова к использованию
				stcConnStat.SimStates[stcConnStat.ActSim] = 2;				
				return false;
			}

			stcConnStat.SimStates[stcConnStat.ActSim] = 1;
		}
		else
		{
			DBGLine("sim state: absent");

			if (checkCancelButtom(oParam))
				return false;

			stcConnStat.SimStates[stcConnStat.ActSim] = 2;

			return false;
		}
/*	}*/



	m_CSQ = _getCSQ(oParam);
	if (!m_CSQ)
	{
		DBGLine("Error: cannot get CSQ");
		return false;
	}
	else
	{
		DBGLine("CSQ: %d", (int)m_CSQ);
	}

	memset(m_pOperatorName, 0, sizeof(m_pOperatorName));
	if (!_getOperatorName(m_pOperatorName, stcConnStat.ActSim, oParam))
		DBGLine("Error: CGPRSConnection::_getOperatorName()");
	DBGLine("Operator name: %s", m_pOperatorName);

	if (checkCancelButtom(oParam))
		return false;

	if (!getRAS(stcConnStat))
	{
		DBGLine("Error: cannot Get RAS element");
		CTaskDisplay::setDisplay(D_SIM_ERROR);
		m_bIsCanceled = true;
		while( (KEYSCAN()&0xff) == 0xff );
		return false;
	}

	return __connect(stcConnStat, oParam);
}

bool CGPRSConnection::__connect(const SConnStat& stcConnStat, CParam& oParam)
{
	DBGLine("Connecting to: %s, port: %u, server node: %u, IMSI: %d, APN: %s, login: %s, password: %s, keep alive: %u, ping answer max time: %u", m_stcConnInfo.pIp, m_stcConnInfo.wPort, m_stcConnInfo.wServerNode, stcConnStat.vecpstcRASTable[m_nActRasInd]->nImsi, stcConnStat.vecpstcRASTable[m_nActRasInd]->pAPN, stcConnStat.vecpstcRASTable[m_nActRasInd]->pLogin, stcConnStat.vecpstcRASTable[m_nActRasInd]->pPassword, m_stcConnInfo.wKeepAlive, m_stcConnInfo.wPingAnswerMaxTime);

	// Не менее 20 секунд на ожидание регистрации GPRS.
	DWORD	gprs_init_time_ms		=60000;//= max(30000, 1000 * m_stcConnInfo.dwGPRSInitTime);
	// Не менее 20 секунд на открытие сокета.
	DWORD	tcpip_connect_time_ms	= 60000;//max(30000, 1000 * m_stcConnInfo.dwTCPIPConnectTime);
	CTaskDisplay::setDisplay(D_GPRS);

	CHECK_AT(atExecute(oParam,1000, "\nOK", NULL, 0, "at+wipcfg=1\r\n"));
	CHECK_AT(atExecute(oParam,1000, "\nOK", NULL, 0, "at+wipbr=1,6\r\n"));



	if (!atExecute(oParam,1000, "\nOK", NULL, 0, "AT+WIPCFG=2,12,10\r\n") && !m_bOldModem)
		{
			DBGLine("OLD MODEM");
			CTaskDisplay::setDisplay(D_WARRYNG);
			BACK_ONOFF(1);

			Beep(9);
			Beep(1);
			Beep(8);
			Beep(3);

			while(1)
			{	
				char ch = KEYSCAN()&0xff;

			if (ch == 0x13)
			{
				m_bOldModem = true;
				break;

			}
			if (ch == 0x12) 
				{
					m_bIsCanceled = true;
					return false;
				};
			}


		}

	CTaskDisplay::setDisplay(D_GPRS);
	CHECK_AT(atExecute(oParam,1000, "\nOK", NULL, 0, "at+wipbr=2,6,11,\"%s\"\r\n", stcConnStat.vecpstcRASTable[m_nActRasInd]->pAPN));
	CHECK_AT(atExecute(oParam,1000, "\nOK", NULL, 0, "at+wipbr=2,6,0,\"%s\"\r\n", stcConnStat.vecpstcRASTable[m_nActRasInd]->pLogin));
	CHECK_AT(atExecute(oParam,1000, "\nOK", NULL, 0, "at+wipbr=2,6,1,\"%s\"\r\n", stcConnStat.vecpstcRASTable[m_nActRasInd]->pPassword));


	// Делаем x попытки на комманду
	bool bGPRSOpen = false;
	for (int idx = 0; idx < 1; idx++)
	{
		if (atExecute(oParam,gprs_init_time_ms, "\nOK", NULL, 0, "at+wipbr=4,6,0\r\n"))
		{
			bGPRSOpen = true;
			break;
		}
	}
	if (!bGPRSOpen)
		CHECK_AT(atExecute(oParam,gprs_init_time_ms, "\nOK", NULL, 0, "at+wipbr=4,6,0\r\n"));


	//CHECK_AT(atExecute(oParam,tcpip_connect_time_ms, "\n+WIPREADY: ", NULL, 0, "at+wipcreate=2,1,\"%s\",%u\r\n", m_stcConnInfo.pIp, m_stcConnInfo.wPort));
	

	//CHECK_AT(atExecute(oParam,3000, "\nCONNECT\r\n", NULL, 0, "at+wipdata=2,1,1\r\n"));

	oParam.storeParam(CParam::PARAM_SERVER_CONNECTED, new bool(true), true);
	m_bConnected = true;
	showOk();

	return true;
}

bool CGPRSConnection::getRAS(const SConnStat& stcConnStat)
{
	int nIMSI = 0;

	if (!getIMSI(&nIMSI))
		return false;

	DBGLine("IMSI: %d", nIMSI);

	int i = 0;

	while (i < stcConnStat.vecpstcRASTable.size())
	{
 		DBGLine("RASElement[i] == IMSI: %d, APN: %s, login: %s, pass: %s, gateway: %s, autodisc: %d", stcConnStat.vecpstcRASTable[i]->nImsi, stcConnStat.vecpstcRASTable[i]->pAPN, stcConnStat.vecpstcRASTable[i]->pLogin, stcConnStat.vecpstcRASTable[i]->pPassword, stcConnStat.vecpstcRASTable[i]->pGateway, stcConnStat.vecpstcRASTable[i]->nAutoDisc);

		if (stcConnStat.vecpstcRASTable[i]->nImsi == nIMSI || stcConnStat.vecpstcRASTable[i]->nImsi == -1) // 0 - ручной ввод
		{
			m_nActRasInd = i;
			DBGLine("ActRasInd: %d", m_nActRasInd);
			return true;
		}

		++i;
	}

	return false;
}

int CGPRSConnection::readByte(unsigned char *pByte)
{
	while (CheckRxInput(COM_GPRS) && m_wHelpBytesCount < 2 )
	{
		m_HelpBuf[m_wHelpBytesCount++] = RdCom(COM_GPRS);
		WaitTxDone(COM_GPRS);
	}

	if (m_wHelpBytesCount == 0)
		return 0;
	if (m_wHelpBytesCount == 1 && m_HelpBuf[0] == DLE)
		return 0; // сам по себе DLE ничего не значит - ждём прихода следующего байта

	if (m_HelpBuf[0] == ETX)
	{
		// сокет закрыт
		m_wHelpBytesCount = 0;
		return -1;
	}	
	else if (m_HelpBuf[0] == DLE )
	{
		// За DLE должен следовать или ETX или ещё один DLE. Но обойдёмся без проверки.
		*pByte = m_HelpBuf[1];
		m_wHelpBytesCount = 0;
		return 1;
	}

	*pByte = m_HelpBuf[0];
	m_HelpBuf[0] = m_HelpBuf[1];
	m_wHelpBytesCount--;
	return 1;
}

void CGPRSConnection::updateIcons() const
{
	// индикация обмена данными с сервером (получение данных, иконка стрелка вниз)
	ICON_DISPLAY(10 + (IsExpired(m_dwLastRecv, 300) ? 0 : (GetTickCount() % 300 < 200)));

	// индикация обмена данными с сервером (отправка данных, иконка стрелка вверх)
	ICON_DISPLAY(12 + (IsExpired(m_dwLastSent, 300 ) ? 0 : (GetTickCount() % 1200 < 800)));
}

void CGPRSConnection::showOk() const
{
	//CTaskDisplay::setDisplay(D_CONNECT_OK);
	//while((KEYSCAN()&0xff)==0xff);

}

void CGPRSConnection::showCanc() const
{
	CTaskDisplay::setDisplay(D_CONNECT_CANC);
	//while( (KEYSCAN()&0xff) == 0xff );
	//Wait1MS(1000);

}

void CGPRSConnection::showImp() const
{
	CTaskDisplay::setDisplay(D_CONNECT_IMP);
	//while((KEYSCAN()&0xff)==0xff);
	//Wait1MS(1000);

}


WORD CGPRSConnection::readData(unsigned char* buf, int size, CParam& oParam)
{
	WORD	bytes_read = 0;	
	char	res = 1;

	while (res == 1 && bytes_read < size)
	{
		unsigned char	byte = 0;
		res = readByte(&byte);
		//DBGLine("READ %.2X",byte);
		if (res == -1)
		{
			// сокет закрыт
			DBGLine("Error: socket closed");

			disconnect(oParam);

			break;
		}
		else if (res == 1)
		{
			// 1 байт считан
			buf[bytes_read++] = byte;
		}
	}

	if (bytes_read > 0)
		m_dwLastRecv = GetTickCount();

	updateIcons();

	return bytes_read;
}

bool CGPRSConnection::writeData(BYTE* buf, int size)
{

	/*int nETX =0;
	for (int i=0; i< size; i++)
		if (buf[i] == ETX || buf[i] == DLE) nETX++;

	char *outBuf = new char[size + nETX +1];
	memset(outBuf, 0 , sizeof(outBuf));
	int idx = 0, idx2 =0;
	while (idx<size)
	{
		if (buf[idx] == ETX || buf[idx] == DLE)
		outBuf[idx2++] = DLE;
		outBuf[idx2++] = buf[idx++];
		}
	DBGBuf("send",(BYTE * ) outBuf, size + nETX);
	ClearRS232Buffer(COM_GPRS);

	WrCom(COM_GPRS,outBuf, nETX + size );
	WaitTxDone(COM_GPRS);

*/






	while (size > 0)
	{
		 if (*buf == ETX || *buf == DLE)
		{
			BYTE dle = DLE;
			WrCom(COM_GPRS, (char*)&dle, 1);
			WaitTxDone(COM_GPRS);
		}
		 Rprintf ("%.2X ",buf [0]) ;
		WrCom(COM_GPRS, (char*)buf, 1);
		WaitTxDone(COM_GPRS);

		buf++; 
		size--;
	}


	m_dwLastSent = GetTickCount();

	return true;
}

int CGPRSConnection::run()
{
	return 0;
}

bool CGPRSConnection::checkCancelButtom(CParam& oParam)
{
	bool bHaveTLV;
	bHaveTLV = true;

	if (oParam.getParam(CParam::PARAM_NO_CFG, true))
		bHaveTLV = false;

	if (bHaveTLV)
	{
		oParam.readKey();
		char key = oParam.lastKey();
		if (m_bIsCanceled || key == BTKEY_SFUNC || key == BTKEY_SHARP || key == BTKEY_ASTERISK || key == BTKEY_CANCEL)
		{
			DBGLine("CGPRSConnection: cancel pressed");
			m_bIsCanceled = true;
			showCanc();
			return true;
		}
	}

	return false;
}

bool CGPRSConnection::_getOperatorName(char* pOut, BYTE actSim, CParam& oParam)
{
	char	answer_buf[100];

	for (int i = 0; i < 5; ++i)
	{
		SLEEP(1000);
		memset(answer_buf, 0, sizeof(answer_buf));

		pOut[0] = 0;
		// устанавливаем символьный формат оператора
		if (!atExecute(oParam,1000, "\nOK", NULL, 0, "AT+COPS=3,1\r\n"))
			continue;

		// получаем имя оператора
		if (!atExecute(oParam,1000, "\n+COPS: ", answer_buf, sizeof(answer_buf), "AT+COPS?\r\n"))
			continue;

		char* pos = answer_buf;
		pos = strstr(pos, ",");
		if (!pos) 
			continue;
		pos = strstr(pos + 1, ",");
		if (!pos) 
			continue;

		newstrncpy(pOut, pos + 1, 12);
		purge_char(pOut, '\n');
		purge_char(pOut, '\r');
		purge_char(pOut, '"');

		sprintf(answer_buf, "%s[%u]", pOut, actSim + 1);
		strcpy(pOut, answer_buf);

		if (strlen(pOut) > 0)
			return true;

		// устанавливаем цифровой формат оператора
// 		if (atExecute(1000, "\nOK", NULL, 0, "AT+COPS=3,2\r\n"))
// 			return true;
	}

	return false;
}
