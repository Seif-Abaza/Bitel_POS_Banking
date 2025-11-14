
#ifndef __CTAST_MSV3_H
#define __CTAST_MSV3_H

#include "../internal/types.h"
#include "../internal/CParam.h"
#include "../internal/CGlobalConfig.h"
#include "common.h"

#define				MAX_BACKUP_HOSTS	5
static const int	nTryConnectN		= 10;

class CTaskConnect
{
public:
	CTaskConnect(bool bManual = false) : m_bManual(bManual) {}
	~CTaskConnect();

	virtual void		start(CParam& oParam);
	virtual int			run(CParam& oParam);
	virtual	void		stop(CParam& oParam);



	// подключение к серверу
	bool	connect(CParam& oParam, SConnInfo* stcConnInfo = 0);
	void	connectLater()	{ m_bConnectionImposible = false; manualRun(); }
	// приостановить работу msv3
	void	manualStop() { m_bManual = true; }
	// запустить msv3
	void	manualRun() { m_bManual = false; }
	// добавить элемент RAS таблицы в конец списка
	void	addRASElement(SRASElement* pRASElement);
	// Берет текущий imsi с сим карты, делегирует обращение к GPRS connection
	bool	getIMSI(int* nImsi);
	// Удалить все смс с симкарты
	bool	clearSMS(CParam& oParam);
	DWORD	getSmsCount() const { return m_vecpServers[m_dwActServer]->getSmsCount(); }
	char*	getActTerminalNumber() { return m_vecpServers[m_dwActServer]->getTeminalNumber(); }
	char*	getOperatorName() { return m_vecpServers[m_dwActServer] ? m_vecpServers[m_dwActServer]->getOperatorName() : 0; }

	void	setCurHostIndx(const DWORD& dwCurHost)	{ m_dwCurHostIndx = dwCurHost; m_dwCurBackupHost = 1; }
	void	setCurBackupHost(const DWORD& dwBackup)	{ m_dwCurBackupHost = dwBackup; }
	DWORD	getCurHostIndx() const					{ return m_dwCurHostIndx; }
	DWORD	getCurBackupHost() const				{ return m_dwCurBackupHost; }

private:
	void	__loadRASTable();
	void	__loadDNTTable();

	// обработать пакет на отсылку
	bool	processInternalMsg(CParam& oParam);

	// если нажата кнопка cancel, возвращает true и отменяет попытки конекта
	bool	checkCancelButtom(CParam& oParam);

	
private:
	std::vector<IConnection*>	m_vecpServers;
	DWORD						m_dwActServer;
	DWORD						m_dwCurBackupHost;
	DWORD						m_dwCurHostIndx;
	SConnStat					m_stcConnStat;
	DWORD						m_dwSendBlockNumber;
	DWORD						m_dwRcvBlockNumber;
	DWORD						m_dwEncryptionType;  // 3 - DES, 2 - GOST
	DWORD						m_dwKeysAp3Imito[32/4];
	BYTE						m_BufIn[2 + DUT_MSG_MAX_SIZE];
	DWORD						m_dwNOfBytes;
	DWORD						m_dwoldNOfBytes;
	DWORD						m_dwKeepAliveRcvTime;
	DWORD						m_dwKeepAliveSentTime;
	bool						m_bKeepAliveSent;
	bool						m_bMCPlaced;
	DWORD						m_dwAuthTimeout;
	bool						m_bConnectionImposible;
	bool						m_bHaveTLV;
	bool						m_bManual;
};

extern CTaskConnect*	g_pConnTask;

#endif
