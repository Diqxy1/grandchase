#include "stdafx.h"
#include "GCBuffDonationDlg.h"
#include "GCBuffDonationManager.h"


IMPLEMENT_CLASSNAME( KGCBuffDonationDlg );
IMPLEMENT_WND_FACTORY( KGCBuffDonationDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCBuffDonationDlg, "gc_buff_donation_dlg" );

KGCBuffDonationDlg::KGCBuffDonationDlg( void )
{
    m_pkBtnOk = NULL;
    m_pkCancel = NULL;
    m_pkStaticMyOwn = NULL;
    m_pkStaticDesc = NULL;
    m_pkEditInput = NULL;

    LINK_CONTROL( "btn_ok", m_pkBtnOk );
    LINK_CONTROL( "btn_cancel", m_pkCancel );
    LINK_CONTROL( "static_my_own", m_pkStaticMyOwn );
    LINK_CONTROL( "static_desc", m_pkStaticDesc );
    LINK_CONTROL( "edit_input", m_pkEditInput );

}

KGCBuffDonationDlg::~KGCBuffDonationDlg( void )
{
}


void KGCBuffDonationDlg::OnCreate( void )
{
    m_pkBtnOk->InitState( true, true, this );
    m_pkCancel->InitState( true, true, this );
    m_pkStaticMyOwn->InitState( true );
    m_pkStaticDesc->InitState( true );
    m_pkEditInput->InitState( true, false, this );

    m_pkStaticDesc->SetText( g_pkStrLoader->GetString(STR_ID_BUFF_DONATION_STR_2) );
}

void KGCBuffDonationDlg::OnCreateComplete( void )
{
    int nCountOwn = SiGCBuffDonationManager()->GetDonationItemCount();
    m_pkStaticMyOwn->SetText( static_cast<int>( nCountOwn ) );

    m_pkEditInput->SetText( L"" );
    m_pkEditInput->SetFocus();
}

void KGCBuffDonationDlg::FrameMoveInEnabledState( void )
{
    //
}


void KGCBuffDonationDlg::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnOk,          KD3DWnd::D3DWE_BUTTON_CLICK, OnClickOK );
    _GCWND_MSG_MAP( m_pkCancel,          KD3DWnd::D3DWE_BUTTON_CLICK, OnClickCancel );
}


void KGCBuffDonationDlg::OnClickOK()
{
    int nCount = 0;

    if( IsNumberValue( m_pkEditInput->GetText() ) )
    {
        nCount = _wtoi( m_pkEditInput->GetText() );
        SiGCBuffDonationManager()->Donating( nCount );
        
        int nCountOwn = SiGCBuffDonationManager()->GetDonationItemCount();
        m_pkStaticMyOwn->SetText( static_cast<int>( nCountOwn ) );
    } else {
        m_pkEditInput->SetText( L"" );
    }

}


bool KGCBuffDonationDlg::IsNumberValue( IN std::wstring strInput )
{
    for( int i = 0 ; i < (int)strInput.size() ; ++i )
    {
        if( !iswdigit(strInput[i] ) )
        {
            return false;
        }
    }

    return true;
}

void KGCBuffDonationDlg::OnClickCancel()
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}