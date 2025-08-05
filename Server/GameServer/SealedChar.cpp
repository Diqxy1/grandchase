#include "SealedChar.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "UserEvent.h"
#include "Log4.h"

ImplementSingleton( KSealedChar );
ImplOstmOperatorW2A( KSealedChar );
NiImplementRootRTTI( KSealedChar );

KSealedChar::KSealedChar( void )
{
    m_mapSealedCharRawInfo.clear();
}

KSealedChar::~KSealedChar( void )
{
}

ImplToStringW( KSealedChar )
{
    KLocker lock( m_csSealedChar );

    START_TOSTRINGW
        << TOSTRINGW( m_mapSealedCharRawInfo.size() );

    {
        std::map< int, KSealedCharRawInfo >::const_iterator mit = m_mapSealedCharRawInfo.begin();

        for ( ; mit != m_mapSealedCharRawInfo.end(); ++mit ) {
            stm_ << L"nCharType: " << mit->second.m_nCharType
                << L"\nnObjectLevel: " << mit->second.m_nObjectLevel
                << L"\ntmBegin: " << ( LPCWSTR )CTime( mit->second.m_tmBegin ).Format( KNC_TIME_FORMAT )
                << L"\ntmEnd: " << ( LPCWSTR )CTime( mit->second.m_tmEnd).Format( KNC_TIME_FORMAT ) << std::endl;
        }
    }

    return stm_;
}

bool KSealedChar::LoadScript()
{
    return _LoadScript(std::string());
}

bool KSealedChar::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "InitSealedChar.lua";
    strScript_ = strScriptName;
    
    std::map< int, KSealedCharRawInfo > sealedCharRawInfo;

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( LoadSealedCharRawInfo( kLuaMng, std::string("SealedCharInfo"), sealedCharRawInfo ), return false );

    {
        KLocker lock( m_csSealedChar );

        m_mapSealedCharRawInfo.swap( sealedCharRawInfo );
    }

    START_LOG( cerr, L"봉인 캐릭터 스크립트 읽기 완료." )
        << END_LOG;

    return true;
}

bool KSealedChar::LoadSealedCharRawInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, KSealedCharRawInfo >& mapSealedCharRawInfo_ )
{
    mapSealedCharRawInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KSealedCharRawInfo kSealedCharRawInfo;

        _JIF( kLuaMng_.GetValue( "CharType", kSealedCharRawInfo.m_nCharType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "ObjectLevel", kSealedCharRawInfo.m_nObjectLevel) == S_OK, return false );
        _JIF( LoadTime( kLuaMng_, std::string( "BeginTime" ), kSealedCharRawInfo.m_tmBegin ), return false );
        _JIF( LoadTime( kLuaMng_, std::string( "EndTime" ), kSealedCharRawInfo.m_tmEnd ), return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( false == mapSealedCharRawInfo_.insert( std::make_pair( kSealedCharRawInfo.m_nCharType, kSealedCharRawInfo ) ).second ) {
            START_LOG( cerr, L"봉인 캐릭터 스크립트에 중복된 캐릭터 정보 존재함: " << kSealedCharRawInfo.m_nCharType )
                << END_LOG;
            return false;
        }
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KSealedChar::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    tmTime_ = 0;

    int nYear = 0, nMonth = 0, nDay = 0, nHour = 0;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    if ( 0 == nYear || 0 == nMonth || 0 == nDay || nMonth > 12 || nDay > 31 ) {
        START_LOG( cerr, L"봉인 캐릭터 스크립트에 잘못된 날짜 정보 존재함" )
            << END_LOG;
        return false;
    }

    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, 0, 0 ) );

    return true;
}

bool KSealedChar::IsEventTerm( IN int& nCharType_ )
{
    KLocker lock( m_csSealedChar );

    std::map< int, KSealedCharRawInfo >::iterator mit;
    mit = m_mapSealedCharRawInfo.find( nCharType_ );
    if ( mit != m_mapSealedCharRawInfo.end() ) {
        time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
        bool bReturn = ( mit->second.m_tmBegin <= tmCurrent && mit->second.m_tmEnd >= tmCurrent );

        if ( bReturn ) {
            START_LOG( clog, L"봉인 캐릭터 이벤트 진행중. charType: " << nCharType_ ) << END_LOG;
        }

        return bReturn;
    }
    return false;
}

bool KSealedChar::GetObjectLevel( IN int& nCharType_, OUT int& nObjectLevel_ )
{
    KLocker lock( m_csSealedChar );

    nObjectLevel_ = 0;
    std::map< int, KSealedCharRawInfo >::iterator mit;
    mit = m_mapSealedCharRawInfo.find( nCharType_ );
    if ( mit != m_mapSealedCharRawInfo.end() ) {
         nObjectLevel_ = mit->second.m_nObjectLevel;
         return true;
    }

    return false;
}

bool KSealedChar::GetObjectTime( IN int& nCharType_, OUT time_t& tmObjectTime_ )
{
    KLocker lock( m_csSealedChar );

    tmObjectTime_ = 0;
    std::map< int, KSealedCharRawInfo >::iterator mit;
    mit = m_mapSealedCharRawInfo.find( nCharType_ );
    if ( mit != m_mapSealedCharRawInfo.end() ) {
        tmObjectTime_ = mit->second.m_tmEnd;
        return true;
    }

    return false;
}
