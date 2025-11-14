#pragma once

#ifndef __CTLV_PARSER__
#define __CTLV_PARSER__

#include "types.h"

#include "string.h"

extern const unsigned char tlvrawData[];
#define MAX_FILENAME_SIZE 255;
struct SADM100
{int *nVal;};

struct SDataCnt
{ 
	char* data;
	int tag;
	int dataSize;

};

struct GUID
{
	unsigned long Data1;
	unsigned short Data2;
	unsigned short Data3;
	char Data4[8];

};


class CTLVTag;

enum eTermConType {COM,TCPIP,UDP};

class CTLVParser
{
public:
	enum TAG_DATA_TYPE{TAG_DATA_STRING,TAG_DATA_DWORD,TAG_DATA_INT,TAG_DATA_RINT,TAG_DATA_STRING_ARRAY,
						TAG_DATA_DWORD_ARRAY,TAG_DATA_INT_ARRAY,TAG_DATA_ADM100,TAG_INVALID};

	enum TAG_TYPE{TAG_AUTH_TIME=0x0A,TAG_TRACE_ANSWER=0x19,TAG_SERAL_PORT_SETTINGS=0x1f,TAG_SERAL_PORT_BAUD_RATE=0x1e,TAG_BASE_DIONE=0x22,TAG_DEVICE_GUID=0x24,
					TAG_BASE_BITEL=0x26,TAG_TERMINAL_CONNECTION_TYPE=0x2b,TAG_BASE_BITEL_EMV=0x29,
					TAG_ADMIN_ARRAY=0x2D,TAG_ADMIN_NODE=0x2E,TAG_RAS_ARRAY=0x32,TAG_DNT_ARRAY=0x3C, TAG_DEVICE_TYPE=0x02,TAG_ADM100=0x100,
					TAG_ADM100_ELEMENT=0x101,TAG_MY_NODE=0x110,TAG_MY_NODE_INTERFACE=0x111,TAG_SERIAL_PORT_HARDWARE_FLOW_CONTROL=0x112,
					TAG_CARD_READ_TIMEOUT=0x118,TAG_INPUT_PIN_TIMEOUT=0x119,TAG_OPEN_TIMEOUT=0x123,TAG_AUTH_TIMEOUT=0x124,TAG_CONFIRM_TIMEOUT=0x125,
					TAG_TERM_NODE=0x120,TAG_TERM_INTERFACE=0x121,TAG_HOST_NODES_ARRAY=0x130,TAG_HOST_NODE=0x131,TAG_HOST_PROTOCOL=0x132,
					TAG_TERM_PROTOCOL=0x133,TAG_USE_LOGON=0x115,TAG_LOAD_TMK=0x116,TAG_TPK_LOAD_RERIOD=0x117,TAG_CHECK_EXP_DATE=0x127,
					TAG_HOST_BACKUP_INDEX=0x136,TAG_HOST_INTERFACE=0x141,TAG_TERMINAL_ID=0x14E,TAG_CURRENCY_ID=0x151,
					TAG_CHECK_HEADER=0x152,TAG_LANGUAGE=0x153,TAG_LOG_LEVEL=0x154,TAG_FILE_ARRAY=0x160,
					TAG_FILE_NAME=0x161,TAG_FILE_CONTENT=0x162,TAG_CORE_NAME=0xF0,TAG_CORE_VERS=0xF1,
					TAG_CORE_BODY=0xF2,TAG_MERCHANT_ID=0xE0,TAG_ACQUIRER_ID=0xE1,TAG_POS_SERIAL=0xE2,
					TAG_TERM_CAPA=0xE3,TAG_TERM_CAPA_ADD=0xE4,TAG_TERM_TYPE=0xE5,TAG_TERM_TCC=0xE6,
					TAG_TERM_MCC=0xE7,TAG_TERM_FORCE_ONLINE=0xE8,TAG_TERM_L2TTA_MODE=0xE9,TAG_EMV_ENABLED=0xEB,
					TAG_WAIT_CONNECT=0x3A,TAG_USE_PAYMENTS=0x39,TAG_CONTACTLESS_ON=0x42, TAG_TERMINAL_OPERATIONS = 0x6D,
					TAG_GPRS_ON=0x43,TAG_CONTACTLESS_TID=0xD8,TAG_TRANSPORT_TYPE=0xD9, TAG_RRN_FOR_REFUND = 0x113,
					TAG_PIN_TABLE=0xD3,TAG_BIN_TABLE=0xD4,TAG_TERM_INDEX=0xD7,TAG_TRANSPORT_SECTOR=0xDA,
					TAG_SOCIAL_SECTOR=0xDB,TAG_TARIF=0xDC,TAG_KEY_A_TRANSPORT=0xDD,TAG_KEY_B_TRANSPORT=0xDE,
					TAG_KEY_A_SOCIAL=0xDF,TAG_ERROR_MESSAGE_TYPE=0x93,TAG_PROC_TIME_ZONE=0x97,TAG_PRN_CHECK_NUM=0x94,
					TAG_ALWAYS_PRN_2CHECK=0x95,TAG_BANK_SPECICIFIC_FLAGS=0x92,TAG_MICRIC_CONNECT_TIMEOUT=0xEE,
					TAG_JRN_UPLOAD_ARRAY=0x90,TAG_GAI_COMMIS=0x9C, TAG_CARD_TERM = 0x6A, TAG_BANK_DATA = 0x6C, TAG_CURRENCY_CHANGE = 0x6B,
					TAG_CURRENCY_TABLE=0xD5, TAG_TPL_CHECKS=0xD6, TAG_DISPLAY=0xD2, TAG_TERMINAL_NUMBERS=0xD7,
					TAG_EMV_MERCHANT_ID=0xE0, TAG_EMV_ACQUIRER_ID=0xE1, TAG_EMV_POS_SERIAL_NUM=0xE2, TAG_EMV_TRM_CAPA=0xE3,
					TAG_EMV_TRM_CAPA_ADDITIONAL=0xE4, TAG_EMV_TRM_TYPE=0xE5, TAG_EMV_TRM_TCC=0xE6, TAG_EMV_TRM_MCC=0xE7,
					TAG_EMV_TRM_FORCE_ONLINE=0xE8, TAG_EMV_USE_PIN=0xEA, TAG_CHANGE_TERM_ENBL = 0x11A,
					TAG_TRANSACTION_ERRORS=0xD1, TAG_SERVER_NODE_VERIFICATION = 0x59, TAG_CHECK_CONST = 0x9B,
					TAG_CHECK_OPERATION_SUPPORT=0x11B, TAG_FUEL_TABLE=0x57, TAG_APPLICATION_TYPE=0x73, TAG_FUEL_CARD_PROGRAMS=0x74,
					TAG_HOSTS=0x147, TAG_MAIN_GATE=0x149, TAG_BACKUP_GATE1=0x14A, TAG_BACKUP_GATE2=0x14B, TAG_BACKUP_GATE3=0x14C,
					TAG_BACKUP_GATE4=0x14D, TAG_HOST_TYPE=0x138, TAG_XML_TABLE=0x103};

	CTLVParser(void);
	~CTLVParser(void);

	int parseTLV(LPBYTE pTLVData, DWORD dwSize, DWORD* pdwCoreBegin, DWORD* pdwCoreSize, bool bTlvDelayed = false); //возвращает количество тэгов, 0 если ошибка
	int getHostNodesCount();
	int getHostNodesItem(int nItem,int *HostNode,int *HostInterface);
	int getAdminNodesCount();
	int getAdminNodesItem(int nItem);

	
	int getHostBackupIndex();
	int getHostInterface();

	
	
	int getOpenTimeout();
	int getAuthTimeout();
	int getAuthTime();
	
	
	int getTermIndexCount();
	int getTermIndexTable(int nItem, char *termNumber, int *index);


	int getDNTCount();
	int getDNTData(int nItem,int *serverNode, char *IP, int* serverPort, int* keepAlive, int* answerMaxTime);

	int getRASCount();
	int getRASData(int nItem,int *operatorID, char *userName, char *password, char *APN, char *gateway,  int* autoDisconnectTime);


	int getHostsCount();
	int getHostsData(int nItem, int* mainGate, int* backupGate1, int* backupGate2, int* backupGate3, int* buckupGate4);

	int getDeviceGUID(GUID* pstc); //(unsigned long *Data1,unsigned short *Data2,unsigned short *Data3,char *pData4[8]);

	int getPrnCheckNum();

	int getApplicationType();
	
	string showCurrentCfg();
	void updateLocalFiles();
	bool checkConfigUpdated() const;
	bool checkConfigDelayed() const;
	bool updateConfigFromDelayed(BYTE** ppFirmware, DWORD* pdwFirmwareSize);

private:
	bool m_bConfigUpdated;
	std::map<TAG_TYPE, CTLVTag> m_map;
	int getIntData(TAG_TYPE tag);
	TAG_DATA_TYPE getTagDataType(TAG_TYPE tag); //возвращает TAG_DATA_TYPE, тип данных за тэгом, TAG_INVALID если такого тэга нет
	int getTagData(TAG_TYPE tag, CTLVTag** ppTag); // возвращаем указатель** на обьект в мапе.
	int CheckReturn(TAG_TYPE tag);	
};

extern CTLVParser* g_pTlvParser;

#endif 

