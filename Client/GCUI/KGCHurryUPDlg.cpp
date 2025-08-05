#include "stdafx.h"
#include "KGCHurryUPDlg.h"
#include "GCMyInfoScene.h"
#include "../Procedure.h"
#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCHurryUPDlg );
IMPLEMENT_WND_FACTORY( KGCHurryUPDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCHurryUPDlg, "gc_hurryup_dlg" );

KGCHurryUPDlg::KGCHurryUPDlg(void)
{
	m_dwStopTime=0;
}

void KGCHurryUPDlg::OnCreate(void)
{	
	ToggleRender(false);

	// 한번도 Enable안되면 Current 0,0 기본값 채로 나중에 나오게 된다..ㅡㅡ;
	// 초기화 관련 인터페이스에 문제가 있는것 같은데 나중에 고치고 일단 땜빵.
	SetCurrentWindowLocalPos(GetFixedWindowLocalPos());
}


void KGCHurryUPDlg::ViewStart()
{
	ToggleRender(true);
	m_dwStopTime = ::timeGetTime();	
	g_KDSound.Play( "31" );
	SetAllWindowAlpha(0);
	SetWindowZOrder(D3DWZ_TOPMOST);
}

//그려지고 있으면 꺼야 되니까 여기서 체크함
void KGCHurryUPDlg::PostDraw()
{
	CheckAndRenderOff();
}

void KGCHurryUPDlg::CheckAndRenderOff()
{
	if (m_dwStopTime == 0 )
		return;

	if (m_dwStopTime + TIME_FOR_HURRYUPDLG + TIME_FOR_HURRYUPDLG_DISAPPEAR < ::timeGetTime())
	{
		m_dwStopTime = 0;
		ToggleRender(false);
	}

	if (m_dwStopTime + TIME_FOR_HURRYUPDLG < ::timeGetTime())
	{
		int iPercent = 255 - (int)(( (float)( ::timeGetTime() - (m_dwStopTime + TIME_FOR_HURRYUPDLG)) / (float)TIME_FOR_HURRYUPDLG_DISAPPEAR) * 255.0f);
		if (iPercent < 255 && iPercent > 0)
			SetAllWindowAlpha(iPercent);		
	}

	if ( ::timeGetTime() <  m_dwStopTime + TIME_FOR_HURRYUPDLG_COMING)
	{
		int iPercent = 255 - (int)(( (float)( m_dwStopTime + TIME_FOR_HURRYUPDLG_COMING - ::timeGetTime()) / (float)TIME_FOR_HURRYUPDLG_COMING) * 255.0f);
		if (iPercent < 255 && iPercent > 0)
			SetAllWindowAlpha(iPercent);		
	}
}

void KGCHurryUPDlg::ActionPerformed( const KActionEvent& event )
{

}

KGCHurryUPDlg::~KGCHurryUPDlg()
{

}

void KGCHurryUPDlg::OnDestroy()
{
	//SetWndState( D3DWS_DISABLED ); //그냥 바로 사라져 임마.
	//ToggleRender
}
