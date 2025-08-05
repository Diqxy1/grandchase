#pragma once
#include "stdafx.h"
#include <Windows.h>

class KGCDllLoader
{
private:
    static std::map< std::string, HINSTANCE > ms_mapHandle;

public:
    KGCDllLoader(void);
    ~KGCDllLoader(void);

    static bool LoadDll( std::string strDllName_ );
    static void* CallFunction( std::string strDllName_, std::string strFuncName_ );
    static bool ReleaseDll( std::string strDllName_ );
    static bool ReleaseAllDll();
};