
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
#ifndef SNDRCVISO8583_H
  #define SNDRCVISO8583_H

/*****************************************************************************/
/* INCLUDE FILES                                                             */
/*****************************************************************************/

/*****************************************************************************/
/* CONSTANTS, MACROS                                                         */
/*****************************************************************************/

extern int  			LeftTrN;
extern int  			MaxTrN;
extern unsigned short   usCommHdrIdx;
/*****************************************************************************/
/* TYPES                                                                     */
/*****************************************************************************/


/*****************************************************************************/
/* FUNCTIONS                                                                 */
/*****************************************************************************/
extern int GPRSConnectionDemo(void);
extern int GPRSInitRegConnect(ushort usHdrIdx);
uchar SIM_Selection(ushort usHdrIdx);
extern uchar PortConfig(uchar bIsDisp);
extern unsigned char COMMS_TransSendReceive(void);
extern unsigned char COMMS_TransOnline(void);
extern unsigned char COMMS_Init();
extern unsigned char COMMS_Connect(char *cpIP,char *cpPORT,unsigned short usTimeOut);
unsigned char gethostbyname_m(const char* hostname, char* IP );
unsigned char COMMS_DisConnect(void);
unsigned char COMMS_TransOnlineNoConn(void);
extern unsigned char COMMS_SendReceiveGprs(unsigned char *pucInData,unsigned short uiInLen,
					unsigned char *pucOutData,unsigned int *puiOutLen);

#endif /*SNDRCVISO8583_H*/

