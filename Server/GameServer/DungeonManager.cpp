#include "DungeonManager.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"

ImplementSingleton( KDungeonManager );
ImplOstmOperatorW2A( KDungeonManager );
NiImplementRootRTTI( KDungeonManager );

KDungeonManager::KDungeonManager(void)
:m_usDefaultUserNum(DEFAULT_USER_NUM)
{
    m_mapDungeonUserNumList.clear();
    m_setModeChangeDisableList.clear();
    m_mapRewardTypeInfo.clear();
}

KDungeonManager::~KDungeonManager(void)
{
}

ImplToStringW( KDungeonManager )
{
    KLocker lock( m_csDungeonManager );
    return START_TOSTRINGW
        << TOSTRINGW( m_usDefaultUserNum )
        << TOSTRINGW( m_mapDungeonUserNumList.size() )
        << TOSTRINGW( m_setModeChangeDisableList.size() )
        << TOSTRINGW( m_mapRewardTypeInfo.size() );
}

bool KDungeonManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KDungeonManager::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    std::map<int,USHORT>                mapDungeonUserNumList;
    std::set<int>                       setModeChangeDisableList;
    std::map<int,int>                   mapRewardTypeInfo;

    const std::string strScriptName = "InitDungeonManager.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( LoadMaxUserData( kLuaMng, std::string("DungeonMaxUserInfo"), mapDungeonUserNumList ), return false );
    _JIF( LoadSetData( kLuaMng, std::string("DungeonModeChangeDisableInfo"), setModeChangeDisableList ), return false );
    _JIF( LoadMapData( kLuaMng, std::string("DungeonRewardTypeInfo"), mapRewardTypeInfo ), return false );

    {
        KLocker lock( m_csDungeonManager );
        m_mapDungeonUserNumList.swap( mapDungeonUserNumList );
        m_setModeChangeDisableList.swap( setModeChangeDisableList );
        m_mapRewardTypeInfo.swap( mapRewardTypeInfo );
    }

    START_LOG( cerr, L"던전 매니저 정보 읽기 완료." )
        << BUILD_LOG( m_mapDungeonUserNumList.size() ) 
        << BUILD_LOG( m_setModeChangeDisableList.size() ) 
        << BUILD_LOG( m_mapRewardTypeInfo.size() ) 
        << END_LOG;

    return true;
}

bool KDungeonManager::LoadMaxUserData( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<int,USHORT>& mapDungeonUserNumList_ )
{
    mapDungeonUserNumList_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nModeID;
        USHORT usUserNum;
        _JIF( kLuaMng_.GetValue( "ModeID", nModeID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "MaxUser", usUserNum ) == S_OK, return false );

        if( mapDungeonUserNumList_.insert( std::make_pair( nModeID, usUserNum ) ).second == false ) {
            START_LOG( cerr, L"중복된 ModeID. :" << nModeID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KDungeonManager::LoadSetData( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set<int>& setInfo_ )
{
    setInfo_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    int nData = 0;
    while( true ) {
        if( kLuaMng_.GetValue( i++, nData ) != S_OK ) break;
        setInfo_.insert( nData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

void KDungeonManager::GetDefaultDungeonInfo( IN const int& nGameMode_, OUT KDefaultModeInfo& kDefaultModeInfo_ )
{
    std::map<int,USHORT>::iterator mitUserNum;
    std::set<int>::iterator sitModeChange;
    std::map<int,int>::iterator mitRewardType;
    std::set<int>::iterator sitUserSlotOpen;

    KLocker lock( m_csDungeonManager );
    mitUserNum = m_mapDungeonUserNumList.find( nGameMode_ );
    if( mitUserNum != m_mapDungeonUserNumList.end() ) {
        kDefaultModeInfo_.m_usDefaultUser = mitUserNum->second;
    } else {
        kDefaultModeInfo_.m_usDefaultUser = m_usDefaultUserNum;
    }

    sitModeChange = m_setModeChangeDisableList.find( nGameMode_ );
    if( sitModeChange != m_setModeChangeDisableList.end() ) {
        kDefaultModeInfo_.m_bModeChangeEnable = false;
    } else {
        kDefaultModeInfo_.m_bModeChangeEnable = true;
    }

    mitRewardType = m_mapRewardTypeInfo.find( nGameMode_ );
    if( mitRewardType != m_mapRewardTypeInfo.end() ) {
        kDefaultModeInfo_.m_nRewardType = mitRewardType->second;
    }
}

bool KDungeonManager::LoadMapData( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<int,int>& mapRewardTypeInfo_ )
{
    mapRewardTypeInfo_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nModeID = -1;
        int nType = -1;

        _JIF( kLuaMng_.GetValue( 1, nModeID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nType ) == S_OK, return false );

        if( mapRewardTypeInfo_.insert( std::make_pair( nModeID, nType ) ).second == false ) {
            START_LOG( cerr, L"던전 보상 Type세팅중 같은 ModeID :" << nModeID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KDungeonManager::GetDungeonRewardType( IN const int& nGameMode_, OUT int& nRewardType_ )
{
    KLocker lock( m_csDungeonManager );
    std::map<int,int>::iterator mitRewardType;

    mitRewardType = m_mapRewardTypeInfo.find( nGameMode_ );
    if( mitRewardType == m_mapRewardTypeInfo.end() ) {
        return false;
    }

    nRewardType_ = mitRewardType->second;
    return true;
}
