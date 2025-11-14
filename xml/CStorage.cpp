//---------------------------------------------------------------------------
/****************************************************************************
* История изменений:
* $Log: CStorage.cpp,v $
* Revision 1.3  2010/06/09 06:04:45  rakhmatulin
* no message
*
*
***************************************************************************/
//---------------------------------------------------------------------------

#include "CStorage.h"
#include "../internal/tools.h"
#include "../internal/CParam.h"
#include "../tasks/CTaskDisplay.h"
#include "../iso/CMegaContainer.h"
#include "xml_tools.h"

//---------------------------------------------------------------------------

CStorage*		g_pStorage;

CStorage::CKey::CKey(STORAGE_TYPE eType, const string &strID) :
m_eType(eType),
m_strID(strID)
{
}

bool CStorage::CKey::operator < (const CStorage::CKey &oKey) const
{
	if (m_eType != oKey.m_eType)
		return m_eType < oKey.m_eType;
	return m_strID < oKey.m_strID;
}

CStorage::CXKey::CXKey(const string& strComplexTypeID, const string& strElementID) :
	m_strComplexTypeID(strComplexTypeID),
	m_strElementID(strElementID)
{
}

bool CStorage::CXKey::operator < (const CStorage::CXKey &oKey) const
{
	if (m_cCOrder < oKey.m_cCOrder) return true;
	else if ((m_cCOrder == oKey.m_cCOrder) && (m_cEOrder < oKey.m_cEOrder)) return true;
	return false;
}
//bool CStorage::CXKey::operator == (const CStorage::CXKey &oKey) const
//{
//	if (m_strComplexTypeID != oKey.m_strComplexTypeID)
//		return m_eType < oKey.m_eType;
//	return m_strID < oKey.m_strID;
//}
//--------------------------------------------------------------------------------------------------

CStorage::CStorage()
{

}

CStorage::~CStorage()
{
	clear();
}

void CStorage::init()
{
	m_bUseXmlDisplay = false;
	for (int idx = 0; idx < TT_CNT; idx++)
	{
		m_pMegaContainers[idx] = new CMegaContainer();
	}
}

void CStorage::deinit()
{
	for (int idx = 0; idx < TT_CNT; idx++)
	{
		DELETE_POINTER(m_pMegaContainers[idx]);
	}
}

void CStorage::clear()
{
	DBGLine("Clearing all storage data");
	std::map<CKey, SPVariable>::iterator iterVariable = m_mapVariables.begin();
	while (iterVariable != m_mapVariables.end())
	{
		SPVariable* pSPVariable = &(iterVariable->second);
		if (!pSPVariable->bIsSystem)
		{
			pSPVariable->pVariable->clear();
		}
		iterVariable++;
	}
}

void CStorage::addDisplay(const string& strDisplayID, const SDisplay& stcDisplay)
{
	m_mapDisplays[strDisplayID] = stcDisplay;
}

void CStorage::addReceipt(const string& strReceiptID, const SReceipt& stcReceipt)
{
	m_mapReceipts[strReceiptID] = stcReceipt;
}

bool CStorage::getReceipt(const string& strReceiptID, char* pDstBuf)
{
	std::map<string, SReceipt>::iterator it = m_mapReceipts.find(strReceiptID);
	if (it == m_mapReceipts.end())
	{
		DBGLine("Warning: CStorage::getReceipt() - can't find display: \"%s\"", strReceiptID.buffer());
		return false;
	}
	strcpy(pDstBuf, it->second.strLang[g_dwCurrLang]);
	transformCheck(pDstBuf, pDstBuf);

	return true;
}

void CStorage::setDisplay(const string& strDisplay)
{
	m_strCurrentDisplay = strDisplay;
	printScreen();
}

const string& CStorage::getMessageString(const string& strDisplayID)
{
	std::map<string, SDisplay>::iterator iterDisplay = m_mapDisplays.find(strDisplayID);
	if (iterDisplay == m_mapDisplays.end())
	{
		DBGLine("CStorage::getMessageString. Can't find display %s", strDisplayID.buffer());
		return strDisplayID;
	}
	return (*iterDisplay).second.strLang[g_dwCurrLang];
}

void CStorage::printScreen()
{
	std::map<string, SDisplay>::iterator iterDisplay = m_mapDisplays.find(m_strCurrentDisplay);
	if (iterDisplay == m_mapDisplays.end())
	{
		DBGLine("CStorage::printScreen. Can't find display %s", m_strCurrentDisplay.buffer());
		return;
	}
	CTaskDisplay::clearDisplay();
	CTaskDisplay::printScreen(*g_pParam, (*iterDisplay).second.strLang[g_dwCurrLang].buffer());
}



void CStorage::addPatternVariable(int nPattern, const SVariable& stcVariable)
{
	DBGLine("CStorage::addPatternVariable: %d", nPattern);
	std::map<int, SVariable>::iterator iterPatternVariable = m_mapPatternVariable.find(nPattern);
	if (iterPatternVariable != m_mapPatternVariable.end())
	{
		DBGLine("CStorage::addPatternVariable. Item: %d already exists.", nPattern);
		return;
	}

	m_mapPatternVariable[nPattern] = stcVariable;
}

string CStorage::getPatternValue(int nPattern)
{
	std::map<int, SVariable>::iterator	iterPatternVariable = m_mapPatternVariable.find(nPattern);
	if (iterPatternVariable == m_mapPatternVariable.end())
	{
		DBGLine("CStorage::getPatternValue. Can't find pattern %d", nPattern);
		return string();
	}
	SVariable* pStcPatternVariable = &(iterPatternVariable->second);
	return pStcPatternVariable->get();
}

void CStorage::useXmlDisplay(bool bUseXmlDisplay)
{
	m_bUseXmlDisplay = bUseXmlDisplay;
	if (m_bUseXmlDisplay)
		g_pParam->storeParam(CParam::PARAM_USE_XML_DISP, new bool(true));
	else
		g_pParam->eraseParam(CParam::PARAM_USE_XML_DISP);
}

//--------------------------------------------------------------------------------------------------

IVariable* CStorage::getVariable(STORAGE_TYPE eType, const string& strID, bool bIsSystem)
{
	return getVariable(CKey(eType, strID), bIsSystem);
}

IVariable* CStorage::getVariable(const CKey& oKey, bool bIsSystem )
{
	std::map<CKey, SPVariable>::iterator iterVariable = m_mapVariables.find(oKey);
	if (iterVariable == m_mapVariables.end())
	{
		IVariable* pVariable;
		switch (oKey.m_eType)
		{
		case ST_GLOBAL:
			pVariable = new CGlobalVariable();
			break;
		case ST_TRANSACTION_SEND:
			pVariable = new CTransactionVariable(TT_SEND, oKey.m_strID);
			break;
		case ST_TRANSACTION_RECEIVE:
			pVariable = new CTransactionVariable(TT_RECEIVE, oKey.m_strID);
			break;
		case ST_TRANSACTION_BACKUP:
			pVariable = new CTransactionVariable(TT_BACKUP, oKey.m_strID);
			break;
		default:
			DBGLine("CStorage::getVariable unknown type");
			return NULL;
		}
		SPVariable stcSPVariable;
		stcSPVariable.pVariable = pVariable;
		stcSPVariable.bIsSystem = bIsSystem;
		m_mapVariables[oKey] = stcSPVariable;
		return pVariable;
	}
	return (*iterVariable).second.pVariable;
}

IVariable* CStorage::getVariable(const string& m_strComplexTypeID, const string& m_strElementID)
{
	return getVariable(CXKey(m_strComplexTypeID, m_strElementID));
}

IVariable* CStorage::getVariable(const CXKey& oKey)
{
	std::map<CXKey, SPVariable>::iterator iterVariable = m_mapACPVariables.find(oKey);
	if (iterVariable == m_mapACPVariables.end())
	{
		IVariable* pVariable = new CGlobalVariable();;
		SPVariable stcSPVariable;
		stcSPVariable.pVariable = pVariable;
		stcSPVariable.bIsSystem = false;
		m_mapACPVariables[oKey] = stcSPVariable;
		return pVariable;
	}
	return (*iterVariable).second.pVariable;
}

//---------------------------------------------------------------------------

void CGlobalVariable::store(const string& strData, FILTER_TYPE eFilter)
{	
	if (eFilter == FLT_NONE)
	{
		m_strData = strData;
	} else
	{
		// Применить фильтр
		m_strData = applyFilter(eFilter, strData);
	}
}

string CGlobalVariable::get(FILTER_TYPE eFilter)
{
	if (eFilter == FLT_NONE)
	{
		return m_strData;
	} else
	{
		// Применить фильтр
		return applyFilter(eFilter, m_strData);
	}
}

void CGlobalVariable::clear()
{
	m_strData.clear();
}

//---------------------------------------------------------------------------

CTransactionVariable::CTransactionVariable(TARGET_TYPE eTarget, string strKey)
:
m_eTarget(eTarget),
m_strKey(strKey)
{	
}

void CTransactionVariable::store(const string &strData, FILTER_TYPE eFilter)
{
	string strFiltered;	
	if (eFilter == FLT_NONE)
	{
		strFiltered = strData;
	} else
	{
		// Применить фильтр
		strFiltered = applyFilter(eFilter, strData);
	}
	// В зависимости от типа eTarget сложить в нужную транзакцию
	g_pStorage->getTransactionContainer(m_eTarget)->set(m_strKey, strFiltered);
}

string CTransactionVariable::get(FILTER_TYPE eFilter)
{
	// В зависимости от типа eTarget вытащить из нужной транзакции
	string strPlain = g_pStorage->getTransactionContainer(m_eTarget)->get(m_strKey);
	if (strPlain.empty())
		DBGLine("Warining, CTransactionVariable::get empty");
	if (eFilter == FLT_NONE)
	{
		return strPlain;
	} else
	{
		// Применить фильтр
		return applyFilter(eFilter, strPlain);
	}
}

//--------------------------------------------------------------------------------------------------

void SVariable::store(const string &strData)
{
	pVariable->store(strData, eFilter);
}

string SVariable::get() const
{
	return pVariable->get(eFilter);
}
void CStorage::transactionReceived(CMegaContainer* pMegaContainer)
{
	DELETE_POINTER(m_pMegaContainers[TT_BACKUP]);
	m_pMegaContainers[TT_BACKUP] = m_pMegaContainers[TT_RECEIVE];
	m_pMegaContainers[TT_RECEIVE] = pMegaContainer;
	m_pMegaContainers[TT_SEND]->clear();
	m_pMegaContainers[TT_SEND]->setType("uninit", "");
}
