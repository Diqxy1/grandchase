// DlgQuestSequence.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgQuestSequence.h"
#include ".\dlgquestsequence.h"


// CDlgQuestSequence 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgQuestSequence, CDialog)
CDlgQuestSequence::CDlgQuestSequence(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgQuestSequence::IDD, pParent)
{
    m_bCreated = 0;
}

CDlgQuestSequence::~CDlgQuestSequence()
{
}

void CDlgQuestSequence::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgQuestSequence, CDialog)
    ON_WM_CREATE()
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgQuestSequence 메시지 처리기입니다.

int CDlgQuestSequence::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialog::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  여기에 특수화된 작성 코드를 추가합니다.
    m_bCreated = 1;

    return 0;
}

void CDlgQuestSequence::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgQuestSequence::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}