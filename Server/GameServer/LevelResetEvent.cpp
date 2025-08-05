#include "LevelResetEvent.h"

#include <dbg/dbg.hpp>
#include "Lua/KLuaManager.h"
#include "Log4.h"

ImplementSingleton( KLevelResetEvent );
ImplOstmOperatorW2A( KLevelResetEvent );
NiImplementRootRTTI( KLevelResetEvent );


KLevelResetEvent::KLevelResetEvent(void)
:m_tmBegin(1)
,m_tmEnd(1)
,m_nLevel(0)
,m_dwVersion(0)
{
    m_mapGPResetItemList.clear();
    m_mapCashResetItemList.clear();

    m_mapCashResetItemList.clear();
    m_mapGPRewardItemList.clear();
}

KLevelResetEvent::~KLevelResetEvent(void)
{
}

ImplToStringW( KLevelResetEvent )
{
    KLocker lock( m_csLevelResetEvent );

    return START_TOSTRINGW
        << TOSTRINGWt( CTime( m_tmBegin ) )
        << TOSTRINGWt( CTime( m_tmEnd ) );
}

bool KLevelResetEvent::LoadScript()
{
    return _LoadScript(std::string());
}

bool KLevelResetEvent::_LoadScript( OUT std::string& strScript_ )
{
    time_t tmBegin;
    time_t tmEnd;
    KLuaManager kLuaMng;
    
    int nLevel = 0;
    DWORD dwVersion = 0;
    std::map<int,GCITEMID>  mapGPResetItemList;
    std::map<int,GCITEMID>  mapCashResetItemList;

    std::map<int,VEC_REWARD>  mapGPRewardItemList;
    std::map<int,VEC_REWARD>  mapCashRewardItemList;

    const std::string strScriptName = "InitLevelResetEvent.lua";
    strScript_ = strScriptName;

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "VERSION", dwVersion ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    _JIF( kLuaMng.GetValue( "Reward_Level", nLevel ) == S_OK, return false );
    
    
    _JIF( LoadRewardList( kLuaMng, std::string( "RewardItemListGP" ), mapGPRewardItemList ), return false );
    _JIF( LoadRewardList( kLuaMng, std::string( "RewardItemListCash" ), mapCashRewardItemList ), return false );
    
    
    _JIF( LoadResetItemList( kLuaMng, std::string( "ResetItemListGP" ), mapGPResetItemList ), return false );
    _JIF( LoadResetItemList( kLuaMng, std::string( "ResetItemListCash" ), mapCashResetItemList ), return false );

    


    KLocker lock( m_csLevelResetEvent );
    m_dwVersion = dwVersion;
    m_tmBegin = tmBegin;
    m_tmEnd = tmEnd;
    m_nLevel = nLevel;
    
    m_mapGPResetItemList.swap( mapGPResetItemList );   
    m_mapCashResetItemList.swap( mapCashResetItemList );   

    m_mapGPRewardItemList.swap( mapGPRewardItemList );
    m_mapCashRewardItemList.swap( mapCashRewardItemList );

    return true;
}

bool KLevelResetEvent::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{ // DicePlay 에서 가져옴
    int nYear, nMonth, nDay, nHour;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, 0, 0 ) );
    return true;
}


bool KLevelResetEvent::IsRun()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csLevelResetEvent );
    return ( m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent );
}

bool KLevelResetEvent::LoadResetItemList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT OUT std::map<int,GCITEMID>&  mapResetItemList_ )
{
    mapResetItemList_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int nOuterIndex = 0;
   
    while( true )
    {
        if ( kLuaMng_.BeginTable( ++nOuterIndex ) != S_OK ) {
            break;
        }

        int nCharType = 0;
        GCITEMID itemID = 0;
        int nIndex = 0;

        if( kLuaMng_.GetValue( ++nIndex, nCharType ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( ++nIndex, itemID ) == S_OK, return false );
        
        
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        
        mapResetItemList_.insert( std::make_pair( nCharType, itemID ) );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KLevelResetEvent::LoadRewardList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<int,VEC_REWARD>& mapRewardList_ )
{
    mapRewardList_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int nOuterIndex1 = 0;

    while ( true ) 
    {
        if ( kLuaMng_.BeginTable( ++nOuterIndex1 ) != S_OK ) {
            break;
        }

        int nCharType = 0;

        if( kLuaMng_.GetValue( "CharType", nCharType ) != S_OK ) break;

        if( kLuaMng_.BeginTable( "ItemList" ) != S_OK ) break;

        int nOuterIndex2 = 0;
        VEC_REWARD vecRewardList;
    
    while( true )
    {
            if ( kLuaMng_.BeginTable( ++nOuterIndex2 ) != S_OK ) {
            break;
        }

        int nIndex = 0;
        
        KDropItemInfo kData;       
        if( kLuaMng_.GetValue( ++nIndex, kData.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( ++nIndex, kData.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( ++nIndex, kData.m_nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

            vecRewardList.push_back( kData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( vecRewardList.empty() )
            return false;

        mapRewardList_.insert( std::make_pair( nCharType, vecRewardList ) );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KLevelResetEvent::GetRewardList( OUT std::map<int,VEC_REWARD>& mapGPRewardItemList, OUT std::map<int,VEC_REWARD>& mapCashRewardItemList )   
{
    mapGPRewardItemList = m_mapGPRewardItemList;
    mapCashRewardItemList = m_mapCashRewardItemList;
}

void KLevelResetEvent::GetResetItemList( OUT std::map<int,GCITEMID>& mapGPResetItemList, OUT std::map<int,GCITEMID>& mapCashResetItemList )
{
    mapGPResetItemList = m_mapGPResetItemList;
    mapCashResetItemList = m_mapCashResetItemList;
}

void KLevelResetEvent::GetCharRewardList( IN const char& cChar, IN const bool& bCashType, OUT std::vector<KDropItemInfo>& vecRewardItemList ) 
{
    vecRewardItemList.clear();

    if ( bCashType ) 
    {
        std::map<int,VEC_REWARD>::iterator pos = m_mapCashRewardItemList.find( cChar );
        if ( pos == m_mapCashRewardItemList.end() ) 
            return;

       vecRewardItemList = pos->second;
       return;
    }
    else
    {
        std::map<int,VEC_REWARD>::iterator pos = m_mapGPRewardItemList.find( cChar );
        if ( pos == m_mapGPRewardItemList.end() ) 
            return;

        vecRewardItemList = pos->second;
        return;
    }
}

bool KLevelResetEvent::IsResetItemID( IN const char& cCharType, IN const GCITEMID& itemID, OUT bool& bCashType )
{
    bCashType = false;

    if ( m_mapCashResetItemList.empty() && m_mapGPResetItemList.empty() ) 
        return false;

    std::map<int,GCITEMID>::iterator iter = m_mapCashResetItemList.find( cCharType );
    if ( iter != m_mapCashResetItemList.end() )
    {
        if ( iter->second == itemID ) 
        {
            bCashType = true;
            return true;
        }
    }

    std::map<int,GCITEMID>::iterator iter2 = m_mapGPResetItemList.find( cCharType );
    if ( iter2 != m_mapGPResetItemList.end() )
    {
        if ( iter2->second == itemID ) 
        {
            bCashType = false;
            return true;
        }
    }

    return false;
}

GCITEMID KLevelResetEvent::GetResetItemID( IN const char& cCharType, IN const bool& bCashType )
{
    if ( bCashType ) 
    {
        if ( m_mapCashResetItemList.empty() )
        return 0;

        std::map<int,GCITEMID>::iterator iter = m_mapCashResetItemList.find( cCharType );
        if ( iter == m_mapCashResetItemList.end() )
        return 0;

    return iter->second;
    }
    else
    {
        if ( m_mapGPResetItemList.empty() )
            return 0;

        std::map<int,GCITEMID>::iterator iter = m_mapGPResetItemList.find( cCharType );
        if ( iter == m_mapGPResetItemList.end() )
            return 0;

        return iter->second;
    }
}
