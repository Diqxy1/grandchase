#pragma once
#include "SimLayer.h"
#include "Kairo.h"
#include "ServerType.h" // 여기에 인클루드 안해도 되는데...
#include <boost/tuple/tuple.hpp> // 젠 ... 빌드 시간 또 느려 질까나??
#include "KncTimer.h"

class KGSSimLayer : public KSimLayer
{
    DeclareSingleton( KGSSimLayer );
    NiDeclareRTTI;
    DeclToStringW;

public:

    KGSSimLayer(void);
    virtual ~KGSSimLayer(void);

    enum ENUM_TICK_COUNT
    {
        TICK_LOUD_MSG,
        TICK_CALENDAR_REWARD,
        TICK_SYNC_TIME,
        TICK_CHANNEL_OUT,
        TICK_LOADING_TIME,
        TICK_CHAT_EVENT,
        TICK_TIME_EVENT,
        TICK_BILLING_ERROR,
        TICK_INDIGO_RANK,
        TICK_P2P_STAT,
        TICK_PERIOD_NOTICE,
        TICK_CHAT_LOG,
        TICK_SANTA,
        TICK_VIRTUAL_CASH_RESTRICT_ITEM,
        TICK_DB_QUEUE_SIZE,
        TICK_SQUARE_COUNT,
        TICK_COUNT_NUM,
    };

    // derived from KSimLayer
    virtual void Init();
    virtual void Tick();
    virtual void ShutDown();
    virtual void RegToLua( std::shared_ptr<lua_State> L );

    bool DestroyAllUsers();

    void AddNickToLogin( std::wstring& strLogin, std::wstring& strNick );   //050728. microcat.
    std::wstring TranslateNickToLogin( std::wstring& strNick );             //050728. microcat.
    void RemoveNickToLogin( std::wstring& strNick );                        //050728. microcat.
    void SetHBGap( DWORD dwHBGap );
    
    void SetKairo( IN int iItemID, IN const Kairo& kKairo );
    void ClearKairo() { m_mapKairo.clear(); }
    bool CheckKairo( IN GCITEMID dwItemID, OUT std::vector<KDropItemInfo>& vecItem, OUT DWORD& dwKairoType ); // 보상할 아이템이 있을 때 true.
    bool CheckOneShotKairo( IN GCITEMID dwItemID_, IN int iCount, OUT std::vector<KDropItemInfo>& vecItem_, OUT DWORD& dwKairoType_ ); // 한방에 까기
    void PrintKairo();

    // 070314. kkurrung. 신규 유저 강제로 특정 모드 플레이 하도록 하는 것.
    bool IsEnableForceNewUser(){ KLocker lock(m_csForceNewUser); return m_bEnableForce; };
    void GetForceMode( OUT std::set<int>& setForceModeID ){ KLocker lock(m_csForceNewUser); setForceModeID = m_setForceModeID; };
    bool IsOldUser( IN time_t tmFirstLoginTime );
    void SetForceNewUser( bool bEnable, int nYear, int nMonth, int nDay ); // for Lua
    void AddForceMode( int nModeID ); // for Lua
    void ClearForceMode();// for Lua

    // 070321. kkurrung. Clinet Protected Table Checksum.
    void SetTableChecksum( IN DWORD dwProtectedTableChecksum ){ m_dwTableChecksum = dwProtectedTableChecksum; };
    DWORD GetTableChecksum(){ return m_dwTableChecksum; };
    bool CheckTableChecksum( DWORD dwTableChecksum );

    // 070429. kkurrung. 상하이 드랍 적용 대상 가입 일자.
    void SetSHAdhustTime( int nYear, int nMonth, int nDay, int nHour, int nMinute );
    CTime GetSHAdhustTime();

    // 070614. woosh.
    void UpdateLuckyDays( std::vector<KSimpleDate>& vecLuckyDays );
    bool FindLuckyDay( KSimpleDate& kSimpleDate );

    // 로딩 통계
    void AddLogingCount( DWORD dwLoadingSec );

    // 채팅 이벤트 관련
    void SetChatEvent( IN KChatEventInfo& mapChatEvent );
    bool GetChatEvent( OUT KChatEventInfo& mapChatEvent );
    void ClearChatEvent();
    DWORD GetChatEventGap() { KLocker lock( m_csChatEvent ); return m_dwChatEventGap; }
    DWORD GetChatEventAllowedTime() { KLocker lock( m_csChatEvent ); return m_dwAllowedTime; }
    void SetChatEventAllowedTime( DWORD dwAllowedTime) { KLocker lock( m_csChatEvent ); m_dwAllowedTime = dwAllowedTime; }
    int GetChatEventPvPUserNum() { KLocker lock( m_csChatEvent ); return m_nChatEventPvPUserNum; }
    int GetChatEventDungeonUserNum() { KLocker lock( m_csChatEvent ); return m_nChatEventDungeonUserNum; }
    void SetChatEventOption( DWORD dwAllowedTime, int nMaxUserPvPNum, int nMaxUserDungeonNum );
    void GetPVPEvent( IN const int& nEventNumber_, IN const int& nNumKills ); //  080603  woosh. 대전이벤트 하드코딩

    DWORD GetUniqueNumber() { KLocker lock( m_csUniqueNumber ); return ++m_dwUniqueNumber; }

    void AddP2PStat( int nPing, int nLossCount );
    void WriteP2PLog();
    void SetP2PStat( bool bEnable,int nMinPing, int nMaxPing, int nMaxLossCount );
    bool IsEnableP2PStat();

    bool GetLoadingCheck() {return m_bLoadingCheck;};

    bool GetCheckInEnable() { return m_bPacketCheckInEnable; };

    // 081127.tslayer.주간/월간 출석보상
    void UpdateAttendanceReward( IN std::vector<KAttendanceReward>& vecRewardItemList );
    void ReloadAttendanceRewardToDB(); //콘솔에서 바로 DB정보 갱신 요청할때.
    bool GetAttendanceRewardList( IN std::map< int, std::vector<KItem> >& mapReward_, IN const int nRewardType_, OUT std::vector<KItem>& vecRewardList_ );
    bool GetAttdanceRandomRewardItem( IN std::map< int, std::vector<KItem> >& mapReward_, OUT KItem& kReqItem_ );
    bool GetAttendanceSelectRewardItem( IN std::map< int, std::vector<KItem> >& mapReward_, IN GCITEMID itemID_, OUT KItem& kReqItem_ );
    //주개근 보상
    bool GetWeeklySelectRewardList( OUT std::vector<KItem>& vecWeeklySelectRewardList ); //선택 보상
    bool GetWeeklySelectRewardItem( IN GCITEMID itemID, OUT KItem& kReqItem );
    bool GetWeeklyBaseRewardItem( OUT std::vector<KItem>& vecWeeklyBaseReward ); //기본 보상
    bool GetWeeklyRandomRewardItem( OUT KItem& kReqItem ); //랜덤 보상
    void DumpWeeklyReward();
    //월개근 보상
    bool GetMonthlySelectRewardList( OUT std::vector<KItem>& vecMonthlySelectRewardList ); //선택 보상
    bool GetMonthlySelectRewardItem( IN GCITEMID itemID, OUT KItem& kReqItem );
    bool GetMonthlyBaseRewardItem( OUT std::vector<KItem>& vecMonthlyBaseReward ); //기본 보상
    bool GetMonthlyRandomRewardItem( OUT KItem& kReqItem ); //랜덤 보상
    void DumpMonthlyReward();

    void ClearChatRecordUserUID();
    void AddChatRecordUserUID( IN DWORD dwUID );
    bool IsChatRecordUserUID( IN DWORD dwUID );

    bool IsPossibleBuyPackage( IN bool bPackage );

    //  090409  woosh. 홍콩 친구함께 이벤트
    bool IsNewUserAfterFriendEvent( IN CTime tmFirstLoginTime_ );
    void SetNewUserFriendEvent( bool bEnable_, int nYear_, int nMonth_, int nDay_ );
    void GetUserUIDs( OUT std::vector<DWORD>& vecUIDs );

    void Enable_LFH();

    void SetPingConfig( KPingConfig& kPingConfig_ );
    DWORD GetPingLimit();
    DWORD GetPingGap();
    DWORD GetPingLimitGap();
    bool IsPingEnable();

    void SetPacketHackCheck( IN bool bEnable )  { KLocker lock(m_csPacketHackCheck); m_bPacketHackCheck = bEnable; }
    bool GetPacketHackCheck()                   { KLocker lock(m_csPacketHackCheck); return m_bPacketHackCheck; }

    void SetLongProcessTime( DWORD LongProcessTime_ ) { KLocker lock(m_csLongProcessTime); m_dwLongProcessTime = LongProcessTime_; }
    DWORD GetLongProcessTime() { KLocker lock(m_csLongProcessTime); return m_dwLongProcessTime; }

    bool IsHostMigrationByPing() { KLocker lock(m_csHostMigrationByPing); return m_bHostMigrationByPing; }
    void SetHostMigrationByPing( bool bEnable ) { KLocker lock(m_csHostMigrationByPing); m_bHostMigrationByPing = bEnable; }

    bool IsEnableDeathMatchQuickJoin() { KLocker lock(m_csDeathMatchQuickJoin); return m_bDeathMatchQuickJoin; }
    void SetDeathMatchQuickJoin( bool bEnable ) { KLocker lock( m_csDeathMatchQuickJoin ); m_bDeathMatchQuickJoin = bEnable; }

    void SetExpDBUpdateTick( IN const DWORD dwTick ) { KLocker lock( m_csExpDBUpdateTick ); m_dwExpDBUpdateTick = dwTick; }
    DWORD GetExpDBUpdateTick() { KLocker lock( m_csExpDBUpdateTick ); return m_dwExpDBUpdateTick; }

    void SetDBUpdateUserDataTick( IN const DWORD dwTick ) { KLocker lock( m_csDBUpdateUserTick ); m_dwDBUpdateUserDataTick = dwTick; }
    DWORD GetDBUpdateUserDataTick() { KLocker lock( m_csDBUpdateUserTick ); return m_dwDBUpdateUserDataTick; }


    bool CheckSamePingGroup( DWORD dwUserPing_, DWORD dwRoomPing_ );
    DWORD MakePingGroup( DWORD dwPing_ );

    void SetCheckUserLoadCompleteInfo( bool bCheckUserStageLoadCompleteInfo_ ) { KLocker lock( m_csStageLoadComplete ); m_bCheckUserStageLoadCompleteInfo = bCheckUserStageLoadCompleteInfo_; }
    bool GetCheckUserLoadCompleteInfo() { return m_bCheckUserStageLoadCompleteInfo; }

    void SetServerRoomInfoTick( IN const DWORD& dwTick ) { KLocker lock( m_csServerRoomInfo ); m_bServerRoomInfoTick = dwTick; }
    DWORD GetServerRoomInfoTick() { KLocker lock( m_csServerRoomInfo ); return m_bServerRoomInfoTick; }
    void SetServerRoomInfoEnable( IN const bool& bEnable ) { KLocker lock( m_csServerRoomInfo ); m_bServerRoomInfoEnable = bEnable; }
    bool GetServerRoomInfoEnable() { KLocker lock( m_csServerRoomInfo ); return m_bServerRoomInfoEnable; }

    void SetCreateUserEnable( IN const bool& bEnable );
    bool CloseAllUser();
    void SetServerListAuthLevel( IN const int& nAuthLevel ) { KLocker lock( m_csServerListAuthLevel ); m_nServerListAuthLevel = nAuthLevel; }
    int GetServerListAuthLevel() { KLocker lock( m_csServerListAuthLevel ); return m_nServerListAuthLevel; }

    void SetAgitEnable( IN const bool& bEnable ) { KLocker lock( m_csAgitEnable ); m_bAgitEnable = bEnable; }
    bool IsAgitEnable() { KLocker lock( m_csAgitEnable ); return m_bAgitEnable; }

    std::wstring MakeSHA1( IN std::wstring& strInput_ );

    void SetDBQAlertTimerInterval( IN const DWORD& dwInterval ) { m_kTimer_DBQ_Alert.SetInterval( dwInterval ); }
    void SetNMCafeEnable( IN const bool& bEnable ) { KLocker lock( m_csNMCafe ); m_bNMCafeEnable = bEnable; }
    bool IsNMCafeEnable() { KLocker lock( m_csNMCafe ); return m_bNMCafeEnable; }
    void SetNMCafeUpdateTickGap( IN const DWORD& dwVal ) { KLocker lock( m_csNMCafe ); m_dwNMCafeUpdateTickGap = dwVal; }
    DWORD GetNMCafeUpdateTickGap() { KLocker lock( m_csNMCafe ); return m_dwNMCafeUpdateTickGap; }
    void InitNMCafe();

    void SetSHACheckEnable( IN const bool& bEnable ) { KLocker lock( m_csSHACheckEnable ); m_bSHACheckEnable = bEnable; }
    bool IsSHACheckEnable() { KLocker lock( m_csSHACheckEnable ); return m_bSHACheckEnable; }
    void SetLvDesignEnable( IN const bool& bEnable ) { KLocker lock( m_csLvDesign ); m_bLvDesign = bEnable; }
    bool IsLvDesignEnable() { KLocker lock( m_csLvDesign ); return m_bLvDesign; }

    void SetUpdateRoomStat( IN const bool& bEnable ) { KLocker lock( m_csRoomStat ); m_bUpdateRoomStatEnable = bEnable; }
    bool IsUpdateRoomStat() { KLocker lock( m_csRoomStat ); return m_bUpdateRoomStatEnable; }
    void SetRoomStatTickGap( IN const DWORD& dwTick ) { KLocker lock( m_csRoomStat ); m_dwRoomStatTickGap = dwTick; }
    DWORD GetRoomStatTickGap() { KLocker lock( m_csRoomStat ); return m_dwRoomStatTickGap; }

    void InitCYOUBilling( std::wstring& strServerName_, DWORD dwUID_ );

    DWORD GetMaxPackageNum(){ KLocker lock( m_csMaxOnePackageNum ); return m_dwPackageNum; }
    void SetMaxPackageNum( IN const DWORD& dwPackageNum ) { KLocker lock( m_csMaxOnePackageNum ); m_dwPackageNum = dwPackageNum; }

    DWORD GetMaxItemNum(){ KLocker lock( m_csMaxOneItemNum ); return m_dwOneItemNum; }
    void SetMaxItemNum( IN const DWORD& dwOneItemNum ) { KLocker lock( m_csMaxOneItemNum ); m_dwOneItemNum = dwOneItemNum; }

    void SetCheckTickCount( IN const bool bCheck );
    bool IsCheckTickCount();

    void DumpInventory( IN const DWORD dwLoginUID_ );
    bool IsMonsterCardMigration(){ KLocker lock( m_csMonsterCardMigration ); return m_bMonsterCardMigraion; }

protected:
    std::wostream& GetMaxTickInfo( std::wostream& stm_ ) const;

public: // for lua..
    unsigned int        m_uiNickNameAvailable;      // 050907. microcat. 닉네임 최대허용치 설정
    unsigned int        m_uiPetNickNameAvailable;   // 2007/1/15. iridology. 펫 닉네임 최대 허용치
    DWORD               m_dwShutdownTimeLimit;      // 060113. florist. 서버 종료시 DB처리를 위한 대기시간.
    int                 m_iMaxGamePoint;            // 060120. florist. gp 최대치.
    DWORD               m_dwGameStartWaitTime;      // 060724. kkurrung. Start Game후 이 시간동안 유저가 답이 오지 않으면 강퇴한다.
    bool                m_bRecommendEnable;         // 060822. kkurrung. 추천인 시스템 사용할지 결정
    DWORD               m_dwIndigoLevel;            // 060913. kkurrung. 대만, 홍콩, 일본 에서 용사의 섬 입장 레벨 변경 요청
    bool                m_bCheckIndigoTicket;       // 061106. kkurrung. 용사의 섬 입장권 체크 유무( 대만 긴급 수정 )
    bool                m_bWriteClientInfo;         // 070315. kkurrung. 클라이언트 사양을 DB에 쓸것인지
    bool                m_bTimeSync;                // 070427. kkurrung. DB와 서버간의 시간 동기화 동작하게 할지 결정.
    bool                m_bGCPointSwitch;           // 070503. woosh.    GCPoint 시스템 사용여부(미적용 국가용).
    int                 m_nMaxReportNum;            // 070806. kkurrung. 최대 신고 횟수..
    bool                m_bTWUserAgreement;         // 080214. tslayer. 사용자 정형화 계약 동의 여부
    bool                m_bEnableIndigoLog;         // 080715. kkurrung. 용섬 전적 로그.
    bool                m_bEnableLimitKeyBuy;
    bool                m_bUSEvent;                 // 080710. kkurrung. 미국 이벤트
    bool                m_bGachaEnable;             // 080819. tslayer. 가챠 사용여부
    bool                m_bPackageBuyVirtualCash;   // 090319  woosh. 가상 캐쉬 패키지 구매 가능 여부
    int                 m_nMaxVirtualCash;          // 090518  woosh. 가상 캐쉬 최대값
    bool                m_bEnableGameCoupon;        // 090320. tslayer. 게임쿠폰 시스템 사용 여부
    bool                m_bVCMPCantBuy;             // 090702  woosh. 가상캐쉬 미션,패키지 구매 불가  Virtual Cash Mission and Package Item Can't buy
    DWORD               m_dwStageLoadLimitTime;     // 101108  woosh. 스테이지 로딩 제한 시간
    bool                m_bBlockMaxUser;            // 110517  woosh. 최대 동접 초과접속 제한
    int                 m_nLimitAcceptQueueCount;   // 110522  woosh. 디비 큐 쌓였을 때 접속 제한
    bool                m_bLimitAcceptEnable;       // 110613  tgkwon 디비 큐 쌓였을 때 접속 제한 사용여부.

    bool                m_bUseCompetitionTicket;    // 20110519 woosh. 대회서버 입장티켓, 인도네시아
    bool                m_bUserCreateRoomCompeteServer; // 2011-6-24 woosh. 브라질 대회서버에서 일반유저 방 생성 가능.
    bool                m_bInitTimeSync;            // 070427. kkurrung. DB와 서버간의 시간 동기화 동작하게 할지 결정.

    bool                m_bWaitInvenMigration;      // 131022. nodefeat. 캐릭터 분리 DB마이그레이션 이전에 서버 실행으로 대기인벤 아이템 강제로 인벤으로 밀어 넣을지 여부

protected:
    unsigned int                             m_auiTickCount[TICK_COUNT_NUM];
    unsigned int GetTick( int nIndex )      { return m_auiTickCount[ nIndex ]; }
    void SetTick( int nIndex )              { m_auiTickCount[ nIndex ] = ::GetTickCount(); }
    std::map<std::wstring, std::wstring>     m_smapNickToLogin;  //050728. microcat.
    std::map<int,Kairo>                      m_mapKairo;         //061113. florist.

    // 070314. kkurrung. 신규 유저 강제로 특정 모드 플레이 하도록 하는 것.
    mutable KncCriticalSection                          m_csForceNewUser;   // 아래의 값 모두 보호
    bool                                                m_bEnableForce;     // 활성화?
    CTime                                               m_tmForceTime;      // 활성화 First Login 시간.
    std::set<int>                                       m_setForceModeID;   // 강제로 플레이 해야하는 모드

    // 070321. kkurrung. Clinet Protected Table Checksum.
    DWORD                                               m_dwTableChecksum;

    // 070423. kkurrung. 달개근 했을때 보상해주는 아이템 리스트
    mutable KncCriticalSection      m_csMonthlyReward;   // 월개근 보상 아이템 보호
    std::vector<KItemUnit>          m_vecMonthlyReward;  // 월개근 보상 아이템 

    // 070429. kkurrung. 한국 상하이 드랍 적용 유저의 최초 접속일.
    mutable KncCriticalSection      m_cstmSHAdjust;
    CTime                           m_tmSHAdjust;

    mutable KncCriticalSection      m_csLuckyDays;  // 운 좋은 날 리스트 보호
    std::vector<KSimpleDate>        m_vecLuckyDays; // 070614. woosh. 운 좋은 날 리스트

    // 최대 틱 카운트.
    mutable KncCriticalSection      m_csMaxTick;
    CTime                           m_tmMaxTickTime;
    DWORD                           m_dwMaxTickDuration;
    
    mutable KncCriticalSection      m_csStatLoadingTime;
    std::map<DWORD,DWORD>           m_mapStatLoadingTime;
    
    mutable KncCriticalSection      m_csChatEvent;
    DWORD                           m_dwChatEventGap;
    bool                            m_bEnableChatEvent;
    DWORD                           m_dwAllowedTime;
    int                             m_nChatEventPvPUserNum;
    int                             m_nChatEventDungeonUserNum;
    KChatEventInfo                  m_kChatEvent;

    mutable KncCriticalSection      m_csUniqueNumber;
    DWORD                           m_dwUniqueNumber;       // p2p 처리 관련 , 게임시작 마다 ++

    mutable KncCriticalSection      m_csP2PStat;
    std::map< std::pair<int, int>, DWORD > m_mapStatP2P;
    bool                            m_bEnableP2PStat;
    int                             m_nMinPing;
    int                             m_nMaxPing;
    int                             m_nMaxLossCount;

    bool                            m_bLoadingCheck;

    bool                            m_bPacketCheckInEnable; // 중복 패킷 처리 여부.

    // 081127.tslayer.출석 개근시 보상해주는 아이템 리스트
    mutable KncCriticalSection              m_csAttendanceReward;
    std::map< int, std::vector<KItem> >     m_mapWeeklyReward;  // 월개근 보상 리스트(보상타입,보상아이템)
    std::map< int, std::vector<KItem> >     m_mapMonthlyReward; // 주개근 보상 리스트(보상타입,보상아이템)

    CTime                           m_tmNewUserEventTime;      //  090409  woosh. 홍콩 친구함께, 신규 유저 기준일
    KncTimer                        m_kTimer_DBQ_Alert; // 20110803 woosh. 

    mutable KncCriticalSection      m_csPingConfig;
    KPingConfig                     m_kPingConfig;

    mutable KncCriticalSection      m_csPacketHackCheck;
    bool                            m_bPacketHackCheck; // inner패킷 체크여부

    mutable KncCriticalSection      m_csLongProcessTime;
    DWORD                           m_dwLongProcessTime;

    mutable KncCriticalSection      m_csHostMigrationByPing;
    bool                            m_bHostMigrationByPing; // ping수치 좋은 유저가 방장이 되는 처리여부

    mutable KncCriticalSection      m_csDeathMatchQuickJoin;
    bool                            m_bDeathMatchQuickJoin;

    mutable KncCriticalSection      m_csExpDBUpdateTick;
    DWORD                           m_dwExpDBUpdateTick; // Exp DB 업데이트 주기

    mutable KncCriticalSection      m_csDBUpdateUserTick; // user data update 주기
    DWORD                           m_dwDBUpdateUserDataTick;

    std::vector<DWORD> m_vecPingRange;
    bool m_bCheckJoinRoomPing;

    mutable KncCriticalSection      m_csStageLoadComplete;
    bool m_bCheckUserStageLoadCompleteInfo;

    mutable KncCriticalSection      m_csServerRoomInfo;
    bool                            m_bServerRoomInfoEnable; // 서버-룸정보 기능사용여부
    DWORD                           m_bServerRoomInfoTick; // 서버-룸정보 Tick gap

    mutable KncCriticalSection      m_csServerListAuthLevel;
    int                             m_nServerListAuthLevel;

    mutable KncCriticalSection      m_csAgitEnable;
    bool                            m_bAgitEnable; // 아지트 기능 사용여부

    mutable KncCriticalSection      m_csNMCafe;
    bool                            m_bNMCafeEnable; // 넷마블 PC방차감 사용여부
    DWORD                           m_dwNMCafeUpdateTickGap; // 넷마블PC방 플레이시간 차감주기

    mutable KncCriticalSection      m_csSHACheckEnable;
    bool                            m_bSHACheckEnable; // SHA Check 기능 사용여부.

    mutable KncCriticalSection      m_csLvDesign;
    bool                            m_bLvDesign; // 레벨디자인 서버인가.

    mutable KncCriticalSection      m_csRoomStat;
    bool                            m_bUpdateRoomStatEnable; // Room 통계 기록할지 여부
    DWORD                           m_dwRoomStatTickGap; // Room 통계 기록 주기 (ms)

    mutable KncCriticalSection      m_csMaxOnePackageNum;
    DWORD                           m_dwPackageNum; // 한번에 보낼 패키지 갯수 설정.

    mutable KncCriticalSection      m_csMaxOneItemNum;
    DWORD                           m_dwOneItemNum;

    mutable KncCriticalSection      m_csCheckAuthTickCount;
    bool                            m_bCheckAuthTickCount; // auth tick  체크 온오프

    mutable KncCriticalSection      m_csMonsterCardMigration;
    bool                            m_bMonsterCardMigraion; // 몬스터 카드 Migration 여부.

// end of protected
private:
    mutable KncCriticalSection m_csNewDeleteCount;
    LONG m_NewCount;
    LONG m_DeleteCount;
};

DefSingletonInline( KGSSimLayer );
