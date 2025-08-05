#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <vector>
#include "CommonPacket.h"
#include "UserPacket.h"
#include "CenterPacket.h"

class KLuaManager;

struct KOlympicGameInfo
{
    enum EOlympicGameType {
        EOGT_TREEDROP = 1,
        EOGT_BALLOON = 2,
        EOGT_WATERBOMB = 3,
        EOGT_GAWIBAWIBO = 4,
        EOGT_DICEPLAY = 5,
        EOGT_BOSSGATE = 6,
    };

    int                         m_nGameType;     // GameType.
    int                         m_nDefaultCount; // 기본으로 지급할 횟수정보.
    int                         m_nRewardCount;  // 보상을 받을 목표 Count.
    int                         m_nOnePointMaxPoint; // 한게임(한판)에 획득할수 있는 최대 Point.
    int                         m_nTotalMaxPoint; // 유저가 획득할 수 있는 최대 Point.
    std::vector<KDropItemInfo>  m_vecRewardInfo; // 보상 아이템 정보.
};

struct KTimeIndexInfo
{
    int                         m_nTimeIndex;
    std::pair<DWORD,DWORD>      m_prStartTime;
    std::pair<DWORD,DWORD>      m_prEndTime;
};

class KOlympicManager
{
    DeclareSingleton( KOlympicManager );
    NiDeclareRootRTTI( KOlympicManager );
    DeclToStringW;

public:
    enum CountryCode
    {
        CC_NATION_KR = 1,
        CC_NATION_TW = 2,
        CC_NATION_BR = 3,
        CC_NATION_US = 4,
        CC_NATION_PH = 5,
        CC_NATION_TH = 6,
        CC_NATION_ID = 7,
        CC_NATION_LA = 8,
        AT_MAX,
    };
    typedef std::vector<KDropItemInfo> VEC_REWARD;

    KOlympicManager(void);
    ~KOlympicManager(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );

    bool IsRun();
    bool GetGradeRewardList( IN const DWORD dwGrade, OUT VEC_REWARD& vecReward );
    bool GetPointGrade( IN const DWORD dwBonusPoint, OUT DWORD& dwGrade );
    DWORD GetMaxGrade(){ KLocker lock( m_csOlympic ); return m_dwMaxGrade; };
    DWORD GetMaxPoint(){ KLocker lock( m_csOlympic ); return m_dwMaxPoint; };
    void GetOlympicScheduleInfo( OUT std::vector<KOlympicSchedule>& vecOlympicSchedule );
    void GetOlympicRegDateRank( OUT std::vector<KRegDateRankInfo>& vecRegDateRank );
    void GetOlympicTotalRank( OUT std::vector<KTotalRankInfo>& vecTotalRank );
    void GetOlympicGradeRewardList( OUT std::map<DWORD, std::vector<KDropItemInfo>>& mapGradeRewardList );
    void GetOlympicGradeDesc( OUT std::map<DWORD,std::pair<DWORD,DWORD>>& mapGradeDesc );
    bool GetGameInfo( IN const int& nGameType, OUT int& nDefaultCount, OUT int& nRewardCount, OUT int& nOnePointMaxPoint, OUT int& nTotalMaxPoint );
    bool GetOlympicGameInfo( IN const int& nGameType_, OUT KOlympicGameInfo& kOlympicGameInfo_ );
    void Tick();
    void UpdateOlympicRankData( IN std::vector<KRegDateRankInfo>& vecRegDateRank, IN std::vector<KTotalRankInfo>& vecTotalRank );
    void InitProcess();
    bool CheckGameResultData( IN const int& nGameType, IN const int& nAccumulateData, IN OUT int& nResultData );
    bool IsGamePlayEnable();
    void GetGameRewardInfo( IN const int& nGameType, OUT std::vector<KDropItemInfo>& vecRewardInfo );
    void GetCurrentOlympicGameInfo( OUT time_t& tmTodayDate_, OUT int& nGameType_, OUT int& nTeamType_ );
    bool CheckUserTimeIndex( IN const time_t& tmTodayDate, IN const int& nTimeIndex );
    bool DoOlympicGawibawibo( IN const int& nRemainTryPoint_, OUT int& nResult_ );
    void GetOlympicDiceNum( OUT int& nFirstDiceNum_, OUT int& nSecondDiceNum_ );
    void UpdateOlympicSoloPlayData( IN const std::wstring& strNickName, IN const time_t& tmDate, IN const int& nGameType, IN const int& nTeamType, IN const int nTotalPlayerData );
    void UpdateOlympicTeamPlayData( IN const time_t& tmDate, IN const int& nGameType, IN const int& nTeamType, IN const int& nIncreasedData );
    void UpdateOlympicSoloData( IN const time_t& tmDate, IN const int& nGameType, IN const int& nTeamType, IN const int& nTopPlayerData, IN const std::wstring& strNickName );
    void UpdateOlympicTeamData( IN const time_t& tmDate, IN const int& nGameType, IN const int& nTeamType, IN const __int64& biTotalData );
    void UpdateOlympicEventData( IN KECN_OLYMPIC_EVENT_DATA_NOT& kPacket );
    int GetCurrentTimeIndex(){ KLocker lock( m_csOlympic ); return m_nTimeIndex; };
    DWORD GetCountryID(){ KLocker lock( m_csOlympic ); return m_dwCountryID; };
    bool CheckGradeEnable( IN DWORD& dwGrade );

protected:
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadReward( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT VEC_REWARD& vecRewardInfo );
    bool LoadGradeRewardInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<DWORD,VEC_REWARD>& mapGradeRewardList );
    bool LoadGradeDescInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<DWORD,std::pair<DWORD,DWORD>>& mapGradeDesc, OUT DWORD& dwMaxGrade, OUT DWORD& dwMaxPoint );
    bool LoadOlympicScheduleInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector<KOlympicSchedule>& vecOlympicSchedule );
    bool LoadOlympicGameInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<int, KOlympicGameInfo>& mapOlympicGameInfo );
    bool ChkUpdateDay( IN time_t& tmDate ); // 하루지났는지에 대한 체크.
    bool ChkOneMinTick();
    bool ChkGameAlarm( IN time_t& tmDate );
    bool LoadTimeIndexDescInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector<KTimeIndexInfo>& vecTimeInfo_, OUT std::set<int>& setGameAlarmNotice );
    bool LoadSetInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::set<int>& setData );
    bool LoadPairInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::pair<DWORD,DWORD>& prData );
    void SendGameAlarmNotice( IN time_t& tmDate );
    int GetTimeIndex( IN int& nHour, IN int& nMinute );
    void UpdateGameInfo( IN time_t& tmDate );

    bool LoadOlympicGawibawiboWinLoseRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, PAIR_INT >& mapOlympicGawibawiboInfo_ );
    void SendCenterSyncData();
    void SetCurrentTimeIndex( OUT int& nTimeIndex );

protected:
    mutable KncCriticalSection                  m_csOlympic;
    time_t                                      m_tmBegin;          // 이벤트 시작 날짜.
    time_t                                      m_tmEnd;            // 이벤트 종료 날짜.
    DWORD                                       m_dwMaxGrade;       // 최대 등급.
    DWORD                                       m_dwMaxPoint;       // 최대 Point.

    // LuaScript Data
    std::vector<KOlympicSchedule>               m_vecOlympicSchedule;
    std::map<int, KOlympicGameInfo>             m_mapOlympicGameInfo;
    std::map<DWORD,VEC_REWARD>                  m_mapGradeRewardList;
    std::map<DWORD,std::pair<DWORD,DWORD>>      m_mapGradeDesc;
    std::map< int, PAIR_INT >                   m_mapOlympicGawibawiboWinLoseRatio;
    std::set<int>                               m_setGameAlarmNotice;
    std::vector<KTimeIndexInfo>                 m_vecTimeInfo;

    // 변경되는 국가 랭킹 데이터.
    std::vector<KRegDateRankInfo>               m_vecRegDateRank;
    std::vector<KTotalRankInfo>                 m_vecTotalRank;

    time_t                                      m_tmTodayDate;
    int                                         m_nGameType;   // GameType
    int                                         m_nTeamType;   // TeamType

    time_t                                      m_tmUpdateTime; // 게임바뀌는 시간.
    DWORD                                       m_dwLastTick;
    DWORD                                       m_dwTickGap;
    int                                         m_nTimeIndex;

    DWORD                                       m_dwCountryID;
    time_t                                      m_tmStatDate; // 통계날짜.
    int                                         m_nStatGameType; // 통계 GameType.
    int                                         m_nStatTeamType; // 통계 TeamType.
    __int64                                     m_biTotalData; // 전체값.(단체전)
    int                                         m_nDeltaData; // 차이값.(단체전)
    int                                         m_nTopPlayerData;// 1등 유저 데이터.(개인전)
    std::wstring                                m_strNickName; // 1등 유저 닉네임.(개인전)
    bool                                        m_bChanged;    // 1등 유저 변경 체크.
};

DefSingletonInline( KOlympicManager );
DeclOstmOperatorA( KOlympicManager );

//=========================================================================================//

class KOlympicUserData
{
public:
    KOlympicUserData();
    ~KOlympicUserData();
    bool IsLoaded() { return m_bLoaded; };
    void SetUserOlympicUserData( IN const time_t& tmDate, IN const int& nMyAccumulateData );
    void GetUserOlympicUserData( IN const time_t& tmTodayDate, OUT int& nMyAccumulateData );
    bool CheckRemainTryPoint();
    void SetRemainTryCount( IN const int& nRemainTryCount_ ) { m_nRemainTryCount = nRemainTryCount_; };
    int GetRemainTryCount() { return m_nRemainTryCount; };
    void DecreaseRemainTryCount( IN const int& nAmount );
    int GetMyAccumulateData() { return m_nMyAccumulateData; };
    void IncreaseMyAccumulateData( IN const time_t& tmTodayDate, IN const int& nAmount );
    void SetTimeIndex( IN int nTimeIndex, IN bool bPlayEnable );
    void SetPlayEnable( IN bool bPlayEnable_ ){ m_bPlayEnable = bPlayEnable_; };
    bool GetPlayEnable(){ return m_bPlayEnable; };
    void UpdateUserGrade();
    bool CheckUserGrade( IN const DWORD& dwGrade, IN const DWORD& dwMaxGrade );
    DWORD GetUserGrade(){ return m_dwGrade; };
    DWORD GetUserTimeIndex(){ return m_nCurrentTimeIndex; };
    void SetUserGrade( IN DWORD& dwGrade ) { m_dwGrade = dwGrade; };

protected:
    time_t                                      m_tmTodayDate;  // 유저 현재 게임중 날짜.
    bool                                        m_bLoaded;
    int                                         m_nRemainTryCount;  // 남은 도전 수
    int                                         m_nMyAccumulateData;
    int                                         m_nCurrentTimeIndex;
    bool                                        m_bPlayEnable;
    DWORD                                       m_dwGrade;
};
