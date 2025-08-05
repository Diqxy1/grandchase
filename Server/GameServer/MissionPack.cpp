#include "MissionPack.h"
#include "Lua/KLuaManager.h"

#include <dbg/dbg.hpp >
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KMissionPack );
ImplOstmOperatorW2A( KMissionPack );
NiImplementRootRTTI( KMissionPack );

ImplToStringW( KMissionPack )
{
    KLocker lock( m_csMissionPack );

    return START_TOSTRINGW
        << TOSTRINGW( m_mapMissionPackList.size() );
}

KMissionPack::KMissionPack(void)
{}
KMissionPack::~KMissionPack(void)
{
}

bool KMissionPack::LoadScript()
{
    return _LoadScript(std::string());
}

bool KMissionPack::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    KEVENT_MISSION_PACK_LIST_NOT    mapMissionPackList;

    const std::string strScriptName = "InitMissionPack.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.BeginTable( "MissionPack" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng.BeginTable( i ) != S_OK ) break;

        std::set<int> setMissionList;
        int nMissionPackID = -1;

        _JIF( kLuaMng.GetValue( "MissionID", nMissionPackID ) == S_OK, return false );

            _JIF( kLuaMng.BeginTable( "MissionList" ) == S_OK, return false );
            _JIF( LoadMissionList( kLuaMng, setMissionList ), return false );
            _JIF( kLuaMng.EndTable() == S_OK, return false );

        _JIF( kLuaMng.EndTable() == S_OK, return false );

        mapMissionPackList[ nMissionPackID ].swap( setMissionList );
        
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    KLocker lock( m_csMissionPack );
    m_mapMissionPackList.swap( mapMissionPackList );

    START_LOG( cerr, L"미션패키지 스크립트 읽기 완료. size : " << m_mapMissionPackList.size() ) << END_LOG;

    return true;
}

bool KMissionPack::LoadMissionList( IN KLuaManager& kLuaMng_, OUT std::set<int>& setMissionList_ )
{
    for( int i = 1 ; ; ++i )
    {
        int nMissionID;
        if( 0 != kLuaMng_.GetValue( i, nMissionID ) ) break;

        setMissionList_.insert( nMissionID );
    }
    return true;
}

void KMissionPack::GetMissionList( OUT KEVENT_MISSION_PACK_LIST_NOT& mapMissionPack_ )
{
    KLocker lock( m_csMissionPack );
    mapMissionPack_ = m_mapMissionPackList;
}

bool KMissionPack::GetMissionPackInfo( IN DWORD dwItemID_, OUT std::set<int>& setMissionList_ )
{
    KEVENT_MISSION_PACK_LIST_NOT::iterator mit;

    mit = m_mapMissionPackList.find( dwItemID_ );
    if( mit == m_mapMissionPackList.end() ) return false;

    setMissionList_ = mit->second;
    return true;
}