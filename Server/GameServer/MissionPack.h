#pragma once

#include <set>
#include <map>

#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <ToString.h>
#include <RTTI.H>
#include "UserPacket.h"


class KLuaManager;

class KMissionPack
{
    DeclareSingleton( KMissionPack );
    NiDeclareRootRTTI( KMissionPack );
    DeclToStringW;

public:
    KMissionPack(void);
    ~KMissionPack(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void GetMissionList( OUT KEVENT_MISSION_PACK_LIST_NOT& mapMissionPack_ );
    bool GetMissionPackInfo( IN DWORD dwItemID_, OUT std::set<int>& setMissionList );

private:

protected:
    bool LoadMissionList( IN KLuaManager& kLuaMng_, OUT std::set<int>& setMissionList_ );

    mutable KncCriticalSection      m_csMissionPack;
    KEVENT_MISSION_PACK_LIST_NOT    m_mapMissionPackList;
};

DefSingletonInline( KMissionPack );
DeclOstmOperatorA( KMissionPack );
