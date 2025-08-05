#include "stdafx.h"
#include "NPC Action.h"

kNPC::kNPC()
{
    ClearNPC_Action();
}

void kNPC::ClearNPC_Action() 
{
	m_vecNPC.clear();
}

bool kNPC::PushNPC_Action(NPC *pNPC)
{
    if( pNPC == NULL )
		return false;

	m_vecNPC.push_back(*pNPC);

    return true;
}

void kNPC::PopNPC_Action()
{
	if( m_vecNPC.empty() == true ){ 
		g_kTransNPC->TransNPC.What = 0;
		return;
	}

	g_kTransNPC->TransNPC = *(m_vecNPC.begin());

	m_vecNPC.erase(m_vecNPC.begin());
}