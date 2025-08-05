#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include "NUserPacket.h"

class KLuaManager;
class KCnClientScriptManager
{
    DeclareSingleton( KCnClientScriptManager );
    NiDeclareRootRTTI( KCnClientScriptManager );
    DeclToStringW;

public:
    KCnClientScriptManager(void);
    ~KCnClientScriptManager(void);

    bool LoadScript();
    bool LoadMonsterInfo( KLuaManager& kLuaMng_ );

    void GetMonsterInfo( OUT std::map< int, KSimpleMonsterInfo >& mapInfo_ );

    void DumpMonsterInfo();

protected:
    mutable KncCriticalSection          m_csMonsterInfo;
    std::map< int, KSimpleMonsterInfo > m_mapMonsterInfo;   // 몬스터 정보
};
DefSingletonInline( KCnClientScriptManager );
DeclOstmOperatorA( KCnClientScriptManager );