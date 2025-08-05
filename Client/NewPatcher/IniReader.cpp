#include "stdafx.h"
#include "IniReader.h"
#include <iostream>
#include <Windows.h>
CIniReader::CIniReader(wchar_t* szFileName)
{ 
    m_szFileName = szFileName;
}

int CIniReader::ReadInteger(wchar_t* szSection, wchar_t* szKey, int iDefaultValue)
{ 
    int iResult = GetPrivateProfileInt(szSection,  szKey, iDefaultValue, m_szFileName.c_str());  
    return iResult;
}

float CIniReader::ReadFloat(wchar_t* szSection, wchar_t* szKey, float fltDefaultValue)
{ 
    wchar_t szResult[255]; 
    wchar_t szDefault[255]; 
    float fltResult; 
    swprintf(szDefault, L"%f",fltDefaultValue); 
    GetPrivateProfileString(szSection,  szKey, szDefault, szResult, 255, m_szFileName.c_str());  
    fltResult =  (float)_wtof(szResult); 
    return fltResult;
}

bool CIniReader::ReadBoolean(wchar_t* szSection, wchar_t* szKey, bool bolDefaultValue)
{ 
    wchar_t szResult[255]; 
    wchar_t szDefault[255]; 
    bool bolResult; 
    swprintf(szDefault, L"%s", bolDefaultValue? L"True" : L"False"); 
    GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName.c_str());  
    bolResult =  (wcscmp(szResult, L"True") == 0 || wcscmp(szResult, L"true") == 0) ? true : false; 
    return bolResult;
}

std::wstring CIniReader::ReadString(wchar_t* szSection, wchar_t* szKey, const wchar_t* szDefaultValue)
{ 
    wchar_t szResult[255];
    memset(szResult, 0x00, 255); 
    GetPrivateProfileString(szSection,  szKey, szDefaultValue, szResult, 255, m_szFileName.c_str());  
    std::wstring strRet = szResult;
    return strRet;
}

