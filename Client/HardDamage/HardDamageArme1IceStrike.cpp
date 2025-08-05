#include "stdafx.h"
#include ".\HardDamageArme1IceStrike.h"


#include "Monster.h"

CHardDamageArme1IceStrike::CHardDamageArme1IceStrike(void)
{
	m_bIsOn = false;
}

CHardDamageArme1IceStrike::~CHardDamageArme1IceStrike(void)
{
}


bool CHardDamageArme1IceStrike::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
	if( !m_bIsOn )
	{
        if( !g_MyD3D->IsPlayerIndex(hitWho) )
            return false;

		g_MyD3D->MyPlayer[hitWho]->SetPlayerState( MID_SKILL_ARME1_ICESTRIKE_FREEZING );
		g_MyD3D->MyPlayer[hitWho]->cFrame = 0;
		m_bIsOn = true;
	}
    return true;
}


void CHardDamageArme1IceStrike::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
    //플레이어 한테는 한대만. 몬스터한테는 여러대.
    //pDamageInstance->m_bAlreadyDamaged[ Monsteri + MAX_PLAYER_NUM ] = false;
	MONSTER *pMonster = g_kMonsterManager.GetMonster(Monsteri);
	if(pMonster == NULL)
		return;

    // 데미지 생성위치와 생성시 생성자위치를 넘겨준다.
    pMonster->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
    pMonster->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;
#if defined( LEVEL_DESIGN_STAT_CALC )	
	pMonster->OnDamage( pDamageInstance->m_Who,pDamageInstance->m_What,
		pDamageInstance->m_pOriginalDamage->m_fDamageToMonster * -1.0f,
		ATTACKTYPE_ICECURSE, ATTACKDIR_NORMAL, 0.0f, 0.0f, true, pDamageInstance->m_pOriginalDamage->m_nSpecialGrade );
#else
	pMonster->OnDamage( pDamageInstance->m_Who,pDamageInstance->m_What,
		pDamageInstance->m_pOriginalDamage->m_fDamageToMonster * -1000.0f,
		ATTACKTYPE_ICECURSE, ATTACKDIR_NORMAL, 0.0f, 0.0f, true, pDamageInstance->m_pOriginalDamage->m_nSpecialGrade );
#endif
    
    CDamage::CrashMonster(pDamageInstance , Monsteri);
}
