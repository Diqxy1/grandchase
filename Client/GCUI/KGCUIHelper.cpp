#include "stdafx.h"
#include ".\kgcuihelper.h"
//

KGCUIHelper* g_pkUIHelper = NULL;
KGCUIHelper::KGCUIHelper(void)
{
	m_iUILockTime.clear();
}

KGCUIHelper::~KGCUIHelper(void)
{
}

void KGCUIHelper::FrameMove()
{
	/////////////////////////////////////////////////////////////////////
	// Date :  06-12-14 
	// milennium9@kogstudios.com 권상구
	//*******************************************************************
	// UI의 제한시간을 줄여 줍니다.
	/////////////////////////////////////////////////////////////////////

	std::map<int,int>::iterator m_itorTemp;

    for( m_itorTemp = m_iUILockTime.begin() ; m_iUILockTime.end() != m_itorTemp ; m_itorTemp++ )
	{
		if( m_itorTemp->second > 0 )
			m_itorTemp->second = m_itorTemp->second - 1;
	}

	//자기 상태 확인
	CheckPressureState();
}

void KGCUIHelper::SetUILockTime( int LockID_, int time_ )
{
	if( time_ > 0 )
	{
		m_iUILockTime[LockID_] = time_;
	}
}

int KGCUIHelper::GetUILockTime( int LockID_ )
{
	std::map<int,int>::iterator m_itorTemp;

	m_itorTemp = m_iUILockTime.find( LockID_ );

	if( m_itorTemp == m_iUILockTime.end() )
	{
		m_iUILockTime[LockID_] = 0;
		return 0;
	}
	
	return m_iUILockTime[LockID_];
}

void KGCUIHelper::SetCharacterGrade( KSafeArray<KD3DWnd*,KGCUserBar::NUM_USER_GRADE>& apkWnd, int iGrade_ )
{
    for ( int i = 0; i < GC_GRADE_NUM; ++i )
    {
        apkWnd[i]->ToggleRender( false );
    }
	iGrade_--;
    if ( iGrade_ < 0 )
        iGrade_=0;	

    int iSelectImg = static_cast<EGCGrade>( iGrade_ / 3 );
    if ( 3 < iSelectImg )
        iSelectImg = 3;

    // 2007.02.26 wony
    // 길드채널에서는 계급이 안보였는데..
    // 왜 그런설정인지는 모르겠지만..
    // 길드채널에서도 계급 모두 보이게 설정합니다
    apkWnd[iSelectImg]->ToggleRender( true );

    switch ( iGrade_ % 3 )
    {
        case 0: apkWnd[iSelectImg]->SetWndMode( KD3DWnd::D3DWM_SELECT );  break; // 동
        case 1: apkWnd[iSelectImg]->SetWndMode( KD3DWnd::D3DWM_HOVER );   break; // 은
        case 2: apkWnd[iSelectImg]->SetWndMode( KD3DWnd::D3DWM_DEFAULT ); break; // 금
    }    
}

void KGCUIHelper::CheckPressureState()
{
	int iIndex = g_MyD3D->Get_MyPlayer();

	PLAYER* pPlayer = g_MyD3D->MyPlayer[iIndex];

	if (pPlayer->m_kPressureState.m_nState==KPressureState::PS_GAUGE)
	{
		
		if (pPlayer->m_kPressureState.m_dwElapsedTime + TIME_FOR_PREESURE < ::timeGetTime())
		{
			KP2P::GetInstance()->Send_SetMyPressState(KPressureState::PS_PRESS);
			//계속 쏘지 않도록 자기 상태는 먼저 바꿔버린다.
			pPlayer->m_kPressureState.m_nState=KPressureState::PS_PRESS;
		}
	}
}