#include "ItemTradeManager.h"
#include "Lua/KLuaManager.h"
#include <KncUtilLua.h> // boost::bind
#include <dbg/dbg.hpp >
#include "NetError.h"
#include "CommonPacket.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KItemTradeManager );
ImplOstmOperatorW2A( KItemTradeManager );
NiImplementRootRTTI( KItemTradeManager );

KItemTradeManager::KItemTradeManager(void)
:m_bItemTradeEnable(false)
{
    m_mapTradeInfo.clear();
    m_mapAllTradeInfo.clear();
}

KItemTradeManager::~KItemTradeManager(void)
{
}

ImplToStringW( KItemTradeManager )
{
    KLocker lock( m_csItemTrade );

    return START_TOSTRINGW
        << TOSTRINGWb( m_bItemTradeEnable )
        << TOSTRINGW( m_mapTradeInfo.size() )
        << TOSTRINGW( m_mapAllTradeInfo.size() );
}

bool KItemTradeManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KItemTradeManager::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    bool        bEnable;
    std::map< std::pair<DWORD, int>, KDropItemInfo > mapTradeInfo;
    std::map< DWORD, KDropItemInfo > mapAllTradeInfo;

    mapTradeInfo.clear();
    mapAllTradeInfo.clear();

    const std::string strScriptName = "InitItemTrade.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    // 아이템 교환 사용여부
    _JIF( kLuaMng.GetValue( "TradeEnable", bEnable ) == S_OK, return false );
    _JIF( LoadItemTradeData( kLuaMng, std::string("ItemTradeInfo"), mapTradeInfo ), return false );
    _JIF( LoadItemAllTradeData( kLuaMng, std::string("ItemAllTradeInfo"), mapAllTradeInfo ), return false );

    KLocker lock( m_csItemTrade );
    m_bItemTradeEnable = bEnable;
    m_mapTradeInfo.swap( mapTradeInfo );
    m_mapAllTradeInfo.swap( mapAllTradeInfo );

    return true;
}

// 교환될 아이템 리스트 테이블 로드
bool KItemTradeManager::LoadItemTradeData( KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< std::pair<DWORD, int>, KDropItemInfo >& mapTradeInfo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        std::pair<DWORD,int> prKey;
        KDropItemInfo kData;
        prKey.first = 0;
        prKey.second = -1;

        _JIF( kLuaMng_.BeginTable( "MoneyItemTable" ) == S_OK, return false );
        if( kLuaMng_.GetValue( 1, prKey.first ) != S_OK ) break;
        if( kLuaMng_.GetValue( 2, prKey.second ) != S_OK ) break;
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( kLuaMng_.BeginTable( "RewardItemTable" ) == S_OK, return false );
        if( kLuaMng_.GetValue( 1, kData.m_ItemID ) != S_OK ) break;
        if( kLuaMng_.GetValue( 2, kData.m_nDuration ) != S_OK ) break;
        if( kLuaMng_.GetValue( 3, kData.m_nPeriod ) != S_OK ) break;
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if( mapTradeInfo_.insert( std::make_pair( prKey, kData ) ).second == false )
        {
            START_LOG( cerr, L"아이템 교환 데이터 입력중 중복된 Key값. ItemID : " << prKey.first << L",ItemCount : " << prKey.second ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"아이템 교환 테이블 Load 완료. size : " << mapTradeInfo_.size() ) << END_LOG;

    return true;
}

bool KItemTradeManager::LoadItemAllTradeData( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< DWORD, KDropItemInfo >& mapAllTradeInfo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        DWORD prKey;
        KDropItemInfo kData;
        prKey= 0;

        if( kLuaMng_.GetValue( "MoneyItem", prKey ) != S_OK ) break;

        _JIF( kLuaMng_.BeginTable( "RewardItem" ) == S_OK, return false );
        if( kLuaMng_.GetValue( 1, kData.m_ItemID ) != S_OK ) break;
        if( kLuaMng_.GetValue( 2, kData.m_nDuration ) != S_OK ) break;
        if( kLuaMng_.GetValue( 3, kData.m_nPeriod ) != S_OK ) break;
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if( mapAllTradeInfo_.insert( std::make_pair( prKey, kData ) ).second == false ) {
            START_LOG( cerr, L"전체 아이템 교환 데이터 입력중 중복된 Key값. ItemID : " << prKey ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"전체 아이템 교환 테이블 Load 완료. size : " << mapAllTradeInfo_.size() ) << END_LOG;

    return true;
}

bool KItemTradeManager::GetItemTradeInfo( IN const GCITEMID dwItemID_, IN const int nItemCount_, OUT KDropItemInfo& nRewardItem_ )
{
    std::map< std::pair<DWORD, int>, KDropItemInfo >::iterator mit;
    KLocker lock( m_csItemTrade );
    mit = m_mapTradeInfo.find(std::make_pair( dwItemID_,nItemCount_) );
    if( mit == m_mapTradeInfo.end() ) return false;
    nRewardItem_ = mit->second;
    return true;
}

bool KItemTradeManager::GetItemAllTradeInfo( IN const GCITEMID dwItemID_, OUT KDropItemInfo& dwRewardItem_ )
{
    std::map< DWORD, KDropItemInfo >::iterator mit;
    KLocker lock( m_csItemTrade );
    mit = m_mapAllTradeInfo.find( dwItemID_ );
    if( mit == m_mapAllTradeInfo.end() ) return false;
    dwRewardItem_ = mit->second;
    return true;
}

void KItemTradeManager::GetItemTradeListNotice( OUT std::map< std::pair<DWORD,int>, KDropItemInfo >& mapItemTradeInfo_, OUT std::map<DWORD, KDropItemInfo>& mapItemAllTradeInfo_ )
{
    KLocker lock( m_csItemTrade );
    mapItemTradeInfo_ = m_mapTradeInfo;
    mapItemAllTradeInfo_ = m_mapAllTradeInfo;
}