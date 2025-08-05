// DlgTrigger.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTrigger.h"
#include ".\dlgtrigger.h"
#include "QuestScripterDlg.h"
#include "DlgStage.h"

// CDlgTrigger 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTrigger, CDialog)
CDlgTrigger::CDlgTrigger(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTrigger::IDD, pParent)
{
	m_bCreated = false;
	m_bClipBoard = false;
	m_hAccelTable = NULL;
	m_pvtCategory = NULL;
	m_pDlgStage = NULL;
}

CDlgTrigger::~CDlgTrigger()
{
	//for(int i=0;i<(int)m_vtCategory.size();++i)
	//    SAFE_DELETE(m_vtCategory[i]);
	//m_vtCategory.clear();
}

void CDlgTrigger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_TRIGGER, m_TriggerTree);
	DDX_Control(pDX, IDC_STATIC_INFO, m_stInfo);

	SetWindowText(L"트리거 에디터");
}

BEGIN_MESSAGE_MAP(CDlgTrigger, CDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_ADDCHILD_ENTER, OnAddchild_Enter_ForMsg)
	ON_MESSAGE(WM_ADDSIBLING_SPACEBAR, OnAddSibling_SpaceBar_ForMsg)
	ON_MESSAGE(WM_SHIFT_SELECT, OnShiftTreeSelect)
	ON_MESSAGE(WM_DELETESUBTREE, OnDeleteSubTree_ForMsg)
	ON_MESSAGE(WM_CHANG_DIALOG, OnChangeDialog)
	ON_MESSAGE(WM_MENU_ENABLE, OnEnableMessage)
	ON_MESSAGE(WM_CHANGED_TREE_LABEL, OnChangedTreeLabel)
	ON_MESSAGE(WM_TREE_CUSTOMMDRAW, OnTreeCustomDraw)
	ON_MESSAGE(WM_TREE_REDRAW_TEXT_TO_SELITEM, OnTreeReDrawTextToSelItem)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_COMMAND(ID_TG_ADD_CATEGORY, OnTgAddCategory)
	ON_COMMAND(ID_TG_ADD_TRIGGER, OnTgAddTrigger)
	ON_COMMAND(ID_TG_CUT, OnTgCut)
	ON_COMMAND(ID_TG_COPY, OnTgCopy)
	ON_COMMAND(ID_TG_PASTE, OnTgPaste)
	ON_COMMAND(ID_TG_DELETE, OnTgDelete)
	ON_COMMAND(ID_TG_USE, OnTgUse)
	ON_COMMAND(ID_TG_START_WITH_USE, OnTgStartWithUse)
	ON_COMMAND(ID_TREE_CUT, OnTreeCut)
	ON_COMMAND(ID_TREE_COPY, OnTreeCopy)
	ON_COMMAND(ID_TREE_PASTE, OnTreePaste)
END_MESSAGE_MAP()

// CDlgTrigger 메시지 처리기입니다.

int CDlgTrigger::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

LRESULT CDlgTrigger::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (message == WM_NOTIFY)
	{
		if (wParam == IDC_STATIC_SPLITTER)
		{
			SPC_NMHDR* pHdr = (SPC_NMHDR*)lParam;
			DoResize1(pHdr->delta);
		}
	}

	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CDlgTrigger::DoResize1(int delta)
{
	CSplitterControl::ChangeWidth(&m_TriggerTree, delta);
	CSplitterControl::ChangeWidth(&m_dlgTriggerContent, -delta, CW_RIGHTALIGN);
	//CSplitterControl::ChangeWidth(&m_txtContent, -delta, CW_RIGHTALIGN);
	//CSplitterControl::ChangeWidth(&m_wndSplitter2, -delta, CW_RIGHTALIGN);

	//CSplitterControl::ChangeWidth(&m_wndSplitter2, -delta, CW_RIGHTALIGN);
	Invalidate();
	UpdateWindow();
}

BOOL CDlgTrigger::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// TODO: Add extra initialization here
	CRect rc;
	CWnd* pWnd;

	pWnd = GetDlgItem(IDC_STATIC_SPLITTER);
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndSplitter1.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_STATIC_SPLITTER);

	CRect rtClient;
	GetClientRect(rtClient);
	m_wndSplitter1.SetRange(rtClient.left + 100, rtClient.right - 100);

	CRect rtTree;
	CRect rtTriggerContent;
	m_TriggerTree.GetWindowRect(rtTree);
	ScreenToClient(rtTree);
	rtTriggerContent.left = rtTree.right + 15;
	rtTriggerContent.right = rtClient.right - 10;
	rtTriggerContent.top = rtTree.top;
	rtTriggerContent.bottom = rtTree.bottom;
	m_dlgTriggerContent.Create(CdlgTriggerContent::IDD, this);
	m_dlgTriggerContent.MoveWindow(rtTriggerContent, 1);

	RefreshTriggerTree();

	//************************************************************************
	// 단축키를 사용하기 위해 단축키 정보를 불러온다.
	m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR_TRIGGER));
	//************************************************************************

	m_bCreated = true;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgTrigger::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_bCreated)
	{
		CRect rtClient;
		GetClientRect(rtClient);

		m_stInfo.MoveWindow(CRect(rtClient.left + 10, rtClient.bottom - 30, rtClient.right - 10, rtClient.bottom - 10), 1);

		m_wndSplitter1.SetRange(rtClient.left + 100, rtClient.right - 100);
		CRect rtSplit;
		m_wndSplitter1.GetWindowRect(rtSplit);
		ScreenToClient(rtSplit);
		rtSplit.bottom = rtClient.bottom - 40;
		m_wndSplitter1.MoveWindow(rtSplit, 1);

		CRect rtTree;
		m_TriggerTree.GetWindowRect(rtTree);
		ScreenToClient(rtTree);
		rtTree.bottom = rtClient.bottom - 40;
		m_TriggerTree.MoveWindow(rtTree);

		m_wndSplitter1.Invalidate(1);

		CRect rtTriggerContent;
		rtTriggerContent.left = rtTree.right + 15;
		rtTriggerContent.right = rtClient.right - 10;
		rtTriggerContent.top = rtTree.top;
		rtTriggerContent.bottom = rtTree.bottom;
		m_dlgTriggerContent.MoveWindow(rtTriggerContent, 1);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

LRESULT CDlgTrigger::OnAddchild_Enter_ForMsg(WPARAM wParam, LPARAM lParam)
{
	if (!m_pvtCategory) return E_FAIL;

	try
	{
		int iLevel = m_TriggerTree.GetLevelTree(m_TriggerTree.GetSelectedItem());
		if (iLevel > 1)
		{
			MessageBox(L"더 이상 하위구조를 추가 할 수 없습니다.", L"알림", MB_OK);
			return E_FAIL;
		}

		HTREEITEM hSelect = NULL;

		switch ((LEVEL_TYPE)(iLevel + 1))
		{
		case LEVEL_CATEGORY:
		{
			hSelect = m_TriggerTree.AddChild(L"기본 범주");

			CCategory *pCategory = new CCategory;
			pCategory->m_hItem = hSelect;
			pCategory->m_strName = L"기본 범주";
			m_pvtCategory->push_back(pCategory);
		}
		break;
		case LEVEL_TG:
		{
			CCategory *pCurCategory = FindCurrentCategory(m_TriggerTree.GetSelectedItem());
			if (pCurCategory)
			{
				CString strNewTriggerName = GetCheckTGDuplicateTitle(L"비어있는 트리거");
				hSelect = m_TriggerTree.AddChild(strNewTriggerName);

				CBaseTrigger *pNewTrigger = new CBaseTrigger;
				pNewTrigger->m_strTGName = strNewTriggerName;
				pNewTrigger->m_hItem = hSelect;
				pCurCategory->AddTrigger(pNewTrigger);
			}
		}
		break;
		}

		if (hSelect) m_TriggerTree.Select(hSelect, TVGN_CARET, 1);
	}
	catch (...)
	{
		MessageBox(L"범주 및 트리거 추가중 예외가 발생되었습니다.(OnAddchild_Enter_ForMsg)", L"예외", MB_OK);
	}

	return S_OK;
}

LRESULT CDlgTrigger::OnAddSibling_SpaceBar_ForMsg(WPARAM wParam, LPARAM lParam)
{
	if (!m_pvtCategory) return E_FAIL;

	try
	{
		int iLevel = m_TriggerTree.GetLevelTree(m_TriggerTree.GetSelectedItem());
		if (iLevel > 2)
		{
			MessageBox(L"더 이상 이웃구조를 추가 할 수 없습니다.", L"알림", MB_OK);
			return E_FAIL;
		}

		HTREEITEM hSelect = NULL;

		switch ((LEVEL_TYPE)iLevel)
		{
		case LEVEL_CATEGORY:
		{
			hSelect = m_TriggerTree.AddSibling(L"기본 범주");

			CCategory *pCategory = new CCategory;
			pCategory->m_hItem = hSelect;
			pCategory->m_strName = L"기본 범주";
			m_pvtCategory->push_back(pCategory);
		}
		break;
		case LEVEL_TG:
		{
			HTREEITEM hCurItem = m_TriggerTree.GetSelectedItem();
			HTREEITEM hParentItem = m_TriggerTree.GetParentItem(hCurItem);
			CCategory *pCurCategory = FindCurrentCategory(hParentItem);
			if (pCurCategory)
			{
				CString strNewTriggerName = GetCheckTGDuplicateTitle(L"비어있는 트리거");
				hSelect = m_TriggerTree.AddSibling(strNewTriggerName);

				CBaseTrigger *pNewTrigger = new CBaseTrigger;
				pNewTrigger->m_strTGName = strNewTriggerName;
				pNewTrigger->m_hItem = hSelect;
				pCurCategory->AddTrigger(pNewTrigger);
			}
		}
		break;
		}

		if (hSelect)m_TriggerTree.Select(hSelect, TVGN_CARET, 1);
	}
	catch (...)
	{
		MessageBox(L"범주 및 트리거 추가중 예외가 발생되었습니다.(OnAddSibling_SpaceBar_ForMsg)", L"예외", MB_OK);
	}

	return S_OK;
}

CString CDlgTrigger::GetCheckTGDuplicateTitle(CString strTitle)
{
	int iCnt = 0;

	CQuestScripterDlg *pDlgScript = (CQuestScripterDlg*)AfxGetMainWnd();
	if (!pDlgScript)
		return L"Error!";

	CString strTriggerTitle;
	while (1)
	{
		strTriggerTitle.Format(L"%s %d", strTitle, iCnt);
		if (!pDlgScript->FindTriggerByName(strTriggerTitle.GetBuffer()))
			break;
		iCnt++;
	}

	return strTriggerTitle;
}

void CDlgTrigger::FindTGEqualCnt(HTREEITEM hItem, int &iCnt, CString strDefaultName)
{
	while (hItem)
	{
		CString strTreeText = m_TriggerTree.GetItemText(hItem);
		if (-1 != strTreeText.Find(strDefaultName, 0))
			iCnt++;

		FindTGEqualCnt(m_TriggerTree.GetChildItem(hItem), iCnt, strDefaultName);

		hItem = m_TriggerTree.GetNextSiblingItem(hItem);
	}
}

// 부모 구조로 이동하는 것임.
LRESULT CDlgTrigger::OnShiftTreeSelect(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hSelItem = NULL;
	std::wstring strSelect = L"";
	switch (wParam)
	{
	case VK_UP:
		strSelect = MoveItem(VK_UP);
		break;
	case VK_DOWN:
		strSelect = MoveItem(VK_DOWN);
		break;
	case VK_LEFT:
		hSelItem = m_TriggerTree.GetPrevSiblingItem(m_TriggerTree.GetSelectedItem());
		break;
	case VK_RIGHT:
		hSelItem = m_TriggerTree.GetNextSiblingItem(m_TriggerTree.GetSelectedItem());
		break;
	}

	if (false == strSelect.empty())
		RefreshTriggerTree(strSelect);
	else if (hSelItem)
		m_TriggerTree.Select(hSelItem, TVGN_CARET);

	return S_OK;
}

LRESULT CDlgTrigger::OnDeleteSubTree_ForMsg(WPARAM wParam, LPARAM lParam)
{
	if (!m_pvtCategory) return E_FAIL;

	try
	{
		CCategory *pCategory = FindCurrentCategory(m_TriggerTree.GetSelectedItem());
		CBaseTrigger *pBaseTrigger = FindCurrentTrigger(m_TriggerTree.GetSelectedItem());
		if (pCategory)
		{
			DeleteCategory(pCategory->m_hItem);
		}
		else if (pBaseTrigger)
		{
			DeleteTrigger(pBaseTrigger->m_hItem);
		}
	}
	catch (...)
	{
		MessageBox(L"범주 및 트리거 삭제중 예외가 발생되었습니다.(OnDeleteSubTree_ForMsg)", L"예외", MB_OK);
	}

	return S_OK;
}

LRESULT CDlgTrigger::OnChangeDialog(WPARAM wParam, LPARAM lParam)
{
	try
	{
		switch ((LEVEL_TYPE)m_TriggerTree.GetLevelTree(m_TriggerTree.GetSelectedItem()))
		{
		case LEVEL_CATEGORY:
			m_dlgTriggerContent.ShowWindow(SW_HIDE);
			break;
		case LEVEL_TG:
			m_dlgTriggerContent.ShowWindow(SW_SHOW);
			m_dlgTriggerContent.SendMessage(WM_SELECT_TRIGGER, (WPARAM)FindCurrentTrigger(m_TriggerTree.GetSelectedItem()), 0);
			break;
		}
	}
	catch (...)
	{
		MessageBox(L"트리거내용 창 갱신중 예외가 발생되었습니다.(OnChangeDialog)", L"예외", MB_OK);
	}

	return S_OK;
}

LRESULT CDlgTrigger::OnEnableMessage(WPARAM wParam, LPARAM lParam)
{
	try
	{
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_POPUP_TRIGGER));
		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		if (pPopup) pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, (int)wParam, (int)lParam, this);
	}
	catch (...)
	{
		MessageBox(L"트리거 창에 팝업을 띄우는 도중 예외가 발생되었습니다.(OnEnableMessage)", L"예외", MB_OK);
	}

	return S_OK;
}

void CDlgTrigger::OnTgAddCategory()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_TriggerTree.GetRootItem())
	{
		m_TriggerTree.Select(m_TriggerTree.GetRootItem(), TVGN_CARET, 0);
		OnAddSibling_SpaceBar_ForMsg(0, 0);
	}
	else
	{
		OnAddchild_Enter_ForMsg(0, 0);
	}
}

void CDlgTrigger::OnTgAddTrigger()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_TriggerTree.GetRootItem())
	{
		m_TriggerTree.Select(m_TriggerTree.GetRootItem(), TVGN_CARET, 1);
		OnAddchild_Enter_ForMsg(0, 0);
	}
	else
	{
		MessageBox(L"트리거를 추가할 범주를 먼저 만들어 주세요.", L"알림", MB_OK);
	}
}

void CDlgTrigger::OnTgCut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnTreeCut();
}

void CDlgTrigger::OnTgCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnTreeCopy();
}

void CDlgTrigger::OnTgPaste()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnTreePaste();
}

void CDlgTrigger::OnTgDelete()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnDeleteSubTree_ForMsg(0, 0);
}

void CDlgTrigger::OnTgUse()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CDlgTrigger::OnTgStartWithUse()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

CCategory *CDlgTrigger::FindCurrentCategory(HTREEITEM hFItem)
{
	if (!hFItem || !m_pvtCategory) return NULL;

	for (int i = 0; i < (int)m_pvtCategory->size(); ++i)
	{
		if (hFItem == (*m_pvtCategory)[i]->m_hItem)
			return (*m_pvtCategory)[i];
	}

	return NULL;
}

CBaseTrigger *CDlgTrigger::FindCurrentTrigger(HTREEITEM hFItem)
{
	if (!hFItem || !m_pvtCategory) return NULL;

	for (int i = 0; i < (int)m_pvtCategory->size(); ++i)
	{
		for (int j = 0; j < (int)(*m_pvtCategory)[i]->m_vtBaseTrigger.size(); ++j)
		{
			if ((*m_pvtCategory)[i]->m_vtBaseTrigger[j]->m_hItem == hFItem)
				return (*m_pvtCategory)[i]->m_vtBaseTrigger[j];
		}
	}

	return NULL;
}

LRESULT CDlgTrigger::OnChangedTreeLabel(WPARAM wParam, LPARAM lParam)
{
	if (!wParam) return E_FAIL;
	CEdit *pEdit = (CEdit*)wParam;
	CString strReNameText;
	pEdit->GetWindowText(strReNameText);

	const int iReNameLenth = strReNameText.GetLength();

	HTREEITEM hSelItem = (HTREEITEM)lParam;

	if (iReNameLenth >= MAX_TG_STR_LENGTH)
	{
		CString strMsg;
		strMsg.Format(L"트리거명의 길이가 최대치(%d)를 넘었습니다. 트리거명을 저장 할 수 없습니다.", MAX_TG_STR_LENGTH);
		MessageBox(strMsg, L"알림", MB_OK);
		return E_FAIL;
	}
	else if (iReNameLenth <= 0)
	{
		MessageBox(L"변경할 이름을 적어주세요.", L"알림", MB_OK);
		return E_FAIL;
	}
	else
	{
		switch (m_TriggerTree.GetLevelTree(hSelItem))
		{
		case LEVEL_CATEGORY:
		{
			CCategory *pCategory = FindCurrentCategory(hSelItem);
			if (pCategory)
				pCategory->m_strName = strReNameText;
		}
		break;
		case LEVEL_TG:
		{
			CBaseTrigger *pBaseTrigger = FindCurrentTrigger(hSelItem);
			CQuestScripterDlg *pScripterDlg = (CQuestScripterDlg*)AfxGetMainWnd();
			if (pBaseTrigger && pScripterDlg)
			{
				// 전체 트리거의 이름 중복 체크.
				if (!pScripterDlg->FindEqualTriggerName(strReNameText.GetBuffer()))        // 이름 중복이 없다면 이름을 바꾼다.
				{
					pScripterDlg->ChangeTGOnOffName(strReNameText.GetBuffer(), pBaseTrigger->m_strTGName);
					pBaseTrigger->m_strTGName = strReNameText;
				}
				else
				{
					MessageBox(L"중복된 트리거 명이 있습니다.", L"알림", MB_OK);
					return E_FAIL;
				}
				m_dlgTriggerContent.SendMessage(WM_SELECT_TRIGGER, (WPARAM)pBaseTrigger, 0);
			}
		}
		break;
		}
	}

	return S_OK;
}

void CDlgTrigger::RefreshTriggerTree(std::wstring strSelect)
{
	if (!m_pvtCategory) return;

	try
	{
		m_TriggerTree.DeleteAllItems();

		HTREEITEM hSelectItem = NULL;
		if (m_pvtCategory->size())
		{
			for (int i = 0; i < (int)m_pvtCategory->size(); ++i)
			{
				(*m_pvtCategory)[i]->m_hItem = m_TriggerTree.AddSibling((*m_pvtCategory)[i]->m_strName.c_str());
				for (int j = 0; j < (int)(*m_pvtCategory)[i]->m_vtBaseTrigger.size(); j++)
				{
					m_TriggerTree.Select((*m_pvtCategory)[i]->m_hItem, TVGN_CARET, 0);
					(*m_pvtCategory)[i]->m_vtBaseTrigger[j]->m_hItem = m_TriggerTree.AddChild((*m_pvtCategory)[i]->m_vtBaseTrigger[j]->m_strTGName.c_str());
					if (strSelect == (*m_pvtCategory)[i]->m_vtBaseTrigger[j]->m_strTGName)
						hSelectItem = (*m_pvtCategory)[i]->m_vtBaseTrigger[j]->m_hItem;
				}
				m_TriggerTree.Select((*m_pvtCategory)[i]->m_hItem, TVGN_CARET, 0);
			}
		}
		else
		{
			m_dlgTriggerContent.ShowWindow(SW_HIDE);
		}
		if (NULL == hSelectItem)
			hSelectItem = m_TriggerTree.GetRootItem();
		m_TriggerTree.Select(hSelectItem, TVGN_CARET, 1);
	}
	catch (...)
	{
		MessageBox(L"트리거창 갱신중 오류가 발생하였습니다.(RefreshTriggerTree)", L"예외", MB_OK);
	}
}

void CDlgTrigger::OnTreeCut(void)
{
	HTREEITEM hSelItem = m_TriggerTree.GetSelectedItem();
	int iLevel = m_TriggerTree.GetLevelTree(hSelItem);

	switch ((LEVEL_TYPE)iLevel)
	{
	case LEVEL_TG:
	{
		m_BaseTrigger_ClipBoard.ClearTrigger();
		CBaseTrigger *pSelBaseTrigger = FindCurrentTrigger(hSelItem);
		if (pSelBaseTrigger)
		{
			m_BaseTrigger_ClipBoard = *pSelBaseTrigger;
			DeleteTrigger(hSelItem);

			m_bClipBoard = true;
			m_iCopyCnt = 0;
		}
	}
	break;
	}
}

void CDlgTrigger::OnTreeCopy()
{
	TreeCopy(NULL);
}

void CDlgTrigger::OnTreePaste()
{
	TreePaste(NULL, NULL, NULL, false);
}

void CDlgTrigger::DeleteTrigger(HTREEITEM hItem)
{
	if (!m_pvtCategory) return;

	try
	{
		for (int i = 0; i < (int)m_pvtCategory->size(); ++i)
		{
			for (int j = 0; j < (int)(*m_pvtCategory)[i]->m_vtBaseTrigger.size(); j++)
			{
				if ((*m_pvtCategory)[i]->m_vtBaseTrigger[j]->m_hItem == hItem)
				{
					// TriggerContent 창에 선택된 트리거를 지웠다면 그 정보도 삭제한다.
					if (m_dlgTriggerContent.m_pCurrentTrigger)
					{
						if (m_dlgTriggerContent.m_pCurrentTrigger->m_hItem == hItem)
							m_dlgTriggerContent.SendMessage(WM_SELECT_TRIGGER, 0, 0);       // 트리거의 포인터로 NULL을 보내 초기화 시킴
					}

					SAFE_DELETE((*m_pvtCategory)[i]->m_vtBaseTrigger[j]);
					(*m_pvtCategory)[i]->m_vtBaseTrigger.erase((*m_pvtCategory)[i]->m_vtBaseTrigger.begin() + j);
					m_TriggerTree.DeleteItem(hItem);
					m_TriggerTree.Select(m_TriggerTree.GetSelectedItem(), TVGN_CARET, 1);
					break;
				}
			}
		}
	}
	catch (...)
	{
		MessageBox(L"트리거 정보 제거중 예외가 발생되었습니다.(DeleteTrigger)", L"예외", MB_OK);
	}
}

void CDlgTrigger::DeleteCategory(HTREEITEM hItem)
{
	if (!m_pvtCategory) return;

	try
	{
		for (int i = 0; i < (int)m_pvtCategory->size(); ++i)
		{
			if ((*m_pvtCategory)[i]->m_hItem == hItem)
			{
				// TriggerContent(트리거내용) 정보를 모두 삭제한다.
				if (m_dlgTriggerContent.m_pCurrentTrigger)
				{
					HTREEITEM hSelParent = m_TriggerTree.GetParentItem(hItem);
					if (m_dlgTriggerContent.m_pCurrentTrigger->m_hItem == hSelParent)
						m_dlgTriggerContent.SendMessage(WM_SELECT_TRIGGER, 0, 0);       // 트리거의 포인터로 NULL을 보내 초기화 시킴
				}

				SAFE_DELETE((*m_pvtCategory)[i]);
				m_pvtCategory->erase(m_pvtCategory->begin() + i);
				m_TriggerTree.DeleteItem(hItem);
				break;
			}
		}
	}
	catch (...)
	{
		MessageBox(L"범주 정보 제거중 예외가 발생되었습니다.(DeleteCategory)", L"예외", MB_OK);
	}
}

BOOL CDlgTrigger::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_hAccelTable != NULL)
	{
		if (TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgTrigger::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CQuestScripterDlg *pDlgScript = (CQuestScripterDlg*)AfxGetMainWnd();
	if (!pDlgScript || !m_pDlgStage) return;

	m_pDlgStage->ClearCategorys(&m_pDlgStage->m_vtCategory);
	m_pDlgStage->CopyToCategorys(&m_pDlgStage->m_vtCategory, pDlgScript->m_dlgTrigger.m_pvtCategory);

	pDlgScript->SaveSelectedBefore();
	pDlgScript->m_Datamanager.AddQuest(&pDlgScript->m_Quest);

	//m_pDlgStage->EnableWindow(true);
	m_pDlgStage = NULL;
	ShowWindow(SW_HIDE);
}

void CDlgTrigger::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (IsWindowVisible())
	{
		if (m_pvtCategory)
		{
			bool bActiveTrigger = false;
			for (int i = 0; i < (int)m_pvtCategory->size(); ++i)
			{
				for (int j = 0; j < (int)(*m_pvtCategory)[i]->m_vtBaseTrigger.size(); ++j)
				{
					if ((*m_pvtCategory)[i]->m_vtBaseTrigger[j]->m_bActive)
					{
						bActiveTrigger = true;
						break;
					}
				}
			}
			if (!bActiveTrigger)
			{
				if (IDNO == MessageBox(L"초기사용할 트리거를 설정 하지 않았습니다. 그대로 종료하시겠습니까?", L"알림", MB_YESNO))
					return;
			}
		}
	}

	CQuestScripterDlg *pDlgScript = (CQuestScripterDlg*)AfxGetMainWnd();
	if (!pDlgScript || !m_pDlgStage) return;

	m_pDlgStage->ClearCategorys(&m_pDlgStage->m_vtCategory);
	m_pDlgStage->CopyToCategorys(&m_pDlgStage->m_vtCategory, pDlgScript->m_dlgTrigger.m_pvtCategory);

	pDlgScript->SaveSelectedBefore();
	pDlgScript->m_Datamanager.AddQuest(&pDlgScript->m_Quest);

	//m_pDlgStage->EnableWindow(true);
	m_pDlgStage = NULL;
	ShowWindow(SW_HIDE);
}

void CDlgTrigger::SetCallWindowPointer(CDlgStage *pDlgStage)
{
	if (!pDlgStage) return;
	//pDlgStage->EnableWindow(false);
	m_pDlgStage = pDlgStage;
}

BOOL CDlgTrigger::ShowWindow(int nCmdShow)
{
	bool bShow = CDialog::ShowWindow(nCmdShow);

	if (AfxGetMainWnd())
		AfxGetMainWnd()->PostMessage(WM_FOCUS_WINDOW, 0, 0);

	return bShow;
}

LRESULT CDlgTrigger::OnTreeCustomDraw(WPARAM wParam, LPARAM lParam)
{
	NMTVCUSTOMDRAW *pcd = (NMTVCUSTOMDRAW*)(wParam);
	if (!pcd) return E_FAIL;

	HTREEITEM hItem = NULL;
	switch (pcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		break;
	case CDDS_ITEMPREPAINT:
	{
		hItem = (HTREEITEM)pcd->nmcd.dwItemSpec;
		CBaseTrigger *pBaseTrigger = FindCurrentTrigger(hItem);
		if (!hItem || !pBaseTrigger) return E_FAIL;

		pcd->clrText = RGB(0, 0, 0);

		if (pBaseTrigger->m_bActive)
			pcd->clrText = RGB(228, 120, 51);

		if (!pBaseTrigger->m_bUse)
			pcd->clrText = RGB(205, 205, 205);
	}
	break;
	}

	return S_OK;
}

LRESULT CDlgTrigger::OnTreeReDrawTextToSelItem(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hSelItem = m_TriggerTree.GetSelectedItem();
	CBaseTrigger *pBaseTrigger = FindCurrentTrigger(hSelItem);
	if (hSelItem && pBaseTrigger)
		m_TriggerTree.SetItemText(hSelItem, pBaseTrigger->m_strTGName.c_str());

	return S_OK;
}

std::wstring CDlgTrigger::MoveItem(WORD wKey)
{
	std::wstring strSelect;
	HTREEITEM hSelect = m_TriggerTree.GetSelectedItem();
	if (NULL == hSelect)
		return strSelect;

	if (wKey == VK_UP)
	{
		HTREEITEM hTemp = m_TriggerTree.GetPrevSiblingItem(hSelect);
		if (hTemp)
		{
			TreeCopyAndPaste(hTemp, m_TriggerTree.GetParentItem(hSelect), hSelect);
			CCategory *pCategory = FindCurrentCategory(hTemp);
			CBaseTrigger *pBaseTrigger = FindCurrentTrigger(hTemp);
			if (pCategory)
			{
				DeleteCategory(pCategory->m_hItem);
			}
			else if (pBaseTrigger)
			{
				DeleteTrigger(pBaseTrigger->m_hItem);
			}

			pCategory = FindCurrentCategory(hSelect);
			pBaseTrigger = FindCurrentTrigger(hSelect);
			if (pCategory)
			{
				strSelect = pCategory->m_strName.c_str();
			}
			else if (pBaseTrigger)
			{
				strSelect = pBaseTrigger->m_strTGName.c_str();
			}
		}
	}
	else if (wKey == VK_DOWN)
	{
		HTREEITEM hTemp = m_TriggerTree.GetNextSiblingItem(hSelect);
		if (hTemp)
		{
			HTREEITEM  hNext = TreeCopyAndPaste(hSelect, m_TriggerTree.GetParentItem(hTemp), hTemp);
			CCategory *pCategory = FindCurrentCategory(hSelect);
			CBaseTrigger *pBaseTrigger = FindCurrentTrigger(hSelect);
			if (pCategory)
			{
				DeleteCategory(pCategory->m_hItem);
			}
			else if (pBaseTrigger)
			{
				DeleteTrigger(pBaseTrigger->m_hItem);
			}

			pCategory = FindCurrentCategory(hNext);
			pBaseTrigger = FindCurrentTrigger(hNext);
			if (pCategory)
			{
				strSelect = pCategory->m_strName.c_str();
			}
			else if (pBaseTrigger)
			{
				strSelect = pBaseTrigger->m_strTGName.c_str();
			}
		}
	}

	return strSelect;
}

HTREEITEM CDlgTrigger::TreeCopyAndPaste(HTREEITEM hSelItem, HTREEITEM hParent, HTREEITEM hDest)
{
	TreeCopy(hSelItem);
	return TreePaste(hSelItem, hParent, hDest, true);
}

void CDlgTrigger::TreeCopy(HTREEITEM hSelItem)
{
	if (NULL == hSelItem)
		hSelItem = m_TriggerTree.GetSelectedItem();
	int iLevel = m_TriggerTree.GetLevelTree(hSelItem);

	switch ((LEVEL_TYPE)iLevel)
	{
	case LEVEL_TG:
	{
		m_BaseTrigger_ClipBoard.ClearTrigger();
		CBaseTrigger *pSelBaseTrigger = FindCurrentTrigger(hSelItem);
		if (pSelBaseTrigger)
		{
			m_BaseTrigger_ClipBoard = *pSelBaseTrigger;
			m_bClipBoard = true;
			m_iCopyCnt = 0;
		}
	}
	break;
	}
}

HTREEITEM CDlgTrigger::TreePaste(HTREEITEM hSelItem, HTREEITEM hParent, HTREEITEM hDest, bool bNoAddName)
{
	if (!m_bClipBoard) return NULL;

	HTREEITEM hNewItem = NULL;

	if (NULL == hSelItem)
		hSelItem = m_TriggerTree.GetSelectedItem();
	int iLevel = m_TriggerTree.GetLevelTree(hSelItem);

	switch ((LEVEL_TYPE)iLevel)
	{
	case LEVEL_CATEGORY:
	{
		CCategory *pCategory = FindCurrentCategory(hSelItem);
		if (pCategory)
		{
			CBaseTrigger *pBaseTrigger = new CBaseTrigger;
			*pBaseTrigger = m_BaseTrigger_ClipBoard;
			CString strTGName = pBaseTrigger->m_strTGName.c_str();
			CQuestScripterDlg *pDlgScript = (CQuestScripterDlg*)AfxGetMainWnd();
			if (!pDlgScript) return NULL;
			if (false == bNoAddName)
			{
				do
				{
					strTGName.Format(L"%s 복사 %d", pBaseTrigger->m_strTGName.c_str(), m_iCopyCnt++);
				} while (NULL != pDlgScript->FindTriggerByName(strTGName.GetBuffer()));
			}
			pBaseTrigger->m_strTGName = strTGName.GetBuffer();
			pBaseTrigger->m_hItem = m_TriggerTree.AddChild(pBaseTrigger->m_strTGName.c_str());
			if (NULL == hDest)
				pCategory->AddTrigger(pBaseTrigger);
			else
				pCategory->AddTriggerToNextItem(pBaseTrigger, hDest);
			m_TriggerTree.Select(pBaseTrigger->m_hItem, TVGN_CARET, 1);
			hNewItem = pBaseTrigger->m_hItem;
		}
	}
	break;
	case LEVEL_TG:
	{
		HTREEITEM hSelParentItem;
		if (NULL == hParent)
			hSelParentItem = m_TriggerTree.GetParentItem(hSelItem);
		else
			hSelParentItem = hParent;
		CCategory *pCategory = FindCurrentCategory(hSelParentItem);

		if (pCategory)
		{
			CBaseTrigger *pBaseTrigger = new CBaseTrigger;
			*pBaseTrigger = m_BaseTrigger_ClipBoard;
			CString strTGName = pBaseTrigger->m_strTGName.c_str();
			CQuestScripterDlg *pDlgScript = (CQuestScripterDlg*)AfxGetMainWnd();
			if (!pDlgScript) return NULL;
			if (false == bNoAddName)
			{
				do
				{
					strTGName.Format(L"%s 복사 %d", pBaseTrigger->m_strTGName.c_str(), m_iCopyCnt++);
				} while (NULL != pDlgScript->FindTriggerByName(strTGName.GetBuffer()));
			}
			pBaseTrigger->m_strTGName = strTGName.GetBuffer();
			pBaseTrigger->m_hItem = m_TriggerTree.AddSibling(pBaseTrigger->m_strTGName.c_str());
			if (NULL == hDest)
				pCategory->AddTrigger(pBaseTrigger);
			else
				pCategory->AddTriggerToNextItem(pBaseTrigger, hDest);
			m_TriggerTree.Select(pBaseTrigger->m_hItem, TVGN_CARET, 1);
			hNewItem = pBaseTrigger->m_hItem;
		}
	}
	break;
	}

	return hNewItem;
}