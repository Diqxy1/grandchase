#include "stdafx.h"
#include "GCSystemGateCount.h"

IMPLEMENT_CLASSNAME( KGCSystemGateCount );
IMPLEMENT_WND_FACTORY( KGCSystemGateCount );
IMPLEMENT_WND_FACTORY_NAME( KGCSystemGateCount, "gc_system_monster_gate_count" );

KGCSystemGateCount::KGCSystemGateCount( void )
: m_pkStaticDesc(NULL)
, m_pkStaticEsc(NULL)
, m_pkNumber(NULL)
, m_pkNumber10(NULL)
, m_iCount(-1)
, m_ulTime(0)
{
	LINK_CONTROL("static_desc",m_pkStaticDesc);
	LINK_CONTROL("static_esc",m_pkStaticEsc);
	LINK_CONTROL("num",m_pkNumber);
	LINK_CONTROL("numten",m_pkNumber10);
}

KGCSystemGateCount::~KGCSystemGateCount( void )
{
}

void KGCSystemGateCount::ActionPerformed( const KActionEvent& event )
{

}

void KGCSystemGateCount::OnCreate( void )
{
	m_pkStaticDesc->InitState(true,false,NULL);
	m_pkStaticEsc->InitState(true,false,NULL);
	m_pkNumber->InitState(false,false,NULL);
	m_pkNumber10->InitState(false,false,NULL);
	m_iCount = -1;
	m_ulTime = 0;
	m_pkStaticDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_USE_CHAPION_STAGE_DESC));
	m_pkStaticDesc->SetAlign(DT_CENTER);
	m_pkStaticEsc->SetText(g_pkStrLoader->GetString(STR_ID_USE_CHAPION_STAGE_ESC));

	g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_TG_TIMER_END, this, &KGCSystemGateCount::OnEndCount );
}

void KGCSystemGateCount::OnSetCount( int second )
{
	ToggleRender(true);

	m_pkNumber->ToggleRender(false);	
	m_pkNumber10->ToggleRender(false);	
	m_iCount = second;
	m_ulTime = CTime::GetCurrentTime().GetSecond();
	SetNumber();
}

void KGCSystemGateCount::SetNumber( void )
{
	if ( m_iCount < 0 || m_iCount > 99 ) {
		return;
	}	

	int second1 = m_iCount % 10;
	SetNumberTex(second1, m_pkNumber);
	int second10 = m_iCount / 10;
	SetNumberTex(second10, m_pkNumber10);
	
	if( second10 == 0 ) {
		m_pkNumber->SetWindowPosX(258.f);
		m_pkNumber10->ToggleRender(false);
	}
	else{
		m_pkNumber->SetWindowPosX(270.f);
		m_pkNumber10->ToggleRender(true);
	}

	m_pkNumber->ToggleRender(true);	
}

void KGCSystemGateCount::SetNumberTex( int index, KD3DWnd *pWnd )
{
	if(pWnd == NULL) return;

	int x_index = index % 9 ; 
	int y_index = index / 9 ; 


	float tu = static_cast<float>(x_index) * 53.f + 8.f;
	float tv = static_cast<float>(y_index) * 53.f + 296.f;
	float tu1 = static_cast<float>(x_index) * 53.f + 55.f ;
	float tv1 = static_cast<float>(y_index) * 53.f + 343.f;
	D3DXVECTOR4 vec(tu,tv,tu1,tv1);
	ChangeUVtoTexel(vec);

	pWnd->SetWndTexCoords(D3DWM_DEFAULT,vec);
}

void KGCSystemGateCount::ChangeUVtoTexel( D3DXVECTOR4& vUV )
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

void KGCSystemGateCount::FrameMoveInEnabledState( )
{
	int iSecond = CTime::GetCurrentTime().GetSecond();
	if ( m_ulTime != iSecond ) {
		m_ulTime = iSecond;
		--m_iCount;
		SetNumber();
	}
}

void KGCSystemGateCount::OnEndCount( void )
{
	ToggleRender(false);
}
