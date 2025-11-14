//---------------------------------------------------------------------------
/****************************************************************************
* История изменений:
* $Log: CTaskDisplay.cpp,v $
* Revision 1.3  2010/06/09 06:04:44  rakhmatulin
* no message
*
*
***************************************************************************/
//---------------------------------------------------------------------------
#include "CTaskDisplay.h"
#include "../internal/tools.h"
#include "../internal/CParam.h"
#include "../internal/string.h"
#include "../internal/printer_common.h"
#include "../internal/font1251.h"
#include "../internal/CGlobalConfig.h"
#include "../xml/CStorage.h"
#include "../xml/xml_tools.h"
#include "../extern.h"

#define	LANG_CNT	5	// количество языков

eDisplay	CTaskDisplay::m_eCurDisplay	= D_CNT;

//---структура для хранения языка---
typedef struct _SLang
{
	char*	pLcdFont;
	char*	pPrtFont;
	string*	ppMessages;
} SLang;
//----------------------------------
SLang	g_pLangs[LANG_CNT];
DWORD	g_dwCurrLang;

void CTaskDisplay::init()
{
	m_eCurDisplay			= D_CNT;
	g_dwCurrLang			= 0;

	g_pLangs[0].ppMessages	= new string[E_CNT];
	g_pLangs[0].pLcdFont	= (char*)LcdArabic;
	g_pLangs[0].pPrtFont	= (char*)PrtArabic;
	g_pLangs[0].ppMessages[SCR_008]			= "^Sending request";
	g_pLangs[0].ppMessages[SCR_009]			= "  Waiting for^    response";
	g_pLangs[0].ppMessages[SCR_010]			= "^Swipe User Card";
	g_pLangs[0].ppMessages[D_PRINTING]		= "^    PRINTING";
	g_pLangs[0].ppMessages[D_E_RC1]			= "^  ERROR: READ,^   TRY AGAIN";
	g_pLangs[0].ppMessages[D_E_PAPER_OVER]	= "^ PAPER IS OVER";
	g_pLangs[0].ppMessages[D_CONNECTING]	= "^   CONNECTING";

	g_pLangs[0].ppMessages[D_TELLER]		= "^  Please Insert^   TELLER card";
	g_pLangs[0].ppMessages[D_CITIZEN]		= "^  Please Insert^  CITIZEN card";
	g_pLangs[0].ppMessages[D_REMOVE]		= "^  PLEASE REMOVE ^      CARD";
	g_pLangs[0].ppMessages[D_ERROR]			= "^   WRONG CARD ";
	g_pLangs[0].ppMessages[D_CONFIRM]		= "  Card number^^^No          Yes";
	g_pLangs[0].ppMessages[D_REMOVE_CARD]	= "^  PLEASE REMOVE ^      CARD";
	g_pLangs[0].ppMessages[D_CANCEL]	= "^    CANCELED^  PRESS ANY KEY";
	g_pLangs[0].ppMessages[D_TRY_AGAIN]	= "^Error read card!^     Insert ^  another one";
	g_pLangs[0].ppMessages[D_WARRYNG]	= "    Modem is^  obsolete. Use^ Upgraded modem^  Continue??";
	g_pLangs[0].ppMessages[D_SIM_ERROR]	= "^   ERROR SIM";
	g_pLangs[0].ppMessages[D_PROCESS]	= "Response received^  Processing ^   please wait";





	

	g_pLangs[0].ppMessages[D_NO_CONFIG]		= "^ NO CONFIG FILE";
	g_pLangs[0].ppMessages[D_CONNECT_OK]		= "^   CONNECTING^    SUCSECC";
	g_pLangs[0].ppMessages[D_CONNECT_CANC]		= "^   CONNECTING^    CANCELED^  PRESS ANY KEY";
	g_pLangs[0].ppMessages[D_CONNECT_IMP]		= "^   CONNECTING^   IMPOSSIBLE";

	g_pLangs[0].ppMessages[D_SIM]		= "^   CONNECTING^   CHECK SIM";
	g_pLangs[0].ppMessages[D_SOCET]		= "^   CONNECTING^  OPEN SOCKET";
	g_pLangs[0].ppMessages[D_CLOSE]		= "^   CONNECTING^  CLOSE SOCKET";
	g_pLangs[0].ppMessages[D_MODEM]		= "^   CONNECTING^  CHECK MODEM";
	g_pLangs[0].ppMessages[D_GPRS]		= "^   CONNECTING^   OPEN GPRS";
	g_pLangs[0].ppMessages[D_SAVE]		= "  Save to file^     system^  please wait";

	/*g_pLangs[0].ppMessages[E_TERM_NUM_ERR ] =		"^terminal number^is invalid";
	g_pLangs[0].ppMessages[E_CARD_CODE_INVAILD] =	 "^card code^is invalid";
	g_pLangs[0].ppMessages[E_NO_FILE] =				"^the download^file cannot be found";
	g_pLangs[0].ppMessages[E_CARD_NO_EXIST ] =		"^card code^ is not exist";
	g_pLangs[0].ppMessages[E_NO_BALANCE] =			"^unbalance";
	g_pLangs[0].ppMessages[E_ENCR] =				"^encrypted message^verify fail";
	g_pLangs[0].ppMessages[E_ERR_BALANCE] =			"^the balance of the^card is not enough";
	g_pLangs[0].ppMessages[E_MAC] =					"^MAC error";
	g_pLangs[0].ppMessages[E_DATA] =				 "^date error^please logon^before reconciliation";
	g_pLangs[0].ppMessages[E_SETTELM] =				 "^The result of^the settlement^is unbalance";
	g_pLangs[0].ppMessages[E_REVERS ] =				 "^reversal";
	g_pLangs[0].ppMessages[E_AMMOUNT] =				 "^the amount is^ more than limited ";
	g_pLangs[0].ppMessages[E_FORMAT] =				 "^Msg. format^is error";
	g_pLangs[0].ppMessages[E_SERVER] =				 "^transaction^server error ";
	*/


	g_pLangs[0].ppMessages[E_TERM_NUM_ERR ] =		"   ERROR 11^Terminal number^is invalid^press any key";
	g_pLangs[0].ppMessages[E_CARD_CODE_INVAILD] =	"   ERROR 12^Card code^is invalid^press any key";
	//g_pLangs[0].ppMessages[E_NO_FILE] =				"^the download^file cannot be found";
	g_pLangs[0].ppMessages[E_CARD_NO_EXIST ] =		"   ERROR 16^Card code^is not exist^press any key";
	//g_pLangs[0].ppMessages[E_NO_BALANCE] =			"^unbalance";
	//g_pLangs[0].ppMessages[E_ENCR] =				"^encrypted message^verify fail";
	//g_pLangs[0].ppMessages[E_ERR_BALANCE] =			"^the balance of the^card is not enough";
	//g_pLangs[0].ppMessages[E_MAC] =					"^MAC error";
	//g_pLangs[0].ppMessages[E_DATA] =				 "^date error^please logon^before reconciliation";
	//g_pLangs[0].ppMessages[E_SETTELM] =				 "^The result of^the settlement^is unbalance";
	//g_pLangs[0].ppMessages[E_REVERS ] =				 "^reversal";
	//g_pLangs[0].ppMessages[E_AMMOUNT] =				 "^the amount is^ more than limited ";
	g_pLangs[0].ppMessages[E_FORMAT] =				 "    ERROR 29 ^Message format^is error^press any key";
	//g_pLangs[0].ppMessages[E_SERVER] =				 "^transaction^server error ";
	g_pLangs[0].ppMessages[E_TIMEOUT] =				 "    ERROR ^Receive timeout^press any key";
	g_pLangs[0].ppMessages[E_ERROR_MESSAGE] =		 "    ERROR ^WRONG ANSWER^press any key";




	g_pLangs[1].ppMessages	= new string[D_CNT];
	g_pLangs[1].pLcdFont	= (char*)LcdRus;
	g_pLangs[1].pPrtFont	= (char*)PrtArabic;
	g_pLangs[1].ppMessages[SCR_008]	= "^Отправка ^запроса";
	g_pLangs[1].ppMessages[SCR_009]	= "^Ожидание ^ответа сервера";
	g_pLangs[1].ppMessages[SCR_010]	= "^Карта";
	g_pLangs[1].ppMessages[D_PRINTING]			= "^   ПЕЧАТЬ";
	g_pLangs[1].ppMessages[D_E_RC1]			= "^Ошибка чтения^ карты^попробуйте ещё";
	g_pLangs[1].ppMessages[D_E_PAPER_OVER]		= "^ НЕТ БУМАГИ";
	g_pLangs[1].ppMessages[D_CONNECTING]		= "^ ПОДКЛЮЧЕНИЕ";

	g_pLangs[1].ppMessages[D_TELLER]		= "Вставьте карту^служащего";
	g_pLangs[1].ppMessages[D_CITIZEN]		= "Вставьте карту^жителя";
	g_pLangs[1].ppMessages[D_REMOVE]		= "Выньте ^карту ";
	g_pLangs[1].ppMessages[D_ERROR]			= "НЕВЕРНАЯ ^КАРТА ";
	g_pLangs[1].ppMessages[D_NO_CONFIG]		= "Нет файла ^ конфигурации";



	USE_LCDFONT(g_pLangs[0].pLcdFont, ON);
	USE_PRTFONT(g_pLangs[0].pPrtFont, ON);


}

	void	CTaskDisplay::setLang(int nLang)
	{

		USE_LCDFONT(g_pLangs[(nLang +1) %2].pLcdFont, OFF);
		USE_PRTFONT(g_pLangs[(nLang +1) %2].pPrtFont, OFF);

		USE_LCDFONT(g_pLangs[nLang].pLcdFont, ON);
		USE_PRTFONT(g_pLangs[nLang].pPrtFont, ON);

	}


//////////////////////////////////////////////////////////////////////////

void CTaskDisplay::printScreen(CParam& oParam, const char* strDisplayExtern)
{
	char pBufOut[NUMBER_OF_DISPLAY_LINES][DISPLAY_WIDTH] = {0};
	if (getDisplay() != D_CNT || strDisplayExtern != NULL)
	{
		const char* strDisplay;
		if (strDisplayExtern != NULL)
			strDisplay = strDisplayExtern;
		else
			strDisplay = getStrDisplay(getDisplay());
		makeBuf(strDisplay, pBufOut);
		for (int i = 0; i < NUMBER_OF_DISPLAY_LINES; ++i)
		{
			char *pdest;
			int  result;
			pdest = strstr(pBufOut[i],"@" );
			result = (int)(pdest - pBufOut[i] + 1);
			if ( pdest != NULL )
			{
				CursorOn(i,result-1);
				pBufOut[i][result-1] = ' ';
			}			


			string::out(i, 0, pBufOut[i]);
		}
	}

// 	oParam.eraseParam(CParam::PARAM_UPDATE_DISPLAY, true);
}

void CTaskDisplay::setDisplay(eDisplay display)
{
	m_eCurDisplay = display;
	ClearLCDScreen();
	printScreen(*g_pParam);
}

const char* CTaskDisplay::getStrDisplay(eDisplay display)
{
	return g_pLangs[g_dwCurrLang].ppMessages[display].buffer();
}

void CTaskDisplay::procPattern(char* pBuf, int nCnt, int nPatt, char align)
{
	//==============================================================================================
	//	Особенности:
	//==============================================================================================
	switch (nPatt)
	{
	default:
		{
			DBGLine("Error: CTaskDisplay pattern not found: %d", nPatt);
			char* pEmptyPatt = new char[nCnt];
			memset(pEmptyPatt, ' ', nCnt);
			insertPattern(pBuf, nCnt, align, pEmptyPatt);
			DELETE_POINTER_ARR(pEmptyPatt);
			break;
		}
	}
}

void CTaskDisplay::procPatternXML(char* pBuf, int nCnt, int nPatt, char align)
{
	//==============================================================================================
	//	Особенности:
	//==============================================================================================
	string strValue = g_pStorage->getPatternValue(nPatt); // Если не нашел, будет пустая строка, предупреждение выдаст в поиске
	if (!strValue.empty())
	{
		insertPattern(pBuf, nCnt, align, strValue.buffer());
	} else
	{
		char* pEmptyPatt = new char[nCnt + 1];
		memset(pEmptyPatt, ' ', nCnt);
		pEmptyPatt[nCnt] = 0;
		insertPattern(pBuf, nCnt, align, pEmptyPatt);
		DELETE_POINTER_ARR(pEmptyPatt);
	}
}


void CTaskDisplay::insertPattern(char* pBuf, int nCnt, char align, const char* field)
{
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

			for (int i = 0; i < nCnt; ++i)
			{
				if (i >= nPos && i - nPos < nLen)
					pBuf[i] = field[i - nPos];
				else
					pBuf[i] = ' ';
			}

			break;
		}
	}
}

void CTaskDisplay::makeBuf(const char* pBuf, char pBufOut[NUMBER_OF_DISPLAY_LINES][DISPLAY_WIDTH])
{
	int nPos = 0;
	int nCurrPos = 0;
	int nCurrLine = 0;
	
	while (pBuf[nPos] != 0)
	{
		switch (pBuf[nPos])
		{
		case SYMBOL_DOG:
			{ 
				DBGLine("DOG %d %d",nCurrLine,nCurrPos);
				pBufOut[nCurrLine][nCurrPos] = ' ';
				CursorOn(nCurrLine,nCurrPos++);
				++nPos;
				break;

			}			
		case SYMBOL_AND:
		case SYMBOL_TILDA:
		case SYMBOL_KC:
			{
				pBufOut[nCurrLine][nCurrPos++] = ' ';
				++nPos;
				break;
			}
		case SYMBOL_NEW_LINE:
		case SYMBOL_CARET:
			{
				nCurrPos = 0;
				++nCurrLine;
				++nPos;

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
				if (bUseXmlDisp)
					procPatternXML(pBufOut[nCurrLine] + nCurrPos, nCnt, nPatt, align);
				else				
					procPattern(pBufOut[nCurrLine] + nCurrPos, nCnt, nPatt, align);

				nCurrPos += nCnt;
				break;
			}
		default:
			{
				pBufOut[nCurrLine][nCurrPos] = pBuf[nPos];
				++nPos;
				++nCurrPos;				
			}
		}	
	}
}



void CTaskDisplay::clearDisplay()
{
	ClearLCDScreen();
	m_eCurDisplay = D_CNT;
}
