#include "stdafx.h"
#include "KGCGuildPublicMbox.h"

IMPLEMENT_CLASSNAME( KGCGuildPublicMbox );
IMPLEMENT_WND_FACTORY( KGCGuildPublicMbox );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildPublicMbox, "gc_guild_public_mbox" );

KGCGuildPublicMbox::KGCGuildPublicMbox(void)
: m_pkConfirmBtn( 0 )
, m_pkCancelBtn( 0 )
, m_pkGuildMboxTitle( 0 )
, m_pkGuildMboxDesc( 0 )
{
	LINK_CONTROL( "confirm_btn",		m_pkConfirmBtn );
	LINK_CONTROL( "cancel_btn",			m_pkCancelBtn );
	LINK_CONTROL( "confirm_only_btn",	m_pkConfirmOnlyBtn );

	LINK_CONTROL( "title_static",		m_pkGuildMboxTitle );
	LINK_CONTROL( "desc_static",		m_pkGuildMboxDesc );

	m_mbox_type	=	MBR_CONFIRM_ONLY;
}

KGCGuildPublicMbox::~KGCGuildPublicMbox(void)
{
}

void KGCGuildPublicMbox::OnCreate()
{
	m_pkConfirmBtn->InitState( false, true, this );
	m_pkCancelBtn->InitState( false, true, this );
	m_pkConfirmOnlyBtn->InitState( false, true, this );

	m_pkGuildMboxTitle->InitState( true, true, this );
	m_pkGuildMboxTitle->SetAlign(DT_CENTER);
	m_pkGuildMboxDesc->InitState( true, true, this );
	m_pkGuildMboxDesc->SetMultiLine(true);
	m_pkGuildMboxDesc->SetLineSpace(1.2f);

	SetType( m_mbox_type );

	SetWindowZOrder(D3DWZ_TOPMOST);
	Activate(true);
}

void KGCGuildPublicMbox::ActionPerformed( const KActionEvent& event )
{
	//	버튼 클릭 이벤트
	if( event.m_pWnd ==	m_pkConfirmBtn && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
		SpeakToActionListener( KActionEvent( this, MBR_YES ) );

	if( event.m_pWnd ==	m_pkCancelBtn && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
		SpeakToActionListener( KActionEvent( this, MBR_NO ) );

	if( event.m_pWnd == m_pkConfirmOnlyBtn && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
		SpeakToActionListener( KActionEvent( this, MBR_OK ) );
}

void KGCGuildPublicMbox::OnCreateComplete()
{
}

void KGCGuildPublicMbox::SetType( EGuildMboxType eType )
{
	m_mbox_type	=	eType;
	switch ( m_mbox_type )
	{
	case MBR_CONFIRM_CANCEL:
		m_pkConfirmBtn->ToggleRender( true );
		m_pkCancelBtn->ToggleRender( true );

		m_pkConfirmOnlyBtn->ToggleRender( false );
		break;

	case MBR_CONFIRM_ONLY:
		m_pkConfirmBtn->ToggleRender( false );
		m_pkCancelBtn->ToggleRender( false );

		m_pkConfirmOnlyBtn->ToggleRender( true );
		break;

	default:
		assert( !"인자를 EGuildMboxType으로 받는데 여기로 들어오다니?" );
		m_pkConfirmBtn->ToggleRender( false );
		m_pkCancelBtn->ToggleRender( false );

		m_pkConfirmOnlyBtn->ToggleRender( true );
		break;
	}
}

void KGCGuildPublicMbox::SetText( const std::wstring& str1, const std::wstring& str2 )
{
	m_pkGuildMboxTitle->SetText( str1 );
	m_pkGuildMboxDesc->SetTextAutoMultiline( str2 );
}

bool KGCGuildPublicMbox::CheckCondition( IN const KDialogInfo& kInfo_ )
{
    this->SetType( KGCGuildPublicMbox::EGuildMboxType(kInfo_.m_lParam) );
    this->SetText( kInfo_.m_strText1, kInfo_.m_strText2 );	
    this->SetUse(kInfo_.m_nMsgBoxUse);
    return true;
}