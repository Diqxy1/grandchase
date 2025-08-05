#pragma once

#include "KNCSingleton.h"


struct SOlympicGameInfo
{
    int                         m_nGameType;     // GameType.
    int                         m_nDefaultCount; // 기본으로 지급할 횟수정보.
    int                         m_nRewardCount;  // 보상을 받을 목표 Count.
    int                         m_nOnePointMaxPoint; // 한게임에 획득할수 있는 최대 Point.
    int                         m_nTotalMaxPoint; // 유저가 획득할 수 있는 최대 Point.
    std::vector<KDropItemInfo>  m_vecRewardInfo; // 보상 아이템 정보.

    SOlympicGameInfo()
        : m_nGameType( 0 )
        , m_nDefaultCount( 0 )
        , m_nRewardCount( 0 )
        , m_nOnePointMaxPoint( 0 )
        , m_nTotalMaxPoint( 0 )
    {
    }
};


class GCOlympicManager
{
	DeclareSingleton( GCOlympicManager );

public:
    enum 
    {
        OLYMPIC_NATION_KR = 0,
        OLYMPIC_NATION_TW = 1,
        OLYMPIC_NATION_BR = 2,
        OLYMPIC_NATION_US = 3,
        OLYMPIC_NATION_PH = 4,
        OLYMPIC_NATION_TH = 5,
        OLYMPIC_NATION_ID = 6,
        OLYMPIC_NATION_LA = 7,
        NUM_OLYMPIC_NATION,
    };

public:
    GCOlympicManager();
    ~GCOlympicManager();

private:
    bool                m_bSetInfo;                                 // 올림픽 관련 정보 세팅 되었나?

    bool                m_bRewardNotice;                            // 보상 받을 수 있는 아이템이 있는지
    bool                m_bPlayEnable;                              // 게임 플레이 가능 여부 
    DWORD               m_dwRewardGrade;                            // 보상 받을수 있는 등급 
    DWORD               m_dwCountryID;                              // 나의 국가 ID
    GCDeviceTexture*    m_pOlympicEvent;                            // 올림픽 레인보우
    float               m_fX1Pos;                                   // 텍스쳐 좌표
    bool                m_bIsRight;                                 // 텍스쳐 움직임
    bool                m_bIsOlympicEvent;                          // 올림픽 기간인가?

    std::vector< KOlympicSchedule >     m_vecSchedule;  // 일정 정보 
    std::vector< KRegDateRankInfo >     m_vecDateRank;  // 일별 우승자/우승국 정보 
    std::vector< KTotalRankInfo >       m_vecTotalRank; // 국가 순위 정보 

    std::map< DWORD, std::pair< DWORD, DWORD > >    m_mapGradeDesc;         // 점수별 등급 정보 < 등급, <시작점수, 끝점수 > >
    std::map< DWORD, std::vector< KDropItemInfo > > m_mapGradeRewardList;   // 등급별 보상 아이템 정보 < 등급, vector<보상아이템> >

    KEVENT_OLYMPIC_JOIN_GAME_ACK m_kGameToday;

    

private:
    void SetPlayEnable( const bool bEnable_ ) { m_bPlayEnable = bEnable_; }
    void SetDateRankList( IN std::vector< KRegDateRankInfo >& vecList_ );
    void SetTotalRankList( const IN std::vector< KTotalRankInfo >& vecList_ );
    void SetGradeDesc( const IN std::map< DWORD, std::pair< DWORD, DWORD > >& mapGrade_ );
    void SetGradeReward( const IN std::map< DWORD, std::vector< KDropItemInfo > >& mapGradeReward_ );

public:
    void SetRewardGrade( const DWORD dwGrade ) { m_dwRewardGrade = dwGrade; }
    void SetInfoState( const bool bSet_ ) { m_bSetInfo = bSet_; }
    bool IsSetInfoState( void ) { return m_bSetInfo; }

    void ClearOlympicInfo( void );
    void ClearOlympicSchedule( void );
    void SetOlympicInfo( IN KEVENT_OLYMPIC_INFO_ACK info_ );
    void SetScheduleList( IN std::vector< KOlympicSchedule >& vecList_ );

    bool IsPlayEnable( void ) { return m_bPlayEnable; }
    bool GetScheduleList( OUT std::vector< KOlympicSchedule >& vecList_ );
    bool GetDateRankList( OUT std::vector< KRegDateRankInfo >& vecList_ );
    bool GetTotalRankList( OUT std::vector< KTotalRankInfo >& vecList_ );
    bool GetGradeDesc( OUT std::map< DWORD, std::pair< DWORD, DWORD > >& mapGrade_ );
    bool GetGradeReward( OUT std::map< DWORD, std::vector< KDropItemInfo > >& mapGradeReward_ );

    bool GetGradeRewardByScore( OUT std::map< DWORD, std::vector< KDropItemInfo > >& mapGradeReward_ );
    bool GetScheduleListWithDateRank( OUT std::vector< std::pair< KOlympicSchedule, KRegDateRankInfo > >& vecList_ );

    std::pair<int, int> GetTodayOlympicGameType( void );  // <게임모드, 팀모드>
    std::wstring GetTodayOlympicGameName( void );
    std::wstring GetOlympicGameName( const int eMode_, const int eTeam_ );
    std::wstring GetTimeToString( const time_t tm_ );
    bool         IsToday( const time_t tm_ );
    bool         GetRewardGradeFromScore( const IN DWORD dwScore, OUT DWORD &dwGrade_ );
    bool         GetRewardGrade( IN const DWORD dwIndex, OUT DWORD& dwGrade_ );
    DWORD        GetCanRecvRewardGrade( void ) { return m_dwRewardGrade; }
    void SendOlympicTreeDropResult( int iScore );
    void SendOlympicBalloonResult( int iScore );

    void OlympicStartGame( KEVENT_OLYMPIC_JOIN_GAME_ACK& kRecv_ );
    void OlympicQuitCurrentGame( void );
    void ShowMinigameReward();
    void SetRewardNotice( const bool bNotice ) { m_bRewardNotice = bNotice; }
    bool IsOlympicNotice();
    void Render();
    void SetOlympicAlram();
    void SetOlympicEventAlram( bool bIsOlympicEvent );


};

DefSingletonInline( GCOlympicManager );