#include "CKKM.h"
#include "../fs/CFile.h"
#include "../extern.h"

#define FILE_KKM		"kkm.dat"

CKKM			g_oKKM;

void CKKM::initialize()
{
	CFile oFile;
	if (oFile.open(FILE_KKM, O_RDONLY))
	{
		oFile.read((BYTE*) &m_stcCounter, sizeof(m_stcCounter));
		oFile.close();
	} else
	{
		m_stcCounter.dwStan = 1;
		syncCounters();
	}

	updateCharBuffer();
}

char* CKKM::incrStan()
{
	if (m_stcCounter.dwStan >= 999999)
		m_stcCounter.dwStan = 1;
	else
		++m_stcCounter.dwStan;	

	updateCharBuffer();
	DBGLine("Stan: %s", m_pStan);
	return m_pStan;
}

void CKKM::updateCharBuffer()
{
	char* pBuf = new char[LENGTH_STAN + 1]; AUTO_DELETE_ARR(char, pBuf);
	memset(pBuf, 0, LENGTH_STAN + 1);
	sprintf(pBuf, "%u", m_stcCounter.dwStan);
	memset(m_pStan, '0', sizeof(m_pStan));
	strcpy(m_pStan + LENGTH_STAN - strlen(pBuf), pBuf);

	// Ref No
	sprintf(m_pReferenceNo, "%06d000000", m_stcCounter.dwStan);
}

void CKKM::syncCounters()
{
	CFile oFile;
	oFile.open(FILE_KKM, O_CREAT | O_WRONLY);
	oFile.write((BYTE*) &m_stcCounter, sizeof(m_stcCounter));
	oFile.close();
	CFile::sync();
}
