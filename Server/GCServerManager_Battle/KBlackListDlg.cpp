// KBlackListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GCServerManager.h"
#include "GCServerManagerView.h"
#include "MainFrm.h"
#include "KBlackListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KBlackListDlg dialog


KBlackListDlg::KBlackListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KBlackListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(KBlackListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_pMainFrm = NULL;
}


void KBlackListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(KBlackListDlg)
	DDX_Control(pDX, IDC_LIST_BLACK_LIST, m_ctrlBlackList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(KBlackListDlg, CDialog)
	//{{AFX_MSG_MAP(KBlackListDlg)
	ON_LBN_DBLCLK(IDC_LIST_BLACK_LIST, OnDblclkListBlackList)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void KBlackListDlg::UpdateBlackList( void )
{
    if ( NULL == m_pMainFrm ) return;

    m_ctrlBlackList.ResetContent();
    for ( size_t i = 0; i < m_pMainFrm->m_vecBlackList.size(); i++ )
    {
        m_ctrlBlackList.InsertString( -1, m_pMainFrm->m_vecBlackList[i].c_str() );
    }
}

/////////////////////////////////////////////////////////////////////////////
// KBlackListDlg message handlers

void KBlackListDlg::OnOK( void )
{
	// empty
}

void KBlackListDlg::OnCancel( void )
{
    m_pMainFrm->SendMessage( WM_COMMAND, MAKEWPARAM( KBlackListDlg::ID_BTN_CLOSE, 0 ), 0 );
}

BOOL KBlackListDlg::OnInitDialog( void )
{
    CDialog::OnInitDialog();

    m_pMainFrm = static_cast<CMainFrame*>( ::AfxGetMainWnd() );

    UpdateBlackList();

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void KBlackListDlg::OnDblclkListBlackList( void )
{
    int iIdx = m_ctrlBlackList.GetCurSel();

    if ( LB_ERR == iIdx ) return;

    CString cstrLoginID;
    m_ctrlBlackList.GetText( iIdx, cstrLoginID );

    std::wstring strLoginID = (LPCWSTR)cstrLoginID;
    strLoginID.erase( 0, strLoginID.find( L']' ) + 1 );

    std::wstring::size_type idx;
    if ( std::wstring::npos != ( idx = strLoginID.find( L"(Abuse)" ) ) ||
         std::wstring::npos != ( idx = strLoginID.find( L"(Papering)" ) ) ||
         std::wstring::npos != ( idx = strLoginID.find( L"(Impersonation)" ) ) )
    {
        strLoginID.erase( idx );
    }

    CGCServerManagerView* pChildView = static_cast<CGCServerManagerView*>(
        m_pMainFrm->GetActiveFrame()->GetActiveView() );

    //microcat. 이녀석은 잘 모르겠다.
    pChildView->m_server.Send_UserDBInfoReq( pChildView->m_strWideServerName.c_str(), strLoginID );
    m_pMainFrm->m_logFile << L"[request] " << strLoginID
                          << L" It queries user's DB information." << std::endl;
}

void KBlackListDlg::OnButtonRemove( void )
{
    int iIdx = m_ctrlBlackList.GetCurSel();

    if ( LB_ERR == iIdx ) return;

    m_ctrlBlackList.DeleteString( iIdx );
    m_pMainFrm->m_vecBlackList.erase( m_pMainFrm->m_vecBlackList.begin() + iIdx );
}
