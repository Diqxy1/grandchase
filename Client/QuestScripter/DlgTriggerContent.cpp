#include "stdafx.h"
#include "QuestScripter.h"
#include "dlgTriggerContent.h"
#include ".\dlgtriggercontent.h"

//************************************************************************
// 이벤트, 액션
//************************************************************************
#include "Win.h"
#include "Lose.h"
#include "MonsterGenerater.h"
#include "SoundOnOff.h"
#include "Loading.h"
#include "TriggerOnOff.h"
#include "Reward.h"
#include "NextStage.h"
#include "SuperPlayer.h"
#include "ClearOfStage.h"
#include "PlayerPosition.h"
#include "ObjectGenerater.h"
#include "OpenDGate.h"
#include "TitleText.h"
#include "TriggerReady.h"
#include "Dialogue.h"
#include "DeleteMonster.h"
#include "FadeInOut.h"
#include "HP.h"
#include "MP.h"
#include "PlayerMotion.h"
#include "PositionParticle.h"
#include "UnitMovement.h"
#include "UnitParticle.h"
#include "PlayerMovingToArea.h"
#include "GateObjectGenerater.h"

//************************************************************************
// 조건
//************************************************************************
#include "KillMaster.h"
#include "Timer.h"
#include "Position.h"

#include "DlgTGWin.h"
#include "DlgTGLose.h"
#include "DlgTGMonsterGenerater.h"
#include "DlgTGSoundOnOff.h"
#include "DlgTGLoading.h"
#include "DlgTriggerOnOff.h"
#include "DlgTGKillMaster.h"
#include "DlgTGTimer.h"
#include "DlgTGPosition.h"
#include "DlgTGReward.h"
#include "DlgTGNextStage.h"

// CdlgTriggerContent 대화 상자입니다.

IMPLEMENT_DYNAMIC(CdlgTriggerContent, CDialog)
CdlgTriggerContent::CdlgTriggerContent(CWnd* pParent /*=NULL*/)
	: CDialog(CdlgTriggerContent::IDD, pParent)
{
	m_bCreated = false;
	m_pCurrentTrigger = NULL;
	m_hEvent = NULL;
	m_hCondition = NULL;
	m_hAction = NULL;
	m_hAccelTable = NULL;
	m_vtEAction_ClipBoard.clear();
	m_vtCondition_ClipBoard.clear();
	m_hCurSelItem = NULL;
}

CdlgTriggerContent::~CdlgTriggerContent()
{
	for (int i = 0; i < (int)m_vtEAction_ClipBoard.size(); ++i)
		SAFE_DELETE(m_vtEAction_ClipBoard[i]);
	for (int i = 0; i < (int)m_vtCondition_ClipBoard.size(); ++i)
		SAFE_DELETE(m_vtCondition_ClipBoard[i]);

	m_vtEAction_ClipBoard.clear();
	m_vtCondition_ClipBoard.clear();
}

void CdlgTriggerContent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_COMMENT, m_stTGComment);
	DDX_Control(pDX, IDC_STATIC_TRIGGER, m_stTGContent);
	DDX_Control(pDX, IDC_EDIT_TG_COMMENT, m_ebTG_Comment);
	DDX_Control(pDX, IDC_TREE_TG, m_treeTG);
	DDX_Control(pDX, IDC_CHECK_USE, m_btUse);
	DDX_Control(pDX, IDC_CHECK_ACTIVE, m_btActive);
	m_ebTG_Comment.SetTextColor(RGB(70, 150, 70));
	m_ebTG_Comment.SetBackColor(RGB(255, 255, 255));
}

BEGIN_MESSAGE_MAP(CdlgTriggerContent, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_SIZE()
	ON_MESSAGE(WM_ADDCHILDQ, OnAddchildQ_ForMsg)
	ON_MESSAGE(WM_ADDSIBLINGQ, OnAddSiblingQ_ForMsg)
	ON_MESSAGE(WM_SHIFT_SELECT, OnShiftTreeSelect)
	ON_MESSAGE(WM_DELETESUBTREE, OnDeleteSubTree_ForMsg)
	ON_MESSAGE(WM_CHANG_DIALOG, OnChangeDialog)
	ON_MESSAGE(WM_MENU_ENABLE, OnEnableMessage)
	ON_COMMAND(ID_TGCT_ADD_POPUP_EVENT, OnTgctAddPopupEvent)
	ON_COMMAND(ID_TGCT_ADD_POPUP_CONDITION, OnTgctAddPopupCondition)
	ON_COMMAND(ID_TGCT_ADD_POPUP_ACTION, OnTgctAddPopupAction)
	ON_MESSAGE(WM_ADD_EVENT, OnTgctAddEvent)
	ON_MESSAGE(WM_ADD_CONDITION, OnTgctAddCondition)
	ON_MESSAGE(WM_ADD_ACTION, OnTgctAddAction)
	ON_MESSAGE(WM_MODIFY_EVENT, OnTgctModifyEvent)
	ON_MESSAGE(WM_MODIFY_CONDITION, OnTgctModifyCondition)
	ON_MESSAGE(WM_MODIFY_ACTION, OnTgctModifyAction)
	ON_COMMAND(ID_TGCT_CUT, OnTgctCut)
	ON_COMMAND(ID_TGCT_COPY, OnTgctCopy)
	ON_COMMAND(ID_TGCT_PASTE, OnTgctPaste)
	ON_COMMAND(ID_TGCT_DELETE, OnTgctDelete)
	ON_COMMAND(ID_TGCT_USE, OnTgctUse)
	ON_COMMAND(ID_TGCT_START_WITH_USE, OnTgctStartWithUse)
	ON_MESSAGE(WM_SELECT_TRIGGER, OnSelectTrigger)
	ON_MESSAGE(WM_TREE_ITEM_DCLICK, OnTreeItemDClick)
	ON_EN_CHANGE(IDC_EDIT_TG_COMMENT, OnEnChangeEditTgComment)
	ON_COMMAND(ID_TREE_CUT, OnTreeCut)
	ON_COMMAND(ID_TREE_COPY, OnTreeCopy)
	ON_COMMAND(ID_TREE_PASTE, OnTreePaste)
	ON_BN_CLICKED(IDC_CHECK_USE, OnBnClickedCheckUse)
	ON_BN_CLICKED(IDC_CHECK_ACTIVE, OnBnClickedCheckActive)
	ON_MESSAGE(WM_TREE_NMCLICK, OnNMTreeClick)
	ON_MESSAGE(WM_TREE_SELCHANGE, OnTreeSelChange)
	ON_MESSAGE(WM_TREE_CUSTOMMDRAW, OnTreeCustomDraw)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CdlgTriggerContent 메시지 처리기입니다.

void CdlgTriggerContent::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//    OnOK();
}

void CdlgTriggerContent::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//    OnCancel();
}

BOOL CdlgTriggerContent::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CRect rc;
	CWnd* pWnd;

	pWnd = GetDlgItem(IDC_STATIC_SPLITTER2);
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndSplitter1.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_STATIC_SPLITTER2);

	CRect rtClient;
	GetClientRect(rtClient);
	m_wndSplitter1.SetRange(rtClient.top + 100, rtClient.bottom - 100);

	//************************************************************************
	// 단축키를 사용하기 위해 단축키 정보를 불러온다.
	m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR_TG_CONTENT));
	//************************************************************************

	m_bCreated = true;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

LRESULT CdlgTriggerContent::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (message == WM_NOTIFY)
	{
		if (wParam == IDC_STATIC_SPLITTER2)
		{
			SPC_NMHDR* pHdr = (SPC_NMHDR*)lParam;
			DoResize1(pHdr->delta);
		}
	}

	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CdlgTriggerContent::DoResize1(int delta)
{
	CSplitterControl::ChangeHeight(&m_ebTG_Comment, delta);
	CSplitterControl::ChangeHeight(&m_treeTG, -delta, CW_BOTTOMALIGN);
	//CSplitterControl::ChangeHeight(&m_stTGContent, -delta, CW_BOTTOMALIGN);

	CRect rtTGContent;
	m_stTGContent.GetWindowRect(rtTGContent);
	ScreenToClient(rtTGContent);
	rtTGContent.OffsetRect(0, delta);
	m_stTGContent.MoveWindow(rtTGContent, 1);

	Invalidate();
	UpdateWindow();
}

void CdlgTriggerContent::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_bCreated)
	{
		CRect rtClient;
		GetClientRect(rtClient);

		m_wndSplitter1.SetRange(rtClient.top + 100, rtClient.bottom - 100);
		CRect rtSplit;
		m_wndSplitter1.GetWindowRect(rtSplit);
		ScreenToClient(rtSplit);
		rtSplit.right = cx;
		m_wndSplitter1.MoveWindow(rtSplit, 1);

		CRect rtTree;
		m_treeTG.GetWindowRect(rtTree);
		ScreenToClient(rtTree);
		rtTree.bottom = cy;
		rtTree.right = cx;
		m_treeTG.MoveWindow(rtTree, 1);

		CRect rtComment;
		m_ebTG_Comment.GetWindowRect(rtComment);
		ScreenToClient(rtComment);
		rtComment.right = cx;
		m_ebTG_Comment.MoveWindow(rtComment, 1);

		m_wndSplitter1.Invalidate(1);
	}
}

LRESULT CdlgTriggerContent::OnAddchildQ_ForMsg(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

LRESULT CdlgTriggerContent::OnAddSiblingQ_ForMsg(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

// 부모 구조로 이동하는 것임.
LRESULT CdlgTriggerContent::OnShiftTreeSelect(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hSelItem = NULL;
	switch (wParam)
	{
	case VK_UP:
		MoveItem(VK_UP);
		break;
	case VK_DOWN:
		MoveItem(VK_DOWN);
		break;
	case VK_LEFT:
		hSelItem = m_treeTG.GetPrevSiblingItem(m_treeTG.GetSelectedItem());
		break;
	case VK_RIGHT:
		hSelItem = m_treeTG.GetNextSiblingItem(m_treeTG.GetSelectedItem());
		break;
	}

	if (hSelItem)
		m_treeTG.Select(hSelItem, TVGN_CARET);

	return S_OK;
}

LRESULT CdlgTriggerContent::OnDeleteSubTree_ForMsg(WPARAM wParam, LPARAM lParam)
{
	try
	{
		HTREEITEM hSelItem = m_treeTG.GetSelectedItem();
		if (hSelItem)
		{
			int iLevel = m_treeTG.GetLevelTree(hSelItem);
			if (iLevel <= 2) return E_FAIL;
			if (!m_pCurrentTrigger) return E_FAIL;

			if (DelEventItem(hSelItem)) {}
			else if (DelActionItem(hSelItem)) {}
			else if (DelConditionItem(hSelItem)) {}
		}
	}
	catch (...)
	{
		MessageBox(L"트리거내용의 트리 삭제중 예외가 발생되었습니다.(OnDeleteSubTree_ForMsg)", L"예외", MB_OK);
	}

	return S_OK;
}

LRESULT CdlgTriggerContent::OnChangeDialog(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

LRESULT CdlgTriggerContent::OnEnableMessage(WPARAM wParam, LPARAM lParam)
{
	try
	{
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_POPUP_TG_CONTENT));
		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		if (pPopup) pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, (int)wParam, (int)lParam, this);
	}
	catch (...)
	{
		MessageBox(L"트리거내용에서 팝업창을 띄우던 중 예외가 발생되었습니다.(OnEnableMessage)", L"예외", MB_OK);
	}

	return S_OK;
}

LRESULT CdlgTriggerContent::OnSelectTrigger(WPARAM wParam, LPARAM lParam)
{
	try
	{
		m_pCurrentTrigger = NULL;
		m_hEvent = NULL;
		m_hCondition = NULL;
		m_hAction = NULL;
		m_treeTG.DeleteAllItems();

		if (!wParam) return E_FAIL;
		CBaseTrigger *pBaseTrigger = (CBaseTrigger*)wParam;
		m_pCurrentTrigger = pBaseTrigger;

		HTREEITEM hTrigger = m_treeTG.AddChild(pBaseTrigger->m_strTGName.c_str());

		m_treeTG.Select(hTrigger, TVGN_CARET, 0);
		m_hEvent = m_treeTG.AddChild(L"Event");
		m_treeTG.Select(hTrigger, TVGN_CARET, 0);
		m_hCondition = m_treeTG.AddChild(L"Condition");
		m_treeTG.Select(hTrigger, TVGN_CARET, 0);
		m_hAction = m_treeTG.AddChild(L"Action");

		for (int i = 0; i < (int)(int)pBaseTrigger->m_vtEvent.size(); ++i)
		{
			m_treeTG.Select(m_hEvent, TVGN_CARET, 0);
			pBaseTrigger->m_vtEvent[i]->m_hItem = m_treeTG.AddChild(pBaseTrigger->m_vtEvent[i]->m_strDesc.c_str());
		}
		for (int i = 0; i < (int)(int)pBaseTrigger->m_vtCondition.size(); ++i)
		{
			m_treeTG.Select(m_hCondition, TVGN_CARET, 0);
			pBaseTrigger->m_vtCondition[i]->m_hItem = m_treeTG.AddChild(pBaseTrigger->m_vtCondition[i]->m_strDesc.c_str());
		}
		for (int i = 0; i < (int)(int)pBaseTrigger->m_vtAction.size(); ++i)
		{
			m_treeTG.Select(m_hAction, TVGN_CARET, 0);
			pBaseTrigger->m_vtAction[i]->m_hItem = m_treeTG.AddChild(pBaseTrigger->m_vtAction[i]->m_strDesc.c_str());
		}

		m_btUse.SetCheck(pBaseTrigger->m_bUse);
		m_btActive.SetCheck(pBaseTrigger->m_bActive);

		CString strTGComment = pBaseTrigger->m_strTGComment.c_str();
		strTGComment.Replace(L"/r/n", L"\r\n");
		m_ebTG_Comment.SetWindowText(strTGComment);
		m_treeTG.Select(m_treeTG.GetRootItem(), TVGN_CARET, 0);
	}
	catch (...)
	{
		MessageBox(L"트리거내용에 트리거의 정보를 저장하던 중 예외가 발생되었습니다.(OnSelectTrigger)", L"예외", MB_OK);
	}

	return S_OK;
}

LRESULT CdlgTriggerContent::OnTgctAddEvent(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (!m_pCurrentTrigger) return E_FAIL;

	try
	{
		//CDlgTGChildGenner dlgTGChildGenner(TG_TYPE_EACTION, TG_EACTION_LOADING, TG_CONDITION_DEFAULT);
		//if( IDOK == dlgTGChildGenner.DoModal() )
		if (CDlgTGChildGenner::CREATE_MODE == m_dlgTGChildGenner.m_Mode
			&& TG_EACTION_DEFAULT != m_dlgTGChildGenner.m_enAction
			)
		{
			CBaseEventAction *pEACtion = m_dlgTGChildGenner.GetCurEAction();
			if (!pEACtion) return E_FAIL;

			m_treeTG.Select(m_hEvent, TVGN_CARET, 0);
			pEACtion->m_hItem = m_treeTG.AddChild(pEACtion->m_strDesc.c_str());
			// Add Event Function.
			m_pCurrentTrigger->m_vtEvent.push_back(pEACtion);
		}
	}
	catch (...)
	{
		MessageBox(L"이벤트 추가중 예외가 발생되었습니다.(OnTgctAddEvent)", L"예외", MB_OK);
	}

	return S_OK;
}

LRESULT CdlgTriggerContent::OnTgctAddCondition(WPARAM wParam, LPARAM lParam)
{
	if (!m_pCurrentTrigger) return E_FAIL;

	try
	{
		//CDlgTGChildGenner dlgTGChildGenner(TG_TYPE_CONDITION, TG_EACTION_DEFAULT, TG_CONDITION_KILLMASTER);
		//if( IDOK == dlgTGChildGenner.DoModal() )
		if (CDlgTGChildGenner::CREATE_MODE == m_dlgTGChildGenner.m_Mode
			&& TG_CONDITION_DEFAULT != m_dlgTGChildGenner.m_enCondition
			)
		{
			CBaseCondition *pCondition = m_dlgTGChildGenner.GetCurCondition();
			if (!pCondition) return E_FAIL;

			m_treeTG.Select(m_hCondition, TVGN_CARET, 0);
			pCondition->m_hItem = m_treeTG.AddChild(pCondition->m_strDesc.c_str());
			// Add Condition Function
			m_pCurrentTrigger->m_vtCondition.push_back(pCondition);
		}
	}
	catch (...)
	{
		MessageBox(L"컨디션 추가중 예외가 발생되었습니다.(OnTgctAddCondition)", L"예외", MB_OK);
	}

	return S_OK;
}

LRESULT CdlgTriggerContent::OnTgctAddAction(WPARAM wParam, LPARAM lParam)
{
	if (!m_pCurrentTrigger) return E_FAIL;

	try
	{
		//CDlgTGChildGenner dlgTGChildGenner(TG_TYPE_EACTION, TG_EACTION_LOADING, TG_CONDITION_DEFAULT);
		//if( IDOK == dlgTGChildGenner.DoModal() )
		if (CDlgTGChildGenner::CREATE_MODE == m_dlgTGChildGenner.m_Mode
			&& TG_EACTION_DEFAULT != m_dlgTGChildGenner.m_enAction
			)
		{
			CBaseEventAction *pEACtion = m_dlgTGChildGenner.GetCurEAction();
			if (!pEACtion) return E_FAIL;

			m_treeTG.Select(m_hAction, TVGN_CARET, 0);
			pEACtion->m_hItem = m_treeTG.AddChild(pEACtion->m_strDesc.c_str());
			// Add Action Function
			m_pCurrentTrigger->m_vtAction.push_back(pEACtion);
		}
	}
	catch (...)
	{
		MessageBox(L"액션 추가중 예외가 발생되었습니다.(OnTgctAddAction)", L"예외", MB_OK);
	}

	return S_OK;
}

void CdlgTriggerContent::OnTgctCut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnTreeCut();
}

void CdlgTriggerContent::OnTgctCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnTreeCopy();
}

void CdlgTriggerContent::OnTgctPaste()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnTreePaste();
}

void CdlgTriggerContent::OnTgctDelete()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnDeleteSubTree_ForMsg(0, 0);
}

void CdlgTriggerContent::OnTgctUse()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CdlgTriggerContent::OnTgctStartWithUse()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

LRESULT CdlgTriggerContent::OnTreeItemDClick(WPARAM wParam, LPARAM lParam)
{
	try
	{
		HTREEITEM hSelectItem = m_treeTG.GetSelectedItem();
		HTREEITEM hType = FindItemType(hSelectItem);

		if (!hSelectItem || !hType) return E_FAIL;

		if (m_hEvent == hType)
		{
			// 창 생성 부분에 있어야함
			CBaseEventAction *pBaseEAction = FindEventItem(hSelectItem);
			if (pBaseEAction)
			{
				m_dlgTGChildGenner.SetType(TG_TYPE_EACTION, pBaseEAction->m_enType, TG_CONDITION_DEFAULT);
				m_dlgTGChildGenner.m_iFunctionType = 0;
				m_dlgTGChildGenner.m_Mode = CDlgTGChildGenner::MODIFY_MODE;
				m_dlgTGChildGenner.SetValue(pBaseEAction);
				m_dlgTGChildGenner.ShowWindow(SW_SHOW);
			}
			//CDlgTGChildGenner dlgTGChildGenner(TG_TYPE_EACTION, pBaseEAction->m_enType, TG_CONDITION_DEFAULT);

			//dlgTGChildGenner.SetValue(pBaseEAction);

			////if( IDOK == dlgTGChildGenner.DoModal() )
			//if( CDlgTGChildGenner::MODIFY_MODE == m_dlgTGChildGenner.m_Mode
			//    && TG_EACTION_DEFAULT != m_dlgTGChildGenner.m_enAction
			//    )
			//{
			//    m_dlgTGChildGenner.GetValue(pBaseEAction);
			//    m_treeTG.SetItemText(hSelectItem, pBaseEAction->m_strDesc.c_str());
			//}
		}
		else if (m_hCondition == hType)
		{
			CBaseCondition *pBaseCondition = FindConditionItem(hSelectItem);
			if (pBaseCondition)
			{
				m_dlgTGChildGenner.SetType(TG_TYPE_CONDITION, TG_EACTION_DEFAULT, pBaseCondition->m_enType);
				m_dlgTGChildGenner.m_iFunctionType = 1;
				m_dlgTGChildGenner.m_Mode = CDlgTGChildGenner::MODIFY_MODE;
				m_dlgTGChildGenner.SetValue(pBaseCondition);
				m_dlgTGChildGenner.ShowWindow(SW_SHOW);
			}
			//CDlgTGChildGenner dlgTGChildGenner(TG_TYPE_CONDITION, TG_EACTION_DEFAULT, pBaseCondition->m_enType);
			//dlgTGChildGenner.m_Mode = CDlgTGChildGenner::MODIFY_MODE;
			//dlgTGChildGenner.SetValue(pBaseCondition);

			////if( IDOK == dlgTGChildGenner.DoModal() )
			//if( CDlgTGChildGenner::MODIFY_MODE == m_dlgTGChildGenner.m_Mode
			//    && TG_CONDITION_DEFAULT != m_dlgTGChildGenner.m_enCondition
			//    )
			//{
			//    m_dlgTGChildGenner.GetValue(pBaseCondition);
			//    m_treeTG.SetItemText(hSelectItem, pBaseCondition->m_strDesc.c_str());
			//}
		}
		else if (m_hAction == hType)
		{
			CBaseEventAction *pBaseEAction = FindActionItem(hSelectItem);
			if (pBaseEAction)
			{
				m_dlgTGChildGenner.SetType(TG_TYPE_EACTION, pBaseEAction->m_enType, TG_CONDITION_DEFAULT);
				m_dlgTGChildGenner.m_iFunctionType = 2;
				m_dlgTGChildGenner.m_Mode = CDlgTGChildGenner::MODIFY_MODE;
				m_dlgTGChildGenner.SetValue(pBaseEAction);
				m_dlgTGChildGenner.ShowWindow(SW_SHOW);
			}
			//CDlgTGChildGenner dlgTGChildGenner(TG_TYPE_EACTION, pBaseEAction->m_enType, TG_CONDITION_DEFAULT);
			//dlgTGChildGenner.m_Mode = CDlgTGChildGenner::MODIFY_MODE;
			//dlgTGChildGenner.SetValue(pBaseEAction);

			////if( IDOK == dlgTGChildGenner.DoModal() )
			//if( CDlgTGChildGenner::MODIFY_MODE == m_dlgTGChildGenner.m_Mode
			//    && TG_EACTION_DEFAULT != m_dlgTGChildGenner.m_enAction
			//    )
			//{
			//    m_dlgTGChildGenner.GetValue(pBaseEAction);
			//    m_treeTG.SetItemText(hSelectItem, pBaseEAction->m_strDesc.c_str());
			//}
		}
	}
	catch (...)
	{
		MessageBox(L"트리거내용의 트리아이템을 더블클릭 하던 중 예외가 발생하였습니다.(OnTreeItemDClick)", L"예외", MB_OK);
	}

	return S_OK;
}

HTREEITEM CdlgTriggerContent::FindItemType(const HTREEITEM hItem)
{
	if (!m_pCurrentTrigger || !hItem) return NULL;

	try
	{
		HTREEITEM hParent = m_treeTG.GetParentItem(hItem);
		if (m_hEvent == hParent)
		{
			return m_hEvent;
		}
		else if (m_hCondition == hParent)
		{
			return m_hCondition;
		}
		else if (m_hAction == hParent)
		{
			return m_hAction;
		}
	}
	catch (...)
	{
		MessageBox(L"인자로 받은 트리거함수와 종류를 찾던 중 예외가 발생되었습니다.(FindItemType)", L"예외", MB_OK);
	}

	return NULL;
}

CBaseEventAction *CdlgTriggerContent::FindEventItem(const HTREEITEM hItem)
{
	if (!m_pCurrentTrigger || !hItem) return NULL;

	try
	{
		for (int i = 0; i < (int)m_pCurrentTrigger->m_vtEvent.size(); ++i)
		{
			if (hItem == m_pCurrentTrigger->m_vtEvent[i]->m_hItem)
				return m_pCurrentTrigger->m_vtEvent[i];
		}
	}
	catch (...)
	{
		MessageBox(L"인자로 받은 이벤트함수와 같은 함수를 찾던 중 예외가 발생되었습니다.(FindEventItem)", L"예외", MB_OK);
	}

	return NULL;
}

CBaseCondition *CdlgTriggerContent::FindConditionItem(const HTREEITEM hItem)
{
	if (!m_pCurrentTrigger || !hItem) return NULL;

	try
	{
		for (int i = 0; i < (int)m_pCurrentTrigger->m_vtCondition.size(); ++i)
		{
			if (hItem == m_pCurrentTrigger->m_vtCondition[i]->m_hItem)
				return m_pCurrentTrigger->m_vtCondition[i];
		}
	}
	catch (...)
	{
		MessageBox(L"인자로 받은 컨디션함수와 같은 함수를 찾던 중 예외가 발생되었습니다.(FindConditionItem)", L"예외", MB_OK);
	}

	return NULL;
}

CBaseEventAction *CdlgTriggerContent::FindActionItem(const HTREEITEM hItem)
{
	if (!m_pCurrentTrigger || !hItem) return NULL;

	try
	{
		for (int i = 0; i < (int)m_pCurrentTrigger->m_vtAction.size(); ++i)
		{
			if (hItem == m_pCurrentTrigger->m_vtAction[i]->m_hItem)
				return m_pCurrentTrigger->m_vtAction[i];
		}
	}
	catch (...)
	{
		MessageBox(L"인자로 받은 액션함수와 같은 함수를 찾던 중 예외가 발생되었습니다.(FindActionItem)", L"예외", MB_OK);
	}

	return NULL;
}

bool CdlgTriggerContent::DelEventItem(HTREEITEM hItem)
{
	if (!m_pCurrentTrigger || !hItem) return false;

	try
	{
		for (int i = 0; i < (int)m_pCurrentTrigger->m_vtEvent.size(); ++i)
		{
			if (hItem == m_pCurrentTrigger->m_vtEvent[i]->m_hItem)
			{
				SAFE_DELETE(m_pCurrentTrigger->m_vtEvent[i]);
				m_pCurrentTrigger->m_vtEvent.erase(m_pCurrentTrigger->m_vtEvent.begin() + i);
				m_treeTG.DeleteItem(hItem);
				return true;
			}
		}
	}
	catch (...)
	{
		MessageBox(L"이벤트함수를 삭제하던 중 예외가 발생되었습니다.(DelEventItem)", L"예외", MB_OK);
	}

	return false;
}

bool CdlgTriggerContent::DelConditionItem(HTREEITEM hItem)
{
	if (!m_pCurrentTrigger || !hItem) return false;

	try
	{
		for (int i = 0; i < (int)m_pCurrentTrigger->m_vtCondition.size(); ++i)
		{
			if (hItem == m_pCurrentTrigger->m_vtCondition[i]->m_hItem)
			{
				SAFE_DELETE(m_pCurrentTrigger->m_vtCondition[i]);
				m_pCurrentTrigger->m_vtCondition.erase(m_pCurrentTrigger->m_vtCondition.begin() + i);
				m_treeTG.DeleteItem(hItem);
				return true;
			}
		}
	}
	catch (...)
	{
		MessageBox(L"컨디션함수를 삭제하던 중 예외가 발생되었습니다.(DelConditionItem)", L"예외", MB_OK);
	}

	return NULL;
}

bool CdlgTriggerContent::DelActionItem(HTREEITEM hItem)
{
	if (!m_pCurrentTrigger || !hItem) return false;

	try
	{
		for (int i = 0; i < (int)m_pCurrentTrigger->m_vtAction.size(); ++i)
		{
			if (hItem == m_pCurrentTrigger->m_vtAction[i]->m_hItem)
			{
				SAFE_DELETE(m_pCurrentTrigger->m_vtAction[i]);
				m_pCurrentTrigger->m_vtAction.erase(m_pCurrentTrigger->m_vtAction.begin() + i);
				m_treeTG.DeleteItem(hItem);
				return true;
			}
		}
	}
	catch (...)
	{
		MessageBox(L"액션함수를 삭제하던 중 예외가 발생되었습니다.(DelActionItem)", L"예외", MB_OK);
	}

	return false;
}

void CdlgTriggerContent::OnEnChangeEditTgComment()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_pCurrentTrigger)
	{
		CString strComment;
		m_ebTG_Comment.GetWindowText(strComment);
		strComment.Replace(L"\r\n", L"/r/n");
		m_pCurrentTrigger->m_strTGComment = strComment;
	}
}

BOOL CdlgTriggerContent::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_hAccelTable != NULL)
	{
		if (TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CdlgTriggerContent::OnTreeCut(void)
{
	HTREEITEM hSelItem = m_treeTG.GetSelectedItem();
	HTREEITEM hItem = m_treeTG.GetParentItem(m_treeTG.GetSelectedItem());

	if (hItem == m_hEvent)
	{
		ClearEActionClipBoard();
		for (int i = 0; i < (int)m_treeTG.m_vtSelectedItem.size(); ++i)
		{
			CBaseEventAction *pEAction = FindEventItem(m_treeTG.m_vtSelectedItem[i]);
			CBaseEventAction *pNewEAction = NULL;
			if (pEAction) pEAction->CopyEventActionToDest(&pNewEAction);
			DelEventItem(m_treeTG.m_vtSelectedItem[i]);
			m_vtEAction_ClipBoard.push_back(pNewEAction);
		}
		m_enType_ClipBoard = TG_TYPE_EACTION;
	}
	else if (hItem == m_hCondition)
	{
		ClearConditionClipBoard();
		for (int i = 0; i < (int)m_treeTG.m_vtSelectedItem.size(); ++i)
		{
			CBaseCondition *pCondition = FindConditionItem(m_treeTG.m_vtSelectedItem[i]);
			CBaseCondition *pNewCondition = NULL;
			if (pCondition) pCondition->CopyConditionToDest(&pNewCondition);
			DelConditionItem(m_treeTG.m_vtSelectedItem[i]);
			m_vtCondition_ClipBoard.push_back(pNewCondition);
		}
		m_enType_ClipBoard = TG_TYPE_CONDITION;
	}
	else if (hItem == m_hAction)
	{
		ClearEActionClipBoard();
		for (int i = 0; i < (int)m_treeTG.m_vtSelectedItem.size(); ++i)
		{
			CBaseEventAction *pEAction = FindActionItem(m_treeTG.m_vtSelectedItem[i]);
			CBaseEventAction *pNewEAction = NULL;
			if (pEAction) pEAction->CopyEventActionToDest(&pNewEAction);
			DelActionItem(m_treeTG.m_vtSelectedItem[i]);
			m_vtEAction_ClipBoard.push_back(pNewEAction);
		}
		m_enType_ClipBoard = TG_TYPE_EACTION;
	}
}

void CdlgTriggerContent::OnTreeCopy(void)
{
	TreeCopy(m_treeTG.GetSelectedItem());
}

void CdlgTriggerContent::OnTreePaste(void)
{
	TreePaste(m_treeTG.GetSelectedItem());
}

void CdlgTriggerContent::OnBnClickedCheckUse()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pCurrentTrigger) return;
	m_pCurrentTrigger->m_bUse = m_btUse.GetCheck();
	GetParent()->SendMessage(WM_TREE_REDRAW_TEXT_TO_SELITEM, 0, 0);
}

void CdlgTriggerContent::OnBnClickedCheckActive()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pCurrentTrigger) return;
	m_pCurrentTrigger->m_bActive = m_btActive.GetCheck();
	GetParent()->SendMessage(WM_TREE_REDRAW_TEXT_TO_SELITEM, 0, 0);
}

int CdlgTriggerContent::GetBlankMonsterSlotInTrigger(void)
{
	if (!m_pCurrentTrigger) return 0;

	bool bMonsterSlot[MAX_SLOT_MONSTER] = { 0, };

	for (int i = 0; i < (int)m_pCurrentTrigger->m_vtEvent.size(); ++i)
	{
		if (TG_EACTION_MON_GENNER == m_pCurrentTrigger->m_vtEvent[i]->m_enType)
			bMonsterSlot[((CMonsterGenerater*)m_pCurrentTrigger->m_vtEvent[i])->m_iMonIndex] = 1;
	}

	for (int i = 0; i < (int)m_pCurrentTrigger->m_vtAction.size(); ++i)
	{
		if (TG_EACTION_MON_GENNER == m_pCurrentTrigger->m_vtAction[i]->m_enType)
			bMonsterSlot[((CMonsterGenerater*)m_pCurrentTrigger->m_vtAction[i])->m_iMonIndex] = 1;
	}

	for (int i = 0; i < MAX_SLOT_MONSTER; ++i)
	{
		if (0 == bMonsterSlot[i])
			return i;
	}

	return 0;
}

int CdlgTriggerContent::GetBlankObjSlotInTrigger(void)
{
	if (!m_pCurrentTrigger) return 0;

	bool bObjSlot[MAX_SLOT_OBJECT] = { 0, };

	for (int i = 0; i < (int)m_pCurrentTrigger->m_vtEvent.size(); ++i)
	{
		if (TG_EACTION_OBJECTGENERATER == m_pCurrentTrigger->m_vtEvent[i]->m_enType)
			bObjSlot[((CObjectGenerater*)m_pCurrentTrigger->m_vtEvent[i])->m_iSlot] = 1;
	}

	for (int i = 0; i < (int)m_pCurrentTrigger->m_vtAction.size(); ++i)
	{
		if (TG_EACTION_OBJECTGENERATER == m_pCurrentTrigger->m_vtAction[i]->m_enType)
			bObjSlot[((CObjectGenerater*)m_pCurrentTrigger->m_vtAction[i])->m_iSlot] = 1;
	}

	for (int i = 0; i < MAX_SLOT_OBJECT; ++i)
	{
		if (0 == bObjSlot[i])
			return i;
	}

	return 0;
}

int CdlgTriggerContent::GetBlankGateObjSlotInTrigger(void)
{
	if (!m_pCurrentTrigger) return 0;

	bool bObjSlot[MAX_SLOT_GATEOBJECT] = { 0, };

	for (int i = 0; i < (int)m_pCurrentTrigger->m_vtEvent.size(); ++i)
	{
		if (TG_EACTION_GATE_OBJECT_GEN == m_pCurrentTrigger->m_vtEvent[i]->m_enType)
			bObjSlot[((CGateObjectGenerater*)m_pCurrentTrigger->m_vtEvent[i])->m_iSlot] = 1;
	}

	for (int i = 0; i < (int)m_pCurrentTrigger->m_vtAction.size(); ++i)
	{
		if (TG_EACTION_GATE_OBJECT_GEN == m_pCurrentTrigger->m_vtAction[i]->m_enType)
			bObjSlot[((CGateObjectGenerater*)m_pCurrentTrigger->m_vtAction[i])->m_iSlot] = 1;
	}

	for (int i = 0; i < MAX_SLOT_OBJECT; ++i)
	{
		if (0 == bObjSlot[i])
			return i;
	}

	return 0;
}

LRESULT CdlgTriggerContent::OnNMTreeClick(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hSelectedItem = (HTREEITEM)wParam;
	if (!hSelectedItem) return E_FAIL;

	HTREEITEM hSelParent = m_treeTG.GetParentItem(hSelectedItem);
	HTREEITEM hBeforeParent = m_treeTG.GetParentItem(m_hCurSelItem);

	//************************************************************************
	// 이전에 선택한 Item과 새로 선택한 Item의 Level이 다른지 체크한다.
	//************************************************************************
	if ((m_hEvent == hSelParent && m_hEvent != hBeforeParent)
		|| (m_hCondition == hSelParent && m_hCondition != hBeforeParent)
		|| (m_hAction == hSelParent && m_hAction != hBeforeParent)
		)
		ClearSelectItem();

	//************************************************************************
	// 멀티셀렉트 실행
	//************************************************************************
	if (GetKeyState(VK_CONTROL) & 0x80000000)
	{
		if (FindSelItem(hSelectedItem))        // 선택되어 있는 아이템을 또 선택했을 경우
		{
			RemoveSelItem(hSelectedItem);
			m_treeTG.SetItemState(hSelectedItem, 0, TVIS_SELECTED);
		}
		else            // 선택되지 않던 아이템을 선택한 경우 리스트에 추가
		{
			m_treeTG.m_vtSelectedItem.push_back(hSelectedItem);
			m_treeTG.SetItemState(hSelectedItem, TVIS_SELECTED, TVIS_SELECTED);
		}

		// 선택했던 아이템 리스트들의 아이템 상태를 '선택되어있는' 상태로 변환시켜준다.
		for (int i = 0; i < (int)m_treeTG.m_vtSelectedItem.size(); ++i)
			m_treeTG.SetItemState(m_treeTG.m_vtSelectedItem[i], TVIS_SELECTED, TVIS_SELECTED);
	}
	else if (GetKeyState(VK_SHIFT) & 0x80000000)
	{
		if (m_treeTG.m_vtSelectedItem.size() && m_hCurSelItem)
		{
			HTREEITEM hSItem = m_treeTG.m_vtSelectedItem[0];
			HTREEITEM hEItem = hSelectedItem;

			// 두 아이템의 분류(이벤트, 컨디션, 액션)가 틀리면 선택했던 아이템 리스트를
			if (m_treeTG.GetParentItem(hSItem) != m_treeTG.GetParentItem(hSelectedItem))
			{
				ClearSelectItem();
				return E_FAIL;
			}

			//************************************************************************
			// 어느 Item이 더 상위에 있는지 확인하고, 위쪽에 있는 Item을 hSItem에
			// 아래쪽에 있는 Item을 hEItem에 넣는다.
			//************************************************************************
			RECT FromRect;
			m_treeTG.GetItemRect(hSItem, &FromRect, FALSE);
			RECT ToRect;
			m_treeTG.GetItemRect(hEItem, &ToRect, FALSE);
			HTREEITEM hTemp;
			if (FromRect.top > ToRect.top)
			{
				hTemp = hSItem;
				hSItem = hEItem;
				hEItem = hTemp;
			}

			HTREEITEM hCurItem = hSItem;
			ClearSelectItem();
			while (1)
			{
				m_treeTG.SetItemState(hCurItem, TVIS_SELECTED, TVIS_SELECTED);
				m_treeTG.m_vtSelectedItem.push_back(hCurItem);
				if (hCurItem == hEItem)
					break;
				hCurItem = m_treeTG.GetNextVisibleItem(hCurItem);
			}
		}
	}

	return S_OK;
}

LRESULT CdlgTriggerContent::OnTreeSelChange(WPARAM wParam, LPARAM lParam)
{
	if (m_treeTG.GetSelectedItem())
	{
		bool bControl = GetKeyState(VK_CONTROL) & 0x80000000;
		bool bShift = GetKeyState(VK_SHIFT) & 0x80000000;
		bool bVKey = GetKeyState('V') & 0x80000000;
		bool bUDLRKey = (GetKeyState(VK_UP) & 0x80000000) || (GetKeyState(VK_DOWN) & 0x80000000)
			|| (GetKeyState(VK_LEFT) & 0x80000000) || (GetKeyState(VK_RIGHT) & 0x80000000);
		// 컨트롤, 쉬프트키가 안눌러 져있는 경우... SelectItemList 초기화
		// 단). Ctrl + V(붙여넣기)나 UP, DOWN, LEFT, RIGHT 키를 누를 시에도 초기화
		if ((bVKey && bControl) || (!bControl && !bShift) || bUDLRKey)
		{
			HTREEITEM hSelectedItem = m_treeTG.GetSelectedItem();
			ClearSelectItem();
			m_treeTG.m_vtSelectedItem.push_back(hSelectedItem);
			m_hCurSelItem = hSelectedItem;
		}
	}

	return S_OK;
}

LRESULT CdlgTriggerContent::OnTreeCustomDraw(WPARAM wParam, LPARAM lParam)
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
		HTREEITEM hParent = FindItemType(hItem);
		if (!hItem || !hParent) return E_FAIL;

		pcd->clrText = RGB(0, 0, 0);

		if (m_hEvent == hParent)
		{
			CBaseEventAction *pBaseEAction = FindEventItem(hItem);
			if (!pBaseEAction) return E_FAIL;
			if (pBaseEAction->m_bRepeat) pcd->clrText = RGB(89, 89, 171);
		}
		else if (m_hCondition == hParent)
		{
			CBaseCondition *pBaseCondition = FindConditionItem(hItem);
			if (!pBaseCondition) return E_FAIL;
			if (pBaseCondition->m_bRepeat) pcd->clrText = RGB(89, 89, 171);
		}
		else if (m_hAction == hParent)
		{
			CBaseEventAction *pBaseEAction = FindActionItem(hItem);
			if (!pBaseEAction) return E_FAIL;
			if (pBaseEAction->m_bRepeat) pcd->clrText = RGB(89, 89, 171);
		}
	}
	break;
	}

	return S_OK;
}

bool CdlgTriggerContent::FindSelItem(HTREEITEM hFItem)
{
	for (int i = 0; i < (int)m_treeTG.m_vtSelectedItem.size(); ++i)
	{
		if (m_treeTG.m_vtSelectedItem[i] == hFItem)
			return true;
	}

	return false;
}

void CdlgTriggerContent::RemoveSelItem(HTREEITEM hDelItem)
{
	for (int i = 0; i < (int)m_treeTG.m_vtSelectedItem.size(); ++i)
	{
		if (m_treeTG.m_vtSelectedItem[i] == hDelItem)
		{
			m_treeTG.m_vtSelectedItem.erase(m_treeTG.m_vtSelectedItem.begin() + i);
			return;
		}
	}
}

void CdlgTriggerContent::ClearSelectItem(void)
{
	for (int i = 0; i < (int)m_treeTG.m_vtSelectedItem.size(); ++i)
		m_treeTG.SetItemState(m_treeTG.m_vtSelectedItem[i], 0, TVIS_SELECTED);
	m_treeTG.m_vtSelectedItem.clear();
}

void CdlgTriggerContent::ClearEActionClipBoard(void)
{
	for (int i = 0; i < (int)m_vtEAction_ClipBoard.size(); ++i)
		SAFE_DELETE(m_vtEAction_ClipBoard[i]);
	m_vtEAction_ClipBoard.clear();
}

void CdlgTriggerContent::ClearConditionClipBoard(void)
{
	for (int i = 0; i < (int)m_vtCondition_ClipBoard.size(); ++i)
		SAFE_DELETE(m_vtCondition_ClipBoard[i]);
	m_vtCondition_ClipBoard.clear();
}

void CdlgTriggerContent::CheckMonsterGenToKillMaster(void)
{
	if (TG_TYPE_EACTION != m_enType_ClipBoard) return;

	bool bMonGen = false;
	for (int i = 0; i < (int)m_vtEAction_ClipBoard.size(); ++i)
	{
		if (TG_EACTION_MON_GENNER == m_vtEAction_ClipBoard[i]->m_enType)
		{
			bMonGen = true;
			break;
		}
	}
	if (!bMonGen) return;

	if (IDYES == MessageBox(L"MonsterGenerator 이벤트,액션을 발견하였습니다. KillMaster 조건으로 전환할 수 있습니다. 전환하시겠습니까??", L"알림", MB_YESNO))
	{
		HTREEITEM hSelItem = m_treeTG.GetSelectedItem();
		HTREEITEM hParentItem = m_treeTG.GetParentItem(hSelItem);

		if ((hSelItem == m_hCondition) || (hParentItem == m_hCondition))
		{
			//************************************************************************
			// 트리거함수의 Desc를 위해서 CDlgTGKillMaster를 생성했다가 소멸시킵니다.
			//************************************************************************
			CDlgTGKillMaster DlgKillMaster;
			DlgKillMaster.Create(CDlgTGKillMaster::IDD, this);
			DlgKillMaster.ShowWindow(SW_HIDE);
			for (int i = 0; i < (int)m_vtEAction_ClipBoard.size(); ++i)
			{
				if (TG_EACTION_MON_GENNER == m_vtEAction_ClipBoard[i]->m_enType)
				{
					//************************************************************************
					// 몬스터 Generator의 정보를 KillMaster 정보로 변환시킵니다.
					//************************************************************************
					CMonsterGenerater *pMonGen = (CMonsterGenerater*)m_vtEAction_ClipBoard[i];
					CKillMaster *pNewKillMaster = new CKillMaster;
					pNewKillMaster->SetValue(pMonGen->m_iMonIndex, pMonGen->m_iMonType);
					DlgKillMaster.SetValue(pNewKillMaster);
					DlgKillMaster.Refresh();
					DlgKillMaster.CalcResult();
					DlgKillMaster.GetValue(pNewKillMaster);
					m_treeTG.Select(m_hCondition, TVGN_CARET, 0);
					pNewKillMaster->m_hItem = m_treeTG.AddChild(pNewKillMaster->m_strDesc.c_str());
					m_pCurrentTrigger->AddCondition(pNewKillMaster);
				}
			}
			DlgKillMaster.DestroyWindow();
		}
	}
}

void CdlgTriggerContent::OnTgctAddPopupEvent()
{
	m_dlgTGChildGenner.SetType(TG_TYPE_EACTION, TG_EACTION_LOADING, TG_CONDITION_DEFAULT);
	m_dlgTGChildGenner.m_iFunctionType = 0;
	m_dlgTGChildGenner.m_Mode = CDlgTGChildGenner::CREATE_MODE;
	m_dlgTGChildGenner.ShowWindow(SW_SHOW);
}

void CdlgTriggerContent::OnTgctAddPopupCondition()
{
	m_dlgTGChildGenner.SetType(TG_TYPE_CONDITION, TG_EACTION_DEFAULT, TG_CONDITION_KILLMASTER);
	m_dlgTGChildGenner.m_iFunctionType = 1;
	m_dlgTGChildGenner.m_Mode = CDlgTGChildGenner::CREATE_MODE;
	m_dlgTGChildGenner.ShowWindow(SW_SHOW);
}

void CdlgTriggerContent::OnTgctAddPopupAction()
{
	m_dlgTGChildGenner.SetType(TG_TYPE_EACTION, TG_EACTION_LOADING, TG_CONDITION_DEFAULT);
	m_dlgTGChildGenner.m_iFunctionType = 2;
	m_dlgTGChildGenner.m_Mode = CDlgTGChildGenner::CREATE_MODE;
	m_dlgTGChildGenner.ShowWindow(SW_SHOW);
}

LRESULT CdlgTriggerContent::OnTgctModifyEvent(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hSelectItem = m_treeTG.GetSelectedItem();
	HTREEITEM hType = FindItemType(hSelectItem);

	if (!hSelectItem || !hType) return E_FAIL;

	if (m_hEvent == hType)
	{
		// 창 생성 부분에 있어야함
		CBaseEventAction *pBaseEAction = FindEventItem(hSelectItem);
		if (!pBaseEAction) return E_FAIL;

		if (CDlgTGChildGenner::MODIFY_MODE == m_dlgTGChildGenner.m_Mode
			&& TG_EACTION_DEFAULT != m_dlgTGChildGenner.m_enAction
			)
		{
			m_dlgTGChildGenner.GetValue(pBaseEAction);
			pBaseEAction->m_hItem = hSelectItem;
			m_treeTG.SetItemText(hSelectItem, pBaseEAction->m_strDesc.c_str());
		}
	}

	return S_OK;
}

LRESULT CdlgTriggerContent::OnTgctModifyCondition(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hSelectItem = m_treeTG.GetSelectedItem();
	HTREEITEM hType = FindItemType(hSelectItem);

	if (!hSelectItem || !hType) return E_FAIL;

	if (m_hCondition == hType)
	{
		// 창 생성 부분에 있어야함
		CBaseCondition *pBaseCondition = FindConditionItem(hSelectItem);
		if (!pBaseCondition) return E_FAIL;

		if (CDlgTGChildGenner::MODIFY_MODE == m_dlgTGChildGenner.m_Mode
			&& TG_CONDITION_DEFAULT != m_dlgTGChildGenner.m_enCondition
			)
		{
			m_dlgTGChildGenner.GetValue(pBaseCondition);
			pBaseCondition->m_hItem = hSelectItem;
			m_treeTG.SetItemText(hSelectItem, pBaseCondition->m_strDesc.c_str());
		}
	}
	return S_OK;
}

LRESULT CdlgTriggerContent::OnTgctModifyAction(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hSelectItem = m_treeTG.GetSelectedItem();
	HTREEITEM hType = FindItemType(hSelectItem);

	if (!hSelectItem || !hType) return E_FAIL;

	if (m_hAction == hType)
	{
		// 창 생성 부분에 있어야함
		CBaseEventAction *pBaseEAction = FindActionItem(hSelectItem);
		if (!pBaseEAction) return E_FAIL;

		if (CDlgTGChildGenner::MODIFY_MODE == m_dlgTGChildGenner.m_Mode
			&& TG_EACTION_DEFAULT != m_dlgTGChildGenner.m_enAction
			)
		{
			m_dlgTGChildGenner.GetValue(pBaseEAction);
			pBaseEAction->m_hItem = hSelectItem;
			m_treeTG.SetItemText(hSelectItem, pBaseEAction->m_strDesc.c_str());
		}
	}

	return S_OK;
}
int CdlgTriggerContent::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	m_dlgTGChildGenner.Create(CDlgTGChildGenner::IDD, this);

	return 0;
}

void CdlgTriggerContent::RefreshTreeTitle(HTREEITEM hItem, std::wstring strTitle)
{
	CBaseEventAction    *pEvent = FindEventItem(hItem);
	CBaseCondition      *pCondition = FindConditionItem(hItem);
	CBaseEventAction    *pAction = FindActionItem(hItem);

	if (pEvent)
	{
		m_treeTG.SetItemText(hItem, strTitle.c_str());
	}
	else if (pCondition)
	{
		m_treeTG.SetItemText(hItem, strTitle.c_str());
	}
	else if (pAction)
	{
		m_treeTG.SetItemText(hItem, strTitle.c_str());
	}
}

void CdlgTriggerContent::MoveItem(WORD wKey)
{
	HTREEITEM hSelect = m_treeTG.GetSelectedItem();
	if (wKey == VK_UP)
	{
		HTREEITEM hTemp = m_treeTG.GetPrevSiblingItem(hSelect);
		if (hTemp)
		{
			m_treeTG.m_vtSelectedItem.clear();
			m_treeTG.m_vtSelectedItem.push_back(hTemp);
			HTREEITEM hNew = TreeCopyAndPaste(hTemp, m_treeTG.GetParentItem(hSelect), hSelect);
			DelEventItem(hTemp);
			DelConditionItem(hTemp);
			DelActionItem(hTemp);
			m_treeTG.Select(hSelect, TVGN_CARET);
		}
	}
	else if (wKey == VK_DOWN)
	{
		HTREEITEM hTemp = m_treeTG.GetNextSiblingItem(hSelect);
		if (hTemp && (NULL == m_treeTG.GetChildItem(hSelect)))
		{
			m_treeTG.m_vtSelectedItem.clear();
			m_treeTG.m_vtSelectedItem.push_back(hSelect);
			TreeCopyAndPaste(hSelect, m_treeTG.GetParentItem(hTemp), hTemp);
			DelEventItem(hSelect);
			DelConditionItem(hSelect);
			DelActionItem(hSelect);
		}
	}
}

HTREEITEM CdlgTriggerContent::TreeCopyAndPaste(HTREEITEM hSelItem, HTREEITEM hParent, HTREEITEM hDest)
{
	TreeCopy(hSelItem);
	return TreePaste(hSelItem, hParent, hDest, true);
}

void CdlgTriggerContent::TreeCopy(HTREEITEM hSelItem)
{
	if (NULL == hSelItem)
		hSelItem = m_treeTG.GetSelectedItem();
	HTREEITEM hItem = m_treeTG.GetParentItem(hSelItem);

	if (hItem == m_hEvent)
	{
		ClearEActionClipBoard();
		for (int i = 0; i < (int)m_treeTG.m_vtSelectedItem.size(); ++i)
		{
			CBaseEventAction *pEAction = FindEventItem(m_treeTG.m_vtSelectedItem[i]);
			CBaseEventAction *pNewEAction = NULL;
			if (pEAction) pEAction->CopyEventActionToDest(&pNewEAction);
			m_vtEAction_ClipBoard.push_back(pNewEAction);
		}
		m_enType_ClipBoard = TG_TYPE_EACTION;
	}
	else if (hItem == m_hCondition)
	{
		ClearConditionClipBoard();
		for (int i = 0; i < (int)m_treeTG.m_vtSelectedItem.size(); ++i)
		{
			CBaseCondition *pCondition = FindConditionItem(m_treeTG.m_vtSelectedItem[i]);
			CBaseCondition *pNewCondition = NULL;
			if (pCondition) pCondition->CopyConditionToDest(&pNewCondition);
			m_vtCondition_ClipBoard.push_back(pNewCondition);
		}
		m_enType_ClipBoard = TG_TYPE_CONDITION;
	}
	else if (hItem == m_hAction)
	{
		ClearEActionClipBoard();
		for (int i = 0; i < (int)m_treeTG.m_vtSelectedItem.size(); ++i)
		{
			CBaseEventAction *pEAction = FindActionItem(m_treeTG.m_vtSelectedItem[i]);
			CBaseEventAction *pNewEAction = NULL;
			if (pEAction) pEAction->CopyEventActionToDest(&pNewEAction);
			m_vtEAction_ClipBoard.push_back(pNewEAction);
		}
		m_enType_ClipBoard = TG_TYPE_EACTION;
	}
}

HTREEITEM CdlgTriggerContent::TreePaste(HTREEITEM hSelItem, HTREEITEM hParent, HTREEITEM hDest, bool bNoAddName)
{
	HTREEITEM hNewItem = NULL;

	if (NULL == hSelItem)
		hSelItem = m_treeTG.GetSelectedItem();
	if (NULL == hParent)
		hParent = m_treeTG.GetParentItem(hSelItem);

	if (((hSelItem == m_hEvent) || (hParent == m_hEvent))
		&& m_enType_ClipBoard == TG_TYPE_EACTION
		&& m_vtEAction_ClipBoard.size()
		)
	{
		for (int i = 0; i < (int)m_vtEAction_ClipBoard.size(); ++i)
		{
			CBaseEventAction *pBaseEAction;
			m_vtEAction_ClipBoard[i]->CopyEventActionToDest(&pBaseEAction);

			if (hSelItem == m_hEvent)
				pBaseEAction->m_hItem = m_treeTG.AddChild(pBaseEAction->m_strDesc.c_str(), hDest);
			else
				pBaseEAction->m_hItem = m_treeTG.AddSibling(pBaseEAction->m_strDesc.c_str(), hDest);

			hNewItem = pBaseEAction->m_hItem;
			m_pCurrentTrigger->m_vtEvent.push_back(pBaseEAction);

			if (1 < static_cast<int>(m_vtEAction_ClipBoard.size()))
				m_treeTG.Select(hSelItem, TVGN_CARET);
		}
	}
	else if (((hSelItem == m_hCondition) || (hParent == m_hCondition))
		&& m_enType_ClipBoard == TG_TYPE_CONDITION
		&& m_vtCondition_ClipBoard.size()
		)
	{
		for (int i = 0; i < (int)m_vtCondition_ClipBoard.size(); ++i)
		{
			CBaseCondition *pBaseCondition;
			m_vtCondition_ClipBoard[i]->CopyConditionToDest(&pBaseCondition);

			if (hSelItem == m_hCondition)
				pBaseCondition->m_hItem = m_treeTG.AddChild(pBaseCondition->m_strDesc.c_str(), hDest);
			else
				pBaseCondition->m_hItem = m_treeTG.AddSibling(pBaseCondition->m_strDesc.c_str(), hDest);

			hNewItem = pBaseCondition->m_hItem;
			m_pCurrentTrigger->m_vtCondition.push_back(pBaseCondition);

			if (1 < static_cast<int>(m_vtCondition_ClipBoard.size()))
				m_treeTG.Select(hSelItem, TVGN_CARET);
		}
	}
	else if (((hSelItem == m_hAction) || (hParent == m_hAction))
		&& m_enType_ClipBoard == TG_TYPE_EACTION
		&& m_vtEAction_ClipBoard.size()
		)
	{
		for (int i = 0; i < (int)m_vtEAction_ClipBoard.size(); ++i)
		{
			CBaseEventAction *pBaseEAction;
			m_vtEAction_ClipBoard[i]->CopyEventActionToDest(&pBaseEAction);

			if (hSelItem == m_hAction)
				pBaseEAction->m_hItem = m_treeTG.AddChild(pBaseEAction->m_strDesc.c_str(), hDest);
			else
				pBaseEAction->m_hItem = m_treeTG.AddSibling(pBaseEAction->m_strDesc.c_str(), hDest);

			hNewItem = pBaseEAction->m_hItem;
			m_pCurrentTrigger->m_vtAction.push_back(pBaseEAction);

			if (1 < static_cast<int>(m_vtEAction_ClipBoard.size()))
				m_treeTG.Select(hSelItem, TVGN_CARET);
		}
	}
	else
	{
		MessageBox(L"추가할 수 있는 트리구조가 없습니다.", L"알림", MB_OK);
		CheckMonsterGenToKillMaster();
	}

	return hNewItem;
}