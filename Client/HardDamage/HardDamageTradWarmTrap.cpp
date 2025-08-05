#include "stdafx.h"
#include ".\HardDamageTradWarmTrap.h"
#include "KGCPlayerCommon.h"
#include "Monster.h"
#include "KGCRoomManager.h"

CHardDamageTradWarmTrap::CHardDamageTradWarmTrap(void)
:dwTime(0) 
{
}

CHardDamageTradWarmTrap::~CHardDamageTradWarmTrap(void)
{
}


bool CHardDamageTradWarmTrap::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    if( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() || (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode()) )
	{
		if ( pDamageInstance->InsertAlreadyDamaged( hitWho ) == false ) {
			return false;
		}

		if ( pDamageInstance->m_HitWho != hitWho )
		{
			return false;
		}

		if ( pDamageInstance->m_emHitWhoType != OBJ_TYPE_PLAYER ) 
			return false;

		if( pDamageInstance->m_Who != MAX_PLAYER_NUM )
		{
			g_MyD3D->MyPlayer[hitWho]->SetLastKillmePlayer( pDamageInstance->m_Who );
		}		
		else
			g_MyD3D->MyPlayer[hitWho]->SetLastKillmePlayer( pDamageInstance->m_ExtraMonsterNum + MAX_PLAYER_NUM );

		dwTime = timeGetTime();
		g_Push_Count = 0;
        g_Max_Push = 20;
		g_MyD3D->MyPlayer[hitWho]->SetPlayerState(MID_COMMON_HOLD_NOEFFECT);
		g_MyD3D->MyPlayer[hitWho]->cFrame    = 0;			
		g_MyD3D->MyPlayer[hitWho]->m_bIsPushRightKey = g_MyD3D->MyPlayer[hitWho]->bIsRight;
		g_MyD3D->MyPlayer[hitWho]->bIsRight  = true;
		g_MyD3D->MyPlayer[hitWho]->x_Speed   = 0;
	}
	return true;
}

void CHardDamageTradWarmTrap::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
	if ( pDamageInstance->InsertAlreadyDamaged( Monsteri + MAX_PLAYER_NUM ) == false ) {
		return;
	}


    // 데미지 생성위치와 생성시 생성자위치를 넘겨준다.
    g_kMonsterManager.m_mapMonster[Monsteri]->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
    g_kMonsterManager.m_mapMonster[Monsteri]->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;

    // 몬스터에게 스톤커스.. 
    g_kMonsterManager.m_mapMonster[Monsteri]->OnDamage( pDamageInstance->m_Who, pDamageInstance->m_What,
		pDamageInstance->m_pOriginalDamage->m_fDamageToMonster * -1000.0f,
                                            ATTACKTYPE_HOLD, ATTACKDIR_NORMAL, 0.0f, 0.0f, true, pDamageInstance->m_pOriginalDamage->m_nSpecialGrade );
}

void CHardDamageTradWarmTrap::FrameMove( CDamageInstance* pDamageInstance )
{
	CDamage::FrameMove( pDamageInstance );

	if ( SiKGCRoomManager()->IsMonsterGameMode() )
		return;

	if( pDamageInstance->m_emHitWhoType == OBJ_TYPE_PLAYER ) 
	{
		PLAYER* pPlayer = g_MyD3D->MyPlayer[pDamageInstance->m_HitWho];

		//====================================================================================
		// 2009.02.18 : Jemitgge
		// Comment : 이상하게.. o10인데 20프레임 마다로.. 되어있다..
		if ( g_MyD3D->Get_MyPlayer() == pDamageInstance->m_HitWho )
		{
			if ( ( o10 == 0 || o10 == 10 ) )
			{
				int iLeftRightEffect = DT_JOYSTICK_LEFT_HIGH + ( (int)o10 % 3 );
				pPlayer->AddDamage( iLeftRightEffect, 0, true );
			}

			if ( g_Push_Count >= 20 
				|| ( dwTime + 5000 <= timeGetTime() 
					&& pDamageInstance->CheckAlreadyDamaged(g_MyD3D->Get_MyPlayer()) ) )
			{
				pDamageInstance->m_Life = 1;
				g_Push_Count = 20;

				return;
			}
		}
	}	
}

void CHardDamageTradWarmTrap::BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY )
{
	CDamage::BeginWithLocate( pDamageInstance, fStartX, fStartY );

	g_Push_Count = 0;
}