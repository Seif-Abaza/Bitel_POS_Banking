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
#include "Receipt.h"
#include "Printer.h"
#include "Utilities.h"
#include "./resources/ReceiptLogo.h"
#include "./resources/Msgs.h"
#include "emv/EMV_App.h"	//heba
#include "emv/EMV.h"	//heba

/******************************************************************************/
/* TYPES                                                                      */
/******************************************************************************/
typedef struct
{
  enuTransType udtTansType;
  enuVendType udtVendType;
  ulong ulTotAmnt;
  ulong ulTotTrns;
}strSumTransItem;

typedef struct
{
  uchar ucLogLen;
  strSumTransItem audtSumTransItem[20];
}strSumTransLog;
/******************************************************************************/
/* CONSTANTS, MACROS                                                          */
/******************************************************************************/
#define DEV_DEBUG  0

strSumTransLog udtSumTransLog;
/******************************************************************************/
/* PRIVATE VARIABLES                                                          */
/******************************************************************************/
//static bool bIsRePrint = FALSE;.
  ulong ulStLine,ulStLine1,ulStLine2;
  uchar ucPageNo =0;
/******************************************************************************/
/* PRIVATE FUNCTIONS PROTOTYPES                                               */
/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/
  uchar ucPrinterWorking(uchar ucPageNo, ulong ulFormFeedLine, bool bIsAraLang)
  {

    int IPrntSt;

    SET_FORMFEED(ulFormFeedLine);
    CHAR_FILL1[ucPageNo] = 0xff; 
    PRINTING_NEED=0xff;
    if ((bIsAraLang)&&(udtAppLang == L_ARABIC))
      USE_PRTFONT((void *)Font12x24, ON); 

    while ((IPrntSt = (PRINTER_WORKING() & 0xFF))!=0x60)
    {
      if (IPrntSt == 0x35) {
        DisplayMsgBox(S_PRNT, INS_PAPR,0, 0);
        PRINTING_NEED = 0;
        while ((PAPER_READ() & 0xFF)!= 0xFF)
        {
          // Printer Off-Line?
          if ((ONLINE_READ() & 0xFF) != 0xFF)
          {
            MAKEONLINE();
          }
        }
        DisplayMsgBox(S_PRNT, S_PLZ_WAIT,0, 0);
        PRINTING_NEED = 0xff;
      }
    }
    if ((bIsAraLang)&&(udtAppLang == L_ARABIC))
      USE_PRTFONT((void*)Font12x24,OFF);
  }

/*
 * ************************************************************************************************
 *\Brief
 *\Param
 *\Return
 *****************************************************************************************************
 */
  uchar vidChkPrntPageSize(ulong line)
  {

    if (line > MAX_PAGE_SIZE)
    {
      DisplayMsgBox(S_PRNT, S_PLZ_WAIT,0, 0);
      Rprintf("No of Lines :%d\r\n",line);
      ucPrinterWorking(0,line,TRUE);
      InitializePrinterParam();
      return  0;
    }

    return line;
  }
 /*****************************************************************************
  * \brief Print Logo on Receipt,
  *        You can generate the graphics data by using utility 
  *        “Bitel Conversion Utility” to convert a monochrome bmp image 
  *        to display data,Image width must be exceed 384 pixel
  * 
  * \param [in] Data - Pointer to Buffer of the logo data
  * 
  * \return  void
  * 
  * ******************************************************************************
  */
int PrintLogo( const unsigned char Data[],int pageNo,int startline)
{

    int  n, line, width, height;

    //InitializePrinterParam();

    line = 0;

    // Push current line
    n = line;

    //ASSIGN_GRAPHICS(pGrBuf, 0);                       // Assign the start address of grahic image information (0 to 4)
    ASSIGN_GRAPHICS((char *)&Data[0] ,0);
    ATTB_BUFFER1[0][n][0] = PRT_GRAPHICS_MODE + 0;  // Fill graphics prinitng required data
    // Input graphic image data


    height=((unsigned short)Data[2] << 8) | (unsigned short)Data[3];    /* Image Height */
    // Calculate next line
    n += (height+23) / 24;
    line = n;

    line++;

    return line;
}
/*
 * ************************************************************************************************
 *\Brief
 *\Param
 *\Return
 *****************************************************************************************************
 */
ushort usPrntCardHoldrName(ushort usline)
{

  int i;
  ushort line;
  char item[33], msg[33];


  line = usline;
  if (udtAppLang == L_ARABIC)
  {
#if DEV_DEBUG
  OutputReceiptData(0,line);
#endif
    ucPrinterWorking(0,line,TRUE);

    line = 0;
    for (i = 0; i < 5; i++) CHAR_FILL1[i] = 0x00;
    memset((char *)&CHAR_BUFFER1[0][0][0], 0x00, 5*PRT_LINES_PER_PAGE*PRT_CHARACTERS_PER_LINE);
    memset((char *)&ATTB_BUFFER1[0][0][0], 0x00, 5*PRT_LINES_PER_PAGE*PRT_CHARACTERS_PER_LINE);
  }

  strcpy(item, "");
  strcpy(msg, DataSave1->udtTransDetail.CardHolderName);
  Rprintf("CardHolderName: %s\r\n",DataSave1->udtTransDetail.CardHolderName);
  BuildLinePrint(0,line++, L_ENGLISH, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

  if (udtAppLang == L_ARABIC)
  {
#if DEV_DEBUG
  OutputReceiptData(0,line);
#endif
    ucPrinterWorking(0,line,FALSE);

    line = 0;
    for (i = 0; i < 5; i++) CHAR_FILL1[i] = 0x00;
    memset((char *)&CHAR_BUFFER1[0][0][0], 0x00, 5*PRT_LINES_PER_PAGE*PRT_CHARACTERS_PER_LINE);
    memset((char *)&ATTB_BUFFER1[0][0][0], 0x00, 5*PRT_LINES_PER_PAGE*PRT_CHARACTERS_PER_LINE);
  }
  return line;
}
/*
 * ************************************************************************************************
 *\Brief
 *\Param
 *\Return
 *****************************************************************************************************
 */
int iPrintMultiLineMsg(int noPage, APP_LANG udtLang, char* pcBuff, int InLine, char attr, char justify_mode)
{
  //unsigned int lineSize=NUM_CHAR_PER_LINE;
  unsigned int startPosition=0;

  uchar ucMsgLen = 0;
  uchar ucRemMsgLen = 0;
  uchar ucIdx;
  int iLine;
  uchar ucMaxCharPerLine = PRT_CHARACTERS_PER_LINE;
  uchar ucBuffLine = 0;
  uchar aucBuff[70];
  uchar aucLineMsg[80];
  uchar aucTempLineMsg[80];

  char item[33], msg[33];

  Rprintf("iPrintErrMsg:\r\n");

  if (attr == PRT_DOUBLE_WIDTH_MODE)
  {
    ucMaxCharPerLine = PRT_CHARACTERS_PER_LINE/2;
  }
  iLine = InLine; 
  sprintf(aucBuff,"%s",pcBuff);
  ucMsgLen = strlen(aucBuff);
  ucRemMsgLen = ucMsgLen; 
  memset(aucTempLineMsg,0,sizeof(aucTempLineMsg));
  memcpy(aucTempLineMsg,aucBuff,ucMsgLen);

  while (ucRemMsgLen > 0)
  {
    Rprintf("ucRemMsgLen:%d\r\n",ucRemMsgLen);

    memset(aucLineMsg,0,sizeof(aucLineMsg));
    if (ucRemMsgLen > ucMaxCharPerLine)
    {
      for (ucIdx = ucMaxCharPerLine; ucIdx >= 0; ucIdx--)
      {
        if (aucTempLineMsg[ucIdx] == ' ')
        {
          break;
        }
      }

      if (ucIdx == 0)
      {
        ucIdx = ucMaxCharPerLine;
      }
      memcpy(aucLineMsg, aucTempLineMsg, ucIdx);
      ucRemMsgLen-=ucIdx;
      memset(aucTempLineMsg,0,sizeof(aucTempLineMsg));
      memcpy(aucTempLineMsg,&aucBuff[ucMsgLen - ucRemMsgLen + 1],ucRemMsgLen);
    }
    else
    {
      memcpy(aucLineMsg, aucTempLineMsg, ucRemMsgLen);
      ucRemMsgLen=0;
    }
    //ucRemMsgLen-=MIN(ucIdx,ucRemMsgLen);
    Rprintf("aucLineMsg: %s\r\n",aucLineMsg);

    strcpy(item, "");
    strcpy(msg, aucLineMsg);
    BuildLinePrint(noPage,iLine++, udtLang, item, strlen(item), msg, strlen(msg), attr, justify_mode);
  }
  return  iLine; 
}

/******************************************************************************/
/* EXPORTED FUNCTIONS                                                         */
/******************************************************************************/
uchar ucReceiptHeader(int iline, ushort usHeaderIdx,bool bIsRePrint)
{

  int i,line,line1,line2;
  char item[33], msg[33];
  char temp[33];

  memset(temp,0,sizeof(temp));

#if DEV_DEBUG
  Rprintf("\r\n");
  Rprintf("ucReceiptHeader()\r\n");
#endif
  //Set printing preference for control density or contrast level
  //SetPrinterDensity(10);

  //check if the paper is existed
  while ((PAPER_READ() & 0xFF)!= 0xFF)
  {

    DisplayMsgBox(EMPTY_MSG, INS_PAPR,0, 0);
    //Lprint(0,0,"Insert Paper");
    // Printer Off-Line?
    if ((ONLINE_READ() & 0xFF) != 0xFF)
    {
      MAKEONLINE();
    }
  }

  InitializePrinterParam();

  DisplayMsgBox(S_RCT_PREP, S_PLZ_WAIT,0, 0);


  ulStLine = 0;
  ulStLine1 = 0;
  ulStLine2 = 0;

  line2 =0;
  line1 =0;
  int height=0;
  //Print Logo on Receipt
  line = PrintLogo(receiptLogo,ucPageNo,0);

//  line++;

//
//  strcpy(item, "");
//  strcpy(msg, DataSave1->udtDeviceConfig.aucBankName[udtAppLang]);
//  BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

  strcpy(item, "");
  strcpy(msg, DataSave1->udtDeviceConfig.aaucMerchantName[udtAppLang]);
  BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, LEFT_JUSTIFIED);

  strcpy(item, "");
  strcpy(msg, DataSave1->udtDeviceConfig.aaucMerchantAddr[udtAppLang]);
  BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, LEFT_JUSTIFIED);

  strcpy(item, GetMsgs(R_TRM_ID));
  strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtDeviceConfig.aucTermId));
  BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, LEFT_JUSTIFIED);

  if (bIsRePrint == TRUE)
  {
    strcpy(item, ""); 
    strcpy(msg, GetMsgs(R_REPRINT_CPY));
    BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);
  }

  line = iPrintMultiLineMsg(ucPageNo,udtAppLang, GetMsgs(usHeaderIdx),line, PRT_DOUBLE_WIDTH_MODE, CENTER_JUSTIFIED);


  strcpy(item, GetMsgs(R_MRCH_ID));
  strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtDeviceConfig.aucMerchantId));
  BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);


  //strcpy(item, "");
  //strcpy(msg, GetMsgs(usHeaderIdx));
  //BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_DOUBLE_WIDTH_MODE, CENTER_JUSTIFIED);

  
  strcpy(item, GetMsgs(R_TIME));
  //Rprintf("aucTransTime:%s",DataSave1->udtTransDetail.aucTransTime);
  strcpy(msg, pcFrmDgts(TIME,DataSave1->udtTransDetail.aucTransTime));
  //Rprintf("R_TIME:%s",msg);
  BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

  strcpy(item, GetMsgs(R_DATE));
  //Rprintf("aucTransDate:%s",DataSave1->udtTransDetail.aucTransDate);
  strcpy(msg, pcFrmDgts(DATE,DataSave1->udtTransDetail.aucTransDate));
  //Rprintf("R_DATE:%s",msg);
  BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);


//
//  strcpy(item, GetMsgs(R_MRCH_ID));
//  strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtDeviceConfig.aucMerchantId));
//  BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

//  if (DataSave1->udtTransDetail.udtMSRCard.bIsValidData)
//  {
    strcpy(item, GetMsgs(R_CRD_NO)); 
//    char * pch;
//
//    pch =strtok(DataSave1->udtTransDetail.udtMSRCard.acTrack2,"=?");
 //   strcpy(msg, pcFrmDgts(PAN_T,pch));

    strcpy(msg, pcFrmDgts(PAN_T,DataSave1->udtTransDetail.PAN));
    BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);


    strcpy(item, "");
    //DataSave1->udtTransDetail.SelectedAID = emvref.SelectedAID;
    //Rprintf("AIDDDDDDDDDDDD=%s\r\n",DataSave1->udtTransDetail.SelectedAID);
    strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtTransDetail.SelectedAID_Name));
    BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_DOUBLE_WIDTH_MODE, CENTER_JUSTIFIED);

    //strcpy(item, ""); 
    //strcpy(msg, pcFrmDgts(PAN,DataSave1->udtTransDetail.udtMSRCard.PAN));
    ////Rprintf("R_CRD_NO:%s",msg);
    //BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

//    strcpy(item, GetMsgs(R_CRD_HLD_NAME));
//    strcpy(msg, "");
//    BuildLinePrint(ucPageNo++,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
//
//    strcpy(item, "");
//    strcpy(msg, DataSave1->udtTransDetail.CardHolderName);
//    Rprintf("CardHolderName: %s\r\n",DataSave1->udtTransDetail.CardHolderName);
//    BuildLinePrint(ucPageNo++,line1++, L_ENGLISH, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

    //line = usPrntCardHoldrName(line);
    strcpy(item, GetMsgs(R_EXP_DATE));
    strcpy(msg, pcFrmDgts(EXP_DATE,DataSave1->udtTransDetail.ExpiryDate));
    BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
  //}

  strcpy(item, GetMsgs(R_TRANS_ID));
  //sprintf(temp,"%ld",(DataSave1->udtTransDetail.ulSysTraceAuditNo));
  if ((DataSave1->udtTransDetail.udtTansType == VOID) ||
      (bIsRePrint == TRUE)) {
    sprintf(temp,"%06d",DataSave1->udtTransDetail.ulSysTraceAuditNo);
  }
  else
  {
    sprintf(temp,"%06d",(DataSave1->ulSysTraceAuditNo-1));
  }
  strcpy(msg, pcFrmDgts(DIGIT,temp));
  BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

  strcpy(item, GetMsgs(R_BTCH_NO));
  sprintf(temp,"%06d",(DataSave1->ulBatchNo));
  strcpy(msg, pcFrmDgts(DIGIT,temp));
  BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

  ulStLine = line; 
  ulStLine1 = line1;
  ulStLine2 = line2;

  if (ucPageNo == 0)
  {
    return line; 
  }
  else if (ucPageNo == 1)
  {
    return line1;
  }
  else if (ucPageNo == 2)
  {
    return line2;
  }

}
/*
 * ************************************************************************************************
 *\Brief
 *\Param
 *\Return
 *****************************************************************************************************
 */
int iPrntNECRct(bool bIsSucceededTrans, int iline)
{

  int line;
  char item[33], msg[33],aucTempMsg[5];
  line = iline;

  if (bIsSucceededTrans)
  {
    if (udtAppLang == L_ARABIC)
    {
      DisplayMsgBox(S_PRNT, S_PLZ_WAIT,0, 0);
    #if DEV_DEBUG
      OutputReceiptData(0,ulStLine);
    #endif
      if (ucPageNo >= 0)
      {
        ucPrinterWorking(0,ulStLine,TRUE);
      }
      if (ucPageNo > 0)
      {
        ucPrinterWorking(1,ulStLine1,FALSE);
      }
      if (ucPageNo > 1)
      {
        ucPrinterWorking(2,line,TRUE);
      }

      InitializePrinterParam();
      ucPageNo = 0;
      ulStLine = 0;
      ulStLine1 = 0;
      line = 0;
    }
    strcpy(item, udtNECInfo.aucUnit);
    strcpy(msg, "");
    //strcat(msg, GetMsgs(R_KWH));
    BuildLinePrint(ucPageNo,line++, L_ENGLISH, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
    if (udtAppLang == L_ARABIC)
    {
       ucPrinterWorking(0,line,FALSE);
       InitializePrinterParam();
       line = 0;
     }

    strcpy(item, GetMsgs(R_PAD_AMNT));
    strcpy(msg, pcFrmDgts(FLOAT,DataSave1->udtTransDetail.aucAmount));
    if (udtAppLang == L_ENGLISH)
      strcat(msg, " "); 
    strcat(msg, GetMsgs(R_SDG));
    BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

    strcpy(item, GetMsgs(R_SL_AMNT));
    strcpy(msg, pcFrmDgts(DIGIT,udtNECInfo.aucSalesAmount));
    if (udtAppLang == L_ENGLISH)
      strcat(msg, " "); 
    strcat(msg, GetMsgs(R_SDG));
    BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

    strcpy(item, GetMsgs(R_FEES));
    strcpy(msg, pcFrmDgts(DIGIT,udtNECInfo.aucFixesFee));
    if (udtAppLang == L_ENGLISH)
      strcat(msg, " "); 
    strcat(msg, GetMsgs(R_SDG));
    BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

   strcpy(item, GetMsgs(R_CUST_NM));
   strcpy(msg, "");
   BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

   if (udtAppLang == L_ARABIC)
   {
      ucPrinterWorking(0,line,TRUE);
      InitializePrinterParam();
      line = 0;
   }

   line = iPrintMultiLineMsg(ucPageNo,L_ENGLISH,udtNECInfo.aucCustomerName,line, PRT_NORMAL_MODE, CENTER_JUSTIFIED);

   if (udtAppLang == L_ARABIC)
   {
      ucPrinterWorking(0,line,FALSE);
      InitializePrinterParam();
      line = 0;
    }

    if (udtAppLang == L_ARABIC)
    {
      strcpy(item, GetMsgs(R_ACNT_NO));
      strcpy(msg, "");
      BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

      ucPrinterWorking(0,line,TRUE);
      InitializePrinterParam();
      line = 0;

      strcpy(item, udtNECInfo.aucAccountNumber);
      strcpy(msg, "");
      BuildLinePrint(ucPageNo,line++, L_ENGLISH, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

      ucPrinterWorking(0,line,FALSE);
      InitializePrinterParam();
      line = 0;
    }
    else
    {
      strcpy(item, GetMsgs(R_ACNT_NO));
      strcpy(msg, pcFrmDgts(DIGIT,udtNECInfo.aucAccountNumber));
      BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
    }

    strcpy(item, GetMsgs(R_MTR_NO));
    strcpy(msg, pcFrmDgts(DIGIT,udtNECInfo.aucMeterNumber));
    BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

    strcpy(item, GetMsgs(R_REF));
    //strcpy(msg, pcFrmDgts(DIGIT,udtNECInfo.aucReferenceNumber));
    strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtTransDetail.aucRtrvalRefNo));
    BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

    strcpy(item, GetMsgs(R_OPR_MSG));
    strcpy(msg, "");
    BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

    if (udtAppLang == L_ARABIC)
    {
      ucPrinterWorking(0,line,TRUE);
      InitializePrinterParam();
      line = 0;
    }

    line = iPrintMultiLineMsg(ucPageNo,L_ENGLISH,udtNECInfo.aucOperator,line, PRT_NORMAL_MODE, CENTER_JUSTIFIED);

    if (udtAppLang == L_ARABIC)
    {
      ucPrinterWorking(0,line,FALSE);
      InitializePrinterParam();
      line = 0;
    }

    strcpy(item, ""); 
    strcpy(msg, "==================================");
    BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

    strcpy(item, "");
    strcpy(msg, pcFrmDgts(DIGIT,udtNECInfo.aucToken));
    BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

    strcpy(item, ""); 
    strcpy(msg, "==================================");
    BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);
  }
  else
  {
    strcpy(item, GetMsgs(R_MTR_NO));
    strcpy(msg, "");
    BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

    strcpy(item, GetMsgs(R_AMNT));
    strcpy(msg, "");
    BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

    strcpy(item, GetMsgs(R_REF_NO));
    strcpy(msg, "");
    BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

    sprintf(aucTempMsg,"%02d",udtRunData.ucErrorExtCode);
    strcpy(item, GetMsgs(R_RES_CD));
    strcpy(msg, pcFrmDgts(DIGIT,aucTempMsg));
    BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

    strcpy(item, GetMsgs(R_FLD_MSG));
    strcpy(msg, "");
    BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

    line = iPrintMultiLineMsg(ucPageNo,udtAppLang, GetErrMsgs(udtRunData.ucErrorExtCode),line, PRT_NORMAL_MODE, CENTER_JUSTIFIED);
  }

  return line;
}
/*
 * ************************************************************************************************
 *\Brief
 *\Param
 *\Return
 *****************************************************************************************************
 */
uchar ucPrintSuccCustRct(enuRctCopyType udtRctCopyType)
{

  switch (udtRctCopyType)
  {
  case CUST_COPY:
    return (ucPrintRct(TRUE,TRUE,FALSE));
    break;
  case MERCH_COPY:
    return (ucPrintRct(FALSE,TRUE,FALSE));
    break;
  default:
    break;

  }

}
/*
 * ************************************************************************************************
 *\Brief
 *\Param
 *\Return
 *****************************************************************************************************
 */
uchar ucPrntCustomerRct(void)
{
  return (ucPrintRct(TRUE,TRUE,FALSE));
}
/*
 * ************************************************************************************************
 *\Brief
 *\Param
 *\Return
 *****************************************************************************************************
 */
uchar ucPrntMerchantRct(void)
{
  return (ucPrintRct(FALSE,TRUE,FALSE));
}
/*
 * ************************************************************************************************
 *\Brief
 *\Param
 *\Return
 *****************************************************************************************************
 */
uchar ucPrntFailedRct(void)
{
  return (ucPrintRct(FALSE,FALSE,FALSE));
}
/*
 * ************************************************************************************************
 *\Brief
 *\Param
 *\Return
 *****************************************************************************************************
 */
uchar ucRePrintRct(void)
{
  return (ucPrintRct(FALSE,TRUE,TRUE));
}
/*
 * ************************************************************************************************
 *\Brief
 *\Param
 *\Return
 *****************************************************************************************************
 */
uchar ucPrintRct(bool bIsCustCopy, bool bIsSucceededTrans, bool bIsRePrint)
{

    int i, j, n, line, width, height, iPrntrResult;
    uchar ucIdx;
    ulong ulIdx;
    uchar ucDateLen;
    uchar ucOpCodeLen;
    uchar ucAmntLen;
    uchar ucTempLen;
    uchar ucSpaceLen;
    char item[33], msg[33], aucTempMsg[50];
    int IPrntSt;
    enuTransType TransType = DataSave1->udtTransDetail.udtTansType;
#if DEV_DEBUG
    Rprintf("\r\n");
    Rprintf("BuildPrintPOSInfoReport()\r\n");
#endif

    if ((bIsCustCopy == TRUE) && (bIsSucceededTrans == FALSE))
    {
      return FALSE;
    }
    //if ((bIsCustCopy == TRUE)&& (bIsRePrint == FALSE)) {
    //  if (bIsSucceededTrans == TRUE) {
    //    ucSaveTansInfo(bIsSucceededTrans); 
    //  }
    //  memset(&DataSave1->udtRevesalData,0,sizeof(strRevesalData));
    //}
    //if ((DataSave1->udtTransDetail.udtTansType == VOID) && (bIsSucceededTrans == TRUE)) {
    //  for (ulIdx = 0; ulIdx <DataSave1->udtTransStore.ulSavedTransNo; ulIdx++) {
    //    if ((DataSave1->udtTransStore.audtTrans[ulIdx].ulSysTraceAuditNo == DataSave1->udtTransDetail.ulSysTraceAuditNo) && 
    //        (DataSave1->udtTransStore.audtTrans[ulIdx].udtTansType == PURCH) &&
    //        (DataSave1->udtTransStore.audtTrans[ulIdx].bIsVoided == FALSE)){
    //      DataSave1->udtTransStore.audtTrans[ulIdx].bIsVoided = TRUE;
    //      break;
    //    }
    //  }
    //}

    line = 0;
    ucPageNo = 0;

    line = ucReceiptHeader(0,(TransType+R_PURCHASE_HDR),bIsRePrint);
    	//Heba
//    if (TransType == ELECT)
//    {
//      if (bIsSucceededTrans)
//      {
//        strcpy(item, GetMsgs(R_RNN_NO));
//        strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtTransDetail.aucRtrvalRefNo));
//        BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
//
//        strcpy(item, GetMsgs(R_APPRVL_CODE));
//        strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtTransDetail.aucAuthIdResponse));
//        BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
//        Rprintf("R_REF_NO\r\n");
//
//      }
//    }


    Rprintf("TransType:%d\r\n",TransType);

  //  line++;
    // Approved or declined
    if (bIsSucceededTrans)
    {
      strcpy(item, "");
//      udtAppLang = L_ARABIC;
//      USE_PRTFONT((void *)Font12x24, ON);
      strcpy(msg, GetMsgs(R_APRV_HDR));
      BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_DOUBLE_WIDTH_MODE, CENTER_JUSTIFIED);

    	//line = line+ PrintLogo(ApprovedMSG,ucPageNo,line);

    }
    else
    {
    	//line = line+ PrintLogo(FailedMsg,ucPageNo,line);
      line = iPrintMultiLineMsg(ucPageNo,udtAppLang, GetMsgs(R_FLD_TRANS_HDR),line, PRT_DOUBLE_WIDTH_MODE, CENTER_JUSTIFIED);
    }
//    udtAppLang = L_ENGLISH;
//    USE_PRTFONT((void *)Font12x24, OFF);

//    strcpy(item, GetMsgs(R_TRANS_ID));
//    strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtTransDetail.aucExtRtrvalRefNo));
//    BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

    if ( (TransType != BATCH) )
    {
    	//receipt Number
      strcpy(item, GetMsgs(R_RCT_NO));

      memset(aucTempMsg,0,sizeof(aucTempMsg));
      if ((DataSave1->udtTransDetail.udtTansType == VOID) ||
          (bIsRePrint == TRUE)) {
        sprintf(aucTempMsg,"%06ld",DataSave1->udtTransDetail.ulSysTraceAuditNo);
      }
      else
      {
        sprintf(aucTempMsg,"%06ld",(DataSave1->ulSysTraceAuditNo-1));
      }
      strcpy(msg, pcFrmDgts(DIGIT,aucTempMsg));
      BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
    }

    if (TransType == ELECT)
    {
      line = iPrntNECRct(bIsSucceededTrans,line);
    }
    else
    {
      if ((TransType == TOPUP_PAY) || (TransType == BILL_PAY))
      {
        strcpy(item, GetMsgs(R_MOB_NO));
        strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtTransDetail.aucAccountNo));
        BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

        if (TransType == BILL_PAY)
        {
          strcpy(item, GetMsgs(R_CONT_NO));
          strcpy(msg, pcFrmDgts(DIGIT,udtBillInqInfo.aucContractNumber));
          BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

          strcpy(item, GetMsgs(R_BLD_AMNT));
          strcpy(msg, pcFrmDgts(DIGIT,udtBillInqInfo.aucBillamount));
          BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

          strcpy(item, GetMsgs(R_PAD_AMNT));
        }
        else
        {
          strcpy(item, GetMsgs(R_VCH_AMNT));
        }
        strcpy(msg, pcFrmDgts(FLOAT,DataSave1->udtTransDetail.aucAmount));

        if (udtAppLang == L_ENGLISH)
          strcat(msg, " ");
        strcat(msg, GetMsgs(R_SDG));
        BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
      }
      else if (TransType == PURCH_CASH_BACK)
      {
        strcpy(item, GetMsgs(R_TOT_AMNT));
        strcpy(msg, pcFrmDgts(FLOAT,DataSave1->udtTransDetail.aucAmount));
        if (udtAppLang == L_ENGLISH)
          strcat(msg, " ");
        strcat(msg, GetMsgs(R_SDG));
        BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

        strcpy(item, GetMsgs(R_CSH_BCK_AMNT));
        strcpy(msg, pcFrmDgts(FLOAT,DataSave1->udtTransDetail.aucCashBackAmount));
        if (udtAppLang == L_ENGLISH)
          strcat(msg, " ");
        strcat(msg, GetMsgs(R_SDG));
        BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
      }
      else if ((TransType != BAL_INQUIRY) && (TransType != PIN_CHANGE) && (TransType != MINI_STATE))
      {
        strcpy(item, GetMsgs(R_AMNT));
        strcpy(msg, pcFrmDgts(FLOAT,DataSave1->udtTransDetail.aucAmount));
        if (udtAppLang == L_ENGLISH)
          strcat(msg, " ");
        strcat(msg, GetMsgs(R_SDG));
        BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
      }

      switch (TransType)
      {
      case CARD_TRANSFER:
        strcpy(item, GetMsgs(R_CRD_NO));
        strcpy(msg, pcFrmDgts(PAN_T,DataSave1->udtTransDetail.aucAccountNo));
        BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
        break;
      case ACC_TRANSFER:
        strcpy(item, GetMsgs(R_ACNT_NO));
        strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtTransDetail.aucAccountNo));
        BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
        break;
      case MONEY_TRANSFER:
      case MONEY_RCV:
        if (bIsSucceededTrans)
        {
          strcpy(item, GetMsgs(R_APPRVL_CODE));
          strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtTransDetail.aucAuthIdResponse));
          BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
        }
      default:
        break;
      }


//      //RRN
//      Rprintf("R_REF_NO\r\n");
//
//      strcpy(item, GetMsgs(R_REF_NO));
//      strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtTransDetail.aucRtrvalRefNo));
//      BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);


      if (bIsSucceededTrans)
      {

    	  if (DataSave1->udtTransDetail.bIsOffline==0)//online only
        {
            //RRN
            Rprintf("R_REF_NO\r\n");

            strcpy(item, GetMsgs(R_REF_NO));
            strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtTransDetail.aucRtrvalRefNo));
            BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);


          strcpy(item, GetMsgs(R_ATH_NO));
          strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtTransDetail.aucAuthIdResponse));
          BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

        }
    	 if (TransType != BAL_INQUIRY)
    	 {

    	 }
        else
        {
          strcpy(item, GetMsgs(R_CURR_BAL));
          strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtTransDetail.aucBalance));
          if (udtAppLang == L_ENGLISH)
            strcat(msg, " ");
          strcat(msg, GetMsgs(R_SDG));
          BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
        }
        if (TransType == MINI_STATE)
        {
          strcpy(item, GetMsgs(R_MINI_STAT_HDR1));
          strcpy(msg, "");
          BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
          Rprintf("ucNoRecords:%d\r\n",udtMiniStatment.ucNoRecords);
          ucSpaceLen = 0;
          for (ucIdx = 0; ucIdx < udtMiniStatment.ucNoRecords; ucIdx++)
          {
            memset(aucTempMsg,' ',sizeof(aucTempMsg));
            ucDateLen = strlen(pcFrmDgts(EXP_DATE,udtMiniStatment.rudtStatmentRcrds[ucIdx].aucDateMMDD));
            ucOpCodeLen = strlen(pcFrmDgts(DIGIT,udtMiniStatment.rudtStatmentRcrds[ucIdx].aucOpCode)) ;
            ucAmntLen = strlen(pcFrmDgts(FLOAT,udtMiniStatment.rudtStatmentRcrds[ucIdx].aucAmount));
            ucTempLen = ucDateLen + ucOpCodeLen + ucAmntLen + strlen(GetMsgs(R_SDG)) + 1;
            //ucSpaceLen = (32-ucTempLen)/2;
            ucSpaceLen = 5;
            aucTempMsg[33]= 0x00;
            Rprintf("ucDateLen:%d,ucOpCodeLen:%d,ucAmntLen:%d,ucTempLen:%d,ucSpaceLen:%d\r\n",ucDateLen,ucOpCodeLen,ucAmntLen,ucTempLen,ucSpaceLen);
/*            memcpy(aucTempMsg, pcFrmDgts(EXP_DATE,udtMiniStatment.rudtStatmentRcrds[ucIdx].aucDateMMDD),ucDateLen);

            memcpy(&aucTempMsg[ucDateLen+ucSpaceLen],pcFrmDgts(DIGIT,udtMiniStatment.rudtStatmentRcrds[ucIdx].aucOpCode),ucOpCodeLen);

            memcpy(&aucTempMsg[ucDateLen+ucSpaceLen+ucOpCodeLen+ucSpaceLen],pcFrmDgts(FLOAT,udtMiniStatment.rudtStatmentRcrds[ucIdx].aucAmount), ucAmntLen);

            if (udtMiniStatment.rudtStatmentRcrds[ucIdx].bIsNegative) {
              memset(&aucTempMsg[ucDateLen+ucOpCodeLen+ucAmntLen+ucSpaceLen+ucSpaceLen],'-',1);
            } else {
              memset(&aucTempMsg[ucDateLen+ucOpCodeLen+ucAmntLen+ucSpaceLen+ucSpaceLen],'+',1);
            }
            memcpy(&aucTempMsg[ucDateLen+ucOpCodeLen+ucAmntLen+ucSpaceLen+ucSpaceLen+1],GetMsgs(R_SDG),strlen(GetMsgs(R_SDG)));
            Rprintf("aucTempMsg:%s\r\n",aucTempMsg);

            asc_to_str(item,aucTempMsg,32);
            //strcpy(item, aucTempMsg);
            strcpy(msg, "");*/
            strcpy(item,pcFrmDgts(EXP_DATE,udtMiniStatment.rudtStatmentRcrds[ucIdx].aucDateMMDD));
            strcat(item,"     ");
            strcat(item,pcFrmDgts(DIGIT,udtMiniStatment.rudtStatmentRcrds[ucIdx].aucOpCode));

            strcpy(msg, pcFrmDgts(FLOAT,udtMiniStatment.rudtStatmentRcrds[ucIdx].aucAmount));
            if (udtMiniStatment.rudtStatmentRcrds[ucIdx].bIsNegative)
            {
              strcat(msg,"-");
            }
            else
            {
              strcat(msg,"+");
            }

            strcat(msg,GetMsgs(R_SDG));

            BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

          }

        }
      }
      else
      {
    	  //Response code
        //sprintf(aucTempMsg,"%02d",udtRunData.ucErrorExtCode); //heba
    	  sprintf(aucTempMsg,"%03d",DataSave1->udtTransDetail.ulActionCode);
        strcpy(item, GetMsgs(R_RES_CD));
        strcpy(msg, pcFrmDgts(DIGIT,aucTempMsg));
        BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

//        strcpy(item, GetMsgs(R_FLD_MSG));
//        strcpy(msg, "");
//        BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
//
//        line = iPrintMultiLineMsg(ucPageNo,udtAppLang, GetErrMsgs(udtRunData.ucErrorExtCode),line, PRT_NORMAL_MODE, CENTER_JUSTIFIED);
      }
    }

    Rprintf("bIsCustCopy\r\n");

    if (bIsCustCopy)
    {
      if (bIsSucceededTrans)
      {
        strcpy(item, GetMsgs(R_CUST_CPY_HDR));
        strcpy(msg, "");
        BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
      }
    }
    else
    {
      if (bIsSucceededTrans && (bIsRePrint == FALSE) && (TransType != BAL_INQUIRY))
      {
        strcpy(item, "");
        strcpy(msg, GetMsgs(R_CUST_SIG));
        BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);
        strcpy(item, "");
        strcpy(msg, "----------------------------------");
        BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

      }
      strcpy(item, "");
      strcpy(msg, GetMsgs(R_MERCH_CPY_HDR));
      BuildLinePrint(ucPageNo,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);
    }


    line += PRT_PAPER_FORM_FEED_LINE;
    Rprintf("ulStLine:%d ulStLine1:%d line:%d\r\n",ulStLine,ulStLine1,line);

    DisplayMsgBox(S_PRNT, S_PLZ_WAIT,0, 0);

  #if DEV_DEBUG
    OutputReceiptData(0,ulStLine);
  #endif
    ucPrinterWorking(ucPageNo,line,TRUE);
//    if (ucPageNo >= 0) {
//      if (ucPageNo == 0) {
//        ucPrinterWorking(0,line,TRUE);
//      } else {
//        ucPrinterWorking(0,ulStLine,TRUE);
//      }
//    }

//    if (ucPageNo > 0){
//      ucPrinterWorking(1,ulStLine1,FALSE);
//    }
//    if (ucPageNo > 1) {
//      ucPrinterWorking(2,line,TRUE);
//    }
}

/*
 * ************************************************************************************************
 *\Brief
 *\Param
 *\Return
 *****************************************************************************************************
 */
uchar ReRrintRct(ulong ulRctNo)
{
  ulong ulIdx;
  bool bIsTransFound = FALSE;

  for (ulIdx = 0; ulIdx <DataSave1->udtTransStore.ulSavedTransNo; ulIdx++)
  {
    if (DataSave1->udtTransStore.audtTrans[ulIdx].ulSysTraceAuditNo == ulRctNo)
    {
      memset(&DataSave1->udtTransDetail,0,sizeof(strTransDetail));
      memcpy(&DataSave1->udtTransDetail, &(DataSave1->udtTransStore.audtTrans[ulIdx]),
               sizeof(strTransDetail));
      bIsTransFound = TRUE;
      break;
    }
  }
  if (bIsTransFound == TRUE)
  {
    ucPrintRct(FALSE,TRUE,TRUE);
    memset(&DataSave1->udtTransDetail,0,sizeof(strTransDetail));
  }
  else
  {
    DisplayMsgBox(S_REPRNT, S_RPRNT_NOT_EXST,1000, 0);
  }
}
/*
 * ************************************************************************************************
 *\Brief
 *\Param
 *\Return
 *****************************************************************************************************
 */
uchar ucPrntBatch(void)
{

  int i, line;
  char item[33], msg[33];
  DATE_TIME udtTransDateTime;
  uchar aucTransDate[9];
  uchar aucTransTime[7];
  uchar aucTempBuff[9];
  uchar aucTempItemBuff[34];
  uchar aucTempMsgBuff[34];
  ulong ulIdx;
  ulong ulLogIdx;
  ulong ulOverallTotAmnt= 0;
  ulong ulOverallTotTrans= 0;
  bool bIsFoundOnLog = FALSE;


#if DEV_DEBUG
  Rprintf("\r\n");
  Rprintf("ucPrntDetailReport()\r\n");
#endif
  //Set printing preference for control density or contrast level
  //SetPrinterDensity(10);

  //check if the paper is existed
  while ((PAPER_READ() & 0xFF)!= 0xFF)
  {

    DisplayMsgBox(EMPTY_MSG, INS_PAPR,0, 0);
    // Printer Off-Line?
    if ((ONLINE_READ() & 0xFF) != 0xFF)
    {
      MAKEONLINE();
    }
  }

  InitializePrinterParam();

  DisplayMsgBox(S_RCT_PREP, S_PLZ_WAIT,0, 0);


  //Print Logo on Receipt
  line = PrintLogo(receiptLogo,0,0);

  strcpy(item, "");
  strcpy(msg, DataSave1->udtDeviceConfig.aucBankName[udtAppLang]);
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

  strcpy(item, "");
  strcpy(msg, DataSave1->udtDeviceConfig.aaucMerchantName[udtAppLang]);
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

  strcpy(item, "");
  strcpy(msg, DataSave1->udtDeviceConfig.aaucMerchantAddr[udtAppLang]);
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

  strcpy(item, "");
  strcpy(msg, GetMsgs(R_BATCH_HDR)); 
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_DOUBLE_WIDTH_MODE, CENTER_JUSTIFIED);

  if (GetRTCDateTime(&udtTransDateTime,0))
  {
    memset(aucTransTime,0,sizeof(aucTransTime));
    memset(aucTransDate,0,sizeof(aucTransDate));

    sprintf(aucTransTime, "%02X%02X%02X",
            udtTransDateTime.CLKHOUR, udtTransDateTime.CLKMIN, udtTransDateTime.CLKSEC);
    sprintf(aucTransDate,"%02X%02X%02X%02X",
            udtTransDateTime.CLKYEARH, udtTransDateTime.CLKYEARL,
            udtTransDateTime.CLKMONTH, udtTransDateTime.CLKDATE);
  }
  else
  {
    return FALSE;
  }
  
  strcpy(item, GetMsgs(R_TIME));
  //Rprintf("aucTransTime:%s",DataSave1->udtTransDetail.aucTransTime);
  strcpy(msg, pcFrmDgts(TIME,aucTransTime));
  //Rprintf("R_TIME:%s",msg);
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

  strcpy(item, GetMsgs(R_DATE));
  //Rprintf("aucTransDate:%s",DataSave1->udtTransDetail.aucTransDate);
  strcpy(msg, pcFrmDgts(DATE,aucTransDate));
  //Rprintf("R_DATE:%s",msg);
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

  Rprintf("R_TIME:%s\r\n",msg);

  strcpy(item, GetMsgs(R_TRM_ID));
  strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtDeviceConfig.aucTermId));
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

  strcpy(item, GetMsgs(R_MRCH_ID));
  strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtDeviceConfig.aucMerchantId));
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

  strcpy(item, GetMsgs(R_BTCH_NO));
  memset(aucTempBuff, 0, sizeof(aucTempBuff));
  sprintf(aucTempBuff,"%06d",DataSave1->ulBatchNo);
  strcpy(msg, pcFrmDgts(DIGIT,aucTempBuff));
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

  Rprintf("R_BTCH_NO:%s\r\n",msg);

  strcpy(item, ""); 
  strcpy(msg, GetMsgs(R_APRV_HDR));
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_DOUBLE_WIDTH_MODE, CENTER_JUSTIFIED);

  //strcpy(item, ""); 
  //strcpy(msg, "----------------------------------");
  //BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

  	  //Heba
//  strcpy(item, GetMsgs(R_TRANS_ID));
//  strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtTransDetail.aucExtRtrvalRefNo));
//  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
//
//  strcpy(item, GetMsgs(R_REF_NO));
//  strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtTransDetail.aucRtrvalRefNo));
//  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
//
//
//  strcpy(item, GetMsgs(R_ATH_NO));
//  strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtTransDetail.aucAuthIdResponse));
//  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

  strcpy(item, GetMsgs(R_BATCH_HDR1));
  strcpy(msg, "");
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

  for (ulIdx = 0; ulIdx < DataSave1->udtTransStore.ulSavedTransNo; ulIdx++)
  {
    memset(item ,0,sizeof(item));
    strcpy(item, pcFrmDgts(DATEYYMMDD,&DataSave1->udtTransStore.audtTrans[ulIdx].aucTransDate[2]));
    strcat(item, " ");
    strcat(item, pcGetTransName(DataSave1->udtTransStore.audtTrans[ulIdx].udtTansType,
                               DataSave1->udtTransStore.audtTrans[ulIdx].udtVendType));
    strcpy(msg, pcFrmDgts(FLOAT,DataSave1->udtTransStore.audtTrans[ulIdx].aucAmount));
    if (udtAppLang == L_ENGLISH)
      strcat(msg, " "); 
    strcat(msg, GetMsgs(R_SDG));
    BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
    line = vidChkPrntPageSize(line);

    Rprintf("ulIdx= %d item:%s msg:%s\r\n",ulIdx, item, msg);
  }
  strcpy(item, "");
  strcpy(msg, GetMsgs(R_MERCH_CPY_HDR));
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);
  line = vidChkPrntPageSize(line);

    line += PRT_PAPER_FORM_FEED_LINE;

    DisplayMsgBox(S_PRNT, S_PLZ_WAIT,0, 0);

#if DEV_DEBUG
  OutputReceiptData(0,line);
#endif
  ucPrinterWorking(0,line,TRUE);
}

/*
 * ************************************************************************************************
 *\Brief
 *\Param
 *\Return
 *****************************************************************************************************
 */
uchar ucReprintMenu(void)
{

  uchar ucselectedItem;
  int RV;
  ulong ulReceiptNo;
  uchar aucTempBuff[8];
  while (1)
  {
    if ((RV = DisplayMsgMenu(S_REPRNT, &ucselectedItem, 10000, S_RPRNT_LAST, S_RPRNT_BY_NUM)) == 0)
    {
      switch (ucselectedItem+1)
      {
      case 1:
        ReRrintRct(DataSave1->udtTransStore.audtTrans[(DataSave1->udtTransStore.ulSavedTransNo-1)].ulSysTraceAuditNo);
        break;
      case 2:
        memset(aucTempBuff, 0, sizeof(aucTempBuff));
        //sprintf(aucTempBuff,"%d",DataSave1->ulReceiptNo);
        if (DispPrompt(S_REPRNT,R_RCT_NO,1,7, DIGIT_PROMPT,aucTempBuff) == TRUE )
        {
          ReRrintRct((unsigned long)atol(aucTempBuff));
        }
        break;
      default:
        break;
      }
    }
    else if((RV ==1)||(RV ==-1))
    { //canceled or timed out
      break;
    }
  }

}
/*
 * ************************************************************************************************
 *\Brief
 *\Param
 *\Return
 *****************************************************************************************************
 */
uchar ucPrntReport(bool bIsDetailed)
{

  int i, line;
  char item[33], msg[33];
  DATE_TIME udtTransDateTime;
  uchar aucTransDate[9];
  uchar aucTransTime[7];
  uchar aucTempBuff[9];
  uchar aucTempItemBuff[34];
  uchar aucTempMsgBuff[34];
  ulong ulIdx;
  ulong ulLogIdx;
  ulong ulOverallTotAmnt= 0;
  ulong ulOverallTotTrans= 0;
  bool bIsFoundOnLog = FALSE;


#if DEV_DEBUG
  Rprintf("\r\n");
  Rprintf("ucPrntDetailReport()\r\n");
#endif

  if (DataSave1->udtTransStore.ulSavedTransNo <= 0)
  {
    DisplayMsgBox(EMPTY_MSG,S_NO_TRANS,1000,FALSE);
    return FALSE;
  }
  //Set printing preference for control density or contrast level
  //SetPrinterDensity(10);

  //check if the paper is existed
  while ((PAPER_READ() & 0xFF)!= 0xFF)
  {
    DisplayMsgBox(EMPTY_MSG, INS_PAPR,0, 0);
    // Printer Off-Line?
    if ((ONLINE_READ() & 0xFF) != 0xFF)
    {
      MAKEONLINE();
    }
  }

  InitializePrinterParam();

  DisplayMsgBox(S_RCT_PREP, S_PLZ_WAIT,0, 0);


  //Print Logo on Receipt
  line = PrintLogo(receiptLogo,0,0);

  strcpy(item, "");
  strcpy(msg, DataSave1->udtDeviceConfig.aucBankName[udtAppLang]);
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

  strcpy(item, "");
  strcpy(msg, DataSave1->udtDeviceConfig.aaucMerchantName[udtAppLang]);
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

  strcpy(item, "");
  strcpy(msg, DataSave1->udtDeviceConfig.aaucMerchantAddr[udtAppLang]);
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

  strcpy(item, "");
  if (bIsDetailed == TRUE)
  {
    strcpy(msg, GetMsgs(R_DTLD_RPRT_HDR)); 
  }
  else
  {
    strcpy(msg, GetMsgs(R_SUMM_RPRT_HDR)); 
  }
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_DOUBLE_WIDTH_MODE, CENTER_JUSTIFIED);

  if (GetRTCDateTime(&udtTransDateTime,0))
  {
    memset(aucTransTime,0,sizeof(aucTransTime));
    memset(aucTransDate,0,sizeof(aucTransDate));

    sprintf(aucTransTime, "%02X%02X%02X",
            udtTransDateTime.CLKHOUR, udtTransDateTime.CLKMIN, udtTransDateTime.CLKSEC);
    sprintf(aucTransDate,"%02X%02X%02X%02X",
            udtTransDateTime.CLKYEARH, udtTransDateTime.CLKYEARL,
            udtTransDateTime.CLKMONTH, udtTransDateTime.CLKDATE);
  }
  else
  {
    return FALSE;
  }
  
  strcpy(item, GetMsgs(R_TIME));
  //Rprintf("aucTransTime:%s",DataSave1->udtTransDetail.aucTransTime);
  strcpy(msg, pcFrmDgts(TIME,aucTransTime));
  //Rprintf("R_TIME:%s",msg);
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

  strcpy(item, GetMsgs(R_DATE));
  //Rprintf("aucTransDate:%s",DataSave1->udtTransDetail.aucTransDate);
  strcpy(msg, pcFrmDgts(DATE,aucTransDate));
  //Rprintf("R_DATE:%s",msg);
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

  Rprintf("R_TIME:%s\r\n",msg);

  strcpy(item, GetMsgs(R_TRM_ID));
  strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtDeviceConfig.aucTermId));
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

  strcpy(item, GetMsgs(R_MRCH_ID));
  strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtDeviceConfig.aucMerchantId));
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

  strcpy(item, GetMsgs(R_BTCH_NO));
  memset(aucTempBuff, 0, sizeof(aucTempBuff));
  sprintf(aucTempBuff,"%06d",DataSave1->ulBatchNo);
  strcpy(msg, pcFrmDgts(DIGIT,aucTempBuff));
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

  Rprintf("R_BTCH_NO:%s\r\n",msg);

  strcpy(item, ""); 
  strcpy(msg, "----------------------------------");
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);
  if (bIsDetailed == TRUE)
  {
    strcpy(item, GetMsgs(R_DTLD_RPRT_HDR1));
    strcpy(msg, "");
    BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);
    strcpy(item, GetMsgs(R_DTLD_RPRT_HDR2));
    strcpy(msg, "");
    BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);
    strcpy(item, GetMsgs(R_DTLD_RPRT_HDR3));
    strcpy(msg, "");
    BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

    strcpy(item, ""); 
    strcpy(msg, "----------------------------------");
    BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

    for (ulIdx = 0; ulIdx < DataSave1->udtTransStore.ulSavedTransNo; ulIdx++)
    {
      memset(aucTempItemBuff,0,sizeof(aucTempItemBuff));
      sprintf(aucTempItemBuff,"%d",DataSave1->udtTransStore.audtTrans[ulIdx].ulSysTraceAuditNo);
      strcpy(item, pcFrmDgts(DIGIT,aucTempItemBuff));
      strcpy(msg, pcGetTransName(DataSave1->udtTransStore.audtTrans[ulIdx].udtTansType,
                                 DataSave1->udtTransStore.audtTrans[ulIdx].udtVendType));
      BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
      line = vidChkPrntPageSize(line);

      Rprintf("ulIdx= %d item:%s msg:%s\r\n",ulIdx, item, msg);

      strcpy(item, pcFrmDgts(DATEYYMMDD,&DataSave1->udtTransStore.audtTrans[ulIdx].aucTransDate[2]));
      if (udtAppLang == L_ENGLISH)
        strcat(item, " "); 
      strcat(item, pcFrmDgts(TIME,DataSave1->udtTransStore.audtTrans[ulIdx].aucTransTime));
      strcpy(msg, pcFrmDgts(FLOAT,DataSave1->udtTransStore.audtTrans[ulIdx].aucAmount));
      if (udtAppLang == L_ENGLISH)
        strcat(msg, " "); 
      strcat(msg, GetMsgs(R_SDG));
      BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
      line = vidChkPrntPageSize(line);

      Rprintf("ulIdx= %d item:%s msg:%s\r\n",ulIdx, item, msg);

      strcpy(item, pcFrmDgts(PAN_T,DataSave1->udtTransStore.audtTrans[ulIdx].PAN));
      strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtTransStore.audtTrans[ulIdx].aucAuthIdResponse));
      BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
      line = vidChkPrntPageSize(line);

      Rprintf("ulIdx= %d item:%s msg:%s\r\n",ulIdx, item, msg);

      strcpy(item, ""); 
      strcpy(msg, "----------------------------------");
      BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);
      line = vidChkPrntPageSize(line);
    }
  }
  else
  {
    strcpy(item, GetMsgs(R_SUMM_RPRT_HDR1));
    strcpy(msg, "");
    BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);
    strcpy(item, GetMsgs(R_SUMM_RPRT_HDR2));
    strcpy(msg, "");
    BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

    strcpy(item, ""); 
    strcpy(msg, "----------------------------------");
    BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);
    memset(&udtSumTransLog, 0,sizeof(udtSumTransLog));
    for (ulIdx = 0; ulIdx < DataSave1->udtTransStore.ulSavedTransNo; ulIdx++)
    {
      bIsFoundOnLog = FALSE;
      for (ulLogIdx = 0; ulLogIdx < udtSumTransLog.ucLogLen; ulLogIdx++)
      {
        if ((DataSave1->udtTransStore.audtTrans[ulIdx].udtTansType == udtSumTransLog.audtSumTransItem[ulLogIdx].udtTansType) &&
            (DataSave1->udtTransStore.audtTrans[ulIdx].udtVendType == udtSumTransLog.audtSumTransItem[ulLogIdx].udtVendType))
        {
          udtSumTransLog.audtSumTransItem[ulLogIdx].ulTotAmnt += atol(DataSave1->udtTransStore.audtTrans[ulIdx].aucAmount);
          udtSumTransLog.audtSumTransItem[ulLogIdx].ulTotTrns++;
          bIsFoundOnLog = TRUE;
          break;
        }
      }
      if (bIsFoundOnLog == FALSE)
      {
        udtSumTransLog.audtSumTransItem[udtSumTransLog.ucLogLen].udtTansType = DataSave1->udtTransStore.audtTrans[ulIdx].udtTansType ;
        udtSumTransLog.audtSumTransItem[udtSumTransLog.ucLogLen].udtVendType = DataSave1->udtTransStore.audtTrans[ulIdx].udtVendType;
        udtSumTransLog.audtSumTransItem[udtSumTransLog.ucLogLen].ulTotAmnt = atol(DataSave1->udtTransStore.audtTrans[ulIdx].aucAmount);
        udtSumTransLog.audtSumTransItem[ulLogIdx].ulTotTrns = 1;
        udtSumTransLog.ucLogLen++;
      }
    }
    for (ulLogIdx = 0; ulLogIdx < udtSumTransLog.ucLogLen; ulLogIdx++)
    {
      strcpy(item, pcGetTransName(udtSumTransLog.audtSumTransItem[ulLogIdx].udtTansType,
                                 udtSumTransLog.audtSumTransItem[ulLogIdx].udtVendType));
      memset(aucTempMsgBuff,0,sizeof(aucTempMsgBuff));
      sprintf(aucTempMsgBuff,"%d",udtSumTransLog.audtSumTransItem[ulLogIdx].ulTotAmnt);
      strcpy(msg, pcFrmDgts(FLOAT,aucTempMsgBuff));
      if (udtAppLang == L_ENGLISH)
        strcat(item, " "); 
      strcat(msg, GetMsgs(R_SDG));
      BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
      Rprintf("ulLogIdx= %d item:%s msg:%s\r\n",ulLogIdx, item, msg);

      memset(aucTempItemBuff,0,sizeof(aucTempItemBuff));
      sprintf(aucTempItemBuff,"%d",udtSumTransLog.audtSumTransItem[ulLogIdx].ulTotTrns);
      strcpy(item, pcFrmDgts(DIGIT,aucTempItemBuff));
      strcpy(msg, "");
      BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
      Rprintf("ulLogIdx= %d item:%s msg:%s\r\n",ulLogIdx, item, msg);

      ulOverallTotAmnt +=udtSumTransLog.audtSumTransItem[ulLogIdx].ulTotAmnt;
      ulOverallTotTrans += udtSumTransLog.audtSumTransItem[ulLogIdx].ulTotTrns;
    }
    strcpy(item, GetMsgs(R_TOTAL)); 
    memset(aucTempMsgBuff,0,sizeof(aucTempMsgBuff));
    sprintf(aucTempMsgBuff,"%d",ulOverallTotAmnt);
    strcpy(msg, pcFrmDgts(FLOAT,aucTempMsgBuff));
    if (udtAppLang == L_ENGLISH)
      strcat(msg, " "); 
    strcat(msg, GetMsgs(R_SDG));
    BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);
    Rprintf("ulLogIdx= %d item:%s msg:%s\r\n",ulLogIdx, item, msg);

    memset(aucTempItemBuff,0,sizeof(aucTempItemBuff));
    sprintf(aucTempItemBuff,"%d",ulOverallTotTrans);
    strcpy(item, pcFrmDgts(DIGIT,aucTempItemBuff));
    strcpy(msg, "");
    BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);


  }

   strcpy(item, "");
   strcpy(msg, GetMsgs(R_MERCH_CPY_HDR));
   BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);
   line = vidChkPrntPageSize(line);

  line += PRT_PAPER_FORM_FEED_LINE;

  DisplayMsgBox(S_PRNT, S_PLZ_WAIT,0, 0);

#if DEV_DEBUG
  OutputReceiptData(0,line);
#endif
  Rprintf("No of Lines :%d\r\n",line);
  ucPrinterWorking(0,line,TRUE);
}
/*
 * ************************************************************************************************
 *\Brief
 *\Param
 *\Return
 *****************************************************************************************************
 */
uchar ucConfigReport(void)
{

  int i, line,line1,line2;
  char item[33], msg[33];
  DATE_TIME udtTransDateTime;
  uchar aucTransDate[9];
  uchar aucTransTime[7];
  uchar aucTempBuff[9];
  uchar aucTempItemBuff[34];
  uchar aucTempMsgBuff[34];
  ulong ulIdx;
  ulong ulLogIdx;
  ulong ulOverallTotAmnt= 0;
  ulong ulOverallTotTrans= 0;
  bool bIsFoundOnLog = FALSE;


#if DEV_DEBUG
  Rprintf("\r\n");
  Rprintf("ucPrntDetailReport()\r\n");
#endif
  //Set printing preference for control density or contrast level
  //SetPrinterDensity(10);

  //check if the paper is existed
  while ((PAPER_READ() & 0xFF)!= 0xFF)
  {
    DisplayMsgBox(EMPTY_MSG, INS_PAPR,0, 0);
    // Printer Off-Line?
    if ((ONLINE_READ() & 0xFF) != 0xFF)
    {
      MAKEONLINE();
    }
  }

  InitializePrinterParam();

  DisplayMsgBox(S_RCT_PREP, S_PLZ_WAIT,0, 0);

  //Print Logo on Receipt
  line = PrintLogo(receiptLogo,0,0);

  strcpy(item, "");
  strcpy(msg, DataSave1->udtDeviceConfig.aucBankName[udtAppLang]);
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

  strcpy(item, "");
  strcpy(msg, DataSave1->udtDeviceConfig.aaucMerchantName[udtAppLang]);
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

  strcpy(item, "");
  strcpy(msg, DataSave1->udtDeviceConfig.aaucMerchantAddr[udtAppLang]);
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

  strcpy(item, "");
  strcpy(msg, GetMsgs(R_CONFIG_RPRT_HDR)); 
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_DOUBLE_WIDTH_MODE, CENTER_JUSTIFIED);

  if (GetRTCDateTime(&udtTransDateTime,0))
  {
    memset(aucTransTime,0,sizeof(aucTransTime));
    memset(aucTransDate,0,sizeof(aucTransDate));

    sprintf(aucTransTime, "%02X%02X%02X",
            udtTransDateTime.CLKHOUR, udtTransDateTime.CLKMIN, udtTransDateTime.CLKSEC);
    sprintf(aucTransDate,"%02X%02X%02X%02X",
            udtTransDateTime.CLKYEARH, udtTransDateTime.CLKYEARL,
            udtTransDateTime.CLKMONTH, udtTransDateTime.CLKDATE);
  }
  else
  {
    return FALSE;
  }
  
  strcpy(item, GetMsgs(R_TIME));
  //Rprintf("aucTransTime:%s",DataSave1->udtTransDetail.aucTransTime);
  strcpy(msg, pcFrmDgts(TIME,aucTransTime));
  //Rprintf("R_TIME:%s",msg);
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

  strcpy(item, GetMsgs(R_DATE));
  //Rprintf("aucTransDate:%s",DataSave1->udtTransDetail.aucTransDate);
  strcpy(msg, pcFrmDgts(DATE,aucTransDate));
  //Rprintf("R_DATE:%s",msg);
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

  Rprintf("R_TIME:%s\r\n",msg);

  strcpy(item, GetMsgs(R_TRM_ID));
  strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtDeviceConfig.aucTermId));
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

  strcpy(item, GetMsgs(R_MRCH_ID));
  strcpy(msg, pcFrmDgts(DIGIT,DataSave1->udtDeviceConfig.aucMerchantId));
  BuildLinePrint(0,line++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, RIGHT_JUSTIFIED);

  line1 = 0;
  strcpy(item, GetLangMsgs(R_APP_VER,L_ENGLISH));
  strcpy(msg, DataSave1->APP_Version);
  BuildLinePrint(1,line1++, L_ENGLISH, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

  strcpy(item, GetLangMsgs(R_APN1,L_ENGLISH));
  strcpy(msg, "");
  BuildLinePrint(1,line1++, L_ENGLISH, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);
  line1 = iPrintMultiLineMsg(1,L_ENGLISH, DataSave1->udtConnSettings.aucSIM1APN,line1, PRT_NORMAL_MODE, CENTER_JUSTIFIED);


  strcpy(item, GetLangMsgs(R_APN2,L_ENGLISH));
  strcpy(msg,"" );
  BuildLinePrint(1,line1++, L_ENGLISH, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);
  line1 = iPrintMultiLineMsg(1,L_ENGLISH, DataSave1->udtConnSettings.aucSIM2APN,line1, PRT_NORMAL_MODE, CENTER_JUSTIFIED);


  strcpy(item, GetLangMsgs(R_TERM_SERIAL_NO,L_ENGLISH));
  //GET_PROD_ID(msg);
  strcpy(msg,DataSave1->udtDeviceConfig.aucProdId);

  //strcpy(msg, udtRunData.acProdId);
  BuildLinePrint(1,line1++, L_ENGLISH, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

  line2 = 0;
  strcpy(item, "");
  strcpy(msg, GetMsgs(R_MERCH_CPY_HDR));
  BuildLinePrint(2,line2++, udtAppLang, item, strlen(item), msg, strlen(msg), PRT_NORMAL_MODE, CENTER_JUSTIFIED);

  line2 += PRT_PAPER_FORM_FEED_LINE;

  DisplayMsgBox(S_PRNT, S_PLZ_WAIT,0, 0);

#if DEV_DEBUG
  OutputReceiptData(0,line);
#endif
  ucPrinterWorking(0,line,TRUE);

  ucPrinterWorking(1,line1,FALSE);

  ucPrinterWorking(2,line2,TRUE);

  Rprintf("line:%d line1:%d line2:%d\r\n",line,line1,line2);

  Rprintf("Config Report Ended\r\n");
  return TRUE;
}
/*
 * ************************************************************************************************
 *\Brief
 *\Param
 *\Return
 *****************************************************************************************************
 */
uchar ucReportMenu(void)
{

  uchar ucselectedItem;
  int RV;
  ulong ulReceiptNo;
  uchar aucTempBuff[8];
  while (1)
  {
    if ((RV = DisplayMsgMenu(S_RPRT, &ucselectedItem, 10000, S_SUM_RPRT, S_CONFIG_RPRT)) == 0)
    {
      switch (ucselectedItem+1)
      {
      case 1:
        ucPrntReport(FALSE);
        break;
      case 2:
        ucPrntReport(TRUE);
        break;
      case 3:
        ucConfigReport();
        break;
      default:
        break;
      }
    }
    else if((RV ==1)||(RV ==-1))
    { //canceled or timed out
      break;
    }
  }

}
