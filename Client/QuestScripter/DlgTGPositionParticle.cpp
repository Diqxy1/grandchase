// DlgTGPositionParticle.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGPositionParticle.h"
#include ".\dlgtgpositionparticle.h"
#include "FunctionToScriptFile.h"

// CDlgTGPositionParticle 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGPositionParticle, CDlgTGBase)
CDlgTGPositionParticle::CDlgTGPositionParticle(CWnd* pParent /*=NULL*/)
{
}

CDlgTGPositionParticle::~CDlgTGPositionParticle()
{
}

void CDlgTGPositionParticle::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_LEFT, m_ebLeft);
    DDX_Control(pDX, IDC_EDIT_RIGHT, m_ebRight);
    DDX_Control(pDX, IDC_EDIT_TOP, m_ebTop);
    DDX_Control(pDX, IDC_EDIT_BOTTOM, m_ebBottom);
    DDX_Control(pDX, IDC_COMBO_POSITION_PARTICLE, m_cbParticleID);
    DDX_Control(pDX, IDC_STATIC_POSITION_PARTICLE_1, m_stPositionParticle1);
    DDX_Control(pDX, IDC_STATIC_POSITION_PARTICLE_2, m_stPositionParticle2);

    m_cbParticleID.Init();
}


BEGIN_MESSAGE_MAP(CDlgTGPositionParticle, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON_SELECT_FROM_MAP_SEL_AREA, OnBnClickedButtonSelectFromMapSelArea)
END_MESSAGE_MAP()


// CDlgTGPositionParticle 메시지 처리기입니다.

void CDlgTGPositionParticle::Refresh(void)
{
    CString strLeft, strRight, strTop, strBottom;
    strLeft.Format(L"%d", m_TGPositionParticle.m_iLeft);
    strRight.Format(L"%d", m_TGPositionParticle.m_iRight);
    strTop.Format(L"%d", m_TGPositionParticle.m_iTop);
    strBottom.Format(L"%d", m_TGPositionParticle.m_iBottom);

    m_ebLeft.SetWindowText(strLeft);
    m_ebRight.SetWindowText(strRight);
    m_ebTop.SetWindowText(strTop);
    m_ebBottom.SetWindowText(strBottom);

    if( m_cbParticleID.GetCount() > (int)m_TGPositionParticle.m_iParticleID ) m_cbParticleID.SetCurSel((int)m_TGPositionParticle.m_iParticleID);
}

void CDlgTGPositionParticle::CalcResult(void)
{
    //************************************************************************
    // 값 저장
    //************************************************************************

    CString strLeft, strRight, strTop, strBottom, strTotal;
    m_ebLeft.GetWindowText(strLeft);
    m_ebRight.GetWindowText(strRight);
    m_ebTop.GetWindowText(strTop);
    m_ebBottom.GetWindowText(strBottom);
    strTotal.Format(L"LTRB (%s, %s, %s, %s)", strLeft, strTop, strRight, strBottom);

    CString strParticleID;
    if( m_cbParticleID.GetCurSel() >= 0 && m_cbParticleID.GetCount() > m_cbParticleID.GetCurSel() )
        m_cbParticleID.GetLBText(m_cbParticleID.GetCurSel(), strParticleID);

    CString strText1, strText2;
    m_stPositionParticle1.GetWindowText(strText1);
    m_stPositionParticle2.GetWindowText(strText2);

    m_strDesc = MiddleBraceSidebyString(strTotal) + L" " + strText1 
        + L" " + MiddleBraceSidebyString(strParticleID) + strText2;

    //************************************************************************
    // 설명 생성
    //************************************************************************
    m_TGPositionParticle.m_iLeft            = (int)_wtoi(strLeft);
    m_TGPositionParticle.m_iRight           = (int)_wtoi(strRight);
    m_TGPositionParticle.m_iTop             = (int)_wtoi(strTop);
    m_TGPositionParticle.m_iBottom          = (int)_wtoi(strBottom);
    m_TGPositionParticle.m_iParticleID      = (int)m_cbParticleID.GetCurSel();
    m_TGPositionParticle.m_strDesc = GetDesc();
}

void CDlgTGPositionParticle::OnBnClickedOk()
{

}

void CDlgTGPositionParticle::OnBnClickedCancel()
{

}

void CDlgTGPositionParticle::OnBnClickedButtonSelectFromMapSelArea()
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

//************************************************************************
// 영역의 오른쪽이 왼쪽보다 큰값, 아래쪽이 위쪽보다 큰값을 가지게 보정
//************************************************************************
void CDlgTGPositionParticle::RectRevision(CRect &rtArea)
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

void CDlgTGPositionParticle::SetArea(CRect rtArea)
{
    RectRevision(rtArea);
    m_TGPositionParticle.m_iLeft = rtArea.left;
    m_TGPositionParticle.m_iRight = rtArea.right;
    m_TGPositionParticle.m_iTop = rtArea.top;
    m_TGPositionParticle.m_iBottom = rtArea.bottom;

    CString strLeft, strRight, strTop, strBottom;
    strLeft.Format(L"%d", m_TGPositionParticle.m_iLeft);
    strRight.Format(L"%d", m_TGPositionParticle.m_iRight);
    strTop.Format(L"%d", m_TGPositionParticle.m_iTop);
    strBottom.Format(L"%d", m_TGPositionParticle.m_iBottom);

    if( m_ebLeft.GetSafeHwnd() )
        m_ebLeft.SetWindowText(strLeft);
    if( m_ebRight.GetSafeHwnd() )
        m_ebRight.SetWindowText(strRight);
    if( m_ebTop.GetSafeHwnd() )
        m_ebTop.SetWindowText(strTop);
    if( m_ebBottom.GetSafeHwnd() )
        m_ebBottom.SetWindowText(strBottom);
}