/*
 * EMV.h
 *
 *  Created on: Sep 9, 2014
 *      Author: Mahmoud Swedan
 */

#ifndef EMV_H_
#define EMV_H_

#ifndef S08
	#define S08 char
#endif

typedef struct {
    U08 ADR;
    U08 CB;
    U08 tpduID;
    U08 tpduAdrDest[2];
    U08 tpduAdrOrig[2];
}TMsgHeader;

typedef struct {
//    S16 code;
    U16 code;
    S08 Str[16];
}RESP_CODES;

typedef struct {
    U32 code;
    S08 Str[16];
}PROC_CODES;
/*
typedef struct {
    U32 code;
    S08 Str[16];
    U08 bitMap[16];
    U08 RFU36;
    U08 bitMap[16];
}PROC_CODES;
*/
typedef struct {
    S08 bit;
    S08 *msg;
}VALIDATE_BITS;

typedef struct {
    U32 sBIN,eBIN;
    S08 Str[16];
}BIN_TABLE;

typedef struct {
    U16 count;
    U32 total;
}REC_TOTAL;

typedef struct {
    U16 BatchNr;
    U16 TraceNr;
    U16 MsgType;
    U16 POSEntryMode;
    U08 zzFlag;
    S08 OnLineFlag;
    U08 Signature;
    U08 POSConditionCode;
    U16 ResponseCode;
    U08 SystemTraceNr[3];
    U08 ProcessingCode[3];
    U08 ApprovalCode[6];
    U08 RetrievalRefNr[12];
    U08 bcdYMDHMS[6]; //YYMMDDHHMMSS, shoule be in order
    U08 AmountTrxn[6];
    U08 Track2Data[20];
    U08 ProcCodeStrNr;
    U08 RespCodeStrNr;
    U08 SelectedAID[17];    //Length 5-16
    U08 CardHolderName[30]; //Length 2-26 //17 to 30 by youssef
    U08 SvcCode[3];		// sispp070213 2CO.038.00
#define ICCbL3_MAX  145
    U08 ICCbL3[2];
    U08 TVR[5];
    U08 TSI[2];
    U08 CVMR[3];
    U08 Reserved[300];
    U08 Trns_ICCbL3[2];
U08 Trns_Reserved[300];
}TTrxnRec;

typedef struct {
    U16 BatchNr;
    U16 TraceNr;
    char CardAcceptorTerminalID[ 8];
    char CardAcceptorMerchantID[15];
    char TrmAcquirerID[6];
    TMsgHeader MsgHeader;
//    TMsgTypeID MsgTypeID;
    U08 aL3_B60orgBatchNr[24];
    U08 aL3_B62orgTraceNr[16];
    U08 aL3_B63reconTotal[160];
    TTrxnRec TrxnRec;  // This is 256 bytes Records
    U08 Reserved[256]; // For IssuerScriptData Dummy 256 bytes
//    MESSAGE_QUE *pMsgQue;
    #define REC_SALE 0
    #define REC_RFUN 1
    #define REC_CASH 2
    REC_TOTAL sum[3];
}TgsPOS;

/*---------------------------------------------------------------------------*/
/* Define for FlashFile & POS SYSTEM FILE */
/*---------------------------------------------------------------------------*/
typedef struct t_SYSINFOT {
	short  eAPP_ver;
    char beep;
    char font;
    char contrast;
    char modemspeed;
    char syncmode;
    char standard;
    char dialmode;
    char pabxused;
    char pabxcode[5];
    char pulsedial;
    char telephoneno[18];
    char telephoneno2[18];
    char NetworkInternationalID[ 2];
    char CardAcceptorTerminalID[ 8];
    char CardAcceptorMerchantID[15];
    char RFU78;
    char POScoNameVerNr[13];
    char RFU92;
    char POSSerialNr[8];
    char InstallmentNr[2];
    char HdlcTpdu[5];
    char RFU108;
    char TrmLogFile[14];
    char TrmPIN[8];
    char TrmL2TTAmode;
    char TrmForceOnLine;
    char TrmType;
    char TrmTCC;
    char TrmCapa[3];
    char NETDEV;
    char TrmCapaAdditional[5];
    char RFU144;
    char TrmAcquirerID[6];
    char TrmMCC[2];
    char TrmIFD_ID[16];
    char dummy[88];
    char msg_info[8][32];
    char deakey[10][16];
    char ServerIPAddress[16];
    char ServerPortNo[6];
    char LocalIPAddress[16];
    char GatewayIPAddress[16];
    char SubnetMask[16];
    char LocalPortNo[6];
    char LocalTimeout[6];
    char RFU754;
    char loginUsed;
    char loginUserName[16];
    char loginPassWord[16];
    char RFU788;
    char autoIPused;
    char autoDHCP;
    char autoPPPoE;
    char autoARP;
    char RFU793;
    char needTCPIPstack;
    char RFU795;
    char User_PIN[3][8];
    char RFUmKB[1023-819];
} SYSINFOT;

typedef struct{
  char CardAcceptorTerminalID[ 8];
  char CardAcceptorMerchantID[15];
  char TrmAcquirerID[6];
  char POSSerialNr[8];
  char TrmL2TTAmode;
  char TrmForceOnLine;
  char TrmType;
  char TrmTCC;
  char TrmCapa[3];
  char TrmCapaAdditional[5];
  char TrmMCC[2];
}TRM_PARAM;
extern void emv_start(unsigned short headerIdx);


extern int OnLineProcessSub(int iReferral);
extern void InitEmvLib();
extern int app_InitReferralCall(void);
extern void CompletionSub();
extern void EMVDISP(int no);

extern const U08 gcTrmRevocList[], gcTrmExcepList[];
extern void InitPinpadDrv(int iLine);
extern void init_message(void);
extern void SET_BUFFER_SIZE_8K();
extern int GemSlotOpen;
extern U08 gem_RS232open;
extern U32 gem_BaudRate;

extern U16 ghResponseCode;
extern U16 gwResponseCode;
extern U16 gwTrmARC;


extern TgsPOS gsPOS;


#endif /* EMV_H_ */
