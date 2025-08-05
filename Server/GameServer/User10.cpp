#include "User.h"
#include <dbg/dbg.hpp>
#include "UserFSM.h"
#include "support_User.h"
#include "Channel.h"
#include "Log4.h"
#include "CnConnector.h"
#include "MatchConnector.h"
#include "boost/bind.hpp"
#include "GSSimLayer.h"
#include "PVPMatchManager.h"
#include "RankManager.h"
#include "ErrandManager.h"
#include "SubjectManager.h"
#include <boost/foreach.hpp>
#include "SlangFilter.h"
#include "ItemManager.h"
#include "GameServer.h"
#include "TongDonation.h"
#include "SocialCommerce.h"
#include "ItemManager.h"
#include "SkillManager.h"
#include "HeroDungeonManager.h"

struct KDropItemInfo;

#define CLASS_TYPE  KUser

IMPL_ON_FUNC_NOPARAM( EVENT_MATCH_INVITE_FRIEND_LIST_REQ )
{
    VERIFY_STATE( ( 1, KUserFSM::STATE_CHANNEL ) ); // 채널에서만 호출가능.
    JIF( m_pkChannel );

    KEVENT_MATCH_INVITE_FRIEND_LIST_ACK kPacket;
    kPacket.m_nOK = -99;
    std::vector<KMatchInviteUserInfo> vecMatchInviteList;
    char cChannelType = m_pkChannel->GetType(); // 로그를 위해서 현재 채널정보 가져오기.

    if( !m_pkChannel->IsPvPChannel() ) {
        START_LOG( cwarn, L"현재 채널에서는 초대가 불가능..." )
            << BUILD_LOG( m_strName )
            << BUILD_LOGc( cChannelType )
            << END_LOG;

        SEND_PACKET( EVENT_MATCH_INVITE_FRIEND_LIST_ACK );
        return;
    }

    kPacket.m_nOK = 0; // 초대가능 채널일 경우 ok값 무조건 0으로 세팅.
    if( ::GetTickCount() - GetTick( MATCH_INVITELIST_TICK ) > 10000 ) { //10초
        GetMatchInviteUserList( vecMatchInviteList );
        SetTick( MATCH_INVITELIST_TICK );
        kPacket.m_vecMatchInviteList = vecMatchInviteList;
    } else {
        kPacket.m_vecMatchInviteList = m_vecMatchPartyInviteList;
    }

    SEND_PACKET( EVENT_MATCH_INVITE_FRIEND_LIST_ACK );

    START_LOG( clog, L"매칭 초대 가능한 유저리스트 전송.. Name : " << m_strName ) 
        << BUILD_LOG( kPacket.m_vecMatchInviteList.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_PARTY_USER_READY_REQ )
{
    // 채널에서만 호출가능. 클라이언트가 채널 대기 상태이어야 한다.
    VERIFY_STATE( ( 1, KUserFSM::STATE_CHANNEL ) );
    JIF( m_pkChannel );

    KPartyData kData;
    KPartyMember kMember;
    kMember.m_dwUID = GetUID();
    kMember.m_dwState = kPacket_.m_dwState;
    kMember.m_cCharType = kPacket_.m_cCharType;

    KCharacterInfo* pkCharacterInfo = GetCharacterInfo( kPacket_.m_cCharType );
    if( pkCharacterInfo ) {
        kMember.m_nRP = pkCharacterInfo->m_kELOUserData.m_nRatingPoint;
    } else {
        START_LOG( cerr, L"존재하지 않는 캐릭터를 선택해서 상태변경 요청." )
            << BUILD_LOG( m_strName )
            << BUILD_LOG( kPacket_.m_dwState )
            << BUILD_LOGc( kPacket_.m_cCharType )
            << END_LOG;
        kData.m_nOK = -1;
        SEND_DATA( EVENT_PARTY_USER_READY_ACK, kData );
        return;
    }

    kData.m_dwPartyUID = m_kParty.m_dwPartyUID;
    kData.m_vecPartyPeople.push_back( kMember );

    SiKCnConnector()->SendPacket( KCenterEvent::ECN_PARTY_USER_READY_REQ, kData );

    START_LOG( clog, L"파티 멤버 상태 변경 요청" )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kData.m_dwPartyUID )
        << BUILD_LOG( kPacket_.m_dwState )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << END_LOG;
}

_IMPL_ON_FUNC( EVENT_PARTY_USER_READY_BROAD, KPartyData )
{
    m_kParty = kPacket_;
    SEND_RECEIVED_PACKET( EVENT_PARTY_USER_READY_BROAD );
}

IMPL_ON_FUNC( EVENT_ADD_MATCH_REQ )
{
    // 채널에서만 호출가능. 클라이언트가 채널 대기 상태이어야 한다.
    VERIFY_STATE( ( 1, KUserFSM::STATE_CHANNEL ) );
    JIF( m_pkChannel );
    SET_ERROR( ERR_UNKNOWN );
    std::vector<KPartyMember>::iterator vit;
    KEVENT_MATCH_ADD_USER_REQ kMatchPacket;
    KEVENT_ADD_MATCH_ACK kPacket;
    kPacket.m_nOK = -99;
    int nMatchSID = 0;

    if( !SiKPVPMatchManager()->CheckModeOpenEnable( kPacket_.m_nModeInfo ) ) {
        SET_ERR_GOTO( ERR_MATCH_ERROR_10, END_PROC );
    }

    if( !SiKPVPMatchManager()->AddMatchEnable() ) {
        SET_ERR_GOTO( ERR_MATCH_ERROR_09, END_PROC );
    }

    if( !IsJoinParty() ) { // 파티가 없을 경우. 개인 매칭 요청시..,
        if( !SetSingleParty( kPacket_.m_nModeInfo, kPacket_.m_cCharType, kPacket_.m_nExceptModeID ) ) {
            SET_ERR_GOTO( ERR_MATCH_ERROR_02, END_PROC );
        }
    } else {
        vit = std::find_if( m_kParty.m_vecPartyPeople.begin(), m_kParty.m_vecPartyPeople.end(), boost::bind( &KPartyMember::m_dwUID, _1 ) == GetUID() );
        if( vit == m_kParty.m_vecPartyPeople.end() ) {
            SET_ERR_GOTO( ERR_MATCH_ERROR_03, END_PROC );
        }

        if( !vit->m_IsHost ) { // 파장이 아닌데 매칭 요청.
            SET_ERR_GOTO( ERR_MATCH_ERROR_06, END_PROC );
        }

        if( !CheckPartyMemberState( m_kParty.m_vecPartyPeople ) ) {
            SET_ERR_GOTO( ERR_MATCH_ERROR_07, END_PROC );
        }

        vit->m_cCharType = kPacket_.m_cCharType;
        m_kParty.m_nModeInfo = kPacket_.m_nModeInfo;
        m_kParty.m_nExceptModeID = kPacket_.m_nExceptModeID;
        m_kParty.m_dwServerUID = vit->m_ServerUID;
        m_kParty.m_dwMatchPartyUID = GetUID(); // 매칭 파티 UID는 파장의 UID를 사용한다.
    }

    if( !SiKMatchConnector()->GetFirstSID( nMatchSID ) ) { // 첫번째 매칭 서버 SID 가져오기.
        SET_ERR_GOTO( ERR_MATCH_ERROR_04, END_PROC );
    }

    if( !SetPartyMemberRP( m_kParty.m_vecPartyPeople ) ) {
        SET_ERR_GOTO( ERR_MATCH_ERROR_08, END_PROC );
    }

    // 계산된 해당 파티의 RP정보 가져오기.
    if( !GetPartyRP( m_kParty.m_vecPartyPeople, m_kParty.m_nRP ) ) {
        SET_ERR_GOTO( ERR_MATCH_ERROR_08, END_PROC );
    }

    kMatchPacket.m_kPartyData = m_kParty;
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_MATCH_ERROR_02,   kPacket.m_nOK, 2 );     // 존재하지 않는 캐릭터로 매칭 요청.
        ERR_CASE( ERR_MATCH_ERROR_03,   kPacket.m_nOK, 3 );     // 파티에 방장이 멤버리스트에 없음.
        ERR_CASE( ERR_MATCH_ERROR_04,   kPacket.m_nOK, 4 );     // 연결된 매칭 서버가 없음.
        ERR_CASE( ERR_MATCH_ERROR_06,   kPacket.m_nOK, 6 );     // 파티장이 아닌데 매칭 요청을 했음.
        ERR_CASE( ERR_MATCH_ERROR_07,   kPacket.m_nOK, 7 );     // 파티원이 전부 Ready상태가 아닌데 매칭 요청을 했음.
        ERR_CASE( ERR_MATCH_ERROR_08,   kPacket.m_nOK, 8 );     // 파티원 정보가 없음.
        ERR_CASE( ERR_MATCH_ERROR_09,   kPacket.m_nOK, 9 );     // 현재 매칭 요청가능한 시간이 아니다.
        ERR_CASE( ERR_MATCH_ERROR_10,   kPacket.m_nOK, 10 );    // 해당 모드는 닫힌 모드이다.
    default:
        START_LOG( cerr, L"매칭 추가 요청중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    if( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_ADD_MATCH_ACK );
    }
    else {
        // 매칭 서버에 매칭 등록 요청.
        SEND_DATA_TO_MATCHSERVER( nMatchSID, EVENT_MATCH_ADD_USER_REQ, kMatchPacket );
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"매칭 추가 요청 MSG :" << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nModeInfo )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_ADD_MATCH_ACK )
{
    SetMatchStartTick(); // 유저 매칭 시작 시간 기억.
    SEND_RECEIVED_PACKET( EVENT_ADD_MATCH_ACK );
}

IMPL_ON_FUNC( EVENT_ADD_MATCH_BROAD )
{
    // 파티정보 갱신. 방장의 정보가 갱신되어서 넘어오기 때문.
    SetMatchStartTick(); // 유저 매칭 시작 시간 기억.
    m_kParty = kPacket_.m_kPartyData;
    SEND_RECEIVED_PACKET( EVENT_ADD_MATCH_BROAD );
}

IMPL_ON_FUNC_NOPARAM( EVENT_DEL_MATCH_REQ )
{
    int kPacket = -99;
    int nMatchSID = 0;
    KPartyData kPartyData;
    SET_ERROR( ERR_UNKNOWN );
    // 파티가 없을 경우.
    // 이 경우는 말이 안되는데 클라이언트에서 가진 파티 정보를 초기화 한다.
    // 이미 다른 Case를 통해서 초기화되었음.
    if( !IsJoinParty() ) {
        SET_ERR_GOTO( ERR_MATCH_ERROR_05, END_PROC );
    }

    if( !SiKMatchConnector()->GetFirstSID( nMatchSID ) ) { // 첫번째 매칭 서버 SID 가져오기.
        SET_ERR_GOTO( ERR_MATCH_ERROR_04, END_PROC );
    }

    kPartyData = m_kParty;
    SetMatchEndTick(); // 매칭 취소 시간 기록.
    DWORD dwDurationTime = GetMatchDurationTime();
    CheckUserDelMatchTime( dwDurationTime, m_kParty.m_nModeInfo ); // 유저 매칭 대기 시간 체크해서 DB넘길지 결정.
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket, 0 );     // 성공
        ERR_CASE( ERR_MATCH_ERROR_04,   kPacket, 4 );     // 연결된 매칭 서버가 없음.
        ERR_CASE( ERR_MATCH_ERROR_05,   kPacket, 5 );     // 현재 파티가 없음.
    default:
        START_LOG( cerr, L"매칭 취소 요청중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    if( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_DEL_MATCH_ACK );
    }
    else {
        // 매칭 서버에 매칭 삭제 요청.
        SEND_DATA_TO_MATCHSERVER( nMatchSID, EVENT_MATCH_DEL_USER_REQ, kPartyData );
    }

    _LOG_SUCCESS( kPacket == 0, L"매칭 취소 요청 MSG :" << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << END_LOG;
}

_IMPL_ON_FUNC( EVENT_DEL_MATCH_BROAD, int )
{
    ExitParty();
    InitMatchTick(); // 매칭 시작&대기 시간 초기화.
    SEND_RECEIVED_PACKET( EVENT_DEL_MATCH_BROAD );
}

IMPL_ON_FUNC_NOPARAM( EVENT_CURRENT_MATCH_MODE_COUNT_REQ )
{
    KEVENT_CURRENT_MATCH_MODE_COUNT_ACK kPacket;
    KEVENT_CURRENT_MATCH_MODE_COUNT_BROAD kBroadPacket;
    kPacket.m_nOK = -99;
    int nMatchModeCount = 0;
    bool bLimitMatchEnable = false;
    int nLimitMatchGroupNum = 0;
    std::vector<KPartyMember>::iterator vit;
    SET_ERROR( ERR_UNKNOWN );
    // 파티가 없을 경우.
    // 이 경우는 말이 안되는데 클라이언트에서 가진 파티 정보를 초기화 한다.
    // 이미 다른 Case를 통해서 초기화되었음.
    if( !IsJoinParty() ) {
        SET_ERR_GOTO( ERR_MATCH_ERROR_05, END_PROC );
    }

    nMatchModeCount = SiKPVPMatchManager()->GetModeCount( m_kParty.m_nModeInfo );
    SiKPVPMatchManager()->GetLimitMatchData( bLimitMatchEnable, nLimitMatchGroupNum );
    kPacket.m_nModeInfo = m_kParty.m_nModeInfo;
    kPacket.m_nMatchModeCount = nMatchModeCount;
    kPacket.m_bLimitMatchEnable = bLimitMatchEnable;
    kPacket.m_nLimitMatchGroupNum = nLimitMatchGroupNum;

    kBroadPacket.m_nModeInfo = kPacket.m_nModeInfo;
    kBroadPacket.m_nMatchModeCount = kPacket.m_nMatchModeCount;
    kBroadPacket.m_bLimitMatchEnable = bLimitMatchEnable;
    kBroadPacket.m_nLimitMatchGroupNum = nLimitMatchGroupNum;

    // 방원들에게 ModeCount정보 Broadcast.
    for( vit = m_kParty.m_vecPartyPeople.begin(); vit != m_kParty.m_vecPartyPeople.end(); ++vit ) {
        if( vit->m_dwUID == GetUID() ) {
            continue;
        }
        SiKGSSimLayer()->m_kActorManager.QueueingEventTo( vit->m_dwUID, KUserEvent::EVENT_CURRENT_MATCH_MODE_COUNT_BROAD, kBroadPacket );
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_MATCH_ERROR_05,   kPacket.m_nOK, 5 );     // 현재 파티가 없음.
    default:
        START_LOG( cerr, L"매칭모드 대기인원 요청중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    SEND_PACKET( EVENT_CURRENT_MATCH_MODE_COUNT_ACK );

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"매칭모드 대기인원 요청중 요청 MSG :" << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nModeInfo )
        << BUILD_LOG( kPacket.m_nMatchModeCount )
        << BUILD_LOG( kPacket.m_bLimitMatchEnable )
        << BUILD_LOG( kPacket.m_nLimitMatchGroupNum )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_CURRENT_MATCH_MODE_COUNT_BROAD )
{
    SEND_RECEIVED_PACKET( EVENT_CURRENT_MATCH_MODE_COUNT_BROAD );
}

_IMPL_ON_FUNC( EVENT_DISCONNECT_MATCHSERVER_NOT, int )
{
    if( GetStateID() == KUserFSM::STATE_CHANNEL && IsMatchParty() ) { // 채널에서 대기중인 유저들에게만 알려준다.
        ExitParty();
        InitMatchTick(); // 매칭 시작&대기 시간 초기화.
        SEND_RECEIVED_PACKET( EVENT_DISCONNECT_MATCHSERVER_NOT );
    }
}

IMPL_ON_FUNC( EVENT_FIND_MATCH_NOT )
{
    if( kPacket_.m_nOK == 0 ) { // 대전 매칭 파티 그룹 저장.
        m_mapMatchParty = kPacket_.m_mapPartyMember;
        m_nMatchSuccessLoopCount = kPacket_.m_nLoopCount;
        SetMatchEndTick();
    }
    SEND_RECEIVED_PACKET( EVENT_FIND_MATCH_NOT );
}

IMPL_ON_FUNC( EVENT_ACCEPT_MATCH_REQ )
{
    KEVENT_ACCEPT_MATCH_BROAD kBroadPacket;
    kBroadPacket.m_nOK = -99;
    kBroadPacket.m_dwAcceptUserUID = GetUID();
    DWORD dwHostUID = 0;

    if( m_mapMatchParty.empty() ) { // 다른 유저가 매칭 취소를 하고 나서 패킷이 오는 경우에 대한 처리.
        START_LOG( clog, L"이미 다른 누군가가 매칭 거절을 했음" )
            << BUILD_LOG( GetUID() )
            << BUILD_LOG( kPacket_.m_bAccept )
            << END_LOG;
        return;
    }

    if( true == kPacket_.m_bAccept ) { // 게임 참여 수락일 경우..,
        kBroadPacket.m_nOK = 0;
        ChangeUserFindMatchState( GetUID() );
        kBroadPacket.m_mapPartyMember = m_mapMatchParty;
        SetMatchIndoorUserInfo( kPacket_.m_kIndoorUserInfo ); // 매칭을 위한 룸 유저 정보 세팅.
        // 현재 파티원들이 전부 게임서버에 있는지에 대한 체크 필요.
        std::map<int, std::vector<KPartyMember>>::iterator mit;
        std::vector<KPartyMember>::iterator vit;
        for( mit = kBroadPacket.m_mapPartyMember.begin(); mit != kBroadPacket.m_mapPartyMember.end(); ++mit ) {
            for( vit = mit->second.begin(); vit != mit->second.end(); ++vit ) {
                if( vit->m_dwUID == GetUID() ) { // 자신은 제외시킨다.
                    continue;
                }
                KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vit->m_dwUID ) );
                if( !spUser ) { // 파티원이 한명이라도 없으면 매칭 취소 처리한다.
                    kBroadPacket.m_nOK = -99;
                }
            }
        }
    }
    else {
        kBroadPacket.m_mapPartyMember = m_mapMatchParty;
    }

    if( false == kPacket_.m_bAccept ) { 
        DWORD dwDurationTime = GetMatchDurationTime();
        SendUserMatchRefuseInfo( dwDurationTime, m_kParty.m_nModeInfo );
    }

    // 방원들에게 정보 Broadcasting.
    std::map<int, std::vector<KPartyMember>> mapMatchParty;
    std::map<int, std::vector<KPartyMember>>::iterator mit;
    std::vector<KPartyMember>::iterator vit;
    for( mit = kBroadPacket.m_mapPartyMember.begin(); mit != kBroadPacket.m_mapPartyMember.end(); ++mit ) {
        for( vit = mit->second.begin(); vit != mit->second.end(); ++vit ) {
            KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vit->m_dwUID ) );
            if( !spUser ) {
                continue;
            }
            if( vit->m_dwUID != GetUID() ) {
                spUser->m_mapMatchParty = kBroadPacket.m_mapPartyMember;
            }
            spUser->QueueingEvent( KUserEvent::EVENT_ACCEPT_MATCH_BROAD, kBroadPacket );
        }
    }

    START_LOG( clog, L"매칭 찾은후에 유저의 수락 거절 정보" )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( kPacket_.m_bAccept )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_ACCEPT_MATCH_BROAD )
{
    KEVENT_CREATE_ROOM_REQ kCreateRoomPacket;
    DWORD dwHostUID = 0;
    if( kPacket_.m_nOK != 0 ) { // 성공이 아니면..,
        InitMatchTick();
        m_mapMatchParty.clear();
        ExitParty();
    } else {
        // 현재 모든 유저가 수락상태인지 체크.
        if( CheckMatchPartyState( dwHostUID ) ) {
            if( dwHostUID == GetUID() && IsMatchRoomEnable() ) { // 매칭 파티 방장일 경우.
                SetMatchRoomEnable(true);
                SetMatchRoomInfo( kCreateRoomPacket.m_kRoomInfo );
                GetMatchMyTeamType( kCreateRoomPacket.m_nTeamType ); 
                kCreateRoomPacket.m_nModeInfo = m_kParty.m_nModeInfo; // 방장 모드정보 넣어주기.
                GetMatchIndoorUserInfo( kCreateRoomPacket.m_kInDoorUserInfo ); // 방장 룸 유저정보 넣어주기.
                QueueingEvent( KUserEvent::EVENT_CREATE_ROOM_REQ, kCreateRoomPacket );
                
            }
        }
    }
    SEND_RECEIVED_PACKET( EVENT_ACCEPT_MATCH_BROAD );
}

IMPL_ON_FUNC_NOPARAM( EVENT_START_GAME_POSSIBLE_REQ )
{
    VERIFY_STATE(( 1, KUserFSM::STATE_ROOM ));

    int kPacket = -99;
    JIF( m_pkChannel );
    JIF( m_pkRoom );

    if( m_pkRoom->CheckStartGamePossible() ) {
        kPacket = 0;
    }

    SEND_PACKET( EVENT_START_GAME_POSSIBLE_ACK );
}

_IMPL_ON_FUNC( EVENT_START_GAME_FAIL_NOT, DWORD )
{
    SEND_ID( EVENT_START_GAME_FAIL_NOT );
}

IMPL_ON_FUNC_NOPARAM( EVENT_MATCH_MODE_STATE_REQ )
{
    KEVENT_MATCH_MODE_STATE_ACK kPacket;
    SiKPVPMatchManager()->GetModeInfo( kPacket );

    SEND_PACKET( EVENT_MATCH_MODE_STATE_ACK );

    START_LOG( clog, L"매칭 모드 정보 전달" )
        << BUILD_LOG( kPacket.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_MATCH_RANK_PAGE_REQ )
{
    VERIFY_STATE( (2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_CHANNEL ) );
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_MATCH_RANK_PAGE_ACK kPacket;
    KListPage kPage;

    if( !SiKRankManager()->GetMatchRankPageData( kPacket_.m_cRankType, kPacket_.m_cCharType, kPacket_.m_cPageNum, kPacket ) ) {
        goto END_PROC;
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,       kPacket.m_nOK, 0 ); // 
        ERR_CASE( ERR_RANK_00,  kPacket.m_nOK, 1 ); // 해당 랭킹 타입이 존재하지 않음.
        ERR_CASE( ERR_RANK_01,  kPacket.m_nOK, 2 ); // 해당 캐릭터 타입이 존재하지 않음.
        ERR_CASE( ERR_RANK_02,  kPacket.m_nOK, 3 ); // 해당 페이지가 존재하지 않음.
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"랭킹 정보 요청시 알수 없는 오류,, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    SEND_PACKET( EVENT_MATCH_RANK_PAGE_ACK );

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn )
        << BUILD_LOG( m_strName )
        << BUILD_LOGc( kPacket_.m_cRankType )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOGc( kPacket_.m_cPageNum )
        << BUILD_LOGc( kPacket.m_cTotalPageCount )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_MY_MATCH_RANK_INFO_NOT, KMyMatchRankInfo )
{
    m_kMatchRankInfo = kPacket_;
    SEND_RECEIVED_PACKET( EVENT_MY_MATCH_RANK_INFO_NOT );
}

IMPL_ON_FUNC( EVENT_MATCH_RANK_SEARCH_REQ )
{
    VERIFY_STATE( (2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_CHANNEL ) );
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_MATCH_RANK_SEARCH_ACK kPacket;
    kPacket.m_nOK = -99;

    // 시간 체크.
    if( ::GetTickCount() - GetTick( RANK_SEARCH_TICK ) < KRankManager::ms_uiRankSearchGap ) {
        SET_ERR_GOTO( ERR_RANK_09, END_PROC );
    }

    if( !SiKRankManager()->CheckRankType( kPacket_.m_cRankType ) ) {
        SET_ERR_GOTO( ERR_RANK_05, END_PROC );
    }

    // 캐릭터 타입 체크
    if( kPacket_.m_cCharType < GC_CHAR_INVALID || kPacket_.m_cCharType >= GC_CHAR_NUM ) {
        SET_ERR_GOTO( ERR_RANK_06, END_PROC );
    }

    // 닉네임 길이 체크
    if( kPacket_.m_strNickName.empty() ) {
        SET_ERR_GOTO( ERR_RANK_03, END_PROC );
    }

    // 쿼리에 사용할수 없는 문자 포함 되어 있는지 체크
    if( SiKSlangFilter()->IsInQueryInvalidChar( kPacket_.m_strNickName ) ) {
        SET_ERR_GOTO( ERR_RANK_04, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_MATCH_RANK_SEARCH_REQ, kPacket_ );
    SetTick( KUser::RANK_SEARCH_TICK );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,       kPacket.m_nOK, 0 );
        ERR_CASE( ERR_RANK_03,  kPacket.m_nOK, 1 ); // 랭킹 조사시 닉네임이 비어 있음. 
        ERR_CASE( ERR_RANK_04,  kPacket.m_nOK, 2 ); // 랭킹 조사시 닉네임에 사용할수 없는 문자 포함. 
        ERR_CASE( ERR_RANK_05,  kPacket.m_nOK, 3 ); // 랭킹 조사시 없는 랭킹 타입 입니다. 
        ERR_CASE( ERR_RANK_06,  kPacket.m_nOK, 4 ); // 랭킹 조사시 없는 캐릭터 타입 입니다. 
        ERR_CASE( ERR_RANK_08,  kPacket.m_nOK, 6 ); // 랭킹 정보가 없음. 
        ERR_CASE( ERR_RANK_09,  kPacket.m_nOK, 7 ); // 랭킹 검색 주기가 짧음. 
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );

    default:
        START_LOG( cerr, L"유저 랭킹 검색시 알수 없는 오류 발생, Name : " << m_strName )
            << BUILD_LOGc( kPacket_.m_cRankType )
            << BUILD_LOGc( kPacket_.m_cCharType )
            << BUILD_LOG( kPacket_.m_strNickName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_MATCH_RANK_SEARCH_ACK );
    }

    LOG_CONDITION( NetError::GetLastNetError() == NetError::NET_OK || NetError::GetLastNetError() == NetError::ERR_NOT_FINISHED_YET, clog, cwarn )
        << L"유저 랭킹 검색요청 : " << (kPacket.m_nOK == 0? L"성공":L"실패" ) << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOGc( kPacket_.m_cRankType )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_strNickName )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_MATCH_RANK_SEARCH_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,       kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_RANK_03,  kPacket_.m_nOK, 1 ); // 랭킹 조사시 닉네임이 비어 있음. 
        ERR_CASE( ERR_RANK_04,  kPacket_.m_nOK, 2 ); // 랭킹 조사시 닉네임에 사용할수 없는 문자 포함. 
        ERR_CASE( ERR_RANK_05,  kPacket_.m_nOK, 3 ); // 랭킹 조사시 없는 랭킹 타입 입니다. 
        ERR_CASE( ERR_RANK_06,  kPacket_.m_nOK, 4 ); // 랭킹 조사시 없는 캐릭터 타입 입니다. 
        ERR_CASE( ERR_RANK_07,  kPacket_.m_nOK, 5 ); // 랭킹 조사시 자기 자신을 조사하려 했음. 
        ERR_CASE( ERR_RANK_08,  kPacket_.m_nOK, 6 ); // 랭킹 정보가 없음. 

    default:
        START_LOG( cerr, L"유저 랭킹 검색시 알수 없는 오류 발생, Name : " << m_strName )
            << BUILD_LOG( kPacket_.m_nOK )
            << BUILD_LOGc( kPacket_.m_cRankType )
            << BUILD_LOGc( kPacket_.m_cCharType )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_MATCH_RANK_SEARCH_ACK );

    LOG_CONDITION( kPacket_.m_nOK == 0 , clog, cwarn )
        << L"유저 랭킹 검색 결과 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOGc( kPacket_.m_cRankType )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_kExpRank.m_nRank )
        << BUILD_LOG( kPacket_.m_kMatchRank.m_nRank ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_MY_MATCH_RANK_INFO_REQ )
{
    std::set<char> kPacket;
    std::map<char, KCharacterInfo>::const_iterator cmit;
    for ( cmit = m_mapCharacterInfo.begin() ; cmit != m_mapCharacterInfo.end() ; ++cmit ) {
        kPacket.insert( cmit->first );
    }

    QUEUEING_EVENT_TO_DB( DB_EVENT_MY_MATCH_RANK_INFO_REQ, kPacket );
}

IMPL_ON_FUNC( EVENT_DUNGEON_SUBJECT_REWARD_REQ )
{
    _VERIFY_STATE( ( 1, KUserFSM::STATE_PLAYING ) );

    JIF( m_pkRoom );

    bool bGetItemReward = false;
    KEVENT_DUNGEON_SUBJECT_REWARD_ACK kPacket;
    kPacket.m_nOK = 99;
    kPacket.m_dwLoginUID = GetUID();
    kPacket.m_dwTriggerID = kPacket_.m_dwTriggerID;
    kPacket.m_dwSubjectID = kPacket_.m_dwSubjectID;
    kPacket.m_dwGainExp = 0;
    kPacket.m_dwGainGp = 0;
    kPacket.m_vecRewards.clear();

    SET_ERROR( ERR_UNKNOWN );

    PAIR_INT prModeAndDifficulty( m_pkRoom->m_iGameMode, m_pkRoom->m_nDifficulty );
    KSubjectManager::KSubject kSubject;

    // 운영자는 클라이언트 치트키 사용을 위해 트리거 ID를 체크하지 않고 과제의 ID만 체크한다
    if ( true == IsAdmin() ) {
        // (모드, 난이도)에 설정된 과제정보를 가져온다
        if ( false == SiKSubjectManager()->GetSubject( kPacket.m_dwSubjectID, kSubject ) ) {
            START_LOG( cerr, L"설정되지 않은 과제 요청. LoginID: " << GetName() << ", SubjectID: " << kPacket_.m_dwSubjectID ) << END_LOG;
            return;
        }
    }
    else {
        // 룸에서 이미 달성한 과제의 트리거인지 여부를 확인한다(트리거ID에 대해서만 확인한다)
        if ( true == m_pkRoom->IsCompletedSubjectTrigger( GetUID(), kPacket.m_dwTriggerID ) ) {
            // 이미 완료한 트리거임
            START_LOG( cerr, L"이미 완료한 트리거입니다. LoginID: " << GetName() << ", TriggerID: " << kPacket_.m_dwTriggerID ) << END_LOG;
            return;
        }

        // (모드, 난이도)에 설정된 과제정보를 가져온다
        if ( false == SiKSubjectManager()->GetSubject( prModeAndDifficulty, kPacket.m_dwTriggerID, kPacket.m_dwSubjectID, kSubject ) ) {
            return;
        }
    }

    // 서버에 유저 정보가 있는지 확인한다
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID< KUser >( GetUID() ) );
    if ( NULL == spUser ) {
        START_LOG( cerr, L"서버에 유저 정보가 없음. UserUID: " << GetUID() ) << END_LOG;
        return;
    }

    // 룸에 유저 정보가 있는지 확인한다
    MAP_CHAR_JOB mapPromotionInfo;
    m_pkRoom->GetPromotionInfo( mapPromotionInfo );
    MAP_CHAR_JOB::iterator mitCharJob;
    mitCharJob = mapPromotionInfo.find( GetUID() );
    if ( mitCharJob == mapPromotionInfo.end() ) {
        START_LOG( cerr, L"룸에 유저 정보가 없음. LoginID: " << GetName() ) << END_LOG;
        return;
    }

    // 룸에 캐릭터 정보가 있는지 확인한다
    if ( true == mitCharJob->second.empty() ) {
        START_LOG( cerr, L"룸에 캐릭터 정보가 없음. LoginID: " << GetName() ) << END_LOG;
        return;
    }

    // 서버에 유저의 캐릭터 정보가 있는지 확인한다
    int nCharType = mitCharJob->second.begin()->first;
    KCharacterInfo* pkCharInfo = GetCharacterInfo( nCharType );
    if ( NULL == pkCharInfo ) {
        START_LOG( cerr, L"서버에 유저 캐릭터 정보가 없음. LoginID: " << GetName() ) << END_LOG;
        return;
    }

    // Exp 보상을 처리한다
    if ( 0 < kSubject.m_nAddExp ) {
        ApplyExpToCharacter( nCharType, kSubject.m_nAddExp );
        kPacket.m_dwGainExp = kSubject.m_nAddExp;
    }

    // GP 보상을 처리한다
    if ( 0 < kSubject.m_nAddGp ) {
        IncreaseGP( KGpStatInfo::GIT_DUNGEON_SUBJECT_REWARD_GP, false, kSubject.m_nAddGp );
        kPacket.m_dwGainGp = kSubject.m_nAddGp;
    }

    // 아이템 보상을 처리한다(DB에 전달해서 처리하도록 한다)
    if ( false == kSubject.m_vecRewardItem.empty() ) {
        BOOST_FOREACH( KDropItemInfo& rDropItemInfo, kSubject.m_vecRewardItem ) {
            KItem kItem;
            kItem.m_ItemID = rDropItemInfo.m_ItemID;
            kItem.m_cGradeID= rDropItemInfo.m_cGradeID;
            kItem.m_nCount = rDropItemInfo.m_nDuration;
            kItem.m_nPeriod = rDropItemInfo.m_nPeriod;

            kPacket.m_vecRewards.push_back( kItem );
        }
        bGetItemReward = true;
    }

    SET_ERROR( NET_OK );

    if ( true == bGetItemReward ) { // 아이템 보상을 받은 경우에는 DB처리를 거친 후 패킷을 전송한다
        QUEUEING_EVENT_TO_DB_CHECK( EVENT_DUNGEON_SUBJECT_REWARD_REQ, kPacket );
    }

    // 룸에 과제 달성 여부를 기록해둔다
    m_pkRoom->AddCompletedSubjectTrigger( GetUID(), kPacket.m_dwTriggerID );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, NetError::NET_OK ); // 성공

        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 ); // 아직 처리중인 작업
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"던전 과제 보상 요청시 알수 없는 오류, Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    if ( false == bGetItemReward ) {
        // 아이템 보상이 있으면 DB처리 거쳐서 보냄
        SEND_PACKET( EVENT_DUNGEON_SUBJECT_REWARD_ACK );
    }

    START_LOG( clog, L"던전 과제 보상 요청 결과: " << NetError::GetLastNetErrMsg() << dbg::endl )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwTriggerID )
        << BUILD_LOG( kPacket.m_dwSubjectID )
        << BUILD_LOG( kPacket.m_dwGainExp )
        << BUILD_LOG( kPacket.m_dwGainGp )
        << BUILD_LOG( kPacket.m_vecRewards.size() )
        << BUILD_LOG( bGetItemReward )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_DUNGEON_SUBJECT_REWARD_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( false == IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    SET_ERROR( ERR_UNKNOWN );

    // DB에서 아이템만 넣고 온다
    if ( false == kPacket_.m_vecRewards.empty() ) {
        m_kInventory.AddItemList( kPacket_.m_vecRewards );
    }

    // 룸에 과제 달성 여부를 기록해둔다
    if ( NULL != m_pkRoom ) {
        m_pkRoom->AddCompletedSubjectTrigger( GetUID(), kPacket_.m_dwTriggerID );
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                              kPacket_.m_nOK, NetError::NET_OK ); // 성공
        ERR_CASE( ERR_DUNGEON_SUBJECT_REWARD_ERROR_01, kPacket_.m_nOK, NetError::ERR_DUNGEON_SUBJECT_REWARD_ERROR_01 ); // 성공

        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket_.m_nOK, -98 ); // 아직 처리중인 작업
    default:
        kPacket_.m_nOK = 99;
        START_LOG( cerr, L"던전 과제 보상 응답 전송(아이템 받음)시 알수 없는 오류, Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    SEND_RECEIVED_PACKET( EVENT_DUNGEON_SUBJECT_REWARD_ACK );

    START_LOG( clog, L"던전 과제 보상 응답 전송(아이템 받음)" )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwTriggerID )
        << BUILD_LOG( kPacket_.m_dwSubjectID )
        << BUILD_LOG( kPacket_.m_dwGainExp )
        << BUILD_LOG( kPacket_.m_dwGainGp )
        << BUILD_LOG( kPacket_.m_vecRewards.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_GRADUATE_CHARACTER_USER_INFO_NOT )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( false == IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    SET_ERROR( ERR_UNKNOWN );

    // DB에서 아이템만 넣고 온다
    if ( false == kPacket_.m_vecRewards.empty() ) {
        m_kInventory.AddItemList( kPacket_.m_vecRewards );
    }

    // 유저 객체에 새로 계산한 미졸업 캐릭터 리스트를 갱신한다
    m_setNonGraduateCharacters      = kPacket_.m_setNonGraduateCharacters;

    // 새로 계산한 모드 정보도 갱신한다
    //m_mapDifficulty                 = kPacket_.m_mapDifficulty;
    UpdateCharDifficultInfo( GetCurrentCharType(), kPacket_.m_mapDifficulty );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                                   kPacket_.m_nOK, NetError::NET_OK ); // 성공
        ERR_CASE( ERR_GRADUATE_CHARACTER_REWARD_ERROR_01,   kPacket_.m_nOK, NetError::ERR_DUNGEON_SUBJECT_REWARD_ERROR_01 ); // 성공

        ERR_CASE( ERR_NOT_FINISHED_YET,                     kPacket_.m_nOK, -98 ); // 아직 처리중인 작업
    default:
        kPacket_.m_nOK = 99;
        START_LOG( cerr, L"캐릭터 졸업 보상 응답 전송(아이템 받음)시 알수 없는 오류, Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    SEND_RECEIVED_PACKET( EVENT_GRADUATE_CHARACTER_USER_INFO_NOT );

    START_LOG( clog, L"캐릭터 졸업 보상 응답 전송(아이템 받음)" )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nSendType )
        << BUILD_LOG( kPacket_.m_mapDifficulty.size() )
        << BUILD_LOG( kPacket_.m_setGraduateCharacters.size() )
        << BUILD_LOG( kPacket_.m_setNonGraduateCharacters.size() )
        << BUILD_LOG( kPacket_.m_vecRewards.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_SYSTEM_GUIDE_COMPLETE_INFO_NOT )
{
    m_kSystemGuideUserData.SetGuideCompleteInfo( kPacket_.m_mapComplete );

    SEND_RECEIVED_PACKET( EVENT_SYSTEM_GUIDE_COMPLETE_INFO_NOT );
}

IMPL_ON_FUNC( EVENT_SYSTEM_GUIDE_ITEM_REQ )
{
    KDB_EVENT_SYSTEM_GUIDE_ITEM_REQ kDBPacket;
    KEVENT_SYSTEM_GUIDE_ITEM_ACK kPacket;

    // 이미 완료한 가이드인지 확인
    if ( true == m_kSystemGuideUserData.CheckAlreadyCompleteGuide( kPacket_.m_nGuideType ) ) {
        SET_ERR_GOTO( ERR_SYSTEM_GUIDE_ERROR_04, END_PROC );
    }

    // 가이드 아이템을 이미 지급 받았는지 확인
    if ( true == m_kSystemGuideUserData.CheckAlreadyItemProvide( kPacket_.m_nGuideType ) ) {
        SET_ERR_GOTO( ERR_SYSTEM_GUIDE_ERROR_05, END_PROC );
    }

    if ( false == SiKSystemGuide()->GetProvideItem( kPacket_.m_nGuideType, kPacket_.m_nCharType, kDBPacket.m_vecProvide ) ) {
        SET_ERR_GOTO( ERR_SYSTEM_GUIDE_ERROR_01, END_PROC );
    }

    kDBPacket.m_nGuideType = kPacket_.m_nGuideType;
    kDBPacket.m_nProvideFlag = KGuideCompleteInfo::FLAG_GUIDE_ITEM_PROVIDE_COMPLETE;
    kDBPacket.m_nCompleteFlag = KGuideCompleteInfo::FLAG_GUIDE_YET;

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_SYSTEM_GUIDE_ITEM_REQ, kDBPacket );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                    kPacket.m_nOK, 0 );
        ERR_CASE( ERR_SYSTEM_GUIDE_ERROR_01, kPacket.m_nOK, NetError::ERR_SYSTEM_GUIDE_ERROR_01); // 아이템을 요청 한 가이드의 타입이 유효하지 않다
        ERR_CASE( ERR_SYSTEM_GUIDE_ERROR_04, kPacket.m_nOK, NetError::ERR_SYSTEM_GUIDE_ERROR_04); // 이미 완료한 가이드
        ERR_CASE( ERR_SYSTEM_GUIDE_ERROR_05, kPacket.m_nOK, NetError::ERR_SYSTEM_GUIDE_ERROR_05); // 이미 가이드 아이템을 지급 받음
        ERR_CASE( ERR_NOT_FINISHED_YET,      kPacket.m_nOK, -98 );

    default:
        START_LOG( cerr, L"가이드 아이템 지급 시 알수 없는 오류 발생, Name : " << m_strName )
            << BUILD_LOG( kPacket_.m_nGuideType )
            << BUILD_LOG( kPacket_.m_nCharType )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }
}

IMPL_ON_FUNC( EVENT_SYSTEM_GUIDE_ITEM_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    if ( !kPacket_.m_vecProvideItem.empty() ) {
        // 획득 아이템 넣기
        m_kInventory.AddItemList( kPacket_.m_vecProvideItem );
    }

    m_kSystemGuideUserData.UpdateItemProvideComplete( kPacket_.m_nGuideType );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                    kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_SYSTEM_GUIDE_ERROR_02, kPacket_.m_nOK, NetError::ERR_SYSTEM_GUIDE_ERROR_02 ); // 가이드 아이템 DB지급 실패
        ERR_CASE( ERR_SYSTEM_GUIDE_ERROR_03, kPacket_.m_nOK, NetError::ERR_SYSTEM_GUIDE_ERROR_03 ); // 가이드 유저 데이터 DB 갱신 실패
    default:
        START_LOG( cerr, L"가이드 아이템 요청 처리중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_SYSTEM_GUIDE_ITEM_ACK );

    START_LOG( clog, L"가이드 아이템 지급 응답 " )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nGuideType )
        << BUILD_LOG( kPacket_.m_vecProvideItem.size() )
        << END_LOG;
}

_IMPL_ON_FUNC( EVENT_SYSTEM_GUIDE_COMPLETE_REQ, int )
{
    KDB_EVENT_SYSTEM_GUIDE_COMPLETE_REQ kDBPacket;
    KEVENT_SYSTEM_GUIDE_COMPLETE_ACK kPacket;

    if ( true == m_kSystemGuideUserData.CheckAlreadyCompleteGuide( kPacket_ ) ) {
        SET_ERR_GOTO( ERR_SYSTEM_GUIDE_ERROR_04, END_PROC );
    }

    // 아이템 받은 기록 있으면 얻어옴
    m_kSystemGuideUserData.GetProvideDataByGuideType( kPacket_, kDBPacket.m_kCompleteInfo );
    kDBPacket.m_nGuideType = kPacket_;
    kDBPacket.m_kCompleteInfo.m_nCompleteFlag = KGuideCompleteInfo::FLAG_GUIDE_COMPLETE;

    QUEUEING_EVENT_TO_DB( DB_EVENT_SYSTEM_GUIDE_COMPLETE_REQ, kDBPacket );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                    kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_SYSTEM_GUIDE_ERROR_02, kPacket.m_nOK, NetError::ERR_SYSTEM_GUIDE_ERROR_02 ); // 가이드 아이템 DB지급 실패
    default:
        START_LOG( cerr, L"가이드 완료 요청 처리 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99;
    }

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        kPacket.m_nGuideType = kPacket_;
        SEND_PACKET( EVENT_SYSTEM_GUIDE_COMPLETE_ACK );
    }

    START_LOG( clog, L"가이드 완료 요청" )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( kPacket.m_nOK )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_SYSTEM_GUIDE_COMPLETE_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    m_kSystemGuideUserData.UpdateGuideComplete( kPacket_.m_nGuideType, kPacket_.m_kCompleteInfo );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                    kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_SYSTEM_GUIDE_ERROR_03, kPacket_.m_nOK, NetError::ERR_SYSTEM_GUIDE_ERROR_03 ); // 가이드 유저 데이터 DB 갱신 실패
    default:
        START_LOG( cerr, L"가이드 완료 처리중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_SYSTEM_GUIDE_COMPLETE_ACK );

    START_LOG( clog, L"가이드 완료 응답 " )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nGuideType )
        << BUILD_LOG( kPacket_.m_kCompleteInfo.m_nProvideFlag )
        << BUILD_LOG( kPacket_.m_kCompleteInfo.m_nCompleteFlag )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_CONNECTION_GIFTBOX_FIRST_CHARACTER_GET_GIFT_NOT )
{
    if ( false == kPacket_.empty() ) {
        m_kInventory.AddItemList( kPacket_ );
    }

    SEND_RECEIVED_PACKET( EVENT_CONNECTION_GIFTBOX_FIRST_CHARACTER_GET_GIFT_NOT );
}

IMPL_ON_FUNC_NOPARAM( EVENT_CONNECTION_GIFTBOX_NEWUSER_GET_GIFT_REQ )
{
    KDB_EVENT_CONNECTION_GIFTBOX_NEWUSER_GET_GIFT_REQ kDBPacket;
    KEVENT_CONNECTION_GIFTBOX_NEWUSER_GET_GIFT_ACK kPacket;
    kPacket.m_nOK = -99;

    int nCharType = static_cast< int >( m_cCurrentCharacter );

    SET_ERROR( ERR_UNKNOWN );

    if ( false == m_kConnectionGiftBoxUserData.IsNewUser() ) {
        SET_ERR_GOTO( ERR_CONNECTION_GIFTBOX_ERROR_01, END_PROC );
    }

    int nNewUserCurrentGrade = m_kConnectionGiftBoxUserData.GetNewUserCurrentGrade( nCharType );
    if ( false == SiKConnectionGiftBox()->GetNewUserCurrentGradeRewards( nNewUserCurrentGrade, nCharType, kDBPacket.m_vecRewards ) ) {
        SET_ERR_GOTO( ERR_CONNECTION_GIFTBOX_ERROR_02, END_PROC );
    }

    if ( 0 < m_kConnectionGiftBoxUserData.GetNewUserRemainAccPlayTime( nCharType ) ) {
        SET_ERR_GOTO( ERR_CONNECTION_GIFTBOX_ERROR_05, END_PROC );
    }

    kDBPacket.m_nVersion = 0; // 신규유저 대상 선물상자는 버전을 0으로 기록한다
    kDBPacket.m_nCurrentGrade = nNewUserCurrentGrade;

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_CONNECTION_GIFTBOX_NEWUSER_GET_GIFT_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_CONNECTION_GIFTBOX_ERROR_01,  kPacket.m_nOK, NetError::ERR_CONNECTION_GIFTBOX_ERROR_01 ); // 신규유저 대상이 아님.
        ERR_CASE( ERR_CONNECTION_GIFTBOX_ERROR_02,  kPacket.m_nOK, NetError::ERR_CONNECTION_GIFTBOX_ERROR_02 ); // 현재 등급의 보상 정보가 없음.
        ERR_CASE( ERR_CONNECTION_GIFTBOX_ERROR_05,  kPacket.m_nOK, NetError::ERR_CONNECTION_GIFTBOX_ERROR_05 ); // 현재 등급의 누적 시간이 부족함.

        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"접속 선물상자 신규유저 보상 요청 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"접속 선물상자 신규유저 보상 요청 처리, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( nNewUserCurrentGrade )
        << BUILD_LOG( kDBPacket.m_nVersion )
        << BUILD_LOG( kDBPacket.m_nCurrentGrade )
        << BUILD_LOG( kDBPacket.m_vecRewards.size() )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_CONNECTION_GIFTBOX_NEWUSER_GET_GIFT_ACK );
    }
}

IMPL_ON_FUNC( EVENT_CONNECTION_GIFTBOX_NEWUSER_GET_GIFT_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( false == IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    if ( false == kPacket_.m_vecGetRewards.empty() ) {
        m_kInventory.AddItemList( kPacket_.m_vecGetRewards );
    }

    int nCharType = static_cast< int >( m_cCurrentCharacter );

    m_kConnectionGiftBoxUserData.ResetNewUserAccPlayTime( nCharType );
    m_kConnectionGiftBoxUserData.IncreaseNewUserLastGetGrade( nCharType );

    kPacket_.m_nAccPlayTime = m_kConnectionGiftBoxUserData.GetNewUserAccPlayTime( nCharType );

    SiKConnectionGiftBox()->GetNewUserNextGift( nCharType, m_kConnectionGiftBoxUserData.GetNewUserCurrentGrade( nCharType ), kPacket_.m_vecNextGift );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_CONNECTION_GIFTBOX_ERROR_03,  kPacket_.m_nOK, NetError::ERR_CONNECTION_GIFTBOX_ERROR_03 ); // 선물상자 유저정보 갱신 중 DB기록 실패.

        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket_.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"접속 선물상자 신규유저 보상 응답 처리 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    /* NET_OK값과 같으므로 따로 설정하지 않는다
    if ( NetError::GetLastNetError() == NetError::NET_OK ) {
        kPacket_.m_nOK = KConnectionGiftboxInfoNot::EST_GET_GIFT_ACK;
    }
    */

    SEND_RECEIVED_PACKET( EVENT_CONNECTION_GIFTBOX_NEWUSER_GET_GIFT_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"접속 선물상자 신규유저 보상 요청 처리 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecGetRewards.size() )
        << BUILD_LOG( kPacket_.m_nAccPlayTime )
        << BUILD_LOG( kPacket_.m_vecNextGift.size() )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_CONNECTION_GIFTBOX_EVENTUSER_GET_GIFT_REQ )
{
    KDB_EVENT_CONNECTION_GIFTBOX_EVENTUSER_GET_GIFT_REQ kDBPacket;
    KEVENT_CONNECTION_GIFTBOX_EVENTUSER_GET_GIFT_ACK kPacket;
    kPacket.m_nOK = -99;

    int nCharType = static_cast< int >( m_cCurrentCharacter );

    SET_ERROR( ERR_UNKNOWN );

    if ( false == SiKConnectionGiftBox()->IsGiftEventUserTerm() ) {
        SET_ERR_GOTO( ERR_CONNECTION_GIFTBOX_ERROR_04, END_PROC );
    }

    int nEventUserCurrentGrade = m_kConnectionGiftBoxUserData.GetEventUserCurrentGrade( nCharType );
    if ( false == SiKConnectionGiftBox()->GetEventUserCurrentGradeRewards( nEventUserCurrentGrade, nCharType, kDBPacket.m_vecRewards ) ) {
        SET_ERR_GOTO( ERR_CONNECTION_GIFTBOX_ERROR_02, END_PROC );
    }

    if ( 0 < m_kConnectionGiftBoxUserData.GetEventUserRemainAccPlayTime( nCharType ) ) {
        SET_ERR_GOTO( ERR_CONNECTION_GIFTBOX_ERROR_05, END_PROC );
    }

    kDBPacket.m_nVersion = SiKConnectionGiftBox()->GetGiftEventUserVersion();
    kDBPacket.m_nCurrentGrade = nEventUserCurrentGrade;

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_CONNECTION_GIFTBOX_EVENTUSER_GET_GIFT_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_CONNECTION_GIFTBOX_ERROR_02,  kPacket.m_nOK, NetError::ERR_CONNECTION_GIFTBOX_ERROR_02 ); // 현재 등급의 보상 정보가 없음.
        ERR_CASE( ERR_CONNECTION_GIFTBOX_ERROR_04,  kPacket.m_nOK, NetError::ERR_CONNECTION_GIFTBOX_ERROR_04 ); // 이벤트유저 선물상자 이벤트 기간이 아님.
        ERR_CASE( ERR_CONNECTION_GIFTBOX_ERROR_05,  kPacket.m_nOK, NetError::ERR_CONNECTION_GIFTBOX_ERROR_05 ); // 현재 등급의 누적 시간이 부족함.

        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"접속 선물상자 이벤트유저 보상 요청 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"접속 선물상자 이벤트유저 보상 요청 처리, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( nEventUserCurrentGrade )
        << BUILD_LOG( kDBPacket.m_nVersion )
        << BUILD_LOG( kDBPacket.m_nCurrentGrade )
        << BUILD_LOG( kDBPacket.m_vecRewards.size() )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_CONNECTION_GIFTBOX_EVENTUSER_GET_GIFT_ACK );
    }
}

IMPL_ON_FUNC( EVENT_CONNECTION_GIFTBOX_EVENTUSER_GET_GIFT_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( false == IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    if ( false == kPacket_.m_vecGetRewards.empty() ) {
        m_kInventory.AddItemList( kPacket_.m_vecGetRewards );
    }

    int nCharType = static_cast< int >( m_cCurrentCharacter );

    m_kConnectionGiftBoxUserData.ResetEventUserAccPlayTime( nCharType );
    m_kConnectionGiftBoxUserData.IncreaseEventUserLastGetGrade( nCharType );

    kPacket_.m_nAccPlayTime = m_kConnectionGiftBoxUserData.GetEventUserAccPlayTime( nCharType );

    SiKConnectionGiftBox()->GetEventUserNextGift( nCharType, m_kConnectionGiftBoxUserData.GetEventUserCurrentGrade( nCharType ), kPacket_.m_vecNextGift );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_CONNECTION_GIFTBOX_ERROR_03,  kPacket_.m_nOK, NetError::ERR_CONNECTION_GIFTBOX_ERROR_03 ); // 선물상자 유저정보 갱신 중 DB기록 실패.

        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket_.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"접속 선물상자 이벤트유저 보상 응답 처리 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    /* NET_OK값과 같으므로 따로 설정하지 않는다
    if ( NetError::GetLastNetError() == NetError::NET_OK ) {
        kPacket_.m_nOK = KConnectionGiftboxInfoNot::EST_GET_GIFT_ACK;
    }
    */

    SEND_RECEIVED_PACKET( EVENT_CONNECTION_GIFTBOX_EVENTUSER_GET_GIFT_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"접속 선물상자 이벤트유저 보상 요청 처리 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecGetRewards.size() )
        << BUILD_LOG( kPacket_.m_nAccPlayTime )
        << BUILD_LOG( kPacket_.m_vecNextGift.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_USER_RELAY_SEND_COUNT_REQ )
{
    // 모드 플레이 시간 저장.
    kPacket_.m_dwPlayTime = m_dwLastPlayTime;
    QUEUEING_EVENT_TO_DB( EVENT_USER_RELAY_SEND_COUNT_REQ, kPacket_ );
}

IMPL_ON_FUNC( EVENT_COORDI_COMPOSE_REQ )
{
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_COORDI_COMPOSE_ACK kPacket;
    KDB_EVENT_COORDI_COMPOSE_REQ kDBPacket;
    KItemPtr pkAbilityItem;  // 코디 능력치 아이템
    KItemPtr pkDesignItem;  // 코디 외형 아이템
    KItemPtr pkCoordiComposeItem;  // 코디 합성 아이템

    pkAbilityItem = m_kInventory.FindItemByItemUID( kPacket_.m_AbilityItemUID );
    pkDesignItem = m_kInventory.FindItemByItemUID( kPacket_.m_DesignItemUID );
    pkCoordiComposeItem = m_kInventory.FindDurationItemByItemID( SiKLookEquipment()->GetCoordiComposeItemID() );
    if( !pkAbilityItem || !pkDesignItem ) {
        SET_ERR_GOTO( ERR_COORDI_COMPOSE_01, END_PROC );
    }

    if( !pkCoordiComposeItem ) {
        SET_ERR_GOTO( ERR_COORDI_COMPOSE_09, END_PROC );
    }

    if( kPacket_.m_AbilityItemUID == kPacket_.m_DesignItemUID ) {
        SET_ERR_GOTO( ERR_COORDI_COMPOSE_10, END_PROC );
    }

    if( !SiKLookEquipment()->IsLookItemID( pkAbilityItem->m_ItemID ) ) {
        SET_ERR_GOTO( ERR_COORDI_COMPOSE_02, END_PROC );
    }

    if( !pkDesignItem->CheckItemType( KItem::TYPE_LOOK ) ) {
        SET_ERR_GOTO( ERR_COORDI_COMPOSE_03, END_PROC );
    }

    if( pkAbilityItem->m_vecAttribute.empty() ) { // 속성이 있는지 체크.
        SET_ERR_GOTO( ERR_COORDI_COMPOSE_04, END_PROC );
    }

    if( !pkAbilityItem->IsUnlimited() ) { // 영구 아이템인지 체크.
        SET_ERR_GOTO( ERR_COORDI_COMPOSE_05, END_PROC );
    }

    if( !pkDesignItem->IsUnlimited() ) { // 영구 아이템인지 체크.
        SET_ERR_GOTO( ERR_COORDI_COMPOSE_08, END_PROC );
    }

    kDBPacket.m_kAbilityItem = *pkAbilityItem;
    kDBPacket.m_kDesignItem = *pkDesignItem;
    kDBPacket.m_kCoordiComposeItem = *pkCoordiComposeItem;

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_COORDI_COMPOSE_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_COORDI_COMPOSE_01, kPacket.m_nOK, 1 ); // 아이템이 인벤토리에 존재하지 않음.
        ERR_CASE( ERR_COORDI_COMPOSE_02, kPacket.m_nOK, 2 ); // 능력치 아이템이 코디가 아님.
        ERR_CASE( ERR_COORDI_COMPOSE_03, kPacket.m_nOK, 3 ); // 외형 아이템이 코디가 아님.
        ERR_CASE( ERR_COORDI_COMPOSE_04, kPacket.m_nOK, 4 ); // 능력치 코디 아이템에 능력치가 없음.
        ERR_CASE( ERR_COORDI_COMPOSE_05, kPacket.m_nOK, 5 ); // 능력치 코디 아이템이 영구 아이템이 아님.
        ERR_CASE( ERR_COORDI_COMPOSE_06, kPacket.m_nOK, 6 ); // 코디 합성 정보 DB 갱신 에러.
        ERR_CASE( ERR_COORDI_COMPOSE_08, kPacket.m_nOK, 8 ); // 외형 코디 아이템이 영구 아이템이 아님.
        ERR_CASE( ERR_COORDI_COMPOSE_09, kPacket.m_nOK, 9 ); // 코디 합성 아이템이 없음.
        ERR_CASE( ERR_COORDI_COMPOSE_10, kPacket.m_nOK, 10 ); // 능력치 코디와 외형 코디가 동일한 아이템임.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"코디 합성 요청중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_COORDI_COMPOSE_ACK );
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"코디 합성 요청, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_AbilityItemUID )
        << BUILD_LOG( kPacket_.m_DesignItemUID )
        << BUILD_LOG( kPacket.m_nOK )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_COORDI_COMPOSE_ACK )
{
    KItemPtr pkAbilityItem;  // 코디 능력치 아이템
    KItemPtr pkDesignItem;  // 코디 외형 아이템
    KItemPtr pkCoordiComposeItem;  // 코디 합성 아이템

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    pkAbilityItem = m_kInventory.FindItemByItemUID( kPacket_.m_kAbilityItem.m_ItemUID );
    pkDesignItem = m_kInventory.FindItemByItemUID( kPacket_.m_kDesignItem.m_ItemUID );
    pkCoordiComposeItem = m_kInventory.FindDurationItemByItemID( SiKLookEquipment()->GetCoordiComposeItemID() );

    if( pkAbilityItem ) {
        pkAbilityItem->m_DesignCoordiID = kPacket_.m_kAbilityItem.m_DesignCoordiID;
        kPacket_.m_vecChangeItem.push_back( *pkAbilityItem );
    }

    if( pkDesignItem ) {
        kPacket_.m_vecDelItem.push_back( *pkDesignItem );
        m_kInventory.RemoveItem( pkDesignItem->m_ItemUID );
    }

    // 코디 합성 주문서 아이템 수량 감소 처리.
    if( pkCoordiComposeItem ) {
        pkCoordiComposeItem->m_nCount = kPacket_.m_kCoordiComposeItem.m_nCount;
        pkCoordiComposeItem->m_nInitCount = kPacket_.m_kCoordiComposeItem.m_nCount;
        kPacket_.m_vecUseItem.push_back( *pkCoordiComposeItem );
    }

    if ( pkCoordiComposeItem && pkCoordiComposeItem->m_nCount <= 0 ) { // 수량다 사용한 수량아이템 깍기.
        m_kInventory.RemoveItem( kPacket_.m_kCoordiComposeItem.m_ItemID, kPacket_.m_kCoordiComposeItem.m_ItemUID );
    }

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_COORDI_COMPOSE_01, kPacket_.m_nOK, 1 ); // 아이템이 인벤토리에 존재하지 않음.
        ERR_CASE( ERR_COORDI_COMPOSE_02, kPacket_.m_nOK, 2 ); // 능력치 아이템이 코디가 아님.
        ERR_CASE( ERR_COORDI_COMPOSE_03, kPacket_.m_nOK, 3 ); // 외형 아이템이 코디가 아님.
        ERR_CASE( ERR_COORDI_COMPOSE_04, kPacket_.m_nOK, 4 ); // 능력치 코디 아이템에 능력치가 없음.
        ERR_CASE( ERR_COORDI_COMPOSE_05, kPacket_.m_nOK, 5 ); // 능력치 코디 아이템이 영구 아이템이 아님.
        ERR_CASE( ERR_COORDI_COMPOSE_06, kPacket_.m_nOK, 6 ); // 코디 합성 정보 DB 갱신 에러.
        ERR_CASE( ERR_COORDI_COMPOSE_08, kPacket_.m_nOK, 8 ); // 외형 코디 아이템이 영구 아이템이 아님.
        ERR_CASE( ERR_COORDI_COMPOSE_09, kPacket_.m_nOK, 9 ); // 코디 합성 아이템이 없음.
        ERR_CASE( ERR_COORDI_COMPOSE_10, kPacket_.m_nOK, 10 ); // 코디 합성 아이템이 없음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 처리중인 작업입니다.
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99; // unknown error
        break;
    }

    SEND_RECEIVED_PACKET( EVENT_COORDI_COMPOSE_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"코디 합성 요청 MSG :" << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName)
        << BUILD_LOG( kPacket_.m_vecUseItem.size() )
        << BUILD_LOG( kPacket_.m_vecDelItem.size() )
        << BUILD_LOG( kPacket_.m_vecChangeItem.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_VIRTUAL_DEPOT_ITEM_LIST_NOT )
{
    if ( false == kPacket_.empty() ) {
        SetVirtualDepotItemList( kPacket_ );

        SEND_COMPRESS_PACKET( EVENT_VIRTUAL_DEPOT_ITEM_LIST_NOT, kPacket_ );
    }
}

IMPL_ON_FUNC( EVENT_MOVE_ITEM_TO_INVENTORY_FROM_VIRTUAL_DEPOT_REQ )
{
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_MOVE_ITEM_TO_INVENTORY_FROM_VIRTUAL_DEPOT_ACK kPacket;
    std::map<int, std::vector< KItem > > mapMoveItems;

    mapMoveItems.clear();

    if ( false == kPacket_.empty() ) {
        std::map< int, std::vector<KItem> >::iterator mitCharMoveItem;
        std::vector<KItem>::iterator vitMoveItem;

        for( mitCharMoveItem = kPacket_.begin(); mitCharMoveItem != kPacket_.end(); mitCharMoveItem++ ) {
            std::vector<KItem> vecTempMoveItem;
            vecTempMoveItem.clear();

            for ( vitMoveItem = mitCharMoveItem->second.begin(); vitMoveItem != mitCharMoveItem->second.end(); vitMoveItem++ ) {
                KItem kVirtualDepotItem;
                // 실제로 가상 창고에 존재 하는 아이템인지 확인
                if ( false == GetVirtualDepotItemByItemUID( vitMoveItem->m_ItemUID, kVirtualDepotItem ) ) {
                    START_LOG( clog, L"가상창고에 없는 아이템을 옮기려고 요청 함" )
                        << BUILD_LOG( vitMoveItem->m_ItemID )
                        << BUILD_LOG( vitMoveItem->m_ItemUID )
                        << BUILD_LOG( vitMoveItem->m_cInvenType )
                        << END_LOG;
                    continue;
                }

                // 클라에서 보낸 ItemID와 서버의 ItemID가 일치하는지 확인 (클라에서 0떼고 보내는 이슈로 검증 코드 추가 함)
                if ( kVirtualDepotItem.m_ItemID != vitMoveItem->m_ItemID ) {
                    vitMoveItem->m_ItemID = kVirtualDepotItem.m_ItemID;
                }

                // 이미 인벤토리에 있는 아이템을 옮기려고 시도 함
                if ( KItem::INVENTORY == vitMoveItem->m_cInvenType ) {

                    START_LOG( clog, L"아이템의 타입이 인벤토리인 아이템을 가상창고에서 인벤으로 옮기려고 함" )
                        << BUILD_LOG( vitMoveItem->m_ItemID )
                        << BUILD_LOG( vitMoveItem->m_ItemUID )
                        << BUILD_LOG( vitMoveItem->m_cInvenType )
                        << END_LOG;
                    continue;
                }

                // 창고에 있는 아이템을 옮기려고 시도 함
                if ( KItem::DEPOT == vitMoveItem->m_cInvenType ) {
                    START_LOG( clog, L"아이템의 타입이 창고인 아이템을 가상창고에서 인벤으로 옮기려고 함" )
                        << BUILD_LOG( vitMoveItem->m_ItemID )
                        << BUILD_LOG( vitMoveItem->m_ItemUID )
                        << BUILD_LOG( vitMoveItem->m_cInvenType )
                        << END_LOG;
                    continue;
                }

                // 비정상적인 수량을 옮기려고 함
                if ( KItem::UNLIMITED_ITEM != vitMoveItem->m_nCount && 0 < vitMoveItem->m_nCount ) {
                    if ( false == IsValidItemCount( *vitMoveItem ) ) {
                        START_LOG( clog, L"비정상적인 수량을 인벤으로 옮기려고 함" )
                            << BUILD_LOG( vitMoveItem->m_ItemID )
                            << BUILD_LOG( vitMoveItem->m_ItemUID )
                            << BUILD_LOG( vitMoveItem->m_nCount )
                            << BUILD_LOG( vitMoveItem->m_cInvenType )
                            << END_LOG;
                        continue;
                    }
                }

                vitMoveItem->m_cInvenType = KItem::INVENTORY;
                vecTempMoveItem.push_back( *vitMoveItem );
            }

            mapMoveItems.insert( std::make_pair( mitCharMoveItem->first, vecTempMoveItem ) );
        }

        QUEUEING_EVENT_TO_DB_CHECK( EVENT_MOVE_ITEM_TO_INVENTORY_FROM_VIRTUAL_DEPOT_REQ, mapMoveItems );

        START_LOG( clog, L"가상창고 -> 인벤토리 아이템 이동 요청" )
            << BUILD_LOG( m_strName )
            << BUILD_LOG( kPacket_.size() )
            << END_LOG;
    }
    else {
        SET_ERR_GOTO( ERR_VIRTUAL_DEPOT_ERROR_01, END_PROC );
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                     kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_VIRTUAL_DEPOT_ERROR_01, kPacket.m_nOK, NetError::ERR_VIRTUAL_DEPOT_ERROR_01 ); // 가상창고 아이템 이동 요청을 하는 리스트가 비었음
    default:
        START_LOG( cerr, L"가상창고 아이템 이동 요청 처리 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99;
    }

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_MOVE_ITEM_TO_INVENTORY_FROM_VIRTUAL_DEPOT_ACK );
    }
}

IMPL_ON_FUNC( EVENT_MOVE_ITEM_TO_INVENTORY_FROM_VIRTUAL_DEPOT_ACK )
{
    // 가상 창고 정보 갱신
    SetVirtualDepotItemList( kPacket_.m_vecVirtualDepot );

    // 마지막 플레이 캐릭터 인벤토리 정보 갱신
    if ( false == kPacket_.m_vecUpdateItem.empty() ) {
        m_kInventory.AddItemList( kPacket_.m_vecUpdateItem );
    }

    SEND_COMPRESS_PACKET( EVENT_MOVE_ITEM_TO_INVENTORY_FROM_VIRTUAL_DEPOT_ACK, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EVENT_RELAY_SERVER_STATUS_REQ )
{
    JIF( m_pkRoom );
    KEVENT_RELAY_SERVER_STATUS_ACK kPacket;
    KRoomInfo kRoomInfo;
    std::vector<KSimpleServerInfo> vecUseTCPServerInfo;
    std::vector<KSimpleServerInfo> vecNoUseTCPServerInfo;
    SiKGameServer()->GetTCPServerList( vecUseTCPServerInfo, vecNoUseTCPServerInfo );

    m_pkRoom->GetRoomInfo( kRoomInfo );
    if( !CheckTCPRelayStatus( kRoomInfo.m_usRoomID, kRoomInfo.m_dwTRelayServerIP, kRoomInfo.m_usTRelayServerPort, vecUseTCPServerInfo, vecNoUseTCPServerInfo, kPacket ) ) {
        START_LOG( cerr, L"룸에 TCP Relay Server 정보 변경" )
            << BUILD_LOG( kRoomInfo.m_usRoomID )
            << BUILD_LOG( kPacket.m_nOK )
            << BUILD_LOG( kPacket.m_dwTRelayServerIP )
            << BUILD_LOG( kPacket.m_usTRelayServerPort )
            << END_LOG;
        m_pkRoom->SetTRServerInfo( kPacket.m_dwTRelayServerIP, kPacket.m_usTRelayServerPort );
    }

    m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_RELAY_SERVER_STATUS_ACK, kPacket );
}

IMPL_ON_FUNC( EVENT_TONG_DONATION_AMOUNT_INFO_ACK )
{
    m_mapTongDonationUserAmount = kPacket_;

    START_LOG( clog, L"통합 기부 이벤트 유저 누적 기부량 DB 정보로 갱신" )
        << BUILD_LOG( kPacket_.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_TONG_DONATION_INFO_REQ )
{
    KTongDonationNPCInfo kTongDonationNPCInfo;
    time_t tmCurrentVersion = 0;
    KEVENT_TONG_DONATION_INFO_ACK kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    if ( false == SiKTongDonation()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_TONG_DONATION_ERROR_01, END_PROC );
    }

    if ( false == SiKTongDonation()->GetTongDonationNPCInfo( kPacket_, kTongDonationNPCInfo ) ) {
        SET_ERR_GOTO( ERR_TONG_DONATION_ERROR_02, END_PROC );
    }

    tmCurrentVersion                = SiKTongDonation()->GetCurrentVersion();

    kPacket.m_dwDonationItemID      = kTongDonationNPCInfo.m_dwDonationItemID;
    kPacket.m_dwDonationUnit        = kTongDonationNPCInfo.m_dwDonationUnit;
    kPacket.m_vecDonationRewards    = kTongDonationNPCInfo.m_vecDonationRewards;
    kPacket.m_dwClientUIType        = SiKTongDonation()->GetClientUIType();
    kPacket.m_vecGoalInfo           = kTongDonationNPCInfo.m_vecGoalInfo;
    kPacket.m_nUserAmount           = GetTongDonationUserAmount( tmCurrentVersion, kPacket_ );
    kPacket.m_nTotalAmount          = SiKTongDonation()->GetTotalAmount( tmCurrentVersion, kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, NetError::NET_OK ); // 성공.
        ERR_CASE( ERR_TONG_DONATION_ERROR_01,       kPacket.m_nOK, NetError::ERR_TONG_DONATION_ERROR_01 ); // 통합 기부 이벤트 기간이 아님.
        ERR_CASE( ERR_TONG_DONATION_ERROR_02,       kPacket.m_nOK, NetError::ERR_TONG_DONATION_ERROR_02 ); // 해당 NPC 정보가 없음.
        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"통합 기부 이벤트 정보 요청(NPC 클릭시) 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"통합 기부 이벤트 정보 요청(NPC 클릭시) 처리, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwDonationItemID )
        << BUILD_LOG( kPacket.m_dwDonationUnit )
        << BUILD_LOG( kPacket.m_vecDonationRewards.size() )
        << BUILD_LOG( kPacket.m_dwClientUIType )
        << BUILD_LOG( kPacket.m_vecGoalInfo.size() )
        << BUILD_LOG( kPacket.m_nUserAmount )
        << BUILD_LOG( kPacket.m_nTotalAmount )
        << BUILD_LOGtm( CTime( tmCurrentVersion ) )
        << END_LOG;

    SEND_PACKET( EVENT_TONG_DONATION_INFO_ACK );
}

IMPL_ON_FUNC( EVENT_TONG_DONATION_DONATE_REQ )
{
    KTongDonationNPCInfo kTongDonationNPCInfo;
    time_t tmCurrentVersion = 0;
    DWORD dwRequestDonateCount = kPacket_.m_dwDonateCount;
    KItemPtr pkItem;
    KEVENT_TONG_DONATION_DONATE_ACK kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    if ( false == SiKTongDonation()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_TONG_DONATION_ERROR_01, END_PROC );
    }

    if ( false == SiKTongDonation()->GetTongDonationNPCInfo( kPacket_.m_dwNPCType, kTongDonationNPCInfo ) ) {
        SET_ERR_GOTO( ERR_TONG_DONATION_ERROR_02, END_PROC );
    }

    tmCurrentVersion                = SiKTongDonation()->GetCurrentVersion();

    kPacket.m_nUserAmount           = GetTongDonationUserAmount( tmCurrentVersion, kPacket_.m_dwNPCType );
    kPacket.m_nTotalAmount          = SiKTongDonation()->GetTotalAmount( tmCurrentVersion, kPacket_.m_dwNPCType );

    // 기부 할 아이템을 갖고 있는지 확인한다(수량 아이템)
    pkItem = m_kInventory.FindDurationItemByItemID( kTongDonationNPCInfo.m_dwDonationItemID );
    if ( NULL == pkItem ) {
        SET_ERR_GOTO( ERR_TONG_DONATION_ERROR_03, END_PROC );
    }

    // 보유하고 있는 기부하려는 아이템 수가 기부 가능한 양인지 확인한다
    if ( 0 > ( pkItem->m_nCount - kPacket_.m_dwDonateCount ) ) {
        SET_ERR_GOTO( ERR_TONG_DONATION_ERROR_04, END_PROC );
    }

    // 실제 기부하게 되는 수량 = 요청 기부 수량 - ( 요청 기부 수량 % 기부 단위 )
    kPacket_.m_dwDonateCount       -= ( kPacket_.m_dwDonateCount % kTongDonationNPCInfo.m_dwDonationUnit );

    // 실제 기부하게 되는 수량이 기부 단위보다 작은지 확인한다
    if ( kPacket_.m_dwDonateCount < kTongDonationNPCInfo.m_dwDonationUnit ) {
        SET_ERR_GOTO( ERR_TONG_DONATION_ERROR_04, END_PROC );
    }

    kPacket_.m_kDonationItem        = *pkItem;
    kPacket_.m_tmVersion            = tmCurrentVersion;
    kPacket_.m_vecDonationRewards   = kTongDonationNPCInfo.m_vecDonationRewards;

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_TONG_DONATION_DONATE_REQ, kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_TONG_DONATION_ERROR_01,       kPacket.m_nOK, NetError::ERR_TONG_DONATION_ERROR_01 ); // 통합 기부 이벤트 기간이 아님.
        ERR_CASE( ERR_TONG_DONATION_ERROR_02,       kPacket.m_nOK, NetError::ERR_TONG_DONATION_ERROR_02 ); // 해당 NPC 정보가 없음.
        ERR_CASE( ERR_TONG_DONATION_ERROR_03,       kPacket.m_nOK, NetError::ERR_TONG_DONATION_ERROR_03 ); // 인벤토리에 기부 할 아이템 정보가 없음(수량).
        ERR_CASE( ERR_TONG_DONATION_ERROR_04,       kPacket.m_nOK, NetError::ERR_TONG_DONATION_ERROR_04 ); // 기부하려는 아이템의 수량이 부족함.
        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"통합 기부 이벤트 기부하기 요청(실제 기부하기) 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"통합 기부 이벤트 기부하기 요청(실제 기부하기) 처리, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_dwNPCType )
        << BUILD_LOG( dwRequestDonateCount )
        << BUILD_LOG( kPacket_.m_dwDonateCount )
        << BUILD_LOG( kPacket_.m_kDonationItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kDonationItem.m_ItemUID )
        << BUILD_LOGtm( CTime( kPacket_.m_tmVersion ) )
        << BUILD_LOG( kPacket_.m_vecDonationRewards.size() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nUserAmount )
        << BUILD_LOG( kPacket.m_nTotalAmount )
        << BUILD_LOGtm( CTime( tmCurrentVersion ) )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_TONG_DONATION_DONATE_ACK );
    }
}

IMPL_ON_FUNC( EVENT_TONG_DONATION_DONATE_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    KItemPtr pkItem;

    if ( false == IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    // 기부 한 아이템의 수량을 갱신한다
    pkItem = m_kInventory.FindDurationItemByItemUID( kPacket_.m_kDonationItem.m_ItemUID );
    if ( NULL != pkItem ) {
        pkItem->m_nCount        = kPacket_.m_kDonationItem.m_nCount;
        pkItem->m_nInitCount    = kPacket_.m_kDonationItem.m_nCount;

        if ( pkItem->m_nCount <= 0 ) {
            m_kInventory.RemoveItem( pkItem->m_ItemUID );
        }
    }

    // 기부 보상으로 받은 아이템 정보를 갱신한다
    m_kInventory.AddItemList( kPacket_.m_vecDonationRewards );

    // 개인 누적 기부량, 전체 기부 누적 기부량을 항상 최신의 DB값으로 덮어쓴다
    SetTongDonationUserAmount( kPacket_.m_tmVersion, kPacket_.m_dwNPCType, kPacket_.m_nUserAmount );
    SiKTongDonation()->SetTotalAmount( kPacket_.m_tmVersion, kPacket_.m_dwNPCType, kPacket_.m_nTotalAmount );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_TONG_DONATION_ERROR_05,       kPacket_.m_nOK, NetError::ERR_TONG_DONATION_ERROR_05 ); // 통합 기부 보상 아이템 DB 지급실패.
        ERR_CASE( ERR_TONG_DONATION_ERROR_06,       kPacket_.m_nOK, NetError::ERR_TONG_DONATION_ERROR_06 ); // 통합 기부시 DB와 기부 아이템 동기화 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket_.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"통합 기부 이벤트 기부하기 응답 처리 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_TONG_DONATION_DONATE_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"통합 기부 이벤트 기부하기 요청(실제 기부하기) 처리 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kDonationItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kDonationItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kDonationItem.m_nCount )
        << BUILD_LOG( kPacket_.m_vecDonationRewards.size() )
        << BUILD_LOGtm( CTime( kPacket_.m_tmVersion ) )
        << BUILD_LOG( kPacket_.m_dwNPCType )
        << BUILD_LOG( kPacket_.m_nUserAmount )
        << BUILD_LOG( kPacket_.m_nTotalAmount )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_SOSCIAL_COMMERCE_INFO_REQ )
{
    KEVENT_SOSCIAL_COMMERCE_INFO_ACK kPacket;

    SiKSocialCommerce()->GetSocialCommerceData( kPacket );

    START_LOG( clog, L"소셜커머스 전체 데이터 요청, User : " << GetName() )
        << BUILD_LOG( kPacket.size() )
        << END_LOG;

    SEND_PACKET( EVENT_SOSCIAL_COMMERCE_INFO_ACK );
}

IMPL_ON_FUNC( EVENT_SOSCIAL_COMMERCE_INFO_UPDATE_NOT )
{
    KEVENT_SOSCIAL_COMMERCE_INFO_ACK kPacket;

    // 캐시 아이템 구매 이후 소셜커머스 아이템 수량을 증가 시킨다
    SiKSocialCommerce()->UpdateSellCountDiff( kPacket_.m_dwEventUID, kPacket_.m_nCountDiff );

    SiKSocialCommerce()->GetSocialCommerceData( kPacket );

    START_LOG( clog, L"소셜커머스 아이템 구매 완료 후 전체 데이터 전송, User : " << GetName() )
        << BUILD_LOG( kPacket.size() )
        << END_LOG;

    SEND_PACKET( EVENT_SOSCIAL_COMMERCE_INFO_ACK );
}

IMPL_ON_FUNC( EVENT_COORDI_GRADE_UPGRADE_REQ )
{
    KEVENT_COORDI_GRADE_UPGRADE_ACK kPacket;
    KDB_EVENT_COORDI_GRADE_UPGRADE_REQ kDBPacket;
    kPacket.m_nOK = -99;
    KItemPtr pkUseItem;
    KItemPtr pkInvenItem;

    std::vector< KAttributeInfo > vecAttributeList;
    std::vector< KAttributeInfo >::iterator vit;
    std::list<std::pair<int,float>> listRandomAttributes;
    std::list<std::pair<int,float>> listFullAttributes;
    std::list<std::pair<int,float>>::iterator lit;

    SET_ERROR( ERR_UNKNOWN );

    pkInvenItem = m_kInventory.FindItemByItemUID( kPacket_.m_ItemUID );
    if( !pkInvenItem  ) { // 코디 아이템이 존재하지 않음.
        SET_ERR_GOTO( ERR_COORDI_GRADE_UPGRADE_01, END_PROC );
    }

    pkUseItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_ItemID );
    if( !pkUseItem  ) { // 업그레이할 재료 아이템이 존재하지 않음.
        SET_ERR_GOTO( ERR_COORDI_GRADE_UPGRADE_02, END_PROC );
    }

    // 등급정보 확인
    if( !CheckGradeUpgrade( kPacket_.m_cGrade, pkInvenItem->m_cGradeID ) ) {
        SET_ERR_GOTO( ERR_COORDI_GRADE_UPGRADE_07, END_PROC );
    }

    // 등급 업그레이드 재료 아이템인지 확인.
    if( !SiKItemManager()->m_kGradeDecider.CheckGradeUpgradeItem( kPacket_.m_ItemID ) ) {
        SET_ERR_GOTO( ERR_COORDI_GRADE_UPGRADE_03, END_PROC );
    }

    // 코디 아이템인지 체크.
    if( SiKItemManager()->m_kGradeDecider.IsCoordiItemType() ) {
        if( !pkInvenItem->CheckItemType(KItem::TYPE_LOOK) ) {
            SET_ERR_GOTO( ERR_COORDI_GRADE_UPGRADE_04, END_PROC );
        }
    }

    if( pkInvenItem->m_vecAttribute.empty() ) {
        SET_ERR_GOTO( ERR_COORDI_GRADE_UPGRADE_09, END_PROC );
    }

    // 등급 업그레이드 가능한 아이템인지 체크.
    if( !SiKItemManager()->m_kGradeDecider.CheckUpgradePossibleItem( pkInvenItem->m_ItemID ) ) {
        SET_ERR_GOTO( ERR_COORDI_GRADE_UPGRADE_08, END_PROC );
    }

    // 등급에 따른 업그레이드 재료 사용 수량정보 가져오기.
    if( !SiKItemManager()->m_kGradeDecider.GetGradeUpgradeUseCount( kPacket_.m_cGrade, kDBPacket.m_nUseCount ) ) {
        SET_ERR_GOTO( ERR_COORDI_GRADE_UPGRADE_05, END_PROC );
    }

    kDBPacket.m_kItem = *pkInvenItem;
    kDBPacket.m_kItem.m_cGradeID = kPacket_.m_cGrade; // 갱신할 등급정보 넣어주기.
    kDBPacket.m_kUseItem = *pkUseItem;

    int nAttributeTableID = SiKItemManager()->GetAttributeTableID( pkInvenItem->m_ItemID );
    int nLevelTableID = SiKItemManager()->GetLevelTableID( pkInvenItem->m_ItemID );

    // 레벨에 맞게 속성 변경해야 하는지 체크.
    int nItemLevel = 0;
    if( SiKItemManager()->m_kItemLevelDecider.IsSelectiveType( nLevelTableID ) ) {
        nItemLevel = static_cast<int>( pkInvenItem->m_sEquipLevel );
        SiKItemManager()->m_kItemLevelDecider.CheckItemLevelAttribute( nLevelTableID, nAttributeTableID, nItemLevel, KItemLevelTable::ILT_SELECTIVE );
    }

    int nAttributeCount = SiKItemManager()->GetAttributeCountFromGrade( pkInvenItem->m_cGradeID );
    // 랜덤 속성이 적용된 리스트 가져오기.
    SiKItemManager()->m_kAttributeDecider.GetSelectiveAttributeList( nAttributeTableID, nAttributeCount, kPacket_.m_cGrade, kDBPacket.m_listAttributeList );

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_COORDI_GRADE_UPGRADE_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_COORDI_GRADE_UPGRADE_01, kPacket.m_nOK, 1 );   // 코디 업그레이드할 아이템이 없음.
        ERR_CASE( ERR_COORDI_GRADE_UPGRADE_02, kPacket.m_nOK, 2 );   // 코디 업그레이드 재료 아이템이 없음.
        ERR_CASE( ERR_COORDI_GRADE_UPGRADE_03, kPacket.m_nOK, 3 );   // 코디 업그레이드 재료 아이템이 아님.
        ERR_CASE( ERR_COORDI_GRADE_UPGRADE_04, kPacket.m_nOK, 4 );   // 코디 아이템이 아님.
        ERR_CASE( ERR_COORDI_GRADE_UPGRADE_05, kPacket.m_nOK, 5 );   // 업그레이드할 등급에 사용 갯수가 설정되어 있지 않음.
        ERR_CASE( ERR_COORDI_GRADE_UPGRADE_06, kPacket.m_nOK, 6 );   // 변경된 속성 DB에 기록 하는데 오류 발생.
        ERR_CASE( ERR_COORDI_GRADE_UPGRADE_07, kPacket.m_nOK, 7 );   // 요청한 등급정보가 잘못되었음.
        ERR_CASE( ERR_COORDI_GRADE_UPGRADE_08, kPacket.m_nOK, 8 );   // 등급 업그레이드 불가능한 아이템임.
        ERR_CASE( ERR_COORDI_GRADE_UPGRADE_09, kPacket.m_nOK, 9 );   // 업그레이드 할려는 아이템 속성이 없음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99; // unknown error
        break;
    }

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_COORDI_GRADE_UPGRADE_ACK );
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"코디 등급 업그레이드 요청 MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_ItemID )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_COORDI_GRADE_UPGRADE_ACK )
{
    KItemPtr pkInvenItem;
    KItemPtr pkUseItem;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    pkInvenItem = m_kInventory.FindItemByItemUID( kPacket_.m_kItem.m_ItemUID );
    if( !pkInvenItem  ) { // 코디 아이템이 존재하지 않음.
        SET_ERR_GOTO( ERR_COORDI_GRADE_UPGRADE_01, END_PROC );
    }

    pkUseItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_kUseItem.m_ItemID );
    if( !pkUseItem  ) { // 업그레이할 재료 아이템이 존재하지 않음.
        SET_ERR_GOTO( ERR_COORDI_GRADE_UPGRADE_02, END_PROC );
    }

    // 속성 초기화한 아이템 리스트.
    pkInvenItem->m_vecAttribute = kPacket_.m_kItem.m_vecAttribute;
    pkInvenItem->m_cGradeID =kPacket_.m_kItem.m_cGradeID;
    kPacket_.m_kItem = *pkInvenItem;

    pkUseItem->m_nCount = kPacket_.m_kUseItem.m_nCount;
    pkUseItem->m_nInitCount = kPacket_.m_kUseItem.m_nCount;
    kPacket_.m_vecUseItem.push_back( *pkUseItem );

    if ( pkUseItem && pkUseItem->m_nCount <= 0 ) {
        m_kInventory.RemoveItem( kPacket_.m_kUseItem.m_ItemID, kPacket_.m_kUseItem.m_ItemUID );
    }

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_COORDI_GRADE_UPGRADE_01, kPacket_.m_nOK, 1 );   // 코디 업그레이드할 아이템이 없음.
        ERR_CASE( ERR_COORDI_GRADE_UPGRADE_02, kPacket_.m_nOK, 2 );   // 코디 업그레이드 재료 아이템이 없음.
        ERR_CASE( ERR_COORDI_GRADE_UPGRADE_03, kPacket_.m_nOK, 3 );   // 코디 업그레이드 재료 아이템이 아님.
        ERR_CASE( ERR_COORDI_GRADE_UPGRADE_04, kPacket_.m_nOK, 4 );   // 코디 아이템이 아님.
        ERR_CASE( ERR_COORDI_GRADE_UPGRADE_05, kPacket_.m_nOK, 5 );   // 업그레이드할 등급에 사용 갯수가 설정되어 있지 않음.
        ERR_CASE( ERR_COORDI_GRADE_UPGRADE_06, kPacket_.m_nOK, 6 );   // 변경된 속성 DB에 기록 하는데 오류 발생.
        ERR_CASE( ERR_COORDI_GRADE_UPGRADE_07, kPacket_.m_nOK, 7 );   // 요청한 등급정보가 잘못되었음.
        ERR_CASE( ERR_COORDI_GRADE_UPGRADE_08, kPacket_.m_nOK, 8 );   // 등급 업그레이드 불가능한 아이템임.
        ERR_CASE( ERR_COORDI_GRADE_UPGRADE_09, kPacket_.m_nOK, 9 );   // 업그레이드 할려는 아이템 속성이 없음.
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99; // unknown error
        break;
    }

    SEND_RECEIVED_PACKET( EVENT_COORDI_GRADE_UPGRADE_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"코디 등급 업그레이드 응답 MSG :" << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName)
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kItem.m_vecAttribute.size() )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_COORDI_GRADE_UPGRADE_INFO_REQ )
{
    KEVENT_COORDI_GRADE_UPGRADE_INFO_ACK kPacket;

    SiKItemManager()->m_kGradeDecider.GetCoordiUpgradeInfo( kPacket );

    START_LOG( clog, L"코디 등급 업그레이드 기본 정보 요청, User : " << GetName() )
        << BUILD_LOG( kPacket.m_GradeUpgradeItemID )
        << BUILD_LOG( kPacket.m_mapGradeUseCount.size() )
        << BUILD_LOG( kPacket.m_nEnableItemType )
        << BUILD_LOG( kPacket.m_setUpgradeBanItem.size() )
        << END_LOG;

    SEND_PACKET( EVENT_COORDI_GRADE_UPGRADE_INFO_ACK );
}

IMPL_ON_FUNC( EVENT_SKILL_SCROOL_TRAINING_REQ )
{
    VERIFY_STATE( ( 5, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL,
        KUserFSM::STATE_ROOM, KUserFSM::STATE_SQUARE, KUserFSM::STATE_SQUARE_JOIN ) );

    SET_ERROR( ERR_UNKNOWN );

    KEVENT_SKILL_SCROOL_TRAINING_ACK kPacket;
    kPacket.m_nOK = -99;
    const KCharacterInfo* pkChar = NULL;
    SSkillElement kSkill;
    KChangeUserSkillBroad kChangeUserSkillBroad;
    KItemPtr pkScrollItem; // 스킬 스크롤 아이템 Ptr.
    KItem kSkillScrollItem; // 스킬 스크롤 아이템.

    pkScrollItem = m_kInventory.FindDurationItemByItemUID( kPacket_.m_ItemUID );
    if( !pkScrollItem ) {
        SET_ERR_GOTO( ERR_SKILL_35, END_PROC );
    }

    if( !SiKSkillManager()->GetSkillScroolInfo( pkScrollItem->m_ItemID, kPacket.m_nSkillID ) ) {
        SET_ERR_GOTO( ERR_SKILL_36, END_PROC );
    }

    if( kPacket_.m_nSkillID != kPacket.m_nSkillID ) {
        SET_ERR_GOTO( ERR_SKILL_37, END_PROC );
    }

    // 이 스킬이 존재 하는가?
    if( !SiKSkillManager()->GetSkill( kPacket_.m_nSkillID, kSkill ) ) {
        SET_ERR_GOTO( ERR_SKILL_00, END_PROC );
    }

    // 이미 획득한 스킬인가?
    if( IsTrainedSkill( kSkill ) ) {
        SET_ERR_GOTO( ERR_SKILL_01, END_PROC );
    }

    // 해당 캐릭터가 존재 하는가?
    pkChar = GetCharacterInfo( kSkill.m_cCharType );
    if( !pkChar ) {
        SET_ERR_GOTO( ERR_SKILL_03, END_PROC );
    }

    // 해당 캐릭터 전직의 배운 스킬에 넣어 준다.
    AddTrainedSkill( kSkill );

    // 스킬정보를 방원에게 전송해 줘야 하는지 체크.
    if( SiKSkillManager()->CheckAbilitySkill( kSkill.m_cCharType, kSkill.m_nID ) ) {
        kChangeUserSkillBroad.m_dwUserUID = GetUID();
        GetSkillInfoData( kSkill.m_cCharType, kChangeUserSkillBroad.m_vecTrainedSkill );
        if( m_pkRoom ) {
            m_pkRoom->UpdateSkillInfo( m_strName, kSkill.m_cCharType );

            LIF( m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL_EX_ME, m_strName, KUserEvent::EVENT_SKILL_UPDATE_BROAD, kChangeUserSkillBroad, true ) );
        }
    }

    // 스킬 스크롤 아이템 갯수 차감.
    pkScrollItem->m_nCount -= 1;
    kSkillScrollItem = *pkScrollItem;
    kPacket.m_vecUseItem.push_back( kSkillScrollItem );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );
        ERR_CASE( ERR_SKILL_35,         kPacket.m_nOK, 1 ); // 스킬 스크롤 아이템을 가지고 있지 않음.
        ERR_CASE( ERR_SKILL_36,         kPacket.m_nOK, 2 ); // 스킬 스크롤 아이템 설정이 되지 않음.
        ERR_CASE( ERR_SKILL_37,         kPacket.m_nOK, 3 ); // 클라이언트와 서버가 알고 있는 스킬정보가 틀림.
        ERR_CASE( ERR_SKILL_00,         kPacket.m_nOK, 4 ); // 존재하지 않는 스킬입니다.
        ERR_CASE( ERR_SKILL_01,         kPacket.m_nOK, 5 ); // 이미 획득한 스킬 입니다.
        ERR_CASE( ERR_SKILL_03,         kPacket.m_nOK, 6 ); // 해당 캐릭터가 존재하지 않음.
    default:
        START_LOG( cerr, L"스킬 스크롤 스킬 학습시 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket.m_nOK ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    SEND_PACKET( EVENT_SKILL_SCROOL_TRAINING_ACK );

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << L" 스킬 스크롤 사용 학습 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nSkillID )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nSkillID )
        << BUILD_LOG( kPacket.m_vecUseItem.size() )
        << END_LOG;
}

_IMPL_ON_FUNC( EVENT_NICKNAME_VALIDITY_CHECK_REQ, std::wstring )
{
    KEVENT_NICKNAME_VALIDITY_CHECK_ACK kPacket;
    kPacket.m_nOK = -99;

    if( kPacket_ == m_strNickName ) {
        SET_ERR_GOTO( ERR_NICK_09, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_NICKNAME_VALIDITY_CHECK_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                    kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_NICK_09,               kPacket.m_nOK, 1 ); // 나의 닉네임을 전달했다.
        ERR_CASE( ERR_NICK_10,               kPacket.m_nOK, 2 ); // 존재하지 않는 닉네임을 입력했다.
        ERR_CASE( ERR_NOT_FINISHED_YET,      kPacket.m_nOK, -98 ); // 아직 처리중인 작업
    default:
        START_LOG( cerr, L"닉네임 유효성 체크 요청 처리 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"닉네임 유효성 체크 요청 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_NICKNAME_VALIDITY_CHECK_ACK );
    }
}

IMPL_ON_FUNC( EVENT_NICKNAME_VALIDITY_CHECK_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                    kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_NICK_09,               kPacket_.m_nOK, 1 ); // 나의 닉네임을 전달했다.
        ERR_CASE( ERR_NICK_10,               kPacket_.m_nOK, 2 ); // 존재하지 않는 닉네임을 입력했다.
        ERR_CASE( ERR_NOT_FINISHED_YET,      kPacket_.m_nOK, -98 ); // 아직 처리중인 작업
    default:
        START_LOG( cerr, L"닉네임 유효성 체크 처리중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"닉네임 유효성 체크 요청 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_wstrNickName ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_NICKNAME_VALIDITY_CHECK_ACK );
}

_IMPL_ON_FUNC(EVENT_IN_ROOM_CHANGE_HOST_REQ, DWORD)
{
    if (GetStateID() == KUserFSM::STATE_CHANNEL)
    {
        return;
    }

    if (!m_pkCurrentState->CheckValidState(2, KUserFSM::STATE_ROOM, KUserFSM::STATE_PLAYING))
    {
        START_LOG(cerr, L"Falha ao trocar lider. Name : " << m_strName)
            << L"    Estado : " << GetStateIDString()
            << L"    KUserFSM::STATE_ROOM, KUserFSM::STATE_PLAYING " << END_LOG;

        return;
    }

    JIF(m_pkRoom);
    JIF(m_pkChannel);

    if (!m_pkRoom->ChangeHost(*this, kPacket_))
    {
        START_LOG(cerr, L"Changing room host failed")
            << BUILD_LOG(GetName())
            << BUILD_LOG(m_bGoToGuildZone)
            << END_LOG;
    }

    START_LOG(clog, L"Changing room host success")
        << BUILD_LOG(GetName())
        << END_LOG;
}

IMPL_ON_FUNC(EVENT_COORDIVIEWER_GETSLOT_REQ)
{
    QUEUEING_EVENT_TO_DB(EVENT_COORDIVIEWER_GETSLOT_REQ, kPacket_);
}

IMPL_ON_FUNC(EVENT_COORDIVIEWER_SETSLOT_REQ)
{
    QUEUEING_EVENT_TO_DB(EVENT_COORDIVIEWER_SETSLOT_REQ, kPacket_);
}

IMPL_ON_FUNC(EVENT_COORDIVIEWER_GETSLOT_ACK)
{
    START_LOG(clog, L"Coordiview Slot Equip sent! MapSize: " << kPacket_.m_mapSlotInfo.size()) << END_LOG;
    SEND_COMPRESS_PACKET(EVENT_COORDIVIEWER_GETSLOT_ACK, kPacket_);
}

IMPL_ON_FUNC(EVENT_COORDIVIEWER_SETSLOT_ACK)
{
    START_LOG(clog, L"Coordiview Slot Equip set! iOK: " << kPacket_.m_iOK) << END_LOG;
    SEND_COMPRESS_PACKET(EVENT_COORDIVIEWER_SETSLOT_ACK, kPacket_);
}

IMPL_ON_FUNC( EVENT_USE_HERO_TICKET_REQ )
{
    KDB_EVENT_USE_HERO_TICKET_REQ kDBPacket;
    kDBPacket.m_dwItemUID  =  kPacket_.m_dwItemUID;
    kDBPacket.m_iCharType  =  kPacket_.m_iCharType;
    kDBPacket.m_iDungeonID = kPacket_.m_iDungeonID;
    QUEUEING_EVENT_TO_DB( DB_EVENT_USE_HERO_TICKET_REQ, kDBPacket );
    SET_ERROR( NET_OK );
    START_LOG( clog, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_dwItemID )
        << BUILD_LOG( kPacket_.m_dwItemUID )
        << BUILD_LOG( kPacket_.m_iCharType )
        << BUILD_LOG( kPacket_.m_iDungeonID ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_USE_HERO_TICKET_ACK )
{
    SEND_COMPRESS_PACKET( EVENT_USE_HERO_TICKET_ACK, kPacket_);
}