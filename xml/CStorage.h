//---------------------------------------------------------------------------
/****************************************************************************
* История изменений:
* $Log: CStorage.h,v $
* Revision 1.3  2010/06/09 06:04:45  rakhmatulin
* no message
*
*
***************************************************************************/
//---------------------------------------------------------------------------

#ifndef __CSTORAGE_H
#define __CSTORAGE_H

#include "xml_common.h"
#include "../internal/string.h"

class IVariable;
class CMegaContainer;
class CStorage;

//---------------------------------------------------------------------------
// Класс хранит все значение вариэйблов, обращаться можно по ключу, если не существует - создает
class CStorage
{
public:
	// Сразу обозначает g_pStorage
	CStorage();

	// Очищает все вариэйблы
	~CStorage();

	// Требуется вызвать при начале работе XML app (создает контейнеры транзакций)
	void init();

	// Требуется вызвать в конце работы XML app (очищает контейнеры транзакций)
	void deinit();

	// Вызывать после получения ответа от транзакции
	void transactionReceived(CMegaContainer* pMegaContainer);

	// Очищает все полностью
	void clear();
	
	// Получает контейнер по развернутому ключу
	IVariable* getVariable(STORAGE_TYPE eType, const string& strID, bool bIsSystem = false);
	IVariable* getVariable(const string& m_strComplexTypeID, const string& m_strElementID);

	// На этапе парсинга xml, складывает на каждое имя дисплея - дисплейная строка
	void addDisplay(const string& strDisplayID, const SDisplay& stcDisplay);

	void addReceipt(const string& strReceiptID, const SReceipt& stcReceipt);

	bool getReceipt(const string& strReceiptID, char* pDstBuf);

	// Устанавливает текущий дисплей по ID
	void setDisplay(const string& strDisplay);

	// Перерисовывает экран
	void printScreen();

	// Получает чистую дисплейную строку без обработки (необходимо для меню)
	const string& getMessageString(const string& strDisplayID);

	// На этапе парсинга xml, сложить соотвествие номеру паттерну - значение variable
	void addPatternVariable(int nPattern, const SVariable& stcVariable);

	// Получает готовую строку, которую нужно подставить на место паттерна для дисплея
	string getPatternValue(int nPattern);

	// Получает текущий контейнер для транзакций по типу
	CMegaContainer* getTransactionContainer(TARGET_TYPE eTarget) const { return m_pMegaContainers[eTarget]; }

	// Устанавливает режим использования xml дисплея
	void useXmlDisplay(bool bUseXmlDisplay);

private:
	// Ключ для поиска variable
	class CKey
	{
	public:
		CKey(STORAGE_TYPE eType, const string& strID);
		bool operator < (const CKey& oKey) const;
		friend class CStorage;

	private:
		STORAGE_TYPE		m_eType;
		string				m_strID;
	};

	class CXKey
	{
	public:
	  CXKey(const string& strComplexTypeID, const string& strElementID);
		bool operator < (const CXKey& oKey) const;

//	  bool operator == (const CXKey& oKey) const;
	  friend class CStorage;

	private:
		char        m_cCOrder;
		char        m_cEOrder;
		string		m_strComplexTypeID;
		string		m_strElementID;
	};

	struct SPVariable
	{
		IVariable*	pVariable;
		bool		bIsSystem;
	};

	// Получает контейнер по свернутуму ключу
	IVariable* getVariable(const CKey& oKey, bool bIsSystem);	
	IVariable* getVariable(const CXKey& oKey);

	// Контейнер всех variable
	std::map<CKey, SPVariable>			m_mapVariables;
	std::map<CXKey, SPVariable>			m_mapACPVariables;

	// Контейнер, хранит сами шаблоны, ключ - имя шаблона
	std::map<string, SDisplay>			m_mapDisplays;

	std::map<string, SReceipt>			m_mapReceipts;
	
	// Контейнер для соответствия id паттерна и связующая переменная
	std::map<int, SVariable>			m_mapPatternVariable;

	// Текущий дисплей
	string								m_strCurrentDisplay;

	// Контейнеры транзакций, массив указателей
	CMegaContainer*						m_pMegaContainers[TT_CNT];

	// Флаг использования стандартного дисплея
	bool								m_bUseXmlDisplay;
};

extern CStorage*		g_pStorage;


//---------------------------------------------------------------------------
// Представляет собой абстрактное хранилище значений string
class IVariable
{
public:
	virtual ~IVariable() {}
	virtual void store(const string& strData, FILTER_TYPE eFilter = FLT_NONE) = 0;
	virtual string get(FILTER_TYPE eFilter = FLT_NONE) = 0;
	virtual void clear() = 0;
};

//---------------------------------------------------------------------------
// Хранит type="global" значение
// <storage type="global" var="app_num" />
class CGlobalVariable : public IVariable
{
public:
	void store(const string& strData, FILTER_TYPE eFilter = FLT_NONE);
	string get(FILTER_TYPE eFilter = FLT_NONE);
	void clear();

private:
	string	m_strData;
};

//---------------------------------------------------------------------------
// Хранит поля транзакций
// <storage type="transaction" target="receive" var="62/92/00000038" />
class CTransactionVariable : public IVariable
{
public:
	CTransactionVariable(TARGET_TYPE eTarget, string strKey);
	void store(const string& strData, FILTER_TYPE eFilter = FLT_NONE);
	string get(FILTER_TYPE eFilter = FLT_NONE);
	void clear() {}

private:
	TARGET_TYPE	m_eTarget;
	string		m_strKey;
};


#endif
