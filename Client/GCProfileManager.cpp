#include "stdafx.h"
#include "GCProFileManager.h"

ImplementSingleton( KGCProfileManager )

KGCProfileManager::KGCProfileManager()
: m_pFile( NULL ) 
, m_bLog ( false )
{
    //  기본값 1분
    memset( m_dwTickCount, 0, sizeof( m_dwTickCount[0] ) * EM_LOG_SIZE );
    memset( m_dwTickGaps, 0, sizeof( m_dwTickGaps[0] ) * EM_LOG_SIZE );


    m_dwTickGaps[EM_LOG_MEMORY_USAGE] = 1 * 60 * 1000;    // 1분마다
    m_dwTickGaps[EM_LOG_UI_MEMORY_USAGE] = 1 * 60 * 1000;    // 1분마다

#if !defined( __PATH__ )
    m_bLog = true;
#endif 
}

KGCProfileManager::~KGCProfileManager()
{
    if ( m_pFile ) 
    {
        fclose( m_pFile );
    }
}

void KGCProfileManager::FrameMove( float fElapsedTime )
{
    if ( !m_bLog ) 
        return;

    DWORD dwCurTick = GetTickCount();

    for ( int i = 0; i < EM_LOG_SIZE; ++i ) 
    {
        if ( dwCurTick - m_dwTickCount[i] >= m_dwTickGaps[i] ) 
        {
            WriteLog( i );    
            m_dwTickCount[i] = dwCurTick;
        }
    }
}

void KGCProfileManager::WriteLog( int emLogType )
{
    if ( !m_pFile ) 
    {
        m_pFile = fopen( "profile.txt", "wb" );
    }

    if ( !m_pFile ) 
        return;

    std::string curtime = GCUTIL_STR::GCStrWideToChar( (LPCTSTR)(CTime::GetCurrentTime().Format( KNC_TIME_FORMAT )) );
    switch( emLogType ) 
    {
    case EM_LOG_MEMORY_USAGE:
        {
            int nUseMemory = GetNowProcessMemory();
            nUseMemory = nUseMemory / 1024;

            std::string strEnum = EnumToString( EM_LOG_MEMORY_USAGE );
            fprintf( m_pFile, "%s\t%s\t%dKB\n", curtime.c_str(), 
                                                strEnum.c_str(), nUseMemory );
            fflush( m_pFile );
        }
        break;
    case EM_LOG_UI_MEMORY_USAGE:
        {
            if ( !g_pkUIScene ) 
                break;
            
            int nUseMemory = g_pkUIScene->GetMemorySize();
            nUseMemory = nUseMemory / 1024;

            std::string strEnum = EnumToString( EM_LOG_UI_MEMORY_USAGE );
            fprintf( m_pFile, "%s\t%s\t%dKB\n", curtime.c_str(), 
                strEnum.c_str(), nUseMemory );
            fflush( m_pFile );
        }
        break;
    default:
        break;
    }
}