#pragma once
#include <map>
#include <ToString.h>
#include <KNCSingleton.h>
#include <Thread/Locker.h>
#include <RTTI.H>
#include "UserPacket.h"
#include "CommonPacket.h"
#include "GCGradeDecider.h"
#include "GCAttributeDecider.h"
#include "GCSocketDecider.h"
#include "GCSocketConsumeItem.h"
#include "ItemBreakUp.h"
#include "ManufactureS3.h"
#include "GCItemConfig.h"
#include "ItemLevelDecider.h"

struct sSellItemInfo
{
    GCITEMID   m_dwItemID;
    char    m_cSellMoneyType;
    int     m_nBuyPrice;
    int     m_nBuyCount;
    int     m_nDivideFactor;
};

struct kWaitInvenMigrationData
{
    DWORD   m_dwUserUID;
    __int64 m_biWaitItemUID;
    KItem   m_kItem;
    int     m_nItemClass;
    int     m_nGetType;
};

class KItemManager
{
    DeclareSingleton( KItemManager );
    NiDeclareRootRTTI( KItemManager );
    DeclToStringW;

public:
    enum EMONEYTYPE
    {
        EMT_INVALID =0,
        EMT_CASH,
        EMT_GAMEPOINT,
        EMT_VIRTUALCASH,
        EMT_CRYSTAL,
        EMT_GCPOINT,
        EMT_GEM,
    };

    enum EWAITITEMTYPE
    {
        EWIT_SERVER = 0,
        EWIT_CASH_BUY = 1,
        EWIT_GP_BUY = 2,
        EWIT_VCASH_BUY = 3,
        EWIT_CRISTAL_BUY = 4,
        EWIT_GCPOINT_BUY = 5,
        EWIT_PACKAGE = 6, // 싼타 지급
        EWIT_PET_SYSTEM = 7,
        EWIT_ITEM_SELL = 8,
        EWIT_REWARD = 9, // 주간 월간 랭킹
        EWIT_TOOL = 10, // D-Bot
        EWIT_GIFT = 11, // 선물
        EWIT_GEM_BUY = 20, // 젬 구매       20번부터 시작.( 다른곳에서 사용할수도 있음. )
        EWIT_GAME_DROP = 21, // 드랍으로 획득.
        EWIT_MISSION = 22, // 미션 완료 보상 획득.
        EWIT_SHANGHAI = 23, // 상하이 드랍으로 획득.
        EWIT_KAIRO = 24, // KAIRO 획득.
        EWIT_EVENT_DROP = 25, // EVENT DROP 획득.
        EWIT_ATTEND_REWARD = 26, // 출석보상.
        EWIT_MANUFACTURE = 27, // 제작 아이템.
        EWIT_MIX_EMOTICON = 28, // 이모티콘 조합.
        EWIT_GACHA = 29, // 가챠 보상.
        EWIT_VIP_BUY = 30, // VIP아이템 구매.
        EWIT_CAPSULE = 31, // GP캡슐 보상.
        EWIT_COUPON_REWARD = 32, // 게임쿠폰 보상.
        EWIT_GCCLUB = 33, // GCClub 보상.
        EWIT_ITEM_TRADE = 34, // 아이템 교환.
        EWIT_MONSTER = 35, // 몬스터 카드.
        EWIT_BREAKUP = 36, // BREAKUP 아이템 지급.
        EWIT_IP_REWARD = 37, // PC방 보상.
        EWIT_EXTERNAL = 38, // 아이템 지급.
        EWIT_GUILDITEM = 39, // 길드 승격에 따른 아이템 지급.
        EWIT_COMPLETE_ACHIEVE = 40, // 업적 완료.
        EWIT_GP_GAMBLE_BUY = 41, // GP아이템 겜블 구매
        EWIT_PROMOTION_REWARD = 42, // 전직 시스템 보상.
        EWIT_HEROITEM = 43, // 영웅포인트 아이템 구매.
        EWIT_EVENT_REPETITION_PROVIDE = 44,
        EWIT_EVENT_YAHOO_BENEFIT = 45,
        EWIT_EVENT_DONATION_POINT = 46,
        EWIT_EVENT_LUNA_NEWYEAR = 47,
        EWIT_EVENT_RECOMMEND_FULL = 48,
        EWIT_EVENT_ATTENDANCE_BONUS = 49,
        EWIT_EVENT_RECOM_DAILY_ATTENDANCE = 50,
        EWIT_EVENT_RAINBOW_JACKPOT_ITEM = 51,
        EWIT_EVENT_CO_OP_ITEM_BUY = 52,
        EWIT_EVENT_MISSION_ITEM = 53,
        EWIT_EVENT_SUBSCRIPT = 54,
        EWIT_EVENT_NPC_GIFT = 55,
        EWIT_EVENT_DICE_PLAY = 56,
        EWIT_EVENT_DONATION_ITEM_GET = 57,
        EWIT_EVENT_BINGO_ANSWER = 58,
        EWIT_EVENT_GWC_DANATION = 59,
        EWIT_EVENT_RECOMMEND_LEVELUP = 60,
        EWIT_EVENT_CHANNELING_ITEM = 61,
        EWIT_EVENT_TIME_DROP = 62,
        EWIT_EVENT_PRESENTBOX_TRADE = 63,
        EWIT_EVENT_PRESENTBOX_ACTION = 64,
        EWIT_GAWIBAWIBO_TRADE = 65, // 가위바위보 아이템<>포인트 교환
        EWIT_GAWIBAWIBO_REWARD = 66, // 가위바위보 보상
        EWIT_SOCKS_REWARD = 67, // 크리스마스 양말 보상
        EWIT_GAME_SPECAIL_DROP = 68, // 던전 보상상자 드랍
        EWIT_PLANTTREE_REWARD = 69, // 나무나무 이벤트 보상.
        EWIT_CHOICE_DROP = 70, // 초이스 드랍 보상.
        EWIT_RKTORNADO_REWARD = 71, // 릭쿤 토네이도 보상.
        EWIT_LAUSERAUTH_REWARD = 72, // 남미 미국 유저 인증 보상.
        EWIT_VIP_EVENT = 73, // VIP 이벤트
        EWIT_MAGIC_BOX = 74, // 두근두근 마법상자
        EWIT_MILEAGE_SYSTEM = 75, // 태국 마일리지 시스템
        EWIT_SPHINX_EVENT = 76, // 스핑크스 OX 보상.
        EWIT_AGIT_STORE = 77, // 아지트 상점
        EWIT_AGIT_OBJ_SELL = 78, // 아지트 물건 판매
        EWIT_FLOWER_POT = 79, // 아지트 화분 채집
        EWIT_FLOWER_POT_BONUS = 80, // 아지트 화분 채집보너스
        EWIT_AGIT_STAY_BONUS = 81, // 아지트 체제시간 보너스
        EWIT_AGIT_ITEM_INIT = 82, // 아지트 일일 아이템 초기화 지급
        EWIT_BROWNIE_STORE = 83, // 도우미상점
        EWIT_AGIT_AUTO_DROP = 84, // 아지트 자동 드랍 (화분,가구)
        EWIT_STRENGTH = 85, // 강화 재료 사용.
        EWIT_SURVEY = 86, // 설문조사 보상.
        EWIT_GWCEVENT = 87, // GWC 이벤트 보상.
        EWIT_EVENT_JIT_REWARD = 88,  // Just In Time 보상
        EWIT_CHARISMASEVENT = 89, // 크리스마스 이벤트 보상.
        EWIT_POST_SYSTEM = 90, // 시스템우편 첨부 아이템
        EWIT_POST_USER = 91, // 유저우편 첨부 아이템
        EWIT_DUNGEON_CLEAR_REWARD = 92, // 던전 클리어보상
        EWIT_MONSTER_CARD_MIX = 93, // 몬스터카드 조합
        EWIT_ECLIPSE_COLLECT_REWARD = 94, // 고서 모으기 이벤트 보상.
        EWIT_ECLIPSE_PLOT_REWARD = 95, // 개기일식의 음모 보상
        EWIT_SONGKRAN_EVENT_REWARD = 96, // 송크란 이벤트 보상
        EWIT_ATTEND_REWARD_GET_CHAR = 97, // 출석 n회 보상
        EWIT_ADVENTURE_CONTINENT_REWARD = 98, // 모험 이벤트 대륙 보상
        EWIT_ADVENTURE_FINAL_REWARD = 99, // 모험 이벤트 최종 보상
        EWIT_LEVEL_RESET_EVENT_REWARD = 100, // 레벨 초기화 이벤트 보상
        EWIT_TEST_ADD_ITEM = 101, // 사내 테스트용 아이템
        EWIT_OLYMPIC_EVENT_REWARD = 102, // 올림픽 이벤트 보상
        EWIT_CHAR_DEFAULT_REWARD = 103, // 캐릭터 생성시 기본 복장 보상.
        EWIT_GACHA_LOTTERY_REWARD  = 104,   // 가챠 복권 보상.
        EWIT_CHAR_LEVEL_EVENT_REWARD = 105,   // 캐릭터 특정 레벨 보상 이벤트
        EWIT_CONNECT_EVENT_REWARD = 106, // 접속 보상 이벤트
        EWIT_ITEM_COMPOSE_REWARD = 107, // 아이템 합성 이벤트
        EWIT_GAME_RESULT_REWARD = 108, // 게임결과 보상.
        EWIT_RITAS_CHRISTMAS_SPECIAL_REWARD = 109, // 리타의 크리스마스 이벤트 스페셜 상자 보상
        EWIT_RITAS_CHRISTMAS_PREMIUM_REWARD = 110, // 리타의 크리스마스 이벤트 프리미엄 상자 보상
        EWIT_HEROITEM_UPGRADE = 111, // 영던템 업그레이드로 획득
        EWIT_VALENTINE_BUFF_DONATION = 112, // 발렌타인 기부 이벤트로 획득 (BuffDonation)
        EWIT_ACCUMULATE_ATTENDANCE = 113, // 누적 출석 이벤트로 획득
        EWIT_ERRAND_EVENT_REWARD = 114, // 심부름왕 이벤트 보상으로 획득
        EWIT_DUNGEON_SUBJECT_REWARD = 115, // 던전 달성과제 보상으로 획득
        EWIT_CHARACTER_GRADUATION_REWARD = 116, // 캐릭터 졸업 보상으로 획득
        EWIT_SYSTEM_GUIDE_PROVIDE = 117, // 가이드 제공 시 지급 하는 아이템
        EWIT_INVEN_TO_DEPOT = 118, // 인벤토리에서 창고로 수량 아이템 일부 이동으로 아이템 생성
        EWIT_DEPOT_TO_INVEN = 119, // 창고에서 인벤토리로 수량 아이템 일부 이동으로 아이템 생성
        EWIT_JUMPING_CHAR_REWARD = 120, // 점핑 캐릭터 보상.
        EWIT_TONG_DONATION_DONATE_REWARD = 121, // 통합 기부 이벤트 기부할 때 받는 보상
        EWIT_CONNECTION_GIFTBOX_FIRST_CHARACTER_REWARD = 122, // 첫 캐릭터 접속 선물상자 시스템 보상
        EWIT_CONNECTION_GIFTBOX_NEW_USER_REWARD = 123, // 신규유저 접속 선물상자 시스템 보상
        EWIT_CONNECTION_GIFTBOX_EVENT_USER_REWARD = 124, // 이벤트유저 접속 선물상자 이벤트 보상
        EWIT_VIRTUAL_DEPOT_TO_INVEN = 125, // 가상창고에서 인벤토리로 수량 아이템 일부 이동으로 아이템 생성
        EWIT_SC_GROUP_PURCHASE_ONE_PLUS_ONE = 126, // 소셜커머스 공동구매 1+1 보상 획득
        EWIT_SC_GROUP_PURCHASE_BONUS = 127, // 소셜커머스 공동구매 보너스 아이템 보상 획득
        EWIT_INFINITY_DUNGEON_DROP = 128, // 무한던전 아이템 획득
        EWIT_GACHA_PON = 129,
    };

    enum EITEMDELETETYPE
    {
        EDEL_USE                    = 0, // 현재 사용중인 아이템
        EDEL_SELL                   = 1, // 되팔기 아이템
        EDEL_DEL_COUNT              = 2, // 수량 사용 다함
        EDEL_DEL_PERIOD             = 3, // 기간 만료
        EDEL_DEL_ITEM               = 4, // 서버 미사용
        EDEL_DEL_USER               = 5, // 유저 삭제.
        EDEL_DEL_DBOT               = 6, // D-Bot으로 아이템 삭제.
        EDEL_DEL_BREAKUP            = 7, // 아이템 해체.
        EDEL_DEL_STRENGTH           = 8, // 강화석 삭제.
        EDEL_DEL_STRENGTH_EQUIP     = 9, // 강화석 장착 아이템 삭제.
        EDEL_DEL_STRENGTH_MATERIAL  = 10, // 강화 재료 아이템 삭제.
        EDEL_DEL_POST_ITEM          = 11, // 우편 첨부아이템 삭제
        EDEL_DEL_COORD_COMPOSITE    = 12, // 코디 합성(아바타) 아이템(외형) 삭제
        EDEL_DEL_SKILL_SLOT_INDEX   = 13, // 스킬 슬롯 인덱스 아이템 삭제.
        EDEL_DEL_STRENGTH_CONNECTION= 14, // 서버 접속시 유효하지 않는 강화석 삭제.
        EDEL_DEL_DEPOT              = 15, // 창고<->인벤토리 이동으로 수량 삭제
        EDEL_DEL_COORDI_COMPOSE     = 16, // 코디 합성 아이템 삭제.
        EDEL_DEL_VIRTUAL_DEPOT      = 17, // 가상창고<->인벤토리 이동으로 수량 삭제
    };

    enum MONEY_CONSUMPTION_TYPE {
        EMC_CASH = 10000, // 10000 ~ 10100 reserve for cash 
        EMC_GP = 10100 , // 10100 ~ 10199 reserve for gp
        EMC_GEM = 10200, // 10200 ~ 10299 reserve for gem
        EMC_HEROITEM = 10300, // 
        EMC_GCPOINT = 10400, // 출석포인트
        EMC_VP = 10500, // VP 가상캐시
        EMC_VIP = 10600, // VIP
    };
    typedef std::vector< std::pair<DWORD,char> > PAIR_VEC;

public:
    KItemManager(void);
    ~KItemManager(void);

    void SetUpdateGap( DWORD dwUpdateGap );
    bool CheckUpdateList();
    bool GetSellInfo( IN GCITEMID dwItmeID, OUT KSellInfo& kSellInfo );
    void UpdateSellItemInfo( IN std::map<GCITEMID, sSellItemInfo>& mapSellItemInfo );

    //아이템 등급, 속성, 소켓, 레벨 테이블 정보.
    int GetGradeTableID( GCITEMID dwItemID );
    int GetAttributeTableID( GCITEMID dwItemID );
    int GetSocketTableID( GCITEMID dwItemID );
    int GetLevelTableID( IN GCITEMID dwItemID );
    int GetEquipLvDownLimit( IN GCITEMID dwItemID );

    void SetGradeTableID( GCITEMID dwItemID, int iTableID );
    void SetAttributeTableID( GCITEMID dwItemID, int iTableID );
    void SetSocketTableID( GCITEMID dwItemID, int iTableID );
    void SetLevelTableID( IN GCITEMID dwItemID, IN int iTableID );
    void SetEquipLvDownLimit( IN GCITEMID dwItemID, IN int nLvLimit );

    bool LoadTableScript( OUT std::string& strScript );
    bool SaveTableScript( std::string strFilePath );

    int GetAttributeCountFromGrade( IN int nGrade );

    void UpdateItemEquipLevelList( std::map< DWORD, DWORD >& mapEquipItemLevel );
    bool GetItemEquipLevel( IN const GCITEMID ItemID_, OUT DWORD& EquipLevel_ );

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool LoadScript_MonsterCard( OUT std::string& strScript );
    bool FindMonsterCardByItemID( GCITEMID ItemID );

    void UpdatePackageInfo( IN std::vector<std::map<GCITEMID,std::vector<KDropItemInfo > > >& vecList );
    void GetPackageInfo( OUT std::vector<std::map<GCITEMID,std::vector<KDropItemInfo > > >& vecList );
    size_t GetPackageInfoSize();

    void DumpBreakupList();
    void DumpBreakupInfo( IN const int& nLv, IN const int& nGrade );
    void DumpSetItemInfo( IN const GCITEMID& dwSetID );

    void SetBuyLimitItem( IN const std::set<GCITEMID>& setList );
    bool IsBuyLimitItem( IN const GCITEMID& ItemID );
    void DumpBuyLimitItem();

    int GetUseSocketItemCount( IN const KItem& kItem_ );
    bool LoadInvenBuffItem( OUT std::string& strScript );
    void GetInvenBuffItemList( OUT std::vector<KInvenBuffItem>& vecList );
    bool GetInvenBuffItem( IN const GCITEMID& ItemID, OUT KInvenBuffItem& kInfo );
    bool GetGroupItemID( IN const int& nGroupID, OUT std::map<USHORT,GCITEMID>& mapItemID );

    void GetItemBreakupInfo( OUT KEVENT_SPECIFIC_ITEM_BREAKUP_INFO_ACK& kPacket_ );

    bool IsPackageItem( IN GCITEMID dwItemID );

private:
    int _GetTableID( GCITEMID dwItemID, std::map<GCITEMID,int>& mapTable );
    void _SetTableID( GCITEMID dwItemID, int iTableID, std::map<GCITEMID,int>& mapTable );

public:
    KGCGradeDecider                 m_kGradeDecider;
    KGCAttributeDecider             m_kAttributeDecider;
    KGCSocketDecider                m_kSocketDecider;
    KGCSocketConsumeItem            m_kSocketCashItem;
    KItemBreakUp                    m_kItemBreakup;
    KManufactureS3                  m_kItemManufacture;
    KGCItemConfig                   m_kItemConfig;
    KItemLevelDecider               m_kItemLevelDecider;

protected:
    mutable KncCriticalSection      m_csSellItem;
    DWORD                           m_dwUpdateGap;
    DWORD                           m_dwLastUpdateTick;
    std::map<GCITEMID,sSellItemInfo> m_mapSellItemInfo;

    mutable KncCriticalSection      m_csItems;
    std::map<DWORD,DWORD>           m_mapItemEquipLevel;
    std::map<DWORD, bool>           m_mapMonsterCard; // ID, use or not


    mutable KncCriticalSection      m_csItemTables;
    std::set<GCITEMID>              m_setItemList;
    std::map<GCITEMID,int>          m_mapItemGradeTable;
    std::map<GCITEMID,int>          m_mapItemAttributeTable;
    std::map<GCITEMID,int>          m_mapItemSocketTable;
    std::map<GCITEMID,int>          m_mapItemLevelTable;
    std::map<GCITEMID,int>          m_mapEquipLvDownLimit;

    mutable KncCriticalSection      m_csPackageItem;
    std::map<GCITEMID,std::vector<KDropItemInfo> >  m_mapPackageItemList; // map[PackageItemID, vector[Item]]
    std::vector<std::map<GCITEMID,std::vector<KDropItemInfo> >> m_vecPackageTotalList;

    mutable KncCriticalSection      m_csBuyLimitItem;
    std::set<GCITEMID>              m_setBuyLimitItem; // 구매제한 ItemID

    mutable KncCriticalSection      m_csInvenBuffItem;
    std::vector<KInvenBuffItem>     m_vecInvenBuffItem; // 인벤토리 버프 아이템
    std::map<int,std::map<USHORT,GCITEMID> > m_mapItemGroup; // map[GroupID, map[Index,ItemID] ]
};

DefSingletonInline( KItemManager );
DeclOstmOperatorA( KItemManager );