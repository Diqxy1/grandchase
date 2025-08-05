#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>

class KLuaManager;
class KNewDonationManager
{
    DeclareSingleton( KNewDonationManager );
    NiDeclareRootRTTI( KNewDonationManager );
    DeclToStringW;

public:
    KNewDonationManager(void);
    ~KNewDonationManager(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void InitProcess();
    void Tick();
    bool IsRun() const; // 진행중인지 체크.

    void UpdateTotalDonationFromCenter( IN std::map<DWORD, int> mapTotalDonation );
    bool SendDeltaDonationToCenter();

    void GetTypeTotalDonation( IN DWORD dwType, OUT int& nTotalDonation );  // Type의 기부액 가져오기.
    void AddDeltaDonationPoint( IN DWORD dwType, IN int nDeltaPoint );      // 유저의 기부액 더하기.
    void GetNPCInfo( OUT std::map<DWORD, std::vector<std::pair<DWORD, int> > >& mapNPCInfo );
    void GetMinMaxType( OUT DWORD& dwMinType, OUT DWORD& dwMaxType );
    bool CheckDonationItemID( IN GCITEMID dwItemID );
    void GetBeginTime( OUT time_t& tmDate ); // 구분 날짜를 이벤트 시작날짜로 한다.
    void GetDonationItemID( OUT GCITEMID& dwItemID_ );

protected:
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadNPCInfo( IN KLuaManager& kLuaMng, OUT std::map<DWORD, std::vector<std::pair<DWORD, int> > >& mapNPCInfo );
    bool LoadItems( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector<std::pair<DWORD,int> >& vecInfos );
    void DumpCurrentDonationState( std::wostream& stm ) const ;

protected:
    mutable KncCriticalSection  m_csNewDonation;
    std::map<DWORD, int> m_mapTotalDonation; // NPC Type별 기부변수.
    std::map<DWORD, int> m_mapDeltaDonation; // 차이값 저장 기부 변수.
    std::map<DWORD, std::vector<std::pair<DWORD, int> > > m_mapNPCInfo; // NPC별 기부 목표치.

    time_t  m_tmBegin;              // 기부 시작 날짜
    time_t  m_tmEnd;                // 기부 종료 날짜
    GCITEMID   m_dwDonationItemId;     // 기부 ItemID
    DWORD   m_dwTickGap;            // TickGap
    DWORD   m_dwLastTick;           // LastTick
    DWORD   m_dwMinType;            // NPC Min Num
    DWORD   m_dwMaxType;            // NPC Max Num
};
DefSingletonInline( KNewDonationManager );
DeclOstmOperatorA( KNewDonationManager );