#include "../GameServer/GSSimLayer.h"
#include "CYOUBilling.h"
#include "Socket/NetCommon.h"
#include "Log4.h"
#include "NUserPacket.h"
#include "../GameServer/GameServer.h"
#include "../GameServer/support_User.h"
#include "NetError.h"
#include "LogManager.h"

//#pragma comment( lib, "../Common/CYOUBilling/PCI" ) // hshield.

#define CLASS_TYPE CYOUBilling

/*CYOUBilling::KCommonData CYOUBilling::ms_ComnData =
{
	KncCriticalSection(),
	std::vector< std::pair<std::string, USHORT> >(),
	0
};

USHORT CYOUBilling::m_usPacketSequence = 0;
std::wstring CYOUBilling::ms_wstrFileDSN_forCashDB;

CYOUBilling::CYOUBilling(void)
{
	CYOUBillingEvent::Init();

	m_dwWaitTime = 20000; // 페이레터 60초, 창유는 몇 초?
	m_dwThreadID = 0;

	InitResultData();

	KLocker lock( ms_ComnData.m_cs );

	//for ( int i = 0; i < NUM_PORT_TYPE; i++ ) {
	//    m_kSock[i].m_pkCYOUBilling = this;
	//    m_kSock[i].SetKey( i );
	//    m_kSock[i].m_dwLastHeartBeatTime = ::GetTickCount();
	//    m_hEvents[i * 2 + 0] = CreateEvent( NULL, false, false, NULL ); // 역할은? IO_RECV
	//    m_hEvents[i * 2 + 1] = CreateEvent( NULL, false, false, NULL ); // IO_SEND
	//    m_kSock[i].SetIOEvent( KOVERLAPPED::IO_RECV, m_hEvents[i * 2 + 0] );
	//    m_kSock[i].SetIOEvent( KOVERLAPPED::IO_SEND, m_hEvents[i * 2 + 1] );

	//    LIF( m_kSock[i].Connect( ms_ComnData.m_vecBillingServerIP_Port[ms_ComnData.m_nCurrentIndex].first.c_str(), ms_ComnData.m_vecBillingServerIP_Port[ms_ComnData.m_nCurrentIndex].second, false ) );

	//    m_hEndEvent[i] = CreateEvent( NULL, false, false, NULL ); // EndEvent의 역할은?
	//}
	KBillDB::DriveConnect();
	DriveConnect_forCashDB();
}

CYOUBilling::~CYOUBilling(void)
{
	End( 15000 ); // 스레드 종료를 15초 기다려줌?
	int i = 0;
	for( i = 0; i < NUM_PORT_TYPE * 2; i++ )
	{
		::CloseHandle( m_hEvents[i] ); // 2개의 이벤트
		m_hEvents[i] = NULL;
	}

	for( i = 0; i < NUM_PORT_TYPE; i++ )
	{
		::CloseHandle( m_hEndEvent[i] ); // 1개의 종료 이벤트?
		m_hEndEvent[i] = NULL;
	}
}

bool CYOUBilling::DriveConnect_forCashDB()
{
	return m_kCashODBC.DriverConnect( ms_wstrFileDSN_forCashDB.c_str() );
}

void CYOUBilling::InitFileDSN_CASH_DB( const char* szFileDSN_ )
{
	ms_wstrFileDSN_forCashDB = KncUtil::toWideString( szFileDSN_ );
}

void CYOUBilling::InitResultData()
{
	m_kResultEvent.Reset();
}

bool CYOUBilling::Begin()
{
	return KThread::Begin();
}

void CYOUBilling::Run()
{
	std::cout << "KSubThread::Run(), thread start." << std::endl;
	m_dwThreadID = ::GetCurrentThreadId();

	DWORD ret;

	while( true )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 1 );

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT )
		{
			for( int i = 0; i < NUM_PORT_TYPE; i++ )
			{
				KeepAliveNot( ( PORT_TYPE )i );
			}
			Loop();
		}

		else std::cout << "*** WaitForSingleObject() - return :" << ret << std::endl;
	}// while
}

void CYOUBilling::InitCYOUBillingInfo( const char* szIP_, USHORT usPort_, const char* szODBC_, const char* szCashODBC_, const USHORT usZoneID_, const USHORT usGroupID_ )
{
	CYOUBilling::InitFileDSN( szODBC_ );
	CYOUBilling::InitFileDSN_CASH_DB( szCashODBC_ );
	KLocker lock( ms_ComnData.m_cs );
	ms_ComnData.m_vecBillingServerIP_Port.push_back( std::make_pair( szIP_, usPort_ ) );
	ms_ComnData.m_usZoneID = usZoneID_;
	ms_ComnData.m_usGroupID = usGroupID_;
}

void CYOUBilling::ProcessIntEvent( const KIntEvent& kIntEvent_ )
{
	KSerializer ks;
	ks.BeginReading( const_cast<KSerBuffer*>( &kIntEvent_.m_kbuff) );

	switch( kIntEvent_.m_usEventID )
	{
		_INT_CASE( EVENT_SERVER_LOGIN_REQ, KCYOUServerInfo );
		_INT_CASE( EVENT_SERVER_HEART_BEAT_REQ, KCYOUServerInfo );
		_INT_CASE( EVENT_CYOU_USER_HEART_BEAT_REQ, KCYOUAuthUserInfo );
		_INT_CASE( EVENT_CYOU_USER_LOGOUT_REQ, KCYOUAuthUserInfo );
		_INT_CASE( EVENT_VERIFY_ACCOUNT_REQ, KCYOUAuthUserInfo );
		_INT_CASE( EVENT_CYOU_SERVER_RESTART_REQ, KCYOUServerInfo );
		_INT_CASE( EVENT_CYOU_USER_SWITCH_CHARACTER_REQ, KCYOUAuthUserInfo );
		_INT_CASE( EVENT_CYOU_CASH_CHANGE_REQ, KCYOUCashData );
		 INT_CASE( EVENT_BUY_FOR_CASH_REQ );
		_INT_CASE( EVENT_CURRENT_CASH_POINT_REQ, KCashInfo );

// #END_OF_CYOU_CASE
// file:.\CYOUBilling.h#END_OF_CYOU_DECL

	default:
		START_LOG( cerr, L"Invalid Event ID : " << kIntEvent_.m_usEventID  ) << END_LOG;
		break;
	}
	START_LOG( clog, L"CYOU Billing Event string : " << KUserEvent::GetEventIDString( kIntEvent_.m_usEventID ) )
		<< END_LOG;
}

bool CYOUBilling::WaitEvents( IN const PORT_TYPE ePortType_, IN const std::wstring& strLogin_ )
{
	DWORD dwRet;
	DWORD dwTotalElapsedTime = ::GetTickCount();
	do
	{
		do
		{
			dwRet = WaitForSingleObject( m_hEvents[ ePortType_ * 2 + 1 ], m_dwWaitTime );
			// ePortType_ * 2 + 1의 의미 ->  IO_SEND  // m_hEvents[i * 2 + 1]
			if ( dwRet == WAIT_TIMEOUT ) {
				break;
			}
			m_kSock[ePortType_].OnIOCompleted( KOVERLAPPED::IO_SEND );
		} while ( m_kSock[ePortType_].IsSending() ); //send가 완료될 때까지 돈다.

		if ( dwRet == WAIT_TIMEOUT ) break;

		do
		{  // 071108. woosh.   m_hEvents[i * 2 + 0] : IO_RECV
			dwRet = WaitForSingleObject( m_hEvents[ ePortType_ * 2 ], m_dwWaitTime );
			if ( dwRet == WAIT_TIMEOUT ) {
				break;
			}
			m_kSock[ePortType_].OnIOCompleted( KOVERLAPPED::IO_RECV );

			dwRet = WaitForSingleObject( m_hEndEvent[ ePortType_ ], 1 );

			if ( dwRet == WAIT_OBJECT_0 ) break;
		} while( dwRet == WAIT_TIMEOUT );

		if ( dwRet == WAIT_TIMEOUT ) break;

		if ( ::GetTickCount() - dwTotalElapsedTime > TIME_OUT_ELAPSED_TIME_LIMIT ) {
			dwRet = WAIT_TIMEOUT;
			break;
		}
	}while(false);

	dwTotalElapsedTime = ::GetTickCount() - dwTotalElapsedTime;

	if ( dwRet == WAIT_TIMEOUT ) { // 시간 초과
		START_LOG( cerr, L"Event Processing - time over. elapsed : " << m_dwWaitTime )
			<< BUILD_LOG( strLogin_ )
			<< END_LOG;

		// Event reset.
		ResetEvent( m_hEvents[ ePortType_ * 2 + 1 ] );
		ResetEvent( m_hEvents[ ePortType_ * 2 ] );
		ResetEvent( m_hEndEvent[ ePortType_ ] );

		for ( int i = 0; i < NUM_PORT_TYPE; i++ ) {
			m_kSock[i].CancelIo();
			m_kSock[i].OnSocketError();
		}
		InitResultData();
		START_LOG( cerr, L"CYOU Billing ReConnection." ) << END_LOG;
		return false;
	}
	return true;
}

bool CYOUBilling::WaitSend( IN const PORT_TYPE ePortType_, IN const std::wstring& strEvent_ )
{ // 보내기만 하는 용도?
	DWORD dwRet;
	DWORD dwTotalElapsedTime = ::GetTickCount();

	do
	{
		do
		{
			dwRet = WaitForSingleObject( m_hEvents[ ePortType_ * 2 + 1 ], m_dwWaitTime );
			if ( dwRet == WAIT_TIMEOUT ) {
				break;
			}
			m_kSock[ePortType_].OnIOCompleted( KOVERLAPPED::IO_SEND );
		} while( m_kSock[ePortType_].IsSending() ); //send 완료할 때까지 도세요

		if ( dwRet  == WAIT_TIMEOUT ) break;
	} while(false);

	dwTotalElapsedTime = ::GetTickCount() - dwTotalElapsedTime;

	if ( dwRet == WAIT_TIMEOUT ) {// 시간 초과
		START_LOG( cerr, L"Event processing - time over. elapsed : " << dwTotalElapsedTime )
			<< BUILD_LOG( strEvent_ )
			<< END_LOG;

		//이벤트 리셋.
		ResetEvent( m_hEvents[ ePortType_ * 2 + 1 ] );
		ResetEvent( m_hEvents[ ePortType_ * 2 ] );
		ResetEvent( m_hEndEvent[ ePortType_ ] );

		for ( int i = 0; i < NUM_PORT_TYPE; i++ ) {
			m_kSock[i].CancelIo();
			m_kSock[i].OnSocketError();
		}
		InitResultData();
		START_LOG( cerr, L"CYOU Billing ReConnection" ) << END_LOG;
		return false;
	}
	return true;
}

void CYOUBilling::KSocketCYOUBilling::OnRecvCompleted( DWORD dwTransfered_ )
{
	if( dwTransfered_ == 0 )
	{
		START_LOG( clog, L"closed by remote machine" ) << END_LOG;
		OnSocketError();
		return;
	}

	START_LOG( clog, L"패킷 받음.")
		<< BUILD_LOG( dwTransfered_ )
		<< BUILD_LOG( m_pkCYOUBilling->m_kResultEvent.GetPacketSize() ) << END_LOG;

	m_ovlRecv.m_dwLeft += dwTransfered_;

	{
		m_pkCYOUBilling->m_kResultEvent.SetInputStream( m_ovlRecv.m_pBuffer );
		m_pkCYOUBilling->m_kResultEvent.SetPacketHeader( ( BYTE* )m_ovlRecv.m_pBuffer );
		m_ovlRecv.m_dwLeft -= ( DWORD )(HEADER_SIZE_OF_CYOUBILLING); // 헤더 추출한 나머지 데이터량

		::memmove( m_ovlRecv.m_pBuffer, &m_ovlRecv.m_pBuffer[HEADER_SIZE_OF_CYOUBILLING], m_ovlRecv.m_dwLeft );

		if( m_ovlRecv.m_dwLeft >= ( DWORD )( m_pkCYOUBilling->m_kResultEvent.GetPacketSize() - static_cast<unsigned short>(HEADER_SIZE_OF_CYOUBILLING) ) )
		{
			m_pkCYOUBilling->m_kResultEvent.CopyToBuffer( ( BYTE* )m_ovlRecv.m_pBuffer, HEADER_SIZE_OF_CYOUBILLING, m_pkCYOUBilling->m_kResultEvent.GetPacketSize() - HEADER_SIZE_OF_CYOUBILLING );

			m_ovlRecv.m_dwLeft -= ( DWORD )( m_pkCYOUBilling->m_kResultEvent.GetPacketSize() - HEADER_SIZE_OF_CYOUBILLING );
			// 아래 문장은 루프 돌지 않는 현재 상태에서는 무의미하다.
			::memmove( m_ovlRecv.m_pBuffer, &m_ovlRecv.m_pBuffer[m_pkCYOUBilling->m_kResultEvent.GetPacketSize() - HEADER_SIZE_OF_CYOUBILLING], m_ovlRecv.m_dwLeft );

			SetEvent( m_pkCYOUBilling->m_hEndEvent[m_dwKey] );
		}
	}
	LIF( InitRecv() );
}

void CYOUBilling::KSocketCYOUBilling::OnSocketError()
{
	CLOSE_SOCKET( m_sock );
	m_ovlSend.Clear();
	m_ovlRecv.Clear();
	m_ovlSend.m_dwLeft      = 0;
	m_ovlRecv.m_dwLeft      = 0;
	m_ovlSend.m_bSending    = false;
	m_ovlRecv.m_bSending    = false;

	// 너무 무자비하게 reconnect 한다.
	::Sleep( 1000 );

	START_LOG( clog, L"CYOU 빌링 소켓 끊어짐... Thread ID : " << ::GetCurrentThreadId() ) << END_LOG;
	if ( Connect( NULL, NULL, false ) == false ) {
		if ( ms_ComnData.m_nCurrentIndex < static_cast<int>( ms_ComnData.m_vecBillingServerIP_Port.size()-1 ) ) {
			++ms_ComnData.m_nCurrentIndex;
		}
		else {
			ms_ComnData.m_nCurrentIndex = 0;
		} // if-else 는 m_nCurrentIndex를 0 부터 size()-1 개 까지 증가시키고, index가 size-1 까지 갔으면 다시 0부터 시작하도록 하려는 의도.
	}
}

void CYOUBilling::KeepAliveNot( const PORT_TYPE ePortType_ )
{
}

void CYOUBilling::Reconnect()
{
	// Event reset.
	ResetEvent( m_hEvents[1] );
	ResetEvent( m_hEvents[0] );
	ResetEvent( m_hEndEvent );
	m_kSock[0].CancelIo();
	m_kSock[0].OnSocketError();
}

void CYOUBilling::AuthUserRequest( IN const KCYOUAuthUserInfo& kUserInfo_ )
{
	CYOUBillingEvent kEvent;

	unsigned char buf[MAX_PACKET_SIZE_OF_CYOUBILLING];
	memset(buf, 0, sizeof(buf)); // buf 을 0 으로 초기화
	int nSize = 0;
	kEvent.SetPacketAuthUserRequest( kUserInfo_, buf, nSize );

	SendCYOUPacket( PT_BILLING, (char*)buf, nSize );

	// response 수신부
	if ( !WaitEvents( PT_BILLING, L"" ) ) {
		//Error, return false
	}

	if ( m_kResultEvent.GetEventID() == PACKET_BL_RETAUTHUSER ) {
		Packet* pPacket = NULL;
		BLAuthUserRet packet;
		pPacket = &packet;

		InputStream iStream = InputStream( (char*)m_kResultEvent.GetBuffer() );

		UCHAR ucIPLength = 0;
		iStream.Read( ucIPLength );
		char* strIP = new char[GAMESERVER_IP];

		iStream.Read( strIP, ucIPLength );

		pPacket->SetServerIP( strIP );
		pPacket->Read( iStream );

		int nOnlineTime = packet.GetOnlineTime();
		int nOfflineTime = packet.GetOutlintTime();
		char cIsForbidden = packet.GetForbiddenInf();

		if ( cIsForbidden == 'Y' ) { // 몰입방지 해당 유저 ( 미성년 )
			// TODO : 구현하기.
		}
		//USHORT ret;
		//ret = packet.GetReturn();
	}
}

void CYOUBilling::AgreementConfirm( IN const KCYOUAuthUserInfo& kUserInfo_ )
{
	CYOUBillingEvent kEvent;

	unsigned char buf[MAX_PACKET_SIZE_OF_CYOUBILLING];
	memset(buf, 0, sizeof(buf)); // buf 을 0 으로 초기화

	//kUserInfo_.m_usPacketSequence = CYOUBilling::m_usPacketSequence;
	int nSize = 0;
	kEvent.SetPacketAgreement( kUserInfo_, buf, nSize );

	SendCYOUPacket( PT_BILLING, (char*)buf, nSize );

	// response 수신부
	if ( !WaitEvents( PT_BILLING, L"" ) ) {
		//Error, return false
	}
	if ( m_kResultEvent.GetEventID() == PACKET_BL_RETCONFIRMPROTOCOL ) {
		Packet* pPacket = NULL;
		BLConfirmProtocolRet packet;
		pPacket = &packet;

		InputStream iStream = InputStream( (char*)m_kResultEvent.GetBuffer() );

		UCHAR ucIPLength = 0;
		iStream.Read( ucIPLength );
		char* strIP = new char[GAMESERVER_IP];

		iStream.Read( strIP, ucIPLength );

		pPacket->SetServerIP( strIP );
		pPacket->Read( iStream );

		//USHORT ret;
		//ret = packet.GetReturn();
	}
}

// GC 에서는 캐릭터 전환이 빈번하고, 1개의 캐릭터로 하는 게임이 아니라서 이 기능을
// 사용하지 않을 것 같다.
void CYOUBilling::CharacterSwitch( IN const KCYOUAuthUserInfo& kUserInfo_ )
{
	CYOUBillingEvent kEvent;

	unsigned char buf[MAX_PACKET_SIZE_OF_CYOUBILLING];
	memset(buf, 0, sizeof(buf)); // buf 을 0 으로 초기화

	//kUserInfo_.m_usPacketSequence = CYOUBilling::m_usPacketSequence;
	int nSize = 0;
	kEvent.SetPacketCharacterSwitch( kUserInfo_, buf, nSize );

	SendCYOUPacket( PT_BILLING, (char*)buf, nSize );

	// response 수신부
	if ( !WaitEvents( PT_BILLING, L"" ) ) {
		//Error, return false
	}
	if ( m_kResultEvent.GetEventID() == PACKET_BL_RETSWITCHROLE ) {
		Packet* pPacket = NULL;
		BLSwitchRoleRet packet;
		pPacket = &packet;

		InputStream iStream = InputStream( (char*)m_kResultEvent.GetBuffer() );

		UCHAR ucIPLength = 0;
		iStream.Read( ucIPLength );
		char* strIP = new char[GAMESERVER_IP];

		iStream.Read( strIP, ucIPLength );

		pPacket->SetServerIP( strIP );
		pPacket->Read( iStream );

		//USHORT ret;
		//ret = packet.GetReturn();
	}
}

void CYOUBilling::SendCYOUPacket( const PORT_TYPE ePortType_, const char* kBuff_, const int nSize_ )
{
	START_LOG( clog, L"CYOU Billing Thread ID : " << m_dwThreadID ) << END_LOG;
	START_LOG( clog, L"CYOU 빌링 서버에 패킷 보냄." )
		<< BUILD_LOG( nSize_ )
		<< END_LOG;
		//<< BUILD_LOG( kPacket_.)
		//<< BUILD_LOG( kEvent_.GetPacketSize() ) << END_LOG;

	InitResultData();

	ResetEvent( m_hEndEvent[ePortType_] );

	m_kSock[ePortType_].m_dwLastHeartBeatTime = ::GetTickCount();
	LIF( m_kSock[ePortType_].SendData( kBuff_, nSize_ ) );

	++CYOUBilling::m_usPacketSequence; // TODO : apply lock ?
}

void CYOUBilling::GetCurrentCash( IN const DWORD dwUserUID_, OUT DWORD& nPoint_ )
{
	//보유 금액 조회

	//유저 존재 유무와는 상관없이 최초 데이터가 없을 경우라도 0을 리턴 합니다
	//보유하고 있다면 그 값이 리턴 됩니다

	/*
	L"{ call dbo.UCGAUserCashInfo_select_20121109 ( %d ) }"

	{ call dbo.UCGAUserCashInfo_select_20121109 ( @1 ) }
	@1: @iLoginUID_input int

	1 select ( @1 )
	@1: CashPoint int
	*//*/

	_DBJIF( m_kCashODBC.Query( L"{ call dbo.UCGAUserCashInfo_select_20121109 ( %d ) }", dwUserUID_ ), return, (&m_kCashODBC) );

	if ( m_kCashODBC.BeginFetch() ) {
		VERIFY_QUERY( m_kCashODBC >> nPoint_ );
	}
	m_kCashODBC.EndFetch();
}

bool CYOUBilling::UpdateCash( IN const DWORD dwUserUID_, IN const int nUseType_, IN const int nCash_, IN const GCITEMUID waitUID_ ) // 충전, 사용, 환불
{
	//    충전, 사용, 환불
	//
	//두 번째 인자에 대한 간략 설명입니다
	//충전: 유저가 캐쉬를 충전할 경우
	//사용: 유저가 캐쉬를 이용하여 아이템을 구매할 경우
	//환불: 사용시 에러가 발생했을 경우
	//
	//세 번째 인자는 유저가 캐쉬를 사용시 어떠한 아이템을 구매했는지에 대한 기록을 남기기 위한 용도입니다
	//충전과 환불에서는 사용하지 않기 때문에 0값을 넣어주시고 사용일 경우에는 구매한 아이템의 대기UID값을 넣어주세요
	//
	//네 번째 인자는 사용될 캐쉬의 금액 입니다

	/*
	L"{ call dbo.UCGAUserCashInfo_merge_20121109 ( %d, %I64d, %d, %d ) }"

	{ call dbo.UCGAUserCashInfo_merge_20121109 ( @1, @2, @3, @4 ) }
	@1: @iLoginUID_input       int
	@2: @iType_input           int
		1:충전
		2:사용
		3:환불
	@3: @iWIGAUID_input        bigint
		대기 아이템의 UID
		@2가 2인 경우에만 0보다 큰 값을 가지며 1, 3인 경우에는 0을 넣어준다
	@4: @iCashPoint_diff_input int
		충전/환불이든 사용이든 0보다 큰 값으로 넣어야 함
		사용일 경우 내부에서 음수값으로 변경하여 저장

	1 return ( @1 )
	@1: OK int
		0        : 성공
		-1       : 없는 타입
		-2       : 입력된 캐쉬 포인트가 0보다 작음
		-3       : 보유 금액보다 더 많은 금액을 사용하려 했음
		-801~-899: 무시가능 에러
		-901~-999: 치명적인 에러
	*//*

	_DBJIF( m_kCashODBC.Query( L"{ call dbo.UCGAUserCashInfo_merge_20121109 ( %d, %d, %I64d, %d ) }",
		dwUserUID_, nUseType_, waitUID_ ,nCash_), return false, ( &m_kCashODBC ) );

	int nOK = -99;

	if ( m_kCashODBC.BeginFetch() ) {
		VERIFY_QUERY( m_kCashODBC >> nOK );
	}
	m_kCashODBC.EndFetch();

	LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

	return ( 0 == nOK );
}

_INT_IMPL_ON_FUNC( EVENT_SERVER_LOGIN_REQ, KCYOUServerInfo )
{
	USHORT usRet = 0;
	kPacket_.m_usZoneID = ms_ComnData.m_usZoneID;
	kPacket_.m_usGroupID = ms_ComnData.m_usGroupID;
	kPacket_.m_usPacketSequence = CYOUBilling::m_usPacketSequence;

	if ( usRet != 0 ) { // 빌링 서버에 정상 접속 안되면 게임서버를 종료한다. TODO : 0은 정상이 아닐거야
		SiKGameServer()->ShutDown();
	}
}

_INT_IMPL_ON_FUNC( EVENT_SERVER_HEART_BEAT_REQ, KCYOUServerInfo )
{
   //ServerKeepAliveRequest( kPacket_ );
}

_INT_IMPL_ON_FUNC( EVENT_CYOU_USER_HEART_BEAT_REQ, KCYOUAuthUserInfo )
{
	//UserHeartBeat( kPacket_ );
}

_INT_IMPL_ON_FUNC( EVENT_CYOU_USER_LOGOUT_REQ, KCYOUAuthUserInfo )
{
	//UserLogout( kPacket_ );
}

_INT_IMPL_ON_FUNC( EVENT_VERIFY_ACCOUNT_REQ, KCYOUAuthUserInfo )
{
	//UserLogin( kPacket_ );
	// _QUEUING_ACK_TO_USER
}

_INT_IMPL_ON_FUNC( EVENT_CYOU_SERVER_RESTART_REQ, KCYOUServerInfo )
{
	//ServerRestart( kPacket_ );
}

_INT_IMPL_ON_FUNC( EVENT_CYOU_USER_SWITCH_CHARACTER_REQ, KCYOUAuthUserInfo )
{
	CharacterSwitch( kPacket_ );
}

_INT_IMPL_ON_FUNC( EVENT_CYOU_CASH_CHANGE_REQ, KCYOUCashData )
{
	//ChangeCash( kPacket_ );
}

_INT_IMPL_ON_FUNC( EVENT_CURRENT_CASH_POINT_REQ, KCashInfo )
{
	GetCurrentCash( dwUID_, kPacket_.m_dwCashPoint );

   _QUEUING_ACK_TO_USER( EVENT_CURRENT_CASH_POINT_ACK, kPacket_ );
}

INT_IMPL_ON_FUNC( EVENT_BUY_FOR_CASH_REQ )
{
	EVENT_CHECK_OUT(EVENT_BUY_FOR_CASH_REQ);

	KEVENT_BUY_FOR_CASH_ACK kPacket;
	DWORD dwOwnerUID = kPacket_.m_dwBuyerUID; //기본으로 구매자의 UID를 셋팅한다.
	__int64 waitItemUID = 0;
	std::wstring wstrItemName;
	int nUseCash = 0;
	kPacket_.m_strBuyerLogin = strLogin_;

	START_LOG( clog, L"아이템 구매 시작" )
		<< BUILD_LOG( kPacket_.m_dwBuyerUID )
		<< END_LOG;

	if ( !KBillDB::CheckBuyLimitItem( dwUID_, kPacket_.m_kBuyInfo.m_ItemID ) ) {
		kPacket.m_nOK = 3; // 구매 제한에 걸렸음.
		SET_ERR_GOTO( ERR_BILL_13, END_PROC );
	}

	GetCurrentCash( kPacket_.m_dwBuyerUID, kPacket_.m_cashInfo.m_dwCashPoint );
	kPacket.m_kVirtualCashInfo.m_dwCashPoint = m_kHelper.GetCurrentVirtualCash( dwUID_, strLogin_ );

	if ( m_kHelper.GetPrice( kPacket_.m_kBuyInfo.m_ItemID,
		kPacket_.m_kBuyInfo.m_nCount,
		kPacket_.m_kBuyInfo.m_nPeriod,
		kPacket_.m_kBuyInfo.m_nPrice,
		kPacket_.m_kBuyInfo.m_nMoneyType ) == false ) {
		START_LOG( cwarn, L"가격 정보 가져오기 실패" ) << END_LOG;
		SET_ERR_GOTO( ERR_BILL_05, END_PROC );
	}

	// TODO ; 추가 조건 체크.
	// 잔액 > 아이템 가격
	if ( kPacket_.m_cashInfo.m_dwCashPoint < static_cast<DWORD>(kPacket_.m_kBuyInfo.m_nPrice) ) {
		SET_ERR_GOTO( ERR_BILL_12, END_PROC );
	}
	// 캐시 아이템 타입인지 확인
	if ( kPacket_.m_kBuyInfo.m_nMoneyType != KItemManager::EMT_CASH ) {
		SET_ERR_GOTO( ERR_BILL_04, END_PROC );
	}

	if( m_kHelper.BuyItemToWait( kPacket_.m_dwBuyerUID, kPacket_.m_strBuyerLogin, kPacket_.m_kBuyInfo, L"", waitItemUID, wstrItemName ) != 0 )
	{
		LogQuery( KBillDB::GetLastQuery(), strLogin_ );
		START_LOG( cwarn, L"구매해서 대기 리스트로 넣기 실패" ) << END_LOG;
		kPacket.m_nOK = 2;
		SET_ERR_GOTO( ERR_BILL_15, END_PROC );
	}

	nUseCash = kPacket_.m_kBuyInfo.m_nPrice;

	//BuyLog( kPacket_ );
	// UpdateCash 실패시에는 롤백
	if ( false == UpdateCash( kPacket_.m_dwBuyerUID, USE_CASH, nUseCash ) ) {
		if( !m_kHelper.RollbackBuyItem( waitItemUID ) ) {
			kPacket.m_nOK = -10;
		}
		LogQuery( KBillDB::GetLastQuery(), strLogin_ );
	}

	m_kHelper.UpdateWaitItemToEnabled( waitItemUID );

	if ( kPacket_.m_kBuyInfo.m_nPrice > 0 ) {
		__int64 BillingKey; // CYOU 에서도 사용될런지 모름.?
		KBillDB::SetBillingKey( waitItemUID, BillingKey );
	}

	if ( !m_kHelper.InsertWaitItemToInven( dwOwnerUID, kPacket_.m_kBuyInfo.m_cCharType, KItemManager::EWIT_CASH_BUY, kPacket.m_vecItem ) ) {
		kPacket.m_nOK = -10;
		LogQuery( KBillDB::GetLastQuery(), strLogin_ );
		SET_ERR_GOTO( ERR_BILL_08, END_PROC );
	}

	CommitBuyLimitItem( dwUID_, kPacket_.m_kBuyInfo.m_ItemID ); // 구매제한 기록
	m_kHelper.UpdateMoneyConsumptionStat( dwUID_, KItemManager::EMC_CASH, kPacket_.m_kBuyInfo.m_nCount, waitItemUID );
	kPacket.m_cashInfo.m_dwCashPoint = kPacket_.m_cashInfo.m_dwCashPoint - kPacket_.m_kBuyInfo.m_nPrice;
	kPacket.m_cashInfo.m_dwDecreaseCashPoint = std::max<int>( nUseCash, 0 );
	kPacket.m_kBuyInfo = kPacket_.m_kBuyInfo;

END_PROC:
	QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
}

void CYOUBilling::LogQuery( IN const std::wstring& strLastQuery_, IN const std::wstring& strLogin_ )
{
	std::wstringstream stm;
	stm << m_dwThreadID << L"|QUERY|" << strLogin_
		<< L"|"<< KNC_TIME_STRING
		<< L"|" << strLastQuery_;
	SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
}

// code:#END_OF_CYOU_CASE
*/