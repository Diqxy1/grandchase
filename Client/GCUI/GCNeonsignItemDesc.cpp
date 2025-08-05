#include "stdafx.h"
#include "GCNeonsignItemDesc.h"
//
//


//
#include "MyD3D.h"

IMPLEMENT_CLASSNAME( KGCNeonsignItemDesc );
IMPLEMENT_WND_FACTORY( KGCNeonsignItemDesc );
IMPLEMENT_WND_FACTORY_NAME( KGCNeonsignItemDesc, "gc_neonsign_item_desc" );

KGCNeonsignItemDesc::KGCNeonsignItemDesc(void)
: m_pkHelp(NULL)
, m_iItemID(-1)
, m_pkItemDesc(NULL)
, m_pItemTexture(NULL)
, m_pkItemBack(NULL)
{
    LINK_CONTROL( "static_help",        m_pkHelp );
    LINK_CONTROL( "static_item_desc",   m_pkItemDesc );
    LINK_CONTROL( "item_back",          m_pkItemBack );
}

KGCNeonsignItemDesc::~KGCNeonsignItemDesc(void)
{
    SAFE_RELEASE( m_pItemTexture );
}

void KGCNeonsignItemDesc::PostChildDraw()
{
    if( IsRenderOn() && m_iItemID > 0 )
    {
        D3DXVECTOR2 vPos( m_pkItemBack->GetRelocatedWindowPos() );
		g_pItemMgr->Draw_ShopImage( vPos + D3DXVECTOR2( 3.0f* GC_SCREEN_DIV_WIDTH * m_fWindowScaleX, 0.0f* GC_SCREEN_DIV_WIDTH * m_fWindowScaleY), 73.0f* GC_SCREEN_DIV_WIDTH * m_fWindowScaleX, 73.0f* GC_SCREEN_DIV_WIDTH * m_fWindowScaleY, m_pItemTexture, false );
    }
}

void KGCNeonsignItemDesc::SetItem( int nItemID )
{
    if( m_iItemID == nItemID )
    {
        return;
    }

    m_iItemID = nItemID;

    SAFE_RELEASE( m_pItemTexture );
    if( m_iItemID == -1 )
    {
        m_pkItemDesc->SetText( L"" );
        return;
    }

    GCItem* pItem = NULL;
    pItem = g_pItemMgr->GetItemData( m_iItemID / 10 );
    m_pkItemDesc->SetTextAutoMultiline( pItem->strItemDesc );
    m_pItemTexture = g_pItemMgr->CreateItemTexture( m_iItemID / 10, 0 );
}

void KGCNeonsignItemDesc::OnCreate()
{
    m_pkHelp->InitState( true );
    m_pkHelp->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_NEONSIGN_ITEM_CHOICE_HELP ) );
    m_pkItemDesc->InitState( true );
}
