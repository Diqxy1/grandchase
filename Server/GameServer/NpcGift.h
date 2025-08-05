#pragma once
#include <map>
#include <set>
#include <vector>
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>

struct KItemUnit;

class KNpcGift
{
    DeclareSingleton( KNpcGift );
    NiDeclareRootRTTI( KNpcGift );
    DeclToStringW;

public:
    KNpcGift(void);
    ~KNpcGift(void);

    void UpdateEventInfo( IN const std::map<int, std::vector< KDropItemInfo > >& mapEventInfo_ );
    bool GetEventItemList( IN const int nEventID_, OUT std::vector< KDropItemInfo >& vecItemList_ );
    void GetEventList( OUT std::set< int >& setEventList_ );
    void DumpList();

protected:
    mutable KncCriticalSection                      m_csNpcGift;
    std::map<int, std::vector< KDropItemInfo > >    m_mapNpcGiftInfo;
    std::set< int >                                 m_setNpcGiftEvent;
};

DefSingletonInline( KNpcGift );
DeclOstmOperatorA( KNpcGift );