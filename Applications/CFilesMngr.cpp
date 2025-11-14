/*
 * FilesMngr.cpp
 *
 *  Created on: Jun 20, 2014
 *      Author: Mahmoud Swedan
 */

extern "C" {
#include "../../BF_SDK/include/global.h"
#include "./fonts/fntmacro.h"
#include "./resources/Msgs.h"
#include "AppGlobal.h"
#include "Utilities.h"
#include "DeviceConfig.h"
#include "minilzo.h"

}
//#include "BTrans.h"
#include "../internal/string.h"
#include "../internal/tools.h"
#include "../fs/CFile.h"
#include "CFilesMngr.h"


uchar CFilesMngr::ucCompressMiniLZO(const uchar* in,uint uiInLen,uchar* out,uint* uioutLen)
{
    int r;
    lzo_uint in_len;
    lzo_uint out_len;
    lzo_uint new_len;

    Rprintf("\nLZO real-time data compression library (v%s, %s).\r\n",
           lzo_version_string(), lzo_version_date());
    Rprintf("Copyright (C) 1996-2011 Markus Franz Xaver Johannes Oberhumer\nAll Rights Reserved.\r\n");


/*
 * Step 1: initialize the LZO library
*/
    if (lzo_init() != LZO_E_OK)
    {
    	Rprintf("internal error - lzo_init() failed !!!\r\n");
    	Rprintf("(this usually indicates a compiler bug - try recompiling\nwithout optimizations, and enable '-DLZO_DEBUG' for diagnostics)\r\n");
        return 3;
    }


    in_len = uiInLen;

/*
 * Step 3: compress from 'in' to 'out' with LZO1X-1
 */
    lzo_align_t* wrkmem = new lzo_align_t [((LZO1X_1_MEM_COMPRESS) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t)];
    r = lzo1x_1_compress(in,in_len,out,&out_len,wrkmem);
	DELETE_POINTER_ARR(wrkmem);

    if (r == LZO_E_OK){
    	Rprintf("compressed %lu bytes into %lu bytes\r\n",
            (unsigned long) in_len, (unsigned long) out_len);
    	*uioutLen = out_len;
    }
    else
    {
        /* this should NEVER happen */
    	Rprintf("internal error - compression failed: %d\r\n", r);
        return 2;
    }
    /* check for an incompressible block */
    if (out_len >= in_len)
    {
    	Rprintf("This block contains incompressible data.\r\n");
        return 1;
    }
    return SUCCESS;
}
uchar CFilesMngr::ucInitMainBatchHdr(LPBYTE Date, LPBYTE Time)
{
  uchar aucMerchantId[8];
  uchar aucBatchNo[3];
  uchar aucSavedTransNo[2];
  uchar aucTotalAmount[4];
  uchar aucBatchOpenDate[3];
  uchar aucBatchCloseDate[3];
  MainBtchHdr* pudtBtchAddr = (MainBtchHdr*)&DataSave2->aucBatch[0];
  strInsCardData* pInsCardData = &udtBrdRunData.udtInsCardData;

  str_to_num((LPTSTR)pudtBtchAddr->aucMerchantId,(char*)DataSave1->udtDeviceConfig.aucMerchantId,15);
  long_hex(pudtBtchAddr->aucMainBtchCounter,3,DataSave3->ulMainBtchCounter);
  long_hex(pudtBtchAddr->aucDistSettlNo,2,DataSave2->ulDistSettlNo);
  long_hex(pudtBtchAddr->aucTotalAmount,4,0);
  long_hex(pudtBtchAddr->aucSavedTransNo,4,0);
  long_hex(pudtBtchAddr->aucInspTotalAmount,4,0);
  long_hex(pudtBtchAddr->aucInspSavedTransNo,4,0);
  memcpy((void*)pudtBtchAddr->aucMainBatchOpenDate,(void*)&Date[1],3);
  memcpy((void*)pudtBtchAddr->aucMainBatchOpenTime,(void*)&Time[0],3);

  DBGBuf("1 - pudtMainBtchHdr",(const BYTE*)pudtBtchAddr,sizeof(MainBtchHdr));

  DataSave2->ulBatchStartWrIdx = sizeof(MainBtchHdr);//pudtBtchAddr - DataSave2->aucBatch;
}

uchar CFilesMngr::ucAdjustMainBatchHdr(LPBYTE Date, LPBYTE Time){
  MainBtchHdr* pudtMainBtchHdr = (MainBtchHdr *)&DataSave2->aucBatch[0];
  volatile strTransLog*  pTransLog= & DataSave2->udtTransLog;

  long_hex((LPBYTE)pudtMainBtchHdr->aucDistSettlNo,2,(hex_long((LPBYTE)pudtMainBtchHdr->aucDistSettlNo,2)+1));
  long_hex((LPBYTE)pudtMainBtchHdr->aucSavedTransNo,4,(hex_long((LPBYTE)pudtMainBtchHdr->aucSavedTransNo,4) + hex_long((LPBYTE)pTransLog->LogHdr.aucSavedTransNo,2)));
  long_hex((LPBYTE)pudtMainBtchHdr->aucTotalAmount,4,(hex_long((LPBYTE)pudtMainBtchHdr->aucTotalAmount,4) + hex_long((LPBYTE)pTransLog->LogHdr.aucTotalAmount,4)));
  long_hex((LPBYTE)pudtMainBtchHdr->aucInspSavedTransNo,4,(hex_long((LPBYTE)pudtMainBtchHdr->aucInspSavedTransNo,4) + hex_long((LPBYTE)pTransLog->LogHdr.aucInspSavedTransNo,2)));
  long_hex((LPBYTE)pudtMainBtchHdr->aucInspTotalAmount,4,(hex_long((LPBYTE)pudtMainBtchHdr->aucInspTotalAmount,4) + hex_long((LPBYTE)pTransLog->LogHdr.aucInspTotalAmount,4)));
  memcpy((void*)pudtMainBtchHdr->aucMainBatchCloseDate,(void*)Date,3);
  memcpy((void*)pudtMainBtchHdr->aucMainBatchCloseTime,(void*)Time,3);

  DBGBuf("2 - pudtMainBtchHdr",(const BYTE*)pudtMainBtchHdr,sizeof(MainBtchHdr));
}

uchar CFilesMngr::ucGenBatchMsg()
{
  DBGLine("Fn ucGenBatchMsg()");

  if (DataSave2->bIsBatchGen) {return SUCCESS;}

  volatile strTransLog*  pTransLog= & DataSave2->udtTransLog;
  ulong ulSavedTransNo = hex_long((LPBYTE)pTransLog->LogHdr.aucSavedTransNo,2);
  ulong ulInspSavedTransNo = hex_long((LPBYTE)pTransLog->LogHdr.aucInspSavedTransNo,2);

  if ((ulSavedTransNo + ulInspSavedTransNo) == 0) {return SUCCESS;}

  uchar aucTime[9], aucDate[9];
  getDateTime((char*)aucDate,(char*)aucTime,true);

  uchar ucHdrSize = sizeof(strLogHdr); //19 + 4;
  uchar ucUsedDateTableSize = (1+ (4 * pTransLog->udtDatesInBatch.ucBatchNoDates));
  uchar ucTranslogSize = sizeof(strTransLogData)* ulSavedTransNo;
  ulong ulBatchSize = ucHdrSize + ucUsedDateTableSize + ucTranslogSize ;

  if (DataSave2->ulBatchStartWrIdx == 0){
	ucInitMainBatchHdr(aucDate, aucTime);
  }

  if ( DataSave2->ulBatchStartWrIdx + ulBatchSize >=BATCH_SIZE_BYTES){
	  DBGLine("Batch Full !!");
	  return BTCH_FULL_ERR;
  }

  DBGLine("ulDistSettlNo:%d ulBatchStartWrIdx:%d",DataSave2->ulDistSettlNo,DataSave2->ulBatchStartWrIdx);

  uchar* pucBtchAddr = (uchar*)&DataSave2->aucBatch[DataSave2->ulBatchStartWrIdx];
  uchar* pucTransLog = (uchar*)pTransLog;

  memcpy((void*)pTransLog->LogHdr.aucBatchCloseDate,(void*)&aucDate[1],3);
  memcpy((void*)pTransLog->LogHdr.aucBatchCloseTime,(void*)&aucTime[0],3);
  memcpy((void*)pucBtchAddr,(void*)pTransLog,ucHdrSize + ucUsedDateTableSize);
  memcpy((void*)&pucBtchAddr[ucHdrSize + ucUsedDateTableSize],(void*)&pucTransLog[ucHdrSize + sizeof(strDatesInBatch)],ucTranslogSize);

  DBGBuf("Batch Hdr", pucBtchAddr, ucHdrSize);

  ucAdjustMainBatchHdr(aucDate, aucTime);

  DataSave2->DistBatchInfo[DataSave2->ulDistSettlNo].aulDistBatchIdx= DataSave2->ulBatchStartWrIdx;
//  memcpy((void*)DataSave2->DistBatchInfo[DataSave2->ulDistSettlNo].aucMerchantId, (void*)pTransLog->LogHdr.aucMerchantId,8);
//  memcpy((void*)DataSave2->DistBatchInfo[DataSave2->ulDistSettlNo].aucSavedTransNo, (void*)pTransLog->LogHdr.aucSavedTransNo,2);
//  memcpy((void*)DataSave2->DistBatchInfo[DataSave2->ulDistSettlNo].aucTotalAmount, (void*)pTransLog->LogHdr.aucTotalAmount,4);
  DataSave2->ulDistSettlNo++;

  DataSave2->ulBatchStartWrIdx += (ucHdrSize + ucUsedDateTableSize + ucTranslogSize);

  DataSave3->ulBatchCounter++,
  memset((void *)&DataSave2->udtTransLog,0,sizeof(strTransLog));

  /*
	DBGLine("After ulBatchStartWrIdx:%d",DataSave2->ulBatchStartWrIdx);
	DBGLine("Batch size",ucHdrSize + ucUsedDateTableSize + ucTranslogSize);
	DBGBuf("Batch", pucBtchAddr, ucHdrSize + ucUsedDateTableSize + ucTranslogSize);
	DBGBuf("Full Batch", pucBtchAddr, DataSave2->ulBatchStartWrIdx);
   */
  ucCompressAllBatch();
  //DataSave2->bIsBatchGen = true;
  return SUCCESS;
  //m_oBTrans.ucDwnldFile((LPBYTE)BL_NAME);
  //	ulong ulBSz = ucHdrSize + ucUsedDateTableSize + ucTranslogSize;
  //	for(int i;i< 1000;i++){
  //		memcpy((void*)(pucBtchAddr+(i*ulBSz)),(void*)pucBtchAddr,ulBSz);
  //	}
  //	uint uiOutLen = 0;
  //	compressme(pucBtchAddr,(1000*ulBSz),(pucBtchAddr+(1000*ulBSz)+100),&uiOutLen);
  //	DBGBuf("Batch Compressed", (pucBtchAddr+(1000*ulBSz)+100), uiOutLen);

}
uchar CFilesMngr::ucCompressAllBatch()
{
  uchar* pucBtchAddr = (uchar*)&DataSave2->aucBatch;
  ulong ulBSz = DataSave2->ulBatchStartWrIdx;
  DataSave2->ulCompBatchSize = 0;
  uint uiOutLen = 0;

  DBGLine("Fn ucCompressAllBatch()");

  if (CFilesMngr::ucCompressMiniLZO(pucBtchAddr,(ulBSz),(pucBtchAddr+ ulBSz + BATCH_COMP_OFFSET),&uiOutLen)== SUCCESS){
	DataSave2->ulCompBatchSize = uiOutLen;
  }
  //	DBGBuf("Compressed Batch: ",(pucBtchAddr+ ulBSz + BATCH_COMP_OFFSET),DataSave2->ulCompBatchSize);
}
bool CFilesMngr::bLoadFileToMem(LPCTSTR sFile) {
	uchar* pucMemAddr;
	if (strcasecmp(sFile, WL_NAME) == 0) {
		memset(udtBrdRunData.aucWLMirror,0,sizeof(udtBrdRunData.aucWLMirror));
		pucMemAddr = udtBrdRunData.aucWLMirror;
	} else if (strcasecmp(sFile, DR_NAME) == 0) {
		memset(udtBrdRunData.aucDRMirror,0,sizeof(udtBrdRunData.aucDRMirror));
		pucMemAddr = udtBrdRunData.aucDRMirror;
	} else{ return false; }
	return CFile::readAllFile(sFile, pucMemAddr);
}
bool CFilesMngr::ucSaveFileVersion(LPCTSTR sFile, char* FileVersion) {
	char* pcFileVerAddr;
	if (strcasecmp(sFile, BL_NAME) == 0) {
		pcFileVerAddr = (char*) DataSave3->udtFilesVer.aucBLVer;
		//pcFileVerAddr = (char*) DataSave3->aucCompBLVer;
	} else if (strcasecmp(sFile, WL_NAME) == 0) {
		pcFileVerAddr = (char*) DataSave3->udtFilesVer.aucWLVer;
	} else if (strcasecmp(sFile, DR_NAME) == 0) {
		pcFileVerAddr = (char*) DataSave3->udtFilesVer.aucDRVer;
	} else if (strcasecmp(sFile, POSPARAN_NAME) == 0) {
		pcFileVerAddr = (char*) DataSave3->udtFilesVer.aucPOSParmVer;
	}
	memcpy((void*) pcFileVerAddr, (void*) FileVersion, 3);
}
uint CFilesMngr::WriteChunkFS(LPCTSTR sFile, LPBYTE pucInBuf, ulong ulInBufSz, bool bIsRemoveOld /* = false */, bool bIsCompressed /* = false */) {
	CFile oFile;
	string str(sFile,strlen(sFile));

	if (bIsCompressed) {
		str.append((LPCTSTR)COMP_TAG,(int)strlen(COMP_TAG));
	}
	if (bIsRemoveOld) {
		oFile.remove(str.buffer(), True);
		ucSaveFileVersion(str.buffer(), "\x00\x00\x00");
	}
	if (ulInBufSz == 0) {return true;}
	DBGLine("File Name:%s",str.buffer());
	if (!oFile.open(str.buffer(), O_APPEND)) {
		if (!oFile.open(str.buffer())) {return false;}
	}
	DBGLine("File Opened");

	bool bRetCode = (oFile.write((LPBYTE)pucInBuf, (DWORD)ulInBufSz) == ulInBufSz);

	DBGLine("bRetCode= %d",bRetCode);
	oFile.close();
	CFile::sync();

	return bRetCode;
}

ulong CFilesMngr::ucParseBLChnk(uchar* pucInBuf, ulong ulInBufSz, uchar* pucOutBuf, ulong* pulOutBufSz)
{
	ulong dst_cur_pos = 0;
	ulong src_cur_pos = 0;

	char* pcSrc = (char*)pucInBuf;
	char* pcDst = (char*)pucOutBuf;

	uchar CardLen;
	uchar BinLen;
	ulong ulAcumBin = 0;
	//ulong ulAcumCard = 0;
	//ulong ulNoOfCards = 0;

	while (src_cur_pos < ulInBufSz)
	{
		if ((m_ulNoOfCards) <=0){
			BinLen = *(pcSrc+src_cur_pos);
			if (((src_cur_pos + 1) + BinLen + 3 ) > ulInBufSz){break;}
			src_cur_pos++;
			m_ulAcumBin += hex_long((LPBYTE)(pcSrc+src_cur_pos),BinLen);
			src_cur_pos+= BinLen;
			m_ulNoOfCards = hex_long((LPBYTE)(pcSrc+src_cur_pos),3);
			src_cur_pos+= 3;

			(void)long_hex((LPBYTE)DataSave3->strBinTbl.udtBinEntry[DataSave3->strBinTbl.usNoOfBins].aucBin, BIN_LEN,  m_ulAcumBin);
			DataSave3->strBinTbl.udtBinEntry[DataSave3->strBinTbl.usNoOfBins].ulBinIdx = m_cur_dest_pos + dst_cur_pos;

			m_ulAcumCard = 0;
		}

		CardLen = *(pcSrc+src_cur_pos);
		if (((src_cur_pos + 1) + CardLen) > ulInBufSz){break;}
		src_cur_pos++;
		m_ulAcumCard += hex_long((LPBYTE)(pcSrc+src_cur_pos),CardLen);
		src_cur_pos+= CardLen;
		dst_cur_pos += long_hex((LPBYTE)(pcDst+dst_cur_pos),CARD_LEN, m_ulAcumCard);
		(m_ulNoOfCards)--;
	}
	*pulOutBufSz = dst_cur_pos;
	return src_cur_pos;

}
uchar CFilesMngr::ucParseBL()
{
	ulong cur_pos = 0;
	m_cur_dest_pos = 0;
	m_cur_src_pos = 0;
	m_ulAcumCard = 0;
	m_ulAcumBin = 0;
	m_ulNoOfCards = 0;

	CFile fFile;
	string strFileName(BL_NAME,strlen(BL_NAME));
	strFileName.append((LPCTSTR)COMP_TAG,(int)strlen(COMP_TAG));

	if (!fFile.open(strFileName.buffer(),O_RDONLY)){fFile.close(); DBGLine ("No file" );return 0;}
	ulong InBLSize = fFile.size();
	fFile.close();

	ulong ulReadChnkSize = min(InBLSize, MAX_RD_CHNK_SIZE);
	uchar* pucBLRDData = new uchar [ulReadChnkSize];
	uchar* pucBLWRData = new uchar [MAX_WR_CHNK_SIZE];
	ulong ulBLWrChnkSz;

	while (m_cur_src_pos < InBLSize)
	{
		ulReadChnkSize = min((InBLSize-m_cur_src_pos), MAX_RD_CHNK_SIZE);
		CFile::readFromFileLoc(strFileName.buffer(), m_cur_src_pos, pucBLRDData,ulReadChnkSize);
		cur_pos = ucParseBLChnk(pucBLRDData,ulReadChnkSize,pucBLWRData,&ulBLWrChnkSz);
		if (WriteChunkFS(BL_NAME, pucBLWRData, ulBLWrChnkSz,(bool)m_cur_src_pos)== true){
			m_cur_src_pos += cur_pos;
			m_cur_dest_pos+=ulBLWrChnkSz;
		}
	}
	DELETE_POINTER_ARR(pucBLRDData);
	DELETE_POINTER_ARR(pucBLWRData);
}

bool CFilesMngr::ulFindDistrLocInBL(LPCTSTR pcDistrID, ulong* pulDistStartLoc, ulong* pulDistEndLoc){

	ulong ulStart = BL_VER_LEN + BL_SZ_LEN;
	uchar ucDistEntryLen = sizeof(strBLDistEntry);
	bool bIsFound = false;

	CFile CardsFile;
	if(!CardsFile.open(BL_NAME,O_RDONLY))
		return false;

	BYTE aucRdData[BL_DIST_NO_LEN];
	if(!CardsFile.seek(ulStart)) {goto End_Label;}
	if (CardsFile.read(aucRdData, BL_DIST_NO_LEN) != BL_DIST_NO_LEN) {goto End_Label;}
	ulong ulNoOfDistrib = hex_long(aucRdData,BL_DIST_NO_LEN);

	BYTE* RdData = new BYTE[ulNoOfDistrib * ucDistEntryLen];
	ulStart  += BL_DIST_NO_LEN;

	if(!CardsFile.seek(ulStart)) {goto End_Label;}
	if (CardsFile.read(RdData, (ulNoOfDistrib * ucDistEntryLen)) != (ulNoOfDistrib * ucDistEntryLen)) {goto End_Label;}
	strBLDistEntry* pBLDistEntry = (strBLDistEntry*)RdData;

	for (int i=0; i< ulNoOfDistrib; i++ ){
		if (!memcmp((void *)pBLDistEntry->aucDistID,(void *)pcDistrID,BL_DIST_ID_LEN)){
			*pulDistStartLoc = hex_long(pBLDistEntry->aucDistLoc,BL_DIST_LOC_LEN);
			pBLDistEntry++;
			*pulDistEndLoc = hex_long(pBLDistEntry->aucDistLoc,BL_DIST_LOC_LEN);
			bIsFound = true;
			break;
		}
	}
End_Label:
	CardsFile.close();
	return bIsFound;
}

bool CFilesMngr::bsearchInFile( register const void *key,
                                const LPCTSTR sFile,
                                ulong ulbase,
                                size_t nmemb,
                                register size_t size,
                                ulong* pulFoundLoc)
{

	register ulong base = ulbase;
	register size_t lim;
	register int cmp;
	register ulong p;
	register bool bIsFound = false;

	LPBYTE RdData = new BYTE[size];

	CFile CardsFile;
	if(!CardsFile.open(sFile,O_RDONLY))
		return false;

	for (lim = nmemb; lim != 0; lim >>= 1) {

		p = base + (lim >> 1) * size;
		if(!CardsFile.seek(p)) {break;}
		if (CardsFile.read(RdData, size) != size) {break;}

		cmp = memcmp(key, RdData, size);
		if (cmp == 0){
			*pulFoundLoc = p; bIsFound = true; break;}
		if (cmp > 0) {	/* key > p: move right */
			base = p + size;
			lim--;
		}		/* else move left */
	}

	CardsFile.close();
	if (bIsFound){return true;}
	else {return false;}

}
