// DlgTGPlayerPosition.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGPlayerPosition.h"
#include "FunctionToScriptFile.h"
#include ".\dlgtgplayerposition.h"

// CDlgTGPlayerPosition 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGPlayerPosition, CDlgTGBase)
CDlgTGPlayerPosition::CDlgTGPlayerPosition(CWnd* pParent /*=NULL*/)
{
}

CDlgTGPlayerPosition::~CDlgTGPlayerPosition()
{
}

void CDlgTGPlayerPosition::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_PLAYER_SLOT, m_cbPlayerIndex);
    DDX_Control(pDX, IDC_EDIT_PLAYER_X, m_ebX);
    DDX_Control(pDX, IDC_EDIT_PLAYER_Y, m_ebY);
    DDX_Control(pDX, IDC_STATIC_PLAYERPOSITION_1, m_stPlayerPosition1);
    DDX_Control(pDX, IDC_STATIC_PLAYERPOSITION_2, m_stPlayerPosition2);
    DDX_Control(pDX, IDC_COMBO_TG_PLAYERPOSITION_RIGHT, m_cbRight);
    DDX_Control(pDX, IDC_STATIC_PLAYERPOSITION_3, m_stPlayerPosition3);
    m_cbPlayerIndex.Init();

    m_cbPlayerIndex.ResetContent();
    for(int i=0;i<(int)MAX_SLOT_PLAYER;++i)
    {
        CString strIndex;
        strIndex.Format(L"%d", i);
        m_cbPlayerIndex.InsertString(i, strIndex);
        m_cbPlayerIndex.AddSearchString(strIndex);
    }

    m_cbRight.ResetContent();
    m_cbRight.InsertString(0, L"왼쪽");
    m_cbRight.InsertString(1, L"오른쪽");
    if( m_cbRight.GetCount() ) m_cbRight.SetCurSel(0);
}


BEGIN_MESSAGE_MAP(CDlgTGPlayerPosition, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()

void CDlgTGPlayerPosition::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTGPlayerPosition::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}

// CDlgTGPlayerPosition 메시지 처리기입니다.

void CDlgTGPlayerPosition::Refresh(void)
{
    if( m_cbPlayerIndex.GetCount() > m_PlayerPosition.m_iPlayerIndex ) m_cbPlayerIndex.SetCurSel(m_PlayerPosition.m_iPlayerIndex);
    if( m_cbRight.GetCount() > (int)m_PlayerPosition.m_bRight ) m_cbRight.SetCurSel((int)m_PlayerPosition.m_bRight);

    CString strX, strY;
    strX.Format(L"%f", m_PlayerPosition.m_fEntranceX);
    strY.Format(L"%f", m_PlayerPosition.m_fEntranceY);
    m_ebX.SetWindowText(strX);
    m_ebY.SetWindowText(strY);
}

void CDlgTGPlayerPosition::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    {
        CString strIndex = L"X";
        if( m_cbPlayerIndex.GetCurSel() >= 0 && m_cbPlayerIndex.GetCount() > m_cbPlayerIndex.GetCurSel() )
            m_cbPlayerIndex.GetLBText(m_cbPlayerIndex.GetCurSel(), strIndex);

        CString strRight = L"X";
        if( m_cbRight.GetCurSel() >= 0 && m_cbRight.GetCount() > m_cbRight.GetCurSel() )
            m_cbRight.GetLBText(m_cbRight.GetCurSel(), strRight);
        

        CString strX, strY;
        m_ebX.GetWindowText(strX);
        m_ebY.GetWindowText(strY);

        CString strText1, strText2, strText3;
        m_stPlayerPosition1.GetWindowText(strText1);
        m_stPlayerPosition2.GetWindowText(strText2);
        m_stPlayerPosition3.GetWindowText(strText3);

        m_strDesc = MiddleBraceSidebyString(strIndex) + L" " + strText1 + L" " + MiddleBraceSidebyString(strRight)
            + L" " + strText2 + L" "
            + L" (" + MiddleBraceSidebyString(strX) + L"," + MiddleBraceSidebyString(strY) + L") " + strText3;
    }


    //************************************************************************
    // 값 저장
    //************************************************************************
    {
        m_PlayerPosition.m_iPlayerIndex = m_cbPlayerIndex.GetCurSel();
        m_PlayerPosition.m_bRight       = (int)m_cbRight.GetCurSel();

        CString strX, strY;
        m_ebX.GetWindowText(strX);
        m_ebY.GetWindowText(strY);
        m_PlayerPosition.m_fEntranceX = (float)_wtof(strX);
        m_PlayerPosition.m_fEntranceY = (float)_wtof(strY);
        m_PlayerPosition.m_strDesc = GetDesc();
    }
}
void CDlgTGPlayerPosition::OnBnClickedButton1()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( AfxGetMainWnd() )
    {
        // GetParent()->GetParent() 는 CDlgTGChildGenner 클래스입니다.
        AfxGetMainWnd()->SendMessage(WM_PLAYER_ADD_WINDOW, 0, 0);
        SetFocus();
    }
}
