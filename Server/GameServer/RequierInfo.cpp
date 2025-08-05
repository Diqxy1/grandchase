#include "RequierInfo.h"
#include "lua/KLuaManager.h"
#include <dbg/dbg.hpp>
#include "Log4.h"
#include <algorithm>
#include <iterator>
#include <xutility>
//FILE_NAME_FOR_LOG

ImplementSingleton( KRequierInfo );
ImplOstmOperatorW2A( KRequierInfo );

KRequierInfo::KRequierInfo(void)
{
    m_mapRequires.clear();
}

KRequierInfo::~KRequierInfo(void)
{
}

#include "StmOperator.h"
ImplToStringW( KRequierInfo )
{
    KLocker lock( m_csRequires );
    stm_ << L"    KRequierInfo" << std::endl;

    stm_ << std::endl << L" RequierInfo : ";
    std::copy( m_mapRequires.begin(), m_mapRequires.end(), 
        std::ostream_iterator<std::map<int, sRequire>::value_type,wchar_t>( stm_, L", " ) );
    return stm_;
}

bool KRequierInfo::LoadScript()
{
    return _LoadScript(std::string());
}

bool KRequierInfo::_LoadScript( OUT std::string& strScript_ )
{
    std::string strFileName( "RequierInfo.lua" );
    KLuaManager kLuaMng;
    std::map< int, sRequire >   mapRequires;
    mapRequires.clear();;

    strScript_ = strFileName;
    _JIF( kLuaMng.DoFile( strFileName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.BeginTable( "RequierInfo" )== S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        sRequire sRequ;
        sRequ.m_vecItems.clear();

        if( kLuaMng.BeginTable( i )== S_OK )
        {
            _JIF( kLuaMng.GetValue( "ID", sRequ.m_nID ) == S_OK, return false );

            _JIF( kLuaMng.BeginTable( "Items" )== S_OK, return false );
            for( int j = 1 ; ; ++j )
            {
                if( kLuaMng.BeginTable( j )== S_OK )
                {
                    std::pair<char,DWORD> prData;
                    int nType;
                    _JIF( kLuaMng.GetValue( 1, nType ) == S_OK, return false );
                    _JIF( kLuaMng.GetValue( 2, prData.second ) == S_OK, return false );
                    _JIF( kLuaMng.EndTable() == S_OK, return false );

                    _JIF( nType >= 0 && nType < 2, return false ); // 이부분...^^...

                    prData.first = (char)nType;
                    sRequ.m_vecItems.push_back( prData );
                }
                else
                    break;
            }
            _JIF( kLuaMng.EndTable() == S_OK, return false ); // Items Table;

            _JIF( kLuaMng.EndTable() == S_OK, return false );

            std::pair< std::map<int,sRequire>::iterator, bool> prRet;
            prRet = mapRequires.insert( std::make_pair(sRequ.m_nID, sRequ) );
            if( prRet.second == false )
            {
                START_LOG( cerr, L"중복된 데이터가 존재함.. ID " << sRequ.m_nID ) << END_LOG;
                return false;
            }
        }
        else
            break;

    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );
    START_LOG( cerr, L"로딩 완료. Size : " << mapRequires.size() ) << END_LOG;

    // LOG...
    KLocker lock( m_csRequires );
    m_mapRequires.swap( mapRequires );
    return true;
}

bool KRequierInfo::GetRequierData( IN const int nID_, OUT sRequire& requireData_ )
{
    KLocker lock( m_csRequires );

    std::map< int, sRequire >::const_iterator mit;
    mit = m_mapRequires.find( nID_ );

    if( mit == m_mapRequires.end() )
        return false;

    requireData_ = mit->second;
    return true;
}