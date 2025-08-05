#pragma warning(push, 2)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)


#include <Windows.h>
#include <iostream>
#include "GenericLogfile.h"
#include <fstream>
#include "LogManager.h"
#include <ToString.h>
#include <atltime.h>
#include <dbg/dbg.hpp>
#include "Log4.h"

void TestGenericLogfile()
{
    std::wcout << L"___________________START Generic Log______________________" << std::endl;


    KGenericLogfile TestFile;
    std::wstring strFolder( L"TestGenLog" );

    TestFile.OpenFile( strFolder );
    std::wcout << TestFile.GetFolderName() << std::endl;
    std::wcout << TestFile.GetFullPath() << std::endl;

    TestFile << L"Test..." << GLOG::endl;

    ::Sleep(2000);

    TestFile.ChangeFile();

    TestFile << L"After ChangeLog() ..." << GLOG::endl;

    TestFile.CloseFile();

    std::wcout << L"___________________END Generic Log______________________" << std::endl;

}


void TestKLogManager()
{
    
    SiKLogManager()->Init();
    SiKLogManager()->Get( KLogManager::LT_ABUSE ) << L"Test Abuse Log" << GLOG::endl;

    SiKLogManager()->Get(30) << L"Test Over Run..." << GLOG::endl;

    SiKLogManager()->Get(30)
        << std::boolalpha
        << L"Test false : " << false << GLOG::endl
        << L"Test true : " << true << GLOG::endl
        << std::noboolalpha;

    SiKLogManager()->Get(KLogManager::LT_ABUSE)
        << std::boolalpha
        << L"Test false : " << false << GLOG::endl
        << L"Test true  : " << true << GLOG::endl
        << std::noboolalpha;

    std::wstring strLogin = L"하하하하";
    DWORD dwExp = 9999;
    DWORD dwLevel = 44444;
    int nGamePoint = -3422;
    int nMode = 23;
    int nUID = 12345;
    SiKLogManager()->Get(KLogManager::LT_EXPGP)
        << L"GP 변화량 이상. Name : " << strLogin
        << L", Exp : " << dwExp
        << L", Level : " << dwLevel
        << L", 획득 GP : " << nGamePoint 
        << L", 모드 : " << nMode
        << L", 길드 : " << static_cast<bool>(nMode == nGamePoint)
        << L", 시간 : " << (LPCTSTR)CTime::GetCurrentTime().Format( KNC_TIME_FORMAT ) << GLOG::endl;

    SiKLogManager()->Get(KLogManager::LT_DURATION)
        << L"Name : " << strLogin << L"  UID : " << nUID <<  L"  TIME : " << KNC_TIME_STRING << GLOG::endl;

    SiKLogManager()->ChangeFile( KLogManager::LT_DURATION );
    SiKLogManager()->Get(KLogManager::LT_DURATION)
        << L"Name2 : " << strLogin << L"  UID2 : " << nUID <<  L"  TIME2 : " << KNC_TIME_STRING << GLOG::endl;
        
    Sleep( 3000 );
    SiKLogManager()->ChangeFile( KLogManager::LT_DURATION );
    SiKLogManager()->Get(KLogManager::LT_DURATION)
        << L"Name3 : " << strLogin << L"  UID3 : " << nUID <<  L"  TIME3 : " << KNC_TIME_STRING << GLOG::endl;

    KLogManager::ReleaseInstance();
}


void TestGenericLog()
{
    std::wcout.imbue( std::locale("KOR"));
    TestGenericLogfile();
    TestKLogManager();

    
}