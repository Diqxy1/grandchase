#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>

class KLuaManager;
struct KDropItemInfo;
class KLunaNewYear
{
    DeclareSingleton( KLunaNewYear );
    NiDeclareRootRTTI( KLunaNewYear );
    DeclToStringW;

public:
    KLunaNewYear(void);
    ~KLunaNewYear(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );

    void Tick();
    bool IsActive() { return m_bActive; }
    bool GetReward( OUT int& nGPReward, OUT std::vector<KDropItemInfo>& vecRewardItems );


protected:
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadRewardItem( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector<KDropItemInfo>& vecRewardItems );
    void UpdateState();
    DWORD GetLastTick() { KLocker lock(m_csLuna); return m_dwLastTick; }
    DWORD GetTickGap() { KLocker lock(m_csLuna); return m_dwTickGap; }
    void SetLastTick() { KLocker lock(m_csLuna); m_dwLastTick = ::GetTickCount(); }
    void DumpRewardItem( std::wostream& stm, const std::vector<KDropItemInfo>& kReward, wchar_t* szName ) const;



protected:
    mutable KncCriticalSection  m_csLuna;

    time_t  m_tmBegin;
    time_t  m_tmEnd;
    bool    m_bActive;
    DWORD   m_dwTickGap;
    DWORD   m_dwLastTick;
    int     m_nRewardGP;
    std::vector< KDropItemInfo > m_vecRewardItems;
};

DefSingletonInline( KLunaNewYear );
DeclOstmOperatorA( KLunaNewYear );