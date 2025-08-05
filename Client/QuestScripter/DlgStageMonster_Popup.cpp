// DlgStageMonster_Popup.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "DlgStageMonster_Popup.h"

// CDlgStageMonster_Popup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgStageMonster_Popup, CDialog)
CDlgStageMonster_Popup::CDlgStageMonster_Popup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStageMonster_Popup::IDD, pParent)
{
    m_pvtMonList = 0;
    m_bFnAction = false;
}

CDlgStageMonster_Popup::~CDlgStageMonster_Popup()
{
}

void CDlgStageMonster_Popup::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_BOSS, m_cbBoss);
    DDX_Control(pDX, IDC_COMBO_ID, m_cbID);
    DDX_Control(pDX, IDC_EDIT_ENTRANCEX, m_edEntranceX);
    DDX_Control(pDX, IDC_EDIT_ENTRANCEY, m_edEntranceY);
    DDX_Control(pDX, IDC_COMBO_DIRECTION2, m_cbRight);
    DDX_Control(pDX, IDC_EDIT_LV, m_edMonLv);

    m_cbBoss.InsertString(0, L"아니오");
    m_cbBoss.InsertString(1, L"예");
    if( m_cbBoss.GetCount() > 0 )
        m_cbBoss.SetCurSel(0);

    m_cbRight.InsertString(0, L"왼쪽");
    m_cbRight.InsertString(1, L"오른쪽");
    if( m_cbRight.GetCount() > 0 )
        m_cbRight.SetCurSel(0);

    m_cbID.ResetContent();
    for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_MON).size();i++)
    {
        m_cbID.InsertString(i, SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_MON)[i]);
    }
    if( m_cbID.GetCount() > 0 )
        m_cbID.SetCurSel(0);

    CString str;
    str.Format(L"%f", monstergen.m_fEntranceX);
    m_edEntranceX.SetWindowText(str);
    str.Format(L"%f", monstergen.m_fEntranceY);
    m_edEntranceY.SetWindowText(str);
    str.Format(L"%d", monstergen.m_iMonLevel);
    m_edMonLv.SetWindowText(str);
    if( m_cbID.GetCount() > monstergen.m_iMonType )
        m_cbID.SetCurSel(monstergen.m_iMonType);
    m_cbBoss.SetCurSel(monstergen.m_iBoss);
    //m_cbActive.SetCurSel(stagemon.bActive);
    m_cbRight.SetCurSel(monstergen.m_bRight);
    DDX_Control(pDX, IDC_COMBO_EACTION, m_cbFunctionType);

    m_cbFunctionType.ResetContent();
    m_cbFunctionType.InsertString(0, L"Event");
    m_cbFunctionType.InsertString(1, L"Action");
    if( m_cbFunctionType.GetCount() > 1 ) m_cbFunctionType.SetCurSel(1);
}


BEGIN_MESSAGE_MAP(CDlgStageMonster_Popup, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_DELETE, OnBnClickedDelete)
END_MESSAGE_MAP()


// CDlgStageMonster_Popup 메시지 처리기입니다.

void CDlgStageMonster_Popup::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    monstergen.m_iBoss      = m_cbBoss.GetCurSel();
    monstergen.m_iMonType   = m_cbID.GetCurSel();

    CString str;
    m_edEntranceX.GetWindowText(str);
    monstergen.m_fEntranceX = (float)_wtof(str);

    m_edEntranceY.GetWindowText(str);
    monstergen.m_fEntranceY = (float)_wtof(str);

    m_edMonLv.GetWindowText(str);
    monstergen.m_iMonLevel  = _wtoi(str);


    monstergen.m_bRight = m_cbRight.GetCurSel();
    m_bFnAction = m_cbFunctionType.GetCurSel();

    OnOK();
}

void CDlgStageMonster_Popup::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnCancel();
}

void CDlgStageMonster_Popup::SetMapObjectList(std::vector<CMonsterGenerater*> *pvtMonList)
{
    m_pvtMonList = pvtMonList;
}

void CDlgStageMonster_Popup::OnBnClickedDelete()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( !m_pvtMonList ) return;

    for(int i=0;i<(int)m_pvtMonList->size();i++)
    {
        if( monstergen.m_hItem == (*m_pvtMonList)[i]->m_hItem )
        {
            m_pvtMonList->erase((m_pvtMonList->begin()+i));
            CQuestScripterDlg *pDlgQuestScript = ((CQuestScripterDlg*)AfxGetMainWnd());
            if( pDlgQuestScript )
            {
                if( !pDlgQuestScript->m_dlgTrigger.m_dlgTriggerContent.DelActionItem(monstergen.m_hItem) )
                    pDlgQuestScript->m_dlgTrigger.m_dlgTriggerContent.DelEventItem(monstergen.m_hItem);
            }
            break;
        }
    }

    OnCancel();
}
