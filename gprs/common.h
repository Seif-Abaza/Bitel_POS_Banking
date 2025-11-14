
#ifndef __STRUCTES_H
#define __STRUCTES_H

#include "../internal/CParam.h"
#include "../internal/tools.h"

#define COM_GPRS			COM1
#define COM_ETHERNET		COM2

#define	AT_ANS_BUF_SIZE		100 // Размер буфера под ответ на AT команду.
#define	AT_CMD_BUF_SIZE		100	// Размер буфера под форматирование AT команды.
#define CLEAR_SMS_PERIOD	3

#define STX					0x02
#define ETX					0x03
#define EOT					0x04
#define ENQ					0x05
#define ACK					0x06
#define NAK					0x15
#define SI					0x0F
#define SO					0x0E
#define FS					0x1C
#define RS					0x1E
#define US					0x1F

#define	MANUAL_RAS				-1	// идентификатор того, что RAS-таблица задается вручную

#define CONINFO_BUF_SIZE		33
#define DUT_MSG_MAX_SIZE		4200			// максимальный размер DUT сообщения

#define	CFG_MAX_SIZE			4200			// максимальный размер файла конфигурации (+ заголовки)
#define TLV_MAX_SIZE			4096			// максимальный размер TLV файла конфигурации
#define	SCARD_INFO_FILE_NAME	"MSV3SC.DAT"	// имя файла с информацией о криптокарте
#define	MSV3_CFG_FILE_NAME		"MSV3.CFG"		// имя файла конфигурации
#define RAS_TABLE_FILE_NAME		"rastable.tbl"	// файл с RAS таблицей

#ifndef Gmin
#define Gmin(X, Y) ((X) < (Y) ? (X) : (Y))
#endif

#ifndef Gmax
#define Gmax(X, Y) ((X) > (Y) ? (X) : (Y))
#endif


#ifndef SLEEP
#define SLEEP(dwMs)							\
	{										\
		DWORD dwStart = GetTickCount();		\
		while (true)						\
		{									\
			if (IsExpired(dwStart, dwMs))	\
				break;						\
			if (checkCancelButtom(oParam))	\
				return false;				\
		}									\
	}
#endif

#ifndef CHECK_AT
#define CHECK_AT(atExecute)						\
	{											\
		if (checkCancelButtom(oParam))			\
			return false;						\
		if (!(atExecute))						\
		{										\
			DBGLine("Error: %s", (#atExecute));	\
			showImp();                          \
			return false;						\
		}										\
	}											
#endif

extern "C"
{
	extern void MODEM_SEL(void);
	extern void DTR_HIGH(void);
	extern void SIM_SELECT(char);
};

//------------------------------------------------
typedef struct _SCARD_INFO	// данные о крипто-карте
{
	BYTE	scard_sn[32];			// серийный номер карты
	BYTE	scard_pers_data[64];	// файл персонализационный (ключ доступа к карте, зашифрованный на EUTRON)
} SCARD_INFO;
//------------------------------------------------
typedef struct _SSAM_DATA
{
	DWORD	dwNode;
	BYTE	ap3Key[32];
} SSAM_DATA;
//------------------------------------------------
typedef struct _SConnInfo
{
	char	pIp[16];
	WORD	wPort;
	DWORD	dwGPRSInitTime;
	DWORD	dwTCPIPConnectTime;
	WORD	wServerNode;
	WORD	wKeepAlive;
	WORD	wPingAnswerMaxTime;
} SConnInfo;
//------------------------------------------------
typedef struct _SRASElement
{
	int		nImsi;
	char	pAPN[CONINFO_BUF_SIZE];
	char	pLogin[CONINFO_BUF_SIZE];
	char	pPassword[CONINFO_BUF_SIZE];
	char	pGateway[16];
	int		nAutoDisc;
} SRASElement;
//------------------------------------------------
typedef struct _SConnStat
{
	BYTE						ActSim;
	BYTE						SimStates[2]; // 0 - не проверялась, 1 - рабочая // 0 - do not check, 1 - working
	std::vector<SRASElement*>	vecpstcRASTable;

	~_SConnStat()
	{
		for (int i = 0; i < vecpstcRASTable.size(); ++i)
		{
			DELETE_POINTER(vecpstcRASTable[i]);
			++i;
		}
	}
} SConnStat;
//------------------------------------------------
typedef struct
{
	WORD  EncrypredSize;
	WORD  OriginalSize;
	DWORD OriginalCRC32;
}
TEncryptionControlBlock, ECB;
//------------------------------------------------
typedef struct
{
	DWORD	dwBlockNumber;
	DWORD	Reserv;
}
TBlockSignature;
//------------------------------------------------
// заголовок блока аутентификации
typedef struct
{
	WORD  Ver;  // версия блока аутентификации канала
	WORD  DUTChannelIdentifier; // идентификатор запрашиваемого узла
	DWORD KeepAliveRead;  // KEEPALIVE по чтению, мсек
	DWORD KeepAliveSend;  // KEEPALIVE по записи, мсек
	WORD  cX6;  // тип протокола линии
	WORD  cX20; // параметр протокола
	WORD  cX21; // параметр протокола
	WORD  EncryptionType; // запрашиваемый тип защиты трафика
	DWORD StartBlockNumber; // запрашиваемый номер первого блока данных в защищённой передаче, случайный параметр GetTickCount
	DWORD LSN;  // LSN, случайный параметр GetTickCount запуска программы + количество разнообразных сессий на данный момент
}
STDCAB_AP3;
//------------------------------------------------
typedef struct _SSYSTEMTIME	// структура дата-время, для синхронизации с сервером времени
{
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
}
SSYSTEMTIME;
//------------------------------------------------
class IConnection
{
public:
	virtual ~IConnection() {}

	virtual bool	initialize(const SConnInfo& stcConnInfo) = 0;
	virtual bool	connect(SConnStat& stcConnStat, CParam& oParam) = 0;
	virtual void	disconnect(CParam& oParam) = 0;
	virtual int		run() = 0;
	virtual WORD	readData(BYTE* pBuf, int size, CParam& oParam) = 0;
	virtual bool	writeData(BYTE* pBuf, int size) = 0;
	virtual bool	clearSMS(CParam& oParam) = 0;

	virtual	bool	isCanceled() const = 0;
	virtual bool	isConnected() const = 0;
	virtual char*	getTeminalNumber() = 0;
	virtual	WORD	getServerNode() const = 0;
	virtual	WORD	getKeepAlive() const = 0;
	virtual	DWORD	getPingAnswerMaxTime() const = 0;
	virtual	BYTE	getCSQ() const = 0;
	virtual bool	getIMSI(int* nImsi) = 0;
	virtual DWORD	getSmsCount() const = 0;
	virtual char*	getOperatorName() const = 0;
	virtual bool	checkRxInput() const = 0;
	virtual bool	openSocket(CParam& oParam) = 0;
	virtual bool	closeSocket(CParam& oParam) = 0;
	virtual bool	isSocketOpen()const = 0;
};
//------------------------------------------------

#endif

