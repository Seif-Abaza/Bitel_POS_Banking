//---------------------------------------------------------------------------
/****************************************************************************
* История изменений:
* $Log: common.h,v $
* Revision 1.3  2010/06/09 06:04:42  rakhmatulin
* no message
*
*
***************************************************************************/
//---------------------------------------------------------------------------

#include "../internal/types.h"

//==============================================================================================
// Result Code ICC
// Коды ошибок для работы с ICC картами
enum RCICC
{
	RCICC_OK,					// Операция вылолнена успешно
	RCICC_ERROR_IFD,			// Ошибка коммуникации между ICC и терминалом, либо ридер отказывается работать
	RCICC_ERROR_READ_FILE,		// Файл на карте отсутствует либо содержание не соотвествует требованиям
};

//==============================================================================================
// Daman Card Type
enum DCARD_TYPE
{
	DCARD_CITYZEN,
	DCARD_GROCER,
	DCARD_TELLER,
};

//==============================================================================================
// Public Information, одинаково для каждого типа карты
#define	LEN_PI_CARD_ID		8
#define LEN_PI_INIT_NUMBER	8
#define LEN_PI_DATE			8
#define LEN_PI_CARD_NUMBER	12
struct SPublicInfo
{
	BYTE		ucCardID[LEN_PI_CARD_ID];
	DCARD_TYPE	eCardType;
	char		cVersion;								// Символьно, например '1'
	BYTE		ucInitNumber[LEN_PI_INIT_NUMBER];
	char		strDateIssuing[LEN_PI_DATE + 1];		// Дата эмиссии карты, строка YYYYMMDD, например "20100112"
	char		strDateExpired[LEN_PI_DATE + 1];		// Дата окончания срока действия карты, формат аналогичен
	char		strCardNumber[LEN_PI_CARD_NUMBER + 1];	// Номер карты, строка
};

//==============================================================================================
// Card Holder Information, одинаково для каждого типа карты
#define LEN_CHI_NAME		30
#define LEN_CHI_ID_NUMBER	16
struct SCardHolderInfo
{
	char		strName[LEN_CHI_NAME + 1];				// Имя держателя карты
	char		strIDNumber[LEN_CHI_ID_NUMBER + 1];		// ID держателя
	char		cIDType;								// Тип ID
};
