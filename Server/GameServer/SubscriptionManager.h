#pragma once
#include "CommonPacket.h"
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>

class KSubscriptionManager
{
    DeclareSingleton( KSubscriptionManager );
    NiDeclareRootRTTI( KSubscriptionManager );
    DeclToStringW;

public:
    KSubscriptionManager(void);
    ~KSubscriptionManager(void);

    void SetEventInfo( IN const std::map< int, KSubscription >& mapInfo_ );
    void GetEventInfo( OUT std::map< int, KSubscription >&  mapInfo_ );
    bool SetCurrentGiftNum( IN const int nEventUID_, const int nNum_ );
    bool GetCurrentEvent( IN const int nEventUID_, OUT KSubscription& kEventInfo_ );
    void GetCurrentGiftNum( OUT MAP_INT_INT& mapData_ );
    void DumpList();

private:
    mutable KncCriticalSection      m_csSubscription;
    std::map< int, KSubscription >  m_mapSubscriptionInfo;  // map< 이벤트UID, 응모이벤트정보 >
};
DefSingletonInline( KSubscriptionManager );
DeclOstmOperatorA( KSubscriptionManager );