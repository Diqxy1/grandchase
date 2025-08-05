#ifndef _PROCEDURE_H_
#define _PROCEDURE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "KNC/UserPacket.h"
////#include <vector>
extern std::wstring     g_strLoginError;
extern int              Result_Connect_LoginServer;
extern int              Result_Register_User;
extern DWORD            Wait_Time_LoginServer;
extern int              Result_Connect_GameServer;
extern int              Result_UserList;
extern int              Result_Create;
extern int              Result_Join;
extern int              Result_StartGame;
extern int              Result_EndGame;
extern int              Result_LeaveGame;
extern int              Result_LeaveRoom;
extern int              Result_Drop;
extern int              Result_Change_Room_UserInfo;
extern int              Result_Connect_Gild;
extern int              Result_Repair;
extern int              Result_Buy;
extern int              Result_Buy_CashItem;
extern int              Result_Buy_VirtualCashItem;
extern int              Result_ReportNickName;
extern int              Result_ReportPetNickName;
extern int              Result_Kid_Gift;
extern int              Result_SetCurrentChar;
extern int              Result_DeletePet;
extern int              Result_FeedingPet;
extern int              Result_CreatePet;
extern int              Result_ExtendInventory;
extern int              Result_TransformationPet;
extern int              Result_SeallingCalendar;
extern int              Result_WeeklyAttend;
extern int              Result_ForceTransition;
extern int              Result_GetRewardItem;
extern int              Result_RemovePointItem;
extern int              Result_ResetWinLoseData;
extern int              Result_EnchantItem;
extern int              Result_Bonus_Charging;
extern int              Result_Enter_Square;
extern int              Result_New_Character;
extern int              Result_New_Emoticon;
extern std::wstring     Result_Connect_Msg;
extern KEVENT_BAD_USER_INFO_NOT Result_BadUser_Info;
extern KEVENT_BAD_USER_INFO_CHANGE_BROAD Result_BadUser_Info_Room;
extern int              Result_Case;
extern KItem            g_kDurItemInfo;

extern bool             g_bAgreePrivateInfo;
extern int              g_TeamPoint;
extern UCHAR            g_ucGuildUserLevel;
extern int              MyGildTotalPoint;
extern int              MyGildTotalNum;
extern int              g_iResult_ShopAgreeMent;

extern int              Result_Join_From_Square;
extern int				Result_Join_Other_Channel;
extern int              Result_SellItemInfo;
extern int              Result_Sell_Count_Item;
extern int              Result_Start_MiniGame;
extern int              Result_Exit_MIniGame;
extern int              Result_End_MIniGame;
extern KMiniGameRankInfo    Result_MyMinigame_info;

extern int              Result_NateOn_Add_Group;
extern int              Result_NateOn_Rename_Group;
extern int              Result_NateOn_Delete_Group;
extern int              Result_NateOn_Move_Group;
extern int              Result_NateOn_Block_Buddy;
extern int              Result_NateOn_UnBlock_Buddy;
extern int              Result_NateOn_Delete_Buddy;
extern int              Result_NateOn_Change_Emoticon;
extern int              Result_Vip_Gacha_Item;

extern int              Result_Char_Poll;
extern int              Result_Loading_image;
extern int              Result_CharOpen;
extern D3DXVECTOR2		Result_Pos_MagicMissile;

extern std::vector<KUserInfo>           g_vecUserInfo;
extern bool                             g_bUserListUpdate;
extern bool                             g_bRoomListUpdate;
extern KDetailUserInfo                  g_kDetailUserInfo;
extern bool                             g_bDetailUserInfoUpdate;
extern KGuildInfo                       g_kGuildInfo;
extern bool                             g_bGuildInfoUpdate;
extern bool                             g_bUpdateTopRoom;
extern HWND                             g_hIMEWnd;

extern int				Result_Gacha_Reward_List;

extern int              Result_RecomEnable;
extern int              Result_RecommendNickName;
extern int              Result_recommendFullINfo;
extern int				Result_HalloweenBoardGame;
extern int				Result_BuyHalloweenDice;
extern int              Result_DonationPoint;
extern int									Result_BingoBoardGame;
extern int									Result_BingoChangeCoin;
extern int									Result_BingoRequestQuestion;
extern int									Result_BingoRequestAnswer;

extern int              Result_RegistMissionPackage;
extern int              Result_Buy_CooperationItem;
extern int              Result_CouponCode_Input;
extern int              Result_SocketManagement_Action;
extern int				Result_CanBuyCashItem;

extern bool			g_bViewMsgBox;
extern int									Result_SHAFileList; 
extern int              Result_HeroItemList;
extern int              Result_BuyForHero;
extern int              Result_ReqGuildStoreCatalog;
extern int              Result_BuyForGuildItem;
extern int              Result_BuyForLottery;

extern std::set<std::wstring>				g_setSHAFileList;	

extern DWORD			g_dwLoadingTimeCheck;
extern int				Result_GawibawiboGameInfo;
extern int				Result_GawibawiboGameGiveUp;
extern int				Result_Random_Init_attribute;

extern int              Result_PlantTreeInfo;
extern int				Result_GuildRankingInfo_Req;

extern int              Result_SelectAttribute;
extern int				Result_SphinxReq;
extern int              Result_GwcRakinglistReq;
extern int              Result_SockTreeReq;
extern int              Result_GwcRakingRewardReq;

extern int              Result_AgitShopCatalog;
extern int              Result_BuyAgitShop; 
extern int              Result_AgitLoadComplete;
extern int              Result_LeaveAgit;
extern int              Result_AgitGuestbookPage;
extern int              Result_EhcnatSystem;

extern int              Result_UseTrainingObject;

extern int              Result_ReceiveChristmasReward;
extern int              Result_PostLetter;
extern int              Result_ReceiveEclipsePlotReward;

extern int              Result_EnterAgitReq;
extern int              Result_EclipseEvent;
extern int              Result_DepotInfoByCharTabReq;
extern int              Result_DepotItemReq;
extern int              Result_DepotUpdateItemReq;
extern int              Result_GcAdventureEvent;
extern int              Result_ContinentRewardReq;
extern int              Result_WholeContinentFinalRewardReq;

extern int              Result_OlympicEven_Data;
extern int              Result_ItemEquip;
extern int              Result_CoordiEquip;
extern int              Result_InventoryRecvData;
extern int              Result_SkillSet;
extern int              Result_Item_Combination;
extern int              Result_MatchPartyUserList;
extern int              Result_MatchPartyReady;
extern int              Result_AddMatchReq;
extern int              Result_StartGamePossibleReq;

extern int              Result_ReceiveChristmasUserInfo;
extern int              Result_ReceiveChristmasPlayInfo;
extern int              Result_ReceiveChristmasRankInfo;

extern int              Result_ReceiveChristmasStageStart;
extern int				Result_ReceiveChristmasBasketEmpty;
extern int				Result_ReceiveChristmasStageClear;
extern int				Result_ReceiveChristmasGetReward;
extern int				Result_ReceiveChristmasExchangeContinueCoin;
extern int				Result_ReceiveChristmasRewardItemList;

extern int              Result_ReceiveAttendanceStampInfo;
extern int              Result_ReceiveAttendanceStampReward;
extern int              Result_ReceiveBuffDonatingReq;
extern int              Result_ReceivePackageInfo;
extern int              Result_ReceivePackageDetailInfo;

extern int              Result_ItemTrade;
extern int              Result_ReceiveErrandReward;
extern int              Result_ReceiveErrandEnd;

extern int              Result_ReceiveGuideCompleteReq;

extern int              Result_AttributeMigration;
extern int              Result_ChangeChar;
extern int              Result_CYOU_WebCash;

extern int              Result_ReceiveVitalitySystemRecharge;
extern int              Result_ClientContentsAdditionInfo;

extern int              Result_Damage_Life;
extern int              Result_VirtualDepotItemListMoveInven;

extern int              Result_Kairo_Oneshot;
extern int              Result_CharNickChange;
extern int				Result_ReceiveSocialCommerceInfo;
extern int              Result_ReceiveCoordiUpgradeInfo;

extern int              Result_ReceiveSkillScroolITemReq;
extern int				Result_GachaPongReq;

extern int				Result_ReceiveInRoomIndoorChangeReq;

extern int				Result_ReceiveCoordiviewSlotGetReq;
extern int				Result_ReceiveCoordiviewSlotSetReq;

extern int				Result_ReceivePetMagicStoneChangeEquipReq;

extern int				Result_ReceiveUseHeroTicketReq;

#endif // _PROCEDURE_H_