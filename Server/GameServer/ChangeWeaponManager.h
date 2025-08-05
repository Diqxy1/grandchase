#pragma once
#include <ToString.h>
#include <KNCSingleton.h>
#include <Thread/Locker.h>
#include <RTTI.H>
#include <map>
#include "PrimitiveTypedef.h"

class KLuaManager;

class KChangeWeaponManager
{
    DeclareSingleton( KChangeWeaponManager );
    NiDeclareRootRTTI( KChangeWeaponManager );
    DeclToStringW;

public:
    KChangeWeaponManager(void);
    ~KChangeWeaponManager(void);

public:
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void DumpInfo();

    bool IsPossibleLv( IN const int& nCharType, IN const int& nLv );
    bool GetUnlockItemList( OUT std::map<GCITEMID,int>& mapItem );

private:
    bool LoadMinLv( IN KLuaManager& kLuaMng, OUT std::map<int,int>& mapData );
    bool LoadUnLockItem( IN KLuaManager& kLuaMng, OUT std::map<GCITEMID,int>& mapData );

protected:
    mutable KncCriticalSection  m_csChangeWeapon;
    std::map<int,int>           m_mapMinLv;         // CharacterType, Level
    std::map<GCITEMID,int>      m_mapUnlockItem;    // ItemID, Count
};

DefSingletonInline( KChangeWeaponManager );
DeclOstmOperatorA( KChangeWeaponManager );
