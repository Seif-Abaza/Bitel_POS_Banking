
#ifndef __XML_COMMON_H
#define __XML_COMMON_H

#include "../internal/types.h"
#include "../internal/string.h"


//------------------------------------------------
#define ACTION_END					"end"
//------------------------------------------------
#define NODE_ROOT					"root"
#define NODE_GLOBALS				"globals"
#define NODE_STATE					"state"
#define NODE_STATES					"states"
#define NODE_ACTION					"action"
#define NODE_ACTIONS				"actions"
#define NODE_PATTERN				"pattern"
#define NODE_PATTERNS				"patterns"
#define NODE_DISPLAY				"display"
#define NODE_DISPLAYS				"displays"
#define NODE_RECEIPT				"receipt"
#define NODE_RECEIPTS				"receipts"
#define NODE_LANG					"lang"
#define NODE_STORAGE				"storage"
#define NODE_SRC					"src"
#define NODE_DST					"dst"
#define NODE_MENUITEM				"menu_item"
#define NODE_MENUPROPS				"menu_props"
#define NODE_INPUTPROPS				"input_props"
#define NODE_STATICPROPS			"static_props"
#define NODE_CARDREADERPROPS		"cardreader_props"
#define NODE_PINPADPROPS			"pinpad_props"
#define NODE_TRANSACTIONPROPS		"transaction_props"
#define NODE_PRINTERPROPS			"printer_props"
#define NODE_CONFIRMPROPS			"confirm_props"
#define NODE_FUNCPROPS				"func_props"
//------------------------------------------------
#define ATR_ENTRYPOINT				"entry_point"
#define ATR_NAME					"name"
#define ATR_TYPE					"type"
#define ATR_DISPLAY					"display"
#define ATR_VALUE					"value"
#define ATR_ID						"id"
#define ATR_MTID					"mtid"
#define ATR_PROCCODE				"proc_code"
//---storage--------------------------------------
#define ATR_STRG_VAR				"var"
#define ATR_STRG_FILTER				"filter"
#define ATR_STRG_TARGET				"target"
#define ATR_STRG_TRNANSACTION		"transaction"
#define ATR_STRG_GLOBAL				"global"
//---filters--------------------------------------
#define ATR_FILTER_DISP_SUM			"disp_sum"
#define ATR_FILTER_DISP_SUM_BAL		"disp_sum_bal"
#define ATR_FILTER_TRN_DATE			"trn_date"
#define ATR_FILTER_BILL_DATE		"bill_date"
#define ATR_FILTER_TRN_TIME			"trn_time"
#define ATR_FILTER_TERM_DATE		"term_date"
#define ATR_FILTER_TERM_TIME		"term_time"
#define ATR_FILTER_ERR_DESC			"err_desc"
#define ATR_FILTER_HIDE_PASS		"hide_pass"
#define ATR_FILTER_HIDE_CARD_NUM	"hide_card_num"
#define ATR_FILTER_DATE				"print date"
#define ATR_FILTER_IP				"ip"
#define ATR_FILTER_ARAB					"arab"
//---state----------------------------------------
#define ATR_STATE_ACTION			"action"
#define ATR_STATE_ONOK				"on_ok"
#define ATR_STATE_ONFAIL			"on_fail"
//---action types---------------------------------
#define ATR_ACTYPE_INPUT			"input"
#define ATR_ACTYPE_MENU				"menu"
#define ATR_ACTYPE_STATIC			"static"
#define ATR_ACTYPE_TRANSACTION		"transaction"
#define ATR_ACTYPE_CARDREADER		"cardreader"
#define ATR_ACTYPE_PINPAD			"pinpad"
#define ATR_ACTYPE_PRINTER			"printer"
#define ATR_ACTYPE_CONFIRM			"confirm"
#define ATR_ACTYPE_FUNC				"func"
#define ATR_ACTYPE_COPY				"copy"
//---input----------------------------------------
#define ATR_INPUT_FORMAT			"format"
#define ATR_INPUT_MIN_LEN			"min_len"
#define ATR_INPUT_MAX_LEN			"max_len"
#define ATR_INPUT_EDIT				"edit"
//---input format---------------------------------
#define ATR_IF_DIGITS				"digits"
#define ATR_IF_LETTERS				"letters"
//---transaction----------------------------------
#define ATR_TRN_AUTOCANCEL			"autocancel"
#define ATR_TRN_JOURNAL				"journal"
#define ATR_TRN_INCR_CNTS			"incr_cnts"
#define ATR_TRN_SHOW_RESULT			"show_result"
//---transaction types----------------------------
#define ATR_TRNTYPE_SEND			"send"
#define ATR_TRNTYPE_RECEIVE			"receive"
#define ATR_TRNTYPE_BACKUP			"backup"
//---printer--------------------------------------
#define ATR_PRINTER_RECEIPT			"receipt"
#define ATR_PRINTER_COUNT			"count"
//---dynamic menu---------------------------------
#define ATR_MENUTYPE_ENC			"enc"
#define ATR_MENUTYPE_MAC			"mac"
//---function-------------------------------------
#define ATR_FNTYPE_PIN_NEED			"is_pin_need"
#define ATR_FNTYPE_BAN_NEED			"is_ban_need"
#define ATR_FNTYPE_BRN_NEED			"is_brn_need"
#define ATR_FNTYPE_CHANGE_TPK		"new_tpk"
#define ATR_FNTYPE_BOOL				"bool"
#define ATR_FNTYPE_CALC_SUM			"calc_sum"
#define ATR_FNTYPE_CALC_DIFF		"calc_diff"
#define ATR_FNTYPE_CALC_MUL			"calc_mul"
#define ATR_FNTYPE_EQUAL			"equal"
#define ATR_FNTYPE_IS_LOGGED_IN		"is_logged_in"
#define ATR_FNTYPE_LOGIN_SAVE_REC	"login_save_rec"
#define ATR_FNTYPE_LOGOUT			"logout"
#define ATR_FNTYPE_KEY_EXISTS		"key_exists"
#define ATR_FNTYPE_MACKEY_EXISTS	"mackey_exists"
#define ATR_FNTYPE_KEY_SAVE			"key_save"
#define ATR_FNTYPE_MACKEY_SAVE		"mackey_save"
#define ATR_FNTYPE_TERM_ID_SAVE		"term_id_save"
#define	ATR_FNTYPE_TERM_ID_GET		"term_id_get"
#define ATR_FNTYPE_CHECK_CARD		"check_card"
#define ATR_FNTYPE_CALC_FEE			"calc_fee"
#define ATR_FNTYPE_TERM_LOC_SAVE	"term_loc_save"
#define	ATR_FNTYPE_TERM_LOC_GET		"term_loc_get"
#define ATR_FNTYPE_TERM_DATE_SAVE	"set_date"
#define	ATR_FNTYPE_TERM_DATE_GET	"get_date"
#define	ATR_FNTYPE_TERM_TIME_GET	"get_time"
#define	ATR_FNTYPE_LCD				"lcd"
#define	ATR_FNTYPE_MAC				"use_mac"
#define	ATR_FNTYPE_ENC				"use_enc"

//---cardreader-----------------------------------
#define ATR_CR_CONTACTLESS			"contactless"
#define ATR_CR_EMV					"emv"
#define ATR_CR_TYPE					"type_card"
//---pinpad---------------------------------------
#define ATR_PP_LINE					"line"
//------------------------------------------------

class IVariable;
class CStorage;
class IAction;
struct SVariable;
struct SCopyPair;
struct SMenuItem;
struct SBaseActionInfo;
struct SState;
struct SXmlActInfoEnt;
typedef std::vector<SVariable>				tVecVars;
typedef std::vector<SCopyPair>				tVecCopyPairs;
typedef std::vector<SMenuItem>				tVecMenuItems;
typedef std::vector<IAction*>				tVecActions;
typedef std::map<string, SBaseActionInfo*>	tMapActList;
typedef std::map<string, SState*>			tMapStateList;
typedef std::map<string, SXmlActInfoEnt*>	tMapActionsInfo;
//------------------------------------------------
enum INPUT_FORMAT
{
	IF_DIGITS,
	IF_LETTERS,
};
//------------------------------------------------
enum ACTION_TYPE
{
	AT_UNKNOWN,
	AT_INPUT,
	AT_MENU,
	AT_STATIC,
	AT_CARDREADER,
	AT_PINPAD,
	AT_TRANSACTION,
	AT_PRINTER,
	AT_CONFIRM,
	AT_FUNC,
	AT_COPY,
};
//------------------------------------------------
enum ACTION_STATE
{
	AS_MALFUNC,
	AS_RUNING,
	AS_COMPLETED,
	AS_FAILED,
};
//------------------------------------------------
enum FUNC_TYPE
{
	FNT_UNKNOWN,
	FNT_BOOL,
	FNT_EQUAL,
	FNT_TERM_ID_SAVE,
	FNT_TERM_ID_GET,
	FNT_TERM_DATE_SAVE,
	FNT_TERM_DATE_GET, 
	FNT_TERM_TIME_GET, 
	FNT_CALC_FEE,
	FNT_IP_GET,
	FNT_PORT_GET,
	FNT_HOST_SET,
	FNT_BATCH_GET,
	FNT_BATCH_SET,
	FNT_MANAGE,
	FNT_CAN_CON,
	FNT_CONFIG,
	FNT_PARAM_CHANGE,
	FNT_LCD,
	FNT_MAC,
	FNT_ENC,
};
//------------------------------------------------
enum STORAGE_TYPE
{
	ST_UNKNOWN,
	ST_GLOBAL,
	ST_TRANSACTION_SEND,
	ST_TRANSACTION_RECEIVE,
	ST_TRANSACTION_BACKUP,
};
//------------------------------------------------
enum FILTER_TYPE
{
	FLT_NONE,
	FLT_DISPLAY_SUM,
	FLT_DISPLAY_SUM_BAL,
	FLT_IP,
	FLT_TRN_DATE,
	FLT_BILL_DATE,
	FLT_TRN_TIME,
	FLT_TERM_DATE,
	FLT_TERM_TIME,
	FLT_HIDE_PASS,
	FLT_HIDE_CARD_NUM,
	FLT_ERR_DESC,
	FLT_DATE,
	FLT_ARAB,
};
//------------------------------------------------
enum TARGET_TYPE
{
	TT_SEND,
	TT_RECEIVE,
	TT_BACKUP,
	TT_CNT,
};
//------------------------------------------------
enum MENU_TYPE
{
	MT_UNKNOWN,
	MT_MAC,
	MT_ENC,
};
//------------------------------------------------
struct SVariable
{
	// Функции определены в CStorage.cpp
	void store(const string& strData);
	string get() const;
	IVariable*	pVariable;
	FILTER_TYPE	eFilter;
};
//------------------------------------------------
struct SCopyPair
{
	SVariable	src_var;
	tVecVars	dst_vars;
};
//------------------------------------------------
struct SMenuItem
{
	string	display;
	string	val;
};
//------------------------------------------------
struct SState
{
	string		action;
	tVecStrings	vec_on_ok;
	string		on_fail;
};
//------------------------------------------------
struct SBaseActionInfo
{
	ACTION_TYPE	type;

	virtual ~SBaseActionInfo() {}
};
//------------------------------------------------
struct SInputInfo : public SBaseActionInfo
{
	string			display;
	INPUT_FORMAT	format;
	WORD			min_len;
	WORD			max_len;
	tVecVars		dst_vars;
	tVecVars		src_vars;
	bool			edit;
};
//------------------------------------------------
struct SMenuInfo : public SBaseActionInfo
{
	MENU_TYPE		type;
	tVecMenuItems	items;
	tVecVars		dst_vars;
};
//------------------------------------------------
struct SStaticInfo : public SBaseActionInfo
{
	string	display;
};
//------------------------------------------------
struct SCardReaderInfo : public SBaseActionInfo
{
	bool	contactless;
	bool	emv;
	string  type;
};
//------------------------------------------------
struct SPinPadInfo : public SBaseActionInfo
{
	string		display;
	DWORD		line;
	tVecVars	dst_vars;
};
//------------------------------------------------
struct SConfirmInfo : public SBaseActionInfo
{
	string	display;
};
//------------------------------------------------
struct STransactionInfo : public SBaseActionInfo
{
	string	mtid;
	string	proccode;
	bool	autocancel;
	bool	journal;
	bool	incr_cnts;
	bool	show_result;
};
//------------------------------------------------
struct SPrinterInfo : public SBaseActionInfo
{
	string	receipt;
	WORD	count;
};
//------------------------------------------------
struct SFuncInfo : public SBaseActionInfo
{
	FUNC_TYPE	func_type;
	tVecVars	src_vars;
	tVecVars	dst_vars;
};
//------------------------------------------------
struct SCopyInfo : public SBaseActionInfo
{
	tVecCopyPairs	copy_pairs;
};
//------------------------------------------------
struct SDisplay
{
	string	strLang[5];
};
//------------------------------------------------
struct SReceipt
{
	string	strLang[5];
};
//------------------------------------------------
class IAction
{
public:
	virtual bool			init(SBaseActionInfo* pActionInfo)	= 0;
	virtual ACTION_STATE	run()								= 0;
	virtual void			updateDisplay()						{ }

	virtual ~IAction() {}
};
//------------------------------------------------

#endif
