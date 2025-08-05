// KServerListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GCServerManager.h"
#include "KAddServerListDlg.h"
#include "KServerListDlg.h"
#include "ChildFrm.h"
#include "GCServerManagerView.h"

#pragma warning(disable:4786)

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KServerListDlg dialog


KServerListDlg::KServerListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KServerListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(KServerListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_pApp      = static_cast<CGCServerManagerApp*>( ::AfxGetApp() );
    m_pMainFrm  = static_cast<CMainFrame*>( ::AfxGetMainWnd() );
}


void KServerListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(KServerListDlg)
	DDX_Control(pDX, IDC_SERVER_LIST, m_ctrlSvrListBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(KServerListDlg, CDialog)
	//{{AFX_MSG_MAP(KServerListDlg)
	ON_BN_CLICKED(IDC_ADD_LIST, OnAddList)
	ON_BN_CLICKED(IDC_REMOVE_LIST, OnRemoveList)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDC_EDIT_LIST, OnEditList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KServerListDlg message handlers

void KServerListDlg::OnAddList( void )
{
    KAddServerListDlg dlg;

    if ( IDOK == dlg.DoModal() )
    {
        SServerInfo        svrInfo;
        std::wostringstream strmIPAddr;

        strmIPAddr << static_cast<DWORD>( dlg.m_dwField0 ) << L"."
                   << static_cast<DWORD>( dlg.m_dwField1 ) << L"."
                   << static_cast<DWORD>( dlg.m_dwField2 ) << L"."
                   << static_cast<DWORD>( dlg.m_dwField3 );

        wcscpy( svrInfo.m_strIP, strmIPAddr.str().c_str() );
        
        wcscpy( svrInfo.m_strName, dlg.m_cstrServerName );
        //wcsncpy( svrInfo.m_strName, dlg.m_cstrServerName, svrInfo.m_strName->size() );
        svrInfo.m_dwPort    = dlg.m_dwServerPort;
        svrInfo.m_iVersion  = dlg.m_iVersion;
        
        m_pMainFrm->m_vecSvrList.push_back( svrInfo );

        AddListBox( svrInfo );
    }
}

void KServerListDlg::OnEditList( void )
{
    if ( m_ctrlSvrListBox.GetSelCount() != 1 )
    {
        ::AfxMessageBox( _T("Please Choose only one server_list for edit.") );
        return;
    }

    KAddServerListDlg dlg;

    dlg.m_bEdit = true;
    m_ctrlSvrListBox.GetSelItems( 1, &dlg.m_iEditIdx );

    if ( IDOK == dlg.DoModal() )
    {
        SServerInfo        svrInfo;
        std::wostringstream strmIPAddr;

        strmIPAddr << static_cast<DWORD>( dlg.m_dwField0 ) << L"."
            << static_cast<DWORD>( dlg.m_dwField1 ) << L"."
            << static_cast<DWORD>( dlg.m_dwField2 ) << L"."
            << static_cast<DWORD>( dlg.m_dwField3 );

        wcscpy( svrInfo.m_strIP, strmIPAddr.str().c_str() );
        wcscpy( svrInfo.m_strName, dlg.m_cstrServerName );
        svrInfo.m_dwPort    = dlg.m_dwServerPort;
        svrInfo.m_iVersion  = dlg.m_iVersion;

        m_pMainFrm->m_vecSvrList[dlg.m_iEditIdx] = svrInfo;

        RefreshListBox();
    }
}

void KServerListDlg::OnRemoveList( void )
{
    int     cnt     = m_ctrlSvrListBox.GetSelCount();
    int*    pIdx    = new int[cnt];

    m_ctrlSvrListBox.GetSelItems( cnt, pIdx );

    for ( int i = cnt - 1; i >= 0; i-- )
    {
        m_ctrlSvrListBox.DeleteString( pIdx[i] );
        m_pMainFrm->m_vecSvrList.erase( m_pMainFrm->m_vecSvrList.begin() + pIdx[i] );
    }

    delete[] pIdx;
}

BOOL KServerListDlg::OnInitDialog( void )
{
	CDialog::OnInitDialog();

    RefreshListBox();

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void KServerListDlg::AddListBox( SServerInfo svrInfo )
{
    std::wostringstream strmSvrInfo;

    strmSvrInfo << svrInfo.m_strIP << L':'
                << svrInfo.m_dwPort << L"(ver"
                << svrInfo.m_iVersion << L"):"
                << svrInfo.m_strName;

    m_ctrlSvrListBox.InsertString( -1, strmSvrInfo.str().c_str() );
}

void KServerListDlg::RefreshListBox( void )
{
    m_ctrlSvrListBox.ResetContent();
    for ( size_t i = 0; i < m_pMainFrm->m_vecSvrList.size(); i++ )
    {
        AddListBox( m_pMainFrm->m_vecSvrList[i] );
    }
}

void KServerListDlg::OnOK( void )
{
    int                     cnt         = m_ctrlSvrListBox.GetSelCount();
    int*                    pIdx        = new int[cnt];
    CChildFrame*            pChildFrm   = NULL;
    CGCServerManagerView*   pChildView  = NULL;

    m_ctrlSvrListBox.GetSelItems( cnt, pIdx );

    for ( int i = 0; i < cnt; i++ )
    {
        m_pApp->OpenServer();
        pChildFrm = static_cast<CChildFrame*>( m_pMainFrm->GetActiveFrame() );
        ASSERT( NULL != pChildFrm );
        pChildView = static_cast<CGCServerManagerView*>( pChildFrm->GetActiveView() );
        ASSERT( NULL != pChildView );
        pChildView->ConnectServer( m_pMainFrm->m_vecSvrList[pIdx[i]] );
    }

   //운영자툴에서 더이상 아이템 박스를 사용하지 않음.
    if ( 0 < cnt )
    {
        //050627. microcat. 7번서버로 접속시 아이템 정보창이 뜨도록 한다.
        if ( wcscmp( pChildView->m_svrInfo.m_strName, L"items") == 0 )
        {
            pChildView->m_dlgAdminDBAccess.SetGCServerManagerView( pChildView );
            pChildView->m_dlgAdminDBAccess.DestroyWindow();
            pChildView->m_dlgAdminDBAccess.Create(IDD_DIALOG_ADMIN_DB_ACCESS, NULL);
            pChildView->m_dlgAdminDBAccess.ShowWindow(SW_SHOW);
        }
    }

    delete[] pIdx;

    CDialog::OnOK();
}