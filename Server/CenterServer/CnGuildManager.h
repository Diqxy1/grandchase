#pragma once
#include <RTTI.h>
#include <ToString.h>
#include "KncSmartPtr.h"
#include <KNCSingleton.h>
#include "CenterPacket.h"
#include "CnGuild.h"
#include "IntEvent.h"
#include <queue>
#include "KncTimer.h"

SmartPointer(KCnGuild);
class KLuaManager;
class KCnGuildManager
{
    DeclareSingleton( KCnGuildManager );
    NiDeclareRootRTTI( KCnGuildManager );
    DeclToStringW;

public:
    enum UPDATE_TYPE {
        UT_GUILD_RANK = 1,  // 길드랭킹
        UT_GUILD_MARK = 2,  // 길드마크
    };

    enum UPDATE_CHECK {
        UC_NOT_CHANGED  = 0, // 정보 안바뀜.
        UC_CHANGED      = 1, // 정보가 바뀌었음(업데이트 필요)
    };

    enum SORT_TYPE {
        ST_GUILD_ID_DATE,   // GuildID/창립일순 정렬(기본)
        ST_GUILD_NAME,      // 이름순
        ST_GUILD_GRADE,     // 등급순
        ST_GUILD_POINT,     // 길드포인트순
        ST_MAX,
    };

public:
    KCnGuildManager(void);
    ~KCnGuildManager(void);

private:
    mutable KncCriticalSection                          m_csGuilds;
    std::map<DWORD,KCnGuildPtr>                         m_mapGuild; // <GuildID, GuildPtr>
    std::vector<KCnGuildPtr>                            m_vecGuild; // 정렬, 순회용.
    std::map<UCHAR,std::vector<DWORD> >                 m_mapGuildPage; // map<정렬타입,길드UID리스트>

    mutable KncCriticalSection                          m_csUserMap;
    std::map<DWORD,DWORD>                               m_mapUserInGuild; // <UserUID, GuildID> 검색용.

    // 길드 랭킹 관련.
    mutable KncCriticalSection                          m_csGuildRank;
    std::map< DWORD, KSerBuffer >                       m_mapCompressedRankData;  // std::map< DWORD, std::vector<KNGuildRankInfo> >
    std::map< DWORD, KSimpleGuildRank >                 m_mapSimpleGuildRank; // 데이터 전달용 map< GuildUID, RankInfo >

protected:
    KncTimer        m_kMinTimer;
    KncTimer        m_kGuildClearTimer;
    KSimpleDate     m_kLastDayCheckDate; // DayCheck.마지막 검사날짜

private:
    mutable KncCriticalSection          m_csEventQueue;
    bool                                m_bChangeRank;
    std::deque<KIntEventPtr>            m_queEvent;

    mutable KncCriticalSection          m_csConfig;
    KGuildConfig                        m_kGuildConfig;
    bool                                m_bConfigChanged;
    // 길드 포인트 갱신 관련.
    DWORD                               m_dwChangeGuildLastTick;
    DWORD                               m_dwUpdateGuildCount;

    mutable KncCriticalSection                  m_csChangeGuild;
    std::map<GCGuildUID, KGuildPointBroad>      m_mapChangePointGuild;
    DWORD                                       m_dwChangeGuildTickGap;

public:
    template<typename T>
    void QueueingEvent( unsigned short usEventID, DWORD dwUID, T& data );
    void QueueingEvent( KIntEventPtr spEvent ) { KLocker lock( m_csEventQueue ); m_queEvent.push_back( spEvent ); }
    void QueueingID( unsigned short usEventID, DWORD dwUID );

    void Init();
    void Tick();

    bool LoadScript();

    bool BroadGuildUserOnline( IN DWORD dwUserUID );
    bool BroadGuildUserOffline( IN DWORD dwUserUID );

    bool GuildChatProc( IN KChatData kChat );

    KCnGuildPtr GetGuild( IN DWORD dwGuildID );
    KCnGuildPtr GetUserGuild( IN DWORD dwUserUID );

    bool EnterGuild( IN );
    bool AddGuildNotice( IN const DWORD dwGuildUID, IN std::vector<KNGuildNotice>& vecNotice );

    // Guild List
    bool AddGuild( IN KNGuildInfo& kInfo );
    void AddGuild( IN std::map< DWORD, KNGuildInfo >& mapGuildList );
    void RemoveGuild( IN const DWORD dwGuildUID );
    void AddNewGuildInfo( IN KNGuildInfo& kInfo, std::vector<KNGuildNotice>& mapNotice, IN std::map<DWORD, KNGuildUserInfo>& mapUserList );

    // Notice
    bool ChangeGuildNotice( IN const DWORD dwGuildUID, IN const std::vector<KNGuildNotice>& vecNotice );

    // Member
    bool AddUser( IN const DWORD dwGuildUID, IN KNGuildUserInfo& kUserInfo, IN KCenterPtr spCenter );
    bool DrumoutUser( IN const DWORD dwGuildUID, IN const std::set<DWORD>& setUserUID, IN const char cOutType );
    bool SetGuildMemberInfo( IN const DWORD dwGuildUID, IN std::map<DWORD, KNGuildUserInfo>& mapGuildUserList );
    bool OfflineUser( IN DWORD dwUserUID );
    void SendGuildMemberListNot( IN const DWORD dwGuildUID, IN std::map<DWORD, KNGuildUserInfo>& mapGuildUserList ); // 센터서버와 연결한 각 게임서버에 전달
    void AddUserInGuildInfo( IN const DWORD dwUserUID, IN const DWORD dwGuildUID );
    void DelUserInGuildInfo( IN const DWORD dwUserUID );

    // MemberInfo
    bool ModifyMemberComment( IN const DWORD dwGuildUID, IN const DWORD dwUserUID, IN const std::wstring& strUserComment );
    bool ChangeGuildMemberLevel( IN const DWORD dwGuildUID, IN const std::map<char,std::set<DWORD> >& mapChangeMemberLevel );
    const std::wstring& GetMarkURL() { KLocker lock(m_csConfig); return m_kGuildConfig.m_strMarkServerURL; }
    void SetGuildRankData( IN std::map< DWORD, std::vector<KNGuildRankInfo> >& mapGuildRank );
    void SetRankInfoInGuildList( IN std::map<DWORD,KSimpleGuildRank>& mapRank );
    bool SetGulidRankInfo( IN const DWORD dwGuildUID, IN const int nWeeklyRank, IN const int nMonthlyRank, IN const int nTotalRank );
    void GetGuildRankData( OUT std::map< DWORD, KSerBuffer >& mapGuildRank );
    void SetSimpleGuildRank( IN std::map<DWORD,KSimpleGuildRank>& mapRank ) { KLocker lock(m_csGuildRank); m_mapSimpleGuildRank.swap(mapRank); }
    void GetSimpleGuildRank( OUT std::map<DWORD,KSimpleGuildRank>& mapRank) { KLocker lock(m_csGuildRank); mapRank = m_mapSimpleGuildRank; }
    void GetGuildRank( IN const DWORD dwGuildUID, OUT KSimpleGuildRank& kRank );
    void MakeGuildMarkName( IN const DWORD dwGuildUID_, IN const int nRevision_,
                            IN const std::wstring& strExt_, OUT std::wstring& strFileName_ );

    void GetGuildConfig( OUT KGuildConfig& kConfig );

    DWORD GetUserGuildID( IN const DWORD dwUserUID );
    void PrintGuild( DWORD dwGuildUID );
    void PrintUser( DWORD dwUserUID );
    bool GetGuildInfo( IN const DWORD dwGuildUID, OUT KNGuildInfo& kGuildInfo );

    void GetGuildPageIDList( OUT std::map<UCHAR,KSerBuffer>& mapData );
    void SendUpdateGuildListReqToDB();
    // Guild Battle Point Update.
    void UpdateChangeGuildInfo( IN const GCGuildUID& guildUID, IN KGuildPointBroad& kGuildPointBroad );
    size_t GetChangePointGuildCount() { KLocker lock( m_csChangeGuild ); return m_mapChangePointGuild.size(); }
    void GetChangeGuildInfo( OUT std::map<GCGuildUID, KGuildPointBroad>& mapChangePointGuild );
    void SendChangeGuildInfo( IN std::map<GCGuildUID, KGuildPointBroad>& mapChangePointGuild );

private:
    // GuildPage
    bool IsPageEmpty();
    void BuildGuildPageList();

    void DistributeEvent();
    bool IsRankChange() { KLocker lock(m_csGuildRank); return m_bChangeRank; }
    void SetRankChange( bool bChange ) { KLocker lock(m_csGuildRank); m_bChangeRank = bChange; }
    void SendGuildRankData();
    bool IsConfigChanged();
    void CheckRankInfo( IN OUT KNGuildInfo& kInfo );

    bool LoadUnicode( IN KLuaManager& kLuaMng, OUT std::vector< std::pair<DWORD,DWORD> >& vecAllowedUnicode );
    bool LoadGuildStoreTabUserPoint( IN KLuaManager& kLuaMng, OUT std::map<int,DWORD>& mapData );
    bool LoadGuildLvPoint( IN KLuaManager& kLuaMng, OUT std::map<USHORT,int>& mapData );
    bool LoadGuildLvAdvantage( IN KLuaManager& kLuaMng, OUT std::map<USHORT,std::pair<int,float> >& mapData );
    bool LoadContributePointAdventageLv( IN KLuaManager& kLuaMng, OUT std::map<int,USHORT>& mapData );
    bool LoadChannelGradeByBattlePoint( IN KLuaManager& kLuaMng_, OUT std::map< char, std::pair< DWORD, DWORD > >& mapData_ );
    bool LoadGuildPoint( IN KLuaManager& kLuaMng_, std::map< int, std::pair< int, int > >& mapData_ );
    void IntervalTick();
    bool DayCheck();
    void EachGuildTick();
    void ClearGuildInfoCheck();

    int PageElementNum() { KLocker lock( m_csConfig ); return m_kGuildConfig.m_nGuildPageSize; }
    int MaxPageSize() { KLocker lock( m_csConfig ); return m_kGuildConfig.m_nMaxPageSize; }

    void PrintGuildUserListInfo( IN const DWORD dwGuildUID );

    void GetGuildUIDList( IN const std::vector<KCnGuildPtr>& vecGuild, OUT std::vector<DWORD>& vecGuildUID );
    void SendGuildPageNot();

    void AddPageNewGuild( IN const DWORD dwGuildUID );
    void RemovePageGuild( IN const DWORD dwGuildUID );
    void UpdateGuildPointTick();
};

DefSingletonInline( KCnGuildManager );
DeclOstmOperatorA( KCnGuildManager );

template< typename T >
void KCnGuildManager::QueueingEvent( unsigned short usEventID_, DWORD dwUID_, T& data_ )
{
    KIntEventPtr spEvent( new KIntEvent );

    spEvent->m_usEventID    = usEventID_;
    spEvent->m_dwSenderUID  = dwUID_;

    KSerializer ks;
    ks.BeginWriting( &spEvent->m_kbuff );
    ks.Put( data_ );
    ks.EndWriting();

    QueueingEvent( spEvent );
}
