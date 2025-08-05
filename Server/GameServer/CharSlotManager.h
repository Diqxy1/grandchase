#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include "CommonPacket.h"

class KLuaManager;

class KCharSlotManager
{
    DeclareSingleton( KCharSlotManager );
    NiDeclareRootRTTI( KCharSlotManager );
    DeclToStringW;

public:
    KCharSlotManager(void);
    ~KCharSlotManager(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool GetAddSlotNum( IN const GCITEMID& ItemID, OUT int& nAddSlotNum );
    DWORD GetMaxSlotNum(){ KLocker lock( m_csCharSlot ); return m_dwMaxSlotNum; }
    void GetCharSlotItemList( OUT std::map<GCITEMID, int>& mapItem_ );

protected:
    bool LoadCharSlotItemInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<GCITEMID, int>& mapCharSlotItemInfo );

protected:
    mutable KncCriticalSection          m_csCharSlot;
    DWORD                               m_dwMaxSlotNum;
    std::map<GCITEMID, int>             m_mapCharSlotItemInfo;
};

DefSingletonInline( KCharSlotManager );
DeclOstmOperatorA( KCharSlotManager );
