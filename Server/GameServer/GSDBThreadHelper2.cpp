#include <WinSock2.h>
#include "GSDBThread.h"
#include <dbg/dbg.hpp>
#include "GSSimLayer.h"
#include "User.h"
#include "GSDBLayer.h"
#include "EventType.h"
#include "GuildManager.h"
#include <boost/bind.hpp>
#include "ItemManager.h"
#include "VitalityManager.h"
#include <boost/foreach.hpp>
#include "ConnectionGiftBox.h"
#include "DungeonRankManager.h"

#define CLASS_TYPE KGSDBThread

int KGSDBThread::UpdateGuildPoint( IN const DWORD dwUserUID_, 
                                  const DWORD dwGuildUID_, 
                                  const DWORD dwGPoint_, 
                                  const int iWin_, 
                                  const int iLose_ )
{
    // 길드전 전적갱신 Game
    /*
    L"{ call dbo.GSGAGuildSystemScore_merge ( %d, %d, %I64d, %d, %d ) }"

    { call dbo.GSGAGuildSystemScore_merge ( @1, @2, @3, @4, @5 ) }
    @1 ; @iLoginUID_   int
    @2 ; @iNID_        int
    @3 ; @iGPointDiff_ bigint
    @4 ; @iWinDiff_    int
    @5 ; @iLoseDiff_   int

    1 select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; OK            int
    0        ; 성공
    -1       ; 길드정보 없음
    -2       ; 길드원이 아님
    -101이하 ; 트랜잭션 에러
    */
    std::wstring strSP;
    int nOK = -99;
    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemScore_merge ( %d, %d, %I64d, %d, %d ) }",
        dwUserUID_, dwGuildUID_, static_cast<__int64>(dwGPoint_), iWin_, iLose_ ), return nOK );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP >> nOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( iLose_ )
        << BUILD_LOG( strSP )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return nOK;
}

int KGSDBThread::UpdateGuildBattlePoint( IN const DWORD dwGuildUID_, IN const int BPointDiff_ )
{
    /*
    L"{ call dbo.GSGAGuildSystemScoreBPoint_merge ( %d, %d ) }"

    { call dbo.GSGAGuildSystemScoreBPoint_merge ( @1, @2 ) }
    @1 ; @iNID_        int
    @2 ; @iBPointDiff_ bigint

    1 select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; OK            int
    0        ; 성공
    -1       ; 길드정보 없음
    -101이하 ; 트랜잭션 에러
    */
    std::wstring strSP;
    int nOK = -99;
    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemScoreBPoint_merge ( %d, %d ) }",
        dwGuildUID_, BPointDiff_ ), return nOK );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> strSP >> nOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( strSP )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return nOK;
}

int KGSDBThread::UpdateCreateGuild( IN const DWORD dwUserUID_, const std::wstring& strGuildName_, const std::wstring& strGuildURL_, OUT DWORD& dwGuildUID_ )
{
    // 길드생성 Game
    /*
    { call dbo.GSGAGuildSystem_insert ( @1, @2, @3 ) }
    @1 ; @iLoginUID_ int
    @2 ; @strName_   nvarchar( 12  )
    @3 ; @strURL_    nvarchar( 100 )


    1 select ( @1, @2, @3 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; OK            int
    0        ; 성공
    -1       ; 유저정보 없음
    -2       ; 길드이름 중에 특수문자가 있음
    -3       ; 길드이름 중복
    -4       ; URL 중복
    -5       ; 이미 가입유저임
    -101이하 ; 트랜잭션 에러
    @3  ; NID           int
    OK 값이 0일 경우에만 신뢰할 것
    */
    std::wstring strSP;
    int iOK = -99;
    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystem_insert ( %d, N'%s', N'%s' ) }",
        dwUserUID_, strGuildName_.c_str(), strGuildURL_.c_str() ), return iOK );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> strSP >> iOK >> dwGuildUID_ );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( iOK == 0 )
        << BUILD_LOG( iOK )
        << BUILD_LOG( strSP )
        << BUILD_LOG( dwGuildUID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return iOK;
}

bool KGSDBThread::GetGuildCommentNoticeFromDB( IN const DWORD dwGuildUID_, OUT std::vector<KNGuildNotice>& vecNotice_ )
{
    // 1개 길드의 소개 및 공지 받아오기
    /*
    { call dbo.GSGAGuildSystemNotice_select_1_guild ( @1 ) }
    @1 ; @iNID_ int

    3 select ( @1, @2, @3, @4 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; NID           int
    @3  ; No            tinyint
    1 ; 길드 소개
    2 ; 길드 공지1
    3 ; 길드 공지2
    @4  ; Content       nvarchar( 100  )
    */
    if ( dwGuildUID_ == 0 ) return false;

    std::wstring strSP;
    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemNotice_select_1_guild ( %d ) }",
        dwGuildUID_ ), return false );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        DWORD dwGuildUID = 0;
        KNGuildNotice kTemp;
        kTemp.m_nMsgID = -1;

        VERIFY_QUERY( m_kODBC >> strSP
            >> dwGuildUID
            >> kTemp.m_nMsgID
            >> kTemp.m_strMsg );

        LOG_SUCCESS( dwGuildUID_ == dwGuildUID )
            << BUILD_LOG( strSP )
            << BUILD_LOG( dwGuildUID_ )
            << BUILD_LOG( dwGuildUID )
            << BUILD_LOG( kTemp.m_nMsgID )
            << BUILD_LOG( kTemp.m_strMsg ) << END_LOG;

        vecNotice_.push_back( kTemp );
    }

    START_LOG( clog, L"GuildID : " << dwGuildUID_ << L", vecNotice_.size() : " << vecNotice_.size() ) << END_LOG;
    return true;
}

bool KGSDBThread::GetGuildMemberListFromDB( IN const DWORD dwGuildUID_, OUT std::map<DWORD, KNGuildUserInfo>& mapUserList_ )
{
    // 길드 맴버 리스트 불러오기
    /*  
    20091109. microcat.  
    */
    /*  
    L"{ call dbo.GSGAGuildSystemMember_select_1_guild ( %d ) }"  

    { call dbo.GSGAGuildSystemMember_select_1_guild ( @1 ) }  
    @1 ; @iNID_ int  

    n select ( @1, @2 )  
    @1  ; 호출된 sp이름 nvarchar( 4000 )  
    @2  ; LoginUID      int  
    */
    if ( dwGuildUID_ == 0 ) return false;

    std::wstring strSP;
    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemMember_select_1_guild_color ( %d ) }",
        dwGuildUID_ ), return false );

    std::set<DWORD> setUserList;
    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        DWORD dwUserUID = 0;
        VERIFY_QUERY( m_kODBC >> strSP
                              >> dwUserUID );
        setUserList.insert( dwUserUID );
    }

    if( !setUserList.empty() ) {
        std::set<DWORD>::iterator sit;
        for( sit = setUserList.begin(); sit != setUserList.end(); ++sit ) {
            KNGuildUserInfo kTemp;
            time_t tmJoinDate = time_t(NULL);
            time_t tmLastLogin = time_t(NULL);

            // 길드멤버의 UserUID리스트를 가져와서 해당 유저 UID를 통해서 유저길드정보를 가져온다.
            // DB에서 기존의 방식의 경우 랜덤 액세스가 일어나서 성능상의 이슈로 인해서 수정.
            GetGuildMemeberInfoFromDB( dwGuildUID_, *sit, kTemp, tmJoinDate, tmLastLogin );

            if ( kTemp.m_dwGuildUID == 0 || kTemp.m_dwUserUID == 0 ) {
                continue;
            }

            // 가입신청일
            CTime tJoinDate( tmJoinDate );
            kTemp.m_kJoinDate.SetDate( tJoinDate.GetYear(), tJoinDate.GetMonth(), tJoinDate.GetDay(), 0 );
            // 마지막접속일
            CTime tLastLogin( tmLastLogin );
            kTemp.m_kLastLoginDate.SetDate( tLastLogin.GetYear(), tLastLogin.GetMonth(), tLastLogin.GetDay(), 0 );

            _LOG_CONDITION( dwGuildUID_ == kTemp.m_dwGuildUID, clog, cwarn, L"GuildUID : " << dwGuildUID_ )
                << BUILD_LOG( kTemp.m_dwGuildUID )
                << BUILD_LOG( kTemp.m_dwUserUID )
                << BUILD_LOG( kTemp.m_kJoinDate.Str() )
                << BUILD_LOGc( kTemp.m_cMemberLevel )
                << BUILD_LOG( kTemp.m_strMyComment )
                << BUILD_LOG( kTemp.m_dwContributePoint )
                << BUILD_LOG( kTemp.m_strLogin )
                << BUILD_LOG( kTemp.m_strNickName )
                << BUILD_LOG( kTemp.m_strNickColor )
                << BUILD_LOG( kTemp.m_kLastLoginDate.Str() ) << END_LOG;

            mapUserList_[ kTemp.m_dwUserUID ] = kTemp;
        }
    }

    START_LOG( clog, L"GuildID : " << dwGuildUID_ << L", mapUserList_.size() : " << mapUserList_.size() ) << END_LOG;
    return true;
}

int KGSDBThread::UpdateGuildMark( IN const DWORD dwUserUID_, const DWORD dwGuildUID_ )
{
    // 길드마크 등록 Game
    /*
    { call dbo.GSGAGuildSystem_update_Mark_Register ( @1, @2 ) }
    @1 ; @iLoginUID_ int
    @2 ; @iNID_      int

    1 select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; OK            int
    0        ; 성공
    -1       ; 유저정보 없음
    -2       ; 길드정보 없음
    -3       ; 길드마크가 정상상태가 아님
    -4       ; 길드마스터가 아님
    -101이하 ; 트랜잭션 에러
    */
    std::wstring strSP;
    int iOK = -99;

    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystem_update_Mark_Register ( %d, %d ) }",
        dwUserUID_, dwGuildUID_ ), return iOK );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP >> iOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( iOK == 0 )
        << BUILD_LOG( iOK )
        << BUILD_LOG( strSP ) << END_LOG;

    return iOK;
}

int KGSDBThread::UpdateGuildName( IN const DWORD dwUserUID_, const DWORD dwGuildUID_, const std::wstring strGuildName_ )
{
    // 길드이름 변경 Game
    /*
    { call dbo.GSGAGuildSystem_update_Name ( @1, @2, @3 ) }
    @1 ; @iLoginUID_ int
    @2 ; @iNID_      int
    @3 ; @strName_   nvarchar( 12 )

    1 select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; OK            int
    0        ; 성공
    -1       ; 유저정보 없음
    -2       ; 길드정보 없음
    -3       ; 길드마스터가 아님
    -4       ; 중복된 길드이름
    -101이하 ; 트랜잭션 에러
    */
    std::wstring strSP;
    int iOK = -99;

    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystem_update_Name ( %d, %d, N'%s' ) }",
        dwUserUID_, dwGuildUID_, strGuildName_.c_str() ), return iOK );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP >> iOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( iOK == 0 )
        << BUILD_LOG( iOK )
        << BUILD_LOG( strSP ) << END_LOG;

    return iOK;
}

int KGSDBThread::UpdateGuildURL( IN const DWORD dwUserUID_, const DWORD dwGuildUID_, const std::wstring strURL_ )
{
    // 길드URL 변경 Game
    /*
    { call dbo.GSGAGuildSystem_update_URL ( @1, @2, @3 ) }
    @1 ; @iLoginUID_ int
    @2 ; @iNID_      int
    @3 ; @strName_   nvarchar( 12 )

    1 select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; OK            int
    0        ; 성공
    -1       ; 유저정보 없음
    -2       ; 길드정보 없음
    -3       ; 길드마스터가 아님
    -101이하 ; 트랜잭션 에러
    */
    std::wstring strSP;
    int iOK = -99;

    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystem_update_URL ( %d, %d, N'%s' ) }",
        dwUserUID_, dwGuildUID_, strURL_.c_str() ), return iOK );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP >> iOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( iOK == 0 )
        << BUILD_LOG( iOK )
        << BUILD_LOG( strSP ) << END_LOG;

    return iOK;
}

int KGSDBThread::UpdateGuildCommentNoice( IN const DWORD dwUserUID_, const DWORD dwGuildUID_, const int iMsgID_, const std::wstring strMsg_ )
{
    // 길드 소개 및 공지 변경 Game
    /*
    { call dbo.GSGAGuildSystemNotice_update_Contents ( @1, @2, @3, @4 ) }
    @1 ; @iLoginUID_   int
    @2 ; @iNID_        int
    @3 ; @iNo_         tinyint
    @3 ; @strContents_ nvarchar( 100 )

    1 select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; OK            int
    0        ; 성공
    -1       ; 유저정보 없음
    -2       ; 길드정보 없음
    -3       ; 길드마스터 또는 제너럴이 아님
    -4       ; 길드 Notice 정보 없음(DBA 호출)
    -101이하 ; 트랜잭션 에러
    */
    std::wstring strSP;
    int iOK = -99;

    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemNotice_update_Contents ( %d, %d, %d, N'%s' ) }",
        dwUserUID_, dwGuildUID_, iMsgID_, strMsg_.c_str() ), return iOK );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP >> iOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( iOK == 0 )
        << BUILD_LOG( iOK )
        << BUILD_LOG( strSP ) << END_LOG;

    return iOK;
}

int KGSDBThread::UpdateMyGuildComment( IN const DWORD dwUserUID_, const DWORD dwGuildUID_, const std::wstring strComment_ )
{
    // 자기소개 변경 Game
    /*
    { call dbo.GSGAGuildSystemMember_update_Contents ( @1, @2, @3 ) }
    @1 ; @iLoginUID_   int
    @2 ; @iNID_        int
    @3 ; @strContents_ nvarchar( 100 )

    1 select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; OK            int
    0        ; 성공
    -1       ; 유저정보 없음
    -2       ; 길드정보 없음
    -3       ; 길드원이 아님
    -101이하 ; 트랜잭션 에러
    */
    std::wstring strSP;
    int iOK = -99;

    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemMember_update_Contents ( %d, %d, N'%s' ) }",
        dwUserUID_, dwGuildUID_, strComment_.c_str() ), return iOK );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP >> iOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( iOK == 0 )
        << BUILD_LOG( iOK )
        << BUILD_LOG( strSP ) << END_LOG;

    return iOK;
}

bool KGSDBThread::SearchGuildListFromDB( IN const std::wstring strKeyword_, OUT std::vector<DWORD>& vecGuildUIDList_ )
{
    //  길드명을 넣으면 해당 단어를 포함한 길드NID가 리턴됩니다
    // 시간적 여유가 생기면 서버랑 클라랑 검사할것
    // 단어를 입력받을 때 포함되어서는 안되는 문자는 다음과 같습니다
    // % (퍼센트)
    // _ (언더바)
    // [ (왼쪽대괄호)
    // ] (오른쪽대괄호)
    // ^ (승수)
    /*
    { call dbo.GSGAGuildSystem_select_search_name ( @1 ) }
    @1 ; @strName_ nvarchar( 12 )

    n select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; NID           int
    */
    std::wstring strSP;
    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystem_select_search_name ( N'%s' ) }",
        strKeyword_.c_str() ), return false );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        DWORD dwGuildUID = 0;

        VERIFY_QUERY( m_kODBC >> strSP >> dwGuildUID );

        vecGuildUIDList_.push_back( dwGuildUID );
    }

    START_LOG( clog, strSP )
        << BUILD_LOG( vecGuildUIDList_.size() ) << END_LOG;
    return true;
}

bool KGSDBThread::GetGuildMarkInfo( IN const DWORD dwGuildUID_, OUT int& nStat_, OUT int& nRevision_, OUT std::wstring& strFileName_ )
{
    if ( dwGuildUID_ == 0 ) return false;

    nStat_ = -1;
    nRevision_ = -1;
    strFileName_.clear();

    //L"{ call dbo.GSGAGuildSystem_select_1_guild ( %d ) }"
    //
    //{ call dbo.GSGAGuildSystem_select_1_guild ( @1 ) }
    //@1 ; @iNID_ int
    //
    //1 select ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10, @11, @12, @13 )
    //@1  ; 호출된 sp이름 nvarchar( 4000 )
    //@2  ; NID           int
    //@3  ; RegDateA      datetime
    //@4  ; Kind          tinyint
    //1 ; 신생길드
    //2 ; 정식길드
    //@5  ; JoinType      tinyint
    //1 ; 즉시가입
    //2 ; 가입후승인
    //3 ; 가입중지
    //@6  ; MarkStatus    tinyint
    //1 ; 정상상태
    //2 ; 마크변경신청상태
    //3 ; 승인성공
    //4 ; 승인실패
    //@7  ; MarkRevision  int
    //@8  ; MarkExtention int
    //@9  ; GExp          bigint
    //@10 ; GPoint        bigint
    //@11 ; Name          nvarchar( 12  )
    //@12 ; URL           nvarchar( 100 )
    //@13 ; MemberCount   tinyint
    std::wstring strDummy;
    std::wstring strExt;

    DBJIF2( m_kODBC.Query( L"{ call dbo.GSGAGuildSystem_select_1_guild ( %d ) }", dwGuildUID_ ) );
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> strDummy
            >> strDummy
            >> strDummy
            >> strDummy
            >> strDummy
            >> nStat_ >> nRevision_ >> strExt );
        SiKGuildManager()->MakeGuildMarkName( dwGuildUID_, nRevision_, strExt, strFileName_ );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L" Guild Mark Info UID : " << dwGuildUID_
        << L", Stat : " << nStat_
        << L", FileName : " << strFileName_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (nStat_ != -1 && nRevision_ != -1 && !strFileName_.empty());
}

bool KGSDBThread::SetGuildMarkRegisted( IN const DWORD dwUID_, IN const DWORD dwGuildUID_, IN const std::wstring& strExt_, IN const bool bCashItem_ )
{
    /*
    { call dbo.GSGAGuildSystem_update_Mark_Register ( @1, @2, @3, @4 ) }
    @1 ; @iLoginUID_            int
    @2 ; @iNID_                 int
    @3 ; @strMarkExtensionTemp_ nvarchar( 5 )
    @4 ; @bCashItemUse_         bit           = 0
    캐쉬로 구매한 마크 변경권 사용시 1

    1 select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; OK            int
    0        ; 성공
    -1       ; 유저정보 없음
    -2       ; 길드정보 없음
    -3       ; 길드마크가 정상상태가 아님
    -4       ; 길드마스터가 아님
    -101이하 ; 트랜잭션 에러
    */
    if ( dwGuildUID_ == 0 ) return false;

    DBJIF2( m_kODBC.Query( L"{ call dbo.GSGAGuildSystem_update_Mark_Register ( %d, %d, N'%s', %d ) }",
            dwUID_, dwGuildUID_, strExt_.c_str(), bCashItem_ ) );
    int nOK = -99;
    std::wstring strSP;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> strSP 
            >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L" Guild Mark Registed UID : " << dwGuildUID_ << L", OK : " << nOK )
        << BUILD_LOG( strSP )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (nOK == 0);
}

bool KGSDBThread::GetGuildMasterInfoFromDB( IN const DWORD dwGuildUID_, OUT KNGuildUserInfo& kInfo_ )
{
    // 길드마스터 정보 가져오기
    /*
    { call dbo.GSGAGuildSystemMember_select_1_guildmaster ( @1 ) }
    @1 ; @iNID_ int

    1 select ( @1, @2, @3, @4, @5, @6, @7, @8, @9 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; NID           int
    @3  ; LoginUID      int
    @4  ; RegDateA      datetime
    @5  ; Grade         tinyint
    0 ; 가입 대기 유저
    1 ; 일반 길드원
    3 ; 제너럴
    5 ; 길드마스터
    @6  ; Contents      nvarchar( 100  )
    @7  ; GPoint        bigint
    @8  ; Login         nvarchar( 20   )
    @9  ; Nickname      nvarchar( 24   )
    @10 ; LastLogin     smalldatetime
    */
    if ( dwGuildUID_ == 0 ) return false;

    std::wstring strSP;
    kInfo_.Init();
    time_t tmJoinDate = time_t(NULL);
    time_t tmLastLogin = time_t(NULL);

    DBJIF2( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemMember_select_1_guildmaster_color ( %d ) }",
        dwGuildUID_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP
            >> kInfo_.m_dwGuildUID
            >> kInfo_.m_dwUserUID
            >> tmJoinDate
            >> kInfo_.m_cMemberLevel
            >> kInfo_.m_strMyComment
            >> kInfo_.m_dwContributePoint
            >> kInfo_.m_strLogin
            >> kInfo_.m_strNickName
            >> kInfo_.m_strNickColor
            >> tmLastLogin );
    }
    m_kODBC.EndFetch();

    // 가입신청일
    CTime tJoinDate( tmJoinDate );
    kInfo_.m_kJoinDate.SetDate( tJoinDate.GetYear(), tJoinDate.GetMonth(), tJoinDate.GetDay(), 0 );
    // 마지막접속일
    CTime tLastLogin( tmLastLogin );
    kInfo_.m_kLastLoginDate.SetDate( tLastLogin.GetYear(), tLastLogin.GetMonth(), tLastLogin.GetDay(), 0 );

    LOG_CONDITION( kInfo_.m_cMemberLevel == KNGuildUserInfo::GL_MASTER, clog, cwarn )
        << BUILD_LOG( strSP )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( kInfo_.m_dwGuildUID )
        << BUILD_LOG( kInfo_.m_dwUserUID )
        << BUILD_LOG( kInfo_.m_kJoinDate.Str() )
        << BUILD_LOGc( kInfo_.m_cMemberLevel )
        << BUILD_LOG( kInfo_.m_strMyComment )
        << BUILD_LOG( kInfo_.m_dwContributePoint )
        << BUILD_LOG( kInfo_.m_strLogin )
        << BUILD_LOG( kInfo_.m_strNickName )
        << BUILD_LOG( kInfo_.m_strNickColor )
        << BUILD_LOG( kInfo_.m_kLastLoginDate.Str() ) << END_LOG;

    return true;
}

DWORD KGSDBThread::GetGuildUIDFromDB( IN const DWORD dwUserUID_ )
{
    /*
    { call dbo.GSGAGuildSystemMember_select_check_member ( @1 ) }
    @1 ; @iLoginUID_ int

    1 select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; NID           int
    */
    std::wstring strSP;
    DWORD dwGuildUID = 0;

    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemMember_select_check_member ( %d ) }",
        dwUserUID_ ), return dwGuildUID );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP >> dwGuildUID );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"DB에서 유저의 길드ID 가지고 오기." )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOG( dwGuildUID )
        << BUILD_LOG( strSP ) << END_LOG;

    return dwGuildUID;
}

bool KGSDBThread::GetGuildInfoFromDB( IN const DWORD dwGuildUID_, OUT KNGuildInfo& kInfo_ )
{
    // 1개길드 길드정보 불러오기
    /*
    { call dbo.GSGAGuildSystem_select_1_guild ( @1 ) }
    @1 ; @iNID_ int

    n select ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10, @11, @12, @13, @14 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; NID           int
    @3  ; RegDateA      datetime
    @4  ; Kind          tinyint
    1 ; 신생길드
    2 ; 정식길드
    @5  ; JoinType      tinyint
    1 ; 즉시가입
    2 ; 가입후승인
    3 ; 가입중지
    @6  ; MarkStatus    tinyint
    1 ; 정상상태
    2 ; 마크변경신청상태
    3 ; 승인성공
    4 ; 승인실패
    @7  ; MarkRevision  int
    @8  ; MarkExtention nvarchar
    @9  ; GExp          bigint
    @10 ; GPoint        bigint
    @11 ; Name          nvarchar( 12  )
    @12 ; URL           nvarchar( 100 )
    @13 ; MemberCount   tinyint
    @14 ; BattlePoint   bigint
    */
    std::wstring strSP;
    std::wstring strExt;
    time_t tRegDate = time_t(NULL);

    if ( dwGuildUID_ == 0 ) return false;

    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystem_select_1_guild ( %d ) }",
        dwGuildUID_ ), return false );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP
            >> kInfo_.m_dwUID
            >> tRegDate
            >> kInfo_.m_ucGrade
            >> kInfo_.m_ucJoinSetting
            >> kInfo_.m_ucMarkStatus
            >> kInfo_.m_nMarkRevision
            >> strExt
            >> kInfo_.m_dwExp
            >> kInfo_.m_dwPoint
            >> kInfo_.m_strName
            >> kInfo_.m_strURL
            >> kInfo_.m_nNumMembers
            >> kInfo_.m_GuildBattlePoint );

        SiKGuildManager()->MakeGuildMarkName( dwGuildUID_, kInfo_.m_nMarkRevision, strExt, kInfo_.m_strFileName );
    }
    m_kODBC.EndFetch();

    CTime tDate( tRegDate );
    KSimpleDate kRegDate( tDate.GetYear(), tDate.GetMonth(), tDate.GetDay(), 0 );
    kInfo_.m_kFoundingDate = kRegDate;

    int nOfficailGuildMemberNum = SiKGuildManager()->GetOfficailGuildMemberNumber();
    if ( kInfo_.m_ucGrade == KNGuildInfo::GG_NEWBIE && kInfo_.m_nNumMembers >= nOfficailGuildMemberNum ) {
        // 신생길드가 정규길드 맴버수를 넘으면 정규길드로 전환
        kInfo_.m_ucGrade = KNGuildInfo::GG_OFFICIAL;

        int nRet = UpdateGuildGrade( dwGuildUID_ );
        _LOG_SUCCESS( nRet == 0, L"신규길드 정식길드로 변환. nRet : " << nRet )
            << BUILD_LOG( dwGuildUID_ ) << END_LOG;
    }

    _LOG_CONDITION( dwGuildUID_ == kInfo_.m_dwUID, clog, cwarn, L"GuildInfo.. Guild UID : " << kInfo_.m_dwUID << L" (" << dwGuildUID_ << L")" )
        << BUILD_LOGtm( CTime(tRegDate) )
        << BUILD_LOGc( kInfo_.m_ucGrade )
        << BUILD_LOGc( kInfo_.m_ucJoinSetting )
        << BUILD_LOGc( kInfo_.m_ucMarkStatus )
        << BUILD_LOG( kInfo_.m_nMarkRevision )
        << BUILD_LOG( kInfo_.m_dwExp )
        << BUILD_LOG( kInfo_.m_dwPoint )
        << BUILD_LOG( kInfo_.m_strName )
        << BUILD_LOG( kInfo_.m_strURL )
        << BUILD_LOG( kInfo_.m_nNumMembers )
        << BUILD_LOG( kInfo_.m_GuildBattlePoint ) << END_LOG;

    return true;
}


bool KGSDBThread::GetAllGuildInfo( OUT std::map< DWORD, KNGuildInfo >& mapGuildList_ )
{
    // 전체 길드 정보 받아오기 center
    /*
    L"{ call dbo.GSGAGuildSystem_select_all_guild }"

    { call dbo.GSGAGuildSystem_select_all_guild }

    n select ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10, @11, @12, @13 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; NID           int
    @3  ; RegDateA      datetime
    @4  ; Kind          tinyint
    1 ; 신생길드
    2 ; 정식길드
    @5  ; JoinType      tinyint
    1 ; 즉시가입
    2 ; 가입후승인
    3 ; 가입중지
    @6  ; MarkStatus    tinyint
    1 ; 정상상태
    2 ; 마크변경신청상태
    3 ; 승인성공
    4 ; 승인실패
    @7  ; MarkRevision  int
    @8  ; MarkExtention nvarchar
    @9  ; GExp          bigint
    @10 ; GPoint        bigint
    @11 ; Name          nvarchar( 12  )
    @12 ; URL           nvarchar( 100 )
    @13 ; MemberCount   tinyint
    @14 : BattlePoint   bigint
    */
    std::wstring strSP;
    std::wstring strExt;
    KNGuildInfo kTemp;
    DBJIF2( m_kODBC.Query( L"{ call dbo.GSGAGuildSystem_select_all_guild }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        time_t tRegDate;
        kTemp.Init();

        VERIFY_QUERY( m_kODBC >> strSP
            >> kTemp.m_dwUID
            >> tRegDate
            >> kTemp.m_ucGrade
            >> kTemp.m_ucJoinSetting
            >> kTemp.m_ucMarkStatus
            >> kTemp.m_nMarkRevision
            >> strExt
            >> kTemp.m_dwExp
            >> kTemp.m_dwPoint
            >> kTemp.m_strName
            >> kTemp.m_strURL
            >> kTemp.m_nNumMembers
            >> kTemp.m_GuildBattlePoint );
        SiKGuildManager()->MakeGuildMarkName( kTemp.m_dwUID, kTemp.m_nMarkRevision, strExt, kTemp.m_strFileName );

        CTime tDate( tRegDate );
        KSimpleDate kRegDate( tDate.GetYear(), tDate.GetMonth(), tDate.GetDay(), 0 );
        kTemp.m_kFoundingDate = kRegDate;

        START_LOG( clog, strSP )
            << BUILD_LOG( kTemp.m_dwUID )
            << BUILD_LOGtm( CTime(tRegDate) )
            << BUILD_LOGc( kTemp.m_ucGrade )
            << BUILD_LOGc( kTemp.m_ucJoinSetting )
            << BUILD_LOGc( kTemp.m_ucMarkStatus )
            << BUILD_LOG( kTemp.m_nMarkRevision )
            << BUILD_LOG( kTemp.m_dwExp )
            << BUILD_LOG( kTemp.m_dwPoint )
            << BUILD_LOG( kTemp.m_strName )
            << BUILD_LOG( kTemp.m_strURL )
            << BUILD_LOG( kTemp.m_nNumMembers )
            << BUILD_LOG( kTemp.m_GuildBattlePoint ) << END_LOG;

        mapGuildList_.insert( std::make_pair(kTemp.m_dwUID, kTemp) );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"vecGuildList_.size() : " << mapGuildList_.size() ) << END_LOG;
    return true;
}

bool KGSDBThread::GetAllGuildNotice( OUT std::map< DWORD, std::vector<KNGuildNotice> >& mapGuildNotice_ )
{
    // 전체 길드 공지 center
    /*
    { call dbo.GSGAGuildSystemNotice_select_all_guild }

    n select ( @1, @2, @3, @4 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; NID           int
    @3  ; No            tinyint
    1 ; 길드 소개
    2 ; 길드 공지1
    3 ; 길드 공지2
    @4  ; Content       nvarchar( 100  )
    */
    std::wstring strSP;
    DBJIF2( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemNotice_select_all_guild }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KNGuildNotice kTemp;
        kTemp.m_nMsgID = -1;
        kTemp.m_strMsg.clear();
        int iGuildID = -1;

        VERIFY_QUERY( m_kODBC >> strSP
            >> iGuildID
            >> kTemp.m_nMsgID
            >> kTemp.m_strMsg );

        START_LOG( clog, L"[" << iGuildID << L"]" << L"[" << kTemp.m_nMsgID << L"] " <<  kTemp.m_strMsg  ) << END_LOG;

        mapGuildNotice_[iGuildID].push_back( kTemp );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"mapGuildNotice_.size() : " << mapGuildNotice_.size() ) << END_LOG;
    return true;
}

void KGSDBThread::GetAllGuildRankData( OUT std::map<DWORD,KSimpleGuildRank>& mapSimpleRank_ )
{
    // 길드랭킹 받아오기, 일/주/월/전체 랭킹이 한번에 리턴 Center
    /*
    { call dbo.GSGAGuildSystemRank_select_100_guild }

    500 return ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10 )
    @1 ; Type           tinyint
    서버 ; 디비 ; 종류
    0    ; 1    ; GPoint일일랭킹
    1    ; 2    ; GPoint주간랭킹
    2    ; 3    ; GPoint월간랭킹
    3    ; 4    ; GPoint전체랭킹
    4    ; 5    ; BattlePoint랭킹
    @2  ; Rank          int
    @3  ; NID           int
    @4  ; Name          nvarchar( 12 )
    @5  ; GPoint        bigint
    @6  ; Win           int
    @7  ; Lose          int
    @8  ; Ratio         float
    @9  ; MarkRevision  int
    @10 ; MarkExtension tinyint
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemRank_select_100_guild }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KSimpleGuildRank kTemp;

        int nRankType;
        int nRank;
        DWORD dwUID;
        VERIFY_QUERY( m_kODBC >> nRankType >> nRank >> dwUID );

        switch( nRankType )
        {
        case KNGuildRankInfo::WEEKLY_RANK:
            mapSimpleRank_[dwUID].m_nWeeklyRank = nRank;
            break;
        case KNGuildRankInfo::MONTHLY_RANK:
            mapSimpleRank_[dwUID].m_nMonthlyRank = nRank;
            break;
        case KNGuildRankInfo::TOTAL_RANK:
            mapSimpleRank_[dwUID].m_nTotalRank = nRank;
            break;
        case KNGuildRankInfo::GUILD_BATTLE_RANK:
            mapSimpleRank_[dwUID].m_nGuildBattleRank = nRank;
            break;

        default:
            break;
        }
    }
    m_kODBC.EndFetch();
}

//void KGSDBThread::GetFullGuildInfo()
//{
//    std::map<DWORD,KSimpleGuildRank> mapSimpleRank;
//    std::map< DWORD, KNGuildInfo > mapGuildList;
//    std::map< DWORD, KNGuildInfo >::iterator mit;
//    // 길드 랭킹 얻어 오기.
//    GetAllGuildRankData( mapSimpleRank );
//    // 길드 정보 얻어 오기
//    GetAllGuildInfo( mapGuildList );
//
//    for ( mit = mapGuildList.begin() ; mit != mapGuildList.end() ; ++mit ) {
//        KNGuildUserInfo kInfo;
//
//        // 얻어온 길드 정보에 길마 정보 넣기.
//        if ( GetGuildMasterInfoFromDB( mit->second.m_dwUID, kInfo ) ==true ) {
//            mit->second.m_strMasterNick = kInfo.m_strNickName;
//        }
//
//        // 얻어온 길드 정보에 랭킹 정보 넣기.
//        std::map<DWORD,KSimpleGuildRank>::iterator mitRank;
//        mitRank = mapSimpleRank.find( mit->second.m_dwUID );
//        if ( mitRank != mapSimpleRank.end() ) {
//            mit->second.m_nWeeklyRank = mitRank->second.m_nWeeklyRank;
//            mit->second.m_nMonthlyRank = mitRank->second.m_nMonthlyRank;
//            mit->second.m_nTotalRank = mitRank->second.m_nTotalRank;
//        }
//    }
//
//    SiKGuildManager()->AddGuild( mapGuildList );
//    START_LOG( cerr, L" 서버 시작시 길드 정보 가져옴.. Size : " << mapGuildList.size() ) << END_LOG;
//
//    // 길드 공지 얻어 오기
//    std::map< DWORD, std::vector<KNGuildNotice> > mapGuildNotice;
//    GetAllGuildNotice( mapGuildNotice );
//    std::map< DWORD, std::vector<KNGuildNotice> >::iterator mitNotice;
//    for ( mitNotice = mapGuildNotice.begin() ; mitNotice != mapGuildNotice.end() ; ++mitNotice ) {
//        LIF( SiKGuildManager()->SetGuildNotice( mitNotice->first, mitNotice->second ) );
//    }
//    START_LOG( cerr, L" 서버 시작시 길드 공지 정보 가져옴.. Size : " << mapGuildNotice.size() ) << END_LOG;
//}

void KGSDBThread::GetUserGuildInfo( IN const DWORD dwUserUID_, OUT KNGuildUserInfo& kUserGuildInfo_ )
{
    kUserGuildInfo_.Init();

    // 길드UID 얻기
    DWORD dwGuildUID = GetGuildUIDFromDB( dwUserUID_ );

    if ( dwGuildUID == 0 ) {
        START_LOG( clog, L"소속 길드가 없음. UserUID : " << dwUserUID_ ) << END_LOG;
        return;
    }

    // 여기서부터 길드원.
    UpdateOneGuildInfo( dwGuildUID );
    UpdateOneGuildMemberList( dwGuildUID );
    KGuildPtr spGuild = SiKGuildManager()->GetGuild( dwGuildUID );

    JIF( spGuild ); // 위에서 없으면 무조건 생성하지만 안전코드.

    // 길드 정보는 있는데, 길드맴버 정보는 없는 경우.
    if ( spGuild->GetGuildMemberListSize() == 0 ) {
        UpdateOneGuildMemberList( dwGuildUID );
    }

    // 내 길드맴버 정보 받아오기.
    if ( spGuild->GetNGuildUserInfo( dwUserUID_, kUserGuildInfo_ ) == false ) {
        kUserGuildInfo_.Init(); // 초기화
        START_LOG( cerr, L"길드매니저에서 내 길드정보 받아오기 실패. GuildUID : " << dwGuildUID ) << END_LOG;
    }

    START_LOG( clog, L"내 길드원 정보.. Login : " << kUserGuildInfo_.m_strLogin << L",UserUID : " << kUserGuildInfo_.m_dwUserUID )
        << BUILD_LOG( kUserGuildInfo_.m_strNickName )
        << BUILD_LOG( kUserGuildInfo_.m_strNickColor )
        << BUILD_LOG( kUserGuildInfo_.m_dwGuildUID )
        << BUILD_LOGc( kUserGuildInfo_.m_cMemberLevel )
        << BUILD_LOGc( kUserGuildInfo_.m_ucCharacterGrade )
        << BUILD_LOG( kUserGuildInfo_.m_dwContributePoint )
        << BUILD_LOG( kUserGuildInfo_.m_nWinCount )
        << BUILD_LOG( kUserGuildInfo_.m_nLoseCount )
        << BUILD_LOG( kUserGuildInfo_.m_strMyComment )
        << BUILD_LOG( kUserGuildInfo_.m_bIsConnect )
        << BUILD_LOGc( kUserGuildInfo_.m_cServerUID )
        << BUILD_LOG( kUserGuildInfo_.m_strLocation ) << END_LOG;
}

void KGSDBThread::UpdateOneGuildInfo( IN const DWORD dwGuildUID_ )
{
    if ( dwGuildUID_ == 0 ) return;

    KNGuildInfo kGuildInfo;
    std::vector<KNGuildNotice> vecNotice;
    KNGuildUserInfo kMasterInfo;
    kGuildInfo.Init();
    kMasterInfo.Init();

    // 길드 정보 받아오기
    JIF( GetGuildInfoFromDB( dwGuildUID_, kGuildInfo ) ); // 길드정보
    LIF( GetGuildCommentNoticeFromDB( dwGuildUID_, vecNotice ) ); // 공지
    LIF( GetGuildMasterInfoFromDB( dwGuildUID_, kMasterInfo ) ); // 길드마스터

    // 마스터 설정.
    kGuildInfo.m_strMasterNick = kMasterInfo.m_strNickName;
    kGuildInfo.m_strMasterNickColor = kMasterInfo.m_strNickColor;

    // 소개말 설정
    std::vector<KNGuildNotice>::iterator vit;
    vit = std::find_if( vecNotice.begin(), vecNotice.end(),
        boost::bind(&KNGuildNotice::m_nMsgID,_1) == KNGuildNotice::GN_COMMENT );
    if ( vit != vecNotice.end() ) {
        kGuildInfo.m_strComment = vit->m_strMsg;
    }

    SiKGuildManager()->Add( kGuildInfo ); // 길드 등록
    SiKGuildManager()->SetGuildNotice( dwGuildUID_, vecNotice ); // 공지

    START_LOG( clog, L"DB에서 길드정보 받아와서 등록. GuildUID : " << dwGuildUID_ << L",vecNotice.size() : " << vecNotice.size() ) << END_LOG;
}

void KGSDBThread::UpdateOneGuildMemberList( IN const DWORD dwGuildUID_ )
{
    if ( dwGuildUID_ == 0 ) return;

    std::map<DWORD, KNGuildUserInfo> mapMemberList;

    LIF( GetGuildMemberListFromDB( dwGuildUID_, mapMemberList ) ); // 맴버
    LIF( SiKGuildManager()->SetGuildMemberList( dwGuildUID_, mapMemberList ) ); // 맴버리스트 등록

    START_LOG( clog, L"DB에서 길드 맴버리스트 받아와서 등록. GuildUID : " << dwGuildUID_ << L",mapMemberList.size() : " << mapMemberList.size() ) << END_LOG;
}

int KGSDBThread::UpdateGuildGrade( IN const DWORD dwGuildUID_ )
{
    // 신규길드 정식길드로 변환 Game
    /*
    { call dbo.GSGAGuildSystem_update_Kind ( @1 ) }
    @1 ; @iNID_ int

    1 select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; OK            int
    0        ; 성공
    -1       ; 길드정보 없음
    -2       ; 신생길드가 아님
    -101이하 ; 트랜잭션 에러
    */
    if ( dwGuildUID_ == 0 ) return false;

    std::wstring strSP;
    int nOK = -99;
    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystem_update_Kind ( %d ) }",
        dwGuildUID_ ), return nOK );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP >> nOK );
    }
    m_kODBC.EndFetch();

    if ( nOK != 0 ) {
        KNGuildInfo kInfo;
        if ( GetGuildInfoFromDB( dwGuildUID_, kInfo ) ) {
            if ( kInfo.m_ucGrade == KNGuildInfo::GG_OFFICIAL ) {
                START_LOG( cwarn, L"이미 DB에 정식길드인 길드.. GuildUID : " << dwGuildUID_ ) << END_LOG;
                nOK = 0;
            }
        }
    }

    _LOG_SUCCESS( nOK == 0, L"신규길드 정식길드로 변환 DB요청.. Ret : " << nOK )
        << BUILD_LOG( strSP ) << END_LOG;

    return nOK;
}

void KGSDBThread::GetDepotItemListFromDB( IN const DWORD dwUID_, 
                                         IN const KDepotInfo& depotInfo_,
                                         OUT std::vector<KItem>& vecInven_,
                                         OUT std::map<GCITEMUID, GCITEMUID>& mapStrengthItem_ )
{
    std::vector<KItem> vecItem;

    m_kHelper.WareHouse_select( dwUID_, depotInfo_, vecItem );

    m_kHelper.WareHouse_Count_select( dwUID_, depotInfo_, vecItem );
    
    m_kHelper.WareHouse_Attribute_select( dwUID_, depotInfo_, vecItem );

    m_kHelper.WareHouse_Enchant_select( dwUID_, depotInfo_, vecItem );

    m_kHelper.WareHouse_Level_select( dwUID_, depotInfo_, vecItem );

    m_kHelper.WareHouse_Period_select( dwUID_, depotInfo_, vecItem );

    m_kHelper.WareHouse_Strength_select( dwUID_, depotInfo_, vecItem, mapStrengthItem_ );

    m_kHelper.WareHouse_Socket_select( dwUID_, depotInfo_, vecItem );

    m_kHelper.WareHouse_Coordi_select( dwUID_, depotInfo_, vecItem );

    // 창고 아이템에 기간 정보 넣기.
    m_kHelper.WareHouse_ReceiveDate_select( dwUID_, vecItem );
    m_kHelper.WareHouse_ItemCooriCompose( dwUID_, vecItem ); // 코디 합성정보 가져오기.

    // 캐릭터 정보는 이미 알고 있으므로, 여기서 값을 넣어준다.
    {
        std::vector<KItem>::iterator vit;
        for ( vit = vecItem.begin(); vit != vecItem.end(); ++vit ) {
            //vit->m_cCharType = depotInfo_.m_cCharType;
            vit->m_cTabID = depotInfo_.m_cTabID;
        }
    }

    vecInven_.insert( vecInven_.end(), vecItem.begin(), vecItem.end() );
}

bool KGSDBThread::UpdateInvenTypeItem( IN DWORD const dwUserUID_, IN const std::vector<KItem>& vecItem_, IN const std::vector<KItem>& vecDelItem_, IN const char cCharType_ )
{
    DWORD dwCount = 0;
    DWORD dwFailCount = 0;
    std::vector<KItem>::const_iterator vit;

    for ( vit = vecItem_.begin() ; vit != vecItem_.end() ; ++vit ) {
        if ( vit->m_cInitInvenType == vit->m_cInvenType ) { 
            continue; // 슬롯만 옮긴 경우는 넘어가기
        }
        if ( !m_kHelper.UserItem_DelState_Update( dwUserUID_, vit->m_ItemUID, vit->m_cInvenType, cCharType_ ) ) {
            ++dwFailCount;
        }
        ++dwCount;
    }

    std::vector<KItem>::const_iterator vitDel;
    for ( vitDel = vecDelItem_.begin() ; vitDel != vecDelItem_.end(); ++vitDel ) {
        if ( vitDel->m_nCount == 0 ) {
            continue;
        }
        if ( !m_kHelper.UserItem_DelState_Update( dwUserUID_, vitDel->m_ItemUID, vitDel->m_cInvenType, cCharType_ ) ) {
            ++dwFailCount;
        }
        ++dwCount;
    }

    _LOG_SUCCESS( dwFailCount == 0, L"인벤토리/창고 아이템 상태 기록 " << (dwFailCount == 0?L"성공":L"실패") )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOG( dwCount )
        << BUILD_LOG( dwFailCount ) << END_LOG;
    return true;
}

bool KGSDBThread::DeleteUserItemCharacter( IN DWORD const dwUserUID_, IN const std::vector<KItem>& vecItem_, IN const char cCharType_ )
{
    int nSuccessCount = 0;
    int nDeleteCount = 0;
    std::vector<KItem>::const_iterator vit;

    for ( vit = vecItem_.begin() ; vit != vecItem_.end() ; ++vit ) {
        if ( vit->m_cInitInvenType == KItem::INVENTORY && vit->m_cInvenType == KItem::DEPOT ) {
            ++nDeleteCount;
            if ( m_kHelper.UIGAUserItemCharacter_delete( dwUserUID_, cCharType_, vit->m_ItemUID ) == true ) {
                ++nSuccessCount;
            }
        }
    }

    if ( nSuccessCount == nDeleteCount ) {
        return true;
    }
    return false;
}

bool KGSDBThread::InsertUserItemCharacter( DWORD const dwUserUID_, IN const std::vector<KItem>& vecItem_, IN const char cCharType_ )
{
    int nSuccessCount = 0;
    int nInsertCount = 0;
    std::vector<KItem>::const_iterator vit;

    for ( vit = vecItem_.begin() ; vit != vecItem_.end() ; ++vit ) {
        if ( vit->m_cInitInvenType == KItem::DEPOT && vit->m_cInvenType == KItem::INVENTORY ) {
            ++nInsertCount;
            if ( m_kHelper.UIGAUserItemCharacter_insert( dwUserUID_, cCharType_, vit->m_ItemUID ) == 0 ) {
                ++nSuccessCount;
            }
        }
    }

    if ( nSuccessCount == nInsertCount ) {
        return true;
    }
    return false;
}

bool KGSDBThread::DeleteDepotItem(IN DWORD const dwUserUID_, IN const std::vector<KItem>& vecItem_)
{ // â������ �κ��丮�� �ű� ������. â������ �����ϱ�.
    DWORD dwCount = 0;
    DWORD dwFailCount = 0;
    std::vector<KItem>::const_iterator vit; //m_vecDeleteDepotItems

    for (vit = vecItem_.begin(); vit != vecItem_.end(); ++vit) {

        KDepotItem kDepotItem;
        kDepotItem.m_ItemID = vit->m_ItemID;
        kDepotItem.m_ItemUID = vit->m_ItemUID;
        kDepotItem.m_kDepotKey.m_cTabID = vit->m_cTabID;
        //kDepotItem.m_kDepotKey.m_cCharType = vit->m_cCharType;
        kDepotItem.m_kDepotKey.m_cSlotID = vit->m_cSlotID;

        int nRet = -1;
        if (!m_kHelper.WareHouse_Delete(dwUserUID_, kDepotItem, nRet)) {
            ++dwFailCount;
        }

        if (nRet == -21)
        {
            return false;
        }

        ++dwCount;
    }

    _LOG_SUCCESS(dwFailCount == 0, L"â�� ������ ���� " << (dwFailCount == 0 ? L"����" : L"����"))
        << BUILD_LOG(dwUserUID_)
        << BUILD_LOG(dwCount)
        << BUILD_LOG(dwFailCount) << END_LOG;
    return true;
}

bool KGSDBThread::DeleteDepotCountItem(IN DWORD const dwUserUID_, IN const char& cCharType_, IN const std::vector<KItem>& vecItem_)
{
    DWORD dwCount = 0;
    DWORD dwFailCount = 0;
    std::vector<KItem>::const_iterator vit; //m_vecDeleteDepotItems

    for (vit = vecItem_.begin(); vit != vecItem_.end(); ++vit) {

        if (vit->m_nCount == KItem::UNLIMITED_ITEM) { //|| vit->m_nCount > 0 ) {
            continue;
        }
        if (vit->m_bReserveNewUID == true) {
            continue;
        }
        KDepotItem kDepotItem;
        kDepotItem.m_ItemID = vit->m_ItemID;
        kDepotItem.m_ItemUID = vit->m_ItemUID;
        kDepotItem.m_kDepotKey.m_cTabID = vit->m_cTabID;
        //kDepotItem.m_kDepotKey.m_cCharType = vit->m_cCharType;
        kDepotItem.m_kDepotKey.m_cSlotID = vit->m_cSlotID;

        int nRet = -1;
        if (!m_kHelper.WareHouse_Delete(dwUserUID_, kDepotItem, nRet)) {
            ++dwFailCount;
        }

        if (nRet == -21)
        {
            return false;
        }

        ++dwCount;

        //LIF( SetTypeEraseItem( L"", dwUserUID_, vit->m_ItemUID, KItemManager::EDEL_DEL_DEPOT, cCharType_ ) );
    }

    _LOG_SUCCESS(dwFailCount == 0, L"â�� ���� ������ ���� " << (dwFailCount == 0 ? L"����" : L"����"))
        << BUILD_LOG(dwUserUID_)
        << BUILD_LOG(dwCount)
        << BUILD_LOG(dwFailCount) << END_LOG;
    return true;
}

bool KGSDBThread::DeleteDepotZeroCountItem(IN DWORD const dwUserUID_, IN const char& cCharType_, IN const std::vector<KItem>& vecItem_)
{ // â������ �κ��丮�� �ű� ������. â������ �����ϱ�.
    DWORD dwCount = 0;
    DWORD dwFailCount = 0;
    std::vector<KItem>::const_iterator vit; //m_vecDeleteDepotItems

    for (vit = vecItem_.begin(); vit != vecItem_.end(); ++vit) {

        if (vit->m_nCount == KItem::UNLIMITED_ITEM || vit->m_nCount > 0) {
            continue;
        }

        KDepotItem kDepotItem;
        kDepotItem.m_ItemID = vit->m_ItemID;
        kDepotItem.m_ItemUID = vit->m_ItemUID;
        kDepotItem.m_kDepotKey.m_cTabID = vit->m_cTabID;
        kDepotItem.m_kDepotKey.m_cSlotID = vit->m_cSlotID;

        int nRet = -1;
        if (!m_kHelper.WareHouse_Delete(dwUserUID_, kDepotItem, nRet)) {
            ++dwFailCount;
        }

        if (nRet == -21)
        {
            return false;
        }

        ++dwCount;

        LIF(SetTypeEraseItem(L"", dwUserUID_, vit->m_ItemUID, KItemManager::EDEL_DEL_DEPOT, cCharType_));
    }

    _LOG_SUCCESS(dwFailCount == 0, L"â�� ���� ������ ���� " << (dwFailCount == 0 ? L"����" : L"����"))
        << BUILD_LOG(dwUserUID_)
        << BUILD_LOG(dwCount)
        << BUILD_LOG(dwFailCount) << END_LOG;
    return true;
}

bool KGSDBThread::MergeDepotItem( IN DWORD const dwUserUID_, IN const std::vector<KItem>& vecItem_ )
{
    DWORD dwCount = 0;
    DWORD dwFailCount = 0;
    std::vector<KItem>::const_iterator vit; //m_vecInvenTypeItemInv

    for ( vit = vecItem_.begin(); vit != vecItem_.end(); ++vit ) {

        KDepotItem kDepotItem;
        kDepotItem.m_ItemID = vit->m_ItemID;
        kDepotItem.m_ItemUID = vit->m_ItemUID;
        kDepotItem.m_kDepotKey.m_cTabID = vit->m_cTabID;
        //kDepotItem.m_kDepotKey.m_cCharType = vit->m_cCharType;
        kDepotItem.m_kDepotKey.m_cSlotID = vit->m_cSlotID;

        if ( !m_kHelper.WareHouse_Merge( dwUserUID_, kDepotItem ) ) {
            ++dwFailCount;

            START_LOG( cerr, L"창고 데이터 기록 실패" )
                << BUILD_LOG( vit->m_ItemID )
                << BUILD_LOG( vit->m_ItemUID )
                << BUILD_LOGc( vit->m_cGradeID )
                << BUILD_LOG( vit->m_nCount )
                << BUILD_LOG( vit->m_nInitCount )
                << BUILD_LOGc( vit->m_cInvenType )
                << BUILD_LOGc( vit->m_cInitInvenType )
                << BUILD_LOG( vit->m_bReserveNewUID )
                << BUILD_LOGc( vit->m_cInitSlotID )
                << BUILD_LOGc( vit->m_cSlotID )
                << END_LOG;
        }
        ++dwCount;        
    }

    _LOG_SUCCESS( dwFailCount == 0, L"창고 아이템 넣기, 위치변경 " << (dwFailCount == 0? L"성공" : L"실패") )
        << BUILD_LOG( dwUserUID_ )  
        << BUILD_LOG( dwCount )
        << BUILD_LOG( dwFailCount ) << END_LOG;
    
    return true;
};

bool KGSDBThread::GetPetExp( IN DWORD const dwUserUID_, IN const char& cCharType_, IN OUT std::map<GCITEMUID, KPetInfo>& mapPetInfo_ )
{
    /*
    L"{ call dbo.UIGAUserItemPetExp_select_20130510 ( %d, %d ) }"

    { call dbo.UIGAUserItemPetExp_select_20130510 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int

    n return ( @01, @02, @03 )
    @01: PetUID    bigint
    @02: Promotion tinyint
    @03: [Exp]     int
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemPetExp_select_20130510 ( %d, %d ) }", dwUserUID_, cCharType_ ) );
    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID dwPetUID;
        char  cPromotion;
        DWORD dwExp;
        VERIFY_QUERY( m_kODBC 
            >>dwPetUID 
            >>cPromotion
            >>dwExp );

        std::map<GCITEMUID, KPetInfo>::iterator mit;
        mit = mapPetInfo_.find( dwPetUID );
        if ( mit == mapPetInfo_.end() )  {            
            START_LOG( cwarn, L"펫 전직별 경험치 가져오는 부분 오류" )
                << BUILD_LOG( dwUserUID_ )                
                << BUILD_LOG( dwPetUID )
                << BUILD_LOGc( cPromotion )
                << BUILD_LOG( dwExp ) << END_LOG;
            continue;
        }
        mit->second.m_mapInitExp.insert( std::make_pair( cPromotion, dwExp ) );
        // 현재 전직과 같은 전직 레벨의 초기 경험치를 '현재' 경험치에 넣기
        if ( mit->second.m_cPromotion == cPromotion ) {
            mit->second.m_dwEXP = dwExp;
            mit->second.m_iLevel = SiKPetHelper()->GetPetLevel( mit->second.m_dwEXP );
        }
    }
    
    return true;
}

bool KGSDBThread::GetPetExp_Depot( IN DWORD const dwUserUID_, IN const KDepotInfo& kDepot_, IN OUT std::map<GCITEMUID, KPetInfo>& mapPetInfo_ )
{
    /* 
    창고Tab단위로유효한펫경험치리턴 
    L"{ call dbo.WHGAWareHousePetExp_select_20130506 ( %d, %d ) }"
    { call dbo.WHGAWareHousePetExp_select_20130506 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iTabID_input    tinyint
     
    n return ( @01, @02, @03 )
    @01: PetUID    bigint
    @02: Promotion tinyint
    @03: Exp       int
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.WHGAWareHousePetExp_select_20130506 ( %d, %d ) }", dwUserUID_, kDepot_.m_cTabID ) );
    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID dwPetUID;
        char  cPromotion;
        DWORD dwExp;
        VERIFY_QUERY( m_kODBC 
            >>dwPetUID 
            >>cPromotion
            >>dwExp );

        std::map<GCITEMUID, KPetInfo>::iterator mit;
        mit = mapPetInfo_.find( dwPetUID );
        if ( mit == mapPetInfo_.end() )  {            
            START_LOG( cwarn, L"펫 전직별 경험치 가져오는 부분 오류" )
                << BUILD_LOG( dwUserUID_ )                
                << BUILD_LOG( dwPetUID )
                << BUILD_LOGc( cPromotion )
                << BUILD_LOG( dwExp ) << END_LOG;
            continue;
        }
        mit->second.m_mapInitExp.insert( std::make_pair( cPromotion, dwExp ) );
        // 현재 전직과 같은 전직 레벨의 초기 경험치를 '현재' 경험치에 넣기
        if ( mit->second.m_cPromotion == cPromotion ) {
            mit->second.m_dwEXP = dwExp;
            mit->second.m_iLevel = SiKPetHelper()->GetPetLevel( mit->second.m_dwEXP );
        }
    }
    
    return true;
}

bool KGSDBThread::Update_DungeonSubject_Stat_ToDB( IN KDungeonSubjectStat& kPacket_ )
{
    /*
    L"{ call dbo.DSGADungeonSubject_insert ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }"

    { call dbo.DSGADungeonSubject_insert ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10, @11 ) }
    @1 ; @iLoginUID_     int        //유저UID
    @2 ; @iModeID_       int        //던전ID
    @3 ; @iDifficulty_   int        //던전난이도
    @4 ; @iStage_        int        //던전스테이지번호
    @5 ; @iBlock_        int        //던전스테이지의블록단위번호
    @6 ; @iCharType_     int        //캐릭터타입
    @7 ; @iLevel_        int        //캐릭터레벨
    @8 ; @iPromotion_    int        //캐릭터전직
    @9 ; @iACWorkNum_    int        //과제의번호
    @10 ; @iRecord_      int        //과제기록
    @11 ; @iResult_      int        //과제성공여부

    1 return ( @1 )
    @1 ; OK int
        0        ; 성공
        -101이하 ; 트랜잭션에러
        -801     ; 로그기록실패
    */

    typedef std::map< int, std::vector< std::pair< int, bool > > > mapSubjectClearInfoType;
    typedef std::vector< std::pair< int, bool > > vecClearInfoType;

    BOOST_FOREACH( mapSubjectClearInfoType::value_type& rSubjectClearInfo, kPacket_.m_mapSubjectClearInfo ) {
        BOOST_FOREACH( vecClearInfoType::value_type& rClearInfo, rSubjectClearInfo.second ) {
            DBJIF2( m_kODBC.Query( L"{ call dbo.DSGADungeonSubject_insert ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }",
                kPacket_.m_dwLoginUID,
                kPacket_.m_nGameMode,
                kPacket_.m_nDifficulty,
                rSubjectClearInfo.first,
                0,  // 기획에서 사용하지 않기로함
                kPacket_.m_nCharType,
                kPacket_.m_nCharLevel,
                kPacket_.m_nCharPromotion,
                rClearInfo.first,
                0, // 기획에서 사용하지 않기로함
                rClearInfo.second ) );

            int nRet = -99;

            if ( m_kODBC.BeginFetch() ) {
                VERIFY_QUERY( m_kODBC >> nRet );
                m_kODBC.EndFetch();
            }

            if ( 0 != nRet ) {
                START_LOG( cerr, L"던전 달성 과제 통계 기록 중 오류. Result : " << nRet )
                    << BUILD_LOG( m_kODBC.m_strLastQuery )
                    << END_LOG;
            }
        }
    }

    return true;
}
int KGSDBThread::GetLastPlayCharacter( IN const DWORD dwUID_ )
{
    /*
    L"{ call dbo.GBGAGuideBook_select_20130703( %d ) }"  

    { call dbo.GBGAGuideBook_select_20130703 ( @01 ) }  
    @01iLoginUID_input int  
      
    1 return ( @01 )  
    @01iCharType int  
    */

    int nCharType = -1;
    DBJIF3( m_kODBC.Query( L"{ call dbo.GBGAGuideBook_select_20130703 ( %d ) }", dwUID_ ), return -1 );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nCharType  );
        m_kODBC.EndFetch();
    }

    START_LOG( clog, L" 마지막 플레이 캐릭터 정보. UID : " << dwUID_  ) << END_LOG;
    return nCharType;
}

int KGSDBThread::UpdateDefaultBonusLife( IN const DWORD dwUID_, IN const char cCharType_, IN const int nSlotID_, IN const int nBaseBonusDiff_ )
{
    /*
    L"{ call dbo.CIGACharacterInfo_update_DefaultBonus_20130620 ( %d, %d, %d, %d ) }"

    { call dbo.CIGACharacterInfo_update_DefaultBonus_20130620 ( @01, @02, @03, @04 ) }
        @01iLoginUID_input          int
    ,   @02iCharType_input          int
    ,   @03iSlotNo_input            int
    ,   @04iDefaultBonus_diff_input int
        
    1 return ( @01 )
    @01: OK        int
        0        : 캐릭터 생성 성공
        -1       : 캐릭터가 존재하지 않음
        -801~-899: 무시가능 에러
        -901~-999: 치명적인 에러
    */
    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.CIGACharacterInfo_update_DefaultBonus_20130620 ( %d, %d, %d, %d ) }",
        dwUID_, cCharType_, nSlotID_, nBaseBonusDiff_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK  );
        m_kODBC.EndFetch();
    }

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

    if ( 0 != nOK ) {
        START_LOG( cerr, L"보너스 포인트 DB 기록 중 오류. Result : " << nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
    }

    return nOK;
}

int KGSDBThread::UpdateSpecialBonusLife( IN const DWORD dwUID_, IN const char cCharType_, IN const int nSlotID_, IN const int nSpecialBonusDiff_ )
{
    /*
    L"{ call dbo.CIGACharacterInfo_update_SpecialBonus_20130620 ( %d, %d, %d, %d ) }"

    { call dbo.CIGACharacterInfo_update_SpecialBonus_20130620 ( @01, @02, @03, @04 ) }
        @01iLoginUID_input          int
    ,   @02iCharType_input          int
    ,   @03iSlotNo_input            int
    ,   @04iSpecialBonus_diff_input int
        
    1 return ( @01 )
    @01: OK        int
        0        : 캐릭터 생성 성공
        -1       : 캐릭터가 존재하지 않음
        -801~-899: 무시가능 에러
        -901~-999: 치명적인 에러
    */
    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.CIGACharacterInfo_update_SpecialBonus_20130620 ( %d, %d, %d, %d ) }",
        dwUID_, cCharType_, nSlotID_, nSpecialBonusDiff_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK  );
        m_kODBC.EndFetch();
    }

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

    if ( 0 != nOK ) {
        START_LOG( cerr, L"보너스 포인트 DB 기록 중 오류. Result : " << nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
    }

    return nOK;
}

bool KGSDBThread::UpdateBonusLife( IN const DWORD dwUID_, IN const char cCharType_, IN const int nSlotID_, IN const KBonusPointInfo& kBonusLifeDiff_ )
{
    UpdateDefaultBonusLife( dwUID_, cCharType_, nSlotID_, kBonusLifeDiff_.m_nBaseBonus );

    UpdateSpecialBonusLife( dwUID_, cCharType_, nSlotID_, kBonusLifeDiff_.m_nSpecialBonus );

    return true;
}

bool KGSDBThread::UseCountItem( IN const DWORD dwUID_, IN const GCITEMUID ItemUID_, IN const int nDellFactor_ )
{
    return UpdateCountItem( dwUID_, ItemUID_, -nDellFactor_ );
}

bool KGSDBThread::AddCountItem( IN const DWORD dwUID_, IN const GCITEMUID ItemUID_, IN const int nDellFactor_ )
{
    return UpdateCountItem( dwUID_, ItemUID_, nDellFactor_ );
}

bool KGSDBThread::Update_VitalitySystem_UserData_ToDB( IN const DWORD dwLoginUID_, IN const std::map< char, KCharacterInfo >& mapCharacterInfo_, IN const KVitalityUpdateData& kVitalityUpdateData_ )
{
    if ( false == SiKVitalityManager()->IsRun() ) {
        return false;
    }

    Update_VitalitySystem_RechargePoint_ToDB( dwLoginUID_, kVitalityUpdateData_.m_nRechargePoint, kVitalityUpdateData_.m_nRechargePointCounter );
    Update_VitalitySystem_RemainVitality_ToDB( dwLoginUID_, mapCharacterInfo_, kVitalityUpdateData_.m_mapRemainVitality, KVitalityManager::EVUT_LOGOUT );

    return true;
}

bool KGSDBThread::Get_VitalitySystem_RechargePoint_FromDB( IN const DWORD dwLoginUID_, OUT int& nRechargePoint_, OUT int& nRechargePointCounter_ )
{
    /* 활력 충전 포인트 검색
    L"{ call dbo.VSGAVitalitySystemCharge_select ( %d ) }"

    { call dbo.VSGAVitalitySystemCharge_select ( @1 ) }
    @1 ; @iLoginUID_ int


    1 return ( @1, @2 )
    @1 ; ChargePoint  int
    @2 ; Consumption  int
    */
    nRechargePoint_ = 0;
    nRechargePointCounter_ = 0;

    DBJIF2( m_kODBC.Query( L"{ call dbo.VSGAVitalitySystemCharge_select ( %d ) }",
        dwLoginUID_ ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        VERIFY_QUERY( m_kODBC
            >> nRechargePoint_
            >> nRechargePointCounter_ );
    }

    START_LOG( clog, L"활력 시스템 재충전 포인트 정보 조회" )
        << BUILD_LOG( dwLoginUID_ )
        << BUILD_LOG( nRechargePoint_ )
        << BUILD_LOG( nRechargePointCounter_ )
        << END_LOG;

    return true;
}

bool KGSDBThread::Get_VitalitySystem_RemainVitality_FromDB( IN const DWORD dwLoginUID_, IN const std::map< char, KCharacterInfo >& mapCharacterInfo_, OUT std::map< int, int >& mapRemainVitality_ )
{
    /* 활력 정보 검색
    L"{ call dbo.VSGAVitalitySystemCharacter_select ( %d, %d ) }"

    { call dbo.VSGAVitalitySystemCharacter_select ( @1, @2 ) }
    @1 ; @iLoginUID_ int
    @2 ; @iCharType_ int


    1 return ( @1 )
    @1 ; Vitality  int
    */
    mapRemainVitality_.clear();

    // 캐릭터를 보유하고 있지만 활력 정보는 없는 경우의 보정을 위해 먼저 활력을 최대치로 설정한다
    Initialize_VitalitySystem_RemainVitality( mapCharacterInfo_, mapRemainVitality_ );

    typedef std::map< int, int > mapRemainVitalityType;
    BOOST_FOREACH( mapRemainVitalityType::value_type& rRemainVitalityInfo, mapRemainVitality_ ) {
        DBJIF2( m_kODBC.Query( L"{ call dbo.VSGAVitalitySystemCharacter_select ( %d, %d ) }",
            dwLoginUID_, rRemainVitalityInfo.first ) );

        while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
            VERIFY_QUERY( m_kODBC
                >> rRemainVitalityInfo.second );
        }
    }

    START_LOG( clog, L"활력 시스템 캐릭터별 잔여 활력 조회(보유한 캐릭터로 보정된 정보)" )
        << BUILD_LOG( dwLoginUID_ )
        << BUILD_LOG( mapCharacterInfo_.size() )
        << BUILD_LOG( mapRemainVitality_.size() )
        << END_LOG;

    return true;
}

bool KGSDBThread::Get_VitalitySystem_LastResetTime_FromDB( IN const DWORD dwLoginUID_, OUT time_t& tmLastInitialize_ )
{
    /* 마지막 활력 초기화 시간 검색
    L"{ call dbo.VSGAVitalitySystemReset_select ( %d ) }"

    { call dbo.VSGAVitalitySystemReset_select ( @1 ) }
    @1 ; @iLoginUID_ int


    1 return ( @1 )
    @1 ; RegDateA  datetime
    */
    tmLastInitialize_ = 0;

    DBJIF2( m_kODBC.Query( L"{ call dbo.VSGAVitalitySystemReset_select ( %d ) }",
        dwLoginUID_ ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        VERIFY_QUERY( m_kODBC
            >> tmLastInitialize_ );
    }

    // 활력 시스템 적용 후 첫 접속 유저의 경우처럼 이전 활력 초기화 정보가 없는 경우에도
    // 초기화 처리를 할 수 있도록 DB에 데이터가 없는 경우 2일전 날짜로 세팅한다
    if ( 0 == tmLastInitialize_ ) {
        CTime kCurrentTime( CTime::GetCurrentTime() );
        CTime tmYesterDay = CTime( kCurrentTime.GetYear(), kCurrentTime.GetMonth(), kCurrentTime.GetDay(), 0, 0, 0 ) - CTimeSpan( 2, 0, 0, 0 );
        tmLastInitialize_ = KncUtil::TimeToInt( tmYesterDay );

        START_LOG( clog, L"DB에서 활력 초기화 시간 얻기 실패. 2일 전 날짜로 설정." )
            << BUILD_LOG( tmLastInitialize_ )
            << END_LOG;
    }

    START_LOG( clog, L"활력 마지막 초기화 시간 조회(데이터가 없는 경우 보정 포함)" )
        << BUILD_LOG( dwLoginUID_ )
        << BUILD_LOG( tmLastInitialize_ )
        << END_LOG;

    return true;
}

bool KGSDBThread::CheckNeedVitalityReset( IN const time_t tmLastInitialize_ )
{
    // 다음 초기화 시간 = 마지막 초기화 시간의 00시 + 하루 + 활력을 초기화 할 시간
    CTimeSpan kVitalityResetSpanTime;
    SiKVitalityManager()->GetVitalityResetSpanTime( kVitalityResetSpanTime );

    CTime kLastInitializeTime( tmLastInitialize_ );
    CTime kNextInitializeTime( kLastInitializeTime.GetYear(), kLastInitializeTime.GetMonth(), kLastInitializeTime.GetDay(), 0, 0, 0 );
    kNextInitializeTime += CTimeSpan( 1, 0, 0, 0 );
    kNextInitializeTime += kVitalityResetSpanTime;

    time_t tmNextInitialize = KncUtil::TimeToInt( kNextInitializeTime );
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    if ( tmNextInitialize < tmCurrent ) {
        // 다음 초기화 시간이 지난 경우 초기화를 진행해야 한다
        return true;
    }

    return false;
}

bool KGSDBThread::Initialize_VitalitySystem_RechargePoint( OUT int& nRechargePoint_, OUT int& nRechargePointCounter_ )
{
    nRechargePoint_ = 0;
    nRechargePointCounter_ = 0;

    return true;
}

bool KGSDBThread::Initialize_VitalitySystem_RemainVitality( IN const std::map< char, KCharacterInfo >& mapCharacterInfo_, OUT std::map< int, int >& mapRemainVitality_ )
{
    mapRemainVitality_.clear();

    int nMaxVitalityPerDay = SiKVitalityManager()->GetMaxVitalityPerDay();

    typedef std::map< char, KCharacterInfo > mapCharacterInfoType;
    BOOST_FOREACH( const mapCharacterInfoType::value_type& rCharacterInfo, mapCharacterInfo_ ) {
        int nCharType = static_cast< int >( rCharacterInfo.first );

        mapRemainVitality_[ nCharType ] = nMaxVitalityPerDay;
    }

    return true;
}

bool KGSDBThread::Update_VitalitySystem_RechargePoint_ToDB( IN const DWORD dwLoginUID_, IN const int nRechargePoint_, IN int nRechargePointCounter_ )
{
    /* 활력 충전 포인트 갱신
    L"{ call dbo.VSGAVitalitySystemCharge_merge ( %d, %d, %d ) }"

    { call dbo.VSGAVitalitySystemCharge_merge ( @1, @2, @3 ) }
    @1: @iLoginUID_     int
    @2: @iChargePoint_  int
    @3: @iConsumption_  int  //Consumption 값이서버에지정된수치에도달하면ChargePoint 값1로변환됨.

    1 return ( @1 )
    @1: OK       int
        0       : 성공
        -1      : 유저가없음
        -801    ; 로그기록실패
        -901    ; 활력충전포인트데이터갱신실패
        -902    ; 활력충전포인트데이터등록실패
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.VSGAVitalitySystemCharge_merge ( %d, %d, %d ) }",
        dwLoginUID_, nRechargePoint_, nRechargePointCounter_ ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nRet == 0, L"활력 시스템 재충전 포인트 갱신(DB)" )
        << BUILD_LOG( nRet )
        << BUILD_LOG( dwLoginUID_ )
        << BUILD_LOG( nRechargePoint_ )
        << BUILD_LOG( nRechargePointCounter_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return true;
}

bool KGSDBThread::Update_VitalitySystem_RemainVitality_ToDB( IN const DWORD dwLoginUID_, IN const std::map< char, KCharacterInfo >& mapCharacterInfo_, IN const std::map< int, int >& mapRemainVitality_, IN const int nType_ )
{
    typedef std::map< int, int > mapRemainVitalityType;
    BOOST_FOREACH( const mapRemainVitalityType::value_type& rRemainVitalityInfo, mapRemainVitality_ ) {
        std::map< char, KCharacterInfo >::const_iterator cmitCharacterInfo;
        char cCharType = static_cast< char >( rRemainVitalityInfo.first );

        cmitCharacterInfo = mapCharacterInfo_.find( cCharType );
        if ( cmitCharacterInfo != mapCharacterInfo_.end() ) {
            Update_VitalitySystem_RemainVitality_Character_ToDB( dwLoginUID_, cmitCharacterInfo->second, rRemainVitalityInfo.second, nType_ );
        }
    }

    return true;
}

bool KGSDBThread::Update_VitalitySystem_RemainVitality_Character_ToDB( IN const DWORD dwLoginUID_, IN const KCharacterInfo& kCharacterInfo_, IN const int nRemainVitality_, IN const int nType_ )
{
    /* 활력 정보 갱신
    L"{ call dbo.VSGAVitalitySystemCharacter_merge ( %d, %d, %d, %d, %d ) }"

    { call dbo.VSGAVitalitySystemCharacter_merge ( @1, @2, @3, @4, @5 ) }
    @1: @iLoginUID_  int
    @2: @iCharType_  int
    @3: @iVitality_  int  
    @4: @iType_      int  //로그데이터
    @5: @iLevel_     int  //로그데이터

    1 return ( @1 )
    @1: OK       int
        0       : 성공
        -1      : 유저가없음
        -801    ; 로그기록실패
        -901    ; 케릭터활력데이터갱신실패
        -902    ; 케릭터활력데이터등록실패
    */

    int nCharType = static_cast< int >( kCharacterInfo_.m_cCharType );
    int nCharLevel = static_cast< int >( kCharacterInfo_.m_dwLevel );

    DBJIF2( m_kODBC.Query( L"{ call dbo.VSGAVitalitySystemCharacter_merge ( %d, %d, %d, %d, %d ) }",
        dwLoginUID_, nCharType, nRemainVitality_, nType_, nCharLevel ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nRet == 0, L"활력 정보 유저 데이터 DB에 기록(DB)" )
        << BUILD_LOG( nRet )
        << BUILD_LOG( dwLoginUID_ )
        << BUILD_LOG( nCharType )
        << BUILD_LOG( nRemainVitality_ )
        << BUILD_LOG( nType_ )
        << BUILD_LOG( nCharLevel )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return true;
}

bool KGSDBThread::Update_VitalitySystem_LastResetTime_ToDB( IN const DWORD dwLoginUID_ )
{
    /* 마지막 활력 초기화 시간 갱신
    L"{ call dbo.VSGAVitalitySystemReset_merge ( %d ) }"

    { call dbo.VSGAVitalitySystemReset_merge ( @1 ) }
    @1: @iLoginUID_     int

    1 return ( @1 )
    @1: OK       int
        0       : 성공
        -1      : 유저가없음
        -801    ; 로그기록실패
        -901    ; 활력포인트초기화데이터갱신실패
        -902    ; 활력포인트초기화데이터등록실패
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.VSGAVitalitySystemReset_merge ( %d ) }", dwLoginUID_ ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nRet == 0, L"활력 마지막 초기화 시간 갱신(DB시간으로 기록함)" )
        << BUILD_LOG( nRet )
        << BUILD_LOG( dwLoginUID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return true;
}

bool KGSDBThread::VitalitySystemDungeonPlayStat_insert( IN const KEVENT_STAT_VITALITY_DUNGEON_PLAY_NOT& kPacket_ )
{
    /*
    L"{ call dbo.VSGAVitalitySystemDungeonCollect_Insert ( %d, %d, %d, %d, %d, %d, %d, %d, %d ) }"

    { call dbo.VSGAVitalitySystemDungeonCollect_Insert ( @1, @2, @3, @4, @5, @6, @7, @8, @9 ) }
    @1 ; @iModeID_     int
    @2 ; @iDifficult_  int
    @3 ; @iLoginUID_   int
    @4 ; @iPlayTime_   int
    @5 ; @iParty_      int
    @6 ; @iCharType_   int
    @7 ; @iLevel_      int
    @8 ; @iResult_     int
    @9 ; @iVitality_   int

    1 return ( @1 )
    @1 ; @iOK int
    0        ; 성공
    -801이하: 로그기록실패
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.VSGAVitalitySystemDungeonCollect_Insert ( %d, %d, %d, %d, %d, %d, %d, %d, %d ) }",
        kPacket_.m_nModeID,
        kPacket_.m_nDifficulty,
        static_cast< int >( kPacket_.m_dwLoginUID ),
        static_cast< int >( kPacket_.m_dwPlayTime ),
        kPacket_.m_nTotalUsers,
        kPacket_.m_nCharType,
        kPacket_.m_nCharLevel,
        kPacket_.m_nClearType,
        kPacket_.m_nRemainVitality ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nRet == 0, L"활력 시스템 던전 플레이 통계 DB기록(DB)" )
        << BUILD_LOG( nRet )
        << BUILD_LOG( kPacket_.m_dwLoginUID )
        << BUILD_LOG( kPacket_.m_nModeID )
        << BUILD_LOG( kPacket_.m_nDifficulty )
        << BUILD_LOG( kPacket_.m_dwPlayTime )
        << BUILD_LOG( kPacket_.m_nTotalUsers )
        << BUILD_LOG( kPacket_.m_nCharType )
        << BUILD_LOG( kPacket_.m_nCharLevel )
        << BUILD_LOG( kPacket_.m_nClearType )
        << BUILD_LOG( kPacket_.m_nRemainVitality )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return true;
}

bool KGSDBThread::Update_ConnectionGiftBox_UserData_ToDB( IN const DWORD dwUID_, IN const int nCharType_, IN const KConnectionGiftBoxUpdateData& kConnectionGiftBoxUpdateData_ )
{
    /*
    L"{ call dbo.PBGAPresentBoxSystem_merge_20130910 ( %d, %d, %d, %d, %d ) }"

    { call dbo.PBGAPresentBoxSystem_merge_20130910 ( @01, @02, @03, @04, @05 ) }
        @01iLoginUID_input      int
    ,   @02iCharType_input      int
    ,   @03iVersionType_input   int
    ,   @04iStep_input          int
    ,   @05iDurationTime_input  int

    1 return ( @01 )
    @01: OK int
        0        : 성공
        -1       : 유저 정보 없음
        -2       : 캐릭터 정보 없음
        -801~-899: 무시가능 에러
        -901~-999: 치명적인 에러
    */

    // 신규유저 유저데이터 저장
    if ( true == kConnectionGiftBoxUpdateData_.m_bIsNewUser ) {
        DBJIF2( m_kODBC.Query( L"{ call dbo.PBGAPresentBoxSystem_merge_20130910 ( %d, %d, %d, %d, %d ) }",
            dwUID_,
            nCharType_,
            0, // 신규유저는 버전을 항상 0으로 기록한다
            kConnectionGiftBoxUpdateData_.m_nNewUserLastGetGrade,
            kConnectionGiftBoxUpdateData_.m_nNewUserAccPlayTime ) );

        int nRet = -99;
        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> nRet );
            m_kODBC.EndFetch();
        }

        LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nRet ) );

        _LOG_SUCCESS( nRet == 0, L"접속 선물상자 신규유저 정보 저장" )
            << BUILD_LOG( nRet )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kConnectionGiftBoxUpdateData_.m_nNewUserLastGetGrade )
            << BUILD_LOG( kConnectionGiftBoxUpdateData_.m_nNewUserAccPlayTime )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
    }

    // 이벤트유저 유저데이터 저장
    if ( true == kConnectionGiftBoxUpdateData_.m_bIsEventUser ) {
        DBJIF2( m_kODBC.Query( L"{ call dbo.PBGAPresentBoxSystem_merge_20130910 ( %d, %d, %d, %d, %d ) }",
            dwUID_,
            nCharType_,
            kConnectionGiftBoxUpdateData_.m_nEventUserVersion,
            kConnectionGiftBoxUpdateData_.m_nEventUserLastGetGrade,
            kConnectionGiftBoxUpdateData_.m_nEventUserAccPlayTime ) );

        int nRet = -99;
        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> nRet );
            m_kODBC.EndFetch();
        }

        LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nRet ) );

        _LOG_SUCCESS( nRet == 0, L"접속 선물상자 이벤트유저 정보 저장" )
            << BUILD_LOG( nRet )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kConnectionGiftBoxUpdateData_.m_nEventUserVersion )
            << BUILD_LOG( kConnectionGiftBoxUpdateData_.m_nEventUserLastGetGrade )
            << BUILD_LOG( kConnectionGiftBoxUpdateData_.m_nEventUserAccPlayTime )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
    }

    return true;
}

bool KGSDBThread::Update_ConnectionGiftBox_EventUserData_ToDB( IN const DWORD dwUID_, IN const int nCharType_, IN const int nVersion_, IN const int nCurrentGrade_ )
{
    /*
    L"{ call dbo.PBGAPresentBoxSystem_merge_20130910 ( %d, %d, %d, %d, %d ) }"

    { call dbo.PBGAPresentBoxSystem_merge_20130910 ( @01, @02, @03, @04, @05 ) }
        @01iLoginUID_input      int
    ,   @02iCharType_input      int
    ,   @03iVersionType_input   int
    ,   @04iStep_input          int
    ,   @05iDurationTime_input  int

    1 return ( @01 )
    @01: OK int
        0        : 성공
        -1       : 유저 정보 없음
        -2       : 캐릭터 정보 없음
        -801~-899: 무시가능 에러
        -901~-999: 치명적인 에러
    */

    // 신규유저 유저데이터 저장
    DBJIF2( m_kODBC.Query( L"{ call dbo.PBGAPresentBoxSystem_merge_20130910 ( %d, %d, %d, %d, %d ) }",
        dwUID_,
        nCharType_,
        nVersion_,
        nCurrentGrade_,
        0 ) ); // 보상을 받았으므로 누적 시간을 0으로 초기화한다

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nRet ) );

    _LOG_SUCCESS( nRet == 0, L"접속 선물상자 유저 정보 저장" )
        << BUILD_LOG( nRet )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( nCharType_ )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( nCurrentGrade_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;

    return ( 0 == nRet );
}

bool KGSDBThread::Get_ConnectionGiftBox_UserData_FromDB( IN const DWORD dwUID_, IN const int nCharType_, IN OUT KConnectionGiftBoxUpdateData& kConnectionGiftBoxUpdateData_ )
{
    /*
    L"{ call dbo.PBGAPresentBoxSystem_select_20130910 ( %d, %d, %d ) }"

    { call dbo.PBGAPresentBoxSystem_select_20130910 ( @01, @02, @03 ) }
        @01iLoginUID_input    int
    ,   @02iCharType_input    int
    ,   @03iVersionType_input int

    1 select ( @01, @02 )
        @01iStep         int
    ,   @02iDurationTime int
    */

    // DB에 값이 없는 경우 0을 기본값으로 설정한다
    kConnectionGiftBoxUpdateData_.m_nNewUserLastGetGrade    = 0;
    kConnectionGiftBoxUpdateData_.m_nNewUserAccPlayTime     = 0;
    kConnectionGiftBoxUpdateData_.m_nEventUserLastGetGrade  = 0;
    kConnectionGiftBoxUpdateData_.m_nEventUserAccPlayTime   = 0;

    // 신규유저 유저데이터 가져오기
    if ( true == kConnectionGiftBoxUpdateData_.m_bIsNewUser ) {
        DBJIF2( m_kODBC.Query( L"{ call dbo.PBGAPresentBoxSystem_select_20130910 ( %d, %d, %d ) }",
            dwUID_, nCharType_, 0 ) ); // 신규유저는 버전을 항상 0으로 기록한다

        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> kConnectionGiftBoxUpdateData_.m_nNewUserLastGetGrade
                                >> kConnectionGiftBoxUpdateData_.m_nNewUserAccPlayTime );

            m_kODBC.EndFetch();
        }

        START_LOG( clog, L"접속 선물상자 신규유저 정보 조회" )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( nCharType_ )
            << BUILD_LOG( kConnectionGiftBoxUpdateData_.m_nNewUserLastGetGrade )
            << BUILD_LOG( kConnectionGiftBoxUpdateData_.m_nNewUserAccPlayTime )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
    }

    // 이벤트유저 유저데이터 가져오기
    if ( true == kConnectionGiftBoxUpdateData_.m_bIsEventUser ) {
        DBJIF2( m_kODBC.Query( L"{ call dbo.PBGAPresentBoxSystem_select_20130910 ( %d, %d, %d ) }",
            dwUID_, nCharType_, kConnectionGiftBoxUpdateData_.m_nEventUserVersion ) );

        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> kConnectionGiftBoxUpdateData_.m_nEventUserLastGetGrade
                                >> kConnectionGiftBoxUpdateData_.m_nEventUserAccPlayTime );

            m_kODBC.EndFetch();
        }

        START_LOG( clog, L"접속 선물상자 이벤트유저 정보 조회" )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( nCharType_ )
            << BUILD_LOG( kConnectionGiftBoxUpdateData_.m_nEventUserLastGetGrade )
            << BUILD_LOG( kConnectionGiftBoxUpdateData_.m_nEventUserAccPlayTime )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
    }

    return true;
}

void KGSDBThread::CheckConnectionGiftBoxNewUser( IN const DWORD dwUID_, IN const time_t tmFirstLoginTime_, OUT bool& bConnectionGiftBoxNewUser_ )
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    time_t tmDecideBeginTimeNewUser = SiKConnectionGiftBox()->GetDecideBeginTimeNewUser();
    int nNewUserEffectDuration = SiKConnectionGiftBox()->GetNewUserEffectDuration();
    time_t tmNewUserEffectDuration = nNewUserEffectDuration * 24 * 60 * 60;
    time_t tmNewUserEffectEnd = 0;

    bConnectionGiftBoxNewUser_ = false;

    int nData = GetTodayEventCount( dwUID_, UET_CONNECTION_GIFTBOX_NEW_USER, tmFirstLoginTime_ );
    if ( 0 == nData && tmDecideBeginTimeNewUser <= tmFirstLoginTime_ ) {
        SetTodayEventDoneDiff( dwUID_, UET_CONNECTION_GIFTBOX_NEW_USER, tmFirstLoginTime_, static_cast< int >( tmCurrent ) );

        bConnectionGiftBoxNewUser_ = true;
    }
    else {
        tmNewUserEffectEnd = static_cast< time_t >( nData ) + tmNewUserEffectDuration;
        if ( tmCurrent < tmNewUserEffectEnd ) {
            bConnectionGiftBoxNewUser_ = true;
        }
    }

    START_LOG( clog, L"접속 선물상자 신규유저 대상인지 확인" )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOGtm( CTime( tmFirstLoginTime_ ) )
        << BUILD_LOGtm( CTime( tmCurrent ) )
        << BUILD_LOGtm( CTime( tmDecideBeginTimeNewUser ) )
        << BUILD_LOGtm( CTime( tmNewUserEffectEnd ) )
        << BUILD_LOG( nNewUserEffectDuration )
        << BUILD_LOG( bConnectionGiftBoxNewUser_ )
        << END_LOG;
}

bool KGSDBThread::SendUpgradeGuildGradeGiftNot( IN const DWORD dwUID_, IN const char cCharType_, IN const DWORD dwGuildUID_ )
{
    KEVENT_UPGRADE_GUILD_GRADE_GIFT_NOT kPacket;
    kPacket.m_dwGuildUID = dwGuildUID_;
    kPacket.m_vecItem.clear();
    kPacket.m_nCharType = static_cast< int >( cCharType_ );

    //time_t tmGuildUID = static_cast< time_t >( dwGuildUID_ );
    time_t tmGuildUID = 0;
    int nData = m_kHelper.GetTodayEventCount( dwUID_, UET_GET_CHANGE_GUILDMARK_ITEM, tmGuildUID );
    if ( 0 < nData ) {
        if ( true == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, SiKGuildManager()->GetGuildMarkItemID(), -1, SiKGuildManager()->GetGuildMarkItemFactor(), KItemManager::EWIT_GUILDITEM, kPacket.m_vecItem ) ) {
            if ( true == m_kHelper.SetTodayEventDone( dwUID_, UET_GET_CHANGE_GUILDMARK_ITEM, tmGuildUID, 0 ) ) {
                QUEUING_ACK_TO_USER_UID( EVENT_UPGRADE_GUILD_GRADE_GIFT_NOT );
            }
        }
        else {
            START_LOG( cerr, L"길드 승격 아이템 지급 오류.. LoginUID: " << dwUID_ << ", GuildUID: " << dwGuildUID_ ) << END_LOG;
        }
    }

    return true;
}

bool KGSDBThread::Update_InfinityDungeon_Stats_ToDB( IN const KDB_EVENT_INFINITY_DUNGEON_STAT_NOT& kDBPacket_ )
{
    /*
    L"{ call dbo.IDGAInfinityDungeon_insert ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d) }"

    { call dbo.IDGAInfinityDungeon_insert ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10, @11, @12, @13, @14, @15 ) }
    @1 ;  ModeID           int         --모드id
    @2 ;  Difficult        int         --모드난이도
    @3 ;  State            int         --게임종료정상처리여부(endgame, 모두leavegame)
    @4 ;  Party            int         --시작시파티원수
    @5 ;  GeneralMonster   int         --소환된일반몬스터총수
    @6 ;  BossMonster      int         --소환된보스몬스터총수(주기적소환)
    @7 ;  RankBossMonster  int         --소환된보스몬스터총수(랭크달성으로소환)
    @8 ;  PlayerTime_1     int         --플레이어플레이시간
    @9 ;  PlayerExp_1      int         --플레이어획득경험치
    @10 ; PlayerTime_2     int  = null --플레이어플레이시간
    @11 ; PlayerExp_2      int  = null --플레이어획득경험치(가변)
    @12 ; PlayerTime_3     int  = null --플레이어플레이시간(가변)
    @13 ; PlayerExp_3      int  = null --플레이어획득경험치(가변)
    @14 ; PlayerTime_4     int  = null --플레이어플레이시간(가변)
    @15 ; PlayerExp_4      int  = null --플레이어획득경험치(가변)

    1 select ( @1 )
    @1  ; OK int
        0        ; 성공
        -901     ; 로그기록실패
    */

    if ( true == kDBPacket_.m_mapGainExp.empty() || true == kDBPacket_.m_mapPlayTime.empty() ) {
        START_LOG( clog, L"무한던전 통계 저장할 정보가 없음" )
            << BUILD_LOG( kDBPacket_.m_mapGainExp.size() )
            << BUILD_LOG( kDBPacket_.m_mapPlayTime.size() )
            << END_LOG;

        return false;
    }

    std::wstringstream stm;
    stm.clear();
    stm.str(L"");

    stm << L"{ call dbo.IDGAInfinityDungeon_insert ( ";
    stm << kDBPacket_.m_nModeID << L",";
    stm << kDBPacket_.m_nDifficulty << L",";
    stm << kDBPacket_.m_nEndState << L",";
    stm << kDBPacket_.m_nStartParty << L",";
    stm << kDBPacket_.m_nSummonedNormal << L",";
    stm << kDBPacket_.m_nSummonedIntervalBoss << L",";
    stm << kDBPacket_.m_nSummonedRankBoss << L",";

    bool bDataAvailable = false;

    std::map< std::pair< DWORD, char >, __int64 >::const_iterator cmitGainExp;
    for ( cmitGainExp = kDBPacket_.m_mapGainExp.begin(); kDBPacket_.m_mapGainExp.end() != cmitGainExp; ++cmitGainExp ) {
        std::map< DWORD, DWORD >::const_iterator cmitPlayTime;
        cmitPlayTime = kDBPacket_.m_mapPlayTime.find( cmitGainExp->first.first );
        if ( kDBPacket_.m_mapPlayTime.end() != cmitPlayTime ) {
            // 획득 경험치 정보와 플레이 시간 정보가 모두 있는 경우에만 DB에 기록을 한다
            bDataAvailable = true;

            if ( kDBPacket_.m_mapGainExp.begin() != cmitGainExp ) {
                stm << L",";
            }
            stm << ( cmitPlayTime->second / ( 60 * 1000 ) ) << L","; // 플레이 시간은 분단위로 기록한다
            stm << cmitGainExp->second;
        }
    }

    stm << L" ) }";

    if ( false == bDataAvailable ) {
        return false;
    }

    DBJIF2( m_kODBC.QueryToWString( stm.str() ) );

    stm.clear();
    stm.str(L"");

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nRet ) );

    _LOG_SUCCESS( nRet == 0, L"무한던전 통계 저장" )
        << BUILD_LOG( nRet )
        << BUILD_LOG( kDBPacket_.m_nModeID )
        << BUILD_LOG( kDBPacket_.m_nDifficulty )
        << BUILD_LOG( kDBPacket_.m_nEndState )
        << BUILD_LOG( kDBPacket_.m_nSummonedNormal )
        << BUILD_LOG( kDBPacket_.m_nSummonedIntervalBoss )
        << BUILD_LOG( kDBPacket_.m_nSummonedRankBoss )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;

    if ( 0 == nRet ) {
        return true;
    }

    return false;
}

bool KGSDBThread::Get_DungeonPersonalRecord_UserData( IN const DWORD dwUID_, IN const char cCharType_ )
{
    KEVENT_DUNGEON_PERSONAL_RECORD_INFO_NOT kPacket;
    kPacket.m_nOK = NetError::NET_OK;
    kPacket.m_cCharType = cCharType_;

    Get_DungeonPersonalRecord_UserData_FromDB( dwUID_, static_cast< int >( cCharType_ ), kPacket.m_mapDungeonPersonalRecord );

    QUEUING_ACK_TO_USER_UID( EVENT_DUNGEON_PERSONAL_RECORD_INFO_NOT );

    return true;
}

bool KGSDBThread::Get_DungeonPersonalRecord_UserData_FromDB( IN const DWORD dwUID_, IN const int nCharType_, OUT std::map< int, std::map< int, KDungeonPersonalRecord > >& mapDungeonPersonalRecord_ )
{
    /*
    L"{ call dbo.ETGAExtinctionTowerPersonal_select ( %d, %d ) }"

    { call dbo.ETGAExtinctionTowerPersonal_select ( @1, @2 ) }
    @1 ; @iLoginUID_    int
    @2 ; @iCharType_    int

    n return ( @1, @2, @3, @4 )
    @1 ; ModeID          int     //던전 번호
    @2 ; StageNum        int     //스테이지 번호
    @3 ; ControlRank     int     //베스트 클리어 랭크 //F = 0, E = 1, D = 2, C = 3, B = 4, A = 5, S = 6, SS = 7
    @4 ; StageClearTime  int     //베스트 클리어 시간 //ms단위 숫자 (1초 = 1000, 1분 = 60초 = 60000 )
    */
    mapDungeonPersonalRecord_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.ETGAExtinctionTowerPersonal_select ( %d, %d ) }",
        dwUID_, nCharType_ ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        int nModeID_ = 0;
        KDungeonPersonalRecord kDungeonPersonalRecord;

        VERIFY_QUERY( m_kODBC >> nModeID_
            >> kDungeonPersonalRecord.m_nStageNum
            >> kDungeonPersonalRecord.m_nControlRank
            >> kDungeonPersonalRecord.m_dwStageClearTime );

        mapDungeonPersonalRecord_[ nModeID_ ][ kDungeonPersonalRecord.m_nStageNum ] = kDungeonPersonalRecord;
    }

    START_LOG( clog, L"던전 개인 최고 기록 조회" )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( nCharType_ )
        << BUILD_LOG( mapDungeonPersonalRecord_.size() )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;

    return true;
}

bool KGSDBThread::Update_DungeonPersonalRecord_UserData_ToDB( IN const DWORD dwUID_, IN const int nCharType_, IN const std::map< int, std::map< int, KDungeonPersonalRecord > >& mapDungeonPersonalRecordDiff_ )
{
    /*
    L"{ call dbo.ETGAExtinctionTowerPersonal_merge ( %d, %d, %d, %d, %d, %d ) }"

    { call dbo.ETGAExtinctionTowerPersonal_merge ( @1, @2, @3, @4, @5, @6 ) }
    @1 ; @iLoginUID_        int
    @2 ; @iCharType_        int
    @3 ; @iModeID_          int   //던전 번호
    @4 ; @iStageNum_        int   //스테이지 번호
    @5 ; @iControlRank_     int   //베스트 클리어 랭크 //F = 0, E = 1, D = 2, C = 3, B = 4, A = 5, S = 6, SS = 7
    @6 ; @iStageClearTime_  int   //베스트 클리어 시간 //ms단위 숫자 (1초 = 1000, 1분 = 60초 = 60000 )

    1 return ( @1 )
    @1: OK       int
        0       : 성공
        -1      : 유저/케릭터 없음
        -801    ; 로그 기록 실패
        -901    ; 데이터 등록 실패
        -902    ; 서버에서 전달해준 ControlRank    값이 더 낮음.
        -903    ; 서버에서 전달해준 StageClearTime 값이 더 높음.
        -904    ; 데이터 갱신 실패
    */
    bool bDBWriteSuccess = true;
    std::map< int, std::map< int, KDungeonPersonalRecord > >::const_iterator cmit1( mapDungeonPersonalRecordDiff_.begin() );
    for ( ; mapDungeonPersonalRecordDiff_.end() != cmit1; ++cmit1 ) {
        std::map< int, KDungeonPersonalRecord >::const_iterator cmit2( cmit1->second.begin() );
        for ( ; cmit1->second.end() != cmit2; ++cmit2 ) {
            DBJIF2( m_kODBC.Query( L"{ call dbo.ETGAExtinctionTowerPersonal_merge ( %d, %d, %d, %d, %d, %d ) }",
                dwUID_,
                nCharType_,
                cmit1->first, // ModeID
                cmit2->first, // StageNum
                cmit2->second.m_nControlRank,
                cmit2->second.m_dwStageClearTime ) );

            int nRet = -99;
            if ( m_kODBC.BeginFetch() ) {
                VERIFY_QUERY( m_kODBC >> nRet );
                m_kODBC.EndFetch();
            }

            LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nRet ) );

            if ( 0 != nRet ) {
                bDBWriteSuccess = false;
                START_LOG( cerr, L"던전 개인 최고 기록 DB 저장 실패: LoginUID: " << dwUID_ << ", ModeID: " << cmit1->first << ", StageNum: " << cmit2->first )
                    << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
            }
        }
    }

    _LOG_SUCCESS( true == bDBWriteSuccess, L"던전 개인 최고 기록 저장" )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( nCharType_ )
        << BUILD_LOG( mapDungeonPersonalRecordDiff_.size() )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;

    return true;
}

bool KGSDBThread::Get_DungeonRank_PreviousSeason_UserRank( IN const DWORD dwUID_ )
{
    KDB_EVENT_DUNGEON_RANK_PREVIOUS_SEASON_USER_RANK_ACK kPacket;
    kPacket.m_nOK = NetError::NET_OK;

    std::set< int > setRankingModes;
    SiKDungeonRankManager()->GetRankingModes( setRankingModes );

    Get_DungeonRank_PreviousSeason_UserRank_FromDB( dwUID_, setRankingModes, kPacket.m_mapPreviousDungeonRankPersonal );

    QUEUING_ACK_TO_USER_UID( DB_EVENT_DUNGEON_RANK_PREVIOUS_SEASON_USER_RANK_ACK );

    return true;
}

bool KGSDBThread::Get_DungeonRank_PreviousSeason_UserRank_FromDB( IN const DWORD dwUID_, IN const std::set< int >& setRankingModes_, OUT std::map< PAIR_CHAR_INT, KDungeonRankInfo >& mapPreviousDungeonRankPersonal_ )
{
    /*
    L"{ call dbo.ETGAExtinctionTowerRankPrevious_Personal_select ( %d, %d ) }"

    { call dbo.ETGAExtinctionTowerRankPrevious_Personal_select ( @1, @2 ) }
    @1 ; @iModeID_    int
    @2 ; @iLoginUID_  int

    n return ( @1, @2, @3, @4, @5 )
    @1 ; Rank             int            
    @2 ; NickName         int
    @3 ; CharType         nvarchar(24)
    @4 ; StageNum         int           //스테이지 번호
    @5 ; StageClearTime   int           //베스트 클리어 시간 //ms단위 숫자 (1초 = 1000, 1분 = 60초 = 60000 )
    */
    mapPreviousDungeonRankPersonal_.clear();

    BOOST_FOREACH( const int& rnModeID, setRankingModes_ ) {
        DBJIF2( m_kODBC.Query( L"{ call dbo.ETGAExtinctionTowerRankPrevious_Personal_select ( %d, %d ) }", rnModeID, dwUID_ ) );

        while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
            KDungeonRankInfo kDungeonRankInfo;
            kDungeonRankInfo.m_dwLoginUID = dwUID_;
            kDungeonRankInfo.m_nModeID = rnModeID;

            VERIFY_QUERY( m_kODBC >> kDungeonRankInfo.m_nRank
                >> kDungeonRankInfo.m_wstrNickName
                >> kDungeonRankInfo.m_cCharType
                >> kDungeonRankInfo.m_nStageNum
                >> kDungeonRankInfo.m_dwStageClearTime );

            PAIR_CHAR_INT prKey( kDungeonRankInfo.m_cCharType, rnModeID );

            mapPreviousDungeonRankPersonal_[ prKey ] = kDungeonRankInfo;
        }

        START_LOG( clog, L"이전 시즌 유저 랭크 조회" )
            << BUILD_LOG( rnModeID )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( mapPreviousDungeonRankPersonal_.size() )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
    }

    return true;
}
