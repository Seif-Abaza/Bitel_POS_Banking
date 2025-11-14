/*!
* \file    $Source: GPRS.c $
* \date    $Date: 2014/7/10  $
* \version $Revision: 1.14 $
* \author  $Author: Mohamed $
*/
/*!
 * *  Implementation of GPRS Functions and SMS
*
* (c) Copyright Bitel Emea 2013 unpublished work.
* This computer program includes confidential, proprietary
* information and is a trade secret of Bitel. All use,
* disclosure and/or reproduction is prohibited unless
* authorized in writing.
* All rights reserved.
*/
/*@{*/

#include "driver.h"
#include "../INCLUDE/BTEMEA_LIB/KEYPAD.h"

#define USER_DEBUG 1



/*
 * ***************************************************************************
 * 			Macro
 * **************************************************************************
 */
#define GPRS_LIB_VERSION 14

#define WAVECOM   1
#define CINTERION 2

// define GPRS module type
#define WAVECOM_Q24PLUS								0
#define WAVECOM_Q2686								1
#define CINTERION_BG2_W								2
#define CINTERION_BG2_E								3
#define SIERRA_WISMO218								4

#define MODEM_NOT_DETECTED -2
#define GPRS_SUCCESS     0x00
#define GPRS_NOTFOUND    0x02
#define GPRS_PORTFAILED  0x03
#define GPRS_NODATARECV  0x04
#define GPRS_NORESP      0x05
#define GPRS_ERROR       0x06
#define GPRS_CMEERROR    0x07
#define GPRS_NOANSWER    0x08
#define GPRS_CONNECTED   0x09
#define GPRS_NOTCONECTED 0x0A
#define GPRS_ERRNETWORK  0x0B
#define GPRS_SOCKET      0x0C
#define Operation_NOT_SUPPORTED 0x10 	//added by heba
#define Operation_SUPPORTED     0x12	//added by heba
#define GPRS_CANCELED     0x0D


#define	PACK		0
#define	UNPACK		1
#define GPRS_TIME_OUT 30000

#define false 0
#define true 1

/*
 * ***************************************************************************
 * 	Global variables
 * **************************************************************************
 */
typedef unsigned char UBYTE;

unsigned char ucGPRSModemType;

char HWINFO[16];

const int BaudValue[8] = {
	1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200
};

unsigned int uiSimSlotNum;	//22/1/2014

unsigned short usWaittime;

ulong ulSleepCnt;
#ifndef SLEEP_M
#define SLEEP_M(dwMs)							\
{										\
	  for (ulSleepCnt = 0; ulSleepCnt< (dwMs/10);ulSleepCnt++){ \
		if (WaitKeyPress10MS(1) == VK_CANCEL)                      \
		  return GPRS_CANCELED;                                   \
	  }															 \
}
#endif

int GSMModemProcess, SIMindex;
int Start_GPRS_bearer_Retry, Create_Retry;
char *gsmtxptr, *gsmrxptr, gsmTxStream[2048], gsmRxStream[2048];

/*
 * ***************************************************************************
 * 					Function declaration
 * **************************************************************************
 */
int GPRS_SendData(unsigned char *ucpData,int iLen);
int GPRS_ReceiveData(unsigned char *ucpRecvBuffer,
					 int *ipLen,
					 unsigned short usTimeOut);
int GPRS_initialize(char *cpAPN,
					char *cpUID, 
					char *cpPASS,
					char *cpPinCode);
int GPRS_DisConnect();
int GPRS_intializePort(void);
unsigned char GPRS_Connect(char *cpIP,char *cpPORT,unsigned short usTimeOut);
void GPRS_ConfigureRSPort(int mode);
unsigned char GPRS_IsConnected();
unsigned char GPRS_SendCommand(unsigned char *ucpCommand,
							   unsigned char *ucpResp,
							   int iRetry);
							   
unsigned char GPRS_Pause_SendCommand(unsigned char *ucpCommand,
							   unsigned char *ucpResp,
							   int iRetry);
int WriteToPort(char *stream);
//extern void SET_TX_TIME_INTERVAL( int COMx, int Time_interval  ); //heba 22/1/2014
void WriteToCom( int channel, char *stream, int size );
extern void CONTROL_POWER_SAVING_STATUS(char command , char data);
int ReadFromCom( int channel );
char GPRS_SimSelect( int iSlot );
int GPRS_getVersion(void );
int GPRS_getSignalStrength();
void GPRSWriteCom( int channel, char *stream, int size );
int GPRSReadCom( int channel );
void GPRSCom1_Init( void );
int GPRS_getSIMID(char *pcSIMID);
int GPRS_getProductIMEI(char *pcPIMEI);


unsigned char GPRS_isSimPresent();
unsigned char GPRS_isSimPresentWAVECOM();
unsigned char GPRS_isSimPresentCINTERION();

unsigned char GPRS_Ping(char *cpHost,
						int iRepeat,
						int iInterval,
						unsigned short usPackTimeOut,
						unsigned short usOpTimeOut);
						
void GPRS_ShowSignal(int iSigStrength);

unsigned char GPRS_SendUSSD(char *cpString,char *cpRetString,unsigned short usOpTimeOut);

int GPRS_initializeWAVECOM(char *cpAPN,char *cpUID, char *cpPASS,char *cpPinCode);
int GPRS_initializeCINTERION(char *cpAPN,char *cpUID, char *cpPASS,char *cpPinCode);

unsigned char GPRS_ConnectWAVECOMM(char *cpIP,char *cpPORT,unsigned short usTimeOut);
unsigned char GPRS_ConnectCINTERION(char *cpIP,char *cpPORT,unsigned short usTimeOut);

unsigned char GPRS_PingCINTERION(char *cpHost,
								 int iRepeat,
								 int iInterval,
								 unsigned short usPackTimeOut,
								 unsigned short usOpTimeOut);
								 
unsigned char GPRS_PingWAVECOM(char *cpHost,
							   int iRepeat,
							   int iInterval,
							   unsigned short usPackTimeOut,
							   unsigned short usOpTimeOut);

int GPRS_DisConnectCINTERION();
int GPRS_DisConnectWAVECOM();

int GPRS_ReceiveDataCINTERION(unsigned char *ucpRecvBuffer,
							  int *ipLen,
							  unsigned short usTimeOut);
							  
int GPRS_ReceiveDataWAVECOM(unsigned char *ucpRecvBuffer,
							int *ipLen,
							unsigned short usTimeOut);

int GPRS_ReceiveDataLen(unsigned char *ucpRecvBuffer,
					 int *ipLen,
					 unsigned short usTimeOut,int len);
int GPRS_ReceiveDataCINTERIONLen(unsigned char *ucpRecvBuffer,
							  int *ipLen,
							  unsigned short usTimeOut,int len);

int GPRS_ReceiveDataWAVECOMLen(unsigned char *ucpRecvBuffer,
							int *ipLen,
							unsigned short usTimeOut,int len);
int GPRS_sendSMSWAVECOM(char *pcTelephoneNo,
						char *pcMessage,
						unsigned short usTimeOut);
						
//int GPRS_sendSMSCINTERION(char *pcTelephoneNo,
//						  char *pcMessage,
//						  unsigned short usTimeOut);
//
//int GPRS_initializeSMSStorage();	//Heba 22/1/2014

extern void SYSINFO_GetHWInformation(char hw_info[16]);


/*
 * ***************************************************************************
 * 			Stucts declaration
 * **************************************************************************
 */

typedef struct {
	int		errorno;
	char	message[70];
} CME_T;

CME_T const CME[] = {
	{   3, "Operation not allowed" },
	{   4, "Operation not supported" },
	{   5, "PH-SIM PIN required" },
	{  10, "SIM not inserted" },
	{  11, "SIM PIN required" },
	{  12, "SIM PUK required" },
	{  13, "SIM failure" },
	{  16, "Incorrect password" },
	{  17, "SIM PIN2 required" },
	{  18, "SIM PUK2 required" },
	{  20, "Memory full" },
	{  21, "Invalid index" },
	{  22, "Not found" },
	{  24, "Text string too long" },
	{  26, "Dial string too long" },
	{  30, "No network service" },
	{  32, "Network not allowed" },
	{  40, "Network personalization PIN required" },
	{ 301, "SMS service of ME reserved" },
	{ 302, "Operation not allowed" },
	{ 303, "Operation not supported" },
	{ 304, "Invalid PDU mode parameter" },
	{ 305, "Invalid text mode parameter" },
	{ 310, "SIM not inserted" },
	{ 311, "SIM PIN required" },
	{ 312, "PH-SIM PIN required" },
	{ 313, "SIM failure" },
	{ 316, "SIM PUK required" },
	{ 317, "SIM PIN2 required" },
	{ 318, "SIM PUK2 required" },
	{ 321, "Invalid memory index" },
	{ 322, "SIM memory full" },
	{ 330, "SC address unknown" },
	{ 340, "no +CNMA acknowledgement expected" },
/* Specific error result codes */
	{ 500, "unknown error" },
	{ 512, "MM establishment failure" },
	{ 513, "Lower layer failure (for SMS)" },
	{ 514, "CP error (for SMS)" },
	{ 515, "Please wait" },
	{ 517, "SIM Toolkit facility not supported" },
	{ 518, "SIM Toolkit indication not received" },
	{ 519, "Reset the product to activate" },
	{ 520, "Autometic abort" },
	{ 526, "PIN deactivation forbidden" },
	{ 527, "Please wait, RR or MM is busy." },
	{ 528, "Location update failure" },
	{ 529, "PLMN selection failure" },
	{ 531, "SMS not send" },
	{ 800, "Invalid option" },
	{ 801, "Invalid option value" },
	{ 802, "Not enough memory" },
	{ 803, "Operation not allowed in the current WIP stack state" },
	{ 804, "Device already open" },
	{ 805, "Network interface not available" },
	{ 806, "Operation not allowed on the considered bearer" },
	{ 807, "Bearer connection failure : line busy" },
	{ 808, "Bearer connection failure : no answer" },
	{ 809, "Bearer connection failure : no carrier" },
	{ 810, "Bearer connection failure : no sim card present" },
	{ 811, "Bearer connection failure : sim not ready (no pin code entered,...)" },
	{ 812, "Beared connection failure : GPRS network failure" },
	{ 813, "Beared connection failure : PPP LCP negotiation failed" },
	{ 814, "Beared connection failure : PPP authentication failed" },
	{ 815, "Beared connection failure : PPP IPCP negotiation failed" },
	{ 816, "Beared connection failure : PPP peer terminates session" },
	{ 817, "Beared connection failure : PPP peer does not answer to echo request" },
	{ 818, "Incoming call refused" },
	{ 819, "Error on Ping channel" },
	{ 820, "Error writing configuration in FLASH memory" },
	{ 821, "Error reading configuration in FLASH memory" },
	{ 830, "Bad index" },
	{ 831, "Bad state" },
	{ 832, "Bad port number" },
	{ 833, "Bad port state" },
	{ 834, "Not implemented" },
	{ 835, "Option not supported" },
	{ 836, "Memory" },
	{ 837, "Bad proto" },
	{ 838, "No more free socket" },
	{ 850, "Unknown reason" },
	{ 851, "Bad state" },
	{  -1, "" },
};



/*
 * ***************************************************************************
 * 			Function implementation
 * **************************************************************************
 */


/*
 * \ brief
 * \param
 * \return
 */
int GSMCommandRead(int msdelay)
{
	UBYTE ch;

	gsmrxptr = gsmRxStream;
	TIME_BASE10 = 0x8000 | msdelay;
	while( 1 ) 
	{
		if (!(TIME_BASE10 & 0x8000)) 
		{
			return (-1);	/* timeout error */
		}
		if (!COM1R_COUNT) 
		{
			continue;
		}
		ch = GPRSReadCom(COM1);
	    //Rprintf("%c", ch);
		*gsmrxptr++ = ch;
		if ((*(gsmrxptr-2) == '\r') && (*(gsmrxptr-1) == '\n')) 
		{
			if (!strncmp(gsmrxptr-4, "OK", 2))
			{
				 return (1);
			}
			if (!strncmp(gsmrxptr-7, "ERROR", 5)) 
			{
				return (0);
			}
		}//end of if condition
	}//end of while(1)
}
//TODO
/*
 * \ brief
 * \param
 * \return
 */
void GSMCommandWrite(char *stream)
{
	int len;

	len = strlen(stream);
	GPRSWriteCom(COM1, stream, len);
	gsmrxptr = gsmRxStream;
}
//TODO
/*
 * \ brief
 * \param
 * \return
 */
void GSMModemOpen(void)
{
	int speed, delayus;

	SIMindex = 0;

	if (!SIMindex)
		SIM_SELECT(0);
	else
		SIM_SELECT(1);

	CONTROL_POWER_SAVING_STATUS(P_RF_MODULE, ON);
	CONTROL_POWER_SAVING_STATUS(P_EXT_SERIAL, ON);

//	MODEM_SEL();
	MODEM_RESET();
	/* 38400 BPS 8Bit No Parity */
	RS232_SETUP(COM1, BPS115200, 0,1);
	RS232_ENABLE(COM1, ENABLE, RX_ENABLE);

	Wait1MS(2000);

	GPRSCom1_Init();

	memset(gsmRxStream, '\0', sizeof(gsmRxStream));
	gsmrxptr = gsmRxStream;

}
//TODO
/*
 * \ brief
 * \param
 * \return
 */

void GSMModemClose(void)
{
	//Disconnect existing connection
	GSMCommandWrite("ATH\r");
	GSMCommandRead(100);
	//TODO
	GSMCommandWrite("AT+CPOF\r");
	GSMCommandRead(100);

	CONTROL_POWER_SAVING_STATUS(P_EXT_SERIAL, OFF);
	CONTROL_POWER_SAVING_STATUS(P_RF_MODULE, OFF);
	GSMModemProcess = 0;
}
//TODO
/*
 * \ brief
 * \param
 * \return
 */
int GetCMEError(char *stream)
{
	int i, eno;

	eno = atoi(stream);
	for ( i = 0 ; ; i++ )
	 {
		if (CME[i].errorno == -1) 
		{
			break;
		}
		if (eno != CME[i].errorno)
		{ 
			continue;
		}
		//STRDISP(34);
		//Lprintf(2, 0, (char *)CME[i].message);
		return (eno);
	}
	return (-1);
}
/*
 * \ brief
 * \param
 * \return
 */
void gsmResultMessage(char *msg)
{
	//Lprintf(6, 0, "                      ");
	//Lprintf(6, 0, msg);
}
/*
 * \ brief
 * \param
 * \return
 */
void gsmDebugMessage(char *msg)
{
	//Lprintf(1, 0, "                      ");
	//Lprint(1, 0, msg);
	//UTIL_WaitKey();
	//SLEEP_M(200);
}

/*
 * \ brief Parent function to connect to GPRS network
 * \param [in]cpAPN      :  pointer to character string of network APN
 * 		  [in]cpUID      :  pointer to character string of network user id
 * 		  [in]cpPASS     :  pointer to character string of network password
 *        [in]cpPinCode  :  pointer to character string of PIN Code of SIM card (in case of a SIM protected by a PIN Code)
 *                         otherwise, put a dummy value
 * \return
 */
int GPRS_initialize(char *cpAPN,char *cpUID, char *cpPASS,char *cpPinCode)
{
	//check modem type
	if (ucGPRSModemType==WAVECOM)
	{
		//old modem
		return GPRS_initializeWAVECOM(cpAPN,cpUID, cpPASS,cpPinCode);
	}
	else if (ucGPRSModemType==CINTERION)
	{
		//new modem

		return GPRS_initializeCINTERION(cpAPN,cpUID, cpPASS,cpPinCode);
	}
}
/*
 * \ brief
 * \param
 * \return
 */
int GPRS_initializeWAVECOM(char *cpAPN,char *cpUID, char *cpPASS,char *cpPinCode)
{
	char temp[30];
	int i, imax, j, k, no, size;
	int tmp_COM1R_COUNT=0;
	UBYTE ch, tmpbuf[32];
	GSMModemProcess=-3;

	//GSMModemOpen();
	GSMModemProcess++;

	while(1)
	{

			switch(GSMModemProcess)
			{
				case -2:
					gsmDebugMessage("PROCESS 1");
					GSMCommandWrite("AT+CFUN=1\r");
					GSMModemProcess++;
					break;
				case 0:
					gsmDebugMessage("PROCESS 1");
					GSMCommandWrite("ATE1\r");
					GSMModemProcess++;
					break;
				case 2:
					gsmDebugMessage("PROCESS 2");
					GSMCommandWrite("AT+WBCM=0,0\r");
					GSMModemProcess++;
					break;
				case 4:	/* Enables the use of result code */
					gsmDebugMessage("PROCESS 4");
					GSMCommandWrite("AT+CMEE=1\r");
					GSMModemProcess++;
					GSMModemProcess=GSMModemProcess+2;
					break;
				case 6:
					gsmDebugMessage("PROCESS 6");
					//GSMCommandWrite("AT+IFC=2,2\r");
					GSMModemProcess++;
					break;
				case 8:
					gsmDebugMessage("PROCESS 8");
					GSMCommandWrite("AT+CREG=0\r");
					GSMModemProcess++;
					GSMModemProcess=GSMModemProcess+4;
					break;
				case 10:
					gsmDebugMessage("PROCESS 12");
					GSMCommandWrite("AT+CPIN?\r");
					GSMModemProcess++;
					break;
				case 12:
					gsmDebugMessage("PROCESS 14");
					sprintf(gsmTxStream, "AT+CPIN=%s\r", cpPinCode);//R_TermSys.SIMPin[SIMindex]);
					GSMCommandWrite(gsmTxStream);
					GSMModemProcess++;
					break;
				case 14:	/* GPRS atteched */
					gsmResultMessage("ATTECHING NETWORK");
					gsmDebugMessage("PROCESS 16");
					GSMCommandWrite("AT+WOPEN=1\r");
					GSMModemProcess++;
					break;
				case 16:
					gsmDebugMessage("PROCESS 18");
					GSMCommandWrite("AT+WIPCFG=1\r");
					GSMModemProcess++;
					break;
				case 18:
					gsmDebugMessage("PROCESS 18");
					GSMCommandWrite("AT+WIPCFG=2,12,10\r");
					GSMModemProcess++;
					break;
				case 20:
					gsmDebugMessage("PROCESS 20");
					GSMCommandWrite("AT+WIPBR=1,6\r");	// OPEN GPRS bearer
					GSMModemProcess++;
					break;
				case 22:
					gsmDebugMessage("PROCESS 22");
					sprintf(gsmTxStream, "AT+WIPBR=2,6,11,\"%s\"\r", cpAPN);
				//                                           ^
				//                                           |------ GPRS host APN name
					GSMCommandWrite(gsmTxStream);	// Set APN of GPRS bearer
					GSMModemProcess++;
					break;
				case 24:
					gsmDebugMessage("PROCESS 24");
					if (strlen(cpUID)==1)
						sprintf(gsmTxStream, "AT+WIPBR=2,6,0,\"\"\r");
					else
						sprintf(gsmTxStream, "AT+WIPBR=2,6,0,\"%s\"\r", cpUID);
					GSMCommandWrite(gsmTxStream);	// Set user name of PPP login
					GSMModemProcess++;
					break;
				case 26:
					gsmDebugMessage("PROCESS 26");
					if (strlen(cpPASS)==1)
						sprintf(gsmTxStream, "AT+WIPBR=2,6,1,\"\"\r");
					else
						sprintf(gsmTxStream, "AT+WIPBR=2,6,1,\"%s\"\r", cpPASS);
					GSMCommandWrite(gsmTxStream);	// Set password for PPP login
					GSMModemProcess++;
					Start_GPRS_bearer_Retry = 30;
					Create_Retry = 10;
					break;
				case 28:
					gsmDebugMessage("PROCESS 28");
					GSMCommandWrite("AT+WIPBR=4,6,0\r");	// Start GPRS bearer
					GSMModemProcess++;
					TIME_BASE10 = 0x8000 | 6000;		// 60 SEC
					break;
				case 29:
					gsmDebugMessage("PROCESS 29");
					if (!(TIME_BASE10 & 0x8000))
					{
						Start_GPRS_bearer_Retry--;
						if (!Start_GPRS_bearer_Retry)
						{
							GSMModemProcess = 999;
						}
						else
						{
							GSMModemProcess = 28;
						}
					}
					break;
				case 30:
					return GPRS_SUCCESS;
					break;
				}

			usWaittime =Get1MSTimer();
			do{
		      if ((KEYSCAN() & 0xFF) == VK_CANCEL) return GPRS_CANCELED;
			}while((COM1R_COUNT==0) && (delta1MS(usWaittime)<GPRS_TIME_OUT));
			do{
				tmp_COM1R_COUNT=COM1R_COUNT;
				SLEEP_M(100);
			}while(tmp_COM1R_COUNT!=COM1R_COUNT);

			if (!COM1R_COUNT)
			{
				return GPRS_NORESP;
				continue;
			}

			for ( i = 0 ; i < COM1R_COUNT ; i++ )
			{
				ch = GPRSReadCom(COM1);
				if (gsmrxptr - &gsmRxStream[0] > 2048) 
				{
					gsmrxptr = gsmRxStream;
				}
				*gsmrxptr++ = ch;
#ifdef USER_DEBUG
				Rprintf("%c", ch);
#endif
				size = gsmrxptr - &gsmRxStream[0];
				if ((size >= 2) && (*(gsmrxptr-2) == '\r') && (*(gsmrxptr-1) == '\n'))
				{
					memset(tmpbuf, 0, sizeof(tmpbuf));
					memcpy(tmpbuf, gsmRxStream, size - 2);
			    	if (!strncmp(gsmrxptr-4, "OK", 2))
			        {
						gsmResultMessage("OK");
	    				GSMModemProcess++;
	    				//if (GSMModemProcess == 10) {
	    				//	GSMModemProcess = 12;
	    				//}

	    			}
			    	else if (!strncmp(gsmrxptr-7, "ERROR", 5))
			    	{
						gsmResultMessage("ERROR");
						return GPRS_ERROR;
	    				GSMModemProcess = 999;
					}
			    	else if (!strncmp(gsmRxStream, "+CME ERROR: ", 12) || 
			    	         !strncmp(gsmRxStream, "+CMS ERROR: ", 12))
			    	{
			    		gsmDebugMessage(gsmRxStream);
			    		return GPRS_CMEERROR;

						size = gsmrxptr - &gsmRxStream[12] - 2;
						memset(gsmTxStream, '\0', 41);
						strncpy(gsmTxStream, &gsmRxStream[12], size);
						gsmResultMessage(gsmTxStream);
						no = GetCMEError(gsmTxStream);
						switch( no )
						 {
							case 515:
							case 527:
	    						GSMModemProcess--;
	    						break;
							case 3:
	//    						GSMModemProcess = 24;
	    						break;
	    					case 803:	// already GPRS opened
	    						GSMModemProcess = 30;
	    						break;
							case 812:
								TIME_BASE10 = 0x8000 | 100;		// 1 SEC
								break;
							case 831:
								if(GSMModemProcess == 34)
									GSMModemProcess = 33;								// 34
								break;
							case 10:
			    				GSMModemProcess = 999;
	    						break;
	    					default:
	//		    				GSMModemProcess = 999;
	    						break;
	    				}
	    			}
			    	else if (!strncmp(gsmRxStream, "+CREG: ", 7))
	    			{
						size = gsmrxptr - &gsmRxStream[7] - 2;
						memset(gsmTxStream, '\0', 5);
						strncpy(gsmTxStream, &gsmRxStream[7], size);
						gsmResultMessage(gsmTxStream);
						no = atoi(gsmTxStream);
						switch( no ) {
	    					case 0:	/* not registered, ME is not currently searching for a new operator */
				    			GSMModemProcess = 12;
	    						break;
	    					case 1:	/* registered, home network. */
	//			    			GSMModemProcess = 12;
	    						break;
		    				case 2:	/* not registered, ME currently searching for a new operator to register to */
				    			GSMModemProcess = 12;
	    						break;
	    					case 5:	/* registered, roaming */
				    			GSMModemProcess = 12;
	    						break;
	    					case 3:	/* registration denird. */
	    					case 4:	/* unknown */
	    						break;
	    				}
					}
			    	else if (!strncmp(gsmrxptr-12, "NO CARRIER", 10))
			    	{
			    		return GPRS_NOANSWER;
						gsmResultMessage("NO CARRIER");
		    			GSMModemProcess = 999;
	    			}
			    	else if (!strncmp(gsmrxptr-11, "NO ANSWER", 9))
			    	{
			    		return GPRS_NOANSWER;
						gsmResultMessage("NO ANSWER");
		    			GSMModemProcess = 999;
		    		}
			    	else if (!strncmp(gsmrxptr-6, "BUSY", 4))
			    	{
			    		return GPRS_NOANSWER;
						gsmResultMessage("BUSY");
		    			GSMModemProcess = 999;
					}
			    	else if (!strncmp(gsmrxptr-14, "+CPIN: READY", 12))
			    	{
						GSMModemProcess = 14;
					}
			    	else if (!strncmp(gsmrxptr-16, "+CPIN: SIM PIN", 14))
			    	{
						GSMModemProcess++;
		    		}
			    	else if (!strncmp(gsmRxStream, "CONNECT", 7))
			    	{
						gsmResultMessage("CONNECT");
						GSMModemProcess++;
					}
			    	else if (!strncmp((char *)gsmRxStream, "+WIPREADY:", 10))
			    	{
						//STRDISP(30);
			    		return GPRS_NOANSWER;
						Lprintf(1, 0, "+WIPREADY: 2,1");
						GSMModemProcess++;
					}
					gsmrxptr = gsmRxStream;
				}

			}

	}

	return GPRS_SUCCESS;
}
/*
 * \ brief Send data to the host
 * \param [in] ucpData- data to send
 *        [in] iLen-Packet length
 * \return GPRS_SUCCESS
 */
int GPRS_SendData(unsigned char *ucpData,int iLen)
{
	
	int i,j;
	unsigned char uctemp[iLen*2];
	unsigned long usWaittime;
	ICON_DISPLAY( 13);

	j=0; //stream size
	
    //if Old modem
	if (ucGPRSModemType==WAVECOM)
	{
		//TODO
		for (i=0;i<iLen;i++)
		{
			if (ucpData[i]==3) 
			{
				uctemp[j]=16;
				j++;
			}
			if (ucpData[i]==16) 
			{
				uctemp[j]=16;
				j++;
			}
			uctemp[j]=*(ucpData+i);
			j++;
		}
	}
	else if (ucGPRSModemType==CINTERION)// new modem
	{
		Rprintf("iLen:%d \n",iLen);

		for (i=0;i<iLen;i++)
		{
			uctemp[j]=*(ucpData+i);
			Rprintf("%c",uctemp[j]);
//			Rprintf("%c",uctemp[j]);

//			Rprintf("i= %d",i);
			j++;
		}
	}
	
	GPRSWriteCom(COM1, (char *)uctemp, iLen);

	ICON_DISPLAY( 12);

	return GPRS_SUCCESS;
}

/*
 * \ brief Parent function to receive data from the host server for specific time.
 */
int GPRS_ReceiveData(unsigned char *ucpRecvBuffer,int *ipLen,unsigned short usTimeOut)
{
	//check modem type
	if (ucGPRSModemType==WAVECOM)
	{
		// old modem
		return GPRS_ReceiveDataWAVECOM(ucpRecvBuffer,ipLen,usTimeOut);
	}
	else if (ucGPRSModemType==CINTERION)
	{
		//new modem
		return GPRS_ReceiveDataCINTERION(ucpRecvBuffer,ipLen,usTimeOut);
	}
}
/*
 * \brief-  wait to receive data from the host server for specific time, 
 * 			if no data received for usTimeOut, 
 *          the function will timeout and returns error  
 * 
 * \param [in/out] ucpRecvBuffer:  Buffer to store the received response 
 * 		  [in/Out] ipLen        : pointer to the length of received data 
 * 		  [in]     usTimeOut    : time out for waiting to receive data in mille-seconds
 * 
 * \return 	GPRS_SUCCESS            : received data  successfully
 * 			GPRS_NODATARECV: time out and no data received from host 
 * 
 */
int GPRS_ReceiveDataCINTERION(unsigned char *ucpRecvBuffer,
							  int *ipLen,
							  unsigned short usTimeOut)
{
	int i, imax, j;
	int tmp_COM1R_COUNT=0;
	unsigned long usWaittime;
	UBYTE ch;
	gsmrxptr = gsmRxStream;
	usWaittime =Get1MSTimer();

	//TODO
	do{
		
	}while((COM1R_COUNT==0) && (delta1MS(usWaittime)<usTimeOut));

	if (COM1R_COUNT==0) Rprintf("\nNothing Recv\n");

	do{
		tmp_COM1R_COUNT=COM1R_COUNT;
		Wait1MS(50);
	}while(tmp_COM1R_COUNT!=COM1R_COUNT);

	if (!COM1R_COUNT){
		*ipLen=0;
		return GPRS_NODATARECV;
	}

	ICON_DISPLAY( 11);

	imax=COM1R_COUNT;
	Rprintf("\nimax = %d\n", imax);
	for ( i = 0 ; i < imax ; i++ )
	{
		ucpRecvBuffer[i]=GPRSReadCom(COM1);

#ifdef USER_DEBUG
//		Rprintf("%c", ucpRecvBuffer[i]);
#endif
	}
	
	*ipLen=i;

	ICON_DISPLAY( 10);


	return GPRS_SUCCESS;
}
/*
 * \ brief
 * \param
 * \return
 */
int GPRS_ReceiveDataWAVECOM(unsigned char *ucpRecvBuffer,
							int *ipLen,
							unsigned short usTimeOut)
{
	int i, imax, j;
	int tmp_COM1R_COUNT=0;
	unsigned long usWaittime;
	UBYTE ch;
	unsigned char IsDLE=0;

	gsmrxptr = gsmRxStream;
	usWaittime =Get1MSTimer();
	do{
	}while((COM1R_COUNT==0) && (delta1MS(usWaittime)<usTimeOut));

	do{
		tmp_COM1R_COUNT=COM1R_COUNT;
		Wait1MS(300);
	}while(tmp_COM1R_COUNT!=COM1R_COUNT);


	if (!COM1R_COUNT)
	{
		*ipLen=0;
		return GPRS_NODATARECV;
	}

	ICON_DISPLAY( 11);

	imax=COM1R_COUNT;
	j=0;
		for ( i = 0 ; i < imax ; i++ )
		{
			ch = GPRSReadCom(COM1);
			if ((ch==16) && (!IsDLE) )
				{
					IsDLE=1;
					continue;
				}
			 ucpRecvBuffer[j]=ch;
			if (IsDLE)
			{
				 IsDLE=0;
			}
			j++;
//#ifdef USER_DEBUG
//			Rprintf("%02X", ch);
//#endif
		}

		*ipLen=j;

	ICON_DISPLAY( 10);

	return GPRS_SUCCESS;
}
///////////////////////////////////////////////////////////////////////////
int GPRS_ReceiveDataLen(unsigned char *ucpRecvBuffer,int *ipLen,unsigned short usTimeOut,int len)
{
	//check modem type
	if (ucGPRSModemType==WAVECOM)
	{
		// old modem
		return GPRS_ReceiveDataWAVECOMLen(ucpRecvBuffer,ipLen,usTimeOut,len);
	}
	else if (ucGPRSModemType==CINTERION)
	{
		//new modem
		return GPRS_ReceiveDataCINTERIONLen(ucpRecvBuffer,ipLen,usTimeOut,len);
	}
}
int GPRS_ReceiveDataCINTERIONLen(unsigned char *ucpRecvBuffer,
							  int *ipLen,
							  unsigned short usTimeOut,int len)
{
	char temp[30];
	int i, imax, j, k, no, size;
	int tmp_COM1R_COUNT=0;
	unsigned char uctemp[500];
	unsigned long usWaittime;
	UBYTE ch, tmpbuf[32];
	GSMModemProcess=-1;
	unsigned char IsDLE=0;
	gsmrxptr = gsmRxStream;
	usWaittime =Get1MSTimer();

	do{

	}while((COM1R_COUNT==0) && (delta1MS(usWaittime)<usTimeOut));

	do{
		tmp_COM1R_COUNT=COM1R_COUNT;
		Wait1MS(300);
	}while(tmp_COM1R_COUNT!=COM1R_COUNT);


	if (!COM1R_COUNT)
	{
		*ipLen=0;
		return GPRS_NODATARECV;
	}

	ICON_DISPLAY( 11);

	imax=COM1R_COUNT;
	j=0;


		for ( i = 0 ; i < imax ; i++ )
		{
			if ((len>0) && (i>=len)) break;
			ch = GPRSReadCom(COM1);
			 ucpRecvBuffer[j]=ch;
			j++;
#ifdef USER_DEBUG
			//Rprintf("%02X", ch);
#endif
		}

		*ipLen=j;

	ICON_DISPLAY( 10);

	return GPRS_SUCCESS;
}
/*
 * \ brief
 * \param
 * \return
 */
int GPRS_ReceiveDataWAVECOMLen(unsigned char *ucpRecvBuffer,
							int *ipLen,
							unsigned short usTimeOut,int len)
{
	char temp[30];
	int i, imax, j, k, no, size;
	int tmp_COM1R_COUNT=0;
	unsigned char uctemp[500];
	unsigned long usWaittime;
	UBYTE ch, tmpbuf[32];
	GSMModemProcess=-1;
	unsigned char IsDLE=0;
#ifdef USER_DEBUG
	Rprintf( "\r\n Receiving ........... \r\n");
#endif
	gsmrxptr = gsmRxStream;
	usWaittime =Get1MSTimer();
	do{
	}while((COM1R_COUNT==0) && (delta1MS(usWaittime)<usTimeOut));

	do{
		tmp_COM1R_COUNT=COM1R_COUNT;
		Wait1MS(300);
	}while(tmp_COM1R_COUNT!=COM1R_COUNT);


	if (!COM1R_COUNT)
	{
		*ipLen=0;
		return GPRS_NODATARECV;
	}

	ICON_DISPLAY( 11);

	imax=COM1R_COUNT;
	j=0;
		for ( i = 0 ; i < imax ; i++ )
		{
			if ((len>0) && (j>=len)) break;
			ch = GPRSReadCom(COM1);
			if ((ch==16) && (!IsDLE) )
				{
					IsDLE=1;
					continue;
				}
			 ucpRecvBuffer[j]=ch;
			if (IsDLE)
			{
				 IsDLE=0;
			}
			j++;
#ifdef USER_DEBUG
			Rprintf("%02X", ch);
#endif
		}

		*ipLen=j;

	ICON_DISPLAY( 10);
#ifdef USER_DEBUG
	Rprintf( "\r\n end receiving ........... %d\r\n",j);
#endif
	return GPRS_SUCCESS;
}


/*
 * \ brief Parent function to Disconnect the connection with host
 * \param
 * \return
 */
int GPRS_DisConnect()
{
	//check modem type
	if (ucGPRSModemType==WAVECOM)
	{
		//Old modem 
		return GPRS_DisConnectWAVECOM();
	}
	else if (ucGPRSModemType==CINTERION)
	{
		//New Modem
		return GPRS_DisConnectCINTERION();
	}
}

//int GPRS_DisConnectCINTERION()
//{
//  char temp[100];
//  int i,j,retry;
//  unsigned char nextstep;
//  unsigned short usWaittime;
//
//  //wait
//  RS232_WaitTxDone(COM1);
//  //Send Exit char
//  GPRSWriteCom(COM1, (char *)"\x10\x01", 2);
//
//  SLEEP_M(1000);  //delay
//  nextstep=true;
//
//  //
//  if (nextstep==true)
//  {
//    nextstep=0xFF;
//    retry=0;
//    memset(temp,0,sizeof(temp));
//    //Close socket
//    sprintf(temp, "AT^SISC=0\r\0");
//    usWaittime =Get1MSTimer();
//    do{
//      GPRS_SendCommand(temp,temp,retry);
//      retry=1;
//      for (i=0;i<sizeof(temp);i++)
//      {
//        if (!memcmp(&temp[i],(char *)"OK",2))
//        {
//          nextstep=true;
//          break;
//        }
//        if (!memcmp(&temp[i],(char *)"ERROR",5))
//         {
//           nextstep=false;
//           break;
//         }
//      }
//      //if no reponse loop for 4 seconds
//    }while((nextstep==0xFF) && (delta1MS(usWaittime)<(4000)));
//  }
//  if (nextstep!=true)
//  {
//    return GPRS_SOCKET;
//  }
//
//  return GPRS_SUCCESS;
//}
/*
 * \ brief Disonnect the connection with host
 * \return  GPRS_SUCCESS            : Disconnection  succeeded
 * 			GPRS_SOCKET             : Error closing socket
 */
int GPRS_DisConnectCINTERION()
{
	char temp[100];
	int i,j,retry;
	unsigned char nextstep;
	unsigned short usWaittime;

	Wait1MS(1200);
		 //loop twice to minimize the delay incase of no response
	  for (j=0;j<2;j++)
     {
     	//send pause command
     	GPRS_Pause_SendCommand((char *)"+++\0",temp,0);
        Wait1MS(1500);
         for(i=0; i<sizeof(temp);i++)
         {
         	//check the response
            if (!memcmp(&temp[i],(char *)"OK",2))
            {

                nextstep=true;
                i=sizeof(temp);
                j=2;
            }
         }
     }
	if (nextstep==true)
	{
		nextstep=0xFF;
		retry=0;
		memset(temp,0,sizeof(temp));
		//Close socket
		sprintf(temp, "AT^SISC=0\r\0");
		usWaittime =Get1MSTimer();
		do{
			GPRS_SendCommand(temp,temp,retry);
			retry=1;
			for (i=0;i<sizeof(temp);i++)
			{
				if (!memcmp(&temp[i],(char *)"OK",2))
				{
					nextstep=true;
					break;
				}
				if (!memcmp(&temp[i],(char *)"ERROR",5))
				 {
				 	nextstep=false;
				 	break;
				 }
			}
			//if no response loop for 4 seconds
		}while((nextstep==0xFF) && (delta1MS(usWaittime)<(4000)));
	}
	if (nextstep!=true)
	{
		return GPRS_SOCKET;
	}

	return GPRS_SUCCESS;
}
/*
 * \ brief
 * \param
 * \return
 */
int GPRS_DisConnectWAVECOM()
{
	char temp[30];
	int i, imax, j, k, no, size;
	int tmp_COM1R_COUNT=0;
	UBYTE ch, tmpbuf[32];
	GSMModemProcess=39;

	Wait1MS(1000);

	while(1)
	{

			switch(GSMModemProcess)
			{
				
				case 39:
					gsmDebugMessage("PROCESS 39");
					GSMCommandWrite("+++");
					GSMModemProcess++;
					break;
				case 41:
					gsmDebugMessage("PROCESS 41");
					GSMCommandWrite("AT+WIPCLOSE=2,1\r");
					GSMModemProcess++;
					break;
				case 99:
				case 999:
					gsmDebugMessage("PROCESS 999");
					//GSMModemClose();
					GSMModemProcess = -1;
					return 0;
			}

			usWaittime =Get1MSTimer();
			do{

			}while((COM1R_COUNT==0) && (delta1MS(usWaittime)<5000));
			do{
				tmp_COM1R_COUNT=COM1R_COUNT;
				Wait1MS(100);
			}while(tmp_COM1R_COUNT!=COM1R_COUNT);

			if (!COM1R_COUNT)
			{
				return GPRS_NORESP;
			}


			for ( i = 0 ; i < COM1R_COUNT ; i++ )
			{
				ch = GPRSReadCom(COM1);
				if (gsmrxptr - &gsmRxStream[0] > 2048) gsmrxptr = gsmRxStream;
				*gsmrxptr++ = ch;
#ifdef USER_DEBUG
				Rprintf("%c", ch);
#endif
				size = gsmrxptr - &gsmRxStream[0];
				if ((size >= 2) && (*(gsmrxptr-2) == '\r') && (*(gsmrxptr-1) == '\n'))
				{
					memset(tmpbuf, 0, sizeof(tmpbuf));
					memcpy(tmpbuf, gsmRxStream, size - 2);
			    	if (!strncmp(gsmrxptr-4, "OK", 2))
			    	 {
						gsmResultMessage("OK");

	    				GSMModemProcess++;
	    				if (GSMModemProcess == 10) 
	    				{
	    					GSMModemProcess = 12;
	    				}
	    				if (GSMModemProcess>41)
	    				{
	    					GPRSCom1_Init();
	    					GSMModemProcess=999;
	    				}
	    			}
			    	else if (!strncmp(gsmrxptr-7, "ERROR", 5))
			    	{
						gsmResultMessage("ERROR");
	    				GSMModemProcess = 999;
					}
			    	else if (!strncmp(gsmRxStream, "+CME ERROR: 842", 15))
			    	{
			    		return 0;
			    		GSMModemProcess = 999;
			    	}
			    	else if (!strncmp(gsmRxStream, "+CME ERROR: ", 12) || 
			    	         !strncmp(gsmRxStream, "+CMS ERROR: ", 12))
			    	{
						size = gsmrxptr - &gsmRxStream[12] - 2;
						memset(gsmTxStream, '\0', 41);
						strncpy(gsmTxStream, &gsmRxStream[12], size);
						gsmResultMessage(gsmTxStream);
						no = GetCMEError(gsmTxStream);
						switch( no ) {
							case 515:
							case 527:
	    						GSMModemProcess--;
	    						break;
							case 3:
	//    						GSMModemProcess = 24;
	    						break;
	    					case 803:	// already GPRS opened
	    						GSMModemProcess = 30;
	    						break;
							case 812:
								TIME_BASE10 = 0x8000 | 100;		// 1 SEC
								GSMModemProcess = 999;
								return 0;
								break;
							case 831:
								if(GSMModemProcess == 34)
									GSMModemProcess = 33;								// 34
								break;
							case 10:
			    				GSMModemProcess = 999;
	    						break;
	    					default:
	//		    				GSMModemProcess = 999;
	    						break;
	    				}
	    			}
			    	else if (!strncmp(gsmRxStream, "+CREG: ", 7))
	    			{
						size = gsmrxptr - &gsmRxStream[7] - 2;
						memset(gsmTxStream, '\0', 5);
						strncpy(gsmTxStream, &gsmRxStream[7], size);
						gsmResultMessage(gsmTxStream);
						no = atoi(gsmTxStream);
						switch( no ) {
	    					case 0:	/* not registered, ME is not currently searching for a new operator */
				    			GSMModemProcess = 12;
	    						break;
	    					case 1:	/* registered, home network. */
	//			    			GSMModemProcess = 12;
	    						break;
		    				case 2:	/* not registered, ME currently searching for a new operator to register to */
				    			GSMModemProcess = 12;
	    						break;
	    					case 5:	/* registered, roaming */
				    			GSMModemProcess = 12;
	    						break;
	    					case 3:	/* registration denird. */
	    					case 4:	/* unknown */
	    						break;
	    				}
					}
			    	else if (!strncmp(gsmrxptr-12, "NO CARRIER", 10))
			    	{
						gsmResultMessage("NO CARRIER");
		    			GSMModemProcess = 999;
	    			}
			    	else if (!strncmp(gsmrxptr-11, "NO ANSWER", 9))
			    	{
						gsmResultMessage("NO ANSWER");
		    			GSMModemProcess = 999;
		    		}
			    	else if (!strncmp(gsmrxptr-6, "BUSY", 4))
			    	{
						gsmResultMessage("BUSY");
		    			GSMModemProcess = 999;
					}
			    	else if (!strncmp(gsmrxptr-14, "+CPIN: READY", 12))
			    	{
						GSMModemProcess = 16;
					}
			    	else if (!strncmp(gsmrxptr-16, "+CPIN: SIM PIN", 14))
			    	{
						GSMModemProcess++;
		    		}
			    	else if (!strncmp(gsmRxStream, "CONNECT", 7))
			    	{
						gsmResultMessage("CONNECT");
						GSMModemProcess++;
					}
			    	else if (!strncmp((char *)gsmRxStream, "+WIPREADY:", 10))
			    	{
						GSMModemProcess++;
					}
					gsmrxptr = gsmRxStream;
				}

			}

	}

	return GPRS_SUCCESS;
}
/* 
 * \brief- Initialize the GPRS device COM port Baud Rate speed to 115200 BPS
 * 
 * \return GPRS_SUCCESS         : initialization success
 * 		   GPRS_NOTFOUND  : No GPRS device found for the COM port 
 *         GPRS_PORTFAILED: Failure to communicate with port
 * 			MODEM_NOT_DETECTED -2: Modem is not detected
 * */
int GPRS_intializePort()
{

	//used to configure the COM port baud rate
	//configure the COM baud rate to be 115200 BPS
	//try the different speeds till get response with BPS X
	//use the X speed to configure higer speed 115200 BPS
	//GPRS_ConfigureRSPort(7);
    //return 0;

	int i,j;
	char ch;
	char temp[200];
//	char HWINFO[16];
	unsigned char retry;
    unsigned char nextstep;

    nextstep=false;

    GetSystemHWInformation(HWINFO);

    //Detect SIM type (0x32:TRIPLE SIM)
    uiSimSlotNum=(HWINFO[14] == 0x32)? 3: 2;

    //check the 7th digit to know modem type
    //value of new modem=7 and 8
    //Value of old modem on Flex5100 =3
    //Value of old modem on IC5100 =2
    if (HWINFO[7]==0x37 ||(HWINFO[7]==0x38)) 
    {
    	ucGPRSModemType=CINTERION;
    }
    else if ((HWINFO[7]==0x33) || (HWINFO[7]==0x35)|| (HWINFO[7]==0x32)) 
    {
    	ucGPRSModemType=WAVECOM;
    }
    else
    {
    	return MODEM_NOT_DETECTED;
    }
#ifdef USER_DEBUG
    Rprintf("GetSystemHWInformation MODEM: %c\r\n",HWINFO[7]);
#endif
       //number of values of BaudValue
		for ( j = 7 ; j >= 0 ; j--)
		{
			GPRS_ConfigureRSPort(j);
			SLEEP_M(200);
			nextstep=0xFF;
			retry=0;
			memset(temp,0,sizeof(temp));
			usWaittime =Get1MSTimer();
			do{
					// Network registration command
					GPRSWriteCom(COM1,(char *)"AT+CREG=0\r\n\0",11);
					SLEEP_M(2000);
					GPRS_SendCommand((char *)"AT+CREG=0\r\n\0",temp,retry);
					retry=0;
					for (i=0;i<sizeof(temp);i++)
					{
						if (!memcmp(&temp[i],(char *)"OK",2)) 
						{
							nextstep=true;
							break;
						}

					}
			//in case of no response,loop for 6 seconds
			}while((nextstep==0xFF) && (delta1MS(usWaittime)<(6000)));

			SLEEP_M(200);
			//Force echo in reply
			GPRS_SendCommand((char *)"ATE1\r\0",temp,retry);
			SLEEP_M(200);
			GPRS_SendCommand((char *)"AT&W\r\0",temp,0);

				if (nextstep==true && j==7) 
				{
					return GPRS_SUCCESS;
				}

			if (nextstep==true)
			{
				SLEEP_M(200);
				// Bit Rate command
				GPRS_SendCommand((char *)"AT+IPR=115200\r\0",temp,0);
				for (i=0;i<sizeof(temp);i++)
				{
					if (!memcmp(&temp[i],(char *)"OK",2))
					{
						nextstep=true;
						break;
					}
				}


				if ((nextstep==true) && (ucGPRSModemType==WAVECOM))
				{
					SLEEP_M(200);
					GPRS_SendCommand((char *)"AT+WIND=0\r\0",temp,0);
					SLEEP_M(200);
					GPRS_SendCommand((char *)"AT+CGREG=0\r\0",temp,0);
					SLEEP_M(200);
					GPRS_SendCommand((char *)"AT&W\r\0",temp,0);
					for (i=0;i<sizeof(temp);i++)
					{
						if (!memcmp(&temp[i],(char *)"OK",2))
						{
							nextstep=true;
							return GPRS_SUCCESS;
						}
					}
				}
				else if ((nextstep==true) && (ucGPRSModemType==CINTERION))
				{
					return GPRS_SUCCESS;
				}
				break;
			}//end of if condition
		}//end of for loop
		if (i < 0)
		{
			return GPRS_NOTFOUND;
		}

		return GPRS_PORTFAILED;

}
//TODO:
/*
 * \ brief
 * \param
 * \return
 */
void GPRS_ConfigureRSPort(int mode)
{
	int speed, delayus;

	if ((mode > 7) || (mode < 0)) 
	{
		speed = 7;
	} 
	else 
	{
		speed = mode;
	}

	CONTROL_POWER_SAVING_STATUS(P_EXT_SERIAL, ON);
	CONTROL_POWER_SAVING_STATUS(P_RF_MODULE, ON);

	SLEEP_M(200);
	SET_BUFFER_SIZE_8K();
	RS232_SETUP(COM1, speed, 0,1);
	RS232_ENABLE(COM1, ENABLE, RX_ENABLE);
	GPRSCom1_Init();
}
//TODO
/*
 * \ brief
 * \param
 * \return
 */

int WriteToPort(char *stream)
{
	int i, j, retry, size;
	unsigned char ch;
	char temp[20];

	i = retry = 0;

	while( 1 )
	{
		if (!i)
		{
			size = strlen(stream);
			WriteToCom(COM1, stream, size);
			gsmrxptr = gsmRxStream;
			TIME_BASE0 = 0x8000 | 1000;
			i++;
		}
		Wait1MS(100);
		size = COM1R_COUNT;
		for ( j = 0 ; j < size ; j++ )
		{
			ch = ReadFromCom(COM1);
#ifdef USER_DEBUG
					Rprintf("%c", ch);
#endif
			*gsmrxptr++ = ch;
		}

			if ((*(gsmrxptr-2) == '\r') && (*(gsmrxptr-1) == '\n'))
			{
				if (!strncmp((char*)gsmrxptr-4, "OK", 2)) 
				{
					return (1);
				}
				if (!strncmp((char*)gsmrxptr-7, "ERROR", 5))
				 {
					i = 0;
					retry++;
				}
				gsmrxptr = gsmRxStream;
			}

		if (i && !(TIME_BASE0 & 0x8000)) 
		{
			gsmrxptr = gsmRxStream;
			i = 0;
			retry++;
		}
		if (retry >= 3) 
		{
			return (-1);
		}
	}

	return (0);
}
//TODO
/*
 * \ brief
 * \param
 * \return
 */
void WriteToCom( int channel, char *stream, int size )
{
	int i, j;

	GPRSCom1_Init();

	while (!RS232_TX_STATUS(channel))
	{
	}

	switch(channel) {
		case 0:
			j = COM0T_IPOINT;
			break;
		case 1:
			j = COM1T_IPOINT;
			break;
		case 2:
			j = COM2T_IPOINT;
			break;
		case 3:
			j = COM3T_IPOINT;
			break;
	}


	for ( i = 0 ; i < size ; i++ ) {
		j &= 0x1FFF; //8K
		switch( channel ) {
			case 0:
				COM0T_BUFFER[j] = *stream++;
				COM0T_COUNT++;
				break;
			case 1:
				COM1T_BUFFER[j] = *stream++;
				COM1T_COUNT++;
				break;
			case 2:
				COM2T_BUFFER[j] = *stream++;
				//COM2T_COUNT++;
				break;
			case 3:
				COM3T_BUFFER[j] = *stream++;
				COM3T_COUNT++;
				break;
		}
		j++;
	}
	switch( channel ) {
				case 0:
					COM0T_COUNT=COM0T_COUNT+size;
					break;
				case 1:
					COM1T_COUNT=COM1T_COUNT+size;
					break;
				case 2:
					COM2T_COUNT=COM2T_COUNT+size;
					break;
				case 3:
					COM3T_COUNT=COM3T_COUNT+size;
					break;
			}

	RS232_ENABLE( channel, ENABLE, TX_ENABLE );
}
//TODO
/*
 * \ brief
 * \param
 * \return
 */
int ReadFromCom( int channel )
{
	int ch;

	switch( channel ) {
		case 0:
			COM0R_MPOINT &= 0x1FFF;
			ch = COM0R_BUFFER[COM0R_MPOINT++];
			GEMRXCOUNT();
			break;
		case 1:
			COM1R_MPOINT &= 0x1FFF;
			ch = COM1R_BUFFER[COM1R_MPOINT++];
			COM1RXCOUNT();
			break;
		case 2:
			COM2R_MPOINT &= 0x1FFF;
			ch = COM2R_BUFFER[COM2R_MPOINT++];
			COM2RXCOUNT();
			break;
		case 3:
			COM3R_MPOINT &= 0x1FFF;
			ch = COM3R_BUFFER[COM3R_MPOINT++];
			COM3RXCOUNT();
			break;
	}
	return (ch);
}
/*
 * \ brief Check if the connection is Up and running
  * \return GPRS_CONNECTED      : Connection is up
 *          GPRS_NOTCONECTED    :Connection is down
 */
unsigned char GPRS_IsConnected()
{
	int size;
	char temp[20];
	int tmp_COM1R_COUNT=0;
	char ch;
	char temp_string[5];
	strcpy(temp_string, "AT\r\0");
	GPRSWriteCom(COM1, temp_string,  strlen(temp_string));
	usWaittime =Get1MSTimer();
	//
	do
	{

	}while((COM1R_COUNT<10) && (delta1MS(usWaittime)<(2000)));

	//
	do
	{
		tmp_COM1R_COUNT=COM1R_COUNT;
		SLEEP_M(200);

	}while(tmp_COM1R_COUNT!=COM1R_COUNT);

	int i=0;
	size=0;

	//
	for ( i = 0 ; i < tmp_COM1R_COUNT ; i++ )
	{

		ch = GPRSReadCom(COM1);
		temp[i]=ch;
		size++;
		if (i>=3)
		{
			if (!strncmp(&temp[i-3], "CME", 3))
			{
				return GPRS_NOTCONECTED ;
			}
		}
	}

	return GPRS_CONNECTED;

}
/*
 * \ brief Parent function to Connect to Host
 */
unsigned char GPRS_Connect(char *cpIP,char *cpPORT,unsigned short usTimeOut)
{
	//check modem type
	if (ucGPRSModemType==WAVECOM)
	{
			return GPRS_ConnectWAVECOMM(cpIP,cpPORT,usTimeOut);
	}
	else if (ucGPRSModemType==CINTERION)
	{
		return GPRS_ConnectCINTERION(cpIP,cpPORT,usTimeOut);
	}
}
/*
 * \ brief
 * \param
 * \return
 */
unsigned char GPRS_ConnectWAVECOMM(char *cpIP,char *cpPORT,unsigned short usTimeOut)
{
	char temp[100];
	int i, imax, j, k, no, size,retry;
	unsigned char nextstep;
	unsigned short usWaittime;

	nextstep=false;
	memset(temp,0,sizeof(temp));
	
	GPRS_SendCommand((char *)"AT+WIPBR?\r\0",temp,0);
	for (i=0;i<sizeof(temp);i++) 
	{
		if (!memcmp(&temp[i],(char *)"6,1",3)) 
		{
			nextstep=true;
			break;
		}
	}

	if (nextstep==false)
	{
		nextstep=0xFF;
		retry=0;
		memset(temp,0,sizeof(temp));
		usWaittime =Get1MSTimer();
		do{
			GPRS_SendCommand((char *)"AT+WIPBR=4,6,0\r\0",temp,retry);
			retry=1;
			for (i=0;i<sizeof(temp);i++)
			{
				if (!memcmp(&temp[i],(char *)"OK",2))
				 {
				 	nextstep=true;
				 	break;
				 }
				if (!memcmp(&temp[i],(char *)"ERROR",5)) 
				{
					nextstep=false;
					break;
				}

			}
		}while((nextstep==0xFF) && (delta1MS(usWaittime)<(usTimeOut)));
	}

	if (nextstep!=true) 
	{
		return GPRS_ERRNETWORK;
	}

	if (nextstep)
	{
		nextstep=0xFF;
		retry=0;
		memset(temp,0,sizeof(temp));
		sprintf(temp, "AT+WIPCREATE=2,1,\"%s\",%s\r\0", cpIP, cpPORT);
		usWaittime =Get1MSTimer();
		do{
			GPRS_SendCommand(temp,temp,retry);
			retry=1;
			for (i=0;i<sizeof(temp);i++)
			{
//				if (!memcmp(&temp[i],(char *)"OK",2))
//				{
//					nextstep=true;
//					break;
//				}
				//changed on 11/11/2013
				if (!memcmp(&temp[i],(char *)"+WIPREADY:",10))
				{
					nextstep=true;
					break;
				}
				if (!memcmp(&temp[i],(char *)"ERROR",5)) 
				{
					nextstep=false;
					break;
				}
			}
		}while((nextstep==0xFF) && (delta1MS(usWaittime)<(usTimeOut)));
	}

	if (nextstep!=true) 
	{
		return GPRS_SOCKET;
	}

	if (nextstep==true)
	{
		nextstep=false;
		memset(temp,0,sizeof(temp));
		GPRS_SendCommand((char *)"AT+WIPDATA=2,1,1\r\0",temp,0);
		for (i=0;i<sizeof(temp);i++) 
		{
			if (!memcmp(&temp[i],(char *)"CONNECT",7)) 
			{ 
				nextstep=true;
				break;
			}
		}
	}

	if (nextstep!=true) 
	{
		return GPRS_ERROR;
	}
	else
	{
		return GPRS_SUCCESS;
	}
}
/*
 * \ brief
 * \param
 * \return
 */
unsigned char GPRS_ConnectCINTERION(char *cpIP,char *cpPORT,unsigned short usTimeOut)
{
	
	char temp[100];
	int i,retry;
	unsigned char nextstep;
	unsigned short usWaittime;

    //1- close the socket
	GPRS_SendCommand((char *)"AT^SISC=0\r\0",temp,0);

	nextstep=0xFF;
	retry=0;
	memset(temp,0,sizeof(temp));
	//2- Internet Service Setup Profile
	//sprintf(temp, "AT^SISS=0,ADDRESS,\"%s:%s;etx=1\"\r\0", cpIP, cpPORT);
	sprintf(temp, "AT^SISS=0,ADDRESS,\"%s:%s\"\r\0", cpIP, cpPORT);
	usWaittime =Get1MSTimer();
	do{
			GPRS_SendCommand(temp,temp,retry);
			retry=1;
			//check the response
			for (i=0;i<sizeof(temp);i++)
			{
				if (!memcmp(&temp[i],(char *)"OK",2))
				 {
				 	nextstep=true;
				 	break;
				 }
				if (!memcmp(&temp[i],(char *)"ERROR",5)) 
				{
					nextstep=false;
					break;
				}
			}
			//if no response loop for 10 seconds
	}while((nextstep==0xFF) && (delta1MS(usWaittime)<(10000)));
		
	if (nextstep!=true) 
	{
		return GPRS_SOCKET;
	}
	if (nextstep==true)
	{
		nextstep=0xFF;
		retry=0;
		memset(temp,0,sizeof(temp));
		usWaittime =Get1MSTimer();
		do{
			//3- Internet Service Open
			GPRS_SendCommand((char *)"AT^SISO=0\r\0",temp,retry);
			retry=1;
			//check the response
			for (i=0;i<sizeof(temp);i++)
			{
				if (!memcmp(&temp[i],(char *)"^SISW: 0, 1",11)) 
				{
					nextstep=true;
					break;
				}
				if (!memcmp(&temp[i],(char *)"ERROR",5)) 
				{
					nextstep=false;
					break;
				}

			}
		}while((nextstep==0xFF) && (delta1MS(usWaittime)<(usTimeOut)));

	}
	
	if (nextstep!=true) 
	{
		return GPRS_ERRNETWORK;
	}

	SLEEP_M(1000);
	if (nextstep==true)// changed by heba 3/7/2013
	{
		nextstep=0xFF;
		retry=0;
		memset(temp,0,sizeof(temp));
		usWaittime =Get1MSTimer();
		do{
			   //4- Enter Transparent Access Mode
				GPRS_SendCommand((char *)"AT^SIST=0\r\0",temp,retry);
				//check the response
				for (i=0;i<sizeof(temp);i++)
				{
					if (!memcmp(&temp[i],(char *)"CONNECT",7)) 
					{
						nextstep=true;
						break;
					}
					if (!memcmp(&temp[i],(char *)"ERROR",5)) 
					{
						nextstep=false;
						break;
					}
					if(!memcmp(&temp[i],(char *)"NO CARRIER",10))
					{
						break;
					}

				}
			}while((nextstep==0xFF) && (delta1MS(usWaittime)<(usTimeOut)));
	}
	if (nextstep!=true) 
	{
		return GPRS_ERROR;
	}
	else
	{
		return GPRS_SUCCESS;
	}

}
/*
 * \ brief
 * \param [in/out] ucpCommand:  AT Command
 * 		  [in/Out] ucpResp       : pointer to the length of received response
 * 		  [in]     iRetry      : 
 * \return GPRS_ERROR
 *         GPRS_SUCCESS
 */
unsigned char GPRS_SendCommand(unsigned char *ucpCommand,
							   unsigned char *ucpResp,
							   int iRetry)
{
	int i;
	char temp_string[2048];
	unsigned short usWaittime;
	int tmp_COM1R_COUNT=0;
	char ch;

	memset(temp_string,0,sizeof(temp_string));
	memcpy(temp_string, ucpCommand,strlen(ucpCommand));

#ifdef USER_DEBUG
//					Rprintf(temp_string);
#endif

//TODO:
	if (iRetry==0) 
	{
		GPRSWriteCom(COM1, temp_string,  strlen(temp_string));
	}

	usWaittime =Get1MSTimer();
	do{

	}while((COM1R_COUNT<1) && (delta1MS(usWaittime)<(20000)));

	if(COM1R_COUNT<1) 
	{
		return GPRS_ERROR;
	}

	do{
		tmp_COM1R_COUNT=COM1R_COUNT;
		Wait1MS(200);
	}while(tmp_COM1R_COUNT!=COM1R_COUNT);

	memset(temp_string,0,sizeof(temp_string));
	for ( i = 0 ; i < tmp_COM1R_COUNT ; i++ )
	{
		ch = GPRSReadCom(COM1);
		temp_string[i]=ch;

#ifdef USER_DEBUG
					Rprintf("%c", ch);
#endif

	}

	memcpy(ucpResp,temp_string,tmp_COM1R_COUNT);

	return GPRS_SUCCESS;
}
/*
 * \ brief Same funaction of Send command but with 3 second dealy instead of 20 seconds
 * build especially to Disconnect function
 */
unsigned char GPRS_Pause_SendCommand(unsigned char *ucpCommand,
							   unsigned char *ucpResp,
							   int iRetry)
{
	int i;
	char temp_string[50];
	unsigned short usWaittime;
	int tmp_COM1R_COUNT=0;
	char ch;

	memset(temp_string,0,sizeof(temp_string));
	memcpy(temp_string, ucpCommand,strlen(ucpCommand));

#ifdef USER_DEBUG
//					Rprintf(temp_string);
#endif

	if (iRetry==0)
	{
		GPRSWriteCom(COM1, temp_string,  strlen(temp_string));
	}

	usWaittime =Get1MSTimer();
	do{

	}while((COM1R_COUNT<1) && (delta1MS(usWaittime)<(2500)));

	if(COM1R_COUNT<1)
	{
		return GPRS_ERROR;
	}

	do{
		tmp_COM1R_COUNT=COM1R_COUNT;
		Wait1MS(200);
	}while(tmp_COM1R_COUNT!=COM1R_COUNT);

	memset(temp_string,0,sizeof(temp_string));
	for ( i = 0 ; i < tmp_COM1R_COUNT ; i++ )
	{
		ch = GPRSReadCom(COM1);
		temp_string[i]=ch;

#ifdef USER_DEBUG
					Rprintf("%c", ch);
#endif

	}

	memcpy(ucpResp,temp_string,tmp_COM1R_COUNT);

	return GPRS_SUCCESS;
}

/*
 * \ brief
 * \param
 * \return
 */
int GPRS_getVersion(void )
{
	return GPRS_LIB_VERSION;
}
/*
 * \ brief Get the quality of the signal by retrieving the signal strength value
 * \param
 * \return
 */
int GPRS_getSignalStrength()
{
	char temp[300];
	unsigned short locationOfCSQ=0;
	int i,retry;
	unsigned char nextstep;
	unsigned short usWaittime;
	int iSigValue;

	nextstep=0xFF;
	retry=0;
	memset(temp,0,sizeof(temp));
	usWaittime =Get1MSTimer();
	
	do{
		// Signal quality command
		GPRS_SendCommand((char *)"AT+CSQ\r\0",temp,retry);
		retry=1;
		for (i=0;i<sizeof(temp);i++)
		{
			if (!memcmp(&temp[i],(char *)"+CSQ:",5)) 
			{
				nextstep=true;
			  //location of SQ is the position of "+CSQ: " + length of it= 6
				locationOfCSQ=i+6;
				break;
			}

		}
	}while((nextstep==0xFF) && (delta1MS(usWaittime)<5000));

	if (nextstep==true)
	{
		iSigValue=atoi(&temp[locationOfCSQ]);
		if (iSigValue>=0) 
		{
			return iSigValue;
		}
	}
    else
    { 
    	return -1;
    }

}
//TODO
/*
 * \ brief
 * \param
 * \return
 */

void GPRSWriteCom( int channel, char *stream, int size )
{
	
	int i, j;

	GPRSCom1_Init();

	while (!RS232_TX_STATUS(channel))
	{
	}

	switch(channel) {
		case 0:
			j = COM0T_IPOINT;
			break;
		case 1:
			j = COM1T_IPOINT;
			break;
		case 2:
			j = COM2T_IPOINT;
			break;
		case 3:
			j = COM3T_IPOINT;
			break;
	}


	for ( i = 0 ; i < size ; i++ )
	 {
		j &= 0x1FFF;//8k
		switch( channel ) {
			case 0:
				COM0T_BUFFER[j] = *stream++;
				COM0T_COUNT++;
				break;
			case 1:
				COM1T_BUFFER[j] = *stream++;
				COM1T_COUNT++;
				break;
			case 2:
				COM2T_BUFFER[j] = *stream++;
				//COM2T_COUNT++;
				break;
			case 3:
				COM3T_BUFFER[j] = *stream++;
				//COM3T_COUNT++;
				break;
		}
		j++;
	}

	COM2T_COUNT=COM2T_COUNT+size;
	COM3T_COUNT=COM3T_COUNT+size;

	RS232_ENABLE( channel, ENABLE, TX_ENABLE );


}
//TODO
/*
 * \ brief
 * \param
 * \return
 */
int GPRSReadCom( int channel )
{
	int ch;

	switch( channel ) {
		case 0:
			COM0R_MPOINT &= 0x1FFF;
			ch = COM0R_BUFFER[COM0R_MPOINT++];
			GEMRXCOUNT();
			break;
		case 1:
			COM1R_MPOINT &= 0x1FFF;
			ch = COM1R_BUFFER[COM1R_MPOINT++];
			COM1RXCOUNT();
			break;
		case 2:
			COM2R_MPOINT &= 0x1FFF;
			ch = COM2R_BUFFER[COM2R_MPOINT++];
			COM2RXCOUNT();
			break;
		case 3:
			COM3R_MPOINT &= 0x1FFF;
			ch = COM3R_BUFFER[COM3R_MPOINT++];
			COM3RXCOUNT();
			break;
	}
	return (ch);
}
//TODO
/*
 * \ brief
 * \param
 * \return
 */
void GPRSCom1_Init( void )
{
	COM1T_IPOINT = COM1T_MPOINT = COM1R_IPOINT = COM1R_MPOINT = 0;
	COM1T_COUNT = COM1R_COUNT = 0;
	memset(COM1T_BUFFER, '\0', sizeof(COM1T_BUFFER));
	memset(COM1R_BUFFER, '\0', sizeof(COM1R_BUFFER));
}


/*
 * \ brief
 * \param
 * \return	0 Success
 * 			-1 SIM Slot not supported
 */
char GPRS_SimSelect( int iSlot )
{

//	CONTROL_POWER_SAVING_STATUS(2, 0);
//	SLEEP_M(1000);
//	SIM_SELECT(iSlot);
//	SLEEP_M(1000);
//	CONTROL_POWER_SAVING_STATUS(2, 1);
//	return;

#ifdef USER_DEBUG
	Rprintf("\r\n");
	Rprintf("GSM Power On\r\n");
#endif
	if((uiSimSlotNum ==2) && (iSlot==2) )
	{
		return -1;
	}
	SIM_SELECT(iSlot);
	CONTROL_POWER_SAVING_STATUS(P_EXT_SERIAL, ON);
	CONTROL_POWER_SAVING_STATUS(P_RF_MODULE, ON);
	CONTROL_POWER_SAVING_STATUS(P_SMARTCARD, ON);
	SLEEP_M(100);

	return 0;
}

/*
 * \ brief Detects whether there is a SIM card inserted in the SIM slot under work
 * \ return 	0     :   SIM Card inserted
 *
 *			 	Else:   SIM is absent
 */
unsigned char GPRS_isSimPresent(int iSlot )
{

	char temp[200];
	int i, retry;
	unsigned char nextstep;
	unsigned short usWaittime;

	nextstep=0xFF;
	retry=0;
	memset(temp,0,sizeof(temp));
	usWaittime =Get1MSTimer();

	//Select Sim and POwer GSM ON
	if(GPRS_SimSelect(iSlot)==-1)
	{
		return GPRS_ERROR;
	}

	//Send Command
	SLEEP_M(200);
	GPRSWriteCom(COM1,(char *)"ATE\r\0",4);
	SLEEP_M(200);
	GPRS_SendCommand((char *)"ATE\r\0",temp,retry);
	SLEEP_M(300);
	do{
		// Pin Configuration command
		SLEEP_M(1000);
		GPRS_SendCommand((char *)"AT+CPIN?\r\0",temp,retry);
		SLEEP_M(200);
		retry=0;
		for (i=0;i<sizeof(temp);i++)
		{
			if (!memcmp(&temp[i],(char *)"+CPIN:",6))
			{
				nextstep=true;
				break;
			}
			if (!memcmp(&temp[i],(char *)"+CME ERROR: 10",14))
			 {
			 	nextstep=false;
			 	break;
			 }

		}
		//if no response loop for 10 seconds
	}while((nextstep==0xFF) && (delta1MS(usWaittime)<(10000)));

	if (nextstep==true)
	{
		return 0; //success
	}
	else
	{
		return GPRS_ERROR;
	}
}
/*
 * \ brief Detects whether there is a SIM card inserted in the SIM slot under work
 * \ return 	0     :   SIM Card inserted
 *			 	Else:   SIM is absent
 */
unsigned char GPRS_isSimPresentCINTERION()
{
	char temp[200];
	int i, retry;
	unsigned char nextstep;
	unsigned short usWaittime;

	//GPRS_intializePort();

	nextstep=0xFF;
	retry=0;
	memset(temp,0,sizeof(temp));
	usWaittime =Get1MSTimer();

	SLEEP_M(200);
	GPRS_SendCommand((char *)"ATE\r\0",temp,retry);
	SLEEP_M(200);
	do{
		// Pin Configuration command
		GPRSWriteCom(COM1,(char *)"AT+CPIN?\r\0",9);
		SLEEP_M(3000);
		GPRS_SendCommand((char *)"AT+CPIN?\r\0",temp,retry);
		retry=0;
		for (i=0;i<sizeof(temp);i++)
		{
			if (!memcmp(&temp[i],(char *)"+CPIN:",6)) 
			{
				nextstep=true;
				break;
			}
			if (!memcmp(&temp[i],(char *)"+CME ERROR: 10",14))
			 {
			 	nextstep=false;
			 	break;
			 }

		}
		//if no response loop for 10 seconds
	}while((nextstep==0xFF) && (delta1MS(usWaittime)<(10000)));

	if (nextstep==true)
	{
		return 0; //success
	}
	else
	{
		return GPRS_ERROR;
	}

}
/*
 * \ brief
 * \param
 * \return
 */
unsigned char GPRS_isSimPresentWAVECOM()
{
	char temp[200];
	int i, retry;
	unsigned char nextstep;
	unsigned short usWaittime;

	nextstep=0xFF;
	retry=0;
	memset(temp,0,sizeof(temp));
	usWaittime =Get1MSTimer();

	SLEEP_M(200);
	GPRS_SendCommand((char *)"ATE\r\0",temp,retry);
	SLEEP_M(200);

	do{
		GPRSWriteCom(COM1,(char *)"AT+CPIN?\r\0",9);
		SLEEP_M(3000);
		GPRS_SendCommand((char *)"AT+CPIN?\r\0",temp,retry);
		retry=0;
		for (i=0;i<sizeof(temp);i++)
		{
			if (!memcmp(&temp[i],(char *)"+CPIN:",6))
			{
				nextstep=true;
				break;
			}
			if (!memcmp(&temp[i],(char *)"+CME ERROR: 10",14))
			{
				nextstep=false;
				break;
			}
//			if (!memcmp(&temp[i],(char *)"ERROR",5))
//			{
//				nextstep=false;
//				break;
//			}
		}
	}while((nextstep==0xFF) && (delta1MS(usWaittime)<(30000)));

	if (nextstep==true) 
	{
		return 0;
	}

	return GPRS_ERROR;
}
/*
 * \ brief Parent function Ping a remote host
 * \param
 * \return
 */
unsigned char GPRS_Ping(char *cpHost,
						int iRepeat,
						int iInterval,
						unsigned short usPackTimeOut,
						unsigned short usOpTimeOut)
{
	//check modem type
	if (ucGPRSModemType==WAVECOM)
	{
		return GPRS_PingWAVECOM(cpHost,iRepeat,iInterval,usPackTimeOut,usOpTimeOut);
	}
	else if (ucGPRSModemType==CINTERION)
	{
		return GPRS_PingCINTERION(cpHost,iRepeat,iInterval,usPackTimeOut,usOpTimeOut);
	}
}
/*
 * \ brief  No hardware checks required
 */
unsigned char GPRS_PingCINTERION(char *cpHost,
								 int iRepeat,
								 int iInterval,
								 unsigned short usPackTimeOut,
								 unsigned short usOpTimeOut)
{

	return 0;
}
/*
 * \ brief
 * \param
 * \return
 */
unsigned char GPRS_PingWAVECOM(char *cpHost,
							   int iRepeat,
							   int iInterval,
							   unsigned short usPackTimeOut,
							   unsigned short usOpTimeOut)
{
	char temp[300];
	int i, imax, j, k, no, size,retry;
	int tmp_COM1R_COUNT=0;
	UBYTE ch, tmpbuf[60];
	//GSMModemProcess=26;
	unsigned char nextstep;
	unsigned short usWaittime;


	nextstep=0xFF;
	retry=0;
	memset(temp,0,sizeof(temp));
	usWaittime =Get1MSTimer();
	do{
		sprintf(tmpbuf,(char *)"AT+WIPPING=%s,%d,%d,%d\r\0",cpHost,
															iRepeat,
															iInterval,
															usPackTimeOut);
		GPRS_SendCommand(tmpbuf,temp,retry);
		retry=1;
		for (i=0;i<sizeof(temp);i++)
		{
			if (!memcmp(&temp[i],(char *)"+WIPPING: 0",11)) 
			{
				nextstep=true;
				break;
			}
			if (!memcmp(&temp[i],(char *)"+CME ERROR:",11)) 
			{
				nextstep=false;
				break;
			}
			if (!memcmp(&temp[i],(char *)"ERROR:",5)) 
			{
				nextstep=false;
				break;
			}

		}
	}while((nextstep==0xFF) && (delta1MS(usWaittime)<usOpTimeOut));

	if (nextstep==true) 
	{
		return 0;
	}

	return GPRS_ERROR;

}
/*
 * \ brief Shows the signal strength indication on top of the LCD screen
 * \param [in] -iSigStrength value of the signal strength
 * \return     - void
 */
void GPRS_ShowSignal(int iSigStrength)
{
	switch(iSigStrength)
	{
	case 0:
		ICON_DISPLAY(1);
		break;
	case 1:
		ICON_DISPLAY(2);
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		ICON_DISPLAY(3);
		break;
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
		ICON_DISPLAY(4);
		break;
	case 31:
		ICON_DISPLAY(5);
		break;
	default:
		ICON_DISPLAY(5);
		break;

	}
}
/*
 * \ brief Sends a USSD message to the mobile operator
 * \ param [in] cpString- Command to operator
 * 		   [in/out]cpRetString- Returned value from Operator
 * 		   [in] usOpTimeOut- Delay in ms
 * 
 * \return   0x12     :   the operation succeeded
 * 			0x10     :   operation not supported
 * 			Else      :   no response from operator
 */
unsigned char GPRS_SendUSSD(char *cpString,char *cpRetString,unsigned short usOpTimeOut)
{
	char temp[300];
	int i, j,retry;
	UBYTE ch, tmpbuf[60];
	unsigned char nextstep;
	int index;
	unsigned short usWaittime;


	nextstep=0xFF;
	index=0;
	retry=0;
	memset(temp,0,sizeof(temp));
	usWaittime =Get1MSTimer();
	do{
		// allows to control the handling of Unstructured Supplementary Service Data
		sprintf(tmpbuf,(char *)"AT+CUSD=1,\"%s\"\r\0",cpString);
		//sprintf(tmpbuf,(char *)"AT+CPIN?\r\0");
		GPRS_SendCommand(tmpbuf,temp,retry);
		retry=1;
		for (i=0;i<sizeof(temp);i++)
		{
			if (!memcmp(&temp[i],(char *)"+CUSD: 4",8)) 
			{
				nextstep=0x10;
				break;
			}
			if (!memcmp(&temp[i],(char *)"+CUSD: 2",8)) 
			{
				nextstep=0x12;
				index=i;
				break;
			}
			if (!memcmp(&temp[i],(char *)"+CUSD: 0",8)) 
			{
				nextstep=0x12;
				index=i;
				break;
			}
			if (!memcmp(&temp[i],(char *)"+CUSD: 1",8)) 
			{
				nextstep=0x12;
				index=i;
				break;
			}

		}
	}while((nextstep==0xFF) && (delta1MS(usWaittime)<usOpTimeOut));

#ifdef USER_DEBUG
			Rprintf("\r\n A=%d %d\r\n",nextstep,index);
#endif
	memcpy(cpRetString,&temp[index+10],strlen(temp)-(index+10+1));
	if (nextstep==0x10) 
	{
		return Operation_NOT_SUPPORTED;
	}
	if (nextstep==0x12)
	{
		memcpy(cpRetString,&temp[index+10],strlen(temp)-(index+10+1));
		return Operation_SUPPORTED;
	}

	return GPRS_ERROR;

}

/*
 * \brief- Initialize the GPRS device, set network parameters and connect to GPRS network
 * \param
 * \return
 */
int GPRS_initializeCINTERION(char *cpAPN,char *cpUID, char *cpPASS,char *cpPinCode)
{
	char temp[30];
	int i, imax, j, k, no, size;
	int tmp_COM1R_COUNT=0;
	UBYTE ch, tmpbuf[32];
	GSMModemProcess=-3;

	GSMModemProcess++;

	while(1)
	{

		switch(GSMModemProcess)
		{

			case -2: // enforce echo message
					gsmDebugMessage("PROCESS -2");
					GSMCommandWrite("ATE1\r");
					GSMModemProcess++;
					break;
			case 0: // stop power saving and return to full functionality
					gsmDebugMessage("PROCESS 1");
					GSMCommandWrite("AT+CFUN=1\r");
					GSMModemProcess++;
					break;
			case 2:	/* Enable error result code with numeric values */
					gsmDebugMessage("PROCESS 2");
					GSMCommandWrite("AT+CMEE=1\r");
					GSMModemProcess++;
					break;
			case 4: //PIN authentication
					gsmDebugMessage("PROCESS 12");
					GSMCommandWrite("AT+CPIN?\r");
					GSMModemProcess++;
					break;
			case 6://PIN authentication
					gsmDebugMessage("PROCESS 14");
					sprintf(gsmTxStream, "AT+CPIN=%s\r", cpPinCode);
					GSMCommandWrite(gsmTxStream);
					GSMModemProcess++;
					break;
			case 8:	/* GPRS atteched */
					gsmResultMessage("ATTECHING NETWORK");
					gsmDebugMessage("PROCESS 16");
					GSMCommandWrite("AT^SICS=0,CONTYPE,GPRS0\r");
					GSMModemProcess++;
					break;
			case 10:
					gsmDebugMessage("PROCESS 18");
					GSMCommandWrite("AT^SICS=0,ALPHABET,1\r");
					GSMModemProcess++;
					break;
			case 12:
					gsmDebugMessage("PROCESS 18");
					GSMCommandWrite("AT^SICS=0,INACTTO,65535\r");
					GSMModemProcess++;
					break;
			case 14:
					gsmDebugMessage("PROCESS 22");
					sprintf(gsmTxStream, "AT^SICS=0,APN,\"%s\"\r", cpAPN);
				//                                           ^
				//                                           |------ GPRS host APN name
					GSMCommandWrite(gsmTxStream);	// Set APN of GPRS bearer
					GSMModemProcess++;
					break;
			case 16:
					gsmDebugMessage("PROCESS 24");
					if (strlen(cpUID)==1)
					{
						sprintf(gsmTxStream, "AT^SICS=0,USER,\"\"\r");
					}
					else
					{
						sprintf(gsmTxStream, "AT^SICS=0,USER,\"%s\"\r", cpUID);
					}
					GSMCommandWrite(gsmTxStream);	// Set user name of PPP login
					GSMModemProcess++;
					break;
			case 18:
					gsmDebugMessage("PROCESS 26");
					if (strlen(cpPASS)==1)
					{
						sprintf(gsmTxStream, "AT^SICS=0,PASSWD,\"\"\r");
					}
					else
					{
						sprintf(gsmTxStream, "AT^SICS=0,PASSWD,\"%s\"\r", cpPASS);
					}
					GSMCommandWrite(gsmTxStream);	// Set password for PPP login
					GSMModemProcess++;
					Start_GPRS_bearer_Retry = 30;
					Create_Retry = 10;
					break;
			case 20:
					gsmDebugMessage("PROCESS 28");
					GSMCommandWrite("AT^SISS=0,SRVTYPE,TRANSPARENT\r");
					GSMModemProcess++;
					break;
			case 22:
					gsmDebugMessage("PROCESS 18");
					GSMCommandWrite("AT^SISS=0,ALPHABET,1\r");
					GSMModemProcess++;
					break;
			case 24:
					gsmDebugMessage("PROCESS 18");
					GSMCommandWrite("AT^SISS=0,ADDRESS,\"4.2.2.2:80\"\r");
					GSMModemProcess++;
					break;
			case 26:
					gsmDebugMessage("PROCESS 18");
					GSMCommandWrite("AT^SISS=0,CONID,0\r");
					GSMModemProcess++;
					break;
			case 28:
					gsmDebugMessage("PROCESS 18");
					GSMCommandWrite("AT^SISO=0\r");
					GSMModemProcess++;
					TIME_BASE10 = 0x8000 | 6000;		// 60 SEC
					break;
			case 29:
					gsmDebugMessage("PROCESS 29");
					if (!(TIME_BASE10 & 0x8000))
					{
						Start_GPRS_bearer_Retry--;
						if (!Start_GPRS_bearer_Retry)
						{
							GSMModemProcess = 999;
						}
						else
						{
							GSMModemProcess = 28;
						}
					}
					break;
			case 30:
					gsmDebugMessage("PROCESS 18");
					GSMCommandWrite("AT^SICI=0\r");
					GSMModemProcess++;
					break;
			case 32:
					return GPRS_SUCCESS;
					break;
		}// end of switch GSMModemProcess

		usWaittime =Get1MSTimer();
		do
		{
	      if ((KEYSCAN() & 0xFF) == VK_CANCEL) return GPRS_CANCELED;
		}while((COM1R_COUNT==0) && (delta1MS(usWaittime)<GPRS_TIME_OUT));

		do
		{
			tmp_COM1R_COUNT=COM1R_COUNT;
			SLEEP_M(100);
		}while(tmp_COM1R_COUNT!=COM1R_COUNT);

		if (!COM1R_COUNT)
		{
			return GPRS_NORESP;
			continue;
		}

		for ( i = 0 ; i < COM1R_COUNT ; i++ )
		{

			ch = GPRSReadCom(COM1);
			if (gsmrxptr - &gsmRxStream[0] > 2048)
			{
				gsmrxptr = gsmRxStream;
			}
			*gsmrxptr++ = ch;

#ifdef USER_DEBUG
			Rprintf("%c", ch);
#endif
			size = gsmrxptr - &gsmRxStream[0];
			if ((size >= 2) && (*(gsmrxptr-2) == '\r') && (*(gsmrxptr-1) == '\n'))
			{

				memset(tmpbuf, 0, sizeof(tmpbuf));
				memcpy(tmpbuf, gsmRxStream, size - 2);
			    if (!strncmp(gsmrxptr-4, "OK", 2))
			    {
					gsmResultMessage("OK");
	    			GSMModemProcess++;
	    				//if (GSMModemProcess == 10) {
	    				//	GSMModemProcess = 12;
	    				//}

	    		}
			    else if (!strncmp(gsmrxptr-7, "ERROR", 5))
			    {
					gsmResultMessage("ERROR");
					return GPRS_ERROR;
	    			GSMModemProcess = 999;
				}
			    else if (!strncmp(gsmRxStream, "+CME ERROR: ", 12) ||
			    		 !strncmp(gsmRxStream, "+CMS ERROR: ", 12))
			    {
			    	gsmDebugMessage(gsmRxStream);
			    	return GPRS_CMEERROR;

					size = gsmrxptr - &gsmRxStream[12] - 2;
					memset(gsmTxStream, '\0', 41);
					strncpy(gsmTxStream, &gsmRxStream[12], size);
					gsmResultMessage(gsmTxStream);
					no = GetCMEError(gsmTxStream);
					switch( no )
					{
						case 515:
						case 527:
	    						GSMModemProcess--;
	    						break;
						case 3:
	//    						GSMModemProcess = 24;
	    						break;
	    				case 803:	// already GPRS opened
	    						GSMModemProcess = 30;
	    						break;
						case 812:
								TIME_BASE10 = 0x8000 | 100;		// 1 SEC
								break;
						case 831:
								if(GSMModemProcess == 34)
									GSMModemProcess = 33;								// 34
								break;
						case 10:
			    				GSMModemProcess = 999;
	    						break;
	    				default:
	//		    				GSMModemProcess = 999;
	    						break;
	    			}//end of switch case
	    		}//end of else if
			    else if (!strncmp(gsmRxStream, "+CREG: ", 7))
	    		{
					size = gsmrxptr - &gsmRxStream[7] - 2;
					memset(gsmTxStream, '\0', 5);
					strncpy(gsmTxStream, &gsmRxStream[7], size);
					gsmResultMessage(gsmTxStream);
					no = atoi(gsmTxStream);
					switch( no )
					{
	    				case 0:	/* not registered, ME is not currently searching for a new operator */
				    			GSMModemProcess = 12;
	    						break;
	    				case 1:	/* registered, home network. */
	//			    			GSMModemProcess = 12;
	    						break;
		    			case 2:	/* not registered, ME currently searching for a new operator to register to */
				    			GSMModemProcess = 12;
	    						break;
	    				case 5:	/* registered, roaming */
				    			GSMModemProcess = 12;
	    						break;
	    				case 3:	/* registration denird. */
	    				case 4:	/* unknown */
	    						break;
	    			}// end of switch case
				}//end of else if
			    else if (!strncmp(gsmrxptr-12, "NO CARRIER", 10))
			    {
			    	return GPRS_NOANSWER;
					gsmResultMessage("NO CARRIER");
		    		GSMModemProcess = 999;
	    		}
			    else if (!strncmp(gsmrxptr-11, "NO ANSWER", 9))
			    {
			    	return GPRS_NOANSWER;
					gsmResultMessage("NO ANSWER");
		    		GSMModemProcess = 999;
		    	}
			    else if (!strncmp(gsmrxptr-6, "BUSY", 4))
			    {
			    	return GPRS_NOANSWER;
					gsmResultMessage("BUSY");
		    		GSMModemProcess = 999;
				}
			    else if (!strncmp(gsmrxptr-14, "+CPIN: READY", 12))
			    {
					GSMModemProcess += 3;
				}
			    else if (!strncmp(gsmrxptr-16, "+CPIN: SIM PIN", 14))
			    {
					GSMModemProcess++;
		    	}
			    else if (!strncmp(gsmRxStream, "CONNECT", 7))
			    {
					gsmResultMessage("CONNECT");
					GSMModemProcess++;
				}
			    else if (!strncmp((char *)gsmRxStream, "+WIPREADY:", 10))
			    {
						//STRDISP(30);
			    	return GPRS_NOANSWER;
					Lprintf(1, 0, "+WIPREADY: 2,1");
					GSMModemProcess++;
				}
				gsmrxptr = gsmRxStream;
			}//end of if condition
		}// end of for loop
	}// end of while loop

	return GPRS_SUCCESS;
}
/*
 * \brief- Initialize the GPRS device, set network parameters and connect to GPRS network
 * \param
 * \return
 */

//int GPRS_initializeCINTERION(char *cpAPN,char *cpUID, char *cpPASS,char *cpPinCode)
//{
//	char temp[30];
//	int i, imax, j, k, no, size;
//	int tmp_COM1R_COUNT=0;
//	UBYTE ch, tmpbuf[32];
//	GSMModemProcess=-3;
//
//	GSMModemProcess++;
//
//	while(1)
//	{
//
//		switch(GSMModemProcess)
//		{
//
//			case -2: // enforce echo message
//					gsmDebugMessage("PROCESS -2");
//					GSMCommandWrite("ATE1\r");
//					GSMModemProcess++;
//					break;
//			case 0: // stop power saving and return to full functionality
//					gsmDebugMessage("PROCESS 1");
//					GSMCommandWrite("AT+CFUN=1\r");
//					GSMModemProcess++;
//					break;
//			case 2:	/* Enable error result code with numeric values */
//					gsmDebugMessage("PROCESS 2");
//					GSMCommandWrite("AT+CMEE=1\r");
//					GSMModemProcess++;
//					break;
//			case 4: //PIN authentication
//					gsmDebugMessage("PROCESS 12");
//					GSMCommandWrite("AT+CPIN?\r");
//					GSMModemProcess++;
//					break;
//			case 6://PIN authentication
//					gsmDebugMessage("PROCESS 14");
//					sprintf(gsmTxStream, "AT+CPIN=%s\r", cpPinCode);
//					GSMCommandWrite(gsmTxStream);
//					GSMModemProcess++;
//					break;
////			case 8:	/* GPRS atteched */
////					gsmResultMessage("ATTECHING NETWORK");
////					gsmDebugMessage("PROCESS 16");
////					GSMCommandWrite("AT^SICS=0,CONTYPE,GPRS0\r");
////					//GSMCommandWrite("AT+CREG?\r");
////					GSMModemProcess++;
////					break;
////			case 10:
////					gsmDebugMessage("PROCESS 18");
////					GSMCommandWrite("AT^SICS=0,ALPHABET,1\r");
////					//GSMCommandWrite("AT+CGACT?\r");
////					GSMModemProcess++;
////					break;
////			case 12:
////					gsmDebugMessage("PROCESS 18");
////					GSMCommandWrite("AT^SICS=0,\"INACTTO\",\"65535\"\r");
////					GSMModemProcess++;
////					break;
////			case 14:
////					gsmDebugMessage("PROCESS 22");
////					sprintf(gsmTxStream, "AT^SICS=0,APN,\"%s\"\r", cpAPN);
////				//                                           ^
////				//                                           |------ GPRS host APN name
////					GSMCommandWrite(gsmTxStream);	// Set APN of GPRS bearer
////					GSMModemProcess++;
////					break;
////			case 16:
////					gsmDebugMessage("PROCESS 24");
////					if (strlen(cpUID)==1)
////					{
////						sprintf(gsmTxStream, "AT^SICS=0,USER,\"\"\r");
////					}
////					else
////					{
////						sprintf(gsmTxStream, "AT^SICS=0,USER,\"%s\"\r", cpUID);
////					}
////					GSMCommandWrite(gsmTxStream);	// Set user name of PPP login
////					GSMModemProcess++;
////					break;
////			case 18:
////					gsmDebugMessage("PROCESS 26");
////					if (strlen(cpPASS)==1)
////					{
////						sprintf(gsmTxStream, "AT^SICS=0,PASSWD,\"\"\r");
////					}
////					else
////					{
////						sprintf(gsmTxStream, "AT^SICS=0,PASSWD,\"%s\"\r", cpPASS);
////					}
////					GSMCommandWrite(gsmTxStream);	// Set password for PPP login
////					GSMModemProcess++;
////					Start_GPRS_bearer_Retry = 30;
////					Create_Retry = 10;
////					break;
//
//			case 8:
//				gsmResultMessage("ATTECHING NETWORK");
//				gsmDebugMessage("PROCESS 16");
//				//GSMCommandWrite("AT^SICS=0,CONTYPE,GPRS0\r");
//				SLEEP_M(300);
//				sprintf(gsmTxStream,"AT+CGDCONT= 1,\"IP\",\"%s\",\"0.0.0.0\",0,0\r",cpAPN);
//				GSMCommandWrite(gsmTxStream);
//				GSMModemProcess+=3;
//				break;
////			case 10:
////				gsmResultMessage("ATTECHING NETWORK");
////				gsmDebugMessage("PROCESS 16");
////				//GSMCommandWrite("AT^SICS=0,CONTYPE,GPRS0\r");
////				GSMCommandWrite("AT+CGDCONT?\r");
//				break;
//			case 12:	/* GPRS atteched */
//					gsmResultMessage("ATTECHING NETWORK");
//					gsmDebugMessage("PROCESS 16");
//					//GSMCommandWrite("AT^SICS=0,CONTYPE,GPRS0\r");
//					GSMCommandWrite("AT+CREG?\r");
//					GSMModemProcess++;
//					break;
//			case 14:	/* GPRS atteched */
//					gsmResultMessage("ATTECHING NETWORK");
//					gsmDebugMessage("PROCESS 16");
//					//GSMCommandWrite("AT^SICS=0,CONTYPE,GPRS0\r");
//					GSMCommandWrite("AT+CGACT?\r");
//					GSMModemProcess++;
//					break;
//			case 16:
//					gsmDebugMessage("PROCESS 18");
//					//GSMCommandWrite("AT^SICS=0,ALPHABET,1\r");
//					GSMCommandWrite("AT+CGATT=1\r");
//					GSMModemProcess++;
//					break;
//			case 18:
//					gsmDebugMessage("PROCESS 18");
//					//GSMCommandWrite("AT^SICS=0,ALPHABET,1\r");
//					GSMCommandWrite("AT+CGACT=1,1\r");
//					SLEEP_M(300);
//					GSMModemProcess++;
//					break;
////			case 20:
////					gsmDebugMessage("PROCESS 28");
////					GSMCommandWrite("AT^SISS=0,SRVTYPE,TRANSPARENT\r");
////					GSMModemProcess++;
////					break;
////			case 22:
////					gsmDebugMessage("PROCESS 18");
////					GSMCommandWrite("AT^SISS=0,ALPHABET,1\r");
////					GSMModemProcess++;
////					break;
//////			case 24:
//////					gsmDebugMessage("PROCESS 18");
//////					GSMCommandWrite("AT^SISS=0,ADDRESS,\"4.2.2.2:80\"\r");
//////					GSMModemProcess++;
//////					break;
//////			case 26:
//////					gsmDebugMessage("PROCESS 18");
//////					GSMCommandWrite("AT^SISS=0,CONID,0\r");
//////					GSMModemProcess++;
//////					break;
//////			case 28:	//22/1/2014
//////					gsmDebugMessage("PROCESS 18");
//////					GSMCommandWrite("AT^SISO=0\r");
//////					GSMModemProcess++;
//////					break;
//			case 20:
//				Rprintf("+CMS ERRORrrrr\r\n");
//				SLEEP_M(300);
//				GPRS_ConnectCINTERION((char *)"41.38.99.162\0",(char *)"80\0",20000);
//				return 0;//GPRS_SUCCESS;
//					break;
//		}// end of switch GSMModemProcess
//
//		usWaittime =Get1MSTimer();
//		do
//		{
//
//		}while((COM1R_COUNT==0) && (delta1MS(usWaittime)<GPRS_TIME_OUT));
//
//		do
//		{
//			tmp_COM1R_COUNT=COM1R_COUNT;
//			SLEEP_M(100);
//		}while(tmp_COM1R_COUNT!=COM1R_COUNT);
//
//		if (!COM1R_COUNT)
//		{
//			return GPRS_NORESP;
//			continue;
//		}
//
//		for ( i = 0 ; i < COM1R_COUNT ; i++ )
//		{
//
//			ch = GPRSReadCom(COM1);
//			if (gsmrxptr - &gsmRxStream[0] > 2048)
//			{
//				gsmrxptr = gsmRxStream;
//			}
//			*gsmrxptr++ = ch;
//
//#ifdef USER_DEBUG
//			Rprintf("%c", ch);
//#endif
//			size = gsmrxptr - &gsmRxStream[0];
//			if ((size >= 2) && (*(gsmrxptr-2) == '\r') && (*(gsmrxptr-1) == '\n'))
//			{
//
//				memset(tmpbuf, 0, sizeof(tmpbuf));
//				memcpy(tmpbuf, gsmRxStream, size - 2);
//			    if (!strncmp(gsmrxptr-4, "OK", 2))
//			    {
//					gsmResultMessage("OK");
//	    			GSMModemProcess++;
//	    			Rprintf("caseok=%d\r\n",GSMModemProcess);
//	    				//if (GSMModemProcess == 10) {
//	    				//	GSMModemProcess = 12;
//	    				//}
//
//	    		}
//			    else if (!strncmp(gsmrxptr-7, "ERROR", 5))
//			    {
//					gsmResultMessage("ERROR");
//					return GPRS_ERROR;
//	    			GSMModemProcess = 999;
//				}
//			    else if (!strncmp(gsmRxStream, "+CME ERROR: ", 12) ||
//			    		 !strncmp(gsmRxStream, "+CMS ERROR: ", 12))
//			    {
//			    	gsmDebugMessage(gsmRxStream);
//			    	Rprintf("+CMS ERRORrrrr\r\n");
//			    	GSMModemProcess ++;
//			    	//return GPRS_CMEERROR;
//
////					size = gsmrxptr - &gsmRxStream[12] - 2;
////					memset(gsmTxStream, '\0', 41);
////					strncpy(gsmTxStream, &gsmRxStream[12], size);
////					gsmResultMessage(gsmTxStream);
////					no = GetCMEError(gsmTxStream);
////					switch( no )
////					{
////						case 515:
////						case 527:
////	    						GSMModemProcess--;
////	    						break;
////						case 3:
////	//    						GSMModemProcess = 24;
////	    						break;
////	    				case 803:	// already GPRS opened
////	    						GSMModemProcess = 30;
////	    						break;
////						case 812:
////								TIME_BASE10 = 0x8000 | 100;		// 1 SEC
////								break;
////						case 831:
////								if(GSMModemProcess == 34)
////									GSMModemProcess = 33;								// 34
////								break;
////						case 10:
////			    				GSMModemProcess = 999;
////	    						break;
////	    				default:
////	//		    				GSMModemProcess = 999;
////	    						break;
////	    			}//end of switch case
//	    		}//end of else if
//			    else if (!strncmp(gsmRxStream, "+CREG: ", 7))
//	    		{
//					size = gsmrxptr - &gsmRxStream[7] - 2;
//					memset(gsmTxStream, '\0', 5);
//					strncpy(gsmTxStream, &gsmRxStream[7], size);
//					gsmResultMessage(gsmTxStream);
//					no = atoi(gsmTxStream);
////					switch( no )
////					{
////	    				case 0:	/* not registered, ME is not currently searching for a new operator */
////				    			GSMModemProcess = 12;
////	    						break;
////	    				case 1:	/* registered, home network. */
////	//			    			GSMModemProcess = 12;
////	    					//GSMModemProcess++;
////	    					//Rprintf("case1=%d\r\n",GSMModemProcess);
////	    						break;
////		    			case 2:	/* not registered, ME currently searching for a new operator to register to */
////				    			//GSMModemProcess = 12;
////		    				//GSMModemProcess--;
////		    				//Rprintf("case2=%d\r\n",GSMModemProcess);
////	    						break;
////	    				case 5:	/* registered, roaming */
////				    			GSMModemProcess = 12;
////	    						break;
////	    				case 3:	/* registration denird. */
////	    				case 4:	/* unknown */
////	    						break;
////	    			}// end of switch case
////
//					GSMModemProcess ++;
//					}//end of else if
//			    else if (!strncmp(gsmrxptr-12, "NO CARRIER", 10))
//			    {
//			    	return GPRS_NOANSWER;
//					gsmResultMessage("NO CARRIER");
//		    		GSMModemProcess = 999;
//	    		}
//			    else if (!strncmp(gsmrxptr-11, "NO ANSWER", 9))
//			    {
//			    	return GPRS_NOANSWER;
//					gsmResultMessage("NO ANSWER");
//		    		GSMModemProcess = 999;
//		    	}
//			    else if (!strncmp(gsmrxptr-6, "BUSY", 4))
//			    {
//			    	return GPRS_NOANSWER;
//					gsmResultMessage("BUSY");
//		    		GSMModemProcess = 999;
//				}
//			    else if (!strncmp(gsmrxptr-14, "+CPIN: READY", 12))
//			    {
//
//					GSMModemProcess += 3;	//heba
////			    	GSMModemProcess ++;
////			    	Rprintf("case ready=%d\r\n",GSMModemProcess);
//				}
//			    else if (!strncmp(gsmrxptr-16, "+CPIN: SIM PIN", 14))
//			    {
//					GSMModemProcess++;
//		    	}
//			    else if (!strncmp(gsmRxStream, "CONNECT", 7))
//			    {
//					gsmResultMessage("CONNECT");
//					GSMModemProcess++;
//				}
//			    else if (!strncmp((char *)gsmRxStream, "+WIPREADY:", 10))
//			    {
//						//STRDISP(30);
//			    	return GPRS_NOANSWER;
//					Lprintf(1, 0, "+WIPREADY: 2,1");
//					GSMModemProcess++;
//				}
//
//			    else if (!strncmp(gsmrxptr-16, "+CGDCONT", 8))
//			    {
//			    	Rprintf("GSMModemProcess CONT=%d\r\n",GSMModemProcess);
//					GSMModemProcess++;
//		    	}
//				gsmrxptr = gsmRxStream;
//			}//end of if condition
//		}// end of for loop
//	}// end of while loop
//
//	return GPRS_SUCCESS;
//}
/*
 * \ brief Parent function to get SIM ID
 * \param
 * \return
 */
int GPRS_getSIMID(char *pcSIMID)
{
	//check modem type
	if (ucGPRSModemType==WAVECOM)
	{
		//old modem
		return GPRS_getSIMID_WAVECOM(pcSIMID);
	}
	else if (ucGPRSModemType==CINTERION)
	{
		//new modem
		return GPRS_getSIMID_CINTERION(pcSIMID);
	}
}
/*\
 * \brief- Get SIM ID with old modem
 * \param [in/out] pcSIMID - pointer to SIM ID 
 * \Return 0 - successful
 * 		   1 - Error
 */
int GPRS_getSIMID_WAVECOM(char *pcSIMID)
{
	char temp[50];
	int i,retry;
	unsigned short locationOfSIMID=0;
	unsigned char nextstep;
	unsigned short usWaittime;

	nextstep=0xFF;
	retry=0;
	memset(temp,0,sizeof(temp));
	usWaittime =Get1MSTimer();
	
	do{

		GPRS_SendCommand((char *)"AT+CCID\r\0",temp,retry);
		retry=1;
		//loop on the size of received buffer
		for (i=0;i<sizeof(temp);i++)
		{
			if (!memcmp(&temp[i],(char *)"+CCID:",6))
			 {
			 	nextstep=true;
			 	locationOfSIMID=i+8;
			 	break;
			 }

		}
	}while((nextstep==0xFF) && (delta1MS(usWaittime)<5000));

	if (nextstep==true)
	{
		memcpy(pcSIMID,&temp[locationOfSIMID],20);
		return 0;
	}
	//return error as SIMID not found
	return -1;

}
/*\
 * \brief- Get SIM ID with new modem
 * \param [in/out] pcSIMID - pointer to SIM ID 
 * \Return 0 - successful
 * 		   -1 - Error
 *  */
int GPRS_getSIMID_CINTERION(char *pcSIMID)
{
	
	char temp[50];
	unsigned short locationOfSIMID=0;
	int i,retry;
	unsigned short usWaittime;
	
	retry=0;
	memset(temp,0,sizeof(temp));
	usWaittime =Get1MSTimer();
	
	do{
		//send AT command to get SIM ID
		GPRS_SendCommand((char *)"AT^SCID\r\0",temp,retry);
		retry=1;
		
		//loop on the size of received buffer
		for (i=0;i<strlen(temp);i++)
		{
			//compare if "^SCID:"is received in the reponse
			if (!memcmp(&temp[i],(char *)"^SCID:",6))  
			{
				//location of SIM is the position of "^SCID:" + length of it= 7
				locationOfSIMID=i+7;
				//return 20 digit of SIM ID
				memcpy(pcSIMID,&temp[locationOfSIMID],20);
				//return successful
				return 0;
			}

		}
	}while((delta1MS(usWaittime)<5000)); // if not found loop for 5 seconds

	//return error as SIMID not found
	return -1;


}
/*\brief- Get Operator Name
 * \param [in/out] pcOPNAME - pointer to operator Name
 * \Return 0 - successful
 * 		   1 - Error
 *  */
int GPRS_getOperatorName(char *pcOPNAME)	//17/2/2014
{
	char temp[50];
	unsigned short locationOfOpName=0;
	int i,retry;
	unsigned short usWaittime;

	retry=0;
	memset(temp,0,sizeof(temp));
	usWaittime =Get1MSTimer();

	do{
		//send AT command to get SIM ID
		GPRS_SendCommand((char *)"AT+COPS?\r\0",temp,retry);
		retry=1;


		//loop on the size of received buffer
		for (i=0;i<strlen(temp);i++)
		{
			//compare if "+COPS:"is received in the reponse
			if (!memcmp(&temp[i],(char *)"+COPS:",6))
			{
				//location of Name is the position of "+COPS:" + length of it= 7
				locationOfOpName=i+12;
				//return operator name
				memcpy(pcOPNAME,&temp[locationOfOpName],strlen(strtok (&temp[locationOfOpName],"\"")));
				//return successful
				return 0;
			}

		}
	}while((delta1MS(usWaittime)<5000)); // if not found loop for 5 seconds

	//return error as SIMID not found
	return -1;
}
/*
 * \ brief parent function to get IMEI
 * \param
 * \return
 */
int GPRS_getProductIMEI(char *pcPIMEI)
{
	//check modem type
	if (ucGPRSModemType==WAVECOM)
	{
		//in case of old modem
		return GPRS_getProductIMEI_WAVECOM(pcPIMEI);
	}
	else if (ucGPRSModemType==CINTERION)
	{
		//in case of new modem
		return GPRS_getProductIMEI_CINTERION(pcPIMEI);
	}
}
/*
 * \brief- Get ProductIMEI using old modem
 * \param [in/out] pcPIMEI - pointer to IMEI  
 * \Return 0 - successful
 * 		   -1 - Error
 *  */
int GPRS_getProductIMEI_WAVECOM(char * pcPIMEI)
{
	char temp[50];
	int i,retry;
	unsigned char nextstep;
	unsigned short usWaittime;

	nextstep=0xFF;
	retry=0;
	memset(temp,0,sizeof(temp));
	usWaittime =Get1MSTimer();
	do{

		GPRS_SendCommand((char *)"AT+CGSN\r\0",temp,retry);
		retry=1;
		for (i=0;i<sizeof(temp);i++)
		{
			if (!memcmp(&temp[i],(char *)"+CGSN",5)) 
			{
				nextstep=true;
				break;
			}

		}
	}while((nextstep==0xFF) && (delta1MS(usWaittime)<5000));

	if (nextstep==true)
	{
		memcpy(pcPIMEI,&temp[10],15);
		return 0;
	}

	return -1;

}
/*
 * \brief- Get ProductIMEI using new modem
 * \param [in/out] pcPIMEI - pointer to IMEI  
 * \Return 0 - successful
 * 		   1 - Error
 *  */
int GPRS_getProductIMEI_CINTERION(char * pcPIMEI)
{
	char temp[50];
	int i,retry;
	unsigned char nextstep;
	unsigned short usWaittime;
	unsigned short locationOfSIMEI=0;

	nextstep=0xFF;
	retry=0;
	memset(temp,0,sizeof(temp));
	usWaittime =Get1MSTimer();
	do{

		GPRS_SendCommand((char *)"AT+CGSN\r\0",temp,retry);
		retry=1;
		for (i=0;i<sizeof(temp);i++)
		{
			if (!memcmp(&temp[i],(char *)"+CGSN",5))
			{
				nextstep=true;
				locationOfSIMEI=10;
				break;

			}
			if (!memcmp(&temp[i],(char *)"OK",2))
			{
				nextstep=true;
				locationOfSIMEI=2;
				break;
			}

		}
		// if not found loop for 5 seconds
	}while((nextstep==0xFF) && (delta1MS(usWaittime)<5000)); 

	if (nextstep==true)
	{
		//Length of IMEI is 15 digits
		memcpy(pcPIMEI,&temp[locationOfSIMEI],15);
		return 0;
	}

	return -1;

}

/********************************************************************************
 * 			SMS handling
 * ********************************************************************************
 */
int GPRS_sendSMSWAVECOM(char *pcTelephoneNo,char *pcMessage,unsigned short usTimeOut)
{
	return 0;
}
/*
 * \brief  Send SMS
 * \param [in] pcTelephoneNo - receipt Telephone number
 * 		  [in] pcMessage	 - Message body
 * 		  [in] usTimeOut 	 - Timeout in ms
 * \Return 		0 	: Success
 *        		Else : Error
 */
int GPRS_sendSMS(char *pcTelephoneNo,char *pcMessage,unsigned short usTimeOut)
{
	char temp[200];
	int i,retry;
	unsigned char nextstep;
	unsigned short usWaittime;

	nextstep=true;


	if (nextstep)
	{
		nextstep=0xFF;
		retry=0;
		memset(temp,0,sizeof(temp));
		//Set SMS text mode
		sprintf(temp, "AT+CMGF=1\r\0");
		usWaittime =Get1MSTimer();
		do{
			GPRS_SendCommand(temp,temp,retry);
			retry=1;
			for (i=0;i<sizeof(temp);i++)
			{
				if (!memcmp(&temp[i],(char *)"OK",2))
				 {
				 	nextstep=true;
				 	break;
				 }
				if (!memcmp(&temp[i],(char *)"ERROR",5))
				{
					nextstep=false;
					break;
				}
			}
		}while((nextstep==0xFF) && (delta1MS(usWaittime)<(10000)));
	}
	if (nextstep!=true)
	{
		return 1;
	}

	if (nextstep)
	{
		nextstep=0xFF;
		retry=0;
		memset(temp,0,sizeof(temp));
		//Set SMS text Mode Parameters
		sprintf(temp, "AT+CSMP=17,167,0,17\r\0");
		usWaittime =Get1MSTimer();
		do{
			GPRS_SendCommand(temp,temp,retry);
			retry=1;
			for (i=0;i<sizeof(temp);i++)
			{
				if (!memcmp(&temp[i],(char *)"OK",2))
				 {
				 	nextstep=true;
				 	break;
				 }
				if (!memcmp(&temp[i],(char *)"ERROR",5))
				{
					nextstep=false;
					break;
				}
			}
		}while((nextstep==0xFF) && (delta1MS(usWaittime)<(10000)));
	}

	if (nextstep!=true)
	{
		return 2;
	}

	if (nextstep)
	{
		nextstep=0xFF;
		retry=0;
		memset(temp,0,sizeof(temp));
		//Send SMS
		sprintf(temp, "AT+CMGS=\"%s\"\r\0",pcTelephoneNo);
		usWaittime =Get1MSTimer();
		do{
			GPRS_SendCommand(temp,temp,retry);
			retry=1;
			for (i=0;i<sizeof(temp);i++)
			{
				if (!memcmp(&temp[i],(char *)">",1))
				 {
				 	nextstep=true;
				 	break;
				 }
				if (!memcmp(&temp[i],(char *)"ERROR",5))
				{
					nextstep=false;
					break;
				}
			}
		}while((nextstep==0xFF) && (delta1MS(usWaittime)<(10000)));
	}
	if (nextstep!=true)
	{
		return 3;
	}
	if (nextstep)
	{
		nextstep=0xFF;
		retry=0;
		memset(temp,0,sizeof(temp));

		sprintf(temp, "%s\x1A\0",pcMessage);
		temp[strlen(temp)]=26;
		usWaittime =Get1MSTimer();
		do{
			GPRS_SendCommand(temp,temp,retry);
			retry=1;
			for (i=0;i<sizeof(temp);i++)
			{
				if (!memcmp(&temp[i],(char *)"+CMGS:",6))
				 {
				 	nextstep=true;
				 	break;
				 }
				if (!memcmp(&temp[i],(char *)"ERROR",5))
				{
					nextstep=false;
					break;
				}
			}
		}while((nextstep==0xFF) && (delta1MS(usWaittime)<(usTimeOut)));
	}
	if (nextstep!=true)
	{
		return 4;
	}

	return 0;
}
/*
 * \brief Get the toral number of available messages
 * 
 * \param  [in/out]piNumberOfAvailableMessages : Total of available messages
 * 
 * \Return 	0 	: Success
 * 			1   : Error
 */
int GPRS_AvailableSMS(int *piNumberOfAvailableMessages)
{
	char temp[100];
	int i,retry;
	unsigned char nextstep;
	unsigned short usWaittime;
	int locationOfCPMS;

	nextstep=true;
	if (nextstep)
	{
		nextstep=0xFF;
		retry=0;
		memset(temp,0,sizeof(temp));
		if (ucGPRSModemType==WAVECOM)
			sprintf(temp, "AT+CPMS=\"ME\"\r\0");
		else
			sprintf(temp, "AT+CPMS=\"MT\"\r\0");

		usWaittime =Get1MSTimer();
		do{
			GPRS_SendCommand(temp,temp,retry);
			retry=1;
			for (i=0;i<sizeof(temp);i++)
			{
				if (!memcmp(&temp[i],(char *)"OK",2))
				 {
				 	nextstep=true;
				 	break;
				 }
				if (!memcmp(&temp[i],(char *)"+CPMS:",6))
				 {
				 	nextstep=true;
				 	locationOfCPMS=i+7;
				 	break;
				 }
				if (!memcmp(&temp[i],(char *)"ERROR",5))
				{
					nextstep=false;
					break;
				}
			}
		}while((nextstep==0xFF) && (delta1MS(usWaittime)<(10000)));
	}
	if (nextstep!=true)
	{
		return 1;
	}

	*piNumberOfAvailableMessages=atoi(&temp[locationOfCPMS]);



	return 0;
}
/*
 * \brief 	Set Short Message Storage Sequence
 * \param 	[in] cSeqOrder- //'0' Modem first, SIM second
 *						    //'1' SIM First, Modem Second
 * \Return  0    : Success
 *          1    : Error
 */
int GPRS_SetSMSStorageSeq(char cSeqOrder)
{
	char temp[100];
	int i,retry;
	unsigned char nextstep;
	unsigned short usWaittime;

	if (ucGPRSModemType==WAVECOM)
	{
		 return 0;
	}

	nextstep=true;
	if (nextstep)
	{
		nextstep=0xFF;
		retry=0;
		memset(temp,0,sizeof(temp));
		sprintf(temp, "AT^SSMSS=%c\r\0",cSeqOrder);
		usWaittime =Get1MSTimer();
		do{
			GPRS_SendCommand(temp,temp,retry);
			retry=1;
			for (i=0;i<sizeof(temp);i++)
			{
				if (!memcmp(&temp[i],(char *)"OK",2))
				 {
				 	nextstep=true;
				 	break;
				 }
				if (!memcmp(&temp[i],(char *)"ERROR",5))
				{
					nextstep=false;
					break;
				}
			}
		}while((nextstep==0xFF) && (delta1MS(usWaittime)<(10000)));
	}
	if (nextstep!=true)
	{
		return 1;
	}


	return 0;
}
/*
 * \brief  Read the required message by Index number
 * 
 * \param [in] iIndex 		 - Index of the required message
 * 		  [in/out] pcMessage - Message body
 * 
 * \Return   0 	  : Success
 * 			 Else : Error
 */
int GPRS_ReadSMSByIndex(int iIndex,char *pcMessage)
{
	char temp[1000];
	int i,retry;
	unsigned char nextstep;
	unsigned short usWaittime;
	int locationOfCMGR;

	nextstep=true;
	if (nextstep)
	{
		nextstep=0xFF;
		retry=0;
		memset(temp,0,sizeof(temp));
		// Read SMS messages
		sprintf(temp, "AT+CMGR=%d\r\0",iIndex);
		usWaittime =Get1MSTimer();
		do{
			GPRS_SendCommand(temp,temp,retry);
			retry=1;
			for (i=0;i<sizeof(temp);i++)
			{
				if (!memcmp(&temp[i],(char *)"+CMGR:",6))
				 {
					if (!memcmp(&temp[i],(char *)"+CMGR: 0,,0",11))
					{
						 return (10);
					}
				 	nextstep=true;
				 	locationOfCMGR=i+7;
				 	break;
				 }
				if (!memcmp(&temp[i],(char *)"ERROR",5))
				{
					nextstep=false;
					break;
				}
				if (!memcmp(&temp[i],(char *)"OK",2))
				{
					nextstep=false;
					return 2;
					break;
				}
			}
		}while((nextstep==0xFF) && (delta1MS(usWaittime)<(10000)));
	}
	if (nextstep!=true)
	{
		return 2;
	}

	memcpy(pcMessage,&temp[locationOfCMGR],strlen(temp)-locationOfCMGR);


	return 0;

}
/*
 *\brief  Read the required message by Phone number
 * 
 *\param [in] pcPhoneNumber: must be null terminated character string
 * 		 [in/out] pcMessage - Message body
 *       [in/out] piIndex	- Index of the message
 * 		 [in] cDelete		- 1: delete message
 * 		 [in] usTimeOut 	- Timeout in ms
 *
 * \return  0 	 : Success
 * 			Else : Error
 */
int GPRS_ReadSMSByPhNo(char * pcPhoneNumber,
					   char *pcMessage,
					   int *piIndex,
					   char cDelete,
					   unsigned short usTimeOut)
{
	char temp[1000];
	int i,retry,iIndex,j;
	unsigned char nextstep;
	unsigned short usWaittime;
	int locationOfCMGR;
	char cFound;
	usWaittime =Get1MSTimer();

	cFound=0;
	nextstep=true;
	for (iIndex=1;iIndex<100;iIndex++)
	{
		if (delta1MS(usWaittime)>(usTimeOut)) 
		{
			return 3;
		}
		if (nextstep)
		{
			nextstep=0xFF;
			retry=0;
			memset(temp,0,sizeof(temp));
			//read message
			sprintf(temp, "AT+CMGR=%d\r\0",iIndex);
			do{
				GPRS_SendCommand(temp,temp,retry);
				retry=1;
				for (i=0;i<sizeof(temp);i++)
				{
					if (!memcmp(&temp[i],(char *)"+CMGR:",6))
					 {
					 	//find telephone number in message body
						j=findExpInStr(temp,pcPhoneNumber,strlen(pcPhoneNumber));
						if (j>0)//found
						{
							j=findExpInStr(&temp[i],"\r\n",2);
							//get the message
							memcpy(pcMessage,&temp[j+i+2],strlen(temp)-(j+i+2));
							*piIndex=iIndex;
							if (cDelete==1)
							{
								//TODO check the return type of the function
								 GPRS_DeleteSMS(iIndex);
							}
							return 0;
						}
						nextstep=true;
						break;
					 }
					if (!memcmp(&temp[i],(char *)"ERROR",5))
					{
						nextstep=false;
						break;
					}
					if (!memcmp(&temp[i],(char *)"OK",2))
					{
						nextstep=true;
						break;
					}
				}
			}while((nextstep==0xFF) && (delta1MS(usWaittime)<(usTimeOut)));
		}
		if (nextstep!=true)
		{
			return 2;
		}
	}

	return 0;

}
/*
 * \brief Delete message by Index number
 * 
 * \param [in]	 iIndex- Index number of the message
 * 
 * \Return   0 	 : Success
 * 			 1   : Error
 */
int GPRS_DeleteSMS(int iIndex)
{
	char temp[100];
	int i,retry;
	unsigned char nextstep;
	unsigned short usWaittime;
	int locationOfCMGR;

	nextstep=true;
	if (nextstep)
	{
		nextstep=0xFF;
		retry=0;
		memset(temp,0,sizeof(temp));
		//delete message
		sprintf(temp, "AT+CMGD=%d\r\0",iIndex);
		usWaittime =Get1MSTimer();
		do{
			GPRS_SendCommand(temp,temp,retry);
			retry=1;
			for (i=0;i<sizeof(temp);i++)
			{
				if (!memcmp(&temp[i],(char *)"OK",2))
				 {
				 	nextstep=true;
				 	locationOfCMGR=i+7;
				 	break;
				 }
				if (!memcmp(&temp[i],(char *)"ERROR",5))
				{
					nextstep=false;
					break;
				}
			}
		}while((nextstep==0xFF) && (delta1MS(usWaittime)<(10000)));
	}
	if (nextstep!=true)
	{
		return 1;
	}

	return 0;

}

int findExpInStr(char *Str, char *Exp, unsigned long ExpLen)
{
	char * StrPointer=Str;
	int TotalLen=strlen(Str);
	int i;
	for (i=0;i<TotalLen;i++)
	{
		if (!memcmp(StrPointer,Exp,ExpLen)) 
		{
			return i;
		}
		StrPointer++;
	}
	return -1;


}
/*
 * \brief delete existed messages
 * 
 * \param No param
 * 
 * \Return   0 	 : Success
 * 			 1   : Error
 */
int GPRS_DeleteSMSInbox()
{
	char temp[1000];
	int i,retry,iIndex,j;
	unsigned char nextstep;
	unsigned short usWaittime;
	int locationOfCMGR;
	char cFound;
	usWaittime =Get1MSTimer();

	cFound=0;
	nextstep=true;
	for (iIndex=1;iIndex<100;iIndex++)
	{
		if (nextstep)
		{
			nextstep=0xFF;
			retry=0;
			memset(temp,0,sizeof(temp));
			sprintf(temp, "AT+CMGD=%d\r\0",iIndex);
			do{
				GPRS_SendCommand(temp,temp,retry);
				retry=1;
				for (i=0;i<sizeof(temp);i++)
				{
					if (!memcmp(&temp[i],(char *)"OK",2))
					 {
						nextstep=true;
						break;
					 }
					if (!memcmp(&temp[i],(char *)"ERROR",5))
					{
						nextstep=false;
						break;
					}
				}
			}while((nextstep==0xFF) && (delta1MS(usWaittime)<(10000)));
		}
		if (nextstep!=true)
		{
			return 1;
		}
	}

	return 0;

}
/*
 * \brief  Initialize memory storage to be used for reading and writing writing
 * \param   No param
 * \Return  0    : Success
 *          1    : Error
 */
int GPRS_initializeSMSStorageWAVECOM()
{
	char temp[100];
	int i,retry;
	unsigned char nextstep;
	unsigned short usWaittime;
	int locationOfCMGR;

	nextstep=0xFF;

	do{
		GPRSWriteCom(COM1,(char *)"AT+CPIN?\r\0",9);
		SLEEP_M(3000);
		GPRS_SendCommand((char *)"AT+CPIN?\r\0",temp,retry);
		retry=0;
		for (i=0;i<sizeof(temp);i++)
		{
			if (!memcmp(&temp[i],(char *)"+CPIN:",6))
			{
				nextstep=true;
				break;
			}
			if (!memcmp(&temp[i],(char *)"+CME ERROR: 10",14))
			{
				nextstep=false;
				break;
			}

		}
	}while((nextstep==0xFF) && (delta1MS(usWaittime)<(30000)));

	if (nextstep)
	{
		nextstep=0xFF;
		retry=0;
		memset(temp,0,sizeof(temp));
		usWaittime =Get1MSTimer();
		do{
			sprintf(temp, "AT+CPMS=\"ME\",\"ME\",\"ME\"\r\0",temp,0);
			GPRS_SendCommand(temp,temp,retry);
			retry=0;
			for (i=0;i<sizeof(temp);i++)
			{
				if (!memcmp(&temp[i],(char *)"OK",2))
				 {
				 	nextstep=true;
				 	break;
				 }
				if (!memcmp(&temp[i],(char *)"“+CME ERROR: 310",15))
				 {
				 	nextstep=false;
				 	break;
				 }
			}
		}while((nextstep==0xFF) && (delta1MS(usWaittime)<(20000)));
	}
	if (nextstep!=true)
	{
		return 1;
	}

	return 0;

}
/*
 * \brief  Initialize memory storage to be used for reading and writing writing
 * \param   No param
 * \Return  0    : Success
 *          1    : Error
 */
int GPRS_initializeSMSStorageCINTRION()
{
	char temp[100];
	int i,retry;
	unsigned char nextstep;
	unsigned short usWaittime;
	int locationOfCMGR;

	nextstep=0xFF;

	do{
		GPRSWriteCom(COM1,(char *)"AT+CPIN?\r\0",9);
		SLEEP_M(3000);
		GPRS_SendCommand((char *)"AT+CPIN?\r\0",temp,retry);
		retry=0;
		for (i=0;i<sizeof(temp);i++)
		{
			if (!memcmp(&temp[i],(char *)"+CPIN:",6))
			{
				nextstep=true;
				break;
			}
			if (!memcmp(&temp[i],(char *)"+CME ERROR: 10",14))
			{
				nextstep=false;
				break;
			}


		}
	}while((nextstep==0xFF) && (delta1MS(usWaittime)<(30000)));

	if (nextstep)
	{
		nextstep=0xFF;
		retry=0;
		memset(temp,0,sizeof(temp));
		usWaittime =Get1MSTimer();
		do{
			sprintf(temp, "AT+CPMS=\"MT\",\"MT\",\"MT\"\r\0",temp,0);
			GPRS_SendCommand(temp,temp,retry);
			retry=0;
			for (i=0;i<sizeof(temp);i++)
			{
				if (!memcmp(&temp[i],(char *)"OK",2))
				 {
				 	nextstep=true;
				 	break;
				 }
				if (!memcmp(&temp[i],(char *)"“+CME ERROR: 310",15))
				 {
				 	nextstep=false;
				 	break;
				 }
			}
		}while((nextstep==0xFF) && (delta1MS(usWaittime)<(20000)));
	}
	if (nextstep!=true)
	{
		return 1;
	}

	return 0;

}
/////////////////////////////////////////////////////////
int GPRS_initializeSMS()
{
	char temp[100];

	GPRS_SendCommand((char *)"AT+CMGF=1\r\0",temp,0);
	
	if (ucGPRSModemType==CINTERION)
	{
		GPRS_initializeSMSStorageCINTRION();
	}
	else
	{
		GPRS_initializeSMSStorageWAVECOM();
	}

	return 0;
}
