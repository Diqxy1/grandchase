#pragma once
#include "UserPacket.h"
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <ToString.h>
#include <RTTI.H>
#include "Lottery.h"

namespace luabind { class object; }

struct KCouponInfo
{
    GCITEMID   m_dwItemID;
    int     m_nRewardGP;
};

struct KExpPotionInfo
{
    GCITEMID   m_dwItemID;
    bool       m_bPercentageGrow;
    float      m_fGrowRate;
    int        m_nGrowValue;
    bool       m_bExceedEXP;
    DWORD      m_dwValidMinLev;
    DWORD      m_dwValidMaxLev;
};

struct KVirtualCashCouponInfo
{
    GCITEMID   m_dwItemID;
    int     m_nRewardCash;
};

class KODBC; //전방 선언.
class KLuaManager;
class KGCHelper
{
    DeclareSingleton( KGCHelper );
    NiDeclareRootRTTI( KGCHelper );
    DeclToStringW;

public:

    // 보너스
    struct sBonusData
    {
        int nBegin;
        int nEnd;
        int nValue;
    };
    // 아이템을 받아오기 위한 구조체.
    typedef std::vector<KDropItemInfo> VEC_REWARD;

public:
    enum REWARD_TYPE {
        RT_ITEM       = 0, // 아이템.
        RT_VIRTUAL    = 1, // 가상캐시.
    };

    enum GCCLUB_TYPE {
        GT_NONE = 0, // 없음
        GT_GCCLUB = 1, // GC클럽
        GT_GCBLOG = 2, // GC블로그
    };

    KGCHelper(void);
    ~KGCHelper(void);
    void Tick();
    // 보너스
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    int GetRefillBonusPoint( int nLevel, int nCurrentBonus );
    int GetMaxBonusCount();
    bool IsBonusItem( GCITEMID dwItemId );

    // 퀵슬롯
    bool IsValidateItem( IN OUT std::vector< KSlotData >& kQuickSlot, IN const std::wstring& strLogin );
    bool IsValidateEmoticon( IN OUT std::vector< KSlotData >& kQuickSlot, IN const std::wstring& strLogin );
    bool EmotconMixResult( OUT GCITEMID& dwResult );
    bool CheckPositionGroupType( IN const int& nPositionType, IN const GCITEMID& ItemID );

    // 용섬 랭크.
    void InitIndigoRank();
    //void UpdateIndigoRank( KODBC& kODBC );
    void GetIndigoRankInfo( KSerBuffer& serBuff );
    void SetIndigoRank( KSerBuffer& serBuffIndigoRank_ );

    // 접속 메세지
    //void GetConnectionMsg( IN KODBC& kODBC, IN std::vector<int>& vecMsgID, OUT std::vector<std::wstring>& vecMessage );
    void ClearConnectionMsg();
    void GetConnectionMessage( std::map<int, std::wstring>& msg_ );
    void SetConnectionMessage( std::map<int, std::wstring>& msg_ );

    // Hell Item 관련
    bool LoadHellItemInfo( KLuaManager& kLuaMng );
    int GetMaximumTicketNum()                   { KLocker lock( m_csHellItemCount ); return m_nMaximumTicketNum; };
    int GetConnectionHellCount()                { KLocker lock( m_csHellItemCount ); return m_nConnectionHellItemCount; };
    int GetAttendanceHellCount()                { KLocker lock( m_csHellItemCount ); return m_nAttendanceHellItemcount; };
    int GetWeekAttendHellCount()                { KLocker lock( m_csHellItemCount ); return m_nWeekAttendHellItemcount; };
    int GetMonthAttendHellCount()               { KLocker lock( m_csHellItemCount ); return m_nMonthAttendHellItemcount; };

    GCITEMID GetHellItemID()                       { KLocker lock( m_csHellItemCount ); return m_dwHellItemID; };
    bool IsEndGameHellTicketDec()               { KLocker lock( m_csHellItemCount ); return m_bEndGameHellTicketDec; }
    bool HellItemCheckEnable( IN int nModeID );
    void SetHellItemFreeMode( IN const std::set<int>& setModeList );
    void GetHellItemFreeMode( OUT std::set<int>& setModeList_ );
    void PrintHellItemInfo();
    void TickHell();
    bool GetDungeonFreePass()                   { KLocker lock( m_csHellItemCount ); return m_bDungeonFreePass; }
    int GetDungeonFreePassID()                 { KLocker lock( m_csHellItemCount ); return m_nDungeonFreePassID; }

    // Help Me.....
    template < typename T > 
    void MakeUserBuff( unsigned short usEventID_, T& data_, KSerBuffer& kbuff_, DWORD dwSize = 0xfffffffe );

    // 인벤토리
    int GetMaxInvenCapacity()                   { KLocker lock(m_csInvenExternItemInfo); return m_nMaxInvenCapacity; }
    bool GetInvenOverflowEnable()               { KLocker lock(m_csInvenExternItemInfo); return m_bEnableInvenOverflow; }
    bool IsInvenExtendItem( IN GCITEMID dwItemID, OUT int& nInvenCapacity );
    void PrintInvenInfo();

    // 쿠폰 관련.
    bool LoadCouponInfo( KLuaManager& kLuaMng );
    bool GetCouponInfo( GCITEMID dwItemID, KCouponInfo& kCouponInfo );
    bool LoadVirtualCashCouponInfo( KLuaManager& kLuaMng );
    bool GetCouponInfo( GCITEMID dwItemID, KVirtualCashCouponInfo& kCouponInfo );


    // 캐릭터 획득 카드 관련.
    bool LoadCharacterCard( KLuaManager& kLuaMng );
    bool GetCharacterCard( IN const GCITEMID itemID, OUT char& cCharType, OUT std::set<DWORD>& setMissionInfo );
    void GetCharacterCardInfo( OUT std::map<GCITEMID,KCharCardInfo>& mapCharacterCard );

    // Vip Item 관련
    bool LoadVipItemInfo( OUT std::string& strScript );
    void GetVipItemInfo( OUT std::map< int, std::map<DWORD,int> >& mapVipItemInfo );
    bool IsCurrentVipItemInfo( IN const int nCharType, IN const GCITEMID dwItemID, IN const int nVipCount );

    // 채팅 기록
    void SetChatRecordEnable( IN bool bEnable_ )    { KLocker lock(m_csChatRecordUserUID); m_bChatRecordEnable = bEnable_; }
    bool GetChatRecordEnable()                      { KLocker lock(m_csChatRecordUserUID); return m_bChatRecordEnable; }
    void SetChatLogQueLimit( IN int nSize_ )        { KLocker lock(m_csChatRecordUserUID); m_nChatLogQueLimit = nSize_; }
    int GetChatLogQueLimit()                        { KLocker lock(m_csChatRecordUserUID); return m_nChatLogQueLimit; }
    bool IsChatRecordUserUID( IN DWORD dwUID );
    void PrintChatRecord();

    // GC클럽 확장
    bool GetGCClubExtensionEnable()  { KLocker lock(m_csGCClub); return m_bGCClubExtensionEnable; }
    void GetGCClubItemID( OUT std::map<GCITEMID,USHORT>& mapItemID )          { KLocker lock(m_csGCClub); mapItemID = m_mapGCClubItemID; }
    void GetGCClubConnectBonusItem( OUT std::vector<KDropItemInfo>& vecItemList );
    void GetGCClubDayAttendBonusItem( OUT std::vector<KDropItemInfo>& vecItemList );

    // 눈꽃 코인 관련
public:
    bool ProcessSnowCoin( IN GCITEMID dwItemID, IN int nCount, IN OUT std::vector<KItem>& vecInven );
    void PrintCoinEvent();
protected:
    bool LoadSnowCoinScript( KLuaManager& kLuaMng );
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool IsSnowCoinEvent();
    bool IsSnowCoin( IN GCITEMID dwItemID );
    void MakeFakeSnowCoin( IN int nCount, OUT KItem& kItem );
    void TickSnowCoin();
    void SnowCoinInit();
    void UpdateState();

protected:
    // 보너스
    bool LoadBonusData( KLuaManager& kLuaMng );
    bool FindBonusData( IN int nKey, OUT sBonusData& bonusData );

    // 퀵슬롯
    bool LoadQuickSlotData( KLuaManager& kLuaMng );
    bool LoadGroupItem( KLuaManager& kLuaMng, OUT std::map< int,std::set<DWORD> >& mapGroupItem );
    bool LoadNonGroupItem( KLuaManager& kLuaMng, OUT std::set<DWORD>& setNonGroup );
    bool LoadEmotionItem( KLuaManager& kLuaMng, OUT std::set<DWORD>& setEmoticon );
    bool LoadEmoticonMixData( KLuaManager& kLuaMng, OUT KLottery& ltrRatio );

    bool LoadInvenExternData( KLuaManager& kLuaMng ); // 가방 확장 아이템
    bool LoadChatRecordUserUID( OUT std::string& strScript ); // 채팅 기록
    bool LoadGCClubExtension( KLuaManager& kLuaMng_ ); // GC클럽 확장

    // 대만 대회참가승리팀 예상 기부
public:
    bool IsTourDonationEvent();                                 // 기부 이벤트 기간인지 체크.
    bool IsDonationItemID( IN GCITEMID dwItemID );                 // 기부 아이템인지 체크.
    void GetBuyCheckItemList( OUT std::set<DWORD>& setBuyCheckItemList ); // 기부 아이템 리스트 가져오기.
protected:
    bool LoadTourDonationScript( KLuaManager& kLuaMng );        // 기부 정보 스크립트 Load

    // 이벤트 내용 출력 여부 확인
public:
    bool CheckEventEnable();
    void GetEventStringList( OUT std::set<DWORD>& setStringList );
protected:
    bool LoadEventInfoScript( IN KLuaManager& kLuaMng );

    // 미국 대회 서버 관련 서버 오픈 시간 체크.
public:
    bool IsTournamentCheck() { KLocker lock(m_csTournament); return m_bTourCheck; }
    GCITEMID GetTournamentTicketID(); // 인도네시아 대회 서버 티켓
protected:
    // 대회서버 시간 세팅.
    bool LoadTournamentData( KLuaManager& kLuaMng ); // 루아 데이터 읽어오기.
    void TickTournament();
    void UpdateTourState();
    void InitTournament();
    bool LoadTimeCheckData( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT bool& bTimeCheck, OUT int& nStartHour, OUT int& nEndHour );
    void UpdateTimeState();

public:
    bool LoadAdditionalMobDropRatio( IN OUT KLuaManager& kLuaMng );
    float GetMobDropRatioByGrade( IN const int nMobGrade );

    // 남미 미국 유저 인증 체크.
protected:
    bool LoadUserAuthData( KLuaManager& kLuaMng  );
    bool LoadRewardList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector<KDropItemInfo>& vecRewardItem );
public:
    bool IsUserAuthEventTerm();
    bool IsUserPurchaseEventTerm();
    void GetUserRewardItem( OUT std::vector<KDropItemInfo>& vecRewardList );
    void GetBeginTime( OUT time_t& tmDate );
    bool IsUserVirtualPurchaseTerm( IN time_t tmDate );

    // 남미 국가별 보상 혜택.
protected:
    bool LoadNationRewardInfo( KLuaManager& kLuaMng );
public:
    bool IsNationRewardCheck();
    void GetNationExpGPRewardRatio( OUT float& fExpRatio, OUT float& fGPRatio );

    // 던전 티켓
protected:
    bool LoadItemNeedCount( KLuaManager& kLuaMng, IN std::string& strTable, IN int nType, OUT std::map<GCITEMID,int>& mapList, IN OUT std::map<int,std::set<GCITEMID> >& mapTicket );
    bool LoadDungeonTicketList( KLuaManager& kLuaMng );
public:
    void GetDungeonTicketList( OUT std::map<int,KDungeonTicket>& mapList );
    bool GetDungeonTicketInfo( IN const int& nModeID, OUT KDungeonTicket& kInfo );
    void GetDungeonTicketTypeList( OUT std::map<int,std::set<GCITEMID> >& mapList );

    // Kario 상자 특정 ItemeID만 공지 출력하게 설정.
protected:
    bool LoadKairoNoticeInfo( KLuaManager& kLuaMng );
    bool LoadSetItemInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::set<DWORD>& setInfo );
public:
    bool CheckKairoBox( IN const GCITEMID& dwItemID );
    bool CheckNoticeItem( IN const GCITEMID& dwKairoBoxItemID, IN const GCITEMID& dwPostItemID );

    // 선물하기 기능 제한 아이템 추가
    bool LoadPresentLimitItem( KLuaManager& kLuaMng );
    bool CheckPresentLimitItem( IN const DWORD& dwItemID );

    // 경험치 포션
protected:
    bool LoadExpPotionInfo( KLuaManager& kLuaMng_ );
public:
    bool GetExpPotionInfo( IN const GCITEMID& dwItemID, OUT KExpPotionInfo& KExpPotionInfo_ );
    bool GetExpPotionInfo( OUT std::vector<GCITEMID>& vecExpPotion_ );

    // 신캐릭터 지급 알림 기능
public:
    bool IsEventNewCharTerm();
    int GetEventNewCharID();

    // 출석 n 회 보상 이벤트(캐릭터 획득 카드)
    bool LoadAttendGetChar( KLuaManager& kLuaMng_ );
    bool IsAttendToGetCharTerm();
    void GetAttendGetCharRewardItem( OUT std::vector<KDropItemInfo>& vecRewardList_ );
    int GetAttendCountGetChar();
    void GetBeginDateGetCharByAttend( OUT time_t& tmDate_ );
    bool IsAttendToGetCharDay( IN KSimpleDate& date_ );

    // 대전 서버에서 캐릭터 제외하기
    bool LoadPVPCharDisable( KLuaManager& kLuaMng_ );
    bool GetPVPCharacterDisableData( OUT std::set<DWORD>& setCharEnable_ );
protected:
    bool LoadNewCharData( KLuaManager& kLuaMng_ );

protected:
    bool LoadCharLevelEvent( KLuaManager& kLuaMng );
    bool LoadGradeReward( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector<KDropItemInfo>& vecRewardItem );

public:
    bool IsCharLevelEvent();
    bool GetCharLevelRewardItem( IN const char& cCharType, IN const int& nLevel, IN const std::set<char>& setCharInfo,OUT std::vector<KDropItemInfo>& vecItemInfo );
    bool ServerConnectCharLevelReward( IN const char& cCharType, IN const int& nLevel, IN const std::set<char>& setCharInfo,OUT std::vector<KDropItemInfo>& vecItemInfo );
    DWORD GetCharLevelEventVersion() { KLocker lock( m_csCharLevelEvent ); return m_dwVersion; }

protected:
    // 보너스
    mutable KncCriticalSection          m_csBonusTable;
    int                                 m_nMaxBonusCount;
    std::vector< sBonusData >           m_vecBonusTable;
    std::set<DWORD>                     m_setBonusItem;

    // 퀵슬롯
    mutable KncCriticalSection          m_csQuickSlot;
    std::map< int,std::set<DWORD> >     m_mapGroupItem;    //<GroupID, Items>
    std::set<DWORD>                     m_setNonGroup;     // Items
    std::set<DWORD>                     m_setEmoticon;     // Emoticon Items..
    KLottery                            m_ltrMixRatio;  // Emoticon Mix Ratio

    mutable KncCriticalSection          m_csIndigoRank;
    KSerBuffer                          m_kserBuffIndigoRank;

    mutable KncCriticalSection          m_csConnectionMsg;
    std::map<int, std::wstring>         m_mapConnectionMsg;

    // 헬모드 입장권 관련 정보
    mutable KncCriticalSection          m_csHellItemCount;

    bool                                m_bHellItemCheckEnable;     // 헬모드 입장권 체크여부
    GCITEMID                               m_dwHellItemID;             // 입장권 ItemID
    int                                 m_nMaximumTicketNum;         // 최대 티켓수(-1:제한없음)
    int                                 m_nConnectionHellItemCount; // 접속시 지급수
    int                                 m_nAttendanceHellItemcount; // 일일 출석시 지급수
    int                                 m_nWeekAttendHellItemcount; // 주개근시 지급수
    int                                 m_nMonthAttendHellItemcount;// 월개근시 지급수
    std::set<int>                       m_setHellItemFreeMode;      // 헬모드 입장권 체크안하는 모드(Mode ID)
    bool                                m_bEndGameHellTicketDec;    // 게임종료후 헬모드 티켓 감소할지 여부
    bool                                m_bCheckSendHellMode;       // 헬모드 정보 전송여부.
    bool                                m_bDungeonFreePass;         // 던전 프리패스 아이템 소유시 챔피언 모드 입장 가능 
    int                                 m_nDungeonFreePassID;       // 던전 프리패스 아이템 아이디

    mutable KncCriticalSection          m_csInvenExternItemInfo;
    int                                 m_nMaxInvenCapacity;        // 인벤 최대 크기.
    bool                                m_bEnableInvenOverflow;     // 인벤토리 초과확장 가능여부.
    std::map< DWORD, int >              m_mapInvenExtendItemInfo;   // 인벤토리 확장 아이템 정보(아이템ID,확장크기)

    mutable KncCriticalSection          m_csCoupon;
    std::map<DWORD,KCouponInfo>         m_mapCoupon;

    mutable KncCriticalSection          m_csCharacterCard;
    std::map<GCITEMID,KCharCardInfo>    m_mapCharacterCard;

    // 눈꽃 코인
    mutable KncCriticalSection          m_csSnowCoin;
    time_t                              m_tmBegin;              // 이벤트 시작 날짜
    time_t                              m_tmEnd;                // 이벤트 끝날짜
    bool                                m_bOnEvent;             // 이벤트 진행중인지 여부 기록
    GCITEMID                            m_dwSnowCoinID;         // 코인 ItemID
    GCITEMUID                           m_dwSnowCoinUID;        // 코인 UID
    DWORD                               m_dwSnowCoinTickGap;    // TickGap
    DWORD                               m_dwSnowCoinLastTick;   // LastTick

    mutable KncCriticalSection              m_csVipItemInfo;
    std::map< int, std::map< DWORD,int > >  m_mapVipItemInfo;   // 미국 VIP아이템 리스트

    // 채팅기록
    mutable KncCriticalSection          m_csChatRecordUserUID;
    bool                                m_bChatRecordEnable;    // 채팅기록여부
    int                                 m_nChatLogQueLimit;     // 채팅기록큐제한
    bool                                m_bChatRecordUserCheck; // 채팅기록 특정유저만 체크할지 여부
    std::set<DWORD>                     m_setChatRecordUserUID; // 채팅 기록남길 유저UID 리스트

    mutable KncCriticalSection          m_csVirtualCashCoupon;
    std::map<DWORD, KVirtualCashCouponInfo> m_mapVirtualCashCoupon; // 가상캐쉬 쿠폰

    // GC클럽 확장기능
    mutable KncCriticalSection          m_csGCClub;
    bool                                m_bGCClubExtensionEnable;   // 확장기능 사용여부
    std::map<GCITEMID,USHORT>           m_mapGCClubItemID;          // GC클럽 ItemID
    std::vector<KDropItemInfo>          m_vecConnectBonusItem;      // 접속시 보너스 지급 아이템 { ItemID, Factor }
    std::vector<KDropItemInfo>          m_vecDayAttendBonusItem;    // 일일 출석시 보너스 지급 아이템 { ItemID, Factor }

    // 대만 대회참가승리팀 예상 기부
    mutable KncCriticalSection          m_csTourDonation;
    time_t                              m_tmTourDonationBegin;  // 이벤트 시작 날짜.
    time_t                              m_tmTourDonationEnd;    // 이벤트 종료 날짜.
    std::set<DWORD>                     m_setBuyCheckItemList;  // 구매제한 체크 ItemList.

    // 이벤트 내용 출력 여부 확인
    mutable KncCriticalSection          m_csEventInfo;
    bool                                m_bCheckEnable;         // 오늘 첫접속인지 확인할지 여부 체크.
    std::set<DWORD>                     m_setStringIDList;      // 금일 전달해야 하는 StringID List.

    // 미국 대회서버 시간 설정 체크.
    mutable KncCriticalSection          m_csTournament;
    time_t                              m_tmTourBegin;              // 미국 Tournament 시작시간.
    time_t                              m_tmTourEnd;                // 미국 Tournament 종료시간.
    bool                                m_bTourCheck;               // 미국 Tournament 중인지를 기록.
    bool                                m_bTourTimeCheck;           // Tournament 시간인지를 기록.
    DWORD                               m_dwTourTickGap;            // TickGap
    DWORD                               m_dwTourLastTick;           // LastTick
    bool                                m_bTimeCheck;               // 시간 체크 변수 사용 여부.
    int                                 m_nStartHour;               // 시작 시간(Hour).
    int                                 m_nEndHour;                 // 종료 시간(Hour).    
    GCITEMID                            m_dwTournamentTicketID;     // 인도네시아 대회서버 티켓 ID

    // 몬스터 등급에 따른 아이템 드랍 확률 수정.
    mutable KncCriticalSection          m_csAdditionalMobDrop;
    std::map<int,float>                 m_mapAdditionalMobDropRatio;

    // 남미 미국 유저 인증 체크.
    mutable KncCriticalSection          m_csUserAuth;
    time_t                              m_tmUserAuthBegin;          // 인증 체크 시작시간.
    time_t                              m_tmUserAuthEnd;            // 인증 체크 종료시간.
    time_t                              m_tmUserPurchaseEnd;         // 보상 체크 종료시간.
    std::vector<KDropItemInfo>          m_vecRewardItem;            // 유저 보상 아이템 리스트.

    // 남미 국가별 보상 혜택.
    mutable KncCriticalSection          m_csNationReward;
    time_t                              m_tmNationBegin;          // 국가별 보상 혜택 시작시간.
    time_t                              m_tmNationEnd;            // 국가별 보상 혜택 종료시간.
    float                               m_fExpRatio;              // 경험치 보상 배율
    float                               m_fGPRatio;               // GP 보상 배율

    // 던전 티켓 정보
    mutable KncCriticalSection          m_csDungeonTicket;
    std::map<int,KDungeonTicket>        m_mapDungeonTicketList;     // map[ModeID, 던전티켓정보]
    std::map<int,std::set<GCITEMID> >   m_mapTicketItem;            // map[티켓타입, set[입장티켓ItemID]

    // Kairo 상자 특정 ItemID 공지 출력 시스템
    mutable KncCriticalSection          m_csKairoNotice;
    std::map<DWORD, std::set<DWORD>>    m_mapKairoNoticeItem;

    // 경험치 포션
    mutable KncCriticalSection          m_csExpPotion;
    std::map<DWORD, KExpPotionInfo>     m_mapExpPotion;
    std::vector<GCITEMID>               m_vecExpPotionItemID; // 클라에 따로 미리 보내 줄 포션 ID리스트

    // 선물하기 기능 제한 아이템 추가
    mutable KncCriticalSection          m_csPresentLimit;
    bool                                m_bLimitUseEnable;
    std::set<GCITEMID>                  m_setPresentLimitItem;

    // 신 캐릭터 지급 이벤트
    mutable KncCriticalSection          m_csNewChar;
    time_t                              m_tmNewCharBegin; // 신캐릭터 지급 시작일
    time_t                              m_tmNewCharEnd; // 신캐릭터 지급 끝
    int                                 m_nNewCharID; // 신캐릭터 번호 ( 엘리시스 0 , ... )
	
    // 출석 n회  캐릭터 획득 아이템 지급 이벤트
    mutable KncCriticalSection          m_csAttendGetChar;
    time_t                              m_tmAttendGetCharBegin;          // 이벤트 시작시간.
    time_t                              m_tmAttendGetCharEnd;            // 종료시간.
    std::vector<KDropItemInfo>          m_vecAttendGetCharRewardItem;            // 유저 보상 아이템 리스트.
    int                                 m_nAttendCount; // n 회 출석 기준	

    // 특정 캐릭터 특정 레벨 아이템 지급 이벤트.
    mutable KncCriticalSection          m_csCharLevelEvent;
    DWORD                               m_dwVersion;
    time_t                              m_tmCharLevelEventBegin;          // 이벤트 시작시간.
    time_t                              m_tmCharLevelEventEnd;            // 종료시간.
    std::map<std::pair<char,int>, std::vector<KDropItemInfo>> m_mapCharRewardItem;       // 캐릭터 타입별 보상 아이템 리스트.

    // 대전 서버에서 캐릭터 제외하기
    mutable KncCriticalSection          m_csPVPCharDisable;
    std::set<DWORD>                     m_setCharDisable;
};
template < typename T > 
void KGCHelper::MakeUserBuff( unsigned short usEventID_, T& data_, KSerBuffer& kbuff_, DWORD dwSize /*= 0xfffffffe*/ )
{
    KEventPtr spEvent( new KUserEvent() );
    spEvent->m_usEventID = usEventID_;

    // make event object
    KSerializer         ks;

    // serialize - only data
    ks.BeginWriting( &spEvent->m_kbuff );
    ks.Put( data_ );
    ks.EndWriting();

    if( dwSize <= spEvent->m_kbuff.GetLength() ) spEvent->m_kbuff.Compress();   // 압축할 경우, KEvent의 buff만을 압축한다.

    // serialize - full event object
    kbuff_.Clear();
    ks.BeginWriting( &kbuff_ );
    ks.Put( *spEvent );
    ks.EndWriting();
}

DefSingletonInline( KGCHelper );
DeclOstmOperatorA( KGCHelper );
