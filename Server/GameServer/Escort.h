#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <vector>
#include "Lua/KLuaManager.h"
#include "CommonPacket.h"
#include "Kairo.h"
#include "UserPacket.h"

class KEscort
{
    DeclareSingleton( KEscort );
    NiDeclareRootRTTI( KEscort );
    DeclToStringW;

public:
    KEscort( void );
    ~KEscort( void );

public:
    struct KEscortMonAppearChance {
        DWORD                   m_dwMonsterID;
        float                   m_fAppearChance;
    };
    struct KEscortMapInfo
    {
        PAIR_DWORD              m_prDungeonInfo;
        PAIR_DWORD              m_prLevelInfo;
        std::vector< KEscortMonAppearChance > m_vecAppearChances;
    };
    struct KEscortReward
    {
        float                   m_fExtraExp;
        float                   m_fExtraGp;
        std::vector< Kairo >    m_vecKairoClearReward;
        std::vector< Kairo >    m_vecKairoExtraReward;
    };
    struct KEscortRewardInfo
    {
        DWORD                   m_dwMonsterID;
        KEscortReward           m_kAliveReward;
        KEscortReward           m_kDeadReward;
    };

public:
    bool IsRun();
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool GetEscortInfoAll( OUT KEVENT_ESCORT_INFO_NOT& kPacket_ );
    bool GetEscortRewardInfo( IN DWORD dwMonID_, OUT KEscortRewardInfo& kEscortRewardInfo_ );   // 보상정보 가져오는 함수
    bool IsValidUserLevel( IN PAIR_DWORD& prKey_, IN DWORD dwlevel_ );
    bool IsHaveRewardInfo( IN DWORD dwGameModeID_, IN DWORD dwDifficulty_, IN DWORD dwMonID_ );
    bool GetAliveReward( IN DWORD dwMonID_, OUT KEscortReward& kAliveReward_ );
    bool GetDeadReward( IN DWORD dwMonID_, OUT KEscortReward& kDeadReward_ );
    bool IsHaveEscortInfo( IN PAIR_DWORD& prKey_, OUT DWORD& dwEscortMonID_ );  // 해당 던전에 이벤트 정보가 있는지 확인. true: 호위 이벤트 진행중, true일 경우 등장한 몬스터 ID 세팅
    bool IsEscortMonster( IN const DWORD dwMonID_ );
    void DebugPrint();

private:
    bool LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ );
    bool LoadEscortInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< PAIR_DWORD, KEscortMapInfo >& mapEscortInfo_ );
    bool LoadEscortMonsterInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< DWORD, KEscortInfo >& mapEscortMonsterInfo_ );
    bool LoadEscortRewardInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< DWORD, KEscortRewardInfo >& mapEscortRewardInfo_ );
    bool LoadRewardInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT KEscortReward& kRewardInfo_ );
    bool LoadRewardList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< Kairo >& vecKairo_ );
    bool LoadEscortMonAppearChanceInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KEscortMonAppearChance >& vecAppearChances_ );
protected:
    mutable KncCriticalSection  m_csEscort;

    int                         m_nVersion;     // 이벤트 버전.
    time_t                      m_tmBegin;      // 이벤트 시작 시간 스크립트에서 읽음
    time_t                      m_tmEnd;        // 이벤트 종료 시간 스크립트에서 읽음

    std::map< PAIR_DWORD, KEscortMapInfo >      m_mapEscortMapInfo;        // (DungeonID, Difficulty), KEscortMapInfo
    std::map< DWORD, KEscortInfo >              m_mapEscortMonsterInfo; // 몬스터 정보(ID, Health, TakeDamage, InvincibleTime, potionID, announce strings vector)
    std::map< DWORD, KEscortRewardInfo >        m_mapEscortRewardInfo;  // MonID, 보상정보
};

DefSingletonInline( KEscort );
DeclOstmOperatorA( KEscort );
