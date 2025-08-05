#pragma once

typedef std::pair<std::pair<int,int>,std::pair<int,int>> HERO_OPEN_TIME;

struct KHeroItemInfo 
{
    PAIR_USHORT_DWORD           m_pairIdIndex;
    KManufactureItem            m_ItemInfo;
    std::vector<KDropItemInfo>  m_vecMaterialInfo;
};

class KGCHeroDungeonManager
{
public:
    KGCHeroDungeonManager();
    ~KGCHeroDungeonManager();

public:
    GCITEMID                    m_PointItemID;      // 영웅포인트ItemID
    std::vector<KHeroDungeon>   m_vecDungeonInfo;   // 영웅던전 정보
    time_t                      m_tmResetTime;      // 입장 초기화 시간
    std::map<PAIR_USHORT_DWORD, KManufactureItem>            m_mapCatalog;      // 영웅 아이템 정보
    std::map<PAIR_USHORT_DWORD, std::vector<KDropItemInfo> > m_mapMaterial;     // 영웅 아이템 재료 정보 
    KEVENT_HERO_ITEM_UPGRADE_INFO_ACK                        m_mapUpgradeInfo;  // 아이템 업그레이드 정보  
    std::map< int, KHeroPlayData >                           m_mapHeroPlayData; // 영웅던전 플레이 데이터
    std::map< int, std::map< int, KDungeonPersonalRecord > > m_mapDungeonPersonalRecord;    // < ModeID, < Stage, Clear > >
    std::map< int, std::map< int, KDungeonRankInfo > >       m_mapMyCurDungeonRank;         // < CharType, < ModeID, 현재 내 랭크 >
    std::map< int, std::map< int, KDungeonRankInfo > >       m_mapMyOldDungeonRank;         // < CharType, < ModeID, 지난 내 랭크 >
    std::map< int, std::map<int, std::vector< KDungeonRankInfo >>>  m_mapCurDungeonRank;    // < ModeID, < PageNum, Rank > >
    std::map< int, std::map<int, std::vector< KDungeonRankInfo >>>  m_mapOldDungeonRank;    // < ModeID, < PageNum, Rank > >

public:
    void Release();
    void SetDungeonPersonalRecord( std::map< int, std::map< int, KDungeonPersonalRecord > > _mapDungeonPersonalRecord );
    bool GetDungeonPersonalRecord( int nGameMode, std::map< int, KDungeonPersonalRecord >& _mapDungeonPersonalRecord );
    void SetMyDungeonRank( bool bCurrent, int nCharType, int nModeID, const KDungeonRankInfo& kDungeonRank );
    bool GetMyDungeonRank( bool bCurrent, int nCharType, int nModeID, KDungeonRankInfo& kDungeonRank );
    void SetDungeonRank( bool bCurrent, int nModeID, int nPageNum, std::vector< KDungeonRankInfo >& vecDungeonRank );
    bool GetDungeonRank( bool bCurrent, int nModeID, int nPageNum, std::vector< KDungeonRankInfo >& vecDungeonRank );    
    void SetHeroDungeonInfo( const std::vector<KHeroDungeon>& _vecDungeonInfo, GCITEMID _PointItemID, time_t _tmResetTime );
    bool GetOpenTime( EGCGameMode _eGameMode, HERO_OPEN_TIME& _sOpenTime );
    bool GetNextTime( EGCGameMode _eGameMode, HERO_OPEN_TIME& _sNextTime );
    bool GetHeroDungeon( EGCGameMode _eGameMode, KHeroDungeon& kHeroDungeon );
    bool IsFirstOpenTime( int nGameMode ); //   첫번째 오픈 시간 전이다. 
    std::wstring GetInitTime();
    std::wstring GetNextEntranceTime( EGCGameMode _eGameMode );
	std::pair<int, int> GetClearInfo( EGCGameMode _eGameMode );
    bool GetClearReward ( EGCGameMode emGameMode, std::map<INDEX_GCITEMID,KManufactureItem>& _mapClearReward );
    int  GetNumReward( EGCGameMode emGameMode );
    int GetClearMonsterLevel( EGCGameMode emGameMode );
    void SetHeroItemCatalog( std::map<PAIR_USHORT_DWORD, KManufactureItem>& mapCatalog );
    void SetHeroItemMaterial( std::map<PAIR_USHORT_DWORD, std::vector<KDropItemInfo> >& mapMaterial );
    void SetHeroItemUpgradeInfo( const IN KEVENT_HERO_ITEM_UPGRADE_INFO_ACK& Info_ );
	std::wstring GetOpenEntranceTime( EGCGameMode _eGameMode );
    void SetHeroPlayData( std::map< int, KHeroPlayData > _mapHeroPlayData );
    void ReSetHeroPlayData();
    void AddHeroDungeonPlayCount( int nGameMode );
    void SetHeroDungeonPlayTicket( int nGameMode );
    int GetHeroDungeonPlayCount( int nGameMode );
    bool GetHeroDungeonPlayTicket( int nGameMode );
};