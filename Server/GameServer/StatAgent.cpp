#include "GSSimLayer.h"
#include "StatAgent.h"
#include "GameServer.h"

//#include <ImportAgentLib.h>
//#include <ImportJson.h>

ImplementSingleton( KStatAgent );


KStatAgent::KStatAgent(void)
:m_dwTickGap(60000)
,m_dwLastTick(0)
//,m_agentlib("localhost",8888,100,NULL)
{
    
}

KStatAgent::~KStatAgent(void) 
{
}

void KStatAgent::Tick()
{
    //if ( ::GetTickCount() - m_dwLastTick < m_dwTickGap ) {
    //    return;
    //}
    //m_dwLastTick = ::GetTickCount();

    int currentUsers = SiKGSSimLayer()->m_kActorManager.GetCount();
    int serverID = SiKGameServer()->GetServerID();

    Send( serverID, currentUsers );
}

void KStatAgent::Send( int serverID_, int concurrentUserNum_ )
{
    /*
	Json::Value doc;       
    doc["WorldID"]=1;
    doc["ChannelID"] = serverID_;
    doc["LoginUser"] = 0;
    doc["LogoutUser"] = 0;   
    doc["ConcurrentUser"] = concurrentUserNum_;
    m_agentlib.Send("ConcurrentUser",doc);
	*/
}