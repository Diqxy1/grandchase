#ifndef REGISTRY_H
#define REGISTRY_H

#include "windows.h"
#include <string>
#include "KAssert.h"


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
        performanceData = reinterpret_cast<int>( HKEY_PERFORMANCE_DATA ),   // Windows NT/2000
        dynData         = reinterpret_cast<int>( HKEY_DYN_DATA ),           // Windows 95/98
    };

    KRegistry()
    {
        m_hKey = NULL;
    };

    ~KRegistry()
    {
        CloseKey();
    };

    bool OpenKey( enum Keys hKey, char* szKey );
    bool CreateKey(enum Keys hKey, char* szKey );
    bool DeleteKey(enum Keys hKey, char* szKey );
    bool DeleteValue( char* lpValueName );
    bool GetValue( char* lpValueName, std::string& strValue);
    bool GetValue( char* lpValueName, DWORD& dwValue);
    bool SetValue( char* lpValueName, char* lpData);
    bool SetValue( char* lpValueName, DWORD dwValue);
    bool SaveKey( char* lpszFileName);
    bool RestoreKey( char* lpszFileName, DWORD dwFlag);
    bool LoadKey(enum Keys hKey, char* lpszSubKey, char* lpszFileName);
    void CloseKey();    

protected:
    HKEY m_hKey;
};

#endif