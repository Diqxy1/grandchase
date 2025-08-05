#include <winsock2.h>
#include "CenterDBThread.h"
#include "NetError.h"
#include "CenterServer.h"
#include <cryptopp/hmac.h>
#include <cryptopp/md5.h>
#include <KncSecurity/ByteStream.h>
#include <boost/format.hpp>
#include <time.h>
#include "CnGuildManager.h"
#include <boost/bind.hpp>
#include "Socket/NetCommon.h"
#include <Strsafe.h>
#include "CnSphinx.h"
#include "CnSurvey.h"
#include "CenterSimLayer.h"
#include "CenterLogManager.h"
#include "CnGWCEvent.h"
#include "CnAdventure.h"
#include "CnAccumulateAttendance.h"
#include "CnSocialCommerce.h"
#include "Log4.h"

#define CLASS_TYPE KCenterDBThread

// new guild
bool KCenterDBThread::GetAllGuildInfo( OUT std::map< DWORD, KNGuildInfo >& mapGuildList_ )
{
    // 전체 길드 정보 받아오기 center
    /*
    L"{ call dbo.GSGAGuildSystem_select_all_guild }"

    { call dbo.GSGAGuildSystem_select_all_guild }

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

        SiKCnGuildManager()->MakeGuildMarkName( kTemp.m_dwUID, kTemp.m_nMarkRevision, strExt, kTemp.m_strFileName );

        CTime tDate( tRegDate );
        KSimpleDate kRegDate( tDate.GetYear(), tDate.GetMonth(), tDate.GetDay(), 0 );
        kTemp.m_kFoundingDate = kRegDate;

        mapGuildList_.insert( std::make_pair(kTemp.m_dwUID, kTemp) );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"vecGuildList_.size() : " << mapGuildList_.size() ) << END_LOG;
    return true;
}

bool KCenterDBThread::GetAllGuildNotice( OUT std::map< DWORD, std::vector<KNGuildNotice> >& mapGuildNotice_ )
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

//         START_LOG( clog, strSP )
//             << BUILD_LOG( iGuildID )
//             << BUILD_LOG( kTemp.m_nMsgID )
//             << BUILD_LOG( kTemp.m_strMsg ) << END_LOG;

        mapGuildNotice_[iGuildID].push_back( kTemp );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"mapGuildNotice_.size() : " << mapGuildNotice_.size() ) << END_LOG;
    return true;
}

bool KCenterDBThread::GetAllGuildMemberList( OUT std::map< DWORD, std::vector<KNGuildUserInfo> >& mapGuildMember_ )
{
    // 전체 길드원 center
    /*
    { call dbo.GSGAGuildSystemMember_select_all_guild }

    n select ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10 )
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
    std::wstring strSP;
    DBJIF2( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemMember_select_all_guild }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KNGuildUserInfo kTemp;
        time_t tmJoinDate;
        time_t tmLastLogin;

        VERIFY_QUERY( m_kODBC >> strSP
            >> kTemp.m_dwGuildUID
            >> kTemp.m_dwUserUID
            >> tmJoinDate
            >> kTemp.m_cMemberLevel
            >> kTemp.m_strMyComment
            >> kTemp.m_dwContributePoint
            >> kTemp.m_strLogin
            >> kTemp.m_strNickName
            >> kTemp.m_strNickColor
            >> tmLastLogin );

        // 가입신청일
        CTime tJoinDate( tmJoinDate );
        kTemp.m_kJoinDate.SetDate( tJoinDate.GetYear(), tJoinDate.GetMonth(), tJoinDate.GetDay(), 0 );
        // 마지막접속일
        CTime tLastLogin( tmLastLogin );
        kTemp.m_kLastLoginDate.SetDate( tLastLogin.GetYear(), tLastLogin.GetMonth(), tLastLogin.GetDay(), 0 );

        START_LOG( clog, strSP )
            << BUILD_LOG( kTemp.m_dwGuildUID )
            << BUILD_LOG( kTemp.m_dwUserUID )
            << BUILD_LOG( kTemp.m_kJoinDate.Str() )
            << BUILD_LOGc( kTemp.m_cMemberLevel )
            << BUILD_LOG( kTemp.m_strMyComment )
            << BUILD_LOG( kTemp.m_dwContributePoint )
            << BUILD_LOG( kTemp.m_strLogin )
            << BUILD_LOG( kTemp.m_strNickName )
            << BUILD_LOG( kTemp.m_kLastLoginDate.Str() ) << END_LOG;

        mapGuildMember_[kTemp.m_dwGuildUID].push_back(kTemp);
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"mapGuildMember_.size() : " << mapGuildMember_.size() ) << END_LOG;
    return true;
}

bool KCenterDBThread::GetAllGuildRankData( OUT std::map< DWORD, std::vector<KNGuildRankInfo> >& mapGuildRank_, OUT std::map<DWORD,KSimpleGuildRank>& mapSimpleRank_ )
{
    DBJIF2( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemRank_select_100_guild }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KNGuildRankInfo kTemp;

        int nRevision = 0;
        std::wstring strExt;
        VERIFY_QUERY( m_kODBC >> kTemp.m_nRankType
            >> kTemp.m_nRank
            >> kTemp.m_dwGuildUID
            >> kTemp.m_strName
            >> kTemp.m_dwPoint
            >> kTemp.m_nWin
            >> kTemp.m_nLose
            >> kTemp.m_fRatio
            >> nRevision
            >> strExt );

        SiKCnGuildManager()->MakeGuildMarkName( kTemp.m_dwGuildUID, nRevision, strExt, kTemp.m_strMarkName );

        START_LOG( clog, L"{ call dbo.GSGAGuildSystemRank_select_100_guild }" )
            << BUILD_LOG( kTemp.m_nRankType )
            << BUILD_LOG( kTemp.m_nRank )
            << BUILD_LOG( kTemp.m_dwGuildUID )
            << BUILD_LOG( kTemp.m_strName )
            << BUILD_LOG( kTemp.m_dwPoint )
            << BUILD_LOG( kTemp.m_nWin )
            << BUILD_LOG( kTemp.m_nLose )
            << BUILD_LOG( kTemp.m_fRatio ) << END_LOG;

        mapGuildRank_[ (DWORD)kTemp.m_nRankType ].push_back( kTemp );

        // 길드UID별 랭킹데이터 넣기.
        KSimpleGuildRank kSimpleRank;
        kSimpleRank.m_dwGuildUID = kTemp.m_dwGuildUID;

        std::map<DWORD,KSimpleGuildRank>::iterator mit;
        mit = mapSimpleRank_.insert( std::map<DWORD,KSimpleGuildRank>::value_type( kTemp.m_dwGuildUID, kSimpleRank ) ).first;

        switch( kTemp.m_nRankType )
        {
        case KNGuildRankInfo::WEEKLY_RANK:
            mit->second.m_nWeeklyRank = kTemp.m_nRank;
            break;
        case KNGuildRankInfo::MONTHLY_RANK:
            mit->second.m_nMonthlyRank = kTemp.m_nRank;
            break;
        case KNGuildRankInfo::TOTAL_RANK:
            mit->second.m_nTotalRank = kTemp.m_nRank;
            break;
        case KNGuildRankInfo::GUILD_BATTLE_RANK:
            mit->second.m_nGuildBattleRank = kTemp.m_nRank;
            break;

        default:
            break;
        }
    }
    m_kODBC.EndFetch();

    // 여기서 소팅 한번 해봅시다.
    std::map< DWORD, std::vector<KNGuildRankInfo> >::iterator mit;
    for ( mit = mapGuildRank_.begin() ; mit != mapGuildRank_.end() ; ++mit  ) {

        if ( mit->second.empty() )
            continue;

        std::sort( mit->second.begin(), mit->second.end(),
            boost::bind( &KNGuildRankInfo::m_nRank, _1 ) < boost::bind( &KNGuildRankInfo::m_nRank, _2 ) );
    }

    START_LOG( clog, L"mapGuildRank_.size() : " << mapGuildRank_.size() << L", mapSimpleRank_.size() : " << mapSimpleRank_.size() ) << END_LOG;
    return true;
}

bool KCenterDBThread::UpdateGuildInfoFromDB( OUT std::map<UCHAR,UCHAR>& mapUpdateCheck_ )
{
    // 길드정보갱신 Tick단위. Center
    /*
    { call dbo.GSGAGuildSystemBit_select }

    n select ( @1, @2, @3 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; Type          tinyint
    1 ; 랭킹 갱신 여부
    2 ; 길드마크 갱신 여부
    @3  ; Value         bit
    */
    std::wstring strSP;
    DBJIF2( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemBit_select }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        UCHAR ucType = 0;
        UCHAR ucValue = 0;

        VERIFY_QUERY( m_kODBC >> strSP
            >> ucType
            >> ucValue );

        START_LOG( clog, strSP )
            << BUILD_LOGc( ucType )
            << BUILD_LOGc( ucValue ) << END_LOG;

        mapUpdateCheck_[ ucType ] = ucValue;
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"mapUpdateCheck_.size() : " << mapUpdateCheck_.size() ) << END_LOG;
    return true;
}
bool KCenterDBThread::GetGuildMarkInfo( OUT std::map< DWORD, std::pair<UCHAR,UCHAR> >& mapMarkStatus_ )
{
    // 길드마크 읽어가기 Center
    /*
    L"{ call dbo.GSGAGuildSystem_select_all_guild_mark }"

    { call dbo.GSGAGuildSystem_select_all_guild_mark }

    n select ( @1, @2, @3, @4, @5 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; NID           int
    @3  ; MarkStatus    tinyint
    1 ; 정상상태
    2 ; 마크변경신청상태
    3 ; 승인성공
    4 ; 승인실패
    @4  ; MarkRevision  int
    @5  ; MarkExtention nvarchar

    */
    std::wstring strSP;
    DBJIF2( m_kODBC.Query( L"{ call dbo.GSGAGuildSystem_select_all_guild_mark }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        DWORD dwGuildUID = 0;
        UCHAR ucStatus = 0;
        UCHAR ucRevision = 0;

        VERIFY_QUERY( m_kODBC >> strSP
            >> dwGuildUID
            >> ucStatus
            >> ucRevision );

        START_LOG( clog, strSP )
            << BUILD_LOG( dwGuildUID )
            << BUILD_LOGc( ucStatus )
            << BUILD_LOGc( ucRevision ) << END_LOG;

        mapMarkStatus_[ dwGuildUID ] = std::make_pair( ucStatus, ucRevision );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"mapMarkStatus_.size() : " << mapMarkStatus_.size() ) << END_LOG;
    return true;
}
bool KCenterDBThread::UpdateGuildMarkInfo( IN const DWORD dwGuildUID_ )
{
    // 3,4일 경우 이 SP를 불러서 갱신 Center
    /*
    { call dbo.GSGAGuildSystem_update_Mark_Broadcast ( @1 ) }
    @1 ; @iNID_      int

    1 select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; OK            int
    0        ; 성공
    -1       ; 길드 정보 없음
    -2       ; 길드마크가 승인성공실패상태가 아님
    -101이하 ; 트랜잭션 에러
    */
    std::wstring strSP;
    int nOK = -99;

    DBJIF2( m_kODBC.Query( L"{ call dbo.GSGAGuildSystem_update_Mark_Broadcast ( %d ) }",
        dwGuildUID_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP >> nOK );
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS( nOK == 0, L"nOK : " << nOK )
        << BUILD_LOG( dwGuildUID_ )
        << BUILD_LOG( strSP ) << END_LOG;

    return (nOK == 0);
}

bool KCenterDBThread::UpdateGuildInfoDB( IN const UCHAR ucUpdateType_ )
{
    // DB에 랭킹정보,길드마크정보 갱신여부 알림
    // 최종적으로 1이 된값을 0으로 변경 Center
    /*
    { call dbo.GSGAGuildSystemBit_update ( @1 ) }
    @1 ; @iType_ tinyint
    1 ; 랭킹 갱신 여부
    2 ; 길드마크 갱신 여부

    1 select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; OK            int
    0        ; 성공
    -1       ; 0으로 갱신할 데이터가 없음
    -101이하 ; 트랜잭션 에러
    */
    std::wstring strSP;
    int nOK = -99;

    DBJIF2( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemBit_update ( %d ) }",
        (int)ucUpdateType_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP >> nOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOGc( ucUpdateType_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( strSP ) << END_LOG;

    return (nOK == 0 || nOK == -1 );
}

DWORD KCenterDBThread::GetGuildUIDFromDB( IN const DWORD dwUserUID_ )
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

bool KCenterDBThread::GetGuildMemberListFromDB( IN const DWORD dwGuildUID_, OUT std::map<DWORD, KNGuildUserInfo>& mapUserList_ )
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
    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemMember_select_1_guild ( %d ) }",
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
            time_t tmJoinDate;
            time_t tmLastLogin;

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
                << BUILD_LOG( kTemp.m_kLastLoginDate.Str() ) << END_LOG;

            mapUserList_[ kTemp.m_dwUserUID ] = kTemp;
        }
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"GuildID : " << dwGuildUID_ << L", mapUserList_.size() : " << mapUserList_.size() ) << END_LOG;
    return true;
}

int KCenterDBThread::UpdateGuildGrade( IN const DWORD dwGuildUID_ )
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
    std::wstring strSP;
    int nOK = -99;
    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystem_update_Kind ( %d ) }",
        dwGuildUID_ ), return nOK );

    if ( m_kODBC.BeginFetch() ) {
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

int KCenterDBThread::UpdateGuildDelete( IN const DWORD dwGuildUID_ )
{
    // 길드강제해산 Game
    /*
    { call dbo.GSGAGuildSystem_delete_B ( @1 ) }
    @1 ; @iNID_ int

    1 select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; OK            int
    0        ; 성공
    -1       ; 길드정보 없음
    -101이하 ; 트랜잭션 에러
    OK 값이 0일 경우에만 신뢰할 것
    */
    std::wstring strSP;
    int nOK = -99;
    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystem_delete_B ( %d ) }",
        dwGuildUID_ ), return nOK );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP >> nOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( strSP ) << END_LOG;

    return nOK;
}

bool KCenterDBThread::GetGuildInfoFromDB( IN const DWORD dwGuildUID_, OUT KNGuildInfo& kInfo_ )
{
    // 길드 생성시 길드정보 설정 Game
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
    time_t tRegDate = 0;

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

        SiKCnGuildManager()->MakeGuildMarkName( kInfo_.m_dwUID, kInfo_.m_nMarkRevision, strExt, kInfo_.m_strFileName );
    }
    m_kODBC.EndFetch();

    CTime tDate( tRegDate );
    KSimpleDate kRegDate( tDate.GetYear(), tDate.GetMonth(), tDate.GetDay(), 0 );
    kInfo_.m_kFoundingDate = kRegDate;

    KGuildConfig kConfig;
    SiKCnGuildManager()->GetGuildConfig( kConfig );
    if ( kInfo_.m_ucGrade == KNGuildInfo::GG_NEWBIE && kInfo_.m_nNumMembers >= kConfig.m_nOfficailGuildMemberNum ) {
        // 신생길드가 정규길드 맴버수를 넘으면 정규길드로 전환
        kInfo_.m_ucGrade = KNGuildInfo::GG_OFFICIAL;

        int nRet = UpdateGuildGrade( dwGuildUID_ );
        _LOG_SUCCESS( nRet == 0, L"신규길드 정식길드로 변환. nRet : " << nRet )
            << BUILD_LOG( dwGuildUID_ ) << END_LOG;
    }

    LOG_SUCCESS( dwGuildUID_ == kInfo_.m_dwUID )
        << BUILD_LOG( dwGuildUID_ )
        << BUILD_LOG( kInfo_.m_dwUID )
        << BUILD_LOG( tRegDate )
        << kRegDate.m_sYear << L"-" << (int)kRegDate.m_cMonth << L"-" << (int)kRegDate.m_cDay << dbg::endl
        << BUILD_LOG( kInfo_.m_ucGrade )
        << BUILD_LOG( kInfo_.m_ucJoinSetting )
        << BUILD_LOG( kInfo_.m_ucMarkStatus )
        << BUILD_LOG( kInfo_.m_nMarkRevision )
        << BUILD_LOG( strExt )
        << BUILD_LOG( kInfo_.m_dwExp )
        << BUILD_LOG( kInfo_.m_dwPoint )
        << BUILD_LOG( kInfo_.m_strName )
        << BUILD_LOG( kInfo_.m_strURL )
        << BUILD_LOG( kInfo_.m_nNumMembers )
        << BUILD_LOG( kInfo_.m_strMasterNick )
        << BUILD_LOG( kConfig.m_nOfficailGuildMemberNum )
        << BUILD_LOG( kInfo_.m_GuildBattlePoint ) << END_LOG;

    return true;
}

bool KCenterDBThread::GetGuildMarkInfo( IN const DWORD dwGuildUID_, OUT UCHAR& ucStat_, OUT int& nRevision_, OUT std::wstring& strFileName_ )
{
    ucStat_ = 0xFF;
    nRevision_ = -1;
    strFileName_.clear();

    //L"{ call dbo.GSGAGuildSystem_select_1_guild ( %d ) }"
    //
    //{ call dbo.GSGAGuildSystem_select_1_guild ( @1 ) }
    //@1 ; @iNID_ int
    //
    //1 select ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10, @11, @12, @13, @14 )
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
    //@14 ; BattlePoint   bigint

    std::wstring strDummy;
    std::wstring strExt;

    DBJIF2( m_kODBC.Query( L"{ call dbo.GSGAGuildSystem_select_1_guild ( %d ) }", dwGuildUID_ ) );
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> strDummy
            >> strDummy
            >> strDummy
            >> strDummy
            >> strDummy
            >> ucStat_ >> nRevision_ >> strExt );
        SiKCnGuildManager()->MakeGuildMarkName( dwGuildUID_, nRevision_, strExt, strFileName_ );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L" Guild Mark Info UID : " << dwGuildUID_
        << L", Stat : " << (int)ucStat_
        << L", nRevision : " << nRevision_
        << L", strExt : " << strExt
        << L", FileName : " << strFileName_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (ucStat_ != 0xFF && nRevision_ != -1 && !strFileName_.empty());
}

int KCenterDBThread::UpdateJoinGuild( IN const DWORD dwUserUID_, IN const DWORD dwGuildUID_, IN const std::wstring& strComment_ )
{
    // 길드원 가입(비길드유저 액션) Game
    // JoinType에 따라서 길드원 등급이 0 또는 1이 될 수 있습니다
    /*
    { call dbo.GSGAGuildSystemMember_insert ( @1, @2, @3 ) }
    @1 ; @iLoginUID_   int
    @2 ; @iNID_        int
    @3 ; @strContents_ nvarchar( 100 )

    1 select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; OK            int
    0        ; 성공
    -1       ; 유저정보 없음
    -2       ; 길드정보 없음
    -3       ; 가입중지상태임
    -4       ; 이미 가입유저임
    -101이하 ; 트랜잭션 에러
    */
    std::wstring strSP;
    int nOK = -99;
    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemMember_insert ( %d, %d, N'%s' ) }",
        dwUserUID_, dwGuildUID_, strComment_.c_str() ), return nOK );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP >> nOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( strSP )
        << BUILD_LOG( dwGuildUID_ ) << END_LOG;

    return nOK;
}

bool KCenterDBThread::UpdateJoinGuildMemberInfo( IN const DWORD dwGuildUID_, const DWORD dwUserUID_, OUT KNGuildUserInfo& kInfo_ )
{
    // 길드원 가입시 맴버정보 설정
    /*
    { call dbo.GSGAGuildSystemMember_select_1_member ( @1, @2 ) }
    @1 ; @iNID_      int
    @1 ; @iLoginUID_ int

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
    std::wstring strSP;
    time_t tmJoinDate = time_t(NULL);
    time_t tmLastLogin = time_t(NULL);

    DBJIF2( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemMember_select_1_member ( %d, %d ) }",
        dwGuildUID_, dwUserUID_ ) );

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

    LOG_SUCCESS( kInfo_.m_dwGuildUID == dwGuildUID_ )
        << BUILD_LOG( strSP )
        << BUILD_LOG( dwGuildUID_ )
        << BUILD_LOG( kInfo_.m_dwUserUID )
        << BUILD_LOG( kInfo_.m_kJoinDate.Str() )
        << BUILD_LOGc( kInfo_.m_cMemberLevel )
        << BUILD_LOG( kInfo_.m_strMyComment )
        << BUILD_LOG( kInfo_.m_dwContributePoint )
        << BUILD_LOG( kInfo_.m_strLogin )
        << BUILD_LOG( kInfo_.m_strNickName )
        << BUILD_LOG( kInfo_.m_kLastLoginDate.Str() ) << END_LOG;
    return true;
}

int KCenterDBThread::UpdateChangeGuildMemberLevel( IN const DWORD dwCommenderUserUID_, const DWORD dwTargetUserUID_, const DWORD dwGuildUID_, const UCHAR ucGrade_ )
{
    // 길드원 등급 조정(일반길드원/제너럴 -> 일반길드원/제너럴) Game
    /*
    { call dbo.GSGAGuildSystemMember_update_Grade_A ( @1, @2, @3, @4 ) }
    @1 ; @iLoginUIDA_  int
    @2 ; @iLoginUIDB_  int
    @3 ; @iNID_        int
    @4 ; @iGrade_      tinyint

    1 select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; OK            int
    0        ; 성공
    -1       ; 유저정보 없음
    -2       ; 길드정보 없음
    -3       ; 길드마스터가 아님
    -4       ; 일반길드원이나 제너럴이 아님
    -4       ; 입력인자의 등급이 잘못되었음
    -101이하 ; 트랜잭션 에러
    */
    std::wstring strSP;
    int nOK = -99;

    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemMember_update_Grade_A ( %d, %d, %d, %d ) }",
        dwCommenderUserUID_, dwTargetUserUID_, dwGuildUID_, ucGrade_ ), return nOK );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP >> nOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( strSP ) << END_LOG;

    return nOK;
}

int KCenterDBThread::UpdateChangeGuildMaster( IN const DWORD dwCommenderUserUID_, const DWORD dwTargetUserUID_, const DWORD dwGuildUID_ )
{
    // 길드마스터 이양(일반길드원/제너럴 -> 길드마스터, 길드마스터 -> 제너럴) Game
    /*
    { call dbo.GSGAGuildSystemMember_update_Grade_C ( @1, @2, @3 ) }
    @1 ; @iLoginUIDA_ int
    @2 ; @iLoginUIDB_ int
    @3 ; @iNID_       int

    1 select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; OK            int
    0        ; 성공
    -1       ; 유저정보 없음
    -2       ; 유저정보 없음
    -3       ; 길드정보 없음
    -4       ; 길드마스터가 아님
    -5       ; 일반길드원이나 제너럴이 아님
    -101이하 ; 트랜잭션 에러
    */
    std::wstring strSP;
    int nOK = -99;

    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemMember_update_Grade_C ( %d, %d, %d ) }",
        dwCommenderUserUID_, dwTargetUserUID_, dwGuildUID_ ), return nOK );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP >> nOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( strSP ) << END_LOG;

    return nOK;
}

int KCenterDBThread::UpdateDeleteGuildMember( IN const DWORD dwCommenderUserUID_, const DWORD dwTargetUserUID_, const DWORD dwGuildUID_ )
{
    // 길드원 강제 탈퇴 Game
    /*
    { call dbo.GSGAGuildSystemMember_delete_B ( @1, @2, @3 ) }
    @1 ; @iLoginUIDA_ int
    @2 ; @iLoginUIDB_ int
    @3 ; @iNID_       int

    1 select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; OK            int
    0        ; 성공
    -1       ; 유저정보 없음
    -2       ; 길드정보 없음
    -3       ; 길드마스터가 아님
    -4       ; 가입대기 또는 일반길드원이나 제너럴이 아님
    -101이하 ; 트랜잭션 에러
    */
    std::wstring strSP;
    int nOK = -99;

    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemMember_delete_B ( %d, %d, %d ) }",
        dwCommenderUserUID_, dwTargetUserUID_, dwGuildUID_ ), return nOK );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP >> nOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( strSP ) << END_LOG;

    return nOK;
}

int KCenterDBThread::UpdateAcceptGuildJoiner( IN const DWORD dwCommenderUserUID_, const DWORD dwTargetUserUID_, const DWORD dwGuildUID_ )
{
    // 가입대기길드원 일반길드원으로 등급 변경 Game
    /*
    { call dbo.GSGAGuildSystemMember_update_Grade_B ( @1, @2, @3 ) }
    @1 ; @iLoginUIDA_  int
    @2 ; @iLoginUIDB_  int
    @3 ; @iNID_        int

    1 select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; OK            int
    0        ; 성공
    -1       ; 유저정보 없음
    -2       ; 길드정보 없음
    -3       ; 길드마스터나 제너럴이 아님
    -4       ; 가입대기길드원이 아님
    -101이하 ; 트랜잭션 에러
    */
    std::wstring strSP;
    int nOK = -99;

    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemMember_update_Grade_B ( %d, %d, %d ) }",
        dwCommenderUserUID_, dwTargetUserUID_, dwGuildUID_ ), return nOK );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP >> nOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( strSP ) << END_LOG;

    return nOK;
}

int KCenterDBThread::UpdateDrumoutGuildMember( IN const DWORD dwUserUID_, const DWORD dwGuildUID_ )
{
    // 길드원 자진 탈퇴
    /*
    { call dbo.GSGAGuildSystemMember_delete_A ( @1, @2 ) }
    @1 ; @iLoginUID_ int
    @2 ; @iNID_      int

    1 select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; OK            int
    0        ; 성공
    -1       ; 유저정보 없음
    -2       ; 길드정보 없음
    -3       ; 탈퇴 등급 아님
    -101이하 ; 트랜잭션 에러
    */
    std::wstring strSP;
    int nOK = -99;

    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemMember_delete_A ( %d, %d ) }",
        dwUserUID_, dwGuildUID_ ), return nOK );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP >> nOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( strSP )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOG( dwGuildUID_ ) << END_LOG;

    return nOK;
}

int KCenterDBThread::UpdateBreakupGuild( IN const DWORD dwUserUID_, const DWORD dwGuildUID_ )
{
    // 길드 해산 Game
    /*
    { call dbo.GSGAGuildSystem_delete_A ( @1, @2 ) }
    @1 ; @iLoginUID_ int
    @2 ; @iNID_      int

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
    int nOK = -99;

    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystem_delete_A ( %d, %d ) }",
        dwUserUID_, dwGuildUID_ ), return nOK );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP >> nOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( strSP ) << END_LOG;

    return nOK;
}

void KCenterDBThread::IntervalGuildMarkCheck()
{
    // map< GuildID, pair<마크 상태,마크 리비전> >
    std::map< DWORD, std::pair<UCHAR,UCHAR> >::iterator mit;
    std::map< DWORD, std::pair<UCHAR,UCHAR> > mapMarkStatus;

    // 길드마크 읽어가기
    if ( GetGuildMarkInfo( mapMarkStatus ) == false ) {
        START_LOG( cerr, L"DB에서 전체 길드 마크를 읽어오지 못했습니다." ) << END_LOG;
        return;
    }// end if

    for ( mit = mapMarkStatus.begin() ; mit != mapMarkStatus.end() ; ++mit ) {
        START_LOG( clog, L"GuildUID : " << mit->first << L",Mark Status : " << (int)mit->second.first << L",Mark Revision : " << (int)mit->second.second ) << END_LOG;

        if ( mit->second.first != KNGuildInfo::MS_ACCEPT_CHANGE &&
            mit->second.first != KNGuildInfo::MS_REJECT_CHANGE ) {
            START_LOG( clog, L"길드마크 변경안됨. GuildUID : " << mit->first << L",status : " << (int)mit->second.first ) << END_LOG;
            continue;
        }

        // 길드별로 마크 업데이트 체크
        if ( UpdateGuildMarkInfo( mit->first ) == false ) {
            START_LOG( cerr, L"길드마크 상태 갱신 실패.. GuildUID : " << mit->first << L",status : " << (int)mit->second.first ) << END_LOG;
            continue;
        }

        // 길드 마크 정보를 가져 와서 길드 에게 전송 한다.
        int nCharType = KItem::EICT_NO_TYPE;
        ON_ECN_GUILD_MARK_INFO_UPDATE_REQ( std::wstring(L""), mit->first, static_cast< char >( nCharType ) );
    }// end for

    // 갱신 완료 체크
    if ( UpdateGuildInfoDB( KCnGuildManager::UT_GUILD_MARK ) == false ) {
        START_LOG( cerr, L"갱신정보 저장 실패" ) << END_LOG;
    }// end if

}

void KCenterDBThread::IntervalGuildRankUpdate()
{
    std::map< DWORD, std::vector<KNGuildRankInfo> > mapGuildRank;
    std::map<DWORD,KSimpleGuildRank> mapSimpleRank;
    mapGuildRank.clear();
    mapSimpleRank.clear();

    // 길드랭킹 받아오기, 일/주/월/전체 랭킹이 한번에 리턴
    if ( GetAllGuildRankData( mapGuildRank, mapSimpleRank ) == false ) {
        START_LOG( cerr, L"DB에서 전체 길드 랭킹을 읽어오지 못했습니다." ) << END_LOG;
        return;
    }// end if

    // 갱신 완료 체크
    if ( !UpdateGuildInfoDB( KCnGuildManager::UT_GUILD_RANK ) != 0 ) {
        START_LOG( cerr, L"갱신정보 저장 실패" ) << END_LOG;
    }// end if

    SiKCnGuildManager()->SetGuildRankData( mapGuildRank );
    SiKCnGuildManager()->SetRankInfoInGuildList( mapSimpleRank );
}

int KCenterDBThread::UpdateGuildJoinSetting( IN const DWORD dwUserUID_, 
                                            const DWORD dwGuildUID_, 
                                            const UCHAR ucJoinSetting_ )
{
    // 가입 방법 변경 Game
    /*
    { call dbo.GSGAGuildSystem_update_JoinType ( @1, @2, @3 ) }
    @1 ; @iLoginUID_ int
    @2 ; @iNID_      int
    @3 ; @iJoinType_ tinyint
    1 ; 즉시가입
    2 ; 가입후승인
    3 ; 가입중지

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
    int nOK = -99;

    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystem_update_JoinType ( %d, %d, %d ) }",
        dwUserUID_, dwGuildUID_, ucJoinSetting_ ), return nOK );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strSP >> nOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( strSP ) << END_LOG;

    return nOK;
}

bool KCenterDBThread::GetGuildMasterInfoFromDB( IN const DWORD dwGuildUID_, OUT KNGuildUserInfo& kInfo_ )
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
    std::wstring strSP;
    kInfo_.Init();
    time_t tmJoinDate = time_t(NULL);
    time_t tmLastLogin = time_t(NULL);

    DBJIF2( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemMember_select_1_guildmaster ( %d ) }",
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

    return true;
}

bool KCenterDBThread::GetRemoveNewbieGuildList( OUT std::set<DWORD>& setGuildUID_ )
{
    // 삭제대상 신생길드 GuildUID List를 리턴해줌.
    /*
    { call dbo.GSGAGuildSystem_select_n_guild_NID }

    n select ( @1, @2 )
    @1  ; 호출된 sp이름 nvarchar( 4000 )
    @2  ; NID           int
    */
    std::wstring strSP;
    setGuildUID_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.GSGAGuildSystem_select_n_guild_NID }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        DWORD dwGuildUID = 0;
        VERIFY_QUERY( m_kODBC >> strSP >> dwGuildUID );

        setGuildUID_.insert( dwGuildUID );
        START_LOG( clog, L"Remove List. GuildUID : " << dwGuildUID << L", strSP : " << strSP ) << END_LOG;
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"DB에서 삭제대상 신생길드 GuildUID list 받아옴. size : " << setGuildUID_.size() ) << END_LOG;
    return true;
}

bool KCenterDBThread::GetGuildCommentNoticeFromDB( IN const DWORD dwGuildUID_, OUT std::vector<KNGuildNotice>& vecNotice_ )
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

bool KCenterDBThread::UpdateOneGuildInfo( IN const DWORD dwGuildUID_ )
{
    KNGuildInfo kGuildInfo;
    std::vector<KNGuildNotice> vecNotice;
    KNGuildUserInfo kMasterInfo;
    kGuildInfo.Init();
    kMasterInfo.Init();

    // 길드정보 받아오기
    _JIF( GetGuildInfoFromDB( dwGuildUID_, kGuildInfo ), return false ); // 길드정보
    LIF( GetGuildCommentNoticeFromDB( dwGuildUID_, vecNotice ) ); // 공지
    LIF( GetGuildMasterInfoFromDB( dwGuildUID_, kMasterInfo ) ); // 맴버
    if ( kGuildInfo.m_ucMarkStatus == KNGuildInfo::MS_ACCEPT_CHANGE || kGuildInfo.m_ucMarkStatus == KNGuildInfo::MS_REJECT_CHANGE ) {
        LIF( UpdateGuildMarkInfo( dwGuildUID_ ) ); // 마크갱신
    }

    // 마스터 설정
    kGuildInfo.m_strMasterNick = kMasterInfo.m_strNickName;
    kGuildInfo.m_strMasterNickColor = kMasterInfo.m_strNickColor;

    // 소개말 설정
    std::vector<KNGuildNotice>::iterator vit;
    vit = std::find_if( vecNotice.begin(), vecNotice.end(),
        boost::bind(&KNGuildNotice::m_nMsgID,_1) == KNGuildNotice::GN_COMMENT );
    if ( vit != vecNotice.end() ) {
        kGuildInfo.m_strComment = vit->m_strMsg;
    }

    LIF( SiKCnGuildManager()->AddGuild( kGuildInfo ) ); // 길드 등록
    LIF( SiKCnGuildManager()->AddGuildNotice( dwGuildUID_, vecNotice ) ); // 공지

    START_LOG( clog, L"DB에서 길드정보 받아와서 등록. GuildUID : " << dwGuildUID_ << L",vecNotice.size() : " << vecNotice.size() ) << END_LOG;
    return true;
}

void KCenterDBThread::UpdateOneGuildMemberList( IN const DWORD dwGuildUID_ )
{
    std::map<DWORD, KNGuildUserInfo> mapMemberList;

    LIF( GetGuildMemberListFromDB( dwGuildUID_, mapMemberList ) ); // 맴버
    LIF( SiKCnGuildManager()->SetGuildMemberInfo( dwGuildUID_, mapMemberList ) ); // 맴버리스트 등록

    START_LOG( clog, L"DB에서 길드 맴버리스트 받아와서 등록. GuildUID : " << dwGuildUID_ << L",mapMemberList.size() : " << mapMemberList.size() ) << END_LOG;
}

void KCenterDBThread::IntervalGuildRemveCheck()
{
    // 삭제 처리되야할 길드 체크
    std::set<DWORD> setGuildUID;
    std::set<DWORD>::iterator sit;

    JIF( GetRemoveNewbieGuildList( setGuildUID ) );

    for ( sit = setGuildUID.begin() ; sit != setGuildUID.end() ; ++sit ) {
        KCnGuildPtr spGuild = SiKCnGuildManager()->GetGuild( *sit );

        // 길드 정보 없으면 DB에서 받아서 생성
        if ( spGuild == NULL ) {
            UpdateOneGuildInfo( *sit );
            spGuild = SiKCnGuildManager()->GetGuild( *sit );
        }

        LIF( spGuild );

        // 길드에 삭제체크 전달.
        SiKCnGuildManager()->QueueingID( KCenterEvent::ECN_NEWBIE_GUILD_DESTROY_CHECK_NOT, *sit );
    }

    START_LOG( clog, L"삭제대상 길드 정보 받아옴. size : " << setGuildUID.size() ) << END_LOG;
}

bool KCenterDBThread::IsExistGuild( IN const DWORD dwGuildUID_ )
{
    if ( dwGuildUID_ == 0 ) return false;

    KCnGuildPtr spGuild = SiKCnGuildManager()->GetGuild( dwGuildUID_ );
    if ( spGuild == NULL ) {
        // 길드 정보가 없으면 DB에서 불러오자.
        _JIF( UpdateOneGuildInfo( dwGuildUID_ ), return false );
        UpdateOneGuildMemberList( dwGuildUID_ );

        spGuild = SiKCnGuildManager()->GetGuild( dwGuildUID_ );
        if ( spGuild == NULL ) {
            return false;
        }
    }

    return true;
}

void KCenterDBThread::GetGuildMemeberInfoFromDB( IN DWORD dwGuildUID_, IN DWORD dwUserUID_, OUT KNGuildUserInfo& kGuildUserInfo_, OUT time_t& tmJoinDate_, OUT time_t& tmLastLogin_ )
{
    /*  
    20091109. microcat.  
    */  
    /*  
    L"{ call dbo.GSGAGuildSystemMember_select_1_member ( %d, %d ) }"

    { call dbo.GSGAGuildSystemMember_select_1_member ( @1, @2 ) }  
    @1 ; @iNID_      int  
    @2 ; @iLoginUID_ int  

    1 select ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10 )  
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

    std::wstring strSP;
    DBJIF( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemMember_select_1_member ( %d, %d ) }",
        dwGuildUID_, dwUserUID_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> strSP
            >> kGuildUserInfo_.m_dwGuildUID
            >> kGuildUserInfo_.m_dwUserUID
            >> tmJoinDate_
            >> kGuildUserInfo_.m_cMemberLevel
            >> kGuildUserInfo_.m_strMyComment
            >> kGuildUserInfo_.m_dwContributePoint
            >> kGuildUserInfo_.m_strLogin
            >> kGuildUserInfo_.m_strNickName
            >> kGuildUserInfo_.m_strNickColor
            >> tmLastLogin_ );
    }
    m_kODBC.EndFetch();
}

void KCenterDBThread::GetGuideBookList( IN const DWORD dwUID_, OUT std::map<short,short>& mapList_ )
{
/*
    L"{ call dbo.GBGAGuideBook_select_20130703( %d ) }"  

    { call dbo.GBGAGuideBook_select_20130703 ( @01 ) }  
    @01iLoginUID_input int  
      
    1 return ( @01 )  
    @01iCharType int  
*/
    DBJIF( m_kODBC.Query( L"{ call dbo.GBGAGuideBook_select ( %d ) }", dwUID_ ) );

    short sType = 255;
    short sCharType = -1;

    if ( m_kODBC.BeginFetch() ) {
        int nCharType = -1;
        VERIFY_QUERY( m_kODBC >> sCharType  );
        m_kODBC.EndFetch();
    }

    mapList_.insert( std::make_pair( sType, sCharType ) );

    START_LOG( clog, L" 가이드 북 정보 가져 옮. UID : " << dwUID_ << L", Size : " << mapList_.size() ) << END_LOG;
}

void KCenterDBThread::CheckTotalServerUser( IN DWORD dwBeforeUserNum_ )
{
    /*  
    20100610. microcat.  

    L"{ call dbo.CUGAConcurrentUser_select }"

    { call dbo.CUGAConcurrentUser_select }  

    1 row select
    ( TotalConcurrentUserCNT int )
    */
    DWORD dwTotalServerUser = 0;
    DBJIF( m_kODBC.Query( L"{ call dbo.CUGAConcurrentUser_select }" ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> dwTotalServerUser );
    }
    m_kODBC.EndFetch();

    // 현재 동접 정보를 바로 갱신해 준다.
    SiKCenterServer()->SetTotalServerUser( dwTotalServerUser );

    START_LOG( clog, L"CUGAConcurrentUser_select" )
        << BUILD_LOG( dwTotalServerUser ) << END_LOG;
}

bool KCenterDBThread::GetPackageItem( IN const GCITEMID itemID_, OUT std::vector<KItem>& vecItems_ )
{
    //     mk8253. 2009124.
    //         패키지 아이템인지 체크
    // 
    //         L"{ call dbo.IIGAItemInfoPackage_check( %d ) }"
    // 
    //     { call dbo.IIGAItemInfoPackage_check ( @1 ) }
    //     @1 ; @iItemID_ int
    // 
    //         n return ( @1, @2, @3, @4 )
    //         @1 ; SubItemID
    //         @2 ; SubGrade
    //         @3 ; SubPeriod
    //         @4 ; SubDuration

    DBJIF2( m_kODBC.Query( L"{ call dbo.IIGAItemInfoPackage_check( %d ) }", itemID_ ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KItem kItem;
        VERIFY_QUERY( m_kODBC
            >> kItem.m_ItemID
            >> kItem.m_cGradeID
            >> kItem.m_nPeriod
            >> kItem.m_nCount );

        kItem.m_nInitCount = kItem.m_nCount;

        vecItems_.push_back( kItem );
    }

    return true;
}

int KCenterDBThread::UpdateGuildBattlePoint( IN const DWORD dwGuildUID_, IN const int BPointDiff_ )
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

bool KCenterDBThread::GetGuildPointFromDB( IN const DWORD dwGuildUID_, OUT KNGuildInfo& kInfo_ )
{
    // 길드 생성시 길드정보 설정 Game
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
    time_t tRegDate;

    DBJIF3( m_kODBC.Query( L"{ call dbo.GSGAGuildSystem_select_1_guild ( %d ) }",
        dwGuildUID_ ), return false );

    if ( m_kODBC.BeginFetch() ) {
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
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( dwGuildUID_ == kInfo_.m_dwUID )
        << BUILD_LOG( dwGuildUID_ )
        << BUILD_LOG( kInfo_.m_dwUID )
        << BUILD_LOG( strExt )
        << BUILD_LOG( kInfo_.m_dwExp )
        << BUILD_LOG( kInfo_.m_strName )
        << BUILD_LOG( kInfo_.m_dwPoint )
        << BUILD_LOG( kInfo_.m_GuildBattlePoint ) << END_LOG;

    return true;
}

bool KCenterDBThread::ItemLimit_select( OUT std::set<GCITEMID>& setList_ )
{
    /*
    구매제한 ItemID 리스트
    { call dbo.ILGAItemLimit_select_20110505 }

    n return ( @1 )
    @1: ItemID int
    */
    setList_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.ILGAItemLimit_select_20110505 }" ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMID ItemID = 0;
        VERIFY_QUERY( m_kODBC >> ItemID );
        setList_.insert( ItemID );
    }

    return true;
}

bool KCenterDBThread::UpdateSphinxData( IN const bool& bForce_, IN const int& nVersion_ )
{
    std::map<int,KSphinxQuestionInfo> mapSphinxQuestionInfo;
    bool bChanged = SphinxUpdateCheck( nVersion_ );
    if( !bForce_ && !bChanged )
        return false;

    _JIF( GetSphinxQuestions( nVersion_, mapSphinxQuestionInfo ), return false );

    START_LOG( clog, L"UpdateSphinxData size : " << mapSphinxQuestionInfo.size() ) << END_LOG;
    SiKCnSphinx()->VerifySphinxData( mapSphinxQuestionInfo );
    SiKCnSphinx()->UpdateSphinxData( mapSphinxQuestionInfo );

    return true;
}

bool KCenterDBThread::SphinxUpdateCheck( IN const int& nVersion_ )
{
    if ( !CheckDBUpdate( SPHINX ) ) {
        return false;
    }
    return true;
}

bool KCenterDBThread::GetSphinxQuestions( IN const int& nVersion_, OUT std::map<int,KSphinxQuestionInfo>& mapSphinxQuestionInfo_ )
{
    mapSphinxQuestionInfo_.clear();

    /*
    L"{ call dbo.SQGASphinxQuiz_select_20110517 ( %d ) }"

    { call dbo.SQGASphinxQuiz_select_20110517 ( @1 ) }
    @1: @iVersion_ int
        
    n return ( @1, @2, @3, @4 )
    @1: Level    int
    @2: UID      int
    @3: Question nvarchar( 100 )
    @4: YesorNo  int
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.SQGASphinxQuiz_select_20110517 ( %d ) }", nVersion_ ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KSphinxQuestionInfo kSphinxQuestionInfo;
        VERIFY_QUERY( m_kODBC >> kSphinxQuestionInfo.m_nLevel
                              >> kSphinxQuestionInfo.m_nID
                              >> kSphinxQuestionInfo.m_strQuestion
                              >> kSphinxQuestionInfo.m_nAnswer );

        if ( !mapSphinxQuestionInfo_.insert( std::make_pair( kSphinxQuestionInfo.m_nID, kSphinxQuestionInfo ) ).second ) {
            START_LOG( cerr, L"중복된 질문이 있음. ID : " << kSphinxQuestionInfo.m_nID ) << END_LOG;
        }
    }

    return true;
}

void KCenterDBThread::InsertBlockIPUserLog( IN const std::wstring& wstrLogin_, IN const DWORD dwIP_, IN const std::wstring& wstrIP_, IN const bool& bNewUser_, IN const std::wstring& wstrNationCode_ )
{
    // 로그 파일에 기록하기.
    if( SiKCenterSimLayer()->GetBlockIPUserLogEnable() ) {

        std::wstringstream stm;
        stm << L"|UserID|" << wstrLogin_            // UserID.
            << L"|UserIP|" << wstrIP_               // UserIP.
            << L"|UserDWORDIP|" << dwIP_            // UserDWORDIP.
            << L"|NationCode|" << wstrNationCode_   // UserDWORDIP.
            << L"|NEWUSER|"<< bNewUser_;            // NewUser.

        stm << L"|Time|" << KNC_TIME_STRING;

        SiKCenterLogManager()->Get( KCenterLogManager::LT_BLOCKIPUSER_CONN ) << stm.str() << GLOG::endl;
        stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
        stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.

        START_LOG( clog, L"클라이언트 블럭 ip처리 로그 기록" ) << END_LOG;
    }
}

bool KCenterDBThread::UpdateSurveyData( IN const bool& bForce_, IN const int& nVersion_ )
{
    bool bChanged = SurveyUpdateCheck( nVersion_ );
    if( !bForce_ && !bChanged )
        return false;

    std::map< int, std::map< int, KSurveyQuestion > > mapSurveyAll;
    std::vector< int > vecLanguageInfo;
    SiKCnSurvey()->GetLanguageInfo( vecLanguageInfo );
    
    _JIF( GetSurveys( nVersion_, vecLanguageInfo, mapSurveyAll ), return false );
    SiKCnSurvey()->UpdateSurveyData( mapSurveyAll );

    return true;
}

bool KCenterDBThread::SurveyUpdateCheck( IN const int& nVersion_ )
{
    if ( !CheckDBUpdate( SURVEY ) ) {
        return false;
    }
    return true;
}

bool KCenterDBThread::GetSurveys( IN const int& nVersion_,
                                    IN std::vector< int >& vecLanguageInfo_ ,
                                        OUT std::map< int, std::map< int, KSurveyQuestion > >& mapSurveyInfo_ )
{
    mapSurveyInfo_.clear();
    /*
    L"{ call dbo.SEGASurveyEventQuestion_select ( %d ) }"

    { call dbo.SEGASurveyEventQuestion_select ( @1 ) }
    @1 ; @LanguageID_ tinyint

    n return ( @1, @2)
    @1 ; QuestionUID tinyint
    @2 ; QuestionType tinyint       //객:1-100, 주:101-200, 이:201-300( ex)이미지 답 개수가 3개이면 203 으로 지정, 1개면 201)
    @3 ; Question     nvarchar(240)
    */

    std::vector<int>::iterator vit;
    for ( vit = vecLanguageInfo_.begin(); vit != vecLanguageInfo_.end(); ++vit ) {
        int nLang = (*vit);
        DBJIF2( m_kODBC.Query( L"{ call dbo.SEGASurveyEventQuestion_select ( %d ) }", nLang ) );
        std::map< int, KSurveyQuestion > mapSurveyQuestion;
        while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
            KSurveyQuestion kSurveyQuestion;
            int nQuestionUID;

            VERIFY_QUERY( m_kODBC >> nQuestionUID
                                  >> kSurveyQuestion.m_iQuestionType
                                  >> kSurveyQuestion.m_wsQuestion );

            if ( !mapSurveyQuestion.insert( std::make_pair( nQuestionUID, kSurveyQuestion ) ).second ) {
                START_LOG( cerr, L"중복된 설문 문항이 있음. SurveyID: " << nQuestionUID ) << END_LOG;
            }
        } // end while

        if ( !mapSurveyInfo_.insert( std::make_pair( nLang, mapSurveyQuestion ) ).second ) {
            START_LOG( cerr, L"설문 국가정보 중복이 있음. LanguageID: " << nLang ) << END_LOG;
        }

        START_LOG( clog, L"설문 국가정보.. Ver : " << nVersion_ )
            << BUILD_LOG( nLang )
            << BUILD_LOG( mapSurveyQuestion.size() ) << END_LOG;
    } // end for loop vecLanguageInfo

    /*
    L"{ call dbo.SEGASurveyEventExample_select ( %d ) }"

    { call dbo.SEGASurveyEventExample_select ( @1 ) }
    @1 ; @LanguageID_ tinyint

    n return ( @1, @2, @3 )
    @1 ; QuestionUID tinyint
    @2 ; ExampleNUM  tinyint
    @3 ; ExampleSTR  nvarchar(100)
    */

    for ( vit = vecLanguageInfo_.begin(); vit != vecLanguageInfo_.end(); ++vit ) {
        int nLang = (*vit);
        DBJIF2( m_kODBC.Query( L"{ call dbo.SEGASurveyEventExample_select ( %d ) }", nLang ) );

        while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
            int nQuestionUID;
            int nExample;
            std::wstring wsExample;

            VERIFY_QUERY( m_kODBC >> nQuestionUID
                                  >> nExample
                                  >> wsExample );

            std::map< int, std::map< int, KSurveyQuestion > >::iterator mitTotal;
            mitTotal = mapSurveyInfo_.find( nLang );
            if ( mitTotal != mapSurveyInfo_.end() ) {
                std::map< int, KSurveyQuestion >::iterator mitSurvey;
                mitSurvey = mitTotal->second.find( nQuestionUID );
                if ( mitSurvey != mitTotal->second.end() ) {
                    if ( !mitSurvey->second.m_mapExample.insert( std::make_pair( nExample, wsExample ) ).second ) {
                        START_LOG( cerr, L"중복된 설문 보기 문항이 있음. SurveyID: " << nQuestionUID ) << END_LOG;
                    }
                }
            }
        } // end while

        START_LOG( clog, L"설문보기 문항 국가정보.. Ver : " << nVersion_ )
            << BUILD_LOG( nLang )
            << BUILD_LOG( mapSurveyInfo_.size() ) << END_LOG;
    }

    START_LOG( clog , L"Get Surveys from DB.. Ver : " << nVersion_ )
        << BUILD_LOG( vecLanguageInfo_.size() )
        << BUILD_LOG( mapSurveyInfo_.size() ) << END_LOG;

    return true;
}

bool KCenterDBThread::UpdateGWCEventData()
{
    std::map<DWORD, KGWCCountryInfo> mapTotalCountryInfo;
    std::vector<std::pair<DWORD,DWORD>> vecRankingInfo;
    _JIF( GetGWCTotalCountryInfo( mapTotalCountryInfo, vecRankingInfo ), return false );
    SiKCnGWCEvent()->UpdateGWCTotalCountryInfo( mapTotalCountryInfo, vecRankingInfo );

    return true;
}

bool KCenterDBThread::GetGWCTotalCountryInfo( OUT std::map<DWORD, KGWCCountryInfo>& mapTotalCountryInfo_, OUT std::vector<std::pair<DWORD,DWORD>>& vecRankingInfo_ )
{
    /*
    L"{ call dbo.GWGAGWC2011_select_20111017 }"

    { call dbo.GWGAGWC2011_select_20111017 }

    n return ( @1, @2 )
    @1: CountryID int
    1 :한국
    3 :대만
    4 :브라질
    8 :미국
    9 :필리핀
    11:태국
    12:인도네시아
    13:남미
    @2: CNT       bigint
    */
    mapTotalCountryInfo_.clear();
    vecRankingInfo_.clear();
    std::pair<DWORD,DWORD> prData;

    DBJIF2( m_kODBC.Query( L"{ call dbo.GWGAGWC2011_select_20111017 }" ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KGWCCountryInfo kGWCCountryInfo;
        kGWCCountryInfo.Init();
        VERIFY_QUERY( m_kODBC >> kGWCCountryInfo.m_dwCountryID
                              >> kGWCCountryInfo.m_dwTotalCnt );
        mapTotalCountryInfo_[kGWCCountryInfo.m_dwCountryID] = kGWCCountryInfo;
        prData.first = kGWCCountryInfo.m_dwCountryID;
        prData.second = kGWCCountryInfo.m_dwTotalCnt;
        vecRankingInfo_.push_back( prData );
    }

    return true;
}

void KCenterDBThread::GetOlympicRegDateRankData( OUT std::vector<KRegDateRankInfo>& vecRegDateRank_ )
{
    /*
    L"{ call dbo.LOEVLondonOlympic2012CountryUserRankDisplay_select_20120712 }"

    { call dbo.LOEVLondonOlympic2012CountryUserRankDisplay_select_20120712 }

    n return ( @1, @2, @3, @4, @5 )
    @1: RegDateD  smalldatetime
    yyyy-mm-dd
    @2: GameType  int
    @3: TeamType  int
    @4: CountryID int
    1: 한국
    2: 대만
    3: 브라질
    4: 북미
    5: 필리핀
    6: 태국
    7: 인도네시아
    8: 남미
    @5: Nickname  nvarchar( 24 )
    */

    vecRegDateRank_.clear();

    DBJIF( m_kODBC.Query( L"{ call dbo.LOEVLondonOlympic2012CountryUserRankDisplay_select_20120712 }" ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KRegDateRankInfo kRegDateRankInfo;
        VERIFY_QUERY( m_kODBC >> kRegDateRankInfo.m_tmRegDate
                              >> kRegDateRankInfo.m_nGameType
                              >> kRegDateRankInfo.m_nTeamType
                              >> kRegDateRankInfo.m_nCountryCode 
                              >> kRegDateRankInfo.m_strNickName );
        vecRegDateRank_.push_back( kRegDateRankInfo );
    }
}

bool KCenterDBThread::UpdateAdventureEventData()
{
    int nVersion = -1;
    time_t tmStartDate = 1;
    time_t tmEndDate = 1;
    if ( !GetAdventureEventVersion( nVersion, tmStartDate, tmEndDate ) ) {
        START_LOG( cerr, L"진행중인 모험 이벤트가 없음. DBA님께 기간 설정이 안되었다고 전해주세요" ) << END_LOG;
        return false;
    }

    std::map< int, KAdventureEventDate > mapAdventureEventDate;
    if ( !GetAdventureEventDate( nVersion, mapAdventureEventDate ) ) {
        START_LOG( cerr, L"모험 이벤트 세부 기간 가져오기 오류. DBA님께 기간 설정이 2개라고 전해주세요" ) << END_LOG;
        return false;
    }

    SiKCnAdventure()->SetAdventureEventDate( nVersion, tmStartDate, tmEndDate, mapAdventureEventDate );
    START_LOG( cerr, L"모험 이벤트 기간 가져오기 최종 성공" ) << END_LOG;

    return true;
}

bool KCenterDBThread::GetAdventureEventVersion( OUT int& nVersion_, OUT time_t& tmStartDate_, OUT time_t& tmEndDate_ )
{
    /*
    GC원정대 이벤트 정보 및 날짜 받아오기

    L"{ call dbo.AEGAAdventureEventInfo_Select }"

    { call dbo.AEGAAdventureEventInfo_Select }

    1 return ( @1, @2, @3 )
    @1 ; Version        int
    @2 ; StartDateD     smalldatetime
    @3 ; EndDateD       smalldatetime
    */

    nVersion_ = -1;
    tmStartDate_ = 1;
    tmEndDate_ = 1;

    DBJIF2( m_kODBC.Query( L"{ call dbo.AEGAAdventureEventInfo_Select }" ) );
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nVersion_ >> tmStartDate_ >> tmEndDate_ );
    }
    m_kODBC.EndFetch();

    if ( nVersion_ == -1 ) {
        return false;
    }

    return true;
}

bool KCenterDBThread::GetAdventureEventDate( IN const int nVersion_, OUT std::map< int, KAdventureEventDate >& mapAdventureEventDate_ )
{
    /*
    L"{ call dbo.AEGAAdventureEventDate_Select ( %d ) }"

    { call dbo.AEGAAdventureEventDate_Select ( @1 ) }
    @1 ; @iVersion_ int

    n return ( @1, @2, @3 )
    @1 ; EventID        int
    @2 ; StartDateD     smalldatetime
    @3 ; EndDateD       smalldatetime
    */

    mapAdventureEventDate_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.AEGAAdventureEventDate_Select ( %d ) }", nVersion_ ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KAdventureEventDate kAdventureEventDate;
        kAdventureEventDate.m_nVersion = nVersion_;

        VERIFY_QUERY( m_kODBC >> kAdventureEventDate.m_nEventID
            >> kAdventureEventDate.m_tmStartDate
            >> kAdventureEventDate.m_tmEndDate );
        if ( !mapAdventureEventDate_.insert( std::make_pair( kAdventureEventDate.m_nEventID, kAdventureEventDate ) ).second ) {
            // 맵에 넣으면서 오류.. 중복된 EventID가 존재함
            START_LOG( cerr, L"모험 이벤트 세부 기간 가져오기 오류. DB에 중복된 EventID가 존재함. EventID: ")
                << kAdventureEventDate.m_nEventID << END_LOG;
            return false;
        }
    }
    return true;
}

void KCenterDBThread::GetOlympicTotalRankData( OUT std::vector<KTotalRankInfo>& vecTotalRank_ )
{
    // 올림픽 이벤트 전체 국가 랭킹 정보 가져오기.
    /*
    L"{ call dbo.LOEVLondonOlympic2012CountryMedalDisplay_select_20120712 }"

    { call dbo.LOEVLondonOlympic2012CountryMedalDisplay_select_20120712 }

    8 return ( @1, @2 )
    @1: CountryID int
    1: 한국
    2: 대만
    3: 브라질
    4: 북미
    5: 필리핀
    6: 태국
    7: 인도네시아
    8: 남미
    @2: Point int
    */

    vecTotalRank_.clear();
    int nGrade = 1;
    int nCount = 1;
    int nBeforeGradePoint = 0;

    DBJIF( m_kODBC.Query( L"{ call dbo.LOEVLondonOlympic2012CountryMedalDisplay_select_20120712 }" ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KTotalRankInfo kTotalRankInfo;
        VERIFY_QUERY( m_kODBC >> kTotalRankInfo.m_nCountryCode 
                              >> kTotalRankInfo.m_nGradePoint );

        if( nBeforeGradePoint == kTotalRankInfo.m_nGradePoint ) {
            kTotalRankInfo.m_nGrade = nGrade;
        } else {
            kTotalRankInfo.m_nGrade = nCount;
            nGrade = nCount;
        }
        nBeforeGradePoint = kTotalRankInfo.m_nGradePoint;
        ++nCount;
        vecTotalRank_.push_back( kTotalRankInfo );
    }
}

bool KCenterDBThread::UpdateOlympicTotalData( IN time_t& tmDate_, IN const __int64& biAccumulateData_ )
{
    /*
    L"{ call dbo.LOEVLondonOlympic2012CountryMedal_merge_20120712 ( N'%s', %I64d ) }"

    { call dbo.LOEVLondonOlympic2012CountryMedal_merge_20120712 ( @1, @2 ) }
    @1: @sdtRegDateD_input smalldatetime
    @2: @iMedalCNT_input   bigint

    1 return ( @1 )
    @1: OK int
    0       : 성공
    -101이하: 트랜잭션 에러
    */

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.LOEVLondonOlympic2012CountryMedal_merge_20120712 ( N'%s', %I64d ) }",
        (LPCTSTR)CTime(tmDate_).Format( L"%Y%m%d" ), biAccumulateData_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"유저 올림픽 데이터 정보 갱신" )
        << BUILD_LOG( nOK )
        << BUILD_LOGtm( CTime(tmDate_) )
        << BUILD_LOG( biAccumulateData_ )
        << END_LOG;

    return true;
};

bool KCenterDBThread::UpdateOlympicUserRankData( IN time_t& tmDate_, IN const int& nGameType_, IN const int& nTeamType_, IN const __int64& biAccumulateData_, IN const std::wstring& strNickName_ )
{
    /*
    L"{ call dbo.LOEVLondonOlympic2012CountryUserRank_merge_20120712 ( N'%s', %d, %d, %I64d, N'%s' ) }"

    { call dbo.LOEVLondonOlympic2012CountryUserRank_merge_20120712 ( @1, @2, @3, @4, @5 ) }
    @1: @sdtRegDateD_input smalldatetime
    @2: @iGameType_input   int
    @3: @iTeamType_input   int
    @4: @iMedalCNT_input   bigint
    @5: @strNickname_input nvarchar( 24 )

    1 return ( @1 )
    @1: OK int
    0       : 성공
    -101이하: 트랜잭션 에러
    */

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.LOEVLondonOlympic2012CountryUserRank_merge_20120712 ( N'%s', %d, %d, %I64d, N'%s' ) }",
        (LPCTSTR)CTime(tmDate_).Format( L"%Y%m%d" ), nGameType_, nTeamType_, biAccumulateData_, strNickName_.c_str() ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"유저 올림픽 유저 랭킹 데이터 정보 갱신" )
        << BUILD_LOG( nOK )
        << BUILD_LOGtm( CTime(tmDate_) )
        << BUILD_LOG( nGameType_ )
        << BUILD_LOG( nTeamType_ )
        << BUILD_LOG( biAccumulateData_ )
        << END_LOG;

    return true;
};

void KCenterDBThread::GetOlympicUserData( OUT std::map<time_t, KOlympicInfoData>& mapOlympicInfoData_ )
{
    /*
    L"{ call dbo.LOEVLondonOlympic2012CountryUserRank_select_20120712 }"

    { call dbo.LOEVLondonOlympic2012CountryUserRank_select_20120712 }

    n return ( @1, @2, @3, @4, @5 )
    @1: RegDateD smalldatetime
    @2: GameType int
    @3: TeamType int
    @4: MedalCNT bigint
    @5: Nickname nvarchar( 24 )
    */

    mapOlympicInfoData_.clear();
    DBJIF( m_kODBC.Query( L"{ call dbo.LOEVLondonOlympic2012CountryUserRank_select_20120712 }" ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KOlympicInfoData kOlympicInfoData;
        VERIFY_QUERY( m_kODBC >> kOlympicInfoData.m_tmRegDate 
            >> kOlympicInfoData.m_nGameType
            >> kOlympicInfoData.m_nTeamType
            >> kOlympicInfoData.m_biTotalData
            >> kOlympicInfoData.m_strNickName );
        mapOlympicInfoData_[kOlympicInfoData.m_tmRegDate] = kOlympicInfoData;
    }

    START_LOG( clog, L"올림픽 이벤트 누적치 정보 가져오기 " )
        << BUILD_LOGtm( CTime::GetCurrentTime() )
        << BUILD_LOG( mapOlympicInfoData_.size() )
        << END_LOG;
}

int KCenterDBThread::GetTotalServerCountData( IN time_t tmDate_, IN const DWORD dwType_ )
{
    /*  
    20090116. microcat.  

    L"{ call dbo.SDGA_server_data_select_integer ( N'%s', %d ) }"  

    { call dbo.SDGA_server_data_select_integer ( @1, @2 ) }  
    @1 ; @sdtRegDateD_ [smalldatetime]  ; 일단위까지만 인식함  
    @2 ; @iType_ [tinyint]              ; 서버에서 원하는 숫자별로 분류됨  

    1 return ( @1 )  
    @1 ; Data int  
    */  
    CTime tmTime( tmDate_ );
    int nRes = 0;
    DBJIF2( m_kODBC.Query( L"{ call dbo.SDGA_server_data_select_integer ( N'%s', %d ) }",
        (LPCTSTR)tmTime.Format( L"%Y%m%d" ), dwType_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nRes );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"버프 기부 누적치 정보 가져오기 " )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( nRes )
        << END_LOG;

    return nRes;
}

void KCenterDBThread::UpdateDBSettingInfo_AccAttendance()
{
    int nVersion = -1;
    time_t tmStartDate;
    time_t tmEndDate;
    if ( !GetSettingInfo_AccAttendance( nVersion, tmStartDate, tmEndDate ) ) {
        START_LOG( cerr, L"진행중인 누적 출석 이벤트가 없음. DBA님께 기간 설정이 안되었다고 전해주세요" ) << END_LOG;
    }
    else {
        KECN_ACCUMULATE_ATTENDANCE_SETTING_DATA_NOT kPacket;
        kPacket.m_nVersion = nVersion;
        kPacket.m_tmBegin = tmStartDate;
        kPacket.m_tmEnd = tmEndDate;
        SiKCnAccumulateAttendanceManager()->UpdateAccumulateAttendanceSettingData( kPacket );
        START_LOG( cerr, L"DB로부터 누적 출석 이벤트 정보 가져옴" )
            << BUILD_LOG( nVersion )
            << BUILD_LOGtm( CTime( tmStartDate ) )
            << BUILD_LOGtm( CTime( tmEndDate ) )
            << END_LOG;
    }
}

bool KCenterDBThread::GetSettingInfo_AccAttendance( OUT int& nVersion_, OUT time_t& tmBegin_, OUT time_t& tmEnd_ )
{
/*
    발렌타인이벤트( 화령의축제) 설정정보받아오기

    L"{ call dbo.FSGAFlowerSpiritEventInfo_Select }"

    { call dbo.FSGAFlowerSpiritEventInfo_Select }

    1 return ( @1, @2, @3 )
    @1 ; Version   int
    @2 ; StartDate smalldatetime
    @3 ; EndDate   smalldatetime
*/

    DBJIF2( m_kODBC.Query( L"{ call dbo.FSGAFlowerSpiritEventInfo_Select }" ) );

    // DB에 설정 된 버전/기간을 얻는다
    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nVersion_
            >> tmBegin_
            >> tmEnd_ );
        m_kODBC.EndFetch();
    }

    if ( nVersion_ == -1 ) {
        return false;
    }

    return true;
}

bool KCenterDBThread::GetCashbackDefaultAllRatio( OUT std::map< GCITEMID, int >& mapCashbackDefaultRatio_ )
{
    /*
    캐시백 비율(전체)
    L"{ call dbo.CBGACashBackRate_All_select }"

    { call dbo.CBGACashBackRate_All_select }
        
    n return ( @1, @2 )
    @1: itemID  int
    @2: Rate    int
    */
    mapCashbackDefaultRatio_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.CBGACashBackRate_All_select }" ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMID dwItemID = 0;
        int nRate = 0;

        VERIFY_QUERY( m_kODBC >> dwItemID
            >> nRate );

        mapCashbackDefaultRatio_[ dwItemID ] = nRate;
    }

    START_LOG( clog, L"mapCashbackDefaultRatio_.size(): " << mapCashbackDefaultRatio_.size() ) << END_LOG;

    return true;
}

bool KCenterDBThread::GetCashbackEventAllRatio( OUT time_t& tmStart_, OUT time_t& tmEnd_, OUT std::map< GCITEMID, int >& mapCashbackEventRatio_ )
{
    /*
    캐시백 이벤트 검색
    L"{ call dbo.CBEVCashBackEvent_select ( %d ) }"

    { call dbo.CBEVCashBackEvent_select ( @1 ) }
    @1 ; @bAll_ bit
        0 : 갱신된데이터만읽음
        1 : 전체데이터읽음

    n return ( @1, @2, @3 )
    @1 ; Version        int
    @2 ; StartDateD     smalldatetime
    @3 ; EndDateD       smalldatetime
    */
    mapCashbackEventRatio_.clear();

    int nVersion = -1;
    tmStart_ = LONG_MAX;
    tmEnd_ = LONG_MAX;

    DBJIF2( m_kODBC.Query( L"{ call dbo.CBEVCashBackEvent_select ( %d ) }", 1 ) );

    // SP에서는 n return 이지만 기간이 중복되도록 설정하지 않는 가정을 하여 1 return 처럼 처리한다
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nVersion
            >> tmStart_
            >> tmEnd_ );
        m_kODBC.EndFetch();
    }

    LOG_SUCCESS( nVersion != -1 )
        << BUILD_LOG( nVersion )
        << BUILD_LOGtm( CTime( tmStart_ ) )
        << BUILD_LOGtm( CTime( tmEnd_ ) )
        << END_LOG;

    if ( -1 == nVersion ) {
        START_LOG( clog, L"캐시백 이벤트 비율 이벤트 진행하지 않음" ) << END_LOG;

        return false;
    }

    /*
    캐시백 이벤트 비율(전체)
    L"{ call dbo.CBEVCashBackEventItem_select ( %d ) }"

    { call dbo.CBEVCashBackEventItem_select ( @1 ) }
    @1 ; @iVersion_ int

    n return ( @1, @2 )
    @1 ; ItemID     int
    @2 ; Rate       int
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.CBEVCashBackEvent_select ( %d ) }",
        nVersion ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMID dwItemID = 0;
        int nRate = 0;

        VERIFY_QUERY( m_kODBC >> dwItemID
            >> nRate );

        mapCashbackEventRatio_[ dwItemID ] = nRate;
    }

    START_LOG( clog, L"mapCashbackEventRatio_.size(): " << mapCashbackEventRatio_.size() ) << END_LOG;

    return true;
}

void KCenterDBThread::GetTCPServerList( OUT std::vector<KSimpleServerInfo>& vecUseTCPServerList_, OUT std::vector<KSimpleServerInfo>& vecNoUseTCPServerList_ )
{
    vecUseTCPServerList_.clear();
    vecNoUseTCPServerList_.clear();

    /*
    L"{ call dbo.SMGAServerManagement_select_20130926 }"

    { call dbo.SMGAServerManagement_select_20130926 }

    n select ( @01, @02, @03, @04, @05, @06 )
        @01iUID    int            서버의 고유 번호
    ,   @02strIP   nvarchar( 15 ) 서버의 IP
    ,   @03iPort   int            서버의 Port
    ,   @04iType   int            서버의 종류
    ,   @05iStatus int            서버의 상태( 꺼질때 : 0, 켜질때 : 1 )
    ,   @06iCNT    int            접속 유저 수
    */

    DBJIF( m_kODBCMain.Query( L"{ call dbo.SMGAServerManagement_select_20130926 }" ) );

    while( m_kODBCMain.Fetch() != SQL_NO_DATA )
    {
        KSimpleServerInfo kSimpleServerInfo;

        VERIFY_QUERY( m_kODBCMain >> kSimpleServerInfo.m_dwUID 
            >> kSimpleServerInfo.m_strIP
            >> kSimpleServerInfo.m_usPort
            >> kSimpleServerInfo.m_cServerType
            >> kSimpleServerInfo.m_cServerStatus
            >> kSimpleServerInfo.m_nCurrentConn );

        if( kSimpleServerInfo.m_cServerStatus == KSimpleServerInfo::SS_CLOSE ) {
            vecNoUseTCPServerList_.push_back( kSimpleServerInfo );
        }
        else {
            vecUseTCPServerList_.push_back( kSimpleServerInfo );
        }
    }
    m_kODBCMain.EndFetch();
}

void KCenterDBThread::UpdateSocialCommerceData( IN bool bChanged_ )
{
    std::map<int, KInformationDataOfSellType> mapInfoOfSellType;

    // 판매 DB 설정 얻기
    GetSellSettingInfo_SocialCommerce( mapInfoOfSellType );
    // 보상 DB 설정 얻기(공동구매)
    if( true == bChanged_ ) {
        GetRewardSettingInfo_SocialCommerce( mapInfoOfSellType );

        START_LOG( cerr, L"DB로부터 소셜 커머스 설정 정보 가져옴" )
            << BUILD_LOG( mapInfoOfSellType.size() )
            << END_LOG;
    }
    
    SiKCnSocialCommerce()->UpdateSocialCommerceSettingData( mapInfoOfSellType, bChanged_ );
}

void KCenterDBThread::GetSellSettingInfo_SocialCommerce( OUT std::map<int, KInformationDataOfSellType>& mapInfoOfSellType_ )
{
    /*
    발렌타인 보부상 이벤트 정보 조회
    : 해당 SP 조회시 이벤트 성공 실패 여부에 관계없이 현재 시간에 해당 되는 모든 이벤트를 조회 함
    
    L"{ call dbo.SCGASocialCommerceEventInfo_select }"

    { call dbo.SCGASocialCommerceEventInfo_select }

    n return ( @1, @2, @3, @4, @5, @6, @7 )
    @1 ; EventUID     int
    @2 ; EventType    int   -- 이벤트 종류 ( 1 : 수량제한, 2 : 기간제한, 3 : 공동구매 1 + 1, 4 : 공동 구매 1 + 보너스
    @3 ; StartDate    smalldatetime
    @4 ; EndDate      smalldatetime
    @5 ; ItemID       int   -- 판매 아이템 ( 클라이언트 UI로 띄워줄 ItemID )
    @6 ; SuccessCount int   -- 목표 수량 ( 목표수량이 없을 경우 -1 입력 )
    @7 ; SuccessFlag  int   -- 목표 달성 성공 여부 ( -1 : 실패, 0 : 진행중, 1 : 성공, 2 : 보상 지급까지 완료 )
    */
    mapInfoOfSellType_.clear();

    std::map<DWORD, KSCSellInfo> mapCountLimit;
    std::map<DWORD, KSCSellInfo> mapTimeLimit;
    std::map<DWORD, KSCSellInfo> mapGroupPurchaseOnePlusOne;
    std::map<DWORD, KSCSellInfo> mapGroupPurchaseBonus;

    mapCountLimit.clear();
    mapTimeLimit.clear();
    mapGroupPurchaseOnePlusOne.clear();
    mapGroupPurchaseBonus.clear();

    DBJIF( m_kODBC.Query( L"{ call dbo.SCGASocialCommerceEventInfo_select }" ) );

    // 각 판매타입 별 DB세팅 데이터를 얻는다
    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KSCSellInfo kInfo;
        int nType = KSCSellInfo::SELLTYPE_INVALID;

        VERIFY_QUERY( m_kODBC >> kInfo.m_dwEventUID
            >> nType
            >> kInfo.m_tmBegin
            >> kInfo.m_tmEnd
            >> kInfo.m_dwItemID
            >> kInfo.m_nLimitCount
            >> kInfo.m_nGroupPurchaseFlag );



        switch ( nType ) {
            case KSCSellInfo::SELLTYPE_COUNT_LIMIT:
                mapCountLimit.insert( std::make_pair( kInfo.m_dwEventUID , kInfo ) );
                break;
            case KSCSellInfo::SELLTYPE_TIME_LIMIT:
                mapTimeLimit.insert( std::make_pair( kInfo.m_dwEventUID, kInfo ) );
                break;
            case KSCSellInfo::SELLTYPE_GROUP_PURCHASE_ONE_PLUS_ONE:
                mapGroupPurchaseOnePlusOne.insert( std::make_pair( kInfo.m_dwEventUID, kInfo ) );
                break;
            case KSCSellInfo::SELLTYPE_GROUP_PURCHASE_BONUS:
                mapGroupPurchaseBonus.insert( std::make_pair( kInfo.m_dwEventUID, kInfo ) );
                break;
            default:
                START_LOG( cerr, L"DB에서 얻은 이벤트 타입이 유효하지 않음 Type : " << nType ) << END_LOG;
                break;
        }

        START_LOG( clog, L"소셜 커머스 SellInfo DB데이터 얻는 While문 내부" )
            << BUILD_LOG( kInfo.m_dwEventUID )
            << BUILD_LOG( nType )
            << BUILD_LOGtm( CTime( kInfo.m_tmBegin ) )
            << BUILD_LOGtm( CTime( kInfo.m_tmEnd ) )
            << BUILD_LOG( kInfo.m_dwItemID )
            << BUILD_LOG( kInfo.m_nLimitCount )
            << BUILD_LOG( kInfo.m_nGroupPurchaseFlag )
            << END_LOG;
    }
    m_kODBC.EndFetch();

    // 넷 중에 하나만 진행할 수 있다고 가정하고 로그로 알려주기만 한다
    if ( false == mapCountLimit.empty() ) {
        int nType = KSCSellInfo::SELLTYPE_COUNT_LIMIT;
        KInformationDataOfSellType kSellInfo;
        kSellInfo.m_mapSellItem = mapCountLimit;
        mapInfoOfSellType_.insert( std::make_pair( nType, kSellInfo ) );
    }
    else {
        START_LOG( cwarn, L"[소셜커머스] 수량 판매 DB에 설정 된 데이터가 없음" ) << END_LOG;
    }

    if ( false == mapTimeLimit.empty() ) {
        int nType = KSCSellInfo::SELLTYPE_TIME_LIMIT;
        KInformationDataOfSellType kSellInfo;
        kSellInfo.m_mapSellItem = mapTimeLimit;
        mapInfoOfSellType_.insert( std::make_pair( nType, kSellInfo ) );
    }
    else {
        START_LOG( cwarn, L"[소셜커머스] 기간 판매 DB에 설정 된 데이터가 없음" ) << END_LOG;
    }

    if ( false == mapGroupPurchaseOnePlusOne.empty() ) {
        int nType = KSCSellInfo::SELLTYPE_GROUP_PURCHASE_ONE_PLUS_ONE;
        KInformationDataOfSellType kSellInfo;
        kSellInfo.m_mapSellItem = mapGroupPurchaseOnePlusOne;
        mapInfoOfSellType_.insert( std::make_pair( nType, kSellInfo ) );
    }
    else {
        START_LOG( cwarn, L"[소셜커머스] 1+1 판매 DB에 설정 된 데이터가 없음" ) << END_LOG;
    }

    if ( false == mapGroupPurchaseBonus.empty() ) {
        int nType = KSCSellInfo::SELLTYPE_GROUP_PURCHASE_BONUS;
        KInformationDataOfSellType kSellInfo;
        kSellInfo.m_mapSellItem = mapGroupPurchaseBonus;
        mapInfoOfSellType_.insert( std::make_pair( nType, kSellInfo ) );
    }
    else {
        START_LOG( cwarn, L"[소셜커머스] 보너스 판매 DB에 설정 된 데이터가 없음" ) << END_LOG;
    }

    START_LOG( clog, L"소셜커머스 판매 정보 컨테이너 From DB : " )
        << BUILD_LOG( mapTimeLimit.size() )
        << BUILD_LOG( mapCountLimit.size() )
        << BUILD_LOG( mapGroupPurchaseOnePlusOne.size() )
        << BUILD_LOG( mapGroupPurchaseBonus.size() )
        << BUILD_LOG( mapInfoOfSellType_.size() )
        << END_LOG;
}

void KCenterDBThread::GetRewardSettingInfo_SocialCommerce( OUT std::map<int, KInformationDataOfSellType>& mapInfoOfSellType_ )
{
    /*
    발렌타인 보부상 이벤트 공동구매 모든 보상아이템 정보 조회
    
    L"{ call dbo.SCGASocialCommerceEventRewardItemList_select }"

    { call dbo.SCGASocialCommerceEventRewardItemList_select }
    
    n return ( @1, @2, @3 )
    @1 ; EventUID     int
    @2 ; RewardItemID int   --보상으로 지급되는 ItemID
    @3 ; Period       int   --기간
    @4 ; Duration     int   --수량
    @4 ; EventType    int
    */

    // 여기서 mapInfoOfSellType_.clear() 하면 안됨
    DBJIF( m_kODBC.Query( L"{ call dbo.SCGASocialCommerceEventRewardItemList_select }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KDropItemInfo kInfo;
        int nType = KSCSellInfo::SELLTYPE_INVALID;
        DWORD dwEventUID = 0;

        VEC_REWARD vecReward;
        vecReward.clear();

        VERIFY_QUERY( m_kODBC >> dwEventUID
            >> kInfo.m_ItemID
            >> kInfo.m_nPeriod
            >> kInfo.m_nDuration
            >> nType );

        vecReward.push_back( kInfo );

        // 공동구매가 아닌 판매 타입에 대한 보상 설정 확인
        if ( KSCSellInfo::SELLTYPE_GROUP_PURCHASE_ONE_PLUS_ONE > nType ||
            KSCSellInfo::SELLTYPE_GROUP_PURCHASE_BONUS < nType ) {
            START_LOG( cerr, L"[소셜커머스] 공동구매가 아닌데 보상이 설정 됨! 기획자분에게 알리세요 Type : " << nType ) << END_LOG;
            continue;
        }

        // 판매정보 컨테이너에 타입에 해당하는 판매 정보가 있는지 확인한다
        // 1 : 수량제한, 2 : 기간제한, 3 : 공동구매 (1 + 1), 4 : 공동 구매 (1 + 보너스)
        std::map<int, KInformationDataOfSellType>::iterator mit;
        mit = mapInfoOfSellType_.find( nType );
        if ( mapInfoOfSellType_.end() == mit ) {
            START_LOG( cerr, L"[소셜커머스] DB에서 얻은 이벤트 타입이 판매정보 컨테이너에 없음 Type : " << nType ) << END_LOG;
            continue;
        }

        std::map<DWORD, VEC_REWARD>::iterator mitReward;
        // 해당 판매 정보에 보상 아이템 정보 확인 후 없으면 <EventID, VEC_REWARD> 컨테이너에 삽입, 있으면 VEC_REWARD에 삽입
        mitReward = mit->second.m_mapRewardItem.find( dwEventUID );
        if ( mit->second.m_mapRewardItem.end() == mitReward ) {
            mit->second.m_mapRewardItem.insert( std::make_pair( dwEventUID, vecReward ) );
        }
        else {
            mitReward->second.push_back( kInfo );
        }

        START_LOG( clog, L"소셜커머스 보상 정보 설정 : " )
            << BUILD_LOG( mitReward->second.size() )
            << BUILD_LOG( dwEventUID )
            << BUILD_LOG( nType )
            << BUILD_LOG( kInfo.m_ItemID )
            << BUILD_LOG( kInfo.m_nPeriod )
            << BUILD_LOG( kInfo.m_nDuration )
            << END_LOG;
    }
}

void KCenterDBThread::GetRewardUserInfo_SocialCommerce( IN DWORD& dwEventUID_, IN int& nCount_, IN VEC_REWARD& vecReward_, OUT VEC_GROUP_PURCHASE_REWARD& vecRewardInfo_ )
{
    /*
    발렌타인 보부상 이벤트 공동구매 유저 리스트 조회

    L"{ call dbo.SCGASocialCommerceEventGroupPurchaseList_Select ( %d, %d ) }"

    { call dbo.SCGASocialCommerceEventGroupPurchaseList_Select ( @1, @2 ) }
    @1 ; @iEventUID_       int
    @2 ; @iReturnRowCount_ int   -- SP 호출 시 리턴되는 Row의 수

    n return ( @1, @2 )
    @1 ; RewardUID  int
    @2 ; LoginUID   int
    */

    vecRewardInfo_.clear();
    DBJIF( m_kODBC.Query( L"{ call dbo.SCGASocialCommerceEventGroupPurchaseList_Select ( %d, %d ) }", dwEventUID_, nCount_ ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KRewardUserListDataForSocialCommerce kData;
        VERIFY_QUERY( m_kODBC >> kData.m_dwRewardUID >> kData.kProvideData.m_dwUserUID );
        kData.kProvideData.m_vecRewardItem = vecReward_;

        vecRewardInfo_.push_back( kData );
    }

    _LOG_SUCCESS( false == vecRewardInfo_.empty(), L"공동구매 신청 유저 리스트 얻기" )
        << BUILD_LOG( vecRewardInfo_.size() )
        << BUILD_LOG( dwEventUID_ )
        << BUILD_LOG( nCount_ )
        << END_LOG;
}

bool KCenterDBThread::UpdateGroupPurchaseRewardComplete_SocialCommerce( IN DWORD& dwEventUID_, IN DWORD& dwRewardUID_, IN DWORD& dwLoginUID_ )
{
    /*
    발렌타인 보부상 이벤트 공동구매 유저 리스트 삭제

    L"{ call dbo.SCGASocialCommerceEventGroupPurchaseDeleteList_Delete ( %d, %d, %d ) }"

    { call dbo.SCGASocialCommerceEventGroupPurchaseDeleteList_Delete ( @1, @2, @3 ) }
    @1: @iEventUID_  int
    @2: @iRewardUID_ int
    @3: @iLoginUID_  int
    
        
    1 return ( @1 )
    @1: OK       int
        0       : 성공
        -1      : 유저가 없음
        -2      : 요청한 지급 리스트가 존재하지 않음
        -101이하: 트랜젝션 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.SCGASocialCommerceEventGroupPurchaseDeleteList_Delete ( %d, %d, %d ) }",
        dwEventUID_, dwRewardUID_, dwLoginUID_ ) );

    int nOK = -99;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS( nOK == 0, L"공동구매 보상 아이템 지급 완료 DB 기록 nOK : " << nOK )
        << BUILD_LOG( dwEventUID_ )
        << BUILD_LOG( dwRewardUID_ )
        << BUILD_LOG( dwLoginUID_ )
        << END_LOG;

    return (nOK == 0);
}

bool KCenterDBThread::InsertToWaitItemJob( IN const DWORD dwUID_, IN const GCITEMID itemID_,
                                          IN const int nCount_, IN const int nPeriod_, IN const int nGetType_, OUT __int64& waitUID_, IN int nGrade_/*=-1*/, IN const int nItemLevel_/*=-1*/, IN const int nEnchantLevel_/*=-1*/ )
{
    /*
    구매외 아이템 획득

    L"{ call dbo.WIGAWaitItem_wait_insert ( %d, %d, %d, %d, %d, %d, %d, %d ) }"

    { call dbo.WIGAWaitItem_wait_insert ( @1, @2, @3, @4, @5, @6, @7, @8 ) }
    @1 ; @iLoginUID_     [int]
    @2 ; @iItemID_       [int]
    @3 ; @iGradeID_      [smallint]
    @4 ; @iPeriod_       [int]
    @5 ; @iDuration_     [smallint]
    @6 ; @iGetType_      [tinyint]
    @7 ; @iItemLevel_    [smallint]
    @8 ; @iStrengthLevel_[int]

    1 return ( @1 )
    @1 ; OK
    0        ; 성공
    -1       ; 유저 정보가 없음
    -101이하 ; 트랜젝션 에러
    @2 ; WIGAUID
    */

    int nOK = -99;
    DBJIF3( m_kODBC.Query( L"{ call dbo.WIGAWaitItem_wait_insert ( %d, %d, %d, %d, %d, %d, %d, %d ) }",
        dwUID_, itemID_, nGrade_, nPeriod_, nCount_, nGetType_, nItemLevel_, nEnchantLevel_ ), return false );

    if ( m_kODBC.BeginFetch() == true ) {
        VERIFY_QUERY( m_kODBC
            >> nOK
            >> waitUID_ );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK >= 0, L"대기 아이템으로 추가. OK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( waitUID_ ) << END_LOG;

    return (nOK >= 0);
}
