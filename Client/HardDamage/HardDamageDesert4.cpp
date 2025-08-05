#include "stdafx.h"
#include ".\HardDamageDesert4.h"
#include "Monster.h"
#include "KGCRoomManager.h"
#include "Headup Display.h"
#include "DamageManager.h"
#include "Buff/KGCBuffManager.h"

CHardDamageDesert4DungFly::CHardDamageDesert4DungFly( void )
{

}

CHardDamageDesert4DungFly::~CHardDamageDesert4DungFly( void )
{

}

void CHardDamageDesert4DungFly::BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY )
{
    // 살아 있는 모든 플레이어에게 데미지 발사!!!
    if ( SiKGCRoomManager()->IsMonsterGameMode() )
    {
        int iCreateDamageEnum = DT_SANDRULER_ATK4;
        if( pDamageInstance->m_What == DT_SUPERMON_SANDRULER_ATK4_LAUNCHER ) { 
            iCreateDamageEnum = DT_SUPERMON_SANDRULER_ATK4;
        }

        for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
        {
            if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && g_MyD3D->MyPlayer[i]->m_cLife > 0 )
            {
                CDamageInstance* damage = NULL;
                damage = g_MyD3D->m_pDamageManager->MonsterAddWithLocate( iCreateDamageEnum, pDamageInstance->m_ExtraMonsterNum, fStartX, fStartY );
                damage->m_HitWho = i;
            }
        }
    }
}

void CHardDamageDesert4DungFly::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
    MONSTER* pMonster = g_kMonsterManager.GetMonster( Monsteri );
    if( pMonster )
    {
        // 데미지 생성위치와 생성시 생성자위치를 넘겨준다.
        pMonster->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
        pMonster->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;
    }

	return;
}

bool CHardDamageDesert4DungFly::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{

	return true;
}



CHardDamageDesert4Pot::CHardDamageDesert4Pot( void )
{

}

CHardDamageDesert4Pot::~CHardDamageDesert4Pot( void )
{

}

bool CHardDamageDesert4Pot::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    //ONEHIT데미지라면 한번만 맞게 한다.
	if ( pDamageInstance->InsertAlreadyDamaged( hitWho ) == false ) {
		return false;
	}

    //if (  pDamageInstance->m_bAlreadyDamaged[hitWho] && (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_ONEHIT_DAMAGE) )
    //{
    //    return false;
    //}
    //else
    //{
    //    pDamageInstance->m_bAlreadyDamaged[hitWho] = true;
    //}

    if ( hitWho == g_MyD3D->Get_MyPlayer() )
    {
        PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
        switch ( pDamageInstance->m_What )
        {
        case DT_MIRAGE_POT_HP:
            {
                pPlayer->SetHP( pPlayer->GetHP() + (pPlayer->m_fAbility[ABILITY_HP] * 0.25f) );

                if( pPlayer->GetHP() >= pPlayer->m_fAbility[ABILITY_HP] )
                    pPlayer->SetHPFull();
            }
            break;
        case DT_MIRAGE_POT_MP:
            {
                g_MyD3D->MyHead->Level = g_kGlobalValue.GetMaxMP();
            }
            break;
        case DT_MIRAGE_POT_DEC_MP:
            {
                g_MyD3D->MyHead->Level = 0.0f;
            }
            break;
        }
    }

    return true;
}

void CHardDamageDesert4Pot::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
    MONSTER* pMonster = g_kMonsterManager.GetMonster( Monsteri );
    if( pMonster )
    {
        // 데미지 생성위치와 생성시 생성자위치를 넘겨준다.
        pMonster->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
        pMonster->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;
    }

    return;
}