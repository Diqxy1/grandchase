// KInsertDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UI Script Studio.h"
#include "KInsertDialog.h"
#include ".\kinsertdialog.h"
#include "KEditDialog.h"


// KInsertDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInsertDialog, CDialog)
KInsertDialog::KInsertDialog(CWnd* pParent /*=NULL*/)
	: CDialog(KInsertDialog::IDD, pParent)
{
}

KInsertDialog::~KInsertDialog()
{
}

void KInsertDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_CtrlValueList);
}


BEGIN_MESSAGE_MAP(KInsertDialog, CDialog)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST1, OnLvnItemActivateList1)
END_MESSAGE_MAP()


// KInsertDialog 메시지 처리기입니다.




int KInsertDialog::OnInitDialog()
{
	if (CDialog::OnInitDialog() == -1 )
		return -1;
		
	m_CtrlValueList.InsertColumn(0, _T("Name"));
	m_CtrlValueList.InsertColumn(1,_T("Value"));
	CStringArray Column;
	Column.Add(_T("name")); 
	Column.Add(_T("type"));
	Column.Add(_T("width"));
	Column.Add(_T("height"));
	Column.Add(_T("window_x"));
	Column.Add(_T("window_y"));

	for (int i = 0 ; i < Column.GetSize(); i++)
		m_CtrlValueList.InsertItem(i,Column[i]);
	

	m_CtrlValueList.SetColumnWidth(0,90);
	m_CtrlValueList.SetColumnWidth(1,90);
	
	
	return 0;
}

void KInsertDialog::OnLvnItemActivateList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	
	KEditDialog dlg;
	CString strName , strValue;

	strName = m_CtrlValueList.GetItemText(pNMIA->iItem,0);

	dlg.m_strName = strName;

	strValue = m_CtrlValueList.GetItemText(pNMIA->iItem,1);

	dlg.m_strValue = strValue;


	if ( dlg.DoModal() == IDOK )
	{
		//m_pDataPackage->SetValue( (LPCTSTR)dlg.m_strName, (LPTSTR)(LPCTSTR)dlg.m_strValue );
		m_CtrlValueList.SetItemText( pNMIA->iItem, 1, (LPTSTR)(LPCTSTR)dlg.m_strValue );
		m_WndPackager.SetValue( ToCharStr( dlg.m_strName.GetBuffer() ) , 
			(char*)ToCharStr( dlg.m_strValue.GetBuffer() ) );
	}
}

void KInsertDialog::GetScriptWindow(KD3DWndClass* pkWnd)
{
	m_WndPackager.PushWnd(pkWnd);
	
    
}
void KInsertDialog::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	//여기서 처리...


	CDialog::OnOK();
}
