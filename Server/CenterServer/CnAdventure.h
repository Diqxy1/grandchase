#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>

class KLuaManager;
class KncTimer;

class KCnAdventure
{
    DeclareSingleton( KCnAdventure );
    NiDeclareRootRTTI( KCnAdventure );
    DeclToStringW;

public:
    KCnAdventure(void);
    ~KCnAdventure(void);

    bool LoadScript();

    void Tick();
    bool SetAdventureEventDate( IN const int nVersion_, IN const time_t tmStartDate_, IN const time_t tmEndDate_,
                                IN const std::map< int, KAdventureEventDate >& mapAdventureEventDate_ );
    void GetAdventureData( OUT KECN_ADVENTURE_DATA_NOT& kPacket_ );

protected:
    bool LoadDropItemInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT PAIR_INT_DWORD& prDropItemInfo_ );
    bool LoadStrings( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< DWORD >& vecDisplayString_ );
    bool LoadMonsterInfo( IN KLuaManager& kLuaMng_, std::string& strTable_, OUT std::map< PAIR_DWORD, KAdventureMonsterInfo >& mapAdventureMonsterInfo_ );
    bool LoadContinentRewardInfo( IN KLuaManager& kLuaMng_, std::string& strTable_, OUT std::map< int, KContinentRewardInfo >& mapContinentRewardInfo_ );
    bool LoadCollectItems( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< DWORD, KCollectItem >& mapCollectItems_ );
    bool LoadRewards( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KDropItemInfo >& vecContinentRewards_ );
    bool LoadFinalRewardInfo( IN KLuaManager& kLuaMng_, std::string& strTable_, OUT KDropItemInfo& kFinalRewardInfo_ );

protected:
    mutable KncCriticalSection  m_csAdventure;

    // 스크립트 정보
    std::map< PAIR_DWORD, KAdventureMonsterInfo >   m_mapAdventureMonsterInfo;     // 몬스터 정보;     key: pair< m_dwDungeonID, m_dwMonsterID >
    std::map< int, KContinentRewardInfo >           m_mapContinentRewardInfo;      // 대륙 보상 정보;  key: m_nContinentID
    KDropItemInfo                                   m_kFinalRewardInfo;            // 최종 보상 정보;

    // DB에서 가져온 이벤트 기간 정보.. 요 정보로 게임 서버가 이벤트 ON/OFF를 판단함
    std::map< int, KAdventureEventDate >            m_mapAdventureEventDate;       // 모험 이벤트 기간 정보; key: EventID(발동 세부 기간; 게임서버는 요 데이터만 유지하면 된다)

    int                                             m_nVersion;                    // 현재 발동중인 이벤트 버전
    time_t                                          m_tmStartDate;                 // 이벤트 시작일(전체 기간)
    time_t                                          m_tmEndDate;                   // 이벤트 종료일(전체 기간)

    bool                                            m_bChanged;                    // 이벤트 변경사항이 있으면 true

    KncTimer                                        m_kUpdateCheckTimer;           // 이벤트 변동이 있는지 확인할 타이머(1분)
};

DefSingletonInline( KCnAdventure );
DeclOstmOperatorA( KCnAdventure );