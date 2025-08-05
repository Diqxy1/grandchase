// DlgTGUITimer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGUITimer.h"


// CDlgTGUITimer 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGUITimer, CDialog)
CDlgTGUITimer::CDlgTGUITimer(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTGUITimer::IDD, pParent)
{
}

CDlgTGUITimer::~CDlgTGUITimer()
{
}

void CDlgTGUITimer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTGUITimer, CDialog)
END_MESSAGE_MAP()


// CDlgTGUITimer 메시지 처리기입니다.
