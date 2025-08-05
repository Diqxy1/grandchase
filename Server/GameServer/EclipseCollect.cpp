#include "EclipseCollect.h"
#include <dbg/dbg.hpp>
#include "Lua/KLuaManager.h"
#include <kncutil.h>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KEclipseCollect );
ImplOstmOperatorW2A( KEclipseCollect );
NiImplementRootRTTI( KEclipseCollect );

KEclipseCollect::KEclipseCollect(void)
:m_tmBegin(1)
,m_tmEnd(1)
,m_dwVersion(0)
{
    m_setEclipseCollectItemList.clear();
    m_vecRewardItem.clear();
}

KEclipseCollect::~KEclipseCollect(void)
{
}

ImplToStringW( KEclipseCollect )
{
    KLocker lock( m_csEclipse );
    return START_TOSTRINGW
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGW( m_dwVersion )
        << TOSTRINGW( m_setEclipseCollectItemList.size() )
        << TOSTRINGW( m_vecRewardItem.size() );
}

bool KEclipseCollect::LoadScript()
{
    return _LoadScript(std::string());
}

bool KEclipseCollect::_LoadScript( OUT std::string& strScript_ )
{
    time_t      tmBegin;
    time_t      tmEnd;
    DWORD       dwVersion;
    std::set<GCITEMID> setEclipseCollectItemList;
    VEC_REWARD vecRewardItem;

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitEclipseCollect.lua";
    strScript_ = strScriptName;

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    // 이벤트 기간 읽어오기.
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );

    // 이벤트 설정값 읽어오기.
    _JIF( kLuaMng.GetValue( "EclipseCollectVersion", dwVersion ) == S_OK, return false );
    _JIF( LoadEclipseCollectItemList( kLuaMng, std::string("EclipseCollectItemList"), setEclipseCollectItemList ), return false );
    _JIF( LoadReward( kLuaMng, std::string("RewardItemList"), vecRewardItem ), return false );

    START_LOG( cerr, L"개기일식 고서 모으기 이벤트 스크립트 로드 완료. " ) << END_LOG;
    {
        KLocker lock( m_csEclipse );
        m_tmBegin = tmBegin;
        m_tmEnd = tmEnd;
        m_dwVersion = dwVersion;
        m_setEclipseCollectItemList.swap( setEclipseCollectItemList );
        m_vecRewardItem.swap( vecRewardItem );

        return true;
    }
}

bool KEclipseCollect::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
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

bool KEclipseCollect::LoadEclipseCollectItemList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set<GCITEMID>& setEclipseCollectItemList_ )
{
    setEclipseCollectItemList_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true ) {
        DWORD dwData;
        if( kLuaMng_.GetValue( i++, dwData ) != S_OK ) break;
        setEclipseCollectItemList_.insert( dwData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KEclipseCollect::LoadReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT VEC_REWARD& vecRewards_ )
{
    vecRewards_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true ) {
        KDropItemInfo kData;
        if( kLuaMng_.GetValue( i++, kData.m_ItemID) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kData.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kData.m_nPeriod ) == S_OK, return false );
        vecRewards_.push_back( kData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KEclipseCollect::IsRun()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csEclipse );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

void KEclipseCollect::GetEclipseCollectItemList( OUT std::set<GCITEMID>& setEclipseCollectItemList_ )
{
    setEclipseCollectItemList_.clear();

    KLocker lock( m_csEclipse );
    setEclipseCollectItemList_ = m_setEclipseCollectItemList;
}

void KEclipseCollect::GetRewardItem( OUT VEC_REWARD& vecRewardItem_ )
{
    vecRewardItem_.clear();

    KLocker lock( m_csEclipse );
    vecRewardItem_ = m_vecRewardItem;
}

void KEclipseCollect::GetBeginTime( OUT time_t& tmDate_ )
{
    KLocker lock(m_csEclipse);
    tmDate_ = m_tmBegin;
}
