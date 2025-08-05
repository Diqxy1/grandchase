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
    // �÷��̾�Դ� �޺� ī��Ʈ�� ���� �ʽ��ϴ�. �ϵ��ϰ� �ؼ� �˼�..... .... ... �Ͼ�...
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
    //�÷��̾� ���״� �Ѵ븸. �������״� ������.
	pDamageInstance->EraseAlreadyDamaged( Monsteri +MAX_PLAYER_NUM );

    // ���Ϳ��Դ� �޺� ī��Ʈ�� ���ϴ�. �ϵ��ϰ� �ؼ� �˼�..... .... ... �Ͼ�...
    pDamageInstance->m_pOriginalDamage->m_dwDMAttribute = pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & ! DA_NOT_COMBO_COUNT;
    CDamage::CrashMonster(pDamageInstance , Monsteri);
    
}
