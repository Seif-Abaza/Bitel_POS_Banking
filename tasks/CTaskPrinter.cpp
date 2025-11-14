
#include "CTaskPrinter.h"
#include "../extern.h"
#include "CTaskDisplay.h"
#include "../internal/tools.h"
#include "../internal/CParam.h"
#include "../xml/CStorage.h"
#include "../internal/CGlobalConfig.h"
#include "../internal/CArabicEncoder.h"
#include "../internal/CArabicString.h"

extern DWORD	g_dwCurrLang;

void CTaskPrinter::init()
{
	g_pParam->eraseParam(CParam::PARAM_PRINTER_WORKING, true);
	m_nLinesCnt			= 0;
	m_nCurrLines		= 0;
	m_nCurrPos			= 0;
	m_wCurrCopy			= 1;
	m_nCopyesCnt		= 2;
	m_nBolt				= 0;
}

void CTaskPrinter::_procPatternXML(char* pBuf, int nCnt, int nPatt, char align)
{
	string strValue = g_pStorage->getPatternValue(nPatt); // Если не нашел, будет пустая строка, предупреждение выдаст в поиске	
	DBGLine("Add pattern: %d, value: \"%s\"", nPatt, strValue.buffer());
	if (!strValue.empty())
	{
		_insertPattern(pBuf, nCnt, align, strValue.buffer());
	}
	else
	{
		DBGLine("Error: CTaskPrinter pattern not found: %d", nPatt);
		char* pEmptyPatt = new char[nCnt + 1];
		pEmptyPatt[nCnt] = 0;
		memset(pEmptyPatt, align, nCnt);
		_insertPattern(pBuf, nCnt, align, pEmptyPatt);
		DELETE_POINTER_ARR(pEmptyPatt);
	}
}
void CTaskPrinter::_procPattern(char* pBuf, int nCnt, int nPatt, char align)
{
	string strValue;
	switch (nPatt)
	{
		case 901:
			{
				char strBuf[25];
				DATE_TIME dt;
				GetRTCDateTime(&dt, 0);
				memset(strBuf, 0, 25);
				sprintf(strBuf, "%02X %s %02X%02X", dt.CLKDATE, getMounth(dt.CLKMONTH,g_dwCurrLang).buffer(),dt.CLKYEARH, dt.CLKYEARL);
				strValue = strBuf;
				break;
			}
		case 902:
			{
				DATE_TIME dt;
				GetRTCDateTime(&dt, 0);
				char* strHHMMSS=new char[7]; AUTO_DELETE_ARR(char,strHHMMSS);
				memset(strHHMMSS,0,7);
				sprintf(strHHMMSS, "%02X:%02X", dt.CLKHOUR,dt.CLKMIN);
				strValue = strHHMMSS ;	
				break;

			}
		case 903:
			{	
				g_pConfig->getTermNumb(strValue);
				break;
			}
		case 904:
			{				
				SPublicInfo stcPublicInfo;
				SCardHolderInfo stcCardHolderInfo;
				g_pConfig->getCardInfo(stcPublicInfo,stcCardHolderInfo,2);
				strValue = stcPublicInfo.strCardNumber;				
				break;
			}		
		case 905:
			{				
				SPublicInfo sPublicInfo;
				SCardHolderInfo sCardHolderInfo;
				g_pConfig->getCardInfo(sPublicInfo,sCardHolderInfo,0);
				strValue = sPublicInfo.strCardNumber;
				break;
			}
		case 907:
			{				
				IVariable* pVar = g_pStorage->getVariable(ST_GLOBAL, "sum", true);
				strValue =pVar->get(FLT_DISPLAY_SUM);		
				break;
			}
		case 906:
			{
				SPublicInfo sPublicInfo;
				SCardHolderInfo sCardHolderInfo;
				g_pConfig->getCardInfo(sPublicInfo,sCardHolderInfo,0);
				string strNameP= CArabicEncoder(sCardHolderInfo.strName).encode();
     			strValue  = CArabicString(strNameP).rightToLeft(32);
				DBGBuf("name",(BYTE*) strValue.buffer(),strValue.length());
				break;
			}
		case 908:
			{	
				IVariable* pVar = g_pStorage->getVariable(ST_GLOBAL, "id", true);
				strValue =pVar->get();		
				break;
			}
		case 909:
			{				
				IVariable* pVar = g_pStorage->getVariable(ST_GLOBAL, "name", true);
				strValue =pVar->get();	
				string strNameP= CArabicEncoder(strValue).encode();
				strValue  = CArabicString(strNameP).rightToLeft(32);
				DBGBuf("name",(BYTE*) strValue.buffer(),strValue.length());
				break;
			}
		default:
			{
				

				strValue = "Empty";
					break;
			}
		

	};
	DBGLine("Add pattern: %d, value: \"%s\"", nPatt, strValue.buffer());
	if (!strValue.empty())
	{
		_insertPattern(pBuf, nCnt, align, strValue.buffer());
	}
	else
	{
		DBGLine("Error: CTaskPrinter pattern not found: %d", nPatt);
		char* pEmptyPatt = new char[nCnt + 1];
		pEmptyPatt[nCnt] = 0;
		memset(pEmptyPatt, align, nCnt);
		_insertPattern(pBuf, nCnt, align, pEmptyPatt);
		DELETE_POINTER_ARR(pEmptyPatt);
	}
}

void CTaskPrinter::_insertPattern(char* pBuf, int nCnt, char align, const char* field)
{
	memset(pBuf, ' ', nCnt);
	switch (align)
	{
	case ALIGN_LEFT:
		{
			int nLen = strlen(field);
			if (nLen > nCnt)
				nLen = nCnt;

			memcpy(pBuf, field, nLen);
			for (int i = 0; i < nCnt - nLen; ++i)
				*(pBuf + nLen + i) = ' ';

			break;
		}
	case ALIGN_RIGHT:
		{
			int nLen = strlen(field);
			if (nLen > nCnt)
				nLen = nCnt;

			memcpy(pBuf + nCnt - nLen, field + strlen(field) - nLen, nLen);
			for (int i = 0; i < nCnt - nLen; ++i)
				*(pBuf + i) = ' ';

			break;
		}
	case ALIGN_CENTER:
		{
			int nLen = strlen(field);
			if (nLen > nCnt)
				nLen = nCnt;

			int nPos = (nCnt - nLen) / 2;
			memcpy(pBuf + nPos, field, nLen);

			break;
		}
	}
}

void CTaskPrinter::_makeBuf(const BYTE* pBuf)
{
	int nPos = 0;

	while (pBuf[nPos] != 0)
	{
		if (m_nBolt > 0 && m_nCurrPos > m_nBolt) // если символы с жирным шрифтом кончились..
			m_nBolt = 0;

		switch (pBuf[nPos])
		{
		case SYMBOL_CARET:
			{
				((char*)CHAR_BUFFER1)[m_nCurrPos++] = ' ';
				++nPos;
				break;
			}
		case SYMBOL_AND:
			{
				((char*)CHAR_BUFFER1)[m_nCurrPos++] = ' ';
				++nPos;
				int n = MAX_IN_LINE_CNT - m_nCurrPos % MAX_IN_LINE_CNT;
				memset(((char*)ATTB_BUFFER1) + m_nCurrPos, 0x20, n);
				m_nBolt = m_nCurrPos + n;

				DBGLine("SYMBOL '&' FOUND! m_nCurrPos: %d, n: %d, m_nBolt: %d", m_nCurrPos, n, m_nBolt);

				break;
			}
		
		case ALIGN_LEFT:
		case ALIGN_RIGHT:
		case ALIGN_CENTER:
			{
				unsigned char	pNum[4];
				int		nCnt	= 1;
				char	align	= pBuf[nPos];
				memset(pNum, 0, sizeof(pNum));

				while (pBuf[++nPos] != align)
				{
					if (pBuf[nPos] < '0' || pBuf[nPos] > '9')
						break;
					pNum[nCnt - 1] = pBuf[nPos];
					++nCnt;
				}

				int nPatt = atoi((char*)pNum);
				while (pBuf[nPos] == align)
				{
					++nCnt;
					++nPos;
				}

				bool* bUseXmlDisp	= (bool*)g_pParam->getParam(CParam::PARAM_USE_XML_DISP);
				if (!bUseXmlDisp)
					DBGLine("Warning: CTaskPrinter::_makeBuf() - !bUseXmlDisp");
				if (nPatt<900) _procPatternXML((char*)CHAR_BUFFER1 + m_nCurrPos, nCnt, nPatt, align);
				           else _procPattern((char*)CHAR_BUFFER1 + m_nCurrPos, nCnt, nPatt, align);


				if (m_nBolt > 0) // если жирный шрифт, то зачем-то надо вставлять мусор через символ (вставляем пробелы)
				{
					char* pStrPatt = new char[nCnt + 1];
					memset(pStrPatt, 0, nCnt + 1);
					memcpy(pStrPatt, ((char*)CHAR_BUFFER1) + m_nCurrPos, nCnt);
					int pos_1 = 0;
					int pos_2 = 0;
					while (pStrPatt[pos_1])
					{
						((char*)CHAR_BUFFER1)[m_nCurrPos + pos_2]		= pStrPatt[pos_1];
						((char*)CHAR_BUFFER1)[m_nCurrPos + pos_2 + 1]	= ' ';
						++pos_1;
						pos_2 += 2;
					}
					DELETE_POINTER_ARR(pStrPatt);
					nPos		+= nCnt;
					m_nCurrPos	+= nCnt;
				}

				m_nCurrPos += nCnt;

				break;
			}
		default:
			{
				((char*)CHAR_BUFFER1)[m_nCurrPos] = pBuf[nPos];
				++nPos;
				++m_nCurrPos;
			}
		}
	}

#ifdef MY_DEBUG


	DBGLine("PRINT");
	int nS=0;
	char strbuf[34];
	memset(strbuf,0,34);

	while (nS<strlen((char*)CHAR_BUFFER1))
	{
		memcpy(strbuf,(char*)CHAR_BUFFER1+nS,32);
		DBGLine(strbuf);
		nS+=32;
	}
#endif

	m_nLinesCnt = m_nCurrPos / MAX_IN_LINE_CNT;
}

void CTaskPrinter::_delCurrLine()
{
	int n = m_nCurrPos % MAX_IN_LINE_CNT;
	DBGLine("deleted: %d", MAX_IN_LINE_CNT - n);
	m_nCurrPos -= MAX_IN_LINE_CNT - n + 1;
}

void CTaskPrinter::_startPrinting()
{
	// Попытки очистки предыдущего буфера
	for (int idx = 0; idx < 5; idx++)
	{
		int nPrintResult = PRINTER_WORKING();
		if (nPrintResult == 0x60 || nPrintResult == 0xFF)
			break;
	}

	DBGLine("Print starting..");

	m_nCurrLines = m_nLinesCnt;	
	SET_LINESPACE(2);

	if (m_nCurrLines > MAX_LINES_CNT)
	{
		DBGLine("m_nCurrLines > MAX_LINES_CNT: %d", m_nCurrLines);
		SET_FORMFEED(MAX_LINES_CNT);
		m_nCurrLines -= MAX_LINES_CNT;
	}
	else
	{
		DBGLine("m_nCurrLines: %d", m_nCurrLines);
		SET_FORMFEED(m_nCurrLines);
		m_nCurrLines = 0;
	}

	CHAR_FILL1[0]	= 0xFF;
	PRINTING_NEED	= 0xFF;

	g_pParam->storeParam(CParam::PARAM_PRINTER_WORKING, new bool(true), true);
	CTaskDisplay::setDisplay(D_PRINTING);
}

void CTaskPrinter::_stopPrinting()
{
	DBGLine("Print stopping..");

	m_wCurrCopy			= 1;
	m_nBolt				= 0;
	m_nBolt				= 0;
	g_pParam->eraseParam(CParam::PARAM_PRINTER_WORKING, true);

	memset(CHAR_FILL1, 0, sizeof(CHAR_FILL1));
	memset(CHAR_BUFFER1, 0, sizeof(CHAR_BUFFER1));
	memset(ATTB_BUFFER1, 0, sizeof(ATTB_BUFFER1));

	CHAR_FILL1[0]	= 0;
	PRINTING_NEED	= 0;
	m_nCurrPos		= 0;
	m_nLinesCnt		= 1;
	m_nCurrLines	= m_nLinesCnt;

	//ClearLCDLine(3);
	//Lprint(3, 0, " PRESS ANY KEY");
}

bool    CTaskPrinter::printDone()
{
	bool* bPrinterWorking = (bool*)g_pParam->getParam(CParam::PARAM_PRINTER_WORKING, true);
	if (!bPrinterWorking)
		return true;
	return false;


}

void CTaskPrinter::printBuf(const BYTE* pBuf, int nCopyes /* = 1 */)
{
	DBGLine("print %s",pBuf);
	m_nCopyesCnt = nCopyes;

	for(int i = 0; i < 3; ++i)
	{
		if (ONLINE_READ() == 0)
			MAKEONLINE();
		else
			break;

		Wait1MS(100);
	}

	//WinToDos((BYTE*)pBuf);

	_makeBuf(pBuf);

	int n = MAX_IN_LINE_CNT - m_nCurrPos % MAX_IN_LINE_CNT;
	memset(((char*)CHAR_BUFFER1) + m_nCurrPos, ' ', n + MAX_IN_LINE_CNT * BOTTOM_SPACE);
	m_nCurrPos	+= n + MAX_IN_LINE_CNT * BOTTOM_SPACE;
	m_nLinesCnt += BOTTOM_SPACE;

	DBGLine("CTaskPrinter - printer buffer: \"%s\"", (char*) pBuf);
	_startPrinting();
}

int CTaskPrinter::run()
{
	do 
	{
		//==========================================================================================
		// 1.
		// Если не стоит флаг, то ничего не делаем
		bool* bPrinterWorking = (bool*)g_pParam->getParam(CParam::PARAM_PRINTER_WORKING, true);
		if (!bPrinterWorking)
			break;
		//==========================================================================================


		//==========================================================================================
		// 2.
		// Вызываем PRINTER_WORKING (функция печати)
		int nRes;
		CTaskDisplay::setDisplay(D_PRINTING);
		while (true)
		{
			nRes = PRINTER_WORKING();
			if (nRes != 0xFF)
			{
				DBGLine("Printer working answer: %02X", nRes & 0xFF);
			}
			if (nRes == 0x60 || nRes == 0x37 || nRes == 0x35 ||nRes == 0x61 )
			{				
				break;
			}
		}
		//==========================================================================================


		//==========================================================================================
		// 3.
		// Если печать завершена..
		if (nRes == 0x60 ||nRes == 0x61 )
		{
			if (m_nCurrLines > MAX_LINES_CNT)
			{
				SET_FORMFEED(MAX_LINES_CNT);
				m_nCurrLines -= MAX_LINES_CNT;
			}
			else
			{
				SET_FORMFEED(m_nCurrLines);
				m_nCurrLines = 0;
			}

			//======================================================================================
			// 4.
			// Если требуется напечатать еще копий..
			if (m_wCurrCopy < m_nCopyesCnt)
			{
				++m_wCurrCopy;

				char	oldKey	= KEYSCAN() & 0xFF;
				bool	bFinish	= false;

				while (!bFinish)
				{
					char newKey = KEYSCAN() & 0xFF;

					do 
					{
						if (oldKey == newKey)
							break;

						if (newKey == BTKEY_CANCEL)
						{
							_stopPrinting();
							bFinish = true;
							break;
						}

						_startPrinting();
						bFinish = true;
					} while (false);
				}
			}
			else
			{
				_stopPrinting();
			}

			break;
			//======================================================================================
		}
		//==========================================================================================


		//==========================================================================================
		// Проверяем, есть ли бумага
		if (nRes == 0x37 || nRes == 0x35)
		{
			DBGLine("Paper is over in printer");
			CTaskDisplay::setDisplay(D_E_PAPER_OVER);

			while (true)
			{
				char key = KEYSCAN() & 0xFF;
				if (key == BTKEY_ENTER)
				{
					_startPrinting();
					break;
				}
				if (key == BTKEY_CANCEL)
				{
					_stopPrinting();
					break;
				}
			}
			break;
		}
		//==========================================================================================


		//==========================================================================================
		// Смотрим, все ли нормально
		if (nRes == 0x36)
		{
			DBGLine("Warning: printer goes to malfunction!");
		}
		//==========================================================================================
	} while (false);

	return 0;
}
