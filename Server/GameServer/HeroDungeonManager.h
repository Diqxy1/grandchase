#pragma once
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include "KncTimer.h"
#include <boost/array.hpp>
#include "Lottery.h"
#include "UserPacket.h"
#include "ManufactureS3.h"
#include "HeroItemUpgradeManager.h"

// 알람 시스템
// 작업 예정

class KLuaManager;
class KHeroDungeonManager
{
    DeclareSingleton( KHeroDungeonManager );
    NiDeclareRootRTTI( KHeroDungeonManager );
    DeclToStringW;
public:

    KHeroDungeonManager(void);
    ~KHeroDungeonManager(void);

    void Tick();

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void DumpItemList();
    void DumpDungeonInfo( IN const int nModeID );

    bool IsActiveMode() { KLocker lock(m_csHeroDungeon); return m_bActiveMode; }
    bool IsHeroDungeon( IN const int nModeID );
    bool GetHeroDungeonInfo( IN const int nModeID, OUT KHeroDungeon& kInfo );
    void GetHeroDungeonList( OUT std::vector<KHeroDungeon>& vecList );
    void GetPointItem( OUT KDropItemInfo& kItem );
    time_t GetResetTime() { KLocker lock(m_csHeroDungeon); return m_tmResetTime; }

    float GetMonsterExpRatio()     { KLocker lock(m_csHeroDungeon); return m_fMonsterExpRatio; }
    float GetDungeonWinClearExpRatio() { KLocker lock(m_csHeroDungeon); return m_fDungeonWinClearExpRatio; }
    float GetDungeonLoseClearExpRatio() { KLocker lock(m_csHeroDungeon); return m_fDungeonLoseClearExpRatio; }
    DWORD GetHeroDungeonMinLv( IN int const nModeID );
    void GetHeroDungeonIDList( OUT std::set<int>& setModeID );
    bool IsHeroDungeonDifficultyCheck( IN const int nModeID );
    bool SendHeroDungeonInfoNot();

private:
    bool LoadHeroDungeon( OUT std::string& strScript );
    bool LoadAlarmTime( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT std::vector<int>& vecAlarm );
    bool LoadItemInfo( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT KDropItemInfo& kItem );
    bool LoadTime( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT time_t& tmResetTime );
    bool LoadTimeTable( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT std::map<USHORT,std::pair<time_t,time_t> >& mapTime );
    bool LoadItemList( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT std::map<INDEX_GCITEMID,KManufactureItem>& mapList );

    void DungeonAlarmNotice();
    void CheckAlarmTime( IN const int nModeID, IN const time_t& tmCurrent, IN const std::pair<time_t,time_t>& prOpenTime );
    void SendHeroDungeonAlarmNot( IN const USHORT usType, IN const int nModeID, IN const int nMin, IN const std::pair<time_t,time_t>& prOpenTime );

public:
    KManufactureS3                  m_kHeroPointItems;  // 영웅포인트 아이템 구매 정보
	KHeroItemUpgradeManager			m_kHeroItemUpgradeManager; // 영웅 아이템 업그레이드 정보

protected:
    KncTimer                        m_kTickCheck;

    mutable KncCriticalSection      m_csHeroDungeon;
    bool                            m_bActiveMode;          // 전체 동작여부
    KDropItemInfo                   m_kPointItem;           // 영웅던전 포인트 아이템
    time_t                          m_tmResetTime;          // 입장 초기화 시간
    std::vector<KHeroDungeon>       m_vecDungeonInfo;       // 영웅 던전 정보
    std::vector<int>                m_vecOpenAlarm;         // 오픈전 알람메세지 설정(분)
    std::vector<int>                m_vecCloseAlarm;        // 클로즈전 알람메세지 설정(분)

    float                           m_fMonsterExpRatio;    // 몬스터 경험치 나누기배율.
    float                           m_fDungeonWinClearExpRatio; // 던전클리어 성공시 경험치 연산배율.
    float                           m_fDungeonLoseClearExpRatio; // 던전클리어 실패시 경험치 연산배율.
};

DefSingletonInline( KHeroDungeonManager );
DeclOstmOperatorA( KHeroDungeonManager );