#pragma once
#include <KNCSingleton.h>
//#include <AgentLib/AgentLib.h>


class KStatAgent
{
    DeclareSingleton( KStatAgent );

public:
    KStatAgent(void);
    ~KStatAgent(void);

    void Tick();
    void Send( int serverID, int concurrentUserNum );
private:
    DWORD    m_dwTickGap;
    DWORD    m_dwLastTick;

   // CAgentLib m_agentlib;
};

DefSingletonInline( KStatAgent );