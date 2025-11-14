//---------------------------------------------------------------------------
/****************************************************************************
* Пример использования модуля:
*
* 	CReaderICC oReaderICC;
*	RCICC eRC = oReaderICC.initialize();
*
*	if (eRC != RCICC_OK)
*	{
*		// Произошла ошибка, result code лучше проверять в каждой функции
*	}
*	while (true)
*	{
*		// Ждем пока не вставят карту
*		while (!oReaderICC.cardInserted())
*			;
*
*		// Карту вставили, включаем
*		eRC = oReaderICC.powerUp();
*		// Проверить result code..
*
*		SPublicInfo		stcPublicInfo;
*		SCardHolderInfo	stcCardHolderInfo;
*		eRC = oReaderICC.readBasicInfo(&stcPublicInfo, &stcCardHolderInfo);
*
*		DBGLine("eCardType: %d, cVersion: %c, strDateIssuing: %s, strDateExpired: %s, strCardNumber: %s",
*		stcPublicInfo.eCardType, stcPublicInfo.cVersion, stcPublicInfo.strDateIssuing, stcPublicInfo.strDateExpired, stcPublicInfo.strCardNumber);
*
*		DBGLine("strName: %s, strIDNumber: %s, cIDType: %c",
*		stcCardHolderInfo.strName, stcCardHolderInfo.strIDNumber, stcCardHolderInfo.cIDType);
*
*		eRC = oReaderICC.powerDown();
*
*		// Висим пока карта вставлена, нужно чтобы вытащил
*		while (oReaderICC.cardInserted())
*			;
*	}
*
* История изменений:
* $Log: CReaderICC.h,v $
* Revision 1.3  2010/06/09 06:04:41  rakhmatulin
* no message
*
*
***************************************************************************/
//---------------------------------------------------------------------------

#ifndef __CREADERICC_H
#define __CREADERICC_H

#include "common.h"


class CReaderICC
{
public:
	// Перед использованием требуется обязательная инициализация
	RCICC	initialize();

	// Требует постоянного вызова, возвращает true если в ридер вставлена карта
	bool	cardInserted();

	// Подает напряжение на карту, после него сигнал RST, получает ответ на сброс ATR
	RCICC	powerUp();

	// Убирает напряжение с карты
	RCICC	powerDown();

	// Читает основные данные карты, присутствует для всех типов карт
	RCICC	readBasicInfo(SPublicInfo* pStcPublicInfo, SCardHolderInfo* pStcCardHolderInfo);

};

#endif
