#include "stdafx.h"
#include ".\HardDamageJin.h"
#include "Monster.h"


void CHardDamageJin4Ball::FrameMove(CDamageInstance* pDamageInstance )
{
	CDamage::FrameMove(pDamageInstance);
    if( pDamageInstance->m_Life < 40 )
        pDamageInstance->m_pOriginalDamage->m_x_Speed = 0;
}

bool CHardDamageJin4Sucker::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    if( !CDamage::CrashPlayer( pDamageInstance, hitWho, fDamageRate ) )
        return false;

    float fDelta = ( pDamageInstance->m_X - g_MyD3D->MyPlayer[hitWho]->GetPosition().x ) / ( pDamageInstance->m_Scale_x / 4 );

    fDelta = ASMfabs( fDelta );

    if( fDelta < 1.0f )
        g_MyD3D->MyPlayer[hitWho]->x_Speed *= fDelta;    

    if( pDamageInstance->m_Y > g_MyD3D->MyPlayer[hitWho]->GetPosition().y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * 0.5f )
        if( !g_MyD3D->MyPlayer[hitWho]->IsContact )
            g_MyD3D->MyPlayer[hitWho]->y_Speed = ( pDamageInstance->m_Y + 0.1f - g_MyD3D->MyPlayer[hitWho]->GetPosition().y ) * 0.02f;

    return true;
}

void CHardDamageJin4Sucker::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
    CDamage::CrashMonster( pDamageInstance, Monsteri );        

	MONSTER *pMonster = g_kMonsterManager.GetMonster( Monsteri );
	if( pMonster == NULL ) return;

    float fDelta = ( pDamageInstance->m_X - pMonster->GetX() ) / ( pDamageInstance->m_Scale_x / 4 );

    fDelta = ASMfabs( fDelta );

    if( fDelta < 1.0f )
        pMonster->SetSpeedX( pMonster->GetSpeedX() * fDelta );

    if( pDamageInstance->m_Y > pMonster->GetY() + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * 0.5f )
        if( !pMonster->GetIsContact() )
            pMonster->SetSpeedY( ( pDamageInstance->m_Y + 0.1f - pMonster->GetY() ) * 0.02f );
}