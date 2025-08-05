#include "stdafx.h"
#include ".\HardDamagebomb2.h"



#include "Stage.h"
#include "MapSystem/GCFantasticMap.h"
#include "KGCRoomManager.h"
#include "Monster.h"

CHardDamageBomb2::CHardDamageBomb2(void)
{
}

CHardDamageBomb2::~CHardDamageBomb2(void)
{
}


void CHardDamageBomb2::FrameMove(CDamageInstance* pDamageInstance )
{
	float Length = ASMsqrt( m_x_Speed * m_x_Speed + m_y_Speed * m_y_Speed );
	float Angle = acosf( m_x_Speed / Length );

	float fPosY;
    if( SiKGCRoomManager()->IsMonsterGameMode() )
    {
        D3DXVECTOR2 dxvCrashPos;
        SiKGCFantasticMap()->CrashCheck( pDamageInstance->m_X, pDamageInstance->m_Y, dxvCrashPos );
        fPosY = dxvCrashPos.y;
    }
    else
    {
        fPosY = g_MyD3D->MyStg->Check_Contact( pDamageInstance->m_X, pDamageInstance->m_Y, 0).y;
    }
	if( fPosY != -1.0f )
	{
		pDamageInstance->m_Y = fPosY;
	}
	else
	{
		if( pDamageInstance->m_IsRight )
		{
			pDamageInstance->m_X		+= cosf(pDamageInstance->m_Angle + Angle) * Length;
			pDamageInstance->m_Angle	+= m_Angle_Speed;
			pDamageInstance->m_Y		+= sinf(pDamageInstance->m_Angle + Angle) * Length;
		}
		else
		{
			pDamageInstance->m_X		-= cosf(pDamageInstance->m_Angle + Angle) * Length;
			pDamageInstance->m_Angle	-= m_Angle_Speed;
			pDamageInstance->m_Y		-= sinf(pDamageInstance->m_Angle + Angle) * Length;
		}
	}


	if( pDamageInstance->m_pAnim != NULL )
	{
		int iLife = pDamageInstance->m_Life;

		float fX = pDamageInstance->m_X + pDamageInstance->m_fAnimScaleX;
		float fY = pDamageInstance->m_Y + pDamageInstance->m_fAnimScaleY;
		float fTransX = ( pDamageInstance->m_IsRight ? 0.01942f : -0.01942f );
		float fTransY = 0.00426f;
		float fScaleX = ( pDamageInstance->m_IsRight ? 0.000109f : -0.000109f )*iLife;
		float fScaleY = 0.000175f*iLife;

		pDamageInstance->m_pAnim->GetMatrix()->Move( fX+fScaleX+fTransX-1.0f, fY+fScaleY+fTransY-0.5f, 0.5f );

		if ( 0 != o3 )
			pDamageInstance->m_pAnim->SetShow( pDamageInstance->m_IsShow );
		else
			pDamageInstance->m_pAnim->SetShow( false );
		
		if(pDamageInstance->m_IsRight)
			pDamageInstance->m_pAnim->GetMatrix()->Scale(  pDamageInstance->m_fAnimScaleX, pDamageInstance->m_fAnimScaleY, 1.0f );
		else
			pDamageInstance->m_pAnim->GetMatrix()->Scale( -pDamageInstance->m_fAnimScaleX, pDamageInstance->m_fAnimScaleY, 1.0f );				


	}
}

bool CHardDamageBomb2::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
	float fPosY;
    if( SiKGCRoomManager()->IsMonsterGameMode() )
    {
        D3DXVECTOR2 dxvCrashPos;
        SiKGCFantasticMap()->CrashCheck( pDamageInstance->m_X, pDamageInstance->m_Y, dxvCrashPos );
        fPosY = dxvCrashPos.y;
    }
    else
    {
        fPosY = g_MyD3D->MyStg->Check_Contact( pDamageInstance->m_X, pDamageInstance->m_Y, 0).y;
    }
	if( fPosY == -1.0f )
	{
		CDamage::CrashPlayer(pDamageInstance,hitWho, fDamageRate );
		pDamageInstance->m_IsRight			= !pDamageInstance->m_IsRight;
		pDamageInstance->m_Angle			*= -1.0f; 
		return true;
	}
	return false;
}

void CHardDamageBomb2::CrashMonster(CDamageInstance* pDamageInstance, int Monsteri )
{
    float fPosY;
    if( SiKGCRoomManager()->IsMonsterGameMode() )
    {
        D3DXVECTOR2 dxvCrashPos;
        SiKGCFantasticMap()->CrashCheck( pDamageInstance->m_X, pDamageInstance->m_Y, dxvCrashPos );
        fPosY = dxvCrashPos.y;
    }
    else
    {
        fPosY = g_MyD3D->MyStg->Check_Contact( pDamageInstance->m_X, pDamageInstance->m_Y, 0).y;
    }

    if( fPosY == -1.0f )
    {
		MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_Who);

		if ( pMonster == NULL ) return;

        // 데미지 생성위치와 생성시 생성자위치를 넘겨준다.
        pMonster->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
        pMonster->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;

        pMonster->OnDamage( pDamageInstance->m_Who, pDamageInstance->m_What,
			0, ATTACKTYPE_NORMAL, ATTACKDIR_MIDDLE, 0.0f, 0.0f, true, pDamageInstance->m_pOriginalDamage->m_nSpecialGrade );


        pDamageInstance->m_IsRight			= !pDamageInstance->m_IsRight;
        pDamageInstance->m_Angle			*= -1.0f; 
    }
}
