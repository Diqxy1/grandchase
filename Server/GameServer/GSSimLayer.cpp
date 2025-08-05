#include "GSSimLayer.h"
#include "CnConnector.h"
#include "UserFSM.h"
#include "GuildManager.h"
#include "GCEnum.h"
#include "DropSystem.h"
#include "FailRate.h"
#include "GSGash.h"
#include "GameServer.h"
#include "MissionManager.h"
#include "IndigoUtil.h" // KServerOpenTime
#include "GashBilling.h"

#include "FatigueSystem.h"
#include "EvtDBThread.h"
#include "PetHelper.h"
#include "GSDBLayer.h"
#include "LogManager.h"
#include "ChannelDrop.h"
#include "EnchantStrong.h"
#include "BillingError.h"
#include "ChannelManager.h"
#include "Channel.h"
#include "CollectionManager.h"
#include "MiniGameRankManager.h"
#include "LoudMessage.h"
#include "PayLetterBilling.h"
#include "Worker.h"
#include "PreHackingCheckManager.h"
#include <KncUtilLua.h>
#include <boost/algorithm/string.hpp> // for replace_all
#include <luabind/operator.hpp> // luabind::self
#include "PeriodNotice.h"
#include "TH_PayLetterBilling.h"
#include "VirtualCash.h"
#include "SquareManager.h"
#include "TickManager.h"
#include "NCashBilling.h"
#include "GCPoint.h"
#include "AgentConnector.h"
#include "NMNetCafeManager.h"
#include "Log4.h"
#include "SHA1.h"
#include "PlayAuthManager.h"
#include "MatchConnector.h"
#include "CYOUBilling/CYOUBilling.h"

#define HEAP_SIZE 128
#define MAX_FILE_SHA1_BUFFER 1024

ImplementSingleton( KGSSimLayer );
NiImplementRTTI( KGSSimLayer, KSimLayer );

KGSSimLayer::KGSSimLayer(void) 
:
    m_uiNickNameAvailable( 16 ),
    m_dwShutdownTimeLimit( 30000 ),
    m_iMaxGamePoint( 499999 ),
    m_dwGameStartWaitTime( 60000 ),
    m_bRecommendEnable( false ),
    m_dwIndigoLevel( 30 ),
    m_bCheckIndigoTicket( true ),
    m_uiPetNickNameAvailable( 8 ),
    m_bEnableForce( false ),
    m_bWriteClientInfo( false ),
    m_dwTableChecksum( 0L ),
    m_bTimeSync( false ),
    m_dwMaxTickDuration( 10 ),
    m_nMaxReportNum( 0 ),
    m_dwChatEventGap( 60 * 1000 ),
    m_dwAllowedTime( 30 * 1000 ),
    m_nChatEventPvPUserNum(6),
    m_nChatEventDungeonUserNum(4),
    m_bTWUserAgreement( false ),
    m_nMaxPing( 180 ),
    m_nMinPing( 60 ),
    m_nMaxLossCount( 10 ),
    m_bEnableP2PStat( true ),
    m_bLoadingCheck( false ),
    m_bPacketCheckInEnable( true ),
    m_bEnableIndigoLog( false ),
    m_bEnableLimitKeyBuy( false ),
    m_bUSEvent( false ),
    m_bGachaEnable( false ),
    m_bPackageBuyVirtualCash( false ),
    m_nMaxVirtualCash( 999999 ),
    m_bEnableGameCoupon( false ),
    m_bVCMPCantBuy( false ),
    m_bPacketHackCheck( true ),
    m_dwLongProcessTime( 3000 ),
    m_bHostMigrationByPing( true ),
    m_bDeathMatchQuickJoin( false ),
    m_dwExpDBUpdateTick( 60 * 1000 * 10 ),
    m_bCheckJoinRoomPing( false ),
    m_dwStageLoadLimitTime( 45000 ),
    m_bCheckUserStageLoadCompleteInfo( false ),
    m_bServerRoomInfoEnable( true ),
    m_bServerRoomInfoTick( 30 * 1000 ),
    m_bBlockMaxUser( true ),
    m_nLimitAcceptQueueCount( 5000 ),
    m_bUseCompetitionTicket( false ),
    m_bLimitAcceptEnable( false ),
    m_bUserCreateRoomCompeteServer( false ),
    m_nServerListAuthLevel( 3 ),
    m_kTimer_DBQ_Alert( 60 * 1000 * 5 ),
    m_bAgitEnable( false ),
    m_bNMCafeEnable( false ),
    m_dwNMCafeUpdateTickGap( 3 * 60 * 1000 ),
    m_bSHACheckEnable( false ),
    m_bLvDesign( false ),
    m_bEnableChatEvent(false),
    m_bGCPointSwitch(false),
    m_dwUniqueNumber(0),
    m_bUpdateRoomStatEnable(true),
    m_dwRoomStatTickGap(60*60*1000),
    m_dwDBUpdateUserDataTick( 20 * 60 * 1000 ),
    m_NewCount(0),
    m_DeleteCount(0),
    m_dwPackageNum(5000),
    m_dwOneItemNum(2000),
    m_bCheckAuthTickCount(true),
    m_bMonsterCardMigraion(false),
    m_bInitTimeSync( true ),
    m_bWaitInvenMigration( false )
{
    for( int iTick = 0 ; iTick < TICK_COUNT_NUM ; ++iTick )
    {
        SetTick( iTick );
    }

    // 070314. kkurrung. 신규 유저 강제로 특정 모드 플레이 하도록 하는 것.
    m_tmForceTime           = CTime( 2030, 1, 1, 0, 0, 0 );
    m_setForceModeID.clear();
    
    m_vecMonthlyReward.clear(); // 070423. kkurrung. 월 개근시 사용되는 데이터.
    m_tmSHAdjust = CTime( 1990, 1, 1, 00, 00, 00 ); // 상하이 적용 시점
    m_tmMaxTickTime = CTime::GetCurrentTime(); // 최대 틱 시간 저장.

    m_mapStatLoadingTime.clear(); // 로딩 타임통계
    m_mapWeeklyReward.clear();
    m_mapMonthlyReward.clear();

    m_tmNewUserEventTime    = CTime( 2000, 4, 8 ); //  090409  woosh. 홍콩 친구함께

    {
        m_vecPingRange.push_back( 200 );
        m_vecPingRange.push_back( 350 );
        m_vecPingRange.push_back( 500 );
    }
}

KGSSimLayer::~KGSSimLayer(void)
{
}

ImplToStringW( KGSSimLayer )
{
    START_TOSTRING_PARENTW( KSimLayer )
        << L"    " << L"Nick Length : " << m_uiNickNameAvailable
        << L", PetNick Length : " << m_uiPetNickNameAvailable << std::endl
        << TOSTRINGW( m_dwShutdownTimeLimit )
        << L"    " << L"MaxGP: " << m_iMaxGamePoint << std::endl
        << TOSTRINGW( m_dwGameStartWaitTime )
        << TOSTRINGWb( m_bRecommendEnable )
        << TOSTRINGWb( m_bWriteClientInfo )
        << TOSTRINGWb( m_bEnableForce )
        << TOSTRINGW( m_dwTableChecksum )
        << TOSTRINGWb( m_bEnableGameCoupon );

    stm_<< TOSTRINGW( m_nMaxReportNum )
        << TOSTRINGWb( m_bEnableP2PStat )
        << TOSTRINGWb( m_bPacketCheckInEnable )
        << TOSTRINGW( m_bMonsterCardMigraion )
        << L"    " << L"SHAdjust Time : " << (LPCTSTR)m_tmSHAdjust.Format( KNC_TIME_FORMAT ) << std::endl
        << std::boolalpha
        << L"      ST_BROADCASTING   : " << CheckTypeFlag(ST_BROADCASTING) << std::endl
        << L"      ST_OPEN_TESOP     : " << CheckTypeFlag(ST_OPEN_TESOP) << std::endl
        << L"      ST_INTERNAL_TESOP : " << CheckTypeFlag(ST_INTERNAL_TESOP) << std::endl
        << L"      ST_DP_LOG         : " << CheckTypeFlag(ST_DP_LOG) << std::endl
        << L"      ST_INDIGO         : " << CheckTypeFlag(ST_INDIGO) << std::endl
        << L"      ST_DEBUG_AUTH     : " << CheckTypeFlag(ST_DEBUG_AUTH) << std::endl
        << L"      ST_ENTER_LIMIT    : " << CheckTypeFlag(ST_ENTER_LIMIT) << std::endl
        << L"      ST_PVP_BALANCE    : " << CheckTypeFlag(ST_PVP_BALANCE) << std::endl
        << L"      ST_CREATE_LIMIT   : " << CheckTypeFlag(ST_CREATE_LIMIT) << std::endl
        << L"      ST_TOURNAMENT     : " << CheckTypeFlag(ST_TOURNAMENT) << std::endl
        << L"      ST_PVP_FREE       : " << CheckTypeFlag(ST_PVP_FREE) << std::endl
        << L"      ST_PVP_NO_AGIT_BUFF       : " << CheckTypeFlag(ST_PVP_NO_AGIT_BUFF) << std::endl
        << L"      ST_NO_AP_CHAR             : " << CheckTypeFlag(ST_NO_AP_CHAR) << std::endl
        << L"      ST_LOCK_SKILL_TREE    : " << CheckTypeFlag(ST_LOCK_SKILL_TREE) << std::endl
        << L"      ST_DUNGEON        : " << CheckTypeFlag(ST_DUNGEON) << std::endl
        << L"      ST_PVP            : " << CheckTypeFlag(ST_PVP) << std::endl

        << std::noboolalpha
        << *KRoom::GetDropSystem()
        << TOSTRINGW( m_mapKairo.size() );
    return GetMaxTickInfo( stm_ );
}

ImplOstmOperatorW2A( KUser );
ImplOstmOperatorW2A( KChannelManager );
ImplOstmOperatorW2A( KChannel );
ImplOstmOperatorW2A( KRoom );
ImplOstmOperatorW2A( KGSSimLayer );
ImplOstmOperatorW2A( KRoomManager );
ImplOstmOperatorW2A( KServerOpenTime );
ImplOstmOperatorW2A( KLogManager );

void KGSSimLayer::RegToLua( std::shared_ptr<lua_State> L )
{
    KSimLayer::RegToLua( L );


    luabind::module( L.get() )
    [
        luabind::class_<Kairo>( "Kairo" )
            .def( luabind::constructor<>() )
            .def( "SetPostItem", &Kairo::SetPostItem )
            .def( "SetEnchantDropItem", &Kairo::SetEnchantDropItem )
            .def( "SetStrengthEquipDropItem", &Kairo::SetStrengthEquipDropItem )
            .def( "SetStrengthPeriodEquipDropItem", &Kairo::SetStrengthPeriodEquipDropItem ),

        luabind::def( "GetSimLayer",                &KGSSimLayer::GetInstance ),
        luabind::class_<KGSSimLayer, KSimLayer>( "KGSSimLayer" )
            .def( luabind::tostring(luabind::self) )
            .def( "SetHBGap",                           &KGSSimLayer::SetHBGap )   // 061024. kkurrung.
            .def( "SetKairo",                           &KGSSimLayer::SetKairo )
            .def( "ClearKairo",                         &KGSSimLayer::ClearKairo )
            .def( "PrintKairo",                         &KGSSimLayer::PrintKairo )
            .def( "SetForceNewUser",                    &KGSSimLayer::SetForceNewUser ) // 유저 강제 선행 플레이
            .def( "AddForceMode",                       &KGSSimLayer::AddForceMode )
            .def( "ClearForceMode",                     &KGSSimLayer::ClearForceMode )
            .def( "SetTableChecksum",                   &KGSSimLayer::SetTableChecksum )
            .def( "SetSHAdhustTime",                    &KGSSimLayer::SetSHAdhustTime )    // 070429. kkurrung. 상하이 드랍 적용 가입일.
            .def( "SetChatEventOption",                 &KGSSimLayer::SetChatEventOption ) // 채팅이벤트 정보 설정.
            .def( "SetP2PStat",                         &KGSSimLayer::SetP2PStat ) //  080601  woosh. P2P 정보 기록 설정
            .def( "ReloadReward",                       &KGSSimLayer::ReloadAttendanceRewardToDB ) 
            .def( "SetNewUserFriendEvent",              &KGSSimLayer::SetNewUserFriendEvent ) //  090413  woosh. 홍콩 친구함께 이벤트, 신규의 기준
            .def( "SetPacketHackCheck",                 &KGSSimLayer::SetPacketHackCheck )
            .def( "SetLongProcessTime",                 &KGSSimLayer::SetLongProcessTime ) // 100715 woosh. 프로세스 오래 걸리면 로그.
            .def( "SetHostMigrationByPing",             &KGSSimLayer::SetHostMigrationByPing )
            .def( "SetDeathMatchQuickJoin",             &KGSSimLayer::SetDeathMatchQuickJoin )
            .def( "SetExpDBUpdateTick",                 &KGSSimLayer::SetExpDBUpdateTick )
            .def( "SetCheckUserLoadCompleteInfo",       &KGSSimLayer::SetCheckUserLoadCompleteInfo )
            .def( "SetServerRoomInfoTick",              &KGSSimLayer::SetServerRoomInfoTick )
            .def( "SetServerRoomInfoEnable",            &KGSSimLayer::SetServerRoomInfoEnable )
            .def( "SetCreateUserEnable",                &KGSSimLayer::SetCreateUserEnable )
            .def( "CloseAllUser",                       &KGSSimLayer::CloseAllUser )
            .def( "SetServerListAuthLevel",             &KGSSimLayer::SetServerListAuthLevel )
            .def( "SetAgitEnable",                      &KGSSimLayer::SetAgitEnable )
            .def( "SetDBQAlertTimerInterval",           &KGSSimLayer::SetDBQAlertTimerInterval )
            .def( "SetNMCafeEnable",                    &KGSSimLayer::SetNMCafeEnable )
            .def( "SetNMCafeUpdateTickGap",             &KGSSimLayer::SetNMCafeUpdateTickGap )
            .def( "SetSHACheckEnable",                  &KGSSimLayer::SetSHACheckEnable )
            .def( "SetLvDesignEnable",                  &KGSSimLayer::SetLvDesignEnable )
            .def( "SetUpdateRoomStat",                  &KGSSimLayer::SetUpdateRoomStat )
            .def( "SetRoomStatTickGap",                 &KGSSimLayer::SetRoomStatTickGap )
            .def( "SetDBUpdateUserDataTick",            &KGSSimLayer::SetDBUpdateUserDataTick )
            .def( "SetMaxPackageNum",                   &KGSSimLayer::SetMaxPackageNum )
            .def( "SetMaxItemNum",                      &KGSSimLayer::SetMaxItemNum )
            .def( "SetCheckTickCount",                  &KGSSimLayer::SetCheckTickCount ) // auth tick 체크 on/off
            .def( "DumpInventory",                      &KGSSimLayer::DumpInventory ) 
            .def_readwrite( "m_uiNickNameAvailable",    &KGSSimLayer::m_uiNickNameAvailable )
            .def_readwrite( "m_uiPetNickNameAvailable", &KGSSimLayer::m_uiPetNickNameAvailable )
            .def_readwrite( "m_dwShutdownTimeLimit",    &KGSSimLayer::m_dwShutdownTimeLimit )
            .def_readwrite( "m_iMaxGamePoint",          &KGSSimLayer::m_iMaxGamePoint )
            .def_readwrite( "m_dwGameStartWaitTime",    &KGSSimLayer::m_dwGameStartWaitTime )
            .def_readwrite( "m_bRecommendEnable",       &KGSSimLayer::m_bRecommendEnable )
            .def_readwrite( "m_dwIndigoLevel",          &KGSSimLayer::m_dwIndigoLevel )
            .def_readwrite( "m_bCheckIndigoTicket",     &KGSSimLayer::m_bCheckIndigoTicket )
            .def_readwrite( "m_bWriteClientInfo",       &KGSSimLayer::m_bWriteClientInfo )
            .def_readwrite( "m_bTimeSync",              &KGSSimLayer::m_bTimeSync )
            .def_readwrite( "m_nMaxReportNum",          &KGSSimLayer::m_nMaxReportNum )
            .def_readwrite( "m_bLoadingCheck",          &KGSSimLayer::m_bLoadingCheck )
            .def_readwrite( "m_bPacketCheckInEnable",   &KGSSimLayer::m_bPacketCheckInEnable ) // 080602 tslayer.
            .def_readwrite( "m_bEnableIndigoLog",       &KGSSimLayer::m_bEnableIndigoLog ) // 080715. kkurrung. 용섬 로그
            .def_readwrite( "m_bEnableLimitKeyBuy",     &KGSSimLayer::m_bEnableLimitKeyBuy ) // 키 구매 제한
            .def_readwrite( "m_bUSEvent", 				&KGSSimLayer::m_bUSEvent ) // 080710. kkurrung. 미국 이벤트
            .def_readwrite( "m_bGachaEnable", 			&KGSSimLayer::m_bGachaEnable ) // 080819 tslayer. 가챠
            .def_readwrite( "m_bPackageBuyVirtualCash", &KGSSimLayer::m_bPackageBuyVirtualCash ) //  090319  woosh. 가상캐쉬 패키지 구매 가능여부
            .def_readwrite( "m_bEnableGameCoupon",      &KGSSimLayer::m_bEnableGameCoupon ) // 게임쿠폰 시스템 사용 여부
            .def_readwrite( "m_nMaxVirtualCash",        &KGSSimLayer::m_nMaxVirtualCash ) //  090518  woosh. 가상캐쉬 최대치
            .def_readwrite( "m_bVCMPCantBuy",           &KGSSimLayer::m_bVCMPCantBuy ) //  090702  woosh. 가상캐쉬 미션,패키지 구매 방지(홍콩에서 미션구매 방지에 사용, 대만은 off-패키지는 기본으로 불가 )
            .def_readwrite( "m_bCheckJoinRoomPing",     &KGSSimLayer::m_bCheckJoinRoomPing ) // 101103 woosh. 핑 기반 퀵조인 제어
            .def_readwrite( "m_dwStageLoadLimitTime",   &KGSSimLayer::m_dwStageLoadLimitTime ) // 101108 woosh. 스테이지 로딩 제한시간
            .def_readwrite( "m_bBlockMaxUser",          &KGSSimLayer::m_bBlockMaxUser )
            .def_readwrite( "m_nLimitAcceptQueueCount", &KGSSimLayer::m_nLimitAcceptQueueCount ) // 110522 woosh. 디비큐 기준치 이상 쌓이면 접속제한
            .def_readwrite( "m_bUseCompetitionTicket",  &KGSSimLayer::m_bUseCompetitionTicket ) // 20110519. 대회서버 입장티켓
            .def_readwrite( "m_bLimitAcceptEnable",     &KGSSimLayer::m_bLimitAcceptEnable )
            .def_readwrite( "m_bUserCreateRoomCompeteServer", &KGSSimLayer::m_bUserCreateRoomCompeteServer ) // 2011-6-24 woosh. 대회서버 - 일반유저가 방 만들 수 있는 옵션
            .def_readwrite( "m_bMonsterMigraion",       &KGSSimLayer::m_bMonsterCardMigraion )
            .def_readwrite( "m_InitbTimeSync",          &KGSSimLayer::m_bInitTimeSync )
            .def_readwrite( "m_bWaitInvenMigration",    &KGSSimLayer::m_bWaitInvenMigration )

            .enum_( "ServerType" )
            [
                luabind::value( "ST_GUILD",             ST_GUILD ),
                luabind::value( "ST_BROADCASTING",      ST_BROADCASTING ),
                luabind::value( "ST_OPEN_TESOP",        ST_OPEN_TESOP ),
                luabind::value( "ST_INTERNAL_TESOP",    ST_INTERNAL_TESOP ),
                luabind::value( "ST_DP_LOG",            ST_DP_LOG ),
                luabind::value( "ST_INDIGO",            ST_INDIGO ),
                luabind::value( "ST_DEBUG_AUTH",        ST_DEBUG_AUTH ),
                luabind::value( "ST_FORCE_DWORD",       ST_FORCE_DWORD ),
                luabind::value( "ST_ENTER_LIMIT",       ST_ENTER_LIMIT ),
                luabind::value( "ST_PVP_BALANCE",       ST_PVP_BALANCE ),
                luabind::value( "ST_CREATE_LIMIT",      ST_CREATE_LIMIT ),
                luabind::value( "ST_OBSERVER_ROOM",     ST_OBSERVER_ROOM ),
                luabind::value( "ST_TOURNAMENT",        ST_TOURNAMENT ),
                luabind::value( "ST_PVP_FREE",          ST_PVP_FREE ),
                luabind::value( "ST_PVP_NO_AGIT_BUFF",  ST_PVP_NO_AGIT_BUFF ),
                luabind::value( "ST_NO_AP_CHAR",        ST_NO_AP_CHAR ),
                luabind::value( "ST_LOCK_SKILL_TREE",   ST_LOCK_SKILL_TREE ),
                luabind::value( "ST_DUNGEON",           ST_DUNGEON ),
                luabind::value( "ST_PVP",               ST_PVP )
            ],

        luabind::def( "GetChannelManager", &KChannelManager::GetInstance ),
        luabind::class_<KChannelManager>( "KChannelManager" )
            .def( luabind::tostring(luabind::self) )
            .def( "Get",                    &KChannelManager::Get )
            .def( "Send_NotMsg",            &KChannelManager::Send_NotMsgA )
            .def( "SetBroadCastingTime",    &KChannelManager::SetBroadCastingTime )
            .def( "SetChannelLobby",        &KChannelManager::SetChannelLobby )
            .def( "ModifyChannelLobby",     &KChannelManager::ModifyChannelLobby )
            .def( "AddChannel",             &KChannelManager::AddChannel ),

        luabind::class_<KChannel>( "KChannel" )
            .def( luabind::tostring(luabind::self) )
            .def( "SetMaxUsers",    &KChannel::SetMaxUsers )
            .def( "SetRoomNum",     &KChannel::SetRoomNum )
            .def( "SetType",        &KChannel::SetType )
            .def( "SetModeList",    &KChannel::SetModeList )
            .def( "SetGrade",       &KChannel::SetGradeRange )
            .def( "ModifyChannel",  &KChannel::ModifyChannel )
            .property( "m_spRoomManager", &KChannel::GetRoomManager ),

        luabind::class_<KRoomManager>( "KRoomManager" )
            .def( luabind::tostring(luabind::self) )
            .def( "GetRoom",        &KRoomManager::GetRoomForLua )
            .def( "Deactive",       &KRoomManager::DeActive )
            .def( "ReserveRoom",    &KRoomManager::ReserveRoomObject ),

        luabind::def( "SetGuildBroadCastingTime", &KGuild::SetBroadCastingTime ),

        luabind::class_<KDropMode>( "KDropMode" )
            .def( luabind::constructor<>() )
            .def( "AddDropInfo", &KDropMode::AddDropInfo ),

        luabind::def( "GetDropSystem", &KRoom::GetDropSystem ),

        luabind::class_<KDropSystem>( "KDropSystem" )
            .def( luabind::tostring(luabind::self) )
            .def( "AddMode",            &KDropSystem::AddMode )
            .def( "EnableAdminEvent",   &KDropSystem::EnableAdminEvent )
            .def( "ClearDropModeList",  &KDropSystem::ClearDropModeList ),

        luabind::def( "GetFailRate", KFailRate::GetInstance ),
        luabind::class_<KFailRate>( "KFailRate" )
            .def( luabind::tostring(luabind::self) )
            .def( "SetWriteToDB",   &KFailRate::SetWriteToDB )
            .def( "ToLog",          &KFailRate::ToLog )
            .def( "ClientErrLog",   &KFailRate::ClientErrLog )
            .def( "SetWriteToDBTick", &KFailRate::SetWriteToDBTick )
            .def( "SetUpdateTime",  &KFailRate::SetUpdateTime ),

        luabind::def( "GetGashManager", &KTThreadManager<KGSGash>::GetInstance ),
        luabind::def( "Init_GashInfo", &KGSGash::InitGashInfo ),
        luabind::class_<KTThreadManager<KGSGash>, KThreadManager>( "KGashManager" )
            .def( luabind::tostring(luabind::self ) ),

        luabind::def( "GetGashBillingManager", &KTThreadManager<KGashBilling>::GetInstance ),
            
        luabind::def( "Init_GashBillingInfo",& KGashBilling::InitGashBillingInfo ),
        luabind::class_<KTThreadManager<KGashBilling>, KThreadManager>( "GashBillingManager" )
            .def( luabind::tostring(luabind::self ) ),

        luabind::def( "GetMissionManager", &KMissionManager::GetInstance ),
        luabind::class_<KMissionManager>("KMissionManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KMissionManager::LoadScript )
            .def( "PrintMission", &KMissionManager::PrintMission )
            .def( "PrintSubMission", &KMissionManager::PrintSubMission )
            .def( "ClearAll", &KMissionManager::ClearAll )
            .def( "DumpUnique", &KMissionManager::DumpUnique )
            .def( "SetMissionBonusExpGp", &KMissionManager::SetMissionBonusExpGp ),

        luabind::def( "GetOpenTime", &KServerOpenTime::GetInstance ),
        luabind::class_<KServerOpenTime>("KServerOpenTime")
            .def( luabind::tostring(luabind::self) )
            .def( "SetEnable", &KServerOpenTime::SetEnable )
            .def( "CheckDBUpdate", &KServerOpenTime::CheckDBUpdate ),

        luabind::def( "GetFatigueSystem", &KFatigueSystem::GetInstance ),
        luabind::class_<KFatigueSystem>("KFatigueSystem")
            .def( luabind::tostring(luabind::self) )
            .def( "SetRewardLimit", &KFatigueSystem::SetRewardLimit ),

        luabind::def( "GetPetHelper", &KPetHelper::GetInstance ),
        luabind::class_<KPetHelper>("KPetHelper")
            .def( luabind::tostring(luabind::self) )
            .def( "AddPetFeed",         &KPetHelper::AddPetFeed )
            .def( "GetPetFeedFactor",   &KPetHelper::GetPetFeedFactor )
            .def( "ClearPetFeed",       &KPetHelper::ClearPetFeed )
            .def( "PrintPetFeed",       &KPetHelper::PrintPetFeed )
            .def( "ClearExpInfo",       &KPetHelper::ClearExpInfo )
            .def( "AddExpInfo",         &KPetHelper::AddExpInfo )
            .def( "SortExpInfo",        &KPetHelper::SortExpInfo )
            .def( "ClearDecreseTable",  &KPetHelper::ClearDecreseTable )
            .def( "AddDecreseRate",     &KPetHelper::AddDecreseRate )
            .def( "PrintDecreseSatiation",  &KPetHelper::PrintDecreseSatiation )
            .def( "ClearAssistItemTable",   &KPetHelper::ClearAssistItemTable )
            .def( "AddAssistItem",          &KPetHelper::AddAssistItem )
            .def( "PrintAssistItemTable",   &KPetHelper::PrintAssistItemTable )
            .def( "SetMaxFeed",             &KPetHelper::SetMaxFeed )
            .def( "ClearExpPanaltyTable",   &KPetHelper::ClearExpPanaltyTable )
            .def( "AddExpPanalty",          &KPetHelper::AddExpPanalty )
            .def( "SortExpPanaltyTable",    &KPetHelper::SortExpPanaltyTable )
            .def( "PrintExpPanaltyTable",   &KPetHelper::PrintExpPanaltyTable )
            .def( "ClearPromotionPanaltyTable",     &KPetHelper::ClearPromotionPanaltyTable )
            .def( "AddPromotionPanalty",            &KPetHelper::AddPromotionPanalty )
            .def( "PrintPromotionPanaltyTable",     &KPetHelper::PrintPromotionPanaltyTable )
            .def( "ClearTransformItem",             &KPetHelper::ClearTransformItem )
            .def( "AddTransformItem",               &KPetHelper::AddTransformItem )
            .def( "AddHatchingInfo",                &KPetHelper::AddHatchingInfo )
            .def( "ClearHatchingInfo",              &KPetHelper::ClearHatchingInfo )
            .def( "PrintHatchingInfo",              &KPetHelper::PrintHatchingInfo )
            .def( "AddPetCostume",                  &KPetHelper::AddPetCostume )
            .def( "ClearPetCostume",                &KPetHelper::ClearPetCostume )
            .def( "DumpPetExpBonusInfo",            &KPetHelper::DumpPetExpBonusInfo )
            .def( "ClearPetVestedItem",             &KPetHelper::ClearPetVestedItem )
            .def( "AddPetVestedItem",               &KPetHelper::AddPetVestedItem ),

        luabind::def( "GetLogMgr", &KLogManager::GetInstance ),
        luabind::class_<KLogManager>("KLogManager")
            .def( luabind::tostring(luabind::self) )
            .def( "ChangeLogFile", &KLogManager::ChangeFile )
            .def( "ToggleWrite", &KLogManager::ToggleWrite ),

        luabind::def( "GetChannelDrop", &KChannelDrop::GetInstance ),
        luabind::class_<KChannelDrop>("KChannelDrop")
            .def( luabind::tostring(luabind::self) )
            .def( "Clear", &KChannelDrop::Clear)
            .def( "AddChannelDrop", &KChannelDrop::AddChannelDrop),

        luabind::def( "GetEnchantStrong", &KEnchantStrong::GetInstance ),
        luabind::class_<KEnchantStrong>( "KEnchantStrong" )
            .def( luabind::tostring(luabind::self) )
            .def( "SetEnchantAction",       &KEnchantStrong::SetEnchantAction )
            .def( "SetEnchantFailResult",   &KEnchantStrong::SetEnchantFailResult )
            .def( "SetEnchantRatio",        &KEnchantStrong::SetEnchantRatio )
            .def( "SetEnchantMaterialCount",&KEnchantStrong::SetEnchantMaterialCount )
            .def( "SetCoreItemID",          &KEnchantStrong::SetCoreItemID )
            .def( "AddStronAssist",         &KEnchantStrong::AddStronAssist )
            .def( "ClearEnchantInfo",       &KEnchantStrong::ClearEnchantInfo )
            .def( "ClearMaterialCount",     &KEnchantStrong::ClearMaterialCount )
            .def( "ClearAssistInfoTable",   &KEnchantStrong::ClearAssistInfoTable )
            .def( "ClearEnchantLimitLv",    &KEnchantStrong::ClearEnchantLimitLv )
            .def( "ClearEquipItemID",       &KEnchantStrong::ClearEquipItemID )
            .def( "ClearEnchantAssist",     &KEnchantStrong::ClearEnchantAssist )
            .def( "AddEnchantEnableItemID", &KEnchantStrong::AddEnchantEnableItemID )
            .def( "AddEnchantAssistItem",   &KEnchantStrong::AddEnchantAssistItem )
            .def( "SetEnchantBonusRate",    &KEnchantStrong::SetEnchantBonusRate )
            .def( "AddStrongAssistByCharacter",    &KEnchantStrong::AddStrongAssistByCharacter )
            .def( "EnchantTest",            &KEnchantStrong::EnchantTest )
            .def( "ClearStrongMaterial",    &KEnchantStrong::ClearStrongMaterial )
            .def( "SetStrongMaterial",      &KEnchantStrong::SetStrongMaterial )
            .def( "ClearStrongAssistWeak",  &KEnchantStrong::ClearStrongAssistWeak )
            .def( "SetStrongAssistWeak",    &KEnchantStrong::SetStrongAssistWeak )
            .def( "ClearStrongLimitLevel",  &KEnchantStrong::ClearStrongLimitLevel )
            .def( "SetStrongLimitLevel",    &KEnchantStrong::SetStrongLimitLevel )
            .def( "ClearSpecificLevelEnchant", &KEnchantStrong::ClearSpecificLevelEnchant )
            .def( "SetSpecificLevelEnchant",   &KEnchantStrong::SetSpecificLevelEnchant )
            .def( "GetSpecificLevelEnchant",   &KEnchantStrong::GetSpecificLevelEnchant )

            .enum_( "ENCHANT_GRADE" )
            [
                luabind::value( "FIRST",        KEnchantStrong::EG_FIRST ),
                luabind::value( "SECOND",       KEnchantStrong::EG_SECOND ),
                luabind::value( "THIRD",        KEnchantStrong::EG_THIRD )
            ]
            .enum_( "ENCHANT_KIND" )
            [
                luabind::value( "FIRE",             KEnchantStrong::EK_FIRE ),
                luabind::value( "ICE",              KEnchantStrong::EK_ICE ),
                luabind::value( "LIGHT",            KEnchantStrong::EK_LIGHT ),
                luabind::value( "DARK",             KEnchantStrong::EK_DARK ),
                luabind::value( "WIND",             KEnchantStrong::EK_WIND ),
                luabind::value( "CASH_SHIELD",      KEnchantStrong::EK_CASH_SHIELD ),
                luabind::value( "GP_SHIELD",        KEnchantStrong::EK_GP_SHIELD ),
                luabind::value( "SHINE",            KEnchantStrong::EK_SHINE ),
                luabind::value( "LOVE",             KEnchantStrong::EK_LOVE ),
                luabind::value( "ROSE",             KEnchantStrong::EK_ROSE ),
                luabind::value( "DEATH",            KEnchantStrong::EK_DEATH ),
                luabind::value( "TIME",             KEnchantStrong::EK_TIME ),
                luabind::value( "LIFE",             KEnchantStrong::EK_LIFE ),
                luabind::value( "DARK_DRAGON_RED",  KEnchantStrong::EK_DARK_DRAGON_RED ),
                luabind::value( "DARK_DRAGON_BLUE", KEnchantStrong::EK_DARK_DRAGON_BLUE ),
                luabind::value( "ARIES_SHIELD",     KEnchantStrong::EK_ARIES_SHIELD ),
                luabind::value( "TAURUS_SHIELD",    KEnchantStrong::EK_TAURUS_SHIELD ),
                luabind::value( "ZORRO_SHIELD",     KEnchantStrong::EK_ZORRO_SHIELD ),
                luabind::value( "GEMINI_SHIELD",        KEnchantStrong::EK_GEMINI_SHIELD ),
                luabind::value( "CANCER_SHIELD",        KEnchantStrong::EK_CANCER_SHIELD ),
                luabind::value( "LEO_SHIELD",           KEnchantStrong::EK_LEO_SHIELD ),
                luabind::value( "VIRGO_SHIELD",         KEnchantStrong::EK_VIRGO_SHIELD ),
                luabind::value( "LIBRA_SHIELD",         KEnchantStrong::EK_LIBRA_SHIELD ),
                luabind::value( "SCORPIO_SHIELD",       KEnchantStrong::EK_SCORPIO_SHIELD ),
                luabind::value( "SAGITTARIUS_SHIELD",   KEnchantStrong::EK_SAGITTARIUS_SHIELD ),
                luabind::value( "CAPRICORN_SHIELD",     KEnchantStrong::EK_CAPRICORN_SHIELD ),
                luabind::value( "AQUARIUS_SHIELD",      KEnchantStrong::EK_AQUARIUS_SHIELD ),
                luabind::value( "PISCES_SHIELD",        KEnchantStrong::EK_PISCES_SHIELD ),
                luabind::value( "HALLOWEEN",            KEnchantStrong::EK_HALLOWEEN ),
                luabind::value( "ELIA",                 KEnchantStrong::EK_ELIA ),
                luabind::value( "ELIA_ANKLE",           KEnchantStrong::EK_ELIA_ANKLE ),
                luabind::value( "ANKLE_10TH",           KEnchantStrong::EK_10TH_ANKLE ),
                luabind::value( "ANKLE_DIMENSION",      KEnchantStrong::EK_DIMENSION_ANKLE ),
                luabind::value( "ANKLE_PEGASUS",      KEnchantStrong::EK_PEGASUS_ANKLE ),
                luabind::value( "NECK_PEGASUS",      KEnchantStrong::EK_PEGASUS_NECK )
            ]
            .enum_( "ENCHANT_ACTION" )
            [
                luabind::value( "SUCCESS",      KEnchantStrong::EA_SUCCESS ),
                luabind::value( "WEAKNESS",     KEnchantStrong::EA_WEAKNESS ),
                luabind::value( "BREAK",        KEnchantStrong::EA_BREAK ),
                luabind::value( "NONE",         KEnchantStrong::EA_NONE ),
                luabind::value( "DROP",         KEnchantStrong::EA_DROP )
            ]
            .enum_( "ItemLevel" )
            [
                luabind::value( "Level1",       1 ),
                luabind::value( "Level2",       2 ),
                luabind::value( "Level3",       3 ),
                luabind::value( "Level4",       4 ),
                luabind::value( "Level5",       5 ),
                luabind::value( "Level6",       6 ),
                luabind::value( "Level7",       7 ),
                luabind::value( "Level8",       8 ),
                luabind::value( "Level9",       9 )
            ]
            .enum_( "MATERIAL_INDEX" )
            [
                // 강화재료 아이템 추가되면 여기도 추가.
                luabind::value( "GEM",          0 ),
                luabind::value( "CHOCO",        1 ),
                luabind::value( "PREMIUM_GEM",  2 )
            ],

        luabind::def( "GetMiniGame", &KMiniGameRankManager::GetInstance ),
        luabind::class_<KMiniGameRankManager>("KMiniGameRankManager")
        .def( luabind::tostring(luabind::self) )
        .def( "DumpMiniRank", &KMiniGameRankManager::DumpMiniRank ),

        luabind::def( "GetCollectionManager", &KCollectionManager::GetInstance ),
        luabind::class_<KCollectionManager>("KCollectionManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCollectionManager::LoadScript ),

        luabind::def( "GetPayLetterBillingManager", &KTThreadManager<KPayLetterBilling>::GetInstance ),
        luabind::def( "Init_PayLetterBillingInfo" , &KPayLetterBilling::InitPayLetterBillingInfo ),
        luabind::class_<KTThreadManager<KPayLetterBilling>, KThreadManager>( "PayLetterBillingManager" )
            .def( luabind::tostring( luabind::self ) ),

        luabind::def( "GetWorkerManager", &KTThreadManager<KWorker>::GetInstance ),
        luabind::class_<KTThreadManager<KWorker>, KThreadManager>( "KWorkerManager" )
            .def( luabind::tostring(luabind::self ) ),

        luabind::def( "GetVirtualCash", &KVirtualCash::GetInstance ),
        luabind::class_<KVirtualCash>("KVirtualCash")
            .def( luabind::tostring(luabind::self) )
            .def( "SetLimitRatio", &KVirtualCash::SetLimitRatio ),

        luabind::def( "GetTHBillingManager", &KTThreadManager<KTH_PayLetterBilling>::GetInstance ),
        luabind::def( "Init_THBillingInfo" , &KTH_PayLetterBilling::InitPayLetterBillingInfo ),
        luabind::class_<KTThreadManager<KTH_PayLetterBilling>, KThreadManager>( "KTH_PayLetterBilling" )
            .def( luabind::tostring( luabind::self ) ),

        luabind::def( "GetGuildManager", &KGuildManager::GetInstance ),
        luabind::class_<KGuildManager>( "KGuildManager" )
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KGuildManager::LoadScript )
            .def( "PrintGuild", &KGuildManager::PrintGuild )
            .def( "DumpPage", &KGuildManager::DumpGuildPage )
            .def( "DumpPageSort", &KGuildManager::DumpGuildPageSort )
            .def( "SetBuildCheck", &KGuildManager::SetBuildCheck )
            .def( "SetBattlePoint", &KGuildManager::SetBattlePoint )
            .def( "DumpStoreList", &KGuildManager::DumpStoreList )
            .def( "SetGuildClearEnable", &KGuildManager::SetGuildClearEnable )
            .def( "UpdateGuild", &KGuildManager::UpdateGuildInfoByLua )
            .def( "SendToGuildInfoCenter", &KGuildManager::SendToGuildInfoCenter ),

        luabind::def( "GetBRBillingManager", &KTThreadManager<KBR_PayLetterBilling>::GetInstance ),
        luabind::def( "Init_BRBillingInfo" , &KBR_PayLetterBilling::InitPayLetterBillingInfo ),
        luabind::class_<KTThreadManager<KBR_PayLetterBilling>, KThreadManager>( "KBR_PayLetterBilling" )
            .def( luabind::tostring( luabind::self ) ),

        luabind::def( "GetIDBillingManager", &KTThreadManager<KID_PayLetterBilling>::GetInstance ),
        luabind::def( "Init_IDBillingInfo" , &KID_PayLetterBilling::InitPayLetterBillingInfo ),
        luabind::class_<KTThreadManager<KID_PayLetterBilling>, KThreadManager>( "KID_PayLetterBilling" )
            .def( luabind::tostring( luabind::self ) ),

        luabind::def( "GetNCashBillingManager", &KTThreadManager<KNCashBilling>::GetInstance ),
        luabind::def( "Init_NCashBillingInfo", &KNCashBilling::InitNCashBillingInfo ),
        luabind::class_<KTThreadManager<KNCashBilling>, KThreadManager>( "NCashBillingManager" )
            .def( luabind::tostring(luabind::self ) ),

        luabind::def( "GetNetCafeManager", &KTThreadManager<NetCafeManager>::GetInstance ),
        luabind::class_<KTThreadManager<NetCafeManager>, KThreadManager>( "NetCafeManager" )
            .def( luabind::tostring(luabind::self ) ),

        luabind::def( "GetPlayAuthManager", &KPlayAuthManager::GetInstance ),
        luabind::class_<KPlayAuthManager>("KPlayAuthManager")
            .def( luabind::tostring(luabind::self) )
            .def( "SetUsePlayAuthSystem", &KPlayAuthManager::SetUsePlayAuthSystem )
            .def( "IsUsePlayAuthSystem", &KPlayAuthManager::IsUsePlayAuthSystem )
            .def( "SetAutoReconnect", &KPlayAuthManager::SetAutoReconnect )
            .def( "IsAutoReconnect", &KPlayAuthManager::IsAutoReconnect )
            .def( "SetAutoReconnectGap", &KPlayAuthManager::SetAutoReconnectGap )
            .def( "GetAutoReconnectGap", &KPlayAuthManager::GetAutoReconnectGap )
            .def( luabind::tostring(luabind::self ) ) //,

        /*luabind::def( "GetCYOUBillingManager", &KTThreadManager<CYOUBilling>::GetInstance ),
        luabind::def( "InitCYOUBillingInfo", &CYOUBilling::InitCYOUBillingInfo ),
        luabind::class_<KTThreadManager<CYOUBilling>, KThreadManager>( "CYOUBilling" )
            .def( luabind::tostring(luabind::self) )*/

    ];
}

void KGSSimLayer::Init()
{
    m_smapNickToLogin.clear();

    m_vecpThreadMgr.push_back( KTThreadManager<KGSGash>::GetInstance() );
    m_vecpThreadMgr.push_back( KTThreadManager<KGashBilling>::GetInstance() );  
    m_vecpThreadMgr.push_back( KTThreadManager<KBR_PayLetterBilling>::GetInstance() );
    m_vecpThreadMgr.push_back( KTDBLayer<KEvtDBThread>::GetInstance() );
    m_vecpThreadMgr.push_back( KTThreadManager<KPayLetterBilling>::GetInstance() );
    m_vecpThreadMgr.push_back( KTThreadManager<KTH_PayLetterBilling>::GetInstance() );
    m_vecpThreadMgr.push_back( KTThreadManager<KWorker>::GetInstance() );
    m_vecpThreadMgr.push_back( KTThreadManager<KID_PayLetterBilling>::GetInstance() );
    m_vecpThreadMgr.push_back( KTThreadManager<KNCashBilling>::GetInstance() );
    m_vecpThreadMgr.push_back( KTThreadManager<NetCafeManager>::GetInstance() );
    //m_vecpThreadMgr.push_back( KTThreadManager<CYOUBilling>::GetInstance() );

    KSimLayer::Init();

    // UserFactory 생성. KUser가 아닌 다른 객체군을 생성한다면, 코드 구현 후 이곳 한줄만 변경하면 됨.
    // Abstract Factory Patern.
    m_kActorManager.SetActorFactory( IActorFactoryPtr( new KActorFactory<KUser, KUserFSM, KUser::UserPoolTag> ) );

    //SiKChannelManager()->InitChannels();
    SiKLogManager()->Init();

    Enable_LFH();
}

void KGSSimLayer::Tick()
{
    BEGIN_TRY_BLOCK;

    DWORD   dwElapsedTime = ::GetTickCount();
    FILETIME ftKernelTimeStart, ftKernelTimeEnd, ftUserTimeStart, ftUserTimeEnd;
    FILETIME ftDummy;
    ::GetThreadTimes( ::GetCurrentThread(), &ftDummy, &ftDummy, &ftKernelTimeStart, &ftUserTimeStart );

    SiKCnConnector()->Tick();
    SiKAgentConnector()->Tick();
    SiKMatchConnector()->Tick(); // MatchConnector Tick.

    KSimLayer::Tick();

    SiKTickManager()->UpdateAccumulateData( m_dwAccumulateTick, m_nAccumulateEvent );
    SiKTickManager()->UpdateAllActorTick( m_dwElapsedTick, m_dwKernelTick, m_dwUserTick );

    SiKChannelManager()->Tick();

    {
        Checker performance( std::wstring(L"GuildManager Tick"), 1000);
        SiKGuildManager()->Tick();
    }
    
    {
        Checker performance( std::wstring(L"GameServer Tick"), 1000);
        SiKGameServer()->Tick();
    }

    SiKFailRate()->Tick();
    SiKServerOpenTime()->Tick();

    KRoom::UpdateStatToDB();

    if( ::GetTickCount() - GetTick( TICK_LOUD_MSG ) > 1000 * 60 * 10 ) // 10분
    {
        SiKGSDBLayer()->QueueingID( KUserEvent::DB_EVENT_UPDATE_LOUD_MSG, L"", 0, 0 );
        SetTick( TICK_LOUD_MSG );
    }

    if( SiKGCPoint()->IsGCPointSwitch() && ::GetTickCount() - GetTick( TICK_CALENDAR_REWARD ) > 1000 * 60 * 60 ) // 한시간.
    {
        SiKGSDBLayer()->QueueingID( KUserEvent::DB_EVENT_UPDATE_ATTENDANCE_REWARD_ITEM, L"", 0, 0 );//주/월개근 보상 아이템 얻어오기.
        SiKGSDBLayer()->QueueingID( KUserEvent::DB_EVENT_UPDATE_LUCKYDAY_LIST, L"", 0, 0 );
        SetTick( TICK_CALENDAR_REWARD );
    }
    
    if( ::GetTickCount() - GetTick( TICK_SYNC_TIME ) > 1000 * 60 * 60 * 3 )
    {
        SiKGSDBLayer()->QueueingID( KUserEvent::DB_EVENT_TIME_SYNCRONIZE_NOT, L"", 0, 0 );
        SetTick( TICK_SYNC_TIME );
    }

    if( ::GetTickCount() - GetTick( TICK_LOADING_TIME ) > 1000 * 60 * 60 )
    {
        KLocker lock( m_csStatLoadingTime);
        if( !m_mapStatLoadingTime.empty() )
        {
            SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_DB_STAT_LOADING_TIME_NOT, L"", 0, 0, m_mapStatLoadingTime );
            m_mapStatLoadingTime.clear();
        }
        SetTick( TICK_LOADING_TIME );
    }

    if( ::GetTickCount() - GetTick( TICK_CHAT_EVENT ) > GetChatEventGap() )
    {
        SiKGSDBLayer()->QueueingID( KUserEvent::EVENT_GET_CHATTING_EVENT_MSG_REQ, L"", 0, 0 );
        SetTick( TICK_CHAT_EVENT );
    }

    if( ::GetTickCount() - GetTick( TICK_TIME_EVENT ) > 1000 * 60 )
    {
        SiKGSDBLayer()->QueueingID( KUserEvent::DB_EVENT_TIME_LIMIT_EVENT_DATA, L"", 0, 0 );
        SetTick( TICK_TIME_EVENT );

    }

    if( ::GetTickCount() - GetTick( TICK_BILLING_ERROR ) > 1000 * 60 * 60 )
    {
        SiKBillingError()->ReportToDB(); // 이 방식...좀 더 생각해보자.
        SetTick( TICK_BILLING_ERROR );
    }


    if( ::GetTickCount() - GetTick( TICK_INDIGO_RANK ) > 3 * 60 * 60 * 1000 && CheckTypeFlag( ST_INDIGO ) )
    {        
        SiKGSDBLayer()->QueueingID( KUserEvent::EVENT_INDIGO_RANK_NOT, L"", 0, 0 );
        SetTick( TICK_INDIGO_RANK );
    }

    if( ::GetTickCount() - GetTick( TICK_P2P_STAT ) > 1000 * 60 * 60 && IsEnableP2PStat() )
    {
        WriteP2PLog();
        SetTick( TICK_P2P_STAT );
    }

    if( ::GetTickCount() - GetTick( TICK_PERIOD_NOTICE ) > 60 * 1000 )
    {
        SiKGSDBLayer()->QueueingID( KUserEvent::DB_EVENT_UPDATE_PERIOD_NOTICE, L"", 0, 0 );
        SetTick( TICK_PERIOD_NOTICE );
    }

    if( ::GetTickCount() - GetTick( TICK_CHAT_LOG ) > 24 * 60 * 60 * 1000 )
    {
        KTThreadManager<KWorker>::GetInstance()->QueueingID( KUserEvent::EVENT_CHANGE_CHAT_LOG_NOT, L"", 0, 0 );
        SetTick( TICK_CHAT_LOG );
    }

    if( ::GetTickCount() - GetTick( TICK_SANTA ) > 1000 * 60 )
    {
        SiKGSDBLayer()->QueueingID( KUserEvent::DB_EVENT_SANTA_DATA, L"", 0, 0 );
        SetTick( TICK_SANTA );
    }

    if( ::GetTickCount() - GetTick( TICK_VIRTUAL_CASH_RESTRICT_ITEM ) > 1000 * 60 * 60 )
    {        
        SiKGSDBLayer()->QueueingID( KUserEvent::DB_EVENT_VIRTUAL_CASH_RESTRICT_ITEM_LIST, L"", 0, 0 );
        SetTick( TICK_VIRTUAL_CASH_RESTRICT_ITEM );
    }

    if( ::GetTickCount() - GetTick( TICK_DB_QUEUE_SIZE ) > 1000 * 60 )
    {        
        int nQueueCount = SiKGSDBLayer()->GetQueueSize();
        
        std::wstringstream stm;
        stm << L"[" << KNC_TIME_STRING << L"], " << nQueueCount;
        KTThreadManager<KWorker>::GetInstance()->QueueingEvent( KUserEvent::EVENT_DB_QUEUE_SIZE_LOG, L"", 0, 0, stm.str() );

        if( m_bLimitAcceptEnable ) {
            if ( nQueueCount > m_nLimitAcceptQueueCount ) {
                m_kActorManager.SetCreateActorEnable( false );
            }
            else {
                m_kActorManager.SetCreateActorEnable( true );
            }
        }

        stm.clear();
        stm.str(L"");

        SetTick( TICK_DB_QUEUE_SIZE );
    }

    if ( m_kTimer_DBQ_Alert.CheckTime() ) {
        int nQueueCount = SiKGSDBLayer()->GetQueueSize();

        if ( nQueueCount > m_nLimitAcceptQueueCount ) {
            std::wstringstream stm;
            stm << nQueueCount;
            KTThreadManager<KWorker>::GetInstance()->QueueingEvent( KUserEvent::EVENT_ALERT_EXCEED_DB_QUEUE, L"", 0, 0, stm.str() );
            START_LOG( clog, L"Alert DB Queue size : " << nQueueCount << L" (limit:" << m_nLimitAcceptQueueCount << L")" ) << END_LOG;

            stm.clear();
            stm.str(L"");
        }
    }

    ::GetThreadTimes( ::GetCurrentThread(), &ftDummy, &ftDummy, &ftKernelTimeEnd, &ftUserTimeEnd );
    __int64 qwKernelTimeElapsed = (::Int64ShllMod32( ftKernelTimeEnd.dwHighDateTime, 32) | ftKernelTimeEnd.dwLowDateTime ) - (::Int64ShllMod32( ftKernelTimeStart.dwHighDateTime, 32) | ftKernelTimeStart.dwLowDateTime );
    __int64 qwUserTimeElapsed = (::Int64ShllMod32( ftUserTimeEnd.dwHighDateTime, 32) | ftUserTimeEnd.dwLowDateTime ) - (::Int64ShllMod32( ftUserTimeStart.dwHighDateTime, 32) | ftUserTimeStart.dwLowDateTime );
    DWORD dwKernelTimeElapsed = static_cast<DWORD>(qwKernelTimeElapsed / 10000);
    DWORD dwUserTimeElapsed = static_cast<DWORD>(qwUserTimeElapsed / 10000);

    dwElapsedTime = ::GetTickCount() - dwElapsedTime;
    if( dwElapsedTime > m_dwMaxTickDuration )
    {
        KLocker lock( m_csMaxTick );
        m_dwMaxTickDuration = dwElapsedTime;
        m_tmMaxTickTime = CTime::GetCurrentTime();
    }
    SiKTickManager()->UpdateSimLayerTick( dwElapsedTime, dwKernelTimeElapsed, dwUserTimeElapsed );

    END_TRY_BLOCK( "" );
}

void KGSSimLayer::ShutDown()
{
    SiKFailRate()->EndDataToDB(); // Record To Log File & Write DB

    KSimLayer::ShutDown();
    KChannelManager::ReleaseInstance();
    KChannel::ms_spChannelLobby.reset();
    KGuildManager::ReleaseInstance();
    KFailRate::ReleaseInstance();
    KTThreadManager<KGSGash>::ReleaseInstance();
    KTThreadManager<KGashBilling>::ReleaseInstance();
    KTThreadManager<KBR_PayLetterBilling>::ReleaseInstance();
    KTThreadManager<KID_PayLetterBilling>::ReleaseInstance();

    KTThreadManager<KPayLetterBilling>::ReleaseInstance();
    KTThreadManager<KTH_PayLetterBilling>::ReleaseInstance();
    KTDBLayer<KEvtDBThread>::ReleaseInstance();
    KServerOpenTime::ReleaseInstance();
    KFatigueSystem::ReleaseInstance();
    KPetHelper::ReleaseInstance();
    KLogManager::ReleaseInstance();
    KChannelDrop::ReleaseInstance();
    KEnchantStrong::ReleaseInstance();
    KCnConnector::ReleaseInstance();
    KBillingError::ReleaseInstance();
    KMiniGameRankManager::ReleaseInstance();
    KLoudMessage::ReleaseInstance();
    KPeriodNotice::ReleaseInstance();
    KTThreadManager<KWorker>::ReleaseInstance();
    KVirtualCash::ReleaseInstance();
    KTThreadManager<KNCashBilling>::ReleaseInstance();
    KTThreadManager<NetCafeManager>::ReleaseInstance();
    //KTThreadManager<CYOUBilling>::ReleaseInstance();
}

bool KGSSimLayer::DestroyAllUsers()
{
    m_kActorManager.SetCreateActorEnable( false );

    START_LOG( cerr, L"서버 종료. 현재 유저 : " << m_kActorManager.GetCount() << L" 명" ) << END_LOG;

    m_kActorManager.ForEach( boost::bind( &KActor::ReserveDestroy, _1, 0 ) );

    DWORD dwBeginTick = ::GetTickCount();

    KDBLayer* pkDBLayer = SiKGameServer()->GetDBLayer();

    if( !pkDBLayer )    return false; //050706. microcat.

    // 모든 유저 객체가 소멸되어야 하고 DB 작업이 모두 완료되어야 함
    while( ( m_kActorManager.GetCount() != 0 || pkDBLayer->GetQueueSize() != 0 ) &&
           ::GetTickCount() - dwBeginTick < m_dwShutdownTimeLimit )   // 하지만 일정 시간이 지나면 그냥 통과.
    {
        ::Sleep( 500 );
    }

    if( m_kActorManager.GetCount() == 0 && pkDBLayer->GetQueueSize() == 0 )
    {
        START_LOG( cerr, L"모든 유저 정상 종료" )
            << L"Elapsed Time : " << ::GetTickCount() - dwBeginTick << dbg::endl
            << L"Time Limit : " << m_dwShutdownTimeLimit << dbg::endl
            << L"non-safed release count : " << m_kActorManager.GetCount() << END_LOG;
        return true;
    }

    START_LOG( cerr, L"캐싱 정보를 모두 저장하지 못하고 종료" )
        << L"Elapsed Time : " << ::GetTickCount() - dwBeginTick << dbg::endl
        << L"Time Limit : " << m_dwShutdownTimeLimit << dbg::endl
        << L"non-safed release count : " << m_kActorManager.GetCount() << dbg::endl
        << L"non-processed DB event : " << pkDBLayer->GetQueueSize() << END_LOG;

    return false;
}

void KGSSimLayer::AddNickToLogin( std::wstring& strLogin, std::wstring& strNick )
{
    if( strNick.empty() )   return;

    wchar_t szBuff[MAX_PATH] = {0, };

    int iRet = ::LCMapString( LOCALE_SYSTEM_DEFAULT, LCMAP_HALFWIDTH | LCMAP_LOWERCASE, strNick.c_str(), strNick.size(), szBuff, MAX_PATH );

    if( iRet == 0 )
    {
        START_LOG( cerr, L"요상한 닉네임을 변환하려 하였음." )
            << BUILD_LOG( strLogin )
            << BUILD_LOG( strNick )
            << BUILD_LOG( szBuff )
            << BUILD_LOG( ::GetLastError() )
            << END_LOG;
        return;
    }

    m_smapNickToLogin[ std::wstring(szBuff) ] = strLogin;
}

void KGSSimLayer::RemoveNickToLogin( std::wstring& strNick )// 060103. kkurrung.확인
{
    if( strNick.empty() )   return;

    wchar_t szBuff[MAX_PATH] = {0, };

    int iRet = ::LCMapString( LOCALE_SYSTEM_DEFAULT, LCMAP_HALFWIDTH | LCMAP_LOWERCASE, strNick.c_str(), strNick.size(), szBuff, MAX_PATH );

    if( iRet == 0 )
    {
        START_LOG( cerr, L"요상한 닉네임을 변환하려 하였음." )
            << BUILD_LOG( strNick )
            << BUILD_LOG( szBuff )
            << BUILD_LOG( ::GetLastError() )
            << END_LOG;
        return;
    }

    m_smapNickToLogin.erase( std::wstring(szBuff) );
}

std::wstring KGSSimLayer::TranslateNickToLogin( std::wstring& strNick ) // 060103. kkurrung.확인
{
    wchar_t szBuff[MAX_PATH] = {0, };

//{{ 060103. kkurrung.
     int iRet = ::LCMapString( LOCALE_SYSTEM_DEFAULT, LCMAP_HALFWIDTH | LCMAP_LOWERCASE, strNick.c_str(), strNick.size(), szBuff, MAX_PATH );
//}} kkurrung.

    if( iRet == 0 )
    {
        START_LOG( cerr, L"요상한 닉네임을 변환하려 하였음." )
            << BUILD_LOG( strNick )
            << BUILD_LOG( strNick.size() )
            << BUILD_LOG( szBuff )
            << BUILD_LOG( ::GetLastError() )
            << END_LOG;
        return strNick;
    }

    std::map<std::wstring, std::wstring>::iterator    mit;
    mit = m_smapNickToLogin.find( std::wstring(szBuff) );
    if( mit == m_smapNickToLogin.end() )
    {
        return std::wstring();
    }
    else
    {
        return  mit->second;
    }
}

void KGSSimLayer::SetHBGap( DWORD dwHBGap )
{
    DWORD dwOldHBGap = KSession::m_dwHBGap;
    KSession::m_dwHBGap = dwHBGap;
    START_LOG( cerr, L"HB Gap Change " << dwOldHBGap << " --> " << dwHBGap ) << END_LOG;
}

void KGSSimLayer::SetKairo( IN int iItemID_, IN const Kairo& kKairo_ )
{
    m_mapKairo.insert( std::make_pair(iItemID_, kKairo_ ) );
}

bool KGSSimLayer::CheckKairo( IN GCITEMID dwItemID_, OUT std::vector<KDropItemInfo>& vecItem_, OUT DWORD& dwKairoType_ )
{
    vecItem_.clear();
    KDropItemInfo kDropItemInfo;
    std::map<int,Kairo>::const_iterator mit;
    mit = m_mapKairo.find( dwItemID_ );

    if( mit == m_mapKairo.end() ) {
        // 요청받은 아이템 아이디의 변환 정보를 찾을 수 없음.
        return false;
    }

    dwKairoType_ = mit->second.GetKairoType(); // 보상 타입 넣어주기.
    if( mit->second.GetKairoType() == Kairo::RT_STRENGTH ) {
        if( !mit->second.Do( vecItem_ ) ) {
            return false;
        }
    } else {
        if( !mit->second.Do( kDropItemInfo ) ) { // 보상받을 아이템의 존재 유무가 return 됨.
            return false;
        }
        if( kDropItemInfo.m_ItemID != 0 ) {
            vecItem_.push_back( kDropItemInfo );
        }
    }
    return true;
}

bool KGSSimLayer::CheckOneShotKairo( IN GCITEMID dwItemID_, IN int iCount, OUT std::vector<KDropItemInfo>& vecItem_, OUT DWORD& dwKairoType_ )
{
    vecItem_.clear();
    KDropItemInfo kDropItemInfo;
    std::map< int, Kairo >::const_iterator mit;
    int iCnt = 0;

    mit = m_mapKairo.find( dwItemID_ );
    if ( m_mapKairo.end() == mit ) {
        // 요청받은 아이템 아이디의 변환 정보를 찾을 수 없음.
        return false;
    }

    dwKairoType_ = mit->second.GetKairoType(); // 보상 타입 넣어주기.
    for ( int i = 1; i <= iCount; ++i ) {
        if ( mit->second.GetKairoType() == Kairo::RT_STRENGTH ) {
            if ( false == mit->second.Do( vecItem_ ) ) {
                return false;
            }
        } else {
            if ( false == mit->second.Do( kDropItemInfo ) ) { // 보상받을 아이템의 존재 유무가 return 됨.
                return false;
            }
            if ( 0 != kDropItemInfo.m_ItemID ) {
                vecItem_.push_back( kDropItemInfo );
            }
        }
    }

    return true;
}

void KGSSimLayer::PrintKairo()
{
    std::wstringstream stm;
    stm << L"   Kairo Information" << std::endl;
    std::map<int,Kairo>::const_iterator mit;
    for( mit = m_mapKairo.begin(); mit != m_mapKairo.end(); mit++ )
    {
        stm << L"  ID : " << mit->first << std::endl;

        mit->second.ToString( stm );
    }
    std::cout << KncUtil::toNarrowString( stm.str() );
    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

// 070314. kkurrung. 신규 유저 강제로 특정 모드 플레이 하도록 하는 것.
bool KGSSimLayer::IsOldUser( IN time_t tmFirstLoginTime_ )
{
    CTime timeFirstLoginTime( tmFirstLoginTime_ );
    KLocker lock( m_csForceNewUser );
    return timeFirstLoginTime < m_tmForceTime;
}
void KGSSimLayer::SetForceNewUser( bool bEnable_, int nYear_, int nMonth_, int nDay_ ) // for Lua
{
    KLocker lock( m_csForceNewUser );
    m_bEnableForce = bEnable_;
    m_tmForceTime = CTime( nYear_, nMonth_, nDay_, 0, 0, 0 );
}
void KGSSimLayer::AddForceMode( int nModeID_ ) // for Lua
{
    KLocker lock( m_csForceNewUser );
    if( GC_GM_TEAM_MODE <= nModeID_ && NUM_GC_GAME_MODE > nModeID_ )
        m_setForceModeID.insert( nModeID_ );
}
void KGSSimLayer::ClearForceMode()// for Lua
{
    KLocker lock( m_csForceNewUser );
    m_setForceModeID.clear();
}

bool KGSSimLayer::CheckTableChecksum( DWORD dwTableChecksum_ )
{
    if( m_dwTableChecksum == 0L )
        return true;

    if( m_dwTableChecksum != dwTableChecksum_ )
        return false;

    return true;
}

void KGSSimLayer::SetSHAdhustTime( int nYear_, int nMonth_, int nDay_, int nHour_, int nMinute_ )
{
    CTime tmShAdjustTime( nYear_, nMonth_, nDay_, nHour_, nMinute_, 00 );
    KLocker lock( m_cstmSHAdjust );
    m_tmSHAdjust = tmShAdjustTime;
}

CTime KGSSimLayer::GetSHAdhustTime()
{
    CTime tmRet;
    KLocker lock( m_cstmSHAdjust );
    tmRet = m_tmSHAdjust;

    return tmRet;
}

void KGSSimLayer::UpdateLuckyDays( std::vector<KSimpleDate>& vecLuckyDays_ )
{
    KLocker lock( m_csLuckyDays );
    m_vecLuckyDays = vecLuckyDays_;
}

bool KGSSimLayer::FindLuckyDay( KSimpleDate& kSimpleDate_ )
{
    KLocker lock( m_csLuckyDays );
    std::vector<KSimpleDate>::iterator vit;
    for( vit = m_vecLuckyDays.begin() ; vit != m_vecLuckyDays.end() ; ++vit )
    {
        if( *vit == kSimpleDate_ ) // 출첵한 날이 럭키데이 인가
            return true;
    }
    return false;
}

std::wostream& KGSSimLayer::GetMaxTickInfo( std::wostream& stm_ ) const
{
    KLocker lock( m_csMaxTick);
    return stm_ << L" -- Max Tick Info --"  << std::endl
        << L" Max Tick Elapsed Time : " << m_dwMaxTickDuration << std::endl
        << L" Max Tick Time : " << (LPCTSTR)m_tmMaxTickTime.Format( KNC_TIME_FORMAT );
}

void KGSSimLayer::AddLogingCount( DWORD dwLoadingSec_ )
{
    KLocker lock( m_csStatLoadingTime );
    m_mapStatLoadingTime[dwLoadingSec_] += 1;
}

void KGSSimLayer::SetChatEvent( IN KChatEventInfo& kChatEvent_ )
{
    if( kChatEvent_.m_vecEventMsg.empty() )
    {
        ClearChatEvent();
        START_LOG( cerr, L"채팅 이벤트 문자열이 존재하지 않음..." ) << END_LOG;
        return;
    }

    KLocker lock( m_csChatEvent );
    m_bEnableChatEvent = true;
    m_kChatEvent = kChatEvent_;
}

bool KGSSimLayer::GetChatEvent( OUT KChatEventInfo& kChatEvent_ )
{
    KLocker lock( m_csChatEvent );
    if( !m_bEnableChatEvent )
        return false;

    kChatEvent_ = m_kChatEvent;
    return true;
}

void KGSSimLayer::ClearChatEvent()
{
    KLocker lock( m_csChatEvent );
    m_bEnableChatEvent = false;
}

void KGSSimLayer::SetChatEventOption( DWORD dwAllowedTime_, int nMaxUserPvPNum_, int nMaxUserDungeonNum_ )
{
    JIF( nMaxUserPvPNum_ >= 2 && nMaxUserDungeonNum_ >= 2 );
    KLocker lock( m_csChatEvent );
    m_dwAllowedTime             = dwAllowedTime_;
    m_nChatEventPvPUserNum      = nMaxUserPvPNum_;
    m_nChatEventDungeonUserNum  = nMaxUserDungeonNum_;
}

void KGSSimLayer::AddP2PStat( int nPing_, int nLossCount_ )
{
    KLocker lock( m_csP2PStat );

    std::map< std::pair<int, int>, DWORD >::iterator mit;

    if( nPing_ < m_nMinPing ) // 관심있는 데이터 영역을 지정하자
        nPing_ = m_nMinPing;
    if( nPing_ > m_nMaxPing )
        nPing_ = m_nMaxPing;

    if( nLossCount_ > m_nMaxLossCount )
        nLossCount_ = m_nMaxLossCount;

    int nRemainder  = nPing_ % 10;
    nPing_ -= nRemainder;

    std::pair<int, int> key( nPing_, nLossCount_ );

    mit = m_mapStatP2P.find( key );
    if( mit == m_mapStatP2P.end() )
    {
        m_mapStatP2P.insert( std::make_pair( key, 1 ) );
    }
    else
    {
        m_mapStatP2P[key] += 1;
    }
}

void KGSSimLayer::WriteP2PLog()
{
    KLocker lock( m_csP2PStat );

    std::map< std::pair<int, int>, DWORD >::iterator mit;
    for( mit = m_mapStatP2P.begin(); mit != m_mapStatP2P.end(); ++mit )
    {
        std::wstringstream stm;

        stm << L"ping : " << mit->first.first << ", Loss count : " <<  mit->first.second << ", count : " << mit->second << std::endl
            << ", Time : " << KNC_TIME_STRING << std::endl
            << L"----" << std::endl;

        TQUEUEING_EVENT_TO_DB( KWorker, KUserEvent::EVENT_LT_P2P, std::wstring(L""), 0, 0, stm.str() );

        stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
        stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    }
    m_mapStatP2P.clear();
}

void KGSSimLayer::SetP2PStat( bool bEnable_,int nMinPing_, int nMaxPing_, int nMaxLossCount_ )
{
    KLocker lock( m_csP2PStat );

    m_bEnableP2PStat = bEnable_;
    m_nMinPing = nMinPing_;
    m_nMaxPing = nMaxPing_;
    m_nMaxLossCount = nMaxLossCount_;
}

bool KGSSimLayer::IsEnableP2PStat()
{
    KLocker lock( m_csP2PStat );
    return m_bEnableP2PStat;
}

void KGSSimLayer::UpdateAttendanceReward( IN std::vector<KAttendanceReward>& vecRewardItemList_ )
{
    std::map< int, std::vector<KItem> > mapWeeklyReward;
    std::map< int, std::vector<KItem> > mapMonthlyReward;
    mapWeeklyReward.clear();
    mapMonthlyReward.clear();

    for( std::vector<KAttendanceReward>::const_iterator cvit = vecRewardItemList_.begin() ;
        cvit != vecRewardItemList_.end() ; ++cvit )
    {
        if( cvit->m_nAttendanceType == KAttendanceReward::WEEKLY_ATTENDANCE )
        {
            mapWeeklyReward[cvit->m_nSupplyType].push_back( cvit->m_kRewardItem );
        }
        else if( cvit->m_nAttendanceType == KAttendanceReward::MONTHLY_ATTENDANCE )
        {
            mapMonthlyReward[cvit->m_nSupplyType].push_back( cvit->m_kRewardItem );
        }
        else
        {
            START_LOG( cerr, L"잘못된 출석 타입 : " << cvit->m_nAttendanceType )
                << BUILD_LOG( cvit->m_nSupplyType )
                << BUILD_LOG( cvit->m_kRewardItem.m_ItemID ) << END_LOG;
        }
    }

    START_LOG( clog, L"개근시 보상 아이템 목록 갱신됨." )
        << BUILD_LOG( mapWeeklyReward.size() )
        << BUILD_LOG( mapMonthlyReward.size() ) << END_LOG;

    KLocker lock( m_csAttendanceReward );
    m_mapWeeklyReward = mapWeeklyReward;
    m_mapMonthlyReward = mapMonthlyReward;
}

bool KGSSimLayer::GetAttendanceRewardList( IN std::map< int, std::vector<KItem> >& mapReward_, IN const int nRewardType_, OUT std::vector<KItem>& vecRewardList_ )
{
    std::map< int, std::vector<KItem> >::iterator mit;
    vecRewardList_.clear();

    KLocker lock( m_csAttendanceReward );

    if( mapReward_.empty() )
    {
        START_LOG( clog, L"보상 아이템 리스트가 없음 : " << nRewardType_ ) << END_LOG;
        return false;
    }

    mit = mapReward_.find( nRewardType_ );
    if( mit == mapReward_.end() )
    {
        START_LOG( clog, L"유저가 선택한 타입의 보상 아이템 리스트가 없음 : " << nRewardType_ ) << END_LOG;
        return false;
    }

    vecRewardList_ = mit->second;
    START_LOG( clog, L"보상 아이템 size : " << vecRewardList_.size() ) << END_LOG;
    if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) )
    {
        START_LOG( cwarn, L"--- Reward Item List ---" ) << END_LOG;
        for( std::vector<KItem>::iterator vit = vecRewardList_.begin() ;
            vit != vecRewardList_.end() ; ++vit )
        {
            START_LOG( cwarn, L" Item ID : " << vit->m_ItemID << L", Count : " << vit->m_nCount << L", Period : " << vit->m_nPeriod ) << END_LOG;
        }
    }
    return true;
}

bool KGSSimLayer::GetWeeklySelectRewardList( OUT std::vector<KItem>& vecWeeklySelectRewardList_ )
{
    return GetAttendanceRewardList( m_mapWeeklyReward, KAttendanceReward::SELECT_SUPPLY, vecWeeklySelectRewardList_ );
}

bool KGSSimLayer::GetAttendanceSelectRewardItem( IN std::map< int, std::vector<KItem> >& mapReward_, IN GCITEMID itemID_, OUT KItem& kReqItem_ )
{
    std::map< int, std::vector<KItem> >::iterator mit;
    std::vector<KItem>::iterator vit;

    KLocker lock( m_csAttendanceReward );

    mit = mapReward_.find( KAttendanceReward::SELECT_SUPPLY );
    if( mit == mapReward_.end() )
    {
        START_LOG( cwarn, L"선택보상 아이템 리스트가 없음 ItemID : " << itemID_ ) << END_LOG;
        return false;
    }

    vit = std::find_if( mit->second.begin(), mit->second.end(),
        boost::bind( &KItem::m_ItemID, _1 ) == itemID_ );

    if( vit == mit->second.end() )
    {
        START_LOG( cwarn, L"선택보상 아이템 리스트에 요청한 아이템이 없음 ItemID : " << itemID_ ) << END_LOG;
        return false;
    }

    kReqItem_ = *vit;
    START_LOG( clog, L"선택 보상 아이템 ID : " << kReqItem_.m_ItemID ) << END_LOG;
    if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) )
    {
        START_LOG( cwarn, L"--- Select Reward Item Info ---" )
            << BUILD_LOG( kReqItem_.m_ItemID )
            << BUILD_LOG( kReqItem_.m_nCount )
            << BUILD_LOG( kReqItem_.m_nPeriod ) << END_LOG;
    }
    return true;
}

bool KGSSimLayer::GetWeeklySelectRewardItem( IN GCITEMID itemID_, OUT KItem& kReqItem_ )
{
    return GetAttendanceSelectRewardItem( m_mapWeeklyReward, itemID_, kReqItem_ );
}

bool KGSSimLayer::GetWeeklyBaseRewardItem( OUT std::vector<KItem>& vecWeeklyBaseReward_ )
{
    return GetAttendanceRewardList( m_mapWeeklyReward, KAttendanceReward::BASE_SUPPLY, vecWeeklyBaseReward_ );
}

bool KGSSimLayer::GetAttdanceRandomRewardItem( IN std::map< int, std::vector<KItem> >& mapReward_, OUT KItem& kReqItem_ )
{
    std::map< int, std::vector<KItem> >::iterator mit;
    std::vector<KItem> vecRandomRewardList;
    vecRandomRewardList.clear();

    {
        KLocker lock( m_csAttendanceReward );
        mit = mapReward_.find( KAttendanceReward::RANDOM_SUPPLY );
        if( mit == mapReward_.end() )
        {
            START_LOG( clog, L"랜덤보상 아이템 리스트가 없음 : " << KAttendanceReward::RANDOM_SUPPLY ) << END_LOG;
            return false;
        }

        vecRandomRewardList = mit->second;
    }

    if( vecRandomRewardList.empty() )
    {
        START_LOG( clog, L"랜덤보상 아이템이 없음" )
            << BUILD_LOG( vecRandomRewardList.size() ) << END_LOG;
        return false;
    }

    //랜덤보상중 하나를 선택해 넣어준다.
    std::random_shuffle( vecRandomRewardList.begin(), vecRandomRewardList.end() );
    kReqItem_ = vecRandomRewardList[0];

    START_LOG( clog, L"랜덤 보상 아이템 ID : " << kReqItem_.m_ItemID ) << END_LOG;
    if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) )
    {
        START_LOG( cwarn, L"--- Random Reward Item Info ---" )
            << BUILD_LOG( kReqItem_.m_ItemID )
            << BUILD_LOG( kReqItem_.m_nPeriod )
            << BUILD_LOG( kReqItem_.m_nCount ) << END_LOG;
    }
    return true;
}

bool KGSSimLayer::GetWeeklyRandomRewardItem( OUT KItem& kReqItem_ )
{
    return GetAttdanceRandomRewardItem( m_mapWeeklyReward, kReqItem_ );
}

void KGSSimLayer::DumpWeeklyReward()
{
    std::map< int, std::vector<KItem> >::const_iterator cmit;
    std::vector<KItem>::const_iterator cvit;
    std::wstringstream stm;

    stm << L" -- Dump Weekly Reward Item List -- " << std::endl;

    {
        KLocker lock( m_csAttendanceReward );

        for( cmit = m_mapWeeklyReward.begin() ; cmit != m_mapWeeklyReward.end() ; ++cmit )
        {
            stm << L"Supply Type : " << cmit->first << std::endl;//1.선택 2.기본 3.랜덤 보상

            int nIndex = 0;
            for( cvit = cmit->second.begin() ; cvit != cmit->second.end() ; ++cvit )
            {
                //[순번] : 아이템ID, 아이템Type (수량 혹은 기간)
                stm << L"[" << nIndex << L"] : "
                    << cvit->m_ItemID << L", "
                    << cvit->m_nCount << L", "
                    << cvit->m_nPeriod
                    << std::endl;

                ++nIndex;
            }
        }
    }

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

bool KGSSimLayer::GetMonthlySelectRewardList( OUT std::vector<KItem>& vecMonthlySelectRewardList_ )
{
    return GetAttendanceRewardList( m_mapMonthlyReward, KAttendanceReward::SELECT_SUPPLY, vecMonthlySelectRewardList_ );
}

bool KGSSimLayer::GetMonthlySelectRewardItem( IN GCITEMID itemID_, OUT KItem& kReqItem_ )
{
    return GetAttendanceSelectRewardItem( m_mapMonthlyReward, itemID_, kReqItem_ );
}

bool KGSSimLayer::GetMonthlyBaseRewardItem( OUT std::vector<KItem>& vecMonthlyBaseReward_ )
{
    return GetAttendanceRewardList( m_mapMonthlyReward, KAttendanceReward::BASE_SUPPLY, vecMonthlyBaseReward_ );
}

bool KGSSimLayer::GetMonthlyRandomRewardItem( OUT KItem& kReqItem_ )
{
    return GetAttdanceRandomRewardItem( m_mapMonthlyReward, kReqItem_ );
}

void KGSSimLayer::DumpMonthlyReward()
{
    std::map< int, std::vector<KItem> >::iterator cmit;
    std::vector<KItem>::const_iterator cvit;
    std::wstringstream stm;

    stm << L" -- Dump Monthly Reward Item List -- " << std::endl;

    {
        KLocker lock( m_csAttendanceReward );

        for( cmit = m_mapMonthlyReward.begin() ; cmit != m_mapMonthlyReward.end() ; ++cmit )
        {
            stm << L"Supply Type : " << cmit->first << std::endl;//1.선택 2.기본 3.랜덤

            int nIndex = 0;
            for( cvit = cmit->second.begin() ; cvit != cmit->second.end() ; ++cvit )
            {
                //[순번] : 아이템ID, 아이템Type (수량 혹은 기간)
                stm << L"[" << nIndex << L"] : "
                    << cvit->m_ItemID << L", "
                    << cvit->m_nCount << L", "
                    << cvit->m_nPeriod << std::endl;

                ++nIndex;
            }
        }
    }

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

void KGSSimLayer::ReloadAttendanceRewardToDB()
{
    SiKGSDBLayer()->QueueingID( KUserEvent::DB_EVENT_UPDATE_ATTENDANCE_REWARD_ITEM, L"", 0 );//주/월개근 보상 아이템 얻어오기.
    START_LOG( cerr, L"출석 개근시 보상 아이템 리스트 갱신을 DB에 요청" ) << END_LOG;
}

bool KGSSimLayer::IsPossibleBuyPackage( IN bool bPackage_ )
{
    if( m_bPackageBuyVirtualCash )
    {
        return true;
    }

    if( bPackage_ )
    {
        return false;
    }

    return true;
}

bool KGSSimLayer::IsNewUserAfterFriendEvent( IN CTime tmFirstLoginTime_ )
{
    //CTime timeFirstLoginTime( tmFirstLoginTime_ );
    KLocker lock( m_csForceNewUser );
    return tmFirstLoginTime_ >= m_tmNewUserEventTime;
}
void KGSSimLayer::SetNewUserFriendEvent( bool bEnable_, int nYear_, int nMonth_, int nDay_ ) // for Lua
{
    KLocker lock( m_csForceNewUser );
    //m_bEnableForce = bEnable_;
    m_tmNewUserEventTime = CTime( nYear_, nMonth_, nDay_, 0, 0, 0 );
}

void KGSSimLayer::GetUserUIDs( OUT std::vector<DWORD>& vecUIDs_ )
{
    struct KIsConnect
    {
        std::vector<DWORD> vecUIDs;
        void operator() ( const KActorPtr spActor )
        {
            if( !spActor ) return;
            KUserPtr spUser = std::static_pointer_cast<KUser>( spActor );
            if( spUser->GetUID() == 0 ) return;
            vecUIDs.push_back( spUser->GetUID() );
        }
    };

    vecUIDs_.swap( m_kActorManager.ForEach( KIsConnect() ).vecUIDs );
}

void KGSSimLayer::Enable_LFH()
{
    ULONG  HeapFragValue = 2;

    DWORD NumberOfHeaps = GetProcessHeaps( 0, NULL );

    if ( NumberOfHeaps == 0 ) {
        START_LOG( cerr, L"GetProcessHeaps 실패" ) << END_LOG;
    }

    HANDLE Heaps[HEAP_SIZE];

    if ( NumberOfHeaps > HEAP_SIZE ) { 
        START_LOG( cerr, L"힙의 개수가 힙 핸들 배열 크기 초과") << END_LOG;
        return;
    }

    NumberOfHeaps = GetProcessHeaps( NumberOfHeaps, Heaps );

    for ( DWORD HeapsIndex = 0 ; HeapsIndex < NumberOfHeaps ; ++HeapsIndex ) {

        if ( HeapSetInformation( Heaps[HeapsIndex], 
            HeapCompatibilityInformation, 
            &HeapFragValue, 
            sizeof(HeapFragValue) ) ) {

            START_LOG( clog, L"[LFH]가 활성화 되었습니다." << Heaps[HeapsIndex] ) << END_LOG;

        }
        else {
            START_LOG( clog, L"[LFH] 활성화 실패. index : " << HeapsIndex ) << END_LOG;
        }
    }
}

void KGSSimLayer::SetPingConfig( KPingConfig& kPingConfig_ )
{
    KLocker lock( m_csPingConfig );
    m_kPingConfig = kPingConfig_;
}

DWORD KGSSimLayer::GetPingLimit()
{
    KLocker lock( m_csPingConfig );
    return m_kPingConfig.m_dwPingLimitValue;
}

DWORD KGSSimLayer::GetPingGap()
{
    KLocker lock( m_csPingConfig );
    return m_kPingConfig.m_dwPingCheckGap;
}

bool KGSSimLayer::IsPingEnable()
{
    KLocker lock( m_csPingConfig );
    return m_kPingConfig.m_bEnable;
}

DWORD KGSSimLayer::GetPingLimitGap()
{
    KLocker lock( m_csPingConfig );
    return m_kPingConfig.m_dwPingLimitGap;
}

bool KGSSimLayer::CheckSamePingGroup( DWORD dwUserPing_, DWORD dwRoomPing_ )
{
    if ( m_bCheckJoinRoomPing == false ) {
        return true;
    }
    DWORD dwUserPingGroup = MakePingGroup( dwUserPing_ );
    DWORD dwRoomPingGroup = MakePingGroup( dwRoomPing_ );

    if ( dwUserPingGroup == dwRoomPingGroup ) {
        return true;
    }
    return false;
}

DWORD KGSSimLayer::MakePingGroup( DWORD dwPing_ )
{
    // this algorithm is LEE Dong Seok's Method. 
    // Dongs algorithm, 2010-10-28

    //     m_vecPingRange.push_back( 200 );
    //     m_vecPingRange.push_back( 350 );
    //     m_vecPingRange.push_back( 500 );

    DWORD dwResultPing = 0xFFFFFFFF;

    for ( size_t i = 0; i < m_vecPingRange.size(); ++i ) {

        if ( m_vecPingRange[i] > dwPing_ ) {
            dwResultPing = i;
            break;
        }
        else {
            continue;
        }
    }

    return dwResultPing;
}

void KGSSimLayer::SetCreateUserEnable( IN const bool& bEnable_ )
{
    // 서버에 유저객체 생성할수 있는지 여부 설정.
    m_kActorManager.SetCreateActorEnable( bEnable_ );
    START_LOG( cerr, L"Set CreateUser Enable : " << ( bEnable_ ? L"True" : L"False" ) ) << END_LOG;
}

bool KGSSimLayer::CloseAllUser()
{
    START_LOG( cerr, L"명령어로 모든 유저 종료. 현재 유저 : " << m_kActorManager.GetCount() << L" 명" ) << END_LOG;

    SetCreateUserEnable( false );
    m_kActorManager.ForEach( boost::bind( &KActor::ReserveDestroy, _1, 27 ) ); // DRW_SERVER_COMMEND = 27

    DWORD dwBeginTick = ::GetTickCount();

    // 서버 종료중엔 안됨.
    KDBLayer* pkDBLayer = SiKGameServer()->GetDBLayer();
    if ( !pkDBLayer ) {
        return false;
    }

    // 모든 유저 객체가 소멸될때까지 일정시간 대기.
    while ( m_kActorManager.GetCount() != 0 && ::GetTickCount() - dwBeginTick < m_dwShutdownTimeLimit ) {
        ::Sleep( 500 );
    }

    _LOG_SUCCESS( m_kActorManager.GetCount() == 0, L"종료되지 못한 유저 : " << m_kActorManager.GetCount() << L" 명" ) << END_LOG;
    return true;

}

std::wstring KGSSimLayer::MakeSHA1( IN std::wstring& strInput_ )
{
    std::string strSourceSHA = KncUtil::toNarrowString( strInput_ );

    char strSHACheckSum[MAX_FILE_SHA1_BUFFER] = {0,};
    CSHA1 cSHA1;
    cSHA1.Update( (unsigned char*)strSourceSHA.c_str(), strSourceSHA.length() );
    cSHA1.Final();
    cSHA1.ReportHash(strSHACheckSum, CSHA1::REPORT_HEX, true);

    return KncUtil::toWideString(strSHACheckSum);
}

void KGSSimLayer::InitNMCafe()
{
    if ( false == IsNMCafeEnable() ) return;
    if ( false == CheckAuthTypeFlag( KSimLayer::AT_NETMARBLE ) ) return;

    KTThreadManager<NetCafeManager>::GetInstance()->QueueingID( KUserEvent::EVENT_NMNETCAFE_STARTUP_NOT, L"", 0 );
    START_LOG( clog, L"넷마블 pc방 매니저 초기화 요청" ) << END_LOG;
}

void KGSSimLayer::SetCheckTickCount( IN const bool bCheck_ )
{
    KLocker lock( m_csCheckAuthTickCount );
    m_bCheckAuthTickCount = bCheck_;
}

bool KGSSimLayer::IsCheckTickCount()
{
    KLocker lock( m_csCheckAuthTickCount );
    return m_bCheckAuthTickCount;
}

void KGSSimLayer::InitCYOUBilling( std::wstring& strServerName_, DWORD dwUID_ )
{
}

void KGSSimLayer::DumpInventory( IN const DWORD dwLoginUID_ )
{
    KUserPtr spUser( m_kActorManager.GetByUID< KUser >( dwLoginUID_ ) );
    if ( NULL == spUser ) {
        START_LOG( cerr, L"해당 유저는 접속중이 아닙니다. dwLoginUID_ : " << dwLoginUID_ ) << END_LOG;
        return;
    }

    spUser->DumpInventory();

    return;
}
