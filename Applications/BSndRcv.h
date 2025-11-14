/*
 * BSndRcv.h
 *
 *  Created on: Jun 18, 2014
 *      Author: Mahmoud Swedan
 */

#ifndef BSNDRCV_H_
#define BSNDRCV_H_

#include "../iso/CMegaContainer.h"

#define RCV_MSG_MAX_SIZE		2000
#define MAX_CHNK_FLDS  3

#define ERR_ISO_HOSTCODE                   0x70

typedef enum{
	UNDEF_FACT,
	END_OF_TRANS ,
	ADD_RECORD,
	MODIFY_RECORD,
	DELETE_RECORD,
	REPLACE_RECORD,
	REPLACE_FILE,
	DELETE_FILE,
	MAX_FILE_ACTIONS
}FileAction;

//#define FILE_TRANSFER_REQUEST

typedef struct{
	FileAction   udtFileAction;
	bool   bIsLastChunk;
	ulong ulMaxChunkno;
	char   acFileVersion[3];
	bool   bIsComprsd;
}strChnkAttr;

struct PrgPar{
	int iPrgParCurrValue;
	int iPrgParMaxValue;
	string strPrgParHdrMsg;
	string strPrgParDispMsg;
	PrgPar(const string& strPrgParHdrMsg, const string& strPrgParDispMsg,int iPrgParCurrValue , int iPrgParMaxValue)
	{this->strPrgParHdrMsg = strPrgParHdrMsg; this->strPrgParDispMsg = strPrgParDispMsg; this->iPrgParCurrValue = iPrgParCurrValue; this->iPrgParMaxValue = iPrgParMaxValue;}
};

class CSndRcv
{
public:
	CSndRcv();
	CSndRcv(const string& strPrgParHdrMsg, const string& strPrgParDispMsg);
	~CSndRcv();
	BYTE ucGetRequest();
	BYTE ucSettlSndRcv(int* pnLastSettBatch);
	BYTE ucBatchUpload(BYTE* pucBtchAddr, ulong ulBSz);
	BYTE ucDldFileChunk (BYTE* pucFileName, ulong ulChunkNo, string& strChunkData, strChnkAttr* pudtChnkAttr);
	BYTE ucUpdtCardInfoSndRcv();
	static BYTE SSLSendReceive();
	void SetSSLFlag(bool Enable){bEnableSSL = Enable;}
	BYTE ucCubeSale(char * EMVTags, int TagsStrLen);
	BYTE ucNBESale(char * EMVTags, int TagsStrLen);
	BYTE ucSoapNBESale(char * EMVTags, int TagsStrLen);


private:
	void vidUpdtPrgPar(int iPrgParCurrValue , int iPrgParMaxValue);
	BYTE TransSendReceive();
	void  vidIncSTAN();
	BYTE CheckResponseValid();

	BYTE ucChkResp(BYTE* pucFileName, ulong ulChunkNo);
	BYTE ucGetFileUpdtFlags(void);
	BYTE ucGetFileActions(FileAction*   pudtFileAction, bool* pbIsLastChunk);
	BYTE ucGetChnkIsoFlds(string& strChnkData);
	BYTE ucFileChunkSndRcv(BYTE* pucFileName, ulong ulChunkNo);

	BYTE ucBatchChunkUpload(BYTE* pucChunkAddr, ulong ulChunkSize, ulong ulChunkNo ,bool bIsLastChnk ,bool bIsComprsd);
	BYTE ucFileChunkSndRcv(BYTE* pucFileName, ulong ulChunkNo, string& strChnkData, strChnkAttr* pudtChnkAttr);

	CMegaContainer*	m_pContainer;
	PrgPar* m_pudtPrgPar;
	ulong m_ulChunkNo;
	bool m_bIsRstBtchCounter;
	LPBYTE m_sRcvBuf;
	bool bEnableSSL;

};



#endif /* BSNDRCV_H_ */
