#include "stdafx.h"

#include "KGCUserMigrationDlg.h"


IMPLEMENT_CLASSNAME( KGCUserMigrationDlg );
IMPLEMENT_WND_FACTORY( KGCUserMigrationDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCUserMigrationDlg, "gc_user_migration_dlg" );

KGCUserMigrationDlg::KGCUserMigrationDlg( void )
{
    m_pkBtnOk = NULL;
    m_pkBtnCancel = NULL;

    m_pkStaticTitle = NULL;
    m_pkStaticLoginID = NULL;
    m_pkStaticEmail = NULL;
    m_pkStaticNickName = NULL;
    m_pkStaticBirthDay = NULL;

    m_pkEditLoginID = NULL;
    m_pkEditEmail = NULL;
    m_pkEditNickname = NULL;
    m_pkEditDay = NULL;
    m_pkEditMonth = NULL;
    m_pkEditYear = NULL;


	LINK_CONTROL( "btn_ok", m_pkBtnOk );
	LINK_CONTROL( "btn_cancel",	m_pkBtnCancel );

	LINK_CONTROL( "static_title", m_pkStaticTitle );
    LINK_CONTROL( "title_loginid", m_pkStaticLoginID );
    LINK_CONTROL( "title_email", m_pkStaticEmail );
    LINK_CONTROL( "title_nickname", m_pkStaticNickName );
    LINK_CONTROL( "title_birthday", m_pkStaticBirthDay );

    LINK_CONTROL( "edit_loginid", m_pkEditLoginID );
    LINK_CONTROL( "edit_email", m_pkEditEmail );
    LINK_CONTROL( "edit_nickname", m_pkEditNickname );
    LINK_CONTROL( "edit_birthday_day", m_pkEditDay );
    LINK_CONTROL( "edit_birthday_month", m_pkEditMonth );
    LINK_CONTROL( "edit_birthday_year", m_pkEditYear );
}

KGCUserMigrationDlg::~KGCUserMigrationDlg( void )
{
}

void KGCUserMigrationDlg::OnCreate( void )
{	
    m_pkBtnOk->InitState(true, true, this);
    m_pkBtnCancel->InitState(true, true, this);;

    m_pkStaticTitle->InitState(true);
    m_pkStaticTitle->SetAlign(DT_CENTER);
    m_pkStaticTitle->SetText(g_pkStrLoader->GetString(STR_ID_USER_MIGRATION_TITLE));
    m_pkStaticLoginID->InitState(true);
    m_pkStaticLoginID->SetAlign(DT_RIGHT);
    m_pkStaticLoginID->SetText(g_pkStrLoader->GetString(STR_ID_USER_MIGRATION_LOGIN_ID));
    m_pkStaticLoginID->SetFontColor( D3DCOLOR_ARGB(255,225,222,0) );
    m_pkStaticEmail->InitState(true);
    m_pkStaticEmail->SetAlign(DT_RIGHT);
    m_pkStaticEmail->SetFontColor( D3DCOLOR_ARGB(255,225,222,0) );
    m_pkStaticEmail->SetText(g_pkStrLoader->GetString(STR_ID_USER_MIGRATION_EMAIL));
    m_pkStaticNickName->InitState(true);
    m_pkStaticNickName->SetAlign(DT_RIGHT);
    m_pkStaticNickName->SetFontColor( D3DCOLOR_ARGB(255,225,222,0) );
    m_pkStaticNickName->SetText(g_pkStrLoader->GetString(STR_ID_USER_MIGRATION_NICKNAME));
    m_pkStaticBirthDay->InitState(true);
    m_pkStaticBirthDay->SetAlign(DT_RIGHT);
    m_pkStaticBirthDay->SetFontColor( D3DCOLOR_ARGB(255,225,222,0) );
    m_pkStaticBirthDay->SetText(g_pkStrLoader->GetString(STR_ID_USER_MIGRATION_BIRTHDAY));

    m_pkEditLoginID->InitState( true, false, this );
    m_pkEditLoginID->SetFocus();
    m_pkEditLoginID->SetText(L"");
    m_pkEditEmail->InitState( true, false, this );
    m_pkEditEmail->SetText(L"");
    m_pkEditNickname->InitState( true, false, this );
    m_pkEditNickname->SetText(L"");
    m_pkEditDay->InitState( true, false, this );
    m_pkEditDay->SetLimitText(2);
    m_pkEditDay->SetText(L"DD(01)");
    m_pkEditMonth->InitState( true, false, this );
    m_pkEditMonth->SetLimitText(2);
    m_pkEditMonth->SetText(L"MM(01)");
    m_pkEditYear->InitState( true, false, this );
    m_pkEditYear->SetLimitText(4);       
    m_pkEditYear->SetText(L"YYYY(2011)");
}

void KGCUserMigrationDlg::FrameMoveInEnabledState( void )
{

}

void KGCUserMigrationDlg::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkBtnOk,				KD3DWnd::D3DWE_BUTTON_CLICK, SendUserInfo );
    GCWND_MSG_MAP( m_pkBtnCancel,    		KD3DWnd::D3DWE_BUTTON_CLICK, OnCancel );

    if( event.m_pWnd == m_pkEditLoginID )
    {
        if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN || event.m_dwCode == KD3DWnd::D3DWE_EDIT_TAB )
            m_pkEditEmail->SetFocus();
        return;
    }

    if( event.m_pWnd == m_pkEditEmail )
    {
        if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN || event.m_dwCode == KD3DWnd::D3DWE_EDIT_TAB )
            m_pkEditNickname->SetFocus();
        return;
    }

    if( event.m_pWnd == m_pkEditNickname )
    {
        if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN || event.m_dwCode == KD3DWnd::D3DWE_EDIT_TAB )
            m_pkEditDay->SetFocus();
        return;
    }

    if( event.m_pWnd == m_pkEditDay )
    {
        if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN || event.m_dwCode == KD3DWnd::D3DWE_EDIT_TAB )
            m_pkEditMonth->SetFocus();
        return;
    }

    if( event.m_pWnd == m_pkEditMonth )
    {
        if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN || event.m_dwCode == KD3DWnd::D3DWE_EDIT_TAB )
            m_pkEditYear->SetFocus();
        return;
    }

    if( event.m_pWnd == m_pkEditYear )
    {
        if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_TAB )
            m_pkEditLoginID->SetFocus();

        if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN )
            SendUserInfo();
        return;
    }
}

void KGCUserMigrationDlg::OnDestroyComplete( void )
{
}

void KGCUserMigrationDlg::SendUserInfo()
{
    std::wstringstream wstrBrithDay;

    wstrBrithDay << m_pkEditYear->GetText() << m_pkEditMonth->GetText() << m_pkEditDay->GetText();

    KEVENT_USER_AUTH_CHECK_REQ kPacket;
    kPacket.m_strLogin = GCUTIL_STR::GCStrWideToChar(m_pkEditLoginID->GetText());
    kPacket.m_strEmail = GCUTIL_STR::GCStrWideToChar(m_pkEditEmail->GetText());
    kPacket.m_strNickName = GCUTIL_STR::GCStrWideToChar(m_pkEditNickname->GetText());
    kPacket.m_strBirthDay = GCUTIL_STR::GCStrWideToChar(wstrBrithDay.str().c_str());

    SiKP2P()->GetInstance()->Send_UserAuthCheckReq(kPacket);
    int iTemp = 0;
    //g_MyD3D->WaitForServerAck( iTemp, INT_MAX, 3000, TIME_OUT_VALUE );
    g_MyD3D->WaitForServerAck( iTemp, -1, 3000, -1 );

    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCUserMigrationDlg::OnCancel()
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}