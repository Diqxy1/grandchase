#include "stdafx.h"
#include "KGCInviteAgitSellBox.h"

IMPLEMENT_CLASSNAME( KGCInviteAgitSellBox );
IMPLEMENT_WND_FACTORY( KGCInviteAgitSellBox );
IMPLEMENT_WND_FACTORY_NAME( KGCInviteAgitSellBox, "gc_agit_sell_box" );

KGCInviteAgitSellBox::KGCInviteAgitSellBox( void )
: m_pkStaticDesc(NULL)
, m_pkBtnOK(NULL)
, m_pkBtnCancel(NULL)
, m_pkBtnClose(NULL)
, m_pkSellItemImg(NULL)
{
    LINK_CONTROL( "static_sell_desc",           m_pkStaticDesc );
    LINK_CONTROL( "btn_ok",                     m_pkBtnOK );
    LINK_CONTROL( "btn_cancel",                 m_pkBtnCancel );
    LINK_CONTROL( "btn_close",                  m_pkBtnClose );
    LINK_CONTROL( "img_sell_item",              m_pkSellItemImg );

    m_prItemInfo = std::make_pair(0,0);
}

KGCInviteAgitSellBox::~KGCInviteAgitSellBox( void )
{
}

void KGCInviteAgitSellBox::OnCreate( void )
{
    m_pkBtnOK->InitState(true, true, this);
    m_pkBtnCancel->InitState(true, true, this);
    m_pkBtnClose->InitState(true, true, this);

    m_pkSellItemImg->InitState(true);

    m_pkStaticDesc->InitState(true);
    m_pkStaticDesc->SetMultiLine(true);
    m_pkStaticDesc->SetTextAutoMultiline(L"");

    m_prItemInfo = std::make_pair(0,0);

    m_pkBtnClose->SetHotKey( DIK_ESCAPE );
}

void KGCInviteAgitSellBox::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnCancel,   KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
    GCWND_MSG_MAP( m_pkBtnClose,    KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
    GCWND_MSG_MAP( m_pkBtnOK,       KD3DWnd::D3DWE_BUTTON_CLICK, OnSell );
}

void KGCInviteAgitSellBox::OnClose()
{
    this->ToggleRender(false);
    m_prItemInfo = std::make_pair(0,0);
}

void KGCInviteAgitSellBox::OnSell()
{
    std::vector<GCITEMUID> vecItemUID;
    vecItemUID.push_back(m_prItemInfo.second);
    KP2P::GetInstance()->Send_SellForAgitReq(vecItemUID);
}

void KGCInviteAgitSellBox::SetSellItem( std::pair<GCITEMID, GCITEMUID> prItemInfo_ )
{
    m_prItemInfo = prItemInfo_;

    GCItem* pItem = g_pItemMgr->GetItemData( m_prItemInfo.first / 10 );
    if( NULL == pItem )
        return;

    m_pkSellItemImg->SetItemInfo(m_prItemInfo.first / 10);
    m_pkSellItemImg->ShowItem( true );
    m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetReplacedString( STR_ID_SELL_AGIT_SHOP, "l", pItem->strItemName ) );
}