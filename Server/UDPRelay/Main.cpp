
#include <WinSock2.h>

#include <ImportKncLua.h>
//#include <ImportKncSerializer.h>
//#include <ImportKncSecurity.h>
#include <ImportKncDebug.h>

#include "RelayServerMgr.h"
#pragma comment( lib, "ws2_32" )

int main()
{

	KRelayServerMgr* pkRelayServerMgr = SiKRelayServerMgr();

	if( pkRelayServerMgr->Init( L"config_relay" ) )
	{
		pkRelayServerMgr->Run( true );
	}

	pkRelayServerMgr->ShutDown();

	KRelayServerMgr::ReleaseInstance();

	return 0;
}