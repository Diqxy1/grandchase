// DlgTGOpenDGate.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGOpenDGate.h"

// CDlgTGOpenDGate 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGOpenDGate, CDlgTGBase)
CDlgTGOpenDGate::CDlgTGOpenDGate(CWnd* pParent /*=NULL*/)
{
}

CDlgTGOpenDGate::~CDlgTGOpenDGate()
{
}

void CDlgTGOpenDGate::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_DIRECTION, m_cbDirection);

    m_cbDirection.ResetContent();
    m_cbDirection.InsertString(0, L"왼쪽");
    m_cbDirection.InsertString(1, L"위쪽");
    m_cbDirection.InsertString(2, L"오른쪽");
    m_cbDirection.InsertString(3, L"아래쪽");
    if( m_cbDirection.GetCount() > 0 ) m_cbDirection.SetCurSel(0);
    DDX_Control(pDX, IDC_STATIC_OPEN_GATELEFT_1, m_stOpenGate1);
}


BEGIN_MESSAGE_MAP(CDlgTGOpenDGate, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgTGOpenDGate 메시지 처리기입니다.

void CDlgTGOpenDGate::Refresh(void)
{
    int iDirection = m_TGOpenDGate.ConvertToComboBoxNumber(m_TGOpenDGate.m_iDirection);
    if( m_cbDirection.GetCount() > iDirection ) m_cbDirection.SetCurSel(iDirection);
}

void CDlgTGOpenDGate::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    CString strDirection;
    if( m_cbDirection.GetCurSel() >= 0 )
        m_cbDirection.GetLBText(m_cbDirection.GetCurSel(), strDirection);

    CString strText1;
    m_stOpenGate1.GetWindowText(strText1);
    m_strDesc = MiddleBraceSidebyString(strDirection) + L" " + strText1;

    //************************************************************************
    // 값 저장
    //************************************************************************    
    m_TGOpenDGate.m_iDirection = m_TGOpenDGate.ConvertToImageNumber(m_cbDirection.GetCurSel());
    m_TGOpenDGate.m_strDesc = GetDesc();
}

void CDlgTGOpenDGate::OnBnClickedOk()
{

}

void CDlgTGOpenDGate::OnBnClickedCancel()
{

}