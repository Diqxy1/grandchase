#include "User.h"
#include <dbg/dbg.hpp>
#include "UserFSM.h"
#include "support_User.h"
#include <boost/algorithm/string.hpp> // to_lower
#include "GameServer.h"
#include "GSSimLayer.h"
#include "AgentConnector.h"
#include "AgitHelper.h"
#include "IPAdvantage.h"
#include "CnConnector.h"
#include "Room.h"
#include "GameDrop.h"
#include "ResultManager.h"
#include "SkillManager.h"
#include "GWCEvent.h"
#include "JustInTime.h"
#include "Log4.h"
#include "CharismasPresent.h"
#include "PostConfig.h"
#include "HeroDungeonManager.h"
#include "ItemManager.h"
#include "Channel.h"
#include "EclipsePlot.h"
#include "ItemTradeManager.h"
#include "LoudMessage.h"
#include "EclipseTimeEvent.h"
#include "SignBoard.h"
#include "EclipseCollect.h"
#include "MonsterCardMix.h"
#include "GCHelper.h"
#include "Songkran.h"
#include "SlangFilter.h"
#include "UserBillBoard.h"
#define CLASS_TYPE  KUser

_IMPL_ON_FUNC( EVENT_ENTER_AGIT_REQ, KEnterAgitReq )
{
    // 아지트 입장.
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 6, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN,
        KUserFSM::STATE_ROOM, KUserFSM::STATE_SQUARE, KUserFSM::STATE_CHANNEL ));

    if ( GetCurrentCharType() != kPacket_.m_cCharType ) {
        START_LOG( cerr, L"서버-클라이언트의 현재 캐릭터가 다르다, User: " << GetName() )
            << END_LOG;
        return;
    }

    //{{ STATE_CHANNELLOBBY 로 상태 변경
    if ( GetStateID() == KUserFSM::STATE_ROOM ) {
        JIF( m_pkRoom );
        LIF( m_pkRoom->Leave( *this, KRoom::LEAVE_MOVE_AGIT ) );

        JIF( m_pkChannel );
        LIF( m_pkChannel->Leave( *this ) );
        START_LOG( clog, L"방에서 아지트 이동요청.. LoginID : " << GetName() ) << END_LOG;
    }

    if ( GetStateID() == KUserFSM::STATE_CHANNEL ) {
        JIF( m_pkChannel );
        LIF( m_pkChannel->Leave( *this ) );
        START_LOG( clog, L"채널에서 아지트 이동요청.. LoginID : " << GetName() ) << END_LOG;
    }

    if( GetStateID() == KUserFSM::STATE_SQUARE) {
        JIF( m_spSquare );
        LIF( LeaveSquare() );
        START_LOG( clog, L"광장에서 아지트 이동요청.. LoginID : " << GetName() ) << END_LOG;
    }
    //}}

    LIF( IsCharExist( kPacket_.m_cCharType ) );
    kPacket_.m_dwEnterUserUID = GetUID();

    int nSID = SiKAgitHelper()->GetAgitSID( kPacket_.m_dwAgitUID );
    if ( nSID < 0 ) {
        // 등록되지않은 SID일 경우, SID문의
        SEND_DATA_TO_AGENT_MASTER( EAG_ENTER_AGIT_SID_REQ, kPacket_ );
        return;
    }

    SEND_RECEIVED_PACKET_TO_AGIT( kPacket_.m_dwAgitUID, EAG_ENTER_AGIT_REQ );

    START_LOG( clog, L"아지트 입장요청.. Login : " << GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( nSID )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_strPassword ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_ENTER_AGIT_NOT, KEnterAgitAck )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN) );

    if ( kPacket_.m_nOK == 0 ) {
        // 내 아지트 입장시, 화분 지급/삭제 확인
        if ( kPacket_.m_kInfo.m_dwUID == GetUID() ) {
            KEVENT_ENTER_AGIT_NOT kDBPacket;
            kDBPacket.m_kEnterAgitAck = kPacket_;
            CheckFlowerPotDrop( kPacket_.m_kInfo.m_dwMapID, kDBPacket.m_vecAddItem, kDBPacket.m_vecEraseItem );
            // 삭제하거나 추가할 화분 아이템이 있으면 DB요청
            if ( !kDBPacket.m_vecAddItem.empty() || !kDBPacket.m_vecEraseItem.empty() ) {
                QUEUEING_EVENT_TO_DB( EVENT_ENTER_AGIT_NOT, kDBPacket );
                return;
            }
        }
    }

    QueueingEvent( KUserEvent::EVENT_ENTER_AGIT_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EVENT_ENTER_AGIT_ACK, KEnterAgitAck )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN));

    // 친구만 공개일 경우, 체크할 수 있는 시점이 현재는 이곳 뿐이다.
    if ( kPacket_.m_kInfo.m_ucOpenType == KAgitInfo::OT_FRIEND ) {
        if ( kPacket_.m_kInfo.m_dwUID != GetUID() ) {
            std::vector<DWORD>::iterator vit;
            vit = std::find( m_vecOnlineFriends.begin(), m_vecOnlineFriends.end(), kPacket_.m_kInfo.m_dwUID );
            if ( vit == m_vecOnlineFriends.end() ) {
                kPacket_.m_nOK = 6; // ERR_AGIT_06
                START_LOG( cerr, L"들어가려는 아지트가 친구만 공개인데, 내 친구리스트에 없는 UID다. AgitUID : " << kPacket_.m_kInfo.m_dwUID << L", OnlineFriend size : " << m_vecOnlineFriends.size() ) << END_LOG;
            }
        }
    }

    // 실패처리
    if ( kPacket_.m_nOK != 0 ) {
        if ( GetStateID() == KUserFSM::STATE_AGIT ) {
            // 아지트 상태일때는 기존 아지트로 되돌린다.
            StateTransition( KUserFSM::INPUT_ENTER_AGIT );
        }
        else {
            StateTransition( KUserFSM::INPUT_TO_CHANNELLOBBY );
            m_dwStayAgitUID = 0;
        }
        goto END_PROC;
    }

    // 성공처리
    StateTransition( KUserFSM::INPUT_TO_AGIT_JOIN );
    m_dwStayAgitUID = kPacket_.m_kInfo.m_dwUID;

    // 아지트 채널 개수 전달
    kPacket_.m_nAigtChannelCount = static_cast<int>( SiKAgentConnector()->GetSIDCount() );
    kPacket_.m_nAigtChannelCount = std::max<int>( kPacket_.m_nAigtChannelCount, 0 );
    // 아지트 상태 기록
    m_ucAgitOpenType = kPacket_.m_kInfo.m_ucOpenType;
    m_strAgitPassword = kPacket_.m_kInfo.m_strPassword;

    if ( kPacket_.m_kInfo.m_dwUID == GetUID() ) {
        // 획득한 화분 아이템 넣기
        if ( false == kPacket_.m_vecRewardItem.empty() ) {
            m_kInventory.AddItemList( kPacket_.m_vecRewardItem );
        }

        // 화분 아이템 제거
        if ( false == kPacket_.m_vecEraseItem.empty() ) {
            RemoveItemFromInven( kPacket_.m_vecEraseItem );

            // 오브젝트 정보에서 제거
            std::vector<KAgitObjectPos>::iterator vit;
            vit = kPacket_.m_kInfo.m_vecObjectPos.begin();
            while ( vit != kPacket_.m_kInfo.m_vecObjectPos.end() ) {
                std::vector<GCITEMUID>::iterator vitUID;
                vitUID = std::find( kPacket_.m_vecEraseItem.begin(), kPacket_.m_vecEraseItem.end(), vit->m_UID );
                if ( vitUID != kPacket_.m_vecEraseItem.end() ) {
                    vit = kPacket_.m_kInfo.m_vecObjectPos.erase( vit );
                    continue;
                }

                ++vit;
            }

            KEAG_DELETE_FOR_AGIT_NOT kAgPacket;
            kAgPacket.m_dwAgitUID = GetUID();
            kAgPacket.m_vecDeleteObject = kPacket_.m_vecEraseItem;
            int nSID = SiKAgitHelper()->GetAgitSID( kAgPacket.m_dwAgitUID );
            SiKAgentConnector()->SendPacket( nSID, KAgentEvent::EAG_DELETE_FOR_AGIT_NOT, kAgPacket );
        }
    }

END_PROC:
    // for Log
    in_addr inTcp;
    inTcp.S_un.S_addr = kPacket_.m_prTRServerIpPort.first;
    in_addr inUdp;
    inUdp.S_un.S_addr = kPacket_.m_prURServerIpPort.first;

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"아지트 입장요청 결과. Result : " << kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kInfo.m_dwUID )
        << BUILD_LOG( kPacket_.m_kInfo.m_strName )
        << BUILD_LOGc( kPacket_.m_kInfo.m_dwMapID )
        << BUILD_LOGc( kPacket_.m_kInfo.m_ucOpenType )
        << BUILD_LOGc( kPacket_.m_kInfo.m_ucInviteType )
        << BUILD_LOGc( kPacket_.m_kInfo.m_ucMaxUser )
        << BUILD_LOG( kPacket_.m_kInfo.m_nTodayVisiter )
        << BUILD_LOG( kPacket_.m_kInfo.m_nTotalVisiter )
        << BUILD_LOGc( kPacket_.m_kInfo.m_kProfile.m_ucTheme )
        << BUILD_LOG( kPacket_.m_kInfo.m_kProfile.m_strProfile )
        << BUILD_LOG( kPacket_.m_kInfo.m_vecObjectPos.size() )
        << BUILD_LOG( kPacket_.m_kInfo.m_mapCharacterPos.size() )
        << BUILD_LOGtm( CTime(kPacket_.m_kInfo.m_tmLastCleaning) )
        << BUILD_LOG( kPacket_.m_kInfo.m_strPassword )
        << BUILD_LOG( inet_ntoa(inTcp) )
        << BUILD_LOG( kPacket_.m_prTRServerIpPort.second )
        << BUILD_LOG( inet_ntoa(inUdp) )
        << BUILD_LOG( kPacket_.m_prURServerIpPort.second )
        << BUILD_LOG( kPacket_.m_mapFlowerPot.size() )
        << BUILD_LOG( kPacket_.m_nAigtChannelCount )
        << BUILD_LOG( kPacket_.m_vecRewardItem.size() )
        << BUILD_LOG( kPacket_.m_vecEraseItem.size() )
        << BUILD_LOG( sizeof(kPacket_) ) << END_LOG;

    SEND_COMPRESS_PACKET( EVENT_ENTER_AGIT_ACK, kPacket_ );
}

IMPL_ON_FUNC( EVENT_ENTER_AGIT_BROAD )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    if ( m_dwStayAgitUID != kPacket_.m_dwAgitUID ) {
        START_LOG( cwarn, L"현재 내가 있는 아지트와 다른 아지트의 입장알림 패킷.. Login : " << GetName() )
            << BUILD_LOG( kPacket_.m_dwAgitUID )
            << BUILD_LOG( m_dwStayAgitUID ) << END_LOG;
        return;
    }

    START_LOG( clog, L"내가 있는 아지트에 새로운 유저 입장 알림. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_kUserInfo.m_dwUID )
        << BUILD_LOG( kPacket_.m_kUserInfo.m_strNick )
        << BUILD_LOGc( kPacket_.m_kUserInfo.m_cCharType )
        << BUILD_LOGc( kPacket_.m_kUserInfo.m_cPromotion )
        << BUILD_LOGc( kPacket_.m_kUserInfo.m_kPetInfo.m_dwPetID ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_ENTER_AGIT_BROAD );
}

IMPL_ON_FUNC_NOPARAM( EVENT_AGIT_LOADING_COMPLETE_REQ )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 1, KUserFSM::STATE_AGIT_JOIN ) );

    KEAG_AGIT_LOADING_COMPLETE_REQ kAgPacket;
    kAgPacket.m_dwAgitUID = m_dwStayAgitUID;
    kAgPacket.m_kUserInfo.m_prUserIPPort.first = static_cast<DWORD>( GetIP() );
    kAgPacket.m_kUserInfo.m_prUserIPPort.second = GetPort();

    KAgitLoadingCompleteAck kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwAgitUID = m_dwStayAgitUID;
    kPacket.m_dwUserUID = GetUID();

    SET_ERROR( ERR_UNKNOWN );

    // 입장할 아지트 정보가 정확하지 않음.
    if ( m_dwStayAgitUID == 0 ) {
        SET_ERR_GOTO( ERR_AGIT_02, END_PROC );
    }

    // 아지트 입장캐릭터 정보
    if ( false == GetAgitUserInfo( kAgPacket.m_kUserInfo ) ) {
        SET_ERR_GOTO( ERR_AGIT_03, END_PROC );
    }

    int nSID = SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID );
    SiKAgentConnector()->SendPacket( nSID, KAgentEvent::EAG_AGIT_LOADING_COMPLETE_REQ, kAgPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,      kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_AGIT_02, kPacket.m_nOK, 2 ); // 입장할 아지트 정보가 정확하지 않음.
        ERR_CASE( ERR_AGIT_03, kPacket.m_nOK, 3 ); // 아지트 입장할 캐릭터 정보가 없음.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( GetName() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    in_addr in; // for Log
    in.S_un.S_addr = kAgPacket.m_kUserInfo.m_prUserIPPort.first;

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"아지트 로딩완료.. 입장요청. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( nSID )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( kAgPacket.m_kUserInfo.m_dwUID )
        << BUILD_LOG( kAgPacket.m_kUserInfo.m_strNick )
        << BUILD_LOGc( kAgPacket.m_kUserInfo.m_cCharType )
        << BUILD_LOGc( kAgPacket.m_kUserInfo.m_cPromotion )
        << BUILD_LOG( kAgPacket.m_kUserInfo.m_dwLevel )
        << BUILD_LOG( kAgPacket.m_kUserInfo.m_vecEquipItems.size() )
        << BUILD_LOG( kAgPacket.m_kUserInfo.m_vecLookEquips.size() )
        << BUILD_LOG( inet_ntoa(in) )
        << BUILD_LOG( KncUtil::toWideString( GetIPStr() ) )
        << BUILD_LOG( kAgPacket.m_kUserInfo.m_prUserIPPort.second )
        << BUILD_LOG( GetPort() )
        << BUILD_LOG( kAgPacket.m_kUserInfo.m_mapSkills.size() )
        << BUILD_LOG( kAgPacket.m_kUserInfo.m_kPetInfo.m_dwPetID ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_COMPRESS_PACKET( EVENT_AGIT_LOADING_COMPLETE_ACK, kPacket );
    }
}

_IMPL_ON_FUNC( EVENT_AGIT_LOADING_COMPLETE_ACK, KAgitLoadingCompleteAck )
{
    CHECK_AGIT_ENABLE;
    if ( kPacket_.m_nOK != 0 ) {
        // 실패처리
        if ( GetStateID() == KUserFSM::STATE_AGIT ) {
            // 아지트 상태일때는 기존 아지트로 되돌린다.
            StateTransition( KUserFSM::INPUT_ENTER_AGIT );
        }
        else {
            StateTransition( KUserFSM::INPUT_TO_CHANNELLOBBY );
            m_dwStayAgitUID = 0;
        }
        goto END_PROC;
    }

    // 성공처리
    StateTransition( KUserFSM::INPUT_ENTER_AGIT );
    m_dwStayAgitUID = kPacket_.m_dwAgitUID;
    SetTick( AGIT_STAY );

    if ( m_dwStayAgitUID == GetUID() ) {
        m_bAgitNewUser = false;
    }

    if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
        //START_LOG( cwarn, L"-- Agit User (size:" << kPacket_.m_mapAgitUser.size() << L") --"  ) << END_LOG;

        //std::map<DWORD,KAgitUserInfo>::const_iterator cmit;
        //for ( cmit = kPacket_.m_mapAgitUser.begin() ; cmit != kPacket_.m_mapAgitUser.end() ; ++cmit ) {
        //    std::Warn << L"UserUID:" << cmit->second.m_dwUID
        //        << L",Nick:" << cmit->second.m_strNick
        //        << L",Char:" << (int)cmit->second.m_cCharType
        //        << L",Promotion:" << (int)cmit->second.m_cPromotion
        //        << L",PetID:" << (int)cmit->second.m_kPetInfo.m_dwPetID << dbg::endl;
        //}
        //std::Warn << dbg::endl;
    }

END_PROC:
    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"아지트 로딩완료후 입장처리 결과. Result : " << kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_mapAgitUser.size() ) << END_LOG;

    SEND_COMPRESS_PACKET( EVENT_AGIT_LOADING_COMPLETE_ACK, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EVENT_AGIT_LOADING_FAIL_NOT )
{
    CHECK_AGIT_ENABLE;
    START_LOG( cwarn, L"아지트 로딩실패.. Login : " << GetName() << L", AgitUID : " << m_dwStayAgitUID ) << END_LOG;
    // 상태 초기화
    if ( GetStateID() == KUserFSM::STATE_AGIT ) {
        // 아지트 상태일때는 기존 아지트로 되돌린다.
        StateTransition( KUserFSM::INPUT_ENTER_AGIT );
    }
    else {
        StateTransition( KUserFSM::INPUT_TO_CHANNELLOBBY );
        m_dwStayAgitUID = 0;
    }
}

IMPL_ON_FUNC_NOPARAM( EVENT_LEAVE_AGIT_REQ )
{
    CHECK_AGIT_ENABLE;
    _VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ));

    // 아지트 퇴장은 무조건 성공 처리..
    LeaveAgit();

    //아지트 들어 오기 전에 확실히 채널 로비 이다.
    JIF( m_pkChannel );

    // 혹시 라도 채널 로비가 아닌지 확인 한번.
    if ( m_pkChannel != KChannel::ms_spChannelLobby.get() ) {
        //만약 채널 로비가 아니면 한번 뻬 즌다
        m_pkChannel->Leave( *this ); // 상태는 채널 로비
    }
    else {
        StateTransition( KUserFSM::INPUT_TO_CHANNELLOBBY );
    }

    // LeaveAgit()에서 아지트UID가 0이면 패킷을 안보내니까
    // 바로 채널로비로 상태변경하고, ack패킷 전달.
    if ( m_dwStayAgitUID == 0 ) {
        StateTransition( KUserFSM::INPUT_TO_CHANNELLOBBY );
        SEND_ID( EVENT_LEAVE_AGIT_ACK );
    }

    START_LOG( clog, L"아지트 퇴장요청.. Login : " << GetName() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( GetUID() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_LEAVE_AGIT_ACK, DWORD )
{
    CHECK_AGIT_ENABLE;

    // 아지트서버 퇴장처리후, 상태변경
    if ( GetStateID() != KUserFSM::STATE_CHANNELLOBBY ) {
        StateTransition( KUserFSM::INPUT_TO_CHANNELLOBBY );
    }
    m_dwStayAgitUID = 0;

    START_LOG( clog, L"아지트 정상 퇴장처리.. Login : " << GetName() << L", AgitUID : " << kPacket_ ) << END_LOG;

    SEND_ID( EVENT_LEAVE_AGIT_ACK );
}

IMPL_ON_FUNC( EVENT_LEAVE_AGIT_BROAD )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    if ( m_dwStayAgitUID != kPacket_.m_dwAgitUID ) {
        START_LOG( cwarn, L"현재 내가 있는 아지트와 다른 아지트의 퇴장알림 패킷.. Login : " << GetName() )
            << BUILD_LOG( kPacket_.m_dwAgitUID )
            << BUILD_LOG( m_dwStayAgitUID ) << END_LOG;
        return;
    }

    START_LOG( clog, L"내가 있는 아지트에 유저 퇴장 알림. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_dwUserUID ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_LEAVE_AGIT_BROAD );
}

IMPL_ON_FUNC( EVENT_AGIT_VISITER_COUNT_BROAD )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    if ( m_dwStayAgitUID != kPacket_.m_dwAgitUID ) {
        START_LOG( cwarn, L"현재 내가 있는 아지트와 다른 아지트의 방문자수 알림 패킷.. Login : " << GetName() )
            << BUILD_LOG( kPacket_.m_dwAgitUID )
            << BUILD_LOG( m_dwStayAgitUID ) << END_LOG;
        return;
    }

    START_LOG( clog, L"내가 있는 아지트에 방문자수 알림. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_nTodayVisiterCount )
        << BUILD_LOG( kPacket_.m_nTotalVisiterCount ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_AGIT_VISITER_COUNT_BROAD );
}

IMPL_ON_FUNC_NOPARAM( EVENT_AGIT_MAP_CATALOGUE_REQ )
{
    CHECK_AGIT_ENABLE;

    KEVENT_AGIT_MAP_CATALOGUE_ACK kPacket;
    kPacket.m_PayItemID = 0;

    SiKAgitHelper()->GetMapCatalogue( kPacket.m_PayItemID, kPacket.m_mapMapList );
    SEND_PACKET( EVENT_AGIT_MAP_CATALOGUE_ACK );

    START_LOG( clog, L"아지트 맵 카탈로그 전달. Login : " << GetName() )
        << BUILD_LOG( kPacket.m_PayItemID )
        << BUILD_LOG( kPacket.m_mapMapList.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_BUY_AGIT_MAP_REQ, DWORD )
{
    // 아지트 맵구매
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 1, KUserFSM::STATE_AGIT ));

    KBuyAgitMapAck kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwUserUID = GetUID();
    kPacket.m_dwMapID = kPacket_;

    KEAG_BUY_AGIT_MAP_REQ kAgPacket;
    kAgPacket.m_kPayItem.m_ItemID = SiKAgitHelper()->GetFlowerItemID();
    kAgPacket.m_nPrice = 0;
    kAgPacket.m_dwUserUID = GetUID();
    kAgPacket.m_dwMapID =  kPacket_;

    KAgitMap kInfo;
    KItemPtr pkItem;

    SET_ERROR( ERR_UNKNOWN );

    // 아지트 맵구매시 자기 아지트에 있어야함.
    if ( m_dwStayAgitUID != GetUID() ) {
        SET_ERR_GOTO( ERR_AGIT_MAP_01, END_PROC );
    }

    // 구매할 맵 정보가 있는지
    if ( false == SiKAgitHelper()->GetCurrentMapInfo( kAgPacket.m_dwMapID, kInfo ) ) {
        SET_ERR_GOTO( ERR_AGIT_MAP_02, END_PROC );
    }
    // 가격 설정
    kAgPacket.m_nPrice = kInfo.m_nPrice;

    // 화폐 아이템이 없음
    pkItem = m_kInventory.FindItemByItemID( kAgPacket.m_kPayItem.m_ItemID );
    if ( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_AGIT_MAP_03, END_PROC );
    }

    // 화폐가 부족함
    if ( pkItem->m_nCount < kAgPacket.m_nPrice ) {
        SET_ERR_GOTO( ERR_AGIT_MAP_04, END_PROC );
    }

    // 화폐 아이템 보유정보
    kAgPacket.m_kPayItem = *pkItem;

    // 구매요청
    int nSID = SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID );
    SiKAgentConnector()->SendPacket( nSID, KAgentEvent::EAG_BUY_AGIT_MAP_REQ, kAgPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,          kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_AGIT_MAP_01, kPacket.m_nOK, 1 ); // 현재 있는 아지트가 내 아지트가 아님.
        ERR_CASE( ERR_AGIT_MAP_02, kPacket.m_nOK, 2 ); // 구매할 맵 정보가 서버에 없음.
        ERR_CASE( ERR_AGIT_MAP_03, kPacket.m_nOK, 3 ); // 화폐 아이템이 없음.
        ERR_CASE( ERR_AGIT_MAP_04, kPacket.m_nOK, 4 ); // 화폐 수량이 부족함.

    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( GetName() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"아지트 맵구매 요청. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kAgPacket.m_dwMapID )
        << BUILD_LOG( kAgPacket.m_nPrice )
        << BUILD_LOG( kAgPacket.m_kPayItem.m_ItemID )
        << BUILD_LOG( kAgPacket.m_kPayItem.m_ItemUID )
        << BUILD_LOG( kAgPacket.m_kPayItem.m_nCount ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_BUY_AGIT_MAP_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_BUY_AGIT_MAP_NOT, KBuyAgitMapAck )
{
    CHECK_AGIT_ENABLE;

    if ( kPacket_.m_nOK == 0 ) {
        // 이사시에 화분 추가지급 확인
        KEVENT_BUY_AGIT_MAP_NOT kDBPacket;
        kDBPacket.m_kBuyAgitMapAck = kPacket_;
        CheckFlowerPotDrop( kPacket_.m_dwMapID, kDBPacket.m_vecAddItem, kDBPacket.m_vecEraseItem );
        // 삭제하거나 추가할 화분 아이템이 있으면 DB요청
        if ( !kDBPacket.m_vecAddItem.empty() || !kDBPacket.m_vecEraseItem.empty() ) {
            QUEUEING_EVENT_TO_DB( EVENT_BUY_AGIT_MAP_NOT, kDBPacket );
            return;
        }
    }

    QueueingEvent( KUserEvent::EVENT_BUY_AGIT_MAP_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EVENT_BUY_AGIT_MAP_ACK, KBuyAgitMapAck )
{
    CHECK_AGIT_ENABLE;
    KItemPtr pkItem;

    if ( kPacket_.m_nOK != 0 ) {
        goto END_PROC;
    }

    pkItem = m_kInventory.FindItemByItemUID( kPacket_.m_kPayItem.m_ItemUID );
    if ( pkItem == NULL ) {
        START_LOG( cerr, L"맵 구매후, 화폐 아이템을 찾을 수 없음. Login : " << GetName() ) << END_LOG;
        goto END_PROC;
    }

    // 화폐 수량 갱신
    if ( pkItem->m_nCount > 0 ) {
        pkItem->m_nCount = kPacket_.m_kPayItem.m_nCount;
        pkItem->m_nInitCount = kPacket_.m_kPayItem.m_nInitCount;
    }

    if ( pkItem->m_nCount <= 0 ) {
        m_kInventory.RemoveItem( kPacket_.m_kPayItem.m_ItemID, kPacket_.m_kPayItem.m_ItemUID );
    }

    // 획득한 화분 아이템 넣기
    if ( false == kPacket_.m_vecRewardItem.empty() ) {
        m_kInventory.AddItemList( kPacket_.m_vecRewardItem );
    }

    // 화분 아이템 제거
    if ( false == kPacket_.m_vecEraseItem.empty() ) {
        RemoveItemFromInven( kPacket_.m_vecEraseItem );

        // 오브젝트 정보에서 제거
        KEAG_DELETE_FOR_AGIT_NOT kAgPacket;
        kAgPacket.m_dwAgitUID = GetUID();
        kAgPacket.m_vecDeleteObject = kPacket_.m_vecEraseItem;
        int nSID = SiKAgitHelper()->GetAgitSID( kAgPacket.m_dwAgitUID );
        SiKAgentConnector()->SendPacket( nSID, KAgentEvent::EAG_DELETE_FOR_AGIT_NOT, kAgPacket );
    }

END_PROC:
    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"아지트 맵구매 결과 받음. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_dwMapID )
        << BUILD_LOG( kPacket_.m_kPayItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kPayItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kPayItem.m_nCount )
        << BUILD_LOGtm( CTime(kPacket_.m_tmLastCleanTime) )
        << BUILD_LOG( kPacket_.m_vecRewardItem.size() )
        << BUILD_LOG( kPacket_.m_vecEraseItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_BUY_AGIT_MAP_ACK );
}

IMPL_ON_FUNC( EVENT_CHANGE_AGIT_MAP_BROAD )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    if ( m_dwStayAgitUID != kPacket_.m_dwAgitUID ) {
        START_LOG( cwarn, L"현재 내가 있는 아지트와 다른 아지트의 맵변경 알림. Login : " << GetName() )
            << BUILD_LOG( kPacket_.m_dwAgitUID )
            << BUILD_LOG( m_dwStayAgitUID )
            << BUILD_LOG( kPacket_.m_dwMapID ) << END_LOG;
        return;
    }

    START_LOG( clog, L"내가 있는 아지트의 맵변경 알림. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_dwMapID ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CHANGE_AGIT_MAP_BROAD );
}

_IMPL_ON_FUNC( EVENT_SET_AGIT_OPTION_REQ, KAgitOption )
{
    // 아지트 옵션설정.
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 1, KUserFSM::STATE_AGIT ));
    kPacket_.m_dwAgitUID = GetUID();

    std::wstring strNameCopy;
    KSetAgitOptionAck kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    // 자기 아지트에 있는가
    if ( m_dwStayAgitUID != GetUID() ) {
        SET_ERR_GOTO( ERR_AGIT_OPT_01, END_PROC );
    }

    // 아지트명-불량단어
    strNameCopy = boost::to_lower_copy( kPacket_.m_strName );
    if( SiKSlangFilter()->HasSlang( strNameCopy ) ) {
        SET_ERR_GOTO( ERR_AGIT_OPT_02, END_PROC );
    }

    
    // 프로필-불량단어
    strNameCopy = boost::to_lower_copy( kPacket_.m_strProfile );
    if( SiKSlangFilter()->HasSlang( strNameCopy ) ) {
        SET_ERR_GOTO( ERR_AGIT_OPT_04, END_PROC );
    }

    // 암호-쿼리문자
    if( SiKSlangFilter()->IsInQueryInvalidChar( kPacket_.m_strPassword ) ) {
        SET_ERR_GOTO( ERR_AGIT_OPT_06, END_PROC );
    }

    // 아지트명 글자수 제한
    if ( kPacket_.m_strName.size() > SiKAgitHelper()->GetMaxAgitNameLength() ) {
        SET_ERR_GOTO( ERR_AGIT_OPT_08, END_PROC );
    }

    // 입장제한 인원수 체크
    if ( kPacket_.m_ucMaxUser > SiKAgitHelper()->GetMaxUserCount() ) {
        SET_ERR_GOTO( ERR_AGIT_OPT_09, END_PROC );
    }

    // 오픈설정
    if ( kPacket_.m_ucOpenType > KAgitInfo::OT_FRIEND ) {
        SET_ERR_GOTO( ERR_AGIT_OPT_10, END_PROC );
    }

    // 패스워드 길이
    if ( kPacket_.m_strPassword.size() > SiKAgitHelper()->GetMaxPasswordLength() ) {
        SET_ERR_GOTO( ERR_AGIT_OPT_11, END_PROC );
    }

    // 초대설정
    if ( kPacket_.m_ucInviteType > KAgitInfo::IT_FRIEND ) {
        SET_ERR_GOTO( ERR_AGIT_OPT_12, END_PROC );
    }
    m_ucAgitInviteOption = kPacket_.m_ucInviteType;

    // 프로필 길이
    if ( kPacket_.m_strProfile.size() > SiKAgitHelper()->GetMaxProfileLength() ) {
        SET_ERR_GOTO( ERR_AGIT_OPT_13, END_PROC );
    }

    // 아지트명-쿼리문자
    boost::replace_all( kPacket_.m_strName, L"'", L"''" );

    // 프로필-쿼리문자
    boost::replace_all( kPacket_.m_strProfile, L"'", L"''" );

    // 그외 검사는 에이전트에서 진행
    int nSID = SiKAgitHelper()->GetAgitSID(m_dwStayAgitUID);
    SiKAgentConnector()->SendPacket( nSID, KAgentEvent::EAG_SET_AGIT_OPTION_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,          kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_AGIT_OPT_01, kPacket.m_nOK, 1 ); // 현재 있는 아지트가 내 아지트가 아님.
        ERR_CASE( ERR_AGIT_OPT_02, kPacket.m_nOK, 2 ); // 아지트명에 사용할 수 없는 단어나 있음.
        ERR_CASE( ERR_AGIT_OPT_03, kPacket.m_nOK, 3 ); // 아지트명에 사용할 수 없는 문자가 있음.
        ERR_CASE( ERR_AGIT_OPT_04, kPacket.m_nOK, 4 ); // 프로필에 사용할 수 없는 단어나 있음.
        ERR_CASE( ERR_AGIT_OPT_05, kPacket.m_nOK, 5 ); // 프로필에 사용할 수 없는 문자가 있음.
        ERR_CASE( ERR_AGIT_OPT_06, kPacket.m_nOK, 6 ); // 암호에 사용할 수 없는 문자가 있음.
        ERR_CASE( ERR_AGIT_OPT_08, kPacket.m_nOK, 8 ); // 아지트명 길이제한을 초과함
        ERR_CASE( ERR_AGIT_OPT_09, kPacket.m_nOK, 9 ); // 입장제한 인원수를 초과함
        ERR_CASE( ERR_AGIT_OPT_10, kPacket.m_nOK, 10 ); // 정의되지않은 오픈설정
        ERR_CASE( ERR_AGIT_OPT_11, kPacket.m_nOK, 11 ); // 패스워드 길이제한을 초과함
        ERR_CASE( ERR_AGIT_OPT_12, kPacket.m_nOK, 12 ); // 정의되지않은 초대설정
        ERR_CASE( ERR_AGIT_OPT_13, kPacket.m_nOK, 13 ); // 프로필 길이제한을 초과함
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( GetName() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"아지트 옵션변경 요청. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( kPacket_.m_strName )
        << BUILD_LOGc( kPacket_.m_ucMaxUser )
        << BUILD_LOGc( kPacket_.m_ucOpenType )
        << BUILD_LOG( kPacket_.m_strPassword )
        << BUILD_LOGc( kPacket_.m_ucInviteType )
        << BUILD_LOG( kPacket_.m_strProfile )
        << BUILD_LOG( SiKAgitHelper()->GetMaxAgitNameLength() )
        << BUILD_LOG( SiKAgitHelper()->GetMaxUserCount() )
        << BUILD_LOG( SiKAgitHelper()->GetMaxPasswordLength() )
        << BUILD_LOG( SiKAgitHelper()->GetMaxProfileLength() ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_SET_AGIT_OPTION_ACK );
    }
}

IMPL_ON_FUNC_NOPARAM( EVENT_AGIT_STORE_CATALOG_REQ )
{
    CHECK_AGIT_ENABLE;

    KEVENT_AGIT_STORE_CATALOG_ACK kPacket;
    SiKAgitHelper()->m_kAgitStoreItems.GetCatalogs( kPacket.m_mapItemList );

    SEND_COMPRESS_PACKET( EVENT_AGIT_STORE_CATALOG_ACK, kPacket );

    START_LOG( clog, L"아지트상점 아이템 리스트 전달. Login : " << GetName() )
        << BUILD_LOG( kPacket.m_mapItemList.size() )
        << BUILD_LOG( kPacket.m_mapItemPos.size() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_AGIT_STORE_MATERIAL_REQ )
{
    CHECK_AGIT_ENABLE;

    std::map<PAIR_USHORT_DWORD,std::vector<KDropItemInfo> > mapMaterialList;
    mapMaterialList.clear();

    std::set<PAIR_USHORT_DWORD>::iterator sit;
    for ( sit = kPacket_.begin() ; sit != kPacket_.end() ; ++sit ) {
        std::vector<KDropItemInfo> vecMaterial;
        if ( false == SiKAgitHelper()->m_kAgitStoreItems.GetMaterials( *sit, vecMaterial ) ) {
            START_LOG( cerr, L"해당 아이템의 구매재료가 등록되어있지 않음. ItemID : " << (*sit).second ) << END_LOG;
            continue;
        }

        //if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
        //    std::Warn << L"-- Agit Store Material (size:" << vecMaterial.size() << L") --" << dbg::endl
        //        << L"Index : " << (*sit).first << L", ItemID" << (*sit).second << dbg::endl;

        //    std::vector<KDropItemInfo>::const_iterator cvit;
        //    for ( cvit = vecMaterial.begin() ; cvit != vecMaterial.end() ; ++cvit ) {
        //        std::Warn << L"(" << cvit->m_ItemID << L", " << cvit->m_nDuration << L"," << cvit->m_nPeriod << L"),";
        //    }
        //    std::Warn << dbg::endl;
        //}

        mapMaterialList[ *sit ].swap( vecMaterial );
    }

    SEND_COMPRESS_PACKET( EVENT_AGIT_STORE_MATERIAL_ACK, mapMaterialList );

    _LOG_SUCCESS( !mapMaterialList.empty(), L"아지트상점 아이템 구매재료 리스트 전달." )
        << BUILD_LOG( kPacket_.size() )
        << BUILD_LOG( mapMaterialList.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_BUY_FOR_AGIT_REQ, PAIR_USHORT_DWORD )
{
    CHECK_AGIT_ENABLE;

    std::vector<KDropItemInfo> vecMaterials;
    std::vector<KDropItemInfo>::iterator vit;

    KDB_EVENT_BUY_FOR_AGIT_REQ kDBPacket;
    kDBPacket.m_kCatalog.m_Index = kPacket_.first;
    kDBPacket.m_kCatalog.m_ItemID = kPacket_.second;

    KEVENT_BUY_FOR_AGIT_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nGP = GetGP();

    bool bLookItem = false;
    bool bAgitObject = false;
    int nItemInvenCount = 0;

    SET_ERROR( ERR_UNKNOWN );

    // 구매 아이템 정보가 없음
    if ( false == SiKAgitHelper()->m_kAgitStoreItems.GetCatalog( kPacket_, kDBPacket.m_kCatalog ) ) {
        SET_ERR_GOTO( ERR_AGIT_STORE_01, END_PROC );
    }

    // 구매 아이템 재료정보가 없음
    if ( false == SiKAgitHelper()->m_kAgitStoreItems.GetMaterials( kPacket_, vecMaterials ) ) {
        SET_ERR_GOTO( ERR_AGIT_STORE_02, END_PROC );
    }

    for ( vit = vecMaterials.begin() ; vit != vecMaterials.end() ; ++vit ) {
        // 구매재료 아이템이 인벤에 없음.
        KItemPtr pkItem = m_kInventory.FindItemByItemID( vit->m_ItemID );
        if ( pkItem == NULL ) {
            SET_ERR_GOTO( ERR_AGIT_STORE_03, END_PROC );
        }

        if ( vit->m_nDuration == KItem::UNLIMITED_ITEM ) {
            // 구매재료 영구아이템이 없음.
            if ( pkItem->m_nCount != KItem::UNLIMITED_ITEM ||
                pkItem->m_nInitCount != KItem::UNLIMITED_ITEM ) {
                    SET_ERR_GOTO( ERR_AGIT_STORE_04, END_PROC );
            }

            kDBPacket.m_vecDelMaterials.push_back( pkItem->m_ItemUID );
        }
        else {
            // 구매재료 수량아이템의 수량이 부족함.
            if ( pkItem->m_nCount < vit->m_nDuration ) {
                SET_ERR_GOTO( ERR_AGIT_STORE_05, END_PROC );
            }
        }
    }

    // 아지트아이템 구매시 충분한 인벤 공간이 없음.
    bAgitObject = kDBPacket.m_kCatalog.CheckItemFlag( KManufactureItem::IF_AGIT_OBJ );
    if ( bAgitObject ) {
        if ( m_kInventory.GetAgitObjectItemNum() >= SiKAgitHelper()->GetMaxInvenSize() ) {
            SET_ERR_GOTO( ERR_AGIT_STORE_06, END_PROC );
        }
    }

    // 코디아이템일 경우, 충분한 인벤 공간이 없음.
    bLookItem = kDBPacket.m_kCatalog.CheckItemFlag( KManufactureItem::IF_LOOK );
    if ( bLookItem ) {
        if ( m_kInventory.GetLookItemNum() >= SiKLookEquipment()->GetMaxInvenSize() ) {
            SET_ERR_GOTO( ERR_AGIT_STORE_07, END_PROC );
        }
    }

    // 일반 아이템 구매시 충분한 인벤 공간이 없음.
    if ( !bLookItem && !bAgitObject ) {
        if( m_kInventory.GetTotalEnableSize() >= GetTrueInvenCapacity() ) {
            SET_ERR_GOTO( ERR_AGIT_STORE_08, END_PROC );
        }
    }

    // 인벤토리 보유량 제한 확인
    nItemInvenCount = m_kInventory.GetItemCount( kDBPacket.m_kCatalog.m_ItemID );
    if ( kDBPacket.m_kCatalog.m_nInvelLimit > 0 && nItemInvenCount >= kDBPacket.m_kCatalog.m_nInvelLimit ) {
        SET_ERR_GOTO( ERR_AGIT_STORE_12, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_BUY_FOR_AGIT_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,            kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_AGIT_STORE_01, kPacket.m_nOK, 1 ); // 구매 아이템 정보가 없음.
        ERR_CASE( ERR_AGIT_STORE_02, kPacket.m_nOK, 2 ); // 구매 아이템 재료정보가 없음.
        ERR_CASE( ERR_AGIT_STORE_03, kPacket.m_nOK, 3 ); // 구매 재료 아이템이 인벤에 없음.
        ERR_CASE( ERR_AGIT_STORE_04, kPacket.m_nOK, 4 ); // 구매재료 영구아이템이 없음.
        ERR_CASE( ERR_AGIT_STORE_05, kPacket.m_nOK, 5 ); // 구매재료 수량아이템의 수량이 부족함.
        ERR_CASE( ERR_AGIT_STORE_06, kPacket.m_nOK, 6 ); // 아지트아이템 구매시 충분한 인벤 공간이 없음.
        ERR_CASE( ERR_AGIT_STORE_07, kPacket.m_nOK, 7 ); // 코디아이템 구매시 충분한 인벤 공간이 없음.
        ERR_CASE( ERR_AGIT_STORE_08, kPacket.m_nOK, 8 ); // 일반 아이템 구매시 충분한 인벤 공간이 없음.
        ERR_CASE( ERR_AGIT_STORE_09, kPacket.m_nOK, 9 ); // DB작업중 아이템 넣기 실패.
        ERR_CASE( ERR_AGIT_STORE_10, kPacket.m_nOK, 10 ); // DB작업중 룩 아이템 변환 실패.
        ERR_CASE( ERR_AGIT_STORE_11, kPacket.m_nOK, 11 ); // DB작업중 아지트 아이템 설정 실패.
        ERR_CASE( ERR_AGIT_STORE_12, kPacket.m_nOK, 12 ); // 인벤토리 보유량 제한에 걸림

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( bLookItem )
        << BUILD_LOG( kPacket_.first )
        << BUILD_LOG( kPacket_.second )
        << BUILD_LOG( vecMaterials.size() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nGP )
        << BUILD_LOG( kPacket.m_vecRewards.size() )
        << BUILD_LOG( kPacket.m_vecCountMaterialItems.size() )
        << BUILD_LOG( kPacket.m_vecPeriodMaterialItems.size() )
        << BUILD_LOG( m_kInventory.GetAgitObjectItemNum() )
        << BUILD_LOG( SiKAgitHelper()->GetMaxInvenSize() )
        << BUILD_LOG( m_kInventory.GetLookItemNum() )
        << BUILD_LOG( SiKLookEquipment()->GetMaxInvenSize() )
        << BUILD_LOG( m_kInventory.GetTotalEnableSize() )
        << BUILD_LOG(GetTrueInvenCapacity() )
        << BUILD_LOG( bAgitObject )
        << BUILD_LOG( bLookItem )
        << BUILD_LOG( nItemInvenCount )
        << BUILD_LOG( kDBPacket.m_kCatalog.m_nInvelLimit ) << END_LOG;

    if ( false == IS_CORRECT( NET_OK ) ) {
        SEND_PACKET( EVENT_BUY_FOR_AGIT_ACK );
    }
}

IMPL_ON_FUNC( EVENT_BUY_FOR_AGIT_ACK )
{
    CHECK_AGIT_ENABLE;

    std::vector< GCITEMUID >::iterator vit;
    NetError::SetLastNetError( kPacket_.m_nOK );

    kPacket_.m_nGP = GetGP();

    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    if ( !kPacket_.m_vecRewards.empty() ) {
        // 획득 아이템 넣기
        m_kInventory.AddItemList( kPacket_.m_vecRewards );

        // 구매재료 영구아이템 제거
        for ( vit = kPacket_.m_vecPeriodMaterialItems.begin() ; vit != kPacket_.m_vecPeriodMaterialItems.end() ; ++vit ) {
            LIF( m_kInventory.RemoveItem( *vit ) );
        }

        const KItem kItem = kPacket_.m_vecRewards.front();
        std::vector<KDropItemInfo> vecMaterials;

        if ( SiKAgitHelper()->m_kAgitStoreItems.GetMaterials( kPacket_.m_prKey, vecMaterials ) ) {

            std::vector<KDropItemInfo>::iterator vitCount;
            for ( vitCount = vecMaterials.begin() ; vitCount != vecMaterials.end() ; ++vitCount ) {
                KItemPtr pkItem = m_kInventory.FindDurationItemByItemID( vitCount->m_ItemID );
                if ( !pkItem ) {
                    START_LOG( cwarn, L"구매재료 수량아이템이 인벤에 없음. ItemID : " << vitCount->m_ItemID ) << END_LOG;
                    continue;
                }
                // 구매재료 수량 아이템 삭감
                pkItem->m_nCount = std::max<int>( 0, pkItem->m_nCount - vitCount->m_nDuration );
                kPacket_.m_vecCountMaterialItems.push_back( *pkItem );
            }
        }
    }

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,            kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_AGIT_STORE_01, kPacket_.m_nOK, 1 ); // 구매 아이템 정보가 없음.
        ERR_CASE( ERR_AGIT_STORE_02, kPacket_.m_nOK, 2 ); // 구매 아이템 재료정보가 없음.
        ERR_CASE( ERR_AGIT_STORE_03, kPacket_.m_nOK, 3 ); // 구매 재료 아이템이 인벤에 없음.
        ERR_CASE( ERR_AGIT_STORE_04, kPacket_.m_nOK, 4 ); // 구매재료 영구아이템이 없음.
        ERR_CASE( ERR_AGIT_STORE_05, kPacket_.m_nOK, 5 ); // 구매재료 수량아이템의 수량이 부족함.
        ERR_CASE( ERR_AGIT_STORE_06, kPacket_.m_nOK, 6 ); // 아지트아이템 구매시 충분한 인벤 공간이 없음.
        ERR_CASE( ERR_AGIT_STORE_07, kPacket_.m_nOK, 7 ); // 코디아이템 구매시 충분한 인벤 공간이 없음.
        ERR_CASE( ERR_AGIT_STORE_08, kPacket_.m_nOK, 8 ); // 일반 아이템 구매시 충분한 인벤 공간이 없음.
        ERR_CASE( ERR_AGIT_STORE_09, kPacket_.m_nOK, 9 ); // DB작업중 아이템 넣기 실패.
        ERR_CASE( ERR_AGIT_STORE_10, kPacket_.m_nOK, 10 ); // DB작업중 룩 아이템 변환 실패.
        ERR_CASE( ERR_AGIT_STORE_11, kPacket_.m_nOK, 11 ); // DB작업중 아지트 아이템 설정 실패.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 아직 처리중인 작업
        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nGP )
        << BUILD_LOG( kPacket_.m_prKey.first )
        << BUILD_LOG( kPacket_.m_prKey.second )
        << BUILD_LOG( kPacket_.m_vecRewards.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_BUY_FOR_AGIT_ACK );
}

_IMPL_ON_FUNC( EVENT_SELL_FOR_AGIT_REQ, std::vector<GCITEMUID> )
{
    CHECK_AGIT_ENABLE;

    KItemPtr pkItem;
    std::vector<GCITEMUID>::iterator vit;

    KEVENT_SELL_FOR_AGIT_ACK kPacket;
    kPacket.m_nOK = -99;

    bool bIsNormalPot = false;
    bool bIsCashPot = false;
    bool bNotResellObject = false;

    SET_ERROR( ERR_UNKNOWN );

    // 판매 리스트가 비었다.
    if ( kPacket_.empty() ) {
        SET_ERR_GOTO( ERR_AGIT_SELL_OBJECT_01, END_PROC );
    }

    for ( vit = kPacket_.begin() ; vit != kPacket_.end() ; ++vit ) {
        // 에러값은 에러스트링의 번호와 일치시킨다.
        pkItem = m_kInventory.FindItemByItemUID( *vit );
        if ( pkItem == NULL ) {
            kPacket.m_mapFailInfo[ *vit ] = 2;
            START_LOG( cwarn, L"아이템이 인벤에 없음. ItemUID : " << *vit << L", Login : " << GetName() ) << END_LOG;
            SET_ERROR( ERR_AGIT_SELL_OBJECT_02 );
            continue;
        }

        if ( false == pkItem->CheckItemType( KItem::TYPE_AGIT_OBJ ) ) {
            if ( false == SiKAgitHelper()->IsObjectItem( pkItem->m_ItemID ) ) {
                kPacket.m_mapFailInfo[ *vit ] = 3;
                START_LOG( cwarn, L"아지트 오브젝트 아이템이 아니다. ItemID : " << pkItem->m_ItemID << L", ItemUID : " << pkItem->m_ItemUID << L", Login : " << GetName() ) << END_LOG;
                SET_ERROR( ERR_AGIT_SELL_OBJECT_03 );
                continue;
            }

            // 아지트 오브젝트로 설정.
            pkItem->AddItemType( KItem::TYPE_AGIT_OBJ );
        }

        bIsNormalPot = SiKAgitHelper()->IsNormalPotItemID( pkItem->m_ItemID );
        bIsCashPot = SiKAgitHelper()->IsCashPotItemID( pkItem->m_ItemID );
        bNotResellObject = SiKAgitHelper()->IsNotResellObjectItemID( pkItem->m_ItemID );

        if ( bIsNormalPot || bIsCashPot || bNotResellObject ) {
            kPacket.m_mapFailInfo[ *vit ] = 6;
            START_LOG( cerr, L"판매할 수 없는 아이템. ItemID : " << pkItem->m_ItemID << L", ItemUID : " << pkItem->m_ItemUID << L", Login : " << GetName() )
                << BUILD_LOG( bIsNormalPot )
                << BUILD_LOG( bIsCashPot ) << END_LOG;
            SET_ERROR( ERR_AGIT_SELL_OBJECT_06 );
            continue;
        }

        // 수량성 아이템은 보상없이 지워짐.
        if ( pkItem->m_nCount >= 0 ) {
            kPacket.m_vecEraseItem.push_back( pkItem->m_ItemUID );
            continue;
        }

        // 기간제 아이템은 보상없이 지워짐.
        if ( pkItem->m_nPeriod >= 0 ) {
            kPacket.m_vecEraseItem.push_back( pkItem->m_ItemUID );
            continue;
        }

        // 판매되는 아이템 리스트
        kPacket.m_vecSellItem.push_back( pkItem->m_ItemUID );
    }

    // 서버 처리 실패
    if ( kPacket.m_vecSellItem.empty() && kPacket.m_vecEraseItem.empty() ) {
        SET_ERR_GOTO( ERR_AGIT_SELL_OBJECT_04, END_PROC );
    }

    // DB처리 요청
    {
        KDB_EVENT_SELL_FOR_AGIT_REQ kDBPacket;
        kDBPacket.m_mapFailInfo = kPacket.m_mapFailInfo;
        kDBPacket.m_vecEraseItem = kPacket.m_vecEraseItem;
        kDBPacket.m_vecSellItem = kPacket.m_vecSellItem;
        QUEUEING_EVENT_TO_DB_CHECK( EVENT_SELL_FOR_AGIT_REQ, kDBPacket );
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                  kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_AGIT_SELL_OBJECT_01, kPacket.m_nOK, 1 ); // 판매할 아이템 리스트가 비었음.
        ERR_CASE( ERR_AGIT_SELL_OBJECT_04, kPacket.m_nOK, 4 ); // 아이템 판매 서버처리 실패.
        ERR_CASE( ERR_AGIT_SELL_OBJECT_06, kPacket.m_nOK, 6 ); // 판매할 수 없는 아이템.
        ERR_CASE( ERR_NOT_FINISHED_YET,    kPacket.m_nOK, -98 ); // 아직 처리중인 작업
    default:
        START_LOG( cerr, L"아지트 물품판매 중 알수 없는 오류.. Name : " << m_strName )
            << L" Msg : " << NetError::GetLastNetErrMsg() << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"아지트 물품판매 요청. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( kPacket_.size() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_mapFailInfo.size() )
        << BUILD_LOG( kPacket.m_vecEraseItem.size() )
        << BUILD_LOG( kPacket.m_vecSellItem.size() ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_SELL_FOR_AGIT_ACK );
    }
}

IMPL_ON_FUNC( EVENT_SELL_FOR_AGIT_ACK )
{
    CHECK_AGIT_ENABLE;

    std::vector<GCITEMUID>::iterator vit;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    if ( !kPacket_.m_vecSellItem.empty() || !kPacket_.m_vecEraseItem.empty() ) {
        KEAG_DELETE_FOR_AGIT_NOT kAgPacket;
        kAgPacket.m_dwAgitUID = GetUID();
        kAgPacket.m_vecDeleteObject.clear();
        kAgPacket.m_vecDeleteObject.insert( kAgPacket.m_vecDeleteObject.end(), kPacket_.m_vecSellItem.begin(), kPacket_.m_vecSellItem.end() );
        kAgPacket.m_vecDeleteObject.insert( kAgPacket.m_vecDeleteObject.end(), kPacket_.m_vecEraseItem.begin(), kPacket_.m_vecEraseItem.end() );

        if ( !kAgPacket.m_vecDeleteObject.empty() ) {
            int nSID = SiKAgitHelper()->GetAgitSID( kAgPacket.m_dwAgitUID );
            SiKAgentConnector()->SendPacket( nSID, KAgentEvent::EAG_DELETE_FOR_AGIT_NOT, kAgPacket );
        }
    }

    // 삭제아이템 인벤에서 제거
    RemoveItemFromInven( kPacket_.m_vecSellItem );
    RemoveItemFromInven( kPacket_.m_vecEraseItem );

    // 획득 아이템 넣기
    m_kInventory.AddItemList( kPacket_.m_vecRewardItem );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                  kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_AGIT_SELL_OBJECT_05, kPacket_.m_nOK, 5 ); // 아이템 판매 DB처리 실패.
        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_vecRewardItem.size() )
        << BUILD_LOG( kPacket_.m_vecSellItem.size() )
        << BUILD_LOG( kPacket_.m_vecEraseItem.size() )
        << BUILD_LOG( kPacket_.m_mapFailInfo.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SELL_FOR_AGIT_ACK );
}

_IMPL_ON_FUNC( EVENT_SEED_FLOWER_POT_REQ, KSeedFlowerPotReq )
{
    // 씨앗 심기
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 1, KUserFSM::STATE_AGIT ));
    kPacket_.m_dwAgitUID = GetUID();

    KSeedFlowerPotAck kPacket;
    kPacket.m_nOK = -99;

    KFlowerPotOption kOption;
    KItemPtr pkItem;

    SET_ERROR( ERR_UNKNOWN );

    // 씨앗 심을때는 자기 아지트에 있어야함.
    if ( m_dwStayAgitUID != GetUID() ) {
        SET_ERR_GOTO( ERR_SEED_FLOWER_01, END_PROC );
    }

    // 화분 보유여부
    pkItem = m_kInventory.FindItemByItemUID( kPacket_.m_kPotItem.m_ItemUID );
    if ( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_SEED_FLOWER_02, END_PROC );
    }
    kPacket_.m_kPotItem = *pkItem;

    // 씨앗 보유여부
    pkItem = m_kInventory.FindItemByItemUID( kPacket_.m_kSeedItem.m_ItemUID );
    if ( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_SEED_FLOWER_03, END_PROC );
    }
    kPacket_.m_kSeedItem = *pkItem;

    // 씨앗심기 옵션
    if ( false == SiKAgitHelper()->GetFlowerPotOption( kPacket_.m_nSeedType, kOption ) ) {
        SET_ERR_GOTO( ERR_SEED_FLOWER_05, END_PROC );
    }
    kPacket_.m_nNeedSeedCount = kOption.m_nNeedSeedCount;

    // 씨앗인가?
    if ( kPacket_.m_kSeedItem.m_ItemID != SiKAgitHelper()->GetSeedItemID() ) {
        SET_ERR_GOTO( ERR_SEED_FLOWER_09, END_PROC );
    }

    // 씨앗의 개수는 충분한가?
    if ( kPacket_.m_kSeedItem.m_nCount < kOption.m_nNeedSeedCount ) {
        SET_ERR_GOTO( ERR_SEED_FLOWER_10, END_PROC );
    }

    int nSID = SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID );
    SiKAgentConnector()->SendPacket( nSID, KAgentEvent::EAG_SEED_FLOWER_POT_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,             kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_SEED_FLOWER_01, kPacket.m_nOK, 1 ); // 현재 있는 아지트가 내 아지트가 아님.
        ERR_CASE( ERR_SEED_FLOWER_02, kPacket.m_nOK, 2 ); // 심고자 하는 화분을 가지고있지 않음.
        ERR_CASE( ERR_SEED_FLOWER_03, kPacket.m_nOK, 3 ); // 씨앗 아이템이 없음.
        ERR_CASE( ERR_SEED_FLOWER_05, kPacket.m_nOK, 5 ); // 정의되지 않은 씨앗심기 타입.
        ERR_CASE( ERR_SEED_FLOWER_09, kPacket.m_nOK, 9 ); // 씨앗 아이템이 아니다.
        ERR_CASE( ERR_SEED_FLOWER_10, kPacket.m_nOK, 10 ); // 씨앗 개수가 부족하다.

    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( GetName() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"씨앗심기 요청. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( kPacket_.m_nSeedType )
        << BUILD_LOG( kPacket_.m_kPotItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kPotItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kSeedItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kSeedItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kSeedItem.m_nCount )
        << BUILD_LOG( SiKAgitHelper()->GetSeedItemID() )
        << BUILD_LOG( kOption.m_nNeedSeedCount ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_SEED_FLOWER_POT_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_SEED_FLOWER_POT_ACK, KSeedFlowerPotAck )
{
    CHECK_AGIT_ENABLE;

    KItemPtr pkItem;

    if ( kPacket_.m_nOK != 0 ) {
        goto END_PROC;
    }

    pkItem = m_kInventory.FindItemByItemUID( kPacket_.m_kSeedItem.m_ItemUID );
    if ( pkItem == NULL ) {
        START_LOG( cerr, L"씨앗심기 후, 씨앗 아이템을 찾을 수 없음. Login : " << GetName() ) << END_LOG;
        goto END_PROC;
    }

    // 씨앗 수량 갱신
    if ( pkItem->m_nCount > 0 ) {
        pkItem->m_nCount = kPacket_.m_kSeedItem.m_nCount;
        pkItem->m_nInitCount = kPacket_.m_kSeedItem.m_nInitCount;
    }

    if ( pkItem->m_nCount <= 0 ) {
        m_kInventory.RemoveItem( kPacket_.m_kSeedItem.m_ItemID, kPacket_.m_kSeedItem.m_ItemUID );
    }

END_PROC:
    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"씨앗심기 결과 받음. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_kPotInfo.m_UID )
        << BUILD_LOG( kPacket_.m_kPotInfo.m_ItemID )
        << BUILD_LOG( kPacket_.m_kPotInfo.m_nSeedType )
        << BUILD_LOGtm( CTime(kPacket_.m_kPotInfo.m_tmSeedTime) )
        << BUILD_LOGtm( CTime(kPacket_.m_kPotInfo.m_tmFlowerTime) )
        << BUILD_LOG( kPacket_.m_kPotInfo.m_usFlowerCount )
        << BUILD_LOG( kPacket_.m_kSeedItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kSeedItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kSeedItem.m_nCount ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SEED_FLOWER_POT_ACK );
}

IMPL_ON_FUNC( EVENT_CHANGE_FLOWER_POT_STATE_BROAD )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    if ( m_dwStayAgitUID != kPacket_.m_dwAgitUID ) {
        START_LOG( cwarn, L"현재 내가 있는 아지트와 다른 아지트의 화분상태 변경 알림. Login : " << GetName() )
            << BUILD_LOG( kPacket_.m_dwAgitUID )
            << BUILD_LOG( m_dwStayAgitUID ) << END_LOG;
        return;
    }

    START_LOG( clog, L"내가 있는 아지트의 화분상태 변경 알림. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_kFlowerPot.m_UID )
        << BUILD_LOG( kPacket_.m_kFlowerPot.m_ItemID )
        << BUILD_LOG( kPacket_.m_kFlowerPot.m_nSeedType ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CHANGE_FLOWER_POT_STATE_BROAD );
}

_IMPL_ON_FUNC( EVENT_HARVEST_FLOWER_POT_REQ, KHarvestFlowerPotReq )
{
    // 수확하기
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 1, KUserFSM::STATE_AGIT ));
    kPacket_.m_dwAgitUID = GetUID();
    kPacket_.m_kFlowerItem.m_ItemID = SiKAgitHelper()->GetFlowerItemID();
    kPacket_.m_nFlowerCount = 0;
    kPacket_.m_nBonusFlowerCount = 0;

    KHarvestFlowerPotAck kPacket;
    kPacket.m_nOK = -99;

    KItemPtr pkItem;

    SET_ERROR( ERR_UNKNOWN );

    // 수확할때 자기 아지트에 있어야함.
    if ( m_dwStayAgitUID != GetUID() ) {
        SET_ERR_GOTO( ERR_HARVEST_FLOWER_01, END_PROC );
    }

    // 화분 보유여부
    pkItem = m_kInventory.FindItemByItemUID( kPacket_.m_kPotItem.m_ItemUID );
    if ( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_HARVEST_FLOWER_02, END_PROC );
    }
    kPacket_.m_kPotItem = *pkItem;

    // 화폐아이템 있으면 정보 받아가자.
    pkItem = m_kInventory.FindItemByItemID( kPacket_.m_kFlowerItem.m_ItemID );
    if ( pkItem != NULL ) {
        kPacket_.m_kFlowerItem = *pkItem;
    }

    int nSID = SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID );
    SiKAgentConnector()->SendPacket( nSID, KAgentEvent::EAG_HARVEST_FLOWER_POT_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_HARVEST_FLOWER_01, kPacket.m_nOK, 1 ); // 현재 있는 아지트가 내 아지트가 아님.
        ERR_CASE( ERR_HARVEST_FLOWER_02, kPacket.m_nOK, 2 ); // 수확할 화분을 가지고있지 않음.

    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( GetName() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"화분 수확 요청. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( kPacket_.m_kPotItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kPotItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kFlowerItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kFlowerItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_bBonusFlower ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_HARVEST_FLOWER_POT_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_HARVEST_FLOWER_POT_NOT, KHarvestFlowerPotAck )
{
    CHECK_AGIT_ENABLE;
    if ( kPacket_.m_nOK != 0 ) {
        goto END_PROC;
    }

    // 획득한 꽃은 게임서버 DB처리에서 넣어준다.
    QUEUEING_EVENT_TO_DB( EVENT_HARVEST_FLOWER_POT_NOT, kPacket_ );

END_PROC:
    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"수확하기 결과 받음. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_kPotInfo.m_UID )
        << BUILD_LOG( kPacket_.m_kPotInfo.m_ItemID )
        << BUILD_LOG( kPacket_.m_kPotInfo.m_nSeedType )
        << BUILD_LOG( kPacket_.m_kPotInfo.m_usFlowerCount )
        << BUILD_LOG( kPacket_.m_nFlowerCount )
        << BUILD_LOG( kPacket_.m_nBonusFlowerCount )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;

    if ( kPacket_.m_nOK != 0 ) {
        SEND_RECEIVED_PACKET( EVENT_HARVEST_FLOWER_POT_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_HARVEST_FLOWER_POT_ACK, KHarvestFlowerPotAck )
{
    // 지급할 아이템이 있으면 인벤에 넣기
    CHECK_AGIT_ENABLE;

    if ( !kPacket_.m_vecItem.empty() ) {
        m_kInventory.AddItemList( kPacket_.m_vecItem );
    }

    //if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
    //    START_LOG( cwarn, L"-- Harvest Flower Pot Item (size:" << kPacket_.m_vecItem.size() << L") --"  ) << END_LOG;

    //    std::vector<KItem>::const_iterator cvit;
    //    for ( cvit = kPacket_.m_vecItem.begin() ; cvit != kPacket_.m_vecItem.end() ; ++cvit ) {
    //        std::Warn << L"ItemID : " << cvit->m_ItemID
    //                    << L", ItemUID : " << cvit->m_ItemUID
    //                    << L", Count : " << cvit->m_nDuration
    //                    << L", Period : " << cvit->m_nPeriod << dbg::endl;
    //    }
    //}

    _LOG_CONDITION( !kPacket_.m_vecItem.empty(), clog, cwarn, L"수확하기 결과.. 획득아이템 지급. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_kPotInfo.m_UID )
        << BUILD_LOG( kPacket_.m_kPotInfo.m_ItemID )
        << BUILD_LOG( kPacket_.m_kPotInfo.m_nSeedType )
        << BUILD_LOG( kPacket_.m_kPotInfo.m_usFlowerCount )
        << BUILD_LOG( kPacket_.m_nFlowerCount )
        << BUILD_LOG( kPacket_.m_nBonusFlowerCount )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_HARVEST_FLOWER_POT_ACK );
}

_IMPL_ON_FUNC( EVENT_AGIT_STAY_BONUS_DROP_ACK, KAgitBonusDrop )
{
    CHECK_AGIT_ENABLE;

    if ( kPacket_.m_vecItem.empty() ) {
        return;
    }

    kPacket_.m_dwAgitUID = m_dwStayAgitUID;
    kPacket_.m_tmDropTime = GetTick( AGIT_STAY );

    // 아이템 인벤에 넣기
    m_kInventory.AddItemList( kPacket_.m_vecItem );

    //if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
    //    START_LOG( cwarn, L"-- Agit StayBonus Item (size:" << kPacket_.m_vecItem.size() << L") --"  ) << END_LOG;

    //    std::vector<KItem>::const_iterator cvit;
    //    for ( cvit = kPacket_.m_vecItem.begin() ; cvit != kPacket_.m_vecItem.end() ; ++cvit ) {
    //        std::Warn << L"ItemID : " << cvit->m_ItemID
    //            << L", ItemUID : " << cvit->m_ItemUID
    //            << L", Count : " << cvit->m_nDuration
    //            << L", Period : " << cvit->m_nPeriod << dbg::endl;
    //    }
    //}

    START_LOG( clog, L"아지트 체재시간 보너스 아이템 획득함. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOGtm( CTime(kPacket_.m_tmDropTime) )
        << BUILD_LOG( kPacket_.m_nDropCount )
        << BUILD_LOG( kPacket_.m_vecItem.size() )
        << BUILD_LOG( m_nTodaySeedDropCount ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_AGIT_STAY_BONUS_DROP_NOT );
}

IMPL_ON_FUNC( EVENT_TODAYS_SEED_DROP_COUNT_ACK )
{
    CHECK_AGIT_ENABLE;

    KDropItemInfo kDropItem;
    kDropItem.m_ItemID = SiKAgitHelper()->GetSeedItemID();
    kDropItem.m_nDuration = 0;
    kDropItem.m_nPeriod = -1;

    int nInitSeedCount = SiKAgitHelper()->GetDailyInitSeedCount();
    int nInvenCount = 0; // 인벤토리 보유수량

    m_nTodaySeedDropCount = kPacket_.m_nTodaySeedDropCount;

    // 씨앗 초기화 진행
    if ( kPacket_.m_bTodaySeedInit ) {
        // 인벤 보유량 확인
        KItemPtr pkItem = m_kInventory.FindItemByItemID( kDropItem.m_ItemID );
        if ( pkItem == NULL ) {
            // 아이템 없으면 초기화 수량만큼 채워주기.
            kDropItem.m_nDuration = nInitSeedCount;
        }
        else {
            // 초기화 수량보다 적으면, 보장수량만큼 채워주기.
            if ( pkItem->m_nCount < nInitSeedCount ) {
                kDropItem.m_nDuration = nInitSeedCount - pkItem->m_nCount;
            }

            nInvenCount = pkItem->m_nCount; // 보유수량
        }

        // 지급할 수량이 있으면, DB 지급요청
        if ( kDropItem.m_nDuration > 0 ) {
            // 획득할 수량을 먼저 더해준다.
            m_nTodaySeedDropCount += kDropItem.m_nDuration;
            QUEUEING_EVENT_TO_DB( EVENT_AGIT_INIT_SEED_REQ, kDropItem );
        }
    }

    START_LOG( clog, L"Todays Seed Drop Count.. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_nTodaySeedDropCount )
        << BUILD_LOG( kPacket_.m_bTodaySeedInit )
        << BUILD_LOGtm( CTime(kPacket_.m_tmRegDate) )
        << BUILD_LOG( kDropItem.m_ItemID )
        << BUILD_LOG( kDropItem.m_nDuration )
        << BUILD_LOG( nInitSeedCount )
        << BUILD_LOG( nInvenCount ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_AGIT_CHAT_REQ, KChatData )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE_LOG( cwarn,( 1, KUserFSM::STATE_AGIT ));

    kPacket_.m_dwSenderUID = GetUID();
    kPacket_.m_strSenderNick = GetNickName();
    kPacket_.m_strSenderNickColor = GetNickColor();
    kPacket_.m_iParam = m_dwStayAgitUID;

    KAgitChatAck kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwUserUID = GetUID();

    SET_ERROR( ERR_UNKNOWN );

    // 아지트 상태가 아니다.
    if ( m_dwStayAgitUID == 0 ) {
        SET_ERR_GOTO( ERR_AGIT_CHAT_01, END_PROC );
    }

    if( !CheckChatEnable() ) { // 채팅 블럭 상태.
        SET_ERR_GOTO( ERR_AGIT_CHAT_08, END_PROC );
    }

    // 채팅주기가 짧음.
    if( ::GetTickCount() - GetTick(CHAT_TICK) < 1000 ) {
        SET_ERR_GOTO( ERR_AGIT_CHAT_02, END_PROC );
    }

    // 메세지가 비어있음.
    if ( kPacket_.m_strChatMsg.empty() ) {
        SET_ERR_GOTO( ERR_AGIT_CHAT_03, END_PROC );
    }

    switch ( kPacket_.m_cChatType )
    {
    case KChatData::MSG_COMMON:
        break;
    case KChatData::MSG_COLOR_CHAT:
        // 칼라채팅할 조건이 안된다.
        if ( false == IsAdmin() && false == IsPremiumUser() && m_cPCBangType == KIPAdvantage::PCBANG_NONE ) {
            SET_ERR_GOTO( ERR_AGIT_CHAT_04, END_PROC );
        }
        break;
    case KChatData::MSG_ADMIN_COMMON:
        // 운영자가 아닌데 영자채팅 요청
        if ( false == IsAdmin() ) {
            SET_ERR_GOTO( ERR_AGIT_CHAT_05, END_PROC );
        }
        break;
    default:
        // 정의되지않은 아지트채팅 타입
        SET_ERR_GOTO( ERR_AGIT_CHAT_06, END_PROC );
        break;
    }

    int nSID = SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID );
    SiKAgentConnector()->SendPacket( nSID, KAgentEvent::EAG_AGIT_CHAT_REQ, kPacket_ );
    SetTick(CHAT_TICK);

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_AGIT_CHAT_01, kPacket.m_nOK, 1 ); // 아지트 상태가 아니다.
        ERR_CASE( ERR_AGIT_CHAT_02, kPacket.m_nOK, 2 ); // 채팅주기가 짧음.
        ERR_CASE( ERR_AGIT_CHAT_03, kPacket.m_nOK, 3 ); // 메세지가 비어있음.
        ERR_CASE( ERR_AGIT_CHAT_04, kPacket.m_nOK, 4 ); // 칼라채팅할 조건이 안된다.
        ERR_CASE( ERR_AGIT_CHAT_05, kPacket.m_nOK, 5 ); // 운영자가 아닌데 영자채팅 요청.
        ERR_CASE( ERR_AGIT_CHAT_06, kPacket.m_nOK, 6 ); // 정의되지않은 아지트채팅 타입.
        ERR_CASE( ERR_AGIT_CHAT_08, kPacket.m_nOK, 7 ); // 채팅 블럭 상태임.
    default:
        START_LOG( cerr, L"아지트 채팅 요청시 알수 없는 오류.. Name : " << m_strName )
            << L" Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
            << BUILD_LOGc( kPacket_.m_cChatType ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"아지트 채팅 요청. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( nSID )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( IsAdmin() )
        << BUILD_LOG( IsPremiumUser() )
        << BUILD_LOGc( m_cPCBangType )
        << BUILD_LOGc( kPacket_.m_cChatType )
        << BUILD_LOG( kPacket_.m_dwChatColor )
        << BUILD_LOG( kPacket_.m_strChatMsg ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_AGIT_CHAT_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_AGIT_CHAT_ACK, KAgitChatAck )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ));

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"채팅 결과 받음. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( m_dwStayAgitUID ) << END_LOG;

    if ( m_dwStayAgitUID > 0 ) {
        SEND_RECEIVED_PACKET( EVENT_AGIT_CHAT_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_AGIT_CHAT_BROAD, KChatData )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ));

    DWORD dwAgitUID = static_cast<DWORD>( kPacket_.m_iParam );

    START_LOG( clog, L"아지트 채팅 받음. Login : " << GetName() )
        << BUILD_LOG( dwAgitUID )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOGc( kPacket_.m_cChatType )
        << BUILD_LOG( kPacket_.m_dwSenderUID )
        << BUILD_LOG( kPacket_.m_strSenderNick )
        << BUILD_LOG(kPacket_.m_strSenderNickColor)
        << BUILD_LOG( kPacket_.m_dwChatColor )
        << BUILD_LOG( kPacket_.m_strChatMsg ) << END_LOG;

    if ( dwAgitUID == m_dwStayAgitUID ) {
        SEND_RECEIVED_PACKET( EVENT_AGIT_CHAT_BROAD );
    }
}

_IMPL_ON_FUNC( EVENT_AGIT_BAN_USER_REQ, KAgitBanUserReq )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 1, KUserFSM::STATE_AGIT ));
    kPacket_.m_dwAgitUID = GetUID();

    KAgitBanUserAck kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
    kPacket.m_dwBanUserUID = kPacket_.m_dwBanUserUID;

    SET_ERROR( ERR_UNKNOWN );

    // 자기 아지트에 있는가
    if ( m_dwStayAgitUID != GetUID() ) {
        SET_ERR_GOTO( ERR_AGIT_BAN_01, END_PROC );
    }

    SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID ), KAgentEvent::EAG_AGIT_BAN_USER_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,          kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_AGIT_BAN_01, kPacket.m_nOK, 1 ); // 현재 있는 아지트가 내 아지트가 아님.
    default:
        START_LOG( cerr, L"아지트 추방요청시 알수 없는 오류.. Name : " << m_strName )
            << L" Msg : " << NetError::GetLastNetErrMsg()  << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"아지트 추방 요청. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( kPacket_.m_dwBanUserUID ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_AGIT_BAN_USER_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_AGIT_BAN_USER_ACK, KAgitBanUserAck )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ));

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"아지트 추방요청 결과받음. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_dwBanUserUID ) << END_LOG;

    if ( m_dwStayAgitUID > 0 ) {
        SEND_RECEIVED_PACKET( EVENT_AGIT_BAN_USER_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_AGIT_BAN_USER_NOT, DWORD )
{
    CHECK_AGIT_ENABLE;

    m_setAgitBanList.insert( kPacket_ );

    START_LOG( clog, L"아지트 추방당함. Login : " << GetName() )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_setAgitBanList.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_AGIT_BAN_USER_NOT );
}

_IMPL_ON_FUNC( EVENT_SET_AGIT_OBJECT_POS_REQ, KSetAgitObjectPosReq )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE_LOG( cwarn,( 1, KUserFSM::STATE_AGIT ));
    kPacket_.m_dwAgitUID = GetUID();

    KSetAgitObjectPosAck kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
    kPacket.m_vecObject = kPacket_.m_vecObject;

    KItemPtr pkItem;
    std::vector<KAgitObjectPos>::iterator vit;

    SET_ERROR( ERR_UNKNOWN );

    // 자기 아지트에 있는가
    if ( m_dwStayAgitUID != GetUID() ) {
        SET_ERR_GOTO( ERR_AGIT_OBJECT_01, END_PROC );
    }

    // 패킷이 비어있는가
    if ( kPacket_.m_vecObject.empty() ) {
        SET_ERR_GOTO( ERR_AGIT_OBJECT_06, END_PROC );
    }

    for ( vit = kPacket_.m_vecObject.begin() ; vit != kPacket_.m_vecObject.end() ; ++vit ) {
        // 아이템 보유하고 있는가
        pkItem = m_kInventory.FindItemByItemUID( vit->m_UID );
        if ( pkItem == NULL ) {
            START_LOG( cwarn, L"오브젝트 아이템이 인벤토리에 없음. Login : " << GetName() )
                << BUILD_LOG( vit->m_ID )
                << BUILD_LOG( vit->m_UID ) << END_LOG;
            SET_ERR_GOTO( ERR_AGIT_OBJECT_02, END_PROC );
        }

        // 오브젝트 아이템인가?
        if ( false == pkItem->CheckItemType( KItem::TYPE_AGIT_OBJ ) ) {
            if ( false == SiKAgitHelper()->IsObjectItem( vit->m_ID ) ) {
                START_LOG( cwarn, L"오브젝트 아이템이 아니다. Login : " << GetName() )
                    << BUILD_LOG( vit->m_ID )
                    << BUILD_LOG( vit->m_UID ) << END_LOG;
                SET_ERR_GOTO( ERR_AGIT_OBJECT_03, END_PROC );
            }

            // 아지트 오브젝트로 설정.
            pkItem->AddItemType( KItem::TYPE_AGIT_OBJ );
        }

        // 좌표값 보정
        vit->m_nPosX = std::max<int>( vit->m_nPosX, -1 );
        vit->m_nPosY = std::max<int>( vit->m_nPosY, -1 );
    }

    SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID ), KAgentEvent::EAG_SET_AGIT_OBJECT_POS_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,             kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_AGIT_OBJECT_01, kPacket.m_nOK, 1 ); // 현재 있는 아지트가 내 아지트가 아님.
        ERR_CASE( ERR_AGIT_OBJECT_02, kPacket.m_nOK, 2 ); // 배치하려는 오브젝트 아이템이 인벤토리에 없음.
        ERR_CASE( ERR_AGIT_OBJECT_03, kPacket.m_nOK, 3 ); // 오브젝트 아이템이 아님.
        ERR_CASE( ERR_AGIT_OBJECT_06, kPacket.m_nOK, 6 ); //  배치하려는 오브젝트 정보가 비어있음.
    default:
        START_LOG( cerr, L"아지트 오브젝트 배치요청 중 알수 없는 오류.. Name : " << m_strName )
            << L" Msg : " << NetError::GetLastNetErrMsg()  << END_LOG;
        kPacket.m_nOK = -99;
    }

    if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
        START_LOG( cwarn, L"--- Dump SetAgitObjectPos (size:" << kPacket_.m_vecObject.size() << L") ---"  ) << END_LOG;
        std::vector<KAgitObjectPos>::const_iterator cvit;
        for ( cvit = kPacket_.m_vecObject.begin() ; cvit != kPacket_.m_vecObject.end() ; ++cvit ) {
            START_LOG( cwarn, L"ItemID : " << cvit->m_ID << L", ItemUID : " << cvit->m_UID << L", Pos(" << cvit->m_nPosX << L"," << cvit->m_nPosY << L")"  ) << END_LOG;
        }
        START_LOG( cwarn, L"--- End Dump --- "  ) << END_LOG;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"아지트 오브젝트 배치요청. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( kPacket_.m_vecObject.size() ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_SET_AGIT_OBJECT_POS_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_SET_AGIT_OBJECT_POS_ACK, KSetAgitObjectPosAck )
{
    CHECK_AGIT_ENABLE;
    if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
        START_LOG( cwarn, L"--- Dump SetAgitObjectPos (size:" << kPacket_.m_vecObject.size() << L") ---"  ) << END_LOG;
        std::vector<KAgitObjectPos>::const_iterator vit;
        for ( vit = kPacket_.m_vecObject.begin() ; vit != kPacket_.m_vecObject.end() ; ++vit ) {
            START_LOG( cwarn, L"ItemID : " << L", ItemUID : " << L", Pos(" << L")"  ) << END_LOG;
        }
        START_LOG( cwarn, L"--- End Dump --- "  ) << END_LOG;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"아지트 오브젝트배치 결과받음. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_vecObject.size() ) << END_LOG;

    if ( m_dwStayAgitUID > 0 ) {
        SEND_RECEIVED_PACKET( EVENT_SET_AGIT_OBJECT_POS_ACK );
    }
}

IMPL_ON_FUNC( EVENT_CHANGE_AGIT_OBJECT_POS_BROAD )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE_LOG( cwarn,( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ));

    if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
        START_LOG( cwarn, L"--- Dump ChangeAgitObjectPos (size:" << kPacket_.m_vecObject.size() << L") ---"  ) << END_LOG;
        std::vector<KAgitObjectPos>::const_iterator cvit;
        for ( cvit = kPacket_.m_vecObject.begin() ; cvit != kPacket_.m_vecObject.end() ; ++cvit ) {
            START_LOG( cwarn, L"ItemID : " << cvit->m_ID << L", ItemUID : " << cvit->m_UID << L", Pos(" << cvit->m_nPosX << L"," << cvit->m_nPosY << L")"  ) << END_LOG;
        }
        START_LOG( cwarn, L"--- End Dump --- "  ) << END_LOG;
    }

    START_LOG( clog, L"아지트 오브젝트 변경알림. Login : " << GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_vecObject.size() ) << END_LOG;

    if ( kPacket_.m_dwAgitUID == m_dwStayAgitUID ) {
        SEND_RECEIVED_PACKET( EVENT_CHANGE_AGIT_OBJECT_POS_BROAD );
    }
}

_IMPL_ON_FUNC( EVENT_AGIT_FRIEND_LIST_REQ, KAgitFriendListReq )
{
    CHECK_AGIT_ENABLE;
    kPacket_.m_dwUserUID = GetUID();
    SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID ), KAgentEvent::EAG_AGIT_FRIEND_LIST_REQ, kPacket_ );
    START_LOG( clog, L"아지트 친구 리스트 요청. Login : " << GetName() << L", UserUID : " << GetUID() << L", size : " << kPacket_.m_setFriendUID.size() ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_AGIT_TOP_RANK_LIST_REQ )
{
    CHECK_AGIT_ENABLE;
    DWORD dwUserUID = GetUID();
    SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( dwUserUID ), KAgentEvent::EAG_AGIT_TOP_RANK_LIST_REQ, dwUserUID );
    START_LOG( clog, L"아지트 인기아지트 리스트 요청. Login : " << GetName() << L", UserUID : " << GetUID() ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_AGIT_FAVORITE_LIST_REQ )
{
    CHECK_AGIT_ENABLE;
    DWORD dwUserUID = GetUID();
    SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( dwUserUID ), KAgentEvent::EAG_AGIT_FAVORITE_LIST_REQ, dwUserUID );
    START_LOG( clog, L"아지트 즐겨찾기 리스트 요청. Login : " << GetName() << L", UserUID : " << GetUID() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_AGIT_ADD_FAVORITE_REQ, KAgitUserParam )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ));

    kPacket_.m_dwUserUID = GetUID();
    KAgitAddFavoriteAck kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwUserUID = GetUID();

    SET_ERROR( ERR_UNKNOWN );

// 데이터가 비어있을 경우, 현재 머물고 있는 아지트UID로 대체
    if ( kPacket_.m_dwAgitUID == 0 ) {
        kPacket_.m_dwAgitUID = m_dwStayAgitUID;
    }

    // 자기 아지트를 즐겨찾기할 수 없음
    if ( kPacket_.m_dwAgitUID == GetUID() ) {
        SET_ERR_GOTO( ERR_AGIT_FAVORITE_01, END_PROC );
    }

    DWORD dwUserUID = GetUID();
    int nSID = SiKAgitHelper()->GetAgitSID( dwUserUID );
    SiKAgentConnector()->SendPacket( nSID, KAgentEvent::EAG_AGIT_ADD_FAVORITE_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_AGIT_FAVORITE_01, kPacket.m_nOK, 1 ); // 내 아지트를 즐겨찾기할 수 없음.
    default:
        START_LOG( cerr, L"아지트 즐겨찾기 추가 중 알수 없는 오류.. Name : " << m_strName )
            << L" Msg : " << NetError::GetLastNetErrMsg() << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"아지트 즐겨찾기 추가. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( nSID )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( kPacket_.m_dwAgitUID ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_AGIT_ADD_FAVORITE_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_AGIT_REMOVE_FAVORITE_REQ, KAgitUserParam )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ));

    kPacket_.m_dwUserUID = GetUID();
    SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( kPacket_.m_dwUserUID ), KAgentEvent::EAG_AGIT_REMOVE_FAVORITE_REQ, kPacket_ );

    START_LOG( clog, L"아지트 즐겨찾기 제거 요청. Login : " << GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( kPacket_.m_dwAgitUID ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_REGISTER_AGIT_PAGE_REQ, KRegisterAgitPageReq )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ));

    SET_ERROR( ERR_UNKNOWN );

    kPacket_.m_dwAgitUID = GetUID();
    kPacket_.m_strNickName = GetNickName();

    KRegisterAgitPageAck kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwAgitUID = GetUID();

    // 자기 아지트에서만 아지트 페이지에 등록할 수 있음.
    if ( m_dwStayAgitUID != GetUID() ) {
        SET_ERR_GOTO( ERR_AGIT_PAGE_01, END_PROC );
    }

    SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID ), KAgentEvent::EAG_REGISTER_AGIT_PAGE_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_AGIT_PAGE_01, kPacket.m_nOK, 1 ); // 자기 아지트에서만 아지트 페이지에 등록할 수 있음.
    default:
        START_LOG( cerr, L"아지트 페이지 등록 중 알수 없는 오류.. Name : " << m_strName )
            << L" Msg : " << NetError::GetLastNetErrMsg()  << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"아지트 페이지 등록 요청. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOGc( kPacket_.m_ucMaxUser )
        << BUILD_LOGc( kPacket_.m_ucOpenType )
        << BUILD_LOG( kPacket_.m_strPassword )
        << BUILD_LOG( kPacket_.m_strNickName ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_REGISTER_AGIT_PAGE_ACK );
    }
}

IMPL_ON_FUNC_NOPARAM( EVENT_UNREGISTER_AGIT_PAGE_REQ )
{
    CHECK_AGIT_ENABLE;
    SET_ERROR( ERR_UNKNOWN );

    DWORD dwUserUID = GetUID();
    SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID ), KAgentEvent::EAG_UNREGISTER_AGIT_PAGE_REQ, dwUserUID );

    START_LOG( clog, L"아지트 페이지 등록해제 요청." )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_AGIT_PAGE_LIST_REQ, KAgitPageList )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ));

    kPacket_.m_dwUserUID = GetUID();
    SiKAgentConnector()->SendPacket( kPacket_.m_nSID, KAgentEvent::EAG_AGIT_PAGE_LIST_REQ, kPacket_ );

    START_LOG( clog, L"아지트 페이지리스트 요청." )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( kPacket_.m_nSID )
        << BUILD_LOG( kPacket_.m_bOpen )
        << BUILD_LOG( kPacket_.m_bNotFull )
        << BUILD_LOG( kPacket_.m_usMinUser )
        << BUILD_LOG( kPacket_.m_usPageNum ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_BROWNIE_STORE_CATALOG_REQ )
{
    CHECK_AGIT_ENABLE;

    KEVENT_BROWNIE_STORE_CATALOG_ACK kPacket;
    SiKAgitHelper()->m_kBrownieStoreItems.GetCatalogs( kPacket.m_mapItemList );

    SEND_COMPRESS_PACKET( EVENT_BROWNIE_STORE_CATALOG_ACK, kPacket );

    START_LOG( clog, L"도우미상점 아이템 리스트 전달. Login : " << GetName() )
        << BUILD_LOG( kPacket.m_mapItemList.size() )
        << BUILD_LOG( kPacket.m_mapItemPos.size() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_BROWNIE_STORE_MATERIAL_REQ )
{
    CHECK_AGIT_ENABLE;

    std::map<PAIR_USHORT_DWORD,std::vector<KDropItemInfo> > mapMaterialList;
    mapMaterialList.clear();

    std::set<PAIR_USHORT_DWORD>::iterator sit;
    for ( sit = kPacket_.begin() ; sit != kPacket_.end() ; ++sit ) {
        std::vector<KDropItemInfo> vecMaterial;
        if ( false == SiKAgitHelper()->m_kBrownieStoreItems.GetMaterials( *sit, vecMaterial ) ) {
            START_LOG( cerr, L"해당 아이템의 구매재료가 등록되어있지 않음. ItemID : " << (*sit).second ) << END_LOG;
            continue;
        }

        //if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
        //    std::Warn << L"-- Brownie Store Material (size:" << vecMaterial.size() << L") --" << dbg::endl
        //        << L"Index : " << (*sit).first << L", ItemID" << (*sit).second << dbg::endl;

        //    std::vector<KDropItemInfo>::const_iterator cvit;
        //    for ( cvit = vecMaterial.begin() ; cvit != vecMaterial.end() ; ++cvit ) {
        //        std::Warn << L"(" << cvit->m_ItemID << L", " << cvit->m_nDuration << L"," << cvit->m_nPeriod << L"),";
        //    }
        //    std::Warn << dbg::endl;
        //}

        mapMaterialList[ *sit ].swap( vecMaterial );
    }

    SEND_COMPRESS_PACKET( EVENT_BROWNIE_STORE_MATERIAL_ACK, mapMaterialList );

    _LOG_SUCCESS( !mapMaterialList.empty(), L"도우미상점 아이템 구매재료 리스트 전달." )
        << BUILD_LOG( kPacket_.size() )
        << BUILD_LOG( mapMaterialList.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_BUY_FOR_BROWNIE_REQ, PAIR_USHORT_DWORD )
{
    CHECK_AGIT_ENABLE;

    std::vector<KDropItemInfo> vecMaterials;
    std::vector<KDropItemInfo>::iterator vit;

    KDB_EVENT_BUY_FOR_BROWNIE_REQ kDBPacket;
    kDBPacket.m_kCatalog.m_Index = kPacket_.first;
    kDBPacket.m_kCatalog.m_ItemID = kPacket_.second;

    KEVENT_BUY_FOR_BROWNIE_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nGP = GetGP();

    int nItemInvenCount = 0;

    SET_ERROR( ERR_UNKNOWN );

    // 구매 아이템 정보가 없음
    if ( false == SiKAgitHelper()->m_kBrownieStoreItems.GetCatalog( kPacket_, kDBPacket.m_kCatalog ) ) {
        SET_ERR_GOTO( ERR_AGIT_STORE_01, END_PROC );
    }

    // 구매 아이템 재료정보가 없음
    if ( false == SiKAgitHelper()->m_kBrownieStoreItems.GetMaterials( kPacket_, vecMaterials ) ) {
        SET_ERR_GOTO( ERR_AGIT_STORE_02, END_PROC );
    }

    for ( vit = vecMaterials.begin() ; vit != vecMaterials.end() ; ++vit ) {
        // 구매재료 아이템이 인벤에 없음.
        KItemPtr pkItem = m_kInventory.FindItemByItemID( vit->m_ItemID );
        if ( pkItem == NULL ) {
            SET_ERR_GOTO( ERR_AGIT_STORE_03, END_PROC );
        }

        if ( vit->m_nDuration == KItem::UNLIMITED_ITEM ) {
            // 구매재료 영구아이템이 없음.
            if ( pkItem->m_nCount != KItem::UNLIMITED_ITEM ||
                pkItem->m_nInitCount != KItem::UNLIMITED_ITEM ) {
                    SET_ERR_GOTO( ERR_AGIT_STORE_04, END_PROC );
            }

            kDBPacket.m_vecDelMaterials.push_back( pkItem->m_ItemUID );
        }
        else {
            // 구매재료 수량아이템의 수량이 부족함.
            if ( pkItem->m_nCount < vit->m_nDuration ) {
                SET_ERR_GOTO( ERR_AGIT_STORE_05, END_PROC );
            }
        }
    }

    // 인벤토리 보유량 제한 확인
    nItemInvenCount = m_kInventory.GetItemCount( kDBPacket.m_kCatalog.m_ItemID );
    if ( kDBPacket.m_kCatalog.m_nInvelLimit > 0 && nItemInvenCount >= kDBPacket.m_kCatalog.m_nInvelLimit ) {
        SET_ERR_GOTO( ERR_AGIT_STORE_12, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_BUY_FOR_BROWNIE_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,            kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_AGIT_STORE_01, kPacket.m_nOK, 1 ); // 구매 아이템 정보가 없음.
        ERR_CASE( ERR_AGIT_STORE_02, kPacket.m_nOK, 2 ); // 구매 아이템 재료정보가 없음.
        ERR_CASE( ERR_AGIT_STORE_03, kPacket.m_nOK, 3 ); // 구매 재료 아이템이 인벤에 없음.
        ERR_CASE( ERR_AGIT_STORE_04, kPacket.m_nOK, 4 ); // 구매재료 영구아이템이 없음.
        ERR_CASE( ERR_AGIT_STORE_05, kPacket.m_nOK, 5 ); // 구매재료 수량아이템의 수량이 부족함.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.first )
        << BUILD_LOG( kPacket_.second )
        << BUILD_LOG( vecMaterials.size() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nGP )
        << BUILD_LOG( kPacket.m_vecRewards.size() )
        << BUILD_LOG( kPacket.m_vecCountMaterialItems.size() )
        << BUILD_LOG( kPacket.m_vecPeriodMaterialItems.size() )
        << BUILD_LOG( nItemInvenCount )
        << BUILD_LOG( kDBPacket.m_kCatalog.m_nInvelLimit ) << END_LOG;

    if ( false == IS_CORRECT( NET_OK ) ) {
        SEND_PACKET( EVENT_BUY_FOR_BROWNIE_ACK );
    }
}

IMPL_ON_FUNC( EVENT_BUY_FOR_BROWNIE_ACK )
{
    CHECK_AGIT_ENABLE;

    std::vector< GCITEMUID >::iterator vit;
    NetError::SetLastNetError( kPacket_.m_nOK );

    kPacket_.m_nGP = GetGP();

    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    if ( !kPacket_.m_vecRewards.empty() ) {
        // 획득 아이템 넣기
        m_kInventory.AddItemList( kPacket_.m_vecRewards );

        // 구매재료 영구아이템 제거
        for ( vit = kPacket_.m_vecPeriodMaterialItems.begin() ; vit != kPacket_.m_vecPeriodMaterialItems.end() ; ++vit ) {
            LIF( m_kInventory.RemoveItem( *vit ) );
        }

        const KItem kItem = kPacket_.m_vecRewards.front();
        std::vector<KDropItemInfo> vecMaterials;

        if ( SiKAgitHelper()->m_kBrownieStoreItems.GetMaterials( kPacket_.m_prKey, vecMaterials ) ) {

            std::vector<KDropItemInfo>::iterator vitCount;
            for ( vitCount = vecMaterials.begin() ; vitCount != vecMaterials.end() ; ++vitCount ) {
                KItemPtr pkItem = m_kInventory.FindDurationItemByItemID( vitCount->m_ItemID );
                if ( !pkItem ) {
                    START_LOG( cwarn, L"구매재료 수량아이템이 인벤에 없음. ItemID : " << vitCount->m_ItemID ) << END_LOG;
                    continue;
                }
                // 구매재료 수량 아이템 삭감
                pkItem->m_nCount = std::max<int>( 0, pkItem->m_nCount - vitCount->m_nDuration );
                kPacket_.m_vecCountMaterialItems.push_back( *pkItem );
            }
        }
    }

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,            kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_AGIT_STORE_09, kPacket_.m_nOK, 9 ); // DB작업중 아이템 넣기 실패.
        ERR_CASE( ERR_AGIT_STORE_11, kPacket_.m_nOK, 11 ); // DB작업중 아지트 아이템 설정 실패.
        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nGP )
        << BUILD_LOG( kPacket_.m_prKey.first )
        << BUILD_LOG( kPacket_.m_prKey.second )
        << BUILD_LOG( kPacket_.m_vecRewards.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_BUY_FOR_BROWNIE_ACK );
}

_IMPL_ON_FUNC( EVENT_EQUIP_BROWNIE_REQ, KEquipBrownieReq )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 1, KUserFSM::STATE_AGIT ));
    kPacket_.m_dwUserUID = GetUID();

    KEquipBrownieAck kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwUserUID = GetUID();

    KItemPtr pkItem;

    SET_ERROR( ERR_UNKNOWN );

    // 자기 아지트에 있는가
    if ( m_dwStayAgitUID != GetUID() ) {
        SET_ERR_GOTO( ERR_AGIT_BROWNIE_01, END_PROC );
    }

    // 장착하려는 아이템을 보유하고 있는가
    pkItem = m_kInventory.FindItemByItemUID( kPacket_.m_kEquip.m_ItemUID );
    if ( pkItem == NULL ) {
        START_LOG( cwarn, L"도우미 장착 아이템이 인벤토리에 없음. Login : " << GetName() )
            << BUILD_LOG( kPacket_.m_kEquip.m_ItemID )
            << BUILD_LOG( kPacket_.m_kEquip.m_ItemUID ) << END_LOG;
        SET_ERR_GOTO( ERR_AGIT_BROWNIE_02, END_PROC );
    }

    // 도우미 아이템인가?
    if ( false == SiKAgitHelper()->IsBrownieItem( kPacket_.m_kEquip.m_ItemID ) ) {
        START_LOG( cwarn, L"도우미 아이템이 아니다. Login : " << GetName() )
            << BUILD_LOG( kPacket_.m_kEquip.m_ItemID )
            << BUILD_LOG( kPacket_.m_kEquip.m_ItemUID ) << END_LOG;
        SET_ERR_GOTO( ERR_AGIT_BROWNIE_03, END_PROC );
    }

    // 도우미 보유여부
    pkItem = m_kInventory.FindItemByItemUID( kPacket_.m_kBrownieInfo.m_dwUID );
    if ( pkItem == NULL ) {
        START_LOG( cwarn, L"도우미가 인벤토리에 없음. Login : " << GetName() )
            << BUILD_LOG( kPacket_.m_kBrownieInfo.m_dwUID ) << END_LOG;
        SET_ERR_GOTO( ERR_AGIT_BROWNIE_05, END_PROC );
    }

    kPacket_.m_kEquip = *pkItem;
    SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID ), KAgentEvent::EAG_EQUIP_BROWNIE_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,             kPacket.m_nOK,  0 ); // 성공
        ERR_CASE( ERR_AGIT_BROWNIE_01, kPacket.m_nOK, 1 ); // 자기 아지트에서만 도우미 정보 변경 가능함.
        ERR_CASE( ERR_AGIT_BROWNIE_02, kPacket.m_nOK, 2 ); // 도우미 장착아이템이 인벤토리에 없음.
        ERR_CASE( ERR_AGIT_BROWNIE_03, kPacket.m_nOK, 3 ); // 장착아이템이 도우미 아이템이 아님.
        ERR_CASE( ERR_AGIT_BROWNIE_05, kPacket.m_nOK, 5 ); // 보유하지 않은 도우미.
    default:
        START_LOG( cerr, L"도우미아이템 장착요청 중 알수 없는 오류.. Name : " << m_strName )
            << L" Msg : " << NetError::GetLastNetErrMsg()  << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"도우미아이템 장착요청. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( kPacket_.m_kEquip.m_ItemID )
        << BUILD_LOG( kPacket_.m_kEquip.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kBrownieInfo.m_dwUID ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_EQUIP_BROWNIE_ACK );
    }
}

IMPL_ON_FUNC( EVENT_CHANGE_BROWNIE_BROAD )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    if ( m_dwStayAgitUID != kPacket_.m_dwAgitUID ) {
        START_LOG( cwarn, L"현재 내가 있는 아지트와 다른 아지트의 도우미변경 알림. Login : " << GetName() )
            << BUILD_LOG( m_dwStayAgitUID )
            << BUILD_LOG( kPacket_.m_dwAgitUID )
            << BUILD_LOG( kPacket_.m_kBrownieInfo.m_dwUID ) << END_LOG;
        return;
    }

    START_LOG( clog, L"내가 있는 아지트의 도우미 변경 알림. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_kBrownieInfo.m_dwUID )
        << BUILD_LOG( kPacket_.m_kBrownieInfo.m_bIsUsed )
        << BUILD_LOG( kPacket_.m_kBrownieInfo.m_EquipItemID )
        << BUILD_LOG( kPacket_.m_kBrownieInfo.m_EquipItemUID ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CHANGE_BROWNIE_BROAD );
}

IMPL_ON_FUNC_NOPARAM( EVENT_WATERING_FAIRY_TREE_REQ )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    SET_ERROR( ERR_UNKNOWN );

    KWateringFairyTreeAck kPacket;
    kPacket.m_nOK = -99;

    // 자기 아지트에 있는가
    if ( m_dwStayAgitUID != GetUID() ) {
        SET_ERR_GOTO( ERR_WATERING_FAIRY_TREE_01, END_PROC );
    }

    DWORD dwUserUID = GetUID();
    SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID ), KAgentEvent::EAG_WATERING_FAIRY_TREE_REQ, dwUserUID );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                     kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_WATERING_FAIRY_TREE_01, kPacket.m_nOK, 1 ); // 자기 아지트에서만 요정의 나무 물주기 가능.
    default:
        START_LOG( cerr, L"요정의 나무 물주기중 알수 없는 오류.. Name : " << m_strName )
            << L" Msg : " << NetError::GetLastNetErrMsg()  << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"요정의 나무 물주기. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_WATERING_FAIRY_TREE_ACK );
    }
}

IMPL_ON_FUNC( EVENT_CHANGE_FAIRY_TREE_BROAD )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    if ( m_dwStayAgitUID != kPacket_.m_dwAgitUID ) {
        START_LOG( cwarn, L"현재 내가 있는 아지트와 다른 아지트의 요정의나무 변경 알림. Login : " << GetName() )
            << BUILD_LOG( m_dwStayAgitUID )
            << BUILD_LOG( kPacket_.m_dwAgitUID ) << END_LOG;
    }

    START_LOG( clog, L"내가 있는 아지트의 요정의나무 변경 알림. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_kFairyTree.m_nLv )
        << BUILD_LOG( kPacket_.m_kFairyTree.m_dwExp )
        << BUILD_LOG( kPacket_.m_kFairyTree.m_tmLastGrowTime )
        << BUILD_LOG( kPacket_.m_kFairyTree.m_tmLastDecreaseTime )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CHANGE_FAIRY_TREE_BROAD );
}

_IMPL_ON_FUNC( EVENT_FAIRY_TREE_BUFF_NOT, KFairyTreeFruitBuff )
{
    CHECK_AGIT_ENABLE;

    CTime tmCurrent = CTime::GetCurrentTime();
    CTime tmBuffTime = CTime(kPacket_.m_tmBuffTime);

    // 버프 초기화
    m_mapFairyTreeFruitBuff.clear();

    // 버프적용 날짜확인
    if ( tmBuffTime.GetYear() != tmCurrent.GetYear() ||
         tmBuffTime.GetMonth() != tmCurrent.GetMonth() ||
         tmBuffTime.GetDay() != tmCurrent.GetDay() ) {

        // 초기화 진행
        kPacket_.m_nFruitType = KFairyTreeConfig::FBT_NONE;
        kPacket_.m_fBuffRate = 0.f;

        START_LOG( cwarn, L"요정의나무 버프적용날짜와 현재날짜가 다름. Login : " << GetName() )
            << BUILD_LOGtm( tmCurrent )
            << BUILD_LOGtm( tmBuffTime ) << END_LOG;
    }

    m_mapFairyTreeFruitBuff[kPacket_.m_nFruitType] = kPacket_.m_fBuffRate;

    START_LOG( clog, L"요정의나무 버프 적용. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_nFruitType )
        << BUILD_LOG( kPacket_.m_fBuffRate )
        << BUILD_LOGtm( tmBuffTime ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_FAIRY_TREE_BUFF_NOT );
}

IMPL_ON_FUNC_NOPARAM( EVENT_USE_FAIRY_TREE_FRUIT_REQ )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    SET_ERROR( ERR_UNKNOWN );

    KUseFairyTreeFruitAck kPacket;
    kPacket.m_nOK = -99;

    // 자기 아지트에 있는가
    if ( m_dwStayAgitUID != GetUID() ) {
        SET_ERR_GOTO( ERR_USE_FAIRY_TREE_FRUIT_01, END_PROC );
    }

    DWORD dwUserUID = GetUID();
    SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID ), KAgentEvent::EAG_USE_FAIRY_TREE_FRUIT_REQ, dwUserUID );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                      kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_USE_FAIRY_TREE_FRUIT_01, kPacket.m_nOK, 1 ); // 자기 아지트에서만 요정의나무 열매먹기 가능.
    default:
        START_LOG( cerr, L"요정의나무 열매먹기 중 알수 없는 오류.. Login : " << GetName() )
            << L" Msg : " << NetError::GetLastNetErrMsg() << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"요정의나무 열매먹기. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_USE_FAIRY_TREE_FRUIT_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_USE_FAIRY_TREE_FRUIT_ACK, KUseFairyTreeFruitAck )
{
    CTime tmCurrent = CTime::GetCurrentTime();
    CTime tmBuffTime = CTime(kPacket_.m_kFruitBuff.m_tmBuffTime);

    if ( kPacket_.m_nOK != 0 ) {
        goto END_PROC;
    }

    // 버프 초기화
    m_mapFairyTreeFruitBuff.clear();

    // 버프적용 날짜확인
    if ( tmBuffTime.GetYear() == tmCurrent.GetYear() && tmBuffTime.GetMonth() == tmCurrent.GetMonth() && tmBuffTime.GetDay() == tmCurrent.GetDay() ) {
        m_mapFairyTreeFruitBuff[kPacket_.m_kFruitBuff.m_nFruitType] = kPacket_.m_kFruitBuff.m_fBuffRate;

        START_LOG( clog, L"요정의나무 버프적용. Login : " << GetName() )
            << BUILD_LOGtm( tmBuffTime ) << END_LOG;
    }
    else {
        // 전달값 초기화.
        kPacket_.m_kFruitBuff.m_nFruitType = KFairyTreeConfig::FBT_NONE;
        kPacket_.m_kFruitBuff.m_fBuffRate = 0.f;
        kPacket_.m_kFruitBuff.m_tmBuffTime = KncUtil::TimeToInt(tmCurrent);

        START_LOG( cwarn, L"요정의나무 버프적용날짜와 현재날짜가 다름. Login : " << GetName() )
            << BUILD_LOGtm( tmCurrent )
            << BUILD_LOGtm( tmBuffTime ) << END_LOG;
    }

END_PROC:
    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"요정의나무 열매먹기 결과.. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kFruitBuff.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_kFruitBuff.m_nFruitType )
        << BUILD_LOG( kPacket_.m_kFruitBuff.m_fBuffRate )
        << BUILD_LOGtm( CTime(kPacket_.m_kFruitBuff.m_tmBuffTime) ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_USE_FAIRY_TREE_FRUIT_ACK );
}

_IMPL_ON_FUNC( EVENT_AGIT_INIT_SEED_ACK, KAgitBonusDrop )
{
    CHECK_AGIT_ENABLE;

    if ( kPacket_.m_vecItem.empty() ) {
        return;
    }

    // 아이템 인벤에 넣기
    m_kInventory.AddItemList( kPacket_.m_vecItem );

    START_LOG( clog, L"아지트 일일 초기화 아이템 지급받음. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOGtm( CTime(kPacket_.m_tmDropTime) )
        << BUILD_LOG( kPacket_.m_nDropCount )
        << BUILD_LOG( kPacket_.m_vecItem.size() )
        << BUILD_LOG( m_nTodaySeedDropCount ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_AGIT_INIT_SEED_NOT );
}

_IMPL_ON_FUNC( EVENT_DROP_FLOWER_POT_ACK, KAddItemAck )
{
    CHECK_AGIT_ENABLE;

    if ( kPacket_.m_vecAddItem.empty() ) {
        return;
    }

    // 아이템 인벤에 넣기
    m_kInventory.AddItemList( kPacket_.m_vecAddItem );

    START_LOG( clog, L"아지트 화분아이템 추가지급받음. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_vecAddItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_DROP_FLOWER_POT_NOT );
}

IMPL_ON_FUNC_NOPARAM( EVENT_AGIT_TUTORIAL_DONE_REQ )
{
    CHECK_AGIT_ENABLE;
    QUEUEING_ID_TO_DB( EVENT_AGIT_TUTORIAL_DONE_REQ );
}

_IMPL_ON_FUNC( EVENT_AGIT_TUTORIAL_DONE_ACK, bool )
{
    CHECK_AGIT_ENABLE;

    if ( false == kPacket_ && false == m_bAgitNewUser ) {
        std::vector<KDropItemInfo> vecDropItem;
        SiKAgitHelper()->GetTutorialItem( vecDropItem );
        QUEUEING_EVENT_TO_DB( EVENT_AGIT_TUTORIAL_REQ, vecDropItem );

        START_LOG( clog, L"아지트 입장했던 유저는 튜토리얼 아이템만 지급.. Login : " << GetName() )
            << BUILD_LOG( vecDropItem.size() ) << END_LOG;

        // 튜토리얼은 진행한 걸로 설정.
        kPacket_ = true;
    }

    SEND_RECEIVED_PACKET( EVENT_AGIT_TUTORIAL_DONE_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_AGIT_TUTORIAL_REQ )
{
    CHECK_AGIT_ENABLE;

    std::vector<KDropItemInfo> vecDropItem;
    SiKAgitHelper()->GetTutorialItem( vecDropItem );

    START_LOG( clog, L"튜토리얼 시작 알림.. Login : " << GetName() )
        << BUILD_LOG( vecDropItem.size() ) << END_LOG;

    QUEUEING_EVENT_TO_DB( EVENT_AGIT_TUTORIAL_REQ, vecDropItem );
}

IMPL_ON_FUNC( EVENT_AGIT_TUTORIAL_ACK )
{
    CHECK_AGIT_ENABLE;

    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    // 아이템 지급
    if ( !kPacket_.m_vecItem.empty() ) {
        m_kInventory.AddItemList( kPacket_.m_vecItem );
    }

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_AGIT_TUTORIAL_01, kPacket_.m_nOK, 1 ); // 이미 아지트 튜토리얼을 진행했음.
        ERR_CASE( ERR_AGIT_TUTORIAL_02, kPacket_.m_nOK, 2 ); // 아지트 튜토리얼용 아이템 DB지급 실패.
        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket_.m_nOK = -99;
    }

    _LOG_CONDITION( kPacket_.m_nOK == 0, clog, cwarn, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_AGIT_TUTORIAL_ACK );
}

_IMPL_ON_FUNC( EVENT_INVITE_AGIT_REQ, std::vector<DWORD> )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    if ( false == kPacket_.empty() ) {
        KInviteAgitReq kPacket;
        kPacket.m_dwAgitUID = m_dwStayAgitUID;
        kPacket.m_strOwnerNick = GetNickName();
        kPacket.m_vecUserUID = kPacket_;
        kPacket.m_ucAgitOpenType = m_ucAgitOpenType;
        kPacket.m_strAgitPassword = m_strAgitPassword;
        kPacket.m_bIsOwnerInvite = ( m_dwStayAgitUID == GetUID() ? true : false );

        SiKCnConnector()->SendPacket( KCenterEvent::ECN_INVITE_AGIT_REQ, kPacket );
    }

    START_LOG( clog, L"아지트 초대 요청.. Login : " << GetName() )
        << BUILD_LOG( kPacket_.size() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( GetUID() )
        << BUILD_LOGc( m_ucAgitOpenType )
        << BUILD_LOG( m_strAgitPassword ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_INVITE_AGIT_NOT, KInviteAgitNot )
{
    CHECK_AGIT_ENABLE;

    bool bSendInviteNot = false;

    if ( m_ucAgitInviteOption == KAgitInfo::IT_REJECT ) {
        m_ucAgitInviteOption = KAgitInfo::IT_ACCEPT;
        START_LOG( clog, L"아지트 초대 거부 설정 미구현으로 인해 초대 무조건 수락 m_ucAgitInviteOption = KAgitInfo::IT_ACCEPT" << GetName() ) << END_LOG;
    }

    // 유저에게 초대 알림
    if ( m_ucAgitInviteOption == KAgitInfo::IT_ACCEPT ) {
        bSendInviteNot = true;
        START_LOG( clog, L"초대수락 설정.. Login : " << GetName() ) << END_LOG;
    }
    else if ( m_ucAgitInviteOption == KAgitInfo::IT_FRIEND ) {
        std::vector<DWORD>::iterator vit;
        vit = std::find( m_vecOnlineFriends.begin(), m_vecOnlineFriends.end(), kPacket_.m_dwAgitUID );
        if ( vit != m_vecOnlineFriends.end() ) {
            bSendInviteNot = true;
            START_LOG( clog, L"친구수락 설정.. Login : " << GetName() ) << END_LOG;
        }
        else {
            bSendInviteNot = false;
            START_LOG( clog, L"친구수락인데 친구가 아님.. Login : " << GetName() ) << END_LOG;
        }
    }
    else if ( m_ucAgitInviteOption == KAgitInfo::IT_REJECT ) {
        bSendInviteNot = false;
        START_LOG( clog, L"초대거부 설정.. Login : " << GetName() ) << END_LOG;
    }
    else {
        bSendInviteNot = false;
        START_LOG( cwarn, L"정의되지않은 초대 옵션.. Option : " << (int)m_ucAgitInviteOption << L", Login : " << GetName() ) << END_LOG;
    }

    if ( bSendInviteNot ) {
        SEND_RECEIVED_PACKET( EVENT_INVITE_AGIT_NOT );
    }

    START_LOG( clog, L"아지트 초대 알림.. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOGc( kPacket_.m_ucAgitOpenType )
        << BUILD_LOG( kPacket_.m_strAgitPassword )
        << BUILD_LOG( kPacket_.m_bIsOwnerInvite )
        << BUILD_LOGc( m_ucAgitInviteOption )
        << BUILD_LOG( bSendInviteNot ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_ENTER_AGIT_SID_ACK, KEnterAgitSIDAck )
{
    CHECK_AGIT_ENABLE;

    SiKAgitHelper()->SetAgitSID( kPacket_.m_kEnterAgitReq.m_dwAgitUID, kPacket_.m_nSID );
    SEND_DATA_TO_AGENT( kPacket_.m_nSID, EAG_ENTER_AGIT_REQ, kPacket_.m_kEnterAgitReq );

    START_LOG( clog, L"아지트 입장요청2.. Login : " << GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( kPacket_.m_nSID )
        << BUILD_LOG( kPacket_.m_kEnterAgitReq.m_dwAgitUID )
        << BUILD_LOGc( kPacket_.m_kEnterAgitReq.m_cCharType )
        << BUILD_LOG( kPacket_.m_kEnterAgitReq.m_strPassword ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_CHANGE_AGIT_OPTION_BROAD, KAgitOption )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    if ( m_dwStayAgitUID != kPacket_.m_dwAgitUID ) {
        START_LOG( cwarn, L"현재 내가 있는 아지트와 다른 아지트의 옵션변경 알림. Login : " << GetName() )
            << BUILD_LOG( kPacket_.m_dwAgitUID )
            << BUILD_LOG( m_dwStayAgitUID ) << END_LOG;
        return;
    }

    // 아지트 정보 기억
    m_ucAgitOpenType = kPacket_.m_ucOpenType;
    m_strAgitPassword = kPacket_.m_strPassword;

    START_LOG( clog, L"내가 있는 아지트의 옵션변경 알림. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwAgitUID ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CHANGE_AGIT_OPTION_BROAD );
}

_IMPL_ON_FUNC( EVENT_AGIT_USER_OPTION_ACK, KAgitInfo )
{
    //m_ucAgitInviteOption = kPacket_.m_ucInviteType;
    // 아지트 초대 거부 기능 미구현으로 무조건 accept
    m_ucAgitInviteOption = KAgitInfo::IT_ACCEPT;
    START_LOG( clog, L"아지트 초대 거부 설정 미구현으로 인해 초대 무조건 수락 m_ucAgitInviteOption = KAgitInfo::IT_ACCEPT" << GetName() ) << END_LOG;
    START_LOG( clog, L"아지트 초대설정 변경. Login : " << GetName() << L", InviteOption : " << (int)m_ucAgitInviteOption ) << END_LOG;

    m_bAgitNewUser = ( kPacket_.m_nTotalVisiter == 0 ? true : false );
    START_LOG( clog, L"아지트 입장한 유저인가.. Login : " << GetName() << L", AgitNewUser : " << (m_bAgitNewUser?L"True":L"False") ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_GUESTBOOK_PAGE_REQ )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    KGuestBookPageList kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwAgitUID = m_dwStayAgitUID;
    kPacket.m_dwUserUID = GetUID();
    kPacket.m_buffCompList.Clear();

    KAgitUserParam kAgPacket;
    kAgPacket.m_dwAgitUID = m_dwStayAgitUID;
    kAgPacket.m_dwUserUID = GetUID();

    SET_ERROR( ERR_UNKNOWN );

    // 아지트인가
    if ( m_dwStayAgitUID == 0 ) {
        SET_ERR_GOTO( ERR_GUESTBOOK_01, END_PROC );
    }

    SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID ), KAgentEvent::EAG_GUESTBOOK_PAGE_REQ, kAgPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_GUESTBOOK_01, kPacket.m_nOK, 1 ); // 아지트 상태가 아님.
    default:
        START_LOG( cerr, L"낙서장 리스트 요청 중 알수 없는 오류.. Login : " << GetName() )
            << L" Msg : " << NetError::GetLastNetErrMsg() << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"낙서장 리스트 요청. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_GUESTBOOK_PAGE_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_GUESTBOOK_WRITE_REQ, KGuestBookWriteReq )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    kPacket_.m_dwUserUID = GetUID();
    kPacket_.m_dwAgitUID = m_dwStayAgitUID;
    kPacket_.m_strNick = GetNickName();

    KAgitUserParamAck kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwUserUID = GetUID();
    kPacket.m_dwAgitUID = m_dwStayAgitUID;

    std::wstring strNameCopy;

    SET_ERROR( ERR_UNKNOWN );

    // 아지트인가
    if ( m_dwStayAgitUID == 0 ) {
        SET_ERR_GOTO( ERR_GUESTBOOK_01, END_PROC );
    }

    // 쿼리문자 제거
    boost::replace_all( kPacket_.m_strMsg, L"'", L"" );

    // 공백 체크
    if ( kPacket_.m_strMsg.empty() ) {
        SET_ERR_GOTO( ERR_GUESTBOOK_03, END_PROC );
    }

    // 길이 체크
    if ( kPacket_.m_strMsg.size() > SiKAgitHelper()->GetMaxGuestbookMsgLenth() ) {
        SET_ERR_GOTO( ERR_GUESTBOOK_04, END_PROC );
    }

    // 불량단어
    strNameCopy = boost::to_lower_copy( kPacket_.m_strMsg );
    if( SiKSlangFilter()->HasSlang( strNameCopy ) ) {
        SET_ERR_GOTO( ERR_GUESTBOOK_05, END_PROC );
    }

    // 쓰기주기
    if ( ::GetTickCount() - GetTick( GUESTBOOK_TICK ) < SiKAgitHelper()->GetGuestbookWriteTerm() ) {
        SET_ERR_GOTO( ERR_GUESTBOOK_10, END_PROC );
    }
    SetTick( GUESTBOOK_TICK );

    SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID ), KAgentEvent::EAG_GUESTBOOK_WRITE_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_GUESTBOOK_01, kPacket.m_nOK, 1 ); // 아지트 상태가 아님.
        ERR_CASE( ERR_GUESTBOOK_03, kPacket.m_nOK, 3 ); // 낙서장에 추가할 글 내용이 비었음.
        ERR_CASE( ERR_GUESTBOOK_04, kPacket.m_nOK, 4 ); // 낙서장에 추가할 글 길이가 제한보다 길다.
        ERR_CASE( ERR_GUESTBOOK_05, kPacket.m_nOK, 5 ); // 낙서장에 추가할 글에 불량단어가 포함되어 있음.
        ERR_CASE( ERR_GUESTBOOK_10, kPacket.m_nOK, 10 ); //  낙서장 글쓰기 시간주기가 짧다.

    default:
        START_LOG( cerr, L"낙서장 글쓰기 요청 중 알수 없는 오류.. Login : " << GetName() )
            << L" Msg : " << NetError::GetLastNetErrMsg() << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"낙서장 글쓰기 요청. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_strNick )
        << BUILD_LOG( kPacket_.m_strMsg )
        << BUILD_LOG( ::GetTickCount() - GetTick( GUESTBOOK_TICK ) )
        << BUILD_LOG( SiKAgitHelper()->GetGuestbookWriteTerm() ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_GUESTBOOK_WRITE_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_GUESTBOOK_WRITE_BROAD, KGuestBookWriteBroad )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    if ( m_dwStayAgitUID != kPacket_.m_dwAgitUID ) {
        START_LOG( cwarn, L"현재 내가 있는 아지트와 다른 아지트의 낙서장 새글 알림. Login : " << GetName() )
            << BUILD_LOG( kPacket_.m_dwAgitUID )
            << BUILD_LOG( m_dwStayAgitUID )
            << BUILD_LOG( kPacket_.m_nNo ) << END_LOG;
        return;
    }

    START_LOG( clog, L"내가 있는 아지트의 낙서장 새글 알림. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_nNo )
        << BUILD_LOG( kPacket_.m_kGuestMsg.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_kGuestMsg.m_strNick )
        << BUILD_LOG( kPacket_.m_kGuestMsg.m_strMsg )
        << BUILD_LOGtm( CTime(kPacket_.m_kGuestMsg.m_tmDate) ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_GUESTBOOK_WRITE_BROAD );
}

_IMPL_ON_FUNC( EVENT_GUESTBOOK_DELETE_REQ, KGuestBookDeleteReq )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    kPacket_.m_dwUserUID = GetUID();
    kPacket_.m_dwAgitUID = m_dwStayAgitUID;

    KAgitUserParamAck kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwUserUID = GetUID();
    kPacket.m_dwAgitUID = m_dwStayAgitUID;

    SET_ERROR( ERR_UNKNOWN );

    // 아지트인가
    if ( m_dwStayAgitUID == 0 ) {
        SET_ERR_GOTO( ERR_GUESTBOOK_01, END_PROC );
    }

    // 낙서장 삭제할 글번호가 비어있음.
    if ( kPacket_.m_setDeleteNo.empty() ) {
        SET_ERR_GOTO( ERR_GUESTBOOK_07, END_PROC );
    }

    SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID ), KAgentEvent::EAG_GUESTBOOK_DELETE_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_GUESTBOOK_01, kPacket.m_nOK, 1 ); // 아지트 상태가 아님.
        ERR_CASE( ERR_GUESTBOOK_07, kPacket.m_nOK, 7 ); //  낙서장 삭제할 글번호가 비어있음.
    default:
        START_LOG( cerr, L"낙서장 글삭제 요청 중 알수 없는 오류.. Login : " << GetName() )
            << L" Msg : " << NetError::GetLastNetErrMsg() << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"낙서장 글삭제 요청. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( kPacket_.m_setDeleteNo.size() ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_GUESTBOOK_DELETE_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_GUESTBOOK_DELETE_BROAD, KGuestBookDeleteBroad )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    if ( m_dwStayAgitUID != kPacket_.m_dwAgitUID ) {
        START_LOG( cwarn, L"현재 내가 있는 아지트와 다른 아지트의 낙서장 글삭제 알림. Login : " << GetName() )
            << BUILD_LOG( kPacket_.m_dwAgitUID )
            << BUILD_LOG( m_dwStayAgitUID )
            << BUILD_LOG( kPacket_.m_setDeleteNo.size() ) << END_LOG;
        return;
    }

    START_LOG( clog, L"내가 있는 아지트의 낙서장 글삭제 알림. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_setDeleteNo.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_GUESTBOOK_DELETE_BROAD );
}

_IMPL_ON_FUNC( EVENT_USE_TRAINING_OBJ_REQ, KUseTrainingObjReq )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    kPacket_.m_dwUserUID = GetUID();

    KUseTrainingObjAck kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nCharType = kPacket_.m_nCharType;
    kPacket.m_dwUserUID = GetUID();
    kPacket.m_kTrainingObj.m_ItemUID = kPacket_.m_TrainingObjUID;
    kPacket.m_nUseMaterialSize = 0;

    KItemPtr pkItem;

    SET_ERROR( ERR_UNKNOWN );

    if ( GetCurrentCharType() != kPacket_.m_nCharType ) {
        START_LOG( cerr, L"서버-클라이언트의 현재 캐릭터가 다르다, User: " << GetName() )
            << END_LOG;
        SET_ERR_GOTO( ERR_TRAINING_OBJ_02, END_PROC );
    }

    // 내 아지트인가
    if ( m_dwStayAgitUID != GetUID() ) {
        SET_ERR_GOTO( ERR_TRAINING_OBJ_01, END_PROC );
    }

    // 보유중인 캐릭터인가
    if ( false == IsCharExist( (char)kPacket_.m_nCharType ) ) {
        SET_ERR_GOTO( ERR_TRAINING_OBJ_02, END_PROC );
    }

    // 훈련소 아이템을 인벤토리에 보유중인가
    pkItem = m_kInventory.FindItemByItemUID( kPacket_.m_TrainingObjUID );
    if ( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_TRAINING_OBJ_03, END_PROC );
    }

    // 훈련소 아이템인가?
    if ( pkItem->m_ItemID != SiKAgitHelper()->TrainingItemID() ) {
        SET_ERR_GOTO( ERR_TRAINING_OBJ_04, END_PROC );
    }

    // 재료 아이템을 인벤토리에 보유중인가
    pkItem = m_kInventory.FindItemByItemID( SiKAgitHelper()->TrainingMaterialItemID() );
    if ( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_TRAINING_OBJ_05, END_PROC );
    }

    // 재료 아이템 수량 체크
    if ( pkItem->m_nCount <= 0 ) {
        SET_ERR_GOTO( ERR_TRAINING_OBJ_06, END_PROC );
    }
    kPacket_.m_kMaterialItem = *pkItem;

    SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID ), KAgentEvent::EAG_USE_TRAINING_OBJ_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,              kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_TRAINING_OBJ_01, kPacket.m_nOK, 1 ); // 내 아지트가 아니다.
        ERR_CASE( ERR_TRAINING_OBJ_02, kPacket.m_nOK, 2 ); // 보유중인 캐릭터가 아니다.
        ERR_CASE( ERR_TRAINING_OBJ_03, kPacket.m_nOK, 3 ); // 훈련소 아이템이 인벤토리에 없음.
        ERR_CASE( ERR_TRAINING_OBJ_04, kPacket.m_nOK, 4 ); // 훈련소 아이템이 아니다.
        ERR_CASE( ERR_TRAINING_OBJ_05, kPacket.m_nOK, 5 ); // 재료 아이템이 인벤토리에 없음.
        ERR_CASE( ERR_TRAINING_OBJ_06, kPacket.m_nOK, 6 ); // 재료 아이템 수량이 부족함.
    default:
        START_LOG( cerr, L"훈련소 사용 요청 중 알수 없는 오류.. Login : " << GetName() )
            << L" Msg : " << NetError::GetLastNetErrMsg() << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"훈련소 사용 요청. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_nCharType )
        << BUILD_LOG( kPacket_.m_TrainingObjUID )
        << BUILD_LOG( kPacket_.m_kMaterialItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kMaterialItem.m_nCount )
        << BUILD_LOG( SiKAgitHelper()->TrainingItemID() )
        << BUILD_LOG( SiKAgitHelper()->TrainingMaterialItemID() ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_USE_TRAINING_OBJ_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_USE_TRAINING_OBJ_ACK, KUseTrainingObjAck )
{
    CHECK_AGIT_ENABLE;

    KItemPtr pkItem;
    KCharacterInfo* pkCharInfo;

    __int64 biMaxExp = SiKResultManager()->GetExpByLv( SiKResultManager()->GetMaxLevel() );

    if ( kPacket_.m_nOK != 0 ) {
        goto END_PROC;
    }

    // 사용한 장작 아이템 차감
    pkItem = m_kInventory.FindItemByItemUID( kPacket_.m_kMaterialItem.m_ItemUID );
    if ( pkItem != NULL ) {
        pkItem->m_nCount = std::max<int>( 0, pkItem->m_nCount - kPacket_.m_nUseMaterialSize );
        kPacket_.m_kMaterialItem = *pkItem;
    }

    pkCharInfo = GetCharacterInfo( kPacket_.m_nCharType );
    if ( pkCharInfo != NULL ) {
        kPacket_.m_dwOldLevel = pkCharInfo->m_dwLevel;

        // 획득 경험치 = (캐릭터레벨에 해당하는 몬스터 경험치 * 3) * 훈련횟수
        int nExp = SiKGameDrop()->GetCharLvMonExp( pkCharInfo->m_dwLevel );
        float fExpRatio = SiKAgitHelper()->GetTrainingExpRatio();
        kPacket_.m_fRewardExp = nExp * fExpRatio * kPacket_.m_nUseMaterialSize;
        kPacket_.m_fRewardExp = std::max<float>( kPacket_.m_fRewardExp, 0.f );

        // 획득 경험치 적용
        pkCharInfo->m_biExp += static_cast<__int64>( kPacket_.m_fRewardExp );
        pkCharInfo->m_biTotalExp += static_cast<__int64>( kPacket_.m_fRewardExp );

        // EXP클립핑
        pkCharInfo->m_biExp = std::min<__int64>( pkCharInfo->m_biExp, biMaxExp );
        pkCharInfo->m_biExp = std::max<__int64>( pkCharInfo->m_biExp, USER_MIN_EXP );
        // TotalExp( 누적 경험치 )
        pkCharInfo->m_biTotalExp = std::min<__int64>( pkCharInfo->m_biTotalExp, _I64_MAX );
        pkCharInfo->m_biTotalExp = std::max<__int64>( pkCharInfo->m_biTotalExp, USER_MIN_EXP );
        // 레벨 계산.
        pkCharInfo->m_dwLevel = SiKResultManager()->Exp_2_Level( pkCharInfo->m_biExp );

        // 캐릭터의 Levelup을 달성했을 경우에 체크하자.
        if( kPacket_.m_dwOldLevel < pkCharInfo->m_dwLevel ) {
            // SPX 계산
            SiKSkillManager()->GetSkillPointInfo( kPacket_.m_nCharType, pkCharInfo->m_cPromotion, pkCharInfo->m_dwLevel, pkCharInfo->m_kSkillInfo.m_nLvSPPoint );

            // 상하이 드랍 체크
            LevelUpDrop( GetThisPtr(), kPacket_.m_nCharType, static_cast<char>(kPacket_.m_dwOldLevel), static_cast<char>(pkCharInfo->m_dwLevel) );
            KEVENT_SHANGHAI_DROP_REQ kDBPacket;
            kDBPacket.m_vecAutoMission.clear();
            kDBPacket.m_vecDropItems.clear();
            kDBPacket.m_vecDropPostItems.clear();
            kDBPacket.m_cCharType = pkCharInfo->m_cCharType;
            CheckLevelUpDrop( kDBPacket );

            // 전직 미션 가이드 정보 체크
            CheckJobMissionGuide( GetUID(), pkCharInfo->m_cCharType, static_cast< int >( pkCharInfo->m_dwLevel ) );

            // 봉인 캐릭터 정보 갱신
            UpdateUserSealedCharInfo( GetThisPtr(), pkCharInfo->m_cCharType, static_cast< int >( pkCharInfo->m_dwLevel ) );

            if( SiKGCHelper()->IsCharLevelEvent() ) { // 레벨업을 했을 경우에..,
                KDB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ kDBPacket;
                if( SiKGCHelper()->GetCharLevelRewardItem( pkCharInfo->m_cCharType, pkCharInfo->m_dwLevel, m_setCharLevelEvent, kDBPacket.m_vecRewardInfo ) ) {
                    kDBPacket.m_setCharInfo.insert( pkCharInfo->m_cCharType );
                    QUEUEING_EVENT_TO_DB( DB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ, kDBPacket );
                }
            }
        }

        // 클라이언트 전달용 패킷
        kPacket_.m_biExp = pkCharInfo->m_biExp;
        kPacket_.m_biTotalExp = pkCharInfo->m_biTotalExp;
        kPacket_.m_dwLevel = pkCharInfo->m_dwLevel;
        kPacket_.m_kSkillInfo = pkCharInfo->m_kSkillInfo;

        START_LOG( clog, L"훈련소 경험치 획득적용.. LoginID : " << GetName() ) << END_LOG;
    }

END_PROC:
    _LOG_CONDITION( kPacket_.m_nOK == 0, clog, cwarn, L"훈련소 사용결과.. Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nCharType )
        << BUILD_LOG( kPacket_.m_biExp )
        << BUILD_LOG( kPacket_.m_biTotalExp )
        << BUILD_LOG( kPacket_.m_dwLevel )
        << BUILD_LOG( kPacket_.m_dwOldLevel )
        << BUILD_LOG( kPacket_.m_fRewardExp )
        << BUILD_LOG( kPacket_.m_kMaterialItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kMaterialItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kMaterialItem.m_nCount )
        << BUILD_LOG( kPacket_.m_kSkillInfo.m_nLvSPPoint )
        << BUILD_LOG( kPacket_.m_kSkillInfo.m_nMaxSPPoint ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_USE_TRAINING_OBJ_ACK );
}

_IMPL_ON_FUNC( EVENT_UPDATE_TRAINING_OBJ_BROAD, KUpdateTrainingObjBroad )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    if ( m_dwStayAgitUID != kPacket_.m_dwAgitUID ) {
        START_LOG( cwarn, L"현재 내가 있는 아지트와 다른 아지트의 훈련소의 상태변경 알림. Login : " << GetName() )
            << BUILD_LOG( kPacket_.m_dwAgitUID )
            << BUILD_LOG( m_dwStayAgitUID ) << END_LOG;
        return;
    }

    START_LOG( clog, L"내가 있는 아지트의 훈련소 상태변경 알림. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_kObject.m_nAccCount )
        << BUILD_LOG( kPacket_.m_kObject.m_ItemUID ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_UPDATE_TRAINING_OBJ_BROAD );
}

_IMPL_ON_FUNC( EVENT_OPEN_AGIT_CHARACTER_REQ, int )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    PAIR_DWORD_INT kAgPacket;
    kAgPacket.first = GetUID();
    kAgPacket.second = kPacket_;

    KOpenAgitCharacterAck kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nCharType = kPacket_;
    kPacket.m_dwUserUID = GetUID();

    KItemPtr pkItem;

    SET_ERROR( ERR_UNKNOWN );

    // 내 아지트인가
    if ( m_dwStayAgitUID != GetUID() ) {
        SET_ERR_GOTO( ERR_AGIT_CHARACTER_01, END_PROC );
    }

    // 보유중인 캐릭터인가
    if ( false == IsCharExist( (char)kPacket.m_nCharType ) ) {
        SET_ERR_GOTO( ERR_AGIT_CHARACTER_02, END_PROC );
    }

    // 재료는 가지고 있는가?
    pkItem = m_kInventory.FindItemByItemID( SiKAgitHelper()->GetFlowerItemID() );
    if ( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_AGIT_CHARACTER_03, END_PROC );
    }
    kPacket.m_kFlowerItem = *pkItem;

    // 재료 아이템 수량 체크
    if ( pkItem->m_nCount < SiKAgitHelper()->GetCharacterOpenCost() ) {
        SET_ERR_GOTO( ERR_AGIT_CHARACTER_04, END_PROC );
    }

    SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID ), KAgentEvent::EAG_OPEN_AGIT_CHARACTER_REQ, kAgPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_AGIT_CHARACTER_01, kPacket.m_nOK, 1 ); // 내 아지트가 아니다.
        ERR_CASE( ERR_AGIT_CHARACTER_02, kPacket.m_nOK, 2 ); // 보유중인 캐릭터가 아니다.
        ERR_CASE( ERR_AGIT_CHARACTER_03, kPacket.m_nOK, 3 ); // 오픈하기 위한 별사탕이 인벤에 없음.
        ERR_CASE( ERR_AGIT_CHARACTER_04, kPacket.m_nOK, 4 ); // 오픈하기 위한 별사탕 수량이 부족함.
    default:
        START_LOG( cerr, L"캐릭터배치 오픈 요청 중 알수 없는 오류.. Login : " << GetName() )
            << L" Msg : " << NetError::GetLastNetErrMsg() << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"캐릭터배치 오픈 요청. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( kPacket.m_nCharType )
        << BUILD_LOG( kPacket.m_dwUserUID )
        << BUILD_LOG( SiKAgitHelper()->GetFlowerItemID() )
        << BUILD_LOG( SiKAgitHelper()->GetCharacterOpenCost() )
        << BUILD_LOG( kPacket.m_kFlowerItem.m_ItemID )
        << BUILD_LOG( kPacket.m_kFlowerItem.m_ItemUID )
        << BUILD_LOG( kPacket.m_kFlowerItem.m_nCount ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_OPEN_AGIT_CHARACTER_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_OPEN_AGIT_CHARACTER_ACK, KOpenAgitCharacterAck )
{
    CHECK_AGIT_ENABLE;

    KItemPtr pkItem;

    if ( kPacket_.m_nOK != 0 ) {
        goto END_PROC;
    }

    pkItem = m_kInventory.FindItemByItemID( SiKAgitHelper()->GetFlowerItemID() );
    if ( pkItem == NULL ) {
        START_LOG( cerr, L"캐릭터오픈 후, 감소시킬 재료아이템을 찾을 수 없음. Login : " << GetName() ) << END_LOG;
        goto END_PROC;
    }

    // 꽃 수량 갱신
    pkItem->m_nCount -= SiKAgitHelper()->GetCharacterOpenCost();
    pkItem->m_nCount = std::max<int>( pkItem->m_nCount, 0 );
    kPacket_.m_kFlowerItem = *pkItem;

END_PROC:
    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"캐릭터오픈 결과 받음. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_nCharType )
        << BUILD_LOG( SiKAgitHelper()->GetFlowerItemID() )
        << BUILD_LOG( SiKAgitHelper()->GetCharacterOpenCost() )
        << BUILD_LOG( kPacket_.m_kFlowerItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kFlowerItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kFlowerItem.m_nCount ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_OPEN_AGIT_CHARACTER_ACK );
}

_IMPL_ON_FUNC( EVENT_SET_AGIT_CHARACTER_POS_REQ, KSetAgitCharPosReq )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    kPacket_.m_dwAgitUID = GetUID();

    KSetAgitCharPosAck kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwAgitUID = GetUID();
    kPacket.m_vecCharPos = kPacket_.m_vecCharPos;

    std::vector<KSimpleAgitCharPos>::iterator vit;

    SET_ERROR( ERR_UNKNOWN );

    // 내 아지트인가
    if ( m_dwStayAgitUID != GetUID() ) {
        SET_ERR_GOTO( ERR_AGIT_CHARACTER_01, END_PROC );
    }

    // 보유중인 캐릭터인가
    for ( vit = kPacket_.m_vecCharPos.begin() ; vit != kPacket_.m_vecCharPos.end() ; ++vit ) {
        if ( false == IsCharExist( static_cast<char>(vit->m_nCharType) ) ) {
            START_LOG( cerr, L"배치하려는 캐릭터가 보유중인 캐릭터가 아니다.. Login : " << GetName() << L", CharType : " << vit->m_nCharType ) << END_LOG;
            SET_ERR_GOTO( ERR_AGIT_CHARACTER_02, END_PROC );
        }

        // 최소값 보장
        vit->m_nPosX = std::max<int>( vit->m_nPosX, -1 );
        vit->m_nPosY = std::max<int>( vit->m_nPosY, -1 );
    }

    SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID ), KAgentEvent::EAG_SET_AGIT_CHARACTER_POS_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_AGIT_CHARACTER_01, kPacket.m_nOK, 1 ); // 내 아지트가 아니다.
        ERR_CASE( ERR_AGIT_CHARACTER_02, kPacket.m_nOK, 2 ); // 보유중인 캐릭터가 아니다.

    default:
        START_LOG( cerr, L"캐릭터배치 위치변경 요청 중 알수 없는 오류.. Login : " << GetName() )
            << L" Msg : " << NetError::GetLastNetErrMsg() << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"캐릭터배치 위치변경 요청. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_vecCharPos.size() ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_SET_AGIT_CHARACTER_POS_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_SET_AGIT_CHARACTER_MOTION_REQ, KSetAgitCharMotionReq )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    kPacket_.m_dwAgitUID = GetUID();

    KSetAgitCharMotionAck kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwAgitUID = GetUID();
    kPacket.m_kCharInfo.m_nCharType = kPacket_.m_nCharType;

    SET_ERROR( ERR_UNKNOWN );

    // 내 아지트인가
    if ( m_dwStayAgitUID != GetUID() ) {
        SET_ERR_GOTO( ERR_AGIT_CHARACTER_01, END_PROC );
    }

    // 보유중인 캐릭터인가
    if ( false == IsCharExist( static_cast<char>(kPacket_.m_nCharType) ) ) {
        SET_ERR_GOTO( ERR_AGIT_CHARACTER_02, END_PROC );
    }

    SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID ), KAgentEvent::EAG_SET_AGIT_CHARACTER_MOTION_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_AGIT_CHARACTER_01, kPacket.m_nOK, 1 ); // 내 아지트가 아니다.
        ERR_CASE( ERR_AGIT_CHARACTER_02, kPacket.m_nOK, 2 ); // 보유중인 캐릭터가 아니다.
    default:
        START_LOG( cerr, L"배치캐릭터 모션변경 요청 중 알수 없는 오류.. Login : " << GetName() )
            << L" Msg : " << NetError::GetLastNetErrMsg() << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"배치캐릭터 모션변경 요청. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_nCharType )
        << BUILD_LOG( kPacket_.m_nMotionID ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_SET_AGIT_CHARACTER_MOTION_ACK );
    }
}

IMPL_ON_FUNC( EVENT_SET_AGIT_CHARACTER_COORDI_REQ )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    KItemPtr pkItem;
    std::map<DWORD,GCITEMUID>::iterator mit;

    KSetAgitCharCoordiReq kAgPacket;
    kAgPacket.m_dwAgitUID = GetUID();
    kAgPacket.m_nCharType = kPacket_.m_nCharType;
    kAgPacket.m_mapSlotCoordiItemUID.clear();

    KSetAgitCharCoordiAck kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwAgitUID = GetUID();
    kPacket.m_kCharInfo.m_nCharType = kPacket_.m_nCharType;

    SET_ERROR( ERR_UNKNOWN );

    // 내 아지트인가
    if ( m_dwStayAgitUID != GetUID() ) {
        SET_ERR_GOTO( ERR_AGIT_CHARACTER_01, END_PROC );
    }

    // 보유중인 캐릭터인가
    if ( false == IsCharExist( static_cast<char>(kPacket_.m_nCharType) ) ) {
        SET_ERR_GOTO( ERR_AGIT_CHARACTER_02, END_PROC );
    }

    // 인벤토리 확인
    for ( mit = kPacket_.m_mapSlotItemUID.begin() ; mit != kPacket_.m_mapSlotItemUID.end() ; ++mit ) {
        KSimpleCoordiItem kSimpleCoordiItem;
        pkItem = m_kInventory.FindItemByItemUID( mit->second );
        if ( pkItem == NULL ) {
            START_LOG( cerr, L"장착시킬 코디 아이템을 인벤토리에서 찾을 수 없음.. LoginID : " << GetName() )
                << BUILD_LOG( mit->first ) // SlotID
                << BUILD_LOG( mit->second ) << END_LOG; // ItemUID
            SET_ERR_GOTO( ERR_AGIT_CHARACTER_08, END_PROC );
        }

        kSimpleCoordiItem.m_ItemID = pkItem->m_ItemID;
        kSimpleCoordiItem.m_ItemUID = pkItem->m_ItemUID;
        kSimpleCoordiItem.m_DesignCoordiID = pkItem->m_DesignCoordiID;
        kAgPacket.m_mapSlotCoordiItemUID[mit->first] = kSimpleCoordiItem;
    }

    SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID ), KAgentEvent::EAG_SET_AGIT_CHARACTER_COORDI_REQ, kAgPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_AGIT_CHARACTER_01, kPacket.m_nOK, 1 ); // 내 아지트가 아니다.
        ERR_CASE( ERR_AGIT_CHARACTER_02, kPacket.m_nOK, 2 ); // 보유중인 캐릭터가 아니다.
        ERR_CASE( ERR_AGIT_CHARACTER_08, kPacket.m_nOK, 8 ); // 장착시킬 코디 아이템을 인벤토리에서 찾을 수 없음.
        //ERR_CASE( ERR_AGIT_CHARACTER_09, kPacket.m_nOK, 9 ); // 장착 요청할 코디정보가 비어있음.
    default:
        START_LOG( cerr, L"배치캐릭터 코디장착 요청 중 알수 없는 오류.. Login : " << GetName() )
            << L" Msg : " << NetError::GetLastNetErrMsg() << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"배치캐릭터 코디장착 요청. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_nCharType )
        << BUILD_LOG( kPacket_.m_mapSlotItemUID.size() ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_SET_AGIT_CHARACTER_COORDI_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_SET_AGIT_CHARACTER_SPEECH_REQ, KSetAgitCharSpeechReq )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    kPacket_.m_dwAgitUID = GetUID();

    KSetAgitCharSpeechAck kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwAgitUID = GetUID();
    kPacket.m_kCharInfo.m_nCharType = kPacket_.m_nCharType;

    USHORT usSpeechLength = SiKAgitHelper()->GetCharacterSpeechLength();
    std::wstring strNameCopy;
    std::map<USHORT,std::wstring>::iterator mit;

    SET_ERROR( ERR_UNKNOWN );

    // 내 아지트인가
    if ( m_dwStayAgitUID != GetUID() ) {
        SET_ERR_GOTO( ERR_AGIT_CHARACTER_01, END_PROC );
    }

    // 보유중인 캐릭터인가
    if ( false == IsCharExist( static_cast<char>(kPacket_.m_nCharType) ) ) {
        SET_ERR_GOTO( ERR_AGIT_CHARACTER_02, END_PROC );
    }

    // 설정할 대사정보가 비어있음.
    if ( kPacket_.m_mapSpeech.empty() ) {
        SET_ERR_GOTO( ERR_AGIT_CHARACTER_10, END_PROC );
    }

    for ( mit = kPacket_.m_mapSpeech.begin() ; mit != kPacket_.m_mapSpeech.end() ; ++mit ) {
        // 불량단어 검사
        strNameCopy = boost::to_lower_copy( mit->second );
        if( SiKSlangFilter()->HasSlang( strNameCopy ) ) {
            START_LOG( cwarn, L"설정할 배치캐릭터 대사에 불량단어가 포함됨.. LoginID : " << GetName() )
                << BUILD_LOG( mit->first )
                << BUILD_LOG( mit->second )
                << BUILD_LOG( strNameCopy ) << END_LOG;
            SET_ERR_GOTO( ERR_AGIT_CHARACTER_11, END_PROC );
        }

        // 쿼리문자
        // boost::replace_all( mit->second, L"'", L"" ); // AGDBThread에서 ' 를 '' 로 변환하여 저장하도록 수정함.
        // AGDBThread.cpp EAG_SET_AGIT_CHARACTER_SPEECH_REQ 처리부분..

        // 길이제한
        if ( mit->second.size() > usSpeechLength ) {
            START_LOG( cwarn, L"설정할 배치캐릭터 대사 길이가 제한에 걸림.. LoginID : " << GetName() )
                << BUILD_LOG( mit->first )
                << BUILD_LOG( mit->second )
                << BUILD_LOG( mit->second.size() )
                << BUILD_LOG( usSpeechLength ) << END_LOG;
            SET_ERR_GOTO( ERR_AGIT_CHARACTER_12, END_PROC );
        }
    }

    SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID ), KAgentEvent::EAG_SET_AGIT_CHARACTER_SPEECH_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_AGIT_CHARACTER_01, kPacket.m_nOK, 1 ); // 내 아지트가 아니다.
        ERR_CASE( ERR_AGIT_CHARACTER_02, kPacket.m_nOK, 2 ); // 보유중인 캐릭터가 아니다.
        ERR_CASE( ERR_AGIT_CHARACTER_10, kPacket.m_nOK, 10 ); // 설정할 대사정보가 비어있음.
        ERR_CASE( ERR_AGIT_CHARACTER_11, kPacket.m_nOK, 11 ); // 설정할 대사에 사용할 수 없는 단어가 있음.
        ERR_CASE( ERR_AGIT_CHARACTER_12, kPacket.m_nOK, 12 ); // 설정할 대사 길이가 제한에 걸림.
    default:
        START_LOG( cerr, L" 요청 중 알수 없는 오류.. Login : " << GetName() )
            << L" Msg : " << NetError::GetLastNetErrMsg() << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L" 요청. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( m_dwStayAgitUID )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_nCharType )
        << BUILD_LOG( kPacket_.m_mapSpeech.size() ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_SET_AGIT_CHARACTER_SPEECH_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_UPDATE_AGIT_CHARACTER_BROAD, KUpdateAgitCharBroad )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    if ( m_dwStayAgitUID != kPacket_.m_dwAgitUID ) {
        START_LOG( cwarn, L"현재 내가 있는 아지트와 다른 아지트의 배치캐릭터 상태변경 알림. Login : " << GetName() )
            << BUILD_LOG( kPacket_.m_dwAgitUID )
            << BUILD_LOG( m_dwStayAgitUID ) << END_LOG;
        return;
    }

    START_LOG( clog, L"내가 있는 아지트의 배치캐릭터 상태변경 알림. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_nCharType )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_nMotionID )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_nPosX )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_nPosY )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_mapCoordi.size() )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_mapSpeech.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_UPDATE_AGIT_CHARACTER_BROAD );
}

_IMPL_ON_FUNC( EVENT_UPDATE_AGIT_CHARACTER_POS_BROAD, KSetAgitCharPosReq )
{
    CHECK_AGIT_ENABLE;
    VERIFY_STATE(( 2, KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) );

    if ( m_dwStayAgitUID != kPacket_.m_dwAgitUID ) {
        START_LOG( cwarn, L"현재 내가 있는 아지트와 다른 아지트의 위치캐릭터 상태변경 알림. Login : " << GetName() )
            << BUILD_LOG( kPacket_.m_dwAgitUID )
            << BUILD_LOG( m_dwStayAgitUID ) << END_LOG;
        return;
    }

    START_LOG( clog, L"내가 있는 아지트의 배치캐릭터 위치변경 알림. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_vecCharPos.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_UPDATE_AGIT_CHARACTER_POS_BROAD );
}

IMPL_ON_FUNC_NOPARAM( EVENT_FAIRY_TREE_LV_TABLE_REQ )
{
    KFairyTreeConfig kConfig;
    SiKAgitHelper()->GetFairyTreeConfig( kConfig );

    START_LOG( clog, L"요정의나무 경험치테이블 전달.. Login : " << GetName() << L", Table size : " << kConfig.m_mapLvExp.size() ) << END_LOG;
    SEND_DATA( EVENT_FAIRY_TREE_LV_TABLE_ACK, kConfig.m_mapLvExp );
}

IMPL_ON_FUNC( EVENT_SHUTDOWN_ALARM_NOT )
{
    // 셧다운 대상 유저가 아니다.
    if ( GetUserAge() < kPacket_.m_prAge.first || GetUserAge() > kPacket_.m_prAge.second ) {
        START_LOG( clog, L"셧다운 대상 유저가 아니다.. LoginID : " << GetName() )
            << BUILD_LOG( GetUserAge() )
            << BUILD_LOG( kPacket_.m_prAge.first )
            << BUILD_LOG( kPacket_.m_prAge.second ) << END_LOG;
        return;
    }

    // 클라이언트 디스플레이용으로 +1 해줍니다.
    kPacket_.m_prAge.second += 1;
    START_LOG( clog, L"셧다운 대상유저에게 알림 메세지 전달.. LoginID : " << GetName() )
        << BUILD_LOG( GetUserAge() )
        << BUILD_LOG( kPacket_.m_prAge.first )
        << BUILD_LOG( kPacket_.m_prAge.second )
        << BUILD_LOG( kPacket_.m_usPrevTime )
        << BUILD_LOG( kPacket_.m_prStartTime.first )
        << BUILD_LOG( kPacket_.m_prStartTime.second ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SHUTDOWN_ALARM_NOT );
}

_IMPL_ON_FUNC( EVENT_NMNETCAFE_LOGIN_ACK, KNmNetcafeParamAck )
{
    m_bNMNetcafeActive = (kPacket_.m_DeductType == 0 ? false : true);

    if ( m_bNMNetcafeActive ) {
        KEVENT_NMNETCAFE_BONUS_REQ kDBPacket;
        kDBPacket.m_mapCharInfo = m_mapCharacterInfo;
        kDBPacket.m_mapPetInfo = m_mapPetInfo;
        QUEUEING_EVENT_TO_DB( EVENT_NMNETCAFE_BONUS_REQ, kDBPacket );

        START_LOG( clog, L"넷마블PC방 혜택 DB에 요청.. Login : " << GetName() )
            << BUILD_LOG( kDBPacket.m_mapCharInfo.size() )
            << BUILD_LOG( kDBPacket.m_mapPetInfo.size() ) << END_LOG;
    }

    START_LOG( clog, L"넷마블 PC방 로그인 응답/ 차감갱신.. Login : " << GetName() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( KncUtil::toWideString( GetIPStr() ) )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_UserCN ) )
        << BUILD_LOG( kPacket_.m_Result )
        << BUILD_LOG( kPacket_.m_DeductType )
        << BUILD_LOG( kPacket_.m_RestPoint )
        << BUILD_LOG( kPacket_.m_uiDecPlayTime )
        << BUILD_LOG( m_bNMNetcafeActive ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_NMNETCAFE_EXPIRE_NOT, KNmNetcafeParamAck )
{
    m_bNMNetcafeActive = false;

    if ( kPacket_.m_Result != 0 && kPacket_.m_DeductType != 0 ) {
        m_bNMNetcafeActive = true;
    }

    START_LOG( clog, L"넷마블 PC방 혜택만료 알림.. Login : " << GetName() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( KncUtil::toWideString( GetIPStr() ) )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_UserCN ) )
        << BUILD_LOG( kPacket_.m_Result )
        << BUILD_LOG( kPacket_.m_DeductType )
        << BUILD_LOG( kPacket_.m_RestPoint )
        << BUILD_LOG( m_bNMNetcafeActive ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_NMNETCAFE_BONUS_ACK )
{
    // PC방 설정
    m_cPCBangType = KIPAdvantage::PCBANG_PUBLISHER;

    // 아이템 넣기
    m_kInventory.AddItemList( kPacket_.m_vecReward );

    // 펫정보 갱신
    m_mapPetInfo = kPacket_.m_mapPetInfo;
    SEND_RECEIVED_PACKET( EVENT_NMNETCAFE_BONUS_ACK );

    START_LOG( clog, L"넷마블 PC방 혜택정보 전달.. LoginID : " << GetName() )
        << BUILD_LOG( kPacket_.m_vecReward.size() )
        << BUILD_LOG( kPacket_.m_mapPetInfo.size() )
        << BUILD_LOG( kPacket_.m_strMsg ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_SHA_ENABLE_REQ )
{
    bool bSHACheckEnable = false;
    if ( SiKGSSimLayer()->IsSHACheckEnable() ) {
        bSHACheckEnable = true;
    }
    START_LOG( clog, L"SHA Check Enable Value 전달.. Login : " << GetName() << L", SHA Enable : " << bSHACheckEnable ) << END_LOG;
    int kPacket = (bSHACheckEnable? 1 : 0 );
    SEND_PACKET( EVENT_SHA_ENABLE_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_GWC_RANKING_LIST_REQ )
{
    KEVENT_GWC_RANKING_LIST_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    // GWC 이벤트 기간인가.
    if ( !SiKGWCEvent()->IsRun() ) {
        SET_ERR_GOTO( ERR_GWCEVENT_00, END_PROC );
    }

    SiKGWCEvent()->GetTotalCountryInfo( kPacket.m_mapTotalCountryInfo );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_GWCEVENT_00,      kPacket.m_nOK, 1 );     // GWC 이벤트 기간이 아님.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"GWC 랭킹 리스트 요청중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L"GWC 랭킹 리스트 요청중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_mapTotalCountryInfo.size() ) << END_LOG;

    SEND_PACKET( EVENT_GWC_RANKING_LIST_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_GWC_RANKING_REWARD_REQ )
{
    KEVENT_GWC_RANKING_REWARD_ACK kPacket;
    VECTOR_STRUCT_DROPITEM kDBPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    // GWC 이벤트 기간인가.
    if ( !SiKGWCEvent()->IsRun() ) {
        SET_ERR_GOTO( ERR_GWCEVENT_00, END_PROC );
    }

    // 보상 아이템 정보 가져오기.
    if( !SiKGWCEvent()->GetRankingReward( kDBPacket ) ) {
        SET_ERR_GOTO( ERR_GWCEVENT_01, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_GWC_RANKING_REWARD_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_GWCEVENT_00,      kPacket.m_nOK, 1 );     // GWC 이벤트 기간이 아님.
        ERR_CASE( ERR_GWCEVENT_01,      kPacket.m_nOK, 2 );     // GWC 보상 아이템 가져오기 실패.
        ERR_CASE( ERR_GWCEVENT_02,      kPacket.m_nOK, 3 );     // GWC 보상 아이템 이미 받아갔음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"GWC 랭킹 보상 요청중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"GWC 랭킹 보상 요청중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecReward.size() ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_GWC_RANKING_REWARD_ACK );
    }
}

IMPL_ON_FUNC( EVENT_GWC_RANKING_REWARD_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 유저 인벤토리 갱신.
    m_kInventory.AddItemList( kPacket_.m_vecReward );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_GWCEVENT_00,      kPacket_.m_nOK, 1 );     // GWC 이벤트 기간이 아님.
        ERR_CASE( ERR_GWCEVENT_01,      kPacket_.m_nOK, 2 );     // GWC 보상 아이템 가져오기 실패.
        ERR_CASE( ERR_GWCEVENT_02,      kPacket_.m_nOK, 3 );     // GWC 보상 아이템 이미 받아갔음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"GWC 랭킹 보상 요청중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"GWC 랭킹 보상 요청 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecReward.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_GWC_RANKING_REWARD_ACK );
}

IMPL_ON_FUNC( EVENT_JUST_IN_TIME_REWARD_REQ )
{
    KEVENT_JUST_IN_TIME_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_tmNextEvent = 0;

    //USHORT usUserType = ( IsPCBangUser() ? KJustInTime::EU_PCCAFE_USER : KJustInTime::EU_EX_PCCAFE_USER );

    KDB_EVENT_JUST_IN_TIME_REWARD_REQ kDBPacket;
    kDBPacket.m_vecItemList.clear();
    kDBPacket.m_tmJIT = kPacket_;
    kDBPacket.m_mapUserTypeEvent.clear();

    SET_ERROR( ERR_UNKNOWN );

    for ( USHORT i = KJustInTime::EU_ALL_USER ; i < KJustInTime::EU_MAX ; ++i ) {
        const USHORT& usUserType = i;
        bool bIsEventUser = false;

        // 대상 유저인가?
        switch (usUserType) {
            case KJustInTime::EU_ALL_USER:
                bIsEventUser = true;
                break;
            case KJustInTime::EU_PCCAFE_USER:
                if ( IsPCBangUser() ) {
                    bIsEventUser = true;
                }
                break;
            case KJustInTime::EU_EX_PCCAFE_USER:
                if ( false == IsPCBangUser() ) {
                    bIsEventUser = true;
                }
                break;
            default:
                START_LOG( cwarn, L"정의되지않은 JIT 대상유저 타입 : " << usUserType )
                    << END_LOG;
                continue;
        }

        if ( false == bIsEventUser ) {
            START_LOG( clog, L"JIT 이벤트 대상 유저가 아님." )
                << BUILD_LOG( usUserType )
                << BUILD_LOG( bIsEventUser )
                << BUILD_LOG( IsPCBangUser() )
                << END_LOG;
            continue;
        }

        std::vector<KDropItemInfo> vecReward;
        vecReward.clear();

        // 유저타입별 보상 아이템 가져오기
        bool bExistReward = SiKJustInTime()->GetRewardList( usUserType, kPacket_, vecReward );
        if ( false == bExistReward ) {
            START_LOG( clog, L"JIT 시간대에 해당하는 보상 없음." )
                << BUILD_LOG( usUserType )
                << BUILD_LOGtm( CTime(kPacket_) )
                << END_LOG;
            continue;
        }

        if ( vecReward.empty() ) {
            START_LOG( cwarn, L"JIT 보상은 설정되어있는데, 보상아이템은 비어있음." )
                << BUILD_LOG( usUserType )
                << BUILD_LOGtm( CTime(kPacket_) )
                << END_LOG;
            continue;
        }

        // 보상을 이미 받아갔는지 확인
        bool bIsReceived = SiKJustInTime()->IsReceivedReward( usUserType, std::make_pair( GetUID(), kPacket_ ) );
        if ( bIsReceived ) {
            START_LOG( cwarn, L"JIT 시간대 보상 이미 받아감." )
                << BUILD_LOG( usUserType )
                << BUILD_LOGtm( CTime(kPacket_) )
                << END_LOG;
            continue;
        }

        // 받아갈 수 있는 보상 설정
        kDBPacket.m_vecItemList.insert( kDBPacket.m_vecItemList.end(), vecReward.begin(), vecReward.end() );
        kDBPacket.m_mapUserTypeEvent[usUserType] = bIsEventUser;
    }

    /*
    QUEUEING_EVENT_TO_DB_USING_DISPATCHER_CHECK_IN() 매크로를 사용하게 되면
    이벤트가 event divide dispatcher에서 db thread로 넘겨질 때까지 동일 유저에게 동일 패킷을 처리하지 않는다
    */
    QUEUEING_EVENT_TO_DB_USING_DISPATCHER_CHECK_IN( DB_EVENT_JUST_IN_TIME_REWARD_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_JIT_00,           kPacket.m_nOK, 1 );     // JIT 보상 가져오기 실패
        ERR_CASE( ERR_JIT_01,           kPacket.m_nOK, 2 );     // JIT 보상 아이템을 이미 받아갔음
        ERR_CASE( ERR_JIT_02,           kPacket.m_nOK, 3 );     // JIT 이벤트 대상 유저가 아님
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"JIT 보상 요청중 알 수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"JIT 보상 요청중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOGtm( CTime(kPacket_) )
        << BUILD_LOG( IsPCBangUser() )
        << BUILD_LOG( kDBPacket.m_mapUserTypeEvent.size() )
        << BUILD_LOG( kDBPacket.m_vecItemList.size() )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_JUST_IN_TIME_REWARD_ACK );
    }
}

IMPL_ON_FUNC( EVENT_JUST_IN_TIME_REWARD_ACK )
{
    // 다음 JIT 이벤트 확인
    CheckJustInTimeEvent( kPacket_.m_tmNextEvent, kPacket_.m_mapUserTypeEvent );

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 유저 인벤토리 갱신.
    m_kInventory.AddItemList( kPacket_.m_vecRecieveItemList );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_JIT_00,           kPacket_.m_nOK, 1 );     // JIT 보상 가져오기 실패
        ERR_CASE( ERR_JIT_01,           kPacket_.m_nOK, 2 );     // JIT 보상 아이템을 이미 받아갔음
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"JIT 보상 요청중 알 수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"JIT 보상 요청 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_tmNextEvent )
        << BUILD_LOG( kPacket_.m_vecRecieveItemList.size() )
        << BUILD_LOG( kPacket_.m_mapUserTypeEvent.size() )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_JUST_IN_TIME_REWARD_ACK );
}

IMPL_ON_FUNC( EVENT_JUST_IN_TIME_ON_TIME_NOT )
{
    kPacket_.m_bIsEventUser = false;

    switch ( kPacket_.m_usUserType ) {
        case KJustInTime::EU_ALL_USER:
            kPacket_.m_bIsEventUser = true;
            break;
        case KJustInTime::EU_PCCAFE_USER:
            if ( IsPCBangUser() ) {
                kPacket_.m_bIsEventUser = true;
            }
            break;
        case KJustInTime::EU_EX_PCCAFE_USER:
            if ( false == IsPCBangUser() ) {
                kPacket_.m_bIsEventUser = true;
            }
            break;
        default:
            START_LOG( cwarn, L"정의되지않은 JIT UserType : " << kPacket_.m_usUserType )
                << END_LOG;
            return;
    }

    START_LOG( clog, L"JustInTime Notice.. Login : " << GetName() )
        << BUILD_LOGtm( CTime(kPacket_.m_tmJIT) )
        << BUILD_LOG( kPacket_.m_vecItemList.size() )
        << BUILD_LOG( kPacket_.m_usUserType )
        << BUILD_LOG( kPacket_.m_bIsEventUser )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_JUST_IN_TIME_ON_TIME_NOT );
}

IMPL_ON_FUNC_NOPARAM( EVENT_CHARISMAS_EVENT_INFO_REQ )
{
    KEVENT_CHARISMAS_EVENT_INFO_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    // 크리스 마스 이벤트 기간인가
    if ( !SiKCharismasPresent()->IsRun() ) {
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_01, END_PROC );
    }

    if ( m_kCharismasUserData.IsLoaded() ) { // 이미 DB에서 데이터를 가져왔으면..,
        m_kCharismasUserData.GetUserCharismasEventData( kPacket.m_dwGrade, kPacket.m_dwBonusPoint );
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_02, END_PROC );
    }

    SET_ERROR( NET_OK );
    QUEUEING_ID_TO_DB_CHECK( EVENT_CHARISMAS_EVENT_INFO_REQ );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                       kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_CHARISMASPRESENT_01,      kPacket.m_nOK, 1 );     // 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_CHARISMASPRESENT_02,      kPacket.m_nOK, 2 );     // DB에서 데이터 이미 가져왔음.
        ERR_CASE( ERR_NOT_FINISHED_YET,         kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"크리스마스 이벤트 유저 정보 요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 || kPacket.m_nOK == 2, L"크리스마스 이벤트 정보 요청 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_CHARISMAS_EVENT_INFO_ACK );
    }
}

IMPL_ON_FUNC( EVENT_CHARISMAS_EVENT_INFO_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    DWORD dwMaxPoint = SiKCharismasPresent()->GetMaxPoint();
    m_kCharismasUserData.SetUserCharismasEventData( kPacket_.m_dwGrade, kPacket_.m_dwBonusPoint, dwMaxPoint, false );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                       kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_CHARISMASPRESENT_01,      kPacket_.m_nOK, 1 );     // 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_CHARISMASPRESENT_02,      kPacket_.m_nOK, 2 );     // DB에서 데이터 이미 가져왔음.
        ERR_CASE( ERR_NOT_FINISHED_YET,         kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"크리스마스 이벤트 응답 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"크리스마스 이벤트 유저 데이터 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGrade )
        << BUILD_LOG( kPacket_.m_dwBonusPoint )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CHARISMAS_EVENT_INFO_ACK );
}

_IMPL_ON_FUNC( EVENT_CHARISMAS_EVENT_CONNECT_CHECK_REQ, std::wstring )
{
    KEVENT_CHARISMAS_EVENT_CONNECT_CHECK_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    // 크리스 마스 이벤트 기간인가
    if ( !SiKCharismasPresent()->IsRun() ) {
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_01, END_PROC );
    }

    SET_ERROR( NET_OK );
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_CHARISMAS_EVENT_CONNECT_CHECK_REQ, kPacket_ );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                       kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_CHARISMASPRESENT_01,      kPacket.m_nOK, 1 );     // 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_CHARISMASPRESENT_03,      kPacket.m_nOK, 3 );     // DB에서 유저 닉네임으로 UID 가져오기 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,         kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"크리스마스 이벤트 유저 접속 확인 요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"크리스마스 이벤트 유저 접속 확인요청 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_CHARISMAS_EVENT_CONNECT_CHECK_ACK );
    }
}

IMPL_ON_FUNC( EVENT_CHARISMAS_EVENT_CONNECT_CHECK_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                       kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_CHARISMASPRESENT_01,      kPacket_.m_nOK, 1 );     // 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_CHARISMASPRESENT_03,      kPacket_.m_nOK, 3 );     // DB에서 유저 닉네임으로 UID 가져오기 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,         kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"크리스마스 이벤트 유저 접속 확인 응답중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"크리스마스 이벤트 유저 접속 확인 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CHARISMAS_EVENT_CONNECT_CHECK_ACK );
}

IMPL_ON_FUNC( EVENT_CHARISMAS_EVENT_PRESENT_REQ )
{
    KEVENT_CHARISMAS_EVENT_PRESENT_ACK kPacket;
    KDB_EVENT_CHARISMAS_EVENT_PRESENT_REQ kDBPacket;
    kPacket.m_nOK = -99;
    KItemPtr pkSledgeItem; // 썰매 아이템( 크리스마스 선물 재료 )
    SET_ERROR( ERR_UNKNOWN );

    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwReceiveUserUID );

    if( kPacket_.m_dwReceiveUserUID == GetUID() ) {
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_07, END_PROC );
    }

    if ( false == SiKPostConfig()->IsPostEnable() ) {
        if( !spUser ) {
            SET_ERR_GOTO( ERR_CHARISMASPRESENT_07, END_PROC );
        }
    }

    // 크리스 마스 이벤트 기간인가
    if ( !SiKCharismasPresent()->IsRun() ) {
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_01, END_PROC );
    }

    pkSledgeItem = m_kInventory.FindDurationItemByItemID( SiKCharismasPresent()->GetSledgeItem() );
    if( !pkSledgeItem ) {
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_04, END_PROC );
    }

    if( !SiKCharismasPresent()->GetItemConfig( kPacket_.m_kDropItemInfo, kDBPacket.m_dwSledgeUseCnt, kDBPacket.m_dwAddBonusPoint ) ) {
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_06, END_PROC );
    }

    if( pkSledgeItem->m_nCount < static_cast<int>( kDBPacket.m_dwSledgeUseCnt ) ) {
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_05, END_PROC );
    }

    kDBPacket.m_kDropItemInfo = kPacket_.m_kDropItemInfo;
    kDBPacket.m_dwReceiveUserUID = kPacket_.m_dwReceiveUserUID;
    kDBPacket.m_kSledgeItem = *pkSledgeItem;
    kDBPacket.m_strFromUserNickname = GetNickName();
    kDBPacket.m_cReceiveUserCharType = -1;
    m_kCharismasUserData.GetUserCharismasEventData( kDBPacket.m_dwGrade, kDBPacket.m_dwBonusPoint );

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_CHARISMAS_EVENT_PRESENT_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                       kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_CHARISMASPRESENT_01,      kPacket.m_nOK, 1 );     // 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_CHARISMASPRESENT_04,      kPacket.m_nOK, 4 );     // 크리스마스 재료 아이템 인벤토리 없음.
        ERR_CASE( ERR_CHARISMASPRESENT_05,      kPacket.m_nOK, 5 );     // 크리스마스 재료 아이템 갯수 부족.
        ERR_CASE( ERR_CHARISMASPRESENT_06,      kPacket.m_nOK, 6 );     // 선물할 아이템 정보 가져오기 실패.
        ERR_CASE( ERR_CHARISMASPRESENT_07,      kPacket.m_nOK, 7 );     // 선물받을 유저가 해당 서버에 없음.
        ERR_CASE( ERR_CHARISMASPRESENT_08,      kPacket.m_nOK, 8 );     // DB 선물 아이템 넣기 실패.
        ERR_CASE( ERR_CHARISMASPRESENT_09,      kPacket.m_nOK, 9 );     // 크리스마스 재료 아이템 동기화 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,         kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"크리스 마스 이벤트 선물 하기 요청중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"크리스 마스 이벤트 선물 하기 요청중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_kDropItemInfo.m_ItemID )
        << BUILD_LOG( kPacket_.m_dwReceiveUserUID )
        << BUILD_LOG( kPacket.m_nOK )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_CHARISMAS_EVENT_PRESENT_ACK );
    }
}

IMPL_ON_FUNC( EVENT_CHARISMAS_EVENT_PRESENT_ACK )
{
    KItemPtr pkSledgeItem; // 썰매 아이템( 크리스마스 선물 재료 )
    DWORD dwGrade = 0;
    DWORD dwMaxPoint = SiKCharismasPresent()->GetMaxPoint();
    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwReceiveUserUID );
    
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) && !IS_CORRECT( ERR_CHARISMASPRESENT_09 ) ) {
        goto END_PROC;
    }

    pkSledgeItem = m_kInventory.FindDurationItemByItemID( SiKCharismasPresent()->GetSledgeItem() );
    if( pkSledgeItem ) {
        pkSledgeItem->m_nCount = kPacket_.m_kSledgeItem.m_nCount;
        pkSledgeItem->m_nInitCount = kPacket_.m_kSledgeItem.m_nCount;
        kPacket_.m_vecUseItem.push_back( *pkSledgeItem );
    }

    m_kCharismasUserData.SetUserCharismasEventData( kPacket_.m_dwGrade, kPacket_.m_dwBonusPoint, dwMaxPoint, false );
    m_kCharismasUserData.GetUserCharismasEventData( dwGrade, kPacket_.m_dwBonusPoint );

    if ( false == SiKPostConfig()->IsPostEnable() ) {
        if( spUser ) {
            KEVENT_CHARISMAS_EVENT_PRESENT_NOT kPacket;
            kPacket.m_strNickName = GetNickName();
            kPacket.m_vecPresentItem = kPacket_.m_vecPresentItem;
            spUser->QueueingEvent( KUserEvent::EVENT_CHARISMAS_EVENT_PRESENT_NOT, kPacket );
        }
    }

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                       kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_CHARISMASPRESENT_01,      kPacket_.m_nOK, 1 );     // 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_CHARISMASPRESENT_04,      kPacket_.m_nOK, 4 );     // 크리스마스 재료 아이템 인벤토리 없음.
        ERR_CASE( ERR_CHARISMASPRESENT_05,      kPacket_.m_nOK, 5 );     // 크리스마스 재료 아이템 갯수 부족.
        ERR_CASE( ERR_CHARISMASPRESENT_06,      kPacket_.m_nOK, 6 );     // 선물할 아이템 정보 가져오기 실패.
        ERR_CASE( ERR_CHARISMASPRESENT_07,      kPacket_.m_nOK, 7 );     // 선물받을 유저가 해당 서버에 없음.
        ERR_CASE( ERR_CHARISMASPRESENT_08,      kPacket_.m_nOK, 8 );     // DB 선물 아이템 넣기 실패.
        ERR_CASE( ERR_CHARISMASPRESENT_09,      kPacket_.m_nOK, 9 );     // 크리스마스 재료 아이템 동기화 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,         kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"크리스마스 이벤트 선물하기 응답중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"크리스마스 이벤트 선물하기 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGrade )
        << BUILD_LOG( kPacket_.m_dwBonusPoint )
        << BUILD_LOG( kPacket_.m_dwReceiveUserUID )
        << BUILD_LOG( kPacket_.m_vecPresentItem.size() )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CHARISMAS_EVENT_PRESENT_ACK );
}

IMPL_ON_FUNC( EVENT_CHARISMAS_EVENT_PRESENT_NOT )
{
    m_kInventory.AddItemList( kPacket_.m_vecPresentItem );
    SEND_RECEIVED_PACKET( EVENT_CHARISMAS_EVENT_PRESENT_NOT );
}

IMPL_ON_FUNC( EVENT_CHARISMAS_EVENT_DONATION_REQ )
{
    KEVENT_CHARISMAS_EVENT_DONATION_ACK kPacket;
    kPacket.m_nOK = -99;
    KItemPtr pkSledgeItem; // 썰매 아이템( 크리스마스 선물 재료 )
    DWORD dwMaxBonusPoint = SiKCharismasPresent()->GetMaxPoint();
    SET_ERROR( ERR_UNKNOWN );

    // 크리스 마스 이벤트 기간인가
    if ( !SiKCharismasPresent()->IsRun() ) {
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_01, END_PROC );
    }

    pkSledgeItem = m_kInventory.FindDurationItemByItemID( SiKCharismasPresent()->GetSledgeItem() );
    if( !pkSledgeItem ) {
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_04, END_PROC );
    }

    if( pkSledgeItem->m_nCount < static_cast<int>( kPacket_.m_dwUseItemCnt ) ) {
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_05, END_PROC );
    }

    if( !SiKCharismasPresent()->GetDonationInfo( kPacket_.m_dwUseItemCnt, kPacket_.m_dwAddBonusPoint ) ) {
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_10, END_PROC );
    }
    
    kPacket_.m_kSledgeItem = *pkSledgeItem;
    m_kCharismasUserData.GetUserCharismasEventData( kPacket_.m_dwGrade, kPacket_.m_dwBonusPoint );

    // 현재 보너스 포인트가 최대일때는 더이상 기부하지 못하게 처리.
    if( kPacket_.m_dwBonusPoint >= dwMaxBonusPoint ) {
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_14, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_CHARISMAS_EVENT_DONATION_REQ, kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                       kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_CHARISMASPRESENT_01,      kPacket.m_nOK, 1 );     // 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_CHARISMASPRESENT_04,      kPacket.m_nOK, 4 );     // 크리스마스 재료 아이템 인벤토리 없음.
        ERR_CASE( ERR_CHARISMASPRESENT_05,      kPacket.m_nOK, 5 );     // 크리스마스 재료 아이템 갯수 부족.
        ERR_CASE( ERR_CHARISMASPRESENT_10,      kPacket.m_nOK, 10 );    // 기부갯수 보너스 포인트 정보 가져오기 실패.
        ERR_CASE( ERR_CHARISMASPRESENT_14,      kPacket.m_nOK, 14 );    // 현재 유저의 보너스 포인트가 최대치임.
        ERR_CASE( ERR_NOT_FINISHED_YET,         kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"크리스 마스 기부하기 요청중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"크리스 마스 이벤트 기부하기 요청중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_dwUseItemCnt )
        << BUILD_LOG( kPacket_.m_dwAddBonusPoint )
        << BUILD_LOG( kPacket_.m_dwGrade )
        << BUILD_LOG( kPacket.m_nOK )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_CHARISMAS_EVENT_DONATION_ACK );
    }
}

IMPL_ON_FUNC( EVENT_CHARISMAS_EVENT_DONATION_ACK )
{
    KItemPtr pkSledgeItem; // 썰매 아이템( 크리스마스 선물 재료 )
    DWORD dwGrade = 0;
    DWORD dwMaxPoint = SiKCharismasPresent()->GetMaxPoint();
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) && !IS_CORRECT( ERR_CHARISMASPRESENT_09 ) ) {
        goto END_PROC;
    }

    pkSledgeItem = m_kInventory.FindDurationItemByItemID( SiKCharismasPresent()->GetSledgeItem() );
    if( pkSledgeItem ) {
        pkSledgeItem->m_nCount = kPacket_.m_kSledgeItem.m_nCount;
        pkSledgeItem->m_nInitCount = kPacket_.m_kSledgeItem.m_nCount;
        kPacket_.m_vecUseItem.push_back( *pkSledgeItem );
    }

    m_kCharismasUserData.SetUserCharismasEventData( kPacket_.m_dwGrade, kPacket_.m_dwBonusPoint, dwMaxPoint, false );
    m_kCharismasUserData.GetUserCharismasEventData( dwGrade, kPacket_.m_dwBonusPoint );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                       kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_CHARISMASPRESENT_01,      kPacket_.m_nOK, 1 );     // 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_CHARISMASPRESENT_04,      kPacket_.m_nOK, 4 );     // 크리스마스 재료 아이템 인벤토리 없음.
        ERR_CASE( ERR_CHARISMASPRESENT_05,      kPacket_.m_nOK, 5 );     // 크리스마스 재료 아이템 갯수 부족.
        ERR_CASE( ERR_CHARISMASPRESENT_10,      kPacket_.m_nOK, 10 );    // 기부갯수 보너스 포인트 정보 가져오기 실패.
        ERR_CASE( ERR_CHARISMASPRESENT_14,      kPacket_.m_nOK, 14 );    // 현재 유저의 보너스 포인트가 최대치임.
        ERR_CASE( ERR_NOT_FINISHED_YET,         kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"크리스마스 이벤트 기부하기 응답중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"크리스마스 이벤트 기부하기 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_dwGrade )
        << BUILD_LOG( kPacket_.m_dwBonusPoint )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CHARISMAS_EVENT_DONATION_ACK );
}

IMPL_ON_FUNC( EVENT_CHARISMAS_EVENT_GRADE_REWARD_REQ )
{
    KEVENT_CHARISMAS_EVENT_GRADE_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;
    DWORD dwCurrentGrade = 0;
    DWORD dwMaxGrade = 0;
    DWORD dwTotalMaxGrade = SiKCharismasPresent()->GetMaxGrade();
    SET_ERROR( ERR_UNKNOWN );

    // 크리스 마스 이벤트 기간인가
    if ( !SiKCharismasPresent()->IsRun() ) {
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_01, END_PROC );
    }

    m_kCharismasUserData.GetUserCharismasEventData( dwCurrentGrade, kPacket_.m_dwBonusPoint );
    if( !SiKCharismasPresent()->GetPointGrade( kPacket_.m_dwBonusPoint, dwMaxGrade ) ) {
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_13, END_PROC );
    }

    if( !m_kCharismasUserData.CheckUserGrade( kPacket_.m_dwGrade ) ) {
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_12, END_PROC );
    }

    if( dwTotalMaxGrade != kPacket_.m_dwGrade && dwMaxGrade <= kPacket_.m_dwGrade ) {
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_12, END_PROC );
    }

    if( !SiKCharismasPresent()->GetGradeRewardList( kPacket_.m_dwGrade, kPacket_.m_vecReward ) ) {
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_11, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_CHARISMAS_EVENT_GRADE_REWARD_REQ, kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                       kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_CHARISMASPRESENT_01,      kPacket.m_nOK, 1 );     // 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_CHARISMASPRESENT_08,      kPacket.m_nOK, 8 );     // DB 선물 아이템 넣기 실패.
        ERR_CASE( ERR_CHARISMASPRESENT_11,      kPacket.m_nOK, 11 );    // 등급 보상 가져오기 실패.
        ERR_CASE( ERR_CHARISMASPRESENT_12,      kPacket.m_nOK, 12 );    // 잘못된 등급정보.
        ERR_CASE( ERR_CHARISMASPRESENT_13,      kPacket.m_nOK, 13 );    // 유저의 보너스 포인트가 설정되지 않음.
        ERR_CASE( ERR_NOT_FINISHED_YET,         kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"크리스 마스 등급보상 받기 요청중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"크리스 마스 이벤트 등급 보상 받기 요청중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_dwGrade )
        << BUILD_LOG( kPacket_.m_vecReward.size() )
        << BUILD_LOG( kPacket.m_nOK )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_CHARISMAS_EVENT_GRADE_REWARD_ACK );
    }
}

IMPL_ON_FUNC( EVENT_CHARISMAS_EVENT_GRADE_REWARD_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    DWORD dwMaxPoint = SiKCharismasPresent()->GetMaxPoint();
    m_kCharismasUserData.SetUserCharismasEventData( kPacket_.m_dwGrade, kPacket_.m_dwBonusPoint, dwMaxPoint, kPacket_.bInitEnable );

    m_kInventory.AddItemList( kPacket_.m_vecReward );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                       kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_CHARISMASPRESENT_01,      kPacket_.m_nOK, 1 );     // 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_CHARISMASPRESENT_08,      kPacket_.m_nOK, 8 );     // DB 선물 아이템 넣기 실패.
        ERR_CASE( ERR_CHARISMASPRESENT_11,      kPacket_.m_nOK, 11 );    // 등급 보상 가져오기 실패.
        ERR_CASE( ERR_CHARISMASPRESENT_12,      kPacket_.m_nOK, 12 );    // 잘못된 등급정보.
        ERR_CASE( ERR_CHARISMASPRESENT_13,      kPacket_.m_nOK, 13 );    // 유저의 보너스 포인트가 설정되지 않음.
        ERR_CASE( ERR_NOT_FINISHED_YET,         kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"크리스마스 이벤트 등급 보상 응답중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"크리스마스 이벤트 등급보상 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGrade )
        << BUILD_LOG( kPacket_.m_dwBonusPoint )
        << BUILD_LOG( kPacket_.m_vecReward.size() )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CHARISMAS_EVENT_GRADE_REWARD_ACK );
}


IMPL_ON_FUNC( EVENT_STAT_HOTKEY_USED_INFO_NOT )
{
    START_LOG( cerr, L"Hotkey info packet received, name: " << m_strName ) << END_LOG;
    {
        START_LOG( clog, L"m_mapDungeonUsedCharge" ) << END_LOG;
        std::map< char, std::map< char, int > >::iterator mitCharType;
        for ( mitCharType = kPacket_.m_mapDungeonUsedCharge.begin(); mitCharType != kPacket_.m_mapDungeonUsedCharge.end(); ++mitCharType ) {
            std::map< char, int >::iterator mitCharge;
            for ( mitCharge = mitCharType->second.begin(); mitCharge != mitCharType->second.end(); ++mitCharge ) {
                START_LOG( clog, L"CharType: " << static_cast< int >( mitCharType->first )
                    << ", SkillLevel: " << static_cast< int >( mitCharge->first )
                    << ", UsedCount: " << mitCharge->second ) << END_LOG;
            }
        }
    }
    {
        START_LOG( clog, L"m_mapDungeonUsedHotkey" ) << END_LOG;
        std::map< char, std::map< char, int > >::iterator mitCharType;
        for ( mitCharType = kPacket_.m_mapDungeonUsedHotkey.begin(); mitCharType != kPacket_.m_mapDungeonUsedHotkey.end(); ++mitCharType ) {
            std::map< char, int >::iterator mitCharge;
            for ( mitCharge = mitCharType->second.begin(); mitCharge != mitCharType->second.end(); ++mitCharge ) {
                START_LOG( clog, L"CharType: " << static_cast< int >( mitCharType->first )
                    << ", SkillLevel: " << static_cast< int >( mitCharge->first )
                    << ", UsedCount: " << mitCharge->second ) << END_LOG;
            }
        }
    }
    {
        START_LOG( clog, L"m_mapPVPUsedCharge" ) << END_LOG;
        std::map< char, std::map< char, int > >::iterator mitCharType;
        for ( mitCharType = kPacket_.m_mapPVPUsedCharge.begin(); mitCharType != kPacket_.m_mapPVPUsedCharge.end(); ++mitCharType ) {
            std::map< char, int >::iterator mitCharge;
            for ( mitCharge = mitCharType->second.begin(); mitCharge != mitCharType->second.end(); ++mitCharge ) {
                START_LOG( clog, L"CharType: " << static_cast< int >( mitCharType->first )
                    << ", SkillLevel: " << static_cast< int >( mitCharge->first )
                    << ", UsedCount: " << mitCharge->second ) << END_LOG;
            }
        }
    }
    {
        START_LOG( clog, L"m_mapPVPUsedHotkey" ) << END_LOG;
        std::map< char, std::map< char, int > >::iterator mitCharType;
        for ( mitCharType = kPacket_.m_mapPVPUsedHotkey.begin(); mitCharType != kPacket_.m_mapPVPUsedHotkey.end(); ++mitCharType ) {
            std::map< char, int >::iterator mitCharge;
            for ( mitCharge = mitCharType->second.begin(); mitCharge != mitCharType->second.end(); ++mitCharge ) {
                START_LOG( clog, L"CharType: " << static_cast< int >( mitCharType->first )
                    << ", SkillLevel: " << static_cast< int >( mitCharge->first )
                    << ", UsedCount: " << mitCharge->second ) << END_LOG;
            }
        }
    }

    QUEUEING_EVENT_TO_DB( EVENT_STAT_HOTKEY_USED_INFO_NOT, kPacket_ );
}

IMPL_ON_FUNC( DB_EVENT_INIT_POST_LETTER_LIST_ACK )
{
    CHECK_POST_ENABLE;
    KEVENT_NEW_POST_LETTER_INFO_NOT kPacket;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    // 우편함 초기화
    m_kUserLetterBox.Clear();
    m_kUserLetterBox.Init( kPacket_.m_vecPostItem, kPacket.m_nNewUserLetterCount, kPacket.m_nNewSystemLetterCount, kPacket.m_vecNewLetterTitle );
    m_kUserLetterBox.InitBlackList( kPacket_.m_setBlackList );
    kPacket.m_nTotalLetterCount = m_kUserLetterBox.GetTotalLetterCount( KEVENT_GET_POST_LETTER_LIST_REQ::LL_ALL );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,            kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_POSTOFFICE_02, kPacket_.m_nOK, 2 ); // 유저 우편정보 DB받아오기 실패.
        ERR_CASE( ERR_POSTOFFICE_03, kPacket_.m_nOK, 3 ); // 유저 우편 첨부아이템 정보 DB받아오기 실패.
        ERR_CASE( ERR_POSTOFFICE_04, kPacket_.m_nOK, 4 ); // 유저 우편 대기아이템 세부정보 DB받아오기 실패.
        ERR_CASE( ERR_POSTOFFICE_11, kPacket_.m_nOK, 11 ); // 첨부아이템 세부정보를 DB에서 찾을 수 없음.
        default:
            START_LOG( cerr, L"우편함 초기화 중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( GetName() ) << END_LOG;
            kPacket_.m_nOK = -99;
    }

    // 클라이언트로 읽지않은 편지수를 보내준다.
    if ( kPacket_.m_nOK == 0 ) {
        SEND_PACKET( EVENT_NEW_POST_LETTER_INFO_NOT );
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"우편함 초기화. Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( GetName() )
        << BUILD_LOG(  kPacket_.m_vecPostItem.size() )
        << BUILD_LOG(  kPacket_.m_setBlackList.size() )
        << BUILD_LOG(  kPacket.m_nNewUserLetterCount )
        << BUILD_LOG(  kPacket.m_nNewSystemLetterCount )
        << BUILD_LOG(  kPacket.m_nTotalLetterCount )
        << BUILD_LOG(  kPacket.m_vecNewLetterTitle.size() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_GET_POST_LETTER_LIST_REQ )
{
    CHECK_POST_ENABLE;

    KEVENT_GET_POST_LETTER_LIST_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nPostPageNo = kPacket_.m_nPostPageNo;
    kPacket.m_nTotalLetterCount = 0;
    kPacket.m_vecPostItemTitle.clear();

    SET_ERROR( ERR_UNKNOWN );

    if ( false == m_kUserLetterBox.GetLetterList( kPacket_.m_cLetterListType, kPacket_.m_nPostPageNo, kPacket.m_vecPostItemTitle ) ) {
        SET_ERR_GOTO( ERR_POSTOFFICE_01, END_PROC );
    }
    kPacket.m_nTotalLetterCount = m_kUserLetterBox.GetTotalLetterCount( kPacket_.m_cLetterListType );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,            kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_POSTOFFICE_01, kPacket.m_nOK, 1 ); // 요청한 우편함 페이지 리스트 번호가 잘못됨.
        default:
            START_LOG( cerr, L"우편함 페이지 정보 요청 중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( GetName() ) << END_LOG;
            kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"우편함 페이지 정보 요청. Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( GetName() )
        << BUILD_LOGc( kPacket_.m_cLetterListType )
        << BUILD_LOG( kPacket_.m_nPostPageNo )
        << BUILD_LOG( kPacket.m_nTotalLetterCount )
        << BUILD_LOG( kPacket.m_vecPostItemTitle.size() ) << END_LOG;

    SEND_PACKET( EVENT_GET_POST_LETTER_LIST_ACK );
}

IMPL_ON_FUNC( EVENT_SEND_LETTER_REQ )
{
    CHECK_POST_ENABLE;

    if ( false == SiKPostConfig()->IsUserSendPostEnable() ) {
        return;
    }

    kPacket_.m_nPostGP = std::max<int>( kPacket_.m_nPostGP, 0 );

    KDB_EVENT_SEND_LETTER_REQ kDBPacket;
    kDBPacket.m_strToNickName = kPacket_.m_strToNickName;
    kDBPacket.m_strTitle = kPacket_.m_strTitle;
    kDBPacket.m_strMessage = kPacket_.m_strMessage;
    kDBPacket.m_nPostGP = kPacket_.m_nPostGP;
    kDBPacket.m_nGamePoint = GetGP();
    kDBPacket.m_vecItem.clear();
    kDBPacket.m_vecCountItem.clear();
    kDBPacket.m_cCharType = kPacket_.m_cCharType; // 받는 유저 캐릭터 타입 추가.

    KEVENT_SEND_LETTER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nPostGP = kPacket_.m_nPostGP;
    kPacket.m_nGamePoint = GetGP();
    kPacket.m_nSendLetterCost = 0; // 수수료는 서버에서 계산
    kPacket.m_vecDecreaseCountItem.clear();
    kPacket.m_vecEraseItem.clear();
    kPacket.m_PostUID = 0;

    std::wstring strTitleCopy = boost::to_lower_copy( kPacket_.m_strTitle );
    std::wstring strMsgCopy = boost::to_lower_copy( kPacket_.m_strMessage );
    USHORT usTitleLength = SiKPostConfig()->GetTitleLength();
    USHORT usMsgLength = SiKPostConfig()->GetMessageLength();

    // 블럭유저인가?
    if ( m_cAuthLevel < 0 ) {
        SET_ERR_GOTO( ERR_POSTOFFICE_05, END_PROC );
    }

    // 자신에게는 편지를 보내는가?
    if ( kPacket_.m_strToNickName == GetNickName() ) {
        SET_ERR_GOTO( ERR_POSTOFFICE_06, END_PROC );
    }

    // 글자 길이 검사
    if ( strTitleCopy.size() > usTitleLength || strMsgCopy.size() > usMsgLength ) {
        START_LOG( cwarn, L"우편 내용 길이가 제한길이를 초과함.. Login : " << GetName() )
            << BUILD_LOG( strTitleCopy.size() )
            << BUILD_LOG( usTitleLength )
            << BUILD_LOG( strMsgCopy.size() )
            << BUILD_LOG( usMsgLength ) << END_LOG;
        SET_ERR_GOTO( ERR_POSTOFFICE_07, END_PROC );
    }

    // 금지어 체크
    if( SiKSlangFilter()->HasSlang( strTitleCopy ) || SiKSlangFilter()->HasSlang( strMsgCopy ) ) {
        START_LOG( cwarn, L"우편 내용에 금지어가 포함되어 있음.. Login : " << GetName() )
            << BUILD_LOG( strTitleCopy )
            << BUILD_LOG( strMsgCopy ) << END_LOG;
        SET_ERR_GOTO( ERR_POSTOFFICE_08, END_PROC );
    }

    // 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
    boost::replace_all( kDBPacket.m_strToNickName, L"'", L"" );
    boost::replace_all( kDBPacket.m_strTitle, L"'", L"" );
    boost::replace_all( kDBPacket.m_strMessage, L"'", L"" );

    // GP첨부 체크
    if ( kPacket_.m_nPostGP > 0 ) {

        if ( false == SiKPostConfig()->IsUserPostGPEnable() ) {
            START_LOG( cerr, L"유저우편 GP 첨부기능이 활성화 되어있지 않은데 아이템을 첨부함.. LoginID : " << GetName() )
                << BUILD_LOG( kPacket_.m_nPostGP ) << END_LOG;
            SET_ERR_GOTO( ERR_POSTOFFICE_32, END_PROC );
        }

        if ( kPacket_.m_nPostGP > GetGP() ) {
            START_LOG( cwarn, L"우편 첨부할 GP보유량 부족함.. Login : " << GetName() )
                << BUILD_LOG( kPacket_.m_nPostGP )
                << BUILD_LOG( GetGP() ) << END_LOG;
            SET_ERR_GOTO( ERR_POSTOFFICE_09, END_PROC );
        }
    }

    if ( false == SiKPostConfig()->IsUserPostItemEnable() ) {
        if ( false == kPacket_.m_vecItemUID.empty() || false == kPacket_.m_mapCountItemUID.empty() ) {
            START_LOG( cerr, L"유저우편 아이템 첨부기능이 활성화 되어있지 않은데 아이템을 첨부함.. LoginID : " << GetName() )
                << BUILD_LOG( kPacket_.m_vecItemUID.size() )
                << BUILD_LOG( kPacket_.m_mapCountItemUID.size() ) << END_LOG;
            SET_ERR_GOTO( ERR_POSTOFFICE_29, END_PROC );
        }
    }

    // 영구아이템 첨부 체크
    if ( false == kPacket_.m_vecItemUID.empty() ) {
        std::vector<GCITEMUID>::iterator vit;
        for ( vit = kPacket_.m_vecItemUID.begin() ; vit != kPacket_.m_vecItemUID.end() ; ++vit ) {
            const GCITEMUID& PostItemUID = *vit;
            KItemPtr pkItem = m_kInventory.FindItemByItemUID( PostItemUID );
            if ( pkItem == NULL ) {
                START_LOG( cwarn, L"우편 첨부할 영구 아이템이 인벤토리에 없음.. Login : " << GetName() )
                    << BUILD_LOG( PostItemUID )
                    << BUILD_LOG( kPacket_.m_vecItemUID.size() ) << END_LOG;
                SET_ERR_GOTO( ERR_POSTOFFICE_10, END_PROC );
            }

            if ( pkItem->m_nCount != -1 || pkItem->m_nPeriod != -1 ) {
                START_LOG( cwarn, L"우편 첨부할 영구 아이템이 영구 설정이 아님.. Login : " << GetName() )
                    << BUILD_LOG( PostItemUID )
                    << BUILD_LOG( kPacket_.m_vecItemUID.size() )
                    << BUILD_LOG( pkItem->m_ItemID )
                    << BUILD_LOG( pkItem->m_nCount )
                    << BUILD_LOG( pkItem->m_nPeriod ) << END_LOG;
                SET_ERR_GOTO( ERR_POSTOFFICE_12, END_PROC );
            }

            if ( PostItemUID <= SiKIPAdvantage()->GetMaximumPCBangItemUID() ) {
                START_LOG( cwarn, L"PC방 아이템은 우편 첨부할 수 없음.. Login : " << GetName() )
                    << BUILD_LOG( PostItemUID )
                    << BUILD_LOG( pkItem->m_ItemID )
                    << BUILD_LOG( kPacket_.m_vecItemUID.size() ) << END_LOG;
                SET_ERR_GOTO( ERR_POSTOFFICE_16, END_PROC );
            }

            if ( SiKPostConfig()->IsNotPostItemID( pkItem->m_ItemID ) ) {
                START_LOG( cwarn, L"우편 첨부할 수 없는 아이템.. Login : " << GetName() )
                    << BUILD_LOG( PostItemUID )
                    << BUILD_LOG( pkItem->m_ItemID ) << END_LOG;
                SET_ERR_GOTO( ERR_POSTOFFICE_33, END_PROC );
            }

            kDBPacket.m_vecItem.push_back( *pkItem );
        }
    }

    // 수량아이템 첨부 체크
    if ( false == kPacket_.m_mapCountItemUID.empty() ) {
        std::map<GCITEMUID,int>::iterator mit;
        for ( mit = kPacket_.m_mapCountItemUID.begin() ; mit != kPacket_.m_mapCountItemUID.end() ; ++mit ) {
            const GCITEMUID& PostItemUID = mit->first;
            const int& nItemCount = mit->second;

            KItemPtr pkItem = m_kInventory.FindItemByItemUID( PostItemUID );
            if ( pkItem == NULL ) {
                START_LOG( cwarn, L"우편 첨부할 수량 아이템이 인벤토리에 없음.. Login : " << GetName() )
                    << BUILD_LOG( PostItemUID )
                    << BUILD_LOG( nItemCount )
                    << BUILD_LOG( kPacket_.m_vecItemUID.size() ) << END_LOG;
                SET_ERR_GOTO( ERR_POSTOFFICE_13, END_PROC );
            }

            if ( pkItem->m_nPeriod != -1 ) {
                START_LOG( cwarn, L"우편 첨부할 수량 아이템이 영구 설정이 아님.. Login : " << GetName() )
                    << BUILD_LOG( PostItemUID )
                    << BUILD_LOG( nItemCount )
                    << BUILD_LOG( kPacket_.m_vecItemUID.size() )
                    << BUILD_LOG( pkItem->m_ItemID )
                    << BUILD_LOG( pkItem->m_nPeriod ) << END_LOG;
                SET_ERR_GOTO( ERR_POSTOFFICE_14, END_PROC );
            }

            if ( pkItem->m_nCount < nItemCount ) {
                START_LOG( cwarn, L"우편 첨부할 수량 아이템의 인벤토리 보유량이 부족함.. Login : " << GetName() )
                    << BUILD_LOG( PostItemUID )
                    << BUILD_LOG( kPacket_.m_vecItemUID.size() )
                    << BUILD_LOG( pkItem->m_ItemID )
                    << BUILD_LOG( pkItem->m_nCount )
                    << BUILD_LOG( nItemCount ) << END_LOG;
                SET_ERR_GOTO( ERR_POSTOFFICE_15, END_PROC );
            }

            if ( PostItemUID <= SiKIPAdvantage()->GetMaximumPCBangItemUID() ) {
                START_LOG( cwarn, L"PC방 아이템은 우편 첨부할 수 없음.. Login : " << GetName() )
                    << BUILD_LOG( PostItemUID )
                    << BUILD_LOG( pkItem->m_ItemID )
                    << BUILD_LOG( kPacket_.m_vecItemUID.size() ) << END_LOG;
                SET_ERR_GOTO( ERR_POSTOFFICE_16, END_PROC );
            }

            if ( SiKPostConfig()->IsNotPostItemID( pkItem->m_ItemID ) ) {
                START_LOG( cwarn, L"우편 첨부할 수 없는 아이템.. Login : " << GetName() )
                    << BUILD_LOG( PostItemUID )
                    << BUILD_LOG( pkItem->m_ItemID ) << END_LOG;
                SET_ERR_GOTO( ERR_POSTOFFICE_33, END_PROC );
            }

            kDBPacket.m_vecCountItem.push_back( std::make_pair( *pkItem, nItemCount ) );
        }
    }

    GetDBUpdateDataGP( kDBPacket.m_kGPData );

    // 수수료 계산은 첨부 아이템의 판매가격을 DB에서 받아서 DB스레드에서 처리함.
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_SEND_LETTER_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,            kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_POSTOFFICE_05, kPacket.m_nOK, 5 ); // 우편이용 블럭 유저.
        ERR_CASE( ERR_POSTOFFICE_06, kPacket.m_nOK, 6 ); // 자기자신에게 편지를 보낼 수 없음.
        ERR_CASE( ERR_POSTOFFICE_07, kPacket.m_nOK, 7 ); // 우편 내용 길이가 제한길이를 초과함.
        ERR_CASE( ERR_POSTOFFICE_08, kPacket.m_nOK, 8 ); // 우편 내용에 금지어가 포함되어 있음.
        ERR_CASE( ERR_POSTOFFICE_09, kPacket.m_nOK, 9 ); // 우편 첨부할 GP보유량 부족함.
        ERR_CASE( ERR_POSTOFFICE_10, kPacket.m_nOK, 10 ); // 우편 첨부할 영구 아이템이 인벤토리에 없음. 
        ERR_CASE( ERR_POSTOFFICE_12, kPacket.m_nOK, 12 ); // 우편 첨부할 영구 아이템이 영구 설정이 아님.
        ERR_CASE( ERR_POSTOFFICE_13, kPacket.m_nOK, 13 ); // 우편 첨부할 수량 아이템이 인벤토리에 없음.
        ERR_CASE( ERR_POSTOFFICE_14, kPacket.m_nOK, 14 ); // 우편 첨부할 수량 아이템이 영구 설정이 아님.
        ERR_CASE( ERR_POSTOFFICE_15, kPacket.m_nOK, 15 ); // 우편 첨부할 수량 아이템의 인벤토리 보유량이 부족함.
        ERR_CASE( ERR_POSTOFFICE_16, kPacket.m_nOK, 16 ); // PC방 아이템은 우편 첨부할 수 없음.
        ERR_CASE( ERR_POSTOFFICE_29, kPacket.m_nOK, 29 ); // 유저우편 아이템 첨부기능이 활성화 되어있지 않은데 아이템을 첨부함.
        ERR_CASE( ERR_POSTOFFICE_32, kPacket.m_nOK, 32 ); // 유저우편 GP 첨부기능이 활성화 되어있지 않은데 아이템을 첨부함.
        ERR_CASE( ERR_POSTOFFICE_33, kPacket.m_nOK, 33 ); // 유저우편 첨부할 수 없는 아이템을 첨부함.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"우편 보내기 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"우편 보내기 처리 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetGP() )
        << BUILD_LOG( kPacket_.m_strToNickName )
        << BUILD_LOG( kPacket_.m_strTitle )
        << BUILD_LOG( kPacket_.m_strMessage )
        << BUILD_LOG( kPacket_.m_vecItemUID.size() )
        << BUILD_LOG( kPacket_.m_mapCountItemUID.size() )
        << BUILD_LOG( kPacket_.m_nPostGP ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_SEND_LETTER_ACK );
    }
}

IMPL_ON_FUNC( EVENT_SEND_LETTER_ACK )
{
    CHECK_POST_ENABLE;

    std::vector<GCITEMUID>::iterator vitItemUID;
    std::vector<KItem>::iterator vitCount;
    bool bDBUpdate = true;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( false == IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    // GP 동기화
    DecreaseGP( KGpStatInfo::GDT_POST_GP, bDBUpdate, kPacket_.m_nPostGP );
    DecreaseGP( KGpStatInfo::GDT_POST_COST, bDBUpdate, kPacket_.m_nSendLetterCost );

    if ( GetGP() != kPacket_.m_nGamePoint ) {
        START_LOG( cerr, L"DB에서 계산한 값과 GP값이 틀리다.. Login : " << GetName() )
            << BUILD_LOG( GetGP() )
            << BUILD_LOG( GetInitGP() )
            << BUILD_LOG( kPacket_.m_nGamePoint )
            << BUILD_LOG( kPacket_.m_nPostGP )
            << BUILD_LOG( kPacket_.m_nSendLetterCost ) << END_LOG;

        SetGP( kPacket_.m_nGamePoint );
        SetInitGP( kPacket_.m_nGamePoint );
    }

    // 첨부한 영구아이템 삭제.
    for ( vitItemUID = kPacket_.m_vecEraseItem.begin() ; vitItemUID != kPacket_.m_vecEraseItem.end() ; ++vitItemUID ) {
        bool bResult = m_kInventory.RemoveItem( *vitItemUID );
        _LOG_SUCCESS( bResult, L"첨부한 영구아이템 삭제 결과.. Login : " << GetName() << L", Result : " << bResult << L", ItemUID : " << *vitItemUID ) << END_LOG;
    }

    // 첨부한 수량 아이템 동기화
    for ( vitCount = kPacket_.m_vecDecreaseCountItem.begin() ; vitCount != kPacket_.m_vecDecreaseCountItem.end() ; ++vitCount ) {
        KItemPtr pkItem = m_kInventory.FindDurationItemByItemUID( vitCount->m_ItemUID );
        if ( false == pkItem ) {
            START_LOG( cerr, L"첨부한 수량아이템이 인벤에 없음. Login : " << GetName() )
                << BUILD_LOG( vitCount->m_ItemID )
                << BUILD_LOG( vitCount->m_ItemUID ) << END_LOG;
            continue;
        }

        // 수량아이템 동기화
        if ( pkItem->m_nCount > 0 ) {
            pkItem->m_nCount = vitCount->m_nCount;
            pkItem->m_nInitCount = vitCount->m_nInitCount;
        }

        if ( pkItem->m_nCount <= 0 ) {
            m_kInventory.RemoveItem( vitCount->m_ItemID, vitCount->m_ItemUID );
        }

        START_LOG( clog, L"첨부한 수량아이템 동기화.. Login : " << GetName() )
            << BUILD_LOG( vitCount->m_ItemID )
            << BUILD_LOG( vitCount->m_ItemUID )
            << BUILD_LOG( vitCount->m_nCount )
            << BUILD_LOG( vitCount->m_nInitCount )
            << END_LOG;
    }

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,            kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_POSTOFFICE_17, kPacket_.m_nOK, 17 ); // 우편을 받을 대상유저를 닉네임으로 찾을 수 없음.
        ERR_CASE( ERR_POSTOFFICE_18, kPacket_.m_nOK, 18 ); // 우편 수수료가 부족함.
        ERR_CASE( ERR_POSTOFFICE_19, kPacket_.m_nOK, 19 ); // 우편 보내기 DB처리 실패.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
        default:
            START_LOG( cerr, L"우편보내기 중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( GetName() ) << END_LOG;
            kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"우편보내기 결과. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nPostGP )
        << BUILD_LOG( kPacket_.m_nGamePoint )
        << BUILD_LOG( kPacket_.m_vecEraseItem.size() )
        << BUILD_LOG( kPacket_.m_vecDecreaseCountItem.size() )
        << BUILD_LOG( kPacket_.m_nSendLetterCost )
        << BUILD_LOG( kPacket_.m_PostUID ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SEND_LETTER_ACK );
}

_IMPL_ON_FUNC( EVENT_FIND_USER_SEND_LETTER_NOT, DWORD )
{
    CHECK_POST_ENABLE;
    if ( false == SiKPostConfig()->IsNewPostAlarmEnable() ) return;
    // 유저 접속확인시, 실시간 우편 알림
    LIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_FIND_USER_SEND_LETTER_NOT, kPacket_ ) );
    START_LOG( clog, L"우편보내기 성공시, 대상유저가 접속중인지 확인 요청.. FromUser Login : " << GetName() << L", ToUser UserUID : " << kPacket_ ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_RECEIVE_LETTER_NOT, DWORD )
{
    CHECK_POST_ENABLE;
    // DB로 새 우편리스트 요청하자.
    QUEUEING_ID_TO_DB_CHECK( EVENT_NEW_POST_LETTER_INFO_REQ );
END_PROC:
    START_LOG( clog, L"새 우편 리스트 DB에 요청.. LoginID : " << GetName() << L", UserUID : " << GetUID() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_READ_LETTER_REQ, POSTUID )
{
    CHECK_POST_ENABLE;

    KEVENT_READ_LETTER_ACK kPacket;
    kPacket.m_nOK = -99;

    bool bReadUpdated = false;

    SET_ERROR( ERR_UNKNOWN );

    // 편지 내용 받아오기
    if ( false == m_kUserLetterBox.ReadLetter( kPacket_, kPacket.m_kLetter, bReadUpdated ) ) {
        SET_ERR_GOTO( ERR_POSTOFFICE_20, END_PROC );
    }

    // DB Update
    if ( false == bReadUpdated ) {
        QUEUEING_EVENT_TO_DB_CHECK( EVENT_READ_LETTER_NOT, kPacket_ );
        START_LOG( clog, L"읽지않은 우편 DB에 상태값 갱신 요청.. Login : " << GetName() << L", PostUID : " << kPacket_ ) << END_LOG;
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,            kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_POSTOFFICE_20, kPacket.m_nOK, 20 ); // 읽기요청한 우편이 없음.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"우편 읽기 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"우편 읽기 결과 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( bReadUpdated )
        << BUILD_LOG( kPacket.m_kLetter.m_PostUID )
        << BUILD_LOG( kPacket.m_kLetter.m_dwFromUserUID )
        << BUILD_LOG( kPacket.m_kLetter.m_dwToUserUID )
        << BUILD_LOGc( kPacket.m_kLetter.m_cScriptType)
        << BUILD_LOG( kPacket.m_kLetter.m_nGamePoint )
        << BUILD_LOG( kPacket.m_kLetter.m_vecItem.size() )
        << BUILD_LOGtm( CTime(kPacket.m_kLetter.m_tmRegDate) )
        << BUILD_LOG( kPacket.m_kLetter.m_strFromNickName )
        << BUILD_LOG( kPacket.m_kLetter.m_strTitle )
        << BUILD_LOG( kPacket.m_kLetter.m_strMessage )
        << BUILD_LOG( kPacket.m_kLetter.m_bRead )
        << BUILD_LOG( kPacket.m_kLetter.m_bGetItemGP )
        << BUILD_LOG( kPacket.m_kLetter.m_nStorageTime ) << END_LOG;

    SEND_PACKET( EVENT_READ_LETTER_ACK );
}

_IMPL_ON_FUNC( EVENT_GET_ITEM_FROM_LETTER_REQ, POSTUID )
{
    CHECK_POST_ENABLE;

    SET_ERROR( ERR_UNKNOWN );

    KEVENT_GET_ITEM_FROM_LETTER_REQ kDBPacket;
    kDBPacket.m_PostUID = kPacket_;
    kDBPacket.m_bSystemLetter = false;
    kDBPacket.m_vecReward.clear();
    kDBPacket.m_nRewardGP = 0;

    KEVENT_GET_ITEM_FROM_LETTER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_PostUID = kPacket_;
    kPacket.m_bSystemLetter = false;
    kPacket.m_vecReward.clear();
    kPacket.m_nRewardGP = 0;
    kPacket.m_nCurrentGP = GetGP();

    int nMaxGP = SiKGSSimLayer()->m_iMaxGamePoint;
    USHORT usAgitItemCount = 0;
    USHORT usLookItemCount = 0;
    USHORT usNormalItemCount = 0;

    // 해당 우편이 있는지
    KPostItemInfo kPostItemInfo;
    if ( false == m_kUserLetterBox.GetLetter( kDBPacket.m_PostUID, kPostItemInfo ) ) {
        SET_ERR_GOTO( ERR_POSTOFFICE_21, END_PROC );
    }

    // 첨부 아이템을 받아갔는지
    if ( kPostItemInfo.m_bGetItemGP ) {
        START_LOG( cerr, L"해당 우편의 첨부 아이템(GP)을 이미 받아갔음.. Login : " << GetName() )
            << BUILD_LOG( kDBPacket.m_PostUID )
            << BUILD_LOG( kPostItemInfo.m_bGetItemGP ) << END_LOG;
        SET_ERR_GOTO( ERR_POSTOFFICE_22, END_PROC );
    }

    // 첨부아이템 정보 받아오기 (GP/아이템)
    kDBPacket.m_nRewardGP = kPostItemInfo.m_nGamePoint;
    kDBPacket.m_vecReward = kPostItemInfo.m_vecItem;
    kDBPacket.m_bSystemLetter = kPostItemInfo.IsSystemLetter();

    // 첨부아이템이 있는지
    if ( kDBPacket.m_nRewardGP <= 0 && kDBPacket.m_vecReward.empty() ) {
        START_LOG( cwarn, L"해당 우편은 첨부 아이템(GP)이 없음.. Login : " << GetName() )
            << BUILD_LOG( kDBPacket.m_PostUID ) << END_LOG;
        SET_ERR_GOTO( ERR_POSTOFFICE_27, END_PROC );
    }

    // GP가 있을 경우, 최대 GP를 초과하지 않는지
    if ( kDBPacket.m_nRewardGP > 0 ) {
        if ( GetGP() >= nMaxGP ) {
            START_LOG( cerr, L"GP소유 한도를 초과해 더이상 GP를 받을 수 없음.. Login : " << GetName() )
                << BUILD_LOG( kDBPacket.m_PostUID )
                << BUILD_LOG( kDBPacket.m_nRewardGP )
                << BUILD_LOG( GetGP() )
                << BUILD_LOG( nMaxGP ) << END_LOG;

            SET_ERR_GOTO( ERR_POSTOFFICE_23, END_PROC );
        }
    }

    // Item이 있을 경우, 인벤토리 크기를 초과하지 않는지.
    if ( false == kDBPacket.m_vecReward.empty() ) {

        std::set<GCITEMID> setAgitItemID;
        SiKAgitHelper()->GetAgitObjectItemIDList( setAgitItemID );

        std::vector<KItem>::iterator vitReward;
        for ( vitReward = kDBPacket.m_vecReward.begin() ; vitReward != kDBPacket.m_vecReward.end() ; ++vitReward ) {
            // 아지트 인벤토리 체크
            bool bAgitObject = ( setAgitItemID.find( vitReward->m_ItemID ) != setAgitItemID.end() );
            if ( bAgitObject ) {
                ++usAgitItemCount;
                if ( m_kInventory.GetAgitObjectItemNum() >= SiKAgitHelper()->GetMaxInvenSize() ) {
                    START_LOG( cerr, L"아지트아이템 첨부 받기에 충분한 인벤 공간이 없음.. Login : " << GetName() )
                        << BUILD_LOG( vitReward->m_ItemID )
                        << BUILD_LOG( m_kInventory.GetAgitObjectItemNum() )
                        << BUILD_LOG( SiKAgitHelper()->GetMaxInvenSize() ) << END_LOG;

                    SET_ERR_GOTO( ERR_POSTOFFICE_24, END_PROC );
                }
            }

            // 코디 인벤토리 체크
            bool bLookItem = SiKLookEquipment()->IsLookItemID( vitReward->m_ItemID );
            if ( bLookItem ) {
                ++usLookItemCount;
                if ( m_kInventory.GetLookItemNum() >= SiKLookEquipment()->GetMaxInvenSize() ) {
                    START_LOG( cerr, L"코디아이템 첨부 받기에 충분한 인벤 공간이 없음.. Login : " << GetName() )
                        << BUILD_LOG( vitReward->m_ItemID )
                        << BUILD_LOG( m_kInventory.GetLookItemNum() )
                        << BUILD_LOG( SiKLookEquipment()->GetMaxInvenSize() ) << END_LOG;

                    SET_ERR_GOTO( ERR_POSTOFFICE_25, END_PROC );
                }
            }

            // 일반 인벤토리 체크
            if ( false == bLookItem && false == bAgitObject ) {
                ++usNormalItemCount;
                if ( m_kInventory.GetTotalEnableSize() >= GetTrueInvenCapacity() ) {
                    START_LOG( cerr, L"일반아이템 첨부 받기에 충분한 인벤 공간이 없음.. Login : " << GetName() )
                        << BUILD_LOG( vitReward->m_ItemID )
                        << BUILD_LOG( m_kInventory.GetTotalEnableSize() )
                        << BUILD_LOG(GetTrueInvenCapacity() ) << END_LOG;

                    SET_ERR_GOTO( ERR_POSTOFFICE_26, END_PROC );
                }
            }
        }
    }

    // DB로 보내기
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_GET_ITEM_FROM_LETTER_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,            kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_POSTOFFICE_21, kPacket.m_nOK, 21 ); // 첨부아이템 받기 요청한 우편이 없음.
        ERR_CASE( ERR_POSTOFFICE_22, kPacket.m_nOK, 22 ); // 첨부아이템을 이미 받아갔음.
        ERR_CASE( ERR_POSTOFFICE_23, kPacket.m_nOK, 23 ); // GP소유 한도를 초과해 더이상 GP를 받을 수 없음.
        ERR_CASE( ERR_POSTOFFICE_24, kPacket.m_nOK, 24 ); // 아지트아이템 첨부 받기에 충분한 인벤 공간이 없음.
        ERR_CASE( ERR_POSTOFFICE_25, kPacket.m_nOK, 25 ); // 코디아이템 첨부 받기에 충분한 인벤 공간이 없음.
        ERR_CASE( ERR_POSTOFFICE_26, kPacket.m_nOK, 26 ); // 일반아이템 첨부 받기에 충분한 인벤 공간이 없음.
        ERR_CASE( ERR_POSTOFFICE_27, kPacket.m_nOK, 27 ); // 해당우편은 첨부아이템이 없음.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"우편 첨부아이템 받기 요청 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"우편 첨부아이템 받기 요청 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( kDBPacket.m_PostUID )
        << BUILD_LOG( kDBPacket.m_bSystemLetter )
        << BUILD_LOG( kDBPacket.m_vecReward.size() )
        << BUILD_LOG( kDBPacket.m_nRewardGP )
        << BUILD_LOG( nMaxGP )
        << BUILD_LOG( GetGP() )
        << BUILD_LOG( usAgitItemCount )
        << BUILD_LOG( usLookItemCount )
        << BUILD_LOG( usNormalItemCount ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_GET_ITEM_FROM_LETTER_ACK );
    }
}

IMPL_ON_FUNC( EVENT_GET_ITEM_FROM_LETTER_ACK )
{
    CHECK_POST_ENABLE;

    bool bDBUpdate = false;
    kPacket_.m_nCurrentGP = GetGP();

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( false == IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    // GP 동기화
    IncreaseGP( KGpStatInfo::GIT_POST_GP, bDBUpdate, kPacket_.m_nRewardGP );
    kPacket_.m_nCurrentGP = GetGP();

    // 우편 첨부물 초기화 (이거 필요한가?)
    m_kUserLetterBox.SetReceivedFlag( kPacket_.m_PostUID );

    // 획득 아이템 인벤토리 넣기
    if ( false == kPacket_.m_vecReward.empty() ) {
        m_kInventory.AddItemList( kPacket_.m_vecReward );
    }

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,            kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_POSTOFFICE_28, kPacket_.m_nOK, 28 ); // 우편 대기아이템 리스트 받아오기 DB처리 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
        default:
            START_LOG( cerr, L"우편 첨부아이템 받기 중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( GetName() ) << END_LOG;
            kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"우편 첨부아이템 받기 결과. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_PostUID )
        << BUILD_LOG( kPacket_.m_bSystemLetter )
        << BUILD_LOG( kPacket_.m_vecReward.size() )
        << BUILD_LOG( kPacket_.m_nRewardGP )
        << BUILD_LOG( kPacket_.m_nCurrentGP ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_GET_ITEM_FROM_LETTER_ACK );
}

IMPL_ON_FUNC( EVENT_DELETE_LETTER_REQ )
{
    CHECK_POST_ENABLE;

    KEVENT_DELETE_LETTER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_vecPostUID = kPacket_;
    KDB_EVENT_DELETE_LETTER_REQ kDBPacket;
    std::vector<POSTUID>::const_iterator cvit;

    SET_ERROR( ERR_UNKNOWN );

    kDBPacket.m_vecPostUID = kPacket_;

    // 삭제할 편지가 존재하는지?
    if ( false == m_kUserLetterBox.IsExist( kPacket_ ) ) {
        SET_ERR_GOTO( ERR_POSTOFFICE_30, END_PROC );
    }

    // 삭제할 우편의 첨부 아이템 정보 가져오기.
    for( cvit = kPacket_.begin(); cvit != kPacket_.end(); ++cvit ) {
        // 해당 우편이 있는지
        KPostItemInfo kPostItemInfo;
        if ( false == m_kUserLetterBox.GetLetter( *cvit, kPostItemInfo ) ) {
            SET_ERR_GOTO( ERR_POSTOFFICE_30, END_PROC );
        }
        kDBPacket.m_mapPostItemInfo[kPostItemInfo.m_PostUID] = kPostItemInfo.m_vecItem;
    }

    // DB Update
    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_DELETE_LETTER_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,            kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_POSTOFFICE_30, kPacket.m_nOK, 30 ); // 삭제요청한 우편이 없음.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"우편 삭제 요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"우편 삭제 요청 결과 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.size() )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_DELETE_LETTER_ACK );
    }
}

IMPL_ON_FUNC( EVENT_DELETE_LETTER_ACK )
{
    CHECK_POST_ENABLE;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( false == IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 우편 삭제
    m_kUserLetterBox.DeleteLetter( kPacket_.m_vecPostUID );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,            kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_POSTOFFICE_31, kPacket_.m_nOK, 31 ); // 우편삭제 DB처리 실패

    default:
        kPacket_.m_nOK = 99;
        START_LOG( cerr, L"우편 삭제 중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"우편삭제 처리 결과 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecPostUID.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_DELETE_LETTER_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_RENEWAL_LETTER_REQ )
{
    CHECK_POST_ENABLE;

    KEVENT_RENEWAL_LETTER_ACK kPacket;
    m_kUserLetterBox.GetRenewalLetter( kPacket.m_nNewUserLetterCount, kPacket.m_nNewSystemLetterCount, kPacket.m_vecNewLetterTitle );
    kPacket.m_nTotalLetterCount = m_kUserLetterBox.GetTotalLetterCount( KEVENT_GET_POST_LETTER_LIST_REQ::LL_ALL );

    SEND_PACKET( EVENT_RENEWAL_LETTER_ACK );

    START_LOG( clog, L"신규우편 타이틀 리스트 요청 전달.. LoginID : " << GetName() )
        << BUILD_LOG( kPacket.m_nNewUserLetterCount )
        << BUILD_LOG( kPacket.m_nNewSystemLetterCount )
        << BUILD_LOG( kPacket.m_vecNewLetterTitle.size() )
        << BUILD_LOG( kPacket.m_nTotalLetterCount ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_NEW_POST_LETTER_INFO_ACK )
{
    CHECK_POST_ENABLE;
    KEVENT_NEW_POST_LETTER_INFO_NOT kPacket;

    // 우편함 갱신
    m_kUserLetterBox.Init( kPacket_, kPacket.m_nNewUserLetterCount, kPacket.m_nNewSystemLetterCount, kPacket.m_vecNewLetterTitle );
    kPacket.m_nTotalLetterCount = m_kUserLetterBox.GetTotalLetterCount( KEVENT_GET_POST_LETTER_LIST_REQ::LL_ALL );

    // 클라이언트로 읽지않은 편지수를 보내준다.
    SEND_PACKET( EVENT_NEW_POST_LETTER_INFO_NOT );

    START_LOG( clog, L"새 우편 갱신.. LoginID : " << GetName() )
        << BUILD_LOG( kPacket_.size() )
        << BUILD_LOG( kPacket.m_nNewUserLetterCount )
        << BUILD_LOG( kPacket.m_nNewSystemLetterCount )
        << BUILD_LOG( kPacket.m_nTotalLetterCount )
        << BUILD_LOG( kPacket.m_vecNewLetterTitle.size() ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_SOCKS_INFO_REQ )
{
    KUserSocksInfo kPacket = m_mapSocksInfo;

    SEND_PACKET( EVENT_SOCKS_INFO_ACK );
}

IMPL_ON_FUNC( EVENT_DUNGEON_CLEAR_REWARD_REQ )
{
    kPacket_.m_vecRewardItem.clear();

    KEVENT_DUNGEON_CLEAR_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nModeID = kPacket_.m_nModeID;
    kPacket.m_vecIndexItemID = kPacket_.m_vecIndexItemID;
    kPacket.m_vecReward.clear();
    kPacket.m_usLeftRewardCount = 0;

    KHeroDungeon kDungeonInfo;

    SET_ERROR( ERR_UNKNOWN );

    // 던전 클리어 정보 찾기
    KDungeonUserInfo kInfo;
    if ( false == GetDungeonUserInfo( kPacket_.m_nModeID, kInfo ) ) {
        SET_ERR_GOTO( ERR_DUNGEON_CLEAR_REWARD_01, END_PROC );
    }
    kPacket.m_usLeftRewardCount = kInfo.m_kClearData.m_usLeftRewardCount;


    if ( SiKHeroDungeonManager()->IsHeroDungeon( kPacket_.m_nModeID ) )
    {
        if ( false == SiKHeroDungeonManager()->GetHeroDungeonInfo( kPacket_.m_nModeID, kDungeonInfo ) ) {
            SET_ERR_GOTO( ERR_DUNGEON_CLEAR_REWARD_02, END_PROC );
        }
    }

    // 보상 개수 체크
    if ( kPacket_.m_vecIndexItemID.size() > kInfo.m_kClearData.m_usLeftRewardCount ) {
        START_LOG( cerr, L"남은 보상개수 : " << kInfo.m_kClearData.m_usLeftRewardCount << L", 요청한 보상개수 : " << kPacket_.m_vecIndexItemID.size() ) << END_LOG;
        SET_ERR_GOTO( ERR_DUNGEON_CLEAR_REWARD_03, END_PROC );
    }

    // 던전 보상 아이템 세부정보 받기
    if ( false == kPacket_.m_vecIndexItemID.empty() ) {
        std::vector<INDEX_GCITEMID>::iterator vit;
        for ( vit = kPacket_.m_vecIndexItemID.begin() ; vit != kPacket_.m_vecIndexItemID.end() ; ++vit ) {
            std::map<INDEX_GCITEMID,KManufactureItem>::iterator mit;
            mit = kDungeonInfo.m_mapClearReward.find( *vit );
            if ( mit == kDungeonInfo.m_mapClearReward.end() ) {
                START_LOG( cerr, L"보상리스트에 없는 보상.. Index : " << vit->first << L", ItemID : " << vit->second ) << END_LOG;
                SET_ERR_GOTO( ERR_DUNGEON_CLEAR_REWARD_04, END_PROC );
            }

            kPacket_.m_vecRewardItem.push_back( mit->second );
        }
    }

    if ( kPacket_.m_vecRewardItem.empty() ) {
        SET_ERR_GOTO( ERR_DUNGEON_CLEAR_REWARD_05, END_PROC );
    }

    // DB Update
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_DUNGEON_CLEAR_REWARD_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_DUNGEON_CLEAR_REWARD_01, kPacket.m_nOK, 1 ); // 해당하는 던전 클리어정보가 없음
        ERR_CASE( ERR_DUNGEON_CLEAR_REWARD_02, kPacket.m_nOK, 2 ); // 해당하는 영웅던전이 없음.
        ERR_CASE( ERR_DUNGEON_CLEAR_REWARD_03, kPacket.m_nOK, 3 ); // 남은 보상개수가 부족함.
        ERR_CASE( ERR_DUNGEON_CLEAR_REWARD_04, kPacket.m_nOK, 4 ); // 보상리스트에 없는 보상을 선택했음.
        ERR_CASE( ERR_DUNGEON_CLEAR_REWARD_05, kPacket.m_nOK, 5 ); // 보상받을 아이템이 없음.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.

    default:
        START_LOG( cerr, L"던전 클리어보상 받기 요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"던전 클리어보상 받기 요청 결과 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_nModeID )
        << BUILD_LOG( kPacket_.m_vecIndexItemID.size() )
        << BUILD_LOG( kPacket_.m_vecRewardItem.size() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_usLeftRewardCount ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_DUNGEON_CLEAR_REWARD_ACK );
    }
}

IMPL_ON_FUNC( EVENT_DUNGEON_CLEAR_REWARD_ACK )
{
    std::map< char, std::map<int,KDungeonUserInfo> >::iterator mitTotal;
    std::map<int,KDungeonUserInfo>::iterator mitDungeon;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( false == IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 획득 아이템 넣기
    if ( false == kPacket_.m_vecReward.empty() ) {
        m_kInventory.AddItemList( kPacket_.m_vecReward );
    }

    mitTotal = m_mapDifficulty.find( m_cCurrentCharacter );
    if( mitTotal == m_mapDifficulty.end() ) {
        START_LOG( cerr, L"이미 체크했는데 해당 던전 클리어 정보가 없음.. Login : " << GetName() << L", ModeID : " << kPacket_.m_nModeID ) << END_LOG;
        goto END_PROC;
    }

    // 던전 클리어 정보 찾기
    mitDungeon = mitTotal->second.find( kPacket_.m_nModeID );
    if ( mitDungeon == mitTotal->second.end() ) {
        START_LOG( cerr, L"이미 체크했는데 해당 던전 클리어 정보가 없음.. Login : " << GetName() << L", ModeID : " << kPacket_.m_nModeID ) << END_LOG;
        goto END_PROC;
    }

    // 보상 개수 갱신
    if ( false == kPacket_.m_vecIndexItemID.empty() ) {
        USHORT usRewardCount = std::min<USHORT>( kPacket_.m_vecIndexItemID.size(), mitDungeon->second.m_kClearData.m_usLeftRewardCount );
        mitDungeon->second.m_kClearData.m_usLeftRewardCount -= usRewardCount;
        kPacket_.m_usLeftRewardCount = mitDungeon->second.m_kClearData.m_usLeftRewardCount;

        START_LOG( clog, L"클리어 보상개수 갱신.. Login : " << GetName() )
            << BUILD_LOG( usRewardCount )
            << BUILD_LOG( kPacket_.m_vecIndexItemID.size() )
            << BUILD_LOG( mitDungeon->second.m_kClearData.m_usLeftRewardCount ) << END_LOG;
    }

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_DUNGEON_CLEAR_REWARD_06, kPacket_.m_nOK, 6 ); // 보상아이템 지급 DB처리 실패.
    default:
        kPacket_.m_nOK = 99;
        START_LOG( cerr, L"던전 클리어보상 받기 중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( GetName() ) << END_LOG;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"던전 클리어보상 받기 결과 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nModeID )
        << BUILD_LOG( kPacket_.m_vecIndexItemID.size() )
        << BUILD_LOG( kPacket_.m_vecReward.size() )
        << BUILD_LOG( kPacket_.m_usLeftRewardCount ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_DUNGEON_CLEAR_REWARD_ACK );
}

IMPL_ON_FUNC( EVENT_EQUIP_LEVEL_DOWN_REQ )
{
    CHECK_EQUIP_LV_DOWN_ENABLE;

    KDB_EVENT_EQUIP_LEVEL_DOWN_REQ kDBPacket;
    kDBPacket.m_nEquipLvDown = 0;
    kDBPacket.m_nMaterialCount = 0;

    KEVENT_EQUIP_LEVEL_DOWN_ACK kPacket;
    kPacket.m_nOK = -99;

    DWORD dwEquipLevel = 0;
    int nEquipLvDownLimit = 0;

    SET_ERROR( ERR_UNKNOWN );

    // 장비 아이템이 있는가
    {
        KItemPtr pkItem = m_kInventory.FindItemByItemUID( kPacket_.m_EquipItemUID );
        if ( !pkItem ) {
            SET_ERR_GOTO( ERR_EQUIP_LEVEL_DOWN_01, END_PROC );
        }

        kDBPacket.m_EquipItem = *pkItem;
    }

    // 장착레벨을 낮출 수 있는 아이템인가
    nEquipLvDownLimit = SiKItemManager()->GetEquipLvDownLimit( kDBPacket.m_EquipItem.m_ItemID );
    if ( nEquipLvDownLimit == 0 ) {
        SET_ERR_GOTO( ERR_EQUIP_LEVEL_DOWN_04, END_PROC );
    }

    // 장착레벨 한계치까지 낮췄는가
    if ( kDBPacket.m_EquipItem.m_sEquipLvDown >= nEquipLvDownLimit ) {
        START_LOG( cwarn, L"장착레벨 낮추기 한계다..  Login : " << GetName() )
            << BUILD_LOG( kDBPacket.m_EquipItem.m_ItemID )
            << BUILD_LOG( kDBPacket.m_EquipItem.m_sEquipLevel )
            << BUILD_LOG( kDBPacket.m_EquipItem.m_sEquipLvDown )
            << BUILD_LOG( nEquipLvDownLimit ) << END_LOG;
        SET_ERR_GOTO( ERR_EQUIP_LEVEL_DOWN_05, END_PROC );
    }

    // 아이템 장착레벨 구하기
    LIF( SiKItemManager()->GetItemEquipLevel( kDBPacket.m_EquipItem.m_ItemID, dwEquipLevel ) );
    if( dwEquipLevel == 0 ) {
        dwEquipLevel = static_cast<DWORD>( kDBPacket.m_EquipItem.m_sEquipLevel );
    }

    if ( dwEquipLevel - kDBPacket.m_EquipItem.m_sEquipLvDown <= 1 ) {
        START_LOG( cwarn, L"장착아이템 레벨이 1이하다..  Login : " << GetName() )
            << BUILD_LOG( kDBPacket.m_EquipItem.m_ItemID )
            << BUILD_LOG( kDBPacket.m_EquipItem.m_sEquipLevel )
            << BUILD_LOG( kDBPacket.m_EquipItem.m_sEquipLvDown )
            << BUILD_LOG( dwEquipLevel )
            << BUILD_LOG( nEquipLvDownLimit ) << END_LOG;
        SET_ERR_GOTO( ERR_EQUIP_LEVEL_DOWN_05, END_PROC );
    }

    // 재료 아이템이 있는가
    {
        KItemPtr pkItem = m_kInventory.FindDurationItemByItemUID( kPacket_.m_MaterialItemUID );
        if ( !pkItem ) {
            SET_ERR_GOTO( ERR_EQUIP_LEVEL_DOWN_02, END_PROC );
        }

        if ( false == SiKEquipLvDown()->IsMarerialItemID( pkItem->m_ItemID ) ) {
            SET_ERR_GOTO( ERR_EQUIP_LEVEL_DOWN_06, END_PROC );
        }

        kDBPacket.m_MaterialItem = *pkItem;
    }

    // 재료 아이템 수량이 충분한가
    kDBPacket.m_nEquipLvDown = kDBPacket.m_EquipItem.m_sEquipLvDown + 1;
    kDBPacket.m_nMaterialCount = SiKEquipLvDown()->CalcMaterialIemCount( dwEquipLevel, kDBPacket.m_EquipItem.m_sEquipLvDown );
    if ( kDBPacket.m_MaterialItem.m_nCount < kDBPacket.m_nMaterialCount ) {
        SET_ERR_GOTO( ERR_EQUIP_LEVEL_DOWN_03, END_PROC );
    }

    // DB Update
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_EQUIP_LEVEL_DOWN_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_EQUIP_LEVEL_DOWN_01, kPacket.m_nOK, 1 ); // 인벤토리에서 장비 아이템을 찾을 수 없음.
        ERR_CASE( ERR_EQUIP_LEVEL_DOWN_02, kPacket.m_nOK, 2 ); // 인벤토리에서 재료 아이템을 찾을 수 없음.
        ERR_CASE( ERR_EQUIP_LEVEL_DOWN_03, kPacket.m_nOK, 3 ); // 인벤토리에서 재료 아이템 수량이 부족함.
        ERR_CASE( ERR_EQUIP_LEVEL_DOWN_04, kPacket.m_nOK, 4 ); // 장착레벨을 낮출 수 없는 아이템.
        ERR_CASE( ERR_EQUIP_LEVEL_DOWN_05, kPacket.m_nOK, 5 ); // 더이상 장착레벨을 낮출 수 없음.
        ERR_CASE( ERR_EQUIP_LEVEL_DOWN_06, kPacket.m_nOK, 6 ); // 선택한 재료가 재료아이템이 아니다.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.

    default:
        START_LOG( cerr, L"아이템 장착레벨 낮추기 요청 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"아이템 장착레벨 낮추기 요청 결과 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_EquipItemUID )
        << BUILD_LOG( kPacket_.m_MaterialItemUID )
        << BUILD_LOG( kDBPacket.m_EquipItem.m_ItemID )
        << BUILD_LOG( kDBPacket.m_EquipItem.m_ItemUID )
        << BUILD_LOG( kDBPacket.m_EquipItem.m_sEquipLevel )
        << BUILD_LOG( kDBPacket.m_EquipItem.m_sEquipLvDown )
        << BUILD_LOG( kDBPacket.m_MaterialItem.m_ItemID )
        << BUILD_LOG( kDBPacket.m_MaterialItem.m_ItemUID )
        << BUILD_LOG( kDBPacket.m_MaterialItem.m_nCount )
        << BUILD_LOG( kDBPacket.m_nEquipLvDown )
        << BUILD_LOG( kDBPacket.m_nMaterialCount )
        << BUILD_LOG( dwEquipLevel )
        << BUILD_LOG( nEquipLvDownLimit ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_EQUIP_LEVEL_DOWN_ACK );
    }
}

IMPL_ON_FUNC( EVENT_EQUIP_LEVEL_DOWN_ACK )
{
    CHECK_EQUIP_LV_DOWN_ENABLE;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( false == IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    {
        KItemPtr pkItem = m_kInventory.FindItemByItemUID( kPacket_.m_EquipItem.m_ItemUID );
        if ( pkItem ) {
            pkItem->m_sEquipLvDown = kPacket_.m_EquipItem.m_sEquipLvDown;

            START_LOG( clog, L"장비 아이템 장착레벨 감소 적용.. Login : " << GetName() )
                << BUILD_LOG( kPacket_.m_EquipItem.m_ItemUID )
                << BUILD_LOG( kPacket_.m_EquipItem.m_ItemID )
                << BUILD_LOG( kPacket_.m_EquipItem.m_sEquipLevel )
                << BUILD_LOG( kPacket_.m_EquipItem.m_sEquipLvDown ) << END_LOG;
        }
        else {
            START_LOG( cerr, L"장착레벨 감소된 장비 아이템을 인벤토리에서 찾을 수 없음.. Login : " << GetName() )
                << BUILD_LOG( kPacket_.m_EquipItem.m_ItemUID )
                << BUILD_LOG( kPacket_.m_EquipItem.m_ItemID )
                << BUILD_LOG( kPacket_.m_EquipItem.m_sEquipLevel )
                << BUILD_LOG( kPacket_.m_EquipItem.m_sEquipLvDown ) << END_LOG;
        }
    }

    {
        KItemPtr pkItem = m_kInventory.FindDurationItemByItemUID( kPacket_.m_MaterialItem.m_ItemUID );
        if ( pkItem ) {
            pkItem->m_nCount = kPacket_.m_MaterialItem.m_nCount;
            pkItem->m_nInitCount = kPacket_.m_MaterialItem.m_nInitCount;

            // 수량이 0이면 삭제 (DB에선 이미 삭제되었음)
            if ( pkItem->m_nCount == 0 ) {
                LIF( m_kInventory.RemoveItem( pkItem->m_ItemUID ) );
            }

            START_LOG( clog, L"재료 아이템 감소 적용.. Login : " << GetName() )
                << BUILD_LOG( kPacket_.m_MaterialItem.m_ItemUID )
                << BUILD_LOG( kPacket_.m_MaterialItem.m_ItemID )
                << BUILD_LOG( kPacket_.m_MaterialItem.m_nCount ) << END_LOG;
        }
    }

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_EQUIP_LEVEL_DOWN_07, kPacket_.m_nOK, 7 ); // 장착레벨 낮추기 DB작업 실패.

    default:
        kPacket_.m_nOK = 99;
        START_LOG( cerr, L"아이템 장착레벨 낮추기 중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( GetName() ) << END_LOG;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"아이템 장착레벨 낮추기 결과 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_EquipItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_EquipItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_EquipItem.m_sEquipLevel )
        << BUILD_LOG( kPacket_.m_EquipItem.m_sEquipLvDown )
        << BUILD_LOG( kPacket_.m_MaterialItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_MaterialItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_MaterialItem.m_nCount )
        << BUILD_LOG( kPacket_.m_MaterialItem.m_nInitCount ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_EQUIP_LEVEL_DOWN_ACK );
}

_IMPL_ON_FUNC( EVENT_ECLIPSE_TIME_EVENT_INFO_NOT, DWORD )
{
    KEVENT_ECLIPSE_TIME_EVENT_INFO_NOT kPacket;
    SiKEclipseTimeEvent()->GetEclipseTimeInfo( kPacket );
    SEND_PACKET( EVENT_ECLIPSE_TIME_EVENT_INFO_NOT );

    START_LOG( clog, L"이클립스 타임 이벤트 스크립트 변경시 접속중인 유저에게 데이터 전달: " << m_strName )
        << BUILD_LOGtm( CTime( kPacket.m_tmBegin ) )
        << BUILD_LOGtm( CTime( kPacket.m_tmEnd ) )
        << BUILD_LOG( kPacket.m_dwDuration )
        << BUILD_LOG( kPacket.m_dwPeriod )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_ECLIPSE_PLOT_INFO_REQ )
{
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_ECLIPSE_PLOT_INFO_ACK kPacket;

    if ( m_kEclipsePlot.m_bSelectProgressDB == false ) { // 서버 접속 후 1회만 DB에서 받아오기
        QUEUEING_ID_TO_DB_CHECK( EVENT_ECLIPSE_PLOT_INFO_REQ );
    }
    else {
        SiKEclipsePlot()->GetRewardInfo( kPacket );
        kPacket.m_nOK = 0;
        kPacket.m_nProgress = m_kEclipsePlot.m_nProgressFinalReward;
        kPacket.m_bGotFinalReward = m_kEclipsePlot.m_bGotFinalReward;
        SEND_PACKET( EVENT_ECLIPSE_PLOT_INFO_ACK );
    }
    SET_ERROR( NET_OK );

END_PROC:

    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업
    default:
        START_LOG( cerr, L"알 수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_ECLIPSE_PLOT_INFO_ACK );
    }

    START_LOG( clog, L"개기일식 미니게임 정보, User : " << GetName() )
        << BUILD_LOG( kPacket.m_nProgress )
        << BUILD_LOG( m_kEclipsePlot.m_bSelectProgressDB )
        << END_LOG;
}

_IMPL_ON_FUNC( EVENT_ECLIPSE_PLOT_INFO_ACK, KEclipsePlotData )
{
    KEVENT_ECLIPSE_PLOT_INFO_ACK kPacket;

    if ( kPacket_.m_nProgressFinalReward >= 0 ) {
        m_kEclipsePlot.m_bSelectProgressDB = true;
        kPacket.m_nOK = 0;
    }
    m_kEclipsePlot.m_nProgressFinalReward = kPacket_.m_nProgressFinalReward;
    m_kEclipsePlot.m_bGotFinalReward = kPacket_.m_bGotFinalReward;

    kPacket.m_nProgress = m_kEclipsePlot.m_nProgressFinalReward;
    kPacket.m_bGotFinalReward = m_kEclipsePlot.m_bGotFinalReward;
    SiKEclipsePlot()->GetRewardInfo( kPacket );
    
    START_LOG( clog, L"개기일식 진행도, 최종보상 여부 , User : " << GetName() )
        << BUILD_LOG( m_kEclipsePlot.m_nProgressFinalReward )
        << BUILD_LOG( m_kEclipsePlot.m_bGotFinalReward )
        << END_LOG;

    SEND_PACKET( EVENT_ECLIPSE_PLOT_INFO_ACK );
}

// #EVENT_ECLIPSE_PLOT_TIME_REWARD_REQ
IMPL_ON_FUNC_NOPARAM( EVENT_ECLIPSE_PLOT_TIME_REWARD_REQ )
{
    SET_ERROR( ERR_UNKNOWN );

    KDB_EVENT_ECLIPSE_PLOT_TIME_REWARD_REQ kDBPacket;
    KEVENT_ECLIPSE_PLOT_TIME_REWARD_ACK kPacket;

    if ( m_kEclipsePlot.m_bRewardable == false ) {
        SET_ERR_GOTO( ERR_ECLIPSE_PLOT_00, END_PROC );
    }

    SiKEclipsePlot()->GetTimeRewardItem( kDBPacket );

    QUEUEING_EVENT_TO_DB_USING_DISPATCHER_CHECK_IN( DB_EVENT_ECLIPSE_PLOT_TIME_REWARD_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_ECLIPSE_PLOT_00,  kPacket.m_nOK, NetError::ERR_ECLIPSE_PLOT_00 ); // 개기일식 시간 보상 받을 수 없는 상태
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }
    
    if (kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_ECLIPSE_PLOT_TIME_REWARD_ACK );

        START_LOG( cerr, L"개기일식의 음모 시간 보상 실패 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( GetName() )
            << BUILD_LOG( m_kEclipsePlot.m_nProgressFinalReward )
            << END_LOG;
    }
}

IMPL_ON_FUNC( EVENT_ECLIPSE_PLOT_TIME_REWARD_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    m_kInventory.AddItemList( kPacket_.m_vecReward );

    m_kEclipsePlot.m_bRewardable = false; // 보상받을 수 있는지 여부, 보상 받았으므로 초기값으로 설정
    
    if ( NetError::GetLastNetError() != NetError::ERR_ECLIPSE_PLOT_08 ) {
        m_kEclipsePlot.m_nProgressFinalReward += 1; // 시간 보상 후 최종 보상 진행도를 올린다.
    }

    kPacket_.m_nProgress = m_kEclipsePlot.m_nProgressFinalReward;

    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_ECLIPSE_PLOT_08,  kPacket_.m_nOK, NetError::ERR_ECLIPSE_PLOT_08 ); // 최종 보상 진행도 DB 저장 실패
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 아직 처리중인 작업
    default:
        START_LOG( cerr, L" 알 수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( GetName() ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"개기일식의 음모 시간 보상 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_vecReward.size() )
        << BUILD_LOG( m_kEclipsePlot.m_nProgressFinalReward )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_ECLIPSE_PLOT_TIME_REWARD_ACK );
}

IMPL_ON_FUNC( EVENT_ECLIPSE_PLOT_HUNT_REWARD_REQ )
{
    // 클라이언트가 아이템 정보를 보내줘야 할까? 아이템 아이디는 필요함.
    SET_ERROR( ERR_UNKNOWN );
    
    KEVENT_ECLIPSE_PLOT_HUNT_REWARD_ACK kPacket;
    KDB_EVENT_ECLIPSE_PLOT_HUNT_REWARD_REQ kDBPacket;
    KItemPtr pkItem;

    pkItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_ItemID );
    if ( NULL == pkItem ) {
        SET_ERR_GOTO( ERR_ECLIPSE_PLOT_01, END_PROC );
    }

    if ( pkItem->m_nCount < SiKEclipsePlot()->GetHuntItemCount() ) {
        SET_ERR_GOTO( ERR_ECLIPSE_PLOT_02, END_PROC );
    }

    kDBPacket.m_kHuntItem = *pkItem;
    kDBPacket.m_nUseHuntItem = SiKEclipsePlot()->GetHuntItemCount(); // 보상에 필요한 수집 아이템 개수
    SiKEclipsePlot()->GetHuntRewardItem( kDBPacket.m_vecHuntRewardItem );

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_ECLIPSE_PLOT_HUNT_REWARD_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,              kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_ECLIPSE_PLOT_01, kPacket.m_nOK, NetError::ERR_ECLIPSE_PLOT_01 );
        ERR_CASE( ERR_ECLIPSE_PLOT_02, kPacket.m_nOK, NetError::ERR_ECLIPSE_PLOT_02 );
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업
    default:
        START_LOG( cerr, L" 알 수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_ECLIPSE_PLOT_HUNT_REWARD_ACK );
    }
    _LOG_SUCCESS( kPacket.m_nOK == 0, L"개기일식 수집 아이템 보상 요청 : " << GetName() )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket_.m_nItemCount )
        << BUILD_LOG( m_kEclipsePlot.m_nProgressFinalReward )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_ECLIPSE_PLOT_HUNT_REWARD_ACK )
{
    KItemPtr pkHuntItem;
    //KEVENT_ECLIPSE_PLOT_HUNT_REWARD_ACK kPacket;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) && !IS_CORRECT( ERR_ECLIPSE_PLOT_04 ) ) {
        goto END_PROC;
    }

    pkHuntItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_kHuntItem.m_ItemID );
    if( pkHuntItem ) {
        pkHuntItem->m_nCount = kPacket_.m_kHuntItem.m_nCount;
        pkHuntItem->m_nInitCount = kPacket_.m_kHuntItem.m_nCount;
        //kPacket.m_kHuntItem = *pkHuntItem;
    }

    m_kInventory.AddItemList( kPacket_.m_vecReward );
    
    if ( NetError::GetLastNetError() != NetError::ERR_ECLIPSE_PLOT_08 ) {
        m_kEclipsePlot.m_nProgressFinalReward += 1; // 수집 보상 후 최종 보상 진행도를 올린다.
    }   
    kPacket_.m_nProgress = m_kEclipsePlot.m_nProgressFinalReward;

END_PROC:

    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,              kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_ECLIPSE_PLOT_04, kPacket_.m_nOK, NetError::ERR_ECLIPSE_PLOT_04 ); // 수집 아이템 보상시 DB 동기화 실패
        ERR_CASE( ERR_ECLIPSE_PLOT_08, kPacket_.m_nOK, NetError::ERR_ECLIPSE_PLOT_08 ); // 최종 보상 진행도 DB 저장 실패
    default:
        START_LOG( cerr, L" 알 수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    SEND_RECEIVED_PACKET( EVENT_ECLIPSE_PLOT_HUNT_REWARD_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"개기일식 수집 아이템 보상 응답 : " << GetName() )
        << BUILD_LOG( kPacket_.m_vecReward.size() )
        << BUILD_LOG( m_kEclipsePlot.m_nProgressFinalReward )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_ECLIPSE_PLOT_FINAL_REWARD_REQ )
{
    KEVENT_ECLIPSE_PLOT_FINAL_REWARD_ACK kPacket;
    KDB_EVENT_ECLIPSE_PLOT_FINAL_REWARD_REQ kDBPacket;

    if ( m_kEclipsePlot.m_nProgressFinalReward < SiKEclipsePlot()->GetMaxProgress() ) {
        SET_ERR_GOTO( ERR_ECLIPSE_PLOT_05, END_PROC );
    }

    if ( m_kEclipsePlot.m_bGotFinalReward == true ) {
        SET_ERR_GOTO( ERR_ECLIPSE_PLOT_06, END_PROC );
    }

    SiKEclipsePlot()->GetFinalRewardItem( kDBPacket );
    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_ECLIPSE_PLOT_FINAL_REWARD_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,              kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_ECLIPSE_PLOT_05, kPacket.m_nOK, NetError::ERR_ECLIPSE_PLOT_05 ); // 최종 보상 조건이 아닌데 최종보상 요청함.
        ERR_CASE( ERR_ECLIPSE_PLOT_06, kPacket.m_nOK, NetError::ERR_ECLIPSE_PLOT_06 ); // 최종 보상을 이미 받았는데 보상 요청함.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업
    default:
        START_LOG( cerr, L" 알 수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( GetName() ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_ECLIPSE_PLOT_FINAL_REWARD_ACK );
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"개기일식 최종 보상 요청 : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_kEclipsePlot.m_bGotFinalReward )
        << BUILD_LOG( m_kEclipsePlot.m_nProgressFinalReward )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_ECLIPSE_PLOT_FINAL_REWARD_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    m_kInventory.AddItemList( kPacket_.m_vecReward );
    m_kEclipsePlot.m_bGotFinalReward = true;

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,              kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_ECLIPSE_PLOT_07, kPacket_.m_nOK, NetError::ERR_ECLIPSE_PLOT_07 ); // 최종 보상 받은 기록 DB 저장 실패
    default:
        START_LOG( cerr, L" 알 수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( GetName() )
            << END_LOG;
        kPacket_.m_nOK = 99;
    }
    SEND_RECEIVED_PACKET( EVENT_ECLIPSE_PLOT_FINAL_REWARD_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"개기일식 최종 보상 응답 : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_kEclipsePlot.m_bGotFinalReward )
        << BUILD_LOG( m_kEclipsePlot.m_nProgressFinalReward )
        << BUILD_LOG( kPacket_.m_vecReward.size() )
        << END_LOG;
}

_IMPL_ON_FUNC( EVENT_SIGN_BOARD_ECLIPSE_ALARM_NOT, int )
{
    std::wstring wstrData;
    std::wstring wstrStartEventMsg;
    std::wstring wstrEndEventMsg;

    if ( !SiKLoudMessage()->GetEclipseMsg( wstrStartEventMsg, wstrEndEventMsg, GetLanguageCode() ) ) {
        START_LOG( cerr, L"일식 이벤트 공지 내용 가져오기 실패" ) << END_LOG;
        return;
    }

    switch ( kPacket_ ) {
        case KEclipseTimeEvent::AL_EVENT_START:
        {
            wstrData = wstrStartEventMsg;
        }
        break;
        case KEclipseTimeEvent::AL_EVENT_END:
        {
            if ( GetStateID() == KUserFSM::STATE_PLAYING ) {    // 던전, 대전 플레이 중이면, '이벤트 종료 공지 메시지'는 뿌리지 않음
                return;
            }
            wstrData = wstrEndEventMsg;
        }
        break;
    }

    KSignBoardData kData;
    kData.m_dwColor         = 0L;
    kData.m_ItemID          = 0L;
    kData.m_dwSenderUID     = 0L;
    kData.m_eType           = KSignBoardData::SBT_SERVER_ADMIN;   // 운영자용 전광판
    kData.m_strSenderNick.clear();
    kData.m_strMsg = wstrData;

    SEND_DATA( EVENT_SIGN_BOARD_NOT, kData );

    START_LOG( clog, L"일식 시간 이벤트 공지 전달 , User : " << GetName() )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( wstrData )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_ECLIPSE_COLLECT_INFO_REQ )
{
    KEVENT_ECLIPSE_COLLECT_INFO_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    // 이벤트 기간인가?
    if( !SiKEclipseCollect()->IsRun() ) {
        SET_ERR_GOTO( ERR_ECLIPSE_COLLECT_01, END_PROC );
    }

    // 고서 리스트 가져오기.
    SiKEclipseCollect()->GetEclipseCollectItemList( kPacket.m_setEclipseCollectItemList );

    // 보상 아이템 리스트 가져오기.
    SiKEclipseCollect()->GetRewardItem( kPacket.m_vecRewardItem );

    kPacket.m_bRewardEnable = GetEclipseCollectComplete();

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 );     // 성공.
        ERR_CASE( ERR_ECLIPSE_COLLECT_01,   kPacket.m_nOK, 1 );     // 이벤트 기간이 아님
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"고서 모으기 Event 정보 요청중 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L"고서 모으기 Event 정보 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_bRewardEnable )
        << BUILD_LOG( kPacket.m_setEclipseCollectItemList.size() )
        << BUILD_LOG( kPacket.m_vecRewardItem.size() ) << END_LOG;

    SEND_PACKET( EVENT_ECLIPSE_COLLECT_INFO_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_ECLIPSE_COLLECT_REWARD_REQ )
{
    KEVENT_ECLIPSE_COLLECT_REWARD_ACK kPacket;
    KDB_EVENT_ECLIPSE_COLLECT_REWARD_REQ kDBPacket;
    KItemPtr pkItem;
    std::set<GCITEMID> setEclipseCollectItemList;
    setEclipseCollectItemList.clear();
    std::set<GCITEMID>::iterator sit;
    bool bComplete = false;
    SET_ERROR( ERR_UNKNOWN );

    // 이벤트 기간인가?
    if( !SiKEclipseCollect()->IsRun() ) {
        SET_ERR_GOTO( ERR_ECLIPSE_COLLECT_01, END_PROC );
    }

    bComplete = GetEclipseCollectComplete();
    if( bComplete ) {
        SET_ERR_GOTO( ERR_ECLIPSE_COLLECT_03, END_PROC );
    }

    // 조각 아이템 전부가 존재하는가?
    SiKEclipseCollect()->GetEclipseCollectItemList( setEclipseCollectItemList );
    for( sit = setEclipseCollectItemList.begin(); sit != setEclipseCollectItemList.end(); ++sit ) {
        pkItem = m_kInventory.FindItemByItemID( *sit );
        if( NULL == pkItem ) {
            SET_ERR_GOTO( ERR_ECLIPSE_COLLECT_02, END_PROC );
        }
        pkItem.reset();
    }

    // 구슬 상자 조각 아이템 리스트 받아오기.
    kDBPacket.m_setItemList = setEclipseCollectItemList;
    SiKEclipseCollect()->GetRewardItem( kDBPacket.m_vecRewardItem );
    if( kDBPacket.m_vecRewardItem.empty() ) {
        SET_ERR_GOTO( ERR_ECLIPSE_COLLECT_02, END_PROC );
    }

    // DB에 큐잉.
    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_ECLIPSE_COLLECT_REWARD_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_ECLIPSE_COLLECT_01,   kPacket.m_nOK, 1 );    // 고서 모으기 이벤트 기간이 아님. 
        ERR_CASE( ERR_ECLIPSE_COLLECT_02,   kPacket.m_nOK, 2 );    // 고서 아이템이 존재 하지 않음. 
        ERR_CASE( ERR_ECLIPSE_COLLECT_03,   kPacket.m_nOK, 3 );    // 이미 최종 보상 아이템을 받았음.
        ERR_CASE( ERR_ECLIPSE_COLLECT_04,   kPacket.m_nOK, 4 );    // DB 보상 아이템 넣기 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );  // 아직 처리중인 작업입니다.
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"고서 모으기 보상 처리중 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L"고서 모으기 이벤트 보상 처리 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    if( !IS_CORRECT( NET_OK ) )
        SEND_PACKET( EVENT_ECLIPSE_COLLECT_REWARD_ACK );
}

IMPL_ON_FUNC( EVENT_ECLIPSE_COLLECT_REWARD_ACK )
{
    KItemPtr pkItem;
    std::set<GCITEMID>::iterator sit;
    bool bComplete = true;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 선물 상자 조각 Item 정보 가져오기.
    for( sit = kPacket_.m_setItemList.begin(); sit != kPacket_.m_setItemList.end(); ++sit ) {
        pkItem = m_kInventory.FindDurationItemByItemID( *sit );
        if( pkItem ) {
            pkItem->m_nCount -= 1;
            kPacket_.m_vecUseItem.push_back( *pkItem );
        }
        pkItem.reset();
    }

    // 획득한 아이템을 넣어주는 코드.
    m_kInventory.AddItemList( kPacket_.m_vecItem );
    SetEclipseCollectComplete( bComplete ); // 고서 모으기 이벤트 보상 획득 정보 갱신.

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket_.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_ECLIPSE_COLLECT_01,   kPacket_.m_nOK, 1 );    // 고서 모으기 이벤트 기간이 아님. 
        ERR_CASE( ERR_ECLIPSE_COLLECT_02,   kPacket_.m_nOK, 2 );    // 고서 아이템이 존재 하지 않음. 
        ERR_CASE( ERR_ECLIPSE_COLLECT_03,   kPacket_.m_nOK, 3 );    // 이미 최종 보상 아이템을 받았음.
        ERR_CASE( ERR_ECLIPSE_COLLECT_04,   kPacket_.m_nOK, 4 );    // DB 보상 아이템 넣기 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket_.m_nOK, -98 );  // 아직 처리중인 작업입니다.
    default:
        kPacket_.m_nOK = 99;
        START_LOG( cerr, L"고서 모으기 보상 처리중 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_nOK ) << END_LOG;
    }

    _LOG_CONDITION( kPacket_.m_nOK == 0, clog, cwarn, L"고서 모으기 보상 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_setItemList.size() )
        << BUILD_LOG( kPacket_.m_vecUseItem.size() )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_ECLIPSE_COLLECT_REWARD_ACK );
}

_IMPL_ON_FUNC( EVENT_ECLIPSE_COLLECT_REWARD_ANSWER_NOT, bool )
{
    SetEclipseCollectComplete( kPacket_ ); // 고서 모으기 이벤트 보상 획득 정보 갱신.
    START_LOG( clog, L"클라이언트 고서 모으기 보상 획득 여부 정보 세팅" << m_strName )
        << BUILD_LOG( kPacket_ ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_MONSTER_CARD_MIX_REQ )
{
    __JIF( SiKMonsterCardMix()->IsEnable(), return );

    SET_ERROR( ERR_UNKNOWN );

    KEVENT_MONSTER_CARD_MIX_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_vecDecreaseCountItem.clear();
    kPacket.m_vecRewards.clear();

    kPacket_.m_vecMaterialItem.clear();
    kPacket_.m_vecRewards.clear();

    int nMaterialCountSum = 0;
    std::vector<int> vecMaterialGrade;
    vecMaterialGrade.clear();

    std::map<GCITEMUID,int>::iterator mit;
    for ( mit = kPacket_.m_mapMaterialItemCount.begin() ; mit != kPacket_.m_mapMaterialItemCount.end() ; ++mit ) {
        const GCITEMUID& ItemUID = mit->first;
        const int& nCount = mit->second;

        // 재료 아이템이 있는가?
        KItemPtr pkItem = m_kInventory.FindDurationItemByItemUID( ItemUID );
        if ( pkItem == NULL ) {
            SET_ERR_GOTO( ERR_MONSTER_CARE_MIX_01, END_PROC );
        }

        // 몬스터 카드인가?
        if ( false == SiKItemManager()->FindMonsterCardByItemID( pkItem->m_ItemID ) ) {
            SET_ERR_GOTO( ERR_MONSTER_CARE_MIX_02, END_PROC );
        }

        if ( nCount <= 0 ) {
            START_LOG( cwarn, L"선택한 재료 수량이 0개 이하다.. Login : " << GetName() )
                << BUILD_LOG( pkItem->m_ItemID )
                << BUILD_LOG( nCount )
                << BUILD_LOG( ItemUID ) << END_LOG;
            SET_ERR_GOTO( ERR_MONSTER_CARE_MIX_03, END_PROC );
        }

        // 재료 아이템 보유량이 충분한가?
        if ( pkItem->m_nCount < nCount ) {
            START_LOG( cwarn, L"재료 아이템이 이 0개 이하다.. Login : " << GetName() )
                << BUILD_LOG( pkItem->m_ItemID )
                << BUILD_LOG( nCount )
                << BUILD_LOG( ItemUID ) << END_LOG;
            SET_ERR_GOTO( ERR_MONSTER_CARE_MIX_03, END_PROC );
        }

        for ( int i = 0 ; i < nCount ; ++i ) {
            int nGrade = static_cast<int>(pkItem->m_cGradeID);
            vecMaterialGrade.push_back( nGrade );
        }

        nMaterialCountSum += nCount;
        kPacket_.m_vecMaterialItem.push_back( *pkItem );
    }

    // 재료 수량이 최소 개수를 이상인가?
    if ( nMaterialCountSum < SiKMonsterCardMix()->GetMinMaterialCount() ) {
        SET_ERR_GOTO( ERR_MONSTER_CARE_MIX_04, END_PROC );
    }

    // 재료 수량이 최대 개수를 이하인가?
    if ( nMaterialCountSum > SiKMonsterCardMix()->GetMaxMaterialCount() ) {
        SET_ERR_GOTO( ERR_MONSTER_CARE_MIX_05, END_PROC );
    }

    // 조합 결과 아이템 구하기
    if ( !vecMaterialGrade.empty() ) {
        KItem kRewardItem;
        kRewardItem.m_ItemID = 0;
        kRewardItem.m_nCount = 1;
        kRewardItem.m_nPeriod = -1;

        if ( SiKMonsterCardMix()->GetMixReward( vecMaterialGrade, kRewardItem.m_ItemID ) ) {
            kPacket_.m_vecRewards.push_back( kRewardItem );
        }
    }

    // 보상할 아이템이 있는가?
    if ( kPacket_.m_vecRewards.empty() ) {
        SET_ERR_GOTO( ERR_MONSTER_CARE_MIX_06, END_PROC );
    }

    // DB Update
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_MONSTER_CARD_MIX_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_MONSTER_CARE_MIX_01, kPacket.m_nOK, 1 ); // 재료 아이템이 인벤토리에 없음.
        ERR_CASE( ERR_MONSTER_CARE_MIX_02, kPacket.m_nOK, 2 ); // 재료 아이템이 몬스터 카드가 아님.
        ERR_CASE( ERR_MONSTER_CARE_MIX_03, kPacket.m_nOK, 3 ); // 재료 아이템 보유량이 부족함.
        ERR_CASE( ERR_MONSTER_CARE_MIX_04, kPacket.m_nOK, 4 ); // 재료 수량이 최소 개수를 미만.
        ERR_CASE( ERR_MONSTER_CARE_MIX_05, kPacket.m_nOK, 5 ); // 재료 수량이 최대 개수를 초과.
        ERR_CASE( ERR_MONSTER_CARE_MIX_06, kPacket.m_nOK, 6 ); // 보상할 아이템이 없음.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.

    default:
        START_LOG( cerr, L"몬스터카드 조합 요청 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"몬스터카드조합 요청 결과 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_mapMaterialItemCount.size() )
        << BUILD_LOG( kPacket_.m_vecMaterialItem.size() )
        << BUILD_LOG( kPacket_.m_vecRewards.size() )
        << BUILD_LOG( nMaterialCountSum )
        << BUILD_LOG( SiKMonsterCardMix()->GetMinMaterialCount() )
        << BUILD_LOG( SiKMonsterCardMix()->GetMaxMaterialCount() )
        << BUILD_LOG( vecMaterialGrade.size() )
        << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_MONSTER_CARD_MIX_ACK );
    }
}

IMPL_ON_FUNC( EVENT_MONSTER_CARD_MIX_ACK )
{
    __JIF( SiKMonsterCardMix()->IsEnable(), return );

    std::vector<KItem>::iterator vit;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( false == IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 사용한 재료아이템 감소
    for ( vit = kPacket_.m_vecDecreaseCountItem.begin() ; vit != kPacket_.m_vecDecreaseCountItem.end() ; ++vit ) {
        KItemPtr pkItem = m_kInventory.FindDurationItemByItemUID( vit->m_ItemUID );
        if ( pkItem ) {
            pkItem->m_nCount = vit->m_nCount;
            pkItem->m_nInitCount = vit->m_nInitCount;

            // 수량이 0이면 삭제 (DB에선 이미 삭제되었음)
            if ( pkItem->m_nCount == 0 ) {
                LIF( m_kInventory.RemoveItem( pkItem->m_ItemUID ) );
            }

            START_LOG( clog, L"몬스터카드 조합재료 수량감소 적용.. Login : " << GetName() )
                << BUILD_LOG( vit->m_ItemUID )
                << BUILD_LOG( vit->m_ItemID )
                << BUILD_LOG( vit->m_nCount ) << END_LOG;
        }
    }

    // 획득한 보상 아이템 인벤토리에 넣기
    if ( !kPacket_.m_vecRewards.empty() ) {
        m_kInventory.AddItemList( kPacket_.m_vecRewards );
    }

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_MONSTER_CARE_MIX_07, kPacket_.m_nOK, 7 ); // 몬스터카드 조합 DB처리 실패.
    default:
        kPacket_.m_nOK = 99;
        START_LOG( cerr, L"몬스터 카드 조합 중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( GetName() ) << END_LOG;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"몬스터 카드 조합 결과 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecRewards.size() )
        << BUILD_LOG( kPacket_.m_vecDecreaseCountItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_MONSTER_CARD_MIX_ACK );
}

_IMPL_ON_FUNC( EVENT_USE_EXP_POTION_REQ, KExpPotionReq )
{
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));

    SET_ERROR( ERR_UNKNOWN );

    KEVENT_USE_EXP_POTION_ACK kPacket;
    kPacket.m_kItem = kPacket_.m_kItem;
    kPacket.m_cCharType = kPacket_.m_cCharType;

    KExpPotionInfo kExpPotionInfo;
    kExpPotionInfo.m_fGrowRate = 0.0f;
    kExpPotionInfo.m_nGrowValue = 0;
    KItemPtr pkItem;
    KCharacterInfo* pkCharInfo = NULL;

    __int64 biExp = 0;
    __int64 biTotalExp = 0;
    DWORD dwLv = 0;

    std::map<DWORD, std::pair<int,DWORD>> mapLevelUpUser;
    KDB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ kDBPacket;

    if ( GetCurrentCharType() != kPacket_.m_cCharType ) {
        START_LOG( cerr, L"서버-클라이언트의 현재 캐릭터가 다르다, User: " << GetName() )
            << END_LOG;
        SET_ERR_GOTO( ERR_USE_EXP_POTION_05, END_PROC );
    }

    // 캐릭터 정보를 얻는다
    pkCharInfo = GetCharacterInfo( kPacket_.m_cCharType );
    if ( NULL == pkCharInfo ) {
        SET_ERR_GOTO( ERR_USE_EXP_POTION_05, END_PROC );
    }

    // 해당 아이템이 내 인벤에 존재하는지.
    pkItem = m_kInventory.FindItem( kPacket_.m_kItem.m_dwID, kPacket_.m_kItem.m_dwUID );
    if( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_USE_EXP_POTION_01, END_PROC );
    }

    // 해당 아이템이 경험치 포션 아이템인지.
    if( false == SiKGCHelper()->GetExpPotionInfo( kPacket_.m_kItem.m_dwID, kExpPotionInfo ) )
    {
        SET_ERR_GOTO( ERR_USE_EXP_POTION_02, END_PROC );
    }

    // 해당 아이템의 수량이 있는지.
    if( pkItem->m_nCount <= 0 )
    {
        SET_ERR_GOTO( ERR_USE_EXP_POTION_03, END_PROC );
    }

    // 해당 아이템 유효 사용 레벨이 맞는지 ( min~max )
    if ( false == ( kExpPotionInfo.m_dwValidMinLev <= pkCharInfo->m_dwLevel ) && ( pkCharInfo->m_dwLevel <= kExpPotionInfo.m_dwValidMaxLev) ) {
        SET_ERR_GOTO( ERR_USE_EXP_POTION_04, END_PROC );
    }

    // 만렙 체크
    if ( pkCharInfo->m_dwLevel >= SiKResultManager()->GetMaxLevel() ) {
        SET_ERR_GOTO( ERR_USE_EXP_POTION_04, END_PROC );
    }

    __int64 biMaxExp = SiKResultManager()->GetExpByLv( SiKResultManager()->GetMaxLevel() ); // 만렙 최대 경험치 얻어 옴
    __int64 biCurLvMaxExp = SiKResultManager()->GetExpByLv( pkCharInfo->m_dwLevel - 1 ); // 현재 레벨 시작 경험치 값
    __int64 biNextLvMaxExp = SiKResultManager()->GetExpByLv( pkCharInfo->m_dwLevel ); // 다음 레벨 시작 경험치 값
    __int64 biLvMaxExp = biNextLvMaxExp - biCurLvMaxExp;
    DWORD dwOldLevel = pkCharInfo->m_dwLevel; // 포션 적용 전 레벨 저장
    __int64 biExpPotionIncreaseValue = 0; // 캐릭터 경험치에 더해 줄 수치

    // PercentageGrow 일 경우 레벨에 따른 최대 경험치에 대한 GrowRate %로 경험치를 증가시킨다.
    if ( true == kExpPotionInfo.m_bPercentageGrow ) {
        __int64 biGrowRate = static_cast<__int64>( kExpPotionInfo.m_fGrowRate );
        biExpPotionIncreaseValue = std::max<__int64>( 0, biLvMaxExp / 100 * biGrowRate );
    }
    else {
        biExpPotionIncreaseValue = std::max<__int64>( 0, kExpPotionInfo.m_nGrowValue );
    }

    // 레벨 넘었을 때 적용 시킬지
    if ( false == kExpPotionInfo.m_bExceedEXP )
        biExpPotionIncreaseValue = std::min<__int64>( biExpPotionIncreaseValue, biNextLvMaxExp );

    pkCharInfo->m_biExp += static_cast<__int64>( biExpPotionIncreaseValue );
    pkCharInfo->m_biTotalExp += static_cast<__int64>( biExpPotionIncreaseValue );

    // 캐릭터에 적용.
    // EXP
    pkCharInfo->m_biExp = std::min<__int64>( biMaxExp, pkCharInfo->m_biExp );
    pkCharInfo->m_biExp = std::max<__int64>( pkCharInfo->m_biExp, USER_MIN_EXP );
    // TotalExp( 누적 경험치 )
    pkCharInfo->m_biTotalExp = std::min<__int64>( _I64_MAX, pkCharInfo->m_biTotalExp );
    pkCharInfo->m_biTotalExp = std::max<__int64>( pkCharInfo->m_biTotalExp, USER_MIN_EXP );
    // 레벨 계산.
    pkCharInfo->m_dwLevel = SiKResultManager()->Exp_2_Level( pkCharInfo->m_biExp );

    // 캐릭터의 Levelup을 달성했을 경우에 체크하자.
    if( dwOldLevel < pkCharInfo->m_dwLevel ) {
        // SPX 계산
        SiKSkillManager()->GetSkillPointInfo( kPacket_.m_cCharType, pkCharInfo->m_cPromotion, pkCharInfo->m_dwLevel, pkCharInfo->m_kSkillInfo.m_nLvSPPoint );
        // 상하이 드랍 체크
        LevelUpDrop( GetThisPtr(), kPacket_.m_cCharType, static_cast<char>(dwOldLevel), static_cast<char>(pkCharInfo->m_dwLevel) );

        KEVENT_SHANGHAI_DROP_REQ kShDrop;
        kShDrop.m_cCharType = pkCharInfo->m_cCharType;
        CheckLevelUpDrop( kShDrop );

        // 레벨업시 설문정보 주는 부분 있는지 확인.
        if ( SiKSurvey()->IsRun() ) {   // 이벤트 기간일때만 설문 확인 및 전달
            CheckLevelUpSurvey( (int)pkCharInfo->m_cCharType, static_cast< int >( pkCharInfo->m_dwLevel ) );
        }
        // 전직 미션 가이드 정보 체크
        CheckJobMissionGuide( GetUID(), pkCharInfo->m_cCharType, static_cast< int >( pkCharInfo->m_dwLevel ) );

        // 봉인 캐릭터 정보 갱신
        UpdateUserSealedCharInfo( GetThisPtr(), pkCharInfo->m_cCharType, static_cast< int >( pkCharInfo->m_dwLevel ) );

        // 유저 레벨업 정보 저장.
        SetUserLevelUpInfo( GetUID(), (int)kPacket_.m_cCharType, pkCharInfo->m_dwLevel, mapLevelUpUser );
        // 통계용
        UpdateCharLvPlayTime( static_cast<int>(kPacket_.m_cCharType), dwOldLevel );

        if ( m_pkRoom ) {
            KInDoorUserInfo* pKInDoorUserInfo = m_pkRoom->FindInDoorUserInfo( m_strName );
            if ( pKInDoorUserInfo ) {
                pKInDoorUserInfo->m_dwLevel = pkCharInfo->m_dwLevel;
                m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL_EX_ME, m_strName, KUserEvent::EVENT_CHANGE_INDOOR_USERINFO_BROAD, *pKInDoorUserInfo, true );
            }
        }

        if( SiKGCHelper()->IsCharLevelEvent() ) { // 레벨업을 했을 경우에..,
            if( SiKGCHelper()->GetCharLevelRewardItem( pkCharInfo->m_cCharType, pkCharInfo->m_dwLevel, m_setCharLevelEvent, kDBPacket.m_vecRewardInfo ) ) {
                kDBPacket.m_setCharInfo.insert( pkCharInfo->m_cCharType );
                QUEUEING_EVENT_TO_DB( DB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ, kDBPacket );
            }
        }
    }

    // 수량 감소
    pkItem->m_nCount = std::max<int>(0, pkItem->m_nCount -1 );

    // 클라이언트에 보내 줄 정보들
    kPacket.m_biExp = pkCharInfo->m_biExp;
    kPacket.m_biTotalExp = pkCharInfo->m_biTotalExp;
    kPacket.m_dwLv = pkCharInfo->m_dwLevel;
    kPacket.m_nDuration = pkItem->m_nCount;
    kPacket.m_dwUserUID = GetUID();
    kPacket.m_kSkillInfo = pkCharInfo->m_kSkillInfo;

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_USE_EXP_POTION_01, kPacket.m_nOK, 1 ); // 아이템 수량이 없음
        ERR_CASE( ERR_USE_EXP_POTION_02, kPacket.m_nOK, 2 ); // 경험치 포션이 아님
        ERR_CASE( ERR_USE_EXP_POTION_03, kPacket.m_nOK, 3 ); // 수량이 모자람
        ERR_CASE( ERR_USE_EXP_POTION_04, kPacket.m_nOK, 4 ); // 사용 가능 레벨 아님
        ERR_CASE( ERR_USE_EXP_POTION_05, kPacket.m_nOK, 5 ); // 캐릭터가 없음

    default:
        START_LOG( cerr, L"경험치 포션 사용 요청 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    SEND_PACKET( EVENT_USE_EXP_POTION_ACK );

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"경험치 포션 사용 요청 결과 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kItem.m_dwUID )
        << BUILD_LOG( kPacket.m_kItem.m_dwID )
        << BUILD_LOG( kPacket.m_cCharType )
        << BUILD_LOG( kPacket.m_biExp )
        << BUILD_LOG( kPacket.m_biTotalExp )
        << BUILD_LOG( kPacket.m_dwLv )
        << BUILD_LOG( kPacket.m_dwUserUID )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_EXP_POTION_LIST_REQ )
{
    KEVENT_EXP_POTION_LIST_ACK kPacket;

    if ( false == SiKGCHelper()->GetExpPotionInfo( kPacket.m_vecItemID ) )
        START_LOG( cwarn, L"경험치 포션 리스트가 비었음" << kPacket.m_vecItemID.size() ) << END_LOG;

    SEND_PACKET( EVENT_EXP_POTION_LIST_ACK );
}

_IMPL_ON_FUNC( EVENT_GACHA_NOTICE_POPUP_INFO_NOT, bool )
{
    START_LOG( clog, L"가차 로그 봤는지 ? " << kPacket_ ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_GACHA_NOTICE_POPUP_INFO_NOT );    
}

IMPL_ON_FUNC_NOPARAM( EVENT_UPDATE_GACHA_POPUP_INFO )
{
    QUEUEING_ID_TO_DB( EVENT_UPDATE_GACHA_POPUP_INFO );
}

IMPL_ON_FUNC_NOPARAM( EVENT_MONSTER_CARD_MIX_INFO_REQ )
{
    KEVENT_MONSTER_CARD_MIX_INFO_ACK kPacket;

    SET_ERROR( ERR_UNKNOWN );

    if ( false == SiKMonsterCardMix()->IsEnable() ) {
        SET_ERR_GOTO( ERR_MONSTER_CARD_MIX_INFO_02, END_PROC );
    }

    SiKMonsterCardMix()->GetMonsterCardInfoForPacket( kPacket );

    if ( kPacket.m_mapGradePoint.empty() ||
        kPacket.m_mapPointReward.empty() ||
        kPacket.m_mapRewardGroup.empty() ) {
        SET_ERR_GOTO( ERR_MONSTER_CARD_MIX_INFO_01, END_PROC );
    }

    SET_ERROR( NET_OK );
END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_MONSTER_CARD_MIX_INFO_01, kPacket.m_nOK, 1 ); // 아이템 수량이 없음
        ERR_CASE( ERR_MONSTER_CARD_MIX_INFO_02, kPacket.m_nOK, 2 ); // 이벤트 기간이 아님
    default:
        START_LOG( cerr, L"몬스터 카드 합성 정보 요청 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    SEND_PACKET( EVENT_MONSTER_CARD_MIX_INFO_ACK );

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"몬스터 카드 정보 요청 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket.m_mapGradePoint.size() )
        << BUILD_LOG( kPacket.m_mapPointReward.size() )
        << BUILD_LOG( kPacket.m_mapRewardGroup.size() )
        << END_LOG;
}

// 송크란 이벤트 시작
IMPL_ON_FUNC( DB_EVENT_SONGKRAN_USER_INFO_ACK ) // verify account에서 호출
{
    //DB에서 받은 데이터 m_kSongkran에 초기화
    m_KSongkranUserData.SetSongkranUserEventData( kPacket_.m_dwGrade,
        kPacket_.m_dwScore,
        kPacket_.m_nAccTime,
        kPacket_.m_dwWaterBombDuration,
        kPacket_.m_dwSuppliedWaterBombDuration );

    kPacket_.m_nOK = 0;

    // 금일 지급 종료인지 여부 판단
    m_KSongkranUserData.UpdateFinishToday();

    // 처음 접속일 때 물풍선 채워준다.
    // 추가 : 접속 유지 초기화 여부 체크 ( 재접속을 하지 않아도 초기화 해주고 있기 때문. 첫접속 지급도 함 )
    if ( ( true == kPacket_.m_bResetEnable ) || ( m_iSessionInfo == SI_NEW_ACCOUNT ) ) {
        KEVENT_SONGKRAN_WATERBOMB_SUPPLY_NOT kPacket;
        DWORD dwDuration = 0;

        dwDuration = m_KSongkranUserData.GetUserWaterBombDuration();
        dwDuration += SiKSongkran()->GetDailySupplyDuration();
        dwDuration = std::min<DWORD> ( ULONG_MAX, dwDuration );
        m_KSongkranUserData.SetUserWaterBombDuration( dwDuration );
        m_KSongkranUserData.SetUserInitWaterBombDuration( dwDuration );

        m_KSongkranUserData.SetSuppliedWaterBombDuration( 0 ); // 금일 처음 접속이면 지급 수량 초기화
        m_KSongkranUserData.ResetAccTime();
        m_KSongkranUserData.SetFinishToday( false );

        kPacket.m_nOK = KEVENT_SONGKRAN_WATERBOMB_SUPPLY_NOT::FIRST_CONNECTION_SUPPLY;
        kPacket.m_dwWaterBombDuration = dwDuration;

        SEND_PACKET( EVENT_SONGKRAN_WATERBOMB_SUPPLY_NOT );
    }
}

IMPL_ON_FUNC_NOPARAM( EVENT_SONGKRAN_SCRIPT_INFO_REQ ) // 최초 접속 시 클라에서 호출
{
    KEVENT_SONGKRAN_SCRIPT_INFO_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    // 이벤트 중인지 검사
    if ( !SiKSongkran()->IsRun() ) { 
        SET_ERR_GOTO( ERR_SONGKRAN_01, END_PROC );
    }

    SiKSongkran()->GetSongkranInfo( kPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );   // 성공
        ERR_CASE( ERR_SONGKRAN_01,      kPacket.m_nOK, 1 );   // 송크란 이벤트 기간이 아님.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
     default:
         START_LOG( cerr, L"송크란 이벤트 유저 정보 요청 중 알 수 없는 오류 Name : " << m_strName )
             << BUILD_LOG( NetError::GetLastNetError() )
             << BUILD_LOG( NetError::GetLastNetErrMsg() )
             << END_LOG;
         kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"송크란 이벤트 스크립트 정보 전달 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_fPlayerCharMoveSpeed )
        << BUILD_LOG( kPacket.m_fWaterBombFlyingSpeed )
        << BUILD_LOG( kPacket.m_fWaterBombRange )
        << BUILD_LOG( kPacket.m_mapTargetInfoList.size() )
        << BUILD_LOG( kPacket.m_mapGradeRewardList.size() )
        << END_LOG;

    SEND_PACKET( EVENT_SONGKRAN_SCRIPT_INFO_ACK );
    
}

IMPL_ON_FUNC_NOPARAM( EVENT_SONGKRAN_USER_INFO_REQ )
{
    KEVENT_SONGKRAN_USER_INFO_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    // 송크란 이벤트 기간 검사
    if ( !SiKSongkran()->IsRun() ) { 
        SET_ERR_GOTO( ERR_SONGKRAN_01, END_PROC );
    }

    // DB데이터 동기화 되어 있는지
    if ( false == m_KSongkranUserData.IsLoaded() ) {
        SET_ERR_GOTO( ERR_SONGKRAN_02, END_PROC );
    }

    m_KSongkranUserData.GetSongkranUserEventDate( kPacket );

    SET_ERROR( NET_OK );
    
END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );   // 성공
        ERR_CASE( ERR_SONGKRAN_01,      kPacket.m_nOK, 1 );   // 송크란 이벤트 기간이 아님.
        ERR_CASE( ERR_SONGKRAN_02,      kPacket.m_nOK, 2 );   // DB에서 데이터 동기화 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
     default:
         START_LOG( cerr, L"송크란 이벤트 유저 정보 요청 중 알 수 없는 오류 Name : " << m_strName )
             << BUILD_LOG( NetError::GetLastNetError() )
             << BUILD_LOG( NetError::GetLastNetErrMsg() )
             << END_LOG;
         kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"송크란 이벤트 유저 정보 전달 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwScore )
        << BUILD_LOG( kPacket.m_dwGrade )
        << BUILD_LOG( kPacket.m_dwWaterBombDuration )
        << BUILD_LOG( kPacket.m_nAccTime )
        << END_LOG;

    SEND_PACKET( EVENT_SONGKRAN_USER_INFO_ACK );
}

_IMPL_ON_FUNC( EVENT_SONGKRAN_WATERBOMB_USE_REQ, int )
{
    KEVENT_SONGKRAN_WATERBOMB_USE_ACK kPacket;
    kPacket.m_nOK = -99;
    DWORD dwScore = 0;
    DWORD dwScoreDiff = kPacket_;
    DWORD dwGrade = m_KSongkranUserData.GetUserGrade();
    DWORD dwMaxGrade = SiKSongkran()->GetMaxGrade();
    DWORD dwDuration = m_KSongkranUserData.GetUserWaterBombDuration();
    DWORD dwCurGradeEndScore = 0;
    KDB_EVENT_SONGKRAN_WATERBOMB_USE_REQ kDBPacket;

    SET_ERROR( ERR_UNKNOWN );

    // 이벤트 기간인지?
    if ( !SiKSongkran()->IsRun() ) { 
        SET_ERR_GOTO( ERR_SONGKRAN_01, END_PROC );
    }

    if ( 0 >= dwDuration ) {
        SET_ERR_GOTO( ERR_SONGKRAN_03, END_PROC);
    }

    DWORD dwOneTimeMaxScore = SiKSongkran()->GetOneTimeMaxScore();
    if ( dwOneTimeMaxScore < dwScoreDiff ) { // 해킹 패킷의 경우 수치 0으로 적용
        kPacket.m_dwScore = 0;
        SET_ERR_GOTO( ERR_SONGKRAN_08, END_PROC );
    }

    // 현재 등급의 최대 스코어 얻는다
    SiKSongkran()->GetCurGradeEndScore( dwGrade, dwCurGradeEndScore );

    dwScore = m_KSongkranUserData.GetUserScore();

    // dwMaxGrade를 넘어서고 나면 최종 보상 획득 후에 스코어만 오르는 상태가 됨
    if ( dwMaxGrade >= dwGrade ) {
        // 현재 유저의 점수가 현재 등급의 최대치 이상일 경우
        if ( dwCurGradeEndScore <= dwScore ) {
            // 보상 받아야 진행 할 수 있다!
            kPacket.m_dwScore = dwScore;
            SET_ERR_GOTO( ERR_SONGKRAN_06, END_PROC );
        }
    }

    kDBPacket.m_dwScoreDiff = dwScoreDiff;
    kDBPacket.m_dwSuppliedDuration = m_KSongkranUserData.GetSuppliedWaterBombDuration();
    kDBPacket.m_nAccTime = m_KSongkranUserData.GetAccTime();

    SET_ERROR( NET_OK );
    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_SONGKRAN_WATERBOMB_USE_REQ, kDBPacket );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );   // 성공
        ERR_CASE( ERR_SONGKRAN_01,      kPacket.m_nOK, 1 );   // 송크란 이벤트 기간이 아님.
        ERR_CASE( ERR_SONGKRAN_02,      kPacket.m_nOK, 2 );   // DB에서 데이터 이미 가져왔음.
        ERR_CASE( ERR_SONGKRAN_03,      kPacket.m_nOK, 3 );   // 물풍선이 인벤토리에 없음.
        ERR_CASE( ERR_SONGKRAN_06,      kPacket.m_nOK, 6 );   // 보상을 받아야 진행 가능
        ERR_CASE( ERR_SONGKRAN_08,      kPacket.m_nOK, 8 );  // 해킹 패킷 처리
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
     default:
         START_LOG( cerr, L"송크란 이벤트 물풍 사용 요청 중 알 수 없는 오류 Name : " << m_strName )
             << BUILD_LOG( NetError::GetLastNetError() )
             << BUILD_LOG( NetError::GetLastNetErrMsg() ) 
             << END_LOG;
         kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"송크란 이벤트 물풍 사용 요청 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
    SEND_PACKET( EVENT_SONGKRAN_WATERBOMB_USE_ACK );
    }
}

IMPL_ON_FUNC( EVENT_SONGKRAN_WATERBOMB_USE_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    // 물풍 사용 기록 적용.
    m_KSongkranUserData.UseWaterBomb( kPacket_.m_dwScore );

    kPacket_.dwWaterBombDuration = m_KSongkranUserData.GetUserWaterBombDuration();
    kPacket_.m_nAccTime = m_KSongkranUserData.GetAccTime();
    kPacket_.m_dwScore = m_KSongkranUserData.GetUserScore();

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );   // 성공
        ERR_CASE( ERR_SONGKRAN_01,      kPacket_.m_nOK, 1 );   // 송크란 이벤트 기간이 아님.
        ERR_CASE( ERR_SONGKRAN_02,      kPacket_.m_nOK, 2 );   // DB에서 데이터 이미 가져왔음.
        ERR_CASE( ERR_SONGKRAN_03,      kPacket_.m_nOK, 3 );   // 물풍선이 인벤토리에 없음.
        ERR_CASE( ERR_SONGKRAN_06,      kPacket_.m_nOK, 6 );   // 보상을 받아야 진행 가능
        ERR_CASE( ERR_SONGKRAN_08,      kPacket_.m_nOK, 8 );   // 해킹 패킷 처리
        ERR_CASE( ERR_SONGKRAN_13,      kPacket_.m_nOK, 13 );  // 풍선 아이템 DB 갯수 갱신 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 아직 처리중인 작업입니다.
     default:
         START_LOG( cerr, L"송크란 이벤트 물풍 사용 응답중 알 수 없는 오류 Name : " << m_strName )
             << BUILD_LOG( NetError::GetLastNetError() )
             << BUILD_LOG( NetError::GetLastNetErrMsg() ) 
             << END_LOG;
         kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"송크란 이벤트 물풍 사용 요청 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.dwWaterBombDuration )
        << BUILD_LOG( kPacket_.m_nAccTime )
        << BUILD_LOG( kPacket_.m_dwScore )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SONGKRAN_WATERBOMB_USE_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_SONGKRAN_GRADE_REWARD_REQ )
{
    KEVENT_SONGKRAN_GRADE_REWARD_ACK kPacket;
    KDB_EVENT_SONGKRAN_GRADE_REWARD_REQ kDBPacket;
    kPacket.m_nOK = -99;
    DWORD dwScore = 0;
    DWORD dwCurrentGrade = 1;
    DWORD dwTotalMaxGrade = SiKSongkran()->GetMaxGrade();
    SET_ERROR( ERR_UNKNOWN );

    // 송크란 이벤트 기간?
    if ( !SiKSongkran()->IsRun() ) {
        SET_ERR_GOTO( ERR_SONGKRAN_01, END_PROC );
    }

    kDBPacket.m_dwGrade = m_KSongkranUserData.GetUserGrade();
    // 최종등급 이상의 등급인 상태에서 보상을 요구한다. 비정상 패킷
    if ( dwTotalMaxGrade < kDBPacket.m_dwGrade ) {
        SET_ERR_GOTO( ERR_SONGKRAN_07, END_PROC );
    }

    // 현재 나의 포인트에서 해당 보상을 받을수 있는가에 대한 체크
    SiKSongkran()->GetUserScoreGrade( m_KSongkranUserData.GetUserScore(), dwCurrentGrade );
    if( dwCurrentGrade < m_KSongkranUserData.GetUserGrade() ) {
        SET_ERR_GOTO( ERR_SONGKRAN_09, END_PROC );
    }

    // 등급 보상 정보 가져오기 실패
    if( false == SiKSongkran()->GetGradeRewardList( kDBPacket.m_dwGrade, kDBPacket.m_vecReward ) ) {
        SET_ERR_GOTO( ERR_SONGKRAN_04, END_PROC );
    }

    kDBPacket.m_dwSuppliedDuration = m_KSongkranUserData.GetSuppliedWaterBombDuration();
    kDBPacket.m_nAccTime = m_KSongkranUserData.GetAccTime();

    SET_ERROR( NET_OK );
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_SONGKRAN_GRADE_REWARD_REQ, kDBPacket );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );   // 성공
        ERR_CASE( ERR_SONGKRAN_01,      kPacket.m_nOK, 1 );   // 송크란 이벤트 기간이 아님.
        ERR_CASE( ERR_SONGKRAN_02,      kPacket.m_nOK, 2 );   // DB에서 데이터 이미 가져왔음.
        ERR_CASE( ERR_SONGKRAN_04,      kPacket.m_nOK, 4 );   // 송크란 이벤트 설정 가져오기 실패.
        ERR_CASE( ERR_SONGKRAN_07,      kPacket.m_nOK, 7 );  // 최종등급 이상을 요구하는 비정상 패킷.
        ERR_CASE( ERR_SONGKRAN_09,      kPacket.m_nOK, 9 );  // 현재 나의 스코어는 등급 보상을 받을수 없다.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 등급 보상 가져오기 실패.
    default:
        START_LOG( cerr, L"송크란 이벤트 등급 보상 요청 중 알 수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"송크란 이벤트 등급 보상 받기 요청 중  : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kDBPacket.m_vecReward.size() )
        << BUILD_LOG( kDBPacket.m_dwGrade )
        << BUILD_LOG( kPacket.m_nOK )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_CHARISMAS_EVENT_GRADE_REWARD_ACK );
    }
}

IMPL_ON_FUNC( EVENT_SONGKRAN_GRADE_REWARD_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }
    // 보상 등급 증가 후 아이템 넣어준다
    m_KSongkranUserData.IncreaseGrade();
    m_kInventory.AddItemList( kPacket_.m_vecReward );

END_PROC:
    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"송크란 이벤트 등급보상 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecReward.size() )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SONGKRAN_GRADE_REWARD_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_SONGKRAN_WATERBOMB_EXCHANGEITEM_USE_REQ )
{
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_SONGKRAN_WATERBOMB_EXCHANGEITEM_USE_ACK kPacket;
    KItemPtr pkItem;
    KItem kWaterBombItem;
    DWORD dwItemDuration = 0;
    DWORD dwWaterBombDuration = m_KSongkranUserData.GetUserWaterBombDuration();

    // 이벤트 기간 체크
    if ( !SiKSongkran()->IsRun() ) {
        SET_ERR_GOTO( ERR_SONGKRAN_01, END_PROC );
    }

    pkItem = m_kInventory.FindDurationItemByItemID( SiKSongkran()->GetWaterBombItem() );
    if ( NULL == pkItem ) {
        SET_ERR_GOTO( ERR_SONGKRAN_03, END_PROC );
    }

    kWaterBombItem = *pkItem;

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_SONGKRAN_WATERBOMB_EXCHANGEITEM_USE_REQ, kWaterBombItem );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_SONGKRAN_01, kPacket.m_nOK, 1 ); // 송크란 기간 아님
        ERR_CASE( ERR_SONGKRAN_03, kPacket.m_nOK, 3 ); // 교환 아이템이 인벤에 없음
        ERR_CASE( ERR_SONGKRAN_11, kPacket.m_nOK, 11 ); // 풍선 교환 캐시 아이템 개수 체크 실패.
        ERR_CASE( ERR_SONGKRAN_12, kPacket.m_nOK, 12 ); // 풍선 교환 캐시 아이템 개수 동기화 실패.
        ERR_CASE( ERR_SONGKRAN_13, kPacket.m_nOK, 13 ); // 풍선 아이템 DB 갯수 갱신 실패.
        default: break;
    }

    _LOG_SUCCESS( IS_CORRECT( NET_OK ), L"송크란 물풍선 아이템 교환 요청: " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( dwItemDuration )
        << BUILD_LOG( dwWaterBombDuration )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_SONGKRAN_WATERBOMB_EXCHANGEITEM_USE_ACK );
    }

}

IMPL_ON_FUNC( EVENT_SONGKRAN_WATERBOMB_EXCHANGEITEM_USE_ACK )
{
    KItemPtr pkItem;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) && !IS_CORRECT( ERR_SONGKRAN_11 ) && !IS_CORRECT( ERR_SONGKRAN_12 ) ) {
        goto END_PROC;
    }

    pkItem = m_kInventory.FindDurationItemByItemID( SiKSongkran()->GetWaterBombItem() );
    if ( NULL == pkItem ) {
        SET_ERR_GOTO( ERR_SONGKRAN_03, END_PROC );
    }
    // 아이템 갯수 동기화.
    pkItem->m_nCount = kPacket_.m_kUseItem.m_nCount;
    pkItem->m_nInitCount = kPacket_.m_kUseItem.m_nCount;

    DWORD dwWaterBombCnt = m_KSongkranUserData.GetUserWaterBombDuration();
    DWORD dwInitWaterBombCnt = m_KSongkranUserData.GetUserInitWaterBombDuration();
    dwWaterBombCnt += kPacket_.m_dwWaterBombDuration;
    dwInitWaterBombCnt += kPacket_.m_dwWaterBombDuration;

    m_KSongkranUserData.SetUserWaterBombDuration( dwWaterBombCnt );
    m_KSongkranUserData.SetUserInitWaterBombDuration( dwInitWaterBombCnt );

    kPacket_.m_vecUseItem.push_back( kPacket_.m_kUseItem );
    kPacket_.m_dwWaterBombDuration = dwWaterBombCnt;

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );           // 성공.
        ERR_CASE( ERR_SONGKRAN_01, kPacket_.m_nOK, 1 ); // 송크란 기간 아님
        ERR_CASE( ERR_SONGKRAN_03, kPacket_.m_nOK, 3 ); // 교환 아이템이 인벤에 없음
        ERR_CASE( ERR_SONGKRAN_11, kPacket_.m_nOK, 11 ); // 풍선 교환 캐시 아이템 개수 체크 실패.
        ERR_CASE( ERR_SONGKRAN_12, kPacket_.m_nOK, 12 ); // 풍선 교환 캐시 아이템 개수 동기화 실패.
        ERR_CASE( ERR_SONGKRAN_13, kPacket_.m_nOK, 13 ); // 풍선 아이템 DB 갯수 갱신 실패.
        default: break;
    }
    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"송크란 이벤트 등급보상 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwWaterBombDuration )
        << BUILD_LOG( kPacket_.m_vecUseItem.size() )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SONGKRAN_WATERBOMB_EXCHANGEITEM_USE_ACK );
}

_IMPL_ON_FUNC( DB_EVENT_SONGKRAN_USER_INFO_REQ, bool )
{
    START_LOG( clog, L"송크란 이벤트 스크립트 변경시 접속중인 유저 데이터 요청: " << m_strName )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    QUEUEING_EVENT_TO_DB( DB_EVENT_SONGKRAN_USER_INFO_REQ, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EVENT_ADVENTURE_DATA_REQ )
{
    KEVENT_ADVENTURE_DATA_ACK kPacket;
    int nInitValue = 0;

    SET_ERROR( ERR_UNKNOWN );

    if ( !SiKAdventure()->IsRun() ) {
        SET_ERR_GOTO( ERR_ADVENTURE_01, END_PROC );
    }

    SET_ERROR( NET_OK );

    QUEUEING_EVENT_TO_DB( DB_EVENT_ADVENTURE_DATA_REQ, nInitValue );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_ADVENTURE_01, kPacket.m_nOK, 1 ); // 이벤트 기간이 아님
    default:
        START_LOG( cerr, L"모험 이벤트정보 요청 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"모험 이벤트 정보 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket.m_mapCollectData.size() )
        << BUILD_LOG( kPacket.m_mapContinentData.size() )
        << BUILD_LOG( kPacket.m_mapContinentRewardInfo.size() )
        << BUILD_LOG( kPacket.m_kFinalRewardInfo.m_ItemID )
        << BUILD_LOGc( kPacket.m_kFinalRewardInfo.m_cGradeID )
        << BUILD_LOG( kPacket.m_kFinalRewardInfo.m_nDuration )
        << BUILD_LOG( kPacket.m_kFinalRewardInfo.m_nPeriod )
        << END_LOG;
    
    if ( !IS_CORRECT( NET_OK ) ) {
        SEND_PACKET( EVENT_ADVENTURE_CONTINENT_REWARD_ACK );
    }
}
IMPL_ON_FUNC( EVENT_ADVENTURE_DATA_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK) && !IS_CORRECT( ERR_ADVENTURE_16 ) ) {
        goto END_PROC;
    }

    // 유저 데이터 저장
    m_kAdventureUserData.SetAdventureUserData( kPacket_.m_mapCollectData, kPacket_.m_mapContinentData );

    // 대륙보상, 최종 보상 정보를 패킷에 넣기
    if ( !SiKAdventure()->GetAdventureData( kPacket_ ) ) {
        SET_ERR_GOTO( ERR_ADVENTURE_02, END_PROC );
    }

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_ADVENTURE_01, kPacket_.m_nOK, 1 ); // 이벤트 기간이 아님
        ERR_CASE( ERR_ADVENTURE_02, kPacket_.m_nOK, 2 ); // 모험 이벤트 보상 정보가 없음
        ERR_CASE( ERR_ADVENTURE_16, kPacket_.m_nOK, 16 ); // 접속시 모험 이벤트 정보 가져오기 성공!(접속시 전달할 경우 16번으로 설정함)
    default:
        START_LOG( cerr, L"모험 이벤트정보 요청 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    // 그리고 클라에게 패킷 전달
    SEND_RECEIVED_PACKET( EVENT_ADVENTURE_DATA_ACK );

    _LOG_CONDITION( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 16, clog, cerr, L"모험 이벤트 정보 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_mapCollectData.size() )
        << BUILD_LOG( kPacket_.m_mapContinentData.size() )
        << BUILD_LOG( kPacket_.m_mapContinentRewardInfo.size() )
        << BUILD_LOG( kPacket_.m_kFinalRewardInfo.m_ItemID )
        << BUILD_LOGc( kPacket_.m_kFinalRewardInfo.m_cGradeID )
        << BUILD_LOG( kPacket_.m_kFinalRewardInfo.m_nPeriod )
        << BUILD_LOG( kPacket_.m_kFinalRewardInfo.m_nDuration )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_ADVENTURE_CONTINENT_REWARD_REQ )
{
    KEVENT_ADVENTURE_CONTINENT_REWARD_ACK kPacket;
    KContinentRewardInfo kContinentRewardInfo;
    std::map< int, std::map< DWORD, int > >::iterator mitCollectData;

    SET_ERROR( ERR_UNKNOWN );

    // 이벤트 기간인지 확인하고
    if ( !SiKAdventure()->IsRun() ) {
        SET_ERR_GOTO( ERR_ADVENTURE_01, END_PROC );
    }

    // 요청한 대륙의 보상 아이템을 이미 받았는지 확인
    if ( m_kAdventureUserData.IsRecievedContinentReward( kPacket_.m_nContinentID ) ) {
        SET_ERR_GOTO( ERR_ADVENTURE_03, END_PROC );
    }

    // 해당 대륙의 던전 전리품을 다 모았는지 확인
    mitCollectData = m_kAdventureUserData.m_mapCollectData.find( kPacket_.m_nContinentID );
    if ( mitCollectData == m_kAdventureUserData.m_mapCollectData.end() ) {
        // 유저 데이터는 값이 없을 경우 항상 0을 채워서 유지하기 때문에
        // 여기서 오류로 걸릴일은 없을 것 같지만 혹시나 해서 넣어둠
        SET_ERR_GOTO( ERR_ADVENTURE_13, END_PROC );
    }

    if ( !SiKAdventure()->CheckContidionContinentReward( kPacket_.m_nContinentID, mitCollectData->second ) ) {
        SET_ERR_GOTO( ERR_ADVENTURE_13, END_PROC );
    }

    // 대륙 보상 리스트 가져와서
    if ( !SiKAdventure()->GetContinentRewardInfo( kPacket_.m_nContinentID, kContinentRewardInfo ) ) {
        SET_ERR_GOTO( ERR_ADVENTURE_04, END_PROC );
    }

    kPacket_.m_vecDropItemInfo = kContinentRewardInfo.m_vecContinentRewards;

    SET_ERROR( NET_OK );
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_ADVENTURE_CONTINENT_REWARD_REQ, kPacket_ );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_ADVENTURE_01, kPacket.m_nOK, 1 ); // 이벤트 기간이 아님
        ERR_CASE( ERR_ADVENTURE_03, kPacket.m_nOK, 3 ); // 요청한 대륙의 보상 아이템을 이미 받았음
        ERR_CASE( ERR_ADVENTURE_04, kPacket.m_nOK, 4 ); // 모험 이벤트 대륙 보상 정보가 없음
        ERR_CASE( ERR_ADVENTURE_05, kPacket.m_nOK, 5 ); // 요청한 대륙의 보상 아이템이 보상 리스트에 없음
        ERR_CASE( ERR_ADVENTURE_13, kPacket.m_nOK, 13 ); // 대륙 보상을 받을 조건이 아님

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"모험 이벤트 대륙 보상 정보 요청 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"모험 이벤트 대륙 보상 정보 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_nContinentID )
        << BUILD_LOG( kPacket_.m_vecDropItemInfo.size() )
        << END_LOG;
    
    if ( !IS_CORRECT( NET_OK ) ) {
        SEND_PACKET( EVENT_ADVENTURE_CONTINENT_REWARD_ACK );
    }
}

IMPL_ON_FUNC( EVENT_ADVENTURE_CONTINENT_REWARD_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 받은 대륙 보상 아이템을 이벤에 넣어주고
    m_kInventory.AddItemList( kPacket_.m_vecReward );

    // 유저 데이터에 결과를 반영 해 준다
    // 대륙 보상 정보 기록
    m_kAdventureUserData.SetRecieveContinentReward( kPacket_.m_nContinentID, true );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_ADVENTURE_01, kPacket_.m_nOK, 1 ); // 이벤트 기간이 아님
        ERR_CASE( ERR_ADVENTURE_02, kPacket_.m_nOK, 2 ); // 모험 이벤트 보상 정보가 없음
        ERR_CASE( ERR_ADVENTURE_03, kPacket_.m_nOK, 3 ); // 요청한 대륙의 보상 아이템을 이미 받았음
        ERR_CASE( ERR_ADVENTURE_04, kPacket_.m_nOK, 4 ); // 모험 이벤트 대륙 보상 정보가 없음
        ERR_CASE( ERR_ADVENTURE_05, kPacket_.m_nOK, 5 ); // 요청한 대륙의 보상 아이템이 보상 리스트에 없음
        ERR_CASE( ERR_ADVENTURE_06, kPacket_.m_nOK, 6 ); // 대륙 보상 받기 DB 실패
        ERR_CASE( ERR_ADVENTURE_07, kPacket_.m_nOK, 7 ); // 대륙 보상 받기 DB 실패2

    default:
        START_LOG( cerr, L"모험 이벤트 대륙 보상 정보 요청 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nContinentID )
        << BUILD_LOG( kPacket_.m_vecReward.size() )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_ADVENTURE_CONTINENT_REWARD_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_ADVENTURE_FINAL_REWARD_REQ )
{
    KDB_EVENT_ADVENTURE_FINAL_REWARD_REQ kDBPacket;
    KEVENT_ADVENTURE_FINAL_REWARD_ACK kPacket;
    KDropItemInfo kFinalRewardInfo;

    SET_ERROR( ERR_UNKNOWN );

    // 이벤트 기간인지 확인하고
    if ( !SiKAdventure()->IsRun() ) {
        SET_ERR_GOTO( ERR_ADVENTURE_01, END_PROC );
    }

    // 대륙 보상들을 전부 받았는지 확인
    if ( !SiKAdventure()->CheckContidionFinalReward( m_kAdventureUserData.m_mapContinentData ) ) {
        SET_ERR_GOTO( ERR_ADVENTURE_14, END_PROC );
    }

    // 해당 캐릭터의 최종 보상 리스트 가져와서
    if ( !SiKAdventure()->GetFinalRewardInfo( kFinalRewardInfo ) ) {
        SET_ERR_GOTO( ERR_ADVENTURE_09, END_PROC );
    }

    kDBPacket = kFinalRewardInfo;

    SET_ERROR( NET_OK );
    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_ADVENTURE_FINAL_REWARD_REQ, kDBPacket );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_ADVENTURE_01, kPacket.m_nOK, 1 ); // 이벤트 기간이 아님
        ERR_CASE( ERR_ADVENTURE_08, kPacket.m_nOK, 8 ); // 요청한 캐릭터의 보상 아이템을 이미 받았음
        ERR_CASE( ERR_ADVENTURE_09, kPacket.m_nOK, 9 ); // 요청한 캐릭터의 모험 이벤트 최종 보상 정보가 없음
        ERR_CASE( ERR_ADVENTURE_10, kPacket.m_nOK, 10 ); // 요청한 캐릭터의 보상 아이템이 보상 리스트에 없음
        ERR_CASE( ERR_ADVENTURE_14, kPacket.m_nOK, 14 ); // 최종 보상을 받을 조건이 아님

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"모험 이벤트 최종 보상 정보 요청 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"모험 이벤트 최종 보상 정보 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kDBPacket.m_ItemID )
        << BUILD_LOGc( kDBPacket.m_cGradeID )
        << BUILD_LOG( kDBPacket.m_nDuration )
        << BUILD_LOG( kDBPacket.m_nPeriod )
        << END_LOG;

    if ( !IS_CORRECT( NET_OK ) ) {
        SEND_PACKET( EVENT_ADVENTURE_FINAL_REWARD_ACK );
    }
}

IMPL_ON_FUNC( EVENT_ADVENTURE_FINAL_REWARD_ACK )
{
    KEVENT_ADVENTURE_DATA_ACK kAdventureData;

    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 받은 최종 보상 아이템을 인벤에 넣어주고
    m_kInventory.AddItemList( kPacket_.m_vecReward );

    // 유저 데이터에 결과를 반영 해 준다
    // 전리품과 대륙 보상 받은 정보 초기화
    SiKAdventure()->InitAdventureUserData( m_kAdventureUserData.m_mapCollectData, m_kAdventureUserData.m_mapContinentData );

    kAdventureData.m_nOK = 17;     // 최종 보상 받은 후 유저 데이터 전달시에는 오류코드 17번으로 전달한다
    kAdventureData.m_mapCollectData = m_kAdventureUserData.m_mapCollectData;
    kAdventureData.m_mapContinentData = m_kAdventureUserData.m_mapContinentData;

    // 대륙보상, 최종 보상 정보를 패킷에 넣기
    if ( !SiKAdventure()->GetAdventureData( kAdventureData ) ) {
        SET_ERR_GOTO( ERR_ADVENTURE_02, END_PROC );
    }

    kPacket_.m_kAdventureData = kAdventureData;

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_ADVENTURE_01, kPacket_.m_nOK, 1 ); // 이벤트 기간이 아님
        ERR_CASE( ERR_ADVENTURE_02, kPacket_.m_nOK, 2 ); // 모험 이벤트 보상 정보가 없음
        ERR_CASE( ERR_ADVENTURE_03, kPacket_.m_nOK, 3 ); // 요청한 대륙의 보상 아이템을 이미 받았음
        ERR_CASE( ERR_ADVENTURE_04, kPacket_.m_nOK, 4 ); // 모험 이벤트 대륙 보상 정보가 없음
        ERR_CASE( ERR_ADVENTURE_05, kPacket_.m_nOK, 5 ); // 요청한 대륙의 보상 아이템이 보상 리스트에 없음
        ERR_CASE( ERR_ADVENTURE_06, kPacket_.m_nOK, 6 ); // 대륙 보상 받기 DB 실패
        ERR_CASE( ERR_ADVENTURE_07, kPacket_.m_nOK, 7 ); // 대륙 보상 받기 DB 실패2
        ERR_CASE( ERR_ADVENTURE_11, kPacket_.m_nOK, 11 ); // 최종 보상 받기 DB 실패
        ERR_CASE( ERR_ADVENTURE_12, kPacket_.m_nOK, 12 ); // 최종 보상 받기 DB 실패2
        ERR_CASE( ERR_ADVENTURE_15, kPacket_.m_nOK, 15 ); // 모험 이벤트 미션 정보가 없음

    default:
        START_LOG( cerr, L"모험 이벤트 최종 보상 정보 요청 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecReward.size() )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_ADVENTURE_FINAL_REWARD_ACK );
}

IMPL_ON_FUNC(EVENT_PET_MAGIC_STONE_CHANGE_EQUIP_REQ)
{
    START_LOG(clog, L"Pet Glyph Change Equip Requirement")
        << BUILD_LOG(kPacket_.m_dwPetGlyphUID)
        << BUILD_LOG(kPacket_.m_iCharType)
        << BUILD_LOG(kPacket_.m_bEquip)
        << END_LOG;


    KItemPtr pItem = m_kInventory.FindItemByItemUID(kPacket_.m_dwPetGlyphUID);
    if (pItem == NULL)
    {
        KEVENT_PET_MAGIC_STONE_CHANGE_EQUIP_ACK kPacket;
        kPacket.m_nOK = 5;
        SEND_COMPRESS_PACKET(EVENT_PET_MAGIC_STONE_CHANGE_EQUIP_ACK, kPacket);
        return;
    }

    KDB_EVENT_PET_MAGIC_STONE_CHANGE_EQUIP_REQ kPacket;

    kPacket.m_dwPetGlyphUID = kPacket_.m_dwPetGlyphUID;
    kPacket.m_dwPetGlyphID = pItem->m_ItemID;
    kPacket.m_iCharType = kPacket_.m_iCharType;
    kPacket.m_bEquip = kPacket_.m_bEquip;

    QUEUEING_EVENT_TO_DB(DB_EVENT_PET_MAGIC_STONE_CHANGE_EQUIP_REQ, kPacket);
}

IMPL_ON_FUNC(EVENT_PET_MAGIC_STONE_CHANGE_EQUIP_ACK)
{
    if (kPacket_.m_nOK == 0)
    {
        std::map<char, KCharacterInfo>::iterator it = m_mapCharacterInfo.find(m_cCurrentCharacter);

        if (it != m_mapCharacterInfo.end())
        {
            it->second.m_vecPetGlyphCharData.clear();

            std::vector<KPetGlyphCharData>::iterator it2 = kPacket_.m_vecEquippedGlyphs.begin();
            for (; it2 != kPacket_.m_vecEquippedGlyphs.end(); ++it2)
            {
                it->second.m_vecPetGlyphCharData.push_back(*it2);
            }
            it->second.m_dwPetGlyphSize = it->second.m_vecPetGlyphCharData.size();
        }
    }

    switch (kPacket_.m_nOK)
    {
        ERR_CASE(NET_OK, kPacket_.m_nOK, 0);
        ERR_CASE(ERR_PET_GLYPH_01, kPacket_.m_nOK, 1);
        ERR_CASE(ERR_PET_GLYPH_02, kPacket_.m_nOK, 2);
        ERR_CASE(ERR_PET_GLYPH_03, kPacket_.m_nOK, 3);

    default:
        START_LOG(cerr, L"Failed to change equip status of Pet Glyph Name : " << GetName())
            << BUILD_LOG(NetError::GetLastNetError())
            << BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    LOG_SUCCESS(kPacket_.m_nOK == 0)
        << BUILD_LOG(NetError::GetLastNetErrMsg())
        << BUILD_LOG(kPacket_.m_nOK)
        << BUILD_LOG(kPacket_.m_vecEquippedGlyphs.size())
        << END_LOG;

    SEND_RECEIVED_PACKET(EVENT_PET_MAGIC_STONE_CHANGE_EQUIP_ACK);
}

IMPL_ON_FUNC(EVENT_BILLBOARD_REGISTRATION_REQ)
{
    START_LOG(cwarn, "BILLBOARD Receive Data")
        << BUILD_LOG(kPacket_.m_wstrNickName)
        << BUILD_LOG(kPacket_.m_dwColor)
        << BUILD_LOG(kPacket_.m_wstrMessage)
        << BUILD_LOG(kPacket_.m_dwDword2)
        << BUILD_LOG(kPacket_.m_dwDword3)
        << END_LOG;

    KEVENT_BILLBOARD_REGISTRATION_ACK kPacket;

    if (!SiKUserBillboardManager()->IsEnableBillboard())
    {
        kPacket.m_nOK = -1;
    }
    else
    {
        kPacket.m_nOK = 0;
        KItemPtr pkItem = m_kInventory.FindDurationItemByItemID(SiKUserBillboardManager()->GetBillboardItem());

        if (pkItem != NULL)
        {
            KDB_EVENT_BILLBOARD_CHAT_MSG_LOG_REQ kDBPacket;
            kDBPacket.m_wstrNickName = kPacket_.m_wstrNickName;
            kDBPacket.m_wstrMessage = kPacket_.m_wstrMessage;

            QUEUEING_EVENT_TO_DB(DB_EVENT_BILLBOARD_CHAT_MSG_LOG_REQ, kDBPacket);

            if (kPacket_.m_wstrMessage.length() > 50)
            {
                kPacket_.m_wstrMessage = kPacket_.m_wstrMessage.substr(0, 49);
            }

            pkItem->m_nCount--;
            kPacket.m_vecKItemList.push_back(*pkItem);
            kPacket.m_nWaitingMessages = SiKUserBillboardManager()->WaitingMessages(); // The number of messages without the new one
            SiKUserBillboardManager()->InsertBillboardData(kPacket_.m_wstrMessage, kPacket_.m_wstrNickName, kPacket_.m_dwColor);
        }
        else
        {
            kPacket.m_nOK = -2;
        }
    }

    SEND_PACKET(EVENT_BILLBOARD_REGISTRATION_ACK);
}

IMPL_ON_FUNC(EVENT_BILLBOARD_CHAT_MSG_NOT)
{
    SEND_RECEIVED_PACKET(EVENT_BILLBOARD_CHAT_MSG_NOT);
}