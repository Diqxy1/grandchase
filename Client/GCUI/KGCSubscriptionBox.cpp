#include "stdafx.h"
#include "KGCSubscriptionDlg.h"
#include "KGCSubscriptionBox.h"
//#include "../UIFX/D3DEdit.h"
//#include "../uifx/KInput.h"
#include "../KDSound.h"
#include "GCItemManager.h"


IMPLEMENT_CLASSNAME( KGCSubscriptionBox );
IMPLEMENT_WND_FACTORY( KGCSubscriptionBox );
IMPLEMENT_WND_FACTORY_NAME( KGCSubscriptionBox, "gc_subscription_box" );

KGCSubscriptionBox::KGCSubscriptionBox( void )
:m_pkOK( NULL )
,m_pkCancel( NULL )
,m_pkInputQuantity( NULL )
,m_pkItemView( NULL )
,m_pkItemInfoBox( NULL )
{

	m_iEventUID = 0;
	m_iHoldingAmount = 0;
	m_iPossibleAmount = 0;
	m_iNeedAmount = 0;
	m_iInput = 0;

	LINK_CONTROL( "subscription_confirm_button", m_pkOK );
	LINK_CONTROL( "subscription_cancel_button",	m_pkCancel );
	LINK_CONTROL( "subscription_quantity_edit", m_pkInputQuantity );

	LINK_CONTROL( "item_img_view", m_pkItemView );
	LINK_CONTROL( "item_info_box"       , m_pkItemInfoBox );
}

KGCSubscriptionBox::~KGCSubscriptionBox( void )
{
	// empty
}

void KGCSubscriptionBox::OnCreate( void )
{
	m_pkOK->InitState(true, true, this);
	m_pkCancel->InitState(true, true, this);
    m_pkCancel->SetHotKey(DIK_ESCAPE);
	m_pkInputQuantity->InitState( true, false, this );	
    m_pkInputQuantity->SetLimitText( LIMIT_INPUT_SIZE );
	m_pkInputQuantity->SetFocus();
	m_pkItemView->InitState(true);
	m_pkItemInfoBox->InitState(false, true, this);
}

void KGCSubscriptionBox::OnCreateComplete( void )
{	
}

void KGCSubscriptionBox::OnDestroyComplete( void )
{
	//SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );
}

void KGCSubscriptionBox::FrameMoveInEnabledState( void )
{
	// 키를 누르는 소리
	for ( int i = 0; i < 256; i++ )
	{
		if ( g_pkInput->IsDown( i ) ) g_KDSound.Play( "71" );
	}

	if( g_pkInput->BtnUp( KInput::MBLEFT ) )
	{
		m_pkInputQuantity->ToggleRender(true);
	}

	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );	

	if( m_pkItemView->CheckPosInWindowBound(vMousePos) )
	{
		D3DXVECTOR2 vecPos( m_pkItemView->GetCurrentWindowPos() );
		vecPos.x = ( vecPos.x + m_pkItemView->GetWidth() ) *m_fWindowScaleX;
		vecPos.y *= m_fWindowScaleY;
        
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
        g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( D3DWZ_TOPMOST );
    }
	else
	{
		//m_pkItemInfoBox->ToggleRender( false );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
    }

}

void KGCSubscriptionBox::ActionPerformed( const KActionEvent& event )
{
	//이벤트를 여러번 먹는것을 막기위한 코드임
	//if( KD3DWnd::D3DWS_ENABLED != GetWndState() )
	//	return;

	GCWND_MSG_MAP( m_pkOK,				KD3DWnd::D3DWE_BUTTON_DOWN, OnClickOKBtn );
    GCWND_MSG_MAP( m_pkCancel,			KD3DWnd::D3DWE_BUTTON_CLICK, OnExit );

	
	SpeakToActionListener( event );
}


void KGCSubscriptionBox::OnClickOKBtn()
{
	int iInputAmount = 0;
	if( IsNumberValue( m_pkInputQuantity->GetText() ) )
	{
		iInputAmount = _wtoi( m_pkInputQuantity->GetText() );
	}

	g_KDSound.Play( "31" );

	if( iInputAmount > 0 )
	{
		if( iInputAmount < m_iNeedAmount ) //필요수량보다 적은 수량을 입력했다!(서버에서 체크하지만, 클라에서도 선 체크 해줌) <-- 많은이라고 주석 달려있었지만 상황상 적은이 맞아서 주석 수정함(20110405찬선)
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetReplacedString( STR_ID_SUBSCRIPTION_ERROR_MSG1, "i", g_pkUIScene->m_pkSubscriptionDlg->GetNeedAmount()), 
				L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		}
		else
		{
            //입력한 수 개선

            if( iInputAmount > m_iHoldingAmount )
                iInputAmount = m_iHoldingAmount;

            int remainder = iInputAmount % m_iNeedAmount;
            iInputAmount -= remainder;


			KP2P::GetInstance()->Send_SubscriptReq(m_iEventUID, iInputAmount);
			StopAll(true);
		}			
	}
	else
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_SUBSCRIPTION_ERROR_MSG6), 
			L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
	}

	m_pkInputQuantity->SetText(L"");
}

void KGCSubscriptionBox::OnExit()
{
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
    g_pkUIScene->m_pkSubscriptionDlg->Update();
}

void KGCSubscriptionBox::PostChildDraw()
{
	D3DXVECTOR2 vPos = this->GetRelocatedWindowPos();

    float x1 = 0.0f, y1 = 0.0f;
    float x2 = 0.0f, y2 = 0.0f;

#if defined(NATION_TAIWAN) || defined(NATION_BRAZIL)
    x1 = 190.f, y1 = 81.f;
    x2 = 215.f, y2 = 105.f;
#elif defined(NATION_USA) || defined(NATION_PHILIPPINE) || defined( NATION_EU )
	x1 = 230.f, y1 = 80.f;
	x2 = 230.f, y2 = 104.f;
#elif defined(NATION_IDN)
    x1 = 178.f, y1 = 80.f;
    x2 = 248.f, y2 = 104.f;
#else
    x1 = 160.f, y1 = 81.f;
    x2 = 215.f, y2 = 105.f;
#endif

#if defined( _DEBUG )//테스트용
	KLuaManager luaMgr;
	GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );

	LUA_GET_VALUE_DEF( "x1", x1, 1.0f );
	LUA_GET_VALUE_DEF( "y1", y1, 1.0f );	
	LUA_GET_VALUE_DEF( "x2", x2, 1.0f );
	LUA_GET_VALUE_DEF( "y2", y2, 1.0f );
#endif

    D3DXVECTOR2 vPos1( x1 * m_fWindowScaleX, y1 * m_fWindowScaleY );
    D3DXVECTOR2 vPos2( x2 * m_fWindowScaleX, y2 * m_fWindowScaleY );

	g_pkUIMgr->RenderNumber("blue_gradation_num", vPos + vPos1, m_iHoldingAmount );
	g_pkUIMgr->RenderNumber("blue_gradation_num", vPos + vPos2, m_iPossibleAmount );
}

void KGCSubscriptionBox::SetSubscriptionInfo(int iEventUID_, int iHoldingAmount_, DWORD dwTicketID_, int iNeedAmount_, int iCurAmount_ )
{
	m_iEventUID = iEventUID_;
	m_iHoldingAmount =iHoldingAmount_;
	m_iNeedAmount = iNeedAmount_;
	m_iPossibleAmount = (int)(iHoldingAmount_/iNeedAmount_);
	if( m_iPossibleAmount > iCurAmount_ )
		m_iPossibleAmount = iCurAmount_;

	m_pkItemView->SetItemInfo( dwTicketID_ );
	GCItem* kItem = g_pItemMgr->GetItemData(dwTicketID_);	
	//m_pkItemInfoBox->SetItem(kItem);

    g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( kItem );
}

bool KGCSubscriptionBox::IsNumberValue( std::wstring strAmount_)
{
	for( int i = 0 ; i < (int)strAmount_.size() ; ++i )
	{
		if( !iswdigit(strAmount_[i] ) )
		{
			return false;
		}
	}
	return true;
}

void KGCSubscriptionBox::SetHoldingAmount( int iHoldingAmount_ )
{ 
	m_iHoldingAmount = iHoldingAmount_; 
	m_iPossibleAmount = (int)(iHoldingAmount_/m_iNeedAmount);
}

void KGCSubscriptionBox::StopAll( bool bStop_ )
{
	this->LockAll(bStop_);
	m_pkInputQuantity->ToggleRender(!bStop_);
	m_pkInputQuantity->SetFocus();
}
