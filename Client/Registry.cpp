#include "stdafx.h"

#include "registry.h"

bool KRegistry::OpenKey(enum Keys hKey, LPCSTR szKey)
{   
    if( RegOpenKeyExA((HKEY)hKey,
                     (LPCSTR)szKey, 
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

bool KRegistry::CreateKey(enum Keys hKey, LPCTSTR szKey)
{   
    if( RegCreateKeyExA((HKEY)hKey,
                       (LPCSTR)szKey, 
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

bool KRegistry::SetValue(LPCSTR lpValueName, LPCSTR lpData)
{
    ASSERT(m_hKey != NULL);

    DWORD dwType = REG_SZ;

    if( ::RegSetValueExA(m_hKey, 
                       (LPCSTR)lpValueName, 
                        0, 
                        dwType, 
                       (LPBYTE)(LPCSTR)lpData, 
                        (DWORD)strlen(lpData)) == ERROR_SUCCESS )
    {
        ::RegFlushKey(m_hKey);
        return true;
    } // if

    return false;
}

bool KRegistry::SetValue(LPCSTR lpValueName, DWORD dwValue)
{
    ASSERT(m_hKey != NULL);
    
    DWORD dwType = REG_DWORD;
    
    if( ::RegSetValueExA(m_hKey, 
                       (LPCSTR)lpValueName, 
                        0, 
                        dwType, 
                       (LPBYTE)&dwValue, 
                        sizeof(DWORD)) == ERROR_SUCCESS )
    {
        ::RegFlushKey(m_hKey);
        return true;
    }
    return false;
}


bool KRegistry::GetValue(LPCSTR lpValueName, std::string& strValue)
{
    bool bRet = false;
    char* lpValue;
    DWORD dwType = REG_SZ;
    DWORD lpcbData;
    
    bRet = false;   
    lpValue = new char[256];
    memset(lpValue, 0, sizeof(char) * 256);
    lpcbData = 256;
    if( ::RegQueryValueExA(m_hKey, lpValueName, NULL, &dwType, (BYTE*)lpValue, &lpcbData) == ERROR_SUCCESS )
    {
        bRet = true;
        strValue = lpValue;
    }
    else
    {
        strValue.empty();
    }
    
    SAFE_DELETE_ARRAY(lpValue);
    
    return bRet;
}   


bool KRegistry::GetValue(LPCSTR lpValueName, DWORD& dwValue)
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

bool KRegistry::DeleteKey(enum Keys hKey, LPCSTR szKey)
{   
    return ::RegDeleteKeyA((HKEY)hKey, szKey) == ERROR_SUCCESS;
}

bool KRegistry::DeleteValue(LPCSTR lpValueName)
{   
    if( ::RegDeleteValueA(m_hKey, lpValueName) == ERROR_SUCCESS )
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

bool KRegistry::SaveKey(LPCSTR lpszFileName)
{
    ASSERT(m_hKey != NULL);
    return ::RegSaveKeyA(m_hKey, lpszFileName, NULL) == ERROR_SUCCESS;
}

bool KRegistry::RestoreKey(LPCSTR lpszFileName, DWORD dwFlags)
{
    ASSERT(m_hKey != NULL);
    return ::RegRestoreKeyA(m_hKey, lpszFileName, dwFlags) == ERROR_SUCCESS;
}

bool KRegistry::LoadKey(enum Keys hKey, LPCSTR lpszSubKey, LPCSTR lpszFileName)
{
    return ::RegLoadKeyA((HKEY)hKey, lpszSubKey, lpszFileName) == ERROR_SUCCESS;
}
