#pragma once
#include "Socket/SocketObject.h"
#include "SubThread.h"
#include "NUserEvent.h"
#include "CenterEvent.h"
#include "CYOUBilling/CYOUBillingEvent.h"
#include "odbc/Odbc.h"

#define TIME_OUT_ELAPSED_TIME_LIMIT 8000
#define MAX_PACKET_SIZE_OF_CYOUBILLING 1024
#define CASH_EXCHANGE_RATIO 0.2

class KCenterSimLayer;

/* 중국 빌링은 센터 서버에 구현되어 있으며
   게임 서버에 구현되어 있는 다른 빌링들과 공용으로 사용할 수 있도록
   BillDB 클래스를 Common으로 옮기는 것을 고려해야 한다
   중국은 접혔으니 더 이상 관리는 하지 않고 추후 센터에 빌링이 추가될 경우 옮기면 좋겠다 */
class KCnCYOUAuth : public KSubThread< KNUserEvent >//, KBillDB
{
    typedef KCenterSimLayer SLayerType;

public:
	KCnCYOUAuth(void) {};
	~KCnCYOUAuth(void) {};

    // derived from KThread
    bool Begin();
    void Run();

    static void InitCYOUBillingInfo( 
        const char* szIP,
        USHORT usPort,
        const char* szODBC, 
        const USHORT usZoneID_, 
        const USHORT usGroupID_,
        const float fRatio_ );

    static void InitFileDSN_CASH_DB( const char* szFileDSN );
    bool DriveConnect_forCashDB();

private:
    void ProcessIntEvent( const KIntEvent& kIntEvent_ );
    void InitResultData();

    enum PORT_TYPE
    {
        PT_BILLING,
        NUM_PORT_TYPE,
    };

    enum CASH_UPDATE_TYPE {
        REFILL_CASH = 1,
        USE_CASH = 2,
        REFUND_CASH = 3,
    };

    enum ITEMID {
        REFILL_ITEMID = 999,
    };

    class KSocketCYOUBilling : public KSocketObject
    {
    public:
        void OnRecvCompleted( DWORD dwTransfered );
        void OnSocketError();
        void SocketClose(); // for test
        KCnCYOUAuth* m_pkCYOUBilling;
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
        //std::string         m_strBillingServerIP;                  // billing server ip
        //unsigned short      m_usPort[NUM_PORT_TYPE];    // billing server port
        int m_nCurrentIndex;
        USHORT m_usZoneID; // a.k.a Section ID 
        USHORT m_usGroupID;

    } ms_ComnData;

    static USHORT m_usPacketSequence;
    KODBC m_kCashODBC; // 캐시(빌링) DB 를 위한 ODBC 객체
    static std::wstring ms_wstrFileDSN_forCashDB;
    static float ms_CashExchangeRatio;
    int m_nServerKeepAliveFailCount;

    bool WaitEvents( IN const PORT_TYPE ePortType, IN const std::wstring& strLogin  );
    bool WaitSend( IN const PORT_TYPE ePortType, IN const std::wstring& strEvent );
    void Reconnect();
    bool SendCYOUPacket( const PORT_TYPE ePortType, const char* kEvent, const int nSize_ );
    void UserAuthRequest( IN KCYOUAuthUserInfo& kUserInfo_, OUT KENU_VERIFY_ACCOUNT_REQ& kAck_ );
    bool ServerLoginRequest( IN const KCYOUServerInfo& kServerInfo_, OUT USHORT& nRet_ );
    bool ServerRestart( IN const KCYOUServerInfo& kServerInfo_ );
    bool UserLogin( IN const KCYOUAuthUserInfo& kUserInfo_, OUT KECN_CYOU_USER_LOGIN_ACK& kPacket );
    bool UserLogout( IN const KCYOUAuthUserInfo& kUserInfo_ );
    bool ServerKeepAliveRequest( IN const KCYOUServerInfo& kServerInfo_ );
    bool UserHeartBeat( IN const KCYOUAuthUserInfo& kUserInfo_, OUT KECN_CYOU_USER_HEART_BEAT_ACK& kPacket_ );
    bool ChangeCash( IN const KCYOUCashData& kCashInfo_, OUT KCYOUCashPoint& kResult_ );
    bool GetRemainPoint( IN const KCYOUCashData& kCashInfo_, OUT KCYOUCashPoint& kResult_ );
    bool BuyLog( IN const KCYOUCashData& kCashInfo_ );
    bool AgreementConfirm( IN const KCYOUAuthUserInfo& kUserInfo_ );

    bool UpdateCash( IN const DWORD dwUserUID_, IN const int nUseType_, IN const int nCash_, IN const GCITEMUID waitUID_ = 0 ); // 충전, 사용, 환불
    void GetCurrentCash( IN const DWORD dwUserUID_, OUT DWORD& nPoint_ );


    INT_DECL_ON_FUNC( ENU_VERIFY_ACCOUNT_REQ );
   _INT_DECL_ON_FUNC( ENU_SERVER_LOGIN_REQ, KCYOUServerInfo );
   _INT_DECL_ON_FUNC( ENU_USER_LOGIN_REQ, KCYOUAuthUserInfo );
   _INT_DECL_ON_FUNC( ENU_USER_LOGOUT_REQ, KCYOUAuthUserInfo );
   _INT_DECL_ON_FUNC( ENU_SERVER_HEART_BEAT_REQ, KCYOUServerInfo );
   _INT_DECL_ON_FUNC( ENU_USER_HEARTBEAT_REQ, KCYOUAuthUserInfo );
   _INT_DECL_ON_FUNC( ENU_CYOU_CHANGE_CASH_REQ, KCYOUCashData );
   _INT_DECL_ON_FUNC( ENU_CYOU_GET_REMAIN_CASH_REQ, KCYOUCashData );
   _INT_DECL_ON_FUNC( ENU_CYOU_BUY_LOG_REQ, KCYOUCashData );
   _INT_DECL_ON_FUNC( ENU_USER_AGREEMENT_REQ, KCYOUAuthUserInfo );
};