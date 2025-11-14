#include "../extern.h"


#include "CFileSystem.h"
#include "fs_flex.h"
#include "../internal/tools.h"



CFileSystem::CFileSystem()
{
	init();  
}
 
CFileSystem::~CFileSystem()
{
	clearHandles();  
}

void CFileSystem::format()
{
	SFSBoot	stcBoot;
	memset(&stcBoot, 0, sizeof(SFSBoot));
	memcpy(stcBoot.strMagicWord, FS_WRONG_STRING, strlen(FS_WRONG_STRING));
	stcBoot.nClustersCount = FS_CLUSTERS_COUNT;
	stcBoot.nClusterSize = FS_CLUSTER_SIZE;
	stcBoot.nMaxFiles = FS_FILES_COUNT;

	stcBoot.pFiles = (SFSFileRecord *)(FS_BEGIN + sizeof(SFSBoot));
	stcBoot.pClusters = (WORD *)((DWORD)stcBoot.pFiles + (DWORD)((DWORD)stcBoot.nMaxFiles * sizeof(SFSFileRecord)));
	// Ant, 13.05.09
	stcBoot.pData =  (BYTE *) FS_DATA_BEGIN; // (BYTE *)((DWORD)stcBoot.pClusters + ((DWORD)stcBoot.nClustersCount*sizeof(WORD)));

	// Ant, 12.05.09
	char pZeroBuffer[sizeof(SFSBoot)] = {0};
	FLASH_WRITE((char *)FS_BACKUP_BEGIN, sizeof(SFSBoot), pZeroBuffer);

	FLASH_WRITE((char *)FS_BEGIN, sizeof(SFSBoot), (char *)&stcBoot);


	/*	Lprint(3, 0, stcBoot.strMagicWord);*/

	/*
	char tmp[100];
	sprintf(tmp, "%X", stcBoot.pClusters);
	Lprint(1, 0, tmp);
	*/




	/*
	SFSFileRecord stcFileRecord;
	memset(&stcFileRecord, 0, sizeof(SFSFileRecord));
	stcFileRecord.nCluster = FS_FREE_CLUSTER;
	*/
	SFSFileRecord *pMemFiles = stcBoot.pFiles;
	int i;
	long nSize = sizeof(SFSFileRecord) * FS_FILES_COUNT;
	SFSFileRecord	arrFileRecords[ FS_FILES_COUNT ];
	memset(arrFileRecords, 0,  nSize);
	for (i = 0; i < stcBoot.nMaxFiles; i++)
	{
		arrFileRecords[i].nCluster = FS_FREE_CLUSTER;
		/*pMemFiles++;*/
	}
	FLASH_WRITE((char *)pMemFiles, nSize, (char *)arrFileRecords);

	/*
	Lprint(0, 0, "File Records...");
	sprintf(tmp, "%d %d %d", sizeof(SFSFileRecord), sizeof(SFSBoot), sizeof(WORD));
	Lprint(0, 6, tmp);
	*/


	WORD *pMemClusters = stcBoot.pClusters;
	WORD arrClusters[ FS_CLUSTERS_COUNT ];
	/*	WORD nClusterValue = FS_FREE_CLUSTER;*/
	for (i = 0; i < stcBoot.nClustersCount; i++)
	{
		/*
		FLASH_WRITE((char *)pMemClusters, sizeof(WORD), (char *)&nClusterValue);
		pMemClusters++;
		*/
		arrClusters[i] = FS_FREE_CLUSTER;
	}
	nSize = sizeof(WORD) * FS_CLUSTERS_COUNT;
	FLASH_WRITE((char *)pMemClusters, nSize, (char *)arrClusters);


	/*
	char	pData[FS_CLUSTER_SIZE];
	memset(pData, 0, FS_CLUSTER_SIZE);
	char *pMemData = (char *)stcBoot.pData;
	for (i = 0; i < stcBoot.nClustersCount; i++)
	{
	FLASH_WRITE(pMemData, FS_CLUSTER_SIZE, pData);
	pMemData += FS_CLUSTER_SIZE;
	}
	*/


	memcpy(stcBoot.strMagicWord, FS_MAGIC_STRING, strlen(FS_MAGIC_STRING));
	FLASH_WRITE((char *)FS_BEGIN, sizeof(SFSBoot), (char *)&stcBoot);

	/*	Lprint(0, 0, "Data............");*/
}

int CFileSystem::getBusyFiles()
{
	int nResult = 0;
	SFSFileRecord	*arrFiles = m_stcBoot.pFiles;
	for (int i = 0; i < m_stcBoot.nMaxFiles; i++)
	{
		if (arrFiles[i].strFileName[0] != 0)
		{
			nResult++;
		}
	}
	return nResult;
}

int CFileSystem::getBusyClusters()
{
	int nResult = 0;
	WORD *arrClusters = m_stcBoot.pClusters;
	for (int i = 0; i < m_stcBoot.nClustersCount; i++)
	{
		if (arrClusters[i] != FS_FREE_CLUSTER)
		{
			nResult++;
		}
	}
	return nResult;
}

DWORD CFileSystem::open(const char *strFileName, WORD nMode, WORD nLock /*= 0 */)
{
	DWORD	nResult = FS_INVALID_HANDLE;
	DWORD	nLength = strlen(strFileName);
	if (nLength <= 0 || nLength > FS_MAX_PATH)
	{
		return nResult;
	}
	m_bNeedSync = true;
	if (nMode & O_CREAT)
	{
		// need to create file
		DWORD handleCreateFile = createFile(strFileName, nMode, nLock);
		if (handleCreateFile != FS_INVALID_HANDLE)
			return handleCreateFile;
	}
	SFSFileRecord	*pFiles = m_stcBoot.pFiles;
	WORD	*pClusters = m_stcBoot.pClusters;
	for (int i = 0; i < m_stcBoot.nMaxFiles; i++)
	{
		if (!strcmp(pFiles[i].strFileName, strFileName))
		{
			if (pFiles[i].attr && (nMode & O_WRONLY || nMode & O_APPEND || nMode & O_RDWR))
			{
				return nResult;
			}

			if (nLock)
			{
				SFSFileRecord stcFile;
				memcpy(&stcFile, &pFiles[i], sizeof(SFSFileRecord));
				stcFile.attr = nLock;

				//				FLASH_WRITE((char *)&pFiles[i], sizeof(SFSFileRecord), (char *)&stcFile);

				memcpy(&pFiles[i], &stcFile, sizeof(SFSFileRecord));
			}


			nResult = m_nCurHandle++;
			if (nResult == FS_INVALID_HANDLE) {	return nResult;	}
			SFSHandle	*pstcHandle = new SFSHandle;
			if (!pstcHandle)
			{
				return nResult;
			}
			memset(pstcHandle, 0, sizeof(SFSHandle));
			pstcHandle->nFSHandle = i;
			pstcHandle->nLock = nLock;
			pstcHandle->nMode = nMode;
			if (nMode & O_WRONLY)
			{
				// clear all contents of file
				WORD nCluster = pFiles[i].nCluster;
				WORD nNextCluster;
				WORD nValue = FS_FREE_CLUSTER;
				while (nCluster != FS_FREE_CLUSTER)
				{
					nNextCluster = pClusters[ nCluster ];

					//					FLASH_WRITE((char *)&pClusters[ nCluster], sizeof(WORD), (char *)&nValue );

					memcpy(&pClusters[ nCluster ], &nValue, sizeof(WORD));

					if (nNextCluster == FS_BUSY_CLUSTER) {	nCluster = FS_FREE_CLUSTER;	} 
					else {	nCluster = nNextCluster; }
				}
				SFSFileRecord stcFile;
				memcpy(&stcFile, &pFiles[i], sizeof(SFSFileRecord));
				stcFile.nCluster = FS_FREE_CLUSTER;
				stcFile.nSize = 0;

				//				FLASH_WRITE((char *)&pFiles[i], sizeof(SFSFileRecord), (char *)&stcFile);

				memcpy(&pFiles[i], &stcFile, sizeof(SFSFileRecord));



			}
			if (nMode & O_APPEND)
			{
				pstcHandle->nSeekPos = pFiles[i].nSize;
			}
			m_mapFSHandles[nResult] = pstcHandle;
			m_arrnOpenedFiles[i] = m_arrnOpenedFiles[i] + 1;
			break;
		}
	}

	return nResult;
}

void CFileSystem::clearHandles()
{
	tMapFSHandles::iterator	Iter = m_mapFSHandles.begin();
	while (Iter != m_mapFSHandles.end())
	{
		SFSHandle	*pstcHandle = Iter->second;
		Iter++;
		delete pstcHandle;
	}
	m_mapFSHandles.clear();
}

bool CFileSystem::remove(const char *strFileName )
{
	bool	bResult = false;
	SFSFileRecord	*pFiles = m_stcBoot.pFiles;
	WORD	*pClusters = m_stcBoot.pClusters;
	WORD	nValue = FS_FREE_CLUSTER;
	for (int i = 0; i < m_stcBoot.nMaxFiles; i++)
	{
		if (!strcmp(pFiles[i].strFileName, strFileName))
		{

			if (m_arrnOpenedFiles[i] > 0) {	break;	}
			m_bNeedSync = true;
			WORD nCluster = pFiles[i].nCluster;
			WORD nNextCluster;
			while (nCluster != FS_FREE_CLUSTER)
			{
				nNextCluster = pClusters[ nCluster ];

				//				FLASH_WRITE((char *)&pClusters[ nCluster], sizeof(WORD), (char *)&nValue );

				memcpy(&pClusters[ nCluster], &nValue, sizeof(WORD));

				if (nNextCluster == FS_BUSY_CLUSTER) {	nCluster = FS_FREE_CLUSTER;	} 
				else {	nCluster = nNextCluster; }
			}
			SFSFileRecord stcFile;
			memset(&stcFile, 0, sizeof(SFSFileRecord));
			stcFile.nCluster = FS_FREE_CLUSTER;

			//			FLASH_WRITE((char *)&pFiles[i], sizeof(SFSFileRecord), (char *)&stcFile);

			memcpy(&pFiles[i], &stcFile, sizeof(SFSFileRecord));

			bResult = true;
			break;
		}
	}
	return bResult;
}

DWORD CFileSystem::createFile(const char *strFileName, WORD nMode, WORD nLock /*= 0*/ )
{
	DWORD nResult = FS_INVALID_HANDLE;
	SFSFileRecord	*pFiles = m_stcBoot.pFiles;
	int i;
	for (i = 0; i < m_stcBoot.nMaxFiles; i++)
	{
		if (!strcmp(pFiles[i].strFileName, strFileName))
		{
			if (!remove(strFileName))
			{
				return nResult;
			};
			break;
		}
	}
	for (i = 0; i < m_stcBoot.nMaxFiles; i++)
	{
		if (!pFiles[i].strFileName[0])
		{
			nResult = m_nCurHandle++;
			if (nResult == FS_INVALID_HANDLE) {	return nResult;	}
			SFSHandle	*pstcHandle = new SFSHandle;
			if (!pstcHandle)
			{
				return nResult;
			}
			memset(pstcHandle, 0, sizeof(SFSHandle));
			SFSFileRecord stcFile;
			memset(&stcFile, 0, sizeof(SFSFileRecord));
			strcpy(stcFile.strFileName, strFileName);
			RTC_CLK_READ((void *)stcFile.pCreateDate);
			RTC_CLK_READ((void *)stcFile.pChangeDate); 
			stcFile.nCluster = FS_FREE_CLUSTER;
			stcFile.attr = nLock;

			//			FLASH_WRITE((char *)&pFiles[i], sizeof(SFSFileRecord), (char *)&stcFile);

			memcpy(&pFiles[i], &stcFile, sizeof(SFSFileRecord));

			pstcHandle->nFSHandle = i;
			pstcHandle->nLock = nLock;
			pstcHandle->nMode = nMode;
			m_mapFSHandles[nResult] = pstcHandle;
			m_arrnOpenedFiles[i] = m_arrnOpenedFiles[i] + 1;
			return nResult;
		}
	}
	return nResult;
}

void CFileSystem::close( DWORD nHandle )
{
	m_objClusterBuf.flush();

	tMapFSHandles::iterator Iter = m_mapFSHandles.find(nHandle);
	if (Iter != m_mapFSHandles.end())
	{
		SFSHandle *pstc = Iter->second;

		// Ant, 13.05.09
		if (!(pstc->nMode & O_RDONLY))
		{
			SFSFileRecord *pStcFile = &m_stcBoot.pFiles[pstc->nFSHandle];
			pStcFile->nCrc32 = calcCRC32(nHandle);
		}

		m_arrnOpenedFiles[pstc->nFSHandle] = m_arrnOpenedFiles[pstc->nFSHandle] - 1;
		delete pstc;
		m_mapFSHandles.erase(Iter);
	}
}

// Ant, 13.05.09
DWORD CFileSystem::calcCRC32(DWORD nHandle)
{
	const int nTileSize = 4096;
	int cnTiles = size(nHandle) / nTileSize + Q(size(nHandle) % nTileSize, 1, 0);
	BYTE *pBuf = new BYTE[nTileSize];
	DWORD crc32 = 0;

	for (int i = 0; i < cnTiles; i++)
	{
		lseek(nHandle, i * nTileSize, FS_SEEK_SET);
		DWORD dwReaded = read(nHandle, pBuf, nTileSize);
		crc32 = ::calcCRC32(pBuf, dwReaded, crc32);
	}

	DELETE_POINTER_ARR(pBuf);
	return crc32;	
}


DWORD CFileSystem::read( DWORD nHandle, BYTE *pBuffer, DWORD nLength )
{
	m_objClusterBuf.flush();

	tMapFSHandles::iterator Iter = m_mapFSHandles.find(nHandle);
	if (Iter == m_mapFSHandles.end())
	{
		return FS_IO_ERROR;
	}
	SFSHandle *pstcHandle = Iter->second;
	SFSFileRecord *pFiles = m_stcBoot.pFiles;
	WORD *pClusters = m_stcBoot.pClusters;
	BYTE *pData = m_stcBoot.pData;

	if (pFiles[ pstcHandle->nFSHandle ].nSize < pstcHandle->nSeekPos || !pFiles[ pstcHandle->nFSHandle ].nSize)
	{
		return FS_IO_ERROR;
	}


	DWORD	nBytesRead;
	if (pstcHandle->nSeekPos + nLength <= pFiles[pstcHandle->nFSHandle].nSize)
	{
		nBytesRead = nLength;
	} else
	{
		nBytesRead = pFiles[pstcHandle->nFSHandle].nSize - pstcHandle->nSeekPos;
	}

	if (!nBytesRead) {	return nBytesRead; }


	WORD	nCurCluster = pFiles[ pstcHandle->nFSHandle ].nCluster;

	if (nCurCluster == FS_FREE_CLUSTER) { return FS_IO_ERROR; }

	DWORD	nCurBytes = 0;
	while ((nCurBytes + m_stcBoot.nClusterSize) <=  pstcHandle->nSeekPos)
	{
		nCurCluster = pClusters[ nCurCluster ];
		nCurBytes += m_stcBoot.nClusterSize;
	}

	if (nCurCluster == FS_FREE_CLUSTER) { return FS_IO_ERROR; }

	DWORD nBeginOffset = pstcHandle->nSeekPos - nCurBytes;
	DWORD nBytesReaded = 0;
	BYTE  *pCurBuf = pBuffer;
	BYTE  *pTmpData;
	while (nBytesReaded < nBytesRead)
	{
		pTmpData = pData + (DWORD)nCurCluster * m_stcBoot.nClusterSize + nBeginOffset;
		if (nBytesRead - nBytesReaded >= m_stcBoot.nClusterSize - nBeginOffset)
		{
			memcpy(pCurBuf, pTmpData, m_stcBoot.nClusterSize - nBeginOffset);
			nBytesReaded += m_stcBoot.nClusterSize - nBeginOffset;
			nCurCluster = pClusters[ nCurCluster ];
			pCurBuf += m_stcBoot.nClusterSize - nBeginOffset;
		} else
		{
			memcpy(pCurBuf, pTmpData, nBytesRead - nBytesReaded);
			nBytesReaded = nBytesRead;
		}
		nBeginOffset = 0;
	}
	pstcHandle->nSeekPos += nBytesRead;
	return nBytesRead;
}


WORD CFileSystem::findFreeCluster()
{
	// need to optimize this function!
	WORD nResult = FS_FREE_CLUSTER;
	WORD	*pCluster = m_stcBoot.pClusters;
	WORD	nValue = FS_BUSY_CLUSTER;
	for (int i = 0; i < m_stcBoot.nClustersCount; i++)
	{
		if (pCluster[i] == FS_FREE_CLUSTER)
		{
			nResult = i;

			//			FLASH_WRITE((char *)&pCluster[i], sizeof(WORD), (char *)&nValue);

			memcpy(&pCluster[i], &nValue, sizeof(WORD));

			break;
		}
	}
	return nResult;
}

DWORD CFileSystem::write( DWORD nHandle, BYTE *pBuffer, DWORD nLength )
{
	tMapFSHandles::iterator Iter = m_mapFSHandles.find(nHandle);
	if (Iter == m_mapFSHandles.end())
	{
		return FS_IO_ERROR;
	}
	SFSHandle *pstcHandle = Iter->second;
	return write(pstcHandle, pBuffer, nLength);
}

DWORD CFileSystem::write( SFSHandle *pstcHandle, BYTE *pBuffer, DWORD nLength )
{
	if (!nLength)
	{
		return FS_IO_ERROR;
	}
	m_bNeedSync = true;
	SFSFileRecord *pFiles = m_stcBoot.pFiles;
	SFSFileRecord stcFile;
	memcpy(&stcFile, &pFiles[pstcHandle->nFSHandle], sizeof(SFSFileRecord));
	WORD *pClusters = m_stcBoot.pClusters;
	BYTE *pData = m_stcBoot.pData;



	WORD	nCurCluster = stcFile.nCluster;
	WORD  nNextCluster;
	if (nCurCluster == FS_FREE_CLUSTER && pstcHandle->nSeekPos) { return FS_IO_ERROR; }

	DWORD	nCurBytes = 0;
	while ((nCurBytes + m_stcBoot.nClusterSize) <=  pstcHandle->nSeekPos)
	{
		nNextCluster = pClusters[ nCurCluster ];
		nCurBytes += m_stcBoot.nClusterSize;
		if (nNextCluster == FS_BUSY_CLUSTER)
		{
			nNextCluster = findFreeCluster();
			if (nNextCluster == FS_FREE_CLUSTER) {	return FS_IO_ERROR;	}

			//			FLASH_WRITE((char *)&pClusters[nCurCluster], sizeof(WORD), (char *)&nNextCluster);

			memcpy(&pClusters[nCurCluster], &nNextCluster, sizeof(WORD));

		}
		nCurCluster = nNextCluster;
	}

	/*
	char tmp[100];
	debugOut("write debug...");
	sprintf(tmp, "nCurCluster=%d", nCurCluster);
	debugOut(tmp);
	*/




	if (nCurCluster == FS_FREE_CLUSTER) 
	{ 
		nCurCluster = findFreeCluster();
		if (nCurCluster == FS_FREE_CLUSTER)
		{
			return FS_IO_ERROR;
		}
		stcFile.nCluster = nCurCluster;
	}

	DWORD	nBytesWritten = 0;
	BYTE	*pTmpData;
	BYTE	*pBuf = pBuffer;
	DWORD nBeginOffset = pstcHandle->nSeekPos - nCurBytes;
	DWORD nBytesToWrite;

	while (nBytesWritten < nLength)
	{
		if (nLength - nBytesWritten  <= m_stcBoot.nClusterSize - nBeginOffset)
		{
			nBytesToWrite = nLength - nBytesWritten;
		} else
		{
			nNextCluster = pClusters[ nCurCluster ];
			if (nNextCluster == FS_BUSY_CLUSTER)
			{
				nNextCluster = findFreeCluster();
				if (nNextCluster == FS_FREE_CLUSTER)
				{
					return FS_IO_ERROR;
				}

				//				FLASH_WRITE((char *)&pClusters[nCurCluster], sizeof(WORD), (char *)&nNextCluster);

				memcpy(&pClusters[nCurCluster], &nNextCluster, sizeof(WORD));

			}
			nBytesToWrite = m_stcBoot.nClusterSize - nBeginOffset;
		}

		/*
		char t[100];
		sprintf(t, "c-%d, b-%d", nCurCluster, nBeginOffset);
		Lprint(1, 0, t);
		Lprint(2, 0, (char *)pBuf);
		*/


		/*
		sprintf(tmp, "Cluster=%d, offset=%d, nBytesToWrite=%d", nCurCluster, nBeginOffset, nBytesToWrite);
		debugOut(tmp);
		*/
		pTmpData = pData + (DWORD)nCurCluster * m_stcBoot.nClusterSize + nBeginOffset;

		//FLASH_WRITE((char *)pTmpData, nBytesToWrite, (char *)pBuf);
		m_objClusterBuf.write((char *)pTmpData, nBytesToWrite, (char *)pBuf);

		//		memcpy(pTmpData, pBuf, nBytesToWrite);

		pBuf += nBytesToWrite;
		nBytesWritten += nBytesToWrite;
		nCurCluster = nNextCluster;
		nBeginOffset = 0;
	}

	pstcHandle->nSeekPos += nBytesWritten;
	if (pstcHandle->nSeekPos > stcFile.nSize)
	{
		stcFile.nSize = pstcHandle->nSeekPos;
	}
	/*
	sprintf(tmp, "Size of file=%d", stcFile.nSize);
	debugOut(tmp);
	*/

	RTC_CLK_READ((void *)stcFile.pChangeDate); 

	//	FLASH_WRITE((char *)&pFiles[pstcHandle->nFSHandle], sizeof(SFSFileRecord), (char *)&stcFile);

	memcpy(&pFiles[pstcHandle->nFSHandle], &stcFile, sizeof(SFSFileRecord));

	return nBytesWritten;
}

DWORD CFileSystem::size( DWORD nHandle )
{
	tMapFSHandles::iterator Iter = m_mapFSHandles.find(nHandle);
	if (Iter == m_mapFSHandles.end())
	{
		return FS_IO_ERROR;
	}
	SFSHandle *pstcHandle = Iter->second;
	SFSFileRecord *pFiles = m_stcBoot.pFiles;
	DWORD	nResult = pFiles[pstcHandle->nFSHandle].nSize;

	/*
	char t[30];
	sprintf(t, "s = %d", nResult);
	debugOut(t);
	*/

	return nResult;
}

void CFileSystem::dump()
{
	/*
	BYTE *pBuf = m_stcBoot.pData;
	SFSFileRecord *pFiles = m_stcBoot.pFiles;
	char tmp[100];
	for (int i = 0; i < m_stcBoot.nMaxFiles; i++)
	{
	if (pFiles[i].strFileName[0])
	{
	Lprint(0, 0, pFiles[i].strFileName);
	sprintf(tmp, "s-%d c-%d", pFiles[i].nSize, pFiles[i].nCluster);
	Lprint(1, 0, tmp);
	if (pFiles[i].nCluster != FS_FREE_CLUSTER)
	{
	pBuf += pFiles[i].nCluster * m_stcBoot.nClusterSize;
	Lprint(2, 0, (char *)pBuf);
	}
	break;
	}
	}

	*/
}

DWORD CFileSystem::lseek( DWORD nHandle, int nOffset, char nOrigin )
{
	DWORD	nResult = FS_IO_ERROR;
	tMapFSHandles::iterator Iter = m_mapFSHandles.find(nHandle);
	if (Iter == m_mapFSHandles.end())
	{
		return nResult;
	}
	SFSHandle *pstcHandle = Iter->second;
	SFSFileRecord *pFiles = m_stcBoot.pFiles;
	int	nStartOffset = 0;
	switch(nOrigin)
	{
	case FS_SEEK_SET: nStartOffset = 0; break;
	case FS_SEEK_CUR: nStartOffset = pstcHandle->nSeekPos; break;
	case FS_SEEK_END: nStartOffset = pFiles[ pstcHandle->nFSHandle].nSize; break;
	}
	if (nStartOffset + nOffset <= 0)
	{
		pstcHandle->nSeekPos = 0;
	} else
		if (nStartOffset + nOffset <= pFiles[ pstcHandle->nFSHandle].nSize)
		{
			pstcHandle->nSeekPos = nStartOffset + nOffset;
		} else
			if (nStartOffset + nOffset > pFiles[ pstcHandle->nFSHandle].nSize)
			{
				pstcHandle->nSeekPos = pFiles[ pstcHandle->nFSHandle].nSize;
				DWORD	BytesToWrite = nStartOffset + nOffset - pFiles[ pstcHandle->nFSHandle].nSize;
				BYTE	*pBuf = new BYTE [BytesToWrite];
				memset(pBuf, 0, BytesToWrite);
				if (write(pstcHandle, pBuf, BytesToWrite) != BytesToWrite)
				{
					DELETE_POINTER_ARR(pBuf);
					return nResult;
				}
				DELETE_POINTER_ARR(pBuf);
			}
			nResult = pstcHandle->nSeekPos;
			return nResult;
}

bool CFileSystem::eof( DWORD nHandle )
{
	tMapFSHandles::iterator Iter = m_mapFSHandles.find(nHandle);
	if (Iter == m_mapFSHandles.end())
	{
		return true;
	}
	SFSHandle *pstcHandle = Iter->second;
	SFSFileRecord *pFiles = m_stcBoot.pFiles;
	return (pstcHandle->nSeekPos >= pFiles[ pstcHandle->nFSHandle ].nSize)?(true):(false);
}

void CFileSystem::init()
{
	m_bNeedSync = false;
	int nFSSize = FS_CALC_SIZE;
	memset(m_arrRAM, 0, nFSSize);
	memcpy(m_arrRAM, FS_BEGIN, nFSSize);

	m_nCurHandle = 0;
	memcpy(&m_stcBoot, FS_BEGIN, sizeof(SFSBoot));
	memset(m_arrnOpenedFiles, 0, sizeof(WORD));

	// Ant, 12.05.09
	do
	{
		if (memcmp(m_stcBoot.strMagicWord, FS_MAGIC_STRING, strlen(FS_MAGIC_STRING)))
		{
			// Необходимо востановить заголовок с backup
			DBGLine("Restore fs header from backup");
			FLASH_WRITE((char *)FS_BEGIN, nFSSize, (char *)FS_BACKUP_BEGIN);
			memcpy(&m_stcBoot, FS_BEGIN, sizeof(SFSBoot));
			memcpy(m_arrRAM, FS_BEGIN, nFSSize);
			// break не нужно, проверяем востановленный херед на валидность.
		}

		if (memcmp(m_stcBoot.strMagicWord, FS_MAGIC_STRING, strlen(FS_MAGIC_STRING)))
		{
			// Filesystem is absent and need to format flash
			format();
			memcpy(&m_stcBoot, FS_BEGIN, sizeof(SFSBoot));
			memcpy(m_arrRAM, FS_BEGIN, nFSSize);
			break;
		}
	} while (false);
	m_stcBoot.pFiles = (SFSFileRecord *)(m_arrRAM + sizeof(SFSBoot));
	m_stcBoot.pClusters = (WORD *)((DWORD)m_stcBoot.pFiles + (DWORD)((DWORD)m_stcBoot.nMaxFiles * sizeof(SFSFileRecord)));

	// Ant, 13.05.09
	// Листаем все файлы и сверяем crc. Если не совпадает, то файл удаляем.
	SFSFileRecord *pMemFiles = m_stcBoot.pFiles;
	for (int i = 0; i < m_stcBoot.nMaxFiles; i++)
	{
		if (pMemFiles[i].strFileName[0])
		{
			DWORD nHandle = open(pMemFiles[i].strFileName, O_RDONLY);
			DWORD crc32Calc = calcCRC32(nHandle);
			if (crc32Calc != pMemFiles[i].nCrc32)
			{
				DBGLine("CRC wrong, filename: %s", pMemFiles[i].strFileName);
				close(nHandle);
				DBGLine("Calced crc: %d, file crc: %d", crc32Calc, pMemFiles[i].nCrc32);
				remove(pMemFiles[i].strFileName);
				continue;
			}
			close(nHandle);
		}
	}

	memset(m_arrnOpenedFiles, 0, sizeof(m_arrnOpenedFiles));
}

DWORD CFileSystem::tell( DWORD nHandle )
{
	tMapFSHandles::iterator Iter = m_mapFSHandles.find(nHandle);
	if (Iter == m_mapFSHandles.end())
	{
		return FS_IO_ERROR;
	}
	SFSHandle *pstcHandle = Iter->second;
	return pstcHandle->nSeekPos;
}


int CFileSystem::ls( char *strFiles )
{
	int nResult = 1;
	SFSFileRecord *pMemFiles;
	pMemFiles = m_stcBoot.pFiles;
	for (int i = 0; i < m_stcBoot.nMaxFiles; i++)
	{
		if (pMemFiles[i].strFileName[0])
		{
			nResult += strlen(pMemFiles[i].strFileName) + 1;

		}
	}
	if (!strFiles)
	{
		return nResult;
	}
	pMemFiles = m_stcBoot.pFiles;
	char *pMem = strFiles;
	for (int i = 0; i < m_stcBoot.nMaxFiles; i++)
	{
		if (pMemFiles[i].strFileName[0])
		{
			memcpy(pMem, pMemFiles[i].strFileName, strlen(pMemFiles[i].strFileName));
			pMem += strlen(pMemFiles[i].strFileName);
			memset(pMem, 0, 1);
			pMem++;

		}
	}
	memset(pMem, 0, 1);
	return nResult;
}

//-----------------------------------------------------------------------------/
/*void ddd(char* s)
{
OpenRS232Port(0, 7, 0, 1);
WrCom(0, s, strlen(s));
WaitTxDone(0);
char del[] = "\n\r";
WrCom(0, del, sizeof(del));
WaitTxDone(0);
}
*/

void CFileSystem::sync()
{
	int nFSSize = FS_CALC_SIZE;

	/*if (m_bNeedSync)
	{
	FLASH_WRITE((char *)FS_BEGIN, nFSSize, m_arrRAM);
	m_bNeedSync = false;
	}*/

	// Ant, 12.05.09
	if (m_bNeedSync)
	{
		// 1. Сбрасываем необходимый блок данных, которые остались незаписанными.
		//1. Reset the desired data block, which remained unwritten.
		m_objClusterBuf.flush();

		// 2. Записываем "нормальный" заголовок в back up область.
		// 2. Write "normal" heading back up in the area
		FLASH_WRITE((char *)FS_BACKUP_BEGIN, nFSSize, m_arrRAM);

		// 3. Записываем m_arrRAM в рабочую область, при этом MAGIC WORD все еще неверный.
		// 3. M_arrRAM write to the workspace, while MAGIC WORD still wrong.
		SFSBoot	*pStcBoot = (SFSBoot *) m_arrRAM;
		memcpy(pStcBoot->strMagicWord, FS_WRONG_STRING, strlen(FS_WRONG_STRING));
		FLASH_WRITE((char *)FS_BEGIN, nFSSize, m_arrRAM);

		// 4. Изменяем MAGIC WORD на верный.
		// 4. Changing MAGIC WORD on the right.
		memcpy(pStcBoot->strMagicWord, FS_MAGIC_STRING, strlen(FS_MAGIC_STRING));
		FLASH_WRITE((char *)FS_BEGIN, sizeof(SFSBoot), (char *)pStcBoot);

		m_bNeedSync = false;
	}



}

DWORD CFileSystem::freeSpace()
{
	DWORD nResult = 0;
	WORD *arrClusters = m_stcBoot.pClusters;
	for (int i = 0; i < m_stcBoot.nClustersCount; i++)
	{
		if (arrClusters[i] == FS_FREE_CLUSTER)
		{
			nResult++;
		}
	}
	nResult *= m_stcBoot.nClusterSize;
	return nResult;
}

bool CFileSystem::fileExistsSize( const char *strFileName, DWORD *nSize )
{
	SFSFileRecord	*pFiles = m_stcBoot.pFiles;
	int i;
	for (i = 0; i < m_stcBoot.nMaxFiles; i++)
	{
		if (!strcmp(pFiles[i].strFileName, strFileName))
		{
			if (nSize) *nSize = pFiles[i].nSize;
			return true;
		}
	}
	if (nSize) *nSize = 0;
	return false;
}

void CFileSystem::formatExtern()
{
	int nFSSize = FS_CALC_SIZE;
	m_bNeedSync = false;
	m_nCurHandle = 0;
	memset(m_arrnOpenedFiles, 0, sizeof(WORD));
	format();
	memcpy(&m_stcBoot, FS_BEGIN, sizeof(SFSBoot));
	memcpy(m_arrRAM, FS_BEGIN, nFSSize);
	m_stcBoot.pFiles = (SFSFileRecord *)(m_arrRAM + sizeof(SFSBoot));
	m_stcBoot.pClusters = (WORD *)((DWORD)m_stcBoot.pFiles + (DWORD)((DWORD)m_stcBoot.nMaxFiles * sizeof(SFSFileRecord)));
}




CClusterBuffer::CClusterBuffer()
{
	m_pDestAddr = NULL;
	memset(m_pBuf, 0, FS_CLUSTER_BUFFER_SIZE);
	m_nSize = 0;


}

void CClusterBuffer::write( char *pDest, int nSize, char *pSource )
{
	if (m_nSize && m_pDestAddr + m_nSize == pDest)
	{
		if (m_nSize + nSize >= FS_CLUSTER_BUFFER_SIZE)
		{
			flush();
		}
		memcpy(m_pBuf + m_nSize, pSource, nSize);
		m_nSize += nSize;

	} else
	{
		flush();
		m_pDestAddr = pDest;
		m_nSize = nSize;
		memcpy(m_pBuf, pSource, nSize);
	}
}



void CClusterBuffer::flush()
{
	if (m_nSize)
	{
		FLASH_WRITE(m_pDestAddr, m_nSize, m_pBuf);
		m_pDestAddr = m_pDestAddr + m_nSize;
		/*
		char tmp[100];
		sprintf(tmp, "flush: addr=0x%X size=%d", m_pDestAddr, m_nSize);
		ddd(tmp);
		*/
		m_nSize = 0;
	}
}

//end
