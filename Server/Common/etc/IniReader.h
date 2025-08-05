#pragma once
#ifndef IN
    #define IN
#endif

#ifndef OUT
    #define OUT
#endif

#include <string>
class KIniReader
{
public:
    KIniReader( wchar_t* szFileName ); 
    int ReadInteger( IN const wchar_t* szSection, IN const wchar_t* szKey, IN int nDefaultValue );
    float ReadFloat( IN const wchar_t* szSection, IN const wchar_t* szKey, IN float fDefaultValue );
    bool ReadBoolean( IN const wchar_t* szSection, IN const wchar_t* szKey, IN bool bolDefaultValue );
    std::wstring ReadString( IN const wchar_t* szSection, IN const wchar_t* szKey, IN const wchar_t* szDefaultValue );
private:
    std::wstring m_strFileName;
};