// DlgHelp.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgHelp.h"
#include "QuestScripterDlg.h"
#include ".\dlghelp.h"
// CDlgHelp 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgHelp, CDialog)
CDlgHelp::CDlgHelp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHelp::IDD, pParent)
{
}

CDlgHelp::~CDlgHelp()
{
}

void CDlgHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_HELP, m_edHelp);

	CString strFullPath = GCUTIL_STR::GCStrCharToWide(g_Path);
	strFullPath += L"/Help.txt";

	HANDLE hRead = CreateFile(strFullPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hRead)
	{
		CString strMsg = strFullPath;
		strMsg += L"파일을 열수 없습니다.";
		MessageBox(strMsg, L"열기오류", MB_OK);
		return;
	}
	DWORD dwSize, dwFileSize;
	dwFileSize = SetFilePointer(hRead, 0, 0, FILE_END);
	SetFilePointer(hRead, 0, 0, FILE_BEGIN);

	CString strHelp;
	try
	{
		char *szHelp = new char[dwFileSize + 10];
		ZeroMemory(szHelp, (dwFileSize + 10) * sizeof(char));
		ReadFile(hRead, szHelp, dwFileSize, &dwSize, 0);
		strHelp = szHelp;
		delete[]szHelp;
	}
	catch (...)
	{
		MessageBox(L"도움말을 열수 없습니다.", L"열기오류", MB_OK);
		CloseHandle(hRead);
		return;
	}

	CloseHandle(hRead);
	m_edHelp.SetBackColor(RGB(255, 255, 255));
	m_edHelp.SetReadOnly(1);
	m_edHelp.SetWindowText(strHelp);
}

BEGIN_MESSAGE_MAP(CDlgHelp, CDialog)
END_MESSAGE_MAP()

// CDlgHelp 메시지 처리기입니다.

BOOL CDlgHelp::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}