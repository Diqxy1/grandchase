#pragma once

#include <map>
#include <windows.h>

// NCash 빌링 시스템의 헤더 사이즈
#define HEADER_SIZE_OF_NCASH ( ( int )sizeof( KNCashPacketHeader ) )
#define MAX_PACKET_SIZE_OF_NCASH 256
#define USER_ID 32
#define GAME_SERVICE_ID 6
#define GAME_SERVER_UNIQUE_ID 1
#define ITEM_NAME 50
#define DEFAULT_MILIAGE_VALUE 1

// htonl 함수가 기본 int밖에 지원하지 않기 때문에 __int64를 지원하기 위해서 정의.
#define htonll(x) \
    ((((x) & 0xff00000000000000LL) >> 56) | \
    (((x) & 0x00ff000000000000LL) >> 40) |  \
    (((x) & 0x0000ff0000000000LL) >> 24) |  \
    (((x) & 0x000000ff00000000LL) >> 8) |  \
    (((x) & 0x00000000ff000000LL) << 8) | \
    (((x) & 0x0000000000ff0000LL) << 24) | \
    (((x) & 0x000000000000ff00LL) << 40) | \
    (((x) & 0x00000000000000ffLL) << 56))
#define ntohll(x) \
    ((((x) & 0x00000000000000FF) << 56) | \
    (((x) & 0x000000000000FF00) << 40) | \
    (((x) & 0x0000000000FF0000) << 24) | \
    (((x) & 0x00000000FF000000) << 8) | \
    (((x) & 0x000000FF00000000) >> 8) | \
    (((x) & 0x0000FF0000000000) >> 24) | \
    (((x) & 0x00FF000000000000) >> 40) | \
    (((x) & 0xFF00000000000000) >> 56))

#pragma pack( push, 1 )

struct KNCashPacketHeader
{
    unsigned short  m_usEventID;
    unsigned short  m_usBodySize;

    KNCashPacketHeader()
    {
        m_usEventID = 0;
        m_usBodySize = 0;
    }
};

struct KEVENT_NCASH_INIT_REQ
{
    unsigned short  m_usGameServiceID;
    unsigned short  m_usCPID;

    KEVENT_NCASH_INIT_REQ()
    {
        m_usGameServiceID = 0;
        m_usCPID = 0;
    }
};

struct KEVENT_NCASH_HEART_BIT_REQ
{
    unsigned short  m_usGameServiceID;
    unsigned short  m_usCPID;

    KEVENT_NCASH_HEART_BIT_REQ()
    {
        m_usGameServiceID = 0;
        m_usCPID = 0;
    }
};

struct KEVENT_NCASH_CURRENT_MONEY_REQ
{
    unsigned short  m_usGameServiceID;
    unsigned short  m_usCPID;
    char            m_aucUserID[USER_ID];

    KEVENT_NCASH_CURRENT_MONEY_REQ()
    {
        m_usGameServiceID = GAME_SERVICE_ID;
        m_usCPID = GAME_SERVER_UNIQUE_ID;
        for( int i = 0; i < USER_ID; i++ )
        {
            m_aucUserID[i] = 0;
        }
    }
};

struct KEVENT_NCASH_BUY_REQ
{
    unsigned short  m_usGameServiceID;
    unsigned short  m_usCPID;
    char            m_aucUserID[USER_ID];
    int             m_nProductCode;
    char            m_aucProductName[ITEM_NAME];
    int             m_nAmount;
    char            m_aucIsMillage[DEFAULT_MILIAGE_VALUE];
    __int64         m_biItemUID;

    KEVENT_NCASH_BUY_REQ()
    {
        m_usGameServiceID = GAME_SERVICE_ID;
        m_usCPID = GAME_SERVER_UNIQUE_ID;
        int i = 0;
        for( i = 0; i < USER_ID; i++ )
        {
            m_aucUserID[i] = 0;
        }
        m_nProductCode = 0;
        for( i = 0; i < ITEM_NAME; i++ )
        {
            m_aucProductName[i] = 0;
        }
        m_nAmount = 0;
        for( i = 0; i < DEFAULT_MILIAGE_VALUE; i++ )
        {
            m_aucIsMillage[i] = 0;
        }
        m_biItemUID = 0;
    }
};

struct KEVENT_NCASH_PRESENT_REQ
{
    unsigned short  m_usGameServiceID;
    unsigned short  m_usCPID;
    char            m_aucSUserID[USER_ID];
    char            m_aucRUserID[USER_ID];
    int             m_nProductCode;
    char            m_aucProductName[ITEM_NAME];
    int             m_nAmount;
    char            m_aucIsMillage[DEFAULT_MILIAGE_VALUE];
    __int64         m_biItemUID;

    KEVENT_NCASH_PRESENT_REQ()
    {
        int i = 0;
        m_usGameServiceID = GAME_SERVICE_ID;
        m_usCPID = GAME_SERVER_UNIQUE_ID;
        for( i = 0; i < USER_ID; i++ )
        {
            m_aucSUserID[i] = 0;
        }
        for( i = 0; i < USER_ID; i++ )
        {
            m_aucRUserID[i] = 0;
        }
        m_nProductCode = 0;
        for( i = 0; i < ITEM_NAME; i++ )
        {
            m_aucProductName[i] = 0;
        }
        m_nAmount = 0;
        for( i = 0; i < DEFAULT_MILIAGE_VALUE; i++ )
        {
            m_aucIsMillage[i] = 0;
        }
        m_biItemUID = 0;
    }
};

struct KEVENT_NCASH_CURRENT_MONEY_ACK
{
    unsigned short  m_usGameServiceID;
    unsigned short  m_usCPID;
    short           m_usResult;
    char            m_aucUserID[USER_ID];
    int             m_nCashBalance;
    int             m_nMiliageBalance;

    KEVENT_NCASH_CURRENT_MONEY_ACK()
    {
        int i = 0;
        m_usGameServiceID = 0;
        m_usCPID = 0;
        m_usResult = 0;
        for( i = 0; i < USER_ID; i++ )
        {
            m_aucUserID[i] = 0;
        }
        m_nCashBalance = 0;
        m_nMiliageBalance = 0;
    }
};

struct KEVENT_NCASH_BUY_ACK
{
    unsigned short  m_usGameServiceID;
    unsigned short  m_usCPID;
    short           m_sResult;
    char            m_aucUserID[USER_ID];
    int             m_nTransactionID;

    KEVENT_NCASH_BUY_ACK()
    {
        int i = 0;
        m_usGameServiceID = 0;
        m_usCPID = 0;
        m_sResult = 0;
        for( i = 0; i < USER_ID; i++ )
        {
            m_aucUserID[i] = 0;
        }
        m_nTransactionID = 0;
    }
};

struct KEVENT_NCASH_PRESENT_ACK
{
    unsigned short  m_usGameServiceID;
    unsigned short  m_usCPID;
    short           m_sResult;
    int             m_nTransactionID;

    KEVENT_NCASH_PRESENT_ACK()
    {
        m_usGameServiceID = 0;
        m_usCPID = 0;
        m_sResult = 0;
        m_nTransactionID = 0;
    }
};

struct KNCashAckPacket
{
    unsigned int    m_uiResult;

    KNCashAckPacket()
    {
        m_uiResult = 0;
    }
};

#pragma pack( pop )

class KNCashEvent
{
public:
#   undef  _ENUM
#   define _ENUM( name, id ) name = id,
    enum ENUM_EVENT_ID { 
#       include "NCashEvent_def.h" 
    };

    static std::map< int, std::wstring > ms_mapEventIDString;

public:
    KNCashEvent(void);
    virtual ~KNCashEvent(void);

    static void Init();
    void Reset();
    std::wstring GetEventIDString() const;

    /*
    void GetPacketHeaderToSend( const KNCashPacketHeader& kHeader, BYTE* abyteNBuffer );
    void GetPacketBodyToSend( BYTE* abyteHBuffer, BYTE* abyteNBuffer );
    void SetPacketHeaderReceived( BYTE* abyteNBuffer );
    void SetPacketBodyReceived( BYTE* abyteNBuffer );
*/
    void SetPacketHeader( IN unsigned short usEventID, IN unsigned short usBodySize );
    void SetPacketHeader( BYTE* abyteNBuffer );
    void CopyToNBuffer( BYTE* abyteNBuffer, unsigned short usOffset, unsigned short usSize );
    void CopyFromNBuffer( BYTE* abyteNBuffer, unsigned short usOffset, unsigned short usSize );
    const BYTE* GetNBuffer() const;

    void ReadPacketHeader();
    void Read( KEVENT_NCASH_INIT_REQ& kPacket );
    void Read( KEVENT_NCASH_HEART_BIT_REQ& kPacket );
    void Read( KEVENT_NCASH_CURRENT_MONEY_REQ& kPacket );
    void Read( KEVENT_NCASH_BUY_REQ& kPacket );
    void Read( KEVENT_NCASH_PRESENT_REQ& kPacket );
    void Read( KNCashAckPacket& kPacket );
    void Read( OUT KEVENT_NCASH_CURRENT_MONEY_ACK& kPacket );
    void Read( OUT KEVENT_NCASH_BUY_ACK& kPacket );
    void Read( OUT KEVENT_NCASH_PRESENT_ACK& kPacket );

    void WritePacketHeader();
    void Write( const KEVENT_NCASH_INIT_REQ& kPacket );
    void Write( const KEVENT_NCASH_HEART_BIT_REQ& kPacket );
    void Write( const KEVENT_NCASH_CURRENT_MONEY_REQ& kPacket );
    void Write( const KEVENT_NCASH_BUY_REQ& kPacket );
    void Write( const KEVENT_NCASH_PRESENT_REQ& kPacket );
    void Write( const KNCashAckPacket& kPacket );

    unsigned short GetPacketSize() const;
    unsigned short GetEventID() const;

    void DumpNBuffer() const;

protected:
    KNCashPacketHeader m_kHeader;
    BYTE m_abyteNBuffer[MAX_PACKET_SIZE_OF_NCASH];
};