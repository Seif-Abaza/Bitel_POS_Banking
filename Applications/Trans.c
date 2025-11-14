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
#include "../../BF_SDK/include/global.h"
#include "./fonts/fntmacro.h"
#include "./resources/Msgs.h"
#include "Display.h"
#include "AppGlobal.h"
#include "TransSndRcv.h"
#include "Receipt.h"
#include "Utilities.h"
#include "DeviceConfig.h"
#include "emv/EMV_Apdu.h"	//heba
#include "emv/EMV.h"	//heba
//#include "Bread.h"


/******************************************************************************/
/* TYPES                                                                      */
/******************************************************************************/
/******************************************************************************/
/* CONSTANTS, MACROS                                                          */
/******************************************************************************/
#define DEV_DEBUG  1
extern U08 gbForceOffline;

/******************************************************************************/
/* PRIVATE VARIABLES                                                          */
/******************************************************************************/
const ulong aulPCode[] =
{
	PCODE_BILL_PURCHASE,    //PURCH = 0,
//	PCODE_BILL_PAYMNT,      //BILL_PAY,
//	PCODE_BILL_PAYMNT,     //TOPUP_PAY,
//	PCODE_BILL_PAYMNT,     //ELECT,
//	PCODE_CARD_TRANS,      //CARD_TRANSFER,
//	PCODE_ACCNT_TRANS,     //ACC_TRANSFER,
//	PCODE_MONY_DEPO,       //MONEY_DEPOS,
//	PCODE_MONY_TRANS,      //MONEY_TRANSFER,
//	PCODE_MONY_RCV,        //MONEY_RCV,
//	PCODE_BAL_INQ,         //BAL_INQUIRY,
//	PCODE_PIN_CHNG,        //PIN_CHANGE,
	PCODE_VOID_SALE,       //VOID,
	PCODE_REFUND,          //REFUND,
//	PCODE_PURCH_CSHBK,     //PURCH_CASH_BACK,
	PCODE_SETTLEMENT,       //BATCH,
	PCODE_MINI_STAT,       //MINI_STATE,
	PCODE_ECHO,            //OTHR_SRVS
};

char acTransName[40];
/******************************************************************************/
/* PRIVATE FUNCTIONS PROTOTYPES                                               */
/******************************************************************************/
uchar ucSetPOSEntryCondMode(uchar ucPANInputMode,uchar ucPosCondCode);

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/*
 *\Brief
 *\Param
 *\return
 */
void Lprintf( char y, char x, char *str )
{
    int size;
    char *tmpbuf;

    size = strlen( str );
    if ( !size ) return;

    x &= 0x0F;
    y &= 0x03;
    if((x+size) > 16) size=16-x;

    tmpbuf = &DSPBUF0[0] + (16 * y);

    strncpy( &tmpbuf[x], str, size );
    LCD_DISPLAY ( 4, y, x, size );
}

/*
 *\Brief
 *\Param
 *\return
 */
void vidDispBillInqInfo(void)
{
  uchar aucBuff[20];
  uchar ucLine;
  ClrScr(CS_ALL, FALSE);
  //DisplayMsgBox(S_BL_INQ_HDR,EMPTY_MSG,0,0);

#if 0
  if ((udtAppLang == L_ARABIC) && (strlen(GetMsgs(S_BL_INQ_HDR)) > 0))
  {
    Highlight_Arabic(0, 0, GetMsgs(S_BL_INQ_HDR));
  }
  else
  { //english
           //SetDisplayMode(SDM_TEXT_ON | SDM_GRAPHIC_OFF, SDM_TEXT_AND);
    SelectFont(FONT_8x16EN); 
    if (udtAppLang == L_ARABIC)
    {
      TextOutA(0, 0, GetLangMsgs(S_BL_INQ_HDR, L_ENGLISH), FA_INVERT, -1, FALSE);
    }
    else
    {
      TextOutA(0, 0, GetMsgs(S_BL_INQ_HDR), FA_INVERT, -1, FALSE);
    }
    LcdDisplay(); //refresh the screan
  } //display it for the given time in ms
#endif    

  SelectFont(FONT_6x8);
  ucLine = 0;
  //SetDisplayMode(SDM_TEXT_ON | SDM_GRAPHIC_OFF, SDM_TEXT_AND);
  memset(aucBuff,0,sizeof(aucBuff));
  sprintf(aucBuff,"Mobile No:%s",DataSave1->udtTransDetail.aucAccountNo);
  TextOut(0,ucLine++,aucBuff);

  TextOut(0,ucLine++,"Contract No:");
  memset(aucBuff,0,sizeof(aucBuff));
  sprintf(aucBuff,"%s",udtBillInqInfo.aucContractNumber);
  TextOut(0,ucLine++,aucBuff);

  memset(aucBuff,0,sizeof(aucBuff));
  sprintf(aucBuff,"Bill Amount:%s",udtBillInqInfo.aucBillamount);
  TextOut(0,ucLine++,aucBuff);

  if (strlen(udtBillInqInfo.aucUNBillAmount)>0)
  {
    memset(aucBuff,0,sizeof(aucBuff));
    sprintf(aucBuff,"UNBill:%s",udtBillInqInfo.aucUNBillAmount);
    TextOut(0,ucLine++,aucBuff);
  }
  if (strlen(udtBillInqInfo.aucTotalBill)>0)
  {
    memset(aucBuff, 0, sizeof(aucBuff)); 
    sprintf(aucBuff,"Total Bill:%s",udtBillInqInfo.aucTotalBill);
    TextOut(0,ucLine++,aucBuff);
  }
  if (strlen(udtBillInqInfo.aucLastInvoiceDate)>0)
  {
    memset(aucBuff,0,sizeof(aucBuff));
    sprintf(aucBuff,"Date:%s",udtBillInqInfo.aucLastInvoiceDate);
    TextOut(0,ucLine++,aucBuff);
  }
  LcdDisplay();//refresh the screan
  DISPLY_InitFontData();
  //SelectFont(FONT_8x16);
  KEYPAD_WaitKey();
  ClrScr(CS_ALL, FALSE);
}
/*
 *\Brief
 *\Param
 *\return
 */
void GetCardOnlyBlock(char *pan, uchar *card_only_block)
{
  char size, temp[16];
  int i, j;

  memset(temp, '0', sizeof(temp));
  size = strlen(pan);
  if (size > 0)
  {
    size = strlen(pan) - 1;
  }
  if (size >= 12)
  {
    for (i = 0; i < 12; i++)
    {
      temp[15-i] = pan[(size-1)-i];
    }
  }
  else
  {
    for (i = 0; i < size; i++)
    {
      temp[4+i] = pan[i];
    }
  }

  for (i = 0, j = 0; i < 16; i++)
  {
    if ((i % 2) == 0)
    {
      card_only_block[j] = ((unsigned char)(temp[i] - 0x30) << 4) & 0xF0;
    }
    else
    {
      card_only_block[j] |= ((unsigned char)(temp[i] - 0x30)) & 0x0F;
      j++;
    }
  }
}
/*
 *\Brief
 *\Param
 *\return
 */
void GetPinOnlyBlock(char *plaintext_pin, unsigned char *pin_only_block)
{
  char size, data;
  int i, j;

  memset(pin_only_block, 0xFF, 8);
  size = strlen(plaintext_pin);
  if(size > 12)
  {
    size = 12;
  }

  pin_only_block[0] = size;
  for (i = 0, j = 1; i < size; i++)
  {
    if (i % 2 == 0)
    {
      pin_only_block[j] = (plaintext_pin[i] << 4) & 0xF0;
    }
    else
    {
      pin_only_block[j] |= plaintext_pin[i] & 0x0F;
      j++;
    }
  }
}
/*
 *\Brief
 *\Param
 *\return
 */
void GetEncryptedPinBlock(uchar *des_key,
                          char *pan,
                          char *plaintext_pin,
                          uchar *ciphertext_pin_block)
{

	  int i;
	  unsigned char plaintext[8], pin_only_block[8], card_only_block[8];

	  GetCardOnlyBlock(pan, card_only_block);
	  GetPinOnlyBlock(plaintext_pin, pin_only_block);
	  memset(plaintext, 0x00, 8);
	  for (i = 0; i < 8; i++)
	  {
		plaintext[i] = pin_only_block[i] ^ card_only_block[i];
	  }
	  des(plaintext, des_key, ENC_DES);
	  memcpy(ciphertext_pin_block, plaintext, 8);
}

/*
 * \Brief Encrypt PIN
 */

char ucEncryptPIN(uchar* pucPlainPIN,uchar* pucCiphertextPinBlock)
{
  uchar aucClearTPK[17];

#if DEV_DEBUG
  uchar ucIdx;
  //sprintf(DataSave1->udtDeviceConfig.aucTMK, "%s", "\xAB\xCD\xEF\x01\x23\x45\x67\x89");
  //sprintf(DataSave1->udtDeviceConfig.aucTPK, "%s", "\x49\x21\x0D\x18\x6E\xD5\xF7\x13");
  //sprintf(DataSave1->udtDeviceConfig.aucTPK, "%s", "\x10\x11\x12\x13\x14\x15\x16\x17");
  //sprintf(DataSave1->udtTransDetail.udtMSRCard.PAN, "%s", "6391754099923183");
#endif
  memset (aucClearTPK, 0, sizeof(aucClearTPK));
  sprintf(aucClearTPK,"%s",DataSave1->udtDeviceConfig.aucTPK);
  des(aucClearTPK, DataSave1->udtDeviceConfig.aucTMK, DEC_DES);

#if DEV_DEBUG
  Rprintf("PAN: %s\r\n",DataSave1->udtTransDetail.PAN);

  for (ucIdx = 0; ucIdx < strlen(DataSave1->udtDeviceConfig.aucTPK); ucIdx++)
  {
    Rprintf("aucTPK[%d]: %02x\r\n", ucIdx, DataSave1->udtDeviceConfig.aucTPK[ucIdx]);
  }
  //for (ucIdx = 0; ucIdx < strlen(aucClearTPK); ucIdx++) {
  //  Rprintf("aucClearTPK[%d]: %02x\r\n", ucIdx, aucClearTPK[ucIdx]);
  //}
  Rprintf("\r\n");
#endif
  GetEncryptedPinBlock(aucClearTPK,
                       DataSave1->udtTransDetail.PAN,
                       (char *)pucPlainPIN,
                       (char *)pucCiphertextPinBlock);
  //sprintf(pucCiphertextPinBlock,"%s","\x1b\xc4\x0e\x23\xe0\x9a\x69\xe1");
#if DEV_DEBUG
  Rprintf("pucCiphertextPinBlock: ");
  for (ucIdx = 0; ucIdx < strlen(pucCiphertextPinBlock); ucIdx++)
  {
    Rprintf("%02x",pucCiphertextPinBlock[ucIdx]);
  }
  Rprintf("\r\n");
#endif

}
/*
 * \Brief Display Amount prompt and set its value to transaction details
 * \Return 1 success
 * 			0 Otherwise
 */
bool bGetSetAmount(unsigned short headerIdx,
				   unsigned short displayedMsgIdx,
                   unsigned long ucMinAmount,
                   unsigned long ucMaxAmount)
{

  ulong ulAmount;
  ulong ulTotAmount;
  bool bRetVal;

//  bRetVal = Display_AmountPrompt(headerIdx,
//		  	  	  	  	  	  	 displayedMsgIdx,
//		  	  	  	  	  	  	 ucMinAmount,
//		  	  	  	  	  	  	 ucMaxAmount,
//		  	  	  	  	  	  	 &ulAmount);

  bRetVal = Display_InputNoAmountPrompt(headerIdx,
		  	  	  	  	  	  	 displayedMsgIdx,
		  	  	  	  	  	  	 ucMinAmount,
		  	  	  	  	  	  	 ucMaxAmount,
		  	  	  	  	  	  	 &ulAmount);
  if (bRetVal)
  {
		if (displayedMsgIdx == S_ENT_CASH_BK_AMNT)	//If cashbackAMount
		{
		  sprintf(DataSave1->udtTransDetail.aucCashBackAmount,"%d",ulAmount);
		  ulTotAmount = atol(DataSave1->udtTransDetail.aucAmount)+ulAmount;
		  memset(DataSave1->udtTransDetail.aucAmount,0,sizeof(DataSave1->udtTransDetail.aucAmount));
		  sprintf(DataSave1->udtTransDetail.aucAmount,"%04d",ulTotAmount);

		}
		else	//If normal AMount
		{
		  sprintf(DataSave1->udtTransDetail.aucAmount,"%04d",ulAmount);
		}
  }
  return bRetVal;
}
/*
 *\Brief Get Card Details in case of Magnetic or Manual
 *  //TODO add another check if it should fallback to EMV
 *
 *\Param	Displayed Header index
 *\Return	1 Success
 *			0 Failure
 */
uchar ucGetCardData(unsigned short headerIdx,bool isKeyEnabled)
{
  uchar arrucBuff[50];
  uchar ucPANInputMode;
  uchar ucLine;
  int iResult;
  bool RetVal = FALSE;
  int CState;

  //Wait for a card

  TRACK_SETUP(0);
  //Display Please swipe your card
  if(isKeyEnabled== TRUE)
  {
	   DisplayMsgBox(headerIdx,S_READ_CRD,0,false);
		Beep_EventSound(5);
  }


  SwipeCard:

  TRACK_SETUP(0);
  //keep Waiting for card
  iResult=MSR_WaitForCard();
  if (iResult ==0)
  {

	  //TODO check is it does support
    memset(arrucBuff, 0, sizeof(arrucBuff));
    //Check if PIN is allowed for Magnetic
    if (DataSave1->udtDeviceConfig.bIsSwipPinCap == TRUE)
    {
    	//ASk for PIN
      if (DispPrompt(headerIdx, S_ENT_CRD_PIN, 1, PIN_MAX_LEN, PSW_PROMPT, arrucBuff) == TRUE)
      {
        RetVal = TRUE;
        //Encrypt PIN
        ucEncryptPIN(arrucBuff,DataSave1->udtTransDetail.udtMSRCard.CiphertextPinBlock);
        DataSave1->udtTransDetail.udtMSRCard.bIsValidData = TRUE;
        //Set Entry mode to mag with PIN and Cond. code to normal
        if(DataSave1->udtTransDetail.bIsFallback == TRUE)
        {
        	 ucSetPOSEntryCondMode(PANINPUT_FLBK_MSR,PCC_NORMAL_PRESENTMENT);
        }
        else
        	ucSetPOSEntryCondMode(PANINPUT_MSR,PCC_NORMAL_PRESENTMENT);
      }
      else
    	  ucSetPOSEntryCondMode(PANINPUT_MSR_NOPIN,PCC_NORMAL_PRESENTMENT);
    }
    else
    {
      RetVal = TRUE;
      DataSave1->udtTransDetail.udtMSRCard.bIsValidData = TRUE;
      //Set Entry mode to mag without PIN and Cond. code to normal
      if(DataSave1->udtTransDetail.bIsFallback == TRUE)
      {
      	 ucSetPOSEntryCondMode(PANINPUT_FLBK_MSR_NOPIN,PCC_NORMAL_PRESENTMENT);
      }
      else
    	  ucSetPOSEntryCondMode(PANINPUT_MSR_NOPIN,PCC_NORMAL_PRESENTMENT);
    }

	  // 2- Set and get Amount
	  if (bGetSetAmount(udtRunData.usHdrIdx,S_ENT_AMNT,1,MAX_AMOUNT)!= TRUE)
	  {
		  RetVal = FALSE;

	  }
	  else
		  RetVal = TRUE;
  }
  else if (iResult == -2)	//In Case user press Enter. Manual Entry
  {
	  if(isKeyEnabled== FALSE)
	  {
		  RetVal = FALSE;
		  return 4;		//TODO change the returned value
	  }
	  //Clear Magnetic stripe details
    memset(&(DataSave1->udtTransDetail.udtMSRCard), 0, sizeof(strMSRCardData));

    memset(arrucBuff, 0, sizeof(arrucBuff));
    //Display prompt asking for card number
    if (DispPrompt(headerIdx, S_ENT_CRD_NO, 16,19, DIGIT_PROMPT, arrucBuff) == TRUE)
    {
    	//Store card Number
      sprintf(DataSave1->udtTransDetail.PAN,"%s",arrucBuff);

      memset(arrucBuff, 0, sizeof(arrucBuff));

      // ASk for Expiry date
        ucLine = DisplayMsgBox(headerIdx,S_EXP_DATE,0,false);
        Lprint(ucLine++, 0,"YYMM");
        if (Display_Input(ucLine, 1, 4,4, DIGIT_PROMPT, arrucBuff, udtAppLang, 0,headerIdx)!= VK_CANCEL)
        {
        	//Store Expiry date in format YYMM
        sprintf(DataSave1->udtTransDetail.ExpiryDate,"%s",arrucBuff);

        Rprintf("PAN: %s\r\n",DataSave1->udtTransDetail.PAN);
        Rprintf("ExpiryDate: %s\r\n",DataSave1->udtTransDetail.ExpiryDate);

        memset(arrucBuff, 0, sizeof(arrucBuff));
        if (DispPrompt(headerIdx, S_CVC2, 1, CVC2_MAX_LEN, PSW_PROMPT, arrucBuff) == TRUE)
        {
        	sprintf(DataSave1->udtTransDetail.aucCVC2,"%s",arrucBuff);
        	 Rprintf("CVC2: %s\r\n",DataSave1->udtTransDetail.aucCVC2);


       	  // 2- Set and get Amount
       	  if (bGetSetAmount(udtRunData.usHdrIdx,S_ENT_AMNT,1,MAX_AMOUNT)== TRUE)
       	  {


        	 memset(arrucBuff, 0, sizeof(arrucBuff));
        	 ucLine = DisplayMsgBox(headerIdx,S_AUTH,0,false);
             if (Display_Input(ucLine, 1, 6,6, DIGIT_PROMPT, arrucBuff, udtAppLang, 0,headerIdx)!= VK_CANCEL)
             {
             	sprintf(DataSave1->udtTransDetail.aucAuthIdResponse,"%s",arrucBuff);
             	 Rprintf("Auth: %s\r\n",DataSave1->udtTransDetail.aucAuthIdResponse);
				//Check PIN capability of terminal
				if (DataSave1->udtDeviceConfig.bIsManPinCap == TRUE)
				{
				  memset(arrucBuff, 0, sizeof(arrucBuff));
				  //ASk for PIN
				  if (DispPrompt(headerIdx, S_ENT_CRD_PIN, 1, PIN_MAX_LEN, PSW_PROMPT, arrucBuff) == TRUE)
				  {
					  //Encrypt PIN
					ucEncryptPIN(arrucBuff,DataSave1->udtTransDetail.udtMSRCard.CiphertextPinBlock);
					//Store Entry mode and conditional code
					ucSetPOSEntryCondMode(PANINPUT_MANUAL,PCC_NORMAL_PRESENTMENT);

					DataSave1->udtTransDetail.udtMSRCard.bIsValidData = TRUE;
					RetVal = TRUE;
				  }
				}
				else//If PIN capability not supported
				{
					//Store Entry mode and conditional code
				  ucSetPOSEntryCondMode(PANINPUT_MANUAL_NOPIN,PCC_NORMAL_PRESENTMENT);
				  DataSave1->udtTransDetail.udtMSRCard.bIsValidData = TRUE;
				  RetVal = TRUE;
				}
             }
       	  }
        }
      }
    }
  }
  else if(iResult == -3)	//Read Card
  {
	  DisplayMsgBox(headerIdx,S_INST_CRD,0,false);
	  Beep_EventSound(5);
	  ucSetPOSEntryCondMode(PANINPUT_ICCR_NOPIN,PCC_NORMAL_PRESENTMENT);
	  //TODO Read it Directly from chip
	  gsPOS.TraceNr = DataSave1->ulSysTraceAuditNo;
	  strcpy( gsPOS.CardAcceptorMerchantID,DataSave1->udtDeviceConfig.aucMerchantId);
	  strcpy( gsPOS.CardAcceptorTerminalID,DataSave1->udtDeviceConfig.aucTermId);
	  strcpy( gsPOS.TrmAcquirerID,"011200");
	  emv_start(headerIdx);
	  //check if card is removed
		 while(1)
		 {
//		      uckey = KEYSCAN() & 0xFF;
//		      if (uckey ==VK_CANCEL  )
//		      {
//		    	  RetVal = FALSE;
//		    	  break;
//		      }
		      if((CState=gsc_SetCardTYPEandSLOT(0x02, 0)) != -1)
		      {
		     	if(pgsSC_STAT->CARD)		//1 means card inserted
		     	{
		     		 DisplayMsgBox(headerIdx,S_REMOVE_CRD,0,false);

		     	}

		      }
		     	else
		     	{
		     		break;
		     	}
		 }
	  RetVal = 5;
  }
  else if(iResult == -4)	//In Case Chip existed
  {
	  unsigned char uckey=0xFF;

	 DisplayMsgBox(headerIdx,S_INST_CRD,0,false);
	 Beep_EventSound(5);
	 while(1)
	 {
	      uckey = KEYSCAN() & 0xFF;
	      if (uckey ==VK_CANCEL  )
	      {
	    	  RetVal = FALSE;
	    	  break;
	      }
	      else if((CState=gsc_SetCardTYPEandSLOT(0x02, 0)) != -1)
	      {
	     	if(pgsSC_STAT->CARD)		//1 means card inserted
	     	{
				//TODO Call EMV start
	     		ucSetPOSEntryCondMode(PANINPUT_ICCR,PCC_NORMAL_PRESENTMENT);
	     		  gsPOS.TraceNr = DataSave1->ulSysTraceAuditNo;
	     		  strcpy( gsPOS.CardAcceptorMerchantID,DataSave1->udtDeviceConfig.aucMerchantId);
	     		  strcpy( gsPOS.CardAcceptorTerminalID,DataSave1->udtDeviceConfig.aucTermId);
	     		  strcpy( gsPOS.TrmAcquirerID,"011200");
	     		emv_start(headerIdx);
	     		  //check if card is removed
	     			 while(1)
	     			 {
	     	//		      uckey = KEYSCAN() & 0xFF;
	     	//		      if (uckey ==VK_CANCEL  )
	     	//		      {
	     	//		    	  RetVal = FALSE;
	     	//		    	  break;
	     	//		      }
	     			      if((CState=gsc_SetCardTYPEandSLOT(0x02, 0)) != -1)
	     			      {
	     			     	if(pgsSC_STAT->CARD)		//1 means card inserted
	     			     	{
	     			     		 DisplayMsgBox(headerIdx,S_REMOVE_CRD,0,false);

	     			     	}

	     			      }
	     			     	else
	     			     	{
	     			     		break;
	     			     	}
	     			 }
	     		RetVal = 5;
	     	}
	      }
	      else if(CState ==-1)
	      {
	       	if(pgsSC_STAT->CARD)		//1 means not inserted
	       	{
	       		//Fallback
	        	DataSave1->udtTransDetail.bIsFallback = TRUE;
	        	DisplayMsgBox(headerIdx,S_SWP_CRD,0,false);
	        	Beep_EventSound(5);
	        	goto SwipeCard;
	       	}
	      }

	 }

  }
  else if(iResult == -5)	//In Case Chip existed
  {
	  DisplayMsgBox(headerIdx,S_SWP_CRD,0,false);
	  Beep_EventSound(5);
	  goto SwipeCard;

  }
  else
  {
    //ClearLCD();
    //Lprint(1, 0, "Read Error!  ");
    RetVal = FALSE;
    //WaitKeyPress10MS(100);
  }
  Rprintf("FLASEEEEEEEEEEEEEEEEEEEEEE=%d",RetVal);
  return RetVal;
}
/*
 *\Brief	Display 2 prompts to ask for data and re-confirmation. Then validated inserted data
 *\Param
 *\return
 */
bool GetConfirmed(ushort usHdrIdx, ushort usTxtIdx,
                  uchar ucMinLength, uchar ucMaxLength,
                  uchar* pucConfirmedNo)
{

  uchar aucRetMsg[50],aucSecRetMsg[50];
  uchar ucType = DIGIT_PROMPT;

  if (usTxtIdx == S_ENT_NW_PN)
  {
    ucType = PSW_PROMPT;
  } 
  while (1)
  {
    memset(aucRetMsg, 0, sizeof(aucRetMsg));
    memset(aucSecRetMsg, 0, sizeof(aucSecRetMsg));
    //Display first prompt
    if (DispPrompt(usHdrIdx, usTxtIdx, ucMinLength, ucMaxLength, ucType, aucRetMsg))
    {
    	//display the second prompt
      if (DispPrompt(usHdrIdx,usTxtIdx+1, ucMinLength, ucMaxLength, ucType, aucSecRetMsg))
      {
    	  //Compare inserted values
        if (strcmp(aucRetMsg,aucSecRetMsg) == 0)
        {
          sprintf(pucConfirmedNo,"%s",aucSecRetMsg);
          memset(DataSave1->udtTransDetail.aucAccountNo,0,sizeof(DataSave1->udtTransDetail.aucAccountNo));
          sprintf(DataSave1->udtTransDetail.aucAccountNo,"%s",aucSecRetMsg);
          return TRUE;
        }
        else	//Inserted data are not matched
        {
          Beep_EventSound(ERROR_TONE);
          DisplayMsgBox(usHdrIdx, S_NUM_DNT_MTCH,1000, 0);
        }
      }
    }
    else
    {
      break;
    }
  }
 return FALSE;
}

/*
 *\Brief Used in Top up transaction to display list of  amount values
 *\Param
 *\return
 */
uchar ucGetAmountMenu(ushort usHerderIdx, ushort usMsgIdx)
{
  char RV;
  ushort usSelectedItem;
  ulong ulAmount = 0;

  //Display menu of available values
  if((RV=DisplayMsgMenu(usMsgIdx,&usSelectedItem,10000,S_5_SDG,S_OTH_AMNT))==0)
  {
    if (usSelectedItem == 0)
    {
      ulAmount = 500;
    }
    else if (usSelectedItem == 1)
    {
      ulAmount = 1000;
    }
    else if (usSelectedItem == 2)
    {
      ulAmount = 2500;
    }
    else if (usSelectedItem == 3)
    {
      ulAmount = 5000;
    } else if (usSelectedItem == 4)
    {
      if (Display_AmountPrompt(usHerderIdx,S_ENT_AMNT,1,MAX_AMOUNT,&ulAmount)== FALSE)
      {
        return FALSE;
      }
    }
    sprintf(DataSave1->udtTransDetail.aucAmount,"%d",ulAmount);
    return TRUE;
  }
  else if((RV ==1)||(RV ==-1))
  { //canceled or timed out
    return FALSE;
  }
}


/*
 * \Brief Store Transaction date and time
 * 	 Time format : hhmmss
 * 	 Date Format :yyyymmdd
 *
 */
bool bSetTransDateTime(void)
{
  DATE_TIME udtTransDateTime;

  if (GetRTCDateTime(&udtTransDateTime,0))
  {
    sprintf(DataSave1->udtTransDetail.aucTransTime, "%02X%02X%02X",
            udtTransDateTime.CLKHOUR, udtTransDateTime.CLKMIN, udtTransDateTime.CLKSEC);

    sprintf(DataSave1->udtTransDetail.aucTransDate,"%02X%02X%02X%02X",
            udtTransDateTime.CLKYEARH, udtTransDateTime.CLKYEARL,
            udtTransDateTime.CLKMONTH, udtTransDateTime.CLKDATE);

    return TRUE;
  }
  return FALSE;
}

/*
 *\Brief
 *\Param
 *\return
 */
char * pcGetTransName(enuTransType udtTansType, enuVendType udtVendType)
{

  memset(acTransName,0,sizeof(acTransName));

  if ((udtTansType == BILL_PAY) || (udtTansType == TOPUP_PAY))
  {
    strcpy(acTransName,GetMsgs(udtVendType+S_ZAIN));
    strcat(acTransName, " ");
    strcat(acTransName, GetMsgs(udtTansType+S_PURCH));
  }
  else
  {
    strcpy(acTransName,GetMsgs(udtTansType+S_PURCH));
  }
  return acTransName; 
}

/*
 *\Brief Set the value of Entry mode(F22) and Conditional code(F25)
 */
uchar ucSetPOSEntryCondMode(uchar ucPANInputMode,uchar ucPosCondCode)
{
 // ushort pos_entry_mode;
  char pos_entry_mode[12];
  uchar ucICCRErrorFlag  = 0;
//  switch (ucPANInputMode)
//  {
//    case PANINPUT_MANUAL:  // Manual (Key in)
//      pos_entry_mode = PEM_MANUAL;
//      pos_entry_mode |= PEM_PIN_ENTRY_CAPABILITY;
//      break;
//    case PANINPUT_MSR:    // Magnetic stripe
//      if (!ucICCRErrorFlag)
//      {
//        pos_entry_mode = PEM_MSR;
//      }
//      else
//      {
//        pos_entry_mode = PEM_MSR_BY_BAD_ICCR;
//      }
//      pos_entry_mode |= PEM_PIN_ENTRY_CAPABILITY;
//      break;
//    case PANINPUT_ICCR:    // ICC reader
//      pos_entry_mode = PEM_ICCR;
//      pos_entry_mode |= PEM_PIN_ENTRY_CAPABILITY;
//      break;
//    case PANINPUT_MANUAL_NOPIN:
//      pos_entry_mode = PEM_MANUAL;
//      pos_entry_mode |= PEM_NO_PIN_ENTRY_CAPABILITY;
//      break;
//    case PANINPUT_MSR_NOPIN:    // Magnetic stripe No Pin
//      if (!ucICCRErrorFlag)
//      {
//        pos_entry_mode = PEM_MSR;
//      }
//      else
//      {
//        pos_entry_mode = PEM_MSR_BY_BAD_ICCR;
//      }
//      pos_entry_mode |= PEM_NO_PIN_ENTRY_CAPABILITY;
//      break;
//    default:        // Unspecified
//      pos_entry_mode = PEM_UNSPECIFIED;
//      break;
//  }

  switch (ucPANInputMode)
  {
    case PANINPUT_MANUAL:  // Manual (Key in)
     // pos_entry_mode = PEM_MANUAL;
      sprintf(DataSave1->udtTransDetail.aucPOSEntryMode,"%x%x",PEM_PIN,PEM_MANUAL_PIN);
      //pos_entry_mode |= PEM_PIN_ENTRY_CAPABILITY;
      break;
    case PANINPUT_MSR:    // Magnetic stripe
      if (!ucICCRErrorFlag)
      {
        //pos_entry_mode = PEM_MSR;
    	  sprintf(DataSave1->udtTransDetail.aucPOSEntryMode,"%x%x",PEM_PIN,PEM_MSR_PIN);
      }
      else
      {
        //pos_entry_mode = PEM_MSR_BY_BAD_ICCR;
        sprintf(DataSave1->udtTransDetail.aucPOSEntryMode,"%x%x",PEM_PIN,PEM_FLBK_MSR_ICCF);
      }
     // pos_entry_mode |= PEM_PIN_ENTRY_CAPABILITY;
      break;
    case PANINPUT_ICCR:    // ICC reader
     // pos_entry_mode = PEM_ICCR;
    	sprintf(DataSave1->udtTransDetail.aucPOSEntryMode,"%x%x",PEM_PIN,PEM_ICCR_PIN);
     // pos_entry_mode |= PEM_PIN_ENTRY_CAPABILITY;
      break;
    case PANINPUT_MANUAL_NOPIN:
     // pos_entry_mode = PEM_MANUAL;
    	sprintf(DataSave1->udtTransDetail.aucPOSEntryMode,"%x%x",PEM_NOPIN,PEM_MANUAL);
     // pos_entry_mode |= PEM_NO_PIN_ENTRY_CAPABILITY;
      break;
    case PANINPUT_ICCR_NOPIN:
        // pos_entry_mode = PEM_ICCR;
       	sprintf(DataSave1->udtTransDetail.aucPOSEntryMode,"%x%x",PEM_NOPIN,PEM_ICCR);
        // pos_entry_mode |= PEM_PIN_ENTRY_CAPABILITY;
       	break;
    case PANINPUT_MSR_NOPIN:    // Magnetic stripe No Pin
      if (!ucICCRErrorFlag)
      {
        //pos_entry_mode = PEM_MSR;
    	  sprintf(DataSave1->udtTransDetail.aucPOSEntryMode,"%x%x",PEM_NOPIN,PEM_MSR);
      }
      else
      {
        //pos_entry_mode = PEM_MSR_BY_BAD_ICCR;
        sprintf(DataSave1->udtTransDetail.aucPOSEntryMode,"%x%x",PEM_NOPIN,PEM_FLBK_MSR_ICCF);
      }
     // pos_entry_mode |= PEM_NO_PIN_ENTRY_CAPABILITY;
      break;
    default:        // Unspecified
      //pos_entry_mode = PEM_UNSPECIFIED;
      sprintf(DataSave1->udtTransDetail.aucPOSEntryMode,"%x%x",PEM_NOPIN,PEM_UNSPECIFIED);
      break;
  }
 // sprintf(DataSave1->udtTransDetail.aucPOSEntryMode,"%s",pos_entry_mode);
  DataSave1->udtTransDetail.usPOSEntryMode = pos_entry_mode;
  DataSave1->udtTransDetail.ucPosCondCode = ucPosCondCode;

  Rprintf("pos_entry_mode=%x,usPOSEntryMode:%d\r\n ucPosCondCode:%d\r\n",PEM_MSR,
          DataSave1->udtTransDetail.usPOSEntryMode,
          DataSave1->udtTransDetail.ucPosCondCode);
}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucEchoTest(void)
{

  bSetTransDateTime();
  DataSave1->udtTransDetail.ulPCode = PCODE_ECHO;
  udtRunData.usHdrIdx = S_EK_TST;
  ucEchoTestSndRcv();
}

/*
 *\Brief Store  transaction in memory and increment number of saved transaction
 *\Param
 *\return
 */

void vidSaveTansDetails(void)
{
 // if (DataSave1->udtTransDetail.udtTansType != VOID )
  {
    DataSave1->udtTransDetail.ulSysTraceAuditNo = (DataSave1->ulSysTraceAuditNo-1);
  }

  memcpy(&(DataSave1->udtTransStore.audtTrans[DataSave1->udtTransStore.ulSavedTransNo]),
         &DataSave1->udtTransDetail,sizeof(strTransDetail));

  if ( DataSave1->udtTransStore.ulSavedTransNo < MAX_TRNS_STORE_SIZE)
  {
    DataSave1->udtTransStore.ulSavedTransNo++;
  }
}

/*
 *\Brief Clear current reversal parameters
 *\Param
 *\return
 */
void vidClearReversal(void)
{
  memset(&DataSave1->udtRevesalData,0,sizeof(strRevesalData));
}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucSaveTansInfo(bool bIsSuccessTrans)
{

  switch (DataSave1->udtTransDetail.udtTansType)
  {
	  case PURCH:
	  case BILL_PAY:
	  case TOPUP_PAY:
	  case ELECT:
	  case CARD_TRANSFER:
	  case ACC_TRANSFER:
	  case MONEY_DEPOS:
	  case MONEY_TRANSFER:
	  case MONEY_RCV:
	  //case BAL_INQUIRY:
	  //case PIN_CHANGE:
	  case PURCH_CASH_BACK:
	  //case BATCH:
	  //case MINI_STATE:
	  case REFUND:
		DataSave1->ulReceiptNo++;
		if (bIsSuccessTrans)
		{
		  DataSave1->udtTransDetail.ulReceiptNo = DataSave1->ulReceiptNo;
		  DataSave1->udtTransDetail.ulSysTraceAuditNo = (DataSave1->udtTransDetail.ulSysTraceAuditNo-1);
		  memcpy(&(DataSave1->udtTransStore.audtTrans[DataSave1->udtTransStore.ulSavedTransNo++]),
				 &DataSave1->udtTransDetail,sizeof(strTransDetail));
		}
		break;
	  case VOID:
		if (bIsSuccessTrans)
		{
		  memcpy(&(DataSave1->udtTransStore.audtTrans[DataSave1->udtTransStore.ulSavedTransNo++]),
				 &DataSave1->udtTransDetail,sizeof(strTransDetail));
		}
		break;
	  default:
		break;
  }
}
/*
 *\Brief
 *\Param
 *\return
 */
int iChosePrintMerchCopy(ushort usHdrIdx)
{
  uchar *aucdlgBtns[2], aucTempBuff[50];
  int rv=-2;

  aucdlgBtns[0]=GetMsgs(S_YS);
  aucdlgBtns[1]=GetMsgs(S_NO);

  rv=DisplayModalDialog(GetMsgs(usHdrIdx), GetMsgs(R_PRINT_CPY_Q), "", 20000,aucdlgBtns,2,1);

  if (rv == 0)
  {
    ucPrntMerchantRct();
  }
  else if (rv == -1)
  {
    ucPrntMerchantRct();
  }
}
/******************************************************************************/
/* EXPORTED FUNCTIONS                                                         */
/******************************************************************************/
void vidSaveReversalInfo(void)
{

  if ((udtRunData.bIsCurrReversal == TRUE) || (udtRunData.bIsBatchUpload == TRUE))
  {
    return;
  }
  switch (DataSave1->udtTransDetail.ulPCode)
  {
    case PCODE_BILL_PURCHASE:  
    //case PCODE_BILL_PAYMNT:    
    case PCODE_CARD_TRANS:     
    case PCODE_ACCNT_TRANS:    
    case PCODE_MONY_DEPO:      
    case PCODE_MONY_TRANS:     
    case PCODE_MONY_RCV:       
    //case PCODE_BAL_INQ:      
    case PCODE_PIN_CHNG:       
    case PCODE_VOID_SALE:      
    case PCODE_PURCH_CSHBK:    
    //case PCODE_SETTLEMENT:   
    //case PCODE_MINI_STAT:    
    case PCODE_REFUND:         
    //PCODE_ECHO:         
      memset(&DataSave1->udtRevesalData,0,sizeof(strRevesalData));
      strcpy(DataSave1->udtRevesalData.aucTermId,DataSave1->udtDeviceConfig.aucTermId);
      memcpy(&DataSave1->udtRevesalData.udtMSRCard,
    		  &DataSave1->udtTransDetail.udtMSRCard,
    		  sizeof(strMSRCardData));
      DataSave1->udtRevesalData.ulSysTraceAuditNo = DataSave1->udtTransDetail.ulSysTraceAuditNo;
      DataSave1->udtRevesalData.ulPCode = DataSave1->udtTransDetail.ulPCode;
      DataSave1->udtRevesalData.bIsReversalReq = TRUE;
      break;
    default:
      break;
  }
}

/*
 * \Brief Start Sale Transaction
 *
 *\Return NA
 */
uchar ucPurchase(bool isDefualtMode)
{
  int ucResult;

  uchar pressedKey=0xFF;
	//3- Set Date and Time of transaction
  bSetTransDateTime();

	DataSave1->udtTransDetail.udtTansType= PURCH;
	DataSave1->udtTransDetail.MTI =	MTYPE_FINANCIAL_TRXN_REQUEST;
	DataSave1->udtTransDetail.ulPCode = PCODE_SALE;

	  //1- Wait for card to Get Card Details
	  gbForceOffline = 0;
    if ((pressedKey=ucGetCardData(udtRunData.usHdrIdx,isDefualtMode))== TRUE)
    {
//	  // 2- Set and get Amount
//	  if (bGetSetAmount(udtRunData.usHdrIdx,S_ENT_AMNT,1,MAX_AMOUNT)!= TRUE)
//	  {
//		  return 0;
//	  }
    	//3- Set Date and Time of transaction
      bSetTransDateTime();
      //ucSetPOSEntryCondMode(PANINPUT_MSR,PCC_NORMAL_PRESENTMENT);
      //DataSave1->udtTransDetail.udtTansType = PURCH;

      Rprintf("\r\nPAN[F2] =%s \r\n"
    		  "P Code[F3] =%d \r\n"
    		  "Amount[F4] =%d \r\n"
    		  "STAN [F11] =%d \r\n"
    		  "Time [F12] =%s \r\n"
    		  "Date [F13] =%s\r\n"
    		  "Expiry [F14]= %s\r\n"
    		  "Entry [F22]= %s\r\n"
    		  "Cond [F25]= %x\r\n"
    		  "SRV [F26]= %s\r\n"
    		  "Track2[F35]=%s\r\n"
    		  "Floor Limit= %d\r\n",DataSave1->udtTransDetail.PAN,
    		  	  	  	  	  	    DataSave1->udtTransDetail.ulPCode,
    		  	  	  	  	  	   atoi(DataSave1->udtTransDetail.aucAmount),
    		  	  	  	  	  	   DataSave1->ulSysTraceAuditNo,
    		  	  	  	  	  	   DataSave1->udtTransDetail.aucTransTime,
    		  	  	  	  	  	   DataSave1->udtTransDetail.aucTransDate,
    		  	  	  	  	  	   DataSave1->udtTransDetail.ExpiryDate,
    		  	  	  	  	  	   DataSave1->udtTransDetail.aucPOSEntryMode,
    		  	  	  	  	  	   DataSave1->udtTransDetail.ucPosCondCode,
    		  	  	  	  	  	   DataSave1->udtTransDetail.SrvCode,
    		  	  	  	  	  	   DataSave1->udtTransDetail.udtMSRCard.acTrack2,
    		  	  	  	  	  	   DataSave1->udtDeviceConfig.ulFloorLimit);


//      Rprintf("\r\nPAN[F2] =%s \r\n",DataSave1->udtTransDetail.udtMSRCard.PAN);

      //4- Send and Receive ISO Message
      //ucResult = ucPurchSndRcv();
      if(DataSave1->udtTransDetail.aucPOSEntryMode[6]=='5')
      {
    	  return pressedKey;
      }
      else
      {
		  if(atoi(DataSave1->udtTransDetail.aucAmount)<= DataSave1->udtDeviceConfig.ulFloorLimit)
		  {
			  DataSave1->udtTransDetail.bIsOffline= TRUE;
			  DataSave1->udtTransDetail.ulPCode= PCODE_OFFLINE_SALE;
		     // memcpy(DataSave1->udtTransDetail.aucAuthIdResponse,"000000",6);
		      DataSave1->udtTransDetail. MTI = MTYPE_FINANCIAL_TRXN_ADVICE;
			  ucResult= SUCCESS;
	    	  vidIncSTAN();
	          sprintf(DataSave1->udtTransDetail.aucAuthIdResponse,"%06d", (DataSave1->ulSysTraceAuditNo-1));
		  }
		  else
		  {
			  DataSave1->udtTransDetail. MTI = MTYPE_FINANCIAL_TRXN_REQUEST;
			  DataSave1->udtTransDetail.bIsOffline= FALSE;
			  ucResult =gsoap_NBE(NULL, 0,0);
		  }
      }

      if(ucResult==SUCCESS)
      {

//    	  vidIncSTAN();
        vidSaveTansDetails(); 	//Save Trans
        vidClearReversal();
        ucPrntCustomerRct();	//Print Receipt
        //Ask to print Merchant Copy
        iChosePrintMerchCopy(udtRunData.usHdrIdx);
      }
      else //if (ucResult==ERR_HOSTCODE)
      {
        vidClearReversal();
        ucPrntFailedRct();
      }
//      else if (DataSave1->bIsReversalReq)
//      {
//      }
      //ucPurchaseRct(PURCH,1,1);
    }

    return pressedKey;

}

/*
 *\Brief Start Bill Payment transaction
 *\Param
 *\return
 */
uchar ucBillPayment(void)
{

  char RV;
  ushort usSelectedItem;
  uchar aucConfirmedMsg[50];
  uchar ucResult;
  uchar *aucdlgBtns[2];
  int rv=-2;

  memset(aucConfirmedMsg, 0, sizeof(aucConfirmedMsg));
  while (1)
  {
	  //1- Choose Vendor
    if((RV=DisplayMsgMenu(udtRunData.usHdrIdx,&usSelectedItem,10000,S_ZAIN,S_CNR))==0)
    {
      if (((usSelectedItem + 1) >= 1) && ((usSelectedItem + 1) <= 4))
      {
    	  //2- Ask for Mobile Number and validate it
        if (GetConfirmed(udtRunData.usHdrIdx, S_ENT_FN_NO,1,10,aucConfirmedMsg) == TRUE)
        {
        	//3- Wait for card to Get Card Details
          if (ucGetCardData(udtRunData.usHdrIdx,TRUE)== TRUE)
          {
        	  //4- Set Date and Time of transaction
            bSetTransDateTime();

            // 5- Set Processing code TODO why?
            DataSave1->udtTransDetail.ulPCode = PCODE_BILL_INQ;
            //Set Selected vendor
            DataSave1->udtTransDetail.udtVendType = usSelectedItem;

            //6- Send and Receive ISO message
            ucResult = ucBillInqSndRcv(usSelectedItem);
            if(ucResult==SUCCESS)
            {
              vidDispBillInqInfo();
              aucdlgBtns[0]=GetMsgs(S_YS);
              aucdlgBtns[1]=GetMsgs(S_NO);
              rv=DispModalDialog(udtRunData.usHdrIdx, S_PY_NOW_Q,"", 20000,aucdlgBtns,2,1);
              if (rv == 0)
              {
            	  //Set and get Amount
                if (bGetSetAmount(udtRunData.usHdrIdx, S_ENT_AMNT,1,MAX_AMOUNT))
                {
                  DataSave1->udtTransDetail.ulPCode = PCODE_BILL_PAYMNT;
                  ucResult = ucPymntSndRcv(usSelectedItem,0);
                  if(ucResult==SUCCESS)
                  {
                    vidSaveTansDetails(); 
                    vidClearReversal();
                    ucPrntCustomerRct();
                    iChosePrintMerchCopy(udtRunData.usHdrIdx);
                  }
                  else if (ucResult==ERR_HOSTCODE)
                  {
                    vidClearReversal();
                    ucPrntFailedRct();
                  }
                  else if (DataSave1->bIsReversalReq)
                  {
                  }
                }
              }
              //WaitKeyPress10MS(10000);
            }
            else if (ucResult==ERR_HOSTCODE)
            {
            }

          }
        }
      }
    }
    else if((RV ==1)||(RV ==-1))
    {//canceled or timed out
      break;
    }
  }
}

/*
 *\Brief	Start Top UP transactions
 *\Param
 *\return
 */
uchar ucTopUpPayment(void)
{
  char RV;
  ushort usSelectedItem;
  ulong ulAmount = 0;
  uchar aucConfirmedMsg[50];
  uchar ucResult;

  memset(aucConfirmedMsg, 0, sizeof(aucConfirmedMsg));

  while (1)
  {
	  //1- Choose Vendor
    if((RV=DisplayMsgMenu(udtRunData.usHdrIdx,&usSelectedItem,10000,S_ZAIN,S_SDN))==0)
    {
      if (((usSelectedItem + 1) >= 1) && ((usSelectedItem + 1) <= 3))
      {
    	  //2- Ask for Mobile Number and validate it
        if (GetConfirmed(udtRunData.usHdrIdx, S_ENT_FN_NO,1,10,aucConfirmedMsg) == TRUE)
        {
        	//3- Ask to select The amount
          if (ucGetAmountMenu(udtRunData.usHdrIdx, S_TP_UP_VAL_Q)== TRUE)
          {
        	  //4- Wait for card to Get Card Details
            if (ucGetCardData(udtRunData.usHdrIdx,TRUE)== TRUE)
            {
            	//5- Set Date and Time of transaction
              bSetTransDateTime();
              //Set Selected vendor
              DataSave1->udtTransDetail.udtVendType = usSelectedItem;

              //6- Send and Receive Iso Transaction
              ucResult = ucPymntSndRcv(usSelectedItem,1);
              if(ucResult==SUCCESS)
              {
                vidSaveTansDetails(); 
                vidClearReversal();
                ucPrntCustomerRct();
                iChosePrintMerchCopy(udtRunData.usHdrIdx);
              }
              else if (ucResult==ERR_HOSTCODE)
              {
                vidClearReversal();
                ucPrntFailedRct();
              }
              else if (DataSave1->bIsReversalReq)
              {
              }
            }
          }
        }
      }
    }
    else if((RV ==1)||(RV ==-1))
    { //canceled or timed out
      break;
    }

  }
}

/*
 *\Brief Pay Electricity Bill
 *\Param
 *\return
 */
uchar ucElectricityNEC(void)
{
  uchar aucConfirmedMsg[50];
  uchar ucResult;

  memset(aucConfirmedMsg, 0, sizeof(aucConfirmedMsg));
  //1- Ask for Odometer value and validate it
  if (GetConfirmed(udtRunData.usHdrIdx, S_ENT_MTR_NO,1,11,aucConfirmedMsg) == TRUE)
  {
	  //2- Ask to select The amount
    if(ucGetAmountMenu(udtRunData.usHdrIdx,S_PURCH_VAL_Q)==TRUE)
    {
    	 //3- Wait for card to Get Card Details
      if (ucGetCardData(udtRunData.usHdrIdx,TRUE)== TRUE)
      {
    	  //4- Set Date and Time of transaction
        bSetTransDateTime();

        //5- Send and receive ISO Message
        ucResult = ucNECSndRcv();
        if(ucResult==SUCCESS)
        {
          vidSaveTansDetails(); 
          vidClearReversal();
          ucPrntCustomerRct();
          iChosePrintMerchCopy(udtRunData.usHdrIdx);
        }
        else if (ucResult==ERR_HOSTCODE)
        {
          vidClearReversal();
          ucPrntFailedRct();
        }
        else if (DataSave1->bIsReversalReq)
        {
        }
      }
    }
  }
}

/*
 *\Brief  Start Transfer Money to another card number
 *\Param
 *\return
 */
uchar ucCardTransfer(void)
{
  uchar aucConfirmedMsg[50];
  uchar ucResult;

  memset(aucConfirmedMsg, 0, sizeof(aucConfirmedMsg));

  //1- Ask for amount value
  if (bGetSetAmount(udtRunData.usHdrIdx, S_ENT_AMNT,1,MAX_AMOUNT))
  {
	  //2- Ask for card Number value and validate it
    if (GetConfirmed(udtRunData.usHdrIdx, S_ENT_CRD_NO,16,19,aucConfirmedMsg) == TRUE)
    {
    	//3- Wait for card to Get Card Details
      if (ucGetCardData(udtRunData.usHdrIdx,TRUE)== TRUE)
      {
    	  //4- Set Date and Time of transaction
        bSetTransDateTime();

        //5 - Send and Receive ISO Transaction
        ucResult = ucCardTransSndRcv();
        if(ucResult==SUCCESS)
        {
          vidSaveTansDetails(); 
          vidClearReversal();
          ucPrntCustomerRct();
          iChosePrintMerchCopy(udtRunData.usHdrIdx);
        }
        else if (ucResult==ERR_HOSTCODE)
        {
          vidClearReversal();
          ucPrntFailedRct();
        }

      }
    }
  }
}

/*
 *\Brief Start transfer money to another account
 *\Param
 *\return
 */
uchar ucAccountTransfer(void)
{
  uchar aucConfirmedMsg[50];
  uchar ucResult;

  memset(aucConfirmedMsg, 0, sizeof(aucConfirmedMsg));

  //1- Ask for amount value
  if (bGetSetAmount(udtRunData.usHdrIdx, S_ENT_AMNT,1,MAX_AMOUNT))
  {
	  //2- Ask for Account Number  and validate it
    if (GetConfirmed(udtRunData.usHdrIdx, S_ENT_ACNT_NO,1,30,aucConfirmedMsg) == TRUE)
    {
    	//3- Wait for card to Get Card Details
      if (ucGetCardData(udtRunData.usHdrIdx,TRUE)== TRUE)
      {
    	  //4- Set Date and Time of transaction
        bSetTransDateTime();

        //5 - Send and Receive ISO Transaction
        ucResult = ucAccntTransSndRcv();
        if(ucResult==SUCCESS)
        {
          vidSaveTansDetails(); 
          vidClearReversal();
          ucPrntCustomerRct();
          iChosePrintMerchCopy(udtRunData.usHdrIdx);
        }
        else if (ucResult==ERR_HOSTCODE)
        {
          vidClearReversal();
          ucPrntFailedRct();
        }

      }
    }
  }
}

/*
 *\Brief Start Deposit Transaction
 *\Param
 *\return
 */
uchar ucMoneyDeposit(void)
{
  uchar aucPswBuff[10];
  uchar ucResult;

  memset(aucPswBuff, 0, sizeof(aucPswBuff));
  while(1)
  {
	  //1- Ask for merchant Transaction
    if (DispPrompt(udtRunData.usHdrIdx,
    				S_ENT_MERCH_PAS,
    				1,
    				MRCHNT_PSW_MAX_LEN,
    				PSW_PROMPT,
    				aucPswBuff) == FALSE)
    {
      return FALSE;
    }
    if (strcmp(aucPswBuff, DataSave1->udtDeviceConfig.aucMerchantPassword) != 0)
    {
      DisplayMsgBox(udtRunData.usHdrIdx, S_WRNG_PAS,1000, 0);
    }
    else
    {
      break;
    }
  }; 

  //2- Ask for amount value
  if (bGetSetAmount(udtRunData.usHdrIdx, S_ENT_AMNT,1,MAX_AMOUNT))
  {
	  //3- Wait for card to Get Card Details
    if (ucGetCardData(udtRunData.usHdrIdx,TRUE)== TRUE)
    {
    	//4- Set Date and Time of transaction
      bSetTransDateTime();

      //5 - Send and Receive ISO Transaction
      ucResult=ucMoneyDepositSndRcv();
      if(ucResult==SUCCESS)
      {
        vidSaveTansDetails(); 
        vidClearReversal();
        ucPrntCustomerRct();
        iChosePrintMerchCopy(udtRunData.usHdrIdx);
      }
      else if (ucResult==ERR_HOSTCODE)
      {
        vidClearReversal();
        ucPrntFailedRct();
      }
    }
  }
}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucMoneyTransfer(void)
{
  uchar aucConfirmedMsg[50];
  uchar ucResult;


  memset(aucConfirmedMsg, 0, sizeof(aucConfirmedMsg));
  //1- Ask for amount value
  if (bGetSetAmount(udtRunData.usHdrIdx, S_ENT_AMNT,1,MAX_AMOUNT))
  {
	  //2 - Ask for Telephone number and validate it
    if (GetConfirmed(udtRunData.usHdrIdx, S_ENT_FN_NO,1,10,aucConfirmedMsg) == TRUE)
    {
    	//3- Wait for card to Get Card Details
      if (ucGetCardData(udtRunData.usHdrIdx,TRUE)== TRUE)
      {
    	  //4- Set Date and Time of transaction
        bSetTransDateTime();
          //5 - Send and Receive ISO Transaction
        ucResult=ucMoneyTransRcvSndRcv(TRUE);
        if(ucResult==SUCCESS)
        {
          vidSaveTansDetails(); 
          vidClearReversal();
          ucPrntCustomerRct();
          iChosePrintMerchCopy(udtRunData.usHdrIdx);
        }
        else if (ucResult==ERR_HOSTCODE)
        {
          vidClearReversal();
          ucPrntFailedRct();
        }

      }
    }
  }
}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucMoneyReceive(void)
{
  uchar aucBuff[50];
  uchar aucTeleBuff[20];
  uchar ucResult;

  memset(aucBuff, 0, sizeof(aucBuff));
  memset(aucTeleBuff, 0, sizeof(aucTeleBuff));
  if (DispPrompt(udtRunData.usHdrIdx, S_ENT_APRV_CD,1,6,DIGIT_PROMPT,aucBuff) == TRUE)
  {
    if (DispPrompt(udtRunData.usHdrIdx, S_ENT_FN_NO,1,10,DIGIT_PROMPT,aucTeleBuff) == TRUE)
    {
      strcpy(DataSave1->udtTransDetail.aucAuthIdResponse,aucBuff);
      strcpy(DataSave1->udtTransDetail.aucAccountNo,aucTeleBuff);
      bSetTransDateTime();
      ucSetPOSEntryCondMode(PANINPUT_MANUAL_NOPIN,PCC_NORMAL_PRESENTMENT);
      ucResult=ucMoneyTransRcvSndRcv(FALSE);
      if(ucResult==SUCCESS)
      {
        vidSaveTansDetails(); 
        vidClearReversal();
        ucPrntCustomerRct();
        iChosePrintMerchCopy(udtRunData.usHdrIdx);
      }
      else if (ucResult==ERR_HOSTCODE)
      {
        vidClearReversal();
        ucPrntFailedRct();
      }
    }
  }
}

/*
 *\Brief Start Balance Inquiry transaction
 *\Param
 *\return
 */

uchar ucBalanceInquiry()
{
  uchar ucResult;
  uchar *aucdlgBtns[2], aucTempBuff[50] ,aucBalBuff[50];
  int rv=-2;

  //1- Wait for card to Get Card Details
  if (ucGetCardData(udtRunData.usHdrIdx,TRUE)== TRUE)
  {
    bSetTransDateTime();
    //2 - Send and Receive ISO Transaction
    ucResult = ucBalInqSndRcv();
    if(ucResult==SUCCESS)
    {
      aucdlgBtns[0]=GetMsgs(S_YS);
      aucdlgBtns[1]=GetMsgs(S_NO);

      sprintf(aucTempBuff,"               %s",GetMsgs(S_PRNT_RCT));
      //DisplayMessageBox(GetMsgs(S_VOID_HDR),aucTempBuff,0,0);
      memset (aucBalBuff,0,sizeof(aucBalBuff));
      sprintf(aucBalBuff,"%s %s",GetLangMsgs(R_SDG,L_ENGLISH),DataSave1->udtTransDetail.aucBalance);

      //3- Ask tp print Receipt
      rv=DisplayModalDialog(GetMsgs(udtRunData.usHdrIdx),
    		  	  	  	  	 aucTempBuff,
    		  	  	  	  	 aucBalBuff,
    		  	  	  	  	 20000,
    		  	  	  	  	 aucdlgBtns,
    		  	  	  	  	 2,
    		  	  	  	  	 1);
      //rv=DispModalDialog(S_BLNC_INQ_HDR, S_PRNT_RCT, 20000,aucdlgBtns,2,1);
      if (rv == 0)
      {
        ucPrntMerchantRct();
      }
    }
    else if (ucResult==ERR_HOSTCODE)
    {
      ucPrntFailedRct();
    }

  }
}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucOtherServs(void)
{
  uchar ucselectedItem;
  uchar aucPswBuff[10];
  bool bIsPswCorrect = FALSE;
  int RV;

  while (1)
  {
    if ((RV = DisplayMsgMenu(udtRunData.usHdrIdx, &ucselectedItem, 10000, S_REPRNT, S_SETUP)) == 0)
    {
      switch (ucselectedItem+S_REPRNT)
      {
      case S_REPRNT:
        ucReprintMenu();
        break;
      case S_RPRT:
        ucReportMenu();
        break;
      case S_LANG:
        ucSetLang();
        break;
      case S_EK_TST:
        ucEchoTest();
        break;
//      case S_MERCH_OUT_PSW:
//    	ucSetConfirmMerchPassword();
//    	break;
      case S_SETUP:
        memset(aucPswBuff, 0, sizeof(aucPswBuff));
        while(1)
        {
			  if (DispPrompt(S_SETUP, S_ENT_ADMIN_PAS, 1, ADMIN_PSW_MAX_LEN, PSW_PROMPT, aucPswBuff) == FALSE)
			  {
				return FALSE;
			  }
			  if (strcmp(aucPswBuff,DataSave1->udtDeviceConfig.aucAdminPassword) != 0)
			  {
					if (strcmp(aucPswBuff,DBG_PSW) == 0)
					{
					  vidEnDisDbgPort();
					  break;
					}
					else
					{
					  DisplayMsgBox(S_SETUP, S_WRNG_PAS,1000, 0);
					}
			  }
			  else
			  {
					ucDeviceConfig();
					break;
			  }
        }; 
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
 *\Brief Start PIN change Transaction
 *\Param
 *\return
 */
uchar ucPINChange(void)
{
  uchar aucConfirmedMsg[50];
  char acCiphertextPinBlock[50];
  uchar ucResult;

  memset(aucConfirmedMsg, 0, sizeof(aucConfirmedMsg));
  memset(acCiphertextPinBlock, 0, sizeof(acCiphertextPinBlock));

  //1- Wait for card to Get Card Details
  if (ucGetCardData(udtRunData.usHdrIdx,TRUE)== TRUE)
  {
	  //2 - Ask for New PIN and validate it
    if (GetConfirmed(udtRunData.usHdrIdx, S_ENT_NW_PN,1,16,aucConfirmedMsg) == TRUE)
    {
    	//3- Set Date and Time of transaction
      bSetTransDateTime();

      //4- Encrypt PIN
      ucEncryptPIN(aucConfirmedMsg,acCiphertextPinBlock);
      //Rprintf("aucConfirmedMsg:%s acCiphertextPinBlock:%s\r\n",aucConfirmedMsg,acCiphertextPinBlock);

      //5 - Send and Receive ISO Transaction
      ucResult=ucPINChangeSndRcv(acCiphertextPinBlock);
      if(ucResult==SUCCESS)
      {
        vidClearReversal();
        ucPrntCustomerRct();
      }
      else if (ucResult==ERR_HOSTCODE)
      {
        vidClearReversal();
        ucPrntFailedRct();
      }
    }
  }

}
/*
 *\Brief  Cancel Transaction in the same Batch
 *\Param
 *\return
 */
uchar ucVoid(void)
{
  uchar aucPswBuff[10], aucRctBuff[10],aucTempBuff[50], aucTemp1Buff[50],aucTempSpace[16],*aucdlgBtns[2];
  uchar ucResult;
  ushort usWaittime;
  ulong ulRctNo, ulIdx;
  uchar aucFrmtStr[16];
  int rv=-2;
  bool bIsTransFound = FALSE;

  memset(aucPswBuff, 0, sizeof(aucPswBuff));
  memset(aucRctBuff, 0, sizeof(aucRctBuff));
  memset(aucTempBuff, 0, sizeof(aucTempBuff));
  memset(aucTemp1Buff, 0, sizeof(aucTemp1Buff));
  memset(aucTempSpace, 0, sizeof(aucTempSpace));
  memset(aucFrmtStr, 0, sizeof(aucFrmtStr));

  while(1)
  {
	  // 1- Ask for Merchant Password
    if (DispPrompt(udtRunData.usHdrIdx,
    			   S_ENT_MERCH_PAS,
    			   1,
    			   MRCHNT_PSW_MAX_LEN,
    			   PSW_PROMPT,
    			   aucPswBuff) == FALSE)
    {
      return FALSE;
    }
    if (strcmp(aucPswBuff, DataSave1->udtDeviceConfig.aucMerchantPassword) != 0)
    {
      DisplayMsgBox(udtRunData.usHdrIdx, S_WRNG_PAS,1000, 0);
    }
    else
    {
      break;
    }
  }; 

  //2- Ask for Receipt Number
  if (DispPrompt(udtRunData.usHdrIdx, S_ENT_RCT_NO,1,10,DIGIT_PROMPT,aucRctBuff) == TRUE)
  {
	  //3- Search for inserted Receipt Number in current Batch
    ulRctNo = atol(aucRctBuff);
    for (ulIdx = 0; ulIdx <DataSave1->udtTransStore.ulSavedTransNo; ulIdx++)
    {
    	Rprintf("ulSysTraceAuditNo=%ld\r\n",DataSave1->udtTransStore.audtTrans[ulIdx].ulSysTraceAuditNo);
		  if ((DataSave1->udtTransStore.audtTrans[ulIdx].ulSysTraceAuditNo == ulRctNo) &&
			  /*(DataSave1->udtTransStore.audtTrans[ulIdx].udtTansType == PURCH) &&*/
			  (DataSave1->udtTransStore.audtTrans[ulIdx].bIsVoided == FALSE))
		  {
			memset(&DataSave1->udtTransDetail,0,sizeof(strTransDetail));

			memcpy(&DataSave1->udtTransDetail,
					&(DataSave1->udtTransStore.audtTrans[ulIdx]),
					 sizeof(strTransDetail));

//			DataSave1->udtTransDetail.udtTansType = VOID;
//			DataSave1->udtTransDetail.ulPCode = PCODE_VOID_SALE;
//			DataSave1->udtTransDetail.MTI =	MTYPE_REVERSAL_REQUEST;
//	    	DataSave1->udtTransDetail.bIsOffline = 1;
//	    	sprintf(DataSave1->udtTransDetail.aucAddtionalData,"%s%05ld%s%s",DataSave1->udtTransStore.audtTrans[ulIdx].MTI,
//	    															DataSave1->udtTransStore.audtTrans[ulIdx].ulSysTraceAuditNo,
//	    															&DataSave1->udtTransStore.audtTrans[ulIdx].aucTransDate[2] ,
//	    															DataSave1->udtTransStore.audtTrans[ulIdx].aucTransTime);
			bIsTransFound = TRUE;
			break;
		  }
    }

    if (bIsTransFound == TRUE)	//If Transaction was found
    {
      aucdlgBtns[0] = GetMsgs(S_YS); 
      aucdlgBtns[1]=GetMsgs(S_NO);
      sprintf(aucTempBuff,"               %s",GetMsgs(S_VOID_AMNT_Q));

      DisplayMessageBox(GetMsgs(udtRunData.usHdrIdx),aucTempBuff,0,0);
      //Convert to floating point format 0.00
      FormatFloat(aucFrmtStr,DataSave1->udtTransDetail.aucAmount);
      memset(aucTempSpace,' ',16-strlen(aucFrmtStr)-strlen(GetLangMsgs(R_SDG,L_ENGLISH)));
      sprintf(aucTemp1Buff,"%s%s%s",GetLangMsgs(R_SDG,L_ENGLISH),aucTempSpace,aucFrmtStr);

      //4- Get confirmation of original Transaction amount
      rv=DisplayModalDialog(GetMsgs(udtRunData.usHdrIdx),
    		                aucTempBuff,
    		                aucTemp1Buff,
    		                500000,
    		                aucdlgBtns,
    		                2,
    		                1);
      if (rv == 0)
      {
    	  //5- Get Confirmation to cancel transaction
        rv=DisplayModalDialog(GetMsgs(udtRunData.usHdrIdx),
        					  GetMsgs(S_VOID_SALE_Q),
        					  "",
        					  50000,
        					  aucdlgBtns,
        					  2,
        					  1);
        if (rv == 0)
        {
        	//6- Set Date and Time of transaction
			  bSetTransDateTime();
			  //7- Send and Receive ISO Message
//			  ucResult = ucVoidSndRcv();
//			  if(ucResult==SUCCESS)
//			  {
				DataSave1->udtTransDetail.udtTansType = VOID;
				DataSave1->udtTransDetail.ulPCode = PCODE_VOID_SALE;
				DataSave1->udtTransDetail.MTI =	MTYPE_REVERSAL_REQUEST;
		    	//DataSave1->udtTransDetail.bIsOffline = 1;
				DataSave1->udtTransDetail.bIsOffline= DataSave1->udtTransStore.audtTrans[ulIdx].bIsOffline;
		    	sprintf(DataSave1->udtTransDetail.aucAddtionalData,"%x%06ld%s%s00000",DataSave1->udtTransStore.audtTrans[ulIdx].MTI,
		    															DataSave1->udtTransStore.audtTrans[ulIdx].ulSysTraceAuditNo,
		    															&DataSave1->udtTransStore.audtTrans[ulIdx].aucTransDate[2] ,
		    															DataSave1->udtTransStore.audtTrans[ulIdx].aucTransTime);



		    	vidIncSTAN();
				vidSaveTansDetails();
				vidClearReversal();
				DataSave1->udtTransStore.audtTrans[ulIdx].bIsVoided = TRUE;
				ucPrntCustomerRct();
				iChosePrintMerchCopy(udtRunData.usHdrIdx);
//			  }
//			  else if (ucResult==ERR_HOSTCODE)
//			  {
//				vidClearReversal();
//				ucPrntFailedRct();
//			  }
        }
      }
    }
    else //If Orginal Transaction not Found
    {
      DisplayMsgBox(udtRunData.usHdrIdx, S_RPRNT_NOT_EXST,1000, 0);
    }
  }
}
/*
 *\Brief Start Purchase and cashBack Amount
 *\Param
 *\return
 */
uchar ucPurchaseCashback(void)
{
  uchar ucResult;

  //1- Ask for Purchase amount value
  if (bGetSetAmount(udtRunData.usHdrIdx, S_ENT_PURCH_AMNT,1,MAX_AMOUNT))
  {
	  //2- Ask for Cashback amount value
    if (bGetSetAmount(udtRunData.usHdrIdx, S_ENT_CASH_BK_AMNT,1,MAX_AMOUNT))
    {
    	 //3- Wait for card to Get Card Details
      if (ucGetCardData(udtRunData.usHdrIdx,TRUE)== TRUE)
      {
    	  //4- Set Date and Time of transaction
        bSetTransDateTime();

        //5 - Send and Receive ISO Transaction
        ucResult = ucPurchaseCashbackSndRcv();
        if(ucResult==SUCCESS)
        {
          vidSaveTansDetails(); 
          vidClearReversal();
          ucPrntCustomerRct();
          iChosePrintMerchCopy(udtRunData.usHdrIdx);
        }
        else if (ucResult==ERR_HOSTCODE)
        {
          vidClearReversal();
          ucPrntFailedRct();
        }
        else if (DataSave1->bIsReversalReq)
        {
        }

      }
    }
  }

}

/*
 *\Brief
 *\Param
 *\return
 */
void vidBatchTimeChkr(void)
{

  DATE_TIME udtTransDateTime;
  uchar aucTime[5];
  uchar aucDate[9];
  uchar ucIdx;
  while (1)
  {
    if (GetRTCDateTime(&udtTransDateTime,0))
    {
      memset(aucTime,0,sizeof(aucTime));
      memset(aucDate,0,sizeof(aucDate));
      sprintf(aucTime, "%02X%02X", udtTransDateTime.CLKHOUR, udtTransDateTime.CLKMIN);
      sprintf(aucDate,"%02X%02X%02X%02X",
              udtTransDateTime.CLKYEARH, udtTransDateTime.CLKYEARL,
              udtTransDateTime.CLKMONTH, udtTransDateTime.CLKDATE);

      if (strcmp(aucDate,DataSave1->udtBatchAlarms.aucAlarmDate) != 0)
      {
        memset(DataSave1->udtBatchAlarms.abIsAlarmChecked,
        		FALSE,sizeof(DataSave1->udtBatchAlarms.abIsAlarmChecked));
        strcpy(DataSave1->udtBatchAlarms.aucAlarmDate,aucDate);
      }
      //Lprint(2, 0, aucTime);
      if (DataSave1->udtBatchAlarms.bIsAlarm == FALSE)
      {
        for (ucIdx = 0; ucIdx < DataSave1->udtBatchAlarms.ucAlarmsNo; ucIdx++)
        {
          if ((atol(aucTime) >= atol(DataSave1->udtBatchAlarms.aaucAlarmsTime[ucIdx])) &&
              (DataSave1->udtBatchAlarms.abIsAlarmChecked[ucIdx] == FALSE))
          {
            DataSave1->udtBatchAlarms.bIsAlarm = TRUE;
            DataSave1->udtBatchAlarms.abIsAlarmChecked[ucIdx] = TRUE;
            break;
          }
        }
      }
      Wait1MS(10000);
      //Wait1MS(30000);
    }
    else
    {
      //Lprint(2, 0, " V.V ");
      //Wait1MS(200);
      //Lprint(2, 0, " ^.^ ");
      //Wait1MS(200);
      //Lprint(2, 0, " -.- ");
      //Wait1MS(200);
      //Lprint(2, 0, " O.O ");

    }
  }
}
/*
 *\Brief
 *\Param
 *\return
 */
//uchar ucBatch(bool bWithPassWrd)
//{
//  uchar aucPswBuff[10];
//  uchar ucResult;
//  int iResult;
//
//  if (bWithPassWrd == TRUE)
//  {
//    memset(aucPswBuff, 0, sizeof(aucPswBuff));
//    while(1)
//    {
//      if (DispPrompt(udtRunData.usHdrIdx,
//    		  	  	  S_ENT_MERCH_PAS,
//    		  	  	  1,
//    		  	  	  MRCHNT_PSW_MAX_LEN,
//    		  	  	  PSW_PROMPT,
//    		  	  	  aucPswBuff) == FALSE)
//      {
//        return FALSE;
//      }
//      if (strcmp(aucPswBuff, DataSave1->udtDeviceConfig.aucMerchantPassword) != 0)
//      {
//        DisplayMsgBox(udtRunData.usHdrIdx, S_WRNG_PAS,1000, 0);
//      }
//      else
//      {
//        break;
//      }
//    };
//  }
//
//  if (DataSave1->udtTransStore.ulSavedTransNo > 0)
//  {
//
//    ChkSendReversal();
//
//    usCommHdrIdx = S_BTCH;
//
//    iSgnlStrength= GPRS_getSignalStrength();
//    DisplayMsgBox(usCommHdrIdx, S_TRY_CNCT,0, 0);
//    iResult = GPRS_Connect(DataSave1->udtConnSettings.aucServerIP,
//                                DataSave1->udtConnSettings.aucServerPort,
//                                (DataSave1->udtConnSettings.ulHostTimeout*1000));
//
//    if (iResult == GPRS_SUCCESS)
//    {
//      DisplayMsgBox(usCommHdrIdx, S_CNCT, 0, 0);
//    }
//    else
//    {
//      DisplayMsgBox(usCommHdrIdx, S_NOT_CNCT,1000, 0);
//      return FALSE;
//    }
//    DataSave1->udtTransDetail.ulPCode = PCODE_SETTLEMENT;
//    ucResult = ucBatchSndRcv();
//
//    if(ucResult==SUCCESS)
//    {
//      DataSave1->bIsBatchPending = FALSE;
//      ucGetSetAuthRefTansID();
//      ucPrntBatch();
//      memset(&DataSave1->udtTransStore,0,sizeof(strTransStore));
//      DataSave1->ulBatchNo++;
//    }
//    else if((ucResult==ERR_HOSTCODE) && (udtRunData.ucErrorExtCode == 95))
//    {
//      ucResult=ucBatchUploadAll();
//
//      if(ucResult==SUCCESS)
//      {
//        memset((void*)&(DataSave1->udtTransDetail),0,sizeof(strTransDetail));
//        usCommHdrIdx = S_BTCH;
//        DataSave1->udtTransDetail.ulPCode = PCODE_SETTLEMENT_TRAILER;
//        ucResult = ucBatchSndRcv();
//        if((ucResult==SUCCESS) || ((ucResult==ERR_HOSTCODE) && (udtRunData.ucErrorExtCode == 95)))
//        {
//          DataSave1->bIsBatchPending = FALSE;
//          ucGetSetAuthRefTansID();
//          ucPrntBatch();
//          memset(&DataSave1->udtTransStore,0,sizeof(strTransStore));
//          DataSave1->ulBatchNo++;
//        } else
//        {
//          //DataSave1->bIsBatchPending = TRUE;
//          ucDispMultiLineMsg(GetMsgs(S_PEND_BATCH),0,2000);
//        }
//      }
//      else
//      {
//        //DataSave1->bIsBatchPending = TRUE;
//        ucDispMultiLineMsg(GetMsgs(S_PEND_BATCH),0,2000);
//      }
//    }
//    else
//    {
//      //DataSave1->bIsBatchPending = TRUE;
//      ucDispMultiLineMsg(GetMsgs(S_PEND_BATCH),0,2000);
//    }
//    if (GPRS_DisConnect() == GPRS_SUCCESS)
//    {
//      Rprintf("Disconnected\r\n");
//    }
//    else
//    {
//      Rprintf("Disconnect Failed\r\n");
//    }
//  }
//  else
//  {
//    DisplayMsgBox(S_BATCH_HDR,S_NO_TRANS,1000,false);
//  }
//
//
//}

/*
 * \return  0: Success
 * 			-1: Failed
 */
uchar ucBatch(bool bWithPassWrd)
{
  uchar aucPswBuff[10];
  uchar ucResult;
  int iResult;

  if (bWithPassWrd == TRUE)
  {
    memset(aucPswBuff, 0, sizeof(aucPswBuff)); 
    while(1)
    {
      if (DispPrompt(udtRunData.usHdrIdx,
    		  	  	  S_ENT_MERCH_PAS,
    		  	  	  1,
    		  	  	  MRCHNT_PSW_MAX_LEN,
    		  	  	  PSW_PROMPT,
    		  	  	  aucPswBuff) == FALSE)
      {
        return FALSE;
      }
      if (strcmp(aucPswBuff, DataSave1->udtDeviceConfig.aucMerchantPassword) != 0)
      {
        DisplayMsgBox(udtRunData.usHdrIdx, S_WRNG_PAS,1000, 0);
      }
      else
      {
        break;
      }
    }; 
  }

  if (DataSave1->udtTransStore.ulSavedTransNo > 0)
  {


	  //**************************************************************************************************************
	  //TODO: Send Offline 1220

	  //**************************************************************************************************************
	  bSetTransDateTime();
	  unsigned int ulIdx=0;

	  for (ulIdx = 0; ulIdx < DataSave1->udtTransStore.ulSavedTransNo; ulIdx++)
	  {
		  if(DataSave1->udtTransStore.audtTrans[ulIdx].MTI == MTYPE_FINANCIAL_TRXN_ADVICE &&
				  DataSave1->udtTransStore.audtTrans[ulIdx].bIsVoided == FALSE)
		  {
			  DataSave1->udtTransDetail.ulPCode =DataSave1->udtTransStore.audtTrans[ulIdx].ulPCode;
		      if(gsoap_NBE(NULL,0,ulIdx)==000)
		      {
	        	  //Unsucess
	        	  Beep_EventSound(5);
	        	 // DisplayMsgBox(udtRunData.usHdrIdx,S_BATCH_MISMATCH,0,false);
		      }
		      else
		    	  return -1;
		  }
	  }
	  //**************************************************************************************************************
	  //TODO : send Void transactions 1420
	  //**************************************************************************************************************
	  for (ulIdx = 0; ulIdx < DataSave1->udtTransStore.ulSavedTransNo; ulIdx++)
	  {
		  if(DataSave1->udtTransStore.audtTrans[ulIdx].MTI == MTYPE_REVERSAL_REQUEST &&
			  DataSave1->udtTransStore.audtTrans[ulIdx].bIsOffline == 0)	//Orignal is online
		  {
			  DataSave1->udtTransDetail.ulPCode =DataSave1->udtTransStore.audtTrans[ulIdx].ulPCode;
		      if((iResult=gsoap_NBE(NULL,0,ulIdx))==000 ||
		    		 ( iResult== ALREADY_VOIDED)||						//Already voided)
		       		 ( iResult== VOIDED_SUCCESS)  )					//Void Correct
		      {
	        	  //Unsucess
	        	  Beep_EventSound(5);
	        	 // DisplayMsgBox(udtRunData.usHdrIdx,S_BATCH_MISMATCH,0,false);
		      }
		      else
		    	  return -1;
		  }
	  }

	  //**************************************************************************************************************
	  //Send Login request 1804
	  //**************************************************************************************************************
	  bSetTransDateTime();
      DataSave1->udtTransDetail.ulPCode = PCODE_LOGIN;

      if(gsoap_NBE(NULL,0,0)==800)
      {
    	  Beep_EventSound(5);

    	  //**************************************************************************************************************
    	  //send Reconcillation 1520
    	  //**************************************************************************************************************
          DataSave1->udtTransDetail.ulPCode = PCODE_SETTLEMENT;
          if((iResult=gsoap_NBE(NULL,0,0))== RECON_SUCCESS||
        		  (iResult ==BATCH_DONE))
          {
        	  Beep_EventSound(5);
        	  //DisplayMsgBox(udtRunData.usHdrIdx,S_BATCH_MATCH,0,false);
              DataSave1->bIsBatchPending = FALSE;
              ucPrntBatch();
              memset(&DataSave1->udtTransStore,0,sizeof(strTransStore));
              DataSave1->udtTransStore.ulSavedTransNo =0;
              DataSave1->ulBatchNo++;
              ucDispMultiLineMsg(GetMsgs(S_BATCH_MATCH),0,5000);

          }
          else if(iResult == RECON_NOTMATCH)
          {
        	  bSetTransDateTime();
        	  //**************************************************************************************************************
        	  //TODO : Send Upload request 	1304
        	  //**************************************************************************************************************
        	  DataSave1->udtTransDetail.ulPCode =PCODE_SETTLEMENT_TRAILER;
              if(gsoap_BatchUpload()== 000)
              {
            	  Beep_EventSound(5);
                  DataSave1->bIsBatchPending = FALSE;
                  ucPrntBatch();
                  memset(&DataSave1->udtTransStore,0,sizeof(strTransStore));
                  DataSave1->udtTransStore.ulSavedTransNo =0;
                  DataSave1->ulBatchNo++;
                  ucDispMultiLineMsg(GetMsgs(S_BATCH_MATCH),0,5000);
              }
              else
              {
            	  DataSave1->bIsBatchPending = TRUE;
            	  ucDispMultiLineMsg(GetMsgs(S_BATCH_MISMATCH),0,5000);
              }
          }
          else
          {
        	  return -1;
          }
      }
      else
      {
    	  return -1;
      }




  }
  else
  {

	    DisplayMsgBox(EMPTY_MSG,S_NO_TRANS,1500,FALSE);
  }

  return 0;


}
/*
 *\Brief
 *\Param
 *\return
 */
uchar ucMiniStatement(void)
{
  uchar ucResult;
  if (ucGetCardData(udtRunData.usHdrIdx,TRUE)== TRUE)
  {
    bSetTransDateTime();
    ucResult = ucMiniStatSndRcv();
    if(ucResult==SUCCESS)
    {
      ucPrntCustomerRct();
    }
    else if (ucResult==ERR_HOSTCODE)
    {
      ucPrntFailedRct();
    }

  }
}
/*
 *\Brief	Start Refund -Cancel Transaction after closing batch
 *\Param
 *\return
 */
uchar ucRefund(void)
{
  uchar aucPswBuff[10];
  uchar ucResult;

  memset(aucPswBuff, 0, sizeof(aucPswBuff));
  while(1)
  {
	  // 1- Ask for Merchant Password
    if (DispPrompt(udtRunData.usHdrIdx,
    				S_ENT_MERCH_PAS,
    				1,
    				MRCHNT_PSW_MAX_LEN,
    				PSW_PROMPT,
    				aucPswBuff) == FALSE)
    {
      return FALSE;
    }
    if (strcmp(aucPswBuff, DataSave1->udtDeviceConfig.aucMerchantPassword) != 0)
    {
      DisplayMsgBox(udtRunData.usHdrIdx, S_WRNG_PAS,1000, 0);
    }
    else
    {
      break;
    }
  }; 

  //2- Ask for amount value
//  if (bGetSetAmount(udtRunData.usHdrIdx, S_ENT_AMNT,1,MAX_AMOUNT))
//  {
	  //3- Wait for card to Get Card Details
  gbForceOffline=1;

    if (ucGetCardData(udtRunData.usHdrIdx,TRUE)>= TRUE)
    {
    	//4- Set Date and Time of transaction
    	bSetTransDateTime();

    	DataSave1->udtTransDetail.udtTansType= REFUND;
    	DataSave1->udtTransDetail.MTI =	MTYPE_FINANCIAL_TRXN_ADVICE;
    	DataSave1->udtTransDetail.ulPCode = PCODE_REFUND;
    	DataSave1->udtTransDetail.ulActionCode =0;
    	DataSave1->udtTransDetail.bIsOffline = 1;
    	//memcpy(DataSave1->udtTransDetail.aucAuthIdResponse,"000000",6);
    	Rprintf("DataSave1->udtTransDetail.udtTansTyp=%u\r\n DataSave1->udtTransDetail.ulPCode=%x\r\n",DataSave1->udtTransDetail.udtTansType,
    																								DataSave1->udtTransDetail.ulPCode);
        vidIncSTAN();

        sprintf(DataSave1->udtTransDetail.aucAuthIdResponse,"%06d", (DataSave1->ulSysTraceAuditNo-1));

        vidSaveTansDetails(); 
        vidClearReversal();
        ucPrntCustomerRct();	//Print customer receipt
        iChosePrintMerchCopy(udtRunData.usHdrIdx);

    }
//  }
}
