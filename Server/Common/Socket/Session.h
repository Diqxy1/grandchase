#pragma once
#include <WinSock2.h>
#include "Performer.h"
#include "Thread/Thread.h"
#include "Serializer/Serializer.h"
#include "Serializer/SerBuffer.h"
#include "SocketObject.h"
#include "KncSecurity/KncSecurity.h"
#include "KncException.h"

class KIocp;
class KSession : public KPerformer, public KThread
{
	NiDeclareRTTI;
	DeclToStringW;

	KSession(); // 구현부는 없음. 사용되면 안된다. (사용하면 링크에러남.)
	KSession(const KSession& right);
	KSession& operator=(const KSession& right);

public:
	typedef USHORT PACKET_HEADER; // 패킷 헤더

	enum {
		EVENT_HEART_BIT = 0,
		EVENT_ACCEPT_CONNECTION_NOT = 1,    // custom event ID들은 2부터 시작한다.
		SECURE_KEY_WAITING_TIME = 3000, // 접속시 보안키를 받기까지 대기하는 최대시간
	};

#undef _ENUM
#define _ENUM( id ) id,
	enum DISCONNECT_REASON_WHY
	{
#include "DisconnectReason_def.h"
	};

	KSession(bool bIsProxy);
	virtual ~KSession(void);

	// derived from KThread
	virtual bool Begin();

	// derived from KPerformer
	virtual void Tick();    // reserver destroy, check heartbit

	void ReserveDestroy(int nReason = 0);
	bool IsReserveDestroy() { return m_bDestroyReserved; }      // Destroy 예약 되었나?
	void SetDisconnReason(int nReason);
	int GetDisconnReason() { return m_nDisconnectReason; }

	// 060321. florist. RefCount가 남아있어 삭제되지 않을 가능성이 있는 경우를 검사한다.
	virtual bool CheckExceedRefCount(int nCount);

	template < typename T >
	bool SendPacket(unsigned short usEventID, T& data, bool bLogging = true, bool bCompress = false);
	bool SendPacket(KSerBuffer& kbuff, const wchar_t* szDataDesc);
	bool SendID(unsigned short usEventID);
	//bool InitRecv();

	bool IsConnected() const { return m_kSockObj.IsConnected(); }
	unsigned int GetIP() const { return m_kSockObj.GetIP(); }
	const char* GetIPStr() const { return m_kSockObj.GetIPStr(); }
	unsigned short GetPort() const { return m_kSockObj.GetPort(); }
	virtual KSocketObject* GetSockObj() { return &m_kSockObj; }

	static void DumpMaxSendData(std::wostream& stm);
	static void ResetMaxSendData();
	// 091113 tgkwon. SequenceNum 체크 추가.( 중복 패킷 방지 )
	static void DumpPacketAuthInfo(std::wostream& stm);

	// use proxy-child only.
	virtual bool Init(bool bUseIocp);
	virtual bool Connect(const char* szIP, unsigned short usPort, bool bNagleAlgOn);

	// user unproxy-child only.
	virtual void OnAcceptConnection(); // accept 되자마자 호출. 이곳에서 인증키를 생성해 클라이언트로 전송한다.
	virtual void    SetKey(DWORD dwKey) { m_kSockObj.SetKey(dwKey); }
	DWORD   GetKey() const { return m_kSockObj.GetKey(); }

	// 091113 tgkwon. SequenceNum 체크 추가.( 중복 패킷 방지 )
	static void InitCheckSequenceNumInfo(IN bool bCheckSequenceNum_, IN UINT uiPacketAuthLimitNum_) { m_bCheckSequenceNum = bCheckSequenceNum_; m_uiPacketAuthLimitNum = uiPacketAuthLimitNum_; }
	UINT GetPacketAuthLimitNum() const { return m_uiPacketAuthLimitNum; }
	bool IsCheckSequenceNum() { return m_bCheckSequenceNum; }
	void IncreasePacketAuthFailCnt() { KLocker lock(m_csPacketAuthFailCnt); ++m_uiPacketAuthFailCnt; }
	UINT GetPacketAuthFailCnt() { KLocker lock(m_csPacketAuthFailCnt); return m_uiPacketAuthFailCnt; }

	static void GetOverRecvEvent(OUT std::vector<std::wstring>& vecOverRecvEvent);
	static void GetPacketCount(OUT std::map<USHORT, __int64>& mapData);

	static void InitPacketHackCheck(IN bool bCheck_, IN int nPacketLimitCount_, IN int nLogPacketCount_, IN DWORD dwSendCountTickGap) { m_bHackCheckEnable = bCheck_; m_nPacketSendCount = nPacketLimitCount_; m_nLogPacketSendCount = nLogPacketCount_; m_dwSendCountTickGap = dwSendCountTickGap; }
	int GetPacketSendCount() { KLocker lock(m_csPacketCount); return m_nPacketSendCount; }
	int GetLogPacketSendCount() { KLocker lock(m_csPacketCount); return m_nLogPacketSendCount; }

protected:  // util function
	//내부에서만 호출될것 같음...나중에 밖으로 나올것 같으면 잘 조지삼..
	void SetSendQueueSize(int nSize) { m_kSockObj.SetSendQueueSize(nSize); }

	//void OnSendCompleted( DWORD dwTransfered );
	//void OnRecvCompleted( DWORD dwTransfered );
	void OnRecvCompleted(IN const KncSecurity::KByteStream& payload);
	// derived from KThread
	virtual void Run();
	// derive to child.
	virtual void OnSocketError();   ///< 050129. WSASend, IOCP등에서 소켓 오류를 발견한 경우 호출.
	virtual void OnDestroy();       ///< 050129. multi thread safe하도록 수정함
	void DeleteCompKey();

	class KSkSession : public KSocketObject {
	public:
		virtual void OnRecvCompleted(DWORD dwTransfered);
		virtual void OnSocketError() { m_pkSession->OnSocketError(); }
		virtual void OnAcceptConnection() { m_pkSession->OnAcceptConnection(); }
		KSession* m_pkSession;
	} m_kSockObj;

	void CheckRecvPacketOver(IN const unsigned short usEventID);
	void CheckMaxPacketInfo(IN const size_t PacketSize, IN const size_t DataSize, IN const wchar_t* szDataDesc);

	void IncreasePacketCount(IN const USHORT& usEventID);
	bool IncreaseUserPacketSendCount(IN const USHORT& usEventID, OUT int& nPacketCount);
	bool CheckUserPacketSendCount(IN const USHORT& usEventID, IN const int& nPacketCount);

public:
	static DWORD        m_dwHBGap;          // 061024. kkurrung. 네트웍 환경이 좋지 못한 지역 서비스시 HB 간격 조절

protected:
	bool                m_bUseIocp;     ///< if use Iocp
	bool                m_bIsProxy;     ///< Proxy인 경우, H.B를 보내고, Connect를 할 수 있다.
	DWORD               m_dwHBTick;     // heart bit tick
	bool                m_bDestroyReserved;     ///< 050129. 생성한 스레드에서 소멸 처리.

	static struct KMaxPacketInfo {
		KncCriticalSection  m_cs;           // lua script에서의 작업에 대해 보호한다
		std::wstring        m_strName;
		std::wstring        m_strDataDesc;
		size_t              m_nDataSize;    // 순수 게임 데이터만 고려된 사이즈
		size_t              m_nPacketSize;  // 암호화 처리를 거친 후, 실제 전송되는 데이터 사이즈
		CTime               m_cTime;
	} ms_kMaxPacketInfo;

	static struct KPacketAuthInfo {
		KncCriticalSection  m_cs;           // lua script에서의 작업에 대해 보호한다
		std::wstring        m_strName;
		UINT                m_uiMaxFailCnt;
		UINT                m_uiTotalFailCnt;
		CTime               m_cRegTime;
	} ms_kPacketAuthInfo;

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
	int                 m_nDisconnectReason;

	// 091113 tgkwon. SequenceNum 체크 추가.( 중복 패킷 방지 )
	KncCriticalSection  m_csPacketAuthFailCnt;
	static bool       m_bCheckSequenceNum;
	static UINT       m_uiPacketAuthLimitNum;
	UINT                m_uiPacketAuthFailCnt;

	std::vector<unsigned short> m_vecRecvEventTemp; // 지정한 Tick Gap 사이에 받는 EventID Pushback
	DWORD               m_dwRecvCountTick;
	DWORD               m_dwRecvCountTickGap; // 100ms
	unsigned short      m_usOverCount; // 공격으로 간주하는 Event 개수
	bool                m_bCheckedOverCount; // 기록되었는지? (한번 기록되면 더이상 기록 안함)
	USHORT              m_usOverCountGap; // 마지막 기록부터 몇개 이상 초과하면 기록을 할 것인지
	USHORT              m_usOverCountWrite; // 기록횟수
	USHORT              m_usMaxOverCountWrite; // 최대기록가능한 횟수

	static KncCriticalSection m_csOverRecvEvent;
	static std::vector<std::wstring> m_vecOverRecvEvent;

	static KncCriticalSection m_csPacketCount;
	static std::map<USHORT, __int64> m_maPacketCount;

	// 유저별 패킷 카운트
	KncCriticalSection               m_csPacketHackCheck;
	std::map< USHORT, int >          m_mapUserPacketSendCount;
	DWORD                            m_dwStartPacketTickCount;

	static bool                      m_bHackCheckEnable;
	static int                       m_nLogPacketSendCount;
	static int                       m_nPacketSendCount;
	static DWORD                     m_dwSendCountTickGap;

	friend class KIocp;
};

template < typename T >
bool KSession::SendPacket(unsigned short usEventID_, T& data_, bool bLogging_, bool bCompress_)
{
	KEventPtr spEvent(CreateKEvent());
	spEvent->m_usEventID = usEventID_;

	// make event object
	KSerializer         ks;

	// serialize - only data
	ks.BeginWriting(&spEvent->m_kbuff);
	ks.Put(data_);
	ks.EndWriting();
	spEvent->m_kbuff.Compress();

	// serialize - full event object
	KSerBuffer          kbuff;
	ks.BeginWriting(&kbuff);
	ks.Put(*spEvent);
	ks.EndWriting();

	// send packet
	if (!KSession::SendPacket(kbuff, spEvent->GetEventIDString()))
		return false;
	return true;
}

#define SEND_ID( id )                       LIF( SendID( EVENT_TYPE::##id ) )
#define SEND_PACKET( id )                   LIF( SendPacket( EVENT_TYPE::##id, kPacket ) )
#define SEND_DATA( id, data )               LIF( SendPacket( EVENT_TYPE::##id, data ) )
#define SEND_RECEIVED_PACKET( id )          LIF( SendPacket( EVENT_TYPE::##id, kPacket_ ) )
#define SEND_COMPRESS_PACKET( id, data )    LIF( SendPacket( EVENT_TYPE::##id, data, true, true ) )
