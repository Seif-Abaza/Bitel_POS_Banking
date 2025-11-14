

#include "../../BF_SDK/include/global.h"
#include "AppGlobal.h"
#include "iso8583.h"
#include "UTILS.h"
#include "Display.h"
#include "./resources/Msgs.h"
#include "Utilities.h"

#define USER_DEBUG 1
#define CHKINIT   0x5555       /* Starting value type dissymetric */

unsigned char aucTPDU[6];
ISO8583	ISO8583Data;

const ISO8583BitAttr Default_ISO8583BitAttrTab[ISO8583_MAXBITNUM]
={
	{0,				0	},	//Bit 1
	{ATTR_LLVARZ,   19	},	//Bit 2*
	{ATTR_N,		6	},  //Bit 3*
	{ATTR_N,		12	},  //Bit 4*
	{ATTR_AN,		12	},  //Bit 5
	{ATTR_AN,		3	},  //Bit 6
	{ATTR_N,		10	},  //Bit 7
	{ATTR_N,		8	},  //Bit 8
	{ATTR_N,		8	},  //Bit 9
	{ATTR_AN,		12	},  //Bit 10
	{ATTR_AN,		6	},  //Bit 11*
	{ATTR_AN,		6	},  //Bit 12*
	{ATTR_AN,		4	},  //Bit 13*
	{ATTR_N,		4	},  //Bit 14*
	{ATTR_N,		4	},  //Bit 15
	{ATTR_N,		4	},  //Bit 16
	{ATTR_N,		4	},  //Bit 17
	{ATTR_N,		4	},  //Bit 18
	{ATTR_AN,		12	},  //Bit 19
	{ATTR_AN,		4	},  //Bit 20
	{ATTR_N,		3	},  //Bit 21
	{ATTR_N,		3	},  //Bit 22
	{ATTR_N,		4	},  //Bit 23
	{ATTR_AN,		3	},  //Bit 24*
	{ATTR_N,		2	},  //Bit 25
	{ATTR_N,		2	},  //Bit 26
	{ATTR_N,		1	},  //Bit 27
	{ATTR_XN,		9	},  //Bit 28
	{ATTR_XN,		9	},  //Bit 29
	{ATTR_AN,		1	},  //Bit 30
	{ATTR_N,		8	},  //Bit 31
	{ATTR_AN,		1	},  //Bit 32
	{ATTR_LLVARANS,	11	},  //Bit 33
	{ATTR_AN,		16	},  //Bit 34
	{ATTR_LLVARZ,	37	},  //Bit 35*
	{ATTR_LLLVARZ,	104	},  //Bit 36
	{ATTR_ANS,		12	},  //Bit 37*
	{ATTR_ANS,		6	},  //Bit 38*
	{ATTR_AN,		2	},  //Bit 39*
	{ATTR_AN,		1	},  //Bit 40
	{ATTR_ANS,		8	},  //Bit 41*
	{ATTR_ANS,		15	},  //Bit 42*
	{ATTR_ANS,		40	},  //Bit 43
	{ATTR_LLVARANS,	25	},  //Bit 44*
	{ATTR_LLVARANS,	76	},  //Bit 45
	{ATTR_LLLVARANS,999	},  //Bit 46
	{ATTR_LLLVARANS,999	},  //Bit 47*
	{ATTR_BIN,      34	},  //Bit 48
	{ATTR_AN,		3	},  //Bit 49
	{ATTR_ANS,		3	},  //Bit 50
	{ATTR_ANS,		3	},  //Bit 51
	{ATTR_ANS,		8	},  //Bit 52
	{ATTR_N,		16	},  //Bit 53
	{ATTR_AN,       3	},  //Bit 54*
	{ATTR_LLLVARANS,999	},  //Bit 55*
	{ATTR_LLLVARANS,999	},  //Bit 56*
	{ATTR_LLLVARANS,999	},  //Bit 57*
	{ATTR_LLLVARANS,999	},  //Bit 58*
	{ATTR_LLLVARANS,999 },  //Bit 59*
	{ATTR_LLLVARANS,999	},  //Bit 60*
	{ATTR_LLLVARANS,999	},  //Bit 61
	{ATTR_LLLVARANS,999	},  //Bit 62
	{ATTR_LLLVARANS,999	},  //Bit 63*
	{ATTR_BIN,		8	},  //Bit 64*
	{ATTR_BIN,		1	},  //Bit 65
	{ATTR_N,		1	},  //Bit 66
	{ATTR_N,		2	},  //Bit 67
	{ATTR_N,		3	},  //Bit 68
	{ATTR_N,		3	},  //Bit 69
	{ATTR_N,		3	},  //Bit 70
	{ATTR_N,		4	},  //Bit 71
	{ATTR_N,		4	},  //Bit 72
	{ATTR_N,		6	},  //Bit 73
	{ATTR_N,		10	},  //Bit 74
	{ATTR_N,		10	},  //Bit 75
	{ATTR_N,		10	},  //Bit 76
	{ATTR_N,		10	},  //Bit 77
	{ATTR_N,		10	},  //Bit 78
	{ATTR_N,		10	},  //Bit 79
	{ATTR_N,		10	},  //Bit 80
	{ATTR_N,		10	},  //Bit 81
	{ATTR_N,		12	},  //Bit 82
	{ATTR_N,		12	},  //Bit 83
	{ATTR_N,		12	},  //Bit 84
	{ATTR_N,		12	},  //Bit 85
	{ATTR_N,		16	},  //Bit 86
	{ATTR_N,		16	},  //Bit 87
	{ATTR_N,		16	},  //Bit 88
	{ATTR_N,		16	},  //Bit 89
	{ATTR_N,		42	},  //Bit 90
	{ATTR_AN,		1	},  //Bit 91
	{ATTR_AN,		2	},  //Bit 92
	{ATTR_AN,		5	},  //Bit 93
	{ATTR_AN,		7	},  //Bit 94
	{ATTR_AN,		42	},  //Bit 95
	{ATTR_BIN,		64	},  //Bit 96
	{ATTR_XN,		17	},  //Bit 97
	{ATTR_ANS,		25	},  //Bit 98
	{ATTR_LLVARN,	11	},  //Bit 99
	{ATTR_LLVARN,	11	},  //Bit 100
	{ATTR_LLVARANS,	17	},  //Bit 101
	{ATTR_LLVARANS,	28	},  //Bit 102
	{ATTR_LLVARANS,	28	},  //Bit 103
	{ATTR_LLVARANS,	100	},  //Bit 104
	{ATTR_LLLVARANS,999	},  //Bit 105
	{ATTR_LLLVARANS,999	},  //Bit 106
	{ATTR_LLLVARANS,999	},  //Bit 107
	{ATTR_LLLVARANS,999	},  //Bit 108
	{ATTR_LLLVARANS,999	},  //Bit 109
	{ATTR_LLLVARANS,999	},  //Bit 110
	{ATTR_LLLVARANS,999	},  //Bit 111
	{ATTR_LLLVARANS,999	},  //Bit 112
	{ATTR_LLLVARANS,999	},  //Bit 113
	{ATTR_LLLVARANS,999	},  //Bit 114
	{ATTR_LLLVARANS,999	},  //Bit 115
	{ATTR_LLLVARANS,999	},  //Bit 116
	{ATTR_LLLVARANS,999	},  //Bit 117
	{ATTR_LLLVARANS,999	},  //Bit 118
	{ATTR_LLLVARANS,999	},  //Bit 119
	{ATTR_LLLVARANS,999	},  //Bit 120
	{ATTR_LLLVARANS,999	},  //Bit 121
	{ATTR_LLLVARANS,999	},  //Bit 122
	{ATTR_LLLVARANS,999	},  //Bit 123
	{ATTR_LLLVARANS,999	},  //Bit 124
	{ATTR_LLLVARANS,999	},  //Bit 125
	{ATTR_LLLVARANS,999	},  //Bit 126
	{ATTR_LLLVARANS,999	},  //Bit 127
	{ATTR_BIN,		64	}   //Bit 128
};

void ISO8583_Clear(void)
{
	ushort uiI;
	for(uiI=0;uiI<ISO8583_MAXBITNUM;uiI++)
	{
		ISO8583Data.Bit[uiI].ucExistFlag = ISO8583_BITNOTEXISTFLAG;
		ISO8583Data.Bit[uiI].uiOffset = 0;
		ISO8583Data.Bit[uiI].uiLen = 0;
	}
	ISO8583Data.uiBitBufLen = 0;
	memset(ISO8583Data.aucBitBuf,0,ISO8583_MAXBITBUFLEN);
	ISO8583Data.uiCommBufLen = 0;
	memset(ISO8583Data.aucCommBuf,0,ISO8583_MAXCOMMBUFLEN);

    //DataSave1->ulSysTraceAuditNo++;
	//if (DataSave1->ulSysTraceAuditNo < 0 || DataSave1->ulSysTraceAuditNo > 9999999)
	//	DataSave1->ulSysTraceAuditNo = 1;

}

void ISO8583_ClearBit(void)
{
	ushort uiI;
	for(uiI=0;uiI<ISO8583_MAXBITNUM;uiI++)
	{
		ISO8583Data.Bit[uiI].ucExistFlag = ISO8583_BITNOTEXISTFLAG;
		ISO8583Data.Bit[uiI].uiOffset = 0;
		ISO8583Data.Bit[uiI].uiLen = 0;
	}
}

void ISO8583_ClearBitBuf(void)
{
	ISO8583Data.uiBitBufLen = 0;
	memset(ISO8583Data.aucBitBuf,0,ISO8583_MAXBITBUFLEN);
}

void ISO8583_ClearCommBuf(void)
{
	ISO8583Data.uiCommBufLen = 0;
	memset(ISO8583Data.aucCommBuf,0,ISO8583_MAXCOMMBUFLEN);
}

unsigned char ISO8583_GetMsgID(unsigned short *piMsgID)
{
	short iMsgID;

	iMsgID = (unsigned short)bcd_long(ISO8583Data.aucMsgID,ISO8583_MSGIDLEN*2);
	*piMsgID = iMsgID;
	return(SUCCESS);
}

unsigned char ISO8583_SetMsgID(unsigned short iMsgID)
{
  uchar aucTemp[6];
  uchar aucTemp1[6];
  memset(aucTemp,0,sizeof(aucTemp));
  memset(aucTemp1,0,sizeof(aucTemp1));
	long_tab(aucTemp,ISO8583_MSGIDLEN/2,&iMsgID);
	sprintf(aucTemp1,"%02x%02x",aucTemp[0],aucTemp[1]);
	memcpy(ISO8583Data.aucMsgID, aucTemp1,4);
	//num_to_asc(ISO8583Data.aucMsgID,aucTemp,ISO8583_MSGIDLEN/2);
	return(SUCCESS);
}
unsigned char ISO8583_SetTPDU(unsigned char* pucTPDU)
{
  memset(aucTPDU,0,sizeof(aucTPDU));
  memcpy(aucTPDU,pucTPDU,5);
	return(SUCCESS);
}
unsigned char ISO8583_GetBitValue(short iBitNo, unsigned char *pucOutData, short *puiOutDataLen, short iOutBufLen)
{
	short iI,iJ;


	if(!pucOutData )
	{
		return(ERR_ISO8583_INVALIDPARAM);
	}

	iBitNo --;
	if(  (iBitNo > ISO8583_MAXBITNUM)
	   ||(iBitNo < 0)
	  )
	{
		return(ERR_ISO8583_INVALIDBIT);
	}

	if( ISO8583Data.Bit[iBitNo].ucExistFlag == ISO8583_BITEXISTFLAG)
	{
		iI = ISO8583Data.Bit[iBitNo].uiLen;
		iJ = ISO8583Data.Bit[iBitNo].uiOffset;
/*
#ifdef USER_DEBUG
		DBGPrt_m("iBitNo=%02x\n",iBitNo);
		DBGPrt_m("ISO8583Data.Bit[iBitNo].uiLen=%3d\n",iI);
		DBGPrt_m("iOutBufLen=%3d\n",iOutBufLen);
		DBGPrt_m("-----------------------------------------------------\n");
#endif
*/
		if( iI > iOutBufLen)
		{
			memcpy(pucOutData,&ISO8583Data.aucBitBuf[iJ],iOutBufLen);
			*puiOutDataLen = iOutBufLen;
		}else
		{
			memcpy(pucOutData,&ISO8583Data.aucBitBuf[iJ],iI);
			*puiOutDataLen = iI;
		}
		return(SUCCESS);
	}else
	{
/*
#ifdef USER_DEBUG
		DBGPrt_m("ERR_ISO8583_BITNOTEXIST\n");
#endif
*/
		return(ERR_ISO8583_BITNOTEXIST);
	}
}

unsigned char ISO8583_SetBitValue(short iBitNo,
				unsigned char *pucInData,unsigned short uiInLen)
{
	const ISO8583BitAttr *pBitAttr;

//	pBitAttr = DataSave0->ISO8583BitAttrTab;
	pBitAttr = Default_ISO8583BitAttrTab;
/* Can pack 0 length data in shanghai */

#if 0
	if( uiInLen == 0)
	{
		return(ERR_ISO8583_INVALIDVALUE);
	}
#endif	
	
	iBitNo --;	
	if(  (iBitNo > ISO8583_MAXBITNUM)
	   ||(iBitNo < 0)
	  )
	{
		return(ERR_ISO8583_INVALIDBIT);
	}
/*
#ifdef USER_DEBUG
	DBGPrt_m("iBitNo=%02x\n",iBitNo);
#endif
*/
	/* uiMaxLen+1 is important, for N type data:) */
	if( uiInLen > pBitAttr[iBitNo].uiMaxLen+1)
	{
		return(ERR_ISO8583_INVALIDPACKLEN);
	}
	if( uiInLen + ISO8583Data.uiBitBufLen > ISO8583_MAXBITBUFLEN )
	{
		return(ERR_ISO8583_OVERBITBUFLIMIT);
	}
	ISO8583Data.Bit[iBitNo].ucExistFlag = ISO8583_BITNOTEXISTFLAG;
	ISO8583Data.Bit[iBitNo].uiOffset = ISO8583Data.uiBitBufLen;
	ISO8583Data.Bit[iBitNo].uiLen = uiInLen;
	if( uiInLen )
	{
		memcpy( &(ISO8583Data.aucBitBuf[ISO8583Data.uiBitBufLen]),
				pucInData,uiInLen);
		ISO8583Data.uiBitBufLen += uiInLen;
	}
	ISO8583Data.Bit[iBitNo].ucExistFlag = ISO8583_BITEXISTFLAG;
	return(SUCCESS);
}

unsigned char ISO8583_SetBitHexValue(short iBitNo,
				unsigned char *pucInData,unsigned short uiInLen)
{
	const ISO8583BitAttr *pBitAttr;

//	pBitAttr = DataSave0->ISO8583BitAttrTab;
	pBitAttr = Default_ISO8583BitAttrTab;
	if( uiInLen == 0)
	{
		return(ERR_ISO8583_INVALIDVALUE);
	}
	iBitNo --;	
	if(  (iBitNo > ISO8583_MAXBITNUM)
	   ||(iBitNo < 0)
	  )
	{
		return(ERR_ISO8583_INVALIDBIT);
	}
	if( uiInLen > pBitAttr[iBitNo].uiMaxLen)
	{
		return(ERR_ISO8583_INVALIDPACKLEN);
	}
	if( uiInLen + ISO8583Data.uiBitBufLen > ISO8583_MAXBITBUFLEN )
	{
		return(ERR_ISO8583_OVERBITBUFLIMIT);
	}
	ISO8583Data.Bit[iBitNo].ucExistFlag = ISO8583_BITNOTEXISTFLAG;
	ISO8583Data.Bit[iBitNo].uiOffset = ISO8583Data.uiBitBufLen;
	ISO8583Data.Bit[iBitNo].uiLen = uiInLen*2;
	hex_asc( &(ISO8583Data.aucBitBuf[ISO8583Data.uiBitBufLen]),
			pucInData,uiInLen*2);
	ISO8583Data.uiBitBufLen += uiInLen*2;
	ISO8583Data.Bit[iBitNo].ucExistFlag = ISO8583_BITEXISTFLAG;

	return(SUCCESS);
}

unsigned char ISO8583_CheckBit(short iBitNo)
{
	iBitNo --;	
	if(  (iBitNo > ISO8583_MAXBITNUM)
	   ||(iBitNo < 0)
	  )
	{
		return(ERR_ISO8583_INVALIDBIT);
	}

	if( ISO8583Data.Bit[iBitNo].ucExistFlag == ISO8583_BITEXISTFLAG)
	{
		return(SUCCESS);
	}else
	{
		return(ERR_ISO8583_BITNOTEXIST);
	}
}
unsigned char ISO8583_RemoveBit(short iBitNo)
{
	iBitNo --;
	if(  (iBitNo > ISO8583_MAXBITNUM)
	   ||(iBitNo < 0)
	  )
	{
		return(ERR_ISO8583_INVALIDBIT);
	}
	ISO8583Data.Bit[iBitNo].ucExistFlag = ISO8583_BITNOTEXISTFLAG;
	ISO8583Data.Bit[iBitNo].uiOffset = 0;
	ISO8583Data.Bit[iBitNo].uiLen = 0;
	return(SUCCESS);
}

unsigned char ISO8583_PackData(unsigned char *pucOutData, unsigned short *puiOutDataLen, short iOutLen)
{
	short iI;
	unsigned char aucBitmap[ISO8583_BITMAPLEN];
	unsigned char aucLen[4];
	unsigned char aucTemp[150];
	unsigned char ucMask;
	unsigned char *pucPtr;
	unsigned short uiBitLen;
	unsigned short uiOffset;
	unsigned short uiPackLen;
	const ISO8583BitAttr *pBitAttr;
#ifdef USER_DEBUG
	unsigned short usI;
//	unsigned char  ucYuBit,displayBuf[512];
	DBGPrt_m("\n Pack 8583 data !!!\n");
#endif		

	uchar aucTempBuff[50];

	memset(aucBitmap,0,sizeof(aucBitmap));
	memset(aucLen,0,sizeof(aucLen));
	memset(aucTemp,0,sizeof(aucTemp));
	memset(aucTempBuff,0,sizeof(aucTempBuff));

//	pBitAttr = DataSave0->ISO8583BitAttrTab;
	pBitAttr = Default_ISO8583BitAttrTab;
	//Setup bitmap
	ucMask = 0x80;

	for(iI = 0; iI < ISO8583_MAXBITNUM;iI++)
	{
		if( ISO8583Data.Bit[iI].ucExistFlag == ISO8583_BITEXISTFLAG)
		{
			aucBitmap[iI/8] |= ucMask;
		}
		ucMask = ucMask >> 1;
		if( !ucMask )
		{
			ucMask = 0x80;
		}
	}
#ifdef USER_DEBUG
		DBGPrt_m("aucBitmap:\r\n");
		for(usI=0;usI<16;usI++)
			DBGPrt_m("%02x,",aucBitmap[usI]);
		DBGPrt_m("\r\n");
		DBGPrt_m("MsgId:\r\n");
		DBGPrt_m("%02x%02x\r\n",ISO8583Data.aucMsgID[0],ISO8583Data.aucMsgID[1]);
#endif

	for(iI=0;iI<ISO8583_BITMAPLEN;iI++)
	{
		if( aucBitmap[ISO8583_BITMAPLEN-iI-1] )
		{
			break;
		}
	}

	if( iI == ISO8583_BITMAPLEN)
	{
		return(ERR_ISO8583_NODATAPACK);
	}

	pucPtr = pucOutData;

	if( iOutLen > 2)
	{
	  memcpy(pucPtr,ISO8583Data.aucMsgID,4);
	  //pucPtr += 2;
		
		pucPtr += 4;
	}else
	{
		return(ERR_ISO8583_OVERBUFLIMIT);
	}

	if( iI >= 8)
	{
		if( (pucPtr-pucOutData)+8 > iOutLen)
		{
			return(ERR_ISO8583_OVERBUFLIMIT);
		}else
		{
			//memcpy(pucPtr,aucBitmap,8);
			//pucPtr += 8;
			num_to_asc(pucPtr,aucBitmap,16);
			pucPtr += 16;
		}
	}else
	{
		if( (pucPtr-pucOutData)+ISO8583_BITMAPLEN > iOutLen)
		{
			return(ERR_ISO8583_OVERBUFLIMIT);
		}else
		{
			aucBitmap[0] |= 0x80;
			//memcpy(pucPtr,aucBitmap,ISO8583_BITMAPLEN);
			//pucPtr += ISO8583_BITMAPLEN;
			num_to_asc(pucPtr,aucBitmap,32);
			pucPtr += 32;
		}
	}
	

	for(iI = 1;iI< ISO8583_MAXBITNUM; iI++)
	{
		if( ISO8583Data.Bit[iI].ucExistFlag == ISO8583_BITEXISTFLAG)
		{
			uiBitLen = ISO8583Data.Bit[iI].uiLen;
			uiOffset = ISO8583Data.Bit[iI].uiOffset;
#if 0
#ifdef USER_DEBUG
			ucYuBit = pBitAttr[iI].ucAttr;
			DBGPrt_m("field%d :attribute=%02x bitLen=%d\n",iI+1,ucYuBit,uiBitLen);
			memset(displayBuf,0x00,sizeof(displayBuf));
			if(
				(ISO8583Data.aucBitBuf[uiOffset]>='0')&&(ISO8583Data.aucBitBuf[uiOffset]<='9')
			  &&(ISO8583Data.aucBitBuf[uiOffset+1]>='0')&&(ISO8583Data.aucBitBuf[uiOffset+1]<='9')
			  )
			  {
				memcpy(displayBuf,&ISO8583Data.aucBitBuf[uiOffset],uiBitLen);
			  }else
			  {
				hex_str(displayBuf,&ISO8583Data.aucBitBuf[uiOffset],uiBitLen*2);
			  }
			if(strlen((char *)displayBuf)<300)
				DBGPrt_m("%s\n",displayBuf);
#endif
#endif	
			switch(pBitAttr[iI].ucAttr)
			{
			case ATTR_N:
				uiPackLen = (pBitAttr[iI].uiMaxLen+1)/2;
				if( (pucPtr-pucOutData+uiPackLen) > iOutLen )
				{
					return(ERR_ISO8583_OVERBUFLIMIT);
				}else
				{
#if 1
					asc_hex(pucPtr,uiPackLen,&ISO8583Data.aucBitBuf[uiOffset],uiBitLen);
					pucPtr += uiPackLen;
#else
					if( uiBitLen%2)
					{
						asc_hex(pucPtr,uiPackLen,&ISO8583Data.aucBitBuf[uiOffset],uiBitLen+1);
					}else
					{
						asc_hex(pucPtr,uiPackLen,&ISO8583Data.aucBitBuf[uiOffset],uiBitLen);
					}
					pucPtr += uiPackLen;
					if( uiBitLen%2 )
					{
						*(pucPtr-1) &= 0xF0;
					}
#endif
				}
				break;
			case ATTR_NZ:
				uiPackLen = (pBitAttr[iI].uiMaxLen+1)/2;
				if( (pucPtr-pucOutData+uiPackLen) > iOutLen )
				{
					return(ERR_ISO8583_OVERBUFLIMIT);
				}else
				{
					zasc2bcd(pucPtr,uiPackLen,&ISO8583Data.aucBitBuf[uiOffset],uiBitLen);
					pucPtr += uiPackLen;
				}
				break;

			case ATTR_BIN:
				uiPackLen = ((pBitAttr+iI)->uiMaxLen+1)/8;
				if( (pucPtr-pucOutData+uiPackLen) > iOutLen )
				{
					return(ERR_ISO8583_OVERBUFLIMIT);
				}else
				{
					asc_hex(pucPtr,uiPackLen,&ISO8583Data.aucBitBuf[uiOffset],uiBitLen);
					pucPtr += uiPackLen;
				}
				break;
			case ATTR_AN:
			case ATTR_NS:
			case ATTR_ANS:
			case ATTR_XN:
				uiPackLen = (pBitAttr+iI)->uiMaxLen;
				if( (pucPtr-pucOutData+uiPackLen) > iOutLen )
				{
					return(ERR_ISO8583_OVERBUFLIMIT);
				}else
				{
					memcpy(pucPtr,&ISO8583Data.aucBitBuf[uiOffset],uiPackLen);
					pucPtr += uiPackLen;
				}
				break;
            case ATTR_LLVARN:
              uiPackLen = (uiBitLen+1)/2;
              if( (pucPtr-pucOutData+uiPackLen+1) > iOutLen )
              {
                  return(ERR_ISO8583_OVERBUFLIMIT);
              }else
              {
                  short_bcd(pucPtr,1,&uiBitLen);
                  pucPtr += 1;
                  asc2bcd(pucPtr,&ISO8583Data.aucBitBuf[uiOffset],uiPackLen*2);
                  pucPtr += uiPackLen;
#if 0
                  if( uiBitLen%2 )
                  {
                      *(pucPtr-1) &= 0xF0;
                  }
#endif
              }

			case ATTR_LLVARZ:
				uiPackLen = (uiBitLen+1)/2;
				if( (pucPtr-pucOutData+uiPackLen+1) > iOutLen )
				{
					return(ERR_ISO8583_OVERBUFLIMIT);
				}else
				{
                  asc_to_str(aucTemp, &ISO8583Data.aucBitBuf[uiOffset],uiBitLen);
                  str_to_zL2(pucPtr,aucTemp,uiPackLen*2);
					pucPtr += 1;
					pucPtr += uiPackLen;
				}
				break;
			case ATTR_LLLVARN:
			case ATTR_LLLVARZ:
				uiPackLen = (uiBitLen+1)/2;
/*
#ifdef USER_DEBUG
				DBGPrt_m("uiPackLen1=%d\n",uiPackLen);
#endif
*/
				if( (pucPtr-pucOutData+uiPackLen+2) > iOutLen )
				{
					DBGPrt_m("error iI=%d\n",iI);
					return(ERR_ISO8583_OVERBUFLIMIT);
				}else
				{
					short_bcd(pucPtr,2,&uiBitLen);
					pucPtr += 2;
                    if ( uiPackLen*2 <= 256)
                    {
						asc2bcd(pucPtr,&ISO8583Data.aucBitBuf[uiOffset],uiPackLen*2);
                    }else
                    {
                        asc2bcd(pucPtr,&ISO8583Data.aucBitBuf[uiOffset],254);
                        asc2bcd(&pucPtr[127],&ISO8583Data.aucBitBuf[uiOffset+254],uiPackLen*2-254);
                    }
					pucPtr += uiPackLen;
#if 0
					if( uiBitLen%2 )
					{
						*(pucPtr-1) &= 0xF0;
					}
#endif
				}
				break;
			case ATTR_LLVARAN:
			case ATTR_LLVARNS:
			case ATTR_LLVARANS:
				uiPackLen = uiBitLen;
				if( (pucPtr-pucOutData+uiPackLen+1) > iOutLen )
				{
					return(ERR_ISO8583_OVERBUFLIMIT);
				}else
				{
					//short_bcd(pucPtr,1,&uiBitLen);
				  //int2LL(pucPtr,uiBitLen);
				  uchar Temp[5];
				  memset(Temp,0,sizeof(Temp));
				  sprintf(Temp,"%02d",uiBitLen);
				  memcpy(pucPtr,Temp,2);
					pucPtr += 2;
					if( uiPackLen )
					{
						memcpy(pucPtr,&ISO8583Data.aucBitBuf[uiOffset],uiPackLen);
						pucPtr += uiPackLen;
					}
				}
				break;
			case ATTR_LLLVARANS:
				uiPackLen = uiBitLen;
/*
#ifdef USER_DEBUG
				DBGPrt_m("uiPackLen2=%d\n",uiPackLen);
#endif
*/
				if( (pucPtr-pucOutData+uiPackLen+2) > iOutLen )
				{
					return(ERR_ISO8583_OVERBUFLIMIT);
				}else
				{
				  uchar Temp[5];
				  memset(Temp,0,sizeof(Temp));
				  sprintf(Temp,"%03d",uiBitLen);
				  memcpy(pucPtr,Temp,3);
					//short_bcd(pucPtr,2,&uiBitLen);
				  //int2LLL(pucPtr,uiBitLen);
					pucPtr += 3;
					if( uiPackLen )
					{
						memcpy(pucPtr,&ISO8583Data.aucBitBuf[uiOffset],uiPackLen);
						pucPtr += uiPackLen;
					}
				}
				break;
			default:
//            	UTIL_PrintHexMsg(&iI,2);
//              UTIL_PrintHexMsg(&pBitAttr[iI],1);
				return(ERR_ISO8583_INVALIDBITATTR);
			}
		}
	}

	*puiOutDataLen = (pucPtr - pucOutData);

	return(SUCCESS);
}

unsigned char ISO8583_UnpackData(unsigned char *pucInData, short iInDataLen)
{
	short iI;
	unsigned char *pucPtr;
	unsigned char *pucBitBufPtr;
	unsigned char *pucBitBufStartPtr;
	unsigned char aucBitmap[ISO8583_BITMAPLEN];
	unsigned char ucMask;
	unsigned short uiPackLen;
	unsigned short uiBitLen;
	unsigned short uiBitNo;
	const ISO8583BitAttr *pBitAttr;
//	unsigned char aucBuff[500];

unsigned char aucTempBuf[ISO8583_BITMAPLEN*2];

	pucPtr = pucInData;
	pucBitBufPtr = ISO8583Data.aucBitBuf;
	pucBitBufStartPtr = ISO8583Data.aucBitBuf;

//	pBitAttr = DataSave0->ISO8583BitAttrTab;
	pBitAttr = Default_ISO8583BitAttrTab;
	memcpy(ISO8583Data.aucMsgID,pucPtr,4);
	pucPtr += 4;

	memset(aucBitmap,0,sizeof(aucBitmap));
	memset(aucTempBuf,0,sizeof(aucTempBuf));
	memcpy(aucTempBuf,pucPtr,ISO8583_BITMAPLEN);
	asc_to_num(aucBitmap,aucTempBuf,ISO8583_BITMAPLEN);

	//memcpy(aucBitmap,pucPtr,ISO8583_BITMAPLEN/2);
	pucPtr += ISO8583_BITMAPLEN;
	if( aucBitmap[0] & 0x80)
	{
	  memset(aucTempBuf,0,sizeof(aucTempBuf));
	  memcpy(aucTempBuf,pucPtr,ISO8583_BITMAPLEN);
	  asc_to_num(&aucBitmap[ISO8583_BITMAPLEN/2],aucTempBuf,ISO8583_BITMAPLEN);
		//memcpy(&aucBitmap[ISO8583_BITMAPLEN/2],pucPtr,ISO8583_BITMAPLEN/2);
		pucPtr += ISO8583_BITMAPLEN;
		uiBitNo = ISO8583_MAXBITNUM;
	}else
	{
		uiBitNo = ISO8583_MAXBITNUM/2;
	}

	ucMask = 0x80;

	ISO8583_ClearBit();
	ISO8583_ClearBitBuf();
	
	ucMask = 0x40;
	for(iI=1;iI<uiBitNo;iI++)
	{
		if( aucBitmap[iI/8] & ucMask)
		{
			switch(pBitAttr[iI].ucAttr)
			{
			case ATTR_N:
				uiBitLen = pBitAttr[iI].uiMaxLen;
				uiPackLen = (uiBitLen+1)/2;
				if( uiBitLen%2 )
				{
					bcd2asc( pucBitBufPtr,pucPtr,uiBitLen+1);
				}else
				{
					bcd2asc( pucBitBufPtr,pucPtr,uiBitLen);
				}

				pucPtr += uiPackLen;
				break;
			case ATTR_BIN:
				uiBitLen = (pBitAttr[iI].uiMaxLen)/4;
				uiPackLen = (uiBitLen+1)/2;
				bcd2asc( pucBitBufPtr,pucPtr,uiBitLen);
				pucPtr += uiPackLen;
				break;
			case ATTR_AN:
			case ATTR_NS:
			case ATTR_XN:
			case ATTR_ANS:
				uiBitLen = pBitAttr[iI].uiMaxLen;
				uiPackLen = uiBitLen;
				memcpy( pucBitBufPtr,pucPtr,uiBitLen);
				pucPtr += uiPackLen;
				break;
			case ATTR_LLVARN:
			case ATTR_LLVARZ:
				uiBitLen = (unsigned short)bcd_long(pucPtr,2);
				uiPackLen = (uiBitLen+1)/2;
				pucPtr += 1;
				if( uiBitLen > pBitAttr[iI].uiMaxLen)
				{
					return(ERR_ISO8583_SETBITLEN);
				}else
				{
					if( uiBitLen%2)
					{
						bcd2asc( pucBitBufPtr,pucPtr,uiBitLen+1);
					}else
					{
						bcd2asc( pucBitBufPtr,pucPtr,uiBitLen);
					}
					pucPtr += uiPackLen;
				}
				break;
			case ATTR_LLLVARN:
			case ATTR_LLLVARZ:
				uiBitLen = (unsigned short)bcd_long(pucPtr,4);
				uiPackLen = (uiBitLen+1)/2;
				pucPtr += 2;
				if( uiBitLen > pBitAttr[iI].uiMaxLen)
				{
					return(ERR_ISO8583_SETBITLEN);
				}else
				{
					if( uiBitLen%2)
					{
						bcd2asc( pucBitBufPtr,pucPtr,uiBitLen+1);
					}else
					{
						bcd2asc( pucBitBufPtr,pucPtr,uiBitLen);
					}
					pucPtr += uiPackLen;
				}
				break;
			case ATTR_LLVARAN:
			case ATTR_LLVARNS:
			case ATTR_LLVARANS:
				uiBitLen = (unsigned short)bcd_long(pucPtr,2);
				uiPackLen = uiBitLen;
				pucPtr += 1;
				if( uiBitLen > pBitAttr[iI].uiMaxLen)
				{
					return(ERR_ISO8583_SETBITLEN);
				}else
				{
					memcpy(pucBitBufPtr,pucPtr,uiBitLen);
					pucPtr += uiPackLen;
				}
				break;
			case ATTR_LLLVARANS:
				uiBitLen = (unsigned short)bcd_long(pucPtr,4);
				uiPackLen = uiBitLen;
				pucPtr += 2;
				if( uiBitLen > pBitAttr[iI].uiMaxLen)
				{
					return(ERR_ISO8583_SETBITLEN);
				}else
				{
					memcpy(pucBitBufPtr,pucPtr,uiBitLen);
					pucPtr += uiPackLen;
				}
				break;
			default:
				return(ERR_ISO8583_INVALIDBITATTR);
			}
/*
#ifdef USER_DEBUG
			DBGPrt_m("ISO8583Data.Bit=%02x,%d\n",iI,uiBitLen);
#endif
*/
			ISO8583Data.Bit[iI].uiLen = uiBitLen;
			ISO8583Data.Bit[iI].uiOffset = (unsigned short)(pucBitBufPtr-pucBitBufStartPtr);
			pucBitBufPtr += uiBitLen;
			ISO8583Data.Bit[iI].ucExistFlag = ISO8583_BITEXISTFLAG;
			/*Print the data */
			/**/DBGPrt_m("\nBitNo:%d",iI+1);
			uchar aucBuff[100];
			memset(aucBuff,0x00,sizeof(aucBuff));
			memcpy(aucBuff,pucBitBufPtr-uiBitLen,uiBitLen);
			DBGPrt_m("\r\n %s",aucBuff);
		}
		ucMask = ucMask >> 1;
		if( !ucMask )
		{
			ucMask = 0x80;
		}
		if( (pucPtr - pucInData) > iInDataLen)
		{
			return(ERR_ISO8583_UNPACKDATA);
		}
	}
	return(SUCCESS);
}

void ISO8583_DumpData(void)
{
	unsigned char aucDumpData[PRN_MAXCHAR+1];
	unsigned int uiI,uiJ,uiLen, uiIdx;
	unsigned short uiBitLen,uiOffset;
	DBGPrt_m("enter ISO8583_DumpData\n");
	for(uiI=1;uiI<ISO8583_MAXBITNUM;uiI++)
	{
		if( ISO8583Data.Bit[uiI].ucExistFlag == ISO8583_BITEXISTFLAG)
		{
			uiBitLen = ISO8583Data.Bit[uiI].uiLen;
			uiOffset = ISO8583Data.Bit[uiI].uiOffset;
			memset(aucDumpData,0,sizeof(aucDumpData));
			memcpy(aucDumpData,"BITNO:",6);
			uiJ = uiI+1;
			int_asc(&aucDumpData[6],3,&uiJ);
			//Os__xprint(aucDumpData);
			DBGPrt_m("%s\r\n",aucDumpData);
			if( uiBitLen > 0)
			{
				for(uiJ=0;uiJ<uiBitLen;uiJ+=PRN_MAXCHAR)
				{
					uiLen = min(PRN_MAXCHAR,uiBitLen-uiJ);
					memset(aucDumpData,0,sizeof(aucDumpData));
					memcpy(aucDumpData,&ISO8583Data.aucBitBuf[uiOffset+uiJ],uiLen);
					//Os__xprint(aucDumpData);
                    for (uiIdx=0; uiIdx<uiLen;uiIdx++) {
                      DBGPrt_m("%02x ", aucDumpData[uiIdx]); 
                    }
                    DBGPrt_m("\r\n"); 

				}
			}
		}
	}
}


unsigned short ISO8583_ComputeBitAttrTabChecksum(void)
{
	//wait for demo
	/*
	return(checksum((unsigned char *)&DataSave0->ISO8583BitAttrTab[0],
					(unsigned char *)&DataSave0->ISO8583BitAttrTab_Chk,
				CHKINIT));
				*/
	return 0;

}


#if 1
unsigned char ISO8583_CheckResponseValid(void) { 
  unsigned char ucResult; 
  unsigned char aucBuf[10]; 
  unsigned char aucTempBuf[100];
  unsigned char aucRespBuf[10];
  short iLen; 
  
  /* Check response package is response of sent package */
  /* Notice: bit 11 maybe different !!!*/
  /* Must add more process */
#ifdef USER_DEBUG
  DBGPrt_m("\r\n FUNCTION:ISO8583_CheckResponseValid \r\n "); 
#endif
  /* Check response code */
  memset(aucBuf, 0, sizeof(aucBuf)); 
    ucResult = ISO8583_GetBitValue(39, aucBuf, &iLen, sizeof(aucBuf)); 
#ifdef USER_DEBUG
    DBGPrt_m("\r\n ucResult %02X %02X %02X \r\n ", ucResult, aucBuf[0], aucBuf[1]); 
#endif
    if (ucResult != SUCCESS) {
      return (ucResult);
    }
    udtRunData.ucErrorExtCode = (unsigned short)asc_long(aucBuf, 2); 
	memset(aucTempBuf, 0, sizeof(aucTempBuf));
	memset(aucRespBuf, 0, sizeof(aucRespBuf));
	sprintf(aucRespBuf,"%02d",udtRunData.ucErrorExtCode);
	sprintf(aucTempBuf,"%s:%s %s",GetMsgs(S_RESPO_CODE),pcFrmDgts(DIGIT,aucRespBuf),GetErrMsgs(udtRunData.ucErrorExtCode));
	if( udtRunData.ucErrorExtCode == 0 ){
	  if( DataSave1->udtTransDetail.ulPCode != PCODE_BILL_INQ ){
		ucDispMultiLineMsg(GetErrMsgs(udtRunData.ucErrorExtCode), 0, 0); 
	  }
	} else {
	  ucDispMultiLineMsg(aucTempBuf, 0, 0); 
	}
    //if (RunData.ucErrorExtCode == 13) FLASH_WRITE(DataSave0->NextOffset, sizeof(long), (long *)"\x00\x00\x00\x00"); 
    //if (RunData.ucErrorExtCode == 94) {
    //  
    //  NormalTransData.ulTraceNumber += 50; 
    //  UTIL_IncreaseTraceNumber(1);
    //  
    //}	
#ifdef USER_DEBUG
    DBGPrt_m("HostRespCode=%s\n", aucBuf); 
#endif		
    
    if (memcmp(aucBuf, "00", 2)) {
      
#ifdef USER_DEBUG
      DBGPrt_m("ISO8583_CompareSentBit39notOK\n"); 
#endif
      return (ERR_HOSTCODE);
    }
  return (SUCCESS);
}
#endif

#if 0
unsigned char ISO8583_CompareSentBit(short iBitNo) { 
  unsigned char aucBuf[20]; 
  unsigned short uiI, uiJ; 
  
  iBitNo--;	
  if ((iBitNo > ISO8583_MAXBITNUM)
      || (iBitNo < 0)
     ) {
    return (ERR_ISO8583_INVALIDBIT);
  }
  
  if (ISO8583Data.Bit[iBitNo].ucExistFlag != ISO8583_BITEXISTFLAG) {
/*
#ifdef USER_DEBUG
        DBGPrt_m("ERR_ISO8583_BITNOTEXIST2,iBitNo=%02x\n",iBitNo);
#endif
*/
    return (ERR_ISO8583_BITNOTEXIST);
  }
  if (DataSave0->SendISO8583Data.Bit[iBitNo].ucExistFlag != ISO8583_BITEXISTFLAG) {
/*
#ifdef USER_DEBUG
        DBGPrt_m("ERR_ISO8583_BITNOTEXIST3\n");
#endif
*/
    return (ERR_ISO8583_BITNOTEXIST);
  }
  
  uiI = ISO8583Data.Bit[iBitNo].uiLen; 
  uiJ = DataSave0->SendISO8583Data.Bit[iBitNo].uiLen; 
  if (uiI != uiJ) {
    return (ERR_ISO8583_COMPARE);
  }
  uiJ = ISO8583Data.Bit[iBitNo].uiOffset; 
  if (uiI <= sizeof(aucBuf)) {
    memcpy(aucBuf, &ISO8583Data.aucBitBuf[uiJ], uiI);
  } else {
    return (ERR_ISO8583_OVERBUFLIMIT);
  }
  uiJ = DataSave0->SendISO8583Data.Bit[iBitNo].uiOffset; 
  if (memcmp(aucBuf, &(DataSave0->SendISO8583Data.aucBitBuf[uiJ]), uiI)) {
    return (ERR_ISO8583_COMPARE);
  }
  return (SUCCESS);
}
#endif

void ISO8583_SaveISO8583Data(unsigned char *pucSrc,unsigned char *pucDest)
{
	//Os__saved_copy((unsigned char *)pucSrc,(unsigned char *)pucDest,sizeof(BACKUPISO8583));
	FLASH_WRITE((long)pucDest, sizeof(BACKUPISO8583), pucSrc);

}

void ISO8583_RestoreISO8583Data(unsigned char *pucSrc,unsigned char *pucDest)
{
	memcpy((unsigned char *)pucSrc,(unsigned char *)pucDest,
					sizeof(BACKUPISO8583));

}





