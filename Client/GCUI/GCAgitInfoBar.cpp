#include "stdafx.h"
#include "GCAgitInfoBar.h"
#include "KGCAgitGameManager.h"

IMPLEMENT_CLASSNAME( KGCAgitInfoBar );
IMPLEMENT_WND_FACTORY( KGCAgitInfoBar );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitInfoBar, "gc_agit_info_bar" );

KGCAgitInfoBar::KGCAgitInfoBar( void )
{
    for( int i=0 ; i<NUM_OPEN_TYPE ; ++i )
    {
        m_apkOpenType[i] = NULL;

        char szTemp[128] = {0, };
        sprintf_s( szTemp, 127, "openType%d", i );
        LINK_CONTROL( szTemp, m_apkOpenType[i] );
    }
    
    m_pkUserCombo = NULL;
    LINK_CONTROL( "userListCombo", m_pkUserCombo );

    m_pkStaticAgitName = NULL;
    m_pkStaticToday = NULL;
    m_pkStaticNumToday = NULL;
    m_pkStaticTotal = NULL;
    m_pkStaticNumTotal = NULL;
    LINK_CONTROL( "static_agitName", m_pkStaticAgitName );
    LINK_CONTROL( "static_today", m_pkStaticToday );
    LINK_CONTROL( "static_numToday", m_pkStaticNumToday );
    LINK_CONTROL( "static_total", m_pkStaticTotal );
    LINK_CONTROL( "static_numTotal", m_pkStaticNumTotal );
}

KGCAgitInfoBar::~KGCAgitInfoBar( void )
{
}

void KGCAgitInfoBar::OnCreate( void )
{
    for( int i=0 ; i<NUM_OPEN_TYPE ; ++i )
        m_apkOpenType[i]->InitState( false, false, NULL );

    m_pkUserCombo->InitState( true, true, this );
    
    m_pkStaticAgitName->InitState( true, false, NULL );
    m_pkStaticToday->InitState( true, false, NULL );
    m_pkStaticNumToday->InitState( true, false, NULL );
    m_pkStaticTotal->InitState( true, false, NULL );
    m_pkStaticNumTotal->InitState( true, false, NULL );

    m_pkStaticAgitName->SetAlign( DT_CENTER );
    m_pkStaticNumToday->SetAlign( DT_RIGHT );
    m_pkStaticNumTotal->SetAlign( DT_RIGHT );

    SetAgitInfo();
}

void KGCAgitInfoBar::ActionPerformed( const KActionEvent& event )
{

}

void KGCAgitInfoBar::SetAgitInfo( void )
{
    m_pkStaticToday->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_TODAY ) );
    m_pkStaticTotal->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_TOTAL ) );

    SetAgitName();
    SetNumToday();
    SetNumTotal();
    SetOpenType( static_cast<int>(SiKGCAgitGameManager()->GetOpenType()) );
    
    SiKGCAgitGameManager()->SetUserNameMap();
    m_pkUserCombo->Clear();
    m_pkUserCombo->SetUserList( SiKGCAgitGameManager()->GetAgitInfo().m_ucMaxUser, SiKGCAgitGameManager()->GetUserNameMap() );
}

void KGCAgitInfoBar::SetOpenType( int iType_ )
{
    if( 0 > iType_ || iType_ >= NUM_OPEN_TYPE )
        iType_ = KAgitInfo::OT_CLOSE;

    for( int i=0 ; i<NUM_OPEN_TYPE ; ++i )
    {
        if( i == iType_ )
            m_apkOpenType[i]->ToggleRender( true );
        else
            m_apkOpenType[i]->ToggleRender( false );
    }
}

void KGCAgitInfoBar::SetAgitName( void )
{
    std::wstring strAgitName = SiKGCAgitGameManager()->GetAgitName();
    
    if( strAgitName.empty() )
        strAgitName = g_pkStrLoader->GetReplacedString( STR_ID_AGIT_NAME_DEFAULT, "l", SiKGCAgitGameManager()->GetOwnerName() );

    m_pkStaticAgitName->SetText( strAgitName );
}

void KGCAgitInfoBar::SetNumToday( void )
{
    m_pkStaticNumToday->SetNumber( SiKGCAgitGameManager()->GetToday() );
}

void KGCAgitInfoBar::SetNumTotal( void )
{
    m_pkStaticNumTotal->SetNumber( SiKGCAgitGameManager()->GetTotal() );
}

bool KGCAgitInfoBar::CheckPosPopupBoxBound( const D3DXVECTOR2& vTempPos )
{ 
	return m_pkUserCombo->CheckPosPopupBoxBound(vTempPos); 
} 

void KGCAgitInfoBar::SetPopupBoxRender( bool bSet )
{
	return m_pkUserCombo->SetPopupBoxRender( bSet );
}