#pragma once
#include <ToString.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <Thread/Locker.h>
#include <atltime.h>
#include <map>
#include <vector>
#include "UserPacket.h"

class KLuaManager;

class KRankManager
{
    DeclareSingleton( KRankManager );
    NiDeclareRootRTTI( KRankManager );
    DeclToStringW;
public:
    KRankManager(void);
    ~KRankManager(void);

    enum RANK_TYPE
    {
        RT_DAILY_RANK,
        RT_WEEFLY_RANK,
        RT_MONTHLY_RANK,
        RT_EXP_RANK,
        RT_GUILD_DAILY_RANK,
        RT_GUILD_WEEKLY_RANK,
        RT_GUILD_MONTHLY_RANK,
        RT_GUILD_TOTAL_RANK,
        RT_MAX,
    };

    enum MATCH_RANK_TYPE
    {
        MRT_RP = 0, // RP 랭킹.( 전체, 캐릭터별 )
        MRT_WINRECORD, // 다승 랭킹.( 전체, 캐릭터별 )
        MRT_BEFORE_RP, // 다승 랭킹.( 전체, 캐릭터별 )
        MRT_BEFORE_WINRECORD, // 경험치 랭킹.( 전체, 캐릭터별 )
        MRT_EXP, // 경험치 랭킹.( 전체, 캐릭터별 )
        MRT_MAX,
    };

    void Tick();
    void SetUpdateTime( int nHour, int nMiute );
    bool CheckUpdate();
    bool GetPageData( char cRankType, char cCharType, char cPage, KEVENT_RANK_PAGE_ACK& kPacket );

    // 랭크 데이터 생성 관련.
    void UpdateExpRankData( IN std::map<char, std::vector<KExpRank> >& mapExpRank );
    void UpdateWinRankData( IN const char cRankType, IN std::map<char, std::vector<KWinRateRank> >& mapWinRank );
    void BuildRankData(); // Raw Data를 압축 한다.

    // 080623 tslayer.
    void SetRankSearchTime( IN unsigned int uiRankSearchTime ); // 랭킹 검색 tick 설정

    static unsigned int ms_uiRankSearchGap;     // 랭킹 검색 주기
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void UpdateMatchRankData( IN const char& cRankType, std::map<char, std::vector<KMatchRank> >& mapMatchRank );
    void UpdateMatchExpRankData( std::map<char, std::vector<KExpRank> >& mapMatchExpRank );
    void BuildMatchRankData();
    bool GetMatchRankPageData( IN const char cRankType, IN const char cCharType, IN const char cPage, OUT KEVENT_MATCH_RANK_PAGE_ACK& kPacket );
    void GetRankRewardInfo( OUT KEVENT_MATCH_RANK_REWARD_NOT& kPacket );
    bool CheckRankType( IN const char& cRankType );
    void GetUpdateTime( OUT KEVENT_MATCH_RANK_CHANGE_NOT& kPacket );

protected:
    void UpdateFlushTime();
    void BuildWinRankData( IN std::vector<KWinRateRank>& vecRankData, OUT std::vector<KListPage>& vecRankCompData );
    void BuildExpRankData( IN std::vector<KExpRank>& vecRankData, OUT std::vector<KListPage>& vecRankCompData );
    void AssignCompData( char cRankType, char cCharType, std::vector<KListPage>& vecCompData );

    void DumpExpRank();
    void DumpMatchRank();
    bool LoadUpdateTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT int& nUpdateHour, OUT int& nUpdateMinute );
    bool LoadRankRewardInfo( KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<int, std::map<char, std::vector<KRankRewardInfo>>>& mapRankRewardInfo );
    bool LoadItems( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector<GCITEMID>& vecItems );
    void BuildMatchRankData( IN std::vector<KMatchRank>& vecMatchRankData, OUT std::vector<KListPage>& vecMatchRankCompData );
    void AssignMatchCompData( IN const char cRankType, IN const char cCharType, IN const std::vector<KListPage>& vecCompData );
    void SendToDayChangeNotice();

    DWORD   m_dwLastTick;
    mutable KncCriticalSection  m_csRankData;
    int     m_nUpdateHour;
    int     m_nUpdateMinute;
    CTime   m_tmUpdateTime;

    std::map<char, std::vector<KExpRank> >      m_mapExpRank;
    std::map< int, std::map<char, std::vector<KWinRateRank> > > m_mapRank;


    // RANK_TYPE, charType, CompData
    std::map<char, std::map<char, std::vector<KListPage> > > m_mapCompRankData;

    // 매칭 랭킹 데이터.
    // 위의 기존 랭킹 코드는 대전 매칭후에 일괄삭제 예정이라서 변수를 새로 추가한다.
    std::map<int, std::map<char, std::vector<KRankRewardInfo>>> m_mapRankRewardInfo;
    std::map< int, std::map<char, std::vector<KMatchRank>>>  m_mapMatchRank;
    std::map<char, std::map<char, std::vector<KListPage> > > m_mapCompMatchRankData;
    std::map<char, std::vector<KExpRank>>    m_mapMatchExpRank;
};

DefSingletonInline( KRankManager );
DeclOstmOperatorA( KRankManager );