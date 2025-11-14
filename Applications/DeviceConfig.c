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

#include "./resources/Msgs.h"
#include "Display.h"
#include "DeviceConfig.h"
#include "AppGlobal.h"
#include "Utilities.h"
#include "minilzo.h"
#include "resources/logo.h"
/******************************************************************************/
/* Extern                                                                       */
/******************************************************************************/
extern const char GraphicTxt[];
/******************************************************************************/
/* CONSTANTS, MACROS                                                          */
/******************************************************************************/
const strVedInstitCode audtInitVedInstitCode[5] =
{
	{ // ZAIN
	  {"31",	"22"},//Payment
	  {"33",	"01"},//Top-Up
	},

	{ //MTN
	  {"43",	"11"},//Payment
	  {"32",	"01"},//Top-Up
	},
	{ // SUDANI
	  {"36",	"08"}, //Payment
	  {"38",	"08"}, //Top-Up
	},
	{ // CANAR
	  {"42",	"22"},//Payment
	  {"00",	"00"},
	},
	{ // NEC
	  {"35",	"01"},//Payment
	  {"00",	"00"},
	}
};
 


/******************************************************************************/
/* PRIVATE VARIABLES                                                          */
/******************************************************************************/
DATE_TIME udtDateTime;
static  uchar aaucBatchAlarmMenu[20][5];

/******************************************************************************/
/* PRIVATE FUNCTIONS PROTOTYPES                                               */
/******************************************************************************/
/* We want to compress the data block at 'in' with length 'IN_LEN' to
 * the block at 'out'. Because the input block may be incompressible,
 * we must provide a little more output space in case that compression
 * is not possible.
 */


//#if defined(__LZO_STRICT_16BIT)
//#define IN_LEN      (8*1024u)
//#elif defined(LZO_ARCH_I086) && !defined(LZO_HAVE_MM_HUGE_ARRAY)
//#define IN_LEN      (60*1024u)
//#else
//#define IN_LEN      (128*1024ul)
//#endif
//#define OUT_LEN     (IN_LEN + IN_LEN / 16 + 64 + 3)

//static unsigned char __LZO_MMODEL in1  [ IN_LEN ];
//static unsigned char __LZO_MMODEL out1 [ OUT_LEN ];


/* Work-memory needed for compression. Allocate memory in units
 * of 'lzo_align_t' (instead of 'char') to make sure it is properly aligned.
 */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/
unsigned char ucSetDateTime(void)
{
  unsigned char aucDate[9];
  unsigned char aucTime[7];
  unsigned char Temp[20];

  memset(aucDate,0,sizeof(aucDate));
  memset(aucTime,0,sizeof(aucTime));
  memset(Temp,0,sizeof(Temp));

  if (GetRTCDateTime(&udtDateTime,0) == 1)
  {
    sprintf(aucTime, "%02X%02X%02X",
            udtDateTime.CLKHOUR,
            udtDateTime.CLKMIN,
            udtDateTime.CLKSEC);
    sprintf(aucDate,"%02X%02X%02X%02X",
            udtDateTime.CLKYEARH,
            udtDateTime.CLKYEARL,
            udtDateTime.CLKMONTH, 
            udtDateTime.CLKDATE);
    if (DispPrompt(R_DATE,S_DATE_FRMT,8,8,DIGIT_PROMPT, aucDate)== TRUE)
    {
      if (DispPrompt(R_TIME,S_TIME_FRMT,6,6, DIGIT_PROMPT, aucTime)== TRUE)
      {
        sprintf(Temp,"%s%s",aucDate,aucTime);
        SetRTCDateTime(Temp,0);
      }
    }
  }
}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucSetPosParameters (void)
{

  uchar ucselectedItem;
  uchar aucTempBuff[30];
  ulong ulSavedTransNo;
 // ulSavedTransNo = hex_long((char*)DataSave2->udtTransLog.LogHdr.aucSavedTransNo,2) + hex_long((uchar*)DataSave2->udtTransLog.LogHdr.aucSavedTransNo,2);
  ulSavedTransNo = DataSave1->udtTransStore.ulSavedTransNo;
  Rprintf("ulSavedTransNo:%d\r\n",ulSavedTransNo);
  memset(aucTempBuff, 0, sizeof(aucTempBuff));

  memcpy(aucTempBuff,DataSave1->udtDeviceConfig.aucBankName[udtAppLang],strlen(DataSave1->udtDeviceConfig.aucBankName[udtAppLang]));
  if (DispPrompt(S_POS_PARAM_HDR,S_BANK_NAME,1,33, ALPAH_PROMPT,aucTempBuff) == true )
  {
    if (strcmp(DataSave1->udtDeviceConfig.aucBankName[udtAppLang],aucTempBuff)!= 0)
    {
      if (ulSavedTransNo == 0)
      {
        sprintf(DataSave1->udtDeviceConfig.aucBankName[udtAppLang], "%s", aucTempBuff); 
      }
      else
      {
        ucDispMultiLineMsg(GetMsgs(S_BTCH_NOT_EMPTY),0,2000);
      }
    }

    memset(aucTempBuff, 0, sizeof(aucTempBuff));
    memcpy(aucTempBuff,DataSave1->udtDeviceConfig.aucTermId,strlen(DataSave1->udtDeviceConfig.aucTermId));
    if ((DispPrompt(S_POS_PARAM_HDR,S_POS_ID,1,8, DIGIT_PROMPT,aucTempBuff) == true ))
    {
      if (strcmp(DataSave1->udtDeviceConfig.aucTermId,aucTempBuff)!= 0)
      {
        if (ulSavedTransNo == 0)
        {
        	sprintf(DataSave1->udtDeviceConfig.aucTermId,"%s",aucTempBuff);
        }
        else
        {
        	ucDispMultiLineMsg(GetMsgs(S_BTCH_NOT_EMPTY),0,2000);
        }
      }
      memset(aucTempBuff, 0, sizeof(aucTempBuff));
      memcpy(aucTempBuff,DataSave1->udtDeviceConfig.aucMerchantId,strlen(DataSave1->udtDeviceConfig.aucMerchantId));
      if (DispPrompt(S_POS_PARAM_HDR,S_MRECH_ID,1,15, DIGIT_PROMPT,aucTempBuff) == true )
      {
        if (strcmp(DataSave1->udtDeviceConfig.aucMerchantId,aucTempBuff)!= 0)
        {
          if (ulSavedTransNo == 0)
          {
        	  sprintf(DataSave1->udtDeviceConfig.aucMerchantId,"%s",aucTempBuff);
          }
          else
          {
        	  ucDispMultiLineMsg(GetMsgs(S_BTCH_NOT_EMPTY),0,2000);
          }
        }

        memset(aucTempBuff, 0, sizeof(aucTempBuff));
        memcpy(aucTempBuff,
        		DataSave1->udtDeviceConfig.aaucMerchantName[udtAppLang],
        		strlen(DataSave1->udtDeviceConfig.aaucMerchantName[udtAppLang]));

        if (DispPrompt(S_POS_PARAM_HDR,S_MERCH_NAME,1,33, ALPAH_PROMPT,aucTempBuff) == true )
        {
          if (strcmp(DataSave1->udtDeviceConfig.aaucMerchantName[udtAppLang],aucTempBuff)!= 0)
          {
            if (ulSavedTransNo == 0)
            {
            	sprintf(DataSave1->udtDeviceConfig.aaucMerchantName[udtAppLang],"%s",aucTempBuff);
            }
            else
            {
            	ucDispMultiLineMsg(GetMsgs(S_BTCH_NOT_EMPTY),0,2000);
            }
          }

          memset(aucTempBuff, 0, sizeof(aucTempBuff));
          memcpy(aucTempBuff,
        		  DataSave1->udtDeviceConfig.aaucMerchantAddr[udtAppLang],
        		  strlen(DataSave1->udtDeviceConfig.aaucMerchantAddr[udtAppLang]));

          if (DispPrompt(S_POS_PARAM_HDR,S_MERCH_ADDR,1,33, ALPAH_PROMPT,aucTempBuff) == true )
          {
            if (strcmp(DataSave1->udtDeviceConfig.aaucMerchantAddr[udtAppLang],aucTempBuff)!= 0)
            {
              if (ulSavedTransNo == 0)
              {
            	  sprintf(DataSave1->udtDeviceConfig.aaucMerchantAddr[udtAppLang],"%s",aucTempBuff);
              }
              else
              {
            	  ucDispMultiLineMsg(GetMsgs(S_BTCH_NOT_EMPTY),0,2000);
              }
            }
          }
        }
      }

      //memset(aucTempBuff, 0, sizeof(aucTempBuff));
      //memcpy(aucTempBuff,DataSave1->udtDeviceConfig.aucTermName[udtAppLang],strlen(DataSave1->udtDeviceConfig.aucTermName[udtAppLang]));
      //if (DispPrompt(S_POS_PARAM_HDR,S_POS_NAME,1,15, ALPAH_EN_PROMPT,aucTempBuff) == true )
      //{
      //  sprintf(DataSave1->udtDeviceConfig.aucTermName[udtAppLang],"%s",aucTempBuff);
      //
      //}

    }
  }
}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucSetGPRSParam(void)
{
  uchar ucselectedItem;
  uchar aucTempBuff[61];
  bool bIsSim1Chng = FALSE;
  bool bIsSim2Chng = FALSE;

  memset(aucTempBuff, 0, sizeof(aucTempBuff));
  memcpy(aucTempBuff,
		  DataSave1->udtConnSettings.aucSIM1APN,
		  strlen(DataSave1->udtConnSettings.aucSIM1APN));

  if (DispPrompt(S_GPRS_PRM_HDR,S_SIM1_APN,0,60, ALPAH_EN_PROMPT,aucTempBuff) == true )
  {
    if (strcmp(DataSave1->udtConnSettings.aucSIM1APN,aucTempBuff) != 0)
    {
      memset(DataSave1->udtConnSettings.aucSIM1APN,
    		  0,
    		  sizeof(aucTempBuff,DataSave1->udtConnSettings.aucSIM1APN));

      memcpy(DataSave1->udtConnSettings.aucSIM1APN,aucTempBuff,strlen(aucTempBuff));
      bIsSim1Chng = TRUE;
    }

    memset(aucTempBuff, 0, sizeof(aucTempBuff));
    memcpy(aucTempBuff,
    		DataSave1->udtConnSettings.aucSIM1UsrName,
    		strlen(DataSave1->udtConnSettings.aucSIM1UsrName));

    if (DispPrompt(S_GPRS_PRM_HDR,S_SIM1_USER_NAME,0,60, ALPAH_EN_PROMPT,aucTempBuff) == true )
    {
      if (strcmp(DataSave1->udtConnSettings.aucSIM1UsrName,aucTempBuff) != 0)
      {
        memset(DataSave1->udtConnSettings.aucSIM1UsrName,
        		0,
        		sizeof(aucTempBuff,DataSave1->udtConnSettings.aucSIM1UsrName));

        memcpy(DataSave1->udtConnSettings.aucSIM1UsrName,aucTempBuff,strlen(aucTempBuff));
        bIsSim1Chng = TRUE;
      }

      memset(aucTempBuff, 0, sizeof(aucTempBuff));
      memcpy(aucTempBuff,
    		  DataSave1->udtConnSettings.aucSIM1Password,
    		  strlen(DataSave1->udtConnSettings.aucSIM1Password));

      if (DispPrompt(S_GPRS_PRM_HDR,S_SIM1_PSW,0,60, ALPAH_EN_PROMPT,aucTempBuff) == true )
      {
        if (strcmp(DataSave1->udtConnSettings.aucSIM1Password,aucTempBuff) != 0)
        {
          memset(DataSave1->udtConnSettings.aucSIM1Password,
        		  0,
        		  sizeof(aucTempBuff,DataSave1->udtConnSettings.aucSIM1Password));

          memcpy(DataSave1->udtConnSettings.aucSIM1Password,aucTempBuff,strlen(aucTempBuff));
          bIsSim1Chng = TRUE;
        }

        memset(aucTempBuff, 0, sizeof(aucTempBuff));
        memcpy(aucTempBuff,
        		DataSave1->udtConnSettings.aucSIM2APN,
        		strlen(DataSave1->udtConnSettings.aucSIM2APN));

        if (DispPrompt(S_GPRS_PRM_HDR,S_SIM2_APN,0,60, ALPAH_EN_PROMPT,aucTempBuff) == true )
        {
          if (strcmp(DataSave1->udtConnSettings.aucSIM2APN,aucTempBuff) != 0)
          {
            memset(DataSave1->udtConnSettings.aucSIM2APN,
            		0,
            		sizeof(aucTempBuff,DataSave1->udtConnSettings.aucSIM2APN));
            memcpy(DataSave1->udtConnSettings.aucSIM2APN,aucTempBuff,strlen(aucTempBuff));
            bIsSim2Chng = TRUE;
          }

          memset(aucTempBuff, 0, sizeof(aucTempBuff));
          memcpy(aucTempBuff,
        		  DataSave1->udtConnSettings.aucSIM2UsrName,
        		  strlen(DataSave1->udtConnSettings.aucSIM2UsrName));

          if (DispPrompt(S_GPRS_PRM_HDR,S_SIM2_USER_NAME,0,60, ALPAH_EN_PROMPT,aucTempBuff) == true )
          {
            if (strcmp(DataSave1->udtConnSettings.aucSIM2UsrName,aucTempBuff) != 0)
            {
              memset(DataSave1->udtConnSettings.aucSIM2UsrName,
            		  0,
            		  sizeof(aucTempBuff,DataSave1->udtConnSettings.aucSIM2UsrName));

              memcpy(DataSave1->udtConnSettings.aucSIM2UsrName,aucTempBuff,strlen(aucTempBuff));
              bIsSim2Chng = TRUE;
            }

            memset(aucTempBuff, 0, sizeof(aucTempBuff));
            memcpy(aucTempBuff,
            		DataSave1->udtConnSettings.aucSIM2Password,
            		strlen(DataSave1->udtConnSettings.aucSIM2Password));

            if (DispPrompt(S_GPRS_PRM_HDR,S_SIM2_PSW,0,60, ALPAH_EN_PROMPT,aucTempBuff) == true )
            {
              if (strcmp(DataSave1->udtConnSettings.aucSIM2Password,aucTempBuff) != 0)
              {
                memset(DataSave1->udtConnSettings.aucSIM2Password,
                		0,
                		sizeof(aucTempBuff,DataSave1->udtConnSettings.aucSIM2Password));
                memcpy(DataSave1->udtConnSettings.aucSIM2Password,aucTempBuff,strlen(aucTempBuff));
                bIsSim2Chng = TRUE;
              }

            }
          }
        }
      }
    }
  }
  if ((bIsSim1Chng == TRUE) || (bIsSim2Chng == TRUE))
  {
    GPRSInitRegConnect(S_PLZ_WAIT);
  }
}
/*
 *\Brief
 *\Param
 *\return
 */
uchar ucSetNetworkParam(void)
{
  uchar ucselectedItem;
  uchar aucTempBuff[61];
  memset(aucTempBuff, 0, sizeof(aucTempBuff));

  memcpy(aucTempBuff,
		  DataSave1->udtConnSettings.aucServerIP,
		  strlen(DataSave1->udtConnSettings.aucServerIP));
  //ClearLCD();
  //Lprint(3, 0, aucTempBuff);
  //KEYPAD_WaitKey();
    if (DispPrompt(S_BNK_IP, EMPTY_MSG, 16, 16, IP_PROMPT, aucTempBuff) == TRUE) 
    {
 
      memset(DataSave1->udtConnSettings.aucServerIP, 0, sizeof(DataSave1->udtConnSettings.aucServerIP));
      sprintf(DataSave1->udtConnSettings.aucServerIP, "%s", aucTempBuff);
    }
    else
    {
      return FALSE;
    }
  memset(aucTempBuff, 0, sizeof(aucTempBuff));
  memcpy(aucTempBuff,
		  DataSave1->udtConnSettings.aucServerPort,
		  strlen(DataSave1->udtConnSettings.aucServerPort));

  if (DispPrompt(S_BNK_PORT,EMPTY_MSG,2,8, DIGIT_PROMPT,aucTempBuff) == true ) 
  {
    memset(DataSave1->udtConnSettings.aucServerPort, 0, sizeof(DataSave1->udtConnSettings.aucServerPort));
    sprintf(DataSave1->udtConnSettings.aucServerPort,"%s",aucTempBuff);
  }
  memset(aucTempBuff, 0, sizeof(aucTempBuff));
  sprintf(aucTempBuff,"%d",DataSave1->udtConnSettings.ulHostTimeout);
  if (DispPrompt(S_HOST_TIMOUT,S_TIMOUT_SEC,1,4, DIGIT_PROMPT,aucTempBuff) == true )
  {
    DataSave1->udtConnSettings.ulHostTimeout = atol(aucTempBuff);
  }
}
/*
 *\Brief
 *\Param
 *\return
 */
uchar ucSetVendInstitCodes()
{

  char RV;
  ushort usSelectedItem1;
  ushort usSelectedItem2;
	ulong ulAmount = 0;

	uchar aucBuff[5];
	memset(aucBuff, 0, sizeof(aucBuff));

  while (1)
  {
    if((RV=DisplayMsgMenu(S_VEND_INSTIT_CODE,&usSelectedItem1,10000,S_ZAIN,S_NEC))==0)// if input is ok
    {
      switch (usSelectedItem1+1)
      {
      case 1:
      case 2:
      case 3:
//        if((RV=DisplayMsgMenu(S_VEND_INSTIT_CODE,&usSelectedItem2,10000,S_BL_PAY,S_TP_PAY))==0)// if input is ok
//        {
//          switch (usSelectedItem2+1)
//          {
//          case 1:
//          case 2:
//            memset(aucBuff, 0, sizeof(aucBuff));
//            memcpy(aucBuff,DataSave1->udtDeviceConfig.audtVedInstitCode[usSelectedItem1][usSelectedItem2].aucVendCode,2);
//            if (DispPrompt((S_BL_PAY + usSelectedItem2),S_VEND_CODE, 2, 2,DIGIT_PROMPT, aucBuff ) == TRUE)
//            {
//              memcpy(DataSave1->udtDeviceConfig.audtVedInstitCode[usSelectedItem1][usSelectedItem2].aucVendCode,aucBuff,2);
//
//              memset(aucBuff, 0, sizeof(aucBuff));
//              memcpy(aucBuff,DataSave1->udtDeviceConfig.audtVedInstitCode[usSelectedItem1][usSelectedItem2].aucInstitCode,2);
//              if (DispPrompt((S_BL_PAY + usSelectedItem2),S_INSTIT_CODE, 2, 2,DIGIT_PROMPT, aucBuff ) == TRUE)
//              {
//                memcpy(DataSave1->udtDeviceConfig.audtVedInstitCode[usSelectedItem1][usSelectedItem2].aucInstitCode,aucBuff,2);
//              }
//            }
//            break;
//          default:
//            break;
//          }
//        }
        break;
      case 4:
      case 5:
        memset(aucBuff, 0, sizeof(aucBuff));
        memcpy(aucBuff, DataSave1->udtDeviceConfig.audtVedInstitCode[usSelectedItem1][0].aucVendCode,2);
        if (DispPrompt((S_ZAIN + usSelectedItem1),S_VEND_CODE, 2, 2,DIGIT_PROMPT, aucBuff ) == TRUE)
        {
          memcpy(DataSave1->udtDeviceConfig.audtVedInstitCode[usSelectedItem1][0].aucVendCode,aucBuff,2);

          memset(aucBuff, 0, sizeof(aucBuff));
          memcpy(aucBuff, DataSave1->udtDeviceConfig.audtVedInstitCode[usSelectedItem1][0].aucInstitCode,2);
          if (DispPrompt((S_ZAIN + usSelectedItem1),S_INSTIT_CODE, 2, 2,DIGIT_PROMPT, aucBuff ) == TRUE)
          {
            memcpy(DataSave1->udtDeviceConfig.audtVedInstitCode[usSelectedItem1][0].aucInstitCode, aucBuff,2);
          }
        }
        break;
      default:
         break;
      }
    }
    else if((RV ==1)||(RV ==-1))//canceled or timed out
    {
      break;
    }

  }

}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucSwipeEntryPinCap(void)
{
	  uchar *aucdlgBtns[2];
	  ushort usDefaultChoice;
	  int rv=-2;

	  aucdlgBtns[0]=GetMsgs(S_YS);
	  aucdlgBtns[1]=GetMsgs(S_NO);
	  if (DataSave1->udtDeviceConfig.bIsSwipPinCap == TRUE)
	  {
		usDefaultChoice = 0;
	  }
	  else
	  {
		usDefaultChoice = 1;
	  }
	  rv = DispModalDialog(S_SWIP_ENT_PIN, S_ACTIV_Q, "", 20000, aucdlgBtns, 2, usDefaultChoice);
	  if (rv == 0)
	  {
		DataSave1->udtDeviceConfig.bIsSwipPinCap = TRUE;
	  }
	  else if (rv == 1)
	  {
		DataSave1->udtDeviceConfig.bIsSwipPinCap = FALSE;
	  }

}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucManulEntryPinCap(void)
{

  uchar *aucdlgBtns[2];
  ushort usDefaultChoice;
  int rv=-2;

  aucdlgBtns[0]=GetMsgs(S_YS);
  aucdlgBtns[1]=GetMsgs(S_NO);
  if (DataSave1->udtDeviceConfig.bIsManEnt == TRUE)
  {
    usDefaultChoice = 0;
  }
  else
  {
    usDefaultChoice = 1;
  }
  rv = DispModalDialog(S_MAN_ENT, S_ACTIV_Q, "", 20000, aucdlgBtns, 2, usDefaultChoice); 
  if (rv == 0)
  {
		DataSave1->udtDeviceConfig.bIsManEnt = TRUE;
		if (DataSave1->udtDeviceConfig.bIsManPinCap == TRUE)
		{
		  usDefaultChoice = 0;
		}
		else
		{
		  usDefaultChoice = 1;
		}
		rv = DispModalDialog(S_PIN_CAPA, S_ACTIV_Q, "", 20000, aucdlgBtns, 2, usDefaultChoice);
		if (rv == 0)
		{
		  DataSave1->udtDeviceConfig.bIsManPinCap = TRUE;
		}
		else if (rv == 1)
		{
		  DataSave1->udtDeviceConfig.bIsManPinCap = FALSE;
		}
  }
  else if (rv == 1)
  {
    DataSave1->udtDeviceConfig.bIsManEnt = FALSE;
  }

}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucSetTMK(void)
{
  uchar ucselectedItem;
  uchar aucTempBuff[20];
  memset(aucTempBuff, 0, sizeof(aucTempBuff));
  //num_to_asc(aucTempBuff,DataSave1->udtDeviceConfig.aucTMK,(strlen(DataSave1->udtDeviceConfig.aucTMK)*2));
  //memcpy(aucTempBuff,DataSave1->udtDeviceConfig.aucTMK,strlen(DataSave1->udtDeviceConfig.aucTMK));
  if (DispPrompt(S_CHNG_TMK,EMPTY_MSG,1,16, HEX_PROMPT,aucTempBuff) == true )
  {
    memset(DataSave1->udtDeviceConfig.aucTMK, 0, sizeof(DataSave1->udtDeviceConfig.aucTMK));
    asc_to_num(DataSave1->udtDeviceConfig.aucTMK,aucTempBuff,strlen(aucTempBuff));
    //sprintf(DataSave1->udtDeviceConfig.aucTMK,"%s",aucTempBuff);
  }

}

///*
// *\Brief
// *\Param
// *\return
// */
//uchar ucDistSetting()
//{
//  uchar aucTempBuff[20];
//  memset(aucTempBuff, 0, sizeof(aucTempBuff));
//
//  memset(aucTempBuff, 0, sizeof(aucTempBuff));
//
//  bcd2asc(aucTempBuff,DataSave1->udtDeviceConfig.aucDistInCode,2*strlen(DataSave1->udtDeviceConfig.aucDistInCode));
//
//  if (DispPrompt(S_DIST_SET,S_INCODE,16,16, HEX_PROMPT,aucTempBuff) == true )
//  {
//    memset(DataSave1->udtDeviceConfig.aucDistInCode, 0, sizeof(DataSave1->udtDeviceConfig.aucDistInCode));
//    asc2bcd(DataSave1->udtDeviceConfig.aucDistInCode,aucTempBuff,strlen(aucTempBuff));
//  }
//  memset(aucTempBuff, 0, sizeof(aucTempBuff));
//  sprintf(aucTempBuff,"%d",DataSave1->udtDeviceConfig.ulDistQuota);
//  if (DispPrompt(S_DIST_SET,S_QUOTA,1,5, DIGIT_PROMPT,aucTempBuff) == true )
//  {
//    DataSave1->udtDeviceConfig.ulDistQuota = atol(aucTempBuff);
//  }
//}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucSetAdminPassword(void)
{
  uchar ucselectedItem;
  uchar aucTempBuff[20];
  memset(aucTempBuff, 0, sizeof(aucTempBuff));
  //memcpy(aucTempBuff,DataSave1->udtDeviceConfig.aucAdminPassword,strlen(DataSave1->udtDeviceConfig.aucAdminPassword));
  if (DispPrompt(S_ADMIN_PSW,S_ENT_PAS,1,ADMIN_PSW_MAX_LEN, PSW_PROMPT,aucTempBuff) == true )
  {
    memset(DataSave1->udtDeviceConfig.aucAdminPassword,
    		0,
    		sizeof(DataSave1->udtDeviceConfig.aucAdminPassword));

    memcpy(DataSave1->udtDeviceConfig.aucAdminPassword,aucTempBuff,strlen(aucTempBuff));
  }
}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucSetMerchPassword(void)
{
  uchar ucselectedItem;
  uchar aucTempBuff[20];
  memset(aucTempBuff, 0, sizeof(aucTempBuff));
  //memcpy(aucTempBuff,DataSave1->udtDeviceConfig.aucMerchantPassword,strlen(DataSave1->udtDeviceConfig.aucMerchantPassword));
  if (DispPrompt(S_MERCH_PSW,S_ENT_PAS,1,MRCHNT_PSW_MAX_LEN, PSW_PROMPT,aucTempBuff) == true )
  {
    memset(DataSave1->udtDeviceConfig.aucMerchantPassword,
    		0,
    		sizeof(DataSave1->udtDeviceConfig.aucMerchantPassword));
    memcpy(DataSave1->udtDeviceConfig.aucMerchantPassword,aucTempBuff,strlen(aucTempBuff));
  }
}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucSetLang(void)
{
  uchar ucselectedItem;

  if((DisplayMsgMenu(S_LANG,&ucselectedItem,10000,S_ENG,S_ARA))==0)
  {
    if ((ucselectedItem+1) == 1)
    {
      DataSave1->udtDeviceConfig.udtAppLang = L_ENGLISH;
    }
    else
    {
      DataSave1->udtDeviceConfig.udtAppLang = L_ARABIC;
    }
    udtAppLang = DataSave1->udtDeviceConfig.udtAppLang;
  }
}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucNetworkSettings(void)
{
  uchar ucselectedItem;
  int RV;


  while (1)
  {
    if ((RV = DisplayMsgMenu(S_NETWRK_SETNGS, &ucselectedItem, 10000, S_GP_PRM, S_BNK_IP)) == 0)
    {
      if ((ucselectedItem+1) == 1)
      {
        ucSetGPRSParam();
      }
      else
      {
        ucSetNetworkParam();
      }
    } else if((RV ==1)||(RV ==-1))
    {
      break;
    }
  }
}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucChngKeys(void)
{
  uchar ucselectedItem;
  int RV;

  while (1)
  {
    if ((RV =DisplayMsgMenu(S_CHNG_KEYS, &ucselectedItem, 10000, S_CHNG_TMK, S_CHNG_TRK)) == 0)
    {
      if ((ucselectedItem+1) == 1)
      {
        ucSetTMK();
      }
      else
      {
//        bSetTransDateTime();
        DataSave1->udtTransDetail.ulPCode = PCODE_NEW_KEY; 
        udtRunData.usHdrIdx = S_CHNG_TRK;
        ucNewKeysInq();
      }
    }
    else if((RV ==1)||(RV ==-1))
    {
      break;
    }
  }
}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucChngPswrds(void)
{
  uchar ucselectedItem;
  int RV;
  while (1)
  {
    if ((RV = DisplayMsgMenu(S_CHNG_PSWS, &ucselectedItem, 10000, S_MERCH_PSW, S_ADMIN_PSW)) == 0)
    {
      if ((ucselectedItem+1) == 1)
      {
        ucSetMerchPassword();
      }
      else
      {
        ucSetAdminPassword();
      }
    }
    else if((RV ==1)||(RV ==-1))
    {
      break;
    }
  }
}

/*
 *\Brief
 *\Param
 *\return
 */
void vidDiplayVersion(void)
{
  uchar aucBuff[20];

  memset(aucBuff, 0, sizeof(aucBuff));
  DisplayMsgBox(S_VERSION,EMPTY_MSG,0,0);
  Lprint(1,0,DataSave1->APP_Project);
  Lprint(2,0,DataSave1->APP_Version);
  Lprint(3,0,SU_CURRENT_BLD);
  WaitKeyPress10MS(500);
}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucClearReversal(void)
{
  uchar *aucdlgBtns[2];
  int rv=-2;

  aucdlgBtns[0]=GetMsgs(S_YS);
  aucdlgBtns[1]=GetMsgs(S_NO);
  rv=DisplayModalDialog(GetMsgs(S_CLR_RVRSL), GetMsgs(S_RST_Q),"", 5000,aucdlgBtns,2,1);
  if (rv == 0)
  {
    if (DataSave1->udtRevesalData.bIsReversalReq == TRUE)
    {
      if (DataSave1->udtRevesalData.ulPCode != PCODE_VOID_SALE)
      {
        vidIncSTAN();
      }
    }
    memset(&(DataSave1->udtRevesalData)  ,0,sizeof(strRevesalData));
  }

}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucSetBatchNo(void)
{
  uchar aucTempBuff[20];
  memset(aucTempBuff, 0, sizeof(aucTempBuff));
  //sprintf(aucTempBuff,"%d",DataSave3->ulBatchCounter);
  sprintf(aucTempBuff,"%ld",DataSave1->ulBatchNo);
  if (DispPrompt(S_BTCH_NO,EMPTY_MSG,1,7, DIGIT_PROMPT,aucTempBuff) == true )
  {
    //DataSave3->ulBatchCounter = atol(aucTempBuff);
	  DataSave1->ulBatchNo = atol(aucTempBuff);
  }
}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucEmptyBatch(void)
{
  uchar *aucdlgBtns[2];
  int rv=-2;

  aucdlgBtns[0]=GetMsgs(S_YS);
  aucdlgBtns[1]=GetMsgs(S_NO);
  rv=DisplayModalDialog(GetMsgs(S_EMPTY_BATCH), GetMsgs(S_RST_Q),"", 5000,aucdlgBtns,2,1);
  if (rv == 0)
  {
    //memset((void *)DataSave2,0,sizeof(DATASAVEPAGE2));
	memset(&DataSave1->udtTransDetail,0,sizeof(strTransDetail));
	memset(&DataSave1->udtTransStore,0,sizeof(strTransStore));
	memset(&DataSave1->udtRevesalData,0,sizeof(strRevesalData));

    DataSave1->bIsBatchPending = 0;
    DataSave1->ulSysTraceAuditNo= 453;
    DataSave1->ulReceiptNo = 1;//P-11 Receipt No
    DataSave1->ulBatchNo = 1;
    DataSave1->bIsReversalReq = 0;
    DataSave1->bIsBatchPending = 0;

    //    DataSave3->ulBatchCounter = 1;
    //    DataSave3->ulMainBtchCounter= 1;

  }

}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucResetPOS(void)
{
  uchar *aucdlgBtns[2];
  int rv=-2;

  aucdlgBtns[0]=GetMsgs(S_YS);
  aucdlgBtns[1]=GetMsgs(S_NO);
  rv=DisplayModalDialog(GetMsgs(S_RST), GetMsgs(S_RST_Q),"", 5000,aucdlgBtns,2,1);
  if (rv == 0)
  {

    memset(&(DataSave1->udtConnSettings)  ,0,sizeof(strConnSettings));
    memset(&(DataSave1->udtDeviceConfig)  ,0,sizeof(strDeviceConfig));
    memset(&(DataSave1->udtTransDetail)  ,0,sizeof(strTransDetail));
    memset(&(DataSave1->udtTransStore)  ,0,sizeof(strTransStore));
    memset(&(DataSave1->udtRevesalData)  ,0,sizeof(strRevesalData));
    memset(&(DataSave1->udtBatchAlarms)  ,0,sizeof(strBatchAlarm));
    memset(DataSave2  ,0,sizeof(DATASAVEPAGE2));
    memset(DataSave3  ,0,sizeof(DATASAVEPAGE3));
    DataSave3->ulBatchCounter = 1;
    DataSave3->ulMainBtchCounter= 1;
    DataSave3->ulSysTraceAuditNo = 1;
    memset(DataSave1->APP_Project  ,0,sizeof(DataSave1->APP_Project));
    sprintf(DataSave1->APP_Project,"%s", SU_CURRENT_PRJ);
    memset(DataSave1->APP_Version  ,0,sizeof(DataSave1->APP_Version));
    sprintf(DataSave1->APP_Version,"%s", SU_CURRENT_VER);
    //Rprintf("APP_Project=%s\tAPP_Version=%s\n", DataSave1->APP_Project, DataSave1->APP_Version);

    DataSave1->ulSysTraceAuditNo= 436;
    DataSave1->ulReceiptNo = 1;//P-11 Receipt No
    DataSave1->ulBatchNo = 1;                   
    DataSave1->bIsReversalReq = 0;
    DataSave1->bIsBatchPending = 0;
    DataSave1->udtDeviceConfig.ulFloorLimit = 100;

    DataSave1->udtDeviceConfig.bIsManEnt = FALSE;
    DataSave1->udtDeviceConfig.bIsManPinCap = FALSE;
    DataSave1->udtDeviceConfig.bIsSwipPinCap = TRUE;
    DataSave1->udtDeviceConfig.bIsEnableDbg = FALSE;


                   
    DataSave1->udtConnSettings.ulHostTimeout = 120;
    DataSave1->udtDeviceConfig.udtAppLang = L_ENGLISH;
    strcpy(DataSave1->udtDeviceConfig.aucMerchantPassword, DEFAULT_MRCHNT_PSW);
    strcpy(DataSave1->udtDeviceConfig.aucAdminPassword,DEFAULT_ADMIN_PSW);
    memcpy(DataSave1->udtDeviceConfig.audtVedInstitCode,audtInitVedInstitCode, sizeof(audtInitVedInstitCode));


    //memset(&(DataSave1->udtDeviceConfig),0,sizeof(strDeviceConfig));
    //memset(&(DataSave1->udtConnSettings),0,sizeof(strConnSettings));
    //memset(&(DataSave1->udtTransDetail),0,sizeof(strTransDetail));
    //DataSave1->ulSysTraceAuditNo = 0;
  }

}

/*
 *\Brief
 *\Param
 *\return
 */
void vidSetIconClock(void)
{
  DATE_TIME udtTransDateTime;
  char clk_data[2];
  while (1)
  {
    if (GetRTCDateTime(&udtTransDateTime,0))
    {
      ICON_DISPLAY(21);	//Clock colon on
      clk_data[0]=udtTransDateTime.CLKHOUR; // for hour data
      clk_data[1]=udtTransDateTime.CLKMIN; // for minute data
      ICON_CLOCK_DISPLAY( clk_data );
    }
  }
}

/*
 *\Brief
 *\Param
 *\return
 */
void vidCheckSignalStrength(void)
{
  int iResult;
  while(1)
  {
    //iResult= GPRS_getSignalStrength();
    if (iSgnlStrength>=0)
    {
        GPRS_ShowSignal(iSgnlStrength);
    }
    else
    {
    }
  }//end of while
}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucSetBatchClk(void)
{
  ushort selectedItem=0;
  uchar ucIdx;
  uchar aucTime[5];
  uchar aucTempBuff[20];
  uchar aucTempDateTime[20];
  uchar* apucBatchAlarmMenu[20];
  uchar* apucdialogButtons[2];
  int RV;
  int rv=-2;

  while (1)
  {
    for (ucIdx = 0; ucIdx < DataSave1->udtBatchAlarms.ucAlarmsNo; ucIdx++)
    {
      //memset(DataSave1->udtBatchAlarms.aaucDispAlarmsTime[ucIdx],0,
      //       sizeof(DataSave1->udtBatchAlarms.aaucDispAlarmsTime[ucIdx]));
      //memset(aucTempBuff,0,sizeof(aucTempBuff));

      sprintf(aucTempBuff,"%c%c:%c%c", 
              DataSave1->udtBatchAlarms.aaucAlarmsTime[ucIdx][0],
              DataSave1->udtBatchAlarms.aaucAlarmsTime[ucIdx][1],
              DataSave1->udtBatchAlarms.aaucAlarmsTime[ucIdx][2],
              DataSave1->udtBatchAlarms.aaucAlarmsTime[ucIdx][3]);
      sprintf(DataSave1->udtBatchAlarms.aaucDispAlarmsTime[ucIdx],"%d - %s",(ucIdx+1),pcFrmDgts(DIGIT,aucTempBuff));
      apucBatchAlarmMenu[ucIdx] = DataSave1->udtBatchAlarms.aaucDispAlarmsTime[ucIdx];
      //Rprintf("aaucBatchAlarmMenu[%d]:%s\r\n",ucIdx,aaucBatchAlarmMenu[ucIdx]);
      //Rprintf("aaucDispAlarmsTime[%d]:%s\r\n",ucIdx,DataSave1->udtBatchAlarms.aaucDispAlarmsTime[ucIdx]);
      //Rprintf("apucBatchAlarmMenu[%d]:%s\r\n",ucIdx,apucBatchAlarmMenu[ucIdx]);
    }
    apucBatchAlarmMenu[ucIdx++]=GetMsgs(S_ADD_NEW);
    apucBatchAlarmMenu[ucIdx]=NULL;

    if ((RV = DisplayMenu(apucBatchAlarmMenu, &selectedItem, 10000)) == 0)
    { // if input is ok
      if (selectedItem < DataSave1->udtBatchAlarms.ucAlarmsNo)
      {
        apucdialogButtons[0]=GetMsgs(S_EDIT);
        apucdialogButtons[1]=GetMsgs(S_DEL);
        rv=DisplayModalDialog(GetMsgs(S_SETL_TIME), //Title
                              GetMsgs(EMPTY_MSG),       //Text,
                              "",
                              5000,                         //Timeout
                              apucdialogButtons,                //Choices
                              2,                            //number of choices
                              0);                           //default selection
        if (rv == 0)
        {
          memset(aucTime,0,sizeof(aucTime));
          memcpy(aucTime,DataSave1->udtBatchAlarms.aaucAlarmsTime[selectedItem],4);
          if (DispPrompt(S_SETL_TIME,S_TIME_HHMM_FRMT,4,4, DIGIT_PROMPT, aucTime)== TRUE)
          {
            memset(aucTempDateTime,0,sizeof(aucTempDateTime));
            sprintf(aucTempDateTime,"20131121%s00",aucTime);
            if (CheckDateAndTime(aucTempDateTime) == 0)
            {
              Beep_EventSound(ERROR_TONE);
              DisplayMsgBox(S_SETL_TIME, S_INVLD_TIME,1000, 0);
            }
            else
            {
              memset(DataSave1->udtBatchAlarms.aaucAlarmsTime[selectedItem], 0, 
                     sizeof(DataSave1->udtBatchAlarms.aaucAlarmsTime[selectedItem]));
              sprintf(DataSave1->udtBatchAlarms.aaucAlarmsTime[selectedItem],"%s",aucTime);
              DataSave1->udtBatchAlarms.abIsAlarmChecked[selectedItem] = FALSE;
            }
          }
        } 
        else if (rv == 1)
        {
          memset(DataSave1->udtBatchAlarms.aaucAlarmsTime[selectedItem], 0, 
                 sizeof(DataSave1->udtBatchAlarms.aaucAlarmsTime[selectedItem]));
          DataSave1->udtBatchAlarms.abIsAlarmChecked[selectedItem] = FALSE;

          for (ucIdx = selectedItem; ucIdx < (DataSave1->udtBatchAlarms.ucAlarmsNo-1); ucIdx++)
          {
            memset(DataSave1->udtBatchAlarms.aaucAlarmsTime[ucIdx], 0, 
                   sizeof(DataSave1->udtBatchAlarms.aaucAlarmsTime[ucIdx]));
            DataSave1->udtBatchAlarms.abIsAlarmChecked[ucIdx] = FALSE;

            memcpy(DataSave1->udtBatchAlarms.aaucAlarmsTime[ucIdx], DataSave1->udtBatchAlarms.aaucAlarmsTime[ucIdx+1],
                   strlen(DataSave1->udtBatchAlarms.aaucAlarmsTime[ucIdx+1]));
            DataSave1->udtBatchAlarms.abIsAlarmChecked[ucIdx] = DataSave1->udtBatchAlarms.abIsAlarmChecked[ucIdx+1];
          }
          DataSave1->udtBatchAlarms.ucAlarmsNo--;
        }
      }
      else
      {
        memset(aucTime,0,sizeof(aucTime));
        if (DispPrompt(S_SETL_TIME,S_TIME_HHMM_FRMT,4,4, DIGIT_PROMPT, aucTime)== TRUE)
        {
          memset(aucTempDateTime,0,sizeof(aucTempDateTime));
          sprintf(aucTempDateTime,"20131121%s00",aucTime);
          if (CheckDateAndTime(aucTempDateTime) == 0)
          {
            Beep_EventSound(ERROR_TONE);
            DisplayMsgBox(S_SETL_TIME, S_INVLD_TIME,1000, 0);
          }
          else
          {
            memset(DataSave1->udtBatchAlarms.aaucAlarmsTime[DataSave1->udtBatchAlarms.ucAlarmsNo], 0, 
                   sizeof(DataSave1->udtBatchAlarms.aaucAlarmsTime[DataSave1->udtBatchAlarms.ucAlarmsNo]));
            DataSave1->udtBatchAlarms.abIsAlarmChecked[DataSave1->udtBatchAlarms.ucAlarmsNo] = FALSE;

            sprintf(DataSave1->udtBatchAlarms.aaucAlarmsTime[DataSave1->udtBatchAlarms.ucAlarmsNo],"%s",aucTime);
            DataSave1->udtBatchAlarms.ucAlarmsNo++;
          }
        }
      }
    }
    else if((RV ==1)||(RV ==-1))
    { //canceled or timed out
        return FALSE;
    }// end of if condition
  }
}

/******************************************************************************/
/* EXPORTED FUNCTIONS                                                         */
/******************************************************************************/
/*
 *\Brief
 *\Param
 *\return
 */
void vidOnPowerOnCheck(void)
{
  uchar ucKey;
  char sz6BiosVer[7], sz17HWInfo[20];

  uchar aucPswBuff[40];
  uchar aucSerialBuff[40];
  uchar aucPlainSerialBuff[40];
  uchar aucSerialBuff1[40];
  uchar aucSerialBuff2[40];
  uchar aucProdId[30];
  uchar aucProdIdHash[30];
  uchar ucResult;
  int iResult;

    int i;

  memset(sz6BiosVer,0,sizeof(sz6BiosVer));
  memset(sz17HWInfo,0,sizeof(sz17HWInfo));

//  EnableDebugOutput();
//  SetDebugPort(COM0, BPS115200, NONE8BIT, STOP1BIT);
  GetSystemBiosVersion(sz6BiosVer);
  GetSystemHWInformation(sz17HWInfo);
  Rprintf("sz6BiosVer = %s\r\n", sz6BiosVer);
  Rprintf("sz17HWInfo = %s\r\n", sz17HWInfo);

  //DataSave1 = DATASAVEPAGE1BaseAddress;
  long FlashStAd = FlashMemoryTest(sz17HWInfo[3]);
  long SramStAd = SramTest(sz17HWInfo[1]);
  Rprintf(" FLASH start_addr = %08lX\r\n", FlashStAd);
  Rprintf("SRAM start_addr = %08lX\r\n", SramStAd);
  Rprintf("DATASAVEPAGE0 = %d\r\n", sizeof(DATASAVEPAGE0));
  Rprintf("DATASAVEPAGE1 = %d\r\n", sizeof(DATASAVEPAGE1));
  Rprintf("DATASAVEPAGE2 = %d\r\n", sizeof(DATASAVEPAGE2));
  Rprintf("DATASAVEPAGE3 = %d\r\n", sizeof(DATASAVEPAGE3));
//DataSave1 = ( DATASAVEPAGE1 *)SramTest(sz17HWInfo[1]);
  //DataSave2 = (DATASAVEPAGE2 *)DataSave1+sizeof(DATASAVEPAGE1);
  //DataSave3 = (DATASAVEPAGE3 *)DataSave2+sizeof(DATASAVEPAGE2);
  Rprintf("start_addr1 = %08lX\r\n", DataSave1);
  Rprintf("start_addr2 = %08lX\r\n", DataSave2);
  Rprintf("start_addr3 = %08lX\r\n", DataSave3);


  memset(aucProdId,0,sizeof(aucProdId));
  GET_PROD_ID(aucProdId);
  Rprintf("aucProdId = %s\r\n", aucProdId);
  Rprintf("APP_Project=%s\tAPP_Version=%s\n", SU_CURRENT_PRJ, DataSave1->APP_Version);

  if ( memcmp(SU_CURRENT_PRJ,DataSave1->APP_Project, strlen(SU_CURRENT_PRJ))
      || memcmp(SU_CURRENT_VER,DataSave1->APP_Version,strlen(SU_CURRENT_VER)))
  {
      ClearLCD();
      Lprint(0,0,"Registration Key");
      Lprint(1,0,aucProdId);
      KEYPAD_WaitKey();
      memset(aucSerialBuff,0,sizeof(aucSerialBuff));
      memset(aucProdIdHash,0,sizeof(aucProdIdHash));
/*
      memset(aucProdId,0,sizeof(aucProdId));
      memcpy(aucProdId,"BRS51130702241",14);

      DES_CRC(aucProdId,strlen(aucProdId),aucProdIdHash);
      Tripple_DES_encode(aucSerialBuff,aucProdIdHash,8,DEFAULT_SERL_KEY,0);
      */
     Rprintf("aucProdId = %s\r\n", aucProdId);
     Rprintf("aucProdIdHash");
        for (i=0; i<16; i++)
        {
          Rprintf("%02x ",aucProdIdHash[i]);
        }
      Rprintf("aucSerialBuff");
        for (i=0; i<strlen(aucSerialBuff); i++)
        {
          Rprintf("%02x ",aucSerialBuff[i]);
        }

      memset(aucPswBuff, 0, sizeof(aucPswBuff));
      while(1)
      {

        if (DispPrompt(EMPTY_MSG, S_ENT_SERIAL_NO, MRCHNT_SERIAL_MAX_LEN, MRCHNT_SERIAL_MAX_LEN, HEX_PROMPT, aucPswBuff) == FALSE)
        {
          memset(aucPswBuff,0,sizeof(aucPswBuff));
          continue;
        }
        memset(aucSerialBuff,0,sizeof(aucSerialBuff));
        memset(aucPlainSerialBuff,0,sizeof(aucPlainSerialBuff));
        memset(aucSerialBuff1,0,sizeof(aucSerialBuff1));
        memset(aucSerialBuff2,0,sizeof(aucSerialBuff2));
        memset(aucProdIdHash,0,sizeof(aucProdIdHash));

        asc_to_num(aucSerialBuff,aucPswBuff,strlen(aucPswBuff));
        memcpy (aucSerialBuff1,aucSerialBuff,16);
        memcpy (aucSerialBuff2,&aucSerialBuff[16],16);

        //Tripple_DES_decode(aucPlainSerialBuff,aucSerialBuff,8,DEFAULT_SERL_KEY,0);
        DES_CRC(aucProdId,strlen(aucProdId),aucProdIdHash);
        /*
        Rprintf("aucProdIdHash");
        for (i=0; i<16; i++) {
          Rprintf("%02x ",aucProdIdHash[i]);
        }

        Rprintf("aucSerialBuff2");

        for (i=0; i<16; i++) {
          Rprintf("%02x ",aucSerialBuff2[i]);
        }
        des(aucSerialBuff1, DEFAULT_SERL_KEY, DEC_3DES);
        des(aucSerialBuff2, DEFAULT_SERL_KEY, DEC_3DES);

        memcpy(aucSerialBuff,aucSerialBuff1,8);
        memcpy(&aucSerialBuff[8],aucSerialBuff2,8);

        Rprintf("aucSerialBuff1 = %s\r\n", aucSerialBuff1);
        Rprintf("aucSerialBuff2 = %s\r\n", aucSerialBuff2);
        Rprintf("aucSerialBuff = %s\r\n", aucSerialBuff);
        */
        if (memcmp(aucPlainSerialBuff, aucProdIdHash,strlen(aucProdIdHash)) != 0)
        {
        	break;
          DisplayMsgBox(EMPTY_MSG, S_WRNG_SERL,1000, 0);
        }
        else
        {
          break;
        }
      }


    if(!memcmp(SU_CURRENT_PRJ,DataSave1->APP_Project,strlen(SU_CURRENT_PRJ))
      && (memcmp(SU_CURRENT_VER,DataSave1->APP_Version,strlen(SU_CURRENT_VER)) !=0))
    {
      ClearLCD();
      Lprint(0,0,"Different Version");
      Lprint(1,0,"Clear memory?");
      Lprint(3,0,"[ENTER]to clear ");
      if((KEYPAD_WaitKey() & 0xFF)!= VK_ENTER)
      {
        memset(DataSave1->APP_Version,0,sizeof(DataSave1->APP_Version));
        sprintf(DataSave1->APP_Version,"%s", SU_CURRENT_VER);
        return;
      }
    }

    memset(&(DataSave1->udtConnSettings)  ,0,sizeof(strConnSettings));
    memset(&(DataSave1->udtDeviceConfig)  ,0,sizeof(strDeviceConfig));
    memset(&(DataSave1->udtTransDetail)  ,0,sizeof(strTransDetail));
    memset(&(DataSave1->udtTransStore)  ,0,sizeof(strTransStore));
    memset(&(DataSave1->udtRevesalData)  ,0,sizeof(strRevesalData));
    memset(&(DataSave1->udtBatchAlarms)  ,0,sizeof(strBatchAlarm));
    memset(DataSave2  ,0,sizeof(DATASAVEPAGE2));
    memset(DataSave3  ,0,sizeof(DATASAVEPAGE3));
    DataSave3->ulBatchCounter = 1;
    DataSave3->ulMainBtchCounter= 1;
    DataSave3->ulSysTraceAuditNo = 1;

//    DataSave2->ulBreadNo = 0;
//    DataSave2->ulNoOfTrans = 0;
//    DataSave2->ulInspBreadNo = 0;
//    DataSave2->ulNoOfInspTrans = 0;

    memset(DataSave1->APP_Project,0,sizeof(DataSave1->APP_Project));
    sprintf(DataSave1->APP_Project,"%s", SU_CURRENT_PRJ);
    memset(DataSave1->APP_Version,0,sizeof(DataSave1->APP_Version));
    sprintf(DataSave1->APP_Version,"%s", SU_CURRENT_VER);
    //Rprintf("APP_Project=%s\tAPP_Version=%s\n", DataSave1->APP_Project, DataSave1->APP_Version);

    DataSave1->ulSysTraceAuditNo= 436;
    DataSave1->ulReceiptNo = 1;//P-11 Receipt No
    DataSave1->ulBatchNo = 1;
    DataSave1->bIsReversalReq = 0;
    DataSave1->bIsBatchPending = 0;
    DataSave1->udtDeviceConfig.bIsManEnt = FALSE;
    DataSave1->udtDeviceConfig.bIsManPinCap = FALSE;
    DataSave1->udtDeviceConfig.bIsSwipPinCap = TRUE;
    DataSave1->udtDeviceConfig.bIsEnableDbg = FALSE;
    DataSave1->udtDeviceConfig.ulFloorLimit = 100;


    DataSave1->udtConnSettings.ulHostTimeout = 120;
    DataSave1->udtDeviceConfig.udtAppLang = L_ENGLISH;
    strcpy(DataSave1->udtDeviceConfig.aucProdId, aucProdId);
    strcpy(DataSave1->udtDeviceConfig.aucMerchantPassword, DEFAULT_MRCHNT_PSW);
    strcpy(DataSave1->udtDeviceConfig.aucAdminPassword,DEFAULT_ADMIN_PSW);
    memcpy(DataSave1->udtDeviceConfig.audtVedInstitCode,audtInitVedInstitCode, sizeof(audtInitVedInstitCode));
  }
}

/*
 *\Brief
 *\Param
 *\return
 */
void vidEnDisDbgPort(void)
{
  uchar *aucdlgBtns[2];
  ushort usDefaultChoice;
  int rv=-2;

  aucdlgBtns[0]=GetMsgs(S_YS);
  aucdlgBtns[1]=GetMsgs(S_NO);
  if (DataSave1->udtDeviceConfig.bIsEnableDbg == TRUE)
  {
    usDefaultChoice = 0;
  }
  else
  {
    usDefaultChoice = 1;
  }
  rv = DispModalDialog(S_DBG, S_ACTIV_Q, "", 20000, aucdlgBtns, 2, usDefaultChoice); 
  if (rv == 0)
  {
    DataSave1->udtDeviceConfig.bIsEnableDbg = TRUE;
//    EnableDebugOutput();
//    SetDebugPort(COM0, BPS115200, NONE8BIT, STOP1BIT);

  }
  else if (rv == 1)
  {
    DataSave1->udtDeviceConfig.bIsEnableDbg = FALSE;
//    DisableDebugOutput();
  }

}

/*
 *\Brief
 *\Param
 *\return
 */
uchar ucDeviceConfig(void)
{
  uchar ucselectedItem;
  int RV;

  while (1)
  {
    if((RV = DisplayMsgMenu(S_SETUP,&ucselectedItem,10000,S_DT_TM, S_VERSION))==0)
    {
      switch (ucselectedItem+S_DT_TM)
      {
      case S_DT_TM:
        ucSetDateTime();
        break;
      case S_PS_NO:
        ucSetPosParameters();
        break;
      case S_NETWRK_SETNGS:
        ucNetworkSettings();
        break;
      case S_CHNG_KEYS:
        ucChngKeys();
        break;
      case S_SETL_TIME:
        ucSetBatchClk();
        break;
//      case S_VEND_INSTIT_CODE:
//        ucSetVendInstitCodes();
//        break;
//      case S_SWIP_ENT_PIN:
//        ucSwipeEntryPinCap();
//        break;
//      case S_MAN_ENT:
//        ucManulEntryPinCap();
        break;
      case S_CHNG_PSWS:
        ucChngPswrds();
        break;
//      case S_CLR_RVRSL:
//        ucClearReversal();
//        break;
      case S_EMPTY_BATCH:
        ucEmptyBatch();
        break;
//      case S_DIST_SET:
//    	ucDistSetting();	//Heba
        break;
      case S_BTCH_NO:
    	ucSetBatchNo();
    	break;
        case S_RST:
        ucResetPOS();
        break;
      case S_VERSION:
        vidDiplayVersion();
        break;
      default:
        break;
      }
    }
    else if((RV ==1)||(RV ==-1))
    {
      break;
    }
  }
}

/* We want to compress the data block at 'in' with length 'IN_LEN' to
 * the block at 'out'. Because the input block may be incompressible,
 * we must provide a little more output space in case that compression
 * is not possible.
 */




/*************************************************************************
//
**************************************************************************/

//int compressme(const uchar* in,uint uiInLen,uchar* out,uint* uioutLen)
//{
//    int r;
//    lzo_uint in_len;
//    lzo_uint out_len;
//    lzo_uint new_len;
//    //void* wrkmem = NULL;
//
//    Rprintf("\nLZO real-time data compression library (v%s, %s).\r\n",
//           lzo_version_string(), lzo_version_date());
//    Rprintf("Copyright (C) 1996-2011 Markus Franz Xaver Johannes Oberhumer\nAll Rights Reserved.\r\n");
//
//
///*
// * Step 1: initialize the LZO library
// */
//    if (lzo_init() != LZO_E_OK)
//    {
//    	Rprintf("internal error - lzo_init() failed !!!\r\n");
//    	Rprintf("(this usually indicates a compiler bug - try recompiling\nwithout optimizations, and enable '-DLZO_DEBUG' for diagnostics)\r\n");
//        return 3;
//    }
//
///*
// * Step 2: prepare the input block that will get compressed.
// *         We just fill it with zeros in this example program,
// *         but you would use your real-world data here.
// */
//    in_len = uiInLen;
//    //lzo_memset(in,0,in_len);
//
///*
// * Step 3: compress from 'in' to 'out' with LZO1X-1
// */
//    r = lzo1x_1_compress(in,in_len,out,&out_len,wrkmem);
//    if (r == LZO_E_OK){
//    	Rprintf("compressed %lu bytes into %lu bytes\r\n",
//            (unsigned long) in_len, (unsigned long) out_len);
//    	*uioutLen = out_len;
//    }
//    else
//    {
//        /* this should NEVER happen */
//    	Rprintf("internal error - compression failed: %d\r\n", r);
//        return 2;
//    }
//    /* check for an incompressible block */
//    if (out_len >= in_len)
//    {
//    	Rprintf("This block contains incompressible data.\r\n");
//        return 0;
//    }
//
///*
// * Step 4: decompress again, now going from 'out' to 'in'
// */
//    new_len = in_len;
//    r = lzo1x_decompress(out,out_len,in,&new_len,NULL);
//    if (r == LZO_E_OK && new_len == in_len)
//    	Rprintf("decompressed %lu bytes back into %lu bytes\r\n",
//            (unsigned long) out_len, (unsigned long) in_len);
//    else
//    {
//        /* this should NEVER happen */
//    	Rprintf("internal error - decompression failed: %d\r\n", r);
//        return 1;
//    }
//
//    Rprintf("\nminiLZO simple compression test passed.\r\n");
//    return 0;
//}

//uchar ucCompressMiniLZO(const uchar* in,uint uiInLen,uchar* out,uint* uioutLen)
//{
//    int r;
//    lzo_uint in_len;
//    lzo_uint out_len;
//    lzo_uint new_len;
//    //void* wrkmem = NULL;
//
//    Rprintf("\nLZO real-time data compression library (v%s, %s).\r\n",
//           lzo_version_string(), lzo_version_date());
//    Rprintf("Copyright (C) 1996-2011 Markus Franz Xaver Johannes Oberhumer\nAll Rights Reserved.\r\n");
//
//
///*
// * Step 1: initialize the LZO library
//*/
//    if (lzo_init() != LZO_E_OK)
//    {
//    	Rprintf("internal error - lzo_init() failed !!!\r\n");
//    	Rprintf("(this usually indicates a compiler bug - try recompiling\nwithout optimizations, and enable '-DLZO_DEBUG' for diagnostics)\r\n");
//        return 3;
//    }
//
//
//    in_len = uiInLen;
//
///*
// * Step 3: compress from 'in' to 'out' with LZO1X-1
// */
//    r = lzo1x_1_compress(in,in_len,out,&out_len,wrkmem);
//    if (r == LZO_E_OK){
//    	Rprintf("compressed %lu bytes into %lu bytes\r\n",
//            (unsigned long) in_len, (unsigned long) out_len);
//    	*uioutLen = out_len;
//    }
//    else
//    {
//        /* this should NEVER happen */
//    	Rprintf("internal error - compression failed: %d\r\n", r);
//        return 2;
//    }
//    /* check for an incompressible block */
//    if (out_len >= in_len)
//    {
//    	Rprintf("This block contains incompressible data.\r\n");
//        return 1;
//    }
//    return SUCCESS;
//}

/*
 *\Brief
 *\Param
 *\return
 */
void Animation( void )
{
	const unsigned char DOTY[] = {0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF,};
	char GraphicBuffer[1024],GraphicBuffer1[1024];
	int i, j, k;
	int xScr, tScr, yScr;

	int iScrWidth, iScrHeight;
	 char *pScrImg;

	int ScrProcess=1;
	while(ScrProcess<3)
	{

	switch(ScrProcess) {
		case 1:
			pScrImg = (char *)GraphicTxt;
			iScrWidth = 128;
			iScrHeight = 48 / 6;
			memset(GraphicBuffer, '\0', sizeof(GraphicBuffer));
			xScr = 0;
			TIME_BASE1 = 0x0400 | 5;
			ScrProcess++;
			break;
		case 2:
			if (TIME_BASE1 & 0x0400)
				break;
			if (xScr < iScrWidth)
			{
				for ( i = 0 ; i < iScrHeight ; i++ )
					GraphicBuffer[i*iScrWidth+xScr] = pScrImg[i*iScrWidth+xScr];
				DisplayLogo(GraphicBuffer, 0, 4);
				TIME_BASE1 = 0x0400 | 5;
				xScr++;
			}
			else
			{
				xScr = 0;
				ScrProcess++;
			}
			break;
		case 3:
			if (TIME_BASE1 & 0x0400)
				break;
			if (xScr < iScrWidth)
			{
				for ( i = 0 ; i < iScrHeight ; i++ )
/*					GraphicBuffer[i*iScrWidth+xScr] = 0x00;*/
					GraphicBuffer[i*iScrWidth+xScr] = pScrImg[i*iScrWidth+xScr];
				DisplayLogo(GraphicBuffer, 0, 4);
				TIME_BASE1 = 0x0400 | 5;
				xScr++;
			}
			else
			{
				xScr = 0;
				ScrProcess++;
			}
			break;
	 }//end of swtich case
	}//end of while
}
