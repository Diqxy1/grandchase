#include <WinSock2.h>
#include "RelayServer.h"

#include "Socket/NetCommon.h"

#include <KncUtil.h>
#include <dbg/dbg.hpp>
#include "RelayServerMgr.h"
#include <boost/algorithm/string.hpp>
#include "Log4.h"

//FILE_NAME_FOR_LOG
#ifdef DEBUG
    #include <vld.h>
    #pragma comment( lib, "vld" )
#endif

#define MINIMUM_PACKET_SIZE  10
#define RECEIVERS_POS  6
#define DATA_HEADER_SIZE 9
#define KBYTE 0.000976562f

NiImplementRootRTTI( KRelayServer );

ImplOstmOperatorW2A( KRelayServer );

ImplToStringW( KRelayServer )
{
    size_t AddressTableSize = 0;
    size_t SendQueueSize = 0;

    {
        KLocker lock( m_csAddress );
        AddressTableSize = m_mapAddress.size();
    }
    {
        KLocker lock( m_csSendData );
        SendQueueSize = m_dequeSendData.size();
    }

    START_TOSTRINGW
        << TOSTRINGW( m_usPort )
        << TOSTRINGW( AddressTableSize )
        << TOSTRINGW( SendQueueSize );

    DumpData( stm_ );

    return stm_;
}
DWORD KRelayServerMgr::g_dwHBTick = 60*1000;

void KRelayServer::GetSimpleInfo( IN OUT std::wostream& stm_ ) const
{
    size_t AddressTableSize = 0;
    size_t SendQueueSize = 0;

    {
        KLocker lock( m_csAddress );
        AddressTableSize = m_mapAddress.size();
    }
    {
        KLocker lock( m_csSendData );
        SendQueueSize = m_dequeSendData.size();
    }

    stm_ << "   Port : " << m_usPort
        << ", AddressTable : " << AddressTableSize
        << ", SendQueue : " << SendQueueSize << std::endl;
}

KRelayServer::KRelayServer(void)
: 
m_sock( INVALID_SOCKET ),
m_usPort( 0 ),
m_dwSendTick( 1 ),
m_dwLogTick( 60*60*1000 )
{
    m_dwLastHBCheckTick = ::GetTickCount();
    m_dwLastLogTick = ::GetTickCount();
}

KRelayServer::~KRelayServer(void)
{
}

ImplOstmOperatorW( KRelayServer );

bool KRelayServer::Init()
{
    START_LOG( cerr, L"Initialize Relay Server" ) << END_LOG;

    // socket init
    _JIF( NetCommon::InitWinSock(), return false );

    m_sock = ::socket( AF_INET, SOCK_DGRAM, 0 );    // 소켓 생성

    if( INVALID_SOCKET == m_sock )
    {
        START_LOG( cerr, NetCommon::GetWsaMsg() ) << END_LOG;
        ::WSACleanup();
        return false;
    }

    SOCKADDR_IN local;
    local.sin_family        = AF_INET;
    local.sin_addr.s_addr   = INADDR_ANY;
    local.sin_port     = htons( m_usPort );
    // m_usPort 를 UDP 포트로 개방한다.
    if( ::bind( m_sock, (SOCKADDR *)&local, sizeof(local) ) == SOCKET_ERROR)
    {
        START_LOG( cerr, NetCommon::GetWsaMsg() ) << END_LOG;
        ::WSACleanup();
        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    // thread setting : recv는 recvfrom() 함수에서 알아서 block된다.
    m_spThreadRecv = boost::shared_ptr< KTThread<KRelayServer> >
        ( new KTThread<KRelayServer>( *this, &KRelayServer::Recv, 0 ) );

    m_spThreadSend = boost::shared_ptr< KTThread<KRelayServer> >
        ( new KTThread<KRelayServer>( *this, &KRelayServer::Send, m_dwSendTick ) );

    LIF( m_logUDP.OpenFile( boost::str( boost::wformat( L"%d" )  %m_usPort ) ) ); // 하드한게 싫다면 수정하기	

    return true;
}

void KRelayServer::Run()
{
    START_LOG( cerr, L"Relay Server Start." ) << END_LOG;

    std::cout << *this << std::endl;

    m_spThreadRecv->Begin();
    m_spThreadSend->Begin();
}

void KRelayServer::ShutDown()
{
    START_LOG( cerr, L"Relay Server End." ) << END_LOG;

    std::cout << *this << std::endl;

    CLOSE_SOCKET( m_sock );     // blocked mode. 소켓을 먼저 닫아 스레드가 스스로 종료되도록 유도한다. (깔끔하지 못함.)

    if( m_spThreadRecv )
        m_spThreadRecv->End( 3000 );    // 현재 소켓이 blocked 이므로, 무조건 timeout 된다.

    if( m_spThreadSend )
        m_spThreadSend->End( 10000 );

    ::WSACleanup();

    m_logUDP.CloseFile();
}

void KRelayServer::Recv()
{
    int ret;
    char recvbuff[MAX_UDP_PACKET_SIZE] = {0};
    SOCKADDR_IN sender;
    int nSenderSize = sizeof(sender);

    ret = ::recvfrom( m_sock,
        recvbuff,
        MAX_UDP_PACKET_SIZE,
        0,
        (SOCKADDR*)&sender,
        &nSenderSize );

    if( ret == SOCKET_ERROR )
    {
        START_LOG( cwarn, NetCommon::GetWsaMsg() ) << END_LOG;
        return;
    }

    if( ret <= MINIMUM_PACKET_SIZE )
    {
        START_LOG( cwarn, L"데이터의 길이가 MINIMUM_PACKET_SIZE 보다 작거나 같음. size : " << ret )
            << BUILD_LOG( MINIMUM_PACKET_SIZE )
            << BUILD_LOG( inet_ntoa(sender.sin_addr) )
            << BUILD_LOG( ntohs(sender.sin_port) )
            << BUILD_LOG( (unsigned int)recvbuff[0] )   // unsigned char로 출력하면 문자로 찍힌다.
            << END_LOG;
        return;
    }

    /*
    Recv Packet 의 모양새.
    bool  bdata
    DWORD SenderID
    uchar userNumber
    DWORD RecvIDs[userNumber]
    DATA
    */

    unsigned char cReceiverNum = 0;
    bool bRakNetData = false;
    DWORD dwSenderUID = 0;

    ::memmove( &bRakNetData, recvbuff, sizeof(bool) );
    ::memmove( &dwSenderUID, &recvbuff[1], sizeof(DWORD) );
    ::memmove( &cReceiverNum, &recvbuff[5], sizeof(unsigned char) );
    
    //////////////////////////////////////////////////////////////////////////
    // 주소 리스트에 존재한다면 tick을 갱신하고, 존재하지 않는다면 추가한다.
    {
        KLocker lock( m_csAddress );		
        std::map<DWORD,KUdpAddrPtr>::iterator mit;
        mit = m_mapAddress.find( dwSenderUID );

        if( mit == m_mapAddress.end() )
        {
            KUdpAddrPtr spAddr( new KUdpAddr );
            spAddr->m_dwUID             = dwSenderUID;
            spAddr->m_sockaddr          = sender;
            spAddr->m_dwLastRecvTick    = ::GetTickCount();

            m_mapAddress.insert( std::make_pair( spAddr->m_dwUID, spAddr) );

            START_LOG( clog, L"주소리스트에 UID 추가. uid:" << spAddr->m_dwUID )
                << BUILD_LOG( inet_ntoa(spAddr->m_sockaddr.sin_addr) )
                << BUILD_LOG( ntohs(spAddr->m_sockaddr.sin_port) )
                << BUILD_LOG( ret )
                << BUILD_LOG( m_mapAddress.size() ) << END_LOG;
        }
        else
        {
            mit->second->m_sockaddr = sender ;
            mit->second->m_dwLastRecvTick = ::GetTickCount();

            START_LOG( clog, L"from [" << inet_ntoa(mit->second->m_sockaddr.sin_addr)     // ip
                << L":" << ntohs(mit->second->m_sockaddr.sin_port) ) << END_LOG;          // port
        }

    } // release m_csAddress.

    int temp = ( sizeof(DWORD) * cReceiverNum );
    
    int nRealDataPos = RECEIVERS_POS + ( sizeof(DWORD) * cReceiverNum );
    QueueSendData( dwSenderUID, (DWORD*)&recvbuff[ RECEIVERS_POS ], cReceiverNum, bRakNetData, &recvbuff[nRealDataPos], ret - nRealDataPos );
    //QueueSendData( dwSenderUID, recvbuff, ret );
    
    KLocker lockInfo( m_kCurrentInfo.m_cs );
    m_kCurrentInfo.m_fRecvSize += (ret*KBYTE);	
}

//void KRelayServer::QueueSendData( DWORD dwSenderUID_, const char* buff_, USHORT usSize_ )
void KRelayServer::QueueSendData( DWORD dwSenderUID_, DWORD* dwReceivers_, unsigned char cReceiverNum_, bool bRakNetData_, const char* buff_, USHORT usSize_ )
{
    JIF( buff_ != NULL );
    JIF( dwReceivers_ != NULL );
    JIF( cReceiverNum_ >= 1 );
    JIF( usSize_ <= MAX_UDP_PACKET_SIZE - DATA_HEADER_SIZE );

    if( usSize_ >  (1024 * 2) - DATA_HEADER_SIZE &&
        usSize_ <= (1024 * 5) - DATA_HEADER_SIZE )
    {
        START_LOG( cerr, L"size 2048~5120. usSize_ : " << usSize_ ) << END_LOG;
    }
    else if( usSize_ >  (1024 * 5) - DATA_HEADER_SIZE &&
        usSize_ <= MAX_UDP_PACKET_SIZE - DATA_HEADER_SIZE )
    {
        START_LOG( cerr, L"size 5120~11264. usSize_ : " << usSize_) << END_LOG;
    }
    else if( usSize_ > MAX_UDP_PACKET_SIZE - DATA_HEADER_SIZE )
    {
        START_LOG( cerr, L"size over 11264. usSize_ : " << usSize_) << END_LOG;
        return;
    }

/// tgkwon 090406
	DWORD dwIgnore = 0;
	DWORD dwRecvElement = 0;
	KSendDataPtr spData( new KSendData );

	for( int i = 0 ; i < cReceiverNum_ ; ++i ) //받는 사람의 수만큼 Loop를 돌면서 큐잉한다.
	{
		++dwRecvElement;
		{   // 받은 사람의 address를 얻어낸다.
			KLocker lock( m_csAddress );
			std::map<DWORD,KUdpAddrPtr>::iterator mit;
			mit = m_mapAddress.find( dwReceivers_[i] );
			if( mit == m_mapAddress.end() )
			{
				START_LOG( cwarn, L"받는 사람 " << dwReceivers_[i] << L" 이 주소리스트에 없음. #list:"
					<< m_mapAddress.size() << L",sender:" << dwSenderUID_ << L",#data:" << usSize_ )
                    << END_LOG;;
				++dwIgnore;
				continue;
			}
			else
			{				
				spData->m_vecUserInfo.push_back(std::make_pair(dwReceivers_[i], mit->second->m_sockaddr));
			}
		}
	}
	// 임계구역이 서로 교차되지 않도록 미리 풀어준다.
	spData->m_usDataSize  = usSize_ + DATA_HEADER_SIZE;

	::memmove( spData->m_buff, &bRakNetData_, sizeof( bool ) );
	::memmove( &spData->m_buff[1], &dwSenderUID_, sizeof( DWORD ) );
	::memmove( &spData->m_buff[5], &dwReceivers_[0], sizeof( DWORD ) );
	::memmove( &spData->m_buff[DATA_HEADER_SIZE], buff_, usSize_ );

	{
		KLocker lock( m_csSendData );
		m_dequeSendData.push_back( spData );		
	}

    KLocker lockInfo( m_kCurrentInfo.m_cs );
    m_kCurrentInfo.m_dwIgnoreCount += dwIgnore;
    m_kCurrentInfo.m_dwRecvCount++;
    m_kCurrentInfo.m_dwRecvElementCount += dwRecvElement;
}

void KRelayServer::Send()
{
    // 전송해야 할 데이터가 있으면 이를 모두 처리한다.
    int ret = SOCKET_ERROR; // SOCKET_ERROR = -1    
    KSendDataPtr spData;

/// tgkwon 090406
    std::deque<KSendDataPtr> m_queueUserSendData;
    std::deque<KSendDataPtr> ::iterator dit;
    if( !GetSendData( m_queueUserSendData ) ) goto send_empty;	

    for( dit = m_queueUserSendData.begin(); dit != m_queueUserSendData.end(); ++dit )
    {
        spData = *dit;

        std::vector< std::pair<DWORD, SOCKADDR_IN> > ::iterator vit;
        for( vit = spData->m_vecUserInfo.begin(); vit != spData->m_vecUserInfo.end(); ++vit )
        {
            if( !IsSamePort( vit->first, vit->second ) )
                continue;

			if( vit == spData->m_vecUserInfo.begin() )
			{
				// 전송이 성공하면 전송한 size를 돌려준다.
				ret = ::sendto( m_sock,
					spData->m_buff,
					spData->m_usDataSize,
					0,
					(SOCKADDR*)&vit->second,
					sizeof(vit->second) );
			}
			else
			{
				char recvbuff[MAX_UDP_PACKET_SIZE] = {0};
				memmove(&recvbuff, &spData->m_buff, sizeof(bool));
				memmove(&recvbuff[1], &spData->m_buff[1], sizeof(DWORD));
				memmove(&recvbuff[5], &vit->first, sizeof(DWORD));
				memmove(&recvbuff[DATA_HEADER_SIZE], &spData->m_buff[DATA_HEADER_SIZE], (spData->m_usDataSize - DATA_HEADER_SIZE) );

				// 전송이 성공하면 전송한 size를 돌려준다.
				ret = ::sendto( m_sock,
					recvbuff,
					spData->m_usDataSize,
					0,
					(SOCKADDR*)&vit->second,
					sizeof(vit->second) );
			}

			if( ret == SOCKET_ERROR )
			{
				START_LOG( cerr, NetCommon::GetWsaMsg() )
					<< BUILD_LOG( inet_ntoa(vit->second.sin_addr) )
					<< BUILD_LOG( ntohs(vit->second.sin_port) )
					<< END_LOG;
			}
			else
			{
				START_LOG( clog, L"To   [" << inet_ntoa(vit->second.sin_addr) << L":" << ntohs(vit->second.sin_port)
					<< L"] size:" << spData->m_usDataSize ) << END_LOG;

				KLocker lockInfo( m_kCurrentInfo.m_cs );
				m_kCurrentInfo.m_dwSendCount++;
				m_kCurrentInfo.m_fSendSize += (spData->m_usDataSize*KBYTE);				
			}
		}
	}

send_empty:
    // 일정 시간동안 Recv받지 않은 주소는 정리한다.
    if( ::GetTickCount() - m_dwLastHBCheckTick > KRelayServerMgr::g_dwHBTick / 2 )
    {
        KLocker lock( m_csAddress );

        std::map<DWORD,KUdpAddrPtr>::iterator mit;
        for( mit = m_mapAddress.begin(); mit != m_mapAddress.end(); /* mit++ 중간에 element를 삭제함. */ )
        {
            if( ::GetTickCount() - mit->second->m_dwLastRecvTick > KRelayServerMgr::g_dwHBTick ) // 시간 경과
            {
                START_LOG( clog, L"address deleted. [" << inet_ntoa(mit->second->m_sockaddr.sin_addr)    // ip
                    << L":" << ntohs(mit->second->m_sockaddr.sin_port)                                   // port
                    << L"] id:" << mit->second->m_dwUID                                      // id
                    << L", tick:" << ::GetTickCount() - mit->second->m_dwLastRecvTick ) 
                    << END_LOG;     // tick.

                mit = m_mapAddress.erase( mit );
                continue;
            }
            else
            {
                mit++;
            }
        }  // for

        m_dwLastHBCheckTick = ::GetTickCount();

        // Max Info
        KLocker lockInfoCurrent( m_kCurrentInfo.m_cs );
        KLocker lockInfoMax( m_kMaxInfo.m_cs );
        if( m_kMaxInfo.m_dwRecvElementCount < m_kCurrentInfo.m_dwRecvElementCount )
        {
            m_kMaxInfo = m_kCurrentInfo;
        }		
        m_kCurrentInfo = KMaxInfo();
    } // if

    if( ::GetTickCount() - m_dwLastLogTick > KRelayServer::m_dwLogTick )
    {
        std::wstringstream stm;
        stm << *this;
        std::wstring strTmp = boost::replace_all_copy( stm.str(), L"\n", L"\r\n" );
        m_logUDP << strTmp;
        m_dwLastLogTick = ::GetTickCount();
    }
}

/// tgkwon 090406
bool KRelayServer::GetSendData( std::deque<KSendDataPtr>& m_dequeUserSendData_ )
{
	KLocker lock( m_csSendData );
	if( m_dequeSendData.empty() ) return false;
	m_dequeSendData.swap( m_dequeUserSendData_ );
	return true;
}

bool KRelayServer::IsSamePort( DWORD dwRecvUID_, SOCKADDR_IN address_ )
{
    std::map<DWORD, KUdpAddrPtr>::iterator mit;
    KLocker lock( m_csAddress );
    mit = m_mapAddress.find( dwRecvUID_ );
    if( mit == m_mapAddress.end() ) return false;

    if( mit->second->m_sockaddr.sin_port != address_.sin_port )
    {
		START_LOG( cerr, L" Corrupt Address. UID : " << dwRecvUID_ << dbg::endl
			<< L" Origin [" << inet_ntoa(address_.sin_addr) << L":" << ntohs(address_.sin_port) << L"]" << dbg::endl
            << L" Now [" << inet_ntoa(mit->second->m_sockaddr.sin_addr) << L":" << ntohs(mit->second->m_sockaddr.sin_port) << L"]" )
            << END_LOG;
        return false;
    }

    return true;
}

void KRelayServer::SetSendTick( DWORD dwSendTick_ )
{
    START_LOG( cerr, L"Change m_dwSendTick from " << m_dwSendTick << " to " << dwSendTick_ ) << END_LOG;

    m_dwSendTick = dwSendTick_;

    if( m_spThreadSend )
    {
        m_spThreadSend->SetTimeout( m_dwSendTick );
        std::cout << *this;
    }
}

void KRelayServer::DumpData( std::wostream& stm_ ) const
{
    stm_ << std::endl << L"-- Max Info --" << std::endl;
    {

        KLocker lock( m_kMaxInfo.m_cs );
        stm_    << L"  Time             : " << (const wchar_t*)(CStringW)m_kMaxInfo.m_cTime.Format( KNC_TIME_FORMAT ) << std::endl
            << L"  RecvCount        : " << m_kMaxInfo.m_dwRecvCount << std::endl
            << L"  RecvSize         : " << m_kMaxInfo.m_fRecvSize << " (KB)" << std::endl
            << L"  RecvElementCount : " << m_kMaxInfo.m_dwRecvElementCount << std::endl
            << L"  IgnoreCount      : " << m_kMaxInfo.m_dwIgnoreCount << std::endl
            << L"  SendCount        : " << m_kMaxInfo.m_dwSendCount << std::endl
            << L"  SendSize         : " << m_kMaxInfo.m_fSendSize << " (KB)" << std::endl;
    }

    stm_ << std::endl << L"-- Current Info --" << std::endl;
    {

        KLocker lock( m_kCurrentInfo.m_cs );
        stm_    << L"  Time             : " << (const wchar_t*)(CStringW)m_kCurrentInfo.m_cTime.Format( KNC_TIME_FORMAT ) << std::endl
            << L"  RecvCount        : " << m_kCurrentInfo.m_dwRecvCount << std::endl
            << L"  RecvElementCount : " << m_kCurrentInfo.m_dwRecvElementCount << std::endl
            << L"  RecvSize         : " << m_kCurrentInfo.m_fRecvSize << " (KB)" << std::endl
            << L"  IgnoreCount      : " << m_kCurrentInfo.m_dwIgnoreCount << std::endl
            << L"  SendCount        : " << m_kCurrentInfo.m_dwSendCount << std::endl
            << L"  SendSize         : " << m_kCurrentInfo.m_fSendSize << " (KB)" << std::endl;
    }

}

void KRelayServer::ToggleLog()
{
    bool bOld = m_logUDP.GetWriteEnable();
    m_logUDP.ToggleWrite();
    START_LOG( cerr, L" Toggle Log (" << m_usPort << L") "
        << (bOld ? L"true" : L"false") <<  L" -> " 
        << (m_logUDP.GetWriteEnable() ? L"true" : L"false" ) ) 
        << END_LOG;
}

void KRelayServer::SetLogTick( DWORD dwLogTick_ )
{
    START_LOG( cerr, L"Change m_dwLogTick from " << m_dwLogTick << " to " << dwLogTick_ ) << END_LOG;

    m_dwLogTick = dwLogTick_;
}

void KRelayServer::DumpAddressList()
{
    std::wstringstream stm;
    stm << L" -- Dump User Address -- " << std::endl;
    stm << L" Server Port : " << m_usPort << std::endl;
    std::map<DWORD, KUdpAddrPtr>::iterator mit;
    {
        KLocker lock( m_csAddress );
        for( mit = m_mapAddress.begin() ; mit != m_mapAddress.end() ; ++mit )
        {
            stm << L"UID : " << mit->second->m_dwUID
                << L", AddRess : " << inet_ntoa(mit->second->m_sockaddr.sin_addr )
                << L"[" << ntohs(mit->second->m_sockaddr.sin_port) << L"]" << std::endl;
        }
    }
    std::wcout << stm.str() << std::endl << L"---- End Dump ----" << std::endl;
}

//#undef MAX_UDP_PACKET_SIZE
