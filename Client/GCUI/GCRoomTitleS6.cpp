#include "stdafx.h"
#include "GCRoomTitleS6.h"

IMPLEMENT_CLASSNAME( KGCRoomTitleS6 );
IMPLEMENT_WND_FACTORY( KGCRoomTitleS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomTitleS6, "gc_roomtitle_s6" );

KGCRoomTitleS6::KGCRoomTitleS6( void )
: m_pkRoomNameStatic( NULL ) 
, m_pkRoomNumStatic( NULL ) 
, m_pkRoomPassWordStatic( NULL ) 
, m_pkRoomLockMark( NULL ) 
, m_pkGCBlogMark( NULL ) 
, m_pkGCClubMark( NULL ) 
, m_pkGoldenTicketMark( NULL ) 
, m_pkPremiumGCClub( NULL )
, m_pkCountryGroup( NULL )
{
    LINK_CONTROL( "room_name",  m_pkRoomNameStatic );
    LINK_CONTROL( "room_num",   m_pkRoomNumStatic );
    LINK_CONTROL( "room_password",   m_pkRoomPassWordStatic );
    
    LINK_CONTROL( "lock_mark",  m_pkRoomLockMark );
    
    LINK_CONTROL( "gc_blog",        m_pkGCBlogMark );
    LINK_CONTROL( "gc_club",        m_pkGCClubMark );
    LINK_CONTROL( "golden_ticket",  m_pkGoldenTicketMark );
    LINK_CONTROL( "premium_gc_club", m_pkPremiumGCClub );

    LINK_CONTROL( "option_button", m_pkOptionButton );

    LINK_CONTROL( "country_group", m_pkCountryGroup );
}

KGCRoomTitleS6::~KGCRoomTitleS6( void )
{
}

void KGCRoomTitleS6::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkOptionButton, KD3DWnd::D3DWE_BUTTON_CLICK, OnOptionClick );
}


void KGCRoomTitleS6::OnCreate( void )
{
    m_pkRoomNameStatic->InitState( true, true, this );
    m_pkRoomNumStatic->InitState( true, true, this );
    m_pkRoomNumStatic->SetAlign( DT_RIGHT );

    m_pkRoomPassWordStatic->InitState( true, true, this );    
    m_pkRoomLockMark->InitState( false, true, this );

    m_pkGCBlogMark->InitState( false, true, this );
    m_pkGCClubMark->InitState( false, true, this );
    m_pkGoldenTicketMark->InitState( false, true, this );
    m_pkPremiumGCClub->InitState(false, true, this);
    
    m_pkOptionButton->InitState( true, true, this );
    m_pkOptionButton->SetToolTip( g_pkStrLoader->GetString(STR_ID_CHANGE_ROOM_TITLE_TOOLTIP) );

    if ( m_pkCountryGroup )
        m_pkCountryGroup->InitState( false );
}

void KGCRoomTitleS6::SetRoomName(int m_nRoomNum, const std::wstring& strName)
{
    m_pkRoomNameStatic->SetFontSize( SiKGCMultipleLanguages()->GetLargeFontSize() );
    m_pkRoomNameStatic->SetFontColor( 0xffffffff );
#if defined( NATION_CHINA ) && !defined(__PATH__)
	m_pkRoomNameStatic->SetText( g_kGlobalValue.m_strRelayServer );
#else
	m_pkRoomNameStatic->SetText( strName );
#endif

    m_pkRoomNumStatic->SetFontSize( SiKGCMultipleLanguages()->GetLargeFontSize() );
    m_pkRoomNumStatic->SetFontColor( 0xffffffff );
    m_pkRoomNumStatic->SetText( m_nRoomNum );
}

void KGCRoomTitleS6::SetRoomPassword( const std::wstring& strPass )
{
    m_pkRoomPassWordStatic->SetFontSize( SiKGCMultipleLanguages()->GetLargeFontSize() );
    m_pkRoomPassWordStatic->SetFontColor( 0xffffffff );
    m_pkRoomPassWordStatic->SetText( strPass );

    if( strPass == L"" )
    {
        m_pkRoomLockMark->ToggleRender( false );
    }
    else
    {
        m_pkRoomLockMark->ToggleRender( true );
    }
}

void KGCRoomTitleS6::LockOptionBtn( bool bLock )
{
    m_pkOptionButton->Lock( bLock );
}

void KGCRoomTitleS6::OnOptionClick()
{
    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_CHANGE_ROOM_TITLE_S6);
    return;
}

void KGCRoomTitleS6::SetCountryGroup()
{
    if ( m_pkCountryGroup ) 
        m_pkCountryGroup->ToggleRender( true );

    for ( int i = 0 ; i < MAX_PLAYER_NUM ; i++ )
    {
        if( m_pkCountryGroup && g_MyD3D->MyPlayer[i]->m_kUserInfo.bHost )
        {
            m_pkCountryGroup->SetCountryGroup(g_MyD3D->MyPlayer[i]->m_kUserInfo.strNation, true, STR_ID_PLAY_SAME_COUNTRY_MESSAGE0, ED3DWND_TOOLTIP_ALIGN_RIGHT);
        }
    }
}

void KGCRoomTitleS6::SetPrimium()
{
    int nHostPlay = g_MyD3D->Get_HostPlayer();
    if ( !g_MyD3D->IsPlayerIndex( nHostPlay ) ) 
        return;

   
    int iPremiumMark = SiKGCRoomListManager()->ConvertPremium( g_MyD3D->MyPlayer[nHostPlay]->m_kUserInfo.dwPremium );
    bool bTicketDun = SiKGCWorldMapManager()->IsEventDungeon(SiKGCRoomManager()->GetGameMode());

    m_pkGCBlogMark->ToggleRender(iPremiumMark == KGCRoomListManager::KRLM_PRIMIUM_GCBLOG);
    m_pkGCClubMark->ToggleRender(iPremiumMark == KGCRoomListManager::KRLM_PRIMIUM_GCCLUB);
    m_pkGoldenTicketMark->ToggleRender(iPremiumMark == KGCRoomListManager::KRLM_PRIMIUM_GOLDEN_TICKET && bTicketDun );
    m_pkPremiumGCClub->ToggleRender(iPremiumMark == KGCRoomListManager::KRLM_PRIMIUM_GCCLUB_PREMIUM);
}