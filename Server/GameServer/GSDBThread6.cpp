#include <WinSock2.h>
#include "GSDBThread.h"
#include <dbg/dbg.hpp>
#include "GSSimLayer.h"
#include "User.h"
#include "GSDBLayer.h"
#include "EventType.h"
#include "support_User.h"
#include "FTPFactory.h"
#include "FTP/FTPClient.h"
#include "GuildManager.h"
#include "GCHelper.h"
#include "PresentBox.h"
#include "ItemManager.h"
#include "GambleBuyManager.h"
#include "ChannelTypeEvent.h"
#include "MissionManager.h"
#include "MissionInfo.h"
#include "Gawibawibo.h"
#include "GameServer.h"
#include "LoginOutStat.h"
#include "Socks.h"
#include "VipEventManager.h"
#include "MagicBox.h"
#include "RKTornado.h"
#include "Sphinx.h"
#include "Survey.h"
#include "StrengthManager.h"
#include "AgitHelper.h"
#include "IPAdvantage.h"
#include <boost/lexical_cast.hpp> // lexical_cast
#include "RecomHelper.h"
#include "JustInTime.h"
#include "CharismasPresent.h"
#include <boost/bind/bind.hpp>
#include "PostConfig.h"
#include <boost/algorithm/string.hpp> // for replace_all
#include "EclipseCollect.h"
#include "Songkran.h"
#include "Adventure.h"
#include "LevelResetEvent.h"
#include "SealedChar.h"
#include "ItemCompose.h"
#include "SkillManager.h"
#include "GachaLottery.h"
#include "EclipsePlot.h"
#include "RankManager.h"
#include <boost/foreach.hpp>
#include "VirtualCash.h"
#include "ErrandManager.h"
#include "ItemManager.h"
#include "JumpingChar.h"
#include "SocialCommerce.h"
#include "SHCheckPoint.h"
#include "GachaPong.h"
#include "PetGlyphMng.h"

#define CLASS_TYPE KGSDBThread

INT_IMPL_ON_FUNC( EVENT_SEARCH_GUILD_LIST_REQ )
{
    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_SEARCH_GUILD_LIST_REQ );

    std::vector<DWORD>::iterator vit;

    KEVENT_SEARCH_GUILD_LIST_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_kSearchInfo = kPacket_;
    kPacket.m_vecGuildUIDList.clear();

    std::set<DWORD> setLiveGuildUIDList; // 게임서버가 가지고 있는 Guild UID List
    SiKGuildManager()->GetGuildUIDList( setLiveGuildUIDList );

    // User에서 검사했지만 확인차.
    // 검색어가 비어 있으면 통과
    if ( kPacket_.m_strKeyword.empty() ) {
        SET_ERR_GOTO( NET_OK, END_PROC );
    }

    if ( SearchGuildListFromDB( kPacket_.m_strKeyword, kPacket.m_vecGuildUIDList ) == false ) {
        START_LOG( cerr, L"길드검색 DB처리 실패." )
            << BUILD_LOG( kPacket_.m_strKeyword ) << END_LOG;
        SET_ERR_GOTO( ERR_GUILD_LIST_02, END_PROC );
    }

    // 게임서버에 없는 길드UID 찾아서 DB에서 정보 받아오자.
    for ( vit = kPacket.m_vecGuildUIDList.begin() ; vit != kPacket.m_vecGuildUIDList.end() ; ++vit ) {
        if ( setLiveGuildUIDList.find( *vit ) != setLiveGuildUIDList.end() ) {
            continue;
        }

        UpdateOneGuildInfo( *vit );
    }

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_strKeyword )
        << BUILD_LOG( kPacket.m_vecGuildUIDList.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_SEARCH_GUILD_LIST_ACK );
}

INT_IMPL_ON_FUNC( EVENT_MARK_UPLOAD_REQ )
{
    KNGuildInfo kGuildInfo;
    int nState = -1;
    int nRevision = -1;
    std::wstring strMarkName;
    KEVENT_MARK_UPLOAD_ACK kPacket;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_kItem = kPacket_.m_kItem;
    bool bIsGuildMarkCashItem = ( kPacket.m_kItem.m_ItemID == SiKGuildManager()->GetGuildMarkCashItemID() ? true : false );

    // 길드정보 확인
    KGuildPtr spGuild = SiKGuildManager()->GetGuild( kPacket_.m_dwGuildUID );
    if ( spGuild == NULL ) {
        UpdateOneGuildInfo( kPacket_.m_dwGuildUID );
        UpdateOneGuildMemberList( kPacket_.m_dwGuildUID );
        spGuild = SiKGuildManager()->GetGuild( kPacket_.m_dwGuildUID );
    }
    JIF( spGuild );

    EVENT_CHECK_OUT( EVENT_MARK_UPLOAD_REQ );
    SET_ERROR( ERR_UNKNOWN );

    if ( spGuild->GetGuildMarkStatus() != KNGuildInfo::MS_NORMAL ) {
        // 길드마크 상태 동기화
        kGuildInfo.Init();
        JIF( GetGuildInfoFromDB( kPacket_.m_dwGuildUID, kGuildInfo ) );

        // 길드마크 승인/실패면 상태 갱신해서 진행.
        if ( kGuildInfo.m_ucMarkStatus == KNGuildInfo::MS_ACCEPT_CHANGE || kGuildInfo.m_ucMarkStatus == KNGuildInfo::MS_REJECT_CHANGE ) {
            if ( UpdateGuildMarkInfo( kPacket_.m_dwGuildUID ) == false ) {
                START_LOG( cerr, L"길드마크 상태 갱신 실패..  Login : " << strLogin_ )
                    << BUILD_LOG( kPacket_.m_dwGuildUID )
                    << BUILD_LOGc( kGuildInfo.m_ucMarkStatus )
                    << BUILD_LOG( kGuildInfo.m_nMarkRevision ) << END_LOG;
                SET_ERR_GOTO( ERR_GUILD_MARK_10, END_PROC );
            }

            kGuildInfo.m_ucMarkStatus = KNGuildInfo::MS_NORMAL;
            SiKGuildManager()->Add( kGuildInfo ); // 길드정보 갱신
        }
        else {
            START_LOG( cerr, L"길드마크 업로드 가능한 상태가 아님. Login : " << strLogin_ )
                << BUILD_LOG( kPacket_.m_dwGuildUID )
                << BUILD_LOGc( kGuildInfo.m_ucMarkStatus )
                << BUILD_LOG( kGuildInfo.m_nMarkRevision ) << END_LOG;
            SET_ERR_GOTO( ERR_GUILD_MARK_10, END_PROC );
        }
    }

    // 등급 확인
    kGuildInfo = spGuild->GetNGuildInfo();
    if ( kGuildInfo.m_ucGrade != KNGuildInfo::GG_OFFICIAL ) {

        int nOfficailGuildMemberNum = SiKGuildManager()->GetOfficailGuildMemberNumber();

        if ( kGuildInfo.m_ucGrade == KNGuildInfo::GG_NEWBIE && kGuildInfo.m_nNumMembers >= nOfficailGuildMemberNum ) {
            kGuildInfo.m_ucGrade = KNGuildInfo::GG_OFFICIAL;
            int nRet = UpdateGuildGrade( kPacket_.m_dwGuildUID );
            _LOG_SUCCESS( nRet == 0, L"신규길드 정식길드로 변환. nRet : " << nRet )
                << BUILD_LOG( kPacket_.m_dwGuildUID ) << END_LOG;
        }
        else {
            SET_ERR_GOTO( ERR_GUILD_MARK_03, END_PROC );
        }
    }

    // 마크 상태가 업로드가능한 상태 인가?
    if ( !GetGuildMarkInfo( kPacket_.m_dwGuildUID, nState, nRevision, std::wstring(L"") ) ) {
        SET_ERR_GOTO( ERR_GUILD_MARK_07, END_PROC );
    }

    if ( nState != KNGuildInfo::MS_NORMAL ) {
        START_LOG( cerr, L"이미 위에서 확인했는데, 길드마크 업로드 가능한 상태가 아님. Login : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_dwGuildUID )
            << BUILD_LOG( nState )
            << BUILD_LOG( nRevision )
            << BUILD_LOG( kGuildInfo.m_ucMarkStatus )
            << BUILD_LOG( kGuildInfo.m_nMarkRevision ) << END_LOG;
        //SET_ERR_GOTO( ERR_GUILD_MARK_10, END_PROC );
    }

    // 업로드
    { // local scope
        CFTPClientPtr spFTP = SiKFTPFactory()->GetConnection( std::wstring(L"GuildMark") );
        if ( !spFTP ) {
            SET_ERR_GOTO( ERR_GUILD_MARK_08, END_PROC );
        }

        SiKGuildManager()->MakeGuildMarkName( kPacket_.m_dwGuildUID, nRevision + 1, kPacket_.m_strFileExt, strMarkName );
        kPacket_.m_buffImage.UnCompress();
        const std::vector<char>& vecBuff = kPacket_.m_buffImage.GetBuffer();
        if ( !spFTP->UploadMem( &vecBuff[0], vecBuff.size(), strMarkName.c_str() ) ) {
            SET_ERR_GOTO( ERR_GUILD_MARK_09, END_PROC );
        }
    }

    // 마크 상태 변경
    if ( !SetGuildMarkRegisted( dwUID_, kPacket_.m_dwGuildUID, kPacket_.m_strFileExt, bIsGuildMarkCashItem ) ) {
        SET_ERR_GOTO( ERR_GUILD_MARK_11, END_PROC );
    }

    LIF( PureEraseItem( strLogin_, dwUID_, kPacket_.m_kItem.m_ItemUID, cCharType_ ) );

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOGc( kGuildInfo.m_ucMarkStatus )
        << BUILD_LOG( kGuildInfo.m_nMarkRevision )
        << BUILD_LOG( nState )
        << BUILD_LOG( nRevision )
        << BUILD_LOG( kPacket_.m_strFileExt )
        << BUILD_LOG( strMarkName ) << END_LOG;

    // 유저로 전달.
    QUEUING_ACK_TO_USER( EVENT_MARK_UPLOAD_ACK );
}

INT_IMPL_ON_FUNC( EVENT_CREATE_GUILD_REQ )
{
    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_CREATE_GUILD_REQ );

    KNGuildUserInfo kMasterInfo;
    KEVENT_CREATE_GUILD_ACK kPacket;
    DWORD dwGuildUID = 0;
    kPacket.m_nOK = -99;
    kPacket.m_kGuildInfo.Init();

    // 길드 생성 쿼리
    int nRet = UpdateCreateGuild( dwUID_, kPacket_.m_strGuildName, kPacket_.m_strGuildURL, dwGuildUID );

    // 에러값 처리
    switch ( nRet )
    {
    case 0: // 성공
        SET_ERROR( NET_OK );
        break;
    case -1: // 유저정보 없음
        SET_ERROR( ERR_GUILD_CREATE_03 );
        break;
    case -2: // 길드이름 중에 특수문자가 있음
        SET_ERROR( ERR_GUILD_CREATE_04 );
        break;
    case -3: // 길드이름 중복
        SET_ERROR( ERR_GUILD_CREATE_05 );
        break;
    case -4: // URL 중복
        SET_ERROR( ERR_GUILD_CREATE_06 );
        break;
    case -5: // 이미 가입유저임
        SET_ERROR( ERR_GUILD_CREATE_07 );
        break;
    default: // DB처리 에러
        SET_ERROR( ERR_GUILD_CREATE_08 );
        break;
    }

    if ( nRet != 0 ) {
        START_LOG( cerr, L"길드 생성 실패. nRet : " << nRet ) << END_LOG;
        goto END_PROC;
    }

    // 여기서부턴 길드생성 성공 처리.
    // 생성된 초기 길드 데이터 받아옴
    LIF( GetGuildInfoFromDB( dwGuildUID, kPacket.m_kGuildInfo ) );
    LIF( GetGuildCommentNoticeFromDB( dwGuildUID, kPacket.m_vecGuildNotice ) );
    LIF( GetGuildMemberListFromDB( dwGuildUID, kPacket.m_mapGuildUserList ) );
    if ( GetGuildMasterInfoFromDB( dwGuildUID, kMasterInfo ) == true ) {
        kPacket.m_kGuildInfo.m_strMasterNick = kMasterInfo.m_strNickName;
        kPacket.m_kGuildInfo.m_strMasterNickColor = kMasterInfo.m_strNickColor;
    }

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_strGuildName )
        << BUILD_LOG( kPacket_.m_strGuildURL )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( dwGuildUID )
        << BUILD_LOG( kPacket.m_vecGuildNotice.size() )
        << BUILD_LOG( kPacket.m_mapGuildUserList.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_CREATE_GUILD_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_EDIT_GUILD_NOTICE_REQ, KNGuildNoticeList )
{
    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_EDIT_GUILD_NOTICE_REQ );

    std::vector<KNGuildNotice>::iterator vit;
    KNGuildNoticeList kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_vecNotice.clear();
    int nRet = -99;

    if ( kPacket_.m_vecNotice.empty() ) {
        // 소개공지 정보가 비어있음.
        SET_ERR_GOTO( ERR_GUILD_NOTICE_01, END_PROC );
    }

    for ( vit = kPacket_.m_vecNotice.begin() ; vit != kPacket_.m_vecNotice.end() ; ++vit ) {

        // 등록된 공지 타입인지 확인.
        if ( vit->m_nMsgID < KNGuildNotice::GN_COMMENT ||
             vit->m_nMsgID >= KNGuildNotice::GN_MAX ) {

            START_LOG( cerr, L"등록되지 않은 공지 타입. MsgID : " << vit->m_nMsgID ) << END_LOG;
            continue;
        }

        // 길드 소개/공지 입력(수정)처리
        nRet = UpdateGuildCommentNoice( dwUID_, kPacket_.m_dwGuildUID, vit->m_nMsgID, vit->m_strMsg );

        if ( nRet != 0 ) {
            START_LOG( cerr, L"길드 소개/공지 입력(수정) DB처리중 실패. nRet : " << nRet ) << END_LOG;
        }

        // 성공한 공지를 담아준다.
        kPacket.m_vecNotice.push_back( *vit );

        START_LOG( clog, L"길드 소개/공지 입력(수정) DB처리 성공." )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_dwGuildUID )
            << BUILD_LOG( vit->m_nMsgID )
            << BUILD_LOG( vit->m_strMsg ) << END_LOG;
    }

    // 성공한 처리가 하나도 없다.
    if ( kPacket.m_vecNotice.empty() ) {
        // 에러값 처리
        switch ( nRet )
        {
        case -1: // 유저정보 없음
            SET_ERR_GOTO( ERR_GUILD_NOTICE_02, END_PROC );
        case -2: // 길드정보 없음
            SET_ERR_GOTO( ERR_GUILD_NOTICE_03, END_PROC );
        case -3: // 길드마스터 또는 제너럴이 아님
            SET_ERR_GOTO( ERR_GUILD_NOTICE_04 , END_PROC);
        case -4: // 길드 Notice 정보 없음(DBA 호출)
            SET_ERR_GOTO( ERR_GUILD_NOTICE_05 , END_PROC);
        default: // DB처리 에러
            SET_ERR_GOTO( ERR_GUILD_NOTICE_06, END_PROC );
        }
    }

    // 성공처리가 1개라도 있으면 성공.
    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_vecNotice.size() )
        << BUILD_LOG( kPacket.m_vecNotice.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_EDIT_GUILD_NOTICE_ACK );
}

INT_IMPL_ON_FUNC( EVENT_EDIT_GUILD_MY_COMMENT_REQ )
{
    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_EDIT_GUILD_MY_COMMENT_REQ );

    KEVENT_EDIT_GUILD_MY_COMMENT_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_strMyComment = kPacket_.m_strMyComment;

    // 자기소개변경 처리
    int nRet = UpdateMyGuildComment( dwUID_, kPacket_.m_dwGuildUID, kPacket_.m_strMyComment );

    // 에러값 처리
    switch ( nRet )
    {
    case 0: // 성공
        SET_ERROR( NET_OK );
        break;
    case -1: // 유저정보 없음
        SET_ERROR( ERR_GUILD_MODIFY_MYCOMMENT_00 );
        break;
    case -2: // 길드정보 없음
        SET_ERROR( ERR_GUILD_MODIFY_MYCOMMENT_01 );
        break;
    case -3: // 길드원이 아님.
        SET_ERROR( ERR_GUILD_MODIFY_MYCOMMENT_02 );
        break;
    default: // DB처리 에러
        SET_ERROR( ERR_GUILD_MODIFY_MYCOMMENT_03 );
        break;
    }

    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwGuildUID )
        << BUILD_LOG( kPacket.m_strMyComment ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_EDIT_GUILD_MY_COMMENT_ACK );    
}

INT_IMPL_ON_FUNC( EVENT_EDIT_GUILD_NAME_REQ )
{   
    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_EDIT_GUILD_NAME_REQ );
    KEVENT_EDIT_GUILD_NAME_ACK kPacket;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_strGuildName = kPacket_.m_strGuildName;

    int nRet = -99;
    
    nRet = UpdateGuildName( dwUID_, kPacket_.m_dwGuildUID, kPacket_.m_strGuildName );

    switch ( nRet ) {
        case 0: // 성공
            SET_ERROR( NET_OK );
            break;
        case -1: // 유저정보 없음
            SET_ERROR( ERR_GUILD_EDIT_NAME_00 );
            break;
        case -2: // 길드정보 없음
            SET_ERROR( ERR_GUILD_EDIT_NAME_05 );
            break;
        case -3: // 길드마스터가 아님
            SET_ERROR( ERR_GUILD_EDIT_NAME_02 );
            break;
        case -4: // 중복된 길드이름
            SET_ERROR( ERR_GUILD_EDIT_NAME_10 );
            break;
        default: // DB 에러나 unknown
            SET_ERROR( ERR_GUILD_EDIT_NAME_06 );
    }

    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_strGuildName ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_EDIT_GUILD_NAME_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_EDIT_GUILD_URL_REQ, KNGuildURL )
{
    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_EDIT_GUILD_URL_REQ );

    int nRet = -99;
    nRet = UpdateGuildURL( dwUID_, kPacket_.m_dwUID, kPacket_.m_strURL );

    switch( nRet ) {
        case 0:
            SET_ERROR( NET_OK );
            break;
        case -1: //-1       ; 유저정보 없음
            SET_ERROR( ERR_GUILD_EDIT_URL_04 );
            break;
        case -2: //-2       ; 길드정보 없음
            SET_ERROR( ERR_GUILD_EDIT_URL_05 );
            break;
        case -3: //-3       ; 길드마스터가 아님
            SET_ERROR( ERR_GUILD_EDIT_URL_03 );
            break;
        default:
            SET_ERROR( ERR_GUILD_EDIT_URL_06 );
    }

    kPacket_.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUID )
        << BUILD_LOG( kPacket_.m_strURL ) << END_LOG;

   _QUEUING_ACK_TO_USER( EVENT_EDIT_GUILD_URL_ACK, kPacket_ );
}

INT_IMPL_ON_FUNC( EVENT_GUILD_MEMBER_LIST_REQ )
{
    EVENT_CHECK_OUT( EVENT_GUILD_MEMBER_LIST_REQ );

    KEVENT_GUILD_MEMBER_LIST_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_nPageNum = kPacket_.m_nPageNum;
    kPacket.m_mapNGuildMemberList.clear();

    bool bRet = GetGuildMemberListFromDB( kPacket.m_dwGuildUID, kPacket.m_mapNGuildMemberList );

    // DB처리 에러
    if ( bRet == false ) {
        SET_ERR_GOTO( ERR_GUILD_MEMBER_LIST_02, END_PROC );
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwGuildUID )
        << BUILD_LOG( kPacket.m_nPageNum )
        << BUILD_LOG( kPacket.m_mapNGuildMemberList.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_GUILD_MEMBER_LIST_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_GET_GUILD_INFO_FROM_DB_REQ, DWORD )
{
    // DB에서 길드정보 받아와서 등록(이미 있는 길드가 대상)
    UpdateOneGuildInfo( kPacket_ );
    UpdateOneGuildMemberList( kPacket_ );
}

_INT_IMPL_ON_FUNC( EVENT_GET_GUILD_MEMBER_LIST_FROM_DB_REQ, DWORD )
{
    // DB에서 길드 맴버리스트 받아와서 등록(이미 있는 길드가 대상)
    UpdateOneGuildMemberList( kPacket_ );
}

_INT_IMPL_ON_FUNC( EVENT_UPGRADE_GUILD_GRADE_GIFT_NOT, KECN_UPGRADE_GUILD_GRADE_GIFT_ACK )
{
    SendUpgradeGuildGradeGiftNot( kPacket_.m_dwGuildMasterUID, cCharType_, kPacket_.m_dwGuildUID );
}

_INT_IMPL_ON_FUNC( EVENT_CHECK_UPGRADE_GUILD_GRADE_GIFT_NOT, DWORD )
{
    // kPacket_: dwGuildUID
    SendUpgradeGuildGradeGiftNot( dwUID_, cCharType_, kPacket_ );
}

_INT_IMPL_ON_FUNC( EVENT_SOCKET_OPEN_REQ, KEVENT_SOCKET_OPEN_ACK )
{
    EVENT_CHECK_OUT( EVENT_SOCKET_OPEN_REQ );
    if ( m_kHelper.UpdateSocket( dwUID_, 
        (int)kPacket_.m_kSocketInfo.m_cSlotID, 
        0, 
        KSocketInfo::STS_OPENED, 
        kPacket_.m_kItem,
        cCharType_ ) ) {
        
        kPacket_.m_nOK = NetError::NET_OK;
    }
    else {
        kPacket_.m_nOK = NetError::ERR_SOCKET_OPEN_11;
    }

    LOG_SUCCESS( kPacket_.m_nOK == NetError::NET_OK )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOGc( kPacket_.m_kSocketInfo.m_cSlotID )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemID ) << END_LOG;

   _QUEUING_ACK_TO_USER( EVENT_SOCKET_OPEN_ACK, kPacket_ );
}

_INT_IMPL_ON_FUNC( EVENT_MONSTER_CARD_INSERT_REQ, KEVENT_MONSTER_CARD_INSERT_ACK )
{
    EVENT_CHECK_OUT( EVENT_MONSTER_CARD_INSERT_REQ );

    if ( m_kHelper.UpdateSocket( dwUID_,
        (int)kPacket_.m_kSocketInfo.m_cSlotID,
        kPacket_.m_kSocketInfo.m_CardItemID,
        KSocketInfo::STS_USING,
        kPacket_.m_kItem,
        cCharType_ ) ) {

        kPacket_.m_nOK = NetError::NET_OK;
    }
    else {
        kPacket_.m_nOK = NetError::ERR_SOCKET_OPEN_11;
    }

    if( kPacket_.m_nOK == NetError::NET_OK ) {
        // 동기화전 InitCount, Count값 변동값 있는지 체크.
        if( !CheckItemDuration( dwUID_, kPacket_.m_kCardItem.m_ItemUID, kPacket_.m_kCardItem.m_nInitCount, kPacket_.m_kCardItem.m_nCount ) ) {
            START_LOG( clog, L"몬스터 카드 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( kPacket_.m_kCardItem.m_ItemUID )
                << BUILD_LOG( kPacket_.m_kCardItem.m_nInitCount )
                << BUILD_LOG( kPacket_.m_kCardItem.m_nCount )
                << END_LOG;
        }

        if ( !UseCountItem( dwUID_, kPacket_.m_kCardItem.m_ItemUID, 1 ) ) {
            START_LOG( cerr, L"몬스터 카드 개수 동기화 실패. Name : " << strLogin_ ) << END_LOG;
            kPacket_.m_nOK = NetError::ERR_SOCKET_OPEN_15;
            kPacket_.m_kCardItem.m_nCount = 0;
        }
        kPacket_.m_kCardItem.m_nCount -= 1;
        kPacket_.m_kCardItem.m_nCount = std::max<int>( kPacket_.m_kCardItem.m_nCount, 0 );

        if ( kPacket_.m_nConsumeKind == KGCSocketConsumeItem::CASHITEM ) {
            // 동기화전 InitCount, Count값 변동값 있는지 체크.
            if( !CheckItemDuration( dwUID_, kPacket_.m_kCashItem.m_ItemUID, kPacket_.m_kCashItem.m_nInitCount, kPacket_.m_kCashItem.m_nCount ) ) {
                START_LOG( clog, L"몬스터 카드 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
                    << BUILD_LOG( dwUID_ )
                    << BUILD_LOG( kPacket_.m_kCashItem.m_ItemUID )
                    << BUILD_LOG( kPacket_.m_kCashItem.m_nInitCount )
                    << BUILD_LOG( kPacket_.m_kCashItem.m_nCount )
                    << END_LOG;
            }

            if ( !UseCountItem( dwUID_, kPacket_.m_kCashItem.m_ItemUID, kPacket_.m_nUseCount ) ) {
                START_LOG( cerr, L"캐시 아이템 개수 동기화 실패. Name : " << strLogin_ ) << END_LOG;
                kPacket_.m_nOK = NetError::ERR_SOCKET_OPEN_16;
                kPacket_.m_kCashItem.m_nCount = 0;
            }
            kPacket_.m_kCashItem.m_nCount -= kPacket_.m_nUseCount;
            kPacket_.m_kCashItem.m_nCount = std::max<int>( kPacket_.m_kCashItem.m_nCount, 0 );
        }
    }

   _QUEUING_ACK_TO_USER( EVENT_MONSTER_CARD_INSERT_ACK, kPacket_ );
}

_INT_IMPL_ON_FUNC( EVENT_MONSTER_CARD_REMOVE_REQ, KEVENT_MONSTER_CARD_REMOVE_ACK )
{
    EVENT_CHECK_OUT( EVENT_MONSTER_CARD_REMOVE_REQ );

    if ( m_kHelper.UpdateSocket( dwUID_,
        (int)kPacket_.m_kSocketInfo.m_cSlotID,
        0, // 0 이어야 함.
        KSocketInfo::STS_OPENED,
        kPacket_.m_kItem,
        cCharType_ ) ) {

        kPacket_.m_nOK = NetError::NET_OK;
        m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, kPacket_.m_kSocketInfo.m_CardItemID, 1, -1, KItemManager::EWIT_MONSTER, kPacket_.m_vecInv );

    }
    else {
        // ERROR
        kPacket_.m_nOK = NetError::ERR_SOCKET_OPEN_11;
    }

    if ( kPacket_.m_nOK == NetError::NET_OK ) {
        int nUseCount = 0;
        nUseCount = SiKItemManager()->GetUseSocketItemCount( kPacket_.m_kItem );

        // 동기화전 InitCount, Count값 변동값 있는지 체크.
        if( !CheckItemDuration( dwUID_, kPacket_.m_kCashItem.m_ItemUID, kPacket_.m_kCashItem.m_nInitCount, kPacket_.m_kCashItem.m_nCount ) ) {
            START_LOG( clog, L"몬스터 카드 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( kPacket_.m_kCashItem.m_ItemUID )
                << BUILD_LOG( kPacket_.m_kCashItem.m_nInitCount )
                << BUILD_LOG( kPacket_.m_kCashItem.m_nCount )
                << END_LOG;
        }

        if ( !UseCountItem( dwUID_, kPacket_.m_kCashItem.m_ItemUID, nUseCount ) ) {
            kPacket_.m_nOK = NetError::ERR_SOCKET_OPEN_16;
            kPacket_.m_kCashItem.m_nCount = 0;
        }
        kPacket_.m_kCashItem.m_nCount -= nUseCount;
        kPacket_.m_kCashItem.m_nCount = std::max<int>( kPacket_.m_kCashItem.m_nCount, 0 );
    }

    _QUEUING_ACK_TO_USER( EVENT_MONSTER_CARD_REMOVE_ACK, kPacket_ );
}

INT_IMPL_ON_FUNC( EVENT_ATTRIBUTE_SELECT_REQ )
{
    EVENT_CHECK_OUT( EVENT_ATTRIBUTE_SELECT_REQ );
    KEVENT_ATTRIBUTE_SELECT_ACK kPacket;

    KItem kItem = kPacket_.m_kItem;
    kItem.m_vecAttribute.clear();

    bool bUpdateSuccess = true;
    for( int i = 0 ; i < (int)kPacket_.m_vecAttributeList.size() ; ++i )
    {
        KAttributeInfo& kAttInfo = kPacket_.m_vecAttributeList[i];
        LIF( bUpdateSuccess = bUpdateSuccess && m_kHelper.UpdateAttribute(dwUID_, i, kAttInfo.m_cType, kAttInfo.m_fValue, kItem, cCharType_ ) );
    }

    if( bUpdateSuccess == false )
    {
        kPacket.m_nOK = NetError::ERR_ATTRIBUTE_SELECT_04;
    }
    else
    {
        kPacket.m_nOK = NetError::NET_OK;
        kPacket.m_kItem = kItem;
    }

    QUEUING_ACK_TO_USER_UID( EVENT_ATTRIBUTE_SELECT_ACK );
}

INT_IMPL_ON_FUNC( EVENT_ITEM_BREAKUP_REQ )
{
    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_ITEM_BREAKUP_REQ );

    KEVENT_ITEM_BREAKUP_ACK kPacket;
    kPacket.m_bJackpot = kPacket_.m_bJackpot;
    kPacket.m_kBreakupItem = kPacket_.m_kBreakupItem;
    kPacket.m_nAccReelPoint = kPacket_.m_nAccReelPoint;
    kPacket.m_nGPCost = kPacket_.m_nGPCost;
    kPacket.m_nGP = 0;

    std::vector<KDropItemInfo>::iterator vit;

    // 아이템을 받아온다.
    for ( vit = kPacket_.m_vecMaterials.begin() ; vit != kPacket_.m_vecMaterials.end() ; ++vit ) {
        LIF( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_BREAKUP, kPacket.m_vecMaterials ) );
    }

    // 릴포인트 업데이트 한다.
    LIF( m_kHelper.SetBreakupReelPoint( dwUID_, kPacket_.m_nAccReelPoint ) );

    // 아이템 삭제 한다.
    LIF( m_kHelper.EraseItemByBreakup( dwUID_, cCharType_, kPacket_.m_kBreakupItem.m_ItemUID ) );
    if( kPacket_.m_kStrengthItem.m_ItemID != 0 ) { // 연계된 강화석 삭제.
        LIF( m_kHelper.EraseItemByBreakup( dwUID_, cCharType_, kPacket_.m_kStrengthItem.m_ItemUID ) );
        kPacket.m_kStrengthItem = kPacket_.m_kStrengthItem;
    }

    SET_ERROR( NET_OK );

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_ITEM_BREAKUP_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_UPDATE_TIME_DROP_NOT )
{
    // 일일 획득한 드랍 아이템 회차 기록
    LIF( SetTodayEventDone( dwUID_, UET_TIME_DROP_COUNT, kPacket_.m_tmRegDate, kPacket_.m_nCurruntDropCount ) );
    // TimeDrop AccPlayTime 기록
    LIF( SetTodayEventDone( dwUID_, UET_TIME_DROP_ACC_TIME, kPacket_.m_tmRegDate, kPacket_.m_nAccPlayTime ) );
    
}

_INT_IMPL_ON_FUNC( DB_EVENT_TIME_DROP_REQ, KTimeDropItem )
{
    // 시간단위별 드랍 아이템 받기 요청.
    KDB_EVENT_TIME_DROP_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_kDropItem = kPacket_;
    kPacket.m_kDropItem.m_vecItems.clear();

    // 아이템 넣기.
    std::vector<KItem>::iterator vit;
    for ( vit = kPacket_.m_vecItems.begin() ; vit != kPacket_.m_vecItems.end() ; ++vit ) {
        LIF( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nCount, vit->m_nPeriod, KItemManager::EWIT_EVENT_TIME_DROP, kPacket.m_kDropItem.m_vecItems ) );
    }

    // 받을 아이템이 있으면 성공
    if ( !kPacket.m_kDropItem.m_vecItems.empty() ) {
        kPacket.m_nOK = 0;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"kPacket.m_nOK : " << kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kDropItem.m_vecItems.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( DB_EVENT_TIME_DROP_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_GET_TIME_DROP_INFO_REQ )
{
    // DB에서 유저의 일일 TimeDrop 정보 받아옴.
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KDB_EVENT_GET_TIME_DROP_INFO_ACK kPacket;
    kPacket.m_nCurruntDropCount = GetTodayEventCount( dwUID_, UET_TIME_DROP_COUNT, tmCurrent );
    kPacket.m_nAccPlayTime = GetTodayEventCount( dwUID_, UET_TIME_DROP_ACC_TIME, tmCurrent );
    kPacket.m_tmRegDate = tmCurrent;

    START_LOG( clog, L"Get TimeDrop Info from DB. Login : " << strLogin_ )
        << BUILD_LOG( kPacket.m_nCurruntDropCount )
        << BUILD_LOG( kPacket.m_nAccPlayTime )
        << BUILD_LOGtm( CTime(tmCurrent) ) << END_LOG;

    QUEUING_ACK_TO_USER( DB_EVENT_GET_TIME_DROP_INFO_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_GET_WEB_CASH_ITEM_REQ, int )
{
    EVENT_CHECK_OUT( EVENT_GET_WEB_CASH_ITEM_REQ );
    KEVENT_GET_WEB_CASH_ITEM_ACK kPacket;
    kPacket.m_nOK = 0;
    kPacket.m_vecGetItems.clear();

    m_kHelper.InsertCashWaitItemToInven( dwUID_, kPacket_, strLogin_, kPacket );
    QUEUING_ACK_TO_USER( EVENT_GET_WEB_CASH_ITEM_ACK );

    START_LOG( clog, L" 웹에서 캐시 아이템 구매. Name : " << strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kVirtualCashInfo.m_nOK )
        << BUILD_LOG( kPacket.m_kVirtualCashInfo.m_dwCashPoint )
        << BUILD_LOG( kPacket.m_kVirtualCashInfo.m_dwAddCashPoint )
        << BUILD_LOG( kPacket.m_kVirtualCashInfo.m_dwDeductCashPoint )
        << BUILD_LOG( kPacket.m_vecGetItems.size() )
        << BUILD_LOG( kPacket_ )
        << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_STAT_RESOLUTION_NOT )
{
    LIF( KSP( m_kODBCStat ).StatrecordResolution( dwUID_, kPacket_.m_nWidth, kPacket_.m_nHeight, kPacket_.m_nMaxWidth, kPacket_.m_nMaxHeight ) );
}

INT_IMPL_ON_FUNC( EVENT_MANUFACTURES3_REQ )
{
    KEVENT_MANUFACTURES3_ACK kPacket;
    kPacket.m_vecPeriodMaterialItems = kPacket_.m_vecNormalMaterials;
    kPacket.m_prKey = std::make_pair( kPacket_.m_kCatalog.m_Index, kPacket_.m_kCatalog.m_ItemID );
    kPacket.m_usFactor = kPacket_.m_usFactor;
    kPacket.m_vecStrengthItem = kPacket_.m_vecStrengthItem;

    if( !kPacket_.m_kCatalog.m_bIsOnDiscount )
        kPacket.m_dDiscount = 0;
    else
        kPacket.m_dDiscount = kPacket_.m_dDiscount;

    int nCurrentFactor = kPacket_.m_usFactor;
    std::vector<GCITEMUID>::iterator vit;
    int nOK = 0;
    int totalDiscount = 0;

    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_MANUFACTURES3_REQ );

    bool bLookItem = kPacket_.m_kCatalog.CheckItemFlag( KManufactureItem::IF_LOOK );

    // 수량성 아이템 제작이면 제작 회수에 따라 수량 정보를 갱신하자.
    if ( kPacket_.m_kCatalog.m_nDuration != KItem::UNLIMITED_ITEM && nCurrentFactor > 0 ) {
        kPacket_.m_kCatalog.m_nDuration *= nCurrentFactor;
        nCurrentFactor = 1;
    }

    for ( int i = 0 ; i < nCurrentFactor ; ++i ) {
        if ( !m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, kPacket_.m_kCatalog.m_ItemID, kPacket_.m_kCatalog.m_nDuration, kPacket_.m_kCatalog.m_nPeriod, KItemManager::EWIT_MANUFACTURE, kPacket.m_vecRewards, kPacket_.m_kCatalog.m_nGrade, kPacket_.m_kCatalog.m_nLevel ) ) {
            START_LOG( cerr, L"제작 아이템 삽입 실패 Name : " << strLogin_ )
                << BUILD_LOG( kPacket_.m_kCatalog.m_Index )
                << BUILD_LOG( kPacket_.m_kCatalog.m_ItemID )
                << BUILD_LOG( kPacket_.m_kCatalog.m_nDuration )
                << BUILD_LOG( kPacket_.m_kCatalog.m_nPeriod )
                << BUILD_LOG( kPacket_.m_kCatalog.m_nGrade ) << END_LOG;
            SET_ERR_GOTO( ERR_MANUFACTURE_S3_05, END_PROC );
        }
    }

    for( vit = kPacket_.m_vecNormalMaterials.begin() ; vit != kPacket_.m_vecNormalMaterials.end() ; ++vit ) {
        LIF( PureEraseItem( strLogin_, dwUID_, *vit, cCharType_ ) );
    }

    for( vit = kPacket_.m_vecStrengthItem.begin() ; vit != kPacket_.m_vecStrengthItem.end() ; ++vit ) { // 연계된 강화석 삭제.
        LIF( PureEraseItem( strLogin_, dwUID_, *vit, cCharType_ ) );
    }

    if ( bLookItem ) {
        std::vector< KItem >::iterator vitReward;
        for ( vitReward = kPacket.m_vecRewards.begin() ; vitReward != kPacket.m_vecRewards.end() ; ++vitReward ) {
            if( vitReward->m_cItemType == KItem::TYPE_LOOK ) {
                continue;
            }

            // 룩 아이템 변환
            if ( !m_kHelper.NormalItemToCoordiItem( dwUID_, vitReward->m_ItemUID, cCharType_, nOK ) ) {
                START_LOG( cerr, L"DB작업중 룩 아이템 변환 실패. OK : " << nOK << L"ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
                vitReward->DeleteItemType(KItem::TYPE_LOOK);
                continue;
            }

            vitReward->AddItemType(KItem::TYPE_LOOK);
            START_LOG( clog, L"룩 아이템으로 변환됨. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
        }
    }

    if ( !kPacket.m_vecRewards.empty() ) {
        std::vector<KDropItemInfo> vecMaterials;
        std::vector<KDropItemInfo>::iterator vitCount;

        if ( SiKItemManager()->m_kItemManufacture.GetMaterials( kPacket.m_prKey, vecMaterials ) ) {

            for ( vitCount = vecMaterials.begin() ; vitCount != vecMaterials.end() ; ++vitCount ) {
                std::map< GCITEMID, KItem >::iterator mit;
                mit = kPacket_.m_mapKItem.find( vitCount->m_ItemID );
                
                if ( mit == kPacket_.m_mapKItem.end() ) continue;

                if (vitCount->m_nDuration != KItem::UNLIMITED_ITEM)
                {
                    if (kPacket.m_dDiscount > 0)
                    {
                        totalDiscount = (int)ceil(vitCount->m_nDuration - (vitCount->m_nDuration * kPacket.m_dDiscount));
                    }
                    else
                    {
                        totalDiscount = vitCount->m_nDuration;
                    }
                }

                int nDelCount = totalDiscount * kPacket_.m_usFactor;

                // 동기화전 InitCount, Count값 변동값 있는지 체크.
                if( !CheckItemDuration( dwUID_, mit->second.m_ItemUID, mit->second.m_nInitCount, mit->second.m_nCount ) ) {
                    START_LOG( clog, L"제작해체 재료 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
                        << BUILD_LOG( dwUID_ )
                        << BUILD_LOG( mit->second.m_ItemUID )
                        << BUILD_LOG( mit->second.m_nInitCount )
                        << BUILD_LOG( mit->second.m_nCount )
                        << END_LOG;
                }
                
                if ( !UseCountItem( dwUID_, mit->second.m_ItemUID, nDelCount ) ) {
                    mit->second.m_nCount = 0;
                    START_LOG( cerr, L"제작해체 재료 아이템 동기화 실패. Name : " << strLogin_ )
                        << BUILD_LOG( vitCount->m_ItemID ) << END_LOG;
                } else {
                    mit->second.m_nCount = std::max<int>( 0, mit->second.m_nCount - nDelCount );
                }
            }
        }
    }

    SET_ERROR( NET_OK );
    kPacket.m_mapKItem = kPacket_.m_mapKItem;
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    QUEUING_ACK_TO_USER( EVENT_MANUFACTURES3_ACK );

    _LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_kCatalog.m_ItemID )
        << BUILD_LOG( kPacket_.m_kCatalog.m_nDuration )
        << BUILD_LOG( kPacket_.m_kCatalog.m_nPeriod )
        << BUILD_LOG( kPacket.m_vecRewards.size() )
        << BUILD_LOG( bLookItem ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_PRESENTBOX_TRADE_REQ )
{
    EVENT_CHECK_OUT( EVENT_PRESENTBOX_TRADE_REQ );

    KEVENT_PRESENTBOX_TRADE_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );
    // 아이템 넣기.
    std::vector< KDropItemInfo >::iterator vit;
    for ( vit = kPacket_.m_vecRewardItem.begin() ; vit != kPacket_.m_vecRewardItem.end() ; ++vit ) {
        LIF( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_EVENT_PRESENTBOX_TRADE, kPacket.m_vecItem ) );
    }
    // 받을 아이템이 있으면 성공
    if ( !kPacket.m_vecItem.empty() ) {
        kPacket.m_nOK = 0;
        // 조각 ItemID 넘기기.
        kPacket.m_dwItemID = kPacket_.m_dwItemID;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"kPacket.m_nOK : " << kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwItemID )
        << BUILD_LOG( kPacket.m_vecItem.size() ) << END_LOG;
    QUEUING_ACK_TO_USER( EVENT_PRESENTBOX_TRADE_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_PRESENTBOX_ACTION_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_PRESENTBOX_ACTION_REQ );

    KEVENT_PRESENTBOX_ACTION_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    LIF( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, kPacket_.m_kPostItem.m_ItemID, kPacket_.m_kPostItem.m_nDuration, kPacket_.m_kPostItem.m_nPeriod, KItemManager::EWIT_EVENT_PRESENTBOX_ACTION, kPacket.m_vecItem ) );

    // 선물 상자 이벤트 버전 정보 얻어오기.
    DWORD dwVersion = SiKPresentBox()->GetVersion();

    // 받을 아이템이 있으면 성공
    if ( !kPacket.m_vecItem.empty() ) {
        kPacket.m_nOK = 0;
        // 조각 ItemID 넘기기.
        kPacket.m_setItemList = kPacket_.m_setItemList;
        if( !kPacket.m_vecItem.empty() ) {
            std::vector<KItem>::iterator vit;
            for( vit = kPacket.m_vecItem.begin(); vit != kPacket.m_vecItem.end(); ++vit ) {
                PresentBoxInsertLog( dwUID_, dwVersion, vit->m_ItemID, vit->m_nCount, vit->m_nPeriod );
            }
        }
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"kPacket.m_nOK : " << kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_setItemList.size() )
        << BUILD_LOG( kPacket.m_vecItem.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_PRESENTBOX_ACTION_ACK );
}

_INT_IMPL_ON_FUNC( DB_EVENT_UPDATE_BUY_CHECK_COUNT_REQ, GCITEMID )
{
    // 구매제한 아이템 구매 횟수 증가.
    CommitBuyLimitItem( dwUID_, kPacket_ );

    START_LOG( clog, L" 구매제한 아이템 구매 횟수 증가... Name : " << strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket_ ) << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_GUILD_PAGE_BUILD_NOT )
{
    // 길드매니저 등록되어있는 길드UID 받아오기.
    std::set<DWORD> setLiveGuildUIDList;
    SiKGuildManager()->GetGuildUIDList( setLiveGuildUIDList );

    // 빌드할 페이지의 길드UID가 있는지 체크.
    std::vector<DWORD>::iterator vit;
    for ( vit = kPacket_.begin() ; vit != kPacket_.end() ; ++vit ) {
        if ( setLiveGuildUIDList.find( *vit ) != setLiveGuildUIDList.end() )
            continue;

        // 없으면 DB에서 길드정보 등록
        UpdateOneGuildInfo( *vit );
    }

    // 길드 페이지 Build
    SiKGuildManager()->BuildGuildPage();

    START_LOG( clog, L"Guild Page Build Notice. size : " << kPacket_.size() ) << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_GUILD_LIST_REQ, KGuildPageBuild )
{
    // 없는 길드 페이지를 빌드하기 위해, 해당 길드들 정보를 DB에 요청.
    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_GUILD_LIST_REQ );

    KEVENT_GUILD_LIST_NOT kPacket;
    kPacket.m_nOK = 0;
    kPacket.m_nPageNum = kPacket_.m_nPagenum;
    kPacket.m_nTotalPageNum = kPacket_.m_nTotalPageNum;
    kPacket.m_kCompBuff.Clear();

    // 길드매니저 등록되어있는 길드UID 받아오기.
    std::set<DWORD> setLiveGuildUIDList;
    SiKGuildManager()->GetGuildUIDList( setLiveGuildUIDList );

    // 빌드할 페이지의 길드UID가 있는지 체크.
    std::vector<DWORD>::iterator vit;
    for ( vit = kPacket_.m_vecGuildUID.begin() ; vit != kPacket_.m_vecGuildUID.end() ; ++vit ) {
        if ( setLiveGuildUIDList.find( *vit ) != setLiveGuildUIDList.end() )
            continue;

        // 없으면 DB에서 길드정보 등록
        UpdateOneGuildInfo( *vit );
    }

    // 길드 페이지 Build
    SiKGuildManager()->BuildOnePage( kPacket_.m_ucOrderBy, kPacket_.m_vecGuildUID, kPacket.m_kCompBuff );

    START_LOG( clog, L"Build Guild List Page Req." )
        << BUILD_LOGc( kPacket_.m_ucSortType )
        << BUILD_LOGc( kPacket_.m_ucOrderBy )
        << BUILD_LOG( kPacket_.m_nPagenum )
        << BUILD_LOG( kPacket_.m_nTotalPageNum )
        << BUILD_LOG( kPacket_.m_vecGuildUID.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_GUILD_LIST_NOT );
}

INT_IMPL_ON_FUNC( DB_EVENT_ONE_GUILD_PAGE_BUILD_NOT )
{
    // 길드매니저 등록되어있는 길드UID 받아오기.
    std::set<DWORD> setLiveGuildUIDList;
    SiKGuildManager()->GetGuildUIDList( setLiveGuildUIDList );

    std::map<UCHAR,std::vector<DWORD> >::iterator mit;
    std::vector<DWORD>::iterator vit;

    // 빌드할 페이지의 길드UID가 있는지 체크.
    for ( mit = kPacket_.m_mapGuildUID.begin() ; mit != kPacket_.m_mapGuildUID.end() ; ++mit ) {
        for ( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit ) {
            if ( setLiveGuildUIDList.find( *vit ) != setLiveGuildUIDList.end() )
                continue;

            // 없으면 DB에서 길드정보 등록
            UpdateOneGuildInfo( *vit );
        }

        // 길드 페이지 Build/등록
        KSerBuffer kCompBuff;
        SiKGuildManager()->BuildOnePage( SiKGuildManager()->EOB_ASC, mit->second, kCompBuff );
        SiKGuildManager()->UpdateGuildPage( SiKGuildManager()->EOB_ASC, mit->first, kPacket_.m_nPagenum, kCompBuff );
        SiKGuildManager()->BuildOnePage( SiKGuildManager()->EOB_DESC, mit->second, kCompBuff );
        SiKGuildManager()->UpdateGuildPage( SiKGuildManager()->EOB_DESC, mit->first, kPacket_.m_nPagenum, kCompBuff );
    }

    START_LOG( clog, L"Build One GuildPage." )
        << BUILD_LOGc( kPacket_.m_nPagenum )
        << BUILD_LOGc( kPacket_.m_mapGuildUID.size() ) << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_GUIDE_BOOL_CHECKPOINT_NOT, PAIR_SHORT )
{
    GBGAGuideBook_merge( dwUID_, kPacket_ );
}

_INT_IMPL_ON_FUNC( DB_EVENT_UPDATE_GUILD_PAGE_INFO_NOT, DWORD )
{
    UpdateOneGuildInfo( kPacket_ );
    SiKGuildManager()->UpdatePageInfo( kPacket_ );
}

_INT_IMPL_ON_FUNC( EVENT_CHANGE_NORMAL_TO_LOOK_REQ, KDB_EVENT_CHANGE_NORMAL_TO_LOOK_REQ )
{
    KChangeLookItem kPacket;
    kPacket.m_vecItems.reserve( kPacket_.m_vecItem.size() );

    int nChangeCount = 0;
    std::vector<KItem>::iterator vit;
    int nOK = 0;
    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_CHANGE_NORMAL_TO_LOOK_REQ );

    for ( vit = kPacket_.m_vecItem.begin() ; vit != kPacket_.m_vecItem.end() ; ++vit ) {
        if ( !m_kHelper.NormalItemToCoordiItem( dwUID_, vit->m_ItemUID, cCharType_, nOK ) ) {
            START_LOG( cerr, L"DB작업중 룩 아이템 변환 실패. OK : " << nOK << L"ItemID : " << vit->m_ItemID << L",ItemUID : " << vit->m_ItemUID ) << END_LOG;
            SET_ERR_GOTO( ERR_LOOK_EQUIP_07, END_PROC );
        }

        vit->AddItemType(KItem::TYPE_LOOK);
        kPacket.m_vecItems.push_back( *vit );

        ++nChangeCount;
    }

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_kItem.m_ItemUID, kPacket_.m_kItem.m_nInitCount, kPacket_.m_kItem.m_nCount ) ) {
        START_LOG( clog, L"룩 변환 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_kItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_kItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_kItem.m_nCount )
            << END_LOG;
    }

    if ( !UseCountItem( dwUID_, kPacket_.m_kItem.m_ItemUID, kPacket_.m_vecItem.size() ) ) {
        START_LOG( cerr, L"룩 변환 아이템 개수 동기화 실패. Name : " << strLogin_ ) << END_LOG;
        SET_ERROR( ERR_LOOK_EQUIP_19 );
    }
    kPacket_.m_kItem.m_nCount -= kPacket_.m_vecItem.size();
    kPacket.m_kItem = kPacket_.m_kItem;
    kPacket.m_vecItems = kPacket_.m_vecItem;

    if ( IS_CORRECT( ERR_LOOK_EQUIP_19 ) ) {
        kPacket.m_kItem.m_nCount = 0;
        goto END_PROC;
    }

    SET_ERROR( NET_OK );
    
END_PROC:
    
    kPacket.m_nOK = NetError::GetLastNetError();
    if ( kPacket.m_nOK != 0 ) {
        std::vector<KItem>::iterator vit;
        for ( vit = kPacket.m_vecItems.begin() ; vit != kPacket.m_vecItems.end() ; ++vit ) {
            // 룩변경된 아이템만 롤백
            if ( vit->CheckItemType(KItem::TYPE_LOOK) ) {
                LIF( m_kHelper.CoordiItemToNormalItem( dwUID_, vit->m_ItemUID, cCharType_ ) );
                vit->DeleteItemType(KItem::TYPE_LOOK);
            }
        }
        kPacket.m_vecItems.clear();
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L" 일반 아이템에서 룩변환 DB 처리 : " << NetError::GetLastNetErrMsg()
        << L", Name : " << strLogin_ 
        << L", OK : " << kPacket.m_nOK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_vecItem.size() )
        << BUILD_LOG( nChangeCount )
        << BUILD_LOG( kPacket.m_vecItems.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_CHANGE_NORMAL_TO_LOOK_ACK );

}

_INT_IMPL_ON_FUNC( EVENT_CHANGE_LOOK_TO_NORMAL_REQ, KChangeLookItem )
{

    KChangeLookItem kPacket;
    kPacket.m_vecItems.reserve( kPacket_.m_vecItems.size() );

    int nChangeCount = 0;
    std::vector<KItem>::iterator vit;
    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_CHANGE_LOOK_TO_NORMAL_REQ );

    for ( vit = kPacket_.m_vecItems.begin() ; vit != kPacket_.m_vecItems.end() ; ++vit ) {
        if ( !m_kHelper.CoordiItemToNormalItem( dwUID_, vit->m_ItemUID, cCharType_ ) ) {
            SET_ERR_GOTO( ERR_LOOK_EQUIP_07, END_PROC );
        }

        vit->DeleteItemType( KItem::TYPE_LOOK );
        kPacket.m_vecItems.push_back( *vit );

        ++nChangeCount;
    }

    SET_ERROR( NET_OK );

END_PROC:

    kPacket.m_nOK = NetError::GetLastNetError();
    if ( kPacket.m_nOK != 0 ) {
        std::vector<KItem>::iterator vit;
        for ( vit = kPacket.m_vecItems.begin() ; vit != kPacket.m_vecItems.end() ; ++vit ) {
            // 룩해제된 아이템만 롤백
            int nOK = 0;
            if ( false == vit->CheckItemType(KItem::TYPE_LOOK) ) {
                LIF( m_kHelper.NormalItemToCoordiItem( dwUID_, vit->m_ItemUID, cCharType_, nOK ) );
                vit->AddItemType( KItem::TYPE_LOOK );
            }
        }
        kPacket.m_vecItems.clear();
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L" 룩 아이템에서 일반 아이템으로 변환 DB 처리 : " << NetError::GetLastNetErrMsg()
        << L", Name : " << strLogin_ 
        << L", OK : " << kPacket.m_nOK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_vecItems.size() )
        << BUILD_LOG( nChangeCount )
        << BUILD_LOG( kPacket.m_vecItems.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_CHANGE_LOOK_TO_NORMAL_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_FULL_LOOK_INFO_REQ )
{
    KLookFullInfo kPacket;
    LoadFullLookInfo( dwUID_, kPacket );

    QUEUING_ACK_TO_USER( EVENT_FULL_LOOK_INFO_NOT );
}

INT_IMPL_ON_FUNC( DB_EVENT_BUNDLE_SELL_ITEM_REQ )
{
    KEVENT_BUNDLE_SELL_ITEM_ACK kPacket;
    kPacket.m_nGamePoint = kPacket_.m_kData.m_iGamePoint;
    std::map<GCITEMUID,KItem> mapRewards;
    std::map<GCITEMUID,KItem>::iterator mit;

    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( DB_EVENT_BUNDLE_SELL_ITEM_REQ );

    // 변경 정보 업데이트
    if( !UpdateUserData( kPacket_.m_kData ) ) {
        SET_ERR_GOTO( ERR_BUNDLE_SELL_09, END_PROC );
    }

    kPacket.m_mapFailInfo.swap( kPacket_.m_mapFailInfo );
    kPacket.m_vecEraseList.swap( kPacket_.m_vecEraseList );

    // 수량 아이템 변경 량 적용.
    BundleSellCountItemSync( dwUID_, cCharType_, kPacket_.m_vecSellCountItem, kPacket.m_mapFailInfo );

    // 삭제 아이템 삭제
    BundleSellEraseItems( dwUID_, kPacket.m_vecEraseList, kPacket.m_mapFailInfo, cCharType_ );

    // 수량 아이템 판매
    BundleSellCountItems( dwUID_, cCharType_, kPacket_.m_vecSellCountItem, kPacket.m_mapFailInfo, kPacket.m_nGamePoint, mapRewards, kPacket.m_vecCountItem, kPacket_.m_kData.m_dwSlotNum );

    // 영구 아이템 판매
    BundleSellNormalItems( dwUID_, cCharType_, kPacket_.m_vecSellNormalItem, kPacket.m_mapFailInfo, kPacket.m_nGamePoint, mapRewards, kPacket.m_vecNormalItem, kPacket_.m_kData.m_dwSlotNum );

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    if ( !mapRewards.empty() ) {
        kPacket.m_vecRewardItem.reserve( mapRewards.size() );
        std::map<GCITEMUID,KItem>::iterator mit;
        for ( mit = mapRewards.begin() ; mit != mapRewards.end() ; ++mit ) {
            kPacket.m_vecRewardItem.push_back( mit->second );
        }
    }

    QUEUING_ACK_TO_USER( EVENT_BUNDLE_SELL_ITEM_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_BUY_FOR_GAMBLE_REQ, KDB_EVENT_BUY_FOR_GAMBLE_REQ )
{
    EVENT_CHECK_OUT( EVENT_BUY_FOR_GAMBLE_REQ );

    KEVENT_BUY_FOR_GAMBLE_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nUseGP = 0;
    kPacket.m_vecItems.clear();

    float fGambleBuyRate = SiKGambleBuyManager()->GetCostRate(); // 겜블 배율정보 확인
    int nItemGrade = KItem::GRADE_NORMAL;
    std::wstring wstrItemName = L"";
    std::vector<KBuySellItemReq>::iterator vit;

    SET_ERROR( ERR_UNKNOWN );

    if ( !UpdateUserData( kPacket_.m_kData ) ) {
        START_LOG( cwarn, L"필수데이터 동기화 실패." ) << END_LOG;
        SET_ERR_GOTO( ERR_BUY_GAMBLE_01, END_PROC );
    }

    // 아이템 가격정보 확인
    for ( vit = kPacket_.m_vecBuyInfo.begin() ; vit != kPacket_.m_vecBuyInfo.end() ; ++vit ) {
        m_kHelper.GetPrice( vit->m_ItemID, vit->m_nCount, vit->m_nPeriod, vit->m_nPrice, vit->m_nMoneyType );

        if ( vit->m_nMoneyType != KItemManager::EMT_GAMEPOINT ) {
            START_LOG( cwarn, L"GP아이템이 아니다. Type : " << vit->m_nMoneyType ) << END_LOG;
            SET_ERR_GOTO( ERR_BUY_GAMBLE_02, END_PROC );
        }

        // GP지만, 겜블만 따로 구분하기 위해서 값을 따로 설정해준다.
        vit->m_nMoneyType = KItemManager::EWIT_GP_GAMBLE_BUY;

        kPacket.m_nUseGP += static_cast<int>(vit->m_nPrice * fGambleBuyRate);
        START_LOG( clog, L"아이템 가격정보 확인. ItemID : " << vit->m_ItemID << L",MoneyType : " << vit->m_nMoneyType << L",Price : " << vit->m_nPrice ) << END_LOG;
    }

    if ( kPacket_.m_kData.m_iGamePoint < kPacket.m_nUseGP ) {
        START_LOG( cwarn, L"잔액 부족. 보유GP : " << kPacket_.m_kData.m_iGamePoint << L", 아이템총가격 : " << kPacket.m_nUseGP ) << END_LOG;
        SET_ERR_GOTO( ERR_BUY_GAMBLE_00, END_PROC );
    }

    for ( vit = kPacket_.m_vecBuyInfo.begin() ; vit != kPacket_.m_vecBuyInfo.end() ; ++vit ) {
        vit->m_nGrade = SiKGambleBuyManager()->GetGradeByLevel( vit->m_nCharLevel );
        START_LOG( clog, L"아이템 등급결정됨. ItemGrade : " << vit->m_nGrade << L",CharLevel : " << vit->m_nCharLevel ) << END_LOG;

        __int64 waitItemUID = 0;
        if ( m_kHelper.BuyItemToWait( dwUID_, strLogin_, *vit, std::wstring(L""), waitItemUID, wstrItemName ) != 0 ) {
            START_LOG( cwarn, L"아이템 넣기 DB작업 실패. ItemID : " << vit->m_ItemID ) << END_LOG;
            SET_ERR_GOTO( ERR_BUY_GAMBLE_03, END_PROC );
        }

        if ( waitItemUID == KItem::INVALID_ITEM ) {
            START_LOG( cerr, L"WaitItemUID가 0 이다." ) << END_LOG;
            SET_ERR_GOTO( ERR_BUY_GAMBLE_03, END_PROC );
        }

        m_kHelper.UpdateWaitItemToEnabled( waitItemUID );

        if ( m_kHelper.InsertSingleWaitItemToInvenOrVDepot( dwUID_, cCharType_, waitItemUID, KItemManager::EWIT_GP_GAMBLE_BUY, kPacket.m_vecItems ) == false ) {
            START_LOG( cwarn, L"인벤토리에 아이템 삽입 DB작업 실패. ItemID : " << vit->m_ItemID << L",waitItemUID : " << waitItemUID ) << END_LOG;
            SET_ERR_GOTO( ERR_BUY_GAMBLE_03, END_PROC );
        }
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_BUY_FOR_GAMBLE_ACK );

    _LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_kData.m_iGamePoint )
        << BUILD_LOG( kPacket.m_nUseGP )
        << BUILD_LOG( kPacket.m_vecItems.size() )
        << BUILD_LOG( kPacket_.m_vecBuyInfo.size() ) << END_LOG;
}

INT_IMPL_ON_FUNC( DB_CHANNELING_EVENT_CHECK )
{
    std::vector<KDropItemInfo> vecItems;
    vecItems.clear();

    // 일일 지급 보상 아이템 받았는지 체크.
    if( GetTodayEventCount( dwUID_, UET_CHANNELING_DAILY_EVENT ) > 0 ) {
        START_LOG( clog, L"금일 채널링 일일 보상 받았음 Login " << strLogin_ ) << END_LOG;
    } else {
        if( SiKChannelTypeEvent()->GetDailyConnectionReward( kPacket_.m_dwUserChannelType, vecItems ) ) {
            // 보상이 있을 경우에만 세팅한다.
            LIF( SetTodayEventDoneDiff( dwUID_, UET_CHANNELING_DAILY_EVENT ) );
        }
    }

    // 이벤트 기간내 한번만 지급 보상 아이템 받았는지 체크.
    time_t tmDate;
    SiKChannelTypeEvent()->GetBeginTime( tmDate );
    int nData = GetTodayEventCount( dwUID_, UET_CHANNELING_ONE_EVENT , tmDate );
    if( 0 == nData ) {
        if( SiKChannelTypeEvent()->GetOneConnectionReward( kPacket_.m_dwUserChannelType, vecItems ) ) {
            LIF( SetTodayEventDoneDiff( dwUID_, UET_CHANNELING_ONE_EVENT, tmDate, 1 ) );
        }
    } else {
        START_LOG( clog, L"금일 채널링 일일 보상 받았음 Login " << strLogin_ ) << END_LOG;
    }

    // 신규 유저 보상 이벤트에 대한 체크.
    if( SiKChannelTypeEvent()->IsNewUserCheck( kPacket_.m_tmFirstLoginTime ) ) {
        time_t tmDate;
        SiKChannelTypeEvent()->GetNewUserCheckTime( tmDate );
        if( SiKChannelTypeEvent()->GetNewConnectionReward( kPacket_.m_dwUserChannelType, vecItems ) ) {
            LIF( SetTodayEventDoneDiff( dwUID_, UET_CHANNELING_NEWUSER_EVENT, tmDate, 1 ) );
        }
    }

    if( !vecItems.empty() ) {
        std::vector<KItem> vecRewards;
        vecRewards.reserve( vecItems.size() );

        std::vector<KDropItemInfo>::iterator vit;
        for( vit = vecItems.begin() ; vit != vecItems.end() ; ++vit ) {
            m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_EVENT_CHANNELING_ITEM, vecRewards );
        }

        _QUEUING_ACK_TO_USER( EVENT_CHANNELING_REWARD_NOT, vecRewards );
    }

    START_LOG( clog, L"유저에게 채널링 이벤트 아이템 지급 Size : " << vecItems.size() )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket_.m_dwUserChannelType ) << END_LOG;
}


INT_IMPL_ON_FUNC_NOPARAM( EVENT_USER_HERO_DUNGEON_INFO_REQ )
{
    // 유저의 영웅던전 플레이 데이터 가져오기
    KEVENT_USER_HERO_DUNGEON_INFO_ACK kPacket;
    LIF( m_kHelper.GetUserHeroDungeonInfo( dwUID_, cCharType_, kPacket ) );

    CTime tmToday( CTime::GetCurrentTime() );
    // 클리어 갱신
    std::map< int, KHeroPlayData >::iterator mit;
    for ( mit = kPacket.begin() ; mit != kPacket.end() ; ++mit ) {
        CTime tmPlayDate( mit->second.m_tmPlayDate );
        // 클리어 날짜 체크
        if ( tmPlayDate.GetYear() < tmToday.GetYear() ||
            (tmPlayDate.GetYear() <= tmToday.GetYear() && tmPlayDate.GetMonth() < tmToday.GetMonth()) ||
            (tmPlayDate.GetYear() <= tmToday.GetYear() && tmPlayDate.GetMonth() <= tmToday.GetMonth() && tmPlayDate.GetDay() < tmToday.GetDay()) ) {
            // 이전 날짜면 초기화
            mit->second.m_nPlayIndex = 0;
            mit->second.m_ClearCount = 0;
            // DB 갱신
            LIF( 0 == m_kHelper.UpdateUserHeroDungeonInfo( dwUID_, cCharType_, mit->first, mit->second.m_nPlayIndex, mit->second.m_ClearCount ) );
        }
    }

    QUEUING_ACK_TO_USER( EVENT_USER_HERO_DUNGEON_INFO_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_USER_HERO_DUNGEON_INFO_UPDATE_NOT )
{
    // 유저의 영웅던전 플레이 데이터 DB에 갱신.
    int nRet = m_kHelper.UpdateUserHeroDungeonInfo( dwUID_, cCharType_, kPacket_.m_nModeID, kPacket_.m_nPlayIndex, kPacket_.m_nClearCount );

    _LOG_SUCCESS( nRet == 0, L"Update User Hero Dungeon Info.. Result : " << nRet )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket_.m_nModeID )
        << BUILD_LOG( kPacket_.m_nPlayIndex )
        << BUILD_LOG( kPacket_.m_nClearCount ) << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_BUY_FOR_HERO_REQ, KDB_EVENT_BUY_FOR_HERO_REQ )
{
    KEVENT_BUY_FOR_HERO_ACK kPacket;
    kPacket.m_vecPeriodMaterialItems = kPacket_.m_vecDelMaterials;
    kPacket.m_prKey = std::make_pair( kPacket_.m_kCatalog.m_Index, kPacket_.m_kCatalog.m_ItemID );
    std::vector<GCITEMUID>::iterator vit;
    std::vector< KItem >::iterator vitReward;
    std::vector<KDropItemInfo>::iterator vitMaterials;
    std::map< GCITEMID, KItem >::iterator mit;

    std::vector<KItem> vecChangedItemList; // 변환된 아이템 List
    vecChangedItemList.clear();
    std::vector<KItem>::iterator vitChange;
    __int64 biWaitUID = 0;

    bool bLookItem = kPacket_.m_kCatalog.CheckItemFlag( KManufactureItem::IF_LOOK );
    int nOK = 0;

    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_BUY_FOR_HERO_REQ );

    if ( false == m_kHelper.InsertSingleItemJob_WaitUID( dwUID_, cCharType_, kPacket_.m_kCatalog.m_ItemID, kPacket_.m_kCatalog.m_nDuration, kPacket_.m_kCatalog.m_nPeriod, KItemManager::EWIT_HEROITEM, kPacket.m_vecRewards, biWaitUID ) ) {
        START_LOG( cerr, L"영웅포인트 아이템 삽입 실패 Name : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_kCatalog.m_ItemID )
            << BUILD_LOG( kPacket_.m_kCatalog.m_nDuration )
            << BUILD_LOG( kPacket_.m_kCatalog.m_nPeriod ) << END_LOG;
        SET_ERR_GOTO( ERR_HERO_POINT_04, END_PROC );
    }
    else {
        // 사용된 기간제 아이템 제거
        for( vit = kPacket_.m_vecDelMaterials.begin() ; vit != kPacket_.m_vecDelMaterials.end() ; ++vit ) {
            LIF( PureEraseItem( strLogin_, dwUID_, *vit, cCharType_ ) );
        }

        if ( bLookItem ) {
            for ( vitReward = kPacket.m_vecRewards.begin() ; vitReward != kPacket.m_vecRewards.end() ; ++vitReward ) {
                if( vitReward->m_cItemType == KItem::TYPE_LOOK ) {
                    continue;
                }

                // 룩 아이템 변환
                if ( !m_kHelper.NormalItemToCoordiItem( dwUID_, vitReward->m_ItemUID, cCharType_, nOK ) ) {
                    START_LOG( cwarn, L"DB작업중 룩 아이템 변환 실패. OK : " << nOK << L"ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
                    SET_ERR_GOTO( ERR_HERO_POINT_05, END_PROC );
                }

                vitReward->AddItemType(KItem::TYPE_LOOK);
                vecChangedItemList.push_back( *vitReward );
                START_LOG( clog, L"룩 아이템으로 변환됨. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
            }
        }
    }

    for( vitMaterials = kPacket_.m_vecMaterials.begin(); vitMaterials != kPacket_.m_vecMaterials.end(); ++vitMaterials ) {
        mit = kPacket_.m_mapKItem.find( vitMaterials->m_ItemID );
        if( mit == kPacket_.m_mapKItem.end() ) {
            continue;
        }
        // 동기화전 InitCount, Count값 변동값 있는지 체크.
        if( !CheckItemDuration( dwUID_, mit->second.m_ItemUID, mit->second.m_nInitCount, mit->second.m_nCount ) ) {
            START_LOG( clog, L"영웅던전 샵 구매시 영웅주화 창고 수량 동기화. Name : " << strLogin_ ) 
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( mit->second.m_ItemUID )
                << BUILD_LOG( mit->second.m_nInitCount )
                << BUILD_LOG( mit->second.m_nCount )
                << END_LOG;
        }

        if ( !UseCountItem( dwUID_, mit->second.m_ItemUID, vitMaterials->m_nDuration ) ) { // 사용 갯수 감소.
            mit->second.m_nCount = 0;
            START_LOG( cerr, L"영웅던전 샵 재료 아이템 동기화 실패. Name : " << strLogin_ ) << END_LOG;
        } else {
            mit->second.m_nCount -= vitMaterials->m_nDuration;
            mit->second.m_nCount = std::max<int>( mit->second.m_nCount, 0 );
        }
    }

    kPacket.m_vecMaterials = kPacket_.m_vecMaterials;
    kPacket.m_mapKItem = kPacket_.m_mapKItem;

    m_kHelper.UpdateMoneyConsumptionStat( dwUID_, KItemManager::EMC_HEROITEM, kPacket_.m_kCatalog.m_nDuration, biWaitUID );

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    if ( kPacket.m_nOK != NetError::NET_OK && bLookItem ) {
        for ( vitChange = vecChangedItemList.begin() ; vitChange != vecChangedItemList.end() ; ++vitChange ) {
            if ( m_kHelper.CoordiItemToNormalItem( dwUID_, vitChange->m_ItemUID, cCharType_ ) ) {
                vitChange->DeleteItemType(KItem::TYPE_LOOK);
                START_LOG( cwarn, L"DB작업중 룩변환 실패시, 일반아이템으로 롤백. ItemID : " << vitChange->m_ItemID << L",ItemUID : " << vitChange->m_ItemUID ) << END_LOG;
            }
        }
    }

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_kCatalog.m_Index )
        << BUILD_LOG( kPacket_.m_kCatalog.m_ItemID )
        << BUILD_LOG( bLookItem )
        << BUILD_LOG( kPacket.m_vecRewards.size() )
        << BUILD_LOG( vecChangedItemList.size() )
        << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_BUY_FOR_HERO_ACK );
}

_INT_IMPL_ON_FUNC( DB_EVENT_STAT_LOADING_INFO, KLoadingInfo )
{
    std::map< std::pair< DWORD, int >, int >::iterator mitSecond;
    std::map< std::pair< DWORD, DWORD>, int >::iterator mitMap;
    std::map< std::pair< DWORD, DWORD >, int >::iterator mitTotalUser;
    std::map< std::pair< int, DWORD >, int >::iterator mitFailInfo;

    if( !kPacket_ .mapTypeSecond.empty() ) {
        for( mitSecond = kPacket_ .mapTypeSecond.begin(); mitSecond != kPacket_ .mapTypeSecond.end(); ++mitSecond ) {
            UpdateLoadingInfoStat( 1, (int)mitSecond->first.first, mitSecond->first.second, mitSecond->second );
        }
    }

    if( !kPacket_.mapTypeMapID.empty() ) {
        for( mitMap = kPacket_ .mapTypeMapID.begin(); mitMap != kPacket_ .mapTypeMapID.end(); ++mitMap ) {
            UpdateLoadingInfoStat( 2, (int)mitMap->first.first, (int)mitMap->first.second, mitMap->second );
        }
    }

    if( !kPacket_.mapTypeTotalUser.empty() ) {
        for( mitTotalUser = kPacket_ .mapTypeTotalUser.begin(); mitTotalUser != kPacket_ .mapTypeTotalUser.end(); ++mitTotalUser ) {
            UpdateLoadingInfoStat( 3, (int)mitTotalUser->first.first, (int)mitTotalUser->first.second, mitTotalUser->second );
        }
    }

    if( !kPacket_.mapFailTypeInfo.empty() ) {
        for( mitFailInfo = kPacket_ .mapFailTypeInfo.begin(); mitFailInfo != kPacket_ .mapFailTypeInfo.end(); ++mitFailInfo ) {
            UpdateLoadingInfoStat( 4, mitFailInfo->first.first, (int)mitFailInfo->first.second, mitFailInfo->second );
        }
    }
}

INT_IMPL_ON_FUNC( EVENT_EXP_UPDATE_TO_DB_NOT )
{
    LIF( UpdateCharacterInfo( strLogin_, dwUID_, kPacket_ ) );
}

INT_IMPL_ON_FUNC( EVENT_PVEXP_UPDATE_TO_DB_NOT )
{
    LIF( UpdatePVExpInfo( dwUID_ ) );
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_GAWIBAWIBO_INFO_REQ )
{ // 유저의 가위바위보 정보 가져오기
    KUserGawibawiboInfo kPacket;
    DWORD dwVersion = SiKGawibawibo()->GetVersion();

    GetUserGawibawiboInfo( dwUID_, dwVersion, kPacket );
    int nData = CheckGawibawiboInit( dwUID_ );

    if ( nData == 0 ) { // 초기화 할 수 있는 상태 -> 초기화 하기
        InitGawibawiboPlayLog( dwUID_ );
        kPacket.m_usReceiveCount = 0;
    }

    kPacket.m_usVersion = static_cast<USHORT>(dwVersion);
    kPacket.m_usResult = 0;

    START_LOG( clog, L"접속시 가위바위보 정보 DB에서 가져오기" << strLogin_ )
        << BUILD_LOG( nData )
        << BUILD_LOG( dwVersion ) << END_LOG;
    
    QUEUING_ACK_TO_USER( DB_EVENT_GAWIBAWIBO_INFO_ACK );
}

_INT_IMPL_ON_FUNC( DB_EVENT_GAWIBAWIBO_INFO_UPDATE, KUserGawibawiboInfo )
{ // 유저의 가위바위보 정보 저장하기
    EVENT_CHECK_OUT( DB_EVENT_GAWIBAWIBO_INFO_UPDATE );

    int nOK = -99;
    nOK = UpdateUserGawibawiboInfo( dwUID_, kPacket_ );

    KEVENT_GAWIBAWIBO_ACK kPacket;

    if ( nOK != 0 ) {
        SET_ERR_GOTO( ERR_GAWIBAWIBO_03, END_PROC );
    }
    
    // 아이템 보상
    if ( !kPacket_.m_vecReward.empty() ) {
        
        std::vector<KDropItemInfo>::iterator vit;
        for ( vit = kPacket_.m_vecReward.begin(); vit != kPacket_.m_vecReward.end(); ++vit ) {
            
            if ( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_GAWIBAWIBO_REWARD, kPacket.m_vecItem ) ) {
                START_LOG( cerr, L"가위바위보 아이템 지급 실패 Name : " << strLogin_ )
                    << BUILD_LOG( vit->m_ItemID )
                    << BUILD_LOG( vit->m_nDuration )
                    << BUILD_LOG( vit->m_nPeriod ) << END_LOG;
                SET_ERR_GOTO( ERR_GAWIBAWIBO_04, END_PROC );
            }
        }
    }

    // 아이템 보상 기록
    if ( !kPacket.m_vecItem.empty() ) {
        std::vector<KItem>::iterator vitItem;
        for ( vitItem = kPacket.m_vecItem.begin(); vitItem != kPacket.m_vecItem.end(); ++vitItem ) {
            GawibawiboItemInsertLog( dwUID_, vitItem->m_ItemID, vitItem->m_nCount, vitItem->m_nPeriod, kPacket_.m_usVersion );
        }
    }

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    kPacket.m_kUserGawibawiboInfo = kPacket_;
    QUEUING_ACK_TO_USER( EVENT_GAWIBAWIBO_ACK );

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( dwUID_ ) << END_LOG;
}

_INT_IMPL_ON_FUNC( DB_EVENT_GAWIBAWIBO_INIT_INFO_REQ, KUserGawibawiboInfo )
{
    // 초기화 기록
    InitGawibawiboPlayLog( dwUID_  );

    // 오늘의 받은 도전 포인트 초기화
    UpdateUserGawibawiboInfo( dwUID_, kPacket_ );
}

_INT_IMPL_ON_FUNC( EVENT_STAT_LOGINOUT_COUNT, KCountInfo )
{
    KODBC* pkODBC;
    pkODBC = &m_kODBC;

    int nServerID = SiKGameServer()->GetServerID();

    if( pkODBC ) {
        // Login, Logout, LoginMigration, LogoutMigration, zombieout stat
        JIF( m_kHelper.GetCommonSP().UpdateChannelCCU( KLoginOutStat::LOGIN, KUser::USER_CT_TOTAL, nServerID, kPacket_.m_nLoginCount ) );
        JIF( m_kHelper.GetCommonSP().UpdateChannelCCU( KLoginOutStat::LOGOUT, KUser::USER_CT_TOTAL, nServerID, kPacket_.m_nLogoutCount ) );
        JIF( m_kHelper.GetCommonSP().UpdateChannelCCU( KLoginOutStat::LOGIN_MIGRATION, KUser::USER_CT_TOTAL, nServerID, kPacket_.m_nServerInCount ) );
        JIF( m_kHelper.GetCommonSP().UpdateChannelCCU( KLoginOutStat::LOGOUT_MIGRATION, KUser::USER_CT_TOTAL, nServerID, kPacket_.m_nServerOutCount ) );
        JIF( m_kHelper.GetCommonSP().UpdateChannelCCU( KLoginOutStat::ZOMBIE_OUT, KUser::USER_CT_TOTAL, nServerID, kPacket_.m_nZombieOut ) );
        JIF( m_kHelper.GetCommonSP().UpdateChannelCCU( KLoginOutStat::LOGIN_RECONNECT, KUser::USER_CT_TOTAL, nServerID, kPacket_.m_nServerReConnectCount ) );
    }
}

INT_IMPL_ON_FUNC( EVENT_INIT_ITEM_GP_ATTRIBUTE_REQ )
{
    EVENT_CHECK_OUT( EVENT_INIT_ITEM_GP_ATTRIBUTE_REQ );
    KEVENT_INIT_ITEM_GP_ATTRIBUTE_ACK kPacket;
    kPacket.m_nOK = -99;

    KItem kItem = kPacket_.m_kItem;
    kItem.m_vecAttribute.clear();

    bool bUpdateSuccess = true;
    for( int i = 0 ; i < (int)kPacket_.m_kItem.m_vecAttribute.size() ; ++i )
    {
        LIF( bUpdateSuccess = bUpdateSuccess && m_kHelper.InitAttribute(dwUID_, i, kItem ) );
    }

    if( bUpdateSuccess == true ) {

        kPacket.m_nOK = NetError::NET_OK;
        kPacket.m_kItem = kItem;
        kPacket.m_kUseItem = kPacket_.m_kUseItem;
        kPacket.m_ItemID = kPacket_.m_ItemID;

        // 동기화전 InitCount, Count값 변동값 있는지 체크.
        if( !CheckItemDuration( dwUID_, kPacket.m_kUseItem.m_ItemUID, kPacket.m_kUseItem.m_nInitCount, kPacket.m_kUseItem.m_nCount ) ) {
            START_LOG( clog, L"GP 속성 초기화 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( kPacket.m_kUseItem.m_ItemUID )
                << BUILD_LOG( kPacket.m_kUseItem.m_nInitCount )
                << BUILD_LOG( kPacket.m_kUseItem.m_nCount )
                << END_LOG;
        }

        if ( UseCountItem( dwUID_, kPacket.m_kUseItem.m_ItemUID, 1 ) ) { // 성공
            kPacket.m_kUseItem.m_nCount -= 1;
            kPacket.m_kUseItem.m_nCount = std::max<int>( kPacket.m_kUseItem.m_nCount, 0 );
        }
        else { // 실패
            kPacket.m_kUseItem.m_nCount = 0;
            kPacket.m_nOK = NetError::ERR_ATTRIBUTE_INIT_06;            
        }
    }
    else {
        kPacket.m_nOK = NetError::ERR_ATTRIBUTE_INIT_04;
    }

    QUEUING_ACK_TO_USER_UID( EVENT_INIT_ITEM_GP_ATTRIBUTE_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_SOCKS_INFO_REQ )
{
    KUserSocksInfo kPacket;
    // GetUserSocksInfo( kPacket );
    std::vector< GCITEMID > vecSocks;
    SiKSocks()->GetSocksList( vecSocks );

    std::vector< GCITEMID >::const_iterator vit;
    for ( vit = vecSocks.begin(); vit != vecSocks.end() ; ++vit ) {
        KSocksInfo socksInfo;
        socksInfo.Clear();

        socksInfo.m_Socks.m_ItemID = *vit;
        SelectSocksInfo( dwUID_, *vit, socksInfo.m_nIsHangup, socksInfo.m_dwRemainSec );

        if ( socksInfo.m_nIsHangup != KSocksInfo::HANGUP ) { // 걸린 양말 정보만 준다
            continue;
        }
        
        if ( kPacket.insert( std::make_pair( socksInfo.m_Socks.m_ItemID, socksInfo ) ).second == false ) {
            START_LOG( cerr, L"양말 정보 DB에서 가져와서 맵에 넣기 실패" ) << END_LOG;
        }
    }
    QUEUING_ACK_TO_USER( DB_EVENT_SOCKS_INFO_ACK );
}

_INT_IMPL_ON_FUNC( DB_EVENT_SOCKS_INFO_UPDATE, KUserSocksInfo )
{
    KSocksResult kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_kSocksInfo = kPacket_;

    std::map< GCITEMID, KSocksInfo >::iterator mit;

    for ( mit = kPacket_.begin(); mit != kPacket_.end() ; ++mit ) {
        int nOK = -99;
        nOK = SocksHangup( dwUID_, mit->first, mit->second.m_dwRemainSec );

        if ( nOK == 0 ) { // 정상으로 처리되는게 하나라도 있다면 ok 로 보내겠다
            kPacket.m_nOK = 0;
        }
        else {
            mit->second.Clear();
            //mit->second.m_nIsHangup = KSocksInfo::HANGUP;
        }

        _LOG_SUCCESS( nOK == 0, L"양말 DB 업데이트, UserUID: " << dwUID_ )
            << BUILD_LOG( nOK )
            << BUILD_LOG( mit->first )
            << BUILD_LOG( mit->second.m_dwRemainSec )
            << BUILD_LOG( mit->second.m_nIsHangup ) << END_LOG;
    }

    QUEUING_ACK_TO_USER( EVENT_SOCKS_HANGUP_COMPLETE_NOT );
}

_INT_IMPL_ON_FUNC( DB_EVENT_SOCKS_HANGUP_REQ, KUserSocksInfo )
{
    EVENT_CHECK_OUT( DB_EVENT_SOCKS_HANGUP_REQ );
    KSocksResult kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_kSocksInfo = kPacket_;

    std::map< GCITEMID, KSocksInfo >::iterator mit;

    for ( mit = kPacket.m_kSocksInfo.begin(); mit != kPacket.m_kSocksInfo.end() ; ++mit ) {
        int nOK = -99;
        nOK = SocksHangup( dwUID_, mit->first, mit->second.m_dwRemainSec );

        if ( nOK == 0 ) { // 정상으로 처리되는게 하나라도 있다면 ok 로 보내겠다
            kPacket.m_nOK = 0;
        }
        else {
            mit->second.Clear();
        }

        _LOG_SUCCESS( nOK == 0, L"양말걸기 DB기록" << dwUID_ )
            << BUILD_LOG( nOK )
            << BUILD_LOG( mit->first )
            << BUILD_LOG( mit->second.m_dwRemainSec ) << END_LOG;
    }
    _QUEUING_ACK_TO_USER( EVENT_SOCKS_HANGUP_ACK, kPacket );
}

_INT_IMPL_ON_FUNC( DB_EVENT_SOCKS_COLLECT_REQ, KUserSocksInfo )
{
    EVENT_CHECK_OUT( DB_EVENT_SOCKS_COLLECT_REQ );
    KSocksResult kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_kSocksInfo = kPacket_;

    std::map< GCITEMID, KSocksInfo >::iterator mit;

    for ( mit = kPacket.m_kSocksInfo.begin(); mit != kPacket.m_kSocksInfo.end() ; ++mit ) {
        int nOK = -99;
        nOK = SocksCollect( dwUID_, mit->first, mit->second.m_dwRemainSec );

        if ( nOK == 0 ) { // 정상으로 처리되는게 하나라도 있다면 ok 로 보내겠다
            kPacket.m_nOK = 0;
            
            std::vector<KDropItemInfo>::iterator vit;
            for ( vit = mit->second.m_vecItemList.begin(); vit != mit->second.m_vecItemList.end(); ++vit ) {
                if ( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_SOCKS_REWARD, kPacket.m_vecRewardItem ) ) {
                    START_LOG( cerr, L"양말 보상 아이템 지급 실패 Name : " << strLogin_ )
                        << BUILD_LOG( vit->m_ItemID )
                        << BUILD_LOG( vit->m_nDuration )
                        << BUILD_LOG( vit->m_nPeriod ) << END_LOG;
                }
            }
        }
        else {
            mit->second.Clear();
        }

        _LOG_SUCCESS( nOK == 0, L"양말걸기 DB 업데이트 " << dwUID_ )
            << BUILD_LOG( nOK )
            << BUILD_LOG( mit->first )
            << BUILD_LOG( mit->second.m_dwRemainSec )
            << BUILD_LOG( kPacket.m_vecRewardItem.size() ) << END_LOG;
    }

    _QUEUING_ACK_TO_USER( EVENT_SOCKS_COLLECT_ACK, kPacket );
}

_INT_IMPL_ON_FUNC( DB_EVENT_SOCKS_INSERT_ITEM, KEVENT_SOCKS_MATERIAL_EXCHANGE_NOT )
{
    KEVENT_SOCKS_MATERIAL_EXCHANGE_NOT kPacket;
    //KSocksResult kPacket;
    
    KEVENT_SOCKS_MATERIAL_EXCHANGE_NOT::iterator mit;
    for ( mit = kPacket_.begin(); mit != kPacket_.end(); ++mit ) { // 유저별 반복자

        std::vector< KDropItemInfo >::const_iterator vit;
        std::vector<KItem> vecItem;

        // 아이템 별 반복
        for ( vit = mit->second.m_vecExchangeSocks.begin(); vit != mit->second.m_vecExchangeSocks.end(); ++vit ) {
            if ( false == m_kHelper.InsertSingleItemJob( mit->first, 
                cCharType_,
                vit->m_ItemID, 
                vit->m_nDuration, 
                vit->m_nPeriod, 
                KItemManager::EWIT_SOCKS_REWARD, 
                vecItem ) ) {

                START_LOG( cerr, L"양말 아이템 지급 실패 UID : " << mit->first )
                    << BUILD_LOG( vit->m_ItemID )
                    << BUILD_LOG( vit->m_nDuration )
                    << BUILD_LOG( vit->m_nPeriod ) << END_LOG;
                
                continue;
            }
        }
        kPacket[mit->first].m_vecReward = vecItem;
        kPacket[mit->first].m_vecUseItem = mit->second.m_vecUseItem;
    }

    QUEUING_ACK_TO_USER( EVENT_SOCKS_MATERIAL_EXCHANGE_NOT );
}
_INT_IMPL_ON_FUNC( DB_EVENT_SOCKS_INSERT_ITEM_NEW, KSocksExchangeData )
{
    std::vector< KDropItemInfo >::const_iterator vit;
    std::vector<KItem> vecItem;
    KSocksExchangeData kPacket;

    // 아이템 별 반복
    for ( vit = kPacket_.m_vecExchangeSocks.begin(); vit != kPacket_.m_vecExchangeSocks.end(); ++vit ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_,
            cCharType_,
            vit->m_ItemID, 
            vit->m_nDuration, 
            vit->m_nPeriod, 
            KItemManager::EWIT_SOCKS_REWARD, 
            vecItem ) ) {

            START_LOG( cerr, L"양말 아이템 지급 실패 UID : " << dwUID_ )
                << BUILD_LOG( vit->m_ItemID )
                << BUILD_LOG( vit->m_nDuration )
                << BUILD_LOG( vit->m_nPeriod ) << END_LOG;

            continue;
        }
    }
    kPacket.m_nOK = kPacket_.m_nOK;
    kPacket.m_vecReward = vecItem;
    kPacket.m_vecUseItem = kPacket_.m_vecUseItem;

    QUEUING_ACK_TO_USER( EVENT_SOCKS_MATERIAL_EXCHANGE_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_SPECIAL_REWARD_REQ, KRewardInfoList )
{
    KEVENT_SPECIAL_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_vecSpecialReward = kPacket_;

    EVENT_CHECK_OUT( EVENT_SPECIAL_REWARD_REQ );
    SET_ERROR( NET_OK ); // 성공을 전제로 작업 시작

    std::vector<KRewardInfo>::iterator vitUser;
    for ( vitUser = kPacket.m_vecSpecialReward.begin() ; vitUser != kPacket.m_vecSpecialReward.end() ; ++vitUser ) {

        std::set<USHORT>::const_iterator csit;
        //std::Info << L"UserUID : " << vitUser->m_dwUID << L", Item size : " << vitUser->m_vecItemList.size() << L", Selct Index : ";
        //for ( csit = vitUser->m_setSelectIndex.begin() ; csit != vitUser->m_setSelectIndex.end() ; ++csit ) {
        //    std::Info << *csit << L", ";
        //}
        //std::Info << dbg::endl;

        USHORT usBoxIndex = 0;
        std::vector<KRewardBox>::iterator vitItem;
        for ( vitItem = vitUser->m_vecItemList.begin() ; vitItem != vitUser->m_vecItemList.end() ; ++vitItem, ++usBoxIndex ) {

            // 유저가 선택한 보상인가?
            std::set<USHORT>::iterator sit = vitUser->m_setSelectIndex.find( usBoxIndex );
            if ( sit == vitUser->m_setSelectIndex.end() ) {
                START_LOG( clog, L"유저가 선택한 보상이 아니다. Index : " << usBoxIndex << L", BoxType : " << vitItem->m_usBoxType << L", ItemID : " << vitItem->m_kRewardItem.m_ItemID ) << END_LOG;
                continue;
            }

            if ( vitItem->m_nRewardGP > 0 ) {
                vitUser->m_nDropGP += vitItem->m_nRewardGP;
                START_LOG( clog, L"유저 선택 보상GP 지급됨. Index : " << usBoxIndex << L", BoxType : " << vitItem->m_usBoxType << L", GP : " << vitItem->m_nRewardGP << L", All GP : " << vitUser->m_nDropGP ) << END_LOG;
            }

            if ( vitItem->m_kRewardItem.m_ItemID <= 0 ) {
                START_LOG( clog, L"유저가 선택했지만 꽝인 보상. Index : " << usBoxIndex << L", BoxType : " << vitItem->m_usBoxType ) << END_LOG;
                continue;
            }

            // 아이템 넣기.
            if ( !m_kHelper.InsertSingleItemJob( vitUser->m_dwUID, vitUser->m_cCharType, vitItem->m_kRewardItem.m_ItemID, vitItem->m_kRewardItem.m_nDuration, vitItem->m_kRewardItem.m_nPeriod, KItemManager::EWIT_GAME_SPECAIL_DROP, vitUser->m_vecRewardItem ) ) {
                START_LOG( cerr, L" Special Reward Item DB insert Fail.. UserUID : " << vitUser->m_dwUID )
                    << BUILD_LOG( vitItem->m_kRewardItem.m_ItemID )
                    << BUILD_LOG( vitItem->m_kRewardItem.m_nDuration )
                    << BUILD_LOG( vitItem->m_kRewardItem.m_nPeriod )
                    << BUILD_LOGc( vitUser->m_cCharType )
                    << END_LOG;

                // 아이템 넣기 실패할 경우, 해당 Drop은 꽝처리
                vitItem->m_usBoxType = 0;
                vitItem->m_kRewardItem.m_ItemID = 0;
                vitItem->m_kRewardItem.m_nDuration = -1;
                vitItem->m_kRewardItem.m_nPeriod = -1;

                SET_ERROR( ERR_SPECIAL_REWARD_04 ); // DB처리 실패시 에러값만 설정.
            }
            START_LOG( clog, L"유저 선택 보상Item 지급됨. Index : " << usBoxIndex << L", BoxType : " << vitItem->m_usBoxType << L", ItemID : " << vitItem->m_kRewardItem.m_ItemID ) << END_LOG;
        }
    }

    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.size() )
        << BUILD_LOG( kPacket.m_vecSpecialReward.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_SPECIAL_REWARD_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_DUNGEON_REWARD_ITEM_REQ, KDB_EVENT_DUNGEON_REWARD_ITEM_REQ )
{
    DWORD& dwRecvUserUID = kPacket_.m_prUserDrops.first;
    KDropItemInfo& kDropItem = kPacket_.m_prUserDrops.second;
    char cReceiveCharType;
    if( kPacket_.m_cReceiveCharType != -1 ) {
        cReceiveCharType = kPacket_.m_cReceiveCharType;
    } else {
        cReceiveCharType = kDropItem.m_cCharType;
    }

    KEVENT_DUNGEON_REWARD_ITEM_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwRecvUserUID = dwRecvUserUID;
    kPacket.m_vecDropItem.clear();
    kPacket.m_mapUserDice.clear();
    kPacket.m_vecPartyDropItem.clear();

    SET_ERROR( ERR_UNKNOWN );

    if ( false == kPacket_.m_vecPrPartyDrops.empty() ) {
        std::vector< std::pair<DWORD,KDropItemInfo> >::iterator vitPartyDrops;
        for ( vitPartyDrops = kPacket_.m_vecPrPartyDrops.begin(); vitPartyDrops != kPacket_.m_vecPrPartyDrops.end(); vitPartyDrops++ ) {
            std::vector<KItem> kRetItem;
            kRetItem.clear();
            // 아이템 넣어주기.
            if ( false == m_kHelper.InsertSingleItemJob( vitPartyDrops->first, cCharType_, vitPartyDrops->second.m_ItemID, vitPartyDrops->second.m_nDuration, vitPartyDrops->second.m_nPeriod, KItemManager::EWIT_GAME_DROP, kRetItem ) ) {
                START_LOG( cerr, L" Game Drop DB insert Fail.. UserUID : " << dwRecvUserUID )
                    << BUILD_LOG( vitPartyDrops->second.m_ItemID )
                    << BUILD_LOG( vitPartyDrops->second.m_nDuration )
                    << BUILD_LOG( vitPartyDrops->second.m_nPeriod ) << END_LOG;
                SET_ERR_GOTO( ERR_REAL_TIME_REWARD_02, END_PROC );
            }

            kPacket.m_vecPartyDropItem.push_back( std::make_pair( vitPartyDrops->first, kRetItem ) );
        }
    }
    else {
        // 미국 이벤트
        if ( kDropItem.m_ItemID == KUser::GC_MEDAL_ID ) {
            int nGCMedalCount = kDropItem.m_nDuration;

            if ( SiKGSSimLayer()->m_bUSEvent && nGCMedalCount > 0 ) {
                int nOK = -99;

                DBJIF( m_kODBC.Query( L"exec dbo.GMW_grandchase_medal %d", nGCMedalCount ) );
                if ( m_kODBC.BeginFetch() ) {
                    VERIFY_QUERY( m_kODBC >> nOK );
                }
                m_kODBC.EndFetch();

                _LOG_SUCCESS( nOK == 0, L"Ret : " << nOK )
                    << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
            }
        }

        // 스노우코인 이벤트
        if ( SiKGCHelper()->ProcessSnowCoin( kDropItem.m_ItemID, kDropItem.m_nDuration, kPacket.m_vecDropItem ) ) {
            InsertSnowCoin( dwRecvUserUID, kDropItem.m_nDuration );
            START_LOG( clog, L"스노우코인 지급됨. UserUID : " << dwRecvUserUID )
                << BUILD_LOG( kDropItem.m_ItemID )
                << BUILD_LOG( kDropItem.m_nDuration )
                << BUILD_LOG( kDropItem.m_nPeriod ) << END_LOG;
        }
        else {
            // 아이템 넣어주기.
            if ( false == m_kHelper.InsertSingleItemJob( dwRecvUserUID, cReceiveCharType, kDropItem.m_ItemID, kDropItem.m_nDuration, kDropItem.m_nPeriod, KItemManager::EWIT_GAME_DROP, kPacket.m_vecDropItem ) ) {
                START_LOG( cerr, L" Game Drop DB insert Fail.. UserUID : " << dwRecvUserUID )
                    << BUILD_LOG( kDropItem.m_ItemID )
                    << BUILD_LOG( kDropItem.m_nDuration )
                    << BUILD_LOG( kDropItem.m_nPeriod ) << END_LOG;
                SET_ERR_GOTO( ERR_REAL_TIME_REWARD_02, END_PROC );
            }

            START_LOG( clog, L"Game Drop 아이템 지급됨. UserUID : " << dwRecvUserUID )
                << BUILD_LOG( kDropItem.m_ItemID )
                << BUILD_LOG( kDropItem.m_nDuration )
                << BUILD_LOG( kDropItem.m_nPeriod ) << END_LOG;
        }

        // 룰렛 아이템 주사위값 지정
        SetRandomDiceNum( dwRecvUserUID, kPacket_.m_prRouletteList.second, kPacket.m_mapUserDice );

        // 더블 크리스탈 카드 사용시, 정보 갱신되도록 넣어준다.
        if ( kPacket_.m_kDubleCrystalCard.m_ItemID > 0 ) {
            kPacket.m_kDubleCrystalCard = kPacket_.m_kDubleCrystalCard;
        }
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_dwRecvUserUID )
        << BUILD_LOGc( kPacket_.m_cReceiveCharType )
        << BUILD_LOGc( cReceiveCharType )
        << BUILD_LOG( kDropItem.m_ItemID )
        << BUILD_LOG( kDropItem.m_nDuration )
        << BUILD_LOG( kDropItem.m_nPeriod )
        << BUILD_LOG( kPacket.m_vecDropItem.size() )
        << BUILD_LOG( kPacket_.m_prRouletteList.second.size() )
        << BUILD_LOG( kPacket.m_mapUserDice.size() )
        << BUILD_LOG( kPacket_.m_kDubleCrystalCard.m_ItemID ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_DUNGEON_REWARD_ITEM_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_CHAT_NOT, KChatData )
{
    // 길드 채팅일 경우, 이곳을 경유한다.
    DWORD dwGuildUID = kPacket_.m_iParam;

    KGuildPtr spGuild = SiKGuildManager()->GetGuild( dwGuildUID );
    if ( spGuild == NULL ) {
        UpdateOneGuildInfo( dwGuildUID );
    }

    SiKGuildManager()->BroadChat( dwGuildUID, kPacket_ );
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_INIT_DICE_DAILY_FREE )
{
    InitDiceDailyFree( UET_DICE_EVENT );
}

INT_IMPL_ON_FUNC( DB_EVENT_ITEM_ATTRIBUTE_RANDOM_SELECT_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_ITEM_ATTRIBUTE_RANDOM_SELECT_REQ );
    KEVENT_ITEM_ATTRIBUTE_RANDOM_SELECT_ACK kPacket;
    kPacket.m_nOK = -99;
    std::list<std::pair<int,float>>::iterator litAtt;

    KItem kItem = kPacket_.m_kItem;
    kItem.m_vecAttribute.clear();


    // 아이템 랜덤 속성 적용.
    bool bUpdateSuccess = true;
    int i;
    for( i = 0, litAtt = kPacket_.m_listAttributeList.begin() ; litAtt != kPacket_.m_listAttributeList.end() ; ++litAtt, ++i )
    {
        LIF( bUpdateSuccess = bUpdateSuccess && m_kHelper.UpdateAttribute(dwUID_, i, litAtt->first, litAtt->second, kItem, cCharType_ ) );
    }

    if( bUpdateSuccess == false ) {
        kPacket.m_nOK = NetError::ERR_RANDOM_ATTRIBUTE_SELECT_03;
    }
    else {
        kPacket.m_nOK = NetError::NET_OK;
        kPacket.m_kItem = kItem;
        kPacket.m_ItemID = kPacket_.m_ItemID;
        kPacket.m_bInitItem = kPacket_.m_bInitItem;
        kPacket.m_kUseItem = kPacket_.m_kUseItem;

        if ( kPacket_.m_bInitItem == false ) {
            // 동기화전 InitCount, Count값 변동값 있는지 체크.
            if( !CheckItemDuration( dwUID_, kPacket_.m_kUseItem.m_ItemUID, kPacket_.m_kUseItem.m_nInitCount, kPacket_.m_kUseItem.m_nCount ) ) {
                START_LOG( clog, L"랜덤속성 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
                    << BUILD_LOG( dwUID_ )
                    << BUILD_LOG( kPacket_.m_kUseItem.m_ItemUID )
                    << BUILD_LOG( kPacket_.m_kUseItem.m_nInitCount )
                    << BUILD_LOG( kPacket_.m_kUseItem.m_nCount )
                    << END_LOG;
            }

            if ( !UseCountItem( dwUID_, kPacket_.m_kUseItem.m_ItemUID, 1 ) ) {
                kPacket.m_kUseItem.m_nCount = 0;
                kPacket.m_nOK = NetError::ERR_RANDOM_ATTRIBUTE_SELECT_05;
            }
            else {
                kPacket.m_kUseItem.m_nCount -= 1;
                kPacket.m_kUseItem.m_nCount = std::max<int>( kPacket.m_kUseItem.m_nCount, 0 );
            }
        }
    }

    QUEUING_ACK_TO_USER( EVENT_ITEM_ATTRIBUTE_RANDOM_SELECT_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_TODAYS_POPUP_INFO_REQ )
{
    int kPacket = GetTodayEventCount( dwUID_, UDT_NICKNAME_EVENT );
    QUEUING_ACK_TO_USER( EVENT_TODAYS_POPUP_INFO_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_DISABLE_TODAYS_POPUP_NOT )
{
    LIF( SetTodayEventDone( dwUID_, UDT_NICKNAME_EVENT, KncUtil::TimeToInt(CTime::GetCurrentTime()), 1 ) );
}

_INT_IMPL_ON_FUNC( EVENT_WHISPER_REQ, KChatData )
{
    KEVENT_WHISPER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_kChatData = kPacket_;

    // 대상 유저의 UserUID 구하기
    if ( false == m_kHelper.NickToUserUID( kPacket.m_kChatData.m_strReceiverNick, kPacket.m_kChatData.m_dwReceiverUID ) ) {
        SET_ERR_GOTO( ERR_CHAT_12, END_PROC );
    }
    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_CONDITION( kPacket.m_nOK == NetError::NET_OK, clog, cwarn, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_kChatData.m_strReceiverNick )
        << BUILD_LOG( kPacket.m_kChatData.m_dwReceiverUID )
        << BUILD_LOG( kPacket.m_kChatData.m_dwSenderUID )
        << BUILD_LOG( kPacket.m_kChatData.m_strSenderNick )
        << BUILD_LOG( kPacket.m_kChatData.m_strChatMsg ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_WHISPER_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_FASHION_CATALOG_REQ )
{
    KEVENT_FASHION_CATALOG_ACK kPacket;
    m_kHelper.GetFashionCatalog( kPacket );
    QUEUING_ACK_TO_USER( EVENT_FASHION_CATALOG_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_USER_SERVER_ROOM_REQ, std::wstring )
{
    // kPacket_ : 대상 유저의 NickName.
    std::wstring& strNick = kPacket_;

    KEVENT_USER_SERVER_ROOM_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_kServerRoomInfo.m_dwUserUID = 0;

    // 대상 유저의 UserUID 구하기
    if ( false == m_kHelper.NickToUserUID( strNick, kPacket.m_kServerRoomInfo.m_dwUserUID ) ) {
        SET_ERR_GOTO( ERR_SERVER_ROOM_03, END_PROC );
    }
    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_CONDITION( kPacket.m_nOK == NetError::NET_OK, clog, cwarn, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strNick )
        << BUILD_LOG( kPacket.m_kServerRoomInfo.m_dwUserUID ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_USER_SERVER_ROOM_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_USER_RECOM_TYPE_REQ, int )
{
    int kPacket = kPacket_;
    LIF( m_kHelper.FriendFriendUserType_select( dwUID_, kPacket, SiKRecomHelper()->GetVersion() ) );
    QUEUING_ACK_TO_USER( EVENT_USER_RECOM_TYPE_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_UPDATE_CHARACTER_STAT_NOT )
{
    LIF( KSP( m_kODBCStat ).SkillCount_merge( kPacket_ ) );
    START_LOG( clog, L"DB_EVENT_UPDATE_CHARACTER_STAT_NOT size : " << kPacket_.size() ) << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_PLANT_TREE_INFO_REQ )
{
    EVENT_CHECK_OUT( EVENT_PLANT_TREE_INFO_REQ );
    SET_ERROR( ERR_UNKNOWN );
    KEVENT_PLANT_TREE_INFO_ACK kPacket;
    kPacket.m_nOK = -99;
    DWORD dwVersion = SiKPlantTree()->GetPlantTreeVersion();

    GetPlantTreePosition( dwUID_, dwVersion, kPacket.m_dwContinentType, kPacket.m_dwCurrentPosition, kPacket.m_dwTreeStatus );
    SiKPlantTree()->CheckTreeStatus( kPacket.m_dwTreeStatus ); // 나무 상태값 보정.
    SET_ERROR( NET_OK );

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_PLANT_TREE_INFO_ACK );
}

INT_IMPL_ON_FUNC( EVENT_PLANT_TREE_ACTION_REQ )
{
    EVENT_CHECK_OUT( EVENT_PLANT_TREE_ACTION_REQ );
    SET_ERROR( ERR_UNKNOWN );
    KEVENT_PLANT_TREE_ACTION_ACK kPacket;
    kPacket.m_nOK = -99;
    DWORD dwVersion = SiKPlantTree()->GetPlantTreeVersion();

    int nOK = SetPlantTreePosition( dwUID_, dwVersion, kPacket_.m_dwContinentType, kPacket_.m_dwCurrentPosition, kPacket_.m_dwTreeStatus );

    if( kPacket_.m_dwActionType != KEVENT_PLANT_TREE_ACTION_REQ::AT_PLANT_TREE ) {
        // 동기화전 InitCount, Count값 변동값 있는지 체크.
        if( !CheckItemDuration( dwUID_, kPacket_.m_kCoinItem.m_ItemUID, kPacket_.m_kCoinItem.m_nInitCount, kPacket_.m_kCoinItem.m_nCount ) ) {
            START_LOG( clog, L"나무나무 코인 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( kPacket_.m_kCoinItem.m_ItemUID )
                << BUILD_LOG( kPacket_.m_kCoinItem.m_nInitCount )
                << BUILD_LOG( kPacket_.m_kCoinItem.m_nCount )
                << END_LOG;
        }

        if ( !UseCountItem( dwUID_, kPacket_.m_kCoinItem.m_ItemUID, 1 ) ) { // 코인 갯수 감소.
            START_LOG( cerr, L"나무나무 코인 개수 동기화 실패. Name : " << strLogin_ ) << END_LOG;
            SET_ERROR( ERR_PLANT_TREE_12 );
        } else {
            kPacket_.m_kCoinItem.m_nCount -= 1;
            kPacket_.m_kCoinItem.m_nCount = std::max<int>( kPacket_.m_kCoinItem.m_nCount, 0 );
            SET_ERROR( NET_OK );
        }
    } else {
        SET_ERROR( NET_OK );
    }

    kPacket.m_kCoinItem = kPacket_.m_kCoinItem;

    if ( IS_CORRECT( ERR_PLANT_TREE_12 ) ) {
        kPacket.m_kCoinItem.m_nCount = 0;
    }

    kPacket.m_nOK = NetError::GetLastNetError();
    kPacket.m_dwContinentType = kPacket_.m_dwContinentType;
    kPacket.m_dwCurrentPosition = kPacket_.m_dwCurrentPosition;
    kPacket.m_dwTreeStatus = kPacket_.m_dwTreeStatus;
    kPacket.m_dwItemID = kPacket_.m_dwItemID;
    kPacket.m_dwActionType = kPacket_.m_dwActionType;
    kPacket.m_vecItem.clear();

    _LOG_SUCCESS( nOK == 0, L"나무나무 이벤트 유저 정보 DB 업데이트, UserUID: " << dwUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( kPacket.m_dwItemID )
        << BUILD_LOG( kPacket.m_dwContinentType )
        << BUILD_LOG( kPacket.m_dwCurrentPosition )
        << BUILD_LOG( kPacket.m_dwTreeStatus ) << END_LOG;

    QUEUING_ACK_TO_USER_UID( EVENT_PLANT_TREE_ACTION_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_PLANT_TREE_REWARD_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_PLANT_TREE_REWARD_REQ );
    KEVENT_PLANT_TREE_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;
    std::vector< KDropItemInfo >::const_iterator vit;
    std::vector<KItem> vecItem;
    vecItem.clear();
    DWORD dwVersion = SiKPlantTree()->GetPlantTreeVersion();
    SET_ERROR( ERR_UNKNOWN );

    // 아이템 별 반복
    for ( vit = kPacket_.m_vecGoalReward.begin(); vit != kPacket_.m_vecGoalReward.end(); ++vit ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, 
            cCharType_,
            vit->m_ItemID, 
            vit->m_nDuration, 
            vit->m_nPeriod, 
            KItemManager::EWIT_PLANTTREE_REWARD, 
            vecItem ) ) {

                START_LOG( cerr, L"나무나무 보상 아이템 지급 실패 UID : " << dwUID_ )
                    << BUILD_LOG( vit->m_ItemID )
                    << BUILD_LOG( vit->m_nDuration )
                    << BUILD_LOG( vit->m_nPeriod ) << END_LOG;

                continue;
        }
    }
    SET_ERROR( NET_OK );
    kPacket.m_nOK = NetError::GetLastNetError();
    kPacket.m_dwRewardType = kPacket_.m_dwRewardType;
    kPacket.m_vecRewardItem = vecItem;
    kPacket.m_dwContinentType = kPacket_.m_dwContinentType;
    kPacket.m_dwCurrentPosition = kPacket_.m_dwCurrentPosition;
    kPacket.m_dwTreeStatus = kPacket_.m_dwTreeStatus;

    int nOK = SetPlantTreePosition( dwUID_, dwVersion, kPacket_.m_dwContinentType, kPacket_.m_dwCurrentPosition, kPacket_.m_dwTreeStatus );

    _LOG_SUCCESS( nOK == 0, L"나무나무 이벤트 유저 정보 DB 업데이트, UserUID: " << dwUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( kPacket.m_dwRewardType )
        << BUILD_LOG( kPacket.m_dwContinentType )
        << BUILD_LOG( kPacket.m_dwCurrentPosition )
        << BUILD_LOG( kPacket.m_dwTreeStatus )
        << BUILD_LOG( kPacket.m_vecRewardItem.size() ) << END_LOG;

    QUEUING_ACK_TO_USER_UID( EVENT_PLANT_TREE_REWARD_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_BUY_FOR_GUILD_REQ, KDB_EVENT_BUY_FOR_GUILD_REQ )
{
    KEVENT_BUY_FOR_GUILD_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_prKey = std::make_pair( kPacket_.m_kCatalog.m_Index, kPacket_.m_kCatalog.m_ItemID );
    std::vector< KItem >::iterator vitReward;
    std::vector<KItem> vecChangedItemList; // 변환된 아이템 List
    vecChangedItemList.clear();
    std::vector<KItem>::iterator vitChange;

    bool bLookItem = kPacket_.m_kCatalog.CheckItemFlag( KManufactureItem::IF_LOOK );
    int nOK = 0;

    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_BUY_FOR_GUILD_REQ );

    // 일일 구매제한
    if ( false == CheckBuyLimitItem( dwUID_, kPacket_.m_nTabID ) ) {
        SET_ERR_GOTO( ERR_GUILD_STORE_08, END_PROC );
    }

    if ( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, kPacket_.m_kCatalog.m_ItemID, kPacket_.m_kCatalog.m_nDuration, kPacket_.m_kCatalog.m_nPeriod, KItemManager::EWIT_HEROITEM, kPacket.m_vecRewards ) ) {
        START_LOG( cerr, L"길드상점 아이템 삽입 실패 Name : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_kCatalog.m_ItemID )
            << BUILD_LOG( kPacket_.m_kCatalog.m_nDuration )
            << BUILD_LOG( kPacket_.m_kCatalog.m_nPeriod ) << END_LOG;
        SET_ERR_GOTO( ERR_GUILD_STORE_10, END_PROC );
    }
    else {
        if ( bLookItem ) {
            for ( vitReward = kPacket.m_vecRewards.begin() ; vitReward != kPacket.m_vecRewards.end() ; ++vitReward ) {
                if( vitReward->m_cItemType == KItem::TYPE_LOOK ) {
                    continue;
                }

                // 룩 아이템 변환
                if ( !m_kHelper.NormalItemToCoordiItem( dwUID_, vitReward->m_ItemUID, cCharType_, nOK ) ) {
                    START_LOG( cwarn, L"DB작업중 룩 아이템 변환 실패. OK : " << nOK << L"ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
                    SET_ERR_GOTO( ERR_GUILD_STORE_11, END_PROC );
                }
                vitReward->AddItemType(KItem::TYPE_LOOK);
                vecChangedItemList.push_back( *vitReward );
                START_LOG( clog, L"룩 아이템으로 변환됨. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
            }
        }
    }

    // 일일 구매기록
    CommitBuyLimitItem( dwUID_, kPacket_.m_nTabID );
    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    if ( kPacket.m_nOK != NetError::NET_OK && bLookItem ) {
        for ( vitChange = vecChangedItemList.begin() ; vitChange != vecChangedItemList.end() ; ++vitChange ) {
            if ( m_kHelper.CoordiItemToNormalItem( dwUID_, vitChange->m_ItemUID, cCharType_ ) ) {
                vitChange->DeleteItemType(KItem::TYPE_LOOK);
                START_LOG( cwarn, L"DB작업중 룩변환 실패시, 일반아이템으로 롤백. ItemID : " << vitChange->m_ItemID << L",ItemUID : " << vitChange->m_ItemUID ) << END_LOG;
            }
        }
    }

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK || kPacket.m_nOK == NetError::ERR_GUILD_STORE_08, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_nTabID )
        << BUILD_LOG( kPacket_.m_kCatalog.m_Index )
        << BUILD_LOG( kPacket_.m_kCatalog.m_ItemID )
        << BUILD_LOG( bLookItem )
        << BUILD_LOG( kPacket.m_vecRewards.size() )
        << BUILD_LOG( vecChangedItemList.size() )
        << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_BUY_FOR_GUILD_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( DB_UPDATE_GUILD_LIST_REQ )
{
    std::map< DWORD, KNGuildInfo > mapGuildList;
    // 전체 길드 정보 받아오기
    if ( GetAllGuildInfo( mapGuildList ) == false ) {
        START_LOG( cerr, L"DB에서 전체 길드 정보를 읽어오지 못했습니다." ) << END_LOG;
        return;
    }// end if

    SiKGuildManager()->AddGuild( mapGuildList );

    START_LOG( cerr, L"DB에서 길드 정보 받아옴." )
        << BUILD_LOG( mapGuildList.size() ) << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_SQUARE_LIST_REQ )
{
    KNGuildUserInfo kPacket;
    GetUserGuildInfo( dwUID_, kPacket );
    QUEUING_ACK_TO_USER_UID( EVENT_SQUARE_LIST_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_CHOICE_BOX_OPEN_REQ, KDB_EVENT_CHOICE_BOX_OPEN_REQ )
{
    // 초이스 드랍박스 열기
    EVENT_CHECK_OUT( EVENT_CHOICE_BOX_OPEN_REQ );
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_CHOICE_BOX_OPEN_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_kBoxItem.m_ItemID = kPacket_.m_BoxItemID;
    kPacket.m_vecItem.clear();
    bool bRet = true;

    std::vector<KDropItemInfo>::const_iterator cvit;
    for ( cvit = kPacket_.m_vecReward.begin() ; cvit != kPacket_.m_vecReward.end() ; ++cvit ) {
        // DB로그 쓰기
        LIF( m_kHelper.ChoiceDrop_insert( dwUID_, kPacket.m_kBoxItem.m_ItemID, cvit->m_ItemID ) );

        // 아이템 넣어주기.
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_,
                cCharType_,
                cvit->m_ItemID,
                cvit->m_nDuration,
                cvit->m_nPeriod,
                KItemManager::EWIT_CHOICE_DROP,
                kPacket.m_vecItem,
                cvit->m_cGradeID ) ) {
            START_LOG( cerr, L" Choice Drop DB insert Fail.. UserUID : " << dwUID_ )
                << BUILD_LOG( cvit->m_ItemID )
                << BUILD_LOG( cvit->m_nDuration )
                << BUILD_LOG( cvit->m_nPeriod )
                << BUILD_LOGc( cvit->m_cGradeID )
                << END_LOG;
            bRet = false;
            continue;
        }
    }

    if ( false == bRet && kPacket.m_vecItem.empty() ) {
        SET_ERR_GOTO( ERR_CHOICE_DROP_08, END_PROC );
    }

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_kBoxItem.m_ItemUID, kPacket_.m_kBoxItem.m_nInitCount, kPacket_.m_kBoxItem.m_nCount ) ) {
        START_LOG( clog, L"초이스 드랍 박스 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_kBoxItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_kBoxItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_kBoxItem.m_nCount )
            << END_LOG;
    }

    if ( !UseCountItem( dwUID_, kPacket_.m_kBoxItem.m_ItemUID, 1 ) ) { // 상자 갯수 감소.
        START_LOG( cerr, L"초이스 드랍 박스 개수 동기화 실패. Name : " << strLogin_ ) << END_LOG;
        SET_ERROR( ERR_CHOICE_DROP_09 );
    } else {
        SET_ERROR( NET_OK );
    }

    kPacket_.m_kBoxItem.m_nCount -= 1;
    kPacket_.m_kBoxItem.m_nCount = std::max<int>( kPacket_.m_kBoxItem.m_nCount, 0 );
    kPacket.m_kBoxItem = kPacket_.m_kBoxItem;

    if ( IS_CORRECT( ERR_CHOICE_DROP_09 ) ) {
        kPacket.m_kBoxItem.m_nCount = 0;
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket_.m_BoxItemID )
        << BUILD_LOG( kPacket_.m_vecReward.size() )
        << BUILD_LOG( kPacket.m_vecItem.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_CHOICE_BOX_OPEN_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_UNLOCK_CHANGE_WEAPON_REQ, char )
{
    KEVENT_UNLOCK_CHANGE_WEAPON_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_cCharType = kPacket_;
    EVENT_CHECK_OUT( EVENT_UNLOCK_CHANGE_WEAPON_REQ );

    bool bUpdateSuccess = true;
    LIF( bUpdateSuccess = bUpdateSuccess && m_kHelper.UpdateChangeWeaponInfoToDB( dwUID_, kPacket_, true, 0 ) );

    if( bUpdateSuccess == false ) {
        kPacket.m_nOK = NetError::ERR_CHANGE_WEAPON_04;
    }
    else {
        kPacket.m_nOK = NetError::NET_OK;
    }

    QUEUING_ACK_TO_USER( EVENT_UNLOCK_CHANGE_WEAPON_ACK );
}

INT_IMPL_ON_FUNC( EVENT_USER_AUTH_CHECK_REQ )
{
    EVENT_CHECK_OUT( EVENT_USER_AUTH_CHECK_REQ );
    KEVENT_USER_AUTH_CHECK_ACK kPacket;
    kPacket.m_nOK = -99;

    KVirtualCashInfo kVirtualCashInfo;
    kVirtualCashInfo.m_nOK = 0;
    kVirtualCashInfo.m_dwCashPoint = 0;
    kVirtualCashInfo.m_dwDeductCashPoint = 0;

    int nGetItemCheck = -1;
    int nVirtualCash = -1;
    int nItemCheck = -1;
    int nVirtualCashCheck = -1;
    std::vector<KDropItemInfo> vecRewardItem;
    std::vector<KDropItemInfo>::iterator vit;
    std::vector<KItem> vecItem;
    vecItem.clear();
    SET_ERROR( ERR_UNKNOWN );

    // 유니코드로 변경.
    std::wstring wstrLogin = KncUtil::toWideString( kPacket_.m_strLogin );
    std::wstring wstrNickName = KncUtil::toWideString( kPacket_.m_strNickName );
    std::wstring wstrBirthDay = KncUtil::toWideString( kPacket_.m_strBirthDay );
    std::wstring wstrEmail = KncUtil::toWideString( kPacket_.m_strEmail );

    // 지급 대상인지에 대한 체크.
    nGetItemCheck = CheckUserAuthCheck( KGCHelper::RT_ITEM, wstrLogin, wstrNickName, wstrBirthDay, wstrLogin );
    // 가상 캐시체크는 Account값을 하나 더 넣는다. 미국에서 Email정보가 넘어오지 않았다.
    nVirtualCash  = CheckUserAuthCheck( KGCHelper::RT_VIRTUAL, wstrLogin, wstrNickName, wstrBirthDay, wstrEmail );

    // 아이템 보상을 받았는지에 대한 체크
    if( nGetItemCheck > 0 && nGetItemCheck != 2 ) { // 아이템 보상을 받아야 하는 유저이면..,
        InsertVirtualCashLog( dwUID_, wstrLogin, KGCHelper::RT_ITEM ); // 가상 캐시 받아간 로그 추가.
        SiKGCHelper()->GetUserRewardItem( vecRewardItem );
        // 아이템 별 반복
        for ( vit = vecRewardItem.begin(); vit != vecRewardItem.end(); ++vit ) {
            if ( false == m_kHelper.InsertSingleItemJob( dwUID_, 
                cCharType_,
                vit->m_ItemID, 
                vit->m_nDuration, 
                vit->m_nPeriod, 
                KItemManager::EWIT_LAUSERAUTH_REWARD, 
                vecItem ) ) {

                    START_LOG( cerr, L"남미 미국 유저 인증 보상 실패 UID : " << dwUID_ )
                        << BUILD_LOG( vit->m_ItemID )
                        << BUILD_LOG( vit->m_nDuration )
                        << BUILD_LOG( vit->m_nPeriod ) << END_LOG;

                    continue;
            }
        }
    }

    // 가상 캐시 보상을 받았는지에 대한 체크.
    if( nVirtualCash > 0 && nVirtualCash != 2 ) { // 아이템 보상을 받아야 하는 유저이면..,
        InsertVirtualCashLog( dwUID_, wstrLogin, KGCHelper::RT_VIRTUAL ); // 가상 캐시 받아간 로그 추가.
        m_kHelper.VirtualCashMergeWithType( dwUID_, 0, KVirtualCash::EVCMT_ADD_LA_US_AUTH_CHECK, nVirtualCash, 0 );
        int nBalance = m_kHelper.GetCurrentVirtualCash( dwUID_, strLogin_ );
        kVirtualCashInfo.m_dwCashPoint = nBalance;
    }

    if( 0 == nGetItemCheck && 0 == nVirtualCash ) { // 지급대상이 아님.
        SET_ERR_GOTO( ERR_USER_AUTH_01, END_PROC );
    }

    if( ( 2 == nGetItemCheck && 2 == nVirtualCash ) || 
            ( 0 == nGetItemCheck && 2 == nVirtualCash ) || 
                ( 2 == nGetItemCheck && 0 == nVirtualCash ) ) { // 둘다 이미 지급받은 유저.
        SET_ERR_GOTO( ERR_USER_AUTH_02, END_PROC );
    }

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    kPacket.m_kVirtualCashInfo = kVirtualCashInfo;
    kPacket.m_vecRewardItem = vecItem;

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kVirtualCashInfo.m_dwCashPoint )
        << BUILD_LOG( kPacket.m_vecRewardItem.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_USER_AUTH_CHECK_ACK );
}

INT_IMPL_ON_FUNC( EVENT_ITEM_LEVEL_SELECT_REQ )
{
    EVENT_CHECK_OUT( EVENT_ITEM_LEVEL_SELECT_REQ );
    KEVENT_ITEM_LEVEL_SELECT_ACK kPacket;
    kPacket.m_nOK = -99;

    bool bUpdateSuccess = true;
    LIF( bUpdateSuccess = bUpdateSuccess && m_kHelper.UpdateLevel(dwUID_, kPacket_.m_nLevel, KItemLevelTable::IL_EQUIP_LV, kPacket_.m_kItem, cCharType_ ) );

    if( bUpdateSuccess == false ) {
        kPacket.m_nOK = NetError::ERR_ITEMLEVEL_SELECT_04;
    }
    else {
        kPacket.m_nOK = NetError::NET_OK;
        kPacket.m_kItem = kPacket_.m_kItem;
    }

    QUEUING_ACK_TO_USER( EVENT_ITEM_LEVEL_SELECT_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_VIPEVENT_INFO_REQ )
{
    std::map< USHORT, std::map< USHORT, USHORT > > mapRewardByVer;
    mapRewardByVer.clear();
    SiKVipEventManager()->GetRewardByVer( mapRewardByVer );

    std::map<USHORT,KVipEvent> mapInfo;
    JIF( m_kHelper.VIPCustomerInfo_select( mapInfo, mapRewardByVer ) );
    SiKVipEventManager()->UpdateEvent( mapInfo );
}

_INT_IMPL_ON_FUNC( EVENT_VIPEVENT_USER_INFO_REQ, USHORT )
{
    KEVENT_VIPEVENT_USER_INFO_ACK kPacket;
    kPacket.m_usVersion = kPacket_;
    kPacket.m_nPoint = 0;
    kPacket.m_mapReward.clear();

    LIF( m_kHelper.VIPCustomerPoint_select( dwUID_, kPacket.m_usVersion, kPacket.m_nPoint ) );
    LIF( m_kHelper.VIPCustomerReward_select( dwUID_, kPacket.m_usVersion, kPacket.m_mapReward ) );

    START_LOG( clog, L"VIP event Info.. UserUID : " << dwUID_ )
        << BUILD_LOG( kPacket.m_usVersion )
        << BUILD_LOG( kPacket.m_nPoint )
        << BUILD_LOG( kPacket.m_mapReward.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_VIPEVENT_USER_INFO_ACK );
}

_INT_IMPL_ON_FUNC( DB_EVENT_VIPEVENT_USER_REWARD_UPDATE_NOT, time_t )
{
    const time_t& tmFirstLogin = kPacket_;

    // 획득한 보상 아이템리스트
    KEVENT_VIPEVENT_USER_REWARD_UPDATE_NOT kPacket;

    // 버전별 이벤트 정보
    std::map<USHORT,KVipEvent> mapList;
    SiKVipEventManager()->GetEventList( mapList );

    // 금액ID별 보상아이템리스트
    std::map<USHORT,std::vector<KDropItemInfo> > mapRewardItem;

    // 버전별 이벤트 유저보상 확인
    std::map<USHORT,KVipEvent>::iterator mit;
    for ( mit = mapList.begin() ; mit != mapList.end() ; ++mit ) {
        const USHORT& usVer = mit->first;
        KVipEvent& kInfo = mit->second;
        std::map<int,bool> mapRewardCheck;
        int nPoint = 0;
        mapRewardItem.clear();

        // 진행중이거나 지나간 이벤트만 처리.
        if ( !SiKVipEventManager()->IsActive( kInfo.m_prEventRange ) && !SiKVipEventManager()->IsEnd( kInfo.m_prEventRange ) ) {
            continue;
        }

        // 사용한 금액, 받은 보상정보 받아오기
        LIF( m_kHelper.VIPCustomerPoint_select( dwUID_, usVer, nPoint ) );
        LIF( m_kHelper.VIPCustomerReward_select( dwUID_, usVer, mapRewardCheck ) );

        // 신규보상 대상자인가?
        bool bNewbieRewardUsre = CheckEventNewbieReward( kInfo, nPoint, tmFirstLogin, mapRewardCheck );
        if ( bNewbieRewardUsre ) {
            std::set<USHORT> setPayIndex;
            setPayIndex.insert( KVipEventManager::NEWBIE_REWARD );
            LIF( SiKVipEventManager()->GetCurrentReward( usVer, KVipEventManager::NEWBIE_REWARD, setPayIndex, mapRewardItem ) );
        }

        // VIP 보상 대상자인가?
        std::set<USHORT> setPayTableID;
        bool bVipRewardUser = CheckEventVipReward( kInfo, nPoint, mapRewardCheck, setPayTableID );
        if ( bVipRewardUser ) {
            LIF( SiKVipEventManager()->GetCurrentReward( usVer, KVipEventManager::VIP_REWARD, setPayTableID, mapRewardItem ) );
        }

        // 보상 지급
        if ( false == mapRewardItem.empty() ) {
            InsertVipEventRewardItemToDB( dwUID_, cCharType_, usVer, mapRewardItem, kPacket );
        }
    }

    START_LOG( clog, L"VIP-event User Reward Update.. Login : " << strLogin_ )
        << BUILD_LOGtm( CTime( tmFirstLogin ) )
        << BUILD_LOG( mapList.size() )
        << BUILD_LOG( mapRewardItem.size() )
        << BUILD_LOG( kPacket.size() ) << END_LOG;

    if ( false == kPacket.empty() ) {
        QUEUING_ACK_TO_USER( EVENT_VIPEVENT_USER_REWARD_UPDATE_NOT );
    }
}

_INT_IMPL_ON_FUNC( DB_EVENT_MAGIC_BOX_LIST_NOT, USHORT )
{
    const USHORT& usVer = kPacket_;
    std::map<GCITEMID, std::pair<KDropItemInfo,int> > mapItemList;
    std::map<GCITEMID,float> mapLuckRatio;

    LIF( m_kHelper.MagicBox_select( usVer, mapItemList, mapLuckRatio ) );

    SiKMagicBox()->UpdateEvent( mapItemList, mapLuckRatio );
}

INT_IMPL_ON_FUNC_NOPARAM(EVENT_MAGIC_BOX_LIST_REQ)
{
    USHORT usVer = SiKMagicBox()->GetVer();
    KEVENT_MAGIC_BOX_LIST_ACK kPacket;
    kPacket.m_selectItemID = 0;
    kPacket.m_mapItemInfo.clear();

    LIF(m_kHelper.MagicBoxRegister_select(dwUID_, usVer, kPacket.m_selectItemID));
    std::map<GCITEMID, std::pair<int, int> > mapItemInfo;
    LIF(m_kHelper.MagicBoxStoneInfo_select(dwUID_, usVer, mapItemInfo));
    std::map<GCITEMID, std::pair<KDropItemInfo, int> > mapItemList;
    SiKMagicBox()->GetItemList(mapItemList);

    std::map<GCITEMID, std::pair<KDropItemInfo, int> >::iterator mit;
    for (mit = mapItemList.begin(); mit != mapItemList.end(); ++mit) {
        int& nNeedKeyCount = mit->second.second;
        int& nUseKeyCount = mapItemInfo[mit->first].first;
        int& nClearCount = mapItemInfo[mit->first].second;

        if (nNeedKeyCount <= 0) {
            START_LOG(cwarn, L"필요 키 개수가 0개 이하.. ItemID : " << mit->first) << END_LOG;
            continue;
        }

        float fRatio = SiKMagicBox()->CalcNeedKeyRatio(nClearCount);
        fRatio = std::max<float>(fRatio, 1.f);
        nNeedKeyCount = static_cast<int>(std::ceil(nNeedKeyCount * fRatio));
        nNeedKeyCount = std::max<int>(nNeedKeyCount, 1);
        int nLeftKeyCount = nNeedKeyCount - nUseKeyCount;
        nLeftKeyCount = std::max<int>(nLeftKeyCount, 1);

        kPacket.m_mapItemInfo[mit->first] = std::make_pair(nLeftKeyCount, nClearCount);
    }

    START_LOG(clog, L"두근두근 마법상자.. 유저 아이템 리스트정보  Login : " << strLogin_)
        << BUILD_LOG(usVer)
        << BUILD_LOG(kPacket.m_selectItemID)
        << BUILD_LOG(kPacket.m_mapItemInfo.size()) << END_LOG;

    QUEUING_ACK_TO_USER(EVENT_MAGIC_BOX_LIST_ACK);
}

_INT_IMPL_ON_FUNC( EVENT_SET_MAGIC_BOX_TARGET_REQ, GCITEMID )
{
    EVENT_CHECK_OUT( EVENT_SET_MAGIC_BOX_TARGET_REQ );
    SET_ERROR( ERR_UNKNOWN );

    USHORT usVer = SiKMagicBox()->GetVer();
    KEVENT_SET_MAGIC_BOX_TARGET_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_ItemID = kPacket_;

    // 메인 아이템 등록
    if ( false == m_kHelper.MagicBoxRegister_update( dwUID_, usVer, kPacket_ ) ) {
        SET_ERR_GOTO( ERR_MAGIC_BOX_02, END_PROC );
    }
    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_ItemID ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_SET_MAGIC_BOX_TARGET_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_USE_MAGIC_BOX_REQ, KDB_EVENT_USE_MAGIC_BOX_REQ )
{
    EVENT_CHECK_OUT( EVENT_USE_MAGIC_BOX_REQ );
    USHORT usVer = SiKMagicBox()->GetVer();

    KEVENT_USE_MAGIC_BOX_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_TargetItemID = kPacket_.m_TargetItemID;
    kPacket.m_nLeftKeyCount = kPacket_.m_nLeftKeyCount;
    kPacket.m_kKeyItem = kPacket_.m_kKeyItem;
    kPacket.m_nClearCount = kPacket_.m_nClearCount;
    kPacket.m_bClear = false;
    kPacket.m_vecItem.clear();

    // 사용된 키 수량 차이값
    int nUseKeyDiff = static_cast<int>( SiKMagicBox()->GetDecKeyCount() );

    // 키는 1개씩 깎는다.
    kPacket.m_nLeftKeyCount -= nUseKeyDiff;

    // 아이템 획득인가?
    if ( kPacket.m_nLeftKeyCount <= 0 ) {
        kPacket.m_bClear = true;
    }

    // 스페셜 키인가?
    if ( kPacket_.m_bIsSpecialKey ) {
        // 확률 확인
        float fRatio = 0.f;
        LIF( SiKMagicBox()->GetItemRatio( kPacket.m_TargetItemID, fRatio ) );

        // 확률로 성공여부 체크
        KLottery kLottery;
        kLottery.AddCase( 1, fRatio );
        int nRet = kLottery.Decision();

        // 럭키로 아이템 획득
        if ( nRet == 1 ) {
            kPacket.m_bClear = true;
        }
    }

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket.m_kKeyItem.m_ItemUID, kPacket.m_kKeyItem.m_nInitCount, kPacket.m_kKeyItem.m_nCount ) ) {
        START_LOG( clog, L"마법상자 키 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket.m_kKeyItem.m_ItemUID )
            << BUILD_LOG( kPacket.m_kKeyItem.m_nInitCount )
            << BUILD_LOG( kPacket.m_kKeyItem.m_nCount )
            << END_LOG;
    }

    if ( !UseCountItem( dwUID_, kPacket.m_kKeyItem.m_ItemUID, nUseKeyDiff ) ) {
        START_LOG( cerr, L"마법상자 키 아이템 DB 동기화 실패. Name : " << strLogin_ )
            << BUILD_LOG( kPacket.m_kKeyItem.m_ItemUID )
            << BUILD_LOG( nUseKeyDiff ) << END_LOG;
        SET_ERR_GOTO( ERR_MAGIC_BOX_07, END_PROC );
    }
    // UpdateCountItem 성공이면 수량 아이템을 소모한다
    kPacket.m_kKeyItem.m_nCount -= nUseKeyDiff;
    kPacket.m_kKeyItem.m_nCount = std::max<int>( kPacket.m_kKeyItem.m_nCount, 0 );

    // 메인아이템 잠금 깎기
    if ( false == m_kHelper.MagicBoxStoneInfo_update( dwUID_, usVer, kPacket.m_TargetItemID, nUseKeyDiff, kPacket.m_bClear ) ) {
        // DB작업 실패
        SET_ERR_GOTO( ERR_MAGIC_BOX_02, END_PROC );
    }

    // 아이템 획득이면 (여기서부턴 낙장불입)
    if ( kPacket.m_bClear ) {
        // 아이템 넣어주기.
        KDropItemInfo kItem;
        int nDefaultNeedKeyCount = -1;

        if ( false == SiKMagicBox()->GetItemInfo( kPacket.m_TargetItemID, kItem, nDefaultNeedKeyCount ) ) {
            START_LOG( cerr, L"획득할 아이템 정보가 없음. DB확인 필요.. Login : " << strLogin_ )
                << BUILD_LOG( kPacket.m_TargetItemID ) << END_LOG;
        }
        else {
            LIF( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, kItem.m_ItemID, kItem.m_nDuration, kItem.m_nPeriod, KItemManager::EWIT_MAGIC_BOX, kPacket.m_vecItem ) );
        }

        // 획득 회수 증가
        ++kPacket.m_nClearCount;
        // 필요한 키 수량 초기화.
        float fRatio = SiKMagicBox()->CalcNeedKeyRatio( kPacket.m_nClearCount );
        fRatio = std::max<float>( fRatio, 1.f );
        kPacket.m_nLeftKeyCount = static_cast<int>( std::ceil( nDefaultNeedKeyCount * fRatio ) );
        kPacket.m_nLeftKeyCount = std::max<int>( kPacket.m_nLeftKeyCount, 1 );
    }

    SET_ERROR( NET_OK );

END_PROC:

    if ( IS_CORRECT( ERR_MAGIC_BOX_07 ) ) {
        kPacket.m_kKeyItem.m_nCount = 0;
    }

    kPacket.m_nOK = NetError::GetLastNetError();
    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( nUseKeyDiff )
        << BUILD_LOG( kPacket.m_TargetItemID )
        << BUILD_LOG( kPacket.m_nLeftKeyCount )
        << BUILD_LOG( kPacket.m_kKeyItem.m_ItemID )
        << BUILD_LOG( kPacket.m_nClearCount )
        << BUILD_LOG( kPacket.m_bClear )
        << BUILD_LOG( kPacket.m_vecItem.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_USE_MAGIC_BOX_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_GET_PACKAGE_LIST_NOT )
{
    std::vector<std::map<GCITEMID,std::vector<KDropItemInfo > > > vecPackage;
    DWORD dwMaxPackageNum = SiKGSSimLayer()->GetMaxPackageNum();
    LIF( m_kHelper.PackageItem_select( dwMaxPackageNum, vecPackage ) );
    START_LOG( cerr, L"DB 패키지아이템 정보 받아옴.. size : " << vecPackage.size() ) << END_LOG;
    SiKItemManager()->UpdatePackageInfo( vecPackage );
}

INT_IMPL_ON_FUNC( EVENT_RKTORNADO_ACTION_REQ )
{
    EVENT_CHECK_OUT( EVENT_RKTORNADO_ACTION_REQ );
    KEVENT_RKTORNADO_ACTION_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    bool bRet = true;
    std::vector<KDropItemInfo>::const_iterator cvit;

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_CoinItem.m_ItemUID, kPacket_.m_CoinItem.m_nInitCount, kPacket_.m_CoinItem.m_nCount ) ) {
        START_LOG( clog, L"릭쿤 토네이도 코인 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_CoinItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_CoinItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_CoinItem.m_nCount )
            << END_LOG;
    }

    if ( !UseCountItem( dwUID_, kPacket_.m_CoinItem.m_ItemUID, 1 ) ) { // 코인 갯수 감소.
        START_LOG( cerr, L"릭쿤 토네이도 코인 개수 동기화 실패. Name : " << strLogin_ ) << END_LOG;
        SET_ERR_GOTO( ERR_RKTORNODO_07, END_PROC );
    }

    int nCount = static_cast<int>( SiKRKTornado()->GetFailItemCount() );
    if( KRKTornado::AT_NONFAIL == kPacket_.m_dwType ) { // 럭키 코인 갯수 감소.
        // 동기화전 InitCount, Count값 변동값 있는지 체크.
        if( !CheckItemDuration( dwUID_, kPacket_.m_LuckyCoinItem.m_ItemUID, kPacket_.m_LuckyCoinItem.m_nInitCount, kPacket_.m_LuckyCoinItem.m_nCount ) ) {
            START_LOG( clog, L"릭쿤 토네이도 럭키 코인 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( kPacket_.m_LuckyCoinItem.m_ItemUID )
                << BUILD_LOG( kPacket_.m_LuckyCoinItem.m_nInitCount )
                << BUILD_LOG( kPacket_.m_LuckyCoinItem.m_nCount )
                << END_LOG;
        }

        if ( !UseCountItem( dwUID_, kPacket_.m_LuckyCoinItem.m_ItemUID, nCount ) ) {
            START_LOG( cerr, L"릭쿤 토네이도 럭키 코인 개수 동기화 실패. Name : " << strLogin_ ) << END_LOG;
            SET_ERR_GOTO( ERR_RKTORNODO_08, END_PROC );
        }
    }

    for ( cvit = kPacket_.m_vecReward.begin() ; cvit != kPacket_.m_vecReward.end() ; ++cvit ) {
        // 아이템 넣어주기.
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, cvit->m_ItemID, cvit->m_nDuration, cvit->m_nPeriod, KItemManager::EWIT_RKTORNADO_REWARD, kPacket.m_vecItem ) ) {
            START_LOG( cerr, L" RKTornado DB insert Fail.. UserUID : " << dwUID_ )
                << BUILD_LOG( cvit->m_ItemID )
                << BUILD_LOG( cvit->m_nDuration )
                << BUILD_LOG( cvit->m_nPeriod ) << END_LOG;
            bRet = false;
            continue;
        }
    }

    if ( false == bRet && kPacket.m_vecItem.empty() ) {
        SET_ERR_GOTO( ERR_RKTORNODO_06, END_PROC );
    }

    kPacket.m_dwType = kPacket_.m_dwType;
    kPacket.m_CoinItemID = kPacket_.m_CoinItemID;
    kPacket.m_LuckyCoinItemID = kPacket_.m_LuckyCoinItemID;
    kPacket.m_dwRewardType = kPacket_.m_dwRewardType;
    kPacket.m_dwRewardItemID = kPacket_.m_dwRewardItemID;

    kPacket_.m_CoinItem.m_nCount -= 1;
    kPacket_.m_CoinItem.m_nCount = std::max<int>( kPacket_.m_CoinItem.m_nCount, 0 );
    kPacket.m_CoinItem = kPacket_.m_CoinItem;

    if( KRKTornado::AT_NONFAIL == kPacket_.m_dwType ) { // 럭키 코인 갯수 감소.
        kPacket_.m_LuckyCoinItem.m_nCount -= nCount;
        kPacket_.m_LuckyCoinItem.m_nCount = std::max<int>( kPacket_.m_LuckyCoinItem.m_nCount, 0 );
        kPacket.m_LuckyCoinItem = kPacket_.m_LuckyCoinItem;
    }

END_PROC:
    if ( IS_CORRECT( ERR_RKTORNODO_07 ) ) {
        kPacket.m_CoinItem.m_nCount = 0;
    }
    if ( IS_CORRECT( ERR_RKTORNODO_08 ) ) {
        kPacket.m_LuckyCoinItem.m_nCount = 0;
    }

    if ( !IS_CORRECT( ERR_RKTORNODO_07 ) && !IS_CORRECT( ERR_RKTORNODO_08 ) ) {
        SET_ERROR( NET_OK );
    }    

    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket.m_dwRewardType )
        << BUILD_LOG( kPacket.m_vecItem.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_RKTORNADO_ACTION_ACK );
}

INT_IMPL_ON_FUNC( EVENT_OPEN_CALENDAR_BONUS_POINT_REQ )
{
    EVENT_CHECK_OUT( EVENT_OPEN_CALENDAR_BONUS_POINT_REQ );

    SET_ERROR( ERR_UNKNOWN );

    KEVENT_OPEN_CALENDAR_BONUS_POINT_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nAddGCPoint = kPacket_.m_nAddGCPoint;
    kPacket.m_nGCPoint = kPacket_.m_nGCPoint;
    kPacket.m_nInitGCPoint = kPacket_.m_nInitGCPoint;
    CTime tmCurrent = CTime::GetCurrentTime();
    std::wstring strDate = (LPCTSTR)tmCurrent.Format( _T("%y%m%d") );

    // 일일 보너스 GCPoint 지급여부 확인
    int nTodayEventCount = GetTodayEventCount( dwUID_, UDT_OPEN_CALENDAR_BONUS_GCPOINT );
    if( nTodayEventCount > 0 ) {
        SET_ERR_GOTO( ERR_GCPOINT_18, END_PROC );
    }

    // 차이값 저장
    if ( kPacket.m_nGCPoint != kPacket.m_nInitGCPoint ) {
        int nDiff = kPacket.m_nGCPoint - kPacket.m_nInitGCPoint;
        if( false == UpdateUserPointBagAccTime( dwUID_, cCharType_, nDiff, kPacket_.m_nAccTime, strDate ) ) {
            SET_ERR_GOTO( ERR_GCPOINT_19, END_PROC );
        }
    }

    // 일일보너스 저장
    if ( kPacket.m_nAddGCPoint > 0 ) {
        if( false == UpdateUserPointBagAccTime( dwUID_, cCharType_, kPacket.m_nAddGCPoint, kPacket_.m_nAccTime, strDate ) ) {
            SET_ERR_GOTO( ERR_GCPOINT_19, END_PROC );
        }
    }

    kPacket.m_nGCPoint += kPacket.m_nAddGCPoint;
    kPacket.m_nInitGCPoint = kPacket.m_nGCPoint;

    if ( false == SetTodayEventDoneDiff( dwUID_, UDT_OPEN_CALENDAR_BONUS_GCPOINT ) ) {
        START_LOG( cerr, L"일일보너스 DB 로그기록 실패.. Login : " << strLogin_ ) << END_LOG;
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK || kPacket.m_nOK == NetError::ERR_GCPOINT_18 )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nGCPoint )
        << BUILD_LOG( kPacket.m_nInitGCPoint )
        << BUILD_LOG( kPacket.m_nAddGCPoint ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_OPEN_CALENDAR_BONUS_POINT_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_USER_SPHINX_DATA_REQ )
{
    EVENT_CHECK_OUT( EVENT_USER_SPHINX_DATA_REQ );
    SET_ERROR( ERR_UNKNOWN );
    KEVENT_USER_SPHINX_DATA_ACK kPacket;
    kPacket.m_nOK = -99;
    int nRigthAnswerCnt = 0;
    std::vector<int> vecCorrectQuestions;
    int nVersion = SiKSphinx()->GetSphinxVersion();

    GetSphinxUserData( dwUID_, nVersion, nRigthAnswerCnt, vecCorrectQuestions );

    kPacket.m_nRightAnswerCnt = nRigthAnswerCnt;
    kPacket.m_vecCorrectQuestions = vecCorrectQuestions;
    SET_ERROR( NET_OK );

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_USER_SPHINX_DATA_ACK );
}

INT_IMPL_ON_FUNC( EVENT_SPHINX_QUESTION_REQ )
{
    EVENT_CHECK_OUT( EVENT_SPHINX_QUESTION_REQ );
    SET_ERROR( ERR_UNKNOWN );
    KEVENT_SPHINX_QUESTION_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nQuestionID = kPacket_.m_nQuestionID;
    kPacket.m_nTimeValue = kPacket_.m_nTimeValue;

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_kCoinItem.m_ItemUID, kPacket_.m_kCoinItem.m_nInitCount, kPacket_.m_kCoinItem.m_nCount ) ) {
        START_LOG( clog, L"스핑크스 코인 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_kCoinItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_kCoinItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_kCoinItem.m_nCount )
            << END_LOG;
    }

    if ( !UseCountItem( dwUID_, kPacket_.m_kCoinItem.m_ItemUID, 1 ) ) { // 코인 갯수 감소.
        START_LOG( cerr, L"스핑크스 코인 개수 동기화 실패. Name : " << strLogin_ ) << END_LOG;
        SET_ERROR( ERR_SPHINX_08 );
    } else {
        SET_ERROR( NET_OK );
    }

    kPacket_.m_kCoinItem.m_nCount -= 1;
    kPacket_.m_kCoinItem.m_nCount = std::max<int>( kPacket_.m_kCoinItem.m_nCount, 0 );
    kPacket.m_kCoinItem = kPacket_.m_kCoinItem;

    if ( IS_CORRECT( ERR_SPHINX_08 ) ) {
        kPacket.m_kCoinItem.m_nCount = 0;
    }

    kPacket.m_nOK = NetError::GetLastNetError();

    QUEUING_ACK_TO_USER( EVENT_SPHINX_QUESTION_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_SPHINX_ANSWER_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_SPHINX_ANSWER_REQ );
    KEVENT_SPHINX_ANSWER_ACK kPacket;
    kPacket.m_nOK = -99;
    std::vector< KDropItemInfo >::const_iterator vit;
    int nVersion = SiKSphinx()->GetSphinxVersion();
    SET_ERROR( ERR_UNKNOWN );

    // 아이템 별 반복
    for ( vit = kPacket_.m_vecReward.begin(); vit != kPacket_.m_vecReward.end(); ++vit ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, 
            cCharType_,
            vit->m_ItemID, 
            vit->m_nDuration, 
            vit->m_nPeriod, 
            KItemManager::EWIT_SPHINX_EVENT, 
            kPacket.m_vecItem ) ) {

                START_LOG( cerr, L"스핑크스 OX 보상 아이템 지급 실패 UID : " << dwUID_ )
                    << BUILD_LOG( vit->m_ItemID )
                    << BUILD_LOG( vit->m_nDuration )
                    << BUILD_LOG( vit->m_nPeriod ) << END_LOG;

                continue;
        }
    }

    if( kPacket_.m_bCorrectCheck ) {
        UpdateUserSphinxAnswerCnt( dwUID_, nVersion, 0, kPacket_.m_nCorrectCount );
        InsertUserSphinxQuizInfo( dwUID_, nVersion, kPacket_.m_nQuestionLevel, kPacket_.m_nQuestionID );
    }
    kPacket.m_bCorrectCheck = kPacket_.m_bCorrectCheck;
    kPacket.m_nCorrectCount = kPacket_.m_nCorrectCount; 

    SET_ERROR( NET_OK );
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"스핑크스 이벤트 유저 정보 DB 업데이트, UserUID: " << dwUID_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecItem.size() ) << END_LOG;

    QUEUING_ACK_TO_USER_UID( EVENT_SPHINX_ANSWER_ACK );
}

INT_IMPL_ON_FUNC( EVENT_STRENGTH_EQUIP_REQ )
{
    EVENT_CHECK_OUT( EVENT_STRENGTH_EQUIP_REQ );
    KEVENT_STRENGTH_EQUIP_ACK kPacket;
    kPacket.m_nOK = -99;
    std::list<std::pair<int,float>>::iterator litAtt;

    SET_ERROR( ERR_UNKNOWN );
    // 초기 강화석 아이템 장착.
    int nOK = m_kHelper.UpdateStrengthItemInfo( dwUID_, kPacket_.m_StrengthItemUID, kPacket_.m_kStrengthItem.m_cEnchantLevel, kPacket_.m_EquipItemUID, kPacket_.m_kStrengthItem.m_cGradeID, cCharType_ );

    if( nOK != 0 ) {
        START_LOG( cerr, L" 강화석 장착시 Name : " << strLogin_ )
            << BUILD_LOG( nOK )
            << BUILD_LOG( kPacket_.m_StrengthItemUID )
            << BUILD_LOG( kPacket_.m_EquipItemUID ) << END_LOG;
        SET_ERR_GOTO( ERR_STRENGTH_02, END_PROC );
    }

    // 아이템 박힌 강화석의 소유정보를 공용템 처리를 한다.
    nOK = m_kHelper.UIGAUserItemCharacter_delete( dwUID_, cCharType_, kPacket_.m_StrengthItemUID );
    kPacket_.m_kStrengthItem.m_cCharType = -1;

    if( nOK != 0 ) {
        START_LOG( cerr, L" 강화석 소유자 정보 DB 삭제시 에러 Name : " << strLogin_ )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( cCharType_ )
            << BUILD_LOG( kPacket_.m_StrengthItemUID )
            << END_LOG;
    }

    if ( !kPacket_.m_kStrengthItem.m_vecAttribute.empty() ) {
        bool bInitSuccess = true;

        for( int i = 0 ; i < (int)kPacket_.m_kStrengthItem.m_vecAttribute.size() ; ++i ) {
            LIF( bInitSuccess = bInitSuccess && m_kHelper.MergeAttribute( KStrengthManager::SU_INIT, dwUID_, i, -1, 0, kPacket_.m_kStrengthItem, cCharType_ ) );
        }

        _LOG_SUCCESS( bInitSuccess, L"강화석 초기화 결과 : " << bInitSuccess ) << END_LOG;

        kPacket_.m_kStrengthItem.m_vecAttribute.clear();
    }

    if( kPacket_.m_kStrengthItem.m_cGradeID > KItem::GRADE_NORMAL ) {
        // 강화석 랜덤 스킬 적용.
        bool bUpdateSuccess = true;
        int i = 0;

        for( i = 0, litAtt = kPacket_.m_listAttributeList.begin() ; litAtt != kPacket_.m_listAttributeList.end() ; ++litAtt, ++i ) {
            LIF( bUpdateSuccess = bUpdateSuccess && m_kHelper.MergeAttribute( KStrengthManager::SU_UPDATE, dwUID_, i, litAtt->first, litAtt->second, kPacket_.m_kStrengthItem, cCharType_ ) );
        }
    }

    kPacket.m_kEquipItem = kPacket_.m_kEquipItem;
    kPacket.m_kStrengthItem = kPacket_.m_kStrengthItem;

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    _QUEUING_ACK_TO_USER( EVENT_STRENGTH_EQUIP_ACK, kPacket );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_STRENGTH_ACTION_REQ )
{
    EVENT_CHECK_OUT( EVENT_STRENGTH_ACTION_REQ );
    KEVENT_STRENGTH_ACTION_ACK kPacket;
    kPacket.m_nOK = -99;
    std::list<std::pair<int,float>>::iterator litAtt;
    SET_ERROR( ERR_UNKNOWN );

    // 강화석 강화 변경 정보 갱신.
    int nOK = m_kHelper.UpdateStrengthItemInfo( dwUID_, kPacket_.m_StrengthItemUID, (int)kPacket_.m_kStrengthItem.m_cEnchantLevel, kPacket_.m_kStrengthItem.m_EnchantEquipItemUID, kPacket_.m_kStrengthItem.m_cGradeID, cCharType_ );
    if( nOK != 0 ) {
        START_LOG( cerr, L" 강화석 강화정보 DB 갱신시 에러 Name : " << strLogin_ )
            << BUILD_LOG( nOK )
            << BUILD_LOG( kPacket_.m_StrengthItemUID )
            << BUILD_LOGc( kPacket_.m_kStrengthItem.m_cEnchantLevel )
            << BUILD_LOG( kPacket_.m_kStrengthItem.m_EnchantEquipItemUID ) << END_LOG;
        SET_ERR_GOTO( ERR_STRENGTH_11, END_PROC );
    }

    if( kPacket_.m_dwStrengthResult == KStrengthManager::SR_BREAK ) { // 깨짐시 강화석 삭제.
        LIF( SetTypeEraseItem( strLogin_, dwUID_, kPacket_.m_StrengthItemUID, KItemManager::EDEL_DEL_STRENGTH, cCharType_ ) );
    }

    if( kPacket_.m_dwStrengthResult == KStrengthManager::SR_UPGRADE ) {
        bool bInitSuccess = true;
        int i;
        for( i = 0 ; i < (int)kPacket_.m_kStrengthItem.m_vecAttribute.size() ; ++i ) {
            LIF( bInitSuccess = bInitSuccess && m_kHelper.MergeAttribute( KStrengthManager::SU_INIT, dwUID_, i, -1, 0, kPacket_.m_kStrengthItem, cCharType_ ) );
        }
        if( !bInitSuccess ) {
            START_LOG( cerr, L"강화석 초기화 실패" ) << END_LOG;
        }

        // 강화석 랜덤 스킬 적용.
        bool bUpdateSuccess = true;
        kPacket_.m_kStrengthItem.m_vecAttribute.clear();
        for( i = 0, litAtt = kPacket_.m_listAttributeList.begin() ; litAtt != kPacket_.m_listAttributeList.end() ; ++litAtt, ++i ) {
            LIF( bUpdateSuccess = bUpdateSuccess && m_kHelper.MergeAttribute( KStrengthManager::SU_UPDATE, dwUID_, i, litAtt->first, litAtt->second, kPacket_.m_kStrengthItem, cCharType_ ) );
        }
        if( !bUpdateSuccess ) {
            SET_ERR_GOTO( ERR_STRENGTH_22, END_PROC );
        }
    }

    // 강화 보조 수량 아이템 삭제.
    if( kPacket_.m_dwAssistType > KEVENT_STRENGTH_ACTION_REQ::AT_NOUSE ) {
        // 동기화전 InitCount, Count값 변동값 있는지 체크.
        if( !CheckItemDuration( dwUID_, kPacket_.m_kAssistItem.m_ItemUID, kPacket_.m_kAssistItem.m_nInitCount, kPacket_.m_kAssistItem.m_nCount ) ) {
            START_LOG( clog, L"강화 보조 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( kPacket_.m_kAssistItem.m_ItemUID )
                << BUILD_LOG( kPacket_.m_kAssistItem.m_nInitCount )
                << BUILD_LOG( kPacket_.m_kAssistItem.m_nCount )
                << END_LOG;
        }

        if ( !UseCountItem( dwUID_, kPacket_.m_kAssistItem.m_ItemUID, 1 ) ) { // 코인 갯수 감소.
            START_LOG( cerr, L"강화 보조 아이템 개수 동기화 실패. Name : " << strLogin_ ) << END_LOG;
            SET_ERROR( ERR_STRENGTH_13 );
        }
        kPacket.m_kAssistItem = kPacket_.m_kAssistItem;
    }

    kPacket_.m_kAssistItem.m_nCount -= 1;
    kPacket_.m_kAssistItem.m_nCount = std::max<int>( kPacket_.m_kAssistItem.m_nCount, 0 );
    kPacket.m_kAssistItem = kPacket_.m_kAssistItem;

    if ( IS_CORRECT( ERR_STRENGTH_13 ) ) {
        kPacket.m_kAssistItem.m_nCount = 0;
    }

    kPacket.m_dwAssistType = kPacket_.m_dwAssistType;
    kPacket.m_dwStrengthResult = kPacket_.m_dwStrengthResult;
    kPacket.m_kStrengthItem = kPacket_.m_kStrengthItem;
    kPacket.m_kEquipItem = kPacket_.m_kEquipItem;
    kPacket.m_nGPCost = kPacket_.m_nGPCost;

    // 강화 통계.
    InsertStrengthStat( dwUID_, // UserUID
                            kPacket.m_kStrengthItem.m_ItemID, // 강화석 ItemID
                                kPacket.m_kStrengthItem.m_ItemUID, // 강화석 ItemUID
                                   (int)kPacket.m_kStrengthItem.m_cEnchantLevel, // 강화석 강화 레벨
                                        kPacket.m_kEquipItem.m_ItemID, // 강화석 장착된 ItemID
                                            kPacket.m_kEquipItem.m_ItemUID, // 강화석 장착된 ItemUID
                                                KStrengthManager::ST_STRENGTH, // Type
                                                    kPacket.m_dwStrengthResult, // 강화 결과
                                                        kPacket.m_kAssistItem.m_ItemID ); // 보조 주문서 ItemID

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    _QUEUING_ACK_TO_USER( EVENT_STRENGTH_ACTION_ACK, kPacket );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kStrengthItem.m_ItemID )
        << BUILD_LOG( kPacket.m_kStrengthItem.m_ItemUID )
        << BUILD_LOG( kPacket.m_kStrengthItem.m_cEnchantLevel )
        << BUILD_LOG( kPacket.m_kEquipItem.m_ItemID )
        << BUILD_LOG( kPacket.m_kEquipItem.m_ItemUID )
        << BUILD_LOG( kPacket.m_dwStrengthResult )
        << BUILD_LOG( kPacket.m_kAssistItem.m_ItemID )
        << BUILD_LOG( kPacket.m_kAssistItem.m_ItemUID )
        << BUILD_LOG( kPacket.m_dwAssistType )
        << BUILD_LOG( kPacket.m_nGPCost ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_STRENGTH_BREAKUP_REQ )
{
    EVENT_CHECK_OUT( EVENT_STRENGTH_BREAKUP_REQ );
    KEVENT_STRENGTH_BREAKUP_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    if( kPacket_.m_dwBreakUpResult == KStrengthManager::SR_SUCCESS ) { // 강화석 해체 결과 저장.
        int nOK = m_kHelper.UpdateStrengthItemInfo( dwUID_, kPacket_.m_StrengthItemUID, (int)kPacket_.m_kStrengthItem.m_cEnchantLevel, kPacket_.m_kStrengthItem.m_EnchantEquipItemUID, kPacket_.m_kStrengthItem.m_cGradeID, cCharType_ );

        if( nOK != 0 ) {
            START_LOG( cerr, L" 강화석 강화정보 DB 갱신시 에러 Name : " << strLogin_ )
                << BUILD_LOG( nOK )
                << BUILD_LOG( kPacket_.m_StrengthItemUID )
                << BUILD_LOGc( kPacket_.m_kStrengthItem.m_cEnchantLevel )
                << BUILD_LOG( kPacket_.m_kStrengthItem.m_EnchantEquipItemUID ) << END_LOG;
            SET_ERR_GOTO( ERR_STRENGTH_11, END_PROC );
        }

        // 아이템 박힌 강화석의 소유정보를 공용템 처리를 한다.
        nOK = m_kHelper.UIGAUserItemCharacter_insert( dwUID_, cCharType_, kPacket_.m_StrengthItemUID );
        kPacket_.m_kStrengthItem.m_cCharType = cCharType_;
        if( nOK != 0 ) {
            START_LOG( cerr, L" 강화석 소유자 정보 DB 입력시 에러 Name : " << strLogin_ )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( cCharType_ )
                << BUILD_LOG( kPacket_.m_StrengthItemUID )
                << END_LOG;
        }
    }

    if( kPacket_.m_dwBreakUpResult == KStrengthManager::SR_BREAK ) { // 깨짐시 강화석 삭제.
        LIF( SetTypeEraseItem( strLogin_, dwUID_, kPacket_.m_StrengthItemUID, KItemManager::EDEL_DEL_STRENGTH, cCharType_ ) );
    }

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_kMaterialItem.m_ItemUID, kPacket_.m_kMaterialItem.m_nInitCount, kPacket_.m_kMaterialItem.m_nCount ) ) {
        START_LOG( clog, L"강화 보조 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_kMaterialItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_kMaterialItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_kMaterialItem.m_nCount )
            << END_LOG;
    }

    // 강화 해체 주문서 아이템 삭제.
    if ( !UseCountItem( dwUID_, kPacket_.m_kMaterialItem.m_ItemUID, 1 ) ) { // 코인 갯수 감소.
        START_LOG( cerr, L"강화 보조 아이템 개수 동기화 실패. Name : " << strLogin_ ) << END_LOG;
        SET_ERROR( ERR_STRENGTH_13 );
    }

    kPacket_.m_kMaterialItem.m_nCount -= 1;
    kPacket_.m_kMaterialItem.m_nCount = std::max<int>( kPacket_.m_kMaterialItem.m_nCount, 0 );
    kPacket.m_kMaterialItem = kPacket_.m_kMaterialItem;

    if ( IS_CORRECT( ERR_STRENGTH_13 ) ) {
        kPacket.m_kMaterialItem.m_nCount = 0;
    }

    kPacket.m_dwBreakUpResult = kPacket_.m_dwBreakUpResult;
    kPacket.m_kStrengthItem = kPacket_.m_kStrengthItem;
    kPacket.m_kMaterialItem = kPacket_.m_kMaterialItem;
    kPacket.m_kEquipItem = kPacket_.m_kEquipItem;

    // 해체 통계.
    InsertStrengthStat( dwUID_, // UserUID
                            kPacket.m_kStrengthItem.m_ItemID, // 강화석 ItemID
                                kPacket.m_kStrengthItem.m_ItemUID, // 강화석 ItemUID
                                    (int)kPacket.m_kStrengthItem.m_cEnchantLevel, // 강화석 강화 레벨
                                        kPacket.m_kEquipItem.m_ItemID, // 강화석 장착된 ItemID
                                            kPacket.m_kEquipItem.m_ItemUID, // 강화석 장착된 ItemUID
                                                KStrengthManager::ST_BREAKUP, // Type
                                                    kPacket.m_dwBreakUpResult, // 해체 결과
                                                        kPacket.m_kMaterialItem.m_ItemID ); // 보조 주문서 ItemID
    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    _QUEUING_ACK_TO_USER( EVENT_STRENGTH_BREAKUP_ACK, kPacket );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwBreakUpResult ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_STRENGTH_COMPOSE_REQ )
{
    EVENT_CHECK_OUT( EVENT_STRENGTH_COMPOSE_REQ );
    KEVENT_STRENGTH_COMPOSE_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    // 강화석 속성 초기화 여부.
    if( kPacket_.m_bAttributeInit ) {
        LIF( m_kHelper.DeleteAttribute( dwUID_, kPacket_.m_kFirstStrengthItem.m_ItemUID ) );
        kPacket_.m_kFirstStrengthItem.m_vecAttribute.clear();
    }

    // 합성된 강화석 정보 갱신.
    int nOK = m_kHelper.UpdateStrengthItemInfo( dwUID_, kPacket_.m_kFirstStrengthItem.m_ItemUID, (int)kPacket_.m_kFirstStrengthItem.m_cEnchantLevel, 0, kPacket_.m_kFirstStrengthItem.m_cGradeID, cCharType_ );

    if( nOK != 0 ) {
        START_LOG( cerr, L" 강화석 강화정보 DB 갱신시 에러 Name : " << strLogin_ )
            << BUILD_LOG( nOK )
            << BUILD_LOG( kPacket_.m_kFirstStrengthItem.m_ItemUID )
            << BUILD_LOGc( kPacket_.m_kFirstStrengthItem.m_cEnchantLevel )
            << BUILD_LOG( kPacket_.m_kFirstStrengthItem.m_EnchantEquipItemUID ) << END_LOG;
        SET_ERR_GOTO( ERR_STRENGTH_11, END_PROC );
    }

    LIF( SetTypeEraseItem( strLogin_, dwUID_, kPacket_.m_kFirstEquipItem.m_ItemUID, KItemManager::EDEL_DEL_STRENGTH_EQUIP, cCharType_ ) );
    LIF( SetTypeEraseItem( strLogin_, dwUID_, kPacket_.m_kSecondEquipItem.m_ItemUID, KItemManager::EDEL_DEL_STRENGTH_EQUIP, cCharType_ ) );
    LIF( SetTypeEraseItem( strLogin_, dwUID_, kPacket_.m_kSecondStrengthItem.m_ItemUID, KItemManager::EDEL_DEL_STRENGTH, cCharType_) );

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_kMaterialItem.m_ItemUID, kPacket_.m_kMaterialItem.m_nInitCount, kPacket_.m_kMaterialItem.m_nCount ) ) {
        START_LOG( clog, L"강화 합성 보조 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_kMaterialItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_kMaterialItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_kMaterialItem.m_nCount )
            << END_LOG;
    }

    // 강화 합성 주문서 아이템 삭제.
    if ( !UseCountItem( dwUID_, kPacket_.m_kMaterialItem.m_ItemUID, 1 ) ) { // 주문서 갯수 감소.
        START_LOG( cerr, L"강화 합성 보조 아이템 개수 동기화 실패. Name : " << strLogin_ ) << END_LOG;
        SET_ERROR( ERR_STRENGTH_13 );
    }

    kPacket_.m_kMaterialItem.m_nCount -= 1;
    kPacket_.m_kMaterialItem.m_nCount = std::max<int>( kPacket_.m_kMaterialItem.m_nCount, 0 );
    kPacket.m_kMaterialItem = kPacket_.m_kMaterialItem;

    if ( IS_CORRECT( ERR_STRENGTH_13 ) ) {
        kPacket.m_kMaterialItem.m_nCount = 0;
    }

    // 아이템 박힌 강화석의 소유정보를 공용템 처리를 한다.
    nOK = m_kHelper.UIGAUserItemCharacter_insert( dwUID_, cCharType_, kPacket_.m_kFirstStrengthItem.m_ItemUID );
    kPacket_.m_kFirstStrengthItem.m_cCharType = cCharType_;
    if( nOK != 0 ) {
        START_LOG( cerr, L" 강화석 소유자 정보 DB 입력시 에러 Name : " << strLogin_ )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( cCharType_ )
            << BUILD_LOG( kPacket_.m_kFirstStrengthItem.m_ItemUID )
            << END_LOG;
    }

    kPacket.m_dwComposeResult = kPacket_.m_dwComposeResult;
    kPacket.m_kFirstStrengthItem = kPacket_.m_kFirstStrengthItem;
    kPacket.m_kSecondStrengthItem = kPacket_.m_kSecondStrengthItem;
    kPacket.m_kMaterialItem = kPacket_.m_kMaterialItem;
    kPacket.m_bAttributeInit = kPacket_.m_bAttributeInit;

    // 합성 통계.
    InsertStrengthStat( dwUID_, // UserUID
                            kPacket.m_kFirstStrengthItem.m_ItemID, // 강화석 ItemID
                                kPacket.m_kFirstStrengthItem.m_ItemUID, // 강화석 ItemUID
                                    (int)kPacket.m_kFirstStrengthItem.m_cEnchantLevel, // 강화석 강화 레벨
                                        kPacket_.m_kFirstEquipItem.m_ItemID, // 강화석 장착된 ItemID
                                            kPacket_.m_kFirstEquipItem.m_ItemUID, // 강화석 장착된 ItemUID
                                                KStrengthManager::ST_COMPOSE, // Type
                                                    kPacket.m_dwComposeResult, // 해체 결과
                                                        kPacket.m_kMaterialItem.m_ItemID ); // 보조 주문서 ItemID
    InsertStrengthStat( dwUID_, // UserUID
                            kPacket.m_kSecondStrengthItem.m_ItemID, // 강화석 ItemID
                                kPacket.m_kSecondStrengthItem.m_ItemUID, // 강화석 ItemUID
                                    (int)kPacket.m_kSecondStrengthItem.m_cEnchantLevel, // 강화석 강화 레벨
                                        kPacket_.m_kSecondEquipItem.m_ItemID, // 강화석 장착된 ItemID
                                            kPacket_.m_kSecondEquipItem.m_ItemUID, // 강화석 장착된 ItemUID
                                                KStrengthManager::ST_COMPOSE, // Type
                                                    kPacket.m_dwComposeResult, // 해체 결과
                                                        0 ); // 보조 주문서 ItemID
    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    _QUEUING_ACK_TO_USER( EVENT_STRENGTH_COMPOSE_ACK, kPacket );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kFirstStrengthItem.m_ItemID )
        << BUILD_LOG( kPacket.m_kFirstStrengthItem.m_ItemUID )
        << BUILD_LOG( kPacket.m_kSecondStrengthItem.m_ItemID )
        << BUILD_LOG( kPacket.m_kSecondStrengthItem.m_ItemUID ) << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_DUNGEON_STAT_INFO, KDungeonStatAddA )
{
    if( !kPacket_.m_mapItemUseCount.empty() ) {
        InsertDungeonUseItemStat( kPacket_.m_nModeID, kPacket_.m_nDifficult, kPacket_.m_mapItemUseCount, kPacket_.m_dwLoginUID, kPacket_.m_tEndGameTime );
    }
    if( !kPacket_.m_mapItemBuyCount.empty() ) {
        InsertDungeonBuyItemStat( kPacket_.m_nModeID, kPacket_.m_nDifficult, kPacket_.m_mapItemBuyCount, kPacket_.m_dwLoginUID, kPacket_.m_tEndGameTime );
    }
    if(!kPacket_.m_mapZoneDeathCount.empty() ) {
        InsertDungeonZoneDeathStat( kPacket_.m_nModeID, kPacket_.m_nDifficult, kPacket_.m_mapZoneDeathCount, kPacket_.m_dwLoginUID, kPacket_.m_tEndGameTime );
    }
    if( !kPacket_.m_mapDeathType.empty() ) {
        InsertDungeonDeathTypeStat( kPacket_.m_nModeID, kPacket_.m_nDifficult, kPacket_.m_mapDeathType, kPacket_.m_dwLoginUID, kPacket_.m_tEndGameTime );
    }

    START_LOG( clog, L" 던전 통계 추가. " )
        << BUILD_LOG( kPacket_.m_mapItemUseCount.size() )
        << BUILD_LOG( kPacket_.m_mapItemBuyCount.size() )
        << BUILD_LOG( kPacket_.m_mapZoneDeathCount.size() )
        << BUILD_LOG( kPacket_.m_mapDeathType.size() ) << END_LOG;
}

_INT_IMPL_ON_FUNC( DB_EVENT_SQUARE_STAT_TIME, DWORD )
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    // 유저의 광장 시간 통계 정보 저장.
    if( !SetTodayEventDoneDiff( dwUID_, UET_USER_SQUARE_STAT_TIME, tmCurrent, kPacket_ ) ) {
        START_LOG( cerr, L"유저의 광장 시간 통계 정보 남기기 실패 : " << strLogin_ )
            << BUILD_LOG( kPacket_ ) << END_LOG;
    }
}

_INT_IMPL_ON_FUNC( EVENT_BUY_FOR_AGIT_REQ, KDB_EVENT_BUY_FOR_AGIT_REQ )
{
    KEVENT_BUY_FOR_AGIT_ACK kPacket;
    kPacket.m_vecPeriodMaterialItems = kPacket_.m_vecDelMaterials;
    kPacket.m_prKey = std::make_pair( kPacket_.m_kCatalog.m_Index, kPacket_.m_kCatalog.m_ItemID );
    std::vector<GCITEMUID>::iterator vit;
    std::vector< KItem >::iterator vitReward;
    std::vector<KItem> vecChangedItemList; // 변환된 아이템 List
    vecChangedItemList.clear();
    std::vector<KItem>::iterator vitChange;

    bool bLookItem = kPacket_.m_kCatalog.CheckItemFlag( KManufactureItem::IF_LOOK );
    bool bAgitObj = kPacket_.m_kCatalog.CheckItemFlag( KManufactureItem::IF_AGIT_OBJ );
    int nOK = 0;

    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_BUY_FOR_AGIT_REQ );

    if ( false == m_kHelper.InsertSingleItemJob( dwUID_, KItem::EICT_AGIT, kPacket_.m_kCatalog.m_ItemID, kPacket_.m_kCatalog.m_nDuration, kPacket_.m_kCatalog.m_nPeriod, KItemManager::EWIT_AGIT_STORE, kPacket.m_vecRewards ) ) {
        START_LOG( cerr, L"아지트 아이템 삽입 실패 Name : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_kCatalog.m_ItemID )
            << BUILD_LOG( kPacket_.m_kCatalog.m_nDuration )
            << BUILD_LOG( kPacket_.m_kCatalog.m_nPeriod ) << END_LOG;
        SET_ERR_GOTO( ERR_AGIT_STORE_09, END_PROC );
    }
    else {
        for( vit = kPacket_.m_vecDelMaterials.begin() ; vit != kPacket_.m_vecDelMaterials.end() ; ++vit ) {
            LIF( PureEraseItem( strLogin_, dwUID_, *vit, KItem::EICT_AGIT ) );
        }

        for ( vitReward = kPacket.m_vecRewards.begin() ; vitReward != kPacket.m_vecRewards.end() ; ++vitReward ) {
            if( vitReward->m_cItemType == KItem::TYPE_LOOK ) {
                continue;
            }

            // 룩 아이템 변환
            if ( bLookItem ) {
                if ( false == m_kHelper.NormalItemToCoordiItem( dwUID_, vitReward->m_ItemUID, cCharType_, nOK ) ) {
                    START_LOG( cwarn, L"DB작업중 룩 아이템 변환 실패. OK : " << nOK << L"ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
                    SET_ERR_GOTO( ERR_AGIT_STORE_10, END_PROC );
                }
                vitReward->AddItemType( KItem::TYPE_LOOK );
                vecChangedItemList.push_back( *vitReward );
                START_LOG( clog, L"룩 아이템으로 변환됨. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
            }

            // 아지트 오브젝트 설정
            if ( bAgitObj ) {
                if ( false == m_kHelper.AgiteObjPos_merge( dwUID_, vitReward->m_ItemUID, -1, -1 ) ) {
                    START_LOG( cerr, L"DB작업중 아지트 아이템 설정 실패. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
                    continue;
                }

                vitReward->AddItemType( KItem::TYPE_AGIT_OBJ );
                START_LOG( clog, L"아지트 아이템으로 설정. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
            }
        }
    }

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    if ( kPacket.m_nOK != NetError::NET_OK && bLookItem ) {
        // 변환 실패시, 롤백
        for ( vitChange = vecChangedItemList.begin() ; vitChange != vecChangedItemList.end() ; ++vitChange ) {
            if ( m_kHelper.CoordiItemToNormalItem( dwUID_, vitChange->m_ItemUID, cCharType_ ) ) {
                vitChange->DeleteItemType( KItem::TYPE_LOOK );
                START_LOG( cwarn, L"DB작업중 룩변환 실패시, 일반아이템으로 롤백. ItemID : " << vitChange->m_ItemID << L",ItemUID : " << vitChange->m_ItemUID ) << END_LOG;
            }
        }
    }

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_kCatalog.m_Index )
        << BUILD_LOG( kPacket_.m_kCatalog.m_ItemID )
        << BUILD_LOG( bLookItem )
        << BUILD_LOG( bAgitObj )
        << BUILD_LOG( kPacket.m_vecRewards.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_BUY_FOR_AGIT_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_SELL_FOR_AGIT_REQ, KDB_EVENT_SELL_FOR_AGIT_REQ )
{
    KEVENT_SELL_FOR_AGIT_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_vecRewardItem.clear();
    kPacket.m_mapFailInfo = kPacket_.m_mapFailInfo;
    kPacket.m_vecEraseItem = kPacket_.m_vecEraseItem;
    kPacket.m_vecSellItem = kPacket_.m_vecSellItem;

    std::vector<GCITEMUID>::iterator vit;
    GCITEMID FlowerItemID = 0;
    int nRewardCount = 0;
    int nCurrentCount = 0;

    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_SELL_FOR_AGIT_REQ );

    if ( kPacket.m_vecSellItem.empty() ) {
        START_LOG( clog, L"판매될 아지트 오브젝트 아이템이 없음. Login : " << strLogin_ )
            << BUILD_LOG( kPacket.m_vecSellItem.size() )
            << BUILD_LOG( kPacket.m_vecEraseItem.size() )
            << BUILD_LOG( kPacket.m_mapFailInfo.size() ) << END_LOG;
        SET_ERR_GOTO( ERR_AGIT_SELL_OBJECT_01, END_PROC );
    }

    // 제거할 아이템 우선 삭제
    for ( vit = kPacket.m_vecEraseItem.begin() ; vit != kPacket.m_vecEraseItem.end() ; ++vit ) {
        LIF( PureEraseItem( strLogin_, dwUID_, *vit, KItem::EICT_AGIT ) );
    }

    // 보상 아이템 정보
    FlowerItemID = SiKAgitHelper()->GetFlowerItemID();
    nRewardCount = static_cast<int>( SiKAgitHelper()->GetSellRewardFlowerCount() );
    nCurrentCount = kPacket.m_vecSellItem.size() * nRewardCount;

    // 보상이 있을 경우,
    if ( nCurrentCount > 0 ) {
        // 판매 보상 아이템 지급
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, KItem::EICT_AGIT, FlowerItemID, nCurrentCount, -1, KItemManager::EWIT_AGIT_OBJ_SELL, kPacket.m_vecRewardItem ) ) {

            START_LOG( cerr, L"아지트 오브젝트 판매보상 아이템 DB지급 실패 Name : " << strLogin_ )
                << BUILD_LOG( FlowerItemID )
                << BUILD_LOG( nRewardCount )
                << BUILD_LOG( kPacket.m_vecSellItem.size() )
                << BUILD_LOG( nCurrentCount ) << END_LOG;

            // 판매물품 모두 실패리스트로..
            for ( vit = kPacket.m_vecSellItem.begin() ; vit != kPacket.m_vecSellItem.end() ; ++vit ) {
                kPacket.m_mapFailInfo[*vit] = 5;
            }
            kPacket.m_vecSellItem.clear();

            SET_ERR_GOTO( ERR_AGIT_SELL_OBJECT_05, END_PROC );
        }

        // 판매된 아이템 삭제
        for ( vit = kPacket.m_vecSellItem.begin() ; vit != kPacket.m_vecSellItem.end() ; ++vit ) {
            LIF( PureEraseItem( strLogin_, dwUID_, *vit, KItem::EICT_AGIT ) );
        }
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( FlowerItemID )
        << BUILD_LOG( nRewardCount )
        << BUILD_LOG( nCurrentCount )
        << BUILD_LOG( kPacket.m_vecRewardItem.size() )
        << BUILD_LOG( kPacket_.m_vecSellItem.size() )
        << BUILD_LOG( kPacket.m_vecSellItem.size() )
        << BUILD_LOG( kPacket_.m_vecEraseItem.size() )
        << BUILD_LOG( kPacket.m_vecEraseItem.size() )
        << BUILD_LOG( kPacket_.m_mapFailInfo.size() )
        << BUILD_LOG( kPacket.m_mapFailInfo.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_SELL_FOR_AGIT_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_HARVEST_FLOWER_POT_NOT, KHarvestFlowerPotAck )
{
    // 획득한 꽃 아이템 넣기
    if ( kPacket_.m_nFlowerCount > 0 ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, KItem::EICT_AGIT, kPacket_.m_kFlowerItem.m_ItemID, kPacket_.m_nFlowerCount, -1, KItemManager::EWIT_FLOWER_POT, kPacket_.m_vecItem ) ) {
            START_LOG( cerr, L"아지트 화분에서 수확한 아이템 삽입 실패 Login : " << strLogin_ )
                << BUILD_LOG( kPacket_.m_kFlowerItem.m_ItemID )
                << BUILD_LOG( kPacket_.m_nFlowerCount )
                << BUILD_LOG( kPacket_.m_nBonusFlowerCount ) << END_LOG;
        }
    }

    if ( kPacket_.m_nBonusFlowerCount > 0 ) {
        // 보너스 지급
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, KItem::EICT_AGIT, kPacket_.m_kFlowerItem.m_ItemID, kPacket_.m_nBonusFlowerCount, -1, KItemManager::EWIT_AGIT_STAY_BONUS, kPacket_.m_vecItem ) ) {
            START_LOG( cerr, L"아지트 화분에서 수확한 보너스 아이템 삽입 실패 Login : " << strLogin_ )
                << BUILD_LOG( kPacket_.m_kFlowerItem.m_ItemID )
                << BUILD_LOG( kPacket_.m_nFlowerCount )
                << BUILD_LOG( kPacket_.m_nBonusFlowerCount ) << END_LOG;
        }
    }

    START_LOG( clog, L"아지트 화분 수확한 아이템 DB에 넣어주기. Login : " << strLogin_ )
        << BUILD_LOG( kPacket_.m_kFlowerItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_nFlowerCount )
        << BUILD_LOG( kPacket_.m_nBonusFlowerCount )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;

    _QUEUING_ACK_TO_USER( EVENT_HARVEST_FLOWER_POT_ACK, kPacket_ );
}

_INT_IMPL_ON_FUNC( EVENT_AGIT_STAY_BONUS_DROP_REQ, KDropItemInfo )
{
    KAgitBonusDrop kPacket;
    kPacket.m_dwAgitUID = 0;
    kPacket.m_vecItem.clear();
    kPacket.m_nDropCount = kPacket_.m_nDuration;

    __JIF( kPacket_.m_nDuration > 0, return );

    // 획득한 아이템 넣기
    if ( false == m_kHelper.InsertSingleItemJob( dwUID_, KItem::EICT_AGIT, kPacket_.m_ItemID, kPacket_.m_nDuration, kPacket_.m_nPeriod, KItemManager::EWIT_AGIT_STAY_BONUS, kPacket.m_vecItem ) ) {
        START_LOG( cerr, L"아지트 체재시간 보너스 아이템 DB삽입 실패 Login : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_ItemID )
            << BUILD_LOG( kPacket_.m_nDuration )
            << BUILD_LOG( kPacket_.m_nPeriod ) << END_LOG;
    }

    // 획득한 수량 기록
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    SetTodayEventDoneDiff( dwUID_, UDT_TODAY_SEED_DROP_COUNT, tmCurrent, kPacket.m_nDropCount );

    START_LOG( clog, L"아지트 체재시간 보너스 아이템 DB에 넣어주기. Login : " << strLogin_ )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket_.m_nDuration )
        << BUILD_LOG( kPacket_.m_nPeriod )
        << BUILD_LOG( kPacket.m_vecItem.size() )
        << BUILD_LOGtm( CTime(tmCurrent) ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_AGIT_STAY_BONUS_DROP_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_TODAYS_SEED_DROP_COUNT_REQ )
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KEVENT_TODAYS_SEED_DROP_COUNT_ACK kPacket;
    kPacket.m_nTodaySeedDropCount = GetTodayEventCount( dwUID_, UDT_TODAY_SEED_DROP_COUNT, tmCurrent );
    kPacket.m_tmRegDate = tmCurrent;
    kPacket.m_bTodaySeedInit = false;

    // 일일 초기화 진행여부 확인
    int nRet = GetTodayEventCount( dwUID_, UDT_TODAY_SEED_DROP_INIT, tmCurrent );
    if ( nRet == 0 ) {
        SetTodayEventDoneDiff( dwUID_, UDT_TODAY_SEED_DROP_INIT, tmCurrent, 1 );
        kPacket.m_bTodaySeedInit = true;
    }

    START_LOG( clog, L"Get TodaysSeedDropCount from DB. Login : " << strLogin_ )
        << BUILD_LOG( kPacket.m_nTodaySeedDropCount )
        << BUILD_LOG( kPacket.m_bTodaySeedInit )
        << BUILD_LOGtm( CTime(tmCurrent) ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_TODAYS_SEED_DROP_COUNT_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_BUY_FOR_BROWNIE_REQ, KDB_EVENT_BUY_FOR_BROWNIE_REQ )
{
    KEVENT_BUY_FOR_BROWNIE_ACK kPacket;
    kPacket.m_vecPeriodMaterialItems = kPacket_.m_vecDelMaterials;
    kPacket.m_prKey = std::make_pair( kPacket_.m_kCatalog.m_Index, kPacket_.m_kCatalog.m_ItemID );
    std::vector<GCITEMUID>::iterator vit;
    std::vector< KItem >::iterator vitReward;
    bool bIsBrownie = SiKAgitHelper()->IsBrownieID( kPacket_.m_kCatalog.m_ItemID );

    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_BUY_FOR_BROWNIE_REQ );

    if ( false == m_kHelper.InsertSingleItemJob( dwUID_, KItem::EICT_AGIT, kPacket_.m_kCatalog.m_ItemID, kPacket_.m_kCatalog.m_nDuration, kPacket_.m_kCatalog.m_nPeriod, KItemManager::EWIT_BROWNIE_STORE, kPacket.m_vecRewards ) ) {
        START_LOG( cerr, L"도우미 아이템 삽입 실패 Name : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_kCatalog.m_ItemID )
            << BUILD_LOG( kPacket_.m_kCatalog.m_nDuration )
            << BUILD_LOG( kPacket_.m_kCatalog.m_nPeriod ) << END_LOG;
        SET_ERR_GOTO( ERR_AGIT_STORE_09, END_PROC );
    }
    else {
        // 사용된 기간제 아이템 제거
        for( vit = kPacket_.m_vecDelMaterials.begin() ; vit != kPacket_.m_vecDelMaterials.end() ; ++vit ) {
            LIF( PureEraseItem( strLogin_, dwUID_, *vit, KItem::EICT_AGIT ) );
        }

        for ( vitReward = kPacket.m_vecRewards.begin() ; vitReward != kPacket.m_vecRewards.end() ; ++vitReward ) {
            // 아지트 오브젝트 설정
            if ( false == m_kHelper.AgiteObjPos_merge( dwUID_, vitReward->m_ItemUID, -1, -1 ) ) {
                START_LOG( cerr, L"DB작업중 아지트 아이템 설정 실패. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
                continue;
            }

            vitReward->AddItemType( KItem::TYPE_AGIT_OBJ );
            START_LOG( clog, L"아지트 아이템으로 설정. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;

            // 도우미 설정
            if ( bIsBrownie ) {
                KBrownieInfo kInfo;
                kInfo.m_dwUID = vitReward->m_ItemUID;
                kInfo.m_EquipItemUID = 0;
                kInfo.m_EquipItemID = 0;
                kInfo.m_bIsUsed = false;
                if ( false == m_kHelper.AgitHelper_merge( dwUID_, kInfo ) ) {
                    START_LOG( cerr, L"DB작업중 도우미 설정 실패. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
                    continue;
                }
                START_LOG( clog, L"도우미로 설정. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
            }
        }
    }

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( bIsBrownie )
        << BUILD_LOG( kPacket_.m_kCatalog.m_Index )
        << BUILD_LOG( kPacket_.m_kCatalog.m_ItemID )
        << BUILD_LOG( kPacket.m_vecRewards.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_BUY_FOR_BROWNIE_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_AGIT_INIT_SEED_REQ, KDropItemInfo )
{
    KAgitBonusDrop kPacket;
    kPacket.m_dwAgitUID = dwUID_;
    kPacket.m_vecItem.clear();
    kPacket.m_nDropCount = kPacket_.m_nDuration;
    kPacket.m_tmDropTime = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    __JIF( kPacket_.m_nDuration > 0, return );

    // 획득한 아이템 넣기
    if ( false == m_kHelper.InsertSingleItemJob( dwUID_, KItem::EICT_AGIT, kPacket_.m_ItemID, kPacket_.m_nDuration, kPacket_.m_nPeriod, KItemManager::EWIT_AGIT_ITEM_INIT, kPacket.m_vecItem ) ) {
        START_LOG( cerr, L"아지트 일일초기화 씨앗아이템지급 DB삽입 실패 Login : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_ItemID )
            << BUILD_LOG( kPacket_.m_nDuration )
            << BUILD_LOG( kPacket_.m_nPeriod ) << END_LOG;
    }

    // 획득한 수량 기록
    LIF( SetTodayEventDoneDiff( dwUID_, UDT_TODAY_SEED_DROP_COUNT, kPacket.m_tmDropTime, kPacket.m_nDropCount ) );

    START_LOG( clog, L"아지트 일일초기화 씨앗아이템지급 DB에 넣어주기. Login : " << strLogin_ )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket_.m_nDuration )
        << BUILD_LOG( kPacket_.m_nPeriod )
        << BUILD_LOG( kPacket.m_vecItem.size() )
        << BUILD_LOGtm( CTime(kPacket.m_tmDropTime) ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_AGIT_INIT_SEED_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_DROP_FLOWER_POT_REQ, KAddItemReq )
{
    KAddItemAck kPacket;
    kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
    kPacket.m_vecAddItem.clear();

    std::vector<KDropItemInfo>::iterator vit;
    for ( vit = kPacket_.m_vecAddItem.begin() ; vit != kPacket_.m_vecAddItem.end() ; ++vit ) {
        // 아이템 지급
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, KItem::EICT_AGIT, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_AGIT_AUTO_DROP, kPacket.m_vecAddItem ) ) {
            START_LOG( cerr, L"아지트 화분아이템 지급 DB삽입 실패 Login : " << strLogin_ )
                << BUILD_LOG( vit->m_ItemID )
                << BUILD_LOG( vit->m_nDuration )
                << BUILD_LOG( vit->m_nPeriod ) << END_LOG;
        }
    }

    std::vector<KItem>::iterator vitReward;
    for ( vitReward = kPacket.m_vecAddItem.begin() ; vitReward != kPacket.m_vecAddItem.end() ; ++vitReward ) {
        // 아지트 오브젝트 설정
        if ( false == m_kHelper.AgiteObjPos_merge( dwUID_, vitReward->m_ItemUID, -1, -1 ) ) {
            START_LOG( cerr, L"DB작업중 아지트 아이템 설정 실패. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
            continue;
        }

        vitReward->AddItemType( KItem::TYPE_AGIT_OBJ );
        START_LOG( clog, L"아지트 아이템으로 설정. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
    }

    START_LOG( clog, L"아지트 화분아이템 지급 DB에 넣어주기. Login : " << strLogin_ )
        << BUILD_LOG( kPacket_.m_vecAddItem.size() )
        << BUILD_LOG( kPacket.m_vecAddItem.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_DROP_FLOWER_POT_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_AGIT_TUTORIAL_DONE_REQ )
{
    // 튜토리얼 진행여부
    bool kPacket = false;

    int nRet = GetTodayEventCount( dwUID_, UDT_AGIT_TUTORAIL_DONE, 0 );
    if ( nRet != 0 ) {
        kPacket = true; // 이미 튜토리얼을 진행했음.
    }

    START_LOG( clog, L"아지트 튜토리얼 진행여부 확인. Login : " << strLogin_ )
        << BUILD_LOG( nRet )
        << BUILD_LOG( kPacket ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_AGIT_TUTORIAL_DONE_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_AGIT_TUTORIAL_REQ, DROP_ITEMS )
{
    SET_ERROR( ERR_UNKNOWN );

    std::vector<KDropItemInfo>::iterator vit;
    std::vector<KItem>::iterator vitReward;

    KEVENT_AGIT_TUTORIAL_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_vecItem.clear();

    // 튜토리얼 진행여부
    int nRet = GetTodayEventCount( dwUID_, UDT_AGIT_TUTORAIL_DONE, 0 );
    if ( nRet != 0 ) {
        SET_ERR_GOTO( ERR_AGIT_TUTORIAL_01, END_PROC );
    }

    // 튜토리얼 진행 갱신
    SetTodayEventDoneDiff( dwUID_, UDT_AGIT_TUTORAIL_DONE, 0, 1 );

    // 튜토리얼 아이템 지급
    for ( vit = kPacket_.begin() ; vit != kPacket_.end() ; ++vit ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, KItem::EICT_AGIT, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_AGIT_AUTO_DROP, kPacket.m_vecItem ) ) {
            START_LOG( cerr, L"아지트 튜토리얼 아이템 지급 DB삽입 실패 Login : " << strLogin_ )
                << BUILD_LOG( vit->m_ItemID )
                << BUILD_LOG( vit->m_nDuration )
                << BUILD_LOG( vit->m_nPeriod ) << END_LOG;
        }
    }

    for ( vitReward = kPacket.m_vecItem.begin() ; vitReward != kPacket.m_vecItem.end() ; ++vitReward ) {
        // 아지트 오브젝트 설정
        if ( false == m_kHelper.AgiteObjPos_merge( dwUID_, vitReward->m_ItemUID, -1, -1 ) ) {
            START_LOG( cerr, L"DB작업중 아지트 아이템 설정 실패. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
            continue;
        }

        vitReward->AddItemType( KItem::TYPE_AGIT_OBJ );
        START_LOG( clog, L"아지트 아이템으로 설정. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
    }

    // 아이템 DB지급 실패
    if ( !kPacket_.empty() && kPacket.m_vecItem.empty() ) {
        SET_ERR_GOTO( ERR_AGIT_TUTORIAL_02, END_PROC );
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.size() )
        << BUILD_LOG( kPacket.m_vecItem.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_AGIT_TUTORIAL_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_FAIRY_TREE_BUFF_REQ )
{
    KFairyTree kTree;
    int nFruitType = KFairyTreeConfig::FBT_NONE;;
    time_t tmDate;

    LIF( AgitFairyTree_select( dwUID_, kTree ) );
    LIF( AgitFairyTreeFruit_select( dwUID_, tmDate, nFruitType ) );
    bool bUserFruit = GetTodayUseFruit( dwUID_ );

    CTime tmUseFruit = CTime( tmDate );
    CTime tmCurrent = CTime::GetCurrentTime();

    KFairyTreeFruitBuff kPacket;
    kPacket.m_dwUserUID = dwUID_;
    kPacket.m_nTreeLv = kTree.m_nLv;
    kPacket.m_fBuffRate = 0.f;
    kPacket.m_nFruitType = KFairyTreeConfig::FBT_NONE;
    kPacket.m_tmBuffTime = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    // 열매 버프 알림.
    if ( tmUseFruit.GetYear() == tmCurrent.GetYear() && tmUseFruit.GetMonth() == tmCurrent.GetMonth() && tmUseFruit.GetDay() == tmCurrent.GetDay() ) {
        if ( bUserFruit ) {
            kPacket.m_nFruitType = nFruitType;
            kPacket.m_fBuffRate = SiKAgitHelper()->GetFruitBuffRate( kTree.m_nLv, kPacket.m_nFruitType );
            START_LOG( clog, L"아지트 열매버프 적용. Login : " << strLogin_ ) << END_LOG;
        }
        else {
            START_LOG( clog, L"열매를 먹지않음. Login : " << strLogin_ ) << END_LOG;
        }
    }

    QUEUING_ACK_TO_USER( EVENT_FAIRY_TREE_BUFF_NOT );

    START_LOG( clog, L"아지트 열매버프 확인. Login : " << strLogin_ )
        << BUILD_LOG( kTree.m_nLv )
        << BUILD_LOG( bUserFruit )
        << BUILD_LOG( nFruitType )
        << BUILD_LOG( kPacket.m_nTreeLv )
        << BUILD_LOG( kPacket.m_nFruitType )
        << BUILD_LOG( kPacket.m_fBuffRate )
        << BUILD_LOGtm( CTime(kPacket.m_tmBuffTime) )
        << BUILD_LOGtm( CTime(tmDate) )
        << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_AGIT_USER_OPTION_REQ )
{
    KAgitInfo kInfo;
    JIF( AgitInfo_select( dwUID_, kInfo ) );
    kInfo.m_ucInviteType = KAgitInfo::IT_ACCEPT;
    START_LOG( clog, L"아지트 초대 거부 설정 미구현으로 인해 초대 무조건 수락 m_ucAgitInviteOption = KAgitInfo::IT_ACCEPT" << strLogin_ ) << END_LOG;
    _QUEUING_ACK_TO_USER( EVENT_AGIT_USER_OPTION_ACK, kInfo );

    START_LOG( clog, L"아지트 설정 확인. Login : " << strLogin_ )
        << BUILD_LOG( kInfo.m_nTotalVisiter )
        << BUILD_LOGc( kInfo.m_ucInviteType ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_ENTER_AGIT_NOT )
{
    KEnterAgitAck kPacket;
    kPacket = kPacket_.m_kEnterAgitAck;
    kPacket.m_vecEraseItem = kPacket_.m_vecEraseItem;

    if ( false == kPacket_.m_vecAddItem.empty() ) {
        std::vector<KDropItemInfo>::iterator vit;
        for ( vit = kPacket_.m_vecAddItem.begin() ; vit != kPacket_.m_vecAddItem.end() ; ++vit ) {
            // 아이템 지급
            if ( false == m_kHelper.InsertSingleItemJob( dwUID_, KItem::EICT_AGIT, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_AGIT_AUTO_DROP, kPacket.m_vecRewardItem ) ) {
                START_LOG( cerr, L"아지트 화분아이템 지급 DB삽입 실패 Login : " << strLogin_ )
                    << BUILD_LOG( vit->m_ItemID )
                    << BUILD_LOG( vit->m_nDuration )
                    << BUILD_LOG( vit->m_nPeriod ) << END_LOG;
            }
        }

        std::vector<KItem>::iterator vitReward;
        for ( vitReward = kPacket.m_vecRewardItem.begin() ; vitReward != kPacket.m_vecRewardItem.end() ; ++vitReward ) {
            // 아지트 오브젝트 설정
            if ( false == m_kHelper.AgiteObjPos_merge( dwUID_, vitReward->m_ItemUID, -1, -1 ) ) {
                START_LOG( cerr, L"DB작업중 아지트 아이템 설정 실패. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
                continue;
            }

            vitReward->AddItemType( KItem::TYPE_AGIT_OBJ );
            START_LOG( clog, L"아지트 아이템으로 설정. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;

            // 화분 초기화
            if ( SiKAgitHelper()->IsNormalPotItemID( vitReward->m_ItemID ) ) {
                LIF( AgiteFlowerpot_init( dwUID_, vitReward->m_ItemUID ) );
            }
        }

        START_LOG( clog, L"아지트 화분아이템 지급 DB에 넣어주기. Login : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_vecAddItem.size() )
            << BUILD_LOG( kPacket.m_vecRewardItem.size() ) << END_LOG;
    }

    if ( false == kPacket_.m_vecEraseItem.empty() ) {
        // 아이템 제거
        std::vector<GCITEMUID>::iterator vit;
        for( vit = kPacket_.m_vecEraseItem.begin() ; vit != kPacket_.m_vecEraseItem.end() ; ++vit ) {
            LIF( PureEraseItem( strLogin_, dwUID_, *vit, KItem::EICT_AGIT ) );
        }

        START_LOG( clog, L"아지트 화분아이템 제거. Login : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_vecEraseItem.size() ) << END_LOG;
    }

    START_LOG( clog, L"아지트 입장시, 화분 지급삭제 처리. Login : " << strLogin_ )
        << BUILD_LOG( kPacket.m_vecRewardItem.size() )
        << BUILD_LOG( kPacket.m_vecEraseItem.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_ENTER_AGIT_ACK );
}

INT_IMPL_ON_FUNC( EVENT_BUY_AGIT_MAP_NOT )
{
    KBuyAgitMapAck kPacket;
    kPacket = kPacket_.m_kBuyAgitMapAck;
    kPacket.m_vecEraseItem = kPacket_.m_vecEraseItem;

    if ( false == kPacket_.m_vecAddItem.empty() ) {
        std::vector<KDropItemInfo>::iterator vit;
        for ( vit = kPacket_.m_vecAddItem.begin() ; vit != kPacket_.m_vecAddItem.end() ; ++vit ) {
            // 아이템 지급
            if ( false == m_kHelper.InsertSingleItemJob( dwUID_, KItem::EICT_AGIT, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_AGIT_AUTO_DROP, kPacket.m_vecRewardItem ) ) {
                START_LOG( cerr, L"아지트 화분아이템 지급 DB삽입 실패 Login : " << strLogin_ )
                    << BUILD_LOG( vit->m_ItemID )
                    << BUILD_LOG( vit->m_nDuration )
                    << BUILD_LOG( vit->m_nPeriod ) << END_LOG;
            }
        }

        std::vector<KItem>::iterator vitReward;
        for ( vitReward = kPacket.m_vecRewardItem.begin() ; vitReward != kPacket.m_vecRewardItem.end() ; ++vitReward ) {
            // 아지트 오브젝트 설정
            if ( false == m_kHelper.AgiteObjPos_merge( dwUID_, vitReward->m_ItemUID, -1, -1 ) ) {
                START_LOG( cerr, L"DB작업중 아지트 아이템 설정 실패. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
                continue;
            }

            vitReward->AddItemType( KItem::TYPE_AGIT_OBJ );
            START_LOG( clog, L"아지트 아이템으로 설정. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;

            // 화분 초기화
            if ( SiKAgitHelper()->IsNormalPotItemID( vitReward->m_ItemID ) ) {
                LIF( AgiteFlowerpot_init( dwUID_, vitReward->m_ItemUID ) );
            }
        }

        START_LOG( clog, L"아지트 화분아이템 지급 DB에 넣어주기. Login : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_vecAddItem.size() )
            << BUILD_LOG( kPacket.m_vecRewardItem.size() ) << END_LOG;
    }

    if ( false == kPacket_.m_vecEraseItem.empty() ) {
        // 아이템 제거
        std::vector<GCITEMUID>::iterator vit;
        for( vit = kPacket_.m_vecEraseItem.begin() ; vit != kPacket_.m_vecEraseItem.end() ; ++vit ) {
            LIF( PureEraseItem( strLogin_, dwUID_, *vit, KItem::EICT_AGIT ) );
        }

        START_LOG( clog, L"아지트 화분아이템 제거. Login : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_vecEraseItem.size() ) << END_LOG;
    }

    START_LOG( clog, L"아지트 이사시, 화분 지급삭제 처리. Login : " << strLogin_ )
        << BUILD_LOG( kPacket.m_vecRewardItem.size() )
        << BUILD_LOG( kPacket.m_vecEraseItem.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_BUY_AGIT_MAP_ACK );
}

INT_IMPL_ON_FUNC( EVENT_STAT_CHAR_EQUIP_NOT )
{
    LIF( DailyCPLItemCollect_insert( kPacket_ ) );
    START_LOG( clog, L"캐릭터 장착아이템 통계 DB기록.. size : " << kPacket_.size() )
        << BUILD_LOG( m_kODBCStat.m_strLastQuery ) << END_LOG;

}

INT_IMPL_ON_FUNC( EVENT_STAT_DUNGEON_STAGE_NOT )
{
    LIF( DailyStageCollect_insert( kPacket_ ) );

    START_LOG( clog, L"던전 스테이지 통계 DB기록.. size : " )
        << BUILD_LOG( kPacket_.m_nType )
        << BUILD_LOG( kPacket_.m_dwCharLevel )
        << BUILD_LOG( kPacket_.m_mapDungeonPlayState.size() )
        << BUILD_LOG( m_kODBCStat.m_strLastQuery ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_STAT_CHAR_LV_PT_NOT )
{
    LIF( DailyCPLPTCollect_insert( kPacket_ ) );
    START_LOG( clog, L"캐릭터 레벨별 PlayTime통계 DB기록.. size : " << kPacket_.size() )
        << BUILD_LOG( m_kODBCStat.m_strLastQuery ) << END_LOG;
}
INT_IMPL_ON_FUNC( EVENT_NMNETCAFE_BONUS_REQ )
{
    KEVENT_NMNETCAFE_BONUS_ACK kPacket;
    kPacket.m_vecReward.clear();
    kPacket.m_mapPetInfo = kPacket_.m_mapPetInfo;

    // PC방 지급 아이템
    InsertIPAdvantageDailyItem( dwUID_, cCharType_, kPacket.m_vecReward );

    // PC방 장비 아이템
    std::map<char, KCharacterInfo>::iterator mit;
    for( mit = kPacket_.m_mapCharInfo.begin() ; mit != kPacket_.m_mapCharInfo.end() ; ++mit ) {
        SiKIPAdvantage()->GetConnectionItem( mit->second.m_cCharType, (int)mit->second.m_dwLevel, kPacket.m_vecReward, KIPAdvantage::PCBANG_PUBLISHER );
    }

    // PC방 펫
    SiKIPAdvantage()->GetPCBangPetItem( cCharType_, kPacket.m_mapPetInfo, kPacket.m_vecReward, KIPAdvantage::PCBANG_PUBLISHER );
    // PC방 메세지
    kPacket.m_strMsg = SiKIPAdvantage()->GetMsg();

    START_LOG( clog, L"넷마블 PC방 혜택정보 받아오기.. LoginID : " << strLogin_ )
        << BUILD_LOG( kPacket_.m_mapCharInfo.size() )
        << BUILD_LOG( kPacket_.m_mapPetInfo.size() )
        << BUILD_LOG( kPacket.m_vecReward.size() )
        << BUILD_LOG( kPacket.m_mapPetInfo.size() )
        << BUILD_LOG( kPacket.m_strMsg ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_NMNETCAFE_BONUS_ACK );
}
INT_IMPL_ON_FUNC( DB_EVENT_SURVEY_ANSWER_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_SURVEY_ANSWER_REQ );
    KEVENT_SURVEY_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    std::map< int, std::vector<int> >::iterator mitNumber;
    std::vector<int>::iterator vit;
    std::map< int, std::wstring >::iterator mitString;
    std::vector< KDropItemInfo >::iterator vitReward;
    int nVersion = SiKSurvey()->GetSurveyVersion();

    // 객관식 설문에 대한 답변 저장.
    if( !kPacket_.m_mapNumberAnswer.empty() ) {
        for( mitNumber = kPacket_.m_mapNumberAnswer.begin(); mitNumber != kPacket_.m_mapNumberAnswer.end(); ++mitNumber ) {
            std::wstring wstrAnswer = L"";;
            for( vit = mitNumber->second.begin(); vit != mitNumber->second.end(); ++vit ) {
                std::wstring wstrTemp = boost::lexical_cast<std::wstring>(*vit);
                wstrTemp += L",";
                wstrAnswer += wstrTemp;
            }
            InsertUserSurveyAnswer( dwUID_, nVersion, kPacket_.m_nType, mitNumber->first, kPacket_.m_nDataFirst, kPacket_.m_nDataSecond, wstrAnswer, kPacket_.m_nLanguageCode );
        }
    }

    // 주관식 설문에 대한 답변 저장.
    if( kPacket_.m_mapStringAnswer.empty() ) {
        for( mitString = kPacket_.m_mapStringAnswer.begin(); mitString != kPacket_.m_mapStringAnswer.end(); ++mitString ) {
            InsertUserSurveyAnswer( dwUID_, nVersion, kPacket_.m_nType, mitString->first, kPacket_.m_nDataFirst, kPacket_.m_nDataSecond, mitString->second, kPacket_.m_nLanguageCode );
        }
    }

    // 보상 아이템 지급.
    for ( vitReward = kPacket_.m_vecRewardItem.begin(); vitReward != kPacket_.m_vecRewardItem.end(); ++vitReward ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, 
            cCharType_,
            vitReward->m_ItemID, 
            vitReward->m_nDuration, 
            vitReward->m_nPeriod, 
            KItemManager::EWIT_SURVEY, 
            kPacket.m_vecReward ) ) {

                START_LOG( cerr, L"설문조사 보상 아이템 지급 실패 UID : " << dwUID_ )
                    << BUILD_LOG( vitReward->m_ItemID )
                    << BUILD_LOG( vitReward->m_nDuration )
                    << BUILD_LOG( vitReward->m_nPeriod ) << END_LOG;

                continue;
        }
    }

    SET_ERROR( NET_OK );
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"설문 답변 정보 DB 업데이트, UserUID: " << dwUID_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecReward.size() ) << END_LOG;

    QUEUING_ACK_TO_USER_UID( EVENT_SURVEY_REWARD_ACK );
}

_INT_IMPL_ON_FUNC( DB_EVENT_GWC_RANKING_REWARD_REQ, VECTOR_STRUCT_DROPITEM )
{
    EVENT_CHECK_OUT( DB_EVENT_GWC_RANKING_REWARD_REQ );
    KEVENT_GWC_RANKING_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_vecReward.clear();

    std::vector<KDropItemInfo>::iterator vit;
    SET_ERROR( ERR_UNKNOWN );

    // 보상 아이템 받았는지 확인.
    if( GetTodayEventCount( dwUID_, UET_GWC_EVENT ) > 0  ) {
        SET_ERR_GOTO( ERR_GWCEVENT_02, END_PROC );
    }

    LIF( SetTodayEventDoneDiff( dwUID_, UET_GWC_EVENT ) );

    // GWC 보상 아이템 지급.
    for ( vit = kPacket_.begin(); vit != kPacket_.end(); ++vit ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, 
            cCharType_,
            vit->m_ItemID, 
            vit->m_nDuration, 
            vit->m_nPeriod, 
            KItemManager::EWIT_GWCEVENT, 
            kPacket.m_vecReward ) ) {

                START_LOG( cerr, L"GWC 이벤트 보상 아이템 지급 실패 UID : " << dwUID_ )
                    << BUILD_LOG( vit->m_ItemID )
                    << BUILD_LOG( vit->m_nDuration )
                    << BUILD_LOG( vit->m_nPeriod ) << END_LOG;

                continue;
        }
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"GWC 이벤트 보상 아이템 DB 업데이트, UserUID: " << dwUID_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecReward.size() ) << END_LOG;

    QUEUING_ACK_TO_USER_UID( EVENT_GWC_RANKING_REWARD_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_SEALLING_CALENDAR_REQ, KEVENT_SEALLING_CALENDAR_ACK )
{
    EVENT_CHECK_OUT( EVENT_SEALLING_CALENDAR_REQ );

    SET_ERROR( ERR_UNKNOWN );
    kPacket_.m_nOK = -99;
    CTime tmCurrent = CTime::GetCurrentTime();
    std::wstring strDate = (LPCTSTR)tmCurrent.Format( _T("%y%m%d") );

    // 일일출석포인트 저장
    if ( kPacket_.m_nIncGCPoint > 0 ) {
        if( false == UpdateUserPointBagAccTime( dwUID_, cCharType_, kPacket_.m_nIncGCPoint, kPacket_.m_nAccTime, strDate ) ) {
            SET_ERR_GOTO( ERR_CALENDAR_14, END_PROC );
        }
    }

    kPacket_.m_nGCPoint += kPacket_.m_nIncGCPoint;
    SET_ERROR( NET_OK );

END_PROC:
    kPacket_.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket_.m_nOK == NetError::NET_OK, L"일일출석포인트 DB저장 결과 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_nGCPoint )
        << BUILD_LOG( kPacket_.m_nIncGCPoint ) << END_LOG;

    _QUEUING_ACK_TO_USER( EVENT_SEALLING_CALENDAR_ACK, kPacket_ );
}

INT_IMPL_ON_FUNC( DB_EVENT_JUST_IN_TIME_REWARD_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_JUST_IN_TIME_REWARD_REQ );
    KEVENT_JUST_IN_TIME_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_tmNextEvent = 0;

    SET_ERROR( ERR_UNKNOWN );

    // 보상 아이템 지급.
    std::vector<KDropItemInfo>::iterator vitReward;
    for ( vitReward = kPacket_.m_vecItemList.begin(); vitReward != kPacket_.m_vecItemList.end(); ++vitReward ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_,
                                                    cCharType_,
                                                    vitReward->m_ItemID,
                                                    vitReward->m_nDuration,
                                                    vitReward->m_nPeriod,
                                                    KItemManager::EWIT_EVENT_JIT_REWARD,
                                                    kPacket.m_vecRecieveItemList ) ) {

                START_LOG( cerr, L"JIT 보상 아이템 지급 실패 Login : " << strLogin_ )
                    << BUILD_LOG( dwUID_ )
                    << BUILD_LOG( vitReward->m_ItemID )
                    << BUILD_LOG( vitReward->m_nDuration )
                    << BUILD_LOG( vitReward->m_nPeriod )
                    << END_LOG;
                continue;
        }
    }

    // JIT보상 받아간 정보 기록
    std::map<USHORT,bool>::iterator mit;
    for ( mit = kPacket_.m_mapUserTypeEvent.begin() ; mit != kPacket_.m_mapUserTypeEvent.end() ; ++mit ) {
        const USHORT& usUserType = mit->first;
        if ( false == mit->second ) {
            continue;
        }

        bool bRewardStatUpdate = SiKJustInTime()->UpdateReceivedRewardStatus( usUserType, std::make_pair( dwUID_, kPacket_.m_tmJIT ) );
        if ( false == bRewardStatUpdate ) {
            START_LOG( cwarn, L"JIT 보상 받아간 기록이 이미 있음.." )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( strLogin_ )
                << BUILD_LOG( usUserType )
                << BUILD_LOGtm( CTime(kPacket_.m_tmJIT) )
                << END_LOG;
        }
    }

    SET_ERROR( NET_OK );
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"JIT 보상 아이템 지급 성공 UserUID: " << dwUID_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecRecieveItemList.size() )
        << BUILD_LOGtm( CTime(kPacket.m_tmNextEvent) )
        << BUILD_LOGtm( CTime(kPacket_.m_tmJIT) )
        << BUILD_LOG( kPacket_.m_vecItemList.size() )
        << BUILD_LOG( kPacket_.m_mapUserTypeEvent.size() )
        << END_LOG;

    QUEUING_ACK_TO_USER_UID( EVENT_JUST_IN_TIME_REWARD_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_CHARISMAS_EVENT_INFO_REQ )
{
    EVENT_CHECK_OUT( EVENT_CHARISMAS_EVENT_INFO_REQ );
    SET_ERROR( ERR_UNKNOWN );
    KEVENT_CHARISMAS_EVENT_INFO_ACK kPacket;
    kPacket.m_nOK = -99;
    DWORD dwGrade = 0;
    DWORD dwBonusPoint = 0;
    DWORD dwVersion = SiKCharismasPresent()->GetVersion();

    GetCharismasUserData( dwUID_, dwVersion, dwGrade, dwBonusPoint );

    kPacket.m_dwGrade = dwGrade;
    kPacket.m_dwBonusPoint = dwBonusPoint;
    SET_ERROR( NET_OK );

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_CHARISMAS_EVENT_INFO_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_CHARISMAS_EVENT_CONNECT_CHECK_REQ, std::wstring )
{
    EVENT_CHECK_OUT( EVENT_CHARISMAS_EVENT_CONNECT_CHECK_REQ );
    SET_ERROR( ERR_UNKNOWN );
    KEVENT_CHARISMAS_EVENT_CONNECT_CHECK_ACK kPacket;
    kPacket.m_nOK = -99;
    // 닉네임으로 LoginUID 정보 가져오기.
    if ( false == m_kHelper.NickToUserUID( kPacket_, kPacket.m_dwUserUID ) ) {
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_03, END_PROC );
    }

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwUserUID )
        << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_CHARISMAS_EVENT_CONNECT_CHECK_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_CHARISMAS_EVENT_PRESENT_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_CHARISMAS_EVENT_PRESENT_REQ );
    KEVENT_CHARISMAS_EVENT_PRESENT_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );
    DWORD dwVersion = SiKCharismasPresent()->GetVersion();
    DWORD dwMaxPoint = SiKCharismasPresent()->GetMaxPoint();
    DWORD dwAddBonusPoint = 0;

    if ( SiKPostConfig()->IsPostEnable() ) {
        // 우편 메세지 만들기
        KPostItemInfo kInfo;
        kInfo.m_dwToUserUID = kPacket_.m_dwReceiveUserUID;
        kInfo.m_cScriptType = KPostItemInfo::LT_SYSTEM;
        kInfo.m_nGamePoint = 0;
        kInfo.m_vecItem.clear();
        kInfo.m_strFromNickName = SiKPostConfig()->GetPostString(L"str_00");
        kInfo.m_strTitle = SiKPostConfig()->GetPostString(L"str_01");
        kInfo.m_strMessage = SiKPostConfig()->GetPostString(L"str_02");;
        kInfo.m_cCharType = -1;
        boost::replace_all( kInfo.m_strMessage, L"__Nick__", kPacket_.m_strFromUserNickname );

        KItem kItem;
        kItem.m_ItemID = kPacket_.m_kDropItemInfo.m_ItemID;
        kItem.m_nCount = kPacket_.m_kDropItemInfo.m_nDuration;
        kItem.m_nPeriod = kPacket_.m_kDropItemInfo.m_nPeriod;
        kItem.m_cEnchantLevel = 0;
        kItem.m_cGradeID = -1;
        kInfo.m_vecItem.push_back( kItem );

        // 우편 지급
        if ( false == InsertSystemPost( kInfo ) ) {
            START_LOG( cerr, L"우편으로 선물 보내기 DB처리 실패.. LoginID : " << strLogin_ ) << END_LOG;
            SET_ERR_GOTO( ERR_CHARISMASPRESENT_08, END_PROC );
        }
    }
    else {
        if ( !m_kHelper.InsertSingleItemJob( kPacket_.m_dwReceiveUserUID, kPacket_.m_cReceiveUserCharType, kPacket_.m_kDropItemInfo.m_ItemID, kPacket_.m_kDropItemInfo.m_nDuration, kPacket_.m_kDropItemInfo.m_nPeriod, KItemManager::EWIT_CHARISMASEVENT, kPacket.m_vecPresentItem ) ) {
            START_LOG( cerr, L"선물 인벤토리에 넣어주기 DB처리 실패.. LoginID : " << strLogin_ ) << END_LOG;
            SET_ERR_GOTO( ERR_CHARISMASPRESENT_08, END_PROC );
        }
    }

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_kSledgeItem.m_ItemUID, kPacket_.m_kSledgeItem.m_nInitCount, kPacket_.m_kSledgeItem.m_nCount ) ) {
        START_LOG( clog, L"썰매 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_kSledgeItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_kSledgeItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_kSledgeItem.m_nCount )
            << END_LOG;
    }

    if ( !UseCountItem( dwUID_, kPacket_.m_kSledgeItem.m_ItemUID, kPacket_.m_dwSledgeUseCnt ) ) {
        START_LOG( cerr, L"썰매 아이템 개수 동기화 실패. Name : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_kSledgeItem.m_ItemUID )
            << END_LOG;
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_09, END_PROC );
    }

    kPacket_.m_kSledgeItem.m_nCount -= kPacket_.m_dwSledgeUseCnt;
    kPacket_.m_kSledgeItem.m_nCount = std::max<int>(0, kPacket_.m_kSledgeItem.m_nCount );
    kPacket.m_kSledgeItem = kPacket_.m_kSledgeItem;
    kPacket.m_dwGrade = kPacket_.m_dwGrade;
    kPacket.m_dwBonusPoint = kPacket_.m_dwAddBonusPoint; // 일단 추가되는 보너스 포인트 저장.
    kPacket.m_dwReceiveUserUID = kPacket_.m_dwReceiveUserUID;

    // DB에 기록되는 포인트에 대한 보정 처리.
    if( dwMaxPoint <= kPacket_.m_dwBonusPoint + kPacket_.m_dwAddBonusPoint ) {
        if( dwMaxPoint <= kPacket_.m_dwBonusPoint ) {
            dwAddBonusPoint = 0;
        } else {
            dwAddBonusPoint = dwMaxPoint - kPacket_.m_dwBonusPoint;
        }
    } else {
        dwAddBonusPoint = kPacket_.m_dwAddBonusPoint;
    }

    // 현재 유저의 포인트/ 등급 갱신.
    if( !UpdateCharismasUserData( dwUID_, dwVersion, kPacket_.m_dwGrade, dwAddBonusPoint ) ) {
        START_LOG( cerr, L"크리스 마스 이벤트 유저 데이트 갱신 실패. Name : " << strLogin_ )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( dwVersion )
            << BUILD_LOG( kPacket_.m_dwGrade )
            << BUILD_LOG( dwAddBonusPoint )
            << END_LOG;
    }

    // 선물한 로그.
    if( !InsertCharismasGift( dwUID_, dwVersion, kPacket_.m_dwReceiveUserUID, kPacket_.m_kDropItemInfo.m_ItemID ) ) {
        START_LOG( cerr, L"크리스 마스 이벤트 선물 로그 남기기 실패. Name : " << strLogin_ ) << END_LOG;
    }

END_PROC:
    if ( IS_CORRECT( ERR_CHARISMASPRESENT_09 ) ) {
        kPacket.m_kSledgeItem.m_nCount = 0;
    }
    if ( !IS_CORRECT( ERR_CHARISMASPRESENT_08 ) && !IS_CORRECT( ERR_CHARISMASPRESENT_09 ) ) {
        SET_ERROR( NET_OK );
    }

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_CHARISMAS_EVENT_PRESENT_ACK );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_kDropItemInfo.m_ItemID )
        << BUILD_LOG( kPacket_.m_dwReceiveUserUID )
        << BUILD_LOG( kPacket_.m_kSledgeItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kSledgeItem.m_nCount )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kSledgeItem.m_ItemID )
        << BUILD_LOG( kPacket.m_kSledgeItem.m_nCount )
        << BUILD_LOG( kPacket.m_vecPresentItem.size() )
        << BUILD_LOG( dwAddBonusPoint )
        << BUILD_LOG( SiKPostConfig()->IsPostEnable() )
        << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_CHARISMAS_EVENT_DONATION_REQ )
{
    EVENT_CHECK_OUT( EVENT_CHARISMAS_EVENT_DONATION_REQ );
    KEVENT_CHARISMAS_EVENT_DONATION_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );
    DWORD dwVersion = SiKCharismasPresent()->GetVersion();
    DWORD dwMaxPoint = SiKCharismasPresent()->GetMaxPoint();
    DWORD dwAddBonusPoint = 0;

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_kSledgeItem.m_ItemUID, kPacket_.m_kSledgeItem.m_nInitCount, kPacket_.m_kSledgeItem.m_nCount ) ) {
        START_LOG( clog, L"썰매 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_kSledgeItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_kSledgeItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_kSledgeItem.m_nCount )
            << END_LOG;
    }

    if ( !UseCountItem( dwUID_, kPacket_.m_kSledgeItem.m_ItemUID, kPacket_.m_dwUseItemCnt ) ) {
        START_LOG( cerr, L"썰매 아이템 개수 동기화 실패. Name : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_kSledgeItem.m_ItemUID ) << END_LOG;
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_09, END_PROC );
    }

    kPacket_.m_kSledgeItem.m_nCount -= kPacket_.m_dwUseItemCnt;
    kPacket_.m_kSledgeItem.m_nCount = std::max<int>(0, kPacket_.m_kSledgeItem.m_nCount );
    kPacket.m_kSledgeItem = kPacket_.m_kSledgeItem;
    kPacket.m_dwBonusPoint = kPacket_.m_dwAddBonusPoint; // 일단 추가되는 보너스 포인트 저장.
    kPacket.m_dwGrade = kPacket_.m_dwGrade;

    // DB에 기록되는 포인트에 대한 보정 처리.
    if( dwMaxPoint <= kPacket_.m_dwBonusPoint + kPacket_.m_dwAddBonusPoint ) {
        if( dwMaxPoint <= kPacket_.m_dwBonusPoint ) {
            dwAddBonusPoint = 0;
        } else {
            dwAddBonusPoint = dwMaxPoint - kPacket_.m_dwBonusPoint;
        }
    } else {
        dwAddBonusPoint = kPacket_.m_dwAddBonusPoint;
    }

    // 현재 유저의 포인트/ 등급 갱신.
    if( !UpdateCharismasUserData( dwUID_, dwVersion, kPacket_.m_dwGrade, dwAddBonusPoint ) ) {
        START_LOG( cerr, L"크리스 마스 이벤트 유저 데이트 갱신 실패. Name : " << strLogin_ )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( dwVersion )
            << BUILD_LOG( kPacket_.m_dwGrade )
            << BUILD_LOG( dwAddBonusPoint )
            << END_LOG;
    }

    // 기부 로그 남기기.
    UpdateCharismasDonationInfo( dwUID_, dwVersion, kPacket_.m_dwUseItemCnt );

END_PROC:
    if ( IS_CORRECT( ERR_CHARISMASPRESENT_09 ) ) {
        kPacket.m_kSledgeItem.m_nCount = 0;
    }
    if ( !IS_CORRECT( ERR_CHARISMASPRESENT_09 ) ) {
        SET_ERROR( NET_OK );
    }

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_CHARISMAS_EVENT_DONATION_ACK );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_dwUseItemCnt )
        << BUILD_LOG( kPacket_.m_dwAddBonusPoint )
        << BUILD_LOG( kPacket_.m_kSledgeItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kSledgeItem.m_nCount )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kSledgeItem.m_ItemID )
        << BUILD_LOG( kPacket.m_kSledgeItem.m_nCount )
        << BUILD_LOG( kPacket.m_dwBonusPoint )
        << BUILD_LOG( kPacket.m_dwGrade )
        << BUILD_LOG( dwAddBonusPoint )
        << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_CHARISMAS_EVENT_GRADE_REWARD_REQ )
{
    EVENT_CHECK_OUT( EVENT_CHARISMAS_EVENT_GRADE_REWARD_REQ );
    KEVENT_CHARISMAS_EVENT_GRADE_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.bInitEnable = false;
    SET_ERROR( ERR_UNKNOWN );
    DWORD dwVersion = SiKCharismasPresent()->GetVersion();
    DWORD dwMaxGrade = SiKCharismasPresent()->GetMaxGrade();
    DWORD dwMaxPoint = SiKCharismasPresent()->GetMaxPoint();
    bool bLoopEnable = SiKCharismasPresent()->GetLoopEnable();
    int nAddBonusPoint = 0;
    std::vector<KDropItemInfo>::iterator vit;

    DWORD dwCurrentGrade = 0;
    DWORD dwCurrentBonusPoint = 0;
    GetCharismasUserData( dwUID_, dwVersion, dwCurrentGrade, dwCurrentBonusPoint );
    if( dwCurrentGrade != kPacket_.m_dwGrade ) {
        SET_ERR_GOTO( ERR_CHARISMASPRESENT_08, END_PROC );
    }

    // 크리스마스 등급 보상 아이템 지급.
    for ( vit = kPacket_.m_vecReward.begin(); vit != kPacket_.m_vecReward.end(); ++vit ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, 
            cCharType_,
            vit->m_ItemID, 
            vit->m_nDuration, 
            vit->m_nPeriod, 
            KItemManager::EWIT_CHARISMASEVENT, 
            kPacket.m_vecReward ) ) {
                START_LOG( cerr, L"크리스마스 이벤트 등급 보상 아이템 지급 실패 UID : " << dwUID_ )
                    << BUILD_LOG( vit->m_ItemID )
                    << BUILD_LOG( vit->m_nDuration )
                    << BUILD_LOG( vit->m_nPeriod )
                    << END_LOG;

                SET_ERR_GOTO( ERR_CHARISMASPRESENT_08, END_PROC );
        }
    }

    kPacket.m_dwGrade = kPacket_.m_dwGrade + 1;
    if( bLoopEnable && kPacket.m_dwGrade > dwMaxGrade ) {
        kPacket.m_dwGrade = 1;
        kPacket.m_dwBonusPoint = 0;
        nAddBonusPoint = static_cast<int>( dwMaxPoint );
        nAddBonusPoint = -nAddBonusPoint;
        kPacket.bInitEnable = true;
    } else {
        kPacket.m_dwBonusPoint = kPacket_.m_dwBonusPoint;
    }

    // 현재 유저의 포인트/ 등급 갱신.
    if( !UpdateCharismasUserData( dwUID_, dwVersion, kPacket.m_dwGrade, nAddBonusPoint ) ) {
        START_LOG( cerr, L"크리스 마스 이벤트 유저 데이트 갱신 실패. Name : " << strLogin_ )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( dwVersion )
            << BUILD_LOG( kPacket_.m_dwGrade )
            << BUILD_LOG( nAddBonusPoint )
            << END_LOG;
    }

END_PROC:
    if ( !IS_CORRECT( ERR_CHARISMASPRESENT_08 ) ) {
        SET_ERROR( NET_OK );
    }

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_CHARISMAS_EVENT_GRADE_REWARD_ACK );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_dwGrade )
        << BUILD_LOG( kPacket_.m_dwBonusPoint )
        << BUILD_LOG( kPacket_.m_vecReward.size() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwGrade )
        << BUILD_LOG( kPacket.m_dwBonusPoint )
        << BUILD_LOG( kPacket.m_vecReward.size() )
        << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_STAT_HOTKEY_USED_INFO_NOT )
{
    {   // 던전, 모아쓰기
        std::map< char, std::map< char, int > >::iterator mitCharType;
        for ( mitCharType = kPacket_.m_mapDungeonUsedCharge.begin(); mitCharType != kPacket_.m_mapDungeonUsedCharge.end(); ++mitCharType ) {
            if ( mitCharType->second.empty() ) {    // 스킬 사용정보가 없는 경우 체크
                continue;
            }
            std::map< char, int >::iterator mitCharge;
            for ( mitCharge = mitCharType->second.begin(); mitCharge != mitCharType->second.end(); ++mitCharge ) {
                InsertHotkeyUsedInfoStat( dwUID_,
                                          KEVENT_STAT_HOTKEY_USED_INFO_NOT::PM_DUNGEON,
                                          mitCharType->first,
                                          KEVENT_STAT_HOTKEY_USED_INFO_NOT::SUT_CHARGE,
                                          mitCharge->first,
                                          mitCharge->second );
            }
        }
    }

    {   // 던전, 단축키쓰기
        std::map< char, std::map< char, int > >::iterator mitCharType;
        for ( mitCharType = kPacket_.m_mapDungeonUsedHotkey.begin(); mitCharType != kPacket_.m_mapDungeonUsedHotkey.end(); ++mitCharType ) {
            if ( mitCharType->second.empty() ) {    // 스킬 사용정보가 없는 경우 체크
                continue;
            }
            std::map< char, int >::iterator mitCharge;
            for ( mitCharge = mitCharType->second.begin(); mitCharge != mitCharType->second.end(); ++mitCharge ) {
                InsertHotkeyUsedInfoStat( dwUID_,
                    KEVENT_STAT_HOTKEY_USED_INFO_NOT::PM_DUNGEON,
                    mitCharType->first,
                    KEVENT_STAT_HOTKEY_USED_INFO_NOT::SUT_HOTKEY,
                    mitCharge->first,
                    mitCharge->second );
            }
        }
    }

    {   // 대전, 모아쓰기
        std::map< char, std::map< char, int > >::iterator mitCharType;
        for ( mitCharType = kPacket_.m_mapPVPUsedCharge.begin(); mitCharType != kPacket_.m_mapPVPUsedCharge.end(); ++mitCharType ) {
            if ( mitCharType->second.empty() ) {    // 스킬 사용정보가 없는 경우 체크
                continue;
            }
            std::map< char, int >::iterator mitCharge;
            for ( mitCharge = mitCharType->second.begin(); mitCharge != mitCharType->second.end(); ++mitCharge ) {
                InsertHotkeyUsedInfoStat( dwUID_,
                    KEVENT_STAT_HOTKEY_USED_INFO_NOT::PM_PVP,
                    mitCharType->first,
                    KEVENT_STAT_HOTKEY_USED_INFO_NOT::SUT_CHARGE,
                    mitCharge->first,
                    mitCharge->second );
            }
        }
    }

    {   // 대전, 단축키쓰기
        std::map< char, std::map< char, int > >::iterator mitCharType;
        for ( mitCharType = kPacket_.m_mapPVPUsedHotkey.begin(); mitCharType != kPacket_.m_mapPVPUsedHotkey.end(); ++mitCharType ) {
            if ( mitCharType->second.empty() ) {    // 스킬 사용정보가 없는 경우 체크
                continue;
            }
            std::map< char, int >::iterator mitCharge;
            for ( mitCharge = mitCharType->second.begin(); mitCharge != mitCharType->second.end(); ++mitCharge ) {
                InsertHotkeyUsedInfoStat( dwUID_,
                    KEVENT_STAT_HOTKEY_USED_INFO_NOT::PM_PVP,
                    mitCharType->first,
                    KEVENT_STAT_HOTKEY_USED_INFO_NOT::SUT_HOTKEY,
                    mitCharge->first,
                    mitCharge->second );
            }
        }
    }
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_INIT_POST_LETTER_LIST_REQ )
{
    KDB_EVENT_INIT_POST_LETTER_LIST_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_setBlackList.clear();
    kPacket.m_vecPostItem.clear();

    std::map<GCITEMUID,KItem> mapPostWaitItem;
    mapPostWaitItem.clear();

    std::map<POSTUID,std::vector<KItem> >::iterator mitPostItem;
    std::map<POSTUID,std::vector<KItem> > mapPostItem;
    mapPostItem.clear();

    SET_ERROR( ERR_UNKNOWN );

    // 1. 우편 정보 받아오기
    if ( false == m_kHelper.Post_select( dwUID_, -1, kPacket.m_vecPostItem ) ) {
        SET_ERR_GOTO( ERR_POSTOFFICE_02, END_PROC );
    }

    // 2. 우편 첨부아이템 정보 받아오기
    if ( false == m_kHelper.PostItem_select( dwUID_, -1, mapPostItem ) ) {
        SET_ERR_GOTO( ERR_POSTOFFICE_03, END_PROC );
    }

    // 2-1. 해당 ItemUID의 대기아이템 정보 받아오기
    if ( false == mapPostItem.empty() ) {
        if ( false == m_kHelper.WaitItem_post_select( dwUID_, -1, mapPostWaitItem ) ) {
            SET_ERR_GOTO( ERR_POSTOFFICE_04, END_PROC );
        }
    }

    // 2-2. 첨부아이템 우편정보에 매칭시키기
    for ( mitPostItem = mapPostItem.begin() ; mitPostItem != mapPostItem.end() ; ++mitPostItem ) {
        const POSTUID& PostUID = mitPostItem->first;
        std::vector<KItem>& vecPostItemUID = mitPostItem->second;

        std::vector<KPostItemInfo>::iterator vitPostInfo;
        vitPostInfo = std::find_if( kPacket.m_vecPostItem.begin(), kPacket.m_vecPostItem.end(),
            boost::bind( &KPostItemInfo::m_PostUID, boost::placeholders::_1 ) == PostUID );

        if ( vitPostInfo == kPacket.m_vecPostItem.end() ) {
            START_LOG( cwarn, L"첨부아이템이 설정된 PostUID가 리스트에 없음.. PostUID : " << PostUID << L", Login : " << strLogin_ ) << END_LOG;
            continue;
        }

        std::vector<KItem>::iterator vitItem;
        for ( vitItem = vecPostItemUID.begin() ; vitItem != vecPostItemUID.end() ; ++vitItem ) {
            // 2-2-1.우편 대기아이템 리스트에서 검색 (시스템우편)
            std::map<GCITEMUID,KItem>::iterator mitPostWaitItem;
            mitPostWaitItem = mapPostWaitItem.find( vitItem->m_ItemUID );
            if ( mitPostWaitItem != mapPostWaitItem.end() ) {
                vitPostInfo->m_vecItem.push_back( mitPostWaitItem->second );
                continue;
            }

            if( vitItem->m_ItemID != 0 ) {
                vitPostInfo->m_vecItem.push_back( *vitItem );
                continue;
            }
/*
            // 2-2-2.보낸 유저 인벤토리에서 검색 (유저우편)
            if ( m_kHelper.PostItem_detail_select( dwUID_, cCharType_, PostUID, vitPostInfo->m_cScriptType, vitItem->m_ItemUID, vitItem->m_ItemID, vitItem->m_nCount, vitItem->m_nPeriod ) ) {
                vitPostInfo->m_vecItem.push_back( *vitItem );
                continue;
            }
*/
            START_LOG( cwarn, L"첨부아이템 세부정보를 찾을 수 없음.. PostUID : " << PostUID << L", Login : " << strLogin_ << L", ItemUID" << vitItem->m_ItemUID ) << END_LOG;
            SET_ERR_GOTO( ERR_POSTOFFICE_11, END_PROC );
        }
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"DB 우편함 정보 받아오기.. Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_setBlackList.size() )
        << BUILD_LOG( kPacket.m_vecPostItem.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( DB_EVENT_INIT_POST_LETTER_LIST_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_SEND_LETTER_REQ, KDB_EVENT_SEND_LETTER_REQ )
{
    EVENT_CHECK_OUT( EVENT_SEND_LETTER_REQ );

    KEVENT_SEND_LETTER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nPostGP = kPacket_.m_nPostGP;
    kPacket.m_nGamePoint = kPacket_.m_nGamePoint;
    kPacket.m_nSendLetterCost = 0; // 수수료는 서버에서 계산
    kPacket.m_vecDecreaseCountItem.clear();
    kPacket.m_vecEraseItem.clear();
    kPacket.m_nSendLetterCost = 0;
    kPacket.m_PostUID = 0;

    SET_ERROR( ERR_UNKNOWN );

    int nLetterCost = SiKPostConfig()->GetnLetterCost(); // 우편 기본 수수료
    float fGPCostRatio = SiKPostConfig()->GetGPCostRatio(); // GP 첨부시 (GP대비)수수료 비율
    float fItemCostRatio = SiKPostConfig()->GetItemCostRatio(); // 아이템 첨부시 (아이템가격대비) 수수료 비율

    int nGPCost = 0; // GP 수수료
    int nItemCost = 0; // 영구아이템 수수료
    int nCountItemCost = 0; // 수량아이템 수수료
    DWORD dwToUserUID = 0;

    // GP 갱신
    {
        int nRet = -99;
        int nDBGamePoint = 0;
        int nGPDiff = kPacket_.m_kGPData.m_iGamePoint - kPacket_.m_kGPData.m_iInitGP;

        UpdateUserGamePoint( dwUID_, cCharType_, kPacket_.m_dwSlotNum, nGPDiff, nRet, nDBGamePoint );
        if ( nRet != 0 ) {
            START_LOG( cerr, L"GP값 DB 데이터 갱신 실패.. Login : " << strLogin_ )
                << BUILD_LOG( nGPDiff )
                << BUILD_LOG( kPacket_.m_kGPData.m_iGamePoint )
                << BUILD_LOG( kPacket_.m_kGPData.m_iInitGP )
                << BUILD_LOG( kPacket_.m_kGPData.m_nIncGpSum ) << END_LOG;
        }
    }

    // 받는 유저UID 검색
    if ( false == m_kHelper.NickToUserUID( kPacket_.m_strToNickName, dwToUserUID ) ) {
        START_LOG( cerr, L"해당 닉네임 유저를 찾을 수 없음.. Nick : " << kPacket_.m_strToNickName ) << END_LOG;
        SET_ERR_GOTO( ERR_POSTOFFICE_17, END_PROC );
    }

    // 받는 유저 마지막 플레이 캐릭터 가져오기.
    // 해당 작업은 클라이언트에서 유저간 우편 보내기 할 때 캐릭터 타입 설정하는 UI 추가되기 전까지만 사용.
    kPacket_.m_cCharType = GetLastPlayCharacter( dwToUserUID );

    // 수수료 계산
    {
        // 기본수수료
        kPacket.m_nSendLetterCost += nLetterCost;

        // GP수수료
        nGPCost = static_cast<int>( static_cast<float>(kPacket_.m_nPostGP) * fGPCostRatio );
        kPacket.m_nSendLetterCost += nGPCost;

        // 영구아이템 수수료 체크
        {
            std::vector<KItem>::iterator vit;
            for ( vit = kPacket_.m_vecItem.begin() ; vit != kPacket_.m_vecItem.end() ; ++vit ) {
                // 가격정보 얻어오기
                int nPrice, nMoneyType;
                if ( false == m_kHelper.GetPrice( vit->m_ItemID, vit->m_nCount, vit->m_nPeriod, nPrice, nMoneyType ) ) {
                    continue;
                }

                if ( nPrice <= 0 ) {
                    START_LOG( clog, L"첨부한 아이템의 가격이 없거나 0 이다. ItemID : " << vit->m_ItemID << L", Price : " << nPrice ) << END_LOG;
                    continue;
                }

                // 화폐별 수수료 계산
                // ToDo. 수수료 계산식 적용 필요
                switch ( nMoneyType )
                {
                case KItemManager::EMT_CASH:
                    // 캐쉬아이템 수수료 = (되팔기 잼 개수 * 잼 가격) * 아이템 수수료 비율
                    break;
                case KItemManager::EMT_GAMEPOINT:
                    // GP아이템 수수료 = 아이템 가격 * 아이템 수수료 비율
                    break;

                case KItemManager::EMT_VIRTUALCASH:
                case KItemManager::EMT_CRYSTAL:
                case KItemManager::EMT_GCPOINT:
                default:
                    START_LOG( clog, L"수수료 정의되지 않은 화폐 타입 : " << nMoneyType ) << END_LOG;
                    break;
                }
            }
            // 영구아이템 수수료
            kPacket.m_nSendLetterCost += nItemCost;
        }

        // 수량 아이템 수수료 체크
        {
            std::vector<std::pair<KItem,int> >::iterator vit;
            for ( vit = kPacket_.m_vecCountItem.begin() ; vit != kPacket_.m_vecCountItem.end() ; ++vit ) {
                // 가격정보 얻어오기
                int nPrice, nMoneyType;
                if ( false == m_kHelper.GetPrice( vit->first.m_ItemID, vit->first.m_nCount, vit->first.m_nPeriod, nPrice, nMoneyType ) ) {
                    continue;
                }

                if ( nPrice <= 0 ) {
                    START_LOG( clog, L"첨부한 아이템의 가격이 없거나 0 이다. ItemID : " << vit->first.m_ItemID << L", Price : " << nPrice ) << END_LOG;
                    continue;
                }

                // 화폐별 수수료 계산
                // ToDo. 수수료 계산식 적용 필요
                switch ( nMoneyType )
                {
                case KItemManager::EMT_CASH:
                    // 캐쉬아이템 수수료 = (되팔기 잼 개수 * 잼 가격) * 아이템 수수료 비율
                    break;
                case KItemManager::EMT_GAMEPOINT:
                    // GP아이템 수수료 = 아이템 가격 * 아이템 수수료 비율
                    break;

                case KItemManager::EMT_VIRTUALCASH:
                case KItemManager::EMT_CRYSTAL:
                case KItemManager::EMT_GCPOINT:
                default:
                    START_LOG( clog, L"수수료 정의되지 않은 화폐 타입 : " << nMoneyType ) << END_LOG;
                    break;
                }
            }
            // 수량아이템 수수료
            kPacket.m_nSendLetterCost += nCountItemCost;
        }

        // 필요한 GP = 첨부GP + 수수료GP
        int nNeedGP = kPacket_.m_nPostGP + kPacket.m_nSendLetterCost;
        if ( kPacket_.m_nGamePoint < nNeedGP ) {
            START_LOG( cerr, L"우편 수수료 GP가 부족함.. LoginID : " << strLogin_ )
                << BUILD_LOG( kPacket_.m_nPostGP )
                << BUILD_LOG( kPacket.m_nSendLetterCost )
                << BUILD_LOG( kPacket_.m_nGamePoint )
                << BUILD_LOG( nItemCost )
                << BUILD_LOG( nCountItemCost )
                << BUILD_LOG( nGPCost )
                << BUILD_LOG( nNeedGP ) << END_LOG;
            SET_ERR_GOTO( ERR_POSTOFFICE_18, END_PROC );
        }
    }

    // 편지보내기
    {
        int nOK = m_kHelper.Post_insert( dwToUserUID, kPacket_.m_cCharType, KPostItemInfo::LT_USER, dwUID_, kPacket_.m_strTitle, kPacket_.m_strMessage,
                                         kPacket.m_PostUID, kPacket_.m_nPostGP, SiKPostConfig()->GetUserPostStorageTime() );
        if ( nOK != 0 ) {
            START_LOG( cerr, L"우편 보내기 DB처리 실패.. LoginID : " << strLogin_ )
                << BUILD_LOG( dwToUserUID )
                << BUILD_LOGc( kPacket_.m_cCharType )
                << BUILD_LOG( nOK )
                << BUILD_LOG( kPacket_.m_strTitle )
                << BUILD_LOG( kPacket_.m_strMessage )
                << BUILD_LOG( kPacket.m_PostUID )
                << BUILD_LOG( kPacket_.m_nPostGP ) << END_LOG;
            SET_ERR_GOTO( ERR_POSTOFFICE_19, END_PROC );
        }
    }

    // GP차감 처리
    {
        // 첨부GP차감
        if ( kPacket_.m_nPostGP > 0 ) {
            int nOK = -1;
            int nInitGP = 0;
            int nDecreaseGP = 0;
            nDecreaseGP -= kPacket_.m_nPostGP;
            nDecreaseGP = std::min<int>( nDecreaseGP, 0 );

            UpdateUserGamePoint( dwUID_, cCharType_, kPacket_.m_dwSlotNum, nDecreaseGP, nOK, nInitGP );
            if ( nOK == 0 ) {
                kPacket.m_nGamePoint -= nDecreaseGP;
            }

            _LOG_SUCCESS( nOK == 0, L"첨부GP차감 DB 데이터 갱신 실패.. Login : " << strLogin_ )
                << BUILD_LOG( nOK )
                << BUILD_LOG( nDecreaseGP )
                << BUILD_LOG( nInitGP )
                << BUILD_LOG( kPacket.m_nGamePoint ) << END_LOG;
        }

        // 수수료 차감
        if ( kPacket.m_nSendLetterCost > 0 ) {
            int nOK = -1;
            int nInitGP = 0;
            int nDecreaseGP = 0;
            nDecreaseGP -= kPacket.m_nSendLetterCost;
            nDecreaseGP = std::min<int>( nDecreaseGP, 0 );

            UpdateUserGamePoint( dwUID_, cCharType_, kPacket_.m_dwSlotNum, nDecreaseGP, nOK, nInitGP );
            if ( nOK == 0 ) {
                kPacket.m_nGamePoint -= nDecreaseGP;
            }

            _LOG_SUCCESS( nOK == 0, L"수수료GP차감 DB 데이터 갱신 실패.. Login : " << strLogin_ )
                << BUILD_LOG( nOK )
                << BUILD_LOG( nDecreaseGP )
                << BUILD_LOG( nInitGP )
                << BUILD_LOG( kPacket.m_nGamePoint ) << END_LOG;
        }
    }


    // 영구아이템 처리
    if ( false == kPacket_.m_vecItem.empty() ) {
        std::vector<KItem>::iterator vit;
        for ( vit = kPacket_.m_vecItem.begin() ; vit != kPacket_.m_vecItem.end() ; ++vit ) {
            // 우편 아이템 첨부
            int nOK = m_kHelper.PostItem_insert( dwToUserUID, kPacket_.m_cCharType, kPacket.m_PostUID, vit->m_ItemUID, vit->m_nCount );
            if ( nOK != 0 ) {
                START_LOG( cerr, L"영구아이템 우편 첨부 DB처리 실패.. Login : " << strLogin_ << L", 결과 : " << nOK ) << END_LOG;

                // ToDo. 언제 호출되어야하나.
                // 실패시 아이템 소유자 롤백
                // m_kHelper.WaitItem_Chang_update()
                continue;
            }

            kPacket.m_vecEraseItem.push_back( vit->m_ItemUID );
        }
    }

    // 수량 아이템 처리
    if ( false == kPacket_.m_vecCountItem.empty() ) {

        std::vector<std::pair<KItem,int> >::iterator vit;
        for ( vit = kPacket_.m_vecCountItem.begin() ; vit != kPacket_.m_vecCountItem.end() ; ++vit ) {
            KItem& kPostItem = vit->first;
            const int& nCount = vit->second;

            // 수량 체크
            if ( nCount <= 0 ) {
                START_LOG( cerr, L"첨부할 수량아이템 수량이 0 이하이다. Name : " << strLogin_ )
                    << BUILD_LOG( kPostItem.m_ItemUID )
                    << BUILD_LOG( kPostItem.m_ItemID )
                    << BUILD_LOG( kPostItem.m_nCount )
                    << BUILD_LOG( nCount ) << END_LOG;
                continue;
            }

            // 동기화전 InitCount, Count값 변동값 있는지 체크.
            if( !CheckItemDuration( dwUID_, kPostItem.m_ItemUID, kPostItem.m_nInitCount, kPostItem.m_nCount ) ) {
                START_LOG( clog, L"첨부할 수량아이템 창고 수량 동기화. Name : " << strLogin_ ) 
                    << BUILD_LOG( dwUID_ )
                    << BUILD_LOG( kPostItem.m_ItemUID )
                    << BUILD_LOG( kPostItem.m_nInitCount )
                    << BUILD_LOG( kPostItem.m_nCount )
                    << END_LOG;
            }

            // 수량 차감
            if ( false == UseCountItem( dwUID_, kPostItem.m_ItemUID, nCount ) ) { // 사용 갯수 감소.
                START_LOG( cerr, L"첨부할 수량아이템 수량차감 DB처리 실패. Name : " << strLogin_ )
                    << BUILD_LOG( kPostItem.m_ItemUID )
                    << BUILD_LOG( kPostItem.m_ItemID )
                    << BUILD_LOG( kPostItem.m_nCount )
                    << BUILD_LOG( nCount ) << END_LOG;
                continue;
            }

            // 우편 대기아이템 등록
            {
                int nOK = m_kHelper.WaitItem_insert_20111028( dwToUserUID, kPacket_.m_cCharType, kPostItem.m_ItemID, kPostItem.m_cGradeID, kPostItem.m_nPeriod, nCount, 
                    KItemManager::EWIT_POST_USER, kPostItem.m_ItemUID, (int)kPostItem.m_sEquipLevel, kPostItem.m_cEnchantLevel );

                if ( nOK != 0 ) {
                    // 실패시
                    if ( false == AddCountItem( dwUID_, kPostItem.m_ItemUID, nCount ) ) { // 사용 갯수 롤백
                        START_LOG( cerr, L"첨부실패한 수량아이템 수량 롤백 DB처리 실패. Name : " << strLogin_ )
                            << BUILD_LOG( kPostItem.m_ItemUID )
                            << BUILD_LOG( kPostItem.m_ItemID )
                            << BUILD_LOG( kPostItem.m_nCount )
                            << END_LOG;
                        continue;
                    }
                }
            }

            // 수량 동기화
            kPostItem.m_nCount -= nCount;
            kPostItem.m_nCount = std::max<int>( kPostItem.m_nCount, 0 );
            kPostItem.m_nInitCount = kPostItem.m_nCount;
            kPacket.m_vecDecreaseCountItem.push_back( kPostItem );

            // 우편 아이템 첨부
            {
                int nOK = m_kHelper.PostItem_insert( dwToUserUID, kPacket_.m_cCharType, kPacket.m_PostUID, kPostItem.m_ItemUID, nCount );
                if ( nOK != 0 ) {
                    START_LOG( cerr, L"수량아이템 우편 첨부 DB처리 실패.. Login : " << strLogin_ << L", 결과 : " << nOK ) << END_LOG;
                    continue;
                }
            }
        }
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nPostGP )
        << BUILD_LOG( kPacket.m_nGamePoint )
        << BUILD_LOG( kPacket.m_vecEraseItem.size() )
        << BUILD_LOG( kPacket.m_vecDecreaseCountItem.size() )
        << BUILD_LOG( kPacket.m_nSendLetterCost ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_SEND_LETTER_ACK );

    if ( kPacket.m_nOK == NetError::NET_OK ) {
        _QUEUING_ACK_TO_USER( EVENT_FIND_USER_SEND_LETTER_NOT, dwToUserUID );
        START_LOG( clog, L"우편보내기 성공시, 대상유저에게 실시간 알림 요청.. FromUse Login : " << strLogin_ << L", ToUser UserUID : " << dwToUserUID ) << END_LOG;
    }
}

_INT_IMPL_ON_FUNC( EVENT_READ_LETTER_NOT, POSTUID )
{
    EVENT_CHECK_OUT( EVENT_READ_LETTER_NOT );

    int nRet = m_kHelper.Post_State_update( dwUID_, -1, kPacket_, KPostItemInfo::LS_READ );

    _LOG_SUCCESS( nRet == 0, L"우편읽기 DB 상태갱신 결과 : " << nRet )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_ ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_GET_ITEM_FROM_LETTER_REQ )
{
    EVENT_CHECK_OUT( EVENT_GET_ITEM_FROM_LETTER_REQ );
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_GET_ITEM_FROM_LETTER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_PostUID = kPacket_.m_PostUID;
    kPacket.m_bSystemLetter = kPacket_.m_bSystemLetter;
    kPacket.m_vecReward.clear();
    kPacket.m_nRewardGP = kPacket_.m_nRewardGP;
    kPacket.m_nCurrentGP = 0;

    int nGetType = ( kPacket.m_bSystemLetter ? KItemManager::EWIT_POST_SYSTEM : KItemManager::EWIT_POST_USER );

    std::map<GCITEMUID,KItem> mapWaitItem;
    mapWaitItem.clear();

    // 우편 대기아이템 리스트 가져오기 (시스템우편이거나 수량아이템)
    if ( false == m_kHelper.WaitItem_post_select( dwUID_, -1, mapWaitItem ) ) {
        SET_ERR_GOTO( ERR_POSTOFFICE_28, END_PROC );
    }

    // 시스템 우편일때,
    if ( kPacket.m_bSystemLetter ) {

        std::map<GCITEMUID,KItem>::iterator mitWaitUID;
        for ( mitWaitUID = mapWaitItem.begin() ; mitWaitUID != mapWaitItem.end() ; ++mitWaitUID ) {

            std::vector<KItem>::iterator vit;
            vit = std::find_if( kPacket_.m_vecReward.begin(), kPacket_.m_vecReward.end(), boost::bind(&KItem::m_ItemUID, boost::placeholders::_1) == mitWaitUID->first );
            if ( vit == kPacket_.m_vecReward.end() ) {
                START_LOG( clog, L"요청한 우편의 보상아이템 대기아이템(영구) ItemUID가 아님.. Login : " << strLogin_ << L", ItemUID : " << mitWaitUID->first << L", ItemID : " << mitWaitUID->second.m_ItemID ) << END_LOG;
                continue;
            }

            // 우편첨부 대기아이템 스테이터스 값 변경
            {
                char cBeforeStatus = 12;
                char cAfterStatus = 2;
                int nOK = m_kHelper.WaitItem_Status_update( dwUID_, -1, vit->m_ItemUID, cBeforeStatus, cAfterStatus );
                if ( nOK != 0 ) {
                    // 에러기록만 남기고, 지급은 계속 진행.
                    START_LOG( cerr, L"우편첨부 대기아이템 스테이터스값 변경 실패.. Login : " << strLogin_ )
                        << BUILD_LOG( dwUID_ )
                        << BUILD_LOG( nOK )
                        << BUILD_LOG( vit->m_ItemUID )
                        << BUILD_LOGc( cBeforeStatus )
                        << BUILD_LOGc( cAfterStatus ) << END_LOG;
                }
            }

            // 인벤토리 지급
            {
                bool bOK = m_kHelper.InsertSingleWaitItemToInvenOrVDepot( dwUID_, cCharType_, vit->m_ItemUID, nGetType, kPacket.m_vecReward );
                if ( false == bOK ) {
                    // 에러기록만 남기고, 지급은 계속 진행.
                    START_LOG( cerr, L"아이템 지급 실패.. Login : " << strLogin_ )
                        << BUILD_LOG( dwUID_ )
                        << BUILD_LOG( vit->m_ItemUID )
                        << BUILD_LOGc( nGetType )
                        << BUILD_LOGc( kPacket.m_vecReward.size() ) << END_LOG;
                }
            }

            // 우편 첨부아이템 받기 완료
            {
                int nOK = m_kHelper.PostItem_delete( dwUID_, -1, kPacket.m_PostUID, vit->m_ItemUID );
                if ( nOK != 0 ) {
                    // 에러기록만 남기고, 지급은 계속 진행.
                    START_LOG( cerr, L"첨부아이템 받기 완료기록 실패.. Login : " << strLogin_ )
                        << BUILD_LOG( dwUID_ )
                        << BUILD_LOG( nOK )
                        << BUILD_LOG( kPacket.m_PostUID )
                        << BUILD_LOG( vit->m_ItemUID ) << END_LOG;
                }
            }

            START_LOG( clog, L"시스템 우편 첨부아이템 받음. Login : " << strLogin_ << L", ItemID : " << vit->m_ItemID << L", ItemUID : " << vit->m_ItemUID ) << END_LOG;
        }
    }
    else {
        // 유저우편인 경우.
        std::vector<KItem>::iterator vit;
        for ( vit = kPacket_.m_vecReward.begin() ; vit != kPacket_.m_vecReward.end() ; ++vit ) {

            // 영구아이템일때,
            if ( vit->m_nCount == -1 ) {
                // 아이템 사용자 변경
                int nOK = SetTypeEraseItem( strLogin_, dwUID_, vit->m_ItemUID, KItemManager::EDEL_DEL_POST_ITEM, cCharType_ );
                if ( nOK != 0 ) {
                    // 에러기록만 남기고, 지급은 계속 진행.
                    START_LOG( cerr, L"유저우편 영구아이템 사용자 변경 실패.. Login : " << strLogin_ )
                        << BUILD_LOG( dwUID_ )
                        << BUILD_LOG( nOK )
                        << BUILD_LOG( vit->m_ItemUID ) << END_LOG;
                }
            }
            else {
                // 수량아이템일때,
                std::map<GCITEMUID,KItem>::iterator mitWaitUID;
                mitWaitUID = mapWaitItem.find( vit->m_ItemUID );
                if ( mitWaitUID == mapWaitItem.end() ) {
                    START_LOG( cerr, L"요청한 우편의 보상아이템 대기아이템(수량) ItemUID를 찾을 수 없음.. Login : " << strLogin_ << L", ItemUID : " << mitWaitUID->first << L", ItemID : " << mitWaitUID->second.m_ItemID ) << END_LOG;
                    continue;
                }

                // 우편첨부 대기아이템 스테이터스 값 변경
                {
                    char cBeforeStatus = 12;
                    char cAfterStatus = 2;
                    int nOK = m_kHelper.WaitItem_Status_update( dwUID_, -1, vit->m_ItemUID, cBeforeStatus, cAfterStatus );
                    if ( nOK != 0 ) {
                        // 에러기록만 남기고, 지급은 계속 진행.
                        START_LOG( cerr, L"우편첨부 대기아이템 스테이터스값 변경 실패.. Login : " << strLogin_ )
                            << BUILD_LOG( dwUID_ )
                            << BUILD_LOG( nOK )
                            << BUILD_LOG( vit->m_ItemUID )
                            << BUILD_LOGc( cBeforeStatus )
                            << BUILD_LOGc( cAfterStatus ) << END_LOG;
                    }
                }

                // 인벤토리 지급
                {
                    bool bOK = m_kHelper.InsertSingleWaitItemToInvenOrVDepot( dwUID_, cCharType_, vit->m_ItemUID, nGetType, kPacket.m_vecReward );
                    if ( false == bOK ) {
                        // 에러기록만 남기고, 지급은 계속 진행.
                        START_LOG( cerr, L"아이템 지급 실패.. Login : " << strLogin_ )
                            << BUILD_LOG( dwUID_ )
                            << BUILD_LOG( vit->m_ItemUID )
                            << BUILD_LOG( nGetType )
                            << BUILD_LOG( kPacket.m_vecReward.size() ) << END_LOG;
                    }
                }
            }

            // 우편 첨부아이템 받기 완료
            {
                int nOK = m_kHelper.PostItem_delete( dwUID_, -1, kPacket.m_PostUID, vit->m_ItemUID );
                if ( nOK != 0 ) {
                    // 에러기록만 남기고, 지급은 계속 진행.
                    START_LOG( cerr, L"첨부아이템 받기 완료기록 실패.. Login : " << strLogin_ )
                        << BUILD_LOG( dwUID_ )
                        << BUILD_LOG( nOK )
                        << BUILD_LOG( kPacket.m_PostUID )
                        << BUILD_LOG( vit->m_ItemUID ) << END_LOG;
                }
            }

            START_LOG( clog, L"유저 우편 첨부아이템 받음. Login : " << strLogin_ << L", ItemID : " << vit->m_ItemID << L", ItemUID : " << vit->m_ItemUID ) << END_LOG;
        }
    }

    // 우편상태 변경
    {
        int nOK = m_kHelper.Post_State_update(dwUID_, -1, kPacket.m_PostUID, KPostItemInfo::LS_READ_ITEM );
        if ( nOK != 0 ) {
            // 에러기록만 남기고, 지급은 계속 진행.
            START_LOG( cerr, L"우편상태 변경 DB처리 실패.. Login : " << strLogin_ )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( nOK )
                << BUILD_LOG( kPacket.m_PostUID ) << END_LOG;
        }
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_PostUID )
        << BUILD_LOG( kPacket.m_bSystemLetter )
        << BUILD_LOG( kPacket.m_vecReward.size() )
        << BUILD_LOG( kPacket.m_nRewardGP )
        << BUILD_LOG( kPacket.m_nCurrentGP )
        << BUILD_LOG( nGetType )
        << BUILD_LOG( mapWaitItem.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_GET_ITEM_FROM_LETTER_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_DELETE_LETTER_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_DELETE_LETTER_REQ );

    SET_ERROR( ERR_UNKNOWN );

    KEVENT_DELETE_LETTER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_vecPostUID = kPacket_.m_vecPostUID;

    std::map< POSTUID, std::vector<KItem> >::iterator mit;
    for( mit = kPacket_.m_mapPostItemInfo.begin(); mit != kPacket_.m_mapPostItemInfo.end(); ++mit ) {
        if ( false == m_kHelper.Post_delete( dwUID_, -1, mit->first, KPostItemInfo::DT_USER_DELETE ) ) {
            START_LOG( cerr, L"우편삭제 DB처리 실패.. Login : " << strLogin_ << L", PostUID : " << mit->first ) << END_LOG;
            SET_ERR_GOTO( ERR_POSTOFFICE_31, END_PROC );
        }

        // 첨부된 우편 아이템 삭제 로직 추가 필요.
        std::vector<KItem>::iterator vit;
        for( vit = mit->second.begin(); vit != mit->second.end(); ++vit ) {
            int nOK = m_kHelper.PostItem_delete( dwUID_, -1, mit->first, vit->m_ItemUID );
            if ( nOK != 0 ) {
                // 에러기록만 남기고, 지급은 계속 진행.
                START_LOG( cerr, L"첨부아이템 받기 완료기록 실패.. Login : " << strLogin_ )
                    << BUILD_LOG( dwUID_ )
                    << BUILD_LOGc( cCharType_ )
                    << BUILD_LOG( nOK )
                    << BUILD_LOG( mit->first )
                    << BUILD_LOG( vit->m_ItemUID ) << END_LOG;
            }
        }
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_vecPostUID.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_DELETE_LETTER_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_NEW_POST_LETTER_INFO_REQ )
{
    EVENT_CHECK_OUT( EVENT_NEW_POST_LETTER_INFO_REQ );

    std::vector<KPostItemInfo> vecPostItem;
    vecPostItem.clear();

    if ( false == GetNewPostLetterInfo( strLogin_, dwUID_, cCharType_, vecPostItem ) ) 
    {
        return;
    }

    START_LOG( clog, L"신규 우편 전달.. LoginID : " << strLogin_ )
        << BUILD_LOG( vecPostItem.size() ) << END_LOG;

    _QUEUING_ACK_TO_USER( EVENT_NEW_POST_LETTER_INFO_ACK, vecPostItem );
}

INT_IMPL_ON_FUNC( EVENT_DUNGEON_CLEAR_REWARD_REQ )
{
    EVENT_CHECK_OUT( EVENT_DUNGEON_CLEAR_REWARD_REQ );
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_DUNGEON_CLEAR_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nModeID = kPacket_.m_nModeID;
    kPacket.m_vecIndexItemID = kPacket_.m_vecIndexItemID;
    kPacket.m_vecReward.clear();
    kPacket.m_usLeftRewardCount = 0;
    int nOK = 0;

    // 아이템 지급
    if ( false == kPacket_.m_vecRewardItem.empty() ) {
        std::vector<KManufactureItem>::iterator vit;
        for ( vit = kPacket_.m_vecRewardItem.begin() ; vit != kPacket_.m_vecRewardItem.end() ; ++vit ) {
            KManufactureItem& kCatalog = *vit;
            bool bLookItem = kCatalog.CheckItemFlag( KManufactureItem::IF_LOOK );
            bool bAgitObj = kCatalog.CheckItemFlag( KManufactureItem::IF_AGIT_OBJ );

            if ( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, kCatalog.m_ItemID, kCatalog.m_nDuration, kCatalog.m_nPeriod, KItemManager::EWIT_DUNGEON_CLEAR_REWARD, kPacket.m_vecReward ) ) {
                START_LOG( cerr, L"던전 클리어보상 아이템 삽입 실패 Name : " << strLogin_ )
                    << BUILD_LOG( kCatalog.m_ItemID )
                    << BUILD_LOG( kCatalog.m_nDuration )
                    << BUILD_LOG( kCatalog.m_nPeriod ) << END_LOG;
                continue;
            }

            // 아이템 설정
            std::vector< KItem >::iterator vitReward;
            for ( vitReward = kPacket.m_vecReward.begin() ; vitReward != kPacket.m_vecReward.end() ; ++vitReward ) {
                // 룩 아이템 변환
                if ( bLookItem ) {
                    if( vitReward->m_cItemType == KItem::TYPE_LOOK ) {
                        continue;
                    }

                    if ( false == m_kHelper.NormalItemToCoordiItem( dwUID_, vitReward->m_ItemUID, cCharType_, nOK ) ) {
                        START_LOG( cerr, L"DB작업중 룩 아이템 변환 실패. OK : " << nOK << L"ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
                    }
                    else {
                        vitReward->AddItemType( KItem::TYPE_LOOK );
                        START_LOG( clog, L"룩 아이템으로 변환됨. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
                    }
                }

                // 아지트 오브젝트 설정
                if ( bAgitObj ) {
                    if ( false == m_kHelper.AgiteObjPos_merge( dwUID_, vitReward->m_ItemUID, -1, -1 ) ) {
                        START_LOG( cerr, L"DB작업중 아지트 아이템 설정 실패. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
                    }
                    else {
                        vitReward->AddItemType( KItem::TYPE_AGIT_OBJ );
                        START_LOG( clog, L"아지트 아이템으로 설정. ItemID : " << vitReward->m_ItemID << L",ItemUID : " << vitReward->m_ItemUID ) << END_LOG;
                    }
                }
            }

            START_LOG( clog, L"던전클리어보상 아이템지급. Login : " << strLogin_ )
                << BUILD_LOG( kCatalog.m_ItemID )
                << BUILD_LOG( kCatalog.m_nDuration )
                << BUILD_LOG( kCatalog.m_nPeriod )
                << BUILD_LOG( bLookItem )
                << BUILD_LOG( bAgitObj ) << END_LOG;
        }
    }

    if ( false == kPacket_.m_vecRewardItem.empty() && kPacket.m_vecReward.empty() ) {
        SET_ERR_GOTO( ERR_DUNGEON_CLEAR_REWARD_06, END_PROC );
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nModeID )
        << BUILD_LOG( kPacket.m_vecIndexItemID.size() )
        << BUILD_LOG( kPacket.m_vecReward.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_DUNGEON_CLEAR_REWARD_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_UPDATE_DUNGEON_CLEAR_INFO_NOT )
{
    bool bRet = m_kHelper.UserGameClear2_merge_20111213( dwUID_, cCharType_, kPacket_.m_nModeID, kPacket_.m_kClearData );

    _LOG_SUCCESS( bRet, L"던전 클리어시 실시간으로 클리어정보 DB기록.. Login : " << strLogin_ )
        << BUILD_LOG( kPacket_.m_nModeID )
        << BUILD_LOG( kPacket_.m_kClearData.m_bClear )
        << BUILD_LOG( kPacket_.m_kClearData.m_shLastDifficulty )
        << BUILD_LOG( kPacket_.m_kClearData.m_shMaxDifficulty )
        << BUILD_LOG( kPacket_.m_kClearData.m_usLeftRewardCount ) << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_EQUIP_LEVEL_DOWN_REQ, KDB_EVENT_EQUIP_LEVEL_DOWN_REQ )
{
    EVENT_CHECK_OUT( EVENT_EQUIP_LEVEL_DOWN_REQ );
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_EQUIP_LEVEL_DOWN_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_EquipItem = kPacket_.m_EquipItem;
    kPacket.m_MaterialItem = kPacket_.m_MaterialItem;

    // 장착레벨 낮추기 DB기록
    if ( false == m_kHelper.UpdateLevel( dwUID_, kPacket_.m_nEquipLvDown, KItemLevelTable::IL_EQUIP_DOWN_LV, kPacket.m_EquipItem, cCharType_ ) ) {
        SET_ERR_GOTO( ERR_EQUIP_LEVEL_DOWN_07, END_PROC );
    }

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket.m_MaterialItem.m_ItemUID, kPacket.m_MaterialItem.m_nInitCount, kPacket.m_MaterialItem.m_nCount ) ) {
        START_LOG( clog, L"장착레벨 낮추기 재료아이템 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket.m_MaterialItem.m_ItemUID )
            << BUILD_LOG( kPacket.m_MaterialItem.m_nInitCount )
            << BUILD_LOG( kPacket.m_MaterialItem.m_nCount )
            << END_LOG;
    }

    // 사용한 재료아이템 감소
    if ( false == UseCountItem( dwUID_, kPacket.m_MaterialItem.m_ItemUID, kPacket_.m_nMaterialCount ) ) {
        kPacket.m_MaterialItem.m_nCount = 0;
        START_LOG( cerr, L"장착레벨 낮추기 재료아이템 수량 동기화 실패. Name : " << strLogin_ ) << END_LOG;
    }
    else {
        // 수량 감소
        kPacket.m_MaterialItem.m_nCount -= kPacket_.m_nMaterialCount;
        kPacket.m_MaterialItem.m_nCount = std::max<int>( kPacket.m_MaterialItem.m_nCount, 0 );
        kPacket.m_MaterialItem.m_nInitCount = kPacket.m_MaterialItem.m_nCount;
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_nMaterialCount )
        << BUILD_LOG( kPacket_.m_nEquipLvDown )
        << BUILD_LOG( kPacket.m_EquipItem.m_ItemID )
        << BUILD_LOG( kPacket.m_EquipItem.m_ItemUID )
        << BUILD_LOG( kPacket.m_EquipItem.m_sEquipLevel )
        << BUILD_LOG( kPacket.m_EquipItem.m_sEquipLvDown )
        << BUILD_LOG( kPacket_.m_EquipItem.m_sEquipLvDown )
        << BUILD_LOG( kPacket.m_MaterialItem.m_ItemID )
        << BUILD_LOG( kPacket.m_MaterialItem.m_ItemUID )
        << BUILD_LOG( kPacket.m_MaterialItem.m_nCount ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_EQUIP_LEVEL_DOWN_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_ECLIPSE_PLOT_TIME_REWARD_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_ECLIPSE_PLOT_TIME_REWARD_REQ );
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_ECLIPSE_PLOT_TIME_REWARD_ACK kPacket;

    std::vector<KDropItemInfo>::iterator vitReward;
    for ( vitReward = kPacket_.begin(); vitReward != kPacket_.end(); ++vitReward ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_,
            cCharType_,
            vitReward->m_ItemID,
            vitReward->m_nDuration,
            vitReward->m_nPeriod,
            KItemManager::EWIT_ECLIPSE_PLOT_REWARD,
            kPacket.m_vecReward ) ) {

            START_LOG( cerr, L"개기일식 음모 보상 아이템 지급 실패 UID : " << dwUID_ )
                << BUILD_LOG( vitReward->m_ItemID )
                << BUILD_LOG( vitReward->m_nDuration )
                << BUILD_LOG( vitReward->m_nPeriod ) << END_LOG;

            continue;
        }
    }

    time_t tmBegin;
    SiKEclipsePlot()->GetBeginTime( tmBegin );
    if ( SetTodayEventDoneDiff( dwUID_, UET_ECLIPSE_PLOT_PROGRESS, tmBegin, 1 ) == false ) {
        SET_ERR_GOTO( ERR_ECLIPSE_PLOT_08, END_PROC );
    }

    SET_ERROR( NET_OK );

END_PROC:

    kPacket.m_nOK = NetError::GetLastNetError();

    QUEUING_ACK_TO_USER_UID( EVENT_ECLIPSE_PLOT_TIME_REWARD_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_ECLIPSE_PLOT_INFO_REQ )
{
    EVENT_CHECK_OUT( EVENT_ECLIPSE_PLOT_INFO_REQ );

    time_t tmBegin;
    SiKEclipsePlot()->GetBeginTime( tmBegin );

    KEclipsePlotData kPacket;
    kPacket.m_nProgressFinalReward = GetTodayEventCount( dwUID_, UET_ECLIPSE_PLOT_PROGRESS, tmBegin );
    kPacket.m_bGotFinalReward = ( GetTodayEventCount( dwUID_, UET_ECLIPSE_PLOT_FINAL_REWARD, tmBegin ) == 1 ? true : false );

    START_LOG( clog, L"개기일식 최종보상 진행도 : " << dwUID_ )
        << BUILD_LOG( kPacket.m_bGotFinalReward )
        << BUILD_LOG( kPacket.m_nProgressFinalReward )
        << END_LOG;

    QUEUING_ACK_TO_USER_UID( EVENT_ECLIPSE_PLOT_INFO_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_ECLIPSE_PLOT_HUNT_REWARD_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_ECLIPSE_PLOT_HUNT_REWARD_REQ );
    SET_ERROR( ERR_UNKNOWN );
    KEVENT_ECLIPSE_PLOT_HUNT_REWARD_ACK kPacket;
    std::vector<KDropItemInfo>::iterator vitReward;

    for ( vitReward = kPacket_.m_vecHuntRewardItem.begin(); vitReward != kPacket_.m_vecHuntRewardItem.end(); ++vitReward ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_,
            cCharType_,
            vitReward->m_ItemID,
            vitReward->m_nDuration,
            vitReward->m_nPeriod,
            KItemManager::EWIT_ECLIPSE_PLOT_REWARD,
            kPacket.m_vecReward ) ) {

            START_LOG( cerr, L"개기일식 미니게임 수집 보상 아이템 지급 실패 UID : " << dwUID_ )
                << BUILD_LOG( vitReward->m_ItemID )
                << BUILD_LOG( vitReward->m_nDuration )
                << BUILD_LOG( vitReward->m_nPeriod ) << END_LOG;

            continue;
        }
    }

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_kHuntItem.m_ItemUID, kPacket_.m_kHuntItem.m_nInitCount, kPacket_.m_kHuntItem.m_nCount ) ) {
        START_LOG( clog, L"개기일식 수집 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_kHuntItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_kHuntItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_kHuntItem.m_nCount )
            << END_LOG;
    }

    if ( !UseCountItem( dwUID_, kPacket_.m_kHuntItem.m_ItemUID, kPacket_.m_nUseHuntItem ) ) {
        START_LOG( cerr, L"개기일식 수집 아이템 개수 동기화 실패. Name : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_kHuntItem.m_ItemUID )
            << END_LOG;
        SET_ERR_GOTO( ERR_ECLIPSE_PLOT_04, END_PROC );
    }

    time_t tmBegin;
    SiKEclipsePlot()->GetBeginTime( tmBegin );
    if ( SetTodayEventDoneDiff( dwUID_, UET_ECLIPSE_PLOT_PROGRESS, tmBegin, 1 ) == false ) {
        SET_ERR_GOTO( ERR_ECLIPSE_PLOT_08, END_PROC );
    }

    kPacket_.m_kHuntItem.m_nCount -= kPacket_.m_nUseHuntItem;
    kPacket_.m_kHuntItem.m_nCount = std::max<int>(0, kPacket_.m_kHuntItem.m_nCount );
    kPacket.m_kHuntItem = kPacket_.m_kHuntItem;


    // 에러 처리
END_PROC:
    if ( IS_CORRECT( ERR_ECLIPSE_PLOT_04 ) ) {
        kPacket.m_kHuntItem.m_nCount = 0;
    }
    else {
        SET_ERROR( NET_OK );
    }

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER_UID( EVENT_ECLIPSE_PLOT_HUNT_REWARD_ACK );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_ECLIPSE_PLOT_FINAL_REWARD_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_ECLIPSE_PLOT_FINAL_REWARD_REQ );
    SET_ERROR( ERR_UNKNOWN );
    KEVENT_ECLIPSE_PLOT_FINAL_REWARD_ACK kPacket;
    std::vector<KDropItemInfo>::iterator vitReward;
    for ( vitReward = kPacket_.begin(); vitReward != kPacket_.end(); ++vitReward ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_,
            cCharType_,
            vitReward->m_ItemID,
            vitReward->m_nDuration,
            vitReward->m_nPeriod,
            KItemManager::EWIT_ECLIPSE_PLOT_REWARD,
            kPacket.m_vecReward ) ) {

            START_LOG( cerr, L"개기일식 미니게임 수집 보상 아이템 지급 실패 UID : " << dwUID_ )
                << BUILD_LOG( vitReward->m_ItemID )
                << BUILD_LOG( vitReward->m_nDuration )
                << BUILD_LOG( vitReward->m_nPeriod ) << END_LOG;

            continue;
        }
    }

    time_t tmBegin;
    SiKEclipsePlot()->GetBeginTime( tmBegin );

    if ( SetTodayEventDone( dwUID_, UET_ECLIPSE_PLOT_FINAL_REWARD, tmBegin, 1 ) == false ) {
        SET_ERR_GOTO( ERR_ECLIPSE_PLOT_07, END_PROC );
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER_UID( EVENT_ECLIPSE_PLOT_FINAL_REWARD_ACK );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_vecReward.size() )
        << END_LOG;

}

INT_IMPL_ON_FUNC( DB_EVENT_ECLIPSE_COLLECT_REWARD_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_ECLIPSE_COLLECT_REWARD_REQ );

    KEVENT_ECLIPSE_COLLECT_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );
    std::vector<KDropItemInfo>::iterator vit;

    // 해킹 이슈로 인해서 정보 DB에서 일단 한번 체크.
    // 이벤트 기간내 한번만 지급 보상 아이템 받았는지 체크.
    time_t tmDate;
    SiKEclipseCollect()->GetBeginTime( tmDate );
    int nData = GetTodayEventCount( dwUID_, UET_ECLIPSE_COLLECT_EVENT , tmDate );
    if( 0 == nData ) {
        // 고서 모으기 보상 아이템 지급.
        for ( vit = kPacket_.m_vecRewardItem.begin(); vit != kPacket_.m_vecRewardItem.end(); ++vit ) {
            if ( false == m_kHelper.InsertSingleItemJob( dwUID_, 
                cCharType_,
                vit->m_ItemID, 
                vit->m_nDuration, 
                vit->m_nPeriod, 
                KItemManager::EWIT_ECLIPSE_COLLECT_REWARD, 
                kPacket.m_vecItem ) ) {
                    START_LOG( cerr, L"고서 모으기 이벤트 보상 아이템 지급 실패 UID : " << dwUID_ )
                        << BUILD_LOG( vit->m_ItemID )
                        << BUILD_LOG( vit->m_nDuration )
                        << BUILD_LOG( vit->m_nPeriod )
                        << END_LOG;

                    SET_ERR_GOTO( ERR_ECLIPSE_COLLECT_04, END_PROC );
            }
        }
    } else {
        START_LOG( cerr, L"고서 모으기 이벤트 이미 아이템 지급받은 유저 UID : " << dwUID_ )
            << END_LOG;
        SET_ERR_GOTO( ERR_ECLIPSE_COLLECT_04, END_PROC );
    }

    LIF( SetTodayEventDoneDiff( dwUID_, UET_ECLIPSE_COLLECT_EVENT, tmDate, 1 ) );

   kPacket.m_setItemList = kPacket_.m_setItemList;
   SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_ECLIPSE_COLLECT_REWARD_ACK );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_vecRewardItem.size() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecItem.size() )
        << BUILD_LOG( kPacket.m_setItemList.size() ) << END_LOG;
}

_INT_IMPL_ON_FUNC( DB_EVENT_ECLIPSE_COLLECT_REWARD_CHECK_REQ, time_t )
{
    bool bEclipseCollectComplete = false;
    // 이벤트 기간내 한번만 지급 보상 아이템 받았는지 체크.
    int nData = GetTodayEventCount( dwUID_, UET_ECLIPSE_COLLECT_EVENT , kPacket_ );
    if( 0 != nData ) {
        bEclipseCollectComplete = true;
    }

    _QUEUING_ACK_TO_USER( EVENT_ECLIPSE_COLLECT_REWARD_ANSWER_NOT, bEclipseCollectComplete );

    START_LOG( clog, L"유저의 고서 모으기 이벤트 최종 보상 획득 여부 확인 : " << bEclipseCollectComplete )
        << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_MONSTER_CARD_MIX_REQ )
{
    EVENT_CHECK_OUT( EVENT_MONSTER_CARD_MIX_REQ );
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_MONSTER_CARD_MIX_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_vecDecreaseCountItem = kPacket_.m_vecMaterialItem;
    kPacket.m_vecRewards.clear();

    std::map<GCITEMUID,int>::iterator mit;
    std::vector<KItem>::iterator vit;

    // 아이템 삽입
    for ( vit = kPacket_.m_vecRewards.begin() ; vit != kPacket_.m_vecRewards.end() ; ++vit ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nCount, vit->m_nPeriod, KItemManager::EWIT_MONSTER_CARD_MIX, kPacket.m_vecRewards ) ) {
            START_LOG( cerr, L"몬스터카드 조합 결과 아이템 삽입 실패 Name : " << strLogin_ )
                << BUILD_LOG( vit->m_ItemID )
                << BUILD_LOG( vit->m_nCount )
                << BUILD_LOG( vit->m_nPeriod ) << END_LOG;
            // 에러기록 남기고 진행
        }
    }

    // DB 처리 실패
    if ( kPacket.m_vecRewards.empty() ) {
        SET_ERR_GOTO( ERR_MONSTER_CARE_MIX_07, END_PROC );
    }

    // 재료 아이템 차감
    for ( mit = kPacket_.m_mapMaterialItemCount.begin() ; mit != kPacket_.m_mapMaterialItemCount.end() ; ++mit ) {
        const GCITEMUID& ItemUID = mit->first;
        const int& nCount = mit->second;

        vit = std::find_if( kPacket.m_vecDecreaseCountItem.begin(), kPacket.m_vecDecreaseCountItem.end(),
            boost::bind( &KItem::m_ItemUID, boost::placeholders::_1 ) == ItemUID );

        if ( vit == kPacket.m_vecDecreaseCountItem.end() ) {
            START_LOG( cerr, L"차감하려는 재료 아이템 정보가 없음.. Login : " << strLogin_ )
                << BUILD_LOG( ItemUID )
                << BUILD_LOG( kPacket_.m_vecMaterialItem.size() )
                << END_LOG;
            continue;
        }

        // 동기화전 InitCount, Count값 변동값 있는지 체크.
        if( !CheckItemDuration( dwUID_, vit->m_ItemUID, vit->m_nInitCount, vit->m_nCount ) ) {
            START_LOG( clog, L"몬스터카드 조합재료 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( vit->m_ItemUID )
                << BUILD_LOG( vit->m_nInitCount )
                << BUILD_LOG( vit->m_nCount )
                << END_LOG;
        }

        // 수량 차감
        if ( false == UseCountItem( dwUID_, ItemUID, nCount ) ) {
            vit->m_nCount = 0;
            START_LOG( cerr, L"몬스터카드 조합재료 수량동기화 실패. Name : " << strLogin_ ) << END_LOG;
        } else {
            vit->m_nCount -= nCount;
            vit->m_nCount = std::max<int>( vit->m_nCount, 0 );
            vit->m_nInitCount = vit->m_nCount;
        }
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_vecRewards.size() )
        << BUILD_LOG( kPacket_.m_vecMaterialItem.size() )
        << BUILD_LOG( kPacket_.m_mapMaterialItemCount.size() )
        << BUILD_LOG( kPacket.m_vecDecreaseCountItem.size() )
        << BUILD_LOG( kPacket.m_vecRewards.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_MONSTER_CARD_MIX_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_SELECT_GACHA_NOTICE_POPUP_INFO )
{
    // 튜토리얼 진행여부
    bool kPacket = false;

    int nRet = GetTodayEventCount( dwUID_, UDT_GACHA_LEVEL_NOTICE, 0 );
    if ( nRet != 0 ) {
        kPacket = true; // 이미 가차 레벨 공지를 확인했음.
    }

    START_LOG( clog, L"가차 레벨 공지 확인. Login : " << strLogin_ )
        << BUILD_LOG( nRet )
        << BUILD_LOG( kPacket ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_GACHA_NOTICE_POPUP_INFO_NOT );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_UPDATE_GACHA_POPUP_INFO )
{
    LIF( SetTodayEventDone( dwUID_, UDT_GACHA_LEVEL_NOTICE, 0, 1 ) );
}

INT_IMPL_ON_FUNC( EVENT_ADD_SKILL_SLOT_OPEN_REQ )
{
    EVENT_CHECK_OUT( EVENT_ADD_SKILL_SLOT_OPEN_REQ );
    SET_ERROR( ERR_UNKNOWN );
    KEVENT_ADD_SKILL_SLOT_OPEN_ACK kPacket;
    kPacket.m_nOK = -99;

    // 슬롯 인덱스 오픈 정보 저장.
    if( !UpdateCharSlotIndex( dwUID_, kPacket_.m_cCharType, kPacket_.m_nAddSlotIndex ) ) {
        SET_ERR_GOTO( ERR_SLOTINDEX_OPEN_05, END_PROC );
    }

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_kUseItem.m_ItemUID, kPacket_.m_kUseItem.m_nInitCount, kPacket_.m_kUseItem.m_nCount ) ) {
        START_LOG( clog, L"슬롯 인덱스 오픈 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_kUseItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_kUseItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_kUseItem.m_nCount )
            << END_LOG;
    }

    if ( !UseCountItem( dwUID_, kPacket_.m_kUseItem.m_ItemUID, 1 ) ) {
        START_LOG( cerr, L"슬롯 인덱스 오픈 아이템 개수 동기화 실패. Name : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_kUseItem.m_ItemUID ) << END_LOG;
    }

    kPacket_.m_kUseItem.m_nCount = -1;
    kPacket.m_vecUseItem.push_back( kPacket_.m_kUseItem );

    kPacket.m_cCharType = kPacket_.m_cCharType;
    kPacket.m_nAddSlotIndex = kPacket_.m_nAddSlotIndex;
    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_ADD_SKILL_SLOT_OPEN_ACK );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_kUseItem.m_ItemID )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOGc( kPacket_.m_nAddSlotIndex )
        << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_STAT_DUNGEON_CHARACTER_NOT )
{
    LIF( DungeonCharStat_insert( kPacket_ ) );
    START_LOG( clog, L"던전 캐릭터 통계 DB기록.. size : " << kPacket_.size() )
        << BUILD_LOG( m_kODBCStat.m_strLastQuery )
        << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_USER_DUNGEON_STAT_NOT )
{
    LIF( UserDungeonCollect_insert( kPacket_ ) );
    START_LOG( clog, L"유저 던전 통계 DB기록.. size : " << kPacket_.size() )
        << BUILD_LOG( m_kODBCStat.m_strLastQuery )
        << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_STAT_CHARACTER_SKILL_COUNT, KEVENT_STAT_CHARACTER_SKILL_COUNT )
{
    LIF( DungeonCharSkillCount_insert( dwUID_, kPacket_ ) );
    START_LOG( clog, L"던전 캐릭터 스킬 횟수 통계 DB기록.. size : " ) 
        << BUILD_LOG( kPacket_.first )
        << BUILD_LOG( kPacket_.second )
        << BUILD_LOG( m_kODBCStat.m_strLastQuery )
        << END_LOG;
}

_INT_IMPL_ON_FUNC( DB_EVENT_SONGKRAN_USER_INFO_REQ, bool )
{
    EVENT_CHECK_OUT( DB_EVENT_SONGKRAN_USER_INFO_REQ );
    SET_ERROR( ERR_UNKNOWN );

    KDB_EVENT_SONGKRAN_USER_INFO_ACK kPacket;
    CTime tmCurrent = CTime::GetCurrentTime();
    time_t tResetTime = KncUtil::TimeToInt( tmCurrent );
    CTime tmResetTime;
    kPacket.m_bResetEnable = false;

    GetSongkranEventResetTime( dwUID_, tResetTime );
    tmResetTime = CTime( tResetTime );

    // 데이터 초기화 시점이 오늘 이전이라면 초기화 가능
    if ( tmResetTime.GetYear() < tmCurrent.GetYear() ||
        ( tmResetTime.GetYear() <= tmCurrent.GetYear() && tmResetTime.GetMonth() < tmCurrent.GetMonth() ) ||
        ( tmResetTime.GetYear() <= tmCurrent.GetYear() && tmResetTime.GetMonth() <= tmCurrent.GetMonth() && tmResetTime.GetDay() < tmCurrent.GetDay() ) ) {
            kPacket.m_bResetEnable = true;
    }

    GetMultiEventUserData( dwUID_, UET_SONGKRAN, SiKSongkran()->GetVersion(),
        kPacket.m_dwScore, kPacket.m_dwGrade, kPacket.m_dwWaterBombDuration, kPacket.m_dwSuppliedWaterBombDuration, kPacket.m_nAccTime );

    // 금일 처음 접속, 신규 유저 물풍선 지급
    // 이전의 지급 누적 개수 초기화
    // 추가 : 00:00 자정 이후 접속을 유지한 상태로 초기화를 받지 않았을 경우
    if ( true == kPacket.m_bResetEnable ) { // kPacket.m_bResetEnable 로 첫 접속 판단이 되기 때문에 true == kPacket_ 조건은 제거함. 2013. 06. 17
        DWORD dwVersion = SiKSongkran()->GetVersion();
        DWORD dwType = UET_SONGKRAN;
        // 달력 정보 갱신.
        LIF( UpdateMultiEventUserData( dwUID_, dwType, dwVersion, 0, 0, SiKSongkran()->GetDailySupplyDuration(), 0, 0 ) );
        kPacket.m_dwSuppliedWaterBombDuration = 0;
    }

    if ( true == kPacket.m_bResetEnable ) {
        UpdateSongkranEventResetTime( dwUID_, tmCurrent );
    }

    SET_ERROR( NET_OK );

    kPacket.m_nOK = NetError::GetLastNetError();

    QUEUING_ACK_TO_USER( DB_EVENT_SONGKRAN_USER_INFO_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_SONGKRAN_GRADE_REWARD_REQ, KDB_EVENT_SONGKRAN_GRADE_REWARD_REQ )
{
    EVENT_CHECK_OUT( EVENT_SONGKRAN_GRADE_REWARD_REQ );
    KEVENT_SONGKRAN_GRADE_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );
    DWORD dwGrade = 1;
    DWORD dwVersion = SiKSongkran()->GetVersion();
    DWORD dwMaxGrade = SiKSongkran()->GetMaxGrade();
    DWORD dwType = UET_SONGKRAN;
    std::vector<KDropItemInfo>::iterator vit;

    // 현재 등급의 아이템을 받았는지 체크.
    if( !CheckGradeReward( dwUID_, dwType, dwVersion, kPacket_.m_dwGrade ) ) {
        SET_ERR_GOTO( ERR_SONGKRAN_10, END_PROC );
    }

    // 보상 지급
    for ( vit = kPacket_.m_vecReward.begin(); vit != kPacket_.m_vecReward.end(); ++vit ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, 
            cCharType_,
            vit->m_ItemID, 
            vit->m_nDuration, 
            vit->m_nPeriod, 
            KItemManager::EWIT_SONGKRAN_EVENT_REWARD,
            kPacket.m_vecReward ) ) {
                START_LOG( cerr, L"송크란 보상 아이템 지급 실패 UID : " << dwUID_ )
                    << BUILD_LOG( vit->m_ItemID )
                    << BUILD_LOG( vit->m_nDuration )
                    << BUILD_LOG( vit->m_nPeriod ) 
                    << END_LOG;

                SET_ERR_GOTO( ERR_SONGKRAN_05, END_PROC );
        }
    }

    dwGrade = kPacket_.m_dwGrade;
    ++dwGrade;
    kPacket.m_dwGrade = dwGrade;
    // 현재 유저의 포인트/ 등급 갱신.
    if( !UpdateMultiEventUserData( dwUID_, dwType, dwVersion, 0, 
        1, 0, kPacket_.m_dwSuppliedDuration, kPacket_.m_nAccTime ) ) {
        START_LOG( cerr, L"송크란 이벤트 유저 데이터 갱신 실패. Name : " << strLogin_ )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( dwType )
            << BUILD_LOG( dwVersion )
            << BUILD_LOG( kPacket_.m_dwGrade )
            << END_LOG;
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_SONGKRAN_GRADE_REWARD_ACK );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_dwGrade )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecReward.size() )
        << END_LOG;
}

_INT_IMPL_ON_FUNC( DB_EVENT_SONGKRAN_WATERBOMB_EXCHANGEITEM_USE_REQ, KItem )
{
    EVENT_CHECK_OUT( DB_EVENT_SONGKRAN_WATERBOMB_EXCHANGEITEM_USE_REQ );
    KEVENT_SONGKRAN_WATERBOMB_EXCHANGEITEM_USE_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );
    DWORD dwGrade = 1;
    DWORD dwVersion = SiKSongkran()->GetVersion();
    DWORD dwType = UET_SONGKRAN;

    // 풍선 교환 아이템 개수 동기화가 가능한지 우선 체크.
    if ( !CheckUpdateCountItem( dwUID_, kPacket_.m_ItemID, kPacket_.m_cGradeID, kPacket_.m_nCount, cCharType_ ) ) {
        START_LOG( cerr, L"풍선 아이템 개수 체크 실패. Name : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_ItemUID )
            << END_LOG;

        SET_ERROR( ERR_SONGKRAN_11 );
    }

    if( !UpdateWateBombCount( dwUID_, dwType, dwVersion, kPacket_.m_nCount ) ) {
        START_LOG( cerr, L"풍선 아이템 DB 갯수 갱신 실패. Name : " << strLogin_ )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( dwType )
            << BUILD_LOG( dwVersion )
            << BUILD_LOG( kPacket_.m_nCount )
            << END_LOG;

        SET_ERR_GOTO( ERR_SONGKRAN_13, END_PROC );
    }

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_ItemUID, kPacket_.m_nInitCount, kPacket_.m_nCount ) ) {
        START_LOG( clog, L"풍선 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_ItemUID )
            << BUILD_LOG( kPacket_.m_nInitCount )
            << BUILD_LOG( kPacket_.m_nCount )
            << END_LOG;
    }

    if ( !UseCountItem( dwUID_, kPacket_.m_ItemUID, kPacket_.m_nCount ) ) {
        START_LOG( cerr, L"풍선 아이템 개수 동기화 실패. Name : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_ItemUID )
            << END_LOG;

        SET_ERROR( ERR_SONGKRAN_12 );
    }

    kPacket.m_dwWaterBombDuration = kPacket_.m_nCount; //  갱신되는 갯수만 일단 넘긴다.
    kPacket_.m_nInitCount = 0;
    kPacket_.m_nCount = 0;
    kPacket.m_kUseItem = kPacket_;

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_SONGKRAN_WATERBOMB_EXCHANGEITEM_USE_ACK );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_dwWaterBombDuration )
        << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_SONGKRAN_WATERBOMB_USE_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_SONGKRAN_WATERBOMB_USE_REQ );
    KEVENT_SONGKRAN_WATERBOMB_USE_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );
    DWORD dwVersion = SiKSongkran()->GetVersion();
    DWORD dwType = UET_SONGKRAN;

    // 현재 유저의 포인트/물풍선 갯수 갱신.
    if( !UpdateMultiEventUserData( dwUID_, dwType, dwVersion, kPacket_.m_dwScoreDiff, 
        0, -1, kPacket_.m_dwSuppliedDuration, kPacket_.m_nAccTime ) ) {
            START_LOG( cerr, L"송크란 이벤트 유저 데이터 갱신 실패. Name : " << strLogin_ )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( dwType )
                << BUILD_LOG( dwVersion )
                << BUILD_LOG( kPacket_.m_dwScoreDiff )
                << BUILD_LOG( kPacket_.m_dwSuppliedDuration )
                << BUILD_LOG( kPacket_.m_nAccTime )
                << END_LOG;

        SET_ERROR( ERR_SONGKRAN_13 );
    }

    kPacket.m_dwScore = kPacket_.m_dwScoreDiff;

    if ( !IS_CORRECT( ERR_SONGKRAN_13 ) ) {
    SET_ERROR( NET_OK );
    }

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_SONGKRAN_WATERBOMB_USE_ACK );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwScore )
        << BUILD_LOG( kPacket_.m_dwSuppliedDuration )
        << BUILD_LOG( kPacket_.m_nAccTime )
        << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_SONGKRAN_WATERBOMB_NOT )
{
    DWORD dwVersion = SiKSongkran()->GetVersion();
    DWORD dwType = UET_SONGKRAN;
    // 달력 정보 갱신.
    LIF( UpdateMultiEventUserData( dwUID_, dwType, dwVersion, 0, 0, kPacket_.m_dwSuppliedDuration, kPacket_.m_dwTotalSuppliedDuration, 0 ) );

    START_LOG( clog, L"송크란 이벤트 물풍선 무료 지급 UID : " << dwUID_ )
        << BUILD_LOG( kPacket_.m_dwTotalSuppliedDuration )
        << BUILD_LOG( kPacket_.m_dwSuppliedDuration )
        << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_ATTEND_GET_CHAR_REQ )
{
    KEVENT_ATTEND_GET_CHAR_ACK kPacket;
    time_t tmBeginDate = time_t(NULL);
    SiKGCHelper()->GetBeginDateGetCharByAttend( tmBeginDate );
    // n 회 출석시에 캐릭터 획득 카드 지급 이벤트 ( 브라질, 아신, 유성철. 업데이트 하루 전인데.. )
    if ( false == SetTodayEventDoneDiff( dwUID_, UET_GET_NEW_CHAR_ATTEND, tmBeginDate, 1 ) ) {
        //START_LOG( cerr, L"출석시 캐릭터 획득 카드 지급 이벤트용 SDGA 기록 실패" )
    }
    std::vector<KDropItemInfo> vecRewardItem;
    std::vector<KDropItemInfo>::iterator vit;
    std::vector<KItem> vecItem;
    vecItem.clear();

    int nAttendCount = GetTodayEventCount( dwUID_, UET_GET_NEW_CHAR_ATTEND, tmBeginDate );

    if ( nAttendCount == SiKGCHelper()->GetAttendCountGetChar() ) {
        SiKGCHelper()->GetAttendGetCharRewardItem( vecRewardItem );
        // 아이템 별 반복
        for ( vit = vecRewardItem.begin(); vit != vecRewardItem.end(); ++vit ) {
            if ( false == m_kHelper.InsertSingleItemJob( dwUID_, 
                cCharType_,
                vit->m_ItemID, 
                vit->m_nDuration, 
                vit->m_nPeriod, 
                KItemManager::EWIT_ATTEND_REWARD_GET_CHAR, 
                vecItem ) ) 
            {
                START_LOG( cerr, L"출석 n 회 보상 실패 UID : " << dwUID_ )
                    << BUILD_LOG( vit->m_ItemID )
                    << BUILD_LOG( vit->m_nDuration )
                    << BUILD_LOG( vit->m_nPeriod )
                    << END_LOG;

                continue;
            }
        }
    }

    kPacket.m_vecRewardItem = vecItem;

    QUEUING_ACK_TO_USER( EVENT_ATTEND_GET_CHAR_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_STAT_VIEW_GRADE_TIME_NOT, KUserViewGradeTime )
{
    LIF( ViewGradeTime_insert( kPacket_ ) );
    START_LOG( clog, L"시야 등급 시간 통계 DB기록..LoginUID  : " << kPacket_.m_dwUserUID )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;
}

_INT_IMPL_ON_FUNC( DB_EVENT_ADVENTURE_DATA_REQ, int )
{   // 접속시 요청은 kPacket_이 1이고, 이후 요청은 0으로 정함
    KEVENT_ADVENTURE_DATA_ACK kPacket;

    SET_ERROR( ERR_UNKNOWN );

    int nVersion = SiKAdventure()->GetVersion();

    if ( nVersion == -1 ) {
        SET_ERR_GOTO( ERR_ADVENTURE_01, END_PROC );
    }

    JIF( GetAdventureCollectCountInfo( dwUID_, nVersion, kPacket.m_mapCollectData ) );
    JIF( GetAdventureContinentRewardInfo( dwUID_, nVersion, kPacket.m_mapContinentData ) );

    SET_ERROR( NET_OK );

    if ( kPacket_ == 1 ) {  // 성공이고 접속시 요청이면 OK 코드를 다르게
        SET_ERROR( ERR_ADVENTURE_16 );
    }

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    QUEUING_ACK_TO_USER( EVENT_ADVENTURE_DATA_ACK );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK || kPacket.m_nOK == NetError::ERR_ADVENTURE_16 )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( nVersion )
        << BUILD_LOG( kPacket.m_mapCollectData.size() )
        << BUILD_LOG( kPacket.m_mapContinentData.size() )
        << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_ADVENTURE_CONTINENT_REWARD_REQ )
{
    EVENT_CHECK_OUT( EVENT_ADVENTURE_CONTINENT_REWARD_REQ );
    KEVENT_ADVENTURE_CONTINENT_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;
    std::vector< KDropItemInfo >::iterator vit;

    SET_ERROR( ERR_UNKNOWN );
    int nVersion = SiKAdventure()->GetVersion();

    if ( nVersion == -1 ) {
        SET_ERR_GOTO( ERR_ADVENTURE_01, END_PROC );
    }

    // 대륙 보상 주자
    if ( !UpdateContinentUserData( dwUID_, nVersion, kPacket_.m_nContinentID, 1 ) ) {
        SET_ERR_GOTO( ERR_ADVENTURE_06, END_PROC );
    }

    vit = kPacket_.m_vecDropItemInfo.begin();
    for ( ; vit != kPacket_.m_vecDropItemInfo.end(); ++vit ) {
        if ( !m_kHelper.InsertSingleItemJob( dwUID_,
            cCharType_,
            vit->m_ItemID,
            vit->m_nDuration,
            vit->m_nPeriod,
            KItemManager::EWIT_ADVENTURE_CONTINENT_REWARD,
            kPacket.m_vecReward ) ) {
                SET_ERR_GOTO( ERR_ADVENTURE_07, END_PROC );
        }
    }

    kPacket.m_nContinentID = kPacket_.m_nContinentID;

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    QUEUING_ACK_TO_USER( EVENT_ADVENTURE_CONTINENT_REWARD_ACK );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( nVersion )
        << BUILD_LOG( kPacket_.m_nContinentID )
        << BUILD_LOG( kPacket_.m_vecDropItemInfo.size() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecReward.size() )
        << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_ADVENTURE_FINAL_REWARD_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_ADVENTURE_FINAL_REWARD_REQ );
    KEVENT_ADVENTURE_FINAL_REWARD_ACK kPacket;
    std::map< int, KContinentRewardInfo > mapContinentRewardInfo;
    int nVersion = SiKAdventure()->GetVersion();

    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    if ( nVersion == -1 ) {
        SET_ERR_GOTO( ERR_ADVENTURE_01, END_PROC );
    }

    if ( !m_kHelper.InsertSingleItemJob(
        dwUID_,
        cCharType_,
        kPacket_.m_ItemID,
        kPacket_.m_nDuration,
        kPacket_.m_nPeriod,
        KItemManager::EWIT_ADVENTURE_FINAL_REWARD,
        kPacket.m_vecReward ) ) {
            SET_ERR_GOTO( ERR_ADVENTURE_12, END_PROC );
    }

    if ( !SiKAdventure()->GetContinentRewardListInfo( mapContinentRewardInfo ) ) {
        SET_ERR_GOTO( ERR_ADVENTURE_04, END_PROC);
    }

    UpdateAdventureUserData( dwUID_, nVersion, mapContinentRewardInfo );

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    QUEUING_ACK_TO_USER( EVENT_ADVENTURE_FINAL_REWARD_ACK );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( nVersion )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecReward.size() )
        << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_ADVENTURE_USER_COLLECT_DATA_NOT )
{
    EVENT_CHECK_OUT( EVENT_ADVENTURE_USER_COLLECT_DATA_NOT );

    int nVersion = SiKAdventure()->GetVersion();

    if ( nVersion == -1 ) {
        START_LOG( cerr, L"모험 이벤트 기간이 아니거나 버전정보가 없음. dwUID_: " << dwUID_ ) << END_LOG;
        return;
    }

    int nUpdateCount = 0;
    // 모험 이벤트 유저 전리품 정보 기록
    std::map< int, std::map< DWORD, int > >::iterator mitCollectData = kPacket_.begin();
    for ( ; mitCollectData != kPacket_.end(); ++mitCollectData ) {
        std::map< DWORD, int >::iterator mitCollect = mitCollectData->second.begin();
        for ( ; mitCollect != mitCollectData->second.end(); ++mitCollect ) {
            if ( mitCollect->second > 0 ) { // 0보다 클 경우에만 DB에 요청한다
                UpdateModeCountUserData( dwUID_, nVersion, mitCollectData->first, mitCollect->first, mitCollect->second );
                nUpdateCount++;
            }
        }
    }

    START_LOG( clog, L"모험 이벤트 전리품 정보 기록 DB요청 완료." )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( nUpdateCount )
        << END_LOG;
}

_INT_IMPL_ON_FUNC( DB_EVENT_LEVEL_RESET_EVENT_INFO_REQ, DWORD )
{
    EVENT_CHECK_OUT( DB_EVENT_LEVEL_RESET_EVENT_INFO_REQ );
    KEVENT_LEVEL_RESET_EVENT_INFO_ACK kPacket;    

    SET_ERROR( ERR_UNKNOWN );

    GetLevelResetEventUserData( dwUID_, kPacket_, kPacket.m_mapResetCharInfo );

    SET_ERROR( NET_OK );

    kPacket.m_nOK = NetError::GetLastNetError();

    QUEUING_ACK_TO_USER( EVENT_LEVEL_RESET_EVENT_INFO_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_LEVEL_RESET_EVENT_ITEM_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_LEVEL_RESET_EVENT_ITEM_REQ );
    KEVENT_LEVEL_RESET_EVENT_ITEM_ACK kPacket;

    std::vector<DWORD>::iterator iter;
    std::vector<DWORD>::iterator iter_end;

    SET_ERROR( ERR_UNKNOWN );

    //  db에 해당 캐릭터에 대한 정보 초기화
    if ( false == SetLevelResetEventChar( dwUID_, kPacket_.m_dwVersion, kPacket_.m_cCharType, false, kPacket_.m_bCashType ) ) 
    {
        SET_ERR_GOTO( ERR_USE_LEVEL_RESET_ITEM_04, END_PROC );
    }

    //  경험치 초기화
    if ( false == SetLevelResetEventExp( strLogin_, dwUID_, kPacket_.m_cCharType, kPacket_.m_biDiffExp ) )
    {
        SET_ERR_GOTO( ERR_USE_LEVEL_RESET_ITEM_05, END_PROC );
    }

    // 미션 삭제 
    iter = kPacket_.m_vecMissionList.begin();
    iter_end = kPacket_.m_vecMissionList.end();

    for ( ; iter != iter_end; ++iter ) {
        if ( false == RemoveMission( dwUID_, cCharType_, *iter, KMissionInfo::DEL_TYPE_USER_DEL ) ) {
            SET_ERR_GOTO( ERR_USE_LEVEL_RESET_ITEM_07, END_PROC );
        }
    }

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_kItem.m_ItemUID, kPacket_.m_kItem.m_nInitCount, kPacket_.m_kItem.m_nCount ) ) {
        START_LOG( clog, L"몬스터 카드 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_kItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_kItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_kItem.m_nCount )
            << END_LOG;
    }

    //  레벨 초기화 아이템 수량 까기
    if ( !UseCountItem( dwUID_, kPacket_.m_kItem.m_ItemUID, 1 ) ) {
        START_LOG( cerr, L"몬스터 카드 개수 동기화 실패. Name : " << strLogin_ ) << END_LOG;
        kPacket_.m_kItem.m_nCount = 0;
        SET_ERR_GOTO( ERR_USE_LEVEL_RESET_ITEM_08, END_PROC );      
    }

    kPacket_.m_kItem.m_nCount -= 1;
    kPacket_.m_kItem.m_nCount = std::max<int>( kPacket_.m_kItem.m_nCount, 0 );


    kPacket.m_cCharType = kPacket_.m_cCharType;
    kPacket.m_bCashType = kPacket_.m_bCashType;
    kPacket.m_kItem = kPacket_.m_kItem;
    kPacket.m_vecMissionList.swap( kPacket_.m_vecMissionList );

    SET_ERROR( NET_OK );

END_PROC:

    kPacket.m_nOK = NetError::GetLastNetError(); 

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_biDiffExp )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_vecMissionList.size() )
        << END_LOG;


    QUEUING_ACK_TO_USER( EVENT_LEVEL_RESET_EVENT_ITEM_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_LEVEL_RESET_EVENT_REWARD_REQ )
{
    KEVENT_LEVEL_RESET_EVENT_REWARD_ACK kPacket;

    SET_ERROR( ERR_UNKNOWN );

    //  db에 해당 캐릭터에 대한 정보 초기화
    if ( false == SetLevelResetEventChar( dwUID_, kPacket_.m_dwVersion, kPacket_.m_cCharType, true, kPacket_.m_bCashType ) ) 
    {
        SET_ERR_GOTO( ERR_USE_LEVEL_RESET_REWARD_01, END_PROC );
    }


    // 아이템 등록
    if ( !kPacket_.m_vecItemInfo.empty() ) {
        std::vector<KDropItemInfo>::iterator vit;
        for ( vit = kPacket_.m_vecItemInfo.begin() ; vit != kPacket_.m_vecItemInfo.end() ; ++vit ) {
            LIF( m_kHelper.InsertSingleItemJob( dwUID_, kPacket_.m_cCharType, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_LEVEL_RESET_EVENT_REWARD, kPacket.m_vecRewardItem ) );
        }
    }

    SET_ERROR( NET_OK );

    kPacket.m_cCharType = kPacket_.m_cCharType;

END_PROC:

    kPacket.m_nOK = NetError::GetLastNetError();     

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_vecItemInfo.size() )
        << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_LEVEL_RESET_EVENT_REWARD_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_LOGIN_TIME_UPDATE_REQ )
{
    UpdateFinalLoginTime( dwUID_ );
    START_LOG( clog, L"00:00 지나서 FinalLoginTime 기록 해줌" << dwUID_ ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_TEST_ADD_ITEM_REQ )
{
    KEVENT_TEST_ADD_ITEM_ACK kPacket;

    // 아이템 등록
    if ( kPacket_.m_itemID != 0 ) {
        if ( !m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, kPacket_.m_itemID, kPacket_.m_nDuration, kPacket_.m_nPeriod, KItemManager::EWIT_TEST_ADD_ITEM, kPacket.m_vecRewardItem, kPacket_.m_nGrade, kPacket_.m_nItemLevel ) )
            SET_ERR_GOTO( ERR_TEST_ADD_ITEM_02, END_PROC );
    }

    SET_ERROR( NET_OK );

END_PROC:

    kPacket.m_nOK = NetError::GetLastNetError();     
    QUEUING_ACK_TO_USER( EVENT_TEST_ADD_ITEM_ACK );
}

INT_IMPL_ON_FUNC(EVENT_NEW_CHAR_CHOICE_REQ)
{
    EVENT_CHECK_OUT(EVENT_NEW_CHAR_CHOICE_REQ);
    KEVENT_NEW_CHAR_CHOICE_ACK kPacket;
    kPacket.m_nOK = -99;
    KEVENT_CONNECTION_GIFTBOX_FIRST_CHARACTER_GET_GIFT_NOT kGiftBoxNotPacket;
    kGiftBoxNotPacket.clear();

    int nOK = InsertSlotCharInfo(dwUID_, kPacket_.m_cCharType, kPacket_.m_dwSlotNumber, kPacket.m_kCharacterInfo);

    if (nOK != 0) {
        switch (nOK) {
        case -1: SET_ERROR(ERR_NEWCHAR_CHOICE_09); break;
        case -2: SET_ERROR(ERR_NEWCHAR_CHOICE_10); break;
        case -3: SET_ERROR(ERR_NEWCHAR_CHOICE_11); break;
        case -4: SET_ERROR(ERR_NEWCHAR_CHOICE_12); break;
        default:
            SET_ERROR(ERR_NEWCHAR_CHOICE_02);
        }
        START_LOG(cerr, L" 캐릭터 생성 중 오류") << END_LOG;
        goto END_PROC;
    }

    if (!kPacket_.m_vecDefaultReward.empty()) {
        std::vector<KDropItemInfo>::iterator vitReward;
        for (vitReward = kPacket_.m_vecDefaultReward.begin(); vitReward != kPacket_.m_vecDefaultReward.end(); ++vitReward) {
            if (false == m_kHelper.InsertSingleItemJob(dwUID_,
                kPacket_.m_cCharType,
                vitReward->m_ItemID,
                vitReward->m_nDuration,
                vitReward->m_nPeriod,
                KItemManager::EWIT_CHAR_DEFAULT_REWARD,
                kPacket.m_vecRewardItem)) {
                START_LOG(cerr, L"캐릭터 생성 보상 아이템 지급 실패 UID : " << dwUID_)
                    << BUILD_LOG(vitReward->m_ItemID)
                    << BUILD_LOG(vitReward->m_nDuration)
                    << BUILD_LOG(vitReward->m_nPeriod)
                    << END_LOG;
                continue;
            }
        }
    }

    if (!kPacket_.m_vecDefaultSlotReward.empty()) {
        std::vector<KDropItemInfo>::iterator vitSlot;
        for (vitSlot = kPacket_.m_vecDefaultSlotReward.begin(); vitSlot != kPacket_.m_vecDefaultSlotReward.end(); ++vitSlot) {
            if (false == m_kHelper.InsertSingleItemJob(dwUID_,
                kPacket_.m_cCharType,
                vitSlot->m_ItemID,
                vitSlot->m_nDuration,
                vitSlot->m_nPeriod,
                KItemManager::EWIT_CHAR_DEFAULT_REWARD,
                kPacket.m_vecSlotItem)) {
                START_LOG(cerr, L"캐릭터 생성 보상 슬롯 포션 아이템 지급 실패 UID : " << dwUID_)
                    << BUILD_LOG(vitSlot->m_ItemID)
                    << BUILD_LOG(vitSlot->m_nDuration)
                    << BUILD_LOG(vitSlot->m_nPeriod)
                    << END_LOG;
                continue;
            }
        }
    }

    int nCharType = static_cast<int>(kPacket_.m_cCharType);
    if (true == SiKSealedChar()->IsEventTerm(nCharType)) {
        START_LOG(clog, L"해당 캐릭터는 봉인 이벤트 진행중이므로 봉인 상태로 지급요청: ")
            << BUILD_LOG(dwUID_)
            << BUILD_LOG(kPacket_.m_cCharType)
            << END_LOG;
        CharacterSeal_Merge(dwUID_, nCharType, true);
    }

    kPacket.m_kSealedCharInfoNot.m_nOK = KEVENT_SEALED_CHARACTER_INFO_NOT::ON_NEW_CHAR_CHOICE;
    GetSealedCharInfo(dwUID_, kPacket.m_kSealedCharInfoNot.m_mapSealedCharInfo);

    if (false == kPacket_.m_vecGiftFirstCharacter.empty()) {
        std::vector< KDropItemInfo >::iterator vitGiftFirstCharacter;
        for (vitGiftFirstCharacter = kPacket_.m_vecGiftFirstCharacter.begin(); vitGiftFirstCharacter != kPacket_.m_vecGiftFirstCharacter.end(); ++vitGiftFirstCharacter) {
            if (false == m_kHelper.InsertSingleItemJob(dwUID_,
                kPacket_.m_cCharType,
                vitGiftFirstCharacter->m_ItemID,
                vitGiftFirstCharacter->m_nDuration,
                vitGiftFirstCharacter->m_nPeriod,
                KItemManager::EWIT_CONNECTION_GIFTBOX_FIRST_CHARACTER_REWARD,
                kGiftBoxNotPacket)) {
                START_LOG(cerr, L"첫 캐릭터 접속 선물상자 시스템 보상 아이템 지급 실패 UID : " << dwUID_)
                    << BUILD_LOG(vitGiftFirstCharacter->m_ItemID)
                    << BUILD_LOG(vitGiftFirstCharacter->m_nDuration)
                    << BUILD_LOG(vitGiftFirstCharacter->m_nPeriod)
                    << END_LOG;
                continue;
            }
        }
    }

    SET_ERROR(NET_OK);

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER(EVENT_NEW_CHAR_CHOICE_ACK);

    LOG_SUCCESS(NetError::GetLastNetError() == NetError::NET_OK)
        << L"Msg : " << NetError::GetLastNetErrMsg()
        << BUILD_LOG(strLogin_)
        << BUILD_LOG(kPacket.m_nOK)
        << BUILD_LOG(kPacket.m_kSealedCharInfoNot.m_nOK)
        << BUILD_LOG(kPacket.m_kSealedCharInfoNot.m_mapSealedCharInfo.size())
        << BUILD_LOGc(kPacket_.m_cCharType)
        << BUILD_LOG(kPacket_.m_vecDefaultReward.size())
        << BUILD_LOG(kPacket_.m_vecDefaultSlotReward.size())
        << BUILD_LOG(kGiftBoxNotPacket.size())
        << END_LOG;

    if (false == kGiftBoxNotPacket.empty()) {
        _QUEUING_ACK_TO_USER(EVENT_CONNECTION_GIFTBOX_FIRST_CHARACTER_GET_GIFT_NOT, kGiftBoxNotPacket);
    }
}

INT_IMPL_ON_FUNC( DB_EVENT_SEALED_CHARACTER_UPDATE_NOT )
{
    CharacterSeal_Merge( dwUID_, kPacket_.m_nCharType, kPacket_.m_bSealed );

    START_LOG( clog, L"봉인 캐릭터 정보 갱신 요청: " << dwUID_ )
        << BUILD_LOG( kPacket_.m_nCharType )
        << BUILD_LOG( kPacket_.m_bSealed )
        << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_OLYMPIC_WATERBOMB_RESULT_REQ )
{
    EVENT_CHECK_OUT( EVENT_OLYMPIC_WATERBOMB_RESULT_REQ );
    KEVENT_OLYMPIC_WATERBOMB_RESULT_ACK kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    if ( kPacket_.m_nIncreasedData <= 0 ) {
        SET_ERROR( ERR_OLYMPIC_12 );
    }
    else {
        if ( false == OlympicUserData_merge( dwUID_, kPacket_.m_nIncreasedData, kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType ) ) {
            SET_ERR_GOTO( ERR_OLYMPIC_05, END_PROC );
        }

        switch ( kPacket_.m_nTeamType ) {
            case KOlympicSchedule::OGT_SOLO:
                SiKOlympicManager()->UpdateOlympicSoloPlayData( strLogin_, kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType, kPacket_.m_nMyAccumulateData + kPacket_.m_nIncreasedData );
                break;

            case KOlympicSchedule::OGT_TEAM:
                SiKOlympicManager()->UpdateOlympicTeamPlayData( kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType, kPacket_.m_nIncreasedData );
                break;

            default:
                START_LOG( cerr, L"올림픽 이벤트 플레이데이터 저장 중 팀 타입 오류: " << dwUID_ )
                    << BUILD_LOG( kPacket_.m_tmTodayDate )
                    << BUILD_LOG( kPacket_.m_nGameType )
                    << BUILD_LOG( kPacket_.m_nTeamType )
                    << END_LOG;
                break;
        }

        SET_ERROR( NET_OK );
    }

    kPacket.m_nIncreasedData = kPacket_.m_nIncreasedData;
    kPacket.m_tmTodayDate = kPacket_.m_tmTodayDate;

    START_LOG( clog, L"올림픽 물폭탄 누적 포인트 갱신 요청: " << dwUID_ )
        << BUILD_LOG( kPacket_.m_nIncreasedData )
        << BUILD_LOG( kPacket_.m_tmTodayDate )
        << BUILD_LOG( kPacket_.m_nGameType )
        << BUILD_LOG( kPacket_.m_nTeamType )
        << END_LOG;

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_OLYMPIC_WATERBOMB_RESULT_ACK );
}

INT_IMPL_ON_FUNC( EVENT_OLYMPIC_GAWIBAWIBO_RESULT_REQ )
{
    EVENT_CHECK_OUT( EVENT_OLYMPIC_GAWIBAWIBO_RESULT_REQ );
    KEVENT_OLYMPIC_GAWIBAWIBO_RESULT_ACK kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    if ( kPacket_.m_nIncreasedData <= 0 ) {
        // 획득 포인트가 0일 경우 가위바위보 결과에 따라 오류코드 설정
        if ( kPacket_.m_nResult == KEVENT_OLYMPIC_GAWIBAWIBO_RESULT_ACK::ER_WIN ) {
            SET_ERROR( ERR_OLYMPIC_06 );
        }
        else {
            SET_ERROR( NET_OK );
        }
    }
    else {
        if ( false == OlympicUserData_merge( dwUID_, kPacket_.m_nIncreasedData, kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType ) ) {
            SET_ERR_GOTO( ERR_OLYMPIC_05, END_PROC );
        }

        switch ( kPacket_.m_nTeamType ) {
            case KOlympicSchedule::OGT_SOLO:
                SiKOlympicManager()->UpdateOlympicSoloPlayData( strLogin_, kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType, kPacket_.m_nMyAccumulateData + kPacket_.m_nIncreasedData );
                break;

            case KOlympicSchedule::OGT_TEAM:
                SiKOlympicManager()->UpdateOlympicTeamPlayData( kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType, kPacket_.m_nIncreasedData );
                break;

            default:
                START_LOG( cerr, L"올림픽 이벤트 플레이데이터 저장 중 팀 타입 오류: " << dwUID_ )
                    << BUILD_LOG( kPacket_.m_tmTodayDate )
                    << BUILD_LOG( kPacket_.m_nGameType )
                    << BUILD_LOG( kPacket_.m_nTeamType )
                    << END_LOG;
                break;
        }

        SET_ERROR( NET_OK );
    }

    kPacket.m_nResult = kPacket_.m_nResult;
    kPacket.m_nIncreasedData = kPacket_.m_nIncreasedData;
    kPacket.m_tmTodayDate = kPacket_.m_tmTodayDate;

    START_LOG( clog, L"올림픽 가위바위보 누적 포인트 갱신 요청: " << dwUID_ )
        << BUILD_LOG( kPacket_.m_nResult )
        << BUILD_LOG( kPacket_.m_nIncreasedData )
        << BUILD_LOG( kPacket_.m_tmTodayDate )
        << BUILD_LOG( kPacket_.m_nGameType )
        << BUILD_LOG( kPacket_.m_nTeamType )
        << END_LOG;

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_OLYMPIC_GAWIBAWIBO_RESULT_ACK );
}

INT_IMPL_ON_FUNC( EVENT_OLYMPIC_DICEPLAY_RESULT_REQ )
{
    EVENT_CHECK_OUT( EVENT_OLYMPIC_DICEPLAY_RESULT_REQ );
    KEVENT_OLYMPIC_DICEPLAY_RESULT_ACK kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    if ( kPacket_.m_nIncreasedData <= 0 ) {
        SET_ERROR( ERR_OLYMPIC_06 );
    }
    else {
        if ( false == OlympicUserData_merge( dwUID_, kPacket_.m_nIncreasedData, kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType ) ) {
            SET_ERR_GOTO( ERR_OLYMPIC_05, END_PROC );
        }

        switch ( kPacket_.m_nTeamType ) {
            case KOlympicSchedule::OGT_SOLO:
                SiKOlympicManager()->UpdateOlympicSoloPlayData( strLogin_, kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType, kPacket_.m_nMyAccumulateData + kPacket_.m_nIncreasedData );
                break;

            case KOlympicSchedule::OGT_TEAM:
                SiKOlympicManager()->UpdateOlympicTeamPlayData( kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType, kPacket_.m_nIncreasedData );
                break;

            default:
                START_LOG( cerr, L"올림픽 이벤트 플레이데이터 저장 중 팀 타입 오류: " << dwUID_ )
                    << BUILD_LOG( kPacket_.m_tmTodayDate )
                    << BUILD_LOG( kPacket_.m_nGameType )
                    << BUILD_LOG( kPacket_.m_nTeamType )
                    << END_LOG;
                break;
        }

        SET_ERROR( NET_OK );
    }

    kPacket.m_nFirstDiceNum = kPacket_.m_nFirstDiceNum;
    kPacket.m_nSecondDiceNum = kPacket_.m_nSecondDiceNum;
    kPacket.m_nIncreasedData = kPacket_.m_nIncreasedData;
    kPacket.m_tmTodayDate = kPacket_.m_tmTodayDate;

    START_LOG( clog, L"올림픽 주사위 누적 포인트 갱신 요청: " << dwUID_ )
        << BUILD_LOG( kPacket_.m_nIncreasedData )
        << BUILD_LOG( kPacket_.m_tmTodayDate )
        << BUILD_LOG( kPacket_.m_nGameType )
        << BUILD_LOG( kPacket_.m_nTeamType )
        << END_LOG;

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_OLYMPIC_DICEPLAY_RESULT_ACK );
}

INT_IMPL_ON_FUNC( EVENT_OLYMPIC_TREEDROP_RESULT_REQ )
{
    EVENT_CHECK_OUT( EVENT_OLYMPIC_TREEDROP_RESULT_REQ );
    KEVENT_OLYMPIC_TREEDROP_RESULT_ACK kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    if ( kPacket_.m_nIncreasedData <= 0 ) {
        SET_ERROR( ERR_OLYMPIC_06 );
    }
    else {
        if ( false == OlympicUserData_merge( dwUID_, kPacket_.m_nIncreasedData, kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType ) ) {
            SET_ERR_GOTO( ERR_OLYMPIC_05, END_PROC );
        }

        switch ( kPacket_.m_nTeamType ) {
            case KOlympicSchedule::OGT_SOLO:
                SiKOlympicManager()->UpdateOlympicSoloPlayData( strLogin_, kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType, kPacket_.m_nMyAccumulateData + kPacket_.m_nIncreasedData );
                break;

            case KOlympicSchedule::OGT_TEAM:
                SiKOlympicManager()->UpdateOlympicTeamPlayData( kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType, kPacket_.m_nIncreasedData );
                break;

            default:
                START_LOG( cerr, L"올림픽 이벤트 플레이데이터 저장 중 팀 타입 오류: " << dwUID_ )
                    << BUILD_LOG( kPacket_.m_tmTodayDate )
                    << BUILD_LOG( kPacket_.m_nGameType )
                    << BUILD_LOG( kPacket_.m_nTeamType )
                    << END_LOG;
                break;
        }

        SET_ERROR( NET_OK );
    }

    kPacket.m_nIncreasedData = kPacket_.m_nIncreasedData;
    kPacket.m_tmTodayDate = kPacket_.m_tmTodayDate;

    START_LOG( clog, L"올림픽 통나무깨기 누적 포인트 갱신 요청: " << dwUID_ )
        << BUILD_LOG( kPacket_.m_nIncreasedData )
        << BUILD_LOG( kPacket_.m_tmTodayDate )
        << BUILD_LOG( kPacket_.m_nGameType )
        << BUILD_LOG( kPacket_.m_nTeamType )
        << END_LOG;

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_OLYMPIC_TREEDROP_RESULT_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_OLYMPIC_JOIN_GAME_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_OLYMPIC_JOIN_GAME_REQ );
    KEVENT_OLYMPIC_JOIN_GAME_ACK kPacket;
    kPacket.m_nOK = -99;
    std::vector<KDropItemInfo>::iterator vit;
    SET_ERROR( ERR_UNKNOWN );

    for ( vit = kPacket_.m_vecReward.begin(); vit != kPacket_.m_vecReward.end(); ++vit ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_,
            cCharType_,
            vit->m_ItemID,
            vit->m_nDuration,
            vit->m_nPeriod,
            KItemManager::EWIT_OLYMPIC_EVENT_REWARD,
            kPacket.m_vecRewardItem ) ) {
                START_LOG( cerr, L"올림픽 게임 참여 보상 아이템 지급 실패 UID : " << dwUID_ )
                    << BUILD_LOG( vit->m_ItemID )
                    << BUILD_LOG( vit->m_nDuration )
                    << BUILD_LOG( vit->m_nPeriod )
                    << END_LOG;

                SET_ERR_GOTO( ERR_OLYMPIC_08, END_PROC );
        }
    }

    // 유저 TimeIndex정보 갱신.
    UpdateOlympicUserTimeIndex( dwUID_, kPacket_.m_tmTodayDate, kPacket_.m_nTimeIndex );

    kPacket.m_tmTodayDate = kPacket_.m_tmTodayDate;
    kPacket.m_nGameType = kPacket_.m_nGameType;
    kPacket.m_nTeamType = kPacket_.m_nTeamType;
    kPacket.m_nDefaultCount = kPacket_.m_nDefaultCount;
    kPacket.m_nRewardCount = kPacket_.m_nRewardCount;
    kPacket.m_nMyAccumulateData = kPacket_.m_nMyAccumulateData;
    kPacket.m_nOnePointMaxPoint = kPacket_.m_nOnePointMaxPoint;
    kPacket.m_nTotalMaxPoint = kPacket_.m_nTotalMaxPoint;
    kPacket.m_nTimeIndex = kPacket_.m_nTimeIndex;

    START_LOG( clog, L"올림픽 게임 참여 보상 아이템 지급 요청: " << dwUID_ )
        << BUILD_LOG( kPacket.m_vecRewardItem.size() )
        << END_LOG;

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_OLYMPIC_JOIN_GAME_ACK );
}

INT_IMPL_ON_FUNC( EVENT_OLYMPIC_BALLOON_RESULT_REQ )
{
    EVENT_CHECK_OUT( EVENT_OLYMPIC_BALLOON_RESULT_REQ );
    KEVENT_OLYMPIC_BALLOON_RESULT_ACK kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    if ( kPacket_.m_nIncreasedData <= 0 ) {
        SET_ERROR( ERR_OLYMPIC_06 );
    }
    else {
        if ( false == OlympicUserData_merge( dwUID_, kPacket_.m_nIncreasedData, kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType ) ) {
            SET_ERR_GOTO( ERR_OLYMPIC_05, END_PROC );
        }

        switch ( kPacket_.m_nTeamType ) {
            case KOlympicSchedule::OGT_SOLO:
                SiKOlympicManager()->UpdateOlympicSoloPlayData( strLogin_, kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType, kPacket_.m_nMyAccumulateData + kPacket_.m_nIncreasedData );
                break;

            case KOlympicSchedule::OGT_TEAM:
                SiKOlympicManager()->UpdateOlympicTeamPlayData( kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType, kPacket_.m_nIncreasedData );
                break;

            default:
                START_LOG( cerr, L"올림픽 이벤트 플레이데이터 저장 중 팀 타입 오류: " << dwUID_ )
                    << BUILD_LOG( kPacket_.m_tmTodayDate )
                    << BUILD_LOG( kPacket_.m_nGameType )
                    << BUILD_LOG( kPacket_.m_nTeamType )
                    << END_LOG;
                break;
        }

        SET_ERROR( NET_OK );
    }
    
    kPacket.m_nIncreasedData = kPacket_.m_nIncreasedData;
    kPacket.m_tmTodayDate = kPacket_.m_tmTodayDate;

    START_LOG( clog, L"올림픽 풍선터뜨리기 누적 포인트 갱신 요청: " << dwUID_ )
        << BUILD_LOG( kPacket_.m_nIncreasedData )
        << BUILD_LOG( kPacket_.m_tmTodayDate )
        << BUILD_LOG( kPacket_.m_nGameType )
        << BUILD_LOG( kPacket_.m_nTeamType )
        << END_LOG;

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_OLYMPIC_BALLOON_RESULT_ACK );
}

INT_IMPL_ON_FUNC( EVENT_OLYMPIC_BOSSGATE_RESULT_NOT )
{
    kPacket_.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    if ( false == OlympicUserData_merge( dwUID_, kPacket_.m_nIncreasedData, kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType ) ) {
        SET_ERR_GOTO( ERR_OLYMPIC_05, END_PROC );
    }

    switch ( kPacket_.m_nTeamType ) {
            case KOlympicSchedule::OGT_SOLO:
                SiKOlympicManager()->UpdateOlympicSoloPlayData( strLogin_, kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType, kPacket_.m_nMyAccumulateData + kPacket_.m_nIncreasedData );
                break;

            case KOlympicSchedule::OGT_TEAM:
                SiKOlympicManager()->UpdateOlympicTeamPlayData( kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType, kPacket_.m_nIncreasedData );
                break;

            default:
                START_LOG( cerr, L"올림픽 이벤트 플레이데이터 저장 중 팀 타입 오류: " << dwUID_ )
                    << BUILD_LOG( kPacket_.m_tmTodayDate )
                    << BUILD_LOG( kPacket_.m_nGameType )
                    << BUILD_LOG( kPacket_.m_nTeamType )
                    << END_LOG;
                break;
    }

    SET_ERROR( NET_OK );

    START_LOG( clog, L"올림픽 보스게이트 누적 포인트 갱신 요청: " << dwUID_ )
        << BUILD_LOG( kPacket_.m_nIncreasedData )
        << BUILD_LOG( kPacket_.m_tmTodayDate )
        << BUILD_LOG( kPacket_.m_nGameType )
        << BUILD_LOG( kPacket_.m_nTeamType )
        << END_LOG;

END_PROC:
    kPacket_.m_nOK = NetError::GetLastNetError();
    _QUEUING_ACK_TO_USER( EVENT_OLYMPIC_BOSSGATE_RESULT_NOT, kPacket_ );
}

INT_IMPL_ON_FUNC( DB_EVENT_OLYMPIC_INFO_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_OLYMPIC_INFO_REQ );
    SET_ERROR( ERR_UNKNOWN );
    KEVENT_OLYMPIC_INFO_ACK kPacket;
    kPacket.m_nOK = -99;

    // 유저 TimeIndex정보 가져오기.
    GetOlympicUserTimeIndex( dwUID_, kPacket.m_tmDBSaveDate, kPacket.m_nTimeIndex, kPacket.m_dwGrade );

    // 유저 누적 데이터 정보 가져오기.
    GetOlympicUserAccumulateData( dwUID_, kPacket_.m_tmTodayDate, kPacket.m_nMyAccumulateData );

    kPacket.m_tmTodayDate = kPacket_.m_tmTodayDate;

    SET_ERROR( NET_OK );

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_OLYMPIC_INFO_ACK );
}

INT_IMPL_ON_FUNC( EVENT_OLYMPIC_REWARD_REQ )
{
    EVENT_CHECK_OUT( EVENT_OLYMPIC_REWARD_REQ );
    KEVENT_OLYMPIC_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;
    std::vector<KDropItemInfo>::iterator vit;
    SET_ERROR( ERR_UNKNOWN );

    if ( false == UpdateOlympicUserGradeData( dwUID_, kPacket_.m_dwGrade ) ) {
        SET_ERR_GOTO( ERR_OLYMPIC_15, END_PROC );
    }

    for ( vit = kPacket_.m_vecReward.begin(); vit != kPacket_.m_vecReward.end(); ++vit ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_,
            cCharType_,
            vit->m_ItemID,
            vit->m_nDuration,
            vit->m_nPeriod,
            KItemManager::EWIT_OLYMPIC_EVENT_REWARD,
            kPacket.m_vecRewardItem ) ) {
                START_LOG( cerr, L"올림픽 게임 등급 보상 아이템 지급 실패 UID : " << dwUID_ )
                    << BUILD_LOG( kPacket_.m_dwGrade )
                    << BUILD_LOG( vit->m_ItemID )
                    << BUILD_LOG( vit->m_nDuration )
                    << BUILD_LOG( vit->m_nPeriod )
                    << END_LOG;

                SET_ERR_GOTO( ERR_OLYMPIC_08, END_PROC );
        }
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_OLYMPIC_REWARD_ACK );
}

_INT_IMPL_ON_FUNC( DB_EVENT_USER_DUNGEON_LOADING_INFO, KUserLoadingStat )
{
    LIF( InsertDungeonLoadingInfo( dwUID_, kPacket_ ) );
    START_LOG( clog, L"던전 로딩정보 통계 DB기록." )
        << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_USER_CHANGE_CHAR_STAT )
{
    LIF( InsertUserChangeCharCount( kPacket_ ) );
    START_LOG( clog, L"유저 캐릭터 변경 횟수 통계 DB기록.. size : " << kPacket_.size() )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_USER_MODE_CLEAR_STAT )
{
    LIF( InsertUserModeClearInfo( kPacket_ ) );
    START_LOG( clog, L"유저 모드 클리어 정보 통계 DB기록.. size : " << kPacket_.size() )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_ROOM_MONSTER_KILL_STAT )
{
    LIF( RoomMonsterKillInfo_insert( kPacket_ ) );
    START_LOG( clog, L"던전 몬스터 Kill 통계.. size : " )
        << BUILD_LOG( kPacket_.m_nModeID )
        << BUILD_LOG( kPacket_.m_nDifficult )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_nPartyCount )
        << BUILD_LOG( kPacket_.m_mapStageMonKillCount.size() )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ )
{
    KEVENT_CHAR_LEVEL_REWARD_EVENT_NOT kPacket;
    std::vector<KDropItemInfo>::iterator vit;
    std::set<char>::iterator sit;
    SET_ERROR( ERR_UNKNOWN );

    for ( vit = kPacket_.m_vecRewardInfo.begin(); vit != kPacket_.m_vecRewardInfo.end(); ++vit ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_,
            cCharType_,
            vit->m_ItemID,
            vit->m_nDuration,
            vit->m_nPeriod,
            KItemManager::EWIT_CHAR_LEVEL_EVENT_REWARD,
            kPacket.m_vecRewardItem,
            vit->m_cGradeID,
            -1,
            0 ) ) {
                START_LOG( cerr, L"특정 캐릭터 레벨 보상 아이템 지급 실패 UID : " << dwUID_ )
                    << BUILD_LOG( vit->m_ItemID )
                    << BUILD_LOG( vit->m_nDuration )
                    << BUILD_LOG( vit->m_nPeriod )
                    << BUILD_LOGc( vit->m_cGradeID )
                    << END_LOG;
        }
    }

    for( sit = kPacket_.m_setCharInfo.begin(); sit != kPacket_.m_setCharInfo.end(); ++sit ) {
        LIF( InsertUserCharLevelEventData( dwUID_, SiKGCHelper()->GetCharLevelEventVersion(), *sit ) );
    }

    kPacket.m_setCharInfo = kPacket_.m_setCharInfo;

    SET_ERROR( NET_OK );
    QUEUING_ACK_TO_USER( EVENT_CHAR_LEVEL_REWARD_EVENT_NOT );
}

INT_IMPL_ON_FUNC( DB_EVENT_SKILL_OPENKEY_USE_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_SKILL_OPENKEY_USE_REQ );
    KEVENT_SKILL_OPENKEY_USE_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_kUseItem.m_ItemUID, kPacket_.m_kUseItem.m_nInitCount, kPacket_.m_kUseItem.m_nCount ) ) {
        START_LOG( clog, L"스킬오픈키 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_kUseItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_kUseItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_kUseItem.m_nCount )
            << END_LOG;
    }

    if ( !UseCountItem( dwUID_, kPacket_.m_kUseItem.m_ItemUID, 1 ) ) { // 갯수 감소.
        START_LOG( cerr, L"스킬오픈키 아이템 개수 동기화 실패. Name : " << strLogin_ ) 
            << END_LOG;
        kPacket_.m_kUseItem.m_nCount = 0;
    } else {
        kPacket_.m_kUseItem.m_nCount -= 1;
        kPacket_.m_kUseItem.m_nCount = std::max<int>( kPacket_.m_kUseItem.m_nCount, 0 );
    }

    SSkillElement kSkill;
    if( SiKSkillManager()->GetSkill( kPacket_.m_nSkillID, kSkill ) ) {
        LIF( m_kHelper.SkillTreeID_insert( dwUID_, (int)kSkill.m_cCharType, (int)(unsigned char)kSkill.m_cPromotion, kSkill.m_nID ) );
    }

    kPacket.m_vecUseItem.push_back( kPacket_.m_kUseItem );
    kPacket.m_nSkillID = kPacket_.m_nSkillID;

    SET_ERROR( NET_OK );

    kPacket.m_nOK = NetError::GetLastNetError();

    START_LOG( clog, L"스킬 오픈키 사용" )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_nSkillID )
        << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_SKILL_OPENKEY_USE_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_ITEM_COMPOSE_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_ITEM_COMPOSE_REQ );
    KEVENT_ITEM_COMPOSE_ACK kPacket;
    std::vector<GCITEMUID>::iterator vit;
    SET_ERROR( ERR_UNKNOWN );

    // 보상 아이템 지급.
    if ( false == m_kHelper.InsertSingleItemJob( dwUID_,
        cCharType_,
        kPacket_.m_kDropItemInfo.m_ItemID,
        kPacket_.m_kDropItemInfo.m_nDuration,
        kPacket_.m_kDropItemInfo.m_nPeriod,
        KItemManager::EWIT_ITEM_COMPOSE_REWARD,
        kPacket.m_vecReward ) ) {
            START_LOG( cerr, L"아이템 합성 보상 지급 실패 UID : " << dwUID_ )
                << BUILD_LOG( kPacket_.m_kDropItemInfo.m_ItemID )
                << BUILD_LOG( kPacket_.m_kDropItemInfo.m_nDuration )
                << BUILD_LOG( kPacket_.m_kDropItemInfo.m_nPeriod )
                << END_LOG;
            SET_ERR_GOTO( ERR_ITEM_COMPOSE_06, END_PROC );
    }

    for( vit = kPacket_.m_vecMaterialUID.begin(); vit != kPacket_.m_vecMaterialUID.end(); ++vit ) {
        LIF( PureEraseItem( strLogin_, dwUID_, *vit, cCharType_ ) );
    }

    // 아이템 합성 로그 남기기.
    LIF( InsertItemComposeRewardLog( dwUID_, SiKItemCompose()->GetVersion(), kPacket_.m_vecMaterialID, kPacket_.m_kDropItemInfo.m_ItemID ) );

    kPacket.m_vecMaterialInfo = kPacket_.m_vecMaterialUID;
    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_ITEM_COMPOSE_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_SKILL_MISSION_INFO_NOT )
{
    std::map<DWORD, std::pair<int,DWORD>> mapDBUpdateSkillMissionInfo;
    SiKSkillManager()->GetDBUpdateSkillMissionInfo( mapDBUpdateSkillMissionInfo );
    LIF( InsertSkillMissionInfo( mapDBUpdateSkillMissionInfo ) );
    START_LOG( clog, L"미션 스킬 DB기록.. size : " << mapDBUpdateSkillMissionInfo.size() )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_GACHA_LOTTERY_ACTION_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_GACHA_LOTTERY_ACTION_REQ );
    KEVENT_GACHA_LOTTERY_ACTION_ACK kPacket;
    kPacket.m_nOK = -99;
    std::vector<KItem>::iterator vit;
    SET_ERROR( ERR_UNKNOWN );

    if ( false == m_kHelper.InsertSingleItemJob( dwUID_,
        cCharType_,
        kPacket_.m_kRewardItem.m_ItemID,
        kPacket_.m_kRewardItem.m_nDuration,
        kPacket_.m_kRewardItem.m_nPeriod,
        KItemManager::EWIT_GACHA_LOTTERY_REWARD,
        kPacket.m_vecRewardItem,
        -1,
        kPacket_.m_kRewardItem.m_cEquipLevel,
        0 ) ) {
            START_LOG( cerr, L"가챠 복권 보상 아이템 지급 실패 UID : " << dwUID_ )
                << BUILD_LOG( kPacket_.m_kRewardItem.m_ItemID )
                << BUILD_LOG( kPacket_.m_kRewardItem.m_nDuration )
                << BUILD_LOG( kPacket_.m_kRewardItem.m_nPeriod )
                << END_LOG;

            SET_ERR_GOTO( ERR_GACHA_LOTTERY_05, END_PROC );
    }

    for ( vit = kPacket.m_vecRewardItem.begin(); vit != kPacket.m_vecRewardItem.end(); ++vit ) {
        if( vit->m_sEquipLevel != 0 ) {
            bool bUpdateSuccess = true;
            LIF( bUpdateSuccess = bUpdateSuccess && m_kHelper.UpdateLevel(dwUID_, (int)vit->m_sEquipLevel, KItemLevelTable::IL_EQUIP_LV, *vit, cCharType_ ) );
        }
    }

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_kGachaLotteryItem.m_ItemUID, kPacket_.m_kGachaLotteryItem.m_nInitCount, kPacket_.m_kGachaLotteryItem.m_nCount ) ) {
        START_LOG( clog, L"가챠 복권 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_kGachaLotteryItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_kGachaLotteryItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_kGachaLotteryItem.m_nCount )
            << END_LOG;
    }

    if ( !UseCountItem( dwUID_, kPacket_.m_kGachaLotteryItem.m_ItemUID, 1 ) ) { // 갯수 감소.
        START_LOG( cerr, L"가챠 복권 아이템 개수 동기화 실패. Name : " << strLogin_ ) 
            << END_LOG;
        SET_ERROR( ERR_GACHA_LOTTERY_06 );
    }

    kPacket_.m_kGachaLotteryItem.m_nCount -= 1;
    kPacket_.m_kGachaLotteryItem.m_nCount = std::max<int>( kPacket_.m_kGachaLotteryItem.m_nCount, 0 );
    kPacket.m_kUseLotteryItem = kPacket_.m_kGachaLotteryItem;

    if ( IS_CORRECT( ERR_GACHA_LOTTERY_06 ) ) {
        kPacket.m_kUseLotteryItem.m_nCount = 0;
    }

    kPacket.m_cCharType = kPacket_.m_cCharType;
    kPacket.m_DisplayItemID = kPacket_.m_DisplayItemID;
    kPacket.m_dwRewardType = kPacket_.m_dwRewardType;

    DWORD dwVersion = SiKGachaLottery()->GetGachaLotteryVersion();
    // 가챠 복권 로그 작업
    LIF( UpdateGachaLotteryLog( dwUID_, dwVersion, kPacket_.m_cCharType, kPacket_.m_DisplayItemID, kPacket_.m_dwRewardType, kPacket.m_kUseLotteryItem.m_nCount ) );

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_dwRewardType )
        << BUILD_LOG( kPacket_.m_DisplayItemID )
        << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_GACHA_LOTTERY_ACTION_ACK );
}

_INT_IMPL_ON_FUNC( DB_EVENT_MONSTER_TYPE_STAT_INFO, KMonsterTypeStat )
{
    LIF( InsertMonsterTypeStatInfo( kPacket_ ) );
    START_LOG( clog, L"몬스터 타입 사용 카운트 정보 통계 DB기록." )
        << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_MATCH_CHAR_DATA_REQ )
{
    int nRatingPoint = 0;
    if( kPacket_.m_kELOUserData.m_nMatchTotalCount == 1 ) { // 첫판일 경우..,DB에 값이 없기 때문에 보정 필요.
        nRatingPoint = kPacket_.m_kELOUserData.m_nRatingPoint;
    } else {
        nRatingPoint = kPacket_.m_kELOUserData.m_nRatingPoint - kPacket_.m_kELOUserData.m_nInitRatingPoint;
    }

    LIF( UpdatePVPELOInfo( dwUID_, 
                           kPacket_.m_nSeasonYear,
                           kPacket_.m_nSeasonVersion,
                           kPacket_.m_cCharType,
                           nRatingPoint,
                           kPacket_.m_kELOUserData.m_nELOWin - kPacket_.m_kELOUserData.m_nInitELOWin,
                           kPacket_.m_kELOUserData.m_nELOLose - kPacket_.m_kELOUserData.m_nInitELOLose,
                           kPacket_.m_kELOUserData.m_nMatchTotalCount - kPacket_.m_kELOUserData.m_nInitMatchTotalCount,
                           kPacket_.m_kELOUserData.m_nLastWinLose,
                           kPacket_.m_kELOUserData.m_nConstantK,
                           kPacket_.m_kELOUserData.m_ucGrade ) );
    START_LOG( clog, L"캐릭터 ELO 정보 DB기록.. size : " )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_DEL_MATCH_REQ )
{
    LIF( InsertDelMatchInfo( kPacket_ ) );
    START_LOG( clog, L"매칭 취소한 유저의 정보 DB기록" )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_nModeInfo )
        << BUILD_LOG( kPacket_.m_dwDurationTime )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_nRP )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_MATCH_ELO_RESULT_REQ )
{
    LIF( InsertPVPELOStatInfo( kPacket_ ) );
    START_LOG( clog, L"대전 매칭 플레이한 캐릭터 정보 DB기록" )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_nSeasonYear )
        << BUILD_LOG( kPacket_.m_nSeasonVersion )
        << BUILD_LOG( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_dwLevel )
        << BUILD_LOG( kPacket_.m_nReceiveRP )
        << BUILD_LOG( kPacket_.m_nTotalRP )
        << BUILD_LOG( kPacket_.m_nModeInfo )
        << BUILD_LOG( kPacket_.m_nWinLose )
        << BUILD_LOG( kPacket_.m_dwMatchWaitTime )
        << BUILD_LOG( kPacket_.m_nMyTeamRP )
        << BUILD_LOG( kPacket_.m_nOtherTeamRP )
        << BUILD_LOG( kPacket_.m_nLoopCount )
        << BUILD_LOG( kPacket_.m_nTotalPartyNum )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_HERO_ITEM_UPGRADE_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_HERO_ITEM_UPGRADE_REQ );
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_HERO_ITEM_UPGRADE_ACK kPacket;
    std::vector<GCITEMUID>::iterator vit;

    kPacket.m_vecReward.clear();
    kPacket.m_vecDecreaseMaterial.clear();
    kPacket.m_vecDelMaterials.clear();

    // 업그레이드 결과 아이템 삽입!
    if ( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, kPacket_.m_rewardItem.m_ItemID, kPacket_.m_rewardItem.m_nDuration, kPacket_.m_rewardItem.m_nPeriod,
            KItemManager::EWIT_HEROITEM_UPGRADE, kPacket.m_vecReward, static_cast<int>( kPacket_.m_rewardItem.m_cGradeID ) ) ) {
        START_LOG( cerr, L"업그레이드 결과 아이템 삽입 실패 Name : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_rewardItem.m_ItemID )
            << BUILD_LOG( kPacket_.m_rewardItem.m_nDuration )
            << BUILD_LOG( kPacket_.m_rewardItem.m_nPeriod )
            << BUILD_LOGc( kPacket_.m_rewardItem.m_cGradeID )
            << END_LOG;
        SET_ERR_GOTO( ERR_HEROITEM_UPGRADE_06, END_PROC );
    }
    else {
        // 삽입에 성공했다면 재료로 사용 된 아이템 제거하자
        for( vit = kPacket_.m_vecDelMaterials.begin() ; vit != kPacket_.m_vecDelMaterials.end() ; ++vit ) {
            LIF( PureEraseItem( strLogin_, dwUID_, *vit, cCharType_ ) );
        }

        for( vit = kPacket_.m_vecStrengthItem.begin() ; vit != kPacket_.m_vecStrengthItem.end() ; ++vit ) { // 연계된 강화석 삭제.
            LIF( PureEraseItem( strLogin_, dwUID_, *vit, cCharType_ ) );
        }
    }

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_kItem.m_ItemUID, kPacket_.m_kItem.m_nInitCount, kPacket_.m_kItem.m_nCount ) ) {
        START_LOG( clog, L"재료로 사용 한 영웅주화 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_kItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_kItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_kItem.m_nCount )
            << END_LOG;
    }

    // 재료로 사용 된 아이템(영웅주화) 제거
    if ( !UseCountItem( dwUID_, kPacket_.m_kItem.m_ItemUID, kPacket_.m_nDecreaseCount ) ) { // 사용 갯수 감소.
        kPacket_.m_kItem.m_nCount = 0;
        START_LOG( cerr, L"재료로 사용 한 영웅주화 차감 실패" << strLogin_ )
            << BUILD_LOG( kPacket_.m_kItem.m_ItemID )
            << BUILD_LOG( kPacket_.m_kItem.m_nCount )
            << BUILD_LOG( kPacket_.m_nDecreaseCount )
            << END_LOG;

        SET_ERR_GOTO( ERR_HEROITEM_UPGRADE_07, END_PROC );
    } else {
        kPacket_.m_kItem.m_nCount -= kPacket_.m_nDecreaseCount;
        kPacket_.m_kItem.m_nCount = std::max<int>( kPacket_.m_kItem.m_nCount, 0 );
    }

    SET_ERROR( NET_OK );
END_PROC:
    if ( IS_CORRECT( ERR_HEROITEM_UPGRADE_07 ) ) {
        kPacket_.m_kItem.m_nCount = 0;
    } 

    kPacket.m_nOK = NetError::GetLastNetError();
    kPacket.m_vecDelMaterials = kPacket_.m_vecDelMaterials;
    kPacket.m_vecDecreaseMaterial.push_back( kPacket_.m_kItem );
    kPacket.m_vecStrengthItem = kPacket_.m_vecStrengthItem;

    START_LOG( clog, L"영웅 던전 아이템 업글 요청 DB 처리 결과" )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << END_LOG;

	QUEUING_ACK_TO_USER( EVENT_HERO_ITEM_UPGRADE_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_RITAS_CHRISTMAS_USER_INFO_REQ )
{
    KEVENT_RITAS_CHRISTMAS_USER_INFO_ACK kPacket;
    kPacket.m_nOK = -99;
    int nVersion = 0;

    SET_ERROR( ERR_UNKNOWN );

    nVersion = SiKRitasChristmas()->GetVersion();

    // 시작 코인 수, 시작 코인을 마지막으로 받은 시간
    Get_RitasChristmas_StartCoin_FromDB( nVersion, dwUID_, kPacket.m_nStartCoinCount, kPacket.m_tmStartCoinReceived );

    // 재도전 코인 수
    Get_RitasChristmas_ContinueCoin_FromDB( nVersion, dwUID_, kPacket.m_nContinueCoinCount );

    // 프리미엄 상자 수
    Get_RitasChristmas_PremiumRewardCount_FromDB( nVersion, dwUID_, kPacket.m_nPremiumRewardCount );

    // 오늘 내 최고 점수
    Get_RitasChristmas_TodayMaxPoint_FromDB( nVersion, dwUID_, kPacket.m_nMaxPoint );

    SET_ERROR( NET_OK );

//END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( nVersion )
        << BUILD_LOG( kPacket.m_nStartCoinCount )
        << BUILD_LOGtm( CTime( kPacket.m_tmStartCoinReceived ) )
        << BUILD_LOG( kPacket.m_nContinueCoinCount )
        << BUILD_LOG( kPacket.m_nPremiumRewardCount )
        << BUILD_LOG( kPacket.m_nMaxPoint )
        << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_RITAS_CHRISTMAS_USER_INFO_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_RITAS_CHRISTMAS_RANK_INFO_REQ )
{
    KEVENT_RITAS_CHRISTMAS_RANK_INFO_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_vecRankInfo.clear();

    SET_ERROR( ERR_UNKNOWN );

    Get_RitasChristmas_RankInfo_FromDB( dwUID_, kPacket.m_vecRankInfo );

    SET_ERROR( NET_OK );

//END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecRankInfo.size() )
        << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_RITAS_CHRISTMAS_RANK_INFO_ACK );
}

_INT_IMPL_ON_FUNC( DB_EVENT_RITAS_CHRISTMAS_UPDATE_CONTINUE_COIN_NOT, int )
{
    int nVersion = SiKRitasChristmas()->GetVersion();

    LIF( Update_RitasChristmas_ContinueCoin_ToDB( nVersion, dwUID_, kPacket_ ) );

    START_LOG( clog, L"유저의 재도전 코인 DB기록(Diff)" )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket_ )
        << END_LOG;
}

_INT_IMPL_ON_FUNC( DB_EVENT_RITAS_CHRISTMAS_UPDATE_RECORD_NOT, int )
{
    int nVersion = SiKRitasChristmas()->GetVersion();

    LIF( Update_RitasChristmas_NewRecord_ToDB( nVersion, dwUID_, kPacket_ ) );

    START_LOG( clog, L"유저의 오늘 최고 점수 DB기록(Diff)" )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket_ )
        << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_RITAS_CHRISTMAS_GET_REWARD_REQ )
{
    EVENT_CHECK_OUT( EVENT_RITAS_CHRISTMAS_GET_REWARD_REQ );

    KEVENT_RITAS_CHRISTMAS_GET_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_vecGetItems.clear();
    kPacket.m_nAfterSpecialRewardCount = kPacket_.m_nBeforeSpecialRewardCount;
    kPacket.m_nAfterPremiumRewardCount = kPacket_.m_nBeforePremiumRewardCount;
    kPacket.m_kPremiumKey = kPacket_.m_kPremiumKey;

    SET_ERROR( ERR_UNKNOWN );

    switch ( kPacket_.m_nOpenBoxType ) {
        case KEVENT_RITAS_CHRISTMAS_GET_REWARD_REQ::BT_SPECIAL:
            if ( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, kPacket_.m_kAddItem.m_ItemID, kPacket_.m_kAddItem.m_nDuration, kPacket_.m_kAddItem.m_nPeriod,
                KItemManager::EWIT_RITAS_CHRISTMAS_SPECIAL_REWARD, kPacket.m_vecGetItems ) ) {
                START_LOG( cerr, L"스페셜 보상 아이템 얻기 DB 작업 실패. Name : " << strLogin_ )
                    << BUILD_LOG( kPacket_.m_kAddItem.m_ItemID )
                    << BUILD_LOG( kPacket_.m_kAddItem.m_nDuration )
                    << BUILD_LOG( kPacket_.m_kAddItem.m_nPeriod ) << END_LOG;
                SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_02, END_PROC );
            }
            kPacket.m_nAfterSpecialRewardCount = std::max< int >( 0, kPacket_.m_nBeforeSpecialRewardCount - 1 );

            break;
        case KEVENT_RITAS_CHRISTMAS_GET_REWARD_REQ::BT_PREMIUM:
            if ( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, kPacket_.m_kAddItem.m_ItemID, kPacket_.m_kAddItem.m_nDuration, kPacket_.m_kAddItem.m_nPeriod,
                KItemManager::EWIT_RITAS_CHRISTMAS_PREMIUM_REWARD, kPacket.m_vecGetItems ) ) {
                START_LOG( cerr, L"프리미엄 보상 아이템 얻기 DB 작업 실패. Name : " << strLogin_ )
                    << BUILD_LOG( kPacket_.m_kAddItem.m_ItemID )
                    << BUILD_LOG( kPacket_.m_kAddItem.m_nDuration )
                    << BUILD_LOG( kPacket_.m_kAddItem.m_nPeriod ) << END_LOG;
                SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_02, END_PROC );
            }

            // 동기화전 InitCount, Count값 변동값 있는지 체크.
            if( !CheckItemDuration( dwUID_, kPacket_.m_kPremiumKey.m_ItemUID, kPacket_.m_kPremiumKey.m_nInitCount, kPacket_.m_kPremiumKey.m_nCount ) ) {
                START_LOG( clog, L"프리미엄 키 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
                    << BUILD_LOG( dwUID_ )
                    << BUILD_LOG( kPacket_.m_kPremiumKey.m_ItemUID )
                    << BUILD_LOG( kPacket_.m_kPremiumKey.m_nInitCount )
                    << BUILD_LOG( kPacket_.m_kPremiumKey.m_nCount )
                    << END_LOG;
            }

            if ( !UseCountItem( dwUID_, kPacket_.m_kPremiumKey.m_ItemUID, 1 ) ) {
                START_LOG( cerr, L"프리미엄 키 아이템 수량 동기화 실패. Name : " << strLogin_ ) << END_LOG;
                SET_ERROR( ERR_RITAS_CHRISTMAS_ERROR_03 );
            }
            kPacket.m_kPremiumKey.m_nCount = std::max< int >( 0, kPacket_.m_kPremiumKey.m_nCount - 1 );
            kPacket.m_nAfterPremiumRewardCount = std::max< int >( 0, kPacket_.m_nBeforePremiumRewardCount - 1 );

            break;
        default:
            // 유효하지 않은 상자 타입일 경우 보상을 지급하지 않는다
            kPacket.m_vecGetItems.clear();
            SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_04, END_PROC );

            break;
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nAfterSpecialRewardCount )
        << BUILD_LOG( kPacket.m_nAfterPremiumRewardCount )
        << BUILD_LOG( kPacket.m_kPremiumKey.m_ItemID )
        << BUILD_LOG( kPacket.m_kPremiumKey.m_ItemUID )
        << BUILD_LOG( kPacket.m_kPremiumKey.m_nCount )
        << BUILD_LOG( kPacket.m_vecGetItems.size() )
        << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_RITAS_CHRISTMAS_GET_REWARD_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_RITAS_CHRISTMAS_EXCHANGE_CONTINUE_COIN_REQ, KItem )
{
    EVENT_CHECK_OUT( EVENT_RITAS_CHRISTMAS_EXCHANGE_CONTINUE_COIN_REQ );

    KEVENT_RITAS_CHRISTMAS_EXCHANGE_CONTINUE_COIN_ACK kPacket;
    kPacket.m_nOK = -99;

    int nVersion = 0;
    int nExchangeConsumeCount = 0;
    int nExchangeResultCount = 0;

    SET_ERROR( ERR_UNKNOWN );

    nVersion = SiKRitasChristmas()->GetVersion();
    nExchangeConsumeCount = SiKRitasChristmas()->GetContinueCoinExchangeConsumeCount();
    nExchangeResultCount = SiKRitasChristmas()->GetContinueCoinExchangeResultCount();

    // 재도전 코인 교환 아이템 수량 확인(DB에서). 캐시아이템이니까 DB에서 직접 확인을 해준다
    if ( false == CheckUpdateCountItem( dwUID_, kPacket_.m_ItemID, kPacket_.m_cGradeID, nExchangeConsumeCount, cCharType_ ) ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_20, END_PROC );
    }

    if ( false == Update_RitasChristmas_ContinueCoin_ToDB( nVersion, dwUID_, nExchangeResultCount ) ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_21, END_PROC );
    }

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_ItemUID, kPacket_.m_nInitCount, kPacket_.m_nCount ) ) {
        START_LOG( clog, L"리타 크리스마스 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_ItemUID )
            << BUILD_LOG( kPacket_.m_nInitCount )
            << BUILD_LOG( kPacket_.m_nCount )
            << END_LOG;
    }

    if ( false == UseCountItem( dwUID_, kPacket_.m_ItemUID, nExchangeConsumeCount ) ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_22, END_PROC );
    }

    kPacket_.m_nCount -= nExchangeConsumeCount;

    // 여기서는 획득한 수. 이후 유저 스레드에서 누적값으로 교체한다
    kPacket.m_nAfterContinueCoinCount = nExchangeResultCount;
    kPacket.m_kConsumeItem = kPacket_;

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nAfterContinueCoinCount )
        << BUILD_LOG( kPacket.m_kConsumeItem.m_ItemID )
        << BUILD_LOG( kPacket.m_kConsumeItem.m_ItemUID )
        << BUILD_LOG( kPacket.m_kConsumeItem.m_nCount )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_nCount )
        << BUILD_LOG( kPacket_.m_nPeriod )
        << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_RITAS_CHRISTMAS_EXCHANGE_CONTINUE_COIN_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_UPDATE_MATCH_RANK_DATA_REQ )
{
    GetRPMatchRank();
    GetWinRecordMatchRank();
    GetExpMatchRank();
    GetRPMatchRankHOF();
    GetWinRecordMatchRankHOF();
    SiKRankManager()->BuildMatchRankData();
}

_INT_IMPL_ON_FUNC( DB_EVENT_MY_MATCH_RANK_INFO_REQ, std::set<char> )
{
    KMyMatchRankInfo kPacket;
    std::set<char>::iterator sit;
    kPacket_.insert( -1 ); // 전체 랭킹을 얻어 내기 위해서.
    for( sit = kPacket_.begin() ; sit != kPacket_.end() ; ++sit ) {
        GetMatchRankInfo( dwUID_, *sit, kPacket );
    }

    QUEUING_ACK_TO_USER( EVENT_MY_MATCH_RANK_INFO_NOT );

    START_LOG( clog, L"자신의 랭킹 정보 가져오기. Name : " << strLogin_ )
        << BUILD_LOG( kPacket.m_mapRPMatchRank.size() )
        << BUILD_LOG( kPacket.m_mapWinRecordRank.size() )
        << BUILD_LOG( kPacket.m_mapExpRank.size() ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_MATCH_RANK_SEARCH_REQ )
{
    EVENT_CHECK_OUT( EVENT_MATCH_RANK_SEARCH_REQ );
    KEVENT_MATCH_RANK_SEARCH_ACK kPacket;
    SET_ERROR( ERR_UNKNOWN );

    kPacket.m_cCharType = kPacket_.m_cCharType;
    kPacket.m_cRankType = kPacket_.m_cRankType;
    kPacket.m_kMatchRank.m_nRank = 0;
    kPacket.m_kExpRank.m_nRank = 0;

    switch( kPacket_.m_cRankType )
    {
    case KRankManager::MRT_RP:
        FindRPRankUseNickName( kPacket_.m_cCharType, kPacket_.m_strNickName, kPacket.m_kMatchRank );
        break;
    case KRankManager::MRT_WINRECORD:
        FindWinRecordRankUseNickName( kPacket_.m_cCharType, kPacket_.m_strNickName, kPacket.m_kMatchRank );
        break;
    case KRankManager::MRT_BEFORE_RP:
        FindRPRankUseNickNameHOF( kPacket_.m_cCharType, kPacket_.m_strNickName, kPacket.m_kMatchRank );
        break;
    case KRankManager::MRT_BEFORE_WINRECORD:
        FindWinRecordRankUseNickNameHOF( kPacket_.m_cCharType, kPacket_.m_strNickName, kPacket.m_kMatchRank );
        break;
    case KRankManager::MRT_EXP:
        FindExpRankUseNickName( kPacket_.m_cCharType, kPacket_.m_strNickName, kPacket.m_kExpRank );
        break;
    }

    if( kPacket.m_kMatchRank.m_nRank == 0 && kPacket.m_kExpRank.m_nRank == 0 ) {
        SET_ERROR( ERR_RANK_08 );
    } else {
        SET_ERROR( NET_OK );
    }

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_MATCH_RANK_SEARCH_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_UPDATE_ITEM_COUNT_NOT, std::vector< KItem > )
{
    BOOST_FOREACH( KItem& kItem, kPacket_ ) {
        if ( false == UseCountItem( dwUID_, kItem.m_ItemUID, kItem.m_nInitCount ) ) {
            START_LOG( cerr, L"아이템 수량 동기화 실패. Name : " << strLogin_ )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( kItem.m_ItemUID )
                << BUILD_LOG( kItem.m_nInitCount )
                << END_LOG;
        }
    }

    START_LOG( clog, L"아이템 수량 동기화 DB처리 완료" )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket_.size() )
        << END_LOG;

    _QUEUING_ACK_TO_USER( EVENT_UPDATE_ITEM_COUNT_NOT, kPacket_ );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_BUFF_DONATION_LOGIN_INFO_REQ )
{
    KEVENT_BUFF_DONATION_LOGIN_INFO_NOT kPacket;
    time_t tmDate;
    int nResult = 0;
    DWORD dwType;

    SiKBuffDonation()->GetType( dwType );
    SiKBuffDonation()->GetBeginTime( tmDate );
    // 내가 기부했는지 확인한다.
    nResult = GetTodayEventCount( dwUID_, dwType, tmDate );

    // DB에 값이 없으면 Result 값이 0
    kPacket.m_biTotalDonationPoint = nResult;

    QUEUING_ACK_TO_USER( EVENT_BUFF_DONATION_LOGIN_INFO_NOT );
}

INT_IMPL_ON_FUNC( DB_EVENT_BUFF_DONATION_REQ )
{
    // 아이템 지급 처리
    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( DB_EVENT_BUFF_DONATION_REQ );

    KEVENT_BUFF_DONATION_ACK kPacket;
    std::vector<KDropItemInfo>::iterator vit;
    time_t tmDate;
    DWORD dwType;

    // 기부 교환 아이템 수량 결정 후 지급
    int nItemCount = -1;
    for ( vit = kPacket_.m_vecReward.begin(); vit != kPacket_.m_vecReward.end(); ++vit ) {
        // 스크립트에서 0으로 설정했으면 기부 수량만큼 지급
        if ( 0 == vit->m_nDuration ) {
            nItemCount = kPacket_.m_nItemCount;
        }
        else {
            // 0이 아니라면 스크립트에 설정 된 수치로 지급 할 것이다
            nItemCount = vit->m_nDuration;
        }
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, nItemCount, vit->m_nPeriod, KItemManager::EWIT_VALENTINE_BUFF_DONATION, kPacket.m_vecRewardItem ) ) {
            START_LOG( cerr, L"교환해 주는 보상 아이템 삽입 실패 Name : " << strLogin_ )
                << BUILD_LOG( vit->m_ItemID )
                << BUILD_LOG( vit->m_nDuration )
                << BUILD_LOG( vit->m_nPeriod )
                << BUILD_LOG( nItemCount )
                << BUILD_LOG( kPacket_.m_nItemCount )
                << END_LOG;
            SET_ERR_GOTO( ERR_BUFF_DONATION_ERROR_05, END_PROC );
        }
    }

    // 기부 했다는 기록을 남기기 위해서 0으로 sp호출 (재접해도 버프 받을 수 있게 수정)
    if ( false == kPacket_.m_bFlag ) { // 기부한 내역이 없다면 DB에 기록
        SiKBuffDonation()->GetType( dwType );
        SiKBuffDonation()->GetBeginTime( tmDate );
        SetTodayEventDoneDiff( dwUID_, dwType, tmDate, 0 );
    }

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_kItem.m_ItemUID, kPacket_.m_kItem.m_nInitCount, kPacket_.m_kItem.m_nCount ) ) {
        START_LOG( clog, L"기부 사용 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_kItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_kItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_kItem.m_nCount )
            << END_LOG;
    }

    // 기부 아이템 차감 처리
    if ( !UseCountItem( dwUID_, kPacket_.m_kItem.m_ItemUID, kPacket_.m_nItemCount ) ) { // 사용 갯수 감소.
        START_LOG( cerr, L"기부에 사용 된 아이템 차감 실패" << strLogin_ )
            << BUILD_LOG( kPacket_.m_kItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_nItemCount )
            << END_LOG;

        SET_ERR_GOTO( ERR_BUFF_DONATION_ERROR_06, END_PROC );
    } else {
        kPacket_.m_kItem.m_nCount -= kPacket_.m_nItemCount;
        kPacket_.m_kItem.m_nCount = std::max<int>( kPacket_.m_kItem.m_nCount, 0 );
    }

    SET_ERROR( NET_OK );
END_PROC:
    if ( IS_CORRECT( ERR_BUFF_DONATION_ERROR_06 ) ) {
        kPacket_.m_kItem.m_nCount = 0;
    } 

    kPacket.m_nOK = NetError::GetLastNetError();
    kPacket.m_nItemCount = kPacket_.m_nItemCount;
    kPacket.m_kDonationItem = kPacket_.m_kItem; 


    START_LOG( clog, L"발렌타인 버프 기부 요청 DB 처리 결과" )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kDonationItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_nItemCount )
        << BUILD_LOG( kPacket.m_vecRewardItem.size() )
        << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_BUFF_DONATION_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_ACCUMULATE_ATTENDANCE_REQ )
{
    KEVENT_ACCUMULATE_ATTENDANCE_ACK kPacket;
    int nVersion = SiKAccumulateAttendanceManager()->GetVersion();
    time_t tmLastAttendanceTime;
    bool bResult = false;

    // 유저 출석 정보 (총 누적 출석 수) 얻기
    GetUserInfo_AccAttendance( dwUID_, nVersion, kPacket.m_nAccumulateCount, tmLastAttendanceTime );

    // 유저 출석 갱신(오늘 날짜로)
    CTime tmCurrent = CTime::GetCurrentTime();
    CTime tmTodayDate = CTime( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), 0, 0, 0 );
    time_t tmToday = KncUtil::TimeToInt( tmTodayDate );
    // 오늘 이미 출석은 했는지 확인 후 출석 요청
    if ( tmLastAttendanceTime < tmToday ) {
        bResult = UpdateAttendance_AccAttendance( dwUID_, nVersion, 1, tmToday );
        // 출석 후 출석일 캐싱 데이터 증가
        kPacket.m_nAccumulateCount++;
    }

    if ( false == bResult ) {
        START_LOG( cwarn, L"누적 출석에서 출석 도중 에러 발생(혹은 이미 출석 완료 날짜)" )
            << BUILD_LOG( bResult )
            << BUILD_LOG( kPacket.m_nAccumulateCount )
            << BUILD_LOGtm( CTime( tmLastAttendanceTime ) )
            << BUILD_LOGtm( CTime( tmToday ) )
            << END_LOG;
    }

    // 유저 누적 출석 보상 정보 얻기
    GetUserRewardStateList_AccAttendance( dwUID_, nVersion, kPacket.m_vecRewardCompleteDay );

    QUEUING_ACK_TO_USER( EVENT_ACCUMULATE_ATTENDANCE_ACK );

    START_LOG( clog, L"누적 출석 데이터 얻기/출석 완료. Name : " << strLogin_ )
        << BUILD_LOG( bResult )
        << BUILD_LOG( nVersion )
        << BUILD_LOGtm( CTime( tmLastAttendanceTime ) )
        << BUILD_LOGtm( CTime( tmToday ) )
        << BUILD_LOG( kPacket.m_nAccumulateCount )
        << BUILD_LOG( kPacket.m_vecRewardCompleteDay.size() )
        << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_ACCUMULATE_ATTENDANCE_REWARD_REQ )
{
    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( DB_EVENT_ACCUMULATE_ATTENDANCE_REWARD_REQ );

    KEVENT_ACCUMULATE_ATTENDANCE_REWARD_ACK kPacket;
    std::vector<KDropItemInfo>::iterator vit;
    int nVersion = SiKAccumulateAttendanceManager()->GetVersion();
    int nResult = -99;

    // 보상 아이템 지급
    for ( vit = kPacket_.m_vecReward.begin(); vit != kPacket_.m_vecReward.end(); ++vit ) {
        // 누적 출석 보상 지급 정보 갱신
        nResult = UpdateUserRewardStateList_AccAttendance( dwUID_, nVersion, kPacket_.m_nDayIndex, vit->m_ItemID );
        if ( 0 != nResult ) {
            START_LOG( cerr, L"누적 출석 보상 지급 정보 갱신 실패 Name : " << strLogin_ )
                << BUILD_LOG( nResult )
                << BUILD_LOG( kPacket_.m_nDayIndex )
                << BUILD_LOG( vit->m_ItemID )
                << END_LOG;
            SET_ERR_GOTO( ERR_ACCUMULATE_ATTENDANCE_ERROR_06, END_PROC );
        }

        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_ACCUMULATE_ATTENDANCE, kPacket.m_vecRewardItem ) ) {
            START_LOG( cerr, L"누적 출석 보상 아이템 삽입 실패 Name : " << strLogin_ )
                << BUILD_LOG( vit->m_ItemID )
                << BUILD_LOG( vit->m_nDuration )
                << BUILD_LOG( vit->m_nPeriod )
                << BUILD_LOG( kPacket_.m_vecReward.size() )
                << END_LOG;
            SET_ERR_GOTO( ERR_ACCUMULATE_ATTENDANCE_ERROR_05, END_PROC );
        }
    }

    // 누적 출석 보상 지급 정보 얻기
    GetUserRewardStateList_AccAttendance( dwUID_, nVersion, kPacket.m_vecRewardCompleteDay );


    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    START_LOG( clog, L"누적출석 보상 요청 DB 처리 결과" )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecRewardItem.size() )
        << BUILD_LOG( kPacket.m_vecRewardCompleteDay.size() )
        << BUILD_LOG( kPacket_.m_nDayIndex )
        << BUILD_LOG( kPacket_.m_vecReward.size() )
        << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_ACCUMULATE_ATTENDANCE_REWARD_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_PVP_BALANCE_STAT_REQ )
{
    int nUID = InsertPvPBalancePromotion( kPacket_.m_dwLoginUID, kPacket_.m_cCharType, kPacket_.m_cCharPromotion, kPacket_.m_dwLevel, kPacket_.m_nModeID, kPacket_.m_nPlayTime, kPacket_.m_nResult, kPacket_.m_nLifeCount );
    if( !kPacket_.m_kUserSkillStat.m_mapSkillUseCount.empty() ) {
        InsertPvPBalanceSkillUseCount( kPacket_.m_dwLoginUID, nUID, kPacket_.m_kUserSkillStat.m_mapSkillUseCount );
    }
    if( !kPacket_.m_kUserSkillStat.m_mapSkillSlotInfo.empty() ) {
        InsertPvPBalanceEquipSkillSlot( kPacket_.m_dwLoginUID, nUID, kPacket_.m_kUserSkillStat.m_mapSkillSlotInfo );
    }

    START_LOG( clog, L"PvP 대전 통계 DB 처리 결과" )
        << BUILD_LOG( kPacket_.m_dwLoginUID )
        << BUILD_LOG( kPacket_.m_kUserSkillStat.m_mapSkillUseCount.size() )
        << BUILD_LOG( kPacket_.m_kUserSkillStat.m_mapSkillSlotInfo.size() )
        << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_ERRAND_USER_INFO_REQ )
{
    int nVersion = 0;
    KErrandUserInfoDataStruct kPacket;

    // 패킷 초기화
    kPacket.m_nRemainTime = SiKErrandManager()->GetDelayTime();
    kPacket.m_bPlayable = false;
    kPacket.m_nTotalPlayCount = 0;
    kPacket.m_nTotalSuccessCount = 0;

    SiKErrandManager()->GetVersion( nVersion );
    GetUserData_ErrandEvent( dwUID_, nVersion, kPacket );
    GetUserReceivedSpecialRewardInfo_ErrandEvent( dwUID_, nVersion, kPacket.m_vecReceivedSpecialRewardInfo );

    QUEUING_ACK_TO_USER( EVENT_ERRAND_USER_INFO_ACK );

    START_LOG( clog, L"심부름왕 이벤트 유저 정보 가져오기. Name : " << strLogin_ )
        << BUILD_LOG( nVersion )
        << BUILD_LOG( kPacket.m_nRemainTime )
        << BUILD_LOG( kPacket.m_bPlayable )
        << BUILD_LOG( kPacket.m_nTotalPlayCount )
        << BUILD_LOG( kPacket.m_nTotalSuccessCount )
        << BUILD_LOG( kPacket.m_vecReceivedSpecialRewardInfo.size() )
        << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_ERRAND_REWARD_REQ )
{
    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( DB_EVENT_ERRAND_REWARD_REQ );

    VEC_REWARD::iterator vitReward;
    KEVENT_ERRAND_REWARD_ACK kPacket;
    int nVersion = 1;

    SiKErrandManager()->GetVersion( nVersion );

    // 최종 보상 지급
    for( vitReward = kPacket_.m_vecSpecialReward.begin(); vitReward != kPacket_.m_vecSpecialReward.end(); vitReward++ ) {
        if ( 0 != UpdateUserReceivedSpecialRewardInfo_ErrandEvent( dwUID_, nVersion, vitReward->m_ItemID, kPacket_.m_nTotalReceivedCount ) ) {
            SET_ERR_GOTO( ERR_ERRAND_ERROR_06, END_PROC );
        }

        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vitReward->m_ItemID, vitReward->m_nDuration, vitReward->m_nPeriod, KItemManager::EWIT_ERRAND_EVENT_REWARD, kPacket.m_vecSpecialRewardItem ) ) {
            START_LOG( cerr, L"심부름왕 최종 보상 아이템 삽입 실패 Name : " << strLogin_ )
                << BUILD_LOG( vitReward->m_ItemID )
                << BUILD_LOG( vitReward->m_nDuration )
                << BUILD_LOG( vitReward->m_nPeriod )
                << BUILD_LOG( kPacket.m_vecRewardItem.size() )
                << BUILD_LOG( kPacket_.m_vecSpecialReward.size() )
                << END_LOG;
            SET_ERR_GOTO( ERR_ERRAND_ERROR_03, END_PROC );
        }
    }

    // 일반 보상 지급
    for( vitReward = kPacket_.m_vecReward.begin(); vitReward != kPacket_.m_vecReward.end(); vitReward++ ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vitReward->m_ItemID, vitReward->m_nDuration, vitReward->m_nPeriod, KItemManager::EWIT_ERRAND_EVENT_REWARD, kPacket.m_vecRewardItem ) ) {
            START_LOG( cerr, L"심부름왕 보상 아이템 삽입 실패 Name : " << strLogin_ )
                << BUILD_LOG( vitReward->m_ItemID )
                << BUILD_LOG( vitReward->m_nDuration )
                << BUILD_LOG( vitReward->m_nPeriod )
                << BUILD_LOG( kPacket.m_vecRewardItem.size() )
                << BUILD_LOG( kPacket_.m_vecReward.size() )
                << END_LOG;
            SET_ERR_GOTO( ERR_ERRAND_ERROR_03, END_PROC );
        }
    }

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    START_LOG( clog, L"심부름왕 보상 DB 처리 결과" )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecRewardItem.size() )
        << BUILD_LOG( kPacket.m_vecSpecialRewardItem.size() )
        << BUILD_LOG( kPacket_.m_vecReward.size() )
        << BUILD_LOG( kPacket_.m_vecSpecialReward.size() )
        << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_ERRAND_REWARD_ACK );
}

INT_IMPL_ON_FUNC( EVENT_INIT_ITEM_ATTRIBUTE_REQ )
{
    EVENT_CHECK_OUT( EVENT_INIT_ITEM_ATTRIBUTE_REQ );
    KEVENT_INIT_ITEM_ATTRIBUTE_ACK kPacket;
    kPacket.m_nOK = -99;

    KItem kItem = kPacket_.m_kItem;
    kItem.m_vecAttribute.clear();

    bool bUpdateSuccess = true;
    for( int i = 0 ; i < (int)kPacket_.m_kItem.m_vecAttribute.size() ; ++i )
    {
        LIF( bUpdateSuccess = bUpdateSuccess && m_kHelper.InitAttribute(dwUID_, i, kItem ) );
    }

    if( bUpdateSuccess == false )
    {
        kPacket.m_nOK = NetError::ERR_ATTRIBUTE_INIT_04;
    }
    else
    {
        kPacket.m_nOK = NetError::NET_OK;
        kPacket.m_kItem = kItem;
        kPacket.m_ItemID = kPacket_.m_ItemID;
    }

    QUEUING_ACK_TO_USER_UID( EVENT_INIT_ITEM_ATTRIBUTE_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_DUNGEON_SUBJECT_REWARD_REQ, KEVENT_DUNGEON_SUBJECT_REWARD_ACK )
{
    EVENT_CHECK_OUT( EVENT_DUNGEON_SUBJECT_REWARD_REQ );

    std::vector< KItem > vecDropItem;
    vecDropItem.clear();

    BOOST_FOREACH( KItem& kItem, kPacket_.m_vecRewards ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, kItem.m_ItemID, kItem.m_nCount, kItem.m_nPeriod, KItemManager::EWIT_DUNGEON_SUBJECT_REWARD , vecDropItem, kItem.m_cGradeID ) ) {
            START_LOG( cerr, L"던전 과제 보상 아이템 추가 실패. UserUID : " << dwUID_ )
                << BUILD_LOG( kItem.m_ItemID )
                << BUILD_LOG( kItem.m_nCount )
                << BUILD_LOG( kItem.m_nPeriod )
                << BUILD_LOG( kItem.m_cGradeID )
                << END_LOG;

            kPacket_.m_vecRewards.clear();

            SET_ERR_GOTO( ERR_DUNGEON_SUBJECT_REWARD_ERROR_01, END_PROC );
        }
    }

    kPacket_.m_vecRewards.swap( vecDropItem );

    SET_ERROR( NET_OK );

END_PROC:
    kPacket_.m_nOK = NetError::GetLastNetError();

    START_LOG( clog, L"던전 과제 보상 DB 처리 결과" )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecRewards.size() )
        << BUILD_LOG( vecDropItem.size() )
        << END_LOG;

    _QUEUING_ACK_TO_USER( EVENT_DUNGEON_SUBJECT_REWARD_ACK, kPacket_ );
}

_INT_IMPL_ON_FUNC( EVENT_STAT_DUNGEON_SUBJECT_NOT, KDungeonSubjectStat )
{
    LIF( Update_DungeonSubject_Stat_ToDB( kPacket_ ) );

    START_LOG( clog, L"던전 달성 과제 통계 DB기록" )
        << BUILD_LOG( kPacket_.m_dwLoginUID )
        << BUILD_LOG( kPacket_.m_nGameMode )
        << BUILD_LOG( kPacket_.m_nDifficulty )
        << BUILD_LOG( kPacket_.m_nCharType )
        << BUILD_LOG( kPacket_.m_nCharLevel )
        << BUILD_LOG( kPacket_.m_nCharPromotion )
        << BUILD_LOG( kPacket_.m_mapSubjectClearInfo.size() )
        << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_GRADUATE_CHARACTER_USER_INFO_REQ )
{
    KEVENT_GRADUATE_CHARACTER_USER_INFO_NOT kPacket;

    BOOST_FOREACH( KItem& kItem, kPacket_.m_vecRewards ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, kItem.m_ItemID, kItem.m_nCount, kItem.m_nPeriod, KItemManager::EWIT_CHARACTER_GRADUATION_REWARD , kPacket.m_vecRewards, kItem.m_cGradeID ) ) {
            START_LOG( cerr, L"캐릭터 졸업 보상 아이템 추가 실패. UserUID : " << dwUID_ )
                << BUILD_LOG( kItem.m_ItemID )
                << BUILD_LOG( kItem.m_nCount )
                << BUILD_LOG( kItem.m_nPeriod )
                << BUILD_LOG( kItem.m_cGradeID )
                << END_LOG;

            SET_ERR_GOTO( ERR_GRADUATE_CHARACTER_REWARD_ERROR_01, END_PROC );
        }
    }

    // 클리어 모드 정보도 DB에 기록한다
    UpdateModeLevel( dwUID_, cCharType_, kPacket_.m_mapOldDifficulty, kPacket_.m_mapDifficulty );

    kPacket.m_nSendType                 = KEVENT_GRADUATE_CHARACTER_USER_INFO_NOT::EGCUI_ON_GRADUATE;
    kPacket.m_mapDifficulty             = kPacket_.m_mapDifficulty;
    kPacket.m_setGraduateCharacters     = kPacket_.m_setGraduateCharacters;
    kPacket.m_setNonGraduateCharacters  = kPacket_.m_setNonGraduateCharacters;

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    START_LOG( clog, L"캐릭터 졸업 보상 DB 처리 결과" )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nSendType )
        << BUILD_LOG( kPacket.m_mapDifficulty.size() )
        << BUILD_LOG( kPacket.m_setGraduateCharacters.size() )
        << BUILD_LOG( kPacket.m_setNonGraduateCharacters.size() )
        << BUILD_LOG( kPacket.m_vecRewards.size() )
        << BUILD_LOG( kPacket_.m_mapOldDifficulty.size() )
        << END_LOG;

    _QUEUING_ACK_TO_USER( EVENT_GRADUATE_CHARACTER_USER_INFO_NOT, kPacket );
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_SYSTEM_GUIDE_COMPLETE_INFO_REQ )
{
    KEVENT_SYSTEM_GUIDE_COMPLETE_INFO_NOT kPacket;
    
    GetSystemGuideUserData( dwUID_, kPacket.m_mapComplete );
    kPacket.m_nOK = 0;

    QUEUING_ACK_TO_USER( EVENT_SYSTEM_GUIDE_COMPLETE_INFO_NOT );
}

INT_IMPL_ON_FUNC( DB_EVENT_SYSTEM_GUIDE_ITEM_REQ )
{
    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( DB_EVENT_SYSTEM_GUIDE_ITEM_REQ );

    KEVENT_SYSTEM_GUIDE_ITEM_ACK kPacket;
    std::vector<KDropItemInfo>::iterator vit;
    int nResult = -99;

    // 아이템 지급
    for ( vit = kPacket_.m_vecProvide.begin(); vit != kPacket_.m_vecProvide.end(); ++vit ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_SYSTEM_GUIDE_PROVIDE,
            kPacket.m_vecProvideItem, vit->m_cGradeID ) ) {
            START_LOG( cerr, L"시스템 가이드 아이템 지급 실패 Name : " << strLogin_ )
                << BUILD_LOG( vit->m_ItemID )
                << BUILD_LOG( vit->m_nDuration )
                << BUILD_LOG( vit->m_nPeriod )
                << BUILD_LOG( vit->m_cGradeID )
                << BUILD_LOG( kPacket_.m_vecProvide.size() )
                << END_LOG;
            SET_ERR_GOTO( ERR_SYSTEM_GUIDE_ERROR_02, END_PROC );
        }
    }

    nResult = UpdateSystemGuideUserData( dwUID_, kPacket_.m_nGuideType, kPacket_.m_nProvideFlag, kPacket_.m_nCompleteFlag );
    if ( 0 != nResult ) {
        SET_ERR_GOTO( ERR_SYSTEM_GUIDE_ERROR_03, END_PROC );
    }
    kPacket.m_nGuideType = kPacket_.m_nGuideType;

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    START_LOG( clog, L"가이드 아이템 지급 DB 처리 결과" )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nGuideType )
        << BUILD_LOG( kPacket.m_vecProvideItem.size() )
        << BUILD_LOG( kPacket_.m_nGuideType )
        << BUILD_LOG( kPacket_.m_nProvideFlag )
        << BUILD_LOG( kPacket_.m_nCompleteFlag )
        << BUILD_LOG( kPacket_.m_vecProvide.size() )
        << END_LOG;


    QUEUING_ACK_TO_USER( EVENT_SYSTEM_GUIDE_ITEM_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_SYSTEM_GUIDE_COMPLETE_REQ )
{
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_SYSTEM_GUIDE_COMPLETE_ACK kPacket;
    int nResult = -99;

    nResult = UpdateSystemGuideUserData( dwUID_, kPacket_.m_nGuideType, kPacket_.m_kCompleteInfo.m_nProvideFlag, kPacket_.m_kCompleteInfo.m_nCompleteFlag );
    if ( 0 != nResult ) {
        SET_ERR_GOTO( ERR_SYSTEM_GUIDE_ERROR_03, END_PROC );
    }
    kPacket.m_nGuideType = kPacket_.m_nGuideType;
    kPacket.m_kCompleteInfo = kPacket_.m_kCompleteInfo;

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    QUEUING_ACK_TO_USER( EVENT_SYSTEM_GUIDE_COMPLETE_ACK );

    START_LOG( clog, L"가이드 완료 요청 DB 처리 결과" )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nGuideType )
        << BUILD_LOG( kPacket.m_kCompleteInfo.m_nProvideFlag )
        << BUILD_LOG( kPacket.m_kCompleteInfo.m_nCompleteFlag )
        << BUILD_LOG( kPacket_.m_nGuideType )
        << BUILD_LOG( kPacket_.m_kCompleteInfo.m_nProvideFlag )
        << BUILD_LOG( kPacket_.m_kCompleteInfo.m_nCompleteFlag )
        << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_NEW_USER_TUTORIAL_CLEAR_NOT, time_t )
{
    SetTodayEventDoneDiff( dwUID_, UET_NEWUSER_TUTORIAL, kPacket_, 1 );
}

INT_IMPL_ON_FUNC( EVENT_ATTRIBUTE_MIGRATION_SELECT_REQ )
{
    EVENT_CHECK_OUT( EVENT_ATTRIBUTE_MIGRATION_SELECT_REQ );
    KEVENT_ATTRIBUTE_MIGRATION_SELECT_ACK kPacket;
    kPacket.m_bEventEnable = kPacket_.m_bEventEnable;

    KItem kItem = kPacket_.m_kItem;
    kItem.m_vecAttribute.clear();

    bool bUpdateSuccess = true;
    for( int i = 0 ; i < (int)kPacket_.m_vecAddAttributeList.size() ; ++i ) {
        KAttributeInfo& kAttInfo = kPacket_.m_vecAddAttributeList[i];
        LIF( bUpdateSuccess = bUpdateSuccess && m_kHelper.UpdateAttribute(dwUID_, i, kAttInfo.m_cType, kAttInfo.m_fValue, kItem, cCharType_ ) );
    }

    if( bUpdateSuccess == false ) {
        kPacket.m_nOK = NetError::ERR_ATTRIBUTE_MIGRATION_07;
    }
    else {
        kPacket.m_nOK = NetError::NET_OK;
        kPacket.m_kItem = kItem;
    }

    if( !kPacket.m_bEventEnable ) {
        LIF( m_kHelper.UserItemAttributeResetCountMerge( dwUID_, kPacket_.m_ItemUID ) );
    }

    QUEUING_ACK_TO_USER_UID( EVENT_ATTRIBUTE_MIGRATION_SELECT_ACK );
}

INT_IMPL_ON_FUNC( EVENT_STAT_DUNGEON_LEAVE_NOT )
{
    LIF( InsertDungeonLeaveInfo( kPacket_ ) );
    START_LOG( clog, L"던전 게임플레이 정상종료,나간유저 통계." )
        << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_STAT_USER_PLAY_TIME_NOT )
{
    LIF( InsertUserPlayTime( kPacket_ ) );
    START_LOG( clog, L" 유저 던전,대전 playTime 통계." )
        << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_STAT_LEVEL_GP_INFO_NOT )
{
    LIF( InsertLevelGPInfo( kPacket_ ) );
    START_LOG( clog, L" 레벨별 GP 증가 감소량 통계." )
        << END_LOG;
}
INT_IMPL_ON_FUNC( DB_EVENT_ADD_SLOT_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_ADD_SLOT_REQ );
    KEVENT_ADD_SLOT_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_kUseItem = kPacket_.m_kUseItem;
    kPacket.m_nAddSlotCnt = kPacket_.m_nAddSlotCnt;

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket.m_kUseItem.m_ItemUID, kPacket.m_kUseItem.m_nInitCount, kPacket.m_kUseItem.m_nCount ) ) {
        START_LOG( clog, L"슬롯 추가 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket.m_kUseItem.m_ItemUID )
            << BUILD_LOG( kPacket.m_kUseItem.m_nInitCount )
            << BUILD_LOG( kPacket.m_kUseItem.m_nCount )
            << END_LOG;
    }

    if ( !UseCountItem( dwUID_, kPacket.m_kUseItem.m_ItemUID, 1 ) ) { // 갯수 감소.
        START_LOG( cerr, L"슬롯 추가 아이템 아이템 개수 동기화 실패. Name : " << strLogin_ ) 
            << END_LOG;
        SET_ERR_GOTO( ERR_ADD_SLOT_04, END_PROC );
    }

    LIF( UpdateUserSlotCnt( dwUID_, kPacket_.m_nAddSlotCnt ) );

    kPacket.m_kUseItem.m_nCount -= 1;
    kPacket.m_kUseItem.m_nCount = std::max<int>( kPacket.m_kUseItem.m_nCount, 0 );

END_PROC:
    if ( IS_CORRECT( ERR_ADD_SLOT_04 ) ) {
        kPacket.m_kUseItem.m_nCount = 0;
    } else {
        SET_ERROR( NET_OK );
    }

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_ADD_SLOT_ACK );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_nAddSlotCnt )
        << BUILD_LOG( kPacket_.m_kUseItem.m_ItemID )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_USER_ATTRIBUTE_MIGRATION_INFO_REQ )
{
    KEVENT_USER_ATTRIBUTE_MIGRATION_INFO_ACK kPacket;

    GetAttributeMigrationInfo( dwUID_, kPacket );

    QUEUING_ACK_TO_USER( EVENT_USER_ATTRIBUTE_MIGRATION_INFO_ACK );
}

INT_IMPL_ON_FUNC( EVENT_USER_CHAT_CHANGE_BY_ADMIN_REQ )
{
    LIF( m_kHelper.User_auth_update( kPacket_.m_dwUserUID, kPacket_.m_bChatEnable ) );
}


INT_IMPL_ON_FUNC_NOPARAM( EVENT_JUMPING_CHAR_INFO_NOT )
{
    KEVENT_JUMPING_CHAR_INFO_NOT kPacket;

    // 유저 강화 마이그레이션 여부 체크.
    if( SiKJumpingChar()->IsEventTerm() ) {
        time_t tmDate = SiKJumpingChar()->GetEventBeginTime();
        int nData = GetTodayEventCount( dwUID_, UET_JUMPING_CHARACTER, tmDate );
        if( nData == 0 ) {
            kPacket.m_bJumpingCharEnable = true;
        } else {
            START_LOG( clog, L"점핑 캐릭터 이미 하였음. Login " << strLogin_ ) << END_LOG;
            kPacket.m_bJumpingCharEnable = false;
        }
    } else {
        kPacket.m_bJumpingCharEnable = false;
    }

    QUEUING_ACK_TO_USER( EVENT_JUMPING_CHAR_INFO_NOT );
}

_INT_IMPL_ON_FUNC( EVENT_JUMPING_CHAR_REWARD_REQ, VECTOR_STRUCT_DROPITEM )
{
     EVENT_CHECK_OUT( EVENT_JUMPING_CHAR_REWARD_REQ );
     SET_ERROR( ERR_UNKNOWN );
 
     KEVENT_JUMPING_CHAR_REWARD_ACK kPacket;
     std::vector< KDropItemInfo >::const_iterator vit;
     std::vector<KItem> vecItem;
     vecItem.clear();
 
     // 보상 아이템 지급
     for ( vit = kPacket_.begin(); vit != kPacket_.end(); ++vit ) {
         if ( false == m_kHelper.InsertSingleItemJob( dwUID_, 
             cCharType_,
             vit->m_ItemID, 
             vit->m_nDuration, 
             vit->m_nPeriod, 
             KItemManager::EWIT_JUMPING_CHAR_REWARD, 
             vecItem ) ) {
                 START_LOG( cerr, L"점핑 캐릭터 보상 아이템 지급 실패 UID : " << dwUID_ )
                     << BUILD_LOG( vit->m_ItemID )
                     << BUILD_LOG( vit->m_nDuration )
                     << BUILD_LOG( vit->m_nPeriod ) << END_LOG;
                 continue;
         }
     }
 
     SET_ERROR( NET_OK );
 
     kPacket.m_nOK = NetError::GetLastNetError();
     kPacket.m_vecRewardItem = vecItem;
 
     // 점핑 캐릭터 보상 받았다는 정보 갱신.
     time_t tmDate = SiKJumpingChar()->GetEventBeginTime();
     LIF( SetTodayEventDoneDiff( dwUID_, UET_JUMPING_CHARACTER, tmDate, 1 ) );
 
     // 점핑 캐릭터 로그 기록.
     LIF( InsertJumpingCharLog( dwUID_, KJumpingChar::JT_REWARD, 0, 0, 0 ) );
 
     _LOG_SUCCESS( kPacket.m_nOK == 0, L"점핑 캐릭터 기본 보상 지급, UserUID: " << dwUID_ )
         << BUILD_LOG( kPacket.m_nOK )
         << BUILD_LOG( kPacket.m_vecRewardItem.size() )
         << END_LOG;
 
     QUEUING_ACK_TO_USER( EVENT_JUMPING_CHAR_REWARD_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_JUMPING_CHAR_REQ )
{
     EVENT_CHECK_OUT( DB_EVENT_JUMPING_CHAR_REQ );
     SET_ERROR( ERR_UNKNOWN );
 
     KEVENT_JUMPING_CHAR_ACK kPacket;
     std::vector< KDropItemInfo >::const_iterator cvit;
     std::vector<KItem> vecItem;
     vecItem.clear();
     std::vector<int>::iterator vit;
     std::set<int>::iterator sit;
     KMissionInfo kMissionInfo;
     int nResult = -1;
     std::vector<int>::iterator vitSkillGroup;
     std::set< int > setSealedChar;
     std::set< int >::iterator sitSealedChar;
 
     // 캐릭터 생성.
     if( kPacket_.m_nType == KEVENT_JUMPING_CHAR_ACK::JP_CREATE ) {
         if( !CreateNewChar( dwUID_, strLogin_, kPacket_.m_kCharacterInfo.m_cCharType, kPacket_.m_kCharacterInfo ) ) {
             SET_ERR_GOTO( ERR_JUMPING_CHAR_07, END_PROC );
         }
     }
 
     kPacket.m_kCharacterInfo = kPacket_.m_kCharacterInfo;
     // 캐릭터 레벨 세팅
     DWORD dwJumpingLevel = SiKJumpingChar()->GetJumpingCharLevel();
     char cJumpingPromotion = 0;
     SiKJumpingChar()->GetJumpingCharPromtion( kPacket.m_kCharacterInfo.m_cCharType, cJumpingPromotion );
     SetJumpingCharLevelPromotion( dwJumpingLevel, cJumpingPromotion, kPacket.m_kCharacterInfo );
     SetJumpingCharInfo( strLogin_, dwUID_, kPacket.m_kCharacterInfo.m_cCharType, kPacket.m_kCharacterInfo );
     // 전직에 대한 DB처리 진행.
     LIF( UpdateCharPromotion( strLogin_, kPacket.m_kCharacterInfo.m_cCharType, kPacket.m_kCharacterInfo.m_cPromotion, nResult ) );
     SetJumpingCharInitLevel( kPacket.m_kCharacterInfo );
     SiKSkillManager()->SetNewCharSkillInfo( kPacket.m_kCharacterInfo.m_cCharType, kPacket.m_kCharacterInfo.m_dwLevel, kPacket.m_kCharacterInfo.m_kSkillInfo.m_nLvSPPoint, kPacket.m_kCharacterInfo.m_kSkillInfo.m_nMaxSPPoint );
 
     // 미션 완료 처리.
     for( vit = kPacket_.m_vecCompleteMissionInfo.begin(); vit != kPacket_.m_vecCompleteMissionInfo.end(); ++vit ) {
         if( !SiKMissionManager()->GetMissionInfo( kMissionInfo, *vit ) ) {
             START_LOG( cerr, L"점핑 캐릭터 미션 정보 가져오기 실패 UID : " << dwUID_ )
                 << BUILD_LOG( *vit )
                 << END_LOG;
             continue;
         }
 
         sit = kPacket_.m_setDeleteMissionInfo.find( *vit );
         if( sit != kPacket_.m_setDeleteMissionInfo.end() ) {
             LIF( CompleteMissionInfo( dwUID_, cCharType_, kMissionInfo.m_dwMissionID, kMissionInfo.m_bUnique, KMissionInfo::DEL_TYPE_USER_COMPLETE, nResult ) );
         } else {
             LIF( PromotionCompleteMission( dwUID_, cCharType_, kMissionInfo.m_dwMissionID, nResult ) );
         }
     }
 
     // 미션 등록 처리
     for( vit = kPacket_.m_vecRegisterMissionInfo.begin(); vit != kPacket_.m_vecRegisterMissionInfo.end(); ++vit ) {
         KMissionData kMissionData;
         int nResultRegistMission;
         if( RegistMission( dwUID_, cCharType_, *vit, kMissionData, nResultRegistMission ) ) {
             kPacket.m_vecMissionSlot.push_back( kMissionData );
         }
     }
 
     // 보상 아이템 지급
     for ( cvit = kPacket_.m_vecReward.begin(); cvit != kPacket_.m_vecReward.end(); ++cvit ) {
         if ( false == m_kHelper.InsertSingleItemJob( dwUID_,
             cCharType_,
             cvit->m_ItemID, 
             cvit->m_nDuration, 
             cvit->m_nPeriod, 
             KItemManager::EWIT_JUMPING_CHAR_REWARD, 
             vecItem ) ) {
                 START_LOG( cerr, L"점핑 캐릭터 보상 아이템 지급 실패 UID : " << dwUID_ )
                     << BUILD_LOG( cvit->m_ItemID )
                     << BUILD_LOG( cvit->m_nDuration )
                     << BUILD_LOG( cvit->m_nPeriod ) << END_LOG;
                 continue;
         }
     }
 
     // 스킬 정보 기록.
     for( vit = kPacket_.m_vecRewardSkillInfo.begin(); vit != kPacket_.m_vecRewardSkillInfo.end(); ++vit ) {
         SSkillElement kSkill;
         if( !SiKSkillManager()->GetSkill( *vit, kSkill ) ) {
             continue;
         }
 
         LIF( m_kHelper.SkillTreeID_insert( dwUID_, (int)kSkill.m_cCharType, (int)(unsigned char)kSkill.m_cPromotion, kSkill.m_nID ) );
     }
 
     // 스킬 그룹정보 오픈
     for( vitSkillGroup = kPacket_.m_vecSkillGroupInfo.begin(); vitSkillGroup != kPacket_.m_vecSkillGroupInfo.end(); ++vitSkillGroup ) {
         LIF( AddUnLockGroupID( dwUID_, kPacket_.m_kCharacterInfo.m_cCharType, *vitSkillGroup ) );
     }
 
     // 봉인 캐릭터 해제 처리
     CharacterSeal_Select( dwUID_, setSealedChar );
     if( !setSealedChar.empty() ) {
         // 봉인된 캐릭터를 점핑하는지 체크.
         sitSealedChar = setSealedChar.find( kPacket_.m_kCharacterInfo.m_cCharType );
         if( sitSealedChar != setSealedChar.end() ) {
             // 봉인 캐릭터 해제.
             int nCharType = static_cast<int>( kPacket_.m_kCharacterInfo.m_cCharType );
             CharacterSeal_Merge( dwUID_, nCharType, false );
         }
     }
 
     // 점핑 캐릭터 보상 받았다는 정보 갱신.
     time_t tmDate = SiKJumpingChar()->GetEventBeginTime();
     LIF( SetTodayEventDoneDiff( dwUID_, UET_JUMPING_CHARACTER, tmDate, 1 ) );
 
     // 점핑 캐릭터 로그 기록.
     LIF( InsertJumpingCharLog( dwUID_, kPacket_.m_nType, kPacket_.m_kCharacterInfo.m_cCharType, kPacket_.m_kCharacterInfoLog.m_dwLevel, kPacket.m_kCharacterInfo.m_dwLevel ) );
 
     SET_ERROR( NET_OK );
 
 END_PROC:
     kPacket.m_nOK = NetError::GetLastNetError();
     kPacket.m_vecRewardItem = vecItem;
     kPacket.m_nType = kPacket_.m_nType;
     kPacket.m_vecCompleteMissionInfo = kPacket_.m_vecCompleteMissionInfo;
     kPacket.m_setDeleteMissionInfo = kPacket_.m_setDeleteMissionInfo;
     kPacket.m_vecUnLockSkillGroup = kPacket_.m_vecSkillGroupInfo;
     kPacket.m_vecRewardSkillInfo = kPacket_.m_vecRewardSkillInfo;
 
     _LOG_SUCCESS( kPacket.m_nOK == 0, L"점핑 캐릭터 기본 보상 지급, UserUID: " << dwUID_ )
         << BUILD_LOG( kPacket.m_nOK )
         << BUILD_LOG( kPacket.m_vecRewardItem.size() )
         << END_LOG;
 
     QUEUING_ACK_TO_USER( EVENT_JUMPING_CHAR_ACK );
}

INT_IMPL_ON_FUNC( EVENT_INSERT_DUNGEON_HACK_NOT )
{
    /*
    CREATE proc [dbo].[sup_user_hack_info_insert_20130405]
    @iLoginUID_ [int],
    @iModeID_   [int],
    @iType_     [tinyint]


    20130405. jgiusb. ModeID 추가

    sutamtwo
    작성일 : 2007년 7월 31일
    사용 : User의 UID와 Type 를 받아 dbo.UserHackInfo에 저장한다
    0 : 성공
    -1 : 유저 정보 없음 
    -11 : 트랜잭션 오류
    */

    int nOK = -99;
    _DBJIF( m_kODBCStat.Query( L"{ call dbo.sup_user_hack_info_insert_20130405 ( %d, %d, %d ) }",
        dwUID_, kPacket_.m_nModeID, kPacket_.m_nHackType ), return, (&m_kODBCStat) );

    if( m_kODBCStat.BeginFetch() ) {
        VERIFY_QUERY( m_kODBCStat >> nOK );
    }
    m_kODBCStat.EndFetch();

    LOG_SUCCESS( nOK == 0 ) // 기록 남기다 잘못되면 로그를 남기자..
        << BUILD_LOG( nOK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket_.m_nHackType )
        << BUILD_LOG( kPacket_.m_nModeID )
        << BUILD_LOG( m_kODBCStat.m_strLastQuery )
      /*  << BUILD_LOG( kPacket_.m_dwPlayTime )
        << BUILD_LOG( kPacket_.m_mapRelaySendCount.size() )*/
        << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_SKILL_TRAINING_REQ, int )
{
    SSkillElement kSkill;
    if( SiKSkillManager()->GetSkill( kPacket_, kSkill ) ) {
        LIF( m_kHelper.SkillTreeID_insert( dwUID_, (int)kSkill.m_cCharType, (int)(unsigned char)kSkill.m_cPromotion, kSkill.m_nID ) );
    }
}

INT_IMPL_ON_FUNC( EVENT_USER_RELAY_SEND_COUNT_REQ )
{
    LIF( InsertUserRelaySendCount( dwUID_, kPacket_ ) );
    START_LOG( clog, L"릴레이 서버 SendCount 로그 남기기 strName" << strLogin_ )
        << BUILD_LOG( kPacket_.m_nModeID )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_dwPlayTime )
        << BUILD_LOG( kPacket_.m_mapRelaySendCount.size() )
        << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_SYSTEM_RESOURCE_INFO )
{
    LIF( InsertProcessCPUMemoryInfo( kPacket_.m_dwServerGroup, kPacket_.m_dwServerUniqueNum, kPacket_.m_nCPUUsage, kPacket_.m_kProcessMemoryInfo ) );
    LIF( InsertSystemHDDInfo( kPacket_.m_dwServerGroup, kPacket_.m_dwServerUniqueNum, kPacket_.m_vecSystemHDDInfo ) );
    LIF( InsertSystemNetworkInfo( kPacket_.m_dwServerGroup, kPacket_.m_dwServerUniqueNum, kPacket_.m_mapNetworkInfo ) );
}

INT_IMPL_ON_FUNC( DB_EVENT_COORDI_COMPOSE_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_COORDI_COMPOSE_REQ );
    KEVENT_COORDI_COMPOSE_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    GCITEMID DesignItemID;
    if( kPacket_.m_kDesignItem.m_DesignCoordiID > 0 ) {
        DesignItemID = kPacket_.m_kDesignItem.m_DesignCoordiID;
    } else {
        DesignItemID = kPacket_.m_kDesignItem.m_ItemID;
    }

    // 코디 합성정보 갱신.
    if( !m_kHelper.SetCooriComposeInfo( dwUID_, kPacket_.m_kAbilityItem.m_ItemUID, kPacket_.m_kDesignItem.m_ItemUID, DesignItemID ) ) {
        START_LOG( cerr, L" 코디 합성 정보 DB 갱신시 에러 Name : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_kAbilityItem.m_ItemID )
            << BUILD_LOG( kPacket_.m_kAbilityItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_kDesignItem.m_ItemID )
            << BUILD_LOG( kPacket_.m_kDesignItem.m_ItemUID )
            << END_LOG;
        SET_ERR_GOTO( ERR_COORDI_COMPOSE_06, END_PROC );
    }

    // 외형 코디 아이템 삭제.
    LIF( SetTypeEraseItem( strLogin_, dwUID_, kPacket_.m_kDesignItem.m_ItemUID, KItemManager::EDEL_DEL_COORDI_COMPOSE, cCharType_ ) );

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_kCoordiComposeItem.m_ItemUID, kPacket_.m_kCoordiComposeItem.m_nInitCount, kPacket_.m_kCoordiComposeItem.m_nCount ) ) {
        START_LOG( clog, L"코디 합성 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_kCoordiComposeItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_kCoordiComposeItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_kCoordiComposeItem.m_nCount )
            << END_LOG;
    }

    // 코디 합성 아이템 삭제.
    if ( !UseCountItem( dwUID_, kPacket_.m_kCoordiComposeItem.m_ItemUID, 1 ) ) {
        START_LOG( cerr, L"코디 합성 아이템 개수 동기화 실패. Name : " << strLogin_ )
            << END_LOG;
        SET_ERROR( ERR_COORDI_COMPOSE_07 );
    }

    kPacket_.m_kCoordiComposeItem.m_nCount -= 1;
    kPacket_.m_kCoordiComposeItem.m_nCount = std::max<int>( kPacket_.m_kCoordiComposeItem.m_nCount, 0 );
    kPacket.m_kCoordiComposeItem = kPacket_.m_kCoordiComposeItem;

    if ( IS_CORRECT( ERR_COORDI_COMPOSE_07 ) ) {
        kPacket.m_kCoordiComposeItem.m_nCount = 0;
    }
    kPacket_.m_kAbilityItem.m_DesignCoordiID = DesignItemID; // 코디 합성 정보 갱신.

    kPacket.m_kAbilityItem = kPacket_.m_kAbilityItem;
    kPacket.m_kDesignItem = kPacket_.m_kDesignItem;

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    _QUEUING_ACK_TO_USER( EVENT_COORDI_COMPOSE_ACK, kPacket );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kAbilityItem.m_ItemID )
        << BUILD_LOG( kPacket.m_kAbilityItem.m_ItemUID )
        << BUILD_LOG( kPacket.m_kDesignItem.m_ItemID )
        << BUILD_LOG( kPacket.m_kDesignItem.m_ItemUID ) << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_VIRTUAL_DEPOT_ITEM_LIST_REQ )
{
    KEVENT_VIRTUAL_DEPOT_ITEM_LIST_NOT kPacket;

    GetVirtualDepotItemInfo( dwUID_, kPacket );

    QUEUING_ACK_TO_USER( EVENT_VIRTUAL_DEPOT_ITEM_LIST_NOT );

    START_LOG( clog, L"가상창고 아이템 얻은 로그 남기기 strName" << strLogin_ )
        << BUILD_LOG( kPacket.size() )
        << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_MOVE_ITEM_TO_INVENTORY_FROM_VIRTUAL_DEPOT_REQ )
{
    KEVENT_MOVE_ITEM_TO_INVENTORY_FROM_VIRTUAL_DEPOT_ACK kPacket;
    std::map< int, std::vector<KItem> >::iterator mitCharMoveItem;
    std::vector<KItem>::iterator vitMoveItem;
    const int nLastPlayCharacter = GetLastPlayCharacter( dwUID_ );

    EVENT_CHECK_OUT( EVENT_MOVE_ITEM_TO_INVENTORY_FROM_VIRTUAL_DEPOT_REQ );

    // 캐릭터별로 가상창고에서 인벤토리로 옮겨준다
    for ( mitCharMoveItem = kPacket_.begin(); mitCharMoveItem != kPacket_.end(); mitCharMoveItem++ ) {
        for ( vitMoveItem = mitCharMoveItem->second.begin(); vitMoveItem != mitCharMoveItem->second.end(); vitMoveItem++ ) {
            // 수량 아이템 처리
            if ( KItem::UNLIMITED_ITEM != vitMoveItem->m_nCount && 0 < vitMoveItem->m_nCount ) {
                // 가상창고의 수량 갱신
                int nResult = UpdateCountItemAtVirtualDepot( dwUID_, vitMoveItem->m_ItemUID, -(vitMoveItem->m_nCount), KItemManager::EDEL_DEL_VIRTUAL_DEPOT );
                if ( 0 != nResult ) {
                    kPacket.m_nFailItemCount++;
                    continue;
                }

                // 캐릭터 인벤에 넣어준다
                if ( nLastPlayCharacter == mitCharMoveItem->first ) {
                    m_kHelper.InsertSingleItemJob( dwUID_, mitCharMoveItem->first, vitMoveItem->m_ItemID, vitMoveItem->m_nCount, vitMoveItem->m_nPeriod,
                        KItemManager::EWIT_VIRTUAL_DEPOT_TO_INVEN, kPacket.m_vecUpdateItem );
                }
                else { // LastPlay캐릭터만 데이터 넣기 위해서 추가
                    m_kHelper.InsertSingleItemJob( dwUID_, mitCharMoveItem->first, vitMoveItem->m_ItemID, vitMoveItem->m_nCount, vitMoveItem->m_nPeriod,
                        KItemManager::EWIT_VIRTUAL_DEPOT_TO_INVEN, std::vector<KItem>() );
                }
            }
            else {
                // 캐릭터 타입을 입력 해준다 (del state 업데이트 이전에 캐릭터 타입 지정 안해주면 -3 에러 남)
                if ( 0 != m_kHelper.UIGAUserItemCharacter_insert( dwUID_, mitCharMoveItem->first, vitMoveItem->m_ItemUID ) ) {
                    kPacket.m_nFailItemCount++;
                    continue;
                }

                // 수량 이외에는 가상창고 -> 인벤으로 delstate 변경
                if ( false == m_kHelper.UserItem_DelState_Update( dwUID_, vitMoveItem->m_ItemUID, vitMoveItem->m_cInvenType, mitCharMoveItem->first ) ) {
                    kPacket.m_nFailItemCount++;
                    continue;
                }

                // 마지막으로 플레이한 캐릭터의 아이템들은 인벤토리 갱신을 위해서 따로 모음
                if ( nLastPlayCharacter == mitCharMoveItem->first ) {
                    kPacket.m_vecUpdateItem.push_back( *vitMoveItem );
                }
            }
        }
    }

    // 가상창고 전체 정보 새로 얻기
    GetVirtualDepotItemInfo( dwUID_, kPacket.m_vecVirtualDepot );

    QUEUING_ACK_TO_USER( EVENT_MOVE_ITEM_TO_INVENTORY_FROM_VIRTUAL_DEPOT_ACK );

    START_LOG( clog, L"가상창고 -> 인벤토리 옮기기 DB작업 완료" )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket_.size() )
        << BUILD_LOG( kPacket.m_vecUpdateItem.size() )
        << BUILD_LOG( kPacket.m_vecVirtualDepot.size() )
        << BUILD_LOG( kPacket.m_nFailItemCount )
        << END_LOG;
}

_INT_IMPL_ON_FUNC( DB_EVENT_CLIENT_NETWORK_REPORT_NOT, MAP_INT_INT )
{
    // 서버 IP정보 받아오기.
    std::wstring strIP = KncUtil::toWideString( SiKGameServer()->GetIP() );

    MAP_INT_INT::iterator mit;
    for ( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit ) {
        m_kHelper.InsertClientNetworkStat( strIP, mit->first, mit->second );
    }
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_SOSCIAL_COMMERCE_BUY_COUNT_INFO )
{
    // 소셜커머스 아이템 별 현재 구매 수량 정보 얻는다
    std::vector<PAIR_DWORD_INT> vecPrBuyCount;
    vecPrBuyCount.clear();

    GetAllSellCount_SocialCommerce( vecPrBuyCount );
    SiKSocialCommerce()->UpdateAllSellCountFromDB( vecPrBuyCount );
}

INT_IMPL_ON_FUNC( DB_EVENT_SOSCIAL_COMMERCE_BUY_REQ )
{
    KEVENT_SOSCIAL_COMMERCE_INFO_UPDATE_NOT kPacket;
    // 캐시 구매는 하나로 처리 되고 있어서 판매 수량 1개로 매직넘버 박아 놓음
    // 기존 구매로직 변경 되면 여기도 바뀌어야 함
    int nSellCountDiff = 1;
    int nResult;

    nResult = UpdateSellCount_SocialCommerce( dwUID_, kPacket_.m_dwEventUID, nSellCountDiff );
    if ( 0 != nResult ) {
        START_LOG( cerr, L"[소셜커머스] 아이템 구매 수량 DB에 갱신할 때 실패 함 nResult : " << nResult ) << END_LOG;
        // 구매 아이템을 소셜커머스 수량 갱신에 실패한다면 아래 처리들 무시해야할까??
        // 실패 나면 안되는데... 여기서 실패했다고 공동구매 아이템 구매 기록을 안하는 것도 좀 그렇고...
        // 무시해야지
    }

    // 공동구매 아이템이면 공동구매 신청을 기록
    if ( KSCSellInfo::SELLTYPE_GROUP_PURCHASE_ONE_PLUS_ONE == kPacket_.m_nType 
        || KSCSellInfo::SELLTYPE_GROUP_PURCHASE_BONUS == kPacket_.m_nType )
        InsertGroupPurchaseUser_SocialCommerce( kPacket_.m_dwEventUID, dwUID_ );

    // 소셜커머스 아이템 구매 했다고 기록 하자
    UpdateBuyLog_SocialCommerce( kPacket_.m_dwEventUID, kPacket_.m_nType, dwUID_, kPacket_.m_dwItemID );

    kPacket.m_nCountDiff = nSellCountDiff;
    kPacket.m_dwEventUID = kPacket_.m_dwEventUID;
    
    QUEUING_ACK_TO_USER_UID( EVENT_SOSCIAL_COMMERCE_INFO_UPDATE_NOT );
}

_INT_IMPL_ON_FUNC( DB_EVENT_SOSCIAL_COMMERCE_SELL_STATE_FLAG_UPDATE_REQ, PAIR_DWORD_INT )
{
    int nFlag = KSCSellInfo::FLAG_SELL_ING;

    m_kHelper.GetCommonSP().GetGroupPurchaseFlag_SocialCommerce( kPacket_.first, nFlag );

    // 갱신 하고자 하는 플래그 상태가 동일한 플래그인지 확인 후 기록
    if ( kPacket_.second != nFlag ) {
        // 소셜커머스 객체에서 얻은 flag로 공동구매 상태 갱신
        m_kHelper.GetCommonSP().UpdateSellStateFlag_SocialCommerce( kPacket_.first, kPacket_.second );
    }
}

INT_IMPL_ON_FUNC( DB_EVENT_USER_UDP_PORT_INFO_REQ )
{
    LIF( InsertUserUDPPortInfo( kPacket_ ) );
    START_LOG( clog, L"유저 UDP 포트 정보 통계 DB기록.. size : " << kPacket_.size() )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_COORDI_GRADE_UPGRADE_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_COORDI_GRADE_UPGRADE_REQ );
    KEVENT_COORDI_GRADE_UPGRADE_ACK kPacket;
    kPacket.m_nOK = -99;
    std::list<std::pair<int,float>>::iterator litAtt;

    KItem kItem = kPacket_.m_kItem;
    kItem.m_vecAttribute.clear();

    // 아이템 랜덤 속성 적용.
    bool bUpdateSuccess = true;
    int i;
    for( i = 0, litAtt = kPacket_.m_listAttributeList.begin() ; litAtt != kPacket_.m_listAttributeList.end() ; ++litAtt, ++i ) {
        LIF( bUpdateSuccess = bUpdateSuccess && m_kHelper.UpdateAttribute(dwUID_, i, litAtt->first, litAtt->second, kItem, cCharType_ ) );
    }

    if( bUpdateSuccess == false ) {
        SET_ERR_GOTO( ERR_COORDI_GRADE_UPGRADE_06, END_PROC );
    }

    // DB에 등급정보 갱신.
    LIF( m_kHelper.UpdateItemGrade( dwUID_, kItem.m_ItemUID, (int)kItem.m_cGradeID ) );

    kPacket.m_kItem = kItem;
    kPacket.m_kUseItem = kPacket_.m_kUseItem;

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_kUseItem.m_ItemUID, kPacket_.m_kUseItem.m_nInitCount, kPacket_.m_kUseItem.m_nCount ) ) {
        START_LOG( clog, L"등급 업그레이드 재료 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_kUseItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_kUseItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_kUseItem.m_nCount )
            << END_LOG;
    }

    if ( !UseCountItem( dwUID_, kPacket.m_kUseItem.m_ItemUID, kPacket_.m_nUseCount ) ) {
        START_LOG( cerr, L"등급 업그레이드 재료 아이템 개수 동기화 실패. Name : " << strLogin_ ) 
            << END_LOG;
        kPacket.m_kUseItem.m_nCount = 0;
    } else {
        kPacket.m_kUseItem.m_nCount -= kPacket_.m_nUseCount;
        kPacket.m_kUseItem.m_nCount = std::max<int>( kPacket.m_kUseItem.m_nCount, 0 );
    }

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_COORDI_GRADE_UPGRADE_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_INVEN_DEPOT_MOVE_LOG_REQ )
{
    std::vector<KInvenDepotMoveLog>::iterator vit;
    for( vit = kPacket_.begin(); vit != kPacket_.end(); ++vit ) {
        LIF( m_kHelper.InsertInvenDepotMoveLog( *vit ) );
    }

    START_LOG( clog, L"유저 공용창고 수량 아이템 통계 DB기록.. size : " << kPacket_.size() )
        << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_NICKNAME_VALIDITY_CHECK_REQ, std::wstring )
{
    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_NICKNAME_VALIDITY_CHECK_REQ );

    KEVENT_NICKNAME_VALIDITY_CHECK_ACK kPacket;
    kPacket.m_nOK               = -99;
    kPacket.m_wstrNickName = kPacket_;
    DWORD dwUID;

    if( !m_kHelper.NickToUserUID( kPacket_, dwUID ) ) {
        SET_ERR_GOTO( ERR_NICK_10, END_PROC );
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_NICKNAME_VALIDITY_CHECK_ACK );

    _LOG_CONDITION( NetError::NET_OK == NetError::GetLastNetError(), clog, cwarn, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_wstrNickName )
        << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_DUNGEON_MISSION_REGIST_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_DUNGEON_MISSION_REGIST_REQ );

    KEVENT_DUNGEON_MISSION_REGIST_ACK kPacket;
    int nResultRegistMission = -99;

    if( !UpdateUserData( kPacket_.m_kUserData ) )
    {
        START_LOG( cerr, L"게임서버/DB간 사용자 정보 동기화 실패." )
            << BUILD_LOG( strLogin_ )
            << END_LOG;
    }

    if( !RegistMission( dwUID_, cCharType_, kPacket_.m_dwMissionID, kPacket.m_kMissiondata, nResultRegistMission ) )
    {
        // 오류는 안에서 설정함.
        goto END_PROC;
    }

    SET_ERROR( NET_OK );
END_PROC:

    kPacket.m_nOK = NetError::GetLastNetError();

    QUEUING_ACK_TO_USER( EVENT_DUNGEON_MISSION_REGIST_ACK );

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK || NetError::GetLastNetError() == NetError::ERR_MISSION_02 )
        << L"던전에서 받는 일반/에픽 미션 DB 등록 로그 : "  << NetError::GetLastNetErrMsg() << dbg::endl
        << L"Name : " << strLogin_ << dbg::endl
        << BUILD_LOG( kPacket_.m_dwMissionID )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kMissiondata.m_dwMissionID )
        << BUILD_LOGtm( CTime(kPacket.m_kMissiondata.m_tmRegDate) )
        << BUILD_LOGtm( CTime(kPacket.m_kMissiondata.m_tmEndDate) )
        << BUILD_LOG( kPacket.m_kMissiondata.m_mapCompletionInfo.size() )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;  
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_FINISHED_MISSION_LIST_REQ )
{
    KEVENT_FINISHED_MISSION_LIST_NOT kPacket;

    // 완료한 미션 목록 얻기
    GetFinishedMissionList( dwUID_, cCharType_, kPacket );

    QUEUING_ACK_TO_USER( EVENT_FINISHED_MISSION_LIST_NOT );
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_SHANGHAI_MISSION_INFO_NOT )
{
    std::map< std::pair<char,char>,std::vector<DWORD> > mapCharMissionList;
    std::map< std::pair<char,char>,std::vector<DWORD> > mapRegistMissionAfterResetPunishment;

    SiKSHCheckPoint()->GetCharMissionList( mapCharMissionList );
    // 초기화 처벌 이후에 초기화 해줘야 하는 미션정보 등록
    LIF( m_kHelper.InsertCharMissionInfo( mapCharMissionList, KSHCheckPoint::EMCT_MUST_BE_DELETE ) );

    SiKSHCheckPoint()->GetRegistAginMissionList( mapRegistMissionAfterResetPunishment );
    // 초기화 처벌 이후에 레벨1에 등록 되어야 하는 미션정보 등록
    LIF( m_kHelper.InsertCharMissionInfo( mapRegistMissionAfterResetPunishment, KSHCheckPoint::EMCT_REGISTER_AGAIN ) );

    START_LOG( clog, L"전용던전 미션 정보 DB기록")
        << BUILD_LOG( mapCharMissionList.size() )
        << BUILD_LOG( mapRegistMissionAfterResetPunishment.size() )
        << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_STAT_PVP_PLAY_CHARACTER_COUNT_NOT )
{
    /* DB 기록을 하도록 설정되어 있는 경우에만 DB에 기록한다
       KStatisticsManager::AddStatistics()의 세번째 인자를 참조하게 된다 */
    if ( 1 == dwUID_ ) {
        LIF( Insert_PvPPlayCharactersCount_ToDB( kPacket_ ) );
    }

    START_LOG( clog, L"대전 플레이 캐릭터 타입 통계 기록. dwWriteDB: " << dwUID_ ) << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_GACHA_PONG_ACTION_REQ )
{
    EVENT_CHECK_OUT(DB_EVENT_GACHA_PONG_ACTION_REQ);
    KEVENT_GACHA_PONG_ACTION_ACK kPacket;
    kPacket.m_nOK = -99;
    std::vector<KItem>::iterator vit;
    SET_ERROR(ERR_UNKNOWN);

    if (false == m_kHelper.InsertSingleItemJob(dwUID_,
        cCharType_,
        kPacket_.m_kRewardItem.m_ItemID,
        kPacket_.m_kRewardItem.m_nDuration,
        kPacket_.m_kRewardItem.m_nPeriod,
        KItemManager::EWIT_GACHA_PON,
        kPacket.m_vecRewardItems,
        -1,
        kPacket_.m_kRewardItem.m_cEquipLevel,
        0)) {
        START_LOG(cerr, L"Failed to insert item at player is inventory UID : " << dwUID_)
            << BUILD_LOG(kPacket_.m_kRewardItem.m_ItemID)
            << BUILD_LOG(kPacket_.m_kRewardItem.m_nDuration)
            << BUILD_LOG(kPacket_.m_kRewardItem.m_nPeriod)
            << END_LOG;

        SET_ERR_GOTO(ERR_GACHA_PONG_03, END_PROC);
    }

    for (vit = kPacket.m_vecRewardItems.begin(); vit != kPacket.m_vecRewardItems.end(); ++vit)
    {
        if (vit->m_sEquipLevel != 0)
        {
            bool bUpdateSuccess = true;
            LIF(bUpdateSuccess = bUpdateSuccess && m_kHelper.UpdateLevel(dwUID_, (int)vit->m_sEquipLevel, KItemLevelTable::IL_EQUIP_LV, *vit, cCharType_));
        }
    }

    // µ¿±âÈ­Àü InitCount, Count°ª º¯µ¿°ª ÀÖ´ÂÁö Ã¼Å©.
    if (!CheckItemDuration(dwUID_, kPacket_.m_kGachaPonCoin.m_ItemUID, kPacket_.m_kGachaPonCoin.m_nInitCount, kPacket_.m_kGachaPonCoin.m_nCount)) {
        START_LOG(clog, L"Failed to check gacha pon coin duration. Name : " << strLogin_)
            << BUILD_LOG(dwUID_)
            << BUILD_LOG(kPacket_.m_kGachaPonCoin.m_ItemUID)
            << BUILD_LOG(kPacket_.m_kGachaPonCoin.m_nInitCount)
            << BUILD_LOG(kPacket_.m_kGachaPonCoin.m_nCount)
            << END_LOG;
    }

    if (!UseCountItem(dwUID_, kPacket_.m_kGachaPonCoin.m_ItemUID, 1)) { // °¹¼ö °¨¼Ò.
        START_LOG(cerr, L"Failed to decrease gacha pon coin count. Name : " << strLogin_)
            << END_LOG;
        SET_ERROR(ERR_GACHA_PONG_04);
    }

    kPacket_.m_kGachaPonCoin.m_nCount -= 1;
    kPacket_.m_kGachaPonCoin.m_nCount = std::max<int>(kPacket_.m_kGachaPonCoin.m_nCount, 0);
    kPacket.m_kGachaPonCoin = kPacket_.m_kGachaPonCoin;

    if (IS_CORRECT(ERR_GACHA_PONG_04)) {
        kPacket.m_kGachaPonCoin.m_nCount = 0;
    }

    kPacket.m_prMachineInfo.first = kPacket_.m_dwMachineNumber;
    kPacket.m_prMachineInfo.second = kPacket_.m_MainRewardItemID;
    kPacket.m_dwLeftCapsules = SiKGachaPong()->GetMachineCapsules(kPacket_.m_dwMachineNumber);
    kPacket.m_dwMainRewardCnt = SiKGachaPong()->GetMachineMainRewardCnt(kPacket_.m_dwMachineNumber);
    kPacket.m_bSuccess = kPacket_.m_kRewardItem.m_ItemID == kPacket_.m_MainRewardItemID;

    // °¡Ã­ º¹±Ç ·Î±× ÀÛ¾÷
    LIF(UpdateGachaPonLog(dwUID_, cCharType_, kPacket_.m_dwMachineNumber, kPacket_.m_MainRewardItemID, kPacket_.m_kRewardItem.m_ItemID, kPacket_.m_iCapsule, kPacket_.m_nNumberOfCapsule));
    {
        START_LOG(clog, "Gacha Pon DB result")
            << BUILD_LOG(dwUID_)
            << BUILD_LOG(kPacket_.m_dwMachineNumber)
            << BUILD_LOG(kPacket_.m_MainRewardItemID)
            << BUILD_LOG(kPacket_.m_kRewardItem.m_ItemID)
            << BUILD_LOG(cCharType_)
            << BUILD_LOG(kPacket_.m_iCapsule)
            << BUILD_LOG(kPacket_.m_nNumberOfCapsule)
            << END_LOG;
    }

    SET_ERROR(NET_OK);

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg())
        << BUILD_LOG(strLogin_)
        << BUILD_LOG(dwUID_)
        << BUILD_LOG(kPacket_.m_dwMachineNumber)
        << BUILD_LOG(kPacket_.m_MainRewardItemID)
        << BUILD_LOG(kPacket_.m_kRewardItem.m_ItemID)
        << BUILD_LOG(cCharType_)
        << BUILD_LOG(kPacket_.m_iCapsule)
        << BUILD_LOG(kPacket_.m_nNumberOfCapsule)
        << END_LOG;

    if (kPacket.m_nOK == 0)
    {
        std::vector<DWORD> machineUsers = SiKGachaPong()->GetMachineUsers(kPacket.m_prMachineInfo.first);

        KEVENT_GACHA_PONG_PLAY_INFO_NOT packet;
        packet.m_dwLeftCapsules = kPacket.m_dwLeftCapsules;
        packet.m_dwMainRewardCnt = kPacket.m_dwMainRewardCnt;
        packet.m_prMachineInfo.first = kPacket.m_prMachineInfo.first;
        packet.m_prMachineInfo.second = kPacket.m_prMachineInfo.second;

        std::vector<DWORD>::iterator it = machineUsers.begin();
        for (; it != machineUsers.end(); ++it)
        {
            if (*it != dwUID_)
            {
                START_LOG(clog, L"User")
                    << BUILD_LOG(*it)
                    << END_LOG;
                QUEUING_ACK_TO_SPECIFIC_USER_UID(*it, EVENT_GACHA_PONG_PLAY_INFO_NOT, packet);
            }
        }
    }

    QUEUING_ACK_TO_USER(EVENT_GACHA_PONG_ACTION_ACK);
}

INT_IMPL_ON_FUNC(DB_EVENT_PET_MAGIC_STONE_CHANGE_EQUIP_REQ)
{
    EVENT_CHECK_OUT(DB_EVENT_PET_MAGIC_STONE_CHANGE_EQUIP_REQ);
    KEVENT_PET_MAGIC_STONE_CHANGE_EQUIP_ACK kPacket;
    kPacket.m_nOK = -99;
    std::vector<KItem>::iterator vit;
    SET_ERROR(ERR_UNKNOWN);

    int iGlyphType = SiKPetGlyphMng()->GetPetGlyphType(kPacket_.m_dwPetGlyphID);

    START_LOG(clog, L"DB_EVENT_PET_MAGIC_STONE_CHANGE_EQUIP_REQ")
        << BUILD_LOG(dwUID_)
        << BUILD_LOGc(cCharType_)
        << BUILD_LOG(kPacket_.m_dwPetGlyphUID)
        << BUILD_LOG(kPacket_.m_dwPetGlyphID)
        << BUILD_LOG(iGlyphType)
        << END_LOG;

    if (kPacket_.m_bEquip)
    {
        if (iGlyphType != -1 && !EquipPetGlyph(dwUID_, cCharType_, kPacket_.m_dwPetGlyphUID, kPacket_.m_dwPetGlyphID, iGlyphType))
        {
            SET_ERROR(ERR_PET_GLYPH_02);
            goto END_PROC;
        }
    }
    else
    {
        if (iGlyphType != -1 && !RemovePetGlyph(dwUID_, cCharType_, kPacket_.m_dwPetGlyphUID))
        {
            SET_ERROR(ERR_PET_GLYPH_03);
            goto END_PROC;
        }
    }

    if (!GetPetGlyph(dwUID_, cCharType_, kPacket.m_vecEquippedGlyphs))
    {
        SET_ERROR(ERR_PET_GLYPH_01);
        goto END_PROC;
    }

    SET_ERROR(NET_OK);

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg())
        << BUILD_LOG(strLogin_)
        << BUILD_LOG(dwUID_)
        << BUILD_LOG(kPacket_.m_dwPetGlyphUID)
        << BUILD_LOG(kPacket_.m_iCharType)
        << END_LOG;

    QUEUING_ACK_TO_USER(EVENT_PET_MAGIC_STONE_CHANGE_EQUIP_ACK);
}

INT_IMPL_ON_FUNC(DB_EVENT_BILLBOARD_CHAT_MSG_LOG_REQ)
{
    UpdateBillboardLog(dwUID_, kPacket_.m_wstrNickName, kPacket_.m_wstrMessage);

    START_LOG(clog, L"Billboard Chat insert")
        << BUILD_LOG(dwUID_)
        << BUILD_LOG(kPacket_.m_wstrNickName.data())
        << BUILD_LOG(kPacket_.m_wstrMessage.data())
        << END_LOG;
}

INT_IMPL_ON_FUNC(EVENT_COORDIVIEWER_GETSLOT_REQ)
{
    EVENT_CHECK_OUT(EVENT_COORDIVIEWER_GETSLOT_REQ);
    KEVENT_COORDIVIEWER_GETSLOT_ACK kPacket;
    SET_ERROR(ERR_UNKNOWN);

    std::map< int, std::vector< GCITEMID > >  m_mapSlotInfo;

    if (!GetCoordiViewerCharSlotItems(kPacket_.m_iLoginUID, kPacket_.m_iCharType, m_mapSlotInfo)) {
        SET_ERR_GOTO(ERR_COORDIVIEW_01, END_PROC);
    }

    kPacket.m_mapSlotInfo = m_mapSlotInfo;

    SET_ERROR(NET_OK);

END_PROC:
    _QUEUING_ACK_TO_USER(EVENT_COORDIVIEWER_GETSLOT_ACK, kPacket);

}

INT_IMPL_ON_FUNC(EVENT_COORDIVIEWER_SETSLOT_REQ)
{
    EVENT_CHECK_OUT(EVENT_COORDIVIEWER_SETSLOT_REQ);
    KEVENT_COORDIVIEWER_SETSLOT_ACK kPacket;
    SET_ERROR(ERR_UNKNOWN);

    int iOK = -99;

    if (!SetCoordiViewerCharSlotItems(kPacket_.m_iLoginUID, kPacket_.m_iSlotID, kPacket_.m_iCharType, kPacket_.m_vecItemList, iOK)) {
        SET_ERR_GOTO(ERR_COORDIVIEW_01, END_PROC);
    }

    kPacket.m_iOK = 0;

    SET_ERROR(NET_OK);

END_PROC:
    _QUEUING_ACK_TO_USER(EVENT_COORDIVIEWER_SETSLOT_ACK, kPacket);

}

INT_IMPL_ON_FUNC(EVENT_LOOK_INVENTORY_EXTEND_REQ)
{
    EVENT_CHECK_OUT(EVENT_LOOK_INVENTORY_EXTEND_REQ);
    KEVENT_LOOK_INVENTORY_EXTEND_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR(ERR_UNKNOWN);
    kPacket.m_nExtendSize = kPacket_.m_nExtendSize;

    if (!CheckItemDuration(dwUID_, kPacket_.m_kUseItem.m_ItemUID, kPacket_.m_kUseItem.m_nInitCount, kPacket_.m_kUseItem.m_nCount)) {
        START_LOG(clog, L"코디 인벤 가방 아이템 창고 수량 동기화. Name : " << strLogin_)
            << BUILD_LOG(dwUID_)
            << BUILD_LOG(kPacket_.m_kUseItem.m_ItemUID)
            << BUILD_LOG(kPacket_.m_kUseItem.m_nInitCount)
            << BUILD_LOG(kPacket_.m_kUseItem.m_nCount)
            << END_LOG;
    }

    if (!UseCountItem(dwUID_, kPacket_.m_kUseItem.m_ItemUID, 1)) {
        START_LOG(cerr, L"코디 인벤 가방 아이템 개수 동기화 실패. Name : " << strLogin_)
            << END_LOG;
        SET_ERR_GOTO(ERR_INVEN_15, END_PROC);
        kPacket_.m_kUseItem.m_nCount = 0;
    }
    else {
        kPacket_.m_kUseItem.m_nCount -= 1;
        kPacket_.m_kUseItem.m_nCount = std::max<int>(kPacket_.m_kUseItem.m_nCount, 0);
    }

    LIF(UpdateCharLookItemInvenSize(dwUID_, cCharType_, kPacket_.m_dwSlotNum, kPacket_.m_nExtendSize));

    kPacket.m_kUseItem = kPacket_.m_kUseItem;

    SET_ERROR(NET_OK);
END_PROC:
    if (IS_CORRECT(ERR_INVEN_15)) {
        kPacket.m_kUseItem = kPacket_.m_kUseItem;
        kPacket.m_kUseItem.m_nCount = 0;
    }
    kPacket.m_nOK = NetError::GetLastNetError();

    START_LOG(clog, L"코디 인벤 확장 가방 사용")
        << BUILD_LOG(strLogin_)
        << BUILD_LOG(kPacket_.m_kUseItem.m_ItemID)
        << END_LOG;

    QUEUING_ACK_TO_USER(EVENT_LOOK_INVENTORY_EXTEND_ACK);
}

INT_IMPL_ON_FUNC(DB_EVENT_USE_HERO_TICKET_REQ)
{
    EVENT_CHECK_OUT(DB_EVENT_USE_HERO_TICKET_REQ);
    KEVENT_USE_HERO_TICKET_ACK kPacket;
    kPacket.m_dwItemUID = kPacket_.m_dwItemUID;
    kPacket.iOK = 0;
    if ( !UseCountItem( dwUID_, kPacket.m_dwItemUID, 1 ) ) {
        START_LOG( cerr, L"영웅던전 티켓 재료 아이템 동기화 실패. Name : " << strLogin_ ) << END_LOG;
        kPacket.iOK = -7;
    }
    else if ( !UpdateUserHeroDungeonTicket( dwUID_, cCharType_, kPacket_.m_iDungeonID ) ) {
        START_LOG( cerr, L"영웅던전 티켓 재료 아이템 동기화 실패. Name : " << strLogin_ ) << END_LOG;
        kPacket.iOK = -8;
    }
    else 
    {
        kPacket.iOK = 0;
        SET_ERROR( NET_OK );
    }
    START_LOG( clog, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_iDungeonID )
        << BUILD_LOG( kPacket.m_dwItemUID )
        << BUILD_LOG( kPacket.iOK )
        << END_LOG;
    _QUEUING_ACK_TO_USER( EVENT_USE_HERO_TICKET_ACK, kPacket );
}