// GCServerManagerView.cpp : implementation of the CGCServerManagerView class
//

#pragma warning(disable:4786)

#include "stdafx.h"
#include "MainFrm.h"
#include "GCServerManager.h"
#include "GCServerManagerDoc.h"
#include "GCServerManagerView.h"
#include "ChildFrm.h"
#include "KChangeDBInfoDlg.h"
#include "KReasonDlg.h"
#include <sstream>
#include <iomanip>
#include "NetError.h"
#include "ChannelType.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static KSerializer ks;

#define LIMIT_NOTICE_LENGTH 400
/////////////////////////////////////////////////////////////////////////////
// CGCServerManagerView

IMPLEMENT_DYNCREATE(CGCServerManagerView, CView)

BEGIN_MESSAGE_MAP(CGCServerManagerView, CView)
	//{{AFX_MSG_MAP(CGCServerManagerView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
    ON_COMMAND(IDOK, OnEnterBtn)
    ON_COMMAND(IDCANCEL, OnExitBtn)
    ON_COMMAND(KChannel::ID_BTN_CHANNEL_EXIT, OnLeaveChannel)
    //ON_COMMAND(IDC_EDIT_ADMIN_CHAT, OnEditAdminChat)
    ON_COMMAND(IDC_EDIT_NORMAL_CHAT, OnEditNoticeMsg)
    ON_MESSAGE(WM_APP, OnServerMsgRecv)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGCServerManagerView construction/destruction

CGCServerManagerView::CGCServerManagerView()
{
	// TODO: add construction code here
    m_eUsrStat = US_INIT; // 초기 상태로 시작한다.
    m_pMainFrm = static_cast<CMainFrame*>( ::AfxGetMainWnd() );
    m_bEndWait = false;
    m_bError   = false;
	m_bScrollLock = false;
}

CGCServerManagerView::~CGCServerManagerView()
{
}

BOOL CGCServerManagerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGCServerManagerView drawing

void CGCServerManagerView::OnDraw(CDC* pDC)
{
	CGCServerManagerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CGCServerManagerView diagnostics

#ifdef _DEBUG
void CGCServerManagerView::AssertValid() const
{
	CView::AssertValid();
}

void CGCServerManagerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGCServerManagerDoc* CGCServerManagerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGCServerManagerDoc)));
	return (CGCServerManagerDoc*)m_pDocument;
}
#endif //_DEBUG

CGCServerManagerView::EWaitResult CGCServerManagerView::WaitVerifyAccountAck( DWORD dwMaxTime )
{
    MSG     msg;
    DWORD   dwTime = ::GetTickCount();

    while ( TRUE == ::GetMessage( &msg, NULL, 0, 0 ) )
    {
        ::TranslateMessage( &msg );
        ::DispatchMessage( &msg );

        if ( true == m_bEndWait )
        {
            if ( true == m_bError )
            {
                m_bError = false;
                return FAIL;
            }
            else
            {
                return SUCCESS;
            }
        }
        if ( ::GetTickCount() - dwTime >= dwMaxTime ) break;
    }

    return TIME_OVER;
}

std::string CGCServerManagerView::GetAuthLevel( char cAuthLevel )
{
    std::string strAuthLevel;

    switch ( cAuthLevel )
    {
        case USER_STOP:         strAuthLevel = "Blocked User";  break;
        case USER_BAD:          strAuthLevel = "Bad User";      break;
        case USER_NORMAL:       strAuthLevel = "Normal User";   break;
        case USER_CHALLENGER:   strAuthLevel = "Challenger";    break;
        case USER_SUPERIOR_MC:  strAuthLevel = "Superior(MC)";  break;
        case USER_ADMIN:        strAuthLevel = "Administrator"; break;
    }

    return strAuthLevel;
}

void CGCServerManagerView::UpdateRoomInfoList( void )
{
    // 방 정보 리스트를 업데이트한다.
    m_vecRoomInfo = m_vecRoomInfoUpdate;

    // 방 정보 리스트박스를 클리어한다.
    m_channel.m_ctrlRoomList.ResetContent();

    // 방 정보 리스트박스에 리스트를 표시한다.
    std::vector<KSimpleRoomInfo>::const_iterator vcit;
    for ( vcit = m_vecRoomInfo.begin(); vcit != m_vecRoomInfo.end(); vcit++ )
    {
        std::wstring strRoomState( true == vcit->m_bPlaying ? L"(P)" :
                                  ( vcit->m_usUsers >= vcit->m_usMaxUsers ? L"(F)" : L"(W)" ) );
        std::wstring strPublic( true == vcit->m_bPublic ? L"[Public]" : L"[Private]" );
        std::wostringstream strmRoomInfo;
        strmRoomInfo << strRoomState << strPublic
                     << L'[' << vcit->m_usRoomID + 1 << L']' << vcit->m_strRoomName
                     << L'(' << vcit->m_usUsers << L'/' << vcit->m_usMaxUsers << L')';
        m_channel.m_ctrlRoomList.InsertString( -1, strmRoomInfo.str().c_str() );
    }
}

void CGCServerManagerView::UpdateUserInfoList( void )
{
    // 유저 정보 리스트를 업데이트한다.
    m_vecUsrInfo = m_vecUsrInfoUpdate;

    // 유저 정보 리스트박스를 클리어한다.
    m_channel.m_ctrlUserList.ResetContent();

    // 유저 정보 리스트박스에 리스트를 표시한다.
    std::vector<KInviteUserInfo>::const_iterator vcit;
    for ( vcit = m_vecUsrInfo.begin(); vcit != m_vecUsrInfo.end(); vcit++ )
    {/*vcit->m_cBestCharGrade
    vcit->m_cGrade
    vcit->m_dwUserUID
    vcit->m_strNickName*/
        //std::wstring strUserState( 1 == vcit->m_cState ? L"(L)" :
        //                            ( 2 == vcit->m_cState ? L"(P)" : L"(W)" ) );
        //std::wstring strZender( true == vcit->m_bMale ? L"[M]" : L"[F]" );
        std::wostringstream strmUserInfo;
        //strmUserInfo << strUserState << strZender << vcit->m_strNickName << L"(" << vcit->m_strLogin << L")";
        strmUserInfo << vcit->m_strNickName << L"(" << vcit->m_dwUserUID << L")";
        m_channel.m_ctrlUserList.InsertString( -1, strmUserInfo.str().c_str() );
    }
}

void CGCServerManagerView::SetUserState( EUserState eState )
{
    m_eUsrStat = eState;
}

void CGCServerManagerView::RestoreSize( void )
{
    // 클라이언트 영역의 다이얼로그 박스들의 크기를 재조정한다.
    CRect rc;
    GetClientRect( &rc );
    SendMessage( WM_SIZE, SIZE_RESTORED, MAKELPARAM( rc.right, rc.bottom ) );
}

void CGCServerManagerView::ConnectServer( const SServerInfo& svrInfo )
{
    // 서버 창을 등록한다.
    m_pMainFrm->m_vecpChildView.push_back( this );

    // 서버 정보를 등록한다.
    m_svrInfo = svrInfo;

    // 서버 이름을 등록한다.
    std::wostringstream strmSvrInfo;
    strmSvrInfo << svrInfo.m_strIP << L':'
                << svrInfo.m_dwPort << L"(ver"
                << svrInfo.m_iVersion << L"):"
                << svrInfo.m_strName;
    m_strWideServerName = strmSvrInfo.str();
    m_strServerName = KncUtil::toNarrowString( m_strWideServerName );

    // 서버 이름을 타이틀로 한다.
    GetDocument()->SetTitle( m_strWideServerName.c_str() );

    m_strLogin  = m_pMainFrm->m_strLogin;
    m_strPasswd = m_pMainFrm->m_strPasswd;
    KEVENT_GMTOOL_AUTHENTICATION_REQ kReq;
    kReq.m_nProtocolVer = m_svrInfo.m_iVersion;
    kReq.m_strLogin     = m_strLogin;
    kReq.m_strPasswd    = m_strPasswd;

    // 서버 스타트업.
    if ( false == m_server.Init( m_hWnd ) )
    {
        ::AfxMessageBox( _T("Startup failed.") );
        GetParent()->SendMessage( WM_CLOSE );
        return; // fail
    }

    // 서버 연결.
    KServerInfo kServerInfo;
    kServerInfo.m_strIP = KncUtil::toNarrowString(m_svrInfo.m_strIP);
    kServerInfo.m_usPort = (USHORT)m_svrInfo.m_dwPort;
    int nRet = m_server.ConnectForGMTool( kServerInfo, kReq );
    if ( 0 != nRet )
    {
        ::AfxMessageBox( _T("ConnectChannel failed.") );
        GetParent()->SendMessage( WM_CLOSE );
        return; // fail
    }

    // 로그인 상태로 진입한다.
    SetUserState( US_LOGINING );

    

    //// 계정 확인을 요청한다.
    //if ( false == m_server.Send_VerifyAccountReq( m_strServerName.c_str(),
    //                                              KncUtil::toNarrowString(m_strLogin).c_str(),
    //                                              KncUtil::toNarrowString(m_strPasswd).c_str(),
    //                                              true,
    //                                              m_svrInfo.m_iVersion,
    //                                              0, //앗쌀.. 하드코딩 ㅠㅠ
    //                                              0x00000000) ) //앗쌀.. 하드코딩 ㅠㅠ
    //{
    //    ::AfxMessageBox( _T("Send_VerifyAccountReq failed.") );
    //    GetParent()->SendMessage( WM_CLOSE );
    //    return; // fail
    //}

    switch ( WaitVerifyAccountAck( 10000 ) ) //godsend 5000-> 10000으로 고침 
    {
        case FAIL:
        {
            GetParent()->SendMessage( WM_CLOSE );
            break;
        }
        case TIME_OVER:
        {
            ::AfxMessageBox( _T("Time out.") );
            GetParent()->SendMessage( WM_CLOSE );
            break;
        }
    }
}

void CGCServerManagerView::CloseServer( void )
{
    // 서버 창 등록을 해제한다.
    std::vector<CGCServerManagerView*>::iterator itr;
    for ( itr = m_pMainFrm->m_vecpChildView.begin(); itr != m_pMainFrm->m_vecpChildView.end(); itr++ )
    {
        if ( this == *itr )
        {
            m_pMainFrm->m_vecpChildView.erase( itr );
            break;
        }
    }

    // 서버를 닫는다.
    //m_server.ClosePeer( m_strServerName.c_str() );
    ///m_server.m_pkUserProxy->ShutDown();
}

/////////////////////////////////////////////////////////////////////////////
// CGCServerManagerView message handlers

void CGCServerManagerView::OnEnterBtn( void )
{
    // 선택한 채널로 들어간다.
    int idx = m_channelLobby.m_ctrlChannelList.GetCurSel();
    if ( LB_ERR != idx )
    {
        m_server.Send_EnterChannelReq( m_strWideServerName.c_str(),
                                        m_vecChannelList[idx].m_dwChannelUID
                                       /*m_vecChannelList[idx].m_strChannelName*/ );

                                       
        m_channel.SetChannelType( m_vecChannelList[idx].m_cType ); // 게임모드를 넣어야 한다. gcenum에 있는 0,5, 7~27

        switch ( WaitVerifyAccountAck( 5000 ) )
        {
            case FAIL:
            {
                GetParent()->SendMessage( WM_CLOSE );
                break;
            }
            case TIME_OVER:
            {
                ::AfxMessageBox( _T("Time out.") );
                GetParent()->SendMessage( WM_CLOSE );
                break;
            }
        }
    }
}

void CGCServerManagerView::OnExitBtn( void )
{
    // 윈도우를 닫는다.
    static_cast<CChildFrame*>( GetParent() )->SendMessage( WM_CLOSE );
}

void CGCServerManagerView::OnLeaveChannel( void )
{
    // 채널에서 빠져 나온 것을 서버에게 알린다.
    m_server.Send_LeaveChannelNot( m_strServerName.c_str() );

    // 채널 로비 화면으로 전환한다.
    SetUserState( US_CHANNELLOBBY ); // 채널 로비 상태로 복귀한다.
    m_channel.DestroyWindow();
    m_channelLobby.Create( IDD_CHANNEL_LOBBY, this );
    RestoreSize();

    // 채널 정보 리스트를 요청한다.
    if ( false == m_server.Send_ChannelInfoListReq( m_strServerName.c_str() ) )
    {
        ::AfxMessageBox( _T("Send_ChannelInfoListReq failed.") );
    }
}

void CGCServerManagerView::OnEditAdminChat( void )
{
    // 운영자 메세지를 서버에게 보내고 에디트 컨트롤을 클리어한다.
//     std::wostringstream strmChatMsg;
//     
//     std::wstring strAdminChat( m_channel.m_cstrNormalChat );
//     strmChatMsg << std::setiosflags( std::ios::left )
//                 << std::setw( 10 ) << std::setfill( L' ' )
//                 << L"Broadcasting" << L':' << strAdminChat;
//     m_server.Send_NotifyMessageNot( m_strWideServerName.c_str(), strmChatMsg.str() );
//     
//     m_channel.m_ctrlEditAdminChat.SetWindowText( _T("") );
//     m_channel.m_cstrNormalChat.Empty();
}

void CGCServerManagerView::OnEditNoticeMsg( void )
{
    // 메세지를 서버에게 보내고 에디트 컨트롤을 클리어한다.
    std::wostringstream strmChatMsg;

    if ( m_channel.m_cstrNormalChat.GetLength() > LIMIT_NOTICE_LENGTH ) {
        AfxMessageBox( _T("Message is too long, can't send it") );
        return;
    }

    switch( m_channel.GetSBChatMessageType() )
    {
    case KChannel::SBCM_ADMIN_CHAT:
        {
            std::wstring strAdminChat( m_channel.m_cstrNormalChat );
            strmChatMsg << std::setiosflags( std::ios::left )
                << std::setw( 10 ) << std::setfill( L' ' )
                << strAdminChat;
            m_server.Send_NotifyMessageNot( m_strWideServerName.c_str(), strmChatMsg.str() );
        }
        break;

    case KChannel::SBCM_NOTICE:
        {
            strmChatMsg << (LPCWSTR)m_channel.m_cstrNormalChat;
            m_server.Send_SignBoard_Notice( strmChatMsg.str(), KSignBoardData::SBT_SQUARE_ADMIN );
        }
        break;

    case KChannel::SBCM_SERVER:
        {
            strmChatMsg << (LPCWSTR)m_channel.m_cstrNormalChat;
            m_server.Send_SignBoard_Notice( strmChatMsg.str(), KSignBoardData::SBT_SERVER_ADMIN );
        }
        break;

    case KChannel::SBCM_NORMAL_CHAT:
        {
            strmChatMsg << (LPCWSTR)m_channel.m_cstrNormalChat;
            m_server.Send_ChatReq( m_strWideServerName.c_str(), strmChatMsg.str().c_str() );
        }
        break;
    }

    m_channel.m_ctrlEditNormalChat.SetWindowText( _T("") );
    m_channel.m_cstrNormalChat.Empty();
}

int CGCServerManagerView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // 채널 로비 화면을 띄운다.

    m_channelLobby.Create( IDD_CHANNEL_LOBBY, this );

    return 0;
}

#define CALL_HANDLER( id, func ) case KUserEvent::id: return func( wParam, lParam );

LRESULT CGCServerManagerView::OnServerMsgRecv( WPARAM wParam, LPARAM lParam )
{
    switch( wParam ) {
        CALL_HANDLER( EVENT_VERIFY_ACCOUNT_ACK, OnServerVerifyAccountAck)
        CALL_HANDLER( EVENT_GMTOOL_AUTHENTICATION_ACK, OnServerGMToolAuthenticationAck)
        CALL_HANDLER( EVENT_CHANNEL_LIST_ACK, OnServerChannelInfoListAck)
        CALL_HANDLER( EVENT_ENTER_CHANNEL_ACK, OnServerEnterChannelAck)
        CALL_HANDLER( EVENT_ROOM_LIST_ACK, OnServerRoomListAck)
        CALL_HANDLER( EVENT_GMTOOL_USER_LIST_ACK, OnServerUserListNot)
        CALL_HANDLER( EVENT_CHAT_NOT, OnServerChatNot)
        CALL_HANDLER( EVENT_CREATE_ROOM_ACK, OnServerCreateRoomAck)
        CALL_HANDLER( EVENT_JOIN_ROOM_ACK, OnServerJoinRoomAck)
        CALL_HANDLER( EVENT_KICK_USER_BY_ADMIN_ACK, OnServerKickUserAck)
        //CALL_HANDLER( EVENT_CHANGE_USER_INFO_ACK, OnServerChangeUserDBInfoAck)
        CALL_HANDLER( EVENT_QUERY_USER_INFO2_ACK, OnServerUserDBInfoAck)
        CALL_HANDLER( EVENT_REMOVE_USER_INFO_ACK, OnServerDropUserDBInfoAck)
        CALL_HANDLER( EVENT_CLOSE_CONNECTION_NOT, OnServerCloseConnectionNot)

        CALL_HANDLER( EVENT_QUERY_NORMAL_ITEM_LIST_ACK, OnSelectNormalItemAck )
        CALL_HANDLER( EVENT_QUERY_SALED_NORMAL_ITEM_LIST_ACK, OnSelectDelNormalItemAck )
        CALL_HANDLER( EVENT_QUERY_DURATION_ITEM_LIST_ACK, OnSelectDurationItemAck )
        CALL_HANDLER( EVENT_QUERY_SALED_DURATION_ITEM_LIST_ACK, OnSelectDelDurationItemAck )
        CALL_HANDLER( EVENT_ADD_NORMAL_ITEM_GP_ACK, OnInsertGPNormalItemAck )
        CALL_HANDLER( EVENT_ADD_DURATION_ITEM_GP_ACK, OnInsertGPDurationItemAck )
        CALL_HANDLER( EVENT_ADD_NORMAL_ITEM_CASH_ACK, OnInsertCashNormalItemAck )
        CALL_HANDLER( EVENT_ADD_DURATION_ITEM_CASH_ACK, OnInsertCashDurationItemAck )
        CALL_HANDLER( EVENT_REMOVE_ITEM_ACK, OnDelItemAck )
        CALL_HANDLER( EVENT_QUERY_USER_INFO_ACK, OnSelectUserDataAck )
        CALL_HANDLER( EVENT_CHANGE_STRONG_ITEM_ACK, OnSelectChangeStrongItemListAck)
        //CALL_HANDLER(WM_APP + KUserEvent::EVENT_QUERY_ITEM_INFO_ACK, OnSelectAdminUseItemInfoDataAck )

        CALL_HANDLER( EVENT_SQUARE_LIST_ACK, OnServerSquareListAck )
        CALL_HANDLER( EVENT_GMTOOL_SQUARE_USER_LIST_ACK, OnServerSquareUserListAck )
        CALL_HANDLER( EVENT_USER_CHAT_CHANGE_BY_ADMIN_ACK, OnServerUserChatChangeAck )
    }

    return 0;
}

LRESULT CGCServerManagerView::OnServerVerifyAccountAck( WPARAM wParam, LPARAM lParam )
{
    ACK_DESERIALIZE( KEVENT_VERIFY_ACCOUNT_ACK );
    SAFE_DELETE( pkBuff );

    ::AfxMessageBox( _T("Internal Error! Wrong Authenticate Processing."), MB_ICONINFORMATION );
    return 0;
}

LRESULT CGCServerManagerView::OnServerGMToolAuthenticationAck( WPARAM wParam, LPARAM lParam )
{
    //KPacketVerifyAccountAck* pkAck = reinterpret_cast<KPacketVerifyAccountAck*>( lParam );
    ACK_DESERIALIZE( KEVENT_GMTOOL_AUTHENTICATION_ACK );

    if( kRecv.m_nOK != NetError::NET_OK )
    {
        ::AfxMessageBox( NetError::GetErrStr( kRecv.m_nOK ), MB_ICONERROR );
        m_bError = true;
    }
    else
    {
        // 채널 로비 상태로 진입한다.
        SetUserState( US_CHANNELLOBBY );
        if ( false == m_server.Send_ChannelInfoListReq( m_strServerName.c_str() ) )
        {
            ::AfxMessageBox( _T("Send_ChannelInfoListReq failed.") );
        }
        m_strNickname = kRecv.m_strNickName;
        m_strLogin = kRecv.m_strLogin;
    }

    m_bEndWait = true;

    SAFE_DELETE( pkBuff );

    return 0;
}



LRESULT CGCServerManagerView::OnServerChannelInfoListAck( WPARAM wParam, LPARAM lParam )
{
    //KPacketChannelInfoListAck* pkAck = reinterpret_cast<KPacketChannelInfoListAck*>( lParam );
    ACK_DESERIALIZE( KEVENT_CHANNEL_LIST_ACK );
    SAFE_DELETE( pkBuff );

    if( kRecv.m_nOK == 0 )
    {
        std::vector<KChannelInfo> kChannelList;
        kRecv.m_buffCompChannelList.UnCompress();
        ks.BeginReading( &kRecv.m_buffCompChannelList );
        ks.Get( kChannelList );
        ks.EndReading();

        m_vecChannelList = kChannelList; //kRecv.m_vecChannelInfo;
        // 채널 정보 리스트를 업데이트한다.
    }

    // 채널 정보 리스트박스를 클리어한다.
    m_channelLobby.m_ctrlChannelList.ResetContent();

    // 채널 정보 리스트박스에 리스트를 표시한다.
    std::vector<KChannelInfo>::const_iterator vcit;
    for ( vcit = m_vecChannelList.begin(); vcit != m_vecChannelList.end(); vcit++ )
    {
        std::wstring         strChannelName;
        std::wostringstream  strmInfo;

        switch ( vcit->m_cType )
        {
        case CT_DUNGEON:
            strChannelName.append(L"[CT_DUNGEON]");
            break;
        case CT_UNLIMITED:
            strChannelName.append(L"[CT_UNLIMITED]");
            break;
        case CT_BEGINNER:
            strChannelName.append(L"[CT_BEGINNER]");
            break;
        case CT_ROOKIE:
            strChannelName.append(L"[CT_ROOKIE]");
            break;
        case CT_EXPERT:
            strChannelName.append(L"[CT_EXPERT]");
            break;
        case CT_MASTER:
            strChannelName.append(L"[CT_MASTER]");
            break;
        //case CT_GUILD:
        //    strChannelName.append(L"[CT_GUILD]");
            break;
        case CT_INDIGO:
            strChannelName.append(L"[CT_INDIGO]");
            break;
        //case CT_QZERO:
        //    strChannelName.append(L"[CT_QZERO]");
            break;
        }
        strChannelName.append( vcit->m_strChannelName );
        strmInfo << strChannelName << L'(' << vcit->m_usUsers << L'/' << vcit->m_usMaxUsers << L')';
        m_channelLobby.m_ctrlChannelList.InsertString( -1, strmInfo.str().c_str() );
    }


    return 0;
}

LRESULT CGCServerManagerView::OnServerEnterChannelAck( WPARAM wParam, LPARAM lParam )
{
    //KPacketEnterChannelAck* pkAck = reinterpret_cast<KPacketEnterChannelAck*>( lParam );
    ACK_DESERIALIZE( KEVENT_ENTER_CHANNEL_ACK );

    if ( 0 == kRecv.m_nOK )
    {
        // 채널 화면으로 전환한다.
        SetUserState( US_CHANNEL ); // 채널 상태로 진입한다.
        m_channelLobby.DestroyWindow();
        m_channel.Create( IDD_CHANNEL, this );
        m_channel.SetChannelType( kRecv.m_cType );
        RestoreSize();

        // 방 정보 리스트를 요청한다.
        if ( false == m_server.Send_RoomListReq( m_strServerName.c_str(), m_channel.GetGameMode(), m_channel.GetRoomListPage() ) )
        {
            ::AfxMessageBox( _T("failed to Send_RoomListReq.") );
        }

        // 유저 정보 리스트를 요청한다.
        if ( false == m_server.Send_UserListReq( m_strServerName.c_str(), m_channel.GetUserListPage() ) ) //  2008112 woosh  room 상태가 아니라는 에러가 서버에서 뜬다.
        {
            ::AfxMessageBox( _T("failed to Send_UserListReq fail.") );
        }

        // 광장 정보 리스트를 요청한다.
        if ( false == m_server.Send_SquareListReq() )
        {
            ::AfxMessageBox( _T("failed to Send_SquareListReq fail.") );
        }

    }
    else
    {
        m_bError   = true;

        ::AfxMessageBox( _T("Channel is full.") );
    }

    m_bEndWait = true;

    SAFE_DELETE( pkBuff );
    //SAFE_DELETE( pkAck );

    return 0;
}

LRESULT CGCServerManagerView::OnServerRoomListAck( WPARAM wParam, LPARAM lParam )
{
    //KPacketRoomListNot* pkNot = reinterpret_cast<KPacketRoomListNot*>( lParam );
    ACK_DESERIALIZE( KPageInfo );
    SAFE_DELETE( pkBuff );

    std::vector<KSimpleRoomInfo> kPacket;
    kRecv.m_buffCompList.UnCompress();
    ks.BeginReading( &kRecv.m_buffCompList );
    ks.Get( kPacket );
    ks.EndReading();

    m_channel.SetRoomPageSize( kRecv.m_nMaxPage ); // 최대 페이지 ( 방 개수에 따라 )
    m_channel.SetRoomListPage( kRecv.m_nCurrentPage ); // 현재 페이지 ( 정상이라면 보낸 페이지가 다시 올 것 )

    m_vecRoomInfoUpdate = kPacket; // kRecv.m_vecRoomInfo;
    
    m_channel.MakeRoomPage(); // 룸 리스트 페이지 번호 구성하기

    UpdateRoomInfoList();
    
    return 0;
}

LRESULT CGCServerManagerView::OnServerUserListNot( WPARAM wParam, LPARAM lParam )
{
    //KPacketUserListNot* pkNot = reinterpret_cast<KPacketUserListNot*>( lParam );
    ACK_DESERIALIZE( KPageInfo );
    SAFE_DELETE( pkBuff );

    if( kRecv.m_nOK == 4 )
    {
        AfxMessageBox( L"잠시 후에 시도해주세요" );
    }
    
    if( kRecv.m_nMaxPage > 300 ) // 어떻게 정하지..
        kRecv.m_nMaxPage = 0;

    m_channel.SetUserListPageSize( kRecv.m_nMaxPage ); // 최대 페이지 ( 방 개수에 따라 )
    m_channel.SetUserListPage( kRecv.m_nCurrentPage );

    kRecv.m_buffCompList.UnCompress();

    std::vector<KInviteUserInfo> kPacket; // KUserInfo -> KInviteUserInfo
    ks.BeginReading( &kRecv.m_buffCompList );
    ks.Get( kPacket );
    ks.EndReading();
    //길드는 EVENT_GUILD_ZONE_USER_LIST_NOT 이걸로 온다..

    m_vecUsrInfoUpdate.clear();
    m_vecUsrInfoUpdate = kPacket;

    std::sort( m_vecUsrInfoUpdate.begin(), m_vecUsrInfoUpdate.end(), KUserInfoLess() );

    m_channel.MakeUserListPage(); // 페이지 구성하기

    UpdateUserInfoList();

    return 0;
}

LRESULT CGCServerManagerView::OnServerChatNot( WPARAM wParam, LPARAM lParam )
{
    //KPacketChatNot* pkNot = reinterpret_cast<KPacketChatNot*>( lParam );
    ACK_DESERIALIZE( KChatData );

	int nScrollLimit = 0;
	int nScrollPos = 0;

	nScrollLimit = m_channel.m_ctrlChatList.GetScrollLimit( SB_VERT );
	nScrollPos = m_channel.m_ctrlChatList.GetScrollPos( SB_VERT );

	if ( nScrollPos < nScrollLimit ) {
		m_bScrollLock = true;
	}
	else {
		m_bScrollLock = false;
	}

    // 채팅 리스트박스에 일반 채팅 메세지를 표시한다.
    std::wstring strMsg = kRecv.m_strSenderNick + L" : " + kRecv.m_strChatMsg;

        m_channel.m_ctrlChatList.InsertString( -1, strMsg.c_str() );
	if ( false == m_bScrollLock ) {
        m_channel.m_ctrlChatList.SetCurSel( m_channel.m_ctrlChatList.GetCount() - 1 );
        m_channel.m_ctrlChatList.SetCurSel( -1 );
    }

    SAFE_DELETE( pkBuff );
    //SAFE_DELETE( pkNot );

    return 0;
}

LRESULT CGCServerManagerView::OnServerCreateRoomAck( WPARAM wParam, LPARAM lParam )
{
    //KPacketCreateRoomAck* pkAck = reinterpret_cast<KPacketCreateRoomAck*>( lParam );
    ACK_DESERIALIZE( KEVENT_CREATE_ROOM_ACK );

    // 실제 게임에 들어 가지 않으므로 사용되지 않음.
    // 훗날을 위해 예약해둠.

    SAFE_DELETE( pkBuff );
    //SAFE_DELETE( pkAck );

    return 0;
}

LRESULT CGCServerManagerView::OnServerJoinRoomAck( WPARAM wParam, LPARAM lParam )
{
    //KPacketJoinRoomAck* pkAck = reinterpret_cast<KPacketJoinRoomAck*>( lParam );
    //ACK_DESERIALIZE( KEVENT_JOIN_ROOM_ACK );

    // 실제 게임에 들어 가지 않으므로 사용되지 않음.
    // 훗날을 위해 예약해둠.

    //SAFE_DELETE( pkBuff );
    //SAFE_DELETE( pkAck );

    return 0;
}

LRESULT CGCServerManagerView::OnServerKickUserAck( WPARAM wParam, LPARAM lParam )
{
    //KPacketKickUserAck* pkAck = reinterpret_cast<KPacketKickUserAck*>( lParam );
    ACK_DESERIALIZE( int );

    if ( false == m_strChangeDBLogin.empty() )
    {
        switch ( kRecv )
        {
            case 0:
            {
                m_pMainFrm->m_logFile << L"[result] Success to kick user from corresponding  server." << std::endl;
                ::AfxMessageBox( _T("succeeded to kick user from corresponding  server.") );
                break;
            }
            case -1:
            {
                m_pMainFrm->m_logFile << L"[result] Not Exist in corresponding server." << std::endl;
                ::AfxMessageBox( _T("Not Exist in corresponding server.") );
                break;
            }
            case -2:
            {
                m_pMainFrm->m_logFile << L"[result] applicant is not administrator." << std::endl;
                ::AfxMessageBox( _T("applicant is not administrator.") );
                break;
            }
        }
    }

    SAFE_DELETE( pkBuff );
    //SAFE_DELETE( pkAck );

    if ( false == m_strChangeDBLogin.empty() )
    {
        //microcat. 이녀석은 아이디만 받을 수 있다.
        m_server.Send_UserDBInfoReq( m_strWideServerName.c_str(), m_strChangeDBLogin );
        m_pMainFrm->m_logFile << L"[request] " << m_strChangeDBLogin
                              << L" I request the User's DataBase information." << std::endl;
        m_strChangeDBLogin.erase();
    }

    return 0;
}

LRESULT CGCServerManagerView::OnServerChangeUserDBInfoAck( WPARAM wParam, LPARAM lParam )
{
    ACK_DESERIALIZE( KPacketAck );

    if ( 0 == kRecv.m_cOK )
    {
        m_pMainFrm->m_logFile << L"[result] Successed to modify DataBase Information." << std::endl;

        ::AfxMessageBox( _T("succeeded to modify DataBase Information") );
    }
    else
    {
        m_pMainFrm->m_logFile << L"[result] failed to modify DataBase Information." << std::endl;

        ::AfxMessageBox( _T("failed to modify DataBase Information") );
    }

    SAFE_DELETE( pkBuff );
    //SAFE_DELETE( pkAck );

    return 0;
}

LRESULT CGCServerManagerView::OnServerUserDBInfoAck( WPARAM wParam, LPARAM lParam )
{
    ACK_DESERIALIZE( KEVENT_QUERY_USER_INFO2_ACK );

    switch ( kRecv.m_cOK )
    {
        case 0:
        {
            m_strChangeDBLogin.erase();

            KChangeDBInfoDlg dlg( this );

            dlg.SetInfo( kRecv.m_kUserInfo );

            switch ( dlg.DoModal() )
            {
                //08-04-21 tslayer. 현우형 결정. 운영자툴 기능 축소.
                /*case IDOK:
                {
                    KUserInfoForOperator usrDBInfo;
                    //KUserDBInfo usrDBInfo;
                    std::wstring strReason;

                    dlg.GetInfo( usrDBInfo );

                    if ( USER_ADMIN == usrDBInfo.m_cAuthLevel )
                    {
                        if ( IDCANCEL == ::AfxMessageBox( _T("This User will be changed to Administrator. continue?"),
                                                          MB_OKCANCEL ) ) //이 유저는 운영자가 될 것입니다. 정말하겠습니까?
                        {
                            break; // 취소했으므로 나간다.
                        }
                    }
                    else if ( USER_BAD == usrDBInfo.m_cAuthLevel )
                    {
                        KReasonDlg reasonDlg( this );

                        if ( IDOK == reasonDlg.DoModal() )
                        {
                            switch ( reasonDlg.m_iSelect )
                            {
                                case 0: strReason = L"Curses"; break; //욕설
                                case 1: strReason = L"paper with useless words"; break;  //도배
                                case 2: strReason = L"false personation"; break; //신분 사칭
                            }
                        }
                        else break;
                    }
                    else if ( USER_STOP == usrDBInfo.m_cAuthLevel )
                    {
                        if ( IDCANCEL == ::AfxMessageBox( _T("This user will be blocked. Continue?"),
                                                          MB_OKCANCEL ) )
                        {
                            break; // 취소했으므로 나간다.
                        }
                        strReason = L"Block Account";
                    }

                    //microcat. 이녀석은 아이디만 받을 수 있다.
                    m_server.Send_ChangeUserDBInfoReq( m_strServerName.c_str(), usrDBInfo );
                    if ( USER_BAD == usrDBInfo.m_cAuthLevel || USER_STOP == usrDBInfo.m_cAuthLevel )
                    {
                        m_pMainFrm->AddBlackList( usrDBInfo.m_strName, strReason );
                    }
                    break;
                }*/
                case IDC_BUTTON_KICK_USER:
                {
                    if ( IDOK == ::AfxMessageBox( _T("Really Kick out?"), MB_OKCANCEL ) )
                    {
                        m_strChangeDBLogin = kRecv.m_kUserInfo.m_strName;
//{{ Changed by parkch: 2004-02-19 오전 10:30:56
//                        m_server.Send_KickUserReq( m_strServerName.c_str(),
//                                                   kRecv.m_kUserDBInfo.m_strLogin );
                        // 모든 서버에서 다 튕기는걸로 바꿈. (2004/02/19)
                        for ( size_t i = 0; i < m_pMainFrm->m_vecpChildView.size(); i++ )
                        {
                            CGCServerManagerView* pView = m_pMainFrm->m_vecpChildView[i];

                            //tslayer. 유저UID로 킥한다.
                            pView->m_server.Send_KickUserReq( kRecv.m_kUserInfo.m_dwUID );
                        }
//}} Changed by parkch: 2004-02-19 오전 10:30:56
                        m_pMainFrm->m_logFile << L"[request] " << kRecv.m_kUserInfo.m_strName
                                              << L" Kick out User." << std::endl;
                    }
                    break;
                }
                /*case IDC_BUTTON_DROP_USER:
                {
                    if ( IDOK == ::AfxMessageBox( _T("Delete the User's Account. Continue?"),
                                                  MB_OKCANCEL ) )
                    {
                        // 일단 유저를 튕겨낸다.
                        for ( size_t i = 0; i < m_pMainFrm->m_vecpChildView.size(); i++ )
                        {
                            CGCServerManagerView* pView = m_pMainFrm->m_vecpChildView[i];

                            //microcat. 이녀석은 아이디만 받을 수 있다.
                            pView->m_server.Send_KickUserReq( pView->m_strWideServerName.c_str(),
                                                              kRecv.m_kUserInfo.m_strName );
                        }
                        m_pMainFrm->m_logFile << L"[request] " << kRecv.m_kUserInfo.m_strName
                                              << L" kick out user." << std::endl;

                        // 계정을 삭제한다.
                        //microcat. 이녀석은 아이디만 받을 수 있다.
                        m_server.Send_DropUserDBInfoReq( m_strWideServerName.c_str(),
                                                         kRecv.m_kUserInfo.m_strName );
                        m_pMainFrm->m_logFile << L"[request] " << kRecv.m_kUserInfo.m_strName
                                              << L" Delete the User's Account." << std::endl;
                    }
                    break;
                }*/
                case IDC_BUTTON_DROP_USER:
                {
                    if ( IDOK == ::AfxMessageBox( _T("Really Chat Enable?"), MB_OKCANCEL ) )
                    {
                        m_strChangeDBLogin = kRecv.m_kUserInfo.m_strName;
                        //{{ Changed by parkch: 2004-02-19 오전 10:30:56
                        //                        m_server.Send_KickUserReq( m_strServerName.c_str(),
                        //                                                   kRecv.m_kUserDBInfo.m_strLogin );
                        // 모든 서버에서 다 튕기는걸로 바꿈. (2004/02/19)
                        for ( size_t i = 0; i < m_pMainFrm->m_vecpChildView.size(); i++ )
                        {
                            CGCServerManagerView* pView = m_pMainFrm->m_vecpChildView[i];

                            //tslayer. 유저UID로 킥한다.
                            pView->m_server.Send_UserChatChangeReq( kRecv.m_kUserInfo.m_dwUID, true );
                        }
                        //}} Changed by parkch: 2004-02-19 오전 10:30:56
                        m_pMainFrm->m_logFile << L"[request] " << kRecv.m_kUserInfo.m_strName
                            << L" Kick out User." << std::endl;
                    }
                    break;
                }
                case IDC_BUTTON_EXP_RESET:
                {
                    if ( IDOK == ::AfxMessageBox( _T("Really Chat Disable?"), MB_OKCANCEL ) )
                    {
                        m_strChangeDBLogin = kRecv.m_kUserInfo.m_strName;
                        //{{ Changed by parkch: 2004-02-19 오전 10:30:56
                        //                        m_server.Send_KickUserReq( m_strServerName.c_str(),
                        //                                                   kRecv.m_kUserDBInfo.m_strLogin );
                        // 모든 서버에서 다 튕기는걸로 바꿈. (2004/02/19)
                        for ( size_t i = 0; i < m_pMainFrm->m_vecpChildView.size(); i++ )
                        {
                            CGCServerManagerView* pView = m_pMainFrm->m_vecpChildView[i];

                            //tslayer. 유저UID로 킥한다.
                            pView->m_server.Send_UserChatChangeReq( kRecv.m_kUserInfo.m_dwUID, false );
                        }
                        //}} Changed by parkch: 2004-02-19 오전 10:30:56
                        m_pMainFrm->m_logFile << L"[request] " << kRecv.m_kUserInfo.m_strName
                            << L" Kick out User." << std::endl;
                    }
                    break;
                }
            }
            break;
        }
        case -1:
        {
            m_pMainFrm->m_logFile << L"[result] Failed to get DataBase Information." << std::endl;
            ::AfxMessageBox( _T("Failed to get DataBase Information") );
            break;
        }
        case -2:
        {
            m_pMainFrm->m_logFile << L"[result] Corresponding User does not exist." << std::endl;
            ::AfxMessageBox( _T("Corresponding User does not exist.") );
            break;
        }
        default:
        {
            ::AfxMessageBox( _T("User Not Found.") );
        }
    }

    SAFE_DELETE( pkBuff );
    //SAFE_DELETE( pkAck );

    return 0;
}

LRESULT CGCServerManagerView::OnServerDropUserDBInfoAck( WPARAM wParam, LPARAM lParam )
{
    //KPacketDropUserDBInfoAck* pkAck = reinterpret_cast<KPacketDropUserDBInfoAck*>( lParam );
    ACK_DESERIALIZE( KPacketAck );

    switch ( kRecv.m_cOK )
    {
        case 0:
        {
            m_pMainFrm->m_logFile << L"[result] Succeeded to delete Account." << std::endl;
            ::AfxMessageBox( _T("Succeeded to delete Account.") );
            break;
        }
        case -1:
        {
            m_pMainFrm->m_logFile << L"[result] failed to delete Account." << std::endl;
            ::AfxMessageBox( _T("failed to delete Account.") );
            break;
        }
        case -2:
        {
            m_pMainFrm->m_logFile << L"[result] Corresponding User does not exist." << std::endl;
            ::AfxMessageBox( _T("Corresponding User does not exist.") );
            break;
        }
    }

    SAFE_DELETE( pkBuff );
    //SAFE_DELETE( pkAck );

    return 0;
}

LRESULT CGCServerManagerView::OnServerCloseConnectionNot( WPARAM wParam, LPARAM lParam )
{
    //KPacketCloseConnectionNot* pkNot = reinterpret_cast<KPacketCloseConnectionNot*>( lParam );
    ACK_DESERIALIZE( KEVENT_CLOSE_CONNECTION_NOT );

    // 초기 상태로 복귀한다.
    SetUserState( US_INIT );

    ::AfxMessageBox( _T(" Disconnected.") );

    SAFE_DELETE( pkBuff );

    return 0;
}

void CGCServerManagerView::OnSize( UINT nType, int cx, int cy )
{
    CView::OnSize( nType, cx, cy );

    // 클라이언트 영역에 붙는 다이얼로그 박스들의 크기를 조정한다.
    if ( TRUE == ::IsWindow( m_channelLobby.m_hWnd ) )
    {
        m_channelLobby.MoveWindow( 0, 0, cx, cy );
    }
    if ( TRUE == ::IsWindow( m_channel.m_hWnd ) )
    {
        m_channel.MoveWindow( 0, 0, cx, cy );
    }
}


LRESULT CGCServerManagerView::OnSelectNormalItemAck( WPARAM wParam, LPARAM lParam )
{
    //KPacketSelectNormalItemAck* pkAck = reinterpret_cast<KPacketSelectNormalItemAck*>( lParam );
    ACK_DESERIALIZE( KItemInv );

    std::vector<KSelectNormalItemInfo> vecItemInv;
    std::vector<KItemInfoForOperator>::const_iterator   vit;
    KSelectNormalItemInfo kSelectNormalItemInfo;
    for( vit = kRecv.m_vecItemInv.begin(); vit != kRecv.m_vecItemInv.end(); vit++)
    {
        kSelectNormalItemInfo.m_strGoodsName    = vit->m_strGoodsName;
        kSelectNormalItemInfo.m_dwGoodsID       = vit->m_dwGoodsID;
        kSelectNormalItemInfo.m_dwGoodsUID      = vit->m_dwGoodsUID;
        kSelectNormalItemInfo.m_strOwnerLogin   = vit->m_strOwnerLogin;
        kSelectNormalItemInfo.m_strBuyerLogin   = vit->m_strBuyerLogin;
        kSelectNormalItemInfo.m_tRegDate        = vit->m_tRegDate;
        kSelectNormalItemInfo.m_tStartDate      = vit->m_tStartDate;
        kSelectNormalItemInfo.m_tEndDate        = vit->m_tEndDate;
        kSelectNormalItemInfo.m_iPeriod         = vit->m_iPeriod;
        kSelectNormalItemInfo.m_tDelDate        = vit->m_tDelDate;

        vecItemInv.push_back( kSelectNormalItemInfo );
    }
    m_dlgAdminDBAccess.SetListContrlData( vecItemInv );
    
    ::AfxMessageBox( _T("(Normal Item)These are belonging Items.") );
    UpdateData( TRUE );

    SAFE_DELETE( pkBuff );
    //SAFE_DELETE( pkAck );
    return 0;
}

LRESULT CGCServerManagerView::OnSelectDelNormalItemAck( WPARAM wParam, LPARAM lParam )
{
    //KPacketSelectDelNormalItemAck* pkAck = reinterpret_cast<KPacketSelectDelNormalItemAck*>( lParam );
    ACK_DESERIALIZE( KItemInv );

    std::vector<KSelectNormalItemInfo> vecItemInv;
    std::vector<KItemInfoForOperator>::const_iterator   vit;
    KSelectNormalItemInfo kSelectNormalItemInfo;
    for( vit = kRecv.m_vecItemInv.begin(); vit != kRecv.m_vecItemInv.end(); vit++)
    {
        kSelectNormalItemInfo.m_strGoodsName    = vit->m_strGoodsName;
        kSelectNormalItemInfo.m_dwGoodsID       = vit->m_dwGoodsID;
        kSelectNormalItemInfo.m_dwGoodsUID      = vit->m_dwGoodsUID;
        kSelectNormalItemInfo.m_strOwnerLogin   = vit->m_strOwnerLogin;
        kSelectNormalItemInfo.m_strBuyerLogin   = vit->m_strBuyerLogin;
        kSelectNormalItemInfo.m_tRegDate        = vit->m_tRegDate;
        kSelectNormalItemInfo.m_tDelDate        = vit->m_tDelDate;

        vecItemInv.push_back( kSelectNormalItemInfo );
    }
    m_dlgAdminDBAccess.SetListContrlData( vecItemInv );
    
    ::AfxMessageBox( _T("(Normal Item)These are sold Items.") );
    UpdateData( TRUE );
    
    SAFE_DELETE( pkBuff );
    //SAFE_DELETE( pkAck );
    return 0;

}

LRESULT CGCServerManagerView::OnSelectDurationItemAck( WPARAM wParam, LPARAM lParam )
{
    //KPacketSelectDurationItemAck* pkAck = reinterpret_cast<KPacketSelectDurationItemAck*>( lParam );
    ACK_DESERIALIZE( KDurationInv );

    std::vector<KSelectDurationItemInfo> vecItemInv;
    std::vector<KDurationItemInfoForOperator>::const_iterator   vit;
    KSelectDurationItemInfo kSelectDurationItemInfo;
    for( vit = kRecv.m_vecDurationItemInv.begin(); vit != kRecv.m_vecDurationItemInv.end(); vit++)
    {
        kSelectDurationItemInfo.m_strGoodsName      = vit->m_strGoodsName;
        kSelectDurationItemInfo.m_dwGoodsID         = vit->m_dwGoodsID;
        kSelectDurationItemInfo.m_dwGoodsUID        = vit->m_dwGoodsUID;
        kSelectDurationItemInfo.m_strOwnerLogin     = vit->m_strOwnerLogin;
        kSelectDurationItemInfo.m_strBuyerLogin     = vit->m_strBuyerLogin;
        kSelectDurationItemInfo.m_iDurationNum      = vit->m_iDurationNum;
        kSelectDurationItemInfo.m_tRegDate          = vit->m_tRegDate;
        kSelectDurationItemInfo.m_tDelDate          = vit->m_tDelDate;

        vecItemInv.push_back( kSelectDurationItemInfo );
    }
    m_dlgAdminDBAccess.SetListContrlDataDurationItem( vecItemInv );

    ::AfxMessageBox( _T("(disposable Item)These are belonging Items.") );
    UpdateData( TRUE );
    
    SAFE_DELETE( pkBuff );
    //SAFE_DELETE( pkAck );
    return 0;
}

LRESULT CGCServerManagerView::OnSelectDelDurationItemAck( WPARAM wParam, LPARAM lParam )
{
    //KPacketSelectDelDurationItemAck* pkAck = reinterpret_cast<KPacketSelectDelDurationItemAck*>( lParam );
    ACK_DESERIALIZE( KDurationInv );

    std::vector<KSelectDurationItemInfo> vecItemInv;
    std::vector<KDurationItemInfoForOperator>::const_iterator   vit;
    KSelectDurationItemInfo kSelectDurationItemInfo;
    for( vit = kRecv.m_vecDurationItemInv.begin(); vit != kRecv.m_vecDurationItemInv.end(); vit++)
    {
        kSelectDurationItemInfo.m_strGoodsName      = vit->m_strGoodsName;
        kSelectDurationItemInfo.m_dwGoodsID         = vit->m_dwGoodsID;
        kSelectDurationItemInfo.m_dwGoodsUID        = vit->m_dwGoodsUID;
        kSelectDurationItemInfo.m_strOwnerLogin     = vit->m_strOwnerLogin;
        kSelectDurationItemInfo.m_strBuyerLogin     = vit->m_strBuyerLogin;
        kSelectDurationItemInfo.m_iDurationNum      = vit->m_iDurationNum;
        kSelectDurationItemInfo.m_tRegDate          = vit->m_tRegDate;
        kSelectDurationItemInfo.m_tDelDate          = vit->m_tDelDate;

        vecItemInv.push_back( kSelectDurationItemInfo );
    }

    m_dlgAdminDBAccess.SetListContrlDataDurationItem( vecItemInv );
    
    ::AfxMessageBox( _T("(disposable Item)These are sold Items.") );
    UpdateData( TRUE );
    
    SAFE_DELETE( pkBuff );
    //SAFE_DELETE( pkAck );
    return 0;
}

LRESULT CGCServerManagerView::OnInsertGPNormalItemAck( WPARAM wParam, LPARAM lParam )
{
    //KPacketInsertGPNormalItemAck* pkAck = reinterpret_cast<KPacketInsertGPNormalItemAck*>( lParam );
    ACK_DESERIALIZE( KPacketAck );
     
    switch( kRecv.m_cOK )
    {
    case -1:
        {
            ::AfxMessageBox( _T("( Wrong buyerLogin.)" ));
            break;
        }
    case -5:
        {
            ::AfxMessageBox( _T("( Ownerlogin does not exist.)" ));
            break;
        }
    case -2:
        {
            ::AfxMessageBox( _T("( GoodsID does not exist.(Corresponding Item does not exist.)" ));
            break;
        }
    case -3:
        {
            ::AfxMessageBox( _T("( Item for Cash.)" ));
            break;
        }
    case -4:
        {
            ::AfxMessageBox( _T("( fail ! disposable Item.)" ));
            break;
        }
    case 0:
        {
            ::AfxMessageBox( _T("( Success ! Inserted Corresponding GP Normal Item.)" ));
            break;
        }
    default:
        {
			::AfxMessageBox( _T("( Error for insert : GP Normal Item.)" ));            
            break;
        }
    } 
    
    SAFE_DELETE( pkBuff );
    //SAFE_DELETE( pkAck );
    return 0;
}
LRESULT CGCServerManagerView::OnInsertGPDurationItemAck( WPARAM wParam, LPARAM lParam )
{
    //KPacketInsertGPDurationItemAck* pkAck = reinterpret_cast<KPacketInsertGPDurationItemAck*>( lParam );
    ACK_DESERIALIZE( KPacketAck );
    
    switch( kRecv.m_cOK )
    {
    case -1:
        {
            ::AfxMessageBox( _T("( Wrong buyerLogin.)" ));
            break;
        }
    case -5:
        {
			::AfxMessageBox( _T("( Ownerlogin does not exist.)" ));
            break;
        }
    case -2:
        {
			::AfxMessageBox( _T("( GoodsID does not exist.(Corresponding Item does not exist" ));
            break;
        }
    case -3:
        {
			::AfxMessageBox( _T("( Item for Cash.)" ));
            break;
        }
    case -4:
        {
			::AfxMessageBox( _T("( fail ! Normal Item.)" ));
            break;
        }
    case 0:
        {
			::AfxMessageBox( _T("( Success ! Inserted Corresponding GP disposable Item.)" ));
            break;
        }
    default:
        {
			::AfxMessageBox( _T("( Error for insert : GP disposable Item.)" ));            
            break;
        }
    } 
    
    SAFE_DELETE( pkBuff );
    //SAFE_DELETE( pkAck );
    return 0;

}
LRESULT CGCServerManagerView::OnInsertCashNormalItemAck( WPARAM wParam, LPARAM lParam )
{
    //KPacketInsertCashNormalItemAck* pkAck = reinterpret_cast<KPacketInsertCashNormalItemAck*>( lParam );
    ACK_DESERIALIZE( KPacketAck );
    
    switch( kRecv.m_cOK )
    {
    case -1:
        {
			::AfxMessageBox( _T("( Wrong buyerLogin.)" ));
            break;
        }
    case -5:
        {
			::AfxMessageBox( _T("( Ownerlogin does not exist.)" ));
            break;
        }
    case -2:
        {
			::AfxMessageBox( _T(" GoodsID does not exist.(Corresponding Item does not exist)" ));
            break;
        }
    case -3:
        {
			::AfxMessageBox( _T("( Item for GP.)" ));
            break;
        }
    case -4:
        {
			::AfxMessageBox( _T("(fail ! disposable Item.)" ));
            break;
        }
    case 0:
        {
			::AfxMessageBox( _T("( Success ! Inserted Corresponding Cash Normal Item.)" ));
            break;
        }
    default:
        {
			::AfxMessageBox( _T("( Error for insert : Cash Normal Item.)" ));            
            break;
        }
    } 
    
    SAFE_DELETE( pkBuff );
    //SAFE_DELETE( pkAck );
    return 0;
}

LRESULT CGCServerManagerView::OnInsertCashDurationItemAck( WPARAM wParam, LPARAM lParam )
{
    //KPacketInsertCashDurationItemAck* pkAck = reinterpret_cast<KPacketInsertCashDurationItemAck*>( lParam );
    ACK_DESERIALIZE( KPacketAck );
    
    switch( kRecv.m_cOK )
    {
    case -1:
        {
			::AfxMessageBox( _T("( Wrong buyerLogin.)" ));
            break;
        }
    case -5:
        {
			::AfxMessageBox( _T("(Ownerlogin does not exist.)" ));
            break;
        }
    case -2:
        {
			::AfxMessageBox( _T("(GoodsID does not exist.(Corresponding Item does not exist)" ));
            break;
        }
    case -3:
        {
			::AfxMessageBox( _T("( Item for GP.)" ));
            break;
        }
    case -4:
        {
			::AfxMessageBox( _T("( fail ! Normal Item.)" ));
            break;
        }
    case 0:
        {
			::AfxMessageBox( _T("( Success ! Inserted Corresponding Cash disposable Item.)" ));
            break;
        }
    default:
        {
			::AfxMessageBox( _T("( Error for insert : Cash disposable Item.)" ));            
            break;
        }
    } 
    
    SAFE_DELETE( pkBuff );
    //SAFE_DELETE( pkAck );
    return 0;
}

LRESULT CGCServerManagerView::OnDelItemAck( WPARAM wParam, LPARAM lParam )
{
    ACK_DESERIALIZE( KPacketAck );
    
    switch( kRecv.m_cOK )
    {
        case -1:
            {
                ::AfxMessageBox( _T("( User Login Does not exist in table.)" )); //유저 login이 테이블에 존재하지 않습니다.
                break;
            }
        case -2:
            {
                ::AfxMessageBox( _T("( User does not have corresponding item.)" ));
                break;
            }
        case 0:
            {
                ::AfxMessageBox( _T("( Success ! Succeeded to delete corresponding item.)" ));
                break;
            }
        default:
            {
                ::AfxMessageBox( _T("( Error for delete item.)" ));            
                break;
            }
    } 

    UpdateData( TRUE );
    
    SAFE_DELETE( pkBuff );
    return 0;

}
LRESULT CGCServerManagerView::OnSelectUserDataAck( WPARAM wParam, LPARAM lParam )
{
    //KPacketSelectUserDataAck* pkAck = reinterpret_cast<KPacketSelectUserDataAck*>( lParam );
    ACK_DESERIALIZE( KEVENT_QUERY_USER_INFO_ACK );

    if ( kRecv.m_cOK == 0 )
    {
        std::wstring strfirstLogin;
        std::wstring strlastLogin;
        CString strTemp;
        CString struserData;

        CTime cfirstTime = CTime( kRecv.m_firstLoginTime );
        strTemp.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), cfirstTime.GetYear(), cfirstTime.GetMonth(), cfirstTime.GetDay(), cfirstTime.GetHour(), cfirstTime.GetMinute(), cfirstTime.GetSecond());
        strfirstLogin = strTemp;

        CTime clastTime = CTime( kRecv.m_lastLoginTime );
        strTemp.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), clastTime.GetYear(), clastTime.GetMonth(), clastTime.GetDay(), clastTime.GetHour(), clastTime.GetMinute(), clastTime.GetSecond());
        strlastLogin  = strTemp;


        struserData.Format( _T(" User's Login : %s\n firstLoginTime : %s\n lastLoginTime : %s\n GP : %d\n Grade : %d\n GuildID : %d\n knight level : %d\n Archer level : %d\n Wizard level : %d\n"),
                            kRecv.m_strName.c_str(),
                            strfirstLogin.c_str(),
                            strlastLogin.c_str(),
                            kRecv.m_iGP,
                            kRecv.m_iGrade,
                            kRecv.m_iGuildid,
                            kRecv.m_iLevel0,
                            kRecv.m_iLevel1,
                            kRecv.m_iLevel2 );

        ::AfxMessageBox( struserData );
    }
    else
    {
        ::AfxMessageBox( _T("( User does not exist in corresponding table.") );
    }
  
    SAFE_DELETE( pkBuff );
    //SAFE_DELETE( pkAck );
    return 0;
}
LRESULT CGCServerManagerView::OnSelectChangeStrongItemListAck( WPARAM wParam, LPARAM lParam )
{
    //KPacketSelectChangeStrongItemListAck* pkAck = reinterpret_cast<KPacketSelectChangeStrongItemListAck*>( lParam );
    ACK_DESERIALIZE( KEVENT_CHANGE_STRONG_ITEM_ACK );

    CString strChangeStrongItem;
    std::string strRegDate;
    strChangeStrongItem = "";

    for ( size_t i = 0; i < kRecv.m_vecStrongItemInv.size(); i++ )
    {
        
        
        CString strTemp2;
        CString strTemp;
    
        strChangeStrongItem += L"Login : ";
        strChangeStrongItem += kRecv.m_vecStrongItemInv[i].m_strName.c_str();
        strChangeStrongItem += L" , item change : ";
        strTemp2.Format(_T("%d -> %d "), kRecv.m_vecStrongItemInv[i].m_dwBeforeGoodsID, kRecv.m_vecStrongItemInv[i].m_dwNextGoodsID );
        strChangeStrongItem += strTemp2;
        strChangeStrongItem += "   time : ";

        CTime cRegTime = CTime( kRecv.m_vecStrongItemInv[i].m_tRegDate);
        strTemp.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), cRegTime.GetYear(), cRegTime.GetMonth(), cRegTime.GetDay(), cRegTime.GetHour(), cRegTime.GetMinute(), cRegTime.GetSecond());
        strChangeStrongItem  += strTemp;
        strChangeStrongItem  += L'\n';
   }

    if ( kRecv.m_vecStrongItemInv.size() == 0 )
    {
        ::AfxMessageBox( _T("There is no change due to strengthening.") );
    }
    else
    {
        ::AfxMessageBox( strChangeStrongItem );
    }

    SAFE_DELETE( pkBuff );
    //SAFE_DELETE( pkAck );
    return 0;
}

//LRESULT CGCServerManagerView::OnSelectAdminUseItemInfoDataAck( WPARAM wParam, LPARAM lParam )
LRESULT CGCServerManagerView::GetAdminUseItemInfoData()
{
    std::vector<KAdminUseItemInfo> vecItemInv;

    m_kItemManager.GetItemInfo( vecItemInv );    

    m_dlgAdminDBAccess.SetAdminItemInfoDataList( vecItemInv );

    if ( vecItemInv.size() == 0 )
    {
        ::AfxMessageBox( _T("Failed to get item information.") );
    }
    else
    {
        ::AfxMessageBox( _T("Succeeded to get item information") );
    }

    //SAFE_DELETE( pkBuff );
    //SAFE_DELETE( pkAck );
    return 0;
    
}

void CGCServerManagerView::ChangeRoomPage() // 게임모드 바꿀 때도 사용하므로 이름을 바꿔야...  //  20080114 woosh 
{
    // 방 정보 리스트를 요청한다.
    if ( false == m_server.Send_RoomListReq( m_strServerName.c_str(), m_channel.GetGameMode(), m_channel.GetRoomListPage() ) )
    {
        ::AfxMessageBox( _T("failed to Send_RoomListReq.") );
    }
}

void CGCServerManagerView::ChangeUserPage()
{
    if ( false == m_server.Send_UserListReq( m_strServerName.c_str(), m_channel.GetUserListPage() ) ) //함수 부르지 말고 인자로 넘길까
    {
        ::AfxMessageBox( _T("failed to Send_UserListReq fail.") );
    }
}

void CGCServerManagerView::ChangeSquareInfoUpdate()
{
    if ( false == m_server.Send_SquareListReq() )
    {
        ::AfxMessageBox( _T("failed to Send_SquareListReq fail.") );
    }
}

LRESULT CGCServerManagerView::OnServerSquareUserListAck( WPARAM wParam, LPARAM lParam )
{
    ACK_DESERIALIZE( KPageInfo );
    SAFE_DELETE( pkBuff );

    std::vector<KInviteUserInfo> kPacket;
    kRecv.m_buffCompList.UnCompress();
    ks.BeginReading( &kRecv.m_buffCompList );
    ks.Get( kPacket );
    ks.EndReading();

    m_channel.SetSquareUserListPageSize( kRecv.m_nMaxPage ); // 최대 페이지 ( 방 개수에 따라 )
    m_channel.SetSquareUserListPage( kRecv.m_nCurrentPage ); // 현재 페이지 ( 정상이라면 보낸 페이지가 다시 올 것 )

    m_vecSquareUserListUpdate.clear();
    m_vecSquareUserListUpdate = kPacket;

    m_channel.MakeSquarePage();

    /*
    CString struserData;
    struserData.Format( _T("m_vecSquareUserListUpdate.size() : %d kRecv.m_nMaxPage : %d kRecv.m_nCurrentPage : %d\n"),
        m_vecSquareUserListUpdate.size(), kRecv.m_nMaxPage, kRecv.m_nCurrentPage );
    ::AfxMessageBox( struserData );
    */

    ChangeSquareUserList();

    return 0;
}

LRESULT CGCServerManagerView::OnServerSquareListAck( WPARAM wParam, LPARAM lParam )
{
    ACK_DESERIALIZE( std::vector<KSquareInfo> );

    m_vecSquareListUpdate.clear();
    m_vecSquareListUpdate = kRecv; // 광장 리스트 갱신

    UpdateSquareList();

    return 0;
}


void CGCServerManagerView::UpdateSquareList( void )
{
    // 광장 리스트를 업데이트한다.
    m_vecSquareList = m_vecSquareListUpdate;

    // 광장 리스트박스를 클리어한다.
    m_channel.m_ctrlSquareList.ResetContent();

    // 광장 리스트박스에 리스트를 표시한다.
    std::vector<KSquareInfo>::const_iterator vcit;
    for ( vcit = m_vecSquareList.begin() ; vcit != m_vecSquareList.end() ; ++vcit )
    {
        std::wostringstream strmSquareInfo;
        strmSquareInfo << vcit->m_dwID << L"." << vcit->m_strName
            << L"(" << vcit->m_dwCurrentUserNum << L"/" << vcit->m_dwMaxUserNum << L")";
        m_channel.m_ctrlSquareList.InsertString( -1, strmSquareInfo.str().c_str() );
    }
}

void CGCServerManagerView::ChangeSquareUserList( void )
{
    // 광장 유저 리스트를 갱신.
    m_vecSquareUserList = m_vecSquareUserListUpdate;

    /*
    CString struserData;
    struserData.Format( _T("m_vecSquareUserList.size() : %d\n"), m_vecSquareUserList.size() );
    ::AfxMessageBox( struserData );
    */

    // 광장 유저 리스트 박스를 클리어.
    m_channel.m_ctrlSquareUserList.ResetContent();

    // 광장 유저 리스트 박스에 리스트를 표시
    std::vector<KInviteUserInfo>::const_iterator vcit;
    for ( vcit = m_vecSquareUserList.begin() ; vcit != m_vecSquareUserList.end() ; ++vcit )
    {
        std::wostringstream strmSquareInfo;
        strmSquareInfo << vcit->m_strNickName; // 일단 된다는 전제하에 작성하지만. 닉네임을 넣자.
        m_channel.m_ctrlSquareUserList.InsertString( -1, strmSquareInfo.str().c_str() );
    }
}

void CGCServerManagerView::UpdateSquareUserList( void )
{
    if ( false == m_server.Send_SquareUserListReq( m_channel.m_ctrlSquareList.GetAnchorIndex()+1, m_channel.GetSquareUserListPage() ) )
    {
        ::AfxMessageBox( _T("failed to Send_SquareUserListReq fail.") );
    }
}

LRESULT CGCServerManagerView::OnServerUserChatChangeAck( WPARAM wParam, LPARAM lParam )
{
    ACK_DESERIALIZE( int );

    switch ( kRecv )
    {
    case 0:
        {
            m_pMainFrm->m_logFile << L"[result] Success to user chat option change from corresponding  server." << std::endl;
            ::AfxMessageBox( _T("succeeded to user chat option change from corresponding  server.") );
            break;
        }
    case -1:
        {
            m_pMainFrm->m_logFile << L"[result] Not Exist in corresponding server." << std::endl;
            ::AfxMessageBox( _T("Not Exist in corresponding server.") );
            break;
        }
    case -2:
        {
            m_pMainFrm->m_logFile << L"[result] The user has already asked." << std::endl;
            ::AfxMessageBox( _T("The user has already asked.") );
            break;
        }
    case -3:
        {
            m_pMainFrm->m_logFile << L"[result] db update fail." << std::endl;
            ::AfxMessageBox( _T("db update fail.") );
            break;
        }
    }

    SAFE_DELETE( pkBuff );

    return 0;
}
