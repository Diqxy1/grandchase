#pragma once
#include <KncUtil.h>
#include "UserPacket.h"
#include <KNCSingleton.h>
#include <Thread/Locker.h>
#include <RTTI.H>
#include <boost/tuple/tuple.hpp>
#include "GameDrop.h"
#include "LookEquipment.h"

struct KItem;
class KItemContainer;
struct sStageInfo
{
    int m_nDifficult;
    int m_nLv;
    int m_nExp;
    int m_nSpx;
    int m_nGp;
    int m_nBonusExp;
    int m_nBonusGP;
    int m_nMinStage;
    int m_nMaxStage;

    void clear()
    {
        m_nDifficult = 0;
        m_nLv = 0;
        m_nExp = 0;
        m_nSpx = 0;
        m_nGp = 0;
        m_nBonusExp = 0;
        m_nBonusGP = 0;
        m_nMinStage = 0;
        m_nMaxStage = 0;
    }
};

struct sQuestInfo
{
    int                             m_nQuestID;     // DungeonID
    std::map< int, sStageInfo >     m_mapStageInfo;   // < Difficult, stageinfo >
};

struct sBoostDetailInfo
{
    char    m_cCharType;        // 혜택받는 캐릭터 타입,  -1 이라면 전캐릭 공통
    char    m_cPromotion;       // 혜택받는 전직 -1 이면 해당 캐릭터의 모든 전직
    float   m_fBoostExpRatio;   // 추가량만 적는다. 예를 들면 1.5배라면 0.5f가 들어가야한다.
    float   m_fBoostGpRatio;    // 추가량만 적는다. 예를 들면 1.5배라면 0.5f가 들어가야한다.
    float   m_fBoostSpxRatio;
};

// 곱하기에서 더하기로 바뀐다고 했으니 ...^^...
struct sBoostInfo
{
    GCITEMID   m_dwItemID;
    bool    m_bEquip;
    bool    m_bAll;    
    std::map< std::pair< char, char > ,sBoostDetailInfo >  m_mapBoostDetailInfo;
};

struct sMonsterHuntInfo
{
    int m_nDifficult;
    int m_nClearPoint;
    int m_nExp;
    int m_nSpx;
    int m_nGp;
};

struct sMonsterCrusaderInfo
{
    int     m_nMonsterID;
    float   m_fExpRatio;
    float   m_fGpRatio;
    float   m_fSpxRatio;
};

// 조정값
struct sReconcile
{
    int     m_nBegin;
    int     m_nEnd;
    float   m_fReconcValue;
};

struct sDefaultReward
{
    int     m_nLv;
    int     m_nMonExp;
    int     m_nMonGp;
    int     m_nDungeonExp;
};

struct sPVPReward
{
    int    m_nKillNum;
    int    m_nEventID;
};

class KLuaManager;
class KResultManager
{
    DeclareSingleton( KResultManager );
    DeclToStringW;
    NiDeclareRootRTTI( KResultManager );

public:
    struct SConfigData
    {
        float       m_fWinRatio;
        float       m_fLoseRatio;
        float       m_fAdditionalGPBoost;
        float       m_fAdditionalEXPBoost;
        float       m_fTestGPRatio;
        float       m_fTestEXPRatio;
        float       m_fTestSPXRatio;
        float       m_fBaseEXPRatio;
        float       m_fBaseGPRatio;
        float       m_fBaseSPXRatio;
        float       m_fClearBonusRatio;
        float       m_fPvpGpRatio;
        float       m_fAngelEggWinRatio;
        float       m_fAngelEggLoseRatio;

        SConfigData()
        {
            m_fWinRatio             = 1.f;
            m_fLoseRatio            = 0.2f;
            m_fAdditionalGPBoost    = 0.f;
            m_fAdditionalEXPBoost   = 0.f;
            m_fTestGPRatio          = 5.f;
            m_fTestEXPRatio         = 5.f;
            m_fTestSPXRatio         = 5.f;
            m_fBaseEXPRatio         = 1.f;
            m_fBaseGPRatio          = 1.f;
            m_fBaseSPXRatio         = 1.f;
            m_fClearBonusRatio      = 0.1f;
            m_fPvpGpRatio           = 1.f;
            m_fAngelEggWinRatio     = 3.f;
            m_fAngelEggLoseRatio    = 1.5f;
        }
    };

public:
    KResultManager(void);
    ~KResultManager(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    char Exp_2_Level( const __int64 biExp );
    __int64 GetGradePoint( const std::vector<__int64>& Exp );
    char GetGrade( const std::vector<__int64>& vecExp );

    int GetCrusaderMonCount( IN const std::map<std::pair<int,int>,int>& mapKillMon );
    __int64 GetMaxExp();
    __int64 GetExpByLv( DWORD dwLevel );

    typedef void (KResultManager::*Handler)( IN const KGameInfo&, IN std::map< std::pair<DWORD,char>, __int64 >&, IN std::map<DWORD,int>&, IN OUT KEndGameReq& );

    void CalcExpGP( IN const KGameInfo& kGameInfo,IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp, IN std::map<DWORD,int>& mapResultGP, IN OUT KEndGameReq& kReq );
    bool LoadQuestInfo( OUT std::string& strScript );
    void DumpQuestInfo( std::wostream& stm ) const;

    bool LoadBoostItemInfo( KLuaManager& kLuaMng );
    bool LoadMonsterHunt( KLuaManager& kLuaMng );
    bool LoadMonsterCrusader( KLuaManager& kLuaMng );
    bool LoadDungeonInfo( OUT std::string& strScript );
    bool LoadRankBonus( KLuaManager& kLuaMng );
    bool LoadDefaultReward( KLuaManager& kLuaMng );
    bool LoadConfigData( KLuaManager& kLuaMng );
    bool LoadCharLimit( KLuaManager& kLuaMng );
    bool LoadControlFeedbackBonus( KLuaManager& kLuaMng );
    void CheckAreaTypeBoost( IN const KGameInfo& kGameInfo, OUT float& fExpBoost, OUT float& fGpBoost );
    void CheckInventoryBoost( IN const int& nModeID, IN const std::vector<KItem>& vecInv, IN const std::map<int,std::set<GCITEMID> >& mapGroupItemAll,
                              OUT float& fExpBoost, OUT float& fGPBoost, IN OUT std::map<GCITEMID, sBoostInfo >& mapBoostAllUser );
    void CheckInventoryBoost( IN const int& nModeID, IN KItemContainer& kInven, IN const std::map<int,std::set<GCITEMID> >& mapGroupItemAll,
                              OUT float& fExpBoost, OUT float& fGPBoost, IN OUT std::map<GCITEMID, sBoostInfo >& mapBoostAllUser );
    void CheckEquipBoost( IN char cCharType, IN char cPromotion, IN const std::vector<KEquipItemInfo>& vecEquip, float& fExpBoost, float& fGPBoost, OUT std::map<DWORD,sBoostInfo>& mapAllBoostInfo );
    void CheckRankBoost( IN const KGameInfo& kGameInfo, IN int nRankPoint, OUT float& fExpBoost, OUT float& fGPBoost );
    void CheckEquipAttributeBoost( IN KItemContainer& kInven, IN const std::vector<KEquipItemInfo>& vecEquip, IN OUT float& fExpBoost, IN OUT float& fGPBoost );

    void CheckDungeonClearBoost( IN const KGameInfo& kGameInfo, IN OUT float& fExpBoost, IN OUT float& fGpBoost );
    void CheckEquipSocketBoost( IN const std::vector< KSocketInfo >& vecSocet, OUT float& fExpBoost, OUT float& fGPBoost );
    void CheckEquipSetBoost( IN const std::vector<KEquipItemInfo>& vecEquip, OUT float& fExpBoost, OUT float& fGPBoost );
    void CheckCoordiSetBoost( IN const char& cCharType, IN std::map<char,KLookEquipChar>& mapCoordi, IN KItemContainer& kInven, OUT float& fExpBoost, OUT float& fGPBoost );

    bool GetStageCountInfo( IN int nQuestID, IN int nDifficult, OUT std::pair<int,int>& StageCountInfo ); // 080223 tslayer.
    char GetCharGrade( DWORD dwWin );

    bool LoadPVPBonus( KLuaManager& kLuaMng );
    bool CheckPVPEvent( IN int nEventNumber, IN int nNumKills, IN bool bWin ); //  080603  woosh. 대전이벤트 하드코딩
    
    bool MiniGameGPReward( IN OUT KMiniGameRankInfo& kPacket ); //  080714  woosh. 미니게임 GP 보상
    int GetMiniGameGPRewardByStage( int dwClearStage_ );

    void DumpBoostInfo( std::wostream& stm ) const;

    bool LoadHellRankBonus( KLuaManager& kLuaMng ); // 081126.tslayer.헬모드 랭크 보너스 로드
    float GetHellRankBonus( IN int nRankPoint );
    void SetPerfectEventGameMode( IN const std::set<int>& setGameMode );
    void GetPerfectEventBonus( IN const int nGameMode, OUT float& fBonusRatio );

    bool LoadClearBonus( KLuaManager& kLuaMng );
    bool LoadExpSpxInfo( OUT std::string& strScript );
    int GetRankGrade( IN const int nRankPoint_, IN const bool bChampionMode_ );
    void GetLevelExp( IN const DWORD dwBeforeLevel, IN const __int64 biBeforeExp, OUT DWORD& dwAfterLevel, OUT __int64& biAfterExp );
    DWORD GetMaxLevel() { KLocker lock( m_csExpSpxInfo ); return m_dwMaxLevel; };
    void SetMaxLevel( IN const int maxLevel_ ) { KLocker lock( m_csExpSpxInfo ); m_dwMaxLevel = maxLevel_; };
    void GetApplyLvDiffRatio( IN const DWORD dwCharLevel, IN const int nMonLevel, OUT float& fMonReward ); // 레벨차 보정한 경험치.

    bool GetDifficulty( IN const int& nModeID, OUT std::set<int>& setDifficulty );
    bool CalcPvpExp( IN const DWORD& dwKillCharLevel, IN const DWORD& dwDeathCharLevel, OUT float& fExp );
    bool CalcPvpGP( IN const DWORD& dwDeathCharLevel, IN const int nTeamUserNum, OUT float& fGP );

    bool IsExistDifficulty( IN const int& nModeID, IN OUT int& nDifficulty );

    __int64 GetExpRewardLimit()     { KLocker lock( m_csCharLimit ); return m_biExpRewardLimit; }
    bool IsClippingExpRewardLimit() { KLocker lock( m_csCharLimit ); return m_bClippingExpRewardLimit; }
    void GetExpTable( OUT std::map< DWORD, __int64>& mapList ) { KLocker lock( m_csExpSpxInfo ); mapList = m_mapExpTable; }

    int GetLastDifficulty( IN const int& nModeID );

    float GetMaxAddRatio() { KLocker lock( m_csExpSpxInfo ); return m_fMaxAddRatio; };
    int GetMaxAddExp() { KLocker lock( m_csExpSpxInfo ); return m_nMaxAddExp; };

    void ControlFeedbackExpBoost( IN const KGameInfo& kGameInfo, IN OUT float& fExpBoost, IN int& nRank );

    int GetMaxRankLimit() { return m_nMaxRank; };

protected:
    bool GetQuestStageInfo( IN KLuaManager& kLuaMng, OUT sStageInfo& stageInfo );
    bool GetQuestReward( IN const int& nQuestID, IN const int& nStageCount, IN const int& nDifficult, OUT sStageInfo& stageInfo );
    bool GetMonsterHuntInfo( IN int nDifficult, OUT sMonsterHuntInfo& kInfo );
    bool GetMonsterCrusaderInfo( IN int nMonsterID, OUT sMonsterCrusaderInfo& sInfo );

    void InitResultIn( OUT KGameResultIn& kResultIn );

    void CalcExpGpBoostProcess( IN const KGameInfo& kGameInfo, IN OUT KEndGameReq& kReq );
    void RegistFunctions();

    void CalcDefaultExpGP(IN const KGameInfo& kGameInfo,  IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp, IN std::map<DWORD,int>& mapResultGP, IN OUT KEndGameReq& kReq );
    void CalcMatchExpGp(IN const KGameInfo& kGameInfo,  IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp, IN std::map<DWORD,int>& mapResultGP, IN OUT KEndGameReq& kReq );
    void CalcTagMatchExpGp(IN const KGameInfo& kGameInfo,  IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp, IN std::map<DWORD,int>& mapResultGP, IN OUT KEndGameReq& kReq );
    void CalcQuestExpGp(IN const KGameInfo& kGameInfo,  IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp, IN std::map<DWORD,int>& mapResultGP, IN OUT KEndGameReq& kReq );
    void CalcMonsterHuntExpGp(IN const KGameInfo& kGameInfo,  IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp, IN std::map<DWORD,int>& mapResultGP, IN OUT KEndGameReq& kReq );
    void CalcMonsterCrusaderExpGp(IN const KGameInfo& kGameInfo,  IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp, IN std::map<DWORD,int>& mapResultGP, IN OUT KEndGameReq& kReq );
    
    void CalcEmptyExpGp(IN const KGameInfo& kGameInfo,  IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp, IN std::map<DWORD,int>& mapResultGP, IN OUT KEndGameReq& kReq );
    void CalcQuestHeroExpGp(IN const KGameInfo& kGameInfo,  IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp, IN std::map<DWORD,int>& mapResultGP, IN OUT KEndGameReq& kReq );
    void CalcDotaExpGp(IN const KGameInfo& kGameInfo,  IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp, IN std::map<DWORD,int>& mapResultGP, IN OUT KEndGameReq& kReq );
    void CalcAngelEggExpGp(IN const KGameInfo& kGameInfo,  IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp, IN std::map<DWORD,int>& mapResultGP, IN OUT KEndGameReq& kReq );

    float FindReconcileValue( std::vector<sReconcile>& vecReconcile, float fValue );
    float GetRankBonus( IN const int nRankPoint, IN const int nGameMode );
    float GetWinRatio() { KLocker lock(m_csConfig); return m_sConfigData.m_fWinRatio; };
    float GetLoseRatio() { KLocker lock(m_csConfig); return m_sConfigData.m_fLoseRatio; };
    float GetExpBoost() { KLocker lock(m_csConfig); return m_sConfigData.m_fAdditionalEXPBoost; };
    float GetGpBoost() { KLocker lock(m_csConfig); return m_sConfigData.m_fAdditionalGPBoost; };

    float GetTestGPRatio() { KLocker lock(m_csConfig); return m_sConfigData.m_fTestGPRatio; };
    float GetTestEXPRatio() { KLocker lock(m_csConfig); return m_sConfigData.m_fTestEXPRatio; };
    float GetTestSPXRatio() { KLocker lock(m_csConfig); return m_sConfigData.m_fTestSPXRatio; };

    void AdjustCharaterLimit( KEndGameReq& kReq );
    float GetCharaterLimitExp( char cCharType );
    float GetCharaterLimitSpx( char cCharType );

    bool LoadBoostDetailInfo( IN KLuaManager& kLuaMng, OUT std::map< std::pair< char, char > ,sBoostDetailInfo >& mapBoostDetailInfo );
    void CheckCountBoost( DWORD dwUserUID, std::map< DWORD,DWORD >& mapGameCountEvent, float& fExpBoostRatio, float& fGpBoostRatio );

    //서버 기본 배율 계산
    void CalcBaseExpGpSpxRatio( IN OUT int& nExp, IN OUT int& nGP );
    float GetBaseExpRatio() { KLocker lock(m_csConfig); return m_sConfigData.m_fBaseEXPRatio; };
    float GetBaseGpRatio()  { KLocker lock(m_csConfig); return m_sConfigData.m_fBaseGPRatio; };
    float GetBaseSpxRatio() { KLocker lock(m_csConfig); return m_sConfigData.m_fBaseSPXRatio; };
    // 클리어 보너스
    float GetClearBonusRatio()  { KLocker lock(m_csConfig); return m_sConfigData.m_fClearBonusRatio; };
    float GetPvpGpRatio()       {  KLocker lock(m_csConfig); return m_sConfigData.m_fPvpGpRatio; };

    // win-win bug 체크.
    void ChkWinWinBug( IN const KGameInfo& kGameInfo, IN KEndGameReq& kReq );
    bool ChkWinWinSurv( IN const KGameInfo& kGameInfo, IN KEndGameReq& kReq, OUT std::vector< boost::tuple<std::wstring,bool,int> >& vecLog );
    bool ChkWinWinTeam( IN const KGameInfo& kGameInfo, IN KEndGameReq& kReq, OUT std::vector< boost::tuple<std::wstring,bool,int> >& vecLog );

    void GetClearBonus( OUT float& fClearExp, OUT float& fClearGp );
    bool LoadMonModifiedRatio( KLuaManager& kLuaMng );
    void GetMatchApplyLvDiffRatio( IN const DWORD dwWinCharLevel, IN const DWORD dwLoseCharLevel, OUT float& fExp );

    int GetEquipSetCount( IN const std::vector<KEquipItemInfo>& vecEquip, IN const std::vector<GCITEMID>& vecItemID );
    void CheckMonsterCrusaderClearBoost( IN const KGameInfo& kGameInfo, IN OUT float& fExpBoost, IN OUT float& fGpBoost );
    void CalcGuildBattleExpGp( IN const KGameInfo& kGameInfo_, IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp_, IN std::map<DWORD,int>& mapResultGP_, IN OUT KEndGameReq& kReq_ );
    float CalcGuildBattleWinningRate( const float RankPointA_, const float RankPointB_ );
    float CalcToAddRankingPoint( const float rankPointOfA_, const float K_, bool bWin_, float Ea_ );
    void CalcGuildPoint( IN OUT KEndGameReq& kReq_, IN const int benefitKind_ );
    int MakeIntegerBPoint( IN const DWORD BPoint_, IN const float K_, IN const bool bWin_, IN const float fWinningRate_ );

    float GetAngelEggWinRatio()  { KLocker lock(m_csConfig); return m_sConfigData.m_fAngelEggWinRatio; }
    float GetAngelEggLoseRatio() { KLocker lock(m_csConfig); return m_sConfigData.m_fAngelEggLoseRatio; }

    void GetGroupItemAll( IN const std::vector<KGameResultIn>& vecGameResult, IN const int& nModeID, OUT std::map<int,std::set<GCITEMID> >& mapGroupItemAll );
    void CheckInventoryBoostGroupItem( IN const std::map<int,std::set<GCITEMID> >& mapGroupItem,
                                       IN const std::map<int,std::set<GCITEMID> >& mapGroupItemAll,
                                       IN OUT std::map<GCITEMID,sBoostInfo>& mapBoostAllUser,
                                       IN OUT float& fExpBoost,
                                       IN OUT float& fGPBoost );
    void CalcELOExpGpRatio( IN OUT int& nExp, IN OUT int& nGP );
	void CheckEscortBoost( IN bool bIsEscortMonsterAlive_, IN DWORD dwEscortMonID_, IN int nGameMode_, IN int nDifficulty_, IN OUT float& fExpBoost_, IN OUT float& fGpBoost_ );
protected:
/*
    static const __int64    ms_biExpTable[]; // Exp/Lv 테이블
    static const DWORD      ms_dwGradeTable[]; // Grade Table
    static const int        ms_iGPGetTable[]; // Gp 보상 테이블
    static const int        ms_iExpGetTable[]; // Exp 보상테이블
    static const int        ms_iSpxGetTable[]; // Spx 보상테이블
    static const DWORD      ms_dwCharacterGrade[]; // 대만 홍콩에서 사용되는 캐릭터 그레이드.
    static const int        ms_iMiniGameGPTable[]; // 미니게임에서 얻는 GP 보상 테이블
*/
    // table 루아 파일로 분리
    mutable KncCriticalSection              m_csExpSpxInfo;
    std::map< DWORD, __int64>               m_mapExpTable; // Exp/Lv 테이블
    std::map< DWORD, DWORD >                m_mapGradeTable; // Grade Table
    std::map< DWORD, int >                  m_mapGPGetTable; // Gp 보상 테이블
    std::map< DWORD, int >                  m_mapExpGetTable; // Exp 보상테이블
    std::map< DWORD, int >                  m_mapSpxGetTable; // Spx 보상테이블
    std::map< DWORD, DWORD >                m_mapCharacterGrade; // 대만 홍콩에서 사용되는 캐릭터 그레이드.
    std::map< DWORD, int >                  m_mapMiniGameGPTable; // 미니게임에서 얻는 GP 보상 테이블


    std::map< int, Handler >                m_mapFunction; // 서버 시작 시에 한번 생성하고 그 이후 참조만 한다.

    mutable KncCriticalSection              m_csQuestExpGp;
    std::map< int, sQuestInfo >             m_mapQuestExpGp; // < QuestID, QuestInfo >

    mutable KncCriticalSection              m_csBoostInfo;
    std::map< GCITEMID, sBoostInfo >        m_mapBoostInfo;

    mutable KncCriticalSection              m_csMonsterHunt;
    std::map< int, sMonsterHuntInfo >       m_mapMonsterHunt;

    mutable KncCriticalSection              m_csMonterInfo;
    std::map< int, sMonsterCrusaderInfo >   m_mapMonterInfo;

    mutable KncCriticalSection              m_csReconcileInfo;
    std::vector<sReconcile>                 m_vecPartyBonus;
    std::vector<sReconcile>                 m_vecLvReconcile;

    mutable KncCriticalSection              m_csRankPoint;
    std::vector<sReconcile>                 m_vecRankPoint;             // 던전일반모드 랭크 보너스
    std::vector<sReconcile>                 m_vecHellRankPoint;         // 헬모드 랭크 보너스.
    float                                   m_fPerfectBonus;            // 퍼펙트이벤트 보너스 확률
    std::set<int>                           m_setPerfectEventGameMode;  // 퍼펙트이벤트 적용대상 게임모드

    mutable KncCriticalSection              m_csDefaultReward;
    std::map<int, sDefaultReward>           m_mapDefaultReward;

    mutable KncCriticalSection              m_csPvPEvent;
    std::vector<sPVPReward>                 m_vecPvPEvent;

    mutable KncCriticalSection              m_csConfig;
    SConfigData                             m_sConfigData;

    mutable KncCriticalSection              m_csCharLimit;
    std::map<char, std::pair<float,float> > m_mapCharLimit; // [char, [exp,spx] ]
    __int64                                 m_biExpRewardLimit;
    bool                                    m_bClippingExpRewardLimit;

    mutable KncCriticalSection              m_csClearBonus;
    float                                   m_fClearExp;
    float                                   m_fClearGp;
    float                                   m_fClearSpx;

    DWORD                                   m_dwMaxLevel; // 캐릭터 최대 MAX LEVEL
                                                          // 기존 코드에 박혀있던걸 루아파일로 분리.
    float                                   m_fMaxAddRatio; // 최대 추가 확률.
    int                                     m_nMaxAddExp;   // 최대 추가 경험치.

    // 레벨차 보정 로직 추가.
    mutable KncCriticalSection              m_csModifiedRatio;
    int                                     m_nModifiedMinLevel; // 보정 MinLevel.
    int                                     m_nModifiedMaxLevel; // 보정 MaxLevel.
    float                                   m_fDefaultReward; // 보정이 안될경우 기본 지급 경험치.
    std::map< int, float >                  m_mapMonModifiedRatio; // 몬스터 레벨차 보정 Table.

    mutable KncCriticalSection              m_csControlFeedback;
    std::map< int, float >                  m_mapControlFeedbackExpBonusRatio; // 컨트롤 피드백 랭크에 따른 경험치 보너스
    int                                     m_nMaxRank; // 랭크 유효 최대 값
};

DefSingletonInline( KResultManager );
DeclOstmOperatorA( KResultManager );
