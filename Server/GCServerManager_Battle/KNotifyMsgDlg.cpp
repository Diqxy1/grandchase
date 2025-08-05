// KNotifyMsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KNotifyMsgDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KNotifyMsgDlg dialog


KNotifyMsgDlg::KNotifyMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KNotifyMsgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(KNotifyMsgDlg)
	m_cstrNotifyMsg = _T("");
	//}}AFX_DATA_INIT
    m_pMainFrm = NULL;
}


void KNotifyMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(KNotifyMsgDlg)
	DDX_Text(pDX, IDC_EDIT_NOTIFY, m_cstrNotifyMsg);

    DDX_Control(pDX, IDC_RADIO_ADMIN_CHAT_S, m_raNotice_Message_S[SBCM_ADMIN_CHAT]);
    DDX_Control(pDX, IDC_RADIO_SB_NOTICE_S, m_raNotice_Message_S[SBCM_NOTICE]);
    DDX_Control(pDX, IDC_RADIO_SB_SERVER_S, m_raNotice_Message_S[SBCM_SERVER]);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(KNotifyMsgDlg, CDialog)
	//{{AFX_MSG_MAP(KNotifyMsgDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KNotifyMsgDlg message handlers

BOOL KNotifyMsgDlg::OnInitDialog( void )
{
    CDialog::OnInitDialog();

    m_pMainFrm = static_cast<CMainFrame*>( ::AfxGetMainWnd() );

    m_raNotice_Message_S[SBCM_ADMIN_CHAT].SetCheck( 1 );

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void KNotifyMsgDlg::OnOK( void )
{
    ASSERT( NULL != m_pMainFrm );

    UpdateData( TRUE );

    int nMsgType = GetSBChatMessageTypeS();

    for ( size_t i = 0; i < m_pMainFrm->m_vecpChildView.size(); i++ )
    {
        CGCServerManagerView* pChildView = m_pMainFrm->m_vecpChildView[i];
        if ( CGCServerManagerView::US_CHANNEL == pChildView->m_eUsrStat )
        {
            for( int i = 0; i < NUM_OF_SBCM; ++i )
            {
                if( i == nMsgType )
                {
                    pChildView->m_channel.m_raNotice_Message[i].SetCheck( 1 );
                }
                else
                {
                    pChildView->m_channel.m_raNotice_Message[i].SetCheck( 0 );
                }
            }

            pChildView->m_channel.m_cstrNormalChat = m_cstrNotifyMsg;
            ::SendMessage( pChildView->m_hWnd, WM_COMMAND,
                           MAKEWPARAM( IDC_EDIT_NORMAL_CHAT, 0 ), 0 );
        }
    }

    m_cstrNotifyMsg.Empty();

    UpdateData( FALSE );
}

void KNotifyMsgDlg::OnCancel( void )
{
    m_pMainFrm->SendMessage( WM_COMMAND, MAKEWPARAM( KNotifyMsgDlg::ID_BTN_CLOSE, 0 ), 0 );
}

int KNotifyMsgDlg::GetSBChatMessageTypeS(void)
{
    if( false == m_raNotice_Message_S[0].GetSafeHwnd() )
        return -1;

    for( int i = 0; i < NUM_OF_SBCM; ++i )
    {
        if( 1 == m_raNotice_Message_S[i].GetCheck() )
        {
            return i;
        }
    }

    return -1;
}