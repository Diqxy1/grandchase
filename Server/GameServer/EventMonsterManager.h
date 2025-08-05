#pragma once
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include "KncTimer.h"
#include <boost/array.hpp>
#include "CommonPacket.h"
#define MAX_DIFFICULTY 255

struct KEventChildMonInfo
{
    int                         m_nChildMonsterID;
    std::vector<std::pair<KDropItemInfo,float> > m_vecChildReward; // 드랍아이템, 드랍확률
};

struct KEventMonInfo
{
    int                         m_nMonsterID;
    bool                        m_bAutoLevel;
    std::vector<std::pair<KDropItemInfo,float> > m_vecReward; // 드랍아이템, 드랍확률
    std::map< int , float > m_mapAppearRate; // 던전 난이도별 등장 확률.
    //boost::array<float,MAX_DIFFICULTY> m_pAppearRate; // 던전난이도별 등장확률
    time_t                      m_tmBegin;
    time_t                      m_tmEnd;
    bool                        m_bIsBoss;
    int                         m_nSpecial;
    std::set<int>               m_setModeID;
    std::map<int,std::vector<int> > m_mapModeStage; // ModeID, StageID
    std::vector<KEventChildMonInfo> m_vecChildMonster;
    std::vector<std::pair<KDropItemInfo,float> > m_vecPartyReward; // 드랍아이템, 드랍확률
};

struct KDungeonLevel
{
    int     m_nLowLv;
    int     m_nHighLv;
};

class KLuaManager;
class KEventMonsterManager
{
    DeclareSingleton( KEventMonsterManager );
    NiDeclareRootRTTI( KEventMonsterManager );
    DeclToStringW;
public:

    KEventMonsterManager(void);
    ~KEventMonsterManager(void);

    void Tick();
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );

    bool IsActiveEvent() { KLocker lock( m_csEventMonster ); return m_bActiveEvent; }
    bool CheckEvent( IN const int nModeID, OUT std::set<int>& setMonsterID );
    bool GetEventMonInfo( IN const int nMonsterID, OUT KEventMonInfo& kInfo );
    bool GetDungeonLevel( IN const int nModeID, IN const int nDifficulty, OUT PAIR_INT& prLevel );
    int GetMonsterLevel( IN const int nMonsterID );

    void DumpEventMonster();
    void DumpModeEvent();
    void DumpDungeonLevel();
    void DumpMonsterLevel();

    bool CheckEclipseEvent( IN const int nModeID_, OUT std::set<int>& setMonsterID_ );

private:
    bool LoadEventMonster( OUT std::string& strScript );
    bool LoadDropItem( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT std::vector<std::pair<KDropItemInfo,float> >& vecItem );
    bool LoadPartyDropItem( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT std::vector<std::pair<KDropItemInfo,float> >& vecItem );
    bool LoadAppearRate( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT std::map<int, float >& mapData );
    bool LoadTimeTable( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT time_t& tmTime );
    bool LoadGameMode( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT std::set<int>& setMode );
    bool LoadModeStage( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT std::map<int,std::vector<int> >& mapList );
    bool LoadDungeonLevel( OUT std::string& strScript );
    bool LoadMonsterLevel( OUT std::string& strScript );
    bool LoadLevelInfo( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT std::map<int, KDungeonLevel>& mapDungeonLevel_ );
    bool LoadChildMonsterInfo( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT std::vector<KEventChildMonInfo>& vecChildMonsterInfo );

    void UpdateEventInfo();
    void GetEventMonster( OUT std::map<int,KEventMonInfo>& mapData ) { KLocker lock(m_csEventMonster); mapData = m_mapEventMonster; }
    void UpdateEclipseEventInfo();

protected:
    mutable KncCriticalSection          m_csEventMonster;
    bool                                m_bActiveEvent; // 전체 이벤트 온오프
    std::map<int,KEventMonInfo>         m_mapEventMonster; // map<몬스터ID,이벤트몬스터정보>

    mutable KncCriticalSection          m_csModeEvent;
    std::map<int,std::set<int> >        m_mapModeEvent; // map<ModeID,set<몬스터ID> >

    mutable KncCriticalSection          m_csDungeonLevel;
    std::map<int,std::map<int,KDungeonLevel> > m_mapDungeonLevel; // map<ModeID,map<난이도,던전레벨>>

    mutable KncCriticalSection          m_csMonsterLevel;
    std::map<int,int>                   m_mapMonsterLevel; // map<몬스터ID,몬스터Lv>

    KncTimer                            m_kTickTimer;

    std::map<int,std::set<int> >        m_mapEclipseModeEvent;
};

DefSingletonInline( KEventMonsterManager );
DeclOstmOperatorA( KEventMonsterManager );