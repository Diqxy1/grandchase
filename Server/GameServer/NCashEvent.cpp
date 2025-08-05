#if (_MSC_VER < 1310 )
#pragma warning( disable : 4786 )
#endif

#include "NCashEvent.h"
#include <dbg/dbg.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

std::map< int, std::wstring > KNCashEvent::ms_mapEventIDString;

KNCashEvent::KNCashEvent(void)
{
    Reset();
}

KNCashEvent::~KNCashEvent(void)
{
}

void KNCashEvent::Init()
{
#undef _ENUM
#define _ENUM( name, id ) KNCashEvent::ms_mapEventIDString[id] = L#name;
#include "NCashEvent_def.h"
}

void KNCashEvent::Reset()
{
    ::ZeroMemory( &m_kHeader, HEADER_SIZE_OF_NCASH );
    ::ZeroMemory( m_abyteNBuffer, MAX_PACKET_SIZE_OF_NCASH );
}

std::wstring KNCashEvent::GetEventIDString() const
{
    std::wstring wstrRet;
    if( ms_mapEventIDString.find( GetEventID() ) != ms_mapEventIDString.end() )
    {
        wstrRet = ms_mapEventIDString[GetEventID()];
    }
    else
    {
        wstrRet = L"";
    }

    return wstrRet;
}

void KNCashEvent::SetPacketHeader( IN unsigned short usEventID_, IN unsigned short usBodySize_ )
{
    m_kHeader.m_usEventID = usEventID_;
    m_kHeader.m_usBodySize = usBodySize_;

    START_LOG( clog, L"패킷 헤더" )
        << BUILD_LOG( usEventID_ )
        << BUILD_LOG( usBodySize_ ) << END_LOG;

    WritePacketHeader();
    return;
}

void KNCashEvent::SetPacketHeader( BYTE* abyteNBuffer )
{
    CopyToNBuffer( abyteNBuffer, 0, HEADER_SIZE_OF_NCASH );
    ReadPacketHeader();
}

void KNCashEvent::CopyToNBuffer( BYTE* abyteNBuffer, unsigned short usOffset, unsigned short usSize )
{
    if( usOffset + usSize > MAX_PACKET_SIZE_OF_NCASH )
    {
        START_LOG( cerr, L"카피 범위 초과." )
            << BUILD_LOG( usOffset )
            << BUILD_LOG( usSize )
            << BUILD_LOG( MAX_PACKET_SIZE_OF_NCASH )
            << END_LOG;
    
        return;
    }

    ::memcpy( m_abyteNBuffer + usOffset, abyteNBuffer, ( size_t )usSize );
}

void KNCashEvent::CopyFromNBuffer( BYTE* abyteNBuffer, unsigned short usOffset, unsigned short usSize )
{
    if( usOffset + usSize > MAX_PACKET_SIZE_OF_NCASH )
    {
        START_LOG( cerr, L"카피 범위 초과." )
            << BUILD_LOG( usOffset )
            << BUILD_LOG( usSize )
            << BUILD_LOG( MAX_PACKET_SIZE_OF_NCASH )
            << END_LOG;

        return;
    }

    ::memcpy( abyteNBuffer, m_abyteNBuffer + usOffset, ( size_t )usSize );
}

const BYTE* KNCashEvent::GetNBuffer() const
{
    return m_abyteNBuffer;
}

void KNCashEvent::ReadPacketHeader()
{
    ::memcpy( &m_kHeader, m_abyteNBuffer, HEADER_SIZE_OF_NCASH );
    m_kHeader.m_usEventID = ::ntohs( m_kHeader.m_usEventID );
    m_kHeader.m_usBodySize = ::ntohs( m_kHeader.m_usBodySize );

    return;
}

void KNCashEvent::Read( KEVENT_NCASH_INIT_REQ& kPacket )
{
    ::memcpy( &kPacket, m_abyteNBuffer + HEADER_SIZE_OF_NCASH, sizeof( KEVENT_NCASH_INIT_REQ ) );
    kPacket.m_usGameServiceID = ::ntohs( kPacket.m_usGameServiceID );
    kPacket.m_usCPID = ::ntohs( kPacket.m_usCPID );

    return;
}

void KNCashEvent::Read( KEVENT_NCASH_HEART_BIT_REQ& kPacket )
{
    ::memcpy( &kPacket, m_abyteNBuffer + HEADER_SIZE_OF_NCASH, sizeof( KEVENT_NCASH_HEART_BIT_REQ ) );
    kPacket.m_usGameServiceID = ::ntohs( kPacket.m_usGameServiceID );
    kPacket.m_usCPID = ::ntohs( kPacket.m_usCPID );

    return;
}

void KNCashEvent::Read( KEVENT_NCASH_CURRENT_MONEY_REQ& kPacket )
{
    ::memcpy( &kPacket, m_abyteNBuffer + HEADER_SIZE_OF_NCASH, sizeof( KEVENT_NCASH_CURRENT_MONEY_REQ ) );
    kPacket.m_usGameServiceID = ::ntohs( kPacket.m_usGameServiceID );
    kPacket.m_usCPID = ::ntohs( kPacket.m_usCPID );

    return;
}

void KNCashEvent::Read( KEVENT_NCASH_BUY_REQ& kPacket )
{
    ::memcpy( &kPacket, m_abyteNBuffer + HEADER_SIZE_OF_NCASH, sizeof( KEVENT_NCASH_BUY_REQ ) );
    kPacket.m_usGameServiceID = ::ntohs( kPacket.m_usGameServiceID );
    kPacket.m_usCPID = ::ntohs( kPacket.m_usCPID );
    kPacket.m_nProductCode = ::ntohl( kPacket.m_nProductCode );
    kPacket.m_nAmount = ::ntohl( kPacket.m_nAmount );

    return;
}

void KNCashEvent::Read( KEVENT_NCASH_PRESENT_REQ& kPacket )
{
    ::memcpy( &kPacket, m_abyteNBuffer + HEADER_SIZE_OF_NCASH, sizeof( KEVENT_NCASH_PRESENT_REQ ) );
    kPacket.m_usGameServiceID = ::ntohs( kPacket.m_usGameServiceID );
    kPacket.m_usCPID = ::ntohs( kPacket.m_usCPID );
    kPacket.m_nProductCode = ::ntohl( kPacket.m_nProductCode );
    kPacket.m_nAmount = ::ntohl( kPacket.m_nAmount );

    return;
}

void KNCashEvent::Read( KNCashAckPacket& kPacket )
{
    ::memcpy( &kPacket, m_abyteNBuffer + HEADER_SIZE_OF_NCASH, sizeof( KNCashAckPacket ) );
    kPacket.m_uiResult = ::ntohl( kPacket.m_uiResult );

    return;
}

void KNCashEvent::Read( OUT KEVENT_NCASH_CURRENT_MONEY_ACK& kPacket_ )
{
    ::memcpy( &kPacket_, m_abyteNBuffer + HEADER_SIZE_OF_NCASH, sizeof( KEVENT_NCASH_CURRENT_MONEY_ACK ) );
    kPacket_.m_usResult         = ::ntohs( kPacket_.m_usResult );
    kPacket_.m_nCashBalance    = ::ntohl( kPacket_.m_nCashBalance );
    kPacket_.m_nMiliageBalance = ::ntohl( kPacket_.m_nMiliageBalance );

    return;
}

void KNCashEvent::Read( OUT KEVENT_NCASH_BUY_ACK& kPacket_ )
{
    ::memcpy( &kPacket_, m_abyteNBuffer + HEADER_SIZE_OF_NCASH, sizeof( KEVENT_NCASH_BUY_ACK ) );
    kPacket_.m_sResult         = ::ntohs( kPacket_.m_sResult );
    kPacket_.m_nTransactionID  = ::ntohl( kPacket_.m_nTransactionID );
    return;
}

void KNCashEvent::Read( OUT KEVENT_NCASH_PRESENT_ACK& kPacket_ )
{
    ::memcpy( &kPacket_, m_abyteNBuffer + HEADER_SIZE_OF_NCASH, sizeof( KEVENT_NCASH_PRESENT_ACK ) );
    kPacket_.m_sResult         = ::ntohs( kPacket_.m_sResult );
    kPacket_.m_nTransactionID  = ::ntohl( kPacket_.m_nTransactionID );
    return;
}

void KNCashEvent::WritePacketHeader()
{
    KNCashPacketHeader kNHeader;

    kNHeader = m_kHeader;
    kNHeader.m_usEventID = ::htons( kNHeader.m_usEventID );
    kNHeader.m_usBodySize = ::htons( kNHeader.m_usBodySize );

    ::memcpy( m_abyteNBuffer, &kNHeader, HEADER_SIZE_OF_NCASH );

    return;
}

void KNCashEvent::Write( const KEVENT_NCASH_INIT_REQ& kPacket )
{
    KEVENT_NCASH_INIT_REQ kNPacket;

    kNPacket = kPacket;
    kNPacket.m_usGameServiceID = ::htons( kNPacket.m_usGameServiceID );
    kNPacket.m_usCPID = ::htons( kNPacket.m_usCPID );

    ::memcpy( m_abyteNBuffer + HEADER_SIZE_OF_NCASH, &kNPacket, sizeof( KEVENT_NCASH_INIT_REQ ) );

    return;
}

void KNCashEvent::Write( const KEVENT_NCASH_HEART_BIT_REQ& kPacket )
{
    KEVENT_NCASH_HEART_BIT_REQ kNPacket;

    kNPacket = kPacket;
    kNPacket.m_usGameServiceID = ::htons( kNPacket.m_usGameServiceID );
    kNPacket.m_usCPID = ::htons( kNPacket.m_usCPID );

    ::memcpy( m_abyteNBuffer + HEADER_SIZE_OF_NCASH, &kNPacket, sizeof( KEVENT_NCASH_HEART_BIT_REQ ) );

    return;
}

void KNCashEvent::Write( const KEVENT_NCASH_CURRENT_MONEY_REQ& kPacket )
{
    KEVENT_NCASH_CURRENT_MONEY_REQ kNPacket;

    kNPacket = kPacket;
    kNPacket.m_usGameServiceID = ::htons( kNPacket.m_usGameServiceID );
    kNPacket.m_usCPID = ::htons( kNPacket.m_usCPID );

    ::memcpy( m_abyteNBuffer + HEADER_SIZE_OF_NCASH, &kNPacket, sizeof( KEVENT_NCASH_CURRENT_MONEY_REQ ) );

    return;
}

void KNCashEvent::Write( const KEVENT_NCASH_BUY_REQ& kPacket )
{
    KEVENT_NCASH_BUY_REQ kNPacket;

    kNPacket = kPacket;
    kNPacket.m_usGameServiceID = ::htons( kNPacket.m_usGameServiceID );
    kNPacket.m_usCPID = ::htons( kNPacket.m_usCPID );
    kNPacket.m_nProductCode = ::htonl( kNPacket.m_nProductCode );
    kNPacket.m_nAmount = ::htonl( kNPacket.m_nAmount );
    kNPacket.m_biItemUID = htonll( kNPacket.m_biItemUID );

    ::memcpy( m_abyteNBuffer + HEADER_SIZE_OF_NCASH, &kNPacket, sizeof( KEVENT_NCASH_BUY_REQ ) );

    return;
}

void KNCashEvent::Write( const KEVENT_NCASH_PRESENT_REQ& kPacket )
{
    KEVENT_NCASH_PRESENT_REQ kNPacket;

    kNPacket = kPacket;
    kNPacket.m_usGameServiceID = ::htons( kNPacket.m_usGameServiceID );
    kNPacket.m_usCPID = ::htons( kNPacket.m_usCPID );
    kNPacket.m_nProductCode = ::htonl( kNPacket.m_nProductCode );
    kNPacket.m_nAmount = ::htonl( kNPacket.m_nAmount );
    kNPacket.m_biItemUID = htonll( kNPacket.m_biItemUID );

    ::memcpy( m_abyteNBuffer + HEADER_SIZE_OF_NCASH, &kNPacket, sizeof( KEVENT_NCASH_PRESENT_REQ ) );

    return;
}

void KNCashEvent::Write( const KNCashAckPacket& kPacket )
{
    KNCashAckPacket kNPacket;

    kNPacket = kPacket;
    kNPacket.m_uiResult = ::htonl( kNPacket.m_uiResult );

    ::memcpy( m_abyteNBuffer + HEADER_SIZE_OF_NCASH, &kNPacket, sizeof( KNCashAckPacket ) );

    return;
}

unsigned short KNCashEvent::GetPacketSize() const
{
    return HEADER_SIZE_OF_NCASH + m_kHeader.m_usBodySize;
}

unsigned short KNCashEvent::GetEventID() const
{
    return m_kHeader.m_usEventID;
}

void KNCashEvent::DumpNBuffer() const
{
    char szNBuffer[MAX_PACKET_SIZE_OF_NCASH * 3 + 1];
    char szByte[4];

    for( int i = 0; i < GetPacketSize(); i++ )
    {
        if( ( int )m_abyteNBuffer[i] >= 16 )
        {
            ::itoa( ( int )m_abyteNBuffer[i], szByte, 16 );
            szNBuffer[i * 3] = szByte[0];
            szNBuffer[i * 3 + 1] = szByte[1];
            szNBuffer[i * 3 + 2] = ' ';
        }
        else
        {
            ::itoa( ( int )m_abyteNBuffer[i], szByte, 16 );
            szNBuffer[i * 3] = '0';
            szNBuffer[i * 3 + 1] = szByte[0];
            szNBuffer[i * 3 + 2] = ' ';
        }

        if( i % 8 == 7 )
        {
            szNBuffer[i * 3 + 2] = '\n';
        }
    }
    szNBuffer[GetPacketSize() * 3] = 0;

    std::string strNBuffer = szNBuffer;
    START_LOG( clog, L"NBUFFER : ")
        << BUILD_LOG( KncUtil::toWideString( strNBuffer ) ) << END_LOG;
}
