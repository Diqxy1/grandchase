#include "StdAfx.h"

#pragma comment( lib, "Ws2_32" )
#pragma warning(disable:4995)
#include "GMProxy.h"

#include <ImportKncSerializer.h>
#include <ImportKncDebug.h>
#include <ImportKncSecurity.h>
#include "UserPacket.h"
//#include "define.h"
#include "NetError.h"
#include "ClientErr.h"


ImplementSingleton( KGMProxy );

int KGMProxy::ConnectForGMTool( IN const KServerInfo& kServerInfo_, IN const KEVENT_GMTOOL_AUTHENTICATION_REQ& kReq_ )
{
    int nRet = NetError::ERR_UNKNOWN;

    KUserProxy* pkUserProxy = new KUserProxy;

    pkUserProxy->Init( false );
    pkUserProxy->SetHwnd( m_hWnd );

    KEventPtr spEvent( new KUserEvent );
    spEvent->m_usEventID = KUserEvent::EVENT_GMTOOL_AUTHENTICATION_REQ;
    spEvent->Serialize( kReq_ );

    if( (nRet = pkUserProxy->ConnectNAuth( kServerInfo_, spEvent, 5000 )) < 0 )
    {
        START_LOG( cerr, L"connecting to game server failed." )
            << BUILD_LOG( KncUtil::toWideString( kServerInfo_.m_strIP ) )
            << BUILD_LOG( kServerInfo_.m_usPort )
            << END_LOG;

        SAFE_DELETE( pkUserProxy );
        return nRet;
    }

    std::swap( m_pkUserProxy, pkUserProxy );

    SAFE_DELETE( pkUserProxy );

    return nRet;
}

bool KGMProxy::Send_EnterChannelReq(const wchar_t* szPeerName,  DWORD dwChannelUID_ )
{
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ENTER_CHANNEL_REQ, dwChannelUID_ );
}

bool KGMProxy::Send_ChannelInfoListReq(const char* szPeerName_)
{
    return m_pkUserProxy->SendID( KUserEvent::EVENT_CHANNEL_LIST_REQ );
}

bool KGMProxy::Send_NotifyMessageNot(const wchar_t* szPeerName_, const std::wstring& strMessage_)
{
    std::wstring kPacket( strMessage_ );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_NOTIFY_REQ, kPacket );
}

bool KGMProxy::Send_RoomListReq(const char* szPeerName_, const int iGameMode_, const int iRoomListPage_ )
{
    KEVENT_ROOM_LIST_REQ kPacket;
    kPacket.m_bWaitRoom = false;
    kPacket.m_nType = iGameMode_;
    kPacket.m_nDifficult = -1; // 헬모드 들어간 국가는 필요(-1:all)
    kPacket.m_nPage = iRoomListPage_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ROOM_LIST_REQ, kPacket );
}

bool KGMProxy::Send_ChangeUserDBInfoReq(const char* szPeerName_, KUserInfoForOperator& kUserDBInfo_)
{
    if( g_nLevel > 0 )
    {
        AfxMessageBox( _T("Low Level..") );
        return false;
    }

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_USER_INFO_REQ, kUserDBInfo_ );
}

bool KGMProxy::Send_KickUserReq( IN const DWORD dwUID_ )
{
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_KICK_USER_BY_ADMIN_REQ, dwUID_ );
}

bool KGMProxy::Send_DropUserDBInfoReq(const wchar_t* szPeerName_, const std::wstring& strLogin_)
{
    KPacket2Name kPacket;
    kPacket.m_strName       = strLogin_;
    kPacket.m_bIsNickname   = false;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_REMOVE_USER_INFO_REQ, kPacket );
}

bool KGMProxy::Init( HWND hWnd_ )
{
    m_hWnd = hWnd_;

#ifndef _OPERATOR_TOOL_
    m_spTRUserProxy->SetHwnd( hWnd_ );
    KUserProxy::InitForClient();

    m_spNUserProxy->Init( false );
    m_spNUserProxy->SetHwnd( m_hWnd );

    //m_spMSProxy->Init( false );
    //m_spMSProxy->SetHwnd( m_hWnd );

#endif
 //   m_kMsgServer = new KGCMsgServer( m_hWnd );

    return true;
}

bool KGMProxy::Send_LeaveChannelNot(const char* szPeerName_)
{
    return m_pkUserProxy->SendID( KUserEvent::EVENT_LEAVE_CHANNEL_NOT );
}


bool KGMProxy::Send_ChatReq(const wchar_t* szPeerName_, 
                        const wchar_t* szMessage_,
                        const DWORD dwColor )
{
    //std::wstring kPacket( szMessage_ );    
    KChatData kPacket;

    if( dwColor != 0xff000000 )
    {
        kPacket.m_cChatType = KChatData::MSG_COLOR_CHAT;
        kPacket.m_dwChatColor = dwColor;
    }
    else
    {
        kPacket.m_cChatType = KChatData::MSG_COMMON;
        kPacket.m_dwChatColor = dwColor;
    }
    kPacket.m_strSenderNick = szPeerName_;
    kPacket.m_strChatMsg = szMessage_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHAT_REQ, kPacket );
} 


bool KGMProxy::Send_InsertCashDurationItemReq( const wchar_t* szPeerName_, std::wstring strName_, DWORD dwGoodsID_ )
{
    KAddItemInfo kPacket;
    kPacket.m_strName       = strName_;
    kPacket.m_dwGoodsID     = dwGoodsID_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ADD_DURATION_ITEM_CASH_REQ, kPacket );
}

//godsend 운영자 툴 서버(DB)에서 일반 아이템 목록 다 가져오기 
bool KGMProxy::Send_SelectNormalItemReq( const wchar_t* szPeerName_, std::wstring strName_ )
{
    KPacket2Name kPacket;
    kPacket.m_strName       = strName_;
    kPacket.m_bIsNickname   = false;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_QUERY_NORMAL_ITEM_LIST_REQ, kPacket );
}

bool KGMProxy::Send_InsertGPNormalItemReq( const wchar_t* szPeerName_, std::wstring strName_, DWORD dwGoodsID_ )
{
    if( g_nLevel > 0 )
    {
        AfxMessageBox( _T("Low Level..") );
        return false;
    }

    KAddItemInfo kPacket;
    kPacket.m_strName       = strName_;
    kPacket.m_dwGoodsID     = dwGoodsID_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ADD_NORMAL_ITEM_GP_REQ , kPacket );
}

bool KGMProxy::Send_InsertCashNormalItemReq( const wchar_t* szPeerName_, std::wstring strName_, DWORD dwGoodsID_ )
{
    if( g_nLevel > 0 )
    {
        AfxMessageBox( _T("Low Level..") );
        return false;
    }

    KAddItemInfo kPacket;
    kPacket.m_strName       = strName_;
    kPacket.m_dwGoodsID     = dwGoodsID_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ADD_NORMAL_ITEM_CASH_REQ, kPacket );
}

bool KGMProxy::Send_DelItemReq( const wchar_t* szPeerName_, std::wstring strName_, DWORD dwGoodsID_, DWORD dwGoodsUID_ )
{
    KEVENT_REMOVE_ITEM_REQ kPacket;
    kPacket.m_dwGoodsUID    = dwGoodsUID_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_REMOVE_ITEM_REQ, kPacket );
}

bool KGMProxy::Send_SelectDelNormalItemReq( const wchar_t* szPeerName_, std::wstring strName_ )
{

    KPacket2Name kPacket;
    kPacket.m_strName       = strName_;
    kPacket.m_bIsNickname   = false;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_QUERY_SALED_NORMAL_ITEM_LIST_REQ, kPacket );
}

//수량성 
bool KGMProxy::Send_SelectDurationItemReq( const wchar_t* szPeerName_, std::wstring strName_ )
{
    KPacket2Name kPacket;
    kPacket.m_strName       = strName_;
    kPacket.m_bIsNickname   = false;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_QUERY_DURATION_ITEM_LIST_REQ, kPacket );
}

bool KGMProxy::Send_SelectDelDurationItemReq( const wchar_t* szPeerName_, std::wstring strName_ )
{
    KPacket2Name kPacket;
    kPacket.m_strName       = strName_;
    kPacket.m_bIsNickname   = false;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_QUERY_SALED_DURATION_ITEM_LIST_REQ, kPacket );
}

bool KGMProxy::Send_SelectUserDataReq( const wchar_t* szPeerName_, std::wstring strName_ ) 
{
    KPacket2Name kPacket;
    kPacket.m_strName       = strName_;
    kPacket.m_bIsNickname   = false;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_QUERY_USER_INFO_REQ, kPacket );
}

bool KGMProxy::Send_SelectChangeStrongItemListReq( const wchar_t* szPeerName_, std::wstring strName_ ) 
{
    KPacket2Name kPacket;
    kPacket.m_strName       = strName_;
    kPacket.m_bIsNickname   = false;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_STRONG_ITEM_REQ, kPacket );
}

bool KGMProxy::Send_InsertGPDurationItemReq( const wchar_t* szPeerName_, std::wstring strName_, DWORD dwGoodsID_ )
{
    if( g_nLevel > 0 )
    {
        AfxMessageBox( _T("Low Level..") );
        return false;
    }

    KAddItemInfo kPacket;
    kPacket.m_strName       = strName_;
    kPacket.m_dwGoodsID     = dwGoodsID_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ADD_DURATION_ITEM_GP_REQ, kPacket );
}

bool KGMProxy::Send_UserListReq(const char* szPeerName_, const int nPage_ )
{
    //return m_pkUserProxy->SendPacket( KUserEvent::EVENT_USER_LIST_REQ, i );  // for test 
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GMTOOL_USER_LIST_REQ, nPage_ );
}

bool KGMProxy::Send_UserDBInfoReq(const wchar_t* szPeerName_, const std::wstring& strLogin_, bool bIsNickname)
{
    KPacket2Name kPacket;
    kPacket.m_strName       = strLogin_;
    kPacket.m_bIsNickname   = bIsNickname;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_QUERY_USER_INFO2_REQ, kPacket );
}

bool KGMProxy::Send_SignBoard_Notice( const std::wstring& strMessage_, int nType_ )
{
    KEVENT_SIGN_BOARD_NOTIFY kPacket;
    kPacket.m_eType = nType_;
    kPacket.m_strMsg = strMessage_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SIGN_BOARD_NOTIFY, kPacket );
}

bool KGMProxy::Send_SquareListReq()
{
    DWORD dwGuildUID = 0;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SQUARE_LIST_REQ, dwGuildUID );
}

bool KGMProxy::Send_SquareUserListReq( const DWORD dwSquareNum, const int nPage)
{
    PAIR_DWORD_INT kPacket;
    kPacket.first = dwSquareNum;
    kPacket.second = nPage;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GMTOOL_SQUARE_USER_LIST_REQ, kPacket );
}

bool KGMProxy::Send_UserChatChangeReq( IN const DWORD dwUID_, IN const bool bChatEnable_ )
{
    KEVENT_USER_CHAT_CHANGE_BY_ADMIN_REQ kPacket;
    kPacket.m_dwUserUID = dwUID_;
    kPacket.m_bChatEnable = bChatEnable_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_USER_CHAT_CHANGE_BY_ADMIN_REQ, kPacket );
}