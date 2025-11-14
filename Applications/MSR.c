 /*! 
* \file    $Source: MSR.c $
* \date    $Date: 2012/06/22  $
* \version $Revision: 1.1 $
* \author  $Author: Mohamed $
*/
/*!
 * *  Implementation of Magnetic stripe Function
*  
* (c) Copyright Bitel Emea 2013 unpublished work.
* This computer program includes confidential, proprietary 
* information and is a trade secret of Bitel. All use, 
* disclosure and/or reproduction is prohibited unless
* authorized in writing.
* All rights reserved.
*/
/*@{*/
#include "../../BF_SDK/include/BTEMEA_LIB/MSR.h"
#include "../../BF_SDK/include/lib/BEEPLIB1.h"
#include "emv/EMV_Apdu.h"
#include "AppGlobal.h"


char sz6BiosVer[6], sz17HWInfo[17];

int MSR_WaitForCard();
int MSR_ReadTrack1();
int MSR_ReadTrack2();
int MSR_ReadTrack3();



int msr_track;

short sMsrErrFlag, sMsrTrack1ErrFlag, sMsrTrack2ErrFlag, sMsrTrack3ErrFlag, sMsrDispChar;
char sz77Track1Data[77], sz27CardholderName[27];
char sz38Track2Data[38], sz20PAN[20], sz5ExpiryDate[5], sz4CardServiceCode[4];
char sz105Track3Data[105];

void ClearAllMsrDataBuf(void)
{
	memset(sz77Track1Data, 0x00, sizeof(sz77Track1Data));
	memset(sz27CardholderName, 0x00, sizeof(sz27CardholderName));
	
	memset(sz38Track2Data, 0x00, sizeof(sz38Track2Data));
	memset(sz20PAN, 0x00, sizeof(sz20PAN));
	memset(sz5ExpiryDate, 0x00, sizeof(sz5ExpiryDate));
	memset(sz4CardServiceCode, 0x00, sizeof(sz4CardServiceCode));
	
	memset(sz105Track3Data, 0x00, sizeof(sz105Track3Data));
}
void MsrEnable(void)
{
	// MSR TYPE
	// 0x33:Uniform Triple, 0x34:MagTek Triple
	// MSR TYPE		//heba
	// 0x33:Uniform Triple, 0x34:MagTek Triple
	if (sz17HWInfo[6] == 0x30) {
		CARD_ENABLE();
	}
	else 
	{
		MAGTEK_MSR_NewMode_Card_Enable();
		//Wait1MS(200);
	}
}

void MsrDisable(void)
{
// MSR TYPE		//heba
	// 0x33:Uniform Triple, 0x34:MagTek Triple
	if (sz17HWInfo[6] == 0x30)
	{
		CARD_DISABLE();
	}
	else 
	{
		MAGTEK_MSR_NewMode_Card_Disable();
		//Wait1MS(200);
	}
}
int MsrRead(void)
{
	int ret=0;
	
	// MSR TYPE		//heba
	// 0x33:Uniform Triple, 0x34:MagTek Triple
	if (sz17HWInfo[6] == 0x30) 
	{
		ret = CARD_READ() & 0xFF;
	}
	else 
	{
		ret = MAGTEK_MSR_NewMode_Card_Read();
		//Wait1MS(200);
	}
	return ret;
}

int MSR_WaitForCard()
{
    GetSystemBiosVersion(sz6BiosVer);
    GetSystemHWInformation(sz17HWInfo);
    
    unsigned char uckey=0xFF;
    unsigned char ucCont;
    int iResult;
	int i;
	int CState;
    msr_track = 0;

    unsigned char aucTempBuff[200];
	ClearAllMsrDataBuf();
    MSR_ClearAllCardDataBuffer();
    ClearMSRCardData();
    MsrEnable();
    
    ucCont=1;
    
    while (ucCont==1)
    {
      uckey = KEYSCAN() & 0xFF;
      if (uckey ==VK_CANCEL  )
      {
          //Rprintf("sssssssssssssssssssssssssss%d",uckey);
          MsrDisable();
          return -1;
      } 
      else if (((uckey >=VK_NO0)&&((uckey <=VK_NO9)) )&&
               (DataSave1->udtDeviceConfig.bIsManEnt == TRUE))
      {
        MsrDisable();
        return -2;
      }
//     Rprintf("gsc_SetCardTYPEandSLOT(0x02, 0)=%d\r\n",gsc_SetCardTYPEandSLOT(0x02, 0));
//     Rprintf("pgsSC_STAT->CARD=%d\r\n",pgsSC_STAT->CARD);
      else if((CState=gsc_SetCardTYPEandSLOT(0x02, 0)) != -1)
      {
     	if(!pgsSC_STAT->CARD)		//0 means not inserted
     	{
     	  DBGprt(("Not Inserted.\r\n"));
     	  //	  Lprintf(0, 0, (char *)"Insert Card"); //"  <EMV-L2 TTA>  "
     	  gsc_Status();	// sispp 070305
     	}
     	else			//1 means card inserted
     	{
     		   	  MsrDisable();
     		   	  return -3;	//Support Chip
     	}
      }
      else if(CState ==-1)
      {
       	if(!pgsSC_STAT->CARD)		//0 means not inserted
       	{
       	  DBGprt(("Not Inserted.\r\n"));
       	  //	  Lprintf(0, 0, (char *)"Insert Card"); //"  <EMV-L2 TTA>  "
       	  gsc_Status();	// sispp 070305
       	}
       	else			//1 means card inserted
       	{
       		//Fallback
        	DataSave1->udtTransDetail.bIsFallback = TRUE;

       		 MsrDisable();
       		 return -5;	//Swipe
       	}
      }
//      else if((ICCARD_IsCardPresent() == ICCARD_EXISTED))
//      {
//    	  MsrDisable();
//    	  return -3;	//Support Chip
//      }
      msr_track = MsrRead(); 
      //msr_track=3;
      if (msr_track != 0) 
      {
        MsrDisable();
        sMsrTrack1ErrFlag = -999;
        sMsrTrack2ErrFlag = -999;
        sMsrTrack3ErrFlag = -999;
		memset(&(DataSave1->udtTransDetail.udtMSRCard), 0, sizeof(strMSRCardData));
        switch (msr_track) {
          case 1:     // Track 1(or 3) Only
          case 2:     // Track 2 Only
          case 3:     // Track 1(or 3) & 2
          case 8:     // Track 3 Only
          case 9:     // Track 1 & 3
          case 10:    // Track 2 & 3
            case 11:    // Track 1 & 2 & 3
              if (msr_track & 0x01) {
                  // Track 1 Data Capture
                  // track 1 data: max 76, cardholder name: max 26
				
				//memcpy(DataSave1->udtTransDetail.udtMSRCard.acTrack2,&CardNo2[0],(CardNo2[0]+1));
				//memcpy(DataSave1->udtTransDetail.udtMSRCard.acTrack1,&CardNo1[0],(CardNo1[0]+1));
				//memcpy(DataSave1->udtTransDetail.udtMSRCard.CardHolderName,&CardHolderName[1],CardHolderName[0]);
				//
				//if (MSR_GetCardPAN(&CardNo2[1],DataSave1->udtTransDetail.udtMSRCard.PAN)!=0){
				//  memset(DataSave1->udtTransDetail.udtMSRCard.PAN, 0,
				//		 sizeof(DataSave1->udtTransDetail.udtMSRCard.PAN));
				//}
				//if (MSR_GetCardExpiryDate(&CardNo2[1],DataSave1->udtTransDetail.udtMSRCard.ExpiryDate)!=0){
				//  memset(DataSave1->udtTransDetail.udtMSRCard.ExpiryDate, 0,
				//		 sizeof(DataSave1->udtTransDetail.udtMSRCard.ExpiryDate));
				//}
				//if (MSR_GetCardServiceCode(&CardNo2[1],DataSave1->udtTransDetail.udtMSRCard.SrvcCode)!=0){
				//  memset(DataSave1->udtTransDetail.udtMSRCard.SrvcCode, 0,
				//		 sizeof(DataSave1->udtTransDetail.udtMSRCard.SrvcCode));
				//}

                memset(aucTempBuff,0,sizeof(aucTempBuff));
                  sMsrTrack1ErrFlag = MSR_GetTrack1Data((unsigned char *)&CARDRDB[0], aucTempBuff);
                  if (!sMsrTrack1ErrFlag)
                  {
                    memcpy(DataSave1->udtTransDetail.udtMSRCard.acTrack1,aucTempBuff,min(strlen(aucTempBuff),sizeof(DataSave1->udtTransDetail.udtMSRCard.acTrack1)));
                    memset(aucTempBuff,0,sizeof(aucTempBuff));
                    if (MSR_GetCardholderName(DataSave1->udtTransDetail.udtMSRCard.acTrack1, aucTempBuff) == 0)
                    {
                      memcpy(DataSave1->udtTransDetail.CardHolderName,aucTempBuff,min(strlen(aucTempBuff),sizeof(DataSave1->udtTransDetail.CardHolderName)));
                    }
                  }
#ifdef USER_DEBUG
				  Rprintf("acTrack1: %s\r\n",DataSave1->udtTransDetail.udtMSRCard.acTrack1);
				  Rprintf("CardHolderName: %s\r\n",DataSave1->udtTransDetail.udtMSRCard.CardHolderName);
#endif
              }
              if (!sMsrTrack1ErrFlag)
              {
                  // Display PAN of Track 1
                  //LCD_DisplayLine(1, LEFT_JUSTIFIED, sz77Track1Data);
              }
              else {
                  // "Track 1 Error!  ",
                  //LCD_DisplayLine(1, LEFT_JUSTIFIED, (char *)msg_tbl1[M1Track1Error_000001]);
#ifdef USER_DEBUG
                 Rprintf("Track 1 Error = %d\r\n", sMsrTrack1ErrFlag);
#endif
             }

            if (msr_track & 0x02)
            {
                // Track 2 Data Capture
                // track 2 data: max 37
              memset(aucTempBuff,0,sizeof(aucTempBuff));
                sMsrTrack2ErrFlag = MSR_GetTrack2Data((unsigned char *)&CARDRDB2[0], aucTempBuff);

                if (!sMsrTrack2ErrFlag)
                {
                  memcpy(DataSave1->udtTransDetail.udtMSRCard.acTrack2,aucTempBuff,
                         min(strlen(aucTempBuff),sizeof(DataSave1->udtTransDetail.udtMSRCard.acTrack2)));
                  memset(aucTempBuff,0,sizeof(aucTempBuff));
                    if (MSR_GetCardPAN(DataSave1->udtTransDetail.udtMSRCard.acTrack2, aucTempBuff)== 0)
                    {
                      memcpy(DataSave1->udtTransDetail.PAN,aucTempBuff,
                             min(strlen(aucTempBuff),sizeof(DataSave1->udtTransDetail.PAN)));
                    }
                    memset(aucTempBuff,0,sizeof(aucTempBuff));
                    if (MSR_GetCardExpiryDate(DataSave1->udtTransDetail.udtMSRCard.acTrack2, aucTempBuff)== 0)
                    {
                      memcpy(DataSave1->udtTransDetail.ExpiryDate,aucTempBuff,
                             min(strlen(aucTempBuff),sizeof(DataSave1->udtTransDetail.ExpiryDate)));
                    }
                    memset(aucTempBuff,0,sizeof(aucTempBuff));
                    if (MSR_GetCardServiceCode(DataSave1->udtTransDetail.udtMSRCard.acTrack2, aucTempBuff)== 0)
                    {
                      memcpy(DataSave1->udtTransDetail.SrvCode,aucTempBuff,
                             min(strlen(aucTempBuff),sizeof(DataSave1->udtTransDetail.SrvCode)));
                    }
                }
                Rprintf("SrvcCode: %s\r\n",DataSave1->udtTransDetail.SrvCode);
#ifdef USER_DEBUG
				Rprintf("acTrack2: %s\r\n",DataSave1->udtTransDetail.udtMSRCard.acTrack2);
				Rprintf("PAN: %s\r\n",DataSave1->udtTransDetail.udtMSRCard.PAN);
				Rprintf("ExpiryDate: %s\r\n",DataSave1->udtTransDetail.udtMSRCard.ExpiryDate);
				Rprintf("SrvcCode: %s\r\n",DataSave1->udtTransDetail.udtMSRCard.SrvcCode);
                if (strlen(sz20PAN))
                {
                    i = MSR_CheckLuhnCheckSum(sz20PAN, strlen(sz20PAN));
                    Rprintf("MSR_CheckLuhnCheckSum = %d\r\n", i);
                }
#endif
                //heba
                if(DataSave1->udtTransDetail.SrvCode[0]=='2' &&
                  (DataSave1->udtTransDetail.bIsFallback == FALSE))
                {
                	DataSave1->udtTransDetail.bIsFallback = TRUE;
                	return -4;	//Support Chip

                }

            }
            if (!sMsrTrack2ErrFlag) {
                    // Display PAN
                    //LCD_DisplayLine(2, LEFT_JUSTIFIED, sz38Track2Data);
            }
            else {
                // "Track 2 Error!  ",
                //LCD_DisplayLine(2, LEFT_JUSTIFIED, (char *)msg_tbl1[M1Track2Error_000001]);
#ifdef USER_DEBUG
                 Rprintf("Track 2 Error = %d\r\n", sMsrTrack2ErrFlag);
#endif
            }

            if (sMsrTrack1ErrFlag == 0 && sMsrTrack2ErrFlag == 0) {
			  ucCont = 0;
              Beep_EventSound(5);
            }
            else {
              Beep_EventSound(ERROR_TONE);
            }

            break;
		}
        //iResult=MSR_ReadTrack1();
        //if (iResult==0){
        //  iResult=MSR_ReadTrack2();
        //  if (iResult==0){
        //    Beep_EventSound(5);
        //    ucCont=0;
        //  }
        //}
          if (ucCont != 0) {
			msr_track = 0;
			ClearAllMsrDataBuf();
			MSR_ClearAllCardDataBuffer();
			ClearMSRCardData();
			MsrEnable();
          }
          //ClearLCD();
          //Lprint(1, 0, "Track 2 Error!  ");
          //ClearLCD();
          //Lprint(1, 0, "Track 1 Error!  ");
          //Read track 2
			//Rprintf("sssssssssssssssssssssssssss%d",msr_track);
			//ucCont=0;
			//break;

		  //TIMER_Wait1MS(500);
		}
			//ucCont=0;
	}

	MsrDisable();

	return 0;

}
//////////////////////////////////////////////////////////////////

int MSR_ReadTrack1()
{
    short sRet;
    sRet=ReadMSRTrack1(msr_track);
    return sRet;
}
//////////////////////////////////////////////////////////////////

int MSR_ReadTrack2()
{
    short sRet;
    sRet=ReadMSRTrack2(msr_track);
    return sRet;
}
//////////////////////////////////////////////////////////////////

int MSR_ReadTrack3()
{
    short sRet;
    sRet=ReadMSRTrack3(msr_track);
    return sRet;
}

