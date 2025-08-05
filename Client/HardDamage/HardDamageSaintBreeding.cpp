#include "stdafx.h"
#include ".\HardDamageSaintBreeding.h"

#include "Monster.h"
#include "KGCRoomManager.h"


CHardDamageSaintBreeding::CHardDamageSaintBreeding(void)
{
    m_iDamageTime = 0;
}

CHardDamageSaintBreeding::~CHardDamageSaintBreeding(void)
{
}

//void CHardDamageSaintBreeding::Begin( CDamageInstance* pDamageInstance )
//{
//	CDamage::Begin( pDamageInstance );
//}

void CHardDamageSaintBreeding::NotAngleFrameMove(CDamageInstance* pDamageInstance )
{
    //	CDamage::FrameMove( pDamageInstance );
    if( pDamageInstance->m_What == DT_ARME_SKILL_SAINTBREEDING_SPACE )
    {
        if( SiKGCRoomManager()->IsMonsterGameMode() )
        {
			MONSTER *pMonster = g_kMonsterManager.GetMonster( pDamageInstance->m_HitWho );
			if( pMonster == NULL ) return;

            pDamageInstance->m_X = pMonster->GetX();
            pDamageInstance->m_Y = pMonster->GetY() + 0.2f;

            if( pMonster->m_fHP < 0 )
                pDamageInstance->m_Life = 1;
        }
        else
        {

			if ( g_MyD3D->IsPlayerIndex( pDamageInstance->m_HitWho ) && 
				pDamageInstance->m_emHitWhoType == OBJ_TYPE_PLAYER ) 
			{
				pDamageInstance->m_X = g_MyD3D->MyPlayer[pDamageInstance->m_HitWho]->GetPosition().x;
				pDamageInstance->m_Y = g_MyD3D->MyPlayer[pDamageInstance->m_HitWho]->GetPosition().y + 0.2f;		

				if( g_MyD3D->MyPlayer[pDamageInstance->m_HitWho]->GetHP() < 0 )
					pDamageInstance->m_Life = 1;
			}
			else if ( g_kMonsterManager.IsMonsterIndex( pDamageInstance->m_HitWho ) &&
				 pDamageInstance->m_emHitWhoType == OBJ_TYPE_MONSTER )
			{
				MONSTER *pMonster = g_kMonsterManager.GetMonster( pDamageInstance->m_HitWho );
				if( pMonster == NULL ) return;

				pDamageInstance->m_X = pMonster->GetX();
				pDamageInstance->m_Y = pMonster->GetY() + 0.2f;

				if( pMonster->m_fHP < 0 )
					pDamageInstance->m_Life = 1;
			}

        }

        if( m_iDamageTime < 385 )
        {
            pDamageInstance->m_Life = 10;		
            m_iDamageTime++;
        }

        if( m_iDamageTime == 385 )
            pDamageInstance->m_Life	= 1;			                        
    }	
    else
    {
        if( SiKGCRoomManager()->IsMonsterGameMode() )
        {
			MONSTER *pMonster = g_kMonsterManager.GetMonster( pDamageInstance->m_HitWho );
			if( pMonster == NULL ) return;

            pDamageInstance->m_X = pMonster->GetX();
            pDamageInstance->m_Y = pMonster->GetY() + 0.1f;
            if( pMonster->m_fHP < 0 )
                pDamageInstance->m_Life = 1;
        }
        else
        {
			if ( g_MyD3D->IsPlayerIndex( pDamageInstance->m_HitWho ) && 
				pDamageInstance->m_emHitWhoType == OBJ_TYPE_PLAYER ) 
			{
                pDamageInstance->m_X = g_MyD3D->MyPlayer[pDamageInstance->m_HitWho]->GetPosition().x;
                pDamageInstance->m_Y = g_MyD3D->MyPlayer[pDamageInstance->m_HitWho]->GetPosition().y + 0.1f;	
                if( g_MyD3D->MyPlayer[pDamageInstance->m_HitWho]->GetHP() < 0 )
                    pDamageInstance->m_Life = 1;
            }
			else if ( g_kMonsterManager.IsMonsterIndex( pDamageInstance->m_HitWho ) &&
				pDamageInstance->m_emHitWhoType == OBJ_TYPE_MONSTER )
            {
				MONSTER *pMonster = g_kMonsterManager.GetMonster( pDamageInstance->m_HitWho );
				if( pMonster == NULL ) return;

                pDamageInstance->m_X = pMonster->GetX();
                pDamageInstance->m_Y = pMonster->GetY() + 0.1f;
                if( pMonster->m_fHP < 0 )
                    pDamageInstance->m_Life = 1;
            }
        }
    }
}

