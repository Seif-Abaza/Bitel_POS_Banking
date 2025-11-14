/*
 * UTILS.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Mohamed
 */
#include "AppGlobal.h"
#include "UTILS.h"


unsigned short usWaittime;
extern short WaitKeyPress10MS(unsigned short t10ms);

void UTIL_Form_Montant(unsigned char *Mnt_Fmt,unsigned long Montant,unsigned char Pos_Virgule);
unsigned long asci_long(unsigned char *ucData, unsigned char ucLen);
void long_asci(unsigned char *ucpResult,int ucCount,unsigned long *ulpData);
void long_str(unsigned char *ucpResult,int ucBase,unsigned long *ulpData);
unsigned long bcd_long(unsigned char *ucData, unsigned char ucLen);
unsigned long tab_long_m(unsigned char *ucIp,unsigned char len);
void long_tab(unsigned char *ucDest,unsigned char len,unsigned long *ulVal);
void int_bcd(unsigned char *ucpResult,int ucBase,unsigned int *ulpData);
void int_hex(unsigned char *ucpResult,int ucBase,unsigned int *ulpData);
void short_bcd(unsigned char *ucpResult,int ucBase,unsigned short *uspData);
void hex_asc(unsigned char *aucAscData,unsigned char *aucHexData,unsigned char len);
void int_asc(unsigned char *ucpResult,int ucCount,unsigned int *uipData);
void int_to_tab(unsigned char *ucDest,unsigned char len,unsigned int *uiVal);
unsigned char UTIL_format_date_bcd_str( unsigned char *Ptd, unsigned char *Pts );
unsigned char UTIL_format_time_bcd_str( unsigned char *Ptd, unsigned char *Pts );


//int min(int x,int y)
//{
//	if (x<= y) return x; else return y;
//
//}
//
//int max(int x,int y)
//{
//	if (x>= y) return x; else return y;
//
//}

void UTIL_Form_Montant(unsigned char *Mnt_Fmt,unsigned long Montant,unsigned char Pos_Virgule)
{
    uchar   i;
    uchar   j;
	uchar aucBuf[11];

    long_str(Mnt_Fmt,10,&Montant);

    j = Pos_Virgule;
    for (i=9; j ; i--, j--)
        Mnt_Fmt[i+1] = Mnt_Fmt[i];
    /* end modify */


    if (Pos_Virgule)
    {
        Mnt_Fmt[i+1] ='.';
        Mnt_Fmt[i+1+Pos_Virgule+1] = 0;
    }
    for(j=0 ; ((j < i) && (Mnt_Fmt[j]=='0')) ; Mnt_Fmt[j++]=' ');
#ifdef EGY_FROMRIGHTTOLEFT
	memset(aucBuf,0,sizeof(aucBuf));
	for(i=0;i<10;i++)
	{
		aucBuf[0]=Mnt_Fmt[10-i];

	}
	memcpy(Mnt_Fmt,aucBuf,10);
#endif
}
unsigned long asc_long(unsigned char *ucData, unsigned char ucLen)
{
    char ucTemp[20];
    memcpy(ucTemp,ucData,ucLen);
    ucTemp[ucLen]=0;
    return (unsigned long)atol(ucTemp);

}
unsigned long asci_long(unsigned char *ucData, unsigned char ucLen)
{
	unsigned char ucTemp[20];
	memcpy(ucTemp,ucData,ucLen);
	ucTemp[ucLen]=0;
	return (unsigned long)atol(ucTemp);

}

void long_asci(unsigned char *ucpResult,int ucCount,unsigned long *ulpData)
{
	unsigned char temp[15];
	memset(temp,0,15);
	memset(ucpResult,'0',ucCount);
	sprintf(temp, "%Lu", (long)(*ulpData));
	memcpy(&ucpResult[ucCount-strlen(temp)],temp,strlen(temp));
	//ltoa (*ulpData,ucpResult,ucBase);
}
void asci_hex(unsigned char *ucpResult,int ucCount,unsigned long *ulpData)
{
	unsigned char temp[15];
	memset(temp,0,15);
	memset(ucpResult,'0',ucCount);
	sprintf(temp, "%x", (long)(*ulpData));
	memcpy(&ucpResult[ucCount-strlen(temp)],temp,strlen(temp));
	//ltoa (*ulpData,ucpResult,ucBase);
}
void long2asci(unsigned char *ucpResult,int ucCount,unsigned long ulData)
{
	unsigned char temp[15];
	memset(temp,0,15);
	memset(ucpResult,'0',ucCount);
	sprintf(temp, "%Lu", ulData);
	memcpy(&ucpResult[ucCount-strlen(temp)],temp,strlen(temp));
	//ltoa (*ulpData,ucpResult,ucBase);
}
void int_asc(unsigned char *ucpResult,int ucCount,unsigned int *uipData)
{
	unsigned char temp[10];
	memset(ucpResult,'0',ucCount);
	sprintf(temp, "%d", (int)(*uipData));
	memcpy(ucpResult,temp,ucCount);
	//ltoa (*ulpData,ucpResult,ucBase);
}
void long_str(unsigned char *ucpResult,int ucBase,unsigned long *ulpData)
{
	unsigned char temp[ucBase];
	int i, index=0;
	memset(temp,'0',sizeof(temp));
	sprintf(temp, "%Lu", (long)(*ulpData));
	for ( i=0;i<ucBase;i++)
		{
			if (temp[i]==0)
				{
					index=i;
					break;
				}
		}
	memset(ucpResult,'0',ucBase);
	memcpy(&ucpResult[ucBase-index],temp,index);
	//ltoa (*ulpData,ucpResult,ucBase);
}
unsigned long bcd_long(unsigned char *ucData, unsigned char ucLen)
{
	unsigned char uctemp[ucLen];
	bcd2asc(uctemp,ucData,ucLen);
	return(asci_long(uctemp,ucLen));
}

/*unsigned long bcd_to_long(unsigned char *ucData, unsigned char ucLen)
{
	unsigned char uctemp[ucLen];
	bcd2asc(uctemp,ucData,ucLen);
	return(asci_long(uctemp,ucLen*2));
}*/

void long_bcd(unsigned char *ucpResult,int ucBase,unsigned long *ulpData)
{
	unsigned char temp[ucBase*2];
	long_asci(temp,ucBase*2,ulpData);
	asc2bcd(ucpResult, temp,ucBase*2);
}
void int_bcd(unsigned char *ucpResult,int ucBase,unsigned int *ulpData)
{
	long a;
	a=(long) *ulpData;
	unsigned char temp[ucBase*2];
	long_asci(temp,ucBase*2,&a);
	asc2bcd(ucpResult,temp,ucBase*2);

}
void short_bcd(unsigned char *ucpResult,int ucBase,unsigned short *uspData)
{
	long a;
	a=(long) *uspData;
	unsigned char temp[ucBase*2];
	long_asci(temp,ucBase*2,&a);
	asc2bcd(ucpResult,temp,ucBase*2);
}
unsigned long tab_long_m(unsigned char *ucIp,unsigned char len)
{
	int i,j,l;
	unsigned long ret=0;
	unsigned char tmp[20];
	l=len;
	j=0;
	for(i=0;i<l;i++)
	{
		tmp[j]=ucIp[i];
		ret+=(tmp[j] << ((len-j-1)*8));
		j++;

		/*
		if (ucIp[i]!='.')
		{
			tmp[j]=ucIp[i];
			ret+=(tmp[j] << ((len-j-1)*8));
			j++;

		}
		else
		{
			j++;
		}*/

	}

	return ret;
}
void long_tab(unsigned char *ucDest,unsigned char len,unsigned long *ulVal)
{
	unsigned char tmp[len];
	int i,j;
	j=0xFF;
	for (i=0;i<len;i++)
	{
		tmp[len-i-1]=((0xFF) & (( *ulVal)>>(i*8)));

	}
	memcpy(ucDest,tmp,len);
}
void int_to_tab(unsigned char *ucDest,unsigned char len,unsigned int *uiVal)
{
	unsigned char tmp[len];
	int i,j;
	j=0xFF;
	for (i=0;i<len;i++)
	{
		tmp[len-i-1]=((j<<i*2) & ( *uiVal));

	}
	memcpy(ucDest,tmp,len);
}

void hex_asc(unsigned char *aucAscData,unsigned char *aucHexData,unsigned char len)
{
	int i;
	for (i=0;i<len/2;i++)
	{
		aucAscData[i*2]=hex2chr(((aucHexData[i] & 0xF0)>>4)) ;
		aucAscData[i*2+1]=hex2chr(aucHexData[i] & 0x0F);
	}
}

void asc_hex(unsigned char *aucHexData ,int dlen,unsigned char *aucAscData,int slen)
{
	int i;
	for (i=0;i<dlen;i++)
	{
		aucHexData[i]=chr2hex(aucAscData[i*2]) << 4;
		aucHexData[i]+= chr2hex(aucAscData[i*2+1]);
	}
}

static char asc2hex(char value)
/* [<][>][^][v][top][bottom][index][help] */
{
  value -= 0x30;                  /* normalise to 0..9 range */
  if (value >= 0) {
    if (value > 9) {              /* but may not be 10..15 */
      value &= 0x1f;              /* make A..F & a..f be the same */
      value -= 0x07;              /* normalise to 10..15 range */
      if ((value < 0x0a) || (value > 0x0f)) { /* if outside range then... */
        value = -1;               /* ...signal error */
      }
    }
  } else {                        /* outside 0..9 range... */
    value = -1;                   /* ...signal error */
  }
  return value;                   /* return hex char or error */
}

unsigned char UTIL_format_time_bcd_str( unsigned char *Ptd, unsigned char *Pts )
{
    unsigned char i ;

    for ( i = 0 ; i < 2 ; i++, Pts++)
    {
    *Ptd++ = (*Pts >> 4) | 0x30;
    *Ptd++ = (*Pts & 0x0F) | 0x30;
    if (!i)
        *Ptd++ = ':';
    }
    *Ptd = 0;
    return(SUCCESS);
}

unsigned char UTIL_format_date_bcd_str( unsigned char *Ptd, unsigned char *Pts )
{
    unsigned char i ;

    for ( i = 0 ; i < 4; i++,Pts++)   //old is 3
    {
    *Ptd++ = (*Pts >> 4) | 0x30;
    *Ptd++ = (*Pts & 0x0F) | 0x30;
    if ( ( i == 1) || ( i == 2) ) *Ptd++ = '-';
    }
//  *Ptd = 0;
    return(SUCCESS);
}


void UTIL_ClearScren(void)
{
	Lprint(0,0,"                                          \0");
	Lprint(1,0,"                                          \0");
	Lprint(2,0,"                                          \0");
	Lprint(3,0,"                                          \0");

}


ulong GetFlashLong(unsigned long *pulAddress)
{
	//return tab_long(pulAddress,4);
	unsigned char uctmp[4];
	memcpy(uctmp,pulAddress,4);
	return (uctmp[0])+(uctmp[1]<<8)+(uctmp[2]<<16)+(uctmp[3]<<24);
}
ushort GetFlashshort(unsigned short *pusAddress)
{
	return tab_long(pusAddress,2);

}
void SetFlashLong(unsigned long *pulAddress,int n,unsigned long *ulData)
{
	//unsigned char ultemp[4];
	//long_tab(ultemp,4,ulData);
	//FLASH_WRITE((unsigned char *)pulAddress,4,ultemp);
	unsigned long ultmp;
	unsigned char ultemp[4];
	ultmp=*ulData;
	ultemp[0]=ultmp & 0xFF;
	ultemp[1]=(ultmp & 0xFF00) >> 8;
	ultemp[2]=(ultmp & 0xFF0000) >> 16;
	ultemp[3]=(ultmp & 0xFF000000) >> 24;
	FLASH_WRITE((unsigned char *)pulAddress,4,ultemp);


}
void SetFlashshort(unsigned long *pulAddress,int n,unsigned short *ulData)
{
	unsigned char ultemp[2];
	long_tab(ultemp,2,ulData);
	FLASH_WRITE((unsigned char *)pulAddress,2,ultemp);

}

void del(char str[],char ch)
{
int i,j=0;
int size;
char ch1;
char str1[10];

size=strlen(str);

for(i=0;i<size;i++)
{
  if((str[i] != ch))
  {
    ch1 = str[i];
    str1[j] = ch1;
    j++;
  }
}
str1[j]='\0';
}
