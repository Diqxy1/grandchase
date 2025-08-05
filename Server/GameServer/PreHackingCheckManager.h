#pragma once
#include <map>
#include <vector>
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include "UserPacket.h"

struct KMiniGameScore
{
    std::pair<DWORD,DWORD>  m_prTimeScope;
    float                   m_fScore;
};

class KLuaManager;

class KPreHackingCheckManager
{
    DeclareSingleton( KPreHackingCheckManager );
    NiDeclareRootRTTI( KPreHackingCheckManager );
    DeclToStringW;

public:
    enum HACK_TYPE
    {
        HT_CHECKSUM             = 1,    // 체크섬 오류
        HT_PROTECTED            = 2,    // 프로텍티드 타입
        HT_TABLE_CHECKSUM       = 3,    // 프로텍티드 타입 체크섬
        HT_CLIENT_HACKING       = 4,    // 클라이언트 변조
        HT_DUNGEON_MONSTER_HACKING = 5,    // 던전 몬스터 해킹
        HT_MAX,
    };

    enum GAME_HACK_TYPE
    {
        GHT_NORMAL              = 0,    // 정상
        GHT_MINIGAME_TIME_SCORE = 1,    // 미니게임 시간대비 점수대 이상
        GHT_DUNGEON_STAGE_COUNT = 2,    // 던전 최소 스테이지수보다 적음
        GHT_DUNGEON_CLEAR_TIME  = 3,    // 던전 최단 클리어시간보다 ?음
        GHT_ATK_HACK            = 4,    // 공격력 해킹
        GHT_INC_ATK_RATE_HACK   = 5,    // 공격력증가율 해킹
        GHT_CLIENT_ALL_KILL     = 6,    // 클라이언트 올킬(방장이 범인)
        GHT_CLIENT_MON_HP       = 7,    // 클라이언트 몬스터HP해킹(방장이 범인)
        GHT_CLIENT_PARTY_USER   = 8,    // 클라이언트 해킹 플레이랑 같이 플레이한 파티원
        GHT_MONSTER_TEMPLATE    = 9,    // 몬스터템플릿 해킹
        GHT_CHARACTER_STAT      = 10,   // 캐릭터 스텟 해킹
        GHT_DAMAGE_INFO         = 11,   // 데미지 정보 해킹
        GHT_PVP_CHECK           = 12,   // 대전 해킹
        GHT_DUNGEON_MONSTER_HACK = 13,  // 던전 몬스터 해킹
        GHT_INFINITY_DUNGEON_MONSTER_HACK = 14,  // 무한던전 몬스터 해킹
        GHT_MAX,
    };

    enum OPTION
    {
        OPT_CHECK_ENABLE        = 0,    // 체크여부
        OPT_WRITE_DB            = 1,    // DB기록 여부
        OPT_WRITE_FILE          = 2,    // File기록 여부
        OPT_NOTICE_MSG          = 3,    // 클라이언트 공지 여부
        OPT_INITIALIZE          = 4,    // 초기화 여부
    };

    KPreHackingCheckManager(void);
    ~KPreHackingCheckManager(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );

    // OptionCheck
    bool CheckOption( IN const int nID_, IN const int nOptionID_ );
    // MiniGame
    int CheckMiniGameScore( IN const DWORD dwPlayTime_, IN const DWORD dwScore_ );
    // Dungeon
    int CheckMinStageCheck( IN const int nDungeonID_, IN const int nDifficult_, IN const int nStageCount_ );
    int CheckClearTimeCheck( IN const int nDungeonID_, IN const int nDifficult_, IN const DWORD dwClearTime_ );
    // Abuse
    bool GetLogShortGame()          { KLocker lock( m_csGameCheck ); return m_bLogShortGame; }
    bool GetIgnoreShortGame()       { KLocker lock( m_csGameCheck ); return m_bIgnoreShortGame; }
    DWORD GetThresholdTick()        { KLocker lock( m_csGameCheck ); return m_dwThresholdTick; }
    DWORD GetDotaThresholdTick()    { KLocker lock( m_csGameCheck ); return m_dwDotaThresholdTick; }
    // Dump
    void DumpMinigameInfo();
    void DumpDungeonInfo();
    void DumpHackingProtectionMode();
    void DumpCharacterStatTable();
    // Ability
    int CheckMaxAtkLimit( IN const char cLevel_, IN const std::vector<float>& vecAtkData_ );
    int CheckMaxIncreaseAtkRate( IN const std::vector<float>& vecIncAPRatio_ );
    float GetMaxAtkLimit( IN const char cLevel_ );
    float GetMaxIncAPRatio()        { KLocker lock( m_csAbility ); return m_fMaxIncAPRatio; }
    // Character Stat
    bool GetCharacterStat( IN std::pair<char,char> prKey_, OUT KSimpleCharacterStat& kData_ );
    // PVP
    bool PvpLifeCountCheck( IN const int& nMapID_, OUT USHORT& usLifeCount_ );
    bool IsPvpErrDisconnect() { KLocker lock( m_csPvpLife ); return m_bPvpErrDisconnect; }
    bool GetPvpLvMaxExp( IN const int& nLv_, OUT __int64& biMaxExp_ );
    bool DungeonMonsterCountCheck( IN const int& nModeID, OUT int& nMonsterCnt );
    bool IsMonsterCntErrDisconnect() { KLocker lock( m_csDungeonMonsterCnt ); return m_bDungeonMonErrDisconnect; }

    bool IsInfinityDungeonMonsterHackErrDisconnect();
    bool IsInfinityDungeonMonsterLimitOver( IN const DWORD dwSummonGap_, IN const DWORD dwSummonMonsterCount_ );

private:
    bool LoadOption( KLuaManager& kLuaMng_ );
    bool LoadMinigameCheckData( KLuaManager& kLuaMng_ );
    bool LoadDungeonCheckData( KLuaManager& kLuaMng_ );
    bool LoadAbuseCheckData( KLuaManager& kLuaMng_ );
    bool LoadAbilityCheckData( KLuaManager& kLuaMng_ );
    bool LoadCharacterStatTable( KLuaManager& kLuaMng_ );
    bool LoadPvpLifeTable( KLuaManager& kLuaMng_ );
    bool LoadDungeonMonsterCountTable( KLuaManager& kLuaMng_ );
    bool LoadInfinityDungeonMonsterHackCheck( IN OUT KLuaManager& kLuaMng_ );
    void PrintCheckOption( std::wostream& stm_ ) const;

protected:
    mutable KncCriticalSection                              m_csCheckOption;
    std::map< int, std::map< int, bool > >                  m_mapCheckOption;       // 해킹체크별 옵션

    mutable KncCriticalSection                              m_csMinigameCheck;
    std::vector<KMiniGameScore>                             m_vecMinigameCheckTable;// < < TimeScope Start,End>, Score >

    mutable KncCriticalSection                              m_csPreHackCheck;
    std::map< std::pair<int,int>, std::pair<int,DWORD> >    m_mapDungeonCheckData;  // <pair<DungeonID,Difficult>, pair<MinStage,MinClearTime>>

    mutable KncCriticalSection                              m_csGameCheck;
    bool                                                    m_bLogShortGame;
    bool                                                    m_bIgnoreShortGame;
    DWORD                                                   m_dwThresholdTick;
    DWORD                                                   m_dwDotaThresholdTick; // 점령전 별도의 ShortGame 체크

    mutable KncCriticalSection                              m_csAbility;
    std::map< char, float >                                 m_mapMaxAtkLimit;       // 레벨별 최대 공격력 <레벨,공격력>
    float                                                   m_fMaxIncAPRatio;       // 최대 공격력 증가 비율값

    mutable KncCriticalSection                              m_csCharacterStat;
    std::map< std::pair<char,char>, KSimpleCharacterStat >  m_mapCharacterStatTable; // 캐릭터스텟 테이블 map< key(캐릭터ID,전직), 스텟정보 >

    mutable KncCriticalSection                              m_csPvpLife;
    bool                                                    m_bPvpErrDisconnect; // 목숨수 에러에 걸린 경우, 접속 끊을지 여부
    std::map<int,USHORT>                                    m_mapPvpLifeTable; // 대전맵별 목숨수 map[ MapID, LifeCount ]
    std::map<int,__int64>                                   m_mapPvpLvMaxExp; // 대전시, 레벨별 획득가능한 최대 경험치 map[ CharLv, MaxExp ]

    mutable KncCriticalSection                              m_csDungeonMonsterCnt;
    bool                                                    m_bDungeonMonErrDisconnect; // 던전 몬스터 숫자 접속 끊을지 여부 확인
    std::map< int, int >                                    m_mapDungeonMonsterCount; // 던전 몬스터 숫자

    mutable KncCriticalSection                              m_csInfinityDungeonMonsterHack;
    bool                                                    m_bInfinityDungeonMonsterHackErrDisconnect; // 무한던전 몬스터 해킹으로 접속 끊을지 여부 확인
    DWORD                                                   m_dwInfinityDungeonMinRoundTerm_Sec; // 무한던전 몬스터 최소 소환 주기(초)
    DWORD                                                   m_dwInfinityDungeonMaxSummonCount; // 무한던전 몬스터 최대 소환 몬스터 수
};

DefSingletonInline( KPreHackingCheckManager );
DeclOstmOperatorA( KPreHackingCheckManager );