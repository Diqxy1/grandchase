#include "stdafx.h"
#include "GCAutoMatchingS6.h"

IMPLEMENT_CLASSNAME( KGCMatchUserBtnS6 );
IMPLEMENT_WND_FACTORY( KGCMatchUserBtnS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCMatchUserBtnS6, "gc_match_invite_user_btn_s6" );

KGCMatchUserBtnS6::KGCMatchUserBtnS6( void )
: m_pkNick(NULL)
, m_dwUserUID(0)
, m_pkBtn(NULL)
, m_tmInvite(0)
{
	LINK_CONTROL("static_name", m_pkNick);
	LINK_CONTROL("ok_btn", m_pkBtn);
}

KGCMatchUserBtnS6::~KGCMatchUserBtnS6( void )
{

}

void KGCMatchUserBtnS6::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickInvite );
}

void KGCMatchUserBtnS6::OnCreate( void )
{
	m_pkNick->InitState(true);
	m_pkBtn->InitState(true, true, this);
	m_pkBtn->Lock(true);
}

void KGCMatchUserBtnS6::SetUserInfo( std::wstring strNick, DWORD dwUserUID, bool bOnBtn )
{
	m_pkNick->SetText(strNick);
	m_dwUserUID = dwUserUID;
	m_pkBtn->Lock(!bOnBtn);
}


void KGCMatchUserBtnS6::FrameMoveInEnabledState( )
{
	if ( m_dwUserUID == 0 )
		return;
	
	if ( m_pkBtn->IsLocked() == false )
		return;

	time_t curTime = GCUTIL_TIME::GetCurTime();

	if ( curTime > m_tmInvite )	
		m_pkBtn->Lock(false);
}

void KGCMatchUserBtnS6::OnClickInvite( )
{
	SiKGCPartyManager()->SendInviteUser(m_dwUserUID, KPartyData::PT_MATCH);
	m_tmInvite = GCUTIL_TIME::GetCurTime() + 5;
	m_pkBtn->Lock(true);
}



IMPLEMENT_CLASSNAME( KGCMatchInviteUserListS6 );
IMPLEMENT_WND_FACTORY( KGCMatchInviteUserListS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCMatchInviteUserListS6, "gc_match_invite_firend_s6" );

KGCMatchInviteUserListS6::KGCMatchInviteUserListS6( void )
: m_pkBtnClose(NULL)
, m_pkBtnLeft(NULL)
, m_pkBtnRight(NULL)
, m_iCurrentPage(0)
{
	for ( int i = 0 ; i < KGCMatchInviteUserListS6::KMIUL_USER_BTN_MAX ; ++i ) {
		m_saUserBtn[i] = NULL;
	}

	LINK_CONTROL("close_btn", m_pkBtnClose);
	LINK_CONTROL("left_page", m_pkBtnLeft);
	LINK_CONTROL("right_page", m_pkBtnRight);
}

KGCMatchInviteUserListS6::~KGCMatchInviteUserListS6( void )
{

}

void KGCMatchInviteUserListS6::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickClose );
	GCWND_MSG_MAP( m_pkBtnLeft, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickLeft );
	GCWND_MSG_MAP( m_pkBtnRight, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickRight );
}

void KGCMatchInviteUserListS6::OnCreate( void )
{
	m_pkBtnLeft->InitState(true,true,this);
	m_pkBtnRight->InitState(true,true,this);
	m_pkBtnClose->InitState(true,true,this);

	m_iCurrentPage = 0;

	if ( m_saUserBtn[0] == NULL ) {
		std::vector<KD3DWnd*> &vecUserBtn =  g_pkUIMgr->CreateTemplateUIListByFindName(this, 
			"ui_scene_match_s6.stg", "","match_invite_firend_s6\\user_btn",true, KGCMatchInviteUserListS6::KMIUL_USER_BTN_MAX );

		for ( int i = 0 ; i < KGCMatchInviteUserListS6::KMIUL_USER_BTN_MAX ; ++i ) {
			m_saUserBtn[i] = static_cast<KGCMatchUserBtnS6*>(vecUserBtn[i]);
			m_saUserBtn[i]->SetWindowPosY(33.f * static_cast<float>(i) + m_saUserBtn[i]->GetFixedWindowLocalPos().y);
		}
	}

	for ( int i = 0 ; i < KGCMatchInviteUserListS6::KMIUL_USER_BTN_MAX ; ++i ) {
		m_saUserBtn[i]->InitState(true,true,this);
	}

}

void KGCMatchInviteUserListS6::UpdateUserList( int iPage )
{
	std::vector<KMatchInviteUserInfo> vecResult;
	if ( SiKGCMatchInviteManager()->GetCurrentPageUserList( m_iCurrentPage, KGCMatchInviteUserListS6::KMIUL_USER_BTN_MAX,vecResult) == false ) {
		if ( vecResult.empty())
			return;
		iPage = 0;
	}

	m_iCurrentPage = iPage;

	for ( int i = 0 ; i < KGCMatchInviteUserListS6::KMIUL_USER_BTN_MAX ; ++i ) {
		m_saUserBtn[i]->SetUserInfo(L"",0,false);
	}

	for ( int i = 0 ; i < static_cast<int>(vecResult.size()) ; ++i ) {
		m_saUserBtn[i]->SetUserInfo(vecResult[i].m_strNickName, vecResult[i].m_dwUserUID ,true);
	}
}


void KGCMatchInviteUserListS6::OnClickClose( void )
{
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );

}

void KGCMatchInviteUserListS6::OnClickLeft( void )
{
	UpdateUserList(m_iCurrentPage-1);

}

void KGCMatchInviteUserListS6::OnClickRight( void )
{
	UpdateUserList(m_iCurrentPage+1);
}


IMPLEMENT_CLASSNAME( KGCMatchUserIconS6 );
IMPLEMENT_WND_FACTORY( KGCMatchUserIconS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCMatchUserIconS6, "gc_user_icon_s6" );

KGCMatchUserIconS6::KGCMatchUserIconS6( void )
: m_pkIcon(NULL)
, m_pkCheck(NULL)
, m_pkBack(NULL)
, m_pkUserName(NULL)
{
	LINK_CONTROL("user_view",m_pkIcon);
	LINK_CONTROL("check",m_pkCheck);
	LINK_CONTROL("check_back",m_pkBack);
	LINK_CONTROL("nickname_static",m_pkUserName);
	
}

KGCMatchUserIconS6::~KGCMatchUserIconS6( void )
{
	
}

void KGCMatchUserIconS6::ActionPerformed( const KActionEvent& event )
{
}

void KGCMatchUserIconS6::OnCreate( void )
{
	m_pkIcon->InitState(true);
	m_pkCheck->InitState(false);
	m_pkUserName->InitState(false);
	m_pkBack->InitState(true);
}

void KGCMatchUserIconS6::SetIconAct( bool bAct )
{
	m_pkIcon->Lock(bAct);
}

void KGCMatchUserIconS6::SetUserNickName( std::wstring strUserNick ,DWORD dwARGB)
{
	m_pkUserName->ToggleRender(true);
	m_pkUserName->SetText(strUserNick.c_str());
	m_pkUserName->SetFontColor(dwARGB);
}

void KGCMatchUserIconS6::SetCheck( int iCheckState )
{
	switch ( iCheckState ) {
		case KGCMatchUserIconS6::KMUI_CHECK_STATE_NOT_RENDER :
			m_pkCheck->ToggleRender(false);
			m_pkBack->ToggleRender(true);
			break;
		case KGCMatchUserIconS6::KMUI_CHECK_STATE_X :
			m_pkCheck->ToggleRender(true);
			m_pkBack->ToggleRender(true);
			m_pkCheck->Lock(true);
			break;
		case KGCMatchUserIconS6::KMUI_CHECK_STATE_V :
			m_pkCheck->ToggleRender(true);
			m_pkCheck->Lock(false);
			m_pkBack->ToggleRender(true);
			break;
		default:
			break;
	}
}


IMPLEMENT_CLASSNAME( KGCGaugeS6 );
IMPLEMENT_WND_FACTORY( KGCGaugeS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCGaugeS6, "gc_gauge_s6" );

KGCGaugeS6::KGCGaugeS6( void )
: m_pkInImage(NULL)
, m_dwOriginalWidth(0)
, m_iPrePoint(-1)
, m_fBasePosX(0.0f)
{
	LINK_CONTROL("in", m_pkInImage);

}

KGCGaugeS6::~KGCGaugeS6( void )
{

}

void KGCGaugeS6::ActionPerformed( const KActionEvent& event )
{
}

void KGCGaugeS6::OnCreate( void )
{
	m_pkInImage->InitState(true);
	
	if ( m_dwOriginalWidth == 0 ) {
		m_dwOriginalWidth = m_pkInImage->GetWidth();
		m_avTexDef = m_pkInImage->GetWndTexCoords(D3DWM_DEFAULT);
		m_fBasePosX = m_pkInImage->GetFixedWindowLocalPos().x;
	}

	m_iPrePoint = -1;
}

void KGCGaugeS6::SetGauge( int iCurrent, int iMax, int iType )
{
	if ( iCurrent == m_iPrePoint || iCurrent < 0 ) return;
	
	m_iPrePoint = iCurrent;

	float percent = static_cast<float>(iCurrent) / static_cast<float>(iMax);

	if ( percent > 1.0f ) 
		percent = 1.0f;

	//소수점 자리 비울려고...
	float fWidth = static_cast<float>( static_cast<DWORD>( m_dwOriginalWidth * percent ) );
	float fTexWidth =  ( m_avTexDef.z - m_avTexDef.x )  * percent;

	SetSameExtentionSize(
		fWidth,
		static_cast<float>(m_dwOriginalWidth),
		fTexWidth,
		( m_avTexDef.z - m_avTexDef.x ));

	D3DXVECTOR4 av4;
	av4 = m_avTexDef;
	if ( iType == AMLB_GAGE_START_RIGHT ) {
		av4.x = m_avTexDef.z - fTexWidth;
		av4.z = m_avTexDef.z;
		float fPosX = m_fBasePosX + ( static_cast<float>(m_dwOriginalWidth) - fWidth );
		m_pkInImage->SetWindowPosXDirect(fPosX);	
	}
	else {
		av4.x = m_avTexDef.x;
		av4.z = m_avTexDef.x + fTexWidth;
		m_pkInImage->SetWindowPosXDirect(m_fBasePosX);
	}


	m_pkInImage->SetWndTexCoords(D3DWM_DEFAULT,av4);
	m_pkInImage->SetWidthDirect( static_cast<DWORD>(fWidth) );

}

void KGCGaugeS6::SetSameExtentionSize ( float &fCurrentSize, float fMaxSize, float &fCurrentTexSize, float fMaxTexSize ) 
{
	// 3:1 == 9:3  1*9 == 3*3
	float fLLCDSize = fCurrentSize * fMaxTexSize;
	float fRLCDSize = fMaxSize * fCurrentTexSize;

	if (fLLCDSize == fRLCDSize)
		return;

	fCurrentTexSize = fLLCDSize / fMaxSize; 
}

IMPLEMENT_CLASSNAME( KGCMatchCompleteDlgS6 );
IMPLEMENT_WND_FACTORY( KGCMatchCompleteDlgS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCMatchCompleteDlgS6, "gc_auto_matching_cpmplete_s6" );

KGCMatchCompleteDlgS6::KGCMatchCompleteDlgS6( void )
: m_pkOKBtn(NULL)
, m_pkCancelBtn(NULL)
, m_pkGauge(NULL)
, m_pkTime(NULL)
, m_tmCurrentTime(0)
, m_tmMaxTime(0)
{
	LINK_CONTROL("ok_btn",m_pkOKBtn);
	LINK_CONTROL("cancel_btn",m_pkCancelBtn);
	LINK_CONTROL("time_gauge",m_pkGauge);
	LINK_CONTROL("static_time",m_pkTime);

	for ( int i = 0 ; i < KMCD_USER_MAX ; ++i ) {
		char str[MAX_PATH];
		m_saUserIcon[i] = NULL;
		sprintf( str, "user_icon_%d", i );
		LINK_CONTROL( str, m_saUserIcon[i] );
	}
}

KGCMatchCompleteDlgS6::~KGCMatchCompleteDlgS6( void )
{

}

void KGCMatchCompleteDlgS6::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkCancelBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OverTime );
	GCWND_MSG_MAP( m_pkOKBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickOKBtn );

}

void KGCMatchCompleteDlgS6::OnCreate( void )
{
	m_pkOKBtn->InitState(true,true,this);
	m_pkCancelBtn->InitState(true,true,this);
	m_pkGauge->InitState(true);
	m_pkTime->InitState(true);
	m_pkTime->SetAlign(DT_RIGHT);

	InitUserIcon();

	m_tmCurrentTime = 0;
	m_tmMaxTime = 0;
	m_dwCloseTime =0;

	SetUserInfo();
	StartTimer(KGCMatchCompleteDlgS6::KMCD_WAIT_TIME);

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(), KGCUIScene::D3DWE_PRE_ENTER_SCENE);
	g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_PRE_ENTER_SCENE, this, &KGCMatchCompleteDlgS6::OnCloseDlgDirect );

	g_KDSound.Play( "766" );


	if ( SiKGCMatchManager()->GetStartOption() )
		OnClickOKBtn();
}

void KGCMatchCompleteDlgS6::OnClickOKBtn( )
{
	KP2P::GetInstance()->Send_AcceptMatchReq(true, SiKGCRoomManager()->CreateIndoorUserInfo()  );
	m_pkOKBtn->ToggleRender(false);
	m_pkCancelBtn->ToggleRender(false);

}

void KGCMatchCompleteDlgS6::InitUserIcon( )
{	
	for ( int i = 0 ; i < KMCD_USER_MAX ; ++i ) {
		m_saUserIcon[i]->SetCheck(KGCMatchUserIconS6::KMUI_CHECK_STATE_NOT_RENDER);
		m_saUserIcon[i]->SetIconAct(false);
		m_saUserIcon[i]->ToggleRender(false);
	}
}

void KGCMatchCompleteDlgS6::FrameMoveInEnabledState( void )
{

	if ( m_tmMaxTime == 0 ) return;	

	++m_tmCurrentTime;


	if ( m_tmCurrentTime >= m_tmMaxTime && m_dwCloseTime == 0 ) {
		if ( m_pkCancelBtn->IsRenderOn() )
			OverTime();
	}

	if ( m_tmCurrentTime >= m_tmMaxTime + 3 ) {
		KP2P::GetInstance()->Send_AcceptMatchReq(false, SiKGCRoomManager()->CreateIndoorUserInfo());
		OnCloseDlgDirect();
		SiKGCMatchManager()->InitMatchInfo();
		SiKGCPartyManager()->EndParty();
		g_pkUIMgr->SendEvent( KActionEvent( NULL,  KGCUIScene::D3DWE_UPDATE_MATCH ) );
		return;
	}

	int iMax = static_cast<int>( m_tmMaxTime );
	int iCurrent = static_cast<int>( m_tmMaxTime - m_tmCurrentTime );

	if ( iCurrent < 0 )
		iCurrent = 0;
	
	m_pkTime->SetText( g_pkStrLoader->GetReplacedString(STR_ID_SECOND_TIME, "i", iCurrent/GC_FPS_LIMIT) );
	
	m_pkGauge->SetGauge(iCurrent,iMax);

	if ( m_dwCloseTime < m_tmCurrentTime && m_dwCloseTime != 0 ) {
		OnCloseDlgDirect();
		return;
	}

}

void KGCMatchCompleteDlgS6::OverTime( )
{
	KP2P::GetInstance()->Send_AcceptMatchReq(false, SiKGCRoomManager()->CreateIndoorUserInfo());
	m_pkOKBtn->ToggleRender(false);
	m_pkCancelBtn->ToggleRender(false);
	OnCloseDlgDirect();
}

void KGCMatchCompleteDlgS6::OnCloseDlg( )
{
	m_dwCloseTime = m_tmCurrentTime + ( 3 * GC_FPS_LIMIT );
}


void KGCMatchCompleteDlgS6::OnCloseDlgDirect( )
{
	m_tmCurrentTime = 0;
	m_tmMaxTime = 0;
	m_dwCloseTime = 0;
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCMatchCompleteDlgS6::StartTimer( DWORD dwMaxTime )
{
	m_tmCurrentTime = 0;
	m_tmMaxTime = dwMaxTime * GC_FPS_LIMIT;
}


void KGCMatchCompleteDlgS6::SetCancelUser( DWORD dwUserUID )
{
	std::map<int, std::vector<KPartyMember> > mapUser =	SiKGCMatchManager()->GetMatchCompleteUser();

	std::vector<KPartyMember> vecBlueTeam;
	std::vector<KPartyMember> vecRedTeam;

	std::map<int, std::vector<KPartyMember> >::iterator mit = mapUser.find( KGCMatchCompleteDlgS6::KMCD_TEAM_BLUE );
	if ( mit != mapUser.end() ) vecBlueTeam = mit->second;

	mit = mapUser.find( KGCMatchCompleteDlgS6::KMCD_TEAM_RED );
	if ( mit != mapUser.end() ) vecRedTeam = mit->second;

	int i = 2;

	std::vector<KPartyMember>::iterator vitBlue = vecBlueTeam.begin();
	for ( int i = 2 ; i > -1 ; --i ) {
		if ( vitBlue == vecBlueTeam.end() ) {
			continue;
		}

		if ( vitBlue->m_dwUID == dwUserUID ) {
			m_saUserIcon[i]->SetCheck(KGCMatchUserIconS6::KMUI_CHECK_STATE_X );

		}

		++vitBlue;
	}

	std::vector<KPartyMember>::iterator vitRed = vecRedTeam.begin();
	for ( int i = 3 ; i < KGCMatchCompleteDlgS6::KMCD_USER_MAX ; ++i ) {
		if ( vitRed == vecRedTeam.end() ) {
			continue;
		}

		if ( vitRed->m_dwUID == dwUserUID ) {
			m_saUserIcon[i]->SetCheck(KGCMatchUserIconS6::KMUI_CHECK_STATE_X );

		}

		++vitRed;
	}
	
}

void KGCMatchCompleteDlgS6::SetUserInfo( )
{
	std::map<int, std::vector<KPartyMember> > mapUser =	SiKGCMatchManager()->GetMatchCompleteUser();
	
	std::vector<KPartyMember> vecBlueTeam;
	std::vector<KPartyMember> vecRedTeam;

	std::map<int, std::vector<KPartyMember> >::iterator mit = mapUser.find( KGCMatchCompleteDlgS6::KMCD_TEAM_BLUE );
	if ( mit != mapUser.end() ) vecBlueTeam = mit->second;

	mit = mapUser.find( KGCMatchCompleteDlgS6::KMCD_TEAM_RED );
	if ( mit != mapUser.end() ) vecRedTeam = mit->second;

	int i = 2;

	std::vector<KPartyMember>::iterator vitBlue = vecBlueTeam.begin();
	for ( int i = 2 ; i > -1 ; --i ) {
		if ( vitBlue == vecBlueTeam.end() ) {
			m_saUserIcon[i]->ToggleRender(false);
			continue;
		}
		
		int iUserState = vitBlue->m_dwState == KPartyMember::US_READY ? KGCMatchUserIconS6::KMUI_CHECK_STATE_V : KGCMatchUserIconS6::KMUI_CHECK_STATE_NOT_RENDER;
		m_saUserIcon[i]->SetCheck(iUserState);
		m_saUserIcon[i]->SetIconAct(true);
		m_saUserIcon[i]->ToggleRender(true);
		++vitBlue;
	}

	std::vector<KPartyMember>::iterator vitRed = vecRedTeam.begin();
	for ( int i = 3 ; i < KGCMatchCompleteDlgS6::KMCD_USER_MAX ; ++i ) {
		if ( vitRed == vecRedTeam.end() ) {
			m_saUserIcon[i]->ToggleRender(false);
			continue;
		}

		int iUserState = vitRed->m_dwState == KPartyMember::US_READY ? KGCMatchUserIconS6::KMUI_CHECK_STATE_V : KGCMatchUserIconS6::KMUI_CHECK_STATE_NOT_RENDER;
		m_saUserIcon[i]->SetCheck(iUserState);
		m_saUserIcon[i]->SetIconAct(false);
		m_saUserIcon[i]->ToggleRender(true);
		++vitRed;
	}


}




IMPLEMENT_CLASSNAME( KGCMatchMapSelectBtnS6 );
IMPLEMENT_WND_FACTORY( KGCMatchMapSelectBtnS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCMatchMapSelectBtnS6, "gc_match_map_select_btn_s6" );

KGCMatchMapSelectBtnS6::KGCMatchMapSelectBtnS6( void )
: m_pkMapName(NULL)
, m_iMapID(0)
{
	LINK_CONTROL("static_room_name", m_pkMapName);
}

KGCMatchMapSelectBtnS6::~KGCMatchMapSelectBtnS6( void )
{
}

void KGCMatchMapSelectBtnS6::ActionPerformed( const KActionEvent& event )
{

}

void KGCMatchMapSelectBtnS6::OnCreate( void )
{
	m_pkMapName->InitState(true);
}

void KGCMatchMapSelectBtnS6::SetInfo( int iMapID )
{
	m_iMapID = iMapID;
	m_pkMapName->SetText(g_pkStrLoader->GetString( SiKGCMatchManager()->GetMatchMapNameID(m_iMapID)));
}


std::pair< int, bool > KGCMatchMapSelectBtnS6::GetBtnState( void ) 
{
	return std::make_pair(m_iMapID, IsLocked());
}


IMPLEMENT_CLASSNAME( KGCMatchMapSelectDlgS6 );
IMPLEMENT_WND_FACTORY( KGCMatchMapSelectDlgS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCMatchMapSelectDlgS6, "gc_auto_matching_map_s6" );

KGCMatchMapSelectDlgS6::KGCMatchMapSelectDlgS6( void )
: m_pkBackBox(NULL)
, m_pkInBackBox(NULL)
, m_pkCloseBtn(NULL)
, m_pkOKBtn(NULL)
{
	LINK_CONTROL("background",m_pkBackBox);
	LINK_CONTROL("in_background",m_pkInBackBox);
	LINK_CONTROL("close",m_pkCloseBtn);
	LINK_CONTROL("ok_btn",m_pkOKBtn);
	
}

KGCMatchMapSelectDlgS6::~KGCMatchMapSelectDlgS6( void )
{
}

void KGCMatchMapSelectDlgS6::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkCloseBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickCloseBtn );
	GCWND_MSG_MAP( m_pkOKBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickOKBtn );

	for ( std::vector<KGCMatchMapSelectBtnS6*>::iterator vit = m_vecMapBtn.begin() ; vit != m_vecMapBtn.end() ; ++vit ) {
		GCWND_WND_MAP( (*vit), KD3DWnd::D3DWE_BUTTON_CLICK, OnClickMapBtn );
	}
}


void KGCMatchMapSelectDlgS6::OnCreate( void )
{
	m_pkBackBox->InitState(true);
	m_pkInBackBox->InitState(true);
	m_pkCloseBtn->InitState(true,true,this);
	m_pkOKBtn->InitState(true,true,this);

	InitBtnList();
}

void KGCMatchMapSelectDlgS6::OnClickMapBtn( KD3DWnd* pWnd )
{	
	for ( std::vector<KGCMatchMapSelectBtnS6*>::iterator vit = m_vecMapBtn.begin() ; vit != m_vecMapBtn.end() ; ++vit ) {
		(*vit)->Lock(pWnd == (*vit));
	}	
}

void KGCMatchMapSelectDlgS6::OnClickCloseBtn( void )
{
	int iMapID = SiKGCMatchManager()->GetCurrentExceptMapID( );

	for ( std::vector<KGCMatchMapSelectBtnS6*>::iterator vit = m_vecMapBtn.begin() ; vit != m_vecMapBtn.end() ; ++vit ) {
		(*vit)->Lock( iMapID == (*vit)->GetBtnState().first );
	}

	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCMatchMapSelectDlgS6::OnClickOKBtn( void )
{
	for ( std::vector<KGCMatchMapSelectBtnS6*>::iterator vit = m_vecMapBtn.begin() ; vit != m_vecMapBtn.end() ; ++vit ) {
		if ((*vit)->GetBtnState().second) {
			SiKGCMatchManager()->SetCurrentExceptMapID( (*vit)->GetBtnState().first );
			break;
		}
	}

	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCMatchMapSelectDlgS6::SetWndSizeAndWndPos( int iBtnNum )
{
	if ( m_vecMapBtn.empty() == true ) return;

	int iWidthNum = iBtnNum / KMMS_MAX_HEIGHT_NUM;

	if ( iWidthNum < KMMS_MIN_WIDTH_NUM ) {
		iWidthNum = KMMS_MIN_WIDTH_NUM;
	}
	int iHeightNum = iBtnNum / iWidthNum;

	int iBtnWidth = m_vecMapBtn[0]->GetWidth();
	int iBtnHeight = m_vecMapBtn[0]->GetHeight();

	float fBaseBtnPosX = m_vecMapBtn[0]->GetFixedWindowLocalPos().x;
	float fBaseBtnPosY = m_vecMapBtn[0]->GetFixedWindowLocalPos().y;

	int iBtnWidthGap = 2;

	int iBackGroundWidth = 57 + ( iBtnWidth * iWidthNum ) + ( iBtnWidthGap * ( iWidthNum - 1) );
	int iBackGroundHeight = 164 + ( iBtnHeight * iHeightNum ) + ( iBtnWidthGap * ( iHeightNum - 1) );

	int iInBackGroundWidth = 23 + ( iBtnWidth * iWidthNum ) + ( iBtnWidthGap * ( iWidthNum - 1) );
	int iInBackGroundHeight = 68 + ( iBtnHeight * iHeightNum ) + ( iBtnWidthGap * ( iHeightNum - 1) );

	int iDiff = iWidthNum - KMMS_MIN_WIDTH_NUM;
	if ( iWidthNum > KMMS_MIN_WIDTH_NUM ) {
		iDiff = (( iBtnWidth * iDiff ) + ( iBtnWidthGap * ( iDiff - 1) )) / 2;
		SetWindowPosXDirect(GetFixedWindowLocalPos().x - static_cast<float>(iDiff));
	}

	float fOKBtnPosX = static_cast<float>( (( iBtnWidth * iDiff ) + ( iBtnWidthGap * ( iDiff - 1) )) / 2 );
	float fOKBtnPosY = static_cast<float>( ( iBtnHeight * ( iHeightNum - 1) ) + ( iBtnWidthGap * ( iHeightNum - 1) ));

	m_pkOKBtn->SetWindowPosDirect(D3DXVECTOR2( m_pkOKBtn->GetFixedWindowLocalPos().x + fOKBtnPosX,
		m_pkOKBtn->GetFixedWindowLocalPos().y + fOKBtnPosY));
	
	fOKBtnPosX = static_cast<float>( (( iBtnWidth * iDiff ) + ( iBtnWidthGap * ( iDiff - 1) )));
	m_pkCloseBtn->SetWindowPosXDirect( m_pkCloseBtn->GetFixedWindowLocalPos().x + fOKBtnPosX );



	m_pkBackBox->SetSize(iBackGroundWidth,iBackGroundHeight);
	m_pkInBackBox->SetSize(iInBackGroundWidth,iInBackGroundHeight);
	
	int iCount = 0;
	for ( std::vector<KGCMatchMapSelectBtnS6*>::iterator vit = m_vecMapBtn.begin() ; vit != m_vecMapBtn.end() ; ++vit, ++iCount ) {
		float fPosX = static_cast<float>( iBtnWidth * ( iCount % iWidthNum ) ) + ( iBtnWidthGap * ( iCount % iWidthNum )  );
		float fPosY = static_cast<float>( iBtnHeight * ( iCount / iWidthNum ) ) + ( iBtnWidthGap * ( iCount / iWidthNum )  );

		(*vit)->SetWindowPosDirect(D3DXVECTOR2(fBaseBtnPosX + fPosX, fBaseBtnPosY + fPosY));
	}
}

void KGCMatchMapSelectDlgS6::InitBtnList( void )
{
	if ( m_vecMapBtn.empty() == false ) return;

	std::set<EGCGameStage> setGameStage = SiKGCRoomManager()->GetGameModeListInChannel(SiKGCChannelManager()->GetCurrentChannelType());

	int iBtnNum = static_cast<int>(setGameStage.size()) + 1;

	std::vector<KD3DWnd*> &vecMapBtn =  g_pkUIMgr->CreateTemplateUIListByFindName(this, 
		"ui_scene_match_s6.stg", "","auto_matching_map_s6\\map_btn_check",true, iBtnNum );

	for ( std::vector<KD3DWnd*>::iterator vit = vecMapBtn.begin() ; vit != vecMapBtn.end() ; ++vit ) {
		m_vecMapBtn.push_back(static_cast<KGCMatchMapSelectBtnS6*>(*vit));
	}

	int i = 1;


	m_vecMapBtn[0]->InitState(true,true,this);
	m_vecMapBtn[0]->SetInfo(-1);
	for ( std::set<EGCGameStage>::iterator sit = setGameStage.begin() ; sit != setGameStage.end() ; ++sit, ++i ) {
		m_vecMapBtn[i]->InitState(true,true,this);
		m_vecMapBtn[i]->SetInfo(static_cast<int>(*sit));
	}
	
	SetWndSizeAndWndPos( iBtnNum + 1 );
}

IMPLEMENT_CLASSNAME( KGCMatchPartyViewS6 );
IMPLEMENT_WND_FACTORY( KGCMatchPartyViewS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCMatchPartyViewS6, "gc_auto_matching_party_view_s6" );

KGCMatchPartyViewS6::KGCMatchPartyViewS6( void )
: m_pkDesc(NULL)
{
	for ( int i = 0 ; i < KGCPartyManager::KPM_MATCH_PARTY_MAX_USER ; ++i ) {
		m_saPartyList[i] = NULL;
		char str[MAX_PATH] = {0,};
		sprintf( str, "user_icon_%d", i );
		LINK_CONTROL( str, m_saPartyList[i] );
	}

	LINK_CONTROL( "desc", m_pkDesc );
}

KGCMatchPartyViewS6::~KGCMatchPartyViewS6( void )
{
}

void KGCMatchPartyViewS6::ActionPerformed( const KActionEvent& event )
{
}

void KGCMatchPartyViewS6::OnCreate( void )
{
	m_pkDesc->InitState(true,false,NULL);
	for ( int i = 0 ; i < KGCPartyManager::KPM_MATCH_PARTY_MAX_USER ; ++i ) {
		m_saPartyList[i]->InitState(false);
	}
	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(), KGCUIScene::D3DWE_UPDATE_MATCH);
	g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_UPDATE_MATCH, this, &KGCMatchPartyViewS6::UpdateUserList );

}

void KGCMatchPartyViewS6::UpdateUserList( void )
{
	std::vector<KPartyMember> vecMemberList = SiKGCPartyManager()->GetPartyMemberList();


	if ( vecMemberList.empty() == true ) {
		for ( int i = 0 ; i < KGCPartyManager::KPM_MATCH_PARTY_MAX_USER ; ++i ) {
			m_saPartyList[i]->ToggleRender(false);
		}

		m_pkDesc->ToggleRender(true);
		return;
	}

	m_pkDesc->ToggleRender(false);
	
	for ( int i = 0 ; i < KGCPartyManager::KPM_MATCH_PARTY_MAX_USER ; ++i ) {
		m_saPartyList[i]->ToggleRender(true);
		if ( i >= static_cast<int>(vecMemberList.size()) ) {
			m_saPartyList[i]->SetIconAct(false);
			m_saPartyList[i]->SetUserNickName(g_pkStrLoader->GetString( STR_ID_AUTO_MATCH_INVITE_FRIEND ),0xff2e394b);
			m_saPartyList[i]->SetCheck(KGCMatchUserIconS6::KMUI_CHECK_STATE_NOT_RENDER);
			continue;
		}

		if ( SiKGCPartyManager()->IsPartyMasterUser(vecMemberList[i].m_dwUID) ) {
			m_saPartyList[i]->SetIconAct(true);
			m_saPartyList[i]->SetUserNickName(vecMemberList[i].m_strNickName, 0xfffec601);
			m_saPartyList[i]->SetCheck(KGCMatchUserIconS6::KMUI_CHECK_STATE_V);
		}
		else {
			m_saPartyList[i]->SetIconAct(true);
			m_saPartyList[i]->SetUserNickName(vecMemberList[i].m_strNickName);
			m_saPartyList[i]->SetCheck(vecMemberList[i].m_dwState == KPartyMember::US_NORMAL ? KGCMatchUserIconS6::KMUI_CHECK_STATE_X : KGCMatchUserIconS6::KMUI_CHECK_STATE_V );
		}
	}


}

IMPLEMENT_CLASSNAME( KGCMatchStartPartS6 );
IMPLEMENT_WND_FACTORY( KGCMatchStartPartS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCMatchStartPartS6, "gc_auto_matching_starter_s6" );

KGCMatchStartPartS6::KGCMatchStartPartS6( void )
: m_pkMatchingBtn(NULL)
, m_pkLeavePartyBtn(NULL)
, m_pkDescUser(NULL)
, m_pkPartyUser(NULL)
, m_pkReadyBtn(NULL)
, m_pkCancelBtn(NULL)
, m_tmInfoPacketReq(0)
{
	LINK_CONTROL("matching_btn",m_pkMatchingBtn);
	LINK_CONTROL("leave_party_btn",m_pkLeavePartyBtn);
	LINK_CONTROL("ready_btn",m_pkReadyBtn);
	LINK_CONTROL("cancel_btn",m_pkCancelBtn);
	

	LINK_CONTROL("desc_user",m_pkDescUser);
	LINK_CONTROL("auto_matching_party_view_s6",m_pkPartyUser);
	


	for ( int i = 0 ; i < KGCMatchManager::MMATCHMODE_MAX ; ++i ) {
		m_saModeBtnList[i] = NULL;
		char str[MAX_PATH] = {0,};

		sprintf( str, "mode%d_btn", i+1 );
		LINK_CONTROL( str, m_saModeBtnList[i] );
	}

	for ( int i = 0 ; i < KGCMatchManager::MMATCHMODE_MAX ; ++i ) {
		m_saModeActList[i] = NULL;
		char str[MAX_PATH] = {0,};

		sprintf( str, "mode%d_act", i+1 );
		LINK_CONTROL( str, m_saModeActList[i] );
	}

	for ( int i = 0 ; i < KGCMatchManager::MMATCHMODE_MAX ; ++i ) {
		m_saDescMode[i] = NULL;
		char str[MAX_PATH] = {0,};

		sprintf( str, "desc_mode_%d", i );
		LINK_CONTROL( str, m_saDescMode[i] );
	}

	for ( int i = 0 ; i < KGCMatchManager::MMATCHMODE_MAX ; ++i ) {
		m_saDescModeBack[i] = NULL;
		char str[MAX_PATH] = {0,};

		sprintf( str, "desc_mode_back_%d", i );
		LINK_CONTROL( str, m_saDescModeBack[i] );
	}


	

}

KGCMatchStartPartS6::~KGCMatchStartPartS6( void )
{
}

void KGCMatchStartPartS6::ActionPerformed( const KActionEvent& event )
{
	for ( int i = 0 ; i < KGCMatchManager::MMATCHMODE_MAX ; ++i ) {
		if ( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK && m_saModeActList[i] == event.m_pWnd ) {
			OnClickMode(i);
			OnClickMatchingBtn();
			return;

		}
		if ( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK && m_saModeBtnList[i] == event.m_pWnd ) {
			OnClickMode(i);
			return;
		}
	}



	GCWND_MSG_MAP( m_pkMatchingBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickMatchingBtn );
	GCWND_MSG_MAP( m_pkLeavePartyBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickLeaveParty );
	GCWND_MSG_MAP( m_pkReadyBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickReadyBtn );
	GCWND_MSG_MAP( m_pkCancelBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickCancelBtn );

}

void KGCMatchStartPartS6::OnCreate( void )
{
	m_pkMatchingBtn->InitState(true,true,this);
	m_pkLeavePartyBtn->InitState(false,true,this);
	m_pkReadyBtn->InitState(false,true,this);
	m_pkCancelBtn->InitState(false,true,this);

	//m_pkDescMode->InitState(true);
	m_pkDescUser->InitState(false);
	m_pkPartyUser->InitState(true);

	for ( int i = 0 ; i < KGCMatchStartPartS6::KMR_MAX_MODE ; ++i ) {
#if defined (NATION_KOREA)
        if ( i == 1 ) { // 2vs2모드를 제거 함.
			m_saModeBtnList[i]->InitState(false);
			m_saModeActList[i]->InitState(false);
			m_saDescMode[i]->InitState(false);
			m_saDescModeBack[i]->InitState(false);
			continue;
		}
#endif
		m_saModeBtnList[i]->InitState(true,true,this);
		m_saModeActList[i]->InitState(false,true,this);
		m_saDescMode[i]->InitState(true);
		m_saDescModeBack[i]->InitState(true);
	}



	if ( m_mapDescWndUV.empty() ) {
		for ( int i = 0 ; i < KGCMatchStartPartS6::KMR_MAX_MODE ; ++i ) {
			m_mapDescWndUV[i] = m_saDescMode[i]->GetWndTexCoords();
		}
	}
}

void KGCMatchStartPartS6::OnClickReadyBtn( void )
{
	Result_MatchPartyReady = INT_MAX;
	KP2P::GetInstance()->Send_PartyUserReadyReq( KPartyMember::US_READY, static_cast<char>( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType ));
	g_MyD3D->WaitForServerAck( Result_MatchPartyReady, -1, 3000, INT_MAX );
}

void KGCMatchStartPartS6::OnClickCancelBtn( void )
{
	Result_MatchPartyReady = INT_MAX;
	KP2P::GetInstance()->Send_PartyUserReadyReq( KPartyMember::US_NORMAL, static_cast<char>( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType ));
	g_MyD3D->WaitForServerAck( Result_MatchPartyReady, -1, 3000, INT_MAX );
}


void KGCMatchStartPartS6::OnClickLeaveParty( void )
{
	SiKGCPartyManager()->LeaveParty();	
}

void KGCMatchStartPartS6::OnClickMode( int iMode )
{
	SiKGCMatchManager()->SetCurrentMode(iMode);
	UpdateBtnState();
}

void KGCMatchStartPartS6::OnClickMatchingBtn( )
{
    if ( SiKGCMatchManager()->IsAutoMatchOpenTime() == false ) {

        KMatchOpenTime kOpenTime;
        if ( SiKGCMatchManager()->GetCurrentOpenTime(kOpenTime) == false )
            return;

        std::wstring strStart = (kOpenTime.m_prBeginTime.first / 12) > 0 ? 
            g_pkStrLoader->GetString( STR_ID_MATCH_INFO_12 ) : g_pkStrLoader->GetString( STR_ID_MATCH_INFO_11 );
        std::wstring strEnd = (kOpenTime.m_prEndTime.first / 12) > 0 ? 
            g_pkStrLoader->GetString( STR_ID_MATCH_INFO_12 ) : g_pkStrLoader->GetString( STR_ID_MATCH_INFO_11 );

        int iStartH = (kOpenTime.m_prBeginTime.first % 12 == 0) ? 12 : (kOpenTime.m_prBeginTime.first % 12);
        int iEndH = (kOpenTime.m_prEndTime.first % 12 == 0) ? 12 : (kOpenTime.m_prEndTime.first % 12);

        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetReplacedString(STR_ID_MATCH_INFO_4, "liilii", strStart, iStartH, kOpenTime.m_prBeginTime.second, strEnd, iEndH, kOpenTime.m_prEndTime.second ) );
        return;
    }

	if ( SiKGCPartyManager()->IsParty() && SiKGCPartyManager()->IsMatchReady(true) == false ) {
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_AUTO_MATCH_INVITE_FRIEND_LIST_ERR2 ));
		return;
	}

	Result_AddMatchReq = INT_MAX;
	
	KP2P::GetInstance()->Send_AddMatchReq( SiKGCMatchManager()->GetCurrentMode(), 
		static_cast<char>( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType ), 
		SiKGCMatchManager()->GetCurrentExceptMapID() );

	g_MyD3D->WaitForServerAck( Result_AddMatchReq, -1, 3000, INT_MAX );


}

void KGCMatchStartPartS6::UpdateState( void )
{
	int iUserState = SiKGCMatchManager()->GetCurrentUserState();

	switch ( iUserState ) {
		case KGCMatchManager::MUSERSTATE_NONE :
			SetUserStateNone();
			break;
		case KGCMatchManager::MUSERSTATE_PARTY_USER :
			SetUserStatePartyUser();
			break;
		case KGCMatchManager::MUSERSTATE_PARTY_MASTER :
			SetUserStatePartyMaster();
			break;
	}
}

void KGCMatchStartPartS6::SetUserStateNone( void )
{
	m_pkMatchingBtn->ToggleRender(true);
	m_pkMatchingBtn->SetWindowPosXDirect( 242.f );

	m_pkLeavePartyBtn->ToggleRender(false);

	for ( int i = 0 ; i < KGCMatchStartPartS6::KMR_MAX_MODE ; ++i ) {
#if defined (NATION_KOREA)
        if ( i == 1 ) { // 2vs2모드를 제거 함.
			m_saDescMode[i]->ToggleRender(false);
			m_saDescModeBack[i]->ToggleRender(false);
			continue;
		}
#endif

		m_saDescMode[i]->ToggleRender(true);
		m_saDescModeBack[i]->ToggleRender(true);
		SetModeStateTex(i, m_saDescMode[i]);
	}

	m_pkDescUser->ToggleRender(false);
	m_pkReadyBtn->ToggleRender(false);
	m_pkCancelBtn->ToggleRender(false);

	UpdateBtnState();
}

void KGCMatchStartPartS6::SetModeStateTex( int index, KD3DWnd *pWnd )
{
	if( pWnd == NULL ) return;

	if ( index > 2 || index < 0 ) {
		return;
	}

	int iState = 0;

	if ( SiKGCMatchManager()->GetMatchModeState( index, iState ) == false )
		return;

	if (m_mapDescWndUV.find(iState) == m_mapDescWndUV.end()) return;

	//ChangeUVtoTexel(m_mapDescWndUV[iState]);

	pWnd->SetWndTexCoords(D3DWM_DEFAULT,m_mapDescWndUV[iState]);
}

void KGCMatchStartPartS6::SetUserStatePartyUser( void )
{
	m_pkMatchingBtn->ToggleRender(false);
	m_pkLeavePartyBtn->ToggleRender(true);
	m_pkLeavePartyBtn->SetWindowPosXDirect( 319.0f );
	
	std::vector<KPartyMember> vecMemberList = SiKGCPartyManager()->GetPartyMemberList();

	for ( std::vector<KPartyMember>::iterator vit = vecMemberList.begin() ; vit != vecMemberList.end() ; ++vit ) {
		if ( vit->m_dwUID == g_kGlobalValue.m_kUserInfo.dwUID ) {
			m_pkReadyBtn->ToggleRender(vit->m_dwState == KPartyMember::US_NORMAL);
			m_pkCancelBtn->ToggleRender(vit->m_dwState == KPartyMember::US_READY);
			break;
		}
	}

	for ( int i = 0 ; i < KGCMatchStartPartS6::KMR_MAX_MODE ; ++i ) {
		m_saDescMode[i]->ToggleRender(false);
		m_saDescModeBack[i]->ToggleRender(false);
	}
	m_pkDescUser->ToggleRender(true);

	UpdateBtnState();
}

void KGCMatchStartPartS6::SetUserStatePartyMaster( void )
{
	m_pkMatchingBtn->ToggleRender(true);
	m_pkMatchingBtn->SetWindowPosXDirect( 163.0f );

	m_pkLeavePartyBtn->ToggleRender(true);

	for ( int i = 0 ; i < KGCMatchStartPartS6::KMR_MAX_MODE ; ++i ) {
#if defined (NATION_KOREA)
        if ( i == 1 ) { // 2vs2모드를 제거 함.
			m_saDescMode[i]->ToggleRender(false);
			m_saDescModeBack[i]->ToggleRender(false);
			continue;
		}
#endif
		m_saDescMode[i]->ToggleRender(true);
		m_saDescModeBack[i]->ToggleRender(true);
		SetModeStateTex(i, m_saDescMode[i]);
	}
	m_pkDescUser->ToggleRender(false);
	m_pkReadyBtn->ToggleRender(false);
	m_pkCancelBtn->ToggleRender(false);

	UpdateBtnState();
}

void KGCMatchStartPartS6::UpdateBtnState( void )
{
	int iCurMode = SiKGCMatchManager()->GetCurrentMode();

	for ( int i = 0 ; i < KGCMatchManager::MMATCHMODE_MAX ; ++i ) {
#if defined (NATION_KOREA)
        if ( i == 1 ) { // 2vs2모드를 제거 함.
			m_saModeBtnList[i]->ToggleRender(false);
			m_saModeActList[i]->ToggleRender(false);
			continue;
		}
#endif
		m_saModeBtnList[i]->ToggleRender( iCurMode != i );
		m_saModeActList[i]->ToggleRender( iCurMode == i );
		m_saModeBtnList[i]->Lock(SiKGCMatchManager()->IsModePlayable(i+1) == false );
	}

	if ( SiKGCPartyManager()->IsParty() && SiKGCPartyManager()->IsLocalPartyMasterUser() == false ) {
		for ( int i = 0 ; i < KGCMatchManager::MMATCHMODE_MAX ; ++i ) {
#if defined (NATION_KOREA)
            if ( i == 1 ) { // 2vs2모드를 제거 함.
				m_saModeBtnList[i]->ToggleRender(false);
				m_saModeActList[i]->ToggleRender(false);
				continue;
			}
#endif
			m_saModeBtnList[i]->ToggleRender( true );
			m_saModeActList[i]->ToggleRender( false );
			m_saModeBtnList[i]->Lock( true );
		}
	}
}

void KGCMatchStartPartS6::FrameMoveInEnabledState( void )
{
	if ( SiKGCPartyManager()->IsParty() && SiKGCPartyManager()->IsLocalPartyMasterUser() == false ) return;

	time_t curTime = GCUTIL_TIME::GetCurTime();
	if ( curTime > m_tmInfoPacketReq ) {
		KP2P::GetInstance()->Send_MatchModeStateReq();
		m_tmInfoPacketReq = curTime + 10;
	}
}

IMPLEMENT_CLASSNAME( KGCMatchWaitPartS6 );
IMPLEMENT_WND_FACTORY( KGCMatchWaitPartS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCMatchWaitPartS6, "gc_auto_matching_standby_s6" );

KGCMatchWaitPartS6::KGCMatchWaitPartS6( void )
: m_pkCancelBtn(NULL)
, m_tmCurTime(0)
, m_pkWaitTime(NULL)
, m_pkCalcWaitTime(NULL)
, m_dwAlpha(0)
, m_pkOverWaitTime(NULL)
, m_pkOvertimeDesc(NULL)
{
	LINK_CONTROL("cancel_btn",m_pkCancelBtn);
	LINK_CONTROL("wait_time",m_pkWaitTime);
	LINK_CONTROL("calc_wait_time",m_pkCalcWaitTime);

	for ( int i = 0 ; i < KGCMatchWaitPartS6::KMWTIME_MAX_WND ; ++i ) {
		m_saTime[i] = NULL;
	}

	for ( int i = 0 ; i < KGCMatchWaitPartS6::KMWTIMEBACK_MAX_WND ; ++i ) {
		char str[MAX_PATH];
		m_saTimeBack[i] = NULL;
		sprintf( str, "back%d", i );
		LINK_CONTROL( str, m_saTimeBack[i] );
	}

	for ( int i = 0 ; i < KGCMatchWaitPartS6::KMWTIMEDIV_MAX_WND ; ++i ) {
		char str[MAX_PATH];
		m_saTimeDiv[i] = NULL;
		sprintf( str, "h%d", i );
		LINK_CONTROL( str, m_saTimeDiv[i] );
	}

	

	LINK_CONTROL( "time_s0x", m_saTime[0] );
	LINK_CONTROL( "time_sx0", m_saTime[1] );
	LINK_CONTROL( "time_m0x", m_saTime[2] );
	LINK_CONTROL( "time_mx0", m_saTime[3] );
	LINK_CONTROL( "time_h0x", m_saTime[4] );
	LINK_CONTROL( "time_hx0", m_saTime[5] );

	LINK_CONTROL( "serch_wait_time", m_pkOverWaitTime );
	LINK_CONTROL( "over_time_static", m_pkOvertimeDesc );
	

}

KGCMatchWaitPartS6::~KGCMatchWaitPartS6( void )
{
}

void KGCMatchWaitPartS6::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkCancelBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickCancelBtn );
}

void KGCMatchWaitPartS6::OnCreate( void )
{
	m_pkCancelBtn->InitState(true,true,this);
	m_pkWaitTime->InitState(true);
	m_pkCalcWaitTime->InitState(false);
	m_pkOverWaitTime->InitState(false);
	m_pkOvertimeDesc->InitState(false);
	m_pkOvertimeDesc->SetTextAutoMultiline(g_pkStrLoader->GetString( STR_ID_AUTO_MATCH_OVER_TIME_DESC ));
	m_pkOvertimeDesc->SetAlign( DT_CENTER );

	for ( int i = 0 ; i < KGCMatchWaitPartS6::KMWTIME_MAX_WND ; ++i ) {
		m_saTime[i]->InitState(true);
	}

	for ( int i = 0 ; i < KGCMatchWaitPartS6::KMWTIMEBACK_MAX_WND ; ++i ) {
		m_saTimeBack[i]->InitState(true);
	}

	for ( int i = 0 ; i < KGCMatchWaitPartS6::KMWTIMEDIV_MAX_WND ; ++i ) {
		m_saTimeDiv[i]->InitState(true);
	}

	


}

void KGCMatchWaitPartS6::OnClickCancelBtn( void )
{
	Result_AddMatchReq = INT_MAX;
	KP2P::GetInstance()->Send_DelMatchReq();
	g_MyD3D->WaitForServerAck( Result_AddMatchReq, -1, 3000, INT_MAX );
}

void KGCMatchWaitPartS6::SetTimeRender( bool bRender )
{
	for ( int i = 0 ; i < KGCMatchWaitPartS6::KMWTIME_MAX_WND ; ++i ) {
		m_saTime[i]->ToggleRender(bRender);
	}

	for ( int i = 0 ; i < KGCMatchWaitPartS6::KMWTIMEBACK_MAX_WND ; ++i ) {
		m_saTimeBack[i]->ToggleRender(bRender);
	}

	for ( int i = 0 ; i < KGCMatchWaitPartS6::KMWTIMEDIV_MAX_WND ; ++i ) {
		m_saTimeDiv[i]->ToggleRender(bRender);
	}



}

void KGCMatchWaitPartS6::FrameMoveInEnabledState( void )
{
	time_t curTime = GCUTIL_TIME::GetCurTime();

	if (  curTime == m_tmCurTime && m_pkCalcWaitTime->IsRenderOn() ) { 
		m_dwAlpha += 5;
		if ( m_dwAlpha > 255 )
			m_dwAlpha = 0;

		m_pkCalcWaitTime->SetAllWindowAlpha( 255 - m_dwAlpha );
		return; 
	}
	else if ( m_pkCalcWaitTime->IsRenderOn()  ) {
		m_pkWaitTime->ToggleRender(true);
		m_pkCalcWaitTime->ToggleRender(false);
	}

	m_tmCurTime = curTime;

	time_t tmWaitTime = SiKGCMatchManager()->GetWaitMachingTime();

	tmWaitTime = tmWaitTime - m_tmCurTime;

	if ( tmWaitTime > 600 ) {
		if ( m_pkOverWaitTime->IsRenderOn() == false ) {
			m_pkOverWaitTime->ToggleRender(true);
			m_pkWaitTime->ToggleRender(false);
			SetTimeRender(false);
			m_pkOvertimeDesc->ToggleRender(true);
			m_dwAlpha =0;
		}
		else {
			m_dwAlpha += 5;
			if ( m_dwAlpha > 255 )
				m_dwAlpha = 0;

			m_pkOverWaitTime->SetAllWindowAlpha( 255 - m_dwAlpha );
		}
		return;
	}
	else if ( m_pkOverWaitTime->IsRenderOn() ) {
		m_pkOverWaitTime->ToggleRender(false);
		SetTimeRender(true);
		m_pkWaitTime->ToggleRender(true);
		m_pkOverWaitTime->ToggleRender(false);
		m_pkOvertimeDesc->ToggleRender(false);

	}

	DWORD dwSecond = tmWaitTime % 60;
	DWORD dwMinute = ( tmWaitTime / 60 ) % 60;
	DWORD dwHour = ( ( tmWaitTime / 60 ) / 60 );

	if ( dwHour >= 100 )
		dwHour = 99;

	SetNumberTex( dwSecond % 10 ,m_saTime[0]);
	SetNumberTex( dwSecond / 10 ,m_saTime[1]);
	SetNumberTex( dwMinute % 10 ,m_saTime[2]);
	SetNumberTex( dwMinute / 10 ,m_saTime[3]);
	SetNumberTex( dwHour % 10 ,m_saTime[4]);
	SetNumberTex( dwHour / 10 ,m_saTime[5]);

	if ( tmWaitTime == 0 && m_pkCalcWaitTime->IsRenderOn() == false )	{
		m_pkWaitTime->ToggleRender(false);
		m_pkCalcWaitTime->ToggleRender(true);
		m_dwAlpha =0;
	}

}

void KGCMatchWaitPartS6::SetNumberTex( int index, KD3DWnd *pWnd )
{
	if(pWnd == NULL) return;

	int x_index = index % 10 ; 

	float tu = static_cast<float>(x_index) * 39.f + 2.f;
	float tv = 466.f;
	float tu1 = static_cast<float>(x_index) * 39.f + 38.f ;
	float tv1 = 510.f;
	D3DXVECTOR4 vec(tu,tv,tu1,tv1);
	ChangeUVtoTexel(vec);

	pWnd->SetWndTexCoords(D3DWM_DEFAULT,vec);
}


void KGCMatchWaitPartS6::ChangeUVtoTexel( D3DXVECTOR4& vUV )
{
	float fMagicNumber=0.5f;
	if (vUV.x>vUV.z)
	{
		fMagicNumber*=-1.0f;
	}
	vUV.x = (vUV.x+fMagicNumber ) / 512.0f;
	vUV.z = (vUV.z-fMagicNumber ) / 512.0f;

	fMagicNumber=0.5f;
	if (vUV.y>vUV.w)
	{
		fMagicNumber*=-1.0f;
	}
	vUV.y = (vUV.y+fMagicNumber ) / 512.0f;
	vUV.w = (vUV.w-fMagicNumber ) / 512.0f;

	TEXEL_BOUND(vUV.x);
	TEXEL_BOUND(vUV.y);
	TEXEL_BOUND(vUV.z);
	TEXEL_BOUND(vUV.w);
}

IMPLEMENT_CLASSNAME( KGCAutoMatchingS6 );
IMPLEMENT_WND_FACTORY( KGCAutoMatchingS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCAutoMatchingS6, "gc_auto_matching_s6" );

KGCAutoMatchingS6::KGCAutoMatchingS6( void )
: m_pkInvitePartyBtn(NULL)
, m_pkMapSelectBtn(NULL)
, m_pkStartScene(NULL)
, m_pkWaitScene(NULL)
, m_pkOptionStart(NULL)
, m_pkOpenTime(NULL)
, m_pkOpenTimeStatic(NULL)
{
	LINK_CONTROL( "party_invite_btn", m_pkInvitePartyBtn );
	LINK_CONTROL( "map_sel_btn", m_pkMapSelectBtn );
	LINK_CONTROL( "auto_matching_starter_s6", m_pkStartScene );
	LINK_CONTROL( "auto_matching_standby_s6", m_pkWaitScene );
	LINK_CONTROL( "checkbox_option_start", m_pkOptionStart );
	LINK_CONTROL( "open_time ", m_pkOpenTime);
	LINK_CONTROL( "open_time_static", m_pkOpenTimeStatic);
	
}

KGCAutoMatchingS6::~KGCAutoMatchingS6( void )
{
}

void KGCAutoMatchingS6::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkMapSelectBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickMapSelBtn );
	GCWND_MSG_MAP( m_pkInvitePartyBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickInviteUserBtn );
	GCWND_MSG_MAP( m_pkOptionStart, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickOption );

}

void KGCAutoMatchingS6::OnCreate( void )
{
	m_pkInvitePartyBtn->InitState(true,true,this);
	m_pkMapSelectBtn->InitState(true,true,this);
	m_pkStartScene->InitState(true,true,this);
	m_pkWaitScene->InitState(false,true,this);
	m_pkOptionStart->InitState(true,true,this);
	m_pkOpenTime->InitState(true);
	m_pkOpenTime->SetAlign(DT_CENTER);
	m_pkOpenTimeStatic->InitState(true);
	m_pkOpenTimeStatic->SetAlign(DT_CENTER);
	m_pkOpenTimeStatic->SetText(g_pkStrLoader->GetString( STR_ID_MATCH_INFO_10 ));
	m_pkOptionStart->SetText(g_pkStrLoader->GetString( STR_ID_AUTO_MATCH_OPTION ));
	m_pkOptionStart->SetCheck(SiKGCMatchManager()->GetStartOption());

	SiKGCMatchManager()->SetCurrentState(KGCMatchManager::MSTATE_NONE);
	UpdateState();

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(), KGCUIScene::D3DWE_UPDATE_MATCH);
	g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_UPDATE_MATCH, this, &KGCAutoMatchingS6::UpdateState );

}

void KGCAutoMatchingS6::UpdateState( void )
{
	m_pkOptionStart->SetText(g_pkStrLoader->GetString( STR_ID_AUTO_MATCH_OPTION ));
	m_pkOptionStart->SetCheck(SiKGCMatchManager()->GetStartOption());
	UpdateTime();

	int iState = SiKGCMatchManager()->GetCurrentState();

	switch ( iState ) {
		case KGCMatchManager::MSTATE_NONE :
		case KGCMatchManager::MSTATE_READY_STATE :
			UpdateReadyState();
			break;
		case KGCMatchManager::MSTATE_WAIT_STATE :
			UpdateWaitState();
			break;
		case KGCMatchManager::MSTATE_FIND_USER_STATE :
			break;
		case KGCMatchManager::MSTATE_COMPLETE_STATE :
			break;
	}
}

void KGCAutoMatchingS6::UpdateTime( void )
{

	KMatchOpenTime kOpenTime;
	if ( SiKGCMatchManager()->GetCurrentOpenTime(kOpenTime) == false )
		return;

	std::wstring strStart = (kOpenTime.m_prBeginTime.first / 12) > 0 ? 
		g_pkStrLoader->GetString( STR_ID_MATCH_INFO_12 ) : g_pkStrLoader->GetString( STR_ID_MATCH_INFO_11 );
	std::wstring strEnd = (kOpenTime.m_prEndTime.first / 12) > 0 ? 
		g_pkStrLoader->GetString( STR_ID_MATCH_INFO_12 ) : g_pkStrLoader->GetString( STR_ID_MATCH_INFO_11 );

	int iStartH = (kOpenTime.m_prBeginTime.first % 12 == 0) ? 12 : (kOpenTime.m_prBeginTime.first % 12);
	int iEndH = (kOpenTime.m_prEndTime.first % 12 == 0) ? 12 : (kOpenTime.m_prEndTime.first % 12);

	m_pkOpenTime->SetText(g_pkStrLoader->GetReplacedString( STR_ID_MATCH_INFO_13,"liilii",
		strStart, iStartH, kOpenTime.m_prBeginTime.second,
		strEnd, iEndH, kOpenTime.m_prEndTime.second));
}


void KGCAutoMatchingS6::UpdateReadyState( void )
{
	m_pkWaitScene->ToggleRender(false);

	m_pkStartScene->ToggleRender(true);

	m_pkInvitePartyBtn->ToggleRender(true);
	m_pkMapSelectBtn->ToggleRender(true);

	m_pkStartScene->UpdateState();
}

void KGCAutoMatchingS6::UpdateWaitState( void )
{
	m_pkStartScene->ToggleRender(false);
	m_pkWaitScene->ToggleRender(true);
	m_pkInvitePartyBtn->ToggleRender(false);
	m_pkMapSelectBtn->ToggleRender(false);
	SiKGCMatchManager()->SetWaitMachingTime();

}

void KGCAutoMatchingS6::OnClickOption( void )
{
	m_pkOptionStart->SetCheck(!SiKGCMatchManager()->GetStartOption());
	SiKGCMatchManager()->SetStartOption(m_pkOptionStart->GetCheck());
}

void KGCAutoMatchingS6::OnClickMapSelBtn( void )
{
	if ( SiKGCPartyManager()->IsLocalPartyMasterUser() == false && SiKGCPartyManager()->IsParty() ) {
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_AUTO_MATCH_INVITE_FRIEND_LIST_ERR1 ));
		return;
	}

	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_MATCH_MAP_SELECT_BOX );
}

void KGCAutoMatchingS6::OnClickInviteUserBtn( void )
{
	if ( SiKGCPartyManager()->IsLocalPartyMasterUser() == false && SiKGCPartyManager()->IsParty() ) {
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_AUTO_MATCH_INVITE_FRIEND_LIST_ERR1 ));
		return;
	}

	Result_MatchPartyUserList = INT_MAX;
	KP2P::GetInstance()->Send_MatchInviteFriendListReq();
	if( false == g_MyD3D->WaitForServerAck( Result_MatchPartyUserList, -1, 3000, INT_MAX ) ) {
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_AUTO_MATCH_INVITE_FRIEND_LIST_ERR0 ));
	}
}