#include "stdafx.h"
#include "KGCContributionManager.h"
#include "KGCContributionBox.h"
//
//


IMPLEMENT_CLASSNAME( KGCContributionBox );
IMPLEMENT_WND_FACTORY( KGCContributionBox );
IMPLEMENT_WND_FACTORY_NAME( KGCContributionBox, "gc_contribution_box" );

KGCContributionBox::KGCContributionBox( void )
:m_pkOK( NULL )
,m_pkCancel( NULL )
,m_pkInputGP( NULL )
,m_pkItemView( NULL )
,m_pkMyCount( NULL )
,m_iInputCount( 0 )
{
	LINK_CONTROL( "contribution_confirm_button", m_pkOK );
	LINK_CONTROL( "contribution_cancel_button",	m_pkCancel );
	LINK_CONTROL( "contribution_input_gp_edit", m_pkInputGP );
    LINK_CONTROL( "static_my_count", m_pkMyCount );
	LINK_CONTROL( "item_img_view", m_pkItemView );
}

KGCContributionBox::~KGCContributionBox( void )
{
	// empty
}

void KGCContributionBox::OnCreate( void )
{
	m_pkOK->InitState(true, true, this);
    m_pkCancel->InitState(true, true, this);
	m_pkCancel->SetHotKey(DIK_ESCAPE);
    m_pkInputGP->InitState(true, true, this);
    m_pkMyCount->InitState( true );
	m_pkItemView->InitState( true );
}

void KGCContributionBox::OnCreateComplete( void )
{
    //
    m_pkMyCount->SetText( SiKGCContributionManager()->GetDonationMyItemCount() );

    //
    m_pkItemView->SetItemInfo( SiKGCContributionManager()->GetDonationItemID() / 10 );
    m_pkItemView->ShowItem( true );


}

void KGCContributionBox::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkOK,          KD3DWnd::D3DWE_BUTTON_CLICK, OnClickOKBtn );
    _GCWND_MSG_MAP( m_pkCancel,          KD3DWnd::D3DWE_BUTTON_CLICK, OnExit );
}

void KGCContributionBox::OnClickOKBtn()
{
    int nCount = 0;
    int nMyCount = SiKGCContributionManager()->GetDonationMyItemCount();

    if( IsNumberValue( m_pkInputGP->GetText() ) )
    {
        nCount = _wtoi( m_pkInputGP->GetText() );

        if( nCount <= nMyCount )
            SiKGCContributionManager()->Donating( nCount );
        // else
            // 수량이 부족합니다~
    } else {
        m_pkInputGP->SetText( L"" );
    }

}

bool KGCContributionBox::IsNumberValue( IN std::wstring strInput )
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

void KGCContributionBox::OnExit()
{
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}