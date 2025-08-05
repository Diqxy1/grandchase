// QuestScripterDlg.cpp : 구현 파일
//
#include "stdafx.h"
#include "QuestScripter.h"
#include "QuestScripterDlg.h"
#include "xSkinButton.h"
#include "QuestScripterDlg.h"
#include "DlgChangeSequence.h"
#include "DlgStruct.h"
#include "DlgHelp.h"
#include "DlgTGChildGenner.h"
#include "DlgTGPosition.h"

#include "MapSystem/KQSLayerManager.h"
//#include "../GCLuaManager/KLuabinder.h"
//#include "../GCDeviceLib/GCDeviceManager.h"
#include "ObjectDataManager.h"

//#include "KLuaManager.h"
//#include "DlgInfo.h"
//#include "DlgMapToObject.h"
#include "DlgDirectorySetting.h"

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
#include "Subject.h"

//************************************************************************
// 조건
//************************************************************************
#include "KillMaster.h"
#include "Timer.h"
#include "Position.h"

#include "DlgTrigger.h"

// CQuestScripterDlg 대화 상자
#define TOTAL_LEVEL 4

CQuestScripterDlg::CQuestScripterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQuestScripterDlg::IDD, pParent)
{
	m_hAccelTable = NULL;

	m_LastUse_ClipBoard = NOTHING;
	m_bAutoSave = true;
	m_bCreated = 0;
	m_hSelectedBefore = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// Quest Level 별 분리 입니다.
	m_NamesOfLevel[0].push_back(STR_QUEST);
	m_NamesOfLevel[1].push_back(STR_STAGE);

	m_NamesOfLevel_Def[0].push_back(QUESTX);
	m_NamesOfLevel_Def[1].push_back(STAGEX);

	GetCurrentDirectoryA(sizeof(g_Path), g_Path);       // 프로그램의 실행경로를 받아온다.

	LoadPreLoad();
}

CQuestScripterDlg::~CQuestScripterDlg()
{
	SiCObjectDataManager()->ReleaseInstance();
	SiCDlgDirectorySetting()->ReleaseInstance();
}

void CQuestScripterDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_TREE_SCRIPT, m_ScriptTree);
	DDX_Control(pDX, IDC_STATIC_MAIN, m_stMain);

	m_dlgQuest.Create(IDD_DIALOG_QUEST, &m_stMain);
	m_dlgStage.Create(IDD_DIALOG_STAGE, &m_stMain);
	m_dlgInfo.Create(IDD_DIALOG_POPUP_BOARD, this);

	m_dlgQuest.SetComboBoxValue(this);
	m_dlgStage.SetComboBoxValue(this);
	m_dlgInfo.m_dlgQuest.SetComboBoxValue(this);
	m_dlgInfo.m_dlgStage.SetComboBoxValue(this);

	m_ScriptTree.InitializeImageList();
	m_Quest.MatchKey = m_ScriptTree.AddChild(L"QuestX");
	m_Quest.NOL = QUESTX;
	m_ScriptTree.SetAllowMoveToOtherLevel(0);

	//////////////////////////////////////////////////////////////////////////
	// 너무 썰렁해서.. 테스트 할때 기본트리 생성으로 일부러 넣어 놓았다는.. ㅋㅋ
	// Jaeho.Test
	m_Datamanager.AddQuest(&m_Quest);
	OnAddchild_Enter();
	OnAddchild_Enter();
	OnAddchild_Enter();
	OnAddchild_Enter();
	m_ScriptTree.DeleteAllItems();
	//////////////////////////////////////////////////////////////////////////

	// m_Quest구조를 확인하여 트리를 다시 그린다.
	MadeTree(m_Quest);

	// 최상위 트리 구조를 선택한다.
	if (m_Quest.MatchKey)
	{
		m_ScriptTree.Select(m_Quest.MatchKey, TVGN_CARET);
		m_hSelectedBefore = m_Quest.MatchKey;
		//ChangeDialog(m_Quest.MatchKey);
	}

	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_stInfo);
}

BEGIN_MESSAGE_MAP(CQuestScripterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	//    ON_WM_MEASUREITEM()
	//    ON_WM_MENUCHAR()
	ON_COMMAND(ID_TREE_NEW, OnTreeNew)
	ON_WM_SIZE()
	ON_COMMAND(ID_ADDCHILD_Q, OnAddchildQ)
	ON_COMMAND(ID_SIBLINGCHILD_Q, OnSiblingchildQ)
	ON_COMMAND(ID_REMARK_Q, OnRemarkQ)
	ON_COMMAND(ID_DELETESUBTREE_Q, OnDeletesubtreeQ)
	ON_COMMAND(ID_DELETEENTIRETREE_Q, OnDeleteentiretreeQ)
	ON_COMMAND(ID_INFO_STRUCT, OnInfoStruct)
	ON_COMMAND(ID_SAVE_TO_FILE, OnSaveToFile)

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_LOAD_FROM_FILE_FULL, OnLoadFromFileFull)
	ON_COMMAND(ID_LOAD_FROM_FILE, OnLoadFromFile)
	ON_COMMAND(ID_SVAE_TO_FILE_FULL, OnSvaeToFileFull)
	ON_COMMAND(ID_EDIT_REFRESH, OnEditRefresh)
	ON_MESSAGE(WM_MONSTER_ADD_WINDOW, OnMonsterAddWindow)
	ON_MESSAGE(WM_PLAYER_ADD_WINDOW, OnPlayerAddWindow)
	ON_MESSAGE(WM_MONSTER_SEL_WINDOW, OnMonsterSeldWindow)
	ON_MESSAGE(WM_AREA_SEL_WINDOW, OnAreaSelWindow)
	ON_MESSAGE(WM_POS_SEL_WINDOW, OnPosSelWindow)
	ON_MESSAGE(WM_OBJECT_ADD_WINDOW, OnObjectAddWindow)
	ON_MESSAGE(WM_GATEOBJECT_ADD_WINDOW, OnGateObjectAddWindow)
	ON_MESSAGE(WM_DAMAGEOBJECT_ADD_WINDOW, OnDamageObjectAddWindow)
	ON_MESSAGE(WM_CHANGESEQUENCE, OnChangeSequence)
	ON_MESSAGE(WM_MENU_ENABLE, OnEnableMessage)
	ON_MESSAGE(WM_CHANG_DIALOG, OnChangeDialog)
	ON_MESSAGE(WM_SHIFT_SELECT, OnShiftTreeSelect)
	ON_MESSAGE(WM_ADDCHILD_ENTER, OnAddchild_Enter_ForMsg)
	ON_MESSAGE(WM_ADDSIBLING_SPACEBAR, OnAddSibling_SpaceBar_ForMsg)
	ON_MESSAGE(WM_ADDCHILDQ, OnAddchildQ_ForMsg)
	ON_MESSAGE(WM_ADDSIBLINGQ, OnAddSiblingQ_ForMsg)
	ON_MESSAGE(WM_DELETESUBTREE, OnDeleteSubTree_ForMsg)
	ON_MESSAGE(WM_MOVESIBLING, OnMoveSibling_forMsg)
	ON_MESSAGE(WM_ACTIVE_INFOSTRUCT, OnInfoStruct_ForMsg)
	ON_MESSAGE(WM_SAVE_TO_FILE, OnSaveToFile_ForMsg)
	ON_MESSAGE(WM_SAVE_TO_FILE_FULL, OnSaveToFileFull_ForMsg)
	ON_MESSAGE(WM_LOAD_FROM_FILE, OnLoadFromFile_ForMsg)
	ON_MESSAGE(WM_LOAD_FROM_FILE_FULL, OnLoadFromFileFull_ForMsg)
	ON_MESSAGE(WM_LOAD_FILE_FROM_FILE_LIST, OnLoadFromFileListFull)
	ON_MESSAGE(WM_NEW_TREE, OnNewTree)
	ON_MESSAGE(WM_CUT_TREE, OnCutTree)
	ON_MESSAGE(WM_COPY_TREE, OnCopyTree)
	ON_MESSAGE(WM_PASTE_TREE, OnPasteTree)
	ON_MESSAGE(WM_LOAD_TOOL_ICON, LoadIconLayerPart)
	ON_MESSAGE(WM_CHAGNE_TREE_FOCUS, OnChangeTreeFocus)
	ON_COMMAND(ID_CHANGE_SEQUENCE, OnChangeSequence)
	ON_COMMAND(ID_TREE_CUT, OnTreeCut)
	ON_COMMAND(ID_TREE_COPY, OnTreeCopy)
	ON_COMMAND(ID_TREE_PASTE, OnTreePaste)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_COLLAPSE_ALL_ENTIRE_TREE, OnCollapseAllEntireTree)
	ON_COMMAND(IDC_EXPAND_ALL_TREE, OnExpandAllTree)
	ON_WM_TIMER()
	ON_COMMAND(ID_CUSTOM_DIRECTORY, OnCustomDirectory)
	ON_WM_DROPFILES()
	ON_MESSAGE(WM_FOCUS_WINDOW, OnFocusWindow)
	ON_COMMAND(ID_FILELIST, OnFilelist)
	ON_MESSAGE(WM_SAVE_SERVER_SCRIPT_FROM_FILE_LIST, OnSaveServerScriptFromFileList)
	ON_COMMAND(ID_FILE_SAVETOSERVERSCRIPT_BY_QUESTSIGN, OnFileSavetoserverscriptByQuestsign)
	ON_COMMAND(ID_SVAE_TO_SAME_FILE_FULL, OnSvaeToSameFileFull)
	ON_COMMAND(ID_FILE_AUTOSAVE, OnFileAutosave)
	ON_MESSAGE(WM_POSASEL_FUNCTION, OnPosAsel)
	ON_COMMAND(ID_FILE_SAVETOSERVERSCRIPT_BY_SUBJECT, OnFileSavetoserverscriptBySubject)
END_MESSAGE_MAP()

// CQuestScripterDlg 메시지 처리기

BOOL CQuestScripterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_bCreated = 1;

	//************************************************************************
	// Tree창과 정보 표시창의 위치를 잡아준다.
	CRect rtClient;
	GetClientRect(rtClient);
	const int iStartX = rtClient.left + 10;
	const int iStartY = rtClient.top + 10;
	const int iTreeWidth = 300;
	m_ScriptTree.MoveWindow(CRect(iStartX, iStartY, iStartX + iTreeWidth, rtClient.bottom - 40));
	m_stMain.MoveWindow(CRect(iStartX + iTreeWidth + 5, iStartY, rtClient.right - 10, rtClient.bottom - 40));
	m_stInfo.MoveWindow(CRect(iStartX, rtClient.bottom - 30, rtClient.right - 10, rtClient.bottom - 10), 1);
	//************************************************************************

	//************************************************************************
	// 단축키를 사용하기 위해 단축키 정보를 불러온다.
	m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR_MAIN_DLG));
	//************************************************************************

	//************************************************************************
	// 윈도우 Title 이름을 설정한다.
	SetWindowText(L"Hello~? Welcome to GC_QuestScipter");
	//************************************************************************

	//************************************************************************
	// DirectX 윈도우 창을 생성한다.
	m_dlgMapToObject.Create(this, L"");
	m_dlgMapToObject.ShowWindow(SW_HIDE);
	//************************************************************************

	ReSizeChildWindow();
	// AutoSave
	m_bAutoSave = true;
	SetTimer(0, QUEST_SCRIPTER_AUTOSAVE_TIME, NULL);

	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CQuestScripterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CQuestScripterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CQuestScripterDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//m_Menu.DestroyMenu();       // 메뉴 제거
	CDialog::OnClose();
}

void CQuestScripterDlg::OnUpdateFlush(CMenu *pMenu)
{
	ASSERT(pMenu != NULL);
	// check the enabled state of various menu items

	CCmdUI state;
	state.m_pMenu = pMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// determine if menu is popup in top-level menu and set m_pOther to
	//  it if so (m_pParentMenu == NULL indicates that it is secondary popup)
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pMenu->m_hMenu)
		state.m_pParentMenu = pMenu;    // parent == child for tracking popup
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = GetTopLevelParent();
		// child windows don't have menus -- need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pMenu->m_hMenu)
				{
					// when popup is found, m_pParentMenu is containing menu
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // menu separator or invalid cmd - ignore it

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// possibly a popup menu, route to first item of that popup
			state.m_pSubMenu = pMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // first item of popup can't be routed to
			}
			state.DoUpdate(this, FALSE);    // popups are never auto disabled
		}
		else
		{
			// normal menu item
			// Auto enable/disable if frame window has 'm_bAutoMenuEnable'
			//    set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, state.m_nID < 0xF000);
		}

		// adjust for menu deletions and additions
		UINT nCount = pMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}

void CQuestScripterDlg::OnTreeNew()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 현재 가지고 있던 퀘스트를 모두 제거 하고, 새로운 구조를 만든다.
	if (IDYES == MessageBox(L"새로운 퀘스트를 생성하시 겠습니까?(기존의 퀘스트는 소멸됩니다.)", L"생성", MB_YESNO))
	{
		m_dlgQuest.SetRowColText(0, 0);
		m_dlgQuest.m_dlgTileBG.DestroyButtons();
		m_Quest.DeleteItem();
		ReDrawTree();
		SetWindowText(L"Hello~? Welcome to GC_QuestScipter");
	}
	m_Datamanager.AddQuest(&m_Quest);
}

void CQuestScripterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	ReSizeChildWindow();
}

void CQuestScripterDlg::ReSizeChildWindow(void)
{
	// 윈도우 크기 조절
	if (m_bCreated)
	{
		CRect rtClient;
		GetClientRect(rtClient);
		const int iStartX = rtClient.left + 10;
		const int iStartY = rtClient.top + 10;
		const int iTreeWidth = 300;
		m_ScriptTree.MoveWindow(CRect(iStartX, iStartY, iStartX + iTreeWidth, rtClient.bottom - 40), 1);
		m_stMain.MoveWindow(CRect(iStartX + iTreeWidth + 5, iStartY, rtClient.right - 10, rtClient.bottom - 40), 1);
		m_stInfo.MoveWindow(CRect(iStartX, rtClient.bottom - 30, rtClient.right - 10, rtClient.bottom - 10), 1);

		m_stMain.GetClientRect(rtClient);
		m_dlgQuest.MoveWindow(rtClient, 1);
	}
}

HTREEITEM CQuestScripterDlg::OnAddchild_Enter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	HTREEITEM hSelItem = m_ScriptTree.GetSelectedItem();
	HTREEITEM hNewChild = 0;
	struct_base *pNewChild = 0;
	if (hSelItem)
	{
		int iLevel = m_ScriptTree.GetLevelTree(hSelItem);

		if (iLevel >= 0 && iLevel < (int)m_NamesOfLevel.size())
		{
			if (m_NamesOfLevel[iLevel].size() > 0)
			{
				hNewChild = m_ScriptTree.AddChild(m_NamesOfLevel[iLevel][0]);
				AddChild(hSelItem, hNewChild, m_NamesOfLevel_Def[iLevel][0]);
				pNewChild = m_Quest.FindStructFromItem(hNewChild);              // 새로만든 트리정보의 포인터를 얻는다.
				if (pNewChild)     // AddChild 함수로 정상적인 구조가 생성되었다면 pNewChild가 Null 이 아니다.
					m_ScriptTree.Select(pNewChild->MatchKey, TVGN_CARET);           // 새로만든 트리정보를 선택합니다.
				else
					ReDrawTree();                                                   // 트리를 다시 그립니다.
			}
			else
			{
				StaticInfoWithMessageBox(L"더 이상의 하위구조는 생성할 수 없습니다.", L"", MB_OK);
			}
		}
	}
	else
	{
		StaticInfoWithMessageBox(L"트리를 다시 선택해 주십시오", L"", MB_OK);
	}

	m_Datamanager.AddQuest(&m_Quest);
	if (pNewChild)
		return pNewChild->MatchKey;

	return NULL;
}

HTREEITEM CQuestScripterDlg::OnAddSibling_SpaceBar()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	HTREEITEM hSelItem = m_ScriptTree.GetSelectedItem();
	if (m_ScriptTree.GetRootItem() == hSelItem)
	{
		StaticInfoWithMessageBox(L"Quest는 2개이상이 될 수 없습니다.", L"알림", MB_OK);
		return NULL;
	}

	HTREEITEM hParentItem = m_ScriptTree.GetParentItem(hSelItem);
	HTREEITEM hNewChild = 0;
	struct_base *pNewChild = 0;
	if (hSelItem && hParentItem)
	{
		int iLevel = m_ScriptTree.GetLevelTree(hSelItem) - 1;
		if (iLevel >= 0 && iLevel < (int)m_NamesOfLevel.size())
		{
			if (m_NamesOfLevel[iLevel].size() > 0)
			{
				hNewChild = m_ScriptTree.AddSibling(m_NamesOfLevel[iLevel][0]);
				AddSibling(hSelItem, hNewChild, hParentItem);
				pNewChild = m_Quest.FindStructFromItem(hNewChild);                // 새로만든 트리정보의 포인터를 얻는다.
				if (pNewChild)     // AddChild 함수로 정상적인 구조가 생성되었다면 pNewChild가 Null 이 아니다.
					m_ScriptTree.Select(pNewChild->MatchKey, TVGN_CARET);           // 새로만든 트리정보를 선택합니다.
				else
					ReDrawTree();                                                   // 트리를 다시 그립니다.
			}
			else
			{
				StaticInfoWithMessageBox(L"추가 할 수 있는 구조가 없습니다.", L"", MB_OK);
			}
		}
	}
	else
	{
		StaticInfoWithMessageBox(L"트리를 다시 선택해 주십시오", L"", MB_OK);
	}

	m_Datamanager.AddQuest(&m_Quest);

	if (pNewChild)
		return pNewChild->MatchKey;

	return NULL;
}

void CQuestScripterDlg::OnAddchildQ()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	HTREEITEM hti, hNewChild;
	hti = m_ScriptTree.GetSelectedItem();

	struct_base *pBase = m_Quest.FindStructFromItem(hti);

	if (pBase)
	{
		CDlgStruct dlgStruct(CDlgStruct::ADD_CHILD_TYPE, pBase->NOL);
		if (IDOK == dlgStruct.DoModal())
		{
			// 트리에 자식 정보 추가
			hNewChild = m_ScriptTree.AddChild(dlgStruct.m_strResult);

			// 퀘스트 구조에 자식 정보 추가
			AddChild(hti, hNewChild, dlgStruct.m_NamesOfLevel);
			struct_base *pNewChild = m_Quest.FindStructFromItem(hNewChild);  // 새로만든 트리정보의 포인터를 얻는다.
			if (pNewChild)
				m_ScriptTree.Select(pNewChild->MatchKey, TVGN_CARET);           // 새로만든 트리정보를 선택합니다.
			else
				ReDrawTree();                                                   // 트리를 다시 그립니다.
		}
	}
	else
	{
		StaticInfoWithMessageBox(L"트리를 다시 선택해 주십시오", L"", MB_OK);
	}

	m_Datamanager.AddQuest(&m_Quest);
}

void CQuestScripterDlg::OnSiblingchildQ()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	HTREEITEM hti, hNewChild;
	hti = m_ScriptTree.GetSelectedItem();
	HTREEITEM hParentItem = m_ScriptTree.GetParentItem(hti);
	if (m_ScriptTree.GetRootItem() == hti)
	{
		StaticInfoWithMessageBox(L"Quest는 2개이상이 될 수 없습니다.", L"알림", MB_OK);
		return;
	}

	struct_base *pBase = m_Quest.FindStructFromItem(hti);
	struct_base *pNewChild = 0;

	if (pBase && hParentItem)
	{
		CDlgStruct dlgStruct(CDlgStruct::ADD_SIBLING_TYPE, pBase->NOL);
		if (IDOK == dlgStruct.DoModal())
		{
			hNewChild = m_ScriptTree.AddSibling(dlgStruct.m_strResult);
			AddSibling(hti, hNewChild, hParentItem);
			pNewChild = m_Quest.FindStructFromItem(hNewChild);               // 새로만든 트리정보의 포인터를 얻는다.
			if (pNewChild)
				m_ScriptTree.Select(pNewChild->MatchKey, TVGN_CARET);           // 새로만든 트리정보를 선택합니다.
			else
				ReDrawTree();                                                   // 트리를 다시 그립니다.
		}
	}
	else
	{
		StaticInfoWithMessageBox(L"트리를 다시 선택해 주십시오", L"", MB_OK);
	}

	m_Datamanager.AddQuest(&m_Quest);
}

void CQuestScripterDlg::OnRemarkQ()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CQuestScripterDlg::OnDeletesubtreeQ()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	HTREEITEM hti;
	hti = m_ScriptTree.GetSelectedItem();
	if (hti == m_ScriptTree.GetRootItem())
	{
		if (IDOK != MessageBox(L"정말 Quest를 지우시겠습니까?", L"질문", MB_OKCANCEL))
			return;
	}

	HTREEITEM hParentItem = m_ScriptTree.GetParentItem(hti);
	HTREEITEM hNextVisibleItem = m_ScriptTree.GetNextVisibleItem(hti);
	struct_base *pBaseParent = m_Quest.FindStructFromItem(hParentItem);

	m_Quest.DeleteItem(hti);
	m_ScriptTree.OnDeleteItem();

	if (hNextVisibleItem)
		m_ScriptTree.Select(hNextVisibleItem, TVGN_CARET);
	else
		m_ScriptTree.Select(m_ScriptTree.GetRootItem(), TVGN_CARET);

	m_Datamanager.AddQuest(&m_Quest);
}

void CQuestScripterDlg::OnDeleteentiretreeQ()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CQuestScripterDlg::OnInfoStruct()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	HTREEITEM hSelected = m_ScriptTree.GetSelectedItem();
	struct_base *pBase = m_Quest.FindStructFromItem(hSelected);

	if (pBase)
	{
		m_dlgInfo.SetDlgType(pBase->NOL, pBase);
		m_dlgInfo.SetWindowText(m_ScriptTree.GetItemText(hSelected) + L" 의 데이터 정보");
		m_dlgInfo.ShowWindow(SW_SHOW);
	}
}

void CQuestScripterDlg::OnSaveToFile()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog dlg(false, L"*.stg", NULL, OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT, L"STG File(*.stg)|*.stg|", NULL);
	if (dlg.DoModal() == IDOK)
	{
		SaveToFile(dlg.GetPathName().GetBuffer());
	}
}

void CQuestScripterDlg::SaveToFile(TCHAR *szFileName)
{
	if (!m_dlgQuest.m_dlgTileBG.IsHaveStartStage())
	{
		if (IDNO == MessageBox(L"시작 스테이지를 지정하지 않으셨습니다. 그대로 저장하시겠습니까?", L"알림", MB_YESNO))
		{
			ChangeDialog(m_ScriptTree.GetRootItem());
			return;
		}
	}

	// Quest는 최상위에 있으므로 중복되는 값들이 없다.
	// 그래서 DlgQuest에 SetValue, GetValue 할필요 없이 저장 전에 값을 받으면된다.
	m_dlgQuest.m_dlgTileBG.CopyToQuest(&m_Quest);

	SaveSelectedBefore();

	HTREEITEM hSelItem = m_ScriptTree.GetSelectedItem();
	if (hSelItem)
	{
		struct_base *pBase = m_Quest.FindStructFromItem(hSelItem);
		if (pBase)
		{
			if (!pBase->Verify())
			{
				StaticInfoWithMessageBox(L"필수 입력 데이터를 입력해주세요.(ex.StageMap.. 등등)", L"저장실패", MB_OK);
				return;
			}

			::_wunlink(szFileName);

			HANDLE hWrite = CreateFile(szFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			if (INVALID_HANDLE_VALUE == hWrite)
			{
				MessageBox(L"저장 할 수 없습니다. 파일이름을 변경해보시거나 디스크 공간을 확인해 보세요.", L"알림", MB_OK);
				return;
			}
			int iTG_ID = 0;
			pBase->SaveToFile(hWrite, 0, 0, iTG_ID);

			CloseHandle(hWrite);

			CString strMsg = L"저장을 완료하였습니다.";
			switch (pBase->NOL)
			{
			case QUESTX:
				strMsg = L"Quest 정보의 " + strMsg;
				break;
			case STAGEX:
				strMsg = L"Stage 정보의 " + strMsg;
				break;
			}
			StaticInfoWithMessageBox(strMsg, L"저장성공", MB_OK);
		}
		else
		{
			StaticInfoWithMessageBox(L"트리 내부 구조에 문제가 있습니다.", L"저장실패", MB_OK);
		}
	}
	else
	{
		StaticInfoWithMessageBox(L"트리를 다시 선택해 주세요.", L"저장실패", MB_OK);
	}
}

void CQuestScripterDlg::AddChild(HTREEITEM hSelected, HTREEITEM hNewChild, NamesOfLevel NOL_Of_NewChild)
{
	struct_base *SelectBase = m_Quest.FindStructFromItem(hSelected);
	if (SelectBase)
	{
		CString strItemName;
		switch (SelectBase->NOL)
		{
		case QUESTX:
			((Quest*)SelectBase)->AddBlankChild(hNewChild, NOL_Of_NewChild);
			switch (NOL_Of_NewChild)
			{
			case STAGEX:
				strItemName.Format(L"%s%d", STR_STAGE, ((Quest*)SelectBase)->vtStage.size() - 1);
				break;
			}
			if (hNewChild) m_ScriptTree.SetItemText(hNewChild, strItemName.GetBuffer());
			break;
		}

		m_ScriptTree.Select(hNewChild, TVGN_CARET, 0);
	}
}

void CQuestScripterDlg::AddSibling(HTREEITEM hSelected, HTREEITEM hNewChild, HTREEITEM hParent)
{
	if (m_ScriptTree.GetRootItem() == hSelected)
	{
		StaticInfoWithMessageBox(L"퀘스트는 이웃구조를 생성할 수 없습니다.", L"알림", MB_OK);
		return;
	}

	struct_base *SelectParents = m_Quest.FindStructFromItem(hParent);
	struct_base *SelectBase = m_Quest.FindStructFromItem(hSelected);
	if (SelectBase && SelectParents)
	{
		CString strItemName;
		switch (SelectBase->NOL)
		{
		case QUESTX:
			break;
		case STAGEX:
			((Quest*)SelectParents)->AddBlankChild(hNewChild, SelectBase->NOL);
			strItemName.Format(L"%s%d", STR_STAGE, ((Quest*)SelectParents)->vtStage.size() - 1);
			m_ScriptTree.SetItemText(hNewChild, strItemName.GetBuffer());
			break;
		}

		m_ScriptTree.Select(hNewChild, TVGN_CARET);
	}
}

void CQuestScripterDlg::MoveSibling(HTREEITEM hSelected, HTREEITEM hNewPos)
{
	struct_base *Selected = m_Quest.FindStructFromItem(hSelected);
	struct_base *NewPos = m_Quest.FindStructFromItem(hNewPos);

	if (Selected->NOL != NewPos->NOL)
	{
		StaticInfoWithMessageBox(L"이웃구조끼리만 이동이 가능합니다", L"알림", MB_OK);
		return;
	}

	HTREEITEM hNewParent = m_ScriptTree.GetParentItem(hNewPos);
	struct_base *NewParentPos = m_Quest.FindStructFromItem(hNewParent);

	if (Selected && NewPos && NewParentPos)
	{
		switch (Selected->NOL)
		{
		case QUESTX:
			break;
		case STAGEX:
		{
			Stage *Temp = new Stage;
			*Temp = *((Stage*)Selected);
			m_Quest.DeleteItem(hSelected);
			((Quest*)NewParentPos)->vtStage.push_back(Temp);
		}
		break;
		}
	}
}

void CQuestScripterDlg::ChangeDialog(HTREEITEM hSelected)
{
	SaveSelectedBefore();
	struct_base *Selected = m_Quest.FindStructFromItem(hSelected);
	if (!Selected) return;
	m_hSelectedBefore = Selected->MatchKey;

	HideAllDialog();

	switch (Selected->NOL)
	{
	case QUESTX:
		m_dlgQuest.ShowWindow(SW_SHOW);
		break;
	case STAGEX:
		m_dlgStage.SetValue(((Stage*)Selected));
		m_dlgStage.ShowWindow(SW_SHOW);
		break;
	}
}

void CQuestScripterDlg::HideAllDialog(void)
{
	m_dlgQuest.ShowWindow(SW_HIDE);
	m_dlgStage.ShowWindow(SW_HIDE);
}

void CQuestScripterDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//OnOK();
}

void CQuestScripterDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (IDYES == MessageBox(L"정말 프로그램을 종료하시겠습니까? 작성하던 데이터는 보존이 되지 않습니다.", L"종료여부확인", MB_YESNO))
		OnCancel();
}

void CQuestScripterDlg::SaveSelectedBefore(void)
{
	struct_base *pBase = m_Quest.FindStructFromItem(m_hSelectedBefore);
	if (!pBase) return;

	// Dialog창이 바뀔 때 마다 저장이 되기 대문에 궂이 하위구조까지 다 저장할 필요가 없다.
	// 그래서 현재 구조의 변수 정보만 저장하고, 배열정보는 그대로 둔다.
	switch (pBase->NOL)
	{
	case QUESTX:
	{
		Quest quest;
		//m_dlgQuest.GetValue(&quest);
		//((Quest*)pBase)->ShallowCopy(quest);
	}
	break;
	case STAGEX:
	{
		Stage stage;
		m_dlgStage.GetValue(&stage);
		((Stage*)pBase)->ShallowCopy(stage);
	}
	break;
	}
}
int CQuestScripterDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!SiCDlgDirectorySetting()->GetFromRegister())
	{
		OnCustomDirectory();
	}

	std::string strMapPath = GCUTIL_STR::GCStrWideToChar(SiCDlgDirectorySetting()->m_strMapPath.GetBuffer());

	SiKQSLayerManager()->SetScriptDirectory(strMapPath);

	m_dlgFileList.Create(CDlgFileList::IDD, this);
	m_dlgFileList.ShowWindow(SW_HIDE);

	m_dlgTrigger.Create(CDlgTrigger::IDD, this);
	m_dlgTrigger.ShowWindow(SW_HIDE);
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

void CQuestScripterDlg::MadeTree(Quest &quest)
{
	quest.MatchKey = MadeTree(quest.NOL, 0);
	m_ScriptTree.Select(quest.MatchKey, TVGN_CARET, 0);

	for (int i = 0; i < (int)quest.vtStage.size(); i++)
	{
		m_ScriptTree.Select(quest.MatchKey, TVGN_CARET, 0);
		MakeTreeByStage(*quest.vtStage[i], i);
	}
}

void CQuestScripterDlg::MakeTreeByStage(Stage &stage, int iLevel)
{
	stage.MatchKey = MadeTree(stage.NOL, iLevel);
	m_ScriptTree.Select(stage.MatchKey, TVGN_CARET, 0);
}

HTREEITEM CQuestScripterDlg::MadeTree(NamesOfLevel NOL, int iNumOfLv)
{
	switch (NOL)
	{
	case QUESTX:
		return m_ScriptTree.AddChild(STR_QUEST);
		break;
	case STAGEX:
	{
		CString strStage;
		strStage.Format(L"%s%d", STR_STAGE, iNumOfLv);
		return m_ScriptTree.AddChild(strStage);
	}
	break;
	}

	return NULL;
}
void CQuestScripterDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnLButtonDown(nFlags, point);
}

void CQuestScripterDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnRButtonDown(nFlags, point);
}

void CQuestScripterDlg::ReDrawTree(void)
{
	//************************************************************************
	// Quest 구조를 참조 하여, 트리를 다시 그립니다.
	m_ScriptTree.ShowWindow(SW_HIDE);
	m_ScriptTree.DeleteAllItems();
	struct_base *pInfoCurBase = m_Quest.FindStructFromItem(m_dlgInfo.m_hCurTreeItem);
	struct_base *pSelectBefore = m_Quest.FindStructFromItem(m_hSelectedBefore);
	MadeTree(m_Quest);
	if (pInfoCurBase) m_dlgInfo.m_hCurTreeItem = pInfoCurBase->MatchKey;
	if (pSelectBefore) m_hSelectedBefore = pSelectBefore->MatchKey;
	m_ScriptTree.ShowWindow(SW_SHOW);
	//************************************************************************
}

HRESULT CQuestScripterDlg::LoadQuestFromLua(KLuaManager &luaMgr, Quest *quest)
{
	try
	{
		LUA_BEGIN_TABLE("quest", return E_FAIL)
		{
			quest->ClearTileMap();
			if (SUCCEEDED(luaMgr.BeginTable("questtilemap")))
			{
				LUA_GET_VALUE("X_Size", quest->iTileX, return E_FAIL);
				LUA_GET_VALUE("Y_Size", quest->iTileY, return E_FAIL);

				for (int i = 1;; ++i)
				{
					char szRow[MAX_PATH];
					sprintf(szRow, "row%d", i - 1);
					LUA_BEGIN_TABLE(szRow, break)
						for (int j = 1;; ++j)
						{
							char szCol[MAX_PATH];
							sprintf(szCol, "col%d", j - 1);
							LUA_BEGIN_TABLE(szCol, break)
							{
								TileMap tileMap;

								std::string strName;

								LUA_GET_VALUE("Name", strName, break);
								tileMap.strName = GCUTIL_STR::GCStrCharToWide(strName.c_str());

								LUA_GET_VALUE("StageNum", tileMap.iStage, break);
								LUA_GET_VALUE("D_Path", tileMap.iD_Path, break);
								LUA_GET_VALUE("Flag", tileMap.iFlag, break);
								tileMap.iRow = i - 1;
								tileMap.iCol = j - 1;
								quest->SetTileMap(tileMap);
							}
							LUA_END_TABLE(break)
						}
					LUA_END_TABLE(break)
				}
				LUA_END_TABLE(return E_FAIL)
			}
			for (int i = 0;; i++)
			{
				Stage *stage = new Stage;
				CString strStage;
				strStage.Format(L"stage%d", i);
				stage->m_iStage = i;
				if (SUCCEEDED(LoadStageFromLua(luaMgr, strStage, &stage)))
				{
					quest->vtStage.push_back(stage);
				}
				else
				{
					SAFE_DELETE(stage);
					break;
				}
			}
		}
		LUA_END_TABLE(return E_FAIL)
	}
	catch (...)
	{
		MessageBox(L"파일을 불러오던중 예외가 발생하였습니다, 스크립터가 정상적인 동작을 하지 않으면 다시 실행해 주세요.", L"알림", MB_OK);
	}

	return E_FAIL;
}

HRESULT CQuestScripterDlg::LoadStageFromLua(KLuaManager &luaMgr, CString strStage, Stage **stage)
{
	try
	{
		std::string strStage2 = GCUTIL_STR::GCStrWideToChar(strStage);;

		LUA_BEGIN_TABLE(strStage2.c_str(), return E_FAIL)
		{
			string strTemp;
			LUA_GET_VALUE("StageMap", strTemp, return E_FAIL);
			(*stage)->strStageMap = strTemp.c_str();

			LUA_GET_VALUE("StageBGM", strTemp, return E_FAIL);
			(*stage)->strStageBGM = strTemp.c_str();

			(*stage)->vtCategory.clear();
			std::vector<CCategory*> vtTempCategory;
			for (int i = 1;; i++)
			{
				char szCategory[MAX_PATH];
				sprintf(szCategory, "Category%d", i - 1);
				LUA_BEGIN_TABLE(szCategory, break)
				{
					CCategory *pCategory = new CCategory;
					if (FAILED(pCategory->LoadFromFile(luaMgr))) break;
					LoadFromFile_Trigger(luaMgr, &pCategory, (*stage)->m_iStage);
					vtTempCategory.push_back(pCategory);
				}
				LUA_END_TABLE(break)
			}
			for (int i = 0; i < (int)vtTempCategory.size(); ++i)
			{
				for (int j = 0; j < (int)vtTempCategory[i]->m_vtBaseTrigger.size(); ++j)
				{
					// 똑같은 이름의 트리거가 있나 확인 후 있으면 트리거 이름을 바꾼다.
					std::wstring strOriginal = vtTempCategory[i]->m_vtBaseTrigger[j]->m_strTGName;
					while (FindEqualTriggerName(vtTempCategory[i]->m_vtBaseTrigger[j]->m_strTGName))
					{
						vtTempCategory[i]->m_vtBaseTrigger[j]->m_strTGName += L"_Copy";
					}

					// 똑같은 이름의 트리거가 발견되어 변경했다면 TriggerOnOff들의 트리거 명도 변경
					if (strOriginal != vtTempCategory[i]->m_vtBaseTrigger[j]->m_strTGName)
					{
						// TGTriggerOnOff 놈들을 찾아서 이름을 다 바꿔준다.
						for (int k = 0; k < (int)vtTempCategory.size(); ++k)
							vtTempCategory[k]->ChangeTGOnOffName(vtTempCategory[i]->m_vtBaseTrigger[j]->m_strTGName, strOriginal);
					}
				}
			}
			for (int i = 0; i < (int)vtTempCategory.size(); ++i)
				(*stage)->vtCategory.push_back(vtTempCategory[i]);
		}
		LUA_END_TABLE(return E_FAIL)
	}
	catch (...)
	{
		MessageBox(L"파일을 불러오던중 예외가 발생하였습니다, 스크립터가 정상적인 동작을 하지 않으면 다시 실행해 주세요.", L"알림", MB_OK);
	}

	return S_OK;
}

BOOL CQuestScripterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_hAccelTable != NULL)
	{
		if (TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CQuestScripterDlg::SetWindowMainTitle(CString str)
{
	CString strTitle = L"GC_QuestScipter - " + str;
	SetWindowText(strTitle);
}

void CQuestScripterDlg::OnLoadFromFileFull()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//m_ScriptTree.OnLoadFromFileFull();

	TCHAR Filter[] = L"모든파일(*.*)|*.*|";
	CFileDialog dlg(TRUE, L"*.*(*.*)", L"*.*", OFN_HIDEREADONLY |
		OFN_FILEMUSTEXIST, Filter, NULL);

	CString strPath = SiCDlgDirectorySetting()->m_strQuestScriptPath;
	//strPath += "\\Data\\script";
	dlg.m_ofn.lpstrInitialDir = (LPWSTR)(strPath.GetBuffer());

	try
	{
		if (IDOK == dlg.DoModal())
		{
			LoadFromFileFull(dlg.GetFileName().GetBuffer());
		}
	}
	catch (...)
	{
		MessageBox(L"파일을 불러오던중 예외가 발생하였습니다, 스크립터가 정상적인 동작을 하지 않으면 다시 실행해 주세요.", L"알림", MB_OK);
	}
}

void CQuestScripterDlg::LoadFromFileFull(TCHAR *szFileName)
{
	m_strLastLoadFile = szFileName;

	std::string strFileName = GCUTIL_STR::GCStrWideToChar(szFileName);

	KLuaManager luaMgr;             // 루아클래스
	if (SUCCEEDED(luaMgr.DoFile(strFileName.c_str())))
	{
		m_ScriptTree.Select(m_ScriptTree.GetRootItem(), TVGN_CARET, 1);
		m_Quest.DeleteItem();

		LoadQuestFromLua(luaMgr, &m_Quest);
		m_dlgQuest.m_dlgTileBG.CopyFromQuest(&m_Quest);
		m_dlgQuest.SetRowColText(m_Quest.iTileX, m_Quest.iTileY);
		m_dlgQuest.m_dlgTileBG.SetDirectionMode();      // 타일맵 표시모드를 Direction 모드로 사용한다.

		ReDrawTree();
		SetWindowMainTitle(szFileName);
		m_ScriptTree.Select(m_ScriptTree.GetRootItem(), TVGN_CARET, 1);

		CString strMsg;
		strMsg.Format(L"%s 파일로드를 완료 하였습니다.", szFileName);
		StaticInfoWithMessageBox(strMsg, L"불러오기성공", MB_OK);
	}
	else
	{
		MessageBox(L"정상적인 퀘스트 스크립트 파일이 아닙니다.", L"알림", MB_OK);
	}
	m_Datamanager.AddQuest(&m_Quest);
}

void CQuestScripterDlg::OnLoadFromFile()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//m_ScriptTree.OnLoadFromFile();

	TCHAR Filter[] = L"모든파일(*.*)|*.*|";
	CFileDialog dlg(TRUE, L"*.*(*.*)", L"*.*", OFN_HIDEREADONLY |
		OFN_FILEMUSTEXIST, Filter, NULL);

	CString strPath = SiCDlgDirectorySetting()->m_strQuestScriptPath;
	//strPath += "\\Data\\script";
	dlg.m_ofn.lpstrInitialDir = (LPWSTR)(strPath.GetBuffer());

	if (IDOK == dlg.DoModal())
	{
		LoadFromFile(dlg.GetFileName().GetBuffer());
	}
}

void CQuestScripterDlg::LoadFromFile(TCHAR *szFileName)
{
	m_ScriptTree.Select(m_ScriptTree.GetRootItem(), TVGN_CARET, 1);

	KLuaManager luaMgr;             // 루아클래스

	std::string strFileName = GCUTIL_STR::GCStrWideToChar(szFileName);

	luaMgr.DoFile(strFileName.c_str());

	HTREEITEM hSelectedItem = m_ScriptTree.GetSelectedItem();
	struct_base *base = m_Quest.FindStructFromItem(hSelectedItem);
	if (!base)
	{
		StaticInfoWithMessageBox(L"선택한 트리를 찾을 수 없습니다, 추가할 곳의 트리를 다시 선택 해주십시오.", L"불러오기실패", MB_OK);
		return;
	}

	// 각각의 정보 별로 추가 되는 놈들이 있는지 확인....
	switch (base->NOL)
	{
	case QUESTX:
	{
		int cnt = 0;
		for (int i = 0;; i++)
		{
			Stage *stage = new Stage;
			CString strStage;
			strStage.Format(L"stage%d", i);

			if (SUCCEEDED(LoadStageFromLua(luaMgr, strStage, &stage)))
			{
				((Quest*)base)->vtStage.push_back(stage);
			}
			else
			{
				SAFE_DELETE(stage);
				break;
			}

			cnt++;
		}

		if (!cnt)
		{
			CString strMsg;
			strMsg.Format(L"%s 파일에는 Quest에 추가 할 수 있는 정보가 없습니다.", szFileName);
			StaticInfoWithMessageBox(strMsg, L"불러오기실패", MB_OK);
			return;
		}
	}
	break;
	case STAGEX:
	{
		int cnt = 0;
		if (!cnt)
		{
			CString strMsg;
			strMsg.Format(L"%s 파일에는 Stage에 추가 할 수 있는 정보가 없습니다.", szFileName);
			StaticInfoWithMessageBox(strMsg, L"불러오기실패", MB_OK);
			return;
		}
	}
	break;
	default:
		m_Quest.DeleteItem();
		LoadQuestFromLua(luaMgr, &m_Quest);
		m_dlgQuest.m_dlgTileBG.CopyFromQuest(&m_Quest);
		m_dlgQuest.SetRowColText(m_Quest.iTileX, m_Quest.iTileY);
		m_dlgQuest.m_dlgTileBG.SetDirectionMode();      // 타일맵 표시모드를 Direction 모드로 사용한다.
		break;
	}

	ReDrawTree();
	m_ScriptTree.Select(m_ScriptTree.GetRootItem(), TVGN_CARET, 1);

	CString strMsg;
	strMsg.Format(L"%s 파일 정보 추가를 완료 하였습니다.", szFileName);
	StaticInfoWithMessageBox(strMsg, L"불러오기성공", MB_OK);

	m_Datamanager.AddQuest(&m_Quest);
}

// Save Full Tree
void CQuestScripterDlg::OnSvaeToFileFull()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	HTREEITEM hRootItem = m_ScriptTree.GetRootItem();
	if (hRootItem)
	{
		m_ScriptTree.Select(hRootItem, TVGN_CARET);
		OnSaveToFile();
	}
	else
	{
		StaticInfoWithMessageBox(L"Root Tree 정보를 찾을 수 없습니다. 트리를 정보를 갱신해 주십시오", L"저장오류", MB_OK);
	}
}

// Redraw
void CQuestScripterDlg::OnEditRefresh()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	HTREEITEM hSelectedItem = m_ScriptTree.GetSelectedItem();
	struct_base *pBase = 0;
	if (hSelectedItem) pBase = m_Quest.FindStructFromItem(hSelectedItem);
	ReDrawTree();
	if (pBase) m_ScriptTree.Select(pBase->MatchKey, TVGN_CARET);
}

HRESULT CQuestScripterDlg::LoadPreLoad(void)
{
	SiCObjectDataManager()->ClearObjImgItem();
	SiCObjectDataManager()->ClearEnumItem();

	KLuaManager luaMgr;

	if (luaMgr.DoFile("PreLoadScript/MonsterForToolPallet.stg")) return E_FAIL;
	LUA_BEGIN_TABLE("EGCMonsterForToolPalette", return E_FAIL)
	{
		for (int j = 1;; j++)
		{
			LUA_BEGIN_TABLE(j, break)
			{
				string id, path;
				if (FAILED(luaMgr.GetValue(1, id)))
					break;
				if (FAILED(luaMgr.GetValue(2, path)))
					break;

				CString strID = GCUTIL_STR::GCStrCharToWide(id.c_str());
				CString strPath = GCUTIL_STR::GCStrCharToWide(path.c_str());

				SiCObjectDataManager()->AddEnumItem(OBJECTTYPE_EACTION_MON, strID);
				SiCObjectDataManager()->AddObjImgItem(OBJECTTYPE_EACTION_MON, std::make_pair(strID, strPath));
			}
			LUA_END_TABLE(break);
		}
	}
	LUA_END_TABLE(return E_FAIL)

		if (luaMgr.DoFile("PreLoadScript/PlayerForToolPallet.stg")) return E_FAIL;
	LUA_BEGIN_TABLE("EGCPlayerForToolPalette", return E_FAIL)
	{
		for (int j = 1;; j++)
		{
			LUA_BEGIN_TABLE(j, break)
			{
				string id, path;
				if (FAILED(luaMgr.GetValue(1, id)))
					break;
				if (FAILED(luaMgr.GetValue(2, path)))
					break;

				CString strID = GCUTIL_STR::GCStrCharToWide(id.c_str());
				CString strPath = GCUTIL_STR::GCStrCharToWide(path.c_str());

				SiCObjectDataManager()->AddEnumItem(OBJECTTYPE_EACTION_PLAYER, strID);
				SiCObjectDataManager()->AddObjImgItem(OBJECTTYPE_EACTION_PLAYER, std::make_pair(strID, strPath));
			}
			LUA_END_TABLE(break);
		}
	}
	LUA_END_TABLE(return E_FAIL)

		if (luaMgr.DoFile("PreLoadScript/ObjectForToolPallet.stg")) return E_FAIL;
	LUA_BEGIN_TABLE("EGCObjectForToolPalette", return E_FAIL)
	{
		for (int j = 1;; j++)
		{
			LUA_BEGIN_TABLE(j, break)
			{
				string id, path;
				if (FAILED(luaMgr.GetValue(1, id)))
					break;
				if (FAILED(luaMgr.GetValue(2, path)))
					break;

				CString strID = GCUTIL_STR::GCStrCharToWide(id.c_str());
				CString strPath = GCUTIL_STR::GCStrCharToWide(path.c_str());

				SiCObjectDataManager()->AddEnumItem(OBJECTTYPE_EACTION_OBJECT, strID);
				SiCObjectDataManager()->AddObjImgItem(OBJECTTYPE_EACTION_OBJECT, std::make_pair(strID, strPath));
			}
			LUA_END_TABLE(break);
		}
	}
	LUA_END_TABLE(return E_FAIL)

		if (luaMgr.DoFile("PreLoadScript/DoorForToolPallet.stg")) return E_FAIL;
	LUA_BEGIN_TABLE("EGCDoorForToolPalette", return E_FAIL)
	{
		for (int j = 1;; j++)
		{
			LUA_BEGIN_TABLE(j, break)
			{
				string id, path;
				if (FAILED(luaMgr.GetValue(1, id)))
					break;
				if (FAILED(luaMgr.GetValue(2, path)))
					break;

				CString strID = GCUTIL_STR::GCStrCharToWide(id.c_str());
				CString strPath = GCUTIL_STR::GCStrCharToWide(path.c_str());

				SiCObjectDataManager()->AddEnumItem(OBJECTTYPE_EACTION_DOOR, strID);
				SiCObjectDataManager()->AddObjImgItem(OBJECTTYPE_EACTION_DOOR, std::make_pair(strID, strPath));
			}
			LUA_END_TABLE(break);
		}
	}
	LUA_END_TABLE(return E_FAIL)

		if (luaMgr.DoFile("PreLoadScript/EmbarkForToolPallet.stg")) return E_FAIL;
	LUA_BEGIN_TABLE("EGCEmbarkForToolPalette", return E_FAIL)
	{
		for (int j = 1;; j++)
		{
			LUA_BEGIN_TABLE(j, break)
			{
				string id, path;
				if (FAILED(luaMgr.GetValue(1, id)))
					break;

				CString strID = GCUTIL_STR::GCStrCharToWide(id.c_str());
				SiCObjectDataManager()->AddEnumItem(OBJECTTYPE_EACTION_EMBARK, strID);
			}
			LUA_END_TABLE(break);
		}
	}
	LUA_END_TABLE(return E_FAIL)

		CString strBlankImage;
	strBlankImage = "/Data/Texture/BLANK.bmp";
	SiCObjectDataManager()->AddEnumItem(OBJECTTYPE_EACTION_DAMAGE, L"물기둥");
	SiCObjectDataManager()->AddObjImgItem(OBJECTTYPE_EACTION_DAMAGE, std::make_pair(L"물기둥", strBlankImage));
	SiCObjectDataManager()->AddEnumItem(OBJECTTYPE_EACTION_DAMAGE, L"가스");
	SiCObjectDataManager()->AddObjImgItem(OBJECTTYPE_EACTION_DAMAGE, std::make_pair(L"가스", strBlankImage));
	SiCObjectDataManager()->AddEnumItem(OBJECTTYPE_EACTION_DAMAGE, L"큰가스");
	SiCObjectDataManager()->AddObjImgItem(OBJECTTYPE_EACTION_DAMAGE, std::make_pair(L"큰가스", strBlankImage));
	SiCObjectDataManager()->AddEnumItem(OBJECTTYPE_EACTION_DAMAGE, L"불기둥");
	SiCObjectDataManager()->AddObjImgItem(OBJECTTYPE_EACTION_DAMAGE, std::make_pair(L"불기둥", strBlankImage));
	SiCObjectDataManager()->AddEnumItem(OBJECTTYPE_EACTION_DAMAGE, L"파이어볼");
	SiCObjectDataManager()->AddObjImgItem(OBJECTTYPE_EACTION_DAMAGE, std::make_pair(L"파이어볼", strBlankImage));

	return S_OK;
}

// 몬스터 추가하는 윈도우를 활성화 시킨다. (D3D용 윈도우)
LRESULT CQuestScripterDlg::OnMonsterAddWindow(WPARAM wParam, LPARAM lParam)
{
	return ShowMapToObjectWindow((int)TG_TYPE_EACTION, (int)OBJECTTYPE_EACTION_MON, wParam, lParam);
}

LRESULT CQuestScripterDlg::OnPlayerAddWindow(WPARAM wParam, LPARAM lParam)
{
	return ShowMapToObjectWindow((int)TG_TYPE_EACTION, (int)OBJECTTYPE_EACTION_PLAYER, wParam, lParam);
}

LRESULT CQuestScripterDlg::OnMonsterSeldWindow(WPARAM wParam, LPARAM lParam)
{
	return ShowMapToObjectWindow((int)TG_TYPE_CONDITION, (int)OBJECTTYPE_CONDITION_SELECT_MON, wParam, lParam);
}

LRESULT CQuestScripterDlg::OnAreaSelWindow(WPARAM wParam, LPARAM lParam)
{
	return ShowMapToObjectWindow((int)TG_TYPE_CONDITION, (int)OBJECTTYPE_CONDITION_SELECT_AREA, wParam, lParam);
}

LRESULT CQuestScripterDlg::OnPosSelWindow(WPARAM wParam, LPARAM lParam)
{
	m_dlgMapToObject.m_PosFunctionManager.SetHeight((int)wParam);
	return ShowMapToObjectWindow((int)TG_TYPE_CONDITION, (int)OBJECTTYPE_CONDITION_SELECT_POS, wParam, lParam);
}

LRESULT CQuestScripterDlg::OnObjectAddWindow(WPARAM wParam, LPARAM lParam)
{
	return ShowMapToObjectWindow((int)TG_TYPE_EACTION, (int)OBJECTTYPE_EACTION_OBJECT, wParam, lParam);
}

LRESULT CQuestScripterDlg::OnGateObjectAddWindow(WPARAM wParam, LPARAM lParam)
{
	return ShowMapToObjectWindow((int)TG_TYPE_EACTION, (int)OBJECTTYPE_EACTION_DOOR, wParam, lParam);
}

LRESULT CQuestScripterDlg::OnDamageObjectAddWindow(WPARAM wParam, LPARAM lParam)
{
	return ShowMapToObjectWindow((int)TG_TYPE_EACTION, (int)OBJECTTYPE_EACTION_DAMAGE, wParam, lParam);
}

LRESULT CQuestScripterDlg::ShowMapToObjectWindow(int iType, int iPaletteMode, WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hSelectedItem = m_ScriptTree.GetSelectedItem();
	if (!hSelectedItem) { StaticInfoWithMessageBox(L"트리를 다시 선택해 주십시오", L"알림", MB_OK); return E_FAIL; }

	struct_base *pSelBase = m_Quest.FindStructFromItem(hSelectedItem);
	if (!pSelBase) { StaticInfoWithMessageBox(L"선택된 트리구조를 찾을 수 없습니다.", L"알림", MB_OK); return E_FAIL; }

	Stage *pStage = (Stage*)pSelBase;
	SiCDlgDirectorySetting()->GetFromRegister();
	CString strFilePath = SiCDlgDirectorySetting()->m_strMapPath + L"\\";
	strFilePath += pStage->strStageMap;

	HANDLE hRead = CreateFile(strFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hRead)
	{
		strFilePath += L" 파일을 찾을 수 없습니다.(맵파일 경로를 올바르게 지정했는지 확인해주세요.)";
		StaticInfoWithMessageBox(strFilePath, L"파일경로오류", MB_OK);
		return E_FAIL;
	}
	CloseHandle(hRead);

	try
	{
		m_dlgMapToObject.SetQuestScripterDlg(this);
		m_dlgMapToObject.SetDlgTGContent(&m_dlgTrigger.m_dlgTriggerContent);

		//************************************************************************
		// 필요한 오브젝트 리스트를 세팅해둔다.
		//************************************************************************
		std::vector<CBaseEventAction*> vtEActionList;
		vtEActionList.clear();
		GetTGEActionListByType(TG_EACTION_MON_GENNER, vtEActionList);
		m_dlgMapToObject.SetMonsterList(vtEActionList);

		vtEActionList.clear();
		GetTGEActionListByType(TG_EACTION_PLAYERPOSITION, vtEActionList);
		m_dlgMapToObject.SetPlayerPosition(vtEActionList);

		vtEActionList.clear();
		GetTGEActionListByType(TG_EACTION_OBJECTGENERATER, vtEActionList);
		m_dlgMapToObject.SetObjectList(vtEActionList);

		vtEActionList.clear();
		GetTGEActionListByType(TG_EACTION_GATE_OBJECT_GEN, vtEActionList);
		m_dlgMapToObject.SetGateObjectList(vtEActionList);

		vtEActionList.clear();
		GetTGEActionListByType(TG_EACTION_ADD_DAMAGE, vtEActionList);
		m_dlgMapToObject.SetDamageObjectList(vtEActionList);

		m_dlgMapToObject.SetTGType((TG_TYPE)iType);
		switch (iType)
		{
		case TG_TYPE_EACTION:
			m_dlgMapToObject.SetObjectType_EAction((OBJECT_TYPE_EACTION)iPaletteMode);
			break;
		case TG_TYPE_CONDITION:
			m_dlgMapToObject.SetObjectType_Condition((OBJECT_TYPE_CONDITION)iPaletteMode);
			break;
		}

		m_dlgMapToObject.LoadMapToObject(strFilePath);
		m_dlgMapToObject.ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		m_dlgMapToObject.ShowWindow(SW_HIDE);
		MessageBox(L"맵을 표시하는데 문제가 있습니다.", L"알림", MB_OK);
	}

	return S_OK;
}

LRESULT CQuestScripterDlg::OnChangeSequence(WPARAM wParam, LPARAM lParam)
{
	CDlgChangeSequence dlgChangeSec;

	HTREEITEM hSelItem = m_ScriptTree.GetSelectedItem();
	if (!hSelItem) { StaticInfoWithMessageBox(L"트리를 다시 선택해 주십시오", L"알림", MB_OK); return E_FAIL; }

	struct_base *pSelBase = m_Quest.FindStructFromItem(hSelItem);
	if (!pSelBase) { StaticInfoWithMessageBox(L"선택된 트리구조를 찾을 수 없습니다.", L"알림", MB_OK); return E_FAIL; }

	switch (pSelBase->NOL)
	{
	case STAGEX:
		break;
	default:
		StaticInfoWithMessageBox(L"Stage, StageLv, Sequence 만 순서를 변경할 수 있습니다.", L"알림", MB_OK);
		return E_FAIL;
		break;
	}

	HTREEITEM hSelParentItem = m_ScriptTree.GetParentItem(hSelItem);
	if (!hSelParentItem) { StaticInfoWithMessageBox(L"선택된 트리의 상위트리를 찾을 수 없습니다..", L"알림", MB_OK); return E_FAIL; }

	struct_base *pSelParentBase = m_Quest.FindStructFromItem(hSelParentItem);
	if (!pSelParentBase) { StaticInfoWithMessageBox(L"선택된 트리의 상위트리 구조를 찾을 수 없습니다.", L"알림", MB_OK); return E_FAIL; }

	// 변경가능한 Index를 추출한다.
	int iMyIndex;
	std::vector<int> vtSequenceList;
	switch (pSelBase->NOL)
	{
	case STAGEX:
	{
		Quest *pQuest = (Quest*)pSelParentBase;
		Stage *pStage = (Stage*)pSelBase;

		for (int i = 0; i < (int)pQuest->vtStage.size(); i++)
		{
			if (pQuest->vtStage[i]->MatchKey != pStage->MatchKey)
				vtSequenceList.push_back(i);
			else
				iMyIndex = i;
		}
	}
	break;
	}

	dlgChangeSec.SetChangeSequenceList(vtSequenceList);

	if (IDOK == dlgChangeSec.DoModal())
	{
		bool bChanged = false;
		switch (pSelBase->NOL)
		{
		case STAGEX:
		{
			Quest *pQuest = (Quest*)pSelParentBase;
			Stage *pStage = (Stage*)pSelBase;

			Stage *hTemp = new Stage;
			*hTemp = *pStage;
			*pStage = *pQuest->vtStage[dlgChangeSec.m_iSequence];
			*pQuest->vtStage[dlgChangeSec.m_iSequence] = *hTemp;
			SAFE_DELETE(hTemp);
			m_dlgQuest.m_dlgTileBG.DeleteStage(iMyIndex);
			m_dlgQuest.m_dlgTileBG.DeleteStage(dlgChangeSec.m_iSequence);
			bChanged = true;
		}
		break;
		}

		if (bChanged)
		{
			ReDrawTree();
			m_ScriptTree.Select(pSelBase->MatchKey, TVGN_CARET);
		}
	}

	vtSequenceList.clear();

	m_Datamanager.AddQuest(&m_Quest);

	return S_OK;
}

void CQuestScripterDlg::OnChangeSequence()
{
	OnChangeSequence(0, 0);
}

LRESULT CQuestScripterDlg::OnEnableMessage(WPARAM wParam, LPARAM lParam)
{
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP_MYTREE));
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	//////////////////////////////////////////////////////////////////////////
	// Change_Sequence 활성화 결정
	HTREEITEM hItem = m_ScriptTree.GetSelectedItem();
	struct_base *pBase = 0;
	pBase = m_Quest.FindStructFromItem(hItem);
	if (!pBase) return E_FAIL;

	switch (pBase->NOL)
	{
	case STAGEX:
		break;
	default:
		pPopup->EnableMenuItem(ID_CHANGE_SEQUENCE, MF_BYCOMMAND | MF_GRAYED);
		break;
	}
	//////////////////////////////////////////////////////////////////////////
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, (int)wParam, (int)lParam, this);

	return S_OK;
}

LRESULT CQuestScripterDlg::OnChangeDialog(WPARAM wParam, LPARAM lParam)
{
	ChangeDialog(m_ScriptTree.GetSelectedItem());

	return S_OK;
}

void CQuestScripterDlg::OnTreeCut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	HTREEITEM hSelected = m_ScriptTree.GetSelectedItem();

	if (!hSelected)
	{
		StaticInfoWithMessageBox(L"선택된 트리가 없습니다. 트리를 선택해주세요.", L"트리선택", MB_OK);
		return;
	}

	HTREEITEM hSelectedParent = m_ScriptTree.GetParentItem(hSelected);

	struct_base *pBase = m_Quest.FindStructFromItem(hSelected);
	if (!pBase)
	{
		StaticInfoWithMessageBox(L"선택된 트리를 찾을 수 없습니다. 다시 선택해 주세요.", L"트리선택", MB_OK);
		return;
	}

	m_ScriptTree.Select(hSelected, TVGN_CARET);
	m_LastUse_ClipBoard = pBase->NOL;
	switch (pBase->NOL)
	{
	case QUESTX:
	{
		m_Quest_ClipBoard.DeleteItem();
		m_Quest_ClipBoard = *(Quest*)pBase;
		m_ScriptTree.DeleteItemM(pBase->MatchKey);
		m_Quest.DeleteItem(pBase->MatchKey);
	}
	break;
	case STAGEX:
	{
		m_Stage_ClipBoard.DeleteItem();
		m_Stage_ClipBoard = *(Stage*)pBase;
		m_ScriptTree.DeleteItemM(pBase->MatchKey);
		m_Quest.DeleteItem(pBase->MatchKey);
	}
	break;
	}

	struct_base *pSelectParent = m_Quest.FindStructFromItem(hSelectedParent);
	//ReDrawTree();
	if (pSelectParent)
		m_ScriptTree.Select(pSelectParent->MatchKey, TVGN_CARET);
	m_Datamanager.AddQuest(&m_Quest);
}

void CQuestScripterDlg::OnTreeCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//m_ScriptTree.OnTreeCopy();
	HTREEITEM hSelected = m_ScriptTree.GetSelectedItem();
	if (!hSelected)
	{
		StaticInfoWithMessageBox(L"선택된 트리가 없습니다. 트리를 선택해주세요.", L"트리선택", MB_OK);
		return;
	}

	struct_base *pBase = m_Quest.FindStructFromItem(hSelected);
	if (!pBase)
	{
		StaticInfoWithMessageBox(L"선택된 트리를 찾을 수 없습니다. 다시 선택해 주세요.", L"트리선택", MB_OK);
		return;
	}

	m_ScriptTree.Select(hSelected, TVGN_CARET);
	m_LastUse_ClipBoard = pBase->NOL;
	switch (pBase->NOL)
	{
	case QUESTX:
	{
		m_Quest_ClipBoard.DeleteItem();
		m_Quest_ClipBoard = *(Quest*)pBase;
	}
	break;
	case STAGEX:
	{
		m_Stage_ClipBoard.DeleteItem();
		m_Stage_ClipBoard = *(Stage*)pBase;
	}
	break;
	}
	m_ScriptTree.Select(hSelected, TVGN_CARET);
}

void CQuestScripterDlg::OnTreePaste()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//m_ScriptTree.OnTreePaste();

	if (NULL == m_ScriptTree.GetRootItem())
	{
		m_Quest.DeleteItem();
		m_Quest = m_Quest_ClipBoard;
		ReDrawTree();
		return;
	}

	HTREEITEM hSelected = m_ScriptTree.GetSelectedItem();
	if (!hSelected)
	{
		StaticInfoWithMessageBox(L"선택된 트리가 없습니다. 트리를 선택해주세요.", L"트리선택", MB_OK);
		return;
	}

	struct_base *pBase = m_Quest.FindStructFromItem(hSelected);

	if (!pBase)
	{
		StaticInfoWithMessageBox(L"선택된 트리를 찾을 수 없습니다. 다시 선택해 주세요.", L"트리선택", MB_OK);
		return;
	}

	m_ScriptTree.Select(hSelected, TVGN_CARET);
	int cnt = 0;
	switch (pBase->NOL)
	{
	case QUESTX:
	{
		if (STAGEX == m_LastUse_ClipBoard)
		{
			Stage *pStage = new Stage;
			*pStage = m_Stage_ClipBoard;

			for (int i = 0; i < (int)pStage->vtCategory.size(); ++i)
			{
				CCategory *pCategory = pStage->vtCategory[i];

				for (int j = 0; j < (int)pCategory->m_vtBaseTrigger.size(); ++j)
				{
					int iCopyCnt = 0;
					CBaseTrigger *pBaseTrigger = pCategory->m_vtBaseTrigger[j];
					CString strTGName = pBaseTrigger->m_strTGName.c_str();
					CQuestScripterDlg *pDlgScript = (CQuestScripterDlg*)AfxGetMainWnd();
					if (!pDlgScript) return;
					do
					{
						strTGName.Format(L"%s 복사 %d", pBaseTrigger->m_strTGName.c_str(), iCopyCnt++);
					} while (NULL != pDlgScript->FindTriggerByName(strTGName.GetBuffer()));
					pBaseTrigger->m_strTGName = strTGName.GetBuffer();
				}
			}

			((Quest*)pBase)->vtStage.push_back(pStage);
			cnt++;
		}

		if (!cnt)
			StaticInfoWithMessageBox(L"현재 클립보드에는 Quest에 추가 할 수 있는 정보가 없습니다.", L"붙여넣기실패", MB_OK);
	}
	break;
	case STAGEX:
	{
		if (!cnt)
			StaticInfoWithMessageBox(L"현재 클립보드에는 Stage에 추가 할 수 있는 정보가 없습니다.", L"붙여넣기실패", MB_OK);
	}
	break;
	}

	if (cnt > 0) ReDrawTree();
	m_ScriptTree.Select(pBase->MatchKey, TVGN_CARET);
	m_Datamanager.AddQuest(&m_Quest);
}

// 부모 구조로 이동하는 것임.
LRESULT CQuestScripterDlg::OnShiftTreeSelect(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hSelected = m_ScriptTree.GetSelectedItem();
	HTREEITEM hSelItem = NULL;
	switch (wParam)
	{
	case VK_UP:
		hSelItem = m_ScriptTree.GetPrevSiblingItem(hSelected);
		break;
	case VK_DOWN:
		hSelItem = m_ScriptTree.GetNextSiblingItem(hSelected);
		break;
	case VK_LEFT:
		hSelItem = m_ScriptTree.GetParentItem(hSelected);
		break;
	case VK_RIGHT:
		hSelItem = m_ScriptTree.GetChildItem(hSelected);
		break;
	}

	if (hSelItem) m_ScriptTree.Select(hSelItem, TVGN_CARET);

	return S_OK;
}

LRESULT CQuestScripterDlg::OnAddchild_Enter_ForMsg(WPARAM wParam, LPARAM lParam)
{
	OnAddchild_Enter();
	return S_OK;
}

LRESULT CQuestScripterDlg::OnAddSibling_SpaceBar_ForMsg(WPARAM wParam, LPARAM lParam)
{
	OnAddSibling_SpaceBar();
	return S_OK;
}

LRESULT CQuestScripterDlg::OnDeleteSubTree_ForMsg(WPARAM wParam, LPARAM lParam)
{
	OnDeletesubtreeQ();
	return S_OK;
}

LRESULT CQuestScripterDlg::OnMoveSibling_forMsg(WPARAM wParam, LPARAM lParam)
{
	MoveSibling(m_ScriptTree.m_hitemDrag, m_ScriptTree.m_hitemDrop);
	return S_OK;
}

LRESULT CQuestScripterDlg::OnAddchildQ_ForMsg(WPARAM wParam, LPARAM lParam)
{
	OnAddchildQ();
	return S_OK;
}

LRESULT CQuestScripterDlg::OnAddSiblingQ_ForMsg(WPARAM wParam, LPARAM lParam)
{
	OnSiblingchildQ();
	return S_OK;
}

LRESULT CQuestScripterDlg::OnInfoStruct_ForMsg(WPARAM wParam, LPARAM lParam)
{
	OnInfoStruct();
	return S_OK;
}

LRESULT CQuestScripterDlg::OnSaveToFile_ForMsg(WPARAM wParam, LPARAM lParam)
{
	OnSaveToFile();
	return S_OK;
}

LRESULT CQuestScripterDlg::OnSaveToFileFull_ForMsg(WPARAM wParam, LPARAM lParam)
{
	OnSvaeToFileFull();
	return S_OK;
}

LRESULT CQuestScripterDlg::OnLoadFromFile_ForMsg(WPARAM wParam, LPARAM lParam)
{
	OnLoadFromFile();
	return S_OK;
}

LRESULT CQuestScripterDlg::OnLoadFromFileFull_ForMsg(WPARAM wParam, LPARAM lParam)
{
	OnLoadFromFileFull();
	return S_OK;
}

LRESULT CQuestScripterDlg::OnNewTree(WPARAM wParam, LPARAM lParam)
{
	OnTreeNew();
	return S_OK;
}

LRESULT CQuestScripterDlg::OnCutTree(WPARAM wParam, LPARAM lParam)
{
	OnTreeCut();
	return S_OK;
}

LRESULT CQuestScripterDlg::OnCopyTree(WPARAM wParam, LPARAM lParam)
{
	OnTreeCopy();
	return S_OK;
}

LRESULT CQuestScripterDlg::OnPasteTree(WPARAM wParam, LPARAM lParam)
{
	OnTreePaste();
	return S_OK;
}

void CQuestScripterDlg::OnEditUndo()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (!m_Datamanager.PrevQuest(&m_Quest))
		m_stInfo.SetWindowText(L"더 이상 Undo할 것이 없습니다.");
	ReDrawTree();
}

void CQuestScripterDlg::OnEditRedo()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (!m_Datamanager.NextQuest(&m_Quest))
		m_stInfo.SetWindowText(L"더 이상 Redo할 것이 없습니다.");
	ReDrawTree();
}

int CQuestScripterDlg::StaticInfoWithMessageBox(CString strText, CString strCaption, UINT nType)
{
	m_stInfo.SetWindowText(strText);
	return MessageBox(strText, strCaption, MB_OK);
}

LRESULT CQuestScripterDlg::LoadIconLayerPart(WPARAM wParam, LPARAM lParam)
{
	std::map<CString, CString>::iterator itorMon = SiCObjectDataManager()->GetObjImgItem(OBJECTTYPE_EACTION_MON).begin();
	for (; itorMon != SiCObjectDataManager()->GetObjImgItem(OBJECTTYPE_EACTION_MON).end(); itorMon++)
	{
		CString strFullPath = GCUTIL_STR::GCStrCharToWide(g_Path);
		strFullPath += (*itorMon).second;
		D3DXIMAGE_INFO ScrInfo;
		D3DXGetImageInfoFromFile(strFullPath, &ScrInfo);
		TCHAR szFileName[256], szEXT[256];
		_wsplitpath(strFullPath.GetBuffer(), 0, 0, szFileName, szEXT);
		CString strFileName = szFileName;
		strFileName += szEXT;

		KQSLayerPart *pPart = new KQSLayerPart;
		pPart->SetPartCategory("MonsterForToolPallet.stg");

		CString path = (*itorMon).first;
		std::string strTemp = GCUTIL_STR::GCStrWideToChar(path.GetBuffer());

		pPart->SetPartName(strTemp);

		KLayerRect<int> rtiLayerRect;
		rtiLayerRect.m_X = 0;
		rtiLayerRect.m_Y = 0;
		rtiLayerRect.m_Width = ScrInfo.Width;
		rtiLayerRect.m_Height = ScrInfo.Height;

		strTemp = GCUTIL_STR::GCStrWideToChar(strFileName.GetBuffer());
		pPart->SetTexturePart(strTemp, rtiLayerRect);

		KLayerRect<float> rtfLayerRect;
		rtfLayerRect.m_X = 0.0f;
		rtfLayerRect.m_Y = 0.0f;
		rtfLayerRect.m_Width = (float)ScrInfo.Width;
		rtfLayerRect.m_Height = (float)ScrInfo.Height;
		pPart->SetLayerPos(rtfLayerRect);
		SiKQSLayerManager()->AddLayerPart(pPart);

		//pPart->SetFHWidth( ScrInfo.Width );
		//pPart->SetFHHeight( ScrInfo.Height );
	}

	std::map<CString, CString>::iterator itorPlayer = SiCObjectDataManager()->GetObjImgItem(OBJECTTYPE_EACTION_PLAYER).begin();
	for (; itorPlayer != SiCObjectDataManager()->GetObjImgItem(OBJECTTYPE_EACTION_PLAYER).end(); itorPlayer++)
	{
		CString strFullPath = GCUTIL_STR::GCStrCharToWide(g_Path);
		strFullPath += (*itorPlayer).second;
		D3DXIMAGE_INFO ScrInfo;
		D3DXGetImageInfoFromFile(strFullPath, &ScrInfo);
		TCHAR szFileName[256], szEXT[256];
		_tsplitpath(strFullPath.GetBuffer(), 0, 0, szFileName, szEXT);
		CString strFileName = szFileName;
		strFileName += szEXT;

		KQSLayerPart* pPart = new KQSLayerPart();
		pPart->SetPartCategory("PlayerForToolPallet.stg");

		CString path = (*itorPlayer).first;
		std::string strTemp = GCUTIL_STR::GCStrWideToChar(path.GetBuffer());

		pPart->SetPartName(strTemp);

		KLayerRect<int> rtiLayerRect;
		rtiLayerRect.m_X = 0;
		rtiLayerRect.m_Y = 0;
		rtiLayerRect.m_Width = ScrInfo.Width;
		rtiLayerRect.m_Height = ScrInfo.Height;

		strTemp = GCUTIL_STR::GCStrWideToChar(strFileName.GetBuffer());
		pPart->SetTexturePart(strTemp, rtiLayerRect);

		KLayerRect<float> rtfLayerRect;
		rtfLayerRect.m_X = 0.0f;
		rtfLayerRect.m_Y = 0.0f;
		rtfLayerRect.m_Width = (float)ScrInfo.Width;
		rtfLayerRect.m_Height = (float)ScrInfo.Height;
		pPart->SetLayerPos(rtfLayerRect);
		SiKQSLayerManager()->AddLayerPart(pPart);
		//pPart->SetFHWidth( ScrInfo.Width );
		//pPart->SetFHHeight( ScrInfo.Height );
	}

	std::map<CString, CString>::iterator itorObject = SiCObjectDataManager()->GetObjImgItem(OBJECTTYPE_EACTION_OBJECT).begin();
	for (; itorObject != SiCObjectDataManager()->GetObjImgItem(OBJECTTYPE_EACTION_OBJECT).end(); itorObject++)
	{
		CString strFullPath = GCUTIL_STR::GCStrCharToWide(g_Path);
		strFullPath += (*itorObject).second;
		D3DXIMAGE_INFO ScrInfo;
		D3DXGetImageInfoFromFile(strFullPath, &ScrInfo);
		TCHAR szFileName[256], szEXT[256];
		_tsplitpath(strFullPath.GetBuffer(), 0, 0, szFileName, szEXT);
		CString strFileName = szFileName;
		strFileName += szEXT;

		KQSLayerPart* pPart = new KQSLayerPart();
		pPart->SetPartCategory("ObjectForToolPallet.stg");

		CString path = (*itorObject).first;
		std::string strTemp = GCUTIL_STR::GCStrWideToChar(path.GetBuffer());

		pPart->SetPartName(strTemp);

		KLayerRect<int> rtiLayerRect;
		rtiLayerRect.m_X = 0;
		rtiLayerRect.m_Y = 0;
		rtiLayerRect.m_Width = ScrInfo.Width;
		rtiLayerRect.m_Height = ScrInfo.Height;

		strTemp = GCUTIL_STR::GCStrWideToChar(strFileName.GetBuffer());
		pPart->SetTexturePart(strTemp, rtiLayerRect);

		KLayerRect<float> rtfLayerRect;
		rtfLayerRect.m_X = 0.0f;
		rtfLayerRect.m_Y = 0.0f;
		rtfLayerRect.m_Width = (float)ScrInfo.Width;
		rtfLayerRect.m_Height = (float)ScrInfo.Height;
		pPart->SetLayerPos(rtfLayerRect);
		SiKQSLayerManager()->AddLayerPart(pPart);
		//pPart->SetFHWidth( ScrInfo.Width );
		//pPart->SetFHHeight( ScrInfo.Height );
	}

	std::map<CString, CString>::iterator itorDoor = SiCObjectDataManager()->GetObjImgItem(OBJECTTYPE_EACTION_DOOR).begin();
	for (; itorDoor != SiCObjectDataManager()->GetObjImgItem(OBJECTTYPE_EACTION_DOOR).end(); itorDoor++)
	{
		CString strFullPath = GCUTIL_STR::GCStrCharToWide(g_Path);
		strFullPath += (*itorDoor).second;
		D3DXIMAGE_INFO ScrInfo;
		D3DXGetImageInfoFromFile(strFullPath, &ScrInfo);
		TCHAR szFileName[256], szEXT[256];
		_tsplitpath(strFullPath.GetBuffer(), 0, 0, szFileName, szEXT);
		CString strFileName = szFileName;
		strFileName += szEXT;

		KQSLayerPart* pPart = new KQSLayerPart();
		pPart->SetPartCategory("DoorForToolPallet.stg");

		CString path = (*itorDoor).first;
		std::string strTemp = GCUTIL_STR::GCStrWideToChar(path.GetBuffer());
		pPart->SetPartName(strTemp);

		KLayerRect<int> rtiLayerRect;
		rtiLayerRect.m_X = 0;
		rtiLayerRect.m_Y = 0;
		rtiLayerRect.m_Width = ScrInfo.Width;
		rtiLayerRect.m_Height = ScrInfo.Height;

		strTemp = GCUTIL_STR::GCStrWideToChar(strFileName.GetBuffer());
		pPart->SetTexturePart(strTemp, rtiLayerRect);

		KLayerRect<float> rtfLayerRect;
		rtfLayerRect.m_X = 0.0f;
		rtfLayerRect.m_Y = 0.0f;
		rtfLayerRect.m_Width = (float)ScrInfo.Width;
		rtfLayerRect.m_Height = (float)ScrInfo.Height;
		pPart->SetLayerPos(rtfLayerRect);
		SiKQSLayerManager()->AddLayerPart(pPart);
		//pPart->SetFHWidth( ScrInfo.Width );
		//pPart->SetFHHeight( ScrInfo.Height );
	}

	{
		CString strFullPath = GCUTIL_STR::GCStrCharToWide(g_Path);
		strFullPath += L"/Data/Texture/BLANK.bmp";
		D3DXIMAGE_INFO ScrInfo;
		D3DXGetImageInfoFromFile(strFullPath, &ScrInfo);
		TCHAR szFileName[256], szEXT[256];
		_tsplitpath(strFullPath.GetBuffer(), 0, 0, szFileName, szEXT);
		CString strFileName = szFileName;
		strFileName += szEXT;

		KQSLayerPart* pPart = new KQSLayerPart();
		pPart->SetPartCategory("AndSoOn");
		pPart->SetPartName("BLANK");

		KLayerRect<int> rtiLayerRect;
		rtiLayerRect.m_X = 0;
		rtiLayerRect.m_Y = 0;
		rtiLayerRect.m_Width = ScrInfo.Width;
		rtiLayerRect.m_Height = ScrInfo.Height;

		std::string strTemp = GCUTIL_STR::GCStrWideToChar(strFileName.GetBuffer());
		pPart->SetTexturePart(strTemp, rtiLayerRect);

		KLayerRect<float> rtfLayerRect;
		rtfLayerRect.m_X = 0.0f;
		rtfLayerRect.m_Y = 0.0f;
		rtfLayerRect.m_Width = (float)ScrInfo.Width;
		rtfLayerRect.m_Height = (float)ScrInfo.Height;
		pPart->SetLayerPos(rtfLayerRect);
		SiKQSLayerManager()->AddLayerPart(pPart);
		//pPart->SetFHWidth( ScrInfo.Width );
		//pPart->SetFHHeight( ScrInfo.Height );
	}

	return S_OK;
}

LRESULT CQuestScripterDlg::OnChangeTreeFocus(WPARAM wParam, LPARAM lParam)
{
	if (!wParam) return E_FAIL;
	m_ScriptTree.Select(((struct_base*)wParam)->MatchKey, TVGN_CARET);
	return S_OK;
}

void CQuestScripterDlg::OnHelp()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CDlgHelp dlg;
	dlg.DoModal();
}

void CQuestScripterDlg::OnCollapseAllEntireTree()
{
	m_ScriptTree.OnCollapseAllEntireTree();
}

void CQuestScripterDlg::OnExpandAllTree()
{
	m_ScriptTree.OnExpandAllTree();
}

HRESULT CQuestScripterDlg::LoadFromFile_Trigger(KLuaManager &luaMgr, CCategory **pCategory, int iStage)
{
	for (int i = 1;; i++)
	{
		char szTrigger[MAX_PATH];
		sprintf(szTrigger, "Trigger%d", i - 1);
		LUA_BEGIN_TABLE(szTrigger, break)
		{
			CBaseTrigger *pBaseTrigger = new CBaseTrigger;
			if (FAILED(pBaseTrigger->LoadFromFile(luaMgr))) break;
			LoadFromFile_Event(luaMgr, &pBaseTrigger, iStage);
			LoadFromFile_Condition(luaMgr, &pBaseTrigger, iStage);
			LoadFromFile_Action(luaMgr, &pBaseTrigger, iStage);
			(*pCategory)->m_vtBaseTrigger.push_back(pBaseTrigger);
		}
		LUA_END_TABLE(break)
	}

	return S_OK;
}

HRESULT CQuestScripterDlg::LoadFromFile_Event(KLuaManager &luaMgr, CBaseTrigger **pBaseTrigger, int iStage)
{
	LUA_BEGIN_TABLE("Event", return E_FAIL)
	{
		for (int i = 1;; i++)
		{
			char szFunction[MAX_PATH];
			sprintf(szFunction, "Function%d", i - 1);
			LUA_BEGIN_TABLE(szFunction, break)
			{
				std::string strName, strDesc;
				int iType, iRepeat, iTG_ID;
				LUA_GET_VALUE("FuctionName", strName, break);
				LUA_GET_VALUE("FuctionType", iType, break);
				LUA_GET_VALUE("FuctionDesc", strDesc, break);
				luaMgr.GetValue("Repeat", iRepeat);
				LUA_GET_VALUE("TG_ID", iTG_ID, break);
				CBaseEventAction *pBaseEAction = NULL;
				m_dlgTrigger.m_dlgTriggerContent.m_dlgTGChildGenner.GetAllocTGFunction((CBaseTGFunction**)&pBaseEAction, TG_TYPE_EACTION, iType);

				if (pBaseEAction)
				{
					pBaseEAction->LoadFromFile_Item(luaMgr);
					pBaseEAction->m_enType = (TG_EACTION)iType;
					pBaseEAction->m_iStage = iStage;
					pBaseEAction->m_strName = GCUTIL_STR::GCStrCharToWide(strName.c_str());
					pBaseEAction->m_strDesc = GCUTIL_STR::GCStrCharToWide(strDesc.c_str());
					pBaseEAction->m_bRepeat = (bool)iRepeat;
					pBaseEAction->m_iTGID = iTG_ID;
					(*pBaseTrigger)->m_vtEvent.push_back(pBaseEAction);
				}
			}
			LUA_END_TABLE(break)
		}
	}
	LUA_END_TABLE(return E_FAIL)

		return S_OK;
}

HRESULT CQuestScripterDlg::LoadFromFile_Condition(KLuaManager &luaMgr, CBaseTrigger **pBaseTrigger, int iStage)
{
	LUA_BEGIN_TABLE("Condition", return E_FAIL)
	{
		for (int i = 1;; i++)
		{
			char szFunction[MAX_PATH];
			sprintf(szFunction, "Function%d", i - 1);
			LUA_BEGIN_TABLE(szFunction, break)
			{
				std::string strName, strDesc;
				int iType, iRepeat, iTG_ID;
				LUA_GET_VALUE("FuctionName", strName, break);
				LUA_GET_VALUE("FuctionType", iType, break);
				LUA_GET_VALUE("FuctionDesc", strDesc, break);
				luaMgr.GetValue("Repeat", iRepeat);
				LUA_GET_VALUE("TG_ID", iTG_ID, break);
				CBaseCondition *pBaseCondition = NULL;
				m_dlgTrigger.m_dlgTriggerContent.m_dlgTGChildGenner.GetAllocTGFunction((CBaseTGFunction**)&pBaseCondition, TG_TYPE_CONDITION, iType);
				if (pBaseCondition)
				{
					pBaseCondition->LoadFromFile_Item(luaMgr);
					pBaseCondition->m_enType = (TG_CONDITION)iType;
					pBaseCondition->m_iStage = iStage;
					pBaseCondition->m_strName = GCUTIL_STR::GCStrCharToWide(strName.c_str());
					pBaseCondition->m_strDesc = GCUTIL_STR::GCStrCharToWide(strDesc.c_str());
					pBaseCondition->m_bRepeat = (bool)iRepeat;
					pBaseCondition->m_iTGID = iTG_ID;
					(*pBaseTrigger)->m_vtCondition.push_back(pBaseCondition);
				}
			}
			LUA_END_TABLE(break)
		}
	}
	LUA_END_TABLE(return E_FAIL)

		return S_OK;
}

HRESULT CQuestScripterDlg::LoadFromFile_Action(KLuaManager &luaMgr, CBaseTrigger **pBaseTrigger, int iStage)
{
	LUA_BEGIN_TABLE("Action", return E_FAIL)
	{
		for (int i = 1;; i++)
		{
			char szFunction[MAX_PATH];
			sprintf(szFunction, "Function%d", i - 1);
			LUA_BEGIN_TABLE(szFunction, break)
			{
				std::string strName, strDesc;
				int iType, iRepeat, iTG_ID;
				LUA_GET_VALUE("FuctionName", strName, break);
				LUA_GET_VALUE("FuctionType", iType, break);
				LUA_GET_VALUE("FuctionDesc", strDesc, break);
				luaMgr.GetValue("Repeat", iRepeat);
				LUA_GET_VALUE("TG_ID", iTG_ID, break);
				CBaseEventAction *pBaseEAction = NULL;
				m_dlgTrigger.m_dlgTriggerContent.m_dlgTGChildGenner.GetAllocTGFunction((CBaseTGFunction**)&pBaseEAction, TG_TYPE_EACTION, iType);
				if (pBaseEAction)
				{
					pBaseEAction->LoadFromFile_Item(luaMgr);
					pBaseEAction->m_enType = (TG_EACTION)iType;
					pBaseEAction->m_iStage = iStage;
					pBaseEAction->m_strName = GCUTIL_STR::GCStrCharToWide(strName.c_str());
					pBaseEAction->m_strDesc = GCUTIL_STR::GCStrCharToWide(strDesc.c_str());
					pBaseEAction->m_bRepeat = (bool)iRepeat;
					pBaseEAction->m_iTGID = iTG_ID;
					(*pBaseTrigger)->m_vtAction.push_back(pBaseEAction);
				}
			}
			LUA_END_TABLE(break)
		}
	}
	LUA_END_TABLE(return E_FAIL)

		return S_OK;
}

bool CQuestScripterDlg::FindEqualTriggerName(std::wstring strTGName)
{
	for (int i = 0; i < (int)m_Quest.vtStage.size(); ++i)
	{
		if (m_Quest.vtStage[i]->IsDuplicateTGName(strTGName)) return true;
	}

	return false;
}

CBaseTrigger *CQuestScripterDlg::FindTriggerByName(std::wstring strTGName)
{
	for (int i = 0; i < (int)m_Quest.vtStage.size(); ++i)
	{
		CBaseTrigger *pBaseTrigger = m_Quest.vtStage[i]->GetEqualNameTrigger(strTGName);
		if (pBaseTrigger) return pBaseTrigger;
	}

	return NULL;
}

void CQuestScripterDlg::ChangeTGOnOffName(std::wstring strTGChanged, std::wstring strTGName)
{
	// TGTriggerOnOff 놈들을 찾아서 이름을 다 바꿔준다.
	for (int i = 0; i < (int)m_Quest.vtStage.size(); ++i)
	{
		std::vector<CCategory*> *pvtCategory = &m_Quest.vtStage[i]->vtCategory;
		for (int j = 0; j < (int)pvtCategory->size(); ++j)
			(*pvtCategory)[j]->ChangeTGOnOffName(strTGChanged, strTGName);
	}
}

void CQuestScripterDlg::GetCntOfSimilarTrigger(std::wstring strTGName, std::vector<std::wstring> *vtSimilar)
{
	if (strTGName.empty())  return;

	for (int i = 0; i < (int)m_Quest.vtStage.size(); ++i)
		m_Quest.vtStage[i]->GetSimilarTGName(strTGName, vtSimilar);
}

void CQuestScripterDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CTime time;
	time = CTime::GetCurrentTime();

	CStringA strTime;
	strTime.Format("--%04d-%02d-%02d_%02d-%02d-%02d\r\n", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

	CString strFileName;
	CString strPath = GCUTIL_STR::GCStrCharToWide(g_Path);
	strFileName.Format(L"%s/AutoSave/AutoSave_%d.stg", strPath, time.GetMinute() % 3);

	HANDLE hWrite = CreateFile(strFileName.GetBuffer(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hWrite)
	{
		CString strAutoSave;
		strAutoSave.Format(L"자동저장을 실패 하였습니다. - (%04d-%02d-%02d_%02d-%02d-%02d)"
			, time.GetYear(), time.GetMonth(), time.GetDay()
			, time.GetHour(), time.GetMinute(), time.GetSecond());
		m_stInfo.SetWindowText(strAutoSave);
		return;
	}
	else
	{
		int iTG_ID = 0;
		DWORD dwSize = 0;
		WriteFile(hWrite, strTime.GetBuffer(), strTime.GetLength(), &dwSize, 0);
		m_Quest.SaveToFile(hWrite, 0, 0, iTG_ID);
		CloseHandle(hWrite);

		CString strAutoSave;
		strAutoSave.Format(L"자동저장 하였습니다. - (%04d-%02d-%02d_%02d-%02d-%02d)"
			, time.GetYear(), time.GetMonth(), time.GetDay()
			, time.GetHour(), time.GetMinute(), time.GetSecond());
		m_stInfo.SetWindowText(strAutoSave);
	}

	CDialog::OnTimer(nIDEvent);
}

void CQuestScripterDlg::OnCustomDirectory()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (SiCDlgDirectorySetting()->DoModal())
	{
		std::string strTextureDir = GCUTIL_STR::GCStrWideToChar(SiCDlgDirectorySetting()->m_strMapSubFilePath);
		strTextureDir += "\\";

		if (m_dlgStage.GetSafeHwnd()) m_dlgStage.RefreshMapDriectory();

		if (g_pGCDeviceManager)
			g_pGCDeviceManager->GetMassFileManager()->AddDirectory(strTextureDir);

		if (m_dlgMapToObject.m_strMapScript.GetLength())
			m_dlgMapToObject.LoadMapToObject(m_dlgMapToObject.m_strMapScript);
		//if( m_dlgMapToObject.GetSafeHwnd() ) m_dlgMapToObject.m_bReLoadResource = true;
	}
	ChangeDialog(m_ScriptTree.GetRootItem());
}

void CQuestScripterDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UINT  uNumFiles;
	TCHAR szNextFile[1024];

	// Get the # of files being dropped.
	uNumFiles = DragQueryFile(hDropInfo, -1, NULL, 0);

	for (UINT uFile = 0; uFile < uNumFiles; uFile++)
	{
		// Get the next filename from the HDROP info.
		if (DragQueryFile(hDropInfo, uFile, szNextFile, 1024) > 0)
		{
			SHFILEINFO shInfo;
			SHGetFileInfo(szNextFile, 0, &shInfo, sizeof(SHFILEINFO)
				, SHGFI_TYPENAME | SHGFI_ATTRIBUTES | SHGFI_SYSICONINDEX);

			if (!(shInfo.dwAttributes & SFGAO_FOLDER))
			{
				CString strMsg;
				strMsg.Format(L"%s 파일을 로드하시겠습니까? 기존의 정보는 저장되지 않습니다", szNextFile);
				if (IDYES == MessageBox(strMsg.GetBuffer(), L"알림", MB_YESNO))
					LoadFromFileFull(szNextFile);
				break;
			}
		}
	}

	// Free up memory.
	DragFinish(hDropInfo);

	CDialog::OnDropFiles(hDropInfo);
}

void CQuestScripterDlg::WriteToRegisterRecent(int iIndex, char *szFileName)
{
	if (iIndex < 0 || iIndex >= MAX_REGISTER_INDEX) return;

	HKEY hkey;
	DWORD dwType = REG_DWORD;
	DWORD dwLength = sizeof(DWORD);

	if (ERROR_SUCCESS == RegCreateKeyEx(HKEY_CURRENT_USER, L"KOG_QUESTSCRIPT_DIR",
		0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL))
	{
		// Write the window width/height.  This is just an example,
		// of how to use DXUtil_Write*() functions.

		CString strRecent;
		strRecent.Format(L"Recent%d", iIndex);
		RegSetValueEx(hkey, strRecent.GetBuffer(), 0, REG_SZ, (BYTE*)szFileName, (DWORD)strlen(szFileName));

		RegCloseKey(hkey);
	}
}

void CQuestScripterDlg::ReadFromRegisterRecent(int iIndex, char *szFileName)
{
	HKEY hkey;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, L"KOG_QUESTSCRIPT_DIR",
		0, KEY_ALL_ACCESS, &hkey))
	{
		// Read the stored window width/height.  This is just an example,
		// of how to use DXUtil_Read*() functions.

		DWORD data_size = 2048;

		CString strRecent;
		strRecent.Format(L"Recent%d", iIndex);
		if (ERROR_SUCCESS != RegQueryValueEx(hkey, strRecent.GetBuffer(), 0, 0, (BYTE*)szFileName, &data_size))
		{
			RegSetValueEx(hkey, strRecent.GetBuffer(), 0, REG_SZ, (BYTE*)szFileName, data_size);
			strncpy_lastNull(szFileName, szFileName, data_size);
		}
		RegCloseKey(hkey);
	}
}

void CQuestScripterDlg::GetTGEActionListByType(TG_EACTION enType, std::vector<CBaseEventAction*> &vtList)
{
	std::vector<CCategory*> *pvtCategory = m_dlgTrigger.m_pvtCategory;
	for (int i = 0; i < (int)pvtCategory->size(); i++)
	{
		CCategory *pCategory = (*pvtCategory)[i];
		for (int j = 0; j < (int)pCategory->m_vtBaseTrigger.size(); ++j)
		{
			CBaseTrigger *pBaseTrigger = pCategory->m_vtBaseTrigger[j];
			for (int k = 0; k < (int)pBaseTrigger->m_vtEvent.size(); ++k)
			{
				if (enType == pBaseTrigger->m_vtEvent[k]->m_enType)
					vtList.push_back(pBaseTrigger->m_vtEvent[k]);
			}

			for (int k = 0; k < (int)pBaseTrigger->m_vtAction.size(); ++k)
			{
				if (enType == pBaseTrigger->m_vtAction[k]->m_enType)
					vtList.push_back(pBaseTrigger->m_vtAction[k]);
			}
		}
	}
}

void CQuestScripterDlg::GetTGConditionListByType(TG_CONDITION enType, std::vector<CBaseCondition*> &vtList)
{
	std::vector<CCategory*> *pvtCategory = m_dlgTrigger.m_pvtCategory;
	for (int i = 0; i < (int)pvtCategory->size(); i++)
	{
		CCategory *pCategory = (*pvtCategory)[i];
		for (int j = 0; j < (int)pCategory->m_vtBaseTrigger.size(); ++j)
		{
			CBaseTrigger *pBaseTrigger = pCategory->m_vtBaseTrigger[j];
			for (int k = 0; k < (int)pBaseTrigger->m_vtCondition.size(); ++k)
			{
				if (enType == pBaseTrigger->m_vtCondition[k]->m_enType)
					vtList.push_back(pBaseTrigger->m_vtCondition[k]);
			}
		}
	}
}

LRESULT CQuestScripterDlg::OnFocusWindow(WPARAM wParam, LPARAM lParam)
{
	const bool bTrigger = m_dlgTrigger.IsWindowVisible();
	const bool bMapToObject = m_dlgMapToObject.IsWindowVisible();
	const bool bTGGenner = m_dlgTrigger.m_dlgTriggerContent.m_dlgTGChildGenner.IsWindowVisible();

	EnableWindow(true);
	m_dlgMapToObject.EnableWindow(true);
	m_dlgTrigger.EnableWindow(true);
	m_dlgTrigger.m_dlgTriggerContent.m_dlgTGChildGenner.EnableWindow(true);

	if (bTrigger || bMapToObject || bTGGenner)
	{
		EnableWindow(false);

		if (bMapToObject)
		{
			m_dlgTrigger.EnableWindow(false);
			m_dlgTrigger.m_dlgTriggerContent.m_dlgTGChildGenner.EnableWindow(false);
			m_dlgMapToObject.SetFocus();
		}

		if (bTGGenner)
		{
			m_dlgTrigger.EnableWindow(false);
			m_dlgTrigger.m_dlgTriggerContent.m_dlgTGChildGenner.SetFocus();
		}

		if (bTrigger)
		{
			m_dlgTrigger.SetFocus();
		}
	}
	else
	{
		SetFocus();
	}

	return S_OK;
}

LRESULT CQuestScripterDlg::OnLoadFromFileListFull(WPARAM wParam, LPARAM lParam)
{
	const int iIndex = (int)wParam;
	if (iIndex >= 0 && iIndex < m_dlgFileList.GetItemCount())
	{
		CString strMsg;
		strMsg.Format(L"%s 파일을 로드하시겠습니까? 기존의 정보는 저장되지 않습니다", m_dlgFileList.GetItem(iIndex).first.GetBuffer());
		if (IDYES == MessageBox(strMsg.GetBuffer(), L"알림", MB_YESNO))
			LoadFromFileFull(m_dlgFileList.GetItem(iIndex).first.GetBuffer());
	}

	return S_OK;
}
void CQuestScripterDlg::OnFilelist()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_dlgFileList.IsWindowVisible())
	{
		m_dlgFileList.ShowWindow(SW_HIDE);
	}
	else
	{
		m_dlgFileList.ShowWindow(SW_SHOW);
	}
}

LRESULT CQuestScripterDlg::OnSaveServerScriptFromFileList(WPARAM wParam, LPARAM lParam)
{
	try
	{
		CFileDialog dlg(false, L"*.stg", NULL, OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT, L"STG File(*.stg)|*.stg|", NULL);
		if (dlg.DoModal() == IDOK)
		{
			HANDLE hWrite = CreateFile(dlg.GetPathName(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			if (INVALID_HANDLE_VALUE == hWrite)
			{
				MessageBox(L"저장 할 수 없습니다. 파일이름을 변경해보시거나 디스크 공간을 확인해 보세요.", L"알림", MB_OK);
				return E_FAIL;
			}

			for (int i = 0; i < m_dlgFileList.GetItemCount(); ++i)
			{
				int iTab = 0;
				TCHAR szFileName[512];
				_tsplitpath(m_dlgFileList.GetItem(i).first, 0, 0, szFileName, 0);

				WriteOpenBrace(hWrite, szFileName, iTab);
				{
					SaveScriptForServer(hWrite, m_dlgFileList.GetItem(i).first, 1, iTab + 1);
				}
				WriteCloseBrace(hWrite, iTab, 0);
			}
			CloseHandle(hWrite);

			MessageBox(L"서버 스크립트 생성을 완료 하였습니다.", L"알림", MB_OK);
		}
	}
	catch (...)
	{
		MessageBox(L"서버 스크립트 생성 중 예외가 발생하였습니다.", L"알림", MB_OK);
	}

	return S_OK;
}

HRESULT CQuestScripterDlg::SaveScriptForServer(HANDLE hHandle, CString strFileName, int iLevel, int iTab/* = 0*/)
{
	KLuaManager luaMgr;
	Quest quest;

	std::string strTemp = GCUTIL_STR::GCStrWideToChar(strFileName);

	if (FAILED(luaMgr.DoFile(strTemp.c_str()))) return E_FAIL;

	LoadQuestFromLua(luaMgr, &quest);

	std::vector<CBaseEventAction*> vtMonsterGen;
	quest.GetEqualTypeOfEAction(TG_EACTION_MON_GENNER, vtMonsterGen);
	BOOL debug_bool = false;
	CString strLevel;
	strLevel.Format(L"Lv_%d", iLevel);
	WriteOpenBrace_NoEnter(hHandle, strLevel.GetBuffer(), iTab);       // 아직은 레벨 개념이 없습니다. 그래서 1만 찍어욧.
	{
		for (int i = 0; i < (int)vtMonsterGen.size(); ++i)
		{
			DWORD dwSize;
			CMonsterGenerater *pMonsterGenner = (CMonsterGenerater*)vtMonsterGen[i];
			CStringA strCondition;
			debug_bool = pMonsterGenner->m_bNotChampion;
			if (pMonsterGenner->m_bNotChampion) continue;
			strCondition.Format("{%d, %d, %d, %d, %d}, ", pMonsterGenner->m_iTGID, pMonsterGenner->m_iMonType, pMonsterGenner->m_iMonLevel, pMonsterGenner->m_iStage, pMonsterGenner->m_iBoss);
			//TRACE(L"%d ",pMonsterGenner->m_iBoss);
			WriteFile(hHandle, strCondition, strCondition.GetLength(), &dwSize, 0);
		}
		//TRACE(L"\n");
	}
	WriteCloseBrace(hHandle, iTab, 0);

	return S_OK;
}

HRESULT CQuestScripterDlg::SaveScriptForServer_Subject(HANDLE hHandle, CString strFileName, CStringA strQuestFileName, int iQuestID, int iLevel, int iTab/* = 0*/)
{
	KLuaManager luaMgr;
	Quest quest;

	std::string strTemp = GCUTIL_STR::GCStrWideToChar(strFileName);

	if (FAILED(luaMgr.DoFile(strTemp.c_str())))
	{
		std::string strError = strTemp.c_str();
		strError += " 파일 로드에 실패하였습니다.\n경로를 확인해주세요!!";
		MessageBoxA(NULL, strError.c_str(), "던전파일로드실패!!", MB_OK);
		return E_FAIL;
	}

	LoadQuestFromLua(luaMgr, &quest);

	std::vector<CBaseEventAction*> vtSubject;
	quest.GetEqualTypeOfEAction(TG_EACTION_SUBJECT, vtSubject);

	if (vtSubject.empty())
		return E_FAIL;

	WriteOpenBrace(hHandle, strQuestFileName, iTab - 1);
	WriteToFileScript(hHandle, "ModeID", iQuestID, iTab);
	WriteToFileScript(hHandle, "Difficulty", iLevel, iTab);

	CString strSubBarceTitle = L"SubjectList";
	WriteOpenBrace(hHandle, strSubBarceTitle, iTab);
	{
		for (int iLoop = 0; iLoop < static_cast<int>(vtSubject.size()); ++iLoop)
		{
			CSubject *pSubject = (CSubject*)vtSubject[iLoop];
			WriteOnlyOpenBrace_NoEnter(hHandle, iTab + 1);
			WriteToFileScript_NoEnter(hHandle, "TriggerID", pSubject->m_iTGID, 0);
			WriteOpenBrace_NoEnterAndNoBackTab(hHandle, "\tSubjectID", 0);
			std::vector<std::pair<int, int>>::iterator vecIter = pSubject->m_vecSubject.begin();
			for (; vecIter != pSubject->m_vecSubject.end(); ++vecIter)
			{
				WriteToFileScript_OnlyValue_Number(hHandle, vecIter->first, 0, false);
			}
			WriteCloseBrace_NoEnter(hHandle, 0);
			WriteCloseBrace(hHandle, 1, 0);
		}
	}
	WriteCloseBrace(hHandle, iTab, 0);
	WriteCloseBrace(hHandle, iTab - 1, 0);

	return S_OK;
}

void CQuestScripterDlg::OnFileSavetoserverscriptByQuestsign()
{
	CFileDialog dlgWriteFile(false, L"*.*", NULL, OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT, L"모든파일(*.*)|*.lua|", NULL);
	if (IDOK != dlgWriteFile.DoModal())
	{
		MessageBox(L"파일을 저장 할 수 없습니다. 파일이름을 변경해보시거나 디스크 공간을 확인해 보세요.", L"알림", MB_OK);
		return;
	}

	bool bFileFail = false;
	HANDLE hWrite = CreateFile(dlgWriteFile.GetPathName(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hWrite)
	{
		MessageBox(L"저장 할 수 없습니다. 파일이름을 변경해보시거나 디스크 공간을 확인해 보세요.", L"알림", MB_OK);
		bFileFail = true;
	}

	KLuaManager luaMgr;
	if (!bFileFail)
		bFileFail = LoadQuestSignForServerScript(luaMgr, hWrite);

	CloseHandle(hWrite);

	if (bFileFail)
		MessageBox(L"서버용 퀘스트 스크립트 파일 생성에 성공하였습니다.", L"알림", MB_OK);
	else
		MessageBox(L"서버용 퀘스트 스크립트 파일 생성에 실패하였습니다.", L"알림", MB_OK);
}

void CQuestScripterDlg::OnFileSavetoserverscriptBySubject()
{
	CFileDialog dlgWriteFile(false, L"*.*", NULL, OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT, L"모든파일(*.*)|*.lua|", NULL);
	if (IDOK != dlgWriteFile.DoModal())
	{
		MessageBox(L"파일을 저장 할 수 없습니다. 파일이름을 변경해보시거나 디스크 공간을 확인해 보세요.", L"알림", MB_OK);
		return;
	}

	bool bFileFail = false;
	HANDLE hWrite = CreateFile(dlgWriteFile.GetPathName(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hWrite)
	{
		MessageBox(L"저장 할 수 없습니다. 파일이름을 변경해보시거나 디스크 공간을 확인해 보세요.", L"알림", MB_OK);
		bFileFail = true;
	}

	KLuaManager luaMgr;
	if (!bFileFail)
		bFileFail = LoadQuestSignForServerScript_Subject(luaMgr, hWrite);

	CloseHandle(hWrite);

	if (bFileFail)
		MessageBox(L"서버용 달성과제 스크립트 파일 생성에 성공하였습니다.", L"알림", MB_OK);
	else
		MessageBox(L"서버용 달성과제 스크립트 파일 생성에 실패하였습니다.", L"알림", MB_OK);
}

bool CQuestScripterDlg::LoadQuestSignForServerScript(KLuaManager &luaMgr, HANDLE hWrite)
{
	CStringA strEnumFile = GCUTIL_STR::GCStrWideToChar(SiCDlgDirectorySetting()->m_strQuestScriptPath);
	strEnumFile += "/Enum.stg";

	if (FAILED(luaMgr.DoFile(strEnumFile.GetBuffer())))
	{
		int iIndex = strEnumFile.ReverseFind('\\');
		strEnumFile = strEnumFile.Left(iIndex);
		strEnumFile += "/Enum.stg";
		if (FAILED(luaMgr.DoFile(strEnumFile.GetBuffer())))
		{
			CStringA strErr;
			strErr.Format("%s 파일로드에 실패하였습니다.", strEnumFile);
			MessageBoxA(NULL, strErr.GetBuffer(), "알림", MB_OK);
			return false;
		}
	}

	CStringA strETCFile = GCUTIL_STR::GCStrWideToChar(SiCDlgDirectorySetting()->m_strQuestScriptPath);
	strETCFile += "/etc.stg";

	if (FAILED(luaMgr.DoFile(strETCFile.GetBuffer())))
	{
		int iIndex = strETCFile.ReverseFind('\\');
		strETCFile = strETCFile.Left(iIndex);
		strETCFile += "/etc.stg";
		if (FAILED(luaMgr.DoFile(strETCFile.GetBuffer())))
		{
			CStringA strErr;
			strErr.Format("%s 파일로드에 실패하였습니다.", strETCFile.GetBuffer());
			MessageBoxA(NULL, strErr.GetBuffer(), "알림", MB_OK);
			return false;
		}
	}

	CStringA strQuestSignFile = GCUTIL_STR::GCStrWideToChar(SiCDlgDirectorySetting()->m_strQuestScriptPath);
	strQuestSignFile += "/QuestSign.stg";

	if (FAILED(luaMgr.DoFile(strQuestSignFile.GetBuffer())))
	{
		int iIndex = strQuestSignFile.ReverseFind('\\');
		strQuestSignFile = strQuestSignFile.Left(iIndex);
		strQuestSignFile += "/QuestSign.stg";
		if (FAILED(luaMgr.DoFile(strQuestSignFile.GetBuffer())))
		{
			CStringA strErr;
			strErr.Format("%s 파일로드에 실패하였습니다.", strQuestSignFile.GetBuffer());
			MessageBoxA(NULL, strErr.GetBuffer(), "알림", MB_OK);
			return false;
		}
	}

	LUA_BEGIN_TABLE("quest", return false)
	{
		int iTab = 0;
		WriteOpenBrace(hWrite, "TriggerInfo", iTab);
		{
			for (int i = 1;; i++)
			{
				char szQuestSign[128];
				sprintf(szQuestSign, "QuestSign%d", i - 1);
				LUA_BEGIN_TABLE(szQuestSign, break)
				{
					int iQuestID = 0;
					int iDefNameStrID = 0;
					int iDefDescStrID = 0;
					int iDefLowLevel = 0;
					int iDefHighLevel = 0;
					std::string strdefTGFileName = "";

					LUA_GET_VALUE_DEF("ID", iQuestID, -1);
					LUA_GET_VALUE_DEF("Name", iDefNameStrID, 1191);
					LUA_GET_VALUE_DEF("Desc", iDefDescStrID, 2267);
					LUA_GET_VALUE_DEF("LOW_LEVEL", iDefLowLevel, 0);
					LUA_GET_VALUE_DEF("HIGH_LEVEL", iDefHighLevel, 75);
					LUA_GET_VALUE_DEF("TGFileName", strdefTGFileName, "");
					if (iDefLowLevel > iDefHighLevel) swap(iDefLowLevel, iDefHighLevel);

					// 중요정보 로드실패 처리
					if (-1 == iQuestID)
					{
						std::string strError = szQuestSign;
						strError += "에서 ID로드 오류입니다.";
						MessageBoxA(NULL, strError.c_str(), "QuestSing.stg로드실패!!", MB_OK);
						break;
					}
					if ("" == strdefTGFileName)
					{
						std::string strError = szQuestSign;
						strError += "에서 TGFileName로드 오류입니다.";
						MessageBoxA(NULL, strError.c_str(), "QuestSing.stg로드실패!!", MB_OK);
						break;
					}

					char szFileName[512];
					_splitpath(strdefTGFileName.c_str(), 0, 0, szFileName, 0);

					CStringA strFileName;
					strFileName.Format("--%s", szFileName);
					WriteOpenBrace(hWrite, strFileName, iTab + 1);
					{
						WriteToFileScript(hWrite, "ID", iQuestID, iTab + 2);

						LUA_BEGIN_TABLE("LevelInfo", return false)
						{
							for (int j = 1;; j++)
							{
								int iChildDefNameStrID = iDefNameStrID;
								int iChildDefDescStrID = iDefDescStrID;
								int iChildDefLowLevel = iDefLowLevel;
								int iChildDefHighLevel = iDefHighLevel;
								std::string strChilddefTGFileName = strdefTGFileName;

								int nLevel = 0;

								LUA_BEGIN_TABLE(j, break)
								{
									LUA_GET_VALUE_DEF("Level", nLevel, 0);
									LUA_GET_VALUE_DEF("Name", iChildDefNameStrID, iDefNameStrID);
									LUA_GET_VALUE_DEF("Desc", iChildDefDescStrID, iDefDescStrID);
									LUA_GET_VALUE_DEF("LOW_LEVEL", iChildDefLowLevel, iDefLowLevel);
									LUA_GET_VALUE_DEF("HIGH_LEVEL", iChildDefHighLevel, iDefHighLevel);
									LUA_GET_VALUE_DEF("TGFileName", strChilddefTGFileName, strdefTGFileName);
									if (iChildDefLowLevel > iChildDefHighLevel) swap(iChildDefLowLevel, iChildDefHighLevel);
								}
								LUA_END_TABLE(break)

									CString strScriptFileName = SiCDlgDirectorySetting()->m_strMapPath + L"/";
								strScriptFileName += GCUTIL_STR::GCStrCharToWide(strChilddefTGFileName.c_str());
								SaveScriptForServer(hWrite, strScriptFileName, nLevel, iTab + 2);
							}
						}
						LUA_END_TABLE(return false)
					}
					WriteCloseBrace(hWrite, iTab + 1, 0);
				}
				LUA_END_TABLE(return false)
			}
		}
		WriteCloseBrace(hWrite, iTab, 0);
	}
	LUA_END_TABLE(return false)

		return true;
}

bool CQuestScripterDlg::LoadQuestSignForServerScript_Subject(KLuaManager &luaMgr, HANDLE hWrite)
{
	CStringA strEnumFile = GCUTIL_STR::GCStrWideToChar(SiCDlgDirectorySetting()->m_strQuestScriptPath);
	strEnumFile += "/Enum.stg";

	if (FAILED(luaMgr.DoFile(strEnumFile.GetBuffer())))
	{
		int iIndex = strEnumFile.ReverseFind('\\');
		strEnumFile = strEnumFile.Left(iIndex);
		strEnumFile += "/Enum.stg";
		if (FAILED(luaMgr.DoFile(strEnumFile.GetBuffer())))
		{
			CStringA strErr;
			strErr.Format("%s 파일로드에 실패하였습니다.", strEnumFile);
			MessageBoxA(NULL, strErr.GetBuffer(), "알림", MB_OK);
			return false;
		}
	}

	CStringA strETCFile = GCUTIL_STR::GCStrWideToChar(SiCDlgDirectorySetting()->m_strQuestScriptPath);
	strETCFile += "/etc.stg";

	if (FAILED(luaMgr.DoFile(strETCFile.GetBuffer())))
	{
		int iIndex = strETCFile.ReverseFind('\\');
		strETCFile = strETCFile.Left(iIndex);
		strETCFile += "/etc.stg";
		if (FAILED(luaMgr.DoFile(strETCFile.GetBuffer())))
		{
			CStringA strErr;
			strErr.Format("%s 파일로드에 실패하였습니다.", strETCFile.GetBuffer());
			MessageBoxA(NULL, strErr.GetBuffer(), "알림", MB_OK);
			return false;
		}
	}

	CStringA strQuestSignFile = GCUTIL_STR::GCStrWideToChar(SiCDlgDirectorySetting()->m_strQuestScriptPath);
	strQuestSignFile += "/QuestSign.stg";

	if (FAILED(luaMgr.DoFile(strQuestSignFile.GetBuffer())))
	{
		int iIndex = strQuestSignFile.ReverseFind('\\');
		strQuestSignFile = strQuestSignFile.Left(iIndex);
		strQuestSignFile += "/QuestSign.stg";
		if (FAILED(luaMgr.DoFile(strQuestSignFile.GetBuffer())))
		{
			CStringA strErr;
			strErr.Format("%s 파일로드에 실패하였습니다.", strQuestSignFile.GetBuffer());
			MessageBoxA(NULL, strErr.GetBuffer(), "알림", MB_OK);
			return false;
		}
	}

	if (SUCCEEDED(luaMgr.BeginTable("quest")))
	{
		int iTab = 0;
		WriteOpenBrace(hWrite, "Subject", iTab);
		{
			for (int i = 1;; i++)
			{
				char szQuestSign[128];
				sprintf(szQuestSign, "QuestSign%d", i - 1);
				if (SUCCEEDED(luaMgr.BeginTable(szQuestSign)))
				{
					int iQuestID = 0;
					int iDefNameStrID = 0;
					int iDefDescStrID = 0;
					int iDefLowLevel = 0;
					int iDefHighLevel = 0;
					std::string strdefTGFileName = "";

					LUA_GET_VALUE_DEF("ID", iQuestID, -1);
					LUA_GET_VALUE_DEF("Name", iDefNameStrID, 1191);
					LUA_GET_VALUE_DEF("Desc", iDefDescStrID, 2267);
					LUA_GET_VALUE_DEF("LOW_LEVEL", iDefLowLevel, 0);
					LUA_GET_VALUE_DEF("HIGH_LEVEL", iDefHighLevel, 75);
					LUA_GET_VALUE_DEF("TGFileName", strdefTGFileName, "");
					if (iDefLowLevel > iDefHighLevel) swap(iDefLowLevel, iDefHighLevel);

					// 중요정보 로드실패 처리
					if (-1 == iQuestID)
					{
						std::string strError = szQuestSign;
						strError += "에서 ID로드 오류입니다.";
						MessageBoxA(NULL, strError.c_str(), "QuestSing.stg로드실패!!", MB_OK);
						break;
					}
					if ("" == strdefTGFileName)
					{
						std::string strError = szQuestSign;
						strError += "에서 TGFileName로드 오류입니다.";
						MessageBoxA(NULL, strError.c_str(), "QuestSing.stg로드실패!!", MB_OK);
						break;
					}

					char szFileName[512];
					_splitpath(strdefTGFileName.c_str(), 0, 0, szFileName, 0);
					{
						if (SUCCEEDED(luaMgr.BeginTable("LevelInfo")))
						{
							for (int j = 1;; j++)
							{
								int iChildDefNameStrID = iDefNameStrID;
								int iChildDefDescStrID = iDefDescStrID;
								int iChildDefLowLevel = iDefLowLevel;
								int iChildDefHighLevel = iDefHighLevel;
								std::string strChilddefTGFileName = strdefTGFileName;

								int nLevel = 0;

								LUA_BEGIN_TABLE(j, break)
								{
									LUA_GET_VALUE_DEF("Level", nLevel, 0);
									LUA_GET_VALUE_DEF("Name", iChildDefNameStrID, iDefNameStrID);
									LUA_GET_VALUE_DEF("Desc", iChildDefDescStrID, iDefDescStrID);
									LUA_GET_VALUE_DEF("LOW_LEVEL", iChildDefLowLevel, iDefLowLevel);
									LUA_GET_VALUE_DEF("HIGH_LEVEL", iChildDefHighLevel, iDefHighLevel);
									LUA_GET_VALUE_DEF("TGFileName", strChilddefTGFileName, strdefTGFileName);
									if (iChildDefLowLevel > iChildDefHighLevel) swap(iChildDefLowLevel, iChildDefHighLevel);
								}
								LUA_END_TABLE(break)

									CStringA strQuestFileName;
								strQuestFileName.Format("--%s", szFileName);
								CString strScriptFileName = SiCDlgDirectorySetting()->m_strMapPath + L"/";
								strScriptFileName += GCUTIL_STR::GCStrCharToWide(strChilddefTGFileName.c_str());
								SaveScriptForServer_Subject(hWrite, strScriptFileName, strQuestFileName, iQuestID, nLevel, iTab + 2);
							}
							luaMgr.EndTable();
						}
						else
						{
							return false;
						}
					}
					luaMgr.EndTable();
				}
				else
				{
					sprintf(szQuestSign, "QuestSign%d", i - 2);
					std::string strError = szQuestSign;
					strError += " 테이블까지 로드했습니다.";
					MessageBoxA(NULL, strError.c_str(), "알림", MB_OK);
					break;
				}
			}
		}
		WriteCloseBrace(hWrite, iTab, 0);
		luaMgr.EndTable();
	}
	else
		return false;

	return true;
}

void CQuestScripterDlg::OnSvaeToSameFileFull()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	HTREEITEM hRootItem = m_ScriptTree.GetRootItem();
	if (hRootItem)
	{
		m_ScriptTree.Select(hRootItem, TVGN_CARET);
		SaveToFile(m_strLastLoadFile.GetBuffer());
	}
	else
	{
		StaticInfoWithMessageBox(L"Root Tree 정보를 찾을 수 없습니다. 트리를 정보를 갱신해 주십시오", L"저장오류", MB_OK);
	}
}

void CQuestScripterDlg::OnFileAutosave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bAutoSave = !m_bAutoSave;

	if (m_bAutoSave)
	{
		SetTimer(0, QUEST_SCRIPTER_AUTOSAVE_TIME, NULL);
		StaticInfoWithMessageBox(L"자동저장 기능을 사용합니다.", L"알림", MB_OK);
	}
	else
	{
		KillTimer(0);
		StaticInfoWithMessageBox(L"자동저장 기능을 종료합니다.", L"알림", MB_OK);
	}
}

LRESULT CQuestScripterDlg::OnPosAsel(WPARAM wParam, LPARAM lParam)
{
	POINT ptPoint;
	ptPoint.x = (int)wParam;
	ptPoint.y = (int)lParam;
	m_dlgTrigger.m_dlgTriggerContent.m_dlgTGChildGenner.SetCurWindowToPos(ptPoint);

	m_dlgMapToObject.ShowWindow(SW_HIDE);

	return S_OK;
}