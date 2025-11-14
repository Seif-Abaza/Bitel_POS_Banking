/*
 * CTMS.h
 *
 *  Created on: ???/???/????
 *      Author: Mahmoud Swedan
 */

#ifndef CTMS_H_
#define CTMS_H_
#define APP_BEGIN		(0x30400000UL)
#define FILESAVELEN  (0x100000L)
#define FILECHNKLEN  (10000)//(0x10000L)

/*typedef struct{
  BYTE FileSize[4];
  BYTE DwnloadedSize[4];
  BYTE Data[1];
}SwZipFile;*/
typedef struct{
  BYTE FileName[200];
  BYTE LastModified[41];
  BYTE FileSize[4];
  BYTE DwnloadedSize[4];
  BYTE Data[1];
}SwFile;
class CTMS
{
public:
  CTMS () : m_pZipFile(( SwFile *)APP_BEGIN), m_bIsConnected(false){};
  CTMS (SwFile * pSwFile);
  virtual ~CTMS ();
   uchar ucDownloadSW(char* pcIP, char* pcPort, char* pcFilePath);
   uchar gethostbyname(LPCTSTR hostname, char* IP );
   uchar ucInstallSW();
   ulong ulPrepGetHTTPRequest(char* pcIP, char* pcPort, char* pcFilePath,
                                    ulong ulStrIdx, ulong ulEndIdx );
private:
   bool m_bIsConnected;
  volatile  SwFile *  m_pZipFile;
  BYTE long_hex(unsigned char *ucDest,unsigned char len,unsigned long ulVal);
  unsigned long CTMS::hex_long(unsigned char *ucIp,unsigned char len);
  ulong CTMS::m_calcCRC32(LPBYTE pFile, ulong FileSize);
};

#endif /* CTMS_H_ */
