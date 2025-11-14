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
#ifndef TRANS_SND_RCV_H
  #define TRANS_SND_RCV_H

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
extern uchar ucPurchSndRcv(void);
extern uchar ucBillInqSndRcv(uchar ucVenIdx);
extern uchar ucPymntSndRcv(uchar ucVenIdx, bool bIsTopUp);
extern uchar ucNECSndRcv(void);
extern uchar ucCardTransSndRcv(void);
extern uchar ucAccntTransSndRcv(void);
extern uchar ucMoneyDepositSndRcv(void);
extern uchar ucMoneyTransRcvSndRcv(bool bIsTrans);
extern uchar ucBalInqSndRcv(void);
extern uchar ucPINChangeSndRcv(uchar *pucNewPin);
extern uchar ucVoidSndRcv(void);
extern uchar ucPurchaseCashbackSndRcv(void);
extern uchar ucMiniStatSndRcv(void);
extern uchar ucRefundSndRcv(void);
extern uchar ucBatchSndRcv(void);
extern uchar ucEchoTestSndRcv(void);
extern uchar ucNewKeysInq(void);
extern uchar ucReversal(void);

extern void gsoap(void);
extern int gsoap_NBE(char * EMVTags, int TagsStrLen,int TXNIndex);
extern int gsoap_BatchUpload();
extern void  setRequestElements(struct _ns1__ExecuteTransaction *RequestMessage,
								char* EMVtags,
								int TagsStrLen,
								int TXNIndex);
#endif /*TRANS_SND_RCV_H*/


