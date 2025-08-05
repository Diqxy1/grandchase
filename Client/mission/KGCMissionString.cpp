#include "stdafx.h"
#include ".\kgcmissionstring.h"
#include "../utf16_cvt.h"
//
//


KGCMissionString::KGCMissionString(void)
{
    m_iLastCount = 0;
}

KGCMissionString::~KGCMissionString(void)
{

}

bool KGCMissionString::LoadStringFile( std::string strFileName , FILE* pFile , bool bForTool /*= false */)
{
    boost::shared_ptr<std::wifstream> ptr;
    if( pFile != NULL )
    {
        ptr.reset( new std::wifstream(pFile) );
    }
    else
    {
        ptr.reset( new std::wifstream );
        ptr.get()->open( strFileName.c_str() , std::ios::binary | std::ios::in );
    }

    std::wifstream& file = *ptr.get();
    IMBUE_NULL_CODECVT( file );
    if( file.is_open() == false )
        return false;

    wchar_t GarvStr[2048];
    file.read( GarvStr, 1 );

    std::wstring str;
    while( std::getline( file, str, L'\n' ) ) 
    {
        int iIndex = _wtoi( str.substr(0, str.find( L"\t" ) ).c_str() );
        std::wstring strMissionString = str.substr( str.find( L"\t" ) + 1, str.find( L"\n" ) - 2 ); // \t 다음부터 \n 까지
        while( true )
        {
            if( strMissionString.find( L"\r" ) == strMissionString.npos )
                break;
            strMissionString.erase( strMissionString.find( L"\r" ) );
        }
        Replacer( strMissionString, L"\\n", (bForTool?L"\r\n":L"\n") );
        m_mapMissionString.insert( StringList::value_type( iIndex, strMissionString ) );
    }

    file.close();
    return true;
}

bool KGCMissionString::OutPutStringFile( std::string strFileName )
{
    std::wofstream file;
    IMBUE_NULL_CODECVT( file );

    file.open( strFileName.c_str() , std::ios::binary | std::ios::out);    

    wchar_t cBom = 0xfeff;
    file.write( &cBom, 1 );

    std::map< DWORD, std::wstring >::iterator mit;
    for( mit = m_mapMissionString.begin() ; mit != m_mapMissionString.end() ; ++mit )
    {
        Replacer( mit->second, L"\r\n", L"\\n" );
        Replacer( mit->second, L"\n", L"\\n" );
        file<<mit->first<<L"\t"<<mit->second.c_str()<<L"\r\n";
    }
    file.close();

    return true;
}

int KGCMissionString::AddMissionString( std::wstring strString )
{
    std::map< DWORD, std::wstring >::iterator mit;
    // 스트링 추가는 툴에서만 한다고 가정합니다.
    Replacer( strString, L"\n", L"\r\n" );
    Replacer( strString, L"\\n", L"\r\n" );
    for( mit = m_mapMissionString.begin() ; mit != m_mapMissionString.end() ; ++mit )
    {
        if( mit->second.compare( strString ) == 0 )
            return mit->first;
        m_iLastCount = mit->first;
    }
    m_mapMissionString.insert( StringList::value_type( ++m_iLastCount, strString ) );
    return m_iLastCount;
}

bool KGCMissionString::GetMissionString( int iIndex, std::wstring& strOut )
{
    std::map< DWORD, std::wstring >::iterator mit;
    mit = m_mapMissionString.find( iIndex );

    if( mit == m_mapMissionString.end() )
    {
        strOut.clear();
        return false;
    }

    strOut = m_mapMissionString[iIndex];
    return true;
}

void KGCMissionString::Clear()
{
    m_mapMissionString.clear();
}

void KGCMissionString::Replacer( std::wstring& str_, wchar_t* OldStr, wchar_t* NewStr )
{
	size_t iFindPos = 0;
	while( (iFindPos = str_.find(OldStr)) != std::wstring::npos )
		str_.replace(iFindPos, wcslen(OldStr) , NewStr);
}
	
const std::wstring& KGCMissionString::GetString( int iIndex )
{
	return m_mapMissionString[iIndex];
}

bool KGCMissionString::LoadStringFromKOM( std::string strFileName )
{
    return SiKGCMultipleLanguages()->LoadStringFile(strFileName,m_mapMissionString);
}
