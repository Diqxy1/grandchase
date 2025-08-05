#include "stdafx.h"
#include ".\gcfilemoniter.h"
//

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif

ImplementSingleton( KGCFileMoniter );

void ClearPointer( CAloFileMonitor* arg )
{
	arg->Stop();
	SAFE_DELETE( arg );
}

KGCFileMoniter::KGCFileMoniter(void)
{
}

KGCFileMoniter::~KGCFileMoniter(void)
{
	std::for_each( m_vecFileMoniters.begin(), m_vecFileMoniters.end(), ClearPointer );
	m_vecFileMoniters.clear();
}

void KGCFileMoniter::RegisterFile( std::string strFileName, PFnFileChanged pFunc )
{
//	if ( -1 != ::_access( strFileName.c_str(), 0 ) ) return;
#ifdef __PATH__
    return;
#endif
	std::string nullStr = "";
	if(strFileName == nullStr) return;

    for( unsigned int i = 0 ; i < m_vecFileMoniters.size(); i++ )
    {
        if( m_vecFileMoniters[i]->m_filePath == strFileName )
            return;
    }

	CAloFileMonitor* pcMoniter = new CAloFileMonitor();

	pcMoniter->SetFileMonitor( strFileName , pFunc );
	pcMoniter->Start();

	m_vecFileMoniters.push_back( pcMoniter );
}
