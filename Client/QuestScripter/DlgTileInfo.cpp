// DlgTileInfo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgTileInfo.h"
#include "DlgDirectorySetting.h"
#include "QuestScripterDlg.h"
#include "OpenDGate.h"
#include "GateObjectGenerater.h"
#include "DlgDefaultDirection.h"

//#include "QuestScripter.h"
//#include "DlgTrigger.h"

// CDlgTileInfo 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTileInfo, CDialog)
CDlgTileInfo::CDlgTileInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTileInfo::IDD, pParent)
{
	m_iCurIndex = 0;

	CString strPath = GCUTIL_STR::GCStrCharToWide(g_Path);
	m_mapArrow_Black[0] = strPath + L"/Data/Texture/Black/0.bmp";
	m_mapArrow_Black[1] = strPath + L"/Data/Texture/Black/1.bmp";
	m_mapArrow_Black[2] = strPath + L"/Data/Texture/Black/2.bmp";
	m_mapArrow_Black[3] = strPath + L"/Data/Texture/Black/3.bmp";
	m_mapArrow_Black[4] = strPath + L"/Data/Texture/Black/4.bmp";
	m_mapArrow_Black[5] = strPath + L"/Data/Texture/Black/5.bmp";
	m_mapArrow_Black[6] = strPath + L"/Data/Texture/Black/6.bmp";
	m_mapArrow_Black[7] = strPath + L"/Data/Texture/Black/7.bmp";
	m_mapArrow_Black[8] = strPath + L"/Data/Texture/Black/8.bmp";
	m_mapArrow_Black[9] = strPath + L"/Data/Texture/Black/9.bmp";
	m_mapArrow_Black[10] = strPath + L"/Data/Texture/Black/10.bmp";
	m_mapArrow_Black[11] = strPath + L"/Data/Texture/Black/11.bmp";
	m_mapArrow_Black[12] = strPath + L"/Data/Texture/Black/12.bmp";
	m_mapArrow_Black[13] = strPath + L"/Data/Texture/Black/13.bmp";
	m_mapArrow_Black[14] = strPath + L"/Data/Texture/Black/14.bmp";
	m_mapArrow_Black[15] = strPath + L"/Data/Texture/Black/15.bmp";

	m_mapArrow_Red[0] = strPath + L"/Data/Texture/Red/0.bmp";
	m_mapArrow_Red[1] = strPath + L"/Data/Texture/Red/1.bmp";
	m_mapArrow_Red[2] = strPath + L"/Data/Texture/Red/2.bmp";
	m_mapArrow_Red[3] = strPath + L"/Data/Texture/Red/3.bmp";
	m_mapArrow_Red[4] = strPath + L"/Data/Texture/Red/4.bmp";
	m_mapArrow_Red[5] = strPath + L"/Data/Texture/Red/5.bmp";
	m_mapArrow_Red[6] = strPath + L"/Data/Texture/Red/6.bmp";
	m_mapArrow_Red[7] = strPath + L"/Data/Texture/Red/7.bmp";
	m_mapArrow_Red[8] = strPath + L"/Data/Texture/Red/8.bmp";
	m_mapArrow_Red[9] = strPath + L"/Data/Texture/Red/9.bmp";
	m_mapArrow_Red[10] = strPath + L"/Data/Texture/Red/10.bmp";
	m_mapArrow_Red[11] = strPath + L"/Data/Texture/Red/11.bmp";
	m_mapArrow_Red[12] = strPath + L"/Data/Texture/Red/12.bmp";
	m_mapArrow_Red[13] = strPath + L"/Data/Texture/Red/13.bmp";
	m_mapArrow_Red[14] = strPath + L"/Data/Texture/Red/14.bmp";
	m_mapArrow_Red[15] = strPath + L"/Data/Texture/Red/15.bmp";

	m_mapArrow_Green[0] = strPath + L"/Data/Texture/Green/0.bmp";
	m_mapArrow_Green[1] = strPath + L"/Data/Texture/Green/1.bmp";
	m_mapArrow_Green[2] = strPath + L"/Data/Texture/Green/2.bmp";
	m_mapArrow_Green[3] = strPath + L"/Data/Texture/Green/3.bmp";
	m_mapArrow_Green[4] = strPath + L"/Data/Texture/Green/4.bmp";
	m_mapArrow_Green[5] = strPath + L"/Data/Texture/Green/5.bmp";
	m_mapArrow_Green[6] = strPath + L"/Data/Texture/Green/6.bmp";
	m_mapArrow_Green[7] = strPath + L"/Data/Texture/Green/7.bmp";
	m_mapArrow_Green[8] = strPath + L"/Data/Texture/Green/8.bmp";
	m_mapArrow_Green[9] = strPath + L"/Data/Texture/Green/9.bmp";
	m_mapArrow_Green[10] = strPath + L"/Data/Texture/Green/10.bmp";
	m_mapArrow_Green[11] = strPath + L"/Data/Texture/Green/11.bmp";
	m_mapArrow_Green[12] = strPath + L"/Data/Texture/Green/12.bmp";
	m_mapArrow_Green[13] = strPath + L"/Data/Texture/Green/13.bmp";
	m_mapArrow_Green[14] = strPath + L"/Data/Texture/Green/14.bmp";
	m_mapArrow_Green[15] = strPath + L"/Data/Texture/Green/15.bmp";

	m_mapArrow_Silver[0] = strPath + L"/Data/Texture/Silver/0.bmp";
	m_mapArrow_Silver[1] = strPath + L"/Data/Texture/Silver/1.bmp";
	m_mapArrow_Silver[2] = strPath + L"/Data/Texture/Silver/2.bmp";
	m_mapArrow_Silver[3] = strPath + L"/Data/Texture/Silver/3.bmp";
	m_mapArrow_Silver[4] = strPath + L"/Data/Texture/Silver/4.bmp";
	m_mapArrow_Silver[5] = strPath + L"/Data/Texture/Silver/5.bmp";
	m_mapArrow_Silver[6] = strPath + L"/Data/Texture/Silver/6.bmp";
	m_mapArrow_Silver[7] = strPath + L"/Data/Texture/Silver/7.bmp";
	m_mapArrow_Silver[8] = strPath + L"/Data/Texture/Silver/8.bmp";
	m_mapArrow_Silver[9] = strPath + L"/Data/Texture/Silver/9.bmp";
	m_mapArrow_Silver[10] = strPath + L"/Data/Texture/Silver/10.bmp";
	m_mapArrow_Silver[11] = strPath + L"/Data/Texture/Silver/11.bmp";
	m_mapArrow_Silver[12] = strPath + L"/Data/Texture/Silver/12.bmp";
	m_mapArrow_Silver[13] = strPath + L"/Data/Texture/Silver/13.bmp";
	m_mapArrow_Silver[14] = strPath + L"/Data/Texture/Silver/14.bmp";
	m_mapArrow_Silver[15] = strPath + L"/Data/Texture/Silver/15.bmp";

	m_mapArrow_Blue[0] = strPath + L"/Data/Texture/Blue/0.bmp";
	m_mapArrow_Blue[1] = strPath + L"/Data/Texture/Blue/1.bmp";
	m_mapArrow_Blue[2] = strPath + L"/Data/Texture/Blue/2.bmp";
	m_mapArrow_Blue[3] = strPath + L"/Data/Texture/Blue/3.bmp";
	m_mapArrow_Blue[4] = strPath + L"/Data/Texture/Blue/4.bmp";
	m_mapArrow_Blue[5] = strPath + L"/Data/Texture/Blue/5.bmp";
	m_mapArrow_Blue[6] = strPath + L"/Data/Texture/Blue/6.bmp";
	m_mapArrow_Blue[7] = strPath + L"/Data/Texture/Blue/7.bmp";
	m_mapArrow_Blue[8] = strPath + L"/Data/Texture/Blue/8.bmp";
	m_mapArrow_Blue[9] = strPath + L"/Data/Texture/Blue/9.bmp";
	m_mapArrow_Blue[10] = strPath + L"/Data/Texture/Blue/10.bmp";
	m_mapArrow_Blue[11] = strPath + L"/Data/Texture/Blue/11.bmp";
	m_mapArrow_Blue[12] = strPath + L"/Data/Texture/Blue/12.bmp";
	m_mapArrow_Blue[13] = strPath + L"/Data/Texture/Blue/13.bmp";
	m_mapArrow_Blue[14] = strPath + L"/Data/Texture/Blue/14.bmp";
	m_mapArrow_Blue[15] = strPath + L"/Data/Texture/Blue/15.bmp";
}

CDlgTileInfo::~CDlgTileInfo()
{
}

void CDlgTileInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_STAGE, m_cbStage);
	DDX_Control(pDX, IDC_COMBO_PROPERTY, m_cbProperty);
	DDX_Control(pDX, IDC_SPIN1, m_spinType);
	DDX_Control(pDX, IDC_BUTTON_SEL, m_btSkinButton);
	DDX_Control(pDX, IDC_CHECK_NEW, m_chNewStage);
	DDX_Control(pDX, IDC_COMBO_STAGEMAP, m_cbStageMap);
}

BEGIN_MESSAGE_MAP(CDlgTileInfo, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SEL, OnBnClickedButtonSel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_BN_CLICKED(IDC_BUTTON1_INIT, OnBnClickedButton1Init)
	ON_BN_CLICKED(IDC_BUTTON_TG_EDITOR, OnBnClickedButtonTgEditor)
	ON_BN_CLICKED(IDC_CHECK_NEW, OnBnClickedCheckNew)
	ON_CBN_SELCHANGE(IDC_COMBO_STAGEMAP, OnCbnSelchangeComboStagemap)
	ON_CBN_SELCHANGE(IDC_COMBO_STAGE, OnCbnSelchangeComboStage)
END_MESSAGE_MAP()

// CDlgTileInfo 메시지 처리기입니다.

void CDlgTileInfo::ComboBoxReset(void)
{
	m_cbStage.ResetContent();
	CQuestScripterDlg *pScriptDlg = (CQuestScripterDlg*)AfxGetMainWnd();
	if (pScriptDlg)
	{
		for (int i = 0; i < (int)pScriptDlg->m_Quest.vtStage.size(); ++i)
		{
			if (!pScriptDlg->m_dlgQuest.m_chSortDirectionStage.GetCheck()               // 진행방향별 소트 여부 확인
				|| CheckGateFlag(pScriptDlg->m_Quest.vtStage[i], m_TileMap.iD_Path))
			{
				CString strStage;
				strStage.Format(L"stage%d", i);
				if (!pScriptDlg->m_dlgQuest.m_dlgTileBG.ExistenceStageInTileMap(i)
					|| i == m_TileMap.iStage
					)    // dlgTileBG는 현재 수정중인 QuestTile 정보를 가지고 있다.
				{
					m_cbStage.InsertString(m_cbStage.GetCount(), strStage);
				}
			}
		}
	}
	if (m_cbStage.GetCount()) m_cbStage.SetCurSel(0);
}

void CDlgTileInfo::Refresh(void)
{
	int index = m_cbStage.FindStringExact(0, m_TileMap.strName.c_str());
	if (CB_ERR != index)
		m_cbStage.SetCurSel(index);

	if (m_cbProperty.GetCount() > m_TileMap.iFlag) m_cbProperty.SetCurSel(m_TileMap.iFlag);

	SetDefaultStageMap();

	m_iCurIndex = m_TileMap.iD_Path;
	SetButtonImage();
}

void CDlgTileInfo::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_TileMap.iD_Path = m_iCurIndex;
	m_TileMap.iFlag = m_cbProperty.GetCurSel();

	if (m_cbStage.GetCount() > m_cbStage.GetCurSel() && m_cbStage.GetCurSel() >= 0)
	{
		const int iSelectStage = GetStageIndexInComboBox(m_cbStage.GetCurSel());
		if (m_TileMap.iStage != iSelectStage)
		{
			CQuestScripterDlg *pScriptDlg = (CQuestScripterDlg*)AfxGetMainWnd();
			if (pScriptDlg->m_dlgQuest.m_dlgTileBG.ExistenceStageInTileMap(iSelectStage))
			{
				MessageBox(L"이미 존재 하는 스테이지를 추가하려 하고 있습니다.", L"알림", MB_OK);
				return;
			}
		}

		m_TileMap.iStage = GetStageIndexInComboBox(m_cbStage.GetCurSel());
	}
	else
	{
		m_TileMap.strName = L"";
		m_TileMap.iStage = -1;
	}

	OnOK();
}

void CDlgTileInfo::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}

bool CDlgTileInfo::CheckGateFlag(Stage *pStage, int iFlag)
{
	std::map<int, bool> mapCheck;
	mapCheck[DIRECTION_NONE] = 0;
	mapCheck[DIRECTION_RIGHT] = 0;
	mapCheck[DIRECTION_DOWN] = 0;
	mapCheck[DIRECTION_LEFT] = 0;
	mapCheck[DIRECTION_UP] = 0;

	for (int i = 0; i < (int)pStage->vtCategory.size(); ++i)
	{
		CCategory *pCategory = pStage->vtCategory[i];
		for (int j = 0; j < (int)pCategory->m_vtBaseTrigger.size(); ++j)
		{
			CBaseTrigger *pBaseTrigger = pCategory->m_vtBaseTrigger[j];
			for (int k = 0; k < (int)pBaseTrigger->m_vtEvent.size(); ++k)
			{
				if (TG_EACTION_OPEN_DGATE == pBaseTrigger->m_vtEvent[k]->m_enType)
				{
					COpenDGate *pOpenDGate = (COpenDGate*)pBaseTrigger->m_vtEvent[k];
					mapCheck[pOpenDGate->m_iDirection] = true;
				}

				if (TG_EACTION_GATE_OBJECT_GEN == pBaseTrigger->m_vtEvent[k]->m_enType)
				{
					CGateObjectGenerater *pGateObject = (CGateObjectGenerater*)pBaseTrigger->m_vtEvent[k];
					mapCheck[pGateObject->m_iDirection] = true;
				}
			}

			for (int k = 0; k < (int)pBaseTrigger->m_vtAction.size(); ++k)
			{
				if (TG_EACTION_OPEN_DGATE == pBaseTrigger->m_vtAction[k]->m_enType)
				{
					COpenDGate *pOpenDGate = (COpenDGate*)pBaseTrigger->m_vtAction[k];
					mapCheck[pOpenDGate->m_iDirection] = true;
				}

				if (TG_EACTION_GATE_OBJECT_GEN == pBaseTrigger->m_vtAction[k]->m_enType)
				{
					CGateObjectGenerater *pGateObject = (CGateObjectGenerater*)pBaseTrigger->m_vtAction[k];
					mapCheck[pGateObject->m_iDirection] = true;
				}
			}
		}
	}

	int iCheckFlag = 0;
	if (mapCheck[DIRECTION_NONE])      iCheckFlag += DIRECTION_NONE;
	if (mapCheck[DIRECTION_RIGHT])     iCheckFlag += DIRECTION_RIGHT;
	if (mapCheck[DIRECTION_DOWN])      iCheckFlag += DIRECTION_DOWN;
	if (mapCheck[DIRECTION_LEFT])      iCheckFlag += DIRECTION_LEFT;
	if (mapCheck[DIRECTION_UP])        iCheckFlag += DIRECTION_UP;

	if (iFlag == iCheckFlag) return true;

	return false;
}

void CDlgTileInfo::OnBnClickedButtonSel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NextIndex();
	SetButtonImage();
	ComboBoxReset();
}

BOOL CDlgTileInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	RefreshMapDriectory();

	ComboBoxReset();

	m_cbProperty.ResetContent();
	m_cbProperty.InsertString(0, L"Normal");
	m_cbProperty.InsertString(1, L"Start");
	m_cbProperty.InsertString(2, L"Boss");
	m_cbProperty.InsertString(3, L"Hidden");
	m_cbProperty.InsertString(4, L"Platformer");
	if (m_cbProperty.GetCount()) m_cbProperty.SetCurSel(0);

	SetButtonImage();

	Refresh();
	m_chNewStage.SetCheck(false);

	SetButtonImage();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgTileInfo::NextIndex(void)
{
	if ((m_iCurIndex + 1) >= MAX_DIRECTION)
		m_iCurIndex = 0;
	else
		m_iCurIndex++;

	m_TileMap.iD_Path = m_iCurIndex;
}

void CDlgTileInfo::PrevIndex(void)
{
	if ((m_iCurIndex - 1) < 0)
		m_iCurIndex = MAX_DIRECTION - 1;
	else
		m_iCurIndex--;

	m_TileMap.iD_Path = m_iCurIndex;
}

void CDlgTileInfo::SetButtonImage(void)
{
	CString strFilePath = m_mapArrow_Black[m_iCurIndex];
	m_btSkinButton.SetSkin(strFilePath, strFilePath, strFilePath, strFilePath, strFilePath, L"", 1, 1, 0);
	m_btSkinButton.Invalidate(1);
}

void CDlgTileInfo::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (-1 == pNMUpDown->iDelta)
		NextIndex();
	else
		PrevIndex();
	SetButtonImage();
	ComboBoxReset();
	*pResult = 0;
}

BOOL CDlgTileInfo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_PRIOR:
			PrevIndex();
			SetButtonImage();
			ComboBoxReset();
			break;
		case VK_NEXT:
			NextIndex();
			SetButtonImage();
			ComboBoxReset();
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

CString CDlgTileInfo::GetArrowPath(ARROW_COLOR arrow_color, int iIndex)
{
	switch (arrow_color)
	{
	case NORMAL:
		return m_mapArrow_Black[iIndex];
		break;
	case START:
		return m_mapArrow_Green[iIndex];
		break;
	case BOSS:
		return m_mapArrow_Red[iIndex];
		break;
	case HIDEN:
		return m_mapArrow_Black[iIndex];
		break;
	case PLATFORMER:
		return m_mapArrow_Blue[iIndex];
		break;
	case NOSTAGE:
		return m_mapArrow_Silver[iIndex];
		break;
	}

	return L"";
}

void CDlgTileInfo::OnBnClickedButton1Init()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (false == m_chNewStage.GetCheck() && 0 == m_cbStage.GetCount())
	{
		MessageBox(L"선택된 스테이지가 없습니다. 스테이지를 선택해 주세요.", L"알림", MB_OK);
		return;
	}

	CDlgDefaultDirection DlgDefDirection;
	if (IDOK == DlgDefDirection.DoModal())
	{
		CString strMsg;
		CString strDirection = L"";
		int bRight = (DlgDefDirection.m_iDirection >> 0) & 0x00000001;  // 1
		int bBottom = (DlgDefDirection.m_iDirection >> 1) & 0x00000001;  // 2
		int bLeft = (DlgDefDirection.m_iDirection >> 2) & 0x00000001;  // 4
		int bTop = (DlgDefDirection.m_iDirection >> 3) & 0x00000001;  // 8
		if (bTop)     strDirection += L"상";
		if (bBottom)  strDirection += L"하";
		if (bLeft)    strDirection += L"좌";
		if (bRight)   strDirection += L"우";

		if (strDirection.GetLength())
			strDirection += L"방향의 문이 있는";
		else
			strDirection = L"기본";

		strMsg.Format(L"%s 트리거로 초기화 하시겠습니까?", strDirection);

		if (IDYES == MessageBox(strMsg, L"질문", MB_YESNO))
		{
			TileMap initTileMap;
			initTileMap.iCol = m_TileMap.iCol;
			initTileMap.iRow = m_TileMap.iRow;
			m_iCurIndex = initTileMap.iD_Path = DlgDefDirection.m_iDirection;
			m_TileMap = initTileMap;

			if (DlgDefDirection.m_bDirectionMode)
			{
				CQuestScripterDlg *pScriptDialog = (CQuestScripterDlg*)AfxGetMainWnd();
				if (!pScriptDialog) return;

				// 스테이지를 새로 생성할 것인지, 기존의 스테이지를 초기화 할 것인지를 처리하는 부분
				int iStageIndex = 0;
				Stage *pStage = NULL;
				if (m_chNewStage.GetCheck())
				{
					pStage = new Stage;
					pScriptDialog->m_Quest.vtStage.push_back(pStage);
					iStageIndex = (int)(pScriptDialog->m_Quest.vtStage.size() - 1);
				}
				else
				{
					if ((int)pScriptDialog->m_Quest.vtStage.size() > m_cbStage.GetCurSel())
						iStageIndex = GetStageIndexInComboBox(m_cbStage.GetCurSel());

					pStage = pScriptDialog->m_Quest.vtStage[iStageIndex];
				}

				// 스테이지 초기화 처리
				if ((int)pScriptDialog->m_Quest.vtStage.size() > iStageIndex)
				{
					m_TileMap.iStage = iStageIndex;
					CString strStage;
					strStage.Format(L"stage%d", m_TileMap.iStage);
					m_TileMap.strName = strStage.GetBuffer();
					pScriptDialog->ReDrawTree();

					LoadDirectionDefScript(m_TileMap.iD_Path, pStage);
				}
			}

			OnOK();
		}
	}
}

void CDlgTileInfo::LoadDirectionDefScript(int iDirection, Stage *pStage)
{
	CQuestScripterDlg *pScriptDialog = (CQuestScripterDlg*)AfxGetMainWnd();
	if (!pScriptDialog) return;

	KLuaManager luaMgr;             // 루아클래스
	CString strPath;
	CString strTemp = GCUTIL_STR::GCStrCharToWide(g_Path);

	strPath.Format(L"%s/DefScript/%d.stg", strTemp, iDirection);
	pStage->DeleteItem();

	CStringA strTemp2 = GCUTIL_STR::GCStrWideToChar(strPath);

	if (SUCCEEDED(luaMgr.DoFile(strTemp2)))
		pScriptDialog->LoadStageFromLua(luaMgr, L"stage0", &pStage);
}

void CDlgTileInfo::OnBnClickedButtonTgEditor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_cbStage.GetCount() <= 0)
	{
		MessageBox(L"스테이지를 찾을 수 없습니다.", L"알림", MB_OK);
		return;
	}
	int iStage = GetStageIndexInComboBox(m_cbStage.GetCurSel());

	CQuestScripterDlg *pScriptDialog = (CQuestScripterDlg*)AfxGetMainWnd();
	if (!pScriptDialog) return;

	if ((int)pScriptDialog->m_Quest.vtStage.size() <= iStage)
	{
		MessageBox(L"스테이지를 찾을 수 없습니다.", L"알림", MB_OK);
		return;
	}

	if (pScriptDialog->m_dlgTrigger)
	{
		pScriptDialog->m_ScriptTree.Select(pScriptDialog->m_Quest.vtStage[iStage]->MatchKey, TVGN_CARET, 1);
		pScriptDialog->m_dlgTrigger.m_pvtCategory = &pScriptDialog->m_Quest.vtStage[iStage]->vtCategory;
		pScriptDialog->m_dlgTrigger.SetCallWindowPointer(&pScriptDialog->m_dlgStage);
		pScriptDialog->m_dlgTrigger.RefreshTriggerTree();
		pScriptDialog->m_dlgTrigger.ShowWindow(SW_SHOW);
		pScriptDialog->m_dlgInfo.ShowWindow(SW_HIDE);
		OnOK();
	}
	else
	{
		MessageBox(L"트리거 에디터를 띄우는데 문제가 발견되었습니다. 프로그램을 다시 시작해 주세요", L"알림", MB_OK);
	}
}

void CDlgTileInfo::OnBnClickedCheckNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_cbStage.EnableWindow(!m_chNewStage.GetCheck());
	m_cbStageMap.EnableWindow(!m_chNewStage.GetCheck());
}

int CDlgTileInfo::GetStageIndexInComboBox(int iIndex)
{
	CString strStageName;
	m_cbStage.GetLBText(iIndex, strStageName);
	m_TileMap.strName = strStageName.GetBuffer();
	strStageName.Replace(L"stage", L"");

	return _wtoi(strStageName);
}

void CDlgTileInfo::RefreshMapDriectory(void)
{
	//CString strFilePath = g_Path;sssss
	//strFilePath += "/Data/Map/*.*";

	m_cbStageMap.ResetContent();

	CString strMapPath = SiCDlgDirectorySetting()->m_strMapPath;
	strMapPath += "\\*.*";

	CFileFind cFileFind;
	if (cFileFind.FindFile(strMapPath))
	{
		while (1)
		{
			if (!cFileFind.FindNextFile()) break;

			if (!cFileFind.IsDirectory()
				&& !cFileFind.IsDots())
			{
				m_cbStageMap.InsertString(m_cbStageMap.GetCount(), cFileFind.GetFileName());
			}
		}
	}

	if (m_cbStageMap.GetCount() > 0) m_cbStageMap.SetCurSel(0);
}

void CDlgTileInfo::OnCbnSelchangeComboStagemap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (IDYES != MessageBox(L"맵을 변경하게 되면 바로 적용이 됩니다. 계속 진행하겠습니까?", L"알림", MB_YESNO))
	{
		SetDefaultStageMap();
		return;
	}

	CQuestScripterDlg *pScriptDlg = (CQuestScripterDlg*)AfxGetMainWnd();
	if (pScriptDlg && m_cbStage.GetCount())
	{
		const int iStage = GetStageIndexInComboBox(m_cbStage.GetCurSel());
		if ((int)pScriptDlg->m_Quest.vtStage.size() > iStage && iStage >= 0)
		{
			m_cbStageMap.GetWindowText(pScriptDlg->m_Quest.vtStage[iStage]->strStageMap);
		}
	}
}

void CDlgTileInfo::OnCbnSelchangeComboStage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetDefaultStageMap();
}

void CDlgTileInfo::SetDefaultStageMap(void)
{
	CQuestScripterDlg *pScriptDlg = (CQuestScripterDlg*)AfxGetMainWnd();
	if (pScriptDlg && m_cbStage.GetCount())
	{
		const int iStage = GetStageIndexInComboBox(m_cbStage.GetCurSel());
		if ((int)pScriptDlg->m_Quest.vtStage.size() > iStage && iStage >= 0)
		{
			const int iIndex = m_cbStageMap.FindStringExact(0, pScriptDlg->m_Quest.vtStage[iStage]->strStageMap.GetBuffer());
			if (CB_ERR != iIndex)
			{
				m_cbStageMap.SetCurSel(iIndex);
			}
		}
	}
}