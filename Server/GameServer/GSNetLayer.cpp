#include "Socket/Session.h"
#include "GSNetLayer.h"
#include "GSSimLayer.h"
#include <sstream>
#include "user.h"
#include "Log4.h"

NiImplementRTTI( KGSNetLayer, KNetLayer );
ImplementSingleton( KGSNetLayer );

KGSNetLayer::KGSNetLayer(void) : m_usUdpPort( 0 )
{
}

KGSNetLayer::~KGSNetLayer(void)
{
}

ImplToStringW( KGSNetLayer )
{
    KLocker lock( m_csURSAddr );

    START_TOSTRING_PARENTW( KNetLayer )
        << TOSTRINGW( m_usUdpPort )
        << TOSTRINGW( m_vecURServerAddr.size() );

    KSession::DumpMaxSendData( stm_ );
    stm_ << L" -- UDP Relay Server Address -- " << std::endl;

    int nCount = 0;
    std::vector<SOCKADDR_IN>::const_iterator vit;

    for( vit = m_vecURServerAddr.begin(); vit != m_vecURServerAddr.end(); vit++, nCount++ )
    {
        stm_ << L"  Index:" << nCount 
             << L", addr:" << inet_ntoa(vit->sin_addr) << L":" << ntohs(vit->sin_port) << std::endl;
    }

    stm_ << L" -- TCP Relay Server Address -- " << std::endl;

    nCount = 0;
    
    for( vit = m_vecTRServerAddr.begin(); vit != m_vecTRServerAddr.end(); vit++, nCount++ )
    {
        stm_ << L"  Index:" << nCount 
            << L", addr:" << inet_ntoa(vit->sin_addr) << L":" << ntohs(vit->sin_port) << std::endl;
    }

    return stm_;
}
ImplOstmOperatorW( KGSNetLayer );
ImplOstmOperatorW2A( KGSNetLayer );

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

bool KGSNetLayer::Init()
{
    m_vecAccepter.push_back( IAccepterPtr( new KAccepter<KUser>( m_usPortMaster ) ) );

    _JIF( KNetLayer::Init(), return false );

    _JIF( m_kUdpEcho.Init( m_usUdpPort ), return false );

    m_kUdpEcho.Begin();

    return true;
}

void KGSNetLayer::ResetMaxData()
{
    std::wostringstream strstm;

    KSession::DumpMaxSendData( strstm );
    KNetLayer::ResetMaxData();
    START_LOG( cerr, L"" << strstm.str() ) << END_LOG;    
    KSession::ResetMaxSendData();

}

void KGSNetLayer::RegToLua( std::shared_ptr<lua_State> L )
{
    KNetLayer::RegToLua( L );

    luabind::module( L.get())
    [
        luabind::def( "GetNetLayer", &KGSNetLayer::GetInstance ),

        luabind::class_<KGSNetLayer, KNetLayer>( "KGSNetLayer" )
            .def( luabind::tostring(luabind::self) )
            .def( "AddURServerAddress", &KGSNetLayer::AddURServerAddress )
            .def( "DeleteURServerAddress", &KGSNetLayer::DeleteURServerAddress )
            .def_readwrite( "m_usUdpPort", &KGSNetLayer::m_usUdpPort )
            .def( "AddTRServerAddress", &KGSNetLayer::AddTRServerAddress )
            .def( "DeleteTRServerAddress", &KGSNetLayer::DeleteTRServerAddress )
            .def( "ResetMaxData", &KGSNetLayer::ResetMaxData )
    ];
}

void KGSNetLayer::ShutDown()
{
    KNetLayer::ShutDown();

    m_kUdpEcho.ShutDown();
}

void KGSNetLayer::AddURServerAddress( const char* szIP_, USHORT usPort_ )
{
    SOCKADDR_IN addrURServer;
    addrURServer.sin_addr.s_addr = inet_addr( szIP_ );
    addrURServer.sin_port        = htons( usPort_ );

    {
        KLocker lock( m_csURSAddr );
        m_vecURServerAddr.push_back( addrURServer );
    }
}

bool KGSNetLayer::GetURServerAddress( IN int nKey_, OUT DWORD& dwIP_, OUT USHORT& usPort_ )
{
    KLocker lock( m_csURSAddr );
    if( m_vecURServerAddr.empty() )
    {
        START_LOG( cerr, L"릴레이 서버의 주소가 설정되어있지 않음." ) << END_LOG;
        return false;
    }

    int nIndex = nKey_ % m_vecURServerAddr.size();

    dwIP_   = m_vecURServerAddr[nIndex].sin_addr.S_un.S_addr;
    usPort_ = ntohs(m_vecURServerAddr[nIndex].sin_port);

    return true;
}

bool KGSNetLayer::DeleteURServerAddress( IN unsigned int nIndex_ )
{
    KLocker lock( m_csURSAddr );

    if( nIndex_ >= m_vecURServerAddr.size() )
    {
        START_LOG( cwarn, L"올바르지 못한 인덱스. index:" << nIndex_ )
            << BUILD_LOG( m_vecURServerAddr.size() ) << END_LOG;
        return false;
    }

    m_vecURServerAddr.erase( m_vecURServerAddr.begin() + nIndex_ );

    // 콘솔 창으로만 출력한다.
    std::wcout << nIndex_ << L"번 릴레이 서버 삭제됨." << std::endl
        << *this;

    return true;
}
//--
void KGSNetLayer::AddTRServerAddress( const char* szIP_, USHORT usPort_ )
{
    SOCKADDR_IN addrTRServer;
    addrTRServer.sin_addr.s_addr = inet_addr( szIP_ );
    addrTRServer.sin_port        = htons( usPort_ );

    {
        KLocker lock( m_csTRSAddr );
        m_vecTRServerAddr.push_back( addrTRServer );
    }
}

bool KGSNetLayer::GetTRServerAddress( IN int nKey_, IN const std::vector<KSimpleServerInfo>& vecUseTCPServerInfo_, IN const std::vector<KSimpleServerInfo>& vecNoUseTCPServerInfo_, OUT DWORD& dwIP_, OUT USHORT& usPort_ )
{
    std::vector<KSimpleServerInfo>::const_iterator cvit;
    in_addr address;
    std::wstring wstrUseIP;
    std::string strUseIP;
    USHORT usUsePort;

    KLocker lock( m_csTRSAddr );
    if( m_vecTRServerAddr.empty() && vecUseTCPServerInfo_.empty() ) {
        START_LOG( cerr, L"TCP릴레이 서버의 주소가 설정되어있지 않음." ) << END_LOG;
        return false;
    }

    int nIndex = nKey_ % m_vecTRServerAddr.size();

    dwIP_   = m_vecTRServerAddr[nIndex].sin_addr.S_un.S_addr;
    usPort_ = ntohs(m_vecTRServerAddr[nIndex].sin_port);

    address.s_addr = dwIP_;
    std::wstring strIP = KncUtil::toWideString( ::inet_ntoa( address ) );

    if( vecNoUseTCPServerInfo_.empty() || vecUseTCPServerInfo_.empty() ) {
        return true;
    }

    for( cvit = vecNoUseTCPServerInfo_.begin(); cvit != vecNoUseTCPServerInfo_.end(); ++cvit ) {
        if( cvit->m_strIP == strIP && cvit->m_usPort == usPort_ ) {
            int nSize = vecUseTCPServerInfo_.size();
            nSize = std::max<int>( 1, nSize );
            nIndex = nKey_ % nSize;
            wstrUseIP = vecUseTCPServerInfo_[nIndex].m_strIP;
            usUsePort = vecUseTCPServerInfo_[nIndex].m_usPort;

            strUseIP = KncUtil::toNarrowString( wstrUseIP );

            SOCKADDR_IN addrTRServer;
            addrTRServer.sin_addr.s_addr = inet_addr( strUseIP.c_str() );
            dwIP_ = addrTRServer.sin_addr.S_un.S_addr;
            usPort_ = usUsePort;
            return true;
        }
    }

    return true;
}

bool KGSNetLayer::DeleteTRServerAddress( IN unsigned int nIndex_ )
{
    KLocker lock( m_csTRSAddr );

    if( nIndex_ >= m_vecTRServerAddr.size() )
    {
        START_LOG( cwarn, L"올바르지 못한 인덱스. index:" << nIndex_ )
            << BUILD_LOG( m_vecURServerAddr.size() ) << END_LOG;
        return false;
    }

    m_vecTRServerAddr.erase( m_vecTRServerAddr.begin() + nIndex_ );

    // 콘솔 창으로만 출력한다.
    std::wcout << nIndex_ << L"번 TCP릴레이 서버 삭제됨." << std::endl
        << *this;

    return true;
}
