// DlgTileBG.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "DlgTileBG.h"
#include "xSkinButton.h"
#include "DlgTileInfo.h"
#include "QuestScripterDlg.h"
//#include "DlgTileTypeSel.h"
//#include "DlgMapToObject.h"

// CDlgTileBG 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTileBG, CDialog)
CDlgTileBG::CDlgTileBG(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTileBG::IDD, pParent)
{
	m_iRow = 0;
	m_iCol = 0;
	m_bDirectionMode = true;
}

CDlgTileBG::~CDlgTileBG()
{
	DestroyButtons();
}

void CDlgTileBG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgTileBG, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_SIZE()
	ON_MESSAGE(WM_CLICK_TOOL_BUTTON, OnClickTileButton)
END_MESSAGE_MAP()

// CDlgTileBG 메시지 처리기입니다.

void CDlgTileBG::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//OnOK();
}

void CDlgTileBG::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//OnCancel();
}

void CDlgTileBG::CreateButtons(int iRow, int iCol)
{
	m_iRow = iRow;
	m_iCol = iCol;
	int iTotalSize = m_iRow * m_iCol;

	const int ibtWidth = 30;
	const int ibtHeight = 30;

	const int iStartPosX = 5;
	const int iStartPosY = 5;

	CRect rtClient;
	GetClientRect(rtClient);

	if (rtClient.Width() < ibtWidth) return;

	for (int i = 0; i < (int)m_vtButton.size(); i++)
	{
		if (m_vtButton[i])
			m_vtButton[i]->DestroyWindow();
		SAFE_DELETE(m_vtButton[i]);
	}
	m_vtButton.clear();
	m_vtTileMap.clear();

	CRect rtButton;
	rtButton.left = iStartPosX;
	rtButton.right = rtButton.left + ibtWidth;
	rtButton.top = iStartPosY;
	rtButton.bottom = rtButton.top + ibtHeight;

	CString strPath = GCUTIL_STR::GCStrCharToWide(g_Path);
	strPath += L"/Data/Texture/Silver/0.bmp";

	int iRowCnt = 0;
	int iColCnt = 0;
	for (int i = 0; i < iTotalSize; i++)
	{
		CxSkinButton *pButton;
		pButton = new CxSkinButton;

		pButton->Create(L"", WS_CHILD | WS_VISIBLE | WS_BORDER, rtButton, this, (WM_USER + 20000) + i);

		TileMap tilemap;
		tilemap.iCol = iColCnt;
		tilemap.iRow = iRowCnt;
		m_vtTileMap.push_back(tilemap);
		if (m_bDirectionMode)
		{
			pButton->SetSkin(strPath, strPath, strPath, strPath, strPath, L"", 1, 1, 0);
		}
		else
		{
			CString strStage;
			strStage.Format(L"%d", tilemap.iStage);
			pButton->SetWindowText(strStage);
		}
		SetTooltipTileMap(pButton, tilemap);
		m_vtButton.push_back(pButton);

		if (0 == (i + 1) % m_iCol)
		{
			iRowCnt++;
			iColCnt = 0;
		}
		else
		{
			iColCnt++;
		}
	}

	ReSizeButton();
}

bool CDlgTileBG::CopyToQuest(Quest *quest)
{
	if (!quest) return false;

	quest->iTileX = m_iCol;
	quest->iTileY = m_iRow;

	quest->ClearTileMap();
	for (int i = 0; i < (int)m_vtTileMap.size(); i++)
		quest->SetTileMap(m_vtTileMap[i]);

	return true;
}

bool CDlgTileBG::CopyFromQuest(Quest *quest)
{
	if (!quest) return false;

	DestroyButtons();
	CreateButtons(quest->iTileY, quest->iTileX);

	for (int i = 0, iCnt = 0; i < (int)quest->iTileY; ++i)
	{
		for (int j = 0; j < (int)quest->iTileX; ++j, ++iCnt)
		{
			TileMap *pTileMap = quest->GetTileMap(i, j);
			TileMap Temp;
			Temp.iCol = j;
			Temp.iRow = i;
			if (!pTileMap) pTileMap = &Temp;
			SetRowColTileMap(i, j, *pTileMap);
			CDlgTileInfo dlgTileInfo;

			if (m_bDirectionMode)
			{
				if (ExistenceStage(pTileMap->iStage))
					SetButtonImage(m_vtButton[iCnt], dlgTileInfo.GetArrowPath((CDlgTileInfo::ARROW_COLOR)pTileMap->iFlag, pTileMap->iD_Path));
				else
					SetButtonImage(m_vtButton[iCnt], dlgTileInfo.GetArrowPath(CDlgTileInfo::NOSTAGE, pTileMap->iD_Path));
			}
			else
			{
				CString strStage;
				strStage.Format(L"%d", pTileMap->iStage);
				m_vtButton[iCnt]->SetWindowText(strStage);
			}

			SetTooltipTileMap(m_vtButton[iCnt], *pTileMap);
		}
	}

	return true;
}

void CDlgTileBG::SetRowColTileMap(int iRow, int iCol, TileMap tilemap)
{
	for (int i = 0; i < (int)m_vtTileMap.size(); ++i)
	{
		if (m_vtTileMap[i].iRow == iRow && m_vtTileMap[i].iCol == iCol)
		{
			m_vtTileMap[i] = tilemap;
			return;
		}
	}

	m_vtTileMap.push_back(tilemap);
}

void CDlgTileBG::DestroyButtons(void)
{
	for (int i = 0; i < (int)m_vtButton.size(); i++)
	{
		m_vtButton[i]->DestroyWindow();
		SAFE_DELETE(m_vtButton[i]);
	}
	m_vtButton.clear();
	m_vtTileMap.clear();
	m_iRow = 0;
	m_iCol = 0;
}

void CDlgTileBG::ReSizeButton(void)
{
	const int ibtWidth = 30;
	const int ibtHeight = 30;

	const int iStartPosX = 5;
	const int iStartPosY = 5;

	CRect rtButton, rtClient;
	GetClientRect(rtClient);

	rtButton.left = iStartPosX;
	rtButton.right = rtButton.left + ibtWidth;
	rtButton.top = iStartPosY;
	rtButton.bottom = rtButton.top + ibtHeight;

	for (int i = 0; i < (int)m_vtButton.size(); i++)
	{
		if (m_vtButton[i])
			m_vtButton[i]->MoveWindow(rtButton, 1);

		if (0 == (i + 1) % m_iCol)
		{
			rtButton.OffsetRect(0, ibtHeight + 5);
			rtButton.left = iStartPosX; rtButton.right = rtButton.left + ibtWidth;
		}
		else
		{
			rtButton.OffsetRect(ibtWidth + 5, 0);
		}
	}
}

BOOL CDlgTileBG::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ReSizeButton();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgTileBG::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ReSizeButton();
}

HRESULT CDlgTileBG::OnClickTileButton(WPARAM wParam, LPARAM lParam)
{
	//************************************************************************
	// 버튼을 찾을 수 없었다면 실패 리턴.
	//************************************************************************
	int iTileIndex = -1;
	for (int i = 0; i < (int)m_vtButton.size(); ++i)
	{
		if (m_vtButton[i] == ((CxSkinButton*)lParam))
		{
			iTileIndex = i;
			break;
		}
	}
	if (-1 == iTileIndex) return E_FAIL;

	CDlgTileInfo dlgTileInfo;
	if (GetKeyState(VK_CONTROL) & 0x80000000)
	{
		// 몇번째 인덱스인지 구한다.
		dlgTileInfo.m_iCurIndex = m_vtTileMap[iTileIndex].iD_Path;
		dlgTileInfo.m_TileMap = m_vtTileMap[iTileIndex];
		dlgTileInfo.NextIndex();

		//************************************************************************
		// 상하좌우에 타일의 화살표가 현재타일을 향하고 있다면
		// 현재타일에서 나를 향해 화살표를 그리고 있는 타일로 화살표를 그려준다.
		//************************************************************************
		int iRow = dlgTileInfo.m_TileMap.iRow;
		int iCol = dlgTileInfo.m_TileMap.iCol;
		int iModifyIndex = dlgTileInfo.m_iCurIndex;
		int bRight = (iModifyIndex >> 0) & 0x00000001;  // 1
		int bBottom = (iModifyIndex >> 1) & 0x00000001;  // 2
		int bLeft = (iModifyIndex >> 2) & 0x00000001;  // 4
		int bTop = (iModifyIndex >> 3) & 0x00000001;  // 8
		if (IsSideArrowToHere(DIRECTION_LEFT, iRow, iCol) && !bLeft)        iModifyIndex |= (0x00000001 << (2));
		if (IsSideArrowToHere(DIRECTION_RIGHT, iRow, iCol) && !bRight)      iModifyIndex |= (0x00000001 << (0));
		if (IsSideArrowToHere(DIRECTION_UP, iRow, iCol) && !bTop)           iModifyIndex |= (0x00000001 << (3));
		if (IsSideArrowToHere(DIRECTION_DOWN, iRow, iCol) && !bBottom)      iModifyIndex |= (0x00000001 << (1));
		dlgTileInfo.m_TileMap.iD_Path = iModifyIndex;

		//// 지도 방향이 바뀌었다면 기존 정보를 초기화 시킨다.
		//TileMap BlankTileMap;
		//BlankTileMap.iCol = m_vtTileMap[iTileIndex].iCol;
		//BlankTileMap.iRow = m_vtTileMap[iTileIndex].iRow;
		//BlankTileMap.iD_Path = iModifyIndex;
		//if( dlgTileInfo.m_TileMap.iD_Path == m_vtTileMap[iTileIndex].iD_Path )
		//    m_vtTileMap[iTileIndex] = dlgTileInfo.m_TileMap;
		//else
		//    m_vtTileMap[iTileIndex] = BlankTileMap;

		m_vtTileMap[iTileIndex].iD_Path = iModifyIndex;

		if (m_bDirectionMode)
		{
			if (ExistenceStage(m_vtTileMap[iTileIndex].iStage))
				SetButtonImage((CxSkinButton*)lParam, dlgTileInfo.GetArrowPath((CDlgTileInfo::ARROW_COLOR)m_vtTileMap[iTileIndex].iFlag, iModifyIndex));
			else
				SetButtonImage((CxSkinButton*)lParam, dlgTileInfo.GetArrowPath(CDlgTileInfo::NOSTAGE, iModifyIndex));
		}
		else
		{
			CString strStage;
			strStage.Format(L"%d", m_vtTileMap[iTileIndex].iStage);
			((CxSkinButton*)lParam)->SetWindowText(strStage);
		}

		SetTooltipTileMap((CxSkinButton*)lParam, m_vtTileMap[iTileIndex]);
	}
	else if (GetKeyState(VK_MENU) & 0x80000000)
	{
		// 몇번째 인덱스인지 구한다.
		dlgTileInfo.m_iCurIndex = m_vtTileMap[iTileIndex].iD_Path;
		dlgTileInfo.m_TileMap = m_vtTileMap[iTileIndex];
		dlgTileInfo.PrevIndex();

		int iRow = dlgTileInfo.m_TileMap.iRow;
		int iCol = dlgTileInfo.m_TileMap.iCol;
		int iModifyIndex = dlgTileInfo.m_iCurIndex;
		int bRight = (iModifyIndex >> 0) & 0x00000001;  // 1
		int bBottom = (iModifyIndex >> 1) & 0x00000001;  // 2
		int bLeft = (iModifyIndex >> 2) & 0x00000001;  // 4
		int bTop = (iModifyIndex >> 3) & 0x00000001;  // 8
		if (IsSideArrowToHere(DIRECTION_LEFT, iRow, iCol) && !bLeft)        iModifyIndex |= (0x00000001 << (2));
		if (IsSideArrowToHere(DIRECTION_RIGHT, iRow, iCol) && !bRight)      iModifyIndex |= (0x00000001 << (0));
		if (IsSideArrowToHere(DIRECTION_UP, iRow, iCol) && !bTop)           iModifyIndex |= (0x00000001 << (3));
		if (IsSideArrowToHere(DIRECTION_DOWN, iRow, iCol) && !bBottom)      iModifyIndex |= (0x00000001 << (1));
		dlgTileInfo.m_TileMap.iD_Path = iModifyIndex;

		//// 지도 방향이 바뀌었다면 기존 정보를 초기화 시킨다.
		//TileMap BlankTileMap;
		//BlankTileMap.iCol = m_vtTileMap[iTileIndex].iCol;
		//BlankTileMap.iRow = m_vtTileMap[iTileIndex].iRow;
		//BlankTileMap.iD_Path = iModifyIndex;
		//if( dlgTileInfo.m_TileMap.iD_Path == m_vtTileMap[iTileIndex].iD_Path )
		//    m_vtTileMap[iTileIndex] = dlgTileInfo.m_TileMap;
		//else
		//    m_vtTileMap[iTileIndex] = BlankTileMap;

		m_vtTileMap[iTileIndex].iD_Path = iModifyIndex;

		if (m_bDirectionMode)
		{
			if (ExistenceStage(m_vtTileMap[iTileIndex].iStage))
				SetButtonImage((CxSkinButton*)lParam, dlgTileInfo.GetArrowPath((CDlgTileInfo::ARROW_COLOR)m_vtTileMap[iTileIndex].iFlag, iModifyIndex));
			else
				SetButtonImage((CxSkinButton*)lParam, dlgTileInfo.GetArrowPath(CDlgTileInfo::NOSTAGE, iModifyIndex));
		}
		else
		{
			CString strStage;
			strStage.Format(L"%d", m_vtTileMap[iTileIndex].iStage);
			((CxSkinButton*)lParam)->SetWindowText(strStage);
		}

		SetTooltipTileMap((CxSkinButton*)lParam, m_vtTileMap[iTileIndex]);
	}
	else
	{
		dlgTileInfo.m_TileMap = m_vtTileMap[iTileIndex];

		if (IDOK == dlgTileInfo.DoModal())
		{
			int iRealIndex = dlgTileInfo.m_iCurIndex;
			if (m_bDirectionMode)
			{
				if (ExistenceStage(dlgTileInfo.m_TileMap.iStage))
					SetButtonImage((CxSkinButton*)lParam, dlgTileInfo.GetArrowPath((CDlgTileInfo::ARROW_COLOR)dlgTileInfo.m_TileMap.iFlag, iRealIndex));
				else
					SetButtonImage((CxSkinButton*)lParam, dlgTileInfo.GetArrowPath(CDlgTileInfo::NOSTAGE, iRealIndex));
			}
			else
			{
				CString strStage;
				strStage.Format(L"%d", dlgTileInfo.m_TileMap.iStage);
				((CxSkinButton*)lParam)->SetWindowText(strStage);
			}

			m_vtTileMap[iTileIndex] = dlgTileInfo.m_TileMap;
			SetTooltipTileMap((CxSkinButton*)lParam, dlgTileInfo.m_TileMap);
		}
	}

	return S_OK;
}

bool CDlgTileBG::IsSideArrowToHere(int iDirection, int iRow, int iCol)
{
	switch (iDirection)
	{
	case DIRECTION_LEFT:
	{
		if ((iCol - 1) < 0) return false;
		TileMap *pTileMap = GetTileMap(iRow, iCol - 1);
		if (!pTileMap) return false;

		int bRight = (pTileMap->iD_Path >> 0) & 0x00000001;  // 1
		if (bRight) return true;
		break;
	}
	case DIRECTION_UP:
	{
		if ((iRow - 1) < 0) return false;
		TileMap *pTileMap = GetTileMap(iRow - 1, iCol);
		if (!pTileMap) return false;

		int bBottom = (pTileMap->iD_Path >> 1) & 0x00000001;  // 4
		if (bBottom) return true;
	}
	break;
	case DIRECTION_RIGHT:
	{
		if ((iCol + 1) >= m_iCol) return false;
		TileMap *pTileMap = GetTileMap(iRow, iCol + 1);
		if (!pTileMap) return false;

		int bLeft = (pTileMap->iD_Path >> 2) & 0x00000001;  // 4
		if (bLeft) return true;
	}
	break;
	case DIRECTION_DOWN:
	{
		if ((iRow + 1) >= m_iRow) return false;
		TileMap *pTileMap = GetTileMap(iRow + 1, iCol);
		if (!pTileMap) return false;

		int bTop = (pTileMap->iD_Path >> 3) & 0x00000001;  // 8
		if (bTop) return true;
	}
	break;
	}

	return false;
}

TileMap *CDlgTileBG::GetTileMap(int iStage)
{
	for (int i = 0; i < (int)m_vtTileMap.size(); ++i)
	{
		if (iStage == m_vtTileMap[i].iStage)
			return &m_vtTileMap[i];
	}

	return NULL;
}

TileMap *CDlgTileBG::GetTileMap(int iRow, int iCol)
{
	for (int i = 0; i < (int)m_vtTileMap.size(); ++i)
	{
		if (iRow == m_vtTileMap[i].iRow
			&& iCol == m_vtTileMap[i].iCol)
			return &m_vtTileMap[i];
	}

	return NULL;
}

void CDlgTileBG::SetTooltipTileMap(CxSkinButton *pButton, TileMap tilemap)
{
	if (!pButton) return;

	CString strRowCol;
	strRowCol.Format(L"Row : [%d]\r\nCol : [%d]", tilemap.iRow, tilemap.iCol);

	CString strFlag;
	switch (tilemap.iFlag)
	{
	case 0:
		strFlag = "Normal";
		break;
	case 1:
		strFlag = "Start";
		break;
	case 2:
		strFlag = "Boss";
		break;
	case 3:
		strFlag = "Hiden";
		break;
	case 4:
		strFlag = "Platformer";
		break;
	}
	CString strToolTip;
	strToolTip.Format(L"Stage : [%s] \r\n%s\nFlag : [%s]"
		, tilemap.strName.c_str(), strRowCol, strFlag);
	pButton->SetToolTipText(strToolTip);
}

void CDlgTileBG::SetButtonImage(CxSkinButton *pButton, CString strFileName)
{
	if (!pButton) return;
	pButton->SetSkin(strFileName, strFileName, strFileName, strFileName, strFileName, L"", 1, 1, 0);
	pButton->Invalidate(1);
}

bool CDlgTileBG::ExistenceStage(int iStage)
{
	CQuestScripterDlg *pQuestScriptDlg = (CQuestScripterDlg*)AfxGetMainWnd();
	if (pQuestScriptDlg)
	{
		if ((int)pQuestScriptDlg->m_Quest.vtStage.size() > iStage && iStage >= 0)
			return true;
	}

	return false;
}

bool CDlgTileBG::ExistenceStageInTileMap(int iStage)
{
	for (int i = 0; i < (int)m_vtTileMap.size(); ++i)
	{
		if (iStage == m_vtTileMap[i].iStage)
		{
			return true;
		}
	}

	return false;
}

void CDlgTileBG::DeleteStage(int iStage)
{
	TileMap tileMap;
	CDlgTileInfo dlgTileInfo;
	for (int i = 0; i < (int)m_vtTileMap.size(); ++i)
	{
		if (iStage == m_vtTileMap[i].iStage)
		{
			tileMap.iRow = m_vtTileMap[i].iRow;
			tileMap.iCol = m_vtTileMap[i].iCol;
			m_vtTileMap[i] = tileMap;

			if (m_bDirectionMode)
			{
				SetButtonImage(m_vtButton[i], dlgTileInfo.GetArrowPath(CDlgTileInfo::NOSTAGE, m_vtTileMap[i].iD_Path));
			}
			else
			{
				CString strStage;
				strStage.Format(L"%d", m_vtTileMap[i].iStage);
				m_vtButton[i]->SetWindowText(strStage);
			}
		}
	}
}

void CDlgTileBG::SetDirectionMode(void)
{
	m_bDirectionMode = true;

	CDlgTileInfo dlgTileInfo;
	for (int i = 0; i < (int)m_vtButton.size(); ++i)
	{
		if (ExistenceStage(m_vtTileMap[i].iStage))
			SetButtonImage(m_vtButton[i], dlgTileInfo.GetArrowPath((CDlgTileInfo::ARROW_COLOR)m_vtTileMap[i].iFlag, m_vtTileMap[i].iD_Path));
		else
			SetButtonImage(m_vtButton[i], dlgTileInfo.GetArrowPath(CDlgTileInfo::NOSTAGE, m_vtTileMap[i].iD_Path));

		m_vtButton[i]->SetWindowText(L"");
	}
}

void CDlgTileBG::SetStageMode(void)
{
	m_bDirectionMode = false;

	for (int i = 0; i < (int)m_vtButton.size(); ++i)
	{
		CString strStage = L"";
		if (m_vtTileMap[i].iStage >= 0)
			strStage.Format(L"%d", m_vtTileMap[i].iStage);
		m_vtButton[i]->SetSkin(L"", L"", L"", L"", L"", L"", 0, 0, 0);
		m_vtButton[i]->SetWindowText(strStage);
		m_vtButton[i]->Invalidate(1);
	}
}

bool CDlgTileBG::IsHaveStartStage(void)
{
	for (int i = 0; i < (int)m_vtTileMap.size(); ++i)
	{
		if (1 == m_vtTileMap[i].iFlag)     // 1이 시작임.
			return true;
	}

	return false;
}