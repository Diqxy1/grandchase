#include "stdafx.h"
#include "GCGawibawiboGameInfo.h"

IMPLEMENT_CLASSNAME( KGCGawibawiboGameInfo );
IMPLEMENT_WND_FACTORY( KGCGawibawiboGameInfo );
IMPLEMENT_WND_FACTORY_NAME( KGCGawibawiboGameInfo, "gc_gawibawibo_game_info_box" );

KGCGawibawiboGameInfo::KGCGawibawiboGameInfo( void )
:	m_pkBtnChallenge(NULL)
,	m_pkStaticChallengePoint(NULL)
,	m_pkStaticReChallengePoint(NULL)
,	m_pkStaticClearCnt(NULL)
,	m_pkStaticChargeTime(NULL)
,	m_iTryPoint(0)
,	m_iSyncPlayTime(0)
,	m_iMaxTryPointTime(0)
,	m_iPlayTurnCount(0)
,	m_iViewPlayTime(0)
{
	LINK_CONTROL("btn_challenge",m_pkBtnChallenge);
	LINK_CONTROL("static_challenge_point",m_pkStaticChallengePoint);
	LINK_CONTROL("static_re_challenge_point",m_pkStaticReChallengePoint);
	LINK_CONTROL("static_clear_cnt",m_pkStaticClearCnt);
	LINK_CONTROL("static_charge_time",m_pkStaticChargeTime);

}

KGCGawibawiboGameInfo::~KGCGawibawiboGameInfo( void )
{
}

void KGCGawibawiboGameInfo::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		if( event.m_pWnd == m_pkBtnChallenge )
		{
			if(m_iTryPoint > 0){
				m_ActionClickFunc();
			}
			else
			{
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_ERR_2 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
				//도전 포인트가 없다 알려줘야 된다.
			}
		}
	}

}

void KGCGawibawiboGameInfo::OnCreate( void )
{
	m_pkBtnChallenge->InitState(true,true,this);

	m_pkStaticChallengePoint->InitState(true,false,false);
	m_pkStaticReChallengePoint->InitState(true,false,false);
	m_pkStaticClearCnt->InitState(true,false,false);
	m_pkStaticChargeTime->InitState(true,false,false);

	m_pkStaticChallengePoint->SetAlign(DT_CENTER);
	m_pkStaticReChallengePoint->SetAlign(DT_CENTER);
	m_pkStaticClearCnt->SetAlign(DT_CENTER);
	m_pkStaticChargeTime->SetAlign(DT_CENTER);

	
}

void KGCGawibawiboGameInfo::SetGameInfo( const KEVENT_GAWIBAWIBO_INFO_ACK &recvInfo_ )
{


	m_iTryPoint = recvInfo_.m_kUserGawibawiboInfo.m_dwTryPoint;
	if(m_iMaxTryPointTime == 0)
		m_iMaxTryPointTime = recvInfo_.m_dwPointTimeGap*60;

	m_iPlayTurnCount = static_cast<int>(recvInfo_.m_kUserGawibawiboInfo.m_dwTurnCount);

	WCHAR cbuf[256];
	// 도전 포인트 설정.
	wcsnset(cbuf,L'\0',256);
	swprintf(cbuf,L"%d",m_iTryPoint);
	m_pkStaticChallengePoint->SetText(cbuf);
	// 스테이지 클리어 횟수 설정
	wcsnset(cbuf,L'\0',256);
	swprintf(cbuf,L"%d",m_iPlayTurnCount);
	m_pkStaticClearCnt->SetText(cbuf);
	// 재도전 포인트 설정
	wcsnset(cbuf,L'\0',256);
	swprintf(cbuf,L"%d",recvInfo_.m_kUserGawibawiboInfo.m_dwRetryPoint);
	m_pkStaticReChallengePoint->SetText(cbuf);
	// 도전 포인트 받는 시간 설정.
	m_iSyncPlayTime = (g_kGlobalValue.m_iPlayTime + static_cast<int>(m_iMaxTryPointTime/60) - recvInfo_.m_kUserGawibawiboInfo.m_dwRemainTime) % static_cast<int>(m_iMaxTryPointTime/60) ;
	m_iViewPlayTime = m_iSyncPlayTime*60;
	SetTime();

}

void KGCGawibawiboGameInfo::SetTryPointInfo(DWORD recvInfo_ )
{
	m_iTryPoint =recvInfo_;

	WCHAR cbuf[256];
	// 도전 포인트 설정.
	wcsnset(cbuf,L'\0',256);
	swprintf(cbuf,L"%d",m_iTryPoint);
	m_pkStaticChallengePoint->SetText(cbuf);
}

void KGCGawibawiboGameInfo::SetTime()
{
	int iPlayTime = (g_kGlobalValue.m_iPlayTime - m_iSyncPlayTime) * 60 + ( timeGetTime() - g_kGlobalValue.m_dwAccTimeNot ) / 1000;

	// 표시되어 있는 플레이 타임과 현재 플레이 타임이 다르면 갱신
	if( m_iViewPlayTime != iPlayTime )
	{
		m_iViewPlayTime = iPlayTime;
			
		iPlayTime = m_iMaxTryPointTime - ( abs(iPlayTime % m_iMaxTryPointTime) ); 
		
		std::wostringstream strm;
		
		WCHAR strMTemp[20];
		WCHAR strSTemp[20];
		wcsnset(strMTemp,'\0',20);
		wcsnset(strSTemp,'\0',20);

		iPlayTime = iPlayTime % ( 60 * 60 );
		swprintf( strMTemp, 20, L"%02d", (int)( iPlayTime / 60 ) );

		iPlayTime = iPlayTime % 60;
		swprintf( strSTemp, 20, L"%02d", iPlayTime );
		strm<<strMTemp<<L" : "<<strSTemp;
		m_pkStaticChargeTime->SetText(strm.str().c_str());

		if((m_iViewPlayTime % m_iMaxTryPointTime) == 0 || (m_iViewPlayTime % m_iMaxTryPointTime) == 5)
		{
			KP2P::GetInstance()->Send_GetUserGwibawiboTryPointInfoReq();
		}
	}

}

void KGCGawibawiboGameInfo::FrameMoveInEnabledState( void )
{
	SetTime();
}
