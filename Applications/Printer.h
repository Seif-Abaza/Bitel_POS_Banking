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
#ifndef PRINTER_H
  #define PRINTER_H

/*****************************************************************************/
/* INCLUDE FILES                                                             */
/*****************************************************************************/

/*****************************************************************************/
/* CONSTANTS, MACROS                                                         */
/*****************************************************************************/
#define PRT_BYTES_PER_LINE				48		// Bytes per line: PRT_BYTES_PER_LINE*8=384 pixels per line
#define PRT_CHARACTERS_PER_LINE			32		// Charaters per line
#define PRT_LINES_PER_PAGE				36		// Lines per page
#define PRT_MAX_LINES_PER_PAGE			150		// Maximum lines per page[up to 150]: depends on Application

#define PRT_LINE_SPACE					0		// Line space gap[up to 15]: depends on Application

#define LEFT_JUSTIFIED			0
#define CENTER_JUSTIFIED		1
#define RIGHT_JUSTIFIED			2

/*
Bit information for Printing attribute buffer data  

bit 7 6 5 4 3 2 1 0 
    | | | |
    | | | +-------------- under line required
    | |	+---------------- double width
    | +------------------ graphics prinitng required
    +-------------------- line space adjust required 

0x00=normal, 0x10=under line, 0x20=double width
*/
#define PRT_NORMAL_MODE					0x00	// normal
#define PRT_UNDER_LINE_MODE				0x10	// under line required
#define PRT_DOUBLE_WIDTH_MODE			0x20	// double width
#define PRT_GRAPHICS_MODE				0x40	// graphics prinitng required

#define PRT_PAPER_FORM_FEED_LINE		6		// 6 lines

#define PRT_TIME_OUT					1000	// 10 sec
#define PRT_FINISH_TIME_OUT				100		// 1 sec

/*****************************************************************************/
/* TYPES                                                                     */
/*****************************************************************************/


/*****************************************************************************/
/* FUNCTIONS                                                                 */
/*****************************************************************************/
extern void BuildLinePrint(int noPage, int noLine,APP_LANG udtLang ,char *item, 
                           int item_len, char *msg, int msg_len, char attr, char justify_mode);


extern void InitializePrinterParam(void);
 
#ifdef USER_DEBUG
	extern void OutputReceiptData(int Page, int line);
#endif


#endif /*PRINTER_H*/


