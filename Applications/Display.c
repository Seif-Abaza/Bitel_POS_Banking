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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./resources/Msgs.h"
#include "../../BF_SDK/include/global.h"
#include "UTILS.h"
#include "AppGlobal.h"

/******************************************************************************/
/* TYPES                                                                      */
/******************************************************************************/
/******************************************************************************/
/* CONSTANTS, MACROS                                                          */
/******************************************************************************/
#define NUM_CHAR_PER_LINE   16
#define NUM_LINE_PER_SCRN   3
#define SCREEN_WIDTH 		128
#define SCREEN_HEIGHT		64


#define CFG_MAXDISPCHAR  16
#define CFG_MAXAMOUNTLEN 8 //changed by heba from 20 to 13
#define CFG_MAXPRINTCHAR 40
#define ONESECOND        1000
#define SUCCESS 0
#define ERR_CANCEL 1

/******************************************************************************/
/* PRIVATE VARIABLES                                                          */
/******************************************************************************/
static unsigned short usWaittime;
static char gbKeyCnt=0;
static char gbTmpKeyBuf[20];
static unsigned char aucAmount[16];
/******************************************************************************/
/* PRIVATE FUNCTIONS PROTOTYPES                                               */
/******************************************************************************/
unsigned char Display_Input(unsigned char ucLine,
						   unsigned char ucClrFlag,
                           unsigned char ucMin, 
                           unsigned char ucMax,
               			   unsigned char ucType,
               			   unsigned char *pucBuf,
                            APP_LANG udtLang,
                		   unsigned char *pucMask,
                            unsigned short headerIdx);
/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/
void vidNumReverse(ulong num ,uchar* pucReturnText)
{
  uchar b[10];
  uchar ucLen;
  int i;
  sprintf(b,"%d\0",num);
  ucLen = strlen(b);
  memset(pucReturnText,0,ucLen);
  pucReturnText[ucLen] = '\0';
  for(i=0;i<ucLen;i++)
  {
    pucReturnText[ucLen-i-1] = b[i];
  } 
}
/******************************************************************************/
/* EXPORTED FUNCTIONS                                                         */
/******************************************************************************/

uchar ucDispMultiLineMsg(char* pcMsg, uchar ucInLine, ushort timeOut){
  unsigned int lineSize=NUM_CHAR_PER_LINE;
  unsigned int startPosition=0;

  uchar ucMsgLen = 0;
  uchar ucRemMsgLen = 0;
  uchar ucIdx;
  uchar ucLine;
  uchar ucBuffLine = 0;
  uchar aucBuff[90];
  uchar aucLineMsg[80];
  uchar aucTempLineMsg[80];

  //clear screen
  ClearLCD();

  ucLine = ucInLine;
  sprintf(aucBuff,"%s",pcMsg);
  ucMsgLen = strlen(aucBuff);
  ucRemMsgLen = ucMsgLen; 
  memset(aucTempLineMsg,0,sizeof(aucTempLineMsg));
  memcpy(aucTempLineMsg,aucBuff,ucMsgLen);

  while (ucRemMsgLen > 0) {
    memset(aucLineMsg,0,sizeof(aucLineMsg));
    if (ucRemMsgLen > 16) {
      for (ucIdx = 16; ucIdx >= 0; ucIdx--) {
        if (aucTempLineMsg[ucIdx] == ' ') {
          break;
        }
      }
      if (ucIdx == 0) {
        ucIdx = 16;
      }
      memcpy(aucLineMsg, aucTempLineMsg, ucIdx);
      ucRemMsgLen-=ucIdx;
      memset(aucTempLineMsg,0,sizeof(aucTempLineMsg));
      memcpy(aucTempLineMsg,&aucBuff[ucMsgLen - ucRemMsgLen + 1],ucRemMsgLen);
    }
    else{
      memcpy(aucLineMsg, aucTempLineMsg, ucRemMsgLen);
      ucRemMsgLen=0;
    }
    //ucRemMsgLen-=MIN(ucIdx,ucRemMsgLen);


    //DBGPrt_m("aucLineMsg: %s\r\n",aucLineMsg);
    if(udtAppLang == L_ARABIC){ 
      Lprint_arabic(ucLine++,0,aucLineMsg); 
    } else {
      Lprint(ucLine++,0,aucLineMsg);
    }
  }
  Wait1MS(timeOut); 
  return  ucLine; 
}
  uchar DisplayMsgBox(unsigned short headerIdx,
                     unsigned short displayedMsgIdx,
                     unsigned short  timeOut,
                     bool displayFrame )
  {
  	
  	unsigned int lineSize=NUM_CHAR_PER_LINE;
  	unsigned int startPosition=0;

    uchar ucMsgLen = 0;
    uchar ucIdx;
    uchar ucLine = 0;
    uchar aucBuff[70];
    uchar aucFrstLineMsg[30];
    uchar aucScndLineMsg[30];

  	//clear screen
  	ClearLCD();

  	//Title of the header
    if ((udtAppLang == L_ARABIC) && (strlen(GetMsgs(headerIdx))>0)) {
      Highlight_Arabic(ucLine++,0,GetMsgs(headerIdx));
    }
    else //english
    {
      if (udtAppLang == L_ARABIC){
      	Lprint(ucLine,0,GetLangMsgs(headerIdx,L_ENGLISH)); 
      } else {
        //check if frame should be displayed
        if(displayFrame == true) {
          DrawFrame();
          lineSize=lineSize-2;
          startPosition=1;
        }
        Lprint(ucLine,startPosition,GetMsgs(headerIdx));
      }
   	  HighlightOn(ucLine++,0,NUM_CHAR_PER_LINE);
    }//display it for the given time in ms


    if ((udtAppLang == L_ARABIC) && (strlen(GetMsgs(displayedMsgIdx)) <= 0)) { 
      sprintf(aucBuff,"%s",GetLangMsgs(displayedMsgIdx,L_ENGLISH));
    } else {
      sprintf(aucBuff,"%s",GetMsgs(displayedMsgIdx));
    }

    memset(aucFrstLineMsg,0,sizeof(aucFrstLineMsg));
    ucMsgLen = strlen(aucBuff);

    if (ucMsgLen > 16) {
      for (ucIdx = 16; ucIdx >= 0; ucIdx--) {
        if (aucBuff[ucIdx] == ' ') {
          break;
        }
      }
      if (ucIdx == 0) {
        ucIdx = 16;
      }
      memset(aucScndLineMsg,0,sizeof(aucScndLineMsg));
      memcpy(aucFrstLineMsg, aucBuff, ucIdx); 
      memcpy(aucScndLineMsg,&aucBuff[ucIdx + 1],ucMsgLen - ucIdx);
      DBGPrt_m("aucFrstLineMsg%s aucScndLineMsg%s\r\n",aucFrstLineMsg,aucScndLineMsg);
    } else {
      memcpy(aucFrstLineMsg, aucBuff, ucMsgLen); 
    }

    //DBGPrt_m("aucFrstLineMsg: %s\r\n",aucFrstLineMsg);
    //DBGPrt_m("aucScndLineMsg: %s\r\n",aucScndLineMsg);
    if ((udtAppLang == L_ARABIC) && (strlen(GetMsgs(displayedMsgIdx))>0)){ 
      Lprint_arabic(ucLine++,0,aucFrstLineMsg);
      if (ucMsgLen > 16){
        Lprint_arabic(ucLine++,0,aucScndLineMsg);
      }
    } else {
      Lprint_n(ucLine++,startPosition,lineSize,aucFrstLineMsg);
      if (ucMsgLen > 16){
        Lprint_n(ucLine++,startPosition,lineSize,aucScndLineMsg);
      }
    }

  	Wait1MS(timeOut); 
    return  ucLine;	
  }

unsigned char Display_EnterAmount(unsigned char ucLine,
                				 unsigned long *pulAmount,
                				 unsigned long ulMinAmount, 
                				 unsigned long ulMaxAmount,
                                  unsigned short headerIdx)
{
    unsigned char   aucDisp[CFG_MAXDISPCHAR+1];     /* Buffer for Display */
    unsigned char   aucInput[CFG_MAXAMOUNTLEN+1];   /* Buffer for Key Input */
    unsigned char   ucKey;
    unsigned char   ucRedraw;
    unsigned char   ucCount;
    unsigned char   ucI;
    unsigned char   ucJ;
    unsigned int    uiTimeout;
    unsigned long   ulAmount;

    uchar    aucPrntBuff[40];
    uchar    aucTempPrntBuff[40];
    uchar    aucTempBuff[50];
    uchar    aucTempNoBuff[5];
    uchar    ucAraTempLen;
    uchar    ucTempLen;
    uchar    ucTempSecPartIdx;
    uchar    ucTempRemIdx;
    uchar    ucTempNoLen;
    uchar    ucScrollIdx;
    bool     bIsLongTxt = 0;
    uchar    ucPartDisplay = 0;
    ushort   usWaittime;


    memset(&aucDisp[0],0,sizeof(aucDisp));
    memset(&aucDisp[0],' ',CFG_MAXDISPCHAR);
    memcpy(aucDisp,GetLangMsgs(R_SDG,L_ENGLISH),strlen(GetLangMsgs(R_SDG,L_ENGLISH)));

    aucDisp[CFG_MAXDISPCHAR-1] = '0';
    aucDisp[CFG_MAXDISPCHAR-2] = '0';
    aucDisp[CFG_MAXDISPCHAR-3] = '.';
    aucDisp[CFG_MAXDISPCHAR-4] = '0';
    memset(&aucInput[0],0,sizeof(aucInput));
    ucRedraw = TRUE;
    uiTimeout = 50*ONESECOND;
    ucCount = 0;
    ucKey=0xFF;

    if (udtAppLang == L_ARABIC){
      ucAraTempLen = strlen(GetMsgs(headerIdx));
      if (ucAraTempLen>0){
        sprintf(aucTempBuff,"%s",GetMsgs(headerIdx));
      } else {
        sprintf(aucTempBuff,"%s",GetLangMsgs(headerIdx,L_ENGLISH));
      }
    } else {
      sprintf(aucTempBuff,"%s",GetMsgs(headerIdx));
    }
    ucTempLen = strlen(aucTempBuff);
    if ((ucTempLen)>16) {
      ucTempRemIdx = ucTempLen - 16;
      bIsLongTxt = 1;
    } else {
      bIsLongTxt = 0;
    }
    do
    {
        if( ucRedraw == TRUE)
        {
        	Lprint(ucLine, 0, "                          ");
        	Lprint_n(ucLine,0,sizeof(aucDisp),&aucDisp[0]);
            ucRedraw = FALSE;

        }


        usWaittime =Get1MSTimer();
         do{
           ucKey = (unsigned char)(WaitKeyPress10MS(20) & 0xFF);

           if ((ucKey==0xFF) && (bIsLongTxt == 1)) {
             if (ucScrollIdx <= ucTempRemIdx) {
               ucScrollIdx ++;
             } else {
               ucScrollIdx = 0;
             }
             sprintf(aucTempPrntBuff, "%s", &aucTempBuff[ucScrollIdx]); 
             memset(aucPrntBuff,0,sizeof(aucPrntBuff));
             memcpy(aucPrntBuff,aucTempPrntBuff,strlen(aucTempPrntBuff)); 

             if ((udtAppLang == L_ARABIC) && (ucAraTempLen > 0)) {
               Highlight_Arabic(0,0,aucPrntBuff);
             } else {
               ClearLCDLine(0);
               Lprint(0,0,aucPrntBuff);
               LCD_HighlightLineOn(0);
             }
           }


         }while((ucKey==0xFF) && (delta1MS(usWaittime)<uiTimeout));


        if (ucKey==0xFF)
        {
            return(ERR_CANCEL);
        }


        switch(ucKey)
        {
        case VK_CANCEL:
            if( ucCount )
            {
                ucCount = 0;
                memset(&aucDisp[0],0,sizeof(aucDisp));
                memset(&aucDisp[0],' ',CFG_MAXDISPCHAR);
                memcpy(aucDisp,GetLangMsgs(R_SDG,L_ENGLISH),strlen(GetLangMsgs(R_SDG,L_ENGLISH)));
                aucDisp[CFG_MAXDISPCHAR-1] = '0';
                aucDisp[CFG_MAXDISPCHAR-2] = '0';
                aucDisp[CFG_MAXDISPCHAR-3] = '.';
                aucDisp[CFG_MAXDISPCHAR-4] = '0';
                ucRedraw = TRUE;
            }else
            {
                return(ERR_CANCEL);
            }
            break;
        case VK_CLEAR:
            if( ucCount )
            {
                ucCount --;
                aucInput[ucCount] = 0;
                ucRedraw = TRUE;
            }
            break;
        case VK_NO0:
      		 if(ucCount==0)
       		 {
        		BEEP_TONE(1, 2);
        	 }
        	 else
        	 {
                if( ucCount < CFG_MAXAMOUNTLEN)
                {
                    aucInput[ucCount] = ucKey+0x30;
                    ucCount ++;
                    ucRedraw = TRUE;
                }
       		 }
//            if( ucCount > (CFG_MAXAMOUNTLEN-2))
//            {
//            	BEEP_TONE(1, 2);
//                break;
//            }
//            memcpy(&aucInput[ucCount],"00",2);
//            ucCount = ucCount +2;
//            ucRedraw = TRUE;
            break;
        case VK_ENTER:
            ulAmount = KEYPAD_asc_long(aucInput,ucCount);
            /*if(ulAmount == 0)
                break;*/
            if( ulAmount < ulMinAmount )
            {
            	BEEP_TONE(1, 2);
            }else
            {
                if( ulAmount > ulMaxAmount )
                {
                	BEEP_TONE(1, 2);
                }else
                {
                    *pulAmount = ulAmount;
                    return(SUCCESS);
                }
            }
            break;
        default :
            if(  (ucKey <= 9)
               &&(ucKey >= 0)
              )
            {
                if( ucCount < CFG_MAXAMOUNTLEN)
                {
                    aucInput[ucCount] = ucKey+0x30;
                    ucCount ++;
                    ucRedraw = TRUE;
                }
            }
            break;
        }
        /* Copy data from Input buffer to Display buffer */
        memset(&aucDisp[0],0,sizeof(aucDisp));
        memset(&aucDisp[0],' ',CFG_MAXDISPCHAR);
        memcpy(aucDisp,GetLangMsgs(R_SDG,L_ENGLISH),strlen(GetLangMsgs(R_SDG,L_ENGLISH)));
        aucDisp[CFG_MAXDISPCHAR-1] = '0';
        aucDisp[CFG_MAXDISPCHAR-2] = '0';
        aucDisp[CFG_MAXDISPCHAR-3] = '.';
        aucDisp[CFG_MAXDISPCHAR-4] = '0';
        if( ucCount )
        {
            for(ucI=0,ucJ=0;ucI<ucCount;ucI++,ucJ++)
            {
                if( ucJ == 2)
                {
                    ucJ ++;
                }
                aucDisp[CFG_MAXDISPCHAR-ucJ-1] = aucInput[ucCount-ucI-1];
            }
        }
    }while(1);
}

 /* *******************************************************
  * \brief  Display Amount prompt and retrieves user input
  * 
  * \param [in] headerTitle - Title
  * 	   [in] displayedMsg - Message to be displayed
  * 	   [in] ucMinAmount- Minimum value of inserted amount
  * 	   [in] ucMaxAmount- Maximum value of inserted amount
  * 	   [in/out] amount - The inserted amount
  * \return
  * 	    true 1  -  user input is correct
  * 		false 0 -  otherwise
  *  *******************************************************
  */
  bool Display_AmountPrompt(unsigned short headerIdx,
                           unsigned short displayedMsgIdx,
  						   unsigned long ucMinAmount,
  						   unsigned long ucMaxAmount,
  						   unsigned long *amount)
  {
  	
  	unsigned char pressedkey=0xFF;
    uchar ucLine;

  	//display password message box without frame
  	ucLine = DisplayMsgBox(headerIdx,displayedMsgIdx,0,false);

    pressedkey=Display_EnterAmount(ucLine,amount,ucMinAmount,ucMaxAmount,headerIdx);
  	//DBGPrt_m("display entered amount= %d",*amount);
  	//if cancel key was pressed
    if((pressedkey == ERR_CANCEL))
  	{
  		
  		return FALSE;
  	}
  	else
  	{
  		return  TRUE;
  	}
  }

uchar Display_IP_PROMPT(uchar YPos, uchar* pucBuf)
{
  unsigned char   ucKey;
  unsigned char   ucCount;
  ushort usWaittime;
  uchar arrucTempBuff[40];
  unsigned int    uiTimeout;

  memset(arrucTempBuff, 0, sizeof(arrucTempBuff));
  memcpy(arrucTempBuff, pucBuf,strlen(pucBuf));
  ucCount = strlen(arrucTempBuff);
  //memcpy(aucInput,pucBuf,ucCount);

  Lprint(YPos, 0, "                          ");
  Lprint_n(YPos, 0, ucCount, arrucTempBuff);

  uiTimeout = 50*1000;
  usWaittime =Get1MSTimer();

  while (1) {
    do{
        ucKey = (unsigned char)(WaitKeyPress10MS(10) & 0xFF);
    }while((ucKey==0xFF) && (delta1MS(usWaittime)<60000));

    if (ucKey==0xFF)
    {
        continue;
    }

    switch(ucKey)
    {
    case VK_CLEAR:
      if( ucCount == 0 )
      {
        LCD_BLINKING(CURSOR_OFF, YPos, ucCount, 0);
        CursorOff();
        return VK_CANCEL;
      }
      else
      {
        //ucCount--;
      }
      break;
    case VK_CANCEL :
      if( ucCount == 0 )
      {
          LCD_BLINKING(CURSOR_OFF, YPos, ucCount, 0);
          CursorOff();
          return ucKey;
      }
      else
      {
        while (ucCount > 0) {
          KEY_InputIPAddress(YPos,VK_CLEAR,arrucTempBuff);
          ucCount = strlen(arrucTempBuff);
        }
      }

        break;
    case VK_ENTER:
      if (KEY_CheckIPAddress(arrucTempBuff) == 0) {
        sprintf(pucBuf,"%s", arrucTempBuff);
        LCD_BLINKING(CURSOR_OFF, YPos, ucCount, 0);
        CursorOff();
        return ucKey;
      }
        break;
    default :
      if( ( (ucKey <= 9) &&(ucKey >= 0)) || (ucKey == VK_ASTERISK)){
      }
      break;
    }

    KEY_InputIPAddress(YPos,ucKey,arrucTempBuff);
    ucCount = strlen(arrucTempBuff);


  }
}
   /* *******************************************************
  * \brief  Display prompt for digits and retrieves user input
  * 
  * \param [in] headerTitle - Title
  * 	   [in] displayedMsg - Message to be displayed
  * 	   [in] ucMinLength- Minimum length of inserted digist
  * 	   [in] ucMaxLength- Maximum length of inserted digits
  * 	   [in/out] digits - The inserted digits
  * \return
  * 	    true 1  -  user input is correct
  * 		false 0 -  otherwise
  *  *******************************************************
  */  
  bool DispPrompt(unsigned short headerIdx,
                  unsigned short displayedMsgIdx,
  				  unsigned char ucMinLength,
  				  unsigned char ucMaxLength,
                  unsigned char ucType,
  				  unsigned char *pucBuff)
  {
  	
  	unsigned char pressedkey=0xFF;
    APP_LANG udtLang = udtAppLang;
    uchar ucMsgLen = 0;
    uchar ucIdx;
    uchar ucLine = 1;
    uchar aucBuff[70];
    uchar aucFrstLineMsg[30];
    uchar aucScndLineMsg[30];

    uchar startPosition = 0;
    uchar lineSize=16;

#if 0
    //clear screen
    ClearLCD(); 
    
    memset(aucFrstLineMsg, 0, sizeof(aucFrstLineMsg)); 
    memset(aucScndLineMsg, 0, sizeof(aucScndLineMsg)); 
    
    if ((udtAppLang == L_ARABIC) && (strlen(GetMsgs(displayedMsgIdx)) <= 0)) {
      sprintf(aucBuff, "%s", GetLangMsgs(displayedMsgIdx, L_ENGLISH));
    } else {
      sprintf(aucBuff, "%s", GetMsgs(displayedMsgIdx));
    }
    ucMsgLen = strlen(aucBuff); 
    
    if (ucMsgLen > 16) {
      for (ucIdx = 16; ucIdx >= 0; ucIdx--) {
        if (aucBuff[ucIdx] == ' ') {
          break;
        }
      }
      if (ucIdx == 0) {
        ucIdx = 16;
      }
      memcpy(aucFrstLineMsg, aucBuff, ucIdx); 
      memcpy(aucScndLineMsg, &aucBuff[ucIdx + 1], ucMsgLen - ucIdx);
    } else {
      memcpy(aucFrstLineMsg, aucBuff, ucMsgLen); 
    }
    
    DBGPrt_m("aucFrstLineMsg: %s\r\n", aucFrstLineMsg); 
    DBGPrt_m("aucScndLineMsg: %s\r\n", aucScndLineMsg); 
    if ((udtAppLang == L_ARABIC) && (strlen(GetMsgs(displayedMsgIdx)) > 0)) {
      Lprint_arabic(ucLine++, 0, aucFrstLineMsg); 
      if (ucMsgLen > 16) {
        Lprint_arabic(ucLine++, 0, aucScndLineMsg);
      }
    } else {
      Lprint_n(ucLine++, startPosition, lineSize, aucFrstLineMsg); 
      if (ucMsgLen > 16) {
        Lprint_n(ucLine++, startPosition, lineSize, aucScndLineMsg);
      }
    }
#endif  	    

    //display password message box without frame
  	ucLine = DisplayMsgBox(headerIdx,displayedMsgIdx,0,false);

  	/* Get password string by keypad */
  	//call function to get password
    if (ucType == IP_PROMPT) 
    {
      pressedkey = Display_IP_PROMPT(2,pucBuff);
    }
    else
    {
      if ((ucType == ALPAH_EN_PROMPT) ||((ucType == HEX_PROMPT))) {
        udtLang = L_ENGLISH;
      }
      pressedkey = Display_Input(ucLine, 1, ucMinLength, ucMaxLength, ucType, pucBuff, udtLang, 0,headerIdx); 
    }
  	
  	//if cancel key was pressed
    if((pressedkey == VK_CANCEL))
  	{
  		return FALSE;
  	}
  	else
  	{
  		return TRUE;
  	}
  }


  /*********************************************************************************
  * \brief Display Menu
  * 
  * \param [in] 	list 		 - List of displayed items it must be terminated with NULL
  * 	   [in/out] selectedItem - The selected item
  * 	   [in]timeout           -Timeout in ms
  * 
  * \return  -1  INPUT_TIMEOUT, timeout occured  
  * 		  0  INPUT_OK, Success
  * 		  1  INPUT_CANCEL,  menu canceled
  * 
  * *******************************************************************************
  */
int DisplayMsgMenu(unsigned short MenuHdr,
				unsigned short *selectedItem, 
				unsigned short  timeOut,
                unsigned short MenuStartIdx,
                unsigned short MenuEndIdx)
{
	unsigned short	ucCurrMenuItem=0;
	unsigned short	ucDispMenuItem=0;
	unsigned short  ucFirstItemToDisp=0;
	unsigned short	ucMenuCount=0;
	unsigned short	ucKey=0xFF;
	unsigned short	ucLineNum = NUM_LINE_PER_SCRN;//number of lines per screen
	unsigned short	ucDispNum;
	unsigned short  lineToHighlight=0;
    uchar    aucRevBuff[4];
    uchar    aucPrntBuff[40];
    uchar    aucTempPrntBuff[40];
    uchar    aucTempBuff[50];
    uchar    aucTempHdrBuff[50];
    uchar    aucTempNoBuff[5];
    uchar    ucAraTempLen;
    uchar    ucAraHdrLen;
    uchar    ucTempLen;
    uchar    ucTempNoLen;
    uchar    ucTempSecPartIdx;
    uchar    ucTempRemIdx;
    uchar    ucTempHdrLen;
    uchar    ucTempHdrRemIdx;
    uchar    ucScrollHdrIdx;
    uchar    ucScrollIdx;
    bool     bIsLongTxt = 0;
    bool     bIsLongHdr = 0;
    bool     bIsScroll = 0;
    uchar    ucPartDisplay = 0;
    ushort   usWaittime;

    ucMenuCount= MenuEndIdx - MenuStartIdx + 1;

	//ucCurrMenuItem = 0;
	//DBGPrt_m("ucCurrMenuItem=%d\r\n",ucCurrMenuItem);

    if (udtAppLang == L_ARABIC) 
    {
      ucAraHdrLen = strlen(GetMsgs(MenuHdr));
      if (ucAraHdrLen>0){
        sprintf(aucTempHdrBuff,"%s",GetMsgs(MenuHdr));
      }else {
        sprintf(aucTempHdrBuff,"%s",GetLangMsgs(MenuHdr,L_ENGLISH));
      }
    } else {
      sprintf(aucTempHdrBuff,"%s",GetMsgs(MenuHdr));
    }

    ucTempHdrLen = strlen(aucTempHdrBuff);
    if ((ucTempHdrLen)>16) {
      ucTempHdrRemIdx = ucTempHdrLen - 16;
      bIsLongHdr = 1;
    } else {
      bIsLongHdr = 0;
    }

	while(1)
	{

		ClearLCD();

        if (udtAppLang == L_ARABIC) 
        {
          if (ucAraHdrLen>0){
            Lprint_arabic(0,0,GetMsgs(MenuHdr));
          }else {
            Lprint(0,0,GetLangMsgs(MenuHdr,L_ENGLISH));
          }
        } else {
          Lprint(0,0,GetMsgs(MenuHdr));
        }

		//LCD_HighlightAllOff();
		//while number of displayed items is less than lcd size
		for(ucDispNum=0;ucDispNum < ucLineNum;ucDispNum++)
		{
			ucDispMenuItem = ucFirstItemToDisp + ucDispNum;

			if( ucDispMenuItem >= ucMenuCount ) {
				break;
			}
            memset(aucPrntBuff,0,sizeof(aucPrntBuff));
            memset(aucTempPrntBuff,0,sizeof(aucTempPrntBuff));

			/*Display menu*/
            if (udtAppLang == L_ARABIC) {
              if (strlen(GetMsgs(ucDispMenuItem + (MenuStartIdx )))>0) {
                vidNumReverse((ucDispMenuItem+1),aucRevBuff);
                sprintf(aucTempPrntBuff, "%s %s", aucRevBuff, GetMsgs(ucDispMenuItem + (MenuStartIdx)));
                memcpy(aucPrntBuff,aucTempPrntBuff,strlen(aucTempPrntBuff)); 
                  Lprint_arabic(ucDispNum + 1,0,aucPrntBuff);
              } else {
                sprintf(aucTempPrntBuff,"%d %s",(ucDispMenuItem+1),GetLangMsgs(ucDispMenuItem + (MenuStartIdx),L_ENGLISH));
                memcpy(aucPrntBuff,aucTempPrntBuff,strlen(aucTempPrntBuff)); 
                Lprint(ucDispNum + 1,0,aucPrntBuff);
              }
			}
			else {
              sprintf(aucTempPrntBuff,"%d %s",(ucDispMenuItem+1),GetMsgs(ucDispMenuItem + (MenuStartIdx)));
              memcpy(aucPrntBuff,aucTempPrntBuff,strlen(aucTempPrntBuff)); 
			  Lprint(ucDispNum + 1,0,aucPrntBuff);
			}

		}

        //ucPartDisplay = 0;
        bIsLongTxt = 0;
        memset(aucPrntBuff,0,sizeof(aucPrntBuff));
        memset(aucTempPrntBuff,0,sizeof(aucTempPrntBuff));
        memset(aucTempNoBuff,0,sizeof(aucTempNoBuff));
        memset(aucTempBuff,0,sizeof(aucTempBuff));
        if (udtAppLang == L_ARABIC) 
		{
          ucAraTempLen = strlen(GetMsgs(ucCurrMenuItem + (MenuStartIdx)));
          if (ucAraTempLen>0){
            vidNumReverse((ucCurrMenuItem+1),aucTempNoBuff);
            ucTempLen = ucAraTempLen + strlen(aucTempNoBuff) + 1;
            if (ucTempLen > 16) {
              sprintf(aucTempBuff,"%s",GetMsgs(ucCurrMenuItem + (MenuStartIdx)));
              ucTempRemIdx = ucTempLen - 16;
              bIsLongTxt = 1;
            }
            sprintf(aucTempPrntBuff, "%s %s", aucTempNoBuff, GetMsgs(ucCurrMenuItem + (MenuStartIdx))); 
            memcpy(aucPrntBuff,aucTempPrntBuff,strlen(aucTempPrntBuff)); 
            Highlight_Arabic((lineToHighlight +1),0,aucPrntBuff);
          }
          else
          {
            sprintf(aucTempNoBuff,"%d",(ucCurrMenuItem+1));
            ucTempLen = strlen( GetLangMsgs(ucCurrMenuItem + (MenuStartIdx),L_ENGLISH) ) + strlen(aucTempNoBuff) + 1;
            if (ucTempLen > 16) {
              sprintf(aucTempBuff,"%s",GetLangMsgs(ucCurrMenuItem + (MenuStartIdx),L_ENGLISH));
              ucTempRemIdx = ucTempLen - 16;
              bIsLongTxt = 1;
            }
			LCD_HighlightLineOn((lineToHighlight+1));

          }
		}
		else
		{
          sprintf(aucTempNoBuff,"%d",(ucCurrMenuItem+1));
          ucTempLen = strlen(GetMsgs(ucCurrMenuItem + (MenuStartIdx))) + strlen(aucTempNoBuff) + 1;
          if (ucTempLen > 16) {
            sprintf(aucTempBuff,"%s",GetMsgs(ucCurrMenuItem + (MenuStartIdx)));
            ucTempRemIdx = ucTempLen - 16;
            bIsLongTxt = 1;
          }
          LCD_HighlightLineOn((lineToHighlight+1));
		}
	
        usWaittime =Get1MSTimer();
        ucScrollIdx = 0;
        ucScrollHdrIdx = 0;
        bIsScroll = 1;
        do{
          ucKey = (unsigned char)(WaitKeyPress10MS(40) & 0xFF);
          if ((ucKey==0xFF)) {
            if ((bIsLongHdr)) {
              if (ucScrollHdrIdx <= ucTempHdrRemIdx) {
                ucScrollHdrIdx ++;
              } else {
                ucScrollHdrIdx = 0;
              }
              memset(aucTempPrntBuff,0,sizeof(aucTempPrntBuff));
              memset(aucPrntBuff,0,sizeof(aucPrntBuff));
              sprintf(aucTempPrntBuff, "%s",&aucTempHdrBuff[ucScrollHdrIdx]); 
              memcpy(aucPrntBuff,aucTempPrntBuff,strlen(aucTempPrntBuff)); 
              ClearLCDLine(0);

              if ((udtAppLang == L_ARABIC) && (ucAraHdrLen > 0)) {
                Lprint_arabic(0,0,aucPrntBuff);
              } else {
                Lprint(0,0,aucPrntBuff);
              }
            }
            if ((bIsLongTxt) && (bIsScroll)) {
              if (ucScrollIdx <= ucTempRemIdx) {
                ucScrollIdx ++;
              } else {
                ucScrollIdx = 0;
                bIsScroll = 0;
              }
              memset(aucTempPrntBuff,0,sizeof(aucTempPrntBuff));
              memset(aucPrntBuff,0,sizeof(aucPrntBuff));
              sprintf(aucTempPrntBuff, "%s %s", aucTempNoBuff, &aucTempBuff[ucScrollIdx]); 
              memcpy(aucPrntBuff,aucTempPrntBuff,strlen(aucTempPrntBuff));
              ClearLCDLine((lineToHighlight+1));
               
              if ((udtAppLang == L_ARABIC) && (ucAraTempLen > 0)) {
                  Highlight_Arabic((lineToHighlight +1),0,aucPrntBuff);
              } else {
                ClearLCDLine((lineToHighlight +1));
                Lprint((lineToHighlight +1),0,aucPrntBuff);
                LCD_HighlightLineOn((lineToHighlight+1));
              }
            }
          }
        }while ((ucKey==0xFF)&&(delta1MS(usWaittime) < timeOut)); 

		/* Wait until any key is pressed or time_out_10ms is elapsed */
		//ucKey = (unsigned char)(WaitKeyPress10MS(timeOut/10) & 0xFF);
		//Inputed key value
		switch(ucKey )
		{
			//in case of no key pressed
		case 0xFF:
			*selectedItem=ucCurrMenuItem;
			ClearLCD();
			DisplayIcon(UpArrow_OFF);
			DisplayIcon(DownArrow_OFF);
			return -1;// time out
		
		case VK_ENTER:
			ClearLCD();
			//retrive the number of selected item
			*selectedItem=ucCurrMenuItem;
			DisplayIcon(UpArrow_OFF);
			DisplayIcon(DownArrow_OFF);
			return(INPUT_OK);

			
		case VK_CANCEL:
		    ClearLCD();
		    DisplayIcon(UpArrow_OFF);
			DisplayIcon(DownArrow_OFF);
			return 1;
			
		case VK_SHAPE:
			if( ucCurrMenuItem+1 < ucMenuCount )
			{
				DisplayIcon(UpArrow_OFF);
				DisplayIcon(DownArrow_ON);
				
				
				ucCurrMenuItem += 1;
				if (lineToHighlight<NUM_LINE_PER_SCRN-1) //changed from 3
				{
					lineToHighlight++; 
				}
				else 
				{
					ucFirstItemToDisp++;
				}

			}
      else {
				ucCurrMenuItem = 0;
				lineToHighlight=0;
				ucFirstItemToDisp=0;
      }
			break;
		case VK_ASTERISK:
		//Go UP
			if( ucCurrMenuItem < 1 )
			{
				ucCurrMenuItem = ucMenuCount-1;
				lineToHighlight= min(NUM_LINE_PER_SCRN,ucMenuCount)-1;
				ucFirstItemToDisp=ucMenuCount - min(NUM_LINE_PER_SCRN,ucMenuCount);
			}
			else
			{
				DisplayIcon(DownArrow_OFF);
				DisplayIcon(UpArrow_ON);
				
				ucCurrMenuItem -= 1;
				if (lineToHighlight>0)
				{
                  lineToHighlight--;
				}
				else 
				{
                  ucFirstItemToDisp--;
				}
			}
			break;

    case VK_NO1:
    case VK_NO2:
    case VK_NO3:
    case VK_NO4:
    case VK_NO5:
    case VK_NO6:
    case VK_NO7:
    case VK_NO8:
    case VK_NO9:
      if (ucKey <= ucMenuCount) {
        *selectedItem = ucKey-1;
        DisplayIcon(UpArrow_OFF);
        DisplayIcon(DownArrow_OFF);
        return(INPUT_OK);
      }
      break; 

    default:
			break;
		}//end of switch case
	}//end of while loop	
}

//////////////////////////////////////////////////////////////////
unsigned char Display_Input(unsigned char ucLine,
						   unsigned char ucClrFlag,
                           unsigned char ucMin, 
                           unsigned char ucMax,
               			   unsigned char ucType,
               			   unsigned char *pucBuf,
                            APP_LANG udtLang,
                		   unsigned char *pucMask,
                            unsigned short headerIdx)
{
	unsigned char  key;


    const unsigned char aucKeyTab[MAX_NUM_LANG][10][8]=
    {
      {
          {"0 .,_*-\0"},
          {"1QZqz#@\0"},
          {"2ABCabc\0"},
          {"3DEFdef\0"},
          {"4GHIghi\0"},
          {"5JKLjkl\0"},
          {"6MNOmno\0"},
          {"7PRSprs\0"},
          {"8TUVtuv\0"},
          {"9WXYwxy\0"},
      },
      {
        {"0 .,_*-\0"},
        {"1#@\0"},
        {"2fjme\0"},
        {"3hxY\0"},
        {"4sawq\0"},
        {"5].v`\0"},
        {"6[po\0"},
        {"7ki,d\0"},
        {"8tr;gl\0"},
        {"9'/uy\0"},
      }
    };

    unsigned char   aucDisp[52];    /* Buffer for Display */
    unsigned char   aucInput[52];   /* Buffer for Key Input */
    unsigned char   ucKey;
    unsigned char   ucLastKey;
    unsigned char   ucKeyTabOffset;
    unsigned char   ucEnd;
    unsigned char   ucRedraw;
    unsigned char   ucCount;
    unsigned char   ucOffset;
    static unsigned int uiTimeout;
    static unsigned int uiLastTime;

    uchar    aucPrntBuff[40];
    uchar    aucTempPrntBuff[40];
    uchar    aucTempBuff[50];
    uchar    aucTempNoBuff[5];
    uchar    ucAraTempLen;
    uchar    ucTempLen;
    uchar    ucTempSecPartIdx;
    uchar    ucTempRemIdx;
    uchar    ucTempNoLen;
    uchar    ucScrollIdx;
    bool     bIsLongTxt = 0;
    uchar    ucPartDisplay = 0;
    ushort   usWaittime;

    memset(&aucDisp[0],0,sizeof(aucDisp));
    memset(&aucInput[0],0,sizeof(aucInput));
    ucLastKey = 0;
    ucEnd = 0;
//  ucRedraw = FALSE;
    ucRedraw = 1;
    uiLastTime = uiTimeout;
    ucCount = 0;


    if( ucClrFlag )
    {
    	Lprint(ucLine, 0, "                          ");
    }

    if( strlen((char *)pucBuf))
    {
        ucRedraw = TRUE;
        ucCount = strlen((char *)pucBuf);
        if( ucCount > sizeof(aucInput))
        {
            ucCount = sizeof(aucInput);
        }
        memcpy(aucInput,pucBuf,ucCount);
    }


    ucAraTempLen = strlen(GetMsgs(headerIdx));
    if ((udtAppLang == L_ARABIC) && (ucAraTempLen <= 0)){
      sprintf(aucTempBuff,"%s",GetLangMsgs(headerIdx,L_ENGLISH));
    } else {
      sprintf(aucTempBuff,"%s",GetMsgs(headerIdx));
    }
    ucTempLen = strlen(aucTempBuff);
    if ((ucTempLen)>16) {
      ucTempRemIdx = ucTempLen - 16;
      bIsLongTxt = 1;
    } else {
      bIsLongTxt = 0;
    }


    do
    {
        if( ucRedraw == TRUE)
        {
        	
            memset(&aucDisp[0],0,sizeof(aucDisp));
            if( ucCount > 15)
            {	//password
                if(  (ucType == 'p')
                   ||(ucType == 'P')
                  )
                {
                    memset(&aucDisp[0],'*',15);
                }
#if 0
else if ((ucType == 'C') || (ucType == 'c')) {
                if (ucCount < 17) {
                  memcpy(&aucDisp[0], &aucInput[ucCount - 15], 3); 
                  memset(&aucDisp[3], '*', 8); 
                  memcpy(&aucDisp[12], &aucInput[0], ucCount - 12);
                  
                } else if (ucCount < 20) {
                  memcpy(&aucDisp[0], &aucInput[ucCount - 15], 4 - (ucCount - 15)); 
                  memset(&aucDisp[4 - (ucCount - 15)], '*', 8 + (ucCount - 16)); 
                  memcpy(&aucDisp[12 + (ucCount - 16) - (ucCount - 15)], &aucInput[12], ucCount - 15);
                } else {
                  memcpy(&aucDisp[0], &aucInput[0], 4); 
                  memset(&aucDisp[4], '*', 8); 
                  memcpy(&aucDisp[12], &aucInput[0], ucCount - 12);
                }
              }
#endif                                
                else
                {
                	
#if 1
                    if(ucCount > ucMax)
                    {	//Alphabetic
                        if((ucType=='a')||(ucType=='A'))
                        {
                            memcpy(&aucDisp[0],&aucInput[ucCount-15],ucCount-1);

                        }
                        else
                            memcpy(&aucDisp[0],&aucInput[ucCount-15],ucCount);
                    }
                    else
#endif
                    	memcpy(&aucDisp[0],&aucInput[ucCount-15],15);
                    
                }
                if(ucCount > ucMax)
                {
                	//Alphabetic
                    if((ucType=='a')||(ucType=='A'))
                    {
                        aucDisp[ucCount-1] = '_';
                    }
                    else
                        aucDisp[ucCount] = '_';
                }
                else
                {
                    aucDisp[ucCount] = '_';
                }
            }
            else
            {
                if(  (ucType == 'p')||(ucType == 'P') )
                {
                    memset(&aucDisp[0],'*',ucCount);
                }
#if 0
else if ((ucType == 'C') || (ucType == 'c')) {
                if (ucCount < 4) {
                  memcpy(&aucDisp[0], &aucInput[0], ucCount);
                } else if (ucCount < 12) {
                  memcpy(&aucDisp[0], &aucInput[0], 4); 
                  memset(&aucDisp[4], '*', ucCount - 4);
                } else {
                  memcpy(&aucDisp[0], &aucInput[0], 4); 
                  memset(&aucDisp[4], '*', 8); 
                  memcpy(&aucDisp[12], &aucInput[0], ucCount - 12);
                }
              }
#endif                                
                else
                {

#if 1
                    if(ucCount > ucMax)
                    {
                        if((ucType=='a')||(ucType=='A'))
                            memcpy(&aucDisp[0],&aucInput[0],ucCount-1);
                        else
                            memcpy(&aucDisp[0],&aucInput[0],ucCount);
                    }
                    else
                    {
#endif
                        memcpy(&aucDisp[0],&aucInput[0],ucCount);
                    }
                }
                if(ucCount > ucMax)
                {
                    if((ucType=='a')||(ucType=='A'))
                        aucDisp[ucCount-1] = '_';
                    else
                        aucDisp[ucCount] = '_';
                }
                else
                {
                    aucDisp[ucCount] = '_';
                }
            }
            Lprint(ucLine, 0, "                          ");

            if (((ucType=='a')||(ucType=='A'))&& (udtLang == L_ARABIC)) 
            {
              Lprint_arabic(ucLine, 0, &aucDisp[0]); 
            }
            else
            {
              Lprint_n(ucLine, 0,ucMax, &aucDisp[0]);
            }
            //InputIPA(ucLine,ucKey,&aucDisp[0]);

            ucRedraw = FALSE;
        }
        uiTimeout = 50*1000;
        usWaittime =Get1MSTimer();
        uiLastTime = usWaittime;

        do{
          ucKey = (unsigned char)(WaitKeyPress10MS(20) & 0xFF);

          if ((ucKey==0xFF) && (bIsLongTxt == 1)) {
            if (ucScrollIdx <= ucTempRemIdx) {
              ucScrollIdx ++;
            } else {
              ucScrollIdx = 0;
            }
            sprintf(aucTempPrntBuff, "%s", &aucTempBuff[ucScrollIdx]); 
            memset(aucPrntBuff,0,sizeof(aucPrntBuff));
            memcpy(aucPrntBuff,aucTempPrntBuff,strlen(aucTempPrntBuff)); 

            if ((udtAppLang == L_ARABIC) && (ucAraTempLen > 0)) {
              Highlight_Arabic(0,0,aucPrntBuff);
            } else {
              ClearLCDLine(0);
              Lprint(0,0,aucPrntBuff);
              LCD_HighlightLineOn(0);

            }
          }
        }while((ucKey==0xFF) && (delta1MS(usWaittime)<60000));

        //DBGPrt_m("Key Code=%02X\r\n", ucKey);

        if(  (ucKey >= (unsigned char)0) && (ucKey <= (unsigned char)9) )
        {
            /*if( pucMask )
            {
                if( !strchr((char *)pucMask, ucKey ))
                {

                    //Os__timer_stop(&uiTimeout);
                    continue;
                }
            }*/
            switch(ucType)
            {
            	//hexaformat
            case 'h':
            case 'H':
                  if( ucCount > ucMax)
                 {
                 	BEEP_TONE(1, 2);
                     break;
                 }

                 if((( ucLastKey != ucKey)&&(ucCount < ucMax)))
                 {

                     ucLastKey = ucKey;
                     ucKeyTabOffset = 0;
                     aucInput[ucCount++] = ucKey+0x30;
                     ucRedraw = TRUE;
                 }
                 else
                 {

                 	if((  delta1MS(usWaittime) < 1*1000)&&(ucKey >= 2)  &&(ucKey <= 3))
                     {

                         if( ucCount )
                         {
                             ucOffset = ucCount -1;
                         }
                         else
                         {
                             ucOffset = 0;
                         }

                            if( ucKeyTabOffset < strlen((char *)aucKeyTab[L_ENGLISH][ucKey])-1)
						 {
							 ucKeyTabOffset ++;
							 ucRedraw = TRUE;
						 }

                         else
                         {
                             if( ucKeyTabOffset != 0)
                             {
                                 ucKeyTabOffset = 0;
                                 ucRedraw = TRUE;
                             }
                         }

                         aucInput[ucOffset] = aucKeyTab[udtLang][ucKey][ucKeyTabOffset];
                     }
                     else
                     {
                         if( ucCount >= ucMax)
                         {
                         	BEEP_TONE(1, 2);
                             break;
                         }
                         ucLastKey = ucKey;
                         ucKeyTabOffset = 0;
                         aucInput[ucCount++] = ucKey+0x30;
                         ucRedraw = TRUE;

                     }
                 }
                break;
            case 'a':
            case 'A':

               if( ucCount > ucMax)
                {
                	BEEP_TONE(1, 2);
                    break;
                }

                if((( ucLastKey != ucKey)&&(ucCount < ucMax)))
                {

                    ucLastKey = ucKey;
                    ucKeyTabOffset = 0;
                    aucInput[ucCount++] = ucKey+0x30;
                    ucRedraw = TRUE;
                }
                else
                {

                	if(  delta1MS(usWaittime) < 1*1000)
                    {
                    	
                        if( ucCount )
                        {
                            ucOffset = ucCount -1;
                        }
                        else
                        {
                            ucOffset = 0;
                        }
                        if( ucKeyTabOffset < strlen((char *)aucKeyTab[udtLang][ucKey])-1)
                        {
                            ucKeyTabOffset ++;
                            ucRedraw = TRUE;
                        }
                        else
                        {
                            if( ucKeyTabOffset != 0)
                            {
                                ucKeyTabOffset = 0;
                                ucRedraw = TRUE;
                            }
                        }

                        aucInput[ucOffset] = aucKeyTab[udtLang][ucKey][ucKeyTabOffset];
                    }
                    else
                    {
                        if( ucCount >= ucMax)
                        {
                        	BEEP_TONE(1, 2);
                            break;
                        }
                        ucLastKey = ucKey;
                        ucKeyTabOffset = 0;
                        aucInput[ucCount++] = ucKey+0x30;
                        ucRedraw = TRUE;

                    }
                }

                break;
            case 'I':
                 if( ucCount > ucMax)
                 {
                 	BEEP_TONE(1, 2);
                     break;
                 }

                 if(( ucLastKey != ucKey)&&(ucCount < ucMax))
                 {

                     ucLastKey = ucKey;
                     ucKeyTabOffset = 0;
                     aucInput[ucCount++] = ucKey+0x30;
                     ucRedraw = TRUE;
                 }
                 else
                 {

                 	if(  (delta1MS(usWaittime) < 1*1000) && (ucKey==0))
                     {

                         if( ucCount )
                         {
                             ucOffset = ucCount - 1;
                         }
                         else
                         {
                             ucOffset = 0;
                         }
                         if( ucKeyTabOffset < strlen((char *)aucKeyTab[L_ENGLISH][ucKey])-1)
                         {
                             ucKeyTabOffset ++;
                             ucRedraw = TRUE;
                         }
                         else
                         {
                             if( ucKeyTabOffset != 0)
                             {
                                 ucKeyTabOffset = 0;
                                 ucRedraw = TRUE;
                             }
                         }


                       aucInput[ucOffset] = aucKeyTab[L_ENGLISH][ucKey][ucKeyTabOffset];

                     }
                     else
                     {
                         if( ucCount >= ucMax)
                         {
                         	BEEP_TONE(1, 2);
                             break;
                         }
                         ucLastKey = ucKey;
                         ucKeyTabOffset = 0;
                         aucInput[ucCount++] = ucKey+0x30;
                         ucRedraw = TRUE;

                     }
                 }

                 break;

            default:
                if( ucCount >= ucMax)
                {
                	BEEP_TONE(1, 2);
                    break;
                }
                aucInput[ucCount++] = ucKey+0x30;
                ucRedraw = TRUE;
                break;
            }
        }
        else
        {
            switch(ucKey)
            {
            case VK_CANCEL :
                if( ucCount )
                {
                    ucCount = 0;
                    ucLastKey = 0;
                    memset(&aucInput[0],0,sizeof(aucInput));
                    memset(&aucDisp[0],0,sizeof(aucDisp));
                    ucRedraw = TRUE;
                }
                else
                {
    
                   // ucKey = -1; //Removed by hebas
                    ucEnd = TRUE;
                }

                break;
            case 0x00:
                if(  (ucType == 'h')||(ucType == 'H')||(ucType == 'a')||(ucType == 'A') )
                {
//                  if( ucCount < strlen((char *)&aucInput[0]))
//                      ucCount ++;
                    if( ucCount > ucMax)
                    {
                    	BEEP_TONE(1, 2);
                        break;
                    }
                    if( ucLastKey != ucKey)
                    {
                        ucLastKey = ucKey;
                        ucKeyTabOffset = 0;
                        if((ucKey != 0)&&(ucKey != (','-0x30))&&(ucKey != ('.'-0x30)))
                            aucInput[ucCount++] = '0';
                        else
                            aucInput[ucCount++] = ucKey+0x30;
                        ucRedraw = TRUE;
                    }
                    else
                    {
                    	if(  delta1MS(usWaittime) < 1*1000)
                        {
                            if( ucCount )
                            {
                                ucOffset = ucCount - 1;
                            }
                            else
                            {
                                ucOffset = 0;
                            }
                            if( ucKeyTabOffset < strlen((char *)aucKeyTab[udtLang][10])-1)
                            {
                                ucKeyTabOffset ++;
                                ucRedraw = TRUE;
                            }
                            else
                            {
                                if( ucKeyTabOffset != 0)
                                {
                                    ucKeyTabOffset = 0;
                                    ucRedraw = TRUE;
                                }
                            }
                            aucInput[ucOffset] = aucKeyTab[udtLang][10][ucKeyTabOffset];
                        }
                        else
                        {
                            if( ucCount >= ucMax)
                            {
                            	BEEP_TONE(1, 2);
                                break;
                            }
                            ucLastKey = ucKey;
                            ucKeyTabOffset = 0;
                            if((ucKey != 0)&&(ucKey != (','-0x30))&&(ucKey != ('.'-0x30)))
                                aucInput[ucCount++] = '0';
                            else
                                aucInput[ucCount++] = ucKey+0x30;
                            ucRedraw = TRUE;
                        }
                    }
                }
                else
                {
                    if( ucCount > (ucMax-2))
                    {
                    	BEEP_TONE(1, 2);
                        break;
                    }
//                  aucInput[ucCount] = ucKey;
                    memcpy(&aucInput[ucCount],"\0\0",2);
                    ucCount = ucCount +2;
                    ucRedraw = TRUE;
                    break;
                }
                break;
            case VK_CLEAR://changed by heba
                if( ucCount )
                {
                	
                    if(ucCount > ucMax)
                    {
                        if((ucType=='a')||(ucType=='A'))
                        {
                            ucCount = ucCount-2;
                        }
                        else
                        {
                            ucCount = ucCount -1;
                        }
                    }
                    else
                    {
                        ucCount --;
                    }
                    
                    aucInput[ucCount] = 0;

                    ucRedraw = TRUE;
                }
                break;
            case VK_ENTER:
                if( ucCount < ucMin )
                {
                	BEEP_TONE(1, 2);
                }
                else
                {
                    memset((char *)pucBuf,0x00,sizeof(pucBuf));
                    strcpy((char *)pucBuf,(char *)&aucInput[0]);
                    ucEnd = TRUE;
                }
                break;
            default :
                break;
            }
        }

//        uiLastTime = uiTimeout;
        //Os__timer_stop(&uiTimeout);
        if( ucEnd == TRUE)
        {
            break;
        }
    }while(1);
//    Os__timer_stop(&uiTimeout);

    if (pucMask!=0) *pucMask=ucCount;
    return(ucKey);
}

 /* * *******************************************************************************
  * \brief  Display ModalDialog with maximum number of 3 possible choices
  * 
  * \param [in] headerTitle     - Title
  * 	   [in] displayedMsg 	- Message to be displayed within one line
  * 	   [in] timeOut     	- Timeout in ms
  *        [in] dialogButtons[] - List of user choices
  * 	   [in] numOfChoices 	- Number of user choices
  * 	   [in] defaultChoice   - Default choice will be highlighted and assigned 
  * 							  to key ENTER
  * 
  * \return

  * 	-2  Invalid value of Parameters
  *     -1  timeout occured  
 		 0  first MODAL_PARAM has been validated  
 		 1  second MODAL_PARAM has been validated  
 		 2  third MODAL_PARAM has been validated 
 		 3  cancelled
  *  * *******************************************************************************
  */
int DispModalDialog( unsigned short headerIdx,
                     unsigned short displayedMsgIdx,
                     char* displayedMsg,
						unsigned short  timeOut,
					    unsigned char*dialogButtons[],
					    unsigned short numOfChoices,
					    unsigned short defaultChoice)

{
	
	unsigned short currentSelection=defaultChoice;
	unsigned char	ucKey = 0xFF;
	unsigned short widthOfButton;
    uchar ucLine;

	
	//get the number of choices
	//DBGPrt_m("numOfChoices=%d \r\n", numOfChoices);
	//check the size of array
	if((numOfChoices > 3) || 
	   (defaultChoice > numOfChoices ))
	{
		return -2;
	}
	//get the width of each button
	widthOfButton = NUM_CHAR_PER_LINE / numOfChoices;
	
	int i=0;
	//current selction is the default one
	currentSelection=defaultChoice;
	//display frame,message and header
  	ucLine = DisplayMsgBox(headerIdx,displayedMsgIdx,0,FALSE);
    if (strlen(displayedMsg) > 0) {
      Lprint(1,0,displayedMsg);
    }
	//DBGPrt_m("numOfChoices=%d,widthOfButton=%d \r\n", numOfChoices, widthOfButton);
	
while(1)
{
	  	//Title of the header
    if (udtAppLang == L_ARABIC) 
  	//if(strcmp(currentLanguage,"Arabic")==0)
    {	
    		//display buttons
		for(i=0;i<numOfChoices;i++)
		{
    				//highlight the required button
			if(i==currentSelection)
			{
				/* Highlight on n characters on LCD */
				/* YPos:0-3, XPos:0-15, Count:1-16 */													 
				Highlight_Arabic(4-1,
							     i*widthOfButton,
							     (char*)dialogButtons[i]);
			}
			else
				Lprint_arabic(4-1,i*widthOfButton,(char*)dialogButtons[i]);
		}
    }
    else	//Another language
    {

    	HighlightOff(4-1, 0,NUM_CHAR_PER_LINE);
    	//display buttons
		for(i=0;i<numOfChoices;i++)
		{
			
			Lprint_n(4-1,
					 i*widthOfButton,
					 widthOfButton,
					 (char*)dialogButtons[i]);
					 
			//highlight the selected button
			if(i==currentSelection)
			{
				/* Highlight on n characters on LCD */
				/* YPos:0-3, XPos:0-15, Count:1-16 */													 
				HighlightOn(4-1,i*widthOfButton,widthOfButton);
			}

		}//end of for loop
    }//end of if

	/* Wait until any key is pressed or time_out_10ms is elapsed */
	ucKey = (unsigned char)( WaitKeyPress10MS(timeOut/10) & 0xFF);
	//DBGPrt_m("currentSelection====%d\r\n",currentSelection);
	
	//Inputed key value
	switch(ucKey )
	{
		case 0xFF:
				return -1; //time out
		case  VK_CANCEL:
				return 3; //cancelled
		case VK_ENTER:
				return currentSelection;			
		case  VK_ASTERISK://VK_NO1 :

                    if (udtAppLang == L_ARABIC) 
					//if(strcmp(currentLanguage,"Arabic")==0)
    				{
    					if(currentSelection>= 0&& currentSelection <(numOfChoices -1))
						{
							currentSelection++;
						}
    				}
    				else
    				{
    					
    					if(currentSelection >0 && (currentSelection <= (numOfChoices -1)))
						{
							currentSelection--;
						}
    				}
				
		   		break;
			case  VK_SHAPE://VK_NO3 :
			
                    if (udtAppLang == L_ARABIC) 
					//if(strcmp(currentLanguage,"Arabic")==0)
    				{
    					if(currentSelection >0 && (currentSelection <= (numOfChoices -1)))
						{
							currentSelection--;
						}
    				}
    				else
    				{
    					if((currentSelection>= 0)&& currentSelection <(numOfChoices -1))
						{
							currentSelection++;
						}
					}
		
		}//end of switch case
   }//end of while loop
	return -1;
}

char insertDOTnComma( char *str, unsigned char gbCurrencyExp )
{
    int i, size;

    memset( gbTmpKeyBuf, 0x00, sizeof(gbTmpKeyBuf) );
    size = strlen( str );

    if(gbCurrencyExp) {
        if(size > gbCurrencyExp) {
            memcpy((char *)&gbTmpKeyBuf[0], (char *)&str[0], (size-gbCurrencyExp));
            i=size-gbCurrencyExp;
            gbTmpKeyBuf[i++]='.';
            memcpy((char *)&gbTmpKeyBuf[i], (char *)&str[i-1], gbCurrencyExp);
            i = size+1;
        }else {
            gbTmpKeyBuf[0]='0';
            gbTmpKeyBuf[1]='.';
            memset((char *)&gbTmpKeyBuf[2], '0', gbCurrencyExp);
            if(size) {
                memcpy((char *)&gbTmpKeyBuf[2+(gbCurrencyExp-size)], (char *)&str[0], size);
            }
            i=gbCurrencyExp+2;
        }

    }else {
        i = size;
        while ( i > 3 ) { i -= 3; size++; }
        i = 0 ;
        while(i < size) {
            if((size > 4) && (((size - i) == 4) || ((size - i) == 8) || ((size - i) == 12))) {
                 gbTmpKeyBuf[i++] = ',';
            }else {
                gbTmpKeyBuf[i++] = *str++;
            }
        }
        if(i==0) gbTmpKeyBuf[i++] = '0';
    }
    return( i );
}
void InputNoAmount_m( char line, char x_top, char x_max, char keymax, char key )
 {
     int i, x_min;

     switch( key ) {
         case BTKEY_CLEAR:
             --gbKeyCnt;
             if(gbKeyCnt < 0) {
                 gbKeyCnt = 0;
                 ClearLCDLine(line);
                 memset((char *)aucAmount, 0, sizeof(aucAmount));
             }
             aucAmount[gbKeyCnt] = 0x00;
             i = insertDOTnComma( (char *)aucAmount, 2);
             x_min = x_top - i;
             ClearLCDLine(line);
             Lprintf( line, x_min, gbTmpKeyBuf );
             break;
         case BTKEY_SHARP:     /* triple zero */
         case BTKEY_STAR:      /* double zero */
             if(gbKeyCnt <= 0) break;
             if(gbKeyCnt >= x_max) break;
             aucAmount[gbKeyCnt++]='0';
             for(i=BTKEY_STAR; i<=key; i++) {
                 if(gbKeyCnt < x_max) aucAmount[gbKeyCnt++]='0';
             }
             i = insertDOTnComma( (char *)aucAmount , 2);
             x_min = x_top - i;
             Lprintf( line, x_min, gbTmpKeyBuf );
             break;
         default:
             if(!gbKeyCnt && !key) break;
             if(key > keymax) break;
             if(gbKeyCnt >= x_max) break;
             aucAmount[gbKeyCnt++] = key | '0';
             i = insertDOTnComma( (char *)aucAmount , 2);
             x_min = x_top - i;
             Lprintf( line, x_min, gbTmpKeyBuf );
             break;
     }
     DBGprt(("Amount=%s, %s\n", aucAmount, gbTmpKeyBuf));
 }

unsigned char Display_InputNoAmount(char line,
                                    unsigned long *pulAmount,
                                    unsigned long ulMinAmount,
                                    unsigned long ulMaxAmount,
                                    unsigned short headerIdx)
{
	unsigned char   aucDisp[CFG_MAXDISPCHAR+1];     /* Buffer for Display */
	unsigned char   aucInput[CFG_MAXAMOUNTLEN+1];   /* Buffer for Key Input */
	unsigned char   ucKey;
	unsigned char   ucRedraw;
	unsigned char   ucCount;
	unsigned char   ucI;
	unsigned char   ucJ;
	unsigned int    uiTimeout;
	unsigned long   ulAmount;

	uchar    aucPrntBuff[40];
	uchar    aucTempPrntBuff[40];
	uchar    aucTempBuff[50];
	uchar    aucTempNoBuff[5];
	uchar    ucAraTempLen;
	uchar    ucTempLen;
	uchar    ucTempSecPartIdx;
	uchar    ucTempRemIdx;
	uchar    ucTempNoLen;
	uchar    ucScrollIdx;
	bool     bIsLongTxt = 0;
	uchar    ucPartDisplay = 0;
	ushort   usWaittime;


	memset(&aucDisp[0],0,sizeof(aucDisp));
	memset(&aucDisp[0],' ',CFG_MAXDISPCHAR);
	memcpy(aucDisp,GetLangMsgs(R_SDG,L_ENGLISH),strlen(GetLangMsgs(R_SDG,L_ENGLISH)));

	aucDisp[CFG_MAXDISPCHAR-1] = '0';
	aucDisp[CFG_MAXDISPCHAR-2] = '0';
	aucDisp[CFG_MAXDISPCHAR-3] = '.';
	aucDisp[CFG_MAXDISPCHAR-4] = '0';
	memset(&aucInput[0],0,sizeof(aucInput));
	ucRedraw = TRUE;
	uiTimeout = 50*ONESECOND;
	ucCount = 0;
	ucKey=0xFF;

	if (udtAppLang == L_ARABIC){
	  ucAraTempLen = strlen(GetMsgs(headerIdx));
	  if (ucAraTempLen>0){
		sprintf(aucTempBuff,"%s",GetMsgs(headerIdx));
	  } else {
		sprintf(aucTempBuff,"%s",GetLangMsgs(headerIdx,L_ENGLISH));
	  }
	} else {
	  sprintf(aucTempBuff,"%s",GetMsgs(headerIdx));
	}
	ucTempLen = strlen(aucTempBuff);
	if ((ucTempLen)>16) {
	  ucTempRemIdx = ucTempLen - 16;
	  bIsLongTxt = 1;
	} else {
	  bIsLongTxt = 0;
	}

	 memset((char *)aucAmount, 0, sizeof(aucAmount));	//heba
	 memset( gbTmpKeyBuf, 0x00, sizeof(gbTmpKeyBuf) );	//heba
	 gbKeyCnt=0;

	do
	{
	  /*
        if( ucRedraw == TRUE)
        {
        	Lprint(ucLine, 0, "                          ");
        	Lprint_n(ucLine,0,sizeof(aucDisp),&aucDisp[0]);
            ucRedraw = FALSE;

        }
	   */


	  usWaittime =Get1MSTimer();
	  do{
		ucKey = (unsigned char)(WaitKeyPress10MS(20) & 0xFF);

		if ((ucKey==0xFF) && (bIsLongTxt == 1)) {
		  if (ucScrollIdx <= ucTempRemIdx) {
			ucScrollIdx ++;
		  } else {
			ucScrollIdx = 0;
		  }
		  sprintf(aucTempPrntBuff, "%s", &aucTempBuff[ucScrollIdx]);
		  memset(aucPrntBuff,0,sizeof(aucPrntBuff));
		  memcpy(aucPrntBuff,aucTempPrntBuff,strlen(aucTempPrntBuff));

		  if ((udtAppLang == L_ARABIC) && (ucAraTempLen > 0)) {
			Highlight_Arabic(0,0,aucPrntBuff);
		  } else {
			ClearLCDLine(0);
			Lprint(0,0,aucPrntBuff);
			LCD_HighlightLineOn(0);
		  }
		}


	  }while((ucKey==0xFF) && (delta1MS(usWaittime)<uiTimeout));



	  if (ucKey==0xFF) { return(ERR_CANCEL); }
	  switch(ucKey)
	  {
		case VK_CANCEL:
		{
		  ulAmount = 0;
		  return(ERR_CANCEL);
		}
		break;
		case VK_ENTER:
		  ulAmount = KEYPAD_asc_long(aucAmount,gbKeyCnt);
		  /*if(ulAmount == 0)
                break;*/
		  if( ulAmount < ulMinAmount )
		  {
			BEEP_TONE(1, 2);
		  }else
		  {
			if( ulAmount > ulMaxAmount )
			{
			  BEEP_TONE(1, 2);
			}else
			{
			  *pulAmount = ulAmount;
			  return(SUCCESS);
			}
		  }
		  break;
		default :
		  InputNoAmount_m(2,15,12,9,ucKey);
		  break;
	  }
	}while(1);
}

bool Display_InputNoAmountPrompt(unsigned short headerIdx,
                         unsigned short displayedMsgIdx,
						   unsigned long ucMinAmount,
						   unsigned long ucMaxAmount,
						   unsigned long *amount)
{

	unsigned char pressedkey=0xFF;
  uchar ucLine;

	//display password message box without frame
	ucLine = DisplayMsgBox(headerIdx,displayedMsgIdx,0,false);

  pressedkey=Display_InputNoAmount(ucLine,amount,ucMinAmount,ucMaxAmount,headerIdx);
	//Rprintf("display entered amount= %d",*amount);
	//if cancel key was pressed
  if((pressedkey == ERR_CANCEL))
	{

		return FALSE;
	}
	else
	{
		return  TRUE;
	}
}
