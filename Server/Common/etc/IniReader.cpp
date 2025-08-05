
#include "IniReader.h"
#include <iostream>
#include <Windows.h>
#include <boost/algorithm/string.hpp> // to_lower

KIniReader::KIniReader( wchar_t* szFileName_ )
{
    if( szFileName_ == NULL ) return;
    m_strFileName = szFileName_;
}

int KIniReader::ReadInteger( IN const wchar_t* szSection_, IN const wchar_t* szKey_, IN int nDefaultValue_ )
{
    if( szSection_== NULL || szKey_ == NULL ) return nDefaultValue_;

    int nResult = ::GetPrivateProfileIntW( szSection_, szKey_, nDefaultValue_, m_strFileName.c_str() );
    return nResult;
}

float KIniReader::ReadFloat( IN const wchar_t* szSection_, IN const wchar_t* szKey_, IN float fDefaultValue_ )
{
    if( szSection_== NULL || szKey_ == NULL ) return fDefaultValue_;

    wchar_t szResult[255]={0};
    wchar_t szDefault[255]={0};
    float fltResult;
    ::swprintf_s( szDefault, sizeof(szDefault), L"%f", fDefaultValue_ ); 
    ::GetPrivateProfileStringW(szSection_,  szKey_, szDefault, szResult, sizeof(szResult), m_strFileName.c_str());  
    fltResult =  (float)::_wtof(szResult); 
    return fltResult;
}

bool KIniReader::ReadBoolean( IN const wchar_t* szSection_, IN const wchar_t* szKey_, IN bool bolDefaultValue_ )
{
    if( szSection_== NULL || szKey_ == NULL ) return bolDefaultValue_;

    wchar_t szResult[255] = {0};
    wchar_t szDefault[255] = {0};
    
    bool bResult = bolDefaultValue_;
    ::swprintf_s( szDefault, sizeof(szDefault), L"%s", (bolDefaultValue_? L"true" : L"false") );
    ::GetPrivateProfileStringW( szSection_, szKey_, szDefault, szResult, sizeof(szResult), m_strFileName.c_str() );

    std::wstring strResult( szResult );
    boost::to_lower( strResult );
    return ( ( ::wcscmp(strResult.c_str(), L"true") == 0 ) ? true : false );
}
  
std::wstring KIniReader::ReadString( IN const wchar_t* szSection_, IN const wchar_t* szKey_, IN const wchar_t* szDefaultValue_ )
{
    std::wstring strResult;
    if( szSection_ == NULL || szKey_ == NULL ) return strResult;
    if( szDefaultValue_ != NULL ) strResult = szDefaultValue_;

    wchar_t szResult[255] = {0};
    ::GetPrivateProfileStringW( szSection_,  szKey_, strResult.c_str(), szResult, sizeof(szResult), m_strFileName.c_str() );
    strResult = szResult;
    return strResult;
}