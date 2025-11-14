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
#include "iso8583.h"
#include "AppGlobal.h"
#include "../../BF_SDK/include/global.h"
#include "./resources/Msgs.h"
#include "SndRcvIso8583.h"
/******************************************************************************/
/* TYPES                                                                      */
/******************************************************************************/
/******************************************************************************/
/* CONSTANTS, MACROS                                                          */
/******************************************************************************/
#define DEV_DEBUG  0
#define USER_DEBUG 1

/******************************************************************************/
/* PRIVATE VARIABLES                                                          */
/******************************************************************************/
static unsigned char apn[50];
static unsigned char userName[50];
static unsigned char password[50];
unsigned short   usCommHdrIdx;

int  			LeftTrN;
int  			MaxTrN;
/******************************************************************************/
/* PRIVATE FUNCTIONS PROTOTYPES                                               */
/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/
int UTIL_WaitKey( void )
{
  int key;

  while(1) 
  {
    key = KEYSCAN() & 0xff;
    if ((key == VK_ENTER)||(key == VK_CANCEL)) 
      break;
  }

  return (key);
}

uchar PortConfig(uchar bIsDisp){
  int iResult;

  if (bIsDisp){
	ClearLCD();
	Lprint_n(2, 0, 16,"Configure Port         ");
  }
  iResult=GPRS_intializePort();
  DBGPrt_m("GPRS_intializePort=%d\r\n ",iResult);
  
  if (iResult!=GPRS_SUCCESS)
  {
    switch (iResult)
    {
    case GPRS_NOTFOUND:
      ClearLCD();
      Lprint_n(2, 0, 16,"NO GPRS Modem             ");
      Lprint_n(3, 0, 16,"Cancel or Enter");
      WaitKeyPress10MS(1000);
      break;
    case GPRS_PORTFAILED:
      ClearLCD();
      Lprint_n(2, 0, 16,"GPRS PORT Failed                 ");
      Lprint_n(3, 0, 16,"Cancel or Enter");
      WaitKeyPress10MS(1000);
      break;
    }
  } else {
	  if (bIsDisp)
		Lprint_n(2, 0, 16, (char *)"Port Configured            ");
    DBGPrt_m("Port Configured \r\n");
  }
  return iResult;

}
uchar SIM_Selection(ushort usHdrIdx){
  uchar ucResult;

  if (usHdrIdx > 0 )
	DisplayMsgBox(usHdrIdx, S_SLCT_SIM1,0, 0);

  ucResult=GPRS_isSimPresent(0);

  if (ucResult==0){
	DBGPrt_m("SIM 1 Present \r\n");

	memcpy(apn,DataSave1->udtConnSettings.aucSIM1APN,strlen(DataSave1->udtConnSettings.aucSIM1APN));
	memcpy(userName,DataSave1->udtConnSettings.aucSIM1UsrName,strlen(DataSave1->udtConnSettings.aucSIM1UsrName));
	memcpy(password,DataSave1->udtConnSettings.aucSIM1Password,strlen(DataSave1->udtConnSettings.aucSIM1Password));
  } else if (ucResult== GPRS_CANCELED) {}
  else {
	if (usHdrIdx > 0 )
	  DisplayMsgBox(usHdrIdx, S_SLCT_SIM2,0, 0);
	ucResult=GPRS_isSimPresent(1);
	if (ucResult==0) {
	  DBGPrt_m("SIM 2 Present \r\n");
	  memcpy(apn,DataSave1->udtConnSettings.aucSIM2APN,strlen(DataSave1->udtConnSettings.aucSIM2APN));
	  memcpy(userName,DataSave1->udtConnSettings.aucSIM2UsrName,strlen(DataSave1->udtConnSettings.aucSIM2UsrName));
	  memcpy(password,DataSave1->udtConnSettings.aucSIM2Password,strlen(DataSave1->udtConnSettings.aucSIM2Password));

	} else if (ucResult== GPRS_CANCELED) {}
	else {
	  DisplayMsgBox(usHdrIdx, S_NO_SIM,0, 0);
	  DBGPrt_m("NO SIM \r\n");
	  WaitKeyPress10MS(400);
	}
  }
  return ucResult;
}

int GPRSInit (ushort usHdrIdx){
  int iResult;
  unsigned short displayedMsgIdx;
  uchar *aucdlgBtns[2];
  int rv=-2;

  DisplayMsgBox(usHdrIdx, S_GPRS_INIT,0, 0);

#if DEV_DEBUG
    DBGPrt_m("APN=%s\r\n", apn); 
    DBGPrt_m("userName1=%s\r\n", userName); 
    DBGPrt_m("password1=%s\r\n", password); 
#endif

  while ((iResult=GPRS_initialize(apn, userName, password, "") != GPRS_SUCCESS)){
    //DisplayMsgBox(usHdrIdx, S_GPRS_ERROR,0, 0);
    switch (iResult){
      case GPRS_NORESP:
       DBGPrt_m("MODEM NO RESP\r\n");
       displayedMsgIdx = S_MDM_NO_RSPO_ERR;
       break;
      case GPRS_ERROR:
        DBGPrt_m("MODEM ERROR\r\n");
        displayedMsgIdx = S_MDM_ERR;
        break;
      case GPRS_CMEERROR:
        DBGPrt_m("MODEM CM ERROR\r\n");
        displayedMsgIdx = S_MDM_CM_ERR;
        break;
      case GPRS_NOANSWER:
        DBGPrt_m("GPRS No Connection\r\n");
        displayedMsgIdx = S_MDM_NO_CONN_ERR;
        break;
    }
    aucdlgBtns[0]=GetMsgs(S_YS);
    aucdlgBtns[1]=GetMsgs(S_NO);
    rv = DispModalDialog(S_GPRS_ERROR, S_RETRY_Q, "", 50000, aucdlgBtns, 2, 1); 
    if (rv == 0) {
      DisplayMsgBox(usHdrIdx, S_GPRS_INIT,0, 0);
    } else if (rv == 1) {
      break;
    }

  }
  if (iResult == GPRS_SUCCESS) {
    DBGPrt_m("GPRS Connected \r\n");
  }
  return iResult;
}
int GPRSInitRegConnect(ushort usHdrIdx){
  int iResult = 0;

  iResult = SIM_Selection(usHdrIdx);
  if (iResult == 0){
    iResult = GPRSInit(usHdrIdx);
    if (iResult == GPRS_SUCCESS){
      iSgnlStrength= GPRS_getSignalStrength();
    }
  }

  return iResult;
}

int GPRSConnectionDemo(void)
{

  int i,j;
  int iResult;
  int iRecvLen;
  char temp[20];
  uchar ucResult=0xFF;
 iResult=0xFF;

unsigned char apn[50];
unsigned char userName[50];
unsigned char password[50];
unsigned char server_IP[16];
unsigned char server_Port[6];


memset(apn, 0, sizeof(apn));
memset(userName, 0, sizeof(userName));
memset(password, 0, sizeof(password));
memset(server_IP, 0, sizeof(server_IP));
memset(server_Port, 0, sizeof(server_Port));

/******************************************************************************
 *               check mandatory parameters
 * 
 * **************************************************************************
 */  
#if DEV_DEBUG
DBGPrt_m("APN1=%s\r\n", DataSave1->udtConnSettings.aucSIM1APN); 
DBGPrt_m("userName1=%s\r\n", DataSave1->udtConnSettings.aucSIM1UsrName); 
DBGPrt_m("password1=%s\r\n", DataSave1->udtConnSettings.aucSIM1Password); 
DBGPrt_m("APN2=%s\r\n", DataSave1->udtConnSettings.aucSIM2APN); 
DBGPrt_m("userName2=%s\r\n", DataSave1->udtConnSettings.aucSIM2UsrName); 
DBGPrt_m("password2=%s\r\n", DataSave1->udtConnSettings.aucSIM2Password); 
DBGPrt_m("server_IP=%s\r\n", DataSave1->udtConnSettings.aucServerIP); 
DBGPrt_m("server_Port=%s\r\n", DataSave1->udtConnSettings.aucServerPort); 


// if((strlen(apn)==0 )||
//   (strlen(userName)==0)||
//   (strlen(password)==0)||
//   (strlen(server_IP)==0)||
//   (strlen(server_Port)==0))
//{
//   DisplayMessageBox(GetMessage(M_PLEASE),"Set GPRS Parameters",2000,1 );
//   return;
//}
#endif  

/******************************************************************************
 *               Port configuration
 * 
 * **************************************************************************
 */

/******************************************************************************
 *               SIM Selection
 * 
 * **************************************************************************
 * */
/***************************************************************************
 *                Get SIM ID and IMEI
 * ***************************************************************************
 * */
#if 1
    char pcSIMID[50]; 
    memset(pcSIMID, 0, sizeof(pcSIMID)); 
    if (GPRS_getSIMID(pcSIMID) == 0) {
      DBGPrt_m("SIM ID =%s\r\n",pcSIMID);
      //Lprint_n(1, 0, 16, "SIM ID");
      //Lprint_n(2, 0, 16,pcSIMID);
      //if(UTIL_WaitKey()==VK_CANCEL)
      //  return;
    }
    
    char pcIMEI[50]; 
    memset(pcIMEI, 0, sizeof(pcIMEI)); 
    
    if (GPRS_getProductIMEI(pcIMEI) == 0) {
      DBGPrt_m("IMEI ID =%s\r\n",pcIMEI);

      //ClearLCD();
      //Lprint_n(1, 0, 16, "IMEI");
      //Lprint_n(2, 0, 16,pcIMEI);
      //Lprint_n(3, 0, 16,"Cancel or Enter");
      //if(UTIL_WaitKey()==VK_CANCEL)
      //  return;
      
    }
#endif  
/******************************************************************************
 *                USSD
 *   \brief: The function is taking command based on the operator. below command is
 *         supported by vodafone
 * 
 * **************************************************************************
 * */
//
//  ClearLCD();
//  Lprint_n(2, 0, 16,"sending.. *100#                  ");
//
//  char USSDReply[50];
//  memset(USSDReply,0,50);
//  ucResult=GPRS_SendUSSD((char *)"*100#", USSDReply,5000);
//  if (ucResult==0x12)
//      {
//        ClearLCD();
//        Lprint_n(0, 0, 16, USSDReply);
//        Lprint_n(1, 0, 16, &USSDReply[16]);
//        Lprint_n(2, 0, 16, &USSDReply[32]);
//
//      }
//    else if (ucResult==0x10)
//    {
//        Lprint_n(2, 0, 16, "Operation not supported");
//    }
//    else
//    {
//        Lprint_n(2, 0, 16, "USSD: No reply");
//    }
//
//
//  Lprint_n(3, 0, 16,"Cancel or Enter");
//  if(UTIL_WaitKey()==VK_CANCEL)
//    return;
  
/******************************************************************************
 *               
 * 
 * **************************************************************************
 * */

/******************************************************************************
 *               Server connection
 * 
 * **************************************************************************
 * */

// ClearLCD();
//   Lprint_n(2, 0, 16,"Connect to server       ");
//
//   DBGPrt_m("IP= %s \r\n",DataSave1->udtConnSettings.aucServerIP);
//   DBGPrt_m("port= %s \r\n",DataSave1->udtConnSettings.aucServerPort);
//
// //if (GPRS_Connect((char *)"41.38.40.72",(char *)"3306",120000)!=GPRS_SUCCESS)
// if ((iResult = GPRS_Connect(DataSave1->udtConnSettings.aucServerIP,DataSave1->udtConnSettings.aucServerPort,20000))!=GPRS_SUCCESS)
// {
//   sprintf(temp, "Not Connected            ");
//   Lprint_n(2, 0, 16, temp);
//   //Lprint_n(3, 0, 16,"Cancel or Enter");
//   //if(UTIL_WaitKey()==VK_CANCEL)
//       WaitKeyPress10MS(1000);
//       return iResult;
// }
// else
// {
//   sprintf(temp, "Connected            ");
//   Lprint_n(2, 0, 16, temp);
//   Lprint_n(3, 0, 16,"Cancel or Enter");
//   
//   if(UTIL_WaitKey()==VK_CANCEL)
//     return iResult;
//   
// }
/******************************************************************
 *   Send and receive data are depending on your host.
 *   Below code is just an example 
 * ********************************************************************
 */

//  unsigned char aucCommuBuf[2048];
//  unsigned char aucRecvBuf [2048];
//  int K,M;
//  int recvLen=0;
//  int iReceived;
//  int iExpectedLength;
//  
//  for (K=0;K<100;K++)
//  {
//    Wait1MS(100);
//    memset(aucRecvBuf,0,sizeof(aucRecvBuf));
//    memset(aucCommuBuf,K,sizeof(aucCommuBuf));
//    aucCommuBuf[0]=0x08;
//    aucCommuBuf[1]=0x00; //length of data sent 2048 0x0800
//    //iResult=GPRS_SendData(aucCommuBuf,2048);
//    Lprint_n(3, 0, 16,"Send             ");
//    iReceived=0;
//    iExpectedLength=2048;
//    do
//    {
//      iResult=GPRS_ReceiveData(&aucRecvBuf[iReceived],&recvLen,10000);
//      DBGPrt_m("rceived lenght=%d",recvLen);
//      if (iResult==GPRS_SUCCESS) 
//      {
//        iReceived=iReceived+recvLen; 
//      }
//    }while(iReceived<iExpectedLength);
//    if (iResult==GPRS_SUCCESS)
//    {
//      ClearLCD();
//      sprintf(temp, "Round: %d",K);
//      Lprint_n(1, 0, 16, temp);
//      sprintf(temp, "recvLen: %d",iReceived);
//      Lprint_n(2, 0, 16, temp);
//    }
//    else
//    {
//      ClearLCD();
//      sprintf(temp, "receive failed            \0");
//      Lprint_n(2, 0, 16, temp);
//      UTIL_WaitKey();
//    }
//    for (M=0;M<sizeof(aucCommuBuf);M++) 
//    {
//      aucCommuBuf[M]=aucCommuBuf[M]+1;
//    }
//    if (memcmp(aucCommuBuf,aucRecvBuf,sizeof(aucRecvBuf)))
//    {
//      sprintf(temp, "Error Loop %d",K);
//      Lprint_n(3, 0, 16, temp);
//      UTIL_WaitKey();
//      K=100;
//    }
//  }

  /***********************************************************
   * Disconnect the connection with server
   * *************************************************************
   */

  //iResult=GPRS_DisConnect();
  //if (iResult== GPRS_SUCCESS  )
  //{
  //  Lprint_n(2, 0,16, "Disconnected            ");
  //}
  //else
  //{
  //  Lprint_n(2, 0,16, "Disconnect Failed             ");
  //}
    //
    //WaitKeyPress10MS(1000);  

}
/******************************************************************************/
/* EXPORTED FUNCTIONS                                                         */
/******************************************************************************/
unsigned char COMMS_Init()
{
	if (!udtBrdRunData.udtGPRSSetting.bIsPortConfig)
	  if (PortConfig(1) == GPRS_SUCCESS){udtBrdRunData.udtGPRSSetting.bIsPortConfig = 1;}
	  else {return CONNECT_ERR;}
	if (!udtBrdRunData.udtGPRSSetting.bIsSimSelected)
	  if (SIM_Selection(S_PLZ_WAIT)== GPRS_SUCCESS){udtBrdRunData.udtGPRSSetting.bIsSimSelected = 1;}
	  else {return CONNECT_ERR;}
	if (!udtBrdRunData.udtGPRSSetting.bIsGPRSConnected)
	  if (GPRSInit(S_PLZ_WAIT)== GPRS_SUCCESS){udtBrdRunData.udtGPRSSetting.bIsGPRSConnected = 1;}
	  else {return CONNECT_ERR;}
    return SUCCESS;
}
unsigned char COMMS_Connect(char *cpIP,char *cpPORT,unsigned short usTimeOut)
{
	uchar ucResult;

	if ((ucResult = COMMS_Init()) != SUCCESS){ return ucResult; }

    iSgnlStrength= GPRS_getSignalStrength();
    DisplayMsgBox(usCommHdrIdx, S_TRY_CNCT,0, 0);
    ucResult = GPRS_Connect(cpIP, cpPORT, usTimeOut);
    if (ucResult == GPRS_SUCCESS){
      DisplayMsgBox(usCommHdrIdx, S_CNCT, 0, 0);
    } else {
      DisplayMsgBox(usCommHdrIdx, S_NOT_CNCT,1000, 0);
      return CONNECT_ERR;
    }
    return SUCCESS;
}

uchar gethostbyname_m(const char* hostname, char* IP ){
  /* RFC 1035*/
  BYTE DNSQuery[200];
  char bhostname[200];
  uchar ucResult;
  BYTE SndRcvBuf[1000];
  bool m_bIsConnected;

  GET_TRUE_RANDOM_NUMBER();
  memcpy(&DNSQuery[2],RANDOM_NUMBER,2);// Transaction ID
/*  Flags: 0x0100 Standard query
      0... .... .... .... = Response: Message is a query
      .000 0... .... .... = Opcode: Standard query (0)
      .... ..0. .... .... = Truncated: Message is not truncated
      .... ...1 .... .... = Recursion desired: Do query recursively
      .... .... .0.. .... = Z: reserved (0)
      .... .... ...0 .... = Non-authenticated data: Unacceptable*/
  memcpy(&DNSQuery[4],"\x01\x00",2);// Flags: 0x0100 Standard query
/*Questions: 1
  Answer RRs: 0
  Authority RRs: 0
  Additional RRs: 0 */
  memcpy(&DNSQuery[6],"\x00\x01\x00\x00\x00\x00\x00\x00",8);

  ulong curr_pos = 0;
  strcpy(bhostname,hostname);
  char* pch = strtok (bhostname,".");
  while (pch != NULL) {
    Rprintf("%s\r\n",pch);
    DNSQuery[14 + curr_pos] = strlen(pch);
    memcpy(&DNSQuery[15 + curr_pos],pch,strlen(pch));
    curr_pos+= 1+strlen(pch);
    pch = strtok (NULL, ".");
  }
  DNSQuery[15 + curr_pos -1] = 0x00;
/*  Type: A (Host Address) (1) 0x0001 representing A records (host addresses)
    Class: IN (0x0001) representing Internet addresses.*/
  memcpy(&DNSQuery[15 + curr_pos],"\x00\x01\x00\x01",4);

  long_hex(DNSQuery,2, (17+curr_pos)); // length
  //DBGBuf("DNSQuery",DNSQuery,19 + curr_pos);
  if ((ucResult = COMMS_Connect("208.67.222.222","53",40000)) != SUCCESS){return ucResult;}// 208.67.220.220 //208.67.222.222 //156.154.70.1 and 156.154.71.1
  m_bIsConnected = true;
  ucResult = GPRS_SendData((unsigned char*)DNSQuery, (2 + hex_long(DNSQuery,2)));
  int     recvLen = 0;
  if ((ucResult=GPRS_ReceiveData(&SndRcvBuf[0],&recvLen, 40000)) != SUCCESS){return ERR_GNRL;}
  //DBGBuf("SndRcvBuf", SndRcvBuf, recvLen);

  if ((!memcmp(&DNSQuery[2],&SndRcvBuf[2],2)) && ((SndRcvBuf[5] & 0x0f) == 0) &&
	  (!memcmp(&DNSQuery[14],&SndRcvBuf[14],curr_pos))){
	ulong ANCOUNT = hex_long(&SndRcvBuf[8],2); //  an unsigned 16 bit integer specifying the number of resource records in the answer section.
	Rprintf("ANCOUNT:%d\r\n",ANCOUNT);
	BYTE* ReplyCurr = &SndRcvBuf[14 + curr_pos + 1 + 4 ];
	//      for( int i=0; i< ANCOUNT; i++){
	while (ReplyCurr < SndRcvBuf + recvLen){
	  if (!memcmp(&ReplyCurr[2],"\x00\x01\x00\x01",4)){
		break;
	  }
	  //DBGBuf("ReplyCurr",ReplyCurr,50);
	  Rprintf("Reply Curr Inc %d\r\n", (12 + hex_long((unsigned char*)&ReplyCurr[10],2)));
	  ReplyCurr += (12 + hex_long((unsigned char*)&ReplyCurr[10],2));
	}
    if ( ReplyCurr < (SndRcvBuf + recvLen)){
      sprintf(IP,"%d.%d.%d.%d",ReplyCurr[12],ReplyCurr[13],ReplyCurr[14],ReplyCurr[15]);
      Rprintf("IP: %s\r\n", IP);
    }
  }

    if (m_bIsConnected)
  	COMMS_DisConnect();
    return SUCCESS;
}
unsigned char COMMS_DisConnect(void)
{
	int iResult;
	DisplayMsgBox(usCommHdrIdx,S_DISCON,0,0);
	iResult=GPRS_DisConnect();
	  if (iResult== GPRS_SUCCESS  ){
	    DBGPrt_m("Disconnected\r\n");
	  } else {
	    DBGPrt_m("Disconnect Failed\r\n");
	  }


    return SUCCESS;
}
unsigned char COMMS_TransOnlineNoConn(void)
{
  uchar ucResult;
    /* restore iso8583data from backup */

  ISO8583_RestoreISO8583Data((unsigned char *)&ISO8583Data,
                  (unsigned char *)&DataSave0->SaveISO8583Data);

  ucResult = COMMS_TransSendReceive();

  return(ucResult);

}

unsigned char COMMS_TransOnline(void)
{
  uchar ucResult;
  int iResult;
    /* restore iso8583data from backup */

  ISO8583_RestoreISO8583Data((unsigned char *)&ISO8583Data,
                  (unsigned char *)&DataSave0->SaveISO8583Data);

  if (udtRunData.bIsCurrReversal == TRUE) {
    usCommHdrIdx = S_RVRSL;
  } else{
    usCommHdrIdx = udtRunData.usHdrIdx;
  }

  iSgnlStrength= GPRS_getSignalStrength();
  DisplayMsgBox(usCommHdrIdx, S_TRY_CNCT,0, 0);
  iResult = GPRS_Connect(DataSave1->udtConnSettings.aucServerIP,
                              DataSave1->udtConnSettings.aucServerPort,
                              (DataSave1->udtConnSettings.ulHostTimeout * 1000));
  if (iResult == GPRS_SUCCESS){
    DisplayMsgBox(usCommHdrIdx, S_CNCT, 0, 0); 
  } else {
    DisplayMsgBox(usCommHdrIdx, S_NOT_CNCT,1000, 0);
    return iResult;
  }

  ucResult = COMMS_TransSendReceive();

  iResult=GPRS_DisConnect();
  if (iResult== GPRS_SUCCESS  ){
    DBGPrt_m("Disconnected\r\n");
  } else {
    DBGPrt_m("Disconnect Failed\r\n");
  }

  return(ucResult);

}

unsigned char COMMS_TransSendReceive(void)
{
    unsigned char aucMAC[8],keybuf[2],mackey[8];
    unsigned int uiLen;
    unsigned char ucResult;

    int i;

    /* Save package to SendISO8583Data */

    ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                        (unsigned char *)&DataSave0->SendISO8583Data);

    ucResult = ISO8583_PackData(ISO8583Data.aucCommBuf,&ISO8583Data.uiCommBufLen,
                    ISO8583_MAXCOMMBUFLEN);



    ISO8583_DumpData();

    /* Set MAC */
    if(  (ISO8583_CheckBit(64) == SUCCESS)
       ||(ISO8583_CheckBit(128) == SUCCESS)
      )
    {
        uiLen = ISO8583Data.uiCommBufLen-8;

        //DES_CRC(ISO8583Data.aucCommBuf,uiLen,aucMAC);

        memcpy( &ISO8583Data.aucCommBuf[uiLen],aucMAC,8);

#ifdef USER_DEBUG
    for (i=0;i<uiLen+8;i++) DBGPrt_m("%02X", ISO8583Data.aucCommBuf[i]);
#endif

    }



    /* SendReceive with host */
    uiLen = sizeof(ISO8583Data.aucCommBuf);
    uiLen=0;

  ucResult = COMMS_SendReceiveGprs(ISO8583Data.aucCommBuf,ISO8583Data.uiCommBufLen,
      ISO8583Data.aucCommBuf,&uiLen);

    if( ucResult != SUCCESS) return(ucResult);


    ISO8583Data.uiCommBufLen = uiLen;
    ISO8583_UnpackData(ISO8583Data.aucCommBuf,ISO8583Data.uiCommBufLen);

#ifdef USER_DEBUG
    DBGPrt_m("after receive\r\n");
    for (i=0;i<ISO8583Data.uiCommBufLen+8;i++) DBGPrt_m("%02X", ISO8583Data.aucCommBuf[i]);
#endif
    /* Check response data valid */
    ucResult = ISO8583_CheckResponseValid();
    if( ucResult != SUCCESS)
    {
        /* If have response return, cancel reversal */
    
        return(ucResult);
    }
    if(  (ISO8583_CheckBit(64) == SUCCESS)
       ||(ISO8583_CheckBit(128) == SUCCESS)
      )
    {
        //uiLen = ISO8583Data.uiCommBufLen-8;
        //
        //DES_CRC(ISO8583Data.aucCommBuf,uiLen,aucMAC);
        //
        //if( memcmp( &ISO8583Data.aucCommBuf[uiLen],aucMAC,8) )
        //{
        //    return(ERR_ISO8583_MACERROR);
        //}
    }
    return(SUCCESS);
}

unsigned char COMMS_SendReceiveGprs(unsigned char *pucInData,unsigned short uiInLen,
          unsigned char *pucOutData,unsigned int *puiOutLen)
{
  int ucResult=SUCCESS,ucIPAddress[5];
  unsigned short uiTimeout;
  int iTCPHandle,uiI;
  unsigned char ucBuf[20];

  unsigned int   uiLen=0;
  unsigned char   aucCommuBuf[1024];
  unsigned short   OutLen=0;
  unsigned char   i;
  unsigned char aucRecvBuf[1024];
  int recvLen;
  int iResult;
    char temp[100];
    unsigned char   ucline;



  memset(aucCommuBuf,0,sizeof(aucCommuBuf));
  memset(ucBuf,0,sizeof(ucBuf));
  //uiLen=uiInLen+5;
  //long_tab(&aucCommuBuf[0],2,&uiLen);
  //memcpy(&aucCommuBuf[2],aucTPDU,5);
  uiLen=uiInLen;
  memcpy(&aucCommuBuf[0],pucInData,uiInLen);

  ucline= DisplayMsgBox(usCommHdrIdx, S_REQ, 0, 0);
  if (udtRunData.bIsBatchUpload) {
	memset(ucBuf,0,sizeof(ucBuf));
    sprintf(ucBuf,"%d/%d",LeftTrN,MaxTrN);
	Lprint(ucline,5,ucBuf);
  }

#ifdef USER_DEBUG
  DBGPrt_m("\r\nConnect Server OK");

  DBGPrt_m("\r\n*******Send  Packet Data*******\n");
  for(uiI=0;uiI<uiLen;uiI++)
  {
    if((uiI+1)%20==0) DBGPrt_m("\r\n");

    DBGPrt_m("%02x ",aucCommuBuf[uiI]);
  }
  DBGPrt_m("\nBegin Send Data");
#endif



  recvLen=0;

  memset(aucRecvBuf,0,sizeof(aucRecvBuf));

//  vidSaveReversalInfo();

  //ucline= DisplayMsgBox(usCommHdrIdx, S_REQ, 0, 0);
  //if (udtRunData.bIsBatchUpload) {
  //  memset(ucBuf,0,sizeof(ucBuf));
  //  sprintf(ucBuf,"%d/%d",LeftTrN,MaxTrN);
  //  Lprint(ucline,5,ucBuf);
  //}
  if (DataSave1->udtTransDetail.udtTansType == BATCH) {
    DataSave1->bIsBatchPending = TRUE;
  }

  if ((udtRunData.bIsCurrReversal == FALSE) && (DataSave1->udtTransDetail.udtTansType != VOID)) {
    vidIncSTAN();
  }
  ucResult = GPRS_SendData(aucCommuBuf, uiInLen + 7); 

  ucline = DisplayMsgBox(usCommHdrIdx,S_RESP,0,0);
  if (udtRunData.bIsBatchUpload) {
	memset(ucBuf,0,sizeof(ucBuf));
    sprintf(ucBuf,"%d/%d",LeftTrN,MaxTrN);
	Lprint(ucline,5,ucBuf);
  }
  ucResult=GPRS_ReceiveData(aucRecvBuf,&recvLen,(DataSave1->udtConnSettings.ulHostTimeout*1000));

#ifdef USER_DEBUG
    DBGPrt_m("\r\nReceive Data= %d\r\n",recvLen);
#endif
  *puiOutLen=recvLen;


  if (recvLen>0)
  {
#ifdef USER_DEBUG
    DBGPrt_m("\r\n*********Receive Packet Data***********\r\n");
    DBGPrt_m("\r\nReceive Data Len befor Decrypt= %d\r\n",recvLen);
    for(uiI=0;uiI<recvLen;uiI++)
    {
      if((uiI+1)%20==0) DBGPrt_m("\r\n");
      DBGPrt_m("%02x ",aucRecvBuf[uiI]);
    }
#endif

     //recvLen=recvLen-7;

     if( !ucResult ){
       memcpy(pucOutData,&aucRecvBuf[0],recvLen);
      }


  }   
  return(ucResult);
}


