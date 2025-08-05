// DlgObjCommon.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgObjCommon.h"
#include ".\dlgobjcommon.h"
#include "QuestScripterDlg.h"


// CDlgObjCommon 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgObjCommon, CDialog)
CDlgObjCommon::CDlgObjCommon(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgObjCommon::IDD, pParent)
{
    m_bCreated = 0;
    m_CurType = STAGE_MON;
}

CDlgObjCommon::~CDlgObjCommon()
{
}

void CDlgObjCommon::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_ID, m_cbID);
    DDX_Control(pDX, IDC_EDIT_TYPE, m_edType);
    DDX_Control(pDX, IDC_EDIT_NAME, m_edName);
    DDX_Control(pDX, IDC_EDIT_LEVEL, m_edLevel);
    DDX_Control(pDX, IDC_EDIT_ENTRANCEX, m_edEntranceX);
    DDX_Control(pDX, IDC_EDIT_ENTRANCEY, m_edEntranceY);
    DDX_Control(pDX, IDC_COMBO_DIRECTION, m_cbDirection);
    DDX_Control(pDX, IDC_COMBO_ACTIVE, m_cbActive);
    m_cbID.Init();

    m_cbDirection.InsertString(0, "왼쪽");
    m_cbDirection.InsertString(1, "오른쪽");

    m_cbActive.InsertString(0, "비활성");
    m_cbActive.InsertString(0, "활성");
}


BEGIN_MESSAGE_MAP(CDlgObjCommon, CDialog)
    ON_WM_CREATE()
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgObjCommon 메시지 처리기입니다.

int CDlgObjCommon::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialog::OnCreate(lpCreateStruct) == -1)
        return -1;

    m_bCreated = 1;
    // TODO:  여기에 특수화된 작성 코드를 추가합니다.

    return 0;
}

void CDlgObjCommon::SetValue(Stage_mon stage_mon)
{
    //stage_mon.EntranceX;
    //stage_mon.EntranceY;
    //stage_mon.Level;
    //stage_mon.strMonsterID;

    m_edType.SetWindowText("");
    if( m_cbID.GetCount() > stage_mon.iMonsterID )
        m_cbID.SetCurSel(stage_mon.iMonsterID);
    m_edName.SetWindowText("");
    SetWindowTextToEdit(&m_edLevel, stage_mon.Level);
    SetWindowTextToEdit(&m_edEntranceX, stage_mon.EntranceX);
    SetWindowTextToEdit(&m_edEntranceY, stage_mon.EntranceY);
    m_cbDirection.SetCurSel(0);
    m_cbActive.SetCurSel(0);
}

Stage_mon CDlgObjCommon::GetValue(void)
{
    Stage_mon stage_mon;
    stage_mon.iMonsterID = m_cbID.GetCurSel();
    GetWindowTextToEdit(&m_edLevel, &stage_mon.Level);
    GetWindowTextToEdit(&m_edEntranceX, &stage_mon.EntranceX);
    GetWindowTextToEdit(&m_edEntranceY, &stage_mon.EntranceY);

    return stage_mon;
}

void CDlgObjCommon::SetDisableControl(bool bDisable)
{
    m_edType.SetReadOnly(bDisable);
    m_edName.SetReadOnly(bDisable);
    m_edLevel.SetReadOnly(bDisable);
    m_edEntranceX.SetReadOnly(bDisable);
    m_edEntranceY.SetReadOnly(bDisable);
    m_cbID.EnableWindow(!bDisable);
    m_cbDirection.EnableWindow(!bDisable);
    m_cbActive.EnableWindow(!bDisable);
}

void CDlgObjCommon::SetComboBoxValue(CQuestScripterDlg *pQuestScripter)
{
    if( !pQuestScripter ) return;
    m_cbID.ResetContent();
    for(int i=0;i<pQuestScripter->m_vtEnumMonster.size();i++)
    {
        m_cbID.AddSearchString(pQuestScripter->m_vtEnumMonster[i]);
        m_cbID.InsertString(i, pQuestScripter->m_vtEnumMonster[i]);
    }

    if( m_cbID.GetCount() > 0 ) m_cbID.SetCurSel(0);
}

void CDlgObjCommon::SetType(CommonType Type)
{
    m_CurType = Type;
}

void CDlgObjCommon::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgObjCommon::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}
BOOL CDlgObjCommon::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  여기에 추가 초기화 작업을 추가합니다.


    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
