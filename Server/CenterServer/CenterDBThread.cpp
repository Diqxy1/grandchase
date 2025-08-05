#include <winsock2.h>
#include "CenterDBThread.h"
#include "NetError.h"
#include "CenterSimLayer.h"
#include "CenterServer.h"
#include <dbg/dbg.hpp>
#include "CnDonationManager.h"
#include "CountType.h"
#include "CnRainBow.h"
#include "CnItemManager.h"
#include "CnCouponManager.h"
#include "CnMissionEvent.h"
#include "CnSubscriptionManager.h"
#include "CnNpcGift.h"
#include "CnDicePlay.h"
#include "CnGuildManager.h"
#include <boost/bind.hpp>
#include "CnGWCSupport.h"
#include "CnNewDonationManager.h"
#include "BlockIP.h"
#include "CnSphinx.h"
#include "CnSurvey.h"
#include <boost/algorithm/string.hpp>
#include "Log4.h"
#include "CnOlympicManager.h"
#include "CnBuffDonation.h"
#include "GCEnum.h"
#include "CnUpdatePlanManager.h"
#include "CnSocialCommerce.h"
#include "CnItemProvider.h"
#include "CnDungeonRankManager.h"

ImplementDBThread( KCenterDBThread );
ImplementDBThreadName( KCenterDBThread, Main );

#define CLASS_TYPE KCenterDBThread
#define DICEPLAYTYPE 4
#define DICEPLAYTOTALCOUNT 24

KCenterDBThread::KCenterDBThread()
: m_kHelper( m_kODBC )
{
    m_kODBC.DriverConnect( ms_wstrFileDSN.c_str() ); // 유저용
    m_kODBCMain.DriverConnect( ms_wstrFileDSNMain.c_str() ); // 로긴서버 데이터 처리용
}

KCenterDBThread::~KCenterDBThread(void)
{
}

void KCenterDBThread::ProcessIntEvent( const KIntEvent& kIntEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &kIntEvent_.m_kbuff ) );

    switch( kIntEvent_.m_usEventID )
    {
       _INT_CASE( DB_EVENT_SERVER_LIST_REQ, int );
        INT_CASE( ECN_VERIFY_ACCOUNT_REQ );
       _INT_CASE( DB_ENU_VERIFY_ACCOUNT_REQ, KENU_VERIFY_ACCOUNT_REQ );
       _INT_CASE( DB_EVENT_FIRST_INIT_REQ, int );
       _INT_CASE( DB_CONNECTION_COUNT_RECORD_REQ, DWORD );
        INT_CASE_NOPARAM( DB_EVENT_MINIGAME_RANK_REQ );
        INT_CASE_NOPARAM( ECN_GET_TOTAL_DONATION_POINT_REQ );
       _INT_CASE( DB_UPDATE_SCRIPT_CHECKSUM_REQ, int );
       _INT_CASE( ECN_UPDATE_CONNECTION_COUNT_REQ, DWORD );
        INT_CASE_NOPARAM( DB_UPDATE_CHANNEL_NEWS_REQ );
       _INT_CASE( ECN_DB_UPDATE_DISCONNECT_TYPE, MAP_INT_INT );
        INT_CASE_NOPARAM( ECN_DB_RAINBOW_UPDATE_REQ );
        INT_CASE_NOPARAM( ECN_DB_UPDATE_CHECK_REQ );
        INT_CASE_NOPARAM( ECN_DB_MISSION_EVENT_REQ );
        INT_CASE_NOPARAM( ECN_DB_SUBSCRIPTION_INFO_UPDATE_REQ );
        INT_CASE_NOPARAM( ECN_DB_NPC_GIFTS_UPDATE_REQ );
        INT_CASE_NOPARAM( ECN_DICE_PLAY_TOTAL_RANK_REQ );
        INT_CASE_NOPARAM( ECN_DB_ITEM_DONATION_UPDATE );
        // Guild
        INT_CASE_NOPARAM( DB_ECN_UPDATE_GUILD_LIST_REQ );
        INT_CASE_NOPARAM( DB_ECN_UPDATE_GUILD_INFO_CHECK_REQ );
        INT_CASE_NOPARAM( ECN_GUILD_MARK_INFO_UPDATE_REQ );
        INT_CASE( ECN_JOIN_GUILD_REQ );
       _INT_CASE( ECN_CANCEL_JOIN_GUILD_REQ, PAIR_DWORD );
       _INT_CASE( ECN_SELF_DRUMOUT_GUILD_REQ, PAIR_DWORD );
        INT_CASE( ECN_CHANGE_GUILD_MEMBER_LEVEL_REQ );
        INT_CASE( ECN_ACCEPT_GUILD_JOINER_REQ );
        INT_CASE( ECN_REJECT_GUILD_JOINER_REQ );
        INT_CASE( ECN_BREAKUP_GUILD_REQ );
       _INT_CASE( ECN_CHANGE_GUILD_JOIN_SETTING_REQ, KCnNGuildJoinPolicy );
       _INT_CASE( ECN_AUTO_CHANGE_GUILD_JOIN_SETTING_NOT, KCnNGuildJoinPolicy );
       _INT_CASE( ECN_UPGRADE_GUILD_GRADE_REQ, DWORD );
       _INT_CASE( ECN_REMOVE_GUILD_NOT, DWORD );
        INT_CASE( ECN_UPDATE_GUILD_MEMBER_LIST_REQ );
        INT_CASE( ECN_AUTO_REJECT_GUILD_JOINER_REQ );
       _INT_CASE( ECN_NEWBIE_GUILD_DESTROY_NOT, DWORD );

        INT_CASE_NOPARAM( ECN_COUPON_INFO_UPDATE_REQ );
        // 091210. kkurrung. GWC Event
        INT_CASE_NOPARAM( ECN_DB_UPDATE_GWC_DATA );
        INT_CASE( ECN_UPGRADE_GUILD_GRADE_GIFT_REQ );
        INT_CASE_NOPARAM( ECN_GET_TOTAL_NEW_DONATION_REQ );

        INT_CASE_NOPARAM( ECN_GUIDE_BOOK_LIST_REQ );
       _INT_CASE( ECN_UPDATE_GUILD_MEMBER_NICK_NOT, KGuildUserName );
       _INT_CASE( ECN_GUILD_MEMBER_LEVELUP_NOT, KGuildUserCharacter );

        INT_CASE_NOPARAM( ECN_DB_QUIZ_BINGO_DATA_NOT );
        INT_CASE( ECN_CHAT_REQ );
        INT_CASE( ECN_ADD_GUILD_USER_REQ );
       _INT_CASE( ECN_UPDATE_UNIQUE_USER_STAT_REQ, MAP_DWORD_SET );

       _INT_CASE( ECN_UPDATE_GUILD_POINT_REQ, KGuildPointBroad );
        INT_CASE_NOPARAM( ECN_DB_GUILD_POINT_UPDATE_REQ );

       _INT_CASE( ECN_SQUARE_LIST_REQ, PAIR_DWORD );
        INT_CASE_NOPARAM( ECN_DB_SPHINX_DATA_NOT );
       _INT_CASE( ECN_UPDATE_LOGIN_CCU, DWORD );
        INT_CASE_NOPARAM( ECN_DB_SURVEY_DATA_NOT );
        INT_CASE_NOPARAM( ECN_DB_GWC_EVENT_DATA_REQ );

        INT_CASE_NOPARAM( ECN_DB_EVENT_TIME_SYNCRONIZE_NOT );

        INT_CASE_NOPARAM( ECN_DB_ADVENTURE_DATA_NOT );

        INT_CASE_NOPARAM( ECN_OLYMPIC_RANK_REQ );
        INT_CASE_NOPARAM( ECN_DB_OLYMPIC_DATA_NOT );
        INT_CASE_NOPARAM( ECN_DB_OLYMPIC_USER_DATA_UPDATE_NOT );
        INT_CASE_NOPARAM( ECN_OLYMPIC_DB_USER_DATA_REQ );

       _INT_CASE( ECN_BUFF_DONATION_TOTAL_DATA_REQ, KECN_BUFF_DONATION_TOTAL_DATA_NOT );
       _INT_CASE( ECN_BUFF_DONATION_DATA_UPDATE_REQ, int );
        INT_CASE_NOPARAM( ECN_ACCUMULATE_ATTENDANCE_DATA_UPDATE_NOT );

       _INT_CASE( ECN_SOCIAL_COMMERCE_DATA_UPDATE_NOT, bool );
        INT_CASE( DB_ECN_SOCIAL_COMMERCE_GROUPPURCHASE_USER_REQ );

        INT_CASE( DB_ECN_ITEM_PROVIDE_REQ );

        INT_CASE_NOPARAM( ECN_DB_DUNGEON_RANK_SEASON_DELIMIT_TIME_REQ );
        INT_CASE_NOPARAM( ECN_DB_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_REQ );
        INT_CASE_NOPARAM( ECN_DB_DUNGEON_RANK_PREVIOUS_SEASON_TOP_RANK_REQ );
        INT_CASE( ECN_DUNGEON_RANK_CURRENT_SEASON_ADD_RANK_REQ );
       _INT_CASE( ECN_DB_DUNGEON_RANK_SEASON_RESET_CHECK_REQ, bool );

       _INT_CASE( DB_ENU_ACCOUNT_REGISTER_REQ, KENU_ACCOUNT_REGISTER_REQ );

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " 
            << IDVenderType::GetEventIDString( kIntEvent_.m_usEventID ) ) << END_LOG;
    }
}

// 이녀석이 1분마다 불리므로 이녀석을 이용해서 한시간 이벤트를 얻어 오겠다.
_INT_IMPL_ON_FUNC( DB_EVENT_SERVER_LIST_REQ, int )
{
    std::vector<KServerInfo> vecServerInfo;
    GetServerList( kPacket_, vecServerInfo );
    SiKCenterServer()->SetServerList( vecServerInfo );

    // TCP 서버 정보 가져오기.
    std::vector<KSimpleServerInfo> vecUseTCPServerInfo;
    std::vector<KSimpleServerInfo> vecNoUseTCPServerInfo;
    GetTCPServerList( vecUseTCPServerInfo, vecNoUseTCPServerInfo );
    SiKCenterServer()->SetTCPServerList( vecUseTCPServerInfo, vecNoUseTCPServerInfo );

    //call dbo.TEG_timer_event_date_select }
    //
    //n return
    //( IssueUID int )   이벤트의 고유 ID
    //( MessageID int )  글로벌 메세지 시스템에서 쓰는 ID


    if( vecServerInfo.empty() )
    {
        START_LOG( cwarn,L" 서버가 떠 있지 않음. " )
            << BUILD_LOG( vecServerInfo.size() ) << END_LOG;
        return;
    }

    DBJIF( m_kODBC.Query( L"{ call dbo.TEG_timer_event_date_select }" ) );
    if( m_kODBC.BeginFetch() )
    {
        DWORD dwEventUID = 0;
        int nMessageID = 0;
        VERIFY_QUERY( m_kODBC >> dwEventUID >> nMessageID );
        m_kODBC.EndFetch();

        if( dwEventUID > 0 && nMessageID > 0 )
        {
            SiKCenterServer()->UpdateRepetitionData( dwEventUID, nMessageID );
            SiKCenterServer()->ProcessRepetitionEvent();
        }
        START_LOG( clog, L"한시간 이벤트 얻어옴.." )
            << BUILD_LOG( dwEventUID )
            << BUILD_LOG( nMessageID ) << END_LOG;
    }
    m_kODBC.EndFetch();
    // 서버 동접 이상 체크.
    // 해당 함수에서 동접 체크를 진행하고 문제가 발생하였을 경우 문자를 보낸다.
    // CheckTotalServerUser( SiKCenterServer()->GetTotalServerUser() );
}

/*
20081106. microcat.

{ call dbo.up_verify_game_server ( N'%s', N'%s', %d, %d ) }

{ call dbo.up_verify_game_server ( @1, @2, @3, @4 ) }
@1 ; ServerName nvarchar(20)
@2 ; ServerIP nvarchar(15)
@3 ; ServerPort int
@4 ; ExtraFlag int

1 return ( @1 )
@1 ; OK int

OK
0 ; 성공
-1 ; 서버 리스트에 서버가 등록되어 있지 않음
-2 ; 서버 ip가 다름
-3 ; 서버 port가 다름
-4 ; 서버 프로토콜이 다름
*/

INT_IMPL_ON_FUNC( ECN_VERIFY_ACCOUNT_REQ )
{
    START_LOG( clog, L" Recv Data Packet .." )
        << BUILD_LOG(kPacket_.m_nProtocolVer )
        << BUILD_LOG(kPacket_.m_nUserProtocolVer )
        << BUILD_LOG(kPacket_.m_strIP )
        << BUILD_LOG(kPacket_.m_strName )
        << BUILD_LOG(kPacket_.m_usPort )
        << BUILD_LOG(kPacket_.m_dwUID ) << END_LOG;

    DBJIF( m_kODBCMain.Query( L"{ call dbo.up_verify_game_server ( N'%s', N'%s', %d, %d ) }",
        kPacket_.m_strName.c_str(),
        kPacket_.m_strIP.c_str(),
        kPacket_.m_usPort,
        kPacket_.m_nUserProtocolVer ) );

    KECN_VERIFY_ACCOUNT_ACK kPacket;
    kPacket.m_nOK = NetError::ERR_UNKNOWN;

    if( m_kODBCMain.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBCMain >> kPacket.m_nOK );
        m_kODBCMain.EndFetch();
    }

    kPacket.m_kServerInfo.m_strName             = kPacket_.m_strName;
    kPacket.m_kServerInfo.m_strIP               = KncUtil::toNarrowString( kPacket_.m_strIP );
    kPacket.m_kServerInfo.m_usPort              = kPacket_.m_usPort;
    kPacket.m_kServerInfo.m_nUserProtocolVer    = kPacket_.m_nUserProtocolVer;
    kPacket.m_kServerInfo.m_iServerPart         = 0;
    kPacket.m_kServerInfo.m_iUserNum            = 0;
    kPacket.m_kServerInfo.m_iMaxUserNum         = 0;
    kPacket.m_kServerInfo.m_dwUID               = kPacket_.m_dwUID;

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"Ret : " << kPacket.m_nOK )
        << BUILD_LOG( m_kODBCMain.m_strLastQuery ) << END_LOG;

    QUEUING_ACK_TO_USER( ECN_VERIFY_ACCOUNT_ACK );
}

_INT_IMPL_ON_FUNC( DB_ENU_VERIFY_ACCOUNT_REQ, KENU_VERIFY_ACCOUNT_REQ )
{
    KENU_VERIFY_ACCOUNT_ACK kPacket;
    kPacket.m_nOK               = -1;
    kPacket.m_strLogin          = KncUtil::toWideString( kPacket_.m_strLogin );
    kPacket.m_dwUID             = 0;
    kPacket.m_bMale             = kPacket_.m_bMale;
    kPacket.m_iAge              = kPacket_.m_iAge;
    kPacket.m_bExpAccount       = kPacket_.m_bExpAccount;
    kPacket.m_bBlockIPPass      = false;
    kPacket.m_cAuthLevel        = 0;
    kPacket.m_nFunBoxBonus      = kPacket_.m_nFunBoxBonus;
    kPacket.m_nLinBonus         = kPacket_.m_nLinBonus;
    kPacket.m_vecScriptCheckResult.clear();
    kPacket.m_dwChannellingType = kPacket_.m_dwChannellingType;

    if ( kPacket_.m_dwAuthType == KSimLayer::AT_FUNBOX_TH ) {
        kPacket_.m_dwChannellingType = kPacket_.m_nFunBoxPostfix + 1;
        kPacket.m_dwChannellingType = kPacket_.m_dwChannellingType;
    }

    kPacket.m_biUniqueKey        = kPacket_.m_biUniqueKey;
    kPacket.m_nAccOnlineTime = kPacket_.m_nAccOnlineTime;
    kPacket.m_cFatigueState = kPacket_.m_cFatigueState;
    kPacket.m_cFatigueLevel = kPacket_.m_cFatigueLevel;
    kPacket.m_cIsRealNameAuth = kPacket_.m_cIsRealNameAuth;
    kPacket.m_cIsRealName = kPacket_.m_cIsRealName;
    kPacket.m_strBillingAccount = kPacket_.m_strBillingAccount;

    std::wstring wstrPasswd;
    std::wstring wstrExpLogin;
    std::string strIP;

    if( kPacket_.m_bExpAccount ) //  080723  woosh. 체험계정 식별자 삽입
    {
        wstrExpLogin       = kPacket.m_strLogin;                 //  080723  woosh. 식별자가 들어가기 전 로긴 보관( 신규 체험 계정 등록 위해서 )
        kPacket.m_strLogin = L"kkurrung1_" + kPacket.m_strLogin;
    }

    int nOK = -99;

    DBJIF(m_kODBC.Query(L"{ call dbo.gnu_exist_user_check ( N'%s' ) }",kPacket.m_strLogin.c_str()));
    if (m_kODBC.BeginFetch())
    {
        VERIFY_QUERY(m_kODBC >> nOK);
        m_kODBC.EndFetch();
        if (nOK != 0)
        {
            SET_ERROR(ERR_NUSER_VERIFY_09);
            goto verify_end;
        }
    }

    if ( false == SiKCenterSimLayer()->CheckTypeFlag( KCenterSimLayer::ST_KOG_AUTH_TEST ) ) { // KOG 사내에서는 인증 서버의 결과를 무시하도록 임시 설정한다
        if( SiKCenterSimLayer()->CheckAuthTypeFlag( KSimLayer::AT_CYOU ) && ( kPacket_.m_nOK != NetError::NET_OK && kPacket_.m_nOK != NetError::ERR_NUSER_VERIFY_37 ) ) {
            START_LOG( clog, L"CenterDBThread goto" )
                << END_LOG;
            NetError::SetLastNetError( kPacket_.m_nOK );
            goto verify_end;
        }
    }

    SET_ERROR( ERR_UNKNOWN );
    
    DBJIF( m_kODBC.Query( L"{ call dbo.gnu_new_user_check ( N'%s' ) }",kPacket.m_strLogin.c_str() ) );
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
        kPacket.m_bNewUser = true;
        if( nOK == 0 )
        {
            kPacket.m_bNewUser = false;
        }
    }

    // DB에 길이없는 Login이 등록된 경우 그냥 인증을 성공할 수 있다. 이런 문제를 해결한다.
    if( kPacket.m_strLogin.empty() )
    {
        SET_ERROR( ERR_NUSER_VERIFY_09 );
        goto verify_end;
    }

    // wstring -> string으로 변환해서 CheckBlockIP 인자에 맞는 형태로 변환.
    strIP = KncUtil::toNarrowString( kPacket_.m_strIP );
    DWORD dwIP = ::ntohl( ::inet_addr( strIP.c_str() ) );

    //Block List의 IP를 검색해 접속한 유저가 해당하는지 검사.(내부에서 에러값 설정)
    if( SiKBlockIPList()->CheckBlockIP( dwIP, kPacket.m_bNewUser, kPacket.m_cAuthLevel, kPacket.m_bBlockIPPass ) == false )
    {
        std::string strNationCode;
        std::wstring wstrNationCode;

        SiKBlockIPList()->CheckUserCountry( dwIP, strNationCode );
        wstrNationCode = KncUtil::toWideString( strNationCode );
        // 블럭 ip유저 로그 추가.
        InsertBlockIPUserLog( kPacket.m_strLogin, dwIP, kPacket_.m_strIP, kPacket.m_bNewUser, wstrNationCode );

        START_LOG( cerr, L"User's DWORD IP Address : " << dwIP )
            << BUILD_LOG( kPacket_.m_strIP )
            << BUILD_LOG( kPacket.m_bNewUser ) << END_LOG;

        SET_ERR_GOTO( ERR_NUSER_VERIFY_14, verify_end );
    }

    if( !SiKCenterServer()->CheckScriptChecksum( m_kODBCMain, kPacket_.m_mapScriptCheckInfo, kPacket.m_strLogin, kPacket.m_vecScriptCheckResult ) )
    {
        SET_ERR_GOTO( ERR_NUSER_VERIFY_15, verify_end );
    }

    // DB 내에 해당 아이디의 데이터가 존재하는지 확인한다.
    DBJIF( m_kODBC.Query( L"{ call dbo.up_query_user_passwd ( N'%s' ) }",
        kPacket.m_strLogin.c_str() ) );
    if( m_kODBC.BeginFetch() ) //사용자 존재함
    {
        VERIFY_QUERY( m_kODBC >> wstrPasswd
                              >> kPacket.m_strLogin );   // 게임서버는 이것을 사용해 '내부인증'을 처리한다.
        m_kODBC.EndFetch();
/*
        if( kPacket_.m_iAge < SiKCenterSimLayer()->m_nOldUserAge )
        {
            SET_ERROR( ERR_NUSER_VERIFY_13 );
            START_LOG( cwarn, L" 연령 제한 Name : " << kPacket.m_strLogin << L"  "
                << kPacket_.m_iAge << L" < " << SiKCenterSimLayer()->m_nOldUserAge << END_LOG );
            goto verify_end;
        }
*/
        switch( kPacket_.m_dwAuthType ) {
                // DB에 있는 passwd값을 가져와서 클라이언트에게 알려주기만 한다. (아래에서 공통 처리됨)
                // 위험한데.. ㅡㅡ
            //case KSimLayer::AT_NTREEV:
            case KSimLayer::AT_SGI_EU:
            case KSimLayer::AT_GASH:
            {
                // NTreev Web에서 가져온 PW를 GC DB로 전송, 패스워드가 수정되었으면
                // 해당 패스워드를 GC DB에 Update해주는 작업을 진행한다.
                // 위 작업은 GameServer의 인증작업시 필요.
                std::wstring strDBPasswd = KncUtil::toWideString( kPacket_.m_strPasswd );
                DBJIF( m_kODBC.Query( L"{ call dbo.up_change_user_password ( N'%s', N'%s', N'%s' ) }",
                    kPacket.m_strLogin.c_str(), 
                    wstrPasswd.c_str(),
                    strDBPasswd.c_str()));

                if( m_kODBC.BeginFetch() )
                {
                    VERIFY_QUERY( m_kODBC >> nOK );
                    m_kODBC.EndFetch();
                    START_LOG ( clog, L"up_change_user_password Success")
                        << BUILD_LOG( nOK )
                        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
                }

                if( nOK != 0 )
                {
                    START_LOG( cerr, L"NTreev Web Password GCDB Update Failed" )
                        << BUILD_LOG( kPacket.m_strLogin)
                        << BUILD_LOG( wstrPasswd )
                        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strPasswd ) ) << END_LOG;
                    SET_ERROR( ERR_NUSER_VERIFY_20 );
                    goto verify_end;
                }
                break;
            }
            case KSimLayer::AT_LEVELUP:
                {
                    if ( !SiKCenterSimLayer()->CheckTypeFlag( KCenterSimLayer::ST_INTERNAL_TESOP ) ) {
                        START_LOG( clog, L"Before SHA1 :  " << KncUtil::toWideString( kPacket_.m_strPasswd ) )
                            << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strLogin ) ) << END_LOG;

                        std::wstring strSALT( L"" );
                        m_kHelper.GetSALT( kPacket.m_strLogin, strSALT );
                        std::wstring wstrPasswdMD5 = KncUtil::toWideString( kPacket_.m_strPasswd );

                        boost::to_upper( wstrPasswdMD5 );

                        kPacket_.m_strPasswd = SiKCenterSimLayer()->MakeSHA1( wstrPasswdMD5 + strSALT );

                        START_LOG( clog, L"SHA1 result :  " << KncUtil::toWideString( kPacket_.m_strPasswd ) )
                            << BUILD_LOG( strSALT )
                            << BUILD_LOG( wstrPasswdMD5 + strSALT )
                            << BUILD_LOG( kPacket_.m_strPasswd.length() ) << END_LOG;

                    }
                }
			case KSimLayer::AT_NTREEV:
            case KSimLayer::AT_NETMARBLE:
            case KSimLayer::AT_LEVELUP_PH:
            case KSimLayer::AT_MEGAXUS:
            {
                // DB의 값과, 클라이언트가 보낸 값을 비교해야 한다.
                std::string strDBPasswd = KncUtil::toNarrowString( wstrPasswd );
                std::string strLwDBPasswd;
                std::string strLwUserPasswd;
                std::transform( strDBPasswd.begin(), strDBPasswd.end(), std::back_inserter( strLwDBPasswd ), tolower );
                std::transform( kPacket_.m_strPasswd.begin(), kPacket_.m_strPasswd.end(), std::back_inserter( strLwUserPasswd ), tolower );
                if( strLwDBPasswd != strLwUserPasswd )
                {
                    START_LOG( clog, L"패스워드가 다름" )
                        << BUILD_LOG( kPacket_.m_dwAuthType )
                        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strLogin ) )
                        << BUILD_LOG( wstrPasswd )
                        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strPasswd ) ) << END_LOG;
                    SET_ERROR( ERR_NUSER_VERIFY_10 );
                    goto verify_end;
                }
                break;
            }
            // 별도의 인증 절차를 거쳐서 더이상 PW 확인 필요 하지 않는 경우.
            case KSimLayer::AT_FUNBOX_TH:
            case KSimLayer::AT_LIN:
            case KSimLayer::AT_CYOU:
                break;
        default:
            SET_ERROR( ERR_NUSER_VERIFY_05 );
            goto verify_end;
            break;
        }
    }
    else //사용자 존재안함
    {
        m_kODBC.EndFetch();
/*
        if( kPacket_.m_iAge < SiKCenterSimLayer()->m_nNewUserAge )
        {
            SET_ERROR( ERR_NUSER_VERIFY_13 );
            START_LOG( cwarn, L"신규 연령 제한 Name : " << kPacket.m_strLogin << L"  "
                << kPacket_.m_iAge << L" < " << SiKCenterSimLayer()->m_nNewUserAge << END_LOG );
            goto verify_end;
        }
*/
        switch( kPacket_.m_dwAuthType ) {
            case KSimLayer::AT_NTREEV:
            case KSimLayer::AT_SGI_EU:
            // 20090601 tgkwon
            // Entreev에서 계정 확인이 끝난 유저에 대해서 GC DB에 없으면 해당 유저 계정을
            // Insert 해주는 작업을 추가한다.
            wstrPasswd = KncUtil::toWideString( kPacket_.m_strPasswd );

             /*
            20081108. microcat.
            L"{ call dbo.up_insert_new_user_20130402 ( N'%s', N'%s', %d, %d ) }"

            { call dbo.up_insert_new_user_20130402 ( @01, @02, @03, @04 ) }
            @01: @01strLogin_input      nvarchar( 40 )
            @02: @02strPassword_input   nvarchar( 32 )
            @03: @03bSex_input          bit
            @04: @04iChannelingID_input tinyint = 1

            1 select ( @1 )
            @1: OK int
            0       : 성공
            -1      : 유저가 이미 존재함
            -101이하: 트랜잭션 에러
            */
            if( !m_kODBC.Query(L"{ call dbo.up_insert_new_user_20130402 ( N'%s', N'%s', %d, %d ) }", 
                kPacket.m_strLogin.c_str(),
                wstrPasswd.c_str(),
                kPacket_.m_bMale ? 0 : 1 ),
                kPacket_.m_dwChannellingType )
            {                
                START_LOG( cerr, L"신규유저 등록 실패" )
                    << BUILD_LOG( kPacket.m_strLogin )
                    << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strPasswd ) )
                    << BUILD_LOG( kPacket_.m_bMale )
                    << BUILD_LOG( kPacket_.m_dwAuthType )
                    << BUILD_LOG( (wchar_t*)(m_kODBC.m_szDiagMsg) )
                    << BUILD_LOG( (wchar_t*)(m_kODBC.m_sqlState) )
                    << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
                SET_ERROR( ERR_NUSER_VERIFY_12 );
                goto verify_end;
            }

            if( m_kODBC.BeginFetch() ) //  080723  woosh. 새로 추가
            {
                int nOK = -99;
                VERIFY_QUERY( m_kODBC >> nOK );
                m_kODBC.EndFetch();
                if( nOK != 0 )
                {
                    START_LOG( cerr, L"신규유저 등록 실패" )
                        << BUILD_LOG( nOK )
                        << BUILD_LOG( kPacket.m_strLogin )
                        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strPasswd ) )
                        << BUILD_LOG( kPacket_.m_bMale )
                        << BUILD_LOG( kPacket_.m_dwAuthType )
                        << BUILD_LOG( (wchar_t*)(m_kODBC.m_szDiagMsg) )
                        << BUILD_LOG( (wchar_t*)(m_kODBC.m_sqlState) )
                        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
                    SET_ERROR( ERR_NUSER_VERIFY_12 );
                    goto verify_end;
                }
            }
            break;

            //웹에서 계정 생성을 요청해야 한다.
            case KSimLayer::AT_LEVELUP:
            case KSimLayer::AT_LEVELUP_PH:
            case KSimLayer::AT_MEGAXUS:
            if( !SiKCenterSimLayer()->CheckTypeFlag( KCenterSimLayer::ST_INTERNAL_TESOP ) )
            {
                // 웹에서 신규 계정 생성이 완료되어 있어야 한다.
                SET_ERROR( ERR_NUSER_VERIFY_11 );
                goto verify_end;
                break;
            }

            case KSimLayer::AT_GASH:
            case KSimLayer::AT_NETMARBLE:
            case KSimLayer::AT_FUNBOX_TH:
            case KSimLayer::AT_LIN:
            case KSimLayer::AT_CYOU:
                if( kPacket_.m_bExpAccount )
                {
                    wstrPasswd = KncUtil::toWideString( kPacket_.m_strPasswd );
                    if( !m_kODBC.Query( L"{ call dbo.EAG_experience_account_create ( N'%s', N'%s', %d ) }",
                        wstrExpLogin.c_str(),   // 위에서 식별자 넣기전 계정을 넣는다
                        wstrPasswd.c_str() ) )
                    {
                        START_LOG( cerr, L"체험계정 신규유저 등록 실패" )
                            << BUILD_LOG( wstrExpLogin )
                            << BUILD_LOG( kPacket.m_strLogin )
                            << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strPasswd ) )
                            << BUILD_LOG( kPacket_.m_bMale )
                            << BUILD_LOG( kPacket_.m_dwAuthType )
                            << BUILD_LOG( (wchar_t*)(m_kODBC.m_szDiagMsg) )
                            << BUILD_LOG( (wchar_t*)(m_kODBC.m_sqlState) )
                            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
                        SET_ERROR( ERR_NUSER_VERIFY_12 );
                        goto verify_end;
                    }

                    if( m_kODBC.BeginFetch() ) //  080723  woosh. 새로 추가
                    {
                        int nOK = -99;
                        VERIFY_QUERY( m_kODBC >> nOK );
                        m_kODBC.EndFetch();

                        switch( nOK )
                        {
                        case 0:
                            START_LOG( cerr, L"체험 계정 생성 성공"  ) << END_LOG;
                            break;
                        case -1:
                            SET_ERROR( ERR_NUSER_VERIFY_12 );
                            START_LOG( cerr, L"체험계정리스트에 이미 존재하는 계정"  ) << END_LOG;
                            goto verify_end;
                            break;
                        case -2:
                            SET_ERROR( ERR_NUSER_VERIFY_12 );
                            START_LOG( cerr, L"체험계정의 게임 정보가 이미 존재하는 계정"  ) << END_LOG;
                            goto verify_end;
                            break;
                        }
                    }
                }
                else
                {
                    wstrPasswd = KncUtil::toWideString( kPacket_.m_strPasswd );

                    /*
                    20081108. microcat.
                    L"{ call dbo.up_insert_new_user_20130402 ( N'%s', N'%s', %d, %d ) }"

                    { call dbo.up_insert_new_user_20130402 ( @01, @02, @03, @04 ) }
                    @01: @01strLogin_input      nvarchar( 40 )
                    @02: @02strPassword_input   nvarchar( 32 )
                    @03: @03bSex_input          bit
                    @04: @04iChannelingID_input tinyint = 1

                    1 select ( @1 )
                    @1: OK int
                    0       : 성공
                    -1      : 유저가 이미 존재함
                    -101이하: 트랜잭션 에러
                    */
                    if( !m_kODBC.Query(L"{ call dbo.up_insert_new_user_20130402 ( N'%s', N'%s', %d, %d ) }", 
                        kPacket.m_strLogin.c_str(),
                        wstrPasswd.c_str(),
                        kPacket_.m_bMale ? 0 : 1,
                        kPacket_.m_dwChannellingType ) )
                    {                
                        START_LOG( cerr, L"신규유저 등록 실패" )
                            << BUILD_LOG( kPacket.m_strLogin )
                            << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strPasswd ) )
                            << BUILD_LOG( kPacket_.m_bMale )
                            << BUILD_LOG( kPacket_.m_dwAuthType )
                            << BUILD_LOG( (wchar_t*)(m_kODBC.m_szDiagMsg) )
                            << BUILD_LOG( (wchar_t*)(m_kODBC.m_sqlState) )
                            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
                        SET_ERROR( ERR_NUSER_VERIFY_12 );
                        goto verify_end;
                    }

                    if( m_kODBC.BeginFetch() ) //  080723  woosh. 새로 추가
                    {
                        int nOK = -99;
                        VERIFY_QUERY( m_kODBC >> nOK );
                        m_kODBC.EndFetch();
                        if( nOK != 0 )
                        {
                            START_LOG( cerr, L"신규유저 등록 실패" )
                                << BUILD_LOG( nOK )
                                << BUILD_LOG( kPacket.m_strLogin )
                                << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strPasswd ) )
                                << BUILD_LOG( kPacket_.m_bMale )
                                << BUILD_LOG( kPacket_.m_dwAuthType )
                                << BUILD_LOG( (wchar_t*)(m_kODBC.m_szDiagMsg) )
                                << BUILD_LOG( (wchar_t*)(m_kODBC.m_sqlState) )
                                << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
                            SET_ERROR( ERR_NUSER_VERIFY_12 );
                            goto verify_end;
                        }
                    }
                }
                break;
            default:
               SET_ERROR( ERR_NUSER_VERIFY_05 );
               goto verify_end;
                break;
        }
    }

    kPacket.m_dwAuthTick = ::GetTickCount(); // DB로 부터 TickCount를 받아 온다.

    DBJIF( m_kODBC.Query( L"{ call dbo.gup_user_auth_value_set ( N'%s', %d ) }",
        kPacket.m_strLogin.c_str(), (int)(kPacket.m_dwAuthTick) ) );
    if( m_kODBC.BeginFetch() )
    {
        int nOK = -99;
        VERIFY_QUERY( m_kODBC >> nOK );
        LOG_SUCCESS( nOK == 0 )
            << BUILD_LOG( nOK )
            << BUILD_LOG( kPacket.m_strLogin )
            << BUILD_LOG( kPacket.m_dwAuthTick )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << BUILD_LOG( (wchar_t*)(m_kODBC.m_szDiagMsg) )
            << BUILD_LOG( (wchar_t*)(m_kODBC.m_sqlState) ) << END_LOG;
        m_kODBC.EndFetch();
    }

    /* 20081023. microcat.미국 한정(현지사내,현지본섭)

    { call dbo.IBGA_ip_block_check_pass_user ( N'%s' ) }

    { call dbo.IBGA_ip_block_check_pass_user ( @1 ) }
    @1 ; Login nvarchar(20)

    1 return ( @1 )
    @1 ; OK int

    OK
    0 ; 이 유저는 ip 블럭 통과 유저입니다.
    -1 ; 이 유저의 계정 정보가 없습니다.
    -2 ; 이 유저의 ip 블럭 통과 정보가 없습니다.
    */
    if( SiKCenterSimLayer()->GetBlockIPPassCheck() == true ) // Block IP 패스여부 체크하는지?(미국은 한다)
    {
        DBJIF( m_kODBC.Query( L"{ call dbo.IBGA_ip_block_check_pass_user ( N'%s' ) }",
            kPacket.m_strLogin.c_str() ) );

        if( m_kODBC.BeginFetch() )
        {
            int nOK = -99;
            VERIFY_QUERY( m_kODBC >> nOK );

            switch( nOK )
            {
            case -2:
                kPacket.m_bBlockIPPass = false;
                START_LOG( clog, L"BlockIP Pass User? : " << kPacket.m_bBlockIPPass << L"(" << nOK << L")" << L", Login : " << kPacket.m_strLogin ) << END_LOG;
                break;
            case -1:
                // 계정정보가 없다(웹에서 신규 계정 생성이 완료되어 있어야 한다)
                SET_ERROR( ERR_NUSER_VERIFY_11 );
                goto verify_end;
                break;
            case 0:
                kPacket.m_bBlockIPPass = true;
                START_LOG( clog, L"BlockIP Pass User? : " << kPacket.m_bBlockIPPass << L"(" << nOK << L")" << L", Login : " << kPacket.m_strLogin ) << END_LOG;
                break;
            default:
                SET_ERROR( ERR_NUSER_VERIFY_05 );
                goto verify_end;
            }
        }
    }

    // AuthLevel 구하는 부분 순서 변경.
    {
        char cLastGrade = 0;
        char cCause = 0;
        time_t tmLimit = time_t(NULL);
        int nPeriodType;

        //로그인 -> UID
        DBJIF( m_kODBC.Query( L"{ call dbo.sp_executesql ( N'select LoginUID from dbo.Users with (nolock) where Login = @1', N'@1 nvarchar(80)', N'%s' ) }",
            kPacket.m_strLogin.c_str() ) );

        if( m_kODBC.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBC >> kPacket.m_dwUID );
            m_kODBC.EndFetch();
        }

        m_kHelper.User_auth_select( kPacket.m_dwUID, cLastGrade, kPacket.m_cAuthLevel, nPeriodType, tmLimit, cCause );
    }

    // beanfun db writing
    // mk8253.빈펀유저

    // { call dbo.UBGA_user_beanfun_insert ( %d) }
    // @1 ; iLoginUID_ [int]
    // 1 return ( @1 )
    // @1 ; OK
    // OK
    // 0 ; 성공
    // -1 ; 유저가존재하지않음
    // -101이하; 트랜잭션에러
    if( SiKCenterSimLayer()->CheckAuthTypeFlag(KSimLayer::AT_GASH) && kPacket_.m_bBeanfunUser == true )
    {
        DBJIF( m_kODBC.Query( L"{ call dbo.UBGA_user_beanfun_insert ( %d ) }",
            kPacket.m_dwUID ) );
        if( m_kODBC.BeginFetch() )
        {
            int nOK;
            VERIFY_QUERY( m_kODBC >> nOK );
            LOG_SUCCESS( nOK == 0 )
                << BUILD_LOG( nOK )
                << BUILD_LOG( kPacket.m_strLogin )
                << BUILD_LOG( (wchar_t*)(m_kODBC.m_szDiagMsg) )
                << BUILD_LOG( (wchar_t*)(m_kODBC.m_sqlState) ) << END_LOG;
            m_kODBC.EndFetch();
        }
    }

    // 초기화
    kPacket.m_kGuildInfo.Init();
    kPacket.m_kGuildUserInfo.Init();
    // 길드ID 받아오기
    kPacket.m_kGuildInfo.m_dwUID = GetGuildUIDFromDB( kPacket.m_dwUID );

    if ( kPacket.m_kGuildInfo.m_dwUID != 0 ) {

        KCnGuildPtr spGuild;
        spGuild = SiKCnGuildManager()->GetGuild( kPacket.m_kGuildInfo.m_dwUID );

        // 소속 길드 정보가 없으면 DB에서 받아오자.
        if ( !spGuild ) {
            UpdateOneGuildInfo( kPacket.m_kGuildInfo.m_dwUID );
            spGuild = SiKCnGuildManager()->GetGuild( kPacket.m_kGuildInfo.m_dwUID );
        }// end if

        JIF( spGuild ); // 위에서 없으면 무조건 생성하지만 안전코드.

        // 정규길드인가 확인
        spGuild->AutoUpgradeGuildGrade();

        // 길드정보 받아옴.
        kPacket.m_kGuildInfo = spGuild->GetGuildInfo();
        spGuild->GetNotice( kPacket.m_vecGuildNotice );

        // 맴버리스트 정보가 없으면 DB에서 맴버리스트를 다시 요청
        if ( spGuild->IsUserListEmpty() ) {
            UpdateOneGuildMemberList( kPacket.m_kGuildInfo.m_dwUID );
        }// end if

        // 길드정보 받아옴.( 한 번 더, 길드마스터 찾기/설정을 SetGuildMemberInfo 안에서 호출했다.
        kPacket.m_kGuildInfo = spGuild->GetGuildInfo();

        // 길드맴버 리스트 받아옴.
        //spGuild->GetMemberList( kPacket.m_mapGuildUserList ); // 게임서버에서 받아옴.
        // 내 길드맴버 정보 저장
        LIF( spGuild->GetUserInfo( kPacket.m_dwUID, kPacket.m_kGuildUserInfo ) );
    }// end if

    LIF( GetCharExpInfo( kPacket.m_dwUID, kPacket.m_mapCharExp ) );

    START_LOG( clog, L"길드 정보 , Login : " << kPacket.m_strLogin )
        << BUILD_LOG( kPacket.m_kGuildInfo.m_dwUID )
        << BUILD_LOG( kPacket.m_kGuildInfo.m_strName )
        << BUILD_LOG( kPacket.m_kGuildInfo.m_strMasterNick )
        << BUILD_LOGc( kPacket.m_kGuildUserInfo.m_cMemberLevel)
        << BUILD_LOG( kPacket.m_kGuildUserInfo.m_dwContributePoint )
        << BUILD_LOG( kPacket.m_mapGuildUserList.size() ) << END_LOG;

    if( SiKCenterSimLayer()->CheckAuthTypeFlag( KSimLayer::AT_CYOU ) && kPacket_.m_nOK == NetError::ERR_NUSER_VERIFY_37 ) {
        SET_ERROR( ERR_NUSER_VERIFY_37 );
    } else {
        SET_ERROR( NET_OK );
    }

    kPacket.m_strNMPasswd = kPacket_.m_strPasswd;

verify_end:
    if( SiKCenterSimLayer()->CheckAuthTypeFlag( KSimLayer::AT_CYOU ) && kPacket_.m_nOK == NetError::ERR_NUSER_VERIFY_02 ) {
        kPacket.m_nOK = NetError::ERR_NUSER_VERIFY_02;
    } else {
        kPacket.m_nOK = NetError::GetLastNetError();
    }

    SLayerType::GetInstance()->m_kActorManager2.QueueingEventTo( strLogin_, KNUserEvent::ENU_VERIFY_ACCOUNT_ACK, kPacket );

    std::wstringstream stmScriptChecksum;
    std::map<std::wstring,KScriptCheckInfo>::const_iterator cmit;
    for( cmit = kPacket_.m_mapScriptCheckInfo.begin() ; cmit != kPacket_.m_mapScriptCheckInfo.end() ; ++cmit )
    {
        stmScriptChecksum << L"{" << cmit->first << L"," << cmit->second.m_dwFileSize << L"," << cmit->second.m_strHash << L"},";
    }

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_strLogin )
        << BUILD_LOG( KncUtil::toWideString( kPacket.m_strNMPasswd ) )
        << BUILD_LOG( kPacket.m_dwAuthTick )
        << BUILD_LOG( kPacket.m_bExpAccount )
        << BUILD_LOG( kPacket.m_bBlockIPPass )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strLogin ) )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strPasswd ) )
        << BUILD_LOG( kPacket_.m_strIP )
        << BUILD_LOG( kPacket_.m_nProtocolVer )
        << BUILD_LOG( kPacket_.m_mapScriptCheckInfo.size() )
        << BUILD_LOG( stmScriptChecksum.str() )
        << BUILD_LOG( kPacket_.m_strServiceTicket )
        << BUILD_LOG( kPacket_.m_dwAuthType )
        << BUILD_LOG( kPacket_.m_bMale )
        << BUILD_LOG( kPacket_.m_iAge ) << END_LOG;
}

_INT_IMPL_ON_FUNC( DB_EVENT_FIRST_INIT_REQ, int )
{
    //서버 기동후 첫 DB작업(필수 데이터 읽어 오기)
    int nCharType = KItem::EICT_NO_TYPE;

    std::vector<KServerInfo> vecServerInfo;
    GetServerList( kPacket_, vecServerInfo );
    SiKCenterServer()->SetServerList( vecServerInfo );
    SiKCenterServer()->m_kChecksumManager.UpdateFromDB( m_kODBCMain, kPacket_, true );
    UpdateChannelNews();
    UpdateMiniGameRank();
    SiKCenterServer()->SetInitEvent(); // 초기 정보 읽기 완료 셋팅..
    ON_ECN_DB_RAINBOW_UPDATE_REQ( std::wstring(L""), 1, static_cast< char >( nCharType ) ); // 최초 호출시 UID 난에 1을 넣어줘야 함.

    // 09.07.26. kkurrung. Item 구매 여부 가져옴
    if( SiKCnItemManager()->IsBuyItemInfoEnable() )
    {
        std::vector<std::pair<DWORD,bool> > vecItemBuyInfo;
        std::map<DWORD,unsigned char> mapRecommInfo;
        GetItemBuyInfo( vecItemBuyInfo, mapRecommInfo );
        SiKCnItemManager()->UpdateBuyInfoFromDB( vecItemBuyInfo );
        SiKCnItemManager()->UpdateItemRecommInfo( mapRecommInfo );
    }

    std::vector<KGameCouponInfo> vecCouponInfo;
    if( UpdateCouponInfo( true, vecCouponInfo ) )
    {
        SiKCnCouponManager()->SetCouponData( vecCouponInfo );
    }

    ON_ECN_DB_MISSION_EVENT_REQ( std::wstring(L""), 1, static_cast< char >( nCharType ) );
    IntervalGuildRankUpdate();
    UpdateQuizBingoData( true ); // 빙고 데이터 가져 오기.

    {
        std::vector<KGWCSupportData> vecGWCData;
        LIF( GetGWCData( vecGWCData ) );
        SiKCnGWCSupport()->UpdateDataFromDB( vecGWCData );
    }

    // 구매제한 아이템 리스트 설정
    std::set<GCITEMID> setList;
    LIF( ItemLimit_select( setList ) );
    SiKCnItemManager()->UpdateBuyLimitItem( setList );

    int nVersion = SiKCnSphinx()->GetSphinxVersion();
    UpdateSphinxData( true, nVersion ); // 스핑크스 데이터 가져 오기.

    nVersion = SiKCnSurvey()->GetSurveyVersion();
    UpdateSurveyData( true, nVersion ); // 설문 데이터 가져오기

    UpdateGWCEventData(); // GWC 데이터 가져오기.

    UpdateAdventureEventData(); // 모험 이벤트 데이터 가져오기

    UpdateDBSettingInfo_AccAttendance(); // 누적 출석 DB세팅 데이터 가져오기

    // 캐시백 비율 정보 가져오기
    std::map< GCITEMID, int > mapCashbackDefaultRatio;
    GetCashbackDefaultAllRatio( mapCashbackDefaultRatio );
    SiKCnItemManager()->UpdateCashbackDefaultRatio( mapCashbackDefaultRatio );

    time_t tmStart, tmEnd;
    std::map< GCITEMID, int > mapCashbackEventRatio;
    if ( GetCashbackEventAllRatio( tmStart, tmEnd, mapCashbackEventRatio ) ) {
        SiKCnItemManager()->UpdateCashbackEventRatio( tmStart, tmEnd, mapCashbackEventRatio );
    }
    std::vector<KSimpleServerInfo> vecUseTCPServerInfo;
    std::vector<KSimpleServerInfo> vecNoUseTCPServerInfo;
    GetTCPServerList( vecUseTCPServerInfo, vecNoUseTCPServerInfo );
    SiKCenterServer()->SetTCPServerList( vecUseTCPServerInfo, vecNoUseTCPServerInfo );

	UpdateSocialCommerceData( true ); // 소셜커머스 DB설정 데이터 가져오기
}

_INT_IMPL_ON_FUNC( DB_CONNECTION_COUNT_RECORD_REQ, DWORD )
{
    if( kPacket_ == 0 ) return;

    KODBC* pkODBC = SiKCenterDBMgr()->GetODBC( "stat" );
    if( !pkODBC )
    {
        START_LOG( cerr, L"접속 카운트 기록중. Stat ODBC를 얻지 못했다.." )
            << BUILD_LOG( kPacket_ ) << END_LOG;
        return;
    }

    KLocker lock( pkODBC->GetCS() );
    KODBC& kODBC = *pkODBC;

    _DBJIF( kODBC.Query( L"{ call dbo.CES_client_execute_collect ( %d ) }",
        kPacket_ ), return, (&kODBC) );

    int nOK = -99;
    if( kODBC.BeginFetch() )
    {
        VERIFY_QUERY( kODBC >> nOK );
        kODBC.EndFetch();
    }

    START_LOG( clog, L" 접속 통계 기록 완료.. " )
        << BUILD_LOG( nOK )
        << BUILD_LOG( kPacket_ ) << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_MINIGAME_RANK_REQ )
{
    UpdateMiniGameRank();
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_GET_TOTAL_DONATION_POINT_REQ )
{
    //{ call dbo.CGGA_contribution_gp_select_total }
    //인자없음
    //
    //1 return ( @1 )
    //@1 ; TotalContributionGP bigint

    __int64 biTotalDonation = 0;
    DBJIF( m_kODBC.Query( L"{ call dbo.CGGA_contribution_gp_select_total }" ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> biTotalDonation );
    }
    m_kODBC.EndFetch();

    SiKCnDonationManager()->SetDonationFromDB( biTotalDonation );
    START_LOG( cerr, L"DB로 부터 총 기부액 가져옴. Point : " << biTotalDonation ) << END_LOG;

    std::vector<KDotantionRank> vecRank;
    GetDonationRanking( vecRank );
    SiKCnDonationManager()->UpdateDonationRankDB( vecRank );
    START_LOG( cerr, L"DB로 부터 기부 랭크 가져옴. Point : " << vecRank.size() ) << END_LOG;

    // DB로 부터 아이템 기부 등급 정보 가져오기.
    std::vector<KItemDonationInfo> vecDoInfo;
    // 처음 Center Server기동후에는 DB의 전체 리스트에 대해서 다 가져온다.
    GetItemDonation( true, vecDoInfo );
    SiKCnDonationManager()->UpdateItemDonationInfo( vecDoInfo );
}

_INT_IMPL_ON_FUNC( DB_UPDATE_SCRIPT_CHECKSUM_REQ, int )
{
    SiKCenterServer()->m_kChecksumManager.UpdateFromDB( m_kODBCMain, kPacket_, true );
}

_INT_IMPL_ON_FUNC( ECN_UPDATE_CONNECTION_COUNT_REQ, DWORD )
{
    AddServerCountData( kPacket_, CDT_CONNECTION_COUNT );
}

_INT_IMPL_ON_FUNC( ECN_DB_UPDATE_DISCONNECT_TYPE, MAP_INT_INT )
{
    if( kPacket_.empty() ) return;

    KODBC* pkODBC = SiKCenterDBMgr()->GetODBC( "stat" );
    if( !pkODBC )
    {
        START_LOG( cerr, L"종료 사유 카운트 기록중. Stat ODBC를 얻지 못했다.." )
            << BUILD_LOG( kPacket_.size() ) << END_LOG;
        return;
    }

    KLocker lock( pkODBC->GetCS() );
    KODBC& kODBC = *pkODBC;

    std::wstringstream stm;
    MAP_INT_INT::iterator mit;
    for( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit )
    {
        stm << mit->first << L","
            << mit->second << L",";
    }

    _DBJIF( kODBC.Query( L"{ call dbo.DTC_disconnect_type_collect ( N'%s' ) }",
        stm.str().c_str() ), return, (&kODBC) );

    int nOK = -99;
    if( kODBC.BeginFetch() )
    {
        VERIFY_QUERY( kODBC >> nOK );
    }
    kODBC.EndFetch();

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( kODBC.m_strLastQuery ) << END_LOG;
}


INT_IMPL_ON_FUNC_NOPARAM( DB_UPDATE_CHANNEL_NEWS_REQ )
{
    UpdateChannelNews();
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_DB_RAINBOW_UPDATE_REQ )
{
    time_t tmBegin(1), tmEnd(1);
    std::vector<KDailyEventData> vecDailyEvent;
    std::vector< KRainBowData > vecJackpotEventList;
    std::vector< KJackpotTimeList > vecJackpotTimeList;

    int nRainbowID = CheckRainbowEvent( (dwUID_ == 1?true:false), tmBegin, tmEnd );
    if( nRainbowID < 0 )
    {
        START_LOG( cwarn, L"레인 보우 이벤트 변경 사항 없음.. "  ) << END_LOG;
        return;
    }

    LIF( GetRainbowDailyEvent( nRainbowID, vecDailyEvent ) );
    LIF( GetJackpotEventList( nRainbowID, vecJackpotEventList) );
    LIF( GetJackpotTimeList( nRainbowID, vecJackpotTimeList ) );

    SiKCnRainBow()->DBRainbowEventUpdate( nRainbowID, tmBegin, tmEnd );
    SiKCnRainBow()->DBDailyEventUpdate( vecDailyEvent );
    SiKCnRainBow()->DBJacPotEventListUpdate( vecJackpotEventList );
    SiKCnRainBow()->DBJacPotTimeListUpdate( vecJackpotTimeList );
    if( dwUID_ == 1 ) { // 전체 정보를 불러오면 무조건 동기화 하자.
        SiKCnRainBow()->SetChangeTag( true );
    }
    SiKCnRainBow()->UpdateState();
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_DB_UPDATE_CHECK_REQ )
{
    std::vector<std::pair<DWORD,bool> > vecItemBuyInfo;
    std::map<DWORD,unsigned char > mapRecommInfo;
    std::set<GCITEMID> setList;
    std::map< GCITEMID, int > mapCashbackDefaultRatio;
    std::map< GCITEMID, int > mapCashbackEventRatio;
    time_t tmStart = time_t(NULL);
    time_t tmEnd = time_t(NULL);
    bool bDBUpdate = false;
    bool bItemLimit = false;
    bool bCashBack = false;

    if ( CheckDBUpdate( SHOW_WEB ) ) {
        // 09.07.26. kkurrung. Show Web 정보 가져 오기..
        if ( SiKCnItemManager()->IsBuyItemInfoEnable() ) {
            JIF( GetItemBuyInfo( vecItemBuyInfo, mapRecommInfo ) );
            SiKCnItemManager()->UpdateBuyInfoFromDB( vecItemBuyInfo );
            SiKCnItemManager()->UpdateItemRecommInfo( mapRecommInfo );
            SiKCnItemManager()->SendToItemUpdateNot( true );
            SiKCnItemManager()->SendItemBuyInfoNot();
        }
        bDBUpdate = true;
    }

    if ( CheckDBUpdate( ITEM_LIMIT ) ) {
        LIF( ItemLimit_select( setList ) );
        SiKCnItemManager()->UpdateBuyLimitItem( setList );
        SiKCnItemManager()->Send_BuyLimitItemListNot( setList );
        bItemLimit = true;
    }

    if ( CheckDBUpdate( CASHBACK ) ) {
        GetCashbackDefaultAllRatio( mapCashbackDefaultRatio );
        SiKCnItemManager()->UpdateCashbackDefaultRatio( mapCashbackDefaultRatio );
        if ( GetCashbackEventAllRatio( tmStart, tmEnd, mapCashbackEventRatio ) ) {
            SiKCnItemManager()->UpdateCashbackEventRatio( tmStart, tmEnd, mapCashbackEventRatio );
            SiKCnItemManager()->SendCashbackRatioInfo( mapCashbackDefaultRatio, mapCashbackEventRatio );
        }
        bCashBack = true;
    }

    if( CheckDBUpdate( VIPEVENT ) ) {
        SiKCnUpdatePlanManager()->SendEventUpdateNot( VIPEVENT );
    }

    START_LOG( clog, L"DB Update 확인." )
        << BUILD_LOG( bDBUpdate )
        << BUILD_LOG( bItemLimit )
        << BUILD_LOG( bCashBack )
        << BUILD_LOG( vecItemBuyInfo.size() )
        << BUILD_LOG( mapRecommInfo.size() )
        << BUILD_LOG( setList.size() )
        << BUILD_LOG( mapCashbackDefaultRatio.size() )
        << BUILD_LOG( mapCashbackEventRatio.size() )
        << BUILD_LOGtm( CTime( tmStart ) )
        << BUILD_LOGtm( CTime( tmEnd ) )
        << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_DB_MISSION_EVENT_REQ )
{
    time_t tmBegin(1), tmEnd(1);
    int nEvnetID = -1;
    int nUpdated = -1;
    int nMissionCheckHour = -1;
    std::vector<KMissionDailyData> vecDailyEvent;
    vecDailyEvent.clear();

    ///////////////////////////////////////////////////////////////////////////
    // 내일 DB에 해당 시간값에 대한 값 세팅 꼭 해두고 테스트 진행~! ( 0 ~ 24시간 )
    CheckMissionEvent( nEvnetID, tmBegin, tmEnd, nMissionCheckHour, nUpdated );

    // nUpdated값의 초기값이 0으로 세팅되어 있다.
    // 이 값이 1로 리턴되면 변경사항이 있다는 거임..,
    if( nUpdated < 0 ) {
        START_LOG( cwarn, L"미션 이벤트 변경 사항 없음.. "  ) << END_LOG;
        return;
    }

    // dwUID_는 처음 서버가 호출될 때에만 1이다.
    if( nUpdated == 0 && dwUID_ == 0 )
    {
        START_LOG( cwarn, L"미션 이벤트 변경 사항 없음.. "  ) << END_LOG;
        return;
    }

    // DB에 정보가 Update된 부분을 확인하면 다시 DB 프로시저를 호출해 줘야 한다.
    if( nUpdated == 1 )
    {
        START_LOG( cwarn, L"미션 이벤트 변경사항 확인하고 DB에 정보 요청.. "  ) << END_LOG;
        UpdateMissionEvent();
    }

    // 해당 정보를 DB에서 새로 가져온다.
    LIF( GetMissionDailyEvent( nEvnetID, vecDailyEvent ) );
    SiKCnMissionEvent()->DBMissionEventUpdate( nEvnetID, tmBegin, tmEnd, nMissionCheckHour );
    // 가져온 정보를 갱신시켜준다.
    SiKCnMissionEvent()->DBDailyEventUpdate( vecDailyEvent );
    // 새로운 정보를 갱신하는 작업을 진행한다.
    SiKCnMissionEvent()->UpdateState();
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_DB_SUBSCRIPTION_INFO_UPDATE_REQ )
{
    std::map< int, KSubscription > mapInfo;
    /*
    20090916. mk8253
    응모권 교환 이벤트

    { call SSGA_subscription_select }

    n return
    EventUID     진행중인 이벤트 UID
    ItemIDA      응모권 ItemID
    FactorA      1회 응모시 필요한 Factor
    ItemIDB      교환권 ItemID
    FactorB      교환권 Count
    FactorC      교환권 Period
    MaxCount     최대 교환권 수
    CurrentCount 남은 교환권 수
    NPCID        NPC ID
    */
    DBJIF( m_kODBC.Query( L"{ call SSGA_subscription_select }" ) );

    while( m_kODBC.Fetch() !=  SQL_NO_DATA )
    {
        int nEventID = -99;
        KSubscription kData;

        VERIFY_QUERY( m_kODBC >> nEventID
                              >> kData.m_dwTicketItemID
                              >> kData.m_nNeedTicketNum
                              >> kData.m_kRewardItem.m_ItemID
                              >> kData.m_kRewardItem.m_nDuration
                              >> kData.m_kRewardItem.m_nPeriod
                              >> kData.m_dwMaxGiftNum
                              >> kData.m_nCurrentGiftNum
                              >> kData.m_dwNPCID );

        mapInfo[nEventID] = kData;

        START_LOG( clog, L"응모권 교환 이벤트 DB정보 받아오기." )
            << BUILD_LOG( nEventID )
            << BUILD_LOG( kData.m_dwTicketItemID )
            << BUILD_LOG( kData.m_nNeedTicketNum )
            << BUILD_LOG( kData.m_kRewardItem.m_ItemID )
            << BUILD_LOG( kData.m_kRewardItem.m_nDuration )
            << BUILD_LOG( kData.m_kRewardItem.m_nPeriod )
            << BUILD_LOG( kData.m_dwMaxGiftNum )
            << BUILD_LOG( kData.m_nCurrentGiftNum )
            << BUILD_LOG( kData.m_dwNPCID ) << END_LOG;
    }

    // 이벤트 설정
    SiKCnSubscriptionManager()->UpdateEventInfoFromDB( mapInfo );
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_DB_NPC_GIFTS_UPDATE_REQ )
{
    std::set<int> setEventID;
    std::map<int, std::vector< KDropItemInfo > > mapEventInfo;

    /*
    20090814. mk8253
    1일 1회 아이템 지급 발동 이벤트
    { call dbo.DSEV_daily_supply_event }
    입력인자 없음

    n return
    @1 ; EventUID
    */
    DBJIF( m_kODBC.Query( L"{ call dbo.DSEV_daily_supply_event }" ) );

    while( m_kODBC.Fetch() !=  SQL_NO_DATA )
    {
        int nEventID = -99;
        VERIFY_QUERY( m_kODBC >> nEventID );

        setEventID.insert( nEventID );
    }

    /*
    20090814. mk8253
    이벤트별 지급 아이템
    { call dbo.DSEV_daily_supply_item ( @1 ) }
    @1 ; @EventUID

    n return
    @1 ; ItemID
    @2 ; Count
    @2 ; Period
    */
    for( std::set<int>::iterator sit = setEventID.begin() ; sit != setEventID.end() ; ++sit )
    {
        std::vector< KDropItemInfo >  vecItem;

        DBJIF( m_kODBC.Query( L"{ call dbo.DSEV_daily_supply_item ( %d ) }",
            *sit ) );

        while( m_kODBC.Fetch() !=  SQL_NO_DATA )
        {
            KDropItemInfo kData;

            VERIFY_QUERY( m_kODBC >> kData.m_ItemID
                                  >> kData.m_nDuration
                                  >> kData.m_nPeriod );
            vecItem.push_back( kData );
        }
        mapEventInfo[*sit] = vecItem;
    }

    // 이벤트 설정(최초,갱신)
    SiKCnNpcGift()->UpdateEventInfo( mapEventInfo );
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_DICE_PLAY_TOTAL_RANK_REQ )
{
    /*
    mk8253. 20091010
    가장멀리간top3 유저의말판정보

    L"{ call dbo.DPGADicePlay_top3_select ( %d, %d ) }"

    { call dbo.DPGADicePlay_top3_select ( @1, @2 ) }
    @1 ; @iType_     ; 해당 이벤트의 Type( 할로윈 주사위 :3, 2번째 주사위 이벤트 : 4 )
    @2 ; @iTurnCount ; 전체 바퀴수

    n return ( @1, @2, @3 )
    @1 ; TurnCount
    @2 ; Position
    @3 ; NickName
    */

    std::vector<KDicePlayRank> vecRank;
    DBJIF( m_kODBC.Query( L"{ call dbo.DPGADicePlay_top3_select( %d, %d ) }",
        DICEPLAYTYPE, DICEPLAYTOTALCOUNT) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KDicePlayRank dicePlayRank;
        VERIFY_QUERY( m_kODBC >> dicePlayRank.m_dwTotalCount
                              >> dicePlayRank.m_dwCurrentPosition
                              >> dicePlayRank.m_strNickName );

        vecRank.push_back( dicePlayRank );
    }
    m_kODBC.EndFetch();

    SiKCnDicePlay()->UpdateDicePlayRankDB( vecRank );
    START_LOG( cerr, L"DB로 부터 주사위 이벤트 Ranking정보 가져옴." << vecRank.size() ) << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_DB_ITEM_DONATION_UPDATE )
{
    std::vector<KItemDonationInfo> vecDoInfo;
    
    GetItemDonation( false, vecDoInfo );

    // DB에 저장된 Grade List를 불러오는 데 갱신된 정보가 없으면
    // 리턴된 vector값이 비어있다. 비었을 경우에는 이벤트 변경사항 없음.
    if( vecDoInfo.empty() )
    {
        START_LOG( cwarn, L"송편 기부 이벤트 변경 사항 없음.. "  ) << END_LOG;
        return;
    }

    // 변경된 사항이 있으므로 DB에 전체정보를 가져와서 넣어주자.
    GetItemDonation( true, vecDoInfo );
    SiKCnDonationManager()->UpdateItemDonationInfo( vecDoInfo );
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_COUPON_INFO_UPDATE_REQ )
{
    std::vector<KGameCouponInfo> vecCouponInfo;
    if( UpdateCouponInfo( false, vecCouponInfo ) )
    {
        SiKCnCouponManager()->AddCouponData( vecCouponInfo );
    }
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_DB_QUIZ_BINGO_DATA_NOT )
{
    // DB에서 변경 된 정보 있으면 새로 퍼온다. 문제 변경 시 새로 퍼오기 위해서 수정 함.
    if( CheckDBUpdate( QUIZ_BINGO ) ) {
        UpdateQuizBingoData( true );
    }
    else
    {
        UpdateQuizBingoData( false );
    }
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_DB_UPDATE_GWC_DATA )
{
    std::vector<KGWCSupportData> vecGWCData;
    LIF( GetGWCData( vecGWCData ) );
    SiKCnGWCSupport()->UpdateDataFromDB( vecGWCData );
}

// new guild
INT_IMPL_ON_FUNC_NOPARAM( DB_ECN_UPDATE_GUILD_LIST_REQ )
{
    std::map< DWORD, KNGuildInfo >::iterator mitGuild;
    std::map< DWORD, std::vector<KNGuildNotice> >::iterator mitNotice;
    std::vector<KNGuildNotice>::iterator vit;
    std::map< DWORD, KSimpleGuildRank >::iterator mitRank;
    std::vector<KNGuildRankInfo>::iterator vitRank;

    std::map< DWORD, KNGuildInfo > mapGuildList;
    std::map< DWORD, std::vector<KNGuildNotice> > mapGuildNotice;
    std::map< DWORD, KSimpleGuildRank > mapGuildRank;
    KNGuildUserInfo kMasterInfo;
    kMasterInfo.Init();

    // 전체 길드 정보 받아오기
    if ( GetAllGuildInfo( mapGuildList ) == false ) {
        START_LOG( cerr, L"DB에서 전체 길드 정보를 읽어오지 못했습니다." ) << END_LOG;
        return;
    }// end if

    // 길드마스터 이름 설정
    for ( mitGuild = mapGuildList.begin() ; mitGuild != mapGuildList.end() ; ++mitGuild ) {
        kMasterInfo.Init();
        if ( GetGuildMasterInfoFromDB( mitGuild->second.m_dwUID, kMasterInfo ) == false ) {
            START_LOG( cerr, L"길드마스터 정보를 가져오지 못함. DB처리 오류. GuildUID : " << mitGuild->second.m_dwUID ) << END_LOG;
            continue;
        }
        mitGuild->second.m_strMasterNick = kMasterInfo.m_strNickName;
        mitGuild->second.m_strMasterNickColor = kMasterInfo.m_strNickColor;
    }

    // 전체 길드 공지 받아오기
    if ( GetAllGuildNotice( mapGuildNotice ) == false ) {
        START_LOG( cerr, L"DB에서 전체 길드 공지를 읽어오지 못했습니다." ) << END_LOG;
        return;
    }// end if

    // 랭킹 데이터 받아오기
    SiKCnGuildManager()->GetSimpleGuildRank( mapGuildRank );
    START_LOG( clog, L"랭킹 데이터 받아옴. size : " << mapGuildRank.size() ) << END_LOG;

    // 길드 추가 설정
    for ( mitGuild = mapGuildList.begin() ; mitGuild != mapGuildList.end() ; ++mitGuild ) {
        DWORD dwGuildUID = mitGuild->first;

        // 소개말
        mitNotice = mapGuildNotice.find( dwGuildUID );
        if ( mitNotice != mapGuildNotice.end() ) {
            // 길드 공지 찾아서
            vit = std::find_if( mitNotice->second.begin(), mitNotice->second.end(),
                boost::bind(&KNGuildNotice::m_nMsgID,_1) == KNGuildNotice::GN_COMMENT );

            if ( vit != mitNotice->second.end() ) {
                // 소개말 설정
                mitGuild->second.m_strComment = vit->m_strMsg;
//                 START_LOG( clog, L"길드정보에 소개말 설정함. GuildUID : " << mitNotice->first )
//                     << BUILD_LOG( vit->m_strMsg ) << END_LOG;
            }
        }

        // 랭킹
        mitRank = mapGuildRank.find( dwGuildUID );
        if ( mitRank != mapGuildRank.end() ) {
            mitGuild->second.m_nWeeklyRank = mitRank->second.m_nWeeklyRank;
            mitGuild->second.m_nMonthlyRank = mitRank->second.m_nMonthlyRank;
            mitGuild->second.m_nTotalRank = mitRank->second.m_nTotalRank;

//             START_LOG( clog, L"길드정보에 랭킹 설정함. GuildUID : " << dwGuildUID )
//                 << BUILD_LOG( mitGuild->second.m_nWeeklyRank )
//                 << BUILD_LOG( mitGuild->second.m_nMonthlyRank )
//                 << BUILD_LOG( mitGuild->second.m_nTotalRank ) << END_LOG;
        }
    }

    // 길드 정보 설정
    SiKCnGuildManager()->AddGuild( mapGuildList );

    // 길드정보 설정하여 Page Build후 바로 삭제하기 때문에 공지를 설정하지 않는다.
    // for ( mitNotice = mapGuildNotice.begin() ; mitNotice != mapGuildNotice.end() ; ++mitNotice ) {
    //     LIF( SiKCnGuildManager()->AddGuildNotice( mitNotice->first, mitNotice->second ) );
    // }// end for

    START_LOG( cerr, L"DB에서 길드 정보 받아옴." )
        << BUILD_LOG( mapGuildList.size() )
        << BUILD_LOG( mapGuildNotice.size() ) << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( DB_ECN_UPDATE_GUILD_INFO_CHECK_REQ )
{
    std::map<UCHAR,UCHAR>::iterator mit;
    std::map<UCHAR,UCHAR> mapUpdateCheck;

    if ( UpdateGuildInfoFromDB( mapUpdateCheck ) == false ) {
        START_LOG( cerr, L"DB에서 길드 업데이트 정보를 읽어오지 못했습니다." ) << END_LOG;
        return;
    }// end if

    if ( mapUpdateCheck.empty() ) return;

    for ( mit = mapUpdateCheck.begin() ; mit != mapUpdateCheck.end() ; ++mit ) {
        if ( mit->second == KCnGuildManager::UC_NOT_CHANGED ) continue;

        switch (mit->first)
        {

        case KCnGuildManager::UT_GUILD_RANK:
            IntervalGuildRankUpdate();
            // newbie Guild destroy Check
            IntervalGuildRemveCheck();
            break;

        case KCnGuildManager::UT_GUILD_MARK:
            IntervalGuildMarkCheck();
            break;

        default:
            START_LOG( cerr, L"등록되지 않은 길드 업데이트 체크 타입 : " << (int)(mit->first) ) << END_LOG;
        }
    }
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_GUILD_MARK_INFO_UPDATE_REQ )
{
    KNGuildMarkInfo kPacket;
    kPacket.m_dwGuildUID = dwUID_;
    if ( GetGuildMarkInfo( kPacket.m_dwGuildUID, kPacket.m_ucMarkStatus, kPacket.m_nMarkRevision, kPacket.m_strFileName ) == false ) {
        START_LOG( cerr, L"길드 마크 정보를 가져 오지 못 했음.. GuildUID : " << dwUID_ ) << END_LOG;
        // 길드 마크 정보가 없는데 그냥 리턴 해야지..
        return;
    }

    SiKCnGuildManager()->QueueingEvent( KCenterEvent::ECN_GUILD_MARK_INFO_UPDATE_ACK, kPacket.m_dwGuildUID, kPacket );
}


INT_IMPL_ON_FUNC( ECN_JOIN_GUILD_REQ )
{
    SET_ERROR( ERR_UNKNOWN );

    KECN_JOIN_GUILD_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_kGuildInfo = kPacket_.m_kGuildInfo;
    kPacket.m_kGuildUserInfo.Init();

    KNGuildInfo kInfo;
    kInfo.Init();

    KCnGuildPtr spGuild = SiKCnGuildManager()->GetGuild( kPacket_.m_dwGuildUID );
    if ( spGuild == NULL ) {
        // 길드 정보가 없으면 DB에서 불러오자.
        LIF( UpdateOneGuildInfo( kPacket_.m_dwGuildUID ) );
        UpdateOneGuildMemberList( kPacket_.m_dwGuildUID );

        spGuild = SiKCnGuildManager()->GetGuild( kPacket_.m_dwGuildUID );
        if ( spGuild == NULL ) {
            START_LOG( cerr, L"길드가 없음 GuildID : "<< kPacket_.m_dwGuildUID ) << END_LOG;
            SET_ERR_GOTO( ERR_GUILD_JOIN_03, END_PROC );
        }
    }

    // 길드정보 받아오기
    kInfo = spGuild->GetGuildInfo();
    kPacket_.m_kGuildInfo = kInfo;
    kPacket.m_kGuildInfo = kInfo;

    // 길드가입중지인가?
    if ( kInfo.m_ucJoinSetting == KNGuildInfo::JS_STOP_JOIN ) {
        SET_ERR_GOTO( ERR_GUILD_JOIN_04, END_PROC );
    }

    // 가입후 승인인데,
    if ( kInfo.m_ucJoinSetting == KNGuildInfo::JS_AFTER_DECISION ) {

        // 대기자수가 한도치를 넘는가?
        if ( spGuild->GetJoinerCount() >= (UINT)(spGuild->GetMaxJoinerNum()) ) {

            START_LOG( cerr, L"길드가입시 최대 대기자수가 다찼음." )
                << BUILD_LOG( spGuild->GetJoinerCount() )
                << BUILD_LOG( spGuild->GetMaxJoinerNum() ) << END_LOG;

            SET_ERR_GOTO( ERR_GUILD_JOIN_07, END_PROC );
        }
    }

    // 바로 가입인데, 길드맴버수가 한도치를 넘는가?
    if ( kInfo.m_ucJoinSetting == KNGuildInfo::JS_AUTO_ACCEPT ) {
        if ( kInfo.m_nNumMembers >= spGuild->GetMaxMemberNum() ) {

            START_LOG( cerr, L"길드가입시 최대 맴버수가 다찼음." )
                << BUILD_LOG( kInfo.m_nNumMembers )
                << BUILD_LOG( spGuild->GetMemberCount() )
                << BUILD_LOG( spGuild->GetJoinerCount() )
                << BUILD_LOG( spGuild->GetMaxJoinerNum() ) << END_LOG;

            SET_ERR_GOTO( ERR_GUILD_JOIN_08, END_PROC );
        }
    }

    // 길드가입 처리
    int nRet = UpdateJoinGuild( kPacket_.m_dwUserUID, kPacket_.m_dwGuildUID,kPacket_.m_strMyComment );

    // 에러값 처리
    switch ( nRet )
    {
    case 0: // 성공
        SET_ERROR( NET_OK );
        break;
    case -1: // 유저정보 없음
        SET_ERROR( ERR_GUILD_CREATE_03 );
        break;
    case -2: // 길드정보 없음
        SET_ERROR( ERR_GUILD_CREATE_04 );
        break;
    case -3: // 가입중지상태임
        SET_ERROR( ERR_GUILD_CREATE_05 );
        break;
    case -4: // 이미 가임유저임
        SET_ERROR( ERR_GUILD_CREATE_06 );
        break;
    default: // DB처리 에러
        SET_ERROR( ERR_GUILD_CREATE_08 );
        break;
    }

    if ( nRet != 0 ) {
        START_LOG( cerr, L"길드 가입 실패. nRet : " << nRet ) << END_LOG;
        goto END_PROC;
    }

    // 여기서부터는 길드 가입 성공.
    LIF( UpdateJoinGuildMemberInfo( kPacket_.m_dwGuildUID, kPacket_.m_dwUserUID, kPacket.m_kGuildUserInfo ) );
    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_strMyComment )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kGuildUserInfo.m_dwGuildUID )
        << BUILD_LOGc( kPacket.m_kGuildUserInfo.m_cMemberLevel ) << END_LOG;


    SiKCenterSimLayer()->m_kActorManager.QueueingEventTo( dwUID_, KCenterEvent::ECN_JOIN_GUILD_ACK, kPacket );
}

_INT_IMPL_ON_FUNC( ECN_CANCEL_JOIN_GUILD_REQ, PAIR_DWORD )
{
    SET_ERROR( ERR_UNKNOWN );

    KECN_CANCEL_JOIN_GUILD_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwUserUID = kPacket_.first;
    kPacket.m_dwGuildUID = kPacket_.second;

    if ( IsExistGuild( kPacket.m_dwGuildUID ) == false ) {
        START_LOG( cerr, L"길드가 없음 GuildID : "<< kPacket.m_dwGuildUID ) << END_LOG;
        SET_ERR_GOTO( ERR_GUILD_CANCEL_JOIN_01, END_PROC );
    }

    // 길드가입 취소처리
    int nRet = UpdateDrumoutGuildMember( kPacket.m_dwUserUID, kPacket.m_dwGuildUID );

    // 에러값 처리
    switch ( nRet )
    {
    case 0: // 성공
        SET_ERROR( NET_OK );
        break;
    case -1: // 유저정보 없음
        SET_ERROR( ERR_GUILD_CANCEL_JOIN_00 );
        break;
    case -2: // 길드정보 없음
        SET_ERROR( ERR_GUILD_CANCEL_JOIN_01 );
        break;
    case -3: // 탈퇴 등급 아님(길마 혹은 DB에 등록되지 않은 등급)
        SET_ERROR( ERR_GUILD_CANCEL_JOIN_02 );
        break;
    default: // DB처리 에러
        SET_ERROR( ERR_GUILD_CANCEL_JOIN_03 );
        break;
    }

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwGuildUID ) << END_LOG;

    //QUEUING_ACK_TO_USER( ECN_CANCEL_JOIN_GUILD_ACK );
    SiKCenterSimLayer()->m_kActorManager.QueueingEventTo( dwUID_, KCenterEvent::ECN_CANCEL_JOIN_GUILD_ACK, kPacket );
}

_INT_IMPL_ON_FUNC( ECN_SELF_DRUMOUT_GUILD_REQ, PAIR_DWORD )
{
    SET_ERROR( ERR_UNKNOWN );

    DWORD dwUserUID = kPacket_.first;
    DWORD dwGuildUID = kPacket_.second;

    KECN_SELF_DRUMOUT_GUILD_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwUserUID = dwUserUID;
    kPacket.m_dwGuildUID = dwGuildUID;

    if ( IsExistGuild( kPacket.m_dwGuildUID ) == false ) {
        START_LOG( cerr, L"길드가 없음 GuildID : "<< kPacket.m_dwGuildUID ) << END_LOG;
        SET_ERR_GOTO( ERR_GUILD_SELF_DRUMOUT_01, END_PROC );
    }

    // 길드 탈퇴신청 처리
    int nRet = UpdateDrumoutGuildMember( dwUserUID, dwGuildUID );

    // 에러값 처리
    switch ( nRet )
    {
    case 0: // 성공
        SET_ERROR( NET_OK );
        break;
    case -1: // 유저정보 없음
        SET_ERROR( ERR_GUILD_SELF_DRUMOUT_00 );
        break;
    case -2: // 길드정보 없음
        SET_ERROR( ERR_GUILD_SELF_DRUMOUT_01 );
        break;
    case -3: // 탈퇴 등급 아님(길마 혹은 DB에 등록되지 않은 등급)
        SET_ERROR( ERR_GUILD_SELF_DRUMOUT_02 );
        break;
    default: // DB처리 에러
        SET_ERROR( ERR_GUILD_SELF_DRUMOUT_03 );
        break;
    }

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwUserUID )
        << BUILD_LOG( kPacket.m_dwGuildUID ) << END_LOG;


    SiKCenterSimLayer()->m_kActorManager.QueueingEventTo( dwUID_, KCenterEvent::ECN_SELF_DRUMOUT_GUILD_ACK, kPacket );
}

INT_IMPL_ON_FUNC( ECN_CHANGE_GUILD_MEMBER_LEVEL_REQ )
{
    SET_ERROR( ERR_UNKNOWN );

    // map< 변경할 맴버레벨, set<UserUID> >
    std::map< char, std::set<DWORD> >::iterator mit;
    std::set<DWORD>::iterator sit;

    KECN_CHANGE_GUILD_MEMBER_LEVEL_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_dwGuildMasterUID = kPacket_.m_dwGuildMasterUID;
    kPacket.m_mapChangeMemberLevel.clear();

    if ( IsExistGuild( kPacket.m_dwGuildUID ) == false ) {
        START_LOG( cerr, L"길드가 없음 GuildID : "<< kPacket_.m_dwGuildUID ) << END_LOG;
        SET_ERR_GOTO( ERR_GUILD_CHANGE_MEMBER_LEVEL_04, END_PROC );
    }

    // 강제탈퇴처리
    int nRet = -99;
    mit = kPacket_.m_mapChangeMemberLevel.find( KNGuildUserInfo::GL_DRUMOUT );
    if ( mit != kPacket_.m_mapChangeMemberLevel.end() ) {

        for ( sit = mit->second.begin() ; sit != mit->second.end() ; ++sit ) {

            nRet = UpdateDeleteGuildMember( kPacket_.m_dwGuildMasterUID, *sit, kPacket_.m_dwGuildUID );

            _LOG_SUCCESS( nRet == 0, L"강제탈퇴처리 결과 : " << L",Ret : " << nRet )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( *sit ) << END_LOG;

            if ( nRet != 0 ) continue;

            // 성공한 처리만 기록한다.
            kPacket.m_mapChangeMemberLevel[ KNGuildUserInfo::GL_DRUMOUT ].insert( *sit );
        }// end for
    }// end if

    // 일반길드원->제너럴
    nRet = -99;
    mit = kPacket_.m_mapChangeMemberLevel.find( KNGuildUserInfo::GL_GENERAL );
    if ( mit != kPacket_.m_mapChangeMemberLevel.end() ) {

        for ( sit = mit->second.begin() ; sit != mit->second.end() ; ++sit ) {

            nRet = UpdateChangeGuildMemberLevel( kPacket_.m_dwGuildMasterUID, *sit, kPacket_.m_dwGuildUID, KNGuildUserInfo::GL_GENERAL );

            _LOG_SUCCESS( nRet == 0, L"제너럴 위임 결과 : " << L",Ret : " << nRet )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( *sit ) << END_LOG;

            if ( nRet != 0 ) continue;

            // 성공한 처리만 기록한다.
            kPacket.m_mapChangeMemberLevel[ KNGuildUserInfo::GL_GENERAL ].insert( *sit ); 
        }// end for
    }

    // 제너럴->일반길드원
    nRet = -99;
    mit = kPacket_.m_mapChangeMemberLevel.find( KNGuildUserInfo::GL_NORMAL );
    if ( mit != kPacket_.m_mapChangeMemberLevel.end() ) {

        for ( sit = mit->second.begin() ; sit != mit->second.end() ; ++sit ) {

            nRet = UpdateChangeGuildMemberLevel( kPacket_.m_dwGuildMasterUID, *sit, kPacket_.m_dwGuildUID, KNGuildUserInfo::GL_NORMAL );

            _LOG_SUCCESS( nRet == 0, L"일반길드원으로 강등 결과 : " << L",Ret : " << nRet )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( *sit ) << END_LOG;

            if ( nRet != 0 ) continue;

            // 성공한 처리만 기록한다.
            kPacket.m_mapChangeMemberLevel[ KNGuildUserInfo::GL_NORMAL ].insert( *sit ); 
        }// end for
    }

    // 길드마스터 위임
    nRet = -99;
    mit = kPacket_.m_mapChangeMemberLevel.find( KNGuildUserInfo::GL_MASTER );
    if ( mit != kPacket_.m_mapChangeMemberLevel.end() ) {

        if ( mit->second.size() > 1 ) {
            START_LOG( cerr, L"길드마스터 위임은 한번에 한명씩만 가능. size : " << mit->second.size() ) << END_LOG;
        } else {

            for ( sit = mit->second.begin() ; sit != mit->second.end() ; ++sit ) {

                nRet = UpdateChangeGuildMaster( kPacket_.m_dwGuildMasterUID, *sit, kPacket_.m_dwGuildUID );

                _LOG_SUCCESS( nRet == 0, L"길드마스터 위임 결과 : " << L",Ret : " << nRet )
                    << BUILD_LOG( dwUID_ )
                    << BUILD_LOG( *sit ) << END_LOG;

                if ( nRet != 0 ) continue;

                // 성공한 처리만 기록한다.
                // 길드마스터는 1명뿐이다.
                // 이전 길드마스터는 자동적으로 제너럴이 된다.
                kPacket.m_mapChangeMemberLevel[ KNGuildUserInfo::GL_MASTER ].insert( *sit ); 
                kPacket.m_mapChangeMemberLevel[ KNGuildUserInfo::GL_GENERAL ].insert( kPacket_.m_dwGuildMasterUID );
            }// end for
        }// end if
    }// end if

    // 성공한 처리가 하나도 없으면 에러
    if ( kPacket.m_mapChangeMemberLevel.empty() ) {
            // DB처리중 에러
            SET_ERR_GOTO( ERR_GUILD_CHANGE_MEMBER_LEVEL_08, END_PROC );
    }// end if

    // 성공처리가 1개라도 있으면 성공.
    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwGuildUID )
        << BUILD_LOG( kPacket.m_dwGuildMasterUID )
        << BUILD_LOG( kPacket.m_mapChangeMemberLevel.size() ) << END_LOG;

    //QUEUING_ACK_TO_USER( ECN_CHANGE_GUILD_MEMBER_LEVEL_ACK );
    SiKCenterSimLayer()->m_kActorManager.QueueingEventTo( dwUID_, KCenterEvent::ECN_CHANGE_GUILD_MEMBER_LEVEL_ACK, kPacket );
}

INT_IMPL_ON_FUNC( ECN_ACCEPT_GUILD_JOINER_REQ )
{
    SET_ERROR( ERR_UNKNOWN );

    std::set<DWORD>::iterator sit;
    KECN_ACCEPT_GUILD_JOINER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
    kPacket.m_setUserUID.clear();

    KNGuildInfo kInfo;
    kInfo.Init();

    KCnGuildPtr spGuild = SiKCnGuildManager()->GetGuild( kPacket_.m_dwGuildUID );
    if( spGuild == NULL )
    {
        // 길드 정보가 없으면 DB에서 불러오자.
        JIF( UpdateOneGuildInfo( kPacket_.m_dwGuildUID ) );
        UpdateOneGuildMemberList( kPacket_.m_dwGuildUID );

        spGuild = SiKCnGuildManager()->GetGuild( kPacket_.m_dwGuildUID );
        if ( spGuild == NULL ) {
            START_LOG( cerr, L"길드정보 없음 GuildID : "<< kPacket_.m_dwGuildUID ) << END_LOG;
            SET_ERR_GOTO( ERR_GUILD_ACCEPT_JOINER_03, END_PROC );
        }
    }

    // 길드정보 받아오기
    kInfo = spGuild->GetGuildInfo();

    // 길드가입중지인가?
    if ( kInfo.m_ucJoinSetting == KNGuildInfo::JS_STOP_JOIN ) {
        SET_ERR_GOTO( ERR_GUILD_ACCEPT_JOINER_07, END_PROC );
    }

    // 길드맴버수가 한도치를 넘는가?
    if ( spGuild->GetMemberCount() + kPacket_.m_setUserUID.size() > (UINT)(spGuild->GetMaxMemberNum()) ) {

        START_LOG( cerr, L"길드가입승인시 최대 맴버수를 넘게된다." )
            << BUILD_LOG( kPacket_.m_setUserUID.size() )
            << BUILD_LOG( spGuild->GetJoinerCount() )
            << BUILD_LOG( spGuild->GetMaxJoinerNum() ) << END_LOG;

        SET_ERR_GOTO( ERR_GUILD_ACCEPT_JOINER_08, END_PROC );
    }

    // 길드가입승인처리
    int nRet = -99;
    for ( sit = kPacket_.m_setUserUID.begin() ; sit != kPacket_.m_setUserUID.end() ; ++sit ) {

        nRet = UpdateAcceptGuildJoiner( kPacket_.m_dwUserUID, *sit, kPacket_.m_dwGuildUID );

        _LOG_SUCCESS( nRet == 0, L"길드가입승인처리 결과 : " << L",Ret : " << nRet )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_dwGuildUID )
            << BUILD_LOG( *sit ) << END_LOG;

        if ( nRet != 0 ) continue;

        // 성공한 처리만 넣어준다.
        kPacket.m_setUserUID.insert( *sit );
    }

    // 모두 실패하면 에러처리.
    if ( kPacket.m_setUserUID.empty() ) {
        // 에러값 처리
        switch ( nRet )
        {
        case -1: // 유저정보 없음
            SET_ERR_GOTO( ERR_GUILD_ACCEPT_JOINER_02, END_PROC );
        case -2: // 길드정보 없음
            SET_ERR_GOTO( ERR_GUILD_ACCEPT_JOINER_03, END_PROC );
        case -3: // 길드마스터나 제너럴이 아님.
            SET_ERR_GOTO( ERR_GUILD_ACCEPT_JOINER_04, END_PROC );
        case -4: // 가입대기길드원이 아님.
            SET_ERR_GOTO( ERR_GUILD_ACCEPT_JOINER_05, END_PROC );
        default: // DB처리 에러
            SET_ERR_GOTO( ERR_GUILD_ACCEPT_JOINER_06, END_PROC );
        }
    }

    // 성공처리가 1개라도 있으면 성공.
    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_setUserUID.size() )
        << BUILD_LOG( kPacket.m_setUserUID.size() ) << END_LOG;

    //QUEUING_ACK_TO_USER( ECN_ACCEPT_GUILD_JOINER_ACK );
    SiKCenterSimLayer()->m_kActorManager.QueueingEventTo( dwUID_, KCenterEvent::ECN_ACCEPT_GUILD_JOINER_ACK, kPacket );
}

INT_IMPL_ON_FUNC( ECN_REJECT_GUILD_JOINER_REQ )
{
    SET_ERROR( ERR_UNKNOWN );

    std::set<DWORD>::iterator sit;
    KECN_REJECT_GUILD_JOINER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
    kPacket.m_setUserUID.clear();

    if ( IsExistGuild( kPacket.m_dwGuildUID ) == false ) {
        START_LOG( cerr, L"길드가 없음 GuildID : "<< kPacket.m_dwGuildUID ) << END_LOG;
        SET_ERR_GOTO( ERR_GUILD_REJECT_JOINER_03, END_PROC );
    }

    // 길드가입거절처리
    int nRet = -99;

    for ( sit = kPacket_.m_setUserUID.begin() ; sit != kPacket_.m_setUserUID.end() ; ++sit ) {

        nRet = UpdateDeleteGuildMember( kPacket_.m_dwUserUID, *sit, kPacket_.m_dwGuildUID );

        _LOG_SUCCESS( nRet == 0, L"가입거절처리 결과 : " << L",Ret : " << nRet )
            << BUILD_LOG( kPacket_.m_dwUserUID )
            << BUILD_LOG( kPacket_.m_dwGuildUID )
            << BUILD_LOG( *sit ) << END_LOG;

        if ( nRet != 0 ) continue;

        // 성공한 처리만 기록한다.
        kPacket.m_setUserUID.insert( *sit );
    }

    // 모두 실패하면 에러처리.
    if ( kPacket.m_setUserUID.empty() ) {
        // 에러값 처리
        switch ( nRet )
        {
        case -1: // 유저정보 없음
            SET_ERR_GOTO( ERR_GUILD_REJECT_JOINER_02, END_PROC );
        case -2: // 길드정보 없음
            SET_ERR_GOTO( ERR_GUILD_REJECT_JOINER_03, END_PROC );
        case -3: // 길드마스터나 제너럴이 아님.
            SET_ERR_GOTO( ERR_GUILD_REJECT_JOINER_04, END_PROC );
        case -4: // 가입대기길드원이 아님.
            SET_ERR_GOTO( ERR_GUILD_REJECT_JOINER_05, END_PROC );
        default: // DB처리 에러
            SET_ERR_GOTO( ERR_GUILD_REJECT_JOINER_06, END_PROC );
        }
    }

    // 성공처리가 1개라도 있으면 성공.
    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_setUserUID.size() )
        << BUILD_LOG( kPacket.m_setUserUID.size() ) << END_LOG;

    SiKCenterSimLayer()->m_kActorManager.QueueingEventTo( dwUID_, KCenterEvent::ECN_REJECT_GUILD_JOINER_ACK, kPacket );
}

INT_IMPL_ON_FUNC( ECN_BREAKUP_GUILD_REQ )
{
    SET_ERROR( ERR_UNKNOWN );

    KECN_BREAKUP_GUILD_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_dwUserUID = kPacket_.m_dwUserUID;

    if ( IsExistGuild( kPacket.m_dwGuildUID ) == false ) {
        START_LOG( cerr, L"길드가 없음 GuildID : "<< kPacket.m_dwGuildUID ) << END_LOG;
        SET_ERR_GOTO( ERR_GUILD_BREAKUP_03, END_PROC );
    }

    // 길드해산 처리
    int nRet = UpdateBreakupGuild( kPacket_.m_dwUserUID, kPacket_.m_dwGuildUID );

    // 에러값 처리
    switch ( nRet )
    {
    case 0: // 성공
        SET_ERROR( NET_OK );
        break;
    case -1: // 유저정보 없음
        SET_ERROR( ERR_GUILD_BREAKUP_02 );
        break;
    case -2: // 길드정보 없음
        SET_ERROR( ERR_GUILD_BREAKUP_03 );
        break;
    case -3: // 길드마스터 아님.
        SET_ERROR( ERR_GUILD_BREAKUP_04 );
        break;
    default: // DB처리 에러
        SET_ERROR( ERR_GUILD_BREAKUP_05 );
        break;
    }

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_dwGuildUID ) << END_LOG;

    SiKCenterSimLayer()->m_kActorManager.QueueingEventTo( dwUID_, KCenterEvent::ECN_BREAKUP_GUILD_ACK, kPacket );
}

_INT_IMPL_ON_FUNC( ECN_CHANGE_GUILD_JOIN_SETTING_REQ, KCnNGuildJoinPolicy )
{
    SET_ERROR( ERR_UNKNOWN );

    KCnNGuildJoinPolicy kPacket;
    kPacket = kPacket_;
    kPacket.m_kPolicy.m_nOK = -99;

    KCnGuildPtr spGuild = SiKCnGuildManager()->GetGuild( kPacket.m_kPolicy.m_dwUID );
    if( spGuild == NULL ) {
        START_LOG( cerr, L"길드정보 없음 GuildID : "<< kPacket.m_kPolicy.m_dwUID ) << END_LOG;
        SET_ERR_GOTO( ERR_GUILD_JOIN_POLICY_05, END_PROC );
    }

    if ( spGuild->GetMemberCount() >= (UINT)(spGuild->GetMaxMemberNum()) ) {

        START_LOG( cerr, L"길드가입 승인방법 변경시 길드원수가 최대치를 넘어 변경할 수 없음." )
            << BUILD_LOG( spGuild->GetJoinerCount() )
            << BUILD_LOG( spGuild->GetMaxJoinerNum() ) << END_LOG;

        SET_ERR_GOTO( ERR_GUILD_JOIN_POLICY_08, END_PROC );
    }

    int nRet = -99;
    nRet = UpdateGuildJoinSetting( kPacket_.m_dwUserUID, kPacket.m_kPolicy.m_dwUID, kPacket.m_kPolicy.m_ucMethod );

    switch( nRet ) {
        case 0: // 성공
            SET_ERROR( NET_OK );
            break;
        case -1: //-1 ; 유저정보 없음
            SET_ERROR( ERR_GUILD_JOIN_POLICY_04 );
            break;
        case -2: //-2 ; 길드정보 없음
            SET_ERROR( ERR_GUILD_JOIN_POLICY_05 );
            break;
        case -3: //-3 ; 길드마스터가 아님
            SET_ERROR( ERR_GUILD_JOIN_POLICY_03 );
            break;
        default: //-101이하 ; 트랜잭션 에러
            SET_ERROR( ERR_GUILD_JOIN_POLICY_07 );
            break;
    }

END_PROC:
    kPacket.m_kPolicy.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_dwUserUID )
        << BUILD_LOG( kPacket.m_kPolicy.m_nOK )
        << BUILD_LOG( kPacket.m_kPolicy.m_dwUID )
        << BUILD_LOGc( kPacket.m_kPolicy.m_ucMethod ) << END_LOG;

    //_QUEUING_ACK_TO_USER( EVENT_CHANGE_GUILD_JOIN_SETTING_ACK, kPacket_ );
    SiKCenterSimLayer()->m_kActorManager.QueueingEventTo( dwUID_, KCenterEvent::ECN_CHANGE_GUILD_JOIN_SETTING_ACK, kPacket );
}

_INT_IMPL_ON_FUNC( ECN_AUTO_CHANGE_GUILD_JOIN_SETTING_NOT, KCnNGuildJoinPolicy )
{
    // 길드가입설정이 자동으로 변경되었을 시 불린다.
    SET_ERROR( ERR_UNKNOWN );

    KCnNGuildJoinPolicy kPacket;
    kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
    kPacket.m_kPolicy.m_nOK = -99;
    kPacket.m_kPolicy.m_dwUID = kPacket_.m_kPolicy.m_dwUID;
    kPacket.m_kPolicy.m_ucMethod = kPacket_.m_kPolicy.m_ucMethod;

    int nRet = UpdateGuildJoinSetting( kPacket_.m_dwUserUID, kPacket_.m_kPolicy.m_dwUID, kPacket_.m_kPolicy.m_ucMethod );

    switch( nRet ) {
        case 0: // 성공
            SET_ERROR( NET_OK );
            break;
        case -1: //-1 ; 유저정보 없음
            SET_ERROR( ERR_GUILD_JOIN_POLICY_04 );
            break;
        case -2: //-2 ; 길드정보 없음
            SET_ERROR( ERR_GUILD_JOIN_POLICY_05 );
            break;
        case -3: //-3 ; 길드마스터가 아님
            SET_ERROR( ERR_GUILD_JOIN_POLICY_03 );
            break;
        default: //-101이하 ; 트랜잭션 에러
            SET_ERROR( ERR_GUILD_JOIN_POLICY_07 );
            break;
    }

    _LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK, L"길드가입설정 자동변경. nRet : " << nRet )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_kPolicy.m_nOK )
        << BUILD_LOG( kPacket_.m_kPolicy.m_dwUID )
        << BUILD_LOGc( kPacket_.m_kPolicy.m_ucMethod ) << END_LOG;

    kPacket.m_kPolicy.m_nOK = NetError::GetLastNetError();

    if ( NetError::GetLastNetError() == NetError::NET_OK ) {
        // 해당길드에 큐잉
        SiKCnGuildManager()->QueueingEvent( KCenterEvent::ECN_CHANGE_GUILD_JOIN_SETTING_ACK, kPacket.m_kPolicy.m_dwUID, kPacket );
    }
}

_INT_IMPL_ON_FUNC( ECN_UPGRADE_GUILD_GRADE_REQ, DWORD )
{
    // 신규길드 정식길드로 변환
    SET_ERROR( ERR_UNKNOWN );

    DWORD dwGuildUID = kPacket_;
    int kPacket = -99;

    int nRet = UpdateGuildGrade( kPacket_ );

    switch ( nRet ) {
        case 0: // 성공
            SET_ERROR( NET_OK );
            break;
        case -1: // 길드정보 없음
            SET_ERROR( ERR_GUILD_UPGRADE_GRADE_00 );
            break;
        case -2: // 신생길드 아님.
            SET_ERROR( ERR_GUILD_UPGRADE_GRADE_01 );
            break;
        default: // DB처리 에러
            SET_ERROR( ERR_GUILD_UPGRADE_GRADE_02 );
            break;
    }

    kPacket = NetError::GetLastNetError();

    _LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK, L"신규길드 정식길드로 변환. nRet : " << nRet )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket )
        << BUILD_LOG( dwGuildUID ) << END_LOG;

    if ( NetError::GetLastNetError() == NetError::NET_OK ) {
        // 해당길드에 큐잉
        SiKCnGuildManager()->QueueingEvent( KCenterEvent::ECN_UPGRADE_GUILD_GRADE_ACK, dwGuildUID, kPacket );
    }
}

_INT_IMPL_ON_FUNC( ECN_REMOVE_GUILD_NOT, DWORD )
{
    // 신규길드 생성후 30일 이내에 길드원수 5명 미만이면 길드삭제.
    // 조건체크는 서버에서 다 해야함.
    SET_ERROR( ERR_UNKNOWN );

    DWORD dwGuildUID = kPacket_;
    int kPacket = -99;

    // 길드강제해산 처리
    int nRet = UpdateGuildDelete( dwGuildUID );

    switch ( nRet ) {
        case 0: // 성공
            SET_ERROR( NET_OK );
            break;
        case -1: // 길드정보 없음
            SET_ERROR( ERR_GUILD_AUTO_REMOVE_00 );
            break;
        default: // DB처리 에러
            SET_ERROR( ERR_GUILD_AUTO_REMOVE_01 );
            break;
    }

    kPacket = NetError::GetLastNetError();

    _LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK, L"길드강제해산. nRet : " << nRet )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket )
        << BUILD_LOG( dwGuildUID ) << END_LOG;

    if ( NetError::GetLastNetError() == NetError::NET_OK ) {
        // 길드매니저에 정보 전달.
        SiKCnGuildManager()->RemoveGuild( dwGuildUID );
    }
}

INT_IMPL_ON_FUNC( ECN_UPDATE_GUILD_MEMBER_LIST_REQ )
{
    if ( GetGuildMemberListFromDB( kPacket_.m_dwGuildUID, kPacket_.m_mapMemberList ) == false ) {
        START_LOG( cerr, L"DB에서 길드 맴버리스트 얻어오기. DB처리중 오류" ) << END_LOG;
        return;
    }

    START_LOG( clog, L"DB에서 받아온 맴버리스트를 길드정보에 저장." )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_mapMemberList.size() ) << END_LOG;

    SiKCnGuildManager()->SetGuildMemberInfo( kPacket_.m_dwGuildUID, kPacket_.m_mapMemberList );
}

INT_IMPL_ON_FUNC( ECN_AUTO_REJECT_GUILD_JOINER_REQ )
{
    SET_ERROR( ERR_UNKNOWN );

    std::set<DWORD>::iterator sit;
    KECN_REJECT_GUILD_JOINER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
    kPacket.m_setUserUID.clear();

    // 길드가입거절처리
    int nRet = -99;

    for ( sit = kPacket_.m_setUserUID.begin() ; sit != kPacket_.m_setUserUID.end() ; ++sit ) {

        nRet = UpdateDeleteGuildMember( kPacket_.m_dwUserUID, *sit, kPacket_.m_dwGuildUID );

        _LOG_SUCCESS( nRet == 0, L"가입거절처리 결과 : " << L",Ret : " << nRet )
            << BUILD_LOG( kPacket_.m_dwUserUID )
            << BUILD_LOG( kPacket_.m_dwGuildUID )
            << BUILD_LOG( *sit ) << END_LOG;

        if ( nRet != 0 ) continue;

        // 성공한 처리만 기록한다.
        kPacket.m_setUserUID.insert( *sit );
    }

    // 모두 실패하면 에러처리.
    if ( kPacket.m_setUserUID.empty() ) {
        // 에러값 처리
        switch ( nRet )
        {
        case -1: // 유저정보 없음
            SET_ERR_GOTO( ERR_GUILD_REJECT_JOINER_02, END_PROC );
        case -2: // 길드정보 없음
            SET_ERR_GOTO( ERR_GUILD_REJECT_JOINER_03, END_PROC );
        case -3: // 길드마스터나 제너럴이 아님.
            SET_ERR_GOTO( ERR_GUILD_REJECT_JOINER_04, END_PROC );
        case -4: // 가입대기길드원이 아님.
            SET_ERR_GOTO( ERR_GUILD_REJECT_JOINER_05, END_PROC );
        default: // DB처리 에러
            SET_ERR_GOTO( ERR_GUILD_REJECT_JOINER_06, END_PROC );
        }
    }

    // 성공처리가 1개라도 있으면 성공.
    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_setUserUID.size() )
        << BUILD_LOG( kPacket.m_setUserUID.size() ) << END_LOG;

    if ( NetError::GetLastNetError() == NetError::NET_OK ) {
        // 해당길드에 큐잉
        SiKCnGuildManager()->QueueingEvent( KCenterEvent::ECN_REJECT_GUILD_JOINER_ACK, kPacket_.m_dwGuildUID, kPacket );
    }
}

INT_IMPL_ON_FUNC( ECN_UPGRADE_GUILD_GRADE_GIFT_REQ )
{
    // 길드승격에 따라 길마에게 아이템 넣어주기(캐릭터 분리 후 지급여부를 세팅한다)
    KECN_UPGRADE_GUILD_GRADE_GIFT_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_dwGuildMasterUID = kPacket_.m_dwGuildMasterUID;
    kPacket.m_WUID = 0;

    // 캐릭터 분리 작업
    //time_t tmGuildUID = static_cast< time_t >( kPacket_.m_dwGuildUID );
    time_t tmGuildUID = 0;
    // 44: UET_GET_CHANGE_GUILDMARK_ITEM
    m_kHelper.SetTodayEventDone( kPacket_.m_dwGuildMasterUID, 44, tmGuildUID, 1 );

    // 성공
    SET_ERROR( NET_OK );

//END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK, L"길드승격아이템 넣어주기. WUID : " << kPacket.m_WUID )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwGuildMasterUID )
        << BUILD_LOG( kPacket.m_WUID ) << END_LOG;

    if ( NetError::GetLastNetError() == NetError::NET_OK ) {
        // 해당 길드에 전달
        SiKCnGuildManager()->QueueingEvent( KCenterEvent::ECN_UPGRADE_GUILD_GRADE_GIFT_ACK, kPacket_.m_dwGuildUID, kPacket );
    }
}
_INT_IMPL_ON_FUNC( ECN_NEWBIE_GUILD_DESTROY_NOT, DWORD )
{
    // 신생길드 조건에 의한 삭제처리
    int nRet = UpdateGuildDelete( kPacket_ );

    switch ( nRet )
    {
    case 0: // 성공
    case -1: // 길드정보 없음(이미 DB에서 삭제되어 있으니까, 성공처리하고 삭제 진행)
        SET_ERROR( NET_OK );
        break;
    default: // DB처리 실패
        SET_ERROR( ERR_DB_COMMON_00 );
        break;
    }

    _LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK, L"길드 강제 삭제처리. Result : " << nRet )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    // 성공시, 삭제처리 진행
    if ( NetError::GetLastNetError() == NetError::NET_OK ) {
        SiKCnGuildManager()->RemoveGuild( kPacket_ );
    }
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_GET_TOTAL_NEW_DONATION_REQ )
{
    DWORD dwMinType = 0;
    DWORD dwMaxType = 0;
    time_t tmDate;
    std::map<DWORD, int> mapTotalDonation;
    mapTotalDonation.clear();

    SiKCnNewDonationManager()->GetMinMaxType( dwMinType, dwMaxType );
    SiKCnNewDonationManager()->GetBeginTime( tmDate );
    // DB에 저장된 전체 NPC 기부 정보 가져오기.
    GetTotalDonationInfo( tmDate, dwMinType, dwMaxType, mapTotalDonation );

    // 가져온 정보를 저장하자.
    SiKCnNewDonationManager()->SetDonationFromDB( mapTotalDonation );
    START_LOG( cerr, L"DB로 부터 신년 이벤트 기부액 가져옴. Point : " << mapTotalDonation.size() ) << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_GUIDE_BOOK_LIST_REQ )
{
    std::map<short,short> kPacket;
    GetGuideBookList( dwUID_, kPacket );

    QUEUING_ACK_TO_NUSER_UID( ENU_GUIDE_BOOK_LIST_ACK );
}

_INT_IMPL_ON_FUNC( ECN_UPDATE_GUILD_MEMBER_NICK_NOT, KGuildUserName )
{
    if ( IsExistGuild( kPacket_.m_dwGuildUID ) == false ) {
        START_LOG( cerr, L"길드가 없음 GuildID : "<< kPacket_.m_dwGuildUID ) << END_LOG;
        return;
    }

    SiKCnGuildManager()->QueueingEvent( KCenterEvent::ECN_UPDATE_GUILD_MEMBER_NICK_NOT, kPacket_.m_dwGuildUID, kPacket_ );
}

_INT_IMPL_ON_FUNC( ECN_GUILD_MEMBER_LEVELUP_NOT, KGuildUserCharacter )
{
    if ( IsExistGuild( kPacket_.m_dwGuildUID ) == false ) {
        START_LOG( cerr, L"길드가 없음 GuildID : "<< kPacket_.m_dwGuildUID ) << END_LOG;
        return;
    }

    SiKCnGuildManager()->QueueingEvent( KCenterEvent::ECN_GUILD_MEMBER_LEVELUP_NOT, kPacket_.m_dwGuildUID, kPacket_ );
}

INT_IMPL_ON_FUNC( ECN_CHAT_REQ )
{
    if ( IsExistGuild( kPacket_.m_kChatData.m_iParam ) == false ) {
        START_LOG( cerr, L"길드가 없음 GuildID : "<< kPacket_.m_kChatData.m_iParam ) << END_LOG;
        return;
    }

    SiKCnGuildManager()->GuildChatProc( kPacket_.m_kChatData );
}

INT_IMPL_ON_FUNC( ECN_ADD_GUILD_USER_REQ )
{
    if ( IsExistGuild( kPacket_.m_dwGuildUID ) == false ) {
        START_LOG( cerr, L"길드가 없음 GuildID : "<< kPacket_.m_dwGuildUID ) << END_LOG;
        return;
    }

    SiKCenterSimLayer()->m_kActorManager.QueueingEventTo( dwUID_, KCenterEvent::ECN_ADD_GUILD_USER_ACK, kPacket_ );
}

//INT_IMPL_ON_FUNC_NOPARAM( ECN_ONEADAY_SELECT_REQ )
//{
//    std::vector<KOneadayItem> vecOneadayItemList;
//    vecOneadayItemList.clear();
//
//    if ( !CheckDBUpdate( ONEADAY ) && dwUID_ == 0 ) {
//        return;
//    }
//    
//    OneadaySelect( vecOneadayItemList );
//
//    std::vector<KOneadayItem>::iterator vit;
//    for ( vit = vecOneadayItemList.begin(); vit != vecOneadayItemList.end(); ++vit ) {
//        GetPackageItem( vit->m_ItemID, vit->m_vecItem );
//    }
//
//    SiKCnOneaday()->SetItemList( vecOneadayItemList );
//
//    START_LOG( clog, L"원어데이 아이템 리스트 사이즈" )
//        << BUILD_LOG( vecOneadayItemList.size() ) << END_LOG;
//};

//_INT_IMPL_ON_FUNC( DB_ECN_ONEADAY_UPDATE_REQ, std::vector<KOneadayItem> )
//{
//    std::vector<KOneadayItem>::const_iterator vit;
//
//    for ( vit = kPacket_.begin(); vit != kPacket_.end(); ++vit ) {
//        if ( false == OneadayUpdate( vit->m_tStartDate, vit->m_tEndDate, vit->m_OneadayNo, vit->m_SellCurrentCount ) ) {
//            START_LOG( cerr, L"원어데이 아이템 정보 DB 업데이트 실패")
//                << BUILD_LOG( vit->m_ItemID ) << END_LOG;
//        }
//    }
//}

_INT_IMPL_ON_FUNC( ECN_SQUARE_LIST_REQ, PAIR_DWORD )
{
    const DWORD& dwGuildUID = kPacket_.second;

    if ( IsExistGuild( dwGuildUID ) == false ) {
        START_LOG( cwarn, L"길드가 없음 GuildID : "<< dwGuildUID ) << END_LOG;
    }

    SiKCenterSimLayer()->m_kActorManager.QueueingEventTo( dwUID_, KCenterEvent::ECN_SQUARE_LIST_ACK, kPacket_ );
}

_INT_IMPL_ON_FUNC( ECN_UPDATE_UNIQUE_USER_STAT_REQ, MAP_DWORD_SET )
{
    // 채널링 타입별 Unique User Stat.
    // 각 채널타입별 
    // PUBLISHER1 : 11
    // PUBLISHER2 : 12
    // PUBLISHER3 : 13
    // PUBLISHER4 : 14
    for ( int i = KNUser::USER_CT_PUBLISHER_1 ; i < KNUser::USER_CT_MAX ; ++i ) {
        std::map< DWORD, std::set<DWORD> >::iterator mit;
        mit = kPacket_.find( i );
        if( mit != kPacket_.end() ) {
            DWORD dwUniqueCount = mit->second.size();
            AddServerCountData( dwUniqueCount, 10 + i );
        }
    }
}

_INT_IMPL_ON_FUNC( ECN_UPDATE_GUILD_POINT_REQ, KGuildPointBroad )
{
    if ( IsExistGuild( kPacket_.m_GuildUID ) == false ) {
        START_LOG( cerr, L"길드가 없음 GuildID : "<< kPacket_.m_GuildUID ) << END_LOG;
        return;
    }

    SiKCnGuildManager()->QueueingEvent( KCenterEvent::ECN_UPDATE_GUILD_POINT_REQ, kPacket_.m_GuildUID, kPacket_ );
    SiKCnGuildManager()->UpdateChangeGuildInfo( kPacket_.m_GuildUID, kPacket_ );
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_DB_GUILD_POINT_UPDATE_REQ )
{
    KNGuildInfo kPoint;
    kPoint.Init();
    GetGuildPointFromDB( dwUID_, kPoint );

    KGuildPointBroad kPointBroad;
    kPointBroad.m_dwGuildPoint = kPoint.m_dwPoint;
    kPointBroad.m_dwBattlePoint = kPoint.m_GuildBattlePoint;
    
    SiKCnGuildManager()->QueueingEvent( KCenterEvent::ECN_DB_GUILD_POINT_UPDATE_ACK, dwUID_, kPointBroad );
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_DB_SPHINX_DATA_NOT )
{
    int nVersion = SiKCnSphinx()->GetSphinxVersion();
    UpdateSphinxData( (dwUID_ == 0 ? false:true ), nVersion );
}

_INT_IMPL_ON_FUNC( ECN_UPDATE_LOGIN_CCU, DWORD )
{
    bool ret = m_kHelper.UpdateChannelCCU( 8, 0, 255, kPacket_ );

    LOG_SUCCESS( ret == true )
        << BUILD_LOG( ret )
        << BUILD_LOG( kPacket_ ) << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_DB_SURVEY_DATA_NOT )
{
    int nVersion = SiKCnSurvey()->GetSurveyVersion();
    UpdateSurveyData( (dwUID_ == 0 ? false:true ), nVersion );
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_DB_GWC_EVENT_DATA_REQ )
{
    UpdateGWCEventData();
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_DB_EVENT_TIME_SYNCRONIZE_NOT )
{
    SYSTEMTIME sysTime;
    DBJIF( m_kODBC.Query( L"SELECT GETDATE()" ) );
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> sysTime );
        m_kODBC.EndFetch();
    }

    LIF( ::SetLocalTime( &sysTime ) );

    START_LOG( cerr, L"현재시각은... --> "
        << sysTime.wYear << L"-"
        << sysTime.wMonth << L"-"
        << sysTime.wDay << L" "
        << sysTime.wHour << L":"
        << sysTime.wMinute << L":"
        << sysTime.wSecond << L":"
        << sysTime.wMilliseconds << L" ( "
        << sysTime.wDayOfWeek<< L" )" ) << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_DB_ADVENTURE_DATA_NOT )
{
    if ( CheckDBUpdate( ADVENTURE ) ) {
        UpdateAdventureEventData();
    }
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_OLYMPIC_RANK_REQ )
{
    std::vector<KRegDateRankInfo> vecRegDateRank;
    std::vector<KTotalRankInfo> vecTotalRank;
    vecRegDateRank.clear();
    vecTotalRank.clear();
    bool bInit = false;

    if( dwUID_ == 1 ) { // 처음 구동후에 호출시에는 true로 변경.
        bInit = true;
    }

    // 올림픽 이벤트 랭킹 데이터 가져오는 부분.
    GetOlympicRegDateRankData( vecRegDateRank );
    GetOlympicTotalRankData( vecTotalRank );

    SiKCnOlympicManager()->UpdateRankDB( vecRegDateRank, vecTotalRank, bInit );
    START_LOG( cerr, L"DB로 부터 올림픽 이벤트 Ranking정보 가져옴." )
        << BUILD_LOG( vecRegDateRank.size() ) 
        << BUILD_LOG( vecTotalRank.size() ) 
        << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_DB_OLYMPIC_DATA_NOT )
{
    std::vector<KRegDateRankInfo> vecRegDateRank;
    std::vector<KTotalRankInfo> vecTotalRank;
    vecRegDateRank.clear();
    vecTotalRank.clear();
    bool bInit = false;

    if( CheckDBUpdate( OLYMPIC ) ) {
        // 올림픽 이벤트 랭킹 데이터 가져오는 부분.
        GetOlympicRegDateRankData( vecRegDateRank );
        GetOlympicTotalRankData( vecTotalRank );
        SiKCnOlympicManager()->UpdateRankDB( vecRegDateRank, vecTotalRank, bInit );
        START_LOG( cerr, L"DB로 부터 올림픽 이벤트 Ranking정보 가져옴." )
            << BUILD_LOG( vecRegDateRank.size() ) 
            << BUILD_LOG( vecTotalRank.size() ) 
            << END_LOG;
    }
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_DB_OLYMPIC_USER_DATA_UPDATE_NOT )
{
    std::map<time_t, KOlympicInfoData> mapOlympicInfoData;
    std::map<time_t, KOlympicInfoData>::reverse_iterator rmit;
    int nCount = 0;

    SiKCnOlympicManager()->GetOlympicUserData( mapOlympicInfoData );

    for( rmit = mapOlympicInfoData.rbegin(); rmit != mapOlympicInfoData.rend(); ++rmit ) {
        if( nCount > 1 ) {
            break;
        }
        UpdateOlympicTotalData( rmit->second.m_tmRegDate, rmit->second.m_biTotalData );
        UpdateOlympicUserRankData( rmit->second.m_tmRegDate, rmit->second.m_nGameType, rmit->second.m_nTeamType, rmit->second.m_biTotalData, rmit->second.m_strNickName );
        ++nCount;
    }
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_OLYMPIC_DB_USER_DATA_REQ )
{
    std::map<time_t, KOlympicInfoData> mapOlympicInfoData;
    // 올림픽 이벤트 누적 유저 데이터 가져오는 부분.
    GetOlympicUserData( mapOlympicInfoData );

    SiKCnOlympicManager()->UpdateUserData( mapOlympicInfoData );
    START_LOG( cerr, L"DB로 부터 올림픽 이벤트 누적 데이터 정보 가져옴." )
        << BUILD_LOG( mapOlympicInfoData.size() )
        << END_LOG;
}

_INT_IMPL_ON_FUNC( ECN_BUFF_DONATION_TOTAL_DATA_REQ, KECN_BUFF_DONATION_TOTAL_DATA_NOT )
{
    time_t tmDate;
    __int64 biComputedPoint;
    __int64 biDecreasePoint;
    DWORD dwType=1;

    SiKCnBuffDonation()->GetType( dwType );
    SiKCnBuffDonation()->GetBeginTime( tmDate );
    // 발렌타인 버프 기부 이벤트 전체 기부 수치 가져 오기
    __int64 biDonationPointFromDB = GetTotalServerCountData( tmDate, dwType );

    // 버프 발동 되면 전체 수치 감소
    if ( FLAG_BUFF_START_NOT == kPacket_.m_nBuffStateFlag ) {
        // db에 수치 감소
        biDecreasePoint = SiKCnBuffDonation()->GetDecreasePoint();
        AddServerCountData( biDecreasePoint, dwType, tmDate );
        // 캐싱 데이터 수치 감소
        SiKCnBuffDonation()->UpdateDonationPointByBuffActivation( biDonationPointFromDB, biComputedPoint );
        // 씽크 포인트 초기화
        SiKCnBuffDonation()->ResetSecondDataAtSyncPointVector();

        kPacket_.m_biTotalDonationPoint = biComputedPoint;

        START_LOG( clog, L"버프가 발동 되어 기부 수치 감소함." )
            << BUILD_LOG( biDecreasePoint )
            << BUILD_LOG( biComputedPoint )
            << BUILD_LOG( biDonationPointFromDB )
            << END_LOG;
    }
    else {
        // 버프 발동이 아니라면 DB에서 얻은 데이터로 갱신
        kPacket_.m_biTotalDonationPoint = biDonationPointFromDB;
        SiKCnBuffDonation()->UpdateDonationPointFromDB( kPacket_.m_biTotalDonationPoint );
    }

    // flag는 CnValentineDonation에서 세팅 되어서 왔기 때문에 별도 세팅없이 전송
    SiKCnBuffDonation()->SendingDataToAllGameServer( kPacket_ );

    START_LOG( clog, L"DB로 부터 발렌타인 버프 기부 누적 데이터 가져 옴." )
        << BUILD_LOGtm( CTime( tmDate ) )
        << BUILD_LOG( dwType )
        << BUILD_LOG( biDonationPointFromDB )
        << BUILD_LOG( kPacket_.m_nBuffStateFlag )
        << END_LOG;
}

_INT_IMPL_ON_FUNC( ECN_BUFF_DONATION_DATA_UPDATE_REQ, int )
{
    time_t tmDate;
    DWORD dwType=1;

    SiKCnBuffDonation()->GetBeginTime( tmDate );
    SiKCnBuffDonation()->GetType( dwType );
    // 유저의 기부 정보를 넣어주자.
    if( !AddServerCountData( kPacket_, dwType, tmDate ) ) {
        START_LOG( cerr, L"버프 기부 DB에 유저 기부 수치 기록 실패." )
            << BUILD_LOGtm( CTime( tmDate ) )
            << BUILD_LOG( dwType )
            << BUILD_LOG( kPacket_ )
            << END_LOG;
    }

    SiKCnBuffDonation()->UpdateDonationPointDiff( kPacket_ );

    START_LOG( clog, L"버프 기부 DB에 유저 기부 수치 기록." )
        << BUILD_LOGtm( CTime( tmDate ) )
        << BUILD_LOG( dwType )
        << BUILD_LOG( kPacket_ )
        << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_ACCUMULATE_ATTENDANCE_DATA_UPDATE_NOT )
{
    // DB에서 변경 된 정보 있으면 퍼온다
    if( CheckDBUpdate( ACC_ATTENDANCE ) ) {
        UpdateDBSettingInfo_AccAttendance();
    }
}

_INT_IMPL_ON_FUNC( ECN_SOCIAL_COMMERCE_DATA_UPDATE_NOT, bool )
{
    // DB데이터 변경 확인 하는 패킷인지 확인 할 변수를 받는다
    bool bCheckChange = kPacket_;
    if ( true == bCheckChange ) { // CheckChangeDBSettingDataTick()에서만 true로 보냄
        // DB에서 변경 된 정보 있으면 퍼온다
        if( CheckDBUpdate( SOCIAL_COMMERCE ) ) {
            UpdateSocialCommerceData( true );
        }
    }
    else {
        // 공동구매 상태 갱신을 위한 전체 리스트 업데이트
        UpdateSocialCommerceData( false );
    }  
}

INT_IMPL_ON_FUNC( DB_ECN_SOCIAL_COMMERCE_GROUPPURCHASE_USER_REQ )
{
    int nCount = SiKCnSocialCommerce()->GetRewardUserCount();
    VEC_REWARD vecRewardItem;
    VEC_GROUP_PURCHASE_REWARD vecRewardInfo;
    VEC_FOR_REWARD_PROVIDE vecProvideInfo;

    // 보상 아이템 정보 설정
    if ( false == SiKCnSocialCommerce()->GetRewardListByEventUID( kPacket_.m_dwEventUID, vecRewardItem ) ) {
        START_LOG( cerr, L"[소셜커머스] 판매 정보에서 보상 설정이 되어 있지 않다 DB설정 확인 필요 EventUID : " << kPacket_.m_dwEventUID ) << END_LOG;
    }

    // 달성 한 공동구매 구매 아이템 구매 유저 정보 얻기
    GetRewardUserInfo_SocialCommerce( kPacket_.m_dwEventUID, nCount, vecRewardItem, vecRewardInfo );
    if ( true == vecRewardInfo.empty() ) {
        // EventUID에 해당 하는 보상 지급 유저가 더 이상 없다면 지급 완료로 Flag 바꿈
        int nFlag = KSCSellInfo::FLAG_SELL_GROUPPURCHASE_REWARD_END;
        if ( false == m_kHelper.UpdateSellStateFlag_SocialCommerce( kPacket_.m_dwEventUID, nFlag ) ) {
            START_LOG( cerr, L"[소셜커머스]아이템 지급 유저 목록 요청 했으나 해당 이벤트가 없음 EventUID : " << kPacket_.m_dwEventUID ) << END_LOG;
        }
    }
    else {
        // ItemProvider 객체에 전달 할 데이터로 재가공
        VEC_GROUP_PURCHASE_REWARD::iterator vit;
        vecProvideInfo.clear();
        for ( vit = vecRewardInfo.begin(); vit != vecRewardInfo.end(); vit++ ) {
            // provide 데이터에 획득 경로 설정
            vit->kProvideData.m_nGetType = kPacket_.m_nGetType;
            vecProvideInfo.push_back( vit->kProvideData );

            // 아이템 지급 시 EventUID도 필요하다고 해서 추가
            vit->m_dwEventUID = kPacket_.m_dwEventUID;
        }

        // ItemProvider 객체에 지급 요청
        int nFrom = FROM_SC_GROUP_PURCHASE;
        if ( false == SiKCnItemProvider()->ItemProvideReq( vecProvideInfo, nFrom ) ) {
            // 다른데서 ItemProvideReq를 이미 했을 가능성이 있다
            START_LOG( cwarn, L"[소셜커머스] 아이템 지급 객체가 잠겨있음(일하는 중)") << END_LOG;
        } else {
            // 소셜커머스 객체 캐싱 데이터 갱신
            SiKCnSocialCommerce()->SetGroupPurchaseRewardInfo( vecRewardInfo );
        }
    }
}

INT_IMPL_ON_FUNC( DB_ECN_ITEM_PROVIDE_REQ )
{
    VEC_FOR_REWARD_PROVIDE vecRemove;
    VEC_FOR_REWARD_PROVIDE::iterator vitProvide;
    VEC_REWARD::iterator vitReward;
    int nFromWhat = 0;
    int nCount = 0;
    const int nMaxCount = kPacket_.m_nOneTimeProvisionUserCount;

    vecRemove.clear();

    SiKCnItemProvider()->GetFromWhat( nFromWhat );

    // 공동구매 보상 아이템 지급 처리를 위한 변수들
    VEC_GROUP_PURCHASE_REWARD vecGPReward;
    VEC_GROUP_PURCHASE_REWARD::iterator vitGroupPurchaseReward;

    vecGPReward.clear();
    SiKCnSocialCommerce()->GetGroupPurchaseRewardInfo( vecGPReward ); // 공동구매 유저 리스트를 얻는다
    vitGroupPurchaseReward = vecGPReward.begin();


    for( vitProvide = kPacket_.m_vecProvideInfo.begin(); vitProvide != kPacket_.m_vecProvideInfo.end(); vitProvide++ ) {
        bool bResult = false;

        if ( nMaxCount <= nCount ) {
            // 한번 요청에 처리할 수 있는 유저 숫자 최대치를 넘어서 루프 빠져나감
            break;
        }

        for ( vitReward = vitProvide->m_vecRewardItem.begin(); vitReward != vitProvide->m_vecRewardItem.end(); vitReward++ ) {
            // 아이템 삽입 sp 호출
            __int64 biWaitUID;
            bResult = InsertToWaitItemJob( vitProvide->m_dwUserUID, vitReward->m_ItemID, vitReward->m_nDuration, vitReward->m_nPeriod, vitProvide->m_nGetType, biWaitUID );
        }
        if ( false == bResult ) {
            // 아이템 지급 실패 했기 때문에 나중에 다시 지급
            continue;
        }

        // 아이템 삽입 완료 후 ItemProvider에 전달 할 삭제 컨테이너에 삽입
        vecRemove.push_back( *vitProvide );

        // 소셜커머스 아이템 지급 기록이 필요해서 여기에서 처리
        if ( FROM_SC_GROUP_PURCHASE == nFromWhat ) {
            // 소셜커머스쪽에서 아이템 지급 요청할 때
            // 벡터 순서 동일하게 세팅해서 보내기 때문에 순서 맞다고 가정 하고 처리
            if ( vecGPReward.end() != vitGroupPurchaseReward ) {
                UpdateGroupPurchaseRewardComplete_SocialCommerce( vitGroupPurchaseReward->m_dwEventUID, vitGroupPurchaseReward->m_dwRewardUID, vitProvide->m_dwUserUID );
                vitGroupPurchaseReward++;
            }
        }

        nCount++;
    }

    // ItemProvider 객체에 지급 완료 후 RemoveList 전달
    SiKCnItemProvider()->ProvideCompleteProcedure( vecRemove );
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_DB_DUNGEON_RANK_SEASON_DELIMIT_TIME_REQ )
{
    time_t tmSeasonDelimit = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    Get_DungeonRank_SeasonDelimitTime_FromDB( tmSeasonDelimit );

    SiKCnDungeonRankManager()->SetSeasonDelimitTime( tmSeasonDelimit );
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_DB_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_REQ )
{
    std::set< int > setRankingModes;
    SiKCnDungeonRankManager()->GetRankingModes( setRankingModes );

    std::map< int, std::list< KDungeonRankInfo > > mapCurrentDungeonRankList;
    Get_DungeonRank_CurrentSeasonTopRank_FromDB( setRankingModes, mapCurrentDungeonRankList );

    SiKCnDungeonRankManager()->SetCurrentSeasonTopRank( mapCurrentDungeonRankList );
}

INT_IMPL_ON_FUNC_NOPARAM( ECN_DB_DUNGEON_RANK_PREVIOUS_SEASON_TOP_RANK_REQ )
{
    Get_DungeonRank_PreviousSeason();
}

INT_IMPL_ON_FUNC( ECN_DUNGEON_RANK_CURRENT_SEASON_ADD_RANK_REQ )
{
    std::vector< KDungeonRankInfo > vecNewEntryAfterDBWrite;
    Update_DungeonRank_CurrentSeasonNewEntry_ToDB( kPacket_, vecNewEntryAfterDBWrite );

    SiKCnDungeonRankManager()->AddCurrentSeasonNewEntryAfterDBWrite( vecNewEntryAfterDBWrite );
}

_INT_IMPL_ON_FUNC( ECN_DB_DUNGEON_RANK_SEASON_RESET_CHECK_REQ, bool )
{
    // kPacket_: true: 강제 리셋

    if ( true == kPacket_ || true == CheckDBUpdate( DUNGEON_RANK ) ) {
        // 랭킹에 등록 예약된 기록들을 DB에 저장한 뒤 시즌 변경을 진행한다
        KECN_DUNGEON_RANK_CURRENT_SEASON_ADD_RANK_REQ kPacket;
        std::vector< KDungeonRankInfo > vecNewEntryAfterDBWrite;
        SiKCnDungeonRankManager()->GetCurrentSeasonNewEntryToDBWithLock( kPacket );

        Update_DungeonRank_CurrentSeasonNewEntry_ToDB( kPacket, vecNewEntryAfterDBWrite );

        CTime ctCurrent = SiKCnDungeonRankManager()->SetSeasonReset();

        // 이전 시즌 랭킹 빌드 요청
        if ( false == Do_DungeonRank_BuildPreviousSeasonRank_ToDB( ctCurrent ) ) {
            START_LOG( cerr, L"지난 시즌 랭킹 빌드 실패" ) << END_LOG;

            return;
        }

        // 이전 시즌 랭크 리스트 요청(ECN_DB_DUNGEON_RANK_PREVIOUS_SEASON_TOP_RANK_REQ 로직과 동일)
        Get_DungeonRank_PreviousSeason();
    }
}

_INT_IMPL_ON_FUNC( DB_ENU_ACCOUNT_REGISTER_REQ, KENU_ACCOUNT_REGISTER_REQ )
{
    KENU_ACCOUNT_REGISTER_ACK kPacket;
    kPacket.m_nOK  = 0;

    std::wstring strPasswd = KncUtil::toWideString( kPacket_.m_strPasswd );
    std::wstring strLogin = KncUtil::toWideString( kPacket_.m_strLogin );
    std::wstring strEmail = KncUtil::toWideString( kPacket_.m_strEmail );
    std::wstring strIP = kPacket_.m_strIP;

    DBJIF( m_kODBC.Query( L"{ call dbo.LUMA_Client_Register ( N'%s', N'%s', N'%s', N'%s' ) }", strLogin.c_str(), strPasswd.c_str(), strEmail.c_str(), strIP.c_str() ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket.m_nOK );
        m_kODBC.EndFetch();
    }

    SLayerType::GetInstance()->m_kActorManager2.QueueingEventTo( strLogin_, KNUserEvent::ENU_ACCOUNT_REGISTER_ACK, kPacket );

    LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK)
        << BUILD_LOG(NetError::GetLastNetErrMsg())
        << BUILD_LOG(kPacket.m_nOK)
        << BUILD_LOG(strPasswd)
        << BUILD_LOG(strLogin)
        << BUILD_LOG(strEmail)
        << BUILD_LOG(strIP)
        << BUILD_LOG(kPacket_.m_nProtocolVer) << END_LOG;
}
