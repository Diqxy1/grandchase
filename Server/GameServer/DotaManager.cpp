#include "DotaManager.h"
#include "UserPacket.h"
#include "CommonPacket.h"
#include "Lua/KLuaManager.h"
#include <dbg/dbg.hpp >
#include "NetError.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KDotaManager );
ImplOstmOperatorW2A( KDotaManager );
NiImplementRootRTTI( KDotaManager );

KDotaManager::KDotaManager(void)
{
    m_mapDotaModeInfo.clear();
    m_mapDotaItemInfo.clear();
    m_mapLevelReward.clear();
    m_mapDPointTable.clear();
}

KDotaManager::~KDotaManager(void)
{
}

ImplToStringW( KDotaManager )
{
    KLocker lock( m_csDota );
    return START_TOSTRINGW
        << TOSTRINGW( m_mapDotaModeInfo.size() )
        << TOSTRINGW( m_mapDotaItemInfo.size() )
        << TOSTRINGW( m_mapLevelReward.size() )
        << TOSTRINGW( m_mapDPointTable.size() );
}

bool KDotaManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KDotaManager::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    std::map< int, KDotaModeInfo >              mapDotaModeInfo;
    std::map< KDropItemInfo, int >              mapDotaItemInfo;
    SConfigData                                 sConfigData;
    std::map< DWORD, std::pair<int,int> >       mapLevelReward;
    std::map< int, int >                        mapDPointTable;

    const std::string strScriptName = "InitDota.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( LoadDotaModeInfo( kLuaMng, std::string("DotaModeInfo"), mapDotaModeInfo ), return false );
    _JIF( LoadDotaItemList( kLuaMng, std::string("DotaItemList"), mapDotaItemInfo ), return false );
    _JIF( LoadDotaRewardConfig( kLuaMng, std::string("RewardConfig"), sConfigData ), return false );
    _JIF( LoadDotaLevelReward( kLuaMng, std::string("LevelReward"), mapLevelReward ), return false );
    _JIF( LoadMapInfo( kLuaMng, std::string("DPointTable"), mapDPointTable ), return false );

    START_LOG( cerr, L"점령전 스크립트 로드 완료. " ) << END_LOG;
    {
        KLocker lock( m_csDota );
        m_mapDotaModeInfo.swap( mapDotaModeInfo );
        m_mapDotaItemInfo.swap( mapDotaItemInfo );
        m_sConfigData = sConfigData;
        m_mapLevelReward.swap( mapLevelReward );
        m_mapDPointTable.swap( mapDPointTable );

        return true;
    }
}

bool KDotaManager::LoadDotaModeInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, KDotaModeInfo >& mapDotaModeInfo_ )
{
    mapDotaModeInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i ) {
        KDotaModeInfo kDotaModeInfo;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "ModeID", kDotaModeInfo.m_nModeID ) == S_OK, return false );
        _JIF( LoadDotaModeMapInfo( kLuaMng_, kDotaModeInfo.m_mapModeMapInfo ), return false );

        if( !mapDotaModeInfo_.insert( std::make_pair( kDotaModeInfo.m_nModeID, kDotaModeInfo ) ).second ) {
            START_LOG( cerr, L" 중복된 Mode 정보가 있음. Mode : " << kDotaModeInfo.m_nModeID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KDotaManager::LoadDotaModeMapInfo( IN KLuaManager& kLuaMng_, OUT std::map< int, std::map<int,int> >& mapModeMapInfo_ )
{
    mapModeMapInfo_.clear();

    for( int i = 1; ; ++i ) {
        int nMapID = 0;
        std::map<int,int> mapCoreSlotInfo;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "MapID", nMapID ) == S_OK, return false );
        _JIF( LoadMapInfo( kLuaMng_, std::string("CoreMonSlotID"), mapCoreSlotInfo ), return false );

        if( !mapModeMapInfo_.insert( std::make_pair( nMapID, mapCoreSlotInfo ) ).second ) {
            START_LOG( cerr, L" 중복된 Map 정보가 있음. Map : " << nMapID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    return true;
}

bool KDotaManager::LoadMapInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<int,int>& mapInfo_ )
{
    mapInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1 ; ; ++i ) {
        int nKey  = 0;
        int nData = 0;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, nKey ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nData ) == S_OK, return false );

        if( mapInfo_.insert( std::make_pair( nKey, nData ) ).second == false ) {
            START_LOG( cerr, L"Map에 중복된 값 있음. Key : " << nKey ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KDotaManager::LoadDotaItemList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< KDropItemInfo, int >& mapDotaItemInfo_ )
{
    mapDotaItemInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1 ; ; ++i ) {
        KDropItemInfo kDropItemInfo;
        int nPrice = 0;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, kDropItemInfo.m_ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, kDropItemInfo.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, kDropItemInfo.m_nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, nPrice ) == S_OK, return false );

        if( mapDotaItemInfo_.insert( std::make_pair( kDropItemInfo, nPrice ) ).second == false ) {
            START_LOG( cerr, L"중복된 아이템 있음. ItemID : " << kDropItemInfo.m_ItemID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KDotaManager::LoadDotaRewardConfig( IN KLuaManager& kLuaMng_, IN std::string& strTable_, SConfigData& sConfigData_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( "WinRatio", sConfigData_.m_fWinRatio ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "LoseRatio", sConfigData_.m_fLoseRatio ) == S_OK, return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}
 
bool KDotaManager::LoadDotaLevelReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< DWORD, std::pair<int,int> >& mapLevelReward_ )
{
    mapLevelReward_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1 ; ; ++i ) {
        DWORD dwLevel = 0;
        std::pair<int,int> prData;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( "Level", dwLevel ) == S_OK, return false );
        _JIF( LoadRewardInfo( kLuaMng_, std::string("Reward"), prData ), return false );

        if( mapLevelReward_.insert( std::make_pair( dwLevel, prData ) ).second == false ) {
            START_LOG( cerr, L"중복된 Level에 대한 보상 있음. Level : " << dwLevel ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KDotaManager::LoadRewardInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::pair<int,int>& prData_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    std::pair<int,int> prData;
    if( kLuaMng_.GetValue( 1, prData.first ) != S_OK ) return false;
    _JIF( kLuaMng_.GetValue( 2, prData.second ) == S_OK, return false );

    prData_ = prData;

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

void KDotaManager::GetDotaExpReward( IN DWORD dwLevel_, OUT int& nExp_ )
{
    std::map< DWORD, std::pair<int,int> >::iterator mit;

    KLocker lock( m_csDota );
    mit = m_mapLevelReward.find( dwLevel_ );
    if( mit == m_mapLevelReward.end() ) {
        nExp_ = 0;
        return;
    }
    nExp_ = mit->second.first;
}

void KDotaManager::GetDotaGPReward( IN DWORD dwLevel_, OUT int& nGP_ )
{
    std::map< DWORD, std::pair<int,int> >::iterator mit;

    KLocker lock( m_csDota );
    mit = m_mapLevelReward.find( dwLevel_ );
    if( mit == m_mapLevelReward.end() ) {
        nGP_ = 0;
        return;
    }
    nGP_ = mit->second.second;
}

bool KDotaManager::CheckDotaMode( IN const int nMode_ )
{
    std::map< int, KDotaModeInfo >::iterator mit;

    KLocker lock( m_csDota );
    mit = m_mapDotaModeInfo.find( nMode_ );
    if( mit == m_mapDotaModeInfo.end() ) {
        return false;
    }
    return true;
}

void KDotaManager::GetDotaItemList( OUT std::map<KDropItemInfo, int>& mapDotaItemInfo_ )
{
    KLocker lock( m_csDota );
    mapDotaItemInfo_ = m_mapDotaItemInfo;
}

void KDotaManager::GetDotaModeInfo( IN const int nMode_, OUT KDotaModeInfo& kDotaModeInfo_ )
{
    std::map< int, KDotaModeInfo >::iterator mit;

    KLocker lock( m_csDota );
    mit = m_mapDotaModeInfo.find( nMode_ );
    if( mit == m_mapDotaModeInfo.end() ) {
        return;
    }
    kDotaModeInfo_ = mit->second;
}

void KDotaManager::GetDotaDPointInfo( IN int nLevel_, OUT int& nDPoint_ )
{
    std::map< int, int >::iterator mit;

    KLocker lock( m_csDota );
    mit = m_mapDPointTable.find( nLevel_ );
    if( mit == m_mapDPointTable.end() ) {
        return;
    }
    nDPoint_ = mit->second;
}