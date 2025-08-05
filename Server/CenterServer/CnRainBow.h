#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>


struct KRainBowData; // CommonPacket.h
struct KECN_RAINBOW_UPDATE_NOT; // CenterPacket.h

struct KDailyEventData
{
    int     m_nDailyID;
    time_t  m_tmEventDate;
    int     m_nEventID;
    float   m_fPrimaryRatio;
    float   m_fSecondaryRatio;
    KDailyEventData()
    :m_nDailyID(-1),m_tmEventDate(1),m_nEventID(-1),m_fPrimaryRatio(0.f),m_fSecondaryRatio(0.f)
    {}
};

struct KJackpotTimeList
{
    int     m_nDailyID;
    int     m_nJackpotID;
    time_t  m_tmBegin;
    time_t  m_tmEnd;
    KJackpotTimeList():m_nDailyID(-1),m_nJackpotID(-1),m_tmBegin(1),m_tmEnd(1){}
};

class KCnRainBow
{
    DeclareSingleton( KCnRainBow );
    NiDeclareRootRTTI( KCnRainBow );
    DeclToStringW;

public:
    KCnRainBow(void);
    ~KCnRainBow(void);

    void DBRainbowEventUpdate( IN const int nRainbowID, IN const time_t tmBegin, IN const time_t tmEnd );
    void DBDailyEventUpdate( IN std::vector<KDailyEventData>& vecDailyEventList );
    void DBJacPotEventListUpdate( IN std::vector<KRainBowData>& vecJackPotEventList );
    void DBJacPotTimeListUpdate( IN std::vector<KJackpotTimeList>& vecJackPotTimeList );
    void UpdateState();
    // 업데이트가 붙은 함수가 데이터 변경을 가한다.
    bool IsChanged();
    void SetChangeTag( IN const bool bChanged );
    bool IsEventTerm() { KLocker lock(m_csRainbow); return m_bEventTerm; }

    void Tick();
    void GetRainbowPacket( OUT KECN_RAINBOW_UPDATE_NOT& kPacket );
    int GetRainbowID();
    int GetDailyEventID();
    int GetJackPotID();
    void GetDailyEventForServer( OUT std::vector<KRainBowData>& vecEventList );
    void ReloadAll();

protected: // public 함수가 아니면 락을 걸지 않겠음.
    bool CheckEventTerm();
    int CheckJacPotTerm();
    bool GetDailyEvent( KDailyEventData& kCurrentDailyEvent );
    void QueuingToDB( IN const bool bFirst );
    void SendToCenter();

    void DumpCurrentEvent( std::wostream& stm ) const ;

protected:

    mutable KncCriticalSection      m_csRainbow;
    int                     m_nRainbowID;
    time_t                  m_tmEventBegin; // 전체 이벤트 시간
    time_t                  m_tmEventEnd;

    std::vector<KDailyEventData>    m_vecDailyEventList;
    std::vector<KRainBowData>       m_vecJackPotEventList;
    std::vector<KJackpotTimeList>   m_vecJacPotTimeList;

    // 현재 상태

    bool    m_bChanged; // 변경 사항이 있었는가
    bool    m_bEventTerm;  // 레인 보우 이벤트 기간 인가.
    int     m_nJackPotID; // 잭팟 시간인가.

    KDailyEventData     m_kCurrentDailyEvent;

    //Tick 관련 데이터.
    DWORD   m_dwLastTick;
    DWORD   m_dwTickGap;

};

DefSingletonInline( KCnRainBow );
DeclOstmOperatorA( KCnRainBow );