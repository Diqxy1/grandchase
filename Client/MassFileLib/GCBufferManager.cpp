#include "stdafx.h"
#include ".\gcbuffermanager.h"

#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////////
// BufferManager
////////////////////////////////////////////////////////////////////////////////////
GCBufferManager::GCBufferManager()
{
	m_pData = NULL;
	m_iMemorySize = 1024 * 1024 + 1024;		//1MB + 1KB

	m_pData = new char[m_iMemorySize];
}

GCBufferManager::~GCBufferManager()
{
    SAFE_DELETE_ARRAY( m_pData );
    m_iMemorySize = 0;
}

char* GCBufferManager::GetBuffer( int iSize )
{
	if( m_iMemorySize <= iSize )
	{
        SAFE_DELETE_ARRAY( m_pData );
		while( (m_iMemorySize += 1024*256 ) <= iSize )       // 메모리를 256KB씩 증가.
		{
		}		
		m_pData = new char[m_iMemorySize];
	}
	return m_pData;
}