#include <WinSock2.h>
#include <iostream>

#include <ImportKncLua.h>
#include <ImportKncSerializer.h>
#include <ImportKncSecurity.h>
#include <ImportKncDebug.h>

#pragma comment( lib, "ws2_32" )
#include "vld.h"

#include "MsgServer.h"

BOOL CALLBACK Handler( DWORD dwEvent )
{
    Sleep(2000);
    return TRUE;
}

void main()
{
    SetConsoleCtrlHandler( Handler,TRUE );

    KBaseServer* pkServer = SiKMsgServer();

    if( pkServer->Init( L"config_msg" ) )
    {
        pkServer->Run( true );
    }

    pkServer->ShutDown();

    KMsgServer::ReleaseInstance();
}
