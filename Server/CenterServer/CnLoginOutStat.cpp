#include ".\CnLoginoutstat.h"
#include <dbg/dbg.hpp>
#include "Log4.h"


//FILE_NAME_FOR_LOG

KCnLoginOutStat::KCnLoginOutStat(void)
:m_bFileExist(false)
{
}

KCnLoginOutStat::~KCnLoginOutStat(void)
{
}

void KCnLoginOutStat::Init( int nID_, int nFlushGap_, bool bWriteToDB_, int nRowCount_ )
{
    KLocker lock( m_csStat );
    KStatistics::Init( nID_, nFlushGap_, bWriteToDB_, nRowCount_ );
    m_kLog_InOut.CloseFile();
    m_bFileExist = m_kLog_InOut.OpenFile( std::wstring ( L"LoginOutCount" ) );

}

void KCnLoginOutStat::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap )
        return;

    Flush();
    ResetTime();
}

void KCnLoginOutStat::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );

    stm_ << L"In : " <<  m_kCountInfo.m_nLoginCount
        << L", Out : " << m_kCountInfo.m_nLogoutCount       
         << std::endl;
}

void KCnLoginOutStat::Flush()
{
    // 파일에다 기록 한다.
    KLocker lock( m_csStat );
    JIF( m_bFileExist );

    DWORD dwWriteDB = 0;
    if( m_bWriteToDB ) {
        dwWriteDB = 1;
    }

    KLoginoutCount kCountInfo;
    kCountInfo = m_kCountInfo;

    m_kLog_InOut << L"[" << (LPCTSTR)CTime( kCountInfo.m_tmStartTime ).Format( KNC_TIME_FORMAT )
                 << L"] LOGIN: " << kCountInfo.m_nLoginCount
                 << L" LOGOUT: " << kCountInfo.m_nLogoutCount
                 << L" CCU: " << kCountInfo.m_dwCCU
                 << GLOG::endl;

    // 클리어 한다.
    Clear();
}

void KCnLoginOutStat::AddStat( int nCase_ )
{
    JIF( m_bFileExist );
    KLocker lock( m_csStat );
    switch( nCase_ )
    {
    case LOGIN:
        ++m_kCountInfo.m_nLoginCount;
        break;
    case LOGOUT:
        ++m_kCountInfo.m_nLogoutCount;
        break;

    default:
        START_LOG( cerr, L"등록되지 않은 경우입니다, Value : " << nCase_ ) << END_LOG;
    }
}

void KCnLoginOutStat::UpdateCCU( DWORD dwCCU_ )
{
    JIF( m_bFileExist );
    KLocker lock( m_csStat );

    m_kCountInfo.m_dwCCU = dwCCU_;
}


void KCnLoginOutStat::Clear()
{
    KLocker lock( m_csStat );

    m_kCountInfo.m_nLoginCount = 0;
    m_kCountInfo.m_nLogoutCount = 0;
    m_kCountInfo.m_dwCCU = 0;

    ::time( &m_kCountInfo.m_tmStartTime );
}
