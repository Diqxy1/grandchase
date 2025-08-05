//code:#END_OF_IMPL_FUNC
#pragma once

#include "GCUTIL/KJohnGen.h"
#include "KNC/UserProxy.h"
#include <KNCSingleton.h>
#include "KNC/TRUser.h"
#include "KGCMsgServer.h"
#include <MMSystem.h>

#ifdef _OPERATOR_TOOL_
//#include <string>
//#include <map>
#else
#include "KNC/NUserProxy.h"
#endif
#include "Packet.h"
#include "KNC/MSProxy.h"
#include "PrimitiveTypedef.h"

class KP2P
{
    DeclareSingleton( KP2P );
public:
    enum
    {

        USERPROXY_ID    = 1,
        NUSERPROXY_ID   = 2,
        TRUSERPROXY_ID  = 3,
        MSPROXY_ID      = 4,
    };

    enum EGCSendPacketTimer
    {
        ESPTServerList = 0,
        ESPTChannelList,
        ESPTRoomList,
        ESPTUserList,
		ESPTFriendList,
		ESPTUpdateRoomList,
        ESPT_NUM,
    };

public:
    KP2P() : m_pkUserProxy( NULL ) 
    {
        m_kMsgServer = NULL;
        Reset();
    }  // inline으로 구현. 운영자툴에서 pch 사용여부 설정때문에 링크되지 않는 문제 수정.
    virtual ~KP2P() {
        SAFE_DELETE( m_kMsgServer );
        SAFE_DELETE( m_pkUserProxy ); 
    }

    bool Init( HWND hWnd );
    void Reset();
    bool IsConnectedToGameServer();

    bool Send_CreateRoomReq( const KRoomInfo& kRoomInfo, const KInDoorUserInfo& kInDoorUserInfo );
    bool Send_CreateRoomInSquareReq( DWORD dwChannelUID, const KRoomInfo& kRoomInfo, const KInDoorUserInfo& kInDoorUserInfo );
    // godsend p2p상의( udp로 구현) 원할한 접속을 위해서 ip_vector 추가 [4/14/2005]
    //bool Send_JoinRoomReq( const unsigned int uiJoinType, USHORT usRoomID, const wchar_t* szRoomPasswd,
    //                       const KInDoorUserInfo& kInDoorUserInfo, std::vector<USHORT> vecKickedRoomID,
    //                       char cQuickJoinCategory = GC_GMC_INVALID, int iQuickJoinMode = GC_GM_INVALID,
    //                       int nDifficult = GC_SGM_DUNGEON_NORMAL,
    //                       bool bGuild_ = false );
	bool Send_JoinRoomReq( const unsigned int uiJoinType, USHORT usRoomID, const wchar_t* szRoomPasswd, const KInDoorUserInfo& kInDoorUserInfo, std::vector<USHORT> vecKickedRoomID, char cQuickJoinCategory /*= GC_GMC_INVALID*/, int iQuickJoinMode /*= GC_GM_INVALID*/, int nDifficult /*= GC_SGM_QUEST_LEVEL1*/, bool bGuild_, std::map<DWORD,DWORD> mapJoinOption);
	//bool Send_QuickJoinReq( const std::vector<DWORD>& vecIP, const std::vector<USHORT>& vecPort);

    // godsend  [6/4/2004]
    //bool Send_JoinRoomWithGuildUserIDReq( const wchar_t* szMessage,
    //                                      const std::vector<DWORD>& vecIP,
    //                                      const std::vector<USHORT>& vecPort );

    bool Send_BillboardChatRegistrationReq( const std::wstring& strMessage, D3DCOLOR color );
    bool Send_ChatReq(const wchar_t* szPeerName_, const wchar_t* szMessage_, KChatData::ENUM_CHAT_TYPE eChatType_, const DWORD dwColor_ = 0xff000000);
    bool Send_WhisperReq( const wchar_t* szPeerName_, const wchar_t* szToID, const wchar_t* szMessage_ );
    bool Send_LeaveRoomReq( const KEVENT_LEAVE_ROOM_REQ& kPacket );
    bool Send_RoomListReq( const bool bSendDelay, const bool bWait, const int nType, const int nDifficult, const int nPage_ );
    bool Send_UserListReq(const int nPage );
	bool Send_FriendInviteListReq();
    bool Send_StartGameReq( const std::vector< std::pair<char,char> >& vecPromotionCount,  const int iGameMode = 0 , const char cSelChar = 0, int iMapID = 0, int nStartStage = -1 );
    bool Send_EndGameReq(const char* szPeerName, KEndGameReq& kReq);
    // godsend p2p상의( udp로 구현) 원할한 접속을 위해서 ip_vector 추가 [4/14/2005]
    bool Send_ChangeHostNot( const std::vector<DWORD>& vecHostIP, const std::vector<USHORT>& vecHostPort );
    bool Send_IncMaxPlayersReq(const char* szPeerName);
    bool Send_DecMaxPlayersReq(const char* szPeerName);

    bool Send_ChannelInfoListReq( const bool bDelayCheck = true );
    
    bool Send_EnterChannelReq(const wchar_t* szPeerName, DWORD dwChannelUID_ );
    bool Send_LeaveChannelNot(const char* szPeerName);
    bool Send_LoadCompleteNot( const int iP2PUser, const int iRelayUser );
    bool Send_StageLoadCompleteNot();
    bool Send_NotifyMessageNot(const wchar_t* szPeerName, const std::wstring& strMessage);

    bool Send_BuyItemForGPReq( IN DWORD dwGoodsID, IN int iDuration, IN int iPeriod = KItem::UNLIMITED_ITEM );
    bool Send_BuyItemForVirtualCashReq( IN const GCITEMID itemID, IN const GCITEMUID itemUID, IN const int nDuration, IN const int nPeriod );
    bool Send_DropItemReq(const char* szPeerName, GCITEMUID dwGoodsUID, DWORD dwGoodsID );
    bool Send_EquipItemReq( const KEquipUser& kReq);
    bool Send_InviteReq( std::vector<DWORD>& kReq);
    bool Send_ChangeRoomInfoReq( std::vector<std::pair<char,bool> >&  vecChangeSlot_ );
    bool Send_ChangeRoomInfoReq( IN SRoomInfo* pkRoomInfo_ );

    // godsend  [6/4/2004]
    bool Send_EnterGuildRoomReq(const char* szPeerName);
    bool Send_LeaveGuildRoomNot(const char* szPeerName);
    bool Send_GuildChatReq(const wchar_t* szPeerName, const std::wstring& strMessage);
    bool Send_GuildUserListReq(const char* szPeerName);
    bool Send_GuildNotifyMsgReq(const wchar_t* szPeerName, const std::wstring& strMsg);
    bool Send_GuildEditNoticeReq(const char* szPeerName, KGuildNoticeInfo& kNotice);
    bool Send_KickUserReq(const wchar_t* szPeerName, const std::wstring& strLogin);
    bool Send_ChangeUserDBInfoReq(const char* szPeerName, KUserInfoForOperator& kUserDBInfo);
    bool Send_InventoryInfoReq(const char* szPeerName);
    bool Send_UserDBInfoReq(const wchar_t* szPeerName, const std::wstring& strLogin, bool bIsNickname = false);
    bool Send_DropUserDBInfoReq(const wchar_t* szPeerName, const std::wstring& strLogin);
    bool Send_EchoNot(const char* szPeerName);
    // godsend 수량성 아이템 사용패킷 보내기 [6/29/2004]
    bool Send_UseDurationItemReq( const std::vector<GCITEMUID>& vecDurationUids );

    // godsend 캐릭터 3명중 가장 레벨 높은 캐릭터의 장작 아이템과 성별 요청하는 패킷 [7/15/2004]
    bool Send_BestCharacterEquipItemReq(const char* szPeerName, std::string struserName );

    // godsend 대회용으로 같은 편이지 확인 , 승정 업데이트 요구하는것  [7/22/2004]
    bool Send_UpdateTeamPointReq(const char* szPeerName, std::vector<std::string> vecUserName );
    bool Send_VerifySameTeamReq( const char* szPeerName, std::vector<std::string> vecFirst, std::vector<std::string> vecSecond );

    // godsend 추석 이벤트를 위한 송편5개 모으면 한복주는 것  [9/9/2004]
    bool Send_ChuseokEventItemReq( const char* szPeerName, DWORD dwGoodsID );

    // godsend 강화요청 패킷 [10/1/2004]
    bool Send_StrongLevelUpReq( IN GCITEMID itemID_, IN GCITEMUID itemUID_, IN GCITEMID assistItemID_, IN GCITEMID materialItemID_ = 0 );

    //====================================================================================
    // 2007-10-16 오전 10:21:31 : Asirion 
    // 목걸이 강화가 아닌 장비 소켓 아이템 강화 이다.
    bool Send_EnchantItemReq( IN const DWORD dwUserUID, IN const GCITEMID itemID, IN const GCITEMUID itemUID, IN const GCITEMID assistItemID, IN const GCITEMUID assistItemUID, 
                              IN const GCITEMID stuffItemID, IN const GCITEMUID stuffItemUID, IN const DWORD dwStuffItemCount );

    // godsend Gem으로 아이템 구매 패킷 [10/6/2004]
    bool Send_BuyItemForGemReq( IN GCITEMID itemID, IN int iDuration, IN int iPeriod );

    // godsend 깨어진 강화 아이템을 소생시킨다. [10/21/2004]
    bool Send_RevivalBreakItemReq( const char* szPeername_, DWORD dwGoodsID_, GCITEMUID dwGoodsUID_ );

    // godsend 크리스탈로 구매할수있는 아이템 요구 패킷 [12/11/2004]
    bool Send_BuyItemForCrystalReq( GCITEMID itemID, int iDuration, int iPeriod, int iMoneyType_ );

    // godsend 생방송 (비비빅) 짱 유저 변경 요청 패킷 [1/5/2005] 
    bool Send_ChangeVivivicZzangUserReq( const char* szPeerName_, std::string strUserName_ );

    // godsend 생방송 비비빅 짱 아이템을 넣고 짱유저 SET한다. [1/10/2005]
    bool Send_SetVivivicZzangUserReq( const char* szPeerName_, std::string    strUserName_ );

    //godsend 운영자 툴 서버(DB) 일반 아이템 가져오기 
    bool Send_SelectNormalItemReq( const wchar_t* szPeerName_, std::wstring strName_ );
    bool Send_SelectDurationItemReq( const wchar_t* szPeerName_, std::wstring strName_ );
    bool Send_SelectDelNormalItemReq( const wchar_t* szPeerName_, std::wstring strName_ );
    bool Send_SelectDelDurationItemReq( const wchar_t* szPeerName_, std::wstring strName_ );
    bool Send_InsertGPNormalItemReq( const wchar_t* szPeerName_, std::wstring strName_, DWORD dwGoodsID_ );
    bool Send_InsertGPDurationItemReq( const wchar_t* szPeerName_, std::wstring strName_, DWORD dwGoodsID_ );
    bool Send_InsertCashNormalItemReq( const wchar_t* szPeerName_, std::wstring strName_, DWORD dwGoodsID_ );
    bool Send_InsertCashDurationItemReq( const wchar_t* szPeerName_, std::wstring strName_, DWORD dwGoodsID_ );
    bool Send_DelItemReq( GCITEMUID dwGoodsUID_ );
    bool Send_SelectUserDataReq( const wchar_t* szPeerName_, std::wstring strName_ );
    bool Send_SelectChangeStrongItemListReq( const wchar_t* szPeerName_, std::wstring strName_ );
    bool Send_SelectAdminUseItemInfoDataReq( const char* szPeerName_ );

    //simon [2005-03-29] 정보 공개 허락한다고 정보를 보낸다.
    bool Send_AgreePrivateInfoNot(const char* szPeerName_);
    //simon [2005-04-25] 어린이날 풍선 터뜨렸을 때 패킷 날리는 함수.
    bool Send_OpenKidDayGiftReq( const char* szPeerName_ );

    //bool Send_MajorDataCheckNot( std::vector<str_UserData>& userData, str_RoomData& roomData );
    bool Send_P2PSocketErrorNot( IN const std::wstring& strName );    // 연결이 끊겼다고 판단한 유저의 이름.
    bool Send_BanUserReq( IN const std::wstring& strTargetUserName, IN int nReasonWhy );
    //bool Send_JoinCompeleteReq(); // 060711. kkurrung.
    bool Send_RegisterNickNameReq( const std::wstring& strNickName );
    //bool Send_UseLasCardReq( IN const std::wstring& strName_, IN const DWORD dwCardUID_, IN const DWORD dwCardID_ ); // 050822. microcat.
    bool Send_ClientCrashedNot();
    bool Send_DetailUserInfoReq( IN const std::wstring& strLogin ); // 포털아이디. 닉네임 아님.

    // 060313. kkurrung.
    bool Send_ClientError( IN const int iErrorType, IN const int iErrorID, IN const std::wstring strErrComment = L"", IN DWORD dwReserve1 = 0, IN DWORD dwReserve2 = 0, IN DWORD dwReserve3 = 0 );
    bool Send_ClientErrorCount( const std::map< std::pair<int,int>, int >& mapErrorCount );

    bool Send_ChangeNickName( IN const GCITEMID itemID, IN const GCITEMUID itemUID, IN const wchar_t* szNewNickName, IN bool bUserItem_ = true, IN char cCharType_ = -1 );
    bool Send_ChangeNickColor(IN const GCITEMID itemID, IN const GCITEMUID itemUID, IN const wchar_t* szNewNickColor, IN bool bUserItem_ = true, IN char cCharType_ = -1);
    bool Send_ChangePetNickName( IN const KItem& NickNameItem, IN const DWORD dwPetID, IN const GCITEMUID dwPetUID, IN const wchar_t* szNewNickName );

    bool Send_RegistMission( IN const GCITEMID itemID, IN const GCITEMUID itemUID );
    bool Send_RemoveMission( IN const DWORD dwMissionID );
    bool Send_MissionComplete( IN const DWORD dwMissionID , IN const GCITEMID dwSelection = 0 );
    //bool Send_Change_Job( IN const DWORD dwGoodsID, IN const DWORD dwGoodsUID, IN const char cCharType, IN const char cPromotionLevel );
    bool Send_LeaveGameReq( const KGameResultIn& kPacket_ );

    bool Send_CurrentCashPoint(); // No Param
    bool Send_BuyForCashItem( IN const DWORD dwGoodsID, IN const int nCount_, int nPeriod_, DWORD dwVirtualCash, bool bPresent, 
        std::wstring strOwnName, std::wstring strComment, IN const DWORD m_dwEventUID = 0 );
    bool Send_RecommendUserReq( IN int iOK_, IN std::wstring strNickName_ );
    bool Send_ChangeRoomUserInfoReq( IN SUserInfo& kUserInfo_, 
                                     IN const KChangeRoomUserInfo::ROOM_USER_INFO eRoomUserInfo_, int iPlayerIndex_ );

    bool Send_ChangeRoomUserInfoReq( IN const KChangeRoomUserInfo& kUserInfo_ );
    bool Send_ChangeIndoorUserInfoReq( IN SUserInfo* pkUserInfo_ );
    bool Send_InRoomChangeIndoorUserInfoReq( IN SUserInfo* pkUserInfo_ );
    bool Send_Into_Myinfo( IN DWORD dwUID, IN std::wstring strLogin_, IN int nState_ );

    bool Send_EndResultNot();
    bool Send_CheckRoomUserlistNot( IN std::vector<DWORD>& vecPeerUID_, IN std::vector<DWORD>& vecPlayerUID_ );

    bool Send_ChangeItemReq( IN const GCITEMUID itemUID );
    bool Send_ChangeItemOneShotReq( IN const int ItemCount_, IN const GCITEMUID itemUID_ );
    bool Send_SetCurrentCharacter( IN char cCharacter );
        
    //펫 관련
    //bool Send_BindingPetReq( IN const KEVENT_BINDING_PET_REQ kPacket );
    bool Send_CreatePetReq( IN const char cCharType, IN const DWORD dwPetID, IN const GCITEMUID dwPetUID, IN const std::wstring strPetName );
    //bool Send_DelPetReq( IN const GCITEMUID dwPetUID );
    bool Send_DelPetReq( IN const GCITEMUID dwPetUID, IN const char cCharType_ = -1 );
    //bool Send_PetEquipReq( IN const DWORD dwPetID, IN const DWORD dwPetUID, IN const std::vector<KSimpleItem>& vecPetEquipItem );
    bool Send_FeedingPetReq( IN const GCITEMUID dwPetUID, IN const GCITEMUID itemUID );

    // 2007.02.02 wony
    // 평균 FPS 리포트
    bool Send_AveFPSReport( IN int iAveFPS );

    // 070314. kkurrung. 유저 강제 선행 플레이(모드 ID를 넣으세요.)
    bool Send_PreCompleteReq( IN const int nModeID );
    // 070314. kkurrung. 인벤 늘리는 아이템
    bool Send_InventoryExtendReq( IN const GCITEMID itemID, IN const GCITEMUID itemUID );
    // 070315. kkurrung. 발자취
    bool Send_StatUserHistory( IN std::vector< std::pair<char,unsigned short> >& vecUserHistory );

    // 2007/3/20. iridology. 프로텍트테이블 체크섬
    bool Send_ProtectTableChkSum( IN DWORD dwCheckSum );

    // 2007/3/29. iridology. 방장이 강제로 방원들의 속성 변경
    bool Send_ForceChangeRoomUserInfo();    // No Param

    // 2007.04.17. wony. 펫진화관련
    bool Send_CheckTransformationPetReq( GCITEMUID dwPetUID, DWORD dwItemID, int iPetTransformationType );
    bool Send_TransformationPetReq( GCITEMUID dwPetUID, std::wstring strPetName, DWORD dwUseItemID, int iPetTransformationType );

    // 2007.04.23 wony 출석부
    bool Send_GetCalendarReq();
    // 070424 woosh 출석부, GCPoint
    bool Send_MonthlyAttendRewardReq( IN const KSimpleDate& kReqData, IN const KItem& kReqItem ); //월개근 보상 아이템 요청
    bool Send_GCPointRecommendReq( std::wstring strNickName ); // 친구 추천
    bool Send_WeeklyAttendReq( IN const char cWeekNum, IN const GCITEMID nReqItemID = 0 ); // 주개근 요청
    bool Send_SeallingCalendarReq( KSimpleDate kReqData ); // 출석 도장 찍기 요청
    bool Send_GetCurrentPointReq(); // GC 포인트 정보 요청 

    // 070424. kkurrung. 알부화
    bool Send_HatchingEggReq( const GCITEMUID dwEggPetUID );// 알펫의 UID를 넣어 주세요.. 서버에서 사용하는 UID
    // 070419. woosh. 강제 상태 변경
    bool Send_BuyForGCPoint(IN const GCITEMID itemID, IN int nCount = 1, IN int nMoneyType = KBuySellItemReq::EMT_GCPOINT);
    bool Send_SellForGCPoint( IN const GCITEMID itemID, IN const GCITEMUID itemUID );
    bool Send_ForceTransitionState( IN int nDestState ); //원하는 상태를 넣어서 보내기
    //  FH
    bool Send_FH_Check_Recommend();
    bool Send_FH_Recommend_Info(); // 추천 받은 정보 퍼오기..
    bool Send_FH_Recommend_ItemReq( const char chID );

    bool Send_Stat_FinishLodingTime( const DWORD dwElapsedTime );
    // 승률 초기화 아이템
    bool Send_Reset_Win_Lose_Data( IN const GCITEMID itemID );

    // 06/21/2007. daeuk. 방제 변경
    bool Send_Change_Room_Option_Req( const KRoomOption& kRoomOption );

    // 070817. woosh. 불량 유저 신고
    bool Send_Report_User_Req( const int nReason, const std::vector<DWORD>& vecUser , const std::wstring& strNickName_ );

    // 070914. kkurrung. 채팅 이벤트.. ACK 없음.
    bool Send_Chatting_Event_Correct_Not( const int nEventUID );

    bool Send_ServerListReq();
    bool Send_ChangBonusCoinReq( DWORD dwItemId, GCITEMUID dwItemUID, int nCount );
    bool Send_UseBonusPoint();

    // 퀵슬롯
    bool Send_ChangeSlotEquipReq( KQuickSlot& kSlotInfo );
    bool Send_UseQuickSlotReq( IN const int nSlotType, IN const int SlotIndex, IN const GCITEMID dwItemID, IN const GCITEMUID dwItemUID, IN const int itemKind );

    // 광장....
    bool Send_SquareListReq( IN DWORD dwGuildUID = 0 );
    bool Send_EnterSquareReq( const char cCharType, const DWORD dwSquareID, const int iSquareType_ = -1, const int iSquareIndex_ = -1);
    bool Send_SquareLodingComplete(); // No Param
    bool Send_LeaveSquareReq(); // No Param
    bool Send_UpdateSquareMyPos( DWORD dwSquareID_, float fXpos, float fYpos );
    bool Send_SquareChat( char cChatType, DWORD ReceiverUID, std::wstring strMsg , DWORD dwChatColor_ );
    bool Send_SyncNeighborhood();

    // 캐릭터 카드
    bool Send_NewCharacterReq( IN const GCITEMID itemID, IN const GCITEMUID itemUID );
    bool Send_TWUserAgreementReq( const int nUserAgreement );

	// 광장내 이모티콘
    bool Send_UseEmoticon_Req( const DWORD dwEmoticonItemID );

	bool Send_SetObserver( const bool bObserver );	// true : Set Observer, false : Release Observer

	// 이모티콘 조합
	bool Send_EmoticonMixReq( const GCITEMID dwItemA, const GCITEMID dwItemB, const GCITEMID dwItemC );

    // 룸에서의 초대.
    bool Send_InvitedRoomFromSquareReq( const DWORD dwChannelUID, const USHORT usRoomID,
                                     const DWORD dwHostUID, const std::wstring& strRoomPW, const KInDoorUserInfo& kInfo );
    bool Send_SquareUserListReq( const DWORD dwSquareID, const int nPage );
    bool Send_Invite_Deny_Not( bool bInviteDiny );
    bool Send_GetSignBoardRegNum(); // 현재 등록된 전광판 개수 얻어 오기.
    bool Send_RegSignBoard( const KSignBoardData& kData );
    // nType 는 다음 두개 중 하나를 선택해주세요. KSignBoardData::SBT_SERVER_ADMIN, KSignBoardData::SBT_SQUARE_ADMIN
    bool Send_AdminSignBoardNot( const int nType, const std::wstring& strMsg );

    // 랭킹 관련
    bool Send_RankPageReq( const char cRankType, const char cCharType, const char cPageNum );
    bool Send_RankSearchReq( const char cRankType, const char cCharType, const std::wstring& strNick );

    // 게임 시작시 p2p 연결 장애 해결
    bool Send_P2PConnectionCompleteNot();
    bool Send_P2PStandbyListAck( const std::set<DWORD>& setList ); // client to server

    // 수량 아이템 구매 관련
    bool Send_GetSellPrice( const DWORD dwItemID );
    bool Send_SellCountItemReq( const DWORD dwItemID, const GCITEMUID dwItemUID, const int nSellCount );

    // 칭호
    bool Send_CollectionComplete( const DWORD dwMissionID );
	// 데스매치
	bool Send_JoinGameReq();

    bool Send_P2PUniqueNumberReq();

    bool Send_UpdateDeathScore( const int nCurrentKill, const int nCurrentTime );
    bool Send_RelayData( const std::vector<DWORD>& vecUsers );

    bool Send_StartMiniGameReq( const char cGameMode );
    bool Send_EndMiniGameReq( const KMiniGameRankInfo& kData );    
    bool Send_ExitMiniGameReq(); // 미니게임에서 광장으로 돌아가기
    bool Send_RestartMiniGame( const char cGameMode ); // 미니게임 메뉴에서 미니게임 시작
    bool Send_MiniGameTopRank( const char cGameMode ); // 미니게임 종류별 top 랭크 요청
    bool Send_MiniGameMyRank(); // 유저의 미니 게임 랭킹

	// 스킬 관련
    bool Send_GetFullSkillInfo();
    bool Send_SkillTrainingReq( const int nSkillID );
    bool Send_SettingSkills( const KChangeSkillSet& kChangeSet );
    bool Send_RemoveSkill( const int nSkillID, const DWORD dwItemID );
    bool Send_UnLockSkill( const int nLockGroupID );
    bool Send_SkillOpenUseReq( const int nSkillID );

    bool Send_StartJumpEvent();     // 운영자가 보내는 이벤트 시작 패킷
    bool Send_FinishJumpEvent();    // 운영자가 보내는 이벤트 종료 패킷
    bool Send_ArriveGoal( const std::wstring strMsg );

    // 쿠폰 관련.
    bool Send_ChangeCouponReq( const GCITEMID dwItemID, const GCITEMUID dwItemUID, const char cItemType, const int iAmmount );

    bool Send_GachaRewardListReq( IN const int iVer, IN const GCITEMID itemID );
    bool Send_GachaActionReq( IN const GCITEMID itemID_, IN const int iLevel_, IN const DWORD dwType_, IN const char cCharType_, IN const int iVer_ );
    bool Send_GachaLevelActionReq( IN const GCITEMID itemID_, IN const int iLevel_, IN const DWORD dwType_, IN const char cCharType_, IN const int iVer_ ); // 가차 레벨, 용도는 Send_GachaActionReq 와 동일
    bool Send_ObtainedGachaSetRewardList(); // 이미 획득한 가차 세트 보상 리스트. (구슬 아이디)
    bool Send_ObtainedGachaLevelSetRewardList(); // 가차 레벨용 획득한 가차 세트 보상 리스트
    bool Send_GachaSetRewardListReq( IN const int iVer, IN const GCITEMID itemID ); // 구슬 아이디 너으면 됨.
    bool Send_GachaSetRewardReq( DWORD dwGachaItemID_, DWORD dwType_, int iLevel_, DWORD dwRewardItemID_, const char cCharType_, const int iVer_ ); // 구슬 아이디 넣음 됨..
    bool Send_GachaLevelSetRewardReq( DWORD dwGachaItemID_, DWORD dwType_, int iLevel_, DWORD dwRewardItemID_, const char cCharType_, const int iver_ ); // 구슬 아이디 넣음 됨.. 가차 레벨용
    
	bool KP2P::Send_ClientFailedGameStartNot( DWORD nFailedType_, int nSecond_ =0, DWORD dwRoomTotalUser_=0, 
		DWORD dwMapID_ =0,int nFailType_ =0, DWORD dwFailReason_=0); // 실패한 타입 넣으면 됨.

  // 커플
    //EVENT_MAKE_COUPLE_REQ
    bool Send_MakeCoupleReq( KEVENT_MAKE_COUPLE_REQ& kData );
    //EVENT_MAKE_COUPLE_ANS
    bool Send_MakeCoupleAns( bool bAccept );
    //EVENT_USE_COUPLE_ITEM_REQ
    bool Send_InsertCoupleInven( DWORD dwItemID, GCITEMUID dwItemUID, bool bDirectEquip );
    //EVENT_EQUIP_COUPLE_ITEM_REQ
    bool Send_ChangeCoupleEquip( std::vector<DWORD>& vecEquip );
    //EVENT_COUPLE_INFORMATION_REQ
    bool Send_CoupleInfoReq( DWORD dwUserUID );
    //EVENT_SET_COUPLE_REQ_DENY
    bool Send_MakeCoupleReqDeny( DWORD dwUserUID ); // 0 이면 전체 거절
    //EVENT_CHANGE_COUPLE_RING_REQ
    bool Send_ChangeCoupleRingReq( DWORD dwItemID, GCITEMUID dwItemUID, std::wstring& strMsg, bool bInUserInven );    
    //EVENT_CHANGE_COUPLE_RING_ANS
    bool Send_ChangeCoupleRingAns( bool bAccept );
    //EVENT_DIVORCE_COUPLE_REQ
    bool Send_DivorceCoupleReq( std::wstring& strMsg );
    //EVENT_CHANGE_COUPLE_CHAR_REQ
    bool Send_ChangeCoupleCharType( const char cCharType );

    bool Send_CoupleChatReq( const std::wstring strMsg, const DWORD dwColor );
    bool Send_CoupleWindowStateReq( const int nState );

    //EVENT_PET_COSTUM_LIST_REQ
    bool Send_PetCostumeListReq();

	//EVENT_GET_USER_DONATION_INFO_REQ
	bool Send_MyDonationInfoReq();
	//EVENT_DONATION_INFO_REQ
	bool Send_DonationInfoReq();
	//EVENT_DONATION_GP_REQ
	bool Send_DonationPointReq( const int nDonationPoint_ );
    bool Send_AddExp( const char cCharType, const __int64 biAddExp );
	bool Send_LunaNewYearReq();
    //bool Send_CheckPollTerm();
    //bool Send_CharPollReq( IN const char cCharType, IN const GCITEMID dwItemID, IN const int nUseCount );

	//EVENT_BUY_FOR_VIP_REQ
	bool Send_BuyForVIPReq( const int nCharType_, const DWORD dwItemID_, const int nDuration, const int nPeriod, const int nVipCount_ );

    // 추천
    //EVENT_CHECK_RECOMMEND_ENABLE_REQ
    bool Send_CheckRecomEnableReq();
    //EVENT_RECOMMEND_USER_REQ
    bool Send_RecommendUserReq( const std::wstring& strNick, const int nState ); //  bState 추천 할지 : 0, 추천 않함 : 1, 나중에 추천 할지 : 3.
    //EVENT_RECOMMEND_FULL_INFO_REQ
    bool Send_RecommendFullInfoReq();
    //EVENT_RECOMMENDER_ACCEPT_REQ
    bool Send_AcceptRecommenderReq( const std::vector<DWORD>& vecRecommenders ); // 나를추천한 사람 중에 승인할 애들..
    //EVENT_ATTENDANCE_BONUS_REQ
    bool Send_AttendBonusReq( const int nState ); // 3, 6, 9, 12
    //EVENT_RECOMMENDER_DENY_REQ
    bool Send_RecommenderDenyReq( const std::vector<DWORD>& vecRecommenders );

	// GP캡슐
	bool Send_OpenCapsuleReq( const int nCharType, const int nItemKind, const int nIndex, const int nPrice );
	bool Send_OnlineFriendList(std::vector<DWORD> vecOnlineFriendList);
	// 게임쿠폰NPC
	bool Send_GameCouponListReq(); // 게임쿠폰정보 리스트 요청
    bool Send_UseGameCouponReq( const int nEventUID, const std::wstring& strCouponID, const std::wstring& strCouponPW ); // 게임쿠폰 사용하기
	bool Send_GameCouponFailNot(); // C->S 쿠폰사용 실패 알림(3회 연속 입력 실패시 보냄)

    // EVENT_SET_EXP_REQ
    bool Send_SetExp( const char cCharType, const __int64 biAddExp );

    // 가상캐쉬쿠폰
    bool Send_ChangeVirtualCashCouponReq( const GCITEMID dwItemID, const GCITEMUID dwItemUID, const char cItemType, const int iAmmount );

    // 미션 패키지 등록
    bool Send_MissionPackage( const DWORD dwItemID_, const GCITEMUID dwItemUID_, const char cItemType_ );

	//EVENT_USER_PING_INFO
	bool Send_StatPingInfo( const std::vector<KStatPingInfo>& vecStatPingInfo_ );

    // RainBow
    bool Send_JackPotItemReq();
    // 구매 가능/불가능 아이템 리스트 얻기..
    bool Send_ItemBuyEnableReq(); //EVENT_ITEM_BUY_ENABLE_REQ
    bool Send_EffectTextureReport( const KEVENT_EFFECT_TEX_REPORT& kData );

	// 일일 퀘스트 지급.
	bool Send_DateChangeReq();

    bool Send_FrientInviteListReq();//EVENT_INVITE_FRIEND_LIST_REQ

	bool Send_ItemTradeReq(DWORD m_dwItemID_, int m_dwItemCount_, int iOption_ );
	// Join From Orther Channel
	bool Send_JoinRoomFromChannelReq( const DWORD dwChannelUID, const USHORT usRoomID,
		const DWORD dwHostUID, const std::wstring& strRoomPW, const KInDoorUserInfo& kInfo );

    //제휴 이벤트 EVENT_CO_OP_ITEM_BUY_REQ
    bool Send_CoOpItemBuyReq( const int nSlotID_ );
    // 광장 NPC클릭시 아이템받기 요청
    bool Send_NpcGiftReq( const int nEventID_ );
	// EVENT_CURRENT_SUBSCRIPTION_GIFT_NUM_REQ
	bool Send_CurrentSubscriptionGiftNumReq();
	//EVENT_SUBSCRIPT_REQ
	bool Send_SubscriptReq( const int nEventUID, const DWORD dwTicketNum_ );
    // 광고노출횟수
    bool Send_AdvertisingExposureCountNot( const int nType_, int nCount = 1 );
	// 유저가 선택한 캐릭터/전직의 스텟 정보 요청
	bool Send_CharacterStatInfoReq( const std::pair<char,char> prCharInfo_ );
	// 해킹 감지시 서버에 알림
	bool Send_ClientHackingUserNot( const int nHackType_ );
	
	bool Send_WebGachaCoinTrade(int dwItemID_, int dwItemCount_ );

    // 할로윈
    bool Send_BoardGameInfoReq();
    bool Send_DicePlayReq();
    bool Send_DiceItemTradeReq( DWORD dwItemID_ );
    bool Send_DiceItemBuyCheckReq( DWORD dwItemID_);

	// 기부 이벤트 
    bool Send_DonationGrade(const DWORD dwGrade);

	//bool Send_GWCEventInfoReq();
	//bool Send_GWCDonationReq( const int nCount );
	//bool Send_GWCEventResultReq();

    // New Guild
    bool Send_SearchGuildListReq( const UCHAR ucSortType_, const std::wstring strKeyword_ = L"", const UCHAR ucOrderBy_ = KEVENT_SEARCH_GUILD_LIST_REQ::OB_ASC ); // C->S. 최초 길드목록창 열때/검색버튼 누를때마다/정렬기준 바꿀마다
    bool Send_GuildListReq( const UCHAR ucSortType_, const std::wstring strKeyword_, const int iPageNum_, const UCHAR ucOrderBy_ = KEVENT_SEARCH_GUILD_LIST_REQ::OB_ASC ); // c->s. 길드목록 요청 (페이지 단위)
    //bool Send_InviteGuildReq(); // c->s. 내 길드에 초대(대상이 접속중이고,UserUID를 알수 있는 상황만 한정)(ex.방,광장,로비채팅창 등) (차후구현)
    bool Send_JoinGuildReq( const int iGuildID_, const std::wstring strComment_ ); // c->s. 길드 가입 신청
    bool Send_CancelJoinGuildReq( const int iGuildID_ ); // C->S. 길드 가입 취소 요청
    bool Send_CreateGuildReq( const std::wstring strGuildName_, const std::wstring  strURL_ ); // c->s. 길드 생성 요청
    bool Send_EditGuildNoticeReq( const int iNoticeID_, const std::wstring strNotice_ ); // C->S. 길드 소개/공지 작성(수정)
    bool Send_EditGuildNameReq( const std::wstring strGuildName_, const DWORD dwGuildID_ ); // c->S. 길드명 변경 요청
    bool Send_ChangeGuildJoinSettingReq( const UCHAR ucJoinSetting_ ); // C->S. 길드 가입설정 변경
    bool Send_EditGuildUrlReq( const std::wstring strURL_, const DWORD dwGuildUID_ ); // c->S. 길드URL 입력(수정)
    bool Send_BreakupGuildReq( const int iGuildID_, const std::wstring strGuildName_ ); // c->S. 길드 해산
    bool Send_ChangeGuildMemberLevelReq( const std::map< char, std::set<DWORD> >&  mapChangeMemberLevel_ ); // C->S. 길드원 등급조정(마스터/제너럴/일반/가입대기자/강퇴)
    bool Send_SelfDrumoutGuildReq(); // C->S. 길드 탈퇴 요청
    bool Send_AcceptGuildJoinerReq( const SET_DWORD setAcceptUserUID_ ); // C->S. 길드가입 승인(Join Setting에 따라 자동응답에도 사용할까?)
    bool Send_RejectGuildJoinerReq( const SET_DWORD setRejectUserUID_ ); // C->S. 길드가입 거절(Join Setting에 따라 자동응답에도 사용할까?)
	bool Send_EditGuildMyCommentReq( const DWORD dwGuildUID_, const std::wstring& strComment_ ); // C->S. 내 길드소갯말 입력(수정) (차구 구현)

    bool Send_GuildMarkUpload( IN const KSerBuffer& buffImage, IN const std::wstring& strExt, IN const GCITEMUID itemUID ); // 이미지 버퍼, 확장자, 아이템 UID
    bool Send_GuildRankReq( const int nType );
    bool Send_GuildNoticeListReq(); // 길드공지사항요청

    bool Send_JoinRoomFromSquareReq( const DWORD dwChannelUID, const unsigned int uiJoinType, 
        USHORT usRoomID, const wchar_t* szRoomPasswd,
        const KInDoorUserInfo& kInDoorUserInfo, std::vector<USHORT> vecKickedRoomID,
        char cQuickJoinCategory = GC_GMC_INVALID, int iQuickJoinMode = GC_GM_INVALID,
        int nDifficult = GC_SGM_DUNGEON_NORMAL,
        bool bGuild_ = false );

    bool Send_InviteGuildUserReq( const std::vector<DWORD>& vecUsers );
    bool Send_GuildPointReq();
    bool Send_GuildMemberListReq( const int nPageNum = 1 );

    // 091119. kkurrung. 퀴즈 빙고.
    bool Send_GetUserBingoDataReq();
    bool Send_ChangeBingoCoin();
    bool Send_GetBingoQuestion( const int nXPos, const int nYPos );
    // 문제 번호, 정답 번호, 빙고판 좌표 <x,y>, 부활 아이템 사용.
    bool Send_AnswerBingoQuestion( const int nQuestionID, const int nAnswerNumver,
                                   const std::pair<int,int>& prPos, const bool bUseResurrection = false );

    bool Send_NewYearDonationInfoReq(DWORD dwType_);
    bool Send_NewYearDonationReq(DWORD dwType_,DWORD dwItemID_, int nItemCount_);

    // 091230 tgkwon. 새해 소원 성취 이벤트
    bool Send_NewYearDonationInfoReq();
    bool Send_NewYearDonationReq();

    bool Send_AttributeSelectListReq( IN GCITEMUID itemUID );
    bool Send_AttributeSelectReq( IN const GCITEMUID ItemUID_, IN const std::vector< KAttributeInfo >& vecSelectedAttributes_ );
    
    // 100119. woosh. 소켓
    bool Send_OpenSocketReq( IN const GCITEMUID ItemUID_, IN const GCITEMID CashItemID_, IN const char SlotID_, IN const int ConsumeKind_ );
    bool Send_InsertItemSocketReq( IN const GCITEMUID ItemUID_, IN const GCITEMID CashItemID_, IN const char SlotID_, IN const int ConsumeKind_, IN const GCITEMID CardItemID_ );
    bool Send_RemoveItemSocketReq( IN const GCITEMUID ItemUID_, IN const GCITEMID CashItemID_, IN const char SlotID_, IN const int ConsumeKind_ );
   
	bool Send_ENU_OVERLAP_FILE_INFO();
    bool Send_GetWebCashItemReq( IN const int iType, IN const DWORD m_dwEventUID = 0);
    bool Send_StatGameResolution( IN const int nWidth, IN const int nHeight, IN const int nMaxWidth, IN const int nMaxHeight );

    // 2010.02.05. kkurrung. Item Breakup
    bool Send_ItemBreakupReq( IN const GCITEMUID ItemUID );

    // 2010.02.17. kkurrung. Item 제작
    bool Send_GetItemCatalog();
    bool Send_GetMaterials( IN const std::set<PAIR_USHORT_DWORD>& setItems );
    bool Send_ItemCreationReq( IN const USHORT Index_, IN const GCITEMID ItemID_, IN const USHORT Factor_, IN const std::vector< GCITEMUID > vecUIDList_ );
    bool Send_BreakUpPriceRatioReq();

    // 100223 tgkwon. 선물 상자 이벤트
    bool Send_PresentboxInfoReq(); // 선물 상자 아이템을 클릭하고 정보요청.
    bool Send_PresentboxTradeReq( DWORD dwItemID ); // C->S 특정 조각 아이템 교환 요청
    bool Send_PresentboxActionReq(); // C->S 조각을 다 모아서 선물받기 버튼 클릭.

    bool Send_CanBuyCashItem( IN const GCITEMID& ItemID );
    bool Send_DungeonStatusReq( IN const std::set<int>& setGameModeList );
    //bool Send_TourmentDonationReq(IN const int nType_, IN const GCITEMID ItemID_);
    bool Send_GuildeBookListReq();
    bool Send_GuideBoolCheckPointNot( IN const short sKey_, IN const short sValue_ );
    bool Send_CheckDynamyLoadReq();

    //  브라질 정상종료 카운트 하려고 만든 패큇
    bool Send_ExitNormalNot();

    bool Send_PingInfoNot( IN const DWORD dwPingInfo_ );
    bool Send_PingConfigReq();
    bool Send_RoomMemberPingInfoReq();

    // 업적 관련.
    bool Send_GetAchievementFullInfo();
    bool Send_ReportAchievementReq( IN const std::map<int,int>& mapAchieve_ );
    bool Send_CompleteAchievementReq( IN const int nAchieveID_ );
    // 대기방 상태/ 압박 ...
    bool Send_GetRoomMenberPressStateReq();
    bool Send_SetMyPressState( IN const int nState_ );
    bool Send_PressMember( IN const DWORD dwUserUID_ );
    bool Send_TRInfoReq();

    bool Send_GetRoomMenberIdleState();
    bool Send_SetMyIdleState( IN const bool bIdle_ );

	bool Send_RelayLoadingState( IN const std::pair<DWORD,int> kPacket );
    // 룩 변환 관련
    bool Send_NormalItemToLookItem( IN const std::set<GCITEMUID>& setItems_ );
    bool Send_LookItemToNormalItem( IN const std::set<GCITEMUID>& setItems_ );
    bool Send_SaveCoordiReq( IN const KSaveCoordiInfo& kCoordiInfo_ );
    bool Send_ChangeLookEqipReq( IN const KEVENT_CHANGE_LOOK_EQUIP_REQ& kReq_ );

    // 묶어 팔기.
    bool Send_BundleSellReq( IN const KEVENT_BUNDLE_SELL_ITEM_REQ& kPacket_ );

    bool Send_BuyForGambleReq( IN const std::vector<GCITEMID>& vecItemID, const int nDuration, const int m_nPeriod ); // EVENT_BUY_FOR_GAMBLE_REQ
    bool Send_CostRateForGambleBuyReq(); // EVENT_COST_RATE_FOR_GAMBLE_BUY_REQ
    bool Send_BossDungeonStat( IN const KEVENT_STAT_END_GAME_INFO& kPacket_ ); // EVENT_STAT_END_GAME_INFO

    bool Send_HeroItemCatalogReq();
    bool Send_HeroItemMaterialReq( IN const std::set<PAIR_USHORT_DWORD>& setItemID_ );
    bool Send_BuyForHeroReq( IN const PAIR_USHORT_PAIR_DWORD_INT prKey_ );

    bool Send_CharPromotionUpdateReq( IN const GCITEMID ItemID_, IN const char cCharType, IN const char cPromotionLevel );
    bool Send_SpecialRewardReq( IN const std::map< DWORD, std::set<USHORT> >& mapUserSelect_ );

	bool Send_GetUserGwibawiboDataReq(int ver_/*버젼!!!*/);
	bool Send_GetUserGwibawiboReq();
	bool Send_GetUserGwibawiboGiveUpReq(int iRetry);
	bool Send_GetUserGwibawiboTryPointInfoReq();
	bool Send_GawibawiboTradeReq( DWORD dwItemID_ );
    bool Send_InitItemAttributeReq( DWORD dwItemUID, DWORD dwItemID_ );//첫번째는 초기화 될 아이템 두번째는 초기화 시키는 쿠폰 아이뒤.
    bool Send_InitGPItemAttributeReq( GCITEMUID dwItemUID, DWORD dwItemID_ );//첫번째는 초기화 될 아이템 두번째는 초기화 시키는 쿠폰 아이뒤.

    // 실시간 보상
    bool Send_DungeonRewardGPNot( IN const std::set<DWORD>& setGpUID );
    bool Send_DungeonRewardItemReq( IN const std::pair<DWORD,DWORD>& prDrop, IN const std::pair<DWORD,std::vector<DWORD> >& prRouletteList );
    bool Send_DungeonRewardExpReq( IN const int& nTriggerID );
    bool Send_PvpRewardExpGpReq( IN const std::pair<DWORD,char>& prKillUserInfo, IN const std::pair<DWORD,char>& prDeathUserInfo );
    bool Send_PvpTagRewardExpGpReq( IN const DWORD& dwKillUserUID, IN const std::vector< std::pair<DWORD,char> >& vecKillTeamInfo, IN const std::pair<DWORD,char>& prDeathUserInfo );
	bool Send_SocksHangupReq( IN const std::vector< GCITEMID >& vecSocks_ );
    bool Send_SocksCollectReq( IN const std::vector< GCITEMID >& vecSocks_ );
    bool Send_SocksMaterialExchangeReq();
	//bool Send_OneadayInfoReq();

    bool Send_UnlockChangeWeaponReq( IN const char& cCharacterType );
    bool Send_ItemAttributeRandomSelectReq( IN GCITEMUID dwItemUID, IN DWORD dwItemID );
    // dwItemUID = 랜덤속성 적용할 ItemUID , dwItemID = 랜덤 속성시 필요한 소비 ItemID.
    bool Send_GPItemAttributeRandomReq( IN GCITEMUID dwItemUID, IN DWORD dwItemID );
    
    bool Send_GuildStoreCatalogReq();
    bool Send_BuyForGuildReq( IN const int& nTabID, IN const USHORT& usIndex, IN const GCITEMID& ItemID );

    bool Send_UserServerRoomReq( IN const DWORD& dwTargetUserUID, IN std::wstring strTargetNick = L"" );
    bool Send_DisableTodaysPopupNot();
    bool Send_ResetSkillReq( IN const GCITEMID& ItemID, IN const char& cCharType );
    bool Send_FashionCatalogReq();
    bool Send_UserRecomTypeReq();

    // 나무나무 이벤트 정보 요청.
    bool Send_PlantTreeInfoReq();
    bool Send_PlantTreeActionReq(DWORD dwActionType,DWORD dwItemID);
    bool Send_PlantTreeRewardReq(int iType);

    bool Send_ChoiceBoxListReq();
    bool Send_ChoiceBoxInfoReq( IN const GCITEMID& BoxItemID );
    bool Send_ChoiceBoxOpenReq( IN const GCITEMID& BoxItemID, IN const std::set<USHORT>& setSelectRewardIndex );

    bool Send_VipEventListReq();
    bool Send_VipEventUserInfoReq( IN const USHORT& usVer );

    bool Send_MagicBoxListReq();
    bool Send_SetMagicBoxTargetReq( IN const GCITEMID& itemID );
    bool Send_UseMagicBoxReq( IN const GCITEMID& targetItemID, IN const GCITEMID& keyItemID );

    bool Send_UserAuthCheckReq( KEVENT_USER_AUTH_CHECK_REQ& kData_ );
    bool Send_ItemLevelListReq( IN const DWORD dwItemUID );
    bool Send_ItemLevelSelectReq( IN const DWORD dwItemUID, IN const int iLevel );
    bool Send_ItemAttributeTableIDReq( IN const GCITEMUID& ItemUID );
    bool Send_PackageInfoReq();
    bool Send_PackageInfoDetailReq( IN const int& nMaxIndex, IN const int& nCurrentIndex );
    bool Send_RickoonActionReq( IN DWORD dwType, IN DWORD dwCharType, IN GCITEMID CoinItemID, IN GCITEMID LuckyCoinItemID );
    bool Send_OpenCalendarNot();

    bool Send_UserSphinxDataReq();
    bool Send_SphinxQuestionReq( IN const DWORD dwItemID);
    bool Send_SphinxAnswerReq( IN const int nQuestionID, IN const int nAnswerNum );

    //아지트 파티시스템 관련
	bool Send_CreatePartyReq( IN const DWORD dwUserUID, IN const DWORD dwPartyType );
	bool Send_JoinPartyReq( IN const KPartyData kPartyData, bool bIsJoin );
    bool Send_InvitePartyRoomReq();
	bool Send_InvitePartyReq( IN const DWORD dwInviteeUID_, IN const DWORD dwPartyType );
	bool Send_InvitePartyAnswer( IN const KPartyData& kData_ , bool bIsInvite );
    bool Send_ChangePartyHostReq( IN const DWORD dwNewHostUID_ );
    bool Send_LeavePartyUserReq( IN const DWORD dwUserUID_ );
    bool Send_BanPartyUserReq( IN const DWORD dwUserUID_ );
    // 아지트
    bool Send_EnterAgitReq( IN const DWORD& dwAgitUID, IN const char& cCharType, IN std::wstring strPassword_=L"" );
    bool Send_AgitLoadingCompleteReq();
    bool Send_AgitLoadingFailNot();
    bool Send_LeaveAgitReq();
    // 아지트설정
    bool Send_AgitMapCatalogueReq();
    bool Send_BuyAgitMapReq( IN const DWORD& dwMapUID );
    bool Send_SetAgitOptionReq( std::wstring strName, UCHAR ucMaxUser, UCHAR ucOpenType, std::wstring strPassword, UCHAR ucInviteType, std::wstring strProfile );
    // 아지트상점
    bool Send_AgitStoreCatalogReq();
    bool Send_AgitStoreMaterialReq( IN const std::set<PAIR_USHORT_DWORD>& setItemID_ );
    bool Send_BuyForAgitReq( IN const PAIR_USHORT_DWORD prKey_ );
    bool Send_SellForAgitReq( IN const std::vector<GCITEMUID>& vecSellItemUID_ );
    // 아지트화분
    bool Send_SeedFlowerPotReq( IN const GCITEMUID& PotItemUID, IN const GCITEMUID& SeedITemUID, IN const int& nSeedType );
    bool Send_HarvestFlowerPotReq( IN const GCITEMUID& PotItemUID, IN const bool& bBonus );
    // 아지트채팅
    bool Send_AgitChatReq( IN const char& cChatType, IN const std::wstring& strMsg, IN const DWORD& dwChatColor );
    bool Send_AgitBanUserReq( IN const DWORD& dwUserUID );
    // 오브젝트배치
    bool Send_SetAgitObjectPosReq( IN const std::vector<KAgitObjectPos>& vecList_ );
    // 아지트 즐겨찾기
    bool Send_AgitFriendListReq( IN const std::set<DWORD>& setFriendUserUID );
    bool Send_AgitTopRankListReq();
    bool Send_AgitFavoriteListReq();
    bool Send_AgitAddFavoriteReq( IN const DWORD& dwAgitUID );
    bool Send_AgitRemoveFavoriteReq( IN const DWORD& dwAgitUID );
    // 아지트 리스트
    bool Send_RegisterAgitPageReq( IN const std::wstring& strAgitName, IN const UCHAR& ucMaxUser, IN const UCHAR& ucOpenType, IN const std::wstring& strPassword );
    bool Send_UnregisterAgitPageReq();
    bool Send_AgitPageListReq( IN int nChannel=0, IN bool bOpen=true, IN bool bNotFull=true, IN USHORT usMinUser=0, IN USHORT usPageNum=1 );
    // 도우미
    bool Send_BrownieStoreCatalogReq();
    bool Send_BrownieStoreMaterialReq( IN const std::set<PAIR_USHORT_DWORD>& setItemID );
    bool Send_BuyForBrownieReq( IN const PAIR_USHORT_DWORD prKey );
    bool Send_EquipBrownieReq( IN const DWORD& dwBrownieID, IN const GCITEMID& ItemID, IN const GCITEMUID& ItemUID );
    // 요정의나무
    bool Send_WateringFairyTreeReq();
    bool Send_UseFairyTreeFruitReq();
    // 튜토리얼
    bool Send_AgitTutorialReq();
    bool Send_AgitTutorialDoneReq();
    // 아지트초대
    bool Send_InviteAgitReq( IN const std::vector<DWORD>& vecUserUID );
    // 낙서장
    bool Send_GuestbookPageReq();
    bool Send_GuestbookWriteReq( IN const std::wstring& strMsg_ );
    bool Send_GuestbookDeleteReq( IN const std::set<int>& setDeleteNo_ );
    // 훈련소
    bool Send_UseTrainingObjReq( IN const int& nCharType, IN const GCITEMUID& ItemUID );

    bool Send_StrengthEquipReq( GCITEMUID dwEnchantItemUID_, GCITEMUID dwEquipItemID_, DWORD dwEquipItemType_ );
    bool Send_StrengthActionReq(KEVENT_STRENGTH_ACTION_REQ& kPacket_ );
    bool Send_StrengthBreakUpReq( GCITEMUID dwEnchantItemUID_, GCITEMUID dwMaterialItemUID_ );
    bool Send_StrengthComposeReq( std::pair<GCITEMUID,GCITEMUID> prItemInfo_, GCITEMUID dwAssistItemUID_ );
    bool Send_InvenBuffItemListReq();
    // 캐릭터 배치
    bool Send_OpenAgitCharReq( const int& nCharType_ );
    bool Send_SetAgitCharPosReq( const std::vector<KSimpleAgitCharPos>& vecCharPos_ );
    bool Send_SetAgitCharMotionReq( const int& nCharType_, const int& nMotionID_ );
    bool Send_SetAgitCharCoordiReq( const int& nCharType_, const std::map<DWORD,GCITEMUID>& mapSlotItemUID_ );
    bool Send_SetAgitCharSpeechReq( const int& nCharType_, const std::map<USHORT,std::wstring>& mapSpeech_ );

    bool Send_FairyTreeLvTableReq();

    bool Send_GWCRakingRewardReq();
    bool Send_GWCRakingListReq();

    // 우편함
    bool Send_GetPostLetterListReq( IN const int& nPageNo_, IN char cLetterListType_ = KEVENT_GET_POST_LETTER_LIST_REQ::LL_ALL );
    bool Send_SendLetterReq( IN std::wstring strToNickName_, IN std::wstring strTitle_, IN std::wstring strMessage_, IN std::vector<GCITEMUID> vecItemUID_, IN std::map<GCITEMUID,int> mapCountItemUID_, IN int nGamePoint_ = 0 );
    bool Send_ReadLetterReq( IN const POSTUID& PostUID_ );
    bool Send_GetItemFromLetterReq( IN const POSTUID& PostUID_ );
    bool Send_DeleteLetterReq( IN const std::vector<POSTUID>& vecPostUID_ );
    bool Send_RenewalLetterReq();

    // 설문 시스템 답변 전달 및 보상 요청
    bool Send_SurveyRewardReq( KEVENT_SURVEY_REWARD_REQ& kPacket_ );

    // 가챠 공지 읽음 표시
    bool Send_GachaNoticeReadCheckReq();

    //SHA ON/OFF 여부
    bool Send_EventSHAEnableReq();

    // Just in time 아이템 보상 요청
    bool Send_EventJustInTimeRewardReq( KEVENT_JUST_IN_TIME_REWARD_REQ& kPacket_);
    
    // 크리스 마스 이벤트
    bool Send_CharismasEventInfoReq(); // 선물하기
    bool Send_CharismasEventConnectCheckReq( IN const std::wstring wstrNickName ); // 유저접속 체크하기
    bool Send_CharismasEventPresentReq( KEVENT_CHARISMAS_EVENT_PRESENT_REQ& kPacket_ ); // 선물하기
    bool Send_CharismasEventDonationReq( KEVENT_CHARISMAS_EVENT_DONATION_REQ& kPacket_ ); // 기부하기.
    bool Send_CharismasEventGradeRewardReq( KEVENT_CHARISMAS_EVENT_GRADE_REWARD_REQ& kPacket_ ); // 등급보상받기.

    bool Send_SockTreeReq(); //양말걸기 정보 요청 REQ
    // 던전 클리어보상 아이템요청
    bool Send_DungeonClearRewardReq( IN const int& nModeID, IN const std::vector<INDEX_GCITEMID>& vecIndexItemID );
    // 아이템 장착레벨 낮추기 요청
    bool Send_EquipLevelDownReq( IN const GCITEMUID& EquipItemUID_, IN const GCITEMUID& MaterialItemUID_ );
    // 몬스터카드 조합 요청
    bool Send_MonsterCardMixReq( IN const std::map<GCITEMUID,int>& mapList );

    bool Send_SSkillHotKeyUsedInfo( KEVENT_STAT_HOTKEY_USED_INFO_NOT& kPacket_ );

    // 개기일식
    bool Send_EclipsePlotEventInfoReq();
    bool Send_EclipsePlotEventTimeRewardReq();
    bool Send_EclipsePlotEventHuntRewardReq( KEVENT_ECLIPSE_PLOT_HUNT_REWARD_REQ& kPacket_ );
    bool Send_EclipsePlotEventFinalRewardReq();

    bool Send_EclipseCollectInfoReq();
    bool Send_EclipseCollectRewardReq();

    // 경험치 포션
    bool Send_UseExpPotionReq( IN const DWORD dwItemID_, IN const GCITEMUID dwItemUID_, IN const char cCharType_ );
    bool Send_ExpPotionListReq();
    bool Send_DepotInfoReq(); // 창고 정보 요청, 전체 보유 캐릭터 창고 + 최대 창고 탭/슬롯 
    bool Send_DepotInfoByCharTabReq( KDepotInfo& kPacket_ ); // 창고 정보 요청, 캐릭터/탭 별
    bool Send_DepotInsertItemReq( KDepotItem& kPacket_ ); // 인벤토리-> 창고로 아이템 옮기기 요청
    bool Send_DepotDeleteItemReq( KDepotItem& kPacket_ ); // 창고-> 인벤토리로 아이템 옮기기 요청
    bool Send_DepotMoveItem( KEVENT_DEPOT_MOVE_ITEM_REQ& kPacket_ ); // 인벤토리 내 아이템 위치 이동
    bool Send_DepotExtendReq( KEVENT_DEPOT_EXTEND_REQ& kPacket_ ); // 탭/슬롯 확장
    bool Send_DepotDBUpdateReq(); // 웹가차 전에 요청하는 디비 데이터 동기화.
    
    bool Send_AddSkillSlotOpenReq( IN const GCITEMID& ItemID_ ); // 스킬트리 추가 슬롯 오픈 요청.

    bool Send_MonsterCardMixInfoReq();

    // 송크란 이벤트
    bool Send_SongkranEventScriptInfoReq();
    bool Send_SongkranEventUserInfoReq();
    bool Send_SongkranEventWaterbombUseReq( DWORD dwScoreDiff_ );
    bool Send_SongkranEventGradeRewardReq();
	bool Send_SongkranEventWaterBombExchangeReq();

    //모험가 이벤트 (GC 원정대)
    bool Send_GcAdventureDateReq();
    bool Send_GcAdventureContinentRewardReq( KEVENT_ADVENTURE_CONTINENT_REWARD_REQ& kPacket_ );
    bool Send_GcAdventureWholeContinentFinalRewardReq();


    bool Send_LevelResetItemReq( const char& cCharType, GCITEMID itemID );


    //  테스트용 
    bool Send_AddItem( const GCITEMID& ItemID, const int nPeriod, const int nDuraiton, const int nGrade, const int nItemLevel );

    //올림픽 이벤트 미니게임
    bool Send_OlympicJoinGameReq();
    bool Send_OlympicGawibawiboReq( const time_t tmTodayDate, const int nGameType, const int nTeamType );
    bool Send_OlympicDicePlayReq( const time_t tmTodayDate, const int nGameType, const int nTeamType );
    bool Send_OlympicWaterbombReq( const int nScore, const time_t tmTodayDate, const int nGameType, const int nTeamType );
    
    bool Send_OlympicInfoReq();
    bool Send_OlympicScheduleReq();
    bool Send_OlympicGradeRewardReq( KEVENT_OLYMPIC_REWARD_REQ& kPacket_ );
    bool Send_OlympicTreeDropResultReq( KEVENT_OLYMPIC_TREEDROP_RESULT_REQ& kPacket_ );
    bool Send_OlympicBalloonResultReq( KEVENT_OLYMPIC_BALLOON_RESULT_REQ& kPacket_ );
    bool Send_GachaLotteryActionReq( IN const char& cCharType );

    bool Send_NewCharChoiceReq( IN const char& cCharType, IN const std::wstring& strNickName );
    bool Send_ItemComposeReq( IN const std::vector<GCITEMUID>& vecInfo );

    // 아이템 해체 정보 요청
    bool Send_GetItemBreakupInfoReq();
	bool Send_MatchInviteFriendListReq( );
	bool Send_PartyUserReadyReq(DWORD dwState, char cCharType);
	bool Send_AddMatchReq(int nModeInfo, char cCharType, int nExceptModeID );
	bool Send_CurrentMatchModeCountReq( );
	bool Send_DelMatchReq( );
	bool Send_AcceptMatchReq( bool bOk, KInDoorUserInfo userInfo );
	bool Send_StartGamePossibleReq( );
	bool Send_MatchModeStateReq( );

	// 영던템 업그레이드 관련(개편)
	bool Send_HeroItemUpgradeInfoReq( );
	bool Send_HeroItemUpgradeReq( std::pair<DWORD, int>& prKey, std::vector<GCITEMUID>& vecMaterials );


	bool Send_RitasChristmas_UserInfo_Req();
	bool Send_RitasChristmas_PlayInfo_Req();
	bool Send_RitasChristmas_RankInfo_Req();

	bool Send_RitasChristmas_StageStart_Req(const KEVENT_RITAS_CHRISTMAS_STAGE_START_REQ& kPacket_ );
	bool Send_RitasChristmas_EmptyBasket_Req(const KEVENT_RITAS_CHRISTMAS_EMPTY_BASKET_REQ& kPacket_ );
	bool Send_RitasChristmas_StageEnd_Req(const KEVENT_RITAS_CHRISTMAS_STAGE_END_REQ& kPacket_ );
	bool Send_RitasChristmas_GetReward_Req(const KEVENT_RITAS_CHRISTMAS_GET_REWARD_REQ& kPacket_ );

	bool Send_RitasChristmas_ExchangeContinueCoin_Req();
	bool Send_RitasChristmas_RewardItemList_Req();

    bool Send_Match_Rank_Page_Req( char cRankType, char cCharType, char cPageNum );
    bool Send_Match_Rank_Search_Req( char cRankType, char cCharType, std::wstring strNickName );
    bool Send_My_Match_Rank_Info_Req();

    bool SendClientHardwareInfo( const KEVENT_STAT_CLIENT_INFO& kStatClientInfo );

    // 소셜커머스(보부상) 전체 정보 요청
    bool Send_SocialCommerceInfoReq();
    // 발렌타인 버프 기부 이벤트
    bool Send_BuffDonationReq( const GCITEMID dwDonationItemID, int nDonationCount );

    // 발렌타인 누적 출석 이벤트
    bool Send_AccumulateAttendanceInfoReq();
    bool Send_AccumulateAttendanceRewardReq( const int& nRewardReqDayIndex );

    //가상캐쉬 추가 적립 정보 요청
    bool Send_CashbackExtraRatioInfoReq();
	
	bool Send_CharSelectJoinReq( );
	// 심부름왕 이벤트
    bool Send_ErrandInitInfoReq();
    bool Send_ErrandRewardReq( const int& nSuccessCount );
    bool Send_ErrandGameEndReq( const int& nSuccessCount );
    bool Send_ErrandUserDataReq();
    bool Send_LookInventoryExtendReq( IN const GCITEMID& ItemID );
    bool Send_AttributeMigrationInfoReq( IN const GCITEMUID& itemUID_ );
    bool Send_AttributeMigrationSelectReq( IN const GCITEMUID& ItemUID, IN const std::vector< KAttributeInfo >& vecAddAttributeList, IN const std::vector< KAttributeInfo >& vecDelAttributeList );
    bool Send_SubjectRewardReq(const KEVENT_DUNGEON_SUBJECT_REWARD_REQ& kPacket_ );
    bool Send_SystemGuideItemReq( const int& nGuideType, const int& nCharType );
    bool Send_SystemGuideCompleteReq( const int& nGuideType );
    bool Send_TutorialClearNOT();
	bool Send_CYOU_UserAgreement();
    bool Send_CYOU_GetWebPoint_Req();
    bool Send_CYOU_CashChangeReq( DWORD dwPoint );
    bool Send_AddSlotReq( IN const GCITEMID& ItemID );
    bool Send_ChangeCharReq( IN const char cPrevChar_,  IN const char cNextChar_ );
    bool Send_VitalitySystem_Recharge_Req( IN const int nCharType_ );
	bool Send_JoinRoomDivideReq( const int& nMaxIndex, const int& nCurrentIndex);
    bool Send_CYOU_UserLogin_Req( IN const char cPrevCharType_, IN const char cNextCharType_ );
    bool SendNickNameValidityCheckReq( IN const std::wstring& wstrNick );
    bool Send_JumpingCharReq( const char& cCharType );
    bool Send_JumpingCharRewardReq();
    bool Send_UserRelaySendCountReq( const int nGameMode, const char cCharType, int nTRCount, int nURCount );
    bool Send_CoordiComposeReq( GCITEMUID _AbilityItemUID, GCITEMUID _DesignItemUID );
    bool Send_MoveItemToInventoryFromVirtualDepot( const KEVENT_MOVE_ITEM_TO_INVENTORY_FROM_VIRTUAL_DEPOT_REQ kPacket );
    bool Send_TongDonationInfoReq( const KEVENT_TONG_DONATION_INFO_REQ& kPacket_ );
    bool Send_TongDonationDonateReq( const KEVENT_TONG_DONATION_DONATE_REQ& kPacket_ );
    bool Send_CoordiGradeUpgradeInfoReq();
    bool Send_CoordiGradeUpgradeReq( const GCITEMUID& _itemUID, const GCITEMID& _CuponItemID, const char& _cGrade );
    bool Send_SkillScrollTrainingReq( const int& _iSkillID, const GCITEMUID& _iItemUID );
    bool Send_InfinityDungeonMonsterSummonReq( int _nCurrentTopRank, DWORD _dwRoundSeq, const std::vector<PAIR_INT>& _vecSummonMonsters, int _nSummonType );
    bool Send_InfinityDungeonRewardExpReq( int _nMonUID );
    bool Send_InfinityDungeonRewardItemReq( int _nDropUID, int _nUserUID );
    bool Send_InfinityDungeonRewardGPNot( IN const std::set<DWORD>& setGpUID );
    bool Send_DungeonMissionRegistReq( DWORD dwMissionID );
    bool Send_DungeonCurrentSeasonUserRankReq( int nModeID );
    bool Send_DungeonCurrentSeasonRankPageReq( int nModeID, int nPageNum );
    bool Send_DungeonPreviousSeasonUserRankReq( int nModeID );
    bool Send_DungeonPreviousSeasonRankPageReq( int nModeID, int nPageNum );
    bool Send_SingleRandomAttributeRandomReq( int _nSlotID, GCITEMUID _itemUID, GCITEMID _UseItemID );
    bool Send_GachaPongPlayInfoReq(DWORD dwMachineNumber);
    bool Send_GachaPongPlayInfoUnsubscribeReq();
    bool Send_GachaPongPlayInfoActionReq(DWORD dwMachineNumber, GCITEMID dwGoodsID);
	bool Send_PvpRoomListSearchReq( const KRoomOptions& options, char cRoomType );
	bool Send_PvpRoomExtraOptionReq( const KRoomOptions& options );
    bool Send_CoordiViewSlotGetReq(IN const int CharType, IN const int LoginUID);
    bool Send_CoordiViewSlotSetReq(int m_iCharType, int m_iSlotID, int m_iLoginUID, std::vector< GCITEMID > m_vecItemList);
    bool Send_ChangeRoomHostReq(DWORD dwUserID_);
    bool Send_InRoomChangeCharReq(IN const char cPrevChar_, IN const char cNextChar_);
    bool Send_PetMagicStoneChangeEquipReq( GCITEMUID _petGlyphUID, int iCharType, bool bEquip );
    bool Send_UseHeroTicketReq( IN const int m_iDungeonID, IN const int m_iCharType, IN const DWORD m_dwItemID, IN const DWORD m_dwItemUID );

//#END_OF_IMPL_FUNC
public:
    KGCMsgServer* m_kMsgServer;

public:
    KUserProxy* m_pkUserProxy; // 서버간 이동을 위해, 포인터를 가지고 중간에 다른 객체를 가르킬 수 있도록 한다.

#if !defined _OPERATOR_TOOL_

    boost::shared_ptr< KNUserProxy >    m_spNUserProxy;
    boost::shared_ptr< KTRUser >        m_spTRUserProxy;

    bool ConnectToLoginServer( const char* szIP, USHORT usPort, bool bNagleAlgOn )  { return m_spNUserProxy->Connect( szIP, usPort, bNagleAlgOn ); }
    bool IsConnectedToLoginServer()                                                 { return m_spNUserProxy->IsConnected(); }

    int ConnectToGameServer( IN const KServerInfo& kServerInfo, IN KEVENT_VERIFY_ACCOUNT_REQ& kReq, IN DWORD dwTimeOut = 5000 );
    
    void DisConnectTCPRelay()   // 060301. kkurrung. TCP Relay 
    {
        m_spTRUserProxy.reset( new KTRUser( true) );
        m_spTRUserProxy->Init( false );
        m_spTRUserProxy->SetHwnd( m_hWnd );
        m_spTRUserProxy->SetClassID( TRUSERPROXY_ID );
    }

#else

    int ConnectForGMTool( IN const KServerInfo& kServerInfo, IN const KEVENT_GMTOOL_AUTHENTICATION_REQ& kReq );

#endif

private:
    DWORD m_adwSendPacketTimer[ESPT_NUM];

public:
    void SetTimer( EGCSendPacketTimer eTimer ){ m_adwSendPacketTimer[eTimer] = timeGetTime(); }
    DWORD GetTimer( EGCSendPacketTimer eTimer ){ return m_adwSendPacketTimer[eTimer]; }
    bool Send_Giftbox_NewUser_Reward_Req();
    bool Send_Giftbox_EventUser_Reward_Req();
	
public:
    bool Send_RelayServerStatusReq();
protected:
    HWND        m_hWnd;
};

DefSingletonInline( KP2P );

#define ACK_DESERIALIZE( packet )  KSerBuffer* pkBuff = (KSerBuffer*)lParam; \
    packet kRecv; \
    ks.BeginReading( pkBuff ); \
    ks.Get( kRecv ); \
    ks.EndReading()

#define ACK_DESERIALIZE_NOPARAM  KSerBuffer* pkBuff = (KSerBuffer*)lParam; \
    SAFE_DELETE(pkBuff)

#define ACK_DESERIALIZE_MYPACKET( kRecv )  KSerBuffer* pkBuff = (KSerBuffer*)lParam; \
    ks.BeginReading( pkBuff ); \
    ks.Get( kRecv ); \
    ks.EndReading()