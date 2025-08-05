#include "stdafx.h"
#include "registry.h"

bool KRegistry::OpenKey( enum Keys hKey, char* szKey )
{   
    if( RegOpenKeyExA((HKEY)hKey,
                      szKey, 
                      0, 
                      KEY_ALL_ACCESS, 
                      &m_hKey) == ERROR_SUCCESS )
    {
        return true;
    }
    else
    {
        m_hKey = NULL;
        return false;
    } //else
}

bool KRegistry::CreateKey(enum Keys hKey, char* szKey)
{   
    if( RegCreateKeyExA((HKEY)hKey,
                        szKey, 
                        0, 
                        NULL, 
                        REG_OPTION_NON_VOLATILE, 
                        KEY_ALL_ACCESS, 
                        NULL, 
                        &m_hKey, 
                        NULL) == ERROR_SUCCESS )
    {
        return true;
    }
    else
    {
        m_hKey = NULL;
        return false;
    } // else
}

bool KRegistry::SetValue( char* lpValueName, char* lpData)
{
    ASSERT(m_hKey != NULL);

    DWORD dwType = REG_SZ;

    if( ::RegSetValueExA(m_hKey, 
                        lpValueName, 
                        0, 
                        dwType, 
                        (BYTE*)lpData, 
                        strlen(lpData)) == ERROR_SUCCESS )
    {
        ::RegFlushKey(m_hKey);
        return true;
    } // if

    return false;
}

bool KRegistry::SetValue( char* lpValueName, DWORD dwValue)
{
    ASSERT(m_hKey != NULL);
    
    DWORD dwType = REG_DWORD;
    
    if( ::RegSetValueExA(m_hKey, 
                        lpValueName, 
                        0, 
                        dwType, 
                        (BYTE*)&dwValue, 
                        sizeof(DWORD)) == ERROR_SUCCESS )
    {
        ::RegFlushKey(m_hKey);
        return true;
    }
    return false;
}


bool KRegistry::GetValue( char* lpValueName, std::string& strValue )
{
    bool bRet = false;
    char* lpstrValue;
    DWORD dwType = REG_SZ;
    DWORD lpcbData;
    
    bRet = false;   
    lpstrValue = new char[256];
    memset(lpstrValue, 0, sizeof(char) * 256);
    lpcbData = 1024;
        
    if( ::RegQueryValueExA(m_hKey,
                          lpValueName,
                          NULL,
                          &dwType, 
                          (BYTE*)lpstrValue,
                          &lpcbData) == ERROR_SUCCESS )
    {
        bRet = true;
        strValue = (char*)(LPCTSTR)lpstrValue;
    }
    else
    {
        strValue.empty();
    }
    
    SAFE_DELETE_ARRAY(lpstrValue);
    
    return bRet;
}   


bool KRegistry::GetValue( char* lpValueName, DWORD& dwValue )
{
    bool bRet = false;
    DWORD dwType = REG_DWORD;
    DWORD lpcbData = sizeof(DWORD);
    dwValue = 0;    
    
        
    if(RegQueryValueExA(m_hKey,
                        lpValueName,
                        NULL,
                        &dwType, 
                        (BYTE*)(DWORD)&dwValue,
                        &lpcbData) == ERROR_SUCCESS )
        bRet = true;

    return bRet;
}

bool KRegistry::DeleteKey(enum Keys hKey, char* szKey)
{   
    return ::RegDeleteKeyA( (HKEY)hKey, szKey ) == ERROR_SUCCESS;
}

bool KRegistry::DeleteValue( char* lpValueName)
{   
    if( ::RegDeleteValueA( m_hKey,lpValueName ) == ERROR_SUCCESS )
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

void KRegistry::CloseKey()
{
    if( NULL != m_hKey )
    {
        ::RegCloseKey(m_hKey);
        m_hKey = NULL;
    } // if
}

bool KRegistry::SaveKey( char* lpszFileName)
{
    ASSERT(m_hKey != NULL);
    return ::RegSaveKeyA(m_hKey, lpszFileName, NULL) == ERROR_SUCCESS;
}

bool KRegistry::RestoreKey( char* lpszFileName, DWORD dwFlags)
{
    ASSERT(m_hKey != NULL);
    return ::RegRestoreKeyA(m_hKey, lpszFileName, dwFlags) == ERROR_SUCCESS;
}

bool KRegistry::LoadKey(enum Keys hKey, char* lpszSubKey, char* lpszFileName)
{
    return ::RegLoadKeyA((HKEY)hKey, lpszSubKey, lpszFileName) == ERROR_SUCCESS;
}
