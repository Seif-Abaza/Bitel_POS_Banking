/******************************************************************************/
/* COMPANY  : Bitel EMEA Co.                                                  */
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
#ifndef  _ISO8583_H_
  #define  _ISO8583_H_

/*****************************************************************************/
/* INCLUDE FILES                                                             */
/*****************************************************************************/

/*****************************************************************************/
/* CONSTANTS, MACROS                                                         */
/*****************************************************************************/
#define TPDU   "\x60\x00\x00\x00\0x00"
#define ERR_HOSTCODE                    0x70
#define ERR_REV_TRIALS                    0xEE

/* ISO8583 Define */
#define ERR_ISO83						0x80
#define ERR_ISO8583_INVALIDVALUE		0x81
#define ERR_ISO8583_INVALIDBIT			0x82
#define ERR_ISO8583_INVALIDPACKLEN		0x83
#define ERR_ISO8583_OVERBITBUFLIMIT		0x84
#define ERR_ISO8583_BITNOTEXIST			0x85
#define ERR_ISO8583_INVALIDPARAM		0x86
#define ERR_ISO8583_NODATAPACK			0x87
#define ERR_ISO8583_OVERBUFLIMIT		0x88
#define ERR_ISO8583_INVALIDBITATTR		0x89
#define ERR_ISO8583_UNPACKDATA			0x8A
#define ERR_ISO8583_SETBITLEN			0x8B
#define ERR_ISO8583_COMPARE				0x8C
#define ERR_ISO8583_MACERROR			0x8D
#define ERR_ISO8583_INVALIDLEN			0x8E

#define ISO8583_MSGIDLEN		4
#define ISO8583_BITMAPLEN		16
#define ISO8583_MAXBITNUM		128
#define ISO8583_MAXBITBUFLEN	10//1024 //((1024 * 4)+200)
#define ISO8583_MAXCOMMBUFLEN	10//1024 //((1024 * 4)+200)
#define ISO8583_BITNOTEXISTFLAG 0x00
#define ISO8583_BITEXISTFLAG	0xff
#define ISO8583_BIT48NUM		9

#define PRN_MAXCHAR                     50


#define ATTR_N			0x01
#define ATTR_NZ			0x0F
#define ATTR_AN			0x02
#define ATTR_NS			0x03
#define ATTR_XN			0x04
#define ATTR_ANS		0x05
#define ATTR_LLVARN		0x06
#define ATTR_LLVARNS	0x07
#define ATTR_LLVARAN	0x08
#define ATTR_LLVARANS	0x09
#define ATTR_LLLVARANS	0x0A
#define ATTR_LLVARZ		0x0B
#define ATTR_LLLVARZ	0x0C
#define ATTR_BIN		0x0D
#define ATTR_LLLVARN	0x0E



/*****************************************************************************/
/* TYPES                                                                     */
/*****************************************************************************/
typedef struct
{
	unsigned char	ucExistFlag;
	unsigned short	uiOffset;
	unsigned short	uiLen;
}ISO8583Bit;

typedef struct
{
	ISO8583Bit		Bit[ISO8583_MAXBITNUM];
	unsigned short	uiBitBufLen;
	unsigned char	aucMsgID[ISO8583_MSGIDLEN+1];
	unsigned char	aucBitBuf[ISO8583_MAXBITBUFLEN];
	unsigned short	uiCommBufLen;
	unsigned char	aucCommBuf[ISO8583_MAXCOMMBUFLEN];
}ISO8583;

typedef struct
{
	ISO8583Bit		Bit[ISO8583_MAXBITNUM];
	unsigned short	uiBitBufLen;
	unsigned char	aucMsgID[ISO8583_MSGIDLEN];
	unsigned char	aucBitBuf[ISO8583_MAXBITBUFLEN];
}BACKUPISO8583;

typedef struct
{
	unsigned char	ucAttr;
	unsigned short	uiMaxLen;
}ISO8583BitAttr;

/*****************************************************************************/
/* EXTERNAILZED VARIABLES                                                    */
/*****************************************************************************/
extern ISO8583	ISO8583Data;
extern int  	LeftTrN;
extern unsigned char aucTPDU[6];
/*****************************************************************************/
/* FUNCTIONS                                                                 */
/*****************************************************************************/
void ISO8583_Clear(void);
void ISO8583_ClearBit(void);
void ISO8583_ClearBitBuf(void);
void ISO8583_ClearCommBuf(void);
unsigned char ISO8583_SetTPDU(unsigned char* pucTPDU);
unsigned char ISO8583_GetMsgID(unsigned short *piMsgID);
unsigned char ISO8583_SetMsgID(unsigned short iMsgID);
unsigned char ISO8583_GetBitValue(short iBitNo, unsigned char *pucOutData, short *piOutDataLen, short iOutBufLen);
unsigned char ISO8583_SetBitValue(short iBitNo,unsigned char *pucInData,unsigned short uiInLen);
unsigned char ISO8583_SetBitHexValue(short iBitNo,unsigned char *pucInData,unsigned short uiInLen);
unsigned char ISO8583_CheckBit(short iBitNo);
unsigned char ISO8583_RemoveBit(short iBitNo);
unsigned char ISO8583_PackData(unsigned char *pucOutData, unsigned short *piOutDataLen, short iOutLen);
unsigned char ISO8583_UnpackData(unsigned char *pucInData, short iInDataLen);
void ISO8583_DumpData(void);
unsigned short ISO8583_ComputeBitAttrTabChecksum(void);
//void ISO8583_CheckBitAttrTab(void);
unsigned char ISO8583_CheckResponseValid(void);
unsigned char ISO8583_CompareSentBit(short iBitNo);
void ISO8583_SaveISO8583Data(unsigned char *pucSrc,unsigned char *pucDest);
void ISO8583_RestoreISO8583Data(unsigned char *pucSrc,unsigned char *pucDest);
void ISO8583_SaveReversalISO8583Data(void);
unsigned short ISO8583_ComputeReversalISO8583DataChk(void);
unsigned char ISO8583_CheckReversalISO8583Data(void);
void ISO8583_WriteReversalISO8583DataChk(unsigned char ucClrMirrorFlag);
	

#endif /*_ISO8583_H_*/

