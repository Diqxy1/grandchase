#include "CenterSimLayer.h"
#include "CnUpdatePlanManager.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

ImplementSingleton( KCnUpdatePlanManager );
ImplOstmOperatorW2A( KCnUpdatePlanManager );
NiImplementRootRTTI( KCnUpdatePlanManager );

KCnUpdatePlanManager::KCnUpdatePlanManager(void)
: m_kUpdateCheckTimer( 60 * 1000 )
, m_nLoadedIndex( 1 )
{
    m_vecICCUpdatePlan.clear();
    m_mapICCInfo.clear();
    m_vecEventUpdatePlan.clear();
    m_vecScriptUpdatePlan.clear();
    m_mapScriptInfoForGameServerSend.clear();
}

KCnUpdatePlanManager::~KCnUpdatePlanManager(void)
{
}

ImplToStringW( KCnUpdatePlanManager )
{
    START_TOSTRINGW;

    {
        KLocker lock( m_csICCUpdatePlan );

        stm_ << TOSTRINGW( m_vecICCUpdatePlan.size() );

        stm_ << L"-------------- m_vecICCUpdatePlan --------------" << std::endl;
        BOOST_FOREACH( const KICCUpdatePlan& rICCUpdatePlan, m_vecICCUpdatePlan ) {
            stm_ << L"CheckTime: " << ( LPCWSTR )( CTime ( rICCUpdatePlan.m_tmCheckTime ) ).Format( KNC_TIME_FORMAT ) << std::endl;
            stm_ << L"FileName: " << KncUtil::toWideString( rICCUpdatePlan.m_strFileName.c_str() ) << std::endl;
        }
    }

    {
        KLocker lock( m_csScriptUpdatePlan );

        stm_ << TOSTRINGW( m_vecScriptUpdatePlan.size() );

        stm_ << L"-------------- m_vecScriptUpdatePlan --------------" << std::endl;
        BOOST_FOREACH( const KScriptUpdatePlan& rScriptUpdatePlan, m_vecScriptUpdatePlan ) {
            stm_ << L"CheckTime: " << ( LPCWSTR )( CTime ( rScriptUpdatePlan.m_tmCheckTime ) ).Format( KNC_TIME_FORMAT ) << std::endl;
            BOOST_FOREACH( const PAIR_INT_STRING& rScriptInfo, rScriptUpdatePlan.m_vecScriptInfo ) {
                stm_ << L"EventID: " << rScriptInfo.first << std::endl;
                stm_ << L"FileName: " << KncUtil::toWideString( rScriptInfo.second.c_str() ) << std::endl;
            }
        }
    }

    return stm_;
}

bool KCnUpdatePlanManager::LoadScript()
{
    KLuaManager kLuaMng;

    _JIF( kLuaMng.DoFile( "InitCnUpdatePlanManager.lua" ) == S_OK, return false );

    _JIF( LoadICCUpdatePlan( kLuaMng ), return false );
    _JIF( LoadEventUpdatePlan( kLuaMng ), return false );
    _JIF( LoadScriptUpdatePlan( kLuaMng ), return false );

    return true;
}

bool KCnUpdatePlanManager::LoadICCUpdatePlan( IN KLuaManager& kLuaMng_ )
{
    std::vector< KICCUpdatePlan > vecICCUpdatePlan;
    vecICCUpdatePlan.clear();
    std::map< int, std::pair< time_t, KCnClientContentsInfo > > mapICCInfo;
    mapICCInfo.clear();

    _JIF( kLuaMng_.BeginTable( "ICCUpdatePlan" ) == S_OK, return false );

    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KICCUpdatePlan kICCUpdatePlan;
        _JIF( LoadTime( kLuaMng_, std::string( "CheckTime" ), kICCUpdatePlan.m_tmCheckTime ), return false );
        _JIF( kLuaMng_.GetValue( "FileName", kICCUpdatePlan.m_strFileName ) == S_OK, return false );

        vecICCUpdatePlan.push_back( kICCUpdatePlan );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );  // i
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // UpdatePlan

    // 혹시나 설정 실수로 날짜가 정렬되어 있지 않을 수 있으니 정렬을 한번 해준다
    std::sort( vecICCUpdatePlan.begin(), vecICCUpdatePlan.end(),
        boost::bind( &KICCUpdatePlan::m_tmCheckTime, _1 ) < boost::bind( &KICCUpdatePlan::m_tmCheckTime, _2 ) );

    // ICC 스크립트들에 오류가 발생하는 경우를 발견하기 쉽도록
    // 매니저 스크립트를 읽는 시점에 지정된 ICC 스크립트들도 전부 읽어둔다
    int nIndex = 1;
    BOOST_FOREACH( KICCUpdatePlan& rICCUpdatePlan, vecICCUpdatePlan ) {
        // 각 스크립트 로드
        KCnClientContentsInfo kCnClientContentsInfo;
        if ( false == kCnClientContentsInfo.LoadScript( rICCUpdatePlan.m_strFileName ) ) {
            // 오류가 있는 스크립트가 하나라도 있다면 전체 로드를 실패처리한다
            START_LOG( cerr, L"InitClientContents 스크립트 로드 실패: " << KncUtil::toWideString( rICCUpdatePlan.m_strFileName.c_str() ) ) << END_LOG;

            return false;
        }

        std::pair< time_t, KCnClientContentsInfo > prICCInfo( rICCUpdatePlan.m_tmCheckTime, kCnClientContentsInfo );

        mapICCInfo.insert( std::make_pair( nIndex, prICCInfo ) );
        nIndex++;
    }

    {
        KLocker lock( m_csICCUpdatePlan );

        m_vecICCUpdatePlan.swap( vecICCUpdatePlan );
    }
    {
        KLocker lock( m_csICCInfo );

        // 최소 하나의 ICC 스크립트는 읽어야 한다
        if ( true == mapICCInfo.empty() ) {
            return false;
        }

        m_mapICCInfo.swap( mapICCInfo );

        m_nLoadedIndex = m_mapICCInfo.begin()->first;
        m_kUsingICC = m_mapICCInfo.begin()->second.second;
    }

    // 스크립트 로드 후 전체 정보를 패킷으로 알린다
    SendCurrentICCFullPacket();

    return true;
}

bool KCnUpdatePlanManager::LoadEventUpdatePlan( IN KLuaManager& kLuaMng_ )
{
    std::vector<KEventUpdatePlan> vecEventPlan;

    _JIF( kLuaMng_.BeginTable( "EventUpdatePlan" ) == S_OK, return false );

    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KEventUpdatePlan kEventPlan;
        _JIF( kLuaMng_.GetValue( "EventID", kEventPlan.m_nEventID ) == S_OK, return false );
        _JIF( LoadTime( kLuaMng_, std::string( "BeginTime" ), kEventPlan.m_tmBeginTime ), return false );
        _JIF( LoadTime( kLuaMng_, std::string( "EndTime" ), kEventPlan.m_tmEndTime ), return false );

        vecEventPlan.push_back( kEventPlan );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }

    {
        KLocker lock( m_csEventUpdatePlan );

        m_vecEventUpdatePlan.swap( vecEventPlan );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"EventUpdatePlan 테이블 로드 완료" ) << END_LOG;

    return true;
}

bool KCnUpdatePlanManager::LoadScriptUpdatePlan( IN KLuaManager& kLuaMng_ )
{
    std::vector< KScriptUpdatePlan > vecScriptUpdatePlan;

    vecScriptUpdatePlan.clear();

    _JIF( kLuaMng_.BeginTable( "ScriptUpdatePlan" ) == S_OK, return false );

    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KScriptUpdatePlan kScriptplan;

        _JIF( LoadTime( kLuaMng_, std::string( "CheckTime" ), kScriptplan.m_tmCheckTime ), return false );
        _JIF( LoadScriptInfo( kLuaMng_, std::string( "ScriptInfo" ), kScriptplan.m_vecScriptInfo ), return false );
        kScriptplan.m_bUsed = false;

        vecScriptUpdatePlan.push_back( kScriptplan );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }

    {
        KLocker lock( m_csScriptUpdatePlan );

        m_vecScriptUpdatePlan.swap( vecScriptUpdatePlan );
        m_mapScriptInfoForGameServerSend.clear();
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"ScriptUpdatePlan 테이블 로드 완료" ) << END_LOG;

    return true;
}

bool KCnUpdatePlanManager::LoadScriptInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< PAIR_INT_STRING >& vecScriptInfo_ )
{
    vecScriptInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true ) {
        if ( kLuaMng_.BeginTable( i++ ) != S_OK ) break;

        PAIR_INT_STRING prScriptInfo;

        int j = 1;
        _JIF( kLuaMng_.GetValue( j++, prScriptInfo.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( j++, prScriptInfo.second ) == S_OK, return false );

        vecScriptInfo_.push_back( prScriptInfo );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCnUpdatePlanManager::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nYear, nMonth, nDay, nHour, nMin;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 5, nMin ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    if ( 0 == nYear || 0 == nMonth || 0 == nDay // 스크립트에서 0으로 설정하면 서버 죽음
        || nMonth > 12 || nDay > 31 ) { // 월, 일 최대값 검사
            START_LOG( cerr, L"스크립트 날짜 정보 오류" ) << END_LOG;
            return false;
    }

    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, nMin, 0 ) );

    return true;
}

void KCnUpdatePlanManager::Tick()
{
    if ( false == m_kUpdateCheckTimer.CheckTime() ) {
        return;
    }

    CTime ctmCurrent = CTime::GetCurrentTime();
    time_t tmCurrent = KncUtil::TimeToInt( CTime( ctmCurrent.GetYear(), ctmCurrent.GetMonth(), ctmCurrent.GetDay(), ctmCurrent.GetHour(), ctmCurrent.GetMinute(), 0 ) );

    // 모든 업데이트 판단에서 같은 시간을 기준으로 사용할 수 있도록 한다
    CheckICCUpdatePlan( tmCurrent );
    CheckEventUpdatePlan( tmCurrent );
    CheckScriptUpdatePlan( tmCurrent );
}

bool KCnUpdatePlanManager::GetUsingICC( OUT KCnClientContentsInfo& kCnClientContentsInfo_ )
{
    KLocker lock( m_csICCInfo );

    kCnClientContentsInfo_ = m_kUsingICC;

    return true;
}

bool KCnUpdatePlanManager::GetScriptUpdateInfo( OUT KECN_PLAN_SCRIPT_UPDATE_NOT& kPacket_ )
{
    std::map< int, std::string >::iterator mitScriptInfo;
    kPacket_.clear();

    KLocker lock( m_csScriptUpdatePlan );

    // map 에서 패킷데이터인 vector로 재가공
    for ( mitScriptInfo = m_mapScriptInfoForGameServerSend.begin(); mitScriptInfo != m_mapScriptInfoForGameServerSend.end(); mitScriptInfo++ ) {
        PAIR_INT_STRING prScriptInfo( mitScriptInfo->first, mitScriptInfo->second );
        
        kPacket_.push_back( prScriptInfo );
    }

    return true;
}

void KCnUpdatePlanManager::CheckICCUpdatePlan( IN const time_t tmCurrent_ )
{
    std::pair< time_t, KCnClientContentsInfo > prTempICCInfo;
    int nNextIndex = 0;

    {
        KLocker lock( m_csICCInfo );

        // 사용할 다음 ICC 스크립트 정보가 있는지 먼저 확인한다
        nNextIndex = m_nLoadedIndex + 1;

        typedef std::map< int, std::pair< time_t, KCnClientContentsInfo > > mapICCInfoType;
        mapICCInfoType::iterator mitICCInfo( m_mapICCInfo.find( nNextIndex ) ); // 이 부분의 특징으로 Tick마다 한 단계식 옮겨가게 된다
        if ( mitICCInfo == m_mapICCInfo.end() ) {
            return;
        }

        prTempICCInfo = mitICCInfo->second;
    }

    // 사용할 수 있는 다음 ICC 스크립트 정보가 있는 경우
    // 현재 날짜와 비교하여 m_kUsingICC 정보에 옮겨담고
    // 게임 서버들에 변경된 내용만 패킷으로 전달한다(추후 필요하다면 센터 서버에 접속중인 클라들에게도 전달한다)
    if ( tmCurrent_ >= prTempICCInfo.first ) {
        KCnClientContentsInfo kTempUsingICC;

        {
            KLocker lock( m_csICCInfo );

            kTempUsingICC = m_kUsingICC;
            m_kUsingICC = prTempICCInfo.second;
            m_nLoadedIndex = nNextIndex;
        }

        SendClientContentsOpenInfoNot( kTempUsingICC, prTempICCInfo.second );
        SendClientContentsFirstInitInfoNot( kTempUsingICC, prTempICCInfo.second );
    }
}

void KCnUpdatePlanManager::CheckEventUpdatePlan( IN const time_t tmCurrent_ )
{
    std::vector<KEventUpdatePlan>::iterator vitEvent;
    bool bChanged = false;

    KLocker lock( m_csEventUpdatePlan );

    for( vitEvent = m_vecEventUpdatePlan.begin(); vitEvent != m_vecEventUpdatePlan.end(); vitEvent++ ) {
        // 현재 시각에 시작 되는 이벤트인지 확인 ( end상태에서도 스크립트 기간 변경으로 다시 시작 가능하게 going이 아닌지 조건으로 체크)
        if ( tmCurrent_ >= vitEvent->m_tmBeginTime && KEventUpdatePlan::EVENT_STATE_GOING != vitEvent->m_nEventState ) {
            vitEvent->m_nEventState = KEventUpdatePlan::EVENT_STATE_GOING;
            bChanged = true;
        } // 현재 진행 중인 이벤트가 끝나는지 확인
        else if ( tmCurrent_ > vitEvent->m_tmEndTime && KEventUpdatePlan::EVENT_STATE_GOING == vitEvent->m_nEventState ) {
            vitEvent->m_nEventState = KEventUpdatePlan::EVENT_STATE_END;
            bChanged = true;
        }
    }

    // 이벤트 시작/종료 상태의 변경이 있으면 게임서버 -> 유저 보내자
    if ( true == bChanged ) {
        //SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_EVENT_CHANGED_EVENT_STATE_INFO_NOT, m_vecEventUpdatePlan );
    }
}

void KCnUpdatePlanManager::CheckScriptUpdatePlan( IN const time_t tmCurrent_ )
{
    std::vector< KScriptUpdatePlan >::iterator vitUpdateScriptPlan;

    KLocker lock( m_csScriptUpdatePlan );

    for( vitUpdateScriptPlan = m_vecScriptUpdatePlan.begin(); vitUpdateScriptPlan != m_vecScriptUpdatePlan.end(); vitUpdateScriptPlan++ ) {
        if ( tmCurrent_ >= vitUpdateScriptPlan->m_tmCheckTime && false == vitUpdateScriptPlan->m_bUsed ) {
            KECN_PLAN_SCRIPT_UPDATE_NOT kPacket;

            kPacket = vitUpdateScriptPlan->m_vecScriptInfo;

            SendScriptUpdateInfoNot( kPacket );

            vitUpdateScriptPlan->m_bUsed = true;

            // 센터 보관용 컨테이너 설정 (게임서버 재연결시 보내 주기 위해)
            std::vector< PAIR_INT_STRING >::iterator vitScriptInfo;
            for ( vitScriptInfo = vitUpdateScriptPlan->m_vecScriptInfo.begin(); vitScriptInfo != vitUpdateScriptPlan->m_vecScriptInfo.end(); vitScriptInfo++ ) {
                m_mapScriptInfoForGameServerSend[vitScriptInfo->first] = vitScriptInfo->second;
            }
        }
    }
}

bool KCnUpdatePlanManager::SendClientContentsOpenInfoNot( IN KCnClientContentsInfo& kUsingICC_, IN KCnClientContentsInfo& kNewICC_ )
{
    bool bNeedToSend = false;

    KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT kPacketCCOIN;
    kPacketCCOIN.m_nSendType = KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT::ECCOI_DATA_CHANGED;

    // 아 이 부분 더럽다... CnClientContentsInfo쪽 구조체를 commonpacket쪽 구조체들을 멤버로 변환해야 정리될 것 같다
    CheckDifferenceOfKCCGameCategoryInfo( kUsingICC_, kNewICC_, kPacketCCOIN.m_prGameCategoryInfo );
    CheckDifferenceOfKCCGameCharInfo( kUsingICC_, kNewICC_, kPacketCCOIN.m_prGameCharInfo );
    CheckDifferenceOfKCCMonsterInfo( kUsingICC_, kNewICC_, kPacketCCOIN.m_prMonsterInfo );
    CheckDifferenceOfKCCMiniEventnInfo( kUsingICC_, kNewICC_, kPacketCCOIN.m_prMiniEventInfo );

    if ( false == kPacketCCOIN.m_prGameCategoryInfo.first.empty() 
        || false == kPacketCCOIN.m_prGameCharInfo.first.empty()
        || false == kPacketCCOIN.m_prMonsterInfo.first.empty()
        || false == kPacketCCOIN.m_prMiniEventInfo.first.empty() ) {
            bNeedToSend = true;
    }

    if ( true == bNeedToSend ) {
        SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_CLIENT_CONTENTS_OPEN_INFO_NOT, kPacketCCOIN );
    }

    return true;
}

bool KCnUpdatePlanManager::CheckDifferenceOfKCCGameCategoryInfo( IN KCnClientContentsInfo& kUsingICC_, IN KCnClientContentsInfo& kNewICC_, OUT std::pair< std::vector< int >, KCCGameCategoryInfo >& prGameCategoryInfo_ )
{
    prGameCategoryInfo_.first.clear();

    // m_vecChannelCategories
    if ( kUsingICC_.m_vecChannelCategories != kNewICC_.m_vecChannelCategories ) {
        prGameCategoryInfo_.first.push_back( KCCGameCategoryInfo::EV_CHANNELCATEGORIES );
        prGameCategoryInfo_.second.m_vecChannelCategories = kNewICC_.m_vecChannelCategories;
    }

    // m_vecCategoryModes
    if ( kUsingICC_.m_vecCategoryModes != kNewICC_.m_vecCategoryModes ) {
        prGameCategoryInfo_.first.push_back( KCCGameCategoryInfo::EV_CATEGORYMODES );
        prGameCategoryInfo_.second.m_vecCategoryModes = kNewICC_.m_vecCategoryModes;
    }

    // m_vecCategoryInfo
    if ( kUsingICC_.m_vecCategoryInfo != kNewICC_.m_vecCategoryInfo ) {
        prGameCategoryInfo_.first.push_back( KCCGameCategoryInfo::EV_CATEGORYINFO );
        prGameCategoryInfo_.second.m_vecCategoryInfo = kNewICC_.m_vecCategoryInfo;
    }

    return true;
}

bool KCnUpdatePlanManager::CheckDifferenceOfKCCGameCharInfo( IN KCnClientContentsInfo& kUsingICC_, IN KCnClientContentsInfo& kNewICC_, OUT std::pair< std::vector<int> , KCCGameCharInfo >& prGameCharInfo_ )
{
    prGameCharInfo_.first.clear();

    // m_nMaxLevel
    if ( kUsingICC_.m_nMaxLevel != kNewICC_.m_nMaxLevel ) {
        prGameCharInfo_.first.push_back( KCCGameCharInfo::EV_MAXLEVEL );
        prGameCharInfo_.second.m_nMaxLevel = kNewICC_.m_nMaxLevel;
    }

    // m_mapCharactes
    if ( kUsingICC_.m_mapCharactes != kNewICC_.m_mapCharactes ) {
        prGameCharInfo_.first.push_back( KCCGameCharInfo::EV_CHARACTES );
        prGameCharInfo_.second.m_mapCharactes = kNewICC_.m_mapCharactes;
    }

    // m_vecCharSP4OpenInfo
    if ( kUsingICC_.m_vecCharSP4OpenInfo != kNewICC_.m_vecCharSP4OpenInfo ) {
        prGameCharInfo_.first.push_back( KCCGameCharInfo::EV_CHARSP4OPENINFO );
        prGameCharInfo_.second.m_vecCharSP4OpenInfo = kNewICC_.m_vecCharSP4OpenInfo;
    }

    // m_mapCharOpenType
    if ( kUsingICC_.m_mapCharOpenType != kNewICC_.m_mapCharOpenType ) {
        prGameCharInfo_.first.push_back( KCCGameCharInfo::EV_CHAROPENTYPE );
        prGameCharInfo_.second.m_mapCharOpenType = kNewICC_.m_mapCharOpenType;
    }

    // m_mapCharCashGoods
    if ( kUsingICC_.m_mapCharCashGoods != kNewICC_.m_mapCharCashGoods ) {
        prGameCharInfo_.first.push_back( KCCGameCharInfo::EV_CHARCASHGOODS );
        prGameCharInfo_.second.m_mapCharCashGoods = kNewICC_.m_mapCharCashGoods;
    }

    // m_setCharSkillEnable
    if ( kUsingICC_.m_setCharSkillEnable != kNewICC_.m_setCharSkillEnable ) {
        prGameCharInfo_.first.push_back( KCCGameCharInfo::EV_CHARSKILLENABLE );
        prGameCharInfo_.second.m_setCharSkillEnable = kNewICC_.m_setCharSkillEnable;
    }

    return true;
}

bool KCnUpdatePlanManager::CheckDifferenceOfKCCMonsterInfo( IN KCnClientContentsInfo& kUsingICC_, IN KCnClientContentsInfo& kNewICC_, OUT std::pair< std::vector< int >, KCCMonsterInfo >& prMonsterInfo_ )
{
    prMonsterInfo_.first.clear();

    // m_mapChampionRatio
    if ( kUsingICC_.m_mapChampionRatio != kNewICC_.m_mapChampionRatio ) {
        prMonsterInfo_.first.push_back( KCCMonsterInfo::EV_CHAMPIONRATIO );
        prMonsterInfo_.second.m_mapChampionRatio = kNewICC_.m_mapChampionRatio;
    }

    // m_mapMonsterAbility
    if ( kUsingICC_.m_mapMonsterAbility != kNewICC_.m_mapMonsterAbility ) {
        prMonsterInfo_.first.push_back( KCCMonsterInfo::EV_MONSTERABILITY );
        prMonsterInfo_.second.m_mapMonsterAbility = kNewICC_.m_mapMonsterAbility;
    }

    return true;
}

bool KCnUpdatePlanManager::CheckDifferenceOfKCCMiniEventnInfo( IN KCnClientContentsInfo& kUsingICC_, IN KCnClientContentsInfo& kNewICC_, OUT std::pair< std::vector< int >, KCCMiniEventnInfo >& prMiniEventInfo_ )
{
    prMiniEventInfo_.first.clear();

    /* 여기 두 멤버는 빈 컨테이너를 의도하여 전송하는 경우가 있는데
       클라이언트에서 멤버 갱신 여부를 컨테이너의 empty()로 확인하여 교체하고 있기 때문에
       의도하지 않은 동작을 하게 된다. 우선 임시 해결책으로 항상 변경된 데이터를 전달하고
       클라이언트에서는 항상 덮어씌우는 방법을 사용하려 한다
       궁극적 해결책은 내부 멤버를 pair< bool, ... > 형태로 감싸야 하겠다 */
    // m_vecDisableMenuList
    if ( kUsingICC_.m_vecDisableMenuList != kNewICC_.m_vecDisableMenuList ) {
        prMiniEventInfo_.first.push_back( KCCMiniEventnInfo::EV_DISABLEMENULIST );
        prMiniEventInfo_.second.m_vecDisableMenuList = kNewICC_.m_vecDisableMenuList;
    }

    // m_vecEventOpenInfo
    if ( kUsingICC_.m_vecEventOpenInfo != kNewICC_.m_vecEventOpenInfo ) {
        prMiniEventInfo_.first.push_back( KCCMiniEventnInfo::EV_EVENTOPENINFO );
        prMiniEventInfo_.second.m_vecEventOpenInfo = kNewICC_.m_vecEventOpenInfo;
    }

    return true;
}

bool KCnUpdatePlanManager::SendClientContentsFirstInitInfoNot( IN KCnClientContentsInfo& kUsingICC_, IN KCnClientContentsInfo& kNewICC_ )
{
    bool bNeedToSend = false;
    KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT kPacketCCFIIN;
    kPacketCCFIIN.m_nSendType = KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT::ECCFII_DATA_CHANGED;

    CheckDifferenceOfKCCUsingFilesInfo( kUsingICC_, kNewICC_, kPacketCCFIIN.m_prUsingFilesInfo );
    CheckDifferenceOfKCCHackCheckInfo( kUsingICC_, kNewICC_, kPacketCCFIIN.m_prHackCheckInfo );

    if ( false == kPacketCCFIIN.m_prUsingFilesInfo.first.empty()
        || false == kPacketCCFIIN.m_prHackCheckInfo.first.empty() ) {
            bNeedToSend = true;
    }

    if ( true == bNeedToSend ) {
        SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT, kPacketCCFIIN );
    }

    return true;
}

bool KCnUpdatePlanManager::CheckDifferenceOfKCCUsingFilesInfo( IN KCnClientContentsInfo& kUsingICC_, IN KCnClientContentsInfo& kNewICC_, OUT std::pair< std::vector< int >, KCCUsingFilesInfo >& prUsingFilesInfo_ )
{
    prUsingFilesInfo_.first.clear();

    // m_vecLoadingImageName
    if ( kUsingICC_.m_mapLoadingImageName != kNewICC_.m_mapLoadingImageName ) {
        prUsingFilesInfo_.first.push_back( KCCUsingFilesInfo::EV_LOADINGIMAGENAME );
        prUsingFilesInfo_.second.m_mapLoadingImageName = kNewICC_.m_mapLoadingImageName;
    }

    // m_vecNewsNumber
    if ( kUsingICC_.m_vecNewsNumber != kNewICC_.m_vecNewsNumber ) {
        prUsingFilesInfo_.first.push_back( KCCUsingFilesInfo::EV_NEWSNUMBER );
        prUsingFilesInfo_.second.m_vecNewsNumber = kNewICC_.m_vecNewsNumber;
    }

    // m_mapPVPLoadingImageName
    if ( kUsingICC_.m_mapPVPLoadingImageName != kNewICC_.m_mapPVPLoadingImageName ) {
        prUsingFilesInfo_.first.push_back( KCCUsingFilesInfo::EV_PVPLOADINGIMAGENAME );
        prUsingFilesInfo_.second.m_mapPVPLoadingImageName = kNewICC_.m_mapPVPLoadingImageName;
    }

    // m_vecEventBannerInfo
    if ( kUsingICC_.m_vecEventBannerInfo != kNewICC_.m_vecEventBannerInfo ) {
        prUsingFilesInfo_.first.push_back( KCCUsingFilesInfo::EV_EVENTBANNERINFO );
        prUsingFilesInfo_.second.m_vecEventBannerInfo = kNewICC_.m_vecEventBannerInfo;
    }

    // m_mapScriptName
    if ( kUsingICC_.m_mapScriptName != kNewICC_.m_mapScriptName ) {
        prUsingFilesInfo_.first.push_back( KCCUsingFilesInfo::EV_SCRIPTNAME );
        prUsingFilesInfo_.second.m_mapScriptName = kNewICC_.m_mapScriptName;
    }

    return true;
}

bool KCnUpdatePlanManager::CheckDifferenceOfKCCHackCheckInfo( IN KCnClientContentsInfo& kUsingICC_, IN KCnClientContentsInfo& kNewICC_, OUT std::pair< std::vector< int >, KCCHackCheckInfo >& prHackCheckInfo_ )
{
    prHackCheckInfo_.first.clear();

    // m_vecExceptionMotionID
    if ( kUsingICC_.m_vecExceptionMotionID != kNewICC_.m_vecExceptionMotionID ) {
        prHackCheckInfo_.first.push_back( KCCHackCheckInfo::EV_EXCEPTIONMOTIONID );
        prHackCheckInfo_.second.m_vecExceptionMotionID = kNewICC_.m_vecExceptionMotionID;
    }

    // m_setDLLBlackList
    if ( kUsingICC_.m_setDLLBlackList != kNewICC_.m_setDLLBlackList ) {
        prHackCheckInfo_.first.push_back( KCCHackCheckInfo::EV_DLLBLACKLIST );
        prHackCheckInfo_.second.m_setDLLBlackList = kNewICC_.m_setDLLBlackList;
    }

    // m_vecExtendSHAList // 현재 ICC 정보에 존재하지 않음. 지울까 했지만 클라에서 나중에 쓴다하여 패킷 구조체는 그냥 둠
    /*if ( kUsingICC_.m_vecExtendSHAList != kNewICC_.m_vecExtendSHAList ) {
        prHackCheckInfo_.first = true;
        prHackCheckInfo_.second.m_vecExtendSHAList = kNewICC_.m_vecExtendSHAList;
    }*/

    return true;
}

bool KCnUpdatePlanManager::SendCurrentICCFullPacket()
{
    KCnClientContentsInfo kUsingICC;
    GetUsingICC( kUsingICC ); // 여기 내부에서 락

    KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT kPacketCCOIN;
    kPacketCCOIN.m_nSendType = KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT::ECCOI_DATA_CHANGED;
    for ( int i = 0 ; i < KCCGameCategoryInfo::EV_MAX ; i++ )
        kPacketCCOIN.m_prGameCategoryInfo.first.push_back( i );
    for ( int i = 0 ; i < KCCGameCharInfo::EV_MAX ; i++ )
        kPacketCCOIN.m_prGameCharInfo.first.push_back( i );
    for ( int i = 0 ; i < KCCMonsterInfo::EV_MAX ; i++ )
        kPacketCCOIN.m_prMonsterInfo.first.push_back( i );
    for ( int i = 0 ; i < KCCMiniEventnInfo::EV_MAX ; i++ )
        kPacketCCOIN.m_prMiniEventInfo.first.push_back( i );

    kPacketCCOIN.m_prGameCharInfo.second.m_nMaxLevel = kUsingICC.GetMaxLevel();
    kUsingICC.GetEventMonsterAbility( kPacketCCOIN.m_prMonsterInfo.second.m_mapMonsterAbility );
    kUsingICC.GetChampionRatio( kPacketCCOIN.m_prMonsterInfo.second.m_mapChampionRatio );
    kUsingICC.GetCharSkillInfo( kPacketCCOIN.m_prGameCharInfo.second.m_setCharSkillEnable );
    kUsingICC.GetEventOpenInfo( kPacketCCOIN.m_prMiniEventInfo.second.m_vecEventOpenInfo );

    if ( kUsingICC.GetContentsInfo( kPacketCCOIN.m_prGameCategoryInfo.second.m_vecChannelCategories,
        kPacketCCOIN.m_prGameCategoryInfo.second.m_vecCategoryModes,
        kPacketCCOIN.m_prGameCategoryInfo.second.m_vecCategoryInfo,
        kPacketCCOIN.m_prGameCharInfo.second.m_mapCharactes,
        kPacketCCOIN.m_prGameCharInfo.second.m_vecCharSP4OpenInfo,
        kPacketCCOIN.m_prGameCharInfo.second.m_mapCharOpenType,
        kPacketCCOIN.m_prGameCharInfo.second.m_mapCharCashGoods,
        kPacketCCOIN.m_prMiniEventInfo.second.m_vecDisableMenuList ) ) {
            SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_CLIENT_CONTENTS_OPEN_INFO_NOT, kPacketCCOIN );
    }
    else {
        START_LOG( cerr, L"클라이언트 컨텐츠 오픈 정보가 비어 있음.." ) << END_LOG;

        return false;
    }

    KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT kPacketCCFIIN;
    kPacketCCFIIN.m_nSendType = KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT::ECCFII_DATA_CHANGED;
    for ( int i = 0 ; i < KCCUsingFilesInfo::EV_MAX ; i++ )
        kPacketCCFIIN.m_prUsingFilesInfo.first.push_back( i );
    for ( int i = 0 ; i < KCCHackCheckInfo::EV_MAX ; i++ )
        kPacketCCFIIN.m_prHackCheckInfo.first.push_back( i );

    kUsingICC.GetChannelNewsImageNumber( kPacketCCFIIN.m_prUsingFilesInfo.second.m_vecNewsNumber );
    kUsingICC.GetLoadingImage( kPacketCCFIIN.m_prUsingFilesInfo.second.m_mapLoadingImageName );
    kUsingICC.GetPVPLoadingImage( kPacketCCFIIN.m_prUsingFilesInfo.second.m_mapPVPLoadingImageName );
    kUsingICC.GetClientScriptName( kPacketCCFIIN.m_prUsingFilesInfo.second.m_mapScriptName );
    kUsingICC.GetEventBannerInfo( kPacketCCFIIN.m_prUsingFilesInfo.second.m_vecEventBannerInfo );

    kUsingICC.GetExceptionMotionID( kPacketCCFIIN.m_prHackCheckInfo.second.m_vecExceptionMotionID );
    kUsingICC.GetDLLBlackList( kPacketCCFIIN.m_prHackCheckInfo.second.m_setDLLBlackList );

    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT, kPacketCCFIIN );

    START_LOG( clog, L"스크립트 로드 후 게임 서버들에 ICC 정보 전송" )
        << BUILD_LOG( kPacketCCOIN.m_nSendType )

        << BUILD_LOG( kPacketCCOIN.m_prGameCategoryInfo.second.m_vecChannelCategories.size() )
        << BUILD_LOG( kPacketCCOIN.m_prGameCategoryInfo.second.m_vecCategoryModes.size() )
        << BUILD_LOG( kPacketCCOIN.m_prGameCategoryInfo.second.m_vecCategoryInfo.size() )

        << BUILD_LOG( kPacketCCOIN.m_prGameCharInfo.second.m_nMaxLevel )
        << BUILD_LOG( kPacketCCOIN.m_prGameCharInfo.second.m_mapCharactes.size() )
        << BUILD_LOG( kPacketCCOIN.m_prGameCharInfo.second.m_vecCharSP4OpenInfo.size() )
        << BUILD_LOG( kPacketCCOIN.m_prGameCharInfo.second.m_mapCharOpenType.size() )
        << BUILD_LOG( kPacketCCOIN.m_prGameCharInfo.second.m_mapCharCashGoods.size() )
        << BUILD_LOG( kPacketCCOIN.m_prGameCharInfo.second.m_setCharSkillEnable.size() )

        << BUILD_LOG( kPacketCCOIN.m_prMonsterInfo.second.m_mapChampionRatio.size() )
        << BUILD_LOG( kPacketCCOIN.m_prMonsterInfo.second.m_mapMonsterAbility.size() )

        << BUILD_LOG( kPacketCCOIN.m_prMiniEventInfo.second.m_vecDisableMenuList.size() )
        << BUILD_LOG( kPacketCCOIN.m_prMiniEventInfo.second.m_vecEventOpenInfo.size() )

        << BUILD_LOG( kPacketCCFIIN.m_nSendType )

        << BUILD_LOG( kPacketCCFIIN.m_prUsingFilesInfo.second.m_vecNewsNumber.size() )
        << BUILD_LOG( kPacketCCFIIN.m_prUsingFilesInfo.second.m_mapLoadingImageName.size() )
        << BUILD_LOG( kPacketCCFIIN.m_prUsingFilesInfo.second.m_mapScriptName.size() )

        << BUILD_LOG( kPacketCCFIIN.m_prHackCheckInfo.second.m_vecExceptionMotionID.size() )
        << BUILD_LOG( kPacketCCFIIN.m_prHackCheckInfo.second.m_setDLLBlackList.size() )
        
        << END_LOG;

    return true;
}

bool KCnUpdatePlanManager::SendScriptUpdateInfoNot( IN const KECN_PLAN_SCRIPT_UPDATE_NOT& kPacket_ )
{
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_PLAN_SCRIPT_UPDATE_NOT, kPacket_ );
    return true;
}

bool KCnUpdatePlanManager::GetCurrentICCFullPacket( OUT KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT& kPacketCCOIN_, OUT KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT& kPacketCCFIIN_ )
{
    KCnClientContentsInfo kUsingICC;
    GetUsingICC( kUsingICC ); // 여기 내부에서 락

    KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT kPacketCCOIN;
    kPacketCCOIN.m_nSendType = KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT::ECCOI_DATA_CHANGED;
    for ( int i = 0 ; i < KCCGameCategoryInfo::EV_MAX ; i++ )
        kPacketCCOIN.m_prGameCategoryInfo.first.push_back( i );
    for ( int i = 0 ; i < KCCGameCharInfo::EV_MAX ; i++ )
        kPacketCCOIN.m_prGameCharInfo.first.push_back( i );
    for ( int i = 0 ; i < KCCMonsterInfo::EV_MAX ; i++ )
        kPacketCCOIN.m_prMonsterInfo.first.push_back( i );
    for ( int i = 0 ; i < KCCMiniEventnInfo::EV_MAX ; i++ )
        kPacketCCOIN.m_prMiniEventInfo.first.push_back( i );

    kPacketCCOIN.m_prGameCharInfo.second.m_nMaxLevel = kUsingICC.GetMaxLevel();
    kUsingICC.GetEventMonsterAbility( kPacketCCOIN.m_prMonsterInfo.second.m_mapMonsterAbility );
    kUsingICC.GetChampionRatio( kPacketCCOIN.m_prMonsterInfo.second.m_mapChampionRatio );
    kUsingICC.GetCharSkillInfo( kPacketCCOIN.m_prGameCharInfo.second.m_setCharSkillEnable );
    kUsingICC.GetEventOpenInfo( kPacketCCOIN.m_prMiniEventInfo.second.m_vecEventOpenInfo );

    if ( !kUsingICC.GetContentsInfo( kPacketCCOIN.m_prGameCategoryInfo.second.m_vecChannelCategories,
        kPacketCCOIN.m_prGameCategoryInfo.second.m_vecCategoryModes,
        kPacketCCOIN.m_prGameCategoryInfo.second.m_vecCategoryInfo,
        kPacketCCOIN.m_prGameCharInfo.second.m_mapCharactes,
        kPacketCCOIN.m_prGameCharInfo.second.m_vecCharSP4OpenInfo,
        kPacketCCOIN.m_prGameCharInfo.second.m_mapCharOpenType,
        kPacketCCOIN.m_prGameCharInfo.second.m_mapCharCashGoods,
        kPacketCCOIN.m_prMiniEventInfo.second.m_vecDisableMenuList ) ) {
            START_LOG( cerr, L"클라이언트 컨텐츠 오픈 정보가 비어 있음.." ) << END_LOG;
            return false;
    }

    KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT kPacketCCFIIN;
    kPacketCCFIIN.m_nSendType = KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT::ECCFII_DATA_CHANGED;
    for ( int i = 0 ; i < KCCUsingFilesInfo::EV_MAX ; i++ )
        kPacketCCFIIN.m_prUsingFilesInfo.first.push_back( i );
    for ( int i = 0 ; i < KCCHackCheckInfo::EV_MAX ; i++ )
        kPacketCCFIIN.m_prHackCheckInfo.first.push_back( i );

    kUsingICC.GetChannelNewsImageNumber( kPacketCCFIIN.m_prUsingFilesInfo.second.m_vecNewsNumber );
    kUsingICC.GetLoadingImage( kPacketCCFIIN.m_prUsingFilesInfo.second.m_mapLoadingImageName );
    kUsingICC.GetPVPLoadingImage( kPacketCCFIIN.m_prUsingFilesInfo.second.m_mapPVPLoadingImageName );
    kUsingICC.GetClientScriptName( kPacketCCFIIN.m_prUsingFilesInfo.second.m_mapScriptName );
    kUsingICC.GetEventBannerInfo( kPacketCCFIIN.m_prUsingFilesInfo.second.m_vecEventBannerInfo );

    kUsingICC.GetExceptionMotionID( kPacketCCFIIN.m_prHackCheckInfo.second.m_vecExceptionMotionID );
    kUsingICC.GetDLLBlackList( kPacketCCFIIN.m_prHackCheckInfo.second.m_setDLLBlackList );

    kPacketCCOIN_ = kPacketCCOIN;
    kPacketCCFIIN_ = kPacketCCFIIN;

    START_LOG( clog, L"스크립트 로드 후 게임 서버들에 ICC 정보 전송" )
        << BUILD_LOG( kPacketCCOIN.m_nSendType )

        << BUILD_LOG( kPacketCCOIN.m_prGameCategoryInfo.second.m_vecChannelCategories.size() )
        << BUILD_LOG( kPacketCCOIN.m_prGameCategoryInfo.second.m_vecCategoryModes.size() )
        << BUILD_LOG( kPacketCCOIN.m_prGameCategoryInfo.second.m_vecCategoryInfo.size() )

        << BUILD_LOG( kPacketCCOIN.m_prGameCharInfo.second.m_nMaxLevel )
        << BUILD_LOG( kPacketCCOIN.m_prGameCharInfo.second.m_mapCharactes.size() )
        << BUILD_LOG( kPacketCCOIN.m_prGameCharInfo.second.m_vecCharSP4OpenInfo.size() )
        << BUILD_LOG( kPacketCCOIN.m_prGameCharInfo.second.m_mapCharOpenType.size() )
        << BUILD_LOG( kPacketCCOIN.m_prGameCharInfo.second.m_mapCharCashGoods.size() )
        << BUILD_LOG( kPacketCCOIN.m_prGameCharInfo.second.m_setCharSkillEnable.size() )

        << BUILD_LOG( kPacketCCOIN.m_prMonsterInfo.second.m_mapChampionRatio.size() )
        << BUILD_LOG( kPacketCCOIN.m_prMonsterInfo.second.m_mapMonsterAbility.size() )

        << BUILD_LOG( kPacketCCOIN.m_prMiniEventInfo.second.m_vecDisableMenuList.size() )
        << BUILD_LOG( kPacketCCOIN.m_prMiniEventInfo.second.m_vecEventOpenInfo.size() )

        << BUILD_LOG( kPacketCCFIIN.m_nSendType )

        << BUILD_LOG( kPacketCCFIIN.m_prUsingFilesInfo.second.m_vecNewsNumber.size() )
        << BUILD_LOG( kPacketCCFIIN.m_prUsingFilesInfo.second.m_mapLoadingImageName.size() )
        << BUILD_LOG( kPacketCCFIIN.m_prUsingFilesInfo.second.m_mapScriptName.size() )

        << BUILD_LOG( kPacketCCFIIN.m_prHackCheckInfo.second.m_vecExceptionMotionID.size() )
        << BUILD_LOG( kPacketCCFIIN.m_prHackCheckInfo.second.m_setDLLBlackList.size() )

        << END_LOG;

    return true;
}

void KCnUpdatePlanManager::SendEventUpdateNot( IN int nEventType_ )
{
    KECN_EVENT_DB_UPDATE_NOT kPacket;
    kPacket.m_nEventType = nEventType_;
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_EVENT_DB_UPDATE_NOT, kPacket );
}