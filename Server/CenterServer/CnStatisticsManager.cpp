#include ".\CnStatisticsManager.h"
#include "Statistics.h"
#include "CnLoginOutStat.h"
#include <dbg/dbg.hpp>
#include "Log4.h"

//FILE_NAME_FOR_LOG

ImplementSingleton( KCnStatisticsManager );
NiImplementRootRTTI( KCnStatisticsManager );
ImplOstmOperatorW2A( KCnStatisticsManager );

const wchar_t KCnStatisticsManager::ms_szSITitle[][SI_MAX_STRING] = {
#define SI_ENUM( id, comment ) L#id,
#include "CnStatisticsID_def.h"
#undef SI_ENUM
};

const wchar_t KCnStatisticsManager::ms_szSIDesc[][SI_MAX_STRING] = {
#define SI_ENUM( id, comment ) L#comment,
#include "CnStatisticsID_def.h"
#undef SI_ENUM
};

ImplToStringW( KCnStatisticsManager )
{
    KLocker lock( m_csStatistics );
    START_TOSTRINGW << TOSTRINGW( m_mapStatistics.size() );

    std::map< int, KStatisticsPtr >::const_iterator mit;
    for( mit = m_mapStatistics.begin() ; mit != m_mapStatistics.end() ; ++mit )
    {
        mit->second->Dump( stm_ );
    }
    return stm_;
}

KCnStatisticsManager::KCnStatisticsManager(void)
{
}

KCnStatisticsManager::~KCnStatisticsManager(void)
{
}

void KCnStatisticsManager::AddStatistics( int nID_, int nFlushGap_, bool bWriteToDB_, int nRowCount_ ) // config_server.lua 에서 호출
{
    KLocker lock( m_csStatistics );

    JIF( nID_ >= 0 && nID_ < SI_SENTINEL );
    JIF( m_mapStatistics.find( nID_ ) == m_mapStatistics.end() ); // 이미 있으면 쌩..
    KStatisticsPtr spStatistics = CreateStatObject( nID_ );
    JIF( spStatistics );
    spStatistics->Init( nID_, nFlushGap_, bWriteToDB_, nRowCount_ );
    spStatistics->SetDesc( std::wstring(KCnStatisticsManager::ms_szSITitle[nID_]), std::wstring(KCnStatisticsManager::ms_szSIDesc[nID_]) );
    m_mapStatistics.insert( std::make_pair( nID_, spStatistics ) );
}

void KCnStatisticsManager::ModifyStatistics( int nID_, int nFlushGap_, bool bWriteToDB_, int nRowCount_ )
{
    std::map< int, KStatisticsPtr >::iterator mit;
    mit = m_mapStatistics.find( nID_ );
    JIF( mit != m_mapStatistics.end() );
    mit->second->Init( nID_, nFlushGap_, bWriteToDB_, nRowCount_ );
}

void KCnStatisticsManager::Tick()
{
    KLocker lock( m_csStatistics );
    std::map< int, KStatisticsPtr >::iterator mit;
    for( mit = m_mapStatistics.begin() ; mit != m_mapStatistics.end() ; ++mit )
    {
        mit->second->Tick();
    }
}

const wchar_t* KCnStatisticsManager::GetSiName( int nID_ )
{
    _JIF( nID_ < SI_SENTINEL && nID_ >= 0, return &KCnStatisticsManager::ms_szSITitle[ SI_SENTINEL ][0] );
    return &KCnStatisticsManager::ms_szSITitle[ nID_ ][0];
}

KStatisticsPtr KCnStatisticsManager::FindStatistics( int nID_ ) // 통계 포인터 얻을 때 사용
{
    std::map< int, KStatisticsPtr >::iterator mit;

    KLocker lock( m_csStatistics );
    mit = m_mapStatistics.find( nID_ );
    if( mit == m_mapStatistics.end() ) return KStatisticsPtr();

    return mit->second;
}

KStatisticsPtr KCnStatisticsManager::CreateStatObject( int nID_ )
{
    switch( nID_ )
    {
    case SI_LOGINOUT_STAT:
        {
            KCnLoginOutStatPtr spStat( new KCnLoginOutStat );
            return std::static_pointer_cast<KStatistics>(spStat);
        }

    default:
        START_LOG( cerr, L" 알수 없는 통계 객체를 생성하려 했음.. ID : " << nID_ ) << END_LOG;
    }

    return KStatisticsPtr();
}

void KCnStatisticsManager::AddStatDisconn( int DisconnReason_ )
{

}