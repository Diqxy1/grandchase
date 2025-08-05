#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>

struct KDotantionRank;
struct KItemDonationInfo; // CommonPacket.h

class KLuaManager;
class KCnDonationManager
{
    DeclareSingleton( KCnDonationManager );
    NiDeclareRootRTTI( KCnDonationManager );
    DeclToStringW;

public:
    KCnDonationManager(void);
    ~KCnDonationManager(void);
    bool LoadScript();
    void InitProcess();
    void Tick();

    __int64 GetTotalDonation();
    void UpdateDeltaDonation( IN __int64 biDeletaDonation );
    void SetDonationFromDB( IN __int64 biDeletaDonation );
    void UpdateDonationRankDB( IN std::vector<KDotantionRank>& vecRank );
    void UpdateDonationRankServer( IN std::vector<KDotantionRank>& vecRank );
    void GetDonationRank( OUT std::vector<KDotantionRank>& vecRank );
    void PrintRank();
    // 090924 tgkwon 아이템(송편) 기부 관련 추가 함수.
    void UpdateItemDonationInfo( IN std::vector<KItemDonationInfo>& vecDoInfo );
    void GetTotalGradeInfo( OUT KECN_ITEM_DONATION_UPDATE_NOT& kPacket ) const;

protected:
    bool ChkUpdateFromDB();
    bool LoadUpdateTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool IsRun()const; // 진행중인지.
    void SortAndNumbering( IN OUT std::vector<KDotantionRank>& vecRank );
    void AddRankNoSort( IN KDotantionRank& kRank, int LastPoint );

    // 090924 tgkwon 아이템(송편) 기부 관련 추가 함수.
    void SetCurrentDoInfo();                                                // 진행중인 등급 세팅
    void GetCurrentGradeInfo( OUT KItemDonationInfo& kCurrentGradeInfo );   // 진행중인 등급 가져오기.
    void SendToDonationInfo();                                              // Center -> GameServer 갱신된 기부 정보 전송.
    void QueuingToDB( IN const bool bFirst );                               // Tick을 돌면서 DB 갱신되었는지 감시.

protected:
    mutable KncCriticalSection  m_csDonation;
    __int64                     m_biTotalDonation;
    std::vector<KDotantionRank> m_vecDonationRank;

    time_t      m_tmBegin;              // 기부 시작 날짜
    time_t      m_tmEnd;                // 기부 종료 날짜
    time_t      m_tmDBUpdateTime;       // 다음 DB 업데이트 시간 지정
    DWORD       m_dwLastTick;
    DWORD       m_dwTickGap;

    DWORD       m_dwMaxRankSize;

    // 090924 tgkwon 아이템(송편) 기부 관련 추가
    std::vector<KItemDonationInfo>      m_vecTotalInfo;     // 전체 기부 Grade 정보.
    KItemDonationInfo                   m_kCurrentGrade;    // 현재 달성해야 하는 Grade
    bool                                m_bDBUpdateCheck;   // DB에 Update Check.
    bool                                m_bMaxGradeCheck;   // Max Grade 체크.
    DWORD                               m_dwMaxGrade;       // Max Grade값.

};
DefSingletonInline( KCnDonationManager );
DeclOstmOperatorA( KCnDonationManager );