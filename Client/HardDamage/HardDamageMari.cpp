#include "stdafx.h"
#include ".\HardDamageMari.h"
#include "Stage.h"
#include "Monster.h"
#include "LTPacket.h"
#include "LatencyTransfer.h"

void CHardDamageTeleporter::Begin( CDamageInstance* pDamageInstance )
{
    CDamage::Begin( pDamageInstance );
    Teleport( pDamageInstance );
}

void CHardDamageTeleporter::BeginWithAngle( CDamageInstance* pDamageInstance , float fStartAngle )
{
    BeginWithAngle( pDamageInstance , fStartAngle );
    Teleport( pDamageInstance );
}

void CHardDamageTeleporter::BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY )
{
    CDamage::BeginWithLocate( pDamageInstance,fStartX, fStartY );
    Teleport( pDamageInstance );
}

void CHardDamageTeleporter::Teleport( CDamageInstance* pDamageInstance )
{

    if ( pDamageInstance->m_X > g_MyD3D->MyStg->GetXLimit() )
    {
        return;
    }

    if( pDamageInstance->m_X < 0.0f )
    {
        return;
    }

    if( pDamageInstance->m_IsMonsterDamage )
    {
        if( g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost )
        {
			MONSTER *pMonster = g_kMonsterManager.GetMonster( pDamageInstance->m_Who );
			if( pMonster != NULL ) {
				pMonster->SetX( pDamageInstance->m_X );
				pMonster->SetY( pDamageInstance->m_Y );
			}
        }
    }
    else
    {
        if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->IsLocalPlayer() )
        {
			if( !g_MyD3D->MyPlayer[pDamageInstance->m_Who]->IsCatchMotion( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion ) )
			{
				g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x = pDamageInstance->m_X;
				g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.y = pDamageInstance->m_Y;
				g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Body_Angle = 0.0f;
			}
        }
    }
}

int CHardDamagePortal::iIndex = 0;

void CHardDamagePortal::End( CDamageInstance* pDamageInstance )
{
    CDamage::End( pDamageInstance );

	if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->IsLocalPlayer() )
	{
		iIndex++;
		LTP_BUILD_PORTAL portal;
		portal.cPairID = pDamageInstance->m_Who * 20;
#if defined( NATION_KOREA )
        // 점령전이 25번 슬롯까지 이용하고 있으므 26번 슬롯부터 이용하도록
        // MAX_OBJECT 숫자 바꿔야함( 30 -> 40 )
        portal.iObjectIndex = 26 + pDamageInstance->m_Who + MAX_PLAYER_NUM * (iIndex % 2);
#else
        portal.iObjectIndex = 15 + pDamageInstance->m_Who + MAX_PLAYER_NUM * (iIndex % 2);
#endif
		portal.vPos.x = pDamageInstance->m_X;
		portal.vPos.y = pDamageInstance->m_Y;
		portal.iDamage = DT_MARI1_PORTAL_MODEL;
		g_LatencyTransfer.PushPacket( &portal );
	}
 
}

