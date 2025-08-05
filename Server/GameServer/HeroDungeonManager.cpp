#include "GSSimLayer.h"
#include "HeroDungeonManager.h"
#include "Lua/KLuaManager.h"
#include <boost/bind.hpp>
#include <dbg/dbg.hpp >
#include "UserEvent.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KHeroDungeonManager );
ImplOstmOperatorW2A( KHeroDungeonManager );
NiImplementRootRTTI( KHeroDungeonManager );

ImplToStringW( KHeroDungeonManager )
{
    KLocker lock( m_csHeroDungeon );

    return START_TOSTRINGW
        << TOSTRINGW( m_bActiveMode )
        << TOSTRINGW( m_kPointItem.m_ItemID )
        << TOSTRINGW( m_kPointItem.m_nDuration )
        << TOSTRINGW( m_kPointItem.m_nPeriod )
        << TOSTRINGW( m_vecDungeonInfo.size() )
        << TOSTRINGW( m_vecOpenAlarm.size() )
        << TOSTRINGW( m_vecCloseAlarm.size() );
}

KHeroDungeonManager::KHeroDungeonManager(void)
:m_kTickCheck( 60 * 1000, false )
,m_fMonsterExpRatio(40.f)
,m_fDungeonWinClearExpRatio(40.f)
,m_fDungeonLoseClearExpRatio(10.f)
,m_bActiveMode(false)
,m_tmResetTime(1)
{
}

KHeroDungeonManager::~KHeroDungeonManager(void)
{
}

void KHeroDungeonManager::Tick()
{
    if ( !m_kTickCheck.CheckTime() ) {
        return;
    }

    DungeonAlarmNotice();

    m_kTickCheck.Reset();
}

void KHeroDungeonManager::DungeonAlarmNotice()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csHeroDungeon );

    if ( m_vecDungeonInfo.empty() ) {
        return;
    }

    std::vector<KHeroDungeon>::iterator vit = m_vecDungeonInfo.begin();
    std::map<USHORT,std::pair<time_t,time_t> >::iterator mit;
    for ( mit = vit->m_mapOpenTime.begin() ; mit != vit->m_mapOpenTime.end() ; ++mit ) {
        CheckAlarmTime( vit->m_nModeID, tmCurrent, mit->second );
    }
}

void KHeroDungeonManager::CheckAlarmTime( IN const int nModeID_, IN const time_t& tmCurrent_, IN const std::pair<time_t,time_t>& prOpenTime_ )
{
    // 알람시간 일치하면 패킷 전달
    CTime tmNow( tmCurrent_ );

    std::vector<int>::iterator vitOpen;
    for ( vitOpen = m_vecOpenAlarm.begin() ; vitOpen != m_vecOpenAlarm.end() ; ++vitOpen ) {
        CTime tmCheck = CTime( prOpenTime_.first ) - CTimeSpan( 0, 0, *vitOpen, 0 );
        if ( tmCheck.GetHour() == CTime( tmCurrent_ ).GetHour() && tmCheck.GetMinute() == CTime( tmCurrent_ ).GetMinute() ) {
            SendHeroDungeonAlarmNot( KEVENT_HERO_DUNGEON_ALARM_NOT::AT_ALARM_OPEN, nModeID_, *vitOpen, prOpenTime_ );
        }
    }

    std::vector<int>::iterator vitClose;
    for ( vitClose = m_vecCloseAlarm.begin() ; vitClose != m_vecCloseAlarm.end() ; ++vitClose ) {
        CTime tmCheck = CTime( prOpenTime_.second ) - CTimeSpan( 0, 0, *vitClose, 0 );
        if ( tmCheck.GetHour() == CTime( tmCurrent_ ).GetHour() && tmCheck.GetMinute() == CTime( tmCurrent_ ).GetMinute() ) {
            SendHeroDungeonAlarmNot( KEVENT_HERO_DUNGEON_ALARM_NOT::AT_ALARM_CLOSE, nModeID_, *vitClose, prOpenTime_ );
        }
    }
}

void KHeroDungeonManager::SendHeroDungeonAlarmNot( IN const USHORT usType_, IN const int nModeID_, IN const int nMin_, IN const std::pair<time_t,time_t>& prOpenTime_ )
{
    KEVENT_HERO_DUNGEON_ALARM_NOT kPacket;
    kPacket.m_usAlarmType = usType_;
    kPacket.m_nModeID = nModeID_;
    kPacket.m_nMin = nMin_;
    kPacket.m_prOpenTime = prOpenTime_;
    SiKGSSimLayer()->m_kActorManager.SendToAll( KUserEvent::EVENT_HERO_DUNGEON_ALARM_NOT, kPacket );
}

bool KHeroDungeonManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KHeroDungeonManager::_LoadScript( OUT std::string& strScript_ )
{
    _JIF( LoadHeroDungeon(strScript_), return false );
    const std::string strScriptName = "InitHeroIteminfo.lua";
    strScript_ = strScriptName;
    _JIF( m_kHeroPointItems.LoadScript( strScriptName ), return false );

	// 영던템 업글 스크립트 로드!
	_JIF( m_kHeroItemUpgradeManager.LoadScript( strScriptName ), return false );

    DungeonAlarmNotice();

    SendHeroDungeonInfoNot();

    return true;
}

bool KHeroDungeonManager::LoadHeroDungeon( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    bool bActive = false;
    KDropItemInfo kPointItem;
    time_t tmResetTime;
    std::vector<KHeroDungeon> vecDungeonInfo;
    std::vector<int> vecOpenAlarm;
    std::vector<int> vecCloseAlarm;
    float fMonsterExpRatio;
    float fDungeonWinClearExpRatio;
    float fDungeonLoseClearExpRatio;
    std::string strScriptName;

    if ( "" == strScript_ ) {
        strScriptName = "InitHeroDungeon.lua";
    }
    else {
        strScriptName = strScript_;
    }

    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( LoadAlarmTime( kLuaMng, "OpenAlarm", vecOpenAlarm ), return false );
    _JIF( LoadAlarmTime( kLuaMng, "CloseAlarm", vecCloseAlarm ), return false );

    _JIF( kLuaMng.GetValue( "MonsterExpRatio", fMonsterExpRatio ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "DungeonWinClearExpRatio", fDungeonWinClearExpRatio ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "DungeonLoseClearExpRatio", fDungeonLoseClearExpRatio ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "HeroDungeon" ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "ActiveMode", bActive ) == S_OK, return false );
    _JIF( LoadItemInfo( kLuaMng, "HeroPointItem", kPointItem ), return false );
    _JIF( LoadTime( kLuaMng, "ResetTime", tmResetTime ), return false );

    for ( int i = 1 ; ; ++i ) {
        KHeroDungeon kInfo;

        if ( kLuaMng.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng.GetValue( "ModeID", kInfo.m_nModeID ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "TicketID", kInfo.m_dwHeroTicketID ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "HeroPoint", kInfo.m_dwHeroPoint ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "MinLv", kInfo.m_dwMinCharacterLv ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "MaxClearCount", kInfo.m_usMaxClearPerDay ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "MonsterLv", kInfo.m_dwMonsterLv ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "ResetDifficulty", kInfo.m_bResetDifficulty ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "DifficultyCheck", kInfo.m_bDifficultyCheck ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "RewardCount", kInfo.m_usClearRewardCount ) == S_OK, return false );
        _JIF( LoadItemList( kLuaMng, "ClearReward", kInfo.m_mapClearReward ), return false );
        _JIF( LoadTimeTable( kLuaMng, "OpenTime", kInfo.m_mapOpenTime ), return false );
        _JIF( kLuaMng.GetValue( "MinUserGrade", kInfo.m_nMinUserGrade ) == S_OK, return false );
        _JIF( kLuaMng.EndTable() == S_OK, return false );

        std::vector<KHeroDungeon>::iterator vit;
        vit = std::find_if( vecDungeonInfo.begin(), vecDungeonInfo.end(),
            boost::bind(&KHeroDungeon::m_nModeID,_1) == kInfo.m_nModeID );
        if ( vit != vecDungeonInfo.end() ) {
            START_LOG( cerr, L"중복된 ModeID 입니다. ModeID : " << kInfo.m_nModeID ) << END_LOG;
            return false;
        }

        vecDungeonInfo.push_back( kInfo );
    }

    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L"Load HeroDungeon script.. IsActive : " << bActive )
        << BUILD_LOG( kPointItem.m_ItemID )
        << BUILD_LOG( fMonsterExpRatio )
        << BUILD_LOG( fDungeonWinClearExpRatio )
        << BUILD_LOG( fDungeonLoseClearExpRatio )
        << BUILD_LOG( vecDungeonInfo.size() )
        << BUILD_LOG( vecOpenAlarm.size() )
        << BUILD_LOG( vecCloseAlarm.size() ) << END_LOG;

    KLocker lock( m_csHeroDungeon );
    m_bActiveMode = bActive;
    m_kPointItem = kPointItem;
    m_tmResetTime = tmResetTime;
    m_vecDungeonInfo.swap( vecDungeonInfo );
    m_vecOpenAlarm.swap( vecOpenAlarm );
    m_vecCloseAlarm.swap( vecCloseAlarm );
    m_fMonsterExpRatio = fMonsterExpRatio;
    m_fDungeonWinClearExpRatio = fDungeonWinClearExpRatio;
    m_fDungeonLoseClearExpRatio = fDungeonLoseClearExpRatio;
    return true;
}

bool KHeroDungeonManager::LoadAlarmTime( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT std::vector<int>& vecAlarm_ )
{
    vecAlarm_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        int nMin = 0;
        if ( kLuaMng_.GetValue( i, nMin ) != S_OK ) break;
        vecAlarm_.push_back( nMin );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KHeroDungeonManager::LoadItemInfo( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT KDropItemInfo& kItem_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, kItem_.m_ItemID ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, kItem_.m_nDuration ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, kItem_.m_nPeriod ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KHeroDungeonManager::LoadTime( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT time_t& tmResetTime_ )
{
    int nHour, nMin;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMin ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    CTime tmToday( CTime::GetCurrentTime() );
    tmResetTime_ = KncUtil::TimeToInt( CTime( tmToday.GetYear(), tmToday.GetMonth(), tmToday.GetDay(), nHour, nMin, 0 ) );
    return true;
}

bool KHeroDungeonManager::LoadItemList( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT std::map<INDEX_GCITEMID,KManufactureItem>& mapList_ )
{
    mapList_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for ( int i = 1 ; ; ++i ) {

        KManufactureItem kItem;
        bool bIsLook = false;
        USHORT usIndex = static_cast<USHORT>(i-1);

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( "ItemID", kItem.m_ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Count", kItem.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Period", kItem.m_nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "IsLook", bIsLook ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( bIsLook ) {
            kItem.SetItemFlag( KManufactureItem::IF_LOOK );
        }

        mapList_[ std::make_pair(usIndex,kItem.m_ItemID) ] = kItem;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KHeroDungeonManager::LoadTimeTable( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT std::map<USHORT,std::pair<time_t,time_t> >& mapTime_ )
{
    mapTime_.clear();

    CTime tmToday( CTime::GetCurrentTime() );

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int nIndex = 0;
    for ( int i = 1 ; ; ++i, ++nIndex ) {
        int nOpenHour, nOpenMin, nCloseHour, nCloseMin;
        time_t tmOpen, tmClose;

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, nOpenHour ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nOpenMin ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, nCloseHour ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, nCloseMin ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        tmOpen = KncUtil::TimeToInt( CTime( tmToday.GetYear(), tmToday.GetMonth(), tmToday.GetDay(), nOpenHour, nOpenMin, 0 ) );
        tmClose = KncUtil::TimeToInt( CTime( tmToday.GetYear(), tmToday.GetMonth(), tmToday.GetDay(), nCloseHour, nCloseMin, 0 ) );

        mapTime_[nIndex] = std::make_pair(tmOpen,tmClose);
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KHeroDungeonManager::DumpItemList()
{
}

void KHeroDungeonManager::DumpDungeonInfo( IN const int nModeID )
{
}

bool KHeroDungeonManager::IsHeroDungeon( IN const int nModeID_ )
{
    KLocker lock( m_csHeroDungeon );
    std::vector<KHeroDungeon>::const_iterator cvit;
    cvit = std::find_if(m_vecDungeonInfo.begin(), m_vecDungeonInfo.end(), [&](const KHeroDungeon& o) { return o.m_nModeID == nModeID_; });
    return ( cvit != m_vecDungeonInfo.end() );
}

bool KHeroDungeonManager::GetHeroDungeonInfo( IN const int nModeID_, OUT KHeroDungeon& kInfo_ )
{
    KLocker lock( m_csHeroDungeon );
    std::vector<KHeroDungeon>::const_iterator cvit;
    cvit = std::find_if(m_vecDungeonInfo.begin(), m_vecDungeonInfo.end(), [&](const KHeroDungeon& o) { return o.m_nModeID == nModeID_; });
    if ( cvit == m_vecDungeonInfo.end() ) {
        START_LOG( cwarn, L"영웅던전 ModeID가 아닙니다. ModeID : " << nModeID_ ) << END_LOG;
        return false;
    }
    kInfo_ = *cvit;
    return true;
}

void KHeroDungeonManager::GetHeroDungeonList( OUT std::vector<KHeroDungeon>& vecList_ )
{
    KLocker lock( m_csHeroDungeon );
    vecList_ = m_vecDungeonInfo;
}

void KHeroDungeonManager::GetPointItem( OUT KDropItemInfo& kItem_ )
{
    KLocker lock( m_csHeroDungeon );
    kItem_ = m_kPointItem;
}

void KHeroDungeonManager::GetHeroDungeonIDList( OUT std::set<int>& setModeID_ )
{
    KLocker lock( m_csHeroDungeon );

    std::vector<KHeroDungeon>::const_iterator cvit;
    for ( cvit = m_vecDungeonInfo.begin() ; cvit != m_vecDungeonInfo.end() ; ++cvit ) {
        setModeID_.insert( cvit->m_nModeID );
    }
}
DWORD KHeroDungeonManager::GetHeroDungeonMinLv( IN int const nModeID_ )
{
    KLocker lock( m_csHeroDungeon );
    std::vector<KHeroDungeon>::iterator vit;
    vit = std::find_if( m_vecDungeonInfo.begin(), m_vecDungeonInfo.end(), [&]( const KHeroDungeon& o ) { return o.m_nModeID == nModeID_; } );
    if  (vit != m_vecDungeonInfo.end() )
    {
        return vit->m_dwMinCharacterLv;
    }
    return 0;
}

bool KHeroDungeonManager::IsHeroDungeonDifficultyCheck( IN const int nModeID_ )
{
    KLocker lock( m_csHeroDungeon );
    std::vector<KHeroDungeon>::const_iterator cvit;
    cvit = std::find_if(m_vecDungeonInfo.begin(), m_vecDungeonInfo.end(), [&](const KHeroDungeon& o) { return o.m_nModeID == nModeID_; });
    if ( cvit == m_vecDungeonInfo.end() ) {
        START_LOG( cwarn, L"영웅던전 ModeID가 아닙니다. ModeID : " << nModeID_ ) << END_LOG;
        return false;
    }
    return cvit->m_bDifficultyCheck;
}

bool KHeroDungeonManager::SendHeroDungeonInfoNot()
{
    DWORD dwTemp = 0;
    SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::EVENT_UPDATEPLAN_HERO_DUNGEON_INFO_NOT, dwTemp );
    return true;
}
