
#ifndef __CMEGA_CONTAINER_H
#define __CMEGA_CONTAINER_H

#include "common.h"
#include "../internal/string.h"

class CBaseMegaContainer;
class CTransactionContainer;
class COpenWayConverter;
class CCompassPlusConverter;
//------------------------------------------------------------------------------
typedef std::map<string, string>				mapFields;
typedef std::map<string, CBaseMegaContainer*>	mapAdditionalFields;
//------------------------------------------------------------------------------
struct SPowerCardHeader
{
	char	strSize[4];
	char	strProductCode[1];
	char	strSoftwareVersion[5];
	char	strErrorPos[3];
};
//------------------------------------------------------------------------------
class CBaseMegaContainer
{
public:
	CBaseMegaContainer(const CBaseMegaContainer& rightContainer);
	virtual ~CBaseMegaContainer();

	virtual void				set(const string& strField, const string& strValue);
	virtual string				get(const string& strField) const;
	virtual	const mapFields*	getFields(const string& strContainer) const;

	virtual	void	clear();
	virtual	void	deleteField(const string& strField);
	virtual	void	deleteEmptySubContainers();

	virtual const string&	getSelfKey() const	{ return m_strSelfKey; }
	virtual	bool			isEmpty() const		{ return (m_mapFields.empty() && m_mapAdditionalFields.empty()); }

	virtual	DWORD	fillJournalEntity(char* pJournalEnt) const;
	virtual void	fillFromJournalEntity(const char* strJournalEnt);

	virtual void	debugContainer() const;
	
	virtual string	composeMessage(TPROTOCOL eTProtocol) const							= 0;
	virtual bool	decomposeMessage(const string& strMessage, TPROTOCOL eTProtocol)	= 0;

	CBaseMegaContainer&	operator=(const CBaseMegaContainer& rightContainer);
	CBaseMegaContainer&	operator+=(const CBaseMegaContainer& rightContainer);

protected:
	virtual CBaseMegaContainer*	_clone() const							= 0;


protected:
	friend class CMegaContainer;
	friend class COpenWayConverter;
	friend class CCompassPlusConverter;

	CBaseMegaContainer(CBaseMegaContainer* pParent) : m_pParent(pParent) {}

	void				_setParent(CBaseMegaContainer* pParent)	{ m_pParent = pParent; }
	CBaseMegaContainer*	_getParent() const						{ return m_pParent; }
	void				_constructSelfKey(const string& selfId);

	void	_setField(const string& fieldName, const string& strValue);

	string	_getField(const string& fieldName) const;
	string	_getSubField(const string& fieldName, const string& subFieldName) const;

	const mapFields*	_getSubFields(const string& containerName, const string& subContainerName) const;

	void	_deleteField(const string& fieldName);
	void	_deleteSubField(const string& fieldName, const string& subFieldName);

	string	_getFieldName(const string& strField) const;
	string	_getSubFieldName(const string& strField) const;	
	string	_getSubFieldNameLast(const string& strField) const;	

protected:
	CBaseMegaContainer*	m_pParent;
	string				m_strSelfKey;
	mapFields			m_mapFields;
	mapAdditionalFields	m_mapAdditionalFields;
};
//------------------------------------------------------------------------------
class CMegaContainer : public CBaseMegaContainer
{
public:
	CMegaContainer()
		: CBaseMegaContainer(0), m_strMTID("uninit"), m_bUseMac(false)			{ }
	CMegaContainer(const string& strMTID, const string& strProcCode)
		: CBaseMegaContainer(0), m_strMTID(strMTID), m_bUseMac(false)			{ set(FIELD_PROCESS_CODE, strProcCode); }
	CMegaContainer(const CMegaContainer& rightContainer);
	CMegaContainer(TPROTOCOL eTProtocol)
			: CBaseMegaContainer(0), m_strMTID("uninit"), m_bUseMac(false), m_eTProtocol(eTProtocol)			{ }

	static void	initialize();

	void			setType(const string& strMTID, const string& strProcCode);
	const string&	getMTID() const												{ return m_strMTID; }
	string			getProcCode() const											{ return get(FIELD_PROCESS_CODE); }
	TPROTOCOL	   getHostProtocol() const											{ return m_eTProtocol; }
	
	bool	isAnswerOk() const;
	bool	operator==(const CMegaContainer& oContainer) const;

	virtual	void	debugContainer() const;

	virtual string	composeMessage(TPROTOCOL eTProtocol = ISO) const;
	virtual bool	decomposeMessage(const string& strMessage, TPROTOCOL eTProtocol = ISO);	

	void			setUseMac(bool bUseMac) { m_bUseMac = bUseMac; }

	friend class COpenWayConverter;
	friend class CCompassPlusConverter;

protected:
	virtual CBaseMegaContainer*	_clone() const { return new CMegaContainer(*this); }
	
private:
	string	m_strMTID;
	bool	m_bUseMac;
	TPROTOCOL m_eTProtocol;
};

//------------------------------------------------------------------------------
// --- Обозначает формат полей
enum FIELD_TYPE
{	
	FIXED_N,
	FIXED_AN,
	FIXED_BIN,
	FLEXIBLE,
	FLEXIBLE_Z,
	FIXED_BCD,
};
struct SFieldFormat
{
	FIELD_TYPE eFieldType;
	int nLen;	
	SFieldFormat() {}
	SFieldFormat(FIELD_TYPE eFieldType, int nLen) {this->eFieldType = eFieldType; this->nLen = nLen;}
};
typedef std::map<string, SFieldFormat>	tMapFieldFormats;
extern tMapFieldFormats* g_pMapFieldFormats;
// ---

//------------------------------------------------------------------------------
// --- Просто хранит mtid и proc code
struct SOperationType
{
	string	strMTID;
	string	strProcCode;
	SOperationType() {}
	SOperationType(string strMTID, string strProcCode) { this->strMTID = strMTID; this->strProcCode = strProcCode; }
	bool operator < (const SOperationType& stcOp) const
	{
		if (strMTID < stcOp.strMTID)
			return true;
		else if (strMTID != stcOp.strMTID)
			return false;
		return strProcCode < stcOp.strProcCode;
	}
};
// ---

#endif
