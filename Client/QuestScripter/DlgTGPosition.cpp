// DlgTGPosition.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGPosition.h"
#include ".\dlgtgposition.h"
#include "ObjectDataManager.h"

// CDlgTGPosition 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGPosition, CDlgTGBase)
CDlgTGPosition::CDlgTGPosition(CWnd* pParent /*=NULL*/)
{
}

CDlgTGPosition::~CDlgTGPosition()
{
}

void CDlgTGPosition::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_MON_SLOT, m_cbSlot);
    DDX_Control(pDX, IDC_COMBO_PLAYER_OR_MONSTER, m_cbPlayer_or_Monster);
    DDX_Control(pDX, IDC_COMBO_MONSTER_TYPE, m_cbMonID);
    DDX_Control(pDX, IDC_COMBO_HALF_MORE, m_cbHalfMore);
    DDX_Control(pDX, IDC_BUTTON_SELECT_FROM_MAP_SEL_MON, m_btMonster);
    DDX_Control(pDX, IDC_COMBO_IN_OUT, m_cbInOut);

    m_cbPlayer_or_Monster.ResetContent();
    m_cbPlayer_or_Monster.InsertString(0, L"Monstro");
    m_cbPlayer_or_Monster.InsertString(1, L"Player");
    if( m_cbPlayer_or_Monster.GetCount() )
    {
        m_cbPlayer_or_Monster.SetCurSel(0);
        ResetSlotByPlayer();
    }

    m_cbMonID.ResetContent();
    for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_MON).size();++i)
        m_cbMonID.InsertString( i, SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_MON)[i] );
    if( m_cbMonID.GetCount() ) m_cbMonID.SetCurSel(0);
    m_cbMonID.EnableWindow(false);
    DDX_Control(pDX, IDC_EDIT_LEFT, m_ebLeft);
    DDX_Control(pDX, IDC_EDIT_RIGHT, m_ebRight);
    DDX_Control(pDX, IDC_EDIT_TOP, m_ebTop);
    DDX_Control(pDX, IDC_EDIT_BOTTOM, m_ebBottom);

    m_cbHalfMore.ResetContent();
    m_cbHalfMore.InsertString(0, L"Slot");
    m_cbHalfMore.InsertString(1, L"Mais da metade");
    if( m_cbHalfMore.GetCount() > 1 )
        m_cbHalfMore.SetCurSel(1);
    OnCbnSelchangeComboHalfMore();

    m_cbInOut.ResetContent();
    m_cbInOut.InsertString(0, L"Escapar.");
    m_cbInOut.InsertString(1, L"Entrar.");

    Refresh();
    DDX_Control(pDX, IDC_STATIC_POSITION_1, m_stPosition1);
    DDX_Control(pDX, IDC_STATIC_POSITION_2, m_stPosition2);
}


BEGIN_MESSAGE_MAP(CDlgTGPosition, CDialog)
    ON_CBN_SELCHANGE(IDC_COMBO_PLAYER_OR_MONSTER, OnCbnSelchangeComboPlayerOrMonster)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BUTTON_SELECT_FROM_MAP_SEL_MON, OnBnClickedButtonSelectFromMapSelMon)
    ON_BN_CLICKED(IDC_BUTTON_SELECT_FROM_MAP_SEL_AREA, OnBnClickedButtonSelectFromMapSelArea)
    ON_CBN_SELCHANGE(IDC_COMBO_HALF_MORE, OnCbnSelchangeComboHalfMore)
END_MESSAGE_MAP()


// CDlgTGPosition 메시지 처리기입니다.

void CDlgTGPosition::OnCbnSelchangeComboPlayerOrMonster()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    switch( m_cbPlayer_or_Monster.GetCurSel() )
    {
        case 0:         // Monster의 경우
            ResetSlotByMonster();
            m_cbSlot.EnableWindow(true);
            m_cbMonID.EnableWindow(true);
            if( m_cbHalfMore.GetCount() > 0 ) m_cbHalfMore.SetCurSel(0); // 슬롯 모드 변경
            m_btMonster.EnableWindow(true);
            break;
        case 1:
            ResetSlotByPlayer();
            m_cbMonID.EnableWindow(false);
            m_btMonster.EnableWindow(false);
            break;      // Player의 경우
    }
}

void CDlgTGPosition::ResetSlotByMonster(void)
{
    m_cbSlot.ResetContent();
    for(int i=0;i<MAX_SLOT_MONSTER;i++)
    {
        CString strText;
        strText.Format(L"%d", i);
        m_cbSlot.InsertString(i, strText);
    }
    if( m_cbSlot.GetCount() > m_Position.m_iIndex ) m_cbSlot.SetCurSel(m_Position.m_iIndex);
}

void CDlgTGPosition::ResetSlotByPlayer(void)
{
    m_cbSlot.ResetContent();
    for(int i=0;i<MAX_SLOT_PLAYER;i++)
    {
        CString strText;
        strText.Format(L"%d", i);
        m_cbSlot.InsertString(i, strText);
    }
    if( m_cbSlot.GetCount() > m_Position.m_iIndex ) m_cbSlot.SetCurSel(m_Position.m_iIndex);
}

void CDlgTGPosition::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTGPosition::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}

void CDlgTGPosition::Refresh(void)
{
    if( m_cbPlayer_or_Monster.GetCount() > (int)m_Position.m_bPlayer ) m_cbPlayer_or_Monster.SetCurSel((int)m_Position.m_bPlayer);
    if( m_cbHalfMore.GetCount() > (int)m_Position.m_bHalfMore ) m_cbHalfMore.SetCurSel((int)m_Position.m_bHalfMore);

    if( m_cbSlot.GetCount() > m_Position.m_iIndex ) m_cbSlot.SetCurSel(m_Position.m_iIndex);
    if( m_cbMonID.GetCount() > m_Position.m_iType ) m_cbMonID.SetCurSel(m_Position.m_iType);

    CString strLeft, strRight, strTop, strBottom;
    strLeft.Format(L"%d", m_Position.m_rtBound.left);
    strRight.Format(L"%d", m_Position.m_rtBound.right);
    strTop.Format(L"%d", m_Position.m_rtBound.top);
    strBottom.Format(L"%d", m_Position.m_rtBound.bottom);

    m_ebLeft.SetWindowText(strLeft);
    m_ebRight.SetWindowText(strRight);
    m_ebTop.SetWindowText(strTop);
    m_ebBottom.SetWindowText(strBottom);

    if( m_cbInOut.GetCount() > (int)m_Position.m_bInOut ) m_cbInOut.SetCurSel(m_Position.m_bInOut);

    OnCbnSelchangeComboPlayerOrMonster();
    OnCbnSelchangeComboHalfMore();
}

void CDlgTGPosition::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    CString strSlot;
    if( m_cbSlot.GetCurSel() >= 0 )
        m_cbSlot.GetLBText(m_cbSlot.GetCurSel(), strSlot);

    CString strPlayer_or_Monster;
    if( m_cbPlayer_or_Monster.GetCurSel() >= 0 )
        m_cbPlayer_or_Monster.GetLBText(m_cbPlayer_or_Monster.GetCurSel(), strPlayer_or_Monster);

    CString strMonID;
    if( m_cbMonID.GetCurSel() >= 0 )
        m_cbMonID.GetLBText(m_cbMonID.GetCurSel(), strMonID);

    CString strHalfMore;
    if( m_cbHalfMore.GetCurSel() >= 0 )
        m_cbHalfMore.GetLBText(m_cbHalfMore.GetCurSel(), strHalfMore);

    CString strLeft, strRight, strTop, strBottom, strTotal;
    m_ebLeft.GetWindowText(strLeft);
    m_ebRight.GetWindowText(strRight);
    m_ebTop.GetWindowText(strTop);
    m_ebBottom.GetWindowText(strBottom);
    strTotal.Format(L"LTRB (%s, %s, %s, %s)", strLeft, strTop, strRight, strBottom);

    CString strInOut;
    if( m_cbInOut.GetCurSel() >= 0 )
        m_cbInOut.GetLBText(m_cbInOut.GetCurSel(), strInOut);

    CString strText1, strText2, strText3;
    m_stPosition1.GetWindowText(strText1);
    m_stPosition2.GetWindowText(strText2);

    switch( m_cbPlayer_or_Monster.GetCurSel() )
    {
        case 0:     // 몬스터
            m_strDesc = MiddleBraceSidebyString(strPlayer_or_Monster) + L" " + MiddleBraceSidebyString(strSlot)
                + L" " + MiddleBraceSidebyString(strMonID) + L" " + strText1 + L" " + MiddleBraceSidebyString(strTotal)
                + strText2 + MiddleBraceSidebyString(strInOut);
            break;
        case 1:     // 플레이어
            {
                CString strPlayerWho;
                if( m_cbHalfMore.GetCurSel() )     // 과반수 이상 모드 else 슬롯 모드
                    strPlayerWho = strHalfMore;
                else
                    strPlayerWho = strSlot;
                m_strDesc = MiddleBraceSidebyString(strPlayer_or_Monster) + L" " + MiddleBraceSidebyString(strPlayerWho)
                    + L" " + strText1 + L" " + MiddleBraceSidebyString(strTotal) + strText2 + MiddleBraceSidebyString(strInOut);
            }
            break;
    }

    //************************************************************************
    // 값 저장
    //************************************************************************
    m_Position.m_iIndex     = m_cbSlot.GetCurSel();
    m_Position.m_bPlayer    = m_cbPlayer_or_Monster.GetCurSel();
    m_Position.m_bHalfMore  = m_cbHalfMore.GetCurSel();
    m_Position.m_iType      = m_cbMonID.GetCurSel();
    m_Position.m_bInOut     = m_cbInOut.GetCurSel();

    m_ebLeft.GetWindowText(strLeft);
    m_ebRight.GetWindowText(strRight);
    m_ebTop.GetWindowText(strTop);
    m_ebBottom.GetWindowText(strBottom);
    m_Position.m_rtBound.left   = _wtoi(strLeft);
    m_Position.m_rtBound.right  = _wtoi(strRight);
    m_Position.m_rtBound.top    = _wtoi(strTop);
    m_Position.m_rtBound.bottom = _wtoi(strBottom);
    m_Position.m_strDesc        = GetDesc();
}

void CDlgTGPosition::OnBnClickedButtonSelectFromMapSelMon()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( AfxGetMainWnd() )
    {
        // GetParent()->GetParent() 는 CDlgTGChildGenner 클래스입니다.
        AfxGetMainWnd()->SendMessage(WM_MONSTER_SEL_WINDOW, 0, 0);
        SetFocus();
    }
}

void CDlgTGPosition::OnBnClickedButtonSelectFromMapSelArea()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( AfxGetMainWnd() )
    {
        CalcResult();
        // GetParent()->GetParent() 는 CDlgTGChildGenner 클래스입니다.
        AfxGetMainWnd()->SendMessage(WM_AREA_SEL_WINDOW, 0, 0);
        SetFocus();
    }
}

void CDlgTGPosition::SetMonsterInfo(bool bPlayer, int iSlot, int iType)
{
    m_Position.m_bPlayer    = bPlayer;
    m_Position.m_iIndex     = iSlot;
    m_Position.m_iType      = iType;

    if( m_cbSlot.GetSafeHwnd() )
    {
        if( m_cbSlot.GetCount() > m_Position.m_iIndex )
            m_cbSlot.SetCurSel(m_Position.m_iIndex);
    }

    if( m_cbPlayer_or_Monster.GetSafeHwnd() )
    {
        if( m_cbPlayer_or_Monster.GetCount() > (int)m_Position.m_bPlayer )
            m_cbPlayer_or_Monster.SetCurSel((int)m_Position.m_bPlayer);
    }

    if( m_cbMonID.GetSafeHwnd() )
    {
        if( m_cbMonID.GetCount() > m_Position.m_iType )
            m_cbMonID.SetCurSel(m_Position.m_iType);
    }

}

//************************************************************************
// 영역의 오른쪽이 왼쪽보다 큰값, 아래쪽이 위쪽보다 큰값을 가지게 보정
//************************************************************************
void CDlgTGPosition::RectRevision(CRect &rtArea)
{
    if( rtArea.left > rtArea.right)
    {
        int iTemp = rtArea.left;
        rtArea.left = rtArea.right;
        rtArea.right = iTemp;
    }
    if( rtArea.top > rtArea.bottom)
    {
        int iTemp = rtArea.top;
        rtArea.top = rtArea.bottom;
        rtArea.bottom = iTemp;
    }
};

void CDlgTGPosition::SetArea(CRect rtArea)
{
    RectRevision(rtArea);
    m_Position.m_rtBound = rtArea;

    CString strLeft, strRight, strTop, strBottom;
    strLeft.Format(L"%d", m_Position.m_rtBound.left);
    strRight.Format(L"%d", m_Position.m_rtBound.right);
    strTop.Format(L"%d", m_Position.m_rtBound.top);
    strBottom.Format(L"%d", m_Position.m_rtBound.bottom);

    if( m_ebLeft.GetSafeHwnd() )
        m_ebLeft.SetWindowText(strLeft);
    if( m_ebRight.GetSafeHwnd() )
        m_ebRight.SetWindowText(strRight);
    if( m_ebTop.GetSafeHwnd() )
        m_ebTop.SetWindowText(strTop);
    if( m_ebBottom.GetSafeHwnd() )
        m_ebBottom.SetWindowText(strBottom);
}

void CDlgTGPosition::OnCbnSelchangeComboHalfMore()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    int iHalMore = m_cbHalfMore.GetCurSel();
    switch(iHalMore)
    {
        case 0:         // 슬롯 모드(한명씩 선택)
            m_cbSlot.EnableWindow(true);
            if( 0 == m_cbPlayer_or_Monster.GetCurSel() )    // 몬스터 모드인가?
            {
                m_cbMonID.EnableWindow(true);
                m_btMonster.EnableWindow(true);
            }
            break;
        case 1:         // 과반수 이상 모드
            if( m_cbPlayer_or_Monster.GetCount() > 1 ) m_cbPlayer_or_Monster.SetCurSel(1);
            m_cbSlot.EnableWindow(false);
            m_cbMonID.EnableWindow(false);
            m_btMonster.EnableWindow(false);
            break;
    }
}
