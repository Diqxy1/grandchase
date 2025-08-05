#pragma once
#include "CommonPacket.h"
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>

class KCnSubscriptionManager
{
    DeclareSingleton( KCnSubscriptionManager );
    NiDeclareRootRTTI( KCnSubscriptionManager );
    DeclToStringW;

public:
    KCnSubscriptionManager(void);
    ~KCnSubscriptionManager(void);

    void Tick();
    void SendDBSubscriptionInfoUpdateReq();
    void UpdateEventInfoFromDB( IN const std::map< int, KSubscription >& mapInfo_ );
    void GetEventInfo( OUT std::map< int, KSubscription >& mapInfo_ );
    bool UpdateCurrentGiftNum( IN const int nEventUID_, const int nNum_ );
    void DumpList();

private:
    DWORD GetTick()                 { return m_dwTick; }
    void SetTick()                  { m_dwTick = ::GetTickCount(); }
    DWORD GetTickGap()              { return m_dwTickGap;}

private:
    mutable KncCriticalSection      m_csCnSubscription;
    std::map< int, KSubscription >  m_mapCnSubscriptionInfo;  // map< 이벤트UID, 응모이벤트정보 >

    DWORD                           m_dwTick;
    DWORD                           m_dwTickGap;
};
DefSingletonInline( KCnSubscriptionManager );
DeclOstmOperatorA( KCnSubscriptionManager );