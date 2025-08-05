#pragma once
#include "MRProxy.h"
#include "KNCSingleton.h"
#include <Thread/Locker.h>
#include <map>
#include <ToString.h>
#include "Socket/Session.h"
#include "MSEvent.h"
#include "MSPacket.h"

struct KMRPacket {
    DWORD               m_dwSenderUID;
    std::vector<DWORD>  m_vecRecverUID;
    unsigned short      m_usEventID;
    KSerBuffer          m_kBuff;
    bool                m_bCheckRecv;
    bool                m_bDirectSend;

    void Init()
    {
        m_dwSenderUID = 0;
        m_vecRecverUID.clear();
        m_usEventID = 0;
        m_kBuff.Clear();
        m_bCheckRecv = false;
        m_bDirectSend = false;
    };
};

class KMRConnector : public KThread
{
    DeclToStringW;
    DeclareSingleton( KMRConnector );
public:
    KMRConnector(void);
    virtual ~KMRConnector(void);

    // DB T. 초기 주소설정시 1회 호출.
    void SetMRAddress( IN int iID, IN const char* szIP, IN unsigned short usPort );

    // Main T. 초기 구동시 1회 호출.
    void ConnectAll();

    // MR을 아예 사용하지 않는 경우 주소가 하나도 설정되지 않음.
    bool IsUsingRouter() { KLocker lockMR( m_csMRAddress ); return !m_mapMRAddress.empty(); }
    int GetRouterCount() { KLocker lockMR( m_csMRAddress ); return m_mapMRAddress.size(); }

    void Tick();

    template < typename T > bool SendPacket( DWORD dwUserUID, unsigned short usEventID, T& data );
    template < typename T > void SendToAll( unsigned short usEventID, T& data );

    template < typename T > void PutBuffer( IN const T& data, OUT KSerBuffer& kBuff );

    void DestroyMRProxy( int iID );
    bool ShutdownFinished();
    void StartShutdown();
    int GetOwnerMRid( DWORD dwUserUID );
    virtual void ShutDown();

    template < typename T > void QueueingPacket( DWORD dwSenderUID,
        DWORD dwRecverUID, 
        unsigned short usEventID, 
        T& data, 
        bool bCheckRecv,
        bool bDirectSend );

    template < typename T > void QueueingPacketPack( DWORD dwSenderUID,
        std::vector<DWORD>& vecRecverUIDList, 
        unsigned short usEventID, 
        T& data, 
        bool bCheckRecv,
        bool bDirectSend );

    template < typename T > bool QueueingEventToLoacl( IN DWORD dwRecverUID, IN unsigned short usEventID, IN T& data );
    template < typename T > bool QueueingEventToLoacl( IN OUT std::vector<DWORD>& vecRecverUIDList, IN unsigned short usEventID, IN T& data );

    bool RemoteQueueingPack( DWORD dwSenderUID,
        std::vector<DWORD>& vecRecverUIDList,
        unsigned short usEventID,
        KSerBuffer& kBuff,
        bool bCheckRecv,
        bool bDirectSend );

private:
    void SetAddress( IN const int nID, IN const KServerInfo kInfo ) { KLocker lock( m_csMRAddress ); m_mapMRAddress[nID] = kInfo; }
    bool GetAddress( IN const int nID, OUT KServerInfo& kInfo );
    void GetAllAddress( OUT std::map<int,KServerInfo>& mapInfo )    { KLocker lock( m_csMRAddress ); mapInfo = m_mapMRAddress; }
    void CheckMRConnection( OUT std::vector<int>& vecMRid );

    void QueueingData( IN const KMRPacket& kData ) { KLocker lock( m_csQuePacket ); m_quePacket.push_back( kData ); }
    void SendQueData();

    void GetDestroyMRProxy( OUT std::set<int>& setID )  { KLocker lock( m_csDestroyList ); setID = m_setDestroyList; }
    void ClearDestroyMRProxy()                          { KLocker lock( m_csDestroyList ); m_setDestroyList.clear(); }

protected:
    inline bool Connect( int iID );     // 내부에서만 호출. 해당 아이디의 MR로 접속을 시도한다.
    KMRProxyPtr GetOwnerMRProxy( DWORD dwUserUID );
    virtual void Run();
    void RemoveMRProxy( IN const int iID );

    // MR Proxy로 보내기 위한 패킷Queue
    mutable KncCriticalSection  m_csQuePacket;
    std::deque< KMRPacket >     m_quePacket;

    mutable KncCriticalSection  m_csMRProxy;    // DBLayer, SimLayer
    std::map<int,KMRProxyPtr>   m_mapMRProxy;

    mutable KncCriticalSection  m_csMRAddress;
    std::map<int,KServerInfo>   m_mapMRAddress;     // MR과의 연결이 손실된 경우 자동 복구를 위해 필요.

    mutable KncCriticalSection  m_csDestroyList;
    std::set<int>               m_setDestroyList;

    bool                        m_bOnClosing;       // 서버 종료중이라면, MR로의 자동복구를 해서는 안된다.
    DWORD                       m_dwReconnectTick;  // 일정 주기로 접속상태를 검색해, 재접속을 시도한다.
    static DWORD                ms_dwReconnectGap;  // 접속상태 체크 주기.

};

DefSingletonInline( KMRConnector );

template < typename T >
bool KMRConnector::SendPacket( DWORD dwUserUID, unsigned short usEventID, T& data )
{
    if( !IsUsingRouter() ) return true;

    KMRProxyPtr spProxy = GetOwnerMRProxy( dwUserUID );

    if( !spProxy )
    {
        START_LOG( cerr, L"MR 포인터를 찾을 수 없음" )
            << BUILD_LOG( dwUserUID )
            << BUILD_LOG( usEventID )
            << BUILD_LOG( KMSEvent::GetEventIDString( usEventID ) )
            << BUILD_LOG( GetOwnerMRid( dwUserUID ) ) << END_LOG;

        __LIF( Connect( GetOwnerMRid( dwUserUID ) ) );    // 무효한 연결을 발견하면 재접속 시도.
        return false;
    }

    return spProxy->SendPacket( usEventID, data );
}

template < typename T >
void KMRConnector::SendToAll( unsigned short usEventID, T& data )
{
    KLocker lock( m_csMRProxy );

    std::map<int,KMRProxyPtr>::iterator mit;
    for( mit = m_mapMRProxy.begin(); mit != m_mapMRProxy.end(); mit++ )
    {
        if ( mit->second != NULL ) {
            mit->second->SendPacket( usEventID, data );
        }
    }
}

template < typename T >
void KMRConnector::PutBuffer( IN const T& data_, OUT KSerBuffer& kBuff_ )
{
    kBuff_.Clear();

    KSerializer ks;
    ks.BeginWriting( &kBuff_ );
    ks.Put( data_ );
    ks.EndWriting();
}

template < typename T >
void KMRConnector::QueueingPacket( DWORD dwSenderUID_,
                                   DWORD dwRecverUID_,
                                   unsigned short usEventID_,
                                   T& data_,
                                   bool bCheckRecv_,
                                   bool bDirectSend_ )
{
    // 로컬에서 다 처리했으면 return.
    if ( QueueingEventToLoacl( dwRecverUID_, usEventID_, data_ ) == true ) {
        return;
    }

    KMRPacket kData;
    kData.Init();

    kData.m_dwSenderUID = dwSenderUID_;
    kData.m_vecRecverUID.push_back( dwRecverUID_ );
    kData.m_usEventID = usEventID_;
    PutBuffer( data_, kData.m_kBuff );
    kData.m_bCheckRecv = bCheckRecv_;
    kData.m_bDirectSend = bDirectSend_;

    QueueingData( kData );
}

template < typename T >
void KMRConnector::QueueingPacketPack( DWORD dwSenderUID_,
                                       std::vector<DWORD>& vecRecverUIDList_,
                                       unsigned short usEventID_,
                                       T& data_,
                                       bool bCheckRecv_,
                                       bool bDirectSend_ )
{
    // 로컬에서 다 처리했으면 return.
    if ( QueueingEventToLoacl( vecRecverUIDList_, usEventID_, data_ ) == true ) {
        return;
    }

    KMRPacket kData;
    kData.Init();

    kData.m_dwSenderUID = dwSenderUID_;
    kData.m_vecRecverUID.swap( vecRecverUIDList_ );
    kData.m_usEventID = usEventID_;
    PutBuffer( data_, kData.m_kBuff );
    kData.m_bCheckRecv = bCheckRecv_;
    kData.m_bDirectSend = bDirectSend_;

    QueueingData( kData );
}

template < typename T >
bool KMRConnector::QueueingEventToLoacl( IN DWORD dwRecverUID_, IN unsigned short usEventID_, IN T& data_ )
{
    // 로컬에서 수신 대상자 확인.
    KActorPtr spUser( SiKMSSimLayer()->m_kActorManager.GetByUID( dwRecverUID_ ) );
    if ( !spUser ) {
        return false;
    }

    spUser->QueueingEvent( usEventID_, data_ );
    return true;
}

template < typename T >
bool KMRConnector::QueueingEventToLoacl( IN OUT std::vector<DWORD>& vecRecverUIDList_, IN unsigned short usEventID_, IN T& data_ )
{
    std::vector<DWORD>::iterator vit;

    vit = vecRecverUIDList_.begin();
    while ( vit != vecRecverUIDList_.end() ) {
        // 로컬에서 수신 대상자 확인.
        KActorPtr spUser( SiKMSSimLayer()->m_kActorManager.GetByUID( *vit ) );
        if ( spUser ) {
            spUser->QueueingEvent( usEventID_, data_ );
            vit = vecRecverUIDList_.erase( vit );
            continue;
        }

        ++vit;
    }

    return vecRecverUIDList_.empty();
}