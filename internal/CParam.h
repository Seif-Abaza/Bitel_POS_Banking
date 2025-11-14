//---------------------------------------------------------------------------
/****************************************************************************
 * История изменений:
 * $Log: CParam.h,v $
 * Revision 1.3  2010/06/09 06:04:42  rakhmatulin
 * no message
 *
 *
 ***************************************************************************/
//---------------------------------------------------------------------------

#ifndef _CPARAM_
#define _CPARAM_

#include "types.h"
#include "tools.h"

class	CParam;
extern	CParam*	g_pParam;

/// Реализация контейнера буфера обмена информацией между модулями	
class CParam
{
public:
	CParam() : m_nLastKey(0xFF)	{ g_pParam = this; }
	enum PARAM_TYPE
	{
	 PARAM_DISPLAY_INPUT_STRING, PARAM_CARD_HOLDER_NAME, PARAM_TERMINAL_NUMBER, PARAM_SUM, 	PARAM_ACTIVE_XML, PARAM_PRINTER_WORKING, PARAM_SERVER_CONNECTED, PARAM_AUTOCANCEL_FLAG, PARAM_AUTOCANCEL_WORKING,PARAM_DIGITS, 
		PARAM_FCARD_TYPE,PARAM_MONEY_TYPE, PARAM_REASON,  PARAM_AUTH_CODE, PARAM_AUTOCANCEL_MSG,PARAM_CRITICAL_ERROR_MESSAGE, PARAM_CRITICAL_ERROR,  PARAM_NO_CFG, PARAM_USE_XML_DISP, PARAM_ACTIVE_MENU_ITEM,PARAM_ERR_DESC, PARAM_MSG_SEND, PARAM_NEED_CON,PARAM_MSG_RECEIVE,PARAM_MSG_RECEIVE_SIZE,PARAM_USE_MAC,PARAM_USE_ENC
	};

	/// Получение данных по идентификатору PARAM_TYPE
	///
	/// \param [out] отдает 4-ре байта в зависимости от типа они могут являтся либо данными, либо указателем. 
	/// \param [in] eType Тип данных
	/// \param [in] bIsParamSys	Защищенные данные для системных задач (пока не используется)
	void*	getParam(enum PARAM_TYPE eType, bool bIsParamSys = false);

	/// Сохранение данных по идентификатору PARAM_TYPE
	///
	/// \param [in] eType Тип данных
	/// \param [in] pParam Данные
	/// \param [in] bIsParamSys	Защищенные данные для системных задач (пока не используется)
	void	storeParam(enum PARAM_TYPE eType, void *pParam, bool bIsParamSys = false);

	/// Удаление параметра из хранилища с вызовом деструктора по идентификатору PARAM_TYPE
	///
	/// \param [in] eType Тип данных
	/// \param [in] bIsParamSys	Защищенные данные для системных задач (пока не используется)
	void	eraseParam(enum PARAM_TYPE eType, bool bIsParamSys = false);

	/// Удаление параметра из хранилища без вызова деструктора по идентификатору PARAM_TYPE
	///
	/// \param [in] eType Тип данных
	/// \param [in] bIsParamSys	Защищенные данные для системных задач (пока не используется)
	void	detachParam(enum PARAM_TYPE eType, bool bIsParamSys = false);

	/// Очищение всего хранилища с вызовом деструктора
	void	clearParams();

	char lastKey()				{ return m_nLastKey; }
	void setLastKey(char key)	{ m_nLastKey = key; }
	void readKey();

private:
	void	storeXmlGlobal(PARAM_TYPE eParam, void* pValue);

private:
	std::map <PARAM_TYPE, void* > m_mapParamStorage;
	std::map <PARAM_TYPE, void* > m_mapSysParamStorage;
	char m_nLastKey;
};

#endif

