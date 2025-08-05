#include "stdafx.h"
#include ".\KGCTreasureItemBox.h"

//

//
//
#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCTreasureItemBox );
IMPLEMENT_WND_FACTORY( KGCTreasureItemBox );
IMPLEMENT_WND_FACTORY_NAME( KGCTreasureItemBox, "gc_treasure_item_box" );

KGCTreasureItemBox::KGCTreasureItemBox(void)
: m_iItemID(0)
, m_pTexItem(NULL)
, m_pkCount(NULL)
{
	LINK_CONTROL( "static_title",           m_pkTitle );
	LINK_CONTROL( "static_content",         m_pkContent );
	LINK_CONTROL( "btn_ok",                 m_pkBtnOk );
	LINK_CONTROL( "button_event",           m_pkPresent );
	LINK_CONTROL( "static_count",			m_pkCount );

	m_iItemID       = 0;
	m_pTexItem      = NULL;
}

KGCTreasureItemBox::~KGCTreasureItemBox(void)
{

}

void KGCTreasureItemBox::FrameMoveInEnabledState()
{
	if( g_pkInput->IsUp(DIK_ESCAPE) )
	{
		OnConfirm();
	}
}

void KGCTreasureItemBox::OnCreate( void )
{
	m_pkTitle->InitState( true, false, this );
    m_pkTitle->SetMultiLine( true );
    m_pkTitle->SetAlign( DT_LEFT );
    m_pkTitle->SetWordWrap( true );

	m_pkContent->InitState( true, false, this );
	m_pkPresent->InitState( true, false, this );

	m_pkBtnOk->InitState( true, true, this );
	m_pkBtnOk->SetHotKey( DIK_ESCAPE );
    m_pkBtnOk->SetHotKey( DIK_RETURN );

	if(m_pkCount)
		m_pkCount->InitState( false, true, this);
}

void KGCTreasureItemBox::SetTitle( std::wstring strTitle_ )
{
	m_pkTitle->SetTextAutoMultiline( strTitle_ );
}

void KGCTreasureItemBox::SetContent( std::wstring strMsg_ )
{
	m_pkContent->SetText( strMsg_ );
}

void KGCTreasureItemBox::SetContentAutoMultiLine( std::wstring strMsg_ )
{
	m_pkContent->SetTextAutoMultiline( strMsg_ );
}

void KGCTreasureItemBox::SetItem(int iItemID, int nEnchantLevel )
{
	SAFE_RELEASE(m_pTexItem);
	m_iItemID   = 0;
	m_pkContent->SetText( L"" );
	m_pkContent->SetTextAutoMultiline( L"" );

    if( m_iItemID != -1 )
    {
        m_pTexItem  = g_pItemMgr->CreateItemTexture( iItemID, 0 );	
        m_iItemID   = iItemID;

		GCItem *pkItem = g_pItemMgr->GetItemData( iItemID );
        if(pkItem == NULL){ 
            ASSERT(!"누가 없는 아이템을 보여 줄려고 했다.!!" );
            return;
        }
		m_pkTitle->SetFontColor( g_pItemMgr->GetItemStringColor( pkItem->eItemKind ) );
		

		std::wstring strName;
		if ( nEnchantLevel > 0 ) 
		{
			g_pkStrLoader->GetReplacedString( STR_ID_ENCHANT_DISPLAY, "i", nEnchantLevel );
		}

		strName += pkItem->strItemName;

		m_pkTitle->SetTextAutoMultiline( strName );
		
		m_pkContent->SetTextAutoMultiline( pkItem->strItemDesc.c_str() );
	}
}

void KGCTreasureItemBox::ActionPerformed( const KActionEvent& event )
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

//확인 버튼을 누를 시에 서버에 보낼 메시지 
void KGCTreasureItemBox::OnReq(int r_)
{
	m_bDoReq = true;
	m_Req = r_;
}


void KGCTreasureItemBox::OnConfirm()
{
	if(NULL != KP2P::GetInstance()->m_pkUserProxy  && m_bDoReq)
	{
		switch(m_Req)
		{
		case REQ_MSG_MISSION_DATE_CHANGE_REQ :
			KP2P::GetInstance()->Send_DateChangeReq();
			break;
		default:
			break;
		}
		m_bDoReq = false;
	}
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCTreasureItemBox::OnDestroy()
{
	SAFE_RELEASE( m_pTexItem );
}

void KGCTreasureItemBox::OnDestroyComplete( void )
{
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );
}

void KGCTreasureItemBox::PostChildDraw( void )
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

void KGCTreasureItemBox::SetCount( std::wstring strTitle_ )
{
	if(m_pkCount)
	{
		m_pkCount->SetText( strTitle_ );
		m_pkCount->ToggleRender( true );
	}
}

void KGCTreasureItemBox::ShowCount(bool bRender)
{
	if(m_pkCount)
		m_pkCount->ToggleRender( bRender );
}

bool KGCTreasureItemBox::InitDialog( IN const KDialogInfo& kInfo_ )
{
    if((int)kInfo_.m_lParam != NULL)
        this->SetItem((int)kInfo_.m_lParam);
    if((int)kInfo_.m_lParam2 > REQ_MSG_NONE)
        this->OnReq(static_cast<int>(kInfo_.m_lParam2));

    this->SetWindowZOrder( D3DWZ_TOPMOST );
    return true;
}