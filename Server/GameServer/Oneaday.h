#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
//#include <Thread/Locker.h>
#include "KncTimer.h"
#include <vector>

//#include "CommonPacket.h"

class KOneaday
{
    DeclareSingleton( KOneaday );
    NiDeclareRootRTTI( KOneaday );
    DeclToStringW;

public:
    KOneaday(void);
    ~KOneaday(void);

    void GetInfo( OUT std::vector<KOneadayItem>& vecOneaday_ );
    void SetInfo( OUT std::vector<KOneadayItem>& vecOneaday_ );

    void UpdateRequest();
    void Tick();
    void UpdateLimitedCount( GCITEMID itemID_ );
    bool CheckItemRemainCount( IN GCITEMID itemID_ );
private:
    std::vector<KOneadayItem> m_vecOneadayItemList;
    mutable KncCriticalSection m_csOneaday;
    KncTimer m_kTimer;
};

DefSingletonInline( KOneaday );
DeclOstmOperatorA( KOneaday );