#include "stdafx.h"
#include ".\GCGiftItemBox.h"
#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCGiftItemBox );
IMPLEMENT_WND_FACTORY( KGCGiftItemBox );
IMPLEMENT_WND_FACTORY_NAME( KGCGiftItemBox, "gc_gift_item_box" );

KGCGiftItemBox::KGCGiftItemBox(void)
: m_iItemID(0)
, m_pTexItem(NULL)
, m_pkCount(NULL)
, m_pkComment(NULL)
{
    LINK_CONTROL( "static_title",           m_pkTitle );
    LINK_CONTROL( "static_content",         m_pkContent );
    LINK_CONTROL( "btn_ok",                 m_pkBtnOk );
    LINK_CONTROL( "button_event",           m_pkPresent );
	LINK_CONTROL( "static_count",			m_pkCount );
    LINK_CONTROL( "static_comment",			m_pkComment );

    m_iItemID       = 0;
    m_pTexItem      = NULL;
}

KGCGiftItemBox::~KGCGiftItemBox(void)
{

}

void KGCGiftItemBox::FrameMoveInEnabledState()
{
    if( g_pkInput->IsUp(DIK_ESCAPE) )
    {
        OnConfirm();
    }
}

void KGCGiftItemBox::OnCreate( void )
{
    m_pkTitle->InitState( true, false, this );
    m_pkContent->InitState( true, false, this );
    m_pkPresent->InitState( true, false, this );
    
    m_pkBtnOk->InitState( true, true, this );
    m_pkBtnOk->SetHotKey( DIK_RETURN );

	if(m_pkCount)
		m_pkCount->InitState( false, true, this);

	m_pkComment->InitState( true, false, this );
}

void KGCGiftItemBox::SetTitle( std::wstring strTitle_ )
{
    m_pkTitle->SetText( strTitle_ );
}

void KGCGiftItemBox::SetContent( std::wstring strMsg_ )
{
    m_pkContent->SetText( strMsg_ );
}

void KGCGiftItemBox::SetContentAutoMultiLine( std::wstring strMsg_ )
{
	m_pkContent->SetTextAutoMultiline( strMsg_ );
}

void KGCGiftItemBox::SetMsgbox(int iItemID, std::wstring strMsg_, std::wstring strMsg2_)
{
    SAFE_RELEASE(m_pTexItem);
    m_iItemID   = 0;
    m_pkContent->SetText( L"" );
    m_pkContent->SetTextAutoMultiline( L"" );

    if( m_iItemID != -1 )
    {
		std::wostringstream strTemp;

        m_pTexItem  = g_pItemMgr->CreateItemTexture( iItemID, -1, 0 );	
        m_iItemID   = iItemID;

        GCItem *pkItem = g_pItemMgr->GetItemData( iItemID );
        m_pkTitle->SetFontColor( g_pItemMgr->GetItemStringColor( pkItem->eItemKind ) );
        m_pkTitle->SetText( pkItem->strItemName.c_str() );
		strTemp << g_pkStrLoader->GetReplacedString( STR_ID_GIFT_RECEIVE, "l", strMsg_ );
		
		m_pkContent->SetTextAutoMultiline( strTemp.str() );
		m_pkComment->SetTextAutoMultiline( strMsg2_ );
    }
}

void KGCGiftItemBox::ActionPerformed( const KActionEvent& event )
{
    switch ( event.m_dwCode )
    {
    case KD3DWnd::D3DWE_CURSOR_ENTER:
        {
            g_KDSound.Play( "30" );
            break;
        }
    case KD3DWnd::D3DWE_BUTTON_DOWN:
        {
            g_KDSound.Play( "31" );
            break;
        }
    }

    GCWND_MSG_MAP( m_pkBtnOk,               KD3DWnd::D3DWE_BUTTON_CLICK,    OnConfirm );
}

void KGCGiftItemBox::OnConfirm()
{
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCGiftItemBox::OnDestroy()
{
    SAFE_RELEASE( m_pTexItem );
}

void KGCGiftItemBox::OnDestroyComplete( void )
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );
}

void KGCGiftItemBox::PostChildDraw( void )
{
    if ( m_pTexItem && m_iItemID != -1 )
    {
        D3DXVECTOR2 vPos( GetRelocatedWindowPos() );

        vPos.x += 82.0f * m_fWindowScaleX;
        vPos.y += 89.0f * m_fWindowScaleY;

        g_pGCDeviceManager2->PushState();
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
        {
			g_pGCDeviceManager2->DrawInScreen(m_pTexItem,
			vPos.x
                , vPos.y
                , vPos.x + 95.0f * m_fWindowScaleX
                , vPos.y + 95.0f * m_fWindowScaleY
			, 0.0f, 0.0f, 1.0f, 1.0f, false, 10.0f,0xffffffff);
        }
        g_pGCDeviceManager2->PopState();
    }
}

bool KGCGiftItemBox::CheckCondition( IN const KDialogInfo& kInfo_ )
{
    if( kInfo_.m_lParam != 0 )
    {
        this->SetMsgbox( static_cast<int>(kInfo_.m_lParam), kInfo_.m_strText1, kInfo_.m_strText2 );
    }
    return true;
}