#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include "CenterPacket.h"

class KDungeonRankManager
{
    DeclareSingleton( KDungeonRankManager );
    NiDeclareRootRTTI( KDungeonRankManager );
    DeclToStringW;

public:
    KDungeonRankManager( void );
    ~KDungeonRankManager( void );

public:
    bool GetRankingModes( OUT std::set< int >& setRankingModes_ );
    bool SetRankingConfig( IN const KECN_DUNGEON_RANK_RANKING_CONFIG_NOT& kRankingConfig_ );
    bool SetCurrentDungeonRankList( IN const KECN_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_NOT& kCurrentRankPacket_ );
    bool SetPreviousDungeonRankList( IN const KECN_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_NOT& kPreviousRankPacket );
    bool IsRankingModes( IN const int nModeID_ );
    int GetRankEntryPerPage();
    bool GetCurrentDungeonRankList( IN const int nModeID_, OUT std::list< KDungeonRankInfo >& lstCurrentDungeonRankList_ );
    bool GetPreviousDungeonRankList( IN const int nModeID_, OUT std::list< KDungeonRankInfo >& lstPreviousDungeonRankList_ );
    bool AddDungeonRankCurrentSeason(  IN const int nModeID_, IN const std::vector< KDungeonRankInfo >& vecDungeonRankInfo_ );

private:
    mutable KncCriticalSection                      m_csDungeonRank;

    int                                             m_nRankEntryPerPage; // 한 페이지당 표시할 랭크 개수(클라 UI의 제약으로 고정하여 사용한다; 8개)
    std::set< int >                                 m_setRankingModes; // 랭킹을 처리할 던전 리스트 < ModeID >

    // 현재 시즌 랭크 리스트
    std::map< int, std::list< KDungeonRankInfo > >  m_mapCurrentDungeonRankList; // < ModeID, DungeonRankInfo >

    // 이전 시즌 랭크 리스트
    std::map< int, std::list< KDungeonRankInfo > >  m_mapPreviousDungeonRankList; // < ModeID, DungeonRankInfo >
};

DefSingletonInline( KDungeonRankManager );
DeclOstmOperatorA( KDungeonRankManager );
