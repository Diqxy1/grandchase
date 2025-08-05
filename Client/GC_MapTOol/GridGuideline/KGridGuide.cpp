#include ".\KGridGuide.h"
//#include <sstream>

#define CHECK_TERM (5.0f)

DWORD KGridGuide::m_dwAlphaEnable = 0;
DWORD KGridGuide::m_dwZEnable = 0;

KGridGuide::KGridGuide(void)
	: m_vPos(-100.0f, -100.0f)
	, m_bVertical(false)
	, m_bDefault(true)
{
}

KGridGuide::~KGridGuide(void)
{
}

HRESULT KGridGuide::RenderFH()
{
	D3DCOLOR Color = 0x7f919191;
	if (m_bVertical)
	{
		m_Vertex[0].Pos.x = m_vPos.x;
		m_Vertex[0].Pos.y = SiKGCMap()->GetPixelHeight();
		m_Vertex[1].Pos.x = m_vPos.x;
		m_Vertex[1].Pos.y = 0.0f;
	}
	else
	{
		m_Vertex[0].Pos.x = 0.0f;
		m_Vertex[0].Pos.y = m_vPos.y;
		m_Vertex[1].Pos.x = SiKGCMap()->GetPixelWidth();
		m_Vertex[1].Pos.y = m_vPos.y;
	}
	m_Vertex[0].Diff = Color;
	m_Vertex[1].Diff = Color;

	g_pd3dDevice->SetFVF(CToolObject::FootHoldVertex::FVF);
	g_pd3dDevice->SetTexture(0, NULL);
	g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, m_Vertex, sizeof(CToolObject::FootHoldVertex));

	return S_OK;
}

#define TO_INT(p) static_cast<int>( static_cast<int>( ( p + FOOTHOLDSIZE*0.5 ) / FOOTHOLDSIZE ) * FOOTHOLDSIZE )
#define TO_INT_INDEX(p) static_cast<int>( (p + FOOTHOLDSIZE*0.5) / FOOTHOLDSIZE )

HRESULT KGridGuide::Render()
{
	D3DCOLOR Color = m_bDefault ? 0xffffff80 : 0xff00ffff;

	if (m_bVertical)
	{
		m_Vertex[0].Pos.x = static_cast<float>(TO_INT(m_vPos.x));
		m_Vertex[0].Pos.y = SiKGCMap()->GetPixelHeight() + 40.0f;
		m_Vertex[1].Pos.x = static_cast<float>(TO_INT(m_vPos.x));
		m_Vertex[1].Pos.y = -40.0f;
	}
	else
	{
		m_Vertex[0].Pos.x = -40.0f;
		m_Vertex[0].Pos.y = static_cast<float>(TO_INT(m_vPos.y));
		m_Vertex[1].Pos.x = SiKGCMap()->GetPixelWidth() + 40.0f;
		m_Vertex[1].Pos.y = static_cast<float>(TO_INT(m_vPos.y));
	}
	m_Vertex[0].Diff = Color;
	m_Vertex[1].Diff = Color;

	g_pd3dDevice->SetFVF(CToolObject::FootHoldVertex::FVF);
	g_pd3dDevice->SetTexture(0, NULL);
	g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, m_Vertex, sizeof(CToolObject::FootHoldVertex));

	D3DXVECTOR3 vPos[2];
	if (m_bVertical)
	{
		vPos[0] = D3DXVECTOR3(m_Vertex[0].Pos.x, m_Vertex[0].Pos.y + 15.0f, 0.3f);
		vPos[1] = D3DXVECTOR3(m_Vertex[1].Pos.x, m_Vertex[1].Pos.y - 15.0f, 0.3f);
	}
	else
	{
		vPos[0] = D3DXVECTOR3(m_Vertex[0].Pos.x - 15.0f, m_Vertex[0].Pos.y + 5.0f, 0.3f);
		vPos[1] = D3DXVECTOR3(m_Vertex[1].Pos.x + 15.0f, m_Vertex[1].Pos.y + 5.0f, 0.3f);
	}

	D3DXMATRIX matView, matProj, matWorld;
	memset(&matView, 0, sizeof(matView));
	memset(&matProj, 0, sizeof(matProj));
	D3DVIEWPORT9 vp;
	D3DXMatrixIdentity(&matWorld);
	g_pd3dDevice->GetViewport(&vp);
	g_pd3dDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pd3dDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXVec3Project(&vPos[0], &vPos[0], &vp, &matProj, &matView, &matWorld);
	D3DXVec3Project(&vPos[1], &vPos[1], &vp, &matProj, &matView, &matWorld);

	std::wstringstream stm;
	if (m_bVertical)
	{
		stm << TO_INT_INDEX(m_vPos.x);
	}
	else
	{
		stm << TO_INT_INDEX(m_vPos.y);
	}

	g_AppFormView->m_pkFont->OutTextXY((int)(vPos[0].x), (int)(vPos[0].y), stm.str().c_str(), Color);
	g_AppFormView->m_pkFont->OutTextXY((int)(vPos[1].x), (int)(vPos[1].y), stm.str().c_str(), Color);

	return S_OK;
}

bool KGridGuide::IsMouseOver(const D3DVECTOR& vMousePos)
{
	if (m_bVertical)
	{
		if (vMousePos.x > m_vPos.x - CHECK_TERM &&
			vMousePos.x < m_vPos.x + CHECK_TERM)
		{
			return true;
		}
	}
	else
	{
		if (vMousePos.y > m_vPos.y - CHECK_TERM &&
			vMousePos.y < m_vPos.y + CHECK_TERM)
		{
			return true;
		}
	}

	return false;
}

void KGridGuide::PreRenderProcess()
{
	g_pd3dDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &m_dwAlphaEnable);
	g_pd3dDevice->GetRenderState(D3DRS_ZENABLE, &m_dwZEnable);

	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
}

void KGridGuide::PostRenderProcess()
{
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, m_dwAlphaEnable);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, m_dwZEnable);
}