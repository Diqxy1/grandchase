#include "stdafx.h"
#include ".\harddamagephoenixarrow.h"
#include "Stage.h"
#include "DamageManager.h"

#include "GCCameraOrtho.h"
#include "MapSystem/GCFantasticMap.h"
#include "KGCRoomManager.h"
#include "Monster.h"
#include "LatencyTransfer.h"

CHardDamagePhoenixArrow::CHardDamagePhoenixArrow(void)
{
}

CHardDamagePhoenixArrow::~CHardDamagePhoenixArrow(void)
{
}

#define ARROWSTART (64)

void CHardDamagePhoenixArrow::Begin( CDamageInstance* pDamageInstance )
{
	CDamage::Begin( pDamageInstance );

	if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame > ARROWSTART - 1 )
	{
		if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame % 2 == 0 )
		{
			if( pDamageInstance->m_IsRight )
				pDamageInstance->m_X -= (float)(g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame - ARROWSTART) * 0.14f;
			else 
				pDamageInstance->m_X += (float)(g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame - ARROWSTART) * 0.14f;
		}
		else
		{
			if( pDamageInstance->m_IsRight )
				pDamageInstance->m_X += (float)(g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame - ARROWSTART) * 0.14f;
			else 
				pDamageInstance->m_X -= (float)(g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame - ARROWSTART) * 0.14f;
		}		
	}
}

void CHardDamagePhoenixArrow::NotAngleFrameMove(CDamageInstance* pDamageInstance )
{
	CDamage::NotAngleFrameMove(pDamageInstance);

	if( pDamageInstance->m_Life < 210 )
    {
        float fPosY;
        if( SiKGCRoomManager()->IsMonsterGameMode() )
        {
            D3DXVECTOR2 dxvCrashPos;
            SiKGCFantasticMap()->CrashCheck( pDamageInstance->m_X, pDamageInstance->m_Y - 0.2f, dxvCrashPos );
            fPosY = dxvCrashPos.y;
        }
        else
        {
            fPosY = g_MyD3D->MyStg->Check_Contact( pDamageInstance->m_X, pDamageInstance->m_Y - 0.2f, 0 ).y;
        }

		if( fPosY != -1.0f )
	    {
		    if( pDamageInstance->m_Life > -m_Life_Change )
			    pDamageInstance->m_Life = -m_Life_Change;

		    pDamageInstance->m_pOriginalDamage->m_y_Speed = 0.0f;

		    if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.y > 0 
			    && g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x > 0 
                && g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x < g_MyD3D->MyStg->GetXLimit() )
		    {
			    g_kCamera.SetEarthQuake(15);

			    g_MyD3D->m_pDamageManager->AddWithLocate(DT_PHOENIX_ARROW_WAVE1, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y - 0.17f, false, pDamageInstance->m_ExtraMonsterNum );
			    g_MyD3D->m_pDamageManager->AddWithLocate(DT_PHOENIX_ARROW_WAVE2, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y - 0.17f, false, pDamageInstance->m_ExtraMonsterNum );
			    g_MyD3D->m_pDamageManager->AddWithLocate(DT_PHOENIX_ARROW_WAVE3, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y - 0.17f, false, pDamageInstance->m_ExtraMonsterNum );
		    }
	    }
    }
}

CHardDamageLire2Drill::CHardDamageLire2Drill( void )
{
	m_bHitted = false;
    m_bFirst = true;
    m_iHittedMonsterID = -1;
}

CHardDamageLire2Drill::~CHardDamageLire2Drill( void )
{

}

bool CHardDamageLire2Drill::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate /*= 1.0f */ )
{
	m_bHitted = true;
	return CDamage::CrashPlayer( pDamageInstance, hitWho, fDamageRate );

}

void CHardDamageLire2Drill::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
	m_bHitted = true;
    MONSTER *pMonster = g_kMonsterManager.GetMonster(Monsteri);
    if( pMonster )
        m_iHittedMonsterID = pMonster->m_iMonsterType;
	return CDamage::CrashMonster( pDamageInstance, Monsteri );

}

void CHardDamageLire2Drill::CrashObject( CDamageInstance* pDamageInstance, int iObjIndex )
{
	m_bHitted = true;
	return CDamage::CrashObject( pDamageInstance, iObjIndex );

}

void CHardDamageLire2Drill::CrashEffect( CDamageInstance* pDamageInstance )
{
	m_bHitted = true;
	return CDamage::CrashEffect( pDamageInstance );

}

void CHardDamageLire2Drill::FrameMove( CDamageInstance* pDamageInstance )
{
    if( m_bFirst ) {
        m_fOriginalSpeedX = m_x_Speed;
        m_fOriginalSpeedY = m_y_Speed;
        m_bFirst = false;
    }

    if( pDamageInstance->m_What == DT_RIN_SPECIAL4_BALL_FIRE_1 ||
        pDamageInstance->m_What == DT_RIN_SPECIAL4_BALL_FIRE_1 ||
        pDamageInstance->m_What == DT_RIN_SPECIAL4_BALL_FIRE_2 ||
        pDamageInstance->m_What == DT_RIN_SPECIAL4_BALL_FIRE_3 ||
        pDamageInstance->m_What == DT_RIN_SPECIAL4_BALL_FIRE_4 ||
        pDamageInstance->m_What == DT_RIN_SPECIAL4_BALL_FIRE_5 ||
        pDamageInstance->m_What == DT_RIN_SPECIAL4_BALL_FIRE_6 ||
        pDamageInstance->m_What == DT_RIN_SPECIAL4_BALL_FIRE_UP ||
        pDamageInstance->m_What == DT_RIN_SPECIAL4_BALL_FIRE_DOWN )
    {
        if( m_iHittedMonsterID == MON_BERKASLAIR_BERKAS ||
            m_iHittedMonsterID == MON_BERKASLAIR_HATCHLING ||
            m_iHittedMonsterID == MON_BERKASLAIR_LIZARDMAN ||
            m_iHittedMonsterID == MON_BERKASLAIR_DRAGOON )
        {
            m_bHitted = false;
        }
    }

	if( m_bHitted )
	{
		m_x_Speed = m_fOriginalSpeedX * 0.2f;
		m_y_Speed = m_fOriginalSpeedY * 0.2f;
	}
	else
	{
		m_x_Speed = m_fOriginalSpeedX;
		m_y_Speed = m_fOriginalSpeedY;
	}

	if ( !o5 )
	{	
		m_bHitted = false;
	}
	CDamage::FrameMove( pDamageInstance );
}

void CHardDamageLire2Drill::Begin( CDamageInstance* pDamageInstance )
{
	m_fOriginalSpeedX = m_x_Speed;
	m_fOriginalSpeedY = m_y_Speed;

	CDamage::Begin( pDamageInstance );	
}