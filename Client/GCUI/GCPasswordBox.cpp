#include "stdafx.h"
#include "GCPasswordBox.h"
//
//


IMPLEMENT_CLASSNAME( KGCPasswordBox );
IMPLEMENT_WND_FACTORY( KGCPasswordBox );
IMPLEMENT_WND_FACTORY_NAME( KGCPasswordBox, "gc_password_box" );

KGCPasswordBox::KGCPasswordBox( void )
{
    m_pkOK      = NULL;
    m_pkCancel  = NULL;
    m_pkPasswd  = NULL;

    m_strPassword.erase();

    LINK_CONTROL( "password_box_ok_button",     m_pkOK );
    LINK_CONTROL( "password_box_cancel_button", m_pkCancel );
    LINK_CONTROL( "passwd_box_edit",            m_pkPasswd );
}

KGCPasswordBox::~KGCPasswordBox( void )
{
    // empty
}

bool KGCPasswordBox::InitDialog( IN const KDialogInfo& kInfo_ )
{
    SetUse( kInfo_.m_nMsgBoxUse);
    SetlParam(static_cast<DWORD>( kInfo_.m_lParam ));
    SetlParam2(static_cast<DWORD>( kInfo_.m_lParam2 ));

    return true;
}

void KGCPasswordBox::OnCreate( void )
{
    m_strPassword.erase();

    m_pkOK->SetSelfInputCheck( true );
    m_pkCancel->SetSelfInputCheck( true );
    m_pkPasswd->SetSelfInputCheck( false );

    m_pkOK->AddActionListener( this );
    m_pkCancel->AddActionListener( this );
    m_pkPasswd->AddActionListener( this );

    m_pkPasswd->SetText( L"" );
    m_pkPasswd->SetFocus();
}

void KGCPasswordBox::OnCreateComplete( void )
{
    SpeakToActionListener( KActionEvent( this, PBR_CREATED ) );
}

void KGCPasswordBox::OnDestroyComplete( void )
{
    SpeakToActionListener( KActionEvent( this, PBR_DESTROYED ) );
}

void KGCPasswordBox::FrameMoveInEnabledState( void )
{
    // 키를 누르는 소리
    for ( int i = 0; i < 256; i++ )
    {
        if ( g_pkInput->IsDown( i ) ) g_KDSound.Play( "71" );
    }

    if ( g_pkInput->IsDown( DIK_ESCAPE ) )
    {
        m_strPassword.erase();
        SpeakToActionListener( KActionEvent( this, PBR_CANCEL ) );
		ReleaseCapture();
        g_pkInput->IgnoreDown(DIK_ESCAPE,30);
    }
}

void KGCPasswordBox::ActionPerformed( const KActionEvent& event )
{
	//이벤트를 여러번 먹는것을 막기위한 코드임
	if( KD3DWnd::D3DWS_ENABLED != GetWndState() )
		return;

	// 엔터키 이벤트를 확인한다.
	if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN )
	{
		if ( event.m_pWnd == m_pkPasswd )
		{
			m_strPassword = m_pkPasswd->GetText();
			SpeakToActionListener( KActionEvent( this, PBR_OK ) );
			ReleaseCapture();
			return;
		}
	}  

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
		case KD3DWnd::D3DWE_BUTTON_CLICK:
        {
            if ( event.m_pWnd == m_pkOK )
            {
				m_strPassword = m_pkPasswd->GetText();
				SpeakToActionListener( KActionEvent( this, PBR_OK ) );
            }
            else if ( event.m_pWnd == m_pkCancel )
            {
                m_strPassword.erase();
                SpeakToActionListener( KActionEvent( this, PBR_CANCEL ) );
            }
            break;
        }
    }
}

const std::wstring& KGCPasswordBox::GetPassword( void ) const
{
    return m_strPassword;
}