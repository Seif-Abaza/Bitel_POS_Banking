
#include "../extern.h"
#include "tools.h"
#include "types.h"
#include "string.h"
#include "font1251.h"
#include "CParam.h"
#include "../tasks/CTaskDisplay.h"
#include "../iso/common.h"

int		gRtcReadBusy = 0;

const char MagicPrefixRouterDebug[] = "ROUTER_MAGIC_2009";

extern unsigned char		g_codepage[0xFF + 1];
extern char*				g_pProgramName;
extern char*				g_pProgramVersion;
extern char *				g_pMonth[2][12];


#define DISP_DBG_TIME	0

#define COM_DBG

int	Rprintf_m(const char *format, ...)
{
	char debugBuf[4056];
	va_list arguments;
	va_start(arguments, format);
	vsprintf(debugBuf, format, arguments);
	va_end(arguments);

#ifdef MY_DEBUG_ROUTER
	int nTotalSize = strlen(debugBuf);
	WrCom(0, (char*) MagicPrefixRouterDebug, strlen(MagicPrefixRouterDebug));
	WaitTxDone(0);
	WrCom(0, (char*) &nTotalSize, 4);
	WaitTxDone(0);
#endif
	WrCom(COM0, debugBuf, strlen(debugBuf));
	WaitTxDone(COM0);

	return 0;
}

void RprintBuf(const char* bufName, const BYTE* buf, int nSize)
{
	DBGPrt("%s: ", bufName);
	for (int idx = 0; idx < nSize; idx++)
	{
		DBGPrt("%02X ", (buf[idx] & 0xFF));
	}
	DBGPrt("\r\n");
}
void setClock()
{
	DATE_TIME dt;
	GetRTCDateTime(&dt, 0);
	char ctime[3];
	ctime[0]=dt.CLKHOUR;
	ctime[1]=dt.CLKMIN;

	ICON_CLOCK_DISPLAY(ctime);
};

DWORD GetTickCount()
{
	return LONGTIMER;
}

bool IsExpired(DWORD dwStart, DWORD dwTimeout)
{
	DWORD	tick_cur = LONGTIMER;
	if (tick_cur >= dwStart)
		return (tick_cur - dwStart >= dwTimeout);
	return (0xFFFFFFFF - (dwStart - tick_cur) >= dwTimeout);
}

const DWORD crc_32_tab[] = { 
	0x00000000UL, 0x77073096UL, 0xEE0E612CUL, 0x990951BAUL, 
	0x076DC419UL, 0x706AF48FUL, 0xE963A535UL, 0x9E6495A3UL, 
	0x0EDB8832UL, 0x79DCB8A4UL, 0xE0D5E91EUL, 0x97D2D988UL, 
	0x09B64C2BUL, 0x7EB17CBDUL, 0xE7B82D07UL, 0x90BF1D91UL, 
	0x1DB71064UL, 0x6AB020F2UL, 0xF3B97148UL, 0x84BE41DEUL, 
	0x1ADAD47DUL, 0x6DDDE4EBUL, 0xF4D4B551UL, 0x83D385C7UL, 
	0x136C9856UL, 0x646BA8C0UL, 0xFD62F97AUL, 0x8A65C9ECUL, 
	0x14015C4FUL, 0x63066CD9UL, 0xFA0F3D63UL, 0x8D080DF5UL, 
	0x3B6E20C8UL, 0x4C69105EUL, 0xD56041E4UL, 0xA2677172UL, 
	0x3C03E4D1UL, 0x4B04D447UL, 0xD20D85FDUL, 0xA50AB56BUL, 
	0x35B5A8FAUL, 0x42B2986CUL, 0xDBBBC9D6UL, 0xACBCF940UL, 
	0x32D86CE3UL, 0x45DF5C75UL, 0xDCD60DCFUL, 0xABD13D59UL, 
	0x26D930ACUL, 0x51DE003AUL, 0xC8D75180UL, 0xBFD06116UL, 
	0x21B4F4B5UL, 0x56B3C423UL, 0xCFBA9599UL, 0xB8BDA50FUL, 
	0x2802B89EUL, 0x5F058808UL, 0xC60CD9B2UL, 0xB10BE924UL, 
	0x2F6F7C87UL, 0x58684C11UL, 0xC1611DABUL, 0xB6662D3DUL, 
	0x76DC4190UL, 0x01DB7106UL, 0x98D220BCUL, 0xEFD5102AUL, 
	0x71B18589UL, 0x06B6B51FUL, 0x9FBFE4A5UL, 0xE8B8D433UL, 
	0x7807C9A2UL, 0x0F00F934UL, 0x9609A88EUL, 0xE10E9818UL, 
	0x7F6A0DBBUL, 0x086D3D2DUL, 0x91646C97UL, 0xE6635C01UL, 
	0x6B6B51F4UL, 0x1C6C6162UL, 0x856530D8UL, 0xF262004EUL, 
	0x6C0695EDUL, 0x1B01A57BUL, 0x8208F4C1UL, 0xF50FC457UL, 
	0x65B0D9C6UL, 0x12B7E950UL, 0x8BBEB8EAUL, 0xFCB9887CUL, 
	0x62DD1DDFUL, 0x15DA2D49UL, 0x8CD37CF3UL, 0xFBD44C65UL, 
	0x4DB26158UL, 0x3AB551CEUL, 0xA3BC0074UL, 0xD4BB30E2UL, 
	0x4ADFA541UL, 0x3DD895D7UL, 0xA4D1C46DUL, 0xD3D6F4FBUL, 
	0x4369E96AUL, 0x346ED9FCUL, 0xAD678846UL, 0xDA60B8D0UL, 
	0x44042D73UL, 0x33031DE5UL, 0xAA0A4C5FUL, 0xDD0D7CC9UL, 
	0x5005713CUL, 0x270241AAUL, 0xBE0B1010UL, 0xC90C2086UL, 
	0x5768B525UL, 0x206F85B3UL, 0xB966D409UL, 0xCE61E49FUL, 
	0x5EDEF90EUL, 0x29D9C998UL, 0xB0D09822UL, 0xC7D7A8B4UL, 
	0x59B33D17UL, 0x2EB40D81UL, 0xB7BD5C3BUL, 0xC0BA6CADUL, 
	0xEDB88320UL, 0x9ABFB3B6UL, 0x03B6E20CUL, 0x74B1D29AUL, 
	0xEAD54739UL, 0x9DD277AFUL, 0x04DB2615UL, 0x73DC1683UL, 
	0xE3630B12UL, 0x94643B84UL, 0x0D6D6A3EUL, 0x7A6A5AA8UL, 
	0xE40ECF0BUL, 0x9309FF9DUL, 0x0A00AE27UL, 0x7D079EB1UL, 
	0xF00F9344UL, 0x8708A3D2UL, 0x1E01F268UL, 0x6906C2FEUL, 
	0xF762575DUL, 0x806567CBUL, 0x196C3671UL, 0x6E6B06E7UL, 
	0xFED41B76UL, 0x89D32BE0UL, 0x10DA7A5AUL, 0x67DD4ACCUL, 
	0xF9B9DF6FUL, 0x8EBEEFF9UL, 0x17B7BE43UL, 0x60B08ED5UL, 
	0xD6D6A3E8UL, 0xA1D1937EUL, 0x38D8C2C4UL, 0x4FDFF252UL, 
	0xD1BB67F1UL, 0xA6BC5767UL, 0x3FB506DDUL, 0x48B2364BUL, 
	0xD80D2BDAUL, 0xAF0A1B4CUL, 0x36034AF6UL, 0x41047A60UL, 
	0xDF60EFC3UL, 0xA867DF55UL, 0x316E8EEFUL, 0x4669BE79UL, 
	0xCB61B38CUL, 0xBC66831AUL, 0x256FD2A0UL, 0x5268E236UL, 
	0xCC0C7795UL, 0xBB0B4703UL, 0x220216B9UL, 0x5505262FUL, 
	0xC5BA3BBEUL, 0xB2BD0B28UL, 0x2BB45A92UL, 0x5CB36A04UL, 
	0xC2D7FFA7UL, 0xB5D0CF31UL, 0x2CD99E8BUL, 0x5BDEAE1DUL, 
	0x9B64C2B0UL, 0xEC63F226UL, 0x756AA39CUL, 0x026D930AUL, 
	0x9C0906A9UL, 0xEB0E363FUL, 0x72076785UL, 0x05005713UL, 
	0x95BF4A82UL, 0xE2B87A14UL, 0x7BB12BAEUL, 0x0CB61B38UL, 
	0x92D28E9BUL, 0xE5D5BE0DUL, 0x7CDCEFB7UL, 0x0BDBDF21UL, 
	0x86D3D2D4UL, 0xF1D4E242UL, 0x68DDB3F8UL, 0x1FDA836EUL, 
	0x81BE16CDUL, 0xF6B9265BUL, 0x6FB077E1UL, 0x18B74777UL, 
	0x88085AE6UL, 0xFF0F6A70UL, 0x66063BCAUL, 0x11010B5CUL, 
	0x8F659EFFUL, 0xF862AE69UL, 0x616BFFD3UL, 0x166CCF45UL, 
	0xA00AE278UL, 0xD70DD2EEUL, 0x4E048354UL, 0x3903B3C2UL, 
	0xA7672661UL, 0xD06016F7UL, 0x4969474DUL, 0x3E6E77DBUL, 
	0xAED16A4AUL, 0xD9D65ADCUL, 0x40DF0B66UL, 0x37D83BF0UL, 
	0xA9BCAE53UL, 0xDEBB9EC5UL, 0x47B2CF7FUL, 0x30B5FFE9UL, 
	0xBDBDF21CUL, 0xCABAC28AUL, 0x53B39330UL, 0x24B4A3A6UL, 
	0xBAD03605UL, 0xCDD70693UL, 0x54DE5729UL, 0x23D967BFUL, 
	0xB3667A2EUL, 0xC4614AB8UL, 0x5D681B02UL, 0x2A6F2B94UL, 
	0xB40BBE37UL, 0xC30C8EA1UL, 0x5A05DF1BUL, 0x2D02EF8DUL 
};

/* CRC16 Definitions */
const unsigned short crc_table[256] = {
	0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
	0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
	0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
	0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
	0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
	0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
	0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
	0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
	0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
	0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
	0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
	0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
	0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
	0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
	0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
	0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
	0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
	0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
	0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
	0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
	0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
	0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
	0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
	0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
	0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
	0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
	0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
	0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
	0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
	0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
	0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
	0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

const BYTE hexConst[] = "0123456789ABCDEF";

const unsigned long crc_32_tab_tms[] = { /* CRC polynomial 0xedb88320 */
	0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L, 0x706af48fL, 0xe963a535L, 0x9e6495a3L,
	0x0edb8832L, 0x79dcb8a4L, 0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L, 0x90bf1d91L,
	0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL, 0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L,
	0x136c9856L, 0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L, 0xfa0f3d63L, 0x8d080df5L,
	0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L, 0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
	0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L, 0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L,
	0x26d930acL, 0x51de003aL, 0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L, 0xb8bda50fL,
	0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L, 0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL,
	0x76dc4190L, 0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL, 0x9fbfe4a5L, 0xe8b8d433L,
	0x7807c9a2L, 0x0f00f934L, 0x9609a88eL, 0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
	0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL, 0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L,
	0x65b0d9c6L, 0x12b7e950L, 0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L, 0xfbd44c65L,
	0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L, 0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL,
	0x4369e96aL, 0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L, 0xaa0a4c5fL, 0xdd0d7cc9L,
	0x5005713cL, 0x270241aaL, 0xbe0b1010L, 0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
	0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L, 0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL,
	0xedb88320L, 0x9abfb3b6L, 0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L, 0x73dc1683L,
	0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L, 0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L,
	0xf00f9344L, 0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL, 0x196c3671L, 0x6e6b06e7L,
	0xfed41b76L, 0x89d32be0L, 0x10da7a5aL, 0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
	0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L, 0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL,
	0xd80d2bdaL, 0xaf0a1b4cL, 0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL, 0x4669be79L,
	0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L, 0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL,
	0xc5ba3bbeL, 0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L, 0x2cd99e8bL, 0x5bdeae1dL,
	0x9b64c2b0L, 0xec63f226L, 0x756aa39cL, 0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
	0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL, 0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L,
	0x86d3d2d4L, 0xf1d4e242L, 0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L, 0x18b74777L,
	0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL, 0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L,
	0xa00ae278L, 0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L, 0x4969474dL, 0x3e6e77dbL,
	0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L, 0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
	0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L, 0xcdd70693L, 0x54de5729L, 0x23d967bfL,
	0xb3667a2eL, 0xc4614ab8L, 0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL, 0x2d02ef8dL
};

#define UPDC32(octet, crc) (crc_32_tab[((crc) ^ (octet)) & 0xff] ^ ((crc) >> 8))

////////////////////////////////////////////////////////////////////////////
unsigned long crc32buffer( unsigned char *buffer, int len )
{
	register unsigned long oldcrc32;
	//register unsigned long crc32;
	register unsigned long oldcrc;
	unsigned char c;
	register int charcnt;

	oldcrc32 = 0xFFFFFFFFL; charcnt = 0;
	while( len )
	{
		c = buffer[charcnt];
		++charcnt;
		--len;
		oldcrc32 = UPDC32(c, oldcrc32);
	}
	//crc32 = oldcrc32;
	oldcrc = oldcrc32 = ~oldcrc32;
	return oldcrc;
}



/* CRC calculation macros */
#define CRC(crcval,newchar) crcval = (crcval >> 8) ^ \
	crc_table[(crcval ^ newchar) & 0x00ff]

void bin2hexstr(unsigned char* src, char* dest, int len)
{
	int i;

	for(i=0;i<len;i++)
	{
		dest[i*2]=hexConst[((src[i]&0xf0)>>4)];
		dest[i*2+1]=hexConst[(src[i]&0x0F)];
	}
	dest[i*2]=0;
}



void bin2hexblk(BYTE* src, BYTE* dest, int len)
{
	int i;

	for(i=0;i<len;i++)
	{
		dest[i*2]=hexConst[((src[i]&0xf0)>>4)];
		dest[i*2+1]=hexConst[(src[i]&0x0F)];
	}
}




void hexblk2bin(BYTE* src, BYTE* dest, int len)
{
	int i;

	for(i=0;i<len;i++)
	{
		dest[i]=hexChar2num(src[i*2])<<4;
		dest[i]|=hexChar2num(src[i*2+1]);
	}
}

BYTE hexChar2num(char ch)
{
	ch=rtoupper(ch);
	if (ch>='0' && ch<='9')
		return(ch-'0');
	if (ch>='A' && ch<='F')
		return(ch-'A'+10);
	return(0);
}

int hex2int(unsigned char hex)
{
	int res = 0;
	res = ((hex >> 4) & 0x0F) * 10 + (hex & 0x0F);
	return res;
}

unsigned long hex_long(unsigned char *ucIp,unsigned char len)
{
	int i;
	unsigned long ret=0;
	for(i=0;i<len;i++)
	{
		ret+=(ucIp[i] << ((len-i-1)*8));
	}
	return ret;
}
BYTE long_hex(unsigned char *ucDest,unsigned char len,unsigned long ulVal)
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

extern "C"
{
  BYTE long_hex(char *ucDest,unsigned char len,unsigned long ulVal){
	return long_hex((LPBYTE)ucDest,len,ulVal);
  }
  unsigned long hex_long(char *ucIp,unsigned char len)
  {
	DBGLine("hex_long");
	return hex_long((LPBYTE)ucIp,len);
  }
}
void str2BinRight(const BYTE* src, BYTE* dest, int len)
{
	memset(dest, 0, getLengthBCD(len));
	int nRight = 0;
	if (len % 2 == 1)
		nRight = 1;
	for (int idxSrc = 0; idxSrc < len; idxSrc++)
	{
		int idxDest = (idxSrc + nRight) / 2;
		int nShift = (1 - ((idxSrc + nRight) % 2)) * 4;
		dest[idxDest] |= (hexChar2num(src[idxSrc]) & 0x0F) << nShift;
	}
}

const int getLengthBCD(const int nLenSrc)
{
	if (nLenSrc % 2 == 0)
		return nLenSrc / 2;
	else
		return nLenSrc / 2 + 1;
}

void str2BCDLeft(const BYTE* src, BYTE* dest, int len)
{
	memset(dest, 0, getLengthBCD(len));
	for (int idxSrc = 0; idxSrc < len; idxSrc++)
	{
		int idxDest = idxSrc / 2;
		int nShift = (1 - (idxSrc % 2)) * 4;
		dest[idxDest] |= ((src[idxSrc] - '0') & 0x0F) << nShift;
	}
}

void str2BCDRight(const BYTE* src, BYTE* dest, int len)
{
	memset(dest, 0, getLengthBCD(len));
	int nRight = 0;
	if (len % 2 == 1)
		nRight = 1;
	for (int idxSrc = 0; idxSrc < len; idxSrc++)
	{
		int idxDest = (idxSrc + nRight) / 2;
		int nShift = (1 - ((idxSrc + nRight) % 2)) * 4;
		dest[idxDest] |= ( hexChar2num(src[idxSrc]) & 0x0F) << nShift;
	}
}

// ѕередаетс€ количество байт от destination, длина src может быть вычислена getLengthBCD
void BCDLeft2str(const BYTE* src, BYTE* dest, int len)
{
	memset(dest, 0, len);
	for (int idxDest = 0; idxDest < len; idxDest++)
	{
		int idxSrc = idxDest / 2;
		int nShift = (1 - (idxDest % 2)) * 4;
		dest[idxDest] = ((src[idxSrc] >> nShift) & 0x0F) + '0';
	}
}

void BCDRight2str(const BYTE* src, BYTE* dest, int len)
{
	memset(dest, 0, len);
	int nLeft = 0;
	if (len % 2 == 1)
		nLeft = 1;
	for (int idxDest = 0; idxDest < len; idxDest++)
	{
		int idxSrc = (idxDest + nLeft) / 2;
		int nShift = (1 - ((idxDest + nLeft) % 2)) * 4;
		dest[idxDest] = hexConst[(src[idxSrc] >> nShift) & 0x0F];
	}
}
ulong bcd_to_long(char* SrcBcd, ulong ulLen)
{
	char cTempBuff[10];
	memset((void*)cTempBuff,0,sizeof(cTempBuff));
	bcd2asc(cTempBuff,SrcBcd,ulLen);
	return atol(cTempBuff);
}
void long_to_bcd(char* DstBcd,ulong ulSrc, ulong ulLen)
{
	char cTempBuff[10];

	memset((void*)cTempBuff,0,sizeof(cTempBuff));
	sprintf(cTempBuff,"%d",ulSrc);

	memset((void*)DstBcd,0,ulLen);
	str_to_num(DstBcd,cTempBuff,ulLen*2);
}
void vidIncrmntBcd(char* DstBcd, char* SrcBcd, ulong ulLen,ulong ulIncNo){

	memset((void*)DstBcd,0,ulLen);
	long_to_bcd(DstBcd, (bcd_to_long(SrcBcd,ulLen*2) + ulIncNo),ulLen);
}

void newstrncpy(char * dest,char * src,int len)
{
	dest[0]=0;
	if (src!=NULL)
	{
		memcpy(dest,src,len);
		dest[len]=0;
	}
	return;
}

char* allocstrcpy(const char* src)
{
	char* dest = new char[strlen(src) + 1];
	memcpy(dest, src, strlen(src));
	dest[strlen(src)] = '\0';
	return dest;
}

void strncpy(char *dest, char *src, int len)
{
	if (src != NULL)
	{
		memcpy(dest, src, len);
		dest[len] = 0;
	}
}

void purgeChar(char* str, char ch)
{
	int i,j;

	if (str==NULL || strlen(str)==0)
		return;
	for(i=0,j=0; i<strlen(str); i++)
	{
		if (str[i]==ch)
			continue;
		str[j]=str[i];
		j++;
	}
	str[j]=0;
	return;
}

/*
 * Find the first occurrence of the byte string s in byte string l.
 */

void *
memmem(const void *l, size_t l_len, const void *s, size_t s_len)
{
	register char *cur, *last;
	const char *cl = (const char *)l;
	const char *cs = (const char *)s;

	/* we need something to compare */
	if (l_len == 0 || s_len == 0)
		return NULL;

	/* "s" must be smaller or equal to "l" */
	if (l_len < s_len)
		return NULL;

	/* special case where s_len == 1 */
	if (s_len == 1)
		return memchr(l, (int)*cs, l_len);

	/* the last position where its possible to find "s" in "l" */
	last = (char *)cl + l_len - s_len;

	for (cur = (char *)cl; cur <= last; cur++)
		if (cur[0] == cs[0] && memcmp(cur, cs, s_len) == 0)
			return cur;

	return NULL;
}



WORD calcCRC_SendFile(const LPBYTE message, ULONG length, WORD crc /*= CRC_INIT*/)
{
	unsigned long i;

	for(i = 0; i < length; i++)
	{
		CRC(crc, message[i]);
	}
	return crc;
}
//-----------------------------------------------------------------------------/
BYTE calcLRC( BYTE *pBuf, int nLength )
{
	BYTE bcc = 0;
	for (int ii = 0; ii < nLength; ii++) 
		bcc = pBuf[ii] ^ bcc;
	return bcc;
}
ulong crc32(ulong crc1, const void *buf, size_t size)
{
	const BYTE *p;

	p = (LPBYTE)buf;
	ulong crc = crc1 ^ ~0U;

	while (size--)
		crc = crc_32_tab[(crc ^ *p++) & 0xFF] ^ (crc >> 8);

	return crc ^ ~0U;
}

unsigned long updcrc( unsigned char * s, unsigned n )
{
	register unsigned long c; /* temporary variable */

	static unsigned long crc = ( unsigned long )0xffffffffL; /* shift register contents */

	if ( s == NULL ) {
		c = 0xffffffffL; /*crc */
	} else {
		c = crc;
		if ( n ) do {
			c = crc_32_tab[(( int )c ^ (* s ++)) & 0xff] ^ ( c >> 8 ); /* crc */
		} while (-- n );
	}
	crc = c;
	return c ^ 0xffffffffL; /* (instead of ~c for 64-bit machines) */
}
//-----------------------------------------------------------------------------/
DWORD calcCRC32(const BYTE *buffer, DWORD len, DWORD oldcrc32)
{
	DWORD  dwCRC;

	dwCRC=~oldcrc32;
	while (len--) 
	{
		dwCRC = ((DWORD)(dwCRC >> 8)) ^ crc_32_tab[(BYTE)((BYTE)(dwCRC & 0x0FFUL) ^ (*buffer))]; 
		buffer++;
	}
	return(dwCRC ^ 0xffffffffUL); 
}
//-----------------------------------------------------------------------------/
WORD calcCRC(const BYTE *stream, int size)
{
	int i = 0;
	WORD crc = 0;
	for (i=0; i < size; i++) {crc += stream[i];}
	return crc;
}
//-----------------------------------------------------------------------------/
WORD deltaTime(WORD wRef, WORD wCurrent)
{
	long nCur = wCurrent;
	if (wRef < wCurrent) {nCur -= 0x00010000UL;}
	long res = wRef - nCur;

	return LOWORD((DWORD)res);	
}
//-----------------------------------------------------------------------------/
void _dbg_tryAssert( bool bAssert, LPCTSTR sFile, int line )
{
	if (!bAssert) {return;}

#ifdef LOG_ENABLED
	{
		string strOut;
		LPCTSTR s = Q(sFile, sFile, "unk");
		strOut.format("ASSERT: %s, %d", s, line);
		LOG(strOut);
	}
#endif

#ifdef DEBUG
	{
		LPCTSTR s = Q(sFile, sFile, "unk");
		ClearLCDScreen();
		string file(sFile);
		DBG(0,0, "ASSERT:");
		DBG(1,0, file.right(16));
		DBG(2,0, "%d ", line);
		while(1);
	}
#endif
}
//-----------------------------------------------------------------------------/
void _dbg_dumpHexToCom(LPBYTE pData, DWORD szData)
{
	if (!pData || !szData) {return;}

	string str;
	str.makeHexDump(pData, szData);
	WrCom(0, str.buffer(), str.length());
	WaitTxDone(0);

	char del[] = "\n\r";
	WrCom(0, del, sizeof(del));
	WaitTxDone(0);
}
//-----------------------------------------------------------------------------/
void _dbg_dumpStringToCom(LPCTSTR s)
{
	if (!s) {return;}

	WrCom(0, (char*)s, strlen(s));
	WaitTxDone(0);

	char del[] = "\n\r";
	WrCom(0, del, sizeof(del));
	WaitTxDone(0);
}
//-----------------------------------------------------------------------------/
int convertFromHex( unsigned char hex )
{
	int i = ((hex >> 4) & 0x0F) * 10;
	i += hex & 0x0F;
	return i;
}
// Returns number of days since civil 1970-01-01.  Negative values indicate
//    days prior to 1970-01-01.
// Preconditions:  y-m-d represents a date in the civil (Gregorian) calendar
//                 m is in [1, 12]
//                 d is in [1, last_day_of_month(y, m)]
//                 y is "approximately" in
//                   [numeric_limits<Int>::min()/366, numeric_limits<Int>::max()/366]
//                 Exact range of validity is:
//                 [civil_from_days(numeric_limits<Int>::min()),
//                  civil_from_days(numeric_limits<Int>::max()-719468)]
long days_from_civil(char* strYYYYMMDD)
{
		unsigned long y = atol(strYYYYMMDD)/10000;
		 unsigned long m = (atol(strYYYYMMDD)%10000) /100;
		unsigned long d = atol(strYYYYMMDD)%100;

		Rprintf("y=%d m=%d d=%d",y,m,d);
    y -= m <= 2;
     long era = (y >= 0 ? y : y-399) / 400;
    unsigned long yoe = (unsigned long)(y - era * 400);      // [0, 399]
    unsigned long doy = (153*(m + (m > 2 ? -3 : 9)) + 2)/5 + d-1;  // [0, 365]
    unsigned long doe = yoe * 365 + yoe/4 - yoe/100 + doy;         // [0, 146096]
		Rprintf("era=%d doe=%d",era,doe);
    return era * 146097 + (long)(doe) - 719468;
}


// Returns year/month/day triple in civil calendar
// Preconditions:  z is number of days since 1970-01-01 and is in the range:
//                   [numeric_limits<Int>::min(), numeric_limits<Int>::max()-719468].
void civil_from_days(long z, char* strYYYYMMDD)
{

    z += 719468;
     long era = (z >= 0 ? z : z - 146096) / 146097;
     unsigned long doe = (unsigned long)(z - era * 146097);          // [0, 146096]
     unsigned long yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;  // [0, 399]
     long y = (long)(yoe) + era * 400;
     unsigned long doy = doe - (365*yoe + yoe/4 - yoe/100);                // [0, 365]
     unsigned long mp = (5*doy + 2)/153;                                   // [0, 11]
     unsigned long d = doy - (153*mp+2)/5 + 1;                             // [1, 31]
     unsigned long m = mp + (mp < 10 ? 3 : -9);
		 sprintf(strYYYYMMDD,"%04d%02d%02d",y + (m <= 2), m, d);                            // [1, 12]
}

int getDays( char *strYYYYMMDD )
{
	int nYear;
	int nMonth;
	int nDay;
	char	strYear[5];
	char	strMonth[3];
	char	strDay[3];
	memcpy(strYear, strYYYYMMDD, 4);
	strYear[4] = 0;
	memcpy(strMonth, strYYYYMMDD + 4, 2);
	strMonth[2] = 0;
	memcpy(strDay, strYYYYMMDD + 6, 2);
	strDay[2] = 0;
	nYear = atoi(strYear);
	nMonth = atoi(strMonth);
	nDay = atoi(strDay);

	int nResult = 0;
	for (int i = 2007; i < nYear; i++)
	{
		nResult += !(i % 4)? !(i % 100)? !(i % 400)? 366: 365: 366: 365;
	}

	int nFebDay =  !(nYear % 4)? !(nYear % 100)? !(nYear % 400)? 29: 28: 29: 28;

	for (int i = 1; i < nMonth; i++)
	{
		switch (i)
		{
		case 12: nResult += 31;
		case 11: nResult += 30;
		case 10: nResult += 31;
		case 9: nResult += 30;
		case 8: nResult += 31;
		case 7: nResult += 31;
		case 6: nResult += 30;
		case 5: nResult += 31;
		case 4: nResult += 30;
		case 3: nResult += 31;
		case 2: nResult += nFebDay;
		case 1: nResult += 31;
		}
	}
	nResult += nDay;
}

int getDaysInMonth( char *strYYYYMMDD )
{
	int nYear;
	int nMonth;
	char	strYear[5];
	char	strMonth[3];

	memcpy(strYear, strYYYYMMDD, 4);
	strYear[4] = 0;
	memcpy(strMonth, strYYYYMMDD + 4, 2);
	strMonth[2] = 0;
	nYear = atoi(strYear);
	nMonth = atoi(strMonth);

	int nResult = 0;

	int nFebDay =  !(nYear % 4)? !(nYear % 100)? !(nYear % 400)? 29: 28: 29: 28;

	switch (nMonth)
	{
	case 12: nResult = 31; break;
	case 11: nResult = 30;    break;
	case 10: nResult = 31;    break;
	case 9: nResult = 30;     break;
	case 8: nResult = 31;     break;
	case 7: nResult = 31;     break;
	case 6: nResult = 30;     break;
	case 5: nResult = 31;     break;
	case 4: nResult = 30;     break;
	case 3: nResult = 31;     break;
	case 2: nResult = nFebDay;break;
	case 1: nResult = 31;     break;
	}
	DBGLine("nYear = %d, nMonth= %d Days=%d",nYear, nMonth, nResult);
	return nResult;
}
void getDDMMYY(char* outDDMMYY)
{
	DATE_TIME dt;
	GetRTCDateTime(&dt, 0);
	memset(outDDMMYY, 0, 9);
	sprintf(outDDMMYY, "%02X-%02X-%02X", dt.CLKDATE, dt.CLKMONTH, dt.CLKYEARL);
}

void getHHMM(char* outHHMM)
{
	DATE_TIME dt;
	GetRTCDateTime(&dt, 0);
	memset(outHHMM, 0, 6);
	sprintf(outHHMM, "%02X:%02X", dt.CLKHOUR, dt.CLKMIN);
}

void getDate( char *strYYYYMMDD , bool bIsBCD)
{
	DATE_TIME dt;
	GetRTCDateTime(&dt, 0);
	if (bIsBCD){
		strYYYYMMDD[0] = dt.CLKYEARH;
	    strYYYYMMDD[1] = dt.CLKYEARL;
    	strYYYYMMDD[2] = dt.CLKMONTH;
	    strYYYYMMDD[3] = dt.CLKDATE;
	} else {
		sprintf(strYYYYMMDD, "%02X%02X%02X%02X", dt.CLKYEARH, dt.CLKYEARL, dt.CLKMONTH, dt.CLKDATE);
	}
}

void getDDMMYYHHMM(char* outDDMMYYHHMM)
{
	DATE_TIME dt;
	GetRTCDateTime(&dt, 0);
	memset(outDDMMYYHHMM, 0, DISPLAY_WIDTH);
	sprintf(outDDMMYYHHMM, " %02X-%02X-%02X %02X:%02X ", dt.CLKDATE, dt.CLKMONTH, dt.CLKYEARL, dt.CLKHOUR, dt.CLKMIN);
}

void getTrmsnDateTime(char* outYYMMDDhhmm)
{
	DATE_TIME dt;
	GetRTCDateTime(&dt, 0);
	memset(outYYMMDDhhmm, 0, LENGTH_TRMSN_DATE_TIME + 1);
	sprintf(outYYMMDDhhmm, "%02X%02X%02X%02X%02X", dt.CLKYEARL, dt.CLKMONTH, dt.CLKDATE, dt.CLKHOUR, dt.CLKMIN);
}

void getCaputeDate(char* outMMDD)
{
	DATE_TIME dt;
	GetRTCDateTime(&dt, 0);
	memset(outMMDD, 0, LENGTH_CAPTURE_DATE + 1);
	sprintf(outMMDD, "%02X%02X", dt.CLKMONTH, dt.CLKDATE);
}
void getDateTime(char* outDate, char* outTime,bool bIsBCD /* false */){
	DATE_TIME dt;
	GetRTCDateTime(&dt, 0);
	if (bIsBCD){
		memset(outTime, 0, 4);
		memset(outDate, 0, 5);
		outTime[0] = dt.CLKHOUR;
		outTime[1] = dt.CLKMIN;
		outTime[2] = dt.CLKSEC;
		outDate[0] = dt.CLKYEARH;
		outDate[1] = dt.CLKYEARL;
		outDate[2] = dt.CLKMONTH;
		outDate[3] = dt.CLKDATE;
	} else {
		memset(outTime, 0, 5);
		memset(outDate, 0, 9);

		sprintf(outTime, "%02X%02X%02X", dt.CLKHOUR, dt.CLKMIN,dt.CLKSEC);
		sprintf(outDate, "%02X%02X%02X%02X", dt.CLKYEARH, dt.CLKYEARL, dt.CLKMONTH, dt.CLKDATE);

	}
}
void getTime(char* outTime,bool bIsBCD)
{
	DATE_TIME dt;
	GetRTCDateTime(&dt, 0);
	if (bIsBCD){
		memset(outTime, 0, 3);
		outTime[0] = dt.CLKHOUR;
		outTime[1] = dt.CLKMIN;
	} else {
		memset(outTime, 0, 5);
		sprintf(outTime, "%02X%02X", dt.CLKHOUR, dt.CLKMIN);
	}
}

void getTrnLocalDateTime(char* outDateTime)
{
	DATE_TIME dt;
	GetRTCDateTime(&dt, 0);
	memset(outDateTime, 0, LENGTH_TRN_LOCAL_DATE_TIME + 1);
	sprintf(outDateTime, "%02X%02X%02X%02X%02X%02X", dt.CLKYEARL, dt.CLKMONTH, dt.CLKDATE, dt.CLKHOUR, dt.CLKMIN, dt.CLKSEC);
}

void getZZTime(char* outZZZZZHHmmss)
{
	DATE_TIME dt;
	GetRTCDateTime(&dt, 0);
	int nZZZZZ = 0;
	sprintf(outZZZZZHHmmss, "%05d%02X%02X%02X", nZZZZZ, dt.CLKHOUR, dt.CLKMIN, dt.CLKSEC);
}

string getMounth(int nMonth,DWORD nlang)
{	
	

	
//!!!!!!!!!!!!!!!
	return string(g_pMonth[0][nMonth]);
}

void addSecondsToCurrentTime(const char* seconds, char* outZZZZZHHmmss)
{
	DATE_TIME dt;
	GetRTCDateTime(&dt, 0);

	int nIncr = atoi(seconds);

	int nSec = (hex2int(dt.CLKSEC) + nIncr) % 60;
	nIncr = (hex2int(dt.CLKSEC) + nIncr) / 60;

	int nMin = (hex2int(dt.CLKMIN) + nIncr) % 60;
	nIncr = (hex2int(dt.CLKMIN) + nIncr) / 60;

	int nHour = (hex2int(dt.CLKHOUR) + nIncr) % 24;
	nIncr = (hex2int(dt.CLKHOUR) + nIncr) / 24;

	int nZZZZZ = 0;

	sprintf(outZZZZZHHmmss, "%05d%02d%02d%02d", nZZZZZ, nHour, nMin, nSec);
}
void changeMode()
{
	char found =0;
	char c;
	for (char idx = 0; idx < 16; idx++)
	{
		DBGLine("Find app (%d)", idx);
		c = FIND_APPLICATION(idx & 0xFF);
		DBGLine("Find app (%d): %02X (%c)", idx, c, c);
		if ((c & 0xFF == 0xff)&&(found<idx)) { found=idx; } 
		
	}
	DBGLine("Find app found (%d): %02X (%c)", found, c, c);

	DBGLine("Burn (press any key)");
	//while((KEYSCAN()&0xff)==0xff);
	 SAVE_BOOT_APPLICATION(0);
	 SELECT_APPLICATION(0);
}

void makeDateTime(const char* inStr, char* outStr)
{
	memcpy(outStr, inStr, 2);
	outStr += 2;
	*outStr = '-';
	memcpy(++outStr, inStr + 2, 2);
	outStr += 2;
	*outStr = '-';
	memcpy(++outStr, inStr + 4, 2);
	outStr += 2;
	*outStr = ' ';
	memcpy(++outStr, inStr + 6, 2);
	outStr += 2;
	*outStr = ':';
	memcpy(++outStr, inStr + 8, 2);
	outStr += 2;
	*outStr = ':';
	memcpy(++outStr, inStr + 10, 2);
	outStr[18] = 0;
}

void makeDisplaySumm(const char* strIn, int nInLen, char* strOut)
{
	int* pDigits = (int*)g_pParam->getParam(CParam::PARAM_DIGITS, true);
	int nDigits = 0;
	if (!pDigits)
	{
		DBGLine("Warning: makeDisplaySumm() - !pDigits");
		nDigits = 2;
	}
	else
	{
		nDigits = *pDigits;
	}

	strcpy(strOut, "            ");
	char* strBasicLine = new char[nDigits + 3];
	memset(strBasicLine, 0, nDigits + 3);
	memset(strBasicLine, '0', nDigits + 2);
	strBasicLine[1] = '.';
	strcpy(strOut + DISPLAY_WIDTH - 5 - nDigits - 2, strBasicLine);

	int		nDispPos	= DISPLAY_WIDTH - 6;
	while (*strIn == '0')
	{
		++strIn;
		--nInLen;
	}
	int j = 0;
	for (int i = nInLen - 1; i >= 0; --i)
	{
		if (j == nDigits)
			strOut[nDispPos--] = '.';

		strOut[nDispPos--] = strIn[i];
		++j;
	}

	DELETE_POINTER_ARR(strBasicLine);
}

void splitLeftZeros(const char* strIn, char* strOut)
{
	const char* strIt = strIn;
	while (*strIt == '0')
		strIt++;
	strcpy(strOut, strIt);
}

int confirm(CParam &oParam)
{
	char key = oParam.lastKey();
	if (key == BTKEY_ENTER/* || key == BTKEY_CLEAR*/)
		return 1;

	return 0;
}

void transformCheck(char* inCheck, char* outCheck)
{
	int nInPos	= 0;
	int nOutPos	= 0;
	int nCnt	= 0;
	int nStart	= 0;
	int nStop	= 0;

	BYTE pOut[150 * 32 + 1];
	memset(pOut, 0, 150 * 32 + 1);

	while (inCheck[nInPos] != 0)
	{
		if (inCheck[nInPos] == '\r')
		{
			pOut[nOutPos++] = ' ';
			++nInPos;
		}

		if (inCheck[nInPos] == '\n')// || inCheck[nInPos] == '\n')
		{
			nStop = nOutPos;
			int n = nStop - nStart;
			if (n > 32)
			{
				nOutPos -= n - 32;
				memset(pOut + nOutPos, 0, n - 32);
			}
			else if (n < 32)
			{
				memset(pOut + nOutPos, ' ', 32 - n);
				nOutPos += 32 - n;
			}
			++nInPos;
			nStart = nOutPos;

			continue;
		}

		pOut[nOutPos++] = inCheck[nInPos++];
	}

	pOut[nOutPos] = 0;

	strcpy((char*) outCheck, (char*) pOut);
}

unsigned int Length (const unsigned char s[])
{
	int len = 0;
	while (s[len++]);
	return len - 1;
}

unsigned char WinToDos(unsigned char tok)
{
	return g_codepage[tok];
}

void WinToDos(unsigned char* str)
{
	if (str == NULL)
		return;
	for (int i = 0; i < Length(str); i++)
		str[i] = WinToDos(str[i]);
}

int pressAnyButton(CParam& oParam)
{
	return ((oParam.lastKey() == (char) BTKEY_NULL) ? 0 : 1);
}

//-----------------------------------------------------------------------------/

bool compareRegularStrings(const char *pStrExactly, const char *pStrRegular)
{
	bool bRes = true;
	for (int idxStr = 0; idxStr < strlen(pStrExactly); idxStr++)
	{
		if (idxStr >= strlen(pStrRegular))
			break;
		if (pStrRegular[idxStr] == '*' || pStrRegular[idxStr] == '?')
			continue;
		if (pStrExactly[idxStr] != pStrRegular[idxStr])
		{
			bRes = false;
			break;
		}
	}
	if (strlen(pStrRegular) > strlen(pStrExactly))
	{
		for (int idx = strlen(pStrExactly); idx < strlen(pStrRegular); idx++)
		{
			if (pStrRegular[idx] == '*' || pStrRegular[idx] == '?')
				continue;
			bRes = false;
		}
	}
	return bRes;
}

void showLogo()
{
	ClearLCDScreen();
	GRA_LOAD((char*)Logo, 0, NUMBER_OF_DISPLAY_LINES);
}
void showVersion()
{
	ClearLCDScreen();


	// ѕока оставл€ем как есть
	Lprint(0, 0, PROGRAM_DEVICE);
	Lprint(1, 0, MACRO_VALUE_TO_STR(PROJ_NAME));
	Lprint(2, 0, MACRO_VALUE_TO_STR(PROJ_MODE));
	Lprint(3, 0, PROGRAM_DATE);
	//Wait1MS(3000);
	//ClearLCDScreen();
}

void purge_char(char* str, char ch)
{
	int i, j;

	if (str == NULL || strlen(str) == 0)
		return;
	for (i = 0, j = 0; i < strlen(str); ++i)
	{
		if (str[i] == ch)
			continue;
		str[j]=str[i];
		j++;
	}
	str[j]=0;
	return;
}


