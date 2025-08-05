#include ".\loginoutstat.h"
#include <dbg/dbg.hpp>
#include "UserPacket.h"
#include "GSDBLayer.h"
#include "UserEvent.h"
#include "Log4.h"
#include "boost/format.hpp"
//FILE_NAME_FOR_LOG

KLoginOutStat::KLoginOutStat(void)
:m_bFileExist(false)
{
    m_kCountInfo.Clear();
    ::time( &m_kCountInfo.m_tmStarTime );
}

KLoginOutStat::~KLoginOutStat(void)
{
}

void KLoginOutStat::Init( int nID_, int nFlushGap_, bool bWriteToDB_, int nRowCount_ )
{
    KLocker lock( m_csStat );
    KStatistics::Init( nID_, nFlushGap_, bWriteToDB_, nRowCount_ );
    m_kLog_InOut.CloseFile();
    m_bFileExist = m_kLog_InOut.OpenFile( std::wstring ( L"LoginOutCount" ) );

}

void KLoginOutStat::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap )
        return;

    Flush();
    ResetTime();
}

void KLoginOutStat::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );

    stm_ << L"In : " <<  m_kCountInfo.m_nLoginCount + m_kCountInfo.m_nServerInCount
        << L", Out : " << m_kCountInfo.m_nLogoutCount +m_kCountInfo.m_nServerOutCount
        << L" (" << m_kCountInfo.m_nZombieOut << L")"
        << L" (" << m_kCountInfo.m_nServerReConnectCount << L")" << std::endl;
}

void KLoginOutStat::Flush()
{
    // 파일에다 기록 한다.
    KLocker lock( m_csStat );
    JIF( m_bFileExist );

    DWORD dwWriteDB = 0;
    if( m_bWriteToDB ) {
        dwWriteDB = 1;
    }

    KCountInfo kCountInfo;
    kCountInfo = m_kCountInfo;

    std::string strLoginCount = boost::str( boost::format("%5d") % kCountInfo.m_nLoginCount );
    std::string strLogOutCount = boost::str( boost::format("%5d") % kCountInfo.m_nLogoutCount );

    m_kLog_InOut << L"[" << (LPCTSTR)CTime( kCountInfo.m_tmStarTime ).Format( KNC_TIME_FORMAT )
                 << L"] LOGIN: " << strLoginCount.c_str()
                 << L" LOGOUT: " << strLogOutCount.c_str()
                 << L" MIGRATION_IN: " << kCountInfo.m_nServerInCount
                 << L" MIGRATION_OUT: " << kCountInfo.m_nServerOutCount
                 << L" ZOMBIE_OUT: " << kCountInfo.m_nZombieOut
                 << L" SERVER_RECONNECT: " << kCountInfo.m_nServerReConnectCount
                 << L" PCBANG_LOGOUT: " << kCountInfo.m_nPCBangLogoutCount
                 << GLOG::endl;

    // DB에 기록.
    SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_STAT_LOGINOUT_COUNT, L"LoginOutStatistics", dwWriteDB, 0, kCountInfo );

    // 클리어 한다.
    Clear();
}

void KLoginOutStat::AddStat( int nCase_ )
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
    case LOGIN_MIGRATION:
        ++m_kCountInfo.m_nServerInCount;
        break;
    case LOGOUT_MIGRATION:
        ++m_kCountInfo.m_nServerOutCount;
    case ZOMBIE_OUT:
        ++m_kCountInfo.m_nZombieOut;
        break;
    case LOGIN_RECONNECT:
        ++m_kCountInfo.m_nServerReConnectCount;
        break;
    case PCBANG_LOGOUT:
        ++m_kCountInfo.m_nPCBangLogoutCount;
        break;
    default:
        START_LOG( cerr, L"등록되지 않은 경우입니다, Value : " << nCase_ ) << END_LOG;
    }
}


void KLoginOutStat::Clear()
{
    KLocker lock( m_csStat );

    m_kCountInfo.m_nLoginCount = 0;
    m_kCountInfo.m_nLogoutCount = 0;
    m_kCountInfo.m_nServerInCount = 0;
    m_kCountInfo.m_nServerOutCount = 0;
    m_kCountInfo.m_nZombieOut = 0;
    m_kCountInfo.m_nServerReConnectCount = 0;
    m_kCountInfo.m_nPCBangLogoutCount = 0;

    ::time( &m_kCountInfo.m_tmStarTime );
}
