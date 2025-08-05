#include "Socks.h"

#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include <kncutil.h>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KSocks );
ImplOstmOperatorW2A( KSocks );
NiImplementRootRTTI( KSocks );

KSocks::KSocks(void)
:m_tmBegin(1)
,m_tmEnd(1)
{
}

KSocks::~KSocks(void)
{

}

ImplToStringW( KSocks )
{
    KLocker lock( m_csSocks );
    return START_TOSTRINGW
        << TOSTRINGWt( CTime( m_tmBegin ) )
        << TOSTRINGWt( CTime( m_tmEnd ) );

}

bool KSocks::LoadScript()
{
    return _LoadScript(std::string());
}

bool KSocks::_LoadScript( OUT std::string& strScript_ )
{
    time_t      tmBegin;
    time_t      tmEnd;
    KLuaManager kLuaMng;
    std::map<GCITEMID, KSocksInfo> mapSocks;
    std::map< GCITEMID, Kairo > mapSocksReward;

    const std::string strScriptName = "InitSocks.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    // 이벤트 기간 읽어오기.
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );

    _JIF( LoadSocks( kLuaMng, std::string("Socks"), mapSocks, mapSocksReward ), return false );

    KLocker lock( m_csSocks );

    m_tmBegin = tmBegin;
    m_tmEnd = tmEnd;
    m_mapSocks.swap( mapSocks );
    m_mapSocksReward.swap( mapSocksReward );

    return true;
}

bool KSocks::LoadSocks( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<GCITEMID, KSocksInfo>& mapSocks_, OUT std::map< GCITEMID, Kairo >& mapSocksReward_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    
    for ( int i = 1; ; ++i ) {
        
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KDropItemInfo kSocks;
        std::pair< GCITEMID, int > prMaterialID; // id and count
        DWORD dwTime;

        Kairo kairo;
        VEC_REWARD vecItemInfo;

        //_JIF( kLuaMng_.GetValue( "Socks_ID", socksID ) == S_OK, return false );
        _JIF( LoadSocksInfo( kLuaMng_, std::string("Socks_ID"), kSocks ), return false );
        _JIF( LoadMaterialItem( kLuaMng_, std::string("Material_ID"), prMaterialID ), return false );
        _JIF( kLuaMng_.GetValue( "Time", dwTime ) == S_OK, return false );

        _JIF( kLuaMng_.BeginTable( "ItemList" ) == S_OK, return false );
        _JIF( LoadRewardList( kLuaMng_, vecItemInfo, kairo ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        KSocksInfo socksInfo;
        socksInfo.Clear();

        socksInfo.m_prMaterialID = prMaterialID;
        socksInfo.m_dwPeriodSec = dwTime;
        socksInfo.m_vecItemList.swap( vecItemInfo );
        socksInfo.m_Socks = kSocks;

        if ( !mapSocks_.insert( std::make_pair( kSocks.m_ItemID, socksInfo ) ).second ) {
            START_LOG( cerr, L"중복된 양말 정보 있음. SocksID : " << kSocks.m_ItemID ) << END_LOG;
        }
        if ( !mapSocksReward_.insert( std::make_pair( kSocks.m_ItemID, kairo ) ).second ) {
            START_LOG( cerr, L"중복된 양말 보상 정보 있음. SocksID : " << kSocks.m_ItemID ) << END_LOG;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KSocks::LoadMaterialItem( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::pair< GCITEMID, int >& prItem_ )
{   
    GCITEMID ItemID;
    int nCount;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, ItemID ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nCount ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    _JIF( nCount != 0, return false ); // 필요 양말 재료는 0개 불가능

    std::pair< GCITEMID, int > prPointItem = std::make_pair( ItemID, nCount );
    prItem_.swap( prPointItem );
    return true;
}

bool KSocks::LoadSocksInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT KDropItemInfo& socksItem_ )
{   
    KDropItemInfo kSocksItem;

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, kSocksItem.m_ItemID ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, kSocksItem.m_nDuration ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, kSocksItem.m_nPeriod ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    socksItem_ = kSocksItem;
    
    return true;
}

bool KSocks::LoadRewardList( IN KLuaManager& kLuaMng_, OUT VEC_REWARD& vecReward_, OUT Kairo& kairo_ )
{
    vecReward_.clear();
    int nOuterIndex = 0;

    while( true )
    {
        if ( kLuaMng_.BeginTable( ++nOuterIndex ) != S_OK ) {
            break;
        }
        KDropItemInfo kReward;
        float fProb = 0.f;
        int nIndex = 0;

        if( kLuaMng_.GetValue( ++nIndex, kReward.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( ++nIndex, kReward.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( ++nIndex, kReward.m_nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( ++nIndex, fProb ) == S_OK, return false ); // 확률

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        vecReward_.push_back( kReward );

        _JIF( kairo_.SetPostItem( kReward.m_ItemID, kReward.m_nDuration, kReward.m_nPeriod, fProb ), return false );        
    }
    return true;
}

bool KSocks::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

bool KSocks::IsRun()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csSocks );
    return ( m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent );
}

void KSocks::GetSocksList( OUT std::vector< GCITEMID >& vecSocks_ )
{
    KLocker lock( m_csSocks );
    vecSocks_.clear();
    std::map<GCITEMID, KSocksInfo>::const_iterator mit;
    for ( mit = m_mapSocks.begin() ; mit != m_mapSocks.end() ; ++mit ) {
        vecSocks_.push_back( mit->first );
    }
}

void KSocks::GetSocksMaterialList( OUT std::map< GCITEMID, std::pair< GCITEMID, int > >& mapMaterial_ )
{
    KLocker lock( m_csSocks );
    mapMaterial_.clear();
    std::map<GCITEMID, KSocksInfo>::const_iterator mit;
    for ( mit = m_mapSocks.begin() ; mit != m_mapSocks.end() ; ++mit ) {
        mapMaterial_.insert( std::make_pair( mit->first, mit->second.m_prMaterialID ) );
    }
}

void KSocks::GetSocksInfo( IN const GCITEMID itemID_, OUT KSocksInfo& socksInfo_ )
{
    KLocker lock( m_csSocks );
    std::map<GCITEMID, KSocksInfo>::const_iterator mit;
    mit = m_mapSocks.find( itemID_ );
    if ( mit == m_mapSocks.end() ) {
        return;
    }

    socksInfo_.m_Socks = mit->second.m_Socks;
    //socksInfo_.m_vecItemList = mit->second.m_vecItemList;
}

DWORD KSocks::GetSocksSecond( IN const GCITEMID itemID_ )
{
    KLocker lock( m_csSocks );
    std::map<GCITEMID, KSocksInfo>::const_iterator mit;
    mit = m_mapSocks.find( itemID_ );
    if ( mit == m_mapSocks.end() ) {
        return 0;
    }
    
    return mit->second.m_dwPeriodSec;
}

void KSocks::CountdownRemainTime( IN OUT std::map< GCITEMID, KSocksInfo >& mapSocksInfo_,
                                 IN std::map< DWORD, bool >& mapSocksTime_ )
{
    std::map< GCITEMID, KSocksInfo >::iterator mit;
    // 양말의 남은 시간(초) 을 1초 감소하기
    for ( mit = mapSocksInfo_.begin(); mit != mapSocksInfo_.end(); ++mit ) {

        if ( mit->second.m_nIsHangup != KSocksInfo::HANGUP ) {
            continue;
        }
        if ( mit->second.m_dwRemainSec > 0 ) {// 양말의 남은 시간(초) 을 1초 감소하기
            mit->second.m_dwRemainSec -= 1; // 단위 : 1초
            mapSocksTime_[mit->first] = true; // 시간 감소된 양말 표시 - 접속시 이미 남은 시간이 0 인 양말을 가려내기
        }
    }
}

void KSocks::CheckSocksDone( IN OUT std::map< GCITEMID, KSocksInfo >& mapSocksInfo_,
                            OUT std::vector< GCITEMID >& vecCompleteSocks_,
                            IN const std::map< DWORD, bool >& mapSocksTime_ )
{
    KLocker lock( m_csSocks );
    std::map< GCITEMID, KSocksInfo >::iterator mit;
    for ( mit = mapSocksInfo_.begin(); mit != mapSocksInfo_.end(); ++mit ) {

        if ( mit->second.m_nIsHangup == KSocksInfo::HANGUP &&
            mit->second.m_dwRemainSec == 0 ) {

            std::map< DWORD, bool >::const_iterator mitTime = mapSocksTime_.find( mit->first );
            if ( mitTime == mapSocksTime_.end() ) {
                continue;
            }
            if ( mitTime->second == true ) {
                vecCompleteSocks_.push_back( mit->second.m_Socks.m_ItemID );
            }
        }
    }
}

void KSocks::GetRewardItem( IN const GCITEMID itemID_, IN OUT std::vector<KDropItemInfo>& vecItemList_ )
{
    KLocker lock( m_csSocks );
    
    vecItemList_.clear();

    std::map< GCITEMID, Kairo >::const_iterator mit;
    mit = m_mapSocksReward.find( itemID_ );

    if ( mit != m_mapSocksReward.end() ) {
        KDropItemInfo kItem;
        mit->second.Do( kItem );
        vecItemList_.push_back( kItem );

        START_LOG( clog, L"양말 카이로 보상" )
            << BUILD_LOG( kItem.m_ItemID )
            << BUILD_LOG( kItem.m_nDuration ) << END_LOG;
    }
}

void KSocks::GetSocksScript( OUT std::map< GCITEMID, KSocksInfo >& SocksInfo_ )
{
    KLocker lock( m_csSocks );
    SocksInfo_ = m_mapSocks;
}