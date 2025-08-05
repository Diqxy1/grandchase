#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <set>
#include <map>
#include <vector>
#include "PrimitiveTypedef.h"
#include "Kairo.h"

class KLuaManager;

class KInfinityDungeon
{
    DeclareSingleton( KInfinityDungeon );
    NiDeclareRootRTTI( KInfinityDungeon );
    DeclToStringW;

public:
    enum EDropType {
        EDT_PARTY   = 0, // 파티원 수 만큼 드랍
        EDT_SOLO    = 1, // 하나만 드랍
    };

    struct KIDDropElement {
        GCITEMID    m_dwItemID;
        int         m_nCount;
        int         m_nPeriod;
        float       m_fProb;

        KIDDropElement() : m_dwItemID( 0 ), m_nCount( -1 ), m_nPeriod( -1 ), m_fProb( 0 )
        {}
    };

    struct KIDRewardInfo {
        int                             m_nMonsterID;
        int                             m_nDropType;
        std::vector< KIDDropElement >   m_vecDrops;
        std::vector< Kairo >            m_vecKairoDrops;

        KIDRewardInfo() : m_nMonsterID( 0 ), m_nDropType( EDT_PARTY ) {
            m_vecDrops.clear();
            m_vecKairoDrops.clear();
        }
    };

    KInfinityDungeon( void );
    ~KInfinityDungeon( void );

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsInfinityDungeon( IN const int nModeID_ );
    bool GetIDRewardInfo( IN const int nModeID_, OUT std::vector< KIDRewardInfo >& vecIDRewards_ );
    float GetGPRewardRatio( IN const int nModeID_ );
    float GetExpRewardRatio( IN const int nModeID_ );

private:
    bool LoadInfinityDungeonInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set< int >& setInfinityDungeonModes_ );
    bool LoadInfinityDungeonExpRewardRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, float >& mapInfinityDungeonExpRewardRatio_ );
    bool LoadInfinityDungeonGPRewardRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, float >& mapInfinityDungeonGPRewardRatio_ );
    bool LoadInfinityDungeonMonsterInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, std::vector< KIDRewardInfo > >& mapRewards_ );
    bool LoadMonsterInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, IN const int nModeID_, OUT std::vector< KIDRewardInfo >& vecIDRewardInfo_ );
    bool LoadDropInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KIDDropElement >& vecDrops_ );
    bool SetDropKairo( IN std::vector< KIDDropElement >& vecDrops_, OUT std::vector< Kairo >& vecKairoDrops_ );

private:
    mutable KncCriticalSection                      m_csInfinityDungeon;

    std::set< int >                                 m_setInfinityDungeonModes;    // 무한던전 ModeID 리스트
    std::map< int, float >                          m_mapInfinityDungeonExpRewardRatio; // 무한던전 EXP 보상 배율
    std::map< int, float >                          m_mapInfinityDungeonGPRewardRatio; // 무한던전 GP 보상 배율
    std::map< int, std::vector< KIDRewardInfo > >   m_mapRewards;                 // (모드ID, 드랍 정보)
};

DefSingletonInline( KInfinityDungeon );
DeclOstmOperatorA( KInfinityDungeon );
