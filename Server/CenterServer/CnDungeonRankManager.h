#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include "CommonPacket.h"
#include "KncTimer.h"

class KCnDungeonRankManager
{
    DeclareSingleton( KCnDungeonRankManager );
    NiDeclareRootRTTI( KCnDungeonRankManager );
    DeclToStringW;

public:
    KCnDungeonRankManager( void );
    ~KCnDungeonRankManager( void );

public:
    bool GetRankingModes( OUT std::set< int >& setRankingModes_ );
    bool SetCurrentSeasonTopRank( IN const std::map< int, std::list< KDungeonRankInfo > >& mapCurrentDungeonRankList_ );
    bool SetPreviousSeasonTopRank( IN const std::map< int, std::list< KDungeonRankInfo > >& mapPreviousDungeonRankList_ );
    bool AddCurrentSeasonNewEntry( IN const std::vector< KDungeonRankInfo >& vecNewEntry_ );
    bool AddCurrentSeasonNewEntryAfterDBWrite( IN const std::vector< KDungeonRankInfo >& vecNewEntryAfterDBWrite_ );
    bool GetCurrentSeasonNewEntryToDBWithLock( OUT KECN_DUNGEON_RANK_CURRENT_SEASON_ADD_RANK_REQ& kPacket_ );
    void Tick();
    bool GetDungeonRankRankingConfigNotPacket( OUT KECN_DUNGEON_RANK_RANKING_CONFIG_NOT& kRankingConfig_ );
    bool GetDungeonRankCurrentSeasonTopRankNotPacket( OUT KECN_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_NOT& kCurrentRankPacket_ );
    bool GetDungeonRankPreviousSeasonTopRankNotPacket( OUT KECN_DUNGEON_RANK_PREVIOUS_SEASON_TOP_RANK_NOT& kPreviousRankPacket_ );
    CTime SetSeasonReset();
    bool SetSeasonDelimitTime( IN const time_t tmSeasonDelimit_ );
    bool DoForceSeasonReset();

private:
    bool SortCurrentSeasonTopRank();
    bool SendDungeonRankCurrentSeasonTopRankNot();
    bool GetCurrentSeasonNewEntryToDB( OUT KECN_DUNGEON_RANK_CURRENT_SEASON_ADD_RANK_REQ& kPacket_ );

private:
    mutable KncCriticalSection                      m_csCnDungeonRank;

    KncTimer                                        m_kRankingTimer; // 랭킹 계산 주기

    int                                             m_nRankEntryPerPage; // 한 페이지당 표시할 랭크 개수(클라 UI의 제약으로 고정하여 사용한다; 8개)
    std::set< int >                                 m_setRankingModes; // 랭킹을 처리할 던전 리스트 < ModeID >
    CTime                                           m_ctSeasonReset; // 현재 시즌 판단 기준 시간(이 시간 이후의 기록만 DB에 기록한다)

    // 현재 시즌 랭크 리스트
    std::map< int, std::list< KDungeonRankInfo > >  m_mapCurrentDungeonRankList; // < ModeID, DungeonRankInfo >
    std::map< int, std::list< KDungeonRankInfo > >  m_mapReservedCurrentDungeonRankList; // < ModeID, DungeonRankInfo >

    // 이전 시즌 랭크 리스트
    std::map< int, std::list< KDungeonRankInfo > >  m_mapPreviousDungeonRankList; // < ModeID, DungeonRankInfo >
    std::map< int, std::list< KDungeonRankInfo > >  m_mapReservedPreviousDungeonRankList; // < ModeID, DungeonRankInfo >
};

DefSingletonInline( KCnDungeonRankManager );
DeclOstmOperatorA( KCnDungeonRankManager );
