/*
 * UTILS.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: Mohamed
 */

#ifndef UTILS_H_
#define UTILS_H_


extern void UTIL_Form_Montant(unsigned char *Mnt_Fmt,unsigned long Montant,unsigned char Pos_Virgule);
extern unsigned long asc_long(unsigned char *ucData, unsigned char ucLen);
extern unsigned long asci_long(unsigned char *ucData, unsigned char ucLen);
extern void long_asci(unsigned char *ucpResult,int ucCount,unsigned long *ulpData);
extern void long2asci(unsigned char *ucpResult,int ucCount,unsigned long ulData);
extern void long_str(unsigned char *ucpResult,int ucBase,unsigned long *ulpData);
extern unsigned long bcd_long(unsigned char *ucData, unsigned char ucLen);
//extern unsigned long bcd_to_long(unsigned char *ucData, unsigned char ucLen);
extern void long_bcd(unsigned char *ucpResult,int ucBase,unsigned long *ulpData);
extern unsigned long tab_long(unsigned char *ucIp,unsigned char len);
extern void long_tab(unsigned char *ucDest,unsigned char len,unsigned long *ulVal);
extern void int_bcd(unsigned char *ucpResult,int ucBase,unsigned int *ulpData);
extern unsigned char UTIL_CalcGoldenCardSHMAC(unsigned char *pucInData,unsigned short uiInLen,unsigned char *pucOutMAC);
extern void hex_asc(unsigned char *aucAscData,unsigned char *aucHexData,unsigned char len);
extern void asc_hex(unsigned char *aucHexData ,int dlen,unsigned char *aucAscData,int slen);
extern void int_asc(unsigned char *ucpResult,int ucCount,unsigned int *uipData);
extern void short_bcd(unsigned char *ucpResult,int ucBase,unsigned short *uspData);
extern void int_tab(unsigned char *ucDest,unsigned char len,unsigned int *uiVal);
extern unsigned char UTIL_format_date_bcd_str( unsigned char *Ptd, unsigned char *Pts );
extern unsigned char UTIL_format_time_bcd_str( unsigned char *Ptd, unsigned char *Pts );

extern void UTIL_ClearScren(void);

ulong GetFlashLong(unsigned long *pulAddress);
ushort GetFlashshort(unsigned short *pusAddress);
void SetFlashLong(unsigned long *pulAddress,int n,unsigned long *ulData);
void setFlashshort(unsigned long *pulAddress,int n,unsigned short *ulData);


#endif /* UTILS_H_ */
