// DlgTGWin.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGInfinityMonsterGen.h"


// CDlgTGInfinityMonsterGen 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGInfinityMonsterGen, CDlgTGBase)
CDlgTGInfinityMonsterGen::CDlgTGInfinityMonsterGen(CWnd* pParent /*=NULL*/)
{
}

CDlgTGInfinityMonsterGen::~CDlgTGInfinityMonsterGen()
{
}

void CDlgTGInfinityMonsterGen::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_INFINITY_MON, m_strInfinityGen);
}


BEGIN_MESSAGE_MAP(CDlgTGInfinityMonsterGen, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgTGInfinityMonsterGen 메시지 처리기입니다.
void CDlgTGInfinityMonsterGen::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
    // TODO: 여기에 명령 처리기 코드를 추가합니다.    
}

void CDlgTGInfinityMonsterGen::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}

void CDlgTGInfinityMonsterGen::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    CString strText1;
    m_strInfinityGen.GetWindowText(strText1);
    m_strDesc = strText1;

    //************************************************************************
    // 값 저장
    //************************************************************************
    m_InfinityMonsterGen.m_strDesc = GetDesc();
    m_InfinityMonsterGen.m_bInfinityMonserGen = true;
}

void CDlgTGInfinityMonsterGen::Refresh(void)
{

}