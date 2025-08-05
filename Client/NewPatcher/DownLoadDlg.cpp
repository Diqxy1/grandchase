#include "stdafx.h"
#include "DownLoadDlg.h"
#include "define.h"
#include ".\downloaddlg.h"
#include "GCGlobalValue.h"
#include "dbg/dbg.hpp"
#include "GCPatcherStr.h"

extern std::vector< std::string > g_vecArgv;
extern bool DeleteFileAbsolutely( const char* szFileName );

extern FILE *fto;


IMPLEMENT_DYNAMIC(CDownLoadDlg, CDialog)
CDownLoadDlg::CDownLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDownLoadDlg::IDD, pParent)
{    

}

CDownLoadDlg::~CDownLoadDlg()
{
	DestroyWindow();
}

void CDownLoadDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_STATE, StrState);
}


BEGIN_MESSAGE_MAP(CDownLoadDlg, CDialog)
    ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CDownLoadDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    SetTimer( 0, 1000, NULL );    
    return true;
}


void CDownLoadDlg::OnCancel()
{	
	CDialog::OnCancel();
	return;
}

int CDownLoadDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CDownLoadDlg::OnClose()
{

	CDialog::OnClose();
}

void CDownLoadDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDownLoadDlg::OnError( std::string url,int curlcode,int httpcode )
{
	
}   

void CDownLoadDlg::SetText( CString str )
{
	StrState.SetWindowText(str);
}
