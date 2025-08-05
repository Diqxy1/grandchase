// KRoomInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GCServerManager.h"
#include "GCServerManagerView.h"
#include "KRoomInfoDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KRoomInfoDlg dialog


KRoomInfoDlg::KRoomInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KRoomInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(KRoomInfoDlg)
	m_dwRoomID = 0;
	m_cstrRoomName = _T("");
	m_cstrPublic = _T("");
	m_cstrPasswd = _T("");
	m_cstrUserNum = _T("");
	m_cstrRoomStatus = _T("");
	//}}AFX_DATA_INIT
    m_iIdxInfo = 0;
    m_pChildView = static_cast<CGCServerManagerView*>( pParent );
}


void KRoomInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(KRoomInfoDlg)
	DDX_Text(pDX, IDC_EDIT_ROOM_ID, m_dwRoomID);
	DDX_Text(pDX, IDC_EDIT_ROOM_NAME, m_cstrRoomName);
	DDX_Text(pDX, IDC_EDIT_PUBLIC, m_cstrPublic);
	DDX_Text(pDX, IDC_EDIT_PASSWD, m_cstrPasswd);
	DDX_Text(pDX, IDC_EDIT_USER_NUM, m_cstrUserNum);
	DDX_Text(pDX, IDC_EDIT_ROOM_STATUS, m_cstrRoomStatus);
    DDX_Control(pDX, IDC_LIST_ROOM_MEMBERS, m_ctrlRoomMembers);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(KRoomInfoDlg, CDialog)
	//{{AFX_MSG_MAP(KRoomInfoDlg)
    ON_LBN_DBLCLK(IDC_LIST_ROOM_MEMBERS, OnDblclkRoomMembers)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KRoomInfoDlg message handlers

BOOL KRoomInfoDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    const KSimpleRoomInfo& roomInfo = m_pChildView->m_vecRoomInfo[m_iIdxInfo];

    //{{ tslayer. 방원 목록 보기
    std::map<std::wstring, char>::const_iterator mit;
    for( mit = roomInfo.m_mapRoomMember.begin() ; mit != roomInfo.m_mapRoomMember.end() ; ++mit )
    {
        std::wostringstream strmRoomMember;
        strmRoomMember<< mit->first; // 닉네임
        m_ctrlRoomMembers.InsertString( -1, strmRoomMember.str().c_str() );
    }
    //}}

    m_dwRoomID      = roomInfo.m_usRoomID + 1;
    m_cstrRoomName  = roomInfo.m_strRoomName.c_str();
    m_cstrPublic    = ( true == roomInfo.m_bPublic ? L"public" : L"private" );
    m_cstrPasswd    = roomInfo.m_strRoomPasswd.c_str();
    m_cstrUserNum.Format( L"%d/%d", roomInfo.m_usUsers, roomInfo.m_usMaxUsers );
    if ( true == roomInfo.m_bPlaying )
    {
        m_cstrRoomStatus = L"Playing";
    }
    else if ( roomInfo.m_usUsers >= roomInfo.m_usMaxUsers )
    {
        m_cstrRoomStatus = L"Full";
    }
    else
    {
        m_cstrRoomStatus = L"Waiting";
    }

    UpdateData( FALSE );

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void KRoomInfoDlg::OnDblclkRoomMembers( void )
{
    CString cstrNickName;

    m_ctrlRoomMembers.GetText( m_ctrlRoomMembers.GetCurSel(), cstrNickName );

    if ( TRUE == cstrNickName.IsEmpty() ) return;

    std::wstring strLogin;
    strLogin.clear();

    for ( int i = 0; i < cstrNickName.GetLength(); i++ )
    {
        strLogin += cstrNickName[i];
    }

    // 모두 닉네임
    m_pChildView->m_server.Send_UserDBInfoReq( m_pChildView->m_strWideServerName.c_str(), strLogin, true );
    m_pChildView->m_pMainFrm->m_logFile << L"[request] " << strLogin << L" It queries user's DB information." << std::endl;
}