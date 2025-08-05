#ifndef _GCUISCENE_H_
#define _GCUISCENE_H_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "../OSInfo.h"
#include "animation/KGCAnimManager.h"

class KGCBuyCashBox;
class KGCPetCostumeInven;
class KD3DStatic;
class KGCMessageBox;
class KGCPasswordBox;
class KGCItemReceiveBox;
class KGCGemBuyBox;
class KGCGameOverlayUI;
class KGCOnGameDlg;
class KGCSkillCheckBtn;
class KGCChannelScene;
class KGCCashShopScene;
class KGCGPShopScene;
class KGCCoordiShopScene;
class KGCGuildScene;
class KGCScene;
class KGCMyInfoScene;
class KGCRoomSceneS6;
class KGCOption;
class KGCNickNameBox;
class KGCItemInfoBox;
class KGCLoadingScene;
class KGCCandidateWnd;
class KGCLoginScene;
class KGCRegisterScene;
class KGCLoadingBox;
class KGCColorDlg;
class KGCBuyCashItemWnd;
class KGCChangeCash;
class KGCToolTip;
class KGCPetPreview;
class KGCEventBox;
class KGCRepetitionEventBox;
class KGCPetEvolutionBox;
class KGCSelectPetNameBox;
class KGCAttendanceCalendar;
class KGCPerfectAttendanceBox;
class KGCColorItemUse;
class KGCReportBox;
class KGCCraftingBox;
class KGCNewTutorialOption;
class KGCLuckyDayReceiveBox;
class KGCServerScene;
#ifdef NEW_CHAR_SELECT_SCENE 
class KGCCharSelectScene_New;
#else
class KGCCharSelectScene;
#endif
class KGCCharGainScene;
class KGCMatchScene;
class KGCWorldMapScene;
class KGCChannelListBox;
class KGCMissionUI;
class KGCFadeoutWnd;
class KGCRoomList;
class KGCWorldMapContainer;
class KGCBonusChargeBox;
class KGCGuideNewbie;
class KGCSquareOverlayUI;
class KGCSquareScene;
class KGCSquareLoadingScene;
class KGCInviteRequestBox;
class KGCInviteMsgBox;
class KGCNeonsignSelectBox;
class KGCNeonsignPreview;
class KGCEmoticonWindow;
class KGCConnectionMsg;
class KGCTreasureItemBox;
class KGCRankingDlg;
class KGCWelcomeBack;
class KGCAskUseVirtualCash;
class KGCTitleWindow;
class KGCPetWindow;
class KGCCTConditionWindow;
class KGCMinigameDlg;
class GCNateOn;
class GCNateOnAddGroupDlg;
class GCEmoticonSelectDlg;
class GCNateOnAddFriend;
class GCBlockToAddFriend;
class KGCSkillTreeDlg;
class GCNateOnMsg;
class GCNateOnInviteBuddy;
class KGCNewClearSealDlg;
class KGCCoupleViewer;
class KGCCoupleAccept;
class KGCCouplePropose;
class KGCCoupleWait;
class KGCCoupleMessage;
class KGCCoupleGoodbye;
class KGCContributionDlg;
class KGCContributionBox;
class KGCVipGachaBox;
class KGCHappyNewYearNPCDlg;
class KGCLogoutBox;
class KGCRecommendList;
class KGCGPCapsuleDlg;
class KGCGPCapsuleObtainWnd;
class KGCDicePlay;
class KGCCouponNPCDlg;
class KGCAgreement;
class KGCSubscriptionDlg;
class KGCSubscriptionBox;
class KGCCooperationEventDlg;
class KGCGuildMarkRegistDlg;
class KGCGuildDlg;
class KGCGuildEnterDlg;
class KGCGuildLobbyDlg;
class KGCGuildListDlg;
class KGCGuildCreateDlg;
class KGCGuildJoinReqMbox;
class KGCGuildPublicMbox;
class KGCGuildSquareNotice;
class KGCCommonEditBox;
class KGCFileFindDialog;
class KGCItemInformationBoxPair;
class KGCMonsterCardBook;
class KGCItemSocketManagementDlg;
class KGCBingo;
class KGCQuiz;
class KGCPuzzleBox;
class KGCInventoryFullMsg;
class KGCGiftItemBox;
class KGCProgressBtn;
class KGCHurryUPDlg;
class KGCStatusInfoPanel;
class KGCCharacterSelectBox;
class KGCCoordiChangeDlg;
class KGCSellCountItemDlg;
class KGCGambleBox;
class KGCCharGetInfoPromotion;
class KGCCharGetInfoPanel;
class KGCPromotionChangePanel;
class KGCCharacterGetInfoDlg;
class KGCHeroDungeonDlg;
class KGCSceneMenuBtn;
class KGCMissionComplete;
class KGCReplayRecordDlg;
class KGCReplayRecordCommonDlg;
class KGCReplayRecordAdvanceDlg;
class KGCReplayExplain;
class KGCReplayRecordREC;
class KGCReplayBoxNew;
struct KInventoryItem;
class KGCGawibawiboGame;
class KGCSocksTreeDlg;
class KGCMissionUI_S4;
class KGCMissionEquipPreview_S4;
class KGCWeaponChangeBox;
class KGCAttributeSelectBox;
class KGCLevelupInfoBox;
class KGCNicknameCampaignBox;
class KGCCoordiCatalogDlg;
class KGCPlantTree;
class KGCPlantTreeRewardItemListView;
class KGCChoiceDrop;
class KGCGuildStore;
class KGCGuildLevelList;
class KGCGuildBenefitListBar;
class KGCUserMigrationDlg;
class KGCPackagePreview;
class KGCVipReward;
class KGCMagicBox;
class KGCSphinxDlg;
class KGCDotaShop;
class KGCRickoonTornado;
class KGCPartyWindow;
class KGCPartyDungeon;
class KGCPartyMatch;
class KGCPartyInviteMsgBox;
class KGCEventMinigameList;
class KGCAgitOverlayUI;
class KGCAgitShop;
class KGCGameInventory;
class KGCMantisReportDlg;
class KGCMsgShutDown;
class KGCNewForgeDlg;
class KGCSurveyDlg;
class KGCGwcEvent;
class KGCGwcRewardCatalogDlg;
class KGCChristmasGiftDlg;
class KGCNotifyMessage;
class KGCPostLetter;
class KGCEclipseTime;
class KGCEventEclipsePlotDlg;
class KGCDepotDlg;
class KGCEclipseOldBooks;
class KGCWelcomeBackNew;
class KGCItemLevelDownDlg;
class KGCCoordiClearSealDlg;
class KGCHotTimeSearchDlg;
class KGCMonsterCardCompose;
class KGCAdventureDialog;
class KGCNewNecklaceEnchantDlg;
class KGCTransferMissionScrollGuideDlg;
class KGCSongkranDlg;
class KGCEventAlarmDlg;
class KGCSongkranRewardDlg;
class KGCRoomListDlgS6;
class KGCRoomDungeonSelectDlgS6;
class KGCRoomCreateDlgS6;
class KGCChatBoxDlgS6;
class KGCSceneMenuS6;
class KGCPlaytimeNotice;
class KGCOlympicEventDlg;
class KGCOlympicGawibawiboGame;
class KGCOlympicDicePlay;
class KGCOlympicSongkranDlg;
class KGC9thEventDlg;
class KGCLevelItemDlg;
class KGCItemCombinationDlg;
class KGCRoomQuickJoinDlgS6;
class KGCRoomUserInfoPopupS6;
class KGCMatchMapSelectDlgS6;
class KGCMatchInviteUserListS6;
class KGCMatchCompleteDlgS6;
class KGCAutoMatchLoadingS6;
class KGCLottery;
class KGCItemCreationSelectBox;
class KGCRitasChristmas;
class KGCSuperGachaDlg;
class KGCAttendanceStampDlg;
class KGCBuffDonationDlg;
class KGCItemBindDlg;
class KGCAddItemInClient;
class KGCBuyCashItemChooseCashTypeDlg;
class KGCBuyCashItemAccumulatedGCDlg;
class KGCBuyCashItemDlgS6;
class KGCBuyCashItemBuyCompleteDlg;
class KGCChallengeErrandDlg;
class KGCPresentCashItemWnd;
class KGCPresentFriendList;
class KGCGuide;
class KGCGiftBox;
class KGCFatigueInfoBox;
class KGCFatigueMessageBox;
class KGCFatigueMessageBox2;
class KGCAgreement1301;
class KGCEscMenuDlg;
class KGCJumpingCharDlg;
class KGCCreateRoomBoxS6;
class KGCChangeRoomTitleBoxS6;
class KGCSquareEnterWndS6;
class KGCReplayBoxS6;
class KGCCoordiComposeDlg;
class KGCCoordiUpgradeDlg;
class KGCVirtualDepotDlg;
class KGCSocialCommerceDlg;
class KGCHeroDungeonSeason5;
class KGCEventDungeonSeason5;
class KGCInfinityGuideDlg;
class KGCEnchantS6Dlg;
class KGCEnchantSubS6Dlg;
class KGCDungeonRankingDlg;
class KGCShortMenuDlg;
class KGCBillboardChatDlg;
class KGCBillBoardChatBoard;
class KGCHeroDungeonInfoS6;
class KGCHeroDungeonShopS6Dlg;
class KGCHeroDungeonUpgradePanel;
class KGCHeroDungeonRewardDlg;
class KGCHeroDungeonUpgradeSlot;
class KGCHeroDungeonUpgradeSidePanel;
class KGCQuantityPurchaseBox;
class KGCNicknameColorBox;
class KGCGachaPongDlg;
class KGCRoomOptionCharDlg;
class KGCRoomOptionRankDlg;
class KGCRoomListFilterDlg;
class KGCCoordiViewDlg;
class KGCCustomizationDlg;
class KGCQuantityUseBox;
class KGCPetMagicStoneDlg;
class KGCHeroTicketUseWnd;
class KGCAllCollection;

class KGCUIScene : public KD3DUIScene,              // extends
    public KActionListener           // implements
{
public:
    DECLARE_CLASSNAME( KGCUIScene );
    /// Constructor
    explicit KGCUIScene( const std::string& strSceneName = "" );
    /// Destructor
    virtual ~KGCUIScene( void );
    /// Copy constructor
    KGCUIScene( const KGCUIScene& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCUIScene& operator=( const KGCUIScene& )
    { ASSERT( false && "Do not use assignment operator!" ); return * this; }

public:
    enum
    {
        NUM_MAIN_TAB = 6,
    };

    enum
    {
        NUM_MAIN_SCENE = 6,
    };

    enum EGCWndEventCode {
        D3DWE_USER_EVENT_BEGIN = KD3DWnd::K3DWE_USER+1,
        D3DWE_CLICK_INVEN_ITEM,
        D3DWE_RCLICK_INVEN_ITEM,
        D3DWE_RECEIVE_LOGIN_SERVER_PAPCKET,
        D3DWE_RECEIVE_GAME_SERVER_PAPCKET,
        D3DWE_RECEIVE_MESSENGER_SERVER_PAPCKET,
        D3DWE_CHARACTER_SELECT,
        D3DWE_SELL_ITEM,
        D3DWE_SELL_EXIT,
        D3DWE_SELL_START,   
        D3DWE_SELL_END,
        D3DWE_SELL_ADD,         // lParam  : ItemUID
        D3DWE_SELL_REMOVE,      // lParam  : ItemUID
        D3DWE_CHANGE_CHAR_INFO,
		D3DWE_GAWIBAWIBO_CHALLENGE_BTN_CLICK,
		D3DWE_MYINFOSCENE_SUB_BTN_MENU_STATE_UPDATE,
        D3DWE_SELL_DLG_OPEN_START,
        D3DWE_HIDE_GUID_BOOK,
        D3DWE_SHOW_GUID_BOOK,

        D3DWE_ALERT_EVENT_UPDATE,        
        //광장에서 메뉴 버튼들 랜더 켜기 끄기
        D3DWE_SQUARE_SCENE_MENUBTN_RENDER_OFF,
        D3DWE_SQUARE_SCENE_MENUBTN_RENDER_ON,
        D3DWE_END_GAME,

        D3DWE_FORGE_SEL_CHARACTER,
        D3DWE_FORGE_CHANGE_PAGE,
        D3DWE_FORGE_CLICK_CREATION_ITEM_BAR,
        D3DWE_FORGE_CLICK_BREAKEUP_ITEM,
        D3DWE_FORGE_COMPLETE_RESULT_ITEM,
        D3DWE_FORGE_BOUND_ITEM,
        
        D3DWE_COORDI_COMPOSE,   //  합성UI_OPEN
        D3DWE_OPTION_KEY_TEXT_UPDATE,
        D3DWE_OPTION_JOYSTICKKEY_TEXT_UPDATE,

        D3DWE_TIME_TICK_ONE_MINUTE,
		D3DWE_DRAG_ITEM_START,
		D3DWE_DRAG_ITEM_END,
		D3DWE_DRAG_ITEM_CANCEL,

		D3DWE_TG_TIMER_END,
        D3DWE_BUFF_FOODCHANGE,

		D3DWE_DUNGEON_ROOM_SELECT_LEVEL,
		D3DWE_UPDATE_ROOM_LIST,
		D3DWE_DUNGEON_SELECT,
		D3DWE_DUNGEON_CREATE,
		D3DWE_DUNGEON_CLICK_TITLE_COLOR_BTN,
		D3DWE_DUNGEON_ROOM_JOIN,
		D3DWE_CHAT_OPTION_BTN_CLICK,
		D3DWE_CHAT_KIND_BTN_CLICK,
		D3DWE_CHAT_CHANGE_SIZE_BTN_CLICK,
		D3DWE_REPLAY_ON,
		D3DWE_BUY_CASH_ITEM,
		D3DWE_OFF_GCMENU,
		D3DWE_UPDATE_MATCH,
		D3DWE_PRE_ENTER_SCENE,
		D3DWE_PARTY_CREATE,
		D3DWE_AGREEMENT_OK,
        D3DWE_BUT_ADD_SLOT_ITEM,
        D3DWE_SELL_INVEN_ITEM,

        D3DWE_ENCHANT_CLEAR_INVEN,
        D3DWE_SEL_ENCHANT_EQUIP_SLOT,
        D3DWE_SEL_ENCHANT_ENCHANT_SLOT,
        D3DWE_SEL_ENCHANT_ASSIST_SLOT,
        D3DWE_ENCHANT_OK,
        D3DWE_ENCHANT_EQUIP_OK,
        D3DWE_PET_MAGIC_STONE_EQUIP_CLICK,
    };

    enum EGCMsgBoxType
    {
        GC_MBOX_OK = 0,
        GC_MBOX_YES_NO = 1,
        GC_MBOX_PASSWORD = 2,
        GC_MBOX_CREATE_ROOM = 3,
        GC_MBOX_REPLAY = 4,
        GC_MBOX_ITEM_RECEIVE = 5,
        GC_MBOX_PATTERN_BUY_nouse = 6,
        GC_MBOX_GEM_BUY = 8,
        GC_MBOX_OPTION = 10,
        GC_MBOX_NICK_NAME = 11,
        GC_MBOX_LOADING = 12,
        GC_MISSION_BOX = 13,
        GC_MBOX_MISSION_COMPLETE = 14,
        GC_MBOX_COLOR = 15,
        GC_MBOX_BUY_CASH_ITEM = 16,
        GC_MSN_DLG = 17,
        GC_MBOX_EXIT_GAME = 20,
        GC_MBOX_EXIT_ROOM = 21,
        GC_PET_PREVIEW = 23,
        GC_PET_FEEDING = 24,
        GC_EVENTBOX = 25,
        GC_MBOX_EXTEND_INVENTORY = 26,
        GC_MBOX_REPETITION_EVENT_BOX = 27,
        GC_MBOX_PET_EVOLUTION_BOX_nouse = 28,
        GC_MBOX_PET_DEVOLUTION_BOX_nouse = 29,
        GC_MBOX_PET_BOX = 30,
        GC_MBOX_PET_NAME_SELECT = 31,
        GC_MBOX_DEVOLUTION_PET_NAME_SELECT_nouse = 32,
        GC_MBOX_ATTENDANCECALENDAR = 33,
        GC_MBOX_PERFECT_ATTENDANCE = 34,
        GC_MBOX_TUTORIAL_OPTION = 35,
        GC_MBOX_CHANGE_ROOM_TITLE = 36,
        GC_MBOX_LUCKY_DAY_RECEIVE_BOX = 37,
        GC_MBOX_FAIRPLAY_REPORT = 38,
        GC_MBOX_GRADE_SHOW = 39,
        GC_MBOX_CRAFTING_BOX_nouse = 40,
        GC_MBOX_CHANNEL_LIST = 42,
        GC_MBOX_MISSION_UI = 43,
        GC_MBOX_CREATE_DUNGEON_ROOM = 44,
        GC_MBOX_DUNGEON_ROOM_LIST = 45,
        GC_MBOX_BONUS_CHARGE = 46,
        GC_MBOX_SQUARE = 47,
        GC_MBOX_EXIT_SQUARE = 48,
        GC_MBOX_SEASON2_INVITE_REQ = 50,
        GC_MBOX_SEASON2_INVITE = 51,
        GC_MBOX_NEONSIGN_SELECT = 52,
        GC_MBOX_NEONSIGN_PREVIEW = 53,
        GC_MBOX_EMOTICON_COLLECTION = 54,
        GC_MBOX_CONNECTION_MSG = 55,
        GC_MBOX_RANKING = 56,
        GC_MBOX_BADUSER_INFO = 57,
        GC_MBOX_TREASURE_ITEM_BOX = 58,
        GC_MBOX_WELCOMEBACK_DLG = 60,
        GC_MBOX_BUY_VIRTUAL_CASH = 61,
        GC_MBOX_TITLE_COLLECTION = 63,
        GC_MBOX_PET_COLLECTION = 64,
        GC_MBOX_CONDITION_WINDOW = 65,
        GC_MBOX_MINIGAME_DLG = 66,
        GC_MBOX_NATEON_INPUT_BOX = 67,
        GC_MBOX_EMOTICON_SELECT_DLG = 68,
        GC_MBOX_NATEON_ADD_BUDDY = 69,
        GC_MBOX_NATEON_BLOCK_TO_ADD_BUDDY = 70,
        GC_MBOX_SKILLTREE = 71,
        GC_MBOX_NATEON_TALK_INVITE = 72,
        GC_MBOX_NATEON_MESSAGE = 73,
        GC_MBOX_COUPLE_VIEWER = 75,
        GC_MBOX_COUPLE_PROPOSE = 76,
        GC_MBOX_COUPLE_ACCEPT = 77,
        GC_MBOX_COUPLE_WAIT = 78,
        GC_MBOX_COUPLE_MESSAGE_OK = 79,
        GC_MBOX_COUPLE_MESSAGE_OK_CANCLE = 80,
        GC_MBOX_COUPLE_GOODBYE = 81,
        GC_MBOX_PET_COSTUME_INVEN = 82,
        GC_MBOX_CONTRIBUTION_DLG = 83,
        GC_MBOX_CONTRIBUTION_BOX = 84,
        GC_MBOX_VIP_GACHA_BOX = 85,
        GC_MBOX_HAPPYNEWYEAR_NPC_DLG = 86,
        GC_MBOX_LOGOUT_BOX = 87,
        GC_MBOX_VALENTINEDAY_DLG = 88,
        GC_MBOX_AGREEMENT = 89,
        GC_MBOX_RECOMMEND = 90,
        GC_MBOX_GP_CAPSULE_DLG = 91,
        GC_MBOX_GP_CAPSULE_OBTAIN_DLG = 92,
        GC_MBOX_GP_CAPSULE_MESSAGE_OK = 93,
        GC_MBOX_COUPON_NPC_DLG = 94,
        GC_MBOX_COUPON_ITEM_OBTAIN_OK = 95,
        GC_MBOX_BUY_CASH_BOX = 99,
        GC_MBOX_COOPERATION_EVENT_BOX = 100,
        GC_MBOX_OK2 = 101,
        GC_MBOX_SUBSCRIPTION_DLG = 102,
        GC_MBOX_SUBSCRIPTION_BOX = 103,
        GC_MBOX_HALLOWEEN_DLG = 104,
        GC_MBOX_GWC_DONATION_DLG = 105,
        GC_MBOX_GWC_STATE_DLG = 106,
        GC_MBOX_SHADOW_EVENT = 107,
        GC_MBOX_GUILDMARK_REGIST_DLG = 108,
        GC_MBOX_GUILD_DLG = 109,
        GC_MBOX_GUILD_ENTER_DLG = 110,
        GC_MBOX_GUILD_LOBBY_DLG = 111,
        GC_MBOX_GUILD_LIST_DLG = 112,
        GC_MBOX_GUILD_CREATE_DLG = 113,
        GC_MBOX_GUILD_JOIN_REQ_MBOX = 114,
        GC_MBOX_GUILD_PUBLIC_MBOX = 115,
        GC_MBOX_GUILD_INTRODUCE_EDIT = 116,
        GC_MBOX_BINGO_DLG = 117,
        GC_MBOX_QUIZ_DLG = 118,
        GC_MBOX_MONSTER_CARD_BOOK = 119,
        GC_MBOX_ITEM_SOCKET_MANAGEMENT_DLG = 121,
        GC_MBOX_ITEM_BREAKUP_DLG = 123,
        GC_MBOX_ITEM_CREATION_DLG = 124,
        GC_MBOX_PUZZLE_DLG = 126,
        GC_MBOX_GIFT_ITEM_BOX = 127,
        GC_MBOX_TOURNAMENT_BOX = 128,
        GC_MBOX_TUTORIAL_OPTION_NEW = 130,
        GC_MBOX_CHARACTER_SELECT_BOX = 131,
        GC_MBOX_COORDI_CHANGE_DLG = 132,
        GC_MBOX_SELL_COUNT_ITEM = 133,
        GC_MBOX_GAMBLE_BOX = 134,
        GC_MBOX_GET_NEW_CHARACTER = 135,
        GC_MBOX_SIZING_OK = 136,
        GC_MBOX_HERO_DUNGEON_DLG = 137,
        GC_MBOX_MISSION_COMPLETE_ITEM_LIST = 138,
        GC_MBOX_EVENT_BANNER = 139,
        GC_MBOX_REPLAY_RECORD = 140,
        GC_MBOX_REPLAY_BOX_NEW = 141,
        GC_MBOX_GAWIBAWIBO_DLG = 142,
        GC_MBOX_MISSION_UI_S4_DLG = 143,
        GC_MBOX_MISSION_PREVIEW_S4_DLG = 144,
        GC_MBOX_SOCKS_TREE_DLG = 145,
        GC_MBOX_WEAPON_CHANGE_DLG = 146,
        GC_MBOX_ATTRIBUTE_SELECT_BOX = 147,
        GC_MBOX_NICKNAME_CAMPAIGN_BOX = 148,
        GC_MBOX_PLANT_TREE_DLG = 149,
        GC_MBOX_PLANT_TREE_REWARD_LIST_VIEW_DLG = 150,
        GC_MBOX_CHOICE_DROP = 153,
        GC_MBOX_GUILDSHOP_DLG = 154,
        GC_MBOX_GUILD_LEVEL_LIST_DLG = 155,
        GC_USER_MIGRATION_DLG = 156,
        GC_MBOX_NEW_CLEAR_SEAL_BEAD = 157,
        GC_MBOX_PACKAGE_PREVIEW = 158,
        GC_MBOX_MAGIC_BOX = 159,
        GC_MBOX_FASHION_CATALOG_BOX = 160,
        GC_MBOX_VIP_REWARD = 161,
        GC_MBOX_RICKOON_TORNADO_DLG = 162,
        GC_MBOX_EVENT_SPHINX_DLG = 163,
        GC_MBOX_EVENT_MINIGAME_LIST_DLG = 164,
        GC_MBOX_DOTA_SHOP = 165,
        GC_MBOX_PARTY_DUNGEON = 166,
        GC_MBOX_PARTY_MATCH = 167,
        GC_MBOX_PARTY_WINDOW = 168,
        GC_MBOX_PARTY_INVITE = 169,
        GC_MBOX_AGIT_SHOP = 170,
        GC_MBOX_MANTIS_REPORT_DLG = 172,
        GC_MBOX_FORGE_DLG = 173,
        GC_MBOX_SURVEY_DLG = 174,
        GC_MBOX_SHUTDOWN = 175,
        GC_MBOX_GWC_EVENT_DLG = 176,
        GC_MBOX_GWC_EVENT_CATALOG = 177,
        GC_MBOX_GAME_INVENTORY = 178,
        GC_MBOX_CHRISTMAS_GIFT_DLG = 179,
        GC_MBOX_POST_LETTER = 180,
        GC_MBOX_WELCOMEBACK_DLG_NEW = 181,
        GC_MBOX_ECLIPSE_TIME = 182,
        GC_MBOX_EVENT_ECLIPSE_PLOT_DLG = 183,
        GC_MBOX_ECLIPSE_OLD_BOOKS_DLG = 184,
        GC_MBOX_ITEM_LEVEL_DOWN_DLG = 185,
        GC_MBOX_HOT_TIME_SEARCH_DLG = 186,
        GC_MBOX_MONSTER_CARD_COMPOSE_DLG = 187,
        GC_MBOX_NEW_NECKLACE_ENCHANT_DLG = 188,
        GC_MBOX_DEPOT = 189,
        GC_MBOX_TRANSFER_MISSION_SCROLL_GUIDE_DLG = 190,
        GC_MBOX_SONGKRAN_DLG = 191,
        GC_MBOX_EVENT_ALARM_DLG = 192,
        GC_MBOX_SONGKRAN_REWARD_DLG = 193,
        GC_MBOX_GC_ADVENTURE_DLG = 194,
        GC_MBOX_OLYMPIC_DLG = 195,
        GC_MBOX_OLYMPIC_GAWIBAWIBO_DLG = 196,
        GC_MBOX_OLYMPIC_DICE_DLG = 197,
        GC_MBOX_OLYMPIC_SONGKRAN_DLG = 198,
        GC_MBOX_ROOM_LIST_DLG_S6 = 199,
        GC_MBOX_ROOM_DUNGEON_SELECT_DLG_S6 = 200,
        GC_MBOX_ROOM_CREATE_DLG_S6 = 201,
        GC_MBOX_CHAT_BOX_DLG_S6 = 204,
        GC_MBOX_SCENE_MENU_S6 = 205,
        GC_MBOX_LEVEL_RESET_ITEM = 206,
        GC_MBOX_GC9th_EVENT_DLG = 207,
        GC_MBOX_LEVEL_ITEM_DLG = 208,
        GC_MBOX_ITEM_COMBINATION_DLG = 209,
		GC_MBOX_LOTTERY = 210,		
		GC_MBOX_ROOM_QUICK_JOIN = 211,		
        GC_MBOX_ROOM_USER_INFO_POPUP = 212,
        GC_MBOX_ITEM_CREATION_SELECT_BOX = 213,
		GC_MBOX_MATCH_MAP_SELECT_BOX = 214,
		GC_MBOX_MATCH_INVITE_USER_LIST_BOX = 215,
		GC_MBOX_MATCH_COMPLETE_DLG = 216,
		GC_MBOX_AUTOMATCH_LOADING = 217,
		GC_MBOX_RITASCHRISTMAS_DLG = 218,
        GC_MBOX_SUPER_GACHA = 219,
        GC_MBOX_ATTENDANCE_STAMP_DLG = 220,
        GC_MBOX_BUFF_DONATION_DLG = 221,
        GC_MBOX_ITEM_BIND_DLG = 222,
        GC_MBOX_BUY_CASH_ITEM_CHOOSE_CASH_TYPE_DLG = 223,
        GC_MBOX_BUY_CASH_ITEM_ACCUMULATED_GC_DLG = 224,
        GC_MBOX_BUY_CASH_ITEM_DLG_S6 = 225,
        GC_MBOX_BUY_CASH_ITEM_BUY_COMPLETE = 226,
		GC_MBOX_CHALLENGE_ERRAND = 227,
        GC_MBOX_ADDITEM_IN_CLIENT = 228,
        GC_MBOX_EXTEND_LOOK_INVENTORY = 229,
        GC_MBOX_TUTORIAL_EXIT = 230,
		GC_MBOX_FATIGUE_SYSTEM_INFO = 231,
		GC_MBOX_FATIGUE_SYSTEM_MESSAGE = 232,
		GC_MBOX_FATIGUE_SYSTEM_MESSAGE2 = 233,
		GC_MBOX_AGREEMENT2 = 234,
		GC_MBOX_CHANGE_WEB_CASH = 235,
		GC_MBOX_OK_CANSEL_BOX = 236,
		GC_MBOX_JOIN_CANSEL_BOX = 237,
		GC_MBOX_INFO_CANSEL_BOX = 238,
        GC_MBOX_ESC_MENU_DLG = 239,
		GC_MBOX_JUMPING_CHAR_DLG = 240,
        GC_MBOX_CREATE_ROOM_S6 = 241,
        GC_MBOX_CHANGE_ROOM_TITLE_S6 = 242,
        GC_MBOX_SQUARE_ENTER_S6 = 243,
        GC_MBOX_REPLAY_S6 = 244,
        GC_MBOX_COORDI_COMPOSE_DLG = 245,
        GC_MBOX_VIRTUALDEPOT = 246,
        GC_MBOX_CHARNICKNAME_CHANGE_DLG = 247,
		GC_MBOX_SOCIAL_COMMERCE = 248,
		GC_MBOX_COORDI_UPGRADE_DLG = 249,
 	    GC_MBOX_HERO_DUNGEON_SEASON5 = 250,
        GC_MBOX_EVENT_DUNGEON_SEASON5 = 251,
		GC_MBOX_PRESENT_CASH_ITEM_WND = 252,
		GC_MBOX_PRESENT_FRIEND_LIST = 253,
        GC_MBOX_INFINITY_GUIDE_DLG = 254,
        GC_MBOX_ENCHANT_S6_DLG = 255,
        GC_MBOX_ENCHANT_SUB_S6_DLG = 256,
        GC_MBOX_DUNGEON_RANKING_DLG = 257,
        GC_MBOX_BILLBOARD_DLG = 258,
        GC_MBOX_NEW_COMBO_BOX_SHOP = 259,
        GC_MBOX_KGCHeroDungeonInfoS6 = 260,
        GC_MBOX_HERODUNGEON_SHOP_DLG_S6 = 261,
        GC_MBOX_NEW_BOX_HERO_REWARD = 262,
        GC_MBOX_NICKNAME_COLOR_DLG = 263,
        GC_MBOX_GACHA_PONG_DLG = 264,
        GC_MBAX_PET_MAGIC_STONE_DLG = 265,
        GC_MBOX_ROOM_OPTION_RANK_DLG = 266,
        GC_MBOX_ROOM_OPTION_CHAR_DLG = 267,
        GC_MBOX_ROOM_LIST_FILTER_DLG = 268,
        GC_MBOX_COORDI_VIEW_DLG = 269,
        GC_MBOX_CUSTOMIZATION_DLG = 270,
        GC_MBOX_QUANTITYUSE_DLG = 271,
        GC_MBOX_SHORT_MENU_DLG = 272,
        GC_MBOX_HERO_TICKET_DLG = 273,
        GC_MBOX_ALL_COLLECTION_DLG = 274,
    };

    enum EGCMsgBoxReturn
    {
        GC_MBOX_RETURN_OK,
        GC_MBOX_RETURN_CANCEL,
        GC_MBOX_RETURN_YES,
        GC_MBOX_RETURN_NO,
    };

    enum EGCMsgBoxUse
    {
        GC_MBOX_USE_NORMAL,
        GC_MBOX_USE_GAMEEXIT,
        GC_MBOX_USE_EXIT,
        GC_MBOX_USE_BUY,
        GC_MBOX_USE_SELL,
        GC_MBOX_USE_BUY_FOR_GEM,
        GC_MBOX_USE_BUY_FOR_CRYSTAL,
        GC_MBOX_USE_REPAIR,
        GC_MBOX_USE_NICK_NAME_AGREE,
        GC_MBOX_USE_CHANGE_NICK_NAME,
        GC_MBOX_USE_DELETE_ITEM,
        GC_MBOX_USE_DELETE_MISSION,
        GC_MBOX_USE_CERTIFY_GASH,
        GC_MBOX_USE_PET_FEEDING,
        GC_MBOX_USE_CHANGE_PET_NICK_NAME,
        GC_MBOX_USE_EVENT_MSG,
        GC_MBOX_USE_EVENT_RECOMMAND,
        GC_MBOX_USE_EXTEND_INVENTORY,
        GC_MBOX_USE_BUY_POINT_ITEM,
        GC_MBOX_USE_DELETE_POINT_ITEM,
        GC_MBOX_USE_GET_REWARD_ITEM,
        GC_MBOX_USE_RESET_WIN_LOSE_DATA,
        GC_MBOX_USE_END_GAME,
        GC_MBOX_USE_NEONSIGN_RESERVE,
        GC_MBOX_USE_DELETE_COLLECTION_MISSION,
        GC_MBOX_USE_MINIGAME_EXIT,
        GC_MBOX_USE_NATEON_DELETE_GROUP,
        GC_MBOX_USE_NATEON_DELETE_GROUP_REALLY,
        GC_MBOX_USE_NATEON_SELECT_EMOTICON,
        GC_MBOX_USE_NATEON_ADD_GROUP,
        GC_MBOX_USE_NATEON_RENAME_GROUP,
        GC_MBOX_USE_NATEON_BLOCK_BUDDY,
        GC_MBOX_USE_NATEON_ADD_BUDDY,
        GC_MBOX_USE_NATEON_INVITE_BUDDY,
		GC_MBOX_USE_NATEON_ADD_BUDDY_ALRAM,
		GC_MBOX_USE_NATEON_INPUT_EMOTICON,
		GC_MBOX_USE_LEARN_SKILL,
		GC_MBOX_USE_REMOVE_SKILL,
		GC_MBOX_USE_NATEON_MSG_SEND,
		GC_MBOX_USE_NATEON_MSG_SENDED,
		GC_MBOX_USE_NATEON_MSG_RECIVE,
		GC_MBOX_USE_CHANGE_COUPON,
		GC_MBOX_USE_COUPLE_ITEM_CONFIRM,
		GC_MBOX_USE_COUPLE_RING_CHANGE,
		GC_MBOX_USE_COUPLE_GOODBYE_REQ,
		GC_MBOX_USE_COUPLE_GOODBYE_NOT,
		GC_MBOX_USE_COUPLE_INSERT_INVEN,
		GC_MBOX_USE_COUPLE_VIEWER_OTHERS,
		GC_MBOX_USE_BUY_VIP,
		GC_MBOX_USE_GACHA_AUTO,
        GC_MBOX_USE_RECOMMEND_NOT,
		GC_MBOX_USE_RECOMMEND_PICKED_USER,
        GC_MBOX_USE_RECOMMEND_NICKNAME,
		GC_MBOX_GIFTS_OK,
		GC_MBOX_DETECT_CHANGED_KOM_OK,
        GC_MBOX_USE_VIRTUAL_CASH_COUPON,
		GC_MBOX_GUILD_BREAK_UP,
		GC_MBOX_GUILD_MASTER_COMMTION,
		GC_MBOX_GUILD_DESTROY,
		GC_MBOX_GUILD_JOIN_CANCEL_REQ,
		GC_MBOX_GUILD_SQUARE_OUT,
		GC_MBOX_USE_INCOMPLETE_PATCH_MSG,
		GC_MBOX_GUILD_MARK_UPLOAD_ALERT,
		GC_MBOX_GUILD_NAME_CHANGE_ALERT,
		GC_MBOX_GUILD_JOIN_RESULT,
        GC_MBOX_USE_CREATE_GUILD_UPDATE,
        GC_MBOX_USE_PET_EVOLUTION_BOX,
        GC_MBOX_USE_PET_DEVOLUTION_BOX,
        GC_MBOX_USE_PET_HATCH_BOX,	
        GC_MBOX_USE_EVOLUTION_PET_NAME_SELECT,		
        GC_MBOX_USE_DEVOLUTION_PET_NAME_SELECT,
        GC_MBOX_USE_TOURNAMENT_VOTE,
		GC_MBOX_USE_GAMBLE_DELETE,
        GC_MBOX_USE_SELL_BUNDLE_ITEM,
        GC_MBOX_USE_BUY_HERO_ITEM,
		GC_MBOX_USE_MAGIC_PROMOTION,
		GC_MBOX_USE_INIT_ATTRIBUTE,
        GC_MBOX_USE_WEAPON_CHANGE,
        GC_MBOX_USE_CHANGE_NICK_NAME_FROM_CAMPAIGN,
        GC_MBOX_RESET_SKILL,
        GC_MBOX_USE_CLOSE_CLIENT_FOR_CASH_CHARGE,
        GC_MBOX_USE_DOTA_GAME,
        GC_MBOX_USE_BUY_AGIT_SHOP,
        GC_MBOX_USE_SELL_AGIT_OBJECT,
        GC_MBOX_USE_AGIT_MOVE,
        GC_MBOX_USE_INVITE_AGIT_MESSAGE,
        GC_MBOX_USE_ADD_FAVORITE_AGIT,
        GC_MBOX_USE_INVITE_AGIT_INPUT_PASSWORD,
        GC_MBOX_USE_AGIT_ERROR,
        GC_MBOX_USE_SEED_PLANT_CONFIRM,
        GC_MBOX_USE_DELETE_GUESTBOOK_MSG,
        GC_MBOX_USE_AGIT_CHARACTER_OPEN,
        GC_MBOX_USE_VIRTUAL_CASH_INCLUDE_CASH,  // 대만에서 사용하는 가상캐쉬 혼용구매
        GC_MBOX_USE_PET_SKILL_BUY,					// 펫 스킬 구매
        GC_MBOX_USE_DELETE_LETTER,
        GC_MBOX_USE_EXP_POTION,
		GC_MBOX_USE_DEPOT_EXTEND, //창고 확장 물음.
        GC_MBOX_USE_OPEN_CHANGE_SKILL_SLOT,
        GC_MBOX_USE_GC_ADVENTURE_FINAL_REWARD,  //GC 원정대 최종 보상 받을 것인지 물음		
        GC_MBOX_USE_LEVEL_RESET_ITEM,
        GC_MBOX_USE_INIT_SKILL_SETTINGS,
        GC_MBOX_USE_BUY_HERO_ITEM_UPGRADE, 
        GC_MBOX_USE_BUY_CASH_ITEM_BY_GC, 
        GC_MBOX_USE_ERRAND_EXIT,
        GC_MBOX_USE_EXTEND_LOOK_INVENTORY,
        GC_MBOX_USE_TUTORIAL_GUIDE_ENTER,
        GC_MBOX_USE_TUTORIAL_EXIT,
        GC_MBOX_USE_RECHARGE_VITALITY,
        GC_MBOX_USE_SELL_INVEN_ITEM,
        GC_MBOX_USE_JUMPING_CHAR,
        GC_MBOX_USE_JUMPING_CHAR_REWARD,
        GC_MBOX_USE_VIRTUAL_INVEN_MOVEN,
        GC_MBOX_USE_VIRTUAL_INVEN_MOVEN_COMPLATE,
        GC_MBOX_USE_VIRTUAL_DEOPT_EXIT,
        GC_MBOX_USE_SKILL_SCROOL_ITEM_USE,
        GC_MBOX_USE_KAIRO_ONESHOT,
        GC_MBOX_USE_KAIRO,
        GC_MBOX_USE_BUY_QUANT_ITEM_BY_GC,
        GC_MBOX_UNDO_COORDI_ITEM,
        GC_MBOX_COORDIVIEW_SAVE_SLOT,
        GC_MBOX_USE_BUY_FONTSKIN,
        GC_MBOX_NEW_QUANTITY_USE,
        GC_MBOX_USE_FORGE_CONFIRM,
        GC_MBOX_USE_BREAKUP_CONFIRM,
        GC_MBOX_USE_COMPOSE_CONFIRM,
        GC_MBOX_USE_BUY_FOR_VP_AMMOUNT,
        GC_MBOX_USE_MAX,
    };

    struct KMsgBoxInfo
    {
        EGCMsgBoxType   m_eMsgBoxType;
        EGCMsgBoxUse    m_eMsgBoxUse;
        std::wstring    m_strText1;
        std::wstring    m_strText2;
        KEVETPARM       m_lParam;
        KEVETPARM       m_lParam2;

        KMsgBoxInfo( void )
        {
            m_eMsgBoxType = GC_MBOX_OK;
            m_eMsgBoxUse = GC_MBOX_USE_NORMAL;
            m_strText1.erase();
            m_strText2.erase();
            m_lParam = 0;
            m_lParam2 = 0;
        }
        KMsgBoxInfo( const KMsgBoxInfo& src )
        {
            m_eMsgBoxType = src.m_eMsgBoxType;
            m_eMsgBoxUse  = src.m_eMsgBoxUse;
            m_strText1    = src.m_strText1;
            m_strText2    = src.m_strText2;
            m_lParam	  = src.m_lParam;
            m_lParam2	  = src.m_lParam2;
        }
        const KMsgBoxInfo& operator=( const KMsgBoxInfo& src )
        {
            m_eMsgBoxType = src.m_eMsgBoxType;
            m_eMsgBoxUse  = src.m_eMsgBoxUse;
            m_strText1    = src.m_strText1;
            m_strText2    = src.m_strText2;
            m_lParam	  = src.m_lParam;
            m_lParam2	  = src.m_lParam2;

            return *this;
        }
    };

public:
    inline bool IsMsgBoxModal( void ) const
    {
        return m_bMsgBoxModal;
    }
    inline void SetMsgBoxModal( bool bModal )
    {
        m_bMsgBoxModal = bModal;
    }
    HRESULT EnterRoom( int iRoomNum, const std::wstring& strRoomName );

	bool IsMessageBoxType( EGCMsgBoxType eMsgBoxType );

    void MessageBox( EGCMsgBoxType eMsgBoxType,
        const std::wstring& strText1 = L"",
        const std::wstring& strText2 = L"",
        EGCMsgBoxUse eUse = GC_MBOX_USE_NORMAL,
        KEVETPARM lParam = 0,
        KEVETPARM lParam2 = 0,
        bool bFirst = false,
        bool bForce = false,
        bool bErasePrevMsgBox = false,
        bool bMsgBoxBack = true );
	void LuaMessageBox( EGCMsgBoxType eMsgBoxType,
		EGCMsgBoxUse eUse = GC_MBOX_USE_NORMAL,
		KEVETPARM lParam = 0,
		KEVETPARM lParam2 = 0,
		int bForce = 0,
		int iStringID1 = -1,
		int iStringID2 = -1);

    virtual void ActionPerformed( const KActionEvent& event );

    void DestroyCurrentMsgBox( void );
	void EnableBuyCashItemWnd( DWORD dwGoodID_, bool bPresent_, bool bTopMost = false, bool waitFirst =false );
	void EnableMessageBox( bool bEnable );
    KD3DWnd* GetCurMsgBox(){ return m_pkCurMsgBox; }
    void DestroyMsgBox();
    void UpdateRoomTitle();
    void EndMessageBox( void );
    bool IsWaitMsgBox( EGCMsgBoxType eMBoxType_ );
    bool IsWaitMsgBox_Use( EGCMsgBoxUse eMBoxUse_ );
    bool IsCurMsgBox_Type( EGCMsgBoxType eMBoxType_ );
    bool IsCurMsgBox_Use( EGCMsgBoxUse eMBoxUse_ );
	bool DeleteAllWaitMsgBox( EGCMsgBoxType eMBoxType_ );
    bool DeleteAllWaitMsgBox( void );
    bool DeleteWaitMsgBox( EGCMsgBoxType eMBoxType_ );
    bool DeleteWaitMsgBoxforUse( EGCMsgBoxUse m_eMsgBoxUse_ );
	void SetActiveWndInStop( KD3DWnd* pkWnd );
	void SetChildWndUpdateLock( bool bLock );
	void VirtualInput( D3DXVECTOR2 vPosMouse );

    void SetLastColorItemDlg( KGCColorItemUse* pWnd ) { m_pkLastColorItemUse = pWnd; }
    KGCColorItemUse* GetLastColorItemDlg() const { return m_pkLastColorItemUse; }
    void SetLastColor(std::wstring strSelColor);

    void OpenCollectionWindow( int iType = 0 );
    void CloseCollectionWindow( void );
    void SelectCollection( int iType = 0  );
    void BlockBuddy( std::wstring strNickName );
    bool IsValidNickname();
    void ChangeNickColor(GCITEMID dwItemID_, GCITEMUID dwItemUID_);
protected:
    virtual void OnEnter( void );
    virtual void OnClear( void );
    virtual void FrameMoveInEnabledState( void );
    void FrameMove_MouseCursor();
    void SetActiveMsgBox( KD3DWnd* pkMsgBox );

protected:
    void BuyFontSkinForCrystal(IN const GCITEMID itemID_, IN const int nCount_, IN const int nPeriod_);
    void BuyGem( int iGemNum );
    void SellCountItem( void );
    void ChargeBonus( KItem* pItemData, UINT nChargeFactor );

    //기간제 아이템의 추가로.. ItemID가 동일한 Item들이 존재하게 되면서.. 
    //구입시 iItemCount로 구분합니다. -1 : 일반 수량
    void BuyItemForGem( IN const GCITEMID itemID, IN const int nCount, IN const int nPeriod );
    void BuyItemForCrystal( IN const GCITEMID itemID, IN const int nCount, IN const int nPeriod );
    void SellItem( KItem*& pkInventoryItemInfo );
    void RepairBreakedItem( KItem*& kInv );
    bool ReportRecommendNickName( std::wstring wstrUserNick );
    void ChangeNickName( GCITEMID dwItemID_, GCITEMUID dwItemUID_ );
    BOOL ChangeNickNameFromCampaign( );
    void ChangePetNickName( IN const KItem& NickNameItem, DWORD dwPetID_, GCITEMUID dwPetUID_ );
    void RemovePointItem( GCITEMUID dwItemUID );
    void UseColorItem( KGCColorItemUse* pTargetWnd );

    void AddNateOnBuddyGroup( std::wstring strGroupName );
    void RenameNateOnBuddyGroup( int nGroupID, std::wstring strGroupName );
    void DeleteNateOnBuddyGroup( int nGroupID );
    void ChangeEmoticon();

protected:
    bool        m_bChangeRoomTitle;
    bool        m_bCreateRoom;
    bool        m_bConnect;
    bool        m_bMsgBoxModal;

    bool		m_bRPLoadSuccess;

    int         m_iCoverPosSave;

    bool        m_bExitMinigame;

    std::wstring m_strNickName;
    KD3DWnd*    m_pkCurMsgBox;
	KD3DWnd*	m_pkActiveWnd;
    KD3DWnd*    m_pkCurActiveTab;

public:
    KGCServerScene*             m_pkServerScene;
#ifdef NEW_CHAR_SELECT_SCENE 
    KGCCharSelectScene_New* m_pkCharSelectScene;
#else
    KGCCharSelectScene* m_pkCharSelectScene;
#endif
	KGCCharGainScene*           m_pkCharGainScene;
    KGCMatchScene*              m_pkMatchScene;
    KGCWorldMapScene*           m_pkWorldMapScene;
    KGCCashShopScene*           m_pkCashShopScene;
    KGCGPShopScene*             m_pkGPShopScene;
    KGCGuildScene*              m_pkGuildScene;
    KGCMyInfoScene*             m_pkMyInfoScene;
    KGCRoomSceneS6*             m_pkRoomSceneS6;        
    KGCLoginScene*              m_pkLoginScene;
    KGCRegisterScene*           m_pkRegisterScene;
    KGCGameOverlayUI*           m_pkGameOverlayUI;
    KGCSquareOverlayUI*         m_pkSquareOverlayUI;
    KGCSquareScene*             m_pkSquareScene;
    KGCCoordiShopScene*         m_pkCoordiShopScene;
    KGCGuildStore*               m_pkGuildShop;
    KGCItemInfoBox*             m_pkCollectionItemInfoBox;
    KD3DWnd*                    m_pkBackGroundWhite;
    KGCSellCountItemDlg*        m_pkSellCountItemDlg;
    KGCCoordiChangeDlg*         m_pkCoordiChangeDlg;
    KGCOnGameDlg*               m_pkOnGameDlg;
    KGCOption*                  m_pkOption;
    GCNateOn*                   m_pkNateOn;
	GCNateOnMsg*                m_pkNateOnMessage;
	GCNateOnInviteBuddy*        m_pkNateOnInvite;
    KGCLoadingScene*            m_pkLoadingScene;
    KGCCandidateWnd*            m_pkCandidateWnd;
    KGCColorDlg*                m_pkColorDlg;
    KGCToolTip*                 m_pkToolTip;
    KGCNickNameBox*             m_pkNickNameBox;
    KGCPetPreview*              m_pkPetPreview;	
    KGCEventBox*                m_pkEventBox;
    KGCRepetitionEventBox*      m_pkRepetitionEventBox;
    KGCPetEvolutionBox*         m_pkPetEvolutionBox;
    KGCSelectPetNameBox*        m_pkSelectPetNameBox;
    KGCAttendanceCalendar*      m_pkAttendanceCalendar;
    KGCPerfectAttendanceBox*    m_pkPerfectAttendance;
    KGCNewTutorialOption*       m_pkNewTutorialOption;
    KGCLuckyDayReceiveBox*      m_pkLuckyDayReceiveBox;
    KGCReportBox*               m_pkFairPlayReport;
    KGCFadeoutWnd*              m_pkGradeBox;
    KGCChannelListBox*          m_pkChannelListBox;
    KGCItemReceiveBox*          m_pkItemReceiveBox;
	KGCGuideNewbie*				m_pkGuideNewbie;
    KGCGuideNewbie*				m_pkGuideAzit;
    KGCReplayRecordDlg*         m_pkReplayRecordDlg;
    KGCReplayRecordREC*         m_pkReplayRecordREC;
    KGCReplayBoxNew*            m_pkReplayBoxNew;
    KGCWorldMapContainer*       m_pkWorldMapContainer;
    KGCSquareEnterWndS6*        m_pkSquareEnterWndS6;
    KGCSquareLoadingScene*      m_pkSquareLoadingScene;
    KGCEmoticonWindow*          m_pkEmoticonCollection;
    KGCTitleWindow*             m_pkTitleCollection;
    KGCPetWindow*               m_pkPetCollection;
    KGCCTConditionWindow*       m_pkCTConditionWindow;
    KGCTreasureItemBox*         m_pkTreaureItemBox;
    KGCInviteRequestBox*        m_pkInviteRequestBox;
    KGCInviteMsgBox*            m_pkInviteBox;
    KGCNeonsignSelectBox*       m_pkNeonsignSelectBox;
    KGCNeonsignPreview*         m_pkNeonsignPreview;
	KGCRankingDlg*				m_pkRanking;
    KGCConnectionMsg*           m_pkConnectionMsg;
    KGCWelcomeBack*             m_pkWelcomeBackDlg;
    KGCAskUseVirtualCash*       m_pkAskUseVirtualCash;
    GCNateOnAddGroupDlg*        m_pkNateOnInputDlg;
    GCEmoticonSelectDlg*        m_pkEmoticonSelect;
    GCNateOnAddFriend*          m_pkNateOnAddFriend;
	GCBlockToAddFriend*			m_pkBlockToAddFriend;
	KGCSkillTreeDlg*			m_pkSkillTree;
    KGCMinigameDlg*             m_pkMiniGameDlg;
	KGCCoupleViewer*			m_pkCoupleViewer;
	KGCPetCostumeInven*			m_pkPetCostumeInven;
	KGCContributionDlg*			m_pkContributionDlg;
	KGCContributionBox*			m_pkContributionBox;
	KGCHappyNewYearNPCDlg*		m_pkHappyNewYearNPCDlg;
    KGCRecommendList*           m_pkRecommendList;
	KGCGPCapsuleDlg*			m_pkGPCapsuleDlg;	
	KGCGPCapsuleObtainWnd*		m_pkGPCapsuleObtainWnd;
	KGCCouponNPCDlg*			m_pkCouponNPCDlg;	
	KGCSubscriptionDlg*			m_pkSubscriptionDlg;
	KGCSubscriptionBox*			m_pkSubscriptionBox;
    KGCCooperationEventDlg*     m_pkCooperationEventDlg;
    KGCGuildMarkRegistDlg*      m_pkGuildMarkRegistDlg;
    KGCGuildDlg*                m_pkGuildDlg;
    KGCGuildEnterDlg*           m_pkGuildEnterDlg;
    KGCGuildLobbyDlg*           m_pkGuildLobbyDlg;
	KGCGuildListDlg*			m_pkGuildListDlg;
    KGCGuildLevelList*          m_pkGuildLevelListDlg;
	KGCGuildCreateDlg*			m_pkGuildCreateDlg;
	KGCGuildJoinReqMbox*		m_pkGuildJoinReqMbox;
	KGCGuildPublicMbox*			m_pkGuildPublicMbox;
	KGCGuildSquareNotice*		m_pkGuildSquareNotice;
	KGCCommonEditBox*			m_pkGuildIntroduceEdit;
    KGCVipGachaBox*             m_pkVipGachaBox;
	KGCDicePlay*				m_pkDicePlay;
    KGCBingo*                   m_pkBingo;
    KGCQuiz*                    m_pkQuiz;
	KGCPuzzleBox*               m_pkPuzzle;
    KGCItemInformationBoxPair*  m_pkItemInformationBox;
    KGCMonsterCardBook*         m_pkMonsterCardBook;
    KGCItemSocketManagementDlg* m_pkItemSocketManagementDlg;
    KGCInventoryFullMsg*        m_pkInventoryFullMsg;
 	KGCInventoryFullMsg*        m_pkAttendanceInfoMsg;
    KGCInventoryFullMsg*        m_pkEventCreateInfoMsg;
    KGCInventoryFullMsg*        m_pkNewSkillMsg;
    KGCGiftItemBox*				m_pkGiftItemBox;
	KGCHurryUPDlg*              m_pkHurryUPDlg;
	KGCCharacterSelectBox*		m_pkCharacterSelectBox;
	KGCGambleBox*               m_pkGambleBox;	
    KGCCharacterGetInfoDlg*     m_pkCharGetInfoDlg;
    KGCHeroDungeonDlg*          m_pkHeroDungeonDlg;
	KGCGawibawiboGame*			m_pkGawibawiboGame;
	KGCSocksTreeDlg*			m_pkSocksTreeDlg;
    KGCMissionUI_S4*			m_pkMissionUI_S4;
    KGCMissionEquipPreview_S4*  m_pkMissionEquipPreview_S4;
    KGCWeaponChangeBox*         m_pkWeaponChangeBox;
	KGCAttributeSelectBox*		m_pkAttributeSelectBox;
    KGCLevelupInfoBox*          m_pkLevelupInfoBox;
    KGCNicknameCampaignBox*     m_pkNicknameCampaignBox;
    KGCCoordiCatalogDlg*        m_pkCoordiCatalogDlg;
    KGCPlantTree*               m_pkPlantTree;
    KGCPlantTreeRewardItemListView*               m_pkPlantTreeRewardItemListView;
    KGCChoiceDrop*              m_pkChoiceDrop;
    KGCMagicBox*                m_pkMagicBox;
    KGCVipReward*               m_pkVipReward;
    KGCUserMigrationDlg*		m_pkUserMigrationDlg;
	KGCPackagePreview*          m_pkPackagePreview;
    KGCRickoonTornado*			m_pkRickoonTornado;
    KGCSphinxDlg*               m_pkSphinxDlg;
    KGCPartyWindow*             m_pkPartyWnd;
    KGCPartyDungeon*            m_pkPartyDungeon;
    KGCPartyMatch*              m_pkPartyMatch;
    KGCPartyInviteMsgBox*       m_pkPartyInviteMsgBox;
    KGCEventMinigameList*       m_pkEventMinigameList;
    KGCDotaShop*                m_pkDotaShop;	
    KGCAgitOverlayUI*           m_pkAgitOverlayUI;
    KGCAgitShop*                m_pkAgitShop;
    KGCGameInventory*           m_pkGameInventory;
    KGCMantisReportDlg*         m_pkMantisReportDlg;
    KGCMsgShutDown*             m_pkShutDownDlg;
    KGCNewForgeDlg*             m_pkForgeDlg;
    KGCSurveyDlg*               m_pkSurveyDlg;
    KGCGwcEvent*                m_pkGwcEventdlg;
    KGCGwcRewardCatalogDlg*     m_pkGwcRewardCatalogDlg;
    KGCChristmasGiftDlg*        m_pkChristmasGiftDlg;
    KGCEventEclipsePlotDlg*     m_pkEventEclipsePlotDlg;
    KGCTransferMissionScrollGuideDlg* m_pkTransferMissionScrollGuideDlg;
	KGCSongkranDlg*				m_pkSongkranDlg;
	KGCSongkranRewardDlg*		m_pkSongkranRewardDlg;
	KGCEventAlarmDlg*			m_pkEventAlarmDlg;
	KGCRitasChristmas*			m_pkRitasChristmasDlg;
    KGCOlympicEventDlg*         m_pkOlympicDlg;
    KGCOlympicGawibawiboGame*   m_pkOlympicGawibawiboDlg;
    KGCOlympicDicePlay*         m_pkOlympicDicePlayDlg;
    KGCOlympicSongkranDlg*      m_pkOlympicSongkranDlg;
	KGCLottery*					m_pkLottery;
    KGCMessageBox*              m_pkMsgBox;
	KGCMessageBox*              m_pkMsgBox2;
	KGCMessageBox*              m_pkMsgSizingBox;
    KGCPasswordBox*             m_pkPasswdBox;
    KGCGemBuyBox*               m_pkGemBuyBox;
    KGCQuantityPurchaseBox*     m_pkNewComboBox;
    KGCLoadingBox*              m_pkLoadingBox;
    KGCBuyCashItemWnd*          m_pBuyCashItemWnd;
	KGCChangeCash*				m_pkChangeCash;
    KGCBonusChargeBox*          m_pkBonusChargeBox;
	KGCLogoutBox*				m_pkLogoutBox;
	KGCAgreement*				m_pkAgreement;
	KGCAgreement1301*			m_pkAgreement1301;
	KGCCoupleAccept*			m_pkCoupleAccept;
	KGCCouplePropose*			m_pkCouplePropose;
	KGCCoupleWait*				m_pkCoupleWait;
	KGCCoupleMessage*			m_pkCoupleMessage;
	KGCCoupleGoodbye*			m_pkCoupleGoodbye;
	KGCBuyCashBox*				m_pkBuyCashBox;
	KGCDepotDlg*					m_pkDepot;
    KGCEclipseTime*             m_pkEclipseTime;
    KGCEclipseOldBooks*         m_pkEclipseOldBooks;
    KGCNotifyMessage*           m_pkNotifyMessage;
    KGCPostLetter*              m_pkPostLetterDlg;
    KGCWelcomeBackNew*          m_pkWelcomeBackNewDlg;
    KGCItemLevelDownDlg*        m_pkItemLevelDownDlg;
    KGCHotTimeSearchDlg*        m_pkHotTimeSearchDlg;
	KGCMonsterCardCompose*		m_pkMonsterCardCompose;
	KGCNewNecklaceEnchantDlg*	m_pkNewNecklaceEnchantDlg;
	KGCRoomListDlgS6*			m_pkRoomListDlgS6;
	KGCRoomDungeonSelectDlgS6*	m_pkRoomDungeonSelectDlgS6;
	KGCRoomCreateDlgS6*			m_pkRoomCreateDlgS6;
	KGCChatBoxDlgS6*			m_pkChatBoxDlgS6;
	KGCSceneMenuS6*				m_pkSceneMenuBtnS6;
    KGCPlaytimeNotice*          m_pkPlaytimeNoticeUI;
    KGC9thEventDlg*             m_pk9thEventDlg;
    KGCLevelItemDlg*            m_pkLevelItemDlg;
    KGCItemCombinationDlg*      m_pkItemCombinationDlg;
	KGCRoomQuickJoinDlgS6*		m_pkQuickJoinDlg;	
    KGCRoomUserInfoPopupS6*     m_pkRoomUserInfoPopup;
    KGCItemCreationSelectBox*   m_pkItemCreationSelectBox;
	KGCMatchMapSelectDlgS6*		m_pkMatchMapSelectDlg;
	KGCMatchCompleteDlgS6*		m_pkMatchCompleteDlg;
	KGCAutoMatchLoadingS6*		m_pkAutoMatchLoading;
    KGCSuperGachaDlg*           m_pkSuperGachaDlg;
    KGCAttendanceStampDlg*      m_pkAttendanceStampDlg;
    KGCBuffDonationDlg*         m_pkBuffDonationDlg;
    KGCItemBindDlg*             m_pkItemBindDlg;
    KGCAddItemInClient*         m_pkAddItemInClient;
    KGCBuyCashItemChooseCashTypeDlg* m_pkBuyCashItemChooseCashTypeDlg;
    KGCBuyCashItemAccumulatedGCDlg* m_pkBuyCashItemAccumulatedGCDlg;
    KGCBuyCashItemDlgS6*        m_pkBuyCashItemDlgS6;
    KGCBuyCashItemBuyCompleteDlg* m_pkBuyCashItemBuyCompleteDlg;
    KGCChallengeErrandDlg*      m_pkChallengErranDlg;
#if defined(NEW_MENU)
    KGCShortMenuDlg* m_pkShortMenuCutBox;
#endif
    KGCEscMenuDlg*              m_pkEscMenuDlg;
	KGCPresentCashItemWnd*		m_pkPresentCashItemWnd;
	KGCPresentFriendList*		m_pkPresentFriendList;
    KSafeArray<KD3DWnd*,NUM_MAIN_TAB> m_pkTabList;
    KGCAdventureDialog* m_pkGcAdventureDlg;
	KGCFatigueInfoBox* m_pkFatigueInfoBox;
	KGCFatigueMessageBox* m_pkFatigueMessageBox;
	KGCFatigueMessageBox2* m_pkFatigueMessageBox2;
    KGCGuide* m_pkGCGuide;
    KGCGiftBox* m_pkGiftBox;
    KGCJumpingCharDlg*  m_pkJumpingCharDlg;
    KGCCoordiComposeDlg*    m_pkCoordiComposeDlg;
    KGCSocialCommerceDlg* m_pkSocialCommerceDlg;
    KGCCoordiUpgradeDlg*  m_pkCoordiUpgradeDlg;
    KGCHeroDungeonSeason5* m_pkHeroDungeonSeason5;
    KGCEventDungeonSeason5* m_pkEventDungeonSeason5;
    KMsgBoxInfo m_kCurrMsgBoxInfo;
    std::vector<KMsgBoxInfo> m_vecWaitMsgBox;
    std::vector<KD3DWnd*>   m_vecOrgWnd;    //사용 안돼고 있음
    std::vector<KD3DWnd*>     m_vecDestroyDlg;
    COSInfo                 m_OSInfo;
    bool                    m_bEnableMessageBox;
    KGCColorItemUse*    m_pkLastColorItemUse;
	KGCMatchInviteUserListS6 *m_pkMatchInviteUserList;
    CKTDGAnim::CAnimInstance*   m_pkMouseCursor;
    KGCCreateRoomBoxS6*         m_pkCreateRoomBoxS6;
    KGCChangeRoomTitleBoxS6*    m_pkChangeRoomTitleBoxS6;
    KGCReplayBoxS6*             m_pkReplayBoxS6;
    KGCVirtualDepotDlg*         m_pkVirtualDepotDlg;
    KGCInfinityGuideDlg*        m_pkInfinityGuideDlg;
    KGCEnchantS6Dlg*            m_pkEnchantS6Dlg;
    KGCEnchantSubS6Dlg*         m_pkEnchantSubS6Dlg;
    KGCDungeonRankingDlg*       m_pkDungeonRankingDlg;
    KGCBillBoardChatBoard*      m_pkBillBoardChat;
    KGCBillboardChatDlg*        m_pkBillBoardBox;
    KGCHeroDungeonInfoS6* m_pkHeroInfoS6;
    KGCHeroDungeonShopS6Dlg* m_pkHeroDungeonShopS6;
    KGCHeroDungeonRewardDlg* m_pkHeroRewardS6;
    KGCHeroDungeonUpgradeSidePanel* m_pkHeroUpgradeSidePanel;
    KGCNicknameColorBox* m_pkNicknameColorBox;
    KGCGachaPongDlg* m_pkGachaPongDlg;
    KGCRoomOptionCharDlg* m_pkRoomOptionCharDlg; //0x5EC
    KGCRoomOptionRankDlg* m_pkRoomOptionRankDlg; //0x5F0
    KGCRoomListFilterDlg* m_pkRoomListFilterDlg; // ????
    KGCCoordiViewDlg* m_pkCoordiViewDlg;
    KGCCustomizationDlg* m_pkCustomizationDlg;
    KGCQuantityUseBox* m_pkQuantityUseDlg;
    KGCPetMagicStoneDlg* m_pkPetMagicStoneDlg;
    KGCHeroTicketUseWnd* m_pkHeroTicketDlg;
    KGCAllCollection* m_pkAllCollectionDlg;

private:
    KGCCoordiClearSealDlg*		m_pkCoordiClearSealDlg;
    KGCNewClearSealDlg*			m_pkNewClearSealDlg;

public:
    bool IsValidState_ShowMessageBox();
	bool IsCashBoxRenderOn();
	bool IsRenderOn(EGCMsgBoxType mbt);
	KD3DWnd::ED3DWndState GetStateUI(EGCMsgBoxType mbt);

    // 모든 상태에서 떠야하는 메시지 박스인지 확인.
private:
    bool IsAllStateMessageBox( EGCMsgBoxType eMBoxType_ );
    KD3DDialog* GetDialogPtr( EGCMsgBoxType eMboxType_ );
    void SetCurMsgBox( KD3DWnd* pDlg );

    bool m_bToggleBackground;
    int m_iBackgroundAlphaCounter;

	int m_iLastCollectionWindow;

	std::set<EGCMsgBoxType>	m_setDoNotOverlapedWnd;

    bool m_bAllStop;
public:
    void AddDoNotOverlapedWnd( EGCMsgBoxType eID_ );
    void ToggleMessageBoxBack( bool bRender, bool bForce = false );

	bool IsEmptyWaitMessageBox(){ return m_vecWaitMsgBox.empty();}
    void ClearWaitMessageBox(){ m_vecWaitMsgBox.clear();}
    bool RemoveWaitMessageBox( EGCMsgBoxType eMsgBox_ );

	void LuaSendGiftsReq(int iEvetnID_ );

    void RECPlay();
    void RECStop();

    void OnLevelupInfo( void );
    bool IsRenderLevelupInfo( void );
    bool IsAllStop(){return m_bAllStop;}
    void SetAllStop(bool bStop){m_bAllStop = bStop;}

    bool IsInMousePos( D3DXVECTOR2& dxvPos_ );
	
	bool IsSetStop();
    void ShowItemEffectGameInven( bool _bShow );
	void CloseGameInventory( void );	
    void UpdateHotItem();

    void StartPlayTimeNotice( int iIndex_, int iPlayTime = 0 );

    void RoomSceneUpdateData( bool bUpdateExtraModel = false );

    void RoomButtonSetStartDelay( int iDelay = 0 );
    int  RoomButtonGetStartDelay();
    bool RoomButtonIsAutoStart();
    void RoomButtonSetAutoStart( bool bAuto );
    void ShowNewSkillMsg();
    void HideSpeechBubbleMsg();

    void UpdateAllHeroShop();


public:
    void CloseInSquareUI( void );   // 광장 나갈때 열려 있는 UI 모두 닫도록 

    KGCCoordiClearSealDlg* GetClearSealDlg() { return m_pkCoordiClearSealDlg; }
    KGCNewClearSealDlg* GetNewClearSealDlg() { return m_pkNewClearSealDlg; }

    void SetLevelItemDlgInfo( const int iLevel_, std::set< char >& setCharIndex_ );
    void TutorialRoomStart();

public:
    bool IsReplayDlgRenderOn( void );

    bool EnableShowMessageBox();

public:
    bool m_bFirstTutorialPlaying;

};

DEFINE_SCENE_FACTORY( KGCUIScene );

extern KGCUIScene* g_pkUIScene;

#endif // _GCUISCENE_H_