#include "stdafx.h"
#include "GCRoomBtnS6.h"
#include "KGCRoomListManager.h"
#include "KGCCategoryBox.h"
#include "KGCWorldMapManager.h"

IMPLEMENT_CLASSNAME( KGCRoomBtnBackS6 );
IMPLEMENT_WND_FACTORY( KGCRoomBtnBackS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomBtnBackS6, "gc_roombtn_back_s6" );

KGCRoomBtnBackS6::KGCRoomBtnBackS6( void )
{
	for ( int i = 0 ; i < 4 ; ++i ){
		m_saBack[i] = NULL;
	}

	LINK_CONTROL("left_back", m_saBack[0]);
	LINK_CONTROL("center_back", m_saBack[1]);
	LINK_CONTROL("right_back", m_saBack[2]);
	LINK_CONTROL("room_option_back", m_saBack[3]);
}

KGCRoomBtnBackS6::~KGCRoomBtnBackS6( void )
{
}

void KGCRoomBtnBackS6::ActionPerformed( const KActionEvent& event )
{
	for ( int i = 0 ; i < 4 ; ++i ){
		GCWND_MSG_MAP( m_saBack[i], KD3DWnd::D3DWE_BUTTON_CLICK, OnClickBtn );
	}

}


void KGCRoomBtnBackS6::OnCreate( void )
{
	for ( int i = 0 ; i < 4 ; ++i ){
		if ( m_saBack[i] != NULL ) {
			m_saBack[i]->InitState(true,true,this);
		}

	}
	
}

void KGCRoomBtnBackS6::InitWnd( void )
{
	for ( int i = 0 ; i < 4 ; ++i ){
		if ( m_saBack[i] != NULL ) {
			m_saBack[i]->InitState(true,true,this);
		}

	}

}

void KGCRoomBtnBackS6::OnClickBtn( void )
{
	m_pParent->SendEvent( KActionEvent( this, KD3DWnd::D3DWE_BUTTON_CLICK ) );
}

IMPLEMENT_CLASSNAME( KGCRoomBtnS6 );
IMPLEMENT_WND_FACTORY( KGCRoomBtnS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomBtnS6, "gc_room_btn_s6" );

KGCRoomBtnS6::KGCRoomBtnS6( void )
: m_pkRoomNumber(NULL)
, m_pkRoomStr(NULL)
, m_pkRoomUser(NULL)
, m_pkLock(NULL)
, m_pkGCBlog(NULL)
, m_pkGCClub(NULL)
, m_pkGoldenTicket(NULL)
, m_pkPremiumGCClub(NULL)
, m_pkState(NULL)
, m_pkCategory(NULL)
, m_pkBack(NULL)
{
	LINK_CONTROL("static_room_num", m_pkRoomNumber);
	LINK_CONTROL("static_room_name", m_pkRoomStr);
	LINK_CONTROL("static_user_num", m_pkRoomUser);
	LINK_CONTROL("close", m_pkLock);
	LINK_CONTROL("gc_blog", m_pkGCBlog);
	LINK_CONTROL("gc_club", m_pkGCClub);
	LINK_CONTROL("golden_ticket", m_pkGoldenTicket);
	LINK_CONTROL("premium_gc_club", m_pkPremiumGCClub);
	LINK_CONTROL("state",m_pkState);
	LINK_CONTROL("category_box",m_pkCategory);
	LINK_CONTROL("back_on",m_pkBack);

	for( int i = 0; i < MAX_OPTIONS; i++ )
	{
		m_saOptions[i] = NULL;
		LINK_CONTROL_STM( "room_option" << i, m_saOptions[i] );
	}
}

KGCRoomBtnS6::~KGCRoomBtnS6( void )
{
}

void KGCRoomBtnS6::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == D3DWE_BUTTON_CLICK )
	{
		if( event.m_pWnd == m_saOptions[OPTION_ALLOWED_RANKS] )
		{
			OnClickAllowedRank();
		}
		
		if( event.m_pWnd == m_saOptions[OPTION_ALLOWED_CHARS] )
		{
			OnClickBlockedChars();
		}
	}
}

void KGCRoomBtnS6::FrameMoveInEnabledState()
{
	if ( m_pkBack ) {
		POINT pt = g_pkInput->GetMousePos();
		D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
		if( m_pkBack->CheckPosInWindowBound(vMousePos) )
		{
			m_pkBack->ToggleRender( true );
		}
		else
		{
			m_pkBack->ToggleRender( false );
		}
	}
}

void KGCRoomBtnS6::OnClickBtn( void )
{
	m_pParent->SendEvent( KActionEvent( this, KD3DWnd::D3DWE_BUTTON_CLICK ) );
}


void KGCRoomBtnS6::OnCreate( void )
{
	m_pkRoomNumber->InitState(false);
	m_pkRoomStr->InitState(false);
	m_pkRoomUser->InitState(false);

	m_pkLock->InitState(false);
	m_pkState->InitState(false);
	m_pkGCBlog->InitState(false);
	m_pkGCClub->InitState(false);
	m_pkGoldenTicket->InitState(false);
	m_pkPremiumGCClub->InitState(false);

	if ( m_pkCategory)
		m_pkCategory->InitState(false);

	if ( m_pkBack) {
		m_pkBack->InitState(false,true,this);
		m_pkBack->DisconnectAllEventProcedure();
		m_pkBack->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK,this,&KGCRoomBtnS6::OnClickBtn);
	}

	for( int i = 0; i < MAX_OPTIONS; i++ )
	{
		if(m_saOptions[i])
		{
			m_saOptions[i]->InitState( false, true, this );
			m_saOptions[i]->SetToolTip( g_pkStrLoader->GetString(STR_ID_ROOM_EXTRA_OPTION_BUTTON_LIST + i));
		}
	}
}

void KGCRoomBtnS6::OnClickAllowedRank()
{
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ROOM_OPTION_RANK_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_kRoomInfo.m_kOptions.m_dwAllowedRankings, 0, false, false );
}

void KGCRoomBtnS6::OnClickBlockedChars()
{
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ROOM_OPTION_CHAR_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_kRoomInfo.m_kOptions.m_dwDisabledChars, 0, false, false );
}

void KGCRoomBtnS6::InitWnd( void )
{
	m_pkRoomNumber->InitState(false);
	m_pkRoomStr->InitState(false);
	m_pkRoomUser->InitState(false);

	m_pkLock->InitState(false);
	m_pkState->InitState(false);
	m_pkGCBlog->InitState(false);
	m_pkGCClub->InitState(false);
	m_pkGoldenTicket->InitState(false);
	m_pkPremiumGCClub->InitState(false);

	if ( m_pkCategory )
		m_pkCategory->InitState(false);

	if ( m_pkBack ) {
		m_pkBack->InitState(false,true,this);
		m_pkBack->DisconnectAllEventProcedure();
		m_pkBack->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK,this,&KGCRoomBtnS6::OnClickBtn);
	}

	for( int i = 0; i < MAX_OPTIONS; i++ )
	{
		if(m_saOptions[i])
		{
			m_saOptions[i]->InitState( false, true, this );
			m_saOptions[i]->SetToolTip( g_pkStrLoader->GetString(STR_ID_ROOM_EXTRA_OPTION_BUTTON_LIST + i));
		}
	}
}

void KGCRoomBtnS6::ClearBarInfo( void )
{
	m_pkRoomNumber->ToggleRender(false);
	m_pkRoomStr->ToggleRender(false);
	m_pkRoomUser->ToggleRender(false);

	m_pkLock->ToggleRender(false);
	m_pkState->ToggleRender(false);
	m_pkGCBlog->ToggleRender(false);
	m_pkGCClub->ToggleRender(false);
	m_pkGoldenTicket->ToggleRender(false);
	m_pkPremiumGCClub->ToggleRender(false);

	if ( m_pkCategory )
		m_pkCategory->ToggleRender(false);

	if ( m_pkBack )
		m_pkBack->ToggleRender(true);

	for( int i = 0; i < MAX_OPTIONS; i++ )
	{
		if( m_saOptions[i] )
		{
			m_saOptions[i]->ToggleRender( false );
		}
	}
}

void KGCRoomBtnS6::OnBarInfo( void )
{
	m_pkRoomNumber->ToggleRender(true);
	m_pkRoomStr->ToggleRender(true);
	m_pkRoomUser->ToggleRender(true);

	m_pkLock->ToggleRender(false);
	m_pkState->ToggleRender(true);
	m_pkGCBlog->ToggleRender(false);
	m_pkGCClub->ToggleRender(false);
	m_pkGoldenTicket->ToggleRender(false);
	m_pkPremiumGCClub->ToggleRender(false);

	if ( m_pkCategory )
		m_pkCategory->ToggleRender(false);

	if ( m_pkBack )
		m_pkBack->ToggleRender(true);
}

void KGCRoomBtnS6::SetRoomBtnInfo( KSimpleRoomInfo &kRoomInfo )
{
	m_kRoomInfo = kRoomInfo;

	OnBarInfo();
	m_pkRoomNumber->SetText(m_kRoomInfo.m_usRoomID + 1);
	m_pkRoomNumber->SetAlign(DT_CENTER);
	m_pkRoomStr->SetText( m_kRoomInfo.m_strRoomName );

    if( m_kRoomInfo.m_bObserverInRoom )
    {
        m_kRoomInfo.m_usMaxUsers--;
        m_kRoomInfo.m_usUsers--;
    }

    std::wstringstream stm;
	stm<<m_kRoomInfo.m_usUsers<<L"/"<<m_kRoomInfo.m_usMaxUsers;

	m_pkRoomUser->SetText(stm.str());
	m_pkRoomUser->SetAlign(DT_CENTER);

	m_pkLock->ToggleRender( !m_kRoomInfo.m_bPublic );
	m_pkState->Lock( m_kRoomInfo.m_bPlaying );

	int iPremiumMark = SiKGCRoomListManager()->ConvertPremium( m_kRoomInfo.m_kPremiumInfo.m_dwPremiumType );

	m_pkGCBlog->ToggleRender(iPremiumMark == KGCRoomListManager::KRLM_PRIMIUM_GCBLOG);
	m_pkGCClub->ToggleRender(iPremiumMark == KGCRoomListManager::KRLM_PRIMIUM_GCCLUB);
	m_pkPremiumGCClub->ToggleRender(iPremiumMark == KGCRoomListManager::KRLM_PRIMIUM_GCCLUB_PREMIUM);
	m_pkGoldenTicket->ToggleRender(iPremiumMark == KGCRoomListManager::KRLM_PRIMIUM_GOLDEN_TICKET && SiKGCWorldMapManager()->IsEventDungeon(static_cast<EGCGameMode>(m_kRoomInfo.m_nGameMode)));

	if ( m_pkCategory ) {
		m_pkCategory->UpdateCategory(m_kRoomInfo.m_cGameCategory);
		m_pkCategory->ToggleRender(true);
	}

	if ( m_pkBack )
	{
		m_pkBack->InitState(false, true);
	}

	if( m_saOptions[OPTION_ATTRIBUTE_CORRECTION])
	{
		bool bRender = (m_kRoomInfo.m_kOptions.m_dwEnabledOptions & KRoomOptions::ATTRIBUTE_CORRECTION) == KRoomOptions::ATTRIBUTE_CORRECTION;
		m_saOptions[OPTION_ATTRIBUTE_CORRECTION]->ToggleRender( bRender );
		m_saOptions[OPTION_ATTRIBUTE_CORRECTION]->SetOrder_Top();
	}

	if( m_saOptions[OPTION_BLOCK_SPECIAL4_ATTACK])
	{
		bool bRender = (m_kRoomInfo.m_kOptions.m_dwEnabledOptions & KRoomOptions::BLOCK_SUPREME_SKILL) == KRoomOptions::BLOCK_SUPREME_SKILL;
		m_saOptions[OPTION_BLOCK_SPECIAL4_ATTACK]->ToggleRender( bRender );
		m_saOptions[OPTION_BLOCK_SPECIAL4_ATTACK]->SetOrder_Top();
	}

	if( m_saOptions[OPTION_BLOCK_PET_ATTACK])
	{
		bool bRender = (m_kRoomInfo.m_kOptions.m_dwEnabledOptions & KRoomOptions::BLOCK_PET_OPTION) == KRoomOptions::BLOCK_PET_OPTION;
		m_saOptions[OPTION_BLOCK_PET_ATTACK]->ToggleRender( bRender );
		m_saOptions[OPTION_BLOCK_PET_ATTACK]->SetOrder_Top();
	}

	if( m_saOptions[OPTION_ALLOWED_RANKS])
	{
		m_saOptions[OPTION_ALLOWED_RANKS]->ToggleRender( m_kRoomInfo.m_kOptions.m_dwAllowedRankings != 0xFFFFFFFF );
		m_saOptions[OPTION_ALLOWED_RANKS]->SetOrder_Top();
	}

	if( m_saOptions[OPTION_ALLOWED_CHARS])
	{
		m_saOptions[OPTION_ALLOWED_CHARS]->ToggleRender( m_kRoomInfo.m_kOptions.m_dwDisabledChars != 0 );
		m_saOptions[OPTION_ALLOWED_CHARS]->SetOrder_Top();
	}
}