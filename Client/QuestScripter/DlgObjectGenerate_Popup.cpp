// DlgObjectGenerate_Popup.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "DlgObjectGenerate_Popup.h"
#include "ObjectDataManager.h"
#include "QuestScripterDlg.h"
//#include "DlgMapToObject.h"
//#include "DlgTriggerContent.h"

// CDlgObjectGenerate_Popup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgObjectGenerate_Popup, CDialog)
CDlgObjectGenerate_Popup::CDlgObjectGenerate_Popup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgObjectGenerate_Popup::IDD, pParent)
{
}

CDlgObjectGenerate_Popup::~CDlgObjectGenerate_Popup()
{
}

void CDlgObjectGenerate_Popup::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_ID, m_cbType);
    DDX_Control(pDX, IDC_EDIT_ENTRANCEX, m_ebEntranceX);
    DDX_Control(pDX, IDC_EDIT_ENTRANCEY, m_ebEntranceY);
    DDX_Control(pDX, IDC_COMBO_DIRECTION2, m_cbRight);
    DDX_Control(pDX, IDC_COMBO_GENERATE, m_cbGenerate);
    DDX_Control(pDX, IDC_COMBO_EACTION, m_cbFunctionType);
    DDX_Control(pDX, IDC_COMBO_STATE, m_cbCrashable);

    m_cbRight.InsertString(0, L"왼쪽");
    m_cbRight.InsertString(1, L"오른쪽");
    if( m_cbRight.GetCount() > 0 )
        m_cbRight.SetCurSel(0);

    for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_OBJECT).size();i++)
    {
        m_cbType.InsertString(i, SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_OBJECT)[i]);
    }
    if( m_cbType.GetCount() > 0 )
        m_cbType.SetCurSel(0);

    m_cbGenerate.ResetContent();
    m_cbGenerate.InsertString(0, L"제거합니다.");
    m_cbGenerate.InsertString(1, L"생성합니다.");

    m_cbFunctionType.ResetContent();
    m_cbFunctionType.InsertString(0, L"Event");
    m_cbFunctionType.InsertString(1, L"Action");

    m_cbCrashable.ResetContent();
    m_cbCrashable.InsertString(0, L"기본 형태로");
    m_cbCrashable.InsertString(1, L"파괴가능 형태로");
    if( m_cbCrashable.GetCount() > 0 ) m_cbCrashable.SetCurSel(0);

    CString str;
    str.Format(L"%f", objectgen.m_fEntranceX);
    m_ebEntranceX.SetWindowText(str);
    str.Format(L"%f", objectgen.m_fEntranceY);
    m_ebEntranceY.SetWindowText(str);
    if( m_cbType.GetCount() > objectgen.m_iType ) m_cbType.SetCurSel(objectgen.m_iType);
    if( m_cbRight.GetCount() > (int)objectgen.m_bRight ) m_cbRight.SetCurSel((int)objectgen.m_bRight);
    if( m_cbGenerate.GetCount() > (int)objectgen.m_bGenerate ) m_cbGenerate.SetCurSel((int)objectgen.m_bGenerate);
    if( m_cbCrashable.GetCount() > (int)objectgen.m_bCrashAble ) m_cbCrashable.SetCurSel((int)objectgen.m_bCrashAble);
    if( m_cbFunctionType.GetCount() > 1 ) m_cbFunctionType.SetCurSel(1);
}


BEGIN_MESSAGE_MAP(CDlgObjectGenerate_Popup, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_DELETE, OnBnClickedDelete)
END_MESSAGE_MAP()


// CDlgObjectGenerate_Popup 메시지 처리기입니다.
void CDlgObjectGenerate_Popup::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    objectgen.m_bGenerate       = m_cbGenerate.GetCurSel();
    objectgen.m_iType           = m_cbType.GetCurSel();
    objectgen.m_bCrashAble      = m_cbCrashable.GetCurSel();

    CString str;
    m_ebEntranceX.GetWindowText(str);
    objectgen.m_fEntranceX = (float)_wtof(str);

    m_ebEntranceY.GetWindowText(str);
    objectgen.m_fEntranceY = (float)_wtof(str);

    objectgen.m_bRight = m_cbRight.GetCurSel();
    m_bFnAction = m_cbFunctionType.GetCurSel();

    OnOK();
}

void CDlgObjectGenerate_Popup::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnCancel();
}

void CDlgObjectGenerate_Popup::SetMapObjectList(std::vector<CObjectGenerater*> *pvtObjList)
{
    m_pvtObjList = pvtObjList;
}

void CDlgObjectGenerate_Popup::OnBnClickedDelete()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( !m_pvtObjList ) return;

    for(int i=0;i<(int)m_pvtObjList->size();i++)
    {
        if( objectgen.m_hItem == (*m_pvtObjList)[i]->m_hItem )
        {
            m_pvtObjList->erase((m_pvtObjList->begin()+i));
            CQuestScripterDlg *pDlgQuestScript = ((CQuestScripterDlg*)AfxGetMainWnd());
            if( pDlgQuestScript )
            {
                if( !pDlgQuestScript->m_dlgMapToObject.m_pDlgTGContent->DelActionItem(objectgen.m_hItem) )
                    pDlgQuestScript->m_dlgMapToObject.m_pDlgTGContent->DelEventItem(objectgen.m_hItem);
            }
            break;
        }
    }

    OnCancel();
}
