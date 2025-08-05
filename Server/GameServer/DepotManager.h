#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>

#include <Thread/Locker.h>
#include <PrimitiveTypedef.h>
#include <set>

struct KDepotExtend;
struct KEVENT_DEPOT_INFO_ACK;

class KLuaManager;

class KDepotManager
{
    DeclareSingleton( KDepotManager );
    NiDeclareRootRTTI( KDepotManager );
    DeclToStringW;

public:
    KDepotManager(void);
    ~KDepotManager(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void GetDepotMaxSizeInfo( OUT KEVENT_DEPOT_INFO_ACK& kData_ );
    char GetInitTabID();
    int GetInitSlotSize();
    int GetMaxSlotSize();
    void SetExtendInfo( IN KDepotExtend& kData_ );
    int GetSlotExtendSize();
    GCITEMID GetSlotExtendItemID() { return m_SlotExtendItemID; };
    GCITEMID GetTabExtendItemID() { return m_TabExtendItemID; };
    int GetMaxTabSize();
    bool LoadRestrictItem( IN OUT KLuaManager& kLuaMng_, OUT std::set<DWORD>& setRestrictItem_  );
    bool IsRestrictItem( IN GCITEMID ItemID_ );
    void GetRestrictItem( OUT std::set<GCITEMID>& setItem_ );

private:
    mutable KncCriticalSection m_csDepotInfo;
    int m_nMaxTabSize;
    int m_nMaxSlotRow;

    char m_cInitTabID;
    int m_nInitSlotSize;

    int m_nMaxSlotSize;
    int m_nExtendSlotSize;

    GCITEMID m_TabExtendItemID; // 탭 확장 아이템
    GCITEMID m_SlotExtendItemID; // 슬롯 확장 아이템

    std::set<GCITEMID> m_setRestrictItem; // 창고에 넣을 수 없는 아이템 
    mutable KncCriticalSection m_csRestrictItem;
};

DefSingletonInline( KDepotManager );
DeclOstmOperatorA( KDepotManager );
