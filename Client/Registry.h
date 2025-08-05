#ifndef _REGISTRY_H_
#define _REGISTRY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "windows.h"
//#include <string>
//#include "KAssert.h"


class KRegistry
{
public:
    enum cregRestoreEnum
    {
        regVolatile = REG_WHOLE_HIVE_VOLATILE,
        regRefresh = REG_REFRESH_HIVE,
        regNoLazy = REG_NO_LAZY_FLUSH
    };

    enum Keys
    {
        classesRoot     = reinterpret_cast<int>( HKEY_CLASSES_ROOT ),
        currentUser     = reinterpret_cast<int>( HKEY_CURRENT_USER ),
        localMachine    = reinterpret_cast<int>( HKEY_LOCAL_MACHINE ),
        currentConfig   = reinterpret_cast<int>( HKEY_CURRENT_CONFIG ),
        users           = reinterpret_cast<int>( HKEY_USERS ),
        performanceData = reinterpret_cast<int>( HKEY_PERFORMANCE_DATA ),   //Windows NT/2000
        dynData         = reinterpret_cast<int>( HKEY_DYN_DATA ),           //Windows 95/98
    };

    KRegistry()
    {
        m_hKey = NULL;
    };

    ~KRegistry()
    {
        CloseKey();
    };

    bool OpenKey(enum Keys hKey, LPCSTR szKey);
    bool CreateKey(enum Keys hKey, LPCTSTR szKey);
    bool DeleteKey(enum Keys hKey, LPCSTR szKey);
    bool DeleteValue(LPCSTR lpValueName);
    bool GetValue(LPCSTR lpValueName, std::string& strValue);
    bool GetValue(LPCSTR lpValueName, DWORD& dwValue);
    bool SetValue(LPCSTR lpValueName, LPCSTR lpData);
    bool SetValue(LPCSTR lpValueName, DWORD dwValue);
    bool SaveKey(LPCSTR lpszFileName);
    bool RestoreKey(LPCSTR lpszFileName, DWORD dwFlag);
    bool LoadKey(enum Keys hKey, LPCSTR lpszSubKey, LPCSTR lpszFileName);
    void CloseKey();    

protected:
    HKEY m_hKey;
};

#endif // _REGISTRY_H_