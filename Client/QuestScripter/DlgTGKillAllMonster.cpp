// DlgTGWin.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGKillAllMonster.h"


// CDlgTGInfinityMonsterGen 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGKillAllMonster, CDlgTGBase)
CDlgTGKillAllMonster::CDlgTGKillAllMonster(CWnd* pParent /*=NULL*/)
{
}

CDlgTGKillAllMonster::~CDlgTGKillAllMonster()
{
}

void CDlgTGKillAllMonster::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_KILL_ALL_MONSTER, m_stKillAllMonster);
}


BEGIN_MESSAGE_MAP(CDlgTGKillAllMonster, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgTGInfinityMonsterGen 메시지 처리기입니다.
void CDlgTGKillAllMonster::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
    // TODO: 여기에 명령 처리기 코드를 추가합니다.    
}

void CDlgTGKillAllMonster::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}

void CDlgTGKillAllMonster::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    CString strText1;
    m_stKillAllMonster.GetWindowText(strText1);
    m_strDesc = strText1;

    //************************************************************************
    // 값 저장
    //************************************************************************
    m_KillAllMonster.m_strDesc = GetDesc();
    m_KillAllMonster.m_bAlive = false;
}

void CDlgTGKillAllMonster::Refresh(void)
{

}