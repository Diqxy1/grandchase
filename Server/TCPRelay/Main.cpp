#include <WinSock2.h>

#include <ImportKncLua.h>
#include <ImportKncSerializer.h>
#include <ImportKncSecurity.h>
#include <ImportKncDebug.h>

#pragma comment( lib, "ws2_32" )
#include "vld.h"

#include "TRServer.h"
BOOL CALLBACK Handler( DWORD dwEvent )
{
    Sleep(2000);
    return TRUE;
}

void main()
{
    SetConsoleCtrlHandler( Handler,TRUE );

    KTRServer* pkTRServer = SiKTRServer();

    if( pkTRServer->Init( "config_TRserver" ) )
    {
        pkTRServer->Run();
    }

    pkTRServer->ShutDown();

    KTRServer::ReleaseInstance();

}
