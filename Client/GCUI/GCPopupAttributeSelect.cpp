#include "stdafx.h"
#include "GCPopupAttributeSelect.h"

IMPLEMENT_CLASSNAME( KGCPopupAttributeSelect );
IMPLEMENT_WND_FACTORY( KGCPopupAttributeSelect );
IMPLEMENT_WND_FACTORY_NAME( KGCPopupAttributeSelect, "gc_popup_attribute_select " );

KGCPopupAttributeSelect::KGCPopupAttributeSelect( void )
: m_pkBtnUseScroll( NULL )
, m_pkBtnFreeSelect( NULL )
, m_pkSelectInvenItem( NULL )
, m_pkSelectItemData( NULL )
, m_pkBackMenu2( NULL )
, m_pkBackMenu3( NULL )
, m_dxvPos( 0.0f, 0.0f )
{
    LINK_CONTROL( "btn_use_scroll",         m_pkBtnUseScroll );
    LINK_CONTROL( "btn_use_scroll_single",  m_pkBtnUseScrollSingle );
    LINK_CONTROL( "btn_free_select",        m_pkBtnFreeSelect );
    LINK_CONTROL( "back2", m_pkBackMenu2 );
    LINK_CONTROL( "back3", m_pkBackMenu3 );    
}

KGCPopupAttributeSelect::~KGCPopupAttributeSelect( void )
{

}

void KGCPopupAttributeSelect::OnCreate( void )
{
    m_pkBtnUseScroll->InitState( true, true, this );
    m_pkBtnUseScrollSingle->InitState( true, true, this );
    m_pkBtnFreeSelect->InitState( true, true, this );
    m_pkBackMenu2->InitState( false );
    m_pkBackMenu3->InitState( true );

    m_pkBtnUseScroll->DisconnectAllEventProcedure();
    m_pkBtnUseScroll->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPopupAttributeSelect::OnClickUseScroll );

    m_pkBtnUseScrollSingle->DisconnectAllEventProcedure();
    m_pkBtnUseScrollSingle->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPopupAttributeSelect::OnClickUseScrollSingle );    

    m_pkBtnFreeSelect->DisconnectAllEventProcedure();
    m_pkBtnFreeSelect->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPopupAttributeSelect::OnClickFreeSelect );
    //m_pkBtnFreeSelect->SetToolTip( g_pkStrLoader->GetString( STR_ID_ATTRIBUTE_MIGRATION_TOOLTIP ), 0.0f, 60.0f );
}

void KGCPopupAttributeSelect::ActionPerformed( const KActionEvent& event )
{

}

void KGCPopupAttributeSelect::OnClickUseScroll( void )
{
    // 아이템 속성 정보 받고 속성선택 UI 세팅 
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ATTRIBUTE_SELECT_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
    g_pkUIScene->m_pkAttributeSelectBox->SetAttributeInfoRandom( m_pkSelectInvenItem->m_ItemUID, false, false );
    this->ToggleRender( false );
}

void KGCPopupAttributeSelect::OnClickUseScrollSingle( void )
{
    // 아이템 속성 정보 받고 속성선택 UI 세팅 
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ATTRIBUTE_SELECT_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
    g_pkUIScene->m_pkAttributeSelectBox->SetAttributeInfoRandom( m_pkSelectInvenItem->m_ItemUID, true, false );
    this->ToggleRender( false );
}

void KGCPopupAttributeSelect::OnClickFreeSelect( void )
{
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ATTRIBUTE_SELECT_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
    g_pkUIScene->m_pkAttributeSelectBox->SetAttributeInfoRandom( m_pkSelectInvenItem->m_ItemUID, false, true );
    this->ToggleRender( false );
}

void KGCPopupAttributeSelect::ShowPopupAttributeSelect( const GCITEMUID itemUID_, const D3DXVECTOR2 dxvPos_, bool bMigration_ )
{
    m_pkSelectInvenItem = NULL;
    m_pkSelectItemData = NULL;

    if( false == g_pItemMgr->FindInventory( itemUID_, &m_pkSelectInvenItem, &m_pkSelectItemData ) ) { 
        return;
    }

    m_dxvPos = dxvPos_;
    SetWindowPosDirect( m_dxvPos );
    m_pkBtnFreeSelect->SetToolTip( g_pkStrLoader->GetReplacedString( STR_ID_ATTRIBUTE_MIGRATION_TOOLTIP, "i", g_kGlobalValue.GetEnableAttributeMigrationCount() ), 0.0f, 60.0f );

    UpdateBackUI( bMigration_ );

    this->ToggleRender( true );
}

void KGCPopupAttributeSelect::UpdateBackUI( bool bMigration )
{
    m_pkBackMenu2->ToggleRender( !bMigration );
    m_pkBackMenu3->ToggleRender( bMigration );
    m_pkBtnFreeSelect->ToggleRender( bMigration );
}