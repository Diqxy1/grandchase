#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>

class KLuaManager;
class KCnNewDonationManager
{
    DeclareSingleton( KCnNewDonationManager );
    NiDeclareRootRTTI( KCnNewDonationManager );
    DeclToStringW;

public:
    KCnNewDonationManager(void);
    ~KCnNewDonationManager(void);
    bool LoadScript();
    void InitProcess();
    void Tick();

    void GetTotalDonation( OUT std::map< DWORD, int >& mapTotalDonation );
    void UpdateDeltaDonation( IN std::map<DWORD, int> mapDonationInfo );
    void SetDonationFromDB( IN std::map< DWORD, int> mapTotalDonation );
    void GetMinMaxType( OUT DWORD& dwMinType, OUT DWORD& dwMaxType );
    void GetBeginTime( OUT time_t& tmDate );        // 구분 날짜를 이벤트 시작날짜로 한다.

protected:
    bool ChkUpdateFromDB();
    bool LoadUpdateTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool IsRun() const; // 진행중인지.
    void DumpCurrentDonationState( std::wostream& stm ) const ;

protected:
    mutable KncCriticalSection  m_csNewCnDonation;
    std::map< DWORD, int >      m_mapTotalDonation;     // Type별 총액.

    time_t                      m_tmBegin;
    time_t                      m_tmEnd;
    time_t                      m_tmDBUpdateTime;
    DWORD                       m_dwLastTick;
    DWORD                       m_dwTickGap;
    DWORD                       m_dwMinTypeNum;
    DWORD                       m_dwMaxTypeNum;
};
DefSingletonInline( KCnNewDonationManager );
DeclOstmOperatorA( KCnNewDonationManager );