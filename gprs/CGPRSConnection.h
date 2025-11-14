
#ifndef __CGPRS_CONNECTION_H
#define __CGPRS_CONNECTION_H

#include "common.h"
#include "../tasks/CTaskDisplay.h"

void startHypTerm();
class CGPRSConnection : public IConnection
{
	class CCancelLocker
	{
	public:
		CCancelLocker(CGPRSConnection* pConnection) : m_pConnection(pConnection) { m_pConnection->m_bAlowCanceling = false; }
		~CCancelLocker() { m_pConnection->m_bAlowCanceling = true; }

	private:
		CGPRSConnection*	m_pConnection;
	};

public:
	CGPRSConnection();
	~CGPRSConnection();

	virtual bool	initialize(const SConnInfo& stcConnInfo);
	virtual bool	connect(SConnStat& stcConnStat, CParam& oParam);
	virtual void	disconnect(CParam& oParam);// { m_bConnected = false; }
	virtual int		run();
	virtual WORD	readData(unsigned char* buf, int size, CParam& oParam);
	virtual bool	writeData(unsigned char* buf, int size);
	virtual bool	clearSMS(CParam& oParam) { return true; }
	virtual bool	openSocket(CParam& oParam);
	virtual bool	closeSocket(CParam& oParam);

	virtual	bool	isCanceled() const { return m_bIsCanceled; }
	virtual bool	isConnected() const { return m_bConnected; }
	virtual char*	getTeminalNumber() { return 0; }
	virtual WORD	getServerNode() const { return m_stcConnInfo.wServerNode; }
	virtual	WORD	getKeepAlive() const { return m_stcConnInfo.wKeepAlive; }
	virtual	DWORD	getPingAnswerMaxTime() const { return m_stcConnInfo.wPingAnswerMaxTime; }
	virtual	BYTE	getCSQ() const { return m_CSQ; }
	virtual DWORD	getSmsCount() const { return m_dwSmsCount; }
	virtual char*	getOperatorName() const { return m_pOperatorName; }
	virtual bool	checkRxInput() const { return true; }
	virtual bool	isSocketOpen()const {return m_bSocketOpen;}

protected:
	bool	__connect(const SConnStat& stcConnStat, CParam& oParam);

	bool	checkCancelButtom(CParam& oParam);
	bool	chillout(CParam& oParam, DWORD dwTimeout);

	void	updateIcons() const;

	void	showOk() const;
	void	showImp() const;
	void	showCanc() const;



	// Настройка модема
	bool	setupModem(CParam& oParam);

	// Сменить симку:
	//    false - симка плохая
	bool	changeSim(SConnStat& stcConnStat,CParam& oParam);

	bool	checkSim(char* strState, WORD size,CParam& oParam);

	// Выполнить AT команду и получить ответ.
	// Параметры:
	//   timeout      - Максимальное время ожидания ответа.
	//   prefix       - Префикс ожидаемого ответа, может быть NULL.
	//   ans_buf      - Буфер под ответ, может быть NULL.
	//   ans_buf_size - Размер буфера под ответ.
	//   cmd          - Строка форматирования AT команды, и далее аргументы форматирования.
	// Возвращаемое значение: true в случае успеха, false - ошибка.
	bool	atExecute(CParam& oParam,DWORD timeout, char* prefix, char* ans_buf, WORD ans_buf_size, char* cmd, ...);
	bool	atPosfixExecute(DWORD timeout, char* prefix, char* ans_buf, WORD ans_buf_size, char* cmd, ...);

	// Чтение 1 байта из сокета.
	// возвращает:
	//    1 - считан 1 байт, результат в byte.
	//    0 - ничего не считано.
	//   -1 - сокет закрыт.
	int		readByte(unsigned char *byte);

	// Запрос ID оператора (первых 5 цифр)
	bool	getIMSI(int* pImsi);
	bool	_getOperatorName(char* pOut, BYTE actSim, CParam& oParam);
	BYTE	_getCSQ(CParam& oParam);

	// Достает данные из RAS таблицы
	bool	getRAS(const SConnStat& stcConnStat);

protected:
	bool			m_bConnected;
	bool			m_bOldModem;
	SConnInfo		m_stcConnInfo;
	BYTE			m_HelpBuf[2]; // Вспомогательный буфер, для входного потока данных с сокета.
	WORD			m_wHelpBytesCount; // Счётчик байт в буфере.
	DWORD			m_dwLastCall;
	bool			m_bIsCanceled;
	int				m_nActRasInd;
	bool			m_bAlowCanceling;
	bool			m_bSocketOpen;
	static DWORD	m_dwDiscCount;
	static DWORD	m_dwSmsCount;
	static DWORD	m_dwLastSent;
	static DWORD	m_dwLastRecv;
	static bool		m_bInitialized;
	static BYTE		m_CSQ;
	static char		m_pOperatorName[DISPLAY_WIDTH + 1];
};

#endif
