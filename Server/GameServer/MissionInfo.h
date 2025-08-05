#pragma once
#include "UserPacket.h"
#include <string>
#include <ToString.h>

class KRoom;
struct sCondition
{
    sCondition();
    sCondition( const sCondition& right ) { *this = right; };
    ~sCondition(){};
    sCondition& operator=( const sCondition& right )
    {
        m_nID               = right.m_nID;
        m_nSolo             = right.m_nSolo;
        m_nMonsterKill      = right.m_nMonsterKill;
        m_nModeID           = right.m_nModeID;
        m_nStageID          = right.m_nStageID;
        m_prDifficultRage   = right.m_prDifficultRage;
        m_nMapID            = right.m_nMapID;
        m_bWin              = right.m_bWin;
        m_nElapsedTime      = right.m_nElapsedTime;
        m_nKill             = right.m_nKill;
        m_nDie              = right.m_nDie;
        m_nExLife           = right.m_nExLife;
        m_fRatio            = right.m_fRatio;
        m_prMonLevelRange   = right.m_prMonLevelRange;
        m_nType             = right.m_nType;
        m_nValue            = right.m_nValue;
        m_prValue           = right.m_prValue;

        return *this;
    }

    // 해당 타입들은 중복으로 적용안된다를 기획서에 명시되어 있어서 enum으로 구분한다.
    enum
    {
        TYPE_NOUSE       = -1, // 사용안함
        TYPE_RANK        = 0,  // 랭크정보
        TYPE_USE_POTION  = 1,  // 포션 사용횟수 제한
        TYPE_BEATTACK    = 2,  // 피격 횟수( 몬스터에서 맞은 횟수 )
        TYPE_COMBO       = 3,  // 콤보 횟수
        TYPE_BACK_ATTACK = 4,  // 백어택 횟수
        TYPE_SKILL_COUNT = 5,  // 스킬 사용제한
    };

    enum
    {
        RANK_GRADE_C     = -1,
    };

    int                     m_nID;
    int                     m_nSolo;
    int                     m_nMonsterKill;     // 몬스터 ID
    int                     m_nModeID;
    int                     m_nStageID;
    std::pair<int,int>      m_prDifficultRage;  // 난이도 범위
    int                     m_nMapID;
    bool                    m_bWin;
    int                     m_nElapsedTime;     // sec
    int                     m_nKill;
    int                     m_nDie;
    int                     m_nExLife;
    float                   m_fRatio;
    std::pair<int,int>      m_prMonLevelRange;
    int                     m_nType;            // 조건 타입.
    int                     m_nValue;           // 랭크, 피격, 콤보, 백어택.
    std::pair<int,int>      m_prValue;          // 포션, 스킬.
};

class KSubMission
{
    DeclToStringW;

public:
    KSubMission();
    KSubMission( const KSubMission& right ) { *this = right; };
    ~KSubMission(){};

    KSubMission& operator=( const KSubMission& right )
    {
        m_dwSubmissionID    = right.m_dwSubmissionID;
        m_setConditions     = right.m_setConditions;
        m_sActionInfo       = right.m_sActionInfo;
        m_sCompletionInfo   = right.m_sCompletionInfo;

        return *this;
    };

    struct sAction
    {
        bool    m_bMonDrop;
        int     m_nItemID;
        int     m_nCount;
    };

    struct sCompletion
    {
        int     m_nItemID;
        int     m_nCount;
        int     m_nCharLv;
    };

protected:
    DWORD                   m_dwSubmissionID;
    std::set<int>           m_setConditions;
    sAction                 m_sActionInfo;
    sCompletion             m_sCompletionInfo;

    friend class KMissionManager;
    friend class KCollectionManager;
};

struct KMissionReward
{
    GCITEMID    m_ItemID;
    int         m_nCount;
    int         m_nPeriod;
    int         m_nGradeID; // 보상아이템 등급
    bool        m_bIsLook; // 코디아이템 여부

    KMissionReward():m_ItemID(0),m_nCount(0),m_nPeriod(0),m_nGradeID(-1),m_bIsLook(false){}
};

struct KMissionInfo
{
    DeclToStringW;

    KMissionInfo();
    KMissionInfo(const KMissionInfo& right) { *this = right; }
    virtual ~KMissionInfo();

    KMissionInfo& operator=(const KMissionInfo& right)
    {
        m_dwMissionID           = right.m_dwMissionID;
        m_cCharType             = right.m_cCharType;
        m_cJobLv                = right.m_cJobLv;
        m_nPeriod               = right.m_nPeriod;
        m_prLevelRage           = right.m_prLevelRage;
        m_nCorrectLv            = right.m_nCorrectLv;
        m_vecInvalidMission     = right.m_vecInvalidMission;
        m_vecInvalidComplete    = right.m_vecInvalidComplete;
        m_bUnique               = right.m_bUnique;
        m_vecSubmission         = right.m_vecSubmission;
        m_cNewChar              = right.m_cNewChar;
        m_cChangeJob            = right.m_cChangeJob;
        m_nGp                   = right.m_nGp;
        m_nExp                  = right.m_nExp;
        m_vecRewardItem         = right.m_vecRewardItem;
        m_vecSelectItem         = right.m_vecSelectItem;
        m_dwChainMissionID      = right.m_dwChainMissionID;
        m_cMissionType          = right.m_cMissionType;
        m_bEnableDelete         = right.m_bEnableDelete;
        m_nDungeonID            = right.m_nDungeonID;
        return *this;
    }

    enum
    {
        MISSION_TYPE_ALL        = 0,
        MISSION_TYPE_NORMAL     = 1,
        MISSION_TYPE_CHARACTER  = 2,
        MISSION_TYPE_EVENT      = 3,
        MISSION_TYPE_EPIC       = 4,
    };

    enum 
    {
        DEL_TYPE_EXPIRY_DEL     = 1,
        DEL_TYPE_USER_DEL       = 2,
        DEL_TYPE_USER_COMPLETE  = 3,
    };

    DWORD                               m_dwMissionID;          // 미션 카드 아이디와 동일
    char                                m_cCharType;
    char                                m_cJobLv;
    int                                 m_nPeriod;
    std::pair<int,int>                  m_prLevelRage;
    int                                 m_nCorrectLv;
    std::vector<DWORD>                  m_vecInvalidMission; // 나의 등록 정보를 확인한다.
    std::vector<DWORD>                  m_vecInvalidComplete; // 나의 완료 정보를 확인한다.
    bool                                m_bUnique;
    std::vector<int>                    m_vecSubmission;

    char                                m_cNewChar;
    char                                m_cChangeJob;
    int                                 m_nGp;
    int                                 m_nExp;
    std::vector<KMissionReward>         m_vecRewardItem;
    std::vector<KMissionReward>         m_vecSelectItem;
    DWORD                               m_dwChainMissionID;
    
    char                                m_cMissionType;       //미션의 타입
    bool                                m_bEnableDelete;      // 미션 삭제 가능 여부

    int                                 m_nDungeonID;         // 미션의 종속 던전

    bool CheckValidation();
	bool IsInSelection( GCITEMID itemID );
};
