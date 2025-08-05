// DlgPlayerPosition_Popup.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgPlayerPosition_Popup.h"
#include ".\dlgplayerposition_popup.h"
#include "DlgMapToObject.h"
#include "DlgTriggerContent.h"
#include "QuestScripterDlg.h"

// CDlgPlayerPosition_Popup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgPlayerPosition_Popup, CDialog)
CDlgPlayerPosition_Popup::CDlgPlayerPosition_Popup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPlayerPosition_Popup::IDD, pParent)
{
    m_bFnAction             = 0;
    m_pvtPlayerPosition     = 0;
}

CDlgPlayerPosition_Popup::~CDlgPlayerPosition_Popup()
{
}

void CDlgPlayerPosition_Popup::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_PLAYERINDEX, m_cbPlayerIndex);
    DDX_Control(pDX, IDC_EDIT_ENTRANCEX, m_edEntranceX);
    DDX_Control(pDX, IDC_EDIT_ENTRANCEY, m_edEntranceY);
    DDX_Control(pDX, IDC_COMBO_EACTION, m_cbFunctionType);
    DDX_Control(pDX, IDC_COMBO_TG_PLAYERPOSITION_RIGHT, m_cbRight);

    SetWindowText(L"플레이어위치추가");

    m_cbPlayerIndex.ResetContent();
    for(int i=0;i<(int)MAX_SLOT_PLAYER;++i)
    {
        CString strIndex;
        strIndex.Format(L"%d", i);
        m_cbPlayerIndex.InsertString(i, strIndex);
    }
    if( m_cbPlayerIndex.GetCount() > 0 ) m_cbPlayerIndex.SetCurSel(0);

    CString str;
    str.Format(L"%f", m_PlayerPosition.m_fEntranceX);
    m_edEntranceX.SetWindowText(str);
    str.Format(L"%f", m_PlayerPosition.m_fEntranceY);
    m_edEntranceY.SetWindowText(str);
    if( m_cbPlayerIndex.GetCount() > m_PlayerPosition.m_iPlayerIndex 
        && m_PlayerPosition.m_iPlayerIndex >= 0 
        )
        m_cbPlayerIndex.SetCurSel(m_PlayerPosition.m_iPlayerIndex);

    m_cbFunctionType.ResetContent();
    m_cbFunctionType.InsertString(0, L"Event");
    m_cbFunctionType.InsertString(1, L"Action");
    if( m_cbFunctionType.GetCount() > 1 ) m_cbFunctionType.SetCurSel(1);

    m_cbRight.ResetContent();
    m_cbRight.InsertString(0, L"왼쪽");
    m_cbRight.InsertString(1, L"오른쪽");
    if( m_cbRight.GetCount() > 0 ) m_cbRight.SetCurSel(0);
}


BEGIN_MESSAGE_MAP(CDlgPlayerPosition_Popup, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_DELETE, OnBnClickedDelete)
END_MESSAGE_MAP()


// CDlgPlayerPosition_Popup 메시지 처리기입니다.
void CDlgPlayerPosition_Popup::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    m_PlayerPosition.m_iPlayerIndex      = m_cbPlayerIndex.GetCurSel();
    if( m_PlayerPosition.m_iPlayerIndex < 0                 // PlayerIndex가 범위를 벗어나있으면 -1 지정.
        || m_PlayerPosition.m_iPlayerIndex >= MAX_SLOT_PLAYER 
        )
        m_PlayerPosition.m_iPlayerIndex = -1;

    CString str;
    m_edEntranceX.GetWindowText(str);
    m_PlayerPosition.m_fEntranceX = (float)_wtof(str);
    m_edEntranceY.GetWindowText(str);
    m_PlayerPosition.m_fEntranceY = (float)_wtof(str);;
    m_PlayerPosition.m_bRight     = (bool)m_cbRight.GetCurSel();
    m_bFnAction                   = m_cbFunctionType.GetCurSel();

    OnOK();
}

void CDlgPlayerPosition_Popup::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnCancel();
}

void CDlgPlayerPosition_Popup::OnBnClickedDelete()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( !m_pvtPlayerPosition ) return;

    for(int i=0;i<(int)m_pvtPlayerPosition->size();i++)
    {
        if( -1 == m_PlayerPosition.m_iPlayerIndex ) continue;

        if( m_PlayerPosition.m_hItem == (*m_pvtPlayerPosition)[i]->m_hItem )
        {
            m_pvtPlayerPosition->erase((m_pvtPlayerPosition->begin()+i));
            CQuestScripterDlg *pDlgQuestScript = ((CQuestScripterDlg*)AfxGetMainWnd());
            if( pDlgQuestScript )
            {
                if( !pDlgQuestScript->m_dlgTrigger.m_dlgTriggerContent.DelActionItem(m_PlayerPosition.m_hItem) )
                    pDlgQuestScript->m_dlgTrigger.m_dlgTriggerContent.DelEventItem(m_PlayerPosition.m_hItem);
            }
            break;
        }
    }

    OnCancel();
}

void CDlgPlayerPosition_Popup::SetPlayerListValue(std::vector<CPlayerPosition*> *pvtPlayerPosition)
{
    m_pvtPlayerPosition = pvtPlayerPosition;
}