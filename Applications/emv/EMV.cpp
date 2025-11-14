/*
 * EMV.c

 *
 *  Created on: Sep 8, 2014
 *      Author: Mahmoud Swedan
 */
extern  "C" {
#include "../../../BF_SDK/INCLUDE/system.h"
#include "../../../BF_SDK/INCLUDE/externs.h"
#include "../../../BF_SDK/INCLUDE/system.h"
//#include "../../../BF_SDK/INCLUDE/gem_TL.h"
//#include "../../../BF_SDK/INCLUDE/global.h"
#include "../../../BF_SDK/INCLUDE/lib/printf.h"
#include "../../../BF_SDK/INCLUDE/lib/ISO8583.H"
#include "../../../BF_SDK/INCLUDE/APPDRV/DES_ECB.H"
#include "../../../BF_SDK/INCLUDE/APPDRV/rsaeuro.h"
#include "../../../BF_SDK/INCLUDE/APPDRV/rsa.h"
#include "EMV_APP.H"
#include "Disp.H"
#include "EMV.H"
#include "../../../BF_SDK/INCLUDE/hal/usip/hal_scs.h"
#include "../../../BF_SDK/INCLUDE/iso3166.h"


#include "../resources/Msgs.h"
#include "../display.h"
//#include "../AppGlobal.h"


extern void bcd2asc(char *dst, char *src, int len);
  extern int gsoap_NBE(char * EMVTags, int TagsStrLen,int TXNIndex);
extern  int OfflineTXN(char * EMVTags, int TagsStrLen);
extern void vidSaveTansDetails(void);
extern void vidClearReversal(void);
extern unsigned char ucPrntCustomerRct(void);
extern int iChosePrintMerchCopy(ushort usHdrIdx);
extern unsigned char ucPrntFailedRct();

}
#include "../BSndRcv.h"

//#include "../../iso/CMegaContainer.h"

#ifndef DBG_REPORT
#define DBG_REPORT
#endif
#ifndef DRV_REPORT
#define DRV_REPORT
#endif

/* PRINTF.C */
extern "C"  int Rprintf (const char *format, ...); /* Our main debug */
extern "C"  int giDEBUG_COM, giDEBUG_END;
extern "C"  void setDEBUG_COMport(int comport);
extern "C"  void DebugBUF(U08 *DbgBuf, int DgLen);
extern "C"  char KEYSCAN(void);
extern "C"  void LCD_BLINKING(char, char, char, char);
extern "C"  void Wait1MS( int ms );
extern "C"  void CARD_ENABLE(void);


extern  "C" char *pfnHex2Asc(U08 *ascBuf, U08 *hexBuf, int nx);
extern  "C" char *pfnAsc2Hex(U08 *hexBuf, U08 *ascBuf, int nx);
extern  "C" int fnAsc2Bcd(U08 *bcdBuf, U08 *ascBuf, int nx);
extern  "C" int fnD642Bcd(U08 *bcdBuf, D64 dwNo, int nx);
extern  "C" D64 fnBcd2D64(U08 *bcdBuf, int nx);
extern  "C" U08 *pfnRTC2YMDHMS(char *pYMD);

extern  void * memmem(const void *l, size_t l_len, const void *s, size_t s_len);
extern const int getLengthBCD(const int nLenSrc);


extern BYTE SC_INPUT[512], SC_OUTPUT[512];
#define COM_PORT0   0
#define COM_PORT1   1


extern U08 gbGemCoR15;
extern U16 gSysTimer;
extern int  TIME_BASE0, TIME_BASE1, TIME_BASE10;
extern char CardNo1[112], CardNo2[52];
extern const U08 gcTrmDATA[];


extern const SYSINFOT SysInfoBNK;


TgsPOS gsPOS;
SYSINFOT *pgsSysInfo, gsSysInfo;
TRM_PARAM gsTrmParam;
U16 gSysTimer;

TTrxnRec gsXRec, *pXRec=NULL, *pXRec_Min=NULL, *pXLog=NULL;
TTrxnRec *pXFileMax=NULL, *pXFileMin=NULL;
U32 gu32ProcessingCode=0L, gu32ProcessingCodeNext=0L;
U32 gdwAmount=0L;

int giTrxnNOnFlag=0;
U16 ghResponseCode=0, gwResponseCode=0, gwTrmARC=0;
U16 gwCurrencyCode;
U08 gbCurrencyExp;
U08 gbSignatureFlag=0;
U08 gbPinBlock[8];
S08 gbReversalFlag=0, gbRefundFlag=0, gbOnLineFlag=0, gbReferralFlag=0;
S08 gbSaveTrxnRec=0, gbValidateBit=0;
BYTE Total_Amount[16];
char gbTmpKeyBuf[20];

U08 gbForceOffline =0;  //Heba

int MSRFlag = 0, sisPOSEntry = 0;	// sispp040830 2CM.091.00
char gbKeyVal;

S08 gbAmountIN=0, gbTrxnType=0, gbPin_Type=0, gbPin_TypeDone=0;
extern "C" const U08 gcHex2Asc[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
const U08 gczzFlag_z='z', gczzFlag_h='h';

unsigned short gwModemWait=0, gwModemTOut=0, gtwCRC=0, gtwLRC=0;

//int giDEBUG_COM=0, giDEBUG_END=0;

char gbEmvScTaskStep=0;

/*unsigned short deltaMS(unsigned short refMS, unsigned short tBase)
{
  long tTms;


    if(tBase) { // Interrupt decrease TIME_BASE0,TIME_BASE1
        tTms = refMS - TIME_BASE1;
	}else {
        tTms = refMS - TIME_BASE0;
	}

  switch(tBase) {
	case 0: tTms = refMS - TIME_BASE0; break;
	case 1: tTms = refMS - TIME_BASE1; break;
	case 10: tTms = refMS - TIME_BASE10; break;
  }
  if(tTms < 0) tTms += 0x10000L;

  return((unsigned short)tTms);
}*/
int WaitKeyInput( void )
{
  while( 1 )
  {
	gbKeyVal=KEYSCAN() & 0xFF;
	if(gbKeyVal != -1) break;
  }
  return(gbKeyVal);
}


int gsc_IFMinit(void)
{
  setDEBUG_COMport(COM_PORT0);
#ifdef BITEL_IC5000_RFM
  CONTROL_POWER_SAVING_STATUS(3,ON); // SMART CARD
  gem_RS232open=0;
#endif
  pgsSC_STAT = (GSC_STAT *)&gsSC_STAT;
  memset(pgsSC_STAT, 0, sizeof(GSC_STAT));
  //gem_Protocol = TLP_PROTOCOL; //Set TLP Protocol for T=0
  gem_Protocol = GBP_PROTOCOL; //Set GBP Protocol for T=1
  if(!gem_RS232open)
  {
	//====Open Serial Port 0 at 9600 Baud================
	if(gem_BaudRate < 9600L) gem_BaudRate=9600L;
#ifdef GEM_IFM_DOUBLE_SPEED
	//====Open Serial Port 0 at 19200Baud================
	if(gem_BaudRate==9600L)
	{
	  TL_Open(0, gem_BaudRate);
	  //Set COM0 speed set done, and configure SIO line
	  DBGprt(("SetSIO_Baud(9600(4)->19200(3)"));
	  SC_OUTPUT[0]=0x0A;
	  //SC_OUTPUT[1]=0x02; // 38400L
	  SC_OUTPUT[1]=0x03; // 19200L
	  //SC_OUTPUT[1]=0x04; //  9600L default
	  gsc_cmmdOUTPUTngetRESPONSE(2); // Always Error!
	  gem_BaudRate=19200L;
	}
#endif
	TL_Open(0, gem_BaudRate);
  }

  /*Get Firmware version from Reader   */
  if(gsc_FWversion() != NO_ERR)
  {
	DBGprt(("\r\n.gsc_FWversion() Error!\r\n"));
	return(-1);
  }

  //Disable TLP compatibility
  //In TLP Mode, the GemCore Firmware adds TA1,TB1,TC1 and TD1 bytes
  //if they are not present in the asynchronous smart card Answer to Reset
  SC_OUTPUT[0]=0x01;
  SC_OUTPUT[1]=0x00;
  SC_OUTPUT[2]=0x00;
  DBGprt(("GEM_SET_MODE"));
  if(gsc_cmmdOUTPUTngetRESPONSE(3) != NO_ERR) return(-1);
  //if(gsc_cmmdOUTPUTngetRESPONSE(2) != NO_ERR) return(-1);

  //Get supported Card Type, Directory
  SC_OUTPUT[0]=0x17;
  SC_OUTPUT[1]=0x00;
  DBGprt(("SupportCardType"));
  if(gsc_cmmdOUTPUTngetRESPONSE(2) != NO_ERR) return(-1);

  //Switch Reader into ISO_MODE, EMV_MODE
  SC_OUTPUT[0]=0x17; SC_OUTPUT[1]=0x00; SC_OUTPUT[2]=0x00; // Just read MODE
  //SC_OUTPUT[0]=0x17; SC_OUTPUT[1]=0x00; SC_OUTPUT[2]=0x47; // ISO_MODE(WinCARD)
  //  SC_OUTPUT[0]=0x17; SC_OUTPUT[1]=0x00; SC_OUTPUT[2]=0x45; // EMV_MODE
  DBGprt(("ISO/EMV MODE"));
  if(gsc_cmmdOUTPUTngetRESPONSE(3) != NO_ERR) return(-1);
  if(gsc_Status() < 0) return(-1);

  return(NO_ERR);
}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

int gsc_cmmdOUTPUTngetRESPONSE(int cmdLen)
{
  gSysTimer = TIME_BASE0;		// sispp 070110 2CJ.010.01
  gwSW1SW2=0;
  cmdLen=TL_SendReceive((U32)cmdLen,(U08 *)&SC_OUTPUT[0],(U32 *)&giScLen,(U08 *)&gbScBuf[0]);
  if(cmdLen == NO_ERR) return(0);
#ifdef DBG_REPORT
  DRVprt(("<E>GemTL:"));
  switch(cmdLen)
  {
	case ERR_SCS_CMD_ABORTED		: DRVprt(("Command aborted")); break;
	case ERR_SCS_CARD_MUTE			: DRVprt(("Card mute.")); break;
	case ERR_SCS_PARITY				: DRVprt(("Parity error during an exchange with the card.")); break;
	case ERR_SCS_OVERRUN			: DRVprt(("Card consuming too much current or is short-circuiting")); break;
	case ERR_SCS_CARD_ABSENT		: DRVprt(("Card absent. There is no card in the smart card interface.")); break;
	case ERR_SCS_POWER_UP			: DRVprt(("Card powered down.")); break;
	case ERR_SCS_INVALID_ATR		: DRVprt(("Error in the card reset response")); break;
	case ERR_SCS_BAD_ATR_TCK		: DRVprt(("In the card reset response, the Check Character (TCK) is not correct")); break;
	case ERR_SCS_PROTOCOL			: DRVprt(("Protocol error")); break;
	case ERR_SCS_CLASS_NOT_SUPPORTED: DRVprt(("The first byte of command (CLA) is invalid")); break;
	case ERR_SCS_PROC_BYTE			: DRVprt(("The card sent an invalid procedure byte")); break;
	case ERR_SCS_INVALID_SPEED		: DRVprt(("Speed (TA1 parameter) not supported")); break;
	case ERR_SCS_UNKNOW				: DRVprt(("Function not implemented")); break;
	case ERR_SCS_INVALID_NAD		: DRVprt(("The card sent an invalid NAD (T=1)")); break;
	case ERR_SCS_OVERFLOW			: DRVprt(("Response too long for the buffer")); break;
  }
  DRVprt(("\n"));
#endif
  if(gbEmvScTaskStep > 3)
  {
	memset(pgsSC_STAT, 0, sizeof(GSC_STAT));
	gbEmvScTaskStep=126; //call goto appEmvTaskEnd;
  }

  return(-1);
}
void EMVDISP(int no)
{
  switch(no)
  {
	case LsEMVreading:
	  STRDISP( LsEMVreading );
	  break;
	case LsEnterPIN:
	  STRDISP( LsEnterPIN );
	  break;
	case LsPINtryLast:
	  STRDISP( LsPINtryLast );
	  break;
	case LsPIN_OK:
	  STRDISP( LsPIN_OK );
	  break;
	case LsPINincorrect:
	  STRDISP( LsPINincorrect );
	  break;
  }/**/
}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
extern "C" int isoCardNo22Track2Data(U08 *pTrac)
{
  int i,j,cn2Len;

  cn2Len=CardNo2[0];
  if(cn2Len > 37) cn2Len=37;
  pTrac[0] = ((cn2Len/10)<<4) | (cn2Len%10); //cnL2 format

  i=j=1;
  while(1)
  { //convert an->cn format
	if(CardNo2[i] == 0) break;
	if(i&1)
	{
	  pTrac[j]  = (CardNo2[i] << 4);
	}
	else
	{
	  pTrac[j] |= (CardNo2[i] & 0x0F); j++;
	}
	if((i++) >= cn2Len) break;
  }
  if((i&1)==0)
  {
	pTrac[j] |= 0x0F;
	i++; j++;
  }

  if(cn2Len <= 20)
  { // TYPING NOTATATION:MAX[19+1]
	pTrac[j++] = 0xDD; i += 2;
	pTrac[j]  = (CardNo2[i++] << 4);
	pTrac[j] |= (CardNo2[i++]& 0xF); j++;
	pTrac[j]  = (CardNo2[i++] << 4);
	pTrac[j] |= (CardNo2[i++]& 0xF); j++;
  }

  //DBGprt(("isoCardNo22Track2Data")); DEBUG_BUF((pTrac, j));
  return(j); //Bytes including Length in pTrac[0]
}
/*
 *
 */
extern "C" int isoTrack2Data2CardNo2(U08 *pTrac, int nByte, U08 *pCard)
{
  int i=1;

  if(nByte <= 0)
  {
	nByte = (*pTrac >> 4)*10 + (*pTrac & 0x0F); //cnL2 format
	pTrac++;
  }
  if(nByte > 37) nByte=37;
  pCard[0] = nByte;
  while(i <= nByte)
  { //convert cn->an format
	pCard[i++]=(*pTrac >> 4)+0x30;
	pCard[i++]=(*pTrac &0xF)+0x30;
	pTrac++;
  }

  if(pCard[0] <= 20) { // TYPING NOTATATION:MAX[19+1]
	pCard[i++]='='; // PAD string
	pCard[i++]='='; // PAD string
	pTrac++;
	pCard[i++]=(*pTrac >> 4)+0x30; // YY
	pCard[i++]=(*pTrac &0xF)+0x30;
	pTrac++;
	pCard[i++]=(*pTrac >> 4)+0x30; // MM
	pCard[i++]=(*pTrac &0xF)+0x30;
  }
  pCard[i]=0;
  //DBGprt(("isoTrack2Data2CardNo2[%d]=%s\r\n", pCard[0], &pCard[1]));

  return(i);
}
/*
 *
 */
int isoStrPANnExpDate(char *pPAN)
{
  char i, j, cd;

  i=j=1;
  while(1)
  {
	if((cd=CardNo2[i])==0) break;
	if((cd == 0x3F) || (cd == '=')) break;
	pPAN[j++]=CardNo2[i++];
  }
  pPAN[0]=(j-1);
  pPAN[j++]=0; // Null string
  while(1)
  {
	if((CardNo2[i] >= '0') && (CardNo2[i] <= '9')) break;
	i++;
  }
  pPAN[j++]=CardNo2[i++];
  pPAN[j++]=CardNo2[i++];
  pPAN[j++]=CardNo2[i++];
  pPAN[j++]=CardNo2[i++];
  pPAN[j]=0; // Null string

  return((int)pPAN[0]);
}
/*
 *
 */
D64 app_getPANamounts(void)
{
  D64 Amounts=0.0;

  /*Calculate total amount of samePAN in this BATCH. RSBON_CODE */
  pXRec = NULL;
  while(1)
  {
	//if(isoGetPanRecords(&CardNo2[0]) < 0) break;
	if(pXRec->zzFlag != gczzFlag_z) break;
	Amounts += fnBcd2D64((U08 *)&pXRec->AmountTrxn, 12);
	pXRec--;
  }
  pXRec = NULL;
  return(Amounts);
}
/*
 *
 */
void initTrmCurrency(void)
{
  U08 *pAdd;

  pgcTrmDATA=(U08 *)&gcTrmDATA[0];
  if(getDnL(pgcTrmDATA,0x7FFF,T2n_TrmTrxnCurrencyCode,&pAdd)==2)
  {
	gwCurrencyCode=(U16)pAdd[0]<<8 | pAdd[1];
  }
  if(getDnL(pgcTrmDATA,0x7FFF,T1n_TrmTrxnCurrencyExp, &pAdd)==1)
  {
	gbCurrencyExp=pAdd[0];
  }
  //    if(getDnL((U08 *)gsFlashFAT[1].FileAddr, 0x7FFF, Tvb_TrmAID, &pAdd) > 0) {
  //        pgcTrmDATA=(U08 *)gsFlashFAT[1].FileAddr;
  //    }
  //    gwCurrencyCode=ISO3166_EG; //(ISO3166_BE=0x056,ISO3166_IN=0x356, ISO3166_MY=0x458,ISO3166_GR=0x300,)
  //    gbCurrencyExp =2;

  giTrmDATAsize=getSysDOLsize(pgcTrmDATA);
  //DBGprt(("gcTrmDATA[]=(%d, %d)\r\n", giTrmDATAsize, sizeof(gcTrmDATA)));
}

/*
 *
 */
int app_InitReferralCall(void)
{
  STRDISP(0);
  Lprintf(0, 0, (char *)&msg_tbl1[gbLangCode][72]); // "CALL YOUR BANK  ", // 72
  isoStrPANnExpDate((char *)&CardNo1[0]);
  Lprintf(1, 0, (char *)&CardNo1[1]);   // Card No Display
  if(CardNo1[0] > 16)
  {
	Lprintf(2, 0, (char *)&CardNo1[17]);
  }
  Lprintf(3, 0, (char *)&msg_tbl1[gbLangCode][73]); // "CANCEL OR ENTER ", // 73
  LCD_BLINKING(CURBOX_ON , 0, 0, 16 );

  APIprt(("PAN=%s",(char *)&CardNo1[1]));
  while(1)
  {
	//	    dequeue_key();
	gbKeyVal=KEYSCAN() & 0xFF;
	if(gbKeyVal == BTKEY_ENTER) break;
	else if(gbKeyVal == BTKEY_CANCEL) return(-1);
  }

  return(0);
}


/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
int app_SeletedAppTrmParameter(void)
{
  int pLen, nLen, nTag, oTag, i;
  U08 *pBuf;

  nLen=gsPOS.TrxnRec.SelectedAID[0];
  if((nLen < 5) || (nLen > 16))
  { // Only 5-16 bytes
	//        pgcTrmDATA=(U08 *)gsFlashFAT[1].FileAddr;
	//        giTrmDATAsize=getSysDOLsize(pgcTrmDATA);
#define FLASH_FILE_SIZE_MIN 	0x002000L // 8 KB

	if((giTrmDATAsize <= 0) || (giTrmDATAsize > FLASH_FILE_SIZE_MIN))
	{
	  DBGprt(("Select Default Param\r\n"));
	  pgcTrmDATA=(U08 *)&gcTrmDATA[0];
	  giTrmDATAsize=getSysDOLsize(pgcTrmDATA);
	}
	return(0); // initial SeletedAppTrmParameter
  }
  for(i=1; i<64; i++)
  {
	gbNthMatch=i;
	if((pLen=getDnL(pgcTrmDATA, 0, Tvb_TrmAID, &pBuf)) <= 0) return(0);
	if(memcmp((U08 *)&gsPOS.TrxnRec.SelectedAID[1], pBuf, pLen) == 0)
	{
	  if(pLen == nLen) break;           // no multiple
	  if((pLen < nLen)&&(i > 2)) break; // partial...
	}
  }
  pBuf += pLen;
  giTrmDATAsize -= ((long)pBuf - (long)pgcTrmDATA);
  pgcTrmDATA = pBuf;
  pLen=giTrmDATAsize;
  oTag=0;
  while(pLen > 0)
  {
	i=getTnL(pBuf, pLen, &nTag, &nLen);
	if(nTag != Tvb_TrmAID)
	{ // If not Tvb_TrmAID
	  if(oTag==0) oTag=nTag;
	  if(nTag==Tag_EOT4) break;
	}
	else if(oTag)
	{ // If not successive Tvb_TrmAID
	  // Restore SeletedAppTrmParameter bytes
	  giTrmDATAsize = ((long)pBuf - (long)pgcTrmDATA);
	  break;
	}
	i += nLen;
	pLen -= i;
	pBuf += i;
  }
  return(1); // use revised SeletedAppTrmParameter
}
/*
 *
 */
int app_ApplicationSelection(void)
{
  U08 *pAdd;
  int  i, n, sNum;

  if(app_makeApplicationList() == -1)
	return -1;

  /* ---------------------- */
  // Display in Priority Order
  /* ---------------------- */
  STRDISP(LsEMVselect);
  pAdd=(U08 *)&gbDestBuf;
  i=n=sNum=0;
  while((i++) < gbIccAIDno)
  {
	if(!pAdd[61])
	{ // No deleted
	  APIprt(("\t[%2X]Priority[%s]sorted[%d]\r\n", pAdd[60], (char *)&pAdd[32], pAdd[62]));
	  if(pAdd[62] <= 4) Lprintf((pAdd[62]-1), 0, (char *)&pAdd[32]);
	  if((pAdd[60] & 0x80) == 0) n=1; // n=1 : Select Option Terminal_AUTO
	  sNum++;                         // sNum: Display Lines
	}
	pAdd += 64;
  }
  /* ---------------------- */
  // Select by Cardholder
  /* ---------------------- */
  if((gbIccAIDno > 1) || (n==0))
  { // Select Option is not Terminal_AUTO
	if(gsPOS.TrxnRec.SelectedAID[0] != 0)
	{ // not first selection
	  i=3;
	  if(sNum < 3) i=2;
	  Lprintf(i, 0, (char *)&msg_tbl1[gbLangCode][87]); //"TRY AGAIN       ", // 87
	}
	n=1;
	LCD_BLINKING(CURBOX_ON , 0, 0, 16 );
	while(1) {
	  //            dequeue_key();
	  gbKeyVal=KEYSCAN() & 0xFF;
	  if((gbKeyVal > BTKEY_0) && (gbKeyVal <= sNum))
	  {
		if(n != gbKeyVal)
		{
		  if(sNum > 4 && (((n-1) ^ (gbKeyVal-1)) & 0x04))
		  {	// Change Page
			STRDISP(LsEMVselect);
			i = 0;
			pAdd=(U08 *)&gbDestBuf;
			while((i++) < sNum)
			{
			  if(!pAdd[61])
			  { // No deleted
				if((gbKeyVal > 0x04 && pAdd[62] > 4) || (gbKeyVal <= 0x04 && pAdd[62] <= 4))
				{
				  Lprintf((pAdd[62]-1) & 0x03, 0, (char *)&pAdd[32]);
				}
			  }
			  pAdd += 64;
			}
		  }
		  LCD_BLINKING(CURBOX_OFF, (n-1) & 0x03, 0, 16 );
		  LCD_BLINKING(CURBOX_ON , (gbKeyVal-1)&0x3, 0, 16 );
		  n=gbKeyVal;
		}
	  }
	  else if(gbKeyVal == BTKEY_STAR)
	  {
		if(sNum > 4) gbKeyVal = (BTKEY_1);
	  }
	  else if(gbKeyVal == BTKEY_SHARP)
	  {
		if(sNum > 4) gbKeyVal = (BTKEY_5);
	  }
	  else if(gbKeyVal == BTKEY_ENTER) break;
	  else if(gbKeyVal == BTKEY_CANCEL) return(-1);

	}
  }
  /* ---------------------- */
  APIprt(("ApplicationSelected[%d/%d]\r\n", n, sNum));
  /* ---------------------- */
  pAdd=(U08 *)&gbDestBuf;
  i=0;
  while((i++) < gbIccAIDno)
  {
	if(pAdd[62] == n)
	{ // sort number match?
	  memcpy((U08 *)&gsPOS.TrxnRec.SelectedAID[0], (U08 *)&pAdd[0], (pAdd[0]+1));
	  STRDISP( LsEMVreading );
	  Lprintf(0, 0, (char *)&pAdd[32]);
	  return(0);
	}
	pAdd += 64;
  }

  return(-1);
}
/*
 *
 */
int OnLineProcessSub(int iReferral)
{
  if(iReferral)
  {
	APIprt(("Referral Processing!\r\n"));
	if(app_InitReferralCall() < 0) return(-1);
	STRDISP(LsEMVreferral);
	LCD_BLINKING(CURBOX_ON , 0, 0, 16 );
	gbReferralFlag=BTKEY_1;
	while(1)
	{
	  //	  dequeue_key();
	  gbKeyVal=KEYSCAN() & 0xFF;
	  if((gbKeyVal >= BTKEY_1) && (gbKeyVal <= BTKEY_3))
	  {
		if(gbReferralFlag != gbKeyVal)
		{
		  LCD_BLINKING(CURBOX_OFF, (gbReferralFlag-1), 0, 16 );
		  LCD_BLINKING(CURBOX_ON , (gbKeyVal-1), 0, 16 );
		  gbReferralFlag=gbKeyVal;
		}
	  }
	  else if(gbKeyVal == BTKEY_ENTER)
	  {
		if(gbReferralFlag==BTKEY_1)
		{			//"    goto ON_LINE", //124
		  gwTrmARC = 0x3030; //shall not set ARC book4 2.5.2.1
		  gbTempBuf[0] = (gwTrmARC >> 8) & 0xFF;
		  gbTempBuf[1] = gwTrmARC & 0xFF;
		  pgbAppBufUpdate(T2a_ARC, 2, gbTempBuf);
		  gbOnLineFlag=1;
		}
		else if(gbReferralFlag==BTKEY_2)
		{		//" OffLINE APPROVE", //125
		  gwResponseCode=0x3030;
		}
		else if(gbReferralFlag==BTKEY_3)
		{		//" OffLINE DECLINE", //126
		  gwResponseCode=0x3531;
		}
		break;
	  }
	  else if(gbKeyVal == BTKEY_CANCEL)
	  {
		return(-1);
	  }
	}
  }

  //isoRequestMsgBNK(gu32ProcessingCode);

  if(gbOnLineFlag == 1)
  {
	STRDISP( LsEMVOn_Line );
	Rprintf("Pin_Block\r\n");
	DebugBUF((unsigned char *)&gbPinBlock[0],16);

	gbEmvScTaskStep = 99;

	while(1)
	{
	  //dequeue_key();
	  gbKeyVal=KEYSCAN() & 0xFF;
	  if(gbKeyVal == BTKEY_CANCEL) return(-1);
	  else if(gbKeyVal == BTKEY_ENTER) break;
	  else if(gbKeyVal == BTKEY_CLEAR)
	  {
		gbOnLineFlag = -1; // Unable to go OnLine
		gbEmvScTaskStep=100;
		break;
	  }
	}
  }
  else
  {
	STRDISP( LsEMVOffLine );
	gbEmvScTaskStep = 100;
  }

  CSndRcv oSndRcv;
//  if (oSndRcv.m_pContainer->getHostProtocol() == NBE)
 gsoap_NBE((char*)pgbAppBuf, giAppBufLen,0);
//	oSndRcv.ucNBESale((char*)pgbAppBuf, giAppBufLen);
//  else   if (oSndRcv.m_pContainer->getHostProtocol() == FDATA)
//	oSndRcv.ucCubeSale((char*)pgbAppBuf, giAppBufLen);
  //isoHostSimRspMsg();

  return(0);
}
/*
 *
 */
void CompletionSub()
{
  /**/
  U08 *pAdd;
  int pLen;

  DBGprt(("CompletionSub\n"));
	DBGprt(("gbRefundFlag = %d\r\n",gbRefundFlag));
  if(getDnL(pgcTrmDATA, 0, T2a_ARC, &pAdd) == 2)
  {
	gwResponseCode=((U16)pAdd[0]<<8) | pAdd[1];
  }
  else
  {
	gwResponseCode=0x3030;
  }


//	gwResponseCode=0x3030;

//  APIprt(("Isseur Referral!\r\n"));
//  if(app_InitReferralCall() < 0); //return(-1);
//  STRDISP(LsISSreferral); // RSBON_CETECOM 3/26 ICS modified.
//  LCD_BLINKING(CURBOX_ON , 1, 0, 16 );
//  pLen=BTKEY_2;
//  while(1) {
//	//	dequeue_key();
//	gbKeyVal=KEYSCAN() & 0xFF;
//
//	if((gbKeyVal >= BTKEY_2) && (gbKeyVal <= BTKEY_3)) {
//	  if(pLen != gbKeyVal) {
//		LCD_BLINKING(CURBOX_OFF, (pLen-1), 0, 16 );
//		LCD_BLINKING(CURBOX_ON , (gbKeyVal-1), 0, 16 );
//		pLen=gbKeyVal;
//	  }
//	}else if(gbKeyVal == BTKEY_ENTER) {
//	  if(pLen==BTKEY_2) { //"    APPROVE     ", //125
//		gwResponseCode=0x3030;
//	  }else
//		if(pLen==BTKEY_3) { //"    DECLINE     ", //126
//		  gwResponseCode=0x3531;
//		}
//	  //------------------------------------------------
//	  // Change ResponseCode for ISSUER REFERRAL.
//	  //------------------------------------------------
//	  if(getDnL(pgcTrmDATA, 0, T2a_ARC, &pAdd) == 2) {
//		pAdd[0]=(gwResponseCode >> 8) & 0xFF;
//		pAdd[1]=(gwResponseCode & 0xFF);
//	  }
//
//	  /*	  isoHostSimRspMsg();
//	  while(1) {
//		if(get_message(&gsPOS.pMsgQue) <= 0) break;
//	  }
//	  if(isoGetMsgInfo(isoRxDptr, isoRxDcnt) == 0) {
//		isoGetTrxnRecords();
//	  }*/
//	  break;
//	}
//  }
}

/*
 *
 */
int PrepNBESubField(char* TagVAL , U08* Result)
{
  char Tagbcd [10]; memset(Tagbcd,0,sizeof(Tagbcd));
  asc2bcd(Tagbcd,TagVAL,strlen(TagVAL));
  char * pch;
  int TagLen = getLengthBCD(strlen(TagVAL));
  if ((pch = (char *)memmem((void*)pgbAppBuf, (size_t)giAppBufLen, (void*)Tagbcd, (size_t)TagLen))!= NULL)
  {
	memcpy( Result, pch, ( pch[TagLen] + TagLen + 1) );
	  DEBUG_BUF((Result,TagLen + 1 + pch[TagLen]));
	return (TagLen + 1 + pch[TagLen]);

  }
  return 0;
}
/*
 *
 */
int PrepSubField(int SubFldId,int TagVAL, char eFieldType, U08* Result)
{
  U08 *ptr, *pAdd;

  int TagLen = getDnL(pgbAppBuf, giAppBufLen,TagVAL , &pAdd);
  if (eFieldType == 0)
  {
	TagLen *= 2;
	bcd2asc((char *)&Result[6],(char*)pAdd,TagLen);
	if (TagLen == 4)
	{
	  for (int i = 0; i<3; i++)
	  {
		  Result[6+i] = Result[7+i];
	  }
	   TagLen = 3; Result[6 + TagLen] = 0;
	}
  }
  else if (eFieldType == 1) memcpy((char *)&Result[6],(char*)pAdd, TagLen);
  else if (eFieldType == 2)
  {
	  TagLen *= 2;
	  bcd2asc((char *)&Result[6],(char*)pAdd,TagLen);
  }

  Result[0]= '0'; Result[1] = '0';
  Result[2] = (TagLen + 2)/10 + '0';
  Result[3] = (TagLen + 2)%10 + '0';
  Result[4] = SubFldId/10 + '0'; Result[5] = SubFldId%10 + '0';
  DBGprt(("[%d]-> %x: %s\n",SubFldId,TagVAL,Result));
  return TagLen + 6;
}
/*--------------------------------------------------------------------------*/
//B55bL3_ICCardSystemRelatedData update
/*--------------------------------------------------------------------------*/
int trans_UPDATEgsXRec_ICCbL3(void)
{
  U08 *ptr, *pAdd;
  int i, j;
  memset(gsXRec.Trns_ICCbL3,0,150);
  ptr=(U08 *)gsXRec.Trns_ICCbL3;
#if 1
  ptr += PrepNBESubField("5F2A", ptr);
  ptr += PrepNBESubField("5F34", ptr);
  ptr += PrepNBESubField("82", ptr);
  ptr += PrepNBESubField("84", ptr);
  ptr += PrepNBESubField("95", ptr);
  ptr += PrepNBESubField("9A", ptr);
  ptr += PrepNBESubField("9C", ptr);
  ptr += PrepNBESubField("9F02", ptr);
  ptr += PrepNBESubField("9F09", ptr);
  ptr += PrepNBESubField("9F10", ptr);
  ptr += PrepNBESubField("9F1A", ptr);
  ptr += PrepNBESubField("9F1E", ptr);
  ptr += PrepNBESubField("9F26", ptr);
  ptr += PrepNBESubField("9F27", ptr);
  ptr += PrepNBESubField("9F33", ptr);
  ptr += PrepNBESubField("9F34", ptr);
  ptr += PrepNBESubField("9F35", ptr);
  ptr += PrepNBESubField("9F36", ptr);
  ptr += PrepNBESubField("9F37", ptr);
  ptr += PrepNBESubField("9F41", ptr);
  ptr += PrepNBESubField("9F53", ptr);
#else
  ptr += PrepSubField(1,T2n_TrmTrxnCurrencyCode, 0, ptr);
  ptr += PrepSubField(4,T2b_AIP, 2, ptr);
  ptr += PrepSubField(5,Tvv_DFname, 0, ptr);
  ptr += PrepSubField(7,T5b_TrmTVR, 2, ptr);
  ptr += PrepSubField(8,T3n_TrmTrxnDate, 0, ptr);
  ptr += PrepSubField(9,T1n_TrmTrxnType, 0, ptr);
  ptr += PrepSubField(10,T6n_TrmAmountAuthorised, 0, ptr);
  ptr += PrepSubField(11,T6n_TrmAmountOther, 0, ptr);
  ptr += PrepSubField(12,T2b_TrmAppVersionNo, 2, ptr);
  ptr += PrepSubField(13,Tvb_IssuerAppData, 2, ptr);
  ptr += PrepSubField(14,T2n_TrmCountryCode, 0, ptr);
  ptr += PrepSubField(15,T8a_TrmIFD_SerialNo, 1, ptr);
  ptr += PrepSubField(16,T8b_AppCrytogram, 2, ptr);
  ptr += PrepSubField(17,T1b_CryptogramInfoData, 2, ptr);
  ptr += PrepSubField(18,T3b_TrmCapa, 2, ptr);
  ptr += PrepSubField(19,T3b_TrmCVMR, 2, ptr);
  ptr += PrepSubField(20,T1n_TrmType, 0, ptr);
  ptr += PrepSubField(21,T2b_ATC, 2, ptr);
  ptr += PrepSubField(22,T4b_TrmUnpredictableNo, 2, ptr);
  ptr += PrepSubField(23,Tvn_TrmTrxnSequenceCounter, 0, ptr);
  ptr += PrepSubField(24,T1a_TrmTCC, 1, ptr);
  ptr += PrepSubField(25,T1n_PAN_SeqNo, 0, ptr);
#endif
  *ptr = '\0';
//  DBGprt(("gsXRec.ICCbL3: %s",ptr));
  DEBUG_BUF((gsXRec.Trns_ICCbL3, (ptr- (U08 *)&gsXRec.Trns_ICCbL3)));
  return (ptr- gsXRec.Trns_ICCbL3);
}
/*
 *
 */
int app_UPDATEgsXRec_ICCbL3(void)
{
  U08 *ptr, *pAdd;
  int i, j;

  ptr=(U08 *)&gsXRec.ICCbL3;
  memcpy(&gsXRec.TVR,  pgsTrmTVR,  5);
  memcpy(&gsXRec.TSI,  pgsTrmTSI,  2);
  memcpy(&gsXRec.CVMR, pgsTrmCVMR, 3);
  memcpy(&ptr[12], pgsTrmISR, 5);
  *pgbPOSEntryMode=0x51;   // unsuccessful IC read!
  ptr[17] = *pgbPOSEntryMode;
  i = 18;
  if(gbOnLineFlag > 0)
  {
          ptr[i++]=(U08)(gwTrmARC >> 8);
          ptr[i++]=(U08)(gwTrmARC&0xFF);
          ptr[i++]=gbCryptoInfoDat;
      getDnL(pgbAppBuf, giAppBufLen, T2b_ATC, &pAdd);
          memcpy(&ptr[i], pAdd, 2); i += 2;
      getDnL(pgbAppBuf, giAppBufLen, T8b_AppCrytogram, &pAdd);
          memcpy(&ptr[i], pAdd, 8); i += 8;
      if((j=getDnL(pgbAppBuf, giAppBufLen, Tvb_IssuerAppData, &pAdd)) > 0)
      {
          ptr[i++]=(U08)j;
          memcpy(&ptr[i], pAdd, j); i += j;
      }
      else
      {
          ptr[i++]=0x00;
      }
  }
  fnD642Bcd(ptr, (i-2), 3); // 16+14+[upto32] BYTEs

  DEBUG_BUF((gsXRec.ICCbL3, i));

  return(i);

#if 1


#else
  ptr += PrepSubField(1,T2n_TrmTrxnCurrencyCode, 0, ptr);
  ptr += PrepSubField(4,T2b_AIP, 2, ptr);
  ptr += PrepSubField(5,Tvv_DFname, 0, ptr);
  ptr += PrepSubField(7,T5b_TrmTVR, 2, ptr);
  ptr += PrepSubField(8,T3n_TrmTrxnDate, 0, ptr);
  ptr += PrepSubField(9,T1n_TrmTrxnType, 0, ptr);
  ptr += PrepSubField(10,T6n_TrmAmountAuthorised, 0, ptr);
  ptr += PrepSubField(11,T6n_TrmAmountOther, 0, ptr);
  ptr += PrepSubField(12,T2b_TrmAppVersionNo, 2, ptr);
  ptr += PrepSubField(13,Tvb_IssuerAppData, 2, ptr);
  ptr += PrepSubField(14,T2n_TrmCountryCode, 0, ptr);
  ptr += PrepSubField(15,T8a_TrmIFD_SerialNo, 1, ptr);
  ptr += PrepSubField(16,T8b_AppCrytogram, 2, ptr);
  ptr += PrepSubField(17,T1b_CryptogramInfoData, 2, ptr);
  ptr += PrepSubField(18,T3b_TrmCapa, 2, ptr);
  ptr += PrepSubField(19,T3b_TrmCVMR, 2, ptr);
  ptr += PrepSubField(20,T1n_TrmType, 0, ptr);
  ptr += PrepSubField(21,T2b_ATC, 2, ptr);
  ptr += PrepSubField(22,T4b_TrmUnpredictableNo, 2, ptr);
  ptr += PrepSubField(23,Tvn_TrmTrxnSequenceCounter, 0, ptr);
  ptr += PrepSubField(24,T1a_TrmTCC, 1, ptr);
  ptr += PrepSubField(25,T1n_PAN_SeqNo, 0, ptr);
  *ptr = '\0';
//  DBGprt(("gsXRec.ICCbL3: %s",ptr));
  DEBUG_BUF((gsXRec.ICCbL3, (ptr- (U08 *)&gsXRec.ICCbL3)));
  return (ptr- gsXRec.ICCbL3);
#endif
 /* memcpy(&gsXRec.TVR,  pgsTrmTVR,  5);
  memcpy(&gsXRec.TSI,  pgsTrmTSI,  2);
  memcpy(&gsXRec.CVMR, pgsTrmCVMR, 3);
  memcpy(&ptr[12], pgsTrmISR, 5);
  ptr[17] = *pgbPOSEntryMode;
  i = 18;
  if(gbOnLineFlag > 0) {
	ptr[i++]=(U08)(gwTrmARC >> 8);
	ptr[i++]=(U08)(gwTrmARC&0xFF);
	ptr[i++]=gbCryptoInfoDat;
	getDnL(pgbAppBuf, giAppBufLen, T2b_ATC, &pAdd);
	memcpy(&ptr[i], pAdd, 2); i += 2;
	getDnL(pgbAppBuf, giAppBufLen, T8b_AppCrytogram, &pAdd);
	memcpy(&ptr[i], pAdd, 8); i += 8;
	if((j=getDnL(pgbAppBuf, giAppBufLen, Tvb_IssuerAppData, &pAdd)) > 0) {
	  ptr[i++]=(U08)j;
	  memcpy(&ptr[i], pAdd, j); i += j;
	}else {
	  ptr[i++]=0x00;
	}
  }
  fnD642Bcd(ptr, (i-2), 3); // 16+14+[upto32] BYTEs*/

  //return(i);
}

/*
 *
 */
void InitEmvLib()
{
  memcpy(&gsSysInfo, &SysInfoBNK, sizeof(SYSINFOT));  // default SysInfoBNK used!

//  emvref.TraceNr = &DataSave3->ulSysTraceAuditNo;
  emvref.SelectedAID = gsPOS.TrxnRec.SelectedAID;

  emvref.TraceNr = &gsPOS.TraceNr;
  emvref.CardAcceptorMerchantID = gsPOS.CardAcceptorMerchantID;
  emvref.CardAcceptorTerminalID = gsPOS.CardAcceptorTerminalID;
  emvref.TrmAcquirerID = gsPOS.TrmAcquirerID;

//  emvref.TraceNr = DataSave1->ulSysTraceAuditNo;
//  emvref.CardAcceptorMerchantID = DataSave1->udtDeviceConfig.aucMerchantId;
//  emvref.CardAcceptorTerminalID = DataSave1->udtDeviceConfig.aucTermId;

  emvref.TrmAcquirerID = gsSysInfo.TrmAcquirerID;

  emvref.POSSerialNr = gsSysInfo.POSSerialNr;

  emvref.TrmCapa = gsSysInfo.TrmCapa;
  emvref.TrmCapaAdditional = gsSysInfo.TrmCapaAdditional;
  emvref.TrmType = &gsSysInfo.TrmType;
  emvref.TrmTCC = &gsSysInfo.TrmTCC;
  emvref.TrmMCC = gsSysInfo.TrmMCC;
  emvref.TrmForceOnLine = &gsSysInfo.TrmForceOnLine;
  emvref.TrmL2TTAmode = &gsSysInfo.TrmL2TTAmode;
  emvref.SvcCode = (char *)gsPOS.TrxnRec.SvcCode;
  emvref.CardHolderName = (char *)gsPOS.TrxnRec.CardHolderName;

//  SetTrmARC(0x3030, 0x3030, 0x3030, 0x3030, 0x3030, 0x3030);

  SetTrmARC(0x5931, 0x5A31, 0x5932, 0x5A32, 0x5933, 0x5A33);
  SetTrmCAPK((U08 *)gcTrmCAPK);
//  SetTrmExcepList((U08 *)gcTrmExcepList);
//  SetTrmRevocList((U08 *)gcTrmRevocList);
  //  initTrmCurrency();
}

/*
 *
 */
void emv_start(unsigned short headerIdx)
{
  int i;
  unsigned long ulAmount;
  gbRefundFlag=gbSignatureFlag=gbCDArequested=gbReversalFlag=gbOnLineFlag=gbReferralFlag=0;
  BYTE* pAdd;
  int TagLen;
  gbOnLineFlag=1;

  gbTrxnType=1;
  /*--------------------------------------------------------------*/
  DBGprt(("\r\nM>2.5.Enter Amount.\r\n"));
  /*--------------------------------------------------------------*/
  if (Display_InputNoAmountPrompt(headerIdx,S_ENT_AMNT,1,9999999,&ulAmount)== 0)
  {
	memset((char *)Total_Amount, 0, sizeof(Total_Amount));
	goto appEmvTaskEnd;
  }
  sprintf((char *)Total_Amount,"%u",ulAmount);
  DBGprt(("\r\nTotal_Amount: %s\r\n",Total_Amount));

//  ulAmount = 0;
//  if (Display_InputNoAmountPrompt(S_ENT_AMNT,S_ENT_AMNT,1,9999999,&ulAmount)== 0){
//	memset((char *)Total_Amount, 0, sizeof(Total_Amount));
//	goto appEmvTaskEnd;
//  }
//  fnD642Bcd((U08 *)&gbTempBuf,ulAmount,12);
//      //pgbAppBufUpdate(T6n_TrmAmountOther,6,(U08 *)&gbTempBuf);
//  	pgbAppBufUpdate(T6n_TrmAmountOther,6,gbTempBuf);
//      pgbAppBufUpdate(T4b_TrmAmountOther,4,(U08 *)&(U32)gbTempBuf);
//
//	_bcdFromStr(ptr,(u8bits*)Total_Amount,12);
//	ptr[6]=0x00;
// // fnD642Bcd((U08 *)&gbTempBuf,gdwAmount,12);
//  pgbAppBufUpdate(T6n_TrmAmountAuthorised,6,ptr);
//
//  gdwAmount = strToInt(Total_Amount);



//#ifdef MIPS_LITTLE		// sispp 061220 2CM.001.00
//	j = sizeof(gdwAmount);
//	for(i = 0; i < j; i++) gbTempBuf[i] = (gdwAmount >> (8 * (j - 1 - i))) & 0xFF;
//  pgbAppBufUpdate(T4b_TrmAmountAuthorised,6,(U08 *)&gbTempBuf);
//#else
//  pgbAppBufUpdate(T4b_TrmAmountAuthorised,6,(U08 *)&(U32)gdwAmount);
//#endif
//
  //  if (  gbEmvScTaskStep <=2 ){
  gbEmvScTaskStep=2;
  TL_Close();
  if(gsc_IFMinit() < 0)
  {
	DBGprt(("\r\n....Error in gsc_IFMinit()....\r\n"));
	TL_Close();
	STRDISP( LsEMVuseMSR );
	gbEmvScTaskStep = -1; // Set FallBack
	gwModemWait = TIME_BASE0;	// sispp060721
	//                 giStsID = 0x7CF4; pStsTbl = &giStsID;
	return;
  }
  else
  {
	STRDISP( LsEMVuseICC );
	Lprintf(0, 0, (char *)&msg_tbl1[gbLangCode][14]); //"  <EMV-L2 TTA>  "
  }
  //  }
WaitForCard:
  gbEmvScTaskStep=3;
  /*--------------------------------------------------------------*/
  //Smart Card Power Up,
  //if a Non EMV smart card is inserted the reader will
  // send back error 0xA0.
  // We suppose that the right EMV smart card is used.
  /*--------------------------------------------------------------*/
  /* Define Main Card Type as "EMV Normal Card"*/
  if(gsc_SetCardTYPEandSLOT(0x02, 0) != NO_ERR)
  {
	if(!pgsSC_STAT->CARD)
	{
	  DBGprt(("Not Inserted.\r\n"));
	  //	  Lprintf(0, 0, (char *)"Insert Card"); //"  <EMV-L2 TTA>  "
	  STRDISP( LsInsertCard );
	  Wait1MS(500);
	  gsc_Status();	// sispp 070305
	  goto WaitForCard;
	}
	else
	{ // Card error!
	  if(pgsSC_STAT->pwON) gsc_Power(0);
	  TL_Close();
	  // sispp050610 2CM.091.00
	  *pgbPOSEntryMode=0x92;   // unsuccessful IC read!
	  sisPOSEntry = 0x92;

	  if(MSRFlag == 1)
	  {
		DBGprt(("POSEntryMode=0x92\r\n"));	// sispp 070305
		gbEmvScTaskStep = -1; // Set FallBack
		// 					    giStsID = 0x7CF1; pStsTbl = &giStsID;
		// 						enqueue_key(BTKEY_ENTER);
		return;
	  }
	  gwResponseCode=1;
	  //                     STRDISP( LsEMVuseMSR );
	  gbEmvScTaskStep = -1; // Set FallBack
	  //                     giStsID = 0x7CF4; pStsTbl = &giStsID;
	  CARD_ENABLE();
	}
	return;
  }

  app_SeletedAppTrmParameter(); // AfterTTA-L2
  /* if(gsSysInfo.TrmL2TTAmode) {
        ----------------------------------------
         DownLoad EMVL2_TTA_PARAMETER from COM2
        ----------------------------------------
        if(isoHostDownLoadFile() != NO_ERR) return;
    }*/
  gbEmvScTaskStep=11;

  /*--------------------------------------------------------------*/
  DBGprt(("\r\nM>1.Build Candidate List.\r\n"));
  /*--------------------------------------------------------------*/
  if((i=app_BuildCandidateList()) < 0)
  {
	gsc_Status();	// sispp 070305
	DBGprt(("i = %d\r\n", i));	// sispp 070305

	if(i == -3)
	{
	  if(pgsSC_STAT->pwON) gsc_Power(0);
	  TL_Close();
	  // sispp050610 2CM.091.00
	  *pgbPOSEntryMode=0x92;   // unsuccessful IC read!
	  sisPOSEntry = 0x92;
	  DBGprt(("1\r\n"));	// sispp 070305

	  if(MSRFlag == 1)
	  {
		DBGprt(("POSEntryMode=0x92\r\n"));	// sispp 070305
		gbEmvScTaskStep = -1; // Set FallBack
		// 					    giStsID = 0x7CF1; pStsTbl = &giStsID;
		// 						enqueue_key(BTKEY_ENTER);
		return;
	  }
	  DBGprt(("2\r\n"));	// sispp 070305

	  gwResponseCode=1;
	  STRDISP( LsEMVuseMSR );
	  gbEmvScTaskStep = -1; // Set FallBack
	  //                     giStsID = 0x7CF4; pStsTbl = &giStsID;
	  CARD_ENABLE();
	  return;
	}

	//	goto appEmvTaskEnd;
  }

  /*--------------------------------------------------------------*/
  DBGprt(("\r\nM>2.Application Selection.\r\n"));
  /*--------------------------------------------------------------*/
  app_selection:
  gbEmvScTaskStep=12;
  if(app_ApplicationSelection() < 0) goto appEmvTaskEnd;
  app_SeletedAppTrmParameter(); // AfterTTA-L2



  /*--------------------------------------------------------------*/
  DBGprt(("\r\nM>3.Get Processing Options.\r\n"));
  /*--------------------------------------------------------------*/
  gbEmvScTaskStep=13;
  if((i=app_GetProcessingOptions()) < 0) goto appEmvTaskEnd;
  else if(i > 0)
  {
	/*------------------------------------------------*/
	DBGprt(("Geographic Restrictions applied!->"));
	DBGprt(("Conditions of use not satisfied.\r\n"));
	/*------------------------------------------------*/
	DBGprt(("Terminal deletes this application.\r\n"));
	/*------------------------------------------------*/
	//Select other ApplicationSelection()
	goto app_selection;
	return;
  }


  /*--------------------------------------------------------------*/
  DBGprt(("\r\nM>4.Read Application Data.\r\n"));
  /*--------------------------------------------------------------*/
  gbEmvScTaskStep=14;
  if(app_ReadApplicationData() < 0) goto appEmvTaskEnd;
  DEBUG_BUF(((LPBYTE)pgsTrmTVR,5));
  DBGprt(("pgsTrmTVR = %d,%d,%d,%d,%d,%d,%d,%d, \r\n",pgsTrmTVR->B1b2,
		  	  	  	  	  	  	  	  	  	  	  	  pgsTrmTVR->B1b3,
		  	  	  	  	  	  	  	  	  	  	  	  pgsTrmTVR->B1b4 ,
		  	  	  	  	  	  	  	  	  	  	  	  pgsTrmTVR->B1b5 ,
		  	  	  	  	  	  	  	  	  	  	  	  pgsTrmTVR->B1b6 ,
		  	  	  	  	  	  	  	  	  	  	  	  pgsTrmTVR->B1b7,
		  	  	  	  	  	  	  	  	  	  	  	  pgsTrmTVR->B5b2,
		  	  	  	  	  	  	  	  	  	  	  	  pgsTrmTVR->B5b7));
  DEBUG_BUF(((LPBYTE)pgsTrmTVR,5));
  /*--------------------------------------------------------------*/
  DBGprt(("\r\no>5.Offline Data Authentication.\r\n"));
  /*--------------------------------------------------------------*/
  gbEmvScTaskStep=15;
  int ret;
  if((ret = app_OfflineDataAuthentication()) <= -5) goto appEmvTaskEnd;
  DBGprt(("ret = %d \r\n",ret));
  DEBUG_BUF(((LPBYTE)pgsTrmTVR,5));
  DBGprt(("pgsTrmTVR = %d,%d,%d,%d,%d,%d,%d,%d, \r\n",pgsTrmTVR->B1b2,
		  	  	  	  	  	  	  	  	  	  	  	  pgsTrmTVR->B1b3,
		  	  	  	  	  	  	  	  	  	  	  	  pgsTrmTVR->B1b4 ,
		  	  	  	  	  	  	  	  	  	  	  	  pgsTrmTVR->B1b5 ,
		  	  	  	  	  	  	  	  	  	  	  	  pgsTrmTVR->B1b6 ,
		  	  	  	  	  	  	  	  	  	  	  	  pgsTrmTVR->B1b7,
		  	  	  	  	  	  	  	  	  	  	  	  pgsTrmTVR->B5b2,
		  	  	  	  	  	  	  	  	  	  	  	  pgsTrmTVR->B5b7));
  DEBUG_BUF(((LPBYTE)pgsTrmTVR,5));
  /*--------------------------------------------------------------*/
  DBGprt(("\r\nM>6.Processing Restrictions.\r\n"));
  /*--------------------------------------------------------------*/
  gbEmvScTaskStep=16;
  app_ProcessRestrictions();
  /*--------------------------------------------------------------*/
  DBGprt(("\r\nM>7.Cardholder Verification.\r\n"));
  /*--------------------------------------------------------------*/
  gbEmvScTaskStep=17;
  if(app_CVMprocess() <= -5) goto appEmvTaskEnd;
  DBGprt(("pgsTrmTVR->B3b3 = %d \r\n",pgsTrmTVR->B3b3));
  if(pgsTrmTVR->B3b3==1)
  { // 1 = Online PIN entered
	goto pre_Authorizarion;
  }
  else
  {
	goto TermRiskMngmnt;
  }
  return;

  pre_Authorizarion:
  gbEmvScTaskStep=19;
  /*--------------------------------------------------------------*/
  // IF any pre_Authorizarion need, process at this step!
  /*--------------------------------------------------------------*/

  TermRiskMngmnt:
  /*--------------------------------------------------------------*/
  DBGprt(("\r\nM>8.Terminal Risk Management.\r\n"));
  /*--------------------------------------------------------------*/
  gbEmvScTaskStep=25;
  if(app_TRMprocess() < 0) goto appEmvTaskEnd;

  /*--------------------------------------------------------------*/
  DBGprt(("\r\nM>9.Terminal Action Analysis.\r\n"));
  /*--------------------------------------------------------------*/
  gbEmvScTaskStep=26;
  if(app_TAAprocess() < 0) goto appEmvTaskEnd;

  /*--------------------------------------------------------------*/
  DBGprt(("\r\nM>A.Card Action Analysis.\r\n"));
  /*--------------------------------------------------------------*/
  gbEmvScTaskStep=7;
  if(app_CAAprocess() < 0) goto appEmvTaskEnd;


  if (gbForceOffline == 1)	//heba
  {
	  Rprintf("OFFFLINEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\r\n");
	  OfflineTXN((char*)pgbAppBuf, giAppBufLen);

		ghResponseCode = 0x3030;
		gwResponseCode=0x3030; // Always approve
		gsXRec.ResponseCode=0x3030;
//        vidSaveTansDetails(); 	//Save Trans
//        vidClearReversal();
//        ucPrntCustomerRct();	//Print Receipt
//
//        //Ask to print Merchant Copy
//        iChosePrintMerchCopy(headerIdx);
        gbEmvScTaskStep = 102;
	  goto appEmvTaskEnd;
//
  }
OnLineProcess:
  /*--------------------------------------------------------------*/
  DBGprt(("\r\no>B. OnLineProcess.\r\n"));
  /*--------------------------------------------------------------*/
  gbEmvScTaskStep=28;
  //  gbModemTaskStep = -1; // no operation in MODEM_close();
  /*--------------------------------------------------------------*/
  /*--------------------------------------------------------------*/
  gbEmvScTaskStep=29;
//  DBGprt(("gbRefundFlag = %d\r\n",gbRefundFlag));
//  DEBUG_BUF(((LPBYTE)pgsTrmTVR,5));
//  DBGprt(("pgsTrmTVR = %d,%d,%d,%d,%d,%d,%d,%d, \r\n",pgsTrmTVR->B1b2, pgsTrmTVR->B1b3, pgsTrmTVR->B1b4 ,pgsTrmTVR->B1b5 , pgsTrmTVR->B1b6 , pgsTrmTVR->B1b7, pgsTrmTVR->B5b2,pgsTrmTVR->B5b7));
//  DEBUG_BUF(((LPBYTE)pgsTrmTVR,5));
//  DBGprt(("gwTrmARC  = %d\r\n",gwTrmARC));
//  DBGprt(("gbOnLineFlag  = %d\r\n",gbOnLineFlag));
//  DBGprt(("gwResponseCode  = %d ghResponseCode  = %d\r\n",gwResponseCode, ghResponseCode));
//  TagLen = getDnL(pgbAppBuf, giAppBufLen,T2a_ARC , &pAdd);
//  DEBUG_BUF(((LPBYTE)pAdd,TagLen));



  if(app_OnLineProcess() < 0) goto appEmvTaskEnd;
gbEmvScTaskStep=100;
  if(gbOnLineFlag)
  {
	setDEBUG_COMport(COM_PORT0);
	TL_Open(0, gem_BaudRate);
  }

  /*--------------------------------------------------------------*/
  DBGprt(("\r\nM>C.Completion.\r\n"));
  /*--------------------------------------------------------------*/
  gbEmvScTaskStep=101;
	DBGprt(("gbRefundFlag Completion = %d\r\n",gbRefundFlag));
	DBGprt(("gwTrmARC Completion = %d\r\n",gwTrmARC));
	DBGprt(("gbOnLineFlag  Completion= %d\r\n",gbOnLineFlag));
	DEBUG_BUF(((LPBYTE)pgsTrmTVR,5));
	DBGprt(("gwResponseCode  = %d ghResponseCode  = %d\r\n",gwResponseCode, ghResponseCode));

	TagLen = getDnL(pgbAppBuf, giAppBufLen,T2a_ARC , &pAdd);
	DEBUG_BUF(((LPBYTE)pAdd,TagLen));

	if(pgbAppBuf!=NULL)
	{
		DBGprt(("\n....Check & see pgbAppBuf[%d]....\n",giAppBufLen));
		DEBUG_DOL((pgbAppBuf, giAppBufLen));
//		        pgbAppBuf=NULL; giAppBufLen=0;
	}

	if(app_Completion()< 0) 	DBGprt(("Completion Failed\r\n"));
	DBGprt(("gwTrmARC after Completion = %d\r\n",gwTrmARC));
	DBGprt(("gbOnLineFlag  after Completion= %d\r\n",gbOnLineFlag));
	DBGprt(("gwResponseCode  = %d ghResponseCode  = %d\r\n",gwResponseCode, ghResponseCode));

	TagLen = getDnL(pgbAppBuf, giAppBufLen,T2a_ARC , &pAdd);
	DEBUG_BUF(((LPBYTE)pAdd,TagLen));


  if(gbRefundFlag)
  {
	DBGprt(("Refund processing required.\r\n"));
	//------------------------------------------------------
	// CETECOM_FAIL [2CI.032.00]
	//------------------------------------------------------
	STRDISP( LsTrxnResult );
	//	isoDispTrxnResults(gu32ProcessingCode, 0x3531, 2000); // DECLINE, Wait 2sec
	//------------------------------------------------------
	/* Need to add fallowing... [2CA.054.00]genac2 declined
	  Values for Cryptogram Information Data, ATC, Application Cryptogram,
	  Issuer Application Data managed by the terminal and included in
	  authorization or financial requests or batch data capture shall be in
	  accordance with values sent back by the LT              */
	//------------------------------------------------------
	// PCodeRefund : It is system hidden process!
	Lprintf(2, 0, (char *)&msg_tbl1[gbLangCode][141]);
	WaitKeyInput();
	//------------------------------------------------------
	gbOnLineFlag=1; // OnLine process for Refund!
	/*	isoRequestMsgBNK(PCodeRefund);
	if(gsSysInfo.TrmL2TTAmode) {
	  isoHostSimRspMsg();
	  gbEmvScTaskStep=124; // Even error process this until completion
	}else*/
	{
//	  goto OnLineProcess;
	}
	  gbEmvScTaskStep=124;
  }

  else
  { // Normal End
	//	isoPutTrxnReports();


      if( gsXRec.ResponseCode==0x3030)
      {
          vidSaveTansDetails(); 	//Save Trans
          vidClearReversal();
          ucPrntCustomerRct();	//Print Receipt

          //Ask to print Merchant Copy
          iChosePrintMerchCopy(headerIdx);
      }
      else
      {
    	  ucPrntFailedRct();
      }

  }

  if(  gbEmvScTaskStep>0)
  {
	  gbEmvScTaskStep++;
  }
  /*--------------------------------------------------------------*/
  DBGprt(("\r\nappEmvTaskEnd.\r\n"));
  /*--------------------------------------------------------------*/
  /*--------------------------------------------------------------*/
  appEmvTaskEnd:
  /*--------------------------------------------------------------*/
  if(giDEBUG_COM != COM_PORT1)
  {
	DBGprt(("\r\n giDEBUG_COM != COM_PORT1 \r\n"));
	setDEBUG_COMport(COM_PORT0);
	TL_Open(0, gem_BaudRate);
  }
  DBGprt(("\n pgsSC_STAT->pwON=%d\n",pgsSC_STAT->pwON));
  DBGprt(("\n gem_RS232open=%d\n",gem_RS232open));

  //Smart Card Power Down
  if(pgsSC_STAT->pwON)
  {
	  gsc_Power(0);
	  DBGprt(("\r\n Smart Card Power Down \r\n"));
  }
  // Close Serial Port
  if(gem_RS232open)
  {
	  TL_Close();
	  DBGprt(("\r\n Close Serial Port \r\n"));
  }

  if(pgbAppBuf!=NULL)
  {
	DBGprt(("\r\n....Check & see pgbAppBuf[%d]....\r\n",giAppBufLen));
	DEBUG_DOL((pgbAppBuf, giAppBufLen));
	pgbAppBuf=NULL; giAppBufLen=0;
  }
  if(pgbRspBuf != NULL)
  {
	pgbRspBuf=NULL; giRspBufLen=0;
  }

  DBGprt(("\ngbEmvScTaskStep=%d\n",gbEmvScTaskStep));
  DBGprt(("\n gwSW1SW2=%X\n",gwSW1SW2));

  /**/
  if(gbEmvScTaskStep == 102)
  { // Normal End
	gbEmvScTaskStep = 0;
  }
  else
  {
	  ucPrntFailedRct();

	*pgbPOSEntryMode=0x92;   // unsuccessful IC read!
	/* sispp 070305
		if(MSRFlag) {
			DBGprt(("POSEntryMode=0x92\r\n"));	// sispp040818 2CM.091.00
		}
	 */
	/*       isoPutTrxnReports();
       PrintTask();*/
	if(gbEmvScTaskStep == 127)
	{ // CANCEL key operation
	  gbEmvScTaskStep = 0;
	}
	else
	  if(gwSW1SW2 != 0x6A81)
	  {  // Application not blocked!
		gbEmvScTaskStep = -1; // Set FallBack
	  }
	  else
	  { // Refund
		gbEmvScTaskStep = 0;
	  }
  }

  gwModemWait = TIME_BASE0;	// sispp060721

  /*******************************************************************************/
  InitPinpadDrv(3);
  //	gSysTimer = TIME_BASE0;
  //  MODEM_close();

  MSRFlag = 0;		// sispp040830 2CM.091.00
  pXRec=NULL;
  gbSignatureFlag=1;
  //  gbRS232TaskStep = 0;
  gbOnLineFlag = gbReferralFlag = 0;
  ghResponseCode = gwResponseCode = gwTrmARC = 0;
  if(!gbReversalFlag)
  {
	gu32ProcessingCode = gu32ProcessingCodeNext = 0L;
	memset((char *)&CardNo2, 0, sizeof(CardNo2));
	memset((char *)&gbPinBlock, 0, sizeof(gbPinBlock));
	memset((char *)&gsPOS.TrxnRec, 0, sizeof(TTrxnRec));
  }
#ifdef BITEL_IC5000_RFM
  CONTROL_POWER_SAVING_STATUS(3,OFF); // SMART CARD
#endif
  LCD_BLINKING(CURSOR_OFF, 0, 0, OFF);

  Total_Amount[0]=0; // Clear Amount
  if(gbEmvScTaskStep > 0)
  {
	gbEmvScTaskStep=127; //call appEmvTaskEnd
  }
  else if(giTrxnNOnFlag > 128)
  {
	giTrxnNOnFlag=0;
	STRDISP( 83 );
	return;
  }
  //  getRtcDateTime();
  //  dispDateTime(1);

  CARD_ENABLE();
  giTrmDATAsize = 0;
  //    if(giPassFlag == -1) enqueue_key(BTKEY_F6SETT);
  /*******************************************************************************/


  //   giStsID = 0x7CF4; pStsTbl = &giStsID;

  //  if(gsSysInfo.TrmL2TTAmode) {
  //	setDEBUG_COMport(COM_PORT0);
  //	//       isoHostTxDsend("REQX");
  //	setDEBUG_COMport(COM_PORT0);
  //  }

  if(pgsTrmTVR != NULL) pgsTrmTVR=NULL;
  if(pgsTrmTSI != NULL) pgsTrmTSI=NULL;

  return;
}




