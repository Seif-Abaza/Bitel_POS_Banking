/******************************************************************************/
/* COMPANY  : Bitel EMEA Co.                                                  */
/*                                                                            */
/* PROJECT  :                                                                 */
/******************************************************************************/
/******************************************************************************/
/*                                                                            */
/* !Component       :                                                         */
/* !Description     :                                                         */
/*                                                                            */
/* !Module          :                                                         */
/* !Description     :                                                         */
/*                                                                            */
/* !File            :                                                         */
/*                                                                            */
/* !Target          :                                                         */
/*                                                                            */
/* Coding language  : C                                                       */
/*                                                                            */
/* !COPYRIGHT 2013 Bitel EMEA Co.                                             */
/* All rights reserved                                                        */
/*                                                                            */
/******************************************************************************/
/******************************************************************************/
/* EVOLUTIONS                                                                 */
/******************************************************************************/
/* $Log: $
*******************************************************************************/
//#ifndef APP_GLOBAL_H
  #define APP_GLOBAL_H

/*****************************************************************************/
/* INCLUDE FILES                                                             */
/*****************************************************************************/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "../../BF_SDK/include/global.h"
#include "../../BF_SDK/include/sysdefs.h"
#include "iso8583.h"
#include "SndRcvIso8583.h"
//#include "XDATA.h"
#include "../extern.h"
#include "../extern_emea.h"
#include "../internal/cfg.h"


/*****************************************************************************/
/* CONSTANTS, MACROS                                                         */
/*****************************************************************************/
#define SU_CURRENT_PRJ		(MACRO_VALUE_TO_STR(PROJ_NAME)) //"BANQUE MISR"
#define SU_CURRENT_VER		(MACRO_VALUE_TO_STR(PROJ_VERSION))//"1.2.2"
#define SU_CURRENT_BLD		__DATE__

#define HOSTID				106



#define IC3100P						0x3100
#define IC3100PE					0x3101
#define IC3300P						0x3300
#define IC5100P						0x5100
#define IC6000P						0x6000
#define UNKNOWN_MODEL		 	    0xFFFF

#define SDRAM_START_ADDR			0x20000000		// 0x20000000-0x20FFFFFF: 16MB	(IC5100P only)
#define FLASH_ROM_START_ADDR		0x30000000//0x304D0000		// 0x30000000-0x307FFFFF:  8MB/ 16MB
#define SRAM_START_ADDR				0x50000000		// 0x50000000-0x501FFFFF:  2MB




//#define USER_DEBUG
//#define DEV_DEBUG  0
#define GSM_DEBUG  				0


#define SUCCESS 				0
#define REM_CARD_ERR 			0x01
#define CARD_DMGED 				0x02

#define BTCH_EMPTY_ERR 			0x09
#define BTCH_FULL_ERR 			0x03
#define BTCH_UPLD_ERR 			0x04

#define ERR_RESPO_RETRY			0x06
#define ERR_EMPTY_FILE			0x07
#define ERR_FILE_CHNK			0x08

#define CONNECT_ERR             0xFD
#define ERR_EMPTY_MSG			0xFE
#define ERR_GNRL			    0xFF

#define GPRS_CANCELED     		0x0D


#define DBG_PSW   				((uchar*)"1582")
#define DEFAULT_ADMIN_PSW   	((uchar*)"159357")
#define DEFAULT_MRCHNT_PSW   	((uchar*)"161095")
#define DEFAULT_SERL_KEY   		((uchar*)"\xA4\x5D\x9E\xBF\xC8\x83\x69\x58\xBD\xF4\xC0\x5C\xFC\xF9\x31\x43\xFA\xEB\xDC\x59\x89\x65\x18\x73")
#define ADMIN_PSW_MAX_LEN    	8
#define MRCHNT_PSW_MAX_LEN   	8
#define MRCHNT_SERIAL_MAX_LEN   16
#define PIN_MAX_LEN  			4
#define CVC2_MAX_LEN   			3			//heba

#define REVSL_TRIALS 			3
#define MAX_PAGE_SIZE 			150
#define MAX_TRNS_STORE_SIZE 	200
#define MAX_TRNS_LOG_SIZE 		700
#define MAX_DATES_IN_BATCH 		7
#define MAX_DISTRIB_NO   		1/20//20+1
#define BATCH_SIZE_BYTES 		1/((35 + (1+ (4 * MAX_DATES_IN_BATCH)) + (MAX_TRNS_LOG_SIZE *10))*(MAX_DISTRIB_NO+1))//(7740*(MAX_DISTRIB_NO+1))

#define BAKERY_LOG_IDX  		0
#define BATCH_COMP_OFFSET 		(50)
#define BATCH_CHUNK_SIZE 		(999*3)
#define MAX_BATCH_UPL_TRIALS 	(4)
#define MAX_FILE_REQ_TRIALS 	(4)


#define ALPAH_PROMPT  			'A'
#define ALPAH_EN_PROMPT  		'a'
#define HEX_PROMPT       		'H'
#define DIGIT_PROMPT     		'N'
#define PSW_PROMPT    			'P'
#define IP_PROMPT    			'I'

#define TRUE 					1
#define FALSE 					0

#define ON 						1
#define OFF 					0

#define MAX_AMOUNT 99999999

#define PS_VISA				"VISA"
#define PS_MASTERCARD		"MASTER"
#define	PS_DINERS			"DINERS"

//*********************************************Message Type**********************************************************8
// define Message Type ID
//#define MTYPE_AUTHORIZATION_REQUEST			0x0100		// Authorization Request
//#define MTYPE_FINANCIAL_TRXN_REQUEST		0x0200		// Financial Transaction Request
//#define MTYPE_FINANCIAL_TRXN_ADVICE			0x0220		// Financial Transaction Advice
//#define MTYPE_FILE_TRANSFER_REQUEST			0x0300		// File Update/Transfer Request
//#define MTYPE_BATCH_UPLOAD_REQUEST			0x0320		// File Update/Transfer Advice
//#define MTYPE_REVERSAL_REQUEST				0x0400		// Reversal Request
//#define MTYPE_SETTLEMENT_REQUEST		    0x0500		// Card Acceptor Reconciliation Request
//#define MTYPE_NMS_REQUEST					0x0800		// Network Management Request

#define MTYPE_AUTHORIZATION_REQUEST			0x1100		// Authorization Request
#define MTYPE_FINANCIAL_TRXN_REQUEST		0x1200		// Financial Transaction Request
#define MTYPE_FINANCIAL_TRXN_ADVICE			0x1220		// Financial Transaction Advice
#define MTYPE_FILE_TRANSFER_REQUEST			0x1304		// File Update/Transfer Request
#define MTYPE_BATCH_UPLOAD_ADVICE			0x1520		// File Update/Transfer Advice
#define MTYPE_REVERSAL_REQUEST				0x1420		// Reversal Request
#define MTYPE_SETTLEMENT_REQUEST		    0x1804		// Card Acceptor Reconciliation Request

#define MTYPE_NMS_REQUEST					0x0800		// Network Management Request
#define MTYPE_BATCH_UPLOAD_REQUEST			0x0320		// File Update/Transfer Advice
// define Processing Code of Authorization Request
#define PCODE_PRE_AUTHORIZATION				0x300000	// Pre-Authorization
#define PCODE_BALANCE_INQUIRY				0x310000	// Balance Inquiry (normal & bank version)


//***********************************************Processing Code**********************************************************************
// define Processing Code of Financial Transaction Request
#define PCODE_OFFLINE_SALE					0x100000	// Sale/Debit/EBT
#define PCODE_SALE							0x000000	// Sale/Debit/EBT
#define PCODE_VOID_SALE						0x020000	// Void Sale
#define PCODE_REFUND						0x200000	// Refund
#define PCODE_VOID_REFUND					0x220000	// Void Refund

// define Processing Code of Financial Transaction Advice
#define PCODE_SALES_COMPLETION				0x000000	// Sales Completion

// define Processing Code of Card Acceptor Reconciliation Request
#define PCODE_LOGIN							0x940000	// Settlement			MSg:1804
#define PCODE_SETTLEMENT					0x920000	// Settlement			Msg:1520
#define PCODE_SETTLEMENT_TRAILER			0x960000	// Settlement Trailer

// define Processing Code of Purchase
#define PCODE_BILL_PURCHASE		     		0x000000	// Purchase

// define Processing Code of Bill Payment
#define PCODE_BILL_PAYMNT		     		0x500000	// Bill Payment
// define Processing Code of Bill Inquiry
#define PCODE_BILL_INQ		     			0x510000	// Bill Inquiry
// define Processing Code of Card Transfer
#define PCODE_CARD_TRANS					0x610000	// Card Transfer
// define Processing Code of Account Transfer
#define PCODE_ACCNT_TRANS					0x400000	// Account Transfer
// define Processing Code of Money deposit
#define PCODE_MONY_DEPO						0x210000	// Money deposit
// define Processing Code of Money Transfer
#define PCODE_MONY_TRANS				   0x590000	    // Money Transfer
// define Processing Code of Money Receive
#define PCODE_MONY_RCV						0x170000	// Money Receive
// define Processing Code of Balance Inquiry
#define PCODE_BAL_INQ						0x310000	// Balance Inquiry
// define Processing Code of PIN CHANGE
#define PCODE_PIN_CHNG						0x700000	// PIN CHANGE
// define Processing Code of Purchase with cash Back
#define PCODE_PURCH_CSHBK					0x090000	// Purchase with cash Back
// define Processing Code of Mini statement
#define PCODE_MINI_STAT						0x340000	// Mini statement
// define Processing Code of New Key Request
#define PCODE_NEW_KEY					    0x920000	// New Key request
// define Processing Code of Echo Request
#define PCODE_ECHO							0x990000	// Echo

//***********************************************************Entry Mode*************************************************
// define POS Entry Mode
#define PANINPUT_UNSPECIFIED				0			// Unspecified
#define PANINPUT_MANUAL						1			// Manual (Key in)
#define PANINPUT_MSR						2			// Magnetic stripe
#define PANINPUT_ICCR						3			// ICC reader
#define PANINPUT_MANUAL_NOPIN				4			// Manual (Key in) No PIN
#define PANINPUT_MSR_NOPIN					5			// Magnetic stripe No PIN
#define PANINPUT_ICCR_NOPIN					6			// ICC reader
#define PANINPUT_FLBK_MSR_NOPIN				7			// Fallback to Magnetic stripe NO PIN	//Heba
#define PANINPUT_FLBK_MSR					8			// Fallback to Magnetic stripe			//Heba


// define POS Entry Mode
#define PEM_PAN_ENTRY_MASK					0xF0		// PAN entry mask
#define PEM_UNSPECIFIED						0x00		// Unspecified
//Heba

#define PEM_PIN								0x81010

#define PEM_MANUAL_PIN						0x161314C		// Manual (Key in)
#define PEM_MSR_PIN							0x121314C		// Magnetic stripe
#define PEM_ICCR_PIN						0x151314C		// ICC reader
#define PEM_FLBK_MSR_ICCF_PIN				0x181314C		// Fallback to Magnetic stripe,last txn successful,Card faul

#define PEM_NOPIN							0x82010
#define PEM_MANUAL							0x160014C		// Manual (Key in)
#define PEM_MSR								0x120014C		// Magnetic stripe
#define PEM_ICCR							0x150014C		// ICC reader
#define PEM_FLBK_MSR_ICCF					0x180014C		// Fallback to Magnetic stripe,last txn successful,Card fault
//Heba
#define PEM_FLBK_MSR_TF						0x81		// Fallback to Magnetic stripe,last txn unsuccessful,Terminal fault							//Heba
#define PEM_MSR_BY_BAD_ICCR					0x00		// Magnetic stripe even though it is ICC capable

#define PEM_PIN_ENTRY_MASK					0x00F		// PIN entry capability mask
#define PEM_PIN_ENTRY_UNSPECIFIED			0x000		// Unspecified
#define PEM_PIN_ENTRY_CAPABILITY			0x001		// PIN entry capability
#define PEM_NO_PIN_ENTRY_CAPABILITY			0x002		// No PIN entry capability

//********************************************Conditional Code****************************************************************

// define POS Condition Code
#define PCC_NORMAL_PRESENTMENT				0x00		// Normal presentment
#define PCC_CUSTOMER_NOT_PRESENT			0x01		// Customer not present (Not implemented)
#define PCC_MERCHANT_SUSPICIOUS				0x03		// Merchant suspicious
#define PCC_ECR_INTERFACE					0x04		// ECR interface
#define PCC_CARD_NOT_PRESENT				0x05		// Card not present
#define PCC_PRE_AUTHRIZED_REQUEST			0x06		// Pre-authorized request
#define PCC_MAIL_TELEPHONE_ORDER			0x08		// Mail and/or telephone order
#define PCC_OPEN_TAB						0x51		// Open tab
#define PCC_FREQUENCY_TRXN					0x52		// Frequency transaction
#define PCC_CARD_PRESENT_MSR_BAD			0x71		// Card present magnetic stripe bad
#define PCC_STORE_AND_FORWARD				0x87		// Store and forward


//*******************************************************************************************************************
//*********************************************Respons Code**********************************************************************
//*******************************************************************************************************************
#define VOIDED_SUCCESS 	480
#define ALREADY_VOIDED	482

#define RECON_SUCCESS   500
#define RECON_NOTMATCH	501
#define BATCH_DONE		581

//typedef struct
//{
//	int		code;
//	char	message[70];
//} MSG_RESP;
//
//MSG_RESP const Msg_Resp[] =
//{
//	{   000, "Approved" },
//	{   308, "Duplicate" },
//	{   480, "Voided" },
//	{   481, "Amount is Not Correct" },
//	{   482, "Already Voided" },
//	{   500, "Reconciliation succeeded" },
//	{   501, "Reconciliation Not Matched" },
//	{   581, "Reconciliation already done" },
//	{   909, "System defect" },
//	{  -1, "" },
//};

//*******************************************************************************************************************
//*******************************************************************************************************************
//*******************************************************************************************************************
//*******************************************************************************************************************
#define MTN_BILL_PYMNT_VEND_CODE              43
#define MTN_BILL_PYMNT_INST_CODE              11 
                                    
#define MTN_TOPUP_VEND_CODE                   32
#define MTN_TOPUP_INST_CODE                   01

#define ZAIN_TOPUP_VEND_CODE                  33
#define ZAIN_TOPUP_INST_CODE                  01

#define ZAIN_BILL_PYMNT_VEND_CODE             31
#define ZAIN_BILL_PYMNT_INST_CODE             22

#define SUDANI_TOPUP_VEND_CODE                38
#define SUDANI_TOPUP_INST_CODE                08

#define SUDANI_BILL_PYMNT_VEND_CODE           36
#define SUDANI_BILL_PYMNT_INST_CODE           08

#define CANAR_VEND_CODE                       42
#define CANAR_INST_CODE                       22

#define NEC_VEND_CODE                         35
#define NEC_INST_CODE                         01

#define TextOut(a, b, c)				TextOutA(a, b, c, 0, -1, 0)

#ifndef min
#define min(X, Y) ((X) < (Y) ? (X) : (Y))
#endif

#ifndef max
#define max(X, Y) ((X) > (Y) ? (X) : (Y))
#endif

#ifdef MY_DEBUG
#define DBGPrt_m(...)		{ Rprintf(__VA_ARGS__); }
#else
#define DBGPrt_m(...)
#endif
/*****************************************************************************/
/* TYPES                                                                     */
/*****************************************************************************/

typedef struct
{
  uchar aucDateMMDD[5];
  uchar aucOpCode[4];
  uchar aucAmount[13];
  bool bIsNegative;
}strStatRecord;

typedef struct
{
  uchar ucNoRecords;
  strStatRecord rudtStatmentRcrds[100];
}strMiniStatment;

typedef struct
{
  uchar aucContractNumber[30];
  uchar aucBillamount[10];
  uchar aucUNBillAmount[10];
  uchar aucTotalBill[10];
  uchar aucLastInvoiceDate[20];
}strBillInqInfo;

typedef struct
{
  uchar aucSalesAmount[33];
  uchar aucFixesFee[33];
  uchar aucToken[33];
  uchar aucMeterNumber[33];
  uchar aucUnit[33];
  uchar aucAccountNumber[34];
  uchar aucReceiptNumber[33];
  uchar aucCustomerName[34];
  uchar aucReferenceNumber[33];
  uchar aucOperator[33];
}strNECInfo;



typedef enum
{
  PURCH = 0,
  VOID,
  REFUND,
  BATCH,
  ECHO_TEST,
  NEW_TPK,

    BILL_PAY,
    TOPUP_PAY,
    ELECT,
    CARD_TRANSFER,
    ACC_TRANSFER,
    MONEY_DEPOS,
    MONEY_TRANSFER,
    MONEY_RCV,
    BAL_INQUIRY,
    PIN_CHANGE,
  MINI_STATE,
    PURCH_CASH_BACK,

//  REVERSL,
  //ZAIN_BILL,
  //MTN_BILL,
  //SUDANI_BILL,
  //CANAL_BILL,
  //
  //ZAIN_TOPUP,
  //MTN_TOPUP,
  //SUDANI_TOPUP,
  //CANAL_TOPUP,
}enuTransType;

typedef enum
{
  ZAIN = 0,
  MTN,
  SUDANI,
  CANAL
}enuVendType;

typedef enum
{
  CUST_COPY = 0,
  MERCH_COPY
}enuRctCopyType;

typedef struct
{
  uchar aucVendCode[3];
  uchar aucInstitCode[3];
}strVedInstitCode[2];

typedef struct
{
  uchar aucSIM1APN[100];
  uchar aucSIM1UsrName[40];
  uchar aucSIM1Password[40];

  uchar aucSIM2APN[100];
  uchar aucSIM2UsrName[40];
  uchar aucSIM2Password[40];

  uchar aucServerIP[18];
  uchar aucServerPort[8];
  uchar aucServerURL[200];

  ulong ulHostTimeout;
}strConnSettings;

typedef struct
{
  uchar aucTermId[10];
  uchar aucTermName[MAX_NUM_LANG][33];
  uchar aucMerchantId[16];
  uchar aaucMerchantName[MAX_NUM_LANG][33];
  uchar aaucMerchantAddr[MAX_NUM_LANG][33];
  uchar aucMerchantPassword[8];
  uchar aucAdminPassword[8];
  uchar aucBankName[MAX_NUM_LANG][33];
  uchar aucMCC[5];								//MCC
  ulong ulFloorLimit;								//Floor Limit
  strVedInstitCode audtVedInstitCode[5];
  uchar aucTMK[17];
  uchar aucTPK[17];
  bool bIsSwipPinCap;
  bool bIsManEnt;
  bool bIsManPinCap;
  bool bIsEnableDbg;
  APP_LANG udtAppLang;
  uchar aucProdId[30];
  uchar aucDistInCode[20];
  ulong ulDistQuota;
}strDeviceConfig;

typedef struct
{
  bool bIsValidData;
//  char CardHolderName[30];		//< Card Holder name in track 1
 // ushort usPOSEntryMode;			//<P-22 - Entry Mode
 // char PAN[20];					//<P-2 Card Number
//  char ExpiryDate[5];				//<P-14- Expiry date in format YYMM
//  char SrvcCode[4];				//<P-26 Service code
  char acTrack1[112];				//< Track1 data
  char acTrack2[52];				//< Track 2 Data
  char CiphertextPinBlock[9];		//< Encrypted PIN Block
  char acPANSeq[4];
}strMSRCardData;

typedef struct
{

  enuTransType udtTansType;			//Message Type
  enuVendType udtVendType;			//<List of supported vendors . Third Parity , Mobile Operator
  strMSRCardData udtMSRCard;
  ulong MTI;						//Message Type Identifier
  char PAN[20];						//<P-2 Card Number
  ulong ulPCode;					//<P-3 Processing code
  uchar aucAmount[13];				//<P-4 Trans Amount
  ulong ulSysTraceAuditNo;			//<P-11 STAN
  uchar aucTransTime[7];			//<P-12 Transaction Time (HHMMSS)
  uchar aucTransDate[9];			//<P-13 Transaction Date (YYYYMMDD)
  char ExpiryDate[5];				//<P-14- Expiry date in format YYMM
  ushort usPOSEntryMode;			//<P-22 - Entry Mode
  uchar aucPOSEntryMode[12];			//<P-22 - Entry Mode
  uchar ucPosCondCode;				//<P-25 Conditional code
  char SrvCode[4];					//<P-26 Service code
  uchar aucBilledAmount[13];
  uchar aucCashBackAmount[13];		//<Cash Back Amount
  uchar aucFee[14];
  uchar aucBalance[17];
  uchar aucAccountNo[30];			//<Phone number in bill payment TXN
  ulong ulReceiptNo;
  uchar aucRtrvalRefNo[13];			//<P-37 Paid Reference No.
  uchar aucAuthIdResponse[7]; 		//<P-38:- AUTH NO
  ulong ulActionCode; 				//<P-39:- Action
  uchar aucCurrencyCode[3];			//<P-48 :Currency Code
  uchar aucExtRtrvalRefNo[13]; 		//<P-57.37 Transaction ID
  char CardHolderName[30];			//< Card Holder name in track 1
  uchar aucCVC2			[4];		//heba	P-48	 CVC2 						  //Manual Entry
  uchar aucAddtionalData[28];				//heba			Authorization number		  //Manual Entry
  bool bIsFallback;					//Added By Heba
  bool bIsVoided;
  bool bIsOffline;					//True is Offline. False is Online

  #define ICCbL3_MAX  145
  U08 SelectedAID[17];    //Length 5-16
  U08 SelectedAID_Name[40];    //Length 5-16
  U08 ICCbL3[2];
  U08 TVR[5];
  U08 TSI[2];
  U08 CVMR[3];
  U08 EMVTags[270];

}strTransDetail;


typedef struct
{
  ulong ulSavedTransNo;
  strTransDetail audtTrans[MAX_TRNS_STORE_SIZE];
}strTransStore;

typedef struct
{
	uchar   ucErrorExtCode;
    bool bIsCurrReversal;
    bool bIsBatchUpload;
    bool bIsStopSgnlStrUpd;
    ushort  usHdrIdx;
    int iSgnlStrength;
    enuTransType udtTansType;
    strTransDetail udtTransData;
    uchar ucMsrTrack;
    int iModelName;
    char acProdId[30];
}RUNDATA;

typedef struct
{
  uchar bIsAlarm;
  uchar ucAlarmsNo;
  bool abIsAlarmChecked[20];
  uchar aaucAlarmsTime[20][5];
  uchar aucAlarmDate[9];
  uchar aaucDispAlarmsTime[20][20];
}strBatchAlarm;

typedef struct
{
  bool bIsReversalReq;
  uchar aucTermId[10];
  ulong ulPCode;
  ulong ulSysTraceAuditNo; 
  ushort usPOSEntryMode;			//<P-22 - Entry Mode //heba
  char ExpiryDate[5];				//<P-14- Expiry date in format YYMM
  char PAN[20];						//<P-2 Card Number
  strMSRCardData udtMSRCard;

}strRevesalData;

typedef struct
{
  ulong ulSysTraceAuditNo; 
  ulong ulReceiptNo;
  ulong ulBatchNo;
  bool bIsReversalReq;
  bool bIsBatchPending;
  bool bIsIsLoggedOn;

  strBatchAlarm udtBatchAlarms;
  strDeviceConfig udtDeviceConfig;
  strConnSettings udtConnSettings;
  strTransDetail udtTransDetail;
  strTransStore udtTransStore;
  strRevesalData udtRevesalData;

  uchar 	APP_Project[20];
  uchar 	APP_Version[10];
}DATASAVEPAGE1;

typedef struct
{
  BACKUPISO8583	SaveISO8583Data;
  BACKUPISO8583	SendISO8583Data;

}DATASAVEPAGE0;

/***************************************BREAD**********************************************/
//typedef struct
//{
//	uchar aucVer[3];
//	uchar aucEffectDate[4];
//	uchar aucExpiryDate[4];
//}strWLHDR;

//typedef struct
//{
//	uchar aucInsideCode[8];
//	uchar aucAmount[3];
//} strWLEntry;

//typedef struct
//{
//	strWLHDR udtWLHDR;
//	uchar ucDistribNo;
//	strWLEntry strDisribWLInfo[MAX_DISTRIB_NO];
//}strDisribWL;

typedef struct
{
	uchar aucItemPrice[3];
	uchar DispWndowDays;
	uchar ucAdvancDisp;
	bool bIsParialAllowed;
}strDispGuideInfo;

typedef struct
{
	uchar aucItemCode[3];
	strDispGuideInfo udtDispGuideInfo;
	uchar aucEffectDate[3];
	uchar aucExpiryDate[3];
	uchar ucRulesType;
//	enum {
//		RANGE_VALUE = 0,
//		MULTPLY_VALUE,
//	}enRulesType;
}strDRItemHdr;

typedef struct
{
	uchar ucPrson_No;
	uchar aucQuota[2];
}strDispRule;

typedef struct
{
	uchar aucVer[3];
	uchar ucItemsNo;
	struct
	{
		strDRItemHdr udtDRItemHdr;
		uchar ucRulesNo;
		strDispRule astrDispRule[20];
	} astrItemDispTbl[5];
}strDispTbl;

typedef enum
{
  CTZN_CARD = 1,
  ORG_CARD,
  INSP_CARD,
  DIST_CARD,
  BKRY_CARD,
  MAX_NO_CARDS
}CardType;

typedef enum
{
	BLACK_LIST = 0,
	WHITE_LIST,
	DISTRIB_RULES,
	POS_PARAM,
	MAX_FILES_NO
}enuFileTypes;

typedef struct
{
	uchar aucSWVer[6];
	uchar aucBLVer[3];
	uchar aucWLVer[3];
	uchar aucDRVer[3];
	uchar aucPOSParmVer[3];
}strFilesVer;

typedef struct
{
	int BLUpdtReq : 1;
	int WLUpdtReq : 1;
	int DRUpdtReq : 1;
	int POSPramUpdtReq : 1;
}strFileUpdtReq;

typedef struct
{
	CardType udtCardType;
	uchar aucInsideCode[8 + 1];
	uchar aucOutsideCode[13];
	uchar ucFmlyMembers;
	ulong ulQuota;
	ulong ulMonthlyQuota;
	ulong ulMonthlyDispensed;
	uchar aucLastTransDate[4];
	uchar aucCreationDate[3];
	uchar aucTransDate[4];
	uchar aucTransTime[4];
	ulong ulPrevDisp;
	ulong ulRemainQuota;
	uchar aucGovernarate[2];
}strInsCardData;

typedef struct
{
	ulong ulAmount;
	ulong ulPatialAmountOfDay;
	uchar ucNoOfDispWndw;
	uchar aucTransDueDate[5];
//	ulong ulMonthlyQuota;
//	ulong ulMonthlyDispensed;
}strTransData;

typedef struct
{
	uchar ucBatchNoDates;
	struct
	{
		uchar aucDate[2];
		uchar aucStartTransIdx[2];
	} audtDateTransIdx [MAX_DATES_IN_BATCH];
}strDatesInBatch;

typedef struct
{
	uchar aucInsideCode[8];
	uchar aucAmount[2];
}strTransLogData;

typedef struct
{
  uchar aucMerchantId[8];
  uchar aucBatchNo[3];
  uchar aucSavedTransNo[2];
  uchar aucTotalAmount[4];
  uchar aucInspSavedTransNo[2];
  uchar aucInspTotalAmount[4];
  uchar aucBatchOpenDate[3];
  uchar aucBatchOpenTime[3];
  uchar aucBatchCloseDate[3];
  uchar aucBatchCloseTime[3];
}strLogHdr;

typedef struct
{
  uchar aucMerchantId[8];
  uchar aucMainBtchCounter[3];
  uchar aucDistSettlNo[2];
  uchar aucSavedTransNo[4];
  uchar aucTotalAmount[4];
  uchar aucInspSavedTransNo[4];
  uchar aucInspTotalAmount[4];
  uchar aucMainBatchOpenDate[3];
  uchar aucMainBatchOpenTime[3];
  uchar aucMainBatchCloseDate[3];
  uchar aucMainBatchCloseTime[3];
}MainBtchHdr;

typedef struct
{
  strLogHdr LogHdr;
  strDatesInBatch udtDatesInBatch;
  strTransLogData audtTrans[MAX_TRNS_LOG_SIZE];
}strTransLog;

typedef struct
{
	 ulong ulNoOfTrans;
	 ulong ulBreadNo;
}strBatchTotal;

typedef struct
{
	uchar aucBin[2];
	ulong ulBinIdx;
}BinEntry;

typedef struct
{
	bool bIsPortConfig;
	bool bIsSimSelected;
	bool bIsGPRSConnected;

}strGPRSSetting;

typedef struct
{
	uchar aucDownFileVer[3];
	uchar aucDRMirror[500];
	uchar aucWLMirror[500];

	strDispGuideInfo DispGuideInfo;

	strGPRSSetting udtGPRSSetting;
    strTransData udtDispTransData;
    strInsCardData udtInsCardData;
    strBatchTotal  udtBatchTotals;
}strRunData;

typedef struct
{

 ulong ulDistSettlNo;
 struct
 {
   uchar aucMerchantId[8];
   ulong aulDistBatchIdx;
   uchar aucSavedTransNo[2];
   uchar aucTotalAmount[4];
   uchar ucSettelStatus;
 }DistBatchInfo[MAX_DISTRIB_NO];

 bool bIsSettlOccur;

 bool bIsBatchGen;
 ulong ulCompBatchSize;
 ulong ulBatchStartWrIdx;
 uchar aucBatch[BATCH_SIZE_BYTES];

 strTransLog udtTransLog;
}DATASAVEPAGE2;

typedef struct
{

	uchar ucNoDisribInBL;
	struct
	{
		uchar aucDistrID[16];
		uchar aucDistrBLLoc[4];
	}DistribBLLoc[99];
}strBLHDR;

typedef struct
{
  ulong ulInspDayRemQuota;
  uchar aucInspDispDay[3];
}strInspSaleInfo;

typedef struct
{
	ulong ulSysTraceAuditNo;
	ulong ulBatchCounter;
	ulong ulMainBtchCounter;

	strInspSaleInfo udtInspSaleInfo;
	uchar aucCompBLVer[3];
	strFilesVer udtFilesVer;
	strFileUpdtReq udtFileUpdtReq;
	strDispTbl udtDispTbl;
	//strDisribWL udtDisribWL;	//Heba
	strBLHDR udtBLHDR;
	struct
	{
		ushort usNoOfBins;
		BinEntry udtBinEntry[10000];
	}strBinTbl;
	//strTransLog udtTransLog;
}DATASAVEPAGE3;


extern char CHAR_FILL1[5], CHAR_BUFFER1[5][36][32], ATTB_BUFFER1[5][36][32];
extern unsigned char PRINTING_NEED;
extern void TRACK_SETUP(int);

extern const ulong aulPCode[];
extern RUNDATA udtRunData;
extern strRunData udtBrdRunData;
extern strMiniStatment udtMiniStatment;
extern strBillInqInfo udtBillInqInfo;
extern strNECInfo udtNECInfo ;
extern int iSgnlStrength;


//#define  DATASAVEPAGE1BaseAddress 	0x50150000
extern volatile  DATASAVEPAGE0 * const DataSave0;
extern volatile  DATASAVEPAGE1 *  const DataSave1;
extern volatile  DATASAVEPAGE2 *  const DataSave2;
extern volatile  DATASAVEPAGE3 *  const DataSave3;
/*****************************************************************************/
/* FUNCTIONS                                                                 */
/*****************************************************************************/
unsigned long strtoul_m( const char *nptr, char **endptr, register int base);
long strtol_m( const char *nptr, char **endptr, register int base);
extern int ltoa(long N, char *str, int base);
extern unsigned long ulstrtoul (const char *, char **, int);
extern unsigned char long_hex(char *ucDest,unsigned char len,unsigned long ulVal);
extern unsigned long hex_long(char *ucIp,unsigned char len);
extern int DDES( unsigned char *source, unsigned char *dest, unsigned char *inkey, int len, int flg );
extern void DES_CRC( unsigned char buf_in[], int buf_len, char buf_out[] );
/*-----------------------------------------------------------------------
	19. Miscellaneous function
-----------------------------------------------------------------------*/
extern int RANDOM_NUMBER[4];

extern void GET_TRUE_RANDOM_NUMBER(void);

// Get Production ID (serial number)
// 	Output
//	- meminput: pointer of production ID (16 bytes)
//	Return value
//	- 0: OK, -1: Error (Production ID is not found)
extern int GET_PROD_ID(char *meminput);

// Save Production ID (serial number)
// 	Input
//	- meminput: pointer of production ID (16 bytes)
//	Return value
//	- 0: OK, -1: Error (Production ID is already saved)
extern int SAVE_PROD_ID(char *meminput);
/*-----------------------------------------------------------------------
	4. Multitasking
-----------------------------------------------------------------------*/
extern char TASK_SEQ[32];		// for SET_TASK_PRIORITY_MODE function

//#endif /*APP_GLOBAL_H*/


