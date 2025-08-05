// DlgTGWin.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGWin.h"
#include ".\dlgtgwin.h"


// CDlgTGWin 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGWin, CDlgTGBase)
CDlgTGWin::CDlgTGWin(CWnd* pParent /*=NULL*/)
{
}

CDlgTGWin::~CDlgTGWin()
{
}

void CDlgTGWin::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_WIN_1, m_stWin1);
}


BEGIN_MESSAGE_MAP(CDlgTGWin, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgTGWin 메시지 처리기입니다.
void CDlgTGWin::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTGWin::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}

void CDlgTGWin::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    CString strText1;
    m_stWin1.GetWindowText(strText1);
    m_strDesc = strText1;

    //************************************************************************
    // 값 저장
    //************************************************************************
    m_Win.m_strDesc = GetDesc();

}

void CDlgTGWin::Refresh(void)
{

}