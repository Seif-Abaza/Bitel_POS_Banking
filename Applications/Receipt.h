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
#ifndef RECEIPT_H
  #define RECEIPT_H

/*****************************************************************************/
/* INCLUDE FILES                                                             */
/*****************************************************************************/

/*****************************************************************************/
/* CONSTANTS, MACROS                                                         */
/*****************************************************************************/


/*****************************************************************************/
/* TYPES                                                                     */
/*****************************************************************************/


/*****************************************************************************/
/* FUNCTIONS                                                                 */
/*****************************************************************************/

extern uchar ucPrntCustomerRct(void);
extern uchar ucPrntMerchantRct(void);
extern uchar ucPrntFailedRct(void);
extern uchar ucRePrintRct(void);

extern uchar ucPrintRct(bool bIsCustCopy, bool bIsSucceededTrans, bool bIsRePrint);
extern uchar ucPrntBatch(void);
extern uchar ucReprintMenu(void);
extern uchar ucReportMenu(void);
extern uchar ucBreadReport(ulong ulTransNo, ulong ulBreadNo,
                           ulong ulInspTransNo, ulong ulInspBreadNo);
extern uchar ucBreadDispRcpt(bool bIsInsp);




#endif /*RECEIPT_H*/


