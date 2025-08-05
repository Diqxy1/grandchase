#pragma once
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include "KncTimer.h"
#include <boost/array.hpp>
#include "Lottery.h"
#include "UserPacket.h"

class KLuaManager;
class KEventDungeonManager
{
    DeclareSingleton( KEventDungeonManager );
    NiDeclareRootRTTI( KEventDungeonManager );
    DeclToStringW;
public:

    KEventDungeonManager(void);
    ~KEventDungeonManager(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsEventDungeon( IN const int nModeID );
    bool GetEventDungeonInfo( IN const int nModeID, OUT KEventDungeon& kInfo );
    void GetEventDungeonList( OUT std::vector<KEventDungeon>& vecList );
    bool SendEventDungeonInfoNot();

private:
    bool LoadEventDungeon( OUT std::string& strScript );
    bool LoadTimeTable( IN KLuaManager& kLuaMng, OUT KEventDungeon& kEventdungeon_ );
    void GetEventDungeonIDList( OUT std::set<int>& setModeID_ );

protected:
    KncTimer                        m_kTickCheck;
    mutable KncCriticalSection      m_csEventDungeon;
    std::vector<KEventDungeon>      m_vecDungeonInfo;       // 영웅 던전 정보
};

DefSingletonInline( KEventDungeonManager );
DeclOstmOperatorA( KEventDungeonManager );