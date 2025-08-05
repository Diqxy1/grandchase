#pragma once
#include "CommonPacket.h"
#include <ToString.h>
#include <KNCSingleton.h>
#include <Thread/Locker.h>
#include <RTTI.H>

class KMiniGameRankManager
{
    DeclareSingleton( KMiniGameRankManager );
    NiDeclareRootRTTI( KMiniGameRankManager );
    DeclToStringW;

public:
    KMiniGameRankManager(void);
    ~KMiniGameRankManager(void);

    bool GetUserRankRange( IN char cType, IN int nBegin, IN int nCount, OUT std::vector<KMiniGameRankInfo>& vecRank ); // 미니게임 종류에 따라 랭킹 리스트 가져오기
    bool GetUserRank( IN char cType, IN DWORD dwUID, OUT KMiniGameRankInfo& kInfo ); // 유저 한 명의 랭킹
    void AddRank( IN char cType, IN OUT KMiniGameRankInfo& kInfo ); // 유저 한 명의 순위를 더해서 랭크 정렬
    void UpdateRank( IN std::map<char, std::vector<KMiniGameRankInfo> >& m_mapTotalRank ); // Center Proxy 에서 값을 받아오면 거기서 호출.
    void Tick();
    void DumpUID( char cType );
    void DumpMiniRank();
    void SetUpdateGap( IN DWORD dwUpdateGap ); // bind For Lua

protected: // CS 걸지 않았음.. 사용하는 곳에서 거시오.
    bool CheckUpdateTime();
    bool SendLocalRankToCenter(); // 
    void BuildRank(); // BuildRankByScore , BuildRankByUID 호출
    void BuildRankByScore( IN OUT std::vector<KMiniGameRankInfo>& vecRank ); // 점수로 정렬
    void BuildRankByUID( IN OUT std::vector<KMiniGameRankInfo>& vecRank );   // USER UID 로 정렬
    void Clipping( IN DWORD dwSize, IN OUT std::vector<KMiniGameRankInfo>& vecRank );
    bool AddGameRank( IN OUT std::vector<KMiniGameRankInfo>& vecRank, IN OUT KMiniGameRankInfo& kInfo );
    bool GetRankVector( IN std::map<char, std::vector<KMiniGameRankInfo> >& mapRank, IN char cType, OUT std::vector<KMiniGameRankInfo>** ppvecRank );

    bool GetUserRank( IN OUT std::vector<KMiniGameRankInfo>& vecRank, IN OUT KMiniGameRankInfo& kInfo );

protected:
    mutable KncCriticalSection  m_csTickInfo;
    DWORD                       m_dwUpdateGap;
    DWORD                       m_dwLastUpdateTick;

    mutable KncCriticalSection                      m_csRank;
    std::map<char, std::vector<KMiniGameRankInfo> > m_mapTotalRank; // < Type, Sorted By Rank >
    std::map<char, std::vector<KMiniGameRankInfo> > m_mapTotalUser; // < Type, Sorted By UID >
};

DefSingletonInline( KMiniGameRankManager );
DeclOstmOperatorA( KMiniGameRankManager );