// KAddServerListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "gcservermanager.h"
#include "KAddServerListDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KAddServerListDlg dialog


KAddServerListDlg::KAddServerListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KAddServerListDlg::IDD, pParent)
    , m_cstrServerName(_T(""))
{
	//{{AFX_DATA_INIT(KAddServerListDlg)
	m_dwServerPort = 0;
	m_iVersion = 0;
	//}}AFX_DATA_INIT
    m_bEdit    = false;
    m_iEditIdx = 0;
    m_pMainFrm = static_cast<CMainFrame*>( ::AfxGetMainWnd() );
}


void KAddServerListDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(KAddServerListDlg)
    DDX_Control(pDX, IDC_SERVER_IPADDRESS, m_ctrlServerIP);
    DDX_Text(pDX, IDC_SERVER_PORT, m_dwServerPort);
    DDX_Text(pDX, IDC_SERVER_VERSION, m_iVersion);
    DDX_Text(pDX, IDC_SERVER_Name, m_cstrServerName);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(KAddServerListDlg, CDialog)
	//{{AFX_MSG_MAP(KAddServerListDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KAddServerListDlg message handlers

void KAddServerListDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

    BYTE field0, field1, field2, field3;
    m_ctrlServerIP.GetAddress( field0, field1, field2, field3 );

    m_dwField0 = field0;
    m_dwField1 = field1;
    m_dwField2 = field2;
    m_dwField3 = field3;

    CDialog::OnOK();
}

BOOL KAddServerListDlg::OnInitDialog( void )
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    if ( true == m_bEdit && m_iEditIdx < (int)m_pMainFrm->m_vecSvrList.size() )
    {
        SetWindowText( _T("Edit Server List") );

        std::wstring strIP = m_pMainFrm->m_vecSvrList[m_iEditIdx].m_strIP;
        BYTE field[4] = { 0, };

        for ( int i = 0; i < 4; i++ )
        {
            std::wstring strField;
            std::wstring::size_type dot = strIP.find( L"." );

            if ( std::wstring::npos == dot )
            {
                strField = strIP;
            }
            else
            {
                strField = strIP.substr( 0, dot );
                strIP.erase( 0, dot + 1 );
            }
            field[i] = static_cast<BYTE>( ::_wtoi( strField.c_str() ) );
        }

        m_ctrlServerIP.SetAddress( field[0], field[1], field[2], field[3] );
        m_dwServerPort      = m_pMainFrm->m_vecSvrList[m_iEditIdx].m_dwPort;
        m_iVersion          = m_pMainFrm->m_vecSvrList[m_iEditIdx].m_iVersion;
        m_cstrServerName    = m_pMainFrm->m_vecSvrList[m_iEditIdx].m_strName;

        UpdateData( FALSE );
    }

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
