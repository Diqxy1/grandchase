#include "stdafx.h"
#include "GCPetItembox.h"

IMPLEMENT_CLASSNAME( KGCPetItembox );
IMPLEMENT_WND_FACTORY( KGCPetItembox );
IMPLEMENT_WND_FACTORY_NAME( KGCPetItembox, "gc_user_equip_wnd" );

KGCPetItembox::KGCPetItembox( void )
{
	m_skillBuyBtn = NULL;
	LINK_CONTROL( "skill_buy_btn", m_skillBuyBtn );
}

KGCPetItembox::~KGCPetItembox( void )
{

	
}

void KGCPetItembox::ActionPerformed( const KActionEvent& event )
{
	if ( event.m_dwCode != KD3DWnd::D3DWE_BUTTON_CLICK )
		return;

	if ( m_skillBuyBtn != NULL && event.m_pWnd == m_skillBuyBtn ) {
		OnClickBuyBtn();
		return;
	}
}

void KGCPetItembox::OnClickBuyBtn( void )
{
	SCharInfo &sCharinfo = g_MyD3D->m_TempPlayer.GetCurrentChar();
	KPetInfo &kPetinfo = sCharinfo.kPetInfo;
	SPetTemplate m_sPet = g_kGlobalValue.GetPetTemplateFromItemID( kPetinfo.m_dwID, kPetinfo.m_cPromotion );
	GCItem* petEnquipItem = NULL;

	SUserInfo &sUserInfo = g_kGlobalValue.m_kUserInfo;
	
    
	petEnquipItem = g_pItemMgr->GetItemData( m_sPet.SkillGP );		//디폴트 GP 스킬을 구매하도록 셋팅 한다.	아무것도 장착이 안되어 있을 경우

    if( petEnquipItem != NULL ) {                                   // 에그상태가 아닐때
	    for ( int iPetEquipIndex = 0; iPetEquipIndex < (int)kPetinfo.m_vecEquipItem.size(); iPetEquipIndex++ ) {
		    if( kPetinfo.m_vecEquipItem[iPetEquipIndex].m_dwID == m_sPet.SkillGP || 
			    kPetinfo.m_vecEquipItem[iPetEquipIndex].m_dwID == m_sPet.SkillCash ||
			    kPetinfo.m_vecEquipItem[iPetEquipIndex].m_dwID == m_sPet.SkillAwakeningGP || 
			    kPetinfo.m_vecEquipItem[iPetEquipIndex].m_dwID == m_sPet.SkillAwakeningCash ) {
			    petEnquipItem = g_pItemMgr->GetItemData( kPetinfo.m_vecEquipItem[iPetEquipIndex].m_dwID );
			    break;
		    }		
	    }
    

	    if( sUserInfo.GetCurrentChar().GetCurrentGP() < (int)petEnquipItem->dwPrice ) {				//현재 보유 GP가 부족하다면 캐쉬스킬 셋팅 현재 각성 스킬머니가 0이기 때문에 통과
		    petEnquipItem = g_pItemMgr->GetItemData( m_sPet.SkillCash );
	    }

	    std::vector<GCItem*> vecItem;
	    if( petEnquipItem->eMoneyType == EMT_CASH ) {
		    vecItem = g_pItemMgr->GetCashShopItem( petEnquipItem->dwGoodsID );
	    } else if( petEnquipItem->eMoneyType == EMT_GAMEPOINT ) {
		    if ( petEnquipItem->bNotBuyItem == false ) {
			    GCItem* addItem = petEnquipItem;
			    vecItem.push_back(addItem);
		    } else {		//각성 스킬일 경우 EMT_GAMEPOINT 타입이다.
			    vecItem.clear();
			    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_SET_NOT_PETSKILL ), L"", KGCUIScene::GC_MBOX_USE_NORMAL );
			    //구매할수 없는 아이템입니다. 메시지 박스
		    }

	    } 
    	
	    for ( int iPetEquipIndex = 0; iPetEquipIndex < (int)vecItem.size(); iPetEquipIndex++ )
	    {
		    if( vecItem[iPetEquipIndex]->dwGoodsID == m_sPet.SkillGP ) { //GP 메시지 박스
#if defined( NATION_KOREA )
                g_pItemMgr->BuyGPItem( vecItem[ iPetEquipIndex ], vecItem[ iPetEquipIndex ]->dwGoodsID, 100 );
#else
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO,
									     g_pkStrLoader->GetReplacedString(STR_ID_BUY_POINT_ITEM, "li", vecItem[iPetEquipIndex]->strItemName, 
									     (int)vecItem[iPetEquipIndex]->dwPrice ), L"", KGCUIScene::GC_MBOX_USE_PET_SKILL_BUY, vecItem[iPetEquipIndex]->dwGoodsID, 100 );
#endif
		    } 
            else if( vecItem[iPetEquipIndex]->dwGoodsID == m_sPet.SkillCash ) 
            {  

#if defined ( USE_VIP_POINT )
                g_pItemMgr->BuyCashItemS6( vecItem[iPetEquipIndex]->dwGoodsID );
#else
    #ifdef GAME_CASH
                g_pkUIScene->EnableBuyCashItemWnd( vecItem[iPetEquipIndex]->dwGoodsID, false );
    #else    
                g_MyD3D->m_kItemMgr.BuyItem( vecItem[iPetEquipIndex]->dwGoodsID, -1  );
    #endif
#endif

		    }
            else if ( vecItem[iPetEquipIndex]->dwGoodsID == m_sPet.SkillAwakeningGP ) 
            { 
			    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO,
				    g_pkStrLoader->GetReplacedString(STR_ID_BUY_POINT_ITEM, "li", vecItem[iPetEquipIndex]->strItemName, 
				    (int)vecItem[iPetEquipIndex]->dwPrice ), L"", KGCUIScene::GC_MBOX_USE_PET_SKILL_BUY, vecItem[iPetEquipIndex]->dwGoodsID, 100 );
		    }
	    }
    } else {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_SET_NOT_PETSKILL ), L"", KGCUIScene::GC_MBOX_USE_NORMAL );
    }

}

void KGCPetItembox::OnCreate( void )
{
	if( m_skillBuyBtn != NULL ) {
		m_skillBuyBtn->InitState( false, true, this );
	}
	
}
void KGCPetItembox::CheckIsItemRedraw( void )
{
    if( m_skillBuyBtn == NULL ) {
        return;
    }
    if( g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo.m_dwID != 0 ) {
		m_skillBuyBtn->ToggleRender( true );

        if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM )
        {
            m_skillBuyBtn->ToggleRender( false );
        }


	} else {
		m_skillBuyBtn->ToggleRender( false );
	} 
}
