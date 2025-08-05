#include "EclipsePlot.h"

#include <dbg/dbg.hpp>
#include "Lua/KLuaManager.h"
#include "Log4.h"

ImplementSingleton( KEclipsePlot );
ImplOstmOperatorW2A( KEclipsePlot );
NiImplementRootRTTI( KEclipsePlot );

KEclipsePlot::KEclipsePlot(void)
:m_tmBegin(1)
,m_tmEnd(1)
{
}

KEclipsePlot::~KEclipsePlot(void)
{
}

ImplToStringW( KEclipsePlot )
{
    KLocker lock( m_csEclipsePlot );

    return START_TOSTRINGW
        << TOSTRINGWt( CTime( m_tmBegin ) )
        << TOSTRINGWt( CTime( m_tmEnd ) );
}

bool KEclipsePlot::LoadScript()
{
    return _LoadScript(std::string());
}

bool KEclipsePlot::_LoadScript( OUT std::string& strScript_ )
{
    int nMaxProgress;
    time_t tmBegin;
    time_t tmEnd;
    KLuaManager kLuaMng;
    VEC_REWARD vecTimeRewardList;
    VEC_REWARD vecHuntRewardList;
    VEC_REWARD vecFinalRewardList;
    std::pair<GCITEMID,int> prHuntItem;

    const std::string strScriptName = "InitEclipsePlot.lua";
    strScript_ = strScriptName;

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    _JIF( LoadRewardList( kLuaMng, std::string( "TimeRewardList" ), vecTimeRewardList ), return false );
    _JIF( LoadRewardList( kLuaMng, std::string( "HuntRewardList" ), vecHuntRewardList ), return false );
    _JIF( LoadRewardList( kLuaMng, std::string( "FinalRewardList" ), vecFinalRewardList ), return false );
    _JIF( LoadHuntItem( kLuaMng, std::string( "HuntItem"), prHuntItem ), return false );
    _JIF( kLuaMng.GetValue( "MaxFinalProgress", nMaxProgress ) == S_OK, return false );
    

    KLocker lock( m_csEclipsePlot );
    m_tmBegin = tmBegin;
    m_tmEnd = tmEnd;
    m_nMaxProgress = nMaxProgress;
    m_vecTimeRewardList.swap( vecTimeRewardList );   
    m_vecHuntRewardList.swap( vecHuntRewardList );
    m_vecFinalRewardList.swap( vecFinalRewardList );
    m_prHuntItem.swap( prHuntItem );

    return true;
}

bool KEclipsePlot::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{ // DicePlay 에서 가져옴
    int nYear, nMonth, nDay, nHour;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, 0, 0 ) );
    return true;
}

bool KEclipsePlot::IsRun()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csEclipsePlot );
    return ( m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent );
}

void KEclipsePlot::GetTimeRewardItem( IN VEC_REWARD& vecItem_ )
{
    KLocker lock( m_csEclipsePlot );

    vecItem_ = m_vecTimeRewardList;
}

bool KEclipsePlot::LoadRewardList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KDropItemInfo >& vecRewardList_ )
{
    vecRewardList_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true )
    {
        KDropItemInfo kData;
        if( kLuaMng_.GetValue( i++, kData.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kData.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kData.m_nPeriod ) == S_OK, return false );
        vecRewardList_.push_back( kData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KEclipsePlot::LoadHuntItem( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::pair<GCITEMID,int>& prData_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( 1, prData_.first ) == S_OK, return false ); // ItemID
    _JIF( kLuaMng_.GetValue( 2, prData_.second ) == S_OK, return false ); // count

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KEclipsePlot::GetRewardInfo( OUT KEVENT_ECLIPSE_PLOT_INFO_ACK& kData_ )
{
    KLocker lock( m_csEclipsePlot );
    kData_.m_nMaxProgress = m_nMaxProgress;
    kData_.m_vecTimeRewardItem = m_vecTimeRewardList;
    kData_.m_vecHuntRewardItem = m_vecHuntRewardList;
    kData_.m_vecFinalRewardItem = m_vecFinalRewardList;
    kData_.m_prHuntItem = m_prHuntItem;
}

int KEclipsePlot::GetMaxProgress()
{
    KLocker lock( m_csEclipsePlot );
    return m_nMaxProgress;
}

void KEclipsePlot::GetHuntRewardItem( IN VEC_REWARD& vecItem_ )
{
    KLocker lock( m_csEclipsePlot );

    vecItem_ = m_vecHuntRewardList;
}

int KEclipsePlot::GetHuntItemCount()
{
    KLocker lock( m_csEclipsePlot );

    return m_prHuntItem.second;
}

void KEclipsePlot::GetFinalRewardItem( IN VEC_REWARD& vecItem_ )
{
    KLocker lock( m_csEclipsePlot );

    vecItem_ = m_vecFinalRewardList;
}

void KEclipsePlot::GetBeginTime( OUT time_t& tmDate_ )
{
    KLocker lock(m_csEclipsePlot);
    tmDate_ = m_tmBegin;
}