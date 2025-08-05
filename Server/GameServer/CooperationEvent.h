#pragma once
// 브라질 제휴 이벤트.

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include "PrimitiveTypedef.h"

class KLuaManager;
struct KCoopEventItem;
class KCooperationEvent
{
    DeclareSingleton( KCooperationEvent );
    NiDeclareRootRTTI( KCooperationEvent );
    DeclToStringW;

public:
    KCooperationEvent(void);
    ~KCooperationEvent(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void Tick();

    int GetEventID();
    GCITEMID GetMoneyItem();
    bool IsEventerTerm();
    bool GetItemList( OUT std::map<int,KCoopEventItem>& mapItemList );
    bool GetItemInfo( IN const int& nSlotID, OUT KCoopEventItem& kItemInfo );
    void PrintItemList();

protected:
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadItems( IN KLuaManager& kLuaMng_, IN int nIndex, OUT KCoopEventItem& kItem );
    void Update();

protected:
    mutable KncCriticalSection      m_csCoopEvent;
    int                             m_nEventID;
    time_t                          m_tmBegin;
    time_t                          m_tmEnd;
    GCITEMID                           m_dwMoneyItemID;
    std::map<int,KCoopEventItem>    m_mapItemList;

    bool        m_bIsEventTerm;
    DWORD       m_dwTickGap;
    DWORD       m_dwLastTick;

};

DefSingletonInline( KCooperationEvent );
DeclOstmOperatorA( KCooperationEvent );