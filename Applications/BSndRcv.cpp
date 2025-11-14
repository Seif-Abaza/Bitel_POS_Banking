/*
 * BSndRcv.cpp
 *
 *  Created on: Jun 18, 2014
 *      Author: Mahmoud Swedan
 */

extern "C"{
#include "AppGlobal.h"
#include "./resources/Msgs.h"
#include "CurrencyCodes.h"
#include "SndRcvIso8583.h"
#include "UTILS.h"
#include "Display.h"
#include "Utilities.h"
#include "../GZIP/zipmem.h"
  extern int WRITE_APP_PROGRAM(long count, char *src_addr, char *des_addr, char code);		/* KHLEE 04/12/11 */
  extern int FLASH_SECTOR_ERASE(char *flash_ROM_physical_address);
  extern  int fnD642Bcd(U08 *bcdBuf, D64 dwNo, int nx);


}
#include "../internal/tools.h"
#include "BSndRcv.h"
#include "emv/EMV_APP.h"
#include "emv/EMV.h"

#include "CFilesMngr.h"



#ifndef ZM
#define ZM(stc) memset(&stc, 0, sizeof(stc))
#endif

extern "C" int Biteltime(int ref);
extern "C" unsigned char InitialzeSSL(char* acIPaddress,int iPort);
extern havege_state hs;
extern ssl_session ssn;
extern x509_cert cacert;
extern x509_cert clicert;
extern rsa_context rsa;
extern ssl_context ssl;

const char HTTPHEAD[]=
{

	"HEAD /BREAD.gz HTTP/1.1\r\n"
	"Host: 41.38.125.23:90\r\n"
	"\r\n\r\n\0"
};
const char HTTPGET[]=
{
//	"GET /BREAD.gz HTTP/1.1\r\n"
//	"Host: 41.38.125.23:90\r\n"
//	"Connection: keep-alive\r\n"
	"GET /BREAD.gz HTTP/1.1\r\n"
	"Host: 41.38.125.23:90\r\n"
	"Connection: keep-alive\r\n"
	"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
	"User-Agent: Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/36.0.1985.143 Safari/537.36\r\n"
	"Accept-Encoding: gzip,deflate,sdch\r\n"
	"Accept-Language: en-US,en;q=0.8,ar;q=0.6\r\n"
	"\r\n\r\n\0"
};

CSndRcv::CSndRcv(){
  m_pContainer = new CMegaContainer(NBE);
  if (m_pContainer->getHostProtocol() == NBE){
	DBGLine("NBE");}
  else{
	DBGLine("FDATA");
  }
  m_sRcvBuf = new BYTE[2 + RCV_MSG_MAX_SIZE];
  m_pudtPrgPar = 0;
  m_ulChunkNo = 1;
}
CSndRcv::CSndRcv(const string& strPrgParHdrMsg, const string& strPrgParDispMsg){
  m_pContainer = new CMegaContainer();
  m_sRcvBuf = new BYTE[RCV_MSG_MAX_SIZE];
  m_pudtPrgPar = new PrgPar(strPrgParHdrMsg, strPrgParDispMsg, 0,128);
}

CSndRcv::~CSndRcv(){
  DELETE_POINTER(m_pContainer);
  DELETE_POINTER_ARR(m_sRcvBuf);
}

uchar CSndRcv::CheckResponseValid()
{
  string ans = m_pContainer->get(FIELD_ACTION_CODE);
  udtRunData.ucErrorExtCode = atol(ans.buffer());
  if (ans == "00"){
	return SUCCESS;
  } else {
	return ERR_ISO_HOSTCODE;
  }
}
void CSndRcv::vidIncSTAN(void){
  DataSave3->ulSysTraceAuditNo++;
  if (DataSave3->ulSysTraceAuditNo < 0 || DataSave3->ulSysTraceAuditNo > 9999999)
	DataSave3->ulSysTraceAuditNo = 1;
}
uchar CSndRcv::SSLSendReceive()
{
	int n,e,i;
	const char *p;
	unsigned char temps[20],ucResult;

		UTIL_ClearScren();


		Lprintf(1, 0,"Connecting  ...             ");
		Lprintf(2, 0,"Please Wait ...             ");


		if ((ucResult = COMMS_Connect((char *)DataSave1->udtConnSettings.aucServerIP,(char *)DataSave1->udtConnSettings.aucServerPort,10000)) != SUCCESS)//"198.241.162.103"
		//if (GPRS_Connect((char *)"31.13.81.113\0",(char *)"443\0",10000)!=GPRS_SUCCESS)
		//if (GPRS_Connect((char *)"173.201.44.188\0",(char *)"80\0")!=GPRS_SUCCESS)
		{
			sprintf((char *)temps, "server not Connected\0");
			Lprintf(2, 0, (char *)temps);
			Lprintf(3, 0,"press Enter             ");
			WaitKeyPress10MS(200);
		}

		Lprintf(1, 0,"Initialize  ...             ");
		Lprintf(2, 0,"Please Wait ...             ");

		//Initialize SSL
		ucResult=InitialzeSSL((char*)"certservicesgatewayoce.visa.com",//DataSave1->udtConnSettings.aucServerIP,
                              atol((char*)DataSave1->udtConnSettings.aucServerPort));
//		ucResult=InitialzeSSL((char *)"31.13.81.113\0",443);

		if (ucResult)
		{
			UTIL_ClearScren();
			Lprintf(3, 0,"SSL Failed ");
			return ERR_GNRL;
		}
		else
		{
			UTIL_ClearScren();
			Lprintf(3, 0,"SSL succeeded ");
		}

		WaitKeyPress10MS(200);

		unsigned char SSLSendBuffer[100];
		unsigned char SSLRecvBuffer[100];
		memset(SSLSendBuffer,0,sizeof(SSLSendBuffer));
		memset(SSLRecvBuffer,0,sizeof(SSLRecvBuffer));


		memcpy(SSLSendBuffer,(char *)"POSV1008002000010000C00002990000091000181740000000100029130300004380300189100BITEL000V02086",91);
		SSLSendBuffer[91]=3;
		ssl_write(&ssl,SSLSendBuffer,91);

		ssl_read(&ssl,SSLRecvBuffer,49);

		DBGLine("SSLRecvBuffer:%s",SSLRecvBuffer);

		//ssl_read(&ssl,SSLRecvBuffer,49);

		//NOTE: use ssl_write to send data to server

		//NOTE: use ssl_read to read data from server
			GPRS_DisConnect();

			//NOTE: Add below lines in case you will close SSL session
			ssl_close_notify( &ssl );
			x509_free( &clicert );
			x509_free( &cacert );
			rsa_free( &rsa );
			ssl_free( &ssl );

	return 0;
}
uchar CSndRcv::TransSendReceive()
{
  uchar ucResult;
  string			rcvMsg;
  m_pContainer->debugContainer();
  string	strComposed	= m_pContainer->composeMessage(/*FDATA*/);
  if (strComposed.empty()){return ERR_EMPTY_MSG;}

 DBGBuf("Send Data:", (LPBYTE)strComposed.buffer(),strComposed.length());

  if (!m_pudtPrgPar)
	DisplayMsgBox(usCommHdrIdx, S_REQ, 0, 0);

  vidIncSTAN();
//  if (bEnableSSL){
//
//  }else
  {
  ucResult = GPRS_SendData((LPBYTE)strComposed.buffer(), strComposed.length());
  }

  int     recvLen;
//  BYTE* aucRcvBuf = new BYTE[2 + RCV_MSG_MAX_SIZE];
//  BYTE aucRcvBuf[2 + RCV_MSG_MAX_SIZE];

  if (!m_pudtPrgPar)
	DisplayMsgBox(usCommHdrIdx,S_RESP,0,0);

  DBGLine("GPRS_ReceiveData");

  if ((ucResult=GPRS_ReceiveData(m_sRcvBuf,&recvLen,(DataSave1->udtConnSettings.ulHostTimeout*1000))) != SUCCESS){return ucResult;}

  DWORD m_dwNOfBytes = recvLen;

  WORD wMsgSize ;

	DBGLine("m_dwNOfBytes %d",m_dwNOfBytes);
	DBGLine("*****************aucRcvBuf Start******************");
	DBGLine("aucRcvBuf Data: %s",m_sRcvBuf);
	DBGLine("****************aucRcvBuf End******************");

  wMsgSize = recvLen;
if (m_pContainer->getHostProtocol() == NBE){
  wMsgSize = hex_long(m_sRcvBuf,2);

  if (wMsgSize > RCV_MSG_MAX_SIZE){
	BYTE* pucTempRcvBuf = new BYTE[2 + wMsgSize];
	memcpy(pucTempRcvBuf,m_sRcvBuf,recvLen);
	DELETE_POINTER_ARR(m_sRcvBuf);
	m_sRcvBuf = pucTempRcvBuf;
  }

  DBGLine("recvLen: %d  wMsgSize: %d",recvLen, wMsgSize + 2);

  while(m_dwNOfBytes < wMsgSize) {
	recvLen = 0;
	//memset((void)aucRcvBuf,0,sizeof(aucRcvBuf));
	if ((ucResult=GPRS_ReceiveData(&m_sRcvBuf[m_dwNOfBytes],&recvLen,(DataSave1->udtConnSettings.ulHostTimeout*1000))) != SUCCESS){return ucResult;}
	m_dwNOfBytes+= recvLen;
	DBGLine("MsgSize %d Recived %d  AccNOfBytes %d",wMsgSize, recvLen, m_dwNOfBytes);

  }
  /*
	DBGLine("m_dwNOfBytes %d",m_dwNOfBytes);
	DBGLine("*****************aucRcvBuf Start******************");
	DBGBuf("aucRcvBuf Data:",aucRcvBuf,m_dwNOfBytes);
	DBGLine("****************aucRcvBuf End******************");
   */

  	rcvMsg = string((char*)(m_sRcvBuf + 7),(wMsgSize - 5));
}
else {
  rcvMsg = string((char*)(m_sRcvBuf + 6 ),(wMsgSize - 6 ));
}
  //if ( recvLen == wMsgSize + 2  )
  {
	//		BYTE* pBuf = new BYTE[wMsgSize - 5];
	//		memcpy(pBuf, m_BufIn + 7, wMsgSize);


//	DELETE_POINTER_ARR(aucRcvBuf);

	m_pContainer->clear();
	if (!m_pContainer->decomposeMessage(rcvMsg/*,FDATA*/))
	{
	  //DELETE_POINTER(pContainer);
	  DBGLine("Error: CActionTransaction::_receiveTransaction() - decomposeMessage()");
	  return ERR_EMPTY_MSG;
	}
	DBGBuf("RECEIVED", (BYTE*)rcvMsg.buffer(), rcvMsg.length());
	m_pContainer->debugContainer();
	uchar ucResult = CheckResponseValid();
	if (ucResult != SUCCESS){
	  return ucResult;
	}
	return SUCCESS;
  }
}

void CSndRcv::vidUpdtPrgPar(int iPrgParCurrValue , int iPrgParMaxValue)
{
  if (m_pudtPrgPar){
	DBGLine("Progress par %d %d", iPrgParCurrValue,iPrgParMaxValue);
	DisplayProgressBar(m_pudtPrgPar->strPrgParHdrMsg.buffer(), m_pudtPrgPar->strPrgParDispMsg.buffer(),
	                   iPrgParCurrValue,iPrgParMaxValue);
  }
}
uchar CSndRcv::ucChkResp(uchar* pucFileName, ulong ulChunkNo)
{
  if (m_pContainer->get(FIELD_STAN) != string((uint)DataSave3->ulSysTraceAuditNo-1,6)) {return ERR_RESPO_RETRY;}
  if (m_pContainer->get(FIELD_TERM_NUM) !=  string((char*)DataSave1->udtDeviceConfig.aucTermId, strlen((char*)DataSave1->udtDeviceConfig.aucTermId))) {return ERR_RESPO_RETRY;}
  if (m_pContainer->get(FIELD_OUTLET_NUM) != string((char*)DataSave1->udtDeviceConfig.aucMerchantId, strlen((char*)DataSave1->udtDeviceConfig.aucMerchantId))){return ERR_RESPO_RETRY;}
  if (m_pContainer->get(FIELD_FILE_NAME) != string((char*)pucFileName,strlen((char *)pucFileName))) {return ERR_RESPO_RETRY;}
  if (m_pContainer->get(FIELD_MSG_NO).left(3) != string((uint)ulChunkNo,3)) {return ERR_RESPO_RETRY;}
  return SUCCESS;
}
uchar CSndRcv::ucGetFileUpdtFlags(void)
{
  string ans = m_pContainer->get(FIELD_FILE_UPDAT_FLAGS);
  DataSave3->udtFileUpdtReq.BLUpdtReq = getBit((uchar*)ans.buffer(),1);
  DataSave3->udtFileUpdtReq.WLUpdtReq = getBit((uchar*)ans.buffer(),2);
  DataSave3->udtFileUpdtReq.DRUpdtReq = getBit((uchar*)ans.buffer(),3);
  DataSave3->udtFileUpdtReq.POSPramUpdtReq = getBit((uchar*)ans.buffer(),4);
}
uchar CSndRcv::ucGetFileActions(FileAction*   pudtFileAction, bool* pbIsLastChunk)
{
  *pudtFileAction =UNDEF_FACT;
  pbIsLastChunk = false;
  string ans = m_pContainer->get(FIELD_FUNCTION_CODE);
  if (ans.empty()) {return ERR_GNRL;}
  *pudtFileAction = (FileAction)(ans.toLong() - 299);
//  DBGLine("FileAction:%d", (int)*pudtFileAction);
  switch ( (int)*pudtFileAction ) {
	case END_OF_TRANS:
	case DELETE_FILE:
	case REPLACE_FILE:
	  *pbIsLastChunk = true;
	  break;
	case ADD_RECORD    :
	case MODIFY_RECORD :
	case DELETE_RECORD :
	case REPLACE_RECORD:
	  *pbIsLastChunk = false;
	  break;

	default:
	  DBGLine("FileAction: ERR_GNRL");
	  return ERR_GNRL;
	  break;
  }
  return SUCCESS;
}

uchar CSndRcv::ucGetChnkIsoFlds(string& strChnkData )
{
  string StrBitNo;
  for (uchar ChnkFld = 0; ChnkFld < MAX_CHNK_FLDS; ChnkFld++)
  {
	StrBitNo.format("%d",(56 + ChnkFld));
	string ans = m_pContainer->get(StrBitNo);
	if (ans.empty()){break;}
	strChnkData += ans;
  }
  return SUCCESS;
}
uchar CSndRcv::ucFileChunkSndRcv(uchar* pucFileName, ulong ulChunkNo, string& strChnkData, strChnkAttr* pudtChnkAttr)
{
  DBGLine("ucChunkSndRcv Fn");

  m_pContainer->clear();
  m_pContainer->set(FIELD_STAN,	string((uint)DataSave3->ulSysTraceAuditNo,6));
  m_pContainer->set(FIELD_TERM_NUM,	string((char*)DataSave1->udtDeviceConfig.aucTermId, strlen((char*)DataSave1->udtDeviceConfig.aucTermId)));
  m_pContainer->set(FIELD_OUTLET_NUM,	string((char*)DataSave1->udtDeviceConfig.aucMerchantId, strlen((char*)DataSave1->udtDeviceConfig.aucMerchantId)));
  m_pContainer->set(FIELD_FILES_VERSIONS,	string((char*)&DataSave3->udtFilesVer,sizeof(DataSave3->udtFilesVer)));
  m_pContainer->set(FIELD_BTCH_NO,	string((uint)DataSave3->ulBatchCounter,6));
  m_pContainer->set(FIELD_MSG_NO,	string((uint)ulChunkNo,3));
  m_pContainer->set(FIELD_FILE_NAME,	string((char*)pucFileName,strlen((char *)pucFileName)));
  m_pContainer->setType(MTID_FILE_TRANSFER_REQUEST,"");

  uchar ucResult;
  if ( (ucResult = TransSendReceive()) != SUCCESS) {return ucResult;}

  //if (ucChkResp(pucFileName,ulChunkNo) == ERR_RESPO_RETRY){ return ERR_RESPO_RETRY; }

  if (ulChunkNo  == 1)
  {
	memset((void*)pudtChnkAttr, 0, sizeof(strChnkAttr));
	string ans;
	if ((ans = m_pContainer->get(FIELD_FILES_VERSIONS)).empty()) {return ERR_RESPO_RETRY; }
	memcpy((void*)pudtChnkAttr->acFileVersion,(void*)ans.buffer(),3);

	if ((ans = m_pContainer->get(FIELD_MSG_NO)).empty()) {return ERR_RESPO_RETRY; }
	pudtChnkAttr->ulMaxChunkno = ans.right(3).toLong();
	DBGLine("ulMaxChunkno = %d", pudtChnkAttr->ulMaxChunkno);
  }

  if(ucGetFileActions(&pudtChnkAttr->udtFileAction, &pudtChnkAttr->bIsLastChunk) != SUCCESS){return ERR_RESPO_RETRY;}

  strChnkData = m_pContainer->get(FIELD_DATA_CHNK1);
//  DBGBuf("ChnkData",(LPBYTE)str.buffer(),str.length());
  //ucGetChnkIsoFlds(strChnkData);
  if ((strChnkData.empty()) && (pudtChnkAttr->udtFileAction != DELETE_FILE)) { return ERR_RESPO_RETRY; }

  return SUCCESS;
}
uchar CSndRcv::ucDldFileChunk (uchar* pucFileName, ulong ulChunkNo, string& strChunkData, strChnkAttr* pudtChnkAttr)
{
  uchar ucTrailsNo = 0;

  DBGLine("ucDwnldChunk Fn");
  do {
	DBGLine("ucTrailsNo %d", ucTrailsNo);
	if(ucFileChunkSndRcv(pucFileName, ulChunkNo, strChunkData, pudtChnkAttr) == SUCCESS){
	  vidUpdtPrgPar(ulChunkNo,pudtChnkAttr->ulMaxChunkno);
	  return SUCCESS;
	} else {ucTrailsNo++; continue;}
  }while ((ucTrailsNo < MAX_FILE_REQ_TRIALS));
  return ERR_FILE_CHNK;
}

uchar CSndRcv::ucSettlSndRcv(int* pnLastSettBatch)
{
  DBGLine("Fn ucSettlSndRcv()");
  uchar ucResult;
  char TempBuff[10], GenTempBuff[50];
  bool ucGnrlResult = SUCCESS;
  strLogHdr* pLogHdr;

  DBGLine("ulDistSettlNo:%d", DataSave2->ulDistSettlNo);
  for (int i = 0; i< DataSave2->ulDistSettlNo; i++){
	if (DataSave2->DistBatchInfo[i].ucSettelStatus == 0){
	  pLogHdr  = (strLogHdr*) &DataSave2->aucBatch[DataSave2->DistBatchInfo[i].aulDistBatchIdx];

	  sprintf(TempBuff,"%d",hex_long((char *)pLogHdr->aucBatchNo,3));
	  sprintf(GenTempBuff," fhja vrl %s",TempBuff);
	  m_pContainer->clear();
	  m_pContainer->set(FIELD_STAN,	string((uint)DataSave3->ulSysTraceAuditNo,6));
	  m_pContainer->set(FIELD_TERM_NUM,	string((char*)DataSave1->udtDeviceConfig.aucTermId, strlen((char*)DataSave1->udtDeviceConfig.aucTermId)));
	  m_pContainer->set(FIELD_OUTLET_NUM,	string((char*)DataSave1->udtDeviceConfig.aucMerchantId, strlen((char*)DataSave1->udtDeviceConfig.aucMerchantId)));
	  m_pContainer->set(FIELD_FILES_VERSIONS,	string((char*)&DataSave3->udtFilesVer,sizeof(DataSave3->udtFilesVer)));
	  //m_pContainer->set(FIELD_BTCH_NO,	string((uint)DataSave3->ulBatchCounter,6));
	  m_pContainer->set(FIELD_BTCH_NO,	string((uint)DataSave3->ulMainBtchCounter,6));
	  m_pContainer->set(FIELD_BTCH_TOTLS, string((LPCTSTR)pLogHdr,sizeof(strLogHdr)));
	  m_pContainer->setType(MTID_SETTLEMENT_REQUEST, "");
	  //   DataSave1->bIsBatchPending = TRUE;
	  if ( (ucResult = TransSendReceive())  == SUCCESS) {
		DataSave2->DistBatchInfo[i].ucSettelStatus = 1;
		DataSave2->bIsSettlOccur = true;
		  ClearLCD();
		  Lprint_arabic(1, 0, (uchar *)GenTempBuff);
		Lprint_arabic(2, 0, (uchar *)"jlj hgjs,dm");
		WaitKeyPress10MS(200);
		//ucGetFileUpdtFlags();
	  } else {
		*pnLastSettBatch = i;
		ucGnrlResult = ucResult;
		  ClearLCD();
		  Lprint_arabic(1, 0, (uchar *)GenTempBuff);
		Lprint_arabic(2, 0, (uchar *)"gl jjl hgjs,dm");
	  }
	}
  }
  return ucGnrlResult;
//  string TotalsStr;
//  TotalsStr.format("%03d",DataSave2->ulDistSettlNo);
//  for (int i = 0; i< DataSave2->ulDistSettlNo; i++){
//	TotalsStr += string((LPCTSTR)&DataSave2->aucBatch[DataSave2->DistBatchInfo[i].aulDistBatchIdx],sizeof(strLogHdr));
//  }
//  m_pContainer->set(FIELD_BTCH_TOTLS, TotalsStr);
//  m_pContainer->set(FIELD_BTCH_TOTLS,	string().format("%012d%06d%012d%06d", hex_long((LPBYTE)DataSave2->udtTransLog.aucTotalAmount,4),
//                                     	                hex_long((LPBYTE)DataSave2->udtTransLog.aucSavedTransNo,2), DataSave2->ulInspBreadNo,
//                                     	                DataSave2->ulNoOfInspTrans));
//  m_pContainer->setType(MTID_SETTLEMENT_REQUEST, "");
//  //   DataSave1->bIsBatchPending = TRUE;
//  uchar ucResult = TransSendReceive();
//  if ( ucResult  == SUCCESS) {
//	//ucGetFileUpdtFlags();
//  }
//  return ucResult;

}
uchar CSndRcv::ucBatchChunkUpload(uchar* pucChunkAddr, ulong ulChunkSize, ulong ulChunkNo , bool bIsLastChnk, bool bIsComprsd)
{
  short iBitNo = 56;
  ulong ulDataToSet = 0;
  ulong ulSrc_Cur = 0;
  string StrBitNo;

  m_pContainer->clear();
  m_pContainer->set(FIELD_STAN,	string((uint)DataSave3->ulSysTraceAuditNo,6));
  m_pContainer->set(FIELD_FUNCTION_CODE,	string().format("%d",(300+ (uchar)(!bIsLastChnk))));
  m_pContainer->set(FIELD_TERM_NUM,	string((char*)DataSave1->udtDeviceConfig.aucTermId, strlen((char*)DataSave1->udtDeviceConfig.aucTermId)));
  m_pContainer->set(FIELD_OUTLET_NUM,	string((char*)DataSave1->udtDeviceConfig.aucMerchantId, strlen((char*)DataSave1->udtDeviceConfig.aucMerchantId)));
  m_pContainer->set(FIELD_FILES_VERSIONS,	string((char*)&DataSave3->udtFilesVer,sizeof(DataSave3->udtFilesVer)));
  m_pContainer->set(FIELD_FILE_NAME,	Q(bIsComprsd, string("BATCH_COMP"), string("BATCH")));
  m_pContainer->set(FIELD_MSG_NO,	string((uint)ulChunkNo,3));
  m_pContainer->set(FIELD_BTCH_NO,	string((uint)DataSave3->ulBatchCounter,6));

  for(ulSrc_Cur = 0; ulSrc_Cur < ulChunkSize; ulSrc_Cur+= ulDataToSet){
	ulDataToSet = ((ulChunkSize - ulSrc_Cur) >= 999 ? 999: (ulChunkSize - ulSrc_Cur));
	StrBitNo.format("%d",iBitNo++);
	m_pContainer->set(StrBitNo,string((char*)(pucChunkAddr+ulSrc_Cur), ulDataToSet));
  }

  m_pContainer->setType(MTID_FILE_TRANSFER_REQUEST,"");

  uchar ucResult = TransSendReceive();
  if ( ucResult  == SUCCESS) {
	ucGetFileUpdtFlags();
  }
  return ucResult;

}
uchar CSndRcv::ucBatchUpload(uchar* pucBtchAddr, ulong ulBSz)
{
  ulong ulRemToSendSize =  ulBSz;
  ulong ulDataToSendSize = 0;
  ulong ulChunkNo = 1;
  uchar ucTrialsNo = 0;
  bool bIsLastChnk;
  bool bIsComprsd = !(pucBtchAddr == (uchar*) DataSave2->aucBatch);

  usCommHdrIdx = S_BATCH_UPLD_HDR;
  udtRunData.bIsBatchUpload = TRUE;
  ulong ulMaxChunkno = (ulBSz/BATCH_CHUNK_SIZE) + ( (ulBSz % BATCH_CHUNK_SIZE)>0 );

  for (ulong src_cur_pos = 0 ;src_cur_pos < ulBSz; )
  {
	ulDataToSendSize = min((ulBSz- src_cur_pos), BATCH_CHUNK_SIZE); //Q((ulBSz- src_cur_pos) > BATCH_CHUNK_SIZE, BATCH_CHUNK_SIZE, (ulBSz - src_cur_pos));
	bIsLastChnk  = (ulDataToSendSize == (ulBSz- src_cur_pos));
	if (ucBatchChunkUpload((uchar*)(pucBtchAddr+ src_cur_pos), ulDataToSendSize, ulChunkNo , bIsLastChnk,bIsComprsd) == SUCCESS){
	  vidUpdtPrgPar(ulChunkNo, ulMaxChunkno);
	  src_cur_pos += ulDataToSendSize;
	  ulChunkNo++;
	  ucTrialsNo = 0;
	} else {
	  ucTrialsNo++;
	}
	if (ucTrialsNo >= MAX_BATCH_UPL_TRIALS){
	  udtRunData.bIsBatchUpload =FALSE;
	  return BTCH_UPLD_ERR;
	}
  }
  udtRunData.bIsBatchUpload =FALSE;
  return SUCCESS;
}
uchar CSndRcv::ucUpdtCardInfoSndRcv()
{
  DBGLine("Fn ucSettlSndRcv()");

  m_pContainer->clear();
  m_pContainer->set(FIELD_STAN,	string((uint)DataSave3->ulSysTraceAuditNo,6));
  m_pContainer->set(FIELD_CARD_NUM,	string((char*)udtBrdRunData.udtInsCardData.aucOutsideCode, strlen((char*)udtBrdRunData.udtInsCardData.aucOutsideCode)));
  m_pContainer->set(FIELD_CARD_CODE,	string((char*)udtBrdRunData.udtInsCardData.aucInsideCode, strlen((char*)udtBrdRunData.udtInsCardData.aucInsideCode)));
  m_pContainer->set(LENGTH_GOV_CODE,	string((char*)udtBrdRunData.udtInsCardData.aucGovernarate, strlen((char*)udtBrdRunData.udtInsCardData.aucGovernarate)));
  m_pContainer->set(FIELD_TERM_NUM,	string((char*)DataSave1->udtDeviceConfig.aucTermId, strlen((char*)DataSave1->udtDeviceConfig.aucTermId)));
  m_pContainer->set(FIELD_OUTLET_NUM,	string((char*)DataSave1->udtDeviceConfig.aucMerchantId, strlen((char*)DataSave1->udtDeviceConfig.aucMerchantId)));
  m_pContainer->setType(MTID_CARD_UPDT_REQUEST, "");
  uchar ucResult = TransSendReceive();
  if ( ucResult  == SUCCESS) {
	//ucGetFileUpdtFlags();
  }
  return ucResult;

}

uchar CSndRcv::ucCubeSale(char * EMVTags, int TagsStrLen)
{
  DBGLine("Fn ucSettlSndRcv()");

  BYTE *ptr, *pAdd;
  char TempBuff[1000];
  char DummyBuff[] = "\x44\x27\x80\x80\x01\x11\x22\x23\x33\x7D\x15\x12\x20\x11\x82\x22\x98\x3F";
  int TagLen = 0;


  DBGBuf("DummyHexBuff",(LPBYTE)DummyBuff,TagLen);
  DBGLine("TempBuff: %s",TempBuff);
//
//  int2LL(TempBuff,(TagLen * 2)); memcpy(TempBuff,pAdd,TagLen);
//  m_pContainer->set(FIELD_TRACK_2,	string((uint)DataSave3->ulSysTraceAuditNo,6));


  m_pContainer->clear();

  /*TagLen = getDnL((LPBYTE)EMVTags, TagsStrLen, Tvc_PAN, &pAdd);   memset(TempBuff,0,sizeof(TempBuff));
  bcd2zasc(TempBuff,(char*)pAdd,(TagLen*2)); if (TempBuff[(TagLen*2)-1] == '?') TempBuff[(TagLen*2)-1] = '\0';
  m_pContainer->set(FIELD_CARD_NUM,	string((char*)TempBuff, strlen(TempBuff)));*/
//  m_pContainer->set(FIELD_PROCESS_CODE,	string((char*)"000000", 6));
  TagLen = getDnL((LPBYTE)EMVTags, TagsStrLen, T6n_TrmAmountAuthorised, &pAdd);   memset(TempBuff,0,sizeof(TempBuff));
  bcd2asc(TempBuff,(LPTSTR)pAdd,(TagLen * 2));
  m_pContainer->set(FIELD_TRN_AMOUNT,	string((char*)TempBuff, strlen((char*)TempBuff)));
  m_pContainer->set(FIELD_STAN,	string((uint)DataSave3->ulSysTraceAuditNo,6));

  sprintf(TempBuff,"%03x",(PEntryModeICC|PEntryModePinEntry));
  m_pContainer->set(FIELD_ENTRY_MODE,	string(TempBuff, 3));
  sprintf(TempBuff,"%02x",(PCC_Normal));
  m_pContainer->set(FIELD_COND_MODE,	string(TempBuff, 2));
  m_pContainer->set(FIELD_FUNCTION_CODE, string("091", 3));


  TagLen = getDnL((LPBYTE)EMVTags, TagsStrLen, Tvb_Track2EquivalentData, &pAdd); memset(TempBuff,0,sizeof(TempBuff));
  bcd2zasc(TempBuff,(char*)pAdd,(TagLen*2)); if (TempBuff[(TagLen*2)-1] == '?') TempBuff[(TagLen*2)-1] = '\0';
  m_pContainer->set(FIELD_TRACK_2,	string((char*)TempBuff, strlen(TempBuff)));

  m_pContainer->set(FIELD_TERM_NUM,	string((char*)DataSave1->udtDeviceConfig.aucTermId, strlen((char*)DataSave1->udtDeviceConfig.aucTermId)));
  m_pContainer->set(FIELD_OUTLET_NUM,	string((char*)DataSave1->udtDeviceConfig.aucMerchantId, strlen((char*)DataSave1->udtDeviceConfig.aucMerchantId)));

  TagLen = getDnL((LPBYTE)EMVTags, TagsStrLen, T2n_TrmTrxnCurrencyCode, &pAdd); memset(TempBuff,0,sizeof(TempBuff));
  bcd2asc(TempBuff,(LPTSTR)pAdd,(TagLen*2));
  m_pContainer->set(FIELD_TRN_CURRENCY_CODE,	string(&TempBuff[1], LENGTH_TRN_CURRENCY_CODE));
  m_pContainer->set(FIELD_BTCH_TOTLS,	string((char*)"02700083700000100043803000396C", strlen("02700083700000100043803000396C")));


  extern TTrxnRec gsXRec;
  int len=trans_UPDATEgsXRec_ICCbL3();
  memset(TempBuff,0,sizeof(TempBuff));
  memcpy(TempBuff, (U08 *)&gsXRec.Trns_ICCbL3, len);
  m_pContainer->set(FIELD_ICC_DATACAPTURE,	string((char*)TempBuff, strlen(TempBuff)));

  m_pContainer->setType(MTID_SALE_REQUEST, "000000");
  uchar ucResult;
  if ((ucResult = COMMS_Connect((char*)DataSave1->udtConnSettings.aucServerIP,
                                 (char*)DataSave1->udtConnSettings.aucServerPort,
                                 (DataSave1->udtConnSettings.ulHostTimeout*1000))) != SUCCESS){return ucResult;}

  ucResult = TransSendReceive();
  if ( ucResult  == SUCCESS) {
	string result = m_pContainer->get(FIELD_ICC_DATACAPTURE);
	LPTSTR curr = result.buffer();
	int SubfldLen = 0, SubfldId = 0;
	char TempBuff[20] = {0};
	while ((curr - result.buffer()) < result.length()){
	  memcpy(TempBuff, curr, 4);
	  SubfldLen = atol(TempBuff); curr += 4;
	  memset(TempBuff,0,sizeof(TempBuff)); memcpy(TempBuff, curr, 2); curr += 2;
	  SubfldId = atol(TempBuff);
	  memset(TempBuff,0,sizeof(TempBuff));
	  asc2bcd(TempBuff, curr, (SubfldLen - 2));
	  DBGBuf("Retrn TAG", (LPBYTE)TempBuff,(SubfldLen - 2)/2);
	  if (SubfldId == 2)
		pgbAppBufUpdate(Ttt_IssuerScript1, (SubfldLen - 2)/2, (LPBYTE)TempBuff);
	  else if (SubfldId == 3)
		pgbAppBufUpdate(Ttt_IssuerScript2, (SubfldLen - 2)/2, (LPBYTE)TempBuff);
	  else if (SubfldId == 6)
		pgbAppBufUpdate(Tvb_IAD, (SubfldLen - 2)/2, (LPBYTE)TempBuff);
	  curr += (SubfldLen - 2);
	}

	result = m_pContainer->get(FIELD_ACTION_CODE);
	pgbAppBufUpdate(T2a_ARC, 2, (LPBYTE)result.buffer());
	DBGLine("T2a_ARC:%s",result.buffer());
    gwResponseCode=0x3030; // Always approve
    ghResponseCode = 0x3030;



	//ucGetFileUpdtFlags();
  }
  //	getDnL(pgbAppBuf, giAppBufLen, Tvb_Track2EquivalentData, &pAdd);
//  m_pContainer->set(FIELD_CARD_CODE,	string((char*)udtBrdRunData.udtInsCardData.aucInsideCode, strlen((char*)udtBrdRunData.udtInsCardData.aucInsideCode)));
//  m_pContainer->set(LENGTH_GOV_CODE,	string((char*)udtBrdRunData.udtInsCardData.aucGovernarate, strlen((char*)udtBrdRunData.udtInsCardData.aucGovernarate)));

  COMMS_DisConnect();
  return ucResult;

}

uchar CSndRcv::ucSoapNBESale(char * EMVTags, int TagsStrLen)
{

}
uchar CSndRcv::ucNBESale(char * EMVTags, int TagsStrLen)
{
  DBGLine("Fn ucSettlSndRcv()");

  BYTE *ptr, *pAdd, *pPtr;
  char TempBuff[1000];
  char DummyBuff[] = "\x44\x27\x80\x80\x01\x11\x22\x23\x33\x7D\x15\x12\x20\x11\x82\x22\x98\x3F";
  int TagLen = 0;


  DBGBuf("DummyHexBuff",(LPBYTE)DummyBuff,TagLen);
  DBGLine("TempBuff: %s",TempBuff);
//
//  int2LL(TempBuff,(TagLen * 2)); memcpy(TempBuff,pAdd,TagLen);
//  m_pContainer->set(FIELD_TRACK_2,	string((uint)DataSave3->ulSysTraceAuditNo,6));


  m_pContainer->clear();

  /*TagLen = getDnL((LPBYTE)EMVTags, TagsStrLen, Tvc_PAN, &pAdd);   memset(TempBuff,0,sizeof(TempBuff));
  bcd2zasc(TempBuff,(char*)pAdd,(TagLen*2)); if (TempBuff[(TagLen*2)-1] == '?') TempBuff[(TagLen*2)-1] = '\0';
  m_pContainer->set(FIELD_CARD_NUM,	string((char*)TempBuff, strlen(TempBuff)));*/
//  m_pContainer->set(FIELD_PROCESS_CODE,	string((char*)"000000", 6));
  TagLen = getDnL((LPBYTE)EMVTags, TagsStrLen, T6n_TrmAmountAuthorised, &pAdd);   memset(TempBuff,0,sizeof(TempBuff));
  bcd2asc(TempBuff,(LPTSTR)pAdd,(TagLen * 2));
  m_pContainer->set(FIELD_TRN_AMOUNT,	string((char*)TempBuff, strlen((char*)TempBuff)));
  m_pContainer->set(FIELD_STAN,	string((uint)DataSave3->ulSysTraceAuditNo,6));

  sprintf(TempBuff,"%03x",(PEntryModeICC|PEntryModePinEntry));
  m_pContainer->set(FIELD_ENTRY_MODE,	string(TempBuff, 3));
  sprintf(TempBuff,"%02x",(PCC_Normal));
  m_pContainer->set(FIELD_COND_MODE,	string(TempBuff, 2));
  m_pContainer->set(FIELD_FUNCTION_CODE, string("001", 3));


  TagLen = getDnL((LPBYTE)EMVTags, TagsStrLen, Tvb_Track2EquivalentData, &pAdd); memset(TempBuff,0,sizeof(TempBuff));
  bcd2zasc(TempBuff,(char*)pAdd,(TagLen*2)); if (TempBuff[(TagLen*2)-1] == '?') TempBuff[(TagLen*2)-1] = '\0';
  m_pContainer->set(FIELD_TRACK_2,	string((char*)TempBuff, strlen(TempBuff)));

  m_pContainer->set(FIELD_TERM_NUM,	string((char*)DataSave1->udtDeviceConfig.aucTermId, strlen((char*)DataSave1->udtDeviceConfig.aucTermId)));
  m_pContainer->set(FIELD_OUTLET_NUM,	string((char*)DataSave1->udtDeviceConfig.aucMerchantId, strlen((char*)DataSave1->udtDeviceConfig.aucMerchantId)));

//  TagLen = getDnL((LPBYTE)EMVTags, TagsStrLen, T2n_TrmTrxnCurrencyCode, &pAdd); memset(TempBuff,0,sizeof(TempBuff));
//  bcd2asc(TempBuff,(LPTSTR)pAdd,(TagLen*2));
//  m_pContainer->set(FIELD_TRN_CURRENCY_CODE,	string(&TempBuff[1], LENGTH_TRN_CURRENCY_CODE));
  m_pContainer->set(FIELD_REDEMPTION_NUM,	string((char*)"000023", strlen("000023")));


  extern TTrxnRec gsXRec;
  int len=trans_UPDATEgsXRec_ICCbL3();
  ptr=(U08 *)&gsPOS.TrxnRec.Trns_ICCbL3;
  memcpy(ptr, (U08 *)&gsXRec.Trns_ICCbL3, len);
  memset(TempBuff,0,sizeof(TempBuff));
  memcpy(TempBuff, (U08 *)&gsXRec.Trns_ICCbL3, len);
  m_pContainer->set(FIELD_ICC_DATACAPTURE1,	string((char*)TempBuff, len));

  m_pContainer->setType(MTID_SALE_REQUEST, "000000");
  uchar ucResult;
  if ((ucResult = COMMS_Connect((char*)DataSave1->udtConnSettings.aucServerIP,
                                 (char*)DataSave1->udtConnSettings.aucServerPort,
                                 (DataSave1->udtConnSettings.ulHostTimeout*1000))) != SUCCESS){return ucResult;}

  ucResult = TransSendReceive();
  int i,j;
//  U08 *pAdd, *pPtr;
  if ( ucResult  == SUCCESS) {
	string result = m_pContainer->get(FIELD_ICC_DATACAPTURE1);
	DBGLine("ICC DATA LEN = %d",result.length());

    i=app_UPDATEgsXRec_ICCbL3();
    pPtr=(U08 *)&gsPOS.TrxnRec.ICCbL3;
    memcpy(pPtr, (U08 *)&gsXRec.ICCbL3, i);
    // Response Messsage Only
//    if(gbOnLineFlag==1)
    {
        if((j= getDnL((LPBYTE)result.buffer(), result.length(),Tvb_IAD , &pAdd)) > 0) {
            pPtr[i++]=(U08)j;
            memcpy(&pPtr[i], pAdd, j); i += j;
        }else {
            pPtr[i++]=0x00;
        }
        if((j=getDnL((LPBYTE)result.buffer(), result.length(),Ttt_IssuerScript1 , &pAdd)) > 0) {
            //ICCbL3_MAX = 148 and dummy[128]
            if((i+j) >= (ICCbL3_MAX+128)) j=((ICCbL3_MAX-127)-i);
            pPtr[i++]=(U08)j;
            memcpy(&pPtr[i], pAdd, j); i += j;
        }else {
            pPtr[i++]=0x00;
        }
//		int2LLL((LPTSTR)pPtr, (i-2));

        fnD642Bcd(pPtr, (i-2), 3);
    }
//	int TagLen = getDnL((LPBYTE)result.buffer(), result.length(),Tvb_IAD , &pAdd);
//	if (TagLen > 0) {pgbAppBufUpdate(Tvb_IAD, TagLen, pAdd);DBGBuf("Retrn TAG", (LPBYTE)result.buffer(),result.length());}
//	TagLen = getDnL((LPBYTE)result.buffer(), result.length(),Ttt_IssuerScript1 , &pAdd);
//	if (TagLen > 0) {pgbAppBufUpdate(Ttt_IssuerScript1, TagLen, pAdd);DBGBuf("Retrn TAG", (LPBYTE)result.buffer(),result.length());}
//	TagLen = getDnL((LPBYTE)result.buffer(), result.length(),Ttt_IssuerScript2 , &pAdd);
//	if (TagLen > 0) {pgbAppBufUpdate(Ttt_IssuerScript2, TagLen, pAdd);DBGBuf("Retrn TAG", (LPBYTE)result.buffer(),result.length());}

/*	LPTSTR curr = result.buffer();
	int SubfldLen = 0, SubfldId = 0;
	char TempBuff[20] = {0};
	while ((curr - result.buffer()) < result.length()){
	  memcpy(TempBuff, curr, 4);
	  SubfldLen = atol(TempBuff); curr += 4;
	  memset(TempBuff,0,sizeof(TempBuff)); memcpy(TempBuff, curr, 2); curr += 2;
	  SubfldId = atol(TempBuff);
	  memset(TempBuff,0,sizeof(TempBuff));
	  asc2bcd(TempBuff, curr, (SubfldLen - 2));
	  DBGBuf("Retrn TAG", (LPBYTE)TempBuff,(SubfldLen - 2)/2);
	  if (SubfldId == 2)
		pgbAppBufUpdate(Ttt_IssuerScript1, (SubfldLen - 2)/2, (LPBYTE)TempBuff);
	  else if (SubfldId == 3)
		pgbAppBufUpdate(Ttt_IssuerScript2, (SubfldLen - 2)/2, (LPBYTE)TempBuff);
	  else if (SubfldId == 6)
		pgbAppBufUpdate(Tvb_IAD, (SubfldLen - 2)/2, (LPBYTE)TempBuff);
	  curr += (SubfldLen - 2);
	}*/

	result = m_pContainer->get(FIELD_ACTION_CODE);
//	pgbAppBufUpdate(T2a_ARC, 2, (LPBYTE)result.buffer());

	DEBUG_BUF(((LPBYTE)result.buffer(),2));
	gwTrmARC = (result.buffer()[0] << 8) + result.buffer()[1]; //shall not set ARC book4 2.5.2.1
	gbTempBuf[0] = (gwTrmARC >> 8) & 0xFF;
	gbTempBuf[1] = gwTrmARC & 0xFF;
	pgbAppBufUpdate(T2a_ARC, 2, gbTempBuf);

	TagLen = getDnL(pgbAppBuf, giAppBufLen,T2a_ARC , &pAdd);
	DEBUG_BUF(((LPBYTE)pAdd,TagLen));
	DBGLine("T2a_ARC:%s",result.buffer());
    ghResponseCode = 0x3030;
    gwResponseCode=0x3030; // Always approve
    gsXRec.ResponseCode=0x3030;




//    if((gbOnLineFlag==1)||(gwResponseCode==0)) {
//      if(getDnL(pgcTrmDATA, 0, T2a_ARC, &pAdd) == 2) {
//    	gwResponseCode=((U16)pAdd[0]<<8) | pAdd[1];
//      }else {
//    	gwResponseCode=0x3030;
//      }
//    }

//	gwTrmARC = 0x3030; //shall not set ARC book4 2.5.2.1
//	gbTempBuf[0] = (gwTrmARC >> 8) & 0xFF;
//	gbTempBuf[1] = gwTrmARC & 0xFF;
//	pgbAppBufUpdate(T2a_ARC, 2, gbTempBuf);

//    memcpy((U08 *)&gsXRec.ResponseCode, dPtr, dLen);
//    ghResponseCode = (U16)(dPtr[0]<<8) | (U16)dPtr[1];
//    if((ghResponseCode==0x3030)||(ghResponseCode==0x3130)||(ghResponseCode==0x3131)) {
//        gwResponseCode=0x3030;
//    }else {
//        gwResponseCode=ghResponseCode;
//    }


	//ucGetFileUpdtFlags();
  }
  //	getDnL(pgbAppBuf, giAppBufLen, Tvb_Track2EquivalentData, &pAdd);
//  m_pContainer->set(FIELD_CARD_CODE,	string((char*)udtBrdRunData.udtInsCardData.aucInsideCode, strlen((char*)udtBrdRunData.udtInsCardData.aucInsideCode)));
//  m_pContainer->set(LENGTH_GOV_CODE,	string((char*)udtBrdRunData.udtInsCardData.aucGovernarate, strlen((char*)udtBrdRunData.udtInsCardData.aucGovernarate)));

  COMMS_DisConnect();
  return ucResult;

}
