#include "stdafx.h"
#include ".\gcdllloader.h"
#include "io.h"
#include "GCUtil.h"

extern void GetCurrentPath( std::string& strPath_ );

std::map< std::string, HINSTANCE > KGCDllLoader::ms_mapHandle;
KGCDllLoader::KGCDllLoader(void)
{
}

KGCDllLoader::~KGCDllLoader(void)
{
}

bool KGCDllLoader::LoadDll( std::string strDllName_ )
{
    HINSTANCE hInst;
	std::transform(strDllName_.begin(),strDllName_.end(),strDllName_.begin(),tolower);

    if( ms_mapHandle.find( strDllName_ ) == ms_mapHandle.end() )
    {        
        std::string strCurrentPath;
        GetCurrentPath( strCurrentPath );
        strCurrentPath.append( strDllName_ );
        if( 0 != ::_access( strDllName_.c_str() , 0 ) )
        {
            return false;
        }

        hInst = ::LoadLibraryA( strCurrentPath.c_str() );
        DWORD dwErr = ::GetLastError();

        if( hInst == NULL )
        {
            if( (hInst = ::LoadLibraryA( strDllName_.c_str() ) ) == NULL )
            {
                dwErr = ::GetLastError();
                return false;
            }
        }

        ms_mapHandle[strDllName_] = hInst;
    }

    return true;
}

void* KGCDllLoader::CallFunction( std::string strDllName_, std::string strFuncName_ )
{
    void* pFunc = NULL;
	std::transform(strDllName_.begin(),strDllName_.end(),strDllName_.begin(),tolower);


    if( ms_mapHandle.find( strDllName_ ) == ms_mapHandle.end() )
    {
        if( LoadDll( strDllName_ ) == false )
            return NULL;
    }

    if( (pFunc = GetProcAddress( ms_mapHandle[strDllName_] , strFuncName_.c_str() )) == NULL )
    {
        //함수가 없네??? 젝일슨 뭐야??
        pFunc = NULL;
    }    

    return pFunc;
}

bool KGCDllLoader::ReleaseDll( std::string strDllName_ )
{
	std::transform(strDllName_.begin(),strDllName_.end(),strDllName_.begin(),tolower);

    if( ms_mapHandle.find( strDllName_ ) == ms_mapHandle.end() )
    {
        return false;
    }
    else
    {
        ::FreeLibrary( ms_mapHandle[strDllName_] );
        ms_mapHandle.erase( strDllName_ );
        return true;
    }    
}

bool KGCDllLoader::ReleaseAllDll()
{
    bool bRet = true;
    std::map< std::string, HINSTANCE >::iterator mit;

    for( mit = ms_mapHandle.begin() ; mit != ms_mapHandle.end() ; ++mit )
    {
        if( !::FreeLibrary( mit->second ) )
            bRet = false;
    }

    ms_mapHandle.clear();
    return bRet;
}