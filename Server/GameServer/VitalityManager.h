#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <set>
#include <vector>
#include "Lua/KLuaManager.h"
#include "CommonPacket.h"
#include "UserPacket.h"
#include "atltime.h"
#include <boost/random.hpp>

class KncTimer;

class KVitalityManager
{
    DeclareSingleton( KVitalityManager );
    NiDeclareRootRTTI( KVitalityManager );
    DeclToStringW;

public:
    enum EVitalityUpdateType {
        EVUT_LOGOUT         = 0, // 유저 로그아웃시 활력 정보 저장(일반적인 상황, 모든 캐릭터 정보 기록)
        EVUT_BEFORE_INIT    = 1, // 활력 정보 초기화 이전 잔여 활력 정보 저장(로그성 기록, 모든 캐릭터 정보 기록)
        EVUT_INIT           = 2, // 활력 정보 초기화 후 잔여 활력 정보 저장
        EVUT_RECHARGE       = 3, // 활력 충전시 저장(로그성 기록, 해당 캐릭터 정보 기록)
    };

    enum EVitalitySendType {
        EVST_DUNGEON_START  = 0, // 던전 시작시 활력이 차감된 이후 잔여 활력 정보 전송
        EVST_CREATE_CHAR    = 1, // 캐릭터 생성시 해당 캐릭터의 활력이 충전된 이후 잔여 활력 정보 전송
    };

    KVitalityManager( void );
    ~KVitalityManager( void );

public:
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsRun();
    int GetVitalityForFirstCharacterCreation();
    int GetVitalityForCharacterCreation();
    int GetMaxVitalityPerDay();
    int GetVitalityPerRechargePoint();
    bool IsVitalityCheckDungeon( IN const int nGameMode_, const IN int nDifficulty_ );
    bool GetDeductVitality( IN const int nGameMode_, const IN int nDifficulty_, OUT int& nDeductVitality_ );
    int GetExpGainRatioNoVitality( IN const int nGameMode_, const IN int nDifficulty_ );
    int GetGpGainRatioNoVitality( IN const int nGameMode_, const IN int nDifficulty_ );
    int GetItemDropRatioNoVitality( IN const int nGameMode_, const IN int nDifficulty_ );
    void Tick();
    void UseVitalitySystem( IN bool bFlag_ );
    void GetVitalityResetSpanTime( OUT CTimeSpan& kVitalityResetSpanTime_ );
    void ApplyNoVitalityRatioToItemDropList( IN const KGameInfo& kGameInfo_, OUT std::vector< KGameDropElement >& vecMonsterDrop_ );
    void GetVitalityCheckDungeonInfoNot( OUT KEVENT_VITALITY_CHECK_DUNGEON_INFO_NOT& kPacket_ );

private:
    bool LoadVitalityCheckDungeon( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< PAIR_INT, KVitalityCheckDungeon >& mapVitalityCheckDungeons_ );
    bool LoadRatioNoVitality( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT KVitalityCheckDungeon& kVitalityCheckDungeon_ );
    void CalcNextInitializeTime();

    float GetRatio() { return GetRandomNum() / static_cast< float >( UINT_MAX ); };
    DWORD GetRandomNum() { return die(); };

protected:
    mutable KncCriticalSection                  m_csVitality;

    bool                                        m_bUseVitalitySystem;                   // 활력(피로도) 시스템 기능 On/Off 여부(true: On, false: Off)
    KncTimer                                    m_kOneMinutesTimer;                     // 1분 틱 타이머
    time_t                                      m_tmNextInitialize;                     // 다음 활력 초기화 시간

    int                                         m_nVitalityForFirstCharacterCreation;   // 최초 캐릭터 생성시 지급하는 기본활력 수치
    int                                         m_nVitalityForCharacterCreation;        // 최초를 제외한 나머지 캐릭터 생성시 지급하는 기본활력 수치
    int                                         m_nMaxVitalityPerDay;                   // 캐릭별 하루 최대 활력 수치
    int                                         m_nVitalityPerRechargePoint;            // 활력 충전 포인트 1을 얻기 위해 소모해야 할 활력 수치
    CTimeSpan                                   m_kVitalityResetSpanTime;               // 활력을 초기화 할 시간. 스크립트에서 설정받지 않는다
    std::map< PAIR_INT, KVitalityCheckDungeon > m_mapVitalityCheckDungeons;             // 활력을 소모할 던전 정보(Key: ModeID, Difficulty)

    boost::lagged_fibonacci607 rng;
    boost::uniform_int< DWORD > uint32;
    boost::variate_generator< boost::lagged_fibonacci607&, boost::uniform_int< DWORD > > die;
};

DefSingletonInline( KVitalityManager );
DeclOstmOperatorA( KVitalityManager );

class KVitalityUserData
{ 
public:
    KVitalityUserData();
    ~KVitalityUserData();
    void GetVitalityUserData( OUT KVitalityUpdateData& kVitalityUpdateData_ );
    bool DecreaseVitalityOnPlay( IN const int nCharType_, IN const int nVitalityDecreaseAmount_ );
    int GetVitality( IN const int nCharType_ );
    bool SetVitality( IN const int nCharType_, IN const int nVitality_ );
    bool IsRemainVitality( IN const int nCharType_ );
    bool RechargeVitality( IN const int nCharType_ );

public:
    int                                     m_nRechargePoint;               // 활력 충전 포인트
    int                                     m_nRechargePointCounter;        // 활력 충전용 소모한 활력 카운터
    std::map< int, int >                    m_mapRemainVitality;            // 캐릭터별 잔여 활력
};
