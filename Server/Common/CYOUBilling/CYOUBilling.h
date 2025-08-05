#pragma once

#include "Socket/SocketObject.h"
#include "../GameServer/UserEvent.h"
#include "SubThread.h"
#include "UserPacket.h"
#include "CYOUBillingEvent.h"
#include "../GameServer/BillDB.h"

#define TIME_OUT_ELAPSED_TIME_LIMIT 800000

class CYOUBilling : public KSubThread< KUserEvent >, KBillDB
{
	typedef KGSSimLayer SLayerType;

public:
	CYOUBilling(void);
	~CYOUBilling();

	bool Begin();
	void Run();

	// 복수개의 빌링 서버 접속을 고려해야 한다.
	static void InitCYOUBillingInfo(
		const char* szIP,
		USHORT usPort,
		const char* szODBC,
		const char* szCashODBC, const USHORT usZoneID_, const USHORT usGroupID_);

	static void InitFileDSN_CASH_DB(const char* szFileDSN);
	bool DriveConnect_forCashDB();

private:

	enum CASH_UPDATE_TYPE {
		REFILL_CASH = 1,
		USE_CASH = 2,
		REFUND_CASH = 3,
	};

	void ProcessIntEvent(const KIntEvent& kIntEvent_);
	void InitResultData();

	enum PORT_TYPE
	{
		PT_BILLING,
		NUM_PORT_TYPE,
	};

	class KSocketCYOUBilling : public KSocketObject
	{
	public:
		void OnRecvCompleted(DWORD dwTransfered);
		void OnSocketError();
		void SocketClose(); // for test
		CYOUBilling* m_pkCYOUBilling;
		DWORD m_dwLastHeartBeatTime;
	} m_kSock[NUM_PORT_TYPE];

	HANDLE              m_hEvents[NUM_PORT_TYPE * 2];   // send, receive
	KIntEventPtr        m_spEvent;
	DWORD               m_dwWaitTime; // waiting time for ACK
	HANDLE              m_hEndEvent[NUM_PORT_TYPE];

	CYOUBillingEvent    m_kResultEvent;
	DWORD               m_dwThreadID;

	static struct KCommonData
	{
		KncCriticalSection  m_cs;
		std::vector< std::pair< std::string, USHORT > > m_vecBillingServerIP_Port; // billing server ip
		int m_nCurrentIndex;
		USHORT m_usZoneID; // a.k.a Section ID
		USHORT m_usGroupID;
	} ms_ComnData;

	static USHORT m_usPacketSequence; //
	KODBC m_kCashODBC; // 캐시(빌링) DB 를 위한 ODBC 객체
	static std::wstring ms_wstrFileDSN_forCashDB;

	bool WaitEvents(IN const PORT_TYPE ePortType, IN const std::wstring& strLogin);
	bool WaitSend(IN const PORT_TYPE ePortType, IN const std::wstring& strEvent);

	void KeepAliveNot(const PORT_TYPE ePortType);
	void Reconnect();

	void ServerKeepAliveRequest(IN const KCYOUServerInfo& kServerInfo_);

	void SendCYOUPacket(const PORT_TYPE ePortType, const char* kEvent, const int nSize);
	void AuthUserRequest(IN const KCYOUAuthUserInfo& kUserInfo_);
	void AgreementConfirm(IN const KCYOUAuthUserInfo& kUserInfo_);
	void CharacterSwitch(IN const KCYOUAuthUserInfo& kUserInfo_);

	void GetCurrentCash(IN const DWORD dwUserUID_, OUT DWORD& nPoint_);
	bool UpdateCash(IN const DWORD dwUserUID_, IN const int nUseType_, IN const int nCash_, IN const GCITEMUID waitUID_ = 0);
	void LogQuery(IN const std::wstring& strLastQuery_, IN const std::wstring& strLogin_);

	_INT_DECL_ON_FUNC(EVENT_SERVER_LOGIN_REQ, KCYOUServerInfo);
	_INT_DECL_ON_FUNC(EVENT_SERVER_HEART_BEAT_REQ, KCYOUServerInfo);
	_INT_DECL_ON_FUNC(EVENT_CYOU_USER_HEART_BEAT_REQ, KCYOUAuthUserInfo);
	_INT_DECL_ON_FUNC(EVENT_CYOU_USER_LOGOUT_REQ, KCYOUAuthUserInfo);
	_INT_DECL_ON_FUNC(EVENT_VERIFY_ACCOUNT_REQ, KCYOUAuthUserInfo);
	_INT_DECL_ON_FUNC(EVENT_CYOU_SERVER_RESTART_REQ, KCYOUServerInfo);
	_INT_DECL_ON_FUNC(EVENT_CYOU_USER_SWITCH_CHARACTER_REQ, KCYOUAuthUserInfo);
	_INT_DECL_ON_FUNC(EVENT_CYOU_CASH_CHANGE_REQ, KCYOUCashData);
	INT_DECL_ON_FUNC(EVENT_BUY_FOR_CASH_REQ);
	_INT_DECL_ON_FUNC(EVENT_CURRENT_CASH_POINT_REQ, KCashInfo);

	//#END_OF_CYOU_DECL
};
