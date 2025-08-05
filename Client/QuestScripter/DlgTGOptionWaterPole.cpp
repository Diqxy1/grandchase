// DlgTGOptionWaterPole.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGOptionWaterPole.h"
#include ".\dlgtgoptionwaterpole.h"


// CDlgTGOptionWaterPole 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGOptionWaterPole, CDialog)
CDlgTGOptionWaterPole::CDlgTGOptionWaterPole(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTGOptionWaterPole::IDD, pParent)
{
    m_iLife         = 500;    
    m_iCrashStart   = 500;
    m_iMySpeed      = 0;
    m_iJumpFrame    = 0;
    m_iContinueance = 3000;
}

CDlgTGOptionWaterPole::~CDlgTGOptionWaterPole()
{
}

void CDlgTGOptionWaterPole::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_LIFE, m_edLife); 
    DDX_Control(pDX, IDC_EDIT_CONTINUEANCE, m_ebContinueance);
}


BEGIN_MESSAGE_MAP(CDlgTGOptionWaterPole, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgTGOptionWaterPole 메시지 처리기입니다.

void CDlgTGOptionWaterPole::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTGOptionWaterPole::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}

void CDlgTGOptionWaterPole::GetValue(void)
{
    CString str;

    m_edLife.GetWindowText(str);
    m_iLife         = _wtoi(str);   
    
    m_ebContinueance.GetWindowText(str);
    m_iContinueance = _wtoi(str);
}

void CDlgTGOptionWaterPole::SetValue(int iLife, int iStartLife, int iCrashStart, int iMySpeed, int iJumpFrame, int iContinueance)
{
    CString str;

    str.Format(L"%d", iLife);
    m_edLife.SetWindowText(str);    
    str.Format(L"%d", iContinueance);
    m_ebContinueance.SetWindowText(str);

    m_iLife         = iLife;    
    m_iCrashStart   = iCrashStart;
    m_iMySpeed      = iMySpeed;
    m_iJumpFrame    = iJumpFrame;
    m_iContinueance = iContinueance;
}
