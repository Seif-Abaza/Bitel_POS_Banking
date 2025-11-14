
#include "xml_tools.h"
#include "../internal/tools.h"
#include "../internal/CParam.h"
#include "../internal/printer_common.h"
#include "CStorage.h"
#include "pugixml/pugixml.hpp"
#include "../internal/CArabicEncoder.h"
#include "../internal/CArabicString.h"
#include "../tasks/CTaskDisplay.h"

using namespace pugi;

//--------------------------------------------------------------------------------------------------
void dbg_storage_node(const xml_node& storage_node)
{
	xml_attribute atr_type		= storage_node.attribute(ATR_TYPE);
	xml_attribute atr_target	= storage_node.attribute(ATR_STRG_TARGET);
	xml_attribute atr_var		= storage_node.attribute(ATR_STRG_VAR);
	xml_attribute atr_value		= storage_node.attribute(ATR_VALUE);
	xml_attribute atr_filter	= storage_node.attribute(ATR_STRG_FILTER);

	DBGLine("storage node, storage_type: \"%s\", storage_target: \"%s\", var: \"%s\", value: \"%s\", filter: \"%s\"",
		atr_type.value(), atr_target.value(), atr_var.value(), atr_value.value(), atr_filter.value());
}
//--------------------------------------------------------------------------------------------------
STORAGE_TYPE storage_name2type(const char* storage_type, const char* storage_target)
{
	STORAGE_TYPE	eType = ST_UNKNOWN;
	do 
	{
		if (!strcmp(storage_type, ATR_STRG_GLOBAL))					{ eType = ST_GLOBAL;				break; }
		if (!strcmp(storage_type, ATR_STRG_TRNANSACTION))
		{
			do 
			{
				if (!strcmp(storage_target, ATR_TRNTYPE_SEND))		{ eType = ST_TRANSACTION_SEND;		break; }
				if (!strcmp(storage_target, ATR_TRNTYPE_RECEIVE))	{ eType = ST_TRANSACTION_RECEIVE;	break; }
				if (!strcmp(storage_target, ATR_TRNTYPE_BACKUP))	{ eType = ST_TRANSACTION_BACKUP;	break; }
			} while (false);
		}
	} while (false);

	if (eType == ST_UNKNOWN)
		DBGLine("Error: storage_name2type() - unknown var, storage_type: %s, storage_target: %s", storage_type, storage_target);

	return eType;
}
//--------------------------------------------------------------------------------------------------
IVariable* storage_node2var(const char* var_name, const char* type, const char* target, bool bIsSystem /* = false */)
{
	IVariable*	pResVar = 0;
	do 
	{
		if (!strlen(var_name))
		{
			DBGLine("Error: storage_node2var() - var name doesn't exists");
			break;
		}

		STORAGE_TYPE	eType = storage_name2type(type, target);
		if (eType == ST_UNKNOWN)
			break;

		pResVar = g_pStorage->getVariable(eType, string(var_name), bIsSystem);
	} while (false);

	return pResVar;
}
//--------------------------------------------------------------------------------------------------
FILTER_TYPE storage_filterName2filterType(const char* storage_filter)
{
	if (!strlen(storage_filter))	// pugi почему-то вместо пустых атрибутов вставляет какой-то спец. символ
		return FLT_NONE;

	FILTER_TYPE	eFilter = FLT_NONE;

	do 
	{	if (!strcmp(storage_filter, ATR_FILTER_DISP_SUM_BAL))	{ eFilter = FLT_DISPLAY_SUM_BAL;	break; }
		if (!strcmp(storage_filter, ATR_FILTER_DISP_SUM))		{ eFilter = FLT_DISPLAY_SUM;	break; }
		if (!strcmp(storage_filter, ATR_FILTER_ERR_DESC))		{ eFilter = FLT_ERR_DESC;		break; }
		if (!strcmp(storage_filter, ATR_FILTER_TRN_DATE))		{ eFilter = FLT_TRN_DATE;		break; }
		if (!strcmp(storage_filter, ATR_FILTER_IP))				{ eFilter = FLT_IP;				break; }
		if (!strcmp(storage_filter, ATR_FILTER_BILL_DATE))		{ eFilter = FLT_BILL_DATE;		break; }
		if (!strcmp(storage_filter, ATR_FILTER_TRN_TIME))		{ eFilter = FLT_TRN_TIME;		break; }
		if (!strcmp(storage_filter, ATR_FILTER_TERM_DATE))		{ eFilter = FLT_TERM_DATE;		break; }
		if (!strcmp(storage_filter, ATR_FILTER_TERM_TIME))		{ eFilter = FLT_TERM_TIME;		break; }
		if (!strcmp(storage_filter, ATR_FILTER_HIDE_PASS))		{ eFilter = FLT_HIDE_PASS;		break; }
		if (!strcmp(storage_filter, ATR_FILTER_HIDE_CARD_NUM))	{ eFilter = FLT_HIDE_CARD_NUM;	break; }
		if (!strcmp(storage_filter, ATR_FILTER_DATE))			{ eFilter = FLT_DATE;		break; }
		if (!strcmp(storage_filter, ATR_FILTER_ARAB))			{ eFilter = FLT_ARAB;		break; }

	} while (false);

	if (eFilter == FLT_NONE)
		DBGLine("Warning: storage_filterName2filterType() - unknown filter: \"%s\"", storage_filter);

	return eFilter;
}
//--------------------------------------------------------------------------------------------------
ACTION_TYPE action_name2type(const char* action_type)
{
	ACTION_TYPE	eType = AT_UNKNOWN;

	do 
	{
		if (!strcmp(action_type, ATR_ACTYPE_INPUT))			{ eType = AT_INPUT;			break; }
		if (!strcmp(action_type, ATR_ACTYPE_MENU))			{ eType = AT_MENU;			break; }
		if (!strcmp(action_type, ATR_ACTYPE_STATIC))		{ eType = AT_STATIC;		break; }
		if (!strcmp(action_type, ATR_ACTYPE_TRANSACTION))	{ eType = AT_TRANSACTION;	break; }
		if (!strcmp(action_type, ATR_ACTYPE_CARDREADER))	{ eType = AT_CARDREADER;	break; }
		if (!strcmp(action_type, ATR_ACTYPE_PINPAD))		{ eType = AT_PINPAD;		break; }
		if (!strcmp(action_type, ATR_ACTYPE_PRINTER))		{ eType = AT_PRINTER;		break; }
		if (!strcmp(action_type, ATR_ACTYPE_CONFIRM))		{ eType = AT_CONFIRM;		break; }
		if (!strcmp(action_type, ATR_ACTYPE_FUNC))			{ eType = AT_FUNC;			break; }
		if (!strcmp(action_type, ATR_ACTYPE_COPY))			{ eType = AT_COPY;			break; }
	} while (false);

	if (eType == AT_UNKNOWN)
		DBGLine("Error: action_name2type() - unknown action type: \"%s\"", action_type);

	return eType;
}
//--------------------------------------------------------------------------------------------------
INPUT_FORMAT input_formatType2formatType(const char* format_type)
{
	INPUT_FORMAT	eFormat = IF_DIGITS;

	do 
	{
		if (!strcmp(format_type, ATR_IF_DIGITS))	{ eFormat = IF_DIGITS;	break; }
		if (!strcmp(format_type, ATR_IF_LETTERS))	{ eFormat = IF_LETTERS;	break; }
	} while (false);

	return eFormat;
}
//--------------------------------------------------------------------------------------------------
FUNC_TYPE func_name2type(const char* func_name)
{
	FUNC_TYPE	eType = FNT_UNKNOWN;

	do 
	{
		if (!strcmp(func_name, "get_port"))		{ eType = FNT_PORT_GET;		break; }
		if (!strcmp(func_name, "get_ip"))		{ eType = FNT_IP_GET;		break; }
		if (!strcmp(func_name, "set_host"))		{ eType = FNT_HOST_SET;		break; }
		if (!strcmp(func_name, "get_batch"))	{ eType = FNT_BATCH_GET;	break; }
		if (!strcmp(func_name, "set_batch"))	{ eType = FNT_BATCH_SET;	break; }
		if (!strcmp(func_name, "manage"))		{ eType = FNT_MANAGE;		break; }
		if (!strcmp(func_name, "can_con"))		{ eType = FNT_CAN_CON;		break; }
		if (!strcmp(func_name, "param_change"))	{ eType = FNT_PARAM_CHANGE;	break; }
		if (!strcmp(func_name, "config"))		{ eType = FNT_CONFIG;		break; }



		if (!strcmp(func_name, ATR_FNTYPE_BOOL))			{ eType = FNT_BOOL;				break; }
		if (!strcmp(func_name, ATR_FNTYPE_EQUAL))			{ eType = FNT_EQUAL;			break; }
		if (!strcmp(func_name, ATR_FNTYPE_TERM_ID_SAVE))	{ eType = FNT_TERM_ID_SAVE;		break; }
		if (!strcmp(func_name, ATR_FNTYPE_TERM_ID_GET))		{ eType = FNT_TERM_ID_GET;		break; }
		if (!strcmp(func_name, ATR_FNTYPE_CALC_FEE))		{ eType = FNT_CALC_FEE;			break; }
		if (!strcmp(func_name, ATR_FNTYPE_TERM_DATE_SAVE))	{ eType = FNT_TERM_DATE_SAVE;	break; }
		if (!strcmp(func_name, ATR_FNTYPE_TERM_DATE_GET))	{ eType = FNT_TERM_DATE_GET;	break; }
		if (!strcmp(func_name, ATR_FNTYPE_TERM_TIME_GET))	{ eType = FNT_TERM_TIME_GET;	break; }
		if (!strcmp(func_name, ATR_FNTYPE_LCD))				{ eType = FNT_LCD;		break; }
		if (!strcmp(func_name, ATR_FNTYPE_MAC))				{ eType = FNT_MAC;		break; }
		if (!strcmp(func_name, ATR_FNTYPE_ENC))				{ eType = FNT_ENC;		break; }

	} while (false);

	if (eType == FNT_UNKNOWN)
		DBGLine("Error: func_name2type() - unknown func, type: \"%s\"", func_name);

	return eType;
}
//--------------------------------------------------------------------------------------------------
MENU_TYPE menu_name2type(const char* menu_name)
{
	if (!strlen(menu_name))	// pugi почему-то вместо пустых атрибутов вставляет какой-то спец. символ
		return MT_UNKNOWN;

	MENU_TYPE	eType = MT_UNKNOWN;

	do 
	{
		if (!strcmp(menu_name, ATR_MENUTYPE_MAC))		{ eType = MT_MAC;		break; }
		if (!strcmp(menu_name, ATR_MENUTYPE_ENC))		{ eType = MT_ENC;	break; }
	} while (false);

	if (eType == MT_UNKNOWN)
		DBGLine("Warning: menu_name2type() - unknown  dynamic menu: \"%s\"", menu_name);

	return eType;
}
//--------------------------------------------------------------------------------------------------
string applyFilter(FILTER_TYPE eFilter, const string& strSrc)
{
	switch (eFilter)
	{
	case FLT_DISPLAY_SUM:
		{
			int* pDigits = (int*) g_pParam->getParam(CParam::PARAM_DIGITS, true);
			int nDigits = 0;
			if (!pDigits)
				nDigits = 2;
			else
				nDigits = *pDigits;

			string strFormated;
			if (strSrc.length() < nDigits + 1)
			{
				// Дополнить нулями, необходимо nDigits + 1 символов
				strFormated.format("%0*d", nDigits + 1 - strSrc.length(), 0);
				strFormated.append(strSrc.buffer());
			} else
				strFormated = strSrc;
			strFormated.insert(strFormated.length() - nDigits, ".");

			return strFormated;
		}
	case FLT_DISPLAY_SUM_BAL:
		{			
			return applyFilter(FLT_DISPLAY_SUM,strSrc.right(strSrc.length()-1));
		}
	case FLT_ARAB:
		{	CTaskDisplay::setLang(0);
			string strName = CArabicEncoder(strSrc).encode();
			return CArabicString(strName).rightToLeft(16);

		}
	case FLT_TRN_DATE:
		{
			char	strDate[10] = {0};
			strDate[2] = '-';
			strDate[5] = '-';
			memcpy(strDate, strSrc.buffer(), 2);
			memcpy(strDate + 3, strSrc.buffer() + 2, 2);
			memcpy(strDate + 3 + 3, strSrc.buffer() + 2 + 2, 3);
			strDate[8] = 0;

			return string(strDate);
		}
	case FLT_IP:
		{
			char	srcBuf[14]	= {0};
			int		nLen		= (strSrc.length() > 13) ? 13 : strSrc.length();
			memset(srcBuf, ' ', 13);
			memcpy(srcBuf, strSrc.buffer(), nLen);
			string strBuf(srcBuf);


			char	strIp[16] = {0};
			memset(strIp, ' ', 16);
			strIp[3] = '.';
			strIp[7] = '.';
			strIp[11] = '.';		
			
			memcpy(strIp, strBuf.buffer(), 3);
			memcpy(strIp + 4, strBuf.buffer() + 3, 3);
			memcpy(strIp + 4 + 4, strBuf.buffer() + 3 + 3, 3);
			memcpy(strIp + 4 + 4 + 4, strBuf.buffer() + 3 + 3 + 3, 4);
			strIp[8]=strSrc.buffer()[6];
			return string(strIp);
		}
	case FLT_ERR_DESC:
		{ 
			
			return string(strSrc);       //?
		}
	case FLT_BILL_DATE:
		{
			char	srcBuf[7]	= {0};
			int		nLen		= (strSrc.length() > 6) ? 6 : strSrc.length();
			memset(srcBuf, '0', 6);
			memcpy(srcBuf, strSrc.buffer(), nLen);

			char	strDate[11] = {0};
			strDate[2] = '/';
			strDate[5] = '/';
			strDate[6] = '2';
			strDate[7] = '0';
			memcpy(strDate, srcBuf+2+2, 2);
			memcpy(strDate + 3, srcBuf+ 2, 2);
			memcpy(strDate + 3 + 5, srcBuf, 2);
			strDate[10] = 0;

			return string(strDate);
		}
	

	case FLT_TRN_TIME:
		{
			char	strTime[10] = {0};
			strTime[2] = ':';
			strTime[5] = ':';
			memcpy(strTime, strSrc.buffer() + 6, 2);
			memcpy(strTime + 3, strSrc.buffer() + 6 + 2, 2);
			memcpy(strTime + 3 + 3, strSrc.buffer() + 6 + 2 + 2, 3);
			strTime[8] = 0;

			return string(strTime);
		}
	case FLT_TERM_DATE:
		{
			char	srcBuf[8]	= {0};
			int		nLen		= (strSrc.length() > 7) ? 7 : strSrc.length();
			memset(srcBuf, '0', 7);
			memcpy(srcBuf, strSrc.buffer(), nLen);

			return applyFilter(FLT_TRN_DATE, srcBuf);
		}

	case FLT_TERM_TIME:
		{
			char	srcBuf[14]	= {0};
			int		nLen		= (strSrc.length() > 7) ? 7 : strSrc.length();
			memset(srcBuf, '0', 13);
			memcpy(srcBuf + 6, strSrc.buffer(), nLen);

			return applyFilter(FLT_TRN_TIME, srcBuf);
		}
	case FLT_HIDE_PASS:
		{
			int		nLen	= strSrc.length();
			char*	pBuf	= new char[nLen + 1];	AUTO_DELETE_ARR(char, pBuf);
			pBuf[nLen]		= 0;
			memset(pBuf, '*', nLen);
			if (nLen > 0 &&  strSrc.buffer()[nLen-1] == '@')
			{				
				pBuf[nLen - 1]	= '@';
			}

			return string(pBuf);

		}
	case FLT_HIDE_CARD_NUM:
		{
			char strDispCN[18];
			memset(strDispCN, 'x', 16);
			memcpy(strDispCN, strSrc.buffer(), 6);
			memcpy(strDispCN + 11, strSrc.buffer() + strSrc.length() - 4, 4);
			strDispCN[17] = 0;

			return string(strDispCN);
		}
	}
	return strSrc;
}
//--------------------------------------------------------------------------------------------------
