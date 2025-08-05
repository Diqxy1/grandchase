#pragma once

//#include "GCUTIL/KJohnGen.h"
#include "UserProxy.h"
#include <KNCSingleton.h>
//#include "KGCMsgServer.h"

#ifdef _OPERATOR_TOOL_
#include <string>
#include <map>
#else
#include "NUserProxy.h"
#endif
//#include "Packet.h"
#include "MSProxy.h"

#define SAFE_DELETE(p) if (p) { delete (p); (p) = NULL; } 
class KGMProxy
{
    DeclareSingleton( KGMProxy );
public:
    enum
    {
        MININUM_PCBANG_UID  = 2147483000,   //050711. microcat.
        USERPROXY_ID    = 1,
        NUSERPROXY_ID   = 2,
        TRUSERPROXY_ID  = 3,
        MSPROXY_ID      = 4,
    };

public:
    KGMProxy() : m_pkUserProxy( NULL ) 
    {
    //    m_kMsgServer = NULL;

    }  // inline으로 구현. 운영자툴에서 pch 사용여부 설정때문에 링크되지 않는 문제 수정.
    virtual ~KGMProxy() {
     //   SAFE_DELETE( m_kMsgServer );
        SAFE_DELETE( m_pkUserProxy ); 
    }

    // 051118. 서버에서 받은 UID를 클라이언트에서는 (일반:*2, 수량:*2+1)해서 중복되지 않게 만든다.
    static int ConvertUID_S2C( int iServerUID, bool bIsDurationItem )  
    { 
        if( MININUM_PCBANG_UID <= iServerUID )
            return iServerUID;
        return iServerUID * 2 + bIsDurationItem; 
    }
    static int ConvertUID_C2S( int iClientUID )
    { 
        if( MININUM_PCBANG_UID <= iClientUID )
            return iClientUID;
        return iClientUID / 2; 
    }

    bool Init( HWND hWnd );
    bool Send_UserDBInfoReq(const wchar_t* szPeerName, const std::wstring& strLogin, bool bIsNickname = false);
    bool Send_ChangeUserDBInfoReq(const char* szPeerName, KUserInfoForOperator& kUserDBInfo);
    bool Send_KickUserReq( const DWORD dwUID_ );
    bool Send_DropUserDBInfoReq(const wchar_t* szPeerName, const std::wstring& strLogin);
    bool Send_LeaveChannelNot(const char* szPeerName);
    bool Send_ChannelInfoListReq(const char* szPeerName);
    int ConnectForGMTool( IN const KServerInfo& kServerInfo, IN const KEVENT_GMTOOL_AUTHENTICATION_REQ& kReq );
    bool Send_EnterChannelReq(const wchar_t* szPeerName, DWORD dwChannelUID_ );
    bool Send_NotifyMessageNot(const wchar_t* szPeerName, const std::wstring& strMessage);
    bool Send_ChatReq(const wchar_t* szPeerName, const wchar_t* szMessage, const DWORD dwColor = 0xff000000 );
    bool Send_RoomListReq(const char* szPeerName, const int iGameMode, const int iRoomListPage );
    //bool Send_RoomListReq(const char* szPeerName );
    bool Send_UserListReq(const char* szPeerName, const int nPage );
    bool Send_InsertCashDurationItemReq( const wchar_t* szPeerName_, std::wstring strName_, DWORD dwGoodsID_ );
    bool Send_SelectNormalItemReq( const wchar_t* szPeerName_, std::wstring strName_ );
    bool Send_InsertCashNormalItemReq( const wchar_t* szPeerName_, std::wstring strName_, DWORD dwGoodsID_ );
    bool Send_InsertGPNormalItemReq( const wchar_t* szPeerName_, std::wstring strName_, DWORD dwGoodsID_ );
    bool Send_DelItemReq( const wchar_t* szPeerName_, std::wstring strName_, DWORD dwGoodsID_, DWORD dwGoodsUID_ );
    bool Send_SelectDelNormalItemReq( const wchar_t* szPeerName_, std::wstring strName_ );
    bool Send_SelectDurationItemReq( const wchar_t* szPeerName_, std::wstring strName_ );
    bool Send_SelectDelDurationItemReq( const wchar_t* szPeerName_, std::wstring strName_ );
    bool Send_SelectUserDataReq( const wchar_t* szPeerName_, std::wstring strName_ );
    bool Send_SelectChangeStrongItemListReq( const wchar_t* szPeerName_, std::wstring strName_ );
    bool Send_InsertGPDurationItemReq( const wchar_t* szPeerName_, std::wstring strName_, DWORD dwGoodsID_ );
     //tslayer. 광장 처리
    bool Send_SignBoard_Notice( const std::wstring& strMessage_, int nType_ );
    bool Send_SquareListReq();
    bool Send_SquareUserListReq( const DWORD dwSquareNum, const int nPage);
    bool Send_UserChatChangeReq( IN const DWORD dwUID, IN const bool bChatEnable );

public:
    //KGCMsgServer* m_kMsgServer;	
    KUserProxy*   m_pkUserProxy;  // 서버간 이동을 위해, 포인터를 가지고 중간에 다른 객체를 가르킬 수 있도록 한다.    

protected:
    HWND        m_hWnd;
};

DefSingletonInline( KGMProxy );

#define ACK_DESERIALIZE( packet )  KSerBuffer* pkBuff = (KSerBuffer*)lParam; \
    packet kRecv; \
    ks.BeginReading( pkBuff ); \
    ks.Get( kRecv ); \
    ks.EndReading()

#define ACK_DESERIALIZE_MYPACKET( kRecv )  KSerBuffer* pkBuff = (KSerBuffer*)lParam; \
    ks.BeginReading( pkBuff ); \
    ks.Get( kRecv ); \
    ks.EndReading()
