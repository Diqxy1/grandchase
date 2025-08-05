#pragma once
#include "CenterProxy.h"
#include "KNCSingleton.h"
#include "KncTimer.h"

class KCnConnector
{
    DeclareSingleton( KCnConnector );
    DeclToStringW;
public:
    KCnConnector(void);
    virtual ~KCnConnector(void);

    // 기본
    void Init();
    void Tick();
    void ShutDown();

    bool Connect();

    template < typename T > 
    bool SendPacket( unsigned short usEventID, T& data, bool bLogging = true, bool bCompress = false );
    bool SendID( unsigned short usEventID );
    void SetRemoteAddress( const char* szIP, unsigned short usPort );
    void SetName( const std::wstring& strServerName );
    bool SendServerRelayPacket( const unsigned short usEventID, const KSerBuffer& kEventBuff, const DWORD dwSenderUID, const std::wstring& strSenderLogin,
                                const DWORD dwRecverUID, const std::wstring& strRecverLogin, const bool bUID_, const bool bDirectSend_ );
    bool SendSquareRelayPacket( const unsigned short usEventID, const KSerBuffer& kEventBuff, const DWORD dwSenderUID_, 
                                const DWORD dwSquareUID_, const bool bNeighborhoodSend_ );

protected:
    KCenterProxyPtr     m_spCnPtr;
    std::wstring        m_strServerName;
    std::string         m_strIP;
    unsigned short      m_usPort;
    DWORD               m_dwLastConnectTick;
};

DefSingletonInline( KCnConnector );
DeclOstmOperatorA( KCnConnector );

template < typename T > 
bool KCnConnector::SendPacket( unsigned short usEventID_, T& data_, bool bLogging_, bool bCompress_ )
{
    __JIF( m_spCnPtr, return false );
    __JIF( m_spCnPtr->IsConnected(), return false );
    __JIF( m_spCnPtr->IsAfterVerify(), return false );
    return m_spCnPtr->SendPacket( usEventID_, data_, bLogging_, bCompress_ );
}

#define  SEND_SERVER_RELAY_UID( eventid, serbuff, senderuid, senderlogin, recvuid )     SiKCnConnector()->SendServerRelayPacket( KUserEvent::##eventid, serbuff, senderuid, senderlogin, recvuid, std::wstring(L""), true, true )
#define  SEND_SERVER_RELAY_LOGIN( eventid, serbuff, senderuid, senderlogin, recvlogin ) SiKCnConnector()->SendServerRelayPacket( KUserEvent::##eventid, serbuff, senderuid, senderlogin, 0, recvlogin, false, true )
#define  QUEUE_SERVER_RELAY_UID( eventid, serbuff, senderuid, senderlogin, recvuid )     SiKCnConnector()->SendServerRelayPacket( KUserEvent::##eventid, serbuff, senderuid, senderlogin, recvuid, std::wstring(L""), true, false )
#define  QUEUE_SERVER_RELAY_LOGIN( eventid, serbuff, senderuid, senderlogin, recvlogin ) SiKCnConnector()->SendServerRelayPacket( KUserEvent::##eventid, serbuff, senderuid, senderlogin, 0, recvlogin, false, false )
