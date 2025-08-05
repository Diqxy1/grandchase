#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <vector>
#include "Lua/KLuaManager.h"

class KMissionGettableCondition
{
    DeclareSingleton( KMissionGettableCondition );
    NiDeclareRootRTTI( KMissionGettableCondition );
    DeclToStringW;
public:
    KMissionGettableCondition(void);
    ~KMissionGettableCondition(void);

public:
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );

    void GetMissionGettableCondition( OUT std::map< DWORD, DWORD >& mapCondition );
    bool MissionRegistEnableCheck( IN const DWORD dwMissionID, IN std::vector< DWORD > vecFinishedMission );

private:
    bool LoadGettableCondition( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< DWORD, DWORD >& mapCondition );

private:
    mutable KncCriticalSection m_csMissionGettableCondition;

    std::map< DWORD, DWORD > m_mapGettableCondition; // < 받고자 하는 미션ID, 받기 전에 클리어 했어야 하는 미션ID >
};


DefSingletonInline( KMissionGettableCondition );
DeclOstmOperatorA( KMissionGettableCondition );