#include "CenterSimLayer.h"
#include "CnGWCEvent.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "CenterDBThread.h"
#include "CenterServer.h"

//FILE_NAME_FOR_LOG

ImplementSingleton( KCnGWCEvent );
ImplOstmOperatorW2A( KCnGWCEvent );
NiImplementRootRTTI( KCnGWCEvent );

KCnGWCEvent::KCnGWCEvent(void)
:m_tmBegin(1)
,m_tmEnd(1)
,m_dwTickGap(60000)
,m_dwVersion(0)
,m_bDBChanged(false)
,m_bNoticeChanged(false)
,m_dwCountryID(0)
,m_dwLastTick(0)
{
    // Init.
    m_mapGradeRewardInfo.clear();
    m_mapRankingRewardInfo.clear();
    m_mapTotalCountryInfo.clear();
    m_mapGradeDesc.clear();

    // DB 데이터 가져오는 시간을 00시로 초기화.
    CTime tmCurrent = CTime::GetCurrentTime();
    CTime tmUpdate( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), 0, 0, 0 );
    m_tmDBUpdateTime = KncUtil::TimeToInt( tmUpdate );
}

KCnGWCEvent::~KCnGWCEvent(void)
{
}

ImplToStringW( KCnGWCEvent )
{
    KLocker lock( m_csCnGWCEvent );
    return START_TOSTRINGW
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGW( m_bDBChanged )
        << TOSTRINGW( m_dwTickGap )
        << TOSTRINGW( m_dwLastTick )
        << TOSTRINGW( m_mapGradeRewardInfo.size() )
        << TOSTRINGW( m_mapRankingRewardInfo.size() )
        << TOSTRINGW( m_mapTotalCountryInfo.size() )
        << TOSTRINGW( m_mapGradeDesc.size() );
}

bool KCnGWCEvent::LoadScript()
{
    KLuaManager     kLuaMng;
    time_t          tmBegin;
    time_t          tmEnd;
    DWORD           dwTickGap;
    int             dwVersion;
    DWORD           dwCountryID;
    std::wstring    wstrCountryName;
    std::map<DWORD, std::pair<float,float>>         mapGradeRewardInfo;
    std::map<DWORD, VEC_ITEMS>                      mapRankingRewardInfo;
    std::map<DWORD,std::pair<DWORD,DWORD>>          mapGradeDesc;

    _JIF( kLuaMng.DoFile( "InitGWCEvent_2011.lua" ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    _JIF( kLuaMng.GetValue( "Version", dwVersion ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "TickGap", dwTickGap ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "CountryID", dwCountryID ) == S_OK, return false );
    std::string strMsgID;
    _JIF( kLuaMng.GetValue( "CountryStringID", strMsgID ) == S_OK, return false );
    wstrCountryName = SiKCenterServer()->m_stringTable.GetValue( KncUtil::toWideString( strMsgID ).c_str() );

    // 등급에 따른 보상 정보 Load.
    _JIF( LoadGradeRewardInfo( kLuaMng, std::string( "GradeRewardInfo" ), mapGradeRewardInfo ), return false );
    // 순위에 따른 보상 정보 Load.
    _JIF( LoadRankingRewardInfo( kLuaMng, std::string( "RankingRewardInfo" ), mapRankingRewardInfo ), return false );
    // 점수에 따른 등급 정보 Load.
    _JIF( LoadGradeDescInfo( kLuaMng, std::string( "GradeDesc" ), mapGradeDesc ), return false );

    START_LOG( clog, L"센터 GWC 이벤트 스크립트 읽기 완료." ) << END_LOG;
    {
        KLocker lock( m_csCnGWCEvent );
        m_tmBegin           = tmBegin;
        m_tmEnd             = tmEnd;
        m_dwVersion         = dwVersion;
        m_dwTickGap         = dwTickGap;
        m_dwCountryID       = dwCountryID;
        m_wstrCountryName   = wstrCountryName;
        m_mapGradeRewardInfo.swap( mapGradeRewardInfo );
        m_mapRankingRewardInfo.swap( mapRankingRewardInfo );
        m_mapGradeDesc.swap( mapGradeDesc );
        m_bDBChanged = true;
    }

    return true;
}

bool KCnGWCEvent::LoadGradeRewardInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<DWORD, std::pair<float,float>>& mapGradeRewardInfo_ )
{
    mapGradeRewardInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        DWORD dwGradeID;
        std::pair<float,float> prData;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "GradeID", dwGradeID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "ExpReward", prData.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "GPReward", prData.second ) == S_OK, return false );

        if( !mapGradeRewardInfo_.insert( std::make_pair( dwGradeID, prData ) ).second ) {
            START_LOG( cerr, L" 중복된 등급 정보가 있음. GraneID : " << dwGradeID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnGWCEvent::LoadRankingRewardInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<DWORD, VEC_ITEMS>& mapRankingRewardInfo_ )
{
    mapRankingRewardInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        DWORD dwRankingNum;
        VEC_ITEMS vecItems;
        vecItems.clear();

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "RankingNum", dwRankingNum ) == S_OK, return false );
        _JIF( LoadItems( kLuaMng_, std::string("RewardInfo"), vecItems ), return false );

        if( !mapRankingRewardInfo_.insert( std::make_pair( dwRankingNum, vecItems ) ).second ) {
            START_LOG( cerr, L" 중복된 랭킹 정보가 있음. RankingNum : " << dwRankingNum ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnGWCEvent::LoadGradeDescInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<DWORD,std::pair<DWORD,DWORD>>& mapGradeDesc_ )
{
    mapGradeDesc_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        DWORD dwGradeID;
        std::pair<DWORD,DWORD> prData;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "GradeID", dwGradeID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "StratPoint", prData.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "EndPoint", prData.second ) == S_OK, return false );

        if( !mapGradeDesc_.insert( std::make_pair( dwGradeID, prData ) ).second ) {
            START_LOG( cerr, L" 중복된 등급 Desc 정보가 있음. GraneID : " << dwGradeID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnGWCEvent::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nYear, nMonth, nDay, nHour;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    tmTime_ = KncUtil::TimeToInt( CTime( nYear, nMonth, nDay, nHour, 0, 0 ) );

    return true;
}

bool KCnGWCEvent::LoadItems( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT VEC_ITEMS& vecRewards_ )
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

void KCnGWCEvent::Tick()
{
    if ( ::GetTickCount() - m_dwLastTick < m_dwTickGap ) {
        return;
    }
    m_dwLastTick = ::GetTickCount();

    if ( CheckDBUpdate() ) {
        SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_GWC_EVENT_DATA_REQ, L"", 0 );
    }

    if ( CheckDBChange() ) {
        KECN_GWC_EVENT_DATA_NOT kPacket;
        GetGWCEventData( kPacket );
        SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_GWC_EVENT_DATA_NOT, kPacket );
    }

    if ( CheckNoticeChange() ) {
        SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_GWC_NOTICE_NOT, m_dwCountryID );
    }
}

bool KCnGWCEvent::CheckDBUpdate()
{
    KLocker lock( m_csCnGWCEvent );
    CTime tmCurrent = CTime::GetCurrentTime();
    CTime tmUpdate( m_tmDBUpdateTime );
    if( tmUpdate < tmCurrent ) {
        CTimeSpan ts(1,0,0,0); // 내일.
        tmUpdate += ts;
        m_tmDBUpdateTime = KncUtil::TimeToInt( tmUpdate );
        return true;
    }

    return false;
}

void KCnGWCEvent::UpdateGWCTotalCountryInfo( IN std::map<DWORD, KGWCCountryInfo>& mapTotalCountryInfo_, IN std::vector<std::pair<DWORD,DWORD>>& vecRankingInfo_ )
{
    KLocker lock( m_csCnGWCEvent );
    ModifyTotalCountryInfo( mapTotalCountryInfo_, vecRankingInfo_ );
    m_mapTotalCountryInfo.swap( mapTotalCountryInfo_ );
    m_bDBChanged = true;
    m_bNoticeChanged = true;
}

void KCnGWCEvent::ModifyTotalCountryInfo( IN OUT std::map<DWORD, KGWCCountryInfo>& mapTotalCountryInfo_, IN std::vector<std::pair<DWORD,DWORD>>& vecRankingInfo_ )
{
    DWORD dwGrade = 0;
    DWORD dwRanking = 0;
    std::map<DWORD, KGWCCountryInfo>::iterator mit;
    std::vector<std::pair<DWORD,DWORD>>::iterator vit;
    std::map<DWORD,DWORD> mapCountryRanking;
    std::map<DWORD,DWORD>::iterator mitRanking;
    mapCountryRanking.clear();

    for( vit = vecRankingInfo_.begin(); vit != vecRankingInfo_.end(); ++vit ) {
        DWORD dwGradeNum = 0;
        if( GetCurrentGrade( vit->second, dwGradeNum ) ) {
            if( dwGrade != dwGradeNum ) {
                dwRanking += 1;
            }
            dwGrade = dwGradeNum;
            mapCountryRanking[vit->first] = dwRanking;
        }
    }

    for( mit = mapTotalCountryInfo_.begin(); mit != mapTotalCountryInfo_.end(); ++mit ) {
        if( GetCurrentGrade( mit->second.m_dwTotalCnt, mit->second.m_dwGrade ) ) {
            mitRanking = mapCountryRanking.find( mit->first );
            if( mitRanking != mapCountryRanking.end() ) {
                mit->second.m_dwRanking = mitRanking->second;
            }
        }
    }
}

bool KCnGWCEvent::CheckDBChange()
{
    KLocker lock( m_csCnGWCEvent );
    if ( m_bDBChanged ) {
        m_bDBChanged = false;
        return true;
    }
    return false;
}

void KCnGWCEvent::GetGWCEventData( OUT KECN_GWC_EVENT_DATA_NOT& kPacket_ )
{
    KLocker lock( m_csCnGWCEvent );
    kPacket_.m_wstrCountryName = m_wstrCountryName;
    kPacket_.m_dwCountryID = m_dwCountryID;
    kPacket_.m_tmBegin = m_tmBegin;
    kPacket_.m_tmEnd = m_tmEnd;
    kPacket_.m_tmDBUpdateTime = m_tmDBUpdateTime;
    kPacket_.m_mapGradeRewardInfo = m_mapGradeRewardInfo;
    kPacket_.m_mapRankingRewardInfo = m_mapRankingRewardInfo;
    kPacket_.m_mapTotalCountryInfo = m_mapTotalCountryInfo;
}

bool KCnGWCEvent::CheckNoticeChange()
{
    KLocker lock( m_csCnGWCEvent );
    if ( m_bNoticeChanged ) {
        m_bNoticeChanged = false;
        return true;
    }
    return false;
}

bool KCnGWCEvent::IsRun() const
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csCnGWCEvent );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

bool KCnGWCEvent::GetCurrentGrade( IN OUT DWORD& dwCountryCnt_, OUT DWORD& dwGrade_ )
{
    std::map<DWORD,std::pair<DWORD,DWORD>>::iterator mit;
    std::map<DWORD,std::pair<DWORD,DWORD>>::reverse_iterator rmit;

    KLocker lock( m_csCnGWCEvent );
    for( mit = m_mapGradeDesc.begin(); mit != m_mapGradeDesc.end(); ++mit ) {
        if( mit->second.first <= dwCountryCnt_ && mit->second.second >= dwCountryCnt_ ) {
            dwGrade_ = mit->first;
            return true;
        } // if
    } // for

    if ( m_mapGradeDesc.empty() ) {
        return false;
    }

    rmit = m_mapGradeDesc.rbegin();
    if( dwCountryCnt_ > rmit->second.second ) {
        dwCountryCnt_ = rmit->second.second;
        dwGrade_ = rmit->first;
        return true;
    }

    return false;
}