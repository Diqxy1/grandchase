#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include "Lua/KLuaManager.h"
#include "Kairo.h"

struct KObjectInfo;
struct KStageInfo;

class KRitasChristmas
{
    DeclareSingleton( KRitasChristmas );
    NiDeclareRootRTTI( KRitasChristmas );
    DeclToStringW;

public:
    KRitasChristmas( void );
    ~KRitasChristmas( void );

    struct KRewardInfo
    {
        GCITEMID    m_dwItemID;
        int         m_nCount;
        int         m_nPeriod;
        float       m_fProb;
    };

public:
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsEventTerm();
    bool IsRegisteredRewardInfo();
    GCITEMID GetPremiumKeyItemID();
    bool GetRewardKairo( IN int nOpenBoxType_, OUT Kairo& kReward_ );
    bool GetPlayInfo( OUT KRitasChristmasPlayInfo& kPlayInfo_ );
    bool GetStageInfo( IN int nStageNum_, OUT KRitasChristmasStageInfo& kStageInfo_ );
    void GetAllStageInfo( OUT std::map< int, KRitasChristmasStageInfo >& mapStageInfo_ );
    bool GetFirstStageNum( OUT int& nFirstStageNum_ );
    bool GetNextStageNum( IN int nCurrentStageNum_, OUT int& nNextStageNum_ );
    bool GetContinueCoinConsumeCount( IN int nStageNum_, OUT int& nContinueCoinConsumeCount_ );
    bool GetObjectInfo( OUT std::map< int, KRitasChristmasObjectInfo >& mapObjectInfo_ );
    int GetPremiumPerFeverObjectCount();
    int GetBasketEmptyPointLimit();
    int GetFeverPointLimit();
    int GetVersion();
    GCITEMID GetContinueCoinExchangeItemID();
    int GetContinueCoinExchangeConsumeCount();
    int GetContinueCoinExchangeResultCount();
    int GetCoinSupplyInterval();
    int GetCoinSupplyCount();
    int GetCoinMaxQuantity();
    GCITEMID GetCoinDoubleGainItemID();
    int GetCoinPerGame();
    void Tick();
    void SendRankUpdateTimeChangedNot( IN bool bReceivedLastRankInfo_ );
    bool GetRewardViewListInfo( OUT std::vector< GCITEMID >& vecRewardViewListInfo_ );

private:
    bool LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ );
    bool LoadUpdateTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ );
    bool LoadPlayInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT KRitasChristmasPlayInfo& kPlayInfo_ );
    bool LoadObjectInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, KRitasChristmasObjectInfo >& mapObjectInfo_ );
    bool LoadStageInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, KRitasChristmasStageInfo >& mapStageInfo_ );
    bool LoadRewardInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KRewardInfo >& vecRewardInfo_ );
    bool LoadRewardViewListInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, IN std::vector< GCITEMID >& vecRewardViewListInfo_ );
    bool SetRewardInfos();
    bool CheckOneMinTick();
    bool CheckRankUpdateTime( IN time_t& tmDate_ );

protected:
    mutable KncCriticalSection                  m_csRitasChristmas;

    int                                         m_nVersion;                         // 이벤트 버전
    time_t                                      m_tmBegin;                          // 이벤트 시작 시간(Y, M, D, H)
    time_t                                      m_tmEnd;                            // 이벤트 종료 시간(Y, M, D, H)
    time_t                                      m_tmRankUpdateTime;                 // 랭킹 데이터 새로 가져오는 시간(H, m). 다음 업데이트 시간

    DWORD                                       m_dwLastTick;
    DWORD                                       m_dwTickGap;

    int                                         m_nCoinPerGame;                     // 게임 시작시 소모되는 시작 코인 수
    int                                         m_nCoinSupplyInterval;              // 시작 코인이 충전되는 시간 간격(ms)
    int                                         m_nCoinSupplyCount;                 // 한번에 충전되는 시작 코인 수
    int                                         m_nCoinMaxQuantity;                 // 최대 시작 코인 충전 수
    GCITEMID                                    m_dwCoinDoubleGainItemID;           // 일명 코인 부스터. 갖고 있으면 코인이 두배로 충전되는 아이템ID
    GCITEMID                                    m_dwContinueCoinExchangeItemID;     // 재도전 코인 충전 아이템ID(수량)
    int                                         m_nContinueCoinExchangeConsumeCount;// 충전시 소모되는 재도전 코인 아이템 수
    int                                         m_nContinueCoinExchangeResultCount; // 충전 아이템 하나당 충전되는 재도전 코인 수

    KRitasChristmasPlayInfo                     m_kPlayInfo;
    std::map< int, KRitasChristmasObjectInfo >  m_mapObjectInfo;                    // 떨어지는 오브젝트 정보. 피버 오브젝트는 하나만 존재하도록 구현되어 있다
    int                                         m_nBasketEmptyPointLimit;           // 바구니 비우기 한번에 얻을 수 있는 최대 점수(해킹체크용)
    int                                         m_nFeverPointLimit;                 // 피버오브젝트로 스테이지당 얻을 수 있는 최대 점수(해킹체크용)
    std::map< int, KRitasChristmasStageInfo >   m_mapStageInfo;
    std::vector< KRewardInfo >                  m_vecSpecialRewardInfo;
    std::vector< KRewardInfo >                  m_vecPremiumRewardInfo;
    std::vector< GCITEMID >                     m_vecRewardViewListInfo;

    Kairo                                       m_kKairoSpecialReward;              // 스페셜 상자 구성품

    GCITEMID                                    m_dwPremiumKeyItemID;               // 프리미엄 상자를 열 때 필요한 열쇠 아이템
    int                                         m_nPremiumPerFeverObjectCount;      // 프리미엄 상자 1개를 얻는데 필요한 피버타임 오브젝트 수
    Kairo                                       m_kKairoPremiumReward;              // 프리미엄 상자 구성품
};

DefSingletonInline( KRitasChristmas );
DeclOstmOperatorA( KRitasChristmas );

class KRitasChristmasUserData
{
public:
    KRitasChristmasUserData();
    ~KRitasChristmasUserData();
    void GetRitasChristmasEventUserData( OUT KRitasChristmasEventUserData& kRitasChristmasEventUserData_ );

public:
    int                                         m_nCurrentStageNum;             // 현재 플레이 중인 스테이지 번호
    int                                         m_nCurrentPoint;                // 현재 게임에서 얻은 총 점수(처음부터 시작하면 초기화 됨)
    int                                         m_nInitialStartCoinCount;       // 시작 코인 수(접속시 DB에서 가져온 초기값)
    int                                         m_nStartCoinCount;              // 시작 코인 수. DB에 저장됨
    time_t                                      m_tmStartCoinReceived;          // 마지막으로 시작 코인을 받은 시간. DB에 저장됨
    int                                         m_nContinueCoinCount;           // 재도전 코인 수(충전, 사용시 DB에 바로 기록함)
    bool                                        m_bContinuable;                 // 재도전 가능한지 여부
    int                                         m_nSpecialRewardCount;          // 프리미엄 상자 수. 게임 종료후에만 사용할 수 있으며 첫 스테이지 부터 게임 시작시 0으로 초기화됨
    int                                         m_nInitialPremiumRewardCount;   // 스페셜 상자 수(접속시 DB에서 가져온 초기값)
    int                                         m_nPremiumRewardCount;          // 스페셜 상자 수. DB에 저장됨
    int                                         m_nMaxPoint;                    // 최고 점수. DB에 저장됨(최고 점수 갱신시 DB에 바로 갱신됨)
    std::vector< KRitasChristmasRankInfo >      m_vecRankInfo;                  // 친구들의 어제 랭킹정보(10위까지, 자신이 포함될 수 있음)
    bool                                        m_bReceivedLastRankInfo;        // 마지막 랭킹 정보를 받아왔는지 여부(true: 이미 가져옴, false: 안가져왔음. DB요청 필요)
};
