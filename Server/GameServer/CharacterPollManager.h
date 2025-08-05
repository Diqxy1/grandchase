#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <set>
#include "PrimitiveTypedef.h"

class KLuaManager;

class KCharacterPollManager
{
    DeclareSingleton( KCharacterPollManager );
    NiDeclareRootRTTI( KCharacterPollManager );
    DeclToStringW;

public:
    enum POLL_TICK_ID
    {
        PTI_TICK,
        PTI_SYNC,
        PTI_MAX,
    };
    KCharacterPollManager(void);
    ~KCharacterPollManager(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsEnablePoll();
    bool IsPollItem( GCITEMID dwItemID );
    void AddPoll( char cCharType, int nCount );
    bool IsCorrectChar( char cCharType );
    GCITEMID GetPollItemID();

    void Tick();

    void TestPoll( int nCharType, int nCount );

protected:
    void UpdateData(); // DB에 기록
    void InitTicks();
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadAdjustChar( IN KLuaManager& kLuaMng, OUT std::set<char>& setAdjustChar );
    void UpdateState();

    DWORD GetTick( int nIndex );
    DWORD GetTickGap( int nIndex );
    void SetTick( int nIndex );

    void DumpAdjustChar( std::wostream& stm ) const;
    void DumpPollData( std::wostream& stm ) const;

protected:

    mutable KncCriticalSection  m_csPollData;
    time_t      m_tmBegin;
    time_t      m_tmEnd;

    DWORD       m_adwLastTic[PTI_MAX];
    DWORD       m_adwTickGap[PTI_MAX];

    GCITEMID       m_dwPollItemID;
    bool        m_bActive;

    std::map<char,int>      m_mapPollData;
    std::set<char>          m_setAdjustChar;

};

DefSingletonInline( KCharacterPollManager );
DeclOstmOperatorA( KCharacterPollManager );