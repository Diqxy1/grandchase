#pragma once

#include "Socket/SocketObject.h"
#include "IntEvent.h"
#include "UserEvent.h"
#include "SubThread.h" // INT..., thread
#include "UserPacket.h"
#include "BillDB.h"

//class KGashBillingManager;
class KGSSimLayer;
class KGashBilling : public KThread, KBillDB
{
    typedef KUserEvent  IDVenderType;
    typedef KGSSimLayer SLayerType;

public:
    KGashBilling(void);
    virtual ~KGashBilling(void);

    // derived from KThread
    virtual bool Begin();

    static void InitGashBillingInfo( const char* szIP, 
        unsigned short usPortAuthor,
        unsigned short usPortAccount,
        int iServiceCode,
        const char* szRegionCode,
        const char* szODBC );

protected:
    virtual void Run();     ///< derived from KThread
    void Tick();
    void InitResultData()
    {
        KLocker lock( m_csResult );
        m_strOutLogin.assign( "" );
        m_strOutErr.assign( "" );
        m_strOutAccType.assign( "" );
        m_nResult = -1;
    }

protected:
    enum ENUM_ACTION_TYPE {
        AUTHOR,
        ACCOUNT,
        ACTION_NUM
    };

    enum VIRTUAL_CASH_RETURN
    {
        NOT_ENOUGH_VIRTUAL_CASH,
        NO_USE_VIRTUAL_CASH,
        USE_VIRTUAL_CASH,
        OVER_VIRTUAL_CASH_RATIO = -8
    };

    class KSocketGash : public KSocketObject {
    public:
        void OnRecvCompleted( DWORD dwTransfered );
        void OnSocketError();
        KGashBilling*  m_pkGash;
    } m_kSock[ACTION_NUM];

    DWORD           m_dwThreadID;
    HANDLE          m_hEvents[ACTION_NUM * 2 + 1];   // 소켓당 이벤트 두개, 스레드 종료 이벤트 한개
    KIntEventPtr    m_spEvent;
    DWORD           m_dwWaitTime;
    // 060525. kkurrung. Recv 후 이벤트 발생을 위해
    HANDLE          m_hEndEvent[ACTION_NUM];

    KncCriticalSection  m_csResult;
    std::string         m_strOutErr;
    std::string         m_strOutLogin;
    std::string         m_strOutAccType;
    int                 m_nResult;

    static struct KCommonData {
        KncCriticalSection  m_cs;
        std::string         m_strGashIP;
        unsigned short      m_usPort[ACTION_NUM];
        std::string         m_strServiceCode;
        std::string         m_strRegionCode;
    } ms_ComnData;

    bool WaitEvents( IN const ENUM_ACTION_TYPE eAction, IN const std::wstring& strLogin  );
    void GetCurrentCashPoint( const std::wstring& strLogin, KCashInfo& cashInfo );
    void DecreaseCashPoint( const std::wstring& strLogin, KCashInfo& cashInfo, __int64 dwItemSetUID, GCITEMID dwItemID, int nCount, int m_nPeriod, std::wstring wstrItemName );

    int CheckVirtualCash( IN KVirtualCashInfo& kVirtualCashInfo );

    void LogQuery( IN const std::wstring& strLastQuery_, IN const std::wstring& strLogin_ );

    void DiscountCash( IN KVirtualCashInfo& kVirtualCashInfo_, IN OUT KCashInfo& cashInfo_ );

    _INT_DECL_ON_FUNC( EVENT_CURRENT_CASH_POINT_REQ, KCashInfo );   // 잔여 캐쉬량
    INT_DECL_ON_FUNC( EVENT_BUY_FOR_CASH_REQ );
};
