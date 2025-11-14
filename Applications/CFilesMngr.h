/*
 * CFilesMngr.h
 *
 *  Created on: Jun 20, 2014
 *      Author: Mahmoud Swedan
 */

#ifndef FILESMNGR_H_
#define FILESMNGR_H_

#define MAX_RD_CHNK_SIZE (5*1024)
#define MAX_WR_CHNK_SIZE (MAX_RD_CHNK_SIZE * 3)
#ifndef min
#define min(X, Y) ((X) < (Y) ? (X) : (Y))
#endif

#ifndef max
#define max(X, Y) ((X) > (Y) ? (X) : (Y))
#endif

#define SW_NAME  "Software"
#define BL_NAME  "Black List"
#define WL_NAME  "White List"
#define DR_NAME  "Distribution Rules"
#define POSPARAN_NAME  "POS PARAM"
#define COMP_TAG  " Comp"


#define BL_VER_LEN 6
#define BL_SZ_LEN 4
#define BL_DIST_NO_LEN 4
#define BL_DIST_ID_LEN 8
#define BL_DIST_LOC_LEN 4
typedef struct{
	uchar aucDistID[BL_DIST_ID_LEN];
	uchar aucDistLoc[BL_DIST_LOC_LEN];
}strBLDistEntry;

#define DR_ITEM_HDR_LEN (3+3+4+4+1+1+1+1)
#define DR_RULE_LEN (1+2)
#define CARD_LEN 4
#define BIN_LEN 2

class CFilesMngr
{
public:
	uchar ucParseBL(void);
	static uchar ucCompressMiniLZO(const uchar* in,uint uiInLen,uchar* out,uint* uioutLen);
	static uchar ucGenBatchMsg();
	static uchar ucCompressAllBatch();

	static bool bLoadFileToMem(const char *  sFile);
	static bool ucSaveFileVersion(const char* sFile,char* FileVersion);
	static DWORD WriteChunkFS(const char* sFile, LPBYTE pucInBuf, ulong ulInBufSz, bool bIsRemoveOld  = false, bool bIsCompressed = false);

	bool CFilesMngr::ulFindDistrLocInBL(LPCTSTR pcDistrID, ulong* pulDistStartLoc, ulong* pulDistEndLoc);
	static bool bsearchInFile( register const void *key,
	                    const LPCTSTR sFile,
	                    ulong ulbase,
	                    size_t nmemb,
	                    register size_t size,
	                    ulong* pulFoundLoc );
private:
	ulong ucParseBLChnk(uchar* pucInBuf, ulong ulInBufSz, uchar* pucOutBuf, ulong* pulOutBufSz);
	static uchar ucInitMainBatchHdr(LPBYTE Date, LPBYTE Time);
	static uchar ucAdjustMainBatchHdr(LPBYTE Date, LPBYTE Time);

	ulong m_ulAcumBin;
	ulong m_ulAcumCard;
	ulong m_ulNoOfCards;
	ulong m_cur_src_pos;
	ulong m_cur_dest_pos;
};




#endif /* FILESMNGR_H_ */
