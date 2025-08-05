#pragma once

#include <map>
#include <windows.h>

#define HEADER_SIZE_OF_PAYLETTER ( ( int )sizeof( KPayLetterPacketHeader ) )
#define MAX_PACKET_SIZE_OF_PAYLETTER 256

#pragma pack( push, 1 )

struct KPayLetterPacketHeader // 6byte
{
    WORD    m_usReqLen;  // size of packet   , WORD = unsigned short, 2byte
    WORD    m_usReqType; // purpose
    WORD    m_usRetCode; // return code

    KPayLetterPacketHeader()
    {
        m_usReqLen = 0;
        m_usReqType = 0;
        m_usRetCode = 0;
    }  
};

struct KEVENT_PAYLETTER_BALANCE
{
    char    m_chUserID[50+1]; // 51 byte
    DWORD   m_dwCashRemain; // 4 byte

    KEVENT_PAYLETTER_BALANCE()
    {
        m_dwCashRemain = 0;
        memset( m_chUserID, 0, 51 );
    }
};

struct KEVENT_PAYLETTER_BUY
{
    char    m_chUserID[50+1];

    DWORD   m_dwCashRemain;
    char    m_chChargeNo[19+1];

    char    m_chItemID[30+1];
    int     m_nItemPrice;
    char    m_chItemName[50+1];

    KEVENT_PAYLETTER_BUY()
    {
        m_dwCashRemain = 0;
        m_nItemPrice = 0;

        memset( m_chUserID, 0, 51 );
        memset( m_chChargeNo, 0, 20 );
        memset( m_chItemID, 0, 31 );
        memset( m_chItemName, 0, 51 );
    }
};


struct KEVENT_PAYLETTER_GIFT
{
    __int64 iUserNo;
    __int64 iRUserNo;
    char    UserID[50+1];
    char    RUserID[50+1];
    DWORD   dwCashRemain;

};

struct KEVENT_PAYLETTER_BUY_CANCEL
{
    char    chUserID[50+1];
    DWORD   dwCashRemain;
    char    chChargeNo[19+1];
};

struct KEVENT_PAYLETTER_ALIVE  // ReqType = 301
{
    char    m_chServerIP[50+1];        // 빌링 서버 GTX 데몬 IP
    char    m_chServerName[64+1];      // 빌링 서버 GTX 데몬 명 (BOQGTXLevelUpGC)
    WORD    m_usServerPort;            // 빌링 서버 GTX 데몬 포트
};

#pragma pack( pop )

class KPayLetterEvent
{
public:
#   undef   _ENUM
#   define  _ENUM( name, id ) name = id,
    enum ENUM_EVENT_ID {
#       include "PayLetterEvent_def.h"
    };

    static std::map< int, std::wstring > ms_mapEventIDString;

public:
    KPayLetterEvent(void);
    virtual ~KPayLetterEvent(void);

    static void Init();
    void Reset();
    std::wstring GetEventIDString() const;
    unsigned short GetEventID() const;
    unsigned short GetPacketSize() const;
    unsigned short GetReturnValue() const;


    void SetPacketHeader( unsigned short usReqType, unsigned short usReqLen, unsigned short usRetCode = 0 );
    void SetPacketHeader( BYTE* abytePLBuffer );
    void CopyToANBuffer( BYTE* abytePLBuffer, unsigned short usOffset, unsigned short usSize );

    void ReadPacketHeader();
    void Read( KEVENT_PAYLETTER_BALANCE& kPacket );
    void Read( KEVENT_PAYLETTER_BUY& kPacket );
    void Read( KEVENT_PAYLETTER_BUY_CANCEL& kPacket );
    void Read( KEVENT_PAYLETTER_GIFT& kPacket );

    void WritePacketHeader();
    void Write( const KEVENT_PAYLETTER_BALANCE& kPacket );
    void Write( const KEVENT_PAYLETTER_BUY& kPacket );
    void Write( const KEVENT_PAYLETTER_BUY_CANCEL& kPacket );
    void Write( const KEVENT_PAYLETTER_GIFT& kPacket );
    void Write( const KEVENT_PAYLETTER_ALIVE& kPacket );
    const BYTE* GetANBuffer() const;

    void DumpNBuffer() const;


protected:
    //unsigned short m_usPacketSize; // 패킷사이즈
    KPayLetterPacketHeader m_kHeader; // 패킷 헤더. (common)
    BYTE m_abyteNBuffer[MAX_PACKET_SIZE_OF_PAYLETTER]; // 패킷을 합칠 버퍼

};
