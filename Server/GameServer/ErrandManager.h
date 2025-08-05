#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include "Lua/KLuaManager.h"
#include "Kairo.h"
#include "UserPacket.h"

// 나는 심부름왕
// 1. 첫 접속 시 심부름 목록을 받지 않았으면 지급 하고, 아니라면 pass
//    (심부름 목록은 그냥 도전 포인트라고 생각하면 됨)
// 2. 미니게임 종료 후 10분을 기다리면 심부름 목록 지급
// 3. 스테이지 정보에 따라 사 와야 할 아이템의 종류와 아이템 수량을 랜덤으로 결정 해서 처리한다 [클라 처리]
// 4. 랜덤 확률로 메리가 출현하는데 여기에 걸리면 랜덤으로 현재 구매 한 아이템을 하나 잃어버린다 [클라 처리]
// 5. 구매 성공 시 칭찬 1회 증가와 스테이지가 넘어가고 실패했다면 해당 스테이지를 재 진행 한다 [클라 처리]
// 6. 칭찬 최대치(현재 9번)까지 성공하거나, 플레이 시간(현재 10분)이 다 지나가면 게임이 끝나게 되고,
// 7. 보상 요청 시 랜덤으로 아이템을 한번에 지급한다(중복 되는 아이템 획득 불가)
// 보상 지급은 AvoidOverlappedLottery 클래스에 RatioReward컨테이너를 전달 해서 획득 하는 방식
// 작업자 :
//    서버 - 손기운
//    클라 - 장원형
//    기획 - 유성철
//    Comment : 최종 보상은 게임 클리어 한번에 하나만 지급 하도록 기획쪽에서 확정
//              추가 작업으로 보상 부분에서 최종 보상 지급 제한 최대치 체크 함

class KErrandManager
{
    DeclareSingleton( KErrandManager );
    NiDeclareRootRTTI( KErrandManager );
    DeclToStringW;
public:
    KErrandManager(void);
    ~KErrandManager(void);

    enum EBUY_ITEM_TYPE
    {
        BREAD = 0,
        FLOWER,
        BOOK,
        EGG,
        FISH,
        TOFU,
        APPLE,
        CARROT,
        MEAT,
    };

public:
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    
    bool IsRun();

    bool GetRewardItem( IN int& nSuccessCount, IN OUT std::vector< PAIR_INT >& vecReceivedSpecialReward_, OUT KDB_EVENT_ERRAND_REWARD_REQ& vecReward );
    void GetClientInitInfo( OUT KEVENT_ERRAND_INIT_INFO_ACK& kPacket );
    const bool IsValidCount( IN int& nReqCount );
    void CheckErrandPlayableStateUpdate();
    int GetDelayTime() { KLocker lock( m_csErrand ); return m_nDelayTime; }; // reference 안쓴 것 의도한 것임
    void GetVersion( IN OUT int& nVersion_ ) { KLocker lock( m_csErrand ); nVersion_ = m_nVersion; };
    const bool CheckFinalSuccess( IN const int nSuccess );
    void ProvideNewItemForSpecialReward( IN const KDropItemInfo& kItemInfo_, IN OUT std::vector< PAIR_INT >& vecReceivedSpecialReward_, OUT KDB_EVENT_ERRAND_REWARD_REQ& kDBPacket_ );
    void ProvideCurrentItemAgainForSpecialReward( IN const KDropItemInfo& kItemInfo_, IN OUT std::vector< PAIR_INT >::iterator vitReceived_, OUT KDB_EVENT_ERRAND_REWARD_REQ& kDBPacket_ );
    bool Get_Errand_UserData_FromDB( IN const std::string& strScript_ );

private:
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadStageInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< int, KErrandStageInfo >& mapErrandStageInfo );
    bool LoadBuyList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector< KErrandBuyInfo >& vecBuyInfoList );
    bool LoadRewardInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector< KRatioRewardInfo >& vecRatioRewardInfo );
    bool LoadSpecialRewardInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector< std::pair< KDropItemInfo, int > >& vecSpecialRewardInfo );

protected:
    mutable KncCriticalSection m_csErrand;

    int                        m_nVersion;          // 이벤트 버전
    time_t                     m_tmBegin;           // 이벤트 시작 시간(Y, M, D, H)
    time_t                     m_tmEnd;             // 이벤트 종료 시간(Y, M, D, H)

    int                        m_nPlayTime;         // 한판 플레이 시간
    int                        m_nDelayTime;        // 보상 지급 완료 후 다음 플레이까지 기다려야 하는 시간
    int                        m_nTimeIncreaseRate; // 심부름 한 번 성공 시 증가 하는 시간 비율
    int                        m_nMaxStage;         // 최대 스테이지 숫자(최대 칭찬 횟수)
    int                        m_nMarryAppearInterval; // 메리 출현 주기
    int                        m_nMarryKeepAliveTime;  // 메리 출현 유지 시간

    std::map<int, KErrandStageInfo>  m_mapErrandStageInfo;
    std::vector< KRatioRewardInfo >  m_vecRatioRewardInfo;
    std::vector< std::pair< KDropItemInfo, int > > m_vecSpecialRewardInfo;
};

DefSingletonInline( KErrandManager );
DeclOstmOperatorA( KErrandManager );

class KErrandUserData
{
public:
    KErrandUserData();
    ~KErrandUserData();
public:
    bool CheckErrandPlayableStateUpdate();
    void ResetRemainTime();
    void ClearRemainTime() { m_nRemainTime = 0; };
    void GetUserDataForClient( OUT KEVENT_ERRAND_USER_INFO_ACK& kPacket );
    void GetUserDataForDB( OUT KErrandUserInfoDataStruct& kUserInfoData );
    void SetPlayable( IN const bool& bPlayable ) { m_bPlayable = bPlayable; };
    void GetPlayable( OUT bool& bPlayable ) { bPlayable = m_bPlayable; };
    void UpdateUserDateFromDB( IN const KErrandUserInfoDataStruct& kUserData );
    void IncreaseDataForLog( IN const int& nPlayCountDiff, IN const int& nSuccessCountDiff );
    void GetReceivedSpecialRewardInfo( OUT std::vector< PAIR_INT >& vecReceived ) { vecReceived = m_vecReceivedSpecialRewardInfo; };
    void SetReceivedSpecialRewardInfo( IN std::vector< PAIR_INT >& vecReceived ) { m_vecReceivedSpecialRewardInfo = vecReceived; };
    const bool GetRewardEnableState() { return m_bRewardEnableState; };
    void SetRewardEnableState( IN const bool& bRewardEnable ) { m_bRewardEnableState = bRewardEnable; };
private:
    int                        m_nRemainTime;        // 다음 플레이까지 남은 시간(종료 시 DB 기록)
    bool                       m_bPlayable;          // 현재 플레이 가능한 상태인지(종료 시 DB 기록)
    int                        m_nTotalPlayCount;    // 누적 플레이 횟수(DB기록 전용)
    int                        m_nTotalSuccessCount; // 누적 성공 횟수(DB기록 전용)
    std::vector< PAIR_INT >    m_vecReceivedSpecialRewardInfo; // 최종 보상 받은 정보
    bool                       m_bRewardEnableState;
};