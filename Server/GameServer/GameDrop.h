#pragma once
#include "UserPacket.h"
#include <KNCSingleton.h>
#include <Thread/Locker.h>
#include <boost/random.hpp>
#include "Lottery.h"
#include "Kairo.h"

struct sGPDropInfo
{
    DWORD   m_dwSilverID;
    int     m_nSilverCount;
    float   m_fSilverRatio;

    DWORD   m_dwGoldID;
    int     m_nGoldCount;
    float   m_fGoldRatio;
};

struct sMondropElement
{
    char    m_cRewardType;
    GCITEMID   m_dwItemID;
    int     m_nCount;
    int     m_nPeriod;
    int     m_nStrongLv;
    float   m_fRatio;
};

struct sMonDropInfo
{
    int                                             m_nMonID;
    std::map< int, std::vector<sMondropElement> >   m_mapDropInfo; // < LV, dropElement >
};

struct sMonDrops
{
    int m_nDungeonID;
    int m_nDifficult;

    std::vector< sMondropElement >  m_vecDefaultDrop;
    std::vector< sMondropElement >  m_vecChampDrop;

    std::vector< std::pair<KLottery, std::vector<sMondropElement> > >   m_vecDefaultGroup;
    std::vector< std::pair<KLottery, std::vector<sMondropElement> > >   m_vecChampGroup;
};

struct sMonCharDrops
{
    int m_nDungeonID;
    int m_nDifficult;

    std::vector< sMondropElement >  m_vecDefaultDrop;
    std::vector< sMondropElement >  m_vecChampDrop;

    std::map<char, std::pair<KLottery, std::vector<sMondropElement>>> m_mapCharDefaultGroup;
    std::map<char, std::pair<KLottery, std::vector<sMondropElement>>> m_mapCharChampGroup;
};

struct sMonsterInfo
{
    int                                     m_nMonID;
    std::map<std::pair<int,int>, sMonDrops> m_mapMonDrops;
    std::map<std::pair<int,int>, sMonCharDrops> m_mapMonCharDrops;

    void AppendNormalDrop( IN const int nTriggerID, IN const int nDungeonID,
                           IN const int nDifficult ,OUT std::vector< KGameDropElement >& vecMonsterDrop,
                           IN float fScale, IN int nStageCount, IN const DWORD& dwDropType, IN const std::map<DWORD, std::pair<char,int> >& mapCharLevelInfo );
    void AppendChampionDrop( IN const int nTriggerID, IN const int nDungeonID,
                             IN const int nDifficult ,OUT std::vector< KGameDropElement >& vecMonsterDrop,
                             IN float fScale, IN const DWORD& dwDropType, IN const std::map<DWORD, std::pair<char,int> >& mapCharLevelInfo );
    void AppendDrop( IN const int nTriggerID, IN std::vector< sMondropElement >& vecDrop,
                     OUT std::vector< KGameDropElement >& vecMonsterDrop, IN float fScale,
                     IN int nStageCount );
    void AppendGroupDrop( IN const int nTriggerID,
                          IN std::vector< std::pair<KLottery, std::vector<sMondropElement> > >& vecGroupDrop,
                          OUT std::vector< KGameDropElement >& vecMonsterDrop, IN float fScale,
                          IN int nStageCount, IN const std::map<DWORD, std::pair<char,int> >& mapCharLevelInfo );
    void AppendCharGroupDrop( IN const int nTriggerID,
        IN std::map<char, std::pair<KLottery, std::vector<sMondropElement>>>& mapCharDefaultGroup_,
        OUT std::vector< KGameDropElement >& vecMonsterDrop, IN float fScale,
        IN int nStageCount, IN const std::map<DWORD, std::pair<char,int> >& mapCharLevelInfo );

    void AppendGpDrop( IN const int nTriggerID, IN const int nMonLv, IN const int nGP, IN const sGPDropInfo& kInfo, OUT std::vector< KGameDropElement >& vecMonsterGpDrop );
    void AppendGP(  IN const int nTriggerID, IN const GCITEMID dwItemID, IN const int nGP, IN const float fRatio,  OUT std::vector< KGameDropElement >& vecMonsterGpDrop );
};

struct sTriggerInfo
{
    int m_nStage;
    int m_nTriggerID;
    int m_nMonsterID;
    int m_nMonLv;
    int m_nIsBoss; // 일반 몬스터:0, 보스 몬스터:클리어한 스테이지의 개수로 세팅
};

// 경험치 Table
struct sLevelReward
{
    int     m_nExp;
    int     m_nSP;
    int     m_nGP;
};

// 몬스터 별 경험치 가중치.
struct sMonAddRatio
{
    float m_fExpRatio;
    float m_fSPRatio;
    float m_fGPRatio;
};

struct sChampionInfo
{
    int                         m_nDungeonID;
    int                         m_nDungeonLv;
    int                         m_nMobGrade;
    KLottery                    m_kChampionCount;
    KLottery                    m_kPropertyCount;
    std::vector< KLottery >     m_vecProperty;

    int GetChampionCount()
    {
        return std::max<int>( m_kChampionCount.Decision(), 0 );
    }
    DWORD GetProperty();
};

struct sMonPropertyDropInfo
{
    int                 m_nMonID;
    DWORD               m_dwProperty;
    sMondropElement     m_monDropElement;
};

class KLuaManager;
class KResultManager;
class KGameDrop
{
    DeclareSingleton( KGameDrop );
    DeclToStringW;

public:
    union UChampKey
    {
        USHORT  m_uaKeyList[4];
        __int64 m_biChampKey;
        UChampKey()
        {
            m_biChampKey = 0;
        }
    };

public:
    enum DROP_TYPE
    {
        DT_TOTAL        = 0, // 전체 캐릭터 아이템 드랍.
        DT_CHAR         = 1, // 플레이한 캐릭터 Drop.
        DT_MAX,
    };

    KGameDrop(void);
    ~KGameDrop(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool LoadMonsterScript( OUT std::string& strScript );
    bool LoadTriggerScript( OUT std::string& strScript );
    bool LoadChampionScript( OUT std::string& strScript );

    bool GetDropInfo( IN const KGameInfo& kInfo, OUT std::vector<KGameDropElement>& vecMonsterDrop, OUT std::vector< KGameDropElement >& vecGpDrop,
                      OUT std::vector<KChampionInfo>& vecChampions, IN const float fScale );
    void SetNumbering( IN OUT KDropData& dropData );

    bool DistributeDrops( IN OUT KDropData& kReservedDrop, 
                          IN OUT KEndGameReq& kReq,
                          OUT std::map<DWORD, DROP_ITEMS>& mapEachUserDrops );

    bool DistributeRareDrops( IN OUT KDropData& kReservedDrop, 
                              IN OUT KEndGameReq& kReq,
                              OUT std::map<DWORD, DROP_ITEMS>& mapEachUserDrops );

    bool AdjustDrops( IN const std::map<DWORD, DROP_ITEMS>& mapInDrops,
                      OUT std::map<DWORD, DROP_ITEMS>& mapOutDrops );

    bool GetTriggerInfo( IN int nGameMode, IN int nDifficult, OUT std::vector<sTriggerInfo>& vecTriggerInfo );

    float GetRatio();
    DWORD GetRandomNum() { return die(); };

    float GetMonExp( int nMonID, int nLv ); 
    float GetMonGp( int nMonID, int nLv );
    float GetMonSpx( int nMonID, int nLv );

    int GetCharLvMonExp( int nLv );    // 캐릭터 레벨을 통해서 경험치를 가져온다.
    int GetCharLvMonGP( int nLv );     // 캐릭터 레벨을 통해서 GP 보상을 가져온다.

    void SetBonusRatio( float fBonusRatio ) { KLocker lock(m_csMultiplyRatio); m_fMultiplyRatio = fBonusRatio; };

    bool DistributeDrop( IN OUT KDropData& kDropInfo, IN const std::vector<DWORD>& vecUserUIDs, IN const DWORD& dwUserUID, IN const DWORD& dwDropUID, OUT std::pair<DWORD,KDropItemInfo>& prUserDrops, 
        IN const std::map<DWORD, int>& mapFatigueUserList_ );
    bool DistributeRareDrop( IN OUT KDropData& kDropInfo, IN const std::vector<DWORD>& vecUserUIDs, IN OUT std::vector<DWORD>& vecRouletUserUIDs, IN const DWORD& dwDropUID, OUT std::pair<DWORD,KDropItemInfo>& prUserDrops, IN std::vector<KDropItemInfo> vecPartyDrop, OUT std::vector< std::pair<DWORD,KDropItemInfo> >& vecPartyDrops, IN const std::map<DWORD, int>& mapFatigueUserList_ );

    void DeleteFatigueUserDistributeItem( IN std::map<DWORD, DROP_ITEMS>& mapEachUserDrops_, IN std::map<DWORD, int>& mapFatigueUserList_ );
    void MakeGPDropInfo( OUT sGPDropInfo& kInfo );
    bool MakeIDReservedDropInfoFromGameDrop( IN const KGameInfo& kGameInfo_, IN const float fDropEventScale_, IN std::list< KIDMonsterInfo >& lstIDSummonedMonsters_, IN DWORD dwIDReservedDropLastUID_, OUT std::list< KGameDropElement >& lstIDReservedDrops_, OUT DWORD& dwIDReservedDropLastUIDAfter_ );

protected:
    bool LoadMonsterInfo( IN OUT KLuaManager& kLuaMng, OUT sMonsterInfo& monInfo );
    bool LoadDropElement( IN OUT KLuaManager& kLuaMng, OUT sMondropElement& dropElement );
    bool LoadTriggerTable( IN OUT KLuaManager& kLuaMng, OUT std::map<int,std::vector< sTriggerInfo > >& mapTrigger );
    bool LoadChampionInfo( IN OUT KLuaManager& kLuaMng, OUT sChampionInfo& champInfo );
    bool LoadGPDropInfo( OUT std::string& strScript );
    bool LoadLottery( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT KLottery& kLottery );
    void GetChampions( IN const KGameInfo& kInfo, OUT std::vector<KChampionInfo>& vecChampions );
    bool PopDropData( IN OUT KDropData& kReservedDrop, IN DWORD dwDropUID, OUT KGameDropElement& dropEl, IN DWORD dwUserUID );
    float GetScaleInfo( IN const KGameInfo& kInfo );
    void ApplyBoostRatio( IN OUT float& fRatio );


    // MonLevelReward Load
    bool LoadMonsterRewardScript( OUT std::string& strScript );
    bool LoadLevelRewardInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< int, sLevelReward >& mapMonLevelReward );
    bool LoadRewardRatioInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< int, sMonAddRatio >& mapMonAddRatio );
    bool LoadLevelReward( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT sLevelReward& sLvReward );
    bool LoadRewardRatio( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT sMonAddRatio& sMonRatio );

    // CharGameDrop Load
    bool LoadCharMonsterScript( OUT std::string& strScript );
    bool LoadMonsterCharInfo( IN OUT KLuaManager& kLuaMng, OUT sMonsterInfo& monInfo );

    // CharDropRatio Load
    bool LoadMonsterDropRatio( OUT std::string& strScript );
    bool LoadRatio( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT Kairo& kairo );

    bool MonsterDropResult( IN const int& nDungeonID, IN const int& nDifficult, OUT DWORD& dwResult );

    mutable KncCriticalSection      m_csMonsterInfo;
    std::map< int, sMonsterInfo >   m_mapMonsterInfo;
    std::map< int, sMonsterInfo >   m_mapMonsterCharInfo; // 캐릭터별 몬스터 드랍 관리 컨테이너
    std::map< std::pair<int,int>, Kairo > m_mapMonsterDropRatio;

    mutable KncCriticalSection                                                  m_csDungeonTrigger;
    std::map< std::pair<int,int>, std::map<int, std::vector<sTriggerInfo> > >   m_mapDungeonTrigger; // Key <QuestID,Lv>, Data <stage,Triggers>

    mutable KncCriticalSection                  m_csChampionTable;
    std::map<__int64, sChampionInfo> m_mapChampionTable;

    boost::lagged_fibonacci607 rng;
    boost::uniform_int<DWORD> uint32;
    boost::variate_generator<boost::lagged_fibonacci607&, boost::uniform_int<DWORD> > die;

    mutable KncCriticalSection                  m_csMultiplyRatio;
    float                                       m_fMultiplyRatio; // Bonus

    mutable KncCriticalSection  m_csGPDrop;

    float                       m_fSilverCoinRatio;
    float                       m_fGoldCoinRatio;
    float                       m_fJackPotRatio;
    int                         m_nJacpotCoinCount;
    KLottery                    m_SilverCoinCount;
    KLottery                    m_GoldCoinCount;

    mutable KncCriticalSection  m_csMonReward;
    // 몬스터 경험치 Table.
    std::map< int, sLevelReward > m_mapMonLevelReward;
    // 몬스터 경험치별 가중치 Table.
    std::map< int, sMonAddRatio > m_mapMonAddRatio;
};

DefSingletonInline( KGameDrop );
DeclOstmOperatorA( KGameDrop );