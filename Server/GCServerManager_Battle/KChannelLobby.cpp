// KChannelLobby.cpp : implementation file
//

#include "stdafx.h"
#include "gcservermanager.h"
#include "KChannelLobby.h"
#include ".\kchannellobby.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KChannelLobby dialog


KChannelLobby::KChannelLobby(CWnd* pParent /*=NULL*/)
	: cdxCDynamicDialog(KChannelLobby::IDD, pParent)
{
	//{{AFX_DATA_INIT(KChannelLobby)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void KChannelLobby::DoDataExchange(CDataExchange* pDX)
{
	cdxCDynamicDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(KChannelLobby)
	DDX_Control(pDX, IDC_CHANNEL_LIST, m_ctrlChannelList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(KChannelLobby, cdxCDynamicDialog)
	//{{AFX_MSG_MAP(KChannelLobby)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_LBN_DBLCLK(IDC_CHANNEL_LIST, OnDblclkChannelList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KChannelLobby message handlers

void KChannelLobby::OnOk() 
{
	::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, MAKEWPARAM( IDOK, 0 ), 0 );
}

void KChannelLobby::OnCancel() 
{
    ::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, MAKEWPARAM( IDCANCEL, 0 ), 0 );
}

void KChannelLobby::OnDblclkChannelList()
{
    ::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, MAKEWPARAM( IDOK, 0 ), 0 );
}

BOOL KChannelLobby::OnInitDialog()
{
    cdxCDynamicDialog::OnInitDialog();

    CRect rt;
    GetClientRect( &rt );

    m_szMin.SetSize( rt.Width(), rt.Height() );

    // TODO:  여기에 추가 초기화 작업을 추가합니다.
    AddSzControl( ::GetDlgItem(*this,IDOK),mdRepos,mdRepos );
    AddSzControl( ::GetDlgItem(*this,IDCANCEL),mdRepos,mdRepos );
    AddSzControl( m_ctrlChannelList,mdResize,mdResize );


    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
