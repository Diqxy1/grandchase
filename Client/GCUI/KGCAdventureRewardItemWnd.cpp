#include "stdafx.h"
#include "KGCAdventureRewardItemWnd.h"

IMPLEMENT_CLASSNAME( KGCAdventureRewardItemWnd );
IMPLEMENT_WND_FACTORY( KGCAdventureRewardItemWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCAdventureRewardItemWnd, "gc_adventure_item_wnd" );

KGCAdventureRewardItemWnd::KGCAdventureRewardItemWnd( void )
: m_pkStaticItemNum(NULL)
, m_pkItemImage(NULL)
, m_bMouseBound(NULL)
, m_dwItemID(NULL)
, m_pkStaticBack(NULL)
{
    LINK_CONTROL("item_image",m_pkItemImage);
    LINK_CONTROL("static_item_num",m_pkStaticItemNum);
    LINK_CONTROL("static_background", m_pkStaticBack );
}

KGCAdventureRewardItemWnd::~KGCAdventureRewardItemWnd( void )
{
}

void KGCAdventureRewardItemWnd::OnCreate( void )
{
    m_pkStaticItemNum->SetText(L"");
    m_pkStaticItemNum->InitState( true, true, this );
    m_pkStaticBack->InitState( true, true, this);
    m_pkItemImage->InitState( true, true, this);
}

void KGCAdventureRewardItemWnd::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkItemImage, KD3DWnd::D3DWE_CURSOR_ENTER, OnMouseEnter );
    GCWND_MSG_MAP( m_pkItemImage, KD3DWnd::D3DWE_CURSOR_LEAVE, OnMouseLeave );
}

void KGCAdventureRewardItemWnd::FrameMoveInEnabledState( void )
{
    if ( SiGCAdventureManager()->GetShowItemInfo() )
        ShowItemInfo();
}

void KGCAdventureRewardItemWnd::ShowItemInfo( void )
{
    m_pkItemImage->SetToolTip( L"");
    if( m_bMouseBound ) {
        GCItem* pItem = g_pItemMgr->GetItemData( m_dwItemID / 10 );

        if( NULL == pItem ) { 
            return;
        }

        if( pItem->dwGoodsID == KItem::INVALID_ITEM ) { 
            return;
        }
        
        //Information Box가 아닌 툴팁으로 전리품 이름만 띄우자.
        m_pkItemImage->SetToolTip(g_pItemMgr->GetItemName( m_dwItemID / 10) );
    }
}

void KGCAdventureRewardItemWnd::OnMouseEnter( void )
{
    m_bMouseBound = true;
}

void KGCAdventureRewardItemWnd::OnMouseLeave( void )
{
    m_bMouseBound = false;
}

//전리품 정보 세팅
void KGCAdventureRewardItemWnd::SetRewardItemBoxInfo( GCITEMID dwItemID )
{
    m_dwItemID = dwItemID;

    m_pkItemImage->SetItemInfo( dwItemID / 10 );
    m_pkItemImage->ShowItem ( false );
}

//대륙의 전리품 획득 상태를 보여준다.
void KGCAdventureRewardItemWnd::SetContinentObtainCnt( DWORD dwObtainCnt, DWORD dwMaxCnt )
{
    int iObtainCnt = static_cast<int>(dwObtainCnt);
    int iMaxCnt = static_cast<int>(dwMaxCnt);
    
    m_pkStaticItemNum->SetText(L"");
    m_pkStaticItemNum->SetAlign( DT_CENTER );
    m_pkStaticItemNum->SetText( g_pkStrLoader->GetReplacedString( STR_ID_MONSTER_CARD_MIX_PAGE_NUM, "ii", iObtainCnt, iMaxCnt ));
}

void KGCAdventureRewardItemWnd::IsRender( bool bRender )
{
    m_pkStaticItemNum->ToggleRender( bRender);
    m_pkStaticBack->ToggleRender( bRender);
    m_pkItemImage->ToggleRender( bRender );
    m_pkItemImage->ShowItem(bRender);
}
