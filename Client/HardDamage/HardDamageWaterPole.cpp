#include "stdafx.h"
#include ".\HardDamageWaterPole.h"
#include "Spark.h"
#include "KGCRoomManager.h"

#define RANDOM_NUMBER (static_cast<float>( ::rand() ) / static_cast<float>( RAND_MAX ))

CHardDamageWaterPole::CHardDamageWaterPole(void)
{
}

CHardDamageWaterPole::~CHardDamageWaterPole(void)
{
}

bool CHardDamageWaterPole::CrashPlayer( CDamageInstance* pDamageInstance , int hitWho, float fDamageRate )
{
	for( int t = 0; t < 5; t++ )
	{                               
		float x = pDamageInstance->m_X - 1.15f + 0.3f * RANDOM_NUMBER;
		float y = pDamageInstance->m_Y - 0.1f;
		int Delay = 0;
		pDamageInstance->Add( WATER_PIECE, x, y, Delay, 0.1f + 0.1f * randf(), x, y - 0.4f * randf(), 12.6f * randf() );
	}
	if( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() || (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode()) )
	{
		if( g_MyD3D->MyPlayer[hitWho]->y_Speed < 0.01f + (float)pDamageInstance->m_Life / (float)pDamageInstance->m_pOriginalDamage->m_Start_Life * 0.05f )
			g_MyD3D->MyPlayer[hitWho]->y_Speed = 0.01f + (float)pDamageInstance->m_Life / (float)pDamageInstance->m_pOriginalDamage->m_Start_Life * 0.05f;

           g_MyD3D->MyPlayer[hitWho]->m_iJumpPossibleFrame = pDamageInstance->m_pOriginalDamage->m_iJumpPossibleFrame;
        g_MyD3D->MyPlayer[hitWho]->IsContact = true;
    }
	return true;
}

void CHardDamageWaterPole::End(CDamageInstance* pDamageInstance )
{
    if( g_MyD3D->MySparks )
	    pDamageInstance->Add(FOUNTAIN_HEAD0, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.1f, 0);
	CDamage::End(pDamageInstance);
}

