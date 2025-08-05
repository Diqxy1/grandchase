#include "stdafx.h"
#include ".\HardDamageMagicMissile.h"

#include "Controls.h"

#include "GCCameraOrtho.h"


#include "Message.h"
#include "Procedure.h"
#include "Headup Display.h"

CHardDamageMagicMissile::CHardDamageMagicMissile(void)
{

}

CHardDamageMagicMissile::~CHardDamageMagicMissile(void)
{
}

void CHardDamageMagicMissile::Begin( CDamageInstance* pDamageInstance )
{
	CDamage::Begin( pDamageInstance );

    if( pDamageInstance->m_What == DT_BEIGAS1_SPECIAL_TYPE3_LV1_GUIDE_BALL )
        return;

	//====================================================================================
    // 2009.02.23 : Jemitgge
    // Comment : ���� ���� �ƴ϶�� ���� �ؽ��ĸ� ����!!
	//			 ��ƼŬ�� �ٲ����.. _-;
	if ( pDamageInstance->m_Who != g_MyD3D->Get_MyPlayer() )
	{
		//pDamageInstance->m_pOriginalDamage->m_Texture = 75;
		pDamageInstance->m_pOriginalDamage->m_vecChildParticle.clear();
	}

	//���Ÿ����� ������ �ʱ�ȭ�Ѵ�.
	if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->IsLocalPlayer())
	{
		g_kCamera.SavePrevState();

		g_kCamera.InitCamera();
		g_kCamera.m_fWideMode = 7.0f;
		// �� �ڽ��� Ÿ������ ī�޶� ��´�.
		g_kCamera.SetTargetPlayer(pDamageInstance->m_Who); 
		//ī�޶� �������� ��´�.     
		g_kCamera.InitCameraPosition();
	}
}

void CHardDamageMagicMissile::FrameMove( CDamageInstance* pDamageInstance )
{
	CDamage::FrameMove( pDamageInstance );

	//====================================================================================
    // 2009.02.23 : Jemitgge
    // Comment : ȭ��ǥ ��������~ ������ �̵�����!!
	//			 ������� �ڽ��� ������ ��ġ�� ��� �÷��̾�� �����ش�!
	if ( pDamageInstance->m_Who == g_MyD3D->Get_MyPlayer() )
	{
        if( pDamageInstance->m_What == DT_BEIGAS1_SPECIAL_TYPE3_LV1_GUIDE_BALL ) {
            if ( pDamageInstance->m_Life <= 1 ) 
                return;
            
            if( g_MyD3D->GetMyPlayer()->uiMotion == MID_BEIGAS1_SPECIAL_TYPE3_LV1 && g_MyD3D->MyHead->Level >= 0.f ) {
                if( g_MyD3D->GetMyPlayer()->cFrame >= 10 && 70 >= g_MyD3D->GetMyPlayer()->cFrame && pDamageInstance->m_Life < 45  ) {
                    if( g_MyD3D->MyCtrl->k_Fresh_Punch ) {
                        g_MyD3D->MyCtrl->k_Fresh_Punch = false;
                        g_MyD3D->MyHead->Level -= 0.2f;
                        pDamageInstance->m_Life += 5;

                        g_KMagicMissile->Type = GC_PID_MAGIC_MISSILE;
                        g_KMagicMissile->fPosX = pDamageInstance->m_X;
                        g_KMagicMissile->fPosY = pDamageInstance->m_Y;
                        g_KMagicMissile->iLife = 5;
                        SendNetworkData( GC_PID_MAGIC_MISSILE, 0 );
                    }
                }
            }
        } else {
            //  �̻��� ����ϸ� �� �����̰� �Ѵ�.
            if ( pDamageInstance->m_Life < 112 ) 
            {
                return;
            }

            if( g_MyD3D->MyCtrl->k_Right )
            {
                pDamageInstance->m_X += 0.03f;
            }
            if( g_MyD3D->MyCtrl->k_Left )
            {
                pDamageInstance->m_X -= 0.03f;
            }
            if ( g_MyD3D->MyCtrl->k_Up )
            {
                pDamageInstance->m_Y += 0.03f;
            }
            if ( g_MyD3D->MyCtrl->k_Down )
            {
                pDamageInstance->m_Y -= 0.03f;
            }            

            if ( o3 == 0 )
            {
                g_KMagicMissile->Type = GC_PID_MAGIC_MISSILE;
                g_KMagicMissile->fPosX = pDamageInstance->m_X;
                g_KMagicMissile->fPosY = pDamageInstance->m_Y;
                g_KMagicMissile->iLife = 0;
                SendNetworkData( GC_PID_MAGIC_MISSILE, 0 );
            }
        }        
	}
	else
	{
        if( pDamageInstance->m_What == DT_BEIGAS1_SPECIAL_TYPE3_LV1_GUIDE_BALL ) {
            pDamageInstance->m_Life += Result_Damage_Life;
            Result_Damage_Life = 0;		    
        } else {
            pDamageInstance->m_X = Result_Pos_MagicMissile.x;
            pDamageInstance->m_Y = Result_Pos_MagicMissile.y;
        }        
	}
}

void CHardDamageMagicMissile::End( CDamageInstance* pDamageInstance )
{
    if( pDamageInstance->m_What == DT_BEIGAS1_SPECIAL_TYPE3_LV1_GUIDE_BALL ) {
        CDamage::End( pDamageInstance );
        return;
    }        

	if ( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->IsLocalPlayer() )
	{
		g_kCamera.BackToPrevState();
		g_kCamera.InitCameraPosition();
	}
}
