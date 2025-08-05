#pragma once
#include "Actor.h"
#include "UserPacket.h"
#include "SobManager.h"
#include <windows.h>    // ::GetTickCount()
#include "GSNetLayer.h"
#include "UserEvent.h"
#include <atltime.h> // CTime
#include "DropSystem.h"
#include <cctype>
#include "CenterPacket.h"
#include "CacheData.h"
#include "Calendar.h"
#include "QuizBingo.h"
#include "ItemContainer.h"
#include "TimeDrop.h"
#include "ClientPing.h"
#include "UserAchievement.h"
#include "LookEquipment.h"
#include "UserEventChecker.h"
#include "PlantTree.h"
#include "Sphinx.h"
#include "Survey.h"
#include "GCPoint.h"
#include "CharismasPresent.h"
#include "UserLetterBox.h"
#include "Songkran.h"
#include "Adventure.h"
#include "OlympicManager.h"
#include "RitasChristmas.h"
#include "BuffDonation.h"
#include "AccumulateAttendance.h"
#include "ErrandManager.h"
#include "SystemGuide.h"
#include "VitalityManager.h"
#include "ConnectionGiftBox.h"
#include "InfinityDungeon.h"

SmartPointer(KUser);
SmartPointer(KSquare); // 전방 선언..

struct KUdpToServer;
class KChannel;
class KRoom;
class KGuild;
class KResultManager;
struct sTriggerInfo;
class KSkillManager;
struct SSkillElement;
struct SCoupleSignBoard;
struct KEventMonInfo;

class KUser : public KActor
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclareException;

public:
    struct UserPoolTag{};
    KUser(void);
    virtual ~KUser(void);

    enum ENUM_TICK_COUNT
    {
        SEND_CHANNEL_LIST = 0,
        SEND_ROOM_LIST,
        SEND_USER_LIST,
        SEND_GUILD_MEMBER_LIST, ///< 길드존 유저리스트 주기
        DB_UPDATE,              ///< DB 업데이트 주기
        CHAT_TICK,
        WHISPER_TICK,
        SEND_SERVER_LIST,
        //TIME_LIMIT_EVNT_TICK,   // 시간 종속적인 이벤트 유저에게 전달.
        MIGRATION_TICK,         // 서버 이동 플래그 성공일때 셋
        GAME_START_TICK,        // 게임 시작했을때 틱을 기록한다.( 게임시작하고 특정 시간동안 반응없으면 퇴장시킨다. )
        PROTECTED_TABLE_TICK,   // 클라이언트 프로텍티드 테이블 체크섬 검사 Tick.
        UPDATE_ACC_TICK,        // 플레이 누적 갱신 시간.
        P2P_CONNECT_TICK,       // 방원 전체의 Load_complete_not 이후 p2p 연결완료를 보고받기까지 시간 
        P2P_POOR_TICK,          // P2P 연결실패 리스트 요청하고 ACK 받기까지의 시간
        RANK_SEARCH_TICK,       // 랭킹 검색
        PERIOD_NOTICE_TICK,     // DBot 공지
        FRIEND_INVITELIST_TICK, // 친구 초대 리스트..
        EXP_DB_UPDATE,          // Exp DB기록 주기
        STAGE_LOAD_TICK,        // 스테이지 로딩
        USER_WHERE,             // 유저 위치 정보
        AGIT_STAY,              // 아지트 거주시간 체크
        SERVER_TIME_NOT,        // 서버시간 전달
        AGIT_BONUS,             // 아지트 보너스 체크
        GUESTBOOK_TICK,         // 낙서장 쓰기 주기
        SHUTDOWN_CHECK_TICK,    // 셧다운 체크 주기
        NMCAFE_PLAY_TIME,       // 넷마블PC방 플레이시간
        NMCAFE_PLAY_TIME_UPDATE,// 넷마블PC방 플레이시간 갱신
        NEW_POST_UPDATE,        // 새 우편 요청
        MATCH_INVITELIST_TICK,  // 대전 매칭 초대 리스트.
        TICK_COUNT_NUM,
    };

    enum HARDCODING_GOODS_ID
    {
        GEM_GOODS_ID                        = 8880,
        CRYSTAL_GOODS_ID                    = 12490,
        DOUBLE_CRYSTAL_CARD_GOODS_ID        = 12500,
        DISCOUNT_GOODS_ID                   = 12510,
        TURTLE_KNIGHT_ID                    = 4590,
        TURTLE_ARCHER_ID                    = 4600,
        TURTLE_MAGICIAN_ID                  = 4610,
        CHANGE_NICK_GOODS_ID                = 23720,    // 닉네임 변경 아이템.
        DC30_DOUBLE_CRYSTAL_CARD_GOODS_ID   = 28830,
        INDIGO_TICKET_ID                    = 27760,
        CHANGE_PET_NICK_GOODS_ID            = 38970,
        //GC_CLUB_ID                          = 42340,    // GC클럽 아이템(브라질)
        HATCHING_ITEM_ID                    = 43550,    // 부화 아이템.
        //STRONG_ASSIST_WEAK                  = 47970,    // 강화 하락 방지 아이템.
        RESET_WIN_LOSE_ALL                  = 47980,    // 승률 리셋 아이템 (이리스의 눈물)
        RESET_WIN_LOSE_ELESIS               = 47990,    // 승률 리셋 아이템 ELESIS
        RESET_WIN_LOSE_LIRE                 = 48000,    // 승률 리셋 아이템 LIRE
        RESET_WIN_LOSE_ARME                 = 48010,    // 승률 리셋 아이템 ARME
        RESET_WIN_LOSE_LAS                  = 48020,    // 승률 리셋 아이템 LAS
        RESET_WIN_LOSE_RYAN                 = 48030,    // 승률 리셋 아이템 RYAN
        TURTLE_LAS_ID                       = 31920,
        TURTLE_RYAN_ID                      = 70040,
        NETMARBLE_START_ID                  = 72560,    // 넷마블 별이벤트용 아이템.
        CASH_KEY_ID                         = 102030,  // 가차 캐시 Key ID
        GP_KEY_ID                           = 110050,  // GP 키 ID
        TURTLE_RONAN_ID                     = 96940,
        TURTLE_AMY_ID                       = 96950,
        GC_MEDAL_ID                         = 96220,    // 미국 이벤트용 아이템.
        QUEENS_BOX_ID                       = 96230,    // 미국 이벤트용 아이템. 여왕의 상자.
        SOCKET_OPEN_ITEM_ID                 = 287060,   // 소켓 봉인해제 아이템
        STRENGTH_ID                         = 627270,   // 강화석 ItemID
        VIPCOIN_ID                          = 1901790,   // 강화석 ItemID
        CHANGE_NICK_COLOR_GOODS_ID          = 1842660,    // 닉네임 변경 아이템.
    };

    enum AUTH_LEVEL
    {
        AL_CHAT_BLOCK   = -4,   // 채팅블럭
        AL_BLOCK        = -3,   // 블럭
        AL_BAD          = -2,   // 불량
        AL_WARNING      = -1,   // 경고
        AL_NORMAL       = 0,    // 일반
        AL_CHALLENGER   = 1,
        AL_HIGH_RANKER  = 2,
        AL_ADMIN        = 3,
    };

    enum SESSION_INFO   // 신규유저, 오늘자 첫 접속 여부를 기억한다. 아이템 증정 이벤트등에 사용.
    {
        SI_NEW_ACCOUNT  = 0,
        SI_TODAY_FIRST_LOGIN_NO_LAS, //051213. microcat. 첫접속한 유저가 라스 미보유
        SI_TODAY_FIRST_LOGIN_LAS, //051213. microcat. 첫접속한 유저가 라스 보유
        SI_TODAY_LOGINED_BEFORE,
        SI_INVALID_VALUE    // default value, sentinel, exception case 등에 사용.
    };

    // 센터서버에서도 사용됨에 따라 KItemUnit에 Enum값 옮김
    // enum ITEM_TYPE
    // {
    //     IT_NORMAL       = 0,
    //     IT_DURATION,
    //     IT_NUM,
    // };

    enum {
        MAX_EQUIP_SIZE          = 36,           // 051004. florist. // 051024. jseop 기술아이템 추가로 늘어남. 15 -> 29
        USER_MIN_EXP            = 0,         // 최소 경험치.
    };

    // 070306. kkurrung. 신학기 이벤트 끝나면 아래 ENUM지우기.
    //2007030801 신규, 2007030802 휴면, 2007030803 추천
    enum RECOMMEND_KOR
    {
        RK_NONE_USER            = 0,          // 아무것도 아닌 유저
        RK_NEW_USER             = 2007030801, // 신규 유저
        RK_DORMANCY_USER        = 2007030802, // 휴면 유저
        RK_RECOMMENDEE_USER     = 2007030803, // 추천 받은 유저
        RK_MAK,
        
    };

    // 070416. kkurrung. 펫 진화/퇴화
    enum PET_TRANSFORM_TYPE
    {
        PTT_EVOLUTION   = 0,    // 진화
        PTT_DEVOLUTION  = 1,    // 퇴화
        PTT_MAX,
    };

    // 100112 tgkwon. 채널링 추가.
    enum CHANNELINGTYPE
    {
        USER_CT_TOTAL       = 0,     // 전체.
        USER_CT_PUBLISHER_1 = 1,     // 넷마블 / 펀박스 / 대만 Gash 유저.
        USER_CT_PUBLISHER_2 = 2,     // 투니버스 / 플레이파크 / 대만 빈펀유저.
        USER_CT_PUBLISHER_3 = 3,     // 네이트.
        USER_CT_PUBLISHER_4 = 4,     // 네이버.
        USER_CT_MAX,                 // MAX.
    };

    enum CHANNELCCODE {
        CCODE_PUBLISHER_1 = 0,      // 넷마블
        CCODE_PUBLISHER_2 = 9,      // 투니버스
        CCODE_PUBLISHER_3 = 30,     // 네이트
    };

    enum EXP_CHECK_CASE {
        ECC_ADJUSTRESULTINANDOUT_DEFAULT = 0,
        ECC_ADJUSTRESULTINANDOUT_CALC = 1,
        ECC_EVENT_COMPLETE_MISSION_ACK_DEFAULT = 2,
        ECC_EVENT_COMPLETE_MISSION_ACK_CALC = 3,
    };

    enum INC_GCPOINT_TYPE {
        IGT_DAY_SEAL = 0, // 출석
        IGT_WEEK_SEAL = 1, // 주개근
        IGT_MONTH_SEAL = 2, // 월개근
        IGT_RECOMMEND = 3, // 추천
        IGT_OPEN_CALENDAR = 4, // 달력오픈
    };

    enum DEC_GCPOINT_TYPE {
        DGT_BUY_GPPOINT_ITEM = 0, // 출석포인트 아이템 구입
    };

    enum NATION_GAMECODE {
        NG_GC_US = 2,
        NG_GC_EU = 6,
    };

    enum NICKNAME_TYPE {
        NT_TEAM = 0, // 팀 닉네임.
        NT_CHAR = 1, // 캐릭터 닉네임.
    };

    enum GETINVEN_TYPE {
        GT_INGAME = 0, // 게임서버 내에서 처리.
        GT_CHANGE_CHAR = 1, // 캐릭터 변경시 처리.
    };

    static KUserPtr Create();
    static void AddManager( KUserPtr spUser );

    // override
    virtual void Tick();    // DB update, exit 상태일 경우 종료처리, logining, db_exit 일 경우 별도 H.B체크
    virtual KEvent* CreateKEvent() { return new KUserEvent; }
    virtual bool CheckExceedRefCount( int nCount );
    
    void GetInfoSimple( OUT KUserInfo& kInfo );
    bool GetInfo( OUT KUserInfo& kInfo );
    void GetInfo( OUT KDetailUserInfo& kInfo );
    bool GetInviteInfo( OUT KInviteUserInfo& kInfo_, IN bool bAdminCall_ = false );
    KUserPtr GetThisPtr()   { return std::static_pointer_cast<KUser>( (*this).shared_from_this() ); }

    void SetChannel( KChannel* pkChannel )      { m_pkChannel = pkChannel; }
    KChannel* GetChannel() const                { return m_pkChannel; }
    void SetRoom( KRoom* pkRoom )               { m_pkRoom = pkRoom; }
    KRoom* GetRoom() const                      { return m_pkRoom; }
    bool IsAdmin() const                        { return m_cAuthLevel == AL_ADMIN; }
    bool IsObserver()const                      { return IsAdmin() && m_bIsObserver; }
    char GetAuthLevel()                         { return m_cAuthLevel; }
    int GetP2PVersion()                         { return m_iP2PVersion; }
    int GetElapsedTimeAfterAuth() const;
    DWORD GetAuthType()                         { return m_dwAuthType; }

    // 050817. florist. Udp Echo를 이용해 알아낸 포트번호를 셋팅.
    void SetUdpPort( IN const SOCKADDR_IN& sender, IN const KUdpToServer& kData );

    // 050817. jseop. 캐릭터 정보 통합.
    KCharacterInfo* GetCharacterInfo( IN int iIndex );
    KCharacterInfo* GetBestCharacter();
    KCharacterInfo* GetCurrentCharacter();
    char GetCurrentCharType();
    char GetCurrentPromotion();
    bool SetCurrentChar( char cCurrentChar );
    bool IsCharExist( char cCharID );

    // 필요로 하는 아이템을 모두 가지고 있을 때 true를 리턴한다.
    bool CheckIfAllItemExist( GCITEMID ItemID, ... );
    bool CheckIfDurItemExist( GCITEMID dwItemID, int nDuration, bool bDirectUse ); // 수량 아이템을 일정 갯수 이상 가지고 있는지 확인.
    UINT GetDurItemCount( IN const GCITEMID itemID );
    // 060630. jseop. 사용한 아이템 찾기.
    bool CheckIfItemUsed( int iItemID );

    // 061229. kkurrung. 펫 관련
    bool GetPetInfo( IN GCITEMUID dwUID, OUT KPetInfo** pkPetInfo );
    bool GetBestPet( OUT KPetInfo* pkPetInfo );
    bool EquipPet( IN GCITEMUID dwPetUID, IN char cCharType );
    bool GetPetOwner( IN GCITEMUID dwPetUID, OUT std::vector<char>& vecCharType );
    bool DeletePet( IN GCITEMUID dwPetUID );

    // 2007/1/11. iridology. 펫이 장착하고 있는 아이템 중에 해당 아이템이 있으면 장착해제 시킴
    bool ReleasePetEquipElement( DWORD dwUID, char cItemType );
    void GetPetCacheData( OUT std::map<GCITEMUID, KPetInfo>& mapPetInfo );
    void GetDepotPetCacheData( OUT std::map<GCITEMUID, KPetInfo>& mapDepotPetInfo_ );

    void UnEquipAllPet();
    void ReleasePetFromChar( IN GCITEMUID dwPetUID );
    void ReleasePetCostume( IN GCITEMUID dwItemUID );

    std::wstring GetNickName() { return m_strNickName; };
    std::wstring GetNickColor() { return m_strNickColor; };

    // 071005. kkurrung. 이벤트를 위해서 만들었삼.
    bool IsExistEquipItem( IN char cCharType, IN GCITEMID dwItemID );
    bool IsExistEquipItem( IN char cCharType, IN const std::map<char, std::set<DWORD> >& mapEquips );
    bool IsEquipedItem( IN const GCITEMUID ItemUID_ );
    bool RefreshEquipItem( IN const KItem& pkRefreshItem_ );
    void CheckEvents( IN OUT KEVENT_START_GAME_BROAD& kPacket );
    void AddEvents( IN OUT KEVENT_START_GAME_BROAD& kPacket ); // JoinGame 시 호출
    static void MakeKillMonInfo( IN const std::set<int>& setKillMonster,
                                 IN const std::vector<sTriggerInfo>& vecTriggers,
                                 OUT std::map<int,int>& mapKillMonsters );

    static void MakeKillMonLevelInfo( IN const std::set<int>& setKillMonster, 
        IN const std::vector<sTriggerInfo>& vecTriggers, 
        OUT std::map<int,int>& mapKillMonstersLevel );

    bool GetSquareUserInfo( KSquareUserInfo& kInfo );
    void SetSquarePos( float fXPos, float fYPos ){ m_kSquarePos.m_fXpos = fXPos; m_kSquarePos.m_fYpos = fYPos; };
    void GetSquarePos( float& fXPos, float& fYPos ) { fXPos = m_kSquarePos.m_fXpos; fYPos = m_kSquarePos.m_fYpos; };
    int GetSquareIndex( KSquarePos& kSquarePos );
    int GetSquareIndex();

    bool HellModeCheck(int nGameMode, int nDifficulty);
    bool BreakModeCheck(int nGameMode, int nDifficulty);
    void SetAbuseToDB();

    bool IsExpAccount() { return m_bExpAccount; }; // 체험 계정인가
    void GetOnlineFriendsList( OUT std::vector<DWORD>& vecFriends );
    bool FindInDoorUserInfo( IN DWORD dwUID_ );
    bool IsNewUserAfterEventDate();

    bool GetEquipItem( IN const char& cCharType, OUT std::vector<KEquipItemInfo>& vecEquipItemInfo ); // 현재 장비 얻어오기
    void GetGuildUserInfo( OUT KNGuildUserInfo& kInfo );
    void GetSimpleGuildUserInfo( OUT KNSimpleGuildUserInfo& kInfo );
    bool GetGuildInfo( OUT KNGuildInfo& guildInfo );
    DWORD GetGuildUID() { return m_kGuildUserInfo.m_dwGuildUID; }
    std::wstring GetGuildName();

    //  091031  woosh. CheckExpirationItem 위치를 protected에서 public 으로 옮겼음. 상관없을까..
    void CheckExpirationItem();     // 051107. florist. 만기된 기간제아이템이 있는지 확인함. 
    void ApplyPetReward( IN const KGameResultOut& kResultOut_ );

    //  091118  woosh. 자신의 미션 정보만 가져오기
    void UpdateMissionInfo( IN std::vector<KMissionData>& vecMissionSlot_ );
    void UpdateCollectionInfo( IN std::vector<KCollectionData>& vecCollectionSlot_ );

    size_t GetMissionListSize() { return m_vecMissionSlot.size(); }
    bool IsPCBangUser();
    char GetPCBangType() { return m_cPCBangType; }
    bool IsTH3BBUser();
    char GetUserBenfitType() { return m_cUserBenfitType; }
    void SetNewRecomUser( IN const std::pair<int,int>& prRecomLevelUpCheck ) { m_prRecomLevelUpCheck = prRecomLevelUpCheck; };
    std::pair<int,int> GetNewRecomUserInfo() { return m_prRecomLevelUpCheck; };
    bool IsNewRecomUser();
    bool IsReturnRecomUser(){ return m_bReturnUserCheck; };

    // for Guild Serach
    void SetGuildSearchTick()                   { m_dwGuildSearchTick = ::GetTickCount(); }
    DWORD GetGuildSearchTick()                   { return m_dwGuildSearchTick; }
    void SetGuildSearchTickGap( DWORD dwGap_ )  { m_dwGuildSearchTickGap = dwGap_; }
    DWORD GetGuildSearchTickGap()                { return m_dwGuildSearchTickGap; }

    void UpdateGrade();
    void MakeCorrectInDoorInfo( IN OUT KInDoorUserInfo& kInDoorUserInfo );

    void UpdateMapDifficulty( IN const char& cCharType, IN const std::map<int,KDungeonUserInfo>& mapDifficulty );

    void UpdateGuildPoint( IN DWORD& dwGuildPoint_ );
    void InitLoadingCompleteTick();
    // 현재 유저의 채널 타입에 대해서 리턴( 채널링 추가:넷마블/투니버스 )
    DWORD GetUserChannelType() { return m_dwChannellingType; };
    USHORT GetUserChannelCode();
    bool GetLoadingComplete() { return m_bRecvLoadingComplete; };

    bool HeroDungeonCheck( IN const int& nModeID, IN const int& nDifficulty );
    bool HeroDungeonLevelCheck( IN const int nModeID, IN const int nCharIndex );
    bool UpdateHeroDungeonPlay( IN const int nModeID, IN const bool bClear );
    void UpdateHeroDungeonPlay( IN const int nModeID, IN KUserPtr& spUser, IN const KGameResultIn& kGameResultIn );
    bool HeroDungeonCharacterCheck( IN const DWORD& dwMinCharacterLv );
    bool HeroDungeonEnterTimeCheck( IN const std::map<USHORT,std::pair<time_t,time_t> >& mapTimeTable, OUT USHORT& usTimeIndex );
    bool GetHeroPlayData( IN const int& nModeID, OUT KHeroPlayData& kData );
    bool HeroDungeonPlayLimitCheck( IN const int& nModeID, IN const USHORT& usTimeIndex, IN const USHORT& usMaxClearPerDay );
    bool HeroDungeonStartCheck( IN const int nModeID );
    bool GetHeroDungeonTimeIndex( IN const std::map<USHORT,std::pair<time_t,time_t> >& mapTimeTable, OUT USHORT& usTimeIndex );
    bool SetHeroDungeonCharIndex( IN const int& nModeID, IN OUT KInDoorUserInfo& kUserInfo );
    bool SetHeroDungeonCharIndex( IN const DWORD& dwMinCharLv, IN OUT char& cCharIndex, IN OUT char& cPromotion );

    DWORD GetPing() { return m_kPingInfo.GetPing(); }

    void InitStageLoadTick() { SetTick( KUser::STAGE_LOAD_TICK ); }
    void InitStageLoadFlag() { SetStageLoadComplete( false ); }

    void AddChangeWeaponEquipInfo( IN const std::pair<GCITEMID,GCITEMUID>& prChangeWeapon, IN OUT std::vector<KEquipItemInfo>& vecEquip );
    void DumpEquipItems( IN const int& nCharType, IN const KEquipItemInfo& kChangeWeapon, IN const std::vector<KEquipItemInfo>& vecEquip );
    std::wstring GetGuildMarkName();
    void AddGuildContributePoint( IN const int guildPoint_ );
    DWORD GetGuildBattlePoint();
    std::wstring GetCountryCode() { return m_wstrCCode; }
    bool CheckSameNationGroup( IN const std::wstring& wstrUserCCode, IN const std::wstring& wstrRoomHostCCode );

    DWORD GetPartyUID() { return m_kParty.m_dwPartyUID; };
    int GetPartyIndex();

    int GetLanguageCode() { return m_nLanguageCode; };
    void SetLanguageCode( int code_ ) { m_nLanguageCode = code_; };
    bool DungeonTicketCheck( IN const int& nGameMode );

    bool CheckItemInvenLimit( IN const GCITEMID& ItemID );
    bool CheckItemCount( IN const GCITEMID& ItemID );

    bool GetDungeonUserInfo( IN const int& nModeID, OUT KDungeonUserInfo& kInfo );
    int GetAccOnlineTime() { return m_nAccOnlineTime; };
    int CheckFatigueTime();

    void SetEclipseCollectComplete( IN bool& bEclipseCollectComplete ) { m_bEclipseCollectComplete = bEclipseCollectComplete; };
    bool GetEclipseCollectComplete(){ return m_bEclipseCollectComplete; };

    bool IsLINBenefitUser();	
    bool CheckServerTypeEnable( IN const int& nGameMode );
    
    void AddStatPlayTime( IN const DWORD& dwPVPPlayTime, IN const DWORD& dwPVEPlayTime, IN const DWORD& dPlayTime );

    void DumpInventory();
    void GetCharSlotNum( OUT DWORD& dwSlotNum, IN char cCharType = -1 );
    void GetCharNickNameInfo( IN const char& cCharType, OUT std::wstring& wstrNickName );
    int GetCharInvenCapacity();
    int GetTrueInvenCapacity();
    void UpdateCharInvenCapacity( IN const int& nAddInvenCapacity, OUT int& nTotalInvenCapacity );
    void UpdateCharNickNameInfo( IN const char& cCharType, IN const std::wstring& wstrNickName );
    void GetCharQuickSlotInfo( OUT KQuickSlot& kQuickSlot );
    bool CheckChatEnable();
    void GetUserDifficultInfo( OUT std::map<int,KDungeonUserInfo>& mapDifficulty );
    void UpdateCharDifficultInfo( IN const char& cCharType, IN std::map<int,KDungeonUserInfo>& mapDifficulty );
    bool ChangeUserChatState( IN const bool& bChatEnable );

    void SetVirtualDepotItemList( IN std::vector<KItem>& vecVirtualDepot );
    bool IsValidItemCount( IN const KItem& kCountItem );
    bool GetVirtualDepotItemByItemUID( IN const GCITEMUID& dwItemUID, OUT KItem& kItem );
    int GetCharLookInvenCapacity();
    int GetTrueLookInvenCapacity();
    void UpdateCharLookInvenCapacity( IN const int& nExtendSize );
    void AddVirtualDepotItemList( IN std::vector<KItem>& vecAddItems );

protected:
    //void PreStartQuestZeroProcess( char cSelChar ); // 060112. kkurrung. 퀘스트 제로는 스타트 게임을 로비에서 호출한다.

    void PreStartIndigo( OUT KInDoorUserInfo& kInDoorUserInfo );
    void GetInDoorCharInfo( OUT std::vector<KInDoorCharInfo>& vecCharInfo );

    void GetDefaultResultOut( OUT KGameResultOut& kResultOut );
    void DumpEndGameInfo( IN const KGameInfo& kGameInfo, IN const KEndGameReq& kReq );
    void DumpResultIn( OUT std::wstringstream& stm , IN const KGameResultIn& kResultIn );
    bool CheckRequireItem();

    void SendIndigoRank();
    void SendVIPItemListNot(); // VIP용 아이템 리스트 보내기
    void SendChatLogNot( IN const std::wstring& strChannel_, IN const int nRoomID_, IN const std::wstring& strMsg_ ); //채팅로그 보내기.
    void SendCapsuleListNot();   // GP캡슐정보 리스트 전달
    void CheckGachaReelPointReq(); // 가챠 릴 점수 가져오기
    void UpdateGachaReelPointToDB(); // 가챠 릴 점수 DB저장
    void CheckGCClubConnectBonus( int nSessionInfo ); // GC클럽 접속시 보너스아이템 지급
    void CheckGCClubmapDayAttendBonus(); // GC클럽 일일 출석시 보너스 아이템 지급
    void SendItemTradeListNot();        // 아이템 교환 리스트 전달
    void SendSubscriptionInfoNotice( IN const KECN_SUBSCRIPTION_INFO_UPDATE_NOT& mapEventInfo_ );  // 응모이벤트 정보 전달(최초접속시,이벤트갱신시)
	void UpdateUserBoardInfo( OUT DWORD& dwInitHour, OUT KUserBoardInfo& kUserBoardInfo, OUT std::wstring& strDiceLastTime ); // 유저의 보드게임 정보 저장.
    void UpdateAdvertisingExposureCountToDB(); // 광고별 노출 카운트 DB저장

    bool HackingCheck( IN const KGameResultIn& vitResultIn_, IN const KEndGameReq& kReq_, OUT KEndGameAck& kAck_, IN KUserPtr spUser_, IN const KGameInfo& kGameInfo_
        , IN const std::vector<KGameResultIn>& vecGameResult_, IN KGameResultOut& kResultOut_, OUT bool& bRet );

    bool GetSocketItem( const IN GCITEMUID ItemUID_, OUT KItem& kItem_ );
    bool CheckSocketState( IN std::vector<KSocketInfo>& vecSocket_, IN char cSlotID_, OUT KSocketInfo& kSocketInfo_, IN char SocketState_ );
    bool CheckSocketCashItem( IN GCITEMID CashItemID_, IN KItem& kItem_, OUT KItem& CashItem_, IN bool bUseItem_ =false );
    bool CheckSocketGP( IN KItem& kItem_ );
    bool UseSocketCashItem( IN GCITEMID CashItemID_, IN KItem& kItem_, OUT KItem& CashItem_ );
    bool UseSocketGP( IN KItem& kItem_, OUT int& nGamePoint_ );
    bool UpdateSocketState( IN OUT KItem& kItem_, IN KSocketInfo& kSocketInfo_, IN char SocketState_, IN char NewSocketState_, IN bool bMonsterCardRelease_ = false );

    bool SendGuildListReqToDB( IN const int nPageNum, IN const UCHAR ucSortType, IN const UCHAR ucOrderBy );
    void SendUpdateGuildMemberNickNotice();
    void SendGuildMemberLevelupNotice( IN const char cCharType, IN const char cLevel );
    void SendGuildPageReq( IN const UCHAR ucSortType );

    bool IsPremiumUser(); // GC클럽 보유 유저인가?
    void CheckInventoryPremiumItem( OUT KPremiumInfo& kPremiumInfo ); // 인벤토리 보유한 GC클럽 ItemID 받아오기.
    bool IsPremiumItem( IN const GCITEMID dwItemID ); // 해당 아이템은 GC클럽 아이템인가?

    void AddStatDungeon( IN KGameResultIn& kResultIn, IN int nType, IN const int nTotalUsers );

    KLookEquipChar* GetLookEquip( IN const char cCharType_ );
    KItemPtr GetItemExEquipByItemID( IN const GCITEMID ItemID );

    void ExchangeSocks( const std::vector<KGameResultOut>& m_vecGameResult );

    // GP 관리용 함수
    void SetInitGP( IN const int nCurrentGP );
    void SetGP( IN const int nCurrentGP );
    void SetInitGP( IN const int nCurrentGP_, IN const char cCharType_ );
    void SetGP( IN const int nCurrentGP_, IN const char cCharType_ );

    int GetInitGP();
    int GetGP();
    int GetInitGP( IN const char cCharType_ );
    int GetGP( IN const char cCharType_ );

    void IncreaseGP( IN const int& nType, IN const bool bDBUpdate, IN OUT int& nIncGP );
    void DecreaseGP( IN const int& nType, IN const bool bDBUpdate, IN const int& nDecGP );

    float GetGuildBonus( IN const int& nBonusType );

    bool GetSocketCashItem( IN GCITEMID CashItemID, IN KItem& kItem, OUT KItem& CashItem, OUT int& nUseCount );
    void AddStatDungeonAdd( IN const KGameResultIn& kResultIn );
    // 요정의 나무 버프
    float GetFairyTreeExpBonus();
    float GetFairyTreeGpBonus();
    void SendUserLogOutInfo();

    // 창고 2012/1/18 woosh
    void GetDBDepotUpdateData( OUT KDepotUpdateData& kDepotData_ );
    // JIT
    bool CheckJustInTimeEvent( OUT time_t& tmNext, OUT std::map<USHORT,bool>& mapUserType );
    void IncreaseChangeCharType(){ ++m_dwChangeCharTypeCnt; };
    DWORD GetChangeCharType(){ return m_dwChangeCharTypeCnt; };
    
    void CYOUGetCurrentPoint( IN const int& nServerID );
    void GetDepotInfo();
    // 진행중인 가차 버전 정보 전달용
    bool GetGachaUseVersionsInfo( OUT std::vector< KGachaNewList >& vecGachaUseVersions_ );

    const bool GetDonationBuffEnable() { return m_bDonationBuffEnable; };
    void SetDonationBuffEnable( const bool& bBuffEnable ) { m_bDonationBuffEnable = bBuffEnable; };
    void AddStatPVPBalance( IN const KGameResultIn& kResultIn );
    void SendPetVestedItemNot();
    void SendEventInfo( IN KEVENT_VERIFY_ACCOUNT_ACK& kVerifyPacket, IN std::map<int,std::set<std::pair<int,int> > >& mapUserSurveyInfo, IN int& nMissionEventCheck, IN std::set<DWORD>& setStringIDList );
    void SendUserDetailInfo( const char cLastPlayCharacter_ = -99 );
    void NewUserSHDropCheck();
    void SendUserGCPointInfo();
    void SendUserAgitInfo();
    bool IsDataReceive();	

public:
    std::map<char, KCharacterInfo>  m_mapCharacterInfo;
    std::vector<int>                m_vecUsedDurationItem;  // 060630. jseop. 사용한 아이템들 기록.

    // 091223. kkurrung, milennium9. ItemInventory 
    KItemContainer                  m_kInventory;

protected:
    KChannel*                              m_pkChannel;
    KRoom*                                 m_pkRoom;

    //유저이름 : SimObject.h 참조
    // 050427. microcat. GC 구조
    std::wstring                    m_strNickName;
    std::wstring                    m_strNickColor;

    std::vector<GCITEMID>           m_vecFontVector;

    int                             m_iPvExp;

    char                            m_cGrade; //계급
    //int                             m_iInitGP;
    //int                             m_iGamePoint;

    bool                            m_bGoToGuildZone; //길드룸에서 방으로 입장했을 때 다시 길드룸으로 올지를 결정

    // 101103 tgkwon. DT,PT 통계를 위한 변수 추가.
    // DT : 게임 접속해서 접속해 있던 시간 Total
    // PT : 실제 게임 플레이( 던전, 대전 )을 진행한 시간
    unsigned int                    m_auiTickCount[TICK_COUNT_NUM];
    CTime                           m_cConnectionTime; // 050224. 접속 시간 저장 
    DWORD                           m_dwPlayTime; // PT
    DWORD                           m_dwLastPlayTime;// 마지막 플레이한 모드 PT.
    DWORD                           m_dwPVPPlayTime; // 대전 PlayTime
    DWORD                           m_dwPVEPlayTime; // 던전 PlayTime

    //std::vector<KItemInfo>          m_vecInv;
    KCacheData<int>                 m_iIndigoWin;
    KCacheData<int>                 m_iIndigoLose;
    bool                            m_bMale;
    KNSimpleGuildUserInfo           m_kGuildUserInfo; // 내 길드원 정보
    
    char                            m_cAuthLevel;
    
    char                            m_cPCBangType;
    char                            m_cUserBenfitType; // 태국 3BB 유저 추가.

    bool                            m_bIndigoDataExist; // DB에 indigo 데이터가 확인되면 true.
    KCacheData<int>                 m_iRP;

    // 051021. florist. 신규유저, 오늘자 첫 접속 여부를 기억한다. 아이템 증정 이벤트등에 사용.
    int         m_iSessionInfo;

    std::map< char, __int64 >       m_mapInitExp;        // 100807. 유저의 초기 경험치. 통계를 위해
    std::map< char, __int64 >       m_mapTotalInitExp;   // 110117. 유저의 누적 초기 경험치. 통계를 위해.
    DWORD       m_dwPlayCount[GC_CHAR_NUM];  //051006. microcat. 유저가 게임에 접속한 동안 게임을 한 횟수
    bool        m_bIsFinalQuest;             //060109. microcat. 파이날 퀘스트를 하였는가

    int         m_iP2PVersion;      // p2p protocol version. 서로 다른 유저들과는 게임할 수 없다.

    std::map< char, std::map<int,KDungeonUserInfo> > m_mapDifficulty;  // 난이도가 있는 게임모드의 클리어정보
    std::map<char,KncBitStream>     m_mapOldDifficulty;

    bool        m_bServerMigrationReserved;        // 060404. kkurrung. 서버간 이동하는 경우 true로 셋팅된다.

    //BYTE        m_abyteGUIDReqInfo[SIZEOF_GUIDREQINFO];     // 060413. jseop. hshield GUIDReqInfo 구조체.
    //DWORD       m_dwCRCInfo;                                // 060413. jseop. hshield crc info의 포인터.
    //BYTE        m_abyteReqInfo[SIZEOF_REQINFO];             // 060413. jseop. hshield ReqInfo 구조체.

    std::vector<KMissionData>   m_vecMissionSlot;           // 060413. kkurrung. 미션 슬롯, SiKMissionManager를 항상 참조한다.
    std::set<DWORD>             m_setCompleteMission;       // 120418. tgkwon. 완료된 미션 ID를 가지고 있는다.

    DWORD                       m_dwAuthType; // 유저의 인증 타입.
    KncBitStream                m_bitCheckPoint;
    bool                        m_bRecvLoadingComplete;

    char                        m_cRecommendUser; // 060822. kkurrung. 추천인

    // 061030. kkurrung. 게임 제한 을 위한 누적시간 저장.
    int                         m_nAccOfflineTime;
    int                         m_nAccOnlineTime;

    char                        m_cCurrentCharacter; // 061117. kkurrung. 현재 선택된 캐릭터를 나타낸다.

    // 061229. kkurrung. Pet관련 데이터
    std::map< GCITEMUID, KPetInfo >             m_mapPetInfo;
    std::map< GCITEMUID, KPetInfo >             m_mapDepotPetInfo; //  2012-1-30 woosh, 창고에 있는 펫 정보


    // 070306. kkurrung. 신학기 이벤트용 계정 타입 저장 변수
    int                         m_nRecommendKRType;
    int                         m_nHackReport; // 해킹 정보를 DB에 쓰기 위해서 

    // 070419. kkurrung. 포인트 시스템.
    KUserGCPoint                m_kGCPoint;
    int                         m_nPointRecomCount; // 포인트 

    // 출석부
    KCalendar                   m_kCalendar;

    CTime                       m_tmFirstLoginTime;
    CTime                       m_tmLastLoginTime;

    bool                        m_bGMuser; // 070504. woosh. 운영자툴 접속이면 true, 달력부분 로그 막기 위함

    int                         m_nReportCount;

    // 보너스 포인트 정보.
    std::map< char, KCacheData<int> >             m_mapBaseBonus; // character 별 bonus life
    std::map< char, KCacheData<int> >             m_mapSpecialBonus;

    // 퀵슬롯 정보.
    std::map< char, KQuickSlot > m_mapQuickSlot; // [char, QuickSlot]

    // 광장 관련 데이터.
    KSquarePtr                  m_spSquare;
    // 대기 중일때 광장에 들어오거나 나간 유저.쩜 챙겨야 할듯.. ㅜㅜ..
    std::vector<KSquareUserInfo>          m_vecEnterUser;
    std::vector<DWORD>          m_vecLeaveUser;

    KSquarePos                  m_kSquarePos;

    // 080222. kkurrun. 초대 거부.
    bool                        m_bInviteDeny;
    bool                        m_bIsObserver;

    KEVENT_BAD_USER_INFO_NOT    m_kBadUserInfo;

    // 080403. kkurrun. 나의 랭킹 정보
    KMyRankInfo                 m_kRankInfo;

    int                         m_nVirtualCashPoint;
    //bool                        m_bRecvP2PPoorListACK;
    //bool                        m_bSendP2PPoorListREQ; // p2p 대기자 목록 요청 여부
    //bool                        m_bP2PComplete;        // p2p 연결 완료
    //bool                        m_bRoomUserLoadComplete; // 방원 전체 리소스 로드 성공여부

    std::vector<KCollectionData>     m_vecCollectionSlot;           //  080526  woosh. 칭호 컬렉션

    KUserEventChecker           m_kEventCheck;  // GSDB스레드 작업 보호용
    std::vector<KMiniGameRankInfo>  m_vecMiniGameInfo; //  080630  woosh. 유저의 미니게임 정보

    DWORD                       m_dwStartMiniGameDuration; //  080716  woosh. 미니게임 플레이 시간
    DWORD                       m_dwLastMiniGameDuration; //  080716  woosh. 미니게임 플레이 시간
    bool                        m_bExpAccount; //  080723  woosh. 체험 계정이면 true

    // 스킬트리, 구버전.
    std::set<int>                                               m_setAddReserveSkill;
    std::set<int>                                               m_setDeleteReserveSkill;
    std::map<std::pair<char,char>, KSPInfo >                    m_mapTrainedSkill;  // <[char type, job], skills>
    std::map<std::pair<char,char>, std::vector<KSPSetInfo> >    m_mapInitSkillSets;
    std::map<std::pair<char,char>, std::vector<KSPSetInfo> >    m_mapSkillSets;
    std::map<char, std::set<int> >                              m_mapUnLockGroupID;
    std::map<char, int>                                         m_mapSlotOpenInfo; // [ charType, SlotIndex ]

    std::map< int, std::set< std::pair< DWORD, DWORD > > >                      m_mapGachaSetObtained; // 가차 풀 세트 보상 { Version, { GachaItemID, Type } }
    std::map< int, std::map< std::pair< DWORD, int >, std::set< DWORD > > >     m_mapGachaSetObtainedLevel; // 가차 풀 세트 보상 { Version, { < GachaItemID, Level >, Type } }

    int                 m_nGachaKeyBuyCount;    // 오늘 구매한
    int                 m_nGPKeyBuyCount;       // 오늘 구매한
    // 가챠 릴(reel)방식 누적점수(DB저장됨) { Version, { { GachaItemID, Type }, { Level, ReelPoint } } }
    std::map< int, std::map< std::pair< DWORD, DWORD >, std::map< int, int > > >    m_mapGachaReelPoint;        // DB에서 읽어오고 릴포인트 계산시 참고하며 변경시 갱신됨
    std::map< int, std::map< std::pair< DWORD, DWORD >, std::map< int, int > > >    m_mapGachaReelPointDiff;    // 릴포인트 변경시 갱신되며 DB에 기록할 때 사용함

    std::vector<int> m_vecPeriodNoticeUID; //  081013  woosh. 주기있는 전체공지 (DB저장) , 클라이언트로 전송

    // 커플 관련
    KCoupleInfo                 m_kCoupleInfo;
    std::vector<KCoupleEquip>   m_kInitEquip;
    int                         m_nCoupleWindowState;
    // 커플 신정 중인 경우 요청 했던 반지 아니템을 임시로 저장해둔다.
    //KSimpleItem                 m_kTmpRingItem;

    // 기부 시스템.
    int                         m_nAccDonationPoint;
    bool                        m_bLunaNewYearEventDone; // 오늘 세배 여부.
    // 09.04.11. kkurrung. 추천(5th)
    KRecommenderInfo                m_MyRecomInfo;
    std::vector<KRecommenderInfo>   m_vecRecommenders;
    int                             m_nRecomBonus;
    std::vector<DWORD>          m_vecOnlineFriends;

    std::wstring                m_wstrCCode; // Country Code;
    std::vector<KInviteUserInfo> m_vecFriendInviteList;
    int                         m_nHackingCount; // 누적해킹횟수

    MAP_INT_DWORD               m_mapMySubscriptionGiftCount; // 응모이벤트에서 획득한 경품 개수
    KUserBoardInfo              m_kUserBoardInfo; // 유저의 보드게임 정보.
    CTime                       m_tmDiceLastTime; // 마지막으로 주사위 받은 시간.
    CTime                       m_tmInitDiceTime; // 주사위 이벤트 초기화할 시간 저장.

    // Guild
    std::wstring                m_strGuildSearchKeyword;    // 검색한 길드키워드
    std::vector<DWORD>          m_vecGuildSearchUIDList;    // 검색한 길드ID목록
    int                         m_nGuildSearchTotalPageNum; // 검색한 길드리스트 총페이지
    DWORD                       m_dwGuildSearchTick;        // 길드검색Tick
    DWORD                       m_dwGuildSearchTickGap;     // 길드검색 주기
    // 09.08.16. kkurrung. FB LogOut Process..
    std::wstring                m_strPasswd;

    // 091118. kkurrung. 퀴즈 빙고
    KBingoUserData              m_kQuizBingo;
    // 090820 tslayer. 광장 광고별 노출 카운트(DB저장됨)
    std::map< int, DWORD >      m_mapAdvertisingExposureCount;

    std::pair<int,int>          m_prRecomLevelUpCheck; // 반갑다 친구야 신규유저 아이템 지급받았는지 체크. pair<달성레벨,받아간보상레벨>
    bool                        m_bReturnUserCheck;   // 반갑다 친구야 복귀 유저인지 체크.

    // 100112 tgkwon. 채널링 추가.
    DWORD                       m_dwChannellingType;     // 채널 타입( 넷마블 : 0 / 투니버스 : 1 )
    std::string                 m_strLoginID;       // 채널링체크를 위해서 로그인 ID 추가.

    // 2010.02.05. kkurrung. Item Breakup
    int                         m_nItemBreakupReelPoint; // 아이템 해체 누적 포인트
    // 특정 시간간격으로 아이템 지급
    KTimeDropCheck              m_kTimeDrop;
    KClientPing                 m_kPingInfo;
    KncTimer                    m_kPingTimer;

    // 대기룸에서 상태 알림 
    KPressureState              m_kPressInfo;   // 재촉 하기.
    int                         m_nIdleState;   // 자리 비움.

    // 룩
    std::map<char,KLookEquipChar> m_mapLookEquip; // <캐릭터 타입, 각 캐릭 룩 장착>
    std::map< int, KHeroPlayData > m_mapHeroDungeonClearInfo; // 영웅던전 플레이데이터

    std::map<USHORT,USHORT>     m_mapChannellingTypeCode; // 채널링타입별 채널코드

    // 가위바위보
    KUserGawibawiboInfo         m_kGawibawiboInfo;

    // 스테이지 로딩 완료
    bool                        m_bRecvStageLoadComplete;
    // 레벨업 드랍 정보
    KEVENT_SHANGHAI_DROP_REQ    m_kLevelUpDropInfo;

    // 2010 크리스마스 양말
    std::map< GCITEMID, KSocksInfo > m_mapSocksInfo;
    KncTimer m_kSocksTimer;

    // 캐릭터타입, pair<잠김여부,ItemUID>
    std::map<char,std::pair<bool,GCITEMUID> > m_mapChangeWeapon;

    int                         m_nIncGpSum; // 획득 GP 총량
    int                         m_nDecGpSum; // 소비 GP 총량
    // 110222. tgkwon. 나무나무 이벤트 
    KPlantTreeUserData          m_kPlantTreeInfo;
    CTime                       m_tmVirtualEnableDate; // 남미 미국 유저 가상 캐시 사용 일자.
    // 두근두근 마법상자 [ItemID, [남은회수,클리어회수]]
    std::map<GCITEMID, std::pair<int,int> > m_mapMagicBoxInfo;
    KSphinxUserData             m_kSphinxUserData; // 스핑크스 OX게임 유저 데이터.


    DWORD                       m_dwEnterSquareTime;
    DWORD                       m_dwLeaveSquareTime;
    KPartyData                  m_kParty; // 유저의 파티 정보

    DWORD                       m_dwStayAgitUID; // 현재 머물고 있는 아지트UID
    UCHAR                       m_ucAgitOpenType; // 현재 머물고있는 아지트의 공개 설정 (KAgitInfo::OPEN_TYPE)
    std::wstring                m_strAgitPassword; // 비공개시 접속암호
    int                         m_nTodaySeedDropCount; // 오늘 획득한 씨앗의 개수
    std::set<DWORD>             m_setAgitBanList; // 추방당한 아지트 목록(AgitUID)
    std::map<int,float>         m_mapFairyTreeFruitBuff; // 요정의나무 열매버프 map[ 버프타입, 버프증가량 ]
    UCHAR                       m_ucAgitInviteOption; // 아지트 초대설정 (0:거절,1:전체수락,2:친구만 수락)
    bool                        m_bAgitNewUser; // 아지트 입장안해본 유저

    int                         m_iAge; // 유저 나이
    __int64                     m_biUniqueKey; // 인증서버로부터 받은 고유키 값.
    std::string                 m_strNMUserCN; // 사용자의 넷마블 CN
    bool                        m_bNMNetcafeActive; // 넷마블PC방 혜택여부
   // 유저의 언어코드
    int m_nLanguageCode; // 유럽 다국어 지원용, 공통으로 사용할 수도 있음

    KSurveyUserData             m_kSurveyUserData;
    int                         m_nUniqueKey; // 미국 인증 사용.
    KCharismasUserData          m_kCharismasUserData; // 크리스마스 이벤트 유저 Data.
    KUserLetterBox              m_kUserLetterBox;
    MapUserCharLvPlayTime       m_mapUserCharLvPlayTime; // 유저캐릭터별 레벨구간당 레벨업시간 통계
    DWORD                       m_dwCharLvStartGameTime; // 캐릭터레벨기준으로 대전/던전 게임플레이 시간시간(ms)

    // 유저 장착정보 변경된 캐릭터
    std::set<DWORD>             m_setChangeEquipInfo;

    // 개기일식의 음모. 20111220. woosh
    KncTimer                    m_kEclipsePlotTimer;
    KEclipsePlotData            m_kEclipsePlot;
    bool                        m_bEclipseCollectComplete;

    std::map<char,USHORT>       m_mapPvpCategoryHackCount; // 대전 해킹횟수

    std::map< char, KDepotInfo > m_mapDepotInfo; // tab , depotInfo // 공용창고, 캐릭터 구분 없음.
    KSongkranUserData           m_KSongkranUserData;
    KAdventureUserData          m_kAdventureUserData;
    KOlympicUserData            m_kOlympicUserData;
    bool                        m_bAdult; // 중국 성인
    //std::map< char, std::set<char> > m_mapCharPromotion; // 캐릭터, 전직 담을 변수. 201205 woosh

    char                        m_cLastSelectCharType;
    DWORD                       m_dwChangeCharTypeCnt;
    std::set<char>              m_setCharLevelEvent; // 캐릭터 특정 레벨 보상 이벤트.	
    std::map<char,std::pair<bool,bool>>     m_mapLevelResetCharInfo;   // 레벨 초기화한 캐릭터 정보( charType, ( cash여부, 보상여부 ) )
    CTime                       m_tmFinalLoginTime; // 마지막 접속 시간
    bool                        m_bFirstLoginToday;

    std::vector<KMatchInviteUserInfo> m_vecMatchPartyInviteList;
    std::map<int, std::vector<KPartyMember> > m_mapMatchParty; // 유저의 매칭 파티 그룹 정보.
    DWORD                       m_dwMatchStartTick; // 유저 매칭 시작 시간.
    DWORD                       m_dwMatchEndTick; // 유저 매칭 거절&종료 시간.
    KInDoorUserInfo             m_kMatchIndoorUserInfo; // 매칭시의 사용하는 방원 정보 객체.
    KELOPenaltyInfo             m_kELOPenaltyInfo; // 매칭 유저 패널티 정보.
    int                         m_nMatchSuccessLoopCount;
    KRitasChristmasUserData     m_kRitasChristmasUserData;

    KMyMatchRankInfo            m_kMatchRankInfo;

    bool                        m_bDonationBuffEnable; // 발렌타인 기부를 한번이라도 하면 true이고, 버프를 준다
    bool                        m_bDataReceive;
    KAccumulateAttendanceUserData m_kAccumulateAttendanceUserData;

    KErrandUserData             m_kErrandUserData; // 심부름왕 유저 데이터
    KncTimer                    m_kErrandTimer;
    std::set< int >             m_setNonGraduateCharacters; // 캐릭터 전용 던전 관련 남은 미졸업 캐릭터들(CharType)

    KSystemGuideUserData        m_kSystemGuideUserData; // 시스템 가이드 유저 데이터
    bool                        m_bTutorialEnable; // 유저 튜토리얼 진행 여부.
    std::map<GCITEMUID, DWORD>  m_mapAttributeMigrationItemInfo; // 속성 재설정한 아이템 정보.
	std::vector<KInDoorUserInfo> m_vecIndoorUserTemp;
	
    KncTimer m_kTimerCYOUHeartbeat; // [2012/11/6 woosh]. 중국 유저 하트비트(1시간)
    std::string         m_strBillingAccount; // 중국 창유 빌링에 저장된 계정으로 
    bool m_bFirstGetCyouPoint; // 중국 창유 웹 포인트 조회 기록. 초기값은 true, 조회했으면 false 가 되고 E2 프로토콜 재요청하지 않는다.
    DWORD                       m_dwUserRemainIndexCnt; // 유저 캐릭터 추가 가능 
    KVitalityUserData           m_kVitalityUserData; // 활력(피로도) 시스템 유저 데이터
    bool                        m_bCYOULogined; // 중국 로그인상태(최초) 체크용. A3에 대한 응답이 정상으로 온 경우

    KConnectionGiftBoxUserData  m_kConnectionGiftBoxUserData; // 접속 선물상자 시스템 유저 데이터

    bool                        m_bJumpCharEnable; // 점핑 캐릭터 진행여부 체크.	

    std::vector< KItem >        m_vecVirtualDepot; // 캐릭터 인벤 분리 이 후 마이그레이션용 가상창고 아이템 정보

    std::map< time_t, std::map< DWORD, DWORD > > m_mapTongDonationUserAmount; // 통합 기부 NPC별 개인 기부 누적량( key: tmVersion, < NPCType, Amount > )
    USHORT                      m_usUDPUsePort; // UDP Port.
    std::vector<KInvenDepotMoveLog>              m_vecInvenDepotMoveLog;

    std::vector< DWORD >          m_vecFinishedMission; // 클리어 한 미션 리스트

    std::map< char, std::map< int, std::map< int, KDungeonPersonalRecord > > >  m_mapDungeonPersonalRecord; // 개인 최고 기록 정보, < < CharType, < ModeID, < StageNum, DungeonPersonalRecord > > >
    std::map< char, std::map< int, std::map< int, KDungeonPersonalRecord > > >  m_mapDungeonPersonalRecordDiff; // 개인 최고 기록 정보 Diff. 접종, 캐릭터 변경시 DB 저장용으로 사용함
    std::map< PAIR_CHAR_INT, KDungeonRankInfo >                                 m_mapPreviousDungeonRankPersonal; // 이전 시즌 유저 랭크, < ( CharType, ModeID ), DungeonRankInfo >

protected:  // util function, Packet Handling
    virtual void ProcessEvent( const KEventPtr& spEvent );
    virtual void OnDestroy();   // LOGINED 상태로 빠져나오고, DB data update를 신청한다 
    virtual void OnAcceptConnection();

    void DisconnectAllFromAttack(unsigned int ipaddr);

    unsigned int GetTick( int nIndex )  { return m_auiTickCount[ nIndex ]; }
    void SetTick( int nIndex )          { m_auiTickCount[ nIndex ] = ::GetTickCount(); }
    bool DBUpdateUserData();
    void GetDBUpdateData( OUT KUserData& kUserData );
    void GetDBUpdateExpData( OUT KEVENT_EXP_UPDATE_TO_DB_NOT& mapData );
    void GetDBUpdateData( OUT KDB_EVENT_EXIT_GAME_NOT& kNot );
    void GetDBUpdatePetData( KPetData& kPetData );

    bool ForceTranslateState( int nWantedState, bool bDestroy = false );       ///< 성공적으로 변경 되었을 때 return true.

    // 050516. jseop.
    bool AdjustExpGP( IN const int& nPlayType, IN const KGameInfo& kGameInfo, IN OUT KEndGameReq& kReq, OUT KEndGameAck& kAck );
    bool CheckConditionDropItem( KUserPtr spUser, const KDropItemDetail& vecDropItem , const KDropItemInfo& kDropItemUnit, const KEndGameReq& kReq ); // 아이템 받기에 적합한지 조건체크 함수
    bool CheckConditionItemDropByLevel( KUserPtr spUser, const KDropItemDetail& vecDropItem_ );

    bool DivideEachShareOfDropItem( IN const KEndGameReq& kReq,
                                    IN OUT KInsertDropItem& kInsertDropItem,
                                    IN OUT bool& bShDrop,
                                    OUT bool& bResultDrop );

    void ProcessPetReward( IN const KGameResultIn &kResultIn, OUT KGameResultOut &kResultOut );

    void AdditionalDropProcess( KUserPtr spUser, KDropItemInfo& kDropItemUnit, KEndGameAck& kAck );

    bool AdditionalDrop( IN const KEndGameReq& kReq, OUT std::map<DWORD, DROP_ITEMS>&  mapEachUserDrops );

    void UpdateMissionInfo( KEndGameAck& kPacket ); // 미션 정보를 ACK에 넣어 준다.
    bool SetIndigoInfo( IN const KIndigoData& kIndigoData );
    void CheckLoadingComplete(); //게임 시작하고 컨넥션 맺는시간동안 너무 오래걸리면 튕겨낸다.
    bool CheckGameMode( int nGameMode );

    // calendar 관련
    void UpdateAccTime();

    // 070905. kkurrung. 장착 관련.
    bool ChkEquipExist( IN std::vector<KEquipItemInfo>& vecEquip );
    bool CopyEquipReq( IN std::map<char,KChangeEquipment>& mapEquipReq,
                         OUT std::map<char,std::vector<KEquipItemInfo> >& mapEquipCopy );

    bool ApplyEquipItem( IN std::map<char,std::vector<KEquipItemInfo> >& mapEquipCopy,
                         IN std::map<char,KChangeEquipment>& mapEquipReq );
    int CheckValidationReleaseReq( IN OUT std::map<char,KChangeEquipment>& mapEquipReq_ ); // 장착 요청 아이템중 해제하려는게 나에게 없으면 제거.
    int CheckValidationEquipReq( IN OUT std::map<char,KChangeEquipment>& mapEquipReq ); // 장착 요청 아이템중 장착하려는게 나에게 없으면 제거.
    bool ReleaseEquipElement( DWORD dwUID, char cItemType );

    // 게임 시작시 드랍 관련 처리.
    void CheckDrop( KEVENT_START_GAME_BROAD& kPacket );

    //아이템 검색 도움
    //아래는 센터 룸 때문에 필요 함.
     const KItem* FindItemByID( IN const std::vector<KItem>& vecItem_, GCITEMID dwItemID_ );

     KMissionData* GetMissionDataFromSlot( DWORD dwMissionID );
    bool IsRegistedMission( DWORD dwMissionID );
    bool EraseMissionDataFromSlot( DWORD dwMissionID );
    bool GetMissionListOfType( IN const int& nMissionType, IN const char& cCharType, OUT std::vector<DWORD>& vecMissionList );

    void DumpMissionSlot();

    // 보너스
    void CheckBonusRefill();
    int GetCurrentBonusCount() { return m_mapBaseBonus[m_cCurrentCharacter] + m_mapSpecialBonus[m_cCurrentCharacter]; }
    bool ReduceBonusCount( int nCount = 1 );
    void GetBonusInfo( IN const char cCharType_, OUT KBonusPointInfo& kBonusInfo );

    // 퀵슬롯.
    bool CheckValidateSlot( IN OUT std::vector< KSlotData >& kQuickSlot, IN bool bEmoticon );
    bool IsInSlotByItemID( IN std::vector< KSlotData >& kQuickSlot, IN GCITEMID dwItemID );
    bool IsInSlot( IN std::vector< KSlotData >& kQuickSlot, KSlotData& kSlotData );
    bool ReleaseQuickSlot( GCITEMUID dwUID );

    // 통계
    void AddStatistics( IN KGameInfo& kGameInfo, IN KEndGameReq& kReq );
    void AddStatisticsMap();
    void AddStatisticsDeathMatch();
    void AddStatisticsMiniGame( IN KMiniGameRankInfo& kMiniGameInfo, IN DWORD nLevel, DWORD dwElapsedSecond );
    void AddStatistics2( IN const KGameInfo& kGameInfo, IN OUT KEndGameAck& kEndGameAck, IN const std::map<std::pair<int,int>, int>& mapStageMonKillCount );
    void AddStatisticsFatalDeathMatch();

	// ping 통계
	void AddStatisticsPing( KStatPingInfo& kStatPingInfo_ );
    void AddStatisticsAvgPing();

    // 대전 플레이 캐릭터 통계(게임 시작시 MP/AP/HB 몇 명씩 있는지)
    void AddStatisticsPvPPlayCharacters();

    // 더블 크리스탈 한방에 처리하자..
    void ProcessDoubleCristalCard( IN OUT KInsertDropItem& kPacket );

    // 광장 관련.
    void AddSquareNewUser( IN KSquareUserInfo& kInfo_ ); // 내가 로딩 중일때..
    void AddSquareLeaveUser( IN DWORD dwUID ); // 내가 로딩 중일때.
    bool LeaveSquare();
    void DumpSquareUserInfo( std::vector<KSquareUserInfo>& vecSquareUser );

    bool RollBackToSquare( KSquarePtr& spSquare );

    void DumpIndoorUserInfo( std::vector<KInDoorUserInfo>& vecInfo );
    void DumpIndoorUserInfo( KInDoorUserInfo& kInfo );


    char GetBestCharGrade();
    void GetMyRankInfo();
    void GetMyMatchRankInfo();

    void UpdateUserAuthLevel();

    void LeaveGameProcess(); // 갑작 스럽게 게임이 종료 될때 처리. 프로세스 킬.

    // 접속시 헬 아이템 체크
    void CheckHellItemConnection( int nSessionInfo );
    void CheckHellItemAttendance( IN int nCount );

    void UpdateCollectionInfo( KEndGameAck& kPacket ); //  080528  woosh. 컬렉션(칭호)
    void DumpCollectionSlot();
    void CheckAndRegistCollection( std::vector<KCollectionData>& vecCollectionSlot );
    KCollectionData* GetCollectionDataFromSlot( DWORD dwMissionID ); // 컬렉션 데이터 얻기

    void Indigo_Log( const KGameResultIn& kResult, const int nGetRp, const char* szIPStr );
    void End_Indigo_Log();
    void ServerTimeNot();
    void ChatNotice( std::wstring& strMsg );
    bool IsBestMiniGameRecord( IN OUT KMiniGameRankInfo& kMiniGameInfo ); //  080630  woosh. 미니게임 최고 점수인지 판단
    bool StartMiniGame(); //  080707  woosh.  광장에서 미니게임으로 옮겨가기 
    bool GetMiniGameBestScore( IN OUT KMiniGameRankInfo& kMiniGameInfo );

    // 스킬 관련
    bool IsTrainedSkill( SSkillElement& kSkill );
    bool IsTrainedPreSkill( SSkillElement& kSkill );
    bool ExistTrainedNextSkill( SSkillElement& kSkill );
    int GetLeftSpPoint( char cCharType );
    int GetUseSpCount( char cCharType, char cPromotion );
    bool AddTrainedSkill( SSkillElement& kSkill );
    bool RemoveTrainedSkill( SSkillElement& kSkill );
    bool RemoveReserveHandleSkill( std::set<int>& setReserveSkill, int nSkillID );
    bool AddReserveHandleSkill( std::set<int>& setReserveSkill, int nSkillID );

    bool VerifySetInfo( IN char cCharType, IN char cPromotion, IN KSPSetInfo& kSetInfo );
    bool CheckSettingID( IN std::map< std::pair<char,char>, int >& mapSetting );
    bool ApplyChangeSkillSet( KChangeSkillSet& kChangeSkillSet );
    void GetSkillSettingData( IN const char& cCharType, OUT std::map<char, std::vector<KSkillSlot> >& mapEquipSkillSet ); // indoorCharInfo
    void GetSkillSetEquip( std::map< std::pair<char,char>, int >& mapSetting );
    void AddNewSkillSet( char cCharType, char cPromotion );
    void DumpSkillSet( std::map<std::pair<char,char>, std::vector<KSPSetInfo> >& mapSkillSets );
    void DumpEquipSkillSet( std::wstring& strLogin, char cCharType, std::map<char, std::vector<KSkillSlot> >& mapEquipSkillSet );
    void AssignSkillSets( IN std::map<std::pair<char,char>, std::vector<KSPSetInfo> >& mapFrom,
                          OUT std::map<std::pair<char,char>, std::vector<KSPSetInfo> >& mapTo );
    int GetSpentSP( char cChar, char cPromotion );
    void MakeChangeSkillSet( KChangeSkillSet& kChangeSkillSet );
    bool IsInUnLockedGroup( char cCharType, int nLockGroupID );
    void AddUnLockedGroup( char cCharType, int nLockGroupID );
    void ReleaseEquipSkill( SSkillElement& kSkill );
    bool IsLockedGroup( SSkillElement& kSkill );
    void DumpSetSkillReq( IN const KChangeSkillSet& kInfo );
    void ResetEquipSkill( IN const char& cCharType );
    bool ResetTrainedSkill( IN const char& cCharType );
    
    void GetSkillInfoData( IN const char& cCharType, OUT std::vector<KSPInfo>& vecSPInfo );

    // 종료 로그 관련.
    void CalcDisconnReasonOnLogin( unsigned char ucReson );

    // 게임 포기 로그
    void AddStatLeaveGame( KGameResultIn& kResultIn );
    void UpdateGachaKeyBuyCount( int nCashDiff, int nGPDiff );
    void UpdatePeriodNotice();

    // 커플관련
    void InitCoupleInfo();
    DWORD GetCoupleState() { return m_kCoupleInfo.m_dwCoupleState; };
    void SetCoupleState( DWORD dwState ) { m_kCoupleInfo.m_dwCoupleState = dwState; };
    //void ClearTempRing();

    bool ChkMakeCoupleState( KUser* pkUser, int nUserState );
    void SetCoupleDeny( DWORD dwRequester = 0 );
    bool IsInCoupleInven( GCITEMID dwItemID );
    bool IsInCoupleInven( std::vector<GCITEMID> vecItemIDs );
    void AddMyInven( KSimpleItem& kItem );
    void AddPartnerInven( KSimpleItem& kItem );

    bool EquipCoupleItem( GCITEMID dwItemID, bool bPartner = false );
    bool EquipCoupleItem( std::vector<GCITEMID>& vecItemIDs, bool bPartner = false );
    DWORD GetPartnerUID()   { return m_kCoupleInfo.m_dwPartnerUID; };
    std::wstring GetPartnerNick()   { return m_kCoupleInfo.m_strPartnerNick; };
    void MakeCoupleInfomation( KEVENT_COUPLE_INFORMATION_ACK& kData );
    bool IsCouple();
    void AdjustCoupleData( KCoupleInfo& kInfo );
    void GetCoupleEquip( std::vector<GCITEMID>& vecCoupleEquip );
    void CoupleBoostCalc( IN OUT float& fExpBoost, IN OUT float& fGpBoost );
    void ChangeRing( std::vector<KCoupleEquip>& vecCoupleEquip, GCITEMID dwItemID, GCITEMUID dwUID );
    void GetCoupleChangeEquip( std::vector<KCoupleEquip>& vecDelEquip, std::vector<KCoupleEquip>& vecUpdateEquip );
    void CleanupCoupleData();
    void SendCoupleNotToSquareUser( SCoupleSignBoard& sBoardInfo );
    bool IsInDenyList( DWORD dwUID );
    void CoupleDivorceNotityToOther( KUser* pkUser, std::pair<DWORD,DWORD> prData );
    void AdjustCoupleEquipExpired();
    void ChkCoupleItemExpired( std::vector<KItem>& vecExpiredItems );
    void ConnectionExpiredCoupleItem();

    bool RemoveCoupleMyInven( GCITEMUID itemUID );
    bool RemoveCoupleMyEquip( GCITEMUID itemUID );
    bool FindItemFromCoupleInven( GCITEMID dwItemID, std::vector<KSimpleItem>& vecInven, KSimpleItem& kItem );
    void ClearEquipExRing( std::vector<KCoupleEquip>& vecEquips );
    void DumpCoupleInfo();
    time_t GetCoupleRegDate();
    void ClearCoupleDenyInfo(); // 신청 거절 리스트 청소.

    void DumpCoupleEquip( std::vector<KCoupleEquip>& vecEquip, std::wstring& strLogin );
    void DumpExpiredCoupleItem( std::vector<KSimpleItem>& vecExpiredItems, std::wstring& strLogin );
    void DumpCoupleInven();

    void CharacterCachingDataSync();
    void AddRelayLog( std::vector<DWORD>& kPacket_, DWORD dwUID_ );
    void DumpAutoMission( IN const std::wstring& strFunc, IN const std::vector<DWORD>& vecAuto );

    bool IsNewUser() { return (m_iSessionInfo == SI_NEW_ACCOUNT); };

    // 09.04.11. kkurrung. 추천 (5h)
    void UpdateRecom5th( IN const KSimpleDate& kAttendDate );
    void SendMissionPackListNot();
    void SendHellTicketFreeModeNot();

    // 09.07.28. kkurrung. 친구 초대.
    void GetFriendInviteList( OUT std::vector<KInviteUserInfo>& vecInviteList, IN char cChannelType );
    // Helper
    void FirstNotEvents();
    void DumpDropData( IN std::vector<KGameDropElement>& vecDrop );
    int GetGachaReelPointSum( IN const int nVersion_, IN const GCITEMID dwItemID_, IN const DWORD dwType_, IN const int nLevel_ );
    void SetGachaReelPointSum( IN const int nVersion_, IN const GCITEMID dwItemID_, IN const DWORD dwType_, IN const int nLevel_, IN const int nReelPoint_ );
    bool GetGachaNotExistList( IN const int nVersion_, IN const GCITEMID dwItemID_, IN const DWORD dwType_, OUT std::map< int, GCITEMID >& mapNotExistList_ );
    bool GetGachaNotExistList_Level( IN const int nVersion_, IN const GCITEMID dwItemID_, IN const DWORD dwType_, IN const int nLevel_, OUT std::map< int, GCITEMID >& mapNotExistList_ );
    int CalcGachaReelPoint( IN const int nVersion_, IN const GCITEMID dwItemID_, IN const DWORD dwNotExistListSize_, IN const float fRate_ );
    bool CheckGachaObtained( IN const int nVersion_, IN const GCITEMID dwGachaItemID_, IN const DWORD dwType_ );
    void InsertGachaObtained( IN const int nVersion_, IN const GCITEMID dwGachaItemID_, IN const DWORD dwType_ ); 
    bool CheckGachaObtained_Level( IN const int nVersion_, IN const GCITEMID dwGachaItemID_, IN const DWORD dwType_, IN const int nLevel_ );
    void InsertGachaObtained_Level( IN const int nVersion_, IN const GCITEMID dwGachaItemID_, IN const DWORD dwType_, IN const int nLevel_ );
    bool CheckGachaObtainedByType( IN const int nVersion_, IN const DWORD dwType_ );
    bool CheckGachaObtainedByType_Level( IN const int nVersion_, IN const DWORD dwType_ );

    void SendExitInfoToCenter();

    // 09.09.09. tgkwon. Mission Event 
    void CheckMissionEvent();
    // 광장 NPC클릭시 아이템받기 이벤트 리스트 전달
    void SendNpcGiftsNot();
    void InitUserBoardInfo( IN int nOK, IN OUT KUserBoardInfo& kUserBoard );    // 유저의 보드게임 정보 초기화 여부 체크.
    void CheckUserBoardInfo( IN OUT KUserBoardInfo& kUserBoard );   // 하루가 지났는지를 체크.
    void SetUserBoardInfo( IN KUserBoardInfo kUserBoard );          // 유저 보드게임 정보 세팅.
    void GetUserBoardInfo( OUT KUserBoardInfo& kUserBoard );        // 유저 보드게임 정보 가져오기.

    int RecomLevelUpCheck( IN const int nOldLv, IN const int nCurrentLv, IN const int nRewardLv_, IN OUT std::vector<KDropItemInfo>& vecItem ); // 반갑다 친구야 레벨업시 아이템 지급 여부 체크.

    //  091102  woosh. 
    void UpdateIndigoInfo( IN const KGameResultIn& vitResultIn_, IN int nGameMode_, IN bool bWin_, KUserPtr& spUser, OUT KGameResultOut& kResultOut_ );
    void UpdateGuildPoint( IN const KGameResultIn& vitResultIn_, IN int nGameMode_, KUserPtr& spUser, OUT KGameResultOut& kResultOut_ );
    void UpdateDifficultyGameMode( IN const KGameResultIn& vitResultIn_, IN int nGameMode_, IN int nDifficulty_, KUserPtr& spUser, OUT KGameResultOut& kResultOut_ );

    //  091104  woosh. 
    bool GetChatExpGP( OUT bool& bActiveChatEvent_, OUT float& fChatBoostExp_, OUT float& fChatBoostGP_ );
    bool ShortGameCheck( KUserPtr& spUser, IN bool bWin_, OUT bool& bRet_, OUT KGameResultOut& kResultOut_, OUT KEndGameAck& kAck_ );

    void CalcStageClearRatio( IN int nGameMode_, IN int nDifficulty_, IN size_t sizeOfClearStage_ , OUT float& fStageClearRatio_ );
    void UpdateWinLoseRecord( IN int nGameMode_ , IN bool bWin_, IN int nUserCount_, IN char cCharType_, 
        OUT int& iWin_, OUT int& iLose_, OUT KGameResultOut& kResultOut_ );
    void SetInitValue_KCharacterExpInfoOut( OUT KCharacterExpInfoOut& kOutData_, IN const KCharacterInfo* kCharacterInfo_ );
    void PVP_NKillEvent( IN const std::vector<KGameResultIn>& vecGameResult_, OUT std::vector< std::wstring >& vecStrLogin_ );

    void ClearUsedQuantityItem( IN const std::vector<KGameResultIn>& vecGameResult_ );
    void ClearUsedQuantityItem();
    void DecreaseHellModeTicket( IN const std::vector<KGameResultIn>& vecGameResult_ );
    void NotifyPremiumToRoomUsers();

    void DBUpdateIndigo();
    void ExitStatistics();
    void GashLogOut();
    void AddStat();
    void AddStat_Zombie_SendBuffer();

    void CenterChat( KChatData& kPacket_ );
    void FillBuyerData( IN OUT KBuySellItemReq& kBuyInfo_ );
    void AdjustBoostInfo( IN OUT KGameResultIn& kResultIn );
    void CalcGPBoostInfo( IN OUT KGameResultIn& kResultIn, IN const float fStageClearRatio );
    void CalcCharacterBoostInfo( IN const int nModeID_, IN KUserPtr& spUser, IN OUT std::vector< KCharacterExpInfo >& vecCharExp, IN const float fStageClearRatio );
    void AdjustResultInAndOut( IN const KGameInfo& kGameInfo, IN OUT KGameResultIn& vitResultIn, IN const __int64 biMaxExp, IN OUT KGameResultOut& kResultOut );

    bool GetItemBreakupCost( IN const KItem& kItem, OUT int& nBreakupCost );

    void UpdateBuyCheckItemCount( IN const GCITEMID dwItemID );
    bool PacketHackCheck( IN const int nFrom );
    void CheckLowPing();

    void ClearPressInfo();
    void SetPressInfo( IN const int nState_ );
    void GetPressInfo( OUT int& nState_, OUT DWORD& dwElapsedTime_ );

    void ClearIdleInfo();
    void SetIdleInfo( IN const bool bIdle_ );
    void GetIdleInfo( OUT int& nIdleState_ );

    void SetEventMonsterInfo( IN const int nModeID, IN const int nDifficulty, OUT std::vector<KEventMonster>& vecMonster, OUT std::vector<KDropItemInfo>& vecPartyRewardInfo );
    bool DropDecision( IN const float fRate );
    void SetEventMonsterDropItem( IN KEventMonInfo& kEventMonsterInfo, IN KEventMonster& kEventMonster, OUT std::vector<KEventMonster>& vecEventMonster );

    void AddEventMonsterDrop( IN const std::vector<KEventMonster>& vecEventMon, IN const std::vector<DWORD>& m_vecUserUIDs, OUT std::vector<KGameDropElement>& vecDrop );
    void AddEventMonsterTrigger( IN const std::vector<KEventMonster>& vecEventMon, IN OUT std::vector<sTriggerInfo>& vecTriggers );
    void AddEventMonsterChampion( IN const int nDifficulty, IN const std::vector<sTriggerInfo>& vecTriggers, IN const std::vector<KEventMonster>& vecEventMon, IN OUT std::vector<KChampionInfo>& vecChampions );
    void DumpEventMonsterInfo( IN const std::vector<KEventMonster>& vecEventMonInfo );

    bool AddStatInitExpInfo( IN const char cChartype_, IN const __int64 biInitExp_, IN const __int64 biTotalInitExp_ );

    // kkurrung 룩 변환
    bool IsEquipedLookItem( IN const GCITEMUID itemUID_ );
    bool IsLookItem( IN const std::set<GCITEMUID>& setLookItems_ );
    void ReleaseCurrentLookEquip( IN const std::map<char,KCoordiDiffInfo>& mapChangeEquip_ );
    void EquipCurrentLookEquip( IN const std::map<char,KCoordiDiffInfo>& mapChangeEquip_ );
    void ExtractCurrentLookUID( OUT std::map<char, std::set<GCITEMUID> >& mapLookEquip_ );
    void ExtractCurrentLookItem( OUT std::map<char, std::vector<KEquipItemInfo> >& mapLookEquip_ );
    void ExtractCurrentLookItem( IN const char cCharType_, OUT std::vector<KEquipItemInfo>& vecLookEquip_ );
    void ExtractItemFromItemUID( IN const std::set<GCITEMUID>& setItemUID_, OUT std::vector<KEquipItemInfo>& vecItemID_ );
    void ReleaseLookEquip( IN const GCITEMUID iteUID_ );
    void ExtractDiffLookEquip( OUT std::map<char, std::map<int,KCoordiDiffInfo> >& mapDiffLookEquip );

    // kkurrung 묶어 팔기
    bool CheckBundleSellDuplicate( IN const KEVENT_BUNDLE_SELL_ITEM_REQ& kPacket_ );
    void VerifyBundleSellCount( IN OUT std::vector<std::pair<GCITEMUID,int> >& vecSellCount_, IN OUT std::map<GCITEMUID,int>& mapFailInfo_ );
    void VerifyBundleSellNormal( IN OUT std::vector<GCITEMUID>& vecSellNormal_, IN OUT std::map<GCITEMUID,int>& mapFailInfo_ );
    void VerifyBundleSellErase( IN OUT std::vector<GCITEMUID>& vecEraseList_, IN OUT std::map<GCITEMUID,int>& mapFailInfo_ );
    int GetBundleSelItemErrNo( IN const int nNetErrCode_ );
    void BundleSellExtractSellCountItems( IN const std::vector<std::pair<GCITEMUID,int> >& vecSellCountItems_, OUT std::vector<std::pair<KItem,int> >& vecExtractSellCountItems_ );
    void BundleSellExtractSellNormalItems( IN const std::vector<GCITEMUID>& vecSellNormalItem_, OUT std::vector<KItem>& vecExtractSellNormalItem_ );
    void RemoveItemFromInven( IN const std::vector<GCITEMUID>& vecItems_);
    void SendHeroDungeonList();

    void SendExpUpdateToDBNotice();
    void CheckServerListByAuthLevel( IN OUT std::vector< KServerInfo >& vecList );

    // 가위바위보
    void UpdateGawibawiboPoint();
    void InitGawibawiboData( int nDayChange_ );

    void RetryInit();

    void SetInitGawibawiboPosition();
    void SetRetryPosition();
    void DeleteRetryPosition();
    void AddGawibawiboTurnCount();
    void MoveGawibawiboCurrentPosition();
    void AddGawibawiboPlayCount();
    void AddGawibawiboRetryPoint();

    // PT에 대한 시간 저장.
    void AddPlayTime( IN const DWORD& dwTime, IN const DWORD& dwModeType );
    DWORD GetPlayTime(){ return m_dwPlayTime; };

    void SetStageLoadComplete( bool bFlag_ ) { m_bRecvStageLoadComplete = bFlag_; }
    bool GetStageLoadComplete() { return m_bRecvStageLoadComplete; }
    void CheckStageLoadComplete();

    // 경험치 보정
    bool CheckExpCalcErr( IN const USHORT& usCase, IN const int& nModeID, IN const DWORD& dwMissionID_, IN const char& cCharType, IN const int& nTotalRecvExp, IN OUT __int64& biCalcExp );
    void CalcMaterialsByFactor( IN const USHORT& usFactor, IN OUT std::vector<KDropItemInfo>& vecMaterials );
    void SetEndGameResultGPInfo( IN const std::vector<KGameResultIn>& vecGameResult );
    void CalcRewardType( IN const int& nModeID, OUT int& nRewardType, IN OUT std::vector<KGameResultOut>& vecGameResult );
    void GetCurruntCharType( IN const std::map<DWORD, std::vector<std::pair<char,char> > >& mapCharType, IN OUT std::vector<KRewardInfo>& vecReward );
    bool IsSpecialDropCharacterBox( IN const int& nDifficulty );
    bool CheckCharacterType( IN const DWORD& dwUID, IN const std::map<DWORD, std::vector<std::pair<char,char> > >& mapCharJob, IN const char& cCharType );
    void GetCharExpReward( IN const std::vector<DWORD>& vecUID, IN const std::map<DWORD, std::vector<std::pair<char,char> > >& mapRoomChar, OUT std::map< DWORD, KCharExpReward >& mapInfo );
    void DumpDungeonDropInfo( IN const std::vector< KGameDropElement >& vecDrop );

    void UpdateSocksTime();
    void LevelUpDrop( IN KUserPtr& spUser_, IN const char& cCharType_, IN const char& cOldLv, IN const char& cCurruntLv );
    void CheckLevelUpDrop( IN const KEVENT_SHANGHAI_DROP_REQ& kPacket );
    void DumpMonsterDropGP( IN const std::map<std::pair<int,int>, std::map<PAIR_DWORD,USHORT> >& mapDropGP );

    void ApplyChangeWeapon( IN const std::map<char, KChangeEquipment >& mapEquip );
    void StartSubmission( IN const std::vector<DWORD>& vecUserUID );

    void DumpServerList( IN const std::vector<KServerInfo>& vecList );

    void ApplyGuildBPoint( IN const KEndGameReq& kReq_, IN OUT KEndGameAck& kAck_ );
    void DivideChannelByBPoint( IN OUT KEndGameAck& kAck_ );

    void SetGuildInfo( IN const KNGuildUserInfo& kInfo );
    void SendVipEventUserRewardUpdateNot();
    void DumpInDoorCharInfo( IN const std::vector<KInDoorCharInfo>& vecList );
    void DumpEquipItemInfo( IN const KEquipItemInfo& kItem );
    bool GetRKTornadoNotExistList( IN DWORD& dwCharType, OUT std::map<int,GCITEMID>& mapNotExistList );
    bool GetNationRewardExpGP( OUT bool& bActiveNationReward, OUT float& fNationBoostExp, OUT float& fNationBoostGP );
    void CheckGameInfo( IN OUT KEVENT_START_GAME_BROAD& kPacket );

    void SetEnterSquareTime();
    DWORD GetLeaveSquareTime();
    void ExitParty();
    void CheckAgitBonus();
    void UpdateServerTimeNotice();
    bool GetAgitUserInfo( OUT KAgitUserInfo& kUserInfo );
    void CheckFlowerPotDrop( IN const DWORD& dwMapID, OUT std::vector<KDropItemInfo>& vecAddItem, OUT std::vector<GCITEMUID>& vecEraseItem );
    void LeaveAgit();
    void AgentUserDisconnectNot();

    void CheckInventoryBuffItem(OUT std::set<GCITEMID>& setItemID);
    void CheckInventoryVIPCollectionItem( OUT std::set<GCITEMID>& setItemID );
    void CheckShutdownUser();

    void SetUserAge( IN const int& nAge ) { m_iAge = nAge; }
    int GetUserAge() { return m_iAge; }
    void SendNMCafeLogin( UINT PlayTime = 0 );
    void SendNMCafeLogout();
    void CheckGameConnectSurvey( IN const DWORD& dwChannelType, IN const time_t& tFirstLoginTime, IN const time_t& tLastLoginTime, OUT KEVENT_SURVEY_LIST_NOT& kSurveyData );
    void CheckSurveyInfo( IN const int& nGameMode, IN const int& nDifficult, IN std::vector<KGameResultIn>& vecGameResult, IN std::map<DWORD,std::pair<int,DWORD>>& m_mapLevelUpUser );
    void SetUserLevelUpInfo( IN const DWORD& dwUID, IN const int& nCharType, IN const DWORD& dwLevel, IN OUT std::map<DWORD, std::pair<int,DWORD>>& mapLevelUpUser );
    void CheckLevelUpSurvey( IN const int& nCharType, IN const int& nLevel );
    void LeaveGameCheckSurveyInfo( IN const int& nCharType, IN const DWORD& dwLevel, IN std::wstring wstrLogin );
    void SendDungeonTicketListNot();
    void SendUserConnectNoticeToAgentMaster();
    void CheckEscortInfo( IN OUT KEVENT_START_GAME_BROAD& kPacket );
    void CheckEscortDrop( IN bool bIsEscortMonsterAlive_, IN DWORD dwEscortMonID_, IN int nGameMode_, IN int nDifficulty_, IN DWORD dwLevel_, OUT std::vector< KDropItemInfo >& vecdrops_ );
    void GetDBUpdateDataGP( OUT KGPData& kGPData );
    void CheckNewPost();
    void UpdateCharLvPlayTime( IN const int& nCharType, IN const DWORD& dwLv );
    void GetCharLvPlayTime( OUT MapUserCharLvPlayTime& mapStat ) { mapStat = m_mapUserCharLvPlayTime; }
    int GetCharLvStartGameTimeDiff();
    void SetCharLvStartGameTime() { m_dwCharLvStartGameTime = ::GetTickCount(); }
    void UpdateDungeonClearInfo( IN const int& nModeID, IN const int& nDifficulty, IN const KncBitStream& kModeDifficulty, IN OUT KDungeonClearData& kClearData );
    void UpdateCharEquipInfo( IN const char& cCharType );
    void UpdateStrengthEquipItemInfo( IN std::map<GCITEMUID, GCITEMUID> mapStrengthItem, IN OUT std::vector<KItem>& vecItem, OUT std::vector<KItem>& vecInvenItem );


    void GetCharEquipInfo( OUT std::set<DWORD>& setChangeEquipInfo );
    void UpdateEclipsePlotTime();
    bool PvpHackCheck( IN const char& cCharType, IN const DWORD& dwLv, IN const __int64& biExpRewardSum, IN OUT __int64& biExpReward );
    void PvpHackDisconnectCheck( IN const char& cModeCategory );

    void CheckJobMissionGuide( IN const DWORD& dwUID_, IN const char& cCharType_, IN const int& nLevel_ );
    void CheckJobMissionInfo( IN std::vector< KGameResultIn >& vecGameResult_ );
    void GetSpecialMonsterInfo( IN const int nModeID, IN const int nDifficulty, OUT KSpecialMonster& kSpecialMonster );
    bool GetProperty( IN Kairo& kPropertyCount, IN std::vector<Kairo>& vecProperty, OUT DWORD& dwProperty );
    // 커플 아이템 기간 만료 후에 m_vecCoupleInven에서 삭제 하는 것 추가
    bool RemoveCoupleInven( GCITEMUID itemUID );
    void UpdateCompleteMission( IN const DWORD& dwMissionID );
    bool CheckCompleteMission( IN const std::vector<DWORD>& vecMissionInfo );

    void CheckAdventureEvent();
    void CheckAdventureCollectingMission( IN KUserPtr& spUser_, IN const int& nTriggerID_ );
    bool UpdateWaterBombItem( KEVENT_SONGKRAN_WATERBOMB_SUPPLY_NOT kNotPacket );
    void SendSongkranWaterBombNot( IN const DWORD& dwTotalSuppliedDuration, IN const DWORD& dwSuppliedDuration );

    void CheckUserSealedCharInfo( IN std::vector< KGameResultIn >& vecGameResult_ );
    void UpdateUserSealedCharInfo( IN KUserPtr spUser_, IN const char& cCharType_, IN const int& nLevel_ );

    void CheckOlympicBossGateDrop( IN KUserPtr spUser_, IN std::vector< KItem >& vecDropItem_ );

    void CheckGetLimit( IN OUT bool& bItemDropped_, OUT std::map< DWORD, DROP_ITEMS >& mapEachUserDrops_ );
    void EraseVectorAFromB( IN DROP_ITEMS& A_, OUT DROP_ITEMS& B_ );
    void EraseMapAFromB( IN std::map< DWORD, DROP_ITEMS >& A_, OUT std::map< DWORD, DROP_ITEMS >& B_ );
    bool IsAdult() { return m_bAdult; }; // 중국 과몰입 구분용
    void SetAdult( IN bool bAdult );

    bool PCBangDungeonItemDrop( IN const KEndGameReq& kReq_, IN OUT KInsertDropItem& kInsertDropItem_ );
// end of protected:

private:
    bool TrainSkill( IN const std::set<int>& kPacket_ );
    bool RemoveSkill( IN std::set<int>& kPacket_, IN const GCITEMID itemID_, OUT KItem& kUseItem_ );
    bool EquipSkill( IN const std::map<std::pair<char,char>, std::vector<KSkillSlot> >& kPacket_ );
    bool ValidateCoupleInfo();
    bool InsertDefaultSkillInfo( IN const int& nSkillID, IN OUT KCharacterInfo& kCharacterInfo );
    void CheckUserCharLevelUpEvent( IN const DWORD& dwUID, IN std::set<char>& setCharInfo );
    void SendInventoryInfo( IN const std::vector<KItem>& vecItem );
    bool CheckRewardLevel( IN const char& cCharType, IN const char& cCharLevel, OUT bool& bCashType, IN OUT std::vector<KDropItemInfo>& vecItemInfo );	
    // 대전매칭.
    void UpdateELOInfo( IN const int& nPlayType, IN const KGameResultIn& vitResultIn, IN int nGameMode, IN bool bWin, IN KUserPtr& spUser, OUT KGameResultOut& kResultOut );
    void GetMatchInviteUserList( OUT std::vector<KMatchInviteUserInfo>& vecMatchInviteList );
    bool GetMatchInviteUserInfo( OUT KMatchInviteUserInfo& kMatchInviteUserInfo );
    bool IsJoinParty();
    bool SetSingleParty( IN const int& nModeInfo, IN const char& cCharType, IN const int& nExceptModeID );
    bool GetPartyRP( IN const std::vector<KPartyMember>& vecPartyPeople, OUT int& nRP );
    bool IsMatchSoloParty();
    bool CheckPartyMemberState( IN const std::vector<KPartyMember>& vecPartyPeople );
    bool IsMatchParty();
    void ChangeUserFindMatchState( IN const DWORD& dwUserUID );
    bool CheckMatchPartyState( OUT DWORD& dwHostUID );
    void SetMatchRoomInfo( OUT KRoomInfo& kRoomInfo );
    void JoinRoomMatchPartyMember( IN USHORT& usRoomID );
    void CalcCharELOData( IN OUT KELOUserData& kELOUserData );
    void GetMatchMyTeamType( OUT int& nTeamType );
    bool SetPartyMemberRP( IN OUT std::vector<KPartyMember>& vecPartyPeople );
    void SetMatchStartTick();
    void SetMatchEndTick();
    void InitMatchTick();
    DWORD GetMatchDurationTime();
    void CheckUserDelMatchTime( IN DWORD& dwDurationTime, IN int& nModeInfo );
    void SendUserMatchRefuseInfo( IN DWORD& dwDurationTime, IN int& nModeInfo );
    void SetELOResultInfo( IN const char& cCharType, IN KELOUserData& kELOUserData, OUT KDB_EVENT_MATCH_CHAR_DATA_REQ& kDBPacket );
    void SetELOResultStatInfo( IN const int& nType, IN const DWORD& dwUserUID, IN const char& cCharType, IN const DWORD& dwLevel, IN const std::pair<int, int> prELOTeamRPAvg, IN const int& nTeam, IN const KELOUserData& kELOUserData, IN const int& nModeInfo, IN const bool& bWin, IN const DWORD& dwMatchWaitTime, OUT KDB_EVENT_MATCH_ELO_RESULT_REQ& kDBPacket );
    void SetMatchIndoorUserInfo( IN KInDoorUserInfo& kMatchIndoorUserInfo );
    void GetMatchIndoorUserInfo( OUT KInDoorUserInfo& kMatchIndoorUserInfo );
    void SendToStartFailMatchMemberNot();
    bool CheckPartyHost( IN const DWORD& dwUserUID );
    void NofityMatchServer();
    void SetMatchRoomEnable( IN bool bMatchCreateRoom );
    bool IsMatchRoomEnable();
    void GetMatchPartyCharInfo( OUT char& cCharType, OUT int& nRP );
    void GetMatchSuccessLoopCount( OUT int& nLoopCount );
    void CreateRoomFailMatchPartyMember();
    void SendMatchTimeTable();
    void LoadRitasChristmasEventInfo();
    bool GetRitasChristmasReward( IN Kairo& kKairo_, OUT KDropItemInfo& kAddItem_ );
    bool CheckRitasChristmasBasketSize( IN int nGetSize_ );
    bool CalcRitasChristmasPoint( IN std::vector< int >& vecGetObjectID_, OUT int& nGetPoint_ );
    int GetRitasChristmasObjectPoint( IN int nObjectID_, IN std::map< int, KRitasChristmasObjectInfo >& mapObjectInfo_ );
    void CalcRitasChristmasFeverBonusPoint( IN int nGetFeverObjectCount_, OUT int& nGetBonusPoint_, OUT int& nGetPremiumRewardCount_ );
    bool CheckDayPassed( IN time_t tmLast_ );
    void CheckRitasChristmasStartCoinSupply();
    bool CheckDuplicateItemUID( IN std::vector< GCITEMUID >& vecItemUID_ );
    void SendRankRewardInfo();
    bool CheckStrengthActionAssistType( IN const DWORD dwAssistType_, IN const GCITEMUID dwAssistItemID_ );
    void LoadErrandEventInfo();
    void UpdateErrandTime();
    bool CheckUserAttributeMigrationPossible( IN const GCITEMUID& dwItemUID, IN const time_t& tmRegDate, OUT DWORD& dwChangePossbileCount );
    void IncreaseAttributeChangePossbileCount( IN const GCITEMUID& dwItemUID, OUT DWORD& dwChangePossbileCount );
    void ApplyExpToCharacter( IN const int nCharType_, IN const int nAddExp_ );
    void CheckCharacterGraduate( IN const int nSendType_ );
    bool CheckCharacterGraduate_OnConnect( OUT KEVENT_GRADUATE_CHARACTER_USER_INFO_NOT& kPacket_ );
    bool GetNonGraduateCharacters( OUT std::set< int >& setNonGraduateCharacters_ );
    bool CheckGraduationCondition( IN const std::map< int, std::set< int > >& mapConditionModes_ );
    bool CheckCharacterGraduate_OnDungeonClear( OUT KDB_EVENT_GRADUATE_CHARACTER_USER_INFO_REQ& kDBPacket_ );
    void DungeonMonsterHackDisconnectCheck( IN const int& nModeID );
    bool InsertSkillInfo( IN SSkillElement kSkill );
    bool AddDBUpdatedTrainedSkill( IN const SSkillElement& kSkill );
    void SendUserSystemInfo( IN int m_iSessionInfo );

    void UpdateCYOUHeartbeat();
    void CYOULogout( IN const char cCharType_ );
    void CYOULogin( IN const char cCharType_ );
    void InitFatigue();
    void CYOUSendBuyLog( IN KEVENT_BUY_FOR_CASH_ACK& kPacket_ );

    void LoadVitalityInfo();
    void SendVitalityInfo( IN const int nVitalitySendType_ );
    void SendVitalityCheckDungeonInfoNot();
    void CheckUpgradeGuildGradeGift();

    void CheckGiftFirstCharacter( IN const int nCharType_, OUT std::vector< KDropItemInfo >& vecGiftFirstCharacter_ );
    void GetConnectionGiftBoxUpdateData( OUT KConnectionGiftBoxUpdateData& kConnectionGiftBoxUpdateData_ );
    void CheckConnectionGiftBoxAccTime();
    void SendConnectionGiftBoxInfoNot( IN const int nSendType_ );
    bool CheckTCPRelayStatus( IN const USHORT& usRoomID, IN const DWORD& dwTRServerIP, IN const USHORT& usTRServerPort, IN const std::vector<KSimpleServerInfo>& vecUseTCPServerList, IN const std::vector<KSimpleServerInfo>& vecNoUseTCPServerList, OUT KEVENT_RELAY_SERVER_STATUS_ACK& kPacket );
    DWORD GetTongDonationUserAmount( IN const time_t tmVersion_, IN const DWORD dwNPCType_ );
    void SetTongDonationUserAmount( IN const time_t tmVersion_, IN const DWORD dwNPCType_, IN const DWORD dwAmount_ );
    void SetUDPUsePort( IN const USHORT& usUDPUsePort ) { m_usUDPUsePort = usUDPUsePort; }
    USHORT GetUDPUsePort(){ return m_usUDPUsePort; }
    bool CheckGradeUpgrade( IN const char& cRequestGradeID, IN const char& cCurrentGradeID );
    void SetCharEquipItemDetail( IN const std::vector<KItem>& vecInv, IN OUT std::vector<KEquipItemInfo>& vecEquipItems );
    void SetCharLookEquipDetail( IN const std::vector<KItem>& vecInv, IN OUT std::vector<KEquipItemInfo>& vecLoogEquip );
    void SetCharChangeWeaponEquipDetail( IN const std::vector<KItem>& vecInv_, IN OUT KEquipItemInfo& kChangeWeaponItem_ );
    bool GetLookEquipItem( IN const char& cCharType, OUT std::vector<KEquipItemInfo>& vecLookEquipInfo );
    void SetCharEquipItem( IN const std::vector<KItem> vecRewardItem, IN OUT KCharacterInfo& kCharacterInfo );
    void ApplyCharEquipItem( IN const char& cCharType, IN const std::vector<KEquipItemInfo>& vecEquipItems );
    void SetCharEquipPet( IN const std::map<GCITEMUID, KPetInfo>& mapPetInfo, IN OUT KPetInfo& kEquipPetInfo );
    void UpdateDepotDataReq();

    void InsertInvenDepotMoveInfo( IN const KInvenDepotMoveLog& kInvenDepotMoveLog );
    void GetInvenDepotMoveInfo( OUT std::vector<KInvenDepotMoveLog>& vecInvenDepotMoveLog );
	void UpdateDungeonBestRecordData( IN KUser& kUser, IN const int& nGameMode, IN const int& nDifficulty, IN const int& nRank, IN const DWORD& dwClearTime );
    void ProcessPrevRecordDataWhenFirstClear( IN KUser& kUser, IN const int& nGameMode, IN const int& nDifficulty );
    bool CheckCharLimitLevel( IN const DWORD dwLevel );

    bool IsInfinityDungeonMonsterHack( IN const DWORD dwSummonGap_, IN const DWORD dwSummonMonsterCount_ );
    void MakeIDSummonedMonsterInfo( IN KEVENT_INFINITY_DUNGEON_MONSTER_SUMMON_REQ& kPacket_, IN DWORD dwIDMonsterLastUID_, OUT std::list< KIDMonsterInfo >& lstIDSummonedMonsters_, OUT DWORD& dwIDMonsterLastUIDAfter_ );
    void MakeIDReservedDropInfo( IN std::list< KIDMonsterInfo >& lstIDSummonedMonsters_, IN std::vector< KInfinityDungeon::KIDRewardInfo >& vecIDRewardInfo_, IN DWORD nPartyNum_, IN DWORD dwIDReservedDropLastUID_, OUT std::list< KGameDropElement >& lstIDReservedDrops_, OUT DWORD& dwIDReservedDropLastUIDAfter_ );
    void MakeIDReservedGPDropInfo( IN std::list< KIDMonsterInfo >& lstIDSummonedMonsters_, IN DWORD dwIDReservedDropLastUID_, OUT std::list< KGameDropElement >& lstIDReservedGPDrops_, OUT DWORD& dwIDReservedDropLastUIDAfter_ );
    bool GetIDReward( IN Kairo& kKairo_, OUT KDropItemInfo& kAddItem_ );
    bool AddIDSummonedMonsters( IN std::list< KIDMonsterInfo >& lstIDSummonedMonsters_, OUT std::list< KIDMonsterInfo >& lstIDSummonedMonstersAfter_ );
    bool AddIDReservedDrops( IN std::list< KGameDropElement >& lstIDReservedDrops_, OUT std::list< KGameDropElement >& lstIDReservedDropsAfter_ );
    bool AddIDReservedGPDrops( IN std::list< KGameDropElement >& lstIDReservedGPDrops_, OUT std::list< KGameDropElement >& lstIDReservedGPDropsAfter_ );
    bool SetInfinityDungeonMonsterSummonBroadPacket( IN std::list< KIDMonsterInfo >& lstIDSummonedMonsters_, IN std::list< KGameDropElement >& lstIDReservedDrops_, IN std::list< KGameDropElement >& lstIDReservedGPDrops_, OUT KEVENT_INFINITY_DUNGEON_MONSTER_SUMMON_BROAD& kPacket_ );
    bool DoKillInfinityDungeonMonster( IN const int nMonsterUID_, IN OUT std::list< KIDMonsterInfo >& lstIDSummonedMonsters_, OUT KIDMonsterInfo& kIDMonsterInfo_ );
    bool DoGetInfinityDungeonItem( IN const DWORD dwDropUID_, IN std::list< KGameDropElement >& lstIDReservedDrops_, OUT KDropItemInfo& kDropItemInfo_, OUT DWORD& dwReservedUserUID_ );
    bool DecideWhoGetDropItem( IN GCITEMID dwItemID_, IN std::vector< DWORD >& vecUserUIDs_, IN OUT DWORD& dwRecvUserUID_, OUT char& cRecvUserCharType_, OUT std::map< DWORD, int >& mapUserDice_ );
    bool GetAffordableInvenUsers( IN GCITEMID dwItemID_, IN std::vector< DWORD >& vecUserUIDs_, OUT std::vector< PAIR_DWORD_CHAR >& vecNewUserUIDs_ );
    bool AddStatInfinityDungeon( IN const int nEndType_ );
    float GetDropEventScale( IN const KGameInfo& kGameInfo_ );
    bool BindIDReservedDropToUsers( IN std::list< KGameDropElement >& lstIDReservedDrops_, IN std::vector< DWORD >& vecUserUIDs_ );

    bool CheckNormalMissionRegistEnable( IN const int& nGameMode );
    bool IsAlreadyFinishedMission( IN const DWORD& dwMissionID );

    bool UpdateDungeonPersonalRecordData( IN const int nModeID_, IN const int nCharType_, IN const std::map< int, KDungeonPersonalRecord >& mapDungeonPersonalRecord_ );

protected:
    DECL_ON_FUNC( EVENT_VERIFY_ACCOUNT_REQ );   // file:.\User.cpp#EVENT_VERIFY_ACCOUNT_REQ 
   _DECL_ON_FUNC( EVENT_VERIFY_ACCOUNT_ACK, KEVENT_DB_VERIFY_ACCOUNT_ACK );
   _DECL_ON_FUNC( EVENT_REGISTER_NICKNAME_REQ, std::wstring );
   _DECL_ON_FUNC( EVENT_REGISTER_NICKNAME_ACK, KPacketNameOK );
    DECL_ON_FUNC_NOPARAM( EVENT_AGREEMENT_PRIVATEINFO_NOT );

   _DECL_ON_FUNC( EVENT_CHAT_REQ, KChatData );
   _DECL_ON_FUNC( EVENT_NOTIFY_REQ, std::wstring );
   _DECL_ON_FUNC( EVENT_WHISPER_REQ, KPacketNameMsg );
    DECL_ON_FUNC( EVENT_WHISPER_ACK );
   _DECL_ON_FUNC( EVENT_WHERE_REQ, std::wstring );

   _DECL_ON_FUNC( EVENT_ENTER_CHANNEL_REQ, DWORD ); // 대전 강화에서 채널 들어갈때 케릭터를 선택한다.
    DECL_ON_FUNC_NOPARAM( EVENT_CHANNEL_LIST_REQ );

    DECL_ON_FUNC( EVENT_ROOM_LIST_REQ );
   _DECL_ON_FUNC( EVENT_USER_LIST_REQ, int );
   _DECL_ON_FUNC( EVENT_JOIN_ROOM_REQ, KJoinRoomReqInfo );
    DECL_ON_FUNC( EVENT_CREATE_ROOM_REQ );
    DECL_ON_FUNC_NOPARAM( EVENT_LEAVE_CHANNEL_NOT );

   _DECL_ON_FUNC( EVENT_CHANGE_ROOM_INFO_REQ, KChangeRoomInfo );
    DECL_ON_FUNC( EVENT_CHANGE_HOST_ADDRESS_NOT );          // file:.\User3.cpp#EVENT_CHANGE_HOST_ADDRESS_NOT
   _DECL_ON_FUNC( EVENT_INVITE_REQ, std::vector<DWORD> );   // file:.\User3.cpp#EVENT_INVITE_REQ
    DECL_ON_FUNC( EVENT_LEAVE_ROOM_REQ );                   // file:.\User2.cpp#EVENT_LEAVE_ROOM_REQ
   _DECL_ON_FUNC( EVENT_START_GAME_REQ, KStartGameReq );

    DECL_ON_FUNC( EVENT_USE_DURATION_ITEM_REQ );
   _DECL_ON_FUNC( EVENT_LEAVE_GAME_REQ, KGameResultIn );
   _DECL_ON_FUNC( EVENT_END_GAME_REQ, KEndGameReq );
   _DECL_ON_FUNC( EVENT_END_GAME_ACK, KEndGameAck );

    DECL_ON_FUNC( EVENT_STRONG_LEVELUP_REQ );
   _DECL_ON_FUNC( EVENT_STRONG_LEVELUP_ACK, KDB_EVENT_STRONG_LEVELUP_ACK );
    DECL_ON_FUNC( EVENT_BUY_FOR_GP_REQ );
    DECL_ON_FUNC( EVENT_BUY_FOR_GP_ACK );
    DECL_ON_FUNC( EVENT_BUY_FOR_GEM_REQ );
    DECL_ON_FUNC( EVENT_BUY_FOR_GEM_ACK );
   _DECL_ON_FUNC( EVENT_BUY_FOR_CRYSTAL_REQ, KEVENT_BUY_FOR_GEM_REQ );
    DECL_ON_FUNC( EVENT_BUY_FOR_CRYSTAL_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_QUERY_INVENTORY_INFO_REQ );
    DECL_ON_FUNC( EVENT_QUERY_INVENTORY_INFO_ACK );
   _DECL_ON_FUNC( EVENT_EQUIP_ITEM_REQ, KEquipUser );
   _DECL_ON_FUNC( EVENT_SELL_ITEM_REQ, KSimpleItem );
    DECL_ON_FUNC( EVENT_SELL_ITEM_ACK );

    //guild 부분
   _DECL_ON_FUNC( EVENT_QUERY_NORMAL_ITEM_LIST_REQ, KPacket2Name );
   _DECL_ON_FUNC( EVENT_QUERY_NORMAL_ITEM_LIST_ACK, KItemInv );
   _DECL_ON_FUNC( EVENT_QUERY_DURATION_ITEM_LIST_REQ, KPacket2Name );
   _DECL_ON_FUNC( EVENT_QUERY_DURATION_ITEM_LIST_ACK, KDurationInv );
   _DECL_ON_FUNC( EVENT_QUERY_SALED_NORMAL_ITEM_LIST_REQ, KPacket2Name );
   _DECL_ON_FUNC( EVENT_QUERY_SALED_NORMAL_ITEM_LIST_ACK, KItemInv );
   _DECL_ON_FUNC( EVENT_QUERY_SALED_DURATION_ITEM_LIST_REQ, KPacket2Name );
   _DECL_ON_FUNC( EVENT_QUERY_SALED_DURATION_ITEM_LIST_ACK, KDurationInv );
   _DECL_ON_FUNC( EVENT_ADD_NORMAL_ITEM_GP_REQ, KAddItemInfo );
   _DECL_ON_FUNC( EVENT_ADD_NORMAL_ITEM_GP_ACK, KPacketAck );
   _DECL_ON_FUNC( EVENT_ADD_DURATION_ITEM_GP_REQ, KAddItemInfo );
   _DECL_ON_FUNC( EVENT_ADD_DURATION_ITEM_GP_ACK, KPacketAck );
   _DECL_ON_FUNC( EVENT_ADD_NORMAL_ITEM_CASH_REQ, KAddItemInfo );
   _DECL_ON_FUNC( EVENT_ADD_NORMAL_ITEM_CASH_ACK, KPacketAck );
   _DECL_ON_FUNC( EVENT_ADD_DURATION_ITEM_CASH_REQ, KAddItemInfo );
   _DECL_ON_FUNC( EVENT_ADD_DURATION_ITEM_CASH_ACK, KPacketAck );
    DECL_ON_FUNC( EVENT_REMOVE_ITEM_REQ );
   _DECL_ON_FUNC( EVENT_REMOVE_ITEM_ACK, KPacketAck );
   _DECL_ON_FUNC( EVENT_QUERY_USER_INFO_REQ, KPacket2Name );
    DECL_ON_FUNC( EVENT_QUERY_USER_INFO_ACK );
   _DECL_ON_FUNC( EVENT_CHANGE_STRONG_ITEM_REQ, KPacket2Name );
    DECL_ON_FUNC( EVENT_CHANGE_STRONG_ITEM_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_QUERY_ITEM_INFO_REQ );  //file:.\GSDBThread3.cpp#EVENT_QUERY_ITEM_INFO_REQ
    DECL_ON_FUNC( EVENT_QUERY_ITEM_INFO_ACK );          //file:.\User4.cpp#EVENT_QUERY_ITEM_INFO_ACK
   _DECL_ON_FUNC( EVENT_KICK_USER_BY_ADMIN_REQ, DWORD );

   _DECL_ON_FUNC( EVENT_QUERY_USER_INFO2_REQ, KPacket2Name );
    DECL_ON_FUNC( EVENT_QUERY_USER_INFO2_ACK );
   _DECL_ON_FUNC( EVENT_REMOVE_USER_INFO_REQ, KPacket2Name );
   _DECL_ON_FUNC( EVENT_REMOVE_USER_INFO_ACK, KPacketAck );

   _DECL_ON_FUNC( DB_EVENT_INSERT_DROP_ITEM_ACK, KEndGameAck );
   _DECL_ON_FUNC( EVENT_UDP_P2P_SOCKET_ERR_NOT, std::wstring );
   _DECL_ON_FUNC( EVENT_BAN_USER_REQ, KPacketNameOK );

    DECL_ON_FUNC_NOPARAM( EVENT_CLIENT_CRASHED_NOT );
   _DECL_ON_FUNC( EVENT_ITEM_EXPIRED_NOT, std::vector<KItem> );
   _DECL_ON_FUNC( EVENT_DETAIL_USER_INFO_REQ, std::wstring );
   _DECL_ON_FUNC( EVENT_P2P_RELAY_REQ, KP2PData );

   _DECL_ON_FUNC( EVENT_SERVER_MIGRATION_REQ, std::wstring );
   _DECL_ON_FUNC( EVENT_SERVER_MIGRATION_ACK, int );

   _DECL_ON_FUNC( EVENT_CLIENT_ERR_REPORT_NOT, KEventErr );
   _DECL_ON_FUNC( EVENT_CLIENT_ERR_REPORT_COUNT_NOT, KEventErrCount );

    DECL_ON_FUNC( EVENT_USE_CHANGE_NICKNAME_REQ );
   _DECL_ON_FUNC( EVENT_USE_CHANGE_NICKNAME_ACK, KEVENT_USE_CHANGE_NICKNAME_REQ );

    DECL_ON_FUNC( EVENT_USE_CHANGE_NICKCOLOR_REQ );
   _DECL_ON_FUNC( EVENT_USE_CHANGE_NICKCOLOR_ACK, KEVENT_USE_CHANGE_NICKCOLOR_REQ );

    DECL_ON_FUNC( EVENT_REGIST_MISSION_REQ );
    DECL_ON_FUNC( EVENT_REGIST_MISSION_ACK );

   _DECL_ON_FUNC( EVENT_REMOVE_MISSION_REQ, KEVENT_REMOVE_MISSION );
   _DECL_ON_FUNC( EVENT_REMOVE_MISSION_ACK, KEVENT_REMOVE_MISSION );

    DECL_ON_FUNC( EVENT_COMPLETE_MISSION_REQ );
    DECL_ON_FUNC( EVENT_COMPLETE_MISSION_ACK );

   _DECL_ON_FUNC( EVENT_DUNGEON_MISSION_REGIST_REQ, DWORD );
    DECL_ON_FUNC( EVENT_DUNGEON_MISSION_REGIST_ACK );
    DECL_ON_FUNC( EVENT_FINISHED_MISSION_LIST_NOT );

    DECL_ON_FUNC( EVENT_GMTOOL_AUTHENTICATION_REQ );
    DECL_ON_FUNC( EVENT_GMTOOL_AUTHENTICATION_ACK );
    DECL_ON_FUNC( DB_EVENT_CREATE_INDIGO_ACK );
   _DECL_ON_FUNC( DB_EVENT_UPDATE_INDIGO_ACK, KIndigoData );
   _DECL_ON_FUNC( EVENT_SET_HBCHECK_FLAG, bool );

    DECL_ON_FUNC_NOPARAM( EVENT_ENCHANT_GEMCOUNT_REQ );	

    //빌링 관련
    DECL_ON_FUNC( EVENT_BUY_FOR_CASH_REQ );
    DECL_ON_FUNC( EVENT_BUY_FOR_CASH_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_CURRENT_CASH_POINT_REQ );
   _DECL_ON_FUNC( EVENT_CURRENT_CASH_POINT_ACK, KCashInfo );
   _DECL_ON_FUNC( EVENT_CHANGE_ROOMUSER_INFO_REQ, KChangeRoomUserInfo );
    DECL_ON_FUNC( EVENT_LOAD_COMPLETE_NOT );
    DECL_ON_FUNC( EVENT_SHANGHAI_DROP_REQ );
    DECL_ON_FUNC( EVENT_SHANGHAI_DROP_ACK ); //샹하이 드랍
   _DECL_ON_FUNC( EVENT_SHANGHAI_CHECK_ACK, KncBitStream );
   _DECL_ON_FUNC( EVENT_CHANGE_INDOOR_USERINFO_REQ, KInDoorUserInfo );
   _DECL_ON_FUNC( EVENT_RECOMMENDER_REQ, KPacketNameOK );
   _DECL_ON_FUNC( EVENT_RECOMMENDER_ACK, int );
    DECL_ON_FUNC_NOPARAM( EVENT_END_RESULT_NOT );
    DECL_ON_FUNC( EVENT_CHECK_ROOM_USERLIST_NOT );
    DECL_ON_FUNC( EVENT_ACCUMULRATION_TIME_NOT );
   _DECL_ON_FUNC( EVENT_SET_CURRENT_CHARACTER_REQ, char );

   _DECL_ON_FUNC( EVENT_KAIRO_REQ, GCITEMUID );
    DECL_ON_FUNC( EVENT_KAIRO_ONESHOT_REQ );
    DECL_ON_FUNC( EVENT_KAIRO_ACK );
    DECL_ON_FUNC( EVENT_KAIRO_ONESHOT_ACK );

    // 2007/1/13. iridology. 펫관련
    DECL_ON_FUNC( EVENT_CREATE_PET_REQ );
   _DECL_ON_FUNC( EVENT_CREATE_PET_ACK, KDB_EVENT_CREATE_PET );
    DECL_ON_FUNC( EVENT_DELETE_PET_REQ );
   _DECL_ON_FUNC( EVENT_DELETE_PET_ACK, KDB_EVENT_DELETE_PET );
    DECL_ON_FUNC( EVENT_FEEDING_PET_REQ );
    DECL_ON_FUNC( EVENT_CHANGE_PET_NAME_REQ );
   _DECL_ON_FUNC( EVENT_CHANGE_PET_NAME_ACK, KEVENT_CHANGE_PET_NAME_REQ );

   _DECL_ON_FUNC( EVENT_LOUD_MSG, std::wstring ); // 전체 메세지

   // 070314. kkurrung. 
   _DECL_ON_FUNC( EVENT_PRE_COMPLETE_REQ, int );
   _DECL_ON_FUNC( EVENT_USE_INVENTORY_EXTEND_REQ, KExtendItem ); 
   _DECL_ON_FUNC( EVENT_USE_INVENTORY_EXTEND_ACK, KExtendItem );

    // 유저 히스토리
    DECL_ON_FUNC( EVENT_STAT_USER_HISTORY_NOT );
    
    // 한시간 이벤트
   _DECL_ON_FUNC( EVENT_REPETITION_PROVIDE_NOT, KEVENT_QUERY_INVENTORY_INFO_ACK );
   _DECL_ON_FUNC( EVENT_PROTECED_TABBLE_CHECKSUM_NOT, DWORD );
    
    //방장에 의해서 방유저의 정보를 강제로 변환할수 있는 패킷.
   _DECL_ON_FUNC( EVENT_FORCE_CHANGE_ROOMUSER_INFO_REQ, KForceChangeRoomUserInfo );

    //펫 진화
    DECL_ON_FUNC( EVENT_CHECK_TRANSFORMATION_PET_REQ ); //진화 퇴화 가능 체크
    DECL_ON_FUNC( EVENT_TRANSFORMATION_PET_REQ ); // 정말 진화 요청.
    DECL_ON_FUNC( EVENT_TRANSFORMATION_PET_ACK );

    //클라이언트 상태 강제 변경 
   _DECL_ON_FUNC( EVENT_FORCE_TRANSITION_STATE_REQ, int );

    //포인트 시스템.
   _DECL_ON_FUNC( EVENT_LOAD_POINTSYSTEM_INFO_ACK, KPointSystemInfo );
    DECL_ON_FUNC_NOPARAM( EVENT_GET_CURRENT_POINT_REQ );
    DECL_ON_FUNC_NOPARAM( EVENT_GET_CALENDAR_REQ );
   _DECL_ON_FUNC( EVENT_SEALLING_CALENDAR_REQ, KSimpleDate );
    DECL_ON_FUNC( EVENT_SEALLING_CALENDAR_ACK );
    DECL_ON_FUNC( EVENT_MONTHLY_ATTEND_NOT );
   _DECL_ON_FUNC( EVENT_LOAD_NEW_CALENDAR_NOT, std::vector<KDailyInfo> );
    DECL_ON_FUNC( EVENT_WEEKLY_ATTEND_REQ );
   _DECL_ON_FUNC( EVENT_GCPOINT_RECOMMEND_REQ, std::wstring );
   _DECL_ON_FUNC( EVENT_GCPOINT_RECOMMEND_ACK, int );
    DECL_ON_FUNC( EVENT_MONTHLY_ATTEND_REWARD_REQ );
    DECL_ON_FUNC( EVENT_MONTHLY_ATTEND_REWARD_ACK );
   _DECL_ON_FUNC( EVENT_MONTHLY_ATTEND_REWARD_LIST_NOT, std::vector<KSimpleDate> );
    
    // 부화
   _DECL_ON_FUNC( EVENT_HATCHING_EGG_REQ, GCITEMUID );
    DECL_ON_FUNC( EVENT_HATCHING_EGG_ACK );

    // 포인트 아이템 구매 관련
   _DECL_ON_FUNC( EVENT_BUY_GCPOINT_ITEM_REQ, KBuySellItemReq );
    DECL_ON_FUNC( EVENT_BUY_GCPOINT_ITEM_ACK );

   _DECL_ON_FUNC( EVENT_SELL_GCPOINT_ITEM_REQ, KBuySellItemReq );
    DECL_ON_FUNC( EVENT_SELL_GCPOINT_ITEM_ACK );

   _DECL_ON_FUNC( EVENT_RESET_WIN_LOSE_REQ, KResetWinLoseData );
   _DECL_ON_FUNC( EVENT_RESET_WIN_LOSE_ACK, KResetWinLoseData );

    DECL_ON_FUNC( EVENT_LUCKYDAY_NOT );

   _DECL_ON_FUNC( EVENT_CHANGE_ROOM_OPTION_REQ, KRoomOption );
   _DECL_ON_FUNC( EVENT_YAHOO_BENEFIT_NOT, KInven_GP ); // DB로 부터 받아온다.
   _DECL_ON_FUNC( EVENT_STAT_LOADING_TIME_NOT, DWORD ); //로딩 타임 통계
   _DECL_ON_FUNC( EVENT_MSG_NOT, std::wstring );

    DECL_ON_FUNC( EVENT_GET_NASTY_INFO_NOT );
    DECL_ON_FUNC( EVENT_REPORT_USER_REQ ); //유저를 신고 한다.
   _DECL_ON_FUNC( EVENT_REPORT_USER_ACK, KEVENT_DB_REPORT_USER_ACK );
   _DECL_ON_FUNC( EVENT_CHATTING_EVENT_CORRECT_NOT, int );
    DECL_ON_FUNC( EVENT_ENCHANT_REQ );
    DECL_ON_FUNC( EVENT_ENCHANT_ACK );

    DECL_ON_FUNC_NOPARAM( EVENT_SERVER_LIST_REQ );
    DECL_ON_FUNC( DB_EVENT_BONUS_POINT_REFILL_ACK );
    DECL_ON_FUNC( EVENT_CHANGE_BONUS_COIN_REQ );
    DECL_ON_FUNC( EVENT_CHANGE_BONUS_COIN_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_USE_BONUS_POINT_REQ );

   _DECL_ON_FUNC( EVENT_SLOT_INFO_NOT, KQuickSlot );
   _DECL_ON_FUNC( EVENT_CHANGE_SLOT_EQUIP_REQ, KQuickSlot );
    DECL_ON_FUNC( EVENT_USE_QUICK_SLOT_REQ );

   _DECL_ON_FUNC( EVENT_SQUARE_LIST_REQ, DWORD );
   _DECL_ON_FUNC( EVENT_SQUARE_LIST_ACK, KNGuildUserInfo );
    DECL_ON_FUNC( EVENT_ENTER_SQUARE_REQ );             // file:.\User6.cpp#EVENT_ENTER_SQUARE_REQ
    DECL_ON_FUNC( EVENT_ENTER_SQUARE_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_SQUARE_LOADING_COMPLETE_REQ );
   _DECL_ON_FUNC( EVENT_NEW_SQUARE_USER_NOT, KSquareUserInfo );
   _DECL_ON_FUNC( EVENT_LEAVE_SQUARE_USER_NOT, DWORD );
    DECL_ON_FUNC_NOPARAM( EVENT_LEAVE_SQUARE_REQ );
   _DECL_ON_FUNC( EVENT_UPDATE_MY_SQUARE_POS, KSquarePos );
   _DECL_ON_FUNC( EVENT_SQUARE_CHAT_REQ, KChatData );
    DECL_ON_FUNC_NOPARAM( EVENT_SQUARE_USER_SYNC_REQ );
    DECL_ON_FUNC( EVENT_NEW_CHARACTER_REQ );
    DECL_ON_FUNC( EVENT_NEW_CHARACTER_ACK );
   _DECL_ON_FUNC( EVENT_USE_EMOTICON_REQ, KUserEmoticon );

   _DECL_ON_FUNC( EVENT_TW_USER_AGREEMENT_REQ, int ); //  2008215 woosh  대만 정형화
   _DECL_ON_FUNC( EVENT_TW_USER_AGREEMENT_ACK, int );
    DECL_ON_FUNC( EVENT_INVITED_ROOM_FROM_SQUARE_REQ );
   _DECL_ON_FUNC( EVENT_SQUARE_USER_LIST_REQ, PAIR_DWORD_INT );
   _DECL_ON_FUNC( EVENT_INVITE_DENY_NOT, bool );
    DECL_ON_FUNC_NOPARAM( EVENT_SIGN_BOARD_NUM_REQ );
   _DECL_ON_FUNC( EVENT_SIGN_BOARD_REG_REQ, KSignBoardData );
    DECL_ON_FUNC( EVENT_SIGN_BOARD_NOTIFY );
    DECL_ON_FUNC( EVENT_EMOTICON_MIX_REQ ); // 이모티콘
    DECL_ON_FUNC( EVENT_EMOTICON_MIX_ACK );
   _DECL_ON_FUNC( EVENT_CHANGE_OBSERVER_REQ, bool );
   _DECL_ON_FUNC( EVENT_CONNECTION_MSG_ACK, std::vector<std::wstring> );

    DECL_ON_FUNC( EVENT_RANK_PAGE_REQ );
   _DECL_ON_FUNC( EVENT_MY_RANK_INFO_NOT, KMyRankInfo );
    DECL_ON_FUNC( EVENT_RANK_SEARCH_REQ );
    DECL_ON_FUNC( EVENT_RANK_SEARCH_ACK );

   _DECL_ON_FUNC( EVENT_GET_HELL_ITEM_ACK, std::vector<KItem> );
   _DECL_ON_FUNC( EVENT_SELL_INFO_REQ, GCITEMID );
   _DECL_ON_FUNC( EVENT_SELL_COUNT_ITEM_REQ, KBuySellItemReq );
   _DECL_ON_FUNC( EVENT_SELL_COUNT_ITEM_ACK, KDBSellCountItemData );

   _DECL_ON_FUNC( EVENT_CURRENT_VIRTUAL_CASH_NOT, int );
    DECL_ON_FUNC( EVENT_BUY_VIRTUAL_CASH_REQ ); // 가상 캐쉬 구매 
    DECL_ON_FUNC( EVENT_BUY_VIRTUAL_CASH_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_JOIN_GAME_REQ );

   _DECL_ON_FUNC( EVENT_COLLECTION_COMPLETE_REQ, int );
    DECL_ON_FUNC( EVENT_COLLECTION_COMPLETE_ACK );

    DECL_ON_FUNC( EVENT_P2P_STAT_INFO );
   _DECL_ON_FUNC( EVENT_CHECK_OUT_RECORD_UPDATE, unsigned short ); // 패킷 중복 처리 방지
    DECL_ON_FUNC_NOPARAM( EVENT_P2P_UNIQUE_NUM_REQ );
   _DECL_ON_FUNC( EVENT_UPDATE_DEATH_SCORE_NOT, KDeathMatchScore );
   _DECL_ON_FUNC( EVENT_RELAY_DATA, std::vector<DWORD> );
    DECL_ON_FUNC( EVENT_MINIGAME_INFO_NOT );
   _DECL_ON_FUNC( EVENT_START_MINIGAME_REQ, char );
   _DECL_ON_FUNC( EVENT_END_MINIGAME_REQ, KMiniGameRankInfo );
    DECL_ON_FUNC_NOPARAM( EVENT_EXIT_MINIGAME_REQ );
   _DECL_ON_FUNC( EVENT_MINIGAME_TOPRANK_REQ, char );
   _DECL_ON_FUNC( EVENT_MINIGAME_RESTART_REQ, char );
   _DECL_ON_FUNC( EVENT_END_MINIGAME_ACK, KMiniGameRankInfo );
    DECL_ON_FUNC_NOPARAM( EVENT_MINIGAME_MY_RANK_INFO_REQ );
    DECL_ON_FUNC_NOPARAM( EVENT_GET_FULL_SP_INFO_REQ );
   _DECL_ON_FUNC( EVENT_SKILL_TRAINING_REQ, int );
   _DECL_ON_FUNC( EVENT_SET_SKILL_REQ, KChangeSkillSet );
    DECL_ON_FUNC( EVENT_REMOVE_SKILL_REQ );
   _DECL_ON_FUNC( EVENT_UNLOCK_SKILL_REQ, int );
    DECL_ON_FUNC( EVENT_UNLOCK_SKILL_ACK );
   _DECL_ON_FUNC( EVENT_CHANGE_COUPON_REQ, KComplexItem );
    DECL_ON_FUNC( EVENT_CHANGE_COUPON_ACK );
    DECL_ON_FUNC( EVENT_GACHA_REWARD_LIST_REQ );
    DECL_ON_FUNC( EVENT_GACHA_ACTION_REQ );
    DECL_ON_FUNC( EVENT_GACHA_ACTION_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_GACHA_OBTAINED_SET_REWARD_REQ );
    DECL_ON_FUNC( EVENT_GACHA_SET_REWARD_REQ );
    DECL_ON_FUNC( EVENT_GACHA_SET_REWARD_ACK );
    DECL_ON_FUNC( EVENT_GACHA_SET_REWARD_LIST_REQ );
   _DECL_ON_FUNC( EVENT_UPDATE_GACHA_KEY_BUY_COUNT_ACK, PAIR_INT );

    DECL_ON_FUNC( EVENT_CLIENT_FAILED_GAME_START_NOT );   // client->server.클라이언트 게임시작실패시 원인 알림.
    
    // 커플 관련
   _DECL_ON_FUNC( EVENT_FULL_COUPLE_INFO_ACK, KCoupleInfo );
    DECL_ON_FUNC( EVENT_MAKE_COUPLE_REQ );
   _DECL_ON_FUNC( EVENT_MAKE_COUPLE_ANS, bool );
   _DECL_ON_FUNC( EVENT_MAKE_COUPLE_ANS_FROM, bool );
    DECL_ON_FUNC( EVENT_MAKE_COUPLE_ACK );
   _DECL_ON_FUNC( EVENT_USE_COUPLE_ITEM_REQ, KCoupleItemData );
   _DECL_ON_FUNC( EVENT_USE_COUPLE_ITEM_ACK, KCoupleItemData );
   _DECL_ON_FUNC( EVENT_USE_COUPLE_ITEM_NOT, KCoupleItemData );

   _DECL_ON_FUNC( EVENT_EQUIP_COUPLE_ITEM_REQ, std::vector<GCITEMID> );
   _DECL_ON_FUNC( EVENT_EQUIP_COUPLE_ITEM_NOT, std::vector<GCITEMID> );

   _DECL_ON_FUNC( EVENT_COUPLE_INFORMATION_REQ, DWORD );
   _DECL_ON_FUNC( EVENT_SET_COUPLE_REQ_DENY, DWORD );

    DECL_ON_FUNC( EVENT_CHANGE_COUPLE_RING_REQ );
   _DECL_ON_FUNC( EVENT_CHANGE_COUPLE_RING_ANS, bool );
   _DECL_ON_FUNC( EVENT_CHANGE_COUPLE_RING_ANS_FROM, bool );
    DECL_ON_FUNC( EVENT_CHANGE_COUPLE_RING_ACK );
   _DECL_ON_FUNC( EVENT_DIVORCE_COUPLE_REQ, std::wstring );
   _DECL_ON_FUNC( EVENT_DIVORCE_COUPLE_NOT, std::wstring );
   _DECL_ON_FUNC( EVENT_CHANGE_COUPLE_CHAR_REQ, char );
   _DECL_ON_FUNC( EVENT_COUPLE_CHAT_REQ, KChatData );
   _DECL_ON_FUNC( EVENT_COUPLE_WINDOW_STATE_REQ, int );
    DECL_ON_FUNC_NOPARAM( EVENT_PET_COSTUM_LIST_REQ );
    DECL_ON_FUNC( EVENT_WEEKLY_ATTEND_ACK );
   _DECL_ON_FUNC( EVENT_GMTOOL_USER_LIST_REQ, int );
   _DECL_ON_FUNC( EVENT_GMTOOL_SQUARE_USER_LIST_REQ, PAIR_DWORD_INT );

    DECL_ON_FUNC_NOPARAM( EVENT_GET_USER_DONATION_INFO_REQ );
    DECL_ON_FUNC( EVENT_GET_USER_DONATION_INFO_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_DONATION_INFO_REQ );
    DECL_ON_FUNC( EVENT_DONATION_POINT_REQ );
    DECL_ON_FUNC( EVENT_DONATION_POINT_ACK );
   _DECL_ON_FUNC( EVENT_ADD_EXP_REQ, KAddExpInfo );
   _DECL_ON_FUNC( EVENT_CHECK_LUNA_NEWYEAR_ACK, bool );
    DECL_ON_FUNC_NOPARAM( EVENT_LUNA_NEWYEAR_REQ );
    DECL_ON_FUNC( EVENT_LUNA_NEWYEAR_ACK );

    DECL_ON_FUNC_NOPARAM( EVENT_CHAR_POLL_TERM_REQ );
    DECL_ON_FUNC( EVENT_CHAR_POLL_REQ );
    DECL_ON_FUNC_NOPARAM( EVENT_GACHA_COIN_COUNT_REQ );
   _DECL_ON_FUNC( EVENT_GACHA_COIN_COUNT_ACK, int );    // 가챠 코인수
    DECL_ON_FUNC( EVENT_BUY_FOR_VIP_REQ );
    DECL_ON_FUNC( EVENT_BUY_FOR_VIP_ACK );
   _DECL_ON_FUNC( EVENT_CHECK_GACHA_REEL_POINT_ACK, GAGHA_REEL_POINT );

    DECL_ON_FUNC( EVENT_OPEN_CAPSULE_REQ );
    DECL_ON_FUNC( EVENT_OPEN_CAPSULE_ACK );
    DECL_ON_FUNC( EVENT_USE_GAME_COUPON_REQ );
    DECL_ON_FUNC( EVENT_USE_GAME_COUPON_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_GAME_COUPON_LIST_REQ );
    DECL_ON_FUNC_NOPARAM( EVENT_GAME_COUPON_FAIL_NOT );

    // 09.04.10. kkurrung. 추천 (5th)
    DECL_ON_FUNC_NOPARAM( EVENT_CHECK_RECOMMEND_ENABLE_REQ );
    DECL_ON_FUNC( EVENT_CHECK_RECOMMEND_ENABLE_ACK );
    DECL_ON_FUNC( EVENT_RECOMMEND_USER_REQ );
    DECL_ON_FUNC( EVENT_RECOMMEND_USER_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_RECOMMEND_FULL_INFO_REQ );
    DECL_ON_FUNC( EVENT_RECOMMEND_FULL_INFO_ACK );
   _DECL_ON_FUNC( EVENT_RECOMMENDER_ACCEPT_REQ, std::vector<DWORD> );
   _DECL_ON_FUNC( EVENT_RECOMMENDER_ACCEPT_ACK, KRecommenderAcceptDBAck );
   _DECL_ON_FUNC( EVENT_ATTENDANCE_BONUS_REQ, int );
    DECL_ON_FUNC( EVENT_ATTENDANCE_BONUS_ACK );
    DECL_ON_FUNC( EVENT_RECOM_DAILY_ATTENDANCE_ACK );
   _DECL_ON_FUNC( EVENT_RECOMMENDER_DENY_REQ, std::vector<DWORD> );
   _DECL_ON_FUNC( EVENT_RECOMMENDER_DENY_ACK, KRecommenderAcceptDBAck );
   _DECL_ON_FUNC( EVENT_RECOM_CHANGE_STATE_NOT, KPacketNameOK );
    DECL_ON_FUNC( EVENT_ONLINE_FRIENDS_NOT );
   _DECL_ON_FUNC( EVENT_SET_EXP_REQ, KAddExpInfo );
   _DECL_ON_FUNC( EVENT_GET_GCCLUB_CONNECT_BONUS_ACK, std::vector<KItem> );
   _DECL_ON_FUNC( EVENT_GET_GCCLUB_ATTEND_BONUS_ACK, std::vector<KItem> );
    DECL_ON_FUNC( EVENT_CLOSE_CONNECTION_NOT );

   _DECL_ON_FUNC( EVENT_CHANGE_VIRTUAL_CASH_COUPON_REQ, KComplexItem );
    DECL_ON_FUNC( EVENT_CHANGE_VIRTUAL_CASH_COUPON_ACK );

    // 090521. tgkwon. ping 통계
   _DECL_ON_FUNC( EVENT_USER_PING_INFO_NOT, VECTOR_PING_INFO );
   _DECL_ON_FUNC( EVENT_MISSION_PACK_REGIST_REQ, KSimpleItem );
    DECL_ON_FUNC( EVENT_MISSION_PACK_REGIST_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_RAINBOW_JACKPOT_ITEM_REQ );
    DECL_ON_FUNC( EVENT_RAINBOW_JACKPOT_ITEM_ACK );

    DECL_ON_FUNC( EVENT_EFFECT_TEX_REPORT );
   _DECL_ON_FUNC( EVENT_CO_OP_ITEM_BUY_REQ, KCoopEventItem );
    DECL_ON_FUNC( EVENT_CO_OP_ITEM_BUY_ACK );

   _DECL_ON_FUNC( EVENT_GET_USER_HACKING_COUNT_ACK, int );
    
    // 090810 tgkwon 아이템 교환
    DECL_ON_FUNC( EVENT_ITEM_TRADE_REQ );
    DECL_ON_FUNC( EVENT_ITEM_TRADE_ACK );

    // 090910. tgkwon. Mission Event 처리.
    DECL_ON_FUNC_NOPARAM( EVENT_MISSION_DATE_CHANGE_REQ );
    DECL_ON_FUNC( EVENT_MISSION_DATE_CHANGE_ACK );

   _DECL_ON_FUNC( EVENT_MY_SUBSCRIPTION_INFO_ACK, MAP_INT_DWORD );
   _DECL_ON_FUNC( EVENT_SUBSCRIPTION_INFO_UPDATE_NOT, KECN_SUBSCRIPTION_INFO_UPDATE_NOT );
    DECL_ON_FUNC_NOPARAM( EVENT_CURRENT_SUBSCRIPTION_GIFT_NUM_REQ );
    DECL_ON_FUNC( EVENT_SUBSCRIPT_REQ );
    DECL_ON_FUNC( EVENT_SUBSCRIPT_ACK );

    // NPC 아이템 지급
   _DECL_ON_FUNC( EVENT_NPC_GIFTS_REQ, int );
    DECL_ON_FUNC( EVENT_NPC_GIFTS_ACK );
   _DECL_ON_FUNC( EVENT_CHARACTER_STAT_INFO_REQ, PAIR_CHAR );
   _DECL_ON_FUNC( EVENT_CLIENT_HACKING_USER_NOT, int );

    // 091007.tgkwon 주사위 게임
    DECL_ON_FUNC( EVENT_GET_USER_BOARD_GAME_INFO );
    DECL_ON_FUNC_NOPARAM( EVENT_BOARD_GAME_INFO_REQ );
    DECL_ON_FUNC_NOPARAM( EVENT_DICE_PLAY_REQ );
    DECL_ON_FUNC( EVENT_DICE_PLAY_ACK );
    DECL_ON_FUNC( EVENT_DICE_ITEM_TRADE_REQ );
    DECL_ON_FUNC( EVENT_DICE_ITME_BUY_CHECK_REQ );
    DECL_ON_FUNC( EVENT_DICE_ITME_BUY_CHECK_ACK );

    // 090924 tgkwon 추석 송편 기부 이벤트
    DECL_ON_FUNC( EVENT_DONATION_ITEM_GET_REQ );
    DECL_ON_FUNC( EVENT_DONATION_ITEM_GET_ACK );
    DECL_ON_FUNC( EVENT_DONATION_INFO_ACK );
    
    // 09.12.07. tgkwon. 추천 (5th) 추가사항
    DECL_ON_FUNC( EVENT_RECOMMEND_LEVELUP_NOT );
    DECL_ON_FUNC_NOPARAM( EVENT_INVITE_FRIEND_LIST_REQ );
   _DECL_ON_FUNC( EVENT_JOIN_ROOM_OTHER_CHANNEL_REQ, KEVENT_INVITED_ROOM_FROM_SQUARE_REQ );

    // 091119.tslayer Guild
    DECL_ON_FUNC( EVENT_SEARCH_GUILD_LIST_REQ );
    DECL_ON_FUNC( EVENT_SEARCH_GUILD_LIST_ACK );
    DECL_ON_FUNC( EVENT_GUILD_LIST_REQ );
   _DECL_ON_FUNC( EVENT_INVITE_GUILD_REQ, DWORD );
    DECL_ON_FUNC( EVENT_JOIN_GUILD_REQ );
    DECL_ON_FUNC( EVENT_JOIN_GUILD_ACK );
   _DECL_ON_FUNC( EVENT_CANCEL_JOIN_GUILD_REQ, DWORD );
    DECL_ON_FUNC( EVENT_CANCEL_JOIN_GUILD_ACK );
    DECL_ON_FUNC( EVENT_CREATE_GUILD_REQ );
    DECL_ON_FUNC( EVENT_CREATE_GUILD_ACK );
   _DECL_ON_FUNC( EVENT_EDIT_GUILD_NOTICE_REQ, KNGuildNoticeList );
   _DECL_ON_FUNC( EVENT_EDIT_GUILD_NOTICE_ACK, KNGuildNoticeList );
    DECL_ON_FUNC( EVENT_EDIT_GUILD_NAME_REQ );
    DECL_ON_FUNC( EVENT_EDIT_GUILD_NAME_ACK );
   _DECL_ON_FUNC( EVENT_CHANGE_GUILD_JOIN_SETTING_REQ, KNGuildJoinPolicy );
   _DECL_ON_FUNC( EVENT_CHANGE_GUILD_JOIN_SETTING_ACK, KNGuildJoinPolicy );
   _DECL_ON_FUNC( EVENT_EDIT_GUILD_URL_REQ, KNGuildURL );
   _DECL_ON_FUNC( EVENT_EDIT_GUILD_URL_ACK, KNGuildURL );
    DECL_ON_FUNC( EVENT_BREAKUP_GUILD_REQ );
    DECL_ON_FUNC( EVENT_BREAKUP_GUILD_ACK );
    DECL_ON_FUNC( EVENT_CHANGE_GUILD_MEMBER_LEVEL_REQ );
    DECL_ON_FUNC( EVENT_CHANGE_GUILD_MEMBER_LEVEL_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_SELF_DRUMOUT_GUILD_REQ );
    DECL_ON_FUNC( EVENT_SELF_DRUMOUT_GUILD_ACK );
    DECL_ON_FUNC( EVENT_ACCEPT_GUILD_JOINER_REQ );
    DECL_ON_FUNC( EVENT_ACCEPT_GUILD_JOINER_ACK );
    DECL_ON_FUNC( EVENT_REJECT_GUILD_JOINER_REQ );
    DECL_ON_FUNC( EVENT_REJECT_GUILD_JOINER_ACK );
    DECL_ON_FUNC( EVENT_MARK_UPLOAD_REQ );
    DECL_ON_FUNC( EVENT_MARK_UPLOAD_ACK );
    DECL_ON_FUNC( EVENT_EDIT_GUILD_MY_COMMENT_REQ );
    DECL_ON_FUNC( EVENT_EDIT_GUILD_MY_COMMENT_ACK );
   _DECL_ON_FUNC( EVENT_GUILD_RANK_REQ, int );
   _DECL_ON_FUNC( EVENT_UPDATE_GUILD_STATE_NOT, PAIR_UCHAR );
    DECL_ON_FUNC_NOPARAM( EVENT_GUILD_NOTICE_LIST_REQ );
    DECL_ON_FUNC( EVENT_UPGRADE_GUILD_GRADE_GIFT_NOT );
    DECL_ON_FUNC( EVENT_CHANGE_GUILD_MEMBER_LEVEL_NOT );
    DECL_ON_FUNC( EVENT_GUILD_DRUMOUT_USER_NOT );

    // 091119. kkurrung. 퀴즈 빙고.
    DECL_ON_FUNC_NOPARAM( EVENT_USER_BINGO_DATA_REQ );
    DECL_ON_FUNC( EVENT_USER_BINGO_DATA_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_CHANGE_BINGO_COIN_REQ );
    DECL_ON_FUNC( EVENT_CHANGE_BINGO_COIN_ACK );
   _DECL_ON_FUNC( EVENT_BINGO_QUESTION_REQ, PAIR_INT );
    DECL_ON_FUNC( EVENT_BINGO_QUESTION_ACK );
    DECL_ON_FUNC( EVENT_BINGO_ANSWER_REQ );
   _DECL_ON_FUNC( EVENT_BINGO_ANSWER_ACK, KBingoAnswerAckData );

    // 대만 대회참가승리팀 예상 기부
    DECL_ON_FUNC( EVENT_TOURNAMENT_DONATION_REQ );
    DECL_ON_FUNC( EVENT_TOURNAMENT_DONATION_ACK );
   _DECL_ON_FUNC( EVENT_ITEM_BUY_CHECK_REQ, GCITEMID );
    DECL_ON_FUNC( EVENT_ITEM_BUY_CHECK_ACK );

    // 090811 tgkwon 웹 가챠 아이템 교환권 교환
    DECL_ON_FUNC( EVENT_WEB_GACHA_COIN_TRADE_REQ );
    DECL_ON_FUNC( EVENT_WEB_GACHA_COIN_TRADE_ACK );

   _DECL_ON_FUNC( EVENT_ADVERTISING_EXPOSURE_COUNT_NOT, PAIR_INT );
    
   // 091210. kkurrung. GWC Event
    DECL_ON_FUNC_NOPARAM( EVENT_GWC_EVENT_INFO_REQ );
    DECL_ON_FUNC( EVENT_GWC_EVENT_INFO_ACK );
   _DECL_ON_FUNC( EVENT_GWC_EVENT_DONATION_REQ, int );
    DECL_ON_FUNC( EVENT_GWC_EVENT_DONATION_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_GWC_EVENT_RESULT_REQ );

    DECL_ON_FUNC_NOPARAM( EVENT_GUILD_TOTAL_POINT_REQ );
    DECL_ON_FUNC( EVENT_GUILD_MEMBER_LIST_REQ );
    DECL_ON_FUNC( EVENT_GUILD_MEMBER_LIST_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_MY_GUILD_INFO_REQ );

    // 091230 tgkwon. 새해 소원 성취 이벤트
   _DECL_ON_FUNC( EVENT_NEW_YEAR_DONATION_INFO_REQ, DWORD );
    DECL_ON_FUNC( EVENT_NEW_YEAR_DONATION_REQ );
    DECL_ON_FUNC( EVENT_NEW_YEAR_DONATION_ACK );

    DECL_ON_FUNC( EVENT_SOCKET_OPEN_REQ );
    DECL_ON_FUNC( EVENT_SOCKET_OPEN_ACK );

    DECL_ON_FUNC( EVENT_MONSTER_CARD_INSERT_REQ );
    DECL_ON_FUNC( EVENT_MONSTER_CARD_INSERT_ACK );
    DECL_ON_FUNC( EVENT_MONSTER_CARD_REMOVE_REQ );
    DECL_ON_FUNC( EVENT_MONSTER_CARD_REMOVE_ACK );

   _DECL_ON_FUNC( EVENT_ATTRIBUTE_LIST_REQ, GCITEMUID );
    DECL_ON_FUNC( EVENT_ATTRIBUTE_SELECT_REQ  );
    DECL_ON_FUNC( EVENT_ATTRIBUTE_SELECT_ACK  );

   _DECL_ON_FUNC( EVENT_ITEM_BREAKUP_REQ, GCITEMUID );
    DECL_ON_FUNC( EVENT_ITEM_BREAKUP_ACK );

    DECL_ON_FUNC( DB_EVENT_TIME_DROP_ACK );
    DECL_ON_FUNC( DB_EVENT_GET_TIME_DROP_INFO_ACK );

    DECL_ON_FUNC( EVENT_GET_WEB_CASH_ITEM_REQ );
    DECL_ON_FUNC( EVENT_GET_WEB_CASH_ITEM_ACK );
    DECL_ON_FUNC( EVENT_STAT_RESOLUTION_NOT );

    DECL_ON_FUNC_NOPARAM( EVENT_MANUFACTURES3_CATALOG_REQ );
    DECL_ON_FUNC( EVENT_MANUFACTURES3_MATERIAL_REQ );
    DECL_ON_FUNC( EVENT_MANUFACTURES3_REQ );
    DECL_ON_FUNC( EVENT_MANUFACTURES3_ACK );

    DECL_ON_FUNC_NOPARAM( EVENT_ITEM_BREAKUP_PRICE_RATIO_REQ );
    
    // 100223. tgkwon. 선물상자 이벤트
    DECL_ON_FUNC_NOPARAM( EVENT_PRESENTBOX_INFO_REQ );
    DECL_ON_FUNC( EVENT_PRESENTBOX_TRADE_REQ );
    DECL_ON_FUNC( EVENT_PRESENTBOX_TRADE_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_PRESENTBOX_ACTION_REQ );
    DECL_ON_FUNC( EVENT_PRESENTBOX_ACTION_ACK );
   _DECL_ON_FUNC( EVENT_DUNGEON_STATE_REQ, std::set<int> );

    DECL_ON_FUNC( EVENT_GUILD_LIST_NOT );
   _DECL_ON_FUNC( EVENT_GUIDE_BOOL_CHECKPOINT_NOT, PAIR_SHORT );
   _DECL_ON_FUNC( EVENT_UPDATE_RECOM_USER_INFO, PAIR_INT );

    DECL_ON_FUNC_NOPARAM( EVENT_NORMAL_EXIT_NOT );
    DECL_ON_FUNC_NOPARAM( EVENT_GET_ROOMUSER_PRESS_STATE_REQ );
   _DECL_ON_FUNC( EVENT_SET_PRESS_STATE_REQ, int );
   _DECL_ON_FUNC( EVENT_PRESSURE_USER_REQ, DWORD );
    DECL_ON_FUNC_NOPARAM( EVENT_GET_ROOMUSER_IDLE_STATE_REQ );
   _DECL_ON_FUNC( EVENT_SET_IDLE_STATE_REQ, int );

   _DECL_ON_FUNC( EVENT_CLIENT_PING_REPORT_NOT, DWORD );
    DECL_ON_FUNC_NOPARAM( EVENT_ROOM_MEMBER_PING_INFO_REQ );

   _DECL_ON_FUNC( EVENT_RELAY_LOADING_STATE, PAIR_DWORD_INT );
    DECL_ON_FUNC( EVENT_BUY_FOR_GAMBLE_REQ );
    DECL_ON_FUNC( EVENT_BUY_FOR_GAMBLE_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_COST_RATE_FOR_GAMBLE_BUY_REQ );

    // 룩 변환
   _DECL_ON_FUNC( EVENT_CHANGE_NORMAL_TO_LOOK_REQ, std::vector<GCITEMUID> );
   _DECL_ON_FUNC( EVENT_CHANGE_NORMAL_TO_LOOK_ACK, KChangeLookItem );
   _DECL_ON_FUNC( EVENT_CHANGE_LOOK_TO_NORMAL_REQ, std::vector<GCITEMUID> );
   _DECL_ON_FUNC( EVENT_CHANGE_LOOK_TO_NORMAL_ACK, KChangeLookItem );
   _DECL_ON_FUNC( EVENT_SAVE_COORDI_REQ, KSaveCoordiInfo );
    DECL_ON_FUNC( EVENT_CHANGE_LOOK_EQUIP_REQ );
   _DECL_ON_FUNC( EVENT_FULL_LOOK_INFO_NOT, KLookFullInfo );

   // 아이템 묶어 팔기.
    DECL_ON_FUNC( EVENT_BUNDLE_SELL_ITEM_REQ );
    DECL_ON_FUNC( EVENT_BUNDLE_SELL_ITEM_ACK );

    // C->S 던전 통계 전용
    DECL_ON_FUNC( EVENT_STAT_END_GAME_INFO );
    DECL_ON_FUNC_NOPARAM( EVENT_TR_SERVER_INFO_REQ );
    
    // DB->S 채널링 이벤트 아이템 지급.
    _DECL_ON_FUNC( EVENT_CHANNELING_REWARD_NOT, std::vector<KItem> );

    // 캐릭터 전직 마법시 시스템.
    DECL_ON_FUNC( EVENT_CHAR_PROMOTION_UPDATE_REQ );
    DECL_ON_FUNC( EVENT_CHAR_PROMOTION_UPDATE_ACK );

    DECL_ON_FUNC( EVENT_USER_HERO_DUNGEON_INFO_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_HERO_ITEM_CATALOG_REQ );
    DECL_ON_FUNC( EVENT_HERO_ITEM_MATERIAL_REQ );
   _DECL_ON_FUNC( EVENT_BUY_FOR_HERO_REQ, PAIR_USHORT_PAIR_DWORD_INT );
    DECL_ON_FUNC( EVENT_BUY_FOR_HERO_ACK );

   _DECL_ON_FUNC( EVENT_GAWIBAWIBO_INFO_REQ, int );
   _DECL_ON_FUNC( DB_EVENT_GAWIBAWIBO_INFO_ACK, KUserGawibawiboInfo );
    DECL_ON_FUNC_NOPARAM( EVENT_GAWIBAWIBO_REQ );
    DECL_ON_FUNC( EVENT_GAWIBAWIBO_ACK );
   _DECL_ON_FUNC( EVENT_GAWIBAWIBO_GIVE_UP_REQ, int );
   _DECL_ON_FUNC( EVENT_GAWIBAWIBO_ITEM_TRADE_REQ, GCITEMID );
    DECL_ON_FUNC_NOPARAM( EVENT_GAWIBAWIBO_TRYPOINT_INFO_REQ );
   _DECL_ON_FUNC( EVENT_GAWIBAWIBO_INIT_INFO_REQ, DWORD );

    DECL_ON_FUNC_NOPARAM( EVENT_STAGE_LOAD_COMPLETE_NOT );
    DECL_ON_FUNC( EVENT_INIT_ITEM_ATTRIBUTE_REQ );
    DECL_ON_FUNC( EVENT_INIT_ITEM_ATTRIBUTE_ACK );

   _DECL_ON_FUNC( DB_EVENT_SOCKS_INFO_ACK, KUserSocksInfo );
   _DECL_ON_FUNC( EVENT_SOCKS_HANGUP_REQ, std::vector< GCITEMID > );
   _DECL_ON_FUNC( EVENT_SOCKS_HANGUP_ACK, KSocksResult );

   _DECL_ON_FUNC( EVENT_SOCKS_COLLECT_REQ, std::vector< GCITEMID > );
   _DECL_ON_FUNC( EVENT_SOCKS_COLLECT_ACK, KSocksResult );
    DECL_ON_FUNC( EVENT_SOCKS_MATERIAL_EXCHANGE_NOT );
   _DECL_ON_FUNC( EVENT_SOCKS_HANGUP_COMPLETE_NOT, KSocksResult );
    DECL_ON_FUNC_NOPARAM( EVENT_SOCKS_MATERIAL_EXCHANGE_REQ );
   _DECL_ON_FUNC( EVENT_SOCKS_MATERIAL_EXCHANGE_ACK, KSocksExchangeData );

   _DECL_ON_FUNC( EVENT_SPECIAL_REWARD_REQ, KRewardInfoList );
    DECL_ON_FUNC( EVENT_SPECIAL_REWARD_ACK );

   _DECL_ON_FUNC( EVENT_DUNGEON_REWARD_GP_NOT, std::set<DWORD> );
    DECL_ON_FUNC( EVENT_DUNGEON_REWARD_ITEM_REQ );
    DECL_ON_FUNC( EVENT_DUNGEON_REWARD_ITEM_ACK );
   _DECL_ON_FUNC( EVENT_DUNGEON_REWARD_EXP_REQ, int );
    DECL_ON_FUNC( EVENT_PVP_REWARD_EXP_GP_REQ );
    DECL_ON_FUNC( EVENT_PVP_TAG_REWARD_EXP_GP_REQ );
   _DECL_ON_FUNC( EVENT_UNLOCK_CHANGE_WEAPON_REQ, char );
    DECL_ON_FUNC( EVENT_UNLOCK_CHANGE_WEAPON_ACK );

   _DECL_ON_FUNC( EVENT_END_GAME_BROAD, KEndGameAck );
    DECL_ON_FUNC( EVENT_ITEM_ATTRIBUTE_RANDOM_SELECT_REQ );
    DECL_ON_FUNC( EVENT_ITEM_ATTRIBUTE_RANDOM_SELECT_ACK );

   _DECL_ON_FUNC( EVENT_USER_SERVER_ROOM_REQ, PAIR_DWORD_WSTRING );
    DECL_ON_FUNC( EVENT_USER_SERVER_ROOM_ACK );

   _DECL_ON_FUNC( EVENT_TODAYS_POPUP_INFO_ACK, int );
    DECL_ON_FUNC_NOPARAM( EVENT_DISABLE_TODAYS_POPUP_NOT );
    DECL_ON_FUNC( EVENT_RESET_SKILL_REQ );
    DECL_ON_FUNC_NOPARAM( EVENT_FASHION_CATALOG_REQ );
    DECL_ON_FUNC( EVENT_FASHION_CATALOG_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_USER_RECOM_TYPE_REQ );
   _DECL_ON_FUNC( EVENT_USER_RECOM_TYPE_ACK, int );

    // 110222. tgkwon. 나무나무 이벤트.
    DECL_ON_FUNC_NOPARAM( EVENT_PLANT_TREE_INFO_REQ );
    DECL_ON_FUNC( EVENT_PLANT_TREE_INFO_ACK );
    DECL_ON_FUNC( EVENT_PLANT_TREE_ACTION_REQ );
    DECL_ON_FUNC( EVENT_PLANT_TREE_ACTION_ACK );
   _DECL_ON_FUNC( EVENT_PLANT_TREE_REWARD_REQ, DWORD );
    DECL_ON_FUNC( EVENT_PLANT_TREE_REWARD_ACK );

    DECL_ON_FUNC_NOPARAM( EVENT_GUILD_STORE_CATALOG_REQ );
    DECL_ON_FUNC( EVENT_BUY_FOR_GUILD_REQ );
    DECL_ON_FUNC( EVENT_BUY_FOR_GUILD_ACK );

    DECL_ON_FUNC_NOPARAM( EVENT_CHOICE_BOX_LIST_REQ );
   _DECL_ON_FUNC( EVENT_CHOICE_BOX_INFO_REQ, GCITEMID );
    DECL_ON_FUNC( EVENT_CHOICE_BOX_OPEN_REQ );
    DECL_ON_FUNC( EVENT_CHOICE_BOX_OPEN_ACK );

    DECL_ON_FUNC( EVENT_USER_AUTH_CHECK_REQ );
    DECL_ON_FUNC( EVENT_USER_AUTH_CHECK_ACK );
   _DECL_ON_FUNC( EVENT_ITEM_ATTRIBUTE_TABLE_ID_REQ, GCITEMUID );
    DECL_ON_FUNC_NOPARAM( EVENT_VIPEVENT_LIST_REQ );
   _DECL_ON_FUNC( EVENT_VIPEVENT_USER_INFO_REQ, USHORT );
    DECL_ON_FUNC( EVENT_VIPEVENT_USER_INFO_ACK );
    DECL_ON_FUNC( EVENT_VIPEVENT_USER_REWARD_UPDATE_NOT );

    DECL_ON_FUNC_NOPARAM( EVENT_MAGIC_BOX_LIST_REQ );
    DECL_ON_FUNC( EVENT_MAGIC_BOX_LIST_ACK );
   _DECL_ON_FUNC( EVENT_SET_MAGIC_BOX_TARGET_REQ, GCITEMID );
    DECL_ON_FUNC( EVENT_SET_MAGIC_BOX_TARGET_ACK );
    DECL_ON_FUNC( EVENT_USE_MAGIC_BOX_REQ );
    DECL_ON_FUNC( EVENT_USE_MAGIC_BOX_ACK );

    _DECL_ON_FUNC( EVENT_ITEM_LEVEL_LIST_REQ, GCITEMUID );
    DECL_ON_FUNC( EVENT_ITEM_LEVEL_SELECT_REQ );
    DECL_ON_FUNC( EVENT_ITEM_LEVEL_SELECT_ACK );

    DECL_ON_FUNC_NOPARAM( EVENT_PACKAGE_INFO_REQ );
    DECL_ON_FUNC_NOPARAM( EVENT_OPEN_CALENDAR_BONUS_POINT_REQ );
    DECL_ON_FUNC( EVENT_OPEN_CALENDAR_BONUS_POINT_ACK );

    DECL_ON_FUNC( EVENT_RKTORNADO_ACTION_REQ );
    DECL_ON_FUNC( EVENT_RKTORNADO_ACTION_ACK );
    DECL_ON_FUNC( EVENT_RKTORNADO_ITEM_LIST_NOT );

    DECL_ON_FUNC_NOPARAM( EVENT_USER_SPHINX_DATA_REQ ); // 스핑크스 OX게임.
    DECL_ON_FUNC( EVENT_USER_SPHINX_DATA_ACK );
    DECL_ON_FUNC( EVENT_SPHINX_QUESTION_REQ );
    DECL_ON_FUNC( EVENT_SPHINX_QUESTION_ACK );
    DECL_ON_FUNC( EVENT_SPHINX_ANSWER_REQ );
    DECL_ON_FUNC( EVENT_SPHINX_ANSWER_ACK );

    DECL_ON_FUNC( EVENT_CREATE_PARTY_REQ );
   _DECL_ON_FUNC( EVENT_CREATE_PARTY_ANSWER, KPartyData );
    DECL_ON_FUNC( EVENT_INVITE_PARTY_REQ );

   _DECL_ON_FUNC( EVENT_INVITE_PARTY_ANSWER, KPartyData );
   _DECL_ON_FUNC( EVENT_CHANGE_PARTY_HOST_REQ, DWORD );
   _DECL_ON_FUNC( EVENT_LEAVE_PARTY_USER_REQ, DWORD );
   _DECL_ON_FUNC( EVENT_LEAVE_PARTY_BROAD, KPartyData );
   _DECL_ON_FUNC( EVENT_CREATE_PARTY_ASK, KPartyData );
   _DECL_ON_FUNC( EVENT_CREATE_PARTY_ACK, KPartyData );
    DECL_ON_FUNC_NOPARAM( EVENT_INVITE_PARTY_ROOM_REQ );
   _DECL_ON_FUNC( EVENT_CHANGE_PARTY_HOST_BROAD, KPartyData );
   _DECL_ON_FUNC( EVENT_BREAK_PARTY_NOT, KPartyData );
   _DECL_ON_FUNC( EVENT_BAN_PARTY_USER_REQ, DWORD );
   _DECL_ON_FUNC( EVENT_BAN_PARTY_USER_NOT, KPartyData );
   _DECL_ON_FUNC( EVENT_INVITE_PARTY_ASK, KPartyData );
   _DECL_ON_FUNC( EVENT_INVITE_PARTY_BROAD, KPartyData );

    DECL_ON_FUNC( EVENT_STRENGTH_EQUIP_REQ ); // 강화석 강화 시스템.
    DECL_ON_FUNC( EVENT_STRENGTH_EQUIP_ACK );
    DECL_ON_FUNC( EVENT_STRENGTH_ACTION_REQ );
    DECL_ON_FUNC( EVENT_STRENGTH_ACTION_ACK );
    DECL_ON_FUNC( EVENT_STRENGTH_BREAKUP_REQ );
    DECL_ON_FUNC( EVENT_STRENGTH_BREAKUP_ACK );
    DECL_ON_FUNC( EVENT_STRENGTH_COMPOSE_REQ );
    DECL_ON_FUNC( EVENT_STRENGTH_COMPOSE_ACK );

   _DECL_ON_FUNC( EVENT_ENTER_AGIT_REQ, KEnterAgitReq );
   _DECL_ON_FUNC( EVENT_ENTER_AGIT_NOT, KEnterAgitAck );
   _DECL_ON_FUNC( EVENT_ENTER_AGIT_ACK, KEnterAgitAck );
    DECL_ON_FUNC( EVENT_ENTER_AGIT_BROAD );
    DECL_ON_FUNC_NOPARAM( EVENT_AGIT_LOADING_COMPLETE_REQ );
   _DECL_ON_FUNC( EVENT_AGIT_LOADING_COMPLETE_ACK, KAgitLoadingCompleteAck );
    DECL_ON_FUNC_NOPARAM( EVENT_AGIT_LOADING_FAIL_NOT );
    DECL_ON_FUNC_NOPARAM( EVENT_LEAVE_AGIT_REQ );
   _DECL_ON_FUNC( EVENT_LEAVE_AGIT_ACK, DWORD );
    DECL_ON_FUNC( EVENT_LEAVE_AGIT_BROAD );
    DECL_ON_FUNC( EVENT_AGIT_VISITER_COUNT_BROAD );

    DECL_ON_FUNC_NOPARAM( EVENT_AGIT_MAP_CATALOGUE_REQ );
   _DECL_ON_FUNC( EVENT_BUY_AGIT_MAP_REQ, DWORD );
   _DECL_ON_FUNC( EVENT_BUY_AGIT_MAP_NOT, KBuyAgitMapAck );
   _DECL_ON_FUNC( EVENT_BUY_AGIT_MAP_ACK, KBuyAgitMapAck );
    DECL_ON_FUNC( EVENT_CHANGE_AGIT_MAP_BROAD );
   _DECL_ON_FUNC( EVENT_SET_AGIT_OPTION_REQ, KAgitOption );

    DECL_ON_FUNC_NOPARAM( EVENT_AGIT_STORE_CATALOG_REQ );
    DECL_ON_FUNC( EVENT_AGIT_STORE_MATERIAL_REQ );
   _DECL_ON_FUNC( EVENT_BUY_FOR_AGIT_REQ, PAIR_USHORT_DWORD );
    DECL_ON_FUNC( EVENT_BUY_FOR_AGIT_ACK );
   _DECL_ON_FUNC( EVENT_SELL_FOR_AGIT_REQ, std::vector<GCITEMUID> );
    DECL_ON_FUNC( EVENT_SELL_FOR_AGIT_ACK );

   _DECL_ON_FUNC( EVENT_SEED_FLOWER_POT_REQ, KSeedFlowerPotReq );
   _DECL_ON_FUNC( EVENT_SEED_FLOWER_POT_ACK, KSeedFlowerPotAck );
    DECL_ON_FUNC( EVENT_CHANGE_FLOWER_POT_STATE_BROAD );
   _DECL_ON_FUNC( EVENT_HARVEST_FLOWER_POT_REQ, KHarvestFlowerPotReq );
   _DECL_ON_FUNC( EVENT_HARVEST_FLOWER_POT_NOT, KHarvestFlowerPotAck );
   _DECL_ON_FUNC( EVENT_HARVEST_FLOWER_POT_ACK, KHarvestFlowerPotAck );
   _DECL_ON_FUNC( EVENT_AGIT_STAY_BONUS_DROP_ACK, KAgitBonusDrop );
    DECL_ON_FUNC( EVENT_TODAYS_SEED_DROP_COUNT_ACK );

   _DECL_ON_FUNC( EVENT_AGIT_CHAT_REQ, KChatData );
   _DECL_ON_FUNC( EVENT_AGIT_CHAT_ACK, KAgitChatAck );
   _DECL_ON_FUNC( EVENT_AGIT_CHAT_BROAD, KChatData );
   _DECL_ON_FUNC( EVENT_AGIT_BAN_USER_REQ, KAgitBanUserReq );
   _DECL_ON_FUNC( EVENT_AGIT_BAN_USER_ACK, KAgitBanUserAck );
   _DECL_ON_FUNC( EVENT_AGIT_BAN_USER_NOT, DWORD );

   _DECL_ON_FUNC( EVENT_SET_AGIT_OBJECT_POS_REQ, KSetAgitObjectPosReq );
   _DECL_ON_FUNC( EVENT_SET_AGIT_OBJECT_POS_ACK, KSetAgitObjectPosAck );
    DECL_ON_FUNC( EVENT_CHANGE_AGIT_OBJECT_POS_BROAD );

   _DECL_ON_FUNC( EVENT_AGIT_FRIEND_LIST_REQ, KAgitFriendListReq );
    DECL_ON_FUNC_NOPARAM( EVENT_AGIT_TOP_RANK_LIST_REQ );
    DECL_ON_FUNC_NOPARAM( EVENT_AGIT_FAVORITE_LIST_REQ );
   _DECL_ON_FUNC( EVENT_AGIT_ADD_FAVORITE_REQ, KAgitUserParam );
   _DECL_ON_FUNC( EVENT_AGIT_REMOVE_FAVORITE_REQ, KAgitUserParam );

   _DECL_ON_FUNC( EVENT_REGISTER_AGIT_PAGE_REQ, KRegisterAgitPageReq );
    DECL_ON_FUNC_NOPARAM( EVENT_UNREGISTER_AGIT_PAGE_REQ );
   _DECL_ON_FUNC( EVENT_AGIT_PAGE_LIST_REQ, KAgitPageList );

    DECL_ON_FUNC_NOPARAM( EVENT_BROWNIE_STORE_CATALOG_REQ );
    DECL_ON_FUNC( EVENT_BROWNIE_STORE_MATERIAL_REQ );
   _DECL_ON_FUNC( EVENT_BUY_FOR_BROWNIE_REQ, PAIR_USHORT_DWORD );
    DECL_ON_FUNC( EVENT_BUY_FOR_BROWNIE_ACK );
   _DECL_ON_FUNC( EVENT_EQUIP_BROWNIE_REQ, KEquipBrownieReq );
    DECL_ON_FUNC( EVENT_CHANGE_BROWNIE_BROAD );

    DECL_ON_FUNC_NOPARAM( EVENT_WATERING_FAIRY_TREE_REQ );
    DECL_ON_FUNC( EVENT_CHANGE_FAIRY_TREE_BROAD );
   _DECL_ON_FUNC( EVENT_FAIRY_TREE_BUFF_NOT, KFairyTreeFruitBuff );
    DECL_ON_FUNC_NOPARAM( EVENT_USE_FAIRY_TREE_FRUIT_REQ );
   _DECL_ON_FUNC( EVENT_USE_FAIRY_TREE_FRUIT_ACK, KUseFairyTreeFruitAck );

   _DECL_ON_FUNC( EVENT_AGIT_INIT_SEED_ACK, KAgitBonusDrop );
   _DECL_ON_FUNC( EVENT_DROP_FLOWER_POT_ACK, KAddItemAck );
    DECL_ON_FUNC_NOPARAM( EVENT_AGIT_TUTORIAL_DONE_REQ );
   _DECL_ON_FUNC( EVENT_AGIT_TUTORIAL_DONE_ACK, bool );
    DECL_ON_FUNC_NOPARAM( EVENT_AGIT_TUTORIAL_REQ );
    DECL_ON_FUNC( EVENT_AGIT_TUTORIAL_ACK );
   _DECL_ON_FUNC( EVENT_INVITE_AGIT_REQ, std::vector<DWORD> );
   _DECL_ON_FUNC( EVENT_INVITE_AGIT_NOT, KInviteAgitNot );
   _DECL_ON_FUNC( EVENT_ENTER_AGIT_SID_ACK, KEnterAgitSIDAck );
   _DECL_ON_FUNC( EVENT_CHANGE_AGIT_OPTION_BROAD, KAgitOption );
   _DECL_ON_FUNC( EVENT_AGIT_USER_OPTION_ACK, KAgitInfo );

    DECL_ON_FUNC_NOPARAM( EVENT_GUESTBOOK_PAGE_REQ );
   _DECL_ON_FUNC( EVENT_GUESTBOOK_WRITE_REQ, KGuestBookWriteReq );
   _DECL_ON_FUNC( EVENT_GUESTBOOK_WRITE_BROAD, KGuestBookWriteBroad );
   _DECL_ON_FUNC( EVENT_GUESTBOOK_DELETE_REQ, KGuestBookDeleteReq );
   _DECL_ON_FUNC( EVENT_GUESTBOOK_DELETE_BROAD, KGuestBookDeleteBroad );

   _DECL_ON_FUNC( EVENT_USE_TRAINING_OBJ_REQ, KUseTrainingObjReq );
   _DECL_ON_FUNC( EVENT_USE_TRAINING_OBJ_ACK, KUseTrainingObjAck );
   _DECL_ON_FUNC( EVENT_UPDATE_TRAINING_OBJ_BROAD, KUpdateTrainingObjBroad );

   _DECL_ON_FUNC( EVENT_OPEN_AGIT_CHARACTER_REQ, int );
   _DECL_ON_FUNC( EVENT_OPEN_AGIT_CHARACTER_ACK, KOpenAgitCharacterAck );
   _DECL_ON_FUNC( EVENT_SET_AGIT_CHARACTER_POS_REQ, KSetAgitCharPosReq );
   _DECL_ON_FUNC( EVENT_SET_AGIT_CHARACTER_MOTION_REQ, KSetAgitCharMotionReq );
    DECL_ON_FUNC( EVENT_SET_AGIT_CHARACTER_COORDI_REQ );
   _DECL_ON_FUNC( EVENT_SET_AGIT_CHARACTER_SPEECH_REQ, KSetAgitCharSpeechReq );
   _DECL_ON_FUNC( EVENT_UPDATE_AGIT_CHARACTER_BROAD, KUpdateAgitCharBroad );
   _DECL_ON_FUNC( EVENT_UPDATE_AGIT_CHARACTER_POS_BROAD, KSetAgitCharPosReq );
    DECL_ON_FUNC_NOPARAM( EVENT_FAIRY_TREE_LV_TABLE_REQ );

    DECL_ON_FUNC( EVENT_SHUTDOWN_ALARM_NOT );
    DECL_ON_FUNC_NOPARAM( EVENT_INVEN_BUFF_ITEM_LIST_REQ );
   _DECL_ON_FUNC( EVENT_NMNETCAFE_LOGIN_ACK, KNmNetcafeParamAck );
   _DECL_ON_FUNC( EVENT_NMNETCAFE_EXPIRE_NOT, KNmNetcafeParamAck );
    DECL_ON_FUNC( EVENT_NMNETCAFE_BONUS_ACK );
   _DECL_ON_FUNC( EVENT_NMNETCAFE_LOGOUT_ACK, UINT ); // int 는 에러값

    DECL_ON_FUNC( EVENT_SURVEY_REWARD_REQ );
    DECL_ON_FUNC( EVENT_SURVEY_REWARD_ACK );
    DECL_ON_FUNC( EVENT_SURVEY_LIST_NOT );
    DECL_ON_FUNC_NOPARAM( EVENT_SHA_ENABLE_REQ );
    
    DECL_ON_FUNC_NOPARAM( EVENT_GWC_RANKING_LIST_REQ );
    DECL_ON_FUNC_NOPARAM( EVENT_GWC_RANKING_REWARD_REQ );
    DECL_ON_FUNC( EVENT_GWC_RANKING_REWARD_ACK );

    DECL_ON_FUNC( EVENT_JUST_IN_TIME_REWARD_REQ );
    DECL_ON_FUNC( EVENT_JUST_IN_TIME_REWARD_ACK );
    DECL_ON_FUNC( EVENT_JUST_IN_TIME_ON_TIME_NOT );

    DECL_ON_FUNC_NOPARAM( EVENT_CHARISMAS_EVENT_INFO_REQ );
    DECL_ON_FUNC( EVENT_CHARISMAS_EVENT_INFO_ACK );
   _DECL_ON_FUNC( EVENT_CHARISMAS_EVENT_CONNECT_CHECK_REQ, std::wstring );
    DECL_ON_FUNC( EVENT_CHARISMAS_EVENT_CONNECT_CHECK_ACK );
    DECL_ON_FUNC( EVENT_CHARISMAS_EVENT_PRESENT_REQ );
    DECL_ON_FUNC( EVENT_CHARISMAS_EVENT_PRESENT_ACK );
    DECL_ON_FUNC( EVENT_CHARISMAS_EVENT_PRESENT_NOT );
    DECL_ON_FUNC( EVENT_CHARISMAS_EVENT_DONATION_REQ );
    DECL_ON_FUNC( EVENT_CHARISMAS_EVENT_DONATION_ACK );
    DECL_ON_FUNC( EVENT_CHARISMAS_EVENT_GRADE_REWARD_REQ );
    DECL_ON_FUNC( EVENT_CHARISMAS_EVENT_GRADE_REWARD_ACK );

    DECL_ON_FUNC( EVENT_STAT_HOTKEY_USED_INFO_NOT );

    DECL_ON_FUNC( DB_EVENT_INIT_POST_LETTER_LIST_ACK );
    DECL_ON_FUNC( EVENT_GET_POST_LETTER_LIST_REQ );
    DECL_ON_FUNC( EVENT_SEND_LETTER_REQ );
    DECL_ON_FUNC( EVENT_SEND_LETTER_ACK );
   _DECL_ON_FUNC( EVENT_FIND_USER_SEND_LETTER_NOT, DWORD );
   _DECL_ON_FUNC( EVENT_RECEIVE_LETTER_NOT, DWORD );
   _DECL_ON_FUNC( EVENT_READ_LETTER_REQ, POSTUID );
   _DECL_ON_FUNC( EVENT_GET_ITEM_FROM_LETTER_REQ, POSTUID );
    DECL_ON_FUNC( EVENT_GET_ITEM_FROM_LETTER_ACK );
    DECL_ON_FUNC( EVENT_DELETE_LETTER_REQ );
    DECL_ON_FUNC( EVENT_DELETE_LETTER_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_RENEWAL_LETTER_REQ );
    DECL_ON_FUNC( EVENT_NEW_POST_LETTER_INFO_ACK );
    DECL_ON_FUNC( EVENT_DUNGEON_CLEAR_REWARD_REQ );
    DECL_ON_FUNC( EVENT_DUNGEON_CLEAR_REWARD_ACK );
    DECL_ON_FUNC( EVENT_EQUIP_LEVEL_DOWN_REQ );
    DECL_ON_FUNC( EVENT_EQUIP_LEVEL_DOWN_ACK );
    DECL_ON_FUNC( EVENT_MONSTER_CARD_MIX_REQ );
    DECL_ON_FUNC( EVENT_MONSTER_CARD_MIX_ACK );

    DECL_ON_FUNC_NOPARAM( EVENT_SOCKS_INFO_REQ );

   _DECL_ON_FUNC( EVENT_ECLIPSE_TIME_EVENT_INFO_NOT, DWORD );
    DECL_ON_FUNC_NOPARAM( EVENT_ECLIPSE_PLOT_TIME_REWARD_REQ );
    DECL_ON_FUNC( EVENT_ECLIPSE_PLOT_TIME_REWARD_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_ECLIPSE_PLOT_INFO_REQ );
   _DECL_ON_FUNC( EVENT_ECLIPSE_PLOT_INFO_ACK, KEclipsePlotData );
    DECL_ON_FUNC( EVENT_ECLIPSE_PLOT_HUNT_REWARD_REQ );
    DECL_ON_FUNC( EVENT_ECLIPSE_PLOT_HUNT_REWARD_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_ECLIPSE_PLOT_FINAL_REWARD_REQ );
    DECL_ON_FUNC( EVENT_ECLIPSE_PLOT_FINAL_REWARD_ACK );
   _DECL_ON_FUNC( EVENT_SIGN_BOARD_ECLIPSE_ALARM_NOT, int );

    DECL_ON_FUNC_NOPARAM( EVENT_ECLIPSE_COLLECT_INFO_REQ );
    DECL_ON_FUNC_NOPARAM( EVENT_ECLIPSE_COLLECT_REWARD_REQ );
    DECL_ON_FUNC( EVENT_ECLIPSE_COLLECT_REWARD_ACK );
   _DECL_ON_FUNC( EVENT_ECLIPSE_COLLECT_REWARD_ANSWER_NOT, bool );

   _DECL_ON_FUNC( EVENT_USE_EXP_POTION_REQ, KExpPotionReq );
    DECL_ON_FUNC_NOPARAM( EVENT_EXP_POTION_LIST_REQ );
   _DECL_ON_FUNC( EVENT_DEPOT_CHAR_TAB_INFO_REQ, KDepotInfo );
    DECL_ON_FUNC( EVENT_DEPOT_CHAR_TAB_INFO_NOT );

   _DECL_ON_FUNC( EVENT_DEPOT_INSERT_ITEM_REQ, KDepotItem );
   _DECL_ON_FUNC( EVENT_DEPOT_DELETE_ITEM_REQ, KDepotItem );
    DECL_ON_FUNC( EVENT_DEPOT_MOVE_ITEM_REQ );
    DECL_ON_FUNC( EVENT_DEPOT_EXTEND_REQ );
    DECL_ON_FUNC( EVENT_DEPOT_EXTEND_ACK );
    DECL_ON_FUNC( EVENT_DEPOT_CREATE_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_DEPOT_INFO_REQ );
    DECL_ON_FUNC( EVENT_DEPOT_INFO_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_UPDATE_DEPOT_DATA_REQ );
   _DECL_ON_FUNC( EVENT_UPDATE_DEPOT_DATA_ACK, KDepotUpdateData );

    DECL_ON_FUNC_NOPARAM( DB_EVENT_SELECT_GACHA_NOTICE_POPUP_INFO );
   _DECL_ON_FUNC( EVENT_GACHA_NOTICE_POPUP_INFO_NOT, bool );
    DECL_ON_FUNC_NOPARAM( EVENT_UPDATE_GACHA_POPUP_INFO );

   _DECL_ON_FUNC( EVENT_GACHA_LEVEL_ACTION_REQ, KEVENT_GACHA_ACTION_REQ );
    DECL_ON_FUNC_NOPARAM( EVENT_GACHA_LEVEL_OBTAINED_SET_REWARD_REQ );
   _DECL_ON_FUNC( EVENT_GACHA_LEVEL_SET_REWARD_REQ, KEVENT_GACHA_SET_REWARD_REQ );
   _DECL_ON_FUNC( EVENT_GACHA_LEVEL_SET_REWARD_ACK, KEVENT_GACHA_SET_REWARD_ACK );

    DECL_ON_FUNC( EVENT_ADD_SKILL_SLOT_OPEN_REQ );
    DECL_ON_FUNC( EVENT_ADD_SKILL_SLOT_OPEN_ACK );

    DECL_ON_FUNC_NOPARAM( EVENT_MONSTER_CARD_MIX_INFO_REQ );
    DECL_ON_FUNC_NOPARAM( EVENT_SONGKRAN_SCRIPT_INFO_REQ );
    DECL_ON_FUNC_NOPARAM( EVENT_SONGKRAN_USER_INFO_REQ );
    DECL_ON_FUNC( DB_EVENT_SONGKRAN_USER_INFO_ACK );
   _DECL_ON_FUNC( EVENT_SONGKRAN_WATERBOMB_USE_REQ, int );
    DECL_ON_FUNC_NOPARAM( EVENT_SONGKRAN_GRADE_REWARD_REQ );
    DECL_ON_FUNC( EVENT_SONGKRAN_GRADE_REWARD_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_SONGKRAN_WATERBOMB_EXCHANGEITEM_USE_REQ );
    DECL_ON_FUNC( EVENT_SONGKRAN_WATERBOMB_EXCHANGEITEM_USE_ACK );
    DECL_ON_FUNC( EVENT_SONGKRAN_WATERBOMB_USE_ACK );
   _DECL_ON_FUNC( DB_EVENT_SONGKRAN_USER_INFO_REQ, bool );

    DECL_ON_FUNC( EVENT_ATTEND_GET_CHAR_ACK );

    DECL_ON_FUNC_NOPARAM( EVENT_ADVENTURE_DATA_REQ );
    DECL_ON_FUNC( EVENT_ADVENTURE_DATA_ACK );
    DECL_ON_FUNC( EVENT_ADVENTURE_CONTINENT_REWARD_REQ );
    DECL_ON_FUNC( EVENT_ADVENTURE_CONTINENT_REWARD_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_ADVENTURE_FINAL_REWARD_REQ );
    DECL_ON_FUNC( EVENT_ADVENTURE_FINAL_REWARD_ACK );

    DECL_ON_FUNC( EVENT_TEST_ADD_ITEM_REQ );
    DECL_ON_FUNC( EVENT_TEST_ADD_ITEM_ACK );

    DECL_ON_FUNC( EVENT_NEW_CHAR_CHOICE_REQ );
    DECL_ON_FUNC( EVENT_NEW_CHAR_CHOICE_ACK );

    DECL_ON_FUNC_NOPARAM( EVENT_OLYMPIC_SCHEDULE_REQ );
    DECL_ON_FUNC_NOPARAM( EVENT_OLYMPIC_INFO_REQ );
    DECL_ON_FUNC( EVENT_OLYMPIC_INFO_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_OLYMPIC_JOIN_GAME_REQ );
    DECL_ON_FUNC( EVENT_OLYMPIC_JOIN_GAME_ACK );
    DECL_ON_FUNC( EVENT_OLYMPIC_ALARM_NOT );

    DECL_ON_FUNC( EVENT_OLYMPIC_WATERBOMB_RESULT_REQ );
    DECL_ON_FUNC( EVENT_OLYMPIC_WATERBOMB_RESULT_ACK );
    DECL_ON_FUNC( EVENT_OLYMPIC_GAWIBAWIBO_RESULT_REQ );
    DECL_ON_FUNC( EVENT_OLYMPIC_GAWIBAWIBO_RESULT_ACK );
    DECL_ON_FUNC( EVENT_OLYMPIC_DICEPLAY_RESULT_REQ );
    DECL_ON_FUNC( EVENT_OLYMPIC_DICEPLAY_RESULT_ACK );
    DECL_ON_FUNC( EVENT_OLYMPIC_TREEDROP_RESULT_REQ );
    DECL_ON_FUNC( EVENT_OLYMPIC_TREEDROP_RESULT_ACK );
    DECL_ON_FUNC( EVENT_OLYMPIC_BALLOON_RESULT_REQ );
    DECL_ON_FUNC( EVENT_OLYMPIC_BALLOON_RESULT_ACK );
    DECL_ON_FUNC( EVENT_OLYMPIC_REWARD_REQ );
    DECL_ON_FUNC( EVENT_OLYMPIC_REWARD_ACK );
    DECL_ON_FUNC( EVENT_OLYMPIC_BOSSGATE_RESULT_NOT );
    DECL_ON_FUNC( EVENT_CHAR_LEVEL_REWARD_EVENT_NOT );
    DECL_ON_FUNC( EVENT_GP_ATTRIBUTE_RANDOM_REQ );
    DECL_ON_FUNC( EVENT_GP_ATTRIBUTE_RANDOM_ACK );
    DECL_ON_FUNC( EVENT_INIT_ITEM_GP_ATTRIBUTE_REQ );
    DECL_ON_FUNC( EVENT_INIT_ITEM_GP_ATTRIBUTE_ACK );

   _DECL_ON_FUNC( EVENT_ITEM_COMPOSE_REQ, std::vector<GCITEMUID> );
    DECL_ON_FUNC( EVENT_ITEM_COMPOSE_ACK );

   _DECL_ON_FUNC( EVENT_SKILL_OPENKEY_USE_REQ, int );
    DECL_ON_FUNC( EVENT_SKILL_OPENKEY_USE_ACK );
   _DECL_ON_FUNC( EVENT_GACHA_LOTTERY_ACTION_REQ, char );
    DECL_ON_FUNC( EVENT_GACHA_LOTTERY_ACTION_ACK );
   _DECL_ON_FUNC( EVENT_LEVEL_RESET_EVENT_INFO_ACK, KEVENT_LEVEL_RESET_EVENT_INFO_ACK );
   _DECL_ON_FUNC( EVENT_LEVEL_RESET_EVENT_ITEM_REQ, KEVENT_LEVEL_RESET_EVENT_ITEM_REQ );
   _DECL_ON_FUNC( EVENT_LEVEL_RESET_EVENT_ITEM_ACK, KEVENT_LEVEL_RESET_EVENT_ITEM_ACK );
   _DECL_ON_FUNC( EVENT_LEVEL_RESET_EVENT_REWARD_ACK, KEVENT_LEVEL_RESET_EVENT_REWARD_ACK );
	
    DECL_ON_FUNC_NOPARAM( EVENT_SPECIFIC_ITEM_BREAKUP_INFO_REQ );
    DECL_ON_FUNC_NOPARAM( EVENT_MATCH_INVITE_FRIEND_LIST_REQ );
    DECL_ON_FUNC( EVENT_PARTY_USER_READY_REQ );
   _DECL_ON_FUNC( EVENT_PARTY_USER_READY_BROAD, KPartyData );
    DECL_ON_FUNC( EVENT_ADD_MATCH_REQ );
    DECL_ON_FUNC( EVENT_ADD_MATCH_ACK );
    DECL_ON_FUNC( EVENT_ADD_MATCH_BROAD );
    DECL_ON_FUNC_NOPARAM( EVENT_DEL_MATCH_REQ );
   _DECL_ON_FUNC( EVENT_DEL_MATCH_BROAD, int );
    DECL_ON_FUNC_NOPARAM( EVENT_CURRENT_MATCH_MODE_COUNT_REQ );
    DECL_ON_FUNC( EVENT_CURRENT_MATCH_MODE_COUNT_BROAD );
   _DECL_ON_FUNC( EVENT_DISCONNECT_MATCHSERVER_NOT, int );
    DECL_ON_FUNC( EVENT_FIND_MATCH_NOT );
    DECL_ON_FUNC( EVENT_ACCEPT_MATCH_REQ );
    DECL_ON_FUNC( EVENT_ACCEPT_MATCH_BROAD );
    DECL_ON_FUNC_NOPARAM( EVENT_START_GAME_POSSIBLE_REQ );
   _DECL_ON_FUNC( EVENT_START_GAME_FAIL_NOT, DWORD );
    DECL_ON_FUNC_NOPARAM( EVENT_MATCH_MODE_STATE_REQ );

    // 영던템 업글 각종 요청 추가!!!
    DECL_ON_FUNC_NOPARAM( EVENT_HERO_ITEM_UPGRADE_INFO_REQ );
    DECL_ON_FUNC( EVENT_HERO_ITEM_UPGRADE_REQ );
    DECL_ON_FUNC( EVENT_HERO_ITEM_UPGRADE_ACK );

    DECL_ON_FUNC_NOPARAM( EVENT_RITAS_CHRISTMAS_USER_INFO_REQ );
    DECL_ON_FUNC( EVENT_RITAS_CHRISTMAS_USER_INFO_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_RITAS_CHRISTMAS_PLAY_INFO_REQ );
    DECL_ON_FUNC_NOPARAM( EVENT_RITAS_CHRISTMAS_RANK_INFO_REQ );
    DECL_ON_FUNC( EVENT_RITAS_CHRISTMAS_RANK_INFO_ACK );
    DECL_ON_FUNC( EVENT_RITAS_CHRISTMAS_STAGE_START_REQ );
    DECL_ON_FUNC( EVENT_RITAS_CHRISTMAS_EMPTY_BASKET_REQ );
    DECL_ON_FUNC( EVENT_RITAS_CHRISTMAS_STAGE_END_REQ );
    DECL_ON_FUNC( EVENT_RITAS_CHRISTMAS_GET_REWARD_REQ );
    DECL_ON_FUNC( EVENT_RITAS_CHRISTMAS_GET_REWARD_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_RITAS_CHRISTMAS_EXCHANGE_CONTINUE_COIN_REQ );
    DECL_ON_FUNC( EVENT_RITAS_CHRISTMAS_EXCHANGE_CONTINUE_COIN_ACK );
   _DECL_ON_FUNC( EVENT_RITAS_CHRISTMAS_RANK_UPDATE_TIME_CHANGED_NOT, bool );
    DECL_ON_FUNC_NOPARAM( EVENT_RITAS_CHRISTMAS_REWARD_VIEW_INFO_REQ );
    DECL_ON_FUNC( EVENT_CYOU_CASH_CHANGE_REQ );
   _DECL_ON_FUNC( EVENT_CYOU_CASH_CHANGE_ACK, KCYOUCashPoint );

    DECL_ON_FUNC( EVENT_MATCH_RANK_PAGE_REQ );
   _DECL_ON_FUNC( EVENT_MY_MATCH_RANK_INFO_NOT, KMyMatchRankInfo );
    DECL_ON_FUNC( EVENT_MATCH_RANK_SEARCH_REQ );
    DECL_ON_FUNC( EVENT_MATCH_RANK_SEARCH_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_MY_MATCH_RANK_INFO_REQ );

   _DECL_ON_FUNC( EVENT_UPDATE_ITEM_COUNT_NOT, std::vector< KItem > );

    DECL_ON_FUNC( EVENT_STAT_CLIENT_INFO );   

    // === 발렌타인데이에 사용 되는 이벤트 ===
    // 버프 기부
    DECL_ON_FUNC( EVENT_BUFF_DONATION_LOGIN_INFO_NOT);
    DECL_ON_FUNC( EVENT_BUFF_DONATION_TOTAL_DATA_NOT);
    DECL_ON_FUNC( EVENT_BUFF_DONATION_REQ );
    DECL_ON_FUNC( EVENT_BUFF_DONATION_ACK );
    // 누적 출석
   _DECL_ON_FUNC( EVENT_ACCUMULATE_ATTENDANCE_REQ, DWORD );
    DECL_ON_FUNC( EVENT_ACCUMULATE_ATTENDANCE_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_ACCUMULATE_ATTENDANCE_INFO_REQ );
   _DECL_ON_FUNC( EVENT_ACCUMULATE_ATTENDANCE_REWARD_REQ, int );
    DECL_ON_FUNC( EVENT_ACCUMULATE_ATTENDANCE_REWARD_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_CHAR_SELECT_JOIN_REQ );
    // === 발렌타인 이벤트 끝 ===

    DECL_ON_FUNC_NOPARAM( EVENT_CASHBACK_EXTRA_RATIO_INFO_REQ );
    DECL_ON_FUNC( EVENT_CASHBACK_EXTRA_RATIO_INFO_ACK );

   _DECL_ON_FUNC( DB_EVENT_ERRAND_USER_INFO_REQ, DWORD );
   _DECL_ON_FUNC( EVENT_ERRAND_USER_INFO_ACK, KErrandUserInfoDataStruct );
    DECL_ON_FUNC_NOPARAM( EVENT_ERRAND_INIT_INFO_REQ );
   _DECL_ON_FUNC( EVENT_ERRAND_REWARD_REQ, int );
    DECL_ON_FUNC( EVENT_ERRAND_REWARD_ACK );
   _DECL_ON_FUNC( EVENT_ERRAND_GAME_END_REQ, int );
    DECL_ON_FUNC_NOPARAM( EVENT_ERRAND_USER_INFO_REQ );
   
   _DECL_ON_FUNC( EVENT_UPDATEPLAN_GET_GACHA_USER_DB_DATA_NOT, DWORD );
    DECL_ON_FUNC_NOPARAM( EVENT_GACHA_DATA_CHANGE_NOT );    

    DECL_ON_FUNC( EVENT_SYSTEM_GUIDE_COMPLETE_INFO_NOT );
    DECL_ON_FUNC( EVENT_SYSTEM_GUIDE_ITEM_REQ );
    DECL_ON_FUNC( EVENT_SYSTEM_GUIDE_ITEM_ACK );
   _DECL_ON_FUNC( EVENT_SYSTEM_GUIDE_COMPLETE_REQ, int );
    DECL_ON_FUNC( EVENT_SYSTEM_GUIDE_COMPLETE_ACK );

   _DECL_ON_FUNC( EVENT_UPDATEPLAN_BOARD_GAME_USER_INFO_NOT, DWORD );
   _DECL_ON_FUNC( EVENT_UPDATEPLAN_HERO_DUNGEON_INFO_NOT, DWORD );

    DECL_ON_FUNC( EVENT_LOOK_INVENTORY_EXTEND_REQ );
    DECL_ON_FUNC( EVENT_LOOK_INVENTORY_EXTEND_ACK );

    DECL_ON_FUNC( EVENT_DUNGEON_SUBJECT_REWARD_REQ );
    DECL_ON_FUNC( EVENT_DUNGEON_SUBJECT_REWARD_ACK );

    DECL_ON_FUNC( EVENT_GRADUATE_CHARACTER_USER_INFO_NOT );
    DECL_ON_FUNC_NOPARAM( EVENT_NEW_USER_TUTORIAL_CLEAR_NOT );
   _DECL_ON_FUNC( EVENT_ATTRIBUTE_MIGRATION_INFO_REQ, GCITEMUID );
    DECL_ON_FUNC( EVENT_ATTRIBUTE_MIGRATION_SELECT_REQ );
    DECL_ON_FUNC( EVENT_ATTRIBUTE_MIGRATION_SELECT_ACK );
    DECL_ON_FUNC( EVENT_PACKAGE_INFO_DETAIL_REQ );
    DECL_ON_FUNC( EVENT_USER_ATTRIBUTE_MIGRATION_INFO_ACK );
    DECL_ON_FUNC( EVENT_JOIN_ROOM_INFO_DIVIDE_REQ );

    DECL_ON_FUNC_NOPARAM( EVENT_CYOU_GET_WEB_POINT_REQ );
   _DECL_ON_FUNC( EVENT_CYOU_GET_WEB_POINT_ACK, KCYOUCashPoint );
   _DECL_ON_FUNC( EVENT_CYOU_LOGIN_ACK, int );
   _DECL_ON_FUNC( EVENT_CYOU_USER_HEART_BEAT_ACK, int );

   _DECL_ON_FUNC( EVENT_ADD_SLOT_REQ, GCITEMID );
    DECL_ON_FUNC( EVENT_ADD_SLOT_ACK );
   _DECL_ON_FUNC( EVENT_VERIFY_INVENTORY_NOT, KEVENT_QUERY_INVENTORY_INFO_ACK );
    DECL_ON_FUNC( EVENT_CHANGE_CHARACTER_INFO_REQ );
    DECL_ON_FUNC( EVENT_CHANGE_CHARACTER_INFO_ACK );
    DECL_ON_FUNC( EVENT_VITALITY_INFO_NOT );
   _DECL_ON_FUNC( EVENT_VITALITY_RECHARGE_REQ, int );
   _DECL_ON_FUNC( DB_EVENT_VITALITY_INFO_REQ, bool );
   _DECL_ON_FUNC( EVENT_CYOU_LOGIN_REQ, PAIR_CHAR_CHAR );
    DECL_ON_FUNC( EVENT_USER_CHAT_CHANGE_BY_ADMIN_REQ );

    DECL_ON_FUNC( EVENT_CONNECTION_GIFTBOX_FIRST_CHARACTER_GET_GIFT_NOT );
    DECL_ON_FUNC_NOPARAM( EVENT_CONNECTION_GIFTBOX_NEWUSER_GET_GIFT_REQ );
    DECL_ON_FUNC( EVENT_CONNECTION_GIFTBOX_NEWUSER_GET_GIFT_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_CONNECTION_GIFTBOX_EVENTUSER_GET_GIFT_REQ );
    DECL_ON_FUNC( EVENT_CONNECTION_GIFTBOX_EVENTUSER_GET_GIFT_ACK );

   _DECL_ON_FUNC( EVENT_NICKNAME_VALIDITY_CHECK_REQ, std::wstring );
    DECL_ON_FUNC( EVENT_NICKNAME_VALIDITY_CHECK_ACK );

    DECL_ON_FUNC( EVENT_JUMPING_CHAR_INFO_NOT );
    DECL_ON_FUNC_NOPARAM( EVENT_JUMPING_CHAR_REWARD_REQ );
    DECL_ON_FUNC( EVENT_JUMPING_CHAR_REWARD_ACK );
   _DECL_ON_FUNC( EVENT_JUMPING_CHAR_REQ, char );
    DECL_ON_FUNC( EVENT_JUMPING_CHAR_ACK );
    DECL_ON_FUNC( EVENT_USER_RELAY_SEND_COUNT_REQ );
    DECL_ON_FUNC( EVENT_COORDI_COMPOSE_REQ );
    DECL_ON_FUNC( EVENT_COORDI_COMPOSE_ACK );

    DECL_ON_FUNC( EVENT_VIRTUAL_DEPOT_ITEM_LIST_NOT );
    DECL_ON_FUNC( EVENT_MOVE_ITEM_TO_INVENTORY_FROM_VIRTUAL_DEPOT_REQ );
    DECL_ON_FUNC( EVENT_MOVE_ITEM_TO_INVENTORY_FROM_VIRTUAL_DEPOT_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_RELAY_SERVER_STATUS_REQ );

    DECL_ON_FUNC( EVENT_TONG_DONATION_AMOUNT_INFO_ACK );
    DECL_ON_FUNC( EVENT_TONG_DONATION_INFO_REQ );
    DECL_ON_FUNC( EVENT_TONG_DONATION_DONATE_REQ );
    DECL_ON_FUNC( EVENT_TONG_DONATION_DONATE_ACK );

    DECL_ON_FUNC_NOPARAM( EVENT_SOSCIAL_COMMERCE_INFO_REQ );
    DECL_ON_FUNC( EVENT_SOSCIAL_COMMERCE_INFO_UPDATE_NOT );
    DECL_ON_FUNC( EVENT_COORDI_GRADE_UPGRADE_REQ );
    DECL_ON_FUNC( EVENT_COORDI_GRADE_UPGRADE_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_COORDI_GRADE_UPGRADE_INFO_REQ );

    DECL_ON_FUNC( EVENT_SKILL_SCROOL_TRAINING_REQ );

    DECL_ON_FUNC( EVENT_INFINITY_DUNGEON_MONSTER_SUMMON_REQ );
    DECL_ON_FUNC( EVENT_INFINITY_DUNGEON_REWARD_EXP_REQ );
    DECL_ON_FUNC( EVENT_INFINITY_DUNGEON_REWARD_ITEM_REQ );
    DECL_ON_FUNC( EVENT_INFINITY_DUNGEON_REWARD_ITEM_ACK );
    DECL_ON_FUNC( EVENT_INFINITY_DUNGEON_REWARD_GP_NOT );

    DECL_ON_FUNC( EVENT_DUNGEON_PERSONAL_RECORD_INFO_NOT );
    DECL_ON_FUNC( EVENT_DUNGEON_RANK_CURRENT_SEASON_RANK_PAGE_REQ );
    DECL_ON_FUNC( EVENT_DUNGEON_RANK_CURRENT_SEASON_USER_RANK_REQ );
    DECL_ON_FUNC( EVENT_DUNGEON_RANK_PREVIOUS_SEASON_RANK_PAGE_REQ );
    DECL_ON_FUNC( EVENT_DUNGEON_RANK_PREVIOUS_SEASON_USER_RANK_REQ );
    DECL_ON_FUNC( DB_EVENT_DUNGEON_RANK_PREVIOUS_SEASON_USER_RANK_ACK );
    DECL_ON_FUNC_NOPARAM( EVENT_DUNGEON_RANK_PREVIOUS_SEASON_USER_RANK_UPDATED_NOT );

    DECL_ON_FUNC( EVENT_SINGLE_RANDOM_ATTRIBUTE_RANDOM_REQ );
    DECL_ON_FUNC( EVENT_SINGLE_RANDOM_ATTRIBUTE_RANDOM_ACK );

    _DECL_ON_FUNC(EVENT_GACHA_PONG_PLAY_INFO_REQ, DWORD);
    DECL_ON_FUNC_NOPARAM(EVENT_GACHA_PONG_PLAY_INFO_UNSUBSCRIBE_REQ);
    _DECL_ON_FUNC(EVENT_GACHA_PONG_ACTION_REQ, PAIR_DWORD);
    DECL_ON_FUNC(EVENT_GACHA_PONG_PLAY_INFO_NOT);
    DECL_ON_FUNC(EVENT_GACHA_PONG_ACTION_ACK);

    DECL_ON_FUNC(EVENT_PVP_ROOM_LIST_SEARCH_REQ);
    _DECL_ON_FUNC(EVENT_CHANGE_PVP_ROOM_EXTRA_OPTION_REQ, KRoomOptions);

    DECL_ON_FUNC(EVENT_IN_ROOM_CHANGE_CHARACTER_INFO_REQ);
    _DECL_ON_FUNC(EVENT_IN_ROOM_CHANGE_CHARACTER_INFO_ACK, KEVENT_CHANGE_CHARACTER_INFO_ACK);
    _DECL_ON_FUNC(EVENT_IN_ROOM_CHANGE_INDOOR_USERINFO_REQ, KInDoorUserInfo);
    DECL_ON_FUNC(EVENT_COORDIVIEWER_GETSLOT_ACK);
    DECL_ON_FUNC(EVENT_COORDIVIEWER_SETSLOT_ACK);
    DECL_ON_FUNC(EVENT_COORDIVIEWER_GETSLOT_REQ);
    DECL_ON_FUNC(EVENT_COORDIVIEWER_SETSLOT_REQ);
    _DECL_ON_FUNC(EVENT_IN_ROOM_CHANGE_HOST_REQ, DWORD);
    DECL_ON_FUNC(EVENT_PET_MAGIC_STONE_CHANGE_EQUIP_REQ);
    DECL_ON_FUNC(EVENT_PET_MAGIC_STONE_CHANGE_EQUIP_ACK);

    DECL_ON_FUNC(EVENT_BILLBOARD_REGISTRATION_REQ);
    DECL_ON_FUNC(EVENT_BILLBOARD_CHAT_MSG_NOT);

    DECL_ON_FUNC(EVENT_USE_HERO_TICKET_REQ);
    DECL_ON_FUNC(EVENT_USE_HERO_TICKET_ACK);


    // #END_OF_DECL
    // Add Case to file:.\User.cpp#USER_EVENT_CASES

    friend class KGuild;
    friend class KGuildManager;
    friend class KRoom;     // 050514. florist. 방에서 방장의 grade와 guild mark를 필요로 한다.
    friend class KChannel;  // 050609. florist. 채널에서 유저의 길드 가입 여부를 알아야 한다.
    friend class KMissionManager;
    friend class KResultManager;
    friend class KSquare;
    friend class KCollectionManager;
};
