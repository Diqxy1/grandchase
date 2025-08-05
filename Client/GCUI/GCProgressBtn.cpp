#include "StdAfx.h"
#include "GCProgressBtn.h"


IMPLEMENT_CLASSNAME( KGCProgressBtn );
IMPLEMENT_WND_FACTORY( KGCProgressBtn );
IMPLEMENT_WND_FACTORY_NAME( KGCProgressBtn, "gc_progress_btn" );

KGCProgressBtn::KGCProgressBtn(void)
{
	m_dwLimitTime=0;
	m_dwPreWidth = 0;
	m_dwPreTexWidth = 0;
	m_dwStartTime = 0;
	LINK_CONTROL( "progress_img", m_pkBackImg );
}

KGCProgressBtn::~KGCProgressBtn(void)
{
}

void KGCProgressBtn::OnCreate( void )
{	
	m_dwLimitTime=0;
	m_avTexDef = m_pkBackImg->GetWndTexCoords(D3DWM_DEFAULT);
	m_iOriginalWidth = m_pkBackImg->GetWidth();
}


void KGCProgressBtn::FrameMoveInEnabledState( void )
{
	if (m_dwLimitTime==0)
		return;

	DWORD dwCurrentTime = ::timeGetTime();
	//프레임 단위로 처리 할려고 했는데.. 시간이 혼용되어 버렸다..ㅡㅡ;;
	if ( m_dwStartTime > dwCurrentTime )
		SetProgressTime(dwCurrentTime);

	float percent = ( dwCurrentTime - m_dwStartTime ) / static_cast<float>(m_dwLimitTime - m_dwStartTime);

	if ( percent > 1.f )
	{
		m_dwLimitTime=0;
		m_pkBackImg->ToggleRender(false);
		Lock(false);
		SpeakToActionListener(KActionEvent( this, KUIPROGBTN_FINISH ));
	}
	else
	{
		m_pkBackImg->ToggleRender(true);
		Lock(true);
	}


	//소수점 자리 비울려고...
	float fWidth = static_cast<float>( static_cast<DWORD>( m_iOriginalWidth * percent ) );
	float fTexWidth =  ( m_avTexDef.z - m_avTexDef.x )  * percent;

	SetSameExtentionSize(
		fWidth,
		static_cast<float>(m_iOriginalWidth),
		fTexWidth,
		( m_avTexDef.z - m_avTexDef.x ));
		
	D3DXVECTOR4 av4;
	av4 = m_avTexDef;
	av4.x = m_avTexDef.x;
	av4.z = m_avTexDef.x + fTexWidth;

	m_pkBackImg->SetWndTexCoords(D3DWM_DEFAULT,av4);
	m_pkBackImg->SetWidthDirect( static_cast<DWORD>(fWidth) );
}

void KGCProgressBtn::SetSameExtentionSize ( float &fCurrentSize, float fMaxSize, float &fCurrentTexSize, float fMaxTexSize ) 
{
	// 3:1 == 9:3  1*9 == 3*3
	float fLLCDSize = fCurrentSize * fMaxTexSize;
	float fRLCDSize = fMaxSize * fCurrentTexSize;
	
	if (fLLCDSize == fRLCDSize)
		return;

	fCurrentTexSize = fLLCDSize / fMaxSize; 
}

//
//void KGCProgressBtn::ActionPerformed( const KActionEvent& event )
//{
//	if (event.m_dwCode==KD3DWnd::D3DWE_BUTTON_CLICK)
//	{
//		SpeakToActionListener(KActionEvent(GetParent(),D3DWE_BUTTON_CLICK));
//	}
//} 