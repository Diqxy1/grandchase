#include <WinSock2.h>
#include <iostream>

#include <ImportKncLua.h>
#include <ImportKncSerializer.h>
#include <ImportKncSecurity.h>
#include <ImportKncDebug.h>

#pragma comment( lib, "ws2_32" )
#include "vld.h"

#include "MsgRouter.h"

BOOL CALLBACK Handler( DWORD dwEvent )
{
    Sleep(2000);
    return TRUE;
}

void main()
{
    SetConsoleCtrlHandler( Handler,TRUE );

    KBaseServer* pkServer = SiKMsgRouter();

    if( pkServer->Init( L"config_mr" ) )
    {
        pkServer->Run( true );
    }

    pkServer->ShutDown();

    KMsgRouter::ReleaseInstance();
}
