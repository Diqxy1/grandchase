#pragma once
//#define MAX_PACKET_SIZE 16384	// 1024 * 16 (16Kbyte)
//#define MAX_PACKET_SIZE 32768	// 1024 * 32 (32Kbyte)
#define MAX_PACKET_SIZE 49152   // 1024 * 48 (48Kbyte)
//#define MAX_PACKET_SIZE 65536	// 1024 * 64 (64Kbyte)

class KOVERLAPPED : public OVERLAPPED
{
public:
    enum ENUM_IO_MODE {
        IO_RECV,
        IO_SEND
    };

    KOVERLAPPED() 
    {
        hEvent              = NULL;

        Clear();
        m_wsaBuffer.buf       = m_pBuffer;
        m_wsaBuffer.len       = 0;
        m_dwLeft              = 0;
        m_bSending            = false;
    }

    void Clear() 
    {
        Internal            = 0;
        InternalHigh        = 0;
        Offset              = 0;
        OffsetHigh          = 0;
    } 

public:
    ENUM_IO_MODE            m_eIOMode;
    // WSARecv()/WSASend()용 buffer
    WSABUF                  m_wsaBuffer;
    char                    m_pBuffer[MAX_PACKET_SIZE];
    // Send / Recv때의 용도가 다름 
    DWORD                   m_dwLeft;
    bool                    m_bSending;
};