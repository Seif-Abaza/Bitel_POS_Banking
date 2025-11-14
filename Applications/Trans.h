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
#ifndef TRANS_H
  #define TRANS_H

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
extern bool bSetTransDateTime(void);
extern char * pcGetTransName(enuTransType udtTansType, enuVendType udtVendType);
extern void vidSaveReversalInfo(void);

extern uchar ucStartTrans(bool isDefualtMode,ulong ulPCode);
extern uchar ucPurchase(bool isDefualtMode);
extern uchar ucBillPayment(void);
extern uchar ucTopUpPayment(void);
extern uchar ucElectricityNEC(void);
extern uchar ucCardTransfer(void);
extern uchar ucAccountTransfer(void);
extern uchar ucMoneyDeposit(void);
extern uchar ucMoneyTransfer(void);
extern uchar ucBalanceInquiry(void);
extern uchar ucOtherServs(void);
extern uchar ucPINChange(void);
extern uchar ucMoneyReceive(void);
extern uchar ucVoid(void);
extern uchar ucPurchaseCashback(void);
extern void vidBatchTimeChkr(void);
extern uchar ucBatch(bool bWithPassWrd);
extern uchar ucMiniStatement(void);
extern uchar ucRefund(void);


#endif /*TRANS_H*/



