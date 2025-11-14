//---------------------------------------------------------------------------
/****************************************************************************
* История изменений:
* $Log: CTaskDisplay.h,v $
* Revision 1.3  2010/06/09 06:04:44  rakhmatulin
* no message
*
*
***************************************************************************/
//---------------------------------------------------------------------------

//**************************************************************************************************
//		Класс для отображения информации на экране
//	По задумке весь вывод должен осуществляться через этот класс
//**************************************************************************************************

#ifndef _CTASKDISPLAY_H_
#define _CTASKDISPLAY_H_

#include "../internal/types.h"

#define DISPLAY_MSG_DELAY	3000				// время задержки сообщений на экране

#define NUMBER_OF_DISPLAY_LINES		4			// количество строк на экране
#define DISPLAY_WIDTH				17			// количество символов в строке + 1

extern DWORD	g_dwCurrLang;


enum eDisplay
{
	SCR_008,
	SCR_009,
	SCR_010,

	D_TELLER,
	D_CITIZEN,
	D_REMOVE,
	D_ERROR,
	D_CONFIRM,

	D_PRINTING,
	D_Q_NUMBER_RIGHT,
	D_E_RC1,
	D_E_RC2,
	D_E_PAPER_OVER,
	D_CONNECTING,
	D_NO_CONFIG,

	D_CONNECT_OK,
	D_CONNECT_CANC,
	D_CONNECT_IMP,
	D_REMOVE_CARD,
	D_MODEM,
	D_SIM,
	D_GPRS,
	D_SOCET,
	D_SAVE,
	D_CANCEL,
	D_TRY_AGAIN,
	D_WARRYNG,
	D_TRASH,
	D_SIM_ERROR,
	D_PROCESS,
	D_CLOSE,

	D_CNT,

	E_TERM_NUM_ERR = 41,
	E_CARD_CODE_INVAILD,
	E_NO_FILE,
	E_CARD_NO_EXIST = 46,
	E_NO_BALANCE = 48,
	E_ENCR,
	E_ERR_BALANCE,
	E_MAC,
	E_RECON = 53,
	E_DATA,
	E_SETTELM,
	E_REVERS = 57,
	E_AMMOUNT,
	E_FORMAT,
	E_SERVER,
	E_TIMEOUT,
	E_ERROR_MESSAGE,
	E_CNT,
};


/// Реализация вывода информации на дисплей
class CTaskDisplay
{
public:
	static void			init();
	static void			setDisplay(eDisplay display);

	static eDisplay		getDisplay() { return m_eCurDisplay; }
	static const char*	getStrDisplay(eDisplay display);

	static void printScreen(CParam&, const char* strDisplayExtern = NULL);
	static void	clearDisplay();
	static void	setLang(int nLang);

private:
	//	Обработка паттерна (см. .cpp)
	static void	procPattern(char* pBuf, int nCnt, int nPatt, char align);
	//	Обработка паттерна при работе с XML
	static void	procPatternXML(char* pBuf, int nCnt, int nPatt, char align);
	//	Вставляет строку field в качестве паттерна
	static void	insertPattern(char* pBuf, int nCnt, char align, const char* field);
	//	Обработка буфера
	static void	makeBuf(const char* pBuf, char pBufOut[NUMBER_OF_DISPLAY_LINES][DISPLAY_WIDTH]);

private:
	static eDisplay	m_eCurDisplay;
};


#endif
