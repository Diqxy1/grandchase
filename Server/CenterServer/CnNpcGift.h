#pragma once
#include <map>
#include <vector>
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>

class KCnNpcGift
{
    DeclareSingleton( KCnNpcGift );
    NiDeclareRootRTTI( KCnNpcGift );
    DeclToStringW;

public:
    KCnNpcGift(void);
    ~KCnNpcGift(void);

    void Tick();
    void UpdateEventInfo( IN const std::map<int, std::vector< KDropItemInfo > >& mapEventInfo_ );
    void GetEventInfo( OUT std::map<int, std::vector< KDropItemInfo > >& mapEventInfo_ );
    void DumpList();

protected:
    DWORD GetTick()                 { return m_dwTick; }
    void SetTick()                  { m_dwTick = ::GetTickCount(); }
    DWORD GetTickGap()              { return m_dwTickGap;}

protected:
    mutable KncCriticalSection                      m_csNpcGift;
    std::map<int, std::vector< KDropItemInfo > >    m_mapNpcGiftInfo;

    DWORD                                           m_dwTick;
    DWORD                                           m_dwTickGap;
};

DefSingletonInline( KCnNpcGift );
DeclOstmOperatorA( KCnNpcGift );