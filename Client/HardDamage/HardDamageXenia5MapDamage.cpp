#include "stdafx.h"
#include "./HardDamageXenia5MapDamage.h"
#include "./QuestGameManager.h"
#include "./gcui/GCGameOverlayUI.h"
#include "DamageManager.h"

CHardDamageXeniaMapDamage::CHardDamageXeniaMapDamage(void)
{
	pDamage = NULL;
	fCheckContactTime = 0.0f;
}

CHardDamageXeniaMapDamage::~CHardDamageXeniaMapDamage(void)
{
    //  ���� �Ǵ� Ÿ�ֿ̹� �������� ���� ������ �����.
    if ( pDamage ) 
    {
        pDamage->m_Life = 1;
    }
}


bool CHardDamageXeniaMapDamage::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
	if ( !g_MyD3D->IsPlayerIndex( hitWho ) )
		return false;

	if ( g_MyD3D->MyPlayer[hitWho]->IsUseItemEffect( ITM_EFF_IMMUN_XENIA5DOTDAMAGE ) )
		return false;

	PLAYER* pPlayer = g_MyD3D->MyPlayer[hitWho];

	if ( g_pkQuestManager->m_bClearStage || g_pkGameOverlayUI->IsShowDialogue() || !pPlayer->IsLocalPlayer())
		return false;

    if( pPlayer->IsContact &&
        pPlayer->IsFatal() == false &&
        pPlayer->CheckInPortal() == false && 
        pPlayer->uiMotion != MID_NEPHILLIM_WAIT_TELEPORT && 
        pPlayer->uiMotion != MID_RYAN4_MDAN_WAIT_TELEPORT) {
        fCheckContactTime += GC_ELAPSED_TIME * 5.0f;
    } else {
        fCheckContactTime = 0.0f;
        //	Life = 1�� �����ϸ� �Ŵ������� ������ �����. NULL�� �����ؾ��մϴ�.
        if( ( pPlayer->IsContact == false || pPlayer->IsFatal() ) && pDamage ) {
            pDamage->m_Life = 1;
            pDamage = NULL;
            return false;
        }
    }         

    if ( (fCheckContactTime > 5.0f) && pPlayer->IsFatal() == false ) {
        fCheckContactTime = 0.0f;

		g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->m_kUserInfo.iTeam = ETRed;
        
        //  5�ʸ��� ������ ���� ����������, ���� ������ �������� �ξ� ���.
        //  ���� ������ �����ְ� ���� �����.
        if ( pDamage ) 
        {
            pDamage->m_Life = 1;
        }

		pDamage = g_MyD3D->m_pDamageManager->AddWithLocate(DT_XENIA5_MAP_DOTDAMAGE, MAX_PLAYER_NUM, pPlayer->vPos.x, pPlayer->vPos.y, false, 0, NULL, hitWho );        

        if ( pDamage ) {
		    pDamage->m_HitWho = hitWho;
        }
	}    

    return true;
}

void CHardDamageXeniaMapDamage::FrameMove( CDamageInstance* pDamageInstance )
{
}