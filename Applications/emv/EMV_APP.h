/*---------------------------------------------------------------------------+
| Author:    RSBON
| File:      EMV_APP.H
| Purpose:   EMV_APP header file
| Changes:
|
| Date:      Author  Comment:
| ---------  ------  --------
| 26-Feb-02  RSBON   Created.
+---------------------------------------------------------------------------*/

#ifndef _EMV_APP_H_
#define _EMV_APP_H_

#include "emv_Apdu.h"
#include "EMV_CAT.H"
#include "../../../BF_SDK/INCLUDE/APPDRV/rsaeuro.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef  DBG_REPORT
    #define APIprt(x) Rprintf x
    #define DEBUG_DOL(y) debug_DOL y
#else
    #define APIprt(x)
    #define DEBUG_DOL(y)
#endif

/*-------------------------------------------------------*/
/*-------------------------------------------------------*/
#define GEN_AC_FIRST    1
#define GEN_AC_SECOND   2

#define MAX_EMV_TIAC    3
#define EMV_TIAC_Denial 0
#define EMV_TIAC_Online 1
#define EMV_TIACdefault 2

#define EMV_RID_BYTES   5
//#define MAX_TrmCAPK     24 //VISA & EUROPAY & DOMESTIC, each 6 MAX
#define MAX_TrmCAPK     50 //VISA & EUROPAY & DOMESTIC, each 6 MAX sispp061207 2CC.003.00

typedef struct {
    U16 *TraceNr;
	U08 *SelectedAID;
    char *CardAcceptorMerchantID;
    char *CardAcceptorTerminalID;
    char *TrmAcquirerID;
    char *POSSerialNr;
    char *TrmCapa;
    char *TrmCapaAdditional;
    char *TrmType;
    char *TrmTCC;
    char *TrmMCC;
    char *TrmForceOnLine;
    char *TrmL2TTAmode;
    char *SvcCode;
    char *CardHolderName;
} EMVREF;

extern EMVREF emvref;

/*-------------------------------------------------------*/
/*-------------------------------------------------------*/
extern EMV_RSA_PK *pgsTrmRsaKey;
extern EMV_RSA_PK gsTrmRsaKey[MAX_TrmCAPK];
extern R_RSA_PUBLIC_KEY gsEmvCA_PK, gsIssuerPK, gsICCardPK;
extern R_DIGEST_CTX gsSHSconText;

extern EMV_40BIT_TAG gsIsuIAC[MAX_EMV_TIAC]; //IssuerActionCodes(0x9F0D,E,F)
extern EMV_40BIT_TAG gsTrmTAC[MAX_EMV_TIAC]; //TerminalActionCodes(0x??)
extern EMV_40BIT_TAG *pgsTrmTVR;  //T5b_TrmTVR              0x95
extern EMV_16BIT_TAG *pgsTrmTSI;  //T2b_TrmTSI              0x9B
extern EMV_24BIT_TAG *pgsTrmCAPA; //T3b_TrmCapa             0x9F33
extern EMV_40BIT_TAG *pgsTrmACAP; //T5b_TrmCapaAdditional   0x9F40
extern EMV_AIP *pgsIccAIP;        //T2b_AIP                 0x82
extern U08 *pgsTrmISR;            //Tvb_TrmISR              0x9F5B
extern U08 *pgsTrmCVMR;           //T3b_TrmCVMR             0x9F34
extern U08 *pgbPOSEntryMode;      //T1n_TrmPOSEntryMode     0x9F39

extern U08 gbIccAIDno;
extern U08 gbIccAFLno;
extern U08 gbCDArequested, gbCryptoInfoDat;

extern int gwOffLineSDAstart, gwOffLineSDAbytes;

/*===============Start of External Functions ==============================*/
int app_BuildCandidateList(void);
int app_ReadRecordPSD(U08 SFI);
int app_ApplicationSelection(void);
int app_GetProcessingOptions(void);
int app_ReadApplicationData(void);
int app_OfflineDataAuthentication(void);
int app_ProcessRestrictions(void);
int app_CVMprocess(void);
int app_TRMprocess(void);
int app_TAAprocess(void);
int app_CAAprocess(void);
int app_OnLineProcess(void);
int app_IssuerAuthentication(void);
int app_Completion(void);
int app_IssuerScriptProcess(int scriptTag);

int app_initTrmTrxnVariables(void);
int app_OffLineSDAproc(void);
int app_OffLineDDAproc(void);
int app_CombinedCDAproc(void);
int app_getTC_HashValue(void);
int app_getDataPinBlock(void);
int app_getPinTryCounter(void);
int app_PlaintextPinVerifyICC(void);
int app_EncipheredPinVerifyICC(void);
int app_EncipheredPinVerifyOnLine(void);

int app_updateGenACresults(void);
int app_checkGenACresults(int nAC);
int app_checkVERIFYstatus(void);
int app_checkSamePrimitiveTag(U08 *cBuf, int cLen);
int app_checkTrmSupportOnLine(void);
int app_checkTVRwithIACorTAC(int optDOD);

int app_makeDOLobjects(int doTag, U08 *doBuf,int doLen, U08 *oBuf);
int app_searchTAGinDOL(U08 *srcPtr, int srcLen, int tarTag);

int app_InitReferralCall(void);
D64 app_getPANamounts(void);
int app_UPDATEgsXRec_ICCbL3(void);
int trans_UPDATEgsXRec_ICCbL3(void);

int app_SeletedAppTrmParameter(void);

int app_makeApplicationList();

#ifdef __cplusplus
}
#endif

#endif
