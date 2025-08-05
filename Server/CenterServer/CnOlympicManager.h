#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include <map>
#include "CommonPacket.h"

class KLuaManager;
struct KECN_OLYMPIC_EVENT_DATA_NOT;

class KCnOlympicManager
{
    DeclareSingleton( KCnOlympicManager );
    NiDeclareRootRTTI( KCnOlympicManager );
    DeclToStringW;

    enum OLYMPIC_GAME_TEAM {
        OGT_SOLO = 1,
        OGT_TEAM = 2,
        OGT_MAX,
    };

public:
    KCnOlympicManager(void);
    ~KCnOlympicManager(void);

    bool LoadScript();
    void Tick();
    bool IsRun(); // 이벤트 진행중인지 체크.
    void UpdateUserSoloData( IN const time_t& tmDate, IN const int& nGameType, IN const int& nTeamType, IN const std::wstring& wstrNickName, IN const int& nAccumulateData );
    void UpdateUserTeamData( IN const time_t& tmDate, IN const int& nGameType, IN const int& nTeamType, IN const int& nAccumulateData );
    void UpdateRankDB( IN std::vector<KRegDateRankInfo>& vecRegDateRank, IN std::vector<KTotalRankInfo>& vecTotalRank, IN const bool& bInit );
    void InitProcess();
    void GetOlympicUserData( OUT std::map<time_t, KOlympicInfoData>& mapOlympicInfoData );
    void UpdateUserData( IN std::map<time_t, KOlympicInfoData>& mapOlympicInfoData );
    void GetOlympicEventData( OUT KECN_OLYMPIC_EVENT_DATA_NOT& kPacket );
    void GetOlympicRankInfo( OUT std::vector<KRegDateRankInfo>& vecRegDateRank, OUT std::vector<KTotalRankInfo>& vecTotalRank );

protected:
    bool ChkUpdateFromDB();
    bool LoadUpdateTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool CheckTick();
    void UpdateDBRankData();

protected:
    mutable KncCriticalSection                  m_csCnOlympic;
    time_t                                      m_tmBegin; // 이벤트 시작 날짜
    time_t                                      m_tmEnd; // 이벤트 종료 날짜
    time_t                                      m_tmDBUpdateTime; // 다음 DB 업데이트 시간
    DWORD                                       m_dwLastTick;
    DWORD                                       m_dwTickGap;

    std::vector<KRegDateRankInfo>               m_vecRegDateRank;
    std::vector<KTotalRankInfo>                 m_vecTotalRank;
    std::map<time_t, KOlympicInfoData>          m_mapOlympicInfoData;
/*
    time_t                                      m_tmTodayDate;
    int                                         m_nGameType;   // GameType
    int                                         m_nTeamType;   // TeamType
    __int64                                     m_biTotalAccumulateData;
    int                                         m_nTopRankUserData;
    std::wstring                                m_wstrNickName;
*/
    bool                                        m_bChanged; // 갱신여부.
};

DefSingletonInline( KCnOlympicManager );
DeclOstmOperatorA( KCnOlympicManager );