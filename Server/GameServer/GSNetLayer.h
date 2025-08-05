#pragma once
#include "NetLayer.h"
#include "Socket/UdpEcho.h"
#include "CommonPacket.h"

class KGSNetLayer : public KNetLayer
{
    DeclToStringW;
    NiDeclareRTTI;
    DeclareSingleton( KGSNetLayer );

protected:
    KGSNetLayer(void);
public:
    virtual ~KGSNetLayer(void);

    // derived from KNetLayer
    void ResetMaxData();
    virtual bool Init();
    virtual void ShutDown();
    virtual void RegToLua( std::shared_ptr<lua_State> L );

    void AddURServerAddress( const char* szIP, USHORT usPort );
    bool GetURServerAddress( IN int nKey, OUT DWORD& dwIP, OUT USHORT& usPort );
    bool DeleteURServerAddress( IN unsigned int nIndex );


    void AddTRServerAddress( const char* szIP, USHORT usPort );
    bool GetTRServerAddress( IN int nKey_, IN const std::vector<KSimpleServerInfo>& vecUseTCPServerInfo_, IN const std::vector<KSimpleServerInfo>& vecNoUseTCPServerInfo_, OUT DWORD& dwIP_, OUT USHORT& usPort_ );
    bool DeleteTRServerAddress( IN unsigned int nIndex );

protected:
    KUdpEcho                    m_kUdpEcho;
    mutable KncCriticalSection  m_csURSAddr;
    std::vector<SOCKADDR_IN>    m_vecURServerAddr;

    mutable KncCriticalSection  m_csTRSAddr;
    std::vector<SOCKADDR_IN>    m_vecTRServerAddr;

public: // lua access
    USHORT          m_usUdpPort;
};

DefSingletonInline( KGSNetLayer );