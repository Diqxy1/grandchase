#include "GetLimit.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include <kncutil.h>
#include "Log4.h"

ImplementSingleton( KGetLimit );
ImplOstmOperatorW2A( KGetLimit );
NiImplementRootRTTI( KGetLimit );

KGetLimit::KGetLimit(void)
{
}

KGetLimit::~KGetLimit(void)
{
}

ImplToStringW( KGetLimit )
{
    KLocker lock( m_csGetLimit );
    return START_TOSTRINGW
        << TOSTRINGWt( CTime( m_tmBegin ) )
        << TOSTRINGWt( CTime( m_tmEnd ) )
        << TOSTRINGW( m_mapGetLimitInfo.size() );
}

bool KGetLimit::LoadScript()
{
    return _LoadScript( std::string() );
}

bool KGetLimit::_LoadScript( OUT std::string& strScript_ )
{
    time_t      tmBegin;
    time_t      tmEnd;
    std::map< PAIR_INT, std::vector< KGetLimitInfo > > mapGetLimitInfo;

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitGetLimit.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );

    _JIF( LoadGetLimit( kLuaMng, std::string("GetLimit"), mapGetLimitInfo ), return false );

    START_LOG( cerr, L"획득 제한 이벤트 스크립트 로드 완료. " ) << END_LOG;
    {
        KLocker lock( m_csGetLimit );

        m_tmBegin = tmBegin;
        m_tmEnd = tmEnd;
        m_mapGetLimitInfo.swap( mapGetLimitInfo );
    }

    return true;
}

bool KGetLimit::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nYear, nMonth, nDay, nHour;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    if ( 0 == nYear || 0 == nMonth || 0 == nDay // 스크립트에서 0으로 설정하면 서버 죽음
        || nMonth > 12 || nDay > 31 ) { // 월, 일 최대값 검사
            START_LOG( cerr, L"아이템 획득제한 스크립트 날짜 정보 오류" ) << END_LOG;
            return false;
    }

    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, 0, 0 ) );
    return true;
}

bool KGetLimit::IsRun()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csGetLimit );

    return ( m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent );
}

bool KGetLimit::LoadGetLimit( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< PAIR_INT, std::vector< KGetLimitInfo > >& mapGetLimitInfo_ )
{
    mapGetLimitInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        PAIR_INT prKey;
        std::vector< KGetLimitInfo > vecGetLimitInfo;

        _JIF( kLuaMng_.GetValue( "ModeID", prKey.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Difficulty", prKey.second ) == S_OK, return false );

        _JIF( LoadCheckItemList( kLuaMng_, std::string("CheckItemList"), vecGetLimitInfo ), return false );

        if ( false == mapGetLimitInfo_.insert( std::make_pair( prKey, vecGetLimitInfo ) ).second ) {
            START_LOG( cerr, L"중복된 (ModeID, Difficulty) 설정이 있음. ModeID: " << prKey.first << ", Difficulty: " << prKey.second ) << END_LOG;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KGetLimit::LoadCheckItemList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KGetLimitInfo >& vecGetLimitInfo_ )
{
    vecGetLimitInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true ) {
        KGetLimitInfo kGetLimitInfo;
        if ( kLuaMng_.GetValue( i++, kGetLimitInfo.m_dwItemID ) != S_OK ) {
            break;
        }
        _JIF( kLuaMng_.GetValue( i++, kGetLimitInfo.m_dwLimit ) == S_OK, return false );

        vecGetLimitInfo_.push_back( kGetLimitInfo );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KGetLimit::GetGetLimitInfo( IN PAIR_INT& prKey_, OUT std::vector< KGetLimitInfo >& vecGetLimitInfo_ )
{
    vecGetLimitInfo_.clear();

    KLocker lock( m_csGetLimit );

    if ( true == m_mapGetLimitInfo.empty() ) {
        return false;
    }

    std::map< PAIR_INT, std::vector< KGetLimitInfo > >::iterator mitGetLimitInfo;
    mitGetLimitInfo = m_mapGetLimitInfo.find( prKey_ );
    if ( mitGetLimitInfo == m_mapGetLimitInfo.end() ) {
        return false;
    }
    vecGetLimitInfo_ = mitGetLimitInfo->second;

    return true;
}
