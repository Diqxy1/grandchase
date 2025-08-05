#include "BlockIP.h"
#include <dbg/dbg.hpp>
#include "Lua/KLuaManager.h"
#include <algorithm>
#include "SOCKET/NetCommon.h"
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include "NetError.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

#define OUTBOUND "OUTBOUND"
#define INBOUND  "INBOUND"

//pair<DWORD,DWORD>데이터 처리를 위한 sort와 binary_search용 사용자 정의 comp
namespace
{
    class KComp
    {
    public:
        bool operator() ( const KBlockIPList::sIPRange& ip1, const KBlockIPList::sIPRange& ip2 )
        {
            if( ip1.m_dwBegin < ip2.m_dwBegin ) return true;
            return false;
        }

        bool operator() ( const DWORD dwVal, const KBlockIPList::sIPRange& ip)
        {
            if( dwVal < ip.m_dwBegin && dwVal < ip.m_dwEnd )
                return true;
            return false;
        }

        bool operator() ( const KBlockIPList::sIPRange& ip, const DWORD dwVal )
        {
            if( dwVal > ip.m_dwBegin && dwVal > ip.m_dwEnd )
                return true;
            return false;
        }
    };
};

ImplementSingleton( KBlockIPList );
NiImplementRootRTTI( KBlockIPList );

KBlockIPList::KBlockIPList(void)
:m_bCheckEnable( false )
,m_bPacthTime( false )
,m_bAdminIPCheckEnable( false )
{
    m_vecBlockIPList.clear();
    m_setExceptUserID.clear();
}

KBlockIPList::~KBlockIPList(void)
{
}


ImplOstmOperatorW2A( KBlockIPList );
ImplToStringW( KBlockIPList )
{
    KLocker lock( m_csBlockIPList );
    START_TOSTRINGW
        << TOSTRINGWb( m_bCheckEnable )
        << TOSTRINGWb( m_bPacthTime )
        << TOSTRINGWb( m_bAdminIPCheckEnable )
        << TOSTRINGW( m_vecBlockIPList.size() )
        << TOSTRINGW( m_vecExceptIPList.size() )
        << TOSTRINGW( m_vecAdminIPList.size() )
        << TOSTRINGW( m_mapConditionTable.size() )
        << TOSTRINGW( m_setBlockCountryList.size() )
        << TOSTRINGW( m_setExceptUserID.size() );

    DumpCondition( stm_ );
    DumpException( stm_ );
    DumpAdminIP( stm_ );
    return stm_;
}

void KBlockIPList::DumpCondition( std::wostream& stm_ ) const
{
    if( m_mapConditionTable.empty() )
    {
        stm_ << L"Empty...." << std::endl;
        return;
    }

    stm_ << L" --- Condition --- " << std::endl;
    std::map<std::string, KBlockIPList::sCondition>::const_iterator mit;
    for( mit = m_mapConditionTable.begin() ; mit != m_mapConditionTable.end() ; ++mit )
    {
        stm_ << KncUtil::toWideString(mit->second.m_strCC) << L", "
            << (mit->second.m_bOldUser?L"true":L"false") << L", "
            << (mit->second.m_bBlock?L"true":L"false") << std::endl;
    }
}

void KBlockIPList::DumpException( std::wostream& stm_ ) const
{
    if( m_mapConditionTable.empty() )
    {
        stm_ << L"Empty...." << std::endl;
        return;
    }

    stm_ << L" --- Exception --- " << std::endl;

    std::vector< sIPRange >::const_iterator vit;
    for( vit = m_vecExceptIPList.begin() ; vit != m_vecExceptIPList.end() ; ++vit )
    {
        stm_ << KncUtil::toWideString(vit->m_strCC) << L" : "
            << L"{" << KncUtil::toWideString( DWORDToIPStr(vit->m_dwBegin) ) << L"} ~ " 
            << L"{" << KncUtil::toWideString( DWORDToIPStr(vit->m_dwEnd) ) << L"}" << std::endl;
    }

}

void KBlockIPList::DumpAdminIP( std::wostream& stm_ ) const
{
    if( m_mapConditionTable.empty() )
    {
        stm_ << L"Empty...." << std::endl;
        return;
    }

    stm_ << L" --- Admin IP --- " << std::endl;

    std::vector< sIPRange >::const_iterator vit;
    for( vit = m_vecAdminIPList.begin() ; vit != m_vecAdminIPList.end() ; ++vit )
    {
        stm_ << KncUtil::toWideString(vit->m_strCC) << L" : "
            << L"{" << KncUtil::toWideString( DWORDToIPStr(vit->m_dwBegin) ) << L"} ~ " 
            << L"{" << KncUtil::toWideString( DWORDToIPStr(vit->m_dwEnd) ) << L"}" << std::endl;
    }
}

bool KBlockIPList::LoadScript()
{
    DWORD dwElTime = ::GetTickCount();
    bool bCheckEnable = false;
    bool bPatchTime = false;
    bool bAdminIPCheck = false;
    std::vector< sIPRange >             vecBlockIPList;
    std::vector< sIPRange >             vecExceptIPList;
    std::vector< sIPRange >             vecAdminIPList;
    std::map<std::string, sCondition>   mapConditionTable;
    std::set<std::string>               setBlockCountryList;
    std::set<std::string>              setExceptUserID;
    vecBlockIPList.clear();
    vecExceptIPList.clear();
    vecAdminIPList.clear();
    bool bExceptUserID = false;

    std::string strBlockFile;
    KLuaManager kLuaMng;
    _JIF( kLuaMng.DoFile( "BlockIPList.lua" ) == S_OK, return false );
    _JIF( kLuaMng.BeginTable( "IPList" ) == S_OK, return false );

    _JIF( kLuaMng.GetValue( "IsPatchTime", bPatchTime ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "CheckEnable", bCheckEnable ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "AdminIPCheck", bAdminIPCheck ) == S_OK, return false );

    _JIF( LoadCondition( kLuaMng, "Condition", mapConditionTable ), return false );

    _JIF( kLuaMng.GetValue( "IPBlockFile", strBlockFile ) == S_OK, return false );
    _JIF( LoadBinIpRange( strBlockFile, vecBlockIPList ), return false ); 

    _JIF( kLuaMng.BeginTable( "ExceptIPRange" ) == S_OK, return false );
    _JIF( LoadIpRange( kLuaMng, vecExceptIPList ), return false );    
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "AdminIPRange" ) == S_OK, return false );
    _JIF( LoadIpRange( kLuaMng, vecAdminIPList ), return false );
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    _JIF( LoadBlockCountry( kLuaMng, std::string("BlockCountryCode"), setBlockCountryList ), return false );

    _JIF( kLuaMng.GetValue( "CheckExceptionUserID", bExceptUserID ) == S_OK, return false );
    _JIF( LoadExceptUserID( kLuaMng, std::string("ExceptionUserID"), setExceptUserID ), return false );

    _JIF( kLuaMng.EndTable() == S_OK, return false );

    std::sort( vecBlockIPList.begin(), vecBlockIPList.end(), KComp() );
    std::sort( vecExceptIPList.begin(), vecExceptIPList.end(), KComp() );
    std::sort( vecAdminIPList.begin(), vecAdminIPList.end(), KComp() );

    START_LOG( cerr, L"Blocked IP List Size : " << vecBlockIPList.size() << L", ElapsedTime : " << ::GetTickCount() - dwElTime ) << END_LOG;

    KLocker lock(m_csBlockIPList);
    m_bPacthTime = bPatchTime;
    m_bAdminIPCheckEnable = bAdminIPCheck;
    m_bCheckEnable = bCheckEnable;
    m_vecBlockIPList.swap( vecBlockIPList );
    m_vecExceptIPList.swap( vecExceptIPList );
    m_vecAdminIPList.swap( vecAdminIPList );
    m_mapConditionTable.swap( mapConditionTable );
    m_setBlockCountryList.swap( setBlockCountryList );
    m_setExceptUserID.swap( setExceptUserID );
    m_bExceptUserID = bExceptUserID;

    return true;
}

DWORD KBlockIPList::IPStrToDWORD( IN const std::string& strIP_ )
{
    return ::ntohl( inet_addr( strIP_.c_str() ) );
}

std::string KBlockIPList::DWORDToIPStr( IN const DWORD dwIP_ ) const
{
    DWORD dwaIP[4];
    dwaIP[0] = dwIP_ >> 24;

    dwaIP[1] = dwIP_ << 8;
    dwaIP[1] = dwaIP[1] >> 24;

    dwaIP[2] = dwIP_ << 16;
    dwaIP[2] = dwaIP[2] >> 24;

    dwaIP[3] = dwIP_ << 24;
    dwaIP[3] = dwaIP[3] >> 24;

    std::stringstream stm;
    stm << dwaIP[0] << "."
        << dwaIP[1] << "."
        << dwaIP[2] << "."
        << dwaIP[3];

    return stm.str();
}

bool KBlockIPList::LoadCondition( IN KLuaManager& kLuaMng_, IN const char* szTable_, OUT std::map<std::string, sCondition>& mapConditionTable_ )
{
    mapConditionTable_.clear();
    _JIF( szTable_ != NULL, return false );
    _JIF( kLuaMng_.BeginTable( szTable_ ) == S_OK, return false );
    for( int i = 1 ;; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        sCondition condition;
        _JIF( kLuaMng_.GetValue( 1, condition.m_strCC ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, condition.m_bOldUser ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, condition.m_bBlock ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        boost::to_upper( condition.m_strCC );
        boost::trim( condition.m_strCC );

        if( !mapConditionTable_.insert( std::make_pair( condition.m_strCC, condition ) ).second )
        {
            START_LOG( cerr, L"조건 테이블에 중복된 국가 코드가 있습니다." )
                << BUILD_LOG( KncUtil::toWideString(condition.m_strCC) ) << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KBlockIPList::LoadIpRange( IN KLuaManager& kLuaMng_, OUT std::vector< sIPRange >& vecIPRange_ )
{
    vecIPRange_.clear();
    for( int i = 1 ;; ++i )
    {
        std::string strFirst;
        std::string strSecond;
        sIPRange ipRange;
        if( kLuaMng_.BeginTable( i ) != S_OK) break;
        _JIF( kLuaMng_.GetValue( 1, strFirst ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, strSecond ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, ipRange.m_strCC ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        ipRange.m_dwBegin = IPStrToDWORD( strFirst );
        ipRange.m_dwEnd   = IPStrToDWORD( strSecond );
        vecIPRange_.push_back( ipRange );
    }
    return true;
}

bool KBlockIPList::LoadBinIpRange( IN std::string& IpRangeFile_, OUT std::vector< sIPRange >& vecIPRange_ )
{
    std::ifstream inFile;
    inFile.open(IpRangeFile_.c_str(), std::ios_base::binary | std::ios_base::in );
    if( !inFile.good() || inFile.eof() || !inFile.is_open() ) return false;

    char szBuff[64]={0,};
    while( !inFile.eof() )
    {
        ::memset( szBuff, 0, sizeof( szBuff) );
        inFile.getline( szBuff, 64 );
        std::string strRead( szBuff );
        if( strRead.empty() ) break;

        typedef boost::tokenizer<boost::char_separator<char,std::string::traits_type>,std::string::const_iterator,std::string > TOKENIZER;
        boost::char_separator<char,std::string::traits_type> sep( "|" );
        TOKENIZER tokens(strRead, sep);
        TOKENIZER::iterator tok_iter = tokens.begin();

        if( tokens.end() == tok_iter ) continue;

        sIPRange ipRange;
        std::string strTmp = *tok_iter;
        ipRange.m_dwBegin   = (DWORD)( ::_atoi64( strTmp.c_str() ) );

        ++tok_iter;
        if( tokens.end() == tok_iter ) continue;

        strTmp = *tok_iter;
        ipRange.m_dwEnd  = (DWORD)( ::_atoi64( strTmp.c_str() ) );

        ++tok_iter;
        if( tokens.end() == tok_iter ) continue;
        ipRange.m_strCC = *tok_iter;
         
        boost::to_upper( ipRange.m_strCC );
        boost::trim( ipRange.m_strCC );

        vecIPRange_.push_back( ipRange );
    }
    inFile.close();
    return true;
}

bool KBlockIPList::CheckBlockIP( IN const DWORD dwIp_, IN const bool bIsNewUser_, IN const char cAuthLevel_, IN const bool bBlockIPPass_ )
{
    // false 가 리턴 되면 블럭 되는것임..
    KLocker lock( m_csBlockIPList );

    // Admin IP 체크(false면 Block)
    if( AdminIPCheck( dwIp_, cAuthLevel_ ) == false )
    {
        START_LOG( cerr, L"Not Admin IP : " << dwIp_ ) << END_LOG;
        SET_ERROR( ERR_NUSER_VERIFY_17 );
        return false;
    }

    // 예외 리스트에 있는지(내부에서 로그 출력)
    if( CheckExceptIP( dwIp_ ) == true )
    {
        return true;
    }

    // 패치시간인지 검사(true:패치중임.모든 유저IP 블럭, false:패치아님.통과)
    if( m_bPacthTime == true )
    {
        START_LOG( cerr, L"Patch Time. Block IP : " << dwIp_ ) << END_LOG;
        SET_ERROR( ERR_NUSER_VERIFY_16 );
        return false;
    }

     // 체크 할래?
    if( !m_bCheckEnable )
    {
        START_LOG( clog, L"BlockIP Check Enable : " << m_bCheckEnable ) << END_LOG;
        return true;
    }

    // IP 블럭 체크 패스.( true : Pass, false : check )
    if( bBlockIPPass_ )
    {
        START_LOG( clog, L"BlockIP Pass? " << bBlockIPPass_ ) << END_LOG;
        return true;
    }

    // 리스트에서 검색 하자.
    std::pair<std::vector<sIPRange>::iterator, std::vector<sIPRange>::iterator > prRange;
    prRange = std::equal_range( m_vecBlockIPList.begin(), m_vecBlockIPList.end(), dwIp_, KComp() );
    if( prRange.first == prRange.second ) // 리스트에서 못찾았다면.
    {
        START_LOG( clog, L" Out Of List ..."  ) << END_LOG;
        return ConditionCheck( std::string(OUTBOUND), bIsNewUser_ );
    }

    START_LOG( clog, L"IP Range : " << KncUtil::toWideString(  DWORDToIPStr(prRange.first->m_dwBegin) ) << L"|" << KncUtil::toWideString( DWORDToIPStr(prRange.first->m_dwEnd) ) << dbg::endl
        << L"IP Range : " << prRange.first->m_dwBegin << L"|" << prRange.first->m_dwEnd << dbg::endl
        << L"Country Code : " << KncUtil::toWideString( prRange.first->m_strCC ) ) << END_LOG;

    return ConditionCheck( prRange.first->m_strCC, bIsNewUser_ );
}

bool KBlockIPList::ConditionCheck( IN const std::string& strCC_, IN const bool bNewUser_ )
{
    // false 가 리턴 되면 블럭 되는것임..
    bool bResult = false;
    std::map<std::string, KBlockIPList::sCondition>::iterator mit;

    mit = m_mapConditionTable.find( strCC_ );

    if( mit == m_mapConditionTable.end() ) // 리스트에 존재하지 않으면 다시 Inbound 조건을 적용 한다.
    {
        mit = m_mapConditionTable.find( std::string(INBOUND) );
    }

    _JIF( mit != m_mapConditionTable.end(), return false );

    bResult = mit->second.Decision( bNewUser_ );

    if( bResult == false )
    {
        START_LOG( cerr, L"Block.. Condition : " << KncUtil::toWideString( strCC_ ) << L", bNewUser : " << bNewUser_ ) << END_LOG;
        SET_ERROR( ERR_NUSER_VERIFY_14 );
        return false;
    }

    return true;
}

bool KBlockIPList::sCondition::Decision( const bool bNewUser_ )
{
    // false 가 리턴 되면 블럭 되는것임..

    if( !m_bBlock ) return true;

    // 이 아래는 막기.
    if( m_bOldUser ) return false; // 기존 유저도 막겠다.

    if( bNewUser_ )  return false; // 신규 유저만 막겠다 그러면 서 나는 신규 유저다.

    return true; // 신규 유저만 막겠다 그러면서 나는 기존 유저다.

    //  block | OLD User | IN Param | Result( TRUE는 막는것임 )
    //  -----------------------------------------
    //  TRUE  | TRUE     | TRUE     | TRUE
    //  TRUE  | TRUE     | FALSE    | TRUE
    //  TRUE  | FALSE    | TRUE     | TRUE
    //  TRUE  | FALSE    | FALSE    | FALSE
    //  FALSE | TRUE     | TRUE     | FALSE
    //  FALSE | TRUE     | FALSE    | FALSE
    //  FALSE | FALSE    | TRUE     | FALSE
    //  FALSE | FALSE    | FALSE    | FALSE
    // ex) BlockIPList.lua 파일에 설정방법
    //   {"US", "FALSE", "FALSE"} 기존유저, 신규유저 모두 오픈
    //   {"US", "FALSE", "TRUE"}  기존유저 오픈, 신규유저 블럭

    //   {"US", "TRUE", "TRUE"}   기존유저, 신규유저 모두 블럭
}

void KBlockIPList::SetCheck( bool bCheckEnable_, bool bCheckOldUser_, bool bIsBlock_ )
{
    KLocker lock( m_csBlockIPList );
    m_bCheckEnable  = bCheckEnable_;
}

void KBlockIPList::DumpIPList()
{
    KLocker lock( m_csBlockIPList );

    START_LOG( cerr, L" IP List : " ) << END_LOG;
    std::vector< sIPRange >::const_iterator vit;
    for( vit = m_vecBlockIPList.begin() ; vit != m_vecBlockIPList.end() ; ++vit )
    {
        START_LOG( cerr, L"{" << KncUtil::toWideString( DWORDToIPStr(vit->m_dwBegin) )<< L"} ~ " 
            << L"{" << KncUtil::toWideString( DWORDToIPStr(vit->m_dwEnd) )<< L"}" ) << END_LOG;
    }

}

void KBlockIPList::TestIPList( IN const char* szIP_, IN const bool bExceptChk_, IN const bool bPatchTimeChk_, IN const bool bNewUSer_, IN const int nAdminLevel_ )
{
    DWORD dwIP = IPStrToDWORD( std::string(szIP_) );
    bool bBlock = false;

    {
        KLocker lock( m_csBlockIPList );
        // 블럭리스트 IP 검사(false가 리턴되면 Block)
        bBlock = !CheckBlockIP( dwIP, bNewUSer_, (char)nAdminLevel_, false );
    }

    std::cout << ( bBlock == true ? "block":"allow") << ", IP : " << szIP_ << ", IP Num : " << dwIP << std::endl;
}

bool KBlockIPList::CheckExceptIP( IN const DWORD dwIp_ )
{
    // 예외 리스트에 있는지.
    bool bResult = std::binary_search( m_vecExceptIPList.begin(), m_vecExceptIPList.end(), dwIp_, KComp() );
    START_LOG( clog, ( bResult == true ? "Exception User":"Not Exception User") << ", IP : " << dwIp_  ) << END_LOG;

    return bResult;
}

bool KBlockIPList::AdminIPCheck( IN const DWORD dwIp_, IN const char cAuthLevel_ )
{
    // false가 리턴되면 Block

    if( m_bAdminIPCheckEnable == false )
    {
        START_LOG( clog, L"운영자IP 체크안함. AdminIP Check Enable : " << m_bAdminIPCheckEnable ) << END_LOG;
        return true;
    }

    if( cAuthLevel_ != AL_ADMIN )
    {
        START_LOG( clog, L"운영자가 아니다. AuthLevel : " << (int)cAuthLevel_ ) << END_LOG;
        return true;
    }

    // 운영자 IP 리스트에 있는지.
    bool bResult = std::binary_search( m_vecAdminIPList.begin(), m_vecAdminIPList.end(), dwIp_, KComp() );
    if( bResult == true )
    {
        START_LOG( clog, L"등록된 AdminIP이다. AuthLevel : " << (int)cAuthLevel_ << L", IP : " << dwIp_ ) << END_LOG;
        return true;
    }

    START_LOG( cerr, L"등록되지 않은 AdminIP로 운영자 계정 접속함. AuthLevel : " << (int)cAuthLevel_ << L", IP : " << dwIp_ ) << END_LOG;
    return false;
}

void KBlockIPList::CheckUserCountry( IN const DWORD dwIp_, OUT std::string& m_strCC_ )
{
    KLocker lock( m_csBlockIPList );

    // 리스트에서 검색 하자.
    std::pair<std::vector<sIPRange>::iterator, std::vector<sIPRange>::iterator > prRange;
    prRange = std::equal_range( m_vecBlockIPList.begin(), m_vecBlockIPList.end(), dwIp_, KComp() );
    if( prRange.first == prRange.second ) // 리스트에서 못찾았다면.
    {
        START_LOG( clog, L" Not Info Country ..."  ) << END_LOG;
        m_strCC_ = "ZZ";
    }
    else
    {
        START_LOG( clog, L"IP Range : " << KncUtil::toWideString( DWORDToIPStr(prRange.first->m_dwBegin) ) << L"|" << KncUtil::toWideString( DWORDToIPStr(prRange.first->m_dwEnd) ) << dbg::endl
            << L"IP Range : " << prRange.first->m_dwBegin << L"|" << prRange.first->m_dwEnd << dbg::endl
            << L"Country Code : " << KncUtil::toWideString( prRange.first->m_strCC ) ) << END_LOG;
        // IP에 대한 Country Code 값 저장
        m_strCC_ = prRange.first->m_strCC;
    }
}

bool KBlockIPList::LoadBlockCountry( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set<std::string>& setBlockCountryList_ )
{
    setBlockCountryList_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    std::string strCountryCode;
    while( true ) {
        if( kLuaMng_.GetValue( i++, strCountryCode ) != S_OK ) break;
        setBlockCountryList_.insert( strCountryCode );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KBlockIPList::CheckBlockCountry( IN const std::string m_strCC_ )
{
    std::set<std::string>::iterator sit;
    KLocker lock( m_csBlockIPList );
    sit = m_setBlockCountryList.find( m_strCC_ );
    if( sit == m_setBlockCountryList.end() ) {
        return true; // 블럭 국가가 아님.
    }
    return false; // 블럭 국가임.
}

bool KBlockIPList::LoadExceptUserID( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set<std::string>& setExceptUserID_ )
{
    setExceptUserID_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    std::string strUserID;
    while( true ) {
        if( kLuaMng_.GetValue( i++, strUserID ) != S_OK ) break;
        setExceptUserID_.insert( strUserID );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KBlockIPList::CheckExceptUserID( IN std::wstring wstrUserID_ )
{
    std::string strUserID = KncUtil::toNarrowString( wstrUserID_ );

    std::set<std::string>::iterator sit;
    KLocker lock( m_csBlockIPList );
    if( m_bExceptUserID ) {
        sit = m_setExceptUserID.find( strUserID );
        if( sit == m_setExceptUserID.end() ) {
            return false; // 허용 유저 아님.
        }
        return true; // 허용 유저임.
    } else {
        return true;
    }
}