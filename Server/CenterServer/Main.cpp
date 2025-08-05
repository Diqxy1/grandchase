#include <WinSock2.h>
#include <iostream>

#include <ImportKncLua.h>
#include <ImportKncSerializer.h>
#include <ImportKncSecurity.h>
#include <ImportKncDebug.h>
#include <vld.h>
#pragma comment( lib, "ws2_32" )

#pragma comment( lib, "CPSSOLib" )  // 넷마블 인증 라이브러리

#include "CenterServer.h"

BOOL CALLBACK Handler( DWORD dwEvent )
{
    Sleep(2000);
    return TRUE;
}

void main()
{
    ::SetConsoleCtrlHandler( Handler,TRUE );

    KBaseServer* pkServer = SiKCenterServer();

    if( pkServer->Init( L"config_Center" ) )
    {
        pkServer->Run( true );
    }

    pkServer->ShutDown();

    KCenterServer::ReleaseInstance();
}