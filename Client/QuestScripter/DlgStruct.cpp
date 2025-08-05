    // DlgStruct.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgStruct.h"
#include "QuestScripterDlg.h"
#include ".\dlgstruct.h"


// CDlgStruct 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgStruct, CDialog)
CDlgStruct::CDlgStruct(UsingType type, NamesOfLevel NOL, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStruct::IDD, pParent)
{
    m_strResult = "";
    m_type = type;
    m_ParentNOL = NOL;
    m_NamesOfLevel = NOTHING;
}

CDlgStruct::~CDlgStruct()
{
}

void CDlgStruct::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_STRUCT, m_cbStruct);
}


BEGIN_MESSAGE_MAP(CDlgStruct, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_WM_CREATE()
END_MESSAGE_MAP()


// CDlgStruct 메시지 처리기입니다.

void CDlgStruct::Clear()
{
    m_cbStruct.Clear();
    m_vtStruct.clear();
}

void CDlgStruct::AddStruct(CString str, NamesOfLevel NOL)
{
    m_cbStruct.InsertString(m_cbStruct.GetCount(), str);
    m_vtStruct.push_back(NOL);
}

void CDlgStruct::DelStruct(CString str)
{
    int index = m_cbStruct.FindStringExact(0, str);
    if( CB_ERR != index )
    {
        m_cbStruct.DeleteString(index);
        m_vtStruct.erase(m_vtStruct.begin()+index);
    }
}
void CDlgStruct::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( (m_cbStruct.GetCurSel() >= 0 && m_cbStruct.GetCount() >= 0)
        && (m_cbStruct.GetCount() > m_cbStruct.GetCurSel())
        )
    {
        m_cbStruct.GetLBText(m_cbStruct.GetCurSel(), m_strResult);
        m_NamesOfLevel = m_vtStruct[m_cbStruct.GetCurSel()];
    }
    OnOK();
}

int CDlgStruct::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialog::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  여기에 특수화된 작성 코드를 추가합니다.

    return 0;
}

BOOL CDlgStruct::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  여기에 추가 초기화 작업을 추가합니다.
    switch(m_type)
    {
    case ADD_CHILD_TYPE:
        AddChildType();
        break;
    case ADD_SIBLING_TYPE:
        AddSiblingType();
        break;
    }

    m_cbStruct.SetFocus();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgStruct::AddChildType(void)
{
    bool bCreate = 1;
    Clear();

    CQuestScripterDlg *pQestScriptDlg = (CQuestScripterDlg*)AfxGetMainWnd();
    if( pQestScriptDlg )
    {
        HTREEITEM hSelItem = pQestScriptDlg->m_ScriptTree.GetSelectedItem();
        if( hSelItem )
            bCreate = SetComboBoxByTreeLevel_Child(pQestScriptDlg->m_ScriptTree.GetLevelTree(hSelItem));
    }

    if( !bCreate )
        OnCancel();
};

void CDlgStruct::AddSiblingType(void)
{
    bool bCreate = 1;
    Clear();

    CQuestScripterDlg *pQestScriptDlg = (CQuestScripterDlg*)AfxGetMainWnd();
    if( pQestScriptDlg )
    {
        HTREEITEM hSelItem = pQestScriptDlg->m_ScriptTree.GetSelectedItem();
        struct_base *pBase = pQestScriptDlg->m_Quest.FindStructFromItem(hSelItem);
        if( pBase )
        {
            switch(pBase->NOL)
            {
                case QUESTX:
                    AddStruct(STR_QUEST, pBase->NOL);
                    break;
                case STAGEX:
                    AddStruct(STR_STAGE, pBase->NOL);
                    break;
            }
        }
    }

    if( m_cbStruct.GetCount() > 0 )
        m_cbStruct.SetCurSel(0);

    if( !bCreate )
        OnCancel();
}

bool CDlgStruct::SetComboBoxByTreeLevel_Child(int iLevel)
{
    CQuestScripterDlg *pQestScriptDlg = (CQuestScripterDlg*)AfxGetMainWnd();
    if( iLevel >= 0 && iLevel < (int)pQestScriptDlg->m_NamesOfLevel.size() )
    {
        for(int i=0;i<(int)pQestScriptDlg->m_NamesOfLevel[iLevel].size();i++)
        {
            switch(m_ParentNOL)
            {
                case QUESTX:
                case STAGEX:
                    AddStruct(pQestScriptDlg->m_NamesOfLevel[iLevel][i], pQestScriptDlg->m_NamesOfLevel_Def[iLevel][i]);
                    break;
            }
        }
    }
    else
    {
        pQestScriptDlg->StaticInfoWithMessageBox(L"더 이상의 하위구조는 생성할 수 없습니다.", L"", MB_OK);
        return false;
    }

    if( m_cbStruct.GetCount() > 0 )
        m_cbStruct.SetCurSel(0);
 
    return true;
}