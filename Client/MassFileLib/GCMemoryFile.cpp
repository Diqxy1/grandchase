#include "stdafx.h"
#include ".\gcmemoryfile.h"
#include "GCMassFileManager.h"

////////////////////////////////////////////////////////////////////////////////////
// MemoryFile
////////////////////////////////////////////////////////////////////////////////////

GCMemoryFile::GCMemoryFile(GCMassFileManager * manager,std::string filename)
	:m_pMFManager(manager),m_iCursorPos(0),m_pData(NULL),m_pCursor(NULL),m_iSize(0)
{
	m_bLoaded = m_pMFManager->LoadMemoryFile(filename,&m_pData,&m_iSize);
	if(m_bLoaded)
		m_pCursor=m_pData;
}

GCMemoryFile::~GCMemoryFile()
{
	SAFE_DELETE_ARRAY(m_pData);
}


int GCMemoryFile::ReadFile(void* pDest, int ReadBytes)
{
	if( m_iCursorPos + ReadBytes > m_iSize )
		return 0;

	memcpy(pDest,m_pCursor,ReadBytes);
	m_pCursor		+= ReadBytes;
	m_iCursorPos	+= ReadBytes;

	return ReadBytes;
}
 
int GCMemoryFile::FileSeek(int iPos, int Pivot)
{
	switch(Pivot)
	{	
	case FILESEEK_CUR:
		{
			m_iCursorPos += iPos;
		}
		break;

	case FILESEEK_SET:
		{
			m_iCursorPos = iPos;
		}
		break;

	case FILESEEK_END:
		{
			m_iCursorPos = (m_iSize-1) + iPos;
		}
		break;	
	}
	m_pCursor = m_pData + m_iCursorPos;
	return m_iCursorPos;
}