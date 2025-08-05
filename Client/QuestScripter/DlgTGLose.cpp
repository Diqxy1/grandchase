// DlgTGLose.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGLose.h"


// CDlgTGLose 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGLose, CDlgTGBase)
CDlgTGLose::CDlgTGLose(CWnd* pParent /*=NULL*/)
{
}

CDlgTGLose::~CDlgTGLose()
{
}

void CDlgTGLose::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_LOSE_1, m_stLose);
}


BEGIN_MESSAGE_MAP(CDlgTGLose, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgTGLose 메시지 처리기입니다.
void CDlgTGLose::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTGLose::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}

void CDlgTGLose::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    CString strText1;
    m_stLose.GetWindowText(strText1);
    m_strDesc = strText1;

    //************************************************************************
    // 값 저장
    //************************************************************************
    m_Lose.m_strDesc = GetDesc();
}

void CDlgTGLose::Refresh(void)
{

}