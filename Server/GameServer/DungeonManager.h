#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <set>
#include "CommonPacket.h"

class KLuaManager;

#define DEFAULT_USER_NUM 4

class KDungeonManager
{
    DeclareSingleton( KDungeonManager );
    NiDeclareRootRTTI( KDungeonManager );
    DeclToStringW;

public:
    KDungeonManager(void);
    ~KDungeonManager(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void GetDefaultDungeonInfo( IN const int& nGameMode, OUT KDefaultModeInfo& kDefaultModeInfo );
    bool GetDungeonRewardType( IN const int& nGameMode, OUT int& nRewardType );

protected:
    bool LoadMaxUserData( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<int,USHORT>& mapDungeonUserNumList );
    bool LoadSetData( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::set<int>& setInfo );
    bool LoadMapData( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<int,int>& mapRewardTypeInfo );

protected:
    mutable KncCriticalSection          m_csDungeonManager;
    USHORT                              m_usDefaultUserNum;
    std::map<int,USHORT>                m_mapDungeonUserNumList;
    std::set<int>                       m_setModeChangeDisableList;
    std::map<int,int>                   m_mapRewardTypeInfo;
    std::set<int>                       m_setUserSlotOpenDisableList;
};

DefSingletonInline( KDungeonManager );
DeclOstmOperatorA( KDungeonManager );