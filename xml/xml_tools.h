
#ifndef __XML_TOOLS_H
#define __XML_TOOLS_H

#include "xml_common.h"

namespace pugi
{
	class xml_node;
}

class IVariable;

void dbg_storage_node(const pugi::xml_node& storage_node);

IVariable*		storage_node2var(const char* var_name, const char* type, const char* target, bool bIsSystem = false);
STORAGE_TYPE	storage_name2type(const char* storage_type, const char* storage_target);
FILTER_TYPE		storage_filterName2filterType(const char* storage_filter);

ACTION_TYPE		action_name2type(const char* action_type);

INPUT_FORMAT	input_formatType2formatType(const char* format_type);

FUNC_TYPE		func_name2type(const char* func_name);

MENU_TYPE		menu_name2type(const char* menu_name);	

string			applyFilter(FILTER_TYPE eFilter, const string& strSrc);

#endif
