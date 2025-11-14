/******************************************************************************/
/* COMPANY  :  Bitel EMEA Co.                                                 */
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

/******************************************************************************/
/* INCLUDE FILES                                                              */
/******************************************************************************/
#include "AppGlobal.h"
#include "./resources/Msgs.h"
#include "CurrencyCodes.h"
#include "SndRcvIso8583.h"
#include "TransSndRcv.h"
#include "UTILS.h"
//#include "./gsoap/ITransactionProcessor.nsmap"
//#include "./gsoap/VACP_C_SoapStub.h"
#include "./gsoap/CustomBinding_USCOREImPOSService.nsmap"
#include "gsoap/BM_C_SoapStub.h"

#include "emv/EMV_APP.h"
#include "emv/EMV.h"

const char server[] = "http://websrv.cs.fsu.edu/~engelen/calcserver.cgi";

char Temp[8];
char LTime[13];
char CapDate[5];
char Stan[7];
char MTI[5];
char Amount[13];
char TempBuff1[1000];
char Track2[50];

//Settlement
char NetAMount[17];
long saleAmount=0;
ulong CreditAmount=0;
ulong DebitAmount=0;

ulong DebitNo=0;
ulong CreditNo=0;

ulong Defutlt=0;

//Batch Upload
int HostID=HOSTID;
char DataRecord[999];

#ifndef ZM
#define ZM(stc) memset(&stc, 0, sizeof(stc))
#endif
	//#include "Bread.h"

/******************************************************************************/
/* TYPES                                                                      */
/******************************************************************************/
/******************************************************************************/
/* CONSTANTS, MACROS                                                          */
/******************************************************************************/
#define DEV_DEBUG 1


/******************************************************************************/
/* PRIVATE VARIABLES                                                          */
/******************************************************************************/
static ushort	usP57BufLen = 0;
static ushort	usP63BufLen = 0;
static uchar    aucP57Buff[300];
static uchar    aucP63Buff[100];
static uchar    aucP57RcvBuff[1024];
static uchar    aucP63RcvBuff[1024];
/******************************************************************************/
/* PRIVATE FUNCTIONS PROTOTYPES                                               */
/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/
uchar ucSetP57(uchar ucSubFldID, uchar* pucFldData,ushort usSize)
{
  ulong ulSubFldDataLen = strlen(pucFldData);
  //DBGPrt_m("ulSubFldDataLen:%d ucSubFldID:%d\r\n",(MAX(ulSubFldDataLen,usSize))+2,ucSubFldID);
  long2asci(&(aucP57Buff[usP57BufLen]),3,(MAX(ulSubFldDataLen,usSize))+2);
  long2asci(&(aucP57Buff[usP57BufLen+3]),2,(ucSubFldID));
  //DBGPrt_m("aucP57Buff:%s \r\n",aucP57Buff);
  memcpy( &(aucP57Buff[usP57BufLen+5]), pucFldData,(ulSubFldDataLen));
  usP57BufLen += (MAX(ulSubFldDataLen,usSize))+5 ;
}
/*
 *
 */
uchar ucSetP63(uchar ucSubFldID, uchar* pucFldData,ushort usSize)
{
  ulong ulSubFldDataLen = strlen(pucFldData);
  DBGPrt_m("ulSubFldDataLen:%d ucSubFldID:%d\r\n",(MAX(ulSubFldDataLen,usSize))+2,ucSubFldID);
  DBGPrt_m("pucFldData:%s\r\n",pucFldData);
  int2LLL(&(aucP63Buff[usP63BufLen]),((MAX(ulSubFldDataLen,usSize))+2));
  long2asci(&(aucP63Buff[usP63BufLen+2]),2,(ucSubFldID));
  memcpy( &(aucP63Buff[usP63BufLen+4]), pucFldData,(ulSubFldDataLen));
  DBGPrt_m("aucP63Buff:%s \r\n",aucP63Buff);
  usP63BufLen += (MAX(ulSubFldDataLen,usSize))+4 ;
}
/*
 *
 */
uchar ucParseP57(uchar* pucP57, ushort usP57Len,uchar ucFldId)
{
  ushort usIdx;
  ushort usP57TempLen = usP57Len;
  ushort usLenSubFld = 0;
  ushort usIDSubFld;

  memset(aucP57RcvBuff,0,sizeof(aucP57RcvBuff));
  for (usIdx = 0; usIdx < usP57Len; (usIdx += (usLenSubFld+3)))
  {
    usLenSubFld = (ushort)asc_long(&pucP57[usIdx],3);
    usIDSubFld = (ushort)asc_long(&pucP57[usIdx+3],2);
    DBGPrt_m("usLenSubFld:%d usIDSubFld:%d\r\n",usLenSubFld,usIDSubFld);
    if (usIDSubFld == ucFldId) {
      memcpy(aucP57RcvBuff,&pucP57[usIdx+5],(usLenSubFld-2));
      DBGPrt_m("aucP57RcvBuff%s\r\n",aucP57RcvBuff);
      return TRUE;
    }
  }
  return FALSE;
}

/*
 *
 */
uchar ucParseP63(uchar* pucP63, ushort usP63Len,uchar ucFldId)
{
  ushort usIdx;
  ushort usP57TempLen = usP63Len;
  ushort usLen = 0;
  ushort usID;

  memset(aucP63RcvBuff,0,sizeof(aucP63RcvBuff));
  for (usIdx = 0; usIdx < usP63Len; (usIdx += (usLen+2)))
  {
    usLen = (ushort)LLL2int(&pucP63[usIdx]);
    usID = (ushort)asc_long(&pucP63[usIdx+2],2);
    DBGPrt_m("usLen:%d usID:%d\r\n",usLen,usID);
    if (usID == ucFldId) {
      memcpy(aucP63RcvBuff,&pucP63[usIdx+4],(usLen-2));
      DBGPrt_m("aucP63RcvBuff%s\r\n",aucP63RcvBuff);
      return TRUE;
    }
  }
  return FALSE;
}
/*
 *
 */
void vidIncSTAN(void)
{
  DataSave1->ulSysTraceAuditNo++;
  if (DataSave1->ulSysTraceAuditNo < 0 || DataSave1->ulSysTraceAuditNo > 9999999)
    DataSave1->ulSysTraceAuditNo = 1;
}

/*
 *
 */
uchar ChkSendReversal(void)
{
  uchar ucResult;
  uchar ucTrialIdx;
  if (DataSave1->udtRevesalData.bIsReversalReq == TRUE){
    for (ucTrialIdx = 0; ucTrialIdx < REVSL_TRIALS; ucTrialIdx++) {
      ucResult= ucReversal(); 
      if ((ucResult==SUCCESS) || (ucResult==ERR_HOSTCODE)){
        //if (DataSave1->udtRevesalData.ulPCode != PCODE_VOID_SALE) {
        //  vidIncSTAN();
        //}
        break;
      }
    }
    if (ucTrialIdx == REVSL_TRIALS) {
      DisplayMsgBox(S_RVRSL, S_ATTMP_END,1000, 0);
      ucResult = ERR_REV_TRIALS;
    }
  }
  return ucResult;
}

/*
 *
 */
uchar ucCmnBitsSet(void)
{
  unsigned char ucResult;
  unsigned char aucBuf[1024];
  unsigned char aucTempBuf[100];
  ulong ullen;

  if ( ChkSendReversal() == ERR_REV_TRIALS) {
    return ERR_REV_TRIALS;
  }

  ISO8583_Clear();

  if (DataSave1->udtTransDetail.udtMSRCard.bIsValidData) {
    if (DataSave1->udtTransDetail.udtTansType != VOID) {
      // Field 2 (Primary Account Number: n..19) - Mandatory
      memset(aucBuf,0,sizeof(aucBuf));
      ullen = strlen(DataSave1->udtTransDetail.PAN);
      memcpy(aucBuf,DataSave1->udtTransDetail.PAN,ullen);
      ISO8583_SetBitValue(2,aucBuf,ullen);
    }
  }

  // Field 3 (Processing Code: n 6) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  sprintf(aucTempBuf, "%06lX", DataSave1->udtTransDetail.ulPCode);
  memcpy(aucBuf,aucTempBuf,strlen(aucTempBuf));
  ISO8583_SetBitValue(3,aucBuf,6);

  if (strlen(DataSave1->udtTransDetail.aucAmount) > 0) {
    // Field 4 (Transaction Amount: n 12) - Mandatory
    memset(aucBuf,0,sizeof(aucBuf));
    memcpy(&aucBuf[12-strlen(DataSave1->udtTransDetail.aucAmount)],DataSave1->udtTransDetail.aucAmount,strlen(DataSave1->udtTransDetail.aucAmount));
    ISO8583_SetBitValue(4,aucBuf,12);
  }

  // Field11 (System Trace Audit Number: n 6) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  if (DataSave1->udtTransDetail.udtTansType == VOID) {
    sprintf(aucTempBuf, "%06d", DataSave1->udtTransDetail.ulSysTraceAuditNo); 
  } else {
    //vidIncSTAN();
    sprintf(aucTempBuf, "%06d", DataSave1->ulSysTraceAuditNo);
  }
  memcpy(aucBuf,aucTempBuf,strlen(aucTempBuf));
  ISO8583_SetBitValue(11,aucBuf,6);

  // Field12 (Local (Terminal) Transaction Time: n 6) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  memcpy(aucBuf,DataSave1->udtTransDetail.aucTransTime,6);
  ISO8583_SetBitValue(12,aucBuf,6);

  // Field13 (Local (Terminal) Transaction Date: n 4) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  memcpy(aucBuf,&DataSave1->udtTransDetail.aucTransDate[4],4);
  ISO8583_SetBitValue(13,aucBuf,4);

  if (DataSave1->udtTransDetail.udtMSRCard.bIsValidData){
    if ((DataSave1->udtTransDetail.udtTansType == VOID)||
        (DataSave1->udtTransDetail.udtTansType == ACC_TRANSFER)||
        (DataSave1->udtTransDetail.udtTansType == BATCH) ||
        ((DataSave1->udtTransDetail.usPOSEntryMode & PEM_PAN_ENTRY_MASK) == PEM_MANUAL))
    {
      //Field14 (Card expiration date: n 4) - Mandatory
      memset(aucBuf,0,sizeof(aucBuf));
      memcpy(aucBuf,DataSave1->udtTransDetail.ExpiryDate,4);
      ISO8583_SetBitValue(14,aucBuf,4);
    }
  }

  // Field22 (POS Entry Mode: n 3) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  sprintf(aucTempBuf, "%04lX", DataSave1->udtTransDetail.usPOSEntryMode);
  memcpy(aucBuf,aucTempBuf,4);
  ISO8583_SetBitValue(22,aucBuf,4);

  // Field23 (card member number (MBR): n 3) - Optional
  //memset(aucBuf,0,sizeof(aucBuf));
  //memcpy(aucBuf,,);
  //ISO8583_SetBitValue(23,aucBuf,);

  // Field25 (POS Condition Code: n 2) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  sprintf(aucTempBuf, "%02X", DataSave1->udtTransDetail.ucPosCondCode);
  memcpy(aucBuf,aucTempBuf,2);
  ISO8583_SetBitValue(25,aucBuf,2);

  DBGPrt_m("usPOSEntryMode %02x\r\n",(DataSave1->udtTransDetail.usPOSEntryMode& PEM_PAN_ENTRY_MASK));
  if ((DataSave1->udtTransDetail.udtMSRCard.bIsValidData) &&
      ((DataSave1->udtTransDetail.usPOSEntryMode & PEM_PAN_ENTRY_MASK) != PEM_MANUAL)){
    // Field35 (Track 2 Data: z..37) - Mandatory
    memset(aucBuf,0,sizeof(aucBuf));
    ullen = strlen(DataSave1->udtTransDetail.udtMSRCard.acTrack2);
    memcpy(aucBuf,DataSave1->udtTransDetail.udtMSRCard.acTrack2,ullen);
    DBGPrt_m("ullen :%d acTrack2[1] :%s usPOSEntryMode %02x\r\n",ullen, DataSave1->udtTransDetail.udtMSRCard.acTrack2,
            (DataSave1->udtTransDetail.usPOSEntryMode));
    ISO8583_SetBitValue(35,aucBuf,ullen);
  }

  if (DataSave1->udtTransDetail.udtTansType == VOID) {
    // Field37 (Retrieval Reference Number: ans12) - Mandatory
    memset(aucBuf,0,sizeof(aucBuf));
    memcpy(aucBuf,DataSave1->udtTransDetail.aucRtrvalRefNo,12);
    ISO8583_SetBitValue(37,aucBuf,12);
  }

  // Field41 (Card Acceptor Terminal Identification: ans8) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  ullen = strlen(DataSave1->udtDeviceConfig.aucTermId);
  memcpy(aucBuf,DataSave1->udtDeviceConfig.aucTermId,ullen);
  ISO8583_SetBitValue(41,aucBuf,8);

  // Field42 (Card Acceptor Identification Code: ans15) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  ullen = strlen(DataSave1->udtDeviceConfig.aucMerchantId);
  memcpy(aucBuf,DataSave1->udtDeviceConfig.aucMerchantId,ullen);
  memset(&aucBuf[ullen],' ',15 - ullen);
  ISO8583_SetBitValue(42,aucBuf,15);

  
  // Field49 (Transaction Currency Code: an3) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  memcpy(aucBuf,udtCurrencyTable[SDG].ucCurrencyCode,3);
  ISO8583_SetBitValue(49,aucBuf,3);

  if ((DataSave1->udtTransDetail.udtMSRCard.bIsValidData) &&
      ((DataSave1->udtTransDetail.usPOSEntryMode & PEM_PIN_ENTRY_MASK) == PEM_PIN_ENTRY_CAPABILITY)){
    if (DataSave1->udtTransDetail.udtTansType != VOID) {
      // Field52 (Personal Identification Number PIN: b8) - Mandatory
      memset(aucBuf,0,sizeof(aucBuf));
      memcpy(aucBuf,DataSave1->udtTransDetail.udtMSRCard.CiphertextPinBlock,8);
      ISO8583_SetBitValue(52,aucBuf,8);
    }
  }

  return TRUE;
}
/*
 *
 */
uchar ucCmnReducedBitsSet(void)
{
  unsigned char aucBuf[1024], aucTempBuf[100];
  ulong ullen;

  if ( ChkSendReversal() == ERR_REV_TRIALS) {
    return ERR_REV_TRIALS;
  }

  ISO8583_Clear();

  // Field 3 (Processing Code: n 6) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  sprintf(aucTempBuf, "%06lX", DataSave1->udtTransDetail.ulPCode);
  memcpy(aucBuf,aucTempBuf,strlen(aucTempBuf));
  ISO8583_SetBitValue(3,aucBuf,6);

  // Field11 (System Trace Audit Number: n 6) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  //vidIncSTAN();
  sprintf(aucTempBuf, "%06d", DataSave1->ulSysTraceAuditNo);
  memcpy(aucBuf,aucTempBuf,strlen(aucTempBuf));
  ISO8583_SetBitValue(11,aucBuf,6);

  // Field12 (Local (Terminal) Transaction Time: n 6) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  memcpy(aucBuf,DataSave1->udtTransDetail.aucTransTime,6);
  ISO8583_SetBitValue(12,aucBuf,6);

  // Field13 (Local (Terminal) Transaction Date: n 4) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  memcpy(aucBuf,&DataSave1->udtTransDetail.aucTransDate[4],4);
  ISO8583_SetBitValue(13,aucBuf,4);

  // Field41 (Card Acceptor Terminal Identification: ans8) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  ullen = strlen(DataSave1->udtDeviceConfig.aucTermId);
  memcpy(aucBuf,DataSave1->udtDeviceConfig.aucTermId,ullen);
  ISO8583_SetBitValue(41,aucBuf,8);
}

/*
 *
 */
uchar ucGetSetAuthRefTansID(void)
{
  unsigned char aucBuf[1024];
  int iLen = 0;

  if (ISO8583_GetBitValue(37, aucBuf, &iLen, sizeof(aucBuf)) == SUCCESS){
    memset(DataSave1->udtTransDetail.aucRtrvalRefNo,0,sizeof(DataSave1->udtTransDetail.aucRtrvalRefNo));
    memcpy(DataSave1->udtTransDetail.aucRtrvalRefNo,aucBuf,iLen);
  }
  if (ISO8583_GetBitValue(38, aucBuf, &iLen, sizeof(aucBuf)) == SUCCESS){
    memset(DataSave1->udtTransDetail.aucAuthIdResponse,0,sizeof(DataSave1->udtTransDetail.aucAuthIdResponse));
    memcpy(DataSave1->udtTransDetail.aucAuthIdResponse,aucBuf,iLen);
  }
  if (ISO8583_GetBitValue(57, aucBuf, &iLen, sizeof(aucBuf)) == SUCCESS){
    DBGPrt_m("aucP57:%s\r\n",aucBuf);

    if (ucParseP57(aucBuf,iLen,37)== TRUE){
      memset(DataSave1->udtTransDetail.aucExtRtrvalRefNo,0,sizeof(DataSave1->udtTransDetail.aucExtRtrvalRefNo));
      memcpy(DataSave1->udtTransDetail.aucExtRtrvalRefNo,aucP57RcvBuff,strlen(aucP57RcvBuff));
      DBGPrt_m("Fld37:%s\r\n",aucP57RcvBuff);
    } else {
      DBGPrt_m("Parse Error");
    }
  }
  return TRUE;
}
/******************************************************************************/
/* EXPORTED FUNCTIONS                                                         */
/******************************************************************************/
uchar ucPurchSndRcv(void)
{
  unsigned char aucBuf[1024], aucTempBuf[100];
  unsigned char ucResult;
  int iLen = 0;
  uchar ucIdx;

  if (ucCmnBitsSet() == ERR_REV_TRIALS) {
    return ERR_REV_TRIALS;
  }

  ISO8583_SetMsgID(MTYPE_FINANCIAL_TRXN_REQUEST);
  ISO8583_SetTPDU("\x60\x00\x00\x00\0x00");
  ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                          (unsigned char *)&DataSave0->SaveISO8583Data);
  ucResult = COMMS_TransOnline();
  if((ucResult==SUCCESS) || (ucResult==ERR_HOSTCODE)){

    ISO8583_GetBitValue(37, aucBuf, &iLen, sizeof(aucBuf));
    //memset(DataSave1->udtTransDetail.aucRtrvalRefNo,0,sizeof(DataSave1->udtTransDetail.aucRtrvalRefNo));
    memcpy(DataSave1->udtTransDetail.aucRtrvalRefNo,aucBuf,iLen);
    
    ISO8583_GetBitValue(38, aucBuf, &iLen, sizeof(aucBuf));
   // memset(DataSave1->udtTransDetail.aucExtRtrvalRefNo,0,sizeof(DataSave1->udtTransDetail.aucExtRtrvalRefNo));
    memcpy(DataSave1->udtTransDetail.aucAuthIdResponse,aucBuf,iLen);

    ISO8583_GetBitValue(57, aucBuf, &iLen, sizeof(aucBuf));
    DBGPrt_m("aucP57:%s\r\n",aucBuf);
    if (ucParseP57(aucBuf,iLen,37)== TRUE){
      memcpy(DataSave1->udtTransDetail.aucExtRtrvalRefNo,aucP57RcvBuff,strlen(aucP57RcvBuff));
      DBGPrt_m("Fld37:%s\r\n",aucP57RcvBuff);
    } else {
      DBGPrt_m("Parse Error");
    }
    if (ucParseP57(aucBuf,iLen,31)== TRUE){
      memcpy(DataSave1->udtTransDetail.aucFee,aucP57RcvBuff,strlen(aucP57RcvBuff));
      DBGPrt_m("Fld31:%s\r\n",aucP57RcvBuff);
    } else {
      DBGPrt_m("Parse Error");
    }
  } else{
    ChkSendReversal();
  }
  return ucResult;
}
/*
 *
 */
uchar ucBillInqSndRcv(uchar ucVenIdx)
{
  uchar aucBuf[1024], aucTempBuf[100];
  uchar ucResult;
  int iLen;
  const char delim[2] = "\x1c";
  char *token;

  if (ucCmnBitsSet() == ERR_REV_TRIALS)
  {
    return ERR_REV_TRIALS;
  }
  

  // Field57 (Additional Data: ans..999) - Mandatory
  // Sub-Field 57.62 (Card PAN of the transfer recipient : n..19)
  memset(aucP57Buff,0,sizeof(aucP57Buff));
  usP57BufLen = 0;
  iLen = strlen(DataSave1->udtDeviceConfig.audtVedInstitCode[ucVenIdx][0].aucVendCode);
  ucSetP57(10,DataSave1->udtDeviceConfig.audtVedInstitCode[ucVenIdx][0].aucVendCode,iLen);

  iLen = strlen(DataSave1->udtDeviceConfig.audtVedInstitCode[ucVenIdx][0].aucInstitCode);
  ucSetP57(11,DataSave1->udtDeviceConfig.audtVedInstitCode[ucVenIdx][0].aucInstitCode,iLen);

  iLen = strlen(DataSave1->udtTransDetail.aucAccountNo);
  ucSetP57(12,DataSave1->udtTransDetail.aucAccountNo,iLen);

  memset(aucBuf,0,sizeof(aucBuf));
  memcpy(aucBuf,aucP57Buff,usP57BufLen);
  ISO8583_SetBitValue(57,aucBuf,usP57BufLen);

  ISO8583_SetMsgID(MTYPE_FINANCIAL_TRXN_REQUEST);
  ISO8583_SetTPDU("\x60\x00\x00\x00\0x00");
  ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                          (unsigned char *)&DataSave0->SaveISO8583Data);

  ucResult = COMMS_TransOnline();
  memset(&DataSave1->udtRevesalData,0,sizeof(strRevesalData));
  if((ucResult==SUCCESS) || (ucResult==ERR_HOSTCODE)){
    ucGetSetAuthRefTansID();
    if (ISO8583_GetBitValue(63, aucBuf, &iLen, sizeof(aucBuf)) == SUCCESS){
      DBGPrt_m("aucP63:%s\r\n",aucBuf);
      if (ucParseP63(aucBuf,iLen,29)== TRUE){
        memset((void*)&udtBillInqInfo,0,sizeof(udtBillInqInfo));
     /* get the first token */
        if ((token = strtok(aucP63RcvBuff, delim)) != NULL){
          if (DataSave1->udtTransDetail.udtVendType ==ZAIN) {
            strcpy(udtBillInqInfo.aucContractNumber,token);
          }
          if ((token = strtok(NULL, delim)) != NULL){
            if (DataSave1->udtTransDetail.udtVendType ==ZAIN) {
              strcpy(udtBillInqInfo.aucBillamount,token);
            } else {
              strcpy(udtBillInqInfo.aucContractNumber,token);
            }
            if ((token = strtok(NULL, delim)) != NULL){
              if (DataSave1->udtTransDetail.udtVendType ==ZAIN) {
                strcpy(udtBillInqInfo.aucUNBillAmount, token); 
              } else {
                strcpy(udtBillInqInfo.aucBillamount,token);
              } 
              if ((token = strtok(NULL, delim)) != NULL){
                if (DataSave1->udtTransDetail.udtVendType == ZAIN) {
                  strcpy(udtBillInqInfo.aucTotalBill, token); 
                } else if (DataSave1->udtTransDetail.udtVendType == MTN) {
                  strcpy(udtBillInqInfo.aucUNBillAmount, token); 
                }
                if ((token = strtok(NULL, delim)) != NULL){
                  if ((DataSave1->udtTransDetail.udtVendType ==ZAIN) ||
                      (DataSave1->udtTransDetail.udtVendType == MTN)){
                    strcpy(udtBillInqInfo.aucLastInvoiceDate,token);
                  }
                }
              }
            }
          }
        }
        DBGPrt_m("aucContractNumber:%s,aucBillamount:%s, aucUNBillAmount:%s,aucTotalBill:%s,aucLastInvoiceDate:%s\r\n",
                udtBillInqInfo.aucContractNumber,udtBillInqInfo.aucBillamount, udtBillInqInfo.aucUNBillAmount,
                udtBillInqInfo.aucTotalBill,udtBillInqInfo.aucLastInvoiceDate);
      }
    }
  }
  return ucResult;
}
/*
 *
 */
uchar ucPymntSndRcv(uchar ucVenIdx, bool bIsTopUp)
{
  unsigned char aucBuf[1024], aucTempBuf[100];
  ulong ullen;
  uchar ucResult;

  if (ucCmnBitsSet() == ERR_REV_TRIALS)
  {
    return ERR_REV_TRIALS;
  }

  // Field57 (Additional Data: ans..999) - Mandatory
  // Sub-Field 57.62 (Card PAN of the transfer recipient : n..19)
  memset(aucP57Buff,0,sizeof(aucP57Buff));
  usP57BufLen = 0;
  ullen = strlen(DataSave1->udtDeviceConfig.audtVedInstitCode[ucVenIdx][bIsTopUp].aucVendCode);
  ucSetP57(10,DataSave1->udtDeviceConfig.audtVedInstitCode[ucVenIdx][bIsTopUp].aucVendCode,ullen);

  ullen = strlen(DataSave1->udtDeviceConfig.audtVedInstitCode[ucVenIdx][bIsTopUp].aucInstitCode);
  ucSetP57(11,DataSave1->udtDeviceConfig.audtVedInstitCode[ucVenIdx][bIsTopUp].aucInstitCode,ullen);

  ullen = strlen(DataSave1->udtTransDetail.aucAccountNo);
  ucSetP57(12,DataSave1->udtTransDetail.aucAccountNo,ullen);

  memset(aucBuf,0,sizeof(aucBuf));
  memcpy(aucBuf,aucP57Buff,usP57BufLen);
  ISO8583_SetBitValue(57,aucBuf,usP57BufLen);

  ISO8583_SetMsgID(MTYPE_FINANCIAL_TRXN_REQUEST);
  ISO8583_SetTPDU("\x60\x00\x00\x00\0x00");
  ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                          (unsigned char *)&DataSave0->SaveISO8583Data);

  ucResult = COMMS_TransOnline();
  if((ucResult==SUCCESS) || (ucResult==ERR_HOSTCODE)){
    ucGetSetAuthRefTansID();
  } else{
    //ChkSendReversal();
  }
  return ucResult;
}

/*
 *
 */
uchar ucNECSndRcv(void)
{
  unsigned char aucBuf[1024], aucTempBuf[100];
  int iLen;
  uchar ucResult;
  const char delim[2] = "\x1c";
  char *token;

  if (ucCmnBitsSet() == ERR_REV_TRIALS)
  {
    return ERR_REV_TRIALS;
  }
  

  // Field57 (Additional Data: ans..999) - Mandatory
  // Sub-Field 57.62 (Card PAN of the transfer recipient : n..19)
  memset(aucP57Buff,0,sizeof(aucP57Buff));
  usP57BufLen = 0;
  iLen = strlen(DataSave1->udtDeviceConfig.audtVedInstitCode[4][0].aucVendCode);
  ucSetP57(10,DataSave1->udtDeviceConfig.audtVedInstitCode[4][0].aucVendCode,iLen);

  iLen = strlen(DataSave1->udtDeviceConfig.audtVedInstitCode[4][0].aucInstitCode);
  ucSetP57(11,DataSave1->udtDeviceConfig.audtVedInstitCode[4][0].aucInstitCode,iLen);

  iLen = strlen(DataSave1->udtTransDetail.aucAccountNo);
  ucSetP57(12,DataSave1->udtTransDetail.aucAccountNo,iLen);

  memset(aucBuf,0,sizeof(aucBuf));
  memcpy(aucBuf,aucP57Buff,usP57BufLen);
  ISO8583_SetBitValue(57,aucBuf,usP57BufLen);

  ISO8583_SetMsgID(MTYPE_FINANCIAL_TRXN_REQUEST);
  ISO8583_SetTPDU("\x60\x00\x00\x00\0x00");
  ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                          (unsigned char *)&DataSave0->SaveISO8583Data);

  ucResult = COMMS_TransOnline();
  if((ucResult==SUCCESS) || (ucResult==ERR_HOSTCODE)){
    ucGetSetAuthRefTansID();
    if (ISO8583_GetBitValue(63, aucBuf, &iLen, sizeof(aucBuf)) == SUCCESS){
      DBGPrt_m("aucP63:%s\r\n",aucBuf);
      if (ucParseP63(aucBuf,iLen,29)== TRUE){
        memset((void*)&udtNECInfo,0,sizeof(udtNECInfo));
     /* get the first token */
        if ((token = strtok(aucP63RcvBuff, delim)) != NULL){
          strcpy(udtNECInfo.aucSalesAmount,token);
          if ((token = strtok(NULL, delim)) != NULL){
            strcpy(udtNECInfo.aucFixesFee,token);
            if ((token = strtok(NULL, delim)) != NULL){
              strcpy(udtNECInfo.aucToken,token);
              if ((token = strtok(NULL, delim)) != NULL){
                strcpy(udtNECInfo.aucMeterNumber,token);
                if ((token = strtok(NULL, delim)) != NULL){
                  strcpy(udtNECInfo.aucUnit,token);
                  if ((token = strtok(NULL, delim)) != NULL){
                    strcpy(udtNECInfo.aucAccountNumber,token);
                    if ((token = strtok(NULL, delim)) != NULL){
                      strcpy(udtNECInfo.aucReceiptNumber,token);
                      if ((token = strtok(NULL, delim)) != NULL){
                        strcpy(udtNECInfo.aucCustomerName,token);
                        if ((token = strtok(NULL, delim)) != NULL){
                          strcpy(udtNECInfo.aucReferenceNumber,token);
                          if ((token = strtok(NULL, delim)) != NULL){
                            strcpy(udtNECInfo.aucOperator,token);
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
        DBGPrt_m("aucSalesAmount:%s\r\n aucFixesFee:%s\r\n aucToken:%s\r\n aucMeterNumber:%s\r\n aucUnit:%s\r\n aucAccountNumber:%s\r\n aucReceiptNumber:%s\r\n aucCustomerName:%s\r\n aucReferenceNumber:%s\r\n aucOperator:%s\r\n ",
                udtNECInfo.aucSalesAmount,
                udtNECInfo.aucFixesFee,
                udtNECInfo.aucToken,
                udtNECInfo.aucMeterNumber,
                udtNECInfo.aucUnit,
                udtNECInfo.aucAccountNumber,
                udtNECInfo.aucReceiptNumber,
                udtNECInfo.aucCustomerName,
                udtNECInfo.aucReferenceNumber,
                udtNECInfo.aucOperator);
      }
    }
  } else{
    //ChkSendReversal();
  }
  return ucResult;
}
/*
 *
 */
uchar ucCardTransSndRcv(void)
{
  unsigned char aucBuf[1024], aucTempBuf[100];
  unsigned char ucResult;
  int iLen = 0;

  if (ucCmnBitsSet() == ERR_REV_TRIALS) {
    return ERR_REV_TRIALS;
  }
  

  // Field57 (Additional Data: ans..999) - Mandatory
  // Sub-Field 57.62 (Card PAN of the transfer recipient : n..19)
  memset(aucP57Buff,0,sizeof(aucP57Buff));
  usP57BufLen = 0;
  iLen = strlen(DataSave1->udtTransDetail.aucAccountNo);
  ucSetP57(62,DataSave1->udtTransDetail.aucAccountNo,iLen);

  memset(aucBuf,0,sizeof(aucBuf));
  memcpy(aucBuf,aucP57Buff,usP57BufLen);
  ISO8583_SetBitValue(57,aucBuf,usP57BufLen);

  ISO8583_SetMsgID(MTYPE_FINANCIAL_TRXN_REQUEST);
  ISO8583_SetTPDU("\x60\x00\x00\x00\0x00");
  ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                          (unsigned char *)&DataSave0->SaveISO8583Data);

  ucResult = COMMS_TransOnline();
  if((ucResult==SUCCESS) || (ucResult==ERR_HOSTCODE)){
    ucGetSetAuthRefTansID();
  } else{
    ChkSendReversal();
  }
  return ucResult;
}
/*
 *
 */
uchar ucAccntTransSndRcv(void)
{
  unsigned char aucBuf[1024], aucTempBuf[100];
  int iLen;
  uchar ucResult;

  if (ucCmnBitsSet() == ERR_REV_TRIALS) {
    return ERR_REV_TRIALS;
  }
  

  // Field57 (Additional Data: ans..999) - Mandatory
  // Sub-Field 57.10 (To Account : ans..30)
  memset(aucP57Buff,0,sizeof(aucP57Buff));
  usP57BufLen = 0;
  iLen = strlen(DataSave1->udtTransDetail.aucAccountNo);
  ucSetP57(10,DataSave1->udtTransDetail.aucAccountNo,iLen);

  memset(aucBuf,0,sizeof(aucBuf));
  memcpy(aucBuf,aucP57Buff,usP57BufLen);
  ISO8583_SetBitValue(57,aucBuf,usP57BufLen);

  ISO8583_SetMsgID(MTYPE_FINANCIAL_TRXN_REQUEST);
  ISO8583_SetTPDU("\x60\x00\x00\x00\0x00");
  ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                          (unsigned char *)&DataSave0->SaveISO8583Data);

  ucResult = COMMS_TransOnline();
  if((ucResult==SUCCESS) || (ucResult==ERR_HOSTCODE)){
    ucGetSetAuthRefTansID();
  } else{
    ChkSendReversal();
  }
  return ucResult;
}
/*
 *
 */
uchar ucMoneyDepositSndRcv(void)
{
  uchar ucResult;
  if (ucCmnBitsSet() == ERR_REV_TRIALS)
  {
    return ERR_REV_TRIALS;
  }
  
  ISO8583_SetMsgID(MTYPE_FINANCIAL_TRXN_REQUEST);
  ISO8583_SetTPDU("\x60\x00\x00\x00\0x00");
  ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                          (unsigned char *)&DataSave0->SaveISO8583Data);

  ucResult = COMMS_TransOnline();
  if((ucResult==SUCCESS) || (ucResult==ERR_HOSTCODE)){
    ucGetSetAuthRefTansID();
  } else{
    ChkSendReversal();
  }
  return ucResult;
}
/*
 *
 */
uchar ucMoneyTransRcvSndRcv(bool bIsTrans)
{
  unsigned char aucBuf[1024], aucTempBuf[100];
  int iLen;
  uchar ucResult;
  ulong ulAmnt;

  if (ucCmnBitsSet() == ERR_REV_TRIALS) {
    return ERR_REV_TRIALS;
  }
  

  if (bIsTrans == FALSE) {
    // Field38 (Authorization Identification Response: ans6) - Mandatory
    memset(aucBuf,0,sizeof(aucBuf));
    memcpy(aucBuf,DataSave1->udtTransDetail.aucAuthIdResponse,
           strlen(DataSave1->udtTransDetail.aucAuthIdResponse));
    ISO8583_SetBitValue(38,aucBuf,6);
  }

  // Field57 (Additional Data: ans..999) - Mandatory
  // Sub-Field 57.90 (Transaction subcode: n 12)
  memset(aucP57Buff,0,sizeof(aucP57Buff));
  usP57BufLen = 0;
  //iLen = strlen(DataSave1->udtTransDetail.aucAccountNo);
  ucSetP57(90,"000000000001",12);

  memset(aucBuf,0,sizeof(aucBuf));
  memcpy(aucBuf,aucP57Buff,usP57BufLen);
  ISO8583_SetBitValue(57,aucBuf,usP57BufLen);

  // Field57 (Additional Data: ans..999) - Mandatory
  // Sub-Field 57.90 (Transaction subcode: n 12)
  memset(aucP63Buff,0,sizeof(aucP63Buff));
  usP63BufLen = 0;
  iLen = strlen(DataSave1->udtTransDetail.aucAccountNo);
  DBGPrt_m("aucAccountNo:%s\r\n",DataSave1->udtTransDetail.aucAccountNo);
  ucSetP63(29,DataSave1->udtTransDetail.aucAccountNo,iLen);

  memset(aucBuf,0,sizeof(aucBuf));
  memcpy(aucBuf,aucP63Buff,usP63BufLen);
  ISO8583_SetBitValue(63,aucBuf,usP63BufLen);

  ISO8583_SetMsgID(MTYPE_FINANCIAL_TRXN_REQUEST);
  ISO8583_SetTPDU("\x60\x00\x00\x00\0x00");
  ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                          (unsigned char *)&DataSave0->SaveISO8583Data);

  ucResult = COMMS_TransOnline();
  if((ucResult==SUCCESS) || (ucResult==ERR_HOSTCODE)){
    ucGetSetAuthRefTansID();
    if ((bIsTrans == FALSE) && (ucResult==SUCCESS)){
      if (ISO8583_GetBitValue(63, aucBuf, &iLen, sizeof(aucBuf)) == SUCCESS){
        DBGPrt_m("aucP63:%s\r\n",aucBuf);
        if (ucParseP63(aucBuf,iLen,29)== TRUE){
          ulAmnt = atol(aucP63RcvBuff);
          sprintf(DataSave1->udtTransDetail.aucAmount,"%d",ulAmnt);
          //strcpy(DataSave1->udtTransDetail.aucAmount,aucP63RcvBuff);
        }
      }
    }
  } else{
    ChkSendReversal();
  }
  return ucResult;
}
/*
 *
 */
uchar ucBalInqSndRcv(void)
{
  unsigned char aucBuf[1024], aucTempBuf[100];
  unsigned char ucResult;
  int iLen = 0;
  char *token;

  if (ucCmnBitsSet() == ERR_REV_TRIALS) {
    return ERR_REV_TRIALS;
  }
  
  ISO8583_SetMsgID(MTYPE_AUTHORIZATION_REQUEST);
  ISO8583_SetTPDU("\x60\x00\x00\x00\0x00");
  ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                          (unsigned char *)&DataSave0->SaveISO8583Data);

  ucResult = COMMS_TransOnline();
  if((ucResult==SUCCESS) || (ucResult==ERR_HOSTCODE)){
    ucGetSetAuthRefTansID();

    ISO8583_GetBitValue(63, aucBuf, &iLen, sizeof(aucBuf));
    asc_to_str(aucTempBuf,aucBuf,iLen);
    if ((token = strrchr(aucTempBuf, ' ')) != NULL){
      strcpy(DataSave1->udtTransDetail.aucBalance,(token+1));
    } else {
      memcpy(DataSave1->udtTransDetail.aucBalance,aucBuf,iLen);
    }
  }
  return ucResult;

}
/*
 *
 */
uchar ucPINChangeSndRcv(uchar* pucNewPin)
{
  unsigned char aucBuf[1024], aucTempBuf[100],aucAscNewPin[20];
  ulong ullen;
  uchar ucResult;

  if (ucCmnBitsSet() == ERR_REV_TRIALS)
  {
    return ERR_REV_TRIALS;
  }
  

  // Field57 (Additional Data: ans..999) - Mandatory
  // Sub-Field 57.70 (New PIN : ans16)
  memset(aucP57Buff,0,sizeof(aucP57Buff));
  memset(aucAscNewPin,0,sizeof(aucAscNewPin));
  usP57BufLen = 0;
  num_to_asc(aucAscNewPin,pucNewPin,16);
#if DEV_DEBUG
  uchar ucIdx;
  DBGPrt_m("pucNewPin");
  for (ucIdx = 0; ucIdx < strlen(pucNewPin); ucIdx++) {
    DBGPrt_m("%02x",pucNewPin[ucIdx]);
  }
  DBGPrt_m("\r\n"); 
  DBGPrt_m("aucAscNewPin:%s\r\n",aucAscNewPin);
#endif    
  ucSetP57(70,aucAscNewPin,16);
  memset(aucBuf,0,sizeof(aucBuf));
  memcpy(aucBuf,aucP57Buff,usP57BufLen);
  ISO8583_SetBitValue(57,aucBuf,usP57BufLen);

  ISO8583_SetMsgID(MTYPE_FINANCIAL_TRXN_REQUEST);
  ISO8583_SetTPDU("\x60\x00\x00\x00\0x00");
  ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                          (unsigned char *)&DataSave0->SaveISO8583Data);

  ucResult = COMMS_TransOnline();
  if((ucResult==SUCCESS) || (ucResult==ERR_HOSTCODE)){
    ucGetSetAuthRefTansID();
  } else{
    ChkSendReversal();
  }
  return ucResult;
}
/*
 *
 */
uchar ucVoidSndRcv(void)
{
  unsigned char aucBuf[1024], aucTempBuf[100];
  unsigned char ucResult;
  int iLen = 0;

  if (ucCmnBitsSet() == ERR_REV_TRIALS)
  {
    return ERR_REV_TRIALS;
  }
  

  ISO8583_SetMsgID(MTYPE_FINANCIAL_TRXN_REQUEST);
  ISO8583_SetTPDU("\x60\x00\x00\x00\0x00");
  ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                          (unsigned char *)&DataSave0->SaveISO8583Data);

  ucResult = COMMS_TransOnline();
  if((ucResult==SUCCESS) || (ucResult==ERR_HOSTCODE)){
    ucGetSetAuthRefTansID();
  } else{
    ChkSendReversal();
  }
  return ucResult;

}
/*
 *
 */
uchar ucPurchaseCashbackSndRcv(void)
{
  unsigned char aucBuf[1024], aucTempBuf[100];
  uchar ucResult;
  int iLen = 0;
  ulong ullen;

  if (ucCmnBitsSet() == ERR_REV_TRIALS)
  {
    return ERR_REV_TRIALS;
  }
  

  // Field60 (Private use: ans..999) - Mandatory
  // Sub-Field (cash back amount : an 12)

  memset(aucBuf,0,sizeof(aucBuf));
  iLen = strlen(DataSave1->udtTransDetail.aucCashBackAmount);
  memset(&aucBuf[0],'0',12-iLen);
  memcpy(&aucBuf[12-iLen],DataSave1->udtTransDetail.aucCashBackAmount,iLen);
  ISO8583_SetBitValue(60,aucBuf,12);

  ISO8583_SetMsgID(MTYPE_FINANCIAL_TRXN_REQUEST);
  ISO8583_SetTPDU("\x60\x00\x00\x00\0x00");
  ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                          (unsigned char *)&DataSave0->SaveISO8583Data);

  ucResult = COMMS_TransOnline();
  if((ucResult==SUCCESS) || (ucResult==ERR_HOSTCODE)){
    ucGetSetAuthRefTansID();
  } else{
    ChkSendReversal();
  }
  return ucResult;
}
/*
 *
 */
uchar ucMiniStatSndRcv(void)
{
  unsigned char aucBuf[1024], aucTempBuf[100];
  unsigned char ucResult;
  uchar ucTempLen;
  char *ret;
  int iLen = 0;
  uchar ucIdx;
  ulong ulFld34Len=0;

  if (ucCmnBitsSet() == ERR_REV_TRIALS)
  {
    return ERR_REV_TRIALS;
  }
  

  ISO8583_SetMsgID(MTYPE_FINANCIAL_TRXN_REQUEST);
  ISO8583_SetTPDU("\x60\x00\x00\x00\0x00");
  ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                          (unsigned char *)&DataSave0->SaveISO8583Data);

  ucResult = COMMS_TransOnline();
  if((ucResult==SUCCESS) || (ucResult==ERR_HOSTCODE)){
    ucGetSetAuthRefTansID();

    if (ISO8583_GetBitValue(57, aucBuf, &iLen, sizeof(aucBuf)) == SUCCESS){
      DBGPrt_m("aucP57:%s\r\n",aucBuf);
      if (ucParseP57(aucBuf,iLen,34)== TRUE){
        memset((void*)&udtMiniStatment,0,sizeof(udtMiniStatment));
        ulFld34Len = strlen(aucP57RcvBuff);
        udtMiniStatment.ucNoRecords = ulFld34Len/20;
        for (ucIdx = 0; ucIdx < udtMiniStatment.ucNoRecords; ucIdx++) {
          memset(aucTempBuf, 0, sizeof(aucTempBuf));
          strncpy(aucTempBuf, &aucP57RcvBuff[20*ucIdx],20);
          memcpy(udtMiniStatment.rudtStatmentRcrds[ucIdx].aucDateMMDD,aucTempBuf,4); 
          memcpy(udtMiniStatment.rudtStatmentRcrds[ucIdx].aucOpCode,&aucTempBuf[4],3); 

          ret = strrchr(&aucTempBuf[7],' ');
          ret++;
          ucTempLen = 12-(uchar)ret+(uchar)&aucTempBuf[7];
          memcpy(udtMiniStatment.rudtStatmentRcrds[ucIdx].aucAmount,ret,ucTempLen);
          DBGPrt_m("aucAmount:%s\r\n",udtMiniStatment.rudtStatmentRcrds[ucIdx].aucAmount);

          if (aucTempBuf[19] == '-') {
            udtMiniStatment.rudtStatmentRcrds[ucIdx].bIsNegative = TRUE;
          }
        }
        DBGPrt_m("Fld34:%s\r\n",aucP57RcvBuff);
      } else {
        DBGPrt_m("Parse Error");
      }
    }
  }
  return ucResult;

}
/*
 *
 */
uchar ucRefundSndRcv(void)
{
  unsigned char aucBuf[1024];
  unsigned char ucResult;
  int iLen = 0;

  if (ucCmnBitsSet() == ERR_REV_TRIALS)
  {
    return ERR_REV_TRIALS;
  }
  

  ISO8583_SetMsgID(MTYPE_FINANCIAL_TRXN_REQUEST);
  ISO8583_SetTPDU("\x60\x00\x00\x00\0x00");
  ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                          (unsigned char *)&DataSave0->SaveISO8583Data);

  ucResult = COMMS_TransOnline();
  if((ucResult==SUCCESS) || (ucResult==ERR_HOSTCODE))
  {
    ucGetSetAuthRefTansID();
  }
  else
  {
    ChkSendReversal();
  }
  return  ucResult;
}
/*
 *
 */
uchar ucBatchSndRcv(void)
{
  unsigned char aucBuf[1024], aucTempBuf[100];
  ulong ullen;
  uchar ucResult;
  ulong ulIdx;
  ulong ulAmount = 0;

  ISO8583_Clear();

  //DataSave1->ulBatchNo++; 

  // Field 3 (Processing Code: n 6) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  sprintf(aucTempBuf, "%06lX", DataSave1->udtTransDetail.ulPCode); 
  memcpy(aucBuf,aucTempBuf,strlen(aucTempBuf));
  ISO8583_SetBitValue(3,aucBuf,6);

  // Field11 (System Trace Audit Number: n 6) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  //vidIncSTAN();
  sprintf(aucTempBuf, "%06d", DataSave1->ulSysTraceAuditNo);
  memcpy(aucBuf,aucTempBuf,strlen(aucTempBuf));
  ISO8583_SetBitValue(11,aucBuf,6);

  // Field41 (Card Acceptor Terminal Identification: ans8) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  ullen = strlen(DataSave1->udtDeviceConfig.aucTermId);
  memcpy(aucBuf,DataSave1->udtDeviceConfig.aucTermId,ullen);
  //DBGPrt_m("aucBuf41:%s\r\n",aucBuf);
  ISO8583_SetBitValue(41,aucBuf,8);

  // Field60 (Batch Number: ans6) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  sprintf(aucTempBuf, "%06d", DataSave1->ulBatchNo);
  memcpy(aucBuf,aucTempBuf,strlen(aucTempBuf));
  ISO8583_SetBitValue(60,aucBuf,6);

  if (DataSave1->udtTransDetail.ulPCode == PCODE_SETTLEMENT) {
    for (ulIdx = 0; ulIdx < DataSave1->udtTransStore.ulSavedTransNo; ulIdx++) {
      ulAmount += atol(DataSave1->udtTransStore.audtTrans[ulIdx].aucAmount);
    }

    // Field63 (Batch Total) - Mandatory
    memset(aucBuf,0,sizeof(aucBuf));
    sprintf(aucTempBuf, "%03d%012d%045d", 
            DataSave1->udtTransStore.ulSavedTransNo,ulAmount,0);
    memcpy(aucBuf,aucTempBuf,strlen(aucTempBuf));
    ISO8583_SetBitValue(63,aucBuf,60);
  }

  ISO8583_SetMsgID(MTYPE_SETTLEMENT_REQUEST);
  ISO8583_SetTPDU("\x60\x00\x00\x00\x00");
  ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                          (unsigned char *)&DataSave0->SaveISO8583Data);

  ucResult = COMMS_TransOnlineNoConn();
  memset(&DataSave1->udtRevesalData,0,sizeof(strRevesalData));
  if((ucResult==SUCCESS) || (ucResult==ERR_HOSTCODE)){
    ucGetSetAuthRefTansID();
  }
  return ucResult;
}
/*
 *
 */

uchar ucBatchUploadSndRcv(void)
{
  unsigned char ucResult;
  unsigned char aucBuf[1024];
  unsigned char aucTempBuf[100];
  ulong ullen;
  int iResult;

  DATE_TIME udtTransDateTime;
  uchar aucTransDate[9];
  uchar aucTransTime[7];


  //ChkSendReversal();

  ISO8583_Clear();

  if (DataSave1->udtTransDetail.udtMSRCard.bIsValidData)
  {
    // Field 2 (Primary Account Number: n..19) - Mandatory
    memset(aucBuf,0,sizeof(aucBuf));
    ullen = strlen(DataSave1->udtTransDetail.PAN);
    memcpy(aucBuf,DataSave1->udtTransDetail.PAN,ullen);
    ISO8583_SetBitValue(2,aucBuf,ullen);
  }

  // Field 3 (Processing Code: n 6) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  sprintf(aucTempBuf, "%06lX", DataSave1->udtTransDetail.ulPCode);
  memcpy(aucBuf,aucTempBuf,strlen(aucTempBuf));
  ISO8583_SetBitValue(3,aucBuf,6);

  if (DataSave1->udtTransDetail.aucAmount > 0) {
    // Field 4 (Transaction Amount: n 12) - Mandatory
    memset(aucBuf,0,sizeof(aucBuf));
    memcpy(&aucBuf[12-strlen(DataSave1->udtTransDetail.aucAmount)],
           DataSave1->udtTransDetail.aucAmount,strlen(DataSave1->udtTransDetail.aucAmount));
    ISO8583_SetBitValue(4,aucBuf,12);
  }

  // Field11 (System Trace Audit Number: n 6) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  //vidIncSTAN();
  sprintf(aucTempBuf, "%06d", DataSave1->ulSysTraceAuditNo); 
  memcpy(aucBuf,aucTempBuf,strlen(aucTempBuf));
  ISO8583_SetBitValue(11,aucBuf,6);

  if (GetRTCDateTime(&udtTransDateTime,0)) {
    memset(aucTransTime,0,sizeof(aucTransTime));
    memset(aucTransDate,0,sizeof(aucTransDate));

    sprintf(aucTransTime, "%02X%02X%02X",
            udtTransDateTime.CLKHOUR, udtTransDateTime.CLKMIN, udtTransDateTime.CLKSEC);
    sprintf(aucTransDate,"%02X%02X%02X%02X",
            udtTransDateTime.CLKYEARH, udtTransDateTime.CLKYEARL,
            udtTransDateTime.CLKMONTH, udtTransDateTime.CLKDATE);
  } else {
    return FALSE;
  }
  // Field12 (Local (Terminal) Transaction Time: n 6) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  memcpy(aucBuf,aucTransTime,6);
  ISO8583_SetBitValue(12,aucBuf,6);

  // Field13 (Local (Terminal) Transaction Date: n 4) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  memcpy(aucBuf,&aucTransDate[4],4);
  ISO8583_SetBitValue(13,aucBuf,4);

  if (DataSave1->udtTransDetail.udtMSRCard.bIsValidData){
    //Field14 (Card expiration date: n 4) - Mandatory
    memset(aucBuf,0,sizeof(aucBuf));
    memcpy(aucBuf,DataSave1->udtTransDetail.ExpiryDate,4);
    ISO8583_SetBitValue(14,aucBuf,4);
  }

  // Field22 (POS Entry Mode: n 3) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  sprintf(aucTempBuf, "%04lX", DataSave1->udtTransDetail.usPOSEntryMode);
  memcpy(aucBuf,aucTempBuf,4);
  ISO8583_SetBitValue(22,aucBuf,4);

  // Field23 (card member number (MBR): n 3) - Optional
  //memset(aucBuf,0,sizeof(aucBuf));
  //memcpy(aucBuf,,);
  //ISO8583_SetBitValue(23,aucBuf,);

  // Field25 (POS Condition Code: n 2) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  sprintf(aucTempBuf, "%02X", DataSave1->udtTransDetail.ucPosCondCode);
  memcpy(aucBuf,aucTempBuf,2);
  ISO8583_SetBitValue(25,aucBuf,2);

  // Field37 (Retrieval Reference Number: ans12) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  memcpy(aucBuf,DataSave1->udtTransDetail.aucRtrvalRefNo,12);
  ISO8583_SetBitValue(37,aucBuf,12);

  // Field38 (Authorization Identification Response: ans6) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  memcpy(aucBuf,DataSave1->udtTransDetail.aucAuthIdResponse,
         strlen(DataSave1->udtTransDetail.aucAuthIdResponse));
  ISO8583_SetBitValue(38,aucBuf,6);

  // Field41 (Card Acceptor Terminal Identification: ans8) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  ullen = strlen(DataSave1->udtDeviceConfig.aucTermId);
  memcpy(aucBuf,DataSave1->udtDeviceConfig.aucTermId,ullen);
  ISO8583_SetBitValue(41,aucBuf,8);

  // Field60 (Batch Number: ans6) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  sprintf(aucTempBuf, "%04lX%06d%012d", MTYPE_FINANCIAL_TRXN_REQUEST,
          DataSave1->udtTransDetail.ulSysTraceAuditNo,0);
  memcpy(aucBuf,aucTempBuf,strlen(aucTempBuf));
  ISO8583_SetBitValue(60,aucBuf,22);

  ISO8583_SetMsgID(MTYPE_BATCH_UPLOAD_REQUEST);
  ISO8583_SetTPDU("\x60\x00\x00\x00\x00");
  ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                          (unsigned char *)&DataSave0->SaveISO8583Data);
}
/*
 *
 */
uchar ucBatchUploadAll()
{
  uchar ucResult;
  ulong ulIdx;
  int iResult;

  usCommHdrIdx = S_BATCH_UPLD_HDR;
  udtRunData.bIsBatchUpload = TRUE;
  for (ulIdx = 0; ulIdx <DataSave1->udtTransStore.ulSavedTransNo; ulIdx++)
  {
    memset(&DataSave1->udtTransDetail,0,sizeof(strTransDetail));
    memcpy(&DataSave1->udtTransDetail, &(DataSave1->udtTransStore.audtTrans[ulIdx]),
             sizeof(strTransDetail));

    MaxTrN = DataSave1->udtTransStore.ulSavedTransNo;
    LeftTrN  = ulIdx+1;
    ucBatchUploadSndRcv();

    ucResult = COMMS_TransOnlineNoConn();
    if((ucResult!=SUCCESS)){
      break;
    }
  }
  udtRunData.bIsBatchUpload =FALSE;

  return ucResult;

}
/*
 *
 */
uchar ucEchoTestSndRcv(void)
{
  unsigned char aucBuf[1024], aucTempBuf[100];
  ulong ullen;
  uchar ucResult;

  if (ucCmnReducedBitsSet() == ERR_REV_TRIALS)
  {
    return ERR_REV_TRIALS;
  }
  ISO8583_SetMsgID(MTYPE_NMS_REQUEST);
  ISO8583_SetTPDU("\x60\x00\x00\x00\x00");
  ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                          (unsigned char *)&DataSave0->SaveISO8583Data);

  ucResult = COMMS_TransOnline();
  if((ucResult==SUCCESS) || (ucResult==ERR_HOSTCODE)){
  }
  return ucResult;
}
/*
 *
 */
uchar ucNewKeysInq(void)
{
  unsigned char aucBuf[1024], aucTempBuf[100];
  unsigned char ucResult;
  int iLen;
  uchar ucIdx;

  if (ucCmnReducedBitsSet() == ERR_REV_TRIALS)
  {
    return ERR_REV_TRIALS;
  }

  ISO8583_SetMsgID(MTYPE_NMS_REQUEST);
  ISO8583_SetTPDU("\x60\x00\x00\x00\x00");
  ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                          (unsigned char *)&DataSave0->SaveISO8583Data);

  ucResult = COMMS_TransOnline();
  if((ucResult==SUCCESS) || (ucResult==ERR_HOSTCODE)){
    ISO8583_GetBitValue(62, aucBuf, &iLen, sizeof(aucBuf));
    memset(DataSave1->udtDeviceConfig.aucTPK,0,sizeof(DataSave1->udtDeviceConfig.aucTPK));
    memcpy(DataSave1->udtDeviceConfig.aucTPK,&aucBuf[2],8);
    for (ucIdx = 0; ucIdx < strlen(DataSave1->udtDeviceConfig.aucTPK); ucIdx++) {
      DBGPrt_m("aucTPK[%d]: %02x", ucIdx, DataSave1->udtDeviceConfig.aucTPK[ucIdx]); 
    }
  }
  return ucResult;
}

/*
 *
 */
uchar ucReversal(void)
{
  unsigned char aucBuf[1024], aucTempBuf[100];
  unsigned char ucResult;
  int iLen;
  uchar ucIdx;

  if (DataSave1->udtRevesalData.udtMSRCard.bIsValidData) {
    // Field 2 (Primary Account Number: n..19) - Mandatory
    memset(aucBuf,0,sizeof(aucBuf));
    iLen = strlen(DataSave1->udtRevesalData.PAN);
    memcpy(aucBuf,DataSave1->udtRevesalData.PAN,iLen);
    ISO8583_SetBitValue(2,aucBuf,iLen);
  }

  // Field 3 (Processing Code: n 6) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  sprintf(aucTempBuf, "%06lX", DataSave1->udtRevesalData.ulPCode);
  memcpy(aucBuf,aucTempBuf,strlen(aucTempBuf));
  ISO8583_SetBitValue(3,aucBuf,6);

  // Field11 (System Trace Audit Number: n 6) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  sprintf(aucTempBuf, "%06d", DataSave1->udtRevesalData.ulSysTraceAuditNo);
  memcpy(aucBuf,aucTempBuf,strlen(aucTempBuf));
  ISO8583_SetBitValue(11,aucBuf,6);


  if (DataSave1->udtRevesalData.udtMSRCard.bIsValidData){
    // Field14 (Card expiration date: n 4) - Mandatory
    memset(aucBuf,0,sizeof(aucBuf));
    memcpy(aucBuf,DataSave1->udtRevesalData.ExpiryDate,4);
    ISO8583_SetBitValue(14,aucBuf,4);
  }

  if ((DataSave1->udtRevesalData.udtMSRCard.bIsValidData)&&
      ((DataSave1->udtRevesalData.usPOSEntryMode & PEM_PAN_ENTRY_MASK) != PEM_MANUAL)){
    // Field35 (Track 2 Data: z..37) - Mandatory
    memset(aucBuf,0,sizeof(aucBuf));
    iLen = strlen(DataSave1->udtTransDetail.udtMSRCard.acTrack2);
    memcpy(aucBuf,DataSave1->udtTransDetail.udtMSRCard.acTrack2,iLen);
    ISO8583_SetBitValue(35,aucBuf,iLen);
  }

  // Field41 (Card Acceptor Terminal Identification: ans8) - Mandatory
  memset(aucBuf,0,sizeof(aucBuf));
  iLen = strlen(DataSave1->udtRevesalData.aucTermId);
  memcpy(aucBuf,DataSave1->udtRevesalData.aucTermId,iLen);
  ISO8583_SetBitValue(41,aucBuf,8);

  ISO8583_SetMsgID(MTYPE_REVERSAL_REQUEST);
  ISO8583_SetTPDU("\x60\x00\x00\x00\x00");
  ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                          (unsigned char *)&DataSave0->SaveISO8583Data);

   udtRunData.bIsCurrReversal = TRUE;
  ucResult = COMMS_TransOnline();
  udtRunData.bIsCurrReversal = FALSE;

  return ucResult;

}
/*
 *
 */
void RprintBuf_my(const char* bufName, const unsigned char* buf, int nSize)
{
	int idx;
	Rprintf("%s: ", bufName);
	for (idx = 0; idx < nSize; idx++) Rprintf("%c", (buf[idx] & 0xFF));
	Rprintf("\r\n");
}
/*
 *
 */
void gsoap()
{
//  char acSysTraceAuditNo[9];
//  Rprintf("gsoap\r\n");
//	struct soap osoap;
//	soap_init2(&osoap,SOAP_IO_KEEPALIVE, SOAP_IO_KEEPALIVE);
////	osoap.max_keep_alive= 100; // at most 100 calls per keep-alive session
////	osoap.send_timeout = 2;
////	osoap.recv_timeout= 2;
////	osoap.connect_timeout= 2;
//
//
//	//TransactionProcessor.soap_endpoint = server;
//	struct SOAP_ENV__Header soapheader; ZM(soapheader);
//	_wsse__Security Security; ZM(Security);
//	_wsse__UsernameToken UsernameToken; ZM(UsernameToken);
//	_wsse__Password Password; ZM(Password);
//
//	soapheader.wsse__Security = &Security;
//	soapheader.wsse__Security->UsernameToken = &UsernameToken;
//	soapheader.wsse__Security->UsernameToken->Username = "v4p225p825184";
//	soapheader.wsse__Security->UsernameToken->Password = &Password;
//	soapheader.wsse__Security->UsernameToken->Password->__item = "lcYmfq+9YfFZD+Yj7j6wb820xodAAMsmmxjhCjvZKtrm9cfvqRY+yGY5QsU/LOB5FO/giv8wmzQjmaJ3sbPwoIgXKDrjaGMHqWM9Ap9Vk/vukoHA6f5bZfqo2LTqtCpqz4QrIoLVIyjgV5X0872UzfVlBOZm6Sxaz1q8A7OuNe5gSjFgPR8ggUUl6z8ti9mECE/EIVQRTmUp9EOjlCSXMJS9MFDfTl/fMGVjvqoYAl0uHjKUa9x1aKoxN0dHEXVgrQbf8/4ohMkFQfTiq0tAgLtHcGyPlGOnWvkY1oslHSXOHHD4/n1xnqk2IFhmWwkJ8kH+1g7hGmQq4qjudRb4dA==";
//	soapheader.wsse__Security->UsernameToken->Password->Type = "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wssusername-token-profile-1.0#PasswordText";
//	osoap.header = &soapheader;
//	  Rprintf("soap_set_namespaces\r\n");
//
//	soap_set_namespaces(&osoap, namespaces);
//
//	struct ns3__RequestMessage RequestMessage; ZM(RequestMessage);
//	struct ns3__ReplyMessage ReplyMessage; ZM(ReplyMessage);
//	RequestMessage.merchantID = "v4p225p825184";
//	RequestMessage.merchantReferenceCode = "ABC123";
//	struct ns3__PurchaseTotals  PurchaseTotals; ZM(PurchaseTotals);
//	PurchaseTotals.grandTotalAmount = "150.00";
//	PurchaseTotals.currency = "usd";
//	RequestMessage.purchaseTotals = &PurchaseTotals;
//	struct ns3__Pos oPos; ZM(oPos);
//	oPos.cardPresent = "Y";
//	oPos.entryMode = "swiped";
//	oPos.terminalCapability = "2";
//	oPos.trackData = ";4111111111111111=171220112345678?";
//	RequestMessage.pos = &oPos;
//
//	struct ns3__CCAuthService  AuthService; ZM(AuthService);
//	AuthService.run = "true";
//	AuthService.commerceIndicator = "retail";
//	AuthService.partialAuthIndicator = "true";
//	RequestMessage.ccAuthService =&AuthService;
//	struct ns3__CCCaptureService CaptureService; ZM(CaptureService);
//	CaptureService.run = "true";
//	RequestMessage.ccCaptureService = &CaptureService;
//
//	RequestMessage.thirdPartyCertificationNumber = "123456789012";
//	sprintf(acSysTraceAuditNo,"%u",DataSave1->ulSysTraceAuditNo++);
//	RequestMessage.merchantTransactionIdentifier = acSysTraceAuditNo;
//
//	soap_call___ns1__runTransaction(&osoap, NULL, NULL, &RequestMessage, &ReplyMessage);
//	Rprintf("ReplyMessage.requestID: %s\r\n  	ReplyMessage.decision: %s\r\n  	ReplyMessage.reasonCode: %s\r\n  	ReplyMessage.ccAuthReply->amount: %s\r\n  	ReplyMessage.ccAuthReply->reasonCode: %s\r\n  	ReplyMessage.ccAuthReply->authorizationCode: %s\r\n  	ReplyMessage.ccAuthReply->authorizedDateTime: %s\r\n  	ReplyMessage.ccAuthReply->paymentNetworkTransactionID: %s\r\n  	ReplyMessage.ccCaptureReply->reasonCode: %s\r\n  	ReplyMessage.ccCaptureReply->reconciliationID: %s\r\n  	ReplyMessage.ccCaptureReply->requestDateTime: %s\r\n",
//	ReplyMessage.requestID,
//	ReplyMessage.decision,
//	ReplyMessage.reasonCode,
//	ReplyMessage.ccAuthReply->amount,
//	ReplyMessage.ccAuthReply->reasonCode,
//	ReplyMessage.ccAuthReply->authorizationCode,
//	ReplyMessage.ccAuthReply->authorizedDateTime,
//	ReplyMessage.ccAuthReply->paymentNetworkTransactionID,
//	ReplyMessage.ccCaptureReply->reasonCode,
//	ReplyMessage.ccCaptureReply->reconciliationID,
//	ReplyMessage.ccCaptureReply->requestDateTime);
}

/*
 * \Brief
 * \Param
 * \return
 *
 */
void  setBatchUploadElements( struct _ns1__UploadBatch *RequestMessage)
{

	//
	  Rprintf("Batch UPLOADDDDDDDDDDDDDDDDDDD\r\n");

	  HostID = HOSTID;

	  RequestMessage->batchData->HostID = &HostID;
	  RequestMessage->batchData->TID =DataSave1->udtDeviceConfig.aucTermId;
	  RequestMessage->batchData->MID =DataSave1->udtDeviceConfig.aucMerchantId;
	  RequestMessage->batchData->AC ="011200";		//TODO DE- Acquirer ID
	  RequestMessage->batchData->Versions=	"00000000000000000000000000000";

	  memcpy(CapDate,&DataSave1->udtTransDetail.aucTransDate[4],4);
	  RequestMessage->batchData->CapDate=&CapDate[0];

	  sprintf(Stan,"%ld", DataSave1->ulSysTraceAuditNo);
	  RequestMessage->batchData->STAN = &Stan[0] ;			//DE-11 STAN

	  memset(LTime,0,sizeof(LTime));
	  sprintf(LTime,"%s%s",&DataSave1->udtTransDetail.aucTransDate[2] ,DataSave1->udtTransDetail.aucTransTime);
	  RequestMessage->batchData->LTime=&LTime[0];
	  Rprintf("Timme=%s\r\n Time= %s\r\n",LTime,RequestMessage->batchData->LTime);

	  memcpy(DataSave1->udtTransDetail.aucCurrencyCode,udtCurrencyTable[EGP].ucCurrencyCode,3);
	  RequestMessage->batchData->CC= DataSave1->udtTransDetail.aucCurrencyCode;						//DE-49 Currency code


	  //Field 72 data
	  int ulIdx=0;
	  memset(DataRecord,0,sizeof(DataRecord));

	  int Location=0;
	  for (ulIdx = 0; ulIdx < DataSave1->udtTransStore.ulSavedTransNo; ulIdx++)
	  {
		  if((DataSave1->udtTransStore.audtTrans[ulIdx].udtTansType == VOID &&
			  (DataSave1->udtTransStore.audtTrans[ulIdx].bIsOffline == 0))
				  ||
				((DataSave1->udtTransStore.audtTrans[ulIdx].bIsVoided == FALSE)&&
				(DataSave1->udtTransStore.audtTrans[ulIdx].udtTansType != VOID))
				||
				((DataSave1->udtTransStore.audtTrans[ulIdx].bIsVoided == TRUE)&&
					(DataSave1->udtTransStore.audtTrans[ulIdx].udtTansType != VOID)&&
					 (DataSave1->udtTransStore.audtTrans[ulIdx].bIsOffline == 0)))	//Online
			  {

				  //Construct field
			  sprintf(&DataRecord[Location],"%x%06x%06ld%s*%012d%s%012d%s%s%c%c%c%c%s000000000%d?",DataSave1->udtTransStore.audtTrans[ulIdx].MTI,
																  DataSave1->udtTransStore.audtTrans[ulIdx].ulPCode,
																  DataSave1->udtTransStore.audtTrans[ulIdx].ulSysTraceAuditNo,
																  DataSave1->udtTransStore.audtTrans[ulIdx].PAN,
																  atoi(DataSave1->udtTransStore.audtTrans[ulIdx].aucAmount),
																  DataSave1->udtTransDetail.aucCurrencyCode,
																  atoi(DataSave1->udtTransStore.audtTrans[ulIdx].aucAmount),
																  DataSave1->udtTransDetail.aucCurrencyCode,
																  &DataSave1->udtTransStore.audtTrans[ulIdx].aucTransDate[6],//DD
																  DataSave1->udtTransStore.audtTrans[ulIdx].aucTransDate[4],
																  DataSave1->udtTransStore.audtTrans[ulIdx].aucTransDate[5],//MM
																  DataSave1->udtTransStore.audtTrans[ulIdx].aucTransDate[2],
																  DataSave1->udtTransStore.audtTrans[ulIdx].aucTransDate[3],
																  DataSave1->udtTransStore.audtTrans[ulIdx].aucAuthIdResponse,
																  (!DataSave1->udtTransStore.audtTrans[ulIdx].bIsOffline));

			  }

			  Location= strlen(DataRecord);
		      Rprintf( "\r\nCurrency[F72]=%s\r\n",&DataRecord[0]);
//		  }

	  }
	  RequestMessage->batchData->DATA=&DataRecord[0];

      Rprintf( "\r\nCurrency[F72]=%s\r\n",RequestMessage->batchData->DATA);



}
/*
 * \Brief
 * \Param
 * \return
 *
 */
void  setRequestElements(struct _ns1__ExecuteTransaction *RequestMessage,
						 char* EMVtags,
						 int TagsStrLen,
						 int TXNIndex)
{

		memset(LTime,0,13);
		memset(CapDate,0,5);
		memset(Stan,0,7);
		memset(Temp,0,8);

		  HostID = HOSTID;

		RequestMessage->posModel->HostID = &HostID;
		RequestMessage->posModel->TID =DataSave1->udtDeviceConfig.aucTermId;
		RequestMessage->posModel->MID =DataSave1->udtDeviceConfig.aucMerchantId;
		RequestMessage->posModel->MN =DataSave1->udtDeviceConfig.aaucMerchantName[0];
		memcpy(DataSave1->udtDeviceConfig.aucMCC, "7011",4);
		RequestMessage->posModel->MC = DataSave1->udtDeviceConfig.aucMCC;
	    RequestMessage->posModel->AC ="011200";		//TODO DE- Acquirer ID
	  //Security data TODO

	  RequestMessage->posModel->SecData= "0099000000";
	  RequestMessage->posModel->versions=	"00000000000000000000000000000";
	  RequestMessage->posModel->isSMS =	  0;
	  RequestMessage->posModel->isPOS=   1;

	  Rprintf("DataSave1->udtTransDetail.ulPCode[%x]\r\n",DataSave1->udtTransDetail.ulPCode);
      switch(DataSave1->udtTransDetail.ulPCode)
      {
      	  case PCODE_SALE://      		  //Sale
      	  {
      		  	  //sprintf(MTI ,"%x",MTYPE_FINANCIAL_TRXN_REQUEST);
      		  	  sprintf(MTI ,"%x",DataSave1->udtTransDetail.MTI);
      		  	  Rprintf("MTI=%s\r\n",MTI);
      		  	 // memcpy(RequestMessage->posModel->MTI,Temp,4) ;//&Temp;
      		  	  RequestMessage->posModel->MTI= &MTI[0];

      			  RequestMessage->posModel->CN = DataSave1->udtTransDetail.PAN;

      			  memset(Temp,0,8);
      			  sprintf(Temp ,"%x",DataSave1->udtTransDetail.ulPCode);
      			  Rprintf("PCC=%s\r\n",Temp);
      			  RequestMessage->posModel->PC ="000000";//&Temp;										//DE-3 Processing code

      			  sprintf(LTime,"%s%s",&DataSave1->udtTransDetail.aucTransDate[2] ,DataSave1->udtTransDetail.aucTransTime);
      			  RequestMessage->posModel->LTime=&LTime[0];
      			  Rprintf("Timme=%s\r\n Time= %s\r\n",LTime,RequestMessage->posModel->LTime);

      			  memcpy(CapDate,&DataSave1->udtTransDetail.aucTransDate[4],4);
      			  RequestMessage->posModel->CapDate=&CapDate[0];

      			  sprintf(Stan,"%ld", (DataSave1->ulSysTraceAuditNo));
      			  RequestMessage->posModel->STAN = &Stan[0] ;			//DE-11 STAN
      			  RequestMessage->posModel->PEM	= DataSave1->udtTransDetail.aucPOSEntryMode;			//DE-22 Entry mode

      			  RequestMessage->posModel->FnCode ="200";														//DE-24 Function Code
      			  RequestMessage->posModel->PanSeq	="001";														//TODO
      			  RequestMessage->posModel->ReversalNo ="000000";												//TODO
      			  memcpy(DataSave1->udtTransDetail.aucCurrencyCode,udtCurrencyTable[EGP].ucCurrencyCode,3);
      			  RequestMessage->posModel->CC= DataSave1->udtTransDetail.aucCurrencyCode;						//DE-49 Currency code

      			  // Optional
      			  if(DataSave1->udtTransDetail.aucPOSEntryMode[6]=='5')	//Chip
      			  {
      				  //AMountttt
      				  int TagLen = 0;
      				  char TempBuff[1000];
      				  BYTE *ptr,*pAdd;
      				  memset(TempBuff,0,sizeof(TempBuff));

      				  //Application Name
      				  TagLen = getDnL((BYTE *)EMVtags, TagsStrLen, Tva_AppLabel, &pAdd);
      				  //bcd2asc(DataSave1->udtTransDetail.SelectedAID,(char *)pAdd,(TagLen * 2));
      				memcpy(DataSave1->udtTransDetail.SelectedAID_Name ,(char *)pAdd,TagLen);
      				  Rprintf("AIDDDDDDDDDDDDDDDDDDDDDDDDDDDD=%s\r\n",DataSave1->udtTransDetail.SelectedAID_Name);

      				  //CardHolder Name
      				  TagLen = getDnL((BYTE *)EMVtags, TagsStrLen, Tva_CardholderName, &pAdd);
      				  bcd2asc(DataSave1->udtTransDetail.CardHolderName,(char *)pAdd,(TagLen * 2));
      				  Rprintf("NAMEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE=%s\r\n",DataSave1->udtTransDetail.CardHolderName);

      				  //Amount
      				  TagLen = getDnL((BYTE *)EMVtags, TagsStrLen, T6n_TrmAmountAuthorised, &pAdd);
      				  bcd2asc(DataSave1->udtTransDetail.aucAmount,(char *)pAdd,(TagLen * 2));

      				  //Track 222
      				  memset(Track2,0,sizeof(Track2));
      				  TagLen = getDnL((BYTE *)EMVtags, TagsStrLen, Tvb_Track2EquivalentData, &pAdd);
      				  memset(TempBuff,0,sizeof(TempBuff));
      				  bcd2zasc(TempBuff,(char*)pAdd,(TagLen*2));
      				  if (TempBuff[(TagLen*2)-1] == '?') TempBuff[(TagLen*2)-1] = '\0';
      				  memcpy(Track2,(char*)TempBuff, strlen(TempBuff));
      				  memcpy(DataSave1->udtTransDetail.udtMSRCard.acTrack2,Track2,sizeof(Track2));

          			  RequestMessage->posModel->A = DataSave1->udtTransDetail.aucAmount;					// DE-4 AMOUNT

          			  //Track 222
          			  RequestMessage->posModel->ET= DataSave1->udtTransDetail.udtMSRCard.acTrack2;				//DE-35 Track 2

          		      char * pch;

          		      pch =strtok(Track2,"=?");
          		      memcpy(DataSave1->udtTransDetail.PAN,pch,sizeof(DataSave1->udtTransDetail.PAN));

          		      pch = strtok (NULL, " ,.-");
          		      //Expiry Date
          			  memcpy(DataSave1->udtTransDetail.ExpiryDate,pch,sizeof(DataSave1->udtTransDetail.ExpiryDate));

          			  extern TTrxnRec gsXRec;
          			  int len=trans_UPDATEgsXRec_ICCbL3();
          			  ptr=(U08 *)&gsPOS.TrxnRec.Trns_ICCbL3;
          			  memcpy(ptr, (U08 *)&gsXRec.Trns_ICCbL3, len);
          			  memset(TempBuff,0,sizeof(TempBuff));
          			  memset(TempBuff1,0,sizeof(TempBuff1));
          			  memcpy(TempBuff, (U08 *)&gsXRec.Trns_ICCbL3, len);

          			  bcd2asc(TempBuff1, TempBuff,len*2);

          			  RequestMessage->posModel->EMVTags =TempBuff1;
          			  //
      			  }
      			  else if(DataSave1->udtTransDetail.aucPOSEntryMode[6]=='1')// Manual
      			  {
          			  RequestMessage->posModel->A = DataSave1->udtTransDetail.aucAmount;					// DE-4 AMOUNT
      			  }
      			  else
      			  {
          			  RequestMessage->posModel->A = DataSave1->udtTransDetail.aucAmount;					// DE-4 AMOUNT
      				  RequestMessage->posModel->ET= DataSave1->udtTransDetail.udtMSRCard.acTrack2;				//DE-35 Track 2
      				  RequestMessage->posModel->PinBlock =DataSave1->udtTransDetail.udtMSRCard.CiphertextPinBlock;
      			  }


      	  }
      		  break;
      	  case PCODE_OFFLINE_SALE:
      	  case PCODE_REFUND://      		  //Sale
      	  {
      		  	  //sprintf(MTI ,"%x",MTYPE_FINANCIAL_TRXN_ADVICE);
      		  	sprintf(MTI ,"%x",DataSave1->udtTransStore.audtTrans[TXNIndex].MTI);
      		  	  Rprintf("MTI=%s\r\n",MTI);
      		  	 // memcpy(RequestMessage->posModel->MTI,Temp,4) ;//&Temp;
      		  	  RequestMessage->posModel->MTI= &MTI[0];

      			  RequestMessage->posModel->CN =  DataSave1->udtTransStore.audtTrans[TXNIndex].PAN;

      			  memset(Temp,0,8);
      			  sprintf(Temp ,"%x",DataSave1->udtTransDetail.ulPCode);
      			  Rprintf("PCC=%s\r\n",Temp);
      			  if(DataSave1->udtTransDetail.ulPCode==PCODE_REFUND )
      			  {
          		  	  Rprintf("REFUNDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\r\n");
      				  RequestMessage->posModel->PC ="200000";//&Temp;										//DE-3 Processing code
      			  }
      			  else
      			  {
          		  	  Rprintf("OFFLINEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\r\n");
      				 RequestMessage->posModel->PC ="000000";//&Temp;										//DE-3 Processing code
      			  }

      			  sprintf(LTime,"%s%s",&DataSave1->udtTransStore.audtTrans[TXNIndex].aucTransDate[2] ,
      					  	  	  	   DataSave1->udtTransStore.audtTrans[TXNIndex].aucTransTime);
      			  RequestMessage->posModel->LTime=&LTime[0];
      			  Rprintf("Timme=%s\r\n Time= %s\r\n",LTime,RequestMessage->posModel->LTime);

      			  memcpy(CapDate,&DataSave1->udtTransStore.audtTrans[TXNIndex].aucTransDate[4],4);
      			  RequestMessage->posModel->CapDate=&CapDate[0];

      			  sprintf(Stan,"%ld", (DataSave1->udtTransStore.audtTrans[TXNIndex].ulSysTraceAuditNo));
      			  RequestMessage->posModel->STAN = &Stan[0] ;			//DE-11 STAN
      			  RequestMessage->posModel->PEM	= DataSave1->udtTransStore.audtTrans[TXNIndex].aucPOSEntryMode;			//DE-22 Entry mode

      			  RequestMessage->posModel->FnCode ="200";														//DE-24 Function Code
      			  RequestMessage->posModel->PanSeq	="001";														//TODO
      			  RequestMessage->posModel->ReversalNo ="000000";												//TODO
      			  memcpy(DataSave1->udtTransDetail.aucCurrencyCode,udtCurrencyTable[EGP].ucCurrencyCode,3);
      			  RequestMessage->posModel->CC= DataSave1->udtTransDetail.aucCurrencyCode;						//DE-49 Currency code

      			  // Optional
      			  if(DataSave1->udtTransStore.audtTrans[TXNIndex].aucPOSEntryMode[6]=='5')	//Chip
      			  {
      				  //AMountttt
      				  int TagLen = 0;
      				  char TempBuff[1000];
      				  BYTE *ptr,*pAdd;
      				  memset(TempBuff,0,sizeof(TempBuff));

      				  //TagLen = getDnL((BYTE *)EMVtags, TagsStrLen, T6n_TrmAmountAuthorised, &pAdd);
//      				  memset(TempBuff,0,sizeof(TempBuff));

      				 // bcd2asc(DataSave1->udtTransStore.audtTrans[TXNIndex].aucAmount,(char *)pAdd,(TagLen * 2));


//      				  memset(Amount,0,sizeof(Amount));
//      				  asc_to_str(Amount,(char*)TempBuff, strlen((char*)TempBuff));
          			  RequestMessage->posModel->A = DataSave1->udtTransStore.audtTrans[TXNIndex].aucAmount;					// DE-4 AMOUNT
//          			  DataSave1->udtTransDetail.aucAmount=&Amount[0];
          			  //Track 222
//          			  memset(Track2,0,sizeof(Track2));
//
//          			  TagLen = getDnL((BYTE *)EMVtags, TagsStrLen, Tvb_Track2EquivalentData, &pAdd);
//          			  memset(TempBuff,0,sizeof(TempBuff));
//          			  bcd2zasc(TempBuff,(char*)pAdd,(TagLen*2));
//          			  if (TempBuff[(TagLen*2)-1] == '?') TempBuff[(TagLen*2)-1] = '\0';
//          			  memcpy(Track2,(char*)TempBuff, strlen(TempBuff));
//          			  memcpy(DataSave1->udtTransStore.audtTrans[TXNIndex].udtMSRCard.acTrack2,Track2,sizeof(Track2));
          			  RequestMessage->posModel->ET= DataSave1->udtTransStore.audtTrans[TXNIndex].udtMSRCard.acTrack2;				//DE-35 Track 2

//            		    char * pch;
//
//            		    pch =strtok(DataSave1->udtTransStore.audtTrans[TXNIndex].udtMSRCard.acTrack2,"=?");
//            		    memcpy(DataSave1->udtTransStore.audtTrans[TXNIndex].PAN,pch,sizeof(DataSave1->udtTransStore.audtTrans[TXNIndex].PAN));

//          			  extern TTrxnRec gsXRec;
//          			  int len=trans_UPDATEgsXRec_ICCbL3();
//          			  ptr=(U08 *)&gsPOS.TrxnRec.Trns_ICCbL3;
//          			  memcpy(ptr, (U08 *)&gsXRec.Trns_ICCbL3, len);
//          			  memset(TempBuff,0,sizeof(TempBuff));
//          			  memset(TempBuff1,0,sizeof(TempBuff1));
//          			  memcpy(TempBuff, (U08 *)&gsXRec.Trns_ICCbL3, len);
//
//          			  bcd2asc(TempBuff1, TempBuff,len*2);

          			  RequestMessage->posModel->EMVTags =DataSave1->udtTransStore.audtTrans[TXNIndex].EMVTags;//TempBuff1;
          			  //
      			  }
      			  else if(DataSave1->udtTransStore.audtTrans[TXNIndex].aucPOSEntryMode[6]=='1')// Manual
      			  {
          			  RequestMessage->posModel->A = DataSave1->udtTransStore.audtTrans[TXNIndex].aucAmount;					// DE-4 AMOUNT
      			  }
      			  else
      			  {
          			  RequestMessage->posModel->A = DataSave1->udtTransStore.audtTrans[TXNIndex].aucAmount;					// DE-4 AMOUNT
      				  RequestMessage->posModel->ET= DataSave1->udtTransStore.audtTrans[TXNIndex].udtMSRCard.acTrack2;				//DE-35 Track 2
      				//  RequestMessage->posModel->PinBlock =DataSave1->udtTransStore.audtTrans[TXNIndex].udtMSRCard.CiphertextPinBlock;
      			  }


      	  }
      	  break;
      	  case PCODE_VOID_SALE://      		  //Void
      	  {
      		  	  Rprintf("VoiDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\r\n");
      		  	  //sprintf(MTI ,"%x",MTYPE_REVERSAL_REQUEST);
      		  	sprintf(MTI ,"%x",DataSave1->udtTransStore.audtTrans[TXNIndex].MTI);
      		  	  Rprintf("MTI=%s\r\n",MTI);
      		  	 // memcpy(RequestMessage->posModel->MTI,Temp,4) ;//&Temp;
      		  	  RequestMessage->posModel->MTI=&MTI[0];

      			  RequestMessage->posModel->CN =  DataSave1->udtTransStore.audtTrans[TXNIndex].PAN;

      			  memset(Temp,0,8);
      			  sprintf(Temp ,"%x",DataSave1->udtTransDetail.ulPCode);
      			  Rprintf("PCC=%s\r\n",Temp);
      			  RequestMessage->posModel->PC ="000000";//&Temp;										//DE-3 Processing code

      			  sprintf(LTime,"%s%s",&DataSave1->udtTransStore.audtTrans[TXNIndex].aucTransDate[2] ,
      					  	  	  	   DataSave1->udtTransStore.audtTrans[TXNIndex].aucTransTime);
      			  RequestMessage->posModel->LTime=&LTime[0];
      			  Rprintf("Timme=%s\r\n Time= %s\r\n",LTime,RequestMessage->posModel->LTime);

      			  memcpy(CapDate,&DataSave1->udtTransStore.audtTrans[TXNIndex].aucTransDate[4],4);
      			  RequestMessage->posModel->CapDate=&CapDate[0];

      			  sprintf(Stan,"%ld", (DataSave1->udtTransStore.audtTrans[TXNIndex].ulSysTraceAuditNo));
      			  RequestMessage->posModel->STAN = &Stan[0] ;			//DE-11 STAN
      			  RequestMessage->posModel->PEM	= DataSave1->udtTransStore.audtTrans[TXNIndex].aucPOSEntryMode;			//DE-22 Entry mode

      			  RequestMessage->posModel->FnCode ="400";														//DE-24 Function Code
     			  //TODO Field 25
      			   RequestMessage->posModel->MsgReason ="4000";	//User cancel

      			  RequestMessage->posModel->PanSeq	="001";														//TODO
      			  RequestMessage->posModel->ReversalNo ="000000";												//TODO Field 37 , Change it

      			  //TODO Field 38
      			  RequestMessage->posModel->AuthCode=DataSave1->udtTransStore.audtTrans[TXNIndex].aucAuthIdResponse;

    			  memcpy(DataSave1->udtTransDetail.aucCurrencyCode,udtCurrencyTable[EGP].ucCurrencyCode,3);
    			  RequestMessage->posModel->CC= DataSave1->udtTransDetail.aucCurrencyCode;						//DE-49 Currency code

    			  //TODO Field 56	(Original MSG ID, Org STAN,Orginal TIme,Date)
    			  RequestMessage->posModel->OriginalData = DataSave1->udtTransStore.audtTrans[TXNIndex].aucAddtionalData;

      			  // Optional
      			  if(DataSave1->udtTransStore.audtTrans[TXNIndex].aucPOSEntryMode[6]=='5')	//Chip
      			  {
      				  //AMountttt
      				  int TagLen = 0;
      				  char TempBuff[1000];
      				  BYTE *ptr,*pAdd;
      				  memset(TempBuff,0,sizeof(TempBuff));

      				  TagLen = getDnL((BYTE *)EMVtags, TagsStrLen, T6n_TrmAmountAuthorised, &pAdd);
//      				  memset(TempBuff,0,sizeof(TempBuff));

      				  bcd2asc(DataSave1->udtTransStore.audtTrans[TXNIndex].aucAmount,(char *)pAdd,(TagLen * 2));


//      				  memset(Amount,0,sizeof(Amount));
//      				  asc_to_str(Amount,(char*)TempBuff, strlen((char*)TempBuff));
          			  RequestMessage->posModel->A = DataSave1->udtTransStore.audtTrans[TXNIndex].aucAmount;					// DE-4 AMOUNT
//          			  DataSave1->udtTransDetail.aucAmount=&Amount[0];
          			  //Track 222
          			  memset(Track2,0,sizeof(Track2));

          			  TagLen = getDnL((BYTE *)EMVtags, TagsStrLen, Tvb_Track2EquivalentData, &pAdd);
          			  memset(TempBuff,0,sizeof(TempBuff));
          			  bcd2zasc(TempBuff,(char*)pAdd,(TagLen*2));
          			  if (TempBuff[(TagLen*2)-1] == '?') TempBuff[(TagLen*2)-1] = '\0';
          			  memcpy(Track2,(char*)TempBuff, strlen(TempBuff));
          			  memcpy(DataSave1->udtTransStore.audtTrans[TXNIndex].udtMSRCard.acTrack2,
          					  Track2,
          					  sizeof(Track2));
          			  RequestMessage->posModel->ET= Track2;				//DE-35 Track 2

          			  extern TTrxnRec gsXRec;
          			  int len=trans_UPDATEgsXRec_ICCbL3();
          			  ptr=(U08 *)&gsPOS.TrxnRec.Trns_ICCbL3;
          			  memcpy(ptr, (U08 *)&gsXRec.Trns_ICCbL3, len);
          			  memset(TempBuff,0,sizeof(TempBuff));
          			  memset(TempBuff1,0,sizeof(TempBuff1));
          			  memcpy(TempBuff, (U08 *)&gsXRec.Trns_ICCbL3, len);

          			  bcd2asc(TempBuff1, TempBuff,len*2);

          			  RequestMessage->posModel->EMVTags =TempBuff1;
          			  //
      			  }
      			  else if(DataSave1->udtTransStore.audtTrans[TXNIndex].aucPOSEntryMode[6]=='1')// Manual
      			  {
          			  RequestMessage->posModel->A = DataSave1->udtTransStore.audtTrans[TXNIndex].aucAmount;					// DE-4 AMOUNT
      			  }
      			  else
      			  {
          			  RequestMessage->posModel->A = DataSave1->udtTransStore.audtTrans[TXNIndex].aucAmount;					// DE-4 AMOUNT
      				  RequestMessage->posModel->ET= DataSave1->udtTransStore.audtTrans[TXNIndex].udtMSRCard.acTrack2;				//DE-35 Track 2
      				//  RequestMessage->posModel->PinBlock =DataSave1->udtTransStore.audtTrans[TXNIndex].udtMSRCard.CiphertextPinBlock;
      			  }


      	  }
      	  break;
      	  case PCODE_LOGIN:		//Login
      	  {

    	  	  sprintf(MTI ,"%x",MTYPE_SETTLEMENT_REQUEST);
    	  	  Rprintf("MTI=%s\r\n",MTI);
    	  	 // memcpy(RequestMessage->posModel->MTI,Temp,4) ;//&Temp;
    	  	  RequestMessage->posModel->MTI=&MTI[0];


    		  sprintf(LTime,"%s%s",&DataSave1->udtTransDetail.aucTransDate[2] ,DataSave1->udtTransDetail.aucTransTime);
    		  RequestMessage->posModel->LTime=&LTime[0];
    		  Rprintf("Timme=%s\r\n Time= %s\r\n",LTime,RequestMessage->posModel->LTime);

    		  memcpy(CapDate,&DataSave1->udtTransDetail.aucTransDate[4],4);
    		  RequestMessage->posModel->CapDate=&CapDate[0];

    		  sprintf(Stan,"%ld", (DataSave1->ulSysTraceAuditNo));
    		  RequestMessage->posModel->STAN = &Stan[0] ;			//DE-11 STAN

    		  RequestMessage->posModel->FnCode ="821";														//DE-24 Function Code

      	  }
      	  break;
      	  case PCODE_SETTLEMENT:				//Settlement
      	  {
    	  	  sprintf(MTI ,"%x",MTYPE_BATCH_UPLOAD_ADVICE);
    	  	  Rprintf("MTI=%s\r\n",MTI);
    	  	 // memcpy(RequestMessage->posModel->MTI,Temp,4) ;//&Temp;
    	  	  RequestMessage->posModel->MTI=&MTI[0];


    		  sprintf(LTime,"%s%s",&DataSave1->udtTransDetail.aucTransDate[2] ,DataSave1->udtTransDetail.aucTransTime);
    		  RequestMessage->posModel->LTime=&LTime[0];
    		  Rprintf("Timme=%s\r\n Time= %s\r\n",LTime,RequestMessage->posModel->LTime);

    		  memcpy(CapDate,&DataSave1->udtTransDetail.aucTransDate[4],4);
    		  RequestMessage->posModel->CapDate=&CapDate[0];

    		  sprintf(Stan,"%ld", (DataSave1->ulSysTraceAuditNo));
    		  RequestMessage->posModel->STAN = &Stan[0] ;			//DE-11 STAN

    		  RequestMessage->posModel->FnCode ="500";														//DE-24 Function Code

  			  memcpy(DataSave1->udtTransDetail.aucCurrencyCode,udtCurrencyTable[EGP].ucCurrencyCode,3);
  			  RequestMessage->posModel->CC= DataSave1->udtTransDetail.aucCurrencyCode;						//DE-49 Currency code

  			  //************************************* Summery**************************************************



  			memset(NetAMount,0,sizeof(NetAMount));
  			unsigned int ulIdx=0;
  			DebitAmount =0;
  			CreditAmount=0;
  			DebitNo=0;
  			CreditNo=0;

			  for (ulIdx = 0; ulIdx < DataSave1->udtTransStore.ulSavedTransNo; ulIdx++)
			  {
				  Rprintf("MSG TYpe= %d\r\n ISOffline=%d\r\n ISVoided =%d,, Amount =%s\r\n",DataSave1->udtTransStore.audtTrans[ulIdx].udtTansType,
						  	  	  	  	  	 	 	 	 	 DataSave1->udtTransStore.audtTrans[ulIdx].bIsOffline,
						  	  	  	  	  	 	 	DataSave1->udtTransStore.audtTrans[ulIdx].bIsVoided,
						  	  	  	  	  	 	 	DataSave1->udtTransStore.audtTrans[ulIdx].aucAmount);
				  if(DataSave1->udtTransStore.audtTrans[ulIdx].ulActionCode==0)
				  {
					 // if(DataSave1->udtTransStore.audtTrans[ulIdx].udtTansType == VOID)
					  if(DataSave1->udtTransStore.audtTrans[ulIdx].udtTansType == VOID )
					  {
						  if(DataSave1->udtTransStore.audtTrans[ulIdx].bIsOffline == 0)		//Online
						  {
							  saleAmount= saleAmount - atoi(DataSave1->udtTransStore.audtTrans[ulIdx].aucAmount);
							  CreditAmount=CreditAmount + atoi(DataSave1->udtTransStore.audtTrans[ulIdx].aucAmount);
							  CreditNo++;
						  }
					  }
					 // else if(DataSave1->udtTransStore.audtTrans[ulIdx].udtTansType == REFUND)
					  else if(DataSave1->udtTransStore.audtTrans[ulIdx].udtTansType == REFUND)
					  {
						  if(DataSave1->udtTransStore.audtTrans[ulIdx].bIsVoided == FALSE)
						  {

							  saleAmount= saleAmount - atoi(DataSave1->udtTransStore.audtTrans[ulIdx].aucAmount);
							  CreditAmount=CreditAmount + atoi(DataSave1->udtTransStore.audtTrans[ulIdx].aucAmount);
							  CreditNo++;
						  }
					  }
					  else if(DataSave1->udtTransStore.audtTrans[ulIdx].udtTansType == PURCH)
					  {
						  if(DataSave1->udtTransStore.audtTrans[ulIdx].bIsOffline == 0)		//Online
						  {
							  DebitAmount = DebitAmount +atoi(DataSave1->udtTransStore.audtTrans[ulIdx].aucAmount);;
							  saleAmount= saleAmount + atoi(DataSave1->udtTransStore.audtTrans[ulIdx].aucAmount);
							  DebitNo++;
						  }
						  else
						  {
							  if(DataSave1->udtTransStore.audtTrans[ulIdx].bIsVoided == FALSE)
							  {

								  saleAmount= saleAmount - atoi(DataSave1->udtTransStore.audtTrans[ulIdx].aucAmount);
								  CreditAmount=CreditAmount + atoi(DataSave1->udtTransStore.audtTrans[ulIdx].aucAmount);
								  CreditNo++;
							  }

						  }
					  }

				  }
			  }
			  Rprintf("CreditAmount=%d  CreditNo =%d\r\n DebitAmount= %d  DebitNo=%d\r\n",CreditAmount,CreditNo,DebitAmount,DebitNo);

			  Defutlt=0;
			  //54
			  RequestMessage->posModel->SettlementData->AddAmounts ="0099818C000000000000";
			  //62
			  RequestMessage->posModel->SettlementData->RedemNo ="0000000000";
			  //Field 74,75,83,84,86,87,89
			  RequestMessage->posModel->SettlementData->DNo=&DebitNo;
			  RequestMessage->posModel->SettlementData->DAmont  =&DebitAmount;

			  RequestMessage->posModel->SettlementData->DRevNo=&Defutlt;
			  RequestMessage->posModel->SettlementData->DRevAmont=&Defutlt;

			  RequestMessage->posModel->SettlementData->CrdRevAmont =&Defutlt;
			  RequestMessage->posModel->SettlementData->CRevNo =&Defutlt;

			  RequestMessage->posModel->SettlementData->PayNo		=&Defutlt;
			  RequestMessage->posModel->SettlementData->PayReverseNo=&Defutlt;


			  //Field 76 Total TXN
			  RequestMessage->posModel->SettlementData->CredNO = &CreditNo;
			  RequestMessage->posModel->SettlementData->CrdAmont=&CreditAmount;

			  //Filed 97 and 88 ,Total Amount
			  //saleAmount
			  if(saleAmount>=0)
			  {
				  sprintf(NetAMount,"C%016u",saleAmount);
			  }
			  else
			  {
				  saleAmount= -1*saleAmount;
				  sprintf(NetAMount,"D%016u",saleAmount);

			  }

			  RequestMessage->posModel->SettlementData->NetAmount =&NetAMount[0];


      	  }
      	  break;
    	  default:
    		  break;


      }
      Rprintf("\r\nPAN[F2] =%s \r\n"
    		  "P Code[F3] =%d \r\n"
    		  "Amount[F4] =%s \r\n"
    		  "STAN [F11] =%d \r\n"
    		  "Time [F12] =%s \r\n"
    		  "Date [F13] =%s\r\n"
    		  "Expiry [F14]= %s\r\n"
    		  "Entry [F22]= %s\r\n"
    		  "SRV [F26]= %s\r\n"
    		  "Track2[F35]=%s\r\n"
    		  "RRN [F37]=%s\r\n"
    		  "TID [F41]=%s\r\n"
    		  "MerchantID[F42]=%s\r\n"
    		  "MerchantName[F43]=%s\r\n"
    		  "Currency[F49]=%s",DataSave1->udtTransDetail.PAN,
    		  	  	  	  	  	    DataSave1->udtTransDetail.ulPCode,
    		  	  	  	  	  	   DataSave1->udtTransDetail.aucAmount,
    		  	  	  	  	  	   DataSave1->ulSysTraceAuditNo,
    		  	  	  	  	  	   DataSave1->udtTransDetail.aucTransTime,
    		  	  	  	  	  	   DataSave1->udtTransDetail.aucTransDate,
    		  	  	  	  	  	   DataSave1->udtTransDetail.ExpiryDate,
    		  	  	  	  	  	   DataSave1->udtTransDetail.aucPOSEntryMode,
    		  	  	  	  	  	   DataSave1->udtDeviceConfig.aucMCC,
    		  	  	  	  	  	   RequestMessage->posModel->ET,
    		  	  	  	  	  	   DataSave1->udtTransDetail.aucRtrvalRefNo,
    		  	  	  	  	  	   DataSave1->udtDeviceConfig.aucTermId,
    		  	  	  	  	  	   DataSave1->udtDeviceConfig.aucMerchantId,
    		  	  	  	  	  	   DataSave1->udtDeviceConfig.aaucMerchantName[0],
    		  	  	  	  	  	   DataSave1->udtTransDetail.aucCurrencyCode);

}
/*
 * \Brief
 * \Param
 * \return
 *
 */
int gsoap_BatchUpload()
{

	  Rprintf("gsoap\r\n");
	  struct soap osoap;

	  soap_init2(&osoap,SOAP_IO_FLUSH, SOAP_IO_FLUSH);
	  	osoap.max_keep_alive= 100; // at most 100 calls per keep-alive session
	  	osoap.send_timeout = 2;
	  	osoap.recv_timeout= 60;	//Heba changed to 20
	  	osoap.connect_timeout= 2;

	  Rprintf("soap_set_namespaces\r\n");

	  soap_set_namespaces(&osoap, namespaces);

	  struct _ns1__UploadBatchResponse ReplyMessage;
	  ZM(ReplyMessage);

	  struct _ns1__UploadBatch UpLoadBatchRequest;
	  ZM(UpLoadBatchRequest);
	  struct ns4__BatchUploadRequest batchData;
	  ZM(batchData);
	  UpLoadBatchRequest.batchData= &batchData;
	  setBatchUploadElements(&UpLoadBatchRequest);

	  vidIncSTAN();
	  char EndPoint [50];
	  memset(EndPoint,0,sizeof(EndPoint));
	  sprintf(EndPoint,"http://%s:%s/mPOSHosting.svc/soap",DataSave1->udtConnSettings.aucServerIP,DataSave1->udtConnSettings.aucServerPort);

	    if((soap_call___ns1__UploadBatch(&osoap,
	    		EndPoint,
	    		"http://tempuri.org/ImPOSService/UploadBatch",
	  		  	 &UpLoadBatchRequest,
	  		  	  &ReplyMessage)<0 ) ||
	  		  	 (osoap.status !=200))
	    {
	   		 return -1;
	   	}
	    Rprintf("ReplyMessage.ExecuteTransactionResult->Code: %d,\r\n "
	    		"ReplyMessage.ExecuteTransactionResult->Message: %d\r\n",
	            *ReplyMessage.UploadBatchResult->Code,
	            osoap.status);


	    DataSave1->udtTransDetail.ulActionCode = *ReplyMessage.UploadBatchResult->Code;
	    DataSave1->ulSysTraceAuditNo = *ReplyMessage.UploadBatchResult->LastStan;

	    return *ReplyMessage.UploadBatchResult->Code;

}
/*
 * \Brief
 * \Param
 * \return
 *
 */
int OfflineTXN(char * EMVTags, int TagsStrLen)
{
	  //AMountttt
	  int TagLen = 0;
	  char TempBuff[1000];
	  BYTE *ptr,*pAdd;
	  memset(TempBuff,0,sizeof(TempBuff));

	  //Application Name
	  TagLen = getDnL((BYTE *)EMVTags, TagsStrLen, Tva_AppLabel, &pAdd);
	  //bcd2asc(DataSave1->udtTransDetail.SelectedAID,(char *)pAdd,(TagLen * 2));
	  memcpy(DataSave1->udtTransDetail.SelectedAID_Name ,(char *)pAdd,TagLen);
	  Rprintf("AIDDDDDDDDDDDDDDDDDDDDDDDDDDDD=%s\r\n",DataSave1->udtTransDetail.SelectedAID_Name);

	  //CardHolder Name
	  TagLen = getDnL((BYTE *)EMVTags, TagsStrLen, Tva_CardholderName, &pAdd);
	  bcd2asc(DataSave1->udtTransDetail.CardHolderName,(char *)pAdd,(TagLen * 2));
	  Rprintf("NAMEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE=%s\r\n",DataSave1->udtTransDetail.CardHolderName);

	  //Amount
	  TagLen = getDnL((BYTE *)EMVTags, TagsStrLen, T6n_TrmAmountAuthorised, &pAdd);
	  bcd2asc(DataSave1->udtTransDetail.aucAmount,(char *)pAdd,(TagLen * 2));

	  //Track 222
	  memset(Track2,0,sizeof(Track2));
	  TagLen = getDnL((BYTE *)EMVTags, TagsStrLen, Tvb_Track2EquivalentData, &pAdd);
	  memset(TempBuff,0,sizeof(TempBuff));
	  bcd2zasc(TempBuff,(char*)pAdd,(TagLen*2));
	  if (TempBuff[(TagLen*2)-1] == '?') TempBuff[(TagLen*2)-1] = '\0';
	  memcpy(Track2,(char*)TempBuff, strlen(TempBuff));
	  memcpy(DataSave1->udtTransDetail.udtMSRCard.acTrack2,Track2,sizeof(Track2));


      char * pch;
      //PAN
      pch =strtok(Track2,"=?");
      memcpy(DataSave1->udtTransDetail.PAN,pch,sizeof(DataSave1->udtTransDetail.PAN));

      pch = strtok (NULL, " ,.-");
      //Expiry Date
	  memcpy(DataSave1->udtTransDetail.ExpiryDate,pch,sizeof(DataSave1->udtTransDetail.ExpiryDate));
	  extern TTrxnRec gsXRec;
	  int len=trans_UPDATEgsXRec_ICCbL3();
	  ptr=(U08 *)&gsPOS.TrxnRec.Trns_ICCbL3;
	  memcpy(ptr, (U08 *)&gsXRec.Trns_ICCbL3, len);
	  memset(TempBuff,0,sizeof(TempBuff));
	  memset(TempBuff1,0,sizeof(TempBuff1));
	  memcpy(TempBuff, (U08 *)&gsXRec.Trns_ICCbL3, len);

	  bcd2asc(TempBuff1, TempBuff,len*2);
	  //Emv Tage Field 55
	  memcpy(DataSave1->udtTransDetail.EMVTags ,TempBuff1,len*2);

		ghResponseCode = 0x3030;
		gwResponseCode=0x3030; // Always approve
		gsXRec.ResponseCode=0x3030;

}
/*
 * \Brief
 * \Param
 * \return
 *
 */
int gsoap_NBE(char * EMVTags, int TagsStrLen,int TXNIndex)
{

   BYTE *ptr, *pAdd, *pPtr;
   char TempBuff[1000];
   int TagLen = 0;

  Rprintf("gsoap\r\n");
  struct soap osoap;
//  soap_init(&osoap);
  soap_init2(&osoap,SOAP_IO_FLUSH, SOAP_IO_FLUSH);
  	osoap.max_keep_alive= 100; // at most 100 calls per keep-alive session
  	osoap.send_timeout = 2;
  	osoap.recv_timeout= 60;	//Heba changed to 20
  	osoap.connect_timeout= 2;

  Rprintf("soap_set_namespaces\r\n");

  soap_set_namespaces(&osoap, namespaces);


  TagLen = getDnL((BYTE *)EMVTags, TagsStrLen, T6n_TrmAmountAuthorised, &pAdd);


  extern TTrxnRec gsXRec;
//  int len=trans_UPDATEgsXRec_ICCbL3();
//  ptr=(U08 *)&gsPOS.TrxnRec.Trns_ICCbL3;
//  memcpy(ptr, (U08 *)&gsXRec.Trns_ICCbL3, len);
//  memset(TempBuff,0,sizeof(TempBuff));
//  memset(TempBuff1,0,sizeof(TempBuff1));
//  memcpy(TempBuff, (U08 *)&gsXRec.Trns_ICCbL3, len);



	  Rprintf("EMVTags: %s\r\n",TempBuff1);
	  struct _ns1__ExecuteTransaction RequestMessage;
	  ZM(RequestMessage);

	  struct _ns1__ExecuteTransactionResponse ReplyMessage;
	  ZM(ReplyMessage);

	  struct ns4__TransactionModel posModel;
	  ZM(posModel);

	  RequestMessage.posModel = &posModel;

	  if(DataSave1->udtTransDetail.ulPCode == PCODE_SETTLEMENT)
	  {
		  struct ns4__settlementData SettlementData;
		  ZM(SettlementData);
		  RequestMessage.posModel->SettlementData = &SettlementData;
	   }



	  //Set Request Parameters
	  setRequestElements(&RequestMessage,EMVTags,TagsStrLen,TXNIndex);		//Heba

	  //Increment Stan
	  vidIncSTAN();
	  //Send Request

	  char EndPoint [50];
	  memset(EndPoint,0,sizeof(EndPoint));
	  sprintf(EndPoint,"http://%s:%s/mPOSHosting.svc/soap",DataSave1->udtConnSettings.aucServerIP,DataSave1->udtConnSettings.aucServerPort);
	 if( (soap_call___ns1__ExecuteTransaction(&osoap,
			 EndPoint,
			  "http://tempuri.org/ImPOSService/ExecuteTransaction",
			 &RequestMessage,
			 &ReplyMessage)<0 ) ||
			 (osoap.status !=200))
	 {
		 DataSave1->udtTransDetail.ulActionCode = -1;
		 return -1;
	 }


	  Rprintf("ReplyMessage.ExecuteTransactionResult->Code: %d,"
			  "\r\n ReplyMessage.ExecuteTransactionResult->Message: %d\r\n",
			  atoi(ReplyMessage.ExecuteTransactionResult->Code),
			  osoap.status);

  if(strncmp(ReplyMessage.ExecuteTransactionResult->Code, "000",3)==0)
  {
	  //In case of success
		memset(TempBuff,0,sizeof(TempBuff));

	  	int AppBufLen =strlen(ReplyMessage.ExecuteTransactionResult->EMVResponse);
		asc2bcd(TempBuff, ReplyMessage.ExecuteTransactionResult->EMVResponse, (AppBufLen));
		AppBufLen=AppBufLen/2;

		TagLen = getDnL(TempBuff, AppBufLen,T2a_ARC , &pAdd);

		DEBUG_BUF((pAdd,TagLen));
		if(TagLen>0)
		{
			pgbAppBufUpdate(T2a_ARC, TagLen, pAdd);
		}

		TagLen=0;
		TagLen = getDnL(TempBuff, AppBufLen,Tvb_IAD , &pAdd);

		DEBUG_BUF((pAdd,TagLen));
		if(TagLen>0)
		{
			pgbAppBufUpdate(Tvb_IAD, TagLen, pAdd);
		}

		TagLen=0;
		TagLen = getDnL(TempBuff, AppBufLen,Ttt_IssuerScript1 , &pAdd);

		DEBUG_BUF((pAdd,TagLen));
		if(TagLen>0)
		{
			pgbAppBufUpdate(Ttt_IssuerScript1, TagLen, pAdd);
		}

		TagLen=0;
		TagLen = getDnL(TempBuff, AppBufLen,Ttt_IssuerScript2 , &pAdd);

		DEBUG_BUF((pAdd,TagLen));
		if(TagLen>0)
		{
			pgbAppBufUpdate(Ttt_IssuerScript2, TagLen, pAdd);
		}
	    ghResponseCode = 0x3030;
	    gwResponseCode=0x3030; // Always approve
	    gsXRec.ResponseCode=0x3030;

	    memcpy(DataSave1->udtTransDetail.aucRtrvalRefNo ,
	    		ReplyMessage.ExecuteTransactionResult->RRN,
	    		sizeof(DataSave1->udtTransDetail.aucRtrvalRefNo));

	    memcpy(DataSave1->udtTransDetail.aucAuthIdResponse ,
	    		ReplyMessage.ExecuteTransactionResult->AuthCode,
	    		sizeof(DataSave1->udtTransDetail.aucAuthIdResponse));


  }
  else
  {
	    ghResponseCode = 0x3531;
	    gwResponseCode=0x3531; // Always approve
	    gsXRec.ResponseCode=0x3531;
  }

  DataSave1->udtTransDetail.ulActionCode =atoi(ReplyMessage.ExecuteTransactionResult->Code);

  return atoi(ReplyMessage.ExecuteTransactionResult->Code);

}

