//---------------------------------------------------------------------------
/****************************************************************************
* История изменений:
* $Log: CReaderICC.cpp,v $
* Revision 1.3  2010/06/09 06:04:41  rakhmatulin
* no message
*
*
***************************************************************************/
//---------------------------------------------------------------------------

#include "CReaderICC.h"
#include "../internal/tools.h"

#include <infra/config.h>
#include <infra/error.h>
#include <hal/usip/hal_apm.h>
#include <hal/usip/hal_scs.h>
#include <devs/sc_at83c26.h>
#include <libemv.h>

//---------------------------------------------------------------------------
RCICC CReaderICC::initialize()
{
	//==============================================================================================
	// Инициализация механического детектора карты

	DBGLine("Initialize ICC reader slot");

	EMV_SLOT_CONFIG stcSlot;

	stcSlot.clock    = HAL_SCS_CLOCK_3_69MHz;
	stcSlot.power    = HAL_SCS_POWER_5V;
	stcSlot.protocol = HAL_SCS_PROTOCOL_T0;

	stcSlot.devio    = (HAL_SCS_DRIVER_IO*)&at83c26_io;

	stcSlot.ucSCIB   = EMV_SCS_SCIB_1;

	stcSlot.mode     = EMV_MODE_MANAGE_PPS | EMV_MODE_T1_IFSD;
	stcSlot.timernum = 3;

	int nRes = emv_init(hal_scs_dev1, &stcSlot);

	if (nRes != ERR_NO)
	{
		DBGLine("Error in initialize ICC reader slot")
		return RCICC_ERROR_IFD;
	}

	return RCICC_OK;
}

//---------------------------------------------------------------------------
bool CReaderICC::cardInserted()
{
	//==============================================================================================
	// Ожидание присутствия карты, основано на механическом переключателе, т.е. можно вставить картон
	// и индикация сработает
	int nICCState;
	int nRes = emv_get_status(hal_scs_dev1, &nICCState);

	if (nRes != ERR_NO)
	{
		DBGLine("Error in emv_get_status");
		return false;
	}

	if ((nICCState & HAL_SCS_STATUS_CARD_NOT_INSERTED) == 0)
	{
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
RCICC CReaderICC::powerUp()
{
	//==============================================================================================
	// Карта присутствует в ридере, подаем напряжение, делаем сброс
	BYTE	ucATR[256];
	DWORD	nATRLength = 256;

	DBGLine("ICC power up");
	int nRes = emv_power_up(hal_scs_dev1, ucATR, &nATRLength);

	if (nRes == ERR_NO)
	{
		DBGBuf("ATR", ucATR, nATRLength);
	} else
	{
		DBGLine("Error ICC power up");
		return RCICC_ERROR_IFD;
	}

	return RCICC_OK;
}

//---------------------------------------------------------------------------
RCICC CReaderICC::powerDown()
{
	DBGLine("ICC power down");
	int nRes = emv_power_down(hal_scs_dev1);
	if (nRes != ERR_NO)
	{
		DBGLine("Error ICC power down");
		return RCICC_ERROR_IFD;
	}

	return RCICC_OK;
}

//---------------------------------------------------------------------------
// NOTE: как будет известна дальнейшая работа с картами - преобразовать эту функцию
RCICC CReaderICC::readBasicInfo(SPublicInfo *pStcPublicInfo, SCardHolderInfo *pStcCardHolderInfo)
{
	BYTE ucAnswer[256];
	DWORD nAnwerLength;
	memset(pStcPublicInfo, 0, sizeof(SPublicInfo));
	memset(pStcCardHolderInfo, 0, sizeof(SCardHolderInfo));

	//==============================================================================================
	// Сначала выполняем комманду SELECT для файла EF 05

	nAnwerLength = 0xFF;
	BYTE cmdSelectEF05[] =	{
		0x00,			// CLA
		0xA4,			// INS
		0x00,			// P1
		0x00,			// P2
		0x02,			// Lc
		0xEF, 0x05,		// FI
		0x01,			// Le, "If P2 is set to '00', then the Le field shall be set to '01'" - толку нет, FCI все равно не приходит
	};

	DBGBuf("C-APDU", cmdSelectEF05, sizeof(cmdSelectEF05));
	int nRes = emv_transmit(hal_scs_dev1, cmdSelectEF05, sizeof(cmdSelectEF05), ucAnswer, &nAnwerLength);
	if (nRes != ERR_NO)
	{
		DBGLine("Error ICC transmit");
		return RCICC_ERROR_IFD;
	}
	DBGBuf("R-APDU", ucAnswer, nAnwerLength);

	// Проверяем SW1 SW2
	if (!(ucAnswer[nAnwerLength - 2] == 0x90 && ucAnswer[nAnwerLength - 1] == 0x00))
	{
		DBGLine("Error in SELECT");
		return RCICC_ERROR_READ_FILE;
	}

	
	//==============================================================================================
	// Выполняем комманду READ BINARY для EF 05

	nAnwerLength = 0xFF;
	BYTE cmdReadEF05[] =	{
		0x00,			// CLA
		0xB0,			// INS
		0x00,			// P1
		0x00,			// P2
		0x00,			// Le
	};

	DBGBuf("C-APDU", cmdReadEF05, sizeof(cmdReadEF05));
	nRes = emv_transmit(hal_scs_dev1, cmdReadEF05, sizeof(cmdReadEF05), ucAnswer, &nAnwerLength);
	if (nRes != ERR_NO)
	{
		DBGLine("Error ICC transmit");
		return RCICC_ERROR_IFD;
	}
	DBGBuf("R-APDU", ucAnswer, nAnwerLength);

	// Проверяем SW1 SW2
	if (!(ucAnswer[nAnwerLength - 2] == 0x90 && ucAnswer[nAnwerLength - 1] == 0x00))
	{
		DBGLine("Error in READ BINARY");
		return RCICC_ERROR_READ_FILE;
	}

	if (nAnwerLength != 38 + 2)
	{
		DBGLine("Error in READ BINARY, wrong answer length");
		return RCICC_ERROR_READ_FILE;
	}

	//==============================================================================================
	// Преобразуем ответ в нужный формат
	memcpy(pStcPublicInfo->ucCardID, ucAnswer + 0, 8);

	char cCardType;
	memcpy(&cCardType, ucAnswer + 8, 1);
	switch (cCardType)
	{
	case '1':
		{
			pStcPublicInfo->eCardType = DCARD_CITYZEN;
			break;
		}
	case '2':
		{
			pStcPublicInfo->eCardType = DCARD_GROCER;
			break;
		}
	case '3':
		{
			pStcPublicInfo->eCardType = DCARD_TELLER;
			break;
		}
	default:
		{
			DBGLine("Unkown card type");
			break;
		}
	}

	memcpy(&pStcPublicInfo->cVersion, ucAnswer + 9, 1);
	memcpy(pStcPublicInfo->ucInitNumber, ucAnswer + 10, 8);

	BYTE ucDate[4];
	memcpy(ucDate, ucAnswer + 18, 4);
	BCDLeft2str(ucDate, (BYTE*) pStcPublicInfo->strDateIssuing, 8);
	memcpy(ucDate, ucAnswer + 22, 4);
	BCDLeft2str(ucDate, (BYTE*) pStcPublicInfo->strDateExpired, 8);

	memcpy(pStcPublicInfo->strCardNumber, ucAnswer + 26, 12);


	//==============================================================================================
	// Выполняем комманду SELECT для файла EF 06

	nAnwerLength = 0xFF;
	BYTE cmdSelectEF06[] =	{
		0x00,			// CLA
		0xA4,			// INS
		0x00,			// P1
		0x00,			// P2
		0x02,			// Lc
		0xEF, 0x06,		// FI
		0x01,			// Le, "If P2 is set to '00', then the Le field shall be set to '01'" - толку нет, FCI все равно не приходит
	};

	DBGBuf("C-APDU", cmdSelectEF06, sizeof(cmdSelectEF06));
	nRes = emv_transmit(hal_scs_dev1, cmdSelectEF06, sizeof(cmdSelectEF06), ucAnswer, &nAnwerLength);
	if (nRes != ERR_NO)
	{
		DBGLine("Error ICC transmit");
		return RCICC_ERROR_IFD;
	}
	DBGBuf("R-APDU", ucAnswer, nAnwerLength);

	// Проверяем SW1 SW2
	if (!(ucAnswer[nAnwerLength - 2] == 0x90 && ucAnswer[nAnwerLength - 1] == 0x00))
	{
		DBGLine("Error in SELECT");
		return RCICC_ERROR_READ_FILE;
	}


	//==============================================================================================
	// Выполняем комманду READ BINARY для EF 06

	nAnwerLength = 0xFF;
	BYTE cmdReadEF06[] =	{
		0x00,			// CLA
		0xB0,			// INS
		0x00,			// P1
		0x00,			// P2
		0x00,			// Le
	};

	DBGBuf("C-APDU", cmdReadEF06, sizeof(cmdReadEF06));
	nRes = emv_transmit(hal_scs_dev1, cmdReadEF06, sizeof(cmdReadEF06), ucAnswer, &nAnwerLength);
	if (nRes != ERR_NO)
	{
		DBGLine("Error ICC transmit");
		return RCICC_ERROR_IFD;
	}
	DBGBuf("R-APDU", ucAnswer, nAnwerLength);

	// Проверяем SW1 SW2
	if (!(ucAnswer[nAnwerLength - 2] == 0x90 && ucAnswer[nAnwerLength - 1] == 0x00))
	{
		DBGLine("Error in READ BINARY");
		return RCICC_ERROR_READ_FILE;
	}

	if (nAnwerLength != 47 + 2)
	{
		DBGLine("Error in READ BINARY, wrong answer length");
		return RCICC_ERROR_READ_FILE;
	}

	//==============================================================================================
	// Преобразуем ответ в нужный формат
	memcpy(pStcCardHolderInfo->strName, ucAnswer + 0, 30);
	memcpy(pStcCardHolderInfo->strIDNumber, ucAnswer + 30, 16);
	memcpy(&pStcCardHolderInfo->cIDType, ucAnswer + 46, 1);




	//==============================================================================================
	// Выполняем комманду SELECT DF

// 	nAnwerLength = 0xFF;
// 	BYTE cmdSelectDF[] =	{
// 		0x00,			// CLA
// 		0xA4,			// INS
// 		0x00,			// P1
// 		0x00,			// P2
// 		0x02,			// Lc
// 		0x3F, 0x00,		// AID
// 		0x01,			// Le, "If P2 is set to '00', then the Le field shall be set to '01'" - толку нет, FCI все равно не приходит
// 	};
// 
// 	DBGBuf("C-APDU", cmdSelectDF, sizeof(cmdSelectDF));
// 	nRes = emv_transmit(hal_scs_dev1, cmdSelectDF, sizeof(cmdSelectDF), ucAnswer, &nAnwerLength);
// 	if (nRes != ERR_NO)
// 	{
// 		DBGLine("Error ICC transmit");
// 		return RCICC_ERROR_IFD;
// 	}
// 	DBGBuf("R-APDU", ucAnswer, nAnwerLength);
// 
// 	// Проверяем SW1 SW2
// 	if (!(ucAnswer[nAnwerLength - 2] == 0x90 && ucAnswer[nAnwerLength - 1] == 0x00))
// 	{
// 		DBGLine("Error in SELECT");
// 		return RCICC_ERROR_READ_FILE;
// 	}


// 	//==============================================================================================
// 	// Выполняем комманду SELECT AID
// 
// 	nAnwerLength = 0xFF;
// 	BYTE cmdSelectAID[] =	{
// 		0x00,			// CLA
// 		0xA4,			// INS
// 		0x00,			// P1
// 		0x00,			// P2
// 		0x02,			// Lc
// 		0xEF, 0x17,		// AID
// 		0x01,			// Le, "If P2 is set to '00', then the Le field shall be set to '01'" - толку нет, FCI все равно не приходит
// 	};
// 
// 	DBGBuf("C-APDU", cmdSelectAID, sizeof(cmdSelectAID));
// 	nRes = emv_transmit(hal_scs_dev1, cmdSelectAID, sizeof(cmdSelectAID), ucAnswer, &nAnwerLength);
// 	if (nRes != ERR_NO)
// 	{
// 		DBGLine("Error ICC transmit");
// 		return RCICC_ERROR_IFD;
// 	}
// 	DBGBuf("R-APDU", ucAnswer, nAnwerLength);
// 
// 	// Проверяем SW1 SW2
// 	if (!(ucAnswer[nAnwerLength - 2] == 0x90 && ucAnswer[nAnwerLength - 1] == 0x00))
// 	{
// 		DBGLine("Error in SELECT");
// 		return RCICC_ERROR_READ_FILE;
// 	}



// 	//==============================================================================================
// 	// Выполняем комманду READ BINARY для EF 15
// 
// 	nAnwerLength = 0xFF;
// 	BYTE cmdReadEF15[] =	{
// 		0x00,			// CLA
// 		0xB0,			// INS
// 		0x00,			// P1
// 		0x00,			// P2
// 		0x00,			// Le
// 	};
// 
// 	DBGBuf("C-APDU", cmdReadEF15, sizeof(cmdReadEF15));
// 	nRes = emv_transmit(hal_scs_dev1, cmdReadEF15, sizeof(cmdReadEF15), ucAnswer, &nAnwerLength);
// 	if (nRes != ERR_NO)
// 	{
// 		DBGLine("Error ICC transmit");
// 		return RCICC_ERROR_IFD;
// 	}
// 	DBGBuf("R-APDU", ucAnswer, nAnwerLength);
// 
// 	// Проверяем SW1 SW2
// 	if (!(ucAnswer[nAnwerLength - 2] == 0x90 && ucAnswer[nAnwerLength - 1] == 0x00))
// 	{
// 		DBGLine("Error in READ BINARY");
// 		return RCICC_ERROR_READ_FILE;
// 	}



// 	//==============================================================================================
// 	// PIN Test
// 	nAnwerLength = 0xFF;
// 	BYTE cmdTestPin[] =	{
// 		0x00,			// CLA
// 		0x20,			// INS
// 		0x00,			// P1
// 		0x00,			// P2
// 		0x03,			// Lc
// 		0x00,
// 		0x79,
// 		0x10,
// 		0x00,			// Le
// 	};
// 
// 	DBGBuf("C-APDU", cmdTestPin, sizeof(cmdTestPin));
// 	nRes = emv_transmit(hal_scs_dev1, cmdTestPin, sizeof(cmdTestPin), ucAnswer, &nAnwerLength);
// 	if (nRes != ERR_NO)
// 	{
// 		DBGLine("Error ICC transmit");
// 		return RCICC_ERROR_IFD;
// 	}
// 	DBGBuf("R-APDU", ucAnswer, nAnwerLength);


	return RCICC_OK;
}
