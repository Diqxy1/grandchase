#include "stdafx.h"
#include "Replay.h"
#include "../MyD3D.h"
#include "../GCStateRoom.h"
#include "KDInput.h"
#include "../GCStateGame.h"
#include "KGCRoomManager.h"
#include "mission/GCMissionManager.h"
#include "KGCAnnounceMgr.h"
#include "GCUIHeader.h"
#include "KGCDeathMatch.h"
#include "BuddySystem/KFriendShip.h"
#include "KGCEmoticon.h"
#include "KGCMinigameManager.h"
#include "../GCStateMachine.h"
#include "KGCDeathMatch.h"
#include "KGCMinigameManager.h"
#include "GCCoupleSystem.h"
#include "../GCSKT.h"
#include "gcui/KGCLogoutBox.h"
#include "gcui/KGCGPCapsuleObtainWnd.h"
#include "gcui/KGCCouponNPCDlg.h"
#include "gcui/KGCSubscriptionBox.h"
#include "gcui/KGCSubscriptionDlg.h"
#include "gcui/KGCFileFindDialog.h"
#include "gcui/GCPuzzleBox.h"
#include "gcui/KGCBingo.h"
#include "gcui/KGCQuiz.h"
#include "gcui/GCInventoryFullMsg.h"
#include "gcui/KGCHurryUPDlg.h"
#include "gcui/GCCharacterSelectBox.h"
#include "gcui/GCReplayRecordREC.h"
#include "gcui/GCGawibawiboGame.h"
#include "gcui/KGCSocksTreeDlg.h"
#include "gcui/GCAttributeSelectBox.h"
#include "gcui/GCLevelupInfoBox.h"
#include "gcui/GCNicknameCampaignBox.h"
#include "gcui/GCCoordiCatalogDlg.h"
#include "gcui/GCPlantTree.h"
#include "gcui/GCPlantTreeRewardItemListView.h"
#include "gcui/GCGuildStore.h"
#include "gcui/GCGuildLevelList.h"
#include "gcui/GCGuildBenefitListBar.h"
#include "gcui/KGCUserMigrationDlg.h"
#include "gcui/KGCNewClearSealDlg.h"
#include "KGCAgitGameManager.h"
#include "gcui/KGCDotaShop.h"
#include "gcui/KGCPartyWindow.h"
#include "gcui/KGCPartyDungeon.h"
#include "gcui/KGCPartyMatch.h"
#include "gcui/KGCPartyInviteMsgBox.h"
#include "KGCPartyManager.h"
#include "GCUI/GCGameInventory.h"
#include "gcui/KGCMantisReportDlg.h"
#include "GCUI/GCMsgShutDown.h"
#include "gcui/KGCSurveyDlg.h"
#include "gcui/KGCGwcEvent.h"
#include "gcui/KGCGwcRewardCatalogDlg.h"
#include "gcui/GCChristmasGiftDlg.h"
#include "GCUI/GCPostLetter.h"
#include "KGCPostLetterManager.h"
#include "gcui/GCDepotDlg.h"
#include "./DragManager.h"
#include "./KGCDepotManager.h"
#include "KGCWelcomeBackNew.h"
#include "GCUI/GCItemLevelDownDlg.h"
#include "GCUI/GCHotTimeSearchDlg.h"
#include "GCUI/GCMonsterCardCompose.h"
#include "./GCUI/GCRoomListDlgS6.h"
#include "GCui/GCEventAlarmDlg.h"
#include "GCUI/KGCAdventureDialog.h"
#include "GCui/GCLottery.h"
#include "GCUI/GCPlaytimeNotice.h"
#include "GCUI/GC9thEventDlg.h"
#include "GCUI/GCLevelItemDlg.h"
#include "GCUI/KGCItemCombinationDlg.h"
#include "GCUI/GCItemCreationSelectBox.h"
#include "GCUI/GCItemBindDlg.h"
#include "GCUI/KGCAddItemInClient.h"
#include "GCUI/KGCAddItemInClientItemList.h"
#include "GCUI/GCEscMenuDlg.h"
#include "KGCJumpingCharManager.h"
#include "KGCVirtualDepotManager.h"
#include "KGCHeroDungeonSeason5.h"
#include "KGCEventDungeonSeason5.h"
#include "KGCInfinityDungeonManager.h"
#include "KGCHeroDungeonUpgradeSlot.h"
#include "KGCGachaPongDlg.h"
#include "GCUI/GCCoordiViewDlg.h"
#include "KGCRoomOptionCharDlg.h"
#include "KGCRoomOptionRankDlg.h"
#include "KGCRoomListFilterDlg.h"
#include "GCUI/GCCustomizarionDlg.h"
#include "GCUI/GCCutInSkinDlg.h"
#include "GCUI/GCRoomAnimationDlg.h"
#include "GCUI/KGCQuantityPurchaseBox.h"
#include "GCUI/GCQuantityUseBox.h"
#include "GCUI/GCPetMagicStoneItemSlot.h"
#include "GCUI/GCPetMagicStoneDlg.h"
#include "GCUI/GCHeroTicketUseWnd.h"
#include "GCUI/GCAllCollection.h"

IMPLEMENT_CLASSNAME( KGCUIScene );

typedef std::vector<KD3DWnd*>::iterator WndListItr;

KGCUIScene* g_pkUIScene = NULL;

#define TOGGLEBACKGROUND_FRAME 25
#define BACKGROUND_ALPHA 0xb0

KGCUIScene::KGCUIScene( const std::string& strSceneName /* = "" */ ) : KD3DUIScene( strSceneName )
, m_bAllStop(false)
{
    g_pkUIScene             = this;
    m_bMsgBoxModal  = false;
    m_bChangeRoomTitle = false;
    m_bCreateRoom   = false;
    m_bConnect      = false;
    m_bRPLoadSuccess        = false;
    m_iCoverPosSave         = 0;
    m_pkMouseCursor         = NULL;
    m_pkGameInventory       = NULL;
	m_pkBuyCashBox          = NULL;
	m_pkPetCostumeInven     = NULL;
    m_pkServerScene         = NULL;
	m_pkCharSelectScene     = NULL;
	m_pkCharGainScene		= NULL;
    m_pkWorldMapScene       = NULL;
    m_pkMatchScene          = NULL;
    m_pkCashShopScene       = NULL;
    m_pkGPShopScene         = NULL;
    m_pkMyInfoScene         = NULL;
    m_pkRoomSceneS6         = NULL;
    m_pkCoordiShopScene     = NULL;
    m_pkCurMsgBox           = NULL;
    m_pkMsgBox              = NULL;
	m_pkMsgBox2             = NULL;
    m_pkPasswdBox           = NULL;
    m_pkItemReceiveBox      = NULL;
    m_pkGemBuyBox           = NULL;
    m_pkNickNameBox         = NULL;
    m_pkOption              = NULL;
    m_pkLoadingBox          = NULL;
    m_pkFairPlayReport      = NULL;
    m_pkGuideNewbie			= NULL;
    m_pkGuideAzit           = NULL;
    m_pkLoadingScene        = NULL;
    m_pkGameOverlayUI       = NULL;
    m_pkSquareOverlayUI     = NULL;
    m_pkSquareScene         = NULL;
    m_pkOnGameDlg           = NULL;
    m_pkCandidateWnd        = NULL;
    m_pkColorDlg            = NULL;
    m_pkNateOn              = NULL;
	m_pkNateOnMessage       = NULL;
	m_pkNateOnInvite        = NULL;
    m_pkToolTip             = NULL;
    m_pkPetPreview          = NULL;
    m_pkEventBox            = NULL;
    m_pkRepetitionEventBox  = NULL;
    m_pkPetEvolutionBox     = NULL;
    m_pkSelectPetNameBox    = NULL;
    m_pkAttendanceCalendar  = NULL;
    m_pkPerfectAttendance   = NULL;
    m_pkNewTutorialOption   = NULL;
    m_pkLuckyDayReceiveBox  = NULL;
    m_pkGradeBox            = NULL;
    m_pkChannelListBox      = NULL;
    m_pkBackGroundWhite     = NULL;
    m_pkWorldMapContainer   = NULL;
    m_pkBonusChargeBox      = NULL;
    m_pkActiveWnd			= NULL;
    m_pkSquareLoadingScene  = NULL;
    m_pkInviteRequestBox    = NULL;
    m_pkInviteBox           = NULL;
    m_pkNeonsignSelectBox   = NULL;
    m_pkNeonsignPreview     = NULL;
    m_pkEmoticonCollection  = NULL;
    m_pkTitleCollection     = NULL;
    m_pkPetCollection       = NULL;
    m_pkCTConditionWindow   = NULL;
    m_pkConnectionMsg       = NULL;
    m_pkTreaureItemBox      = NULL;
    m_pkRanking				= NULL;
    m_pkWelcomeBackDlg      = NULL;
    m_pkAskUseVirtualCash   = NULL;
    m_pkCollectionItemInfoBox = NULL;
    m_pkMiniGameDlg = NULL;
    m_pkNateOnInputDlg      = NULL;
    m_pkEmoticonSelect      = NULL;
    m_pkNateOnAddFriend     = NULL;
	m_pkBlockToAddFriend	= NULL;
	m_pkSkillTree			= NULL;
    m_pkNewClearSealDlg		= NULL;
    m_pkCoordiClearSealDlg  = NULL;
	m_pkCoupleViewer		= NULL;
	m_pkCoupleAccept		= NULL;
	m_pkCouplePropose		= NULL;
	m_pkCoupleWait			= NULL;
	m_pkCoupleMessage		= NULL;
	m_pkCoupleGoodbye		= NULL;
	m_pkContributionDlg		= NULL;
	m_pkContributionBox		= NULL;
	m_pkHappyNewYearNPCDlg	= NULL;
    m_pkVipGachaBox         = NULL;
	m_pkAgreement			= NULL;
	m_pkAgreement1301		= NULL;
	m_pkLogoutBox = NULL;
    m_pkRecommendList = NULL;
	m_pkGPCapsuleDlg			= NULL;	
	m_pkGPCapsuleObtainWnd		= NULL;
	m_pkDicePlay			= NULL;
	m_pkCouponNPCDlg			= NULL;
	m_pkSubscriptionDlg = NULL;
	m_pkSubscriptionBox = NULL;
    m_pkCooperationEventDlg = NULL;
    m_pkGuildMarkRegistDlg = NULL;
    m_pkGuildDlg        = NULL;
    m_pkGuildEnterDlg   = NULL;
    m_pkGuildLobbyDlg   = NULL;
	m_pkGuildListDlg	= NULL;
	m_pkGuildCreateDlg	= NULL;
	m_pkGuildJoinReqMbox	= NULL;
	m_pkGuildPublicMbox		= NULL;
	m_pkGuildSquareNotice	= NULL;
	m_pkGuildIntroduceEdit	= NULL;
	m_pkBingo               = NULL;
    m_pkQuiz                = NULL;
    m_pkItemInformationBox = NULL;
    m_pkMonsterCardBook = NULL;
    m_pkItemSocketManagementDlg = NULL;
	m_pkPuzzle				= NULL;
    m_pkInventoryFullMsg = NULL;
	m_pkHurryUPDlg = NULL;
	m_bEnableMessageBox = false;
    m_bExitMinigame = false;
#ifdef CALENDAR_RECOMMAND
	m_pkAttendanceInfoMsg   = NULL;
#endif
    m_pkEventCreateInfoMsg = NULL;
    m_vecOrgWnd.clear();
	m_pkCharacterSelectBox = NULL;
    m_pkCoordiChangeDlg =  NULL;
    m_pkSellCountItemDlg = NULL;
    m_pkCharGetInfoDlg = NULL;
    m_pkHeroDungeonDlg     = NULL;
    m_pkReplayRecordDlg = NULL;
    m_pkReplayRecordREC = NULL;
    m_pkReplayBoxNew    = NULL;
	m_pkGawibawiboGame =NULL;
	m_pkSocksTreeDlg = NULL;
    m_pkMissionUI_S4 = NULL;
    m_pkMissionEquipPreview_S4 = NULL;
    m_pkWeaponChangeBox = NULL;
	m_pkAttributeSelectBox =NULL;
    m_pkLevelupInfoBox = NULL;
    m_pkNicknameCampaignBox = NULL;
    m_pkCoordiCatalogDlg = NULL;
    m_pkPlantTree = NULL;
    m_pkPlantTreeRewardItemListView = NULL;
    m_pkChoiceDrop = NULL;
    m_pkPackagePreview = NULL;
    m_pkVipReward = NULL;
    m_pkMagicBox = NULL;
    m_pkRickoonTornado = NULL;
    m_pkUserMigrationDlg    = NULL;
    m_pkGuildShop = NULL;
    m_pkGuildLevelListDlg = NULL ; 
    m_pkDotaShop = NULL;
    m_pkPartyWnd = NULL;
    m_pkPartyDungeon = NULL ;
    m_pkPartyMatch = NULL;
    m_pkPartyInviteMsgBox = NULL;
    m_pkSphinxDlg = NULL;
    m_pkEventMinigameList = NULL;
    m_pkAgitOverlayUI = NULL;
    m_pkAgitShop = NULL;
    m_pkMantisReportDlg = NULL;
    m_pkShutDownDlg = NULL;
    m_pkSurveyDlg = NULL;
    m_pkGwcEventdlg = NULL;
    m_pkGwcRewardCatalogDlg = NULL;
    m_pkForgeDlg = NULL;
    m_pkNotifyMessage = NULL;
    m_pkPostLetterDlg = NULL;
	m_pkEclipseTime = NULL;
    m_pkChristmasGiftDlg = NULL;
    m_pkEventEclipsePlotDlg = NULL;
	m_pkDepot = NULL;
    m_pkEclipseOldBooks = NULL;
    m_pkWelcomeBackNewDlg = NULL;
    m_pkItemLevelDownDlg = NULL;
    m_pkHotTimeSearchDlg = NULL;
	m_pkMonsterCardCompose = NULL;
	m_pkNewNecklaceEnchantDlg = NULL;
    m_pkTransferMissionScrollGuideDlg = NULL;
	m_pkRoomListDlgS6 = NULL;
	m_pkRoomDungeonSelectDlgS6 = NULL;
	m_pkRoomCreateDlgS6 = NULL;
    m_pkWelcomeBackNewDlg = NULL;
    m_pkMonsterCardCompose = NULL;
    m_pkGcAdventureDlg = NULL;
	m_pkSongkranDlg = NULL;
	m_pkSongkranRewardDlg = NULL;
	m_pkEventAlarmDlg = NULL;	
	m_pkRitasChristmasDlg = NULL;
    m_pkPlaytimeNoticeUI = NULL;
    m_pkOlympicDlg = NULL;
    m_pkOlympicGawibawiboDlg = NULL;
    m_pkOlympicDicePlayDlg = NULL;
    m_pkOlympicSongkranDlg = NULL;
	m_pkChatBoxDlgS6 = NULL;
	m_pkSceneMenuBtnS6 = NULL;
    m_pk9thEventDlg = NULL;
    m_pkLevelItemDlg = NULL;
    m_pkItemCombinationDlg = NULL;
	m_pkQuickJoinDlg = NULL;	
    m_pkRoomUserInfoPopup = NULL;
	m_pkMatchMapSelectDlg = NULL;
	m_pkMatchInviteUserList = NULL;
	m_pkMatchCompleteDlg = NULL;
	m_pkAutoMatchLoading = NULL;
    m_pkItemCreationSelectBox = NULL;
    m_pkSuperGachaDlg = NULL;
    m_pkAttendanceStampDlg = NULL;
    m_pkBuffDonationDlg = NULL;
    m_pkItemBindDlg = NULL;
    m_pkAddItemInClient = NULL;
    m_pkBuyCashItemChooseCashTypeDlg = NULL;
    m_pkBuyCashItemAccumulatedGCDlg = NULL;
    m_pkBuyCashItemDlgS6 = NULL;
    m_pkBuyCashItemBuyCompleteDlg = NULL;
    m_pkChallengErranDlg = NULL;
    m_pkGCGuide = NULL;
    m_pkNewSkillMsg = NULL;
	m_pkFatigueInfoBox = NULL;
	m_pkFatigueMessageBox = NULL;
	m_pkFatigueMessageBox2 = NULL;
	m_pkChangeCash = NULL;
#if defined(NEW_MENU)
    m_pkShortMenuCutBox = NULL;
#endif
    m_pkEscMenuDlg = NULL;
	m_pkGiftBox = NULL;
    m_pkJumpingCharDlg = NULL;
    m_pkCreateRoomBoxS6 = NULL;
    m_pkChangeRoomTitleBoxS6 = NULL;
    m_pkSquareEnterWndS6 = NULL;
    m_pkReplayBoxS6 = NULL;
    m_pkCoordiComposeDlg = NULL;
    m_pkVirtualDepotDlg = NULL;
	m_pkSocialCommerceDlg = NULL;
    m_pkCoordiUpgradeDlg = NULL;
    m_pkHeroDungeonSeason5 = NULL;
    m_pkEventDungeonSeason5 = NULL;
    m_pkInfinityGuideDlg = NULL;	
    m_pkEnchantS6Dlg = NULL;
    m_pkEnchantSubS6Dlg = NULL;
    m_pkDungeonRankingDlg = NULL;
    m_pkBillBoardBox = NULL;
    m_pkBillBoardChat = NULL;
    m_pkHeroInfoS6 = NULL;
    m_pkHeroDungeonShopS6 = NULL;
    m_pkHeroRewardS6 = NULL;
    m_pkHeroUpgradeSidePanel = NULL;
    m_pkNicknameColorBox = NULL;
    m_pkGachaPongDlg = NULL;
    m_pkRoomOptionCharDlg = NULL;
    m_pkRoomOptionRankDlg = NULL;
    m_pkRoomListFilterDlg = NULL;
    m_pkCoordiViewDlg = NULL;
    m_pkCustomizationDlg = NULL;
    m_pkQuantityUseDlg = NULL;
    m_pkPetMagicStoneDlg = NULL;
    m_pkHeroTicketDlg = NULL;
    m_pkAllCollectionDlg = NULL;

    LINK_WINDOW("guild_shop_dlg",m_pkGuildShop);
    LINK_WINDOW( "sell_count_item_dlg",             m_pkSellCountItemDlg );
    LINK_WINDOW( "coordi_change_dlg",               m_pkCoordiChangeDlg );
	LINK_WINDOW( "buy_cashitem_box",                m_pkBuyCashBox );
	LINK_WINDOW( "pet_costume_inven",               m_pkPetCostumeInven );
    LINK_WINDOW( "server_scene",                    m_pkServerScene );
	LINK_WINDOW( "char_select_scene",           m_pkCharSelectScene );
	LINK_WINDOW( "char_gain_scene",					m_pkCharGainScene );
    LINK_WINDOW( "worldmap_scene",                  m_pkWorldMapScene );
    LINK_WINDOW( "match_scene_s6",                     m_pkMatchScene );
    LINK_WINDOW( "ongame_inventory",                m_pkGameInventory );
    LINK_WINDOW( "cash_shop_scene",                 m_pkCashShopScene );
    LINK_WINDOW( "gp_shop_scene",                   m_pkGPShopScene );
    LINK_WINDOW( "my_info_scene",                   m_pkMyInfoScene );
    LINK_WINDOW( "guild_scene",                     m_pkGuildScene );
    LINK_WINDOW( "room_scene_s6",                   m_pkRoomSceneS6 );
    LINK_WINDOW( "square_scene",                    m_pkSquareScene );
    LINK_WINDOW( "coordi_shop_scene",               m_pkCoordiShopScene );
    LINK_WINDOW( "message_box",                     m_pkMsgBox );
	LINK_WINDOW( "message_box2",                    m_pkMsgBox2 );
	LINK_WINDOW( "message_sizing_box",              m_pkMsgSizingBox );
    LINK_WINDOW( "password_box",                    m_pkPasswdBox );
    LINK_WINDOW( "item_receive_box2",               m_pkItemReceiveBox );
    LINK_WINDOW( "gem_buy_box",                         m_pkGemBuyBox );
    LINK_WINDOW( "gcquantitypurchasebox",            m_pkNewComboBox);
    LINK_WINDOW( "recommend_box",                    m_pkNickNameBox );
    LINK_WINDOW( "option_tab",                      m_pkOption );
    LINK_WINDOW( "login_scene",                     m_pkLoginScene);
    LINK_WINDOW( "register_scene",                  m_pkRegisterScene);
    LINK_WINDOW( "loading_box",                     m_pkLoadingBox );
    LINK_WINDOW( "pet_preview",                     m_pkPetPreview );
    LINK_WINDOW( "game_overlay_ui",                 m_pkGameOverlayUI );
    LINK_WINDOW( "square_overlayui",                m_pkSquareOverlayUI );
    LINK_WINDOW( "ongamedlg",                       m_pkOnGameDlg );
    LINK_WINDOW( "loading_scene",                   m_pkLoadingScene );
    LINK_WINDOW( "square_loading_scene",            m_pkSquareLoadingScene );
    LINK_WINDOW( "candidate_window",                m_pkCandidateWnd );
    LINK_WINDOW( "color_box",                       m_pkColorDlg );
    LINK_WINDOW( "buy_cashitem_window",             m_pBuyCashItemWnd );
    LINK_WINDOW( "gc_nateon",                       m_pkNateOn );
	LINK_WINDOW( "nateon_message",                  m_pkNateOnMessage );
	LINK_WINDOW( "nateon_inviate",                  m_pkNateOnInvite );
    LINK_WINDOW( "tool_tip",                        m_pkToolTip );
    LINK_WINDOW( "event_box",                       m_pkEventBox );
    LINK_WINDOW( "repetition_provide_event_box",    m_pkRepetitionEventBox );
    LINK_WINDOW( "pet_evolution_box",               m_pkPetEvolutionBox );
    LINK_WINDOW( "select_pet_name_box",             m_pkSelectPetNameBox );
    LINK_WINDOW( "attendance_calendar",             m_pkAttendanceCalendar );
    LINK_WINDOW( "perfect_attendance",              m_pkPerfectAttendance );
    LINK_WINDOW( "tutorial_select_box_new",         m_pkNewTutorialOption );
    LINK_WINDOW( "lucky_day_receive_box",           m_pkLuckyDayReceiveBox );
    LINK_WINDOW( "fairplay_report_box",             m_pkFairPlayReport );
    LINK_WINDOW( "grade_wnd",                       m_pkGradeBox );
    LINK_WINDOW( "channel_list_box",                m_pkChannelListBox );
    LINK_WINDOW( "background_white",                m_pkBackGroundWhite );
    LINK_WINDOW( "worldmap_container",              m_pkWorldMapContainer );
    LINK_WINDOW( "bonus_charge_box",                m_pkBonusChargeBox );
    LINK_WINDOW( "game_guide",						m_pkGuideNewbie );
    LINK_WINDOW( "game_guide_azit",						m_pkGuideAzit );
    LINK_WINDOW( "invite_request_box",              m_pkInviteRequestBox );
    LINK_WINDOW( "invite_msg_box",                  m_pkInviteBox );
    LINK_WINDOW( "treasure_item_box",               m_pkTreaureItemBox );
    LINK_WINDOW( "neonsign_select_box",             m_pkNeonsignSelectBox );
    LINK_WINDOW( "neonsign_preview_box",            m_pkNeonsignPreview );
    LINK_WINDOW( "emoticon_wnd",                   	m_pkEmoticonCollection );
    LINK_WINDOW( "title_wnd",                   	m_pkTitleCollection );
    LINK_WINDOW( "pet_wnd",                   	    m_pkPetCollection );    
    LINK_WINDOW( "condition_window",                m_pkCTConditionWindow );
    LINK_WINDOW( "ranking_list",					m_pkRanking );
    LINK_WINDOW( "connect_msg",                     m_pkConnectionMsg );
    LINK_WINDOW( "welcome_back",         m_pkWelcomeBackDlg );
    LINK_WINDOW( "ask_use_virtualcash",    m_pkAskUseVirtualCash );   
    LINK_WINDOW( "collection_item_info_box", m_pkCollectionItemInfoBox );
    LINK_WINDOW( "nateon_add_group_dlg",            m_pkNateOnInputDlg );
    LINK_WINDOW( "emoticon_select",                 m_pkEmoticonSelect );
    LINK_WINDOW( "nateon_add_friend_dlg",           m_pkNateOnAddFriend );
	LINK_WINDOW( "nateon_block_to_add_friend",      m_pkBlockToAddFriend );
    LINK_WINDOW( "minigame_select_box", m_pkMiniGameDlg );
	LINK_WINDOW( "SkilltreeDlg", m_pkSkillTree );
    LINK_WINDOW( "clear_seal_bead_new", m_pkNewClearSealDlg);
    LINK_WINDOW( "clear_seal_bead_coordi", m_pkCoordiClearSealDlg);
	LINK_WINDOW( "couple_viewer",	m_pkCoupleViewer );
	LINK_WINDOW( "couple_propose",	m_pkCouplePropose );
	LINK_WINDOW( "couple_accept",	m_pkCoupleAccept );
	LINK_WINDOW( "couple_wait",		m_pkCoupleWait );
	LINK_WINDOW( "couple_message",	m_pkCoupleMessage );
	LINK_WINDOW( "couple_goodbye",	m_pkCoupleGoodbye );
	LINK_WINDOW( "contribution_dlg", m_pkContributionDlg);
	LINK_WINDOW( "contribution_box", m_pkContributionBox);
	LINK_WINDOW( "happynewyear_dlg", m_pkHappyNewYearNPCDlg );
    LINK_WINDOW( "vip_gacha_box", m_pkVipGachaBox );
	LINK_WINDOW( "logout_box", m_pkLogoutBox);
	LINK_WINDOW( "rule_agreement", m_pkAgreement );
	LINK_WINDOW( "rule_agreement_1301", m_pkAgreement1301 );
	LINK_WINDOW( "gp_capsule_dlg", m_pkGPCapsuleDlg);		
	LINK_WINDOW( "obtain_item_wnd", m_pkGPCapsuleObtainWnd );
	LINK_WINDOW( "coupon_npc_dlg", m_pkCouponNPCDlg);		
    LINK_WINDOW( "recommend_list", m_pkRecommendList );
	LINK_WINDOW( "halloween_board", m_pkDicePlay );
	LINK_WINDOW( "subscription_dlg", m_pkSubscriptionDlg);		
	LINK_WINDOW( "subscription_box", m_pkSubscriptionBox);		
    LINK_WINDOW( "cooperation_event_dlg", m_pkCooperationEventDlg );
	LINK_WINDOW( "guildmark_regist_dlg", m_pkGuildMarkRegistDlg );
    LINK_WINDOW( "guild_dlg", m_pkGuildDlg );
    LINK_WINDOW( "guild_enter_dlg", m_pkGuildEnterDlg );
    LINK_WINDOW( "guild_lobby_dlg", m_pkGuildLobbyDlg );
	LINK_WINDOW( "guild_list_dlg", m_pkGuildListDlg );
    LINK_WINDOW( "guild_level_list",m_pkGuildLevelListDlg );
	LINK_WINDOW( "guild_create_dlg", m_pkGuildCreateDlg );
	LINK_WINDOW( "guild_join_req_mbox", m_pkGuildJoinReqMbox );
	LINK_WINDOW( "guild_public_mbox", m_pkGuildPublicMbox );
	LINK_WINDOW( "guild_square_enter_notice", m_pkGuildSquareNotice );
	LINK_WINDOW( "guild_my_introduce_edit", m_pkGuildIntroduceEdit);
    LINK_WINDOW( "item_information_box_pair", m_pkItemInformationBox );
    LINK_WINDOW( "monster_card_book", m_pkMonsterCardBook );
    LINK_WINDOW( "item_socket_management_dlg", m_pkItemSocketManagementDlg );
	LINK_WINDOW( "bingo_board", m_pkBingo );
    LINK_WINDOW( "quiz_board", m_pkQuiz );
	LINK_WINDOW( "puzzle_box", m_pkPuzzle );
    LINK_WINDOW( "inventory_full_msg", m_pkInventoryFullMsg );
#ifdef CALENDAR_RECOMMAND
    LINK_WINDOW( "attendance_info_msg", m_pkAttendanceInfoMsg );
#endif
    LINK_WINDOW( "event_create_info_msg", m_pkEventCreateInfoMsg );
    LINK_WINDOW( "gift_item_box", m_pkGiftItemBox );
	LINK_WINDOW( "character_select_box",   m_pkCharacterSelectBox );	
	LINK_WINDOW( "gamble_bo",   m_pkGambleBox );	
	LINK_WINDOW( "hurryup_dlg",   m_pkHurryUPDlg );	
    LINK_WINDOW( "character_get_info", m_pkCharGetInfoDlg );
    LINK_WINDOW( "hero_dungeon_dlg",   m_pkHeroDungeonDlg );	
    LINK_WINDOW( "replay_record_dlg",   m_pkReplayRecordDlg );
    LINK_WINDOW( "replay_rec",          m_pkReplayRecordREC );
    LINK_WINDOW( "replay_box_new",      m_pkReplayBoxNew );
	LINK_WINDOW( "gawibawibo_game_box",      m_pkGawibawiboGame );
	LINK_WINDOW( "socksTreeDlg",      m_pkSocksTreeDlg );
    LINK_WINDOW( "gc_mission_ui_s4",  m_pkMissionUI_S4 );
    LINK_WINDOW( "equippreview",      m_pkMissionEquipPreview_S4 );
	LINK_WINDOW( "weapon_change_box",      m_pkWeaponChangeBox );
	LINK_WINDOW( "item_attribute_select",      m_pkAttributeSelectBox );
    LINK_WINDOW( "levelup_info_box",        m_pkLevelupInfoBox );
    LINK_WINDOW( "nickname_campaign_box",   m_pkNicknameCampaignBox );
    LINK_WINDOW( "coordi_catalog_dlg",      m_pkCoordiCatalogDlg );
    LINK_WINDOW("event_plant_tree_box",m_pkPlantTree);
    LINK_WINDOW("reward_list_view_box",m_pkPlantTreeRewardItemListView);
    LINK_WINDOW("choice_drop",m_pkChoiceDrop);
    LINK_WINDOW("gc_package_preview",m_pkPackagePreview);
    LINK_WINDOW("magic_box",m_pkMagicBox);
    LINK_WINDOW("vip_reward",m_pkVipReward);
    LINK_WINDOW( "user_migration_dlg", m_pkUserMigrationDlg );
    LINK_WINDOW("event_rickoon_tornado",m_pkRickoonTornado);
    LINK_WINDOW( "dota_shop", m_pkDotaShop );
	LINK_WINDOW( "event_sphinx_dlg", m_pkSphinxDlg);
    LINK_WINDOW( "event_minigame_list_dlg",m_pkEventMinigameList);
#if defined (PARTY_SYSTEM)
    LINK_WINDOW( "party", m_pkPartyWnd);
    LINK_WINDOW( "party_dungeon_dlg",m_pkPartyDungeon );
    LINK_WINDOW( "party_match_dlg",m_pkPartyMatch );
    LINK_WINDOW( "party_invite_msg", m_pkPartyInviteMsgBox );
#endif
    LINK_WINDOW( "agit_overlay", m_pkAgitOverlayUI );
    LINK_WINDOW( "agit_shop", m_pkAgitShop );
    LINK_WINDOW( "mantis_report_dlg", m_pkMantisReportDlg);
    LINK_WINDOW( "new_forge_dlg",m_pkForgeDlg);
    LINK_WINDOW( "msgshutdown", m_pkShutDownDlg );
    LINK_WINDOW( "survey_dlg", m_pkSurveyDlg );
#if defined (GWC_EVENT_2011)
    LINK_WINDOW("gwc_2011" , m_pkGwcEventdlg);
    LINK_WINDOW("gwc_reward_catalog" , m_pkGwcRewardCatalogDlg );
#endif
#if defined (USE_CHRISTMAS_GIFT_EVENT)
    LINK_WINDOW( "event_christmas_gift_main", m_pkChristmasGiftDlg );
#endif
    LINK_WINDOW( "NotifyMessage", m_pkNotifyMessage );
    LINK_WINDOW( "post_letter", m_pkPostLetterDlg );
    LINK_WINDOW( "eclipse_time",m_pkEclipseTime);
    LINK_WINDOW( "event_eclipse_plot_main",m_pkEventEclipsePlotDlg);
    LINK_WINDOW( "eclipse_old_books", m_pkEclipseOldBooks );
    LINK_WINDOW( "welcome_back_new", m_pkWelcomeBackNewDlg );
    LINK_WINDOW( "item_level_down_dlg", m_pkItemLevelDownDlg );
    LINK_WINDOW( "hottime_search_dlg", m_pkHotTimeSearchDlg );
	LINK_WINDOW( "monster_card_compose_dlg", m_pkMonsterCardCompose );
	LINK_WINDOW( "necklace_enchant_dlg", m_pkNewNecklaceEnchantDlg );
	LINK_WINDOW( "depot_dlg", m_pkDepot);
    LINK_WINDOW( "transfer_mission_scroll_dlg", m_pkTransferMissionScrollGuideDlg );
	LINK_WINDOW( "songkran_dlg", m_pkSongkranDlg );
	LINK_WINDOW( "songkran_reward_dlg", m_pkSongkranRewardDlg );
	LINK_WINDOW( "event_alarm_dlg", m_pkEventAlarmDlg );
	LINK_WINDOW( "ritaschristmas_dlg", m_pkRitasChristmasDlg );
    LINK_WINDOW( "playtime_notice", m_pkPlaytimeNoticeUI );
    LINK_WINDOW( "olympic_dlg", m_pkOlympicDlg );
	LINK_WINDOW( "dungeon_room_list_dlg_s6", m_pkRoomListDlgS6);
	LINK_WINDOW( "room_dungeon_sel_dlg_s6", m_pkRoomDungeonSelectDlgS6);
	LINK_WINDOW( "room_create_dlg_s6",     m_pkRoomCreateDlgS6);
    LINK_WINDOW( "gawibawibo_box_olympic", m_pkOlympicGawibawiboDlg );
    LINK_WINDOW( "halloween_board_olympic", m_pkOlympicDicePlayDlg );
    LINK_WINDOW( "songkran_olympic_dlg", m_pkOlympicSongkranDlg );   
    LINK_WINDOW( "event_gc_adventure_dlg", m_pkGcAdventureDlg ); 
	m_pkLottery = NULL;
	LINK_WINDOW( "lottery_dlg", m_pkLottery );
	LINK_WINDOW( "chat_box_s6" , m_pkChatBoxDlgS6 );
	LINK_WINDOW( "scene_menu_btn_6", m_pkSceneMenuBtnS6);
    LINK_WINDOW( "gc9th_event_dlg", m_pk9thEventDlg );
    LINK_WINDOW( "level_item_dlg", m_pkLevelItemDlg );
    LINK_WINDOW( "gc_item_combination", m_pkItemCombinationDlg );
	LINK_WINDOW( "room_quick_join_s6" , m_pkQuickJoinDlg );	
    LINK_WINDOW( "user_info_popup", m_pkRoomUserInfoPopup );
	LINK_WINDOW( "auto_matching_map_s6", m_pkMatchMapSelectDlg);
    LINK_WINDOW( "item_creation_select_box", m_pkItemCreationSelectBox );
	LINK_WINDOW( "match_invite_firend_s6", m_pkMatchInviteUserList );
	LINK_WINDOW( "auto_matching_complete_s6", m_pkMatchCompleteDlg );
	LINK_WINDOW( "auto_match_loading", m_pkAutoMatchLoading );
    LINK_WINDOW( "attendance_stamp_main", m_pkAttendanceStampDlg );
    LINK_WINDOW( "buff_donation_dlg", m_pkBuffDonationDlg );
    LINK_WINDOW( "super_gacha_dlg", m_pkSuperGachaDlg );
    LINK_WINDOW( "item_bind_dlg", m_pkItemBindDlg );
    LINK_WINDOW( "additeminclient", m_pkAddItemInClient );
    LINK_WINDOW( "choose_cash_type", m_pkBuyCashItemChooseCashTypeDlg );
    LINK_WINDOW( "accumulated_gc", m_pkBuyCashItemAccumulatedGCDlg );
    LINK_WINDOW( "buy_cash_item_s6", m_pkBuyCashItemDlgS6 );
    LINK_WINDOW( "buy_complete", m_pkBuyCashItemBuyCompleteDlg );
    LINK_WINDOW( "challenge_erran_dlg", m_pkChallengErranDlg );
	LINK_WINDOW( "present_cashitem_wnd", m_pkPresentCashItemWnd );
	LINK_WINDOW( "present_friend_list", m_pkPresentFriendList );
#if defined ( USE_GUIDE_SYSTEM )
    LINK_WINDOW( "guide", m_pkGCGuide );
    LINK_WINDOW( "new_skill_msg", m_pkNewSkillMsg );
#endif
	LINK_WINDOW( "fatigue_info_box", m_pkFatigueInfoBox);
	LINK_WINDOW( "fatigue_message_box", m_pkFatigueMessageBox);
	LINK_WINDOW( "fatigue_message_box2", m_pkFatigueMessageBox2);
    LINK_WINDOW( "create_room_box_s6", m_pkCreateRoomBoxS6 );
    LINK_WINDOW( "change_room_title_box_s6", m_pkChangeRoomTitleBoxS6 );
    LINK_WINDOW( "square_enter_ui_s6", m_pkSquareEnterWndS6 );
    LINK_WINDOW( "replay_box_s6" , m_pkReplayBoxS6 );
	LINK_WINDOW( "change_cash_dlg", m_pkChangeCash);
    LINK_WINDOW( "esc_menu_dlg", m_pkEscMenuDlg );
	LINK_WINDOW("gift_box", m_pkGiftBox);
	LINK_WINDOW( "jumping_char_dlg", m_pkJumpingCharDlg );
    LINK_WINDOW( "coordi_compose_dlg", m_pkCoordiComposeDlg );	
    LINK_WINDOW( "virtual_depot_dlg", m_pkVirtualDepotDlg );
	LINK_WINDOW( "social_commerce_main_dlg", m_pkSocialCommerceDlg );
    LINK_WINDOW( "coordi_upgrade_dlg", m_pkCoordiUpgradeDlg );
    LINK_WINDOW( "herodungeon_season5", m_pkHeroDungeonSeason5 );
    LINK_WINDOW( "eventdungeon_season5", m_pkEventDungeonSeason5 );
    LINK_WINDOW( "infinity_guide_dlg", m_pkInfinityGuideDlg );
    LINK_WINDOW( "enchant_s6_dlg", m_pkEnchantS6Dlg );
    LINK_WINDOW( "enchant_sub_s6_dlg", m_pkEnchantSubS6Dlg );
    LINK_WINDOW( "dungeon_ranking_dlg", m_pkDungeonRankingDlg );
    LINK_WINDOW("shortmenu_dlg", m_pkShortMenuCutBox);
    LINK_WINDOW("billboard_chat_dlg", m_pkBillBoardBox);
    LINK_WINDOW("hero_dungeon_info_s6_dlg", m_pkHeroInfoS6);
    LINK_WINDOW("hero_dungeon_shop_s6_dlg", m_pkHeroDungeonShopS6);
    LINK_WINDOW("hero_dungeon_reward_s6_dlg", m_pkHeroRewardS6);
    LINK_WINDOW("hero_upgrade_side_panel", m_pkHeroUpgradeSidePanel);
    LINK_WINDOW("nickname_color_change_dlg", m_pkNicknameColorBox);
    LINK_WINDOW("gacha_pong_dlg", m_pkGachaPongDlg);
    LINK_WINDOW("room_option_char_dlg", m_pkRoomOptionCharDlg);
    LINK_WINDOW("room_option_rank_dlg", m_pkRoomOptionRankDlg);
    LINK_WINDOW("room_list_filter_dlg", m_pkRoomListFilterDlg);
    LINK_WINDOW("coordi_view_dlg", m_pkCoordiViewDlg);
    LINK_WINDOW("customization_dlg", m_pkCustomizationDlg);
    LINK_WINDOW("quantity_chest_use", m_pkQuantityUseDlg);
    LINK_WINDOW("pet_magic_stone_dlg", m_pkPetMagicStoneDlg);
    LINK_WINDOW("hero_ticket_use", m_pkHeroTicketDlg);
    LINK_WINDOW("all_collection_dlg", m_pkAllCollectionDlg);

	//중복해서 등장하면 안되는 UI 등록
	AddDoNotOverlapedWnd( GC_MBOX_MISSION_UI );
    AddDoNotOverlapedWnd( GC_MBOX_RANKING );
	AddDoNotOverlapedWnd( GC_MBOX_GUILDSHOP_DLG );
    AddDoNotOverlapedWnd( GC_MBOX_GUILD_LEVEL_LIST_DLG );
    AddDoNotOverlapedWnd( GC_MBOX_TOURNAMENT_BOX );
	AddDoNotOverlapedWnd( GC_MBOX_ITEM_COMBINATION_DLG );
	AddDoNotOverlapedWnd( GC_MBOX_OK_CANSEL_BOX );
	AddDoNotOverlapedWnd( GC_MBOX_JOIN_CANSEL_BOX );
	AddDoNotOverlapedWnd( GC_MBOX_INFO_CANSEL_BOX );

#if defined ( PARTY_SYSTEM )
    AddDoNotOverlapedWnd( GC_MBOX_PARTY_DUNGEON );
    AddDoNotOverlapedWnd( GC_MBOX_PARTY_MATCH );
    AddDoNotOverlapedWnd( GC_MBOX_PARTY_WINDOW );
    AddDoNotOverlapedWnd( GC_MBOX_PARTY_INVITE );
#endif
#if defined ( USE_AGIT_SYSTEM )
    AddDoNotOverlapedWnd( GC_MBOX_AGIT_SHOP );
#endif
#if defined( USE_ONGAME_INVENTORY )
    AddDoNotOverlapedWnd( GC_MBOX_GAME_INVENTORY );
#endif

#if defined( USE_SURVEY_SYSTEM )
    AddDoNotOverlapedWnd( GC_MBOX_SURVEY_DLG );
#endif

#if defined (GWC_EVENT_2011)
//    AddDoNotOverlapedWnd( GC_MBOX_GWC_EVENT_DLG );
#endif

#if defined( USE_POST_LETTER_SYSTEM )
    AddDoNotOverlapedWnd( GC_MBOX_POST_LETTER );
#endif
    AddDoNotOverlapedWnd(GC_MBOX_LOTTERY);

    AddDoNotOverlapedWnd( GC_MBOX_SUPER_GACHA );
	AddDoNotOverlapedWnd( GC_MBOX_FATIGUE_SYSTEM_MESSAGE );

    SetActivationManage( false );

    m_bToggleBackground = false;
    m_iBackgroundAlphaCounter = 0;
//    m_iDialogCount = 0;

    m_pkLastColorItemUse = NULL;


    // 루아에서 메시지 박스를 띄우는 등의 행위를 할 수 있도록 해 주기 위한 Binding
    lua_tinker::class_add<KGCUIScene>( KGCLuabinder::getInstance().GetLuaState(), "GCUISceneClass" );
    //	lua_tinker::class_mem<KGCUIScene>( KGCLuabinder::getInstance().GetLuaState(), "iCurrTexIndex", 	&GCSquare::m_iCurrTexIndex );
    lua_tinker::class_def<KGCUIScene>( KGCLuabinder::getInstance().GetLuaState(), "MessageBox", 			&KGCUIScene::LuaMessageBox );
    lua_tinker::class_def<KGCUIScene>( KGCLuabinder::getInstance().GetLuaState(), "SendGiftsReq",				&KGCUIScene::LuaSendGiftsReq );

    lua_tinker::decl( KGCLuabinder::getInstance().GetLuaState(), "GCUIScene", this );

	m_iLastCollectionWindow = -1;
    m_bFirstTutorialPlaying = false;
}

KGCUIScene::~KGCUIScene( void )
{
    g_pkUIScene = NULL;
}

void KGCUIScene::OnEnter( void )
{
    m_pkServerScene->ToggleRender( false );
	m_pkCharSelectScene->ToggleRender( false );
	m_pkCharGainScene->ToggleRender( false );
    m_pkWorldMapScene->ToggleRender( false );
    m_pkMatchScene->ToggleRender( false );

    if (m_pkGuildShop)
        m_pkGuildShop->InitState(false,true,this);


    
    // WONYWONY
    //m_pkChannelScene->SetSelfInputCheck( true );
    //m_pkChannelScene->ToggleRender(false);
    if (m_pkCashShopScene)
        m_pkCashShopScene->ToggleRender(false);
    if ( m_pkGPShopScene )
        m_pkGPShopScene->ToggleRender(false);
    if (m_pkMyInfoScene)
        m_pkMyInfoScene->ToggleRender(false);
    if (m_pkGuildScene)
        m_pkGuildScene->ToggleRender(false);
    //m_pkLobbyScene->ToggleRender(false);
        
    m_pkRoomSceneS6->InitState( false );
    
    
    m_pkCollectionItemInfoBox->ToggleRender(false);
    m_pkLoadingScene->ToggleRender(false);
    m_pkCandidateWnd->ToggleRender(false);
	m_pkNickNameBox->ToggleRender(false);

	m_pkLogoutBox->ToggleRender(false);

	m_pkMsgBox->SetSelfInputCheck( false );	
	m_pkMsgBox2->SetSelfInputCheck( false );	
	m_pkMsgSizingBox->SetSelfInputCheck(false);

    m_pkMsgBox->SetSelfInputCheck( false );
    m_pkPasswdBox->SetSelfInputCheck( false );
    m_pkLoadingBox->SetSelfInputCheck( false );
    m_pkCreateRoomBoxS6->SetSelfInputCheck( false );
    m_pkChangeRoomTitleBoxS6->SetSelfInputCheck( false );
    //m_pkInviteReplayBox->SetSelfInputCheck( false );
    if (m_pkItemReceiveBox)
        m_pkItemReceiveBox->SetSelfInputCheck( true );
    m_pkGemBuyBox->SetSelfInputCheck( false );
    m_pkNickNameBox->SetSelfInputCheck( false );
    m_pkOption->SetSelfInputCheck( false );
    m_pkLoadingScene->SetSelfInputCheck( false );
    m_pkCandidateWnd->SetSelfInputCheck( false );	

    m_pkPetPreview->InitState( false, true, this );
    m_pkColorDlg->InitState(false,false,this);	

    m_pkMsgBox->AddActionListener( this );
	m_pkMsgBox2->AddActionListener( this );
	m_pkMsgSizingBox->AddActionListener(this);
    m_pkPasswdBox->AddActionListener( this );
    m_pkLoadingBox->AddActionListener( this );
    m_pkCreateRoomBoxS6->AddActionListener( this );
    m_pkChangeRoomTitleBoxS6->AddActionListener( this );
    //m_pkInviteReplayBox->AddActionListener( this );
    if (m_pkItemReceiveBox)
        m_pkItemReceiveBox->AddActionListener( this );
    if (m_pkGemBuyBox)
        m_pkGemBuyBox->AddActionListener( this );
    if (m_pkNickNameBox)
        m_pkNickNameBox->AddActionListener( this );
    if (m_pkOption)
        m_pkOption->AddActionListener( this );	

    m_pkGameOverlayUI->SetSelfInputCheck( false );
    m_pkSquareOverlayUI->SetSelfInputCheck(false);
    m_pkSquareScene->SetSelfInputCheck(false);

    m_pkOnGameDlg->SetSelfInputCheck( false );

    m_pkRoomSceneS6->SetFontSize( SiKGCMultipleLanguages()->GetLargeFontSize() );
    m_pkRoomSceneS6->SetFontColor( 0xff000000 );  

    m_pBuyCashItemWnd->InitState( false, true, this );
    SetCurMsgBox( m_pkMsgBox );
    m_pkNateOn->InitState( false, true, this );
	m_pkNateOnMessage->InitState( false, true, this );
	m_pkNateOnInvite->InitState( false, true, this );
    m_pkToolTip->InitState( false );
    m_pkEventBox->InitState( false, false, this );
    m_pkTreaureItemBox->InitState( false, false, this );
    m_pkRepetitionEventBox->InitState( false, true, this );
    m_pkPetEvolutionBox->InitState( false, true, this );
    m_pkSelectPetNameBox->InitState( false, true, this );
    m_pkAttendanceCalendar->InitState( false, true, this );
    m_pkPerfectAttendance->InitState( false, true, this );
    m_pkNewTutorialOption->InitState( false, true, this );
    m_pkLuckyDayReceiveBox->InitState( false, true, this );
    m_pkFairPlayReport->InitState( false, true, this );
    m_pkGradeBox->InitState( false, true, this );
    //m_pkDungeonRoomlist->InitState( false, true, this );
    //m_pkDungeonRoomlist->InitRoomList( KGCRoomList::RLT_DUNGEON );

    m_pkChannelListBox->InitState( false, true, this );
    m_pkWorldMapContainer->InitState( false );

    m_pkBackGroundWhite->InitState( false );
    m_pkBackGroundWhite->ForceSetWindowColor( D3DCOLOR_ARGB( BACKGROUND_ALPHA, 0x00, 0x00, 0x00 ) );

    m_pkBonusChargeBox->InitState( false, false, this );

    m_pkGuideNewbie->InitState( false, true, this );
    if (m_pkGuideAzit) {
        m_pkGuideAzit->InitState( false, true, this );
    }
    m_pkSquareLoadingScene->InitState( false );

    if (m_pkBillBoardBox) {
        m_pkBillBoardBox->InitState(false, true, this);
    }

    if (m_pkNicknameColorBox) {
        m_pkNicknameColorBox->InitState(false, true, this);
    }

    m_pkNewComboBox->InitState(false, true, this);

    if (m_pkInviteRequestBox)
        m_pkInviteRequestBox->InitState( false, false, this );
    if (m_pkInviteBox)
        m_pkInviteBox->InitState( false, false, this );

    m_pkNeonsignSelectBox->InitState( false, false, this );
    m_pkNeonsignPreview->InitState( false, false, this );

    m_pkEmoticonCollection->InitState( false, false, this );
    m_pkTitleCollection->InitState( false, false, this );
    m_pkPetCollection->InitState( false, false, this );
    m_pkCTConditionWindow->InitState( false, false, this );

    m_pkRanking->InitState( false, true, this );
    m_pkConnectionMsg->InitState( true, false, this );

    m_pkWelcomeBackDlg->InitState( false, true, this );
    m_pkAskUseVirtualCash->InitState( false, true, this );
    m_pkSelectPetNameBox->InitState( false, true, this );

    m_pkMiniGameDlg->InitState( false, true, this );
    m_pkNateOnInputDlg->InitState( false, true, this );
    m_pkEmoticonSelect->InitState( false, true, this );
    m_pkNateOnAddFriend->InitState( false, true, this );
	m_pkBlockToAddFriend->InitState( false, true, this );

    if (m_pkSkillTree)
	    m_pkSkillTree->InitState( false, true, this );
    if (m_pkNewClearSealDlg)
        m_pkNewClearSealDlg->InitState(false, true, this );
    if (m_pkCoordiClearSealDlg)
        m_pkCoordiClearSealDlg->InitState(false, true, this );

    if (m_pkCoupleViewer)
	    m_pkCoupleViewer->InitState( false, true, this );
    if (m_pkCoupleAccept)
	    m_pkCoupleAccept->InitState( false, true, this );
    if (m_pkCouplePropose)
	    m_pkCouplePropose->InitState( false, true, this );
    if (m_pkCoupleWait)
	    m_pkCoupleWait->InitState( false, true, this );
    if (m_pkCoupleMessage)
	    m_pkCoupleMessage->InitState( false, true, this );
    if (m_pkCoupleGoodbye)
	    m_pkCoupleGoodbye->InitState( false, true, this );

	//for macro : wnd initialize
    m_pkSellCountItemDlg->InitState( false, true, this );
    m_pkCoordiChangeDlg->InitState( false, true, this );
	m_pkBuyCashBox->InitState( false, true, this );
	m_pkPetCostumeInven->InitState( false, true, this );
	
	if ( m_pkGameInventory )
	    m_pkGameInventory->InitState( false, true, this );

	m_pkContributionDlg->InitState(false, true, this );
	m_pkContributionBox->InitState(false, true, this );
	m_pkHappyNewYearNPCDlg->InitState( false, true, this );


    m_pkVipGachaBox->InitState( false, true, this );
	m_pkLogoutBox->InitState(false, true, this );

	//m_pkValentineDayDlg->InitState( false, true, this );

	m_pkGPCapsuleDlg->InitState( false, true, this );	
	m_pkGPCapsuleObtainWnd->InitState(false,true, this );

	m_pkCouponNPCDlg->InitState(false, true, this);

	m_pkSubscriptionBox->InitState( false, true, this );
	m_pkSubscriptionDlg->InitState( false, true, this );

	if( m_pkAgreement != NULL )
		m_pkAgreement->InitState( false, true, this );

	if( m_pkAgreement1301 != NULL )
		m_pkAgreement1301->InitState( false, true, this );

    m_pkRecommendList->InitState( false, true, this );

	m_pkDicePlay->InitState( false, true, this );


	m_pkCooperationEventDlg->InitState( false, true, this );

	m_pkGuildMarkRegistDlg->InitState( false, true, this );
    m_pkGuildDlg->InitState( false, true, this );
    m_pkGuildLobbyDlg->InitState( false, true, this );
    m_pkGuildEnterDlg->InitState( false , true, this );
	m_pkGuildListDlg->InitState( false , true, this );
	m_pkGuildCreateDlg->InitState( false , true, this );
	m_pkGuildJoinReqMbox->InitState( false , true, this );
	m_pkGuildPublicMbox->InitState( false , true, this );
	m_pkGuildSquareNotice->InitState(false, true, this );
	m_pkGuildIntroduceEdit->InitState(false, true, this);
    if ( m_pkGuildLevelListDlg )
        m_pkGuildLevelListDlg->InitState( false, true, this );

	m_pkBingo->InitState(false, true, this);
    m_pkQuiz->InitState(false, true, this);
	m_pkGawibawiboGame->InitState(false,true,this);
	//m_pkGWCDonationDlg->InitState( false, true, this);
	//m_pkGWCShowStateDlg->InitState( false, true, this);

    m_pkItemInformationBox->ToggleRender( false );
    m_pkMonsterCardBook->InitState(false, true, this );
    m_pkItemSocketManagementDlg->InitState( false , true ,this );
        
    m_pkPuzzle->InitState( false, true, this);
    m_pkInventoryFullMsg->ToggleRender( false );

	m_pkHurryUPDlg->InitState(false,true,NULL);
	m_pkCharacterSelectBox->InitState(false,true,this);
    m_pkCharGetInfoDlg->InitState(false, true, this);

    if( m_pkReplayRecordDlg ){
        m_pkReplayRecordDlg->InitState( false, true, this);
        m_pkReplayRecordREC->InitState( false, false, this );
		if (m_pkReplayBoxNew)
			m_pkReplayBoxNew->InitState( false, true, this);
    }

    //Loja Heroica EP2
    if (m_pkHeroDungeonShopS6) {
        m_pkHeroDungeonShopS6->InitState(false, true, this);
    }

    //Loja Heroica EP2
    if (m_pkHeroInfoS6) {
        m_pkHeroInfoS6->InitState(false, true, this);
    }

    if (m_pkHeroRewardS6) {
        m_pkHeroRewardS6->InitState(false, true, this);
    }

    if( m_pkHeroDungeonDlg ){
        m_pkHeroDungeonDlg->InitState( false, true, this );
    }

    if( m_pkSocksTreeDlg ){
		m_pkSocksTreeDlg->InitState( false, true, this );
        m_pkSocksTreeDlg->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
	}

    if(m_pkPlantTree)
    {
        m_pkPlantTree->InitState(false,true,this);
    }

    if(m_pkPlantTreeRewardItemListView)
    {
        m_pkPlantTreeRewardItemListView->InitState(false,true,this);
    }

    if( m_pkUserMigrationDlg )
    {
        m_pkUserMigrationDlg->InitState( false, true, this );
    }


    if (m_pkRickoonTornado)
    {
        m_pkRickoonTornado->InitState(false,true,this);
    }

    if( m_pkSphinxDlg )
    {
        m_pkSphinxDlg->InitState(false,true,this);
    }

    if( m_pkEventMinigameList )
    {
        m_pkEventMinigameList->InitState(false,true,this);
    }

    if(m_pkForgeDlg)
    {
        m_pkForgeDlg->InitState(false,true,this);
    }

    if( m_pkPostLetterDlg ) {
        m_pkPostLetterDlg->InitState( false, true, this );
    }
	if( m_pkDepot )
	{
		m_pkDepot->InitState(false,true,this);
	}
    if( m_pkTransferMissionScrollGuideDlg ) {
        m_pkTransferMissionScrollGuideDlg->InitState( false, true, this );
    }
	
    if (m_pkGcAdventureDlg )
    {
        m_pkGcAdventureDlg->InitState( false, true, this);
    }
	if( m_pkRoomListDlgS6 ) {
		m_pkRoomListDlgS6->InitState(false, true, this);
		m_pkRoomDungeonSelectDlgS6->InitState(false, true, this);
		m_pkRoomCreateDlgS6->InitState(false,true,this);
	}

    if( m_pk9thEventDlg ) { 
        m_pk9thEventDlg->InitState( false, true, this );
    }

    if( m_pkAttendanceStampDlg ) { 
        m_pkAttendanceStampDlg->InitState( false, true, this );
    }

    if( m_pkBuffDonationDlg ) { 
        m_pkBuffDonationDlg->InitState( false, true, this );
    }

    if( m_pkItemBindDlg ) {
        m_pkItemBindDlg->InitState( false, true, this );
    }
    if( m_pkBuyCashItemChooseCashTypeDlg ){
        m_pkBuyCashItemChooseCashTypeDlg->InitState( false, true, this );
    }

    if( m_pkBuyCashItemAccumulatedGCDlg ){
        m_pkBuyCashItemAccumulatedGCDlg->InitState( false, true, this );
    }

    if( m_pkBuyCashItemDlgS6 ){
        m_pkBuyCashItemDlgS6->InitState( false, true, this );
    }

    if( m_pkBuyCashItemBuyCompleteDlg ){
        m_pkBuyCashItemBuyCompleteDlg->InitState( false, true, this );
    }

    if( m_pkAddItemInClient ){
        m_pkAddItemInClient->InitState( false, true, this );
    }

    if( m_pkLevelItemDlg ) { 
        m_pkLevelItemDlg->InitState( false, true, this );
    }


#if !defined( NATION_IDN )
#ifdef CALENDAR_RECOMMAND
    m_pkAttendanceInfoMsg->ToggleRender( false );
#endif
    if (m_pkEventCreateInfoMsg)
        m_pkEventCreateInfoMsg->ToggleRender(false);

    m_pkGiftItemBox->InitState( false, true, this );
#endif


    if( m_pkMissionUI_S4 != NULL ) {
        m_pkMissionUI_S4->InitState( false, true, this );
    }
    if( m_pkMissionEquipPreview_S4 != NULL ) {
        m_pkMissionEquipPreview_S4->InitState( false, true, this );
    }
    if( m_pkWeaponChangeBox != NULL ){
        m_pkWeaponChangeBox->InitState( false, true, this );
    }

	if( m_pkAttributeSelectBox != NULL)
	{
		m_pkAttributeSelectBox->InitState(false,true,this);
	}

    if( m_pkLevelupInfoBox != NULL )
        m_pkLevelupInfoBox->InitState( false, true, this );

    if( m_pkNicknameCampaignBox != NULL )
        m_pkNicknameCampaignBox->InitState( false, true, this );

    if ( m_pkChoiceDrop != NULL ) {
        m_pkChoiceDrop->InitState( false, true, this );
    }

    if( m_pkCoordiShopScene )
        m_pkCoordiShopScene->InitState( false );

    if( m_pkCoordiCatalogDlg )
        m_pkCoordiCatalogDlg->InitState( false, true, this );

    if( m_pkPackagePreview != NULL )
    {
        m_pkPackagePreview->InitState( false, true, this );
    }

    if ( m_pkVipReward != NULL )
        m_pkVipReward->InitState(false, true, this );
 
    
    if ( m_pkMagicBox != NULL )
        m_pkMagicBox->InitState(false, true, this );

    if ( m_pkDotaShop != NULL )
        m_pkDotaShop->InitState(false, true, this );
#if defined (PARTY_SYSTEM)
    if ( m_pkPartyWnd != NULL)
        m_pkPartyWnd->InitState( false, true, this );
    if ( m_pkPartyDungeon!= NULL)
        m_pkPartyDungeon->InitState( false, true, this );
    if ( m_pkPartyMatch != NULL)
        m_pkPartyMatch->InitState( false, true, this );
    if ( m_pkPartyInviteMsgBox != NULL)
        m_pkPartyInviteMsgBox->InitState( false, true, this );
#endif

#if defined( USE_AGIT_SYSTEM )
    if( m_pkAgitOverlayUI != NULL )
        m_pkAgitOverlayUI->InitState( false, true, this );

    if( m_pkAgitShop != NULL )
        m_pkAgitShop->InitState( false, true, this );
#else
    if( m_pkAgitOverlayUI != NULL ) { 
        m_pkAgitOverlayUI->InitState( false, false, NULL );
    }

    if( m_pkAgitShop != NULL ) { 
        m_pkAgitShop->InitState( false, false, NULL );
    }
#endif

    if( m_pkMantisReportDlg != NULL ) {
        m_pkMantisReportDlg->InitState( false, true, this );
    }

    if( m_pkSurveyDlg != NULL ) {
        m_pkSurveyDlg->InitState( false, true, this );
    }

    if ( m_pkShutDownDlg != NULL ) 
    {
        m_pkShutDownDlg->InitState( false, true, this );
    }
#if defined (GWC_EVENT_2011)
    if ( m_pkGwcEventdlg != NULL )
    {
        m_pkGwcEventdlg->InitState(false, true, this );
    }

    if (m_pkGwcRewardCatalogDlg != NULL)
    {
        m_pkGwcRewardCatalogDlg->InitState( false, true, this);
    }
#endif

#if defined (USE_CHRISTMAS_GIFT_EVENT)
    if ( m_pkChristmasGiftDlg != NULL ){
        m_pkChristmasGiftDlg->InitState( false, true, this );
    }
#endif

    if( m_pkEclipseTime != NULL) {
        m_pkEclipseTime->InitState( false, true, this );
    }
    if ( m_pkNotifyMessage )
    {
        m_pkNotifyMessage->InitState( true, true, this );
    }
	
	if ( m_pkTransferMissionScrollGuideDlg != NULL ){
        m_pkTransferMissionScrollGuideDlg->InitState( false, true, this );
    }


    if( m_pkEventEclipsePlotDlg != NULL ) {
        m_pkEventEclipsePlotDlg->InitState( false, true, this );
    }

    if( m_pkEclipseOldBooks != NULL ) {
        m_pkEclipseOldBooks->InitState( false, true, this );
    }
    if( m_pkWelcomeBackNewDlg ) { 
        m_pkWelcomeBackNewDlg->InitState( false, true, this );
    }

    if ( m_pkItemLevelDownDlg ) 
    {
        m_pkItemLevelDownDlg->InitState( false, true, this );
    }

    if ( m_pkHotTimeSearchDlg ) 
    {
        m_pkHotTimeSearchDlg->InitState( false, true, this );
    }

	if ( m_pkMonsterCardCompose ) 
	{
		m_pkMonsterCardCompose->InitState( false, true, this );
	}

	if ( m_pkNewNecklaceEnchantDlg ) 
	{
		m_pkNewNecklaceEnchantDlg->InitState( false, true, this );
	}

	if ( m_pkSongkranDlg ) 
	{
		m_pkSongkranDlg->InitState( false, true, this );
	}

	if (m_pkRitasChristmasDlg)
	{
		m_pkRitasChristmasDlg->InitState(false,true,this);
	}
	
	if ( m_pkSongkranRewardDlg ) 
	{
		m_pkSongkranRewardDlg->InitState( false , true, this );
	}	

	if ( m_pkEventAlarmDlg ) 
	{
		m_pkEventAlarmDlg->InitState( false, true, this );
	}

    if( m_pkPlaytimeNoticeUI ) { 
        m_pkPlaytimeNoticeUI->InitState( false );
    }
	

    if( m_pkOlympicDlg ) { 
        m_pkOlympicDlg->InitState( false, true, this );
    }
    
    if( m_pkOlympicGawibawiboDlg ) { 
        m_pkOlympicGawibawiboDlg->InitState( false, true, this );
    }

    if( m_pkOlympicDicePlayDlg ) { 
        m_pkOlympicDicePlayDlg->InitState( false, true, this );
    }

    if( m_pkOlympicSongkranDlg ) { 
        m_pkOlympicSongkranDlg->InitState( false, true, this );
    }
	
	if ( m_pkChatBoxDlgS6 )
	{
		m_pkChatBoxDlgS6->InitState( false, true, this );
	}

	if ( m_pkSceneMenuBtnS6 )
	{
		m_pkSceneMenuBtnS6->InitState( false, true, this );
	}

    if (m_pkItemCombinationDlg )
    {
        m_pkItemCombinationDlg->InitState( false, true, this );
    }
	
	if (m_pkLottery)
	{
		m_pkLottery->InitState(false,true,this);
	}	
	
	if ( m_pkQuickJoinDlg ) 
	{
		m_pkQuickJoinDlg->InitState( false, true, this );
	}	
    if ( m_pkRoomUserInfoPopup )
    {
        m_pkRoomUserInfoPopup->InitState( false, true, this );
    }
    if( m_pkItemCreationSelectBox ) { 
        m_pkItemCreationSelectBox->InitState( false, true, this );
    }


	if ( m_pkMatchMapSelectDlg )
	{
		m_pkMatchMapSelectDlg->InitState(false,true, this);
	}

	if ( m_pkMatchInviteUserList ) {
		m_pkMatchInviteUserList->InitState(false, true, this);
	}

	if ( m_pkMatchCompleteDlg ) {
		m_pkMatchCompleteDlg->InitState( false, true, this );
	}

	if ( m_pkAutoMatchLoading ) {
		m_pkAutoMatchLoading->InitState( false, true, this );

	}

    if( m_pkSuperGachaDlg ) { 
        m_pkSuperGachaDlg->InitState( false, true, this );
    }

    if( m_pkChallengErranDlg ) {
        m_pkChallengErranDlg->InitState( false, true, this );
    }

    if( m_pkGCGuide ) {
        m_pkGCGuide->InitState( false, true, this );
    }

    if( m_pkNewSkillMsg ){
        m_pkNewSkillMsg->InitState( false );
    }
	
	if ( m_pkFatigueInfoBox ) {
		m_pkFatigueInfoBox->InitState(false,true,this);
	}

	if ( m_pkFatigueMessageBox ) {
		m_pkFatigueMessageBox->InitState(false,true,this);
	}

	if ( m_pkFatigueMessageBox2 ) {
		m_pkFatigueMessageBox2->InitState(false,true,this);
	}

	if ( m_pkChangeCash ) {
		m_pkChangeCash->InitState(false,true,this);
	}

#if defined(NEW_MENU)
    if (m_pkShortMenuCutBox) {
        m_pkShortMenuCutBox->InitState(false, true, this);
    }
#endif
    if (m_pkEscMenuDlg)
    {
        m_pkEscMenuDlg->InitState(false, true, this);
    }
	
	m_pkGiftBox->InitState( false , true, this );
	
	if ( m_pkJumpingCharDlg )
    {
        m_pkJumpingCharDlg->InitState( false, true, this );
    }    
    m_pkSquareEnterWndS6->InitState( false, false, this );
    
    if (m_pkReplayBoxS6)
        m_pkReplayBoxS6->SetSelfInputCheck( false );
    if (m_pkReplayBoxS6)
        m_pkReplayBoxS6->AddActionListener( this );

    if ( m_pkCoordiComposeDlg )
        m_pkCoordiComposeDlg->InitState( false, true, this );

    if( m_pkVirtualDepotDlg)
        m_pkVirtualDepotDlg->InitState( false, true, this );

    if( m_pkSocialCommerceDlg )
        m_pkSocialCommerceDlg->InitState( false, true, this );
    if ( m_pkCoordiUpgradeDlg )
        m_pkCoordiUpgradeDlg->InitState( false, true, this );
	 if (m_pkHeroDungeonSeason5 )
        m_pkHeroDungeonSeason5->InitState( false, true, this );
     if (m_pkEventDungeonSeason5 )
         m_pkEventDungeonSeason5 ->InitState( false, true, this );
		 
	if( m_pkPresentCashItemWnd ){
		m_pkPresentCashItemWnd->InitState( false, true, this );
	}

	if( m_pkPresentFriendList ){
		m_pkPresentFriendList->InitState( false, true, this );
	}
		 
     if ( m_pkInfinityGuideDlg )
         m_pkInfinityGuideDlg->InitState( false, true, this );


     if ( m_pkEnchantS6Dlg )
         m_pkEnchantS6Dlg->InitState( false, true, this );

     if ( m_pkEnchantSubS6Dlg )
         m_pkEnchantSubS6Dlg->InitState( false, true, this );

     if ( m_pkDungeonRankingDlg )
         m_pkDungeonRankingDlg->InitState( false, true, this );

     if (m_pkGachaPongDlg)
         m_pkGachaPongDlg->InitState(false, true, this);

     if (m_pkRoomOptionCharDlg)
         m_pkRoomOptionCharDlg->InitState(false, true, this);

     if (m_pkRoomOptionRankDlg)
         m_pkRoomOptionRankDlg->InitState(false, true, this);

     if (m_pkRoomListFilterDlg)
         m_pkRoomListFilterDlg->InitState(false, true, this);

     if (m_pkCoordiViewDlg)
         m_pkCoordiViewDlg->InitState(false, true, this);

     if (m_pkCustomizationDlg)
         m_pkCustomizationDlg->InitState(false, true, this);

     if (m_pkQuantityUseDlg)
         m_pkQuantityUseDlg->InitState(false, true, this);

     if (m_pkPetMagicStoneDlg)
         m_pkPetMagicStoneDlg->InitState(false, true, this);

     if (m_pkHeroTicketDlg)
         m_pkHeroTicketDlg->InitState(false, true, this);

     if (m_pkAllCollectionDlg)
         m_pkAllCollectionDlg->InitState(false, true, this);
}

void KGCUIScene::UseColorItem( KGCColorItemUse* pTargetWnd )
{
    if (!SiKGCChannelManager()->IsPVPBallanceServer() &&
        (g_kGlobalValue.m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCCLUB || g_kGlobalValue.m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCBLOG || g_kGlobalValue.m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCPREMIUM || g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN)
        )
        return;

    KItem* Info = NULL;
    if( pTargetWnd->IsUseColorItem() )
    {
        bool bFind = false;
        if( pTargetWnd->IsUseColorItemCash() )
        {
            if( g_pItemMgr->FindInventoryForItemID(KGCItemManager::ITEM_ROOM_NAME_COLORING_CASH, &Info ) )
            {
                bFind = g_pItemMgr->DecDurationItem( Info->m_ItemUID );
            }
        }
        else
        {
            if( g_pItemMgr->FindInventoryForItemID(KGCItemManager::ITEM_ROOM_NAME_COLORING_GP, &Info ) )
            {
                bFind = g_pItemMgr->DecDurationItem( Info->m_ItemUID );
            }
            // GP템이 없으면 캐쉬템 깎아야함
            else if( g_pItemMgr->FindInventoryForItemID(KGCItemManager::ITEM_ROOM_NAME_COLORING_CASH, &Info ) )
            {
                bFind = g_pItemMgr->DecDurationItem( Info->m_ItemUID );
            }
        }

        if( bFind )
        {
            //요놈은 나쁜놈이다..아주아주 나쁜x
        }
    }
}

void KGCUIScene::OnClear( void )
{
}

void KGCUIScene::FrameMoveInEnabledState( void )
{
    PROFILE_SET("KGCUIScene::FrameMoveInEnabledState");

    // 2008.03.20 milennium9
    // 운영자는 어디서든 옵저버 옵션을 조절할 수 있음.
    if( g_kGlobalValue.IsAdmin() )
    {
        if( diks[DIK_LCONTROL] && diks[DIK_O] && !Old_diks[DIK_O] ) 
        {
            KP2P::GetInstance()->Send_SetObserver( !g_kGlobalValue.IsObserverMode() );
        }
    }

    // 2007.04.13 wony
    // 메세지 박스가 나올 때 뒤에 약간 어둡게 해주는거 사라질 때 알파값 슬며시 빠지도록 합니다
    if( !m_bToggleBackground && m_iBackgroundAlphaCounter > 0 )
    {
        g_pkInput->IgnoreDown(DIK_ESCAPE,30);   //메세지박스 뜰때 esc키 눌러서 백그라운드 활성화 하는 문제 있어서 코드 추가합니다.
        m_iBackgroundAlphaCounter--;

        BYTE a = static_cast<BYTE>( static_cast<float>( BACKGROUND_ALPHA ) * static_cast<float>( m_iBackgroundAlphaCounter ) / static_cast<float>( TOGGLEBACKGROUND_FRAME ) );

        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MINIGAME )
            m_pkBackGroundWhite->ForceSetWindowColor( D3DCOLOR_ARGB( 0x00, 0x00, 0x00, 0x00 ) );
        else
            m_pkBackGroundWhite->ForceSetWindowColor( D3DCOLOR_ARGB( a, 0x00, 0x00, 0x00 ) );

        if( m_iBackgroundAlphaCounter == 0 )
            m_pkBackGroundWhite->ToggleRender( false );
    }

    // 2007.11.13 wony
    m_pkToolTip->FrameMove();

    // 2008.01.22 milennium9
    g_kGlobalValue.m_kUserMission.ShowAutoMission();

    // 방에서 스타트 딜레이 관리..
    // RoomButtonPanel에서 해줬었는데..
    // 메세지 박스 등이 떠있어서.. Enable상태가 아닐때
    // 시간이 안흘러서 여기서 관리를 해주도록 합니다
    if( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
    {
        m_pkRoomSceneS6->m_pkRoomButtonPanel->DecreaseStartDelay();
    }

    for( UINT iMsg = 0; iMsg < g_kGlobalValue.m_vecDelayMsg.size(); iMsg++ )
    {	
        KMsgBoxInfo kMsgBoxTemp;
        kMsgBoxTemp.m_strText1 = g_kGlobalValue.m_vecDelayMsg[iMsg];        
        m_vecWaitMsgBox.push_back( kMsgBoxTemp );
    }
    g_kGlobalValue.m_vecDelayMsg.clear();

    // 메세지 박스 대기 리스트를 확인해보고 있으면 메세지 박스를 띄운다.
    if ( false == m_bMsgBoxModal && int(m_vecWaitMsgBox.size()) != 0  )
    {
        if( ( g_MyD3D->m_pStateMachine->GetState() != GS_ROOM || 
              m_pkRoomSceneS6->GetWndState() == KD3DWnd::D3DWS_ENABLED )
            && ( IsValidState_ShowMessageBox() || IsAllStateMessageBox( m_vecWaitMsgBox[0].m_eMsgBoxType ) ) )
        {
            KMsgBoxInfo kMsgBoxInfo = m_vecWaitMsgBox[0];
            m_vecWaitMsgBox.erase( m_vecWaitMsgBox.begin() ); // 대기목록에서 미리 지워준다
            MessageBox( kMsgBoxInfo.m_eMsgBoxType, kMsgBoxInfo.m_strText1, kMsgBoxInfo.m_strText2, kMsgBoxInfo.m_eMsgBoxUse,kMsgBoxInfo.m_lParam, kMsgBoxInfo.m_lParam2 );
        }
    }


    if ( IsStop() && IsAllStop() == false)
    {
        m_pkLevelupInfoBox->FrameMove();
        if ( m_bMsgBoxModal && m_pkCurMsgBox != NULL )
        {
            //게임중에는 종료 다이얼로그만 있을 수 있다			
            /*if( GS_GAME == g_MyD3D->m_pStateMachine->GetState() )
            {
                if( m_pkMsgBox->GetUse() != GC_MBOX_USE_EXIT )
                {
                    m_pkMsgBox->Destroy();					
                    m_bMsgBoxModal = false;
                    Stop(false);
                }
            }*/

                // 메시지 박스는 이미 닫혔는데 아직도 Stop 상태다?? 그럼 풀어주자!!
                if( m_pkCurMsgBox->GetWndState() == KD3DWnd::D3DWS_DISABLED )
                {
                    EndMessageBox();
                }

                if( m_pkActiveWnd != NULL )
                    m_pkActiveWnd->ToggleRender( false );

				if( m_pkCharacterSelectBox != NULL && !m_pkCharacterSelectBox->m_bNonStopMode && m_pkCharacterSelectBox->IsRenderOn()){
					m_pkCharacterSelectBox->SpeakToActionListener( KActionEvent( g_pkUIScene->m_pkCharacterSelectBox, KD3DWnd::EWNDMESSAGE_CLOSE ) );
	
				}
				else if(m_pkCharacterSelectBox != NULL){
					m_pkCharacterSelectBox->SetWndUpdateLock(false);
					m_pkCharacterSelectBox->FrameMove();
				}

                //SetChildWndUpdateLock( false );
                g_pkInput->SetVirtualMode(false);
                m_pkCurMsgBox->FrameMove();

                //광장의 경우 예외로 프레임 무브 돌려준다..-_-;
                if(g_MyD3D->m_pStateMachine && GS_SQUARE == g_MyD3D->m_pStateMachine->GetState())
                {
                    g_pkUIScene->m_pkSquareScene->FrameMove();
                }

				//챗창 다른 다이얼 로그 발생해도 돌려준다.
				if( m_pkCurMsgBox == m_pkRoomListDlgS6 )
				{
					m_pkChatBoxDlgS6->FrameMove();
				}
                return;
        }
        else
        {
            if( m_pkActiveWnd != NULL )
            {
                m_pkActiveWnd->SetWndUpdateLock( false );
                m_pkActiveWnd->FrameMove();
            }
        }
    }

    std::vector<KD3DWnd*>::iterator lit = m_vecDestroyDlg.begin();
    while( lit != m_vecDestroyDlg.end() ) {
        (*lit)->FrameMove();

        if( (*lit)->GetWndState() == KD3DWnd::D3DWS_DISABLED ) {
            lit = m_vecDestroyDlg.erase( lit );
        }
        else {
            ++lit;
        }
    }
    
    // 전직미션 구매 가이드
#if defined( USE_TRANSFER_MISSION_SCROLL_BUYING_GUIDE )
    if( g_pkUIScene->EnableShowMessageBox() &&
        g_MyD3D->m_kGuideMgr.IsGuideOngoing() == false )
    {
        g_pkUIScene->m_pkTransferMissionScrollGuideDlg->ShowMissionGuide();
    }
#endif

    if( g_pkUIScene->EnableShowMessageBox() &&
        g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP && 
        SiKGCInfinityDungeonManager()->IsShowGuide() == true )
    {
        SiKGCInfinityDungeonManager()->ShowGuide();
    }

#if defined( USE_SURVEY_SYSTEM )
    if ( true ==  g_pkUIScene->m_pkSurveyDlg->IsReserveQuestion()
        && g_MyD3D->m_kGuideMgr.IsGuideOngoing() == false ) { 
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SURVEY_DLG );
    }
#endif

#if defined(USE_CHRISTMAS_GIFT_EVENT)
    // 크리스마스 이벤트 보관 중인 선물 메시지를 출력
    if( g_MyD3D->m_pStateMachine->GetState() != GS_GAME
        && g_MyD3D->m_kGuideMgr.IsGuideOngoing() == false ){
        g_pkUIScene->m_pkChristmasGiftDlg->ShowGiftMsg();
    }
#endif

}

HRESULT KGCUIScene::EnterRoom( int iRoomNum, const std::wstring& strRoomName )
{
    if ( g_pkRoomChatBox != NULL ) 
        g_pkRoomChatBox->ClearChatBox( GCPairPlaySystem::PPSCT_ROOM );

    return S_OK;
}


void KGCUIScene::ActionPerformed( const KActionEvent& event )
{
    if ( event.m_pWnd == m_pkMsgBox ) // 일반 메세지 박스일 경우
    {
        switch ( event.m_dwCode )
        {
        case KGCMessageBox::MBR_YES: // '예'를 눌렀을 경우
            {
                switch ( m_pkMsgBox->GetUse() )
                {
                case GC_MBOX_USE_GAMEEXIT:
                    {        
                        g_MyD3D->m_pStateMachine->GoExitClient();
                        break;
                    }

                case GC_MBOX_USE_CERTIFY_GASH:
                    {
                        g_MyD3D->m_pStateMachine->GoServer();
                        PopUpWebPage( L"https://ssl.grandchase.com.tw/member/authentication.htm" );
                        break;
                    }
                case GC_MBOX_USE_EXIT:
                    {
                        if (g_MyD3D->m_pStateMachine->GetState() == GS_GAME)
                        {
                            if( IsDetermineWin &&
                                !IsResultComplete &&
                                g_kGlobalValue.m_kUserInfo.bHost )
                            {
                                //이때는 못나감. 
                            }
                            //else if( g_MyD3D->Story < GC_FPS_LIMIT * 60 )
                            //{
                            //    //이때도 못나감.
                            //    g_pkUIScene->MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_CANNOT_EXIT_YET),L"",GC_MBOX_USE_NORMAL,0,0,false,true );

                            //}
                            else
                            {
#if defined( COLLECT_FRAME_DATA )
                                g_kGlobalValue.WriteCollectedFrameData();
#endif
                                if ( g_pkGameOverlayUI->m_pkAddtionGuide->GetGuideUserState() == STATE_PROGRESS_IN_TUTORIAL) {
                                    g_pkGameOverlayUI->m_pkAddtionGuide->SetGuideUserState( STATE_TUTORIAL_END );
                                    KP2P::GetInstance()->Send_TutorialClearNOT( );
                                }

                                int iPlayer = g_MyD3D->Get_MyPlayer();
                                if( g_MyD3D->IsPlayerIndex( iPlayer ) )
                                {
                                    // 갈려면 니가 소환했던 몬스터는 정리하고 가라~
                                    g_MyD3D->MyPlayer[iPlayer]->SetSummonMonsterHP_SendHost( 0.0f, -1, true );
#if defined( DUNGEONPLAYSTATE )
                                    // 던전플레이통계( 강제종료 )
                                    g_kGlobalValue.AddDungeonPlayState( KGCGlobal::DUNPLAY_STATE_LEAVE );
                                    g_MyD3D->MyPlayer[iPlayer]->SendDungeonPlayState( KGCGlobal::DUNPLAY_STATE_LEAVE );
#endif
                                }

                                // 점령전이면 대기중인 DPointShop모두 제거해라.
                                while( g_pkUIScene->RemoveWaitMessageBox( KGCUIScene::GC_MBOX_DOTA_SHOP ) );

                                g_MyD3D->m_pStateMachine->OnExitGame();

                                if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT )
                                {
                                    g_pkAgitOverlayUI->OnExitAgit();

                                    if ( SiKGCPartyManager()->IsParty() )
                                        SiKGCPartyManager()->LeaveParty();

                                    // 아지트 퇴장일 경우 AgitGameManager에 상태를 저장 해 둔다
                                    SiKGCAgitGameManager()->SetAgitEnterd( false );
                                }
                            }

                        }
                        else if (g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE)
                        {
                            g_pkSquareOverlayUI->ExitSquare();
							m_pkGuildSquareNotice->Destroy();
                            
                            if ( SiKGCPartyManager()->IsParty() )
                                SiKGCPartyManager()->LeaveParty();
                        }
						else if( g_MyD3D->m_pStateMachine->GetState() == GS_GAME_LOADING )
						{
							KP2P::GetInstance()->KP2P::GetInstance()->Send_ClientFailedGameStartNot(KFailRate::USER_QUIT_GAME);
						}
						break;
                    }
                case GC_MBOX_USE_BUY: // 구입
                    {
                        GCITEMID dwItemID = static_cast<GCITEMID>(m_pkMsgBox->GetlParam());
                        int   iPeriod = static_cast<int>(m_pkMsgBox->GetlParam2());
                        g_MyD3D->m_kItemMgr.BuyItem(dwItemID, iPeriod);
                        m_pkCashShopScene->UpdateUserInfo();
#ifndef DISABLE_MAP_SHOP // Kawan>
                        m_pkGPShopScene->UpdateUserInfo();
#endif
                        m_pkCoordiClearSealDlg->UpdateOwnItemList();
                        m_pkNewClearSealDlg->UpdateOwnItemList();
                        break;
                    }
                case GC_MBOX_USE_BUY_FOR_GEM: // Gem으로 구입
                    {
                        BuyItemForGem( static_cast<GCITEMID>(m_pkMsgBox->GetlParam()) ,static_cast<int>(m_pkMsgBox->GetlParam2()), KItem::UNLIMITED_ITEM );
                        m_pkCashShopScene->UpdateUserInfo();
                        m_pkGPShopScene->UpdateUserInfo();
                        break;
                    }
                case GC_MBOX_USE_BUY_FOR_CRYSTAL: // 크리스탈로 구입
                    {
                        BuyItemForCrystal( static_cast<GCITEMID>(m_pkMsgBox->GetlParam()) , static_cast<int>(m_pkMsgBox->GetlParam2()), KItem::UNLIMITED_ITEM );
                        m_pkCashShopScene->UpdateUserInfo();                        
                        m_pkGPShopScene->UpdateUserInfo();
                        break;
                    }
                case GC_MBOX_USE_BUY_FOR_VP_AMMOUNT: // 크리스탈로 구입
                    {
                        GCItem* kItem = g_pItemMgr->GetItemData( static_cast<GCITEMID>( m_pkMsgBox->GetlParam() ) );
                        g_pItemMgr->SendBuyForVirtualCashItem( kItem, static_cast<int>( m_pkMsgBox->GetlParam2() ) );
                        m_pkCashShopScene->UpdateUserInfo();
                        m_pkGPShopScene->UpdateUserInfo();
                        break;
                    }
                case GC_MBOX_USE_BUY_POINT_ITEM:    // Æ÷ÀÎÆ® ¾ÆÀÌÅÛ ±¸ÀÔ
                {
                    GCITEMID dwItemID = static_cast<GCITEMID>(m_pkMsgBox->GetlParam());
                    int nCount = static_cast<int>(m_pkMsgBox->GetlParam2());
                    g_MyD3D->m_kItemMgr.BuyPointItem(dwItemID, nCount);
                    m_pkCashShopScene->UpdateUserInfo();
#ifndef DISABLE_MAP_SHOP
                    m_pkGPShopScene->UpdateUserInfo();
#endif
                    break;
                }
                case GC_MBOX_USE_DELETE_POINT_ITEM: // 포인트 아이템 제거
                    {
                        // 요놈은 UID를 받아서 찾아서 서버로 지워달라고 보냅니다
                        GCITEMUID dwItemUID = m_pkMsgBox->GetlParam();
                        RemovePointItem( dwItemUID );
                        m_pkCashShopScene->UpdateUserInfo();
                        m_pkGPShopScene->UpdateUserInfo();
                        break;
                    }
                case GC_MBOX_USE_SELL: // 팔기
                    {
                        KItem* pkInvenItemInfo = NULL;

                        if( g_pItemMgr->FindInventory( m_pkMsgBox->GetlParam(), &pkInvenItemInfo ) )
                        {
                             SellItem(pkInvenItemInfo);
                        }
                        break;
                    }
                case GC_MBOX_USE_SELL_BUNDLE_ITEM:
                    {
                        g_pItemMgr->SellItemFromList();
                        break;
                    }
                case GC_MBOX_USE_SELL_INVEN_ITEM:
                    {
                        g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_SELL_INVEN_ITEM ) );
                        break;
                    }
				case GC_MBOX_USE_GAMBLE_DELETE: // 팔기
					{						
						g_pkUIScene->m_pkGambleBox->DeleteItem( static_cast<GCITEMID>(m_pkMsgBox->GetlParam()) );
						break;
					}
                case GC_MBOX_USE_DELETE_ITEM:		//기간제 아이템 삭제
                    {
                        KItem* pkInvenItemInfo = NULL;

                        if(g_pItemMgr->FindInventory(m_pkMsgBox->GetlParam(), &pkInvenItemInfo ) )
                        {
                            SellItem(pkInvenItemInfo);
                        }
                        break;
                    }
                case GC_MBOX_USE_REPAIR: // 강화 속성 소생
                    {
                        KItem* pInventoryItem = (KItem *)m_pkMsgBox->GetlParam();
                        RepairBreakedItem( pInventoryItem );
                        break;
                    }


                case GC_MBOX_USE_NICK_NAME_AGREE: // 닉네임 승인
                    {
                        break;
                    }
                case GC_MBOX_USE_CHANGE_NICK_NAME:
                    {
                        GCITEMID dwItemID  = static_cast<GCITEMID>(m_pkMsgBox->GetlParam());
                        GCITEMUID dwItemUID = m_pkMsgBox->GetlParam2();
                        DeleteWaitMsgBox(GC_MBOX_NICK_NAME);
                        ChangeNickName( dwItemID, dwItemUID );

                        break;
                    }
                case GC_MBOX_USE_CHANGE_NICK_NAME_FROM_CAMPAIGN:
                    {
                        if ( ChangeNickNameFromCampaign( ) ) {
                            DeleteWaitMsgBox(GC_MBOX_NICKNAME_CAMPAIGN_BOX);
                        }

                        break;
                    }
                case GC_MBOX_USE_CHANGE_PET_NICK_NAME:
                    {
                        GCITEMID dwItemID  = static_cast<GCITEMID>(m_pkMsgBox->GetlParam());
                        GCITEMUID dwItemUID = m_pkMsgBox->GetlParam2();

                        KPetInfo kInfo = g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo;
                        if( kInfo.m_dwUID != 0 )
                        {
                            std::wstring s1 = m_pkNickNameBox->GetNickName();
                            std::wstring s2 = g_MyD3D->m_mapPetInfo[ kInfo.m_dwUID ].m_strName;

                            // 현재 펫이름과 입력한 이름이 같다면 변경할 수 없다
                            if(  m_pkNickNameBox->GetNickName().compare( g_MyD3D->m_mapPetInfo[ kInfo.m_dwUID ].m_strName ) == 0 )
                            {
                                MessageBox( GC_MBOX_OK,
                                    g_pkStrLoader->GetString( STR_PET_NICKNAME_SAME_NAME_ERROR1 ),
                                    g_pkStrLoader->GetString( STR_PET_NICKNAME_SAME_NAME_ERROR2 ), GC_MBOX_USE_NORMAL, 0, 0, true);
                            }
                            else
                            {
                                KItem NickItem;
                                NickItem.m_ItemUID      = dwItemUID;
                                NickItem.m_ItemID       = dwItemID;
                                NickItem.m_nCount    = g_MyD3D->m_kItemMgr.GetInventoryItemDuration( dwItemID );

                                DeleteWaitMsgBox(GC_MBOX_NICK_NAME);
								ChangePetNickName( NickItem, kInfo.m_dwID, kInfo.m_dwUID );
								
                            }
                        }
                        break;
                    }
                case GC_MBOX_USE_DELETE_MISSION:
                    {
                        // 서버에게 지워달라고 요청한다
                        KP2P::GetInstance()->Send_RemoveMission( g_kGlobalValue.m_kUserMission.GetDeleteReadyMission() );

                        break;
                    }
                case GC_MBOX_USE_DELETE_COLLECTION_MISSION:
                    {
                        // 서버에게 지워달라고 요청한다
                        KP2P::GetInstance()->Send_RemoveMission( g_kGlobalValue.m_kUserCollectionMission.GetDeleteReadyCollectionMission() );
                        break;
                    }
                case GC_MBOX_USE_PET_FEEDING:
                    {
                        Result_FeedingPet = INT_MAX;

                        KPetInfo kPetInfo = g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo;
                        GCITEMUID dwItemUID = m_pkMsgBox->GetlParam();

                        int iPreSatiation = kPetInfo.m_iSatiation;

                        KItem* kInventoryItem = NULL;
                        GCItem* kItem = NULL;
                        if( g_pItemMgr->FindInventory( dwItemUID, &kInventoryItem, &kItem ) == false )
                            break;

                        KP2P::GetInstance()->Send_FeedingPetReq( kPetInfo.m_dwUID, kInventoryItem->m_ItemUID );

                        g_MyD3D->WaitForServerAck( Result_FeedingPet, INT_MAX, 0xffffffff, TIME_OUT_VALUE );
                        switch( Result_FeedingPet )
                        {
                        case 0:
                            {
								g_pkUIScene->m_pkMyInfoScene->SetEquipWndCategory(UEC_PET);

								for( int i = 0; i < (int)g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.size(); i++ )
                                {
                                    int iCurrentCharType = g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo[i].iCharType;
                                    GCITEMUID dwPetUID = g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo[i].kPetInfo.m_dwUID;
                                    g_MyD3D->m_TempPlayer.SetEquipPetInfo( iCurrentCharType, dwPetUID );
                                }

                                std::vector<GCITEMUID> vecDecItemUID;
                                g_pItemMgr->DecDurationItem( kInventoryItem->m_ItemUID, &vecDecItemUID );
                                g_pkUIScene->m_pkMyInfoScene->UpdateData();
                                g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
                                break;
                            }
                        case 8:
                        case 1:
                            {
                                std::wstring strTemp = g_pkStrLoader->GetString( STR_ID_NO_EXIST_PET );
                                strTemp += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", 
                                    KUserEvent::EVENT_FEEDING_PET_ACK, Result_FeedingPet );	

                                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CASH_FAIL_TITLE ), 
                                    strTemp );
                                break;
                            }
                        case 3:
                        case 4:
                        case 5:
                        case 6:
                        case 7:
                            {
                                std::wstring strTemp = g_pkStrLoader->GetString( STR_ID_NOT_PET_FOOD );
                                strTemp += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", 
                                    KUserEvent::EVENT_FEEDING_PET_ACK, Result_FeedingPet );	
                                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CASH_FAIL_TITLE ), 
                                    strTemp );
                                break;
                            }
                        default:
                            {
                                std::wstring strTemp = g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_DISCONNECTED2 );
                                strTemp += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", 
                                    KUserEvent::EVENT_FEEDING_PET_ACK, Result_FeedingPet );	
                                
                                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_DISCONNECTED1 ), 
                                    strTemp );

                                break;
                            }
                        }
                        break;
                    }
                case GC_MBOX_USE_EXTEND_INVENTORY:
                    {
                        Result_ExtendInventory = INT_MAX;

                        GCITEMUID dwItemUID = m_pkMsgBox->GetlParam();
                        int iItemID = static_cast<int>(m_pkMsgBox->GetlParam2());

                        KP2P::GetInstance()->Send_InventoryExtendReq( iItemID, dwItemUID );

                        g_MyD3D->WaitForServerAck( Result_ExtendInventory, INT_MAX, 5000, TIME_OUT_VALUE );

                        //kRecv.m_nOK = 0; // 성공
                        //kRecv.m_nOK = 1; // 아이템 아이디가 다름
                        //kRecv.m_nOK = 2; // 아이템을 보유하고 있지 않음
                        //kRecv.m_nOK = 3; // 아이템 수량이 부족
                        //kRecv.m_nOK = 4; // 더이상 인벤토리를 확장할수 없음
                        //kRecv.m_nOK = 5; // 해당 유저가 존재하지 않음
                        switch( Result_ExtendInventory )
                        {
                        case 0: // 성공
                            {
                                //아이템 사용에 성공했습니다.
                                //"@1개의 아이템을 가질 수 있습니다."
                                int iInvenCapacity = g_kGlobalValue.m_iInvenCapacity;
                                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                                    g_pkStrLoader->GetString( STR_ID_EXTEND_INVENTORY_MESSAGE3 ),
                                    g_pkStrLoader->GetReplacedString( STR_ID_EXTEND_INVENTORY_MESSAGE4, "i", iInvenCapacity ) );

                                std::vector<GCITEMUID> vecDecItemUID;
                                g_pItemMgr->DecDurationItem( dwItemUID, &vecDecItemUID );
                                g_pkUIScene->m_pkMyInfoScene->UpdateGameData();

                                break;
                            }
                        case 4: // 더이상 인벤토리를 확장할수 없음
                            {
                                // 인벤토리는 MAX_INVENTORY_SIZE 칸 까지 늘릴 수 있습니다._더 이상 사용 할 수 없습니다."
                                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                                    g_pkStrLoader->GetReplacedString( STR_ID_EXTEND_INVENTORY_MESSAGE5, "i", MAX_INVENTORY_SIZE ), L"" );
                                break;
                            }
                        case 1: // 아이템 아이디가 다름
                        case 2: // 아이템을 보유하고 있지 않음
                        case 3: // 아이템 수량이 부족
                        case 5: // 해당 유져가 존재하지 않음
                            {
                                //"인벤토리 늘리기에 실패했습니다."

                                std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", 
                                    KUserEvent::EVENT_USE_INVENTORY_EXTEND_ACK, Result_ExtendInventory );	
                                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                                    g_pkStrLoader->GetString( STR_ID_EXTEND_INVENTORY_MESSAGE6 ), strTemp );
                                break;
                            }
                        default: // 타임아웃
                            {
                                std::wstring strTemp = g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_DISCONNECTED2 );
                                strTemp += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", 
                                    KUserEvent::EVENT_USE_INVENTORY_EXTEND_ACK, Result_ExtendInventory );	

                                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                                    g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_DISCONNECTED1 ), strTemp );
                                break;
                            }
                        }
                        break;
                    }
                case GC_MBOX_USE_GET_REWARD_ITEM: // 월 개근 보상 아이템 받기
                    {
                        //int iItemID = m_pkMsgBox->GetlParam();

                        //Result_GetRewardItem = INT_MAX;

                        //KSimpleDate kDate = g_kGlobalValue.m_vecReservedReward[0];
                        //KItemUnit kItemUnit = g_MyD3D->m_kItemMgr.GetPerfectMonthItem( iItemID );

                        //KP2P::GetInstance()->Send_MonthlyAttendRewardReq( kDate, kItemUnit );
                        //g_MyD3D->WaitForServerAck( Result_GetRewardItem, INT_MAX, 5000, TIME_OUT_VALUE );

                        ////  0 ; 성공
                        //// -1 : 요청한 월개근 보상 아이템이 보상 리스트에 존재하지 않음
                        ////  2 ; 포인트백 정보가 없음
                        ////  3 ; 유저의 해당월 정보가 없음)
                        ////  4 ; ItemID가 -1이 입력되었음)
                        ////  5 ; 월개근 보상 Item얻기 실패)
                        //switch( Result_GetRewardItem )
                        //{
                        //case 0:
                        //    {
                        //        //GCItem* kItem = g_MyD3D->m_kItemMgr.GetItemData( iItemID, 0 );
                        //        //BREAK_NIL( kItem );

                        //        //g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                        //        //    g_pkStrLoader->GetReplacedString( STR_ID_GET_REWARD_ITEM_SUCCESS, "l", kItem->strItemName ), L"",
                        //        //    KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false );
                        //        break;
                        //    }
                        //case -1:
                        //case 2:
                        //case 3:
                        //case 4:
                        //case 5:
                        //default: // 일괄적으로 처리해도 되겠지-ㅅ- 실패했다는..
                        //    {
                        //        //g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_GET_REWARD_FAIL ), L"",
                        //        //    KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false );
                        //        break;
                        //    }
                        //}
                        break;
                    }
                    // 승패 정보 초기화 아이템
                case GC_MBOX_USE_RESET_WIN_LOSE_DATA:
                    {
                        GCITEMID dwItemID = static_cast<GCITEMID>(m_pkMsgBox->GetlParam());

                        Result_ResetWinLoseData = INT_MAX;

                        KP2P::GetInstance()->Send_Reset_Win_Lose_Data( dwItemID );
                        g_MyD3D->WaitForServerAck( Result_ResetWinLoseData, INT_MAX, 5000, TIME_OUT_VALUE );

                        //  0 성공
                        //  1 승률 초기화 아이템이 아닙니다.
                        //  2 아이템과 캐릭터의 쌍이 맞지 않음.
                        //  3 승률 초기화 아이템을 보유하고 있지 않습니다.
                        //  4 캐릭터 정보가 없습니다.
                        //  5 해당 유저가 존재하지 않습니다.
                        //  6 캐릭터 타입이 잘못 되었습니다.
                        //  7 캐릭터테이블에 캐릭터 정보가 없습니다.
                        //  8 알수 없는 오류
                        switch( Result_ResetWinLoseData )
                        {
                        case 0: // 성공
                            {
                                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_RESULT_USE_RESET_WIN_LOSE_ITEM1 ) );
                                break;
                            }
                        case 1: // 승률 초기화 아이템이 아닙니다
                        case 2: // 아이템과 캐릭터의 쌍이 맞지 않음.
                        case 3: // 승률 초기화 아이템을 보유하고 있지 않습니다.
                            {
                                // 잘못된 아이템을 사용했습니다.
                                // 게임을 다시 실행 시킨 뒤 시도해 주세요.
                                std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_RESET_WIN_LOSE_ACK, Result_ResetWinLoseData );	
                                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_RESULT_USE_RESET_WIN_LOSE_ITEM2 ), strTemp );
                                break;
                            }
                        case 4: // 캐릭터 정보가 없습니다.
                        case 5: // 해당 유저가 존재하지 않습니다.
                        case 6: // 캐릭터 타입이 잘못 되었습니다.
                        case 7: // 캐릭터테이블에 캐릭터 정보가 없습니다.
                        case 8: // 알수 없는 오류
                        default: // 타임아웃
                            {
                                // 오류가 발생했습니다.
                                // 게임을 다시 실행 시킨 뒤 시도해 주세요.
                                std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_RESET_WIN_LOSE_ACK, Result_ResetWinLoseData );	
                                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_RESULT_USE_RESET_WIN_LOSE_ITEM3 ), strTemp );
                                break;
                            }
                        }

                        break;
                    }
                case GC_MBOX_USE_END_GAME:
                    {
						if( g_MyD3D->m_pStateMachine->GetState() == GS_GAME_LOADING )
							KP2P::GetInstance()->KP2P::GetInstance()->Send_ClientFailedGameStartNot(KFailRate::USER_QUIT_GAME);

                        if( g_MyD3D->m_pStateMachine->GetState() == GS_GAME &&
                            IsDetermineWin &&
                            !IsResultComplete &&
                            g_kGlobalValue.m_kUserInfo.bHost )
                        {
                            //이때는 못나감. 
                        }
#if defined( NATION_BRAZIL)
                        else if( g_MyD3D->Story < GC_FPS_LIMIT * 60 )
                        {
                            //이때도 못나감.
                            g_pkUIScene->MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_CANNOT_EXIT_YET),L"",GC_MBOX_USE_NORMAL,0,0,false,true );

                        }
#endif
                        else if( SiKGCWorldMapManager()->IsHeroDungeonMode() )
                        {
                            g_MyD3D->m_pStateMachine->ExitRoom( true );
                        }
                        else
                        {
                            g_pStateGame->EndGame();
                            g_MyD3D->m_pStateMachine->ExitRoom( true );
                        }
                        break;
                    }
                case GC_MBOX_USE_NEONSIGN_RESERVE:
                    {
                        if( false == SiKGCAnnounceMgr()->IsAnnounceListFull() )
                        {
                            KP2P::GetInstance()->Send_RegSignBoard( m_pkNeonsignPreview->GetAnnounceData() );
                            //SiKGCAnnounceMgr()->ReserveforDebug( m_pkNeonsignPreview->GetAnnounceData() );
                            if( true == IsWaitMsgBox( KGCUIScene::GC_MBOX_NEONSIGN_PREVIEW ) )
                            {
                                g_pkUIScene->DeleteWaitMsgBox( KGCUIScene::GC_MBOX_NEONSIGN_PREVIEW );
                            }
                            m_pkNeonsignPreview->OnReserveClose();
                        }
                        else
                        {
                            MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NEONSIGN_IS_FULL ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true );
                            m_pkNeonsignPreview->WaitForServer( false );
                        }
                        break;
                    }
                case GC_MBOX_USE_MINIGAME_EXIT:
                    {
                        m_bExitMinigame = true;
                        break;
                    }
					break;                
				case GC_MBOX_USE_NATEON_DELETE_GROUP:
					{
                m_pkMsgBox->Destroy();
                        MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_NATEON_DELETE_GROUP_ASK2 ), 
                            L"", KGCUIScene::GC_MBOX_USE_NATEON_DELETE_GROUP_REALLY, m_pkMsgBox->GetlParam(), 0, true, false );
                        return;
					}
                case GC_MBOX_USE_NATEON_DELETE_GROUP_REALLY:
                    {
                        m_pkMsgBox->Destroy();
                        DeleteNateOnBuddyGroup( (int)m_pkMsgBox->GetlParam() );
                        return;
                    }
				case GC_MBOX_USE_LEARN_SKILL:
					{
						KP2P::GetInstance()->Send_SkillTrainingReq( KGCSkillBoxSub::GetRequestedSkill() );
						m_pkMsgBox->Destroy();
						return;
					}
				case GC_MBOX_USE_REMOVE_SKILL:
					{
						KP2P::GetInstance()->Send_RemoveSkill( KGCSkillBoxSub::GetRequestedSkill(), KGCSkillBoxSub::GetUntrainItemID() );
						m_pkMsgBox->Destroy();
						return;
					}
				case GC_MBOX_USE_CHANGE_COUPON:
					{
						GCITEMUID dwItemUID = m_pkMsgBox->GetlParam();
						int iAmmount = static_cast<int>( m_pkMsgBox->GetlParam2() );

						GCItem* kItem = g_MyD3D->m_kItemMgr.GetItemData( g_kGlobalValue.m_pkCurrentUseItemID );
						BREAK_NIL( kItem );
					
						KP2P::GetInstance()->Send_ChangeCouponReq( g_kGlobalValue.m_pkCurrentUseItemID, dwItemUID, (char)kItem->eItemType, iAmmount );
						m_pkMsgBox->Destroy();

                        g_kGlobalValue.m_pkCurrentUseItemID = 0;
						return;
                    }
                case GC_MBOX_USE_VIRTUAL_CASH_COUPON:
                    {
                        GCITEMUID dwItemUID = m_pkMsgBox->GetlParam();
                        int iAmmount = static_cast<int>( m_pkMsgBox->GetlParam2() );

                        GCItem* kItem = g_MyD3D->m_kItemMgr.GetItemData( g_kGlobalValue.m_pkCurrentUseItemID );
                        BREAK_NIL( kItem );

                        KP2P::GetInstance()->Send_ChangeVirtualCashCouponReq( g_kGlobalValue.m_pkCurrentUseItemID, dwItemUID, (char)kItem->eItemType, iAmmount );
                        m_pkMsgBox->Destroy();

                        g_kGlobalValue.m_pkCurrentUseItemID = 0;
                        return;
                    }
                case GC_MBOX_USE_BUY_VIP:
                    {
                        int iIndex = static_cast<int>(m_pkMsgBox->GetlParam());
                        m_pkVipGachaBox->BuyItemProcedure( iIndex );
                        break;
					}
                case GC_MBOX_USE_GACHA_AUTO:
                {
                    if (g_kGlobalValue.m_iCurrentGachaType == 0) {
                        GetNewClearSealDlg()->StartAuto();
                        GetNewClearSealDlg()->RollGacha();
                    }
                    else {
                        GetClearSealDlg()->StartAuto();
                        GetClearSealDlg()->RollGacha();
                    }

                    m_pkMsgBox->Destroy();

                    return;
                }
                case GC_MBOX_USE_RECOMMEND_NOT:
                    {
						DeleteWaitMsgBox(GC_MBOX_NICK_NAME);	
                        m_pkNickNameBox->OnConfirmRecommend();
                        if( m_pkMsgBox->GetlParam() == 1 )
						    m_pkNickNameBox->SetRecommendRegComplete(true);
                        break;
                    }
				case GC_MBOX_USE_RECOMMEND_PICKED_USER:
					{
						GCSquarePeople* pRecomPeople = SiGCSquare()->GetSquarePeople(static_cast<DWORD>(m_pkMsgBox->GetlParam()));
						
						if(pRecomPeople)
						{
							ReportRecommendNickName(pRecomPeople->GetPeopleInfo().m_strNick);
						}
					}
                    break;
                case GC_MBOX_USE_RECOMMEND_NICKNAME:
                    {
                        ReportRecommendNickName( m_pkNickNameBox->GetRecommendNickName() );
                    }
                    break;
                case GC_MBOX_USE_BUY_HERO_ITEM:
                    {
                        GCITEMID dwItemID = static_cast<GCITEMID>(m_pkMsgBox->GetlParam());
                        USHORT usIndex = static_cast<USHORT>(m_pkMsgBox->GetlParam2());
                        Result_BuyForHero = INT_MAX;

                        PAIR_USHORT_PAIR_DWORD_INT pairTmp = std::make_pair( usIndex, std::make_pair( dwItemID, g_kGlobalValue.m_iCurrentUseItemMultiplier ) );
                        KP2P::GetInstance()->Send_BuyForHeroReq( pairTmp );
                        g_MyD3D->WaitForServerAck( Result_BuyForHero, INT_MAX, 5000, TIME_OUT_VALUE);

                        if( 0 == Result_BuyForHero ) {
                            if ( NULL != g_pkUIScene->m_pkHeroDungeonDlg->m_pkDungeonItemShop ) {
                                g_pkUIScene->m_pkHeroDungeonDlg->m_pkDungeonItemShop->SetMyHeroCoin();
                            }
                            if ( NULL != g_pkUIScene->m_pkHeroDungeonDlg->m_pkItemUpgrade ) {
                                g_pkUIScene->m_pkHeroDungeonDlg->m_pkItemUpgrade->SetMyHeroCoin();
                            }
                        }

                        m_pkMsgBox->Destroy();
                        return;
                    }
                    break;
                case GC_MBOX_USE_MAGIC_PROMOTION:
                    {
                        KP2P::GetInstance()->Send_CharPromotionUpdateReq( g_kGlobalValue.m_iAMPItemID, g_kGlobalValue.m_iAMPCharType, g_kGlobalValue.m_iAMPPromotion );
                    }
                    break;
                case GC_MBOX_USE_WEAPON_CHANGE:
                    {
                        KItem* pkItem = NULL;
                        g_MyD3D->m_kItemMgr.FindInventoryForItemID( ITEM_ID_WC_UNLOCK, &pkItem );

                        if ( NULL == pkItem || pkItem->m_nCount < 1 )
                        {
                            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_WC_STRING4 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL );
                        }
                        else
                            SiKP2P()->GetInstance()->Send_UnlockChangeWeaponReq( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType );
                    }
                    break;
                case GC_MBOX_RESET_SKILL:
                    {
                        char cCharIdx = g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex;
                        KP2P::GetInstance()->Send_ResetSkillReq( KGCItemManager::ITEM_RESET_SKILL, g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo[ cCharIdx ].iCharType );
                    }
                    break;
                case GC_MBOX_USE_CLOSE_CLIENT_FOR_CASH_CHARGE:
                    {
                        PopUpWebPage( HOMEPAGE_CHARGE_CASH );
                        PostMessage( g_MyD3D->Get_hWnd(), WM_CLOSE, 0, 0 );                        
                    }
                    break;
                case GC_MBOX_USE_BUY_AGIT_SHOP:
                    {
                        DWORD ItemID = static_cast<DWORD>(m_pkMsgBox->GetlParam());
                        USHORT usIndex = static_cast<USHORT>(m_pkMsgBox->GetlParam2());
                        Result_BuyAgitShop = INT_MAX;
                        
                        PAIR_USHORT_DWORD temp = std::make_pair( usIndex, ItemID );
                        KP2P::GetInstance()->Send_BuyForAgitReq( temp );
                        g_MyD3D->WaitForServerAck( Result_BuyAgitShop, INT_MAX, 5000, TIME_OUT_VALUE );

                        SiKGCAgitGameManager()->SetPrePlace( false );
                        g_pkAgitOverlayUI->ShowPrePlaceUI( false );

                        m_pkMsgBox->Destroy();
                    }
                    break;
                case GC_MBOX_USE_SELL_AGIT_OBJECT:
                    {

                    }
                case GC_MBOX_USE_INVITE_AGIT_MESSAGE:
                    {
                        m_pkMsgBox->Destroy();

                        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT &&	g_MyD3D->m_pStateMachine->GetState() == GS_GAME ) {
                            g_pStateGame->OnExitGame(); 
                        }
                        else if (g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE) // 광장이라면 광장 종료 처리 해주고 
                        {
                            g_pkSquareOverlayUI->ExitSquare();
                            m_pkGuildSquareNotice->Destroy();

                            if ( SiKGCPartyManager()->IsParty() )
                                SiKGCPartyManager()->LeaveParty();
                        }

                        KInviteAgitNot kInviteAgitInfo = SiKGCAgitGameManager()->GetInviteAgitIno();                        
                        if( kInviteAgitInfo.m_ucAgitOpenType == 0 )
                        {
                            Result_EnterAgitReq = INT_MAX;
                            KP2P::GetInstance()->Send_EnterAgitReq( kInviteAgitInfo.m_dwAgitUID, g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType, L"");
                            g_MyD3D->WaitForServerAck( Result_EnterAgitReq, INT_MAX, 5000, TIME_OUT_VALUE );

                        }
                        else
                        {
                            if( kInviteAgitInfo.m_bIsOwnerInvite )
                            {
                                Result_EnterAgitReq = INT_MAX;
                                KP2P::GetInstance()->Send_EnterAgitReq( kInviteAgitInfo.m_dwAgitUID, 
                                    g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType, kInviteAgitInfo.m_strAgitPassword);
                                g_MyD3D->WaitForServerAck( Result_EnterAgitReq, INT_MAX, 5000, TIME_OUT_VALUE );

                            }
                            else
                            {                                
                                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_PASSWORD, L"", L"", KGCUIScene::GC_MBOX_USE_INVITE_AGIT_INPUT_PASSWORD, 
                                    kInviteAgitInfo.m_dwAgitUID, 0, true, true );
                            }                            
                        }
                    }
                    return;
                case GC_MBOX_USE_AGIT_MOVE:
                    {
                        if( SiKGCAgitGameManager()->GetRegisterState() )
                            SiKGCAgitGameManager()->UnRegisterAgitList();

                        DWORD dwMapID = static_cast<DWORD>(m_pkMsgBox->GetlParam());
                        Result_BuyAgitShop = INT_MAX;
                        KP2P::GetInstance()->Send_BuyAgitMapReq( dwMapID );
                        g_MyD3D->WaitForServerAck( Result_BuyAgitShop, INT_MAX, 5000, TIME_OUT_VALUE );

                        if( 0 == Result_BuyAgitShop )
                        {
                            m_pkMsgBox->Destroy();
                            SiKGCAgitGameManager()->MoveAgit();
                        }
                    }
                    break;
                case GC_MBOX_USE_ADD_FAVORITE_AGIT:
                    {
                        DWORD dwAgitUID = SiKGCAgitGameManager()->GetAgitUID();
                        KP2P::GetInstance()->Send_AgitAddFavoriteReq( dwAgitUID );
                    }
                    break;
                case GC_MBOX_USE_SEED_PLANT_CONFIRM:
                    g_pkAgitOverlayUI->ShowSeedPlant( true );
                    break;
                case GC_MBOX_USE_DELETE_GUESTBOOK_MSG:
                    {
                        DWORD dwMsgID = static_cast<DWORD>(m_pkMsgBox->GetlParam());
                        
                        std::set<int> deleteID;
                        deleteID.insert( static_cast<int>( dwMsgID ) );
                        KP2P::GetInstance()->Send_GuestbookDeleteReq( deleteID );
                    }
                    break;
                case GC_MBOX_USE_AGIT_CHARACTER_OPEN:
                    {
                        // 오픈 요청 )
                        DWORD dwCharType = static_cast<DWORD>(m_pkMsgBox->GetlParam());
                        KP2P::GetInstance()->Send_OpenAgitCharReq( static_cast<int>( dwCharType ) );
                    }
                    break;
                case GC_MBOX_USE_VIRTUAL_CASH_INCLUDE_CASH:
                    {
                        // 구매요청
                        m_pkBuyCashBox->BuyCashItemUseCashAndVirtualCash();
                        DeleteWaitMsgBoxforUse( GC_MBOX_USE_VIRTUAL_CASH_INCLUDE_CASH );
                        g_KDSound.Play( "32" );
                        return;
                    }
                    break;
				case GC_MBOX_USE_PET_SKILL_BUY: // 구입
					{
						GCITEMID dwItemID = static_cast<GCITEMID>(m_pkMsgBox->GetlParam());
						int   iPeriod  = static_cast<int>(m_pkMsgBox->GetlParam2());
						g_MyD3D->m_kItemMgr.BuyItem( dwItemID, iPeriod );
						g_MyD3D->m_TempPlayer.EquipPetItem( dwItemID );
						g_MyD3D->m_TempPlayer.RemovePlayerObject();
						g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
						g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();
						g_pkUIScene->m_pkMyInfoScene->UpdateData();
						g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
						break;
					}
                case GC_MBOX_USE_DELETE_LETTER:
                    {
                        int iType = static_cast<int>( m_pkMsgBox->GetlParam() );
                        if( iType == 0 ) {
                            g_pkUIScene->m_pkPostLetterDlg->DeleteLetter();
                        }
                        else if( iType == 1 ) { 
                            g_pkUIScene->m_pkPostLetterDlg->DeleteRecvUILetter();
                        }
                    }
                    break;
                case GC_MBOX_USE_EXP_POTION:
                    {
                        SCharInfo sCharInfo = g_MyD3D->m_TempPlayer.GetCurrentChar();
                        if( 0 <= sCharInfo.iCharType && sCharInfo.iCharType < GC_CHAR_NUM )
                        {
                            KP2P::GetInstance()->Send_UseExpPotionReq( static_cast<int>(m_pkMsgBox->GetlParam()), static_cast<int>(m_pkMsgBox->GetlParam2()), sCharInfo.iCharType );
                        }
                    }
                    break;
				case GC_MBOX_USE_DEPOT_EXTEND: // 구입
					{
						m_pkMsgBox->Destroy();

						int iType = static_cast<int>(m_pkMsgBox->GetlParam());
						// 0 = 창고 확장, 1 = 슬롯 확장.

						if ( iType == KDepotExtend::TAB_EXTEND ) {
							SiKGCDepotManager()->SendSafeAdd();
						}
						else if ( iType == KDepotExtend::SLOT_EXTEND ) {
							SiKGCDepotManager()->SendSlotAdd();
						}

						return;
					}
                case GC_MBOX_USE_GC_ADVENTURE_FINAL_REWARD:
                    {
                        m_pkMsgBox->Destroy();
                        if ( SiGCAdventureManager()->isValidEventPeriod() ) {
                            SiGCAdventureManager()->WholeContinentFinalRewardReq();
                            g_pkUIScene->m_pkGcAdventureDlg->OnClickWholeContinent();
                        }
                    }
                    break;
                case GC_MBOX_USE_OPEN_CHANGE_SKILL_SLOT:
                    {                        
                        KP2P::GetInstance()->Send_AddSkillSlotOpenReq( static_cast<GCITEMID>(m_pkMsgBox->GetlParam()) );                        
                    }
                    break;
                case GC_MBOX_USE_LEVEL_RESET_ITEM:
                    {
                        KP2P::GetInstance()->Send_LevelResetItemReq( static_cast<char>(m_pkMsgBox->GetlParam()), static_cast<GCITEMID>(m_pkMsgBox->GetlParam2()) );
                    }
                    break;
                case GC_MBOX_USE_BUY_HERO_ITEM_UPGRADE:
                    {
                        std::pair< GCITEMID, int > prKey;
                        prKey.first  = static_cast<GCITEMID>( m_pkMsgBox->GetlParam() );
                        prKey.second = static_cast<int>( m_pkMsgBox->GetlParam2() );
                        std::vector< GCITEMUID > vecUID;

                        if ( g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradeSidePanel )
                        {
                            if( false == g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradeSidePanel->GetUpgradeMaterialUIDSideFirst( prKey, vecUID ) ) { 
                                m_pkMsgBox->Destroy();
                                return;
                            }
                        }

                        Result_BuyForHero = INT_MAX;
                        KP2P::GetInstance()->Send_HeroItemUpgradeReq( prKey, vecUID );
                        g_MyD3D->WaitForServerAck( Result_BuyForHero, INT_MAX, 5000, TIME_OUT_VALUE);

                        if( 0 == Result_BuyForHero ) { 
                            g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradePanel->SetMyHeroCoin();
                            if ( g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradePanel )
                            {
                                g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradePanel->SetMyHeroCoin();
                                g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradeSidePanel->InitUIStateSide();
                                g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradeSidePanel->m_nState = 3;
                                for (int i = 0; i < (int)g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradeSidePanel->m_vecInvenItemImgListSide.size(); ++i) {
                                    g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradeSidePanel->m_vecInvenItemImgListSide[i]->SelectedItem(false);
                                }
                                g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradePanel->UpdateFrame(g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradePanel->m_eCurrentShopUpgradeCategoryType, g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradePanel->m_iCurrentPage);
                            }
                            
                        }
                        m_pkMsgBox->Destroy();
                        
                        return;
                    }
                    break;
                case GC_MBOX_USE_BUY_CASH_ITEM_BY_GC :
                    {
                        g_pkUIScene->m_pkBuyCashItemDlgS6->BuyCurrentItem();

                        m_pkMsgBox->Destroy();
                        return;
                    }
                    break;
                case GC_MBOX_USE_ERRAND_EXIT:
                    {
                        m_pkMsgBox->Destroy();
                        DeleteWaitMsgBox( GC_MBOX_CHALLENGE_ERRAND );
                        g_pkUIScene->m_pkChallengErranDlg->OnClose();
                        g_pkUIScene->m_pkChallengErranDlg->GamExitRewardPacket();
                        return;
                    }
                case GC_MBOX_USE_EXTEND_LOOK_INVENTORY:
                    {
                        Result_ExtendInventory = INT_MAX;
                        GCITEMUID ItemUID = static_cast<GCITEMUID>(m_pkMsgBox->GetlParam());
                        GCITEMID  ItemID = static_cast<GCITEMID>(m_pkMsgBox->GetlParam2());
                        KP2P::GetInstance()->Send_LookInventoryExtendReq(ItemID * 10);
                        g_MyD3D->WaitForServerAck(Result_ExtendInventory, INT_MAX, 5000, TIME_OUT_VALUE);

                        switch (Result_ExtendInventory)
                        {
                            case 0:
                            {
                                int iLookInvenCapacity = g_kGlobalValue.m_iCoordiCapacity;
                                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                                    g_pkStrLoader->GetString(STR_ID_COORDI_EXPANSION_SUCCESS1),
                                    g_pkStrLoader->GetReplacedString(STR_ID_COORDI_EXPANSION_SUCCESS2, "i", iLookInvenCapacity));

                                std::vector<GCITEMUID> vecDecItemUID;
                                g_pItemMgr->SetCoordiInventoryMaxSize(iLookInvenCapacity);
                                g_pItemMgr->DecDurationItem(ItemUID, &vecDecItemUID);
                                g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
                                break;
                            }
                            case 3:
                            {
                                int iMaxCoordiInven = g_kGlobalValue.m_iCoordiCapacity;
                                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                                    g_pkStrLoader->GetReplacedString(STR_ID_CANT_EXPAND_COORDI_INVENTORY, "i", iMaxCoordiInven), L"");
                                break;
                            }
                            case 1:
                            case 2:
                            {
                                std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii",
                                    KUserEvent::EVENT_USE_INVENTORY_EXTEND_ACK, Result_ExtendInventory);

                                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                                    g_pkStrLoader->GetString(STR_ID_EXTEND_INVENTORY_MESSAGE6), strTemp);
                                break;
                            }
                            default:
                            {
                                std::wstring strTemp = g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_DISCONNECTED2);
                                strTemp += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii",
                                    KUserEvent::EVENT_USE_INVENTORY_EXTEND_ACK, Result_ExtendInventory);

                                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                                    g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_DISCONNECTED1), strTemp);
                                break;
                            }
                        }
                        break;
                    }
                case GC_MBOX_USE_TUTORIAL_GUIDE_ENTER:
                    {
                        //튜토리얼 가이드 절차
                        m_pkMsgBox->Destroy();
                        m_pkMsgBox->SetWndState( KD3DWnd::D3DWS_DISABLED );
                        g_pkGameOverlayUI->m_pkAddtionGuide->SetGuideUserState( STATE_PROGRESS_IN_TUTORIAL );
                        g_MyD3D->m_pStateMachine->GoWorldMap();
                        TutorialRoomStart();
                    }
                    break;
                case GC_MBOX_USE_RECHARGE_VITALITY :
                    {
                        m_pkMsgBox->Destroy();
                        Result_ReceiveVitalitySystemRecharge = INT_MAX;
                        SCharInfo* pCharInfo = &g_kGlobalValue.m_kUserInfo.GetCurrentChar();
                        KP2P::GetInstance()->Send_VitalitySystem_Recharge_Req( pCharInfo->iCharType );
                        g_MyD3D->WaitForServerAck( Result_ReceiveVitalitySystemRecharge, INT_MAX, 3000, TIME_OUT_VALUE );
                        return;
                    }
                    break;
				case GC_MBOX_USE_JUMPING_CHAR:
                    {
                        m_pkMsgBox->Destroy();

                        char cChar = static_cast<char>( m_pkMsgBox->GetlParam() );

                        g_pkUIScene->m_pkJumpingCharDlg->SetButtonLock( true );
                        SiKGCJumpingCharManager()->DoJumpingChar( cChar );
                        
                    }
                    break;
                case GC_MBOX_USE_JUMPING_CHAR_REWARD:
                    {
                        m_pkMsgBox->Destroy();    

                        g_pkUIScene->m_pkJumpingCharDlg->SetButtonLock( true );
                        SiKGCJumpingCharManager()->DoJumpingCharReward();
                    }
                    break;
                case GC_MBOX_USE_VIRTUAL_INVEN_MOVEN:
                    {
                        SiKGCVirtualDepotManager()->SendMoveItemData();
                    }
                    break;
                case GC_MBOX_USE_VIRTUAL_INVEN_MOVEN_COMPLATE:
                    {
                        SiKGCVirtualDepotManager()->SendMoveItemData();
                    }
                    break;
                case GC_MBOX_USE_SKILL_SCROOL_ITEM_USE:
                    {
                        int         iSkillID  = static_cast<int>(m_pkMsgBox->GetlParam());
                        GCITEMUID   dwItemUID = static_cast<GCITEMID>(m_pkMsgBox->GetlParam2());

                        Result_ReceiveSkillScroolITemReq = INT_MAX;
                        KP2P::GetInstance()->Send_SkillScrollTrainingReq( iSkillID, dwItemUID );
                        g_MyD3D->WaitForServerAck( Result_ReceiveSkillScroolITemReq, INT_MAX, 5000, TIME_OUT_VALUE);
                    }
                    break;
                case GC_MBOX_UNDO_COORDI_ITEM:
                    {
                        GCITEMUID   dwItemUID = static_cast<GCITEMID>(m_pkMsgBox->GetlParam());
                        GCITEMID BaseID, DesignID;

                        KItem* recItem1 = g_pItemMgr->GetInventoryItem(dwItemUID);
                        BaseID = recItem1->m_ItemID;
                        DesignID = recItem1->m_DesignCoordiID;

                        if (BaseID > 0 && DesignID > 0) {
                            g_kGlobalValue.isUndoCoordi = true;

                            KEVENT_BUNDLE_SELL_ITEM_REQ kPacket;
                            kPacket.m_vecEraseList.clear();
                            kPacket.m_vecSellCountItem.clear();
                            kPacket.m_vecSellNormalItem.push_back(dwItemUID);

                            SiKP2P()->Send_AddItem(DesignID * 10, -1, -1, -1, -1);
                            g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
                            g_pkUIScene->m_pkMyInfoScene->UpdateData();

                            SiKP2P()->Send_AddItem(BaseID * 10, -1, -1, -1, -1);
                            g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
                            g_pkUIScene->m_pkMyInfoScene->UpdateData();

                            SiKP2P()->Send_BundleSellReq(kPacket);

                            g_MyD3D->m_TempPlayer.RemovePlayerObject();
                            g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
                            g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();
                            g_pkUIScene->m_pkMyInfoScene->UpdateGameData(false, UEC_LOOK, 0);

                            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_UNDO_COORDI_SUCCESS),
                                L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false);
                        }
                    }
                    break;
                case GC_MBOX_COORDIVIEW_SAVE_SLOT:
                    {
                        m_pkMsgBox->Destroy();
                        KP2P::GetInstance()->Send_CoordiViewSlotSetReq(
                            g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType,
                            g_pkUIScene->m_pkCoordiViewDlg->m_iCurrentSlot,
                            g_kGlobalValue.m_kUserInfo.dwUID,
                            g_pkUIScene->m_pkCoordiViewDlg->m_vecCurrentEquipedItems
                        );
                    }
                break;
                case GC_MBOX_USE_BUY_FONTSKIN:
                    {
                        BuyFontSkinForCrystal(static_cast<GCITEMID>(m_pkMsgBox->GetlParam()), static_cast<int>(m_pkMsgBox->GetlParam2()), KItem::UNLIMITED_ITEM);
                        m_pkCashShopScene->UpdateUserInfo();
#ifndef DISABLE_MAP_SHOP
                        m_pkGPShopScene->UpdateUserInfo();
#endif
                    }
                break;
                case GC_MBOX_USE_FORGE_CONFIRM:
                    {
                        m_pkForgeDlg->m_pkCraftMachine->m_bBypassForgeVerify = false;
                        m_pkForgeDlg->m_pkCraftMachine->OnClickStartBtnCreate();
                    }
                break;
                case GC_MBOX_USE_BREAKUP_CONFIRM:
                    {
                        GCITEMUID iItemUID = m_pkMsgBox->GetlParam();
                        GCITEMID iItemID = m_pkMsgBox->GetlParam2();

                        SiGCForgeManager()->m_setWaitItemList.insert(iItemUID);
                        SiGCForgeManager()->m_vecWaitItemList.push_back(std::pair<GCITEMUID, GCITEMID>(iItemUID, iItemID));

                        m_pkForgeDlg->m_pkItemBreakUp->RefreshItemList();
                        m_pkForgeDlg->m_pkItemBreakUp->SetItemList();
                        m_pkForgeDlg->m_pkItemBreakUp->SpeakToActionListener(KActionEvent(NULL, KGCUIScene::D3DWE_FORGE_CLICK_BREAKEUP_ITEM, iItemUID, iItemID));
                    }
                break;
                case GC_MBOX_USE_COMPOSE_CONFIRM:
                {
                    int iOptionID = m_pkMsgBox->GetlParam();
                    m_pkCoordiComposeDlg->OnInvenItemList( iOptionID, true );
                }
                break;
#ifdef FEATURE_2021
                case GC_MBOX_USE_KAIRO_ONESHOT:
                    {
                        m_pkMsgBox->Destroy();

                        GCITEMUID iItemUID = m_pkMsgBox->GetlParam();
                        int iDuration = static_cast<int>(m_pkMsgBox->GetlParam2());

                        g_kGlobalValue.m_pkCurrentUseItemID = 0;
                        g_kGlobalValue.m_bIsCurrentUsechest = false;

                        Result_Kairo_Oneshot = INT_MAX;
                        KP2P::GetInstance()->Send_ChangeItemOneShotReq(iDuration, iItemUID);
                        g_MyD3D->WaitForServerAck(Result_Kairo_Oneshot, INT_MAX, 5000, TIME_OUT_VALUE);

                        if (Result_Kairo_Oneshot == -99)
                        {
                            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1), L"");
                        }
                    }
                break;
#endif
                case GC_MBOX_USE_KAIRO:
                    {
                        m_pkMsgBox->Destroy();

                        Result_Kid_Gift = -100;
                        KP2P::GetInstance()->Send_ChangeItemReq(m_pkMsgBox->GetlParam());
                        g_MyD3D->WaitForServerAck(Result_Kid_Gift, -100, 15000, -99);

                        if (Result_Kid_Gift == -99)
                        {
                            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1), L"");
                        }
                        break;
                    }
                break;
				default:
					break;
                }
                m_pkMsgBox->Destroy();
                g_KDSound.Play( "32" );
                break;
            }
        case KGCMessageBox::MBR_NO: // '아니오'를 눌렀을 경우
            {
                switch ( m_pkMsgBox->GetUse() )
                {
                case GC_MBOX_USE_RECOMMEND_NOT:
                    {
                        m_pkMsgBox->Destroy();
                        m_pkNickNameBox->SetMsgBox();
                        break;
                    }
                case GC_MBOX_USE_NICK_NAME_AGREE: // 닉네임 비승인
                    {
                        MessageBox( GC_MBOX_NICK_NAME, L"", L"", GC_MBOX_USE_NORMAL); // 닉네임을 다시 물어본다.
                        m_pkMsgBox->Destroy();
                        break;
                    }
                case GC_MBOX_USE_NEONSIGN_RESERVE:
                    {
                        m_pkNeonsignPreview->WaitForServer( false );
                        m_pkMsgBox->Destroy();
                        break;
                    }
                case GC_MBOX_USE_TUTORIAL_GUIDE_ENTER:
                    {
                        m_pkMsgBox->Destroy();
                        m_pkMsgBox->SetWndState( KD3DWnd::D3DWS_DISABLED );
                        g_pkGameOverlayUI->m_pkAddtionGuide->SetGuideUserState( STATE_TUTORIAL_END );
                        g_MyD3D->m_pStateMachine->GoWorldMap();
                        break;
                    }
                case GC_MBOX_USE_VIRTUAL_INVEN_MOVEN_COMPLATE:
                    {
                        m_pkMsgBox->Destroy();
                        DeleteWaitMsgBox( GC_MBOX_VIRTUALDEPOT );
                        g_pkUIScene->m_pkVirtualDepotDlg->OnClose();
                    }
                    break;
                case GC_MBOX_USE_KAIRO_ONESHOT:
                    {
                        m_pkMsgBox->Destroy();
                    }
                    break;
                default:
                    m_pkMsgBox->Destroy();
                }                
                g_KDSound.Play( "32" );
                break;
            }
        case KGCMessageBox::MBR_OK: // '확인'을 눌렀을 겨우
            {
				m_pkMsgBox->Destroy();
				g_KDSound.Play( "32" );

				switch( m_pkMsgBox->GetUse() )
				{
				case GC_MBOX_GIFTS_OK:
					{
						EndMessageBox();
						break;
					}
				case GC_MBOX_USE_END_GAME:
					{
                        if( !SiKGCWorldMapManager()->IsHeroDungeonMode() )
                        {
                            g_pStateGame->EndGame();
                        }						
						g_MyD3D->m_pStateMachine->ExitRoom( true );
						break;
					}
				case GC_MBOX_DETECT_CHANGED_KOM_OK:
					{
						g_MyD3D->m_pStateMachine->GoServer();
						SiKGCRoomManager()->SetCreateToJoin( false );
						break;
					}
                case GC_MBOX_USE_VIRTUAL_DEOPT_EXIT:
                    if( SiKGCVirtualDepotManager()->GetClossDlg() == true )
                    {
                        SiKGCVirtualDepotManager()->SetCloseDlg( false );
                        g_pkUIScene->m_pkVirtualDepotDlg->OnClose();
                        DeleteAllWaitMsgBox();
                    }
                    break;
				}
                if( EGCMsgBoxUse(GC_MBOX_USE_DOTA_GAME) == m_kCurrMsgBoxInfo.m_eMsgBoxUse )
                    m_kCurrMsgBoxInfo.m_eMsgBoxUse = EGCMsgBoxUse(GC_MBOX_USE_MAX);

                break;
            }
        case KGCMessageBox::MBR_DESTROYED: // 일반 메세지 박스가 사라진 후
            {
                if ( m_pkMsgBox->GetUse() == GC_MBOX_USE_MINIGAME_EXIT && true == m_bExitMinigame )
                {
                   //KP2P::GetInstance()->Send_ExitMiniGameReq();
				   g_pkGameOverlayUI->GetMinigameResult()->ToggleRender( false );
                   m_bExitMinigame = false;
                   SiKGCMinigameManager()->SetExitGame( true );
                }

                EndMessageBox();
                break;
            }

		case KGCMessageBox::MBR_CHACK:
			{
				m_pkMsgBox->Destroy();
				g_KDSound.Play( "32" );
				if ( m_pkMsgBox->GetlParam() == KGCMessageBox::MBSE_D3DWE_AGREEMENT_OK)
					g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_AGREEMENT_OK ) );

				EndMessageBox();
				break;

			}
		case KGCMessageBox::MBR_CANSEL:
			{
				m_pkMsgBox->Destroy();
				g_KDSound.Play( "32" );
				if ( m_pkMsgBox->GetlParam() == KGCMessageBox::MBSE_D3DWE_AGREEMENT_OK)
					g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_AGREEMENT_OK ) );

				EndMessageBox();
				break;				
			}
		case KGCMessageBox::MBR_JOIN_USER:
			{
				m_pkMsgBox->Destroy();
				g_KDSound.Play( "32" );

				EndMessageBox();
				PopUpWebPage( L"http://member.changyou.com/register/regPhoneSmAction.do?gametype=PE-GC" );
				break;				
			}
		case KGCMessageBox::MBR_USER_INFO:
			{
				m_pkMsgBox->Destroy();
				g_KDSound.Play( "32" );

				EndMessageBox();
				PopUpWebPage( L"http://account.changyou.com/fangchenmi/submitlogin.jsp" );
				break;				

			}

        }
    }
    else if ( event.m_pWnd == m_pkPasswdBox ) // 패스워드 박스일 경우
    {
        switch ( event.m_dwCode )
        {
        case KGCPasswordBox::PBR_OK:
            {
                m_pkPasswdBox->Destroy();
                g_KDSound.Play( "32" );
                m_bConnect = true;
                break;
            }
        case KGCPasswordBox::PBR_CANCEL:
            {
                m_pkPasswdBox->Destroy();
                g_KDSound.Play( "32" );
                m_bConnect = false;
                break;
            }
        case KGCPasswordBox::PBR_DESTROYED:
            {
                EndMessageBox();
                switch ( m_pkPasswdBox->GetUse() )
                {
                case GC_MBOX_USE_INVITE_AGIT_INPUT_PASSWORD:                    
                    Result_EnterAgitReq = INT_MAX;
                    KP2P::GetInstance()->Send_EnterAgitReq( m_pkPasswdBox->GetlParam(), g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType, m_pkPasswdBox->GetPassword());
                    g_MyD3D->WaitForServerAck( Result_EnterAgitReq, INT_MAX, 5000, TIME_OUT_VALUE );
                    break;
                default:                    
                if ( m_bConnect )
                {
                    SiKGCRoomManager()->JoinRoom(   KJoinRoomReqInfo::JOIN_NORMAL, 
                                                    SiKGCRoomManager()->GetRoomID(), 
                                                    m_pkPasswdBox->GetPassword(),
                                                    SiKGCRoomManager()->IsChannelRoomList() );
                }
                break;
            }
                break;
        }
    }
    }
    //else if ( event.m_pWnd == m_pkInviteReplayBox ) // 초대 박스일 경우
    //{
    //    switch ( event.m_dwCode )
    //    {
    //        case KGCInviteReplayBox::IRR_CLOSE:
    //        {
    //            m_pkInviteReplayBox->Destroy();
    //            g_KDSound.Play( "32" );
    //            g_pkRoomChatBox->SetFocus();
    //            break;
    //        }
    //        case KGCInviteReplayBox::IRR_DESTROYED:
    //        {
    //            EndMessageBox();
    //            break;
    //        }
    //    }
    //}
    else if ( event.m_pWnd == m_pkLoadingBox ) //로딩 박스일 경우 (취소는 없다; 무조건 다될때까지 기다린다)
    {
        switch ( event.m_dwCode )
        {
        case KGCLoadingBox::LBR_SUCCESS:
            {
                m_bRPLoadSuccess =  true;
                m_pkLoadingBox->Destroy();
                g_KDSound.Play( "32" );
                break;
            }
        case KGCLoadingBox::LBR_FAILED:
            {
                m_bRPLoadSuccess = false;
                m_pkLoadingBox->Destroy();
                g_KDSound.Play( "32" );

                // 리플레이 로딩에 실패했으므로 게임모드를 새로 세팅해줍니다.
                SiKGCRoomManager()->UpdateShowGameModeCategory( true, SiKGCChannelManager()->GetCurrentChannelType() );
                SiKGCRoomManager()->InitGameMode();
                break;
            }
        case KGCLoadingBox::LBR_DESTROYED:
            {
                EndMessageBox();
                if ( m_bRPLoadSuccess )
                {
                    g_MyD3D->m_pStateMachine->GoGame();
                    g_MyD3D->PlayGameBGM(); //음악 선정한다.
                }
                else
                {
                    g_MyD3D->m_pStateMachine->GoServer();
                    //실패 메세지.
                    MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_LOADING ));
                }
                break;
            }
        }
    }
    else if (event.m_pWnd == m_pkBonusChargeBox) // 보너스 충전 박스인 경우
    {
        switch (event.m_dwCode)
        {
            case KGCGemBuyBox::GBB_YES:
            {
                KItem* pInvenItem = NULL;

                if (g_pItemMgr->FindInventory(m_pkBonusChargeBox->GetChargingItem(), &pInvenItem))
                    ChargeBonus(pInvenItem, m_pkBonusChargeBox->GetChargeFactor());

                m_pkBonusChargeBox->Destroy();
                g_KDSound.Play("32");
                break;
            }
            case KGCGemBuyBox::GBB_NO:
            {
                m_pkBonusChargeBox->Destroy();
                g_KDSound.Play("32");
                break;
            }
            case KGCGemBuyBox::GBB_DESTROYED:
            {
                EndMessageBox();
                break;
            }
        }
    }
    else if ( event.m_pWnd == m_pkNickNameBox ) // 닉네임 박스일 경우
    {
        switch ( event.m_dwCode )
        {
        case KGCNickNameBox::NBR_DESTROYED:
            {
                EndMessageBox();
                break;
            }
        }
    }
    else if ( event.m_pWnd == m_pBuyCashItemWnd ) // 캐쉬 아이템 구매 박스
    {
        switch( event.m_dwCode )
        {
        case KGCBuyCashItemWnd::ID_CLOSE:
            {
                m_pBuyCashItemWnd->ToggleRender( false );
                EndMessageBox();

//				if( SiGCSKT()->GetBuyingReqGroup() != -1 )
//				{
//					//정체를 알수 없는 코드....... 뭔가 웹구매쪽도 얽힌듯 하고..-_-
//#if defined(NATION_THAILAND)
//					SiGCSKT()->Process_UnlockGroupItem( SiGCSKT()->GetBuyingReqGroup(), true );
//#else
//					// 대만 홍콩은 구매시 기본적으로 무한대기 이므로 이부분이 필요 없을 것 같다.
//					#if !defined( NATION_TAIWAN ) && !defined( NATION_HONGKONG ) && !defined(NATION_PHILIPPINE)
//					if( true == g_MyD3D->WaitForServerAck( Result_Buy, 0, 300000, 2 ) )
//					#endif
//						SiGCSKT()->Process_UnlockGroupItem( SiGCSKT()->GetBuyingReqGroup(), true );
//#endif
//				}
                break;
            }
        }
    }
    else if( event.m_pWnd == m_pkColorDlg )
    {
        if( m_pkLastColorItemUse != NULL )
        {
            if( event.m_dwCode == GC_MBOX_RETURN_OK )
            {
                m_pkLastColorItemUse->SetColor( m_pkColorDlg->GetSelectColor() );
                event.m_pWnd->Destroy();
                EndMessageBox();
            }
            else
            {
                event.m_pWnd->Destroy();
                EndMessageBox();
            }
            m_pkLastColorItemUse->SetFocus();

            m_pkLastColorItemUse = NULL;
            return;
        }
    }
    else if ( event.m_pWnd == m_pkEventBox ) // 신학기 이벤트 박스
    {
        switch ( event.m_dwCode )
        {
        case GC_MBOX_RETURN_CANCEL:
            {
                m_pkEventBox->Destroy();
                g_KDSound.Play( "32" );
                break;
            }
        case GC_MBOX_USE_EVENT_MSG:
            {
                m_pkEventBox->Destroy();
                g_KDSound.Play( "32" );
                break;
            }
        case GC_MBOX_USE_EVENT_RECOMMAND:
            {
                m_pkEventBox->Destroy();
                g_KDSound.Play( "32" );
                break;
            }
        case KGCEventBox::MBR_DESTROYED:
            {
                EndMessageBox();
                break;
            }
        }
    }
    else if( event.m_pWnd == m_pkRepetitionEventBox ) // 한시간 이벤트 박스
    {
        switch( event.m_dwCode )
        {
        case GC_MBOX_RETURN_OK:
            {
                g_KDSound.Play( "32" );
                m_pkRepetitionEventBox->Destroy();
                EndMessageBox();
            }
        }
    }
    else if( event.m_pWnd == m_pkPetEvolutionBox ) // 펫진화박스
    {
        switch( event.m_dwCode )
        {
        case KGCPetEvolutionBox::CURRENT_PET_PREVIEW:
            {
                MessageBox( KGCUIScene::GC_PET_PREVIEW, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                m_pkPetPreview->SetMyInfoPreview();
                m_pkPetPreview->SetDescState( 2 );

                m_pkPetPreview->SetNowPet( m_pkPetEvolutionBox->GetCurrentPetID(), m_pkPetEvolutionBox->GetCurrentPetEvolutionLevel() );
                m_pkPetPreview->m_pkPetMsg->SetMentState( PET_MENT_FIRST_MEETING );

                break;
            }
        case KGCPetEvolutionBox::NEXT_PET_PREVIEW:
            {
                MessageBox( KGCUIScene::GC_PET_PREVIEW, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                m_pkPetPreview->SetMyInfoPreview();
                m_pkPetPreview->SetDescState( 2 );

                g_pkUIScene->m_pkPetPreview->SetNowPet( m_pkPetEvolutionBox->GetNextPetID() , m_pkPetEvolutionBox->GetNextPetEvolutionLevel() );
                g_pkUIScene->m_pkPetPreview->m_pkPetMsg->SetMentState( PET_MENT_FIRST_MEETING );

                break;
            }
        case KGCPetEvolutionBox::TRANSFORMATION:
            {
                Result_TransformationPet = INT_MAX;

                GCITEMUID dwPetUID = m_pkPetEvolutionBox->GetPetUID();
                int iPetTransformationType = m_pkPetEvolutionBox->GetEvolutionType(); // 0이진화, 1이 퇴화 이다
                DWORD dwUseItemID = m_pkPetEvolutionBox->GetUseItemID();

                KP2P::GetInstance()->Send_CheckTransformationPetReq( dwPetUID, dwUseItemID, iPetTransformationType );

                g_MyD3D->WaitForServerAck( Result_TransformationPet, INT_MAX, 10000, 9999 );

                // 0 - 성공
                // 1 - 펫 정보가 존재하지 않음
                // 2 - 펫 이 진화하기에 레벨이 낮다.
                // 3 - 펫 진화 카드를 가지고 있지 않거나 부족함.
                // 4 - 이름 정보가 없습니다.
                // 5 - 펫 전직 타입이 올바르지 않음.
                // 6 - 잘못된 진화/퇴화 아이템 정보를 보냈음.

                switch( Result_TransformationPet )
                {
                case 0:
                    g_KDSound.Play( "32" );
                    m_pkPetEvolutionBox->Destroy();
                    m_pkPetEvolutionBox->Release();
                    break;
                case 1: // 펫 정보가 없습니다._게임을 다시 실행 시킨 뒤 시도해 주세요.
                    MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PET_EVOLUTION_MESSAGE4 ),
                        L"", GC_MBOX_USE_NORMAL, 0, 0, false, true );
                    break;
                case 2: // 펫의 레벨이 낮아서 진화를 할 수 없습니다._펫의 레벨을 올려주세요^-^
                    MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PET_EVOLUTION_MESSAGE5 ),
                        L"", GC_MBOX_USE_NORMAL, 0, 0, false, true );
                    break;
                case 3:
                    switch( iPetTransformationType )
                    {
                    case 0: // 펫 진화 마법서를 가지고 있지_않습니다.
                        MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PET_EVOLUTION_MESSAGE6 ),
                            L"", GC_MBOX_USE_NORMAL, 0, 0, false, true );
                        break;
                    case 1: // 펫 퇴화 마법서를 가지고 있지_않습니다.
                        MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PET_EVOLUTION_MESSAGE10 ),
                            L"", GC_MBOX_USE_NORMAL, 0, 0, false, true );
                        break;
                    }
                    break;
                case 4:
                case 5:
                case 6:
                    switch( iPetTransformationType )
                    {
                    case 0: // 펫을 진화 시키지 못했습니다._게임을 다시 실행 시킨 뒤 시도해 주세요.
                        MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PET_EVOLUTION_MESSAGE7 ),
                            L"", GC_MBOX_USE_NORMAL, 0, 0, false, true );
                        break;
                    case 1: // 펫을 퇴화 시키지 못했습니다._게임을 다시 실행 시킨 뒤 시도해 주세요.
                        MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PET_EVOLUTION_MESSAGE11 ),
                            L"", GC_MBOX_USE_NORMAL, 0, 0, false, true );
                        break;
                    }
                    break;
                default: // 타임아웃 - 클라이언트 종료시키자

                    std::wstring strTemp = g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE2 );
                    strTemp += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", 
                        KUserEvent::EVENT_CHECK_TRANSFORMATION_PET_ACK, Result_TransformationPet );	

                    MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1 ),
                        strTemp, GC_MBOX_USE_NORMAL, 0, 0, false, true );

                    break;
                }

                break;
            }
        case KGCPetEvolutionBox::HATCH:
            {
                Result_TransformationPet = INT_MAX;

                GCITEMUID dwEggPetUID = m_pkPetEvolutionBox->GetPetUID();
                KP2P::GetInstance()->Send_HatchingEggReq( dwEggPetUID );
                g_MyD3D->WaitForServerAck( Result_TransformationPet, INT_MAX, 10000, 9999 );

                //  0   성공
                //  1   펫 정보가 존재하지 않음
                //  2   알 펫이 아니라서 부화 불가능.
                //  3   부화 대상 정보가 존재하지 않음
                //  4   아이템을 보유하고 있지 않음.
                //  5   아이템 수량이 부족.
                //  6   부화 하기에 레벨이 부족함. 
                //  7   해당 펫 아이템을 가지고 있지 않음
                //  8   해당 유저가 존재하지 않음
                //  9   펫 아이템이 아님..
                //  10  펫 아이템(시민권)이 없음
                //  11  부화시 새펫 봉인 카드 넣기 실패.
                //  12  신규 생성하려는 팻이 이미 존재함
                //  13  해당 캐릭터가 존재하지 않음.
                switch( Result_TransformationPet )
                {
                case 0:
                    g_KDSound.Play( "32" );
                    m_pkPetEvolutionBox->Destroy();
                    m_pkPetEvolutionBox->Release();
                    break;

                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                    break;

                default: // 타임아웃 - 클라이언트 종료시키자

                    std::wstring strTemp = g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE2 );
                    strTemp += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", 
                        KUserEvent::EVENT_HATCHING_EGG_ACK, Result_TransformationPet );	

                    MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1 ),
                        strTemp, GC_MBOX_USE_NORMAL, 0, 0, false, true );
                    break;
                }

                break;
            }
        case KGCPetEvolutionBox::EXIT:
            {
                g_KDSound.Play( "32" );
                m_pkPetEvolutionBox->Release();
                m_pkPetEvolutionBox->Destroy();
                break;
            }
        case KGCPetEvolutionBox::DESTROYED:
            {
                EndMessageBox();
                break;
            }
        }
    }
    else if( event.m_pWnd == m_pkSelectPetNameBox ) // 펫이름선택
    {
        switch( event.m_dwCode )
        {
        case KGCSelectPetNameBox::TRANSFORM_SELECTNAME:
            {
                Result_TransformationPet = INT_MAX;

                GCITEMUID dwPetUID = m_pkSelectPetNameBox->GetPetUID();
                std::wstring strPetName = m_pkSelectPetNameBox->GetSelectedPetName();
                DWORD dwUseItemID = m_pkSelectPetNameBox->GetUseItemID();;
                int iPetTransformationType = m_pkSelectPetNameBox->GetEvolutionType(); // 0 - 진화, 1 - 퇴화

                KP2P::GetInstance()->Send_TransformationPetReq( dwPetUID, strPetName, dwUseItemID, iPetTransformationType );

                g_MyD3D->WaitForServerAck( Result_TransformationPet, INT_MAX, 10000, 9999 );

                // 0 - 성공
                // 1 - 펫 정보가 존재하지 않음
                // 2 - 펫 이 진화하기에 레벨이 낮다.
                // 3 - 펫 진화 카드를 가지고 있지 않거나 부족함.
                // 4 - 이름 정보가 없습니다.
                // 5 - 이름 정보가 옳지 못함.
                // 6 - 아이템 구매자 존재하지 않음
                // 7 - 구매할수 없는 아이템.. 존재하지 않음. 비매품. 캐쉬아이템아님. 패턴 아이템.
                // 8 - 아이템 구매 시도 실패
                // 9 - 펫 전직 타입이 올바르지 않음.
                // 10 - 펫 전직 레벨 오류
                // 11 - 잘못된 진화/퇴화 아이템 정보를 보냈음.
                // 12 - 펫 전직 레벨 정보 가져오기 실패

                switch( Result_TransformationPet )
                {
                case 0: // 성공
                    break;
                case 1: // 펫 정보가 없습니다._게임을 다시 실행 시킨 뒤 시도해 주세요.
                    MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PET_EVOLUTION_MESSAGE4 ) );
                    break;
                case 2: // 펫의 레벨이 낮아서 진화를 할 수 없습니다._펫의 레벨을 올려주세요^-^
                    MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PET_EVOLUTION_MESSAGE5 ) );
                    break;
                case 3:
                    switch( iPetTransformationType )
                    {
                    case 0: // 펫 진화 마법서를 가지고 있지_않습니다.
                        MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PET_EVOLUTION_MESSAGE6 ) );
                        break;
                    case 1: // 펫 퇴화 마법서를 가지고 있지_않습니다.
                        MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PET_EVOLUTION_MESSAGE10 ) );
                        break;
                    }
                    break;
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                    switch( iPetTransformationType )
                    {
                    case 0: // 펫을 진화 시키지 못했습니다._게임을 다시 실행 시킨 뒤 시도해 주세요.
                        MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PET_EVOLUTION_MESSAGE7 ) );
                        break;
                    case 1: // 펫을 퇴화 시키지 못했습니다._게임을 다시 실행 시킨 뒤 시도해 주세요.
                        MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PET_EVOLUTION_MESSAGE11 ) );
                        break;
                    }    
                    break;
                default: // 타임아웃 - 클라이언트 종료시키자
                    std::wstring strTemp = g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE2 );
                    strTemp += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", 
                        KUserEvent::EVENT_TRANSFORMATION_PET_ACK, Result_TransformationPet );	

                    MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1 ), strTemp );

                    break;
                }

                g_KDSound.Play( "32" );
                m_pkSelectPetNameBox->Release();
                m_pkSelectPetNameBox->Destroy();
                break;
            }
        case KGCSelectPetNameBox::EXIT:
            {
                g_KDSound.Play( "32" );
                m_pkSelectPetNameBox->Release();
                m_pkSelectPetNameBox->Destroy();
                break;
            }
        case KGCSelectPetNameBox::DESTROYED:
            {
                EndMessageBox();
                break;
            }
        }
    }
    else if( event.m_pWnd == m_pkNewTutorialOption )	// 튜토리얼 옵션
    {
        switch( event.m_dwCode )
        {
        case KGCNewTutorialOption::CANCLE:
            {
                g_KDSound.Play( "32" );
                m_pkNewTutorialOption->Destroy();
                EndMessageBox();
                break;
            }
        case KGCNewTutorialOption::STARTTUTORIAL:
            {
                m_pkNewTutorialOption->Destroy();
                EndMessageBox();
                break;
            }
        }
    }
    else if( event.m_pWnd == m_pkAttendanceCalendar ) // 달력
    {
        switch ( event.m_dwCode )
        {
        case KGCAttendanceCalendar::EXIT:
            {
                m_pkAttendanceCalendar->Destroy();
                g_KDSound.Play( "32" );
                break;
            }
        case KGCAttendanceCalendar::DESTROYED:
            {
                EndMessageBox();
                break;
            }
        case KGCAttendanceCalendar::PERFECT_ATTENDANCE:
            {
                MessageBox( KGCUIScene::GC_MBOX_PERFECT_ATTENDANCE, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                break;
            }
        }
    }
    else if( event.m_pWnd == m_pkPerfectAttendance ) // 월 개근상
    {
        switch ( event.m_dwCode )
        {
        case KGCPerfectAttendanceBox::SELECT_REWARD_ITEM:
            {
                int iItemID = m_pkPerfectAttendance->m_iGetItem->dwGoodsID;

                Result_GetRewardItem = INT_MAX;

				if( m_pkPerfectAttendance->GetRewardType() == 0 )
				{
					KSimpleDate kDate = g_kGlobalValue.m_vecReservedReward[0];
					KItem kItem = g_MyD3D->m_kItemMgr.GetPerfectMonthItem( iItemID );

					KP2P::GetInstance()->Send_MonthlyAttendRewardReq( kDate, kItem );                    
					g_MyD3D->WaitForServerAck( Result_GetRewardItem, INT_MAX, 5000, TIME_OUT_VALUE );
				}
				else
				{
					KItem kItemUnit;
					kItemUnit.m_ItemID = 0;

                    std::vector<KItem> vecItemList = g_MyD3D->m_kItemMgr.GetPerfectWeekItemList();
					for( int i = 0; i < (int)vecItemList.size(); i++ )
					{
						if( vecItemList[i].m_ItemID == iItemID )
						{
							kItemUnit = vecItemList[i];

						}
					}                    
					KP2P::GetInstance()->Send_WeeklyAttendReq( m_pkPerfectAttendance->GetRewardType() ,kItemUnit.m_ItemID );
					g_MyD3D->WaitForServerAck( Result_GetRewardItem, INT_MAX, 5000, TIME_OUT_VALUE );
				}

                switch( Result_GetRewardItem )
                {
                case 0:
                    {
                        break;
                    }
                case -1:
                case 2:
                case 3:
                case 4:
                case 5:
                default:
                    {
                        std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", 
                            KUserEvent::EVENT_WEEKLY_ATTEND_ACK, Result_GetRewardItem );	
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_GET_REWARD_FAIL ), strTemp,
                            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false );
                        break;
                    }
                }

                m_pkPerfectAttendance->Destroy();
                g_KDSound.Play( "32" );
                break;
            }
        case KGCPerfectAttendanceBox::EXIT:
            {
                m_pkPerfectAttendance->Destroy();
                g_KDSound.Play( "32" );
                break;
            }
        case KGCPerfectAttendanceBox::DESTROYED:
            {
                EndMessageBox();
                break;
            }
        }
    }
    else if( event.m_pWnd == m_pkFairPlayReport )
    {
        switch( event.m_dwCode )
        {
        case KGCUIScene::GC_MBOX_RETURN_CANCEL:
            {
                SiGCPairPlaySystem()->ClearReportDatas();
                m_pkFairPlayReport->Destroy();
                EndMessageBox();
                break;
            }
        case KGCUIScene::GC_MBOX_RETURN_OK:
            {
                m_pkFairPlayReport->Destroy();
                EndMessageBox();
                break;
            }
        }

        if ( g_pkGameOverlayUI != NULL )
        {
            if ( false == g_pkGameOverlayUI->HasEditFocus() )
            {
                g_pkGameOverlayUI->HandleChatEvent();
            }
        }
        ::SetFocus( g_hUIMainWnd );
    }
    else if( event.m_pWnd == m_pkGradeBox )
    {
        switch( event.m_dwCode )
        {
        case KD3DWnd::D3DWE_BUTTON_CLICK:
            {
                g_KDSound.Play( "32" );
                event.m_pWnd->Destroy();
                break;
            }
        case KD3DWnd::EWNDMESSAGE_DESTROYED:
            {
                EndMessageBox();
                break;
            }
        }
    }
	else if( event.m_pWnd == m_pkSkillTree )
	{
		switch( event.m_dwCode )
		{
		case GC_MBOX_RETURN_OK:
            if( m_pkNewTutorialOption != NULL )
            {
                m_pkNewTutorialOption->SetSkillTreeState( false );
            }
			event.m_pWnd->Destroy();
			EndMessageBox();
			break;
		}
	}
    // 2007.11.15 wony
    // 바로 없어지는게 아니고 알파 빠지면서 스르륵 없어지는 녀석들..
    // UI Script에서 move_gap 값이 있는 녀석들..
    else if( event.m_pWnd == m_pkOption ||
             event.m_pWnd == m_pkItemReceiveBox ||
             event.m_pWnd == m_pkLuckyDayReceiveBox ||
             event.m_pWnd == m_pkGradeBox ||
             event.m_pWnd == m_pkChannelListBox ||
             //event.m_pWnd == m_pkDungeonRoomlist ||
             event.m_pWnd == m_pkInviteRequestBox ||
             event.m_pWnd == m_pkInviteBox ||
             event.m_pWnd == m_pkNeonsignSelectBox ||
             event.m_pWnd == m_pkNeonsignPreview ||
			 event.m_pWnd == m_pkTreaureItemBox ||
   			 event.m_pWnd == m_pkConnectionMsg ||
             event.m_pWnd == m_pkWelcomeBackDlg ||
             event.m_pWnd == m_pkMiniGameDlg ||
			 event.m_pWnd == m_pkNateOn ||
			 event.m_pWnd == m_pkNateOnMessage ||
			 event.m_pWnd == m_pkNateOnInvite ||
			 event.m_pWnd == m_pkGuildSquareNotice ||
             event.m_pWnd == m_pkGuildShop ||
             event.m_pWnd == m_pkGuildLevelListDlg ||
             event.m_pWnd == m_pkPartyInviteMsgBox || 
             event.m_pWnd == m_pkWelcomeBackNewDlg ||
             event.m_pWnd == m_pkSquareEnterWndS6
			 )
    {
        switch ( event.m_dwCode )
        {
            // X버튼이나 창을 닫을 때.. EWNDMESSAGE_CLOSE 메세지를 던져준다
        case KD3DWnd::EWNDMESSAGE_CLOSE:
            {
                event.m_pWnd->Destroy();
                g_KDSound.Play( "32" );
                break;
            }
            // OnDestroyComplete 에서 EWNDMESSAGE_DESTROYED를 보낸다
        case KD3DWnd::EWNDMESSAGE_DESTROYED:
            {
                EndMessageBox();
                if ( SiKGCRoomManager()->GetDungeonLevel() == GC_DUNGEON_LEVEL_HELL && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON && g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
                {
                    if( !SiKGCRoomManager()->CanPlayHellmode() )
                    {           
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_TOOLTIP_YOU_NEED_HELL_TICKET1 ), g_pkStrLoader->GetString( STR_ID_TOOLTIP_YOU_NEED_HELL_TICKET2 ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false);

	                    g_MyD3D->m_pStateMachine->ExitRoom( true, KFailRate::P2P_NO_ERROR, false );
                    }
                }
                break;
            }
        }
#if defined( USE_SURVEY_SYSTEM )
        if( event.m_pWnd == m_pkItemReceiveBox ) {
            if ( g_pkUIScene->m_pkSurveyDlg->IsReserveQuestion() ) {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SURVEY_DLG );
            }
        }
#endif
    }
    else if ( event.m_pWnd == m_pkEmoticonCollection ||
              event.m_pWnd == m_pkTitleCollection ||
              event.m_pWnd == m_pkPetCollection
              )
    {
        switch ( event.m_dwCode )
        {
            // X버튼이나 창을 닫을 때.. EWNDMESSAGE_CLOSE 메세지를 던져준다
            case KD3DWnd::EWNDMESSAGE_CLOSE:
            {
                CloseCollectionWindow();
                g_KDSound.Play( "32" );
                break;
            }
            // OnDestroyComplete 에서 EWNDMESSAGE_DESTROYED를 보낸다
            case KD3DWnd::EWNDMESSAGE_DESTROYED:
            {
                EndMessageBox();
                break;
            }
            case GC_MBOX_TITLE_COLLECTION:
                SelectCollection( GC_MBOX_TITLE_COLLECTION );
                break;
            case GC_MBOX_EMOTICON_COLLECTION:
                SelectCollection( GC_MBOX_EMOTICON_COLLECTION );
                break;
            case GC_MBOX_PET_COLLECTION:
                SelectCollection( GC_MBOX_PET_COLLECTION );
                break;
        }
    }
    else if( event.m_pWnd == m_pkCTConditionWindow )
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkCTConditionWindow->Destroy();
            //EndMessageBox();
        }
    }
    else if ( event.m_pWnd == m_pkRanking )
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkRanking->Destroy();            
        }
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
        {
            EndMessageBox();
        }
        
    }
    else if ( event.m_pWnd == m_pkAskUseVirtualCash )
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
//            m_pkAskUseVirtualCash->ToggleRender( false );
            m_pkAskUseVirtualCash->Destroy();
            EndMessageBox();
        }
    }
    else if( event.m_pWnd == m_pkNewClearSealDlg)
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkNewClearSealDlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }
    else if( event.m_pWnd == m_pkCoordiClearSealDlg)
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkCoordiClearSealDlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }
    else if( event.m_pWnd == m_pkNateOnInputDlg )
    {
        if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN )
        {
            m_pkNateOnInputDlg->Destroy();
            g_KDSound.Play( "32" );
            m_pkNateOnInputDlg->SetReserveAction( true );
        }
        else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkNateOnInputDlg->SetReserveAction( false );
            m_pkNateOnInputDlg->Destroy();
            g_KDSound.Play( "32" );
        }
        else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
        {
            EndMessageBox();

            if( m_pkNateOnInputDlg->IsReserveAction() )
            {
                if( m_pkNateOnInputDlg->GetUse() == GCNateOnAddGroupDlg::ADD )
                {
                    AddNateOnBuddyGroup( m_pkNateOnInputDlg->GetInputStr() );
                }
                else if( m_pkNateOnInputDlg->GetUse() == GCNateOnAddGroupDlg::RENAME )
                {
                    RenameNateOnBuddyGroup( m_pkNateOnInputDlg->GetGroupID(), m_pkNateOnInputDlg->GetInputStr() );
                }
                else if( m_pkNateOnInputDlg->GetUse() == GCNateOnAddGroupDlg::BLOCK_BUDDY )
                {
                    BlockBuddy( m_pkNateOnInputDlg->GetInputStr() );
                }
            }
        }
    }
    else if( event.m_pWnd == m_pkNateOnAddFriend )
    {
        if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN )
        {
            m_pkNateOnAddFriend->Destroy();
            g_KDSound.Play( "32" );
            m_pkNateOnAddFriend->SetReserveAction( true );
        }
        else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkNateOnAddFriend->SetReserveAction( false );
            m_pkNateOnAddFriend->Destroy();
            g_KDSound.Play( "32" );

			switch( m_pkNateOnAddFriend->GetUse() )
			{
				case GCNateOnAddFriend::ADD:
					break;
				case GCNateOnAddFriend::REQUEST:
					if( KP2P::GetInstance()->m_kMsgServer != NULL )
						KP2P::GetInstance()->m_kMsgServer->Send_RejectFriendReq_S2( m_pkNateOnAddFriend->GetRequesterUID() );
					break;
			}
        }
        else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
        {
            EndMessageBox();

            if( m_pkNateOnAddFriend->IsReserveAction() )
            {
                switch( m_pkNateOnAddFriend->GetUse() )
                {
					case GCNateOnAddFriend::ADD:
						{
							KBuddy FData;
							if( SiKFriendShip()->GetBlockBuddy( m_pkNateOnAddFriend->GetNickName(), FData ) )
							{
								MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NATEON_BLOCK_FRIEND_TO_ADD )
									, L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
							}
							else
							{
                                if( KP2P::GetInstance()->m_kMsgServer != NULL ) { 
                                    KP2P::GetInstance()->m_kMsgServer->Send_AddFriendReq_S2( m_pkNateOnAddFriend->GetNickName(), m_pkNateOnAddFriend->GetMessage() );
                                }
							}
						}
						break;
					case GCNateOnAddFriend::REQUEST:
						if( KP2P::GetInstance()->m_kMsgServer != NULL )
							KP2P::GetInstance()->m_kMsgServer->Send_AcceptFriendReq_S2( m_pkNateOnAddFriend->GetRequesterUID() );
						break;
                }
            }
        }
    }
	else if( event.m_pWnd == m_pkBlockToAddFriend )
	{
		if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN )
		{
			m_pkBlockToAddFriend->Destroy();
			g_KDSound.Play( "32" );
		}
		else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkBlockToAddFriend->Destroy();
			g_KDSound.Play( "32" );
		}
		else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
		{
			EndMessageBox();
		}
	}
    else if( event.m_pWnd == m_pkEmoticonSelect )
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkEmoticonSelect->Destroy();
            g_KDSound.Play( "32" );
        }
        else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
        {
            EndMessageBox();

			switch( m_pkEmoticonSelect->GetUse() )
			{
				case GCEmoticonSelectDlg::NATEON:
					{
						KGCEmoticon* pEmoticon = m_pkEmoticonSelect->GetData();
						bool bSend = false;
						if( pEmoticon != NULL )
						{
							bSend = SiKFriendShip()->SetEmoticon( (DWORD)pEmoticon->m_iItemID );
						}
						else
						{
							bSend = SiKFriendShip()->SetEmoticon( KFriend::DEFAULT_EMOTICON_ID );
						}
						if( true == bSend )
						{
							ChangeEmoticon();
						}
					}
					break;
				case GCEmoticonSelectDlg::INPUT:
					{
						KGCEmoticon* pEmoticon = m_pkEmoticonSelect->GetData();
						if( pEmoticon )
						{
							KD3DEdit* pEditor = m_pkNateOn->GetNateOnChatWindow()->GetEditor();
							if( pEditor )
							{
								int iStart = 0;
								int iEnd = 0;
								::SendMessage( pEditor->GetHandle()
									, EM_GETSEL,
									reinterpret_cast<WPARAM>( &iStart ),
									reinterpret_cast<LPARAM>( &iEnd ) );
								
								std::wstring wstrContent = pEditor->GetText();
								std::wstring wstrEmoticon;
								wstrEmoticon += EMOTICON_SEPRATOR;
								wstrEmoticon += g_pkStrLoader->GetString( pEmoticon->m_iStrID );
								wstrEmoticon += EMOTICON_SEPRATOR;
								wstrContent.insert( iStart, wstrEmoticon.c_str() );
								pEditor->SetText( wstrContent.c_str() );
								iEnd = iStart + wstrEmoticon.length();
								pEditor->SetSelection( iEnd, iEnd );
							}
						}
					}
					break;
			}

            m_pkEmoticonSelect->InitData();
        }
    }
	else if( event.m_pWnd == m_pkCoupleViewer )
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkCoupleViewer->Destroy();
		}
		else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
		{
			EndMessageBox();
		}
	}
	else if( event.m_pWnd == m_pkCoupleAccept )
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkCoupleAccept->Destroy();
		}
		else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
		{
			EndMessageBox();
		}
	}
	else if( event.m_pWnd == m_pkCouplePropose )
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkCouplePropose->Destroy();
		}
		else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
		{
			EndMessageBox();
		}
	}
	else if( event.m_pWnd == m_pkCoupleWait )
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkCoupleWait->Destroy();
		}
		else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
		{
			EndMessageBox();
		}
	}
	else if( event.m_pWnd == m_pkCoupleMessage )
	{
		if( event.m_dwCode == KGCCoupleMessage::CMBR_OK )
		{
			if( m_pkCoupleMessage->GetUse() == GC_MBOX_USE_COUPLE_ITEM_CONFIRM )
			{
				SiKGCCoupleSystem()->SendInsertToInven();
			}
			m_pkCoupleMessage->Destroy();
		}
		else if( event.m_dwCode == KGCCoupleMessage::CMBR_CANCLE )
		{
			m_pkCoupleMessage->Destroy();
		}
		else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
		{
			EndMessageBox();
		}
	}
	else if( event.m_pWnd == m_pkCoupleGoodbye )
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkCoupleGoodbye->Destroy();
		}
		else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
		{
			EndMessageBox();
		}
	}
	else if( event.m_pWnd == m_pkPetCostumeInven )
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkPetCostumeInven->Destroy();
		}
		else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
		{
			EndMessageBox();
		}
	}
	else if( event.m_pWnd == m_pkContributionDlg)
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkContributionDlg->Destroy();
			g_KDSound.Play( "32" );
		}
		else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
		{
			EndMessageBox();
		}
	}
	else if ( event.m_pWnd == m_pkContributionBox ) // 기부 박스일 경우
	{

		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkContributionBox->Destroy();
			g_KDSound.Play( "32" );
		}
		else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
		{
			EndMessageBox();
        }
    }
    else if ( event.m_pWnd == m_pkVipGachaBox )
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkVipGachaBox->Destroy();
            g_KDSound.Play( "32" );
        }
        else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
        {
            EndMessageBox();
            g_pkUIScene->m_pkMyInfoScene->UpdateData();
            g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
			g_pkUIScene->m_pkVipGachaBox->UpdateBadgeNum();

		}
	}
	else if( event.m_pWnd == m_pkHappyNewYearNPCDlg )
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkHappyNewYearNPCDlg->Destroy();
			g_KDSound.Play( "32" );
		}
		else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
		{
			EndMessageBox();
		}
	}
	else if( event.m_pWnd == m_pkLogoutBox )
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkLogoutBox->Destroy();
		}
		else if(event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
		{
			EndMessageBox();			
			switch(m_pkLogoutBox->GetReturn())
            {
            case KGCLogoutBox::LOGOUT:
                if( g_MyD3D->m_pStateMachine->GetState() == GS_GAME &&
                    g_pStateGame != NULL )
                    g_pStateGame->ClearGameResource();
                
                if ( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO || g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM )
                    g_pkUIScene->m_pkMyInfoScene->OnClickCloseSellUI();

                g_kGlobalValue.m_kUserInfo.vecCharInfo.clear();
                g_kGlobalValue.SetSavedCharType(-1);
                g_MyD3D->InitializeData();

#if defined ( USE_SURVEY_SYSTEM )
                g_pkUIScene->m_pkSurveyDlg->SetHaveQuestion( false );
#endif

#if defined( USE_POST_LETTER_SYSTEM )
                SiKGCPostLetterManager()->ClearPostInfo();
#endif

                g_MyD3D->m_pStateMachine->GoState( GS_LOGIN );
				g_kGlobalValue.m_bSendServerInfoNot = false;
                SiKGCJumpingCharManager()->SetEnableEvent( false );

                break;
			case KGCLogoutBox::QUIT:
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EXIT_GAME, g_pkStrLoader->GetString(STR_ID_CONFIRM_EXIT), L""
					, KGCUIScene::GC_MBOX_USE_GAMEEXIT, 0, 0, false, false );
				break;
			case KGCLogoutBox::DETECT_HACK_QUIT:
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EXIT_GAME, g_pkStrLoader->GetString(STR_ID_DETECT_HACK_FORCE_QUIT), L""
					, KGCUIScene::GC_MBOX_USE_GAMEEXIT, 0, 0, false, false );
				break;			
			}		
		}		
	}

    else if (event.m_pWnd == m_pkBillBoardBox) {
    if (event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE)
    {
        m_pkBillBoardBox->Destroy();
    }
    else if (event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED)
    {
        EndMessageBox();
    }
    }

    else if (event.m_pWnd == m_pkNicknameColorBox) {
    if (event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE)
    {
        m_pkNicknameColorBox->Destroy();
    }
    else if (event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED)
    {
        EndMessageBox();
    }
    }

    else if (event.m_pWnd == m_pkNewComboBox)
    {
    if (event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE)
    {
        m_pkNewComboBox->Destroy();
    }
    else if (event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED)
    {
        EndMessageBox();
    }
    }

#if defined (NEW_MENU)
    else if (event.m_pWnd == m_pkShortMenuCutBox)
    {
    if (event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE)
    {
        m_pkShortMenuCutBox->Destroy();
        g_KDSound.Play("32");
        EndMessageBox();
    }
    }
#else
    else if (event.m_pWnd == m_pkEscMenuDlg)
    {
    if (event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE)
    {
        m_pkEscMenuDlg->Destroy();
        g_KDSound.Play("32");
    }
    else if (event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED)
    {
        EndMessageBox();
    }
    }

#endif
	/*else if( event.m_pWnd == m_pkValentineDayDlg)
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkValentineDayDlg->Destroy();
			g_KDSound.Play( "32" );
		}
		else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
		{
			EndMessageBox();
		}
	}*/

    //Loja Heroica EP2
    else if (event.m_pWnd == m_pkHeroInfoS6) {
    if (event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE) {
        m_pkHeroInfoS6->Destroy();
    }
    else if (event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED) {
        EndMessageBox();
    }
    }

    else if (event.m_pWnd == m_pkHeroDungeonShopS6) {
    if (event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE) {
        m_pkHeroDungeonShopS6->Destroy();
    }
    else if (event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED) {
        EndMessageBox();
    }
    }

    else if (event.m_pWnd == m_pkHeroRewardS6) {
    if (event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE) {
        m_pkHeroRewardS6->Destroy();
    }
    else if (event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED) {
        EndMessageBox();
    }
    }

	else if( event.m_pWnd == m_pkGPCapsuleDlg)
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkGPCapsuleDlg->Destroy();
			g_KDSound.Play( "32" );
		}
		else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
		{
			EndMessageBox();
		}
	}
	else if( event.m_pWnd == m_pkGPCapsuleObtainWnd)
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkGPCapsuleObtainWnd->Destroy();
			g_KDSound.Play( "32" );
		}
		else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
		{
			EndMessageBox();
		}
	}
	else if( event.m_pWnd == m_pkCouponNPCDlg)
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkCouponNPCDlg->Destroy();
			g_KDSound.Play( "32" );
		}
		else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
		{
			EndMessageBox();
		}
	}
	else if( m_pkAgreement != NULL && event.m_pWnd == m_pkAgreement )
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkAgreement->Destroy();
			EndMessageBox();
		}
	}
    else if( event.m_pWnd == m_pkRecommendList )
    {
        if( event.m_dwCode == KGCRecommendList::IRR_CLOSE )
        {
            m_pkRecommendList->Destroy();
            g_KDSound.Play( "32" );
        }
        else if( event.m_dwCode == KGCRecommendList::IRR_DESTROYED )
        {
            EndMessageBox();

#if defined( WELCOMEBACK_FRIEND_NEW )
            MessageBox( GC_MBOX_WELCOMEBACK_DLG_NEW );
#else
            MessageBox( GC_MBOX_WELCOMEBACK_DLG );
#endif
        }
    }
	else if ( event.m_pWnd == m_pkMsgBox2 )
	{
		switch ( event.m_dwCode )
		{
		case KGCMessageBox::MBR_OK: // '예'를 눌렀을 경우
			{
				m_pkMsgBox2->Destroy();	
				g_KDSound.Play( "32" );
			}
		}
	}
	else if(event.m_pWnd == m_pkMsgSizingBox)
	{
		switch ( event.m_dwCode )
		{
		case KGCMessageBox::MBR_OK: // '예'를 눌렀을 경우
			{
				m_pkMsgSizingBox->Destroy();	
				g_KDSound.Play( "32" );
			}
		}

	}
 	else if ( event.m_pWnd == m_pkDicePlay )
 	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkDicePlay->Destroy();
			g_KDSound.Play( "32" );
			EndMessageBox();
		}
	}
	else if( event.m_pWnd == m_pkSubscriptionDlg)
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkSubscriptionDlg->Destroy();
			g_KDSound.Play( "32" );
		}
		else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
		{
			EndMessageBox();
		}
	}
	else if( event.m_pWnd == m_pkSubscriptionBox)
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkSubscriptionBox->Destroy();
			g_KDSound.Play( "32" );
		}
		else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
		{
			EndMessageBox();
		}
	}
    else if(event.m_pWnd == m_pkForgeDlg)
    {
        if(event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE)
        {
            SiGCForgeManager()->SetEnableBreakUp( false );
            m_pkForgeDlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }
	else if(event.m_pWnd == m_pkSongkranDlg)
	{
		if(event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE)
		{
			m_pkSongkranDlg->Destroy();
			g_KDSound.Play( "32" );
			EndMessageBox();
		}
	}
	else if(event.m_pWnd == m_pkSongkranRewardDlg)
	{
		if(event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE)
		{
			m_pkSongkranRewardDlg->Destroy();
			g_KDSound.Play( "32" );
			EndMessageBox();
		}
	}
	else if(event.m_pWnd == m_pkEventAlarmDlg)
	{
		if(event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE)
		{
			m_pkEventAlarmDlg->Destroy();
			g_KDSound.Play( "32" );
			EndMessageBox();
		}
	}
	else if (event.m_pWnd ==m_pkRitasChristmasDlg)
	{
		if(event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE)
		{
			m_pkRitasChristmasDlg->Destroy();
			g_KDSound.Play( "32" );
			EndMessageBox();
		}
	}
    else if (event.m_pWnd == m_pkGcAdventureDlg )
    {
        if (event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkGcAdventureDlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }
	/*else if( event.m_pWnd == m_pkGWCDonationDlg )
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkGWCDonationDlg->Destroy();
			EndMessageBox();
		}
	}*/
	else if ( event.m_pWnd == m_pkPuzzle )
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkPuzzle->Destroy();
			g_KDSound.Play( "32" );
			EndMessageBox();
		}
	}
	/*else if( event.m_pWnd == m_pkGWCShowStateDlg )
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkGWCShowStateDlg->Destroy();
			EndMessageBox();
		}
	}*/
	else if( event.m_pWnd == m_pkBuyCashBox )
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkBuyCashBox->Destroy();
			EndMessageBox();
		}
    }
    else if( event.m_pWnd == m_pkCoordiChangeDlg )
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkCoordiChangeDlg->Destroy();
            EndMessageBox();
        }
    }
    else if( event.m_pWnd == m_pkOlympicDlg ) { 
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE ) {
            m_pkOlympicDlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }
    else if( event.m_pWnd == m_pkOlympicGawibawiboDlg ) { 
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE ) {
            m_pkOlympicGawibawiboDlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
            SiGCOlympicManager()->ShowMinigameReward();
        }
    }
    else if( event.m_pWnd == m_pkOlympicDicePlayDlg ) { 
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE ) {
            m_pkOlympicDicePlayDlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
            SiGCOlympicManager()->ShowMinigameReward();
        }
    }
    else if( event.m_pWnd == m_pkOlympicSongkranDlg ) { 
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE ) {
            m_pkOlympicSongkranDlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
            SiGCOlympicManager()->ShowMinigameReward();
        }
    }
	else if( event.m_pWnd == m_pkLottery ) { 
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE ) {
			m_pkLottery->Destroy();
			g_KDSound.Play( "32" );
			EndMessageBox();
		}
	}
    else if ( event.m_pWnd == m_pkAttendanceStampDlg ) 
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkAttendanceStampDlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }
    else if ( event.m_pWnd == m_pkBuffDonationDlg ) 
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkBuffDonationDlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }
    else if ( event.m_pWnd == m_pkBuyCashItemChooseCashTypeDlg ) 
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkBuyCashItemChooseCashTypeDlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }
    else if ( event.m_pWnd == m_pkBuyCashItemAccumulatedGCDlg ) 
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkBuyCashItemAccumulatedGCDlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }
    else if ( event.m_pWnd == m_pkBuyCashItemDlgS6 ) 
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkBuyCashItemDlgS6->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }
    else if ( event.m_pWnd == m_pkBuyCashItemBuyCompleteDlg ) 
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkBuyCashItemBuyCompleteDlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }
    else if( event.m_pWnd == m_pkSocialCommerceDlg ) { 
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE ) {
            m_pkSocialCommerceDlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }
    
    else if ( event.m_pWnd == m_pkSongkranDlg ) 
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkSongkranDlg->Destroy();
            EndMessageBox();
        }
    }
    else if(event.m_pWnd == m_pkRoomUserInfoPopup )
    {
        if(event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE)
        {
            m_pkRoomUserInfoPopup->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }
    else if ( event.m_pWnd == m_pkHotTimeSearchDlg ) 
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkHotTimeSearchDlg->Destroy();
            EndMessageBox();
        }
    }
	else if ( event.m_pWnd == m_pkNewNecklaceEnchantDlg ) 
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkNewNecklaceEnchantDlg->Destroy();
			EndMessageBox();
		}
	}
	else if ( event.m_pWnd == m_pkMonsterCardCompose ) 
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkMonsterCardCompose->Destroy();
			EndMessageBox();
		}
	}	
	else if( event.m_pWnd == m_pkCharacterSelectBox)
	{
		if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
		{
			m_pkCharacterSelectBox->ToggleRender(false);
			if(m_pkCharacterSelectBox->m_UpdateFunc != NULL ){
				m_pkCharacterSelectBox->m_UpdateFunc();
				m_pkCharacterSelectBox->m_SetFunc = NULL;
				m_pkCharacterSelectBox->m_UpdateFunc = NULL;
			}
		}
	}
	else if ( event.m_pWnd == m_pkChallengErranDlg ) 
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkChallengErranDlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }
    else if ( event.m_pWnd == m_pkJumpingCharDlg ) 
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkJumpingCharDlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }
    else if ( event.m_pWnd == m_pkCoordiComposeDlg ) 
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkCoordiComposeDlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }    
    else if ( event.m_pWnd == m_pkCoordiUpgradeDlg ) 
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkCoordiUpgradeDlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }
    else if ( event.m_pWnd == m_pkInfinityGuideDlg ) 
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkInfinityGuideDlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }
    else if ( event.m_pWnd == m_pkEnchantS6Dlg ) 
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkEnchantS6Dlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }    
    else if ( event.m_pWnd == m_pkEnchantSubS6Dlg ) 
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkEnchantSubS6Dlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }
    else if ( event.m_pWnd == m_pkDungeonRankingDlg ) 
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkDungeonRankingDlg->Destroy();
            g_KDSound.Play( "32" );
            EndMessageBox();
        }
    }  
    else if( event.m_pWnd == m_pkCooperationEventDlg 
            || event.m_pWnd == m_pkGuildMarkRegistDlg
            || event.m_pWnd == m_pkGuildDlg
            || event.m_pWnd == m_pkGuildEnterDlg 
            || event.m_pWnd == m_pkGuildLobbyDlg
			|| event.m_pWnd == m_pkGuildListDlg 
			|| event.m_pWnd == m_pkGuildCreateDlg
			|| event.m_pWnd == m_pkGuildJoinReqMbox
			|| event.m_pWnd == m_pkGuildIntroduceEdit 
            || event.m_pWnd == m_pkMonsterCardBook 
            || event.m_pWnd == m_pkItemSocketManagementDlg
            || event.m_pWnd == m_pkGiftItemBox
            || event.m_pWnd == m_pkHeroDungeonDlg
            || event.m_pWnd == m_pkReplayRecordDlg
            || event.m_pWnd == m_pkReplayBoxNew
            || event.m_pWnd == m_pkAttributeSelectBox
            || event.m_pWnd == m_pkNicknameCampaignBox
            )
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            event.m_pWnd->Destroy();
            g_KDSound.Play( "32" );
        }
        else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
        {
            EndMessageBox();
        }
    }
	else if ( event.m_pWnd == m_pkGuildPublicMbox )
	{
		switch ( event.m_dwCode )
		{
		case KGCGuildPublicMbox::MBR_YES: // '예'를 눌렀을 경우
			{
				switch ( m_pkGuildPublicMbox->GetUse() )
				{
				case GC_MBOX_GUILD_BREAK_UP:
					{
						g_pkUIScene->m_pkGuildDlg->Destroy();
						m_pkGuildSquareNotice->Destroy();
						DeleteWaitMsgBox(GC_MBOX_GUILD_DLG);
						DeleteWaitMsgBox(GC_MBOX_GUILD_PUBLIC_MBOX);
						KP2P::GetInstance()->Send_SelfDrumoutGuildReq();						
					}
					break;
				case GC_MBOX_GUILD_MASTER_COMMTION:
					{
						KP2P::GetInstance()->Send_ChangeGuildMemberLevelReq(g_kGlobalValue.m_mapNGuildChangeMemberGrade);
					}
					break;
				case GC_MBOX_GUILD_DESTROY:
					{
						g_pkUIScene->m_pkGuildDlg->Destroy();
						m_pkGuildSquareNotice->Destroy();
						DeleteWaitMsgBox(GC_MBOX_GUILD_DLG);
						DeleteWaitMsgBox(GC_MBOX_GUILD_PUBLIC_MBOX);
						KP2P::GetInstance()->Send_BreakupGuildReq(g_kGlobalValue.m_kGuildInfo.m_dwUID, g_kGlobalValue.m_kGuildInfo.m_strName);
					}
					break;
				case GC_MBOX_GUILD_JOIN_CANCEL_REQ:
					{
						KP2P::GetInstance()->Send_CancelJoinGuildReq(g_kGlobalValue.m_kGuildInfo.m_dwUID);
					}
					break;
				case GC_MBOX_GUILD_MARK_UPLOAD_ALERT:
					{
						m_pkGuildMarkRegistDlg->UploadGuildMark();
                        g_kGlobalValue.m_kGuildInfo.m_ucMarkStatus = KNGuildInfo::MS_REQUEST_CHANGE;
					}
					break;
				case GC_MBOX_GUILD_NAME_CHANGE_ALERT:
					{
						KP2P::GetInstance()->Send_EditGuildNameReq(g_kGlobalValue.m_strTempGuildName, g_kGlobalValue.m_kGuildInfo.m_dwUID );
					}
					break;
				}
				m_pkGuildPublicMbox->Destroy();
				g_KDSound.Play( "32" );
			}
			break;
		case KGCGuildPublicMbox::MBR_NO: // '예'를 눌렀을 경우
			{
				switch ( m_pkGuildPublicMbox->GetUse() )
				{
				case GC_MBOX_GUILD_MASTER_COMMTION:
					{
						g_kGlobalValue.m_mapNGuildChangeMemberGrade.erase((int)KNGuildUserInfo::GL_MASTER);
						m_pkGuildDlg->UpdateGuildMangeDlg(KGCGuildManageDlg::BTN_MEMBER_MANAGE);						
					}
					break;
				case GC_MBOX_GUILD_BREAK_UP:
					{
						m_pkGuildDlg->SetClickSubMenu(false);
					}
					break;
				}
				m_pkGuildPublicMbox->Destroy();
				g_KDSound.Play( "32" );
				break;
			}
		case KGCGuildPublicMbox::MBR_OK: // '예'를 눌렀을 경우
			{					
				switch( m_pkGuildPublicMbox->GetUse() )
				{
				case GC_MBOX_GUILD_SQUARE_OUT:					
					if( SiGCSquare()->GetSquareType() == KSquareInfo::ST_GUILD )
					{
						g_pkSquareOverlayUI->ExitSquare();
						m_pkGuildSquareNotice->Destroy();
					}
					else
					{
						g_pkUIScene->m_pkGuildListDlg->UpdateWaitingJoinerState();
					}
					break;
				case GC_MBOX_GUILD_JOIN_RESULT:
					g_pkUIScene->m_pkGuildListDlg->UpdateWaitingJoinerState();
					break;
				case GC_MBOX_USE_CREATE_GUILD_UPDATE:
					KP2P::GetInstance()->Send_SearchGuildListReq(KEVENT_SEARCH_GUILD_LIST_REQ::ST_GUILD_ID_DATE, L"");
					break;
				}							
				m_pkGuildPublicMbox->Destroy();
				g_KDSound.Play( "32" );
			}
			break;
		}
	}
    else if( event.m_pWnd == m_pkCharGetInfoDlg)
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkCharGetInfoDlg->Destroy();
            g_KDSound.Play( "32" );
        }
        else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
        {
            EndMessageBox();
        }
    }



    else if( event.m_pWnd == m_pkMissionUI_S4)
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkMissionUI_S4->Destroy();
            g_KDSound.Play( "32" );
        }
        else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
        {
            EndMessageBox();
        }
    }
    else if( event.m_pWnd == m_pkMissionEquipPreview_S4)
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkMissionEquipPreview_S4->Destroy();
            g_KDSound.Play( "32" );
        }
        else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
        {
            EndMessageBox();
        }
    }

    else if( event.m_pWnd == m_pkWeaponChangeBox)
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            g_MyD3D->m_TempPlayer.m_mapSwapWeapon[g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType] = std::make_pair<bool, std::pair<GCITEMID,GCITEMUID>>(false, std::make_pair<GCITEMID,GCITEMUID>(m_pkWeaponChangeBox->GetSelectItemID() * 10,m_pkWeaponChangeBox->GetSelectItemUID()));
            m_pkMyInfoScene->SetEquipWndCategory(UEC_EQUIP1);
            m_pkMyInfoScene->UpdateData();
            m_pkWeaponChangeBox->Destroy();
            g_KDSound.Play( "32" );
        }
        else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
        {
            EndMessageBox();
        }
    }
    else if( event.m_pWnd == m_pkCoordiCatalogDlg )
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
            m_pkCoordiCatalogDlg->Destroy();
        else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
            EndMessageBox();
    }
    else if( event.m_pWnd == m_pkSellCountItemDlg)
    {
        if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            m_pkSellCountItemDlg->Destroy();
            g_KDSound.Play( "32" );
        }
        else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_DESTROYED )
        {
            EndMessageBox();
        }
    }
    else if (event.m_pWnd == m_pkGachaPongDlg) {
        if (event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE)
        {
            m_pkGachaPongDlg->Destroy();
        }
    }
    else if (event.m_pWnd == m_pkCoordiViewDlg) {
        if (event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE)
        {
            m_pkCoordiViewDlg->Destroy();
        }
    }
    else if (event.m_pWnd == m_pkCustomizationDlg) {
        if (event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE)
        {
            m_pkCustomizationDlg->Destroy();
        }
    }
    else if (event.m_pWnd == m_pkQuantityUseDlg) {
        if (event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE)
        {
            m_pkQuantityUseDlg->Destroy();
        }
    }
    else //Default는 닫히는거다.
    {
        if( event.m_dwCode == GC_MBOX_RETURN_OK )
        {
            g_KDSound.Play( "32" );
            event.m_pWnd->Destroy();
            EndMessageBox();
        }
        else if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
        {
            g_KDSound.Play( "32" );
            event.m_pWnd->Destroy();
            EndMessageBox();
        }
    }
}

bool KGCUIScene::IsMessageBoxType( EGCMsgBoxType eMsgBoxType ) 
{
	//if ( g_MyD3D->m_pStateMachine->GetState() != GS_GAME && m_bEnableMessageBox ) 
    if( ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL || SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT || g_MyD3D->m_pStateMachine->GetState() != GS_GAME ) 
        && m_bEnableMessageBox ) 
		return false;

    if( m_kCurrMsgBoxInfo.m_eMsgBoxType == GC_MBOX_GEM_BUY && eMsgBoxType == GC_MBOX_OK ) { 
        return false;
    }

    if (m_kCurrMsgBoxInfo.m_eMsgBoxType == GC_MBOX_NEW_COMBO_BOX_SHOP && eMsgBoxType == GC_MBOX_OK) {
        return false;
    }

	if ( IsAllStateMessageBox( eMsgBoxType ) ) 
		return false;

    if ( g_MyD3D->m_pStateMachine->GetState() == GS_GAME && eMsgBoxType == GC_MBOX_TUTORIAL_EXIT ) 
        return false;
    
    if ( m_kCurrMsgBoxInfo.m_eMsgBoxType == GC_MBOX_TUTORIAL_EXIT && eMsgBoxType == GC_MBOX_TUTORIAL_EXIT ) 
        return false;
    
    if ( g_MyD3D->m_pStateMachine->GetState() == GS_GAME && eMsgBoxType == GC_MBOX_EXIT_GAME ) 
		return false;

	if ( g_MyD3D->m_pStateMachine->GetState() == GS_GAME && eMsgBoxType == GC_MBOX_LOGOUT_BOX )
		return false;

	if ( m_kCurrMsgBoxInfo.m_eMsgBoxType == GC_MBOX_EXIT_GAME && eMsgBoxType == GC_MBOX_EXIT_GAME ) 
		return false;

	if ( g_MyD3D->m_pStateMachine->GetState() == GS_LOGIN && eMsgBoxType == GC_MBOX_AGREEMENT ) 
		return false;

    return true;
}

void KGCUIScene::MessageBox( EGCMsgBoxType eMsgBoxType,
                            const std::wstring& strText1 /* = "" */,
                            const std::wstring& strText2 /* = "" */,
                            EGCMsgBoxUse eUse /* = GC_MBOX_USE_NORMAL */,
                            KEVETPARM lParam /* = 0 */,
                            KEVETPARM lParam2 /* = 0 */,
                            bool bFirst /* = false */,
                            bool bForce /* = false */,
                            bool bErasePrevMsgBox /* = false */,
                            bool bMsgBoxBack /* = true */ )
{
    // 초보자 가이드 중에는 각종 이벤트 및 아이템 획득 메시지 박스 출력을 하지 않습니다.
    if( g_pkGameOverlayUI->m_pkAddtionGuide->GetGuideUserState() == STATE_PROGRESS_IN_TUTORIAL || 
        g_pkGameOverlayUI->m_pkAddtionGuide->GetGuideUserState() == STATE_PROGRESS_IN_TUTORIAL_DUNGEON_OUT ) 
    {
        if( eMsgBoxType == KGCUIScene::GC_MBOX_OK ||
            eMsgBoxType == KGCUIScene::GC_MBOX_EVENT_ALARM_DLG ||
            eMsgBoxType == KGCUIScene::GC_MBOX_WELCOMEBACK_DLG ||
            eMsgBoxType == KGCUIScene::GC_MBOX_WELCOMEBACK_DLG_NEW
            )
            return;
    }

    //가이드 진행중인지 체크
    if ( g_MyD3D->m_kGuideMgr.IsGuideOngoing() )
    {
        if ( eMsgBoxType == KGCUIScene::GC_MBOX_NICK_NAME  )
            return;
    }

	// 메시지 박스 정보세팅
    KMsgBoxInfo kMsgBoxInfo;
    kMsgBoxInfo.m_eMsgBoxType = eMsgBoxType;
    kMsgBoxInfo.m_eMsgBoxUse  = eUse;
    kMsgBoxInfo.m_strText1    = strText1;
    kMsgBoxInfo.m_strText2    = strText2;
    kMsgBoxInfo.m_lParam      = lParam;
    kMsgBoxInfo.m_lParam2	  = lParam2;


	//2008.05.13 milennium9
	// 중복되어서 vector에 쌓으면 안되는 Wnd들을 검사해 줍니다	
    if( m_setDoNotOverlapedWnd.find( eMsgBoxType ) != m_setDoNotOverlapedWnd.end() )
	{
        if( m_kCurrMsgBoxInfo.m_eMsgBoxType == eMsgBoxType && m_bEnableMessageBox == false )
            return;

        if( IsWaitMsgBox( eMsgBoxType ) == true )
			return;
	}

    //if( eMsgBoxType != GC_MBOX_EXIT_GAME && m_bEnableMessageBox == false/* && eMsgBoxType != GC_MBOX_REPLAY*/ )
    // 2007.03.19 wony
    // 토할것 같은 조건=ㅅ= 뭔가 좋은방법이 있을텐데...후...
    if( IsMessageBoxType( eMsgBoxType ) ) 
    {
        // 강제로 기존의 모달 다이얼로그를 밀어내고 자신이 활성화된다
        if( bForce || IsAllStateMessageBox( eMsgBoxType ) )
        {

            if ( bErasePrevMsgBox )
            {
                m_kCurrMsgBoxInfo = kMsgBoxInfo;
            }
            if( m_bMsgBoxModal )
            {
                // 2007/4/26. iridology. 메신저 서버 접속이 실패 했을 때 이리로 들어오게 되서
                // 더미박스가 하나 나오게 됨. 일단 지금 바빠서 -_-
				if( g_MyD3D->m_pStateMachine->GetState() != GS_LOGO ){
					//타이밍 관련 문제 발생하여 아래 코드 들어감...
					//메시지 박스가 대기중인 상태에 있으면서 현재 메시지 박스를 종료중에 있는데 
					//새로 만든 메시지 박스가 총알받이가 되듯이 먼저 나와서 바로 사라지는 현상이 발생하여
					//아래와 같이 WAIT 리스트가 0인경우에만 갈아타도록 만들어놓음...
					if(bFirst && bForce){
						if(m_vecWaitMsgBox.empty()){
							m_vecWaitMsgBox.insert( m_vecWaitMsgBox.begin(), m_kCurrMsgBoxInfo );
						}
						else{
							m_vecWaitMsgBox.insert( m_vecWaitMsgBox.begin(), kMsgBoxInfo );
							return;
						}						
					}
					else{
						m_vecWaitMsgBox.insert( m_vecWaitMsgBox.begin(), m_kCurrMsgBoxInfo );
					}
				}
            }
        }
        else
        {
            if( bFirst )
                m_vecWaitMsgBox.insert( m_vecWaitMsgBox.begin(), kMsgBoxInfo );
            else 
                m_vecWaitMsgBox.push_back( kMsgBoxInfo );

            return;
        }
    }

    SetReverse(false);

	if(m_pkItemInformationBox->IsRenderOn()){
		m_pkItemInformationBox->ToggleRender(false);
	}

	if(SiKGCDragManager()->GetDragState() == KGCDragManager::KGDM_DRAG_PROGRESS)
		SiKGCDragManager()->EndDrag();

    // 현재 활성화된 메시지박스의 정보를 저장
    m_kCurrMsgBoxInfo = kMsgBoxInfo;
	
    KD3DDialog::KDialogInfo kDialogInfo;
    kDialogInfo.m_nMsgBoxType = m_kCurrMsgBoxInfo.m_eMsgBoxType;
    kDialogInfo.m_nMsgBoxUse = m_kCurrMsgBoxInfo.m_eMsgBoxUse;
    kDialogInfo.m_lParam = m_kCurrMsgBoxInfo.m_lParam;
    kDialogInfo.m_lParam2 = m_kCurrMsgBoxInfo.m_lParam2;
    kDialogInfo.m_strText1 = m_kCurrMsgBoxInfo.m_strText1;
    kDialogInfo.m_strText2 = m_kCurrMsgBoxInfo.m_strText2;
	

    switch ( eMsgBoxType )
    {
    case GC_MBOX_EMOTICON_COLLECTION:
    case GC_MBOX_TITLE_COLLECTION:
    case GC_MBOX_PET_COLLECTION:
        {
            OpenCollectionWindow( eMsgBoxType );
            break;
        }
    }
    
    if( eMsgBoxType != GC_MBOX_EMOTICON_COLLECTION &&
        eMsgBoxType != GC_MBOX_TITLE_COLLECTION &&
        eMsgBoxType != GC_MBOX_PET_COLLECTION
#if defined( NATION_BRAZIL )    //  이벤트 베너 안들어간 국가
        && eMsgBoxType != GC_MBOX_EVENT_BANNER
#endif
    )
    {
        KD3DDialog* pDialog = GetDialogPtr( eMsgBoxType );
        if( pDialog == NULL ) {
#ifndef __PATH__
            terminate();
#endif
            return;
        }

        if( pDialog->CheckCondition( kDialogInfo ) ) {
			SetActiveMsgBox( pDialog );
			pDialog->InitDialog( kDialogInfo );
			pDialog->SetWindowZOrder(KD3DWnd::D3DWZ_TOPMOST);
        }
        else {
            return;
        }
    }

    // 2007.11.14   
    // 메세지 박스가 뜰 때는 뒤의 채팅창에 포커스를 잃게 한다
    switch( g_MyD3D->m_pStateMachine->GetState() )
    {
    case GS_GUILD:
        g_pkGuildChatBox->m_pkEdit->Activate( false );
        break;
    case GS_ROOM:
        g_pkRoomChatBox->m_pkEdit->Activate( false );
        break;
    case GS_SHOP_CASH:
        g_pkUIScene->m_pkCashShopScene->m_pkSearchBar->SetFocus( false );
        break;
    case GS_SHOP_GP:
        g_pkUIScene->m_pkGPShopScene->m_pkSearchBar->SetFocus( false );
        break;
    case GS_MATCH:
        g_pkLobbyChatBox->m_pkEdit->Activate( false );
        break;
    case GS_WORLDMAP:
		if ( eMsgBoxType != GC_MBOX_ROOM_LIST_DLG_S6 )
			g_pkWorldmapChatBox->m_pkEdit->Activate( false );
        break;
//#if defined( USE_AGIT_SYSTEM )
//    case GS_GAME:
//        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT )
//            g_pkAgitChatBox->m_pkEdit->Activate( false );
//        break;
//#endif
    }

    // 2007.04.12
    // 메세지 박스를 제외한 뒷부분에 색을 다르게 해 준다
    ToggleMessageBoxBack( bMsgBoxBack );

    // 2007.12.24 wony
    // 월드맵 상태라면 메세지 박스가 뜰 때 파티클을 꺼준다!
//     if( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP )
//         m_pkWorldMapContainer->RenderMapEffect( false );

    // 메시지 박스가 뜰 때 상점 알리미 파티클도 끄자.
    g_kShopAdvertising.RenderParticle( false );

    // 게임이나 광장에서는 메시지 박스 계열 떠도 뒤에 중지 안됨
    if( IsSetStop() ) 
    {
        Stop( true );
    }

    m_bMsgBoxModal = true;

    if ( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP )
        m_pkWorldMapContainer->RenderMapEffect( false );

    if( m_pkCollectionItemInfoBox != NULL )
    {
        m_pkCollectionItemInfoBox->ToggleRender( false );
    }

    if( m_pkItemInformationBox != NULL )
    {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
    }

    ReleaseCapture();

    // 다른소리를 듣고 싶다 ;ㅅ;
    g_KDSound.Play( "32" );

}

bool KGCUIScene::IsSetStop()
{
	if( !g_MyD3D->m_pStateMachine ) 
		return false;

	//	게임모드 아니면 무조껀 true
	if ( GS_GAME != g_MyD3D->m_pStateMachine->GetState() ) 
		return true;


	//	게임중이고 아지트라면
	if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT ) 
		return true;


	return false;

}

void KGCUIScene::LuaMessageBox( EGCMsgBoxType eMsgBoxType, EGCMsgBoxUse eUse /*= GC_MBOX_USE_NORMAL*/, KEVETPARM lParam /*= 0*/, KEVETPARM lParam2 /*= 0*/, int bForce /*= 0 */, int iStringID1 /*= -1 */, int iStringID2 /*= -1 */)
{
	if( iStringID1 == -1 )
		iStringID1 = (int)STR_ID_BLANK;

	if( iStringID2 == -1 )
		iStringID2 = (int)STR_ID_BLANK;

	MessageBox( eMsgBoxType, g_pkStrLoader->GetString(iStringID1), g_pkStrLoader->GetString(iStringID2), eUse, lParam, lParam2, false, bForce != 0 );
}

void KGCUIScene::EndMessageBox( void )
{
    if (g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE)
    {
        if (g_pkSquareOverlayUI!=NULL)
        {
            //ActionListenr로 보낼려다 걍 처리함..-0-;;
            g_pkSquareOverlayUI->EndMessageBox();
        }
    }

    if (g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP)
        m_pkWorldMapContainer->SetEffectRenderDelay();

    g_pkUIMgr->SendEvent( KActionEvent( m_pkMsgBox, KD3DWnd::D3DWE_MESSAGEBOX_CLOSE ) );

    //if ( GS_GAME == g_MyD3D->m_pStateMachine->GetState() )
    //{
    //    m_bMsgBoxModal = false;
    //}
    //else
    //{
    //    Stop( false );
    //    m_bMsgBoxModal = false;
    //}

    // 도타상점이 열려있을땐 페이드처리 키처리 풀지마라
    if( KD3DWnd::D3DWS_ENABLED != g_pkUIScene->GetStateUI( KGCUIScene::GC_MBOX_DOTA_SHOP ) )       
    {
        Stop( false );
        m_bMsgBoxModal = false;
        m_bEnableMessageBox = true;
        ToggleMessageBoxBack( false );

#ifdef FEATURE_2021
        if (g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP)
            m_pkWorldMapContainer->RenderMapEffect(true);
#endif
    }

    g_kShopAdvertising.RenderParticle( true );
}

void KGCUIScene::SetActiveMsgBox( KD3DWnd* pkMsgBox )
{
    if( pkMsgBox->GetWndState() != KD3DWnd::D3DWS_DESTROY )
    {
        pkMsgBox->ToggleRender(true);

        pkMsgBox->Create();

// 		if( m_pkCurMsgBox != NULL )
// 			SetChildOrder( pkMsgBox, m_pkCurMsgBox, true );
        m_pkCurMsgBox = pkMsgBox;

        m_bEnableMessageBox = false;

        pkMsgBox->ReleaseCapture();

        g_pkUIMgr->SendEvent( KActionEvent( pkMsgBox, KD3DWnd::D3DWE_MESSAGEBOX_OPEN ) );
    }
}

void KGCUIScene::BuyItemForGem( IN const GCITEMID itemID_, IN const int nItemCount_, IN const int nPeriod_ )
{
    Result_Buy = 0;

    KP2P::GetInstance()->Send_BuyItemForGemReq( 10 * itemID_, nItemCount_, nPeriod_ );

    g_MyD3D->WaitForServerAck(Result_Buy, 0 , 3000, 2 );

    if ( Result_Buy == 1 ) // 성공
    {
        g_KDSound.Play( "78" );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString( STR_ID_BUY_ITEM_SUCCEEDED1 ),
            g_pkStrLoader->GetString( STR_ID_BUY_ITEM_SUCCEEDED2 ) );
    }
    else if ( Result_Buy == 2 ) // 타임아웃
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1 ),
            g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE2 ) );
    }
    else // 실패
    {
        std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_BUY_FOR_GEM_ACK, Result_Buy );	
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString( STR_ID_BUY_GEM_ITEM_FAILED ), strTemp );
    }
}

void KGCUIScene::BuyItemForCrystal( IN const GCITEMID itemID_, IN const int nCount_, IN const int nPeriod_ )
{
    Result_Buy = 0;

    KP2P::GetInstance()->Send_BuyItemForCrystalReq(itemID_ * 10, nCount_, nPeriod_, 0);

    g_MyD3D->WaitForServerAck(Result_Buy, 0, 3000, 2);

    if (Result_Buy == 1)
    {
        g_KDSound.Play("78");
        g_pkUIScene->MessageBoxW(KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString(STR_ID_BUY_ITEM_SUCCEEDED1),
            g_pkStrLoader->GetString(STR_ID_BUY_ITEM_SUCCEEDED2), GC_MBOX_USE_NORMAL, 0i64, 0i64, true);
    }
    else if (Result_Buy == 2)
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1),
            g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE2), GC_MBOX_USE_NORMAL, 0i64, 0i64, true);
    }
    else if (Result_Buy == -98)
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_PACKET_DELAY));
    }
    else
    {
        std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_BUY_FOR_CRYSTAL_ACK, Result_Buy);
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString(STR_ID_BUY_CRYSTAL_ITEM_FAILED), strTemp, GC_MBOX_USE_NORMAL, 0i64, 0i64, true);
    }
}

void KGCUIScene::BuyFontSkinForCrystal(IN const GCITEMID itemID_, IN const int nCount_, IN const int nPeriod_)
{
    Result_Buy = 0;

    KP2P::GetInstance()->Send_BuyItemForCrystalReq(itemID_ * 10, nCount_, nPeriod_, 0);

    g_MyD3D->WaitForServerAck(Result_Buy, 0, 3000, 2);

    if (Result_Buy == 1)
    {
        g_KDSound.Play("78");
        g_pkUIScene->MessageBoxW(KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString(STR_ID_BUY_ITEM_SUCCEEDED1),
            g_pkStrLoader->GetString(STR_ID_BUY_ITEM_SUCCEEDED2), GC_MBOX_USE_NORMAL, 0i64, 0i64, true, true);

        if (g_pkUIScene->m_pkCustomizationDlg->IsWndState(KD3DWnd::ED3DWndState::D3DWS_ENABLED))
            g_pkUIScene->m_pkCustomizationDlg->UpdateAllSubWindows();
    }
    else if (Result_Buy == 2)
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1),
            g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE2), GC_MBOX_USE_NORMAL, 0i64, 0i64, true, true);
    }
    else if (Result_Buy == -98)
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_PACKET_DELAY));
    }
    else
    {
        std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_BUY_FOR_CRYSTAL_ACK, Result_Buy);
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString(STR_ID_BUY_CRYSTAL_ITEM_FAILED), strTemp, GC_MBOX_USE_NORMAL, 0i64, 0i64, true, true);
    }
}


void KGCUIScene::BuyGem( int iGemNum )
{
    Result_Buy = 0;

    KP2P::GetInstance()->Send_BuyItemForGPReq( 10 * GEM_ITEM_ID, iGemNum );

    g_MyD3D->WaitForServerAck( Result_Buy, 0, 3000, 2 );

    /*
    MSG     msg;
    DWORD   dwStartTime = ::timeGetTime();
    while ( !Result_Buy )
    {
        if ( ::PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
            ::DispatchMessage( &msg );
        }

        if ( dwStartTime + 3000 < ::timeGetTime() ) Result_Drop = 2;
    }*/

        if ( Result_Buy == 1 ) // 성공
        {
            g_KDSound.Play( "78" );
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_BUY_GEM_SUCCEEDED1 ),
                g_pkStrLoader->GetString( STR_ID_BUY_GEM_SUCCEEDED2 ),  
                KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        }
        else if ( Result_Buy == 2 ) // 타임아웃
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1 ),
                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE2 ) );
        }
        else // 실패
        {
            std::wstring strTemp = g_pkStrLoader->GetString( STR_ID_BUY_GEM_FAILED2 );
            strTemp += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_BUY_FOR_GP_ACK, Result_Buy );	
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_BUY_GEM_FAILED1 ),
                strTemp );
        }
}

void KGCUIScene::SellCountItem( void )
{
    g_MyD3D->WaitForServerAck( Result_Sell_Count_Item, INT_MAX, 30000, TIME_OUT_VALUE );

    if ( Result_Sell_Count_Item == 0 )
	{
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_SELL_COUNT_COMPLETE ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
    }
    else
    {
        std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_SELL_COUNT_ITEM_ACK, Result_Sell_Count_Item );	
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_SELL_COUNT_ERROR ), strTemp, KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
    }
}


void KGCUIScene::SellItem( KItem*& pkInventoryItemInfo )
{
    if( pkInventoryItemInfo == NULL)
        return;

    GCITEMUID SellItemUID = pkInventoryItemInfo->m_ItemUID;
    if( g_MyD3D->GetMyShortCutSlot()->isExistByItemUID(SellItemUID) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString( STR_ID_NETMARBLE_PC_RENAS2 ),
            g_pkStrLoader->GetString( STR_ID_ALL_CHAR_UNEQUIP2 ) );

        return;
    }

    GCItem* kItem = g_pItemMgr->GetItemData( pkInventoryItemInfo->m_ItemID );
    RETURN_NIL( kItem );

    if( kItem->eItemKind == GIK_PET && kItem->ePetItemKind == EPET_KIND_PET
        && g_MyD3D->IsExistPetInfo( pkInventoryItemInfo->m_ItemUID ) )
    {
        Result_DeletePet = INT_MAX;

        // 펫 : 펫을 판매할 때는 펫 판매 패킷을 보낸다
#if defined( USE_BIND_PET )
        if( g_kGlobalValue.IsVestedItem( kItem->dwGoodsID ) == false ) {    // 공용 펫
            KP2P::GetInstance()->Send_DelPetReq( pkInventoryItemInfo->m_ItemUID );
        }
        else {  // 귀속 펫
            KP2P::GetInstance()->Send_DelPetReq( pkInventoryItemInfo->m_ItemUID, static_cast<char>( g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType ) );
        }
#else
        KP2P::GetInstance()->Send_DelPetReq( pkInventoryItemInfo->m_ItemUID );
#endif
        g_MyD3D->WaitForServerAck( Result_DeletePet, INT_MAX, 10000, -6 );

        if( Result_DeletePet == 0 )		// 성공
        {
            // 판매하는 펫을 장착하고 있는 캐릭터의 펫 장착 정보를 지워준다
            for( int i = 0; i < (int)g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.size(); i++ )
            {
                SCharInfo &SChar = g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo[i];

                // 끼고있는 펫과 지운 펫이 같다면
                if( SChar.kPetInfo.m_dwUID == pkInventoryItemInfo->m_ItemUID )
                {
                    // 펫이 끼고 있는 아이템 장착해제
                    g_MyD3D->m_TempPlayer.UnEquipPet( SChar.iCharType );
                }
            }

           // 클라이언트에 존재하는 펫정보를 지워준다
            std::map<GCITEMUID, KPetInfo>::iterator mit;
            mit = g_MyD3D->m_mapPetInfo.find( pkInventoryItemInfo->m_ItemUID );
            if( mit != g_MyD3D->m_mapPetInfo.end() )
                g_MyD3D->m_mapPetInfo.erase( mit );

            g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();
            g_pkUIScene->m_pkMyInfoScene->UpdateGameData( false, g_pkUIScene->m_pkMyInfoScene->GetEquipCategory() );
        }
        else							// 실패
        {
            std::wstring strTemp = g_pkStrLoader->GetString ( STR_ID_APPMSG_ERR_DISCONNECTED2 );
            strTemp += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_DELETE_PET_ACK, Result_DeletePet );	

            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_DISCONNECTED1 ),
                strTemp );            

            return;
        }
    }
    else
    {
        Result_Drop = 0;
        KP2P::GetInstance()->Send_DropItemReq( "Server", pkInventoryItemInfo->m_ItemUID,
            10 * pkInventoryItemInfo->m_ItemID );	
        g_MyD3D->WaitForServerAck( Result_Drop, 0, 3000, 2 );

        if ( Result_Drop != 1 )
        {
            return;
        }
    }

    // 공용아이템인 경우 다른 캐릭터가 끼고있는 아이템을 판매하는 경우가 있는데
    // 이 때 장착정보에서도 모두 지워준다
    for( int i = 0; i < (int)g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.size(); i++ )
    {
        SCharInfo &SChar = g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo[i];
        g_pItemMgr->UnequipItemForItemUID( &SChar, pkInventoryItemInfo->m_ItemUID );
    }

    // 펫 : 펫장착 아이템을 판매하는 경우 현재 펫이 장착하고 있다면 정보를 모두 삭제 해 준다
    if( kItem->eItemKind == GIK_PET && kItem->ePetItemKind == EPET_KIND_EQUIP_ITEM )
    {
        // pkInventoryItemInfo->m_dwUID -> 판매하는 펫아이템의 UID
        std::map<GCITEMUID, KPetInfo>::iterator mit;
        std::vector<KSimpleItem>::iterator vit;
        for( mit = g_MyD3D->m_mapPetInfo.begin(); mit != g_MyD3D->m_mapPetInfo.end(); mit++ )
        {
            for( vit = mit->second.m_vecEquipItem.begin(); vit != mit->second.m_vecEquipItem.end();  )
            {
                KSimpleItem& petEqipItem = (*vit);
                // 펫이 끼고있는 아이템과 판매하는 아이템이 같다면
                if( petEqipItem.m_dwUID == pkInventoryItemInfo->m_ItemUID )
                {
                    vit = mit->second.m_vecEquipItem.erase( vit );
                }
                else
                {
                    ++vit;
                }
            }
        }

        // 캐릭터의 펫정보 갱신
        for( int i = 0; i < (int)g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.size(); i++ )
        {
            KPetInfo &kPet = g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo[i].kPetInfo;
            if( kPet.m_dwUID == 0 )
            {
                kPet.Init();
            }
            else
            {
                std::map<GCITEMUID, KPetInfo>::iterator mit = g_MyD3D->m_mapPetInfo.find( kPet.m_dwUID );
                if( mit != g_MyD3D->m_mapPetInfo.end() )
                {
                    kPet = mit->second;
                }
            }
        }
    }

    g_KDSound.Play( "78" );
	g_pkUIScene->m_pkGambleBox->RemoveItem( pkInventoryItemInfo->m_ItemUID );

    g_pItemMgr->m_kInventory.RemoveItem( pkInventoryItemInfo->m_ItemUID );

	

    //돌려받은 돈을 업데이트 해야해서 아래 함수 부르는듯...
    g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().SetCurrentGP( g_kGlobalValue.m_kUserInfo.GetCurrentChar().GetCurrentGP() );
    //g_MyD3D->m_TempPlayer.SetMyPlayer();
    g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
}

void KGCUIScene::RepairBreakedItem( KItem*& kInv )
{
    Result_Repair = 0;

    KP2P::GetInstance()->Send_RevivalBreakItemReq( "Server",
        10 * kInv->m_ItemID , kInv->m_ItemUID );

    g_MyD3D->WaitForServerAck( Result_Repair, 0, 3000, 2 );

    switch ( Result_Repair )
    {
    case 1:
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_ENCNT_REPAIR_OK1 ),
                L"" );
            break;
        }
    case 2:
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1 ),
                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE2 ) );
            break;
        }
    case 3:
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_ENCNT_REPAIR_ERROR1 ),
                L"" );
            break;
        }
    }
}
// 3  추천인 시스템 없음
// 2  추천인 닉네임이 비어 있음
// 1  비추하였음
// 0  정상적으로 추천인이 등록 되었음
// -1 추천하려는 사람의 정보가 없음 --> 이녀석인 경우 심각한 문제이다. 넘어가시오.
// -2 이미 추천하였습니다.(이중접속등으로 인해서 다른넘이 먼저 등록했다. 일수 있음)
// -3 다시 추천하세욤.(대상이 존재하지 않음)
bool KGCUIScene::ReportRecommendNickName(std::wstring wstrUserNick)
{
    if ( m_pkNickNameBox->GetRecommendRegisterComplete() == true )
	{
		m_pkNickNameBox->Destroy();
        return true;
	}
    Result_RecommendNickName = INT_MAX;

    KP2P::GetInstance()->Send_RecommendUserReq( wstrUserNick, 0);

    g_MyD3D->WaitForServerAck(Result_RecommendNickName, INT_MAX, 10000, 4 );

    //1   이벤트 기간이 아님.
    //2   신규 혹은 복귀 유저가 아님.
    //3   사용할수 없는 문자. 공백이 포함 되어 있음.
    //4   추천 요청시 잘못된 요청 범위.
    //5   DB 처리 오류.
    //6   추천을 할수 없는 상태임.
    //7   추천 대상 유저가 존재하지 않음.
    //-98 처리중
    switch( Result_RecommendNickName )
    {
    case 0:     //추천인 등록성공
        {
            m_pkNickNameBox->SetRecommendRegisterComplete( true );
            MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetReplacedString( STR_ID_MAKE_RECOMMEND, "L", wstrUserNick ), L"", KGCUIScene::GC_MBOX_USE_NORMAL);
            m_pkNickNameBox->Destroy();
            break;
        }
    case 1:     // 이벤트 기간이 아님
    case 2:
        {
            m_pkNickNameBox->SetRecommendRegisterComplete( true );
            MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_HAPPY_NEWYEAR_NOT_EVENT_TIME), L"", KGCUIScene::GC_MBOX_USE_NORMAL);
            m_pkNickNameBox->Destroy();
            break;
        }
	case 8:
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_DONOT_RECOMUSER ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false );
		break;
    case 3:
    case 4:
    case 5:
    case 7:    //추천하려는 대상이없음
        {
            MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NOT_EXIST_RECOMMEND1), g_pkStrLoader->GetString( STR_ID_NOT_EXIST_RECOMMEND2), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true);
            m_pkNickNameBox->SetRecommendBox(true);
            break;
        }
    case -98:
        {
            MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PACKET_DELAY), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true);
            m_pkNickNameBox->SetLock(1);
            break;
        }
    case 6:     //추천인 만땅
        MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetReplacedString( STR_ID_ALREADY_FULL_PYRAMID, "L", wstrUserNick ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true);
        break;
    default:
        {
            std::wstring strTemp = g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE2 );
            strTemp += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", 
                KUserEvent::EVENT_RECOMMEND_USER_ACK, Result_RecommendNickName );	

            MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1), strTemp, KGCUIScene::GC_MBOX_USE_NORMAL);
            m_pkNickNameBox->Destroy();
            break;
        }
    }
    return true;
}


void KGCUIScene::DestroyCurrentMsgBox( void )
{
    if ( m_bMsgBoxModal )
    {
        m_pkCurMsgBox->Destroy();
        m_vecDestroyDlg.push_back( m_pkCurMsgBox );
    }
}

void KGCUIScene::ChangePetNickName( IN const KItem& NickNameItem, DWORD dwPetID_, GCITEMUID dwPetUID_ )
{
	m_pkNickNameBox->Destroy();

    Result_ReportPetNickName = INT_MAX;

    KP2P::GetInstance()->Send_ChangePetNickName( NickNameItem, dwPetID_, dwPetUID_, m_pkNickNameBox->GetNickName().c_str() );

    g_MyD3D->WaitForServerAck( Result_ReportPetNickName, INT_MAX, 10000, -5 );

    switch( Result_ReportPetNickName )
    {
    case 0: // 성공
        {
            // 닉네임을 셋팅한다.
            std::map<GCITEMUID, KPetInfo>::iterator mit;
            mit = g_MyD3D->m_mapPetInfo.find( dwPetUID_ );
            if( mit != g_MyD3D->m_mapPetInfo.end() )
            {
                mit->second.m_strName = m_pkNickNameBox->GetNickName();
				g_kGlobalValue.m_kUserInfo.GetCurrentChar().kPetInfo.m_strName = m_pkNickNameBox->GetNickName();
                std::vector<GCITEMUID> vecUseItem;
                g_pItemMgr->DecDurationItem( NickNameItem.m_ItemUID, &vecUseItem );
            }

            for( int i = 0; i < (int)g_kGlobalValue.m_kUserInfo.vecCharInfo.size(); i++ )
            {
                KPetInfo &kPet =g_kGlobalValue.m_kUserInfo.vecCharInfo[i].kPetInfo;

                if( kPet.m_dwUID == dwPetUID_ )
                {
                    std::map<GCITEMUID, KPetInfo>::iterator mit = g_MyD3D->m_mapPetInfo.find( dwPetUID_ );
                    if( mit != g_MyD3D->m_mapPetInfo.end() )
                    {
                        kPet = mit->second;
                    }
                }
			
			}
            g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
            m_pkNickNameBox->SetChangeNickName(false);  // m_bUseChangeNickName 을 원래대로 돌려줌

            return;
        }
        break;
    case 1: // 닉네임 카드가 없음
        {
            MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_CHANGE_PET_NICK_ITEM_NO_EXIT ) );
        }
        break;
    case 2: // 적법하지 않은 닉네임
        {
            MessageBox( GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_PET_NICKNAME_CHANGE_FAIL ),
                g_pkStrLoader->GetString( STR_PET_NICKNAME_ANOTHER_USE ) );
        }
        break;
    case 3: // 펫이 없음
        {
            MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PET_NO_EXIST ) );
        }
        break;
    case 4: // 펫 ID가 다름
        break;
        // 절대로 일어나지 않을것임..
    case 5: // 닉네임 카드가 없음
        break;
    default:
        {
            std::wstring strTemp = g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE2 );
            strTemp += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", 
                KUserEvent::EVENT_CHANGE_PET_NAME_ACK, Result_ReportPetNickName );	

            // 시간 초과
            MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1 ), strTemp );
        }
        break;
    }
    m_pkNickNameBox->SetChangeNickName(false);  // m_bUseChangeNickName 을 원래대로 돌려줌
}

//==============================================================================================
// 0 : 성공
//-1 : 아이디와 기존닉네임으로 해당 유저를 찾을 수 없음
//-2 : 신규닉네임이 이미 사용중임
//-3 : 닉네임변경아이템의 UID 또는 ID값이 다르거나 아이템이 존재하지 않음
//-4 : 부적절 닉네임
//-10이하 : 트랜잭션 에러
void KGCUIScene::ChangeNickName( GCITEMID dwItemID_, GCITEMUID dwItemUID_ )
{
	m_pkNickNameBox->Destroy();

    Result_ReportNickName = INT_MAX;
	
    KP2P::GetInstance()->Send_ChangeNickName( dwItemID_*10, dwItemUID_, m_pkNickNameBox->GetNickName().c_str(), true ,  g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType );

    g_MyD3D->WaitForServerAck( Result_ReportNickName, INT_MAX, 30000, -100 );

    switch ( Result_ReportNickName )
    {
        case 0: // 0 : 성공
        {
            g_kGlobalValue.m_kUserInfo.strNickName = m_pkNickNameBox->GetNickName();
            g_pItemMgr->m_kInventory.RemoveItem( dwItemUID_ );
            g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->SetMyPlayer();
            g_MyD3D->m_TempPlayer.SetMyPlayer(-1);
            g_pkUIScene->m_pkMyInfoScene->UpdateGameData();

            if(g_pkUIScene->m_pkNicknameCampaignBox != NULL)
                g_pkUIScene->m_pkNicknameCampaignBox->SetValidNickname( true );

            if( KP2P::GetInstance()->m_kMsgServer != NULL )
            {
                // 메신저 서버에 닉네임 변경을 알린다
                if( KP2P::GetInstance()->m_kMsgServer->IsConnectedToMsgServer() )
                {
                    KP2P::GetInstance()->m_kMsgServer->Send_ChangeNickName( g_kGlobalValue.m_kUserInfo.strNickName );
                }
            }
            m_pkNickNameBox->SetChangeNickName(false);  // m_bUseChangeNickName 을 원래대로 돌려줌
            return;;
        }
        case -1: // -1 : 해당 유저가 존재하지 않음. -_-
        {
            MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NICK_NAME_ERROR ) );
            break;
        }
        case -2: // -2 : 해당 유저는 이미 닉네임이 등록되어 있음.
        {
            MessageBox( GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_NICK_NAME_ALREADY_EXIST ),
                g_pkStrLoader->GetString( STR_ID_NICK_NAME_ANOTHER_USE ),GC_MBOX_USE_NORMAL);
            break;
        }       
        case -3: // -3 : 닉네임변경아이템의 UID 또는 ID값이 다르거나 아이템이 존재하지 않음
        {
            MessageBox( GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_INVAILD_ITEM ),
                g_pkStrLoader->GetString( STR_ID_INQUIRE_TO_CUSTOMER_SUPPORT ),GC_MBOX_USE_NORMAL);
            break;
        }
        case -4: // -4 : 부적절한 닉네임(빈칸, 탭, \', 욕설, 크기가0, 제한크기초과..등)
        {
            MessageBox( GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_NICK_NAME_INVALID ),
                g_pkStrLoader->GetString( STR_ID_NICK_NAME_ANOTHER_USE ) ,GC_MBOX_USE_NORMAL);
            break;
        }
	    case -5: // -5 : 부적절한 길이(크기가0, 제한크기초과)
		{
			MessageBox( GC_MBOX_OK,
				g_pkStrLoader->GetString( STR_ID_NICK_NAME_INVALID_LENGTH ),
				g_pkStrLoader->GetString( STR_ID_NICK_NAME_INVALID_LENGTH2 ),GC_MBOX_USE_NORMAL);
			break;
		}
        case -100: // 시간 초과
        {
			MessageBox( GC_MBOX_OK,
				g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1 ).c_str(),
				g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE2 ).c_str(),GC_MBOX_USE_NORMAL );
            break;
        }
	    default:
		{
            std::wstring strTemp = g_pkStrLoader->GetString( STR_ID_NICK_NAME_ANOTHER_USE );
            strTemp += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", 
                KUserEvent::EVENT_USE_CHANGE_NICKNAME_ACK, Result_ReportNickName );	

            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_NICK_NAME_INVALID ),
                strTemp, KGCUIScene::GC_MBOX_USE_NORMAL );
			break;
		}
    }
    m_pkNickNameBox->SetChangeNickName(false);  // m_bUseChangeNickName 을 원래대로 돌려줌

    MessageBox( GC_MBOX_NICK_NAME, L"", L"",(EGCMsgBoxUse)m_pkNickNameBox->GetUse(), m_pkNickNameBox->GetlParam1(), m_pkNickNameBox->GetlParam2() );
}

void KGCUIScene::ChangeNickColor(GCITEMID dwItemID_, GCITEMUID dwItemUID_)
{
    SiKP2P()->Send_ChangeNickColor(dwItemID_ * 10, dwItemUID_, m_pkNicknameColorBox->currentColor.c_str(), true, g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType);

    g_kGlobalValue.m_kUserInfo.strNickColor = m_pkNicknameColorBox->currentColor;
    g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->SetMyPlayer();
    g_MyD3D->m_TempPlayer.SetMyPlayer(-1);
    g_pkUIScene->m_pkMyInfoScene->UpdateGameData();

    if (KP2P::GetInstance()->m_kMsgServer != NULL)
    {
        if (KP2P::GetInstance()->m_kMsgServer->IsConnectedToMsgServer())
        {
            KP2P::GetInstance()->m_kMsgServer->Send_ChangeNickColor( g_kGlobalValue.m_kUserInfo.strNickName, g_kGlobalValue.m_kUserInfo.strNickColor );
        }
    }

    MessageBox(GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_DONE_NICKCOLOR_TEXT));
}

BOOL KGCUIScene::ChangeNickNameFromCampaign()
{
    m_pkNicknameCampaignBox->Destroy();

    Result_ReportNickName = INT_MAX;

    KP2P::GetInstance()->Send_ChangeNickName( 0, 0, m_pkNicknameCampaignBox->GetNickName().c_str(), false, g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType );

    g_MyD3D->WaitForServerAck( Result_ReportNickName, INT_MAX, 10000, -100 );



    std::wostringstream strmText;
    switch ( Result_ReportNickName )
    {
    case 0: // 0 : 성공
        {
            // 닉네임을 셋팅한다.
            g_pkUIScene->m_pkNicknameCampaignBox->SetValidNickname( true );
            g_pkUIScene->m_pkNicknameCampaignBox->SetFirst( false );

            g_kGlobalValue.m_kUserInfo.strNickName = m_pkNicknameCampaignBox->GetNickName();
            //g_pItemMgr->m_kInventory.RemoveItem( dwItemUID_ );
            g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->SetMyPlayer();
            g_MyD3D->m_TempPlayer.SetMyPlayer(-1);
            g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
            if( KP2P::GetInstance()->m_kMsgServer != NULL )
            {
                // 메신저 서버에 닉네임 변경을 알린다
                if( KP2P::GetInstance()->m_kMsgServer->IsConnectedToMsgServer() )
                {
                    KP2P::GetInstance()->m_kMsgServer->Send_ChangeNickName( g_kGlobalValue.m_kUserInfo.strNickName );
                }
            }

            return TRUE;
        }
    case -1: // -1 : 해당 유저가 존재하지 않음. -_-
        {
            MessageBox( GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NICK_NAME_ERROR ) );
            break;
        }
    case -2: // -2 : 해당 유저는 이미 닉네임이 등록되어 있음.
        {
            MessageBox( GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_NICK_NAME_ALREADY_EXIST ),
                g_pkStrLoader->GetString( STR_ID_NICK_NAME_ANOTHER_USE ),GC_MBOX_USE_NORMAL, 
                0 , 0, true, true ); 

            break;
        }       
    case -3: // -3 : 닉네임변경아이템의 UID 또는 ID값이 다르거나 아이템이 존재하지 않음
        {
            MessageBox( GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_INVAILD_ITEM ),
                g_pkStrLoader->GetString( STR_ID_INQUIRE_TO_CUSTOMER_SUPPORT ),GC_MBOX_USE_NORMAL, 
                0 , 0, true, true ); 
            break;
        }
    case -4: // -4 : 부적절한 닉네임(빈칸, 탭, \', 욕설, 크기가0, 제한크기초과..등)
        {
            MessageBox( GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_NICK_NAME_INVALID_CHAR ),
                g_pkStrLoader->GetString( STR_ID_NICK_NAME_ANOTHER_USE ) ,GC_MBOX_USE_NORMAL, 
                0 , 0, true, true ); 
            break;
        }
    case -5: // -5 : 부적절한 길이(크기가0, 제한크기초과)
        {
            MessageBox( GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_NICK_NAME_INVALID_LENGTH ),
                g_pkStrLoader->GetString( STR_ID_NICK_NAME_INVALID_LENGTH2 ),GC_MBOX_USE_NORMAL, 
                0 , 0, true, true ); 
            break;
        }
    case -100: // 시간 초과
        {
            MessageBox( GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1 ).c_str(),
                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE2 ).c_str(),GC_MBOX_USE_NORMAL, 
                0 , 0, true, true ); 
            break;
        }
    default:
        {
            std::wstring strTemp = g_pkStrLoader->GetString( STR_ID_NICK_NAME_ANOTHER_USE );
            strTemp += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", 
                KUserEvent::EVENT_USE_CHANGE_NICKNAME_ACK, Result_ReportNickName );	

            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_NICK_NAME_INVALID ),
                strTemp, KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );

            break;
        }
    }
    return FALSE;
}

void KGCUIScene::EnableBuyCashItemWnd( DWORD dwGoodID_, bool bPresent_, bool bTopMost /*= false*/, bool waitFirst /*=false*/ )
{
    if ( g_kGlobalValue.m_kLoginInfo.bExpAccount == true )
    {
        // 체험 계정이라는 MessageBox 띄워준다. 확인 누르면 인증 페이지로 취소 누르면 걍 창 닫자.
        MessageBox( GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_EXPACCOUNT1 ).c_str(), 
            g_pkStrLoader->GetString( STR_ID_EXPACCOUNT2 ).c_str(), KGCUIScene::GC_MBOX_USE_CERTIFY_GASH );
        return;
    }

    GCItem* kItem = g_pItemMgr->GetItemData( dwGoodID_ );
    RETURN_NIL( kItem );

    if( bTopMost ){
		MessageBox( KGCUIScene::GC_MBOX_BUY_CASH_ITEM, L"", L"", GC_MBOX_USE_NORMAL, 0, 0, false, true );
        m_pBuyCashItemWnd->SetWindowZOrder(KD3DWnd::D3DWZ_TOPMOST);
    }
	else if(waitFirst){
		MessageBox( KGCUIScene::GC_MBOX_BUY_CASH_ITEM, L"", L"", GC_MBOX_USE_NORMAL, 0, 0, true, false );
	}
	else
		MessageBox( KGCUIScene::GC_MBOX_BUY_CASH_ITEM );
    m_pBuyCashItemWnd->EnableBuyCashItemWnd( kItem, bPresent_ );
}

bool KGCUIScene::IsCashBoxRenderOn()
{
	return m_pBuyCashItemWnd->IsRenderOn();

}

bool KGCUIScene::IsValidState_ShowMessageBox()
{
    GAME_STATE state = g_MyD3D->m_pStateMachine->GetState();

#if defined( USE_AGIT_SYSTEM )
    if( state == GS_GAME && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT )
        return true;
    else
    return ( state != GS_GAME )
        && ( state != GS_GAME_LOADING )
        && ( state != GS_GAME_END )
        && ( state != GS_INIT_LOADING )
        && ( state != GS_TUTORIAL )
        && ( state != GS_SQUARE_LOADING )
        && ( state != GS_SQUARE_END );
#endif
    return ( state != GS_GAME )
        && ( state != GS_GAME_LOADING )
        && ( state != GS_GAME_END )
        && ( state != GS_INIT_LOADING )
        && ( state != GS_TUTORIAL )
        && ( state != GS_SQUARE_LOADING )
        && ( state != GS_SQUARE_END );
}

void KGCUIScene::ToggleMessageBoxBack( bool bRender, bool bForce )
{
    if( bRender )
    {
        m_pkBackGroundWhite->ToggleRender( true );
        m_pkBackGroundWhite->ForceSetWindowColor( D3DCOLOR_ARGB( BACKGROUND_ALPHA, 0x00, 0x00, 0x00 ) );
        //SetChildOrder( m_pkBackGroundWhite, m_pkCurMsgBox, false );
        m_iBackgroundAlphaCounter = TOGGLEBACKGROUND_FRAME;
    }
    
    // 강제로 끄는거면 알파 빼는 부분 없앤다.
    if ( !bRender && bForce )
    {
        m_pkBackGroundWhite->ToggleRender( false );
        m_iBackgroundAlphaCounter = 0;

    }

    m_bToggleBackground = bRender;
}

void KGCUIScene::RemovePointItem( GCITEMUID dwItemUID )
{
    KItem* kInvenItem = NULL;
    GCItem* kItem = NULL;

    if( !g_pItemMgr->FindInventory( dwItemUID, &kInvenItem, &kItem ) )
    {
        // 니 인벤에 그딴 아이템은 없다! =ㅅ=
        return;
    }

    if( g_MyD3D->GetMyShortCutSlot()->isExistByItemUID(dwItemUID) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString( STR_ID_NETMARBLE_PC_RENAS2 ),
            g_pkStrLoader->GetString( STR_ID_ALL_CHAR_UNEQUIP2 ) );

        return;
    }

    if( kItem->eItemKind == GIK_PET && kItem->ePetItemKind == EPET_KIND_PET
        && g_MyD3D->IsExistPetInfo( kInvenItem->m_ItemUID ) )
    {
        Result_DeletePet = INT_MAX;

        // 펫 : 펫을 판매할 때는 펫 판매 패킷을 보낸다
#if defined( USE_BIND_PET )
        if( g_kGlobalValue.IsVestedItem( kItem->dwGoodsID ) == false ) {    // 공용 펫 
            KP2P::GetInstance()->Send_DelPetReq( kInvenItem->m_ItemUID );
        }
        else {  // 귀속 펫 
            KP2P::GetInstance()->Send_DelPetReq( kInvenItem->m_ItemUID, static_cast<char>( g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType ) );
        }
#else
        KP2P::GetInstance()->Send_DelPetReq( kInvenItem->m_ItemUID );
#endif
        g_MyD3D->WaitForServerAck( Result_DeletePet, INT_MAX, 10000, -6 );

        if( Result_DeletePet == 0 )		// 성공
        {
            // 판매하는 펫을 장착하고 있는 캐릭터의 펫 장착 정보를 지워준다
            for( int i = 0; i < (int)g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.size(); i++ )
            {
                SCharInfo &SChar = g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo[i];

                // 끼고있는 펫과 지운 펫이 같다면
                if( SChar.kPetInfo.m_dwUID == kInvenItem->m_ItemUID )
                {
                    // 펫이 끼고 있는 아이템 장착해제
                    g_MyD3D->m_TempPlayer.UnEquipPet( SChar.iCharType );
                }
            }

            // 클라이언트에 존재하는 펫정보를 지워준다
            std::map<GCITEMUID, KPetInfo>::iterator mit;
            mit = g_MyD3D->m_mapPetInfo.find( kInvenItem->m_ItemUID );
            if( mit != g_MyD3D->m_mapPetInfo.end() )
                g_MyD3D->m_mapPetInfo.erase( mit );

            g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();
            g_pkUIScene->m_pkMyInfoScene->UpdateGameData( false, g_pkUIScene->m_pkMyInfoScene->GetEquipCategory() );
        }
        else							// 실패
        {
            std::wstring strTemp = g_pkStrLoader->GetString ( STR_ID_APPMSG_ERR_DISCONNECTED2 );
            strTemp += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_DELETE_PET_ACK, Result_DeletePet );	

            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_DISCONNECTED1 ), 
                strTemp );

            return;
        }
    }
    else
    {
        Result_RemovePointItem = INT_MAX;

        KP2P::GetInstance()->Send_SellForGCPoint( kInvenItem->m_ItemID, kInvenItem->m_ItemUID );

        g_MyD3D->WaitForServerAck( Result_RemovePointItem, INT_MAX, 5000, TIME_OUT_VALUE );

        if( Result_RemovePointItem != 0 )
            return;
    }

    // 공용아이템인 경우 다른 캐릭터가 끼고있는 아이템을 판매하는 경우가 있는데
    // 이 때 장착정보에서도 모두 지워준다
    for( int i = 0; i < (int)g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.size(); i++ )
    {
        SCharInfo &SChar = g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo[i];
        g_pItemMgr->UnequipItemForItemUID( &SChar, kInvenItem->m_ItemUID );
    }

    // 펫 : 펫장착 아이템을 판매하는 경우 현재 펫이 장착하고 있다면 정보를 모두 삭제 해 준다
    if( kItem->eItemKind == GIK_PET && kItem->ePetItemKind == EPET_KIND_EQUIP_ITEM )
    {
        // pkInventoryItemInfo->m_dwUID -> 판매하는 펫아이템의 UID
        std::map<GCITEMUID, KPetInfo>::iterator mit;
        std::vector<KSimpleItem>::iterator vit;
        for( mit = g_MyD3D->m_mapPetInfo.begin(); mit != g_MyD3D->m_mapPetInfo.end(); mit++ )
        {
            for( vit = mit->second.m_vecEquipItem.begin(); vit != mit->second.m_vecEquipItem.end();   )
            {
                KSimpleItem& petEqipItem = (*vit);
                // 펫이 끼고있는 아이템과 판매하는 아이템이 같다면
                if( petEqipItem.m_dwUID == kInvenItem->m_ItemUID )
                {
                    vit = mit->second.m_vecEquipItem.erase( vit );
                
                }
                else
                {
                    ++vit;
                }
            }
        }

        // 캐릭터의 펫정보 갱신
        for( int i = 0; i < (int)g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.size(); i++ )
        {
            KPetInfo &kPet = g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo[i].kPetInfo;
            if( kPet.m_dwUID == 0 )
            {
                kPet.Init();
            }
            else
            {
                std::map<GCITEMUID, KPetInfo>::iterator mit = g_MyD3D->m_mapPetInfo.find( kPet.m_dwUID );
                if( mit != g_MyD3D->m_mapPetInfo.end() )
                {
                    kPet = mit->second;
                }
            }
        }
	}

    g_KDSound.Play( "78" );
    g_pItemMgr->m_kInventory.RemoveItem( kInvenItem->m_ItemUID );
    g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
}

void KGCUIScene::DestroyMsgBox()
{
    m_pkMsgBox->Destroy();
}

void KGCUIScene::UpdateRoomTitle()
{
    //방옵션 변경 중에는 룸타이틀은 기본적으로 변경 할 수 있는 조건이어야 하므로 이름을 굳이 가져오지 않아도 된다.
    //g_pStateRoom->SetRoomName( m_pkChangeRoomTitleBoxS6->GetRoomName() );
    g_pStateRoom->SetRoomPassword( m_pkChangeRoomTitleBoxS6->GetRoomPasswd() );
}

bool KGCUIScene::IsAllStateMessageBox( EGCMsgBoxType eMBoxType_ )
{
    switch( eMBoxType_ )
    {
    case GC_MBOX_FAIRPLAY_REPORT:
    case GC_MBOX_GAME_INVENTORY:
        return true;
    }
    return false;
}

bool KGCUIScene::IsWaitMsgBox( EGCMsgBoxType eMBoxType_ )
{
    for(int i=0;i<(int)m_vecWaitMsgBox.size();++i)
    {
        if( m_vecWaitMsgBox[i].m_eMsgBoxType == eMBoxType_ )
            return true;
    }

    return false;
}
bool KGCUIScene::IsWaitMsgBox_Use( EGCMsgBoxUse eMBoxUse_ )
{
    for(int i=0;i<(int)m_vecWaitMsgBox.size();++i)
    {
        if( m_vecWaitMsgBox[i].m_eMsgBoxUse == eMBoxUse_ )
            return true;
    }

    return false;
}

bool KGCUIScene::IsCurMsgBox_Type( EGCMsgBoxType eMBoxType_ )
{
    if( m_kCurrMsgBoxInfo.m_eMsgBoxType == eMBoxType_ )
        return true;
    return false;
}

bool KGCUIScene::IsCurMsgBox_Use( EGCMsgBoxUse eMBoxUse_ )
{
    if( m_kCurrMsgBoxInfo.m_eMsgBoxUse == eMBoxUse_ )
        return true;
    return false;
}

bool KGCUIScene::DeleteAllWaitMsgBox( EGCMsgBoxType eMBoxType_ ) 
{	
	std::vector<KMsgBoxInfo>::iterator pos = m_vecWaitMsgBox.begin();

	for ( ; pos != m_vecWaitMsgBox.end(); ) 
	{
		if( pos->m_eMsgBoxType == eMBoxType_ )
		{
			pos = m_vecWaitMsgBox.erase( pos );
		}
		else
			++pos;
	}

	return true;
}

bool KGCUIScene::DeleteAllWaitMsgBox( void )
{
    if( m_vecWaitMsgBox.empty() ) {
        return false;
    }

    std::vector< KMsgBoxInfo >::iterator it = m_vecWaitMsgBox.begin();
    for( ; it != m_vecWaitMsgBox.end() ; ) { 
        it = m_vecWaitMsgBox.erase( it );
    }

    return true;
}

bool KGCUIScene::DeleteWaitMsgBox( EGCMsgBoxType eMBoxType_ )
{
    for(int i=0;i<(int)m_vecWaitMsgBox.size();++i)
    {
        if( m_vecWaitMsgBox[i].m_eMsgBoxType == eMBoxType_ )
        {
            m_vecWaitMsgBox.erase( m_vecWaitMsgBox.begin() + i );
            return true;
        }
    }

    return false;
}

bool KGCUIScene::DeleteWaitMsgBoxforUse( EGCMsgBoxUse m_eMsgBoxUse_ )
{
    for(int i=0;i<(int)m_vecWaitMsgBox.size();++i)
    {
        if( m_vecWaitMsgBox[i].m_eMsgBoxUse == m_eMsgBoxUse_ )
        {
            m_vecWaitMsgBox.erase( m_vecWaitMsgBox.begin() + i );
            return true;
        }
    }

    return false;
}

void KGCUIScene::ChargeBonus( KItem* pItemData, UINT nChargeFactor )
{
    KP2P::GetInstance()->Send_ChangBonusCoinReq( pItemData->m_ItemID, pItemData->m_ItemUID, nChargeFactor );

    g_MyD3D->WaitForServerAck( Result_Bonus_Charging, INT_MAX, 3000, INT_MAX );

    std::wstring str;
    switch( Result_Bonus_Charging )
    {
        // 0  성공.
    case 0:
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetReplacedString( STR_ID_BONUS_REFILL, "i", nChargeFactor ) );
        break;
        // 2  요청한 아이템은 보너스 코인 아이템이 아님.
    case 2:
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_BONUS_CHARGING_ERR1 ) );
        break;
        // 3  해당 보너스 아이템을 보유하고 있지 않음.
    case 3:
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_BONUS_CHARGING_ERR2 ) );
        break;
        // 4  보너스 코인 아이템의 수량 부족.
    case 4:
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_BONUS_CHARGING_ERR3 ) );
        break;
        // 5  더이상 보너스 포인트를 보유할수 없음.
    case 5:
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_BONUS_CHARGING_ERR4 ) );
        break;
        // 1  Best Character가 존재하지 않음_캐릭터 맵이 비어 있음.
    case 1:
        //알 수 없는 오류 발생
    default:
        std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_CHANGE_BONUS_COIN_ACK, Result_Bonus_Charging );	
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_BONUS_CHARGING_ERR5 ),
            strTemp );
        break;
    }
    m_pkMyInfoScene->UpdateGameData();
    Result_Bonus_Charging = INT_MAX;
}

void KGCUIScene::SetActiveWndInStop( KD3DWnd* pkWnd )
{
    m_pkActiveWnd = pkWnd;
}

void KGCUIScene::SetChildWndUpdateLock( bool bLock )
{
    for ( UINT i = 0; i < m_vecWnd.size(); ++i )
    {
        m_vecWnd[i]->SetWndUpdateLock( bLock );
    }
}

void KGCUIScene::SetLastColor(std::wstring strSelColor)
{
    m_pkLastColorItemUse->SetColor(strSelColor);
}

void KGCUIScene::AddDoNotOverlapedWnd( EGCMsgBoxType eID_ )
{
	m_setDoNotOverlapedWnd.insert( eID_ );
}

void KGCUIScene::OpenCollectionWindow( int iType/* = 0*/ )
{
    SetActiveMsgBox( m_pkEmoticonCollection );
    SetActiveMsgBox( m_pkTitleCollection );
    SetActiveMsgBox( m_pkPetCollection );

    // 컬렉션들의 리스트 갱신
    m_pkTitleCollection->ClearAllTitleIndex();
    m_pkTitleCollection->RefreshTitleList();
    m_pkTitleCollection->RefreshAllTitleIndex();

    // 윈도우 생성, 소멸 될 때 FrameMove를 1번이라도 반드시 거쳐야한다.
    // 렌더링 하지 않으면 FrmaeMove 체크를 안하므로 여기서 강제로 제어.
    m_pkEmoticonCollection->ToggleRender( true );
    m_pkTitleCollection->ToggleRender( true );
    m_pkPetCollection->ToggleRender( true );
    m_pkEmoticonCollection->FrameMove();
    m_pkTitleCollection->FrameMove();
    m_pkPetCollection->FrameMove();

	if( -1 == m_iLastCollectionWindow )
    SelectCollection( iType );
	else
		SelectCollection( m_iLastCollectionWindow );
}

void KGCUIScene::SelectCollection( int iType/* = 0*/  )
{
    m_pkEmoticonCollection->ToggleRender( false );
    m_pkTitleCollection->ToggleRender( false );
    m_pkPetCollection->ToggleRender( false );
    //m_pkCollectionItemInfoBox->ToggleRender( false );
    m_pkItemInformationBox->ToggleRender( false );

    switch( iType )
    {
    case GC_MBOX_EMOTICON_COLLECTION:
        m_pkEmoticonCollection->ToggleRender( true );
        m_pkCurMsgBox = m_pkEmoticonCollection;
        break;
    case GC_MBOX_TITLE_COLLECTION:
        m_pkTitleCollection->ToggleRender( true );
        m_pkCurMsgBox = m_pkTitleCollection;
        break;
    case GC_MBOX_PET_COLLECTION:
        m_pkPetCollection->ToggleRender( true );
        m_pkCurMsgBox = m_pkPetCollection;
        break;
    }

	m_iLastCollectionWindow = iType;
}

void KGCUIScene::CloseCollectionWindow( void )
{
    m_pkEmoticonCollection->Destroy();

    m_pkTitleCollection->Destroy();
    m_pkPetCollection->Destroy();
    m_pkCTConditionWindow->Destroy();

    // 윈도우 생성, 소멸 될 때 FrameMove를 1번이라도 반드시 거쳐야한다.
    // 렌더링 하지 않으면 FrmaeMove 체크를 안하므로 여기서 강제로 제어.
    m_pkEmoticonCollection->ToggleRender( true );
    m_pkTitleCollection->ToggleRender( true );
    m_pkPetCollection->ToggleRender( true );
    m_pkCTConditionWindow->ToggleRender( true );

    m_pkEmoticonCollection->FrameMove();
    m_pkTitleCollection->FrameMove();
    m_pkPetCollection->FrameMove();
    m_pkCTConditionWindow->FrameMove();

    //m_pkCollectionItemInfoBox->ToggleRender( false );
    m_pkItemInformationBox->ToggleRender( false );

    m_pkCTConditionWindow->ToggleRender( false );

	m_iLastCollectionWindow = -1;
}

void KGCUIScene::AddNateOnBuddyGroup( std::wstring strGroupName )
{
    SiKFriendShip()->AddBuddyGroupReq( strGroupName );

    g_MyD3D->WaitForServerAck(Result_NateOn_Add_Group, INT_MAX, 30000, TIME_OUT_VALUE );
    switch( Result_NateOn_Add_Group )
    {
    case 0:
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NATEON_ADD_GROUP_SUCCESS ), L"", GC_MBOX_USE_NORMAL, 0, 0, false, true );
            m_pkNateOn->UpdateTreeData();
            break;
        }
	case 10: // 해당 이름을 가진 그룹이 존재함
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NATEON_ADD_GROUP_ALREADY_REG ), L"", GC_MBOX_USE_NORMAL, 0, 0, false, true );
		break;
    case 12: // 최대 그룹수 초과
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NATEON_ADD_GROUP_ERROR2 ), L"", GC_MBOX_USE_NORMAL, 0, 0, false, true );
            break;
        }
    case 13: // 그룹명 문자열 길이 초과
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NATEON_ADD_GROUP_ERROR3 ), L"", GC_MBOX_USE_NORMAL, 0, 0, false, true );
            break;
        }
    case 25: // 쿼리에 사용할수 없는 문자 포함(빈칸, 탭, \')
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NATEON_ADD_GROUP_ERROR4 ), L"", GC_MBOX_USE_NORMAL, 0, 0, false, true );
            break;
        }
    case 1: // 내 정보가 존재하지 않음
    case 9: // 친구 리스트가 요청된 적이 없음
    default:
        {
            std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KMSEvent::EMS_S2_ADD_GROUP_ACK, Result_NateOn_Add_Group );	
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NATEON_ADD_GROUP_ERROR ),
                strTemp, GC_MBOX_USE_NORMAL, 0, 0, false, true );
            break;
        }
    }
    Result_NateOn_Add_Group = INT_MAX;
}

void KGCUIScene::RenameNateOnBuddyGroup( int nGroupID, std::wstring strGroupName )
{
    SiKFriendShip()->RenameGroupReq( nGroupID, strGroupName );

    g_MyD3D->WaitForServerAck( Result_NateOn_Rename_Group, INT_MAX, 30000, TIME_OUT_VALUE );

    switch( Result_NateOn_Rename_Group )
    {
    case 0:
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NATEON_RENAME_GROUP_SUCCESS ), L"", GC_MBOX_USE_NORMAL, 0, 0, false, true );
            m_pkNateOn->UpdateTreeData();
            break;
        }
	case 10: // 해당 그룹이 존재함
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NATEON_ADD_GROUP_ALREADY_REG ), L"", GC_MBOX_USE_NORMAL, 0, 0, false, true );
			break;
		}
    case 13: // 그룹명 문자열 길이 초과
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NATEON_ADD_GROUP_ERROR3 ), L"", GC_MBOX_USE_NORMAL, 0, 0, false, true );
            break;
        }
    case 25: // 쿼리에 사용할수 없는 문자 포함(빈칸, 탭, \')
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NATEON_ADD_GROUP_ERROR4 ), L"", GC_MBOX_USE_NORMAL, 0, 0, false, true );
            break;
        }
    case 1: // 내 정보가 존재하지 않음
    case 11: // 해당그룹이 없음
    case 14: // 바꾸려는 그룹 이름이 같음
    case 15: // 기본그룹의 이름을 변경할 수 없음
    default: // 알 수 없는 오류
        {
            std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KMSEvent::EMS_S2_RENAME_GROUP_ACK, Result_NateOn_Rename_Group );	
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NATEON_RENAME_GROUP_ERROR ),
                strTemp, GC_MBOX_USE_NORMAL, 0, 0, false, true );
            break;
        }
    }

    Result_NateOn_Rename_Group = INT_MAX;
}

void KGCUIScene::DeleteNateOnBuddyGroup( int nGroupID )
{
	// 여기서 솔로가 아니면 그룹 내의 사람들 검색해보고 내 커플이 있으면 다른 그룹으로 옮겨야될거 가튼..
	if( false == SiKGCCoupleSystem()->Am_I_Solo()
		&& KP2P::GetInstance()->m_kMsgServer != NULL )
	{
		JIF( KP2P::GetInstance()->m_kMsgServer != NULL );

		KBuddy kBuddy;
		SiKFriendShip()->GetFriendBuddyByNickName( SiKGCCoupleSystem()->GetNickName_of_my_honey(), kBuddy );
		
		if( nGroupID == SiKFriendShip()->GetFriendBuddyGroupID( kBuddy.GetUID() ) )
		{
			SiKFriendShip()->MoveBuddyReq( nGroupID, 10, kBuddy.GetUID() );	// '친구' 그룹 번호 10인거 같은데.. define 되있는지 못찾아서 그냥 씀.
		}	
	}

	std::vector< DWORD> vecContactList = SiKFriendShip()->GetContactFriendList();
	std::vector< DWORD>::iterator itor;

	for(itor = vecContactList.begin(); itor != vecContactList.end(); ++itor)
	{
		if( nGroupID == SiKFriendShip()->GetFriendBuddyGroupID( (*itor) ) )
		{
			SiKFriendShip()->MoveBuddyReq( nGroupID, 10, (*itor) );
		}
	}

    SiKFriendShip()->DeleteGroupReq( nGroupID );

    g_MyD3D->WaitForServerAck( Result_NateOn_Delete_Group, INT_MAX, 30000, TIME_OUT_VALUE );

    switch( Result_NateOn_Delete_Group )
    {
    case 0:
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NATEON_DELETE_GROUP_SUCCESS ), L"", GC_MBOX_USE_NORMAL, 0, 0, true, false );
            m_pkNateOn->UpdateTreeData();
            break;
        }
    case 1: // 내 정보가 존재하지 않음
    case 11: // 해당그룹이 없음
    case 15: // 기본그룹을 삭제하려 함
    default: // 알 수 없는 오류
        {
            std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KMSEvent::EMS_S2_DELETE_GROUP_ACK, Result_NateOn_Delete_Group );	
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NATEON_DELETE_GROUP_ERROR ),
                strTemp, GC_MBOX_USE_NORMAL, 0, 0, true, false );
            break;
        }
    }

    Result_NateOn_Delete_Group = INT_MAX;
}

void KGCUIScene::BlockBuddy( std::wstring strNickName )
{
	// 커플인 경우 블록 하지 않는다.
 	if( 0 == wcsicmp(strNickName.c_str(), SiKGCCoupleSystem()->GetNickName_of_my_honey().c_str()) )
 	{					
 		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM40 )
			, L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );

		return;
	}

	std::vector< DWORD > vecContactFriendList = SiKFriendShip()->GetContactFriendList();

	if(!vecContactFriendList.empty())
	{
		KBuddy kFindBuddy;
		SiKFriendShip()->GetFriendBuddyByNickName( strNickName, kFindBuddy);
		if(SiKFriendShip()->FindUIDInContactFriendList( kFindBuddy.GetUID() ) )
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_CAN_NOT_DELETE_RECOMMENDER )
				, L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );

			return;
		}
	}

	SiKFriendShip()->BlockNoFriendReq( strNickName );

    g_MyD3D->WaitForServerAck( Result_NateOn_Block_Buddy, INT_MAX, 30000, TIME_OUT_VALUE );

    switch( Result_NateOn_Block_Buddy )
    {
    case 0:
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NATEON_BLOCK_BUDDY_SUCCESS ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            g_pkUIScene->m_pkNateOn->UpdateTreeData();
            break;
        }
	case 2: // 상대방 정보가 존재하지 않음
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NATEON_DONT_FIND ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		break;
	case 6: // 자기 자신을 추가하려 함
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NATEON_BLOCK_ME ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		break;
	case 1: // 내 정보가 존재하지 않음
    case 7: // 이미 요청한 상태임
    case 25: // 해당 그룹에 존재하지 않는 유저임
    case 11: // 해당하는 그룹이 없음
    case 16: // 해당 그룹에 이미 등록된 유저임
    case 27: // 그룹내 해당유저를 찾을 수 없음
    default: // 알 수 없는 오류
        {
            std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KMSEvent::EMS_S2_BLOCK_MOVE_FRIEND_ACK, Result_NateOn_Block_Buddy );	
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NATEON_BLOCK_BUDDY_ERROR ),
                strTemp, KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            break;
        }
    }

    Result_NateOn_Block_Buddy = INT_MAX;
}

void KGCUIScene::ChangeEmoticon()
{
    g_MyD3D->WaitForServerAck( Result_NateOn_Change_Emoticon, INT_MAX, 30000, TIME_OUT_VALUE );

    switch( Result_NateOn_Change_Emoticon )
    {
    case 0:
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NATEON_CHANGE_EMOTICON_SUCCESS ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            break;
        }
    case 1: // 내 정보가 존재하지 않음
    case 9: // 메신저 접속시 이모티콘 정보 검색 요청을 한적이 없음
    case 18: // 변경될 이모티콘을 소유하고 있지 않음
    default: // 알 수 없는 오류
        {
            std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KMSEvent::EMS_S2_SET_EMOTICON_ACK, Result_NateOn_Change_Emoticon );	
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NATEON_CHANGE_EMOTICON_ERROR ),
                strTemp, KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            break;
        }
    }

    Result_NateOn_Change_Emoticon = INT_MAX;
}

void KGCUIScene::LuaSendGiftsReq( int iEventID_ )
{
	std::set<int>::iterator itor = g_kGlobalValue.m_setGiftsEvent.find(iEventID_);

	if( itor != g_kGlobalValue.m_setGiftsEvent.end() )
	{
		KP2P::GetInstance()->Send_NpcGiftReq(iEventID_);
        g_kGlobalValue.m_setGiftsEvent.erase(iEventID_);
	}
    else
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_GITFS_MESSAGE_ALREADY_RECEIVED), L"",
            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
    }
}

bool KGCUIScene::RemoveWaitMessageBox( EGCMsgBoxType eMsgBox_ )
{
    std::vector<KMsgBoxInfo>::iterator vit;

    for( vit = m_vecWaitMsgBox.begin() ; vit != m_vecWaitMsgBox.end() ; ++vit )
    {
        if( (*vit).m_eMsgBoxType == eMsgBox_ )
        {
            m_vecWaitMsgBox.erase( vit );
            return true;
        }
    }
    return false;
}

KD3DDialog* KGCUIScene::GetDialogPtr( EGCMsgBoxType eMboxType_ )
{
    KD3DDialog* pDialog = NULL;
    switch ( eMboxType_ )
    {
    case GC_MBOX_OK:
    case GC_MBOX_YES_NO:
    case GC_MBOX_EXIT_GAME:
    case GC_MBOX_EXIT_ROOM:
    case GC_PET_FEEDING:
    case GC_MBOX_EXTEND_INVENTORY:
    case GC_MBOX_EXIT_SQUARE:
    case GC_MBOX_GP_CAPSULE_MESSAGE_OK:
    case GC_MBOX_TOURNAMENT_BOX:
    case GC_MBOX_LEVEL_RESET_ITEM:
    case GC_MBOX_EXTEND_LOOK_INVENTORY:
    case GC_MBOX_TUTORIAL_EXIT:
	case GC_MBOX_OK_CANSEL_BOX:
	case GC_MBOX_JOIN_CANSEL_BOX:
	case GC_MBOX_INFO_CANSEL_BOX:
        {
            pDialog = (KD3DDialog*)m_pkMsgBox;
            break;
        }
    case GC_MBOX_OK2:
        {
            pDialog = (KD3DDialog*)m_pkMsgBox2;
            break;
        }
    case GC_MBOX_GP_CAPSULE_DLG:
        {
            pDialog = (KD3DDialog*)m_pkGPCapsuleDlg;
            break;
        }	
    case GC_MBOX_GP_CAPSULE_OBTAIN_DLG:
        {
            pDialog = (KD3DDialog*)m_pkGPCapsuleObtainWnd;
            break;
        }	
    case GC_MBOX_BUY_CASH_ITEM:
        {
            pDialog = (KD3DDialog*)m_pBuyCashItemWnd;
            break;
        }
    case GC_MBOX_PASSWORD:
        {
            pDialog = (KD3DDialog*)m_pkPasswdBox;
            break;
        }
    case GC_MBOX_SEASON2_INVITE_REQ:
        {		
            pDialog = (KD3DDialog*)m_pkInviteRequestBox;
            break;
        }
    case GC_MBOX_SEASON2_INVITE:
        {
            pDialog = (KD3DDialog*)m_pkInviteBox;
            break;
        }
    case GC_MBOX_LOADING:
        {
            pDialog = (KD3DDialog*)m_pkLoadingBox;
            break;
        }
    case GC_MBOX_ITEM_RECEIVE:
        {
            pDialog = (KD3DDialog*)m_pkItemReceiveBox;
            break;
        }
    case GC_MBOX_GEM_BUY:
        {
            pDialog = (KD3DDialog*)m_pkGemBuyBox;
            break;
        }
    case GC_MBOX_NEW_COMBO_BOX_SHOP:
    {
        pDialog = (KD3DDialog*)m_pkNewComboBox;
        break;
    }
    case GC_MBOX_BONUS_CHARGE:
        {
            pDialog = (KD3DDialog*)m_pkBonusChargeBox;
            break;
        }
    case GC_MBOX_NICK_NAME:
        {
            pDialog = (KD3DDialog*)m_pkNickNameBox;
            break;
        }
    case GC_MBOX_OPTION:
        {
            pDialog = (KD3DDialog*)m_pkOption;
            break;
        }
    case GC_MBOX_COLOR:
        {
            pDialog = (KD3DDialog*)m_pkColorDlg;
            break;
        }
    case GC_MSN_DLG:
        {
            pDialog = (KD3DDialog*)m_pkNateOn;
            break;
        }
    case GC_MBOX_NATEON_TALK_INVITE:
        {
            pDialog = (KD3DDialog*)m_pkNateOnInvite;
            break;
        }
    case GC_MBOX_NATEON_MESSAGE:
        {
            pDialog = (KD3DDialog*)m_pkNateOnMessage;
            break;
        }
    case GC_PET_PREVIEW:
        {			
            pDialog = (KD3DDialog*)m_pkPetPreview;
            break;
        }
    case GC_EVENTBOX:
        {
            pDialog = (KD3DDialog*)m_pkEventBox;
            break;
        }
    case GC_MBOX_REPETITION_EVENT_BOX:
        {
            pDialog = (KD3DDialog*)m_pkRepetitionEventBox;
            break;
        }
    case GC_MBOX_PET_BOX:
        {
            pDialog = (KD3DDialog*)m_pkPetEvolutionBox;
            break;
        }
    case GC_MBOX_PET_NAME_SELECT:        // 부화는 이름을 선택할 필요가 없기 때문에 얘들과 상관없다..
        {
            pDialog = (KD3DDialog*)m_pkSelectPetNameBox;

            break;
        }
    case GC_MBOX_TUTORIAL_OPTION_NEW:
        {
            pDialog = (KD3DDialog*)m_pkNewTutorialOption;
            break;
        }
    case GC_MBOX_ATTENDANCECALENDAR:
        {
            pDialog = (KD3DDialog*)m_pkAttendanceCalendar;
            break;
        }
    case GC_MBOX_PERFECT_ATTENDANCE:
        {
            pDialog = (KD3DDialog*)m_pkPerfectAttendance;
            break;
        }
    case GC_MBOX_LUCKY_DAY_RECEIVE_BOX:
        {
            pDialog = (KD3DDialog*)m_pkLuckyDayReceiveBox;
            break;
        }
    case GC_MBOX_FAIRPLAY_REPORT:
        {
            pDialog = (KD3DDialog*)m_pkFairPlayReport;
            break;
        }
    case GC_MBOX_GRADE_SHOW:
        {
            pDialog = (KD3DDialog*)m_pkGradeBox;
            break;
        }
    //case GC_MBOX_DUNGEON_ROOM_LIST:
    //    {
    //        pDialog = (KD3DDialog*)m_pkDungeonRoomlist;
    //        break;
    //    }
    case GC_MBOX_CHANNEL_LIST:
        {
            pDialog = (KD3DDialog*)m_pkChannelListBox;
            break;
        }
    case GC_MBOX_NEONSIGN_SELECT:
        {
            pDialog = (KD3DDialog*)m_pkNeonsignSelectBox;
            break;
        }
    case GC_MBOX_NEONSIGN_PREVIEW:
        {
            pDialog = (KD3DDialog*)m_pkNeonsignPreview;
            break;
        }
    case GC_MBOX_CONDITION_WINDOW:
        {
            pDialog = (KD3DDialog*)m_pkCTConditionWindow;
            break;
        }
    case GC_MBOX_CONNECTION_MSG:
        {
            pDialog = (KD3DDialog*)m_pkConnectionMsg;
            break;
        }
    case GC_MBOX_BADUSER_INFO:
        {
            pDialog = (KD3DDialog*)m_pkConnectionMsg;
            break;
        }
    case GC_MBOX_RANKING:
        {
            pDialog = (KD3DDialog*)m_pkRanking;
            break;
        }
    case GC_MBOX_GUILDSHOP_DLG:
        {
            pDialog = (KD3DDialog*)m_pkGuildShop;
            break;
        }
    case GC_MBOX_GUILD_LEVEL_LIST_DLG:
        {
            pDialog = (KD3DDialog*)m_pkGuildLevelListDlg;
            break;
        }
    case GC_MBOX_TREASURE_ITEM_BOX:
        {
            pDialog = (KD3DDialog*)m_pkTreaureItemBox;
            break;
        }
    case GC_MBOX_WELCOMEBACK_DLG:
        {
            pDialog = (KD3DDialog*)m_pkWelcomeBackDlg;
            break;
        }
    case GC_MBOX_BUY_VIRTUAL_CASH:
        {
            pDialog = (KD3DDialog*)m_pkAskUseVirtualCash;
            break;
        }
    case GC_MBOX_MINIGAME_DLG:
        {
            pDialog = (KD3DDialog*)m_pkMiniGameDlg;
            break;
        }
    case GC_MBOX_NATEON_INPUT_BOX:
        {
            pDialog = (KD3DDialog*)m_pkNateOnInputDlg;
            break;
        }
    case GC_MBOX_EMOTICON_SELECT_DLG:
        {
            pDialog = (KD3DDialog*)m_pkEmoticonSelect;
            break;
        }
    case GC_MBOX_NATEON_ADD_BUDDY:
        {
            pDialog = (KD3DDialog*)m_pkNateOnAddFriend;
            break;
        }
    case GC_MBOX_NATEON_BLOCK_TO_ADD_BUDDY:
        {
            pDialog = (KD3DDialog*)m_pkBlockToAddFriend;
            break;
        }
    case GC_MBOX_SKILLTREE:
        {
            pDialog = (KD3DDialog*)m_pkSkillTree;
            break;
        }
    case GC_MBOX_NEW_CLEAR_SEAL_BEAD:
    {
        if (g_kGlobalValue.m_iCurrentGachaType == 0) {
            pDialog = (KD3DDialog*)GetNewClearSealDlg();
        }
        else {
            pDialog = (KD3DDialog*)GetClearSealDlg();
        }
        break;
    }
    case GC_MBOX_COUPLE_VIEWER:
        {
            pDialog = (KD3DDialog*)m_pkCoupleViewer;
            break;
        }
    case GC_MBOX_COUPLE_ACCEPT:
        {
            pDialog = (KD3DDialog*)m_pkCoupleAccept;
            break;
        }
    case GC_MBOX_COUPLE_PROPOSE:
        {
            pDialog = (KD3DDialog*)m_pkCouplePropose;
            break;
        }
    case GC_MBOX_COUPLE_WAIT:
        {
            pDialog = (KD3DDialog*)m_pkCoupleWait;
            break;
        }
    case GC_MBOX_COUPLE_MESSAGE_OK:
    case GC_MBOX_COUPLE_MESSAGE_OK_CANCLE:
        {
            pDialog = (KD3DDialog*)m_pkCoupleMessage;
            break;
        }
    case GC_MBOX_COUPLE_GOODBYE:
        {
            pDialog = (KD3DDialog*)m_pkCoupleGoodbye;
            break;
        }
    case GC_MBOX_PET_COSTUME_INVEN:
        {
            pDialog = (KD3DDialog*)m_pkPetCostumeInven;
            break;
        }
    case GC_MBOX_CONTRIBUTION_DLG:
        {
            pDialog = (KD3DDialog*)m_pkContributionDlg;
            break;
        }
    case GC_MBOX_CONTRIBUTION_BOX:
        {
            pDialog = (KD3DDialog*)m_pkContributionBox;
            break;
        }
    case GC_MBOX_VIP_GACHA_BOX:
        {
            pDialog = (KD3DDialog*)m_pkVipGachaBox;
            break;
        }
    case GC_MBOX_HAPPYNEWYEAR_NPC_DLG:
        {
            pDialog = (KD3DDialog*)m_pkHappyNewYearNPCDlg;
            break;
        }
    case GC_MBOX_LOGOUT_BOX:
        {
            pDialog = (KD3DDialog*)m_pkLogoutBox;
            break;
        }
    //case GC_MBOX_VALENTINEDAY_DLG:
    //    {
    //        pDialog = (KD3DDialog*)m_pkValentineDayDlg;
    //        break;
    //    }
    case GC_MBOX_AGREEMENT:
        {
            pDialog = (KD3DDialog*)m_pkAgreement;
            break;
        }
	case GC_MBOX_AGREEMENT2:
		{
			pDialog = (KD3DDialog*)m_pkAgreement1301;
			break;
		}
    case GC_MBOX_RECOMMEND:
        {
            // 신청 온거 있으면 리스트 박스 먼저
            pDialog = (KD3DDialog*)m_pkRecommendList;
            break;
        }
    case GC_MBOX_HALLOWEEN_DLG:
        {
            pDialog = (KD3DDialog*)m_pkDicePlay;
            break;
        }
    case GC_MBOX_BINGO_DLG:
        {
            pDialog = (KD3DDialog*)m_pkBingo;
            break;
        }
    case GC_MBOX_QUIZ_DLG:
        {
            pDialog = (KD3DDialog*)m_pkQuiz;
            break;
        }
    case GC_MBOX_PUZZLE_DLG:
        {
            pDialog = (KD3DDialog*)m_pkPuzzle;
            break;
        }
	case GC_MBOX_GAWIBAWIBO_DLG:
		{
			pDialog = (KD3DDialog*)m_pkGawibawiboGame;
			break;
		}

	case GC_MBOX_SOCKS_TREE_DLG:
		{
			pDialog = static_cast<KD3DDialog*>(m_pkSocksTreeDlg);
			break;
		}
    case GC_MBOX_COUPON_NPC_DLG:
        {
            pDialog = (KD3DDialog*)m_pkCouponNPCDlg;
            break;
        }
    case GC_MBOX_COUPON_ITEM_OBTAIN_OK:
        {
            pDialog = (KD3DDialog*)m_pkGPCapsuleObtainWnd;
            break;
        }
    case GC_MBOX_SUBSCRIPTION_DLG:
        {
            pDialog = (KD3DDialog*)m_pkSubscriptionDlg;
            break;
        }
    case GC_MBOX_SUBSCRIPTION_BOX:
        {
            pDialog = (KD3DDialog*)m_pkSubscriptionBox;
            break;
        }
    //case GC_MBOX_GWC_DONATION_DLG:
    //    {
    //        pDialog = (KD3DDialog*)m_pkGWCDonationDlg;
    //        break;
    //    }
    //case GC_MBOX_GWC_STATE_DLG:
    //    {
    //        pDialog = (KD3DDialog*)m_pkGWCShowStateDlg;
    //        break;
    //    }
    case GC_MBOX_BUY_CASH_BOX:
        {
            pDialog = (KD3DDialog*)m_pkBuyCashBox;
            break;
        }
    case GC_MBOX_COOPERATION_EVENT_BOX:
        {
            pDialog = (KD3DDialog*)m_pkCooperationEventDlg;
            break;
        }
    case GC_MBOX_GUILDMARK_REGIST_DLG:
        {
            pDialog = (KD3DDialog*)m_pkGuildMarkRegistDlg;
            break;
        }
    case GC_MBOX_GUILD_DLG:
        {
            pDialog = (KD3DDialog*)m_pkGuildDlg;
            break;
        }
    case GC_MBOX_GUILD_ENTER_DLG:
        {
            pDialog = (KD3DDialog*)m_pkGuildEnterDlg;
            break;
        }
    case GC_MBOX_GUILD_LOBBY_DLG:
        {
            pDialog = (KD3DDialog*)m_pkGuildLobbyDlg;
            break;
        }
    case GC_MBOX_GUILD_LIST_DLG:
        {			
            pDialog = (KD3DDialog*)m_pkGuildListDlg;
            break;
        }
    case GC_MBOX_GUILD_CREATE_DLG:
        {
            pDialog = (KD3DDialog*)m_pkGuildCreateDlg;
            break;
        }
    case GC_MBOX_GUILD_JOIN_REQ_MBOX:
        {
            pDialog = (KD3DDialog*)m_pkGuildJoinReqMbox;
            break;
        }
    case GC_MBOX_GUILD_PUBLIC_MBOX:
        {
            pDialog = (KD3DDialog*)m_pkGuildPublicMbox;
            break;
        }
    case GC_MBOX_GUILD_INTRODUCE_EDIT:
        {
            pDialog = (KD3DDialog*)m_pkGuildIntroduceEdit;
            break;
        }
    case GC_MBOX_MONSTER_CARD_BOOK:
        {
            pDialog = (KD3DDialog*)m_pkMonsterCardBook;
            break;
        }
    case GC_MBOX_ITEM_SOCKET_MANAGEMENT_DLG:
        {
            pDialog = (KD3DDialog*)m_pkItemSocketManagementDlg;
            break;
        }
    case GC_MBOX_GIFT_ITEM_BOX:
        {
            pDialog = (KD3DDialog*)m_pkGiftItemBox;
            break;
        }
	case GC_MBOX_CHARACTER_SELECT_BOX:
		{
			pDialog = (KD3DDialog*)m_pkCharacterSelectBox;
			break;
		}
    case GC_MBOX_COORDI_CHANGE_DLG:
        {
            pDialog = (KD3DDialog*)m_pkCoordiChangeDlg;
            break;
        }
    case GC_MBOX_SELL_COUNT_ITEM:
        {
            pDialog = (KD3DDialog*)m_pkSellCountItemDlg;
            break;
        }
	case GC_MBOX_GAMBLE_BOX:
		{			
			pDialog = (KD3DDialog*)m_pkGambleBox;
			break;
		}
    case GC_MBOX_GET_NEW_CHARACTER:
        {
            pDialog = (KD3DDialog*)m_pkCharGetInfoDlg;
            break;
        }
	case GC_MBOX_SIZING_OK:
		{
			pDialog = (KD3DDialog*)m_pkMsgSizingBox;
			break;
		}
    case GC_MBOX_REPLAY_RECORD:
        {
            pDialog = (KD3DDialog*)m_pkReplayRecordDlg;
            break;
        }
    case GC_MBOX_REPLAY_BOX_NEW:
        {
            pDialog = (KD3DDialog*)m_pkReplayBoxNew;
            break;
        }
    case GC_MBOX_HERO_DUNGEON_DLG:
        {
            pDialog = (KD3DDialog*)m_pkHeroDungeonDlg;
            break;
        }
    case GC_MBOX_MISSION_UI_S4_DLG:
        {
            pDialog = (KD3DDialog*)m_pkMissionUI_S4;
            break;
        }
        break;
    case GC_MBOX_MISSION_PREVIEW_S4_DLG:
        {
            pDialog = (KD3DDialog*)m_pkMissionEquipPreview_S4;
            break;
        }
        break;
    case GC_MBOX_WEAPON_CHANGE_DLG:
        {
            pDialog = (KD3DDialog*)m_pkWeaponChangeBox;
            break;
        }
        break;
	case GC_MBOX_ATTRIBUTE_SELECT_BOX:
		{
			pDialog = (KD3DDialog*)m_pkAttributeSelectBox;
			break;
		}
		break;
    case GC_MBOX_NICKNAME_CAMPAIGN_BOX:
        {
            pDialog = (KD3DDialog*)m_pkNicknameCampaignBox;
            break;
        }
    case GC_MBOX_PLANT_TREE_DLG:
        {
            pDialog = (KD3DDialog*)m_pkPlantTree;
            break;
        }
    case GC_MBOX_PLANT_TREE_REWARD_LIST_VIEW_DLG:
        {
            pDialog = (KD3DDialog*)m_pkPlantTreeRewardItemListView;
            break;
        }
    case GC_MBOX_CHOICE_DROP:
        {
            pDialog = (KD3DDialog*)m_pkChoiceDrop;
            break;
        }
    case GC_MBOX_FASHION_CATALOG_BOX:
        {
            pDialog = (KD3DDialog*)m_pkCoordiCatalogDlg;
            break;
        }
    case GC_MBOX_PACKAGE_PREVIEW:
        {
            pDialog = (KD3DDialog*)m_pkPackagePreview;
            break;
        }
    case GC_MBOX_VIP_REWARD:
        {
            pDialog = (KD3DDialog*)m_pkVipReward;
            break;
        }
    case GC_USER_MIGRATION_DLG:
        {
            pDialog = (KD3DDialog*)m_pkUserMigrationDlg;
            break;
        }
    case GC_MBOX_MAGIC_BOX:
        {
            pDialog = (KD3DDialog*)m_pkMagicBox;
            break;
        }
    case GC_MBOX_RICKOON_TORNADO_DLG:
        {
            pDialog = (KD3DDialog*)m_pkRickoonTornado;
            break;
        }
    case GC_MBOX_EVENT_SPHINX_DLG:
        {
            pDialog = (KD3DDialog*)m_pkSphinxDlg;
            break;
        }
    case GC_MBOX_EVENT_MINIGAME_LIST_DLG:
        {
            pDialog = (KD3DDialog*)m_pkEventMinigameList;
            break;
        }
    case GC_MBOX_DOTA_SHOP:
        {
            pDialog = (KD3DDialog*)m_pkDotaShop;
            break;
        }		
#if defined ( PARTY_SYSTEM )
    case GC_MBOX_PARTY_MATCH:
        {
            pDialog = (KD3DDialog*)m_pkPartyMatch;
            break;
        }
    case GC_MBOX_PARTY_DUNGEON:
        {
            pDialog = (KD3DDialog*)m_pkPartyDungeon;
            break;
        }
    case GC_MBOX_PARTY_WINDOW:
        {
            pDialog = (KD3DDialog*) m_pkPartyWnd;
            break;
        }
    case GC_MBOX_PARTY_INVITE:
        {
            pDialog = (KD3DDialog*)m_pkPartyInviteMsgBox;
            break;
        }
#endif
    case GC_MBOX_AGIT_SHOP:
        {
            pDialog = (KD3DDialog*)m_pkAgitShop;
            break;
        }
  case GC_MBOX_GAME_INVENTORY:
        {
            pDialog = (KD3DDialog*)m_pkGameInventory;
            break;
        }
    case GC_MBOX_MANTIS_REPORT_DLG:
        {
            pDialog = (KD3DDialog*)m_pkMantisReportDlg;
            break;
        }
    case GC_MBOX_SHUTDOWN:
        {
            pDialog = (KD3DDialog*)m_pkShutDownDlg;
            break;
        }
    case GC_MBOX_FORGE_DLG:
        {
            pDialog = (KD3DDialog*)m_pkForgeDlg;
            break;
        }		
    case GC_MBOX_SURVEY_DLG:
        {
            pDialog = (KD3DDialog*)m_pkSurveyDlg;
            break;
        }
    case GC_MBOX_GWC_EVENT_DLG:
        {
            pDialog = (KD3DDialog*) m_pkGwcEventdlg;
            break;
        }
    case GC_MBOX_GWC_EVENT_CATALOG:
        {
            pDialog = (KD3DDialog*) m_pkGwcRewardCatalogDlg;
            break;
        }
    case GC_MBOX_CHRISTMAS_GIFT_DLG:
        {
            pDialog = (KD3DDialog*)m_pkChristmasGiftDlg;
            break;
        }
    case GC_MBOX_POST_LETTER:
        {
            pDialog = (KD3DDialog*)m_pkPostLetterDlg;
            break;
        }
    case GC_MBOX_EVENT_ECLIPSE_PLOT_DLG:
        {
            pDialog = (KD3DDialog*)m_pkEventEclipsePlotDlg;
            break;
        }
    case GC_MBOX_ECLIPSE_OLD_BOOKS_DLG:
        {
            pDialog = (KD3DDialog*)m_pkEclipseOldBooks;
            break;
        }
    case GC_MBOX_WELCOMEBACK_DLG_NEW:
        {
            pDialog = (KD3DDialog*)m_pkWelcomeBackNewDlg;
            break;
        }
    case GC_MBOX_ITEM_LEVEL_DOWN_DLG:
        {
            pDialog = (KD3DDialog*)m_pkItemLevelDownDlg;
            break;   
        }
    case GC_MBOX_HOT_TIME_SEARCH_DLG:
        {
            pDialog = (KD3DDialog*)m_pkHotTimeSearchDlg;
            break;   
        }   
	case GC_MBOX_MONSTER_CARD_COMPOSE_DLG:
		{
			pDialog = (KD3DDialog*)m_pkMonsterCardCompose;
			break;   
		}   
	case GC_MBOX_NEW_NECKLACE_ENCHANT_DLG:
		{
			pDialog = (KD3DDialog*)m_pkNewNecklaceEnchantDlg;
			break;   
		}   
	case GC_MBOX_DEPOT:
		{
			pDialog = (KD3DDialog*)m_pkDepot;
			break;
		}
    case GC_MBOX_TRANSFER_MISSION_SCROLL_GUIDE_DLG:
        {
            pDialog = (KD3DDialog*)m_pkTransferMissionScrollGuideDlg;
            break;
        }
	case GC_MBOX_SONGKRAN_DLG:
		{
			pDialog = (KD3DDialog*)m_pkSongkranDlg;
			break;
		}
	case GC_MBOX_EVENT_ALARM_DLG:
		{
			pDialog = (KD3DDialog*)m_pkEventAlarmDlg;
			break;
		}		
	case GC_MBOX_SONGKRAN_REWARD_DLG:
		{
			pDialog = (KD3DDialog*)m_pkSongkranRewardDlg;
			break;
		}
    case GC_MBOX_GC_ADVENTURE_DLG:
        {
            pDialog = (KD3DDialog*)m_pkGcAdventureDlg;
            break;
        }		
    case GC_MBOX_OLYMPIC_DLG:
        {
            pDialog = (KD3DDialog*)m_pkOlympicDlg;
            break;
        }
    case GC_MBOX_OLYMPIC_GAWIBAWIBO_DLG:
        {
            pDialog = (KD3DDialog*)m_pkOlympicGawibawiboDlg;
            break;
        }
    case GC_MBOX_OLYMPIC_DICE_DLG:
        {
            pDialog = (KD3DDialog*)m_pkOlympicDicePlayDlg;
            break;
        }
    case GC_MBOX_OLYMPIC_SONGKRAN_DLG:
        {
            pDialog = (KD3DDialog*)m_pkOlympicSongkranDlg;
            break;
        }
	case GC_MBOX_ROOM_LIST_DLG_S6:
		{
			pDialog = (KD3DDialog*)m_pkRoomListDlgS6;
			break;
		}
	case GC_MBOX_ROOM_DUNGEON_SELECT_DLG_S6:
		{
			pDialog = (KD3DDialog*)m_pkRoomDungeonSelectDlgS6;
			break;
		}
	case GC_MBOX_ROOM_CREATE_DLG_S6:
		{
			pDialog = (KD3DDialog*)m_pkRoomCreateDlgS6;
			break;
		}
    case GC_MBOX_GC9th_EVENT_DLG:
        {
            pDialog = (KD3DDialog*)m_pk9thEventDlg;
            break;
        }
    case GC_MBOX_LEVEL_ITEM_DLG:
        {
            pDialog = (KD3DDialog*)m_pkLevelItemDlg;
            break;
        }
    case GC_MBOX_ITEM_COMBINATION_DLG:
        {
            pDialog = (KD3DDialog*)m_pkItemCombinationDlg;
            break;
        }
	case GC_MBOX_LOTTERY:
		{
			pDialog = (KD3DDialog*)m_pkLottery;
			break;
		}		
	case GC_MBOX_ROOM_QUICK_JOIN:
		{
			pDialog = (KD3DDialog*)m_pkQuickJoinDlg;
			break;
		}	
		
    case GC_MBOX_ROOM_USER_INFO_POPUP:
        {
            pDialog = (KD3DDialog*)m_pkRoomUserInfoPopup;
            break;
        }
    case GC_MBOX_ITEM_CREATION_SELECT_BOX:
        {
            pDialog = (KD3DDialog*)m_pkItemCreationSelectBox;
            break;
        }
	case GC_MBOX_MATCH_MAP_SELECT_BOX :
		{
			pDialog = (KD3DDialog*)m_pkMatchMapSelectDlg;
			break;
		}
	case GC_MBOX_MATCH_INVITE_USER_LIST_BOX :
		{
			pDialog = (KD3DDialog*)m_pkMatchInviteUserList;
			break;
		}
	case GC_MBOX_MATCH_COMPLETE_DLG :
		{
			pDialog = (KD3DDialog*)m_pkMatchCompleteDlg;
			break;
		}		
	case GC_MBOX_AUTOMATCH_LOADING :
		{
			pDialog = (KD3DDialog*)m_pkAutoMatchLoading;
			break;
		}
	case GC_MBOX_RITASCHRISTMAS_DLG:
		{
			pDialog = (KD3DDialog*)m_pkRitasChristmasDlg;
			break;
		}
    case GC_MBOX_SUPER_GACHA:
        {
            pDialog = (KD3DDialog*)m_pkSuperGachaDlg;
            break;
        }
    case GC_MBOX_ATTENDANCE_STAMP_DLG:
        {
            pDialog = (KD3DDialog*)m_pkAttendanceStampDlg;
            break;
        }
    case GC_MBOX_BUFF_DONATION_DLG:
        {
            pDialog = (KD3DDialog*)m_pkBuffDonationDlg;
            break;
        }
    case GC_MBOX_ITEM_BIND_DLG:
        {
            pDialog = (KD3DDialog*)m_pkItemBindDlg;
            break;
        }
    case GC_MBOX_BUY_CASH_ITEM_CHOOSE_CASH_TYPE_DLG:
        {
            pDialog = (KD3DDialog*)m_pkBuyCashItemChooseCashTypeDlg;
            break;
        }
    case GC_MBOX_BUY_CASH_ITEM_ACCUMULATED_GC_DLG:
        {
            pDialog = (KD3DDialog*)m_pkBuyCashItemAccumulatedGCDlg;
            break;
        }
    case GC_MBOX_BUY_CASH_ITEM_DLG_S6:
        {
            pDialog = (KD3DDialog*)m_pkBuyCashItemDlgS6;
            break;
        }
    case GC_MBOX_BUY_CASH_ITEM_BUY_COMPLETE:
        {
            pDialog = (KD3DDialog*)m_pkBuyCashItemBuyCompleteDlg;
            break;
        }   
	case GC_MBOX_CHALLENGE_ERRAND:
        {
            pDialog = (KD3DDialog*)m_pkChallengErranDlg;
            break;
        }
    case GC_MBOX_ADDITEM_IN_CLIENT:
        {
            pDialog = (KD3DDialog*)m_pkAddItemInClient;
            break;
        }
	case GC_MBOX_FATIGUE_SYSTEM_MESSAGE:
		{
			pDialog = (KD3DDialog*)m_pkFatigueMessageBox;
			break;
		}
	case GC_MBOX_FATIGUE_SYSTEM_MESSAGE2:
		{
			pDialog = (KD3DDialog*)m_pkFatigueMessageBox2;
			break;
		}
	case GC_MBOX_CHANGE_WEB_CASH :
		{
			pDialog = (KD3DDialog*)m_pkChangeCash;
			break;
		}
    case GC_MBOX_ESC_MENU_DLG:
    {
        pDialog = (KD3DDialog*)m_pkEscMenuDlg;
        break;
    }
    case GC_MBOX_SHORT_MENU_DLG:
    {
        pDialog = (KD3DDialog*)m_pkShortMenuCutBox;
        break;
    }
    case GC_MBOX_JUMPING_CHAR_DLG:
    {
        pDialog = (KD3DDialog*)m_pkJumpingCharDlg;
        break;
    }
    case GC_MBOX_CREATE_ROOM_S6:
        {
            pDialog = (KD3DDialog*)m_pkCreateRoomBoxS6;
            break;
        }
    case GC_MBOX_CHANGE_ROOM_TITLE_S6:
        {
            pDialog = (KD3DDialog*)m_pkChangeRoomTitleBoxS6;
            break;
        }
    case GC_MBOX_SQUARE_ENTER_S6:
        {
            pDialog = (KD3DDialog*)m_pkSquareEnterWndS6;
            break;
        }
    case GC_MBOX_REPLAY_S6:
        {
            pDialog = (KD3DDialog*)m_pkReplayBoxS6;
            break;
        }
    case GC_MBOX_COORDI_COMPOSE_DLG:
        {
            pDialog = (KD3DDialog*)m_pkCoordiComposeDlg;
            break;
        }
	case GC_MBOX_VIRTUALDEPOT:
	    {
	        pDialog = (KD3DDialog*)m_pkVirtualDepotDlg;
	        break;
	    }
    case GC_MBOX_SOCIAL_COMMERCE:
        {
            pDialog = (KD3DDialog*)m_pkSocialCommerceDlg;
            break;
        }
    case GC_MBOX_COORDI_UPGRADE_DLG:
        {
            pDialog = (KD3DDialog*)m_pkCoordiUpgradeDlg;
            break;
	    }  	
	case GC_MBOX_HERO_DUNGEON_SEASON5:
        {
	        pDialog = (KD3DDialog*)m_pkHeroDungeonSeason5;
	        break;
        }
    case GC_MBOX_EVENT_DUNGEON_SEASON5:
        {
            pDialog = (KD3DDialog*)m_pkEventDungeonSeason5;
            break;
        }
	case GC_MBOX_PRESENT_CASH_ITEM_WND:
		{
			pDialog = (KD3DDialog*)m_pkPresentCashItemWnd;
			break;
		}
	case GC_MBOX_PRESENT_FRIEND_LIST:
		{
			pDialog = (KD3DDialog*)m_pkPresentFriendList;
			break;
		}		
    case GC_MBOX_INFINITY_GUIDE_DLG:
        {
            pDialog = (KD3DDialog*)m_pkInfinityGuideDlg;
            break;
        }
    case GC_MBOX_ENCHANT_S6_DLG:
        {
            pDialog = (KD3DDialog*)m_pkEnchantS6Dlg;
            break;
        }
    case GC_MBOX_ENCHANT_SUB_S6_DLG:
        {
            pDialog = (KD3DDialog*)m_pkEnchantSubS6Dlg;
            break;
        }
    case GC_MBOX_BILLBOARD_DLG:
    {
        pDialog = (KD3DDialog*)m_pkBillBoardBox;
        break;
    }
    case GC_MBOX_DUNGEON_RANKING_DLG:
        {
            pDialog = (KD3DDialog*)m_pkDungeonRankingDlg;
            break;
        }

    case GC_MBOX_KGCHeroDungeonInfoS6:
    {
        pDialog = (KD3DDialog*)m_pkHeroInfoS6;
        break;
    }
    case GC_MBOX_HERODUNGEON_SHOP_DLG_S6:
    {
        pDialog = (KD3DDialog*)m_pkHeroDungeonShopS6;
        break;
    }
    case GC_MBOX_NEW_BOX_HERO_REWARD:
    {
        pDialog = (KD3DDialog*)m_pkHeroRewardS6;
        break;
    }
    case GC_MBOX_NICKNAME_COLOR_DLG:
    {
        pDialog = (KD3DDialog*)m_pkNicknameColorBox;
        break;
    }
    //GachaPong
    case GC_MBOX_GACHA_PONG_DLG:
    {
        pDialog = (KD3DDialog*)m_pkGachaPongDlg;
        break;
    }
    case GC_MBOX_ROOM_OPTION_CHAR_DLG:
    {
        pDialog = (KD3DDialog*)m_pkRoomOptionCharDlg;
        break;
    }
    case GC_MBOX_ROOM_OPTION_RANK_DLG:
    {
        pDialog = (KD3DDialog*)m_pkRoomOptionRankDlg;
        break;
    }
    case GC_MBOX_ROOM_LIST_FILTER_DLG:
    {
        pDialog = (KD3DDialog*)m_pkRoomListFilterDlg;
        break;
    }
    case GC_MBOX_COORDI_VIEW_DLG:
    {
        pDialog = (KD3DDialog*)m_pkCoordiViewDlg;
        break;
    }
    case GC_MBOX_CUSTOMIZATION_DLG:
    {
        pDialog = (KD3DDialog*)m_pkCustomizationDlg;
        break;
    }
    case GC_MBOX_QUANTITYUSE_DLG:
    {
        pDialog = (KD3DDialog*)m_pkQuantityUseDlg;
        break;
    }
    case GC_MBAX_PET_MAGIC_STONE_DLG:
    {
        pDialog = (KD3DDialog*)m_pkPetMagicStoneDlg;
        break;
    }
    case GC_MBOX_HERO_TICKET_DLG:
    {
        pDialog = (KD3DDialog*)m_pkHeroTicketDlg;
        break;
    }
    case GC_MBOX_ALL_COLLECTION_DLG:
    {
        pDialog = (KD3DDialog*)m_pkAllCollectionDlg;
        break;
    }
    default: //for macro : last of msgbox proc func 
        {
            return NULL; // invalid message box type
        }
    }

    return pDialog;
}

void KGCUIScene::FrameMove_MouseCursor()
{
//     lua_State* L = KGCLuabinder::getInstance().GetLuaState();
//     float x = lua_tinker::call<float>( L, "GetMousePosX" );
//     float y = lua_tinker::call<float>( L, "GetMousePosY" );
    if( m_pkMouseCursor == NULL )
        return;

    m_pkMouseCursor->GetMatrix()->Move( g_MyD3D->MyCtrl->m_x, g_MyD3D->MyCtrl->m_y, g_AnimManager.GetZPos() );
}

bool KGCUIScene::IsRenderOn( EGCMsgBoxType mbt )
{
	KD3DDialog* buf = GetDialogPtr(mbt);
	if(buf== NULL) return false;
	return buf->IsRenderOn();
}

KD3DWnd::ED3DWndState KGCUIScene::GetStateUI( EGCMsgBoxType mbt )
{
	KD3DDialog* buf = GetDialogPtr(mbt);
	if(buf== NULL) return KD3DWnd::D3DWS_DISABLED;
	return buf->GetWndState();
}

void KGCUIScene::RECPlay()
{
    m_pkReplayRecordREC->ToggleRender( true );
    m_pkReplayRecordREC->RECPlay();
	m_pkReplayRecordREC->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
}

void KGCUIScene::RECStop()
{
    m_pkReplayRecordREC->ToggleRender( false );
    m_pkReplayRecordREC->RECStop();
}

void KGCUIScene::SetCurMsgBox( KD3DWnd* pDlg )
{
    m_pkCurMsgBox = pDlg;
}

void KGCUIScene::OnLevelupInfo( void )
{
    m_pkLevelupInfoBox->InitInfo();
    m_pkLevelupInfoBox->OnLevelup();
}

bool KGCUIScene::IsRenderLevelupInfo( void )
{
    return m_pkLevelupInfoBox->IsRenderOn();
}

bool KGCUIScene::IsValidNickname()
{
    return m_pkNicknameCampaignBox->GetValidNickname();
}

bool KGCUIScene::IsInMousePos( D3DXVECTOR2& dxvPos_ )
{
    for( std::vector< KD3DWnd* >::iterator it = m_vecWnd.begin() ; it != m_vecWnd.end() ; ++it )
    {
        if( (*it)->IsRenderOn() && (*it)->CheckPosInWindowBoundWithChild( dxvPos_ ) )
            return true;
    }

    return false;
}

void KGCUIScene::ShowItemEffectGameInven( bool _bShow )
{
//     if ( m_pkGameInventory )
//     {
//         m_pkGameInventory->ShowItemEffect( _bShow );
//     }
}

void KGCUIScene::CloseGameInventory( void )
{
    if( m_pkGameInventory ) { 
        m_pkGameInventory->OnClickBtnClose();
    }
}

void KGCUIScene::UpdateHotItem()
{
#if defined( USE_NEW_ITEM_CHECK ) 
    if ( m_pkMatchScene ) 
        m_pkMatchScene->ShowHotItemType( g_pItemMgr->GetHotItemType() );


    if ( m_pkCashShopScene ) 
        m_pkCashShopScene->ShowHotItemType( g_pItemMgr->GetHotItemType() );

    if ( m_pkGPShopScene ) 
        m_pkGPShopScene->ShowHotItemType( g_pItemMgr->GetHotItemType() );

    if ( m_pkRoomSceneS6 ) 
        m_pkRoomSceneS6->ShowHotItemType( g_pItemMgr->GetHotItemType() );

    if ( m_pkCoordiShopScene ) 
        m_pkCoordiShopScene->ShowHotItemType( g_pItemMgr->GetHotItemType() );
#endif
}

void KGCUIScene::CloseInSquareUI( void )
{
    // 게시판
    m_pkRanking->SpeakToActionListener( KActionEvent( m_pkRanking, KD3DWnd::EWNDMESSAGE_CLOSE ) );

    // 제작/해제
    m_pkForgeDlg->OnClickCloseBtn();

    // 길드
    m_pkGuildDlg->OnClose();

    // 강화 
    m_pkEnchantSubS6Dlg->OnClose();
    m_pkEnchantS6Dlg->OnClose();


    // 기부 
    m_pkContributionDlg->OnExit();
    m_pkContributionBox->OnExit();
}

void KGCUIScene::StartPlayTimeNotice( int iIndex_, int iPlayTime_ /*= 0*/ )
{
    if( iPlayTime_ <= 0  ) { 
        return;
    }
         
    if( m_pkPlaytimeNoticeUI ) { 
        m_pkPlaytimeNoticeUI->StartNotice( iIndex_, iPlayTime_ );
    }
}

void KGCUIScene::SetLevelItemDlgInfo( int iLevel_, std::set< char >& setCharIndex_  )
{
    if( m_pkLevelItemDlg ) { 
        m_pkLevelItemDlg->SetLevelItemInfo( iLevel_, setCharIndex_ );
    }
}

void KGCUIScene::RoomSceneUpdateData( bool bUpdateExtraModel )
{
    m_pkRoomSceneS6->UpdateGameData( bUpdateExtraModel );
}

void KGCUIScene::RoomButtonSetStartDelay( int iDelay )
{
    m_pkRoomSceneS6->m_pkRoomButtonPanel->SetStartDelay(iDelay);
}

int KGCUIScene::RoomButtonGetStartDelay()
{
    return m_pkRoomSceneS6->m_pkRoomButtonPanel->GetStartDelay();
}

bool KGCUIScene::RoomButtonIsAutoStart()
{
    return m_pkRoomSceneS6->m_pkRoomButtonPanel->IsAutoStart();
}

void KGCUIScene::RoomButtonSetAutoStart( bool bAuto )
{
    m_pkRoomSceneS6->m_pkRoomButtonPanel->SetAutoStart( bAuto );
}

void KGCUIScene::TutorialRoomStart()
{
    SiKGCWorldMapManager()->SetCurrentWorldMap(WORLDMAP_BERMESIA_SERDIN);
    SiKGCWorldMapManager()->SetCurrentSelectDungeon(1);

    std::wstring wTemp = SiKGCRoomManager()->m_vecDungeonRoomNameList[rand() % KGCRoomManager::m_vecDungeonRoomNameList.size()].c_str();
    HRESULT hr = SiKGCRoomManager()->CreateRoomParty(wTemp, L"", GC_GM_QUEST0, 0, GC_GMC_DUNGEON, 2, GC_GS_FOREST_OF_ELF, true);

    Result_StartGame = 0;
    std::vector< std::pair<char, char> > vecPromotionCount;
    g_MyD3D->GetStatPromotionCount(vecPromotionCount);
    KP2P::GetInstance()->Send_StartGameReq(vecPromotionCount);
    g_MyD3D->WaitForServerAck(Result_StartGame, 0, 5000, 2);
    g_pkUIScene->ToggleMessageBoxBack(false, true);

    std::wstringstream stm;
    std::wstringstream strm;
    std::wstring strColorKey = L"#cff0000";
    std::wstring strColorKeyEnd = L"#cX";
    stm << strColorKey << g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_START_GAME_ERROR) << strColorKeyEnd;

    // 051102. jseop. 서버에서 온 응답에 따른 처리.
    switch (Result_StartGame)
    {
    case 1:
        break;
    case 2:
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString(STR_ID_START_GAME_ERROR20),
            g_pkStrLoader->GetString(STR_ID_START_GAME_ERROR21));
        break;
    case 11:
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, stm.str(),
            g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR5), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
        break;
    case 13:
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, stm.str(),
            g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR1), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
        break;
    case 14:
        strColorKey = L"#cffff00";
        strm << strColorKey << L"\n"
            << g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR3)
            << g_kGlobalValue.m_kHeroDungeonInfo.GetNextEntranceTime(SiKGCRoomManager()->GetGameMode())
            << strColorKeyEnd;
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO, stm.str(),
            g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR7) + strm.str(), KGCUIScene::GC_MBOX_USE_EXIT, 0, 0, false, true);
        break;
    case 15:
        strColorKey = L"#cffff00";
        strm << strColorKey << L"\n"
            << g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR3)
            << g_kGlobalValue.m_kHeroDungeonInfo.GetNextEntranceTime(SiKGCRoomManager()->GetGameMode())
            << strColorKeyEnd;
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, stm.str(),
            g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR6) + strm.str(), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
        break;
    case 16:
        strColorKey = L"#cffff00";
        strm << strColorKey << L"\n"
            << g_kGlobalValue.m_kHeroDungeonInfo.GetInitTime()
            << strColorKeyEnd;
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, stm.str(),
            g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR4) + strm.str(), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
        break;
    case 17:
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ERR_GAME_START_TIME), L"");
        break;
    default:
        std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_START_GAME_ACK, Result_StartGame);
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_START_GAME_ERROR30), strTemp);
        break;
    }
}

void KGCUIScene::ShowNewSkillMsg()
{
    if ( m_pkNewSkillMsg == NULL )
        return;

    if( !m_pkNewSkillMsg->IsRenderOn() )
    {
        m_pkNewSkillMsg->SetFixedWindowLocalPos( D3DXVECTOR2( 797, 1 ) );
        m_pkNewSkillMsg->SetMsgType( KGCInventoryFullMsg::NEW_SKILL_MSG );
        m_pkNewSkillMsg->UpdateMsg();
        m_pkNewSkillMsg->ToggleRender( true );
    }
}

void KGCUIScene::HideSpeechBubbleMsg()
{
    if( m_pkInventoryFullMsg )
        m_pkInventoryFullMsg->ToggleRender(false);

    if( m_pkNewSkillMsg )
        m_pkNewSkillMsg->ToggleRender(false);
}

void KGCUIScene::UpdateAllHeroShop()
{
    m_pkHeroDungeonShopS6->m_pkHeroShopPanel->m_mapItemInfo.clear();
    m_pkHeroDungeonShopS6->m_pkHeroShopPanel->UpdateAllHeroShop();
}

bool KGCUIScene::IsReplayDlgRenderOn( void )
{
    if( m_pkReplayRecordDlg ) { 
        return ( KD3DWnd::D3DWS_ENABLED == GetStateUI( GC_MBOX_REPLAY_RECORD ) );
    }

    return false;
}
void KGCUIScene::EnableMessageBox( bool bEnable )
{
    m_bEnableMessageBox = bEnable;
}

bool KGCUIScene::EnableShowMessageBox()
{
    if ( !g_pkUIScene->IsValidState_ShowMessageBox() )
        return false;

    if ( !g_pkUIScene->IsEmptyWaitMessageBox() )
        return false;

    if ( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
        return false;

    if ( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM )
        return false;

    return true;
}