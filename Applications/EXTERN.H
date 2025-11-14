
/*======================================================================*/
/*																		*/
/*								EXTERN.H								*/
/*																		*/
/*======================================================================*/

//#ifndef EXTERN_H
//#define EXTERN_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "include/drv/driver.h"

#include "include/lib/keydef.h"
#include "include/lib/lcdfontc.h"
#include "include/lib/prtfontc.h"
#include "include/lib/lcdlogo.h"
#include "include/lib/lcdlogo240x160.h"
#include "include/lib/prtlogo.h"
#include "include/lib/beeplib.h"
#include "include/lib/timerlib.h"
#include "include/lib/lcdlib.h"
#include "include/lib/commlib.h"
#include "include/lib/keylib.h"
#include "include/lib/rtclib.h"
#include "include/lib/msrlib.h"
#include "include/lib/sysinfo.h"
#include "include/lib/des_ecb.h"
#include "include/lib/fcs16.h"
#include "include/lib/country.h"
#include "include/lib/hsmodem.h"
#include "include/lib/hdlc.h"
#include "include/lib/iso8583.h"

//#include "include/emv/bitel_tl.h"
#include "include/emv/emv_apdu.h"

#include "include/lib/barcode.h"
#include "include/lib/magtek_msr.h"
#include "include/lib/wifilib.h"
#include "include/lib/key_util.h"
#include "include/lib/utillib.h"

#include "stsdef.h"
#include "main.h"
#include "flash.h"
#include "rtc.h"
#include "key.h"
#include "utils.h"
#include "modem.h"
#include "dialup.h"
#include "ethernet.h"
#include "wifi.h"
#include "cetest.h"
#include "lcdmsg1.h"
#include "lcdmsg4.h"
#include "lcdmsg.h"
#include "app8583.h"
#include "systest.h"
#include "lcdtest.h"
#include "keytest.h"
#include "msrtest.h"
#include "ictest.h"
#include "rfcardtest.h"
#include "memtest.h"
#include "printer.h"
#include "rs232.h"
#include "gsm.h"
#include "foma.h"
#include "hyper_terminal.h"

#include "RICReg.h"
#include "MfRc500.h"
#include "PICCCmdConst.h"
#include "MfErrNo.h"

#include "include/lib/frmwri.h"
#include "include/lib/printf.h"


// usblib.a
// 0: Not connected, 1: Connected
extern int UsbStartFlg;

// Return Value
// 0: OK, else: Open Error
extern int USB_Open(void);

// Return Value
// 0: OK, else: Close Error
extern int USB_Close(void);

// Output
//		stream
//			point of received data string (maximum 64 bytes)
// Return Value
//		length of received data
extern int USB_Read(unsigned char *stream);

// Output
//		stream
//			point of sending data string
//		length
//			length of sending data
// Return Value
//		0: OK, else: Error
extern int USB_Write(unsigned char *stream, int length);

//#endif

/* */
