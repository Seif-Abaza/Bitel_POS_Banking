

//#ifndef EXTERN_H
//#define EXTERN_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************
						Bitel Header Files
********************************************************************/
//#include "../BF_SDK/include/drv/driver.h"
//#include "../BF_SDK/include/btbios.h"
#include "../BF_SDK/include/driver.h"

#include "../BF_SDK/include/lib/printf.h"

#include "../BF_SDK/include/lib/keydef.h"
#include "../BF_SDK/include/lib/beeplib.h"
#include "../BF_SDK/include/lib/beeplib1.h"
#include "../BF_SDK/include/lib/timerlib.h"
#include "../BF_SDK/include/lib/timerlib_1.h"
#include "../BF_SDK/include/lib/lcdlib.h"
#include "../BF_SDK/include/lib/commlib.h"
#include "../BF_SDK/include/lib/commlib_1.h"
#include "../BF_SDK/include/lib/keylib.h"
#include "../BF_SDK/include/lib/keylib_1.h"
#include "../BF_SDK/include/lib/rtclib.h"
//#include "../BF_SDK/include/lib/rtclib_1.h"
#include "../BF_SDK/include/lib/msrlib.h"
#include "../BF_SDK/include/lib/sysinfo.h"
//#include "../BF_SDK/include/lib/bitel_tl.h"
#include "../BF_SDK/include/lib/des_ecb.h"
#include "../BF_SDK/include/lib/hdlcfcs.h"
#include "../BF_SDK/include/lib/hdlc.h"
#include "../BF_SDK/include/lib/hsmodem.h"
#include "../BF_SDK/include/lib/iso8583.h"
#include "../BF_SDK/include/lib/utillib.h"

#include "../BF_SDK/include/lib/frmwri.h"


/******************************************************************
						MultiTasking
********************************************************************/
extern void  TASK_START(int, void (*task)(void));
extern void  TASK_STOP(char);
extern void  SET_MULTITASKING_USAGE(char);
extern void  SET_TASK_PRIORITY_MODE(char);
extern void  SET_TASK_CHANGE_TIME(char);
extern void  SET_TASK_STACK_SIZE(char);

#ifdef __cplusplus
}
#endif




//#endif

/* */
