#pragma once
#include <WinSock2.h>
#include "Performer.h"
#include "Thread/Thread.h"
#include <Serializer/Serializer.h>
#include <Serializer/SerBuffer.h>
#include "SocketObject.h"
#include "KncSecurity/KncSecurity.h"
#include "KncException.h"

class KSession : public KPerformer, public KThread
{
    NiDeclareRTTI;
    DeclToStringW;

    KSession(); // 구현부는 없음. 사용되면 안된다. (사용하면 링크에러남.)
    KSession(const KSession& right);
    KSession& operator=(const KSession& right);

public:
    enum {
        EVENT_HEART_BIT             = 0,
        EVENT_ACCEPT_CONNECTION_NOT = 1,    // custom event ID들은 2부터 시작한다.
        SECURE_KEY_WAITING_TIME     = 3000, // 접속시 보안키를 받기까지 대기하는 최대시간
    };

    KSession( bool bIsProxy );
	virtual ~KSession(void);

    // derived from KThread
    virtual bool Begin();

    // derived from KPerformer
    virtual void Tick();    // reserver destroy, check heartbit

    void ReserveDestroy()       { m_bDestroyReserved = true; }     // OnDestory 예약

    // 060321. florist. RefCount가 남아있어 삭제되지 않을 가능성이 있는 경우를 검사한다.
    virtual bool CheckExceedRefCount( int nCount );

    template < typename T > 
    bool SendPacket( unsigned short usEventID, T& data, bool bLogging = true, bool bCompress = false );
    bool SendPacket( KSerBuffer& kbuff, const wchar_t* szDataDesc );
    bool SendID( unsigned short usEventID );
    //bool InitRecv();

    bool IsConnected() const			{ return m_kSockObj.IsConnected(); }
    unsigned int GetIP() const			{ return m_kSockObj.GetIP(); }
    const char* GetIPStr() const		{ return m_kSockObj.GetIPStr(); }
    unsigned short GetPort() const		{ return m_kSockObj.GetPort(); }
    KSocketObject* GetSockObj()         { return &m_kSockObj; }

    static void DumpMaxSendData( std::wostream& stm );
    static void ResetMaxSendData();

    // use proxy-child only.
    virtual bool Init( bool bUseIocp );
    virtual bool Connect( const char* szIP, unsigned short usPort, bool bNagleAlgOn );

    // user unproxy-child only.
    virtual void OnAcceptConnection(); // accept 되자마자 호출. 이곳에서 인증키를 생성해 클라이언트로 전송한다.

protected:  // util function
	//void OnSendCompleted( DWORD dwTransfered );
    //void OnRecvCompleted( DWORD dwTransfered );
    void OnRecvCompleted( IN const KncSecurity::KByteStream& payload );
    // derived from KThread
    virtual void Run();
    // derive to child.
    virtual void OnSocketError( int nType );   ///< 050129. WSASend, IOCP등에서 소켓 오류를 발견한 경우 호출.
    virtual void OnDestroy();       ///< 050129. multi thread safe하도록 수정함

    class KSkSession : public KSocketObject {
    public:
        void OnRecvCompleted( DWORD dwTransfered );
        void OnSocketError( int nType ) { m_pkSession->OnSocketError( nType ); }
        void OnAcceptConnection() { m_pkSession->OnAcceptConnection(); }
        KSession* m_pkSession;
    } m_kSockObj;
public:
    static DWORD        m_dwHBGap;          // 061024. kkurrung. 네트웍 환경이 좋지 못한 지역 서비스시 HB 간격 조절

protected:
    bool                m_bUseIocp;     ///< if use Iocp
    bool				m_bIsProxy;		///< Proxy인 경우, H.B를 보내고, Connect를 할 수 있다.
    DWORD               m_dwHBTick;     // heart bit tick    
    bool                m_bDestroyReserved;     ///< 050129. 생성한 스레드에서 소멸 처리.
    bool                m_bZombieDestroy;   ///< 060913. Zombie로 죽은경우 체크.(Game Server에서 Zombie Count를 하기위해서.)

    static struct KMaxPacketInfo {
        KncCriticalSection  m_cs;           // lua script에서의 작업에 대해 보호한다
        std::wstring        m_strName;
        std::wstring        m_strDataDesc;
        size_t              m_nDataSize;    // 순수 게임 데이터만 고려된 사이즈
        size_t              m_nPacketSize;  // 암호화 처리를 거친 후, 실제 전송되는 데이터 사이즈
        CTime               m_cTime;
    } ms_kMaxPacketInfo;

    // proxy-child only.
    enum ENUM_EVENT_TYPE {    
        EVENT_DISCONNECTED,
        EVENT_RECV_COMPLETED,        
        EVENT_SEND_COMPLETED,        
        EVENT_MAX_VALUE
    };
    HANDLE              m_hEvents[EVENT_MAX_VALUE];

    friend class KIOThread;         // access On(Recv/Send)oCompleted()

    bool                m_bAuthKeyRecved;   // 보안 키 받았는지 확인 하는 변수.
    SpiType             m_nSPIndex;
    bool                m_bCheckHBTick;     // florist. debug의 편의를 위해 hb 체크 on/off를 가능하게 함.
};

template < typename T > 
bool KSession::SendPacket( unsigned short usEventID_, T& data_, bool bLogging_, bool bCompress_ )
{
    KEventPtr spEvent( CreateKEvent() );
    spEvent->m_usEventID = usEventID_;

    // make event object
    KSerializer         ks;

    // serialize - only data
    ks.BeginWriting( &spEvent->m_kbuff );
    ks.Put( data_ );
    ks.EndWriting();

    if( bCompress_ ) spEvent->m_kbuff.Compress();   // 압축할 경우, KEvent의 buff만을 압축한다.

    // serialize - full event object
    KSerBuffer          kbuff;
    ks.BeginWriting( &kbuff );
    ks.Put( *spEvent );
    ks.EndWriting();

    // send packet
    if( ! KSession::SendPacket( kbuff, spEvent->GetEventIDString() ) )
        return false;

    if( bLogging_ )
        dbg::clog << L"ⓢ " << spEvent->GetEventIDString() << L" (name:" << m_strName << L")" << END_LOG;

    return true;
}

#define SEND_ID( id )               LIF( SendID( EVENT_TYPE::##id ) )
#define SEND_PACKET( id )           LIF( SendPacket( EVENT_TYPE::##id, kPacket ) )
#define SEND_RECEIVED_PACKET( id )  LIF( SendPacket( EVENT_TYPE::##id, kPacket_) )
