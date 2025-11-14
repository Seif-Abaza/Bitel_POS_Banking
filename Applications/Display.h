/******************************************************************************/
/* COMPANY  : Bitel EMEA Co.                                                  */
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
#ifndef DISPLAY_H
  #define DISPLAY_H
/******************************************************************************/
/* INCLUDE FILES                                                              */
/******************************************************************************/

/******************************************************************************/
/* CONSTANTS, MACROS                                                          */
/******************************************************************************/


/******************************************************************************/
/* TYPES                                                                      */
/******************************************************************************/



/******************************************************************************/
/* CONSTANTS                                                                  */
/******************************************************************************/


/******************************************************************************/
/* PRIVATE VARIABLES                                                          */
/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTIONS PROTOTYPES                                               */
/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/******************************************************************************/
/* EXPORTED FUNCTIONS                                                         */
/******************************************************************************/
extern unsigned char ucDispMultiLineMsg(char* pcMsg, unsigned char ucInLine, unsigned short timeOut);

extern unsigned char DisplayMsgBox(unsigned short headerIdx,
                     unsigned short displayedMsgIdx,
                     unsigned short  timeOut,
                     bool displayFrame );
extern int DisplayMsgMenu(unsigned short MenuHdr,
				unsigned short *selectedItem, 
				unsigned short  timeOut,
                unsigned short MenuStartIdx,
                unsigned short MenuEndIdx);
extern int DispModalDialog( unsigned short headerIdx,
                     unsigned short displayedMsgIdx,
                     char* displayedMsg,
						unsigned short  timeOut,
					    unsigned char*dialogButtons[],
					    unsigned short numOfChoices,
					    unsigned short defaultChoice);

 
extern bool DispPrompt(unsigned short headerIdx,
					   unsigned short displayedMsgIdx,
					   unsigned char ucMinLength,
					   unsigned char ucMaxLength,
					   unsigned char ucType,
					   unsigned char *pucBuff);
bool Display_InputNoAmountPrompt(unsigned short headerIdx,
                         unsigned short displayedMsgIdx,
						   unsigned long ucMinAmount,
						   unsigned long ucMaxAmount,
						   unsigned long *amount);
#endif

