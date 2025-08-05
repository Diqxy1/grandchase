#include "stdafx.h"
//#include "D3DColorButton.h"

//


IMPLEMENT_CLASSNAME( KD3DColorButton );
IMPLEMENT_WND_FACTORY( KD3DColorButton );
IMPLEMENT_WND_FACTORY_NAME( KD3DColorButton, "gc_colorbutton" );

KD3DColorButton::KD3DColorButton(void)
{
	SetRect(&m_rtEdge,0,0,0,0);
	m_iFocusShiftX = 0;
	m_iFocusShiftY = 0;
	m_bEnableColor = true;
}

KD3DColorButton::~KD3DColorButton(void)
{
}

void KD3DColorButton::OnCreate( void )
{	
}

void KD3DColorButton::EnableColor(bool bEnable)
{	
	m_bEnableColor = bEnable;
}

void KD3DColorButton::PreDraw()
{
	
	if( !m_bEnableColor )
		return;
	
	const KSafeArray<KD3DUIVertex,4>& Vertex=GetReLocatedWindowRect();

	int iShiftX = 0;
	int iShiftY = 0;
		
	if( GetWndMode() != D3DWM_DEFAULT )
	{
		iShiftX = m_iFocusShiftX;
		iShiftY = m_iFocusShiftY;
	}
	g_pGCDeviceManager2->DrawInScreen(NULL,
		Vertex[0].m_vPos.x, Vertex[0].m_vPos.y, Vertex[3].m_vPos.x, Vertex[3].m_vPos.y,
		0.0f, 0.0f, 1.0f, 1.0f, false, 10.0f, m_Color);

	/*g_pGCDeviceManager2->DrawInScreen(NULL,
		Vertex[0] + iShiftX, vPos.y + m_rtEdge.top + iShiftY,
		vPos.x + GetWidth() - (m_rtEdge.left + m_rtEdge.right) + iShiftX,
		vPos.y + GetHeight() - (m_rtEdge.top + m_rtEdge.bottom) + iShiftY,
		0.0f, 0.0f, 1.0f, 1.0f, false, 10.0f, m_Color);*/

	/*g_pGCDeviceManager2->DrawInScreen(NULL,
		vPos.x + m_rtEdge.left + iShiftX, vPos.y + m_rtEdge.top + iShiftY,
		vPos.x + GetWidth() - (m_rtEdge.left + m_rtEdge.right) + iShiftX,
		vPos.y + GetHeight() - (m_rtEdge.top + m_rtEdge.bottom) + iShiftY,
		0.0f, 0.0f, 1.0f, 1.0f, false, 10.0f, m_Color);*/

	/*g_MyD3D->Draw_TL( vPos.x + m_rtEdge.left + iShiftX, vPos.y + m_rtEdge.top + iShiftY, vPos.x + GetWidth() - (m_rtEdge.left + m_rtEdge.right) + iShiftX, vPos.y + GetHeight() - (m_rtEdge.top + m_rtEdge.bottom) + iShiftY,
		0.0f, 0.0f, 1.0f, 1.0f, false, 10.0f, 
		255,											// A
		(BYTE)((m_Color & 0x00FF0000) >> 16), // R
		(BYTE)((m_Color & 0x0000FF00) >> 8) , // G
		(BYTE)((m_Color & 0x0000FF) >> 0) );	// B		*/
}
