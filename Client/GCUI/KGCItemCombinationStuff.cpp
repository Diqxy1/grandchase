#include "stdafx.h"
#include "KGCItemCombinationStuff.h"

IMPLEMENT_CLASSNAME( KGCItemCombinationStuff );
IMPLEMENT_WND_FACTORY( KGCItemCombinationStuff );
IMPLEMENT_WND_FACTORY_NAME( KGCItemCombinationStuff, "gc_item_combination_stuff" );

KGCItemCombinationStuff::KGCItemCombinationStuff( void )
: m_pkItemImg(NULL)
, m_pkSeleted(NULL)
, m_pkItemActiveFrame(NULL)
, m_pkBlockWnd(NULL)
, m_bSelect(false)
{
    LINK_CONTROL("item_combination_stuff" , m_pkItemActiveFrame );
    LINK_CONTROL("item_img" , m_pkItemImg );
    LINK_CONTROL("select_text" , m_pkSeleted );
    LINK_CONTROL("block_wnd", m_pkBlockWnd );

}

KGCItemCombinationStuff::~KGCItemCombinationStuff( void )
{
}

void KGCItemCombinationStuff::ActionPerformed( const KActionEvent& event )
{
}

void KGCItemCombinationStuff::OnCreate( void )
{
    m_pkItemImg->InitState( true );
    if ( m_pkItemActiveFrame != NULL )
        m_pkItemActiveFrame->InitState( false);
    if ( m_pkSeleted != NULL)
        m_pkSeleted->InitState( false );
    if ( m_pkBlockWnd != NULL )
        m_pkBlockWnd->InitState( false );
}

void KGCItemCombinationStuff::SetItemInfo( const GCITEMID itemID_, const GCITEMUID itemUID_ /*= 0*/ )
{
    m_ItemID = itemID_;
    m_ItemUID = itemUID_;
    m_pkItemImg->SetItemInfo( itemID_, -1, false, -1, itemUID_  );
    m_pkItemImg->ShowItem( itemID_ != 0 );

}

GCITEMID KGCItemCombinationStuff::GetItemID()
{
    return m_ItemID;
}


void KGCItemCombinationStuff::SetShowItem( bool _bShow )
{
    m_pkItemImg->ShowItem( _bShow );
}

void KGCItemCombinationStuff::SetBlockWnd( bool _bShow )
{
    if ( m_pkBlockWnd != NULL)
        m_pkBlockWnd->ToggleRender( _bShow );
}

void KGCItemCombinationStuff::FrameMoveInEnabledState( void )
{
    POINT   pt = g_pkInput->GetMousePos();
    bool    bMouseHover = CheckPosInWindowBound( D3DXVECTOR2( (float)pt.x, (float)pt.y ) );

    if( m_pkItemActiveFrame != NULL ) {
        m_pkItemActiveFrame->SetWndMode( m_eMode );
        m_pkItemActiveFrame->ToggleRender( bMouseHover || m_bSelect );
    }
}

void KGCItemCombinationStuff::SetSelect( bool bSelect )
{
    m_bSelect = bSelect;
    if ( m_pkSeleted != NULL )
        m_pkSeleted->ToggleRender( bSelect );
    if( m_pkItemActiveFrame != NULL ) 
        m_pkItemActiveFrame->ToggleRender( false );
}

void KGCItemCombinationStuff::PostChildDraw( void )
{
    if( false == m_pkItemImg->IsShowItem() ) { 
        return;
    }

    // 강화석일때 강화 단계를 표시해 줍니다. 
    if( m_ItemID == 0 || m_ItemUID == 0 ) { 
        return;
    }

    GCItem* pItemInfo = NULL;
    KItem*  pInvenItem = NULL;
    if( false == g_pItemMgr->FindInventory( m_ItemUID, &pInvenItem, &pItemInfo ) ) { 
        return;
    }

    if( pItemInfo->eItemKind != GIK_ENCHANT_ENABLE_ITEM ) { 
        return;
    }

    D3DXVECTOR2 dxvPos = m_pkItemImg->GetRelocatedWindowPos();
    dxvPos.x += 45.0f * m_fWindowScaleX;
    dxvPos.y += 30.0f * m_fWindowScaleY;

    if( pInvenItem->m_cGradeID == KItem::GRADE_NORMAL ) { 
        g_pkUIMgr->RenderNumber( "normal_num_enchant", dxvPos, pInvenItem->m_cEnchantLevel, false, false );
    }
    else { 
        g_pkUIMgr->RenderNumber( "epic_num_enchant", dxvPos, pInvenItem->m_cEnchantLevel, false, false );
    }
}