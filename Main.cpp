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
//C Files
extern "C"
{
#include "./Applications/AppGlobal.h"
#include "./Applications/fonts/LCDFONTC.H"
#include "./Applications/resources/Msgs.h"
#include "./Applications/resources/logo.h"
#include "./Applications/Display.h"
#include "./Applications/Utilities.h"
#include "./Applications/Trans.h"
#include "./Applications/TransSndRcv.h"
#include "./Applications/Receipt.h"
#include "./Applications/DeviceConfig.h"
#include "./Applications/Display.h"
#include "Applications/emv/EMV.h"
#include "Applications/emv/EMV_APP.h"
#include "../BF_SDK/INCLUDE/system.h"

  extern void setDEBUG_COMport(int comport);

 extern int MsrRead(void);
}
#include "internal/CParam.h"
#include "internal/CGlobalConfig.h"
#include "internal/tools.h"
#include "fs/CFile.h"
#include "fs/fs_flex.h"
#include "tasks/CTaskReadKey.h"
#include "tasks/CTaskDisplay.h"
#include "tasks/CTaskPrinter.h"
#include "iso/CMegaContainer.h"
#include "gprs/CTaskConnect.h"
#include "gprs/CGPRSConnection.h"
#include "internal/CArabicEncoder.h"
#include "internal/CArabicString.h"


//C++ Files

//#include "./Applications/Bread.h"
#ifndef DBG_REPORT
#define DBG_REPORT
#endif

#ifndef DRV_REPORT
#define DRV_REPORT
#endif



/******************************************************************************/
/* TYPES                                                                      */
/******************************************************************************/
/******************************************************************************/
/* CONSTANTS, MACROS                                                          */
/******************************************************************************/
#define CONF_CHANGE


unsigned char	g_codepage[0xFF + 1];
char			g_strProgramName[DISPLAY_WIDTH + 1];
char			g_strProgramVersion[DISPLAY_WIDTH + 1];
char*			g_pProgramName;
char*			g_pProgramVersion;
CParam*			g_pParam;
CTaskPrinter*	g_pPrinter;
char *			g_pMonth[2][12];

void	critErr(const char* msg)
{
	ClearLCDScreen();
	Lprint(1, 0, (char*)msg);
	string	err;
	err.format("Error: \"%s\"", msg);
	DBGLine(err.buffer());
	while (true)
		;
}

const unsigned char multi_seq[32] = 
{

	  0x00,0x01,0x02,0x03,0x00,0x01,0x02,0x03,
	  0x00,0x01,0x02,0x03,0x00,0x01,0x02,0x03,
	  0x00,0x01,0x02,0x03,0x00,0x01,0x02,0x03,
	  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,

};

/******************************************************************************/
/* PRIVATE VARIABLES                                                          */
/******************************************************************************/

  char sz6BiosVer[6], sz17HWInfo[17];

/******************************************************************************/
/* PRIVATE FUNCTIONS PROTOTYPES                                               */
/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/
void InitApplication()
{
  uchar ucIdx;
  int i,j;
  unsigned char ucKey;
  char temp[17];
	uchar ucPRTDensity;

  GetSystemBiosVersion(sz6BiosVer);
  GetSystemHWInformation(sz17HWInfo);
  GET_PROD_ID(udtRunData.acProdId);

  temp[0] = sz17HWInfo[9];
	temp[1] = sz17HWInfo[10];
	temp[2] = 0x00;
	switch (atoi(temp)) {
		case 31:
			//iModelName = IC3100P;
			udtRunData.iModelName = IC3100PE;
			break;
		case 33:
			udtRunData.iModelName = IC3300P;
			break;
		case 51:	// FLEX5100
			udtRunData.iModelName = IC5100P;
			break;
		case 52:	// IC5100 (JAPAN)
			udtRunData.iModelName = IC5100P;
			break;
		case 60:	// PINPAD
			udtRunData.iModelName = IC6000P;
			break;
		default:
			udtRunData.iModelName = UNKNOWN_MODEL;
			break;
	}
  SetLCDContrast(26);
  ActivateLCD();
  ClearLCD();
  SetBackLight( ON );
  BeepEnable();

  	// MODEM TYPE
	// 0x30:MODEM NONE, 0x31:2400bps, 0x32:56Kbps(default), 0x33:GSM, 0x34:CDMA, 0x35:56K+GSM, 0x36:FOMA
	if (sz17HWInfo[7] == 0x36)
	{
		udtRunData.ucMsrTrack = 3;
	}
	// MSR TYPE
	// 0x33:Uniform Triple, 0x34:MagTek Triple
	else if (sz17HWInfo[6] == 0x33 || sz17HWInfo[6] == 0x34)
	{
		udtRunData.ucMsrTrack = 2;
	}
	else
	{
		udtRunData.ucMsrTrack = 0;
	}
	TRACK_SETUP(udtRunData.ucMsrTrack);	// 0:1&2, 1:2&3, 2:1&2&3, 3:1&2&3 (JIS-II)

  //to display traces on hyperterminal


  #ifdef USER_DEBUG
  //EnableDebugOutput();
  //SetDebugPort(COM0, BPS115200, NONE8BIT, STOP1BIT);
  #endif

/*************************Initialization*****************************/

     //call once in the project initialization to initialize printer
  PRINT_PrintInit();
  	if (udtRunData.iModelName == IC3100P)
  	{
		PRINT_assignPaperFeedKey(BTKEY_UPARROW_IC3100P);
		ucPRTDensity = 12;
	}
	else if (udtRunData.iModelName == IC3100PE)
	{
		PRINT_assignPaperFeedKey(BTKEY_UPARROW_IC3100PE);
		ucPRTDensity = 12;
	}
	else if (udtRunData.iModelName == IC3300P)
	{
		PRINT_assignPaperFeedKey(BTKEY_F4_IC3300P);
		ucPRTDensity = 10;
	}
	else if (udtRunData.iModelName == IC5100P)
	{
		PRINT_assignPaperFeedKey(BTKEY_UPARROW_IC5100P);
		// PRINTER TYPE
		// 0x31:BIXOLON(default), 0x32:Seiko Epson
		if (sz17HWInfo[8] == 0x31)
		{
			ucPRTDensity = 10;
		}
		else
		{
			ucPRTDensity = 16;
			//ucPaperCoverDectionFlag = 1;
		}
	}
	else
	{
		PRINT_assignPaperFeedKey(BTKEY_NULL);
		ucPRTDensity = 10;
	}
	SetPrinterDensity(ucPRTDensity);
  //PRINT_assignPaperFeedKey(BTKEY_UPARROW_IC5100P);

    /*
     * this function is very important to display arabic font into screen
     * */
  DISPLY_InitFontData();

  // ICC initialization function
//  ICCARD_inintICDevices();

  Beep_KeySoundEnable();
  Beep_EventSoundEnable();

  //TASK sequence
  for (ucIdx = 1 ; ucIdx < 16 ; ucIdx++ )
  {
      TASK_SEQ[ucIdx]= multi_seq[ucIdx];
  }

  //Global Multitasking on and off control
  // To allowed LCD or other I/O access by multi-application
  SET_MULTITASKING_USAGE(1);
  /*
   *  Accept 0 to 2 number for mode setting
   *  0 � Default,Each task have same priority and perform task sequentially.
   *  1 � Give higher priority(50%) to main routine.
   *  2 - Used TASK sequence assignment table
   */
  SET_TASK_PRIORITY_MODE(2);
  //Accept 5 to 200ms task change time input,10 ms time (Default)
  SET_TASK_CHANGE_TIME(200);

  SET_TASK_STACK_SIZE(0);
  //Inputs: Task number and function name
  //This Task function cannot return to C main.
  TASK_START(1, (void (*)()) vidBatchTimeChkr);
  TASK_START(2, (void (*)()) vidSetIconClock);
  TASK_START(3, (void (*)()) vidCheckSignalStrength);



  // NOTE: This function must be called in initialization
  //Arabic or English.

  //if you want to add new language it must be added in this function
  //New mechanism to display messages
  //SetCurrentLanguage(L_ARABIC);

  USE_LCDFONT( (char *)&OtherLcdFont[0], ON );


  SET_BUFFER_SIZE_8K();
  COM0_POINT_INIT();
  COM1_POINT_INIT();
  setDEBUG_COMport(COM_PORT0);

}


/******************************************************************************/
/* EXPORTED FUNCTIONS                                                         */
/******************************************************************************/

int main(void)
{
  //declaration phase
  int RV;
  int key;
  int msr_track;	//heba
  unsigned short selectedItem=0;

  int iResult;
  char cProcessCard = 0;
  uchar aucPswBuff[10];

  char actemp[20];
  char actemp1[20];

  uchar *aucdlgBtns[2];
  int rv=-2;

  DATE_TIME udtTransDateTime;
  char ucTransDateTime[16];

	gem_BaudRate = 0;
	GemSlotOpen = 0;
	gem_RS232open = 0;

  /*********************Initialization**************************/

  InitApplication();

  InitPinpadDrv(3);
	setDEBUG_COMport(COM_PORT0);

	Rprintf("\r\nM>2.5.Enter Amount.\r\n");

  vidOnPowerOnCheck();

//  Rprintf("sz6BiosVer 2= %s\r\n", sz6BiosVer);

  if (DataSave1->udtDeviceConfig.bIsEnableDbg == TRUE)
  {
    //to display traces on hyperterminal
//    EnableDebugOutput();
//    SetDebugPort(COM0, BPS115200, NONE8BIT, STOP1BIT);
	setDEBUG_COMport(COM_PORT0);
  }
  else
  {
//    DisableDebugOutput();
  }

  InitPinpadDrv(3);
  setDEBUG_COMport(COM_PORT0);

  udtAppLang = DataSave1->udtDeviceConfig.udtAppLang;
  tMapFieldFormats	mapFieldFormats;
  Rprintf("1");
  g_pMapFieldFormats	= &mapFieldFormats;
  Rprintf("2");
  CMegaContainer::initialize();
  Rprintf("3");

  vidDiplayVersion();

  DisplayLogo(GraphicTxt, 0, 4);
  WaitKeyPress10MS(500);

//  PortConfig(0);
//  GPRSInitRegConnect(S_PLZ_WAIT);
//  iSgnlStrength= GPRS_getSignalStrength();

  //CBread oBread;		//heba
//  while (DataSave1->bIsBatchPending == TRUE){
//    ucBatch(FALSE);
//    if (DataSave1->bIsBatchPending == TRUE) {
//      aucdlgBtns[0]=(uchar*)GetMsgs(S_YS);
//      aucdlgBtns[1]=(uchar*)GetMsgs(S_NO);
//      rv = DispModalDialog(S_BTCH_ERR, S_RETRY_Q, "", 50000, aucdlgBtns, 2, 1);
//      if (rv == 0) {
//      } else if (rv == 1) {
//        break;
//      }
//    }
//  }
  InitEmvLib();
//  app_pgsTrmRsaKeyMake(NULL, 0);
  InitPinpadDrv(3);
	setDEBUG_COMport(COM_PORT0);

	ClearLCD();	//heba
	  while(1)
	  {

		  if (GetRTCDateTime(&udtTransDateTime,0))
		  {
		    memset(ucTransDateTime,0,sizeof(ucTransDateTime));

		    sprintf(ucTransDateTime,"%02X-%02X-%02X %02X:%02X",
		            udtTransDateTime.CLKDATE,
		            udtTransDateTime.CLKMONTH,
		            udtTransDateTime.CLKYEARL,
		            udtTransDateTime.CLKHOUR,
		            udtTransDateTime.CLKMIN);
		  }

	    //Display logo On LCD
		  //Check if Batch Alarm is fired
		  //Start Heba
	//    if (!DataSave1->udtBatchAlarms.bIsAlarm)
	//    {
	      //DisplayLogo(GraphicTxt, 0, 4);

		  //
	        HighlightOn(0,0,16);
		  //TextOutA(0,0, "                     ",FA_INVERT,-1,1);
		    Lprint(0,0,"VodaFone [A]          ");

	        Lprint(1,0,ucTransDateTime);
	        Lprint(2,0,"Ready -  Online");

	//    }
	//    else
	//    {	//Send Batch Automatically
	//      DisplayMsgBox(S_BATCH_HDR,S_AUTO_BATCH,0,0);
	//      ucBatch(FALSE);
	//      DataSave1->udtBatchAlarms.bIsAlarm = 0;
	//    }
		  //End heba


	        //Start heba
        	//Start heba
//          if ((DataSave1->bIsBatchPending == TRUE) &&
//              ((selectedItem + S_PURCH) != S_SERV) &&
//              ((selectedItem + S_PURCH) != S_BTCH))
//          {
//            ucDispMultiLineMsg(GetMsgs(S_PEND_BATCH),0,2000);
//            continue;
//          }
//
//          //If memory is Full
//          if ((DataSave1->udtTransStore.ulSavedTransNo >= MAX_TRNS_STORE_SIZE) &&
//              ((selectedItem + S_PURCH) != S_SERV) &&
//              ((selectedItem + S_PURCH) != S_BLNC_INQ) &&
//             /* ((selectedItem + S_PURCH) != S_PN_CHNG) && */
//              ((selectedItem + S_PURCH) != S_MIN_STATE) &&
//              ((selectedItem + S_PURCH) != S_BTCH))
//          {
//            ucDispMultiLineMsg(GetMsgs(S_MEM_FULL),0,2000);
//            continue;
//          }
        	//End heba
          //Reset transaction details
          memset((void*)&(DataSave1->udtTransDetail),0,sizeof(strTransDetail));
          memset((void*)&(udtRunData),0,sizeof(udtRunData));

          //Set transaction type
          DataSave1->udtTransDetail.udtTansType = (enuTransType)selectedItem;
          //set processing code value
          DataSave1->udtTransDetail.ulPCode = aulPCode[selectedItem];	//TODO ask why you have set it twice

          //Set fallback to false Heba
          DataSave1->udtTransDetail.bIsFallback= FALSE;

          //Header Index of displayed Message in Header
          udtRunData.usHdrIdx = S_PURCHASE_HDR;
	      key = ucPurchase(FALSE);		//Normal Purchase


	   // key = KEYSCAN() &0xff;
	        //End heba
	    if (key != 0xff) //if (key == BTKEY_FCN_IC5100P)  //display menu
	    {
	    	//Disable Alarm temp
	      DataSave1->udtBatchAlarms.bIsAlarm = 0;
	      DataSave1->udtDeviceConfig.bIsManEnt = TRUE; //Added by Heba
	      //KEY_InputIPAddress(2,0,actemp);
	      while(1)
	      {
	        //udtRunData.iSgnlStrength= GPRS_getSignalStrength();
	    	  //Dispaly Main Menu
	        if((RV=DisplayMsgMenu(S_MAIN_MENU_HDR,&selectedItem,10000,S_PURCH,S_SERV))==0)
	        {

	        	// if input is ok
	        	//Start heba
	//          if ((DataSave1->bIsBatchPending == TRUE) &&
	//              ((selectedItem + S_PURCH) != S_SERV) &&
	//              ((selectedItem + S_PURCH) != S_BTCH))
	//          {
	//            ucDispMultiLineMsg(GetMsgs(S_PEND_BATCH),0,2000);
	//            continue;
	//          }
	//
	//          //If memory is Full
	//          if ((DataSave1->udtTransStore.ulSavedTransNo >= MAX_TRNS_STORE_SIZE) &&
	//              ((selectedItem + S_PURCH) != S_SERV) &&
	//              ((selectedItem + S_PURCH) != S_BLNC_INQ) &&
	//             /* ((selectedItem + S_PURCH) != S_PN_CHNG) && */
	//              ((selectedItem + S_PURCH) != S_MIN_STATE) &&
	//              ((selectedItem + S_PURCH) != S_BTCH))
	//          {
	//            ucDispMultiLineMsg(GetMsgs(S_MEM_FULL),0,2000);
	//            continue;
	//          }
	        	//End heba
	          //Reset transaction details
	          memset((void*)&(DataSave1->udtTransDetail),0,sizeof(strTransDetail));
	          memset((void*)&(udtRunData),0,sizeof(udtRunData));

	          //Set transaction type
	          DataSave1->udtTransDetail.udtTansType = (enuTransType)selectedItem;
	          //set processing code value
	          DataSave1->udtTransDetail.ulPCode = aulPCode[selectedItem];	//TODO ask why you have set it twice

	          //Set fallback to false Heba
	          DataSave1->udtTransDetail.bIsFallback= FALSE;

	          //Header Index of displayed Message in Header
	          udtRunData.usHdrIdx = selectedItem + S_PURCHASE_HDR;

	          //Start Operations
	          switch (selectedItem + S_PURCH)
	          {
				  case S_PURCH:
					ucPurchase(TRUE);		//Normal Purchase
					 // ucStartTrans(TRUE,PCODE_BILL_PURCHASE);
					break;
		//          case S_BL_PAY:
		//            ucBillPayment();	//Pay bills
		//            break;
		//          case S_TP_PAY:
		//            ucTopUpPayment();	//Recharge Mobile credit
		//            break;
		//          case S_ELC:
		//            ucElectricityNEC();       //Pay Electricity Bill
		//            break;
		//          case S_CARD_TRANS:
		//            ucCardTransfer();         //Transfer money to card number
		//            break;
		//          case S_ACNT_TRANS:
		//            ucAccountTransfer();       //Transfer money to Account number
		//            break;
		//          case S_MNY_DEPOS:
		//            ucMoneyDeposit();         //Deposit money
		//            break;
		//          case S_MNY_TRANS:
		//            ucMoneyTransfer();
		//            break;
		//          case S_MNY_RCV:
		//            ucMoneyReceive();
					break;
//				  case S_BLNC_INQ:
//					ucBalanceInquiry();			//Balance Inquiry transaction
//					break;
		//          case S_PN_CHNG:
		//            ucPINChange();				//Change PIN
		//            break;
				  case S_VOID:
					ucVoid();              		//Cancel Transaction in the same Batch
					break;
		//          case S_PURCH_CASH_BK:
		//            ucPurchaseCashback();
		//            break;
		//          case S_MIN_STATE:
		//            ucMiniStatement();
		//            break;
				  case S_RFND:
					ucRefund();			//Refund
					break;
				  case S_BTCH:
					ucBatch(TRUE);
					break;
				  case S_SERV:
					ucOtherServs();
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
	      HighlightOn(0,0,16);
	    }


  }
  

}
