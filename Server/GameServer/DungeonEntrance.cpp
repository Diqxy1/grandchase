#include "DungeonEntrance.h"
#include <dbg/dbg.hpp>
#include "KncBitStream.h"
#include "Lua/KLuaManager.h"
#include "ResultManager.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KDungeonEntrance );
ImplOstmOperatorW2A( KDungeonEntrance );
NiImplementRootRTTI( KDungeonEntrance );

KDungeonEntrance::KDungeonEntrance(void)
{
}

KDungeonEntrance::~KDungeonEntrance(void)
{
}


std::wostream& operator<< ( std::wostream& stm_, const std::pair< int, sEntranceInfo >& data_ )
{
    stm_ << data_.first << L"{";
    std::vector<int>::const_iterator vit;
    for( vit = data_.second.m_vecConditions.begin() ; vit != data_.second.m_vecConditions.end() ; ++vit )
    {
        stm_ << *vit << L",";
    }
    stm_ << L"}";
    return stm_;
}

ImplToStringW( KDungeonEntrance )
{
    KLocker lock( m_csEntranceInfo );

    START_TOSTRINGW
        << TOSTRINGW( m_mapEntranceInfo.size() );

    std::copy( m_mapEntranceInfo.begin(), m_mapEntranceInfo.end(), 
        std::ostream_iterator<std::map<int, sEntranceInfo>::value_type,wchar_t>( stm_, L", " ) );

    return stm_;
}

bool KDungeonEntrance::LoadScript()
{
    return _LoadScript(std::string());
}

bool KDungeonEntrance::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    std::map< int, sEntranceInfo> mapEntranceInfo;
    mapEntranceInfo.clear();

    const std::string strScriptName = "DungeonState.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "DungeonEntranceInfo" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng.BeginTable( i ) != S_OK ) break;

        sEntranceInfo sInfo;
        _JIF( LoadEntranceInfo( kLuaMng, sInfo ), return false );
        _JIF( kLuaMng.EndTable() == S_OK, return false );
        // 넣어 주자...^^

        std::pair<std::map<int,sEntranceInfo>::iterator, bool> prRet;
        prRet = mapEntranceInfo.insert( std::make_pair( sInfo.m_nDungeonID, sInfo ) );
        if( !prRet.second )
        {
            START_LOG( cerr, L"중복된 데이터가 있습니다.." )
                << BUILD_LOG( sInfo.m_nDungeonID ) << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );
    START_LOG( cerr, L" DungeonEntranceInfo 읽기 성공.. Size : " << mapEntranceInfo.size() ) << END_LOG;

    KLocker lock( m_csEntranceInfo );
    m_mapEntranceInfo.swap( mapEntranceInfo );
    return true;
}

bool KDungeonEntrance::LoadEntranceInfo( IN KLuaManager& kLuaMng_, OUT sEntranceInfo& sInfo_ )
{
    _JIF( 0 == kLuaMng_.GetValue( "DungeonID", sInfo_.m_nDungeonID ) , return false );

    sInfo_.m_vecConditions.clear();
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        int nID;
        _JIF( 0 == kLuaMng_.GetValue( i, nID ) , return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        sInfo_.m_vecConditions.push_back( nID );
    }
    return true;
}

bool KDungeonEntrance::CheckDungeonEntrance( IN const std::map<int,KDungeonUserInfo>& m_mapDifficulty_, IN const int& nDungeonID_, IN OUT int& nDifficulty_ )
{
    // 스크립트에서 찾는다.
    sEntranceInfo sInfo;
    __JIF( GetEntranceInfo( nDungeonID_, sInfo ), return false );

    // 해당 던전에 플레이 가능한 난이도가 아니면 플레이 가능한 난이도로 강제 변경
    __LIF( SiKResultManager()->IsExistDifficulty( nDungeonID_, nDifficulty_ ) );

    // 조건 던전이 없다면. OK
    if( sInfo.m_vecConditions.empty() )
        return true;

    std::vector<int>::iterator vit;
    for( vit = sInfo.m_vecConditions.begin() ; vit != sInfo.m_vecConditions.end() ; ++vit )
    {
        std::map<int,KDungeonUserInfo>::const_iterator mit;
        mit = m_mapDifficulty_.find( *vit );
        if( mit != m_mapDifficulty_.end() && mit->second.m_kModeDifficulty.Get( 0 ) )
            return true;
    }

    return false;
}

bool KDungeonEntrance::GetEntranceInfo( IN int nID_, OUT sEntranceInfo& sInfo_ )
{
    KLocker lock( m_csEntranceInfo );

    std::map< int, sEntranceInfo >::iterator mit;
    mit = m_mapEntranceInfo.find( nID_ );

    if( mit == m_mapEntranceInfo.end() )
        return false;

    sInfo_ = mit->second;
    return true;
}