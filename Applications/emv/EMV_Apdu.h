/*---------------------------------------------------------------------------+
| Author:    RSBON
| File:      emv_Apdu.H
| Purpose:   emv_Apdu header file
| Changes:
|
| Date:      Author  Comment:
| ---------  ------  --------
| 26-Feb-02  RSBON   Created.
+---------------------------------------------------------------------------*/

#ifndef _emv_Apdu_H_
#define _emv_Apdu_H_

#include "../../../BF_SDK/INCLUDE/gem_TL.H"
#include "EMV_DEF.H"
#include "EMV_TAG.H"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MIPS_LITTLE
#define MIPS_LITTLE
#endif
/*===============Start of Definitions        ==============================*/
#define MAX_AppBufSIZE  8192
#define MAX_ATR_SIZE    (33+7)
/*-------------------------------------------------------*/
/* EMV Commands */
/*-------------------------------------------------------*/
#define EMV_GET_PROC_OPTIONS    0x80A80000 //Case4
#define EMV_GENERATE_AC         0x80AE0000 //Case4
    #define EMV_GAC_AAC         0x80AE0000
    #define EMV_GAC_TC          0x80AE4000
    #define EMV_GAC_ARQC        0x80AE8000
    #define EMV_GAC_CDA         0x80AE1000 //SU09
#define EMV_GET_DATA            0x80CA0000 //Case1,2
    #define EMV_GET_DATA_ATC    0x80CA9F36
    #define EMV_GET_DATA_ATCR   0x80CA9F13
    #define EMV_GET_DATA_PINTC  0x80CA9F17
#define EMV_CARD_BLOCK          0x84160000 //Case3
#define EMV_APP_UNBLOCK         0x84180000 //Case3
#define EMV_APP_BLOCK           0x841E0000 //Case3
#define EMV_PIN_CHANGE_UNBLOCK  0x84240000 //Case3
#define EMV_VERIFY              0x00200000 //Case3
    #define EMV_VERIFY_PLTXT    0x00200080
    #define EMV_VERIFY_ENTXT    0x00200088
#define EMV_EXT_AUTHENTICATE    0x00820000 //Case3
#define EMV_GET_CHALLENGE       0x00840000 //Case1,2
#define EMV_READ_RECORD         0x00B20000 //Case1,2
    #define EMV_READ_RECORD_NO  0x00B20004
#define EMV_INT_AUTHENTICATE    0x00880000 //Case4
#define EMV_SELECT              0x00A40000 //Case4
    #define EMV_SELECT_CDF      0x00A40100 /* Select child DF */
    #define EMV_SELECT_CEF      0x00A40200 /* Select EF in current DF */
    #define EMV_SELECT_PDF      0x00A40300 /* Select Parent DF  */
    #define EMV_SELECT_NAME     0x00A40400 /* Select by DF name */
    #define EMV_SELECT_NEXT     0x00A40002 /* Select Next occurrence */
#define EMV_GET_RESPONSE        0x00C00000 //Case1,2
/*-------------------------------------------------------*/
/* For GFM2K4K(AT88SC1608) Card Commands   P1/P2=AddrH/L */
/* For GPM2K(SLE4432/4442 or PCB2032/2042) Card Commands */
/*-------------------------------------------------------*/
    #define GFM4K_CARD_TYPE     (U08)0x06
    #define GEM2K_CARD_TYPE     (U08)0x09
    #define GEM2K_WRITE_MEMORY  0x00D00000
    #define GEM2K_READ_MEMORY   0x00B00000
    #define GEM2K_WRITE_PROTECT 0x00D08000
    #define GEM2K_READ_PROTECT  0x00B08000
    #define GEM2K_WRITE_SECURE  0x00D0C000
    #define GEM2K_READ_SECURE   0x00B0C000
    #define GEM2K_VERIFY_PSC    0x00200000
/*===============Start of Structure Definitions ===========================*/
typedef struct {
    U08 status;
#ifdef MIPS_LITTLE
    U08 V3V  : 1;  /* Power Supply Voltage 3.3V/5.0V*/
    U08 pwON : 1;  /* Card Power ON*/
    U08 CARD : 1;  /* Card Inserted*/
    U08 T1   : 1;  /* T1 protocols/T0*/
    U08 SCno : 4;  /* Smart Card Slot Number*/
#else
    U08 SCno : 4;  /* Smart Card Slot Number*/
    U08 T1   : 1;  /* T1 protocols/T0*/
    U08 CARD : 1;  /* Card Inserted*/
    U08 pwON : 1;  /* Card Power ON*/
    U08 V3V  : 1;  /* Power Supply Voltage 3.3V/5.0V*/
#endif
    U08 TYPE;      /* Activated Card Type*/
    U08 CNF1;      /* TA1(FI/DI)*/
    U08 CNF2;      /* TC1(EGT)*/
    U08 CNF3;      /* WI / IFSC*/
    U08 CNF4;      /* 00 / TB3 (BWI/CWI)*/
} GSC_STAT;

/*===============Start of External Variables ==============================*/
extern GSC_STAT gsSC_STAT, *pgsSC_STAT;
extern U08 gem_Protocol;
extern U08 gem_AUXcmmd;
extern U08 gem_RS232open;
extern U08 gem_BaudFlag;
extern U32 gem_BaudRate;

extern U08 gbScBuf[GBP_SIZE];
extern int giScLen;
extern U08 *pgbAppBuf;
extern int giAppBufLen;
extern U08 *pgbRspBuf;
extern int giRspBufLen;
extern U16 gwSW1SW2;

extern U08 gbATRdata[MAX_ATR_SIZE];
extern S08 gbTagFormat, gbTagLength;
extern U08 gbNthMatch;

extern U08 gbDestBuf[512], gbSignBuf[512], gbTempBuf[512];
extern U08 *pgcTrmDATA;
extern int giTrmDATAsize;
extern int giTrmCAPKsize;

extern const U08 gcTrmCAPK[];
extern const char fileNameCAPK[];
extern const EMV_TagNAME  gcTagName[];

/*===============Start of External Functions ==============================*/
int gsc_IFMinit(void);
int gsc_cmmdOUTPUTngetRESPONSE(int cmdLen);
int gsc_Power(int power);
int gsc_Status(void);
int gsc_ChangeParameters(U08 *PRT_CNF);
int gsc_SetCardTYPEandSLOT(U08 Type, U08 SlotNo);
int gsc_ISOwrite(U32 CLAinsP1P2, U16 Lc, U08 *pData);
int gsc_ISOread(U32 CLAinsP1P2, U16 Lc);
int gsc_FWversion(void);

int Apdu(U08 *ApduData);      /* Case 1, Case 2,  Case 3, Case 4*/
int ApduC4(U32 CLAinsP1P2, U16 Lc, U08 *pData); /* Case 3, Case 4*/
int ApduC2(U32 CLAinsP1P2);                   /* Case 1, Case 2*/
int ApduResponseCheck(void);

int pgbAppBufAdjust(int pLen);
int pgbAppBufUpdate(int uTag, int uLen, U08 *uDat);
int debug_FCI(U08 *dolBuf, int dolLen);
int debug_DOL(U08 *dolBuf, int dolLen);
int debugCDOL(U08 *cdoBuf, int cdoLen);
int getTagInfo(int tag, U08 prt);
int getTnL(U08 *DatPtr, int DatLen, int *tagVAL, int *tagLEN);
int getDnL(U08 *BufPtr, int BufLen, int TagVAL, U08 **pDATA);
int getSysDOLsize(U08 *sysDoL);
int getEMVgbPinBlock(char *pinPtr);
char *getCurrencyName(int code);

int isoHostSimRspMsg(void);
int isoHostDownLoadFile(void);
int isoHostTxDsend(char *txD);
int isoHostRxDcheck(void);

#ifdef __cplusplus
}
#endif

#endif
