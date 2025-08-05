#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>

/*
생각..
. 토탈 금액은 센터 서버를 통해서 받아 온다.
  - 게임 서버에서 추가된 금액을 Center로 보내고, total 금액을 센터에서 받아 온다.
. 랭킹은 게임 서버에서 바로 DB에서 받아 온다.
  - 랭킹은 하루 한번 지정된 시간에 받아 온다.
. 기간은 시작, 끝, DP 시간을 가지고 특정 시간 마다 한번식 상태 갱신을 한다.
*/

struct KECN_ITEM_DONATION_UPDATE_NOT;   // CenterPacket.h
struct KDotantionRank;
struct KItemUnit;
struct KItemDonationInfo;               // CommonPacket.h
struct KDropItemInfo;
class KLuaManager;

struct sDonationReward
{
    int                             m_nDonationPoint;
    std::vector<KDropItemInfo >     m_vecReward;
};

class KDonationManager
{
    DeclareSingleton( KDonationManager );
    NiDeclareRootRTTI( KDonationManager );
    DeclToStringW;
public:

    enum DONATION_STATE
    {
        DS_NONE     = 0,
        DS_DONATION = 1,
        DS_DISPLAY  = 2,

        DS_MAX,
    };

    enum DONATION_TICK
    {
        DT_TICK = 0,
        DT_SYNC = 1,
        DT_MAX,
    };

public:
    KDonationManager(void);
    ~KDonationManager(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void InitProcess();
    void Tick();

    void UpdateTotalDonationFromCenter( __int64 biTotalDonation );
    bool SendDeltaDonationToCenter();

    int GetDonationState();
    void GetDonationReword( int nCurrentPoint, int nDeltaPoint, OUT std::vector<KDropItemInfo>& vecItem );
    __int64 GetTotalDonation();
    void UpdateDonationRankFromCenter( IN std::vector<KDotantionRank>& vecRank );
    void GetDonationRank( OUT std::vector<KDotantionRank>& vecRank );
    int GetMinDonationPoint();
    void UpdateDonationRankPerUser( IN int nAccDonationPoint, IN std::wstring strNick );
    void PrintRank();
    void AddDeltaDonationPoint( int nDeltaPoint );
    void Clear();
    DWORD GetDonationUnit();

    // 090924 tgkwon 아이템(송편) 기부 관련 추가 함수.
    void UpdateFromCenter( IN KECN_ITEM_DONATION_UPDATE_NOT& kPacket );
    void GetGradeInfo( IN const DWORD& dwGrade, OUT KItemDonationInfo& kGradeInfo );
    void GetTotalGradeInfo( OUT std::vector<KItemDonationInfo>& vecDoInfo );
    void NotifyFromCenter( IN const DWORD& dwGrade );

protected:
    DWORD GetTickGap( int nIndex );
    DWORD GetTick( int nIndex );
    void SetTick( int nIndex );

    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadReward( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT sDonationReward& rewardInfo );
    void UpdateState();
    void SortAndNumbering( IN OUT std::vector<KDotantionRank>& vecRank );
    int GetLastAccDonationPoint();

    void DumpDonationReward( std::wostream& stm, const sDonationReward& kReward, wchar_t* szName ) const;
    void AddReward( IN const std::vector<KDropItemInfo>& vecReward, IN OUT std::vector<KDropItemInfo>& vecItem );

    // 090924 tgkwon 아이템(송편) 기부 관련 추가 함수.
    void SetCurrentDonation();
    void SetUpdateGrade( IN const DWORD& dwGrade );

protected:
    mutable KncCriticalSection  m_csDonation;
    __int64                     m_biTotalDonation;
    __int64                     m_biDeltaDonation;

    std::vector<KDotantionRank> m_vecDonationRank;

    time_t  m_tmBegin;              // 기부 시작 날짜
    time_t  m_tmEnd;                // 기부 종료 날짜
    time_t  m_tmDisplay;            // 기부는 중지되고 DP만 하는 기간.
    int     m_nCurrentDSState;      // 현재 상태.

    DWORD   m_dwaLastTick[DT_MAX];
    DWORD   m_dwaTickGap[DT_MAX];
    
    DWORD   m_dwDonationUnit;           // 0이면 GP, 0> 면 수량 아이템
    int     m_nMinDonationPoint;        // 최소 기부액.
    sDonationReward m_userReward;       // 개인 누적 기부액에 따른 보상.
    sDonationReward m_donationReward;   // 1회 기부액이 얼마 이상이면 주는것.

    DWORD   m_dwMaxRankSize;

    // 090924 tgkwon 아이템(송편) 기부 관련 추가.
    KItemDonationInfo              m_kCurrentDonation;        // 현재 기부중인 등급의 정보.
    std::vector<KItemDonationInfo> m_vecItemGradeInfo;        // 아이템 기부 전체 정보.
    bool                           m_bDonationMaxGradeCheck;  // 현재 등급이 5단계이면 공지 출력 체크해서 그만하기.
    DWORD                          m_dwMaxGrade;              // 최대 등급 저장.
};

DefSingletonInline( KDonationManager );
DeclOstmOperatorA( KDonationManager );