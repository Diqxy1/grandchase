#include "SpecialMonsterManager.h"
#include "Lua/KLuaManager.h"
#include <dbg/dbg.hpp>
#include "Log4.h"

//FILE_NAME_FOR_LOG

ImplementSingleton( KSpecialMonsterManager );
ImplOstmOperatorW2A( KSpecialMonsterManager );
NiImplementRootRTTI( KSpecialMonsterManager );

ImplToStringW( KSpecialMonsterManager )
{
    KLocker lock( m_csSpecialMonster );

    return START_TOSTRINGW
        << TOSTRINGW( m_mapSpecialMonsterInfo.size() );
}

KSpecialMonsterManager::KSpecialMonsterManager(void)
:m_bActiveEvent(false)
{
    m_mapSpecialMonsterInfo.clear();
}

KSpecialMonsterManager::~KSpecialMonsterManager(void)
{
}

bool KSpecialMonsterManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KSpecialMonsterManager::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    bool bActiveEvent;
    std::map< std::pair<int,int>, KSpecialMonInfo > mapSpecialMonsterInfo;
    mapSpecialMonsterInfo.clear();

    _JIF( kLuaMng.DoFile( "InitSpecialMonster.lua" ) == S_OK, return false );
    _JIF( kLuaMng.BeginTable( "SpecialMonster" ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "ActiveEvent", bActiveEvent ) == S_OK, return false );

    for ( int i = 1 ; ; ++i ) {
        KSpecialMonInfo kSpecialMonInfo;
        std::pair<int,int> prData;

        if ( kLuaMng.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng.GetValue( "GameMode", kSpecialMonInfo.m_nGameModeID ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "Difficult", kSpecialMonInfo.m_nDifficult ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "MonsterID", kSpecialMonInfo.m_nMonsterID ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "MonsterLevel", kSpecialMonInfo.m_nMonLv ) == S_OK, return false );

        _JIF( LoadPropertyInfo( kLuaMng, "PropertyInfo", kSpecialMonInfo.m_bPropertyEnable, kSpecialMonInfo.m_kPropertyCount, kSpecialMonInfo.m_vecProperty ), return false );
        _JIF( kLuaMng.EndTable() == S_OK, return false );

        prData.first = kSpecialMonInfo.m_nGameModeID;
        prData.second = kSpecialMonInfo.m_nDifficult;
        _JIF( mapSpecialMonsterInfo.insert( std::make_pair( prData, kSpecialMonInfo ) ).second == true, return false );
    }

    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L"Load SpecialMonster script.. IsActive : " << bActiveEvent << L", size : " << mapSpecialMonsterInfo.size() ) << END_LOG;

    KLocker lock( m_csSpecialMonster );
    m_bActiveEvent = bActiveEvent;
    m_mapSpecialMonsterInfo.swap( mapSpecialMonsterInfo );
    return true;
}

bool KSpecialMonsterManager::LoadPropertyInfo( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT bool& bPropertyEnable_, OUT Kairo& kPropertyCount_, OUT std::vector< Kairo >& vecProperty_ )
{
    vecProperty_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( 0 == kLuaMng_.GetValue( "bEnable", bPropertyEnable_ ), return false );

    //Property Count 읽기..
    _JIF( kLuaMng_.BeginTable( "PropertyCount" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        DWORD dwCnt = 0;
        float fProb = 0.f;

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( 0 == kLuaMng_.GetValue( 1, dwCnt ), return false );
        _JIF( 0 == kLuaMng_.GetValue( 2, fProb ), return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        kPropertyCount_.SetPostType( dwCnt, fProb );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    // Property 읽기..
    _JIF( kLuaMng_.BeginTable( "Property" ) == S_OK, return false );
    for( int i = 1 ; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        Kairo kKairo;
        for( int j = 1 ; ; ++j ) {
            if( kLuaMng_.BeginTable( j ) != S_OK ) break;
            int nCase = 0;
            float fRatio = 0.f;
            _JIF( 0 == kLuaMng_.GetValue( 1, nCase ), return false );
            _JIF( 0 == kLuaMng_.GetValue( 2, fRatio ), return false );
            _JIF( kLuaMng_.EndTable() == S_OK, return false );

            kKairo.SetPostType( nCase, fRatio );
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        // 벡터에 넣기..
        vecProperty_.push_back( kKairo );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KSpecialMonsterManager::GetSpecialMonsterInfo( IN int nModeID_, IN int nDifficult_, OUT KSpecialMonInfo& kSpecialMonInfo_ )
{
    std::pair<int,int> prData;
    prData.first = nModeID_;
    prData.second = nDifficult_;
    std::map< std::pair<int,int>, KSpecialMonInfo >::iterator mit;

    KLocker lock( m_csSpecialMonster );
    mit = m_mapSpecialMonsterInfo.find( prData );
    if( mit == m_mapSpecialMonsterInfo.end() ) {
        return false;
    }
    kSpecialMonInfo_ = mit->second;
    return true;
}