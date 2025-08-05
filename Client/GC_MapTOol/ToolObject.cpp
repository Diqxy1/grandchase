#include "stdafx.h"
#include ".\toolobject.h"
#include "WOGMapTOol.h"
//#include "GCDeviceManager.h"
#include "KGCMap.h"

DWORD CToolObject::m_dwAlphaEnable = FALSE;
DWORD CToolObject::m_dwZEnable = FALSE;
DWORD CToolObject::m_dwSrcBlend = D3DBLEND_SRCALPHA;
DWORD CToolObject::m_dwDestBlend = D3DBLEND_INVSRCALPHA;

GCDeviceTexture* CToolObject::m_pTexture[4] = { NULL, NULL, NULL, NULL };
GCDeviceTexture* CToolObject::m_pTexValue = NULL;
GCDeviceTexture* CToolObject::m_pTexDead = NULL;

CToolObject::CToolObject(void)
	: m_pEyeInfo(NULL)
	, m_pBlockInfo(NULL)
{
	if (m_pTexValue == NULL)
	{
		SetCurrentDirectoryA(g_Path);
		m_pTexture[DIRECTION::UP] = g_pGCDeviceManager->CreateTexture("Up.dds");
		m_pTexture[DIRECTION::DOWN] = g_pGCDeviceManager->CreateTexture("Down.dds");
		m_pTexture[DIRECTION::LEFT] = g_pGCDeviceManager->CreateTexture("Left.dds");
		m_pTexture[DIRECTION::RIGHT] = g_pGCDeviceManager->CreateTexture("Right.dds");
		m_pTexDead = g_pGCDeviceManager->CreateTexture("dead.dds");
		m_pTexValue = g_pGCDeviceManager->CreateTexture("Center.dds");
	}
	m_dwProp = CFootHold::FHP_JUST_FOOTHOLD;
}

CToolObject::CToolObject(CRenderEyeInfo* pEyeInfo, CFHBlockInfo* pBlockInfo)
	: m_pEyeInfo(pEyeInfo)
	, m_pBlockInfo(pBlockInfo)
{
	if (m_pTexValue == NULL)
	{
		SetCurrentDirectoryA(g_Path);
		m_pTexture[DIRECTION::UP] = g_pGCDeviceManager->CreateTexture("Up.dds");
		m_pTexture[DIRECTION::DOWN] = g_pGCDeviceManager->CreateTexture("Down.dds");
		m_pTexture[DIRECTION::LEFT] = g_pGCDeviceManager->CreateTexture("Left.dds");
		m_pTexture[DIRECTION::RIGHT] = g_pGCDeviceManager->CreateTexture("Right.dds");
		m_pTexDead = g_pGCDeviceManager->CreateTexture("dead.dds");
		m_pTexValue = g_pGCDeviceManager->CreateTexture("Center.dds");
	}
	m_dwProp = CFootHold::FHP_JUST_FOOTHOLD;
}

CToolObject::~CToolObject(void)
{
	m_pEyeInfo = NULL;
}

void CToolObject::PreRenderProcess()
{
	g_pd3dDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &m_dwAlphaEnable);
	g_pd3dDevice->GetRenderState(D3DRS_ZENABLE, &m_dwZEnable);
	g_pd3dDevice->GetRenderState(D3DRS_SRCBLEND, &m_dwSrcBlend);
	g_pd3dDevice->GetRenderState(D3DRS_DESTBLEND, &m_dwDestBlend);

	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CToolObject::PostRenderProcess()
{
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, m_dwAlphaEnable);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, m_dwZEnable);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, m_dwSrcBlend);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, m_dwDestBlend);
}

void CToolObject::Render()
{
	g_pd3dDevice->SetFVF(FootHoldVertex::FVF);
	if (m_dwType != CFootHold::FHT_INIT)
	{
		if (m_dwProp & CFootHold::FHP_NO_COME_RIGHT)
		{
			m_pTexture[DIRECTION::RIGHT]->SetDeviceTexture();
			g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_TexVertex, sizeof(FootHoldVertex));
		}
		if (m_dwProp & CFootHold::FHP_NO_COME_LEFT)
		{
			m_pTexture[DIRECTION::LEFT]->SetDeviceTexture();
			g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_TexVertex, sizeof(FootHoldVertex));
		}
		if (m_dwProp & CFootHold::FHP_NO_JUMP_DOWN)
		{
			m_pTexture[DIRECTION::UP]->SetDeviceTexture();
			g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_TexVertex, sizeof(FootHoldVertex));
		}
		//if( m_dwProp & CFootHold::FHP_NO_JUMP_UP )
		//{
		//	m_pTexture[DIRECTION::DOWN]->SetDeviceTexture();
		//	g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_TexVertex, sizeof(FootHoldVertex) );
		//}
		if (m_dwProp & CFootHold::FHP_DEAD_FOOTHOLD)
		{
			m_pTexDead->SetDeviceTexture();
			g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_TexVertex, sizeof(FootHoldVertex));
		}

		if (m_FH.GetValue().compare("") != 0)
		{
			m_pTexValue->SetDeviceTexture();
			g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_TexVertex, sizeof(FootHoldVertex));
		}
	}

	g_pd3dDevice->SetTexture(0, NULL);
	g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, m_Vertex, sizeof(FootHoldVertex));
}

void CToolObject::SetPos(int iIndexX, int iIndexY, DWORD Type, int iIndexX_2 /*= 0*/, int iIndexY_2 /*= 0 */)
{
	// 처리 전에 Map Load 필요하네?
	// Index Bound 처리

	m_iIndexX = iIndexX;
	m_iIndexY = iIndexY;
	m_iIndexX_2 = iIndexX_2;
	m_iIndexY_2 = iIndexY_2;
	m_dwType = Type;

	float fX = static_cast<float>(static_cast<int>(iIndexX * FOOTHOLDSIZE));
	float fY = static_cast<float>(static_cast<int>(iIndexY * FOOTHOLDSIZE));

	float fX2 = static_cast<float>(static_cast<int>((iIndexX + 1) * FOOTHOLDSIZE));
	float fY2 = static_cast<float>(static_cast<int>((iIndexY + 1) * FOOTHOLDSIZE));

	fX += iIndexX_2;
	fY += iIndexY_2;
	fX2 += iIndexX_2;
	fY2 += iIndexY_2;

	m_Vertex[0].Set(fX, fY, 0.4f);
	m_Vertex[1].Set(fX, fY2, 0.4f);
	m_Vertex[2].Set(fX2, fY2, 0.4f);
	m_Vertex[3].Set(fX2, fY, 0.4f);
	m_Vertex[4].Set(fX, fY, 0.4f);

	m_TexVertex[0].Set(fX, fY2, 0.5f, 0.0f, 0.0f);
	m_TexVertex[1].Set(fX2, fY2, 0.5f, 1.0f, 0.0f);
	m_TexVertex[2].Set(fX, fY, 0.5f, 0.0f, 1.0f);
	m_TexVertex[3].Set(fX2, fY, 0.5f, 1.0f, 1.0f);

	for (int i = 0; i < 5; ++i)
	{
		m_Vertex[i].SetDiff(Type);
	}

	m_FH.SetPos(fX, fY);
	m_FH.SetFootHoldType(Type);
}

void CToolObject::MoveRight(bool bScroll /*= true*/, bool bPerOnePixel /*= false */)
{
	if (!bPerOnePixel)
	{
		if (m_iIndexX < m_pBlockInfo->GetSBlockWidth() - 1)
			m_iIndexX++;
	}
	else
	{
		if (m_iIndexX < m_pBlockInfo->GetSBlockWidth() - 1 || m_iIndexX_2 < FOOTHOLDSIZE - 1)
		{
			if (m_iIndexX_2 == FOOTHOLDSIZE - 1)
			{
				m_iIndexX++;
				m_iIndexX_2 = 0;
			}
			else
			{
				m_iIndexX_2++;
			}
		}
	}

	SetPos(m_iIndexX, m_iIndexY, m_dwType, m_iIndexX_2, m_iIndexY_2);

	//if( bScroll && (m_iIndexX + 1) * FOOTHOLDSIZE > m_pEyeInfo->GetEyeX() + m_pEyeInfo->GetScreenWidth() * 0.5f )
	//{
	//	m_pEyeInfo->SetEyeX( m_pEyeInfo->GetEyeX() + FOOTHOLDSIZE );

	//	D3DXMATRIX matView;
	//	D3DXVECTOR3 vFromPt   = D3DXVECTOR3( m_pEyeInfo->GetEyeX(), m_pEyeInfo->GetEyeY(), 0.0f );
	//	D3DXVECTOR3 vLookatPt = D3DXVECTOR3( m_pEyeInfo->GetEyeX(), m_pEyeInfo->GetEyeY(), 1.0f );
	//	D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	//	D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
	//	g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
	//}
}

void CToolObject::MoveLeft(bool bScroll /*= true*/, bool bPerOnePixel /*= false */)
{
	if (!bPerOnePixel)
	{
		if (m_iIndexX > 0)
			m_iIndexX--;
	}
	else
	{
		if (m_iIndexX > 0 || m_iIndexX_2 > 0)
		{
			if (m_iIndexX_2 == 0)
			{
				m_iIndexX--;
				m_iIndexX_2 = FOOTHOLDSIZE - 1;
			}
			else
			{
				m_iIndexX_2--;
			}
		}
	}

	SetPos(m_iIndexX, m_iIndexY, m_dwType, m_iIndexX_2, m_iIndexY_2);

	//if( bScroll && (m_iIndexX - 1) * FOOTHOLDSIZE < m_pEyeInfo->GetEyeX() - m_pEyeInfo->GetScreenWidth() * 0.5f )
	//{
	//	m_pEyeInfo->SetEyeX( m_pEyeInfo->GetEyeX() - FOOTHOLDSIZE );

	//	D3DXMATRIX matView;
	//	D3DXVECTOR3 vFromPt   = D3DXVECTOR3( m_pEyeInfo->GetEyeX(), m_pEyeInfo->GetEyeY(), 0.0f );
	//	D3DXVECTOR3 vLookatPt = D3DXVECTOR3( m_pEyeInfo->GetEyeX(), m_pEyeInfo->GetEyeY(), 1.0f );
	//	D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	//	D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
	//	g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
	//}
}

void CToolObject::MoveUp(bool bScroll /*= true*/, bool bPerOnePixel /*= false */)
{
	if (!bPerOnePixel)
	{
		if (m_iIndexY < m_pBlockInfo->GetSBlockHeight() - 1)
			m_iIndexY++;
	}
	else
	{
		if (m_iIndexY < m_pBlockInfo->GetSBlockHeight() - 1 || m_iIndexY_2 < FOOTHOLDSIZE - 1)
		{
			if (m_iIndexY_2 == FOOTHOLDSIZE - 1)
			{
				m_iIndexY++;
				m_iIndexY_2 = 0;
			}
			else
			{
				m_iIndexY_2++;
			}
		}
	}

	SetPos(m_iIndexX, m_iIndexY, m_dwType, m_iIndexX_2, m_iIndexY_2);

	//if( bScroll && (m_iIndexY + 1) * FOOTHOLDSIZE > m_pEyeInfo->GetEyeY() + m_pEyeInfo->GetScreenHeight() * 0.5f )
	//{
	//	m_pEyeInfo->SetEyeY( m_pEyeInfo->GetEyeY() + FOOTHOLDSIZE );

	//	D3DXMATRIX matView;
	//	D3DXVECTOR3 vFromPt   = D3DXVECTOR3( m_pEyeInfo->GetEyeX(), m_pEyeInfo->GetEyeY(), 0.0f );
	//	D3DXVECTOR3 vLookatPt = D3DXVECTOR3( m_pEyeInfo->GetEyeX(), m_pEyeInfo->GetEyeY(), 1.0f );
	//	D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	//	D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
	//	g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
	//}
}

void CToolObject::MoveDown(bool bScroll /*= true*/, bool bPerOnePixel /*= false */)
{
	if (!bPerOnePixel)
	{
		if (m_iIndexY > 0)
			m_iIndexY--;
	}
	else
	{
		if (m_iIndexY > 0 || m_iIndexY_2 > 0)
		{
			if (m_iIndexY_2 == 0)
			{
				m_iIndexY--;
				m_iIndexY_2 = FOOTHOLDSIZE - 1;
			}
			else
			{
				m_iIndexY_2--;
			}
		}
	}

	SetPos(m_iIndexX, m_iIndexY, m_dwType, m_iIndexX_2, m_iIndexY_2);

	//   if( bScroll && (m_iIndexY - 1) * FOOTHOLDSIZE < m_pEyeInfo->GetEyeY() - m_pEyeInfo->GetScreenHeight() * 0.5f )
	//{
	//	m_pEyeInfo->SetEyeY( m_pEyeInfo->GetEyeY() - FOOTHOLDSIZE );

	//	D3DXMATRIX matView;
	//	D3DXVECTOR3 vFromPt   = D3DXVECTOR3( m_pEyeInfo->GetEyeX(), m_pEyeInfo->GetEyeY(), 0.0f );
	//	D3DXVECTOR3 vLookatPt = D3DXVECTOR3( m_pEyeInfo->GetEyeX(), m_pEyeInfo->GetEyeY(), 1.0f );
	//	D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	//	D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
	//	g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
	//}
}

void CToolObject::SetProp(DWORD Prop, std::string Mode)
{
	if (Mode == "XOR")
		m_dwProp = m_dwProp ^ Prop;
	else if (Mode == "OR")
		m_dwProp = m_dwProp | Prop;
	else if (Mode == "INIT")
		m_dwProp = CFootHold::FHP_JUST_FOOTHOLD;
	else if (Mode == "FOR")
		m_dwProp = Prop;
}

std::string CToolObject::GetPropString()
{
	std::string str = "";

	if (m_dwProp & CFootHold::FHP_NO_JUMP_DOWN)
		str += "FHP_NO_JUMP_DOWN + ";

	//if( m_dwProp & CFootHold::FHP_NO_JUMP_UP )
	//	str += "FHP_NO_JUMP_UP + ";

	if (m_dwProp & CFootHold::FHP_NO_COME_RIGHT)
		str += "FHP_NO_COME_RIGHT + ";

	if (m_dwProp & CFootHold::FHP_NO_COME_LEFT)
		str += "FHP_NO_COME_LEFT + ";

	if (m_dwProp & CFootHold::FHP_DEAD_FOOTHOLD)
		str += "FHP_DEAD_FOOTHOLD + ";

	str += "FHP_JUST_FOOTHOLD";

	return str;
}

std::string CToolObject::ToString()
{
	std::stringstream str;

	str << "\t{ " << m_iIndexX << ", " << m_iIndexY << ", " << GetPropString() << ", \"" << m_FH.GetValue() << "\", },\n";

	return str.str();
}

bool CToolObject::IsIndexValid()
{
	return (m_pBlockInfo != NULL) &&
		(m_iIndexX <= m_pBlockInfo->GetSBlockWidth() - 1) &&
		m_iIndexX >= 0 &&
		(m_iIndexY <= m_pBlockInfo->GetSBlockHeight() - 1) &&
		m_iIndexY >= 0;
}

void CToolObject::MoveGap(int iIndexX, int iIndexY, int iIndexX_2 /*= 0*/, int iIndexY_2 /*= 0 */)
{
	SetPos(m_iIndexX + iIndexX, m_iIndexY + iIndexY, m_dwType, iIndexX_2, iIndexY_2);
}