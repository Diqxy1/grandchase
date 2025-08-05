#pragma once

#include <string>
#include <map>
#include <queue>

#include <KNCSingleton.h>
#include <KNCException.h>
#include <KncLua.h>
#include <ObjectPool/BlockAllocator.h>
#include <RTTI.h>
#include <ToString.h>
#include <Thread/Thread.h>
#include <boost/shared_ptr.hpp>
#include "KncSmartPtr.h"
#include "GenericLogfile.h"

#define MAX_UDP_PACKET_SIZE (1024 * 11)

struct KUdpAddr : public BlockAllocator<KUdpAddr, 5000>
{
    DWORD           m_dwUID;
    SOCKADDR_IN     m_sockaddr;
    DWORD           m_dwLastRecvTick;
};

/// tgkwon 090406
struct KSendData : public BlockAllocator<KSendData, 5000>
{
	std::vector< std::pair<DWORD, SOCKADDR_IN> > m_vecUserInfo;
	USHORT          m_usDataSize;
	char            m_buff[MAX_UDP_PACKET_SIZE];

	KSendData()
	{
		m_vecUserInfo.reserve( 10 );
		m_usDataSize = 0;
		memset( m_buff, 0, sizeof(m_buff) );
	}
};

typedef boost::shared_ptr<KUdpAddr>  KUdpAddrPtr;
typedef boost::shared_ptr<KSendData> KSendDataPtr;

class KRelayServer
{
    NiDeclareRootRTTI( KRelayServer );
    DeclToStringW;
    
public:
	KRelayServer(void); // private 에서 public으로 바꿈
    virtual ~KRelayServer(void);

    bool Init();
    void Run();
    void ShutDown();
    void ToggleLog();

    // for lua
    void SetSendTick( DWORD dwSendTick );
    void GetSimpleInfo( IN OUT std::wostream& stm ) const;
    void SetLogTick( DWORD dwLogTick );
    void DumpAddressList();	
 
protected:
    struct KMaxInfo {
        mutable KncCriticalSection  m_cs;
        CTime                       m_cTime;                // 기록 시간
        DWORD                       m_dwRecvCount;          // Recv한 횟수
        float                       m_fRecvSize;
        DWORD                       m_dwRecvElementCount;   // 실제 받은 요소수
        DWORD                       m_dwIgnoreCount;        // 큐잉되지 못한수
        DWORD                       m_dwSendCount;          // Send한 수
        float                       m_fSendSize;

        KMaxInfo(): m_cTime( CTime::GetCurrentTime() ), m_dwRecvCount(0L), m_dwRecvElementCount(0L),
                    m_dwIgnoreCount(0L), m_dwSendCount(0L), m_fRecvSize(0.f), m_fSendSize(0.f) {}

        KMaxInfo(const KMaxInfo& t) { *this = t; }
        KMaxInfo& operator= (const KMaxInfo& right )
        {
            m_cTime                 = right.m_cTime;
            m_dwRecvCount           = right.m_dwRecvCount;
            m_fRecvSize             = right.m_fRecvSize;
            m_dwRecvElementCount    = right.m_dwRecvElementCount;
            m_dwIgnoreCount         = right.m_dwIgnoreCount;
            m_dwSendCount           = right.m_dwSendCount;
            m_fSendSize             = right.m_fSendSize;

            return *this;
        }
    };

    inline void Recv();
    inline void Send();

    inline void QueueSendData( DWORD dwSenderUID, DWORD* dwReceivers, unsigned char cReceiverNum, bool bRakNetData, const char* buff, USHORT usSize );
    void DumpData( std::wostream& stm ) const;
    
    inline bool IsSamePort( DWORD dwRecvUID, SOCKADDR_IN address );
	// tgkwon 090406
	inline bool GetSendData( std::deque<KSendDataPtr>& m_dequeUserSendData );

    boost::shared_ptr<lua_State>    m_sptlua; 

    SOCKET                          m_sock;
    DWORD                           m_dwSendTick;

    boost::shared_ptr< KTThread<KRelayServer> >     m_spThreadRecv;
    boost::shared_ptr< KTThread<KRelayServer> >     m_spThreadSend;

    mutable KncCriticalSection      m_csAddress;
    std::map<DWORD, KUdpAddrPtr>    m_mapAddress;
    DWORD                           m_dwLastHBCheckTick;

    mutable KncCriticalSection      m_csSendData;
    std::deque<KSendDataPtr>        m_dequeSendData;

    KMaxInfo m_kMaxInfo, m_kCurrentInfo; //현재와 최고의 수치를 가진다.
    DWORD                           m_dwLastLogTick; 
    
    KGenericLogfile                 m_logUDP; // 제네릭 로그 객체

public:    // for lua
    USHORT          m_usPort;
    DWORD           m_dwLogTick; // 로그 기록 시간 간격
};

DeclOstmOperatorA( KRelayServer );
DeclOstmOperatorW( KRelayServer );
