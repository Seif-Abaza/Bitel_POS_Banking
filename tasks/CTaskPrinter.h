
//**************************************************************************************************
//		Класс для работы с принтером
//**************************************************************************************************

#ifndef __CTASK_PRINTER_H
#define __CTASK_PRINTER_H

#include "../internal/types.h"
#include "../internal/printer_common.h"

#define BOTTOM_SPACE	6			// количество пустых строк, вставляемых в конце чека

class	CTaskPrinter;
extern	CTaskPrinter*	g_pPrinter;

class CTaskPrinter
{
public:
	CTaskPrinter()	{ g_pPrinter = this; }
	void	init();
	int		run();

	//	Напечатать буфер
	// nCopyes - количество копий
	//	Независимо от протокола
	void	printBuf(const BYTE* pBuf, int nCopyes = 1);
	bool    printDone();

private:
	//	Обработка паттерна XML (см. .cpp)
	void	_procPatternXML(char* pBuf, int nCnt, int nPatt, char align);
	//	Обработка паттерна  (см. .cpp)
	void	_procPattern(char* pBuf, int nCnt, int nPatt, char align);
	//	Вставляет строку field в качестве паттерна
	void	_insertPattern(char* pBuf, int nCnt, char align, const char* field);
	//	Обработка буфера
	void	_makeBuf(const BYTE* pBuf);
	//	Win2Dos для русских букв
	void	_trnslt2rus(char& tok);
	//	Удаление строки (используется для необязательных строк)
	void	_delCurrLine();
	//	Начать печать
	void	_startPrinting();
	//	Остановить печать
	void	_stopPrinting();

private:
	int			m_nLinesCnt;
	int			m_nCurrLines;
	int			m_nCurrPos;
	WORD		m_wCurrCopy;
	int			m_nCopyesCnt;
	int			m_nBolt;
};

#endif
