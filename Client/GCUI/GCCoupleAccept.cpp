#include "StdAfx.h"
#include "KGC2DObject.h"
#include "GCCheckBox.h"
#include "GCCoupleAccept.h"
#include "gcsizingthreebox.h"
#include "GCItemInfoBox.h"
#include "GCItemManager.h"
#include "GCCoupleSystem.h"

IMPLEMENT_CLASSNAME( KGCCoupleAccept );
IMPLEMENT_WND_FACTORY( KGCCoupleAccept );
IMPLEMENT_WND_FACTORY_NAME( KGCCoupleAccept, "gc_couple_accept" );

KGCCoupleAccept::KGCCoupleAccept(void)
: m_dwProposeUserUID( 0 )
, m_dwRingID( 0 )
, m_iDenyCounter( 55 * 20 )
, m_bFirst( true )
, m_eCoupleState( GCCA_RINGCHANGE )
{
	m_pkRingSlot = NULL;
	m_pkCoupleString = NULL;
	m_pkMessage = NULL;
	m_pkProposeMessage = NULL;
	m_pkAgree = NULL;
	m_pkDisagree = NULL;
	m_pkDayGuage = NULL;
	m_pkDisagreeToday = NULL;
	m_pkDisagreeHuman = NULL;
	m_pkDisagreeTodayStatic = NULL;
	m_pkDisagreeHumanStatic = NULL;

	LINK_CONTROL( "ring_slot", m_pkRingSlot );
	LINK_CONTROL( "static", m_pkCoupleString );
	LINK_CONTROL( "static_message", m_pkMessage );
	LINK_CONTROL( "static_message_view", m_pkProposeMessage );
	LINK_CONTROL( "btn_ok", m_pkAgree );
	LINK_CONTROL( "btn_cancle", m_pkDisagree );
	LINK_CONTROL( "bg_guage", m_pkDayGuage );
	LINK_CONTROL( "check_disagree_today", m_pkDisagreeToday );
	LINK_CONTROL( "check_disagree_human", m_pkDisagreeHuman );
	LINK_CONTROL( "static_disagree_today", m_pkDisagreeTodayStatic );
	LINK_CONTROL( "static_disagree_human", m_pkDisagreeHumanStatic );
	LINK_CONTROL( "title_accept", m_pkTitle[GCCA_ACCEPT] );
	LINK_CONTROL( "title_ringchange", m_pkTitle[GCCA_RINGCHANGE] );
}

KGCCoupleAccept::~KGCCoupleAccept(void)
{
}

void KGCCoupleAccept::OnCreate( void )
{
	m_pkRingSlot->InitState( true );
	m_pkCoupleString->InitState( true );
	m_pkMessage->InitState( true );
	m_pkMessage->SetText( g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM6 ) );
	m_pkProposeMessage->InitState( true );
	m_pkAgree->InitState( true, true, this );
	m_pkDisagree->InitState( true, true, this );
	m_pkDayGuage->InitState( true, true );
	m_pkDisagreeToday->InitState( true, true, this );
	m_pkDisagreeHuman->InitState( true, true, this );
	m_pkDisagreeTodayStatic->InitState( true );
	m_pkDisagreeHumanStatic->InitState( true );
	m_pkDisagreeTodayStatic->SetText( g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM25 ) );
	m_pkDisagreeHumanStatic->SetText( g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM26 ) );

#if defined( _DEBUG )
	KLuaManager luaMgr;
	GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );

	float x = 0.0f, y = 1.0f;
	LUA_GET_VALUE_DEF( "Scalex", x, 1.0f );
	LUA_GET_VALUE_DEF( "Scaley", y, 1.0f );

	float px, py;
	LUA_GET_VALUE_DEF( "Px", px, 1.0f );
	LUA_GET_VALUE_DEF( "Py", py, 1.0f );
#endif
	m_kRingImage.SetPosition( ( m_pkRingSlot->GetFixedWindowPos().x + 3.0f ) / (400.0f * GC_SCREEN_DIV_WIDTH), -( m_pkRingSlot->GetFixedWindowPos().y +36.0f ) / (300.0f * GC_SCREEN_DIV_WIDTH) * 0.75f + 1.1f );
	m_kRingImage.SetScale( 0.23f, 0.23f );
	m_kRingImage.CameraMatrixOn();
	m_iDenyCounter= 55 * 20;

	if( m_bFirst )
	{
		m_iGuageFullWidth = m_pkDayGuage->GetWidth();
		m_bFirst = false;
	}
	m_iDenyCounter = 1100;
}

void KGCCoupleAccept::ActionPerformed( const KActionEvent& event )
{
	_GCWND_MSG_MAP( m_pkAgree,		KD3DWnd::D3DWE_BUTTON_CLICK, OnClickAgree );
	_GCWND_MSG_MAP( m_pkDisagree,	KD3DWnd::D3DWE_BUTTON_CLICK, OnClickDisagree );
	GCWND_MSG_MAP( m_pkDisagreeToday, KD3DWnd::D3DWE_BUTTON_CLICK, m_pkDisagreeToday->ToggleCheck );
	GCWND_MSG_MAP( m_pkDisagreeHuman, KD3DWnd::D3DWE_BUTTON_CLICK, m_pkDisagreeHuman->ToggleCheck );
}

void KGCCoupleAccept::OnClickAgree()
{
	SendAccept( true );
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCCoupleAccept::OnClickDisagree()
{
	SendAccept( false );
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCCoupleAccept::SendAccept( bool bAccept )
{
	switch( m_eCoupleState )
	{
	case GCCA_ACCEPT:
		SiKGCCoupleSystem()->SendAccept( bAccept );
		break;
	case GCCA_RINGCHANGE:
		SiKGCCoupleSystem()->SendRingAccept( bAccept );
		break;
	}
}

void KGCCoupleAccept::PostChildDraw( void )
{
	m_kRingImage.Render();
}

void KGCCoupleAccept::SetData( std::wstring strRequesterNick, std::wstring strMessage, DWORD dwRingID, DWORD dwUserUID )
{
	switch( m_eCoupleState )
	{
	case GCCA_ACCEPT:
		m_pkCoupleString->SetText( g_pkStrLoader->GetReplacedString( STR_ID_COUPLE_SYSTEM7, "l", strRequesterNick ));
		m_pkProposeMessage->SetTextAutoMultiline( strMessage );
		m_dwRingID = dwRingID;
		m_dwProposeUserUID = dwUserUID;
		break;
	case GCCA_RINGCHANGE:
		m_pkCoupleString->SetText( g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM28 ) );
		m_pkProposeMessage->SetTextAutoMultiline( strMessage );
		m_dwRingID = dwRingID;
		m_dwProposeUserUID = dwUserUID;
		break;
	}

	m_kRingImage.RemoveAllTexture();
	char strItemID[MAX_PATH];

	int iItemID = g_pItemMgr->GetSameImageIndex(dwRingID);

	sprintf( strItemID, "Sbta%03d.dds", iItemID );
	m_kRingImage.AddTexture( strItemID );
}

void KGCCoupleAccept::FrameMoveInEnabledState( void )
{
	DrawItemInfo();

	if( m_iDenyCounter == 0 )
	{
		SendAccept( false );
		SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
	}
	else if( m_iDenyCounter < 0 )
	{
		SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
	}
	else
	{
#if !defined( _DEBUG )
		--m_iDenyCounter;
#endif
		m_pkDayGuage->SetWidth( (DWORD)(m_iGuageFullWidth * ( (float)m_iDenyCounter / 1100.0f )) );
	}
}

void KGCCoupleAccept::DrawItemInfo()
{
    ASSERT( g_pkInput != NULL );
    D3DXVECTOR2 vMousePos( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

    if( m_pkRingSlot->CheckPosInWindowBound( vMousePos ) == false )
    {
		g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
        return;
    }

    GCItem* pItem = g_pItemMgr->GetItemData( m_dwRingID );
    ASSERT( pItem != NULL );
	g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
	g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
	g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
	g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( m_pkRingSlot->GetCurrentWindowPos()+ D3DXVECTOR2( (float)m_pkRingSlot->GetWidth(), 0.0f ));
	g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(D3DWZ_TOPMOST);
}

void KGCCoupleAccept::OnDestroy( void )
{
	if( m_pkDisagreeToday->GetCheck() )
		SiKGCCoupleSystem()->SendDenyToday();

	if( m_pkDisagreeHuman->GetCheck() )
		SiKGCCoupleSystem()->SendDenyThisUser( m_dwProposeUserUID );
}

void KGCCoupleAccept::SetUse( GCCoupleAcceptState eState )
{
	m_eCoupleState = eState;
	switch( m_eCoupleState )
	{
	case GCCA_ACCEPT:
		m_pkDisagreeToday->InitState( true, true, this );
		m_pkDisagreeHuman->InitState( true, true, this );
		m_pkDisagreeToday->SetCheck( false );
		m_pkDisagreeHuman->SetCheck( false );
		m_pkDisagreeTodayStatic->SetFontColor( D3DCOLOR_ARGB(255,255,255,255) );
		m_pkDisagreeHumanStatic->SetFontColor( D3DCOLOR_ARGB(255,255,255,255) );
		break;
	case GCCA_RINGCHANGE:
		m_pkDisagreeToday->InitState( true );
		m_pkDisagreeHuman->InitState( true );
		m_pkDisagreeTodayStatic->SetFontColor( D3DCOLOR_ARGB(100,100,100,255) );
		m_pkDisagreeHumanStatic->SetFontColor( D3DCOLOR_ARGB(100,100,100,255) );
		break;
	}
	m_pkTitle[GCCA_ACCEPT]->ToggleRender( m_eCoupleState == GCCA_ACCEPT );
	m_pkTitle[GCCA_RINGCHANGE]->ToggleRender( m_eCoupleState == GCCA_RINGCHANGE );
}

bool KGCCoupleAccept::InitDialog( IN const KDialogInfo& kInfo_ )
{
    switch( kInfo_.m_nMsgBoxUse )
    {
    case KGCUIScene::GC_MBOX_USE_NORMAL:
        this->SetUse( KGCCoupleAccept::GCCA_ACCEPT );
        break;
    case KGCUIScene::GC_MBOX_USE_COUPLE_RING_CHANGE:
        this->SetUse( KGCCoupleAccept::GCCA_RINGCHANGE );
        break;
    }
    this->SetData( kInfo_.m_strText1, kInfo_.m_strText2, static_cast<GCITEMID>(kInfo_.m_lParam), static_cast<DWORD>(kInfo_.m_lParam2) );
    return true;
}