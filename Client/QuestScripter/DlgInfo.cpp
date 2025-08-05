// DlgInfo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgInfo.h"
#include ".\dlginfo.h"
#include "QuestScripterDlg.h"


// CDlgInfo 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgInfo, CDialog)
CDlgInfo::CDlgInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInfo::IDD, pParent)
{
    m_CurType = QUESTX;
    m_bCreated = 0;
    m_hCurTreeItem = 0;
}

CDlgInfo::~CDlgInfo()
{
}

void CDlgInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInfo, CDialog)
    ON_WM_CREATE()
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgInfo 메시지 처리기입니다.

int CDlgInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialog::OnCreate(lpCreateStruct) == -1)
        return -1;

    m_bCreated = 1;
    m_dlgQuest.Create(IDD_DIALOG_QUEST, this);
    m_dlgStage.Create(IDD_DIALOG_STAGE, this);
    // TODO:  여기에 특수화된 작성 코드를 추가합니다.

    m_dlgQuest.SetDisableControl(1);
    m_dlgStage.SetDisableControl(1);


    return 0;
}

void CDlgInfo::AllDlgHide(void)
{
    m_dlgQuest.ShowWindow(SW_HIDE);
    m_dlgStage.ShowWindow(SW_HIDE);
}

void CDlgInfo::SetDlgType(NamesOfLevel Type, struct_base *pBase)
{
    m_CurType = Type;

    AllDlgHide();

    switch(m_CurType)
    {
    case QUESTX:
        m_dlgQuest.SetValue((Quest*)pBase);
        m_dlgQuest.ShowWindow(SW_SHOW);
        break;
    case STAGEX:
        m_dlgStage.SetValue((Stage*)pBase);
        m_dlgStage.ShowWindow(SW_SHOW);
        break;
    }

    m_hCurTreeItem = pBase->MatchKey;
}

void CDlgInfo::SetQuest(Quest quest)
{
    m_Quest;
}

void CDlgInfo::SetStage(Stage stage)
{
    m_Stage;
}

void CDlgInfo::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
    ShowWindow(SW_HIDE);
}

void CDlgInfo::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
    ShowWindow(SW_HIDE);
}

void CDlgInfo::OnBnClickedButton1()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CQuestScripterDlg *pQuestScriptDlg = (CQuestScripterDlg*)AfxGetMainWnd();
    if( pQuestScriptDlg )
    {
        if( !m_hCurTreeItem || !pQuestScriptDlg->m_Quest.FindStructFromItem(m_hCurTreeItem) )
        {
            pQuestScriptDlg->StaticInfoWithMessageBox(L"해당 정보의 위치를 찾을 수 없습니다.", L"알림", MB_OK);
            return;
        }
        pQuestScriptDlg->m_ScriptTree.Select(m_hCurTreeItem, TVGN_CARET);
        pQuestScriptDlg->SetFocus();
    }
}
