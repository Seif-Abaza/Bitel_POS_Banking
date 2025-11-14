/*
 * CTMS.cpp
 *
 *  Created on: ???/???/????
 *      Author: Mahmoud Swedan
 */

extern "C"{
#include "AppGlobal.h"
#include "SndRcvIso8583.h"
#include "Display.h"
#include "Utilities.h"
#include "../GZIP/zipmem.h"
  extern int WRITE_APP_PROGRAM(long count, char *src_addr, char *des_addr, char code);		/* KHLEE 04/12/11 */
  extern int FLASH_SECTOR_ERASE(char *flash_ROM_physical_address);
}
#include "../internal/tools.h"
#include "CTMS.h"

#ifndef min
#define min(X, Y) ((X) < (Y) ? (X) : (Y))
#endif

#ifndef max
#define max(X, Y) ((X) > (Y) ? (X) : (Y))
#endif


ulong CTMS::m_calcCRC32(LPBYTE pFile, ulong FileSize)
{
	const int nTileSize = 2*(0x8000);
	int cnTiles = FileSize / nTileSize + ((FileSize % nTileSize) ? 1 : 0);
	ulong crc32 = 0;

	DBGLine("cnTiles")
	crc32 = ::updcrc(0,0);
//	for (int i = 0; i < cnTiles; i++)
//	{
//		crc32 = ::updcrc( pFile, min(nTileSize,(FileSize - (i * nTileSize))));
//	}
	crc32 = ::updcrc( pFile, FileSize);
	return crc32;/**/
  //return ::crc32(0xffffffffL, (LPBYTE)pFile, FileSize);
}

unsigned long CTMS::hex_long(unsigned char *ucIp,unsigned char len)
{
  int i;
  unsigned long ret=0;
  for(i=0;i<len;i++)
  {
	ret+=(ucIp[i] << ((len-i-1)*8));
  }
  return ret;
}
BYTE CTMS::long_hex(unsigned char *ucDest,unsigned char len,unsigned long ulVal)
{
  unsigned char tmp[20];
  int i;
  for (i=0;i<len;i++)
  {
	tmp[len-i-1]=((0xFF) & (( ulVal)>>(i*8)));

  }
  memcpy(ucDest,tmp,len);
  return len;
}

CTMS::CTMS (SwFile * pSwFile)
{
  m_pZipFile = pSwFile;
  // TODO Auto-generated constructor stub

}

CTMS::~CTMS ()
{
  // TODO Auto-generated destructor stub
}
// first write

uchar CTMS::gethostbyname(LPCTSTR hostname, char* IP ){
  /* RFC 1035*/
  BYTE DNSQuery[200];
  char bhostname[200];
  uchar ucResult;
  BYTE SndRcvBuf[1000];

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
    DBGLine("%s",pch);
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
  DBGBuf("DNSQuery",DNSQuery,19 + curr_pos);
  if ((ucResult = COMMS_Connect("208.67.222.222","53",40000)) != SUCCESS){return ucResult;}// 208.67.220.220 //208.67.222.222 //156.154.70.1 and 156.154.71.1
  m_bIsConnected = true;
  ucResult = GPRS_SendData((LPBYTE)DNSQuery, (2 + hex_long(DNSQuery,2)));
  int     recvLen = 0;
  if ((ucResult=GPRS_ReceiveData(&SndRcvBuf[0],&recvLen, 40000)) != SUCCESS){return ERR_GNRL;}
  DBGBuf("SndRcvBuf", SndRcvBuf, recvLen);

  if ((!memcmp(&DNSQuery[2],&SndRcvBuf[2],2)) && ((SndRcvBuf[5] & 0x0f) == 0) &&
	  (!memcmp(&DNSQuery[14],&SndRcvBuf[14],curr_pos))){
	ulong ANCOUNT = hex_long(&SndRcvBuf[8],2); //  an unsigned 16 bit integer specifying the number of resource records in the answer section.
	DBGLine("ANCOUNT:%d",ANCOUNT);
	BYTE* ReplyCurr = &SndRcvBuf[14 + curr_pos + 1 + 4 ];
	//      for( int i=0; i< ANCOUNT; i++){
	while (ReplyCurr < SndRcvBuf + recvLen){
	  if (!memcmp(&ReplyCurr[2],"\x00\x01\x00\x01",4)){
		break;
	  }
	  DBGBuf("ReplyCurr",ReplyCurr,50);
	  DBGLine("Reply Curr Inc %d", (12 + hex_long((LPBYTE)&ReplyCurr[10],2)));
	  ReplyCurr += (12 + hex_long((LPBYTE)&ReplyCurr[10],2));
	}
    if ( ReplyCurr < (SndRcvBuf + recvLen)){
      sprintf(IP,"%d.%d.%d.%d",ReplyCurr[12],ReplyCurr[13],ReplyCurr[14],ReplyCurr[15]);
      DBGLine("IP: %s", IP);
    }
  }

    if (m_bIsConnected)
  	COMMS_DisConnect();
}

uchar CTMS::ucDownloadSW(char* pcIP, char* pcPort, char* pcFilePath)
{
  int     recvLen = 0;
  uchar ucResult;
  char * pch;

  DWORD m_dwNOfBytes = 0;
  DWORD dwNOfBytes = 0;

  BYTE aucDataSize[50];
  LPBYTE SndRcvBuf = new BYTE[1000];  AUTO_DELETE_ARR(BYTE, SndRcvBuf);
  //gethostbyname((LPTSTR)DataSave1->udtConnSettings.aucServerURL, (char*)aucDataSize);

  memset((void *)SndRcvBuf,(int)0,(1000));

  sprintf((LPTSTR)&SndRcvBuf[0],	"HEAD %s HTTP/1.1\r\n", pcFilePath);
  sprintf((LPTSTR)&SndRcvBuf[strlen((LPTSTR)SndRcvBuf)],	"Host: %s:%s\r\n", pcIP, pcPort);
  sprintf((LPTSTR)&SndRcvBuf[strlen((LPTSTR)SndRcvBuf)],	"\r\n\r\n");

  if ((ucResult = COMMS_Connect(pcIP,pcPort,40000)) != SUCCESS){return ucResult;}
  m_bIsConnected = true;

  ucResult = GPRS_SendData((LPBYTE)SndRcvBuf, (int) strlen((LPCTSTR)SndRcvBuf));
  memset((void *)SndRcvBuf,(int)0,(1000));
  if ((ucResult=GPRS_ReceiveData(&SndRcvBuf[0],&recvLen, 40000)) != SUCCESS){return ERR_GNRL;}
  if (!(pch = strstr ((LPCTSTR)SndRcvBuf,"HTTP/1.1 200 OK\r\n"))) return ERR_GNRL;
  if (!(pch = strstr ((LPCTSTR)SndRcvBuf,"Content-Length: "))) return ERR_GNRL;
  ulong DataSize = atol((LPCTSTR)(pch+strlen("Content-Length: ")));
  if (!(pch = strstr ((LPCTSTR)SndRcvBuf,"Last-Modified: "))) return ERR_GNRL;
  DBGLine("DataSize : %u",DataSize);
  //COMMS_DisConnect();

  sprintf((LPTSTR)&SndRcvBuf[1],	"%s:%s%s\r\n", pcIP, pcPort, pcFilePath);
  //long_hex(SndRcvBuf,1,(ulong)min(200,strlen((LPCTSTR)&SndRcvBuf[1])));
  SndRcvBuf[0]= min(200,strlen((LPCTSTR)&SndRcvBuf[1]));
  long_hex(aucDataSize,4, DataSize);

  DBGLine("FileNameLen: %d, ", SndRcvBuf[0]);
  DBGBuf("SndRcv FileName", SndRcvBuf,SndRcvBuf[0]);
  DBGBuf("FileName", (LPBYTE)m_pZipFile->FileName,min(200,m_pZipFile->FileName[0]));

  DBGBuf("SndRcv FileSize", aucDataSize,4);
  DBGBuf("FileSize",(LPBYTE) m_pZipFile->FileSize,4);

  DBGBuf("SndRcv LastModified",(LPBYTE) (pch + 16),40);
  DBGBuf("LastModified", (LPBYTE)(m_pZipFile->LastModified),40);

  if ( (memcmp(SndRcvBuf,(void *)m_pZipFile->FileName,max(m_pZipFile->FileName[0],SndRcvBuf[0])) != 0) ||
	  (memcmp(aucDataSize,(void *)m_pZipFile->FileSize, 4) != 0) ||
	  (memcmp(pch + 16,(void *)m_pZipFile->LastModified, 40) != 0))
  {
	DBGLine("Different File");
	FLASH_WRITE((LPTSTR)&m_pZipFile->FileName,SndRcvBuf[0],(LPTSTR)SndRcvBuf);
	FLASH_WRITE((LPTSTR)&m_pZipFile->FileSize,4,(LPTSTR)aucDataSize);
	FLASH_WRITE((LPTSTR)&m_pZipFile->DwnloadedSize,4,(LPTSTR)"\x00\x00\x00\x00");
	FLASH_WRITE((LPTSTR)&m_pZipFile->LastModified,40,(LPTSTR)(pch + 16));
	m_dwNOfBytes = 0;
  }
  else if (memcmp((void *)m_pZipFile->FileSize, (void *)m_pZipFile->DwnloadedSize, 4) == 0){ DBGLine("File Downloaded Before"); return SUCCESS;}
  else { DBGLine("Continue Previous File"); m_dwNOfBytes = hex_long((LPBYTE)m_pZipFile->DwnloadedSize,4); }

	DBGLine("m_dwNOfBytes : %u", m_dwNOfBytes);
  do {
	if ((dwNOfBytes = ulPrepGetHTTPRequest(pcIP, pcPort, pcFilePath, m_dwNOfBytes,min(DataSize,(m_dwNOfBytes+FILECHNKLEN)))) == 0) {break;}
	m_dwNOfBytes = hex_long((LPBYTE)m_pZipFile->DwnloadedSize, 4);
	//m_dwNOfBytes += dwNOfBytes;
	DBGLine("m_dwNOfBytes : %u", m_dwNOfBytes);
	DBGLine("dwNOfBytes : %u", dwNOfBytes);
  }while (m_dwNOfBytes  < DataSize);
  if (m_bIsConnected)
	COMMS_DisConnect();

}

ulong CTMS::ulPrepGetHTTPRequest(char* pcIP, char* pcPort,
                                 char* pcFilePath,
                                 ulong ulStrIdx,
                                 ulong ulEndIdx)
{
  const char GETReq[]=
  {
	  "Connection: keep-alive\r\n"
	  "Accept: application/gzip \r\n"
	  "User-Agent: Apache-HttpClient/4.1.1 (java 1.5)\r\n"
	  //"Accept-Encoding: gzip\r\n"
	  "Accept-Language: en-US,en;q=0.8,ar;q=0.6\r\n"
	  "\r\n\r\n\0"
  };
  uchar ucResult;
  bool bDisconnect = false;
  LPBYTE SndRcvBuf = new BYTE[ulEndIdx - ulStrIdx + 10]; AUTO_DELETE_ARR(BYTE, SndRcvBuf);
  int recvLen = 0;
  ulong DataSize;
  LPTSTR ch;
  char acDwnloadedSize[5];
  LPTSTR pData = (LPTSTR)&m_pZipFile->Data[ulStrIdx];
  if (!SndRcvBuf) return ERR_GNRL;

  memset((void *)SndRcvBuf,(int)0,(ulEndIdx - ulStrIdx));
  sprintf((LPTSTR)&SndRcvBuf[0],	"GET %s HTTP/1.1\r\n", pcFilePath);
  sprintf((LPTSTR)&SndRcvBuf[strlen((LPTSTR)SndRcvBuf)],	"Host: %s:%s\r\n", pcIP, pcPort);
  if (!ulEndIdx) {
	if (ulStrIdx > 0)
	  sprintf((LPTSTR)&SndRcvBuf[strlen((LPTSTR)SndRcvBuf)],"Range: bytes=%u-\r\n", ulStrIdx);
  } else sprintf((LPTSTR)&SndRcvBuf[strlen((LPTSTR)SndRcvBuf)],"Range: bytes=%u-%u\r\n", ulStrIdx,ulEndIdx);
  strcat((LPTSTR)SndRcvBuf,(LPCTSTR)GETReq);

  if (!m_bIsConnected){
	if ((ucResult = COMMS_Connect(pcIP, pcPort, 20000)) != SUCCESS){return ucResult;}
	m_bIsConnected = true;
  }
  DBGLine("SndRcvBuf:%s",SndRcvBuf);
  ucResult = GPRS_SendData((LPBYTE)SndRcvBuf, (int)strlen((LPCTSTR)SndRcvBuf));
  memset((void *)SndRcvBuf,0,(ulEndIdx - ulStrIdx));

  int nHdrLen = 0;
  ulong dwNOfBytes = 0;
  ulong Temp = 0;
  ulong dwTotalNOfBytes = 0;

  do {
	recvLen = 0;
	if ((ucResult=GPRS_ReceiveData((LPBYTE)&SndRcvBuf[dwNOfBytes],&recvLen,20000)) != SUCCESS){bDisconnect = true; break;}
	if (!nHdrLen) {
	  if (!(ch = (LPTSTR)strstr((const char *)SndRcvBuf, "Content-Length: "))) { DBGLine("Content-Length Missing"); break;}
	  DataSize = atol(ch+16);
	  if (!(ch = strstr ((LPCTSTR)SndRcvBuf,"HTTP/1.1 200 OK\r\n")) &&
		  !(ch = strstr ((LPCTSTR)SndRcvBuf,"HTTP/1.1 206 Partial Content\r\n"))) {DBGLine("HTTP ERROR");break;}
	  if ((ch = (LPTSTR)strstr((const char *)SndRcvBuf, "\r\n\r\n"))){
		nHdrLen = (int) (ch - (LPTSTR)SndRcvBuf) + 4;
		recvLen -= nHdrLen;
		LPBYTE TempBuf = new BYTE [recvLen + 1];
		memcpy((void*)TempBuf, &SndRcvBuf[nHdrLen] ,recvLen);
		memset(SndRcvBuf,0,recvLen+4);
		memcpy((void*)SndRcvBuf ,TempBuf ,recvLen);
		DBGLine("nHdrLen:%d DataSize:%d", nHdrLen, DataSize);
	  } else {DBGLine("EOH Missing"); break;}
	}
	DBGBuf("SndRcvBuf",(LPBYTE)&SndRcvBuf[dwNOfBytes], 15);
	dwNOfBytes+= recvLen;
	//Temp += recvLen;
	DBGPrt(" Recived %d  AccNOfBytes %d ", recvLen, dwNOfBytes);
//	recvLen = 0;

/*	if (dwNOfBytes >= FILESAVELEN){
	  long_hex((LPBYTE)acDwnloadedSize, 4, dwNOfBytes + hex_long((LPBYTE)m_pZipFile->DwnloadedSize, 4));
	  FLASH_WRITE((LPTSTR)&m_pZipFile->Data[ulStrIdx + dwTotalNOfBytes], (long) dwNOfBytes, (LPTSTR)SndRcvBuf);
	  FLASH_WRITE((LPTSTR)m_pZipFile->DwnloadedSize, (long) 4, (LPTSTR)acDwnloadedSize);
	  memset(SndRcvBuf,0,dwNOfBytes);
	  dwTotalNOfBytes += dwNOfBytes;
	  dwNOfBytes = 0;
	}*/
	DBGLine(" TotalNOfBytes %d Temp 0x%08lX", (ulStrIdx + dwNOfBytes), (ulStrIdx + dwNOfBytes));
	/*	if (dwNOfBytes){
	  if ((ucResult=GPRS_ReceiveData((LPBYTE)&TMSProgram[ulStrIdx + dwNOfBytes],&recvLen,10000)) != SUCCESS){break;}
	} else {
	  if ((ucResult=GPRS_ReceiveData((LPBYTE)SndRcvBuf,&recvLen,10000)) != SUCCESS){break;}
	  ch = (LPTSTR)memmem((const void *)SndRcvBuf, (ulong)recvLen, "\r\n\r\n",4);
	  if (ch){
		nHdrLen = (int) (ch - (LPTSTR)SndRcvBuf) + 4;
		recvLen -= nHdrLen;
		memcpy((void*)&TMSProgram[ulStrIdx], &SndRcvBuf[nHdrLen] ,recvLen);
		DBGLine("nHdrLen:%d",nHdrLen);
	  } else {break;}
	}
	dwNOfBytes+= recvLen;
	DBGLine(" Recived %d  AccNOfBytes %d", recvLen, dwNOfBytes);
	 */
  } while (dwNOfBytes < DataSize);

  DBGLine("dwNOfBytes: %u recvLen: %u",dwNOfBytes, recvLen);
  DBGBuf("SndRcvBuf",(LPBYTE)&SndRcvBuf[dwNOfBytes], 15);
  DBGBuf("SndRcvBuf",(LPBYTE)(SndRcvBuf + dwNOfBytes - 500), 500);

  if (dwNOfBytes > 0){
	long_hex((LPBYTE)acDwnloadedSize, 4, dwNOfBytes + hex_long((LPBYTE)m_pZipFile->DwnloadedSize, 4));
	FLASH_WRITE((LPTSTR)&m_pZipFile->Data[ulStrIdx],(long) dwNOfBytes,(LPTSTR)SndRcvBuf);
	FLASH_WRITE((LPTSTR)m_pZipFile->DwnloadedSize,(long) 4,(LPTSTR)acDwnloadedSize);
  }
  if (bDisconnect){
	COMMS_DisConnect();
	m_bIsConnected = false;
  }
  return dwNOfBytes;
}

uchar CTMS::ucInstallSW()
{

  if (memcmp((void *)m_pZipFile->FileSize, (void *)m_pZipFile->DwnloadedSize, 4) != 0){DBGLine("File Not Completely Download"); return ERR_GNRL;}
  ulong ulSwSize = hex_long((LPBYTE)m_pZipFile->FileSize,4);
	/*  ulong DataSize = 63607;*/
  DBGLine("ulSwSize: %u", ulSwSize);
  DBGLine("ZIPAdrr: 0x%08lX", m_pZipFile->Data);
  DBGBuf("ZIP",(LPBYTE)m_pZipFile->Data, 500);
  DBGBuf("ZIP",(LPBYTE)&m_pZipFile->Data[ulSwSize - 500], 500);

  volatile  SwFile * const SWFile = ( SwFile *) (m_pZipFile->Data +(((ulSwSize/0x10000L)+(ulSwSize%0x10000L > 0))*0x10000L));
  int nOutLen = unzipmem((unsigned char *)m_pZipFile->Data, ulSwSize, (unsigned char *)SWFile->Data);
  DBGLine("nOutLen: %d", nOutLen);
  DBGLine("ProgAdrr: 0x%08lX", SWFile);
  DBGBuf("APP",(LPBYTE)SWFile->Data, 500);
  DBGBuf("APP",(LPBYTE)(SWFile->Data + nOutLen - 500), 500);
  DWORD dwCRC = m_calcCRC32((LPBYTE)SWFile->Data, (DWORD)nOutLen);
  BYTE aucCRC[5];
  long_hex(aucCRC,4,dwCRC);
  DBGBuf("aucCRC",aucCRC,4);
  bool IsCRCCorrect = true;
  for (uchar i; i<4; i++){
	if (aucCRC[3-i]!= m_pZipFile->Data[ulSwSize- 8 + i]){ IsCRCCorrect = false; break;}
  }
  if (IsCRCCorrect)
	WRITE_APP_PROGRAM((long)nOutLen,(char *)SWFile->Data ,(char*)0x30000000, 0);
}
