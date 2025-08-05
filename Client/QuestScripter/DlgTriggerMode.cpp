// DlgTriggerMode.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTriggerMode.h"
#include ".\dlgtriggermode.h"


// CDlgTriggerMode 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTriggerMode, CDialog)
CDlgTriggerMode::CDlgTriggerMode(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTriggerMode::IDD, pParent)
{
}

CDlgTriggerMode::~CDlgTriggerMode()
{
}

void CDlgTriggerMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTriggerMode, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgTriggerMode 메시지 처리기입니다.

void CDlgTriggerMode::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnOK();
}

void CDlgTriggerMode::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnCancel();
}
