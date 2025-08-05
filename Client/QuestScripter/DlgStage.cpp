// DlgStage.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "DlgStage.h"
#include "DlgDirectorySetting.h"
#include "QuestScripterDlg.h"

//#include "QuestScripter.h"
//#include "Category.h"
//#include "DlgTrigger.h"

// CDlgStage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgStage, CDialog)
CDlgStage::CDlgStage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStage::IDD, pParent)
{
    m_bCreated      = 0;
    m_pvtCategory   = 0;
}

CDlgStage::~CDlgStage()
{
    ClearCategorys(&m_vtCategory);
}

void CDlgStage::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //DDX_Control(pDX, IDC_EDIT_STAGEMAP, m_edStageMap);
    //DDX_Control(pDX, IDC_BUTTON_STAGEMAP, m_btStageMap);
    DDX_Control(pDX, IDC_BUTTON_OPEN_TGEDITOR, m_btOpenTGEditor);
    DDX_Control(pDX, IDC_COMBO_STAGEMAP, m_cbStageMap);
    m_cbStageMap.Init();

    RefreshMapDriectory();
}


BEGIN_MESSAGE_MAP(CDlgStage, CDialog)
    ON_WM_CREATE()
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON_STAGEMAP, OnBnClickedButtonStagemap)
    ON_BN_CLICKED(IDC_BUTTON_OPEN_TGEDITOR, OnBnClickedButtonOpenTgeditor)
END_MESSAGE_MAP()


// CDlgStage 메시지 처리기입니다.
void CDlgStage::RefreshMapDriectory(void)
{
    //CString strFilePath = g_Path;
    //strFilePath += "/Data/Map/*.*";

    m_cbStageMap.ResetContent();

    CString strMapPath = SiCDlgDirectorySetting()->m_strMapPath;
    strMapPath += "\\*.*";

    CFileFind cFileFind;
    if( cFileFind.FindFile(strMapPath) )
    {
        while(1)
        {
            if( !cFileFind.FindNextFile() ) break;

            if( !cFileFind.IsDirectory()
                && !cFileFind.IsDots() )
            {
                m_cbStageMap.InsertString(m_cbStageMap.GetCount(), cFileFind.GetFileName());
                m_cbStageMap.AddSearchString(cFileFind.GetFileName());
            }
        }
    }

    if( m_cbStageMap.GetCount() > 0 ) m_cbStageMap.SetCurSel(0);
}

int CDlgStage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialog::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  여기에 특수화된 작성 코드를 추가합니다.
    m_bCreated = 1;

    return 0;
}

void CDlgStage::SetValue(Stage *stage)
{
    int index = m_cbStageMap.FindStringExact(0, stage->strStageMap);
    if( CB_ERR != index )
        m_cbStageMap.SetCurSel(index);
    else
        m_cbStageMap.SetCurSel(0);
    //m_edStageMap.SetWindowText(stage->strStageMap);
    ClearCategorys(&m_vtCategory);
    CopyToCategorys(&m_vtCategory, &stage->vtCategory);
    m_pvtCategory = &stage->vtCategory;
}

void CDlgStage::GetValue(Stage *stage)
{
    //m_edStageMap.GetWindowText();
    m_cbStageMap.GetWindowText(stage->strStageMap);
    ClearCategorys(&stage->vtCategory);
    CopyToCategorys(&stage->vtCategory, &m_vtCategory);
}

void CDlgStage::SetDisableControl(bool bDisable)
{
    //m_edStageMap.SetReadOnly(1);
}

void CDlgStage::SetComboBoxValue(CQuestScripterDlg *pQuestScripter)
{

}

void CDlgStage::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgStage::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}
void CDlgStage::OnBnClickedButtonStagemap()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //char Filter[] = "맵스크립트(*.lua)|*.lua|";
    //CFileDialog dlg(TRUE, "*.lua(*.lua)", "*.lua", OFN_HIDEREADONLY | 
    //    OFN_FILEMUSTEXIST, Filter, NULL);

    //CString strPath = g_Path;
    //strPath += "\\Data\\Map";
    //dlg.m_ofn.lpstrInitialDir = (LPSTR)(strPath.GetBuffer());

    //if( IDOK == dlg.DoModal() )
    //{
    //    char szFileName[512];
    //    char szExt[256];
    //    _splitpath(dlg.GetPathName(), 0, 0, szFileName, szExt);
    //    strcat(szFileName, szExt);
    //    m_edStageMap.SetWindowText(szFileName);
    //}
}

void CDlgStage::OnBnClickedButtonOpenTgeditor()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

    CQuestScripterDlg *pScriptDialog = (CQuestScripterDlg*)AfxGetMainWnd();
    if( !pScriptDialog ) return;
    pScriptDialog->ChangeDialog(pScriptDialog->m_ScriptTree.GetSelectedItem());

    if( pScriptDialog->m_dlgTrigger )
    {
        pScriptDialog->m_dlgTrigger.m_pvtCategory = m_pvtCategory;
        pScriptDialog->m_dlgTrigger.SetCallWindowPointer(this);
        pScriptDialog->m_dlgTrigger.RefreshTriggerTree();
        pScriptDialog->m_dlgTrigger.ShowWindow(SW_SHOW);
        pScriptDialog->m_dlgInfo.ShowWindow(SW_HIDE);
    }
    else
    {
        MessageBox(L"트리거 에디터를 띄우는데 문제가 발견되었습니다. 프로그램을 다시 시작해 주세요", L"알림", MB_OK);
    }
}

void CDlgStage::CopyToCategorys(std::vector<CCategory*> *pvtDest, std::vector<CCategory*> *pvtSource)
{
    for(int i=0;i<(int)pvtSource->size();i++)
    {
        CCategory *pCategory = new CCategory;
        *pCategory = *(*pvtSource)[i];
        pvtDest->push_back(pCategory);
    }
}

void CDlgStage::ClearCategorys(std::vector<CCategory*> *pvtCategory)
{
    for(int i=0;i<(int)pvtCategory->size();i++)
        SAFE_DELETE((*pvtCategory)[i]);

    pvtCategory->clear();
}