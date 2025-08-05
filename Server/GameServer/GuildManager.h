#pragma once

#include "Guild.h"
#include "Performer.h"
#include "KncTimer.h"
#include "ManufactureS3.h"

#define GUILD_MANAGER_NAME L"_%%GUILD^MANAGER%%_"

class KGuildManager : public KPerformer
{
    DeclareSingleton( KGuildManager );
    DeclToStringW;

public:
    virtual ~KGuildManager();

    enum ENUM_ORDER_BY {
        EOB_ASC = 0, // 올림차순 정렬(값이 작은것 부터)
        EOB_DESC = 1, // 내림차순 정렬
        EOB_MAX,
    };

    enum SORT_TYPE {
        ST_GUILD_ID_DATE,   // GuildID/창립일순 정렬(기본)
        ST_GUILD_NAME,      // 이름순
        ST_GUILD_GRADE,     // 등급순
        ST_GUILD_POINT,     // 길드포인트순
        ST_MAX,
    };
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void DumpStoreList();

    // 길드 추가/삭제
    bool Add( IN const KNGuildInfo& kGuildInfo_ ); // 길드매니저에 길드 추가
    void AddGuild( IN std::map<DWORD,KNGuildInfo>& mapAddGuildList ); // 길드매니저에 길드리스트 추가
    bool RemoveGuild( IN const DWORD dwGuildUID );
    // 길드생성
    void CreateGuild( IN const KNGuildInfo& kInfo, IN std::vector<KNGuildNotice>& vecNotice, IN std::map<DWORD,KNGuildUserInfo>& mapMember, IN KUserPtr spUser );

    // 길드 공지/맴버리스트 설정
    bool SetGuildNotice( IN const DWORD dwGuildUID, IN std::vector<KNGuildNotice>& vecNotice );
    bool GetNotice( IN const DWORD dwGuildUID, OUT std::vector<KNGuildNotice>& vecNotice );
    bool ModifyNotice( IN const DWORD dwGuildUID, IN std::vector<KNGuildNotice>& vecNotice );
    bool ModifyGuildNotice( IN const DWORD dwGuildUID, IN std::vector<KNGuildNotice>& vecNotice );
    bool SetGuildMemberList( IN const DWORD dwGuildUID, IN std::map< DWORD, KNGuildUserInfo >& mapMemberList );

    // 길드 페이지(클라이언트 전달용)
    int GetGuildPageSize() { KLocker lock(m_csConfig); return m_kGuildConfig.m_nGuildPageSize; }
    void SetGuildPageID( IN const UCHAR ucSortType, IN const KSerBuffer& kBuf );
    void UpdateGuildPage( IN const UCHAR ucSortType, IN const UCHAR ucOrderBy, IN const int nPageNum, IN const KSerBuffer& kPageBuf );
    bool GetGuildPage( IN OUT UCHAR& ucSortType, IN OUT UCHAR& ucOrderBy_, IN OUT int& nPageNum, OUT KSerBuffer& kPage, OUT int& nTotalPageNum );
    void GetGuildSearchPage( IN const int nPageNum, IN std::vector<DWORD>& vecGuildUID, OUT std::vector<KNGuildInfo>& vecGuildInfo );
    void BuildGuildPage();
    void BuildOnePage( IN const UCHAR ucOrderBy_, IN std::vector<DWORD>& vecGuildUID, OUT KSerBuffer& kBuff );
    bool GetGuildPageIDListSort( IN const UCHAR ucSortType, OUT std::vector<DWORD>& vecData );
    int GetTotalGuildPageNum() { KLocker lock( m_csPage ); return m_nTotalGuildPageNum; }
    int GetCurrentGuildCount();
    void DumpGuildPage( IN const int nSortType, IN const int nOrderBy, IN const int nPageNum );
    void DumpGuildPageSort( IN const int nSortType );
    bool UpdatePageInfo( IN const DWORD dwGuildUID );

    // 길드원 처리
    bool AddUser( IN const KNGuildUserInfo& kUserInfo );
    bool Register( IN const DWORD dwGuildUID, IN KUserPtr spUser );
    void Unregister( IN KUserPtr spUser );
    bool LogOffUser( IN const DWORD dwGuildUID, IN const DWORD dwUserUID );
    bool DeleteUser( IN const DWORD dwGuildUID, IN const DWORD dwUserUID, IN const char cOutType );
    // 탈퇴/추방 처리
    void DrumoutUser( IN const DWORD dwGuildUID, IN std::set<DWORD>& setUserUID, IN const char cOutType );
    // 맴버레벨 변경 처리
    bool ChangeGuildMemberLevel( IN const DWORD dwGuildUID, IN const std::map<char,std::set<DWORD> >& mapChangeMemberLevel );

    KGuildPtr GetGuild( IN const DWORD dwGuildUID );
    bool GetGuildInfo( IN const DWORD dwGuildUID, OUT KNGuildInfo& kGuildInfo );

    // derived from KPerformer
    KEvent* CreateKEvent() { return new KUserEvent; }
    virtual void Tick();

    //bool UpdateGuildPoint( IN const DWORD dwGuildUID, IN const DWORD dwPoint );
    bool UpdateGuildPoint( IN const DWORD dwGuildUID, IN const DWORD dwPoint, IN const DWORD dwUserUID, IN const int nWin, IN const int nLose );

    void BroadChat( IN const DWORD dwGuildUID, IN KChatData& kChat );

    // For Guild Makr..
    bool CheckGuildMarkExt( IN const std::wstring& strExt );
    bool CheckGuildMarkSize( IN const DWORD dwMarkSize );
    GCITEMID GetGuildMarkItemID();
    int GetGuildMarkItemFactor();
    GCITEMID GetGuildMarkCashItemID();
    void MakeGuildMarkName( IN const DWORD dwGuildUID, IN const int nRevision,
                            IN const std::wstring& strExt, OUT std::wstring& strFileName );

    // 길드생성
    int GetGuildNameMinSize()       { KLocker lock(m_csConfig); return m_kGuildConfig.m_prGuildNameLength.first; }
    int GetGuildNameMaxSize()       { KLocker lock(m_csConfig); return m_kGuildConfig.m_prGuildNameLength.second; }
    DWORD GetGuildMasterMinLevel()  { KLocker lock(m_csConfig); return m_kGuildConfig.m_dwGuildMasterMinLevel; }
    int GetGuildCreateGP()          { KLocker lock(m_csConfig); return m_kGuildConfig.m_nGuildCreateGP; }

    bool SetMemberComment( IN const DWORD dwGuildUID, IN const DWORD dwUserUID, IN const std::wstring& strMyComment );
    bool ModifyMemberComment( IN const DWORD dwGuildUID, IN const DWORD dwUserUID, IN const std::wstring& strMyComment );

    // 길드 이름 바꾸기
    bool ModifyGuildName( IN const DWORD dwGuildUID, IN std::wstring& strGuildName );
    bool ModifyName( IN const DWORD dwGuildUID, IN std::wstring& strGuildName );
    GCITEMID GetGuildNameItemID() { KLocker lock( m_csConfig ); return m_kGuildConfig.m_dwChangGuildNameItemID; }
    void SetGuildRankData( IN std::map< DWORD, KSerBuffer >& mapGuildRank );
    void GetGuildRankData( IN const int nType, OUT KSerBuffer& buffGuildRank );

    // 길드 URL
    int GetGuildURLMinSize()       { KLocker lock(m_csConfig); return m_kGuildConfig.m_prGuildURLLength.first; }
    int GetGuildURLMaxSize()       { KLocker lock(m_csConfig); return m_kGuildConfig.m_prGuildURLLength.second; }
    bool ChangeURL( KNGuildURL& kGuildURL_ );
    bool ChangeURLToCenter( KNGuildURL& kGuildURL );

    // 길드 가입 승인 방법 바꾸기
    bool ChangeJoinPolicy( KNGuildJoinPolicy& kPolicy );

    void SetGuildConfig( IN const KGuildConfig& kConfig );

    // 길드등급 변경
    bool ChangeGuildGrade( IN const DWORD dwGuildUID, IN const UCHAR ucGrade );

    bool GetGuildMemberList( IN const DWORD dwGuildUID, OUT std::map< DWORD, KNGuildUserInfo >& mapUserList );

    // 정보 출력( 서버 명령 )
    void PrintGuild( DWORD dwGuildUID_ );

    // 검색어 최소길이
    int GetKeywordMinLength()   { KLocker lock(m_csConfig); return m_kGuildConfig.m_nKeywordMinLength; }
    bool GetAllowUnicode() { KLocker lock(m_csConfig); return m_kGuildConfig.m_bAllowUnicode; }
    void GetUnicode( std::vector< std::pair<DWORD,DWORD> >& vecUnicode );

    void UpdateGuildPointFromCenter( IN std::map<GCGuildUID, KGuildPointBroad>& kPacket );

    void GetGuildUIDList( OUT std::set<DWORD>& setGuildUID );

    int GetOfficailGuildMemberNumber() { KLocker lock(m_csConfig); return m_kGuildConfig.m_nOfficailGuildMemberNum; }
    bool UpdateMemberNick( IN const DWORD dwGuildUID, IN const DWORD dwUserUID, IN const std::wstring& strNickName, IN const std::wstring& strNickColor);
    bool SendMemberLevelupNotice( IN const DWORD dwGuildUID, IN const DWORD dwUserUID, IN const char cCharType, IN const char cLevel );
    void Init(); // 게임서버 시작시의 초기화 함수 추가.
    void SetBuildCheck( IN const bool bBuildEnable_ ) { m_bBuildEnable = bBuildEnable_; }
    bool GetBuildCheck() { return m_bBuildEnable; }

    void SetGuildClearEnable( IN const bool bEnable_ ) { m_bGuildClearEnable = bEnable_; }
    bool GetGuildClearEnable() { return m_bGuildClearEnable; }


    char DivideChannelGrade( IN const DWORD dwBattlePoint_ );
    DWORD GetGuildBattlePoint( IN const DWORD dwGuildUID_ );
    void UpdateBattlePoint( IN const DWORD dwGuildUID_, IN const int point_ );
    void SetBattlePoint( IN const DWORD dwGuildUID_, IN const DWORD dwPoint_ );

    USHORT GetGuildLv( IN const int& dwPoint );
    float GetGuildBonus( IN const int& nAdvType, IN const USHORT& usGuildLv );
    bool CheckStoreTabByGuildLv( IN const USHORT& usGuildLv, IN const int& nTabID );
    USHORT GetUserContributeGuildLv( IN const int& nContributePoint );

    int GetGuildPointDiff( IN const int type_, IN const bool bWin_ = true );
    void AddGuildPoint( IN const DWORD dwGuildUID_, IN const DWORD dwUserUID_, IN const int type_, IN const bool bWin_ = true );
    void GetGuildPointLevel( OUT std::map<USHORT,int>& kPacket_ ) { kPacket_ = m_kGuildConfig.m_mapGuildLvPoint; }

    void UpdateGuildInfoByLua( IN const DWORD dwGuildUID_ );
    void SendToGuildInfoCenter();

    // 길드 마크 문제 수정 부분 추가
    void GetGuildMarkStringName( IN const DWORD& dwGuildUID, OUT std::wstring& strName );

private:
    void Remove( IN const DWORD dwGuildUId_ ); // 길드삭제시,길드매니저에서 길드 제거
    bool SendModifyGuildNoticeNot( IN const DWORD dwGuildUID, IN std::vector<KNGuildNotice>& vecNotice );
    bool IsGuildPageEmpty();
    bool GetCurrentPage( IN const UCHAR ucSortType, IN const UCHAR ucOrderBy_, IN OUT int& nPageNum, OUT KSerBuffer& kPage, OUT int& nTotalPageNum );
    bool SendDrumoutGuildUserNot( IN const DWORD dwGuildUID, IN std::set<DWORD>& setUID, IN const char cOutType );
    void ChangeMemberLevel( IN const DWORD dwGuildUID, IN const std::map<char,std::set<DWORD> >& mapChangeMemberLevel );
    bool SendChangeGuildMemberLevelNot( IN const DWORD dwGuildUID, IN const std::map<char,std::set<DWORD> >& mapChangeMemberLevel );
    bool SendModifyGuildNameNot( IN const DWORD dwGuildUID, IN std::wstring& strGuildName );
    bool SendChangeGuildURLNot( KNGuildURL& kGuildURL );

    void IntervalTick();

    void ClearGuildInfo();
    void ClearMemberList();

    // Guild Page
    void SetReservePageBuild( IN bool bVal_ ) { KLocker lock( m_csPage ); m_bReservePageBuild = bVal_; }
    bool GetReservePageBuild() { KLocker lock( m_csPage ); return m_bReservePageBuild; }
    void GetGuildPageIDList( OUT std::map<UCHAR,std::vector<DWORD> >& mapData );

    int GetMaxPageSize() { KLocker lock(m_csConfig); return m_kGuildConfig.m_nMaxPageSize; }
    void BuildPageBuffer( IN const int nPageSize, IN const DWORD dwGuildUID, IN OUT int& nPageNum, IN OUT std::vector<KNGuildInfo>& vecPage, IN OUT std::map<int,KSerBuffer>& mapPage );
    void SetGuildPage( IN const std::pair<UCHAR,UCHAR> prKey, IN const std::map<int,KSerBuffer>& mapPage );
    void SetTotalGuildPageNum();
    void BuildPage( IN const UCHAR ucOrderBy, IN const int nPageSize, IN const int nMaxPage, IN std::vector<DWORD> vecGuildUID_, OUT std::map<int,KSerBuffer>& mapPage );

    void AddPageNewGuild( IN const DWORD dwGuildUID );
    void SendOneGuildPageBuildNotice( IN const int nPageNum );
    void GetCurrentPageIDList( IN const int nPageNum, IN const std::vector<DWORD>& vecGuildUID, OUT std::vector<DWORD>& vecData );
    void RemovePageGuild( IN const DWORD dwGuildUID );

    void GetCurrentPageNumOrderByAsc( IN const DWORD dwGuildUID, IN const UCHAR ucSortType, IN const std::vector<DWORD>& vecGuild, OUT std::map< std::pair<UCHAR,UCHAR>, int >& mapUpdatePage );
    void GetCurrentPageNumOrderByDesc( IN const DWORD dwGuildUID, IN const UCHAR ucSortType, IN const std::vector<DWORD>& vecGuild, OUT std::map< std::pair<UCHAR,UCHAR>, int >& mapUpdatePage );
    void UpdateGuildPageInfo( IN const DWORD dwGuildUID );
    bool CompressPage( IN std::vector<KNGuildInfo> vecPage, OUT KSerBuffer& kBuff );
    void ReadPage( IN const KSerBuffer kBuff, OUT std::vector<KNGuildInfo>& vecPage );
    void SendUpdateGuildListReqToDB(); // DB에서 전체 길드 정보 받아오는 함수추가.
    void BuildGuildPageList();
    bool DayCheck();
    void GetGuildUIDList( IN const std::vector<KGuildPtr>& vecGuild_, OUT std::vector<DWORD>& vecGuildUID_ );
    int GetMaxBattlePoint();

protected:
    KGuildManager();
    virtual void ProcessEvent( const KEventPtr& spEvent );

   _DECL_ON_FUNC( DB_EVENT_UPDATE_GUILD_POINT_ACK, KGuildGameResult );

public:
    KManufactureS3                  m_kGuildStoreItems;  // 길드상점

protected:
    mutable KncCriticalSection      m_csMapGuild;
    std::map<DWORD, KGuildPtr>      m_mapGuild;         // 길드 리스트 map<GuildUID,GuildPtr> (=날짜순 정렬)
    std::set<DWORD>                 m_setLiveGuildUIDList; // 게임서버에서 가지고 있는 Guild UID List

    mutable KncCriticalSection                  m_csPage;
    bool                                        m_bReservePageBuild; // 페이지 빌드 Flag
    std::map<UCHAR,std::vector<DWORD> >         m_mapSimplePageInfo; // 센터서버에서 전달.map<SortType, vector<GuildUID>>
    std::map< std::pair<UCHAR,UCHAR>, std::map<int,KSerBuffer> > m_mapGuildPage; // 정렬된 페이지 map< pair<SortType,Orderby>, map<PageNum,Page> >
    int                                         m_nTotalGuildPageNum; // 길드리스트 총 페이지 번호

    // 길드 관련 설정 정보.
    mutable KncCriticalSection      m_csConfig;
    KGuildConfig                    m_kGuildConfig;

    mutable KncCriticalSection      m_csGuildRank;
    std::map< DWORD, KSerBuffer >   m_mapCompressedRankData;  // std::map< DWORD, std::vector<KNGuildRankInfo> >

    // tick
    KncTimer                        m_kGuildClearTimer; // 길드정보 삭제주기
    KncTimer                        m_kMemberClearTimer; // 맴버리스트 삭제주기
    KncTimer                        m_kPageBuildTimer; // 페이지빌드 주기
    KSimpleDate                     m_kLastDayCheckDate; // DayCheck.마지막 검사날짜

    std::vector<KGuildPtr>          m_vecGuild; // 페이지 정보를 위해서 Keep하는 정보.
    bool                            m_bBuildEnable;

    bool                            m_bGuildClearEnable;
};

DefSingletonInline( KGuildManager );
DeclOstmOperatorA( KGuildManager );
