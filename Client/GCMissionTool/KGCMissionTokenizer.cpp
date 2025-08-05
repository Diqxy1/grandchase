#include "StdAfx.h"
#include ".\kgcmissiontokenizer.h"
#include "GlobalFunc.h"

KGCMissionTokenizer::KGCMissionTokenizer(void)
: m_iIndex( 0 )
, m_iCount( 0 )
{
}

KGCMissionTokenizer::~KGCMissionTokenizer(void)
{
}


void KGCMissionTokenizer::LoadFile( std::string strFileName )
{
    typedef boost::char_separator<wchar_t, std::wstring::traits_type> wchar_separator;
    typedef boost::tokenizer<wchar_separator,std::wstring::const_iterator, std::wstring > tokenizer;
    m_vecStringTok.clear();

    std::wcout.imbue( std::locale("") );

    std::wifstream file;
    file.open( strFileName.c_str() , std::ios::binary | std::ios::in );

    IMBUE_NULL_CODECVT( file );
    if( file.is_open() == false )
        return;

    wchar_t GarvStr[2048];
    file.read( GarvStr, 1 );

    std::wstring str;
    while( std::getline( file, str, L'\r' ) ) 
    {
        std::wstring strTmp( str );
        wchar_separator sep(L"\t");
        tokenizer tokens( strTmp, sep);
        tokenizer::iterator tok_iter = tokens.begin();
        std::vector< std::wstring > vecTok;
        for( tok_iter = tokens.begin() ; tok_iter != tokens.end() ; tok_iter++ )
        {
            std::wstring strToken( *tok_iter );
            if( strToken.find( L"\n" ) != strToken.npos 
                && tok_iter == tokens.begin() )
            {
                GC::Replacer( strToken, L"\r", L"" );
                GC::Replacer( strToken, L"\n", L"" );
            }
            if( strToken.empty() == true )
                break;
            vecTok.push_back( strToken );
        }
        if( vecTok.empty() == true )
            continue;
        m_vecStringTok.push_back( vecTok );
    }
}

int KGCMissionTokenizer::GetInteger( int iIndex )
{
    if( m_iIndex != iIndex )
    {
        m_iIndex = iIndex;
        m_iCount = 0;
    }

    return _wtoi( m_vecStringTok[m_iIndex][m_iCount++].c_str() );
}

std::wstring KGCMissionTokenizer::GetString( int iIndex )
{
    if( m_iIndex != iIndex )
    {
        m_iIndex = iIndex;
        m_iCount = 0;
    }

    return m_vecStringTok[m_iIndex][m_iCount++];
}