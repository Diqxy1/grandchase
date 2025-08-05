#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>

struct KDicePlayRank;
class KLuaManager;

class KCnDicePlay
{
    DeclareSingleton( KCnDicePlay );
    NiDeclareRootRTTI( KCnDicePlay );
    DeclToStringW;

public:
    KCnDicePlay(void);
    ~KCnDicePlay(void);

     bool LoadScript();
     void InitProcess();
     void Tick();

     void UpdateDicePlayRankDB( IN std::vector<KDicePlayRank>& vecRank ); // DB에서 랭킹정보 가져오기.
     void UpdateDicePlayRankServer( IN std::vector<KDicePlayRank>& vecRank ); // GameServer -> CenterServer 랭킹 정보 전송.
     void GetDicePlayRank( OUT std::vector<KDicePlayRank>& vecRank ); // 랭킹 정보 가져오기.
     void PrintRank();       // 랭킹 정보 출력.

protected:
    bool ChkUpdateFromDB(); // DB Update를 위해서 하루가 지났는지를 체크.
    bool LoadUpdateTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool IsRun() const; // 이벤트 진행중인지 체크.
    void AddRankNoSort( IN KDicePlayRank& kRank, DWORD dwTotalCount, DWORD dwCurrentPosition );  // 랭킹 정보 정렬.
    void SortAddNumbering( IN OUT std::vector<KDicePlayRank>& vecRank ); // 랭킹정보 최대 숫자만큼 가지고 있기.

protected:
    mutable KncCriticalSection          m_csDicePlay;
    std::vector<KDicePlayRank>          m_vecDicePlayRank;      // 랭킹정보.

    time_t      m_tmBegin;              // 기부 시작 날짜
    time_t      m_tmEnd;                // 기부 종료 날짜
    time_t      m_tmDBUpdateTime;       // 다음 DB 업데이트 시간 지정
    DWORD       m_dwLastTick;
    DWORD       m_dwTickGap;

    DWORD                               m_dwMaxRankSize;        // 최대 랭킹 정보 크기.

};
DefSingletonInline( KCnDicePlay );
DeclOstmOperatorA( KCnDicePlay );