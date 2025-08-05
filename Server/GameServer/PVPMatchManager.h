#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include <map>
#include "CommonPacket.h"
#include "UserPacket.h"

struct KConstantKInfo
{
    int                     m_nRaStartValue;
    int                     m_nRaEndValue;
    int                     m_nConstantK;
};

struct KPlacementTestRPTable
{
    DWORD                   m_dwStartLevel;
    DWORD                   m_dwEndLevel;
    int                     m_nStartRPValue;
};

struct KRPDefineTable
{
    DWORD                   m_dwStartRP;
    DWORD                   m_dwEndRP;
    int                     m_nDefineRP;
};

struct KRPMarkGradeTable
{
    DWORD                   m_dwStartRP;
    DWORD                   m_dwEndRP;
    DWORD                   m_dwGrade;
};

class KLuaManager;
class KPVPMatchManager
{
    DeclareSingleton( KPVPMatchManager );
    NiDeclareRootRTTI( KPVPMatchManager );
    DeclToStringW;

public:
    enum CalcType
    {
        CT_PLACEMENTTEST = 0, // 배치고사
        CT_NORMAL = 1,        // 일반
        CM_MAX,
    };

    enum KModeInfo 
    {
        MI_PVP_1VS1 = 0,
        MI_PVP_2VS2 = 1,
        MI_PVP_3VS3 = 2,
        MI_PVP_MAX,
    };

    enum TeamInfo
    {
        TI_RED = 0,
        TI_BLUE = 1,
        TI_MAX,
    };

    KPVPMatchManager(void);
    ~KPVPMatchManager(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    int GetConstantK( IN const int& nRP );
    void GetPlacementTestRP( IN const DWORD& dwLevel, OUT int& nRP );
    void CalcChangeRP( IN const int& nOurTeamRP, IN const int& nOtherTeamRP, IN const bool& bWin, IN OUT int& nMyRP );
    void CalcPlacementTestChangeRP( IN const int& nOurTeamRP, IN const int& nOtherTeamRP, IN const bool& bWin, IN OUT int& nConstantK, IN const int& nMatchTotalCount, IN OUT int& nLastWinLose, IN OUT int& nMyRP );
    int GetMaxReceiveRP( IN const int& nType );
    float GetELOAddExpRatio(){ KLocker lock( m_csPVPMatchManager ); return m_fAddExpRatio; };
    float GetELOAddGPRatio(){ KLocker lock( m_csPVPMatchManager ); return m_fAddGPRatio; };
    void CalcRatingPoint( IN std::pair<int, int>& prELOTeamRPAvg, IN bool IsBlueTeam, IN const bool& bWin, IN OUT int& nMyRP );
    void CalcPlacementTestRatingPoint( IN std::pair<int, int>& prELOTeamRPAvg, IN bool IsBlueTeam, IN const bool& bWin, IN OUT int& nConstantK, IN const int& nMatchTotalCount, IN OUT int& nLastWinLose, IN OUT int& nMyRP );
    int GetMatchPartyMaxNum(){ KLocker lock( m_csPVPMatchManager ); return m_nMatchPartyMaxNum; };
    int GetDefineRP( IN const int& nRP );
    void InitModeCount();
    void SetModeCount( IN const int& nModeInfo, IN const int& nWaitListCount );

    int GetModeCount( IN const int& nModeInfo );
    bool CalcMatchTeam( IN const int& nModeInfo, IN const std::vector<KPartyData>& vecPartyData, OUT std::map<int, std::vector<KPartyMember> >& mapPartyMember );
    bool CheckCorrectPartyData( IN const int& nModeInfo, IN const int& nPartySize );
    void GetLimitMatchData( OUT bool& bLimitMatchEnable, OUT int& nLimitMatchGroupNum );

    bool SetModeTeam( IN const std::vector<KPartyData>& vecPartyData, IN int nStaticTeamSize, OUT std::map<int, std::vector<KPartyData>>& mapTeamParty );

    void CalcPartyMemeber( IN std::map<int, std::vector<KPartyData>>& mapTeamParty, OUT std::map<int, std::vector<KPartyMember> >& mapPartyMember );
    void GetSeasonInfo( OUT int& nSeasonYear, OUT int& nSeasonVersion );
    void CalcELOType( IN const int& nMatchTotalCount, OUT int& nELOType );
    DWORD GetMinDelMatchTime(){ KLocker lock( m_csPVPMatchManager ); return m_dwMinDelMatchTime; };
    void CheckModeCount( IN const int& nModeInfo, IN const int& nWaitListCount );
    DWORD GetMarkGrade( IN const int& nRP );
    void CheckDecreaseMarkGrade( IN const int& nRP, IN OUT UCHAR& ucGrade );
    void CalcCharELOGrade( IN const int& nRatingPoint, IN OUT UCHAR& ucGrade );
    void GetMatchTimeTable( OUT KEVENT_MATCH_TIME_TABLE_NOT& kPacket );
    int GetPlacementTestPlayCount(){ KLocker lock( m_csPVPMatchManager ); return m_nPlacementTestPlayCount; };
    void GetModeInfo( OUT std::map<int,int>& mapModeCount );
    void Tick();
    bool AddMatchEnable();
    bool CheckModeOpenEnable( IN const int& nModeInfo );

private:
    bool LoadConstantKTable( IN KLuaManager& kLuaMng, OUT std::vector<KConstantKInfo>& vecConstantKInfo );
    bool LoadPlacementTestRPTable( IN KLuaManager& kLuaMng, OUT std::vector<KPlacementTestRPTable>& vecPlacementTestRPTable );
    bool LoadRPDefineTable( IN KLuaManager& kLuaMng, OUT std::vector<KRPDefineTable>& vecRPDefineTable );
    void PrintMatchModeInfo( std::wostream& stm ) const;
    bool LoadRPMarkGradeTable( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT std::vector<KRPMarkGradeTable>& vecRPMarkGradeTable );
    bool LoadOpenTimeTable( IN KLuaManager& kLuaMng, OUT bool& bOpenTimeEnable, OUT std::map<int, KMatchOpenTime>& mapOpenTimeTable );
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::pair<int,int>& prTime );
    bool LoadModeMaxMinInfo( IN KLuaManager& kLuaMng, OUT std::map<int,std::pair<int,int>>& mapModeMaxMinInfo );
    void SendMatchInfoAllUser();
    bool LoadModeOpenEnableInfo( IN KLuaManager& kLuaMng, OUT std::map<int,bool>& mapModeOpenEnable );
    bool LoadAlarmTime( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT std::vector<int>& vecAlarm );
    void SendMatchAlarmNot( IN const int nAlarmType, IN const std::pair<int,int>& prBeginTime, IN const std::pair<int,int>& prEndTime, IN const int nMinute );
    void MatchAlarmNotice();
    void CheckAlarmNotice( IN time_t& tmCurrent, IN const std::pair<int,int>& prBeginTime, IN const std::pair<int,int>& prEndTime );

private:
    mutable KncCriticalSection              m_csPVPMatchManager;
    std::vector<KConstantKInfo>             m_vecConstantKInfo;
    std::vector<KPlacementTestRPTable>      m_vecPlacementTestRPTable;
    int                                     m_nPlacementTestPlayCount;
    int                                     m_nPlacementTestFirstConstantK;
    float                                   m_fPlacementTestRatio;
    int                                     m_nMaxConstantK;
    int                                     m_nMaxReceiveRP;
    int                                     m_nPlacementTestMaxReceiveRP;
    int                                     m_nMaxStartRPValue;
    float                                   m_fAddExpRatio;
    float                                   m_fAddGPRatio;
    int                                     m_nMatchPartyMaxNum;
    std::vector<KRPDefineTable>             m_vecRPDefineTable;
    std::map<int,int>                       m_mapModeCount; // ModeType, Cnt
    bool                                    m_bLimitMatchEnable; // 최소 수치 
    int                                     m_nLimitMatchGroupNum; // 최소 인원수.
    int                                     m_nSeasonYear;
    int                                     m_nSeasonVersion;
    DWORD                                   m_dwMinDelMatchTime; // 나가는 통계를 위한 최소 매칭 대기 시간.
    bool                                    m_bPenaltyEnable; // 패널티 적용 여부.
    int                                     m_nPenaltyRefuseCount; // 패널티 적용 거절 Count.
    DWORD                                   m_dwPenaltyTime; // 패널티 적용 시간.
    DWORD                                   m_dwMaxGrade;
    std::vector<KRPMarkGradeTable>          m_vecRPMarkGradeTable;
    std::vector<KRPMarkGradeTable>          m_vecRPMarkDecreaseGradeTable;
    // 오픈 시간 설정.
    bool                                    m_bOpenTimeEnable;
    std::map<int, KMatchOpenTime>           m_mapOpenTimeTable;
    std::map<int,std::pair<int,int>>        m_mapModeMaxMinInfo;
    std::map<int,bool>                      m_mapModeOpenEnable;

    std::vector<int>                        m_vecOpenAlarm;         // 오픈전 알람메세지 설정(분)
    std::vector<int>                        m_vecCloseAlarm;        // 닫히기전 알람메세지 설정(분)

    // 서버의 시작 시간을 저장하고 있는 변수.
    time_t                                  m_tmStartBegin;

    // Tick 관련 데이터
    DWORD                                   m_dwLastTick;
    DWORD                                   m_dwTickGap;
};

DefSingletonInline( KPVPMatchManager );
DeclOstmOperatorA( KPVPMatchManager );
