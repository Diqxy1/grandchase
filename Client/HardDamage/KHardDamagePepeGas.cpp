#include "stdafx.h"
#include ".\KHardDamagePePeGas.h"


KHardDamagePepeGas::KHardDamagePepeGas(void)
{
}

KHardDamagePepeGas::~KHardDamagePepeGas(void)
{
}


bool KHardDamagePepeGas::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    // 플레이어에게는 콤보 카운트를 세지 않습니다. 하드하게 해서 죄송..... .... ... 하아...
    pDamageInstance->m_pOriginalDamage->m_dwDMAttribute = pDamageInstance->m_pOriginalDamage->m_dwDMAttribute | DA_NOT_COMBO_COUNT;
    if( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() )
    {        
		if ( pDamageInstance->InsertAlreadyDamaged( hitWho ) == false ) {
			return false;
		}

        CDamage::CrashPlayer( pDamageInstance , hitWho , fDamageRate);
    }

    return true;
}


void KHardDamagePepeGas::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
    //플레이어 한테는 한대만. 몬스터한테는 여러대.
	pDamageInstance->EraseAlreadyDamaged( Monsteri +MAX_PLAYER_NUM );

    // 몬스터에게는 콤보 카운트를 셉니다. 하드하게 해서 죄송..... .... ... 하아...
    pDamageInstance->m_pOriginalDamage->m_dwDMAttribute = pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & ! DA_NOT_COMBO_COUNT;
    CDamage::CrashMonster(pDamageInstance , Monsteri);
    
}
