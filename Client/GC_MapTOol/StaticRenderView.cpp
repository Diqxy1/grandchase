// StaticRenderView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
//#include "WOGMapTOol.h"
#include "StaticRenderView.h"
#include ".\staticrenderview.h"
#include "resource.h"
#include "Layer.h"
#include "LayerPart.h"
#include "LayerManager.h"
#include "FootHold.h"
#include "ToolObject.h"
#include "KGCMap.h"
#include "LayerList.h"
#include "FootHoldValue.h"

#include "Command/DragPartCmd.h"
#include "Command/AddLayerPartCmd.h"
#include "Command/CommandRecv.h"

#include "./GridGuideline/KGridGuideMgr.h"

// JPG파일저장
#undef TCHAR
#define TCHAR wchar_t
#include "../jpeg/header/ximage.h"
#undef TCHAR
#define TCHAR char

#define ZDELTA (32.0f)
#define CTRL_ZDELTA (256.0f)

#define FOOTHOLDSIZE_INT (20)

// CStaticRenderView

IMPLEMENT_DYNAMIC(CStaticRenderView, CStatic)
CStaticRenderView::CStaticRenderView()
	: CRenderEyeInfo()
	, m_bNeedReload(false)
	, IsShiftPushed(false)
	, m_pCursorPart(NULL)
	, m_pDragPart(NULL)
	, m_iMIndexX(0)
	, m_iMIndexY(0)
	, m_iDragStartIndexX(0)
	, m_iDragStartIndexY(0)
	, m_iDragObjectPosX(0)
	, m_iDragObjectPosY(0)
	, m_iDragObjectWidth(0)
	, m_iDragObjectHeight(0)
	, IsLDownShiftPushed(false)
	, m_pCurGridGuide(NULL)
{
	m_vecPart.clear();
	m_fEyeX = 400.0f;
	m_fEyeY = 300.0f;
	m_fWidth = 800.0f;
	m_fHeight = 600.0f;
}

CStaticRenderView::~CStaticRenderView()
{
}

BEGIN_MESSAGE_MAP(CStaticRenderView, CStatic)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CStaticRenderView::OnFHDrag()
{
	if (m_iMIndexX >= m_iDragStartIndexX)
	{
		if (m_iMIndexY >= m_iDragStartIndexY)
		{
			for (int i = m_iDragStartIndexX; i != m_iMIndexX + 1; ++i)
			{
				if (i < 0 && i > SiKGCMap()->GetBlockWidth() - 1)
					break;

				for (int j = m_iDragStartIndexY; j != m_iMIndexY + 1; ++j)
				{
					if (j < 0 && j > SiKGCMap()->GetBlockHeight() - 1)
						break;

					AddToolObject(i, j);
				}
			}
		}
		else
		{
			for (int i = m_iDragStartIndexX; i != m_iMIndexX + 1; ++i)
			{
				if (i < 0 && i > SiKGCMap()->GetBlockWidth() - 1)
					break;

				for (int j = m_iDragStartIndexY; j != m_iMIndexY - 1; --j)
				{
					if (j < 0 && j > SiKGCMap()->GetBlockHeight() - 1)
						break;

					AddToolObject(i, j);
				}
			}
		}
	}
	else
	{
		if (m_iMIndexY >= m_iDragStartIndexY)
		{
			for (int i = m_iDragStartIndexX; i != m_iMIndexX - 1; --i)
			{
				if (i < 0 && i > SiKGCMap()->GetBlockWidth() - 1)
					break;

				for (int j = m_iDragStartIndexY; j != m_iMIndexY + 1; ++j)
				{
					if (j < 0 && j > SiKGCMap()->GetBlockHeight() - 1)
						break;

					AddToolObject(i, j);
				}
			}
		}
		else
		{
			for (int i = m_iDragStartIndexX; i != m_iMIndexX - 1; --i)
			{
				if (i < 0 && i > SiKGCMap()->GetBlockWidth() - 1)
					break;

				for (int j = m_iDragStartIndexY; j != m_iMIndexY - 1; --j)
				{
					if (j < 0 && j > SiKGCMap()->GetBlockHeight() - 1)
						break;

					AddToolObject(i, j);
				}
			}
		}
	}
}

// CStaticRenderView 메시지 처리기입니다.

void CStaticRenderView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON)
	{
		// 공통
		if (IsShiftPushed && IsLDownShiftPushed)
		{
			float fDiffX = (float)(point.x - m_Point.x);
			float fDiffY = (float)(m_Point.y - point.y);

			m_fEyeX = (float)(static_cast<int>(m_StartPoint.x + static_cast<int>(FOOTHOLDSIZE_INT*0.5) - static_cast<int>(fDiffX)) / FOOTHOLDSIZE_INT) * FOOTHOLDSIZE_INT;
			m_fEyeY = (float)(static_cast<int>(m_StartPoint.y + static_cast<int>(FOOTHOLDSIZE_INT*0.5) - static_cast<int>(fDiffY)) / FOOTHOLDSIZE_INT) * FOOTHOLDSIZE_INT;

			float fCheckTermX = 0.0f;
			if (m_fWidth < 400.0f)
				fCheckTermX = m_fWidth * 0.5f;
			else
				fCheckTermX = 400.0f;

			float fCheckTermY = 0.0f;
			if (m_fHeight < 300.0f)
				fCheckTermY = m_fHeight * 0.5f;
			else
				fCheckTermY = 300.0f;

			if (m_fEyeX < fCheckTermX)
				m_fEyeX = fCheckTermX;
			else if (m_fEyeX + fCheckTermX > SiKGCMap()->GetPixelWidth())
				m_fEyeX = SiKGCMap()->GetPixelWidth() - fCheckTermX;

			if (m_fEyeY < fCheckTermY)
				m_fEyeY = fCheckTermY;
			else if (m_fEyeY + fCheckTermY > SiKGCMap()->GetPixelHeight())
				m_fEyeY = SiKGCMap()->GetPixelHeight() - fCheckTermY;

			D3DXMATRIX matView;
			D3DXVECTOR3 vFromPt = D3DXVECTOR3(m_fEyeX, m_fEyeY, 0.0f);
			D3DXVECTOR3 vLookatPt = D3DXVECTOR3(m_fEyeX, m_fEyeY, 1.0f);
			D3DXVECTOR3 vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			D3DXMatrixLookAtLH(&matView, &vFromPt, &vLookatPt, &vUpVec);
			g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

			std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
			if (g_AppFormView->GetLayerIndex() >= 0 && g_AppFormView->GetLayerIndex() < SiCLayerManager()->GetMaxLayerNum() && !vecLayer.empty())
			{
				float ScrollX = 0.0f;
				float ScrollY = 0.0f;
				ScrollX = (((float)vecLayer[g_AppFormView->GetLayerIndex()]->GetLayerXSize() - WOG_WIDTH) / (SiKGCMap()->GetPixelWidth() - WOG_WIDTH)) * (g_AppFormView->GetRenderView()->GetEyeX() - WOG_WIDTH * 0.5f);
				ScrollY = (((float)vecLayer[g_AppFormView->GetLayerIndex()]->GetLayerYSize() - WOG_HEIGHT) / (SiKGCMap()->GetPixelHeight() - WOG_HEIGHT)) * (g_AppFormView->GetRenderView()->GetEyeY() - WOG_HEIGHT * 0.5f);

				float fLeft = (float)(m_fEyeX - WOG_WIDTH * 0.5f - ScrollX);
				float fTop = (float)(m_fEyeY - WOG_HEIGHT * 0.5f - ScrollY);
				float fRight = (float)(fLeft + vecLayer[g_AppFormView->GetLayerIndex()]->GetLayerXSize());
				float fBottom = (float)(fTop + vecLayer[g_AppFormView->GetLayerIndex()]->GetLayerYSize());

				g_AppFormView->SetCurLayerVertex(fLeft, fTop, fRight, fBottom);
			}
		}
		else
		{
			switch (g_AppFormView->GetCurMode())
			{
			case CAppForm::WOGMAPTOOLMODE::MODE_FH:
			{
				if (m_pCurGridGuide == NULL)
				{
					if (0 == g_AppFormView->m_vecCurObj.size())
						break;

					UpdateMouseIndex();

					if (m_bToolObjectAdd == false)
					{
						CToolObject* pTemp = g_AppFormView->m_vecCurObj[0];
						for (int i = 1; i < (int)g_AppFormView->m_vecCurObj.size(); ++i)
						{
							SAFE_DELETE(g_AppFormView->m_vecCurObj[i]);
						}
						g_AppFormView->m_vecCurObj.clear();
						g_AppFormView->m_vecCurObj.push_back(pTemp);
					}

					if (m_iDragStartIndexX == m_iMIndexX && m_iDragStartIndexY == m_iMIndexY)
						break;

					if (g_AppFormView->m_vecCurObj.size() != 1)
					{
						g_AppFormView->GetFootHoldValue()->DisableAllProperty();
					}
					OnFHDrag();
				}
				else
				{
					if (m_pCurGridGuide != NULL)
					{
						D3DVECTOR MPos;
						Get3DWorldPos(&MPos);
						m_pCurGridGuide->SetGridGuidePos(D3DXVECTOR2(MPos.x, MPos.y));
					}
				}
			}
			break;
			case CAppForm::WOGMAPTOOLMODE::MODE_LAYER:
			{
				// 드래그
				if (m_pCursorPart == NULL)
				{
					POSITION pos = g_AppFormView->GetLayerList()->GetCurObjectPos();
					if (pos == NULL)
						break;
					else
					{
						D3DVECTOR MPos;
						Get3DWorldPos(&MPos);

						if (m_pDragPart != NULL)
						{
							int iPosX;
							int iPosY;

							if (m_bToolObjectAdd)
							{
								iPosX = (static_cast<int>(MPos.x) / 1) * 1 + static_cast<int>(1 * 0.5);
								iPosY = (static_cast<int>(MPos.y) / 1) * 1 + static_cast<int>(1 * 0.5);
							}
							else
							{
								iPosX = (static_cast<int>(MPos.x) / FOOTHOLDSIZE_INT) * FOOTHOLDSIZE_INT + static_cast<int>(FOOTHOLDSIZE_INT*0.5);
								iPosY = (static_cast<int>(MPos.y) / FOOTHOLDSIZE_INT) * FOOTHOLDSIZE_INT + static_cast<int>(FOOTHOLDSIZE_INT*0.5);
							}
							int DraggingPosX = m_iDragStartPosX - iPosX;
							int DraggingPosY = m_iDragStartPosY - iPosY;
							m_pDragPart->SetLayerPos(
								KLayerRect<float>(static_cast<float>(m_iDragObjectPosX - DraggingPosX),
									static_cast<float>(m_iDragObjectPosY - DraggingPosY),
									static_cast<float>(m_iDragObjectWidth),
									static_cast<float>(m_iDragObjectHeight))
							);
							m_pDragPart->SetVertex();
							m_pDragPart->SetPlayerBack(true);
							g_AppFormView->SetControlValue(m_pDragPart);
						}
					}
				}
			}
			break;
			}
		}
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CStaticRenderView::QuaterZoom(bool Zoom)
{
	if (Zoom == true)
	{
		if (m_fHeight > CTRL_ZDELTA)
			m_fHeight -= CTRL_ZDELTA;
		if (m_fWidth > CTRL_ZDELTA * 1.3f)
			m_fWidth -= CTRL_ZDELTA * 1.3f;
	}
	else
	{
		m_fHeight += CTRL_ZDELTA;
		m_fWidth += CTRL_ZDELTA * 1.3f;
	}

	D3DXMATRIX matProj;
	D3DXMatrixOrthoLH(&matProj, m_fWidth, m_fHeight, 0.0f, 3000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CStaticRenderView::Clear()
{
	SAFE_DELETE(m_pCursorPart);
	m_pDragPart = NULL;
}

void CStaticRenderView::ClearCursor()
{
	SAFE_DELETE(m_pCursorPart);
}

void CStaticRenderView::AddToolObject(int iIndexX, int iIndexY)
{
	std::vector< CToolObject* >::iterator vit = std::find_if(g_AppFormView->m_vecCurObj.begin(), g_AppFormView->m_vecCurObj.end(),
		KFindToolObject(iIndexX, iIndexY));

	// 없으면 추가
	if (vit == g_AppFormView->m_vecCurObj.end())
	{
		CToolObject* pTemp = new CToolObject(static_cast<CRenderEyeInfo*>(this), static_cast<CFHBlockInfo*>(SiKGCMap()));
		pTemp->SetPos(iIndexX, iIndexY, CFootHold::FHT_HEAD);
		g_AppFormView->m_vecCurObj.push_back(pTemp);
	}
}

void CStaticRenderView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();

	IsLDownShiftPushed = false;
	if (nFlags & MK_LBUTTON)
	{
		if (IsShiftPushed)
		{
			IsLDownShiftPushed = GetAsyncKeyState(VK_LSHIFT) & 0x8000 || GetAsyncKeyState(VK_RSHIFT) & 0x8000;
			m_Point = point;
			m_StartPoint.x = (LONG)m_fEyeX;
			m_StartPoint.y = (LONG)m_fEyeY;
			return;
		}
		else
		{
			switch (g_AppFormView->GetCurMode())
			{
			case CAppForm::WOGMAPTOOLMODE::MODE_FH:
			{
				D3DVECTOR MPos;
				Get3DWorldPos(&MPos);
				KGridGuide* pGuide = SiKGridGuideMgr()->GetMouseOverGuide(MPos);
				if (pGuide == NULL && m_pCurGridGuide == NULL)
				{
					UpdateMouseIndex();
					m_iDragStartIndexX = m_iMIndexX;
					m_iDragStartIndexY = m_iMIndexY;

					if (m_bToolObjectAdd == false)
					{
						g_AppFormView->ClearCurObj();
						AddToolObject(m_iMIndexX, m_iMIndexY);
						g_AppFormView->GetFootHoldValue()->SetValue(g_AppFormView->m_vecCurObj);

						return;
					}
					else
					{
						AddToolObject(m_iMIndexX, m_iMIndexY);
						return;
					}
					m_pCurGridGuide = NULL;
				}
				else
				{
					g_AppFormView->ClearCurObj();
					m_pCurGridGuide = pGuide;
				}
			}
			break;
			case CAppForm::WOGMAPTOOLMODE::MODE_LAYER:
			{
				if (m_pCursorPart == NULL)
				{
					D3DVECTOR MPos;
					Get3DWorldPos(&MPos);
					CLayerPart* pPart = g_AppFormView->GetLayerList()->GetCurItemData();
					if (pPart != NULL)
					{
						if (pPart->IsMouseIn(MPos) == true)
						{
							if (m_bToolObjectAdd)
							{
								// 이놈 드래그 시작
								m_iDragStartPosX = (static_cast<int>(MPos.x) / 1) * 1 + static_cast<int>(1 * 0.5);
								m_iDragStartPosY = (static_cast<int>(MPos.y) / 1) * 1 + static_cast<int>(1 * 0.5);
							}
							else
							{
								// 이놈 드래그 시작
								m_iDragStartPosX = (static_cast<int>(MPos.x) / FOOTHOLDSIZE_INT) * FOOTHOLDSIZE_INT + static_cast<int>(FOOTHOLDSIZE_INT*0.5);
								m_iDragStartPosY = (static_cast<int>(MPos.y) / FOOTHOLDSIZE_INT) * FOOTHOLDSIZE_INT + static_cast<int>(FOOTHOLDSIZE_INT*0.5);
							}

							if (IsAltPushed) // 복사
							{
								m_pDragPart = pPart->Clone();
								AddObject_(pPart, m_pDragPart);
								m_bNeedReload = true;
							}
							else // 이동
							{
								m_pDragPart = pPart;
							}
							KLayerRect<float> kRect;
							m_pDragPart->GetLayerRect(kRect);
							m_pDragPart->GetLayerRect(m_rtStart);
							m_iDragObjectPosX = static_cast<int>(kRect.m_X);
							m_iDragObjectPosY = static_cast<int>(kRect.m_Y);
							m_iDragObjectWidth = static_cast<int>(kRect.m_Width);
							m_iDragObjectHeight = static_cast<int>(kRect.m_Height);
							m_pDragPart->SetPlayerBack(true);
							g_AppFormView->SetBoxObj(m_pDragPart);
						}
						else
						{
							m_pDragPart = NULL;
						}
					}
					else
						m_pDragPart = NULL;
				}
			}
			return;
			break;
			}
		}
	}
	CStatic::OnLButtonDown(nFlags, point);
}

void CStaticRenderView::RestoreView()
{
	D3DXMATRIX matView;
	D3DXVECTOR3 vFromPt = D3DXVECTOR3(m_fEyeX, m_fEyeY, 0.0f);
	D3DXVECTOR3 vLookatPt = D3DXVECTOR3(m_fEyeX, m_fEyeY, 1.0f);
	D3DXVECTOR3 vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vFromPt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProj;
	D3DXMatrixOrthoLH(&matProj, m_fWidth, m_fHeight, 0.0f, 3000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CStaticRenderView::ResetView()
{
	// Set the world matrix
	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matIdentity);

	D3DXMATRIX matView;
	D3DXVECTOR3 vFromPt = D3DXVECTOR3(400.0f, 300.0f, 0.0f);
	D3DXVECTOR3 vLookatPt = D3DXVECTOR3(400.0f, 300.0f, 1.0f);
	D3DXVECTOR3 vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vFromPt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// Set the projection matrix
	D3DXMATRIX matProj;
	D3DXMatrixOrthoLH(&matProj, 800.0f, 600.0f, 0.0f, 3000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	m_fWidth = 800.0f;
	m_fHeight = 600.0f;

	m_fEyeX = 400.0f;
	m_fEyeY = 300.0f;
}

void CStaticRenderView::CreateCursorPart(std::string PartName)
{
	ClearCursor();

	m_pCursorPart = SiCLayerManager()->CreateClonePart(PartName);
	if (m_pCursorPart == NULL)
		return;

	KLayerRect<float> kRect;
	m_pCursorPart->GetLayerRect(kRect);
	kRect.m_X = 0;
	kRect.m_Y = 0;
	m_pCursorPart->SetLayerPos(kRect);
	m_pCursorPart->SetPlayerBack(true);
	SetCursorPos();
}

void CStaticRenderView::SetCursorPos()
{
	KLayerRect<float> kRect;
	m_pCursorPart->GetLayerRect(kRect);

	CPoint point;
	CRect WndRt;
	GetCursorPos(&point);
	ScreenToClient(&point);
	GetClientRect(&WndRt);
	if (PtInRect(WndRt, point) == TRUE)
	{
		D3DVECTOR MPos;
		Get3DWorldPos(&MPos);

		KLayerRect<float> kRect;
		m_pCursorPart->GetLayerRect(kRect);

		kRect.m_X = (float)(static_cast<int>((MPos.x + static_cast<int>(FOOTHOLDSIZE*0.5) - kRect.m_Width / 2) / FOOTHOLDSIZE) * FOOTHOLDSIZE_INT);
		kRect.m_Y = (float)(static_cast<int>((MPos.y + static_cast<int>(FOOTHOLDSIZE*0.5) - kRect.m_Height / 2) / FOOTHOLDSIZE) * FOOTHOLDSIZE_INT);

		m_pCursorPart->SetLayerPos(kRect);
		m_pCursorPart->SetVertex();
	}
}

BOOL CStaticRenderView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 비율에 방해가 됨
	return CStatic::OnMouseWheel(nFlags, zDelta, pt);

	CRect WndRt;
	GetWindowRect(&WndRt);
	if (PtInRect(WndRt, pt) == TRUE)
	{
		if (zDelta < 0)
		{
			m_fHeight += ZDELTA;
			m_fWidth += ZDELTA * 1.3f;
		}
		else
		{
			if (m_fHeight > ZDELTA)
				m_fHeight -= ZDELTA;
			if (m_fWidth > ZDELTA * 1.3f)
				m_fWidth -= ZDELTA * 1.3f;
		}

		D3DXMATRIX matProj;
		D3DXMatrixOrthoLH(&matProj, m_fWidth, m_fHeight, 0.0f, 3000.0f);
		g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	}

	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}

void CStaticRenderView::UpdateMouseIndex()
{
	D3DVECTOR MPos;
	Get3DWorldPos(&MPos);

	m_iMIndexX = (int)(MPos.x / FOOTHOLDSIZE);
	m_iMIndexY = (int)(MPos.y / FOOTHOLDSIZE);

	if (m_iMIndexX < 0)
		m_iMIndexX = 0;
	if (m_iMIndexX > SiKGCMap()->GetBlockWidth() - 1)
		m_iMIndexX = SiKGCMap()->GetBlockWidth() - 1;

	if (m_iMIndexY < 0)
		m_iMIndexY = 0;
	if (m_iMIndexY > SiKGCMap()->GetBlockHeight() - 1)
		m_iMIndexY = SiKGCMap()->GetBlockHeight() - 1;
}

void CStaticRenderView::Get3DWorldPos(D3DVECTOR* PosOut)
{
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	D3DVECTOR v;
	D3DMATRIX matProj;
	memset(&matProj, 0, sizeof(matProj));
	D3DVIEWPORT9 vp;
	g_pd3dDevice->GetViewport(&vp);
	HRESULT hr = g_pd3dDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	v.x = (((((point.x - vp.X) * 2.0f / vp.Width) - 1.0f)) - matProj._31) / matProj._11;
	v.y = ((-(((point.y - vp.Y) * 2.0f / vp.Height) - 1.0f)) - matProj._32) / matProj._22;
	v.z = 0.0f;

	D3DXMATRIX m, matView;
	D3DVECTOR vPickRayDir, vPickRayOrig;
	g_pd3dDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&m, NULL, &matView);
	vPickRayDir.x = v.x*m._11 + v.y*m._21 + v.z*m._31;
	vPickRayDir.y = v.x*m._12 + v.y*m._22 + v.z*m._32;
	vPickRayDir.z = v.x*m._13 + v.y*m._23 + v.z*m._33;
	vPickRayOrig.x = m._41;
	vPickRayOrig.y = m._42;
	vPickRayOrig.z = m._43;

	PosOut->x = vPickRayOrig.x + vPickRayDir.x;
	PosOut->y = vPickRayOrig.y + vPickRayDir.y;
	PosOut->z = 0.0f;
}

void CStaticRenderView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if (g_AppFormView->GetScriptLoaded() == false)
		return;

	switch (g_AppFormView->GetCurMode())
	{
	case CAppForm::WOGMAPTOOLMODE::MODE_FH:
		break;
	case CAppForm::WOGMAPTOOLMODE::MODE_LAYER:
	{
		if (m_pCursorPart != NULL)
			ClearCursor();
		else
		{
			m_NewMenu.DestroyMenu();
			VERIFY(m_NewMenu.CreatePopupMenu());

			m_vecPart.clear();

			D3DVECTOR MPos;
			Get3DWorldPos(&MPos);
			SiCLayerManager()->FindLayerPartsByPoint(MPos, m_vecPart);

			std::stringstream stm;
			for (int i = 0; i < (int)m_vecPart.size(); ++i)
			{
				stm.str("");
				if (m_vecPart[i]->GetObjectName() == "")
					stm << m_vecPart[i]->GetPartName() << " (" << m_vecPart[i]->GetLayer() << "-" << m_vecPart[i]->GetZOrder() << ")";
				else
					stm << m_vecPart[i]->GetObjectName() << " (" << m_vecPart[i]->GetLayer() << "-" << m_vecPart[i]->GetZOrder() << ")";
				m_NewMenu.AppendMenu(MF_STRING, (UINT_PTR)(WM_USER + i), stm.str().c_str());
			}

			int nRetValue = TrackPopupMenu(m_NewMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, 0, pWnd->GetSafeHwnd(), NULL);

			if (!nRetValue) return;
			m_NewMenu.DestroyMenu();
		}
	}
	break;
	}
}

BOOL CStaticRenderView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (wParam >= WM_USER)
	{
		// 선택된 인덱스를 찾는다.
		int nClicked = wParam - WM_USER;

		if (nClicked >= 0 && nClicked < (int)m_vecPart.size())
		{
			int iLayerNum = 0;
			int iIndex = 0;
			SiCLayerManager()->GetLayerLocationByPtr(m_vecPart[nClicked], iLayerNum, iIndex);

			if (iLayerNum != -1 && iIndex != -1)
			{
				g_AppFormView->GetLayerList()->ReloadLayerList();
				g_AppFormView->GetLayerList()->GetLayerControl()->SetItemState(iLayerNum, LVIS_SELECTED, LVIS_SELECTED);
				g_AppFormView->GetLayerList()->ReloadObjectList(iLayerNum);
				g_AppFormView->GetLayerList()->GetObjectControl()->SetItemState(iIndex, LVIS_SELECTED, LVIS_SELECTED);
				g_AppFormView->GetLayerList()->GetObjectControl()->EnsureVisible(iIndex, FALSE);

				m_pDragPart = (CLayerPart*)g_AppFormView->GetLayerList()->GetObjectControl()->GetItemData(iIndex);
			}
		}
	}

	return CStatic::OnCommand(wParam, lParam);
}

void CStaticRenderView::OnLButtonUp(UINT nFlags, CPoint point)
{
	switch (g_AppFormView->GetCurMode())
	{
	case CAppForm::WOGMAPTOOLMODE::MODE_FH:
	{
		if (m_pCurGridGuide != NULL)
		{
			if (SiKGridGuideMgr()->AddGridGuide(m_pCurGridGuide))
			{
				m_pCurGridGuide = NULL;
			}
		}
	}
	break;
	case CAppForm::WOGMAPTOOLMODE::MODE_LAYER:
		if (m_pCursorPart != NULL && IsShiftPushed == false)
		{
			AddAndReload_(m_pCursorPart, m_pCursorPart->Clone());
			return;
		}

		// 드래그 끝
		if (m_pDragPart != NULL)
		{
			m_pDragPart->SetPlayerBack(false);

			if (m_bNeedReload)
			{
				POSITION pos = g_AppFormView->GetLayerList()->GetCurLayerPos();
				if (pos == NULL)
				{
					return;
				}
				ReloadLayer_(pos);
				m_bNeedReload = false;
			}

			m_pDragPart->GetLayerRect(m_rtEnd);
			if (m_rtEnd != m_rtStart && IsShiftPushed == false)
			{
				CDragPartCmd* pCmd = new CDragPartCmd();
				DragPosData kData;
				kData.m_iDragEndPosX = static_cast<int>(m_rtEnd.m_X);
				kData.m_iDragEndPosY = static_cast<int>(m_rtEnd.m_Y);
				kData.m_iDragStartPosX = static_cast<int>(m_rtStart.m_X);
				kData.m_iDragStartPosY = static_cast<int>(m_rtStart.m_Y);
				pCmd->SetDragData(m_pDragPart, kData);
				g_AppFormView->GetCommandRecv()->AddCommand(static_cast<ICommand*>(pCmd));
				g_AppFormView->AddLog(pCmd->GetLogStr());
			}
		}
		return;
		break;
	}

	CStatic::OnLButtonUp(nFlags, point);
}

void CStaticRenderView::AddAndReload_(IN CLayerPart* pTargetObj, IN CLayerPart* pCloneObj)
{
	if (pTargetObj == NULL)
		return;

	POSITION pos = g_AppFormView->GetLayerList()->GetCurLayerPos();
	if (pos == NULL)
	{
		AfxMessageBox("레이어를 선택하세요");
		return;
	}

	AddObject_(pTargetObj, pCloneObj);
	ReloadLayer_(pos);
}

void CStaticRenderView::ReloadLayer_(IN POSITION& pos)
{
	SiCLayerManager()->UpdateCategoryUse();
	g_AppFormView->GetLayerList()->ReloadLayerList();
	g_AppFormView->GetLayerList()->ReloadObjectList(pos);
	g_AppFormView->GetLayerList()->GetLayerControl()->SetItemState(g_AppFormView->GetLayerIndex(), LVIS_SELECTED, LVIS_SELECTED);
}

void CStaticRenderView::AddObject_(IN CLayerPart* pTargetObj, IN CLayerPart* pCloneObj)
{
	CAddLayerPartCmd* pCmd = new CAddLayerPartCmd();

	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	pCloneObj->SetPlayerBack(false);
	pCloneObj->SetZOrder(g_AppFormView->GetLayerList()->GetObjectControl()->GetItemCount());
	pCloneObj->SetObjectName(pTargetObj->GetPartName());
	pCloneObj->SetLayer(g_AppFormView->GetLayerIndex());
	pCloneObj->SetVertex();
	vecLayer[g_AppFormView->GetLayerIndex()]->AddLayerPart(pCloneObj);

	pCmd->SetAddPart(pCloneObj);
	g_AppFormView->GetCommandRecv()->AddCommand(static_cast<ICommand*>(pCmd));
	g_AppFormView->AddLog(pCmd->GetLogStr());
}

void CStaticRenderView::RenderCurGridGuide()
{
	if (m_pCurGridGuide != NULL)
	{
		m_pCurGridGuide->Render();
	}
}

void CStaticRenderView::DeleteCurGridGuide()
{
	SAFE_DELETE(m_pCurGridGuide);
}

void CStaticRenderView::MakeFootHoldImagePrepare(float fX, float fY)
{
	// Set the world matrix
	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matIdentity);

	D3DXMATRIX matView;
	D3DXVECTOR3 vFromPt = D3DXVECTOR3(fX, fY, 0.0f);
	D3DXVECTOR3 vLookatPt = D3DXVECTOR3(fX, fY, 1.0f);
	D3DXVECTOR3 vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vFromPt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// Set the projection matrix
	D3DXMATRIX matProj;
	D3DXMatrixOrthoLH(&matProj, 600.0f, 600.0f, 0.0f, 3000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	m_fWidth = 600.0f;
	m_fHeight = 600.0f;

	m_fEyeX = fX;
	m_fEyeY = fY;
}

void CStaticRenderView::MakeFootHoldImage(const char* strName)
{
	HBITMAP hBitmap;
	RECT rect;
	HDC hMemDC;
	HDC hScrDC;
	HBITMAP hOldBitmap;

	// 포커스를 가진 윈도우의 좌표를 조사한다.
	GetClientRect(&rect);

	// 윈도우 화면 DC와 메모리 DC를 만들고 메모리 비트맵을 선택한다.
	hScrDC = ::GetDC(m_hWnd);
	::CreateDC("DISPLAY", NULL, NULL, NULL);
	hMemDC = ::CreateCompatibleDC(hScrDC);
	hBitmap = ::CreateCompatibleBitmap(hScrDC, 512, 512);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	// 화면을 메모리 비트맵으로 복사한다.
	BitBlt(hMemDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
		hScrDC, rect.left, rect.top, SRCCOPY);
	SelectObject(hMemDC, hOldBitmap);

	CxImage cimage(CXIMAGE_FORMAT_JPG);
	if (cimage.CreateFromHBITMAP(hBitmap))
	{
		cimage.SetJpegQuality(255);
		cimage.Save(GCUTIL_STR::GCStrCharToWide(strName), CXIMAGE_FORMAT_JPG);
	}
	else
	{
		return;
	}

	DeleteDC(hMemDC);
	DeleteDC(hScrDC);
	DeleteObject(hBitmap);
	DeleteObject(hOldBitmap);

	// 클립보드의 내용을 지운다
	if (::IsClipboardFormatAvailable(CF_BITMAP) == TRUE)
	{
		if (::OpenClipboard(m_hWnd))
		{
			::EmptyClipboard();
			::CloseClipboard();
		}
	}
}