/*---------------------------------------------------------------------------+
| Author:    RSBON
| File:      PINPAD.C
| Purpose:   PINPAD handler
| Changes:
|
| Date:      Author  Comment:
| ---------  ------  --------
| 26-Feb-02  RSBON   Created.
+---------------------------------------------------------------------------*/
#include "../../BF_SDK/INCLUDE/system.h"
#include "../../BF_SDK/INCLUDE/extern.h"

#define DBG_REPORT
#define TEST_PED_REPORT
#ifdef  TEST_PED_REPORT
    #ifdef  DBG_REPORT
        #define PINprt(x) Rprintf x
        #define DEBUG_PIN(y) DebugBUF y
    #else
        #define PINprt(x)
        #define DEBUG_PIN(y)
    #endif
#else
    #define PINprt(x)
    #define DEBUG_PIN(y)
#endif

#ifndef BTKEY_ENTER
#define BTKEY_ENTER				0x13
#endif

#ifndef ERROR_TONE
#define ERROR_TONE  2
#endif
#ifndef CURBOX_ON
#define CURBOX_ON   2
#endif

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
unsigned char gbPinSTX, gbPinETX;
const char gbPinACK[2]={ACK,0x00};
const char gbPinNAK[2]={NAK,0x00};
const char gbPinEOT[2]={EOT,0x00};
const char T01_RunTst[]="01";
const char T02_Txf_MK[]="02";
const char T04_Chk_MK[]="04";
const char T05_Txf_SN[]="05";
const char T06_Req_SN[]="06";
const char T08_Sel_MK[]="08";
const char T09_ComTst[]="09";
const char T11_PinPad[]="11";
const char T12_SelEng[]="12";
const char T13_BaudRt[]="13";
const char T15_KeyMgr[]="15";
const char T17_SelM_S[]="17"; //BITEL data only 1 'M','S'
const char T70_ReqPin[]="70";
const char T71_TxfPin[]="71";
const char T72_CANCEL[]="72";
const char D90_IPEKSN[]="90";
const char Z02_DspStr[]="Z2";
const char Z07_OffMSG[]="Z7";
const char Z08_GoIDLE[]="Z8";
const char Z42_ReqKey[]="Z42";
const char Z62_EncPIN[]="Z62";
const char Z66_ReqMAC[]="Z66";

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
char gbExtPinPadTaskWork, gbExtPinPadTaskStep, gbExtPinPadFound, gbExtPinPadbaud;
char gbIntPEDchip;
U08 gbPEDmode, gbPEDMKID, gbPEDopen, gbPEDWK[16];
extern int pinWait;
int pinTOut, giNAKcnt;
int giPinTryCnt;
static unsigned char *psbRxD, *psbTMP;
unsigned char RxDBUF[2048], TxDBUF[2048];

extern U16 gSysTimer;

#define PEDAT0  256
#define PED_MODE_MKEY   0
#define PED_MODE_DKEY   1
#define PED_MODE_DUAL   2

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
int closePinPAD( void )
{
    gbExtPinPadTaskWork=0;
    gbExtPinPadTaskStep=0;
    gbPEDopen=0;
    return(0);
}

extern int PinpadDrvTask();
extern int RdPED();
extern void WrPED(char *stream, int size);
extern void InitPinpadDrv(int iLine);
extern int PEDR_COUNT, PEDR_IPOINT, PEDR_MPOINT; 
extern void EncryptWorkingKey(char *pWorkingKey, int iKeyIdx, int iMode);

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
void pinApdu(char *cmd, char *str)
{
    int i, size;
    char *ptr;

    gbExtPinPadTaskWork=1;
    ptr = (char *)&TxDBUF[0];
    if(cmd != NULL) {
        if(cmd[0] <= 0x32) { //only for Diagnostic Command "0x","1x","2x"
            gbPinSTX=(unsigned char)SI;  gbPinETX=(unsigned char)SO;
        }else {
            gbPinSTX=(unsigned char)STX; gbPinETX=(unsigned char)ETX;
        }
        *ptr++ = gbPinSTX;
            while(*cmd != 0x00) *ptr++ = *cmd++;
        if(str != NULL) {
            while(*str != 0x00) *ptr++ = *str++;
        }
        *ptr++ = gbPinETX;

        size = (int)((long)ptr - (long)&TxDBUF[1]);
        i=1;
        gtwLRC=0;
        while((size--) > 0) gtwLRC ^= TxDBUF[i++];
        *ptr++ = gtwLRC;
    }else if(str != NULL) { // All string data
        while(*str != 0x00) *ptr++ = *str++;
    }else return;
    *ptr=0;

    size = (int)((long)ptr - (long)&TxDBUF[0]);

    memset((char *)&RxDBUF[0], 0x00, 128);
    psbRxD = &RxDBUF[0];
    pinWait = TIME_BASE0;

    COM0_POINT_INIT();
	WrPED((char *)&TxDBUF[0], size );
}

/*--------------------------------------------------------------------------*/
// return = 0 -  3 : Pin Entry Bypass,PIN was not entered.
//        = 4 - 12 : Normal PinData
//        < 0      : PinPAD mulfunctioning,PIN pad not present or not working.
//        PinData : gbTmpKeyBuf[2]...
/*--------------------------------------------------------------------------*/
int getExtPinPadData(int tryCnt)
{
    int rts=0, i;

/*	srand(TIME_BASE0);
	for(i = 0; i < 8; i++)
		gbPEDWK[i] = rand() & 0xFF;
*/	memcpy(&gbPEDWK[0], "\x01\x23\x45\x67\x89\xAB\xCD\xEF", 8);
	
    giPinTryCnt=tryCnt;

    gbExtPinPadTaskWork=1;
    gbExtPinPadTaskStep=4;
    if(gbPEDmode == PED_MODE_DKEY) gbExtPinPadTaskStep=6;
    pinTOut = 16000;
    while(1) {
        if(PinpadTask() == 1) {
        	rts = -1;
        	break;
        }
        if(gbExtPinPadTaskStep >= 10) {
            rts=GetPinBlock();
            break;
        }
    }
    closePinPAD();
	//gSysTimer = TIME_BASE0;

    return(rts);
}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
void PED_SelectMKEY(void)
{
    char mk[2],i=1;
/*
;1234567890123445=99011211XXXXXXX00000?*
^^               ^^   ^  ^^   ^       ^^
||_ Card number  ||   |  ||   |_ CVV  ||_ LRC
|_ Start sentinel||   |  ||_ PVV*     |_ End sentinel
Field separator _||   |  |_ PVV key indicator
      Expiration _|   |_ Service code
*/
    while(1) { /* Get Last number of PAN [0-9]*/
        gbPEDMKID = CardNo2[i++];
        if((CardNo2[i] == '=')||(CardNo2[i] == '\0')) break;
    }
    if((gbPEDMKID < '0') || (gbPEDMKID>'9')) gbPEDMKID='0';
    mk[0]=gbPEDMKID;
    mk[1]=0;
    pinApdu((char *)&T08_Sel_MK[0], (char *)&mk[0]);
}

/*--------------------------------------------------------------------------*/
// Before this call...
// gbPEDMKID('0'-'9'), gbPEDWK[16], giPinTryCnt must be initialized...
/*--------------------------------------------------------------------------*/
void Z62AcceptEncryptPIN( unsigned short pinmin, unsigned short pinmax )
{
    int i;
    char *ptr;

    ptr = (char *)&TxDBUF[PEDAT0];
    *ptr++ = '.';                    /* command delimiter, the period '.' */
    for(i=1; i<=19; i++) {
        if((CardNo2[i] == '\0')||(CardNo2[i] == '=')) break;
        *ptr++ = CardNo2[i];         /* card account number (8-19) */
    }
    *ptr++ = FS;

    if(gbPEDmode != PED_MODE_DKEY) {
        memcpy((U08 *)&ptr[16], (U08 *)&gbPEDWK[0], 8);
        i = gbPEDMKID & 0x0F;
		EncryptWorkingKey(&ptr[16], i, DEAmoEncS);
        pfnHex2Asc((U08 *)&ptr[0], (U08 *)&ptr[16], 8); /* Working key (16 unsigned char) */
        ptr += 8*2;
    }

    if(pinmin > 12) pinmin = 12;    /* Minimum acceptable PIN (04-12) */
    sprintf((char *)ptr, "%02d", pinmin);
    ptr += 2;
    if(pinmax > 12) pinmax = 12;    /* Maximum acceptable PIN (04-12) */
    sprintf((char *)ptr, "%02d", pinmax);
    ptr += 2;
    *ptr++ = 'Y';                   /* Y = Null PINs allowed, N = Null PINs not allowed */

    if(giPinTryCnt & 0xFF00) {
        pinmin= 95; //"THANK YOU", //95
        pinmax= 77; //"PLEASE ENTER PIN", //77
    }else if(giPinTryCnt==1) {
        pinmin= 78; //"INCORRECT PIN   ", // 78
        pinmax=132; //"LAST PIN TRY!   ", //132
    }else {
        pinmin= 78; //"INCORRECT PIN   ", // 78
        pinmax= 87; //"TRY AGAIN       ", // 87
        //pinmin= 81; //"PIN OK          ", // 81
    }
    giPinTryCnt &= 0x00FF; // RSBON_EXTPinPAD
    for(i=0; i<16; i++) {
        if(msg_tbl1[gbLangCode][pinmin][i] == '\0') break;
        *ptr++ = msg_tbl1[gbLangCode][pinmin][i]; /* message-1 (0-16) */
    }
    *ptr++ = FS;

    for(i=0; i<16; i++) {
        if(msg_tbl1[gbLangCode][pinmax][i] == '\0') break;
        *ptr++ = msg_tbl1[gbLangCode][pinmax][i]; /* message-2 (0-16) */
    }
    *ptr++ = FS;

    for(i=0; i<16; i++) {           /* processing message displayed after PIN entry */
        if(msg_tbl1[gbLangCode][82][i] == '\0') break;
        *ptr++ = msg_tbl1[gbLangCode][82][i];   /*"PLEASE WAIT     "*/
    }
    *ptr=0x00;

    pinApdu((char *)&Z62_EncPIN[0], (char *)&TxDBUF[PEDAT0]);

    PINprt(("\n.Track2Data(%s)\n", (char *)&CardNo2[1]));
}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
int GetPinBlock( void )
{
    int  i;

    psbTMP=(unsigned char *)&RxDBUF[PEDAT0];
    if(strncmp((char *)&psbTMP[1], (char *)&T71_TxfPin[0], 2)) return(-1);

    if(psbTMP[3]=='.') { // This is Master/Session mode encPIN
        pfnAsc2Hex((U08 *)&psbTMP[64], (U08 *)&psbTMP[9], 16);
        des((U08 *)&psbTMP[64], (U08 *)&gbPEDWK[0], DEAmoDecS);
        PINprt(("Pin_Block")); DEBUG_PIN(((unsigned char *)&psbTMP[64],16));
    }else { //psbTMP[3] must be '0', DUKPT mode encPIN
        // Reply KSN[5-10 bytes] & encPinBlock[8]!
        return(1);
    }
    i=1;
    while(1) {
        if((CardNo2[i] == '=')||(i > 19)) break;
        i++;
    }
    if(i > 2) i -= 2;
    if(i > 12) {
        i -= (12-1);
        memcpy((U08 *)&psbTMP[96], (U08 *)&CardNo2[i], 12); i=12;
    }else {
        memcpy((U08 *)&psbTMP[96], (U08 *)&CardNo2[1], i);
    }
    psbTMP[96+i]=0x00;

    // Convert BCD format, ie right justified with leading zeros...
    fnAsc2Bcd((U08 *)&psbTMP[80], (U08 *)&psbTMP[96], 16);
    for(i=2; i<8; i++) psbTMP[i+64] ^= psbTMP[i+80];
    PINprt(("PinBlockF")); DEBUG_PIN(((unsigned char *)&psbTMP[64], 8));

    i=(int)(psbTMP[64] & 0x0F)+2;
    pfnHex2Asc((U08 *)&gbTmpKeyBuf[0], (U08 *)&psbTMP[64], i);
    gbTmpKeyBuf[i]=0;

    if((i > 14) || (psbTMP[64+7] != 0xFF)) {
        return(-1); // PinPAD Mulfunctioning
    }
    return(i);
}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
int PinpadTask( void )
{
    int i, j, imax, ch;

//    PINprt(("\n.<E>gbExtPinPadTaskStep(%d).\n",gbExtPinPadTaskStep));

    if ( !gbExtPinPadTaskWork ) return 0;
	if(PinpadDrvTask() == 1) return 1;

    switch(gbExtPinPadTaskStep) {
        case 0:
            giNAKcnt = 0;
            pinTOut = 4000;
            pinApdu((char *)&Z08_GoIDLE[0], (char *)&msg_tbl1[gbLangCode][120] ); /*""BITEL PASS""*/
            gbExtPinPadTaskStep=3;
            break;
        case 2:
            pinApdu((char *)&Z07_OffMSG[0], "1"); /* TurnOFF CANCEL REQUESTED */
            gbExtPinPadTaskStep++;
            break;
        case 4:
            PED_SelectMKEY(); /* Last number of PAN [0-9]*/
            gbExtPinPadTaskStep++;
            break;
        case 6:
            pinTOut = 10000;
            Z62AcceptEncryptPIN( 4, 12 );
            gbExtPinPadTaskStep++;
            break;
        case 8:
            gbExtPinPadTaskStep++;
            break;
        case 10:
            if ( GetPinBlock() > 0 ) { //OK!
                pinApdu((char *)&Z08_GoIDLE[0], (char *)&Total_Amount[0] );
                gbExtPinPadTaskStep++;
            } else { //ERROR
                STRDISP( 42 ); //"Communication - failed.",116,117
                giNAKcnt = 0;
                gbExtPinPadTaskWork=0;
//                enqueue_key(BTKEY_ENTER);
            }
            break;
        case 12:
            pinTOut = 4000;
            pinApdu((char *)&T72_CANCEL[0], NULL );
            gbExtPinPadTaskStep++;
            break;
        case 14:
            pinApdu((char *)&Z08_GoIDLE[0], (char *)&msg_tbl1[gbLangCode][120] ); /*""BITEL PASS""*/
            gbExtPinPadTaskStep++;
            break;
        case 16:
            gbExtPinPadTaskWork=0;
            gbExtPinPadTaskStep=0;
            break;
    }
    if((gbExtPinPadTaskStep != 9) && (deltaMS(pinWait, 0) > pinTOut)) {
        BEEP_TONE( 1, ERROR_TONE );
        STRDISP( 41 ); //"Pinpad not found",115
        LCD_BLINKING(CURBOX_ON , 0, 0, 16 );
        PINprt(("\n.<E>gbExtPinPadTaskStep(%d).\n",gbExtPinPadTaskStep));
        gbExtPinPadTaskWork=0;
//        enqueue_key(BTKEY_ENTER);
        return 1;
    }

	imax = PEDR_COUNT;
    for ( i = 0 ; i < imax ; i++ ) {
		ch = RdPED();
        if(((RxDBUF[0]==STX)&&(*(psbRxD-1)==ETX)) || ((RxDBUF[0]==SI)&&(*(psbRxD-1)==SO))) {
            gtwLRC = 0;
            for(j=1; j<(int)((long)psbRxD-(long)&RxDBUF[0]); j++) gtwLRC ^= RxDBUF[j];
            PINprt(("\n.LRC[%02X,%02x]\n", ch, gtwLRC));
            if (gtwLRC == ch) {
                psbTMP=(unsigned char *)&RxDBUF[PEDAT0];
                memcpy(psbTMP, &RxDBUF[0], j); psbTMP[j+1]=0;
                pinApdu(NULL, (char *)&gbPinACK[0]);
                giNAKcnt = 0;
                gbExtPinPadTaskStep++;
            } else {
                if (giNAKcnt > 3 ) {
                    pinApdu(NULL, (char *)&gbPinEOT[0]);
                    STRDISP( 42 ); /*"Communication - failed again...",116,117 */
                    giNAKcnt = 0;
                    gbExtPinPadTaskWork=0;
//                    enqueue_key(BTKEY_ENTER);
                } else {
                    pinApdu(NULL, (char *)&gbPinNAK[0]);
                    gbExtPinPadTaskStep--;  /* resend command */
                    giNAKcnt++;
                }
            }
        } else {
            switch( ch ) {
                case ACK: PINprt(("(ACK)\n"));
                    giNAKcnt = 0;
                    gbExtPinPadTaskStep++;
                    break;
                case EOT: PINprt(("(EOT)\n"));
                    giNAKcnt = 0;
                    if(gbExtPinPadTaskStep >= 4) {
                        gbExtPinPadTaskWork=0;
                    }else {
                        gbExtPinPadTaskStep++; /* Version received */
                    }
                    break;
                case NAK: PINprt(("(NAK)\n"));
                    if (giNAKcnt > 3 ) {
                        STRDISP( 42 ); /*"Communication - failed again...",116,117 */
                        giNAKcnt = 0;
                        gbExtPinPadTaskWork=0;
//                        enqueue_key(BTKEY_ENTER);
                        break;
                    }
                    gbExtPinPadTaskStep--;  /* resend command */
                    giNAKcnt++;
                    break;
                default:
                    if (gbExtPinPadTaskStep < 1) break;
                    *psbRxD++ = ch;
                    break;
            }
        }
    }
	return 0;
}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
int openPinPAD( void )
{
    gbExtPinPadFound = 0x01; //[BITEL_PASS]
    gbExtPinPadTaskWork=0;
    gbExtPinPadTaskStep=0;
    gbPEDopen=1;

    return(0);
}

/*--------------------------------------------------------------------------*/
// Before this call...
// gbPinBlock[8],CardNo2[],gbPEDWK[16] must be initialized...
/*--------------------------------------------------------------------------*/
int EncryptPIN(void)
{
    int i=1,id=0;

    while(1) {
        if((CardNo2[i] == '=')||(i > 19)) break;
        i++;
    }
    i--;
    id=CardNo2[i]&0x0F;
    if(id>9) id=0;

    psbTMP=(unsigned char *)&RxDBUF[PEDAT0];
    if(i > 1) i -= 1;
    if(i > 12) {
        i -= (12-1);
        memcpy((U08 *)&psbTMP[96], (U08 *)&CardNo2[i], 12); i=12;
    }else {
        memcpy((U08 *)&psbTMP[96], (U08 *)&CardNo2[1], i);
    }

    psbTMP[96+i]=0x00;
    // Convert BCD format, ie right justified with leading zeros...
    fnAsc2Bcd((U08 *)&psbTMP[80], (U08 *)&psbTMP[96], 16);
        PINprt(("PANwoCBit")); DEBUG_PIN(((unsigned char *)&psbTMP[80], 8));
    for(i=2; i<8; i++) gbPinBlock[i] ^= psbTMP[i+80];
        PINprt(("Pin_Block")); DEBUG_PIN(((unsigned char *)&gbPinBlock, 8));

#ifdef LANG_FARSI
//    des((U08 *)&gbPinBlock[0], (U08 *)&pgsSysInfo->deakey[0], DEAmoEncT);
		EncryptWorkingKey((U08 *)&gbPinBlock[0], 0, DEAmoEncT);
//        PINprt(("Pin_Block")); DEBUG_PIN(((unsigned char *)&gbPinBlock, 8));
#else
    des((U08 *)&gbPinBlock[0], (U08 *)&gbPEDWK[0], DEAmoEncS);
        PINprt(("Pin_Block")); DEBUG_PIN(((unsigned char *)&gbPinBlock, 8));
#endif
    return(0);
}

