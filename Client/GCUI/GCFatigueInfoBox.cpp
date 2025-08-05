#include "stdafx.h"
#include "GCFatigueInfoBox.h"

ImplementSingleton( KGCFatigueManager )

KGCFatigueManager::KGCFatigueManager( void )
: m_iOnlineTime(0)
, m_iOfflineTime(0)
, m_tmConnectServerTime(0)
, m_bInitData(false)
, m_iAuth(0)
, m_iOldAlarmTime(0)
, m_iResetOfflineTime(1)
, m_iMaxOnlineTime(1)
, m_iGameRatio(0)
, m_bAlarm(false)
, m_iCurrentGameFatigueType(0)
{
	//int m_iOnlineTime;
	//int m_iOfflineTime;
	//time_t m_tmConnectServerTime;
	//std::map<int, KFatigueTable> m_mapFatigueSystemInfo;
	LoadLuaTable();
	m_iAuth = -1;
}

KGCFatigueManager::~KGCFatigueManager( void )
{

}

void KGCFatigueManager::InitFatigueSystem( KEVENT_ACCUMULRATION_TIME_NOT &kFatigueData )
{
	m_mapFatigueSystemInfo = kFatigueData.m_mapFatigueTable;
	//m_iOnlineTime = kFatigueData.m_nAccOnlineTime;
	//m_iOldAlarmTime = kFatigueData.m_nAccOnlineTime;
	m_iOfflineTime = 0;
	if ( m_bInitData == false )
		m_tmConnectServerTime = g_kGlobalValue.TimeToInt(g_kGlobalValue.m_tmServerTime);
	m_iMaxOnlineTime = kFatigueData.m_nMaxOnlineTime;
	//m_iResetOfflineTime = kFatigueData.m_nResetOfflineTime;


	//if (m_iMaxOnlineTime < m_iOnlineTime)
	//	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_FATIGUE_SYSTEM_MESSAGE, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
	//if ( m_iAccOnlineTime >= 180 &&  m_bInitData == false ) {
	//	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_FATIGUE_SYSTEM_MESSAGE, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
	//}
	m_bInitData = true;

}


void KGCFatigueManager::_GetFatigueSystemTime(OUT int &iOnlineTime, OUT int &iOfflineTime )
{
	time_t tmDiff = g_kGlobalValue.TimeToInt(g_kGlobalValue.m_tmServerTime);
	tmDiff = (tmDiff - m_tmConnectServerTime)/60;

	if ( m_iAccOnlineTime + tmDiff >= 300 )
		iOnlineTime = tmDiff;
	else
		iOnlineTime = m_iAccOnlineTime + tmDiff;
	iOfflineTime = m_iOfflineTime;
}

int KGCFatigueManager::GetCurrentFatigueSystemType()
{
	int iOnlineTime = 0;
	int iOfflineTime = 0;

	time_t tmDiff = g_kGlobalValue.TimeToInt(g_kGlobalValue.m_tmServerTime);
	tmDiff = (tmDiff - m_tmConnectServerTime)/60;

	iOnlineTime = m_iAccOnlineTime + tmDiff;
	iOfflineTime = m_iOfflineTime;

	for ( std::map<int, KFatigueTable>::iterator mit = m_mapFatigueSystemInfo.begin() ; mit != m_mapFatigueSystemInfo.end() ; ++mit ) {
		if ( mit->second.m_nBeginMin <= iOnlineTime && (mit->second.m_nEndMin >= iOnlineTime || mit->second.m_nEndMin == -1 ) ) {
			if ( mit->second.m_nType == KEVENT_ACCUMULRATION_TIME_NOT::YELLOW_TIME && mit->second.m_nBeginMin == iOnlineTime )
				continue;

			if ( mit->second.m_nType == KEVENT_ACCUMULRATION_TIME_NOT::YELLOW_TIME && mit->second.m_nEndMin == iOnlineTime )
				continue;

			return mit->first;
		}
	}
	return 0;
}

SFatigueData KGCFatigueManager::GetCurrentInfo()
{
	SFatigueData sTemp;
	if ( !m_bInitData )
		return sTemp;

	int iTimeType = GetCurrentFatigueSystemType();
	_GetFatigueSystemTime(sTemp.iOnline, sTemp.iOffline);
	std::map<int, KFatigueTable>::iterator mit = m_mapFatigueSystemInfo.find(iTimeType);
	int iPeriod = 1;
	if ( mit != m_mapFatigueSystemInfo.end() ){
		sTemp.iRatio = mit->second.m_nRatio;
		iPeriod = ( mit->second.m_nPeriod <= 0 ) ? 1 : mit->second.m_nPeriod;
	}
	else
		return sTemp;

	if ( sTemp.iOnline%iPeriod == 0 && m_iOldAlarmTime != sTemp.iOnline && sTemp.iOnline != 0 ) {
		m_bAlarm = true;
		m_iOldAlarmTime = sTemp.iOnline;
	}

	sTemp.iMaxOnlineTime = m_iMaxOnlineTime;
	sTemp.iResetOfflineTime = m_iResetOfflineTime;
	return sTemp;
}

bool KGCFatigueManager::IsFatigueSystemApplyUser()
{
#ifndef NATION_CHINA
	return false;
#endif
	return m_iAuth != FUT_AUTH_ADULT && m_iAuth != -1;
}

void KGCFatigueManager::LoadLuaTable()
{
	KLuaManager luaMgr;

	if( GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) == false )
		return;

	if( GCFUNC::LoadLuaScript( luaMgr, "GCFatigueInfo.lua" ) == false )
		return;

	m_setViewState.clear();

	LUA_BEGIN_TABLE("GCFatigue", return; ){
		for ( int i = 1 ; ; i++ )
		{
			int iTemp = 0;
			LUA_GET_VALUE_NOASSERT(i,iTemp, break;);
			m_setViewState.insert(iTemp);
		}
	}LUA_END_TABLE(;);
}

bool KGCFatigueManager::IsViewFatigueInfoScene()
{
	int iState = static_cast<int>(g_MyD3D->m_pStateMachine->GetState());

	return m_setViewState.find(iState) != m_setViewState.end();
}

IMPLEMENT_CLASSNAME( KGCFatigueInfoBox );
IMPLEMENT_WND_FACTORY( KGCFatigueInfoBox );
IMPLEMENT_WND_FACTORY_NAME( KGCFatigueInfoBox, "gc_fatigue_info_box" );

KGCFatigueInfoBox::KGCFatigueInfoBox( void )
: m_pkOnlineProgressBar(NULL)
, m_pkStaticOnline(NULL)
, m_pkOfflineProgressBar(NULL)
, m_pkStaticOffline(NULL)
, m_iOriginalOnlinePBarWidth(0)
, m_iOriginalOfflinePBarWidth(0)
{
	LINK_CONTROL("progress_online",m_pkOnlineProgressBar);
	LINK_CONTROL("static_online",m_pkStaticOnline);
	LINK_CONTROL("progress_offline",m_pkOfflineProgressBar);
	LINK_CONTROL("static_offline",m_pkStaticOffline);

}

KGCFatigueInfoBox::~KGCFatigueInfoBox( void )
{
}

void KGCFatigueInfoBox::ActionPerformed( const KActionEvent& event )
{

}

void KGCFatigueInfoBox::OnCreate( void )
{
	//g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(), KD3DWnd::D3DWE_SCENE_CHANGE);
	//g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(), KGCUIScene::D3DWE_TIME_TICK_ONE_MINUTE);
	
	//g_pkUIMgr->ConnectGlobalEventProcedure( KD3DWnd::D3DWE_SCENE_CHANGE, this, &KGCFatigueInfoBox::OnChangeState );
	//g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_TIME_TICK_ONE_MINUTE, this, &KGCFatigueInfoBox::UpdateInfo );
	
	m_pkOnlineProgressBar->InitState(true,false,NULL);
	m_pkOfflineProgressBar->InitState(true,false,NULL);
	m_pkStaticOnline->InitState(true,false,NULL);
	m_pkStaticOffline->InitState(true,false,NULL);

	m_iOriginalOnlinePBarWidth = m_pkOnlineProgressBar->GetWidth();
	m_iOriginalOfflinePBarWidth = m_pkOfflineProgressBar->GetWidth();


	m_dfOriginalOnlinePBarIB = m_pkOnlineProgressBar->GetWndTexCoords(D3DWM_DEFAULT);
	m_dfOriginalOfflinePBarIB = m_pkOfflineProgressBar->GetWndTexCoords(D3DWM_DEFAULT);
}

void KGCFatigueInfoBox::OnChangeState( void )
{
	bool bRender = SiKGCFatigueManager()->IsViewFatigueInfoScene();
	UpdateInfo();
	ToggleRender(bRender);
}

void KGCFatigueInfoBox::UpdateInfo( void )
{
	SFatigueData sTemp = SiKGCFatigueManager()->GetCurrentInfo();
	
	int iOnline = static_cast<int>((100.f * sTemp.iOnline) / sTemp.iMaxOnlineTime);
	if ( iOnline > 100 )
		iOnline = 100;

	int iOffline = static_cast<int>((100.f * sTemp.iOffline) / sTemp.iResetOfflineTime);
	if ( iOffline > 100 )
		iOffline = 100;

	float fOnlineWidth = static_cast<float>(m_iOriginalOnlinePBarWidth) / 100.f;
	int iOnlineWidth = static_cast<int>(fOnlineWidth * iOnline);
	fOnlineWidth = (m_dfOriginalOnlinePBarIB.z - m_dfOriginalOnlinePBarIB.x) * 512.f;
	fOnlineWidth = static_cast<float>(static_cast<int>(fOnlineWidth / 100.f) * iOnline);
	fOnlineWidth = fOnlineWidth / 512.f;

	float fOfflineWidth = static_cast<float>(m_iOriginalOfflinePBarWidth) / 100.f;
	int iOfflineWidth = static_cast<int>(fOfflineWidth * iOffline);
	fOfflineWidth = (m_dfOriginalOfflinePBarIB.z - m_dfOriginalOfflinePBarIB.x) * 512.f;
	fOfflineWidth = static_cast<float>(static_cast<int>(fOfflineWidth / 100.f) * iOffline);
	fOfflineWidth = fOfflineWidth / 512.f;

	if (iOnline >= 100) {
		fOnlineWidth = (m_dfOriginalOnlinePBarIB.z - m_dfOriginalOnlinePBarIB.x);
		iOnlineWidth = m_iOriginalOnlinePBarWidth;
	}

	if (iOffline >= 100) {
		fOfflineWidth = (m_dfOriginalOfflinePBarIB.z - m_dfOriginalOfflinePBarIB.x);
		iOfflineWidth = m_iOriginalOfflinePBarWidth;
	}


	D3DXVECTOR4 dvOnline;
	dvOnline = m_dfOriginalOnlinePBarIB;
	dvOnline.z = m_dfOriginalOnlinePBarIB.x + fOnlineWidth;
	m_pkOnlineProgressBar->SetWndTexCoords(D3DWM_DEFAULT,dvOnline);
	m_pkOnlineProgressBar->SetWidthDirect(iOnlineWidth);

	D3DXVECTOR4 dvOffline;
	dvOffline = m_dfOriginalOfflinePBarIB;
	dvOffline.z = m_dfOriginalOfflinePBarIB.x + fOfflineWidth;
	m_pkOfflineProgressBar->SetWndTexCoords(D3DWM_DEFAULT,dvOffline);
	m_pkOfflineProgressBar->SetWidthDirect(iOfflineWidth);


	m_pkStaticOnline->SetText(g_pkStrLoader->GetReplacedString(STR_FATIGUE_SYSTEM_MESSAGE,"ii",sTemp.iOnline/60,sTemp.iOnline%60));
	m_pkStaticOffline->SetText(g_pkStrLoader->GetReplacedString(STR_FATIGUE_SYSTEM_MESSAGE,"ii",sTemp.iOffline/60,sTemp.iOffline%60));

}


IMPLEMENT_CLASSNAME( KGCFatigueMessageBox );
IMPLEMENT_WND_FACTORY( KGCFatigueMessageBox );
IMPLEMENT_WND_FACTORY_NAME( KGCFatigueMessageBox, "gc_fatigue_message_box" );


KGCFatigueMessageBox::KGCFatigueMessageBox( void )
: m_iCount(0)
, m_pkMin(NULL)
, m_pkSec10(NULL)
, m_pkSec1(NULL)
, m_iOldTime(0)
, m_pkBtnOK(NULL)
, m_pkBtnExitGame(NULL)
, m_pkBtnAuth(NULL)
, m_pkStaticDesc(NULL)
, m_pkBtnOK2(NULL)
{
	LINK_CONTROL("h_time",m_pkMin);
	LINK_CONTROL("m_time1",m_pkSec10);
	LINK_CONTROL("m_time2",m_pkSec1);
	LINK_CONTROL("message_box_ok_button",m_pkBtnOK);
	LINK_CONTROL("message_box_ok_button_1",m_pkBtnOK2);
	LINK_CONTROL("message_box_exit_button",m_pkBtnExitGame);
	LINK_CONTROL("message_box_question_button",m_pkBtnAuth);
	LINK_CONTROL("message_box_text1",m_pkStaticDesc);

	

}

KGCFatigueMessageBox::~KGCFatigueMessageBox( void )
{

}

void KGCFatigueMessageBox::ActionPerformed( const KActionEvent& event )
{

}

void KGCFatigueMessageBox::OnCreate( void )
{
	m_pkSec1->InitState(true);
	m_pkSec10->InitState(true);
	m_pkMin->InitState(true);
	m_pkBtnOK->InitState(true,true,this);
	m_pkBtnOK->Lock(true);
	m_pkBtnExitGame->InitState(true,true,this);
	m_pkBtnAuth->InitState(true,true,this);
	m_pkStaticDesc->InitState(true);
	m_pkBtnOK2->InitState(false,true,this);
	m_pkBtnOK2->Lock(true);

	m_pkBtnOK->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this,&KGCFatigueMessageBox::OnClickBtnOK);
	m_pkBtnOK2->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this,&KGCFatigueMessageBox::OnClickBtnOK);
	m_pkBtnExitGame->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this,&KGCFatigueMessageBox::OnClickBtnExit);
	m_pkBtnAuth->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this,&KGCFatigueMessageBox::OnClickBtnAuth);


	SFatigueData sTemp = SiKGCFatigueManager()->GetCurrentInfo();

	int iType = SiKGCFatigueManager()->GetCurrentFatigueSystemType();

	switch(iType) {
		case KGCFatigueManager::GREEN_TIME :
			m_pkBtnAuth->ToggleRender(false);
			m_pkBtnExitGame->ToggleRender(false);
			if ( ( sTemp.iOnline / 60 ) == 1 )
			{
				m_pkBtnOK->ToggleRender(false);
				m_pkBtnOK2->ToggleRender(true);
				m_pkBtnOK2->SetWindowPosXDirect(147.f);
			}
			else
			{
				m_pkBtnOK->ToggleRender(true);
				m_pkBtnOK2->ToggleRender(false);
				m_pkBtnOK->SetWindowPosXDirect(147.f);
			}

			if ( ( sTemp.iOnline / 60 ) == 3 )
				m_pkStaticDesc->SetTextAutoMultiline(g_pkStrLoader->GetReplacedString(STR_FATIGUE_SYSTEM_MESSAGE_6,"i",sTemp.iOnline/60));
			else 
				m_pkStaticDesc->SetTextAutoMultiline(g_pkStrLoader->GetReplacedString(STR_FATIGUE_SYSTEM_MESSAGE_1,"i",sTemp.iOnline/60));

			break;
		case KGCFatigueManager::YELLOW_TIME :
			m_pkBtnOK->SetWindowPosXDirect(200.f);
			m_pkBtnAuth->SetWindowPosXDirect(91.f);
			m_pkBtnExitGame->ToggleRender(false);
			m_pkStaticDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_FATIGUE_SYSTEM_MESSAGE_2));
			break;
		case KGCFatigueManager::RED_TIME :
			m_pkBtnOK->SetWindowPosXDirect(155.f);
			m_pkBtnAuth->SetWindowPosXDirect(46.f);
            m_pkBtnExitGame->SetWindowPosXDirect(245.f);
			m_pkStaticDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_FATIGUE_SYSTEM_MESSAGE_3));
			break;
	}

	m_iCount = 60;
	m_iOldTime = CTime::GetCurrentTime().GetSecond();
	SiKGCFatigueManager()->SetAlarm(false);
}


void KGCFatigueMessageBox::SetNumber( void )
{
	if ( m_iCount < 0 || m_iCount > 60 ) {
		return;
	}	

	int second1 = m_iCount % 10;
	SetNumberTex(second1, m_pkSec1);
	int second10 = (m_iCount%60) / 10;
	SetNumberTex(second10, m_pkSec10);
	int minute = m_iCount/60;
	SetNumberTex(minute, m_pkMin);
}

void KGCFatigueMessageBox::SetNumberTex( int index, KD3DWnd *pWnd )
{
	if(pWnd == NULL) return;
    
    float tu = static_cast<float>(index) * 39.f + 2.f;
    float tv = 466.f;
    float tu1 = static_cast<float>(index) * 39.f + 38.f ;
    float tv1 = 510.f;
	D3DXVECTOR4 vec(tu,tv,tu1,tv1);
	ChangeUVtoTexel(vec);

	pWnd->SetWndTexCoords(D3DWM_DEFAULT,vec);
}

void KGCFatigueMessageBox::ChangeUVtoTexel( D3DXVECTOR4& vUV )
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

void KGCFatigueMessageBox::FrameMoveInEnabledState( )
{
	int iSecond = CTime::GetCurrentTime().GetSecond();
	if ( m_iOldTime != iSecond ) {
		m_iOldTime = iSecond;
		--m_iCount;
		SetNumber();
	}
	
	if ( m_iCount <= 0 ) {
		m_pkBtnOK->Lock(false);
		m_pkBtnOK2->Lock(false);
        m_pkBtnExitGame->Lock(false);
	}
}

void KGCFatigueMessageBox::OnClickBtnOK( void )
{
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCFatigueMessageBox::OnClickBtnExit( void )
{
	g_MyD3D->m_pStateMachine->GoExitClient();
}

void KGCFatigueMessageBox::OnClickBtnAuth( void )
{
	PopUpWebPage( L"http://member.changyou.com/anti/loginAnti.do" );
}

IMPLEMENT_CLASSNAME( KGCFatigueMessageBox2 );
IMPLEMENT_WND_FACTORY( KGCFatigueMessageBox2 );
IMPLEMENT_WND_FACTORY_NAME( KGCFatigueMessageBox2, "gc_fatigue_message_box2" );


KGCFatigueMessageBox2::KGCFatigueMessageBox2( void )
: m_pkBtnOK(NULL)
, m_pkBtnAuth(NULL)
, m_pkBtnfatigue(NULL)
, m_pkStaticDesc(NULL)
{
	LINK_CONTROL("message_box_ok_button",m_pkBtnOK);
	LINK_CONTROL("message_box_addinfo_button",m_pkBtnAuth);
	LINK_CONTROL("message_box_fatigue_button",m_pkBtnfatigue);
	LINK_CONTROL("message_box_text1",m_pkStaticDesc);
}

KGCFatigueMessageBox2::~KGCFatigueMessageBox2( void )
{

}

void KGCFatigueMessageBox2::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP(m_pkBtnOK, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickOK );
	GCWND_MSG_MAP(m_pkBtnAuth, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickUserAddInfo );
	GCWND_MSG_MAP(m_pkBtnfatigue, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickFatigueAddInfo );
}

void KGCFatigueMessageBox2::OnCreate( void )
{
	m_pkBtnOK->InitState(true,true,this);
	m_pkBtnAuth->InitState(true,true,this);
	m_pkBtnfatigue->InitState(true,true,this);
	m_pkStaticDesc->InitState(true);

	if ( SiKGCFatigueManager()->GetConnectType() == KGCFatigueManager::FUT_MULTI_CONNECT ) {
		m_pkBtnAuth->ToggleRender(false);
		m_pkStaticDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_FATIGUE_SYSTEM_MESSAGE_5));
	}
	else {
		m_pkBtnfatigue->ToggleRender(false);
		m_pkStaticDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_FATIGUE_SYSTEM_MESSAGE_4));
	}

}

void KGCFatigueMessageBox2::OnClickOK( void )
{
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCFatigueMessageBox2::OnClickFatigueAddInfo( void )
{
	PopUpWebPage( L"http://member.changyou.com/anti/loginAnti.do" );
}

void KGCFatigueMessageBox2::OnClickUserAddInfo( void )
{
	PopUpWebPage( L"http://member.changyou.com/info/userinfo.do?pageTo=SSZ" );
}

