#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>

class KLuaManager;

class KCnBuffDonation
{
    DeclareSingleton( KCnBuffDonation );
    NiDeclareRootRTTI( KCnBuffDonation );
    DeclToStringW;
public:
    KCnBuffDonation(void);
    ~KCnBuffDonation(void);

    bool LoadScript();
    bool LoadSyncPointListInfo( IN KLuaManager& kLuaMng, std::string& strTable, OUT std::vector< PAIR_INT_BOOL >& vecSyncPointList );
    void Tick();
    void GetBeginTime( OUT time_t& tmDate );
    void GetDonationTotalData( OUT KECN_BUFF_DONATION_TOTAL_DATA_NOT& kPacket );
    void UpdateDonationPointFromDB( IN const __int64& biTotalPoint );
    void UpdateDonationPointDiff( IN const int& pointDiff );
    void BuffStateUpdateProcess( IN const bool& bEndBuffTime, OUT int& nFlagType );
    void SendingEventToDBThread( IN const int& nFlagType_ );
    void SendingDataToAllGameServer( IN const KECN_BUFF_DONATION_TOTAL_DATA_NOT& kPacket );
    void UpdateDonationPointByBuffActivation( IN __int64& biDBPoint, OUT __int64& biComputedPoint );
    void GetType( OUT DWORD& dwType ) { KLocker lock( m_csBuffDonation ); dwType = m_dwType; };

    const __int64 GetActiveBuffPoint() { KLocker lock( m_csBuffDonation ); return m_biActiveBuffPoint; };
    const bool IsBuffActive() { return m_bBuffActive; };
    const bool IsBuffActivationEnable();
    const __int64 GetDecreasePoint();
    const bool IsSyncPointToAllUser();
    void ResetSecondDataAtSyncPointVector();
private:
    bool LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ );

private:
    mutable KncCriticalSection   m_csBuffDonation;
    time_t                       m_tmBegin;                 // 기부 시작 날짜
    time_t                       m_tmEnd;                   // 기부 종료 날짜

    __int64                      m_biActiveBuffPoint;       // 버프 걸어주는 기준치
    DWORD                        m_dwBuffRetainTime;        // 버프 발동 유지 시간
    int                          m_nDecreaseCount;          // 기부 수치가 감소 되는 수치
    DWORD                        m_dwType;                  // 데이터 타입 (SDGA_server_data_select_integer 에서 사용)
    std::vector< PAIR_INT_BOOL > m_vecSyncPointList;        // 클라이언트 전체에 동기화 패킷 보낼 시점(점수별로 판단)

    __int64                      m_biTotalDonationPoint;    // 전체 서버 기부 수치
    DWORD                        m_dwLastTick;
    bool                         m_bBuffActive;

    KncTimer                     m_kUpdateCheckTimer;       // 버프 발동 상태 체크 타이머(1분)
};

DefSingletonInline( KCnBuffDonation );
DeclOstmOperatorA( KCnBuffDonation );