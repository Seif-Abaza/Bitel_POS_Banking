
#include "CMegaContainer.h"
#include "../internal/tools.h"
#include "../internal/CGlobalConfig.h"
#include "../extern.h"
#include "../internal/des.h"

// Глоб. переменные для хранения типов
tMapFieldFormats* g_pMapFieldFormats;

//==================================================================================================
// Определить соотвествие поля и формата представляения
// Заполление статических полей
//==============================================================================
void CMegaContainer::initialize()
{
	tMapFieldFormats& g_mapFieldFormats = *g_pMapFieldFormats;
	// Возможные значения: фиксированное либо LLvar
	g_mapFieldFormats[FIELD_CARD_NUM]					= SFieldFormat(FLEXIBLE, 2);
	g_mapFieldFormats[FIELD_TRMSN_DATE_TIME]			= SFieldFormat(FIXED_N, LENGTH_TRMSN_DATE_TIME);
	g_mapFieldFormats[FIELD_CARD_CODE]					= SFieldFormat(FIXED_AN, LENGTH_CARD_CODE);

	g_mapFieldFormats[FIELD_GOV_CODE]				    = SFieldFormat(FIXED_N, LENGTH_GOV_CODE);
	g_mapFieldFormats[FIELD_SETTLEMENT_DATE]			= SFieldFormat(FIXED_N, LENGTH_SETTLEMENT_DATE);
	g_mapFieldFormats[FIELD_CAPTURE_DATE]				= SFieldFormat(FIXED_N, LENGTH_CAPTURE_DATE);
	if (true){
	g_mapFieldFormats[FIELD_PROCESS_CODE]				= SFieldFormat(FIXED_BIN, LENGTH_PROCESS_CODE);
	g_mapFieldFormats[FIELD_TRN_AMOUNT]					= SFieldFormat(FIXED_BIN, LENGTH_TRN_AMOUNT);
	g_mapFieldFormats[FIELD_STAN]						= SFieldFormat(FIXED_BIN, LENGTH_STAN);
	g_mapFieldFormats[FIELD_TRN_LOCAL_TIME]				= SFieldFormat(FIXED_BIN, LENGTH_TRN_LOCAL_TIME * 2);
	g_mapFieldFormats[FIELD_TRN_LOCAL_DATE]				= SFieldFormat(FIXED_BIN, LENGTH_TRN_LOCAL_DATE);
	g_mapFieldFormats[FIELD_ENTRY_MODE]					= SFieldFormat(FIXED_BIN, LENGTH_ENTRY_MODE);
	g_mapFieldFormats[FIELD_FUNCTION_CODE]				= SFieldFormat(FIXED_BIN, LENGTH_FUNCTION_CODE);
	g_mapFieldFormats[FIELD_COND_MODE]				    = SFieldFormat(FIXED_BIN, LENGTH_COND_MODE);
	g_mapFieldFormats[FIELD_TRACK_2]					= SFieldFormat(FLEXIBLE_Z, 2);
	g_mapFieldFormats[FIELD_ICC_DATACAPTURE1]			= SFieldFormat(FLEXIBLE, 3);
	}
	else{
	g_mapFieldFormats[FIELD_PROCESS_CODE]				= SFieldFormat(FIXED_AN, LENGTH_PROCESS_CODE);
	g_mapFieldFormats[FIELD_TRN_AMOUNT]					= SFieldFormat(FIXED_AN, LENGTH_TRN_AMOUNT);
	g_mapFieldFormats[FIELD_STAN]						= SFieldFormat(FIXED_AN, LENGTH_STAN);
	g_mapFieldFormats[FIELD_TRN_LOCAL_TIME]				= SFieldFormat(FIXED_AN, LENGTH_TRN_LOCAL_TIME * 2);
	g_mapFieldFormats[FIELD_TRN_LOCAL_DATE]				= SFieldFormat(FIXED_AN, LENGTH_TRN_LOCAL_DATE);
	g_mapFieldFormats[FIELD_ENTRY_MODE]					= SFieldFormat(FIXED_AN, LENGTH_ENTRY_MODE);
	g_mapFieldFormats[FIELD_FUNCTION_CODE]				= SFieldFormat(FIXED_AN, LENGTH_FUNCTION_CODE);
	g_mapFieldFormats[FIELD_COND_MODE]				    = SFieldFormat(FIXED_AN, LENGTH_COND_MODE);
	g_mapFieldFormats[FIELD_TRACK_2]					= SFieldFormat(FLEXIBLE, 2);
	g_mapFieldFormats[FIELD_ICC_DATACAPTURE]			= SFieldFormat(FLEXIBLE, 3);
	}

	g_mapFieldFormats[FIELD_MRCH_BUSINESS_CODE]			= SFieldFormat(FIXED_N, LENGTH_MRCH_BUSINESS_CODE);
	g_mapFieldFormats[FIELD_ACQRN_INSTTN_IC]			= SFieldFormat(FLEXIBLE, 2);
	g_mapFieldFormats[FIELD_PRIMARY_ACC_NUM_EXT]		= SFieldFormat(FLEXIBLE, 2);
	g_mapFieldFormats[FIELD_RRN]						= SFieldFormat(FIXED_N, LENGTH_RRN);
	g_mapFieldFormats[FIELD_APPROVAL_CODE]				= SFieldFormat(FIXED_AN, LENGTH_APPROVAL_CODE);
	g_mapFieldFormats[FIELD_ACTION_CODE]				= SFieldFormat(FIXED_N, LENGTH_ACTION_CODE);
	g_mapFieldFormats[FIELD_TERM_NUM]					= SFieldFormat(FIXED_AN, LENGTH_TERM_NUM);
	g_mapFieldFormats[FIELD_OUTLET_NUM]					= SFieldFormat(FIXED_AN, LENGTH_OUTLET_NUM);
	g_mapFieldFormats[FIELD_CARD_NAME_ADDRESS]			= SFieldFormat(FLEXIBLE, 2);
	g_mapFieldFormats[FIELD_FILE_UPDAT_FLAGS]			= SFieldFormat(FLEXIBLE, 3);
	g_mapFieldFormats[FIELD_FILES_VERSIONS]			    = SFieldFormat(FLEXIBLE, 2);
	g_mapFieldFormats[FIELD_PROVIDE_ADD_DATA]			= SFieldFormat(FLEXIBLE, 3);
	g_mapFieldFormats[FIELD_TRN_CURRENCY_CODE]			= SFieldFormat(FIXED_AN, LENGTH_TRN_CURRENCY_CODE);
	g_mapFieldFormats[FIELD_STLM_CURRENCY_CODE]			= SFieldFormat(FIXED_N, LENGTH_STLM_CURRENCY_CODE);
	g_mapFieldFormats[FIELD_PIN_DATA]					= SFieldFormat(FIXED_BIN, LENGTH_PIN_DATA);
	g_mapFieldFormats[FIELD_FILE_NAME]				    = SFieldFormat(FLEXIBLE, 2);
	g_mapFieldFormats[FIELD_ADD_AMOUNTS]				= SFieldFormat(FLEXIBLE, 3);
//	g_mapFieldFormats[FIELD_MSG_NO]				        = SFieldFormat(FLEXIBLE, 3);
//	g_mapFieldFormats[FIELD_DATA_CHNK1]				    = SFieldFormat(FLEXIBLE, 5);
	g_mapFieldFormats[FIELD_DATA_CHNK2]				    = SFieldFormat(FLEXIBLE, 3);
	g_mapFieldFormats[FIELD_DATA_CHNK3]				    = SFieldFormat(FLEXIBLE, 3);
	g_mapFieldFormats[FIELD_DATA_CHNK4]				    = SFieldFormat(FLEXIBLE, 3);
	g_mapFieldFormats[FIELD_DATA_CHNK5]				    = SFieldFormat(FLEXIBLE, 3);
	g_mapFieldFormats[FIELD_BTCH_NO]					= SFieldFormat(FLEXIBLE, 3);
	g_mapFieldFormats[FIELD_REDEMPTION_NUM]				= SFieldFormat(FLEXIBLE, 3);
	g_mapFieldFormats[FIELD_BTCH_TOTLS]					= SFieldFormat(FLEXIBLE, 3);
	g_mapFieldFormats[FIELD_MSG_AUTH_CODE]				= SFieldFormat(FIXED_N, LENGTH_MSG_AUTH_CODE);
	g_mapFieldFormats[FIELD_MSG_NUM]					= SFieldFormat(FIXED_N, LENGTH_MSG_NUM);
	g_mapFieldFormats[FIELD_DATA_RECORD]				= SFieldFormat(FLEXIBLE, 3);
	g_mapFieldFormats[FIELD_ACTION_DATE]				= SFieldFormat(FIXED_N, LENGTH_ACTION_DATE);
	g_mapFieldFormats[FIELD_CREDITS_NUM]				= SFieldFormat(FIXED_N, LENGTH_CREDITS_NUM);
	g_mapFieldFormats[FIELD_CREDITS_REV_NUM]			= SFieldFormat(FIXED_N, LENGTH_CREDITS_REV_NUM);
	g_mapFieldFormats[FIELD_DEBIT_NUM]					= SFieldFormat(FIXED_N, LENGTH_DEBIT_NUM);
	g_mapFieldFormats[FIELD_DEBIT_REV_NUM]				= SFieldFormat(FIXED_N, LENGTH_DEBIT_REV_NUM);
	g_mapFieldFormats[FIELD_PAYMENTS_NUM]				= SFieldFormat(FIXED_N, LENGTH_PAYMENTS_NUM);
	g_mapFieldFormats[FIELD_PAYMENTS_REV_NUM]			= SFieldFormat(FIXED_N, LENGTH_PAYMENTS_REV_NUM);
	g_mapFieldFormats[FIELD_CREDITS_AMOUNT]				= SFieldFormat(FIXED_N, LENGTH_CREDITS_AMOUNT);
	g_mapFieldFormats[FIELD_CREDITS_REV_AMOUNTS]		= SFieldFormat(FIXED_N, LENGTH_CREDITS_REV_AMOUNTS);
	g_mapFieldFormats[FIELD_DEBITS_AMOUNT]				= SFieldFormat(FIXED_N, LENGTH_DEBITS_AMOUNT);
	g_mapFieldFormats[FIELD_DEBITS_REV_AMOUNTS]			= SFieldFormat(FIXED_N, LENGTH_DEBITS_REV_AMOUNTS);
	g_mapFieldFormats[FIELD_NET_RECONCIL_AMOUNT]		= SFieldFormat(FIXED_N, LENGTH_NET_RECONCIL_AMOUNT);
	g_mapFieldFormats[FIELD_FILE_NAME_DUMM]				= SFieldFormat(FLEXIBLE, 2);
	g_mapFieldFormats[FIELD_ADD_MSG_AUTH_CODE]			= SFieldFormat(FIXED_N, LENGTH_AUTH_CODE);

	g_mapFieldFormats[AFIELD_OUT_BTC]					= SFieldFormat(FIXED_AN, 36);
	g_mapFieldFormats[AFIELD_BILLING_REF]				= SFieldFormat(FIXED_AN, 36);
	g_mapFieldFormats[AFIELD_OUT_BILLING_ACC_NUM]		= SFieldFormat(FIXED_N, 32);
	g_mapFieldFormats[AFIELD_AMMOUNT]					= SFieldFormat(FIXED_N, 12);
		

 	// Инициализировать формат OpenWay.
// 	COpenWayConverter::initialize();
}


//==================================================================================================
// CBaseMegaContainer
//==============================================================================
CBaseMegaContainer::CBaseMegaContainer(const CBaseMegaContainer& rightContainer)
{
	m_pParent		= rightContainer.m_pParent;
	m_strSelfKey	= rightContainer.m_strSelfKey;
	m_mapFields		= rightContainer.m_mapFields;

	mapAdditionalFields::const_iterator cit = rightContainer.m_mapAdditionalFields.begin();
	for (; cit != rightContainer.m_mapAdditionalFields.end(); ++cit)
	{
		CBaseMegaContainer*	pContainer		= cit->second->_clone();
		m_mapAdditionalFields[cit->first]	= pContainer;
		pContainer->_setParent(this);
		pContainer->_constructSelfKey(cit->first); 
	}
}
//------------------------------------------------------------------------------
CBaseMegaContainer::~CBaseMegaContainer()
{
	clear();
}
//------------------------------------------------------------------------------
void CBaseMegaContainer::set(const string& strField, const string& strValue)
{
	const string&	fieldName = _getFieldName(strField);
	if (fieldName.empty() || strValue.empty())
	{
		DBGLine("Warning: CBaseMegaContainer::set() - empty field name or value");
		return;
	}

	const string&	subFieldName = _getSubFieldName(strField);
	_setField(fieldName, strValue);

}
//------------------------------------------------------------------------------
string CBaseMegaContainer::get(const string& strField) const
{
	const string&	fieldName = _getFieldName(strField);
	if (fieldName.empty())
		return string();

	const string&	subFieldName = _getSubFieldName(strField);
		return _getField(fieldName);


}
//------------------------------------------------------------------------------
const mapFields* CBaseMegaContainer::getFields(const string& strContainer) const
{
	const string&	containerName = _getFieldName(strContainer);
	if (containerName.empty())
		return &m_mapFields;

	const string&	subContainerName = _getSubFieldName(strContainer);

	return _getSubFields(containerName, subContainerName);
}
//------------------------------------------------------------------------------
void CBaseMegaContainer::_setField(const string& fieldName, const string& strValue)
{
	m_mapFields[fieldName] = strValue;
// 	DBGLine("CBaseMegaContainer - field added, id: \"%s\", value: \"%s\"", fieldName.buffer(), strValue.buffer());
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
string CBaseMegaContainer::_getField(const string& fieldName) const
{
	mapFields::const_iterator	cit = m_mapFields.find(fieldName);
	if (cit == m_mapFields.end())
	{
		DBGLine("Warning: CBaseMegaContainer::_getField() - field doesn't exists, id: \"%s\"", fieldName.buffer());
		return string();
	}

	return cit->second;
}
//------------------------------------------------------------------------------
string CBaseMegaContainer::_getSubField(const string& fieldName, const string& subFieldName) const
{
	mapAdditionalFields::const_iterator	cit = m_mapAdditionalFields.find(fieldName);
	if (cit == m_mapAdditionalFields.end())
	{
		DBGLine("Warning: CBaseMegaContainer::_getSubField() - sub field doesn't exists, id: \"%s\"", fieldName.buffer());
		return string();
	}

	return cit->second->get(subFieldName);
}
//------------------------------------------------------------------------------
const mapFields* CBaseMegaContainer::_getSubFields(const string& containerName, const string& subContainerName) const
{
	mapAdditionalFields::const_iterator	cit = m_mapAdditionalFields.find(containerName);
	if (cit == m_mapAdditionalFields.end())
	{
		DBGLine("Warning: CBaseMegaContainer::_getSubFields() - sub container doesn't exists, id: \"%s\"", containerName.buffer());
		return 0;
	}

	return cit->second->getFields(subContainerName);
}
//------------------------------------------------------------------------------
void CBaseMegaContainer::deleteField(const string& strField)
{
	const string&	fieldName = _getFieldName(strField);
	if (fieldName.empty())
		DBGLine("Warning: CBaseMegaContainer::deleteField() - empty field name");

	const string&	subFieldName = _getSubFieldName(strField);
	if (subFieldName.empty())
		_deleteField(fieldName);
	else
		_deleteSubField(fieldName, subFieldName);
}
//------------------------------------------------------------------------------
void CBaseMegaContainer::_deleteField(const string& fieldName)
{
	m_mapFields.erase(fieldName);
// 	DBGLine("CBaseMegaContainer - field deleted, id: \"%s\"", fieldName.buffer());
}
//------------------------------------------------------------------------------
void CBaseMegaContainer::_deleteSubField(const string& fieldName, const string& subFieldName)
{
	mapAdditionalFields::iterator	it = m_mapAdditionalFields.find(fieldName);
	if (it == m_mapAdditionalFields.end())
	{
		DBGLine("Warning: CBaseMegaContainer::_deleteSubField() - sub field doesn't exists, id: \"%s\"", fieldName.buffer());
	}
	else
	{
		it->second->deleteField(subFieldName);
		deleteEmptySubContainers();
	}
}
//------------------------------------------------------------------------------
string CBaseMegaContainer::_getFieldName(const string& strField) const
{
	if (strField.empty())
		return string();

	char*	pDelimiter = strchr(strField.buffer(), '/');
	if (!pDelimiter)
		return strField;

	DWORD	size = pDelimiter - strField.buffer();
	char*	pBuf = new char[size + 1];	AUTO_DELETE_ARR(char, pBuf);

	memcpy(pBuf, strField.buffer(), size);
	pBuf[size] = 0;

	return string(pBuf);
}
//------------------------------------------------------------------------------
string CBaseMegaContainer::_getSubFieldName(const string& strField) const
{
	char*	subFieldName = strchr(strField.buffer(), '/');
	if (subFieldName)
		return string(subFieldName + 1);

	return string();
}
//------------------------------------------------------------------------------
string CBaseMegaContainer::_getSubFieldNameLast(const string& strField) const
{
	char*	subFieldName = strrchr(strField.buffer(), '/');
	if (subFieldName && strlen(subFieldName) > 1)
		return string(subFieldName + 1);

	return string();
}
//------------------------------------------------------------------------------
void CBaseMegaContainer::_constructSelfKey(const string& selfId)
{
	string				resultKey	= selfId;
	CBaseMegaContainer*	pParent		= _getParent();
	while (pParent)
	{
		const string&	parentKey	= pParent->getSelfKey();
		string			strSubField = _getSubFieldNameLast(parentKey);
		if (strSubField.empty())
			strSubField = parentKey;

		if (!parentKey.empty())
			resultKey = (string) strSubField + "/" + (string) resultKey;

		pParent	= pParent->_getParent();
	}

	m_strSelfKey = resultKey;
}
//------------------------------------------------------------------------------
void CBaseMegaContainer::clear()
{
	mapAdditionalFields::iterator it;
	for (it = m_mapAdditionalFields.begin(); it != m_mapAdditionalFields.end(); ++it)
	{
		DELETE_POINTER(it->second);
		m_mapAdditionalFields.erase(it);
	}

	m_strSelfKey.clear();
	m_mapFields.clear();
	m_mapAdditionalFields.clear();
}
//------------------------------------------------------------------------------
void CBaseMegaContainer::deleteEmptySubContainers()
{
	mapAdditionalFields::iterator	it;
	for (it = m_mapAdditionalFields.begin(); it != m_mapAdditionalFields.end(); ++it)
	{
		it->second->deleteEmptySubContainers();
		if (it->second->isEmpty())
		{
			DELETE_POINTER(it->second);
			m_mapAdditionalFields.erase(it);
		}
	}
}
//------------------------------------------------------------------------------
DWORD CBaseMegaContainer::fillJournalEntity(char* pJournalEnt) const //!
{
	return 0;
	/*
	if (!pJournalEnt)
	{
		DBGLine("Error: CBaseMegaContainer::fillJournalEntity() - !pJournalEnt");
		return 0;
	}

	DWORD						dwCount		= 0;
	char*						pEntCurPos	= pJournalEnt;
	mapFields::const_iterator	fcit		= m_mapFields.begin();
	for (; fcit != m_mapFields.end(); ++fcit)
	{
		if (getSelfKey().empty())
			sprintf(pEntCurPos, "%c%s%c%s%c", JRN_BLOCK_OPEN, fcit->first.buffer(), JRN_DELEMITER, fcit->second.buffer(), JRN_BLOCK_CLOSE);
		else
			sprintf(pEntCurPos, "%c%s/%s%c%s%c", JRN_BLOCK_OPEN, getSelfKey().buffer(), fcit->first.buffer(), JRN_DELEMITER, fcit->second.buffer(), JRN_BLOCK_CLOSE);

		DWORD	dwLen = strlen(pEntCurPos);
		dwCount		+= dwLen;
		pEntCurPos	+= dwLen;
	}

	mapAdditionalFields::const_iterator	acit = m_mapAdditionalFields.begin();
	for (; acit != m_mapAdditionalFields.end(); ++acit)
	{
		DWORD dwCurrent = acit->second->fillJournalEntity(pEntCurPos);
		dwCount += dwCurrent;
		pEntCurPos += dwCurrent;
	}

	return dwCount;
	*/
}
//------------------------------------------------------------------------------
void CBaseMegaContainer::fillFromJournalEntity(const char* strJournalEnt)
{
	
	
}
//------------------------------------------------------------------------------
CBaseMegaContainer& CBaseMegaContainer::operator=(const CBaseMegaContainer& rightContainer)
{
	clear();

	m_pParent		= rightContainer.m_pParent;
	m_strSelfKey	= rightContainer.m_strSelfKey;
	m_mapFields		= rightContainer.m_mapFields;

	mapAdditionalFields::const_iterator cit = rightContainer.m_mapAdditionalFields.begin();
	for (; cit != rightContainer.m_mapAdditionalFields.end(); ++cit)
	{
		CBaseMegaContainer*	pContainer		= cit->second->_clone();
		m_mapAdditionalFields[cit->first]	= pContainer;
		pContainer->_setParent(this);
		pContainer->_constructSelfKey(cit->first);
	}
}
//------------------------------------------------------------------------------
CBaseMegaContainer& CBaseMegaContainer::operator+=(const CBaseMegaContainer& rightContainer)
{
	mapFields::const_iterator	fcit = rightContainer.m_mapFields.begin();
	for (; fcit != rightContainer.m_mapFields.end(); ++fcit)
	{
		#ifdef MY_DEBUG
			mapFields::iterator	it = m_mapFields.find(fcit->first);
			if (it != m_mapFields.end())
			{
				DBGLine("Warning: CBaseMegaContainer::operator+=() - field: \"%s\" already in container", fcit->first.buffer());
				DELETE_POINTER(it->second);
			}
		#endif

		m_mapFields[fcit->first] = fcit->second;
	}

	mapAdditionalFields::const_iterator acit = rightContainer.m_mapAdditionalFields.begin();
	for (; acit != rightContainer.m_mapAdditionalFields.end(); ++acit)
	{
		CBaseMegaContainer*	pContainer		= acit->second->_clone();
		if (m_mapAdditionalFields.find(acit->first) != m_mapAdditionalFields.end())
		{
			*m_mapAdditionalFields[acit->first]	+= *pContainer;
			DELETE_POINTER(pContainer);
		}
		else
		{
			m_mapAdditionalFields[acit->first]	= pContainer;
			pContainer->_setParent(this);
			pContainer->_constructSelfKey(acit->first);
		}
	}

	return *this;
}
//------------------------------------------------------------------------------
void CBaseMegaContainer::debugContainer() const
{
#ifdef MY_DEBUG
	for (mapFields::const_iterator cit = m_mapFields.begin(); cit != m_mapFields.end(); ++cit)
	{
		if (getSelfKey().empty())
			{DBGLine("\t\"%s\" : \"%s\"", cit->first.buffer(), cit->second.buffer());}
		else
			{DBGLine("\t\"%s/%s\" : \"%s\"", getSelfKey().buffer(), cit->first.buffer(), cit->second.buffer());}
	}

	for (mapAdditionalFields::const_iterator cit = m_mapAdditionalFields.begin(); cit != m_mapAdditionalFields.end(); ++cit)
	{
		cit->second->debugContainer();
	}
#endif
}
//------------------------------------------------------------------------------



//==================================================================================================
// CMegaContainer
//==============================================================================
CMegaContainer::CMegaContainer(const CMegaContainer& rightContainer)
	: CBaseMegaContainer(rightContainer)
{
	m_strMTID	= rightContainer.m_strMTID;
	m_bUseMac	= rightContainer.m_bUseMac;
}
//------------------------------------------------------------------------------
void CMegaContainer::setType(const string& strMTID, const string& strProcCode)
{
	if (!strMTID.empty())
		m_strMTID = strMTID;

	if (!strProcCode.empty())
		set(FIELD_PROCESS_CODE, strProcCode); // Устанавливает в поля проц. код
}
//------------------------------------------------------------------------------
bool CMegaContainer::isAnswerOk() const //!
{
	return false;
	/*
	if (get(FIELD_ANSWER_CODE) == "000")
		return true;

	return false;
	*/
}
//------------------------------------------------------------------------------
bool CMegaContainer::operator==(const CMegaContainer &oContainer) const //!
{
	return false;
	/*
	bool	bRes = false;
	do 
	{
		int	nLeftMTID	= atoi(getMTID());
		int	nRightMTID	= atoi(oContainer.getMTID());
		if (abs(nRightMTID - nLeftMTID) != 10)
			break;

		if (get(FIELD_PROCESS_CODE) != oContainer.get(FIELD_PROCESS_CODE))
			break;

		if (get(FIELD_TERM_NUM) != oContainer.get(FIELD_TERM_NUM))
			break;

		if (get(FIELD_STAN) != oContainer.get(FIELD_STAN))
			break;

		if (get(FIELD_TRN_AMOUNT) != oContainer.get(FIELD_TRN_AMOUNT))
			break;

		string	origSum1	= get(FIELD_ORIGINAL_SUMM);
		string	origSum2	= oContainer.get(FIELD_ORIGINAL_SUMM);
		if ((!origSum1.empty() && !origSum2.empty()) && (origSum1 != origSum2))
			break;

		bRes	= true;
	} while (false);

	return bRes;
	*/
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void CMegaContainer::debugContainer() const
{
#ifdef MY_DEBUG
	int nmtid = hex2int(m_strMTID.buffer()[0])*100 + hex2int(m_strMTID.buffer()[1]);
	DBGLine("---Debugging CMegaContainer..  MTID: \"%.4d\"", nmtid);
	for (mapFields::const_iterator cit = m_mapFields.begin(); cit != m_mapFields.end(); ++cit)
	{


		string strValue = cit->second;

		// На основе ключа (номер поля) формируем правильное сообщение
		tMapFieldFormats& g_mapFieldFormats = *g_pMapFieldFormats;
		tMapFieldFormats::iterator itFormat = g_mapFieldFormats.find(cit->first.buffer());			
		SFieldFormat& stcFormat = itFormat->second;
		if (stcFormat.eFieldType == FIXED_BCD)
		{
			BYTE * pValue = new BYTE[stcFormat.nLen  * 2 + 1 ];
			memset(pValue, 0, sizeof(pValue));
			(BCDLeft2str((BYTE*) strValue.buffer(),pValue,stcFormat.nLen * 2 ));
			strValue = string((char*)pValue,stcFormat.nLen  * 2);
			DELETE_POINTER_ARR(pValue);
		}
		if (stcFormat.eFieldType != FLEXIBLE)
		{		
			DBGLine("\t\"%s\" : \"%s\"", cit->first.buffer(), strValue.buffer());
		}
		else
		{
			DBGPrt("\t\"%s\" : ", cit->first.buffer());			
			DBGBuf("",(BYTE*) strValue.buffer(), strValue.length());
		}
	}

	
	DBGLine("--/Debugging CMegaContainer..");
#endif
}
//------------------------------------------------------------------------------
string CMegaContainer::composeMessage(TPROTOCOL eTProtocol /* = ISO */) const
{
  if (m_eTProtocol == NBE){
	DBGLine("NBE1");}
  else{
	DBGLine("FDATA1");
  }

	if (eTProtocol == OPENWAY)
	{
		DBGLine("Error: CMegaContainer::composeMessage() - doesn't support OPEN WAY yet");
		return string();
	}

	if (eTProtocol == COMPASS_PLUS)
	{
		DBGLine("Error: CMegaContainer::composeMessage() - doesn't support COMPASS PLUS yet");
		return string();
	}

	tMapFieldFormats& g_mapFieldFormats = *g_pMapFieldFormats;

	string strComposed;
	typedef std::map<string, string> tMapComposed;
	tMapComposed mapComposed;
	char bitMask[16] = {0};			// Битовая маска, по умолчанию хранит 8 байт (64бит), если потребуется, может хранить 16 байт. Формат: сначала хранит 8 байт младших, потом старших
	bool bIsMask16byte = false;		// По умолчанию 8 байт

	// Проходим по корневым полям
	mapFields::const_iterator itFields = m_mapFields.begin();
	while (itFields != m_mapFields.end())
	{
		mapComposed[itFields->first] = itFields->second;
		itFields++;
	}

	// Проходим по дочерним полям
	mapAdditionalFields::const_iterator itAddFields = m_mapAdditionalFields.begin();
	while (itAddFields != m_mapAdditionalFields.end())
	{
		string strKey = itAddFields->first;
		mapComposed[strKey] = itAddFields->second->composeMessage(eTProtocol);
		itAddFields++;
	}

	// Проходим по только что сформированному map, порядок по возрастанию
	tMapComposed::iterator itComposed = mapComposed.begin();
	while (itComposed != mapComposed.end())
	{
		// Получаем готовый текст поля
		string strKey = itComposed->first;
		string strField = itComposed->second;		

		// На основе ключа (номер поля) формируем правильное сообщение
		tMapFieldFormats::iterator itFormat = g_mapFieldFormats.find(strKey);
		if (itFormat == g_mapFieldFormats.end())
		{
			DBGLine("CMegaContainer::composeMessage, format: \"%s\" is not found", strKey.buffer());
			return string();
		}
		SFieldFormat& stcFormat = itFormat->second;

		// На основе формата поля получаем правильное сообщение
		string strFieldComposed;
		if (stcFormat.eFieldType == FIXED_N ||stcFormat.eFieldType == FIXED_BCD )
		{
			strFieldComposed = strField;
			strFieldComposed.setSize(stcFormat.nLen);
		}
		if (stcFormat.eFieldType == FIXED_AN)
		{
			strFieldComposed = strField;
			strFieldComposed.setSizeRightSpaces(stcFormat.nLen);
//			strFieldComposed.setSize(stcFormat.nLen);
		}
		if (stcFormat.eFieldType == FIXED_BIN)
		{
			strFieldComposed = strField;
			strFieldComposed.setSize(stcFormat.nLen); // Eg. "0F" - is represented in 1 byte
			char* pStrFormated = new char[stcFormat.nLen];
			memset(pStrFormated,0,stcFormat.nLen);
			str2BCDRight((BYTE*) strFieldComposed.buffer(), (BYTE*) pStrFormated, stcFormat.nLen);
			DBGBuf("FIXED_BIN",(const LPBYTE)pStrFormated, getLengthBCD(stcFormat.nLen));
			strFieldComposed = string(pStrFormated, getLengthBCD(stcFormat.nLen));
			DELETE_POINTER_ARR(pStrFormated);
		}
		if (stcFormat.eFieldType == FLEXIBLE_Z)
		{
		  char* pStrFormated = new char[strField.length()];
		  char lenasc[5]; memset(lenasc,0,sizeof(lenasc));
          str_to_zL2((LPTSTR)pStrFormated,(LPTSTR)strField.buffer(),strField.length());
          bcd2asc(lenasc,(LPTSTR)pStrFormated,2);
          strFieldComposed = string(pStrFormated, 1 + getLengthBCD(atol(lenasc)));
			DELETE_POINTER_ARR(pStrFormated);

		}
		if (stcFormat.eFieldType == FLEXIBLE)
		{
			// Рассчет длины
			string strLength;
			strLength.format("%0*d", stcFormat.nLen, strField.length());
			char* pStrFormatedLength = new char[getLengthBCD(stcFormat.nLen)];

			if (m_eTProtocol == FDATA){
				strFieldComposed = strLength;
			} else {
			  int len = stcFormat.nLen;
			  DBGLine("LEN:%d %d",stcFormat.nLen,getLengthBCD(stcFormat.nLen))

			str2BCDRight((BYTE*) strLength.buffer(), (BYTE*) pStrFormatedLength, len);
			  DBGLine("LEN:%d %d",stcFormat.nLen,getLengthBCD(stcFormat.nLen))
			DBGBuf("pStrFormatedLength", (BYTE*)pStrFormatedLength, getLengthBCD(stcFormat.nLen));
			strFieldComposed = string(pStrFormatedLength, getLengthBCD(stcFormat.nLen));
			DBGBuf("strFieldComposed", (BYTE*)strFieldComposed.buffer(), strFieldComposed.length());

			}
			strFieldComposed += strField;

			DELETE_POINTER_ARR(pStrFormatedLength);
		}
		strComposed += strFieldComposed;

		// Обновить маску
		int nField = atoi(strKey.buffer());
		// Например:
		// nField: 3 ->		0010 0000
		// nField: 5 ->		0000 1000
		// nField: 16 ->	0000 0000 0000 0001
		bitMask[(nField - 1) / 8] |= 1 << (7 - ((nField - 1) % 8));
		if (nField > 64)
			bIsMask16byte = true;

		itComposed++; 
	}

	if (bIsMask16byte)
	{
		bitMask[0] |= 1 << 7;
	}

	// Если используется MAC, взять сообщение, посчитать MAC, обновить маску, добавить MAC в конец сообщения
/*	do
	{
		if (!m_bUseMac)
			break;

		string strMACKeyEncrypted;
		if (!g_pConfig->getMACEncrypted(strMACKeyEncrypted))
			break;

		BYTE tmkKey[16];
		CPinPad oPinpad;
		if (oPinpad.getMasterKey(0, tmkKey) != CPinPad::PIN_OK)
			break;

		DBGBuf("TKM", tmkKey, sizeof(tmkKey));

		BYTE MACKeyPlain[8];
		hexblk2bin((BYTE*) strMACKeyEncrypted.buffer(), MACKeyPlain, 8);
		des(MACKeyPlain, tmkKey, DEAmoDecT);

		DBGBuf("Mac key plain", MACKeyPlain, sizeof(MACKeyPlain));

		des_context dctx;
		memset(&dctx, 0, sizeof(dctx));
		des_set_key(&dctx, MACKeyPlain);

		BYTE MACsign[8]	= {0};

		for (int idx = 0; idx < strComposed.length(); idx += 8)
		{
			BYTE cryptBuf[8] = {0};
			int nCopySize;
			if (idx + 8 < strComposed.length())
				nCopySize = 8;
			else
				nCopySize = strComposed.length() - idx;

			memcpy(cryptBuf, strComposed.buffer(), nCopySize);
			
			for (int b = 0; b < 8; b++)
			{
				MACsign[b] ^= cryptBuf[b];
			}

			des_encrypt_data(&dctx, MACsign, MACsign, 8, DES_MODE_CBC, NULL);			
		}
		DBGBuf("MACsign", MACsign, sizeof(MACsign));
		string strCompose1 = string((char *) MACsign);
		strComposed += strCompose1;


	} while (false);
*/
	// Определить длинну маски
	int nMaskLength = 8;
	if (bIsMask16byte)
		nMaskLength += 8;

	// Заполнить PowerCARDHeader
	


	//	BYTE  pBuf[] = {0x00, 0x49, 0x60, 0x80, 0x00, 0x00, 0x03, 0x06, 0x60, 0x40, 0x40, 0x00, 0x00, 0x00,0x80, 0x00, 0x00};

	BYTE  pBuf1[] ={0x10, 0x80, 0x60, 0x10, 0x03, 0x80, 0x10, 0x06, 0x60, 0x40, 0x40, 0x10, 0x10, 0x80,0x10, 0x10, 0x10,0x00};

	string strResult((char *)pBuf1);

	if (m_eTProtocol == FDATA){
	  DBGLine("1111111111");
	  string strResult1;
	  char cTempBuff[50]; memset(cTempBuff, 0, sizeof(cTempBuff));

		char  pPlainn[] ="POSV10";
		strResult1.append(pPlainn,6);
		bcd2asc(cTempBuff,(char *)m_strMTID.buffer(),4);
		strResult1.append(cTempBuff,4);
		memset(cTempBuff, 0, sizeof(cTempBuff));
		bcd2asc(cTempBuff,bitMask,(nMaskLength*2));
		strResult1.append(cTempBuff,(nMaskLength*2));
		strResult = strResult1;
//		for (int i = 0; i <6; i++) strResult.buffer()[i] = pPlainn[i];
//
//		bcd2asc(cTempBuff,(char *)m_strMTID.buffer(),4);
//		for (int i = 6; i <10; i++) strResult.buffer()[i] =  cTempBuff[i-6];
//
//		memset(cTempBuff, 0, sizeof(cTempBuff));
//		bcd2asc(cTempBuff,bitMask,(nMaskLength*2));
//		for (int i = 10; i < (10 + (nMaskLength*2)); i++) strResult.buffer()[i] =  cTempBuff[i-10];
	} else {

	  DBGLine("222222222222");

//		char  pPlainn[] ={0x60, 0x00, 0x03, 0x80, 0x00};
		char  pPlainn[] ={0x60, 0x00, 0x01, 0x00, 0x00};

// 	char  pPlain2[] = {0x33, 0x30, 0x30, 0x39, 0x30 };
// 	char  pPlain1[] = {0x60, 0x80, 0x00, 0x00, 0x03 };
// 	char  pMtid[] ={0x06, 0x60};
	
	for (int i = 2; i <7; i++) strResult.buffer()[i] = pPlainn[i-2];
	for (int i = 7; i <9; i++) strResult.buffer()[i] =  m_strMTID.buffer()[i-7];
	for (int i = 9; i <17; i++) strResult.buffer()[i] =  bitMask[i-9];
	}
	strResult+=strComposed;

//	string strKey1,strKey2;
//
//	g_pConfig->getKey(strKey1,strKey2);
//	DBGBuf("key 1" ,(BYTE*) strKey1.buffer(), 8);
//	DBGBuf("key 2" ,(BYTE*) strKey2.buffer(), 8);


	int nLen = strResult.length() - 2;

	//==============================================================================================
	// Шифрование трафика
//	if (g_pConfig->isEncUse())
	if (false)
	{
		// Ключ надо взять сохраненный из ФС

		BYTE ucEncryptedKey[]	= {0x9B, 0xA4, 0xD3, 0x6B, 0x68, 0xF2, 0x88, 0x20};
		BYTE ucKeyForDecrypt[]	= {0x72, 0xB2, 0x55, 0xBD, 0x59, 0x27, 0x13, 0x1A};

		des(ucEncryptedKey, ucKeyForDecrypt, DEAmoDecS);

		int nFullLength = strResult.length() - 7;
		if (nFullLength % 8 != 0)
			nFullLength += 8 - (nFullLength % 8);

		BYTE* pUcMessage = new BYTE[nFullLength];

		memcpy(pUcMessage, strResult.buffer() + 7, strResult.length() - 7);
		

		for (int idx = 0; idx < nFullLength; idx += 8)
		{
			des(pUcMessage + idx, ucEncryptedKey, DEAmoEncS);
		}

		strResult = string("\x00\x00", 2);				// len
		strResult.append("\x90\x10\x03\x80\x10", 5);	// tpdu

		strResult.append((char*) pUcMessage, nFullLength);

		DELETE_POINTER_ARR(pUcMessage);
	}
	
	if (m_eTProtocol != FDATA){
		long_hex((LPBYTE)strResult.buffer(),2,nLen);
	}
//	strResult.buffer()[1] = (nLen /10) * 16+ (nLen % 10) ;
//	strResult.buffer()[0] = 0 ;



	return strResult;
}
//------------------------------------------------------------------------------
bool CMegaContainer::decomposeMessage(const string &strMessage, TPROTOCOL eTProtocol /* = ISO */)	//!
{
	DBGLine("Decompose message");
//	DBGBuf("message", (BYTE*) strMessage.buffer(), strMessage.length());

	//==============================================================================================
	// Шифрование трафика
	if (g_pConfig->isEncUse())
	{
		// Ключ надо взять сохраненный из ФС

		BYTE ucEncryptedKey[]	= {0x9B, 0xA4, 0xD3, 0x6B, 0x68, 0xF2, 0x88, 0x20};
		BYTE ucKeyForDecrypt[]	= {0x72, 0xB2, 0x55, 0xBD, 0x59, 0x27, 0x13, 0x1A};

		des(ucEncryptedKey, ucKeyForDecrypt, DEAmoDecS);

		for (int idx = 0; idx < strMessage.length(); idx += 8)
		{
			des((BYTE*) strMessage.buffer() + idx, ucEncryptedKey, DEAmoDecS);
		}
	}

//	DBGBuf("message", (BYTE*) strMessage.buffer(), strMessage.length());
	
	tMapFieldFormats& g_mapFieldFormats = *g_pMapFieldFormats;

	const char* pCur;
	pCur =  strMessage.buffer();

	// Mtid
	char strMTID[ISO_LEN_MSG_TYPE + 1] = {0};
	// Mask bit
	char bitMask[16] = {0};
	bool bIsMask16byte = false;		// По умолчанию 8 байт

	if (m_eTProtocol == FDATA){
	  asc2bcd(strMTID,(char *)pCur,(ISO_LEN_MSG_TYPE*2));
	  m_strMTID = strMTID;
	  pCur += (ISO_LEN_MSG_TYPE*2);

	  asc2bcd(bitMask, (char *)pCur, 16);
		pCur += 16;
		DBGBuf("bitMask", (LPBYTE)bitMask,8);

		if (bitMask[0] & 0x80)
		{
			bIsMask16byte = true;
			asc2bcd(bitMask + 8, (char *)pCur, 16);
			pCur += 16;
			DBGBuf("bitMask", (LPBYTE)bitMask+8,8);
		}
	}
	else {
	  memcpy(strMTID, pCur, ISO_LEN_MSG_TYPE);
	  m_strMTID = strMTID;
	  pCur += ISO_LEN_MSG_TYPE;

	  memcpy(bitMask, pCur, 8);
	  pCur += 8;
	  if (bitMask[0] & 0x80)
	  {
		bIsMask16byte = true;
		memcpy(bitMask + 8, pCur, 8);
		pCur += 8;
	  }

	  DBGBuf("bitMask", (LPBYTE)bitMask,8);
	}
	// Проход по всей маске
	int nTotal = 64;
	if (bIsMask16byte)
		nTotal = 128;

	//==============================================================================================
	// Запоминаем 72 поле, чтобы можно было потом распарсить, в зависимости от условий
	string strField72Composed;

	for (int bit = 2; bit <= nTotal; bit++)
	{
		if (bitMask[(bit - 1) / 8] & (1 << (7 - ((bit - 1) % 8))))
		{
			string strKey(bit);
			string strValue;

			// На основе ключа (номер поля) формируем правильное сообщение
			tMapFieldFormats::iterator itFormat = g_mapFieldFormats.find(strKey);
			if (itFormat == g_mapFieldFormats.end())
			{
				DBGLine("CMegaContainer::decomposeMessage, format: \"%s\" is not found", strKey.buffer());
				DBGLine("Next mem: %s", pCur);
				return false;
			}
			SFieldFormat& stcFormat = itFormat->second;
			if (stcFormat.eFieldType == FIXED_N || stcFormat.eFieldType == FIXED_AN || stcFormat.eFieldType == FIXED_BCD)
			{
				char* pField = new char[stcFormat.nLen + 1];
				memset(pField, 0, stcFormat.nLen + 1);
				memcpy(pField, pCur, stcFormat.nLen);
				pCur += stcFormat.nLen;
				strValue = pField;
				DELETE_POINTER_ARR(pField);
			}
			if (stcFormat.eFieldType == FIXED_BIN)
			{
				char* pStrClear = new char[stcFormat.nLen + 1];
				memset(pStrClear, 0, stcFormat.nLen + 1);
				BCDRight2str((BYTE*) pCur, (BYTE*) pStrClear, stcFormat.nLen);
				pCur += getLengthBCD(stcFormat.nLen);
				strValue.append(pStrClear);
				DELETE_POINTER_ARR(pStrClear);
			}
			if (stcFormat.eFieldType == FLEXIBLE_Z)
			{
			  char strLength[11] = {0};
			  BCDRight2str((BYTE*) pCur, (BYTE*) strLength, stcFormat.nLen);
			  pCur += getLengthBCD(stcFormat.nLen);

			  int nLength = atoi(strLength);
			  DBGLine("len %d",nLength);
			  char* pStrClear = new char[nLength + 1];
			  memset(pStrClear, 0, nLength + 1);
			  zL2_to_str(pStrClear,(LPTSTR)pCur,nLength);
//			  memcpy(pStrClear, pCur, getLengthBCD(nLength));
			  pCur += getLengthBCD(nLength);
			  strValue.append(pStrClear,nLength);
			  DELETE_POINTER_ARR(pStrClear);

			}
			if (stcFormat.eFieldType == FLEXIBLE)
			{
				char strLength[11] = {0};
				if (m_eTProtocol == FDATA){
				  memcpy (strLength,pCur,stcFormat.nLen);
				  pCur += stcFormat.nLen;
				} else {
				BCDRight2str((BYTE*) pCur, (BYTE*) strLength, stcFormat.nLen);
				pCur += getLengthBCD(stcFormat.nLen);
				}
				int nLength = atoi(strLength);
				DBGLine("len %d",nLength);

				char* pStrClear = new char[nLength + 1];
				memset(pStrClear, 0, nLength + 1);
				memcpy(pStrClear, pCur, nLength);
				pCur += nLength;
				strValue.append(pStrClear,nLength);
				DELETE_POINTER_ARR(pStrClear);
			}


//			DBGPrt("For key: %s, decomposed: ", strKey.buffer());
//			DBGBuf("",(BYTE*) strValue.buffer(), min(strValue.length(),100));

			set(strKey, strValue);

			

			if (pCur - strMessage.buffer() > strMessage.length())
			{
				DBGLine("Warning CMegaContainer::decomposeMessage, out bounds of string");
			}
		}
	}
	return true;
}
