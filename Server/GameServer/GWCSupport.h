#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include "KncTimer.h"
#include "CommonPacket.h"

class KGWCSupport
{
    DeclareSingleton( KGWCSupport );
    NiDeclareRootRTTI( KGWCSupport );
    DeclToStringW;
public:
    KGWCSupport(void);
    ~KGWCSupport(void);

    void Tick();
    bool IsEventTerm();
    void UpdateConfigData( IN const KGWCSupportConfigData& kConfigData );
    void UpdateGWCData( IN std::vector<KGWCSupportData>& vecGWCData );
    GCITEMID GetEventItem();
    int GetNationCode();
    void GetGWCData( OUT std::vector<KGWCSupportData>& vecGWCData );
    void IncreaseNationAccCount( IN const int nCount );
    int GetRewardCount() const { KLocker lock(m_csGWCData); return m_kConfigData.m_nDonationCount; }
    void GetRewardItems( std::vector<KDropItemInfo>& vecRewards );

    bool AddUserData( IN const DWORD dwUID, IN const int nAccCount );
    void RemoveUserData( IN const DWORD dwUID );
    int GetUserAccCount( IN const DWORD dwUID );
    bool IncreaseUserAccCount( IN const DWORD dwUID, IN const int nDiffCount );
    bool SetUserAccCount( IN const DWORD dwUID, IN const int nAccCount );
    bool IsInUserData( IN const DWORD dwUID );
    DWORD GetUserCount() const { KLocker lock(m_csGWCUserData); return m_mapDonationData.size(); }

private:
    void UpdateEventTerm();

private:
    mutable KncCriticalSection      m_csGWCData;
    bool                            m_bEventTerm;
    KncTimer                        m_kTimer;
    KGWCSupportConfigData           m_kConfigData;
    std::vector<KGWCSupportData>    m_vecGWCData;

    mutable KncCriticalSection      m_csGWCUserData;
    std::map<DWORD,int>             m_mapDonationData;
};
DefSingletonInline( KGWCSupport );
DeclOstmOperatorA( KGWCSupport );