#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include "KncTimer.h"
#include "CommonPacket.h"


class KLuaManager;
class KCnGWCSupport
{
    DeclareSingleton( KCnGWCSupport );
    NiDeclareRootRTTI( KCnGWCSupport );
    DeclToStringW;
public:
    KCnGWCSupport(void);
    ~KCnGWCSupport(void);
    bool LoadScript();
    void Tick();
    void UpdateDataFromDB( IN std::vector<KGWCSupportData>& vecGWCData );
    void SendConfigData();
    void SendGWCData();
    void GetConfigData( OUT KGWCSupportConfigData& kConfig );
    void GetGWCData( OUT std::vector<KGWCSupportData>& vecGWCData );

private:
    bool IsConfigChanged();
    void SetConfigChanged( IN const bool bChange );
    bool IsDataChanged();
    void SetDataChanged( IN const bool bChange );
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    void QueueingToDB();
    bool IsEventTerm();
    void IntervalTick();
    bool LoadRewardList( IN KLuaManager& luaMng, OUT std::vector< KDropItemInfo >& vecRewardList );

private:
    mutable KncCriticalSection      m_csGWCData;
    KncTimer                        m_kTimer;
    bool                            m_bConfigChanged;
    bool                            m_bDataChanged;
    KGWCSupportConfigData           m_kConfigData;
    std::vector<KGWCSupportData>    m_vecGWCData;

};
DefSingletonInline( KCnGWCSupport );
DeclOstmOperatorA( KCnGWCSupport );