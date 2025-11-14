
//**************************************************************************************************
//		Эмулятор ККМ-а
//**************************************************************************************************

#ifndef __CKKM_H
#define __CKKM_H

#include "../internal/tools.h"
#include "common.h"
#include "../internal/CParam.h"
#include "../internal/types.h"

class CKKM
{
public:
	struct SCounter
	{
		DWORD dwStan;
	};

public:
	void	initialize();

	//	Увеличивает номер транзакции
	char*	incrStan();	
	char*	getStan() { return m_pStan; }
	char*	getReferenceNo() { return m_pReferenceNo; }
	void	syncCounters();

private:
	void	updateCharBuffer();

	char	m_pStan[LENGTH_STAN + 1];
	char	m_pReferenceNo[LENGTH_RRN + 1];
	SCounter m_stcCounter;							// Номер транзакции и номер операции (чека)
};

extern CKKM	g_oKKM;

#endif
