#include "InfinityDungeon.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"
#include <boost/foreach.hpp>

ImplementSingleton( KInfinityDungeon );
ImplOstmOperatorW2A( KInfinityDungeon );
NiImplementRootRTTI( KInfinityDungeon );

ImplToStringW( KInfinityDungeon )
{
    KLocker lock( m_csInfinityDungeon );

    return START_TOSTRINGW
        << TOSTRINGW( m_setInfinityDungeonModes.size() )
        << TOSTRINGW( m_mapInfinityDungeonExpRewardRatio.size() )
        << TOSTRINGW( m_mapInfinityDungeonGPRewardRatio.size() )
        << TOSTRINGW( m_mapRewards.size() );
}

KInfinityDungeon::KInfinityDungeon( void )
{
    m_setInfinityDungeonModes.clear();
    m_mapInfinityDungeonExpRewardRatio.clear();
    m_mapInfinityDungeonGPRewardRatio.clear();
    m_mapRewards.clear();
}

KInfinityDungeon::~KInfinityDungeon( void )
{
}

bool KInfinityDungeon::LoadScript()
{
    return _LoadScript(std::string());
}

bool KInfinityDungeon::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    std::set< int > setInfinityDungeonModes;
    float fInfinityDungeonGPRewardRatio = 0.f;
    std::map< int, std::vector< KIDRewardInfo > > mapRewards;
    std::map< int, float > mapInfinityDungeonExpRewardRatio;
    std::map< int, float > mapInfinityDungeonGPRewardRatio;

    _JIF( kLuaMng.DoFile( "Enum.stg" ) == S_OK, return false );
    _JIF( kLuaMng.DoFile( "InitInfinityDungeon.lua" ) == S_OK, return false );

    _JIF( LoadInfinityDungeonInfo( kLuaMng, std::string( "InfinityDungeon" ), setInfinityDungeonModes ), return false );
    _JIF( LoadInfinityDungeonExpRewardRatio( kLuaMng, std::string( "InfinityDungeonExpRewardRatio" ), mapInfinityDungeonExpRewardRatio ), return false );
    _JIF( LoadInfinityDungeonGPRewardRatio( kLuaMng, std::string( "InfinityDungeonGPRewardRatio" ), mapInfinityDungeonGPRewardRatio ), return false );
    _JIF( LoadInfinityDungeonMonsterInfo( kLuaMng, std::string( "InfinityDungeonMonster" ), mapRewards ), return false );

    {
        KLocker lock( m_csInfinityDungeon );

        m_setInfinityDungeonModes.swap( setInfinityDungeonModes );
        m_mapInfinityDungeonExpRewardRatio.swap( mapInfinityDungeonExpRewardRatio );
        m_mapInfinityDungeonGPRewardRatio.swap( mapInfinityDungeonGPRewardRatio );
        m_mapRewards.swap( mapRewards );
    }

    START_LOG( cerr, L"무한던전 스크립트 읽기 완료." ) << END_LOG;

    return true;
}

bool KInfinityDungeon::LoadInfinityDungeonInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set< int >& setInfinityDungeonModes_ )
{
    setInfinityDungeonModes_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while ( true ) {
        int nModeID = -1;
        if ( kLuaMng_.GetValue( i++, nModeID ) != S_OK ) break;

        setInfinityDungeonModes_.insert( nModeID );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KInfinityDungeon::LoadInfinityDungeonExpRewardRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, float >& mapInfinityDungeonExpRewardRatio_ )
{
    mapInfinityDungeonExpRewardRatio_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        int nModeID = -1;
        float fRatio = 0.f;

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( 1, nModeID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, fRatio ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable i

        if ( false == mapInfinityDungeonExpRewardRatio_.insert( std::make_pair( nModeID, fRatio ) ).second ) {
            START_LOG( cerr, L"무한던전 스크립트에 중복된 모드ID 정보가 있습니다. (InfinityDungeonExpRewardRatio)" )
                << BUILD_LOG( nModeID )
                << END_LOG;

            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KInfinityDungeon::LoadInfinityDungeonGPRewardRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, float >& mapInfinityDungeonGPRewardRatio_ )
{
    mapInfinityDungeonGPRewardRatio_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        int nModeID = -1;
        float fRatio = 0.f;

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( 1, nModeID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, fRatio ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable i

        if ( false == mapInfinityDungeonGPRewardRatio_.insert( std::make_pair( nModeID, fRatio ) ).second ) {
            START_LOG( cerr, L"무한던전 스크립트에 중복된 모드ID 정보가 있습니다. (InfinityDungeonGPRewardRatio)" )
                << BUILD_LOG( nModeID )
                << END_LOG;

            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KInfinityDungeon::LoadInfinityDungeonMonsterInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, std::vector< KIDRewardInfo > >& mapRewards_ )
{
    mapRewards_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        int nModeID = -1;
        std::vector< KIDRewardInfo > vecIDRewardInfo;

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "ModeID", nModeID ) == S_OK, return false );
        _JIF( LoadMonsterInfo( kLuaMng_, std::string( "Monster" ), nModeID, vecIDRewardInfo ), return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // // endTable i

        if ( false == mapRewards_.insert( std::make_pair( nModeID, vecIDRewardInfo ) ).second ) {
            START_LOG( cerr, L"무한던전 스크립트에 중복된 모드ID 정보가 있습니다." )
                << BUILD_LOG( nModeID )
                << END_LOG;

            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KInfinityDungeon::LoadMonsterInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, IN const int nModeID_, OUT std::vector< KIDRewardInfo >& vecIDRewardInfo_ )
{
    vecIDRewardInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        KIDRewardInfo kIDRewardInfo;
        Kairo kKairo;

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "MonsterID", kIDRewardInfo.m_nMonsterID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "DropType", kIDRewardInfo.m_nDropType ) == S_OK, return false );
        _JIF( LoadDropInfo( kLuaMng_, std::string( "Drops" ), kIDRewardInfo.m_vecDrops ), return false );

        if ( false == SetDropKairo( kIDRewardInfo.m_vecDrops, kIDRewardInfo.m_vecKairoDrops ) ) {
            START_LOG( cerr, L"무한던전 스크립트 로드 중 보상 정보에 오류가 있습니다." )
                << BUILD_LOG( kIDRewardInfo.m_nMonsterID )
                << BUILD_LOG( kIDRewardInfo.m_nDropType )
                << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // // endTable i

        vecIDRewardInfo_.push_back( kIDRewardInfo );

    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KInfinityDungeon::LoadDropInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KIDDropElement >& vecDrops_ )
{
    vecDrops_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    int i = 1;
    while( true ) {
        KIDDropElement kData;
        if( kLuaMng_.GetValue( i++, kData.m_dwItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kData.m_nCount ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kData.m_nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kData.m_fProb ) == S_OK, return false );

        vecDrops_.push_back( kData );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KInfinityDungeon::SetDropKairo( IN std::vector< KIDDropElement >& vecDrops_, OUT std::vector< Kairo >& vecKairoDrops_ )
{
    vecKairoDrops_.clear();

    if ( true == vecDrops_.empty() ) {
        return false;
    }

    BOOST_FOREACH( KIDDropElement& rIDDropElement, vecDrops_ ) {
        Kairo kKairo;
        if ( false == kKairo.SetPostItem( rIDDropElement.m_dwItemID, rIDDropElement.m_nCount, rIDDropElement.m_nPeriod, rIDDropElement.m_fProb ) ) {
            START_LOG( cerr, L"무한던전 스크립트의 보상 정보에 오류가 있습니다." ) << END_LOG;
            return false;
        }

        vecKairoDrops_.push_back( kKairo );
    }

    return true;
}

bool KInfinityDungeon::IsInfinityDungeon( IN const int nModeID_ )
{
    KLocker lock( m_csInfinityDungeon );

    std::map< int, std::vector< KIDRewardInfo > >::iterator mit;
    mit = m_mapRewards.find( nModeID_ );

    if ( m_mapRewards.end() == mit ) {
        return false;
    }

    return true;
}

bool KInfinityDungeon::GetIDRewardInfo( IN const int nModeID_, OUT std::vector< KIDRewardInfo >& vecIDRewards_ )
{
    KLocker lock( m_csInfinityDungeon );

    std::map< int, std::vector< KIDRewardInfo > >::iterator mit;
    mit = m_mapRewards.find( nModeID_ );

    if ( m_mapRewards.end() == mit ) {
        return false;
    }

    vecIDRewards_ = mit->second;

    return true;
}

float KInfinityDungeon::GetGPRewardRatio( IN const int nModeID_ )
{
    KLocker lock( m_csInfinityDungeon );

    float fGPRewardRatio = 1.0f;

    std::map< int, float >::iterator mitInfinityDungeonGPRewardRatio;
    mitInfinityDungeonGPRewardRatio = m_mapInfinityDungeonGPRewardRatio.find( nModeID_ );
    if ( m_mapInfinityDungeonGPRewardRatio.end() != mitInfinityDungeonGPRewardRatio ) {
        fGPRewardRatio = mitInfinityDungeonGPRewardRatio->second;
    }

    return fGPRewardRatio;
}

float KInfinityDungeon::GetExpRewardRatio( IN const int nModeID_ )
{
    KLocker lock( m_csInfinityDungeon );

    float fExpRewardRatio = 1.0f;

    std::map< int, float >::iterator mitInfinityDungeonExpRewardRatio;
    mitInfinityDungeonExpRewardRatio = m_mapInfinityDungeonExpRewardRatio.find( nModeID_ );
    if ( m_mapInfinityDungeonExpRewardRatio.end() != mitInfinityDungeonExpRewardRatio ) {
        fExpRewardRatio = mitInfinityDungeonExpRewardRatio->second;
    }

    return fExpRewardRatio;
}
