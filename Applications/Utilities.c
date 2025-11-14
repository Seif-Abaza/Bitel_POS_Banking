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
#include "Utilities.h"

/******************************************************************************/
/* TYPES                                                                      */
/******************************************************************************/
/******************************************************************************/
/* CONSTANTS, MACROS                                                          */
/******************************************************************************/


//#define USER_DEBUG 1
extern int _etext;
extern int __usr_ram_start;
extern int __usr_ram_end;
extern int main(void);
/******************************************************************************/
/* PRIVATE VARIABLES                                                          */
/******************************************************************************/
static char acBuff[40];

/******************************************************************************/
/* PRIVATE FUNCTIONS PROTOTYPES                                               */
/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/******************************************************************************/
/* EXPORTED FUNCTIONS                                                         */
/******************************************************************************/

uchar getBit(uchar* data, int pos) {
	int posByte = pos/8;
	int posBit = pos%8;
	uchar valByte = data[posByte];
	int valInt = valByte>>(8-(posBit+1)) & 0x0001;
	return valInt;
}
uchar formatTime( uchar *Ptd, uchar *Pts ){
  uchar i ;
  for ( i = 0 ; i < 6 ; i++){
    *Ptd++ = *Pts++;
    if ( ( i == 1) || ( i == 3) ){
      *Ptd++ = ':';
    }
  }
  *Ptd = 0;
  return(SUCCESS);
}

uchar formatDate( uchar *Ptd, uchar *Pts ){
  unsigned char i ;
  if (udtAppLang == L_ARABIC){
    for ( i = 0 ; i < 8; i++){
      *Ptd++ = *Pts++;
      if ( ( i == 3) || ( i == 5) ){
        *Ptd++ = '/';
      }
    }
    *Ptd = 0;
  } else {
    *(Ptd+9)= 0;
    memcpy((Ptd+6),Pts,4);
    *(Ptd+5)= '/';
    memcpy((Ptd+3),(Pts+4),2);
    *(Ptd+2)= '/';
    memcpy((Ptd),(Pts+6),2);
  }
  return(SUCCESS);
}
uchar formatDateYYMMDD( uchar *Ptd, uchar *Pts ){
  unsigned char i ;
  //*Pts +=2;
  if (udtAppLang == L_ARABIC){
    for ( i = 0 ; i < 6; i++){
      *Ptd++ = *Pts++;
      if ( ( i == 1) || ( i == 3) ){
        *Ptd++ = '-';
      }
    }
    *Ptd = 0;
  } else {
    *(Ptd+8)= 0;
    memcpy((Ptd+6),Pts,2);
    *(Ptd+5)= '-';
    memcpy((Ptd+3),(Pts+2),2);
    *(Ptd+2)= '-';
    memcpy((Ptd),(Pts+4),2);
  }
  return(SUCCESS);
}
uchar formatExpDate( uchar *Ptd, uchar *Pts ){
  unsigned char i ;
  if (udtAppLang == L_ARABIC){
    for ( i = 0 ; i < 4; i++){
      *Ptd++ = *Pts++;
      if ( ( i == 1)  ){
        *Ptd++ = '\\';
      }
    }
    *Ptd = 0;
  } else {
    *(Ptd+6)= 0;
    memcpy((Ptd+3),Pts,2);
    *(Ptd+2)= '/';
    memcpy((Ptd),(Pts+2),2);
  }
  return(SUCCESS);
}
uchar FormatFloat(uchar *Ptd, uchar *Pts)
{
  uchar ucIdx;
  uchar ucStrLen = strlen(Pts);
  for (ucIdx = 0; ucIdx < ucStrLen; ucIdx++) {
    if (ucIdx == (ucStrLen - 2)) {
      *Ptd++ = '.';
    }
    *Ptd++= *Pts++;
  }
  *Ptd = 0;
  return(SUCCESS);
}
uchar FormatPAN( uchar *Ptd, uchar *Pts ){
  uchar ucSrcStrLen;
  uchar ucIdx;

  ucSrcStrLen = strlen(Pts);
  for (ucIdx = 0; ucIdx< ucSrcStrLen; ucIdx++) {
    if ((ucIdx<6) || (ucIdx >= (ucSrcStrLen - 4))){
      *Ptd++= *Pts++;
    } else {
      *Ptd++ = '*';
      Pts++;
    }
  }
  *Ptd = 0;
  return(SUCCESS);
}
char* pcFrmDgts(enuStrType udtStrType, char* pcDgts){
  char acTempBuff[50];

  memset(acTempBuff, 0, sizeof(acTempBuff));
  switch (udtStrType) {
  case DATE:
    formatDate(acTempBuff,pcDgts);
    break; 
  case DATEYYMMDD:
    formatDateYYMMDD(acTempBuff,pcDgts);
    break; 
  case EXP_DATE:
    formatExpDate(acTempBuff,pcDgts);
    break; 
  case TIME:
    formatTime(acTempBuff,pcDgts);
    break;
  case FLOAT:
    FormatFloat(acTempBuff,pcDgts);
    break;
  case DIGIT:
    strcpy(acTempBuff,pcDgts);
    break;
  case PAN_T:
    FormatPAN(acTempBuff,pcDgts);
    break;
  default:
    break;
  }
  if (udtAppLang == L_ARABIC) {
    sprintf(acBuff, "{%s}", acTempBuff); 
  }else{
    sprintf(acBuff, "%s", acTempBuff);
  }
  //DBGPrt_m("acBuff:%s",acBuff);

  return acBuff;
}

long FlashMemoryTest(char rom_info)
{
	int start_sector_no, total_sector_no;
	long sector_size, start_addr, ltemp;
#ifdef USER_DEBUG
	DBGPrt_m("Flash ROM Test\r\n");
	DBGPrt_m("_etext = %08lX\r\n", (long)&_etext);
#endif
	
	switch (rom_info) {
		case 0x30:	// NONE
			LCD_DisplayLine(1, 0, "Flash ROM - NONE");
			return -2;
		case 0x34:	// 2 Mbytes
			total_sector_no = 32;
			break;
		case 0x35:	// 4 Mbytes
			total_sector_no = 64;
			break;
		case 0x36:	// 8 Mbytes
		case 0x37:
			total_sector_no = 128;	// 64 KB/ sector
			break;
		case 0x38:	// 16 Mbytes
			total_sector_no = 128;	// 128 KB/ sector
			break;
		case 0x39:	// 32 Mbytes
			//total_sector_no = 256;	// 128 KB/ sector
			total_sector_no = 255;	// 128 KB/ sector (doesn't support the last one sector: 128 KB)
			break;
		default:	// Unknown Code
			LCD_DisplayLine(1, 0, "Flash ROM - ??? ");
			return -3;
	}
	
	if (rom_info < 0x38) {
		sector_size = 0x10000L;		// 64 KB
	}
	else {
		sector_size = 0x20000L;		// 128 KB
	}
	
	start_addr = (long)&_etext & 0x0FFFFFFF;
#ifdef USER_DEBUG
	DBGPrt_m("Size of code = %08lX\r\n", start_addr);
#endif
	if (start_addr % 16) {
		ltemp = start_addr / 16;
		start_addr = (ltemp+1) * 16;
	}
#ifdef USER_DEBUG
	DBGPrt_m("Load address of user data = %08lX\r\n", start_addr);
#endif
	
	start_addr += (long)&__usr_ram_end - (long)&__usr_ram_start;
#ifdef USER_DEBUG
	DBGPrt_m("Size of code & user data = %08lX\r\n", start_addr);
#endif
	if (start_addr % sector_size) {
		start_addr /= sector_size;
		start_addr += 1;
	}
	else {
		start_addr /= sector_size;
	}
	start_sector_no = (int)start_addr;
	
	start_addr *= sector_size;
	start_addr += FLASH_ROM_START_ADDR;
#ifdef USER_DEBUG
	DBGPrt_m("Test start address of flash = %08lX\r\n", start_addr);
	DBGPrt_m("Test start sector of flash = %d\r\n", start_sector_no);
#endif
	
	
	return start_addr;
}

long SramTest(char sram_info)
{
	int i, j, k, random_no, start_sector_no, total_sector_no;
	long start_addr;
	unsigned char data, *ucptr1, *ucptr2;
	char temp[17];
	
#ifdef USER_DEBUG
	DBGPrt_m("SRAM Test\r\n");
	DBGPrt_m("__usr_ram_end = %08lX\r\n", (long)&__usr_ram_end);
#endif
	
	switch (sram_info) {
		case 0x30:	// NONE
			ClearLCD();
			Lprint(0,0,"SRAM      - NONE ");
			return -2;
		case 0x31:	// 256 Kbytes
			total_sector_no = 4;
			break;
		case 0x34:	// 2 Mbytes
			total_sector_no = 32;
			break;
		case 0x35:	// 4 Mbytes
			total_sector_no = 64;
			break;
		case 0x36:	// 8 Mbytes
		case 0x37:
			total_sector_no = 128;
			break;
		case 0x38:	// 16 Mbytes
			total_sector_no = 256;
			break;
		case 0x39:	// 32 Mbytes
			total_sector_no = 512;
			break;
	default:	// Unknown Code
		ClearLCD();
    Lprint(0,0,"SRAM      - ??? ");
			return -3;
	}
	
	if ((long)&main < FLASH_ROM_START_ADDR) {	// SDRAM application
		start_sector_no = 0;
		start_addr = SRAM_START_ADDR;
	}
	else {										// Flash application
		start_addr = (long)&__usr_ram_end & 0x0FFFFFFF;
		if (start_addr % 0x10000L) {
			start_addr /= 0x10000L;
			start_addr += 1;
		}
		else {
			start_addr /= 0x10000L;
		}
		start_sector_no = (int)start_addr;
		
		start_addr *= 0x10000L;
		start_addr += SRAM_START_ADDR;
	}
#ifdef USER_DEBUG
	DBGPrt_m("start_addr = %08lX\r\n", start_addr);
	DBGPrt_m("start_sector_no = %d\r\n", start_sector_no);
#endif
	
	ClearLCD();
	Lprint(0,0,"SRAM      - OK ");
	
	return start_addr;
}
