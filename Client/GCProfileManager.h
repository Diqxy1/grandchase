#pragma once

#include "KNCSingleton.h"

class KGCProfileManager
{
    DeclareSingleton( KGCProfileManager );

    enum
    {
        EM_LOG_MEMORY_USAGE = 0,   //  프로세서 메모리 사용량
        EM_LOG_UI_MEMORY_USAGE = 1,
        EM_LOG_SIZE,
    };

public:
    KGCProfileManager();
    ~KGCProfileManager();

public:
    void FrameMove( float fElapsedTime );
    void WriteLog( int emLogType );

public:
    void SetLog( bool bLog ) { m_bLog = bLog; }

public:
    bool  m_bLog;
    
    DWORD m_dwTickCount[EM_LOG_SIZE];
    DWORD m_dwTickGaps[EM_LOG_SIZE];    
    
    FILE* m_pFile;
};

DefSingletonInline( KGCProfileManager );