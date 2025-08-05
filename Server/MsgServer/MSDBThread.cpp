#include <winsock2.h>
#include <iomanip>
#include <sstream>
#include <dbg/dbg.hpp>
#include "MSDBThread.h"
#include "MsgServer.h"
#include "MSSimLayer.h"
#include "MRPacket.h"
#include <boost/algorithm/string.hpp> // for replace_all
#include "NetError.h"
//FILE_NAME_FOR_LOG

ImplementDBThread( KMSDBThread );
ImplementDBThreadName( KMSDBThread, Main );

#define CLASS_TYPE KMSDBThread

KMSDBThread::KMSDBThread()
{
    ConnectDBThread();
    ConnectDBThreadName( Main );
}

KMSDBThread::~KMSDBThread(void)
{
    End();
}

void KMSDBThread::ProcessIntEvent( const KIntEvent& kIntEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &kIntEvent_.m_kbuff ) );

    switch( kIntEvent_.m_usEventID )
    {
       INT_CASE( DB_EVENT_SERVER_INFO_REQ );
       //New Packet
       INT_CASE( EMS_S2_VERIFY_ACCOUNT_REQ );
       INT_CASE( EMS_S2_ADD_FRIEND_REQ );
       INT_CASE( EMS_S2_DELETE_FRIEND_REQ );
      _INT_CASE( EMS_S2_ACCEPT_FRIEND_REQ, DWORD );
      _INT_CASE( EMS_S2_REJECT_FRIEND_REQ, DWORD );
       INT_CASE( EMS_S2_BLOCK_FRIEND_REQ );
      _INT_CASE( EMS_S2_UNBLOCK_FRIEND_REQ, DWORD );
       INT_CASE( EMS_S2_BLOCK_MOVE_FRIEND_REQ );
      _INT_CASE( EMS_S2_UNBLOCK_MOVE_FRIEND_REQ, DWORD );
       //Group
      _INT_CASE( EMS_S2_ADD_GROUP_REQ, KEMS_S2_ADD_GROUP_ACK );
      _INT_CASE( EMS_S2_DELETE_GROUP_REQ, int );
       INT_CASE( EMS_S2_RENAME_GROUP_REQ );
       INT_CASE( EMS_S2_MOVE_FRIEND_TO_GROUP_REQ );
       //Emoticon
      _INT_CASE( EMS_S2_SET_EMOTICON_REQ, DWORD );
       // 쪽지
       INT_CASE( EMS_S2_SEND_PAPER_REQ );
       INT_CASE_NOPARAM( EMS_S2_LOAD_SENT_PAPERBOX_REQ );
       INT_CASE_NOPARAM( EMS_S2_LOAD_RECV_PAPERBOX_REQ );
      _INT_CASE( EMS_S2_READ_PAPER_REQ, std::set<DWORD> );
      _INT_CASE( EMS_S2_DEL_SENT_PAPER_REQ, std::set<DWORD> );
      _INT_CASE( EMS_S2_DEL_RECV_PAPER_REQ, std::set<DWORD> );
      _INT_CASE( EMS_S2_SEAL_PAPER_REQ, std::set<DWORD> );
       INT_CASE( EMS_S2_ADD_FRIEND_FORCE_REQ );
       INT_CASE( EMS_S2_LOGOUT_LOG_NOT );
      
      _INT_CASE( EMS_ADD_GUILD_MEMBER_LIST_REQ, KUpdateGuildMemberReq );
       INT_CASE( EMS_GUILD_ALL_MEMBER_LIST_REQ );

      

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " 
            << IDVenderType::GetEventIDString( kIntEvent_.m_usEventID ) ) << END_LOG;
    }
}

INT_IMPL_ON_FUNC( DB_EVENT_SERVER_INFO_REQ )
{
    if( kPacket_.m_bFirstDBWork )
    {
        // new
        // db에서 uid를 얻어와서 이름을 만든다.
//         -- 서버의 UID를 리턴합니다.
//             call dbo.MSG_MsgServerUID_Select
//             @strIP [nvarchar](24)
//             ,   @iPort [int]
// 
//             return 1 loop select
//                 UID     int
        DBJIF( m_kODBC.Query( L"{ call dbo.MSG_MsgServerUID_Select ( N'%s', %d ) }",
            kPacket_.m_strIP.c_str(),
            kPacket_.m_usPort ) );

        if( m_kODBC.BeginFetch() )
        {
            int iUID = 0;
            VERIFY_QUERY( m_kODBC >> iUID );

            boost::wformat fmt( L"MsgServer_%02d" );
            fmt % iUID;
            SiKMsgServer()->SetUID( iUID );
            SiKMsgServer()->SetServerName( fmt.str() );
        }
        m_kODBC.EndFetch();

        //new
        //msg router의 주소정보를 얻어낸다.
//         -- 메신저 라우터의 ID등을 리턴합니다.
//             call dbo.MSG_MsgRouterID_Select
//             @bBUse    [bit]
// 
//             return 1 loop select
//                 ID        int
//                 IP       nvarchar(32)
//                 Port    int
        DBJIF( m_kODBC.Query( L"{ call dbo.MSG_MsgRouterList_select ( %d ) }",
            1 ) );

        int iUID;
        std::wstring strIP;
        int iPort;

        while( m_kODBC.Fetch() != SQL_NO_DATA )
        {
            VERIFY_QUERY( m_kODBC >> iUID >> strIP >> iPort );

            SiKMRConnector()->SetMRAddress( iUID, KncUtil::toNarrowString( strIP ).c_str(), (USHORT)iPort );
        }
    }

    /*  
    L"{ call dbo.MSG_MsgServer_UserNum_Update_20120329 ( %d, %d, N'%s', %d, %d ) }"  
      
    { call dbo.MSG_MsgServer_UserNum_Update_20120329 ( @1, @2, @3, @4, @5 ) }  
    @1 ; @iUserNum_          int  
    @2 ; @iProtocolVersion_  int  
    @3 ; @strIP_             nvarchar(16)  
    @4 ; @iPort_             int  
    @5 ; @iServerID_         int  
      
    1 select ( @1 )  
    @1  ; OK int  
        0        ; 성공  
        -1       ; 해당 서버ID/Port가 유효하지 않음  
        -101이하 ; 트랜잭션 에러  
    */

    DWORD dwUID = SiKMsgServer()->GetUID();
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_MsgServer_UserNum_Update_20120329 ( %d, %d, N'%s', %d, %d ) }",
        kPacket_.m_mapChannellingUserNum[0],
        kPacket_.m_nProtocolVer,
        kPacket_.m_strIP.c_str(),
        kPacket_.m_usPort,
        dwUID ) );

    int nOK = -99;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    if( nOK != 0 )
    {
        START_LOG( cerr, L"?" << nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }

    if( kPacket_.m_hSyncEvent != NULL ) // 060404. florist. 메인 스레드에 작업의 종료를 알려야 하는 경우.
    {
        SetEvent( kPacket_.m_hSyncEvent );
    }
}

//-------------------------------------------------------------------------------

INT_IMPL_ON_FUNC( EMS_S2_VERIFY_ACCOUNT_REQ )
{
    KEMS_S2_VERIFY_ACCOUNT_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_bNewPaperAlarm = false;
    kPacket.m_dwEmoticon = 1;
 
    // 접속 인증
/*
{ call dbo.MSG_Verify_Account ( %d, N'%s' ) }

{ call dbo.MSG_Verify_Account ( @1, @2 ) }
@1 ; LoginUID int
@2 ; Login nvarchar(20)

1 return ( @1 )
@1 ; OK int

OK
0 ; 인증 성공
-1 ; 유저 정보가 없음
*/
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Verify_Account ( %d, N'%s' ) }",
        kPacket_.m_dwUserUID,
        kPacket_.m_strLogin.c_str() ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket.m_nOK );
        m_kODBC.EndFetch();
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_strLogin ) << END_LOG;

    // 접속 실패
    if( kPacket.m_nOK != 0 )
    {
        SET_ERR_VAL( ERR_MSN_01, kPacket.m_nOK, 1 );
        QUEUING_ACK_TO_USER( EMS_S2_VERIFY_ACCOUNT_ACK );
        return;
    }

/*
L"{ call dbo.MSG_Group_Select ( %d, %d ) }"

{ call dbo.MSG_Group_Select( @1, @2 }
@1: @iLoginUID_   int
@2: @iServerLanguageID_ int

n return ( @1, @2 )
@1: GroupID   tinyint
@2: GroupName nvarchar( 15 )
*/

    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Group_Select ( %d, %d ) }",
        kPacket_.m_dwUserUID, kPacket_.m_nLanguageCode ) );
//     DB에서 내려주는 값
//     FRIEND_REQ   = 0 // 친구요청(아직 친구는 아닌 상태)
//     NORMAL_BLOCK = 1 // 쌩차단(유저 직접 닉네임 입력 차단)
//     FRIEND_BLOCK = 2 // 친구차단(친구목록서 R버튼 메뉴 차단)
//     GUILD_BASE   = 3 // 길드
//     FRIEND_BASE  = 10// 친구기본그룹
//     11~19 : 사용자 정의 친구 그룹
    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KGroup kGroup;

        VERIFY_QUERY( m_kODBC >> kGroup.m_nGroupKey
            >> kGroup.m_strGroupName );

        switch( kGroup.m_nGroupKey )
        {
        case KGroup::FRIEND_BLOCK:// 친구를 블럭한 그룹(그룹생성x)
        case KGroup::FRIEND_REQ:  // 친구추가요청대기그룹(그룹생성x)
            break;
        case KGroup::NORMAL_BLOCK:
            kPacket.m_kBlockGroup.m_nGroupKey = KGroup::NORMAL_BLOCK;
            kPacket.m_kBlockGroup.m_strGroupName = kGroup.m_strGroupName;
            break;
        case KGroup::GUILD_BASE:
            kPacket.m_kGuildGroup.m_nGroupKey = KGroup::GUILD_BASE;
            kPacket.m_kGuildGroup.m_strGroupName = kGroup.m_strGroupName;
            break;
        case KGroup::FRIEND_BASE: // 기본그룹
            kPacket.m_mapFriendGroup.insert( std::make_pair( KGroup::FRIEND_BASE, kGroup ) );
            break;
        default: // 추가된 친구그룹으로 판단.
            kPacket.m_mapFriendGroup.insert( std::make_pair( kGroup.m_nGroupKey, kGroup ) );
            break;
        }

        START_LOG( clog, L"Friend group key and name" )
            << BUILD_LOG( kGroup.m_nGroupKey )
            << BUILD_LOG( kGroup.m_strGroupName ) << END_LOG;
    }

    START_LOG( clog, L"MSG_Group_Select" )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_nLanguageCode )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

/*
20081023. microcat.

{ call dbo.MSG_Friend_Select ( %d ) }

{ call dbo.MSG_Friend_Select ( @1 ) }
@1 ; LoginUID int

n return ( @1, @2, @3 )
@1 ; GroupID int
@2 ; FriendUID int
@3 ; FriendNick nvarchar(24)
*/
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Friend_Select_Color ( %d ) }",
        kPacket_.m_dwUserUID ) );
    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        int nGroupKey = -1;
        KGroup kGroup;
        KFriend kFriend;

        VERIFY_QUERY( m_kODBC >> nGroupKey
            >> kFriend.m_dwUserUID
            >> kFriend.m_strNickName
            >> kFriend.m_strNickColor);

        std::map<int,KGroup>::iterator mit;

        switch( nGroupKey )
        {
        case KGroup::NORMAL_BLOCK:
            kFriend.m_bIsFriend = false;//쌩판남이다.
            kPacket.m_kBlockGroup.m_mapMembers.insert( std::make_pair(kFriend.m_dwUserUID, kFriend) );
            break;
        case KGroup::FRIEND_BLOCK:
            kFriend.m_bIsFriend = true;//이전엔친구였지.
            kPacket.m_kBlockGroup.m_mapMembers.insert( std::make_pair(kFriend.m_dwUserUID, kFriend) );
            break;
        case KGroup::FRIEND_REQ: // 친구추가요청대기그룹
            {
                mit = kPacket.m_mapFriendGroup.find( KGroup::FRIEND_BASE );
                if( mit == kPacket.m_mapFriendGroup.end() )
                {
                    kGroup.m_nGroupKey = KGroup::FRIEND_BASE;//앞에서 제대로 안만들어진경우.
                    mit = kPacket.m_mapFriendGroup.insert( std::make_pair( kGroup.m_nGroupKey, kGroup ) ).first;
                }

                kFriend.m_bIsFriend = false;//친추했지만 아직 친구는 아니다.
                mit->second.m_mapMembers.insert( std::make_pair(kFriend.m_dwUserUID, kFriend) );
            }
            break;
        case KGroup::FRIEND_BASE: // 기본그룹
            {
                mit = kPacket.m_mapFriendGroup.find( KGroup::FRIEND_BASE );
                if( mit == kPacket.m_mapFriendGroup.end() )
                {
                    kGroup.m_nGroupKey = KGroup::FRIEND_BASE;
                    mit = kPacket.m_mapFriendGroup.insert( std::make_pair( kGroup.m_nGroupKey, kGroup ) ).first;
                }

                kFriend.m_bIsFriend = true;
                mit->second.m_mapMembers.insert( std::make_pair(kFriend.m_dwUserUID, kFriend) );
            }
            break;
        default: // 추가된 친구그룹.
            {
                mit = kPacket.m_mapFriendGroup.find( nGroupKey );
                if( mit == kPacket.m_mapFriendGroup.end() )
                {
                    kGroup.m_nGroupKey = nGroupKey;
                    mit = kPacket.m_mapFriendGroup.insert( std::make_pair( kGroup.m_nGroupKey, kGroup ) ).first;
                }

                kFriend.m_bIsFriend = true;
                mit->second.m_mapMembers.insert(  std::make_pair(kFriend.m_dwUserUID, kFriend) );
            }
            break;
        }
    }


    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Emoticon_Select ( %d ) }",
        kPacket_.m_dwUserUID ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket.m_dwEmoticon );
        m_kODBC.EndFetch();
    }

    {
        static unsigned int MAX_GUILD_USER_NUM = 200;

        if ( kPacket_.m_nGuildID > 0 ) {
            _DBJIF( m_kODBCMain.Query( L"{ call dbo.GSGAGuildSystemMember_select_1_guild ( %d ) }", kPacket_.m_nGuildID ), return, ( &m_kODBCMain ) );

            std::wstring strSP;
            std::set<DWORD> setUserList;

            while( m_kODBCMain.Fetch() != SQL_NO_DATA )
            {
                DWORD dwUserUID = 0;
                VERIFY_QUERY( m_kODBCMain>> strSP
                    >> dwUserUID );

                if ( dwUserUID > 0 ) {
                    setUserList.insert( dwUserUID );
                }
            }

            if( !setUserList.empty() ) {
                std::set<DWORD>::iterator sit;
                for( sit = setUserList.begin(); sit != setUserList.end(); ++sit ) {
                    KNGuildUserInfo kTemp;
                    time_t tmJoinDate;
                    time_t tmLastLogin;
                    KFriend kFriend;

                    GetGuildMemeberInfoFromDB( kPacket_.m_nGuildID, *sit, kTemp, tmJoinDate, tmLastLogin );

                    if ( kTemp.m_cMemberLevel <= KNGuildUserInfo::GL_JOINER ) {
                        continue;
                    }

                    kFriend.m_dwUserUID = kTemp.m_dwUserUID;
                    kFriend.m_strNickName = kTemp.m_strNickName;
                    kFriend.m_strNickColor = kTemp.m_strNickColor;
                    kFriend.m_nGuildUserLevel = kTemp.m_cMemberLevel;
                    kFriend.m_bIsFriend = true;
                    kPacket.m_mapFriendGroup[KGroup::GUILD_BASE].m_nGroupKey = KGroup::GUILD_BASE;
                    kPacket.m_mapFriendGroup[KGroup::GUILD_BASE].m_mapMembers.insert( std::make_pair( kFriend.m_dwUserUID, kFriend ) );

                    START_LOG( clog, L"길드원 정보. GuildUID : " << kTemp.m_dwGuildUID )
                        << BUILD_LOG( kFriend.m_strNickName )
                        << BUILD_LOG( kFriend.m_strNickColor )
                        << BUILD_LOG( kFriend.m_nGuildUserLevel ) << END_LOG;

                    if ( kPacket.m_mapFriendGroup[KGroup::GUILD_BASE].m_mapMembers.size() > MAX_GUILD_USER_NUM ) {
                        break;
                    }
                }
            }
            m_kODBCMain.EndFetch();

            START_LOG( clog, L"길드 유저 리스트 사이즈 : " << kPacket.m_mapFriendGroup[KGroup::GUILD_BASE].m_mapMembers.size() )
                << BUILD_LOG( strSP ) << END_LOG;
        }
    }

    QUEUING_ACK_TO_USER( EMS_S2_VERIFY_ACCOUNT_ACK ); //유저에게 자기 정보 우선 주자.

/*----------------------------------------------------------------------------------------------*/
    // 다른 유저에게 받은 친구 추가 요청 목록 얻기.
/*
20081101. microcat.

{ call dbo.MSG_Friend_Request ( %d ) }

{ call dbo.MSG_Friend_Request ( @1 ) }
@1 ; LoginUID int

n return ( @1, @2, @3 )
@1 ; Login nvarchar(20)
@2 ; Nick nvarchar(24)
@3 ; Msg nvarchar(25)
*/
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Friend_Request ( %d ) }",
        kPacket_.m_dwUserUID ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KEMS_S2_ADD_FRIEND_NOT kNot;

        VERIFY_QUERY( m_kODBC >> kNot.m_dwSenderUID
            >> kNot.m_strSenderNick 
            >> kNot.m_strMessage );

        //친구 요청받은 내역을 다이렉트로 알려준다.
        SLayerType::GetInstance()->m_kActorManager.QueueingEventTo( 
            kPacket_.m_dwUserUID, IDVenderType::EMS_S2_ADD_FRIEND_NOT, kNot );
    }
}

INT_IMPL_ON_FUNC( EMS_S2_ADD_FRIEND_REQ )
{
// --친구 요청합니다.
//     call dbo.MSG_Friend_Insert
//     @iLoginUID_ [int]
//     , @strENick_ [nvarchar](24)
//         , @strMsg_ [nvarchar](25) = NULL
// 
//         return 1 loop select
//         @iOK
//         ,   @iELoginUID
//         ,   @strENick_
// 
//         OK value
//        100 : 이전에 요청한 기록이 있다. 나하고 친구는 아니다. 고로 친구추가메세지만 안보내면 된다.
//         10 : 친구 될놈이 나를 이미 친구로 알고있음
//          0 : 요청 성공
//         -1 : 유저가 존재하지 않음
//         -2 : 친구놈이 존재하지 않음
//         -3 : 나를 추가하려 함
//         -4 : 이미 친구이거나 요청중임
//         -101 이하 : 트랜젝션 에러
    std::wstring strQueryMsg = boost::replace_all_copy( kPacket_.m_strMessage, L"'", L"''" );

    _DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Friend_Insert ( %d, N'%s', N'%s' ) }",
        dwUID_, kPacket_.m_strNickName.c_str(), strQueryMsg.c_str() ), , (&m_kODBC) );

    KEMS_S2_ADD_FRIEND_ACK kPacket;
    kPacket.m_strMessage                    = kPacket_.m_strMessage;
    kPacket.m_kFriendInfo.m_dwUserUID       = 0;
    kPacket.m_kFriendInfo.m_strNickName     = kPacket_.m_strNickName;
    kPacket.m_kFriendInfo.m_bIsOnline       = false;
    kPacket.m_kFriendInfo.m_bIsFriend       = false;
    kPacket.m_kFriendInfo.m_bIsGamming      = false;
    kPacket.m_kFriendInfo.m_strServerName   = L"off";

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> kPacket.m_nOK );

        if( kPacket.m_nOK == 0  || // 요청 성공
            kPacket.m_nOK == 10 || // 이미 친구.
            kPacket.m_nOK == 100 ) // 이미 요청했음.
        {
            VERIFY_QUERY( m_kODBC >> kPacket.m_kFriendInfo.m_dwUserUID 
                >> kPacket.m_kFriendInfo.m_strNickName );
        }

        m_kODBC.EndFetch();
    }

    LOG_CONDITION( kPacket.m_nOK == 0 || kPacket.m_nOK == 10 || kPacket.m_nOK == 100, clog, cerr )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    if( kPacket.m_nOK < 0 )
    {
        kPacket.m_kFriendInfo.m_strNickName     = kPacket_.m_strNickName;
        kPacket.m_kFriendInfo.m_dwUserUID       = 0;
    }
    if ( kPacket.m_nOK == 10 ) { // 다이렉트친구
        kPacket.m_kFriendInfo.m_bIsFriend = true;
    }

    switch ( kPacket.m_nOK ) {
    case 100:SET_ERR_VAL( NET_OK, kPacket.m_nOK, 100 );   break; // 이전에 요청한 기록이 있다. 나하고 친구는 아니다. 고로 친구추가메세지만 안보내면 된다.
    case 10: SET_ERR_VAL( NET_OK, kPacket.m_nOK, 10 );    break; // 친구 될놈이 나를 이미 친구로 알고있음
    case  0: SET_ERR_VAL( NET_OK, kPacket.m_nOK, 0 );     break; // 처음 맺는 친구입
    case -1: SET_ERR_VAL( ERR_MSN_01, kPacket.m_nOK, 1 ); break; // 유저가 존재하지 않음
    case -2: SET_ERR_VAL( ERR_MSN_02, kPacket.m_nOK, 2 ); break; // 추가하려는 님이 존재하지 않음
    case -3: SET_ERR_VAL( ERR_MSN_06, kPacket.m_nOK, 6 ); break; // 자기 자신을 추가하려 함
    case -4: SET_ERR_VAL( ERR_MSN_07, kPacket.m_nOK, 7 ); break; // 이미 요청중이거나 친구이거나 블록상태임
    default:
        SET_ERR_VAL( ERR_UNKNOWN, kPacket.m_nOK, -99 );
    }

    QUEUING_ACK_TO_USER( EMS_S2_ADD_FRIEND_ACK );
}


INT_IMPL_ON_FUNC( EMS_S2_DELETE_FRIEND_REQ )
{
    // -- 친구 또는 블럭 유저를 메신저에서 삭제합니다.

/*
{ call dbo.MSG_Friend_Delete ( %d, %d ) }

{ call dbo.MSG_Friend_Delete ( @1, @2 ) }
@1 ; LoginUID int ; 내꺼
@2 ; ELoginUID int ; 친구

1 return ( @1 )
@1 ; OK int
*/
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Friend_Delete ( %d, %d ) }",
        dwUID_,
        kPacket_.m_dwFriendUID ) );

    KEMS_S2_DELETE_FRIEND_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nGroupID= kPacket_.m_nGroupID;
    kPacket.m_dwFriendUID  = kPacket_.m_dwFriendUID;

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket.m_nOK );

        m_kODBC.EndFetch();
    }

    switch( kPacket.m_nOK )
    {
    case  0: SET_ERR_VAL( NET_OK, kPacket.m_nOK, 0 );     break;
    case -1: SET_ERR_VAL( ERR_MSN_01, kPacket.m_nOK, 1 ); break; // 유저가 존재하지 않음
    case -2: SET_ERR_VAL( ERR_MSN_02, kPacket.m_nOK, 2 ); break; // 추가하려는 님이 존재하지 않음
    case -3: SET_ERR_VAL( ERR_MSN_27, kPacket.m_nOK, 27 ); break; // 친구나 블록 정보가 없음
    default:
        SET_ERR_VAL( ERR_UNKNOWN, kPacket.m_nOK, -99 );
    }

    QUEUING_ACK_TO_USER( EMS_S2_DELETE_FRIEND_ACK );
}


_INT_IMPL_ON_FUNC( EMS_S2_ACCEPT_FRIEND_REQ, DWORD )
{
    // -- 친구요청을 수락합니다.
    // call dbo.MSG_Friend_Accept
    // (
    //  @iLoginUID_       int     -- 친구 요청한 님의 UID
    //  ,    @iELoginUID_     int     -- 요청을 수락한 님의 UID
    //  )
    // 
    //  return 1 loop select
    //  ( OK int )
    //  nickname
    // 
    //  OK
    //  0 : 수락 성공
    //  -1 : 요청한 유저가 존재하지 않음
    //  -2 : 수락할 유저가 존재하지 않음
    //  -3 : 친구요청을 받은적이 없음
    //  -101이하 : 트랜잭션 에러
    // (요놈아가 정상적으로 실행되면 서로 친구관계를 맺어줍니다)
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Friend_Accept ( %d, %d ) }",
        kPacket_,
        dwUID_ ) );

    KEMS_S2_ADD_FRIEND_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_kFriendInfo.m_bIsOnline       = false;
    kPacket.m_kFriendInfo.m_bIsFriend       = false;
    kPacket.m_kFriendInfo.m_strServerName   = L"off";
    kPacket.m_kFriendInfo.m_dwUserUID       = kPacket_;

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket.m_nOK );

        if( kPacket.m_nOK == 0 )
        {
            VERIFY_QUERY( m_kODBC >> kPacket.m_kFriendInfo.m_strNickName );
            kPacket.m_kFriendInfo.m_bIsFriend = true; // 성공적으로 친구 수락됨.
            m_kODBC.EndFetch();
        }

        m_kODBC.EndFetch();
    }

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cerr )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    switch( kPacket.m_nOK )
    {
    case  0: SET_ERR_VAL( NET_OK, kPacket.m_nOK, 0 );     break;
    case -1: SET_ERR_VAL( ERR_MSN_01, kPacket.m_nOK, 1 ); break; // 유저가 존재하지 않음
    case -2: SET_ERR_VAL( ERR_MSN_02, kPacket.m_nOK, 2 ); break; // 추가하려는 님이 존재하지 않음
    case -3: SET_ERR_VAL( ERR_MSN_09, kPacket.m_nOK, 9 ); break; // 친구정보를 요청한 적이 없음
    default:
        SET_ERR_VAL( ERR_UNKNOWN, kPacket.m_nOK, -99 );
    }

    QUEUING_ACK_TO_USER( EMS_S2_ACCEPT_FRIEND_ACK );
}

_INT_IMPL_ON_FUNC( EMS_S2_REJECT_FRIEND_REQ, DWORD )
{
    // -- 친구요청을 거부합니다.
    // call dbo.MSG_Friend_Reject
    // (
    // @iLoginUID_       int     -- 친구 요청한 님의 UID
    // ,    @iELoginUID_     int     -- 요청을 거부한 님의 UID
    // )
    // 
    // return 1 loop select
    // ( OK int )
    // 
    // OK
    // 0 : 거부 성공
    // -1 : 요청한 유저가 존재하지 않음
    // -2 : 거부할 유저가 존재하지 않음
    // -3 : 친구요청을 받은적이 없음
    // -101이하 : 트랜잭션 에러
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Friend_Reject ( %d, %d ) }",
        kPacket_,
        dwUID_ ) );

    int nOK = -99;

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );

        m_kODBC.EndFetch();
    }

    LOG_CONDITION( nOK == 0, clog, cwarn )
        << BUILD_LOG( nOK )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( dwUID_ ) << END_LOG;

    if( nOK == 0 )      // 성공한 경우에만 이후작업 진행.
    {
        DWORD kPacket = kPacket_;
        QUEUING_ACK_TO_USER( EMS_S2_REJECT_FRIEND_ACK );
    }
}

INT_IMPL_ON_FUNC( EMS_S2_BLOCK_FRIEND_REQ )
{
// --차단 요청합니다.
/*
20081023. microcat.

{ call dbo.MSG_Friend_Block_Insert ( %d, N'%s' ) }

{ call dbo.MSG_Friend_Block_Insert ( @1, @2 ) }
@1 ; LoginUID int
@1 ; BlockNick nvarchar(24)

1 return ( @1, @2, @3 )
@1 ; OK int
@2 ; BlockLoginUID int
@3 ; BlockNick nvarchar(24)
*/
    std::wstring strMsg;
    strMsg.clear();

    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Friend_Block_Insert ( %d, N'%s' ) }",
        dwUID_, kPacket_.m_strNickName.c_str() ) );

    KEMS_S2_BLOCK_FRIEND_ACK kPacket;
    kPacket.m_nOK           = -99;

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket.m_nOK );

        if( kPacket.m_nOK == 0 )
        {
            VERIFY_QUERY( m_kODBC >> kPacket.m_dwBlockUID
                >> kPacket.m_strBlockNick );
            m_kODBC.EndFetch();
        }

        m_kODBC.EndFetch();
    }

    if( kPacket.m_nOK != 0 )
    {
        kPacket.m_dwBlockUID = 0;
        kPacket.m_strBlockNick = kPacket_.m_strNickName;
    }

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cerr )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwBlockUID )
        << BUILD_LOG( kPacket.m_strBlockNick )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    switch( kPacket.m_nOK )
    {
    case 0: SET_ERR_VAL( NET_OK, kPacket.m_nOK, 0 ); break;
    case -1: SET_ERR_VAL( ERR_MSN_01, kPacket.m_nOK, 1 ); break; // 유저가 존재하지 않음
    case -2: SET_ERR_VAL( ERR_MSN_02, kPacket.m_nOK, 2 ); break; // 추가하려는 님이 존재하지 않음
    case -3: SET_ERR_VAL( ERR_MSN_06, kPacket.m_nOK, 6 ); break; // 자기 자신을 추가하려 함
    default:
        SET_ERR_VAL( ERR_UNKNOWN, kPacket.m_nOK, -99 );
    }

    QUEUING_ACK_TO_USER( EMS_S2_BLOCK_FRIEND_ACK );
}

_INT_IMPL_ON_FUNC( EMS_S2_UNBLOCK_FRIEND_REQ, DWORD )
{
    // -- 친구 또는 블럭 유저를 메신저에서 삭제합니다.
    // 바로 블럭만 한 경우. 나랑 생판 남이니까, 블럭상태 해제 = 삭제

/*
{ call dbo.MSG_Friend_Delete ( %d, %d ) }

{ call dbo.MSG_Friend_Delete ( @1, @2 ) }
@1 ; LoginUID int ; 내꺼
@2 ; ELoginUID int ; 친구

1 return ( @1 )
@1 ; OK int
*/
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Friend_Delete ( %d, %d ) }",
        dwUID_, kPacket_ ) );

    KEMS_S2_UNBLOCK_FRIEND_ACK kPacket;
    kPacket.m_nOK           = -99;
    kPacket.m_dwBlockUID    = kPacket_;

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket.m_nOK );

        m_kODBC.EndFetch();
    }

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cerr )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwBlockUID )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    switch( kPacket.m_nOK )
    {
    case 0: SET_ERR_VAL( NET_OK, kPacket.m_nOK, 0 ); break;
    case -1: SET_ERR_VAL( ERR_MSN_01, kPacket.m_nOK, 1 ); break; // 유저가 존재하지 않음
    case -2: SET_ERR_VAL( ERR_MSN_02, kPacket.m_nOK, 2 ); break; // 추가하려는 님이 존재하지 않음
    case -3: SET_ERR_VAL( ERR_MSN_27, kPacket.m_nOK, 27 ); break; // 친구나 블록 정보가 없음
    default:
        SET_ERR_VAL( ERR_UNKNOWN, kPacket.m_nOK, -99 );
    }

    QUEUING_ACK_TO_USER( EMS_S2_UNBLOCK_FRIEND_ACK );
}

INT_IMPL_ON_FUNC( EMS_S2_BLOCK_MOVE_FRIEND_REQ )
{
    // -- 친구 또는 블록을 요청합니다.
    // call dbo.MSG_Friend_Update
    //     (
    //         @iLoginUID_      int     -- 변경요청을 한 UID
    //         ,    @iELoginUID_    int     -- 변경될 UID
    //         ,    @bBeBlock_      bit     -- 친구에서 블록이면 1 // 블록에서 친구면 0
    //         )
    // 
    //         return 1 loop select
    //         ( OK int )
    // 
    //         OK
    //         0 : 변경 성공
    //         -1 : 요청한 유저가 존재하지 않음
    //         -2 : 요청받은 유저가 존재하지 않음
    //         -3 : 친구로 변경할 유저가 블록이 아님
    //         -4 : 블록으로 변경할 유저가 친구가 아님
    //         -101이하 : 트랜잭션 에러
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Friend_Update ( %d, %d, %d ) }",
        dwUID_, kPacket_.m_dwFriendUID, 1 ) );

    KEMS_S2_BLOCK_MOVE_FRIEND_ACK kPacket;
    kPacket.m_nOK           = -99;
    kPacket.m_nGroupID      = kPacket_.m_nGroupID;
    kPacket.m_dwFriendUID    = kPacket_.m_dwFriendUID;

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket.m_nOK );

        m_kODBC.EndFetch();
    }

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cerr )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nGroupID )
        << BUILD_LOG( kPacket.m_dwFriendUID )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    switch( kPacket.m_nOK )
    {
    case 0: SET_ERR_VAL( NET_OK, kPacket.m_nOK, 0 ); break;
    case -1: SET_ERR_VAL( ERR_MSN_01, kPacket.m_nOK, 1 ); break; // 유저가 존재하지 않음
    case -2: SET_ERR_VAL( ERR_MSN_02, kPacket.m_nOK, 2 ); break; // 추가하려는 님이 존재하지 않음
    case -3: SET_ERR_VAL( ERR_MSN_27, kPacket.m_nOK, 27 ); break;// 해당 (내블럭)그룹에 존재하지 않는 유저임
    case -4: SET_ERR_VAL( ERR_MSN_27, kPacket.m_nOK, 27 ); break;// 해당 (내친구)그룹에 존재하지 않는 유저임
    default:
        SET_ERR_VAL( ERR_UNKNOWN, kPacket.m_nOK, -99 );
    }

    QUEUING_ACK_TO_USER( EMS_S2_BLOCK_MOVE_FRIEND_ACK );
}

_INT_IMPL_ON_FUNC( EMS_S2_UNBLOCK_MOVE_FRIEND_REQ, DWORD )
{
    // -- 친구 상태를 변경합니다.
    // call dbo.MSG_Friend_Update
    // (
    // @iLoginUID_      int     -- 변경요청을 한 UID
    // ,    @iELoginUID_    int     -- 변경될 UID
    // ,    @bBeBlock_      bit     -- 친구에서 블록이면 1 // 블록에서 친구면 0
    // )
    // 
    // return 1 loop select
    // ( OK int )
    // 
    // OK
    // 0 : 변경 성공
    // -1 : 요청한 유저가 존재하지 않음
    // -2 : 요청받은 유저가 존재하지 않음
    // -3 : 친구로 변경할 유저가 블록이 아님
    // -4 : 블록으로 변경할 유저가 친구가 아님
    // -101이하 : 트랜잭션 에러
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Friend_Update ( %d, %d, %d ) }",
        dwUID_, kPacket_, 0 ) );

    KEMS_S2_UNBLOCK_MOVE_FRIEND_ACK kPacket;
    kPacket.m_nOK         = -99;
    kPacket.m_nGroupID    = KGroup::FRIEND_BASE;
    kPacket.m_dwFriendUID = kPacket_;

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket.m_nOK );

        m_kODBC.EndFetch();
    }

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cerr )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nGroupID )
        << BUILD_LOG( kPacket.m_dwFriendUID )
        << BUILD_LOG( dwUID_ ) << END_LOG;

    switch( kPacket.m_nOK )
    {
    case 0: SET_ERR_VAL( NET_OK, kPacket.m_nOK, 0 ); break;
    case -1: SET_ERR_VAL( ERR_MSN_01, kPacket.m_nOK, 1 ); break; // 유저가 존재하지 않음
    case -2: SET_ERR_VAL( ERR_MSN_02, kPacket.m_nOK, 2 ); break; // 추가하려는 님이 존재하지 않음
    case -3: SET_ERR_VAL( ERR_MSN_06, kPacket.m_nOK, 6 ); break; // 자기 자신을 추가하려 함
    case -4: SET_ERR_VAL( ERR_MSN_07, kPacket.m_nOK, 7 ); break; // 이미 요청중이거나 친구이거나 블록상태임
    default:
        SET_ERR_VAL( ERR_UNKNOWN, kPacket.m_nOK, -99 );
    }

    QUEUING_ACK_TO_USER( EMS_S2_UNBLOCK_MOVE_FRIEND_ACK );
}

_INT_IMPL_ON_FUNC( EMS_S2_ADD_GROUP_REQ, KEMS_S2_ADD_GROUP_ACK )
{
//     -- 그룹을 생성합니다.
// call dbo.MSG_Group_Insert
// (
// @iLoginUID_             int
// ,    @tiGroupID_             tinyint
// ,    @strGroupName_    nvarchar(12)
// )
// 
// return 1 loop select
// ( OK int )
// 
// OK
// 0 : 그룹생성 성공
// -1 : 유저가 존재하지 않음
// -2 : 해당 이름을 가진 그룹이 존재함
// -3 : 그룹의 수가 10개임
// -101이하 : 트랜잭션 에러
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Group_Insert ( %d, %d, N'%s' ) }",
        dwUID_, kPacket_.m_nGroupID, kPacket_.m_strGroupName.c_str() ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket_.m_nOK );
        m_kODBC.EndFetch();
    }

    _LOG_CONDITION( kPacket_.m_nOK == 0, clog, cerr, L"kPacket_.m_nOK : " << kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nGroupID )
        << BUILD_LOG( kPacket_.m_strGroupName )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    switch( kPacket_.m_nOK )
    {
    case 0: SET_ERR_VAL( NET_OK, kPacket_.m_nOK, 0 ); break;
    case -1: SET_ERR_VAL( ERR_MSN_01, kPacket_.m_nOK, 1 ); break; // 유저가 존재하지 않음
    case -2: SET_ERR_VAL( ERR_MSN_10, kPacket_.m_nOK, 10 ); break; // 해당 이름을 가진 그룹이 존재함
    case -3: SET_ERR_VAL( ERR_MSN_12, kPacket_.m_nOK, 12 ); break; // 그룹의 수가 10개임
    case -4: SET_ERR_VAL( ERR_MSN_09, kPacket_.m_nOK, 9 ); break; // 친구 리스트가 요청된 적이 없음
    default:
        SET_ERR_VAL( ERR_UNKNOWN, kPacket_.m_nOK, -99 );
    }

    _QUEUING_ACK_TO_USER( EMS_S2_ADD_GROUP_ACK, kPacket_ );
}
_INT_IMPL_ON_FUNC( EMS_S2_DELETE_GROUP_REQ, int )
{
    // -- 그룹을 삭제합니다.
    // call dbo.MSG_Group_Delete
    // (
    // @iLoginUID_      int
    // ,   @tiGroupID_      tinyint
    // )
    // 
    // return 1 loop select
    // ( OK int )
    // 
    // OK
    // 0 : 그룹명 삭제 성공
    // -1 : 유저가 존재하지 않음
    // -2 : 기본그룹을 삭제하려 함
    // -3 : 활성그룹이 아님
    // -101이하 : 트랜잭션 에러
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Group_Delete ( %d, %d ) }",
        dwUID_, kPacket_ ) );

    KEMS_S2_DELETE_GROUP_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nGroupID = kPacket_;

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket.m_nOK );
        m_kODBC.EndFetch();
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nGroupID )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    switch( kPacket.m_nOK )
    {
    case 0: SET_ERR_VAL( NET_OK, kPacket.m_nOK, 0 ); break;
    case -1: SET_ERR_VAL( ERR_MSN_01, kPacket.m_nOK, 1 ); break; // 유저가 존재하지 않음
    case -2: SET_ERR_VAL( ERR_MSN_15, kPacket.m_nOK, 15 ); break; // 기본그룹을 삭제하려 함
    case -3: SET_ERR_VAL( ERR_MSN_11, kPacket.m_nOK, 11 ); break; // 활성그룹이 아님
    default:
        SET_ERR_VAL( ERR_UNKNOWN, kPacket.m_nOK, -99 );
    }

    QUEUING_ACK_TO_USER( EMS_S2_DELETE_GROUP_ACK );
}

INT_IMPL_ON_FUNC( EMS_S2_RENAME_GROUP_REQ )
{
    // -- 그룹명을 변경합니다.
    // call dbo.MSG_Group_Update
    // (
    // @iLoginUID_             int
    // ,   @tiGroupID_             tinyint
    // ,   @strGroupName_     nvarchar(12)
    // )
    // 
    // return 1 loop select
    // ( OK int )
    // 
    // OK
    // 0 : 그룹명 변경 성공
    // -1 : 유저가 존재하지 않음
    // -2 : 기본그룹을 변경하려 함
    // -3 : 활성그룹이 아님
    // -101이하 : 트랜잭션 에러
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Group_Update ( %d, %d, N'%s' ) }",
        dwUID_, kPacket_.m_nGroupID, kPacket_.m_strNewGroupName.c_str() ) );

    KEMS_S2_RENAME_GROUP_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nGroupID = kPacket_.m_nGroupID;
    kPacket.m_strGroupName = kPacket_.m_strNewGroupName;

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket.m_nOK );
        m_kODBC.EndFetch();
    }

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cerr )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nGroupID )
        << BUILD_LOG( kPacket.m_strGroupName )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( strLogin_ ) << END_LOG;

    switch( kPacket.m_nOK )
    {
    case 0: SET_ERR_VAL( NET_OK, kPacket.m_nOK, 0 ); break;
    case -1: SET_ERR_VAL( ERR_MSN_01, kPacket.m_nOK, 1 ); break; // 유저가 존재하지 않음
    case -2: SET_ERR_VAL( ERR_MSN_15, kPacket.m_nOK, 15 ); break; // 기본그룹을 변경하려 함
    case -3: SET_ERR_VAL( ERR_MSN_11, kPacket.m_nOK, 11 ); break; // 활성그룹이 아님
    default:
        SET_ERR_VAL( ERR_UNKNOWN, kPacket.m_nOK, -99 );
    }

    QUEUING_ACK_TO_USER( EMS_S2_RENAME_GROUP_ACK );
}

INT_IMPL_ON_FUNC( EMS_S2_MOVE_FRIEND_TO_GROUP_REQ )
{
    // -- 친구를 그룹 이동합니다.
    // call dbo.MSG_Friend_Group_Update
    // (
    // @iLoginUID_       int
    // ,   @iELoginUID_     int
    // ,   @tiGroupID_       tinyint
    // )
    // 
    // return 1 loop select
    // ( OK int )
    // 
    // OK
    // 0 : 그룹 이동 성공
    // -1 : 유저가 존재하지 않음
    // -2 : 친구가 존재하지 않음
    // -3 : 이동될 그룹이 없음
    // -101이하 : 트랜잭션 에러
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Friend_Group_Update ( %d, %d, %d ) }",
        dwUID_, kPacket_.m_dwFriendUID, kPacket_.m_nToGroupID ) );

    KEMS_S2_MOVE_FRIEND_TO_GROUP_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwFriendUID = kPacket_.m_dwFriendUID;
    kPacket.m_nFromGroupID = kPacket_.m_nFromGroupID;
    kPacket.m_nToGroupID = kPacket_.m_nToGroupID;

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket.m_nOK );
        m_kODBC.EndFetch();
    }

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cerr )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nFromGroupID )
        << BUILD_LOG( kPacket.m_nToGroupID )
        << BUILD_LOG( kPacket.m_dwFriendUID )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( strLogin_ ) << END_LOG;

    switch( kPacket.m_nOK )
    {
    case 0: SET_ERR_VAL( NET_OK, kPacket.m_nOK, 0 ); break;
    case -1: SET_ERR_VAL( ERR_MSN_01, kPacket.m_nOK, 1 ); break; // 유저가 존재하지 않음
    case -2: SET_ERR_VAL( ERR_MSN_02, kPacket.m_nOK, 2 ); break; // 친구가 존재하지 않음
    case -3: SET_ERR_VAL( ERR_MSN_11, kPacket.m_nOK, 11 ); break; // 해당그룹이 없음
    default:
        SET_ERR_VAL( ERR_UNKNOWN, kPacket.m_nOK, -99 );
    }

    QUEUING_ACK_TO_USER( EMS_S2_MOVE_FRIEND_TO_GROUP_ACK );
}

_INT_IMPL_ON_FUNC( EMS_S2_SET_EMOTICON_REQ, DWORD )
{
    // -- 메인 이모티콘을 변경합니다.
    // call dbo.MSG_Emoticon_Update
    // (
    // @iLoginUID_         int
    // ,   @iEmoticonID_     int
    // )
    // 
    // return 1 loop select
    // ( OK int )
    // 
    // OK
    // 0 : 이모티콘 변경 성공
    // -1 : 유저가 존재하지 않음
    // -2 : 이모티콘을 검색한 적이 없음
    // -3 : 변경될 이모티콘을 소유하지 않음
    // -101이하 : 트랜잭션 에러
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Emoticon_Update ( %d, %d ) }",
        dwUID_, kPacket_ ) );

    KEMS_S2_SET_EMOTICON_ACK kPacket;
    kPacket.m_nOK =  -99;
    kPacket.m_dwEmoticonID = kPacket_;

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket.m_nOK );
        m_kODBC.EndFetch();
    }

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cerr )
        << BUILD_LOG( kPacket.m_dwEmoticonID )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( strLogin_ ) << END_LOG;

    switch( kPacket.m_nOK )
    {
    case 0: SET_ERR_VAL( NET_OK, kPacket.m_nOK, 0 ); break;
    case -1: SET_ERR_VAL( ERR_MSN_01, kPacket.m_nOK, 1 ); break; // 유저가 존재하지 않음
    case -2 : SET_ERR_VAL( ERR_MSN_09, kPacket.m_nOK, 9 ); break; // 이모티콘을 검색한 적이 없음
    case -3 : SET_ERR_VAL( ERR_MSN_18, kPacket.m_nOK, 18 ); break; // 변경될 이모티콘을 소유하지 않음
    default:
        SET_ERR_VAL( ERR_UNKNOWN, kPacket.m_nOK, -99 );
    }

    QUEUING_ACK_TO_USER( EMS_S2_SET_EMOTICON_ACK );
}

INT_IMPL_ON_FUNC( EMS_S2_SEND_PAPER_REQ )
{
//-- 닉네임을 입력하면 LoginUID를 리턴합니다.
// call dbo.MSG_LoginUID_Nick
// @strNick     [nvarchar](24)
// 
// return 1 loop select
// LoginUID     int

    //유저가 입력한 수신자 닉네임이 있으면 UID를 받아오자.
    if( !kPacket_.m_strRecvNick.empty() )
    {
        DBJIF( m_kODBC.Query( L"{ call dbo.MSG_LoginUID_Nick ( N'%s' ) }",
            kPacket_.m_strRecvNick.c_str() ) );

        if( m_kODBC.BeginFetch() )
        {
            DWORD dwUserInputUID = 0;

            VERIFY_QUERY( m_kODBC >> dwUserInputUID );
            m_kODBC.EndFetch();

            if( dwUserInputUID != 0 )
            {
                kPacket_.m_setRecvUIDs.insert( dwUserInputUID );
            }

            START_LOG( clog, L"-Nick To UID" )
                << BUILD_LOG( kPacket_.m_strRecvNick )
                << BUILD_LOG( dwUserInputUID ) << END_LOG;
        }
    }

    KEMS_S2_SEND_PAPER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_strRecvNick  = kPacket_.m_strRecvNick;
    kPacket.m_setRecvUIDs  = kPacket_.m_setRecvUIDs;
    kPacket.m_strMessage   = kPacket_.m_strMessage;
    kPacket.m_dwSkinItemID = kPacket_.m_dwSkinItemID;

    //받는사람이 비어있음.
    if( kPacket_.m_setRecvUIDs.empty() )
    {
        //돌려보내자.
        kPacket.m_nOK = 34; //쪽지 받을수 있는 사람이 없음.
        QUEUING_ACK_TO_USER( EMS_S2_SET_EMOTICON_ACK );
    }

// -- 쪽지 발송합니다.
// call dbo.MSG_Message_Send
// @iLoginUID_          [int]
// ,   @iELoginUID_     [int]
// ,   @strMessage_      [nvarchar](100)
// ,   @SkinType_          [tinyint]
// 
// return 1 loop select
//     OK            int
// 
//     OK value
//     0 : 쪽지보내기 성공
//     -1 : 보내는 님이 존재하지 않음
//     -11 : UID파싱에러
//     -21 : 누군지 모르겠지만 보내님을 블럭함
//     -22 : 누군지 모르겠지만 받는님을 블럭함
//     -31 : 누군지 모르겠지만 50통 넘은 님이 있음
//     -101 이하 : 트랜젝션 에러
    int nOK;
    int nResult = 0;
    std::set<DWORD>::const_iterator csit;

    for( csit = kPacket_.m_setRecvUIDs.begin() ; csit != kPacket_.m_setRecvUIDs.end() ; ++csit )
    {
        DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Message_Send ( %d, %d, N'%s', %d ) }",
            dwUID_, *csit, kPacket_.m_strMessage.c_str(), kPacket_.m_dwSkinItemID ) );

        if( m_kODBC.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBC >> nOK );
            m_kODBC.EndFetch();
        }

        if( nOK != 0 )
        {
            START_LOG( cerr, L"ㄴ쪽지 발송중 실패. nOK : " << nOK )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( strLogin_ )
                << BUILD_LOG( *csit )
                << BUILD_LOG( kPacket.m_strRecvNick )
                << BUILD_LOG( kPacket.m_strMessage )
                << BUILD_LOG( kPacket.m_dwSkinItemID )
                << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
            //실패를 기억
            nResult = nOK;
        }
    }

    switch( nResult )
    {
    case   0: SET_ERR_VAL( NET_OK,     kPacket.m_nOK, 0 ); break;
    case  -1: SET_ERR_VAL( ERR_MSN_01, kPacket.m_nOK, 1 ); break; // 유저가 존재하지 않음
    case -11: SET_ERR_VAL( ERR_MSN_36, kPacket.m_nOK, 36 ); break; // UID파싱에러
    case -21: SET_ERR_VAL( ERR_MSN_37, kPacket.m_nOK, 37 ); break; // 누군지 모르겠지만 보내님을 블럭함
    case -22: SET_ERR_VAL( ERR_MSN_38, kPacket.m_nOK, 38 ); break; // 누군지 모르겠지만 받는님을 블럭함
    case -31: SET_ERR_VAL( ERR_MSN_39, kPacket.m_nOK, 39 ); break; // 누군지 모르겠지만 50통 넘은 님이 있음
    default:
        SET_ERR_VAL( ERR_UNKNOWN, kPacket.m_nOK, -99 );
    }

    QUEUING_ACK_TO_USER( EMS_S2_SEND_PAPER_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EMS_S2_LOAD_SENT_PAPERBOX_REQ )
{
//-- 보낸 쪽지 리스트 입니다.
// { call dbo.MSG_Message_Receive_List ( @1 ) }
// @1 ; @iLoginUID_ [int]
// 
// return n Loop select
// 
// MessageUID        int
// ,    ReceiverUID         int
// ,    ReceiverNick       nvarchar(24)
// ,    Msg                     nvarchar(100)
// ,    RegDate              smalldatetime
// ,    Status                  tinyint
// ,    SkinType             tinyint
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Message_Receive_List ( %d ) }", 
        dwUID_ ) );

    KEMS_S2_UPDATE_SENT_PAPERBOX_NOT kPacket;
    kPacket.clear();

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KPaper kPaperTemp;
        kPaperTemp.m_dwSenderUID = dwUID_;
        kPaperTemp.m_strSenderNick.clear();//닉네임은 유저에서 채우자..
        time_t tmDate;

        VERIFY_QUERY( m_kODBC >> kPaperTemp.m_dwPaperUID
            >> kPaperTemp.m_dwReceiverUID
            >> kPaperTemp.m_strReceiverNick
            >> kPaperTemp.m_strMessage
            >> tmDate
            >> kPaperTemp.m_cState
            >> kPaperTemp.m_dwSkinType );

        //시간기록
        kPaperTemp.m_kDate.m_sYear    = CTime(tmDate).GetYear();
        kPaperTemp.m_kDate.m_cMonth   = CTime(tmDate).GetMonth();
        kPaperTemp.m_kDate.m_cDay     = CTime(tmDate).GetDay();
        kPaperTemp.m_kDate.m_cWeekDay = CTime(tmDate).GetDayOfWeek();

        kPacket.insert( std::make_pair( kPaperTemp.m_dwPaperUID, kPaperTemp ) );
    }

    QUEUING_ACK_TO_USER( EMS_S2_UPDATE_SENT_PAPERBOX_NOT );
}

INT_IMPL_ON_FUNC_NOPARAM( EMS_S2_LOAD_RECV_PAPERBOX_REQ )
{
//-- 받은 쪽지 리스트 입니다.
// { call dbo.MSG_Message_Receive_List ( @1 ) }
// @1 ; @iLoginUID_ [int]
// 
// return n Loop select
// 
// MessageUID        int
// ,    SenderUID           int
// ,    SenderNick          nvarchar(24)
// ,    Msg                     nvarchar(100)
// ,    RegDate              smalldatetime
// ,    Status                  tinyint
// ,    SkinType             tinyint
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Message_Receive_List ( %d ) }", 
        dwUID_ ) );

    KEMS_S2_UPDATE_RECV_PAPERBOX_NOT kPacket;
    kPacket.clear();

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KPaper kPaperTemp;
        kPaperTemp.m_dwReceiverUID = dwUID_;
        kPaperTemp.m_strReceiverNick.clear();//닉네임은 유저에서 채우자..
        time_t tmDate;

        VERIFY_QUERY( m_kODBC >> kPaperTemp.m_dwPaperUID
            >> kPaperTemp.m_dwSenderUID
            >> kPaperTemp.m_strSenderNick
            >> kPaperTemp.m_strMessage
            >> tmDate
            >> kPaperTemp.m_cState
            >> kPaperTemp.m_dwSkinType );

        //시간기록
        kPaperTemp.m_kDate.m_sYear    = CTime(tmDate).GetYear();
        kPaperTemp.m_kDate.m_cMonth   = CTime(tmDate).GetMonth();
        kPaperTemp.m_kDate.m_cDay     = CTime(tmDate).GetDay();
        kPaperTemp.m_kDate.m_cWeekDay = CTime(tmDate).GetDayOfWeek();

        kPacket.insert( std::make_pair( kPaperTemp.m_dwPaperUID, kPaperTemp ) );
    }

    QUEUING_ACK_TO_USER( EMS_S2_UPDATE_RECV_PAPERBOX_NOT );
}

_INT_IMPL_ON_FUNC( EMS_S2_READ_PAPER_REQ, std::set<DWORD> )
{
// -- 쪽지를 읽습니다.
// call dbo.MSG_Message_Read
// @iMessageUID_ [int] -- MessageUID
// 
// return 1 Loop select
// 
// @iOK    int
// 0 : 해당쪽지들 잘 읽음
// -1 : 해당 쪽지가 없음
// -101이하 : 트랜젝션 에러

    //건당 프로시저 호출
    int nOK = -99;
    int nResult = 0;
    std::set<DWORD>::const_iterator csit;
    for( csit = kPacket_.begin() ; csit != kPacket_.end() ; ++csit )
    {
        nOK = -99;
        
        DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Message_Read ( %d ) }",
            *csit ) );

        if( m_kODBC.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBC >> nOK );
            m_kODBC.EndFetch();
        }

        if( nOK != 0 )
        {
            START_LOG( cerr, L"ㄴ쪽지읽기 기록 실패. nOK : " << nOK )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( strLogin_ )
                << BUILD_LOG( *csit )
                << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
            //실패를 기억
            nResult = nOK;
        }
    }

    KEMS_S2_READ_PAPER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_setPaperUIDs = kPacket_;
    
    switch( nResult )
    {
    case  0: SET_ERR_VAL( NET_OK,     kPacket.m_nOK,  0 ); break; // 성공
    case -1: SET_ERR_VAL( ERR_MSN_42, kPacket.m_nOK, 42 ); break; // 하나라도 없는 쪽지가 있음
    default:
        SET_ERR_VAL( ERR_UNKNOWN, kPacket.m_nOK, -99 );
    }

    QUEUING_ACK_TO_USER( EMS_S2_READ_PAPER_ACK );
}

_INT_IMPL_ON_FUNC( EMS_S2_DEL_SENT_PAPER_REQ, std::set<DWORD> )
{
// -- 보낸 쪽지를 삭제합니다.
// call dbo.MSG_Message_Send_Delete
// @iMessageUID_ [int] -- MessageUID
// 
// return 1 Loop select
// 
// @iOK    int
// 0 : 해당쪽지들 잘 삭제됨
// -1 : 하나라도 쪽지가 없음
// -101이하 : 트랜젝션 에러
    int nOK;
    int nResult = 0;
    std::set<DWORD>::const_iterator csit;

    for( csit = kPacket_.begin() ; csit != kPacket_.end() ; ++csit )
    {
        DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Message_Send_Delete ( %d ) }",
            *csit ) );

        if( m_kODBC.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBC >> nOK );
            m_kODBC.EndFetch();
        }

        if( nOK != 0 )
        {
            START_LOG( cerr, L"ㄴ보낸쪽지 삭제 실패. nOK : " << nOK )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( strLogin_ )
                << BUILD_LOG( *csit )
                << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
            //실패를 기억
            nResult = nOK;
        }
    }

    KEMS_S2_DEL_SENT_PAPER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_setPaperUIDs = kPacket_;

    switch( nResult )
    {
    case   0: SET_ERR_VAL( NET_OK,     kPacket.m_nOK, 0 ); break;
    case  -1: SET_ERR_VAL( ERR_MSN_42, kPacket.m_nOK, 42 ); break; // 하나라도 없는 쪽지가 있음
    default:
        SET_ERR_VAL( ERR_UNKNOWN, kPacket.m_nOK, -99 );
    }

    QUEUING_ACK_TO_USER( EMS_S2_DEL_SENT_PAPER_ACK );
}

_INT_IMPL_ON_FUNC( EMS_S2_DEL_RECV_PAPER_REQ, std::set<DWORD> )
{
// -- 받은 쪽지를 삭제합니다.
// call dbo.MSG_Message_Receive_Delete
// @iMessageUID_ [int](4000) -- MessageUID
// 
// return 1 Loop select
// 
// @iOK    int
// 0 : 해당쪽지들 잘 삭제됨
// -1 : 해당쪽지가 없음
// -101이하 : 트랜젝션 에러
    int nOK;
    int nResult = 0;
    std::set<DWORD>::const_iterator csit;

    for( csit = kPacket_.begin() ; csit != kPacket_.end() ; ++csit )
    {
        DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Message_Receive_Delete ( %d ) }",
            *csit ) );

        if( m_kODBC.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBC >> nOK );
            m_kODBC.EndFetch();
        }
        
        if( nOK != 0 )
        {
            START_LOG( cerr, L"ㄴ쪽지 삭제 실패. nOK : " << nOK )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( strLogin_ )
                << BUILD_LOG( *csit )
                << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
            //실패를 기억
            nResult = nOK;
        }
    }

    KEMS_S2_DEL_RECV_PAPER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_setPaperUIDs = kPacket_;

    switch( nResult )
    {
    case   0: SET_ERR_VAL( NET_OK,     kPacket.m_nOK, 0 ); break;
    case  -1: SET_ERR_VAL( ERR_MSN_42, kPacket.m_nOK, 42 ); break; // 하나라도 없는 쪽지가 있음
    default:
        SET_ERR_VAL( ERR_UNKNOWN, kPacket.m_nOK, -99 );
    }

    QUEUING_ACK_TO_USER( EMS_S2_DEL_RECV_PAPER_ACK );
}

_INT_IMPL_ON_FUNC( EMS_S2_SEAL_PAPER_REQ, std::set<DWORD> )
{
// -- 쪽지를 영구로 변경합니다.
// call dbo.MSG_Message_Permanence
// @iMessageUID_ [int] -- MessageUID
// 
// return 1 Loop select
// 
// @iOK    int
// 0 : 해당쪽지들 잘 변경됨
// -1 : 하나라도 쪽지가 없음
// -2 : 하나라도 이미 영구 쪽지임
// -11 : 영구쪽지가 이미 30개가 넘음
// -101이하 : 트랜젝션 에러
    int nOK;
    int nResult = 0;
    std::set<DWORD>::const_iterator csit;

    for( csit = kPacket_.begin() ; csit != kPacket_.end() ; ++csit )
    {
        DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Message_Permanence ( %d ) }",
            *csit ) );

        if( m_kODBC.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBC >> nOK );
            m_kODBC.EndFetch();
        }

        if( nOK != 0 )
        {
            START_LOG( cerr, L"ㄴ쪽지영구 변경 실패. nOK : " << nOK )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( strLogin_ )
                << BUILD_LOG( *csit )
                << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
            //실패를 기억
            nResult = nOK;
        }
    }

    KEMS_S2_SEAL_PAPER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_setPaperUIDs = kPacket_;

    switch( nResult )
    {
    case   0: SET_ERR_VAL( NET_OK,     kPacket.m_nOK, 0 ); break;
    case  -1: SET_ERR_VAL( ERR_MSN_42, kPacket.m_nOK, 42 ); break; // 하나라도 없는 쪽지가 있음
    case  -2: SET_ERR_VAL( ERR_MSN_44, kPacket.m_nOK, 44 ); break; // 하나라도 이미 영구 쪽지임
    case -11: SET_ERR_VAL( ERR_MSN_45, kPacket.m_nOK, 45 ); break; // 영구쪽지가 이미 30개가 넘음
    default:
        SET_ERR_VAL( ERR_UNKNOWN, kPacket.m_nOK, -99 );
    }

    QUEUING_ACK_TO_USER( EMS_S2_SEAL_PAPER_ACK );
}

INT_IMPL_ON_FUNC( EMS_S2_ADD_FRIEND_FORCE_REQ )
{
    // 이자식은 블럭된 친구 일수도 있고, 아무것도 아닌것일수 있다..
    // 블럭 해제 한다..
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Friend_Delete ( %d, %d ) }",
        kPacket_.m_dwRequesterUID, kPacket_.m_dwRequesteeUID ) );
    m_kODBC.BeginFetch();
    m_kODBC.EndFetch();

    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Friend_Delete ( %d, %d ) }",
        kPacket_.m_dwRequesteeUID, kPacket_.m_dwRequesterUID ) );
    m_kODBC.BeginFetch();
    m_kODBC.EndFetch();

    // 친구 등록 한다.
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Friend_Insert ( %d, N'%s', N'%s' ) }", 
        kPacket_.m_dwRequesterUID, kPacket_.m_strRequesteeNick.c_str(), L"" ) );
    //        100 : 이전에 요청한 기록이 있다. 나하고 친구는 아니다. 고로 친구추가메세지만 안보내면 된다.
    //         10 : 친구 될놈이 나를 이미 친구로 알고있음
    //          0 : 요청 성공
    //         -1 : 유저가 존재하지 않음
    //         -2 : 친구놈이 존재하지 않음
    //         -3 : 나를 추가하려 함
    //         -4 : 이미 친구이거나 요청중임
    //         -101 이하 : 트랜젝션 에러
    m_kODBC.BeginFetch();
    m_kODBC.EndFetch();



    // 친구 수락 한다.
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_Friend_Accept ( %d, %d ) }",
        kPacket_.m_dwRequesterUID, kPacket_.m_dwRequesteeUID ) );
    //  0 : 수락 성공
    //  -1 : 요청한 유저가 존재하지 않음
    //  -2 : 수락할 유저가 존재하지 않음
    //  -3 : 친구요청을 받은적이 없음
    //  -101이하 : 트랜잭션 에러
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket_.m_nOK );
    }
    m_kODBC.EndFetch();
    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwRequesterUID )
        << BUILD_LOG( kPacket_.m_strRequesterNick )
        << BUILD_LOG( kPacket_.m_dwRequesteeUID )
        << BUILD_LOG( kPacket_.m_strRequesteeNick ) << END_LOG;

    if( kPacket_.m_nOK != 0 ) kPacket_.m_nOK = 1;

    _QUEUING_ACK_TO_USER( EMS_S2_ADD_FRIEND_FORCE_ACK, kPacket_ );
}

INT_IMPL_ON_FUNC( EMS_S2_LOGOUT_LOG_NOT )
{

    //{ call dbo.UTGA_user_trace_insert ( @1, @2, @3, @4 ) }
    //@1 ; @iLoginUID_   int
    //@2 ; @dtStartDate_ datetime
    //@3 ; @dtEndDate_   datetime
    //@4 ; @iIPn_        bigint
    //
    //1 return ( @1 )
    //@1 ; OK int
    //0         ; 성공
    //-1        ; 유저 정보가 없음
    //-101 이하 ; 트랜잭션 에러

    _DBJIF( m_kODBCMain.Query( L"{ call dbo.UTGA_user_trace_insert ( %d, N'%s', N'%s', %I64d, %d ) }",
         dwUID_,
         (LPCWSTR)CTime(kPacket_.m_tmLogin).Format( KNC_TIME_FORMAT ),
         (LPCWSTR)CTime(kPacket_.m_tmLogout).Format( KNC_TIME_FORMAT ),
         (__int64)kPacket_.m_uiIP,
         kPacket_.m_dwChannelType ),
         return, (&m_kODBCMain) );

    int nOK = -1;
    if( m_kODBCMain.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBCMain >> nOK );
    }
    m_kODBCMain.EndFetch();

    _LOG_SUCCESS( nOK == 0, L"Netmarble Logout Log : " << nOK )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket_.m_uiIP )
        << BUILD_LOGtm( CTime(kPacket_.m_tmLogin) )
        << BUILD_LOGtm( CTime(kPacket_.m_tmLogout) )
        << BUILD_LOG( m_kODBCMain.m_strLastQuery ) << END_LOG;


}

_INT_IMPL_ON_FUNC( EMS_ADD_GUILD_MEMBER_LIST_REQ, KUpdateGuildMemberReq )
{
    KUpdateGuildMemberAck kPacket;
    kPacket.m_mapFriendGroup[KGroup::GUILD_BASE].m_nGroupKey = KGroup::GUILD_BASE;

    std::set< DWORD >::iterator sit;
    for ( sit = kPacket_.m_setMemberUID.begin() ; sit != kPacket_.m_setMemberUID.end() ; ++sit ) {
        KFriend kFriend;
        UpdateJoinGuildMemberInfo( kPacket_.m_GuildUID, *sit, kFriend );

        if ( kFriend.m_nGuildUserLevel <= KNGuildUserInfo::GL_JOINER ) {
            continue;
        }
        
        kPacket.m_mapFriendGroup[KGroup::GUILD_BASE].m_mapMembers.insert( std::make_pair( kFriend.m_dwUserUID, kFriend ) );
    }
    
    _QUEUING_ACK_TO_USER( EMS_ADD_GUILD_MEMBER_LIST_ACK, kPacket );
}

INT_IMPL_ON_FUNC( EMS_GUILD_ALL_MEMBER_LIST_REQ )
{
    static unsigned int MAX_GUILD_USER_NUM = 200;
    KEMS_GUILD_ALL_MEMBER_LIST_ACK kPacket;

    if ( kPacket_.m_GuildUID > 0 ) {
        _DBJIF( m_kODBCMain.Query( L"{ call dbo.GSGAGuildSystemMember_select_1_guild ( %d ) }", kPacket_.m_GuildUID ), return, ( &m_kODBCMain ) );

        std::wstring strSP;
        std::set<DWORD> setUserList;

        while( m_kODBCMain.Fetch() != SQL_NO_DATA )
        {
            DWORD dwUserUID = 0;
            VERIFY_QUERY( m_kODBCMain>> strSP
                >> dwUserUID );

            if ( dwUserUID > 0 ) {
                setUserList.insert( dwUserUID );
            }
        }

        if( !setUserList.empty() ) {
            std::set<DWORD>::iterator sit;
            for( sit = setUserList.begin(); sit != setUserList.end(); ++sit ) {
                KNGuildUserInfo kTemp;
                time_t tmJoinDate;
                time_t tmLastLogin;
                KFriend kFriend;

                // 길드멤버의 UserUID리스트를 가져와서 해당 유저 UID를 통해서 유저길드정보를 가져온다.
                // DB에서 기존의 방식의 경우 랜덤 액세스가 일어나서 성능상의 이슈로 인해서 수정.
                GetGuildMemeberInfoFromDB( kPacket_.m_GuildUID, *sit, kTemp, tmJoinDate, tmLastLogin );

                if ( kTemp.m_cMemberLevel <= KNGuildUserInfo::GL_JOINER ) {
                    continue;
                }

                kFriend.m_dwUserUID = kTemp.m_dwUserUID;
                kFriend.m_strNickColor = kTemp.m_strNickColor;
                kFriend.m_strNickName = kTemp.m_strNickName;
                kFriend.m_nGuildUserLevel = kTemp.m_cMemberLevel;
                kFriend.m_bIsFriend = true;
                kPacket.m_mapFriendGroup[KGroup::GUILD_BASE].m_nGroupKey = KGroup::GUILD_BASE;
                kPacket.m_mapFriendGroup[KGroup::GUILD_BASE].m_mapMembers.insert( std::make_pair( kFriend.m_dwUserUID, kFriend ) );

                START_LOG( clog, L"길드원 정보. GuildUID : " << kTemp.m_dwGuildUID )
                    << BUILD_LOG( kFriend.m_strNickName )
                    << BUILD_LOG( kFriend.m_nGuildUserLevel ) << END_LOG;

                if ( kPacket.m_mapFriendGroup[KGroup::GUILD_BASE].m_mapMembers.size() > MAX_GUILD_USER_NUM ) {
                    break;
                }
            }
        }
        m_kODBCMain.EndFetch();

        START_LOG( clog, L"길드 유저 리스트 사이즈 : " << kPacket.m_mapFriendGroup[KGroup::GUILD_BASE].m_mapMembers.size() )
            << BUILD_LOG( strSP ) << END_LOG;
    }

    QUEUING_ACK_TO_USER( EMS_GUILD_ALL_MEMBER_LIST_ACK );
}

bool KMSDBThread::UpdateJoinGuildMemberInfo( IN const DWORD dwGuildUID_, IN const DWORD dwUserUID_, OUT KFriend& kInfo_ )
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
    time_t tmJoinDate;
    time_t tmLastLogin;

    __JIF( dwGuildUID_ > 0, return false );
    __JIF( dwUserUID_ > 0, return false );

    DBJIF2( m_kODBCMain.Query( L"{ call dbo.GSGAGuildSystemMember_select_1_member ( %d, %d ) }",
        dwGuildUID_, dwUserUID_ ) );

    KNGuildUserInfo kTemp;
    kTemp.Init();

    if( m_kODBCMain.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBCMain >> strSP
            >> kTemp.m_dwGuildUID
            >> kInfo_.m_dwUserUID
            >> tmJoinDate
            >> kInfo_.m_nGuildUserLevel
            >> kTemp.m_strMyComment
            >> kTemp.m_dwContributePoint
            >> kTemp.m_strLogin
            >> kInfo_.m_strNickName
            >> tmLastLogin );
    }
    m_kODBCMain.EndFetch();

    kInfo_.m_bIsFriend = true;

    START_LOG( clog, L"strSP : " << strSP )
        << BUILD_LOG( dwGuildUID_ )
        << BUILD_LOG( kInfo_.m_dwUserUID )
        << BUILD_LOGc( kInfo_.m_nGuildUserLevel )
        << BUILD_LOG( kTemp.m_strMyComment )
        << BUILD_LOG( kTemp.m_dwContributePoint )
        << BUILD_LOG( kTemp.m_strLogin )
        << BUILD_LOG( kInfo_.m_strNickName ) << END_LOG;
    return true;
}

void KMSDBThread::GetGuildMemeberInfoFromDB( IN DWORD dwGuildUID_, IN DWORD dwUserUID_, OUT KNGuildUserInfo& kGuildUserInfo_, OUT time_t& tmJoinDate_, OUT time_t& tmLastLogin_ )
{
    std::wstring strSP;
    time_t tmJoinDate;
    time_t tmLastLogin;

    __JIF( dwGuildUID_ > 0, return );
    __JIF( dwUserUID_ > 0, return );

    DBJIF( m_kODBCMain.Query( L"{ call dbo.GSGAGuildSystemMember_select_1_member_color ( %d, %d ) }",
        dwGuildUID_, dwUserUID_ ) );

    KNGuildUserInfo kTemp;
    kTemp.Init();

    if( m_kODBCMain.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBCMain >> strSP
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
    }
    m_kODBCMain.EndFetch();

    kGuildUserInfo_ = kTemp;
    tmJoinDate_ = tmJoinDate;
    tmLastLogin_ = tmLastLogin;

    START_LOG( clog, L"strSP : " << strSP )
        << BUILD_LOG( dwGuildUID_ )
        << BUILD_LOG( kTemp.m_dwUserUID )
        << BUILD_LOGc( kTemp.m_cMemberLevel )
        << BUILD_LOG( kTemp.m_strMyComment )
        << BUILD_LOG( kTemp.m_dwContributePoint )
        << BUILD_LOG( kTemp.m_strLogin )
        << BUILD_LOG( kTemp.m_strNickName )
        << BUILD_LOG( kTemp.m_strNickColor )<< END_LOG;
}
