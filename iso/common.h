
#ifndef __ISO_COMMON_H
#define __ISO_COMMON_H

#define ISO_LEN_MSG_TYPE			2							// длина типа сообщени€
#define ISO_LEN_POWER_CARD_HEADER	13							// длина PowerCard header'a
#define SV3_PROTOCOL_ID				"P11"
#define	ISO_PROTOCOL_ID				"ISO"						// protocol identification
#define ISO_PRODUCT_CODE			"2"							// '2' - POS
																// верси€
#define	ISO_POS_VERSION				"0.0.99"
#define ISO_NO_ERR					"000"						// ошибок нет



#define		ISO_C05_DELEMITER	"~"

#define	MTID_LOGIN			"0660"
#define MTID_PENSION		"0682"



//==================================================================================================
// Ќазвание полей в строковом виде и их длины
//==============================================================================
#define FIELD_CARD_NUM				"2"
#define FIELD_PROCESS_CODE			"3"
//#define FIELD_GOV_CODE			    "3"
#define FIELD_TRN_AMOUNT			"4"
#define FIELD_TRMSN_DATE_TIME		"7"
#define FIELD_CARD_CODE				"10"
#define FIELD_STAN					"11"
#define FIELD_TRN_LOCAL_TIME		"12"
#define FIELD_TRN_LOCAL_DATE		"13"
#define FIELD_GOV_CODE			    "14"
#define FIELD_SETTLEMENT_DATE		"15"
#define FIELD_CAPTURE_DATE			"17"
#define FIELD_ENTRY_MODE			"22"
#define FIELD_FUNCTION_CODE			"24"
#define FIELD_COND_MODE			    "25"
#define FIELD_MRCH_BUSINESS_CODE	"26"
#define FIELD_ACQRN_INSTTN_IC		"32"
#define FIELD_PRIMARY_ACC_NUM_EXT	"34"
#define FIELD_TRACK_2				"35"
#define FIELD_RRN					"37"
#define FIELD_APPROVAL_CODE			"38"
#define FIELD_ACTION_CODE			"39"
#define FIELD_TERM_NUM				"41"
#define FIELD_OUTLET_NUM			"42"
#define FIELD_CARD_NAME_ADDRESS		"43"
#define FIELD_FILE_UPDAT_FLAGS		"44"
#define FIELD_FILES_VERSIONS		"47"
#define FIELD_PROVIDE_ADD_DATA		"48"
#define FIELD_TRN_CURRENCY_CODE		"49"
#define FIELD_STLM_CURRENCY_CODE	"50"
#define FIELD_PIN_DATA				"52"
#define FIELD_FILE_NAME				"53"
#define FIELD_ADD_AMOUNTS			"54"
#define FIELD_MSG_NO    			"54"
#define FIELD_ICC_DATACAPTURE1		"55"
#define FIELD_DATA_CHNK1 			"55"
#define FIELD_DATA_CHNK2 			"56"
#define FIELD_DATA_CHNK3 			"57"
#define FIELD_DATA_CHNK4 			"58"
#define FIELD_DATA_CHNK5 			"59"
#define FIELD_BTCH_NO				"60"
#define FIELD_REDEMPTION_NUM		"62"
#define FIELD_BTCH_TOTLS			"63"
#define FIELD_MSG_AUTH_CODE			"64"
#define FIELD_MSG_NUM				"71"
#define FIELD_DATA_RECORD			"72"
#define FIELD_ACTION_DATE			"73"
#define FIELD_CREDITS_NUM			"74"
#define FIELD_CREDITS_REV_NUM		"75"
#define FIELD_DEBIT_NUM				"76"
#define FIELD_DEBIT_REV_NUM			"77"
#define FIELD_PAYMENTS_NUM			"83"
#define FIELD_PAYMENTS_REV_NUM		"84"
#define FIELD_CREDITS_AMOUNT		"86"
#define FIELD_CREDITS_REV_AMOUNTS	"87"
#define FIELD_DEBITS_AMOUNT			"88"
#define FIELD_DEBITS_REV_AMOUNTS	"89"
#define FIELD_NET_RECONCIL_AMOUNT	"97"

#define FIELD_FILE_NAME_DUMM				"101"
#define FIELD_ICC_DATACAPTURE		"125"
#define FIELD_ADD_MSG_AUTH_CODE		"128"

#define TAG_DATA_RECORD_RETAILER				"C05"

//==================================================================================================
// ¬озможные теги пол€ "72/C05"
//==============================================================================
#define TAG_DATA_RECORD_RETAILER_ACCOUNTS		"00"
#define TAG_DATA_RECORD_RETAILER_INFORMATION	"01"
#define TAG_DATA_RECORD_RETAILER_BTC			"02"

// Additional
#define AFIELD_OUT_BTC					"72/A01"
#define AFIELD_BILLING_REF				"72/A02"
#define AFIELD_OUT_BILLING_ACC_NUM		"72/A03"
#define AFIELD_AMMOUNT					"72/A06"
#define AFIELD_SESSION_DURATION			"72/C01"
#define AFIELD_MAX_TRN_CNT				"72/C02"
#define AFIELD_MAX_TRN_AMOUNT			"72/C03"
#define AFIELD_NEED_PIN					"72/C04"
#define AFIELD_ACC_TYPE					"72/C05/00/D04"
#define AFIELD_IN_BTC					"72/C05/02/D09"
#define	AFIELD_IN_BILLING_ACC_NUM		"72/C05/02/D10"
#define	AFIELD_IN_BILL_ACC_REF_REQ		"72/C05/02/D12"
#define AFIELD_FEE						"72/C05/02/D17"
#define AFIELD_SESSION_ID				"72/D01"
#define AFIELD_KEY						"72/TMK"
#define AFIELD_MACKEY					"72/MACKEY"
#define AFIELD_TPK						"48/P10"

//==================================================================================================
// Bread Msgs Types
//==============================================================================
#define MTID_CARD_UPDT_REQUEST		    "\x01\x00"
#define MTID_FILE_TRANSFER_REQUEST		"\x03\x00"
#define MTID_SETTLEMENT_REQUEST		    "\x05\x00"
#define MTID_SALE_REQUEST		    	"\x02\x00"
#define MTID_BATCH_UPLOAD_REQUEST			0x0320		// File Update/Transfer Advice




//==================================================================================================
// —лучаи, когда 72 поле состоит из тегов / без тегов
//==============================================================================
#define MTID_REQUEST_72_WITHOUT_TAGS		"1304"
#define MTID_RESPONSE_72_WITHOUT_TAGS		"1314"
#define FILE_NAME_SESSION_IN				"SESSIONIN"
#define FILE_NAME_SESSION_OUT				"SESSIONOUT"
#define FILE_NAME_KEY						"KEY"
#define FILE_NAME_MAC_KEY					"MAC KEY"

//==================================================================================================
// —генерованные теги дл€ KEY и смещени€ в 72 поле
//==============================================================================
#define TAG_KEY_TMK				"TMK"
#define TAG_KEY_CHECK			"CHECK"
#define TAG_KEY_MKIDX			"MKIDX"
#define TAG_KEY_MKMODE			"MKMODE"
#define TAG_KEY_TKM_LENGTH		48
#define TAG_KEY_CHECK_LENGTH	16
#define TAG_KEY_MKIDX_LENGTH	3
#define TAG_KEY_MKMODE_LENGTH	1
#define TAG_KEY_TOTAL_LENGTH	(TAG_KEY_TKM_LENGTH + TAG_KEY_CHECK_LENGTH + TAG_KEY_MKIDX_LENGTH + TAG_KEY_MKMODE_LENGTH)

//==================================================================================================
// —генерованные теги дл€ MAC KEY и смещени€ в 72 поле
//==============================================================================
#define TAG_MACKEY_MACKEY			"MACKEY"
#define TAG_MACKEY_CHECK			"MACCHECK"
#define TAG_MACKEY_IDX				"MACIDX"
#define TAG_MACKEY_MACKEY_LENGTH	16
#define TAG_MACKEY_CHECK_LENGTH		16
#define TAG_MACKEY_IDX_LENGTH		3
#define TAG_MACKEY_TOTAL_LENGTH		(TAG_MACKEY_MACKEY_LENGTH + TAG_MACKEY_CHECK_LENGTH + TAG_MACKEY_IDX_LENGTH)

#define LENGTH_MAX_CARDNUM_LENGTH	19
#define LENGTH_PROCESS_CODE		    6
#define LENGTH_GOV_CODE		    	2
#define LENGTH_TRN_AMOUNT			12
#define LENGTH_CARD_CODE			12
#define LENGTH_TRMSN_DATE_TIME		10
#define LENGTH_STAN					6
#define LENGTH_TRN_LOCAL_DATE		4
#define LENGTH_TRN_LOCAL_TIME		3
#define LENGTH_CARD_EXPIRE			4
#define LENGTH_SETTLEMENT_DATE		6
#define LENGTH_CAPTURE_DATE			4
#define LENGTH_ENTRY_MODE			3
#define LENGTH_FUNCTION_CODE		3
#define LENGTH_COND_MODE			2
#define LENGTH_MRCH_BUSINESS_CODE	4
#define LENGTH_RRN					12
#define LENGTH_APPROVAL_CODE		6
#define LENGTH_ACTION_CODE			2
#define LENGTH_TERM_NUM				8
#define LENGTH_OUTLET_NUM			15
#define LENGTH_TRN_CURRENCY_CODE	3
#define  LENGTH_TRN_LOCAL_DATE_TIME 12
#define LENGTH_STLM_CURRENCY_CODE	3
#define LENGTH_PIN_DATA				8
#define LENGTH_MSG_AUTH_CODE		8
#define LENGTH_MSG_NUM				8
#define LENGTH_ACTION_DATE			6
#define LENGTH_CREDITS_NUM			10
#define LENGTH_CREDITS_REV_NUM		10
#define LENGTH_DEBIT_NUM			10
#define LENGTH_DEBIT_REV_NUM		10
#define LENGTH_PAYMENTS_NUM			10
#define LENGTH_PAYMENTS_REV_NUM		10
#define LENGTH_CREDITS_AMOUNT		16
#define LENGTH_CREDITS_REV_AMOUNTS	16
#define LENGTH_DEBITS_AMOUNT		16
#define LENGTH_DEBITS_REV_AMOUNTS	16
#define LENGTH_NET_RECONCIL_AMOUNT	17
#define LENGTH_AUTH_CODE			8
#define LENGTH_TERMINAL_NUMBER      8

#define ALENGTH_SESSION_ID			36
#define ALENGTH_FEE					39

enum TPROTOCOL
{
	ISO				= 0,
	COMPASS_PLUS	= 1,
	OPENWAY			= 2,
	FDATA			= 3,
	NBE			= 4,

};


typedef enum {
        e_verISO_85831987,
        e_verISO_85831993
}TMsgVer;

typedef enum {
        EclassAuthorization = 1,
        EclassFinancial,
        EclassFileAction,
        EclassReversalChargeback,
        EclassReconciliation,
        EclassAdministrative,
        EclassFeeCollection,
        EclassNetworkManagement
}TMsgClass;

typedef enum {
        EfuncRequest = 0,
        EfuncRequestResponse,
        EfuncAdvice,
        EfuncAdviceResponse,
        EfuncNotification
}TMsgFunc;

typedef enum {
        EorgAcquirer = 0,
        EorgAcquirerRepeat,
        EorgCardIssuer,
        EorgCardIssuerRepeat,
        EorgOther,
        EorgOtherRepeat
}TTrxnOrg;

typedef struct {
  unsigned char MsgVer   :4;
  unsigned char MsgClass :4;
  unsigned char MsgFunc  :4;
  unsigned char TrxnOrg  :4;
}TMsgDigits;

typedef union {
  unsigned short         Word;
        TMsgDigits  Digits;
}TMsgTypeID;

#define PEntryModeNULL          0x0000
#define PEntryModeManual        0x0010
#define PEntryModeMSR           0x0020
#define PEntryModeICC           0x0050
#define PEntryModePinEntryMASK  0xFFF0
#define PEntryModePinEntry      0x0001
#define PEntryModeNoPINEntry    0x0002

#define PCC_Normal             0x00
#define PCC_CustomerNone       0x01
#define PCC_Merchant           0x03
#define PCC_ECR_Interface      0x04
#define PCC_Pre_authorize      0x06
#define PCC_Mail_order         0x08
#define PCC_Open_Tab           0x51
#define PCC_Frequency_trxn     0x52
#define PCC_Mag_stripe_bad     0x71
#define PCC_Store_and_forward  0x87

#endif
