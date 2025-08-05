#pragma once

#include "Socket/SocketObject.h"
#include "UserEvent.h"
#include "SubThread.h"
#include "UserPacket.h"
#include "PayLetterEvent.h"
#include "BillDB.h"

#define TIME_OUT_ELAPSED_TIME_LIMIT 800000
#define HEART_BIT_PERIOD 60000

class KPayLetterBilling : public KSubThread< KUserEvent >, KBillDB
{
    typedef KGSSimLayer SLayerType;

public:
    KPayLetterBilling( void );
    virtual ~KPayLetterBilling( void );

    // derived from KThread
    virtual bool Begin();
    virtual void Run();

    static void InitPayLetterBillingInfo(
        const char*     szIP, 
        unsigned short  usPort,
        const char*     szODBC ); // 초기화 패킷이 없는건가?

protected:
    // derived from KSubThread
    virtual void ProcessIntEvent( const KIntEvent& kIntEvent_ );

    void InitResultData();

    enum PORT_TYPE
    {
        PT_BILLING,
        NUM_PORT_TYPE,
    };

    class KSocketPayLetter : public KSocketObject
    {
    public:
        void OnRecvCompleted( DWORD dwTransfered );
        void OnSocketError();
        void SocketClose(); // for test
        KPayLetterBilling* m_pkPayLetter;
        DWORD              m_dwLastHeartBitTime;
    } m_kSock[NUM_PORT_TYPE];

    HANDLE              m_hEvents[NUM_PORT_TYPE * 2];   // send, receive
    KIntEventPtr        m_spEvent;
    DWORD               m_dwWaitTime; // waiting time for ACK
    HANDLE              m_hEndEvent[NUM_PORT_TYPE];

    KPayLetterEvent     m_kResultEvent;
    DWORD               m_dwThreadID;

    static struct KCommonData
    {
        KncCriticalSection  m_cs;
        std::string         m_strBillingServerIP;                  // billing server ip
        unsigned short      m_usPort[NUM_PORT_TYPE];    // billing server port
        unsigned char       m_ucServerID;
        unsigned char       m_ucGameID;
    } ms_ComnData;

    bool WaitEvents( IN const PORT_TYPE ePortType, IN const std::wstring& strLogin  );
    bool WaitSend( IN const PORT_TYPE ePortType, IN const std::wstring& strEvent );

    bool GetCurrentCashPoint( const std::wstring& strLogin, KCashInfo& kCashInfo );
    void InitializeConnection( const PORT_TYPE ePortType );

    void CopyStringToNCashPacketA( char* pbyteDest, int iSizeDest, const char* szSrc, int iLenSrcStr );
    void SendPayLetterPacket( const PORT_TYPE ePortType, const KPayLetterEvent& kEvent );
    bool HandleErrorNot( const KPayLetterEvent& kEvent );

    bool BuyAndDeduct( const std::wstring& strLogin, IN OUT KEVENT_PAYLETTER_BUY& kBuyPacket, KEVENT_BUY_FOR_CASH_REQ& kPacket, std::wstring& wstrItemName, KEVENT_BUY_FOR_CASH_ACK& kAck, __int64& BillingKey );
    void CancelBuyProcess();

    void KeepAliveNot( const PORT_TYPE ePortType );
    void LogQuery( IN const std::wstring& strLastQuery, IN const std::wstring& strLogin );

    void Reconnect();

   _INT_DECL_ON_FUNC( EVENT_CURRENT_CASH_POINT_REQ, KCashInfo );   // 잔여 캐쉬량
    INT_DECL_ON_FUNC( EVENT_BUY_FOR_CASH_REQ );
};
class KBR_PayLetterBilling : public KPayLetterBilling {};
class KID_PayLetterBilling : public KPayLetterBilling {};
