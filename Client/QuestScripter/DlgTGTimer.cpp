// DlgTGTimer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGTimer.h"
#include ".\dlgtgtimer.h"


// CDlgTGTimer 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGTimer, CDlgTGBase)
CDlgTGTimer::CDlgTGTimer(CWnd* pParent /*=NULL*/)
{
}

CDlgTGTimer::~CDlgTGTimer()
{
}

void CDlgTGTimer::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SPIN_TIMER, m_SpinButton);
    DDX_Control(pDX, IDC_EDIT_TIMER, m_ebTime);
    DDX_Control(pDX, IDC_STATIC_TIMER_1, m_stTimer1);
    DDX_Control(pDX, IDC_COMBO_TIMER_ONOFF, m_cbTimerUI);

    m_ebTime.SetWindowText(L"0");
    m_cbTimerUI.ResetContent();
    m_cbTimerUI.InsertString(0, L"Não mostrar timer na UI");
	m_cbTimerUI.InsertString(1, L"Mostrar timer na UI");
	m_cbTimerUI.InsertString(2, L"Mostrar timer na UI ao aparecer o monstro");
    if( m_cbTimerUI.GetCount() ) m_cbTimerUI.SetCurSel(0);

    Refresh();
}


BEGIN_MESSAGE_MAP(CDlgTGTimer, CDialog)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TIMER, OnDeltaposSpinTimer)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgTGTimer 메시지 처리기입니다.

void CDlgTGTimer::OnDeltaposSpinTimer(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CString strInteger;
    m_ebTime.GetWindowText(strInteger);
    int iPos = _wtoi(strInteger);
    if( pNMUpDown->iDelta < 0 )
        iPos++;
    else
        iPos--;
    strInteger.Format(L"%d", iPos);
    m_ebTime.SetWindowText(strInteger);

    *pResult = 0;
}

void CDlgTGTimer::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTGTimer::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}

void CDlgTGTimer::Refresh(void)
{    
    CString strValue;
    strValue.Format(L"%d", m_Timer.m_iTime);
    m_ebTime.SetWindowText(strValue);

    if( m_cbTimerUI.GetCount() > (int)m_Timer.m_iUI ) m_cbTimerUI.SetCurSel(m_Timer.m_iUI);
}

void CDlgTGTimer::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    CString strTime;
    CString strText1;
    m_ebTime.GetWindowText(strTime);
    m_stTimer1.GetWindowText(strText1);

    CString strUIOnOff = L"";
    if( m_cbTimerUI.GetCurSel() >= 0 )
        m_cbTimerUI.GetLBText(m_cbTimerUI.GetCurSel(), strUIOnOff);

    m_strDesc = MiddleBraceSidebyString(strTime) + L" " + strText1 + L" " + MiddleBraceSidebyString(strUIOnOff);

    //************************************************************************
    // 값 저장
    //************************************************************************
    CString strValue;
    m_ebTime.GetWindowText(strValue);
    m_Timer.m_iUI   = (int)m_cbTimerUI.GetCurSel();
    m_Timer.m_iTime = _wtoi(strValue);
    m_Timer.m_strDesc = GetDesc();
}