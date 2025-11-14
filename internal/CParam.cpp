//---------------------------------------------------------------------------
/****************************************************************************
 * История изменений:
 * $Log: CParam.cpp,v $
 * Revision 1.3  2010/06/09 06:04:42  rakhmatulin
 * no message
 *
 *
 ***************************************************************************/
//---------------------------------------------------------------------------
#include "CParam.h"
#include "tools.h"
#include "../tasks/CTaskDisplay.h"
#include "../xml/CStorage.h"
#include "../internal/CGlobalConfig.h"
#include "../extern.h"


void * CParam::getParam( enum PARAM_TYPE eType, bool bIsParamSys /*= false*/ )
{
	std::map <PARAM_TYPE, void* >::iterator iterParam;
	if(bIsParamSys)
	{
		iterParam = m_mapSysParamStorage.find(eType);
		if(iterParam != m_mapSysParamStorage.end())
		{			
			return (*iterParam).second;
		}
	} else {
		iterParam = m_mapParamStorage.find(eType);
		if(iterParam != m_mapParamStorage.end())
		{
			return (*iterParam).second;
		}
	}

	return NULL;
}

void CParam::storeParam(enum PARAM_TYPE eType, void *pParam, bool bIsParamSys /*= false*/)
{
	std::map<PARAM_TYPE, void*>::iterator	it;

	if(bIsParamSys)
	{
		it = m_mapSysParamStorage.find(eType);
		if ((it != m_mapSysParamStorage.end()) && (it->second == pParam))
			return;

		eraseParam(eType, bIsParamSys);
		m_mapSysParamStorage[eType] = pParam;
	}
	else
	{
		it = m_mapParamStorage.find(eType);
		if ((it != m_mapParamStorage.end()) && (it->second == pParam))
			return;

		eraseParam(eType, bIsParamSys);
		m_mapParamStorage[eType] = pParam;
	}
	storeXmlGlobal(eType, pParam);
}

void CParam::eraseParam(enum PARAM_TYPE eType, bool bIsParamSys /*= false*/)
{
	std::map <PARAM_TYPE, void* >::iterator iterParam;
	void *pParam = NULL;
	if(bIsParamSys)
	{
		iterParam = m_mapSysParamStorage.find(eType);
		if(iterParam != m_mapSysParamStorage.end())
			pParam = (*iterParam).second;
	}
	else
	{
		iterParam = m_mapParamStorage.find(eType);
		if(iterParam != m_mapParamStorage.end())
			pParam = (*iterParam).second;
	}

	if(!pParam) return;

	switch(eType)
	{
		//todo: call right destructor for Params
	case PARAM_ACTIVE_XML:
	
		{
			string* p = (string*)pParam;
			DELETE_POINTER(p);
			break;
		}
	case PARAM_CARD_HOLDER_NAME:
	case PARAM_TERMINAL_NUMBER:
	case PARAM_AUTH_CODE:
	case PARAM_MSG_SEND:
	case PARAM_MONEY_TYPE:
	case PARAM_REASON:
	case PARAM_MSG_RECEIVE:
	case PARAM_DISPLAY_INPUT_STRING:
		{
			char *str = (char *)pParam;
			DELETE_POINTER_ARR(str);
			break;
		}
	case PARAM_ACTIVE_MENU_ITEM:
	case PARAM_MSG_RECEIVE_SIZE:
		{
			int *data = (int *)pParam;
			DELETE_POINTER(data);
			break;
		}
	case PARAM_SERVER_CONNECTED:
	case PARAM_AUTOCANCEL_FLAG:
	case PARAM_NO_CFG:
	case PARAM_AUTOCANCEL_WORKING:
	case PARAM_PRINTER_WORKING:
	case PARAM_CRITICAL_ERROR:
	case PARAM_USE_XML_DISP:
	case PARAM_USE_MAC:
	case PARAM_USE_ENC:
	case PARAM_NEED_CON:
		{
			bool* data = (bool*)pParam;
			if (data)
				DELETE_POINTER(data);
			break;
		}
	case PARAM_AUTOCANCEL_MSG:
		{
			char* data = (char*)pParam;
			if (data)
				DELETE_POINTER_ARR(data);
			break;
		}
	case PARAM_SUM:
		{
			char* pStr = (char*)pParam;
			DELETE_POINTER_ARR(pStr);
			break;
		}
	case PARAM_FCARD_TYPE:
		{
			FCARD_TYPE* pData = (FCARD_TYPE*)pParam;
			DELETE_POINTER(pData);
			break;
		}
	}
	if(bIsParamSys)	{m_mapSysParamStorage.erase(iterParam);}
	else {m_mapParamStorage.erase(iterParam);};
}

void CParam::clearParams()
{
	std::map <PARAM_TYPE, void* >::iterator iterParam;
	for (iterParam = m_mapParamStorage.begin(); iterParam != m_mapParamStorage.end(); iterParam++)
	{
		eraseParam((*iterParam).first);
	}
	m_mapParamStorage.clear();

	CTaskDisplay::setDisplay(D_CNT);
}

void CParam::detachParam( enum PARAM_TYPE eType, bool bIsParamSys /*= false*/ )
{
	std::map <PARAM_TYPE, void* >::iterator iterParam;
	if(bIsParamSys)
	{
		iterParam = m_mapSysParamStorage.find(eType);
		if(iterParam != m_mapSysParamStorage.end())
		{
			m_mapSysParamStorage.erase(iterParam);
		}
	} else {
		iterParam = m_mapParamStorage.find(eType);
		if(iterParam != m_mapParamStorage.end())
		{
			m_mapParamStorage.erase(iterParam);
		}
	}
}

void CParam::readKey()
{
	m_nLastKey = KEYSCAN()&0xFF;

}

void CParam::storeXmlGlobal(PARAM_TYPE eParam, void* pValue)
{
	if (!g_pStorage)
	{
		DBGLine("Error: CParam::storeXmlGlobal() - xml working without g_pStorage!");
		return;
	}

	switch (eParam)
	{
	
	case PARAM_TERMINAL_NUMBER:
		{
			IVariable* pVar = g_pStorage->getVariable(ST_GLOBAL, "TERMINAL_NUM", true);
			pVar->store((char*)pValue);
			break;
		}
	case PARAM_CARD_HOLDER_NAME:
		{
			IVariable* pVar = g_pStorage->getVariable(ST_GLOBAL, "CARD_HOLDER", true);
			pVar->store((char*)pValue);
			break;
		}	
	case PARAM_USE_MAC:
		{
			IVariable* pVar = g_pStorage->getVariable(ST_GLOBAL, "use_mac", true);
			if (*(bool*) pValue){  DBGLine("MAC ON"); pVar->store("ON");}
			else { DBGLine("MAC of"); pVar->store("OFF");}
			break;
		}
	case PARAM_USE_ENC:
		{
			IVariable* pVar = g_pStorage->getVariable(ST_GLOBAL, "use_enc", true);
			if (*(bool*) pValue) pVar->store("ON");
			else pVar->store("OFF");
			break;
		}	
	}
}
