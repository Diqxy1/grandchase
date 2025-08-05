#include "Session.h"
#include "Event.h"
#include "Thread/Locker.h"
#include "dbg/dbg.hpp"
#include "NetCommon.h"
#include <time.h>   // localtime
#include "LogManager.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

#ifdef KNC_USE_IOCP
#include "IOCP.h"
#endif

#define SEND_IMITATION_PACKET( id, buff ) \
do { \
    unsigned short      usEventID = id; \
    KSerBuffer          kbuffSend; \
    int                 nfrom = 0; \
    ks.BeginWriting( &kbuffSend ); \
    ks.Put( usEventID ); \
    ks.Put( buff ); \
    ks.Put( nfrom ); \
    ks.EndWriting(); \
    LIF( SendPacket( kbuffSend, L#id ) ); \
} while(false)

NiImplementRTTI(KSession, KPerformer);

//#define PROXY_ONLY cassert( m_bIsProxy )(m_strName).fatal()
//#define UNPROXY_ONLY cassert( !m_bIsProxy )(m_strName).fatal()

KSession::KMaxPacketInfo KSession::ms_kMaxPacketInfo = {
	KncCriticalSection(), L"초기상태", L"초기상태", 0, 0, CTime::GetCurrentTime()
};

// 091113 tgkwon. SequenceNum 체크 추가.( 중복 패킷 방지 )
KSession::KPacketAuthInfo KSession::ms_kPacketAuthInfo = {
	KncCriticalSection(), L"실패패킷없음", 0, 0, CTime::GetCurrentTime()
};

DWORD   KSession::m_dwHBGap = 30000;
bool    KSession::m_bCheckSequenceNum = false;
UINT    KSession::m_uiPacketAuthLimitNum = 0;
std::vector<std::wstring> KSession::m_vecOverRecvEvent;
std::map<USHORT, __int64> KSession::m_maPacketCount;
KncCriticalSection KSession::m_csOverRecvEvent;
KncCriticalSection KSession::m_csPacketCount;
int     KSession::m_nPacketSendCount = 200;
int     KSession::m_nLogPacketSendCount = 100;
DWORD   KSession::m_dwSendCountTickGap = 1000 * 60;
bool    KSession::m_bHackCheckEnable = false;

KSession::KSession(bool bIsProxy)
	:
	m_bIsProxy(bIsProxy),
	m_bUseIocp(true),
	m_bDestroyReserved(false),
	m_nSPIndex(0),
	m_bAuthKeyRecved(false),
	m_bCheckHBTick(true),
	m_nDisconnectReason(DRW_NONE),
	m_uiPacketAuthFailCnt(0),
	m_dwRecvCountTickGap(1000), // ms
	m_usOverCount(50),
	m_bCheckedOverCount(false),
	m_usOverCountGap(50),
	m_usOverCountWrite(0),
	m_usMaxOverCountWrite(3)
{
	m_dwHBTick = GetTickCount();

	m_kSockObj.m_pkSession = this;

	m_hEvents[EVENT_RECV_COMPLETED] = NULL;
	m_hEvents[EVENT_SEND_COMPLETED] = NULL;

	m_dwRecvCountTick = ::GetTickCount();
	m_mapUserPacketSendCount.clear();
	m_dwStartPacketTickCount = ::GetTickCount();
}

KSession::~KSession(void)
{
	m_kSockObj.CloseSocket();

	if (m_bUseIocp)
	{
		DeleteCompKey();
	}
	else
	{
		End();  // thread use

		::CloseHandle(m_hEvents[EVENT_RECV_COMPLETED]);
		::CloseHandle(m_hEvents[EVENT_SEND_COMPLETED]);
	}

	if (m_nSPIndex != 0)
	{
		START_LOG(clog, L"SA 삭제. Name : " << m_strName << L", SPI : " << m_nSPIndex) << END_LOG;
		KncSecurity::GetSADB().Delete(m_nSPIndex);
		m_nSPIndex = 0;
	}
}

ImplToStringW(KSession)
{
	return START_TOSTRING_PARENTW(KPerformer)
		<< TOSTRINGW(m_dwHBTick)
		<< TOSTRINGWb(m_bDestroyReserved)
		<< TOSTRINGW(m_nSPIndex)
		<< TOSTRINGWb(m_bIsProxy)
		<< TOSTRINGWb(m_bUseIocp)
		<< TOSTRINGWb(m_bCheckHBTick);
}

bool KSession::Init(bool bUseIocp_)
{
	//PROXY_ONLY;
	m_bUseIocp = bUseIocp_;

	if (!m_bUseIocp)
	{
		m_hEvents[EVENT_RECV_COMPLETED] = CreateEvent(NULL, false, false, NULL);
		m_kSockObj.SetIOEvent(KOVERLAPPED::IO_RECV, m_hEvents[EVENT_RECV_COMPLETED]);
		m_hEvents[EVENT_SEND_COMPLETED] = CreateEvent(NULL, false, false, NULL);
		m_kSockObj.SetIOEvent(KOVERLAPPED::IO_SEND, m_hEvents[EVENT_SEND_COMPLETED]);
	}

	return true;
}

bool KSession::Connect(const char* szIP_, unsigned short usPort_, bool bNagleAlgOn)
{
	//PROXY_ONLY;

	std::vector< std::pair<int, int> > vecOpt;
	vecOpt.push_back(std::make_pair(SOL_SOCKET, SO_REUSEADDR)); // 사용중인 address를 재사용할 수 있게 허용.

	if (!bNagleAlgOn)
		vecOpt.push_back(std::make_pair(IPPROTO_TCP, TCP_NODELAY)); // Nagle option을 제거. 데이터를 모아서 보내지 않고 그 즉시 전송하도록 한다.

	START_LOG(clog, L"Connect To : " << szIP_ << L":" << usPort_) << END_LOG;

	_JIF(m_kSockObj.Connect(szIP_, usPort_, m_bUseIocp, &vecOpt), return false);

	if (m_bUseIocp)
	{
#ifdef KNC_USE_IOCP
        KSessionPtr spSession = std::static_pointer_cast<KSession>( shared_from_this() );
        _JIF( SiKIocp()->AssociateSocket( spSession ), return false );
#endif
	}

	_JIF(m_kSockObj.InitRecv(), return false);

	if (!m_bUseIocp)   // no use iocp
	{
		Begin();    // thread run
	}

	// 050520. 키를 받아 설정할 때까지 대기한다.
	DWORD dwWaitStartTick = ::GetTickCount();

	Sleep(20);

	// 대기시간을 초과하지 않았고 받은 메세지가 아무것도 없는 동안은
	while (::GetTickCount() - dwWaitStartTick < SECURE_KEY_WAITING_TIME && !m_bAuthKeyRecved)
		Sleep(20);    // 계속 기다린다.

	if (!m_bAuthKeyRecved)  // 키를 받지 못했다면
	{
		START_LOG(cerr, L"서버에서 보안 키를 받지 못했습니다. name:" << m_strName << L", 대기시간:"
			<< ::GetTickCount() - dwWaitStartTick) << END_LOG;
		ReserveDestroy();
		return false;
	}

	return true;
}

void KSession::OnAcceptConnection()
{
	//UNPROXY_ONLY;
	// 091113 tgkwon. SequenceNum 체크 추가.( 중복 패킷 방지 ) comment
	// 1. 클라이언트의 접속이 성공하면 호출됨
	// 2. 패킷 보안을 위한 KSecurityAssociation객체를 여기서 생성함 (Session당 하나씩있음)
	// 3. 클라이언트로 KSecurityAssociation객체 정보를 보내어 보안인증을 동기화시키려는 목적임

	const KncSecurity::KSecurityAssociation* pkSA = NULL;
	SpiType nSpiNew = 0;
	pkSA = KncSecurity::GetSADB().CreateNewSA(nSpiNew);

	KSerializer ks;
	KSerBuffer  kbuff;
	ks.BeginWriting(&kbuff);
	ks.Put(nSpiNew);
	ks.Put(*pkSA);
	ks.EndWriting();

	SEND_IMITATION_PACKET(EVENT_ACCEPT_CONNECTION_NOT, kbuff);    // 상수키로 보낸 후,
	m_nSPIndex = nSpiNew;   // 새 키로 변경해준다.
}

void KSession::OnDestroy()
{
	m_kSockObj.CloseSocket();
	m_bAuthKeyRecved = false;    // 070307. kkurrung. 소켓을 끊을때 인증키 받은 정보를 초기화 해준다. 다음에 다시 연결 할수 있도록.
	m_bDestroyReserved = false;    // 이전에 종료가 예약 되었든 아니든, 한번 Destory 호출이 성공하면 없어진다.

	// 050323. floris. IOCP 사용 안할 경우, ReserveDestroy 호출하면 Thread 멈춘다.
	if (!m_bUseIocp)
	{
		SetEvent(m_hKillEvent);
	}
	else
	{
		// 091113 tgkwon. SequenceNum 체크 추가.( 중복 패킷 방지 )
		const UINT uiPacketAuthFailCnt = GetPacketAuthFailCnt();
		if (uiPacketAuthFailCnt > 0) {
			KLocker lock(ms_kPacketAuthInfo.m_cs);
			ms_kPacketAuthInfo.m_uiTotalFailCnt += uiPacketAuthFailCnt; // 전체 AuthFailCnt + 현재 유저의 FailCnt

			// MaxFailCnt보다 TotalFailCnt다 클경우..,
			if (ms_kPacketAuthInfo.m_uiMaxFailCnt < uiPacketAuthFailCnt) {
				ms_kPacketAuthInfo.m_strName = m_strName;
				ms_kPacketAuthInfo.m_cRegTime = CTime::GetCurrentTime();
				ms_kPacketAuthInfo.m_uiMaxFailCnt = uiPacketAuthFailCnt;
				START_LOG(clog, L"Packet Auth Fail 갱신. Name : " << m_strName)
					<< BUILD_LOG(ms_kPacketAuthInfo.m_uiMaxFailCnt) << END_LOG;
			}
		}

		DeleteCompKey();
	}

	if (GetQueueSize() > 0)   // 050328. 아직 처리되지 않은 이벤트들은 처리해준다.
		KPerformer::Tick();
}

void KSession::DeleteCompKey()
{
#ifdef KNC_USE_IOCP
	if (m_kSockObj.GetKey() != 0)
	{
		SiKIocp()->DeleteCompKey(m_kSockObj.GetKey());
		m_kSockObj.SetKey(0);
	}
#endif
}

void KSession::KSkSession::OnRecvCompleted(DWORD dwTransfered_)
{
	if (dwTransfered_ == 0)
	{
		START_LOG(clog, L"closed by remote machine") << END_LOG;
		OnSocketError();
		return;
	}

	PACKET_HEADER PacketLen = 0;
	m_ovlRecv.m_dwLeft += dwTransfered_;

	// 패킷 길이 진행량보다 크다면 계속 새 패킷 가능성을 체크한다.
	while (m_ovlRecv.m_dwLeft >= sizeof(PACKET_HEADER)) {
		// 패킷 길이를 알아낸다.
		memcpy(&PacketLen, m_ovlRecv.m_pBuffer, sizeof(PACKET_HEADER));

		// 버퍼에 받아둔 데이터로 완전한 packet이 가능한 경우 다음으로 진행한다.
		// 온전한 패킷을 구성하기에 충분한 데이터가 없음. loop를 빠져나간다.
		if (m_ovlRecv.m_dwLeft < PacketLen) {
			break;
		}

		// 길이를 제외한 데이터만 버퍼에 담는다.
		KncSecurity::KSecureBuffer kSecBuff(m_pkSession->m_nSPIndex, m_ovlRecv.m_pBuffer + sizeof(PACKET_HEADER),
			PacketLen - sizeof(PACKET_HEADER));

		// 091113 tgkwon. SequenceNum 체크 추가.( 중복 패킷 방지 )
		// DOS 공격이 들어온다면 에러로그를 공격량만큼 찍어낼 것이기 때문에 서버가 위험해 지는건 마찬가지다.
		if (!kSecBuff.IsAuthentic_notRWM(m_pkSession->IsCheckSequenceNum())) {
			START_LOG(cwarn, L"packet authentication failed.")
				<< BUILD_LOG(kSecBuff.GetSize()) << END_LOG;
			//<< BUILD_LOG( m_pkSession->GetName() ) << END_LOG;

		// 패킷 인증이 실패하면 실패 카운트를 올린다.
			m_pkSession->IncreasePacketAuthFailCnt();
			// 패킷 인증 실패가 특정갯수 이상 발생시 접속 차단
			if (m_pkSession->GetPacketAuthFailCnt() > m_pkSession->GetPacketAuthLimitNum()) {
				START_LOG(clog, L"Find Traffic Attacker!") << END_LOG;
				m_pkSession->SetDisconnReason(DRW_TRAFFIC_ATTACK);
				OnSocketError();

				// 여기서 return해야만 InitRecv()를 호출하지 않으므로 IOCP완료 이벤트가 더이상 오지 않는다.
				return;
			}
		}
		else {
			// 인증 및 복호화 처리를 마치고 순수 payload만을 얻어낸다.
			KncSecurity::KByteStream payload;
			JIF(kSecBuff.GetPayload_notRWM(payload, false)); // 위에서 이미 IsAuthentic()을 호출했으므로 두번째 인자는 false를 전달!

			m_pkSession->OnRecvCompleted(payload);
		}

		// buffer management
		m_ovlRecv.m_dwLeft -= PacketLen;
		memmove(&m_ovlRecv.m_pBuffer[0], &m_ovlRecv.m_pBuffer[PacketLen], m_ovlRecv.m_dwLeft);
	} // while( m_ovlRecv.m_dwLeft >= sizeof(DWORD) )

	// recv function call
	LIF(InitRecv());
}

void KSession::OnRecvCompleted(IN const KncSecurity::KByteStream& payload_)
{
	KSerializer     ks;
	KSerBuffer      kbuff;

	// payload를 KEvent 형식으로 전환한다.
	if (!kbuff.Write(payload_.data(), payload_.length()))
	{
		START_LOG(cerr, L"페이 로드 얻어오는중 실패 Name : " << m_strName) << END_LOG;
		return;
	}

	KEventPtr spEvent(CreateKEvent());
	if (!ks.BeginReading(&kbuff) || !ks.Get(*spEvent) || !ks.EndReading())
	{
		START_LOG(cerr, L"이벤트 생성시 실패 Name : " << m_strName) << END_LOG;
		return;
	}

	// 압축 된 경우만 자동으로 해제한다.
	spEvent->m_kbuff.UnCompress();

	// Event check
	CheckRecvPacketOver(spEvent->m_usEventID);
	IncreasePacketCount(spEvent->m_usEventID);

	// Packet Hack Check
	int nSendPacketCount;
	if (!IncreaseUserPacketSendCount(spEvent->m_usEventID, nSendPacketCount)) {
		if (!CheckUserPacketSendCount(spEvent->m_usEventID, nSendPacketCount)) {
			START_LOG(cerr, L"User Packet Attacker!") << END_LOG;
			SetDisconnReason(DRW_PACKET_ATTACK);
			OnSocketError();
			return;
		}
	}

	switch (spEvent->m_usEventID)
	{
	case EVENT_HEART_BIT:                   // heart bit filtering - no queueing
		//UNPROXY_ONLY;
		break;
	case EVENT_ACCEPT_CONNECTION_NOT:
	{
		//PROXY_ONLY;

		// comment
		// 1. 클라이언트의 서버접속이 성공하면 가장 처음 서버로부터 받는 패킷임
		// 2. 패킷 보안을 위한 KSecurityAssociation객체를 서버로부터 받아서 SADB에 저장함
		// 3. 서버로부터 도착한 SPIndex는 쓰이지 않음
		// 4. 클라이언트에서는 중복되지않는 SPIndex를 랜덤하게 생성하여 사용 ( KSecurityAssociation는 서버로부터 받은것 그대로 써야함 )

		KncSecurity::KSecurityAssociation kSA;

		ks.BeginReading(&spEvent->m_kbuff);
		ks.Get(m_nSPIndex);
		ks.Get(kSA);
		ks.EndReading();

		KncSecurity::GetSADB().Insert(m_nSPIndex, kSA);
		START_LOG(clog, m_strName << L", 보안키 설정. spi:" << m_nSPIndex) << END_LOG;

		m_bAuthKeyRecved = true;
	}
	break;
	default:
		QueueingEvent(spEvent, KEvent::FT_OUTTER);
	}

	// renew m_dwHBRecvTick
	if (!m_bIsProxy)
		m_dwHBTick = ::GetTickCount();
}

bool KSession::SendPacket(KSerBuffer& kbuff_, const wchar_t* szDataDesc_)
{
	if (m_bDestroyReserved) return true;       ///< 종료가 예약되었을때 send 시도. 이런 경우가 다분하므로, true 인정.
	//__JIF( m_kSockObj.IsConnected(), return false );    ///< 소켓이 유효하지 않음. //MeGaMaX
	__JIF( m_kSockObj.IsConnected(), return false);    ///< 소켓이 유효하지 않음.

	// 암호화한다.
	KncSecurity::KByteStream    bsbuff;
	KncSecurity::KSecureBuffer  kSecBuff(m_nSPIndex);

	bsbuff.assign((byte*)kbuff_.GetData(), kbuff_.GetLength());   // 실제 전송할 데이터 넣기.
	_JIF(kSecBuff.Create(bsbuff), return false);                       // 암호화 및 인증 절차 처리.

	// bsbuff를 재활용해, 암호화된 데이터 앞에 총 길이를 덧붙인 byte stream을 만든다.
	bsbuff.clear();
	bsbuff.reserve(kSecBuff.GetSize() + sizeof(PACKET_HEADER));   // 패킷 총 사이즈[4] + 전송할 데이터[가변]
	bsbuff.Assign(kSecBuff.GetSize() + sizeof(PACKET_HEADER), sizeof(PACKET_HEADER));
	bsbuff.append((byte*)kSecBuff.GetDataPtr(), kSecBuff.GetSize());

	CheckMaxPacketInfo(bsbuff.length(), kbuff_.GetLength(), szDataDesc_);

	return m_kSockObj.SendData((const char*)bsbuff.data(), bsbuff.length());
}

bool KSession::Begin()  // KThread의 이벤트 객체를 배열에 보관해, WaitMultipleObject를 사용한다.
{
	if (KThread::Begin())
	{
		m_hEvents[EVENT_DISCONNECTED] = m_hKillEvent;
		return true;
	}

	return false;
}

void KSession::Run()
{
	//PROXY_ONLY;

	DWORD ret = NULL;
	bool bLoop = true;

	while (bLoop)
	{
		ret = ::WaitForMultipleObjects(EVENT_MAX_VALUE,
			m_hEvents,
			false,
			100);     // 0.1s 간격

		switch (ret)
		{
		case EVENT_DISCONNECTED + WAIT_OBJECT_0:    // 스레드 종료
			bLoop = false;
			break;
		case EVENT_RECV_COMPLETED + WAIT_OBJECT_0:
		{
			m_kSockObj.OnIOCompleted(KOVERLAPPED::IO_RECV);
		}
		break;
		case EVENT_SEND_COMPLETED + WAIT_OBJECT_0:
		{
			m_kSockObj.OnIOCompleted(KOVERLAPPED::IO_SEND);
		}
		break;
		case WAIT_TIMEOUT:
			break;

		case WAIT_FAILED:
			//cassert( false )( GET_WSA_MSG ).fatal();
			break;
		}

		Tick(); // 050214. IOCP를 사용하지 않을 경우, Tick을 내부에서 처리한다.
	} // while
}

void KSession::Tick()
{
	KPerformer::Tick();
	if ( m_bDestroyReserved )
	{
		OnDestroy();
		return;
	}
	if ( !m_bIsProxy )
	{
		if ( m_bCheckHBTick && ::GetTickCount() - m_dwHBTick > KSession::m_dwHBGap )
		{
			START_LOG(cwarn, L"Regard " << m_strName
				<< L" as a Zombie. (tick: "
				<< ::GetTickCount() - m_dwHBTick
				<< L" ) IP: ") << KncUtil::toWideString(GetIPStr()) << END_LOG;

			SetDisconnReason( DRW_ZOMBIE );
			OnDestroy();
		}
	}
	else
	{
		if ( !IsConnected() || ::GetTickCount() - m_dwHBTick <= 15000 || !m_bAuthKeyRecved )
			return;
		m_dwHBTick = ::GetTickCount();
		KSerializer ks;
		KSerBuffer kbuff;
		SEND_IMITATION_PACKET( EVENT_HEART_BIT, kbuff );
	}
}

void KSession::OnSocketError()
{
	m_bDestroyReserved = true;
}

void KSession::DumpMaxSendData(std::wostream& stm_)
{
	KLocker lock(ms_kMaxPacketInfo.m_cs);

	stm_ << L" -- Max size Data sending -- " << std::endl
		<< L"  user name   : " << ms_kMaxPacketInfo.m_strName << std::endl
		<< L"  data desc   : " << ms_kMaxPacketInfo.m_strDataDesc << std::endl
		<< L"  data size   : " << ms_kMaxPacketInfo.m_nDataSize
		<< L" (" << ms_kMaxPacketInfo.m_nDataSize / 1024.0f << " Kb)" << std::endl
		<< L"  packet size : " << ms_kMaxPacketInfo.m_nPacketSize
		<< L" (" << ms_kMaxPacketInfo.m_nPacketSize / 1024.0f << " Kb)" << std::endl
		<< L"  send time   : " << (LPCWSTR)ms_kMaxPacketInfo.m_cTime.Format(KNC_TIME_FORMAT) << std::endl;
}

void KSession::ResetMaxSendData()
{
	KLocker lock(ms_kMaxPacketInfo.m_cs);

	ms_kMaxPacketInfo.m_strDataDesc.clear();
	ms_kMaxPacketInfo.m_nDataSize = 0;
	ms_kMaxPacketInfo.m_nPacketSize = 0;
	ms_kMaxPacketInfo.m_cTime = CTime::GetCurrentTime();
	ms_kMaxPacketInfo.m_strName = (LPCWSTR)ms_kMaxPacketInfo.m_cTime.Format(KNC_TIME_FORMAT);
	ms_kMaxPacketInfo.m_strName.append(L" 에 데이터 초기화 되었습니다.");
}

bool KSession::SendID(unsigned short usEventID_)
{
	// check valide state
	KEventPtr spEvent(CreateKEvent());
	spEvent->m_usEventID = usEventID_;

	// make event object
	KSerializer         ks;

	// serialize - full event object
	KSerBuffer          kbuff;
	ks.BeginWriting(&kbuff);
	ks.Put(*spEvent);
	ks.EndWriting();

	// send packet
	if (!KSession::SendPacket(kbuff, spEvent->GetEventIDString()))
		return false;

	START_LOG(clog, L"ⓢ " << spEvent->GetEventIDString() << L" (name:" << m_strName << L")") << END_LOG;

	return true;
}

bool KSession::CheckExceedRefCount(int nCount_)
{
	// 입력받은 수와 같거나 적은 경우는 문제를 일으키지 않으므로, true를 리턴한다.
	// false가 리턴된 경우, 로그출력 등의 추가작업을 위해선 child 클래스에서 상속받아 구현한다.
	return (GetRefCount() <= nCount_);
}

void KSession::ReserveDestroy(int nReason_ /*= 0*/)
{
	if (nReason_ != DRW_NONE)
		SetDisconnReason(nReason_);
	m_bDestroyReserved = true;
}     // OnDestory 예약

void KSession::SetDisconnReason(int nReason_)
{
	if (nReason_ != DRW_NONE) // 이미 다른게 셋팅 되어 있으면 쌩..
		m_nDisconnectReason = nReason_;
}

// 091113 tgkwon. SequenceNum 체크 추가.( 중복 패킷 방지 )
void KSession::DumpPacketAuthInfo(std::wostream& stm_)
{
	KLocker lock(ms_kPacketAuthInfo.m_cs);

	stm_ << L" -- Packet Auth Fail Max Cnt -- " << std::endl
		<< L"  User Name   : " << ms_kPacketAuthInfo.m_strName << std::endl
		<< L"  Fail Count  : " << ms_kPacketAuthInfo.m_uiMaxFailCnt << std::endl
		<< L"  Total Fail  : " << ms_kPacketAuthInfo.m_uiTotalFailCnt << std::endl
		<< L"  Reg Time    : " << (LPCWSTR)ms_kPacketAuthInfo.m_cRegTime.Format(KNC_TIME_FORMAT) << std::endl;
}

void KSession::CheckRecvPacketOver(IN const unsigned short usEventID_)
{
	if (m_bCheckedOverCount) {
		return;
	}

	// Tick Check
	if (::GetTickCount() - m_dwRecvCountTick > m_dwRecvCountTickGap) {
		if (m_vecRecvEventTemp.size() >= m_usOverCount) {
			std::wstringstream stm;
			stm << KNC_TIME_STRING
				<< L" | " << m_dwUID
				<< L" | " << m_strName
				<< L" | " << KncUtil::toWideString(GetIPStr())
				<< L" | " << m_vecRecvEventTemp.size()
				<< L" | ";
			std::vector<unsigned short>::iterator vit;
			for (vit = m_vecRecvEventTemp.begin(); vit != m_vecRecvEventTemp.end(); ++vit) {
				stm << *vit << L",";
			}

			{
				KLocker lock(m_csOverRecvEvent);
				m_vecOverRecvEvent.push_back(stm.str());
			}

			// 이전 조건보다 높은 카운트로 보낼 경우에 기록한다.
			m_usOverCount = m_vecRecvEventTemp.size();
			m_usOverCount += m_usOverCountGap;

			// 최대 횟수까지 기록한 후, 더이상 기록하지 않는다.
			++m_usOverCountWrite;
			if (m_usOverCountWrite >= m_usMaxOverCountWrite) {
				m_bCheckedOverCount = true;
			}
		}

		m_vecRecvEventTemp.clear();
	}

	// EventID 기록
	m_vecRecvEventTemp.push_back(usEventID_);
	m_dwRecvCountTick = ::GetTickCount();
}

void KSession::CheckMaxPacketInfo(IN const size_t PacketSize_, IN const size_t DataSize_, IN const wchar_t* szDataDesc_)
{
	KLocker lock(ms_kMaxPacketInfo.m_cs);

	if (ms_kMaxPacketInfo.m_nPacketSize < PacketSize_) {
		ms_kMaxPacketInfo.m_strName = m_strName;
		// 050930. florist. std::wstring에 NULL이 assign되면 출력시 runtime-error가 발생함을 테스트 코드로 확인했다.
		ms_kMaxPacketInfo.m_strDataDesc = (szDataDesc_ != NULL) ? szDataDesc_ : L"NULL";
		ms_kMaxPacketInfo.m_cTime = CTime::GetCurrentTime();
		ms_kMaxPacketInfo.m_nDataSize = DataSize_;
		ms_kMaxPacketInfo.m_nPacketSize = PacketSize_;

		START_LOG(clog, L"패킷 최대사이즈 갱신. Name : " << m_strName)
			<< BUILD_LOG(ms_kMaxPacketInfo.m_strDataDesc)
			<< BUILD_LOG(ms_kMaxPacketInfo.m_nDataSize)
			<< BUILD_LOG(ms_kMaxPacketInfo.m_nPacketSize) << END_LOG;
	}
}

void KSession::GetOverRecvEvent(OUT std::vector<std::wstring>& vecOverRecvEvent_)
{
	vecOverRecvEvent_.clear();
	if (m_vecOverRecvEvent.empty()) {
		return;
	}

	KLocker lock(m_csOverRecvEvent);
	vecOverRecvEvent_.swap(m_vecOverRecvEvent);
}

void KSession::IncreasePacketCount(IN const USHORT& usEventID_)
{
	KLocker lock(m_csPacketCount);
	++m_maPacketCount[usEventID_];
}

void KSession::GetPacketCount(OUT std::map<USHORT, __int64>& mapData_)
{
	mapData_.clear();
	KLocker lock(m_csPacketCount);
	mapData_.swap(m_maPacketCount);
}

bool KSession::IncreaseUserPacketSendCount(IN const USHORT& usEventID_, OUT int& nPacketCount_)
{
	nPacketCount_ = 0;
	std::map< USHORT, int >::iterator mit;
	KLocker lock(m_csPacketCount);
	if (m_mapUserPacketSendCount.empty()) {
		m_dwStartPacketTickCount = ::GetTickCount();
	}

	++m_mapUserPacketSendCount[usEventID_];

	mit = m_mapUserPacketSendCount.find(usEventID_);
	if (mit == m_mapUserPacketSendCount.end()) { // 말도 안되는 상황.
		return true;
	}

	// 로그 데이터.
	if (mit->second == GetLogPacketSendCount()) {
		if (::GetTickCount() - m_dwStartPacketTickCount < m_dwSendCountTickGap) {
			START_LOG(cerr, L"유저 패킷 공격 로그 데이터")
				<< BUILD_LOG(usEventID_)
				<< BUILD_LOG(mit->second)
				<< END_LOG;
		}
	}

	// 패킷 SendCount Check.
	if (mit->second >= GetPacketSendCount()) {
		nPacketCount_ = mit->second;
		return false;
	}
	return true;
}

bool KSession::CheckUserPacketSendCount(IN const USHORT& usEventID_, IN const int& nPacketCount_)
{
	KLocker lock(m_csPacketCount);

	if (::GetTickCount() - m_dwStartPacketTickCount < m_dwSendCountTickGap) {
		START_LOG(cerr, L"유저 패킷 공격")
			<< BUILD_LOG(usEventID_)
			<< BUILD_LOG(nPacketCount_)
			<< END_LOG;

		if (m_bHackCheckEnable) { // 로그는 남기고 옵션이 켜져 있을때만 유저를 종료시킨다.
			return false;
		}
	}
	m_mapUserPacketSendCount.clear();
	m_dwStartPacketTickCount = ::GetTickCount();

	return true;
}