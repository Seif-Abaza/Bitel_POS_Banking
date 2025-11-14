#include "../extern.h"
#include "CTLVParser.h"
#include "../fs/CFile.h"
#include "tools.h"
#include "../iso/common.h"


CTLVParser* g_pTlvParser = NULL;

#define FILE_CONFIG_BUFFER_SIZE  (10*1024) // 10 kbytes
#define FILE_NAME_CONFIG			"config.tlv"
#define FILE_NAME_CONFIG_DELAYED	"config.delayed"
#define FILE_NAME_FIRMWARE_DELAYED	"firmware.delayed"
#define FILE_NAME_CONFIG_CR			"config_cr.cfg"




int parseArrayItems(unsigned char* buf,unsigned long size,int arrayItem,std::vector<SDataCnt> &dataCnt);
int clearCntVectors(std::vector<SDataCnt> &dataCnt);

CTLVParser::TAG_DATA_TYPE CTLVParser::getTagDataType(TAG_TYPE tag)
{
	switch( tag )
	{
	case TAG_ADM100 :return TAG_DATA_ADM100;
	case TAG_MY_NODE :return TAG_DATA_INT;
	case TAG_TERM_NODE :return TAG_DATA_INT;
	case TAG_TERM_INTERFACE :return TAG_DATA_INT;
	case TAG_HOST_NODE :return TAG_DATA_INT;
	case TAG_HOST_INTERFACE :return TAG_DATA_INT;
	case TAG_HOST_BACKUP_INDEX :return TAG_DATA_INT;
	case TAG_MY_NODE_INTERFACE :return TAG_DATA_INT;

	case TAG_TERMINAL_ID :return TAG_DATA_STRING_ARRAY;
	case TAG_CURRENCY_ID :return TAG_DATA_INT;
	case TAG_LANGUAGE :return TAG_DATA_INT;
	case TAG_LOG_LEVEL :return TAG_DATA_INT;
	case TAG_ADMIN_NODE :return TAG_DATA_INT;
	case TAG_TERM_PROTOCOL :return TAG_DATA_INT;
	case TAG_HOST_PROTOCOL :return TAG_DATA_INT;
	case TAG_USE_LOGON :return TAG_DATA_INT;
	case TAG_LOAD_TMK :return TAG_DATA_INT;
	case TAG_OPEN_TIMEOUT :return TAG_DATA_INT;
	case TAG_INPUT_PIN_TIMEOUT :return TAG_DATA_INT;
	case TAG_CHECK_EXP_DATE :return TAG_DATA_INT;
	case TAG_TPK_LOAD_RERIOD :return TAG_DATA_INT;
	case TAG_AUTH_TIMEOUT :return TAG_DATA_INT;
	case TAG_AUTH_TIME :return TAG_DATA_INT;
	case TAG_CONFIRM_TIMEOUT :return TAG_DATA_INT;
	case TAG_CARD_READ_TIMEOUT :return TAG_DATA_INT;
	case TAG_SERAL_PORT_SETTINGS :return TAG_DATA_INT;
	case TAG_SERAL_PORT_BAUD_RATE :return TAG_DATA_INT;
	case TAG_TERMINAL_CONNECTION_TYPE :return TAG_DATA_INT;
	case TAG_CHECK_HEADER :return TAG_DATA_STRING_ARRAY;
	case TAG_TRACE_ANSWER :return TAG_DATA_STRING_ARRAY;
	case TAG_SERVER_NODE_VERIFICATION :return TAG_DATA_INT;
	case TAG_PRN_CHECK_NUM :return TAG_DATA_INT;
	case TAG_CHANGE_TERM_ENBL :return TAG_DATA_INT;
	case TAG_RRN_FOR_REFUND :return TAG_DATA_INT;
	case TAG_CHECK_OPERATION_SUPPORT :return TAG_DATA_INT;
	case TAG_EMV_USE_PIN :return TAG_DATA_INT;
	case TAG_EMV_ENABLED :return TAG_DATA_INT;
	case TAG_APPLICATION_TYPE :return TAG_DATA_INT;
	case TAG_MAIN_GATE :return TAG_DATA_INT;
	case TAG_BACKUP_GATE1 :return TAG_DATA_INT;
	case TAG_BACKUP_GATE2 :return TAG_DATA_INT;
	case TAG_BACKUP_GATE3 :return TAG_DATA_INT;
	case TAG_BACKUP_GATE4 :return TAG_DATA_INT;
	case TAG_HOST_TYPE :return TAG_DATA_INT;
	default : return TAG_INVALID;
	}
}


class CTLVTag
{
public :
	CTLVParser::TAG_DATA_TYPE m_eType;
	int nTag;
	int nIntData;
	int m_nSize; //m_nSize=0, на стороне девайса должен означать, что необходимо очистить значение.
	int nItemsCount;
	bool bIsComposite;
	char *m_pData;
};

int swapint(int value,int cnt)
{
	struct Sint{char b1,b2,b3,b4;};

	Sint *p;
	p=(Sint*)&value;

	int tmp=value;
	Sint *b;
	b=(Sint*)&tmp;

	if (cnt==4)
	{
		b->b1=p->b4;
		b->b2=p->b3;
		b->b3=p->b2;
		b->b4=p->b1;
		return tmp;
	};

	if (cnt==3) {
		b->b1=p->b3;
		b->b2=p->b2;
		b->b3=p->b1;
		return tmp;
	};

	if (cnt==2) {
		b->b1=p->b2;
		b->b2=p->b1;
		return tmp;
	};

	return(tmp);

}

short swapshort(short a)
{
	struct Sshort{char b1,b2;};
	Sshort *p;
	p=(Sshort*)&a;
	int tmp=a;
	Sshort *b;
	b=(Sshort*)&tmp;
	b->b1=p->b2;
	b->b2=p->b1;
	return tmp;
}

int CTLVParser::CheckReturn(TAG_TYPE tag)
{
	if (m_map.find(tag)!=m_map.end() && m_map[tag].nIntData != 0)
	{
		return m_map[tag].nIntData;
	} else
	{
		return -1;
	}
}


int CTLVParser::getAuthTimeout()
{
	return CheckReturn(TAG_AUTH_TIMEOUT);
}

int CTLVParser::getAuthTime()
{
	return CheckReturn(TAG_AUTH_TIME);
}


int CTLVParser::getOpenTimeout()
{	
	return CheckReturn(TAG_OPEN_TIMEOUT);
}

int CTLVParser::getHostInterface()
{
	return CheckReturn(TAG_HOST_INTERFACE);
}

int CTLVParser::getHostBackupIndex()
{
	return CheckReturn(TAG_HOST_BACKUP_INDEX);
}



int CTLVParser::getAdminNodesCount()
{
	if (m_map[TAG_ADMIN_ARRAY].nItemsCount != 0) return m_map[TAG_ADMIN_ARRAY].nItemsCount;
	return 1;
}


int CTLVParser::getDeviceGUID(GUID* pstc)// (unsigned long *Data1,unsigned short *Data2,unsigned short *Data3,char *Data4[8])
{
	if (m_map[TAG_DEVICE_GUID].m_nSize!=0x10 || !pstc) return 0;

	memcpy(&(pstc->Data1), m_map[TAG_DEVICE_GUID].m_pData,4);
	//pstc->Data1 = swapint(pstc->Data1,4);

	memcpy(&(pstc->Data2),m_map[TAG_DEVICE_GUID].m_pData+4,2);
	//pstc->Data2 = swapshort(pstc->Data2);

	memcpy(&(pstc->Data3),m_map[TAG_DEVICE_GUID].m_pData+6,2);
	//pstc->Data3 = swapshort(pstc->Data3);

	memcpy(&(pstc->Data4),m_map[TAG_DEVICE_GUID].m_pData+8,8);

	return 1;
}


int CTLVParser::getHostNodesCount()
{
	if (m_map[TAG_HOST_NODES_ARRAY].nItemsCount != 0) return m_map[TAG_HOST_NODES_ARRAY].nItemsCount;
	return 1;
}


int clearCntVectors(std::vector<SDataCnt> &dataCnt)
{
	if (dataCnt.size()==0) return 0;
	for (int nIt=0;nIt!=dataCnt.size();nIt++)
	{
		delete (dataCnt[nIt].data);
		dataCnt[nIt].dataSize=0;
		dataCnt[nIt].tag=0;
	}
	return 1;
}

int fillVectorFromTags(unsigned char* buf,unsigned long size,std::vector<SDataCnt> &dataCnt)
{
	// SDataCnt i;
	// i.dataSize=tag_valueSize;
	// dataCnt.push_back(i);

	unsigned long cur_pos=0;
	unsigned long tags_count = 0;
	unsigned long tag_valueSize=0;

	if (size == 0 )
		return 0;

	while(cur_pos!=size)
	{
		unsigned char len_byte_1=0;
		unsigned long value_size;
		unsigned long tag_value = 0;
		unsigned long tag_number = 0;
		value_size =0;

		//if (!buf[cur_pos]&0x20) return NULL;

		if ((buf[cur_pos]&0x1F)==0x1F)
			cur_pos++;

		for(unsigned char i=0;i<4 && cur_pos<size;i++)
		{
			(tag_number)<<=7;
			(tag_number)|= (buf[cur_pos] & 0x7F);
			if (!(buf[cur_pos++]&0x80))
				break;
		}
		if (cur_pos>= size)
			return 0;
		len_byte_1=buf[cur_pos++];

		if (!(len_byte_1 & 0x80))
		{
			(value_size)=len_byte_1;
		}
		else
		{
			len_byte_1=len_byte_1&0x7F;
			for(unsigned char i=0;i<len_byte_1; i++)
			{
				(value_size)=((value_size)<<8) | buf[cur_pos++];
			}
		}
		if (cur_pos+(value_size)>size)
			return 0;

		tags_count++;

		cur_pos+=value_size;
		tag_valueSize=value_size;

		SDataCnt i;
		i.dataSize=tag_valueSize;
		i.data = new char[tag_valueSize];
		i.tag = tag_number;
		memcpy(i.data,(unsigned char*)(buf+cur_pos-tag_valueSize),tag_valueSize);
		dataCnt.push_back(i);
	}




	return 1;
}



int parseArrayItems(unsigned char* buf,unsigned long size,int arrayItem,std::vector<SDataCnt> &dataCnt)
{

	// SDataCnt i;
	// i.dataSize=100;
	// dataCnt.push_back(i);



	unsigned long cur_pos=0;
	unsigned long tags_count = 0;
	unsigned long tag_valueSize=0;

	if (size == 0 )
		return 0;

	while(cur_pos!=size)
	{
		unsigned char len_byte_1=0;
		unsigned long value_size;
		unsigned long tag_value = 0;
		unsigned long tag_number = 0;
		value_size =0;

		//if (!buf[cur_pos]&0x20) return NULL;

		if ((buf[cur_pos]&0x1F)==0x1F)
			cur_pos++;

		for(unsigned char i=0;i<4 && cur_pos<size;i++)
		{
			(tag_number)<<=7;
			(tag_number)|= (buf[cur_pos] & 0x7F);
			if (!(buf[cur_pos++]&0x80))
				break;
		}
		if (cur_pos>= size)
			return 0;
		len_byte_1=buf[cur_pos++];

		if (!(len_byte_1 & 0x80))
		{
			(value_size)=len_byte_1;
		}
		else
		{
			len_byte_1=len_byte_1&0x7F;
			for(unsigned char i=0;i<len_byte_1; i++)
			{
				(value_size)=((value_size)<<8) | buf[cur_pos++];
			}
		}
		if (cur_pos+(value_size)>size)
			return 0;

		tags_count++;

		cur_pos+=value_size;
		tag_valueSize=value_size;
		if (tags_count==arrayItem)
			break;
	}

	fillVectorFromTags((unsigned char*)(buf+cur_pos-tag_valueSize),tag_valueSize,dataCnt);
	return 1;
}

int CTLVParser::getAdminNodesItem(int nItem)
{
	if (m_map[TAG_ADMIN_ARRAY].nItemsCount == 0)
	{
		return m_map[TAG_ADMIN_NODE].nIntData;
	}

	else
	{ std::vector<SDataCnt> dataCnt;
	int AdminNode=0;
	parseArrayItems((unsigned char*)m_map[TAG_TYPE(0x2d)].m_pData,m_map[TAG_TYPE(0x2d)].m_nSize,nItem,dataCnt);
	if (dataCnt.size()!=1) return 0;
	memcpy(&AdminNode,dataCnt[0].data,dataCnt[0].dataSize);
	AdminNode=swapint(AdminNode,dataCnt[0].dataSize);
	clearCntVectors(dataCnt);
	return AdminNode;
	}
	return 0;
}

int CTLVParser::getRASCount()
{
	if (m_map[TAG_RAS_ARRAY].nItemsCount != 0) return m_map[TAG_RAS_ARRAY].nItemsCount;
	if (m_map[TAG_RAS_ARRAY].m_nSize!=0) return 1;
	return 0;
}


int CTLVParser::getRASData(int nItem,int *operatorID, char *userName, char *password, char *APN, char *gateway,  int* autoDisconnectTime)
{
	std::vector<SDataCnt> dataCnt;

	parseArrayItems((unsigned char*)m_map[TAG_TYPE(TAG_RAS_ARRAY)].m_pData,m_map[TAG_TYPE(TAG_RAS_ARRAY)].m_nSize,nItem-1,dataCnt);

	for (int i=0; i < dataCnt.size(); i++)
	{
		if (dataCnt[i].tag==0x33)
		{
			memcpy(operatorID,dataCnt[i].data,dataCnt[i].dataSize);
			*operatorID=swapint(*operatorID,dataCnt[i].dataSize);
		}

		if (dataCnt[i].tag==0x34)
		{
			memcpy(userName,dataCnt[i].data,dataCnt[i].dataSize);
		}

		if (dataCnt[i].tag==0x35)
		{
			memcpy(password,dataCnt[i].data,dataCnt[i].dataSize);
		}

		if (dataCnt[i].tag==0x36)
		{
			memcpy(APN,dataCnt[i].data,dataCnt[i].dataSize);
		}

		if (dataCnt[i].tag==0x37)
		{
			memcpy(gateway,dataCnt[i].data,dataCnt[i].dataSize);
		}

		if (dataCnt[i].tag==0x38)
		{
			memcpy(autoDisconnectTime,dataCnt[i].data,dataCnt[i].dataSize);
			*autoDisconnectTime=swapint(*autoDisconnectTime,dataCnt[i].dataSize);
		}


	}

	clearCntVectors(dataCnt);

	return 1;
}


int CTLVParser::getHostsCount()
{
	if (m_map[TAG_HOSTS].nItemsCount != 0) return m_map[TAG_HOSTS].nItemsCount;
	if (m_map[TAG_HOSTS].m_nSize!=0) return 1;
	return 0;
}

int CTLVParser::getHostsData(int nItem, int* mainGate, int* backupGate1, int* backupGate2, int* backupGate3, int* backupGate4)
{
	std::vector<SDataCnt> dataCnt;

	parseArrayItems((unsigned char*)m_map[TAG_TYPE(TAG_HOSTS)].m_pData,m_map[TAG_TYPE(TAG_HOSTS)].m_nSize,nItem,dataCnt);

	for (int i=0; i < dataCnt.size(); i++)
	{
		if (dataCnt[i].tag==TAG_MAIN_GATE)
		{
			memcpy(mainGate,dataCnt[i].data,dataCnt[i].dataSize);
			*mainGate=swapint(*mainGate,dataCnt[i].dataSize);
		}
		if (dataCnt[i].tag==TAG_BACKUP_GATE1)
		{
			memcpy(backupGate1,dataCnt[i].data,dataCnt[i].dataSize);
			*backupGate1=swapint(*backupGate1,dataCnt[i].dataSize);
		}
		if (dataCnt[i].tag==TAG_BACKUP_GATE2)
		{
			memcpy(backupGate2,dataCnt[i].data,dataCnt[i].dataSize);
			*backupGate2=swapint(*backupGate2,dataCnt[i].dataSize);
		}
		if (dataCnt[i].tag==TAG_BACKUP_GATE3)
		{
			memcpy(backupGate3,dataCnt[i].data,dataCnt[i].dataSize);
			*backupGate3=swapint(*backupGate3,dataCnt[i].dataSize);
		}
		if (dataCnt[i].tag==TAG_BACKUP_GATE4)
		{
			memcpy(backupGate4,dataCnt[i].data,dataCnt[i].dataSize);
			*backupGate4=swapint(*backupGate4,dataCnt[i].dataSize);
		}
	}

	clearCntVectors(dataCnt);

	return 1;
}








int CTLVParser::getDNTCount()
{
	if (m_map[TAG_DNT_ARRAY].nItemsCount != 0) return m_map[TAG_DNT_ARRAY].nItemsCount;
	if (m_map[TAG_DNT_ARRAY].m_nSize!=0) return 1;
	return 0;
}

int CTLVParser::getDNTData(int nItem,int *serverNode, char *IP, int* serverPort, int* keepAlive, int* answerMaxTime)
{
	std::vector<SDataCnt> dataCnt;
	
	parseArrayItems((unsigned char*)m_map[TAG_TYPE(TAG_DNT_ARRAY)].m_pData,m_map[TAG_TYPE(TAG_DNT_ARRAY)].m_nSize,nItem,dataCnt);
	
	for (int i=0; i < dataCnt.size(); i++)
	{
		if (dataCnt[i].tag==0x3D)
		{
			memcpy(serverNode,dataCnt[i].data,dataCnt[i].dataSize);
			*serverNode=swapint(*serverNode,dataCnt[i].dataSize);
		}

		if (dataCnt[i].tag==0x3E)
		{
			memcpy(IP,dataCnt[i].data,dataCnt[i].dataSize);
		}

		if (dataCnt[i].tag==0x3F)
		{
			memcpy(serverPort,dataCnt[i].data,dataCnt[i].dataSize);
			*serverPort=swapint(*serverPort,dataCnt[i].dataSize);
		}

		if (dataCnt[i].tag==0x40)
		{
			memcpy(keepAlive,dataCnt[i].data,dataCnt[i].dataSize);
			*keepAlive=swapint(*keepAlive,dataCnt[i].dataSize);
		}

		if (dataCnt[i].tag==0x41)
		{
			memcpy(answerMaxTime,dataCnt[i].data,dataCnt[i].dataSize);
			*answerMaxTime=swapint(*answerMaxTime,dataCnt[i].dataSize);
		}

	}

	clearCntVectors(dataCnt);

	return 1;
}

int CTLVParser::getHostNodesItem(int nItem,int *HostNode,int *HostInterface)
{
	std::vector<SDataCnt> dataCnt;

	parseArrayItems((unsigned char*)m_map[TAG_TYPE(TAG_HOST_NODES_ARRAY)].m_pData,m_map[TAG_TYPE(TAG_HOST_NODES_ARRAY)].m_nSize,nItem,dataCnt);

	for (int i=0; i < dataCnt.size(); i++)
	{
		if (dataCnt[i].tag==0x131)
		{
			memcpy(HostNode,dataCnt[i].data,dataCnt[i].dataSize);
			*HostNode=swapint(*HostNode,dataCnt[i].dataSize);
		}
		if (dataCnt[i].tag==0x141)
		{
			memcpy(HostInterface,dataCnt[i].data,dataCnt[i].dataSize);
			*HostInterface=swapint(*HostInterface,dataCnt[i].dataSize);
		}
	}

	clearCntVectors(dataCnt);

	return 1;

}



CTLVParser::CTLVParser(void)
{
}

CTLVParser::~CTLVParser(void)
{
	while( !m_map.empty() ) {
		std::map<TAG_TYPE, CTLVTag>::iterator iter = m_map.begin();
		if ((*iter).second.m_pData!=NULL && (*iter).second.m_nSize!=0)
		{delete((*iter).second.m_pData);};
		m_map.erase( m_map.begin() );
	}
}

int countArrayItems(unsigned char* buf,unsigned long size)
{
	unsigned long cur_pos=0;
	unsigned long tags_count = 0;

	if (size == 0 )
		return 0;

	while(cur_pos!=size)
	{ unsigned char len_byte_1=0;
	unsigned long value_size;
	unsigned long tag_value = 0;
	unsigned long tag_number = 0;
	value_size =0;

	//if (!buf[cur_pos]&0x20) return NULL;

	if ((buf[cur_pos]&0x1F)==0x1F)
		cur_pos++;

	for(unsigned char i=0;i<4 && cur_pos<size;i++)
	{
		(tag_number)<<=7;
		(tag_number)|= (buf[cur_pos] & 0x7F);
		if (!(buf[cur_pos++]&0x80))
			break;
	}
	if (cur_pos>= size)
		return 0;

	len_byte_1=buf[cur_pos++];

	if (!(len_byte_1 & 0x80))
	{
		(value_size)=len_byte_1;
	}
	else
	{
		len_byte_1=len_byte_1&0x7F;
		for(unsigned char i=0;i<len_byte_1; i++)
		{
			(value_size)=((value_size)<<8) | buf[cur_pos++];
		}
	}
	if (cur_pos+(value_size)>size)
		return 0;

	tags_count++;
	cur_pos+=value_size;
	}
	return tags_count;
}

char* parseIntArray(unsigned char* buf,unsigned long size, int *itemsCount)
{
	unsigned long cur_pos=0;
	unsigned long tags_count = 0;
	*itemsCount = 0;

	std:: vector <int> vecT;

	if (size == 0 )
		return 0;

	while(cur_pos!=size)
	{
		unsigned char len_byte_1=0;
		unsigned long value_size;
		unsigned long tag_value = 0;
		unsigned long tag_number = 0;
		value_size =0;

		//if (!buf[cur_pos]&0x20) return NULL;

		if ((buf[cur_pos]&0x1F)==0x1F)
			cur_pos++;

		for(unsigned char i=0;i<4 && cur_pos<size;i++)
		{
			(tag_number)<<=7;
			(tag_number)|= (buf[cur_pos] & 0x7F);
			if (!(buf[cur_pos++]&0x80))
				break;
		}
		if (cur_pos>= size)
			return 0;
		len_byte_1=buf[cur_pos++];

		if (!(len_byte_1 & 0x80))
		{
			(value_size)=len_byte_1;
		}
		else
		{
			len_byte_1=len_byte_1&0x7F;
			for(unsigned char i=0;i<len_byte_1; i++)
			{
				(value_size)=((value_size)<<8) | buf[cur_pos++];
			}
		}
		if (cur_pos+(value_size)>size)
			return 0;

		// tag_value=(unsigned long*)(buf+cur_pos);

		unsigned long tagPos=cur_pos;
		unsigned long tag_size=value_size;

		if ((buf[tagPos]&0x1F)==0x1F)
			tagPos++;
		tag_number=0;


		for(unsigned char i=0;i<4 && tagPos<size;i++)
		{
			(tag_number)<<=7;
			(tag_number)|= (buf[tagPos] & 0x7F);
			if (!(buf[tagPos++]&0x80))
				break;
		}
		if (tagPos>= size)
			return 0;
		len_byte_1=buf[tagPos++];

		if (!(len_byte_1 & 0x80))
		{
			(tag_size)=len_byte_1;
		}
		else
		{
			len_byte_1=len_byte_1&0x7F;
			for(unsigned char i=0;i<len_byte_1; i++)
			{
				(tag_size)=((tag_size)<<8) | buf[tagPos++];
			}
		}
		//vecT.push_back();

		int ggg=0;

		memcpy(&ggg,&buf[tagPos],tag_size);

		vecT.push_back(swapint(ggg,tag_size));

		tags_count++;
		cur_pos+=value_size;

	}
	char *mem=new char [sizeof(int)*tags_count];

	*itemsCount = tags_count;
	return mem;

}

unsigned long parse_tag(unsigned char *buf, unsigned long buf_size, unsigned short *tag_number, unsigned long *value_size, unsigned char **tag_value, bool *bComposite)
{
	unsigned long cur_pos;
	unsigned char len_byte_1;
	unsigned char i;
	*bComposite=false;

	*value_size = 0;
	*tag_value = 0;
	*tag_number = 0;
	cur_pos=0;
	if (buf_size == 0 )
		return 0;

	if (buf[cur_pos]&0x20) *bComposite=true;

	if ((buf[cur_pos]&0x1F)==0x1F)
		cur_pos++;


	for(i=0;i<4 && cur_pos<buf_size;i++)
	{
		(*tag_number)<<=7;
		(*tag_number)|= (buf[cur_pos] & 0x7F);
		if (!(buf[cur_pos++]&0x80))
			break;
	}
	if (cur_pos>= buf_size)
		return 0;
	len_byte_1=buf[cur_pos++];
	/*two types of len*/
	if (!(len_byte_1 & 0x80))
	{
		(*value_size)=len_byte_1;
	}
	else
	{
		len_byte_1=len_byte_1&0x7F;
		for(i=0;i<len_byte_1; i++)
		{
			(*value_size)=((*value_size)<<8) | buf[cur_pos++];
		}
	}
	if (cur_pos+(*value_size)>buf_size)
		return 0;
	(*tag_value)=buf+cur_pos;
	//cur_pos+=(*value_size);
	return(cur_pos);
}


string CTLVParser::showCurrentCfg()
{
	string sOut;
	sOut="";

	CFile fFile;

	if (!fFile.open("config.tlv",O_RDONLY)){sOut+="file `config.tlv` not founded  \n\n";} 
		else 
		{
			sOut+="file `config.tlv` founded, parsing: \n\n";			
		};

	fFile.close();

	string sTmp;
	
	sTmp.format("TAGs count: %d : \n",(int)m_map.size());
	sOut+=sTmp;
	
	std::map<TAG_TYPE, CTLVTag>::iterator iter = m_map.begin();

	while(iter!=m_map.end()) {
		
		string sTmp;

		//sprintf((char*)sOut, "Tag = %X", (*iter).second.nTag);

		if ((*iter).second.bIsComposite)
		{
		sTmp.format("     TAG <0x%X Composite, %d items> \n",(int)(*iter).second.nTag,(int)(*iter).second.nItemsCount);
		}
		else
		{sTmp.format("     TAG <0x%X> value : 0x%X \n",(int)(*iter).second.nTag,(int)(*iter).second.nIntData);};

		if ((*iter).second.nTag!=0){sOut+=sTmp;}
		//(*iter).second.nTag;
		iter++;
	};



	sOut+=0;
	return sOut;
}

int CTLVParser::parseTLV(LPBYTE pTLVData, DWORD dwSize, DWORD* pdwCoreBegin, DWORD* pdwCoreSize, bool bTlvDelayed)
{
	bool bConfigExists = false;
	if (pTLVData==0) //it is mean to read config from file `config.tlv`
	{
		CFile fFile;
		if (!fFile.open("config.tlv",O_RDONLY)){fFile.close(); DBGLine ("No config file" );return 0;} 
		
		pTLVData = new BYTE [fFile.size()];

		
		fFile.read(pTLVData,fFile.size());


		dwSize = fFile.size();
		fFile.close();
		bConfigExists = true;
	}
	
	CTLVTag iCTLVTag;

	*pdwCoreBegin=0;
	*pdwCoreSize=0;

	// TAG_TYPE eType;
	unsigned short base_id = 0; //constant for this terminal
	bool bComposite;
	int tl=1;
	unsigned short current_tag;
	unsigned long cur_pos = 0;
	unsigned char *tag=NULL;
	unsigned long tag_end;
	//unsigned long *tag_size;
	unsigned long tag_size=0;
	unsigned long buf_size=dwSize;
	DWORD dwCoreBeginCur;
	while (cur_pos < buf_size)
	{
		current_tag=0;
		
		tag_end = parse_tag((unsigned char*)pTLVData+cur_pos, buf_size-cur_pos, &current_tag, &tag_size, &tag, &bComposite);
		tag_size=0;
		while (cur_pos < buf_size)
		{
			cur_pos+=tag_end+tag_size;
			tag_end = parse_tag((unsigned char*)pTLVData+cur_pos, buf_size-cur_pos, &current_tag, &tag_size, &tag, &bComposite);			

			iCTLVTag.nTag=current_tag;
			iCTLVTag.m_eType=getTagDataType((TAG_TYPE)current_tag);
			iCTLVTag.m_nSize=tag_size;
			iCTLVTag.nIntData=0;
			iCTLVTag.nItemsCount=0;


			if (bComposite){iCTLVTag.bIsComposite=true;} else {iCTLVTag.bIsComposite=false;};

			if (iCTLVTag.m_eType == TAG_INVALID)
			{
/* Вывод неопределенных тегов, включить если какието теги не будут приходить
				DBGLine("Unknown tag type: %X", current_tag);
*/
			}

			if (iCTLVTag.m_eType==TAG_DATA_INT)
			{
				if (tag_size > sizeof(int))
					DBGLine("Error: CTLVParser::parseTLV() - tag_size == %d", tag_size);
				memcpy(&iCTLVTag.nIntData, (unsigned char*)pTLVData+cur_pos+tag_end, ((tag_size > sizeof(int)) ? sizeof(int) : tag_size));
				iCTLVTag.nIntData=swapint(iCTLVTag.nIntData,tag_size);
			}

			if (current_tag!=TAG_CORE_BODY)
			{
				iCTLVTag.m_pData = new char [tag_size];
				memcpy(iCTLVTag.m_pData,(unsigned char*)pTLVData+cur_pos+tag_end,tag_size);
			} else 
			{
				*pdwCoreBegin=cur_pos+tag_end;
				dwCoreBeginCur=cur_pos;
				*pdwCoreSize=tag_size;
			}
			//DBGLine ("tag  do %s %d",iCTLVTag.m_pData,iCTLVTag.m_nSize );


			if (iCTLVTag.bIsComposite)
				{ iCTLVTag.nItemsCount=countArrayItems((unsigned char*)iCTLVTag.m_pData,iCTLVTag.m_nSize); }
		//	DBGLine ("tag  count %d currect tag %d  %d %s %d %d",iCTLVTag.nItemsCount,current_tag,iCTLVTag.m_nSize,iCTLVTag.m_pData,iCTLVTag.nIntData,iCTLVTag.nTag);

			// Ant, 24.07.09. Если тэг уже есть, то добавлять не нужно (и очистить память которую выделили)
			
			m_map[(TAG_TYPE) current_tag] = iCTLVTag;


		
		};

		if (tag_end<=0)
			break;

		tag_size = 0;
		tag=NULL;
		current_tag = 0;
	}
	

	bool bTLVOK = true;
	do 
	{	
		if (m_map.find(TAG_ADMIN_ARRAY) == m_map.end())
		{
			DBGLine("Error in parse tlv, no tag: TAG_ADMIN_ARRAY");
			bTLVOK = false;
			break;
		}		


		if (m_map.find(TAG_RAS_ARRAY) == m_map.end())
		{
			DBGLine("Error in parse tlv, no tag: TAG_RAS_ARRAY");
			bTLVOK = false;
			break;
		}

		if (m_map.find(TAG_DNT_ARRAY) == m_map.end())
		{
			DBGLine("Error in parse tlv, no tag: TAG_DNT_ARRAY");
			bTLVOK = false;
			break;		}

		
		DBGLine("TLV config OK");

	} while (false);

	// Ошибка, неверная конфигурация
	if (!bTLVOK)
	{
		return -1;
	}
	

	if (bConfigExists)
	{
		DELETE_POINTER_ARR(pTLVData);	// Необходимо, ибо если файл есть, то память выделяется в куче
	}

	return 1;

}



bool CTLVParser::checkConfigUpdated() const
{
	return m_bConfigUpdated;
}

bool CTLVParser::checkConfigDelayed() const
{
	CFile oFile;
	if (oFile.open(FILE_NAME_CONFIG_DELAYED, O_RDONLY))
	{
		oFile.close();
		return true;
	}
	if (oFile.open(FILE_NAME_FIRMWARE_DELAYED, O_RDONLY))
	{
		oFile.close();
		return true;
	}
	return false;
}

