// DlgStageMode.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgStageMode.h"
#include ".\dlgstagemode.h"


// CDlgStageMode 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgStageMode, CDialog)
CDlgStageMode::CDlgStageMode(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStageMode::IDD, pParent)
{
    m_hAccelTable = NULL;
    GetCurrentDirectoryA( MAX_PATH, g_Path );       // 프로그램의 실행경로를 받아온다.

    m_LastUse_ClipBoard = NOTHING;
    m_bCreated = 0;
    m_hSelectedBefore = 0;
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    // Quest Level 별 분리 입니다.
    m_NamesOfLevel[0].push_back(STR_QUEST);
    m_NamesOfLevel[1].push_back(STR_STAGE);
    m_NamesOfLevel[1].push_back(STR_SEQUENCE);
    m_NamesOfLevel[2].push_back(STR_STAGELV);
    m_NamesOfLevel[3].push_back(STR_STAGEMON);

    m_NamesOfLevel_Def[0].push_back(QUESTX);
    m_NamesOfLevel_Def[1].push_back(STAGEX);
    m_NamesOfLevel_Def[1].push_back(SEQUENCE);
    m_NamesOfLevel_Def[2].push_back(STAGE_LV);
    m_NamesOfLevel_Def[3].push_back(STAGE_MON);
}

CDlgStageMode::~CDlgStageMode()
{
}

void CDlgStageMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_TREE_SCRIPT, m_ScriptTree);
    DDX_Control(pDX, IDC_STATIC_MAIN, m_stMain);

    m_dlgQuest.Create(IDD_DIALOG_QUEST, &m_stMain);
    m_dlgSequence.Create(IDD_DIALOG_SEQUENCE, &m_stMain);
    m_dlgStage.Create(IDD_DIALOG_STAGE, &m_stMain);
    m_dlgStageLv.Create(IDD_DIALOG_STAGE_LV, &m_stMain);
    m_dlgStageMonster.Create(IDD_DIALOG_STAGE_MONSTER, &m_stMain);
    m_dlgInfo.Create(IDD_DIALOG_POPUP_BOARD, this);

    LoadPreLoad();
    m_dlgQuest.SetComboBoxValue(this);
    m_dlgStage.SetComboBoxValue(this);
    m_dlgStageMonster.SetComboBoxValue(this);
    m_dlgInfo.m_dlgQuest.SetComboBoxValue(this);
    m_dlgInfo.m_dlgStage.SetComboBoxValue(this);
    m_dlgInfo.m_dlgStageMonster.SetComboBoxValue(this);

    m_ScriptTree.InitializeImageList();
    m_Quest.MatchKey = m_ScriptTree.AddChild("QuestX");
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
    if( m_Quest.MatchKey )
    {
        m_ScriptTree.Select(m_Quest.MatchKey, TVGN_CARET);
        m_hSelectedBefore = m_Quest.MatchKey;
        //ChangeDialog(m_Quest.MatchKey);
    }
}


BEGIN_MESSAGE_MAP(CDlgStageMode, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_MESSAGE(WM_MONSTER_ADD_WINDOW, OnMonsterAddWindow)
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
    ON_MESSAGE(WM_NEW_TREE, OnNewTree)
    ON_MESSAGE(WM_CUT_TREE, OnCutTree)
    ON_MESSAGE(WM_COPY_TREE, OnCopyTree)
    ON_MESSAGE(WM_PASTE_TREE, OnPasteTree)
    ON_MESSAGE(WM_LOAD_ICON_MON, LoadIconMonLayerPart)
    ON_MESSAGE(WM_CHAGNE_TREE_FOCUS, OnChangeTreeFocus)
END_MESSAGE_MAP()


// CDlgStageMode 메시지 처리기입니다.

void CDlgStageMode::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnOK();
}

void CDlgStageMode::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnCancel();
}

BOOL CDlgStageMode::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  여기에 추가 초기화 작업을 추가합니다.

    // TODO: 여기에 추가 초기화 작업을 추가합니다.
    m_bCreated = 1;

    //************************************************************************
    // Tree창과 정보 표시창의 위치를 잡아준다.
    CRect rtClient;
    GetClientRect(rtClient);
    const int iStartX = rtClient.left+10;
    const int iStartY = rtClient.top+45;
    const int iTreeWidth = 300;
    m_ScriptTree.MoveWindow(CRect(iStartX, iStartY, iStartX+iTreeWidth, rtClient.bottom-40));
    m_TriggerTree.MoveWindow(CRect(iStartX, iStartY, iStartX+iTreeWidth, rtClient.bottom-40));
    m_stMain.MoveWindow(CRect(iStartX+iTreeWidth+5, iStartY, rtClient.right - 10, rtClient.bottom-40));
    m_stMainTrigger.MoveWindow(CRect(iStartX+iTreeWidth+5, iStartY, rtClient.right - 10, rtClient.bottom-40));
    m_stInfo.MoveWindow(CRect(iStartX, rtClient.bottom - 30, rtClient.right-10, rtClient.bottom-10), 1);

    CRect rtTriggerText;
    m_stTrigger.GetWindowRect(rtTriggerText);
    ScreenToClient(rtTriggerText);
    rtTriggerText.right = 10 + rtTriggerText.Width();
    rtTriggerText.left = 10;
    m_stTrigger.MoveWindow(rtTriggerText, 1);

    m_stTriggerContent.GetWindowRect(rtTriggerText);
    ScreenToClient(rtTriggerText);
    rtTriggerText.right = (iStartX+iTreeWidth+5) + rtTriggerText.Width();
    rtTriggerText.left = iStartX+iTreeWidth+5;
    m_stTriggerContent.MoveWindow(rtTriggerText, 1);
    //************************************************************************

    //************************************************************************
    // 단축키를 사용하기 위해 단축키 정보를 불러온다.
    m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR_MAIN_DLG)); 
    //************************************************************************

    //************************************************************************
    // DirectX 윈도우 창을 생성한다.
    m_dlgMapToObject.Create(this, "");
    m_dlgMapToObject.ShowWindow(SW_HIDE);
    //************************************************************************

    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// 몬스터 추가하는 윈도우를 활성화 시킨다. (D3D용 윈도우)
LRESULT CDlgStageMode::OnMonsterAddWindow(WPARAM wParam, LPARAM lParam)
{
    HTREEITEM hSelectedItem = m_ScriptTree.GetSelectedItem();
    if( !hSelectedItem ) { StaticInfoWithMessageBox("트리를 다시 선택해 주십시오", "알림", MB_OK); return E_FAIL; }

    HTREEITEM hParentsItem = m_ScriptTree.GetParentItem(hSelectedItem);
    if( !hParentsItem ) { StaticInfoWithMessageBox("선택된 트리의 상위트리를 찾을 수 없습니다..", "알림", MB_OK); return E_FAIL; }

    struct_base *pSelBase = m_Quest.FindStructFromItem(hSelectedItem);
    if( !pSelBase ) { StaticInfoWithMessageBox("선택된 트리구조를 찾을 수 없습니다.", "알림", MB_OK); return E_FAIL; }

    StageLv *pStageLv = (StageLv*)pSelBase;

    struct_base *pSelParentBase = m_Quest.FindStructFromItem(hParentsItem);
    if( !pSelParentBase )  { StaticInfoWithMessageBox("선택된 트리의 상위트리 구조를 찾을 수 없습니다.", "알림", MB_OK); return E_FAIL; }

    if( STAGEX != pSelParentBase->NOL )  { StaticInfoWithMessageBox("선택된 트리의 상위트리는 반드시 Stage 구조여야 합니다.", "알림", MB_OK); return E_FAIL; }

    Stage *pStage = (Stage*)pSelParentBase;
    CString strFilePath = g_Path;
    strFilePath += "/Data/Map/" + pStage->strStageMap;

    HANDLE hRead = CreateFile(strFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if( INVALID_HANDLE_VALUE == hRead  )
    {
        strFilePath += " 파일을 찾을 수 없습니다.(선택한 맵파일이 Data/Map/ 폴더 안에 존재해야합니다.)";
        StaticInfoWithMessageBox(strFilePath, "파일경로오류", MB_OK);
        return E_FAIL;
    }
    CloseHandle(hRead);

    m_dlgMapToObject.SetMonsterList(pStageLv->vtStageMon);
    m_dlgMapToObject.LoadMapToObject(strFilePath);
    m_dlgMapToObject.SetQuestScripterDlg(this);
    m_dlgMapToObject.ShowWindow(SW_SHOW);

    return S_OK;
}

LRESULT CDlgStageMode::OnChangeSequence(WPARAM wParam, LPARAM lParam)
{
    CDlgChangeSequence dlgChangeSec;

    HTREEITEM hSelItem = m_ScriptTree.GetSelectedItem();
    if( !hSelItem ) { StaticInfoWithMessageBox("트리를 다시 선택해 주십시오", "알림", MB_OK); return E_FAIL; }

    struct_base *pSelBase = m_Quest.FindStructFromItem(hSelItem);
    if( !pSelBase ) { StaticInfoWithMessageBox("선택된 트리구조를 찾을 수 없습니다.", "알림", MB_OK); return E_FAIL; }

    switch( pSelBase->NOL )
    {
    case STAGEX:
    case STAGE_LV:
    case SEQUENCE:
        break;
    default:
        StaticInfoWithMessageBox("Stage, StageLv, Sequence 만 순서를 변경할 수 있습니다.", "알림", MB_OK);
        return E_FAIL;
        break;
    }

    HTREEITEM hSelParentItem = m_ScriptTree.GetParentItem(hSelItem);
    if( !hSelParentItem ) { StaticInfoWithMessageBox("선택된 트리의 상위트리를 찾을 수 없습니다..", "알림", MB_OK); return E_FAIL; }

    struct_base *pSelParentBase = m_Quest.FindStructFromItem(hSelParentItem);
    if( !pSelParentBase )  { StaticInfoWithMessageBox("선택된 트리의 상위트리 구조를 찾을 수 없습니다.", "알림", MB_OK); return E_FAIL; }

    // 변경가능한 Index를 추출한다.
    vector<int> vtSequenceList;
    switch( pSelBase->NOL )
    {
    case STAGEX:
        {
            Quest *pQuest = (Quest*)pSelParentBase;
            Stage *pStage = (Stage*)pSelBase;

            for(int i=0;i<(int)pQuest->vtStage.size();i++)
            {
                if( pQuest->vtStage[i].MatchKey != pStage->MatchKey )
                    vtSequenceList.push_back(i);
            }
        }
        break;
    case STAGE_LV:
        {
            Stage *pStage = (Stage*)pSelParentBase;
            StageLv *pStageLv = (StageLv*)pSelBase;

            for(int i=0;i<(int)pStage->vtStageLv.size();i++)
            {
                if( pStage->vtStageLv[i].MatchKey != pStageLv->MatchKey )
                    vtSequenceList.push_back(i);
            }
        }
        break;
    case SEQUENCE:
        {
            Quest *pQuest = (Quest*)pSelParentBase;
            Sequence *pSequence = (Sequence*)pSelBase;

            for(int i=0;i<(int)pQuest->vtSequence.size();i++)
            {
                if( pQuest->vtSequence[i].MatchKey != pSequence->MatchKey )
                    vtSequenceList.push_back(i);
            }
        }
        break;
    }

    dlgChangeSec.SetChangeSequenceList(vtSequenceList);

    if( IDOK == dlgChangeSec.DoModal() )
    {
        bool bChanged = false;
        switch( pSelBase->NOL )
        {
        case STAGEX:
            {
                Quest *pQuest = (Quest*)pSelParentBase;
                Stage *pStage = (Stage*)pSelBase;

                Stage hTemp = *pStage;
                *pStage = pQuest->vtStage[dlgChangeSec.m_iSequence];
                pQuest->vtStage[dlgChangeSec.m_iSequence] = hTemp;
                bChanged = true;
            }
            break;
        case STAGE_LV:
            {
                Stage *pStage = (Stage*)pSelParentBase;
                StageLv *pStageLv = (StageLv*)pSelBase;

                StageLv hTemp = *pStageLv;
                *pStageLv = pStage->vtStageLv[dlgChangeSec.m_iSequence];
                pStage->vtStageLv[dlgChangeSec.m_iSequence] = hTemp;
                bChanged = true;
            }
            break;
        case SEQUENCE:
            {
                Quest *pQuest = (Quest*)pSelParentBase;
                Sequence *pSequence = (Sequence*)pSelBase;

                Sequence hTemp = *pSequence;
                *pSequence = pQuest->vtSequence[dlgChangeSec.m_iSequence];
                pQuest->vtSequence[dlgChangeSec.m_iSequence] = hTemp;
                bChanged = true;
            }
            break;
        }

        if( bChanged )
        {
            ReDrawTree();
            m_ScriptTree.Select(pSelBase->MatchKey, TVGN_CARET);
        }
    }

    vtSequenceList.clear();

    m_Datamanager.AddQuest(&m_Quest);

    return S_OK;
}

LRESULT CDlgStageMode::OnEnableMessage(WPARAM wParam, LPARAM lParam)
{
    CMenu* pPopup = (CMenu*)lParam;

    //////////////////////////////////////////////////////////////////////////
    // Change_Sequence 활성화 결정
    HTREEITEM hItem = m_ScriptTree.GetSelectedItem();
    struct_base *pBase = 0;
    pBase = m_Quest.FindStructFromItem(hItem);
    if( !pBase ) return E_FAIL;

    switch(pBase->NOL)
    {
    case STAGEX:
    case STAGE_LV:
    case SEQUENCE:
        break;
    default:
        pPopup->EnableMenuItem( ID_CHANGE_SEQUENCE, MF_BYCOMMAND | MF_GRAYED);
        break;
    }
    //////////////////////////////////////////////////////////////////////////

    return S_OK;
}

LRESULT CDlgStageMode::OnChangeDialog(WPARAM wParam, LPARAM lParam)
{
    ChangeDialog(m_ScriptTree.GetSelectedItem());

    return S_OK;
}

// 부모 구조로 이동하는 것임.
LRESULT CDlgStageMode::OnShiftTreeSelect(WPARAM wParam, LPARAM lParam)
{
    switch(lParam)
    {
    case VK_UP:
        {
            HTREEITEM hSelected = m_ScriptTree.GetSelectedItem();
            HTREEITEM hPrev = m_ScriptTree.GetPrevSiblingItem(hSelected);
            if( hPrev ) m_ScriptTree.Select(hPrev, TVGN_CARET);
        }
        break;
    case VK_DOWN:
        {
            HTREEITEM hSelected = m_ScriptTree.GetSelectedItem();
            HTREEITEM hNext = m_ScriptTree.GetNextSiblingItem(hSelected);
            if( hNext ) m_ScriptTree.Select(hNext, TVGN_CARET);
        }
        break;
    case VK_LEFT:
        {
            HTREEITEM hSelected = m_ScriptTree.GetSelectedItem();
            HTREEITEM hParent = m_ScriptTree.GetParentItem(hSelected);
            if( hParent ) m_ScriptTree.Select(hParent, TVGN_CARET);
        }
        break;
    case VK_RIGHT:
        {
            HTREEITEM hSelected = m_ScriptTree.GetSelectedItem();
            HTREEITEM hChild = m_ScriptTree.GetChildItem(hSelected);
            if( hChild ) m_ScriptTree.Select(hChild, TVGN_CARET);
        }
        break;
    }

    return S_OK;
}

LRESULT CDlgStageMode::OnAddchild_Enter_ForMsg(WPARAM wParam, LPARAM lParam)
{
    OnAddchild_Enter();
    return S_OK;
}

LRESULT CDlgStageMode::OnAddSibling_SpaceBar_ForMsg(WPARAM wParam, LPARAM lParam)
{
    OnAddSibling_SpaceBar();
    return S_OK;
}

LRESULT CDlgStageMode::OnAddSiblingQ_ForMsg(WPARAM wParam, LPARAM lParam)
{
    OnSiblingchildQ();
    return S_OK;
}

LRESULT CDlgStageMode::OnAddchildQ_ForMsg(WPARAM wParam, LPARAM lParam)
{
    OnAddchildQ();
    return S_OK;
}

LRESULT CDlgStageMode::OnDeleteSubTree_ForMsg(WPARAM wParam, LPARAM lParam)
{
    OnDeletesubtreeQ();
    return S_OK;
}

LRESULT CDlgStageMode::OnMoveSibling_forMsg(WPARAM wParam, LPARAM lParam)
{
    MoveSibling(m_ScriptTree.m_hitemDrag, m_ScriptTree.m_hitemDrop);
    return S_OK;
}

LRESULT CDlgStageMode::OnInfoStruct_ForMsg(WPARAM wParam, LPARAM lParam)
{
    OnInfoStruct();
    return S_OK;
}

LRESULT CDlgStageMode::OnSaveToFile_ForMsg(WPARAM wParam, LPARAM lParam)
{
    OnSaveToFile();
    return S_OK;
}

LRESULT CDlgStageMode::OnSaveToFileFull_ForMsg(WPARAM wParam, LPARAM lParam)
{
    OnSvaeToFileFull();
    return S_OK;
}

LRESULT CDlgStageMode::OnLoadFromFile_ForMsg(WPARAM wParam, LPARAM lParam)
{
    OnLoadFromFile();
    return S_OK;
}

LRESULT CDlgStageMode::OnLoadFromFileFull_ForMsg(WPARAM wParam, LPARAM lParam)
{
    OnLoadFromFileFull();
    return S_OK;
}

LRESULT CDlgStageMode::OnNewTree(WPARAM wParam, LPARAM lParam)
{
    OnTreeNew();
    return S_OK;
}

LRESULT CDlgStageMode::OnCutTree(WPARAM wParam, LPARAM lParam)
{
    OnTreeCut();
    return S_OK;
}

LRESULT CDlgStageMode::OnCopyTree(WPARAM wParam, LPARAM lParam)
{
    OnTreeCopy();
    return S_OK;
}

LRESULT CDlgStageMode::OnPasteTree(WPARAM wParam, LPARAM lParam)
{
    OnTreePaste();
    return S_OK;
}

LRESULT CDlgStageMode::LoadIconMonLayerPart(WPARAM wParam, LPARAM lParam)
{
    map<CString, CString>::iterator itor = m_mapMonsterImage.begin();
    for(;itor != m_mapMonsterImage.end();itor++)
    {
        CString strPath;// = g_Path;
        CString strFullPath = g_Path;
        strFullPath += (*itor).second;
        strPath += (*itor).second;
        D3DXIMAGE_INFO ScrInfo;
        D3DXGetImageInfoFromFile( strFullPath, &ScrInfo);
        char szFileName[256], szEXT[256];
        _splitpath(strFullPath.GetBuffer(), 0, 0, szFileName, szEXT);
        CString strFileName = szFileName;
        strFileName += szEXT;

        CLayerPart* pPart = new CLayerPart();
        pPart->SetPartCategory( "MonsterForToolPallet.stg" );

        CString path = (*itor).first;
        pPart->SetPartName( path.GetBuffer() );
        KLayerRect rtLayerRect;
        rtLayerRect.m_iX = 0;
        rtLayerRect.m_iY = 0;
        rtLayerRect.m_iWidth = ScrInfo.Width;
        rtLayerRect.m_iHeight = ScrInfo.Height;
        pPart->SetTexturePart( strFileName.GetBuffer(), rtLayerRect );
        pPart->SetLayerPos( rtLayerRect );
        pPart->SetFHWidth( ScrInfo.Width );
        pPart->SetFHHeight( ScrInfo.Height );
        SiCLayerManager()->AddLayerPart(pPart);
    }

    return S_OK;
}

LRESULT CDlgStageMode::OnChangeTreeFocus(WPARAM wParam, LPARAM lParam)
{
    if( !wParam ) return E_FAIL;
    m_ScriptTree.Select( ((struct_base*)wParam)->MatchKey, TVGN_CARET );
    return S_OK;
}
