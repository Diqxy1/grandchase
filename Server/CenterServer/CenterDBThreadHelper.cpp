#include <winsock2.h>
#include "CenterDBThread.h"
#include "NetError.h"
#include "CenterServer.h"
#include <cryptopp/hmac.h>
#include <cryptopp/md5.h>
#include <KncSecurity/ByteStream.h>
#include <boost/format.hpp>
#include <time.h>
#include "CnRainBow.h"
#include "CnMissionEvent.h"
#include <boost/bind.hpp>
#include "CnQuizBingo.h"
#include "Log4.h"
#include "CnDungeonRankManager.h"
#include <boost/foreach.hpp>

#define CLASS_TYPE KCenterDBThread

void KCenterDBThread::UpdateMiniGameRank()
{
    //게임모드별 BestPoint 상위 Top 100을 리턴합니다.
    //call dbo.MGG_MiniGame_Top100_Select
    //call dbo.MGG_MiniGame_Top100_Select => 
    //return n loop select
    //     GameMode    tinyint
    //,    LoginUID    int
    //,    Nick        nvarchar(24)
    //,    CharType    tinyint
    //,    BestPoint   int 
    std::map< char, std::vector<KMiniGameRankInfo> > mapMiniGameRank;
    //vecMiniGameRank.reserve( MINIGAMEMODE * RANK_ARRANGE );

    DBJIF( m_kODBC.Query( L"{ call dbo.MGG_mini_game_top100_select }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KMiniGameRankInfo kMiniGameInfo;
        KMiniGameRankInfo kMini;
        VERIFY_QUERY( m_kODBC >> kMiniGameInfo.m_cGameMode
            >> kMiniGameInfo.m_dwUserUID
            >> kMiniGameInfo.m_strNick
            >> kMiniGameInfo.m_cCharType
            >> kMiniGameInfo.m_dwBestScore );

        kMini.m_dwScore = kMiniGameInfo.m_dwBestScore; // 최고점수를 얻어와서  일반 점수에 넣는다는 것에 주의
        kMini.m_cGameMode = kMiniGameInfo.m_cGameMode;
        kMini.m_strNick = kMiniGameInfo.m_strNick;
        kMini.m_dwUserUID = kMiniGameInfo.m_dwUserUID;

        if( kMini.m_dwScore > 9999 ) // 최고 점수 제한 ( 그렇지만 이건 디스플레이에만 영향을 주게 된다는 것..)
            kMini.m_dwScore = 9999;

        if( kMini.m_dwScore > 0 ) // 최고점수가 0 이상일 때만 랭킹에 포함
        {
            mapMiniGameRank[kMini.m_cGameMode].push_back( kMini );
        }
    }
    m_kODBC.EndFetch();

    SiKCenterServer()->SetMiniGameRank( mapMiniGameRank );
}

void KCenterDBThread::GetDonationRanking( OUT std::vector<KDotantionRank>& vecRank_ )
{
    //{ call dbo.CGGA_contribution_gp_select_top5 }
    //인자없음
    //
    //5 return ( @1 )
    //@1 ; NickName nvarchar(24)
    //@2 ; Contribution int
    DBJIF( m_kODBC.Query( L"{ call dbo.CGGA_contribution_gp_select_top5 }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KDotantionRank donationRank;
        VERIFY_QUERY( m_kODBC >> donationRank.m_strNickName
            >> donationRank.m_nAccDonationPoint );

        vecRank_.push_back( donationRank );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"기부 랭크 얻어 오기. Size : " << vecRank_.size() ) << END_LOG;
}

void KCenterDBThread::AddServerCountData( int nData_, int nType_ )
{
    //{ call dbo.SDGA_server_data_merge_integer ( @1, @2, @3 ) }
    //@1 ; @sdtRegDateD_ [smalldatetime]  ; 일단위까지만 인식함
    //@2 ; @iType_ [tinyint]              ; 서버에서 원하는 숫자별로 분류됨
    //@3 ; @iData_ [int]                  ; 차이값을 주쇼

    //1 return ( @1 )
    //@1 ; OK int
    //0        ; 성공
    //-101이하 ; 트랜잭션 에러
    DBJIF( m_kODBC.Query( L"{ call dbo.SDGA_server_data_merge_integer ( N'%s', %d, %d ) }",
        (LPCTSTR)CTime::GetCurrentTime().Format( L"%Y%m%d" ), nType_, nData_ ) );

    int nOK = -99;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
}

bool KCenterDBThread::AddServerCountData( __int64 biData_, int nType_, IN time_t tmDate_ )
{
    //{ call dbo.SDGA_server_data_merge_integer ( @1, @2, @3 ) }
    //@1 ; @sdtRegDateD_ [smalldatetime]  ; 일단위까지만 인식함
    //@2 ; @iType_ [tinyint]              ; 서버에서 원하는 숫자별로 분류됨
    //@3 ; @iData_ [int]                  ; 차이값을 주쇼

    //1 return ( @1 )
    //@1 ; OK int
    //0        ; 성공
    //-101이하 ; 트랜잭션 에러
    CTime tmTime( tmDate_ );
    DBJIF2( m_kODBC.Query( L"{ call dbo.SDGA_server_data_merge_integer ( N'%s', %d, %d ) }",
        (LPCTSTR)tmTime.Format( L"%Y%m%d" ), nType_, biData_ ) );

    int nOK = -99;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    if( nOK == 0 ) return true;
    return false;
}

void KCenterDBThread::GetServerList( IN const int nProtocol_, OUT std::vector<KServerInfo>& vecServerList_ )
{
    vecServerList_.clear();

    /*
    L"{ call dbo.CUGAConcurrentUser_select_All_by_ProtocolVersion ( %d ) }"

    { call dbo.CUGAConcurrentUser_select_All_by_ProtocolVersion ( @1 ) }
    @1 ; @iProtocolVersion_ int

    n select ( @1, @2, @3, @4, @5, @6, @7 )
    @1  ; ServerPart smallint
    @2  ; ServerName nvarchar( 20 )
    @3  ; ServerIP   varchar ( 25 ) // 인증용 ip.
    @4  ; ServerPort int
    @5  ; UserNum    int
    @6  ; MaxNum     int
    @7  ; NO         int
    @8  ; ServerIP2  varchar ( 25 ) // 전달용 ip.
    */

    _DBJIF( m_kODBCMain.Query( L"{ call dbo.CUGAConcurrentUser_select_All_by_ProtocolVersion ( %d ) }",
        nProtocol_ ), return, (&m_kODBCMain) );

    while( m_kODBCMain.Fetch() != SQL_NO_DATA )
    {
        KServerInfo kServerInfo;
        kServerInfo.m_nUserProtocolVer = nProtocol_;

        VERIFY_QUERY( m_kODBCMain >> kServerInfo.m_iServerPart 
            >> kServerInfo.m_strName
            >> kServerInfo.m_strIP
            >> kServerInfo.m_usPort
            >> kServerInfo.m_iUserNum
            >> kServerInfo.m_iMaxUserNum
            >> kServerInfo.m_dwUID
            >> kServerInfo.m_strConnIP );

        vecServerList_.push_back( kServerInfo );
    }
    m_kODBCMain.EndFetch();

    //{ call dbo.SIGAServerInfo_select }
    //n return ( @1, @2, @3 )
    //    @1 ; NO
    //    @2 ; MinLevel
    //    @3 ; MaxLevel

    std::vector<KServerInfo>::iterator vit;
    _DBJIF( m_kODBCMain.Query( L"{ call dbo.SIGAServerInfo_select }" ), return, (&m_kODBCMain) );
    while( m_kODBCMain.Fetch() != SQL_NO_DATA )
    {
        DWORD dwUID;
        std::pair<int,int> prRange;
        VERIFY_QUERY( m_kODBCMain >> dwUID
            >> prRange.first
            >> prRange.second );
        vit = std::find_if( vecServerList_.begin(), vecServerList_.end(),
                boost::bind( &KServerInfo::m_dwUID, _1 ) == dwUID );
        if ( vit != vecServerList_.end() ) {
            vit->m_prLvRange = prRange;
        }
    }
    m_kODBCMain.EndFetch();
}

void KCenterDBThread::UpdateChannelNews()
{
    /*{ call dbo.GNGA_game_notice_select_No }
    인자없음
    n return ( @1 )
    @1 ; No int ; 공지 식별자

    호출간격 : 매분

    { call dbo.GNGA_game_notice_select_message ( @1 ) }
    @1 ; No int

    1 return ( @1, @2 )
    @1 ; Message nvarchar(256)
    @2 ; IntervalM int ; 분단위

    호출 간격 : 메세지가 있는 경우 호출
    공지 출력 시각은 db에서 제어하겠음
    서버 및 클라이언트는 메세지 뿌리는 것에만 집중해주삼 */
    std::vector<int> vecNewsUID;
    std::vector< std::pair< int, std::wstring > > vecChannelNews;

    // 디비에서 uid 들을 가져온다
    DBJIF( m_kODBC.Query( L"{ call dbo.GNGA_game_notice_select_No }" ) );
    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        int nNoticeUID = -1;

        VERIFY_QUERY( m_kODBC >> nNoticeUID );
        vecNewsUID.push_back( nNoticeUID );
    }
    START_LOG( clog, L"Channel News UID 받아옴. UID Size : " << vecNewsUID.size() ) << END_LOG;

    for( std::vector<int>::iterator vit = vecNewsUID.begin() ; vit != vecNewsUID.end() ; ++vit )
    {
        int nPeriod = -1; // 전송 주기(분단위)
        std::wstring wstrNotice; // 공지 메세지

        DBJIF( m_kODBC.Query( L"{ call dbo.GNGA_game_notice_select_Message ( %d ) }", *vit ) );
        if( m_kODBC.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBC >> wstrNotice >> nPeriod );
            m_kODBC.EndFetch();
        }
        if( nPeriod <= -1 )
        {
            vecChannelNews.push_back( std::make_pair( nPeriod, wstrNotice ) );
        }
    }

    // 그렇다면  periodNotice 의 메세지를 바꿔준다.
    //SiKPeriodNotice()->UpdatePeriodNotice( vecNotice );
    SiKCenterServer()->UpdateChannelNews( vecChannelNews );
    START_LOG( clog, L"Channel News 업데이트 완료. Notice Size : " << vecChannelNews.size() << L", UID Size : " << vecNewsUID.size() ) << END_LOG;
}

std::wstring KCenterDBThread::GenInternalPasswd( IN const std::wstring strData_, IN const size_t nReturnLength_ )
{
    size_t nByteSize = nReturnLength_ / 2;   // 리턴되는 스트링 사이즈는 얻어낸 hash코드의 2배 길이.

    boost::wformat fmt( L"%s_%s_%d" );  // source = input string + date + system tick.

    fmt % strData_ 
        % (const wchar_t*)CTime::GetCurrentTime().Format( KNC_TIME_FORMAT ) 
        % GetTickCount();

    std::wstring strBuff = fmt.str();

    KncSecurity::KByteStream bsData;
    bsData.assign( (byte*)&strBuff[0], strBuff.size() * sizeof(wchar_t) );

    //std::wcout << L"Source : " << strBuff << std::endl;
    //std::cout << "bsData : " << bsData << std::endl;

    KncSecurity::KByteStream bsAuthKey;
    bsAuthKey.Assign( "니들이 키값을 알아?", 8 );

    CryptoPP::HMAC<CryptoPP::MD5> mac( &bsAuthKey[0], bsAuthKey.length() );

    KncSecurity::KByteStream icv;
    icv.resize( nByteSize );

    mac.Update( bsData.data(), bsData.length() );
    mac.TruncatedFinal( &icv[0], nByteSize );

    std::wostringstream stm;

    KncSecurity::KByteStream::iterator itr;
    for( itr = icv.begin(); itr != icv.end(); itr++ )
    {
        stm << boost::wformat( L"%02x" ) % *itr;
    }

    //std::cout << "key : " << bsAuthKey << std::endl
    //          << "icv : " << icv << std::endl;

    return stm.str();
}

int KCenterDBThread::CheckRainbowEvent( IN const bool bFirst_, OUT time_t& tmBegin_, OUT time_t& tmEnd_ )
{
    //{ call dbo.REEV_rainbow_event_select ( @1 ) }
    //@1 ; @bAll_ bit
    //0 ; 갱신된 데이터만 읽음
    //1 ; 전체 데이터 읽음
    //
    //n return ( @1, @2, @3 )
    //@1 ; EventUID        int
    //@2 ; EventStartDateD smalldatetime
    //@3 ; EventEndDateD   smalldatetime

    // 반환 되는 시간을 사용할 이유는 없다.
    // 전체 리스트를 다시 받게 되니까..

    int nRet = -1;
    DBJIF3( m_kODBC.Query( L"{ call dbo.REEV_rainbow_event_select ( %d ) }",
        (bFirst_?1:0) ), return nRet );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        VERIFY_QUERY( m_kODBC >> nRet
            >> tmBegin_
            >> tmEnd_ );
    }

    return nRet;
}

bool KCenterDBThread::GetRainbowDailyEvent( IN const int nRainbowID_, OUT std::vector<KDailyEventData>& vecDailyEvent_ )
{
    vecDailyEvent_.clear();
    if( nRainbowID_ <0 ) return false;

    //{ call dbo.REEV_rainbow_event_select_daily ( @1 ) }
    //@1 ; @iEventUID_ int
    //
    //n return ( @1, @2, @3, @4, @5 )
    //@1 ; EventID       tinyint
    //@2 ; EventRegDateD smalldatetime
    //@3 ; ServerEventID tinyint
    //@4 ; Factor1       int
    //@5 ; Factor2       int

    DBJIF2( m_kODBC.Query( L"{ call dbo.REEV_rainbow_event_select_daily ( %d ) }",
        nRainbowID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KDailyEventData kData;
        VERIFY_QUERY( m_kODBC >> kData.m_nDailyID
            >> kData.m_tmEventDate
            >> kData.m_nEventID 
            >> kData.m_fPrimaryRatio
            >> kData.m_fSecondaryRatio ); 
        kData.m_fPrimaryRatio = static_cast<float>( kData.m_fPrimaryRatio * 0.01 );
        kData.m_fSecondaryRatio = static_cast<float>( kData.m_fSecondaryRatio * 0.01 );

        vecDailyEvent_.push_back( kData );
    }

    return ( !vecDailyEvent_.empty() );
}

bool KCenterDBThread::GetJackpotEventList( IN const int nRainbowID_, OUT std::vector< KRainBowData >& vecJackpotEventList_ )
{
    //{ call dbo.REEV_rainbow_event_select_daily ( @1 ) }
    //@1 ; @iEventUID_ int
    //
    //n return ( @1, @2, @3, @4, @5 )
    //@1 ; EventID       tinyint
    //@2 ; EventRegDateD smalldatetime
    //@3 ; ServerEventID tinyint
    //@4 ; Factor1       int
    //@5 ; Factor2       int

    DBJIF2( m_kODBC.Query( L"{ call dbo.REEV_rainbow_event_select_eventgroup ( %d ) }",
    nRainbowID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KRainBowData kData;
        VERIFY_QUERY( m_kODBC >> kData.m_nEventID
            >> kData.m_fPrimaryRatio
            >> kData.m_fSecondaryRatio );
        kData.m_fPrimaryRatio = static_cast<float>( kData.m_fPrimaryRatio * 0.01 );
        kData.m_fSecondaryRatio = static_cast<float>( kData.m_fSecondaryRatio * 0.01 );

        vecJackpotEventList_.push_back( kData );
    }

    return ( !vecJackpotEventList_.empty() );
}

bool KCenterDBThread::GetJackpotTimeList( IN const int nRainbowID_, OUT std::vector< KJackpotTimeList >& vecJackpotTimeList_ )
{

    vecJackpotTimeList_.clear();

    //{ call dbo.REEV_rainbow_event_select_jackpot ( @1 ) }
    //@1 ; @iEventUID_ int
    //
    //n return ( @1, @2, @3, @4 )
    //@1 ; EventID           tinyint
    //@2 ; JackpotID         tinyint
    //@3 ; JackpotStartDateB smalldatetime
    //@4 ; JackpotEndDateB   smalldatetime

    DBJIF2( m_kODBC.Query( L"{ call dbo.REEV_rainbow_event_select_jackpot ( %d ) }",
    nRainbowID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KJackpotTimeList kData;
        VERIFY_QUERY( m_kODBC >> kData.m_nDailyID
            >> kData.m_nJackpotID
            >> kData.m_tmBegin
            >> kData.m_tmEnd );
        vecJackpotTimeList_.push_back( kData );
    }
    return ( !vecJackpotTimeList_.empty() );
}

bool KCenterDBThread::CheckDBUpdate( IN int kind_ )
{
    /*
    L"{ call dbo.GVGA_global_value_select_bit ( %d ) }"

    { call dbo.GVGA_global_value_select_bit ( @1 ) }
    @1: 번호int
        1: 아이템정보갱신여부    : GoodsInfoList
        2: 원어데이정보갱신어부  : ODDBOneaDay
        3: 아이템구매제한갱신여부 : ILGAItemLimit
        4: 스핑크스퀴즈갱신여부  : SQCDSphinxQuiz
        5: 설문문제정보갱신여부 : SEGASurveyEventQuestion, SEGASurveyEventExample
        6: 모험이벤트정보갱신여부: AEGAAdventureEventInfo
        7: 런던올림픽갱신      : LO__LondonOlympic2012*
        8: 누적 출석 정보 갱신 : FSGAFlowerSpiritEventInfo
        9: 보부상 이벤트 갱신 여부   : SCGASocialCommerceEventRewardItemList_select , SCGASocialCommerceEventInfo_select(개발취소됨)
        10: 캐쉬백 비율 정보 갱신 여부: CBGACashBackRate
        11: 퀴즈빙고 정보 갱신 여부 : QBGAQuizBingo
        12: VIP 이벤트 기간 갱신 여부 : VCDBVIPCustomerInfo
    1 return ( @1 )
    @1: 값bit
        0: 지금서버가가지고있는값이최신값임
        1: 업데이트하세요
    */

    int nValue = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.GVGA_global_value_select_bit ( %d ) }", kind_ ) );
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nValue );
    }
    m_kODBC.EndFetch();

    if ( nValue != 1 ) {
        return false; // 변화가 없음.
    }

    /*
    L"{ call dbo.GVGA_global_value_merge_bit ( %d, %d ) }"

    { call dbo.GVGA_global_value_merge_bit ( @1, @2 ) }
    @1: 번호int
        1: 아이템정보갱신여부    : GoodsInfoList
        2: 원어데이정보갱신어부  : ODDBOneaDay
        3: 아이템구매제한갱신여부 : ILGAItemLimit
        4: 스핑크스퀴즈갱신여부  : SQCDSphinxQuiz
        5: 설문문제정보갱신여부 : SEGASurveyEventQuestion, SEGASurveyEventExample
        6: 모험이벤트정보갱신여부: AEGAAdventureEventInfo
        7: 런던올림픽갱신      : LO__LondonOlympic2012*
        8: 누적 출석 정보 갱신 : FSGAFlowerSpiritEventInfo
        9: 보부상 이벤트 갱신 여부   : SCGASocialCommerceEventRewardItemList_select , SCGASocialCommerceEventInfo_select(개발취소됨)
        10: 캐쉬백 비율 정보 갱신 여부: CBGACashBackRate
        11: 퀴즈빙고 정보 갱신 여부 : QBGAQuizBingo
        12: VIP 이벤트 기간 갱신 여부 : VCDBVIPCustomerInfo
    @2: 값  bit
        0: 지금서버가가지고있는값이최신값임
        1: 업데이트하세요

    1 return ( @1 )
    @1: OK int
    */

    nValue = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.GVGA_global_value_merge_bit ( %d, 0 ) }", kind_ ) );
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nValue );
    }
    m_kODBC.EndFetch();
    LIF( nValue == 0 );

    return true;
}

bool KCenterDBThread::GetItemBuyInfo( OUT std::vector<std::pair<DWORD,bool> >& vecItemBuyInfo_, OUT std::map<DWORD,unsigned char>& mapRecommInfo_ )
{
    //L"{ call dbo.IIGA_item_info_select }"
    //
    //{ call dbo.IIGA_item_info_select  }
    //
    //n return ( @1, @2 )
    //@1 ; ItemID  int
    //@2 ; ShowWeb bit

    vecItemBuyInfo_.clear();
    mapRecommInfo_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.IIGA_item_info_select }") );
    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        std::pair<DWORD,bool> prItem(0,false);
        DWORD dwRecommInfo;
        VERIFY_QUERY( m_kODBC >> prItem.first >> prItem.second >> dwRecommInfo );

        if( prItem.first == 0 ) continue;

        vecItemBuyInfo_.push_back( prItem );

        unsigned char ucRecomm = (unsigned char)(dwRecommInfo >> 24);
        if( ucRecomm > 0 )
            mapRecommInfo_.insert( std::make_pair( prItem.first, ucRecomm ) );
    }

    return (!vecItemBuyInfo_.empty());
}

void KCenterDBThread::CheckMissionEvent( OUT int& nEventID_, 
                                            OUT time_t& tmBegin_, 
                                                OUT time_t& tmEnd_, 
                                                        OUT int& nMissionCheckHour_,
                                                            OUT int& nUpdated_)
{
    /*
        L"{ call dbo.DQGADailyQuest_select }"

        { call dbo.DQGADailyQuest_select }

        n return ( @1, @2, @3, @4 )
        @1 ; EventNID   int
        @2 ; StartDateD smalldatetime
        @3 ; EndDateD   smalldatetime
        @4 ; CheckHour  tinyint
        @5 ; Updated    bit
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.DQGADailyQuest_select }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        VERIFY_QUERY( m_kODBC >> nEventID_
            >> tmBegin_
            >> tmEnd_ 
            >> nMissionCheckHour_
            >> nUpdated_ );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"CheckMissionEvent" )
        << BUILD_LOG( nEventID_ )
        << BUILD_LOGtm( CTime(tmBegin_) )
        << BUILD_LOGtm( CTime(tmEnd_) )
        << BUILD_LOG( nMissionCheckHour_ )
        << BUILD_LOG( nUpdated_ ) << END_LOG;
}

bool KCenterDBThread::GetMissionDailyEvent( IN const int nMissionEventID_, 
                                OUT std::vector<KMissionDailyData>& vecDailyEvent_ )
{
    vecDailyEvent_.clear();
    if( nMissionEventID_ <0 ) return false;

    /*
    L"{ call dbo.DQGADailyQuestDetail_select ( %d ) }"

    { call dbo.DQGADailyQuestDetail_select ( @1 ) }
    @1 ; EventNID int

    n return ( @1, @2, @3, @4 )
    @1 ; RegDateD smalldatetime
    @2 ; EventNID int
    @3 ; OrderID  tinyint
    @4 ; ItemID   int
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.DQGADailyQuestDetail_select ( %d ) }",
        nMissionEventID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KMissionDailyData kData;
        VERIFY_QUERY( m_kODBC >> kData.m_tmEventDate
            >> kData.m_nEventID
            >> kData.m_nOrderID 
            >> kData.m_nItemID );

        vecDailyEvent_.push_back( kData );

        START_LOG( clog, L"미션 이벤트 금일 미션 정보." )
            << BUILD_LOG( kData.m_nEventID )
            << BUILD_LOG( kData.m_nOrderID )
            << BUILD_LOG( kData.m_nItemID )
            << BUILD_LOG( (LPCTSTR)CTime(kData.m_tmEventDate).Format( KNC_TIME_FORMAT ) ) << END_LOG;
    }
    m_kODBC.EndFetch();

    if ( vecDailyEvent_.empty() ) {
        START_LOG( cwarn, L"미션 이벤트 금일미션 없음." ) << END_LOG;
    }
    return true;
}

void KCenterDBThread::UpdateMissionEvent()
{
    /*
    L"{ call dbo.DQGADailyQuest_update }"

    { call dbo.DQGADailyQuest_update }

    1 return ( @1 )
    @1 ; OK int
    0       ; 성공
    -101이하 ; 트랜잭션 에러
    */

    int nOK = 0;
    DBJIF( m_kODBC.Query( L"{ call dbo.DQGADailyQuest_update }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"미션 이벤트 DB정보 Update" ) << END_LOG;
}

// Center -> DB
// 기부 Grade List 얻어오기.
void KCenterDBThread::GetItemDonation( IN const bool bFirst_, OUT std::vector<KItemDonationInfo>& vecDoInfo_ )
{
    vecDoInfo_.clear();
    /*
    L"{ call dbo.CSGAContributionSystem_select ( %d ) }"

    { call dbo.CSGAContributionSystem_select ( @1 ) }
    @1 ; @bAllDisplay_ bit
    0 ; 갱신된 것만 리턴
    1 ; 전체 리스트 리턴

    n return ( @1, @2 )
    @1 ; Type      tinyint
    @2 ; GoalCount bigint
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.CSGAContributionSystem_select ( %d ) }",
        (bFirst_?1:0) ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KItemDonationInfo ItemDonation;
        VERIFY_QUERY( m_kODBC >> ItemDonation.m_dwGrade
                              >> ItemDonation.m_biGradeNum );

        vecDoInfo_.push_back( ItemDonation );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"송편 기부 정보 얻어오기. Size : " << vecDoInfo_.size() ) << END_LOG;

    if( bFirst_ )
    {
        std::vector<KItemDonationInfo>::iterator vit;
        for( vit = vecDoInfo_.begin(); vit != vecDoInfo_.end(); ++vit )
        {
            /*
            L"{ call dbo.CSGAContributionSystemItem_select ( %d ) }"

            { call dbo.CSGAContributionSystemItem_select ( @1 ) }
            @1 ; @iType_ tinyint

            n return ( @1, @2 )
            @1 ; ItemID int
            @2 ; Factor int
            @3 ; Period int
            */
            DBJIF( m_kODBC.Query( L"{ call dbo.CSGAContributionSystemItem_select ( %d ) }",
                vit->m_dwGrade) );

            while( m_kODBC.Fetch() != SQL_NO_DATA )
            {
                KDropItemInfo kData;
                VERIFY_QUERY( m_kODBC >> kData.m_ItemID
                    >> kData.m_nDuration
                    >> kData.m_nPeriod );

                vit->m_vecRewardList.push_back( kData );
            }
            m_kODBC.EndFetch();
            START_LOG( clog, L"송편 기부 등급별 아이템 리스트 얻어오기." )
                << BUILD_LOG( vit->m_dwGrade )
                << BUILD_LOG( vit->m_biGradeNum )
                << BUILD_LOG( vit->m_vecRewardList.size() ) << END_LOG;
        }
    }
}

bool KCenterDBThread::UpdateCouponInfo( IN const bool bAll_, OUT std::vector<KGameCouponInfo>& vecCouponInfo_ )
{

    //{ call dbo.GCGA_game_coupon_select_event ( @1 ) }
    //@1 ; 전체출력여부 bit
    //0 ; Updated 필드값이 1인 쿠폰정보
    //1 ; 이벤트 날짜 범위내에 있는 모든 쿠폰정보

    //n return ( @1, @2, @3, @4, @5, @6 )
    //@1 ; EventNID   int
    //@2 ; New        bit
    //@3 ; Name       nvarchar( 20 )
    //@4 ; StartDateB smalldatetime
    //@5 ; EndDateB   smalldatetime
    //@6 ; Contents   nvarchar( 25 )
    vecCouponInfo_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.GCGA_game_coupon_select_event ( %d ) }", (bAll_?1:0) ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KGameCouponInfo kInfo;
        VERIFY_QUERY( m_kODBC >> kInfo.m_nEventUID
            >> kInfo.m_bNew
            >> kInfo.m_strEventName
            >> kInfo.m_tStartDate
            >> kInfo.m_tEndDate
            >> kInfo.m_strContents );
        kInfo.m_bActive = false;
        vecCouponInfo_.push_back( kInfo );
    }

    if( vecCouponInfo_.empty() ) return false;

    if( vecCouponInfo_.empty() || bAll_ == false )
    {
        //{ call dbo.GCGA_game_coupon_update_event }
        //1 return ( @1 )
        //@1 ; @iOK int
        //0        ; 성공
        //-101이하 ; 트랜잭션 에러
        DBJIF2( m_kODBC.Query( L"{ call dbo.GCGA_game_coupon_update_event }" ) );
        if( m_kODBC.BeginFetch() ) {
            int nOK = -99;
            VERIFY_QUERY( m_kODBC >> nOK );
        }
        m_kODBC.EndFetch();
    }

    return true;
}

bool KCenterDBThread::UpdateQuizBingoData( IN const bool bForce_ )
{
    KCnQuizBingo::SBingoConfigData bingoConfig;
    bool bChanged = BingoUpdateCheck( bingoConfig.m_nEventID, bingoConfig.m_tmBegin, bingoConfig.m_tmEnd );
    if( !bForce_ )
        return false;

    _JIF( GetBingoQuestions( bingoConfig.m_nEventID, bingoConfig.m_mapQuestions ), return false );
    _JIF( SiKCnQuizBingo()->VerifyQuestions( bingoConfig.m_mapQuestions ), return false );
    SiKCnQuizBingo()->UpdateBingoConfig( bingoConfig );

    return true;
}

bool KCenterDBThread::BingoUpdateCheck( OUT int& nEventID_, OUT time_t& tmBegin_, OUT time_t& tmEnd_ )
{
    //{ call dbo.QBGAQuizBingo_select }
    //
    //n select ( @1, @2, @3, @4 )
    //@1  ; NID        int
    //@2  ; StartDateB smalldatetime
    //@3  ; EndDateB   smalldatetime
    //@4  ; Updated    bit

    DBJIF2( m_kODBC.Query( L"{ call dbo.QBGAQuizBingo_select }" ) );

    bool bChanged = false;

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {

        KCnQuizBingo::SBingoConfigData bingoConfig;

        VERIFY_QUERY( m_kODBC >> bingoConfig.m_nEventID
            >> bingoConfig.m_tmBegin
            >> bingoConfig.m_tmEnd
            >> bingoConfig.m_bChanged );

        // 현재 진행중인 이벤트, 1개의 이벤트만 유효함        
        if ( IsCurrentBingo( bingoConfig.m_tmBegin, bingoConfig.m_tmEnd ) == true ) {
            nEventID_ = bingoConfig.m_nEventID;
            tmBegin_ = bingoConfig.m_tmBegin;
            tmEnd_ = bingoConfig.m_tmEnd;
            bChanged = bingoConfig.m_bChanged;
        }
    }

    if ( bChanged ) {
        //{ call dbo.QBGAQuizBingo_update ( @1 ) }
        //@1 ; @iNID_ int
        //
        //1 select ( @1 )
        //@1  ; OK int
        //0        ; 성공
        //-1       ; 등록된 퀴즈빙고가 아님
        //-101이하 ; 트랜잭션 에러
        DBJIF2( m_kODBC.Query( L"{ call dbo.QBGAQuizBingo_update ( %d ) }", nEventID_ ) );
        if ( m_kODBC.BeginFetch() ) {
            int nOK = -1;
            VERIFY_QUERY( m_kODBC >> nOK );
            m_kODBC.EndFetch();
            _LOG_SUCCESS( nOK == 0, L"OK : " << nOK )
                << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
        }
    }

    return bChanged;
}

bool KCenterDBThread::IsCurrentBingo( IN const time_t tmBegin_, IN const time_t tmEnd_ )
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    if ( tmBegin_ <= tmCurrent && tmCurrent <= tmEnd_ ) {
        return true;
    }
    return false;
}

bool KCenterDBThread::GetBingoQuestions( IN const int nEventID_, OUT std::map<int,KBingoQuestionInfo>& mapQuestions_ )
{
    mapQuestions_.clear();

    // 질문 얻어 오기.
    //{ call dbo.QBGAQuizBingoContents1_select ( @1 ) }
    //@1 ; @iNID_ int
    //
    //n select ( @1, @2, @3, @4 )
    //@1  ; NID      int // 이벤트 번호
    //@2  ; NID1     tinyint // 질문 번호
    //@3  ; Contents nvarchar( 200 ) // 질문 
    //@4  ; Answer   tinyint // 답

    DBJIF2( m_kODBC.Query( L"{ call dbo.QBGAQuizBingoContents1_select ( %d ) }", nEventID_ ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KBingoQuestionInfo questionInfo;
        int nEventID = -1;
        VERIFY_QUERY( m_kODBC >> nEventID
            >> questionInfo.m_nID
            >> questionInfo.m_strQuestion
            >> questionInfo.m_nAnswer );
        if ( nEventID != nEventID_ ) continue;
        if ( !mapQuestions_.insert( std::make_pair( questionInfo.m_nID, questionInfo ) ).second ) {
            START_LOG( cerr, L"중복된 질문이 있음. ID : " << questionInfo.m_nID ) << END_LOG;
        }
    }

    // 선택지 얻어 오기.
    //{ call dbo.QBGAQuizBingoContents2_select ( @1 ) }
    //@1 ; @iNID_ int
    //
    //n select ( @1, @2, @3, @4 )
    //@1  ; NID      int // 이벤트 번호
    //@2  ; NID1     tinyint // 질문 번호
    //@3  ; NID2     tinyint // 선택지 번호
    //@4  ; Contents nvarchar( 50 ) // 문장

    DBJIF2( m_kODBC.Query( L"{ call dbo.QBGAQuizBingoContents2_select ( %d ) }", nEventID_ ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        int nEventID = -1;
        int nQuestionID = -1;
        int nChoiceNum = -1;
        std::wstring strChoice;
        VERIFY_QUERY( m_kODBC >> nEventID
            >> nQuestionID
            >> nChoiceNum
            >> strChoice );
        if( nEventID != nEventID_ ) continue;

        std::map<int,KBingoQuestionInfo>::iterator mit;
        mit = mapQuestions_.find( nQuestionID );
        if ( mit == mapQuestions_.end() ) {
            START_LOG( cerr, L"없는 질문의 선택지가 있음. ID : " << nQuestionID ) << END_LOG;
            continue;
        }

        if ( !mit->second.m_mapChoices.insert( std::make_pair( nChoiceNum, strChoice) ).second ) {
            START_LOG( cerr, L"중복된 선택지가 있음. QuestionID : " << nQuestionID << L", ChoiceNum : " << nChoiceNum ) << END_LOG;
        }
    }
    return true;
}

bool KCenterDBThread::GetGWCData( OUT std::vector<KGWCSupportData>& vecGWCData_ )
{
    vecGWCData_.clear();

    //{ call dbo.WCGAWorldChampionship2009_select_total }

    //n select ( @1, @2 )
    //@1 ; Type tinyint
    //    1 ; 01kr한국
    //    2 ; 03tw대만
    //    3 ; 04br브라질
    //    4 ; 05hk홍콩
    //    5 ; 08us미국
    //    6 ; 09ph필리핀
    //    7 ; 11th태국
    //@2 ; Data int

    DBJIF2( m_kODBC.Query( L"{ call dbo.WCGAWorldChampionship2009_select_total }" ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KGWCSupportData kData;
        VERIFY_QUERY( m_kODBC >> kData.m_nNationCode 
            >> kData.m_nAccCount );
        vecGWCData_.push_back( kData );
    }
    return ( !vecGWCData_.empty() );
}

bool KCenterDBThread::GetTotalDonationInfo( IN time_t tmDate_, IN const DWORD dwMinType_, IN const DWORD dwMaxType_, OUT std::map<DWORD, int>& mapTotalDonation_ )
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
    for( DWORD i = dwMinType_; i <= dwMaxType_; i++ ) {

        int nRes = 0;
        DBJIF2( m_kODBC.Query( L"{ call dbo.SDGA_server_data_select_integer ( N'%s', %d ) }",
            (LPCTSTR)tmTime.Format( L"%Y%m%d" ), i ) );

        if( m_kODBC.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBC >> nRes );
        }
        m_kODBC.EndFetch();

        mapTotalDonation_[i] = nRes;
    }

    return true;
}

bool KCenterDBThread::GetCharExpInfo( IN const DWORD dwUID_, OUT std::map<char,__int64>& mapCharExp_ )
{
    /*
    L"{ call dbo.CIGACharacterInfo_select_20130326 ( %d ) }"

    { call dbo.CIGACharacterInfo_select_20130326 ( @01 ) }
    @01: @01iLoginUID_input int

    n return ( @01, @02, @03, @04, @05, @06, @07, @08, @09, @10, @11, @12 )
    @01: SlotNo       int
    @02: CharType     tinyint
    @03: Promotion    tinyint
    @04: ExpS4        bigint
    @05: Win          int
    @06: Lose         int
    @07: Level        int
    @08: Exp          bigint
    @09: GamePoint    bigint
    @10: DefaultBonus int
    @11: SpecialBonus int
    @12: InvenSize    int
    */

    mapCharExp_.clear();
    DBJIF2( m_kODBC.Query( L"{ call dbo.CIGACharacterInfo_select_20130326 ( %d ) }", dwUID_ ) );

    while( m_kODBC.Fetch() !=  SQL_NO_DATA ) {
        std::pair<char, __int64> prCharExp;
        int nSlotNum;
        char cPromotion;
        int nWin;
        int nLose;
        int nLevel;
        __int64 biExp;
        __int64 biGP;
        int nDefaultBouns;
        int nSpecialBouns;
        int nInvenCapacity;
        VERIFY_QUERY( m_kODBC
            >> nSlotNum
            >> prCharExp.first
            >> cPromotion
            >> prCharExp.second
            >> nWin
            >> nLose
            >> nLevel
            >> biExp
            >> biGP
            >> nDefaultBouns
            >> nSpecialBouns
            >> nInvenCapacity );
        mapCharExp_.insert( prCharExp );
    }
    return true;
}

bool KCenterDBThread::Get_DungeonRank_SeasonDelimitTime_FromDB( OUT time_t& tmSeasonDelimit_ )
{
    /*
    L"{ call dbo.ETGAExtinctionTowerSession_select }"

    { call dbo.ETGAExtinctionTowerSession_select }

    1 return ( @1 )
    @1 ; SeasonDelimitTime  datetime
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.ETGAExtinctionTowerSession_select }", tmSeasonDelimit_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> tmSeasonDelimit_ );
        m_kODBC.EndFetch();
    }

    START_LOG( clog, L"시즌 기준시간 조회 성공: " << tmSeasonDelimit_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return true;
}

bool KCenterDBThread::Get_DungeonRank_CurrentSeasonTopRank_FromDB( IN const std::set< int >& setRankingModes_, OUT std::map< int, std::list< KDungeonRankInfo > >& mapCurrentDungeonRankList_ )
{
    /*
    L"{ call dbo.ETGAExtinctionTowerRankCurrent_select ( %d ) }"

    { call dbo.ETGAExtinctionTowerRankCurrent_select ( @1 ) }
    @1 ; @iModeID_    int

    n return ( @1, @2, @3, @4, @5 )
    @1 ; StageNum        int            //스테이지 번호
    @2 ; StageClearTime  int            //베스트 클리어 시간 //ms단위 숫자 (1초 = 1000, 1분 = 60초 = 60000 )
    @3 ; LoginUID        int
    @4 ; NickName        nvarchar(24)
    @5 ; CharType        int
    */

    /*
    현재 진행중인 시즌 기록들을 아래 조건 순서대로 정렬하여 Top 100을 리턴.
    조건1: StageNum이 높으면 상위
    조건2: StageNum이 같고 StageClearTime이 낮으면 상위
    조건3: StageNum과 StageClearTime이 같고 LoginUID가 낮으면 상위
    */
    mapCurrentDungeonRankList_.clear();

    BOOST_FOREACH( const int& rnModeID, setRankingModes_ ) {
        DBJIF2( m_kODBC.Query( L"{ call dbo.ETGAExtinctionTowerRankCurrent_select ( %d ) }", rnModeID ) );

        while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
            KDungeonRankInfo kDungeonRankInfo;

            VERIFY_QUERY( m_kODBC >> kDungeonRankInfo.m_nStageNum
                >> kDungeonRankInfo.m_dwStageClearTime
                >> kDungeonRankInfo.m_dwLoginUID
                >> kDungeonRankInfo.m_wstrNickName
                >> kDungeonRankInfo.m_cCharType );

            mapCurrentDungeonRankList_[ rnModeID ].push_back( kDungeonRankInfo );
        }

        START_LOG( clog, L"현재 시즌 랭킹 리스트 조회" )
            << BUILD_LOG( rnModeID )
            << BUILD_LOG( mapCurrentDungeonRankList_.size() )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
    }

    return true;
}

bool KCenterDBThread::Get_DungeonRank_PreviousSeasonTopRank_FromDB( IN const std::set< int >& setRankingModes_, OUT std::map< int, std::list< KDungeonRankInfo > >& mapPreviousDungeonRankList_ )
{
    /*
    L"{ call dbo.ETGAExtinctionTowerRankPrevious_select ( %d ) }"

    { call dbo.ETGAExtinctionTowerRankPrevious_select ( @1 ) }
    @1 ; @iModeID_    int

    n return ( @1, @2, @3, @4, @5, @6 )
    @1 ; Rank             int            
    @2 ; LoginUID         int            
    @3 ; NickName         int
    @4 ; CharType         nvarchar(24)
    @5 ; StageNum         int           //스테이지 번호
    @6 ; StageClearTime   int           //베스트 클리어 시간 //ms단위 숫자 (1초 = 1000, 1분 = 60초 = 60000 )
    */
    mapPreviousDungeonRankList_.clear();

    BOOST_FOREACH( const int& rnModeID, setRankingModes_ ) {
        DBJIF2( m_kODBC.Query( L"{ call dbo.ETGAExtinctionTowerRankPrevious_select ( %d ) }", rnModeID ) );

        while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
            KDungeonRankInfo kDungeonRankInfo;

            VERIFY_QUERY( m_kODBC >> kDungeonRankInfo.m_nRank
                >> kDungeonRankInfo.m_dwLoginUID
                >> kDungeonRankInfo.m_wstrNickName
                >> kDungeonRankInfo.m_cCharType
                >> kDungeonRankInfo.m_nStageNum
                >> kDungeonRankInfo.m_dwStageClearTime );

            mapPreviousDungeonRankList_[ rnModeID ].push_back( kDungeonRankInfo );
        }

        START_LOG( clog, L"이전 시즌 랭킹 리스트 조회" )
            << BUILD_LOG( rnModeID )
            << BUILD_LOG( mapPreviousDungeonRankList_.size() )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
    }

    return true;
}

bool KCenterDBThread::Update_DungeonRank_CurrentSeasonNewEntry_ToDB( IN const std::vector< KDungeonRankInfo >& vecNewEntry_, OUT std::vector< KDungeonRankInfo >& vecNewEntryAfterDBWrite_ )
{
    /*
    L"{ call dbo.ETGAExtinctionTowerRankCurrent_merge ( %d, %d, N'%s', %d, %d, %d ) }"

    { call dbo.ETGAExtinctionTowerRankCurrent_merge ( @1, @2, @3, @4, @5, @6 ) }
    @1 ; @iModeID_          int
    @2 ; @iLoginUID_        int
    @3 ; @strNickName_      nvarchar(24)
    @4 ; @iCharType_        int   
    @5 ; @iStageNum_        int   //스테이지 번호
    @6 ; @iStageClearTime_  int   //베스트 클리어 시간 //ms단위 숫자 (1초 = 1000, 1분 = 60초 = 60000 )

    1 return ( @1 )
    @1: OK       int
        0       : 성공
        -1      : 유저/케릭터 없음
        -901    ; 데이터 등록 실패
        -902    ; 데이터 갱신 실패
    */
    vecNewEntryAfterDBWrite_.clear();

    bool bDBWriteSuccess = true;
    BOOST_FOREACH( const KDungeonRankInfo& rDungeonRankInfo, vecNewEntry_ ) {
        DBJIF2( m_kODBC.Query( L"{ call dbo.ETGAExtinctionTowerRankCurrent_merge ( %d, %d, N'%s', %d, %d, %d ) }",
            rDungeonRankInfo.m_nModeID,
            rDungeonRankInfo.m_dwLoginUID,
            rDungeonRankInfo.m_wstrNickName.c_str(),
            rDungeonRankInfo.m_cCharType,
            rDungeonRankInfo.m_nStageNum,
            rDungeonRankInfo.m_dwStageClearTime ) );

        int nRet = -99;
        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> nRet );
            m_kODBC.EndFetch();
        }

        LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nRet ) );

        if ( 0 == nRet ) {
            vecNewEntryAfterDBWrite_.push_back( rDungeonRankInfo );
        }
        else {
            bDBWriteSuccess = false;

            START_LOG( cerr, L"현재 시즌 랭크 DB 저장 실패: LoginUID: " << rDungeonRankInfo.m_dwLoginUID << ", ModeID: " << rDungeonRankInfo.m_nModeID << ", StageNum: " << rDungeonRankInfo.m_nStageNum )
                << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
        }
    }

    _LOG_SUCCESS( true == bDBWriteSuccess, L"던전 개인 최고 기록 저장" )
        << BUILD_LOG( vecNewEntry_.size() )
        << BUILD_LOG( vecNewEntryAfterDBWrite_.size() )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;

    return true;
}

bool KCenterDBThread::Do_DungeonRank_BuildPreviousSeasonRank_ToDB( IN const CTime& ctCurrent_ )
{
    /*
    L"{ call dbo.ETGAExtinctionTowerRankBuild ( N'%s' ) }"

    { call dbo.ETGAExtinctionTowerRankBuild ( @1 ) }
    @1 ; @strSeasonDelimitTime_      nvarchar(20)

    1 return ( @1 )
    @1: OK       int
        0       : 성공
        -1      : 시즌갱신 날짜가 이전시즌 날짜와 같거나 이전임.
        -901    ; 시즌갱신 날짜 등록 실패.
        -902    ; 이전 시즌 테이블 초기화 실패.
        -903    ; 현재 시즌 -> 이전 시즌 데이터 등록 실패.
        -904    ; 현재 시즌 테이블 데이터 삭제 실패.
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.ETGAExtinctionTowerRankBuild ( N'%s' ) }",
        (LPCTSTR)ctCurrent_.Format( L"%Y%m%d %H:%M:%S" ) ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nRet ) );

    _LOG_SUCCESS( 0 == nRet, L"던전 랭크 빌드 요청: " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery )  << END_LOG;

    return ( 0 == nRet );
}

bool KCenterDBThread::Get_DungeonRank_PreviousSeason()
{
    std::set< int > setRankingModes;
    SiKCnDungeonRankManager()->GetRankingModes( setRankingModes );

    std::map< int, std::list< KDungeonRankInfo > > mapPreviousDungeonRankList;
    Get_DungeonRank_PreviousSeasonTopRank_FromDB( setRankingModes, mapPreviousDungeonRankList );

    SiKCnDungeonRankManager()->SetPreviousSeasonTopRank( mapPreviousDungeonRankList );

    START_LOG( cerr, L"지난 시즌 랭킹 가져오기 완료. ModeSize: " << setRankingModes.size() ) << END_LOG;

    return true;
}
