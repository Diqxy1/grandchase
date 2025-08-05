#include "User.h"
#include <sstream>
#include <iomanip>
#include "GameServer.h"
#include "GSSimLayer.h"
#include "GSNetLayer.h"
#include "NetError.h"
#include "KncUtil.h"
#include "ChannelManager.h"
#include "GSDBThread.h"
#include "GSDBLayer.h"
#include "CnConnector.h"
#include "MissionManager.h"
#include "Message.h"
#include "slangfilter.h"
#include "ResultManager.h"
#include "TimeEventManager.h"
#include "GameDrop.h"
#include "DungeonEntrance.h"
#include "RequierInfo.h"
#include "GCHelper.h"
#include "StatisticsManager.h"
#include "SquareManager.h"
#include "EnchantStrong.h"
#include "SignBoard.h"
#include "RankManager.h"
#include "ItemManager.h"
#include "CollectionManager.h"
#include "MiniGameRankManager.h"
#include "SkillManager.h"
#include "GachaManager.h"
#include "CoupleSystem.h"
#include "DonationManager.h"
#include "LunaNewYear.h"
#include "CharacterPollManager.h"
#include "GPCapsuleManager.h"
#include "SHCheckPoint.h"
#include "GameCouponManager.h"
#include "RecomHelper.h"
#include "PreHackingCheckManager.h"
#include "SubscriptionManager.h"
#include <boost/format.hpp>
#include <KncUtilLua.h>
#include <luabind/operator.hpp>
#include "MissionPack.h"
#include "Rainbow.h"
#include "CooperationEvent.h"
#include "ItemTradeManager.h"
#include "MissionEvent.h"
#include "FTPFactory.h"
#include "NpcGift.h"
#include "DicePlay.h"
#include "QuizBingo.h"
#include "IPAdvantage.h"
#include "GWCSupport.h"
#include "NewDonationManager.h"
#include "SpecialReward.h"
#include "TimeDrop.h"
#include "PresentBox.h"
#include "ServerBenefit.h"
#include "CharDefault.h"
#include "TickManager.h"
#include "EventMonsterManager.h"
#include "GambleBuyManager.h"
#include "HeroDungeonManager.h"
#include "ChannelTypeEvent.h"
#include "CharPromotionSystem.h"
#include "Gawibawibo.h"
#include "Socks.h"
#include "ChangeWeaponManager.h"
#include "PlantTree.h"
#include "GuildManager.h"
#include "ChoiceDrop.h"
#include "VipEventManager.h"
#include "MagicBox.h"
#include "RKTornado.h"
#include "GCPoint.h"
#include "Sphinx.h"
#include "DotaManager.h"
#include "StrengthManager.h"
#include "AgentConnector.h"
#include "AgitHelper.h"
#include "ShutdownUser.h"
#include "Survey.h"
#include "MultiLanguageString.h"
#include "GWCEvent.h"
#include "Escort.h"
#include "JustInTime.h"
#include "CharismasPresent.h"
#include "PostConfig.h"
#include "EventDivideDispatcher.h"
#include "EquipLvDown.h"
#include "MonsterCardMix.h"
#include "EclipseTimeEvent.h"
#include "EclipsePlot.h"
#include "EclipseCollect.h"
#include "DepotManager.h"
#include "JobMissionGuide.h"
#include "SpecialMonsterManager.h"
#include "Adventure.h"
#include "Songkran.h"
#include "LevelResetEvent.h"
#include "SealedChar.h"
#include "PlayAuthManager.h"
#include "OlympicManager.h"
#include "ItemCompose.h"
#include "DungeonManager.h"
#include "GetLimit.h"
#include "GachaLottery.h"
#include "ResultReward.h"
#include "MatchConnector.h"
#include "PVPMatchManager.h"
#include "RitasChristmas.h"
#include "BuffDonation.h"
#include "AccumulateAttendance.h"
#include "ErrandManager.h"
#include "AvoidOverlappingLottery.h"
#include "SubjectManager.h"
#include "CharacterGraduation.h"
#include "CharSlotManager.h"
#include "VitalityManager.h"
#include "ConnectionGiftBox.h"
#include "JumpingChar.h"
#include "SystemResourceManager.h"
#include "TongDonation.h"
#include "SocialCommerce.h"
#include "InfinityDungeon.h"
#include "MissionGettableCondition.h"
#include "DungeonRankManager.h"
#include "GachaPong.h"
#include "PetGlyphMng.h"
#include "UserBillboard.h"
#include "StageReward.h"
#include "BlockedAttackIP.h"

#define WM_PATCH ( WM_USER + 1 )
#define WM_PATCH_FAIL ( WM_USER + 3 )

NiImplementRTTI( KGameServer, KBaseServer );
ImplementSingleton( KGameServer );

ImplToStringW( KGameServer )
{
    size_t uiMsgServerList;
    {
        KLocker lock( m_csMsgServerList );
        uiMsgServerList = m_vecMsgServerList.size();
    }

    {
        KLocker lock( m_csAbuseInfo );
        START_TOSTRING_PARENTW( KBaseServer )
            << TOSTRINGW( m_kEventGameMode.m_vecGameModeCategory.size() )
            << TOSTRINGW( m_strAppName )
            << TOSTRINGW( uiMsgServerList )
            << TOSTRINGWb( m_bDiscountEvent )
            << TOSTRINGW( m_nAbuseCount )
            << TOSTRINGW( m_nAbusePlayerCount )
            << TOSTRINGW( m_nAbuseTime );
    }

    if( m_bDiscountEvent )
    {
        KLocker lock( m_csDiscountEventItem );
        stm_ << L"-- Discount Event Item --" << std::endl;
        for( std::vector< std::pair<int, short> >::const_iterator iter = m_vecDiscountEventItem.begin(); iter != m_vecDiscountEventItem.end(); iter++ )
        {
            stm_ << L"    " << (iter->first) << L"  , discount rate = " << (iter->second) << std::endl;
        }
    }

    DumpChecksum( stm_ );
    DumpPubEvent( stm_ );
    return stm_;
}

KGameServer::KGameServer(void)
:m_hEventLog(NULL)
,m_bDiscountEvent(false)
,m_nPubEventGP( 0 )
,m_bPubEvent( false )
,m_nAbuseCount( 30 )
,m_nAbusePlayerCount( 1 )
,m_nAbuseTime( 60 )
,m_dwMaxUserNum(0)
,m_dwUID(0)
,m_bServerCrashBefore(false)
,m_bConnectCYOU(true)
{
    m_vecDiscountEventItem.clear();
    m_timeApplyGuideLine = CTime( 2020, 1, 1, 0, 0, 0 );
    m_vecPubEventItem.clear();
    m_mapWeaponInfo.clear();
    m_mapChannellingUserNum.clear();
    m_vecInvalidChecksum.clear();
    m_mapUserNickLoginIDInfo.clear();
}

KGameServer::~KGameServer(void)
{
}

ImplOstmOperatorW2A( KGameServer );

void KGameServer::RegToLua()
{
    KBaseServer::RegToLua();

    luabind::module( m_sptlua.get())
    [
        luabind::def( "GetGameServer", &KGameServer::GetInstance ),
        luabind::class_<KGameServer, KBaseServer>( "KGameServer" )
            .def( luabind::tostring(luabind::self) )
            .def( "AddChecksum", &KGameServer::AddChecksum )
            .def( "DeleteChecksum", &KGameServer::DeleteChecksum )
            .def( "ClearChecksum", &KGameServer::ClearChecksum )
            .def( "AddInvalidChecksum", &KGameServer::AddInvalidChecksumForLua )
            .def( "DeleteInvalidChecksum", &KGameServer::DeleteInvalidChecksum )
            .def( "ClearInvalidChecksum", &KGameServer::ClearInvalidChecksum )
            .def( "AddKGameModeInfo", &KGameServer::AddKGameModeInfo )
            .def( "AddGameModeCategory", &KGameServer::AddGameModeCategory )
            .def( "SetDiscountEvent", &KGameServer::SetDiscountEvent )
            .def( "AddDiscountEventItem", &KGameServer::AddDiscountEventItem )
            .def( "DeleteDiscountEventItem", &KGameServer::DeleteDiscountEventItem )
            .def( "ClearDiscountEventItem", &KGameServer::ClearDiscountEventItem )
            .def( "GetDiscountItem", &KGameServer::GetDiscountItem )
            .def( "ResetPubEvent", &KGameServer::ResetPubEvent )
            .def( "SetPubEvent", &KGameServer::SetPubEvent )
            .def( "SetPubEventComment", &KGameServer::SetPubEventComment )
            .def( "AddPubEventItem", &KGameServer::AddPubEventItem )
            .def( "SetPubEventGP", &KGameServer::SetPubEventGP )
            .def( "ApplyGuideLineAfter", &KGameServer::ApplyGuideLineAfter )
            .def( "AddWeaponInfo", &KGameServer::AddWeaponInfo )
            .def( "ClearWeaponInfo", &KGameServer::ClearWeaponInfo )
            .def( "SetAbuseInfo", &KGameServer::SetAbuseInfo )
            .def( "UpdateDBPackage", &KGameServer::SendGetPackageListNotToDB ),

        luabind::class_<KGameModeCategory>( "KGameModeCategory" )
            .def( luabind::constructor<>() ),

        luabind::def( "GetCenterProxy", &KCnConnector::GetInstance ),
        luabind::class_<KCnConnector>( "KCenterProxy" )
            .def( luabind::tostring(luabind::self) )
            .def( "SetRemoteAddress", &KCnConnector::SetRemoteAddress ),

        luabind::def( "GetResultManager", &KResultManager::GetInstance ),
        luabind::class_<KResultManager>( "KResultManager" )
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KResultManager::LoadScript ),

        luabind::def( "GetGameDrop", &KGameDrop::GetInstance ),
        luabind::class_<KGameDrop>( "KGameDrop" )
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KGameDrop::LoadScript )
            .def( "SetBonusRatio", &KGameDrop::SetBonusRatio ),

        luabind::def( "GetTimeEvent", &KTimeEventManager::GetInstance ),
        luabind::class_<KTimeEventManager>( "KTimeEventManager" )
            .def( luabind::tostring(luabind::self) ),
        
        luabind::def( "GetDungeonEntrance", &KDungeonEntrance::GetInstance ),
        luabind::class_<KDungeonEntrance>( "KDungeonEntrance" )
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KDungeonEntrance::LoadScript ),

        luabind::def( "GetRequireInfo", &KRequierInfo::GetInstance ),
        luabind::class_<KRequierInfo>( "KRequierInfo" )
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KRequierInfo::LoadScript ),

        luabind::def( "GetSlangFilter", KSlangFilter::GetInstance ),
        luabind::class_<KSlangFilter>( "KSlangFilter" )
            .def( "AddLanguage",    &KSlangFilter::AddLanguage )
            .def( "SetEnableUnicode", &KSlangFilter::SetEnableUnicode )
            .def( "LoadFilter", &KSlangFilter::LoadFilter ),

        luabind::def( "GetGCHelper", KGCHelper::GetInstance ),
        luabind::class_<KGCHelper>( "KGCHelper" )
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KGCHelper::LoadScript )
            .def( "ClearConnectionMsg", &KGCHelper::ClearConnectionMsg )
            .def( "PrintCoinEvent", &KGCHelper::PrintCoinEvent )
            .def( "SetChatRecEnable", &KGCHelper::SetChatRecordEnable )
            .def( "SetChatQueLimit", &KGCHelper::SetChatLogQueLimit )
            .def( "PrintChatRec", &KGCHelper::PrintChatRecord )
            .def( "PrintHellItemInfo", &KGCHelper::PrintHellItemInfo )
            .def( "PrintInvenInfo", &KGCHelper::PrintInvenInfo ),

        luabind::def( "GetStatisticsManager", KStatisticsManager::GetInstance ),
        luabind::class_<KStatisticsManager>( "KStatisticsManager" )
            .def( luabind::tostring(luabind::self) )
            .def( "AddStatistics", &KStatisticsManager::AddStatistics )
            .def( "ModifyStatistics", &KStatisticsManager::ModifyStatistics )
            .enum_( "STATISTICS_ID" )
            [
                luabind::value( "SI_CPL",                       KStatisticsManager::SI_CPL ),
                luabind::value( "SI_PLAY_TIME",                 KStatisticsManager::SI_PLAY_TIME ),
                luabind::value( "SI_MAP_COUNT",                 KStatisticsManager::SI_MAP_COUNT ),
                luabind::value( "SI_LOGINOUT_STAT",             KStatisticsManager::SI_LOGINOUT_STAT ), // tslayer
                luabind::value( "SI_STAT_EMOTICON",             KStatisticsManager::SI_STAT_EMOTICON ),
                luabind::value( "SI_STAT_ITEM_GET",             KStatisticsManager::SI_STAT_ITEM_GET ),
                luabind::value( "SI_STAT_GAMELOADING",          KStatisticsManager::SI_STAT_GAMELOADING ),
                luabind::value( "SI_STAT_DEATH",                KStatisticsManager::SI_STAT_DEATH ),
                luabind::value( "SI_STAT_CHECK_IN",             KStatisticsManager::SI_STAT_CHECK_IN ),
                luabind::value( "SI_STAT_P2P_CRITICAL",         KStatisticsManager::SI_STAT_P2P_CRITICAL ),
                luabind::value( "SI_STAT_MINIGAME",             KStatisticsManager::SI_STAT_MINIGAME ),
                luabind::value( "SI_STAT_SPX",                  KStatisticsManager::SI_STAT_SPX ),
                luabind::value( "SI_STAT_DISCONNECT",           KStatisticsManager::SI_STAT_DISCONNECT ),
                luabind::value( "SI_STAT_LEAVE_GAME",           KStatisticsManager::SI_STAT_LEAVE_GAME ),
                luabind::value( "SI_STAT_COUPLE_ROOM",          KStatisticsManager::SI_STAT_COUPLE_ROOM ),
                luabind::value( "SI_STAT_GAME_START_GAP",       KStatisticsManager::SI_STAT_GAME_START_GAP ),
                luabind::value( "SI_STAT_PING_STAT",            KStatisticsManager::SI_STAT_PING_STAT ),
                luabind::value( "SI_STAT_DUNGEON",              KStatisticsManager::SI_STAT_DUNGEON ),
                luabind::value( "SI_STAT_LOADINGINFO",          KStatisticsManager::SI_STAT_LOADINGINFO ),
                luabind::value( "SI_STAT_ROOMJOIN",             KStatisticsManager::SI_STAT_ROOMJOIN ),
                luabind::value( "SI_STAT_GP",                   KStatisticsManager::SI_STAT_GP ),
                luabind::value( "SI_STAT_CHARACTER",            KStatisticsManager::SI_STAT_CHARACTER ),
                luabind::value( "SI_STAT_CHAR_EQUIP",           KStatisticsManager::SI_STAT_CHAR_EQUIP ),
                luabind::value( "SI_STAT_NETWORK",              KStatisticsManager::SI_STAT_NETWORK ),
                luabind::value( "SI_STAT_FATAL_DEATH",          KStatisticsManager::SI_STAT_FATAL_DEATH ),
                luabind::value( "SI_STAT_PVP_PLAY_CHARACTER",   KStatisticsManager::SI_STAT_PVP_PLAY_CHARACTER )
            ],

        luabind::def( "GetSquareManager", KSquareManager::GetInstance ),
        luabind::class_<KSquareManager>( "KSquareManager" )
            .def( luabind::tostring(luabind::self) )
            .def( "AddSquare", &KSquareManager::AddSquare )
            .def( "Send_NotMsg", &KSquareManager::Send_NotMsgA ),

        luabind::def( "GetSignBoard", KSignBoard::GetInstance ),
        luabind::class_<KSignBoard>( "KSignBoard" )
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KSignBoard::LoadScript )
            .def( "LuaMsg", &KSignBoard::LuaMsg )
            .def( "ClearLuaMsg", &KSignBoard::ClearLuaMsg ),

        luabind::def( "GetRankManager", KRankManager::GetInstance ),
        luabind::class_<KRankManager>( "KRankManager" )
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KRankManager::LoadScript )
            .def( "SetUpdateTime", &KRankManager::SetUpdateTime )
            .def( "SetRankSearchTime", &KRankManager::SetRankSearchTime),

        luabind::def( "GetItemManager", KItemManager::GetInstance ),
        luabind::class_<KItemManager>( "KItemManager" )
            .def( luabind::tostring(luabind::self) )
            .def( "SetUpdateGap", &KItemManager::SetUpdateGap )
            .def( "LoadScript", &KItemManager::LoadScript )
            .def( "DumpBreakupList", &KItemManager::DumpBreakupList )
            .def( "DumpBreakupInfo", &KItemManager::DumpBreakupInfo )
            .def( "DumpSetItemInfo", &KItemManager::DumpSetItemInfo )
            .def( "DumpBuyLimitItem", &KItemManager::DumpBuyLimitItem ),

        luabind::def( "GetSkillManager", KSkillManager::GetInstance ),
        luabind::class_<KSkillManager>( "KSkillManager" )
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KSkillManager::LoadScript )
            .def( "PrintSkill", &KSkillManager::PrintSkill ),

        luabind::def( "GetGachaManager", &KGachaManager::GetInstance ),
        luabind::class_<KGachaManager>("KGachaManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KGachaManager::LoadScript )
            .def( "Print", &KGachaManager::Print )
            .def( "DumpGacha", &KGachaManager::DumpGacha )
            .def( "DumpNeedKey", &KGachaManager::DumpNeedKey ),

        luabind::def( "GetCoupleSystem", &KCoupleSystem::GetInstance ),
        luabind::class_<KCoupleSystem>("KCoupleSystem")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCoupleSystem::LoadScript ),

        luabind::def( "GetDonationManager", &KDonationManager::GetInstance ),
        luabind::class_<KDonationManager>("KDonationManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KDonationManager::LoadScript )
            .def( "PrintRank",  &KDonationManager::PrintRank )
            .def( "Clear",  &KDonationManager::Clear ),
            
        luabind::def( "GetLunaNewYear", &KLunaNewYear::GetInstance ),
        luabind::class_<KLunaNewYear>("KLunaNewYear")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KLunaNewYear::LoadScript ),

        luabind::def( "GetPollManager", &KCharacterPollManager::GetInstance ),
        luabind::class_<KCharacterPollManager>("KCharacterPollManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCharacterPollManager::LoadScript )
            .def( "TestPoll", &KCharacterPollManager::TestPoll ),

        luabind::def( "GetCapsuleManager", &KGPCapsuleManager::GetInstance ),
        luabind::class_<KGPCapsuleManager>("KGPCapsuleManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KGPCapsuleManager::LoadScript )
            .def( "DumpCapsule", &KGPCapsuleManager::DumpCapsule )
            .def( "PrintCapsuleInfo", &KGPCapsuleManager::PrintCapsuleInfo )
            .def( "PrintCapsuleItem", &KGPCapsuleManager::PrintCapsuleItem ),

        luabind::def( "GetSHCheckPoint", &KSHCheckPoint::GetInstance ),
        luabind::class_<KSHCheckPoint>("KSHCheckPoint")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KSHCheckPoint::LoadScript ),

        luabind::def( "GetCouponManager", &KGameCouponManager::GetInstance ),
        luabind::class_<KGameCouponManager>("KGameCouponManager")
            .def( luabind::tostring(luabind::self) )
            .def( "DumpCoupon", &KGameCouponManager::DumpGameCoupon )
            .def( "PrintCoupon", &KGameCouponManager::PrintGameCouponInfo ),

        luabind::def( "GetRecomHelper", &KRecomHelper::GetInstance ),
        luabind::class_<KRecomHelper>("KRecomHelper")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KRecomHelper::LoadScript ),

        luabind::def( "GetMissionPack", &KMissionPack::GetInstance ),
        luabind::class_<KMissionPack>("KMissionPack")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KMissionPack::LoadScript ),

        luabind::def( "GetRainbow", &KRainbow::GetInstance ),
        luabind::class_<KRainbow>("KRainbow")
            .def( luabind::tostring(luabind::self) )
            .def( "AddItem", &KRainbow::AddItem ),

        luabind::def( "GetIPAdvantage", &KIPAdvantage::GetInstance ),
        luabind::class_<KIPAdvantage>("KIPAdvantage")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KIPAdvantage::LoadScript ),

        luabind::def( "GetCoopEvent", &KCooperationEvent::GetInstance ),
        luabind::class_<KCooperationEvent>("KCooperationEvent")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCooperationEvent::LoadScript )
            .def( "PrintItemList", &KCooperationEvent::PrintItemList ),

        luabind::def( "GetItemTradeManager", &KItemTradeManager::GetInstance ),
        luabind::class_<KItemTradeManager>("KItemTradeManager")
        .def( luabind::tostring(luabind::self) )
        .def( "LoadScript", &KItemTradeManager::LoadScript ),

        luabind::def( "GetMissionEvent", &KMissionEvent::GetInstance ),
        luabind::class_<KMissionEvent>("KMissionEvent")
            .def( luabind::tostring(luabind::self) ),

        luabind::def( "GetSubscriptionManager", &KSubscriptionManager::GetInstance ),
        luabind::class_<KSubscriptionManager>("KSubscriptionManager")
            .def( luabind::tostring(luabind::self) )
            .def( "DumpList", &KSubscriptionManager::DumpList ),

        luabind::def( "GetNpcGift", &KNpcGift::GetInstance ),
        luabind::class_<KNpcGift>("KNpcGift")
            .def( luabind::tostring(luabind::self) )
            .def( "DumpList", &KNpcGift::DumpList ),

        luabind::def( "GetDicePlay", &KDicePlay::GetInstance ),
        luabind::class_<KDicePlay>("KDicePlay")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KDicePlay::LoadScript )
            .def( "PrintRank",  &KDicePlay::PrintRank ),

        luabind::def( "GetFTPFactory", &KFTPFactory::GetInstance ),
        luabind::class_<KFTPFactory>("KFTPFactory")
            .def( luabind::tostring(luabind::self) )
            .def( "AddConnectionInfo", &KFTPFactory::AddConnectionInfoA )
            .def( "DelConnectionInfo",  &KFTPFactory::DelConnectionInfoA )
            .def( "ModifyConnectionInfo",  &KFTPFactory::ModifyConnectionInfoA ),

        luabind::def( "GetQuizBingo", &KQuizBingo::GetInstance ),
        luabind::class_<KQuizBingo>("KQuizBingo")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KQuizBingo::LoadScript ),

        luabind::def( "GetGWCSupport", &KGWCSupport::GetInstance ),
        luabind::class_<KGWCSupport>("KGWCSupport")
            .def( luabind::tostring(luabind::self) ),

        luabind::def( "GetNewDonationManager", &KNewDonationManager::GetInstance ),
        luabind::class_<KNewDonationManager>("KNewDonationManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KNewDonationManager::LoadScript ),

        luabind::def( "GetSpecialReward", &KSpecialReward::GetInstance ),
        luabind::class_<KSpecialReward>("KSpecialReward")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KSpecialReward::LoadScript )
            .def( "DumpBoxInfo", &KSpecialReward::DumpBoxInfo )
            .def( "DumpDifficultBoxRate", &KSpecialReward::DumpDifficultBoxRate )
            .def( "DumpBoxItemList", &KSpecialReward::DumpBoxItemList ),

        luabind::def( "GetTimeDrop", &KTimeDrop::GetInstance ),
        luabind::class_<KTimeDrop>("KTimeDrop")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KTimeDrop::LoadScript )
            .def( "DumpInfo", &KTimeDrop::DumpInfo ),

        luabind::def( "GetPresentBox", &KPresentBox::GetInstance ),
        luabind::class_<KPresentBox>("KPresentBox")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KPresentBox::LoadScript ),

        luabind::def( "GetServerBenefit", &KServerBenefit::GetInstance ),
        luabind::class_<KServerBenefit>("KServerBenefit")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KServerBenefit::LoadScript ),

        luabind::def( "GetCharDefault", &KCharDefault::GetInstance ),
        luabind::class_<KCharDefault>("KCharDefault")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCharDefault::LoadScript ),

        luabind::def( "GetEventMonster", &KEventMonsterManager::GetInstance ),
        luabind::class_<KEventMonsterManager>("KEventMonsterManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KEventMonsterManager::LoadScript )
            .def( "DumpEventMonster", &KEventMonsterManager::DumpEventMonster )
            .def( "DumpModeEvent", &KEventMonsterManager::DumpModeEvent )
            .def( "DumpDungeonLevel", &KEventMonsterManager::DumpDungeonLevel )
            .def( "DumpMonsterLevel", &KEventMonsterManager::DumpMonsterLevel ),

        luabind::def( "GetLookEquipment", &KLookEquipment::GetInstance ),
        luabind::class_<KLookEquipment>("KLookEquipment")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KLookEquipment::LoadScript ),

        luabind::def( "GetGambleBuy", &KGambleBuyManager::GetInstance ),
        luabind::class_<KGambleBuyManager>("KGambleBuyManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KGambleBuyManager::LoadScript )
            .def( "TestGamble", &KGambleBuyManager::TestGamble ),

        luabind::def( "GetChannelTypeEvent", &KChannelTypeEvent::GetInstance ),
        luabind::class_<KChannelTypeEvent>("KChannelTypeEvent")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KChannelTypeEvent::LoadScript ),

        luabind::def( "GetCharPromotionSystem", &KCharPromotionSystem::GetInstance ),
        luabind::class_<KCharPromotionSystem>("KCharPromotionSystem")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCharPromotionSystem::LoadScript ),

        luabind::def( "GetHeroDungeon", &KHeroDungeonManager::GetInstance ),
        luabind::class_<KHeroDungeonManager>("KHeroDungeonManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KHeroDungeonManager::LoadScript )
            .def( "DumpItemList", &KHeroDungeonManager::DumpItemList )
            .def( "DumpDungeonInfo", &KHeroDungeonManager::DumpDungeonInfo ),

        luabind::def( "GetGawibawibo", &KGawibawibo::GetInstance ),
        luabind::class_<KGawibawibo>("KGawibawibo")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KGawibawibo::LoadScript )
            .def( "TestPlay", &KGawibawibo::TestPlay ),

        luabind::def( "GetSocks", &KSocks::GetInstance ),
        luabind::class_<KSocks>("KSocks")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KSocks::LoadScript ),

        luabind::def( "GetTickManager", &KTickManager::GetInstance ),
        luabind::class_<KTickManager>("KTickManager")
            .def( luabind::tostring(luabind::self) )
            .def( "SetTickGap", &KTickManager::SetTickGap )
            .def( "SetMinTickGap", &KTickManager::SetMinTickGap ),

        luabind::def( "GetChangeWeapon", &KChangeWeaponManager::GetInstance ),
        luabind::class_<KChangeWeaponManager>("KChangeWeaponManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KChangeWeaponManager::LoadScript )
            .def( "DumpInfo", &KChangeWeaponManager::DumpInfo ),

        luabind::def( "GetPlantTree", &KPlantTree::GetInstance ),
        luabind::class_<KPlantTree>("KPlantTree")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KPlantTree::LoadScript ),

        luabind::def( "GetChoiceDrop", &KChoiceDrop::GetInstance ),
        luabind::class_<KChoiceDrop>("KChoiceDrop")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KChoiceDrop::LoadScript )
            .def( "DumpInfo", &KChoiceDrop::DumpInfo ),

        luabind::def( "GetVipEventManager", &KVipEventManager::GetInstance ),
        luabind::class_<KVipEventManager>("KVipEventManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KVipEventManager::LoadScript )
            .def( "UpdateInfo", &KVipEventManager::SendVipEventInfoReqToDB )
            .def( "DumpInfo", &KVipEventManager::DumpInfo ),

        luabind::def( "GetMagicBox", &KMagicBox::GetInstance ),
        luabind::class_<KMagicBox>("KMagicBox")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KMagicBox::LoadScript )
            .def( "UpdateEvent", &KMagicBox::SendMagicBoxListNotToDB )
            .def( "DumpInfo", &KMagicBox::DumpInfo ),

        luabind::def( "GetRKTornado", &KRKTornado::GetInstance ),
        luabind::class_<KRKTornado>("KRKTornado")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KRKTornado::LoadScript ),

        luabind::def( "GetGCPoint", &KGCPoint::GetInstance ),
        luabind::class_<KGCPoint>("KGCPoint")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KGCPoint::LoadScript )
            .def( "DumpInfo", &KGCPoint::DumpInfo ),

        luabind::def( "GetSphinx", &KSphinx::GetInstance ),
        luabind::class_<KSphinx>("KSphinx")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KSphinx::LoadScript ),

        luabind::def( "GetDotaManager", &KDotaManager::GetInstance ),
        luabind::class_<KDotaManager>("KDotaManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KDotaManager::LoadScript ),

        luabind::def( "GetAgentProxy", &KAgentConnector::GetInstance ),
        luabind::class_<KAgentConnector>( "KAgentProxy" )
            .def( luabind::tostring(luabind::self) )
            .def( "SetRemoteAddress", &KAgentConnector::SetRemoteAddress )
            .def( "SetAgentMasterSID", &KAgentConnector::SetAgentMasterSID ),

        luabind::def( "GetAgitHelper", &KAgitHelper::GetInstance ),
        luabind::class_<KAgitHelper>("KAgitHelper")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KAgitHelper::LoadScript )
            .def( "DumpInfo", &KAgitHelper::DumpInfo ),

        luabind::def( "GetStrengthManager", &KStrengthManager::GetInstance ),
        luabind::class_<KStrengthManager>("KStrengthManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KStrengthManager::LoadScript ),

        luabind::def( "GetShutdownUser", &KShutdownUser::GetInstance ),
        luabind::class_<KShutdownUser>("KShutdownUser")
            .def( luabind::tostring(luabind::self) ),

        luabind::def( "GetSurvey", &KSurvey::GetInstance ),
        luabind::class_<KSurvey>("KSurvey")
            .def( luabind::tostring(luabind::self) ),

        luabind::def( "GetGWCEvent", &KGWCEvent::GetInstance ),
        luabind::class_<KGWCEvent>("KGWCEvent")
            .def( luabind::tostring(luabind::self) ),

        luabind::def( "GetEscort", &KEscort::GetInstance ),
        luabind::class_<KEscort>("KEscort")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KEscort::LoadScript )
			.def( "DebugPrint", &KEscort::DebugPrint ),
			
        luabind::def( "GetJustInTime", &KJustInTime::GetInstance ),
        luabind::class_< KJustInTime >( "KJustInTime" )
            .def( luabind::tostring( luabind::self ) )
            .def( "LoadScript", &KJustInTime::LoadScript ),

        luabind::def( "GetCharismasPresent", &KCharismasPresent::GetInstance ),
        luabind::class_<KCharismasPresent>("KCharismasPresent")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCharismasPresent::LoadScript ),

        luabind::def( "GetPostConfig", &KPostConfig::GetInstance ),
        luabind::class_<KPostConfig>("KPostConfig")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KPostConfig::LoadScript ),

        luabind::def( "GetEventDivideDispatcher", &KEventDivideDispatcher::GetInstance ),
        luabind::class_<KEventDivideDispatcher>("KEventDivideDispatcher")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KEventDivideDispatcher::LoadScript )
            .def( "PrintStatus", &KEventDivideDispatcher::PrintStatus ),

        luabind::def( "GetEclipseTimeEvent", &KEclipseTimeEvent::GetInstance ),
        luabind::class_<KEclipseTimeEvent>("KEclipseTimeEvent")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KEclipseTimeEvent::LoadScript ),

        luabind::def( "GetEclipsePlot", &KEclipsePlot::GetInstance ),
        luabind::class_<KEclipsePlot>("KEclipsePlot")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KEclipsePlot::LoadScript ),

        luabind::def( "GetEclipseCollect", &KEclipseCollect::GetInstance ),
        luabind::class_<KEclipseCollect>("KEclipseCollect")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KEclipseCollect::LoadScript ),

        luabind::def( "GetMonsterCardMix", &KMonsterCardMix::GetInstance ),
        luabind::class_<KMonsterCardMix>("KMonsterCardMix")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KMonsterCardMix::LoadScript ),

        luabind::def( "GetDepotManager", &KDepotManager::GetInstance ),
        luabind::class_<KDepotManager>("KDepotManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KDepotManager::LoadScript ),

        luabind::def( "GetJobMissionGuide", &KJobMissionGuide::GetInstance ),
        luabind::class_<KJobMissionGuide>("KJobMissionGuide")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KJobMissionGuide::LoadScript ),

        luabind::def( "GetSpecialMonsterManager", &KSpecialMonsterManager::GetInstance ),
        luabind::class_<KSpecialMonsterManager>("KSpecialMonsterManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KSpecialMonsterManager::LoadScript ),

        luabind::def( "GetSongkran", &KSongkran::GetInstance ),
        luabind::class_<KSongkran>("KSongkran")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KSongkran::LoadScript ),

        luabind::def( "GetAdventure", &KAdventure::GetInstance ),
        luabind::class_<KAdventure>("KAdventure")
            .def( luabind::tostring(luabind::self) ),

        luabind::def( "GetLevelResetEvent", &KLevelResetEvent::GetInstance ),
        luabind::class_<KLevelResetEvent>("KLevelResetEvent")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KLevelResetEvent::LoadScript ),

        luabind::def( "GetSealedChar", &KSealedChar::GetInstance ),
        luabind::class_<KSealedChar>("KSealedChar")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KSealedChar::LoadScript ),

        luabind::def( "GetOlympicManager", &KOlympicManager::GetInstance ),
        luabind::class_<KOlympicManager>("KOlympicManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KOlympicManager::LoadScript ),

        luabind::def( "GetGachaLotteryManager", &KGachaLottery::GetInstance ),
        luabind::class_<KGachaLottery>("KGachaLottery")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KOlympicManager::LoadScript ),

        luabind::def( "GetItemCompose", &KItemCompose::GetInstance ),
        luabind::class_<KItemCompose>("KItemCompose")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KItemCompose::LoadScript ),

        luabind::def( "GetAdventure", &KAdventure::GetInstance ),
        luabind::class_<KAdventure>("KAdventure")
            .def( luabind::tostring(luabind::self) ),

        luabind::def( "GetLevelResetEvent", &KLevelResetEvent::GetInstance ),
        luabind::class_<KLevelResetEvent>("KLevelResetEvent")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KLevelResetEvent::LoadScript ),

        luabind::def( "GetGetLimit", &KGetLimit::GetInstance ),
        luabind::class_<KGetLimit>("KGetLimit")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KGetLimit::LoadScript ),

        luabind::def( "GetDungeonManager", &KDungeonManager::GetInstance ),
        luabind::class_<KDungeonManager>("KDungeonManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KDungeonManager::LoadScript ),

        luabind::def( "GetGachaLotteryManager", &KGachaLottery::GetInstance ),
        luabind::class_<KGachaLottery>("KGachaLottery")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KGachaLottery::LoadScript ),

        luabind::def( "GetResultRewardManager", &KResultReward::GetInstance ),
        luabind::class_<KResultReward>("KResultReward")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KResultReward::LoadScript ),

        luabind::def( "GetMatchProxy", &KMatchConnector::GetInstance ),
        luabind::class_<KMatchConnector>( "KMatchProxy" )
            .def( luabind::tostring(luabind::self) )
            .def( "SetRemoteAddress", &KMatchConnector::SetRemoteAddress ),

        luabind::def( "GetPVPMatchManager", &KPVPMatchManager::GetInstance ),
        luabind::class_<KPVPMatchManager>("KPVPMatchManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KPVPMatchManager::LoadScript ),

        luabind::def( "GetRitasChristmas", &KRitasChristmas::GetInstance ),
        luabind::class_<KRitasChristmas>("KRitasChristmas")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KRitasChristmas::LoadScript )
            .def( "SendRankUpdateTimeChangeNot", &KRitasChristmas::SendRankUpdateTimeChangedNot ),

        luabind::def( "GetEquipLvDown", &KEquipLvDown::GetInstance ),
        luabind::class_<KEquipLvDown>("KEquipLvDown")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KEquipLvDown::LoadScript ),

        luabind::def( "GetBuffDonation", &KBuffDonation::GetInstance ),
        luabind::class_<KBuffDonation>("KBuffDonation")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KBuffDonation::LoadScript ),

        luabind::def( "GetAccumulateAttendance", &KAccumulateAttendanceManager::GetInstance ),
        luabind::class_<KAccumulateAttendanceManager>("KAccumulateAttendanceManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KAccumulateAttendanceManager::LoadScript ),

        luabind::def( "GetErrandManager", &KErrandManager::GetInstance ),
        luabind::class_<KErrandManager>("KErrandManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KErrandManager::LoadScript ),

        luabind::def( "GetCharacterGraduation", &KCharacterGraduation::GetInstance ),
        luabind::class_<KCharacterGraduation>("KCharacterGraduation")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCharacterGraduation::LoadScript ),

        luabind::def( "GetSubjectManager", &KSubjectManager::GetInstance ),
        luabind::class_<KSubjectManager>("KSubjectManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KSubjectManager::LoadScript ),

        luabind::def( "GetSystemGuide", &KSystemGuide::GetInstance ),
        luabind::class_<KSystemGuide>("KSystemGuide")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KSystemGuide::LoadScript ),

        luabind::def( "GetCharSlotManager", &KCharSlotManager::GetInstance ),
        luabind::class_<KCharSlotManager>("KCharSlotManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCharSlotManager::LoadScript ),

        luabind::def( "GetVitalityManager", &KVitalityManager::GetInstance ),
        luabind::class_<KVitalityManager>("KVitalityManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KVitalityManager::LoadScript )
            .def( "UseVitalitySystem", &KVitalityManager::UseVitalitySystem ),

        luabind::def( "GetConnectionGiftBox", &KConnectionGiftBox::GetInstance ),
        luabind::class_<KConnectionGiftBox>("KConnectionGiftBox")
            .def( luabind::tostring(luabind::self) ),

        luabind::def( "GetJumpingChar", &KJumpingChar::GetInstance ),
        luabind::class_<KJumpingChar>("KJumpingChar")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KJumpingChar::LoadScript ),

        luabind::def( "GetSystemResourceManager", &KSystemResourceManager::GetInstance ),
        luabind::class_<KSystemResourceManager>("KSystemResourceManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KSystemResourceManager::LoadScript ),

        luabind::def( "GetTongDonation", &KTongDonation::GetInstance ),
        luabind::class_<KTongDonation>("KTongDonation")
            .def( luabind::tostring(luabind::self) ),

        luabind::def( "GetSocialCommerce", &KSocialCommerce::GetInstance ),
        luabind::class_<KSocialCommerce>("KSocialCommerce")
            .def( luabind::tostring(luabind::self) ),

        luabind::def( "GetHackCheck", &KPreHackingCheckManager::GetInstance ),
        luabind::class_<KPreHackingCheckManager>("KPreHackingCheckManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KPreHackingCheckManager::LoadScript )
            .def( "DumpHackingProtectionMode", &KPreHackingCheckManager::DumpHackingProtectionMode ),

        luabind::def( "GetInfinityDungeon", &KInfinityDungeon::GetInstance ),
        luabind::class_<KInfinityDungeon>("KInfinityDungeon")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KInfinityDungeon::LoadScript ),

        luabind::def( "GetMissionGettableCondition", &KMissionGettableCondition::GetInstance ),
        luabind::class_<KMissionGettableCondition>("KMissionGettableCondition")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KMissionGettableCondition::LoadScript ),

        luabind::def( "GetDungeonRankManager", &KDungeonRankManager::GetInstance ),
        luabind::class_<KDungeonRankManager>("KDungeonRankManager")
            .def( luabind::tostring(luabind::self) ),

        luabind::def( "GetGachaPon", &KGachaPong::GetInstance ),
		luabind::class_<KGachaPong>("KGachaPong")
		    .def( luabind::tostring(luabind::self) )
		    .def( "LoadScript", &KGachaPong::LoadScript )
		    .def( "PrintMachine", &KGachaPong::PrintMachine ),

        luabind::def( "GetPetGlyphManager", &KPetGlyphMng::GetInstance ),
		luabind::class_<KPetGlyphMng>("KPetGlyphMng")
		    .def( luabind::tostring(luabind::self) )
		    .def( "LoadScript", &KPetGlyphMng::LoadScript ),

        luabind::def( "GetBillboardManager", &KUserBillboardManager::GetInstance ),
		luabind::class_<KUserBillboardManager>("KUserBillboardManager")
		    .def( luabind::tostring(luabind::self) )
		    .def( "LoadScript", &KUserBillboardManager::LoadScript ),

        luabind::def( "GetStageRewardManager", &KStageReward::GetInstance ),
        luabind::class_<KStageReward>("KStageReward")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KStageReward::LoadScript ),

        luabind::def( "GetBlockedAttackIP", &KBlockedAttackIP::GetInstance ),
        luabind::class_<KBlockedAttackIP>("KBlockedAttackIP")
            .def( "LoadScript", &KBlockedAttackIP::LoadScript )
    ];
}

bool KGameServer::Init( const wchar_t* szFileName_ )
{
    m_pkDBLayer     = SiKGSDBLayer();
    m_pkNetLayer    = SiKGSNetLayer();
    m_pkSimLayer    = SiKGSSimLayer();


   _JIF( KBaseServer::Init( szFileName_ ), return false );


   _JIF( 0 == lua_dofile( m_sptlua.get(), "LevelLimit.lua" ), return false );       // 060731. kkurrung. 게임 모드 레벨 제한 
   _JIF( 0 == lua_dofile( m_sptlua.get(), "InitChannels.lua" ), return false );     // 061107. kkurrung. 채널 로드
   _JIF( 0 == lua_dofile( m_sptlua.get(), "InitChannelDrop.lua" ), return false );  // 070607. kkurrung. 채널 드랍
   _JIF( LoadScript( std::string() ), return false ); // <---  이 함수 안에서 스크립트 로드 해주세요.. ^^

    SiKPreHackingCheckManager()->DumpHackingProtectionMode();

    //std::cout << std::lbreak;

    KncUtil::GetAppName( m_strAppName );

    // 050808. open the eventlog
    m_hEventLog = ::RegisterEventSource( NULL, m_strAppName.c_str() );

    // db 데이터 초기작업. (체크섬 얻기, 서버 리스트 얻기, 서버 이름 얻기, 동접수 0 초기화)
    // note : db handler와 protocol ver 설정이 마무리 된 후에 호출해야 한다.
    // (dbthread 가동 전이라 하더라도 미리 queueing되었다가 호출된다. but 지금은 thread 가동 된 후임)
    KDB_EVENT_SERVER_INFO_REQ kPacket;
    kPacket.m_nProtocolVer  = m_nProtocolVer;
    kPacket.m_strIP         = NetCommon::GetLocalIPW();
    kPacket.m_usPort        = m_pkNetLayer->m_usPortMaster;
    kPacket.m_mapChannellingUserNum[KUser::USER_CT_TOTAL] = m_pkSimLayer->m_kActorManager.GetCount();
    kPacket.m_bFirstDBWork  = true;
    kPacket.m_hSyncEvent    = ::CreateEvent( NULL, false, false, NULL );

    // 시간 동기화 시켜 준다.
    SiKGameServer()->GetDBLayer()->QueueingID( KUserEvent::DB_EVENT_TIME_SYNCRONIZE_NOT, L"", 1 );

    // 오픈시 초기 액션을 취한다.
    SiKGameServer()->GetDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_SERVER_INFO_REQ, NULL, 0, 0, kPacket );
    SiKGameCouponManager()->GetFirstDataFromDB(); // DB 에 큐잉 한다.

    // 070620. woosh. 비속어 필터 파일 로드
    _JIF( true == SiKSlangFilter()->LoadFilter( "Curse.txt" ), return false );

    switch( ::WaitForSingleObject( kPacket.m_hSyncEvent, 3000000 ) ) {
    case WAIT_OBJECT_0:
        {
            KLocker lock( m_csServerName );

            START_LOG( clog, L""
                << L"** Server Starting Configuration **"
                << L" Server Name : " << m_strServerName 
                << L" Channel Number : " << SiKChannelManager()->GetNumChannel()
                << L" ServerID : " << GetServerID() ) << END_LOG;

            // connect to Center Server.
            ReConnectCenterServer();
            //LIF( SiKCnConnector()->Connect() );

            // connect to Agent Server.
            if ( SiKGSSimLayer()->IsAgitEnable() ) {
                KAgentConnector* pkAgentProxy = SiKAgentConnector();
                LIF( pkAgentProxy->InitConnect() );
            }

            // connect to Room Server.
            KMatchConnector* pkMatchProxy = SiKMatchConnector();
            LIF( pkMatchProxy->InitConnect() );


            if ( m_bServerCrashBefore ) {
                //SiKGSSimLayer()->CYOUSendServerCrash( m_strServerName, GetUID() );
            }

        }
        break;
    case WAIT_TIMEOUT:
        START_LOG( cerr, L"Getting Server Info from DB failed."  ) << END_LOG;
        CloseHandle( kPacket.m_hSyncEvent );
        return false;
        break;
    default:
        START_LOG( cerr, L"Unexpected Return value from ::WaitForSingleObject()." ) << END_LOG;
        CloseHandle( kPacket.m_hSyncEvent );
        return false;
    }

    CloseHandle( kPacket.m_hSyncEvent );

    if ( GetServerName().empty() ) {
        return false;
    }

    // 랭킹 데이터 최초 가져옴..
    SiKGSDBLayer()->QueueingID( KUserEvent::EVENT_DB_UPDATE_RANK_DATA_REQ, L"", 0, 0 );
    // 매칭 랭킹 데이터 최초 가져옴.
    SiKGSDBLayer()->QueueingID( KUserEvent::DB_EVENT_UPDATE_MATCH_RANK_DATA_REQ, L"", 0, 0 );

    SiKDonationManager()->InitProcess();
    SiKDicePlay()->InitProcess();   // 주사위 이벤트 정보 초기화.

    SiKNewDonationManager()->InitProcess();

    SiKGameServer()->GetDBLayer()->QueueingID( KUserEvent::DB_EVENT_SANTA_DATA, L"", 0, 0 );
    SiKGameServer()->GetDBLayer()->QueueingID( KUserEvent::DB_EVENT_VIRTUAL_CASH_RESTRICT_ITEM_LIST, L"", 0, 0 );
    SiKVipEventManager()->SendVipEventInfoReqToDB();

    SendGetPackageListNotToDB();
    SiKGSSimLayer()->InitNMCafe();

    // 길드 페이지 수정작업
    // 길드 정보 받아오기
    SiKGuildManager()->Init();
    SiKOlympicManager()->InitProcess(); // 올림픽 이벤트 초기화 정보 세팅.


    if ( true == SiKPlayAuthManager()->IsUsePlayAuthSystem() ) {
        SiKPlayAuthManager()->Initialize();
    }

    SiKGameServer()->GetDBLayer()->QueueingID( KUserEvent::DB_EVENT_SKILL_MISSION_INFO_NOT, L"", 0, 0 );
    SiKGameServer()->GetDBLayer()->QueueingID( KUserEvent::DB_EVENT_SHANGHAI_MISSION_INFO_NOT, L"", 0, 0 );
    SiKGameServer()->GetDBLayer()->QueueingID( KUserEvent::DB_EVENT_TONG_DONATION_TOTAL_AMOUNT_INFO_REQ, L"", 0 );

    // 이거 깔끔하게 정리 가능하면 좋겠음.....
    // 엘소드는 SimLayer 자식들이 스크립트 로드하고 이것저것 다 하던데.... 흠...
    SiKGSSimLayer()->SetCreateActorEnable( true );
    START_LOG( cerr, L"Create Actor Enable!!" ) << END_LOG;

    return true;
}

void KGameServer::Run( bool bRunInterpreter_ )
{

#ifdef NDEBUG
    wchar_t szUserName[MAX_PATH] = {0};
    DWORD dwSize = MAX_PATH;
    ::GetUserNameW( szUserName, &dwSize );

    PCTSTR aInsertions[] = { m_strAppName.c_str(), szUserName };    // 실행파일명, 로그인 계정이름

    BOOL bSuccess = ::ReportEvent( 
        m_hEventLog,                    //
        EVENTLOG_INFORMATION_TYPE,      // type of event
        CATEGORY_ONE,                   // Category (could also be 0)
        EVENT_STARTED_BY,               // Event id
        NULL,                           // User's sid (NULL for none)
        NUM_ELEMENTS( aInsertions ),    // Number of insertion strings
        0,                              // Number of additional bytes
        aInsertions,                    // Array of insertion strings
        NULL                            // Pointer to additional bytes
        );
#endif

    KBaseServer::Run( bRunInterpreter_ ); // lua interpreter 시작

    //// 종료처리 하기 전, 모든 유저들의 캐싱 데이터를 업데이트 해야 한다.
    //SiKGSSimLayer()->DestroyAllUsers();
    // ^ 이 부분에서 처리하면 Test 프로젝트에서 초기화 이후 바로 실행된다.
}

void KGameServer::ShutDown()
{
    // 051012. 서버를 종료할 때 DB의 동접수치를 0으로 내려준다.
    KDB_EVENT_SERVER_INFO_REQ kPacket;
    kPacket.m_nProtocolVer  = -1;
    kPacket.m_strIP         = NetCommon::GetLocalIPW();
    kPacket.m_usPort        = m_pkNetLayer->m_usPortMaster;
    kPacket.m_mapChannellingUserNum[KUser::USER_CT_TOTAL]   = 0;
    kPacket.m_bFirstDBWork  = false;
    kPacket.m_hSyncEvent    = NULL;

    SiKGameServer()->GetDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_SERVER_INFO_REQ, NULL, 0, 0, kPacket );

    // 종료처리 하기 전, 모든 유저들의 캐싱 데이터를 업데이트 해야 한다.
    SiKGSSimLayer()->DestroyAllUsers();

    KBaseServer::ShutDown();

    SiKPlayAuthManager()->Uninitialize();

    KCoupleSystem::ReleaseInstance();
    KSignBoard::ReleaseInstance();
    KRankManager::ReleaseInstance();
    KSquareManager::ReleaseInstance();
    KStatisticsManager::ReleaseInstance();
    KGCHelper::ReleaseInstance();
    KRequierInfo::ReleaseInstance();
    KSlangFilter::ReleaseInstance();
    KDungeonEntrance::ReleaseInstance();
    KMissionManager::ReleaseInstance();
    KGSSimLayer::ReleaseInstance();
    KGSDBLayer::ReleaseInstance();
    KGSNetLayer::ReleaseInstance();
    KResultManager::ReleaseInstance();
    KTimeEventManager::ReleaseInstance();
    KGameDrop::ReleaseInstance();
    KItemManager::ReleaseInstance();
    KCollectionManager::ReleaseInstance();
    KSkillManager::ReleaseInstance();
    KGachaManager::ReleaseInstance();
    KDonationManager::ReleaseInstance();
    KLunaNewYear::ReleaseInstance();
    KCharacterPollManager::ReleaseInstance();
    KGPCapsuleManager::ReleaseInstance();
    KSHCheckPoint::ReleaseInstance();
    KGameCouponManager::ReleaseInstance();
    KRecomHelper::ReleaseInstance();
    KMissionPack::ReleaseInstance();
    KPreHackingCheckManager::ReleaseInstance();
    KRainbow::ReleaseInstance();
    KCooperationEvent::ReleaseInstance();
    KItemTradeManager::ReleaseInstance();
    KSanta::ReleaseInstance();
    KMissionEvent::ReleaseInstance();
    KSubscriptionManager::ReleaseInstance();
    KNpcGift::ReleaseInstance();
    KDicePlay::ReleaseInstance();
    KFTPFactory::ReleaseInstance();
    KIPAdvantage::ReleaseInstance();
    KQuizBingo::ReleaseInstance();
    KGWCSupport::ReleaseInstance();
    KNewDonationManager::ReleaseInstance();
    KSpecialReward::ReleaseInstance();
    KTimeDrop::ReleaseInstance();
    KPresentBox::ReleaseInstance();
    KServerBenefit::ReleaseInstance();
    KCharDefault::ReleaseInstance();
    KEventMonsterManager::ReleaseInstance();
    KTickManager::ReleaseInstance();
    KLookEquipment::ReleaseInstance();
    KGambleBuyManager::ReleaseInstance();
    KChannelTypeEvent::ReleaseInstance();
    KCharPromotionSystem::ReleaseInstance();
    KHeroDungeonManager::ReleaseInstance();
    KGawibawibo::ReleaseInstance();
    KChangeWeaponManager::ReleaseInstance();
    KPlantTree::ReleaseInstance();
    KChoiceDrop::ReleaseInstance();
    KVipEventManager::ReleaseInstance();
    KMagicBox::ReleaseInstance();
    KRKTornado::ReleaseInstance();
    KGCPoint::ReleaseInstance();
    KSphinx::ReleaseInstance();
    KDotaManager::ReleaseInstance();
    KStrengthManager::ReleaseInstance();
    KAgitHelper::ReleaseInstance();
    KAgentConnector::ReleaseInstance();
    KShutdownUser::ReleaseInstance();
    KSurvey::ReleaseInstance();
    KGWCEvent::ReleaseInstance();
    KCharismasPresent::ReleaseInstance();
    KJustInTime::ReleaseInstance();
    KEventDivideDispatcher::ReleaseInstance();
    KPostConfig::ReleaseInstance();
    KEquipLvDown::ReleaseInstance();
    KEclipseTimeEvent::ReleaseInstance();
    KEclipsePlot::ReleaseInstance();
    KEclipseCollect::ReleaseInstance();
    KMonsterCardMix::ReleaseInstance();
    KDepotManager::ReleaseInstance();
    KJobMissionGuide::ReleaseInstance();
    KMatchConnector::ReleaseInstance();
    KSpecialMonsterManager::ReleaseInstance();
    KSongkran::ReleaseInstance();
    KSealedChar::ReleaseInstance();
    KPlayAuthManager::ReleaseInstance();
    KOlympicManager::ReleaseInstance();
    KItemCompose::ReleaseInstance();
    KDungeonManager::ReleaseInstance();
    KGachaLottery::ReleaseInstance();	
    KResultReward::ReleaseInstance();
    KLevelResetEvent::ReleaseInstance();
    KAdventure::ReleaseInstance();	
    KGetLimit::ReleaseInstance();
	KPVPMatchManager::ReleaseInstance();
    KRitasChristmas::ReleaseInstance();
    KBuffDonation::ReleaseInstance();
    KAccumulateAttendanceManager::ReleaseInstance();
    KErrandManager::ReleaseInstance();
    KAvoidOverlappingLottery::ReleaseInstance();
    KSubjectManager::ReleaseInstance();
    KCharacterGraduation::ReleaseInstance();
    KSystemGuide::ReleaseInstance();
    KCharSlotManager::ReleaseInstance();
    KVitalityManager::ReleaseInstance();
    KConnectionGiftBox::ReleaseInstance();
    KJumpingChar::ReleaseInstance();	
    KSystemResourceManager::ReleaseInstance();
    KTongDonation::ReleaseInstance();
    KSocialCommerce::ReleaseInstance();
    KInfinityDungeon::ReleaseInstance();
    KMissionGettableCondition::ReleaseInstance();
    KDungeonRankManager::ReleaseInstance();
    KGachaPong::ReleaseInstance();
    KPetGlyphMng::ReleaseInstance();
    KUserBillboardManager::ReleaseInstance();
    KStageReward::ReleaseInstance();
    KBlockedAttackIP::ReleaseInstance();

    if ( logger ) {
        ::log4cxx::LogManager::shutdown();
    }

    if( m_hEventLog != NULL)
        ::DeregisterEventSource( m_hEventLog );     // 050808. close eventlog.
}

bool KGameServer::IsRegisteredChecksum( DWORD dwChecksum_ )
{
    KLocker lock( m_csClientChecksum );

    std::vector<DWORD>::iterator vit;
    vit = std::find( m_vecClientChecksum.begin(), m_vecClientChecksum.end(), dwChecksum_ );

    return (vit != m_vecClientChecksum.end() );
}

void KGameServer::AddChecksum( DWORD dwChecksum_ )
{
    KLocker lock( m_csClientChecksum );

    std::vector<DWORD>::iterator vit;
    vit = std::find( m_vecClientChecksum.begin(), m_vecClientChecksum.end(), dwChecksum_ );

    if(vit != m_vecClientChecksum.end() )
    {
        START_LOG( cwarn, L"이미 등록된 checksum 입니다. chksm:" << dwChecksum_ ) << END_LOG;
    }
    else
    {
        m_vecClientChecksum.push_back( dwChecksum_ );
        START_LOG( cerr, L"checksum is added." ) << END_LOG;
    }
}

bool KGameServer::DeleteChecksum( unsigned int nIndex_ )
{
    {
        KLocker lock( m_csClientChecksum );

        if( nIndex_ >= m_vecClientChecksum.size() )
        {
            START_LOG( cwarn, L"올바르지 못한 인덱스. index:" << nIndex_ )
                << BUILD_LOG( m_vecClientChecksum.size() ) << END_LOG;
            return false;
        }

        m_vecClientChecksum.erase( m_vecClientChecksum.begin() + nIndex_ );
    }

    // 콘솔 창으로만 출력한다.
    START_LOG( cerr, L"checksum 삭제됨. index:" << nIndex_ ) << END_LOG;

    return true;
}

void KGameServer::ClearChecksum()
{
    KLocker lock( m_csClientChecksum );

    START_LOG( cerr, L"clear all checksum. size:" << m_vecClientChecksum.size() ) << END_LOG;

    m_vecClientChecksum.clear();
}

bool KGameServer::IsRegisteredInvalidChecksum( DWORD dwInvalidChecksum_ ) // gsdbthreadhelper3 에서 호출
{
    KLocker lock( m_csInvalidChecksum );

    std::vector<KInvalidChecksumInfo>::iterator vit;
    vit = std::find_if( m_vecInvalidChecksum.begin(), m_vecInvalidChecksum.end(), 
        boost::bind( &KInvalidChecksumInfo::m_dwInvChecksum, _1 ) == dwInvalidChecksum_ );

    if( vit != m_vecInvalidChecksum.end() )
    {
        vit->m_nCount++;

        if( vit->m_nCount >= 50 )   // 060811.
        {
            m_vecInvalidChecksum.erase( vit );  // 50회가 넘으면 삭제. 삭제해도 db에 등록되어있지 않으면 허용불가.
            return false;                       // 다시한번 db에 문의하도록 false를 리턴한다.
        }
        return true;
    }//end if

    return false;
}

void KGameServer::AddInvalidChecksumForLua( DWORD dwInvalidChecksum_ )
{
    std::string strTmp("");
    AddInvalidChecksum( dwInvalidChecksum_, strTmp );
}

void KGameServer::AddInvalidChecksum( DWORD dwInvalidChecksum_, const std::string& strLogin_  )
{
    {
        KLocker lock( m_csInvalidChecksum );

        std::vector<KInvalidChecksumInfo>::iterator vit;
        vit = std::find_if( m_vecInvalidChecksum.begin(), m_vecInvalidChecksum.end(), 
            boost::bind( &KInvalidChecksumInfo::m_dwInvChecksum, _1 ) == dwInvalidChecksum_ );

        if( vit != m_vecInvalidChecksum.end() ) // 이미 등록된 체크섬인 경우는 횟수만 증가시킴
        {
            vit->m_nCount++;
            //부정 체크섬이 추가되면 전체를 덤프하지 않고 해당 체크섬만 프린트 한다.(수천명이 접속하면 너무 많다.)
            // 출력 포맷을 지정할 필요가 있어서 추후 참고하라고 기존 코드를 주석으로 남겨둠
            //std::wcout << boost::wformat( L" Add Existed Invalid Checksum : 0x%08x, %d \n" ) % dwInvalidChecksum_ % vit->m_nCount;
            START_LOG( cerr, L"Add Existed Invalid Checksum : " << dwInvalidChecksum_ << ", count: " << vit->m_nCount ) << END_LOG;
            return;
        }
    }

    // 처음 등록되는 체크섬인 경우
    {
        KInvalidChecksumInfo tmpInfo;
        tmpInfo.m_dwInvChecksum = dwInvalidChecksum_;
        tmpInfo.m_nCount        = 1;
        tmpInfo.m_regTime       = CTime::GetCurrentTime();
        tmpInfo.m_strLogin      = KncUtil::toWideString( strLogin_ );

        {
            KLocker lock( m_csInvalidChecksum );
            m_vecInvalidChecksum.push_back( tmpInfo );
        }
    }
}

bool KGameServer::DeleteInvalidChecksum( unsigned int nIndex_ )
{
    {
        KLocker lock( m_csInvalidChecksum );

        if( nIndex_ >= m_vecInvalidChecksum.size() )
        {
            START_LOG( cwarn, L"올바르지 못한 인덱스. index:" << nIndex_ )
                << BUILD_LOG( m_vecInvalidChecksum.size() ) << END_LOG;
            return false;
        }

        m_vecInvalidChecksum.erase( m_vecInvalidChecksum.begin() + nIndex_ );
    }

    START_LOG( cerr, L"invalid checksum 삭제됨. index: " << nIndex_ ) << END_LOG;

    return true;
}

void KGameServer::ClearInvalidChecksum()
{
    {
        KLocker lock( m_csInvalidChecksum );

        START_LOG( cerr, L"clear all invalid checksum. size:" << m_vecInvalidChecksum.size() ) << END_LOG;

        m_vecInvalidChecksum.clear();
    }
}

void KGameServer::DumpChecksum( OUT std::wostream& stm_ ) const
{
    {
        KLocker lock( m_csClientChecksum );

        stm_ << L" -- Client Binary Checksum -- (size:" << m_vecClientChecksum.size() << L")" << std::endl;

        int nCount = 0;
        std::vector<DWORD>::const_iterator vit;
        for( vit = m_vecClientChecksum.begin(); vit != m_vecClientChecksum.end(); vit++, nCount++ )
        {
            stm_ << boost::wformat( L"    Index : %d, checksum : 0x%08x \n" ) % nCount % *vit;
        }
    }

    {
        KLocker lock( m_csInvalidChecksum );

        if( m_vecInvalidChecksum.empty() ) return;
        stm_ << L" -- Client Invalid Binary Checksum -- (size:" << m_vecInvalidChecksum.size() 
            << L")" << std::endl;

        int nCount = 0;
        std::vector<KInvalidChecksumInfo>::const_iterator vit;
        for( vit = m_vecInvalidChecksum.begin(); vit != m_vecInvalidChecksum.end(); vit++, nCount++ )
        {
            stm_ << boost::wformat( L"    Index : %d, checksum : 0x%08x, Count : %d\n" ) % nCount % vit->m_dwInvChecksum % vit->m_nCount;
            stm_ << boost::wformat( L"    RegTime : %s, RegLogin : %s\n" ) % (const wchar_t*)vit->m_regTime.Format(_T("%Y-%m-%d %H:%M:%S")) % vit->m_strLogin.c_str();
        }
    }
}

void KGameServer::Tick()
{
    // 서버 리스트 갱신주기 확인 & 갱신 요청.
    if( ::GetTickCount() - m_dwServerInfoUpdateTick > ms_dwSendServerListGap )
    {
        KDB_EVENT_SERVER_INFO_REQ kPacket;
        kPacket.m_strIP         = NetCommon::GetLocalIPW();
        kPacket.m_usPort        = m_pkNetLayer->m_usPortMaster;
        kPacket.m_mapChannellingUserNum[KUser::USER_CT_TOTAL]   = m_pkSimLayer->m_kActorManager.GetCount();
        kPacket.m_nProtocolVer  = m_nProtocolVer;
        kPacket.m_bFirstDBWork  = false;
        kPacket.m_hSyncEvent    = NULL;

        for ( int i = KUser::USER_CT_PUBLISHER_1 ; i < KUser::USER_CT_MAX ; ++i ) {
            kPacket.m_mapChannellingUserNum[i] = GetChannellingUserNum(i);
        }

        m_pkDBLayer->QueueingEvent( KUserEvent::DB_EVENT_SERVER_INFO_REQ, NULL, 0, 0, kPacket );
        m_dwServerInfoUpdateTick = ::GetTickCount();
    }

    SiKStatisticsManager()->Tick();
    SiKSquareManager()->Tick();
    SiKSignBoard()->Tick();
    SiKSquareManager()->Tick();
    SiKRankManager()->Tick();
    SiKMiniGameRankManager()->Tick();
    SiKGCHelper()->Tick();
    SiKDonationManager()->Tick();
    SiKLunaNewYear()->Tick();
    SiKCharacterPollManager()->Tick();
    SiKRecomHelper()->Tick();
    SiKRainbow()->Tick();
    SiKCooperationEvent()->Tick();
    SiKMissionEvent()->Tick();
    SiKDicePlay()->Tick();
    SiKQuizBingo()->Tick();
    SiKGWCSupport()->Tick();
    SiKNewDonationManager()->Tick();
    SiKTimeDrop()->Tick();
    SiKGameCouponManager()->Tick();
    SiKEventMonsterManager()->Tick();
    SiKTickManager()->Tick();
    SiKHeroDungeonManager()->Tick();
    SiKStrengthManager()->Tick();
    SiKShutdownUser()->Tick();
    SiKJustInTime()->Tick();
    SiKEventDivideDispatcher()->Tick();
    SiKEclipseTimeEvent()->Tick();
    SiKPlayAuthManager()->Tick();
    SiKOlympicManager()->Tick();
    SiKPVPMatchManager()->Tick();
    SiKRitasChristmas()->Tick();
    SiKVitalityManager()->Tick();
    SiKSystemResourceManager()->Tick();
    SiKSocialCommerce()->Tick();
    SiKUserBillboardManager()->Tick();
}

void KGameServer::AddKGameModeInfo( IN OUT KGameModeCategory& kGameModeCategory_, int nID_, int nLevel_, IN bool bRandomable_, const luabind::object& table_ )
{
    JIF( table_.type() == LUA_TTABLE );

    KGameModeInfo kGameModeInfo;
    kGameModeInfo.m_nID = nID_;
    kGameModeInfo.m_nLevel = nLevel_;
    kGameModeInfo.m_bRandomable = bRandomable_;

    luabind::object::iterator oit;
    for( oit = table_.begin(); oit != table_.end(); ++oit )
    {
        int nMapID = -1 ;
        int nMapLevel= -1 ;
        luabind::object stages = (*oit);
        if( stages.type() == LUA_TTABLE )
        {
            nMapID      = luabind::object_cast<int>( stages[1] );
            nMapLevel   = luabind::object_cast<int>( stages[2] );
            if( nMapID != -1 && nMapLevel != -1 )
            {
                kGameModeInfo.m_vecGameStage.push_back( std::make_pair( nMapID, nMapLevel ) );
            }
        }
    }

    kGameModeCategory_.m_vecGameModeInfo.push_back( kGameModeInfo );
    
    START_LOG( clog, L"GameModeInfo Load.." )
        << BUILD_LOG( kGameModeInfo.m_nID )
        << BUILD_LOG( kGameModeInfo.m_nLevel )
        << BUILD_LOG( kGameModeInfo.m_vecGameStage.size() ) << END_LOG;

}

void KGameServer::AddGameModeCategory( IN int nID_, IN int nLevel_, IN KGameModeCategory& kGameModeCategory_ )
{

    kGameModeCategory_.m_nID        = nID_;
    kGameModeCategory_.m_nLevel     = nLevel_;
    m_kEventGameMode.m_vecGameModeCategory.push_back( kGameModeCategory_ );
}

KSerBuffer KGameServer::GetGameModeCategory( )
{
    static KSerBuffer serBuffEventGameMode;
    if( serBuffEventGameMode.GetLength() == 0 )
    {
        KEventPtr spEvent( new KUserEvent() );
        spEvent->m_usEventID = KUserEvent::EVENT_SET_GAME_MODE;

        KSerializer         ks;
        ks.BeginWriting( &spEvent->m_kbuff );
        ks.Put( m_kEventGameMode );
        ks.EndWriting();

        spEvent->m_kbuff.Compress();

        ks.BeginWriting( &serBuffEventGameMode );
        ks.Put( *spEvent );
        ks.EndWriting();
    }

    return serBuffEventGameMode;

}

void KGameServer::BuildPublisherUserCount( KActorPtr spActor_ )
{
    KUserPtr spActor = std::static_pointer_cast<KUser>( spActor_->shared_from_this() );

    if( !spActor )
    {
        START_LOG( cerr, L"퍼블리셔별 유저 취합시 객체가 유효하지 않음") << END_LOG;
        return;
    }
    if( !SiKGSSimLayer()->CheckAuthTypeFlag( spActor->GetAuthType() ) )
    {
        START_LOG( cwarn, L"퍼블리셔별 유저 취합시 인증타입이 유효하지 않음")
            << BUILD_LOG( spActor->GetName() )
            << BUILD_LOG( spActor->IsAdmin() )
            << BUILD_LOG( spActor->GetAuthType() ) << END_LOG;
        return;
    }

    KLocker lock( m_csPublishUserCount );

    std :: map<DWORD, DWORD>::iterator mit;
    mit = m_mapPublisherUserCount.find( spActor->GetAuthType() );
    if( mit != m_mapPublisherUserCount.end() )
    {
        mit->second += 1;
    }
    else
    {
        m_mapPublisherUserCount[spActor->GetAuthType()] = 1;
    }
}

void KGameServer::GetPublisherUser( std::map<DWORD,DWORD>& mapPublishUser_ )
{
    KLocker lock( m_csPublishUserCount );
    mapPublishUser_ = m_mapPublisherUserCount;
}

void KGameServer::ClearPublisherUser() //일본이 계속 잘돌아 간다면...이녀석들을 배열로 만들어야지.
{
    KLocker lock( m_csPublishUserCount );
    m_mapPublisherUserCount.clear();

    if( SiKGSSimLayer()->CheckAuthTypeFlag( KSimLayer::AT_NETMARBLE ) )
    {
        m_mapPublisherUserCount[KSimLayer::AT_NETMARBLE] = 0L;
    }

    if( SiKGSSimLayer()->CheckAuthTypeFlag( KSimLayer::AT_GASH ) )
    {
        m_mapPublisherUserCount[KSimLayer::AT_GASH] = 0L;
    }

    if( SiKGSSimLayer()->CheckAuthTypeFlag( KSimLayer::AT_LEVELUP ) )
    {
        m_mapPublisherUserCount[KSimLayer::AT_LEVELUP] = 0L;
    }

    if( SiKGSSimLayer()->CheckAuthTypeFlag( KSimLayer::AT_NTREEV ) )
    {
        m_mapPublisherUserCount[KSimLayer::AT_NTREEV] = 0L;
    }

	if( SiKGSSimLayer()->CheckAuthTypeFlag( KSimLayer::AT_LEVELUP_PH ) )
	{
		m_mapPublisherUserCount[KSimLayer::AT_LEVELUP_PH] = 0L;
	}
}

void KGameServer::SetMsgServerList( IN std::vector<KServerInfo>& vecMsgServerList_ )
{
    KLocker lock( m_csMsgServerList );

    m_vecMsgServerList  = vecMsgServerList_;

    START_LOG( clog, L"[MsgServer List] (size:" << vecMsgServerList_.size() << L")" ) << END_LOG;
    for( std::vector<KServerInfo>::iterator vit = vecMsgServerList_.begin() ;
        vit != vecMsgServerList_.end() ; ++vit )
    {
        START_LOG( clog, L" - NAME:" << vit->m_strName << L", IP:" << KncUtil::toWideString( vit->m_strIP ) << L", PORT:" << vit->m_usPort ) << END_LOG;
    }
}

void KGameServer::GetSuitableMsgServer( OUT KServerInfo& kInfo_ )
{
    std::vector<KServerInfo> vecOrderedList;

    {
        KLocker lock( m_csMsgServerList );

        if ( m_vecMsgServerList.empty() ) {
            kInfo_.m_strIP.clear();     // IP 변수를 비워내서 무효접속 시도가 일어나지 않도록 한다.
            return;
        }

        vecOrderedList = m_vecMsgServerList;
    }

    std::sort( vecOrderedList.begin(), vecOrderedList.end(), 
        boost::bind( &KServerInfo::m_iUserNum, _1 ) < boost::bind( &KServerInfo::m_iUserNum, _2 ) );

    kInfo_ = *vecOrderedList.begin();

    START_LOG( clog, L"[Get MsgServer Info]" )
        << BUILD_LOG( kInfo_.m_strName )
        << BUILD_LOG( KncUtil::toWideString( kInfo_.m_strIP ) )
        << BUILD_LOG( kInfo_.m_usPort ) << END_LOG;
}

void KGameServer::SetDiscountEvent( bool bEnable )
{
    m_bDiscountEvent = bEnable;
}

void KGameServer::AddDiscountEventItem( IN GCITEMID nItemID, IN short sDiscountRate )
{
    KLocker lock( m_csDiscountEventItem );

    std::vector< std::pair<int, short> >::iterator iter = m_vecDiscountEventItem.begin();

    while( iter != m_vecDiscountEventItem.end() )
    {
        if( ( iter->first ) == nItemID )
        {
            START_LOG( cwarn, L"이미 할인되고 있는 아이템임." )
                << BUILD_LOG( nItemID )
                << BUILD_LOG( iter->second ) << END_LOG;

            return;
        }

        iter++;
    }

    m_vecDiscountEventItem.push_back( std::make_pair(nItemID, sDiscountRate) );

    return;
}

void KGameServer::DeleteDiscountEventItem( IN GCITEMID nItemID )
{
    KLocker lock( m_csDiscountEventItem );

    std::vector< std::pair<int, short> >::iterator iter = m_vecDiscountEventItem.begin();

    while( iter != m_vecDiscountEventItem.end() )
    {
        if( ( iter->first ) == nItemID )
        {
            iter = m_vecDiscountEventItem.erase( iter );
            return;
        }

        iter++;
    }

    START_LOG( cwarn, L"할인 아이템 리스트에 존재하지 않음." )
        << BUILD_LOG( nItemID ) << END_LOG;

    return;
}

void KGameServer::ClearDiscountEventItem()
{
    KLocker lock( m_csDiscountEventItem );

    m_vecDiscountEventItem.clear();
}

bool KGameServer::GetDiscountItem( IN GCITEMID nItemID, OUT short& sDiscountRate )
{
    KLocker lock( m_csDiscountEventItem );

    if( !m_bDiscountEvent )
    {
        return false;
    }

    std::vector< std::pair<int, short> >::iterator iter = m_vecDiscountEventItem.begin();

    while( iter != m_vecDiscountEventItem.end() )
    {
        if( ( iter->first ) == nItemID )
        {
            sDiscountRate = (iter->second);
            return true;
        }
        iter++;
    }

    return false;
}

void KGameServer::ApplyGuideLineAfter( int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond )
{
    m_timeApplyGuideLine = CTime( nYear, nMonth, nDay, nHour, nMinute, nSecond );
}

CTime KGameServer::GetPivotFirstLoginTimeToApplyGuideLine()
{
    return m_timeApplyGuideLine;
}

#include <boost/shared_array.hpp>
bool KGameServer::dofileUTF8( const char* szFileName_ )
{
    _JIF( false, return false );
    FILE* fp = fopen( szFileName_, "rb" );
    _JIF( fp, return false );

    fseek( fp, 0, SEEK_END );
    int nFileByteLen = ftell( fp );
    fseek( fp, 0, SEEK_SET );

    _JIF( nFileByteLen > 3 , return false );
    boost::shared_array<char> spszChannel( new char [nFileByteLen+1] );

    _JIF( spszChannel, return false );

    if ( nFileByteLen )
    {
        fread( spszChannel.get(), nFileByteLen, 1, fp );
        spszChannel[nFileByteLen] = 0;
    }
    fclose( fp );
    _JIF( 0 == lua_dostring( m_sptlua.get(), spszChannel.get() ), return false );

    return true;
    
}

void KGameServer::ResetPubEvent()
{
    KLocker lock( m_csPubEventBenefit );
    m_bPubEvent = false;
    m_vecPubEventItem.clear();
    m_nPubEventGP=0;
    m_strPubEvent.clear();
}

void KGameServer::SetPubEventComment( const char* szComment_ )
{
    JIF( szComment_ != NULL );
    KLocker lock( m_csPubEventBenefit );
    m_strPubEvent = m_stringTable.GetValue( KncUtil::toWideString( szComment_ ).c_str() );
}

void KGameServer::AddPubEventItem( IN const GCITEMID nItemId_, IN const int nCount_, IN const int nPeriod_ )
{
    KLocker lock( m_csPubEventBenefit );
    if( std::find_if( m_vecPubEventItem.begin(), m_vecPubEventItem.end(),
        boost::bind( &KItem::m_ItemID, _1) == nItemId_ ) != m_vecPubEventItem.end() )
    {
        START_LOG( cwarn, L"이미 중복된 아이템이 있습니다. ItemID : " << nItemId_ ) << END_LOG;
        return;
    }

    KItem kItem;
    kItem.m_ItemID = nItemId_;
    kItem.m_nCount = nCount_;
    kItem.m_nPeriod = nPeriod_;

    m_vecPubEventItem.push_back( kItem );
}

void KGameServer::SetPubEventGP( int nGP )
{
    JIF( nGP >= 0 );
    KLocker lock( m_csPubEventBenefit );
    m_nPubEventGP = nGP;
}

void KGameServer::DumpPubEvent(  OUT std::wostream& stm_ ) const
{
    stm_ << L"-- PubEvent -- " << std::endl;
    std::vector<KItem>::const_iterator vit;
    KLocker lock( m_csPubEventBenefit );

    stm_ << L"Enable : " << std::boolalpha << m_bPubEvent << std::noboolalpha << std::endl;
    stm_ << L"Comment : " << m_strPubEvent << std::endl;
    stm_ << L"GP : " << m_nPubEventGP << std::endl;

    if( m_vecPubEventItem.empty() ) // 아이템이 없으면 넘어 가자..
        return;

    stm_ << L"  -- ItemList --" << std::endl;
    for( vit = m_vecPubEventItem.begin() ; vit != m_vecPubEventItem.end() ; ++vit )
        stm_ << L"(" << vit->m_ItemID << L"," << vit->m_nCount << L"), ";
    stm_ << std::endl;
}

void KGameServer::ClearWeaponInfo()
{
    KLocker lock( m_csWeaponInfo );
    m_mapWeaponInfo.clear();
}

void KGameServer::AddWeaponInfo( GCITEMID dwItemID_, char cCharType_, char cJobLV_ )
{

    KWeaponInfo weaponInfo;
    weaponInfo.m_dwItemID   = dwItemID_;
    weaponInfo.m_cCharType  = cCharType_;
    weaponInfo.m_cJobLV     = cJobLV_;

    KLocker lock( m_csWeaponInfo );
    m_mapWeaponInfo.insert( std::make_pair( weaponInfo.m_dwItemID, weaponInfo ) );
}

DWORD KGameServer::ChkValidWeapon( IN OUT std::vector<KEquipItemInfo>& vecEquip_, IN char cCharType_,
                                   IN char cMaxJobLV_, OUT char& cCurrentJobLv_ )
{
    SET_ERROR( ERR_UNKNOWN );
    cCurrentJobLv_ = 0;
    int nCheckCount = 0;
    std::vector<KEquipItemInfo>::iterator vit;
    KLocker lock( m_csWeaponInfo );
    if( m_mapWeaponInfo.empty() )
    {
        SET_ERROR( ERR_WEAPON_01 );
        goto END_PROC;
    }

    for( vit = vecEquip_.begin() ; vit != vecEquip_.end() ; ++vit )
    {
        std::map< DWORD, KWeaponInfo >::iterator mit;
        mit = m_mapWeaponInfo.find( vit->m_dwID );

        if( mit == m_mapWeaponInfo.end() )
            continue;

        if( mit->second.m_cCharType != cCharType_ )
        {
            SET_ERROR( ERR_WEAPON_02 );
            goto END_PROC;
        }

        if( mit->second.m_cJobLV > cMaxJobLV_ )
        {
            // 장착 무기 초기화. 080402. tslayer
            vit->m_dwID = 0;
            vit->m_dwUID = 0;
            vit->m_cEnchantLevel = 0;

            SET_ERROR( ERR_WEAPON_03 );
            goto END_PROC;
        }

        if( nCheckCount > 0 )
            SET_ERROR( ERR_WEAPON_00 );

        ++nCheckCount;
        cCurrentJobLv_ = mit->second.m_cJobLV;
    }

    if( nCheckCount <= 1 )
        SET_ERROR( NET_OK );

END_PROC:
    return NetError::GetLastNetError();
}

void KGameServer::SetAbuseInfo( int nAbuseCount_, int nAbuseTime_, int nAbusePlayerCount_ )
{
    JIF( nAbuseTime_ > 0 && nAbuseCount_ > 0 );
    KLocker lock( m_csAbuseInfo );
    m_nAbuseCount           = nAbuseCount_;
    m_nAbuseTime            = nAbuseTime_;
    m_nAbusePlayerCount     = nAbusePlayerCount_;
}

bool KGameServer::LoadScript( OUT std::string& strScript_ )
{
    _JIF( 0 == lua_dofile( m_sptlua.get(), "InitEnchant.lua" ), strScript_ = "InitEnchant.lua"; return false );      // 050517. 강화 확률 설정.
    _JIF( 0 == lua_dofile( m_sptlua.get(), "InitDrop.lua" ), strScript_ = "InitDrop.lua"; return false );         // 050525. Drop system 설정.
    _JIF( 0 == lua_dofile( m_sptlua.get(), "InitKairo.lua" ), strScript_ = "InitKairo.lua"; return false );        // 061113. florist. 1:1 아이템 변환 시스템.
    _JIF( 0 == lua_dofile( m_sptlua.get(), "InitPetScript.lua" ), strScript_ = "InitPetScript.lua"; return false );    // 070107. kkurrung. 펫 먹이 관련.
    _JIF( 0 == lua_dofile( m_sptlua.get(), "WeaponInfo.lua" ), strScript_ = "WeaponInfo.lua"; return false );       // 070822. kkurrung. 무기 정보 읽음.
    _JIF( SiKResultManager()->_LoadScript(strScript_),      return false );
    strScript_.clear();
    _JIF( SiKGameDrop()->_LoadScript(strScript_),           return false );
    _JIF( SiKMissionManager()->_LoadScript(strScript_),     return false );
    _JIF( SiKDungeonEntrance()->_LoadScript(strScript_),    return false );
    _JIF( SiKRequierInfo()->_LoadScript(strScript_),        return false );
    _JIF( SiKGCHelper()->_LoadScript(strScript_),           return false );
    _JIF( SiKSignBoard()->_LoadScript(strScript_),          return false );
    _JIF( SiKCollectionManager()->_LoadScript(strScript_),  return false );
    _JIF( SiKSkillManager()->_LoadScript(strScript_),       return false );
    strScript_.clear();
    _JIF( SiKGachaManager()->_LoadScript(strScript_),       return false );
    _JIF( SiKCoupleSystem()->_LoadScript(strScript_),       return false );
    _JIF( SiKDonationManager()->_LoadScript(strScript_),    return false );
    _JIF( SiKLunaNewYear()->_LoadScript(strScript_),        return false );
    _JIF( SiKCharacterPollManager()->_LoadScript(strScript_),   return false );
    _JIF( SiKGPCapsuleManager()->_LoadScript(strScript_),   return false );
    _JIF( SiKSHCheckPoint()->_LoadScript(strScript_),       return false );
    _JIF( SiKRecomHelper()->_LoadScript(strScript_),       return false );
    _JIF( SiKMissionPack()->_LoadScript(strScript_),       return false );
    _JIF( SiKPreHackingCheckManager()->_LoadScript(strScript_), return false );
    _JIF( SiKIPAdvantage()->_LoadScript(strScript_), return false );
    _JIF( SiKDicePlay()->_LoadScript(strScript_),    return false );
    _JIF( SiKCooperationEvent()->_LoadScript(strScript_), return false );
    _JIF( SiKItemTradeManager()->_LoadScript(strScript_), return false );
    _JIF( SiKQuizBingo()->_LoadScript(strScript_), return false );
    _JIF( SiKNewDonationManager()->_LoadScript(strScript_), return false );
    _JIF( SiKSpecialReward()->_LoadScript(strScript_), return false );
    _JIF( SiKItemManager()->_LoadScript(strScript_), return false );
    _JIF( SiKTimeDrop()->_LoadScript(strScript_), return false );
    _JIF( SiKPresentBox()->_LoadScript(strScript_), return false );
    _JIF( SiKServerBenefit()->_LoadScript(strScript_), return false );
    _JIF( SiKCharDefault()->_LoadScript(strScript_), return false );
    _JIF( SiKEventMonsterManager()->_LoadScript(strScript_), return false );
    _JIF( SiKLookEquipment()->_LoadScript(strScript_), return false );
    _JIF( SiKGambleBuyManager()->_LoadScript(strScript_), return false );
    _JIF( SiKChannelTypeEvent()->_LoadScript(strScript_), return false );
    _JIF( SiKCharPromotionSystem()->_LoadScript(strScript_), return false );
    strScript_.clear();
    _JIF( SiKHeroDungeonManager()->_LoadScript(strScript_), return false );
    strScript_.clear();
    _JIF( SiKGawibawibo()->_LoadScript(strScript_), return false );
    _JIF( SiKSocks()->_LoadScript(strScript_), return false );
    _JIF( SiKChangeWeaponManager()->_LoadScript(strScript_), return false );
    _JIF( SiKPlantTree()->_LoadScript(strScript_), return false );
    _JIF( SiKGuildManager()->_LoadScript(strScript_), return false );
    _JIF( SiKChoiceDrop()->_LoadScript(strScript_), return false );
    _JIF( SiKVipEventManager()->_LoadScript(strScript_), return false );
    _JIF( SiKMagicBox()->_LoadScript(strScript_), return false );
    _JIF( SiKRKTornado()->_LoadScript(strScript_), return false );
    _JIF( SiKGCPoint()->_LoadScript(strScript_), return false );
    _JIF( SiKSphinx()->_LoadScript(strScript_), return false );
    _JIF( SiKDotaManager()->_LoadScript(strScript_), return false );
    strScript_.clear();
    _JIF( SiKStrengthManager()->_LoadScript(strScript_), return false );
    _JIF( SiKAgitHelper()->_LoadScript(strScript_), return false );
    _JIF( SiKMultiLanguageString()->_LoadScript(strScript_), return false );
    _JIF( SiKEscort()->_LoadScript(strScript_), return false );
    _JIF( SiKJustInTime()->_LoadScript(strScript_), return false );
    _JIF( SiKCharismasPresent()->_LoadScript(strScript_), return false );
    _JIF( SiKPostConfig()->_LoadScript(strScript_), return false );
    _JIF( SiKEventDivideDispatcher()->_LoadScript(strScript_), return false );
    _JIF( SiKEquipLvDown()->_LoadScript(strScript_), return false );
    strScript_.clear();
    _JIF( SiKEclipseTimeEvent()->_LoadScript(strScript_), return false );
    _JIF( SiKEclipsePlot()->_LoadScript(strScript_), return false );
    _JIF( SiKEclipseCollect()->_LoadScript(strScript_), return false );
    _JIF( SiKMonsterCardMix()->_LoadScript(strScript_), return false );
    _JIF( SiKDepotManager()->LoadScript(), return false );
    _JIF( SiKJobMissionGuide()->_LoadScript(strScript_), return false );
    _JIF( SiKSpecialMonsterManager()->_LoadScript(strScript_), return false );
    _JIF( SiKSongkran()->_LoadScript( strScript_ ), return false );
    _JIF( SiKSealedChar()->_LoadScript( strScript_ ), return false );
    _JIF( SiKOlympicManager()->_LoadScript( strScript_ ), return false );
    _JIF( SiKItemCompose()->_LoadScript( strScript_ ), return false );
    _JIF( SiKDungeonManager()->_LoadScript( strScript_ ), return false );
    _JIF( SiKGachaLottery()->_LoadScript( strScript_ ), return false );
    _JIF( SiKGetLimit()->_LoadScript(strScript_), return false );		
    _JIF( SiKResultReward()->_LoadScript( strScript_ ), return false );
    _JIF( SiKPVPMatchManager()->_LoadScript( strScript_ ), return false );
    _JIF( SiKRitasChristmas()->_LoadScript( strScript_ ), return false );
	_JIF( SiKLevelResetEvent()->_LoadScript(strScript_), return false );
    _JIF( SiKRankManager()->_LoadScript( strScript_ ), return false );
    _JIF( SiKBuffDonation()->_LoadScript( strScript_ ), return false );
    _JIF( SiKAccumulateAttendanceManager()->_LoadScript( strScript_ ), return false );
    _JIF( SiKErrandManager()->_LoadScript( strScript_ ), return false );
    _JIF( SiKSubjectManager()->_LoadScript( strScript_ ), return false );
    _JIF( SiKCharacterGraduation()->_LoadScript( strScript_ ), return false );
    _JIF( SiKSystemGuide()->_LoadScript( strScript_ ), return false );
    _JIF( SiKCharSlotManager()->_LoadScript(strScript_), return false );
    _JIF( SiKVitalityManager()->_LoadScript(strScript_), return false );
    _JIF( SiKJumpingChar()->_LoadScript( strScript_ ), return false );
    _JIF( SiKSystemResourceManager()->_LoadScript( strScript_ ), return false );
    _JIF( SiKInfinityDungeon()->_LoadScript( strScript_ ), return false );
    _JIF( SiKMissionGettableCondition()->_LoadScript( strScript_ ), return false );
    _JIF( SiKGachaPong()->LoadScript(), return false );
    _JIF( SiKPetGlyphMng()->LoadScript(), return false );
    _JIF( SiKStageReward()->_LoadScript( strScript_ ), return false );
    _JIF( SiKBlockedAttackIP()->LoadScript(), return false );

    return true;
}

bool KGameServer::ReLoadScript()
{// 사내에서만 실행되도록 해두자.

    _JIF( 0 == lua_dofile( m_sptlua.get(), "a.lua" ), return false );

    std::string strMsg;
    std::string strScript;
    bool bLoadScriptSucess = LoadScript( strScript );

    if ( bLoadScriptSucess ) {
        strMsg = "[GameServer] LoadScript Success. Thank you.";
    }
    else {
        strMsg = "[GameServer] Error!! LoadScript Failed.. Check ";
        strMsg += strScript;
    }

    HWND	m_hRemote;
    m_hRemote = FindWindow( NULL, L"KEsdRemoteController_Server" );
    
    if ( m_hRemote == NULL ) {
        return false;
    }

    //{{ 리모컨 패치결과 전달
    {
        struct sPatchResult
        {
            int m_nID;
            TCHAR m_tcErrMsg[100];
        };
        sPatchResult patchResult = { 1000, {0,} };
        patchResult.m_nID = 1000;

        CString strTemp = strMsg.c_str();
        TCHAR* tcErrMsg = (TCHAR*)(LPCTSTR)strTemp;
        _tcsncpy( patchResult.m_tcErrMsg, tcErrMsg, _tcslen(tcErrMsg) );

        COPYDATASTRUCT cds;
        cds.dwData = 0; // Index
        cds.cbData = sizeof(patchResult);
        cds.lpData = (LPVOID)&patchResult;
        ::SendMessage( m_hRemote, WM_COPYDATA, NULL, (LPARAM)&cds);

        _LOG_SUCCESS( bLoadScriptSucess, L"패치 결과.. " << bLoadScriptSucess )
            << BUILD_LOG( KncUtil::toWideString(strMsg) ) << END_LOG;
    }

    return bLoadScriptSucess;
}

void KGameServer::IncreaseChannellingUserNum( IN const USHORT& usChannellingType_ )
{
    KLocker lock( m_csChannelling );
    ++m_mapChannellingUserNum[usChannellingType_];
}

void KGameServer::DecreaseChannellingUserNum( IN const USHORT& usChannellingType_ )
{
    KLocker lock( m_csChannelling );
    if ( m_mapChannellingUserNum[usChannellingType_] > 0 ) {
        --m_mapChannellingUserNum[usChannellingType_];
    }
}
void KGameServer::GetPubEventItemList( OUT std::vector<KItem>& vecItemList_ )
{
    {
        KLocker lock( m_csPubEventBenefit );
        vecItemList_ = m_vecPubEventItem;
    }
}

int KGameServer::GetPubEventGP()
{
    return m_nPubEventGP;
}

void KGameServer::SetMaxUserNum( IN DWORD dwMaxUser_ )
{
    KLocker lock( m_csMaxUserNum );
    m_dwMaxUserNum = dwMaxUser_;
}

DWORD KGameServer::GetMaxUserNum()
{
    KLocker lock( m_csMaxUserNum );
    return m_dwMaxUserNum;
}

void KGameServer::SendGetPackageListNotToDB()
{
    SiKGameServer()->GetDBLayer()->QueueingID( KUserEvent::DB_EVENT_GET_PACKAGE_LIST_NOT, L"", 0, 0 );
}

void KGameServer::ReConnectCenterServer()
{
    // connect to Center Server.
    KCnConnector* pkCenterProxy = SiKCnConnector();
    pkCenterProxy->Init();
    pkCenterProxy->SetName( m_strServerName );

    LIF( pkCenterProxy->Connect() );
}

void KGameServer::SetServerIP()
{
    std::vector< KServerInfo >::const_iterator vit;
    KLocker lock( m_csServerInfo );

    vit = std::find_if( m_vecServerInfoList.begin(), m_vecServerInfoList.end(),
        boost::bind( &KServerInfo::m_strIP, _1) == NetCommon::GetLocalIP() );

    if ( vit != m_vecServerInfoList.end() ) {
        SetIP( vit->m_strConnIP );
    }
}

void KGameServer::SetIP( IN const std::string& strIP_ )
{
    KLocker lock( m_csIP );
    m_strIP = strIP_;
}

std::string KGameServer::GetIP()
{
    KLocker lock( m_csIP );
    return m_strIP;
}

void KGameServer::SetServerCrashBefore()
{
    m_bServerCrashBefore = true;
}

void KGameServer::SetUserNickToLoginID( IN const std::wstring& wstrNickName_, IN const DWORD& dwUserUID_ )
{
    KLocker lock( m_csNickLoginIDInfo );
    std::map<std::wstring,DWORD>::iterator mit;

    mit = m_mapUserNickLoginIDInfo.find( wstrNickName_ );
    if( mit == m_mapUserNickLoginIDInfo.end() ) {
       m_mapUserNickLoginIDInfo[wstrNickName_] = dwUserUID_;
       return;
    }
    mit->second = dwUserUID_;
}

bool KGameServer::GetUserNickToLoginID( IN const std::wstring& wstrNickName_, OUT DWORD& dwUserUID_ )
{
    KLocker lock( m_csNickLoginIDInfo );
    std::map<std::wstring,DWORD>::iterator mit;

    mit = m_mapUserNickLoginIDInfo.find( wstrNickName_ );
    if( mit != m_mapUserNickLoginIDInfo.end() ) {
        dwUserUID_ = mit->second;
        return true;
    }
    return false;
}

void KGameServer::DeleteUserNickToLoginID( IN const std::wstring& wstrNickName_ )
{
    KLocker lock( m_csNickLoginIDInfo );
    std::map<std::wstring,DWORD>::iterator mit;

    mit = m_mapUserNickLoginIDInfo.find( wstrNickName_ );
    if( mit != m_mapUserNickLoginIDInfo.end() ) {
        m_mapUserNickLoginIDInfo.erase( wstrNickName_ );
    }
}
