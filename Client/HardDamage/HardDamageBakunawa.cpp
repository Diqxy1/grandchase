#include "stdafx.h"
#include "Controls.h"
#include ".\HardDamageBakunawa.h"
#include "Controls.h"
#include "Buff\KGCBuffManager.h"

CHardDamageBakunawa::CHardDamageBakunawa( void )
{

}

CHardDamageBakunawa::~CHardDamageBakunawa( void )
{

}

void CHardDamageBakunawa::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
	CheckAndDebuff(pDamageInstance);
    
    // 끌어당기는 데미지 m_fPushTargetX를 다시 계산하자
    // PUSH_X_TO_MONSTER, PUSH_Y_TO_MONSTER 이걸로는 버그가 생긴다.
    if( pDamageInstance->m_What == DT_PET_BAKUNAWA_SPECIAL_PULL1 || 
        pDamageInstance->m_What == DT_PET_BAKUNAWA_SPECIAL_PULL2 ||
        pDamageInstance->m_What == DT_PET_BAKUNAWA_SPECIAL_PULL3 ||
        pDamageInstance->m_What == DT_PET_BAKUNAWA_SPECIAL_PULL4 ) {

            PLAYER* pkPlayer =  g_MyD3D->GetMyPlayer();
            D3DXVECTOR3* pvPlayerPos = &(pkPlayer->GetPosition());

            KPet* pkPet = g_MyD3D->m_akPet[pDamageInstance->m_Who];
            D3DXVECTOR2* pvPetPos = pkPet->GetPos();

            if( pvPlayerPos->x > pvPetPos->x && pDamageInstance->m_IsRight == true ||
                pvPlayerPos->x < pvPetPos->x && pDamageInstance->m_IsRight == false ){
                pDamageInstance->m_pOriginalDamage->m_fDMXSpeed *= -1.0f;
                pDamageInstance->m_pOriginalDamage->m_fPushTargetX *= -1.0f;
            } 
    }

	CDamage::CrashMonster(pDamageInstance,Monsteri);
	return;
}

void CHardDamageBakunawa::CheckAndDebuff(CDamageInstance* pDamageInstance)
{
	if ( pDamageInstance->m_What == DT_PET_BAKUNAWA_SPECIAL_BLACKHOLL ||
        pDamageInstance->m_What == DT_PET_BAKUNAWA_SPECIAL_BOOM )
	{
        if( g_pMagicEffect->IsMagicEffect(pDamageInstance->m_Who,EGC_EFFECT_BAKUNAWA_SP_UP) == false &&
            g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Is_SP_Character()==true )
		{
			g_MyD3D->MyPlayer[pDamageInstance->m_Who]->DecreaseMP(0.4f);
			g_pMagicEffect->SetMagicEffect(pDamageInstance->m_Who,EGC_EFFECT_BAKUNAWA_SP_UP, 5.0f);
		}
	}
}

bool CHardDamageBakunawa::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    CheckAndDebuff(pDamageInstance);

    // 끌어당기는 데미지 m_fPushTargetX를 다시 계산하자
    // PUSH_X_TO_MONSTER, PUSH_Y_TO_MONSTER 이걸로는 버그가 생긴다.
    if( pDamageInstance->m_What == DT_PET_BAKUNAWA_SPECIAL_PULL1 || 
        pDamageInstance->m_What == DT_PET_BAKUNAWA_SPECIAL_PULL2 ||
        pDamageInstance->m_What == DT_PET_BAKUNAWA_SPECIAL_PULL3 ||
        pDamageInstance->m_What == DT_PET_BAKUNAWA_SPECIAL_PULL4 ) {
            PLAYER* pkPlayer =  g_MyD3D->GetMyPlayer();
            D3DXVECTOR3* pvPlayerPos = &(pkPlayer->GetPosition());

            KPet* pkPet = g_MyD3D->m_akPet[pDamageInstance->m_Who];
            D3DXVECTOR2* pvPetPos = pkPet->GetPos();

            if( pvPlayerPos->x > pvPetPos->x && pDamageInstance->m_IsRight == true ||
                pvPlayerPos->x < pvPetPos->x && pDamageInstance->m_IsRight == false ){
                    pDamageInstance->m_pOriginalDamage->m_fDMXSpeed *= -1.0f;
                    pDamageInstance->m_pOriginalDamage->m_fPushTargetX *= -1.0f;
            } 
    }

	CDamage::CrashPlayer(pDamageInstance, hitWho ,fDamageRate);
	return true;
}