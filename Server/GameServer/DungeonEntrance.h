#pragma once
#include <vector>
#include <map>
#include <Thread/Locker.h>
#include <ToString.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include "CommonPacket.h"

struct sEntranceInfo
{
    int                 m_nDungeonID;
    std::vector<int>    m_vecConditions;
};

class KncBitStream; // 전방 선언
class KLuaManager;

class KDungeonEntrance
{
    DeclareSingleton( KDungeonEntrance );
    NiDeclareRootRTTI( KDungeonEntrance );
    DeclToStringW;
public:
    KDungeonEntrance(void);
    ~KDungeonEntrance(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool CheckDungeonEntrance( IN const std::map<int,KDungeonUserInfo>& m_mapDifficulty, IN const int& nDungeonID, IN OUT int& nDifficulty );

protected:
    bool LoadEntranceInfo( IN KLuaManager& kLuaMng, OUT sEntranceInfo& sInfo );
    bool GetEntranceInfo( IN int nID, OUT sEntranceInfo& sInfo );

    mutable KncCriticalSection          m_csEntranceInfo;
    std::map< int, sEntranceInfo>       m_mapEntranceInfo;
};

DefSingletonInline( KDungeonEntrance );
DeclOstmOperatorA( KDungeonEntrance );
