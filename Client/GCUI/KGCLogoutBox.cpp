#include "stdafx.h"

#include "KGCLogoutBox.h"
#include "BuddySystem/KFriendShip.h"
#include "./gcui/GCNateOn.h"
#include "GCChallengeErrandManager.h"
#include "KGCVirtualDepotManager.h"

IMPLEMENT_CLASSNAME( KGCLogoutBox );
IMPLEMENT_WND_FACTORY( KGCLogoutBox );
IMPLEMENT_WND_FACTORY_NAME( KGCLogoutBox, "gc_logout_box" );

KGCLogoutBox::KGCLogoutBox( void )
:m_pkLogout(NULL)
,m_pkQuit(NULL)
,m_pkCancel(NULL)
{
	LINK_CONTROL( "logout", m_pkLogout );
	LINK_CONTROL( "quit",	m_pkQuit );
	LINK_CONTROL( "cancel", m_pkCancel );	
}

KGCLogoutBox::~KGCLogoutBox( void )
{
	// empty
}

void KGCLogoutBox::OnCreate( void )
{	
	m_pkLogout->SetSelfInputCheck( true );
	m_pkQuit->SetSelfInputCheck( true );
	m_pkCancel->SetSelfInputCheck( true );

	m_pkLogout->AddActionListener( this );
	m_pkQuit->AddActionListener( this );
	m_pkCancel->AddActionListener( this );

}

void KGCLogoutBox::FrameMoveInEnabledState( void )
{
	if ( g_pkInput->IsDown( DIK_L ) )
	{
		OnClickLogout();
		
	}	
	if ( g_pkInput->IsDown( DIK_U ) )
	{		
		OnClickQuit();	
	}	
	if ( g_pkInput->IsDown( DIK_ESCAPE ) )
	{
		OnClickCancel();
        g_pkInput->IgnoreDown(DIK_ESCAPE,30);
	}	
}

void KGCLogoutBox::ActionPerformed( const KActionEvent& event )
{
	//이벤트를 여러번 먹는것을 막기위한 코드임
	if( KD3DWnd::D3DWS_ENABLED != GetWndState() )
		return;

	GCWND_MSG_MAP( m_pkLogout,				KD3DWnd::D3DWE_BUTTON_CLICK, OnClickLogout );
	GCWND_MSG_MAP( m_pkCancel,			KD3DWnd::D3DWE_BUTTON_CLICK, OnClickCancel );
	GCWND_MSG_MAP( m_pkQuit,			KD3DWnd::D3DWE_BUTTON_CLICK, OnClickQuit );		
}


void KGCLogoutBox::OnClickLogout()
{	
#if defined(ONLY_COLLECT_HACKING_DATA)
	if(	g_kGlobalValue.m_bUseCrack )
		g_kGlobalValue.m_bUseCrack = FALSE;
#endif
    g_kGlobalValue.SendSavedCharType();

#if defined( COLLECT_SKILL_HOTKEY_USED_INFO )
    if( g_kGlobalValue.IsEmptySSkillHotKeyUsedInfo() ) {
        KP2P::GetInstance()->Send_SSkillHotKeyUsedInfo( g_kGlobalValue.GetSSkillHotKeyUsedInfo() );
    }                    
#endif

	if( !g_kGlobalValue.m_bUseCrack )
	{
		m_iReturn=LOGOUT;
		ReleaseCapture();       
		g_pkUIScene->m_pkNateOn->SetNateonMode( GCNateOn::INIT_MODE );
		g_pkUIScene->m_pkNateOn->DeleteAllTalkList();	
		g_pkUIScene->m_pkNateOn->DeleteTalkBox();
		SiKFriendShip()->ClearFriendShipData();	
		KP2P::GetInstance()->m_kMsgServer->InitFriendList();
		KP2P::GetInstance()->m_kMsgServer->DisconnectMsgServer();
		g_MyD3D->m_mapPetInfo.clear();
        g_kGlobalValue.m_bUseOldReplay = false;
        if ( g_pkUIScene->m_pkMagicBox != NULL ) {
            g_pkUIScene->m_pkMagicBox->ClearItemList();
        }

        KP2P::GetInstance()->Send_ExitNormalNot();
		SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );			
	}
	else
	{
		m_iReturn=DETECT_HACK_QUIT;
		ReleaseCapture();
        KP2P::GetInstance()->Send_ExitNormalNot();
		SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );		
	}

    SiKGCVirtualDepotManager()->ClearDepotData();
    g_kGlobalValue.m_kHeroDungeonInfo.Release();

    // 아지트에서 파티중일 때 보이는 파티창 감춤
#if defined(PARTY_SYSTEM)
    g_pkUIScene->m_pkPartyWnd->ToggleRender( false );
#endif
}

void KGCLogoutBox::OnClickCancel()
{
	g_KDSound.Play( "32" );
	m_iReturn=CANCEL;
	ReleaseCapture();
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );		
}

void KGCLogoutBox::OnClickQuit()
{
	m_iReturn=QUIT;
	ReleaseCapture();
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );		
}

void KGCLogoutBox::OnDestroyComplete( void )
{
	ReleaseCapture();
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );	
}