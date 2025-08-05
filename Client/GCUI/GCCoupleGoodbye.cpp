#include "StdAfx.h"
#include "GCCoupleGoodbye.h"
#include "GCCoupleSystem.h"

IMPLEMENT_CLASSNAME( KGCCoupleGoodbye );
IMPLEMENT_WND_FACTORY( KGCCoupleGoodbye );
IMPLEMENT_WND_FACTORY_NAME( KGCCoupleGoodbye, "gc_couple_goodbye" );

KGCCoupleGoodbye::KGCCoupleGoodbye(void)
{
	m_pkDesc = NULL;
	m_pkStatic = NULL;
	m_pkMessageEdit = NULL;
	m_pkMessageStatic = NULL;

	m_pkOk = NULL;
	m_pkOk_Center = NULL;
	m_pkCancle = NULL;

	LINK_CONTROL( "static",				m_pkDesc );
	LINK_CONTROL( "message",			m_pkStatic );
	LINK_CONTROL( "static_message",		m_pkMessageStatic );
	LINK_CONTROL( "edit_message",		m_pkMessageEdit );
	LINK_CONTROL( "btn_ok",				m_pkOk );
	LINK_CONTROL( "btn_cancle",			m_pkCancle );
	LINK_CONTROL( "btn_ok_center",		m_pkOk_Center );

}

KGCCoupleGoodbye::~KGCCoupleGoodbye(void)
{
}

void KGCCoupleGoodbye::OnCreate( void )
{
	m_pkDesc->InitState( true );
	m_pkStatic->InitState( true );
	m_pkMessageEdit->InitState( true );
	m_pkOk->InitState( true, true, this );
	m_pkCancle->InitState( true, true, this );
	m_pkOk_Center->InitState( false, true, this );
	m_pkStatic->SetText( g_pkStrLoader->GetString(STR_ID_COUPLE_SYSTEM6) );
}

void KGCCoupleGoodbye::ActionPerformed( const KActionEvent& event )
{
	_GCWND_MSG_MAP( m_pkOk,	KD3DWnd::D3DWE_BUTTON_CLICK, OnOk );
	_GCWND_MSG_MAP( m_pkOk_Center,	KD3DWnd::D3DWE_BUTTON_CLICK, OnOk );
	_GCWND_MSG_MAP( m_pkCancle,	KD3DWnd::D3DWE_BUTTON_CLICK, OnCancle );
}

void KGCCoupleGoodbye::OnOk()
{
	if( m_pkMessageEdit->IsRenderOn() )
		SiKGCCoupleSystem()->SendGoodbye( m_pkMessageEdit->GetText() );
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCCoupleGoodbye::OnCancle()
{
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCCoupleGoodbye::SetUse( bool bNotify )
{
	m_pkOk->ToggleRender( !bNotify );
	m_pkCancle->ToggleRender( !bNotify );
	m_pkOk_Center->ToggleRender( bNotify );
	m_pkMessageStatic->ToggleRender( bNotify );
	m_pkMessageEdit->ToggleRender( !bNotify );

	if( bNotify )
	{
		m_pkDesc->SetText( g_pkStrLoader->GetReplacedString( STR_ID_COUPLE_SYSTEM9, "l", SiKGCCoupleSystem()->GetNickName_of_my_honey() ) );
	}
	else
	{
		m_pkMessageEdit->SetText(L"");
		m_pkDesc->SetText( g_pkStrLoader->GetReplacedString( STR_ID_COUPLE_SYSTEM8, "l", SiKGCCoupleSystem()->GetNickName_of_my_honey() ) );
	}
}

void KGCCoupleGoodbye::SetData( std::wstring strMessage )
{
	m_pkMessageStatic->SetText( strMessage );
}

bool KGCCoupleGoodbye::InitDialog( IN const KDialogInfo& kInfo_ )
{
    if( kInfo_.m_nMsgBoxUse == KGCUIScene::GC_MBOX_USE_COUPLE_GOODBYE_REQ )
        this->SetUse( false );
    else
    {
        this->SetUse( true );
        this->SetData( kInfo_.m_strText1 );
    }
    return true;
}