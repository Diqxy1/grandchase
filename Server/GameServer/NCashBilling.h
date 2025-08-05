#pragma once

#include "Socket/SocketObject.h"
#include "UserEvent.h"
#include "SubThread.h"
#include "UserPacket.h"
#include "NCashEvent.h"
#include "BillDB.h"

#define TIME_OUT_TOLERANCE_OF_CLIENT 100000
#define HEART_BIT_PERIOD 60000
//#define DISCOUNT_FACTOR -3

class KNCashBilling : public KSubThread< KUserEvent >, KBillDB
{
    typedef KGSSimLayer SLayerType;

public:
    KNCashBilling( void );
    virtual ~KNCashBilling( void );

    // derived from KThread
    virtual bool Begin();
    virtual void Run();

    static void InitNCashBillingInfo(
        const char*     szIP,
        unsigned short  usPort,
        unsigned short  usServiceID, // 해당 게임 ID.
        unsigned short  usCPID,      // 접속하는 서버가 갖는 Unique Value.
        const char*     szODBC );

protected:
    // derived from KSubThread
    virtual void ProcessIntEvent( const KIntEvent& kIntEvent_ );

    void InitResultData();

    enum PORT_TYPE
    {
        PT_BILLING,
        NUM_PORT_TYPE,
    };

    enum VIRTUAL_CASH_RETURN
    {
        NOT_ENOUGH_VIRTUAL_CASH,
        NO_USE_VIRTUAL_CASH,
        USE_VIRTUAL_CASH,
        OVER_VIRTUAL_CASH_RATIO = -8
    };

    class KSocketNCash : public KSocketObject
    {
    public:
        void OnRecvCompleted( DWORD dwTransfered );
        void OnSocketError();
        KNCashBilling*  m_pkNCash;
        DWORD           m_dwLastHeartBitTime;
    } m_kSock[NUM_PORT_TYPE];

    DWORD               m_dwThreadID;
    HANDLE              m_hEvents[NUM_PORT_TYPE * 2];   // send, receive
    KIntEventPtr        m_spEvent;
    DWORD               m_dwWaitTime;
    HANDLE              m_hEndEvent[NUM_PORT_TYPE];

    KNCashEvent         m_kResultEvent;

    static struct KCommonData
    {
        KncCriticalSection  m_cs;
        std::string         m_strBSIP;                  // billing server ip
        unsigned short      m_usPort[NUM_PORT_TYPE];    // billing server port
        unsigned short      m_usServiceID;
        unsigned short      m_usCPID;
    } ms_ComnData;

    bool WaitEvents( IN const PORT_TYPE ePortType, IN const std::wstring& strLogin  );
    void GetCurrentCashPoint( const std::wstring& strLogin, KCashInfo& cashInfo );
    void DecreaseCashPoint( IN const std::wstring& strLogin, IN OUT KCashInfo& cashInfo, IN __int64 biItemSetUID, IN GCITEMID dwItemID, IN int nCount, IN int nPeriod, IN std::wstring wstrItemName, OUT int& nTransactionID );
    int HandleErrorNot( const KNCashEvent& kEvent );
    void SendNCashPacket( const PORT_TYPE ePortType, const KNCashEvent& kEvent );
    void InitializeConnection( const PORT_TYPE ePortType );
    void CheckConnection( const PORT_TYPE ePortType );
    void CopyStringToNCashPacketA( BYTE* pbyteDest, int iSizeDest, const char* szSrc, int iLenSrcStr );
    void CopyStringToNCashPacketW( BYTE* pbyteDest, int iSizeDest, const wchar_t* szSrc, int iLenSrcStr );
    void LogQuery( IN const std::wstring& strLastQuery, IN const std::wstring& strLogin );
    void SendGiftItem( IN const std::wstring& strSenderLogin, IN const std::wstring& strReceiverLogin, IN OUT KCashInfo& cashInfo, IN __int64 biItemSetUID, IN GCITEMID dwItemID, IN int nCount, IN int nPeriod, IN std::wstring wstrItemName, OUT int& nTransactionID );

    int CheckVirtualCash( IN KVirtualCashInfo& kVirtualCashInfo );
    void DiscountCash( IN KVirtualCashInfo& kVirtualCashInfo_, IN OUT KCashInfo& cashInfo_ );

    _INT_DECL_ON_FUNC( EVENT_CURRENT_CASH_POINT_REQ, KCashInfo );   // 잔여 캐쉬량
    INT_DECL_ON_FUNC( EVENT_BUY_FOR_CASH_REQ );
};
