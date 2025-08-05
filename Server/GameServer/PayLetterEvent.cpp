#include "PayLetterEvent.h"
#include <dbg/dbg.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

std::map< int, std::wstring > KPayLetterEvent::ms_mapEventIDString;


KPayLetterEvent::KPayLetterEvent(void)
{
    Reset();
}

KPayLetterEvent::~KPayLetterEvent(void)
{
}

void KPayLetterEvent::Init()
{
#undef _ENUM
#define _ENUM( name, id ) KPayLetterEvent::ms_mapEventIDString[id] = L#name;
#include "PayLetterEvent_def.h"
}

void KPayLetterEvent::Reset()
{
    ::ZeroMemory( &m_kHeader, HEADER_SIZE_OF_PAYLETTER );
    ::ZeroMemory( m_abyteNBuffer, MAX_PACKET_SIZE_OF_PAYLETTER );
}

std::wstring KPayLetterEvent::GetEventIDString() const // usually for log
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

unsigned short KPayLetterEvent::GetEventID() const
{
    return m_kHeader.m_usReqType;
}

unsigned short KPayLetterEvent::GetReturnValue() const
{
    return m_kHeader.m_usRetCode;
}

void KPayLetterEvent::CopyToANBuffer( BYTE* abyteANBuffer_, unsigned short usOffset_, unsigned short usSize_ )
{
    if( usOffset_ + usSize_ > MAX_PACKET_SIZE_OF_PAYLETTER )
    {
        START_LOG( cerr, L"카피 범위 초과." )
            << BUILD_LOG( usOffset_ )
            << BUILD_LOG( usSize_ )
            << BUILD_LOG( MAX_PACKET_SIZE_OF_PAYLETTER )
            << END_LOG;

        return;
    }

    ::memcpy( m_abyteNBuffer + usOffset_, abyteANBuffer_, ( size_t )usSize_ );
}

const BYTE* KPayLetterEvent::GetANBuffer() const
{
    return m_abyteNBuffer;
}

unsigned short KPayLetterEvent::GetPacketSize() const
{
    return m_kHeader.m_usReqLen;
}

void KPayLetterEvent::SetPacketHeader( unsigned short usReqType_, unsigned short usReqLen_, unsigned short usRetCode_ )
{
    m_kHeader.m_usReqType = usReqType_;
    m_kHeader.m_usReqLen = usReqLen_;
    m_kHeader.m_usRetCode = usRetCode_; // not use in header

    START_LOG( clog, L"PayLetterEvent Packet Header :" )
        << BUILD_LOG( usReqType_ )
        << BUILD_LOG( usReqLen_ )
        << BUILD_LOG( usRetCode_ ) << END_LOG;

    WritePacketHeader();
}

void KPayLetterEvent::SetPacketHeader( BYTE* abytePLBuffer_ )
{
    CopyToANBuffer( abytePLBuffer_, 0, HEADER_SIZE_OF_PAYLETTER );
    // 헤더이기 때문에 오프셋 0
    ReadPacketHeader();
}

void KPayLetterEvent::WritePacketHeader()
{   // 패킷 헤더
    KPayLetterPacketHeader kPLHeader;

    kPLHeader = m_kHeader;
    kPLHeader.m_usReqType   = ::htons( kPLHeader.m_usReqType );
    kPLHeader.m_usReqLen    = ::htons( kPLHeader.m_usReqLen );
    kPLHeader.m_usRetCode   = ::htons( kPLHeader.m_usRetCode );

    ::memcpy( m_abyteNBuffer, &kPLHeader, HEADER_SIZE_OF_PAYLETTER );

    return;
}

void KPayLetterEvent::Write( const KEVENT_PAYLETTER_BALANCE& kPacket_ )
{
    KEVENT_PAYLETTER_BALANCE kNPacket;

    kNPacket = kPacket_;
    kNPacket.m_dwCashRemain = ::htonl( kNPacket.m_dwCashRemain );
    
    ::memcpy( m_abyteNBuffer + HEADER_SIZE_OF_PAYLETTER, &kNPacket, sizeof( KEVENT_PAYLETTER_BALANCE ) );

    return;
}

void KPayLetterEvent::Write( const KEVENT_PAYLETTER_BUY& kPacket_ )
{
    KEVENT_PAYLETTER_BUY kNPacket;

    kNPacket = kPacket_;
    kNPacket.m_nItemPrice = ::htonl( kNPacket.m_nItemPrice );
    kNPacket.m_dwCashRemain = ::htonl( kNPacket.m_dwCashRemain );

    ::memcpy( m_abyteNBuffer + HEADER_SIZE_OF_PAYLETTER, &kNPacket, sizeof( KEVENT_PAYLETTER_BUY ) );
}

void KPayLetterEvent::Write( const KEVENT_PAYLETTER_ALIVE& kPacket_ )
{
    KEVENT_PAYLETTER_ALIVE kNPacket;

    kNPacket = kPacket_;
    kNPacket.m_usServerPort = ::htons( kNPacket.m_usServerPort );

    ::memcpy( m_abyteNBuffer + HEADER_SIZE_OF_PAYLETTER, &kNPacket, sizeof( KEVENT_PAYLETTER_ALIVE ) );
}


void KPayLetterEvent::ReadPacketHeader()
{
    ::memcpy( &m_kHeader, m_abyteNBuffer, HEADER_SIZE_OF_PAYLETTER );
    m_kHeader.m_usReqType = ::ntohs( m_kHeader.m_usReqType );
    m_kHeader.m_usReqLen  = ::ntohs( m_kHeader.m_usReqLen );
    m_kHeader.m_usRetCode = ::ntohs( m_kHeader.m_usRetCode );

    START_LOG( clog, L"ReadPacketHeader, 패킷헤더 도착" )
        << BUILD_LOG( m_kHeader.m_usReqType )
        << BUILD_LOG( m_kHeader.m_usReqLen )
        << BUILD_LOG( m_kHeader.m_usRetCode ) << END_LOG;

    return;
}

void KPayLetterEvent::Read( KEVENT_PAYLETTER_BALANCE& kPacket_ )
{
    ::memcpy( &kPacket_, m_abyteNBuffer + HEADER_SIZE_OF_PAYLETTER, sizeof( KEVENT_PAYLETTER_BALANCE ) );
    kPacket_.m_dwCashRemain = ::ntohl( kPacket_.m_dwCashRemain );
}

void KPayLetterEvent::Read( KEVENT_PAYLETTER_BUY& kPacket_ )
{
    ::memcpy( &kPacket_, m_abyteNBuffer + HEADER_SIZE_OF_PAYLETTER, sizeof( KEVENT_PAYLETTER_BUY ) );
    
    kPacket_.m_dwCashRemain = ::ntohl( kPacket_.m_dwCashRemain );
    kPacket_.m_nItemPrice = ::ntohl( kPacket_.m_nItemPrice );
}


void KPayLetterEvent::DumpNBuffer() const
{
    char szNBuffer[MAX_PACKET_SIZE_OF_PAYLETTER * 3 + 1]; // ?
    char szByte[4]; // ?

    for( int i = 0; i < GetPacketSize(); i++ )
    {
        if( (int)m_abyteNBuffer[i] >= 16 ) // 16보다 크면
        {
            ::itoa( ( int )m_abyteNBuffer[i], szByte, 16 ); // 숫자를 문자로 변환, 16진수
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
            szNBuffer[i * 3 + 2] = '\n'; // 8개 출력 후 다음줄
        }
    }
    szNBuffer[GetPacketSize() * 3] = 0;

    std::string strNBuffer = szNBuffer;

    std::string strTmp;
    strTmp.append( (char*)(m_abyteNBuffer), sizeof(m_abyteNBuffer) );

    START_LOG( clog, L"BUFFER : ")
        << BUILD_LOG( KncUtil::toWideString( strNBuffer ) )
        << BUILD_LOG( KncUtil::toWideString( strTmp ) )
        << BUILD_LOG( sizeof(m_abyteNBuffer) ) << END_LOG;

}