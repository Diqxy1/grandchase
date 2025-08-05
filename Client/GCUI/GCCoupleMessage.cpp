#include "StdAfx.h"
#include "GCCoupleMessage.h"

IMPLEMENT_CLASSNAME( KGCCoupleMessage );
IMPLEMENT_WND_FACTORY( KGCCoupleMessage );
IMPLEMENT_WND_FACTORY_NAME( KGCCoupleMessage, "gc_couple_message" );

KGCCoupleMessage::KGCCoupleMessage(void)
{
	m_pkMessage = NULL;
	m_pkOk_Center = NULL;
	m_pkOk = NULL;
	m_pkCancle = NULL;

	LINK_CONTROL( "static",		m_pkMessage );
	LINK_CONTROL( "btn_ok_center", m_pkOk_Center );
	LINK_CONTROL( "btn_ok",		m_pkOk );
	LINK_CONTROL( "btn_cancle",	m_pkCancle );
}

KGCCoupleMessage::~KGCCoupleMessage(void)
{
}

void KGCCoupleMessage::OnCreate( void )
{
	m_pkMessage->InitState( true );
	m_pkMessage->SetAlign( DT_CENTER );
	m_pkOk_Center->InitState( true, true, this );
	m_pkOk->InitState( true, true, this );
	m_pkCancle->InitState( true, true, this );
}

void KGCCoupleMessage::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkOk, KD3DWnd::D3DWE_BUTTON_CLICK, OnOk );
	GCWND_MSG_MAP( m_pkOk_Center, KD3DWnd::D3DWE_BUTTON_CLICK, OnOk );
	GCWND_MSG_MAP( m_pkCancle, KD3DWnd::D3DWE_BUTTON_CLICK, OnCancle );
}

void KGCCoupleMessage::OnOk()
{
	SpeakToActionListener( KActionEvent( this, CMBR_OK ) );
}

void KGCCoupleMessage::OnCancle()
{
	SpeakToActionListener( KActionEvent( this, CMBR_CANCLE) );
}

void KGCCoupleMessage::SetMessage( std::wstring strMsg )
{
	m_pkMessage->SetTextAutoMultiline( strMsg );
}

void KGCCoupleMessage::SetButton( ECoupleMessageBoxUse eUse )
{
	switch( eUse )
	{
	case CMBR_USE_OK_ONLY:
		m_pkOk->ToggleRender( false );
		m_pkCancle->ToggleRender( false );
		m_pkOk_Center->ToggleRender( true );
		break;
	case CMBR_USE_OK_CANCLE:
		m_pkOk->ToggleRender( true );
		m_pkCancle->ToggleRender( true );
		m_pkOk_Center->ToggleRender( false );
		break;
	}
}

void KGCCoupleMessage::SetUse( const int eUse )
{
	m_eUse = (KGCUIScene::EGCMsgBoxUse)eUse;
}

bool KGCCoupleMessage::InitDialog( IN const KDialogInfo& kInfo_ )
{
    this->SetMessage( kInfo_.m_strText1 );
    switch( kInfo_.m_nMsgBoxUse ) {
        case KGCUIScene::GC_MBOX_USE_COUPLE_ITEM_CONFIRM:
            this->SetButton( KGCCoupleMessage::CMBR_USE_OK_CANCLE );
            break;
        default:
            this->SetButton( KGCCoupleMessage::CMBR_USE_OK_ONLY );
            break;
    }
    this->SetUse( kInfo_.m_nMsgBoxUse );
    return true;
}