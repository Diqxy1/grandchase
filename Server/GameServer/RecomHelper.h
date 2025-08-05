#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include <map>
#include "CommonPacket.h"
#include "KncTimer.h"

class KLuaManager;
struct KRecommenderInfo;
struct KItemUnit;
class KRecomHelper
{
    DeclareSingleton( KRecomHelper );
    NiDeclareRootRTTI( KRecomHelper );
    DeclToStringW;
public:
    
    typedef std::vector<KDropItemInfo> VEC_REWARD;
    enum USER_TYPE
    {
        UT_NEW,
        UT_OLD,
        UT_RECOMMENDEE,
        UT_MAX,
    };

    enum RECOMMEND_USER_OPTION
    {
        RUO_RECOMMEND   = 0,  // 추천함
        RUO_REFUSE      = 1,  // 추천 하지 않음
        RUO_DEFER       = 2,  // 나중에 추천함
    };

    enum RECOMMEND_B_STATE
    {
        RBS_NONE            = 1,
        RBS_NOT_RECOM       = 2,
        RBS_WAIT_RECOM      = 3,
        RBS_DENY_RECOM      = 4,
        RBS_ACCEPT_RECOM    = 5,
        RBS_ATTEND          = 6,
    };

    enum RECOMMEND_AB_STATE
    {
        RABS_WAIT       = 1,
        RABS_DENY       = 2,
        RABS_ACCEPT     = 3,
        RABS_ATTEND     = 4,
        RABS_REWARDED   = 5,
    };

    KRecomHelper(void);
    ~KRecomHelper(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsReturnUser( time_t tmLastLogin );
    bool IsEventTerm() { KLocker lock( m_csRecommend ); return m_bEventTerm;};
    bool IsEventTerm( IN const time_t tmDate );
    bool IsLimitEventTerm() { KLocker lock( m_csRecommend ); return m_bLimitEventTerm;};
    bool IsLimitEventTerm( IN const time_t tmDate );
    bool IsAdjustTerm( IN const KRecommenderInfo& kMyData );
    void Tick();
    bool IsNewBeginTerm( IN const time_t tmDate );
    bool IsRecomLevel( IN const int nOldLv, IN const int nCurrentLv ); // 복귀유저 추천한 유저 레벨.

    void GetRecommenders( IN const std::vector<KRecommenderInfo>& vecAll, IN const int nState, OUT std::vector<DWORD>& vecWaiting );
    DWORD GetRecommendersCount( IN const std::vector<KRecommenderInfo>& vecAll, IN const int nState );
    bool GetFirstAttenReward( IN const int nUserType, OUT std::vector<KDropItemInfo>& vecReward );
    bool GetDailyAttenReward( IN const int nUserType, OUT std::vector<KDropItemInfo>& vecReward );
    bool CheckRecomBonus( IN const int nBonusDay );
    bool GetRecomBonusReward( IN const int nBonusDay, OUT std::vector<KDropItemInfo>& vecReward );
    void GetRecommendeeReward( OUT std::map< int, VEC_REWARD >& mapRewardList );
    void GetRecommendLevelUpReward( IN const std::pair<int,int> prRecomInfo, OUT VEC_REWARD& vecRewardList );

    DWORD GetMaxRecommReqNum();
    DWORD GetMaxAcceptNum();

    int GetEventID();
    float GetPartyPlayExpBonus();
    float GetPartyPlayGpBonus();
    int GetAdjustPeriod() { KLocker lock( m_csRecommend ); return m_nAdjustPeriod; }
    int GetRecomBonusGap() { KLocker lock( m_csRecommend ); return m_nRecomBonusGap; }
    int AddLevelUpReward( IN const int nOldLv, IN const int nCurrentLv, IN const int nRewardLv, IN OUT std::vector<KDropItemInfo>& vecItem );

    std::wstring GetMsg();
    int GetVersion() { KLocker lock( m_csRecommend ); return m_nVersion; }

protected:
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadRewardList( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT VEC_REWARD& vecReward );
    bool LoadRewardList( IN KLuaManager& kLuaMng, IN const int nIndex, OUT VEC_REWARD& vecReward );
    bool LoadRewardList( IN KLuaManager& kLuaMng, OUT VEC_REWARD& vecReward );
    bool LoadLevelUpReward(IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT std::map<int,VEC_REWARD>& mapReward );

    bool IsUpdateTime();
    void UpdateEventState();
    void DumpRewardList( IN OUT std::wostream& stm, IN const std::wstring& strName, IN const std::map< int, VEC_REWARD >& mapReward ) const;

protected:
    mutable KncCriticalSection  m_csRecommend;

    bool    m_bEventTerm;
    bool    m_bLimitEventTerm;
    KncTimer m_kUpdateTimer;
    time_t  m_tmBegin;
    time_t  m_tmEnd;
    time_t  m_tmReturn;
    time_t  m_tmLimitDate; // 마지막 날보다 하루 많다.ㅎㅎ.
    time_t  m_tmNewBeginDate; // 신규유저 체크 날짜.

    int     m_nAdjustPeriod;
    int     m_nEventID;
    float   m_fExpRatio;
    float   m_fGpRatio;
    int     m_nRecomBonusGap;

    DWORD   m_dwMaxRecommReqNum; // 최대 리스트 숫자..
    DWORD   m_dwMaxAcceptNum;    // 최대 승인 숫자.

    std::map< int, VEC_REWARD > m_mapFirstAttendance;
    std::map< int, VEC_REWARD > m_mapDailyAttendance;
    std::map< int, VEC_REWARD > m_mapRecommendeeReward; // Key 가 출석자 수..3,6,9,12
    std::map< int, VEC_REWARD > m_mapLevelUpReward; // 추천한 유저가 Levelup했을 경우, 추천한 유저 아이템 지급.
    std::map< int, VEC_REWARD> m_mapRecommendLevelUpReward; // 추천한 유저가 Levelup했을 경우, 추천받은 유저 아이템 지급.

    std::wstring m_strConnectMsg; // 접속시 메세지

    int m_nVersion;
};

DefSingletonInline( KRecomHelper );
DeclOstmOperatorA( KRecomHelper );