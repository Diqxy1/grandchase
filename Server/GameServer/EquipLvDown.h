#pragma once
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <ToString.h>
#include <RTTI.H>
#include "PrimitiveTypedef.h"
#include <set>

class KLuaManager;
class KEquipLvDown
{
    DeclareSingleton( KEquipLvDown );
    NiDeclareRootRTTI( KEquipLvDown );
    DeclToStringW;
public:
    KEquipLvDown(void);
    ~KEquipLvDown(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );

    bool IsEquipLvDownEnable() { KLocker lock(m_csEnable); return m_bEquipLvDownEnable; }

    bool IsMarerialItemID( IN const GCITEMID& ItemID );
    int CalcMaterialIemCount( IN const int& nItemLv, IN const int& nLvDiff );

private:
    mutable KncCriticalSection  m_csEnable;
    bool                        m_bEquipLvDownEnable; // 장착아이템레벨다운 사용할지 여부

    mutable KncCriticalSection  m_csConfig;
    std::set<GCITEMID>          m_setMaterialItemID; // 재료아이템 ItemID

    // 공식 변수
};
DefSingletonInline( KEquipLvDown );
DeclOstmOperatorA( KEquipLvDown );