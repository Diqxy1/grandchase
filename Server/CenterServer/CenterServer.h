#pragma once

#include "BaseServer.h"
#include "ChecksumManager.h"

class KCenterServer : public KBaseServer
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclareSingleton( KCenterServer );
    
protected:
    KCenterServer(void);
public:
    virtual ~KCenterServer(void);

    virtual bool Init( const wchar_t* szFileName );
    virtual void RegToLua();
    virtual void Run( bool bRunInterpreter );
    virtual void ShutDown();
    void Tick();

    const int& GetServerListProtocol()const { KLocker lock( m_csServerListProtocol ); return m_nServerListProtocol; };
    void SetServerListProtocol( int nServerListProtocol ) { KLocker lock( m_csServerListProtocol ); m_nServerListProtocol = nServerListProtocol; };

    // 070316. kkurrung. 한시간 이벤트 관련 
    void InitRepetitionInfo();
    void SetRepetitionInfo( const int nMaxRequestCount, const int nServerUserLimit ); // for lua
    void Dump( std::wostream& stm ) const;
    void UpdateRepetitionData( DWORD dwRepetitionUID, int nCommentID );
    void ProcessRepetitionEvent();

    void SetInitEvent() { ::SetEvent( m_hFirstSyncEvent ); };
    
    void SetMiniGameRank( std::map< char, std::vector<KMiniGameRankInfo> >& mapRank );
    void AddMiniGameRank( IN std::map< char, std::vector<KMiniGameRankInfo> > mapRank ); // 서버에서 받은 랭킹정보를 더한다.
    void Clipping( IN DWORD dwSize, IN OUT std::vector<KMiniGameRankInfo>& vecRank );
    void GetMiniGameRank( OUT std::map< char, std::vector<KMiniGameRankInfo> >& mapRank );
    void BuildRankByScore( IN OUT std::vector<KMiniGameRankInfo>& vecRank_ );

    void DumpMiniRank();
    bool AddGameRank( IN OUT std::vector<KMiniGameRankInfo>& vecRank_, IN OUT KMiniGameRankInfo& kInfo_ );
    void DumpRank( std::map< char, std::vector<KMiniGameRankInfo> >& mapRank_ );
    KChecksumManager* GetChecksumManager() { return &m_kChecksumManager; }

    void AddConnectionCount() { KLocker lock( m_csConnectionCount); ++m_dwConnectionCount; };
    void ClearConnectionCount() { KLocker lock( m_csConnectionCount); m_dwConnectionCount = 0; };
    DWORD GetConnectionCount() { KLocker lock( m_csConnectionCount); return m_dwConnectionCount; };


    void UpdateChannelNews( IN const std::vector< std::pair< int, std::wstring > >& vecChannelNews );
    void GetChannelNews( OUT std::vector< std::pair< int, std::wstring > >& vecChannelNews );
    void SetChannelNewsGap( int iGap_ ){ KLocker lock(m_csChannelNews); m_dwChannelNewsGap = iGap_; };
    DWORD GetChannelNewsGap(){ KLocker lock(m_csChannelNews); return m_dwChannelNewsGap; };
    bool CheckScriptChecksum( KODBC& kOdbc, IN const std::map<std::wstring,KScriptCheckInfo>& mapScript, const std::wstring& strLogin, OUT std::vector< std::pair<std::wstring,bool> >& vecFileCheck );

    bool LoadScript();
    bool ReLoadScript();
    void GetSHAFileNameList( OUT std::set<std::wstring>& setSHAFileNameList );

    // 전체 서버 동접 제어 함수.
    void SetTotalServerUser( IN const DWORD dwTotalServerUser_ ) { KLocker lock( m_csTotalServerUser ); m_dwTotalServerUser = dwTotalServerUser_; };
    DWORD GetTotalServerUser() { KLocker lock( m_csTotalServerUser ); return m_dwTotalServerUser; };
    // 유저 문자 보낼 수 Gap 저장.
    void SetUserNumGap( IN const DWORD dwUserNumGap ) { KLocker lock( m_csTotalServerUser ); m_dwUserNumGap = dwUserNumGap; };
    DWORD GetUserNumGap() { KLocker lock( m_csTotalServerUser ); return m_dwUserNumGap; };
    void AddPhoneNumber( IN const char* cPhoneNumber );
    void GetPhoneNumber( OUT std::set< std::wstring >& setPhoneNumber ) { KLocker lock( m_csTotalServerUser ); setPhoneNumber = m_setPhoneNumber; };
    void SetProblemMsg( IN const char* cProblemMsg, IN const char* cCurrentMsg );
    void GetProblemMsg( OUT std::wstring& wstrProblemMsg, OUT std::wstring& wstrCurrentMsg );
    void DumpServerList();
    void SetUniqueUserStat( IN const DWORD dwChannelType, IN const DWORD dwUID );
    void GetUniqueUserStat( OUT std::map< DWORD, std::set<DWORD> >& mapUniqueUserCount );

    DWORD GetLoginStayLimitTime();
    DWORD GetConnectStayLimitTime();

    void SetIP( IN const std::string& strIP );
    std::string GetIP();


private:
    void SetLoginStayLimitTime( IN DWORD dwTime_ ) { KLocker lock ( m_csStayLimitTime ); m_dwLoginStayLimitTime = dwTime_; };
    void SetConnectStayLimitTime( IN DWORD dwTime_ ) { KLocker lock ( m_csStayLimitTime ); m_dwConnectStayLimitTime = dwTime_; };

public:
    KChecksumManager    m_kChecksumManager;
protected:

    // 070316. kkurrung. 한시간 이벤트 관련
    mutable KncCriticalSection  m_csRepetitionEvent;        // 한시간 이벤트 관련 데이터 보호.
    CTime                       m_tmGetEventTime;
    DWORD                       m_dwLastRepetitionUID;      // 마지막 이벤트 고유 아이디.( >0 )
    int                         m_nCommentID;               // 해당 코멘트 ID
    int                         m_nRequestCount;            // 한시간 이벤트 요청 횟수.
    int                         m_nMaxRequestCount;         // 최대 요청 횟수.
    int                         m_nServerUserLimit;         // 서버에 유저가 이정도는 있어야 한다.
    
    // 070316. kkurrung. 한시간 이벤트 관련
    mutable KncCriticalSection  m_csServerListProtocol;
    int                         m_nServerListProtocol;

    // 070319. kkurrung. 서버 초기화시 DB로 부터 필수 데이터 읽어 오기..
    HANDLE                      m_hFirstSyncEvent;
    mutable KncCriticalSection  m_csMiniGameRank;

    // 090118. kkurrung. 유저 접속 카운트 기록 하는것.
    mutable KncCriticalSection  m_csConnectionCount;
    DWORD                       m_dwConnectionCount;
    DWORD                       m_dwConnectionLastTick;
    
    std::map< char, std::vector<KMiniGameRankInfo> > m_mapMiniGameRank;   //  080709  woosh. 데이터 통일중

    KncCriticalSection          m_csChannelNews;
    std::vector< std::pair< int, std::wstring > > m_vecChannelNews; //  090316  woosh. 채널 뉴스
    DWORD                       m_dwChannelNewsGap;
    DWORD                       m_dwChannelNewsTick;

    KncCriticalSection          m_csTotalServerUser;
    DWORD                       m_dwTotalServerUser;    // 100610 tgkwon. 전체 동접수.
    DWORD                       m_dwUserNumGap; // 유저 얼마이상 차이날때 문자를 보낼것인가를 체크.
    std::set< std::wstring >    m_setPhoneNumber; // 관리자 번호 리스트.
    std::wstring                m_wstrProblemMsg; // 문제 내용 리스트.
    std::wstring                m_wstrCurrentMsg; // 동접 내용 리스트.

    //std::vector<KOneadayItem> m_vecOneadayItemList; //원어데이 아이템리스트

    KncCriticalSection          m_csUniqueUser;
    DWORD                       m_dwUniqueUserTick;
    std::map< DWORD, std::set<DWORD> >    m_mapUniqueUserCount; // 유닉 유저 Count.

    std::string m_strIP;

private:

    KncCriticalSection m_csStayLimitTime;
    DWORD m_dwLoginStayLimitTime; // 로그인 상태로 머물 수 있는 최대 시간 2012/1/5 woosh
    DWORD m_dwConnectStayLimitTime; // 접속 상태로 머물 수 있는 최대 시간 2012/1/5 woosh
};

DefSingletonInline( KCenterServer );
