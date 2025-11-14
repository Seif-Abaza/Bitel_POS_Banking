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
#include "Printer.h"
/******************************************************************************/
/* TYPES                                                                      */
/******************************************************************************/
/******************************************************************************/
/* CONSTANTS, MACROS                                                          */
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
// Build One Line Print Data
void BuildLinePrint(int noPage, int noLine,APP_LANG udtLang ,char *item, int item_len, char *msg, int msg_len, char attr, char justify_mode)
{
	int i, j;
	char *p, *q;
	char temp[34];
	unsigned char ch;
	
	if (item_len <= 0 && msg_len <= 0)
		return;
	
	p = (char *)&CHAR_BUFFER1[noPage];
	p += noLine * PRT_CHARACTERS_PER_LINE;
	q = (char *)&ATTB_BUFFER1[noPage];
	q += noLine * PRT_CHARACTERS_PER_LINE;
	
	if (item_len < 0)
		item_len = 0;
	if (item_len) {
		if (attr != PRT_DOUBLE_WIDTH_MODE) {
			if (item_len > PRT_CHARACTERS_PER_LINE) {
				item_len = PRT_CHARACTERS_PER_LINE;
			}
		}
		else {
			if ((item_len*2) > PRT_CHARACTERS_PER_LINE) {
				item_len = PRT_CHARACTERS_PER_LINE / 2;
			}
		}
		
		memset(temp, 0x00, sizeof(temp));
		
        if (udtLang == L_ARABIC) {
          ARABIC_Parser( item,temp,item_len);
          // Right justified
          j = PRT_CHARACTERS_PER_LINE - item_len; 
        } else {
          memcpy(temp, item, item_len);
          // left justified
          j = 0; 
        }
		
		i = 0;
		while (i < item_len) {
			p[j] = temp[i++];
			q[j] = attr;
			// Double width
			if (attr == PRT_DOUBLE_WIDTH_MODE) {
				j += 2;
			}
			else {
				j++;
			}
		}
	}
	
	if (msg_len <= 0) {
		return;
	}
	
	if (msg_len) {
		if (attr != PRT_DOUBLE_WIDTH_MODE) {
			if (msg_len > PRT_CHARACTERS_PER_LINE) {
				msg_len = PRT_CHARACTERS_PER_LINE;
			}
		}
		else {
			if ((msg_len*2) > PRT_CHARACTERS_PER_LINE) {
				msg_len = PRT_CHARACTERS_PER_LINE / 2;
			}
		}
		
		if (item_len) {
          /*
			if (attr != PRT_DOUBLE_WIDTH_MODE) {
				if ((item_len+msg_len) > PRT_CHARACTERS_PER_LINE) {
					msg_len = PRT_CHARACTERS_PER_LINE - item_len;
				}
			}
			else {
				if ((item_len+msg_len) > (PRT_CHARACTERS_PER_LINE/2)) {
					msg_len = (PRT_CHARACTERS_PER_LINE/2) - item_len;
				}
            }
            */
		}
		
		if (msg_len <= 0) {
			return;
		}
		
		memset(temp, 0x00, sizeof(temp));
        if (udtLang == L_ARABIC) {
          ARABIC_Parser( msg,temp,msg_len);
        } else {
          memcpy(temp, msg, msg_len);
        }

		
		if (item_len) {
          if (udtLang == L_ARABIC) {
            // left justified
            j = 0; 
          } else {
			// right justified
			if (attr != PRT_DOUBLE_WIDTH_MODE) {
				j = PRT_CHARACTERS_PER_LINE - msg_len;
			}
			else {
				j = PRT_CHARACTERS_PER_LINE - (msg_len*2);
			}
          }
		}
		else {
			if (justify_mode == LEFT_JUSTIFIED) {
				// left justified
				j = 0;
			}
			else if (justify_mode == CENTER_JUSTIFIED) {
				// center justified
				if (attr != PRT_DOUBLE_WIDTH_MODE) {
					j = (PRT_CHARACTERS_PER_LINE - msg_len) / 2;
				}
				else {
					j = (PRT_CHARACTERS_PER_LINE - (msg_len*2)) / 2;
				}
			}
			else {
				// right justified
				if (attr != PRT_DOUBLE_WIDTH_MODE) {
					j = PRT_CHARACTERS_PER_LINE - msg_len;
				}
				else {
					j = PRT_CHARACTERS_PER_LINE - (msg_len*2);
				}
			}
		}
		
		i = 0;
		while (i < msg_len) {
			p[j] = temp[i++];
			q[j] = attr;
			// Double width
			if (attr == PRT_DOUBLE_WIDTH_MODE) {
				j += 2;
			}
			else {
				j++;
			}
		}
	}

}

// Build One Line Print Data
void BuildAraLinePrint(int noPage, int noLine, char *item, int item_len, char *msg, int msg_len, char attr, char justify_mode)
{
  uchar aucAraItem[70];
  uchar aucAraMsg[70];

  memset(aucAraItem, 0, sizeof(aucAraItem));
  memset(aucAraMsg, 0, sizeof(aucAraMsg));

  ARABIC_Parser( item,aucAraItem,item_len);
  ARABIC_Parser( msg,aucAraMsg,msg_len);
  BuildLinePrint(noPage,noLine,1,aucAraItem,item_len,aucAraMsg,msg_len,attr,justify_mode);
}

/******************************************************************************/
/* EXPORTED FUNCTIONS                                                         */
/******************************************************************************/
#ifdef USER_DEBUG
void OutputReceiptData(int Page, int line)
{
	int i, j;
	unsigned char *ptr, str[33];
	
//	DBGPrt_m("\r\nOutputReceiptData()\r\n");
//	DBGPrt_m("line = %d\r\n", line);
//	DBGPrt_m("================================\r\n");
	
	ptr = (unsigned char *)CHAR_BUFFER1[Page];
	for (i = 0; i < line; i++) {
		memset(str, 0x00, sizeof(str));
		memcpy(str, ptr, 32);
		for (j = 0; j < 32; j++) {
			if (str[j] == 0x00) {
				str[j] = ' ';
			}
			else if ((str[j] >= 0xA0 && str[j] <= 0xA9)) {
				str[j] = (str[j] - 0xA0) + '0';
			}
			DBGPrt_m("%c", str[j]);
		}
		ptr += 32;
		DBGPrt_m("\r\n");
	}
	
//	DBGPrt_m("================================\r\n");
}
#endif


