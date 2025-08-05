#include <WinSock2.h>
#include <iostream>

#include <ImportKncLua.h>
#include <ImportKncSerializer.h>
#include <ImportKncSecurity.h>
#include <ImportKncDebug.h>


#pragma comment( lib, "ws2_32" )
#include "vld.h"

#pragma comment( lib, "nprotect/ggsrvlib25" ) // nprotect. // 070914. woosh.
#pragma comment( lib, "hshield/AntiCpXSvr" ) // hshield.
#pragma comment( lib, "Wininet.lib" ) // AgentLib

#include "GameServer.h"

BOOL CALLBACK Handler( DWORD dwEvent )
{
    Sleep(2000);
    return TRUE;
}

void main()
{
    SetConsoleCtrlHandler( Handler,TRUE );

    KBaseServer* pkServer = SiKGameServer();

    if( pkServer->Init( L"config_server" ) )
    {
        pkServer->Run( true );
    }

    pkServer->ShutDown();

    KGameServer::ReleaseInstance();
}
