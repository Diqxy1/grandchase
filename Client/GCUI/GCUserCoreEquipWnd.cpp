#include "stdafx.h"
#include "GCUserCoreEquipWnd.h"
//
#include "GCUI/GCMyInfoScene.h"
#include "../MyD3D.h"

//

#include "GCItemInfoBox.h"

IMPLEMENT_CLASSNAME( KGCUserCoreEquipWnd );
IMPLEMENT_WND_FACTORY( KGCUserCoreEquipWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCUserCoreEquipWnd, "gc_user_core_equip_wnd" );

KGCUserCoreEquipWnd::KGCUserCoreEquipWnd( void ) : m_pkSocketWnd(NULL)
{
	LINK_CONTROL( "socket",				m_pkSocketWnd );
#ifdef WEAPONCHANGE
    m_pWCBack = NULL;
    m_pWCLockBtn = NULL;
    m_pWCBtn = NULL;
    LINK_CONTROL( "WC_back",        m_pWCBack );
    LINK_CONTROL( "WC_lock_btn",    m_pWCLockBtn );
    LINK_CONTROL( "WC_btn",         m_pWCBtn );
#endif
}

KGCUserCoreEquipWnd::~KGCUserCoreEquipWnd( void )
{
}

void KGCUserCoreEquipWnd::OnCreate( void )
{
 
    if( m_pkActiveFrame != NULL ) {
        m_pkActiveFrame->InitState( false );
    }

	m_pkItemName->SetSelfInputCheck( false );
	m_pkItemName->SetOffset( D3DXVECTOR2( 0.0f, 1.0f ) );
	m_pkSocketWnd->InitState( false, true, this );

#ifdef WEAPONCHANGE
    m_pWCBack->InitState( false, false, NULL );
    m_pWCBtn->InitState( false, false, NULL );
    m_pWCLockBtn->InitState( false, false, NULL );
#endif
}

void KGCUserCoreEquipWnd::ActionPerformed( const KActionEvent& event )
{	
	if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
		g_KDSound.Play( "30" );

    if ( event.m_dwCode != KD3DWnd::D3DWE_BUTTON_CLICK )
        return;


#ifdef WEAPONCHANGE
    if ( event.m_pWnd == m_pWCBtn )
    {
        if ( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iLevel < WEAPONCHANGE_LV )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_WC_STRING3 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL );
            return;
        }
        else
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_WEAPON_CHANGE_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, true );
        }
    }
    else if ( event.m_pWnd == m_pWCLockBtn )
    {
        if ( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iLevel < WEAPONCHANGE_LV )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_WC_STRING3 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL );
            return;
        }
        
        GCItem* pkGCItem = g_MyD3D->m_kItemMgr.GetItemData( ITEM_ID_WC_UNLOCK );
        KItem* pkItem;
        bool bIsIn = g_MyD3D->m_kItemMgr.FindInventoryForItemID( ITEM_ID_WC_UNLOCK, &pkItem );

        if ( NULL == pkGCItem )
            return;

        int iQuantity = 0;
        if ( bIsIn == true )
        {
            iQuantity = pkItem->m_nCount;
        }

        std::wstring str = g_pkStrLoader->GetReplacedString( STR_ID_WC_STRING5, "li", pkGCItem->strItemName, iQuantity );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, str, L"", KGCUIScene::GC_MBOX_USE_WEAPON_CHANGE );
    }
#endif
}

#ifdef WEAPONCHANGE
void KGCUserCoreEquipWnd::SetWCBtn( bool bLock_ )
{
    m_pWCBtn->InitState( bLock_, bLock_, bLock_ ? this : NULL );
    m_pWCLockBtn->InitState( !bLock_, !bLock_, !bLock_ ? this : NULL );
}
#endif
