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
#ifndef DEV_CONFIG_H
  #define DEV_CONFIG_H

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
extern void vidEnDisDbgPort(void);
extern void vidCheckSignalStrength(void);
extern void vidSetIconClock(void);
extern void vidDiplayVersion(void);
extern void vidOnPowerOnCheck(void);
extern uchar ucDeviceConfig(void);
extern uchar ucSetMerchPassword();
extern void Animation();
//extern int compressme(const uchar* in,unsigned int uiInLen,uchar* out, unsigned int* uioutLen);
//extern uchar ucCompressMiniLZO(const uchar* in,unsigned int uiInLen,uchar* out,unsigned int* uioutLen);

#endif /*DEV_CONFIG_H*/


