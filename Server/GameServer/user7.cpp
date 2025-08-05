#include "User.h"
#include <dbg/dbg.hpp>
#include "UserFSM.h"
#include "support_User.h"
#include "GuildManager.h"
#include <boost/algorithm/string.hpp>
#include "CnConnector.h"
#include "ChannelManager.h"
#include "FailRate.h"
#include "MissionManager.h"
#include "MissionInfo.h"
#include "GameServer.h"
#include "boost/bind.hpp"
#include <KncUtilLua.h>
#include "ItemManager.h"
#include <math.h>
#include "ItemBreakUp.h"
#include "IPAdvantage.h"
#include "PresentBox.h"
#include "GCHelper.h"
#include "LogManager.h"
#include "GambleBuyManager.h"
#include "StatDungeon.h"
#include "CharPromotionSystem.h"
#include "HeroDungeonManager.h"
#include "Gawibawibo.h"
#include "SpecialReward.h"
#include "GameDrop.h"
#include "StatItemGet.h"
#include "LoudMessage.h"
#include "CollectionManager.h"
#include "SkillManager.h"
#include "ResultManager.h"
#include "TickManager.h"
#include "Socks.h"
#include "ChangeWeaponManager.h"
#include "RecomHelper.h"
#include "PlantTree.h"
#include "ChoiceDrop.h"
#include "LookEquipment.h"
#include "VipEventManager.h"
#include "MagicBox.h"
#include "RKTornado.h"
#include "Sphinx.h"
#include "Survey.h"
#include "StrengthManager.h"
#include "Rainbow.h"
#include "PreHackingCheckManager.h"
#include "SlangFilter.h"
#include "FatigueSystem.h"
#include "PVPMatchManager.h"
#include "SocialCommerce.h"

#define CLASS_TYPE  KUser


IMPL_ON_FUNC( EVENT_SEARCH_GUILD_LIST_REQ )
{
    VERIFY_STATE(( 1, KUserFSM::STATE_SQUARE ));
    // 이 패킷을 부르는 경우는.
    // 1.길드목록창을 처음 열때,
    // 2.검색버튼을 누를때.
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_GUILD_LIST_NOT kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nPageNum = 1; // 첫요청은 무조건 1페이지
    kPacket.m_nTotalPageNum = 0; // 총 페이지는 아직 모른다.
    kPacket.m_kCompBuff.Clear();
    std::wstring strKeywordCopy;

    m_strGuildSearchKeyword.clear(); // 기존 검색어를 초기화.
    m_vecGuildSearchUIDList.clear(); // 기존 검색한 길드UID목록 초기화
    m_nGuildSearchTotalPageNum = 0; // 기존 검색한 페이지수 초기화

    // 정렬타입 검사
    if ( kPacket_.m_ucSortType >= KEVENT_SEARCH_GUILD_LIST_REQ::ST_MAX ) {
        SET_ERR_GOTO( ERR_GUILD_LIST_00, END_PROC );
    }

    // 키워드가 없는 경우. 바로 패킷 전달.
    if ( kPacket_.m_strKeyword.empty() ) {
        SiKGuildManager()->GetGuildPage( kPacket_.m_ucSortType, kPacket_.m_ucOrderBy, kPacket.m_nPageNum, kPacket.m_kCompBuff, kPacket.m_nTotalPageNum );
        kPacket.m_nOK = 0; // 성공
        SEND_PACKET( EVENT_GUILD_LIST_NOT );
        SET_ERR_GOTO( NET_OK, END_PROC );
    }

    // 키워드 길이가 최소길이보다 작음
    if ( kPacket_.m_strKeyword.length() < (UINT)SiKGuildManager()->GetKeywordMinLength() ) {
        SET_ERR_GOTO( ERR_GUILD_LIST_04, END_PROC );
    }

    if ( SiKSlangFilter()->IsInQueryInvalidChar( kPacket_.m_strKeyword ) ) {
        SET_ERR_GOTO( ERR_GUILD_LIST_02, END_PROC );
    }

    boost::replace_all( kPacket_.m_strKeyword, L"'", L"''" );
    strKeywordCopy = boost::to_lower_copy( kPacket_.m_strKeyword );
    kPacket_.m_strKeyword = strKeywordCopy;

    // 키워드가 있는 경우만, 검색주기 검사
    if ( ::GetTickCount() - GetGuildSearchTick() < GetGuildSearchTickGap() ) {
        SET_ERR_GOTO( ERR_GUILD_LIST_01, END_PROC );
    }
    // 검색주기 갱신
    SetGuildSearchTick();

    // 키워드가 있는 경우만 DB스레드로 간다(위에서 거름)
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_SEARCH_GUILD_LIST_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,            kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_LIST_00, kPacket.m_nOK, 1 ); // 정의되지 않은 정렬타입
        ERR_CASE( ERR_GUILD_LIST_01, kPacket.m_nOK, 2 ); // 길드목록 검색 갱신주기가 ?음
        ERR_CASE( ERR_GUILD_LIST_02, kPacket.m_nOK, 3 ); // 길드검색 DB처리 실패
        ERR_CASE( ERR_GUILD_LIST_03, kPacket.m_nOK, 4 ); // 검색어가 다르니 검색 초기화함.
        ERR_CASE( ERR_GUILD_LIST_04, kPacket.m_nOK, 5 ); // 키워드 길이가 최소길이보다 작음

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOGc( kPacket_.m_ucSortType )
        << BUILD_LOGc( kPacket_.m_ucOrderBy )
        << BUILD_LOG( kPacket_.m_strKeyword )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nPageNum )
        << BUILD_LOG( kPacket.m_nTotalPageNum )
        << BUILD_LOG( kPacket.m_kCompBuff.GetLength() ) << END_LOG;

    // 실패시 패킷 전달
    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_GUILD_LIST_NOT );
    }
}

IMPL_ON_FUNC( EVENT_SEARCH_GUILD_LIST_ACK )
{
    // 여기로 오는 경우는 검색어가 있는 경우로 DB스레드를 거쳐온다.

    std::vector<KNGuildInfo> vecGuildInfo;

    KEVENT_GUILD_LIST_NOT kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nPageNum = 1; // 첫 요청은 무조건 1페이지
    kPacket.m_nTotalPageNum = 0;
    kPacket.m_kCompBuff.Clear();

    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK) )
        goto END_PROC;

    // 검색에 성공한 경우. 검색한 키워드는 기억한다.
    // 검색한 길드UID 목록은 User가 가지고 있는다.
    m_strGuildSearchKeyword = kPacket_.m_kSearchInfo.m_strKeyword;
    m_vecGuildSearchUIDList.swap( kPacket_.m_vecGuildUIDList );

    // 1페이지당 길드정보 개수
    int nPageSize = SiKGuildManager()->GetGuildPageSize();

    // 총 페이지수 구하기
    m_nGuildSearchTotalPageNum = m_vecGuildSearchUIDList.size() / std::max<int>( 1, nPageSize );
    if ( m_vecGuildSearchUIDList.size() % nPageSize > 0 ) {
        m_nGuildSearchTotalPageNum += 1;
    }
    kPacket.m_nTotalPageNum = m_nGuildSearchTotalPageNum;

    // 검색결과가 없음.
    if ( m_vecGuildSearchUIDList.empty() ) {
        START_LOG( clog, L"검색결과 일치하는 길드가 없음. Keyword : " << kPacket_.m_kSearchInfo.m_strKeyword ) << END_LOG;
        SET_ERR_GOTO( NET_OK, END_PROC );
    }

    // DB에서 받은 리스트로 페이지의 길드정보 받아오기.
    SiKGuildManager()->GetGuildSearchPage( kPacket.m_nPageNum, m_vecGuildSearchUIDList, vecGuildInfo );

    // 받아온 길드정보를 압축하자.
    if ( !vecGuildInfo.empty() ) {
        // 압축
        KSerializer ks;
        ks.BeginWriting( &kPacket.m_kCompBuff );
        ks.Put( vecGuildInfo );
        ks.EndWriting();
        kPacket.m_kCompBuff.Compress();
    }
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,       kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_LIST_00, kPacket.m_nOK, 1 ); // 정의되지 않은 정렬타입
        ERR_CASE( ERR_GUILD_LIST_01, kPacket.m_nOK, 2 ); // 길드목록 검색 갱신주기가 ?음
        ERR_CASE( ERR_GUILD_LIST_02, kPacket.m_nOK, 3 ); // 길드검색 DB처리 실패
        ERR_CASE( ERR_GUILD_LIST_03, kPacket.m_nOK, 4 ); // 검색어가 다르니 검색 초기화함.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_kSearchInfo.m_strKeyword )
        << BUILD_LOGc( kPacket_.m_kSearchInfo.m_ucSortType )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nPageNum )
        << BUILD_LOG( kPacket.m_nTotalPageNum ) << END_LOG;

    SEND_PACKET( EVENT_GUILD_LIST_NOT );
}

IMPL_ON_FUNC( EVENT_GUILD_LIST_REQ )
{
    VERIFY_STATE(( 1, KUserFSM::STATE_SQUARE ));

    // 길드목록에서 페이지를 넘길때 불린다. 인자값은 페이지Num.
    SET_ERROR( ERR_UNKNOWN );

    std::vector<KNGuildInfo> vecGuildInfo;

    KEVENT_GUILD_LIST_NOT kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nPageNum = kPacket_.m_nPagenum;
    kPacket.m_nTotalPageNum = m_nGuildSearchTotalPageNum;
    std::wstring strKeywordCopy;

    // 일반 페이지 목록 요청 처리
    if ( kPacket_.m_strKeyword.empty() ) {

        // 정렬타입 검사
        if ( kPacket_.m_ucSortType >= KEVENT_SEARCH_GUILD_LIST_REQ::ST_MAX ) {
            SET_ERR_GOTO( ERR_GUILD_LIST_00, END_PROC );
        }

        // 길드 매니저에서 만들어놓은 페이지 가지고 오기.
        if ( SiKGuildManager()->GetGuildPage( kPacket_.m_ucSortType, kPacket_.m_ucOrderBy, kPacket.m_nPageNum, kPacket.m_kCompBuff, kPacket.m_nTotalPageNum ) == true ) {
            SET_ERR_GOTO( NET_OK, END_PROC );
        }

        // 페이지가 없는 경우, DB에 정보 요청해서 해당 Page 빌드하도록 패킷 전달
        if ( SendGuildListReqToDB( kPacket_.m_nPagenum, kPacket_.m_ucSortType, kPacket_.m_ucOrderBy ) == false ) {
            // 센터에 길드페이지 데이터 재요청
            SendGuildPageReq( kPacket_.m_ucSortType );
            SET_ERR_GOTO( ERR_GUILD_LIST_06, END_PROC );
        }
        SET_ERR_GOTO( ERR_GUILD_LIST_05, END_PROC );
    }

    // 검색결과 목록에 대한 페이지 처리
    boost::replace_all( kPacket_.m_strKeyword, L"'", L"''" );
    strKeywordCopy = boost::to_lower_copy( kPacket_.m_strKeyword );
    kPacket_.m_strKeyword = strKeywordCopy;

    if ( kPacket_.m_strKeyword != m_strGuildSearchKeyword ) {
        START_LOG( cerr, L"검색결과에 대한 목록 페이지 정보 요청시, 검색어가 다름." )
            << BUILD_LOG( kPacket_.m_strKeyword )
            << BUILD_LOG( m_strGuildSearchKeyword )
            << BUILD_LOG( m_nGuildSearchTotalPageNum ) << END_LOG;

        //검색어가 다름. 이전 검색결과 초기화함.
        m_strGuildSearchKeyword.clear();
        m_vecGuildSearchUIDList.clear();
        m_nGuildSearchTotalPageNum = 0;

        kPacket.m_nPageNum = 0;
        SET_ERR_GOTO( ERR_GUILD_LIST_03, END_PROC );
    }

    // 요청한 페이지 번호가 총 페이지수보다 많음
    if ( kPacket.m_nPageNum > m_nGuildSearchTotalPageNum ) {
        START_LOG( cerr, L"요청한 페이지 번호가 총 페이지수보다 많음." )
            << BUILD_LOG( kPacket.m_nPageNum )
            << BUILD_LOG( m_nGuildSearchTotalPageNum ) << END_LOG;

        // 총페이지 수 재설정/마지막 페이지 반환
        kPacket.m_nPageNum = m_nGuildSearchTotalPageNum;
        kPacket.m_nTotalPageNum = m_nGuildSearchTotalPageNum;
    }

    // DB에서 받은 리스트로 페이지의 길드정보 받아오기.
    SiKGuildManager()->GetGuildSearchPage( kPacket.m_nPageNum, m_vecGuildSearchUIDList, vecGuildInfo );

    // 받아온 길드정보를 압축하자.
    if ( !vecGuildInfo.empty() ) {
        // 압축
        KSerializer ks;
        ks.BeginWriting( &kPacket.m_kCompBuff );
        ks.Put( vecGuildInfo );
        ks.EndWriting();
        kPacket.m_kCompBuff.Compress();
    }
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,            kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_LIST_00, kPacket.m_nOK, 1 ); // 정의되지 않은 정렬타입
        ERR_CASE( ERR_GUILD_LIST_01, kPacket.m_nOK, 2 ); // 길드목록 검색 갱신주기가 ?음
        ERR_CASE( ERR_GUILD_LIST_02, kPacket.m_nOK, 3 ); // 길드검색 DB처리 실패
        ERR_CASE( ERR_GUILD_LIST_03, kPacket.m_nOK, 4 ); // 검색어가 다르니 검색 초기화함.
        ERR_CASE( ERR_GUILD_LIST_05, kPacket.m_nOK, 6 ); // 페이지 빌드 처리 중(DB에 요청)
        ERR_CASE( ERR_GUILD_LIST_06, kPacket.m_nOK, 7 ); // 페이지 빌드 처리 중 데이터가 없음.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 || kPacket.m_nOK == 4 || kPacket.m_nOK == 6, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOGc( kPacket_.m_ucSortType )
        << BUILD_LOGc( kPacket_.m_ucOrderBy )
        << BUILD_LOG( kPacket_.m_strKeyword )
        << BUILD_LOG( kPacket_.m_nPagenum )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nPageNum )
        << BUILD_LOG( kPacket.m_nTotalPageNum ) << END_LOG;

    // DB에 요청중이 아닐때만 보낸다.
    if ( NetError::GetLastNetError() != NetError::ERR_GUILD_LIST_05 ) {
        SEND_PACKET( EVENT_GUILD_LIST_NOT );
    }
}

_IMPL_ON_FUNC( EVENT_INVITE_GUILD_REQ, DWORD )
{
    // 내 길드에 초대(대상이 접속중이고,UserUID를 알수 있는 상황만 한정)(ex.방,광장,로비채팅창 등)
    VERIFY_STATE(( 3, KUserFSM::STATE_SQUARE, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));

    KNGuildInfo kGuild;
    kGuild.Init();

    if ( GetGuildInfo( kGuild ) == false ) {
        START_LOG( cerr, L"내 길드정보가 없음." ) << END_LOG;
        return;
    }

    // 초대패킷 설정
    KGuildInvite kCnPacket;
    kCnPacket.m_dwGuildUID = kGuild.m_dwUID;
    kCnPacket.m_strGuildName = kGuild.m_strName;
    kCnPacket.m_ucGrade = kGuild.m_ucGrade;
    kCnPacket.m_dwToUserUID = kPacket_;
    kCnPacket.m_strFromUserNickName = GetNickName();

    LIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_INVITE_GUILD_NOT, kCnPacket ) );

    START_LOG( clog, L"길드 초대. GuildUID : " << kGuild.m_dwUID )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( kGuild.m_strName )
        << BUILD_LOGc( kGuild.m_ucGrade ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_JOIN_GUILD_REQ )
{
    VERIFY_STATE(( 1, KUserFSM::STATE_SQUARE ));

    // 길드가입신청
    SET_ERROR( ERR_UNKNOWN );
    KEVENT_JOIN_GUILD_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_kGuildUserInfo.Init();
    kPacket.m_kGuildInfo.Init();
    kPacket.m_mapGuildUserList.clear();

    KECN_JOIN_GUILD_REQ kCnPacket;
    kCnPacket.m_dwUserUID = GetUID();
    kCnPacket.m_kGuildInfo.m_dwUID = kPacket_.m_dwGuildUID;
    kCnPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    boost::replace_all( kPacket_.m_strMyComment, L"'", L"''" );
    kCnPacket.m_strMyComment = kPacket_.m_strMyComment;

    KGuildPtr spGuild;

    // 내 길드UID확인
    if ( m_kGuildUserInfo.m_dwGuildUID != 0 ) {
        // 이미 길드 가입 신청중인 상태
        if ( m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_JOINER ) {
            SET_ERR_GOTO( ERR_GUILD_JOIN_00, END_PROC );
        }

        // 이미 길드 가입상태.
        SET_ERR_GOTO( ERR_GUILD_JOIN_01, END_PROC );
    }

    spGuild = SiKGuildManager()->GetGuild( kPacket_.m_dwGuildUID );
    if ( spGuild == NULL ) {
        // 해당 길드정보가 없으면, DB에 문의해놓자.
        QUEUEING_EVENT_TO_DB( EVENT_GET_GUILD_INFO_FROM_DB_REQ, kPacket_.m_dwGuildUID );
    }
    else if ( spGuild->GetGuildMemberListSize() <= 0 ) {
        // 맴버리스트가 없으면 DB에 문의하자.
        QUEUEING_EVENT_TO_DB( EVENT_GET_GUILD_MEMBER_LIST_FROM_DB_REQ, kPacket_.m_dwGuildUID );
    }
    

    // 센터서버로 전달
    SiKCnConnector()->SendPacket( KCenterEvent::ECN_JOIN_GUILD_REQ, kCnPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,            kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_JOIN_00, kPacket.m_nOK, 1 ); // 이미 길드 가입 신청중인 상태
        ERR_CASE( ERR_GUILD_JOIN_01, kPacket.m_nOK, 2 ); // 이미 길드 가입상태.
        ERR_CASE( ERR_GUILD_JOIN_02, kPacket.m_nOK, 3 ); // 길드가입시 유저정보 없음.
        ERR_CASE( ERR_GUILD_JOIN_03, kPacket.m_nOK, 4 ); // 길드가입시 길드정보 없음.
        ERR_CASE( ERR_GUILD_JOIN_04, kPacket.m_nOK, 5 ); // 길드가입시 가입중지상태임.
        ERR_CASE( ERR_GUILD_JOIN_05, kPacket.m_nOK, 6 ); // 길드가입시 이미 가임유저임.
        ERR_CASE( ERR_GUILD_JOIN_06, kPacket.m_nOK, 7 ); // 길드가입시 DB처리 에러
        ERR_CASE( ERR_GUILD_JOIN_07, kPacket.m_nOK, 8 ); // 길드가입시 최대 대기자수가 다찼음.
        ERR_CASE( ERR_GUILD_JOIN_08, kPacket.m_nOK, 9 ); // 길드가입시 최대 맴버수가 다찼음.
        ERR_CASE( ERR_GUILD_JOIN_09, kPacket.m_nOK, 10 ); // 채널링 유저 길드 가입 실패.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_strMyComment ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_JOIN_GUILD_ACK );
    }
}

IMPL_ON_FUNC( EVENT_JOIN_GUILD_ACK )
{
    // 센터서버에서 패킷처리 결과값 받음.
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 여기서부터는 길드가입 성공
    // 접속중인 서버UID 설정
    kPacket_.m_kGuildUserInfo.m_cServerUID = (char)( SiKGameServer()->GetUID() );

    m_kGuildUserInfo.m_dwGuildUID = kPacket_.m_kGuildUserInfo.m_dwGuildUID;
    m_kGuildUserInfo.m_cMemberLevel = kPacket_.m_kGuildUserInfo.m_cMemberLevel;
    m_kGuildUserInfo.m_cServerUID = kPacket_.m_kGuildUserInfo.m_cServerUID;
    m_kGuildUserInfo.m_dwContributePoint = kPacket_.m_kGuildUserInfo.m_dwContributePoint;

    // 로컬에서 먼저 길드원 정보 추가해주도록 하자.
    LIF( SiKGuildManager()->AddUser( kPacket_.m_kGuildUserInfo ) );

    // 길드맴버리스트 받아오기
    LIF( SiKGuildManager()->GetGuildMemberList( m_kGuildUserInfo.m_dwGuildUID, kPacket_.m_mapGuildUserList ) );

    // 길드매니저에 알려서 해당 길드에 길드 가입상태 알림.
    LIF( SiKGuildManager()->Register( m_kGuildUserInfo.m_dwGuildUID, GetThisPtr() ) );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,            kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_JOIN_00, kPacket_.m_nOK, 1 ); // 이미 길드 가입 신청중인 상태
        ERR_CASE( ERR_GUILD_JOIN_01, kPacket_.m_nOK, 2 ); // 이미 길드 가입상태.
        ERR_CASE( ERR_GUILD_JOIN_02, kPacket_.m_nOK, 3 ); // 길드가입시 유저정보 없음.
        ERR_CASE( ERR_GUILD_JOIN_03, kPacket_.m_nOK, 4 ); // 길드가입시 길드정보 없음.
        ERR_CASE( ERR_GUILD_JOIN_04, kPacket_.m_nOK, 5 ); // 길드가입시 가입중지상태임.
        ERR_CASE( ERR_GUILD_JOIN_05, kPacket_.m_nOK, 6 ); // 길드가입시 이미 가임유저임.
        ERR_CASE( ERR_GUILD_JOIN_06, kPacket_.m_nOK, 7 ); // 길드가입시 DB처리 에러
        ERR_CASE( ERR_GUILD_JOIN_07, kPacket_.m_nOK, 8 ); // 길드가입시 최대 대기자수가 다찼음.
        ERR_CASE( ERR_GUILD_JOIN_08, kPacket_.m_nOK, 9 ); // 길드가입시 최대 맴버수가 다찼음.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kGuildUserInfo.m_dwGuildUID )
        << BUILD_LOGc( kPacket_.m_kGuildUserInfo.m_cMemberLevel )
        << BUILD_LOG( kPacket_.m_kGuildInfo.m_dwUID )
        << BUILD_LOG( kPacket_.m_kGuildInfo.m_strName )
        << BUILD_LOG( kPacket_.m_mapGuildUserList.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_JOIN_GUILD_ACK );
}

_IMPL_ON_FUNC( EVENT_CANCEL_JOIN_GUILD_REQ, DWORD )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_CHANNEL ));

    // 길드가입 취소의 경우, 길드탈퇴와 동일한 방식으로 진행한다.
    PAIR_DWORD kPacket;
    kPacket.first = GetUID(); // 유저UID
    kPacket.second = kPacket_; // 길드UID

    // 센터서버로 전달
    SiKCnConnector()->SendPacket( KCenterEvent::ECN_CANCEL_JOIN_GUILD_REQ, kPacket );

    START_LOG( clog, L"길드가입 취소 신청. Login : " << m_strName << L", GuildUID : " << kPacket_ ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_CANCEL_JOIN_GUILD_ACK )
{
    // 센터서버에서 취소요청의 결과값 전달
    NetError::SetLastNetError( kPacket_.m_nOK );

    std::set<DWORD> setUID;

    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 여기서부터는 길드가입취소 성공
    // 내 길드정보 초기화하고,
    m_kGuildUserInfo.Init();

    // 길드매니저랑 길드에서 내 정보 제거/탈퇴했다는 것에대한 처리
    // 탈퇴처리되면 센터서버에도 알려서 다른 게임서버에도 알려줌.
    setUID.insert( GetUID() );
    SiKGuildManager()->DrumoutUser( kPacket_.m_dwGuildUID, setUID, KNGuildUserInfo::OT_CANCEL_JOIN ); // 필요한가?

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,            kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_CANCEL_JOIN_00, kPacket_.m_nOK, 1 ); // 유저정보 없음
        ERR_CASE( ERR_GUILD_CANCEL_JOIN_01, kPacket_.m_nOK, 2 ); // 길드정보 없음
        ERR_CASE( ERR_GUILD_CANCEL_JOIN_02, kPacket_.m_nOK, 3 ); // 탈퇴 등급 아님(길마 혹은 DB에 등록되지 않은 등급)
        ERR_CASE( ERR_GUILD_CANCEL_JOIN_03, kPacket_.m_nOK, 4 ); // DB처리 에러
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CANCEL_JOIN_GUILD_ACK );
}

IMPL_ON_FUNC( EVENT_CREATE_GUILD_REQ )
{
    VERIFY_STATE(( 1, KUserFSM::STATE_SQUARE ));

    // 길드 생성
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_CREATE_GUILD_ACK kPacket;
    kPacket.m_nOK = -99;

    // 길드생성 제한조건
    int nGuildNameMinSize = SiKGuildManager()->GetGuildNameMinSize(); // 길드명 최소사이즈
    int nGuildNameMaxSize = SiKGuildManager()->GetGuildNameMaxSize(); // 길드명 최대사이즈
    DWORD dwGuildMasterMinLevel = SiKGuildManager()->GetGuildMasterMinLevel(); // 캐릭터최소레벨제한
    int nGuildCreateGP = SiKGuildManager()->GetGuildCreateGP(); // 길드생성시 필요한 GP량
    std::wstring strNameCopy;
    std::vector<std::pair<DWORD,DWORD> > vecUnicode;
    bool bAllowUnicode = true;

    // 내정보
    const KCharacterInfo* pkBestCharInfo = GetBestCharacter();
    if ( pkBestCharInfo == NULL ) {
        SET_ERR_GOTO( ERR_GUILD_CREATE_01, END_PROC );
    }

    DWORD dwBestCharLevel = pkBestCharInfo->m_dwLevel;

    // 길드명 길이 조건 체크
    if ( kPacket_.m_strGuildName.size() < (UINT)nGuildNameMinSize ||
         kPacket_.m_strGuildName.size() > (UINT)nGuildNameMaxSize ) {
        // 길드생성시 길드명이 길이 조건이 안맞음.
        SET_ERR_GOTO( ERR_GUILD_CREATE_00, END_PROC );
    }

    strNameCopy = boost::to_lower_copy( kPacket_.m_strGuildName ); // 070504. woosh.
    if( SiKSlangFilter()->HasSlang( strNameCopy ) ) {
        SET_ERR_GOTO( ERR_GUILD_CREATE_09, END_PROC );
    }

    if( SiKSlangFilter()->IsInQueryInvalidChar( kPacket_.m_strGuildName ) ) {
        SET_ERR_GOTO( ERR_GUILD_CREATE_04, END_PROC );
    }

    if( SiKSlangFilter()->IsInQueryInvalidChar( kPacket_.m_strGuildURL ) ) {
        SET_ERR_GOTO( ERR_GUILD_CREATE_10, END_PROC );
    }

    SiKGuildManager()->GetUnicode( vecUnicode );
    bAllowUnicode = SiKGuildManager()->GetAllowUnicode();
    if( !vecUnicode.empty() && SiKSlangFilter()->CheckExtUnicode( bAllowUnicode, vecUnicode, kPacket_.m_strGuildName ) ) {
        SET_ERR_GOTO( ERR_GUILD_CREATE_09, END_PROC );
    }

    // 조건레벨 이상 캐릭터를 보유하고 있는지?
    if ( dwBestCharLevel < dwGuildMasterMinLevel ) {
        SET_ERR_GOTO( ERR_GUILD_CREATE_01, END_PROC );
    }

    // 필요 GP량 가지고 있는지?
    if ( GetGP() < nGuildCreateGP ) {
        SET_ERR_GOTO( ERR_GUILD_CREATE_02, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_CREATE_GUILD_REQ, kPacket_ );
    SET_ERROR( NET_OK );
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,              kPacket.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_GUILD_CREATE_00, kPacket.m_nOK, 1 );    // 길드생성시 길드명이 길이 조건이 안맞음.
        ERR_CASE( ERR_GUILD_CREATE_01, kPacket.m_nOK, 2 );    // 길드생성시 조건레벨 이상의 캐릭터가 없다.
        ERR_CASE( ERR_GUILD_CREATE_02, kPacket.m_nOK, 3 );    // 길드생성시 필요GP량 부족.
        ERR_CASE( ERR_GUILD_CREATE_03, kPacket.m_nOK, 4 );    // 유저정보 없음
        ERR_CASE( ERR_GUILD_CREATE_04, kPacket.m_nOK, 5 );    // 길드이름 중에 특수문자가 있음
        ERR_CASE( ERR_GUILD_CREATE_05, kPacket.m_nOK, 6 );    // 길드이름 중복
        ERR_CASE( ERR_GUILD_CREATE_06, kPacket.m_nOK, 7 );    // URL 중복
        ERR_CASE( ERR_GUILD_CREATE_07, kPacket.m_nOK, 8 );    // 이미 가입유저임
        ERR_CASE( ERR_GUILD_CREATE_08, kPacket.m_nOK, 9 );    // DB처리 에러
        ERR_CASE( ERR_GUILD_CREATE_09, kPacket.m_nOK, 10 );   // 길드생성시 길드이름 중에 금지어 있음.
        ERR_CASE( ERR_GUILD_CREATE_10, kPacket.m_nOK, 11 );   // 길드생성시 URL 중에 특수문자가 있음.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 || kPacket.m_nOK == 5 || kPacket.m_nOK == 6 || kPacket.m_nOK == 10 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( nGuildNameMinSize )
        << BUILD_LOG( nGuildNameMaxSize )
        << BUILD_LOG( dwBestCharLevel )
        << BUILD_LOG( dwGuildMasterMinLevel )
        << BUILD_LOG( GetGP() )
        << BUILD_LOG( nGuildCreateGP )
        << BUILD_LOG( kPacket_.m_strGuildName )
        << BUILD_LOG( kPacket_.m_strGuildURL ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_CREATE_GUILD_ACK );
    }
}

IMPL_ON_FUNC( EVENT_CREATE_GUILD_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    KGuildPtr spGuild;

    if ( !IS_CORRECT( NET_OK) )
        goto END_PROC;

    // 여기서부터는 성공적으로 길드가 생성되었다.
    // 길드 매니저에 길드정보 등록
    kPacket_.m_kGuildInfo.m_strMasterNick = GetNickName(); // 마스터 닉네임 등록
    SiKGuildManager()->CreateGuild( kPacket_.m_kGuildInfo, kPacket_.m_vecGuildNotice, kPacket_.m_mapGuildUserList, GetThisPtr() );

    // 길드 만들었으니 길드생성비 GP 깎기
    int nGuildCreateGP = SiKGuildManager()->GetGuildCreateGP();
    bool bDBUpdate = false; // DB에서 GP 갱신 처리 하지 않았음.
    DecreaseGP( KGpStatInfo::GDT_CREATE_GUILD, bDBUpdate, nGuildCreateGP );

    // 길드 생성후 남은 GP량 클라이언트에게 전송.
    kPacket_.m_iGamePoint = GetGP();

    // 나의 길드정보 얻기
    spGuild = SiKGuildManager()->GetGuild( kPacket_.m_kGuildInfo.m_dwUID );
    if ( !spGuild ) {
        START_LOG( cerr, L"바로 위에서 등록했는데, 내 길드정보가 없음. GuildUID : " << kPacket_.m_kGuildInfo.m_dwUID ) << END_LOG;
    } else {
        // 내 길드정보 받아오기.
        KNGuildUserInfo kInfo;
        kInfo.Init();

        LIF( spGuild->GetNGuildUserInfo( GetUID(), kInfo ) );

        m_kGuildUserInfo.m_dwGuildUID = kInfo.m_dwGuildUID;
        m_kGuildUserInfo.m_cMemberLevel = kInfo.m_cMemberLevel;
        m_kGuildUserInfo.m_cServerUID = (char)( SiKGameServer()->GetUID() );
        m_kGuildUserInfo.m_dwContributePoint = kInfo.m_dwContributePoint;

        // 접속 등록
        LIF( SiKGuildManager()->Register( kPacket_.m_kGuildInfo.m_dwUID, GetThisPtr() ) );
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,              kPacket_.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_GUILD_CREATE_00, kPacket_.m_nOK, 1 );    // 길드생성시 길드명이 길이 조건이 안맞음.
        ERR_CASE( ERR_GUILD_CREATE_01, kPacket_.m_nOK, 2 );    // 길드생성시 조건레벨 이상의 캐릭터가 없다.
        ERR_CASE( ERR_GUILD_CREATE_02, kPacket_.m_nOK, 3 );    // 길드생성시 필요GP량 부족.
        ERR_CASE( ERR_GUILD_CREATE_03, kPacket_.m_nOK, 4 );    // 유저정보 없음
        ERR_CASE( ERR_GUILD_CREATE_04, kPacket_.m_nOK, 5 );    // 길드이름 중에 특수문자가 있음
        ERR_CASE( ERR_GUILD_CREATE_05, kPacket_.m_nOK, 6 );    // 길드이름 중복
        ERR_CASE( ERR_GUILD_CREATE_06, kPacket_.m_nOK, 7 );    // URL 중복
        ERR_CASE( ERR_GUILD_CREATE_07, kPacket_.m_nOK, 8 );    // 이미 가입유저임
        ERR_CASE( ERR_GUILD_CREATE_08, kPacket_.m_nOK, 9 );    // DB처리 에러
        ERR_CASE( ERR_GUILD_CREATE_09, kPacket_.m_nOK, 10 );   // 길드생성시 길드이름 중에 금지어 있음.
        ERR_CASE( ERR_GUILD_CREATE_10, kPacket_.m_nOK, 11 );   // 길드생성시 URL 중에 특수문자가 있음.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kGuildInfo.m_dwUID )
        << BUILD_LOG( kPacket_.m_vecGuildNotice.size() )
        << BUILD_LOG( kPacket_.m_mapGuildUserList.size() )
        << BUILD_LOG( GetGP() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CREATE_GUILD_ACK );
}

_IMPL_ON_FUNC( EVENT_EDIT_GUILD_NOTICE_REQ, KNGuildNoticeList )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_CHANNEL ));

    SET_ERROR( ERR_UNKNOWN );
    std::vector<KNGuildNotice>::iterator vit;
    //KNGuildNoticeList kPacket;
    //kPacket.m_nOK = -99;
    //kPacket.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;

    if ( m_kGuildUserInfo.m_dwGuildUID == 0 ) {
        // 소속 길드가 없음.
        SET_ERR_GOTO( ERR_GUILD_NOTICE_00, END_PROC );
    }
    kPacket_.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;

    if ( kPacket_.m_vecNotice.empty() ) {
        // 소개공지 정보가 비어있음.
        SET_ERR_GOTO( ERR_GUILD_NOTICE_01, END_PROC );
    }

    if ( m_kGuildUserInfo.m_cMemberLevel != KNGuildUserInfo::GL_GENERAL &&
         m_kGuildUserInfo.m_cMemberLevel != KNGuildUserInfo::GL_MASTER ) {
        // 길드소개 수정권한이 없음.
        SET_ERR_GOTO( ERR_GUILD_NOTICE_03, END_PROC );
    }

    for ( vit = kPacket_.m_vecNotice.begin() ; vit != kPacket_.m_vecNotice.end() ; ++vit ) {
        boost::replace_all( vit->m_strMsg, L"'", L"''" );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_EDIT_GUILD_NOTICE_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_GUILD_NOTICE_00,  kPacket_.m_nOK, 1 );    // 길드소개공지 수정시 소속 길드가 없음.
        ERR_CASE( ERR_GUILD_NOTICE_01,  kPacket_.m_nOK, 2 );    // 길드소개공지 수정시 소개공지가 비어있음.
        ERR_CASE( ERR_GUILD_NOTICE_02,  kPacket_.m_nOK, 3 );    // 길드소개공지 수정시 유저정보 없음.
        ERR_CASE( ERR_GUILD_NOTICE_03,  kPacket_.m_nOK, 4 );    // 길드소개공지 수정시 길드정보 없음.
        ERR_CASE( ERR_GUILD_NOTICE_04,  kPacket_.m_nOK, 5 );    // 길드소개공지 수정시 수정권한이 없음.
        ERR_CASE( ERR_GUILD_NOTICE_05,  kPacket_.m_nOK, 6 );    // 길드소개공지 수정시 길드Notice정보 없음 DBA호출.
        ERR_CASE( ERR_GUILD_NOTICE_06,  kPacket_.m_nOK, 7 );    // 길드소개공지 수정시 DB처리 에러.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    START_LOG( clog, L"-- Received Guild Notice --  size : " << kPacket_.m_vecNotice.size() ) << END_LOG;
    for ( vit = kPacket_.m_vecNotice.begin() ; vit != kPacket_.m_vecNotice.end() ; ++vit ) {
        START_LOG( cwarn, L"" << vit->m_nMsgID )
            << BUILD_LOG( vit->m_strMsg  ) << END_LOG;
    }
    START_LOG( clog, L"-- end data --" ) << END_LOG;

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_vecNotice.size() ) << END_LOG;

    if ( kPacket_.m_nOK != 0 ) {
        SEND_RECEIVED_PACKET( EVENT_EDIT_GUILD_NOTICE_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_EDIT_GUILD_NOTICE_ACK, KNGuildNoticeList )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    std::vector<KNGuildNotice>::iterator vit;

    if ( !IS_CORRECT( NET_OK) )
        goto END_PROC;

    // 여기서부턴 길드 소개/공지 입력(수정) 성공
    // 길드매니저를 통해 해당 길드의 공지 바꾸기

    for ( vit = kPacket_.m_vecNotice.begin() ; vit != kPacket_.m_vecNotice.end() ; ++vit ) {
        boost::replace_all( vit->m_strMsg, L"''", L"'" );
    }

    LIF( SiKGuildManager()->ModifyGuildNotice( kPacket_.m_dwGuildUID, kPacket_.m_vecNotice ) );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_GUILD_NOTICE_00,  kPacket_.m_nOK, 1 );    // 길드소개공지 수정시 소속 길드가 없음.
        ERR_CASE( ERR_GUILD_NOTICE_01,  kPacket_.m_nOK, 2 );    // 길드소개공지 수정시 소개공지가 비어있음.
        ERR_CASE( ERR_GUILD_NOTICE_02,  kPacket_.m_nOK, 3 );    // 길드소개공지 수정시 유저정보 없음.
        ERR_CASE( ERR_GUILD_NOTICE_03,  kPacket_.m_nOK, 4 );    // 길드소개공지 수정시 길드정보 없음.
        ERR_CASE( ERR_GUILD_NOTICE_04,  kPacket_.m_nOK, 5 );    // 길드소개공지 수정시 수정권한이 없음.
        ERR_CASE( ERR_GUILD_NOTICE_05,  kPacket_.m_nOK, 6 );    // 길드소개공지 수정시 길드Notice정보 없음 DBA호출.
        ERR_CASE( ERR_GUILD_NOTICE_06,  kPacket_.m_nOK, 7 );    // 길드소개공지 수정시 DB처리 에러.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_vecNotice.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_EDIT_GUILD_NOTICE_ACK );
}

IMPL_ON_FUNC( EVENT_EDIT_GUILD_NAME_REQ )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_CHANNEL ));

    SET_ERROR( ERR_UNKNOWN );

    KEVENT_EDIT_GUILD_NAME_ACK kPacket;
    KGuildPtr spGuild;
    std::wstring strNameCopy;
    std::vector< std::pair<DWORD,DWORD> > vecUnicode;
    bool bAllowUnicode = false;

    // 패킷으로 온 길드 UID 로 검색되는 길드가 존재하는가
    spGuild = SiKGuildManager()->GetGuild( kPacket_.m_dwGuildUID );
    if( spGuild == NULL ) {
        // 없는 길드UID
        SET_ERR_GOTO( ERR_GUILD_EDIT_NAME_09, END_PROC );
    }

    // 길드명 변경 아이템은 존재 하는가?
    if ( m_kInventory.FindDurationItemByItemID( SiKGuildManager()->GetGuildNameItemID() ) == NULL ) {
        SET_ERR_GOTO( ERR_GUILD_EDIT_NAME_07, END_PROC );
    }

    int nGuildNameMinSize = SiKGuildManager()->GetGuildNameMinSize(); // 길드명 최소사이즈
    int nGuildNameMaxSize = SiKGuildManager()->GetGuildNameMaxSize(); // 길드명 최대사이즈

    // 길드명 길이 조건 체크
    if ( kPacket_.m_strGuildName.size() < (UINT)nGuildNameMinSize ||
        kPacket_.m_strGuildName.size() > (UINT)nGuildNameMaxSize ) {
            // 길드생성시 길드명이 길이 조건이 안맞음.
        SET_ERR_GOTO( ERR_GUILD_EDIT_NAME_08, END_PROC );
    }

    // 길드원이 아니다.
    if ( m_kGuildUserInfo.m_dwGuildUID == 0 ) {
        SET_ERR_GOTO( ERR_GUILD_EDIT_NAME_01, END_PROC );
    }

    // 길드마스터가 아님(권한 없음)
    if ( m_kGuildUserInfo.m_cMemberLevel != KNGuildUserInfo::GL_MASTER ) {
        SET_ERR_GOTO( ERR_GUILD_EDIT_NAME_02, END_PROC );
    }

    // 바꾸고자 하는 길드명이 적합한 문자열인가
    strNameCopy = boost::to_lower_copy( kPacket_.m_strGuildName ); // 070504. woosh.
    if( SiKSlangFilter()->HasSlang( strNameCopy ) ) {
        SET_ERR_GOTO( ERR_GUILD_EDIT_NAME_03, END_PROC );
    }

    if( SiKSlangFilter()->IsInQueryInvalidChar( kPacket_.m_strGuildName) ) {
        SET_ERR_GOTO( ERR_GUILD_EDIT_NAME_04, END_PROC );
    }

    boost::replace_all( kPacket_.m_strGuildName, L"'", L"''" );

    SiKGuildManager()->GetUnicode( vecUnicode );
    bAllowUnicode = SiKGuildManager()->GetAllowUnicode();
    if( !vecUnicode.empty() && SiKSlangFilter()->CheckExtUnicode( bAllowUnicode, vecUnicode, kPacket_.m_strGuildName ) ) {
        SET_ERR_GOTO( ERR_GUILD_EDIT_NAME_03, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_EDIT_GUILD_NAME_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                 kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_EDIT_NAME_00, kPacket.m_nOK, 1 ); // 길드명변경시 유저 정보 없음.
        ERR_CASE( ERR_GUILD_EDIT_NAME_01, kPacket.m_nOK, 2 ); // 길드명변경시 길드원이 아님.
        ERR_CASE( ERR_GUILD_EDIT_NAME_02, kPacket.m_nOK, 3 ); // 길드명변경시 길드마스터가 아님.
        ERR_CASE( ERR_GUILD_EDIT_NAME_03, kPacket.m_nOK, 4 ); // 길드명변경시 금지어 포함. 
        ERR_CASE( ERR_GUILD_EDIT_NAME_04, kPacket.m_nOK, 5 ); // 길드명변경시 DB금지문자 포함. 
        ERR_CASE( ERR_GUILD_EDIT_NAME_05, kPacket.m_nOK, 6 ); // 길드명변경시 길드정보 없음.
        ERR_CASE( ERR_GUILD_EDIT_NAME_06, kPacket.m_nOK, 7 ); // 길드명변경시 DB처리 에러.
        ERR_CASE( ERR_GUILD_EDIT_NAME_07, kPacket.m_nOK, 8 ); // 길드명변경시 필요 아이템 없음.
        ERR_CASE( ERR_GUILD_EDIT_NAME_08, kPacket.m_nOK, 9 ); // 길드명변경시 길드명이 길이 조건이 안맞음.
        ERR_CASE( ERR_GUILD_EDIT_NAME_09, kPacket.m_nOK, 10 ); // 길드명 변경시 길드가 존재하지 않음.
        ERR_CASE( ERR_GUILD_EDIT_NAME_10, kPacket.m_nOK, 11 ); // 길드명 변경시 중복된 길드이름.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket.m_nOK = 99;
    }

     _LOG_SUCCESS( kPacket.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
         << BUILD_LOG( m_strName )
         << BUILD_LOG( kPacket.m_nOK )
         << BUILD_LOG( kPacket_.m_strGuildName ) << END_LOG;

     if ( kPacket.m_nOK != 0 ) {
         SEND_PACKET( EVENT_EDIT_GUILD_NAME_ACK );
     }
}

IMPL_ON_FUNC( EVENT_EDIT_GUILD_NAME_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    KItemPtr pkItem;
    GCITEMID dwGuildNameItemID = SiKGuildManager()->GetGuildNameItemID();

    if ( !IS_CORRECT( NET_OK ) )
        goto END_PROC;

    // 여기서부턴 길드 이름 수정 성공
    // 길드매니저를 통해 해당 길드의 이름 바꾸기
    LIF( SiKGuildManager()->ModifyGuildName( kPacket_.m_dwGuildUID, kPacket_.m_strGuildName ) );

    pkItem = m_kInventory.FindDurationItemByItemID( dwGuildNameItemID );
    if( pkItem == NULL )
    {
        START_LOG( cerr, L"감소시키려는 길드명변경 아이템이 인벤에 없음. Login : " << m_strName )
            << BUILD_LOG( dwGuildNameItemID ) << END_LOG;
        goto END_PROC;
    }

    // 아이템 감소
    pkItem->m_nCount = std::max<int>( 0, pkItem->m_nCount -1 );
    kPacket_.m_vecItem.push_back( *pkItem );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                  kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_EDIT_NAME_00,  kPacket_.m_nOK, 1 ); // 길드명변경시 유저 정보 없음.
        ERR_CASE( ERR_GUILD_EDIT_NAME_01,  kPacket_.m_nOK, 2 ); // 길드명변경시 길드원이 아님.
        ERR_CASE( ERR_GUILD_EDIT_NAME_02,  kPacket_.m_nOK, 3 ); // 길드명변경시 길드마스터가 아님.
        ERR_CASE( ERR_GUILD_EDIT_NAME_03,  kPacket_.m_nOK, 4 ); // 길드명변경시 금지어 포함. 
        ERR_CASE( ERR_GUILD_EDIT_NAME_04,  kPacket_.m_nOK, 5 ); // 길드명변경시 DB금지문자 포함. 
        ERR_CASE( ERR_GUILD_EDIT_NAME_05,  kPacket_.m_nOK, 6 ); // 길드명변경시 길드정보 없음.
        ERR_CASE( ERR_GUILD_EDIT_NAME_06,  kPacket_.m_nOK, 7 ); // 길드명변경시 DB처리 에러.
        ERR_CASE( ERR_GUILD_EDIT_NAME_07,  kPacket_.m_nOK, 8 ); // 길드명변경시 필요 아이템 없음.
        ERR_CASE( ERR_GUILD_EDIT_NAME_08,  kPacket_.m_nOK, 9 ); // 길드명변경시 길드명이 길이 조건이 안맞음.
        ERR_CASE( ERR_GUILD_EDIT_NAME_09,  kPacket_.m_nOK, 10 ); // 길드명 변경시 길드가 존재하지 않음.
        ERR_CASE( ERR_GUILD_EDIT_NAME_10,  kPacket_.m_nOK, 11 ); // 길드명 변경시 중복된 길드이름.
        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( dwGuildNameItemID )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_strGuildName )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_EDIT_GUILD_NAME_ACK );

}

_IMPL_ON_FUNC( EVENT_CHANGE_GUILD_JOIN_SETTING_REQ, KNGuildJoinPolicy )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_CHANNEL ));

    SET_ERROR( ERR_UNKNOWN );

    KCnNGuildJoinPolicy kCnPacket;
    kCnPacket.m_dwUserUID = GetUID();
    kPacket_.m_dwUID = m_kGuildUserInfo.m_dwGuildUID; // 서버에 있는 길드 데이터를 사용한다
    kCnPacket.m_kPolicy = kPacket_;

    // 패킷으로 온 길드 UID 로 검색되는 길드가 존재하는가
    KGuildPtr spGuild = SiKGuildManager()->GetGuild( m_kGuildUserInfo.m_dwGuildUID );
    if( spGuild == NULL ) {
        SET_ERR_GOTO( ERR_GUILD_JOIN_POLICY_01, END_PROC );
    }

    // 가입 승인 방법에 대한  값 범위 확인. ( 1,2,3 세 종류 )
    if ( kPacket_.m_ucMethod < KNGuildInfo::JS_AUTO_ACCEPT || 
        kPacket_.m_ucMethod > KNGuildInfo::JS_STOP_JOIN ) {
        SET_ERR_GOTO( ERR_GUILD_JOIN_POLICY_00, END_PROC ); // 길드가입 승인방법 변경시 값 범위 부적합 
    }

    // 길드원이 아니다.
    if ( m_kGuildUserInfo.m_dwGuildUID == 0 ) {
        SET_ERR_GOTO( ERR_GUILD_JOIN_POLICY_02, END_PROC );
    }

    // 길드마스터가 아님(권한 없음)
    if ( m_kGuildUserInfo.m_cMemberLevel != KNGuildUserInfo::GL_MASTER ) {
        SET_ERR_GOTO( ERR_GUILD_JOIN_POLICY_03, END_PROC );
    }

    // 센터서버로 전달
    SiKCnConnector()->SendPacket( KCenterEvent::ECN_CHANGE_GUILD_JOIN_SETTING_REQ, kCnPacket );
    SET_ERROR( NET_OK );

END_PROC:
     switch( NetError::GetLastNetError() )
     {
         ERR_CASE( NET_OK, kPacket_.m_nOK, 0 ); // 성공.
         ERR_CASE( ERR_GUILD_JOIN_POLICY_00, kPacket_.m_nOK, 1 ); // 길드가입 승인방법 변경시 값 범위 부적합  
         ERR_CASE( ERR_GUILD_JOIN_POLICY_01, kPacket_.m_nOK, 2 ); // 길드가입 승인방법 변경시 길드 없음
         ERR_CASE( ERR_GUILD_JOIN_POLICY_02, kPacket_.m_nOK, 3 ); // 길드가입 승인방법 변경시 길드원 아님
         ERR_CASE( ERR_GUILD_JOIN_POLICY_03, kPacket_.m_nOK, 4 ); // 길드가입 승인방법 변경시 권한없음[길드마스터 아님]
         default:
             START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                 << BUILD_LOG( m_strName ) << END_LOG;
             kPacket_.m_nOK = 99;
     }

     if ( kPacket_.m_nOK != 0 ) {
         SEND_RECEIVED_PACKET( EVENT_CHANGE_GUILD_JOIN_SETTING_ACK );
     }
 
     _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
         << BUILD_LOG( m_strName )
         << BUILD_LOG( kPacket_.m_nOK )
         << BUILD_LOG( kPacket_.m_dwUID )
         << BUILD_LOGc( kPacket_.m_ucMethod ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_CHANGE_GUILD_JOIN_SETTING_ACK, KNGuildJoinPolicy )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    // 여기서부턴 길드 가입승인 방법 수정 성공
    // 길드매니저를 통해 해당 길드의 가입승인 방법 정보 바꾸기
    LIF( SiKGuildManager()->ChangeJoinPolicy( kPacket_ ) );


END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_JOIN_POLICY_00, kPacket_.m_nOK, 1 ); // 길드가입 승인방법 변경시 값 범위 부적합  
        ERR_CASE( ERR_GUILD_JOIN_POLICY_01, kPacket_.m_nOK, 2 ); // 길드가입 승인방법 변경시 길드 없음
        ERR_CASE( ERR_GUILD_JOIN_POLICY_02, kPacket_.m_nOK, 3 ); // 길드가입 승인방법 변경시 길드원 아님
        ERR_CASE( ERR_GUILD_JOIN_POLICY_03, kPacket_.m_nOK, 4 ); // 길드가입 승인방법 변경시 권한없음[길드마스터 아님]
        ERR_CASE( ERR_GUILD_JOIN_POLICY_04, kPacket_.m_nOK, 5 ); // 길드가입 승인방법 변경시 유저정보 없음 
        ERR_CASE( ERR_GUILD_JOIN_POLICY_05, kPacket_.m_nOK, 6 ); // 길드가입 승인방법 변경시 길드정보 없음 
        ERR_CASE( ERR_GUILD_JOIN_POLICY_06, kPacket_.m_nOK, 7 ); // 길드가입 승인방법 변경시 길드정보 없음 
        ERR_CASE( ERR_GUILD_JOIN_POLICY_07, kPacket_.m_nOK, 8 ); // 길드가입 승인방법 변경시 DB 에러
        ERR_CASE( ERR_GUILD_JOIN_POLICY_08, kPacket_.m_nOK, 9 ); // 길드가입 승인방법 변경시 길드원수가 최대치를 넘어 변경할 수 없음.

        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUID )
        << BUILD_LOGc( kPacket_.m_ucMethod ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CHANGE_GUILD_JOIN_SETTING_ACK );
}

// TODO:woosh  url 형식 확인 가능한가
_IMPL_ON_FUNC( EVENT_EDIT_GUILD_URL_REQ, KNGuildURL )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_CHANNEL ));

    SET_ERROR( ERR_UNKNOWN );
    KNGuildURL kPacket;

    KGuildPtr spGuild;
    // 패킷으로 온 길드 UID 로 검색되는 길드가 존재하는가
    spGuild = SiKGuildManager()->GetGuild( kPacket_.m_dwUID );
    if( !spGuild ) {
        SET_ERR_GOTO( ERR_GUILD_EDIT_URL_00, END_PROC );
    }

    int nGuildURLMinSize = SiKGuildManager()->GetGuildURLMinSize(); // 길드명 최소사이즈
    int nGuildURLMaxSize = SiKGuildManager()->GetGuildURLMaxSize(); // 길드명 최대사이즈

    // 길드 URL 길이 조건 체크
    if ( kPacket_.m_strURL.size() < (UINT)nGuildURLMinSize ||
        kPacket_.m_strURL.size() > (UINT)nGuildURLMaxSize ) {
        SET_ERR_GOTO( ERR_GUILD_EDIT_URL_01, END_PROC );
    }

    if( SiKSlangFilter()->IsInQueryInvalidChar( kPacket_.m_strURL ) ) {
        SET_ERR_GOTO( ERR_GUILD_EDIT_URL_07, END_PROC );
    }

    // 길드원이 아니다.
    if ( m_kGuildUserInfo.m_dwGuildUID == 0 ) {
        SET_ERR_GOTO( ERR_GUILD_EDIT_URL_02, END_PROC );
    }

    // 길드마스터가 아님(권한 없음)
    if ( m_kGuildUserInfo.m_cMemberLevel != KNGuildUserInfo::GL_MASTER ) {
        SET_ERR_GOTO( ERR_GUILD_EDIT_URL_03, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_EDIT_GUILD_URL_REQ, kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:

    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_EDIT_URL_00, kPacket.m_nOK, 1 ); // 길드 URL 변경시 길드가 존재하지 않음
        ERR_CASE( ERR_GUILD_EDIT_URL_01, kPacket.m_nOK, 2 ); // 길드 URL 변경시 URL 길이 에러
        ERR_CASE( ERR_GUILD_EDIT_URL_02, kPacket.m_nOK, 3 ); // 길드 URL 변경시 길드원이 아님
        ERR_CASE( ERR_GUILD_EDIT_URL_03, kPacket.m_nOK, 4 ); // 길드 URL 변경시 길드마스터가 아님
        ERR_CASE( ERR_GUILD_EDIT_URL_04, kPacket.m_nOK, 5 ); // 길드 URL 변경시 유저 정보 없음
        ERR_CASE( ERR_GUILD_EDIT_URL_05, kPacket.m_nOK, 6 ); // 길드 URL 변경시 길드 정보 없음
        ERR_CASE( ERR_GUILD_EDIT_URL_06, kPacket.m_nOK, 7 ); // 길드 URL 변경시 DB처리 에러.
        ERR_CASE( ERR_GUILD_EDIT_URL_07, kPacket.m_nOK, 8 ); // 길드 URL 변경시 특수문자 포함됨.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
        default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_EDIT_GUILD_URL_ACK );
    }

     _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
         << BUILD_LOG( m_strName )
         << BUILD_LOG( kPacket_.m_strURL )
         << BUILD_LOG( kPacket.m_nOK ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_EDIT_GUILD_URL_ACK, KNGuildURL )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK ) )
        goto END_PROC;

    LIF( SiKGuildManager()->ChangeURLToCenter( kPacket_ ) );

END_PROC:

    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_EDIT_URL_00, kPacket_.m_nOK, 1 ); // 길드 URL 변경시 길드가 존재하지 않음
        ERR_CASE( ERR_GUILD_EDIT_URL_01, kPacket_.m_nOK, 2 ); // 길드 URL 변경시 URL 길이 에러
        ERR_CASE( ERR_GUILD_EDIT_URL_02, kPacket_.m_nOK, 3 ); // 길드 URL 변경시 길드원이 아님
        ERR_CASE( ERR_GUILD_EDIT_URL_03, kPacket_.m_nOK, 4 ); // 길드 URL 변경시 길드마스터가 아님
        ERR_CASE( ERR_GUILD_EDIT_URL_04, kPacket_.m_nOK, 5 ); // 길드 URL 변경시 유저 정보 없음
        ERR_CASE( ERR_GUILD_EDIT_URL_05, kPacket_.m_nOK, 6 ); // 길드 URL 변경시 길드 정보 없음
        ERR_CASE( ERR_GUILD_EDIT_URL_06, kPacket_.m_nOK, 7 ); // 길드 URL 변경시 DB처리 에러.
        ERR_CASE( ERR_GUILD_EDIT_URL_07, kPacket_.m_nOK, 8 ); // 길드 URL 변경시 특수문자 포함됨.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 아직 처리중인 작업입니다.
        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_strURL ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_EDIT_GUILD_URL_ACK );
}


IMPL_ON_FUNC( EVENT_BREAKUP_GUILD_REQ )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_CHANNEL ));

    SET_ERROR( ERR_UNKNOWN );

    KECN_BREAKUP_GUILD_REQ kCnPacket;
    kCnPacket.m_dwUserUID = GetUID();
    kCnPacket.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;

    KEVENT_BREAKUP_GUILD_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwUserUID = GetUID();
    kPacket.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;

    // 길드원이 아니다.
    if ( m_kGuildUserInfo.m_dwGuildUID == 0 ) {
        SET_ERR_GOTO( ERR_GUILD_BREAKUP_00, END_PROC );
    }
    kPacket_.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;

    // 길드마스터가 아님(권한 없음)
    if ( m_kGuildUserInfo.m_cMemberLevel != KNGuildUserInfo::GL_MASTER ) {
        SET_ERR_GOTO( ERR_GUILD_BREAKUP_01, END_PROC );
    }

    // 센터서버로 전달
    SiKCnConnector()->SendPacket( KCenterEvent::ECN_BREAKUP_GUILD_REQ, kCnPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_BREAKUP_00, kPacket.m_nOK, 1 ); // 길드해산시 길드원이 아님.
        ERR_CASE( ERR_GUILD_BREAKUP_01, kPacket.m_nOK, 2 ); // 길드해산시 길드마스터가 아님.
        ERR_CASE( ERR_GUILD_BREAKUP_02, kPacket.m_nOK, 3 ); // 길드해산시 유저정보 없음.
        ERR_CASE( ERR_GUILD_BREAKUP_03, kPacket.m_nOK, 4 ); // 길드해산시 길드정보 없음.
        ERR_CASE( ERR_GUILD_BREAKUP_04, kPacket.m_nOK, 5 ); // 길드해산시 길드마스터가 아님.
        ERR_CASE( ERR_GUILD_BREAKUP_05, kPacket.m_nOK, 6 ); // 길드해산시 DB처리 에러.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_dwUserUID )
        << BUILD_LOG( kPacket.m_dwGuildUID )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_BREAKUP_GUILD_ACK );
    }
}

IMPL_ON_FUNC( EVENT_BREAKUP_GUILD_ACK )
{
    // 길드해체 결과를 센터서버로부터 받음.
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 여기서부터 길드해체 성공
    // 내 길드원 정보 초가화
    m_kGuildUserInfo.Init();

    // 로컬에서 해당 길드 우선 지워줄까?

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_BREAKUP_00, kPacket_.m_nOK, 1 ); // 길드해산시 길드원이 아님.
        ERR_CASE( ERR_GUILD_BREAKUP_01, kPacket_.m_nOK, 2 ); // 길드해산시 길드마스터가 아님.
        ERR_CASE( ERR_GUILD_BREAKUP_02, kPacket_.m_nOK, 3 ); // 길드해산시 유저정보 없음.
        ERR_CASE( ERR_GUILD_BREAKUP_03, kPacket_.m_nOK, 4 ); // 길드해산시 길드정보 없음.
        ERR_CASE( ERR_GUILD_BREAKUP_04, kPacket_.m_nOK, 5 ); // 길드해산시 길드마스터가 아님.
        ERR_CASE( ERR_GUILD_BREAKUP_05, kPacket_.m_nOK, 6 ); // 길드해산시 DB처리 에러.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_nOK ) << END_LOG;

   SEND_RECEIVED_PACKET( EVENT_BREAKUP_GUILD_ACK );
}

IMPL_ON_FUNC( EVENT_CHANGE_GUILD_MEMBER_LEVEL_REQ )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_CHANNEL ));

    // 길드 맴버레벨 변경 요청
    // 길드마스터만 가능.
    SET_ERROR( ERR_UNKNOWN );

    // map< 변경할 맴버레벨, set<UserUID> >
    std::map< char, std::set<DWORD> >::iterator mit;
    std::set<DWORD>::iterator sit;

    KECN_CHANGE_GUILD_MEMBER_LEVEL_REQ kCnPacket;
    kCnPacket.m_dwGuildMasterUID = GetUID();
    kCnPacket.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;
    kCnPacket.m_mapChangeMemberLevel = kPacket_.m_mapChangeMemberLevel;

    KEVENT_CHANGE_GUILD_MEMBER_LEVEL_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;
    kPacket.m_dwGuildMasterUID = 0;
    kPacket.m_mapChangeMemberLevel.clear();

    // 변경 요청할 정보가 비어있다.
    if ( kPacket_.m_mapChangeMemberLevel.empty() ) {
        SET_ERR_GOTO( ERR_GUILD_CHANGE_MEMBER_LEVEL_00, END_PROC );
    }

    // 길드원이 아니다.
    if ( m_kGuildUserInfo.m_dwGuildUID == 0 ) {
        SET_ERR_GOTO( ERR_GUILD_CHANGE_MEMBER_LEVEL_01, END_PROC );
    }

    // 길드마스터가 아님
    if ( m_kGuildUserInfo.m_cMemberLevel != KNGuildUserInfo::GL_MASTER ) {
        SET_ERR_GOTO( ERR_GUILD_CHANGE_MEMBER_LEVEL_05, END_PROC );
    }

    // 길드마스터UID/길드UID 설정
    kPacket_.m_dwGuildMasterUID = GetUID();
    kPacket_.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;

    // 데이터 검사 (길드마스터 위임)
    mit = kPacket_.m_mapChangeMemberLevel.find( KNGuildUserInfo::GL_MASTER );
    if ( mit != kPacket_.m_mapChangeMemberLevel.end() ) {

        if ( mit->second.size() > 1 ) {
            START_LOG( cerr, L"길드마스터 위임은 한번에 한명씩만 가능. size : " << mit->second.size() ) << END_LOG;
            SET_ERR_GOTO( ERR_GUILD_CHANGE_MEMBER_LEVEL_02, END_PROC );
        }
    }

    // 데이터 검사 (길드원 탈퇴)
    mit = kPacket_.m_mapChangeMemberLevel.find( KNGuildUserInfo::GL_DRUMOUT );
    if ( mit != kPacket_.m_mapChangeMemberLevel.end() ) {

        sit = mit->second.find( kPacket_.m_dwGuildMasterUID );
        if ( sit != mit->second.end() ) {
            START_LOG( cerr, L"길드마스터는 변경 대상이 아니다. MasterUID : " << kPacket_.m_dwGuildMasterUID ) << END_LOG;
            SET_ERR_GOTO( ERR_GUILD_CHANGE_MEMBER_LEVEL_09, END_PROC );
        }
    }

    // 데이터 검사 (제너럴 위임)
    mit = kPacket_.m_mapChangeMemberLevel.find( KNGuildUserInfo::GL_GENERAL );
    if ( mit != kPacket_.m_mapChangeMemberLevel.end() ) {

        sit = mit->second.find( kPacket_.m_dwGuildMasterUID );
        if ( sit != mit->second.end() ) {
            START_LOG( cerr, L"길드마스터는 변경 대상이 아니다. MasterUID : " << kPacket_.m_dwGuildMasterUID ) << END_LOG;
            SET_ERR_GOTO( ERR_GUILD_CHANGE_MEMBER_LEVEL_09, END_PROC );
        }
    }

    // 데이터 검사 (일반길드원으로 강등)
    mit = kPacket_.m_mapChangeMemberLevel.find( KNGuildUserInfo::GL_NORMAL );
    if ( mit != kPacket_.m_mapChangeMemberLevel.end() ) {

        sit = mit->second.find( kPacket_.m_dwGuildMasterUID );
        if ( sit != mit->second.end() ) {
            START_LOG( cerr, L"길드마스터는 변경 대상이 아니다. MasterUID : " << kPacket_.m_dwGuildMasterUID ) << END_LOG;
            SET_ERR_GOTO( ERR_GUILD_CHANGE_MEMBER_LEVEL_09, END_PROC );
        }
    }

    // 센터서버로 전달
    SiKCnConnector()->SendPacket( KCenterEvent::ECN_CHANGE_GUILD_MEMBER_LEVEL_REQ, kCnPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_GUILD_CHANGE_MEMBER_LEVEL_00, kPacket.m_nOK, 1 );    // 길드맴버레벨 변경요청시 정보가 비어있음.
        ERR_CASE( ERR_GUILD_CHANGE_MEMBER_LEVEL_01, kPacket.m_nOK, 2 );    // 길드맴버레벨 변경요청시 길드원이 아니다.
        ERR_CASE( ERR_GUILD_CHANGE_MEMBER_LEVEL_02, kPacket.m_nOK, 3 );    // 길드맴버레벨 변경요청시 길드마스터 위임은 한번에 한명씩만 가능.
        ERR_CASE( ERR_GUILD_CHANGE_MEMBER_LEVEL_03, kPacket.m_nOK, 4 );    // 길드맴버레벨 변경요청시 유저정보 없음.
        ERR_CASE( ERR_GUILD_CHANGE_MEMBER_LEVEL_04, kPacket.m_nOK, 5 );    // 길드맴버레벨 변경요청시 길드정보 없음.
        ERR_CASE( ERR_GUILD_CHANGE_MEMBER_LEVEL_05, kPacket.m_nOK, 6 );    // 길드맴버레벨 변경요청시 길드마스터가 아님.
        ERR_CASE( ERR_GUILD_CHANGE_MEMBER_LEVEL_06, kPacket.m_nOK, 7 );    // 길드맴버레벨 변경요청시 변경대상이 일반길드원이나 제너럴이 아님.
        ERR_CASE( ERR_GUILD_CHANGE_MEMBER_LEVEL_07, kPacket.m_nOK, 8 );    // 길드맴버레벨 변경요청시 입력인자의 등급이 잘못되었음.
        ERR_CASE( ERR_GUILD_CHANGE_MEMBER_LEVEL_08, kPacket.m_nOK, 9 );    // 길드맴버레벨 변경요청시 DB처리 오류.
        ERR_CASE( ERR_GUILD_CHANGE_MEMBER_LEVEL_09, kPacket.m_nOK, 10 );   // 길드맴버레벨 변경요청시 길드마스터를 탈퇴시킬수 없음.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_mapChangeMemberLevel.size() ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_CHANGE_GUILD_MEMBER_LEVEL_ACK );
    }
}

IMPL_ON_FUNC( EVENT_CHANGE_GUILD_MEMBER_LEVEL_ACK )
{
    // 센터서버로부터 길드원 맴버레벨 변경요청에 대한 결과값 받음.
    NetError::SetLastNetError( kPacket_.m_nOK );

    std::map< char, std::set<DWORD> >::iterator mit;

    // 클라이언트에 보낼 패킷가공
    std::map< char, std::set<DWORD> >::const_iterator cmit;
    std::set<DWORD>::const_iterator csit;
    KChangeGuildMemberInfo kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;

    for ( cmit = kPacket_.m_mapChangeMemberLevel.begin() ; cmit != kPacket_.m_mapChangeMemberLevel.end() ; ++cmit ) {
        for ( csit = cmit->second.begin() ; csit != cmit->second.end() ; ++csit ) {
            kPacket.m_vecChangeMemberLevel.push_back( std::make_pair( *csit, cmit->first ) );
        }
    }
    START_LOG( clog, L"클라이언트에 전달할 패킷 만듬. size : " << kPacket.m_vecChangeMemberLevel.size() ) << END_LOG;

    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 여기서부터는 길드원 맴버레벨 변경 성공
    // 길드매니저에 바뀐 길드원 정보들 적용하고,
    // 센터길드매니저에도 알려서 다른 게임서버에도 알려줘야한다.

    // 탈퇴처리
    // 센터서버와 패킷을 주고받는 처리 부분들에 대한 수정이 필요하다.
    // 로컬에서 변경후 중앙으로 보내는 것을
    // 중앙에서 변경하여 로컬들에게 보내는 방식으로 바뀌었기 때문이다.
    mit = kPacket_.m_mapChangeMemberLevel.find( KNGuildUserInfo::GL_DRUMOUT );
    if ( mit != kPacket_.m_mapChangeMemberLevel.end() ) {
        SiKGuildManager()->DrumoutUser( kPacket_.m_dwGuildUID, mit->second, KNGuildUserInfo::OT_DRUMOUT );
        START_LOG( clog, L"길드매니저에 길드원 강제 탈퇴 알림. size : " << mit->second.size() ) << END_LOG;
    }

    // 탈퇴외 맴버레벨 처리
    // 반드시 DB스레드에서 키(맴버레벨)별로 초기화 시킨다.
    if ( !kPacket_.m_mapChangeMemberLevel[KNGuildUserInfo::GL_NORMAL].empty() ||
         !kPacket_.m_mapChangeMemberLevel[KNGuildUserInfo::GL_GENERAL].empty() ||
         !kPacket_.m_mapChangeMemberLevel[KNGuildUserInfo::GL_MASTER].empty() ) {

        SiKGuildManager()->ChangeGuildMemberLevel( kPacket_.m_dwGuildUID, kPacket_.m_mapChangeMemberLevel );
        START_LOG( clog, L"길드매니저에 길드원 맴버레벨 변경 알림. " )
            << BUILD_LOG( kPacket_.m_mapChangeMemberLevel[KNGuildUserInfo::GL_NORMAL].size() )
            << BUILD_LOG( kPacket_.m_mapChangeMemberLevel[KNGuildUserInfo::GL_GENERAL].size() )
            << BUILD_LOG( kPacket_.m_mapChangeMemberLevel[KNGuildUserInfo::GL_MASTER].size() ) << END_LOG;
    }

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_CHANGE_MEMBER_LEVEL_00, kPacket.m_nOK, 1 );    // 길드맴버레벨 변경요청시 정보가 비어있음.
        ERR_CASE( ERR_GUILD_CHANGE_MEMBER_LEVEL_01, kPacket.m_nOK, 2 );    // 길드맴버레벨 변경요청시 길드원이 아니다.
        ERR_CASE( ERR_GUILD_CHANGE_MEMBER_LEVEL_02, kPacket.m_nOK, 3 );    // 길드맴버레벨 변경요청시 길드마스터 위임은 한번에 한명씩만 가능.
        ERR_CASE( ERR_GUILD_CHANGE_MEMBER_LEVEL_03, kPacket.m_nOK, 4 );    // 길드맴버레벨 변경요청시 유저정보 없음.
        ERR_CASE( ERR_GUILD_CHANGE_MEMBER_LEVEL_04, kPacket.m_nOK, 5 );    // 길드맴버레벨 변경요청시 길드정보 없음.
        ERR_CASE( ERR_GUILD_CHANGE_MEMBER_LEVEL_05, kPacket.m_nOK, 6 );    // 길드맴버레벨 변경요청시 길드마스터가 아님.
        ERR_CASE( ERR_GUILD_CHANGE_MEMBER_LEVEL_06, kPacket.m_nOK, 7 );    // 길드맴버레벨 변경요청시 변경대상이 일반길드원이나 제너럴이 아님.
        ERR_CASE( ERR_GUILD_CHANGE_MEMBER_LEVEL_07, kPacket.m_nOK, 8 );    // 길드맴버레벨 변경요청시 입력인자의 등급이 잘못되었음.
        ERR_CASE( ERR_GUILD_CHANGE_MEMBER_LEVEL_08, kPacket.m_nOK, 9 );    // 길드맴버레벨 변경요청시 DB처리 오류.
        ERR_CASE( ERR_GUILD_CHANGE_MEMBER_LEVEL_09, kPacket.m_nOK, 10 );   // 길드맴버레벨 변경요청시 길드마스터를 탈퇴시킬수 없음.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwGuildUID )
        << BUILD_LOG( kPacket.m_vecChangeMemberLevel.size() )
        << BUILD_LOG( kPacket_.m_mapChangeMemberLevel.size() ) << END_LOG;

    SEND_PACKET( EVENT_CHANGE_GUILD_MEMBER_LEVEL_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_SELF_DRUMOUT_GUILD_REQ )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_CHANNEL ));

    SET_ERROR( ERR_UNKNOWN );
    // 길드탈퇴 신청.
    KEVENT_SELF_DRUMOUT_GUILD_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;

    PAIR_DWORD kCnPacket;
    kCnPacket.first = GetUID();
    kCnPacket.second = m_kGuildUserInfo.m_dwGuildUID;

    if ( kPacket.m_dwGuildUID == 0 ) {
        kPacket.m_nOK = 0;
        m_kGuildUserInfo.Init();

        SEND_PACKET( EVENT_SELF_DRUMOUT_GUILD_ACK );
        START_LOG( cwarn, L"길드원이 아닌데 탈퇴 신청. 성공으로 처리. Login : " << m_strName ) << END_LOG;
        return;
    }

    if ( m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_MASTER ) {
        START_LOG( cerr, L"길드마스터는 탈퇴할 수 없다. Login : " << GetName() )
            << BUILD_LOGc( m_kGuildUserInfo.m_dwGuildUID )
            << BUILD_LOGc( m_kGuildUserInfo.m_cMemberLevel ) << END_LOG;
        SET_ERR_GOTO( ERR_GUILD_SELF_DRUMOUT_04, END_PROC );
    }

    // 센터서버로 전달
    SiKCnConnector()->SendPacket( KCenterEvent::ECN_SELF_DRUMOUT_GUILD_REQ, kCnPacket );
    START_LOG( clog, L"길드 탈퇴 신청. Login : " << m_strName << L", GuildUID : " << kPacket.m_dwGuildUID ) << END_LOG;
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                    kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_SELF_DRUMOUT_00, kPacket.m_nOK, 1 ); // 유저정보 없음
        ERR_CASE( ERR_GUILD_SELF_DRUMOUT_01, kPacket.m_nOK, 2 ); // 길드정보 없음
        ERR_CASE( ERR_GUILD_SELF_DRUMOUT_02, kPacket.m_nOK, 3 ); // 탈퇴 등급 아님(길마 혹은 DB에 등록되지 않은 등급)
        ERR_CASE( ERR_GUILD_SELF_DRUMOUT_03, kPacket.m_nOK, 4 ); // DB처리 에러
        ERR_CASE( ERR_GUILD_SELF_DRUMOUT_04, kPacket.m_nOK, 5 ); // 길드마스터는 탈퇴할 수 없음.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwGuildUID ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_SELF_DRUMOUT_GUILD_ACK );
    }
}

IMPL_ON_FUNC( EVENT_SELF_DRUMOUT_GUILD_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    std::set<DWORD> setUID;

    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 여기서부터는 길드탈퇴 성공
    // 내 길드정보 초기화하고,
    m_kGuildUserInfo.Init();

    // 길드매니저랑 길드에서 내 정보 제거/탈퇴했다는 것에대한 처리
    // 탈퇴처리되면 센터서버에도 알려서 다른 게임서버에도 알려줌.
    setUID.insert( GetUID() );
    SiKGuildManager()->DrumoutUser( kPacket_.m_dwGuildUID, setUID, KNGuildUserInfo::OT_SELF_OUT );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                    kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_SELF_DRUMOUT_00, kPacket_.m_nOK, 1 ); // 유저정보 없음
        ERR_CASE( ERR_GUILD_SELF_DRUMOUT_01, kPacket_.m_nOK, 2 ); // 길드정보 없음
        ERR_CASE( ERR_GUILD_SELF_DRUMOUT_02, kPacket_.m_nOK, 3 ); // 탈퇴 등급 아님(길마 혹은 DB에 등록되지 않은 등급)
        ERR_CASE( ERR_GUILD_SELF_DRUMOUT_03, kPacket_.m_nOK, 4 ); // DB처리 에러
        ERR_CASE( ERR_GUILD_SELF_DRUMOUT_04, kPacket_.m_nOK, 5 ); // 길드마스터는 탈퇴할 수 없음.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SELF_DRUMOUT_GUILD_ACK );
}

IMPL_ON_FUNC( EVENT_ACCEPT_GUILD_JOINER_REQ )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_CHANNEL ));

    SET_ERROR( ERR_UNKNOWN );
    // 길드원 가입 승인요청
    // 제너럴과 길드마스터만 요청 가능하다.

    KECN_ACCEPT_GUILD_JOINER_REQ kCnPacket;
    kCnPacket.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;
    kCnPacket.m_dwUserUID = GetUID();
    kCnPacket.m_setUserUID = kPacket_.m_setUserUID;

    KEVENT_ACCEPT_GUILD_JOINER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;
    kPacket.m_setUserUID = kPacket_.m_setUserUID;

    // 데이터가 비어있음.
    if ( kPacket_.m_setUserUID.empty() ) {
        SET_ERR_GOTO( ERR_GUILD_ACCEPT_JOINER_00, END_PROC );
    }

    // 길드원이 아니다.
    if ( m_kGuildUserInfo.m_dwGuildUID == 0 ) {
        SET_ERR_GOTO( ERR_GUILD_ACCEPT_JOINER_01, END_PROC );
    }
    // 길드설정
    kPacket_.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;

    // 길드마스터나 제너럴이 아님(권한 없음)
    if ( m_kGuildUserInfo.m_cMemberLevel != KNGuildUserInfo::GL_MASTER &&
         m_kGuildUserInfo.m_cMemberLevel != KNGuildUserInfo::GL_GENERAL ) {
        SET_ERR_GOTO( ERR_GUILD_ACCEPT_JOINER_04, END_PROC );
    }

    // 센터서버로 전달(전송실패시의 에러처리도 추가해야한다. 이를테면 센터서버와의 응답이 없습니다.)
    SiKCnConnector()->SendPacket( KCenterEvent::ECN_ACCEPT_GUILD_JOINER_REQ, kCnPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                     kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_ACCEPT_JOINER_00, kPacket.m_nOK, 1 ); // 길드가입승인시 정보가 비어있음.
        ERR_CASE( ERR_GUILD_ACCEPT_JOINER_01, kPacket.m_nOK, 2 ); // 길드가입승인시 길드원이 아님.
        ERR_CASE( ERR_GUILD_ACCEPT_JOINER_02, kPacket.m_nOK, 3 ); // 길드가입승인시 유저정보 없음.
        ERR_CASE( ERR_GUILD_ACCEPT_JOINER_03, kPacket.m_nOK, 4 ); // 길드가입승인시 길드정보 없음.
        ERR_CASE( ERR_GUILD_ACCEPT_JOINER_04, kPacket.m_nOK, 5 ); // 길드가입승인시 길드마스터나 제너럴이 아님.
        ERR_CASE( ERR_GUILD_ACCEPT_JOINER_05, kPacket.m_nOK, 6 ); // 길드가입승인시 가입대기길드원이 아님.
        ERR_CASE( ERR_GUILD_ACCEPT_JOINER_06, kPacket.m_nOK, 7 ); // 길드가입승인시 DB처리 에러.
        ERR_CASE( ERR_GUILD_ACCEPT_JOINER_07, kPacket.m_nOK, 8 ); // 길드가입승인시 가입중지 상태.
        ERR_CASE( ERR_GUILD_ACCEPT_JOINER_08, kPacket.m_nOK, 9 ); // 길드가입승인시 최대 맴버수를 넘게된다.

    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwGuildUID )
        << BUILD_LOG( kPacket.m_setUserUID.size() ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_ACCEPT_GUILD_JOINER_ACK );
    }
}

IMPL_ON_FUNC( EVENT_ACCEPT_GUILD_JOINER_ACK )
{
    // 센터서버로부터 가입승인에 대한 결과 받음.
    NetError::SetLastNetError( kPacket_.m_nOK );

    // 가입승인된 유저정보 map< MemberLevel, set<UserUID> >
    std::map<char,std::set<DWORD> > mapAcceptJoiner;

    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 여기서부터는 길드가입승인 성공
    // 길드맴버변경의 일반길드원 변경 처리와 동일하게 처리된다.
    mapAcceptJoiner[ KNGuildUserInfo::GL_NORMAL ] = kPacket_.m_setUserUID;

    // 승인된 유저들의 맴버레벨을 일반길드원으로 변경 요청.
    SiKGuildManager()->ChangeGuildMemberLevel( kPacket_.m_dwGuildUID, mapAcceptJoiner );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                     kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_ACCEPT_JOINER_00, kPacket_.m_nOK, 1 ); // 길드가입승인시 정보가 비어있음.
        ERR_CASE( ERR_GUILD_ACCEPT_JOINER_01, kPacket_.m_nOK, 2 ); // 길드가입승인시 길드원이 아님.
        ERR_CASE( ERR_GUILD_ACCEPT_JOINER_02, kPacket_.m_nOK, 3 ); // 길드가입승인시 유저정보 없음.
        ERR_CASE( ERR_GUILD_ACCEPT_JOINER_03, kPacket_.m_nOK, 4 ); // 길드가입승인시 길드정보 없음.
        ERR_CASE( ERR_GUILD_ACCEPT_JOINER_04, kPacket_.m_nOK, 5 ); // 길드가입승인시 길드마스터나 제너럴이 아님.
        ERR_CASE( ERR_GUILD_ACCEPT_JOINER_05, kPacket_.m_nOK, 6 ); // 길드가입승인시 가입대기길드원이 아님.
        ERR_CASE( ERR_GUILD_ACCEPT_JOINER_06, kPacket_.m_nOK, 7 ); // 길드가입승인시 DB처리 에러.
		ERR_CASE( ERR_GUILD_ACCEPT_JOINER_07, kPacket_.m_nOK, 8 ); // 길드가입승인시 가입중지 상태.
		ERR_CASE( ERR_GUILD_ACCEPT_JOINER_08, kPacket_.m_nOK, 9 ); // 길드가입승인시 최대 맴버수를 넘게된다.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_setUserUID.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_ACCEPT_GUILD_JOINER_ACK );
}

IMPL_ON_FUNC( EVENT_REJECT_GUILD_JOINER_REQ )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_CHANNEL ));

    // 길드가입거절시 불려진다.
    SET_ERROR( ERR_UNKNOWN );

    KECN_REJECT_GUILD_JOINER_REQ kCnPacket;
    kCnPacket.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;
    kCnPacket.m_dwUserUID = GetUID();
    kCnPacket.m_setUserUID = kPacket_.m_setUserUID;

    KEVENT_REJECT_GUILD_JOINER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;
    kPacket.m_setUserUID.clear();

    // 데이터가 비어있음.
    if ( kPacket_.m_setUserUID.empty() ) {
        SET_ERR_GOTO( ERR_GUILD_REJECT_JOINER_00, END_PROC );
    }

    // 길드원이 아니다.
    if ( m_kGuildUserInfo.m_dwGuildUID == 0 ) {
        SET_ERR_GOTO( ERR_GUILD_REJECT_JOINER_01, END_PROC );
    }
    kPacket_.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;

    // 길드마스터나 제너럴이 아님(권한 없음)
    if ( m_kGuildUserInfo.m_cMemberLevel != KNGuildUserInfo::GL_MASTER &&
         m_kGuildUserInfo.m_cMemberLevel != KNGuildUserInfo::GL_GENERAL ) {
            SET_ERR_GOTO( ERR_GUILD_REJECT_JOINER_04, END_PROC );
    }

    // 센터서버로 전달(전송실패시의 에러처리도 추가해야한다. 이를테면 센터서버와의 응답이 없습니다.)
    SiKCnConnector()->SendPacket( KCenterEvent::ECN_REJECT_GUILD_JOINER_REQ, kCnPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                     kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_REJECT_JOINER_00, kPacket.m_nOK, 1 ); // 길드가입거절시 정보가 비어있음.
        ERR_CASE( ERR_GUILD_REJECT_JOINER_01, kPacket.m_nOK, 2 ); // 길드가입거절시 길드원이 아님.
        ERR_CASE( ERR_GUILD_REJECT_JOINER_02, kPacket.m_nOK, 3 ); // 길드가입거절시 유저정보 없음.
        ERR_CASE( ERR_GUILD_REJECT_JOINER_03, kPacket.m_nOK, 4 ); // 길드가입거절시 길드정보 없음.
        ERR_CASE( ERR_GUILD_REJECT_JOINER_04, kPacket.m_nOK, 5 ); // 길드가입거절시 길드마스터가 아님.
        ERR_CASE( ERR_GUILD_REJECT_JOINER_05, kPacket.m_nOK, 6 ); // 길드가입거절시 가입대기 또는 일반길드원이나 제너럴이 아님.
        ERR_CASE( ERR_GUILD_REJECT_JOINER_06, kPacket.m_nOK, 7 ); // 길드가입거절시 DB처리 에러.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwGuildUID )
        << BUILD_LOG( kPacket.m_setUserUID.size() ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_REJECT_GUILD_JOINER_ACK );
    }
}

IMPL_ON_FUNC( EVENT_REJECT_GUILD_JOINER_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 여기서부터는 길드가입거절 성공
    // 길드매니저의 탈퇴처리와 동일하게 처리된다.
    SiKGuildManager()->DrumoutUser( kPacket_.m_dwGuildUID, kPacket_.m_setUserUID, KNGuildUserInfo::OT_REJECT_JOIN );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                     kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_REJECT_JOINER_00, kPacket_.m_nOK, 1 ); // 길드가입거절시 정보가 비어있음.
        ERR_CASE( ERR_GUILD_REJECT_JOINER_01, kPacket_.m_nOK, 2 ); // 길드가입거절시 길드원이 아님.
        ERR_CASE( ERR_GUILD_REJECT_JOINER_02, kPacket_.m_nOK, 3 ); // 길드가입거절시 유저정보 없음.
        ERR_CASE( ERR_GUILD_REJECT_JOINER_03, kPacket_.m_nOK, 4 ); // 길드가입거절시 길드정보 없음.
        ERR_CASE( ERR_GUILD_REJECT_JOINER_04, kPacket_.m_nOK, 5 ); // 길드가입거절시 길드마스터가 아님.
        ERR_CASE( ERR_GUILD_REJECT_JOINER_05, kPacket_.m_nOK, 6 ); // 길드가입거절시 가입대기 또는 일반길드원이나 제너럴이 아님.
        ERR_CASE( ERR_GUILD_REJECT_JOINER_06, kPacket_.m_nOK, 7 ); // 길드가입거절시 DB처리 에러.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_setUserUID.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_REJECT_GUILD_JOINER_ACK );
}

IMPL_ON_FUNC( EVENT_MARK_UPLOAD_REQ )
{
    KEVENT_MARK_UPLOAD_ACK kPacket;
    KNGuildUserInfo kGuildUserInfo;
    KNGuildInfo kGuildInfo;
    KGuildPtr spGuild;
    KItemPtr pkItem;
    GCITEMID dwMarkItemID = SiKGuildManager()->GetGuildMarkItemID();
    GCITEMID dwMarkCashItemID = SiKGuildManager()->GetGuildMarkCashItemID();

    SET_ERROR( ERR_UNKNOWN );

    // 이 유저가 길드 가입 유저 인가?
    GetGuildUserInfo( kGuildUserInfo );
    kPacket_.m_dwGuildUID = kGuildUserInfo.m_dwGuildUID;

    if ( kGuildUserInfo.m_dwGuildUID <= 0 ) {
        SET_ERR_GOTO( ERR_GUILD_MARK_00, END_PROC );
    }

    // 길드에서 권한이 되는가?
    if ( kGuildUserInfo.m_cMemberLevel != KNGuildUserInfo::GL_MASTER ) {
        SET_ERR_GOTO( ERR_GUILD_MARK_01, END_PROC );
    }

    spGuild = SiKGuildManager()->GetGuild( kPacket_.m_dwGuildUID );
    if( !spGuild ) {
        SET_ERR_GOTO( ERR_GUILD_MARK_02, END_PROC );
    }

    // 업로드 가능한 이미지 인가?
    boost::to_lower( kPacket_.m_strFileExt );
    if ( !SiKGuildManager()->CheckGuildMarkExt( kPacket_.m_strFileExt ) ) {
        SET_ERR_GOTO( ERR_GUILD_MARK_04, END_PROC );
    }
    // 이미지는 비어 있지 않은가?
    if ( SiKGuildManager()->CheckGuildMarkSize( kPacket_.m_buffImage.GetLength() ) ) {
        SET_ERR_GOTO( ERR_GUILD_MARK_05, END_PROC );
    }

    // 1.업로드 아이템(일반)은 존재 하는가?
    pkItem = m_kInventory.FindItemByItemID( dwMarkItemID );
    if ( pkItem == NULL ) {
        // 2.업로드 아이템(캐쉬)는 있는가?
        pkItem = m_kInventory.FindItemByItemID( dwMarkCashItemID );
        if ( pkItem == NULL ) {
            START_LOG( cerr, L"길드마크 업로드 아이템이 존재하지 않음." )
                << BUILD_LOG( dwMarkItemID )
                << BUILD_LOG( dwMarkCashItemID ) << END_LOG;

            SET_ERR_GOTO( ERR_GUILD_MARK_06, END_PROC );
        }
    }

    kPacket_.m_kItem  = *pkItem;
    kPacket_.m_dwGuildUID   = kGuildUserInfo.m_dwGuildUID;
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_MARK_UPLOAD_REQ, kPacket_ );

    SET_ERROR( NET_OK );
END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_GUILD_MARK_00,    kPacket.m_nOK, 1 );    // 해당 유저는 길드에 가입 되어 있지 않습니다.
        ERR_CASE( ERR_GUILD_MARK_01,    kPacket.m_nOK, 2 );    // 해당 유저는 길드 마크를 변경할 권한이 없습니다.
        ERR_CASE( ERR_GUILD_MARK_02,    kPacket.m_nOK, 3 );    // 길드가 존재하지 않습니다.
        ERR_CASE( ERR_GUILD_MARK_03,    kPacket.m_nOK, 4 );    // 해당 길드는 정규 길드가 아닙니다.
        ERR_CASE( ERR_GUILD_MARK_04,    kPacket.m_nOK, 5 );    // 길드 마크로 사용 할수 있는 이미지가 아닙니다.
        ERR_CASE( ERR_GUILD_MARK_05,    kPacket.m_nOK, 6 );    // 길드 마크 이미지 용량에 이상이 있습니다.
        ERR_CASE( ERR_GUILD_MARK_06,    kPacket.m_nOK, 7 );    // 길드 마크 업로드 아이템을 보유하고 있지 않습니다.
        ERR_CASE( ERR_GUILD_MARK_07,    kPacket.m_nOK, 8 );    // 길드 마크 정보 가져 오기 실패.
        ERR_CASE( ERR_GUILD_MARK_08,    kPacket.m_nOK, 9 );    // FTP 연결 실패.
        ERR_CASE( ERR_GUILD_MARK_09,    kPacket.m_nOK, 10 );   // FTP 업로드 실패.
        ERR_CASE( ERR_GUILD_MARK_10,    kPacket.m_nOK, 11 );   // 길드 마크를 업로드 할수 있는 상태가 아님
        ERR_CASE( ERR_GUILD_MARK_11,    kPacket.m_nOK, 12 );   // 길드 마크를 상태 변경 실패

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );    // 처리 중.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_strFileExt )
        << BUILD_LOG( kPacket_.m_buffImage.GetLength() )
        << BUILD_LOGc( kGuildInfo.m_ucMarkStatus )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemID )
        << BUILD_LOG( dwMarkItemID )
        << BUILD_LOG( dwMarkCashItemID )
        << BUILD_LOGc( kGuildInfo.m_ucGrade )
        << BUILD_LOG( kGuildInfo.m_nNumMembers ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_MARK_UPLOAD_ACK );
    }
}

IMPL_ON_FUNC( EVENT_MARK_UPLOAD_ACK )
{
    KGuildPtr spGuild;
    PAIR_DWORD_UCHAR kGuildPacket;
    KNGuildInfo kGuildInfo;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 길드 마크 상태를 바꾸자.
    spGuild = SiKGuildManager()->GetGuild( kPacket_.m_dwGuildUID );
    if( spGuild ) {
        kGuildInfo = spGuild->GetNGuildInfo();
        // 어차피 센터 갔다가 오면 업데이트 진행 하니까.. 
        //spGuild->SetGuildMarkStatus( KNGuildInfo::MS_REQUEST_CHANGE );

        kGuildPacket.first = kPacket_.m_dwGuildUID;
        kGuildPacket.second = KNGuildInfo::MS_REQUEST_CHANGE;
        // 센터에도 리포팅 해야 하는데 어떻게 하지?
        SiKCnConnector()->SendPacket( KCenterEvent::ECN_GUILD_MARK_REG_STATUS_REQ, kGuildPacket );
    }

    m_kInventory.RemoveItem( kPacket_.m_kItem.m_ItemID, kPacket_.m_kItem.m_ItemUID );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_GUILD_MARK_00,    kPacket_.m_nOK, 1 );    // 해당 유저는 길드에 가입 되어 있지 않습니다.
        ERR_CASE( ERR_GUILD_MARK_01,    kPacket_.m_nOK, 2 );    // 해당 유저는 길드 마크를 변경할 권한이 없습니다.
        ERR_CASE( ERR_GUILD_MARK_02,    kPacket_.m_nOK, 3 );    // 길드가 존재하지 않습니다.
        ERR_CASE( ERR_GUILD_MARK_03,    kPacket_.m_nOK, 4 );    // 해당 길드는 정규 길드가 아닙니다.
        ERR_CASE( ERR_GUILD_MARK_04,    kPacket_.m_nOK, 5 );    // 길드 마크로 사용 할수 있는 이미지가 아닙니다.
        ERR_CASE( ERR_GUILD_MARK_05,    kPacket_.m_nOK, 6 );    // 길드 마크 이미지 용량에 이상이 있습니다.
        ERR_CASE( ERR_GUILD_MARK_06,    kPacket_.m_nOK, 7 );    // 길드 마크 업로드 아이템을 보유하고 있지 않습니다.
        ERR_CASE( ERR_GUILD_MARK_07,    kPacket_.m_nOK, 8 );    // 길드 마크 정보 가져 오기 실패.
        ERR_CASE( ERR_GUILD_MARK_08,    kPacket_.m_nOK, 9 );    // FTP 연결 실패.
        ERR_CASE( ERR_GUILD_MARK_09,    kPacket_.m_nOK, 10 );   // FTP 업로드 실패.
        ERR_CASE( ERR_GUILD_MARK_10,    kPacket_.m_nOK, 11 );   // 길드 마크를 업로드 할수 있는 상태가 아님

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );    // 처리 중.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOGc( kGuildInfo.m_ucGrade )
        << BUILD_LOG( kGuildInfo.m_nNumMembers ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_MARK_UPLOAD_ACK );
}


IMPL_ON_FUNC( EVENT_EDIT_GUILD_MY_COMMENT_REQ )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_CHANNEL ));

    SET_ERROR( ERR_UNKNOWN );
    // 길드원이 자기소개 변경

    KEVENT_EDIT_GUILD_MY_COMMENT_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    boost::replace_all( kPacket_.m_strMyComment, L"'", L"''" );
    kPacket.m_strMyComment = kPacket_.m_strMyComment;

    if ( m_kGuildUserInfo.m_dwGuildUID == 0 ) {
        // 소속 길드가 없음.
        SET_ERR_GOTO( ERR_GUILD_MODIFY_MYCOMMENT_02, END_PROC );
    }
    kPacket_.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_EDIT_GUILD_MY_COMMENT_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_GUILD_MODIFY_MYCOMMENT_00,    kPacket.m_nOK, 1 );    // 유저정보 없음
        ERR_CASE( ERR_GUILD_MODIFY_MYCOMMENT_01,    kPacket.m_nOK, 2 );    // 길드정보 없음
        ERR_CASE( ERR_GUILD_MODIFY_MYCOMMENT_02,    kPacket.m_nOK, 3 );    // 길드원이 아님
        ERR_CASE( ERR_GUILD_MODIFY_MYCOMMENT_03,    kPacket.m_nOK, 4 );    // DB처리 에러

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwGuildUID )
        << BUILD_LOG( kPacket.m_strMyComment ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_EDIT_GUILD_MY_COMMENT_ACK );
    }
}

IMPL_ON_FUNC( EVENT_EDIT_GUILD_MY_COMMENT_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    boost::replace_all( kPacket_.m_strMyComment, L"''", L"'" );

    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 여기서부터 길드 내 소갯말 변경 성공
    SiKGuildManager()->ModifyMemberComment( kPacket_.m_dwGuildUID, GetUID(), kPacket_.m_strMyComment );

END_PROC:
    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_strMyComment ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_EDIT_GUILD_MY_COMMENT_ACK );
}


_IMPL_ON_FUNC( EVENT_GUILD_RANK_REQ, int )
{
    // 광장에서만..
    // 광장에 접속하는 시점에 클라이언트가 해당 정보를 요청하기 때문에 CHANNEL STATE 추가.
    VERIFY_STATE(( 2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_CHANNEL ));

    KEVENT_GUILD_RANK_ACK kPacket;
    kPacket.m_nType = kPacket_;

    SET_ERROR( ERR_UNKNOWN );
    if ( KNGuildRankInfo::DAILY_RANK > kPacket_ || KNGuildRankInfo::MAX_RANK <= kPacket_ ) {
        SET_ERR_GOTO( ERR_RANK_00, END_PROC );
    }

    SiKGuildManager()->GetGuildRankData( kPacket.m_nType, kPacket.m_buffRank );
    SET_ERROR( NET_OK );
END_PROC:

    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,       kPacket.m_nOK, 0 );
        ERR_CASE( ERR_RANK_00,  kPacket.m_nOK, 1 );
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"랭킹 데이터 요청, Name : " << GetName() << L", OK : " << kPacket.m_nOK )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( kPacket.m_nType )
        << BUILD_LOG( kPacket.m_buffRank.GetLength() ) << END_LOG;

    SEND_PACKET( EVENT_GUILD_RANK_ACK ); // 이미 데이터가 압축 되어 있기 때문에.
}

_IMPL_ON_FUNC( EVENT_UPDATE_GUILD_STATE_NOT, PAIR_UCHAR )
{
    // 길드상태 변경 알림.(신규->정식길드,신규->삭제, 정식->삭제)
    // kPacket_.first : 이전상태
    // kPacket_.second : 현재상태
    // 변경되는 상태가 길드 삭제이면 초기화.

    if ( kPacket_.second == KNGuildInfo::GG_BREAKUP ) {
        START_LOG( clog, L"길드해산(삭제)로 길드상태 변경. 내 길드정보 초기화. GuildUID : " << m_kGuildUserInfo.m_dwGuildUID )
            << BUILD_LOG( GetUID() )
            << BUILD_LOG( GetName() ) << END_LOG;

        m_kGuildUserInfo.Init();
    }

    START_LOG( clog, L"길드상태 변경알림. GuildUID : " << m_kGuildUserInfo.m_dwGuildUID )
        << BUILD_LOG( GetName() )
        << BUILD_LOGc( kPacket_.first )
        << BUILD_LOGc( kPacket_.second ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_UPDATE_GUILD_STATE_NOT );
}

IMPL_ON_FUNC_NOPARAM( EVENT_GUILD_NOTICE_LIST_REQ )
{
    // 길드공지목록 요청
    SET_ERROR( ERR_UNKNOWN );

    KNGuildNoticeList kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;
    kPacket.m_vecNotice.clear();

    if ( m_kGuildUserInfo.m_dwGuildUID == 0 ) {
        // 소속 길드가 없음.
        SET_ERR_GOTO( ERR_GUILD_NOTICE_LIST_00, END_PROC );
    }

    if ( SiKGuildManager()->GetNotice( m_kGuildUserInfo.m_dwGuildUID, kPacket.m_vecNotice ) == false ) {
        // 해당 길드가 없음.
        SET_ERR_GOTO( ERR_GUILD_NOTICE_LIST_01, END_PROC );
    }
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_NOTICE_LIST_00, kPacket.m_nOK, 1 ); // 길드공지요청시 길드원이 아님.
        ERR_CASE( ERR_GUILD_NOTICE_LIST_01, kPacket.m_nOK, 2 ); // 길드공지요청시 길드정보 없음.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_dwGuildUID )
        << BUILD_LOG( kPacket.m_vecNotice.size() )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    SEND_PACKET( EVENT_GUILD_NOTICE_LIST_ACK );
}

IMPL_ON_FUNC( EVENT_UPGRADE_GUILD_GRADE_GIFT_NOT )
{
    m_kInventory.AddItemList( kPacket_.m_vecItem );

    START_LOG( clog, L"길드승격에 따른 아이템 보상받음. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_vecItem.size() )
        << BUILD_LOG( kPacket_.m_nCharType ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_UPGRADE_GUILD_GRADE_GIFT_NOT );
}

IMPL_ON_FUNC( EVENT_CHANGE_GUILD_MEMBER_LEVEL_NOT )
{
    // 자기 길드 맴버 레벨이 바뀌었음.
    std::map<DWORD,KGuildMemberLevelInfo>::iterator mit;
    mit = kPacket_.m_mapGuildMemberLevelInfo.find( GetUID() );
    if ( mit == kPacket_.m_mapGuildMemberLevelInfo.end() ) {
        START_LOG( clog, L"길드맴버레벨 변경대상이 아니다. GuildUID : " << kPacket_.m_dwGuildUID )
            << BUILD_LOG( GetName() )
            << BUILD_LOG( GetUID() ) << END_LOG;
        return;
    }

    // 변경대상이니, 변경.
    m_kGuildUserInfo.m_cMemberLevel = mit->second.m_prMemberLevel.second;

    START_LOG( clog, L"길드맴버레벨 변경됨. GuildUID : " << kPacket_.m_dwGuildUID )
        << BUILD_LOG( GetName() )
        << BUILD_LOGc( mit->second.m_prMemberLevel.first )
        << BUILD_LOGc( mit->second.m_prMemberLevel.second ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_GUILD_DRUMOUT_USER_NOT )
{
    // 길드에서 탈퇴당했음을 접속중인 유저에게 알림.
    m_kGuildUserInfo.Init();
    // 알리는 패킷은 guild 에서 처리함.
    START_LOG( clog, L"길드에서 탈퇴됨. GuildUID : " << kPacket_.m_kOutUserInfo.m_dwGuildUID )
        << BUILD_LOG( GetName() )
        << BUILD_LOGc( kPacket_.m_cOutType )
        << BUILD_LOG( kPacket_.m_kOutUserInfo.m_bIsConnect )
        << BUILD_LOGc( kPacket_.m_kOutUserInfo.m_cMemberLevel )
        << BUILD_LOGc( kPacket_.m_kOutUserInfo.m_cServerUID )
        << BUILD_LOG( kPacket_.m_kOutUserInfo.m_dwContributePoint )
        << BUILD_LOG( kPacket_.m_kOutUserInfo.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_kOutUserInfo.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_kOutUserInfo.m_kJoinDate.Str() )
        << BUILD_LOG( kPacket_.m_kOutUserInfo.m_kLastLoginDate.Str() )
        << BUILD_LOG( kPacket_.m_kOutUserInfo.m_nLoseCount )
        << BUILD_LOG( kPacket_.m_kOutUserInfo.m_nWinCount )
        << BUILD_LOG( kPacket_.m_kOutUserInfo.m_strLocation )
        << BUILD_LOG( kPacket_.m_kOutUserInfo.m_strLogin )
        << BUILD_LOG( kPacket_.m_kOutUserInfo.m_strMyComment )
        << BUILD_LOG( kPacket_.m_kOutUserInfo.m_strNickName )
        << BUILD_LOGc( kPacket_.m_kOutUserInfo.m_ucCharacterGrade ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_GUILD_TOTAL_POINT_REQ )
{
    KNGuildInfo kGuildInfo;
    if ( SiKGuildManager()->GetGuildInfo( m_kGuildUserInfo.m_dwGuildUID, kGuildInfo ) == false ) {
        START_LOG( cerr, L"길드 총 포인트 요청하는 길드 존재하지 않음" << m_kGuildUserInfo.m_dwGuildUID ) << END_LOG;
        return;
    }

    KEVENT_GUILD_TOTAL_POINT_ACK kPacket;
    kPacket.m_dwGuildPoint = kGuildInfo.m_dwPoint;
    kPacket.m_dwBattlePoint = kGuildInfo.m_GuildBattlePoint;
    kPacket.m_dwContributePoint = m_kGuildUserInfo.m_dwContributePoint;

    START_LOG( clog, L"길드의 총 포인트, Point :" << kPacket.m_dwGuildPoint )
        << BUILD_LOG( kPacket.m_dwBattlePoint ) << END_LOG;
    
    SEND_PACKET( EVENT_GUILD_TOTAL_POINT_ACK );
}

IMPL_ON_FUNC( EVENT_GUILD_MEMBER_LIST_REQ )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_CHANNEL ));
    SET_ERROR( ERR_UNKNOWN );

    KGuildPtr spGuild;
    // 길드UID 설정
    kPacket_.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;

    KEVENT_GUILD_MEMBER_LIST_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_nPageNum = kPacket_.m_nPageNum;
    kPacket.m_mapNGuildMemberList.clear();

    if ( m_kGuildUserInfo.m_dwGuildUID == 0 ) {
        // 소속 길드가 없음.
        SET_ERR_GOTO( ERR_GUILD_MEMBER_LIST_00, END_PROC );
    }

    spGuild = SiKGuildManager()->GetGuild( m_kGuildUserInfo.m_dwGuildUID );
    if ( spGuild == NULL ) {
        // 없는 길드
        SET_ERR_GOTO( ERR_GUILD_MEMBER_LIST_01, END_PROC );
    }

    // 길드 맴버리스트 받아옴.
    spGuild->GetNGuildMemberList( kPacket.m_mapNGuildMemberList );

    if ( !kPacket.m_mapNGuildMemberList.empty() ) {
        // 정상적으로 맴버 리스트 받아옴. 클라로 전달.
        kPacket.m_nOK = 0;
        SEND_PACKET( EVENT_GUILD_MEMBER_LIST_ACK );
        SET_ERR_GOTO( NET_OK, END_PROC );
    }
    START_LOG( cwarn, L"길드원 리스트가 비어있음. GuildUID : " << kPacket.m_dwGuildUID ) << END_LOG;

    // DB에 리스트 요청.
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_GUILD_MEMBER_LIST_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 );   // 0 : 성공
        ERR_CASE( ERR_GUILD_MEMBER_LIST_00, kPacket.m_nOK, 1 );   // 1 : 길드원이 아님.
        ERR_CASE( ERR_GUILD_MEMBER_LIST_01, kPacket.m_nOK, 2 );   // 2.: 없는 길드.
        ERR_CASE( ERR_GUILD_MEMBER_LIST_02, kPacket.m_nOK, 3 );   // 3 : DB처리 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 ); // -98 : 처리중인 작업
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99; // unknown error
        break;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwGuildUID )
        << BUILD_LOG( kPacket.m_nPageNum )
        << BUILD_LOG( kPacket.m_mapNGuildMemberList.size() ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_GUILD_MEMBER_LIST_ACK );
    }
}

IMPL_ON_FUNC( EVENT_GUILD_MEMBER_LIST_ACK )
{
    // DB에서 맴버리스트 직접 받아옴.
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }
    // 여기서부터 성공.
    // 내 길드에 리스트 정보 넣어주자.
    if ( SiKGuildManager()->SetGuildMemberList( kPacket_.m_dwGuildUID, kPacket_.m_mapNGuildMemberList ) == false ) {
        START_LOG( cerr, L"위에서 검사했는데, 길드정보가 없음. Login : " << m_strName )
            << BUILD_LOG( kPacket_.m_dwGuildUID ) << END_LOG;
        // 없는 길드
        SET_ERR_GOTO( ERR_GUILD_MEMBER_LIST_01, END_PROC );
    }

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                   kPacket_.m_nOK, 0 );   // 0 : 성공
        ERR_CASE( ERR_GUILD_MEMBER_LIST_00, kPacket_.m_nOK, 1 );   // 1 : 길드원이 아님.
        ERR_CASE( ERR_GUILD_MEMBER_LIST_01, kPacket_.m_nOK, 2 );   // 2.: 없는 길드.
        ERR_CASE( ERR_GUILD_MEMBER_LIST_02, kPacket_.m_nOK, 3 );   // 3 : DB처리 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket_.m_nOK, -98 ); // -98 : 처리중인 작업
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99; // unknown error
        break;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_nPageNum )
        << BUILD_LOG( kPacket_.m_mapNGuildMemberList.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_GUILD_MEMBER_LIST_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_MY_GUILD_INFO_REQ )
{
    // 내 길드원 정보 요청
    KNGuildUserInfo kPacket;
    GetGuildUserInfo( kPacket );

    START_LOG( clog, L"내 길드원 정보 전달. GuildUID : " << kPacket.m_dwGuildUID )
        << BUILD_LOG( GetName() )
        << BUILD_LOGc( kPacket.m_cMemberLevel )
        << BUILD_LOGc( kPacket.m_ucCharacterGrade )
        << BUILD_LOG( kPacket.m_dwContributePoint )
        << BUILD_LOG( kPacket.m_strMyComment )
        << BUILD_LOG( kPacket.m_kJoinDate.Str() )
        << BUILD_LOG( kPacket.m_kLastLoginDate.Str() ) << END_LOG;

    SEND_PACKET( EVENT_MY_GUILD_INFO_ACK );
}

IMPL_ON_FUNC( EVENT_SOCKET_OPEN_REQ )
{
    VERIFY_STATE( ( 2, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ) );
    
    SET_ERROR( ERR_UNKNOWN );
    KEVENT_SOCKET_OPEN_ACK kPacket;
    std::vector<KSocketInfo>::iterator vitSocket;

    // 소켓 열려는 아이템 보유 확인
    if ( GetSocketItem( kPacket_.m_ItemUID, kPacket.m_kItem ) == false ) {
        goto END_PROC;
    }

    // 보유한 아이템의  소켓 상태 확인
    if ( CheckSocketState( kPacket.m_kItem.m_vecSocket, kPacket_.m_kSocketInfo.m_cSlotID, kPacket.m_kSocketInfo, KSocketInfo::STS_CLOSED ) == false ) {
        goto END_PROC;
    }

    // 캐쉬 아이템 체크
    if ( kPacket_.m_nConsumeKind == KGCSocketConsumeItem::CASHITEM ) {
        if ( CheckSocketCashItem( kPacket_.m_CashItemID, kPacket.m_kItem, kPacket.m_kCashItem ) == false ) {
            goto END_PROC;
        }
    }
    else { // if ( kPacket_.m_nConsumeKind == CONSUME_KIND::GAMEPOINT )
        if ( CheckSocketGP( kPacket.m_kItem ) == false ) {
            goto END_PROC;
        }
    }

    // consumekind 복사
    kPacket.m_nConsumeKind = kPacket_.m_nConsumeKind;

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_SOCKET_OPEN_REQ, kPacket );
    SET_ERROR( NET_OK );

END_PROC:
    // 에러 처리, 로그
    switch( NetError::GetLastNetError() ) {

        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_SOCKET_OPEN_00, kPacket.m_nOK, 1 ); // 소켓 봉인해제 할 아이템 없음
        ERR_CASE( ERR_SOCKET_OPEN_01, kPacket.m_nOK, 2 ); // 소켓 봉인해제/카드제거 캐쉬 아이템 없음
        ERR_CASE( ERR_SOCKET_OPEN_02, kPacket.m_nOK, 3 ); // 클라이언트가 보낸 봉인해제 아이템 ID 틀림
        ERR_CASE( ERR_SOCKET_OPEN_03, kPacket.m_nOK, 4 ); // 봉인해제 요청한 소켓 슬롯 ID 를 찾을 수 없음
        ERR_CASE( ERR_SOCKET_OPEN_04, kPacket.m_nOK, 5 ); // 봉인해제 요청한 소켓을 열 수 없는 상태
        ERR_CASE( ERR_SOCKET_OPEN_12, kPacket.m_nOK, 6 ); // 봉인해제 요청한 소켓을 열 수 없는 상태

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
        default:
            kPacket.m_nOK = 99;
    }
    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_CashItemID )
        << BUILD_LOGc( kPacket.m_kSocketInfo.m_cSlotID )
        << BUILD_LOGc( kPacket.m_kSocketInfo.m_cState ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_SOCKET_OPEN_ACK );
    }
}

IMPL_ON_FUNC( EVENT_SOCKET_OPEN_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    if ( UpdateSocketState( kPacket_.m_kItem, kPacket_.m_kSocketInfo, KSocketInfo::STS_CLOSED, KSocketInfo::STS_OPENED ) == false ) {
        goto END_PROC;
    }

    if ( kPacket_.m_nConsumeKind == KGCSocketConsumeItem::CASHITEM ) {
        // 소켓 캐쉬 아이템 차감 테이블 참조해서  아이템 소모.
        if ( UseSocketCashItem( kPacket_.m_kCashItem.m_ItemID, kPacket_.m_kItem, kPacket_.m_kCashItem ) == false ) {
            goto END_PROC;
        }
    }
    else { // if ( kPacket_.m_nConsumeKind == GAMEPOINT )
        if ( UseSocketGP( kPacket_.m_kItem, kPacket_.m_nGamePoint ) == false ) {
            goto END_PROC;
        }
    }

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_SOCKET_OPEN_00, kPacket_.m_nOK, 1 );
        ERR_CASE( ERR_SOCKET_OPEN_01, kPacket_.m_nOK, 2 );
        ERR_CASE( ERR_SOCKET_OPEN_03, kPacket_.m_nOK, 3 );
        ERR_CASE( ERR_SOCKET_OPEN_04, kPacket_.m_nOK, 4 );
        ERR_CASE( ERR_SOCKET_OPEN_12, kPacket_.m_nOK, 5 );

        default:
            kPacket_.m_nOK = 99;            
    }

    LOG_SUCCESS( kPacket_.m_nOK == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kCashItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_nGamePoint ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SOCKET_OPEN_ACK );
}

IMPL_ON_FUNC( EVENT_MONSTER_CARD_INSERT_REQ )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    
    KEVENT_MONSTER_CARD_INSERT_ACK kPacket;
    KItemPtr pkCardItem;
    if ( GetSocketItem( kPacket_.m_ItemUID, kPacket.m_kItem ) == false ) {
        SET_ERR_GOTO( ERR_SOCKET_OPEN_05, END_PROC );
    }

    if ( SiKItemManager()->FindMonsterCardByItemID( kPacket_.m_CardItemID ) == false ) {
        SET_ERR_GOTO( ERR_SOCKET_OPEN_14, END_PROC );
    }

    if ( kPacket_.m_nConsumeKind == KGCSocketConsumeItem::CASHITEM ) {
        if ( GetSocketCashItem( kPacket_.m_CashItemID, kPacket.m_kItem, kPacket.m_kCashItem, kPacket.m_nUseCount ) == false ) {
            goto END_PROC;
        }
    }
    else {
        if ( CheckSocketGP( kPacket.m_kItem ) == false ) {
            goto END_PROC;
        }
    }

    // 카드 아이템 존재 확인
    pkCardItem = m_kInventory.FindItemByItemID( kPacket_.m_CardItemID );
    if ( pkCardItem == NULL ) {
        SET_ERR_GOTO( ERR_SOCKET_OPEN_13, END_PROC );
    }
    // 보유한 아이템의  소켓 상태 확인

    if ( CheckSocketState( kPacket.m_kItem.m_vecSocket, 
        kPacket_.m_kSocketInfo.m_cSlotID, 
        kPacket.m_kSocketInfo, 
        KSocketInfo::STS_OPENED ) == false ) {

        goto END_PROC;
    }
    kPacket.m_kSocketInfo.m_CardItemID = kPacket_.m_CardItemID;
    // consumekind 복사
    kPacket.m_nConsumeKind = kPacket_.m_nConsumeKind;
    kPacket.m_kCardItem = *pkCardItem;

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_SOCKET_OPEN_05, kPacket.m_nOK, 1 );
        ERR_CASE( ERR_SOCKET_OPEN_06, kPacket.m_nOK, 2 );
        ERR_CASE( ERR_SOCKET_OPEN_07, kPacket.m_nOK, 3 );
        ERR_CASE( ERR_SOCKET_OPEN_12, kPacket.m_nOK, 4 );
        ERR_CASE( ERR_SOCKET_OPEN_13, kPacket.m_nOK, 5 );
        ERR_CASE( ERR_SOCKET_OPEN_14, kPacket.m_nOK, 6 );
        default:
            kPacket.m_nOK = 99;
    }

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_CardItemID ) << END_LOG;

    // error LOG
    if( NetError::GetLastNetError() == NetError::NET_OK ) {
        QUEUEING_EVENT_TO_DB( EVENT_MONSTER_CARD_INSERT_REQ, kPacket );
    }
    else {
        SEND_PACKET( EVENT_MONSTER_CARD_INSERT_ACK );
    }
    
}

IMPL_ON_FUNC( EVENT_MONSTER_CARD_INSERT_ACK )
{
    // DB 갔다온 후 처리.
    KItemPtr pkItem;
    KItemPtr pkCardItem;
    KItemPtr pkCashItem;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    pkItem = m_kInventory.FindItemByItemUID( kPacket_.m_kItem.m_ItemUID );

    if ( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_SOCKET_OPEN_05, END_PROC );
    }

    if ( UpdateSocketState( kPacket_.m_kItem, kPacket_.m_kSocketInfo, KSocketInfo::STS_OPENED, KSocketInfo::STS_USING ) == false ) {
        goto END_PROC;
    }

    // 장착한 카드 아이템의 수량 감소
    pkCardItem = m_kInventory.FindItemByItemID( kPacket_.m_kSocketInfo.m_CardItemID );
    if ( pkCardItem == NULL ) {
        SET_ERR_GOTO( ERR_SOCKET_OPEN_13, END_PROC );
    }
    if ( pkCardItem ) {
        pkCardItem->m_nCount = kPacket_.m_kCardItem.m_nCount;
        pkCardItem->m_nInitCount = kPacket_.m_kCardItem.m_nCount;

        if ( pkCardItem->m_nCount <= 0 ) {
            m_kInventory.RemoveItem( kPacket_.m_kCardItem.m_ItemID, kPacket_.m_kCardItem.m_ItemUID );
            START_LOG( clog, L"수량이 없는 몬스터카드 인벤토리에서 제거.. LoginID : " << GetName() )
                << BUILD_LOG( kPacket_.m_kCardItem.m_ItemID )
                << BUILD_LOG( kPacket_.m_kCardItem.m_ItemUID ) << END_LOG;
        }
    }

    if ( kPacket_.m_nConsumeKind == KGCSocketConsumeItem::CASHITEM ) {
        // 소켓 캐쉬 아이템 차감 테이블 참조해서  아이템 소모.
        pkCashItem = m_kInventory.FindItemByItemID( kPacket_.m_kCashItem.m_ItemID );
        if( pkCashItem ) {
            pkCashItem->m_nCount = kPacket_.m_kCashItem.m_nCount;
            pkCashItem->m_nInitCount = kPacket_.m_kCashItem.m_nCount;
        }
    }
    else {
        // gp 사용
        if ( UseSocketGP( kPacket_.m_kItem, kPacket_.m_nGamePoint ) == false ) {
            goto END_PROC;
        }
    }

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_SOCKET_OPEN_05, kPacket_.m_nOK, 1 ); // 카드를 장착할 대상 아이템이 없음
        ERR_CASE( ERR_SOCKET_OPEN_06, kPacket_.m_nOK, 2 ); // 카드를 장착할 아이템의 소켓을 찾을 수 없음
        ERR_CASE( ERR_SOCKET_OPEN_07, kPacket_.m_nOK, 3 ); // 카드를 장착할 수 없는 소켓의 상태
        ERR_CASE( ERR_SOCKET_OPEN_12, kPacket_.m_nOK, 4 ); // 카드 장착시 GP 부족
        ERR_CASE( ERR_SOCKET_OPEN_15, kPacket_.m_nOK, 5 ); // 몬스터 카드 아이템 DB 동기화 실패
        ERR_CASE( ERR_SOCKET_OPEN_16, kPacket_.m_nOK, 6 ); // 사용 캐시 아이템 수량 DB 동기화 실패
        default:
            kPacket_.m_nOK = 99;
    }

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kCardItem.m_ItemID )
        << BUILD_LOG( GetGP() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_MONSTER_CARD_INSERT_ACK );
}

IMPL_ON_FUNC( EVENT_MONSTER_CARD_REMOVE_REQ )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    SET_ERROR( ERR_UNKNOWN );
    KEVENT_MONSTER_CARD_REMOVE_ACK kPacket;

    // 아이템 ( 소켓 ) 확인
    // 카드 제거할 아이템 확인
    if ( GetSocketItem( kPacket_.m_ItemUID, kPacket.m_kItem ) == false ) {
        SET_ERR_GOTO( ERR_SOCKET_OPEN_08, END_PROC );
    }

    // 보유한 아이템의  소켓 상태 확인
    if ( CheckSocketState( kPacket.m_kItem.m_vecSocket, 
        kPacket_.m_kSocketInfo.m_cSlotID, 
        kPacket.m_kSocketInfo, 
        KSocketInfo::STS_USING ) == false ) {

        goto END_PROC;
    }

    if ( kPacket_.m_nConsumeKind == KGCSocketConsumeItem::CASHITEM ) {
        if ( CheckSocketCashItem( kPacket_.m_CashItemID, kPacket.m_kItem, kPacket.m_kCashItem ) == false ) {
            goto END_PROC;            
        }        
    }
    else {
        if ( CheckSocketGP( kPacket.m_kItem ) == false ) {
            goto END_PROC;
        }
    }
    // consumekind 복사
    kPacket.m_nConsumeKind = kPacket_.m_nConsumeKind;

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_MONSTER_CARD_REMOVE_REQ, kPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_SOCKET_OPEN_08, kPacket.m_nOK, 1 ); // 카드를 빼낼 아이템 없음
        ERR_CASE( ERR_SOCKET_OPEN_09, kPacket.m_nOK, 2 ); // 카드를 빼낼 수 없는 소켓의 상태
        ERR_CASE( ERR_SOCKET_OPEN_10, kPacket.m_nOK, 3 ); // 카드를 빼낼 소켓을 찾을 수 없음
        ERR_CASE( ERR_SOCKET_OPEN_01, kPacket.m_nOK, 4 ); // 소켓 봉인해제/카드제거 캐쉬 아이템 없음
        ERR_CASE( ERR_SOCKET_OPEN_12, kPacket.m_nOK, 5 ); // 카드 장착시 GP 부족
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.

        default:
            kPacket.m_nOK = 99;
    }

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_MONSTER_CARD_REMOVE_ACK );
    }

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_CashItemID ) << END_LOG;

}

IMPL_ON_FUNC( EVENT_MONSTER_CARD_REMOVE_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    KItemPtr spItem;

    if ( !IS_CORRECT( NET_OK ) && !IS_CORRECT( ERR_SOCKET_OPEN_16 ) ) {
        goto END_PROC;
    }

    // 제거한 카드는 다시 인벤토리로 복구    
    m_kInventory.AddItemList( kPacket_.m_vecInv );

    if ( UpdateSocketState( kPacket_.m_kItem, kPacket_.m_kSocketInfo, KSocketInfo::STS_USING, KSocketInfo::STS_OPENED, true ) == false ) {
        goto END_PROC;
    }

    if ( kPacket_.m_nConsumeKind == KGCSocketConsumeItem::CASHITEM ) {
        // 소켓 캐쉬 아이템 차감 테이블 참조해서  아이템 소모.

        spItem = m_kInventory.FindDurationItemByItemUID( kPacket_.m_kCashItem.m_ItemUID );
        
        if ( spItem ) { // 방화벽을 이용한 아이템 차감안되는 버그 방지 위한 수정
            spItem->m_nCount = kPacket_.m_kCashItem.m_nCount;
            spItem->m_nInitCount = kPacket_.m_kCashItem.m_nCount;
        }

        if ( spItem && spItem->m_nCount <= 0 ) {
            m_kInventory.RemoveItem( kPacket_.m_kCashItem.m_ItemID, kPacket_.m_kCashItem.m_ItemUID );
        }

        //if ( UseSocketCashItem( kPacket_.m_kCashItem.m_ItemID, 
        //    kPacket_.m_kItem,
        //    kPacket_.m_kCashItem ) == false ) {

        //    goto END_PROC;
        //}
    }
    else {
        // gp 사용
        if ( UseSocketGP( kPacket_.m_kItem, kPacket_.m_nGamePoint ) == false ) {
            goto END_PROC;
        }
    }

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_SOCKET_OPEN_08, kPacket_.m_nOK, 1 ); // 카드를 빼낼 아이템 없음
        ERR_CASE( ERR_SOCKET_OPEN_09, kPacket_.m_nOK, 2 ); // 카드를 빼낼 수 없는 소켓의 상태
        ERR_CASE( ERR_SOCKET_OPEN_10, kPacket_.m_nOK, 3 ); // 카드를 빼낼 소켓을 찾을 수 없음
        ERR_CASE( ERR_SOCKET_OPEN_01, kPacket_.m_nOK, 4 ); // 소켓 봉인해제/카드제거 캐쉬 아이템 없음
        ERR_CASE( ERR_SOCKET_OPEN_12, kPacket_.m_nOK, 5 ); // 카드 장착시 GP 부족
        ERR_CASE( ERR_SOCKET_OPEN_16, kPacket_.m_nOK, 6 ); // 사용 캐시 아이템 수량 DB 동기화 실패
        default:
            kPacket_.m_nOK = 99;
    }

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecInv.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_MONSTER_CARD_REMOVE_ACK );
}

_IMPL_ON_FUNC( EVENT_ATTRIBUTE_LIST_REQ, GCITEMUID )
{
    KEVENT_ATTRIBUTE_LIST_ACK kPacket;
    kPacket.Clear();

    KItemPtr pkInvenItem = m_kInventory.FindItemByItemUID( kPacket_ );
    std::list<std::pair<int,float>> listAttributes;
    std::list<std::pair<int,float>>::iterator lit;
    std::set<DWORD> setRandomAttributeIndex;

    if( pkInvenItem == NULL )
    {
        SET_ERR_GOTO( ERR_ATTRIBUTE_SELECT_01, END_PROC );
    }

    kPacket.m_nSelectCount = 0;
    for( int i = 0 ; i < (int)pkInvenItem->m_vecAttribute.size() ; ++i )
    {
        if( pkInvenItem->m_vecAttribute[i].m_cState != KAttributeInfo::ATS_OPENED )
            continue;

        ++kPacket.m_nSelectCount;
    }
    int nAttributeTableID = SiKItemManager()->GetAttributeTableID( pkInvenItem->m_ItemID );
    int nLevelTableID = SiKItemManager()->GetLevelTableID( pkInvenItem->m_ItemID );

    int nItemLevel = 0;
    if( SiKItemManager()->m_kItemLevelDecider.IsSelectiveType( nLevelTableID ) ) {
        nItemLevel = static_cast<int>( pkInvenItem->m_sEquipLevel );
        SiKItemManager()->m_kItemLevelDecider.CheckItemLevelAttribute( nLevelTableID, nAttributeTableID, nItemLevel, KItemLevelTable::ILT_SELECTIVE );
    }

    SiKItemManager()->m_kAttributeDecider.GetAttributeFullList( nAttributeTableID, (int)pkInvenItem->m_cGradeID, listAttributes );

    kPacket.m_vecAttributeList;
    for( lit = listAttributes.begin() ; lit != listAttributes.end() ; ++lit )
    {
        KAttributeInfo kAttInfo;
        kAttInfo.m_cSlotID = 0;
        kAttInfo.m_cState = KAttributeInfo::ATS_OPENED;
        kAttInfo.m_cType = lit->first;
        kAttInfo.m_fValue = lit->second;

        kPacket.m_vecAttributeList.push_back( kAttInfo );
    }

    // 랜덤 속성 리스트 전달.
    SiKItemManager()->m_kAttributeDecider.GetRandomAttributeIndex( nAttributeTableID, (int)pkInvenItem->m_cGradeID, setRandomAttributeIndex );
    kPacket.m_setRandomAttributeIndex.swap( setRandomAttributeIndex );

    SET_ERROR( NET_OK );
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_ATTRIBUTE_SELECT_01, kPacket.m_nOK, 1 );   // 속성을 선택할 아이템이 인벤에 없음.
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99; // unknown error
        break;
    }
    SEND_PACKET( EVENT_ATTRIBUTE_LIST_ACK );
}

IMPL_ON_FUNC( EVENT_ATTRIBUTE_SELECT_REQ )
{
    struct SAttributeEqualCheck {
    private:
        KAttributeInfo m_kAttribute;
    public:
        SAttributeEqualCheck( KAttributeInfo& kAtt_ ) {
            m_kAttribute = kAtt_;
        }
        bool operator() ( const std::pair<int,float>& kInfo_ ) {
            static const double EPSILON = 2.2204460492503131e-016;
            return  m_kAttribute.m_cType == kInfo_.first && 
                (static_cast<double>(m_kAttribute.m_fValue - kInfo_.second)<=EPSILON); 
        }
    };

    KEVENT_ATTRIBUTE_SELECT_ACK kPacket;
    std::list<std::pair<int,float>> listAttributes;
    std::list<std::pair<int,float>>::iterator lit;
    std::vector< KAttributeInfo >::iterator vit;
    std::vector< KAttributeInfo >::iterator vitRandom;
    KItemPtr pkInvenItem;

    SET_ERROR( ERR_UNKNOWN );

    pkInvenItem = m_kInventory.FindItemByItemUID( kPacket_.m_ItemUID );
    if( pkInvenItem == NULL )
    {
        SET_ERR_GOTO( ERR_ATTRIBUTE_SELECT_01, END_PROC );
    }

    kPacket_.m_kItem = *pkInvenItem;

    // 속성을 선택할 수 있는 갯수를 확인 합니다.
    for ( vit = pkInvenItem->m_vecAttribute.begin() ; vit != pkInvenItem->m_vecAttribute.end() ; ++vit ) {
        if ( vit->m_cState == KAttributeInfo::ATS_USING ) {
            kPacket_.m_vecAttributeList.push_back( *vit );
        }
    } // for 와 아래 if 가 필요없어 보이는데?

    if( kPacket_.m_vecAttributeList.size() != pkInvenItem->m_vecAttribute.size() )
    {
        SET_ERR_GOTO( ERR_ATTRIBUTE_SELECT_03, END_PROC );
    }

    //해당 아이템의 Attribute List에 선택한 속성들이 있는지 확인합니다.
    int nAttributeTableID = SiKItemManager()->GetAttributeTableID( pkInvenItem->m_ItemID );
    int nLevelTableID = SiKItemManager()->GetLevelTableID( pkInvenItem->m_ItemID );

    // 레벨에 맞게 속성 변경해야 하는지 체크.
    int nItemLevel = 0;
    if( SiKItemManager()->m_kItemLevelDecider.IsSelectiveType( nLevelTableID ) ) {
        nItemLevel = static_cast<int>( pkInvenItem->m_sEquipLevel );
        SiKItemManager()->m_kItemLevelDecider.CheckItemLevelAttribute( nLevelTableID, nAttributeTableID, nItemLevel, KItemLevelTable::ILT_SELECTIVE );
    }

    SiKItemManager()->m_kAttributeDecider.GetAttributeFullList( nAttributeTableID, (int)pkInvenItem->m_cGradeID, listAttributes );

    for( vit = kPacket_.m_vecAttributeList.begin() ; vit != kPacket_.m_vecAttributeList.end() ; ++vit )
    {
        lit = std::find_if( listAttributes.begin(), listAttributes.end(), SAttributeEqualCheck( *vit ) );

        if( lit == listAttributes.end() )
            SET_ERR_GOTO( ERR_ATTRIBUTE_SELECT_02, END_PROC );
        else 
            listAttributes.erase( lit ); // 같은 속성을 보내 올까봐 한번 검색 된것은 삭제 시킴.
    }

    // 클라이언트가 선택한 속성중에 랜덤 속성이 있는지 체크.
    for( vit = kPacket_.m_vecAttributeList.begin() ; vit != kPacket_.m_vecAttributeList.end() ; ++vit ) {
        if( !SiKItemManager()->m_kAttributeDecider.CheckRandomAttributeIndex( nAttributeTableID, (int)pkInvenItem->m_cGradeID, static_cast<DWORD>( vit->m_cType ) ) ) {
            // 랜덤 속성을 현재 사용중인지 체크.
            vitRandom = std::find_if( pkInvenItem->m_vecAttribute.begin(), pkInvenItem->m_vecAttribute.end(), 
                boost::bind( &KAttributeInfo::m_cType, _1 ) == vit->m_cType );
            if( vitRandom->m_fValue <= 0.f ) {
                SET_ERR_GOTO( ERR_ATTRIBUTE_SELECT_05, END_PROC );
            }
        }
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_ATTRIBUTE_SELECT_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_ATTRIBUTE_SELECT_01, kPacket.m_nOK, 1 );   // 속성을 선택할 아이템이 인벤에 없음.
        ERR_CASE( ERR_ATTRIBUTE_SELECT_02, kPacket.m_nOK, 2 );   // 선택한 속성이 선택 가능한 속성에 포함되지 않습니다.
        ERR_CASE( ERR_ATTRIBUTE_SELECT_03, kPacket.m_nOK, 3 );   // 선택한 속성의 갯수가 올바르지 못합니다.
        ERR_CASE( ERR_ATTRIBUTE_SELECT_04, kPacket.m_nOK, 4 );   // 속성을 DB 에 기록 하는데 오류 발생
        ERR_CASE( ERR_ATTRIBUTE_SELECT_05, kPacket.m_nOK, 5 );   // 랜덤 속성을 선택할 수 없습니다.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99; // unknown error
        break;
    }

    if ( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_ATTRIBUTE_SELECT_ACK );

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"속성 설정 요청중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_vecAttributeList.size() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_ATTRIBUTE_SELECT_ACK )
{
    KItemPtr pkInvenItem;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    pkInvenItem = m_kInventory.FindItemByItemUID( kPacket_.m_kItem.m_ItemUID );

    if( pkInvenItem == NULL ) {
        SET_ERR_GOTO( ERR_ATTRIBUTE_SELECT_01, END_PROC );
    }

    pkInvenItem->m_vecAttribute = kPacket_.m_kItem.m_vecAttribute;
    kPacket_.m_kItem = *pkInvenItem;

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_ATTRIBUTE_SELECT_01, kPacket_.m_nOK, 1 );   // 속성을 선택할 아이템이 인벤에 없음.
        ERR_CASE( ERR_ATTRIBUTE_SELECT_02, kPacket_.m_nOK, 2 );   // 선택한 속성이 선택 가능한 속성에 포함되지 않습니다.
        ERR_CASE( ERR_ATTRIBUTE_SELECT_03, kPacket_.m_nOK, 3 );   // 선택한 속성의 갯수가 올바르지 못합니다.
        ERR_CASE( ERR_ATTRIBUTE_SELECT_04, kPacket_.m_nOK, 4 );   // 속성을 DB 에 기록 하는데 오류 발생
        ERR_CASE( ERR_ATTRIBUTE_SELECT_05, kPacket_.m_nOK, 5 );   // 랜덤 속성을 선택할 수 없습니다.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99; // unknown error
        break;
    }

    SEND_RECEIVED_PACKET( EVENT_ATTRIBUTE_SELECT_ACK );
    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"속성 설정 응답 MSG :" << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName)
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kItem.m_vecAttribute.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_ITEM_BREAKUP_REQ, GCITEMUID )
{
    KEVENT_ITEM_BREAKUP_REQ kDBPacket;
    KEVENT_ITEM_BREAKUP_ACK kPacket;
    kPacket.m_nOK = 99;
    kDBPacket.m_nGPCost = 0;
    KItemPtr pkItem;
    KItemPtr pkStrengthItem; // 연계된 강화석 아이템.

    SET_ERROR( ERR_UNKNOWN );

    // 아이템을 보유 하고 있는가?
    pkItem = m_kInventory.FindItemByItemUID( kPacket_ );
    if ( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_ITEM_BREAKUP_00, END_PROC );
    }

    // 가격 정보 가져 오기.
    if ( !GetItemBreakupCost( *pkItem, kDBPacket.m_nGPCost ) ) {
        SET_ERR_GOTO( ERR_ITEM_BREAKUP_04, END_PROC );
    }

    // 가격은 충분 한가.
    if ( GetGP() < kDBPacket.m_nGPCost ) {
        SET_ERR_GOTO( ERR_ITEM_BREAKUP_05, END_PROC );
    }

    // 해체 가능한 아이템인가?
    if ( !SiKItemManager()->m_kItemBreakup.CheckBreakUpEnable( *pkItem ) ) {
        SET_ERR_GOTO( ERR_ITEM_BREAKUP_01, END_PROC );
    }

    if ( pkItem->m_ItemUID <= SiKIPAdvantage()->GetMaximumPCBangItemUID() ) {
        SET_ERR_GOTO( ERR_ITEM_BREAKUP_01, END_PROC );
    }

    // 재료 아이템 정보를 얻어 온다.
    if ( !SiKItemManager()->m_kItemBreakup.BreakUpItem( *pkItem, kDBPacket.m_vecMaterials ) ) {
        SET_ERR_GOTO( ERR_ITEM_BREAKUP_02, END_PROC );
    }

    if ( pkItem->m_EnchantEquipItemUID != 0 ) { // 연계된 강화석이 있을 경우..,
        pkStrengthItem = m_kInventory.FindItemByItemUID( pkItem->m_EnchantEquipItemUID );
        if( pkStrengthItem ) {
            kDBPacket.m_kStrengthItem = *pkStrengthItem;
        }
    }

    kDBPacket.m_kBreakupItem = *pkItem;
    kDBPacket.m_nAccReelPoint = 0;

    SET_ERROR( NET_OK );
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_ITEM_BREAKUP_REQ, kDBPacket );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_ITEM_BREAKUP_00, kPacket.m_nOK, 1 ); //해체 하려는 아이템이 존재 하지 않습니다.
        ERR_CASE( ERR_ITEM_BREAKUP_01, kPacket.m_nOK, 2 ); //해체 가능한 아이템이 아닙니다.
        ERR_CASE( ERR_ITEM_BREAKUP_02, kPacket.m_nOK, 3 ); //재료 아이템 정보 얻는중 오류.
        ERR_CASE( ERR_ITEM_BREAKUP_03, kPacket.m_nOK, 4 ); //DB 처리 오류.
        ERR_CASE( ERR_ITEM_BREAKUP_04, kPacket.m_nOK, 5 ); //비용 정보 얻기 실패
        ERR_CASE( ERR_ITEM_BREAKUP_05, kPacket.m_nOK, 6 ); //보유 GP 부족

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"아이템 해체중. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( kDBPacket.m_bJackpot )
        << BUILD_LOG( kDBPacket.m_kBreakupItem.m_ItemID )
        << BUILD_LOG( kDBPacket.m_nAccReelPoint )
        << BUILD_LOG( kDBPacket.m_vecMaterials.size() ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_ITEM_BREAKUP_ACK );
    }
}

IMPL_ON_FUNC( EVENT_ITEM_BREAKUP_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) )
        goto END_PROC;

    // 해체 아이템 삭제.
    LIF( m_kInventory.RemoveItem( kPacket_.m_kBreakupItem.m_ItemID, kPacket_.m_kBreakupItem.m_ItemUID ) );
    if( kPacket_.m_kStrengthItem.m_ItemID != 0 ) {
        LIF( m_kInventory.RemoveItem( kPacket_.m_kStrengthItem.m_ItemID, kPacket_.m_kStrengthItem.m_ItemUID ) );
    }
    // 아이템 적용.
    m_kInventory.AddItemList( kPacket_.m_vecMaterials );
    // 릴 포인트 적용.
    m_nItemBreakupReelPoint = kPacket_.m_nAccReelPoint;
    // GP 차감.
    bool bDBUpdate = false; // DB에서 GP 갱신 처리 하지 않았음.
    DecreaseGP( KGpStatInfo::GDT_ITEM_BREAKUP, bDBUpdate, kPacket_.m_nGPCost );
    kPacket_.m_nGP = GetGP();

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_ITEM_BREAKUP_00, kPacket_.m_nOK, 1 ); //해체 하려는 아이템이 존재 하지 않습니다.
        ERR_CASE( ERR_ITEM_BREAKUP_01, kPacket_.m_nOK, 2 ); //해체 가능한 아이템이 아닙니다.
        ERR_CASE( ERR_ITEM_BREAKUP_02, kPacket_.m_nOK, 3 ); //재료 아이템 정보 얻는중 오류.
        ERR_CASE( ERR_ITEM_BREAKUP_03, kPacket_.m_nOK, 4 ); //DB 처리 오류.
        ERR_CASE( ERR_ITEM_BREAKUP_04, kPacket_.m_nOK, 5 ); //비용 정보 얻기 실패
        ERR_CASE( ERR_ITEM_BREAKUP_05, kPacket_.m_nOK, 6 ); //보유 GP 부족

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"아이템 해체응답. MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_nItemBreakupReelPoint )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nGPCost )
        << BUILD_LOG( kPacket_.m_nGP )
        << BUILD_LOG( kPacket_.m_bJackpot )
        << BUILD_LOG( kPacket_.m_nAccReelPoint )
        << BUILD_LOG( kPacket_.m_kBreakupItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_vecMaterials.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_ITEM_BREAKUP_ACK );
}

IMPL_ON_FUNC( DB_EVENT_TIME_DROP_ACK )
{
    KTimeDropItem kPacket;
    kPacket = kPacket_.m_kDropItem;

    if ( kPacket_.m_nOK != 0 ) {
        goto END_PROC;
    }

    // TimeDrop 아이템 지급
    m_kInventory.AddItemList( kPacket.m_vecItems );

END_PROC:
    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"TimeDrop Item.. Result : " << kPacket_.m_nOK )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket.m_nIndex )
        << BUILD_LOG( kPacket.m_vecItems.size() ) << END_LOG;

    if ( kPacket_.m_nOK == 0 ) {
        SEND_PACKET( EVENT_TIME_DROP_NOT );
    }
}

IMPL_ON_FUNC( DB_EVENT_GET_TIME_DROP_INFO_ACK )
{
    // DB에서 TimeDrop 정보 받아옴.
    m_kTimeDrop.SetInfo( kPacket_.m_nAccPlayTime, kPacket_.m_nCurruntDropCount );

    START_LOG( clog, L"TimeDrop 정보 설정." )
        << BUILD_LOG( kPacket_.m_nAccPlayTime )
        << BUILD_LOG( kPacket_.m_nCurruntDropCount )
        << BUILD_LOGtm( CTime(kPacket_.m_tmRegDate) ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_GET_WEB_CASH_ITEM_REQ )
{
    VERIFY_STATE((5,KUserFSM::STATE_CHANNELLOBBY,KUserFSM::STATE_CHANNEL,KUserFSM::STATE_ROOM,KUserFSM::STATE_AGIT,KUserFSM::STATE_SQUARE));
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_GET_WEB_CASH_ITEM_REQ, kPacket_.m_nItemCharType );

    // 웹 구매는 성공 하고 대기 아이템을 넣기 때문에 REQ부분에서 수량 갱신
    // 아이템 구매 성공 후 소셜커머스 객체에 DB갱신 처리 요청
    // 아이템 구매 성공 후 소셜커머스 객체에 DB갱신 처리 요청
    if ( true == SiKSocialCommerce()->IsRun() && 0 != kPacket_.m_dwEventUID ) {
        KDB_EVENT_SOSCIAL_COMMERCE_BUY_REQ kDBPacket;
        if ( true == SiKSocialCommerce()->GetSocialCommerceItemInfoFromCurrentBuyItem( kPacket_.m_dwEventUID, kDBPacket ) ) {
            SiKSocialCommerce()->SendSellCountUpdateReqToDB( GetUID(), kDBPacket );
        } // else면 일반 캐시 아이템 구매
    }
END_PROC:
    START_LOG( clog, L"Get WebCashItem Req. Login: " << GetName()
        << ", ItemCharType: " << kPacket_.m_nItemCharType
        << ", m_cCurrentCharacter: " << static_cast<int>(m_cCurrentCharacter) ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_GET_WEB_CASH_ITEM_ACK )
{
    m_kInventory.AddItemList( kPacket_.m_vecGetItems );
    SEND_RECEIVED_PACKET( EVENT_GET_WEB_CASH_ITEM_ACK );

    // VIP이벤트 보상확인
    SendVipEventUserRewardUpdateNot();

    if ( std::find_if( kPacket_.m_vecGetItems.begin(), kPacket_.m_vecGetItems.end(),
        boost::bind( &KItem::m_ItemID, _1) == CASH_KEY_ID ) != kPacket_.m_vecGetItems.end() ) {
        UpdateGachaKeyBuyCount( 1, 0 );
    }

    // 웹 구매가 진행되는 국가의 경우 DB에서 해당 아이템의 구매 Count을 올려준다.
    // 서버에서 체크하는 로직 주석 처리.
    /*
    // 구매제한 있는 웹 CASH 아이템이 있을 경우 값을 올려줘야 한다.
    std::set<DWORD> setBuyCheckItemList;
    std::set<DWORD>::iterator sit;
    GCITEMID dwItemID = 0;
    int nCount = 0;
    SiKGCHelper()->GetBuyCheckItemList( setBuyCheckItemList );
    for( sit = setBuyCheckItemList.begin(); sit != setBuyCheckItemList.end(); ++sit ) {
        if ( std::find_if( kPacket_.begin(), kPacket_.end(), boost::bind( &KItem::m_ItemID, _1) == *sit ) != kPacket_.end() ) {
            dwItemID = *sit;
            nCount += 1;
        }
    }
    UpdateBuyCheckItemCount( dwItemID );
    */
}

IMPL_ON_FUNC( EVENT_STAT_RESOLUTION_NOT )
{
    QUEUEING_EVENT_TO_DB( EVENT_STAT_RESOLUTION_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EVENT_MANUFACTURES3_CATALOG_REQ )
{
    KSerBuffer buffPacket;
    SiKItemManager()->m_kItemManufacture.GetCatalogs( buffPacket );
    LIF( SendPacket( buffPacket, L"EVENT_MANUFACTURES3_CATALOG_ACK" ));

    START_LOG( clog, L" 제작 목록.. Name : " << m_strName << L", Size : " << buffPacket.GetLength() ) << END_LOG;

}

IMPL_ON_FUNC( EVENT_MANUFACTURES3_MATERIAL_REQ )
{
    std::map< PAIR_USHORT_DWORD,std::vector<KDropItemInfo> > kPacket;
    SiKItemManager()->m_kItemManufacture.GetMaterialList( kPacket_, kPacket );
    SEND_COMPRESS_PACKET( EVENT_MANUFACTURES3_MATERIAL_ACK, kPacket );
    START_LOG( clog, L" 제작 재료 요청.. Name : " << m_strName
        << L", Req Size : " << kPacket_.size()
        << L", Ack Size : " << kPacket.size() ) << END_LOG;

}

IMPL_ON_FUNC( EVENT_MANUFACTURES3_REQ )
{
    //Checker check( KncUtil::toWideString("EVENT_MANUFACTURES3_REQ"), 1000 );
    // 아이템 목록이 존재 하는가?
    std::vector<KDropItemInfo> vecMaterials;
    std::vector<KDropItemInfo>::iterator vit;
    std::vector<GCITEMUID>::iterator vitMaterials;

    KEVENT_MANUFACTURES3_ACK kPacket;
    kPacket.m_nOK = 99;
    KItemPtr pkStrengthItem; // 연계된 강화석 아이템.

    int totalDiscount = 0;

    PAIR_USHORT_DWORD prKey = std::make_pair( kPacket_.m_kCatalog.m_Index, kPacket_.m_kCatalog.m_ItemID );
    SET_ERROR( ERR_UNKNOWN );

    if ( !SiKItemManager()->m_kItemManufacture.GetCatalog( prKey, kPacket_.m_kCatalog ) ) {
        SET_ERR_GOTO( ERR_MANUFACTURE_S3_00, END_PROC );
    }

    // 가격 정보가 있는가?
    if ( kPacket_.m_kCatalog.m_nPrice < 0 ) {
        SET_ERR_GOTO( ERR_MANUFACTURE_S3_01, END_PROC );
    }

    // 재료 아이템이 존재 하는가?
    if ( !SiKItemManager()->m_kItemManufacture.GetMaterials( prKey, vecMaterials ) ) {
        SET_ERR_GOTO( ERR_MANUFACTURE_S3_03, END_PROC );
    }

    // 재료 아이템에 동일한 UID의 아이템이 존재하는지 체크
    if ( false == CheckDuplicateItemUID( kPacket_.m_vecDelMaterials ) ) {
        SET_ERR_GOTO( ERR_MANUFACTURE_S3_10, END_PROC );
    }

    // 수량이 1개 이상인가?
    if ( kPacket_.m_usFactor <= 0 ) {
        SET_ERR_GOTO( ERR_MANUFACTURE_S3_06, END_PROC );
    }
    else if ( kPacket_.m_usFactor > 1 ) {
        // 수량이 1개 초과인데 패키지 아이템인지 확인
        if ( true == SiKItemManager()->IsPackageItem( kPacket_.m_kCatalog.m_ItemID ) ) {
            SET_ERR_GOTO( ERR_MANUFACTURE_S3_11, END_PROC );
        }
    }

    // 제작 회수만큼 제작아이템 정보 동기화
    CalcMaterialsByFactor( kPacket_.m_usFactor, vecMaterials );

    // GP는 충분한가.
    if ( GetGP() < kPacket_.m_kCatalog.m_nPrice * kPacket_.m_usFactor ) {
        SET_ERR_GOTO( ERR_MANUFACTURE_S3_02, END_PROC );
    }

    // 코디아이템일 경우, 충분한 인벤 공간이 없음.
    bool bLookItem = kPacket_.m_kCatalog.CheckItemFlag( KManufactureItem::IF_LOOK );
    if ( bLookItem ) {
        if ( m_kInventory.GetLookItemNum() >= GetTrueLookInvenCapacity() ) {
            SET_ERR_GOTO( ERR_MANUFACTURE_S3_06, END_PROC );
        }
    }

    // 스크립트 설정된 재료 아이템과 클라이언트에서 전달된 재료 아이템 갯수 체크.
    if( vecMaterials.size() != kPacket_.m_vecDelMaterials.size() ) {
        SET_ERR_GOTO( ERR_MANUFACTURE_S3_09, END_PROC );
    }

    kPacket_.m_vecNormalMaterials.clear();
    for( vitMaterials = kPacket_.m_vecDelMaterials.begin(); vitMaterials != kPacket_.m_vecDelMaterials.end(); ++vitMaterials ) {
        KItemPtr pkItem = m_kInventory.FindItemByItemUID( *vitMaterials );
        if ( pkItem == NULL ) {
            START_LOG( cerr, L"재료 아이템이 인벤에 없음. Login : " << GetName() )
                << BUILD_LOG( *vitMaterials ) << END_LOG;
            SET_ERR_GOTO( ERR_MANUFACTURE_S3_04, END_PROC );
        }

        vit = std::find_if( vecMaterials.begin(), vecMaterials.end(), boost::bind( &KDropItemInfo::m_ItemID, _1 ) == pkItem->m_ItemID );
        if( vit == vecMaterials.end() ) {
            SET_ERR_GOTO( ERR_MANUFACTURE_S3_09, END_PROC );
        }

        if ( vit->m_nDuration == KItem::UNLIMITED_ITEM ) {

            if ( pkItem->m_nCount != KItem::UNLIMITED_ITEM || pkItem->m_nInitCount != KItem::UNLIMITED_ITEM ) {
                START_LOG( cerr, L"재료 아이템이 무제한이 아니다. Login : " << GetName() )
                    << BUILD_LOG( vit->m_ItemID )
                    << BUILD_LOG( pkItem->m_nCount )
                    << BUILD_LOG( pkItem->m_nInitCount ) << END_LOG;
                SET_ERR_GOTO( ERR_MANUFACTURE_S3_04, END_PROC );
            }

            kPacket_.m_vecNormalMaterials.push_back( pkItem->m_ItemUID );
        }
        else {
            if ( kPacket_.m_dDiscount > 0 && kPacket_.m_kCatalog.m_bIsOnDiscount )
            {
                totalDiscount = (int)ceil(vit->m_nDuration - (vit->m_nDuration * kPacket_.m_dDiscount));
            }
            else
            {
                totalDiscount = vit->m_nDuration;
            }

            if (pkItem->m_nCount < totalDiscount){
                START_LOG( cerr, L"재료 아이템이 수량이 부족하다. Login : " << GetName() )
                    << BUILD_LOG( vit->m_ItemID )
                    << BUILD_LOG( totalDiscount )
                    << BUILD_LOG( pkItem->m_nCount ) << END_LOG;
                SET_ERR_GOTO( ERR_MANUFACTURE_S3_04, END_PROC );
            }

            kPacket_.m_mapKItem[pkItem->m_ItemID] = *pkItem; // 디비에서 아이템 동기화하기 위해서 아이템 세팅
        }

        if( pkItem->m_EnchantEquipItemUID != 0 ) { // 연계된 강화석이 있을 경우..,
            pkStrengthItem = m_kInventory.FindItemByItemUID( pkItem->m_EnchantEquipItemUID );
            if( pkStrengthItem ) {
                kPacket_.m_vecStrengthItem.push_back( pkStrengthItem->m_ItemUID );
            }
        }
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_MANUFACTURES3_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_MANUFACTURE_S3_00, kPacket.m_nOK, 1 ); // 아이템이 목록에 존재 하지 않음.
        ERR_CASE( ERR_MANUFACTURE_S3_01, kPacket.m_nOK, 2 ); // 제작 아이템의 가격 정보가 잘못 되었음.
        ERR_CASE( ERR_MANUFACTURE_S3_02, kPacket.m_nOK, 3 ); // GP가 충분하지 않음.
        ERR_CASE( ERR_MANUFACTURE_S3_03, kPacket.m_nOK, 4 ); // 재료 정보가 없음.
        ERR_CASE( ERR_MANUFACTURE_S3_04, kPacket.m_nOK, 5 ); // 재료 아이템을 보유하고 있지 않음.
        ERR_CASE( ERR_MANUFACTURE_S3_05, kPacket.m_nOK, 6 ); // Item 삽입 실패.
        ERR_CASE( ERR_MANUFACTURE_S3_06, kPacket.m_nOK, 7 ); // 코디 인벤토리 부족
        ERR_CASE( ERR_MANUFACTURE_S3_09, kPacket.m_nOK, 9 ); // 전달된 재료 아이템이 존재하지 않음.
        ERR_CASE( ERR_MANUFACTURE_S3_10, kPacket.m_nOK, 10 ); // 재료 아이템에 동일한 UID의 아이템이 존재함
        ERR_CASE( ERR_MANUFACTURE_S3_11, kPacket.m_nOK, 11 ); // 패키지 아이템을 수량으로 제작 하려고 함

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 || NetError::GetLastNetError() == NetError::ERR_MANUFACTURE_S3_02, L"제작 요청 시즌 3 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_kCatalog.m_Index )
        << BUILD_LOG( kPacket_.m_kCatalog.m_ItemID )
        << BUILD_LOG( kPacket_.m_kCatalog.m_nDuration )
        << BUILD_LOG( kPacket_.m_kCatalog.m_nPeriod )
        << BUILD_LOG( kPacket_.m_kCatalog.m_nPrice )
        << BUILD_LOG( kPacket_.m_vecDelMaterials.size() )
        << BUILD_LOG( kPacket_.m_usFactor )
        << BUILD_LOG( bLookItem )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nGP )
        << BUILD_LOG( kPacket.m_vecRewards.size() )
        << BUILD_LOG( kPacket.m_vecCountMaterialItems.size() )
        << BUILD_LOG( kPacket.m_vecPeriodMaterialItems.size() ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_MANUFACTURES3_ACK );
    }
}

IMPL_ON_FUNC( EVENT_MANUFACTURES3_ACK )
{
    std::vector< GCITEMUID >::iterator vit;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) )
        goto END_PROC;
    m_kInventory.AddItemList( kPacket_.m_vecRewards );

    for( vit = kPacket_.m_vecPeriodMaterialItems.begin() ; vit != kPacket_.m_vecPeriodMaterialItems.end() ; ++vit ) {
        LIF( m_kInventory.RemoveItem( *vit ) );
    }

    if ( !kPacket_.m_vecRewards.empty() ) {
        std::vector<KDropItemInfo> vecMaterials;
        std::vector<KDropItemInfo>::iterator vitCount;

        if ( SiKItemManager()->m_kItemManufacture.GetMaterials( kPacket_.m_prKey, vecMaterials ) ) {

            for ( vitCount = vecMaterials.begin() ; vitCount != vecMaterials.end() ; ++vitCount ) {
                KItemPtr pkItem = m_kInventory.FindDurationItemByItemID( vitCount->m_ItemID );
                if ( !pkItem ) continue;

                std::map< GCITEMID, KItem >::iterator mit;
                mit = kPacket_.m_mapKItem.find( vitCount->m_ItemID );

                if ( mit == kPacket_.m_mapKItem.end() ) { continue; }

                pkItem->m_nCount = mit->second.m_nCount;
                pkItem->m_nInitCount = pkItem->m_nCount;

                // 수량이 0이면 삭제 (DB에선 이미 삭제되었음)
                if ( pkItem->m_nCount == 0 ) {
                    LIF( m_kInventory.RemoveItem( pkItem->m_ItemUID ) );
                    kPacket_.m_vecPeriodMaterialItems.push_back( pkItem->m_ItemUID );
                }
                else {
                    kPacket_.m_vecCountMaterialItems.push_back( *pkItem );
                }
            }
        }

        KManufactureItem kCatalog;
        if ( SiKItemManager()->m_kItemManufacture.GetCatalog( kPacket_.m_prKey, kCatalog ) ) {
            int nDecGP = kCatalog.m_nPrice * kPacket_.m_usFactor;
            bool bDBUpdate = false; // DB에서 GP 갱신 처리 하지 않았음.
            DecreaseGP( KGpStatInfo::GDT_MANUFACTURE, bDBUpdate, nDecGP );
        }
    }

    if( !kPacket_.m_vecStrengthItem.empty() ) {
        for( vit = kPacket_.m_vecStrengthItem.begin() ; vit != kPacket_.m_vecStrengthItem.end() ; ++vit ) {
            LIF( m_kInventory.RemoveItem( *vit ) );
            kPacket_.m_vecPeriodMaterialItems.push_back( *vit ); // 강화석은 영구아이템이기 때문에..,
        }
    }

    kPacket_.m_nGP = GetGP();

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_MANUFACTURE_S3_00, kPacket_.m_nOK, 1 ); // 아이템이 목록에 존재 하지 않음.
        ERR_CASE( ERR_MANUFACTURE_S3_01, kPacket_.m_nOK, 2 ); // 제작 아이템의 가격 정보가 잘못 되었음.
        ERR_CASE( ERR_MANUFACTURE_S3_02, kPacket_.m_nOK, 3 ); // GP가 충분하지 않음.
        ERR_CASE( ERR_MANUFACTURE_S3_03, kPacket_.m_nOK, 4 ); // 재료 정보가 없음.
        ERR_CASE( ERR_MANUFACTURE_S3_04, kPacket_.m_nOK, 5 ); // 재료 아이템을 보유하고 있지 않음.
        ERR_CASE( ERR_MANUFACTURE_S3_05, kPacket_.m_nOK, 6 ); // Item 삽입 실패.
        ERR_CASE( ERR_MANUFACTURE_S3_06, kPacket_.m_nOK, 7 ); // 코디 인벤토리 부족

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"제작 시즌 3 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nGP )
        << BUILD_LOG( kPacket_.m_prKey.first )
        << BUILD_LOG( kPacket_.m_prKey.second )
        << BUILD_LOG( kPacket_.m_usFactor )
        << BUILD_LOG( kPacket_.m_vecRewards.size() )
        << BUILD_LOG( kPacket_.m_vecCountMaterialItems.size() )
        << BUILD_LOG( kPacket_.m_vecPeriodMaterialItems.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_MANUFACTURES3_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_ITEM_BREAKUP_PRICE_RATIO_REQ )
{
    float kPacket = SiKItemManager()->m_kItemBreakup.GetPriceRatio();
    SEND_PACKET( EVENT_ITEM_BREAKUP_PRICE_RATIO_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_PRESENTBOX_INFO_REQ )
{
    KEVENT_PRESENTBOX_INFO_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    // 이벤트 기간인가?
    if( !SiKPresentBox()->IsRun() ) {
        SET_ERR_GOTO( ERR_PRESENTBOX_00, END_PROC );
    }

    // 조각 교환정보 리스트 가져오기.
    SiKPresentBox()->GetTradeItemList( kPacket.m_mapTradeList );

    // 선물 상자 보상 아이템 리스트 가져오기.
    SiKPresentBox()->GetRewardItemList( kPacket.m_vecRewardList );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 );     // 성공.
        ERR_CASE( ERR_PRESENTBOX_00,        kPacket.m_nOK, 1 );     // 이벤트 기간이 아님
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"선물상자 Event 정보 요청중 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L"선물상자 Event 정보 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_mapTradeList.size() )
        << BUILD_LOG( kPacket.m_vecRewardList.size() ) << END_LOG;

    SEND_PACKET( EVENT_PRESENTBOX_INFO_ACK );
}

IMPL_ON_FUNC( EVENT_PRESENTBOX_TRADE_REQ )
{
    KEVENT_PRESENTBOX_TRADE_ACK kPacket;
    KItemPtr pkItem;
    SET_ERROR( ERR_UNKNOWN );

    // 이벤트 기간인가?
    if( !SiKPresentBox()->IsRun() ) {
        SET_ERR_GOTO( ERR_PRESENTBOX_00, END_PROC );
    }

    // 해당 아이템이 존재 하는가?
    pkItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_dwItemID );
    if ( !pkItem ) {
        SET_ERR_GOTO( ERR_PRESENTBOX_02, END_PROC );
    }

    if( !SiKPresentBox()->GetTradeItem( kPacket_.m_dwItemID, kPacket_.m_vecRewardItem ) ) {
        START_LOG( cerr, L"해당 아이템의 교환 정보 없음 ..  Name : " << m_strName )
            << BUILD_LOG( kPacket_.m_dwItemID ) << END_LOG;
        SET_ERR_GOTO( ERR_PRESENTBOX_03, END_PROC );
    }

    // DB에 큐잉.
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_PRESENTBOX_TRADE_REQ, kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_PRESENTBOX_00,        kPacket.m_nOK, 1 );    // 선물상자 이벤트 기간이 아님. 
        ERR_CASE( ERR_PRESENTBOX_02,        kPacket.m_nOK, 2 );    // 선물상자 조각 아이템이 존재 하지 않음. 
        ERR_CASE( ERR_PRESENTBOX_03,        kPacket.m_nOK, 3 );    // 선물상자 교환 아이템 정보가 없습니다. 
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"선물 Event 조각 아이템 교환중 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_dwItemID )
            << BUILD_LOG( kPacket_.m_vecRewardItem.size() ) << END_LOG;
    }

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L"선물 Event 아이템 교환 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_dwItemID )
        << BUILD_LOG( kPacket_.m_vecRewardItem.size() ) << END_LOG;

    if( !IS_CORRECT( NET_OK ) )
        SEND_PACKET( EVENT_PRESENTBOX_TRADE_ACK );
}

IMPL_ON_FUNC( EVENT_PRESENTBOX_TRADE_ACK )
{
    KItemPtr pkItem;
    KItem kdItemInfo;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 선물 상자 조각 Item 정보 가져오기.
    pkItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_dwItemID );
    if( NULL == pkItem ) {
        SET_ERR_GOTO( ERR_PRESENTBOX_02, END_PROC );
    }

    pkItem->m_nCount -= 1; // 해당 아이템 갯수 차감.
    // 클라이언트에게 사용한 아이템 정보를 보내주기 위해서.
    kPacket_.m_vecUseItem.push_back( *pkItem );

    // 획득한 아이템을 넣어주는 코드.
    m_kInventory.AddItemList( kPacket_.m_vecItem );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket_.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_PRESENTBOX_00,        kPacket_.m_nOK, 1 );    // 선물상자 이벤트 기간이 아님. 
        ERR_CASE( ERR_PRESENTBOX_02,        kPacket_.m_nOK, 2 );    // 선물상자 조각 아이템이 존재 하지 않음. 
        ERR_CASE( ERR_PRESENTBOX_03,        kPacket_.m_nOK, 3 );    // 선물상자 교환 아이템 정보가 없습니다. 
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        kPacket_.m_nOK = 99;
        START_LOG( cerr, L"선물 Event 조각 아이템 교환중 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_dwItemID ) << END_LOG;
    }

    _LOG_CONDITION( kPacket_.m_nOK == 0, clog, cwarn, L"선물 Event 아이템 교환요청 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwItemID )
        << BUILD_LOG( kPacket_.m_vecUseItem.size() )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_PRESENTBOX_TRADE_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_PRESENTBOX_ACTION_REQ )
{
    KEVENT_PRESENTBOX_ACTION_ACK kPacket;
    KDB_EVENT_PRESENTBOX_ACTION_REQ kDBPacket;
    KItemPtr pkItem;
    std::set<GCITEMID> setPresentBoxList;
    std::set<GCITEMID>::iterator sit;
    SET_ERROR( ERR_UNKNOWN );

    // 이벤트 기간인가?
    if( !SiKPresentBox()->IsRun() ) {
        SET_ERR_GOTO( ERR_PRESENTBOX_00, END_PROC );
    }

    // 조각 아이템 전부가 존재하는가?
    SiKPresentBox()->GetPresentBoxItemList( setPresentBoxList );
    for( sit = setPresentBoxList.begin(); sit != setPresentBoxList.end(); ++sit ) {
        pkItem = m_kInventory.FindItemByItemID( *sit );
        if( NULL == pkItem ) {
            SET_ERR_GOTO( ERR_PRESENTBOX_02, END_PROC );
        }
        pkItem.reset();
    }

    // 구슬 상자 열기.
    if( !SiKPresentBox()->DoGamble( kDBPacket.m_kPostItem ) ) {
        goto END_PROC;
    }
    // 구슬 상자 조각 아이템 리스트 받아오기.
    kDBPacket.m_setItemList = setPresentBoxList;

    // DB에 큐잉.
    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_PRESENTBOX_ACTION_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_PRESENTBOX_00,        kPacket.m_nOK, 1 );    // 선물상자 이벤트 기간이 아님. 
        ERR_CASE( ERR_PRESENTBOX_02,        kPacket.m_nOK, 2 );    // 선물상자 조각 아이템이 존재 하지 않음. 
        ERR_CASE( ERR_PRESENTBOX_03,        kPacket.m_nOK, 3 );    // 선물상자 교환 아이템 정보가 없습니다. 
        ERR_CASE( ERR_PRESENTBOX_04,        kPacket.m_nOK, 4 );    // 꽝입니다.
        ERR_CASE( ERR_PRESENTBOX_05,        kPacket.m_nOK, 5 );    // 로터리에 등록 되지 않음.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );  // 아직 처리중인 작업입니다.
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"선물 상자 보상 처리중 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L"선물 상자 보상 처리 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    if( !IS_CORRECT( NET_OK ) )
        SEND_PACKET( EVENT_PRESENTBOX_ACTION_ACK );
}

IMPL_ON_FUNC( EVENT_PRESENTBOX_ACTION_ACK )
{
    KItemPtr pkItem;
    std::set<GCITEMID>::iterator sit;
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

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket_.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_PRESENTBOX_00,        kPacket_.m_nOK, 1 );    // 선물상자 이벤트 기간이 아님. 
        ERR_CASE( ERR_PRESENTBOX_02,        kPacket_.m_nOK, 2 );    // 선물상자 조각 아이템이 존재 하지 않음. 
        ERR_CASE( ERR_PRESENTBOX_03,        kPacket_.m_nOK, 3 );    // 선물상자 교환 아이템 정보가 없습니다. 
        ERR_CASE( ERR_PRESENTBOX_04,        kPacket_.m_nOK, 4 );    // 꽝입니다.
        ERR_CASE( ERR_PRESENTBOX_05,        kPacket_.m_nOK, 5 );    // 로터리에 등록 되지 않음.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket_.m_nOK, -98 );  // 아직 처리중인 작업입니다.
    default:
        kPacket_.m_nOK = 99;
        START_LOG( cerr, L"선물 상자 보상 처리중 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_nOK ) << END_LOG;
    }

    _LOG_CONDITION( kPacket_.m_nOK == 0, clog, cwarn, L"선물 Event 아이템 교환요청 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_setItemList.size() )
        << BUILD_LOG( kPacket_.m_vecUseItem.size() )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_PRESENTBOX_ACTION_ACK );
}

_IMPL_ON_FUNC( EVENT_DUNGEON_STATE_REQ, std::set<int> )
{
    std::map<int,std::vector<KGameModeState> > kPacket;
    if ( !m_pkChannel )
        goto END_PROC;

    if ( m_pkChannel->GetType() != KChannel::CT_DUNGEON )
        goto END_PROC;

    m_pkChannel->GetGameModeState( kPacket_, kPacket );

END_PROC:
    SEND_PACKET( EVENT_DUNGEON_STATE_ACK );
    _LOG_CONDITION( kPacket.empty() == false, clog, cwarn, L"Size : " << kPacket.size() )
        << BUILD_LOG( kPacket_.size() ) << END_LOG;
}
IMPL_ON_FUNC( EVENT_GUILD_LIST_NOT )
{
    START_LOG( clog, L"Guild List Notice. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nPageNum )
        << BUILD_LOG( kPacket_.m_nTotalPageNum ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_GUILD_LIST_NOT );
}

_IMPL_ON_FUNC( EVENT_GUIDE_BOOL_CHECKPOINT_NOT, PAIR_SHORT )
{
    QUEUEING_EVENT_TO_DB( EVENT_GUIDE_BOOL_CHECKPOINT_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EVENT_UPDATE_RECOM_USER_INFO, PAIR_INT )
{
    SetNewRecomUser( kPacket_ );

    START_LOG( clog, L"Update Recom Info. LoginID : " << GetName() )
        << BUILD_LOG( kPacket_.first ) // 달성 레벨
        << BUILD_LOG( kPacket_.second ) << END_LOG; // 받아간 보상 아이템 레벨
}

IMPL_ON_FUNC_NOPARAM( EVENT_NORMAL_EXIT_NOT )
{
    SetDisconnReason( DRW_NORMAL_EXIT );
    
    START_LOG( clog, L"Exit program nomarlly. LoginID : " << GetName() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_CLIENT_PING_REPORT_NOT, DWORD )
{
    m_kPingInfo.AddPing( kPacket_ );
    if ( m_pkRoom ) {
        m_pkRoom->UpdateHostOrderByPing();
    }
}

IMPL_ON_FUNC_NOPARAM( EVENT_ROOM_MEMBER_PING_INFO_REQ )
{
    std::vector<std::pair<DWORD,DWORD> > kPacket;
    if ( !m_pkRoom ) {
        kPacket.push_back( std::make_pair( GetUID(), m_kPingInfo.GetPing() ) );
    }
    else {

        m_pkRoom->GetAllMemberPingInfo( kPacket );
    }

    SEND_PACKET( EVENT_ROOM_MEMBER_PING_INFO_ACK );
    START_LOG( clog, L"방원들 Ping 정보 요청. Name : " << m_strName << L", Size : " << kPacket.size() ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_GET_ROOMUSER_PRESS_STATE_REQ )
{
    JIF( m_pkRoom );
    std::vector<std::pair<DWORD, KPressureState> > kPacket;
    m_pkRoom->GetPressState( kPacket );
    SEND_PACKET( EVENT_GET_ROOMUSER_PRESS_STATE_ACK );
    START_LOG( clog, L"방원들의 압박 상태 전송. Name : " << m_strName << L", Size : " << kPacket.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_SET_PRESS_STATE_REQ, int )
{
    JIF( m_pkRoom );
    JIF( KPressureState::PS_NORMAL <= kPacket_ && KPressureState::PS_MAX > kPacket_ );

    SetPressInfo( kPacket_ );
    std::pair<DWORD, KPressureState> kPacket;
    kPacket.first = GetUID();
    GetPressInfo( kPacket.second.m_nState, kPacket.second.m_dwElapsedTime );
    m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_PRESS_STATE_NOT, kPacket );
}

_IMPL_ON_FUNC( EVENT_PRESSURE_USER_REQ, DWORD )
{
    JIF( m_pkChannel );
    JIF( m_pkRoom );

    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_ );
    if ( !spUser ) {
        START_LOG( cwarn, L"대상 유저가 서버에 존재하지 않음.. Name : " << m_strName << L", UID : " << kPacket_ ) << END_LOG;
        return;
    }

    if ( !spUser->m_pkChannel ) {
        START_LOG( cwarn, L"대상 유저의 채널 정보가 존재하지않음. Name : " << spUser->m_strName ) << END_LOG;
        return;
    }

    if ( spUser->m_pkChannel->GetUID() != m_pkChannel->GetUID() ) {
        START_LOG( cwarn, L"대상 유저가 같은 채널에 있지 않음. Nmer : " << spUser->m_strName ) << END_LOG;
        return;
    }

    if ( !spUser->m_pkRoom ) {
        START_LOG( cwarn, L"대상 유저가 룸에 존재하지않음. Name : " << spUser->m_strName ) << END_LOG;
        return;
    }

    if ( spUser->m_pkRoom->GetRoomID() != m_pkRoom->GetRoomID() ) {
        START_LOG( cwarn, L"대상 유저가 같은 방에 있지 않음. Nmer : " << spUser->m_strName ) << END_LOG;
        return;
    }

    int nState = KPressureState::PS_NORMAL;
    DWORD dwElapsedTime = 0;
    spUser->GetPressInfo( nState, dwElapsedTime );

    if ( nState != KPressureState::PS_PRESS ) {
        START_LOG( clog, L"압박 실패.. Name : " << m_strName << L", State : " << nState ) << END_LOG;
        return;
    }

    if ( nState == KPressureState::PS_PRESS ) {
        spUser->SetPressInfo( KPressureState::PS_PRESSED );
        KEVENT_PRESSURE_USER_NOT kPacket;
        kPacket.m_dwRequester = GetUID();
        kPacket.m_dwRequestee = spUser->GetUID();
        kPacket.m_nPressState = KPressureState::PS_PRESSED;
        m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_PRESSURE_USER_NOT, kPacket );

        START_LOG( clog, L"누군가를 압박 하였음.  " << m_strName << L" -> " << spUser->GetName() ) << END_LOG;
    }

}

IMPL_ON_FUNC_NOPARAM( EVENT_GET_ROOMUSER_IDLE_STATE_REQ )
{
    JIF( m_pkRoom );
    std::vector<std::pair<DWORD, int> > kPacket;
    m_pkRoom->GetIdleState( kPacket );
    SEND_PACKET( EVENT_GET_ROOMUSER_IDLE_STATE_ACK );
    START_LOG( clog, L"방원들의 자리비움 상태 전송. Name : " << m_strName << L", Size : " << kPacket.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_SET_IDLE_STATE_REQ, int )
{
    SetIdleInfo( kPacket_ == 1 );
    std::pair<DWORD, int > kPacket;
    kPacket.first = GetUID();
    GetIdleInfo( kPacket.second );

    if ( m_pkRoom )
        m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_IDLE_STATE_NOT, kPacket );
    else
        SEND_PACKET( EVENT_IDLE_STATE_NOT );
}

_IMPL_ON_FUNC( EVENT_RELAY_LOADING_STATE, PAIR_DWORD_INT )
{
    JIF( m_pkRoom );

    m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, GetName(), KUserEvent::EVENT_RELAY_LOADING_STATE, kPacket_ );

    START_LOG( clog, L"로딩상태 릴레이" )
        << BUILD_LOG( kPacket_.first )
        << BUILD_LOG( kPacket_.second ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_CHANGE_NORMAL_TO_LOOK_REQ, std::vector<GCITEMUID> )
{
    KChangeLookItem kPacket;
    kPacket.m_nOK = -99;
    KItemPtr spChangeItem;
    std::vector<GCITEMUID>::iterator vit;
    KDB_EVENT_CHANGE_NORMAL_TO_LOOK_REQ kDBPacket;

    SET_ERROR( ERR_UNKNOWN );

    if ( kPacket_.empty() ) {
        SET_ERR_GOTO( ERR_LOOK_EQUIP_00, END_PROC );
    }

    // 충분한 공간은 있는가?
    if ( m_kInventory.GetLookItemNum() >= GetTrueLookInvenCapacity() ) {
        SET_ERR_GOTO( ERR_LOOK_EQUIP_16, END_PROC );
    }

    // 충분한 량의 교환 아이템을 가지고 있는가?
    spChangeItem = m_kInventory.FindDurationItemByItemID( SiKLookEquipment()->GetChangeItemID() );
    if ( !spChangeItem ) {
        SET_ERR_GOTO( ERR_LOOK_EQUIP_06, END_PROC );
    }

    if ( spChangeItem->m_nCount < (int) kPacket_.size() ) {
        SET_ERR_GOTO( ERR_LOOK_EQUIP_06, END_PROC );
    }
    kDBPacket.m_kItem = *spChangeItem;

    for ( vit = kPacket_.begin() ; vit != kPacket_.end() ; ++vit ) {
        KItemPtr spItem = m_kInventory.FindItemByItemUID( *vit );
        if ( !spItem ) {
            // 해당 아이템을 보유 중인가?
            SET_ERR_GOTO( ERR_LOOK_EQUIP_01, END_PROC );
        }

        if ( spItem->CheckItemType(KItem::TYPE_LOOK) ) {
            //변경 할려는 아이템이 현재 일반 아이템이 아님
            SET_ERR_GOTO( ERR_LOOK_EQUIP_03, END_PROC );
        }

        if ( SiKLookEquipment()->IsLookBanItem( spItem->m_ItemID ) ) {
            START_LOG( cwarn, L"변경 할려는 아이템이 룩변환 금지 아이템. ItemID : " << spItem->m_ItemID ) << END_LOG;
            SET_ERR_GOTO( ERR_LOOK_EQUIP_17, END_PROC );
        }

        kDBPacket.m_vecItem.push_back( *spItem );

        // 클라이언트에서 장착 변경 패킷을 보내는 시점이 다르기 때문에..ㅜㅜ
        //if ( IsEquipedItem( spItem->m_ItemUID ) ) {
        //    // 변경 할려는 아이템을 장착 하고 있음
        //    SET_ERR_GOTO( ERR_LOOK_EQUIP_05, END_PROC );
        //}
    }

    // 교환 시도 하자.
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_CHANGE_NORMAL_TO_LOOK_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_LOOK_EQUIP_00, kPacket.m_nOK, 1 ); // 요청 패킷이 비어 있음.
        ERR_CASE( ERR_LOOK_EQUIP_06, kPacket.m_nOK, 2 ); // 교환 아이템의 수량이 부족.
        ERR_CASE( ERR_LOOK_EQUIP_01, kPacket.m_nOK, 3 ); // 변경 하려는 아이템을 보유하고 있지 않음
        ERR_CASE( ERR_LOOK_EQUIP_03, kPacket.m_nOK, 4 ); // 변경 할려는 아이템이 현재 일반 아이템이 아님
        ERR_CASE( ERR_LOOK_EQUIP_05, kPacket.m_nOK, 5 ); // 변경 할려는 아이템을 장착 하고 있음.
        ERR_CASE( ERR_LOOK_EQUIP_07, kPacket.m_nOK, 6 ); // 룩 아이템으로 변환중 DB 오류.
        ERR_CASE( ERR_LOOK_EQUIP_16, kPacket.m_nOK, 7 ); // 룩 인벤크기 초과.
        ERR_CASE( ERR_LOOK_EQUIP_17, kPacket.m_nOK, 8 ); // 변경 할려는 아이템이 룩변환 금지 아이템
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 이미 처리 중임..
    default:
        START_LOG( cerr, L"EVENT_CHANGE_NOMAL_TO_LOOK_REQ 요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"Name : " << m_strName << L", OK : " << kPacket.m_nOK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kDBPacket.m_vecItem.size() ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_CHANGE_NORMAL_TO_LOOK_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_CHANGE_NORMAL_TO_LOOK_ACK, KChangeLookItem )
{
    JIF( nFrom_ == KUserEvent::FT_INNER );
    KItemPtr spChangeItem;
    std::vector<KItem>::iterator vit;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) && !IS_CORRECT( ERR_LOOK_EQUIP_19 ) ) {
        goto END_PROC;
    }

    //  룩 변환 아이템 제거 하고
    spChangeItem = m_kInventory.FindDurationItemByItemID( SiKLookEquipment()->GetChangeItemID() );
    if ( spChangeItem ) {
        int nCount = (int)kPacket_.m_vecItems.size();
        spChangeItem->m_nCount = kPacket_.m_kItem.m_nCount; //std::max<int>( 0, spChangeItem->m_nCount - nCount );
        spChangeItem->m_nInitCount = spChangeItem->m_nCount;
        kPacket_.m_vecChangeItem.push_back( *spChangeItem );
    }

    if ( spChangeItem && spChangeItem->m_nCount <= 0 ) {
        m_kInventory.RemoveItem( kPacket_.m_kItem.m_ItemID, kPacket_.m_kItem.m_ItemUID );
    }

    // 아이템 룩변경 적용 (DB스레드에서 변경함)
    for ( vit = kPacket_.m_vecItems.begin() ; vit != kPacket_.m_vecItems.end() ; ++vit ) {
        KItemPtr spItem = m_kInventory.FindItemByItemUID( vit->m_ItemUID );
        if ( spItem ) {
            *spItem = *vit;
            START_LOG( clog, L"아이템 룩변경 적용..  ItemID : " << spItem->m_ItemID << L", ItemUID : " << spItem->m_ItemUID ) << END_LOG;
        }
    }

    SET_ERROR( NET_OK );
END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_LOOK_EQUIP_00, kPacket_.m_nOK, 1 ); // 요청 패킷이 비어 있음.
        ERR_CASE( ERR_LOOK_EQUIP_06, kPacket_.m_nOK, 2 ); // 교환 아이템의 수량이 부족.
        ERR_CASE( ERR_LOOK_EQUIP_01, kPacket_.m_nOK, 3 ); // 변경 하려는 아이템을 보유하고 있지 않음
        ERR_CASE( ERR_LOOK_EQUIP_03, kPacket_.m_nOK, 4 ); // 변경 할려는 아이템이 현재 일반 아이템이 아님
        ERR_CASE( ERR_LOOK_EQUIP_05, kPacket_.m_nOK, 5 ); // 변경 할려는 아이템을 장착 하고 있음.
        ERR_CASE( ERR_LOOK_EQUIP_07, kPacket_.m_nOK, 6 ); // 룩 아이템으로 변환중 DB 오류.
        ERR_CASE( ERR_LOOK_EQUIP_16, kPacket_.m_nOK, 7 ); // 룩 인벤크기 초과.
        ERR_CASE( ERR_LOOK_EQUIP_17, kPacket_.m_nOK, 8 ); // 변경 할려는 아이템이 룩변환 금지 아이템
        ERR_CASE( ERR_LOOK_EQUIP_19, kPacket_.m_nOK, 9 ); // 코디 변경 아이템 DB 동기화 실패
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 이미 처리 중임..
    default:
        START_LOG( cerr, L"응답 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"Name : " << m_strName << L", OK : " << kPacket_.m_nOK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_vecItems.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CHANGE_NORMAL_TO_LOOK_ACK );
}

_IMPL_ON_FUNC( EVENT_CHANGE_LOOK_TO_NORMAL_REQ, std::vector<GCITEMUID> )
{
    KChangeLookItem kPacket;
    kPacket.m_nOK = -99;
    KItemPtr spChangeItem;
    std::vector<GCITEMUID>::iterator vit;

    SET_ERROR( ERR_UNKNOWN );

    if ( kPacket_.empty() ) {
        SET_ERR_GOTO( ERR_LOOK_EQUIP_00, END_PROC );
    }

    for ( vit = kPacket_.begin() ; vit != kPacket_.end() ; ++vit ) {
        KItemPtr spItem = m_kInventory.FindItemByItemUID( *vit );
        if ( !spItem ) {
            // 해당 아이템을 보유 중인가?
            SET_ERR_GOTO( ERR_LOOK_EQUIP_01, END_PROC );
        }

        if ( false == spItem->CheckItemType(KItem::TYPE_LOOK) ) {
            START_LOG( cwarn, L"변경 할려는 아이템이 현재 룩용 아이템이 아님..  ItemID : " << spItem->m_ItemID << L", ItemUID : " << spItem->m_ItemUID ) << END_LOG;
            SET_ERR_GOTO( ERR_LOOK_EQUIP_02, END_PROC );
        }

        if ( SiKLookEquipment()->IsNormalBanItem( spItem->m_ItemID ) ) {
            START_LOG( cwarn, L"변경 할려는 아이템이 일반변환 금지 아이템. ItemID : " << spItem->m_ItemID ) << END_LOG;
            SET_ERR_GOTO( ERR_LOOK_EQUIP_18, END_PROC );
        }

        kPacket.m_vecItems.push_back( *spItem );

        //if ( IsEquipedLookItem( spItem->m_ItemUID ) ) {
        //    // 변경 할려는 아이템을 장착 하고 있음
        //    SET_ERR_GOTO( ERR_LOOK_EQUIP_05, END_PROC );
        //}
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_CHANGE_LOOK_TO_NORMAL_REQ, kPacket );
    
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_LOOK_EQUIP_00, kPacket.m_nOK, 1 ); // 요청 패킷이 비어 있음.
        ERR_CASE( ERR_LOOK_EQUIP_01, kPacket.m_nOK, 2 ); // 변경 하려는 아이템을 보유하고 있지 않음
        ERR_CASE( ERR_LOOK_EQUIP_02, kPacket.m_nOK, 3 ); // 변경 할려는 아이템이 현재 룩이 아님
        ERR_CASE( ERR_LOOK_EQUIP_05, kPacket.m_nOK, 4 ); // 변경 할려는 아이템을 장착 하고 있음.
        ERR_CASE( ERR_LOOK_EQUIP_08, kPacket.m_nOK, 5 ); // 일반 아이템으로 변환중 DB 오류.
        ERR_CASE( ERR_LOOK_EQUIP_18, kPacket.m_nOK, 6 ); // 변경 할려는 아이템이 일반변환 금지 아이템
    
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 이미 처리 중임..
    default:
        START_LOG( cerr, L"요청 중 알수 없는 오류 Name : " << m_strName )
    	    << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }
    
    _LOG_SUCCESS( kPacket.m_nOK == 0, L"Name : " << m_strName << L", OK : " << kPacket.m_nOK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.size() ) << END_LOG;
    
    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_CHANGE_LOOK_TO_NORMAL_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_CHANGE_LOOK_TO_NORMAL_ACK, KChangeLookItem )
{
    JIF( nFrom_ == KUserEvent::FT_INNER );
    KItemPtr spChangeItem;
    std::vector<KItem>::iterator vit;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    // 아이템 룩해제 적용 (DB스레드에서 변경)
    for ( vit = kPacket_.m_vecItems.begin() ; vit != kPacket_.m_vecItems.end() ; ++vit ) {
        KItemPtr spItem = m_kInventory.FindItemByItemUID( vit->m_ItemUID );
        if ( spItem ) {
            *spItem = *vit;
            START_LOG( clog, L"아이템 룩해제 적용..  ItemID : " << spItem->m_ItemID << L", ItemUID : " << spItem->m_ItemUID ) << END_LOG;
        }
    }

    SET_ERROR( NET_OK );
END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_LOOK_EQUIP_00, kPacket_.m_nOK, 1 ); // 요청 패킷이 비어 있음.
        ERR_CASE( ERR_LOOK_EQUIP_01, kPacket_.m_nOK, 2 ); // 변경 하려는 아이템을 보유하고 있지 않음
        ERR_CASE( ERR_LOOK_EQUIP_02, kPacket_.m_nOK, 3 ); // 변경 할려는 아이템이 현재 룩이 아님
        ERR_CASE( ERR_LOOK_EQUIP_05, kPacket_.m_nOK, 4 ); // 변경 할려는 아이템을 장착 하고 있음.
        ERR_CASE( ERR_LOOK_EQUIP_08, kPacket_.m_nOK, 5 ); // 일반 아이템으로 변환중 DB 오류.
        ERR_CASE( ERR_LOOK_EQUIP_18, kPacket_.m_nOK, 6 ); // 변경 할려는 아이템이 일반변환 금지 아이템

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 이미 처리 중임..
    default:
        START_LOG( cerr, L"응답 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"Name : " << m_strName << L", OK : " << kPacket_.m_nOK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_vecItems.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CHANGE_LOOK_TO_NORMAL_ACK );
}

_IMPL_ON_FUNC( EVENT_SAVE_COORDI_REQ, KSaveCoordiInfo )
{
    KLookEquipChar* pkLookEquip = NULL;
    std::set<GCITEMUID>::iterator sit;
    int kPacket = -99;

    SET_ERROR( ERR_UNKNOWN );
    // 코디 이름 길이
    if ( (int)kPacket_.m_kCoordiInfo.m_strCoordiName.size() > SiKLookEquipment()->GetCoordiNameLength() ) {
        SET_ERR_GOTO( ERR_LOOK_EQUIP_15, END_PROC );
    }

    // 해당 캐릭터가 존재 하는가?
    if ( false == IsCharExist( kPacket_.m_cCharType ) ) {
        SET_ERR_GOTO( ERR_LOOK_EQUIP_09, END_PROC );
    }

    // 해당 캐릭터의 코디 정보가 없음.
    pkLookEquip = GetLookEquip( kPacket_.m_cCharType );
    if ( !pkLookEquip ) {
        SET_ERR_GOTO( ERR_LOOK_EQUIP_10, END_PROC );
    }

    // 코디 번호는 유효 한가?
    if ( pkLookEquip->VerifySaveCoordiID( kPacket_.m_kCoordiInfo.m_nCoordiID ) ) {
        SET_ERR_GOTO( ERR_LOOK_EQUIP_11, END_PROC );
    }
    
    for ( sit = kPacket_.m_kCoordiInfo.m_setEquip.begin() ; sit != kPacket_.m_kCoordiInfo.m_setEquip.end() ; ++sit ) {
        // 해당 아이템들을 보유 하고 있는가?
        KItemPtr spItem = m_kInventory.FindItemByItemUID( *sit );
        if ( !spItem ) {
            SET_ERR_GOTO( ERR_LOOK_EQUIP_12, END_PROC );
        }

        // 해당 아이템이 룩 아이템인가?
        if ( !spItem->CheckItemType(KItem::TYPE_LOOK) ) {
            SET_ERR_GOTO( ERR_LOOK_EQUIP_13, END_PROC );
        }
    }

    LIF( pkLookEquip->SaveCoordi( kPacket_.m_kCoordiInfo ) );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket, 0 );
        ERR_CASE( ERR_LOOK_EQUIP_09, kPacket, 1 ); // 코디 저장시 없는 캐릭터 정보를 보내 왔음.
        ERR_CASE( ERR_LOOK_EQUIP_10, kPacket, 2 ); // 해당 캐릭터의 코디 정보가 없음.
        ERR_CASE( ERR_LOOK_EQUIP_11, kPacket, 3 ); // 저장하려는 코디의 코디 ID 가 범위를 벗어났음.
        ERR_CASE( ERR_LOOK_EQUIP_12, kPacket, 4 ); // 해당 아이템을 보유 하고 있지 않음.
        ERR_CASE( ERR_LOOK_EQUIP_13, kPacket, 5 ); // 해당 아이템은 룩 아이템이 아닙니다.
        ERR_CASE( ERR_LOOK_EQUIP_15, kPacket, 6 ); // 코디 이름 길이 초과.

    default:
        START_LOG( cerr, L"요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket = -99;
    }

    _LOG_SUCCESS( kPacket == 0, L"Name : " << m_strName << L", OK : " << kPacket )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_kCoordiInfo.m_nCoordiID )
        << BUILD_LOG( kPacket_.m_kCoordiInfo.m_strCoordiName )
        << BUILD_LOG( kPacket_.m_kCoordiInfo.m_setEquip.size() ) << END_LOG;

    SEND_PACKET( EVENT_SAVE_COORDI_ACK );
}

IMPL_ON_FUNC( EVENT_CHANGE_LOOK_EQUIP_REQ )
{
    int nOK = -99;
    KChangeLookEquipInfo kPacket;
    kPacket.m_dwUID = GetUID();
    std::map<char,KCoordiDiffInfo>::iterator mit;

    SET_ERROR( ERR_UNKNOWN );
    // 캐릭터 유효 한가?
    for ( mit = kPacket_.m_mapChangeLookEquip.begin() ; mit != kPacket_.m_mapChangeLookEquip.end() ; ++mit ) {
        if ( false == IsCharExist( mit->first ) ) {
            SET_ERR_GOTO( ERR_LOOK_EQUIP_14, END_PROC );
        }
    }

    // 장착 할려는 아이템이 내가 보유 하고 있는것인가?
    for ( mit = kPacket_.m_mapChangeLookEquip.begin() ; mit != kPacket_.m_mapChangeLookEquip.end() ; ++mit ) {
        if ( !IsLookItem( mit->second.m_setEquip ) ) {
            SET_ERR_GOTO( ERR_LOOK_EQUIP_13, END_PROC );
        }
    }

    // 해제 먼저
    ReleaseCurrentLookEquip( kPacket_.m_mapChangeLookEquip );
    // 장착 나중
    EquipCurrentLookEquip( kPacket_.m_mapChangeLookEquip );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, nOK, 0 );
        ERR_CASE( ERR_LOOK_EQUIP_13, nOK, 1 ); // 해당 아이템은 룩 아이템이 아닙니다.

    default:
        START_LOG( cerr, L"요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        nOK = -99;
    }

    ExtractCurrentLookItem( kPacket.m_mapCurrentLookEquip );
    if ( nOK == 0 ) { // 성공 & 룸.
        if ( m_pkRoom ) {
            // IndoorUserInfo 변경 해준다.
            m_pkRoom->UpdateLookEquip( m_strName, kPacket.m_mapCurrentLookEquip );
            // 자신과 방원들에게 NOT 보내기.
            m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL_EX_ME, m_strName, KUserEvent::EVENT_CHANGE_LOOK_EQUIP_NOT, kPacket, true );
        }
        SEND_COMPRESS_PACKET( EVENT_CHANGE_LOOK_EQUIP_NOT, kPacket );
    }
    else {
        SEND_COMPRESS_PACKET( EVENT_CHANGE_LOOK_EQUIP_ACK, kPacket );
    }
    _LOG_SUCCESS( nOK == 0, L"룩 장착, Name : " << GetName() << L", OK : " << nOK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( (bool)(m_pkRoom != NULL) )
        << BUILD_LOG( kPacket_.m_mapChangeLookEquip.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_FULL_LOOK_INFO_NOT, KLookFullInfo )
{
    JIF( nFrom_ == KUserEvent::FT_INNER );

    m_mapLookEquip.clear();
    std::map<char, KCharacterInfo>::const_iterator cmit;
    for ( cmit = m_mapCharacterInfo.begin() ; cmit != m_mapCharacterInfo.end() ; ++cmit ) {
        std::map<char, std::map<int,KCoordiInfo> >::iterator mitLookEqip;
        mitLookEqip = kPacket_.m_mapLookFullInfo.find( cmit->first );
        KLookEquipChar kLookEquip;
        kLookEquip.SetCharacter( cmit->first );
        if ( mitLookEqip != kPacket_.m_mapLookFullInfo.end() ) {
            kLookEquip.SetFullCoordiInfo( mitLookEqip->second );
        }

        LIF( m_mapLookEquip.insert( std::map<char,KLookEquipChar>::value_type( kLookEquip.GetCharacter(), kLookEquip ) ).second );
    }
    kPacket_.m_changeItemID = SiKLookEquipment()->GetChangeItemID();
    kPacket_.m_nMaxLookInvenSize = GetTrueLookInvenCapacity();

    SEND_COMPRESS_PACKET( EVENT_FULL_LOOK_INFO_NOT, kPacket_ );
}

IMPL_ON_FUNC( EVENT_BUNDLE_SELL_ITEM_REQ )
{
    KEVENT_BUNDLE_SELL_ITEM_ACK kPacket;
    KDB_EVENT_BUNDLE_SELL_ITEM_REQ kDBPacket;
    SET_ERROR( ERR_UNKNOWN );

    if ( kPacket_.m_vecEraseList.empty() && kPacket_.m_vecSellCountItem.empty() && kPacket_.m_vecSellNormalItem.empty() ) {
        // 빈 패킷.
        SET_ERR_GOTO( ERR_BUNDLE_SELL_07, END_PROC );
    }

    if ( CheckBundleSellDuplicate( kPacket_ ) ) {
        SET_ERR_GOTO( ERR_BUNDLE_SELL_00, END_PROC ); // 중복 아이템이 있습니다.
    }

    // 실패된 아이템에 대한 처리.
    VerifyBundleSellCount( kPacket_.m_vecSellCountItem, kPacket.m_mapFailInfo );
    VerifyBundleSellNormal( kPacket_.m_vecSellNormalItem, kPacket.m_mapFailInfo );
    VerifyBundleSellErase( kPacket_.m_vecEraseList, kPacket.m_mapFailInfo );

    if ( kPacket_.m_vecEraseList.empty() && kPacket_.m_vecSellCountItem.empty() && kPacket_.m_vecSellNormalItem.empty() ) {
        // 전부 오류..
        SET_ERR_GOTO( ERR_BUNDLE_SELL_08, END_PROC );
    }
    // 이제 DB 로 갈 패킷을 처리..
    kDBPacket.m_vecEraseList = kPacket_.m_vecEraseList;
    BundleSellExtractSellNormalItems( kPacket_.m_vecSellNormalItem, kDBPacket.m_vecSellNormalItem );
    BundleSellExtractSellCountItems( kPacket_.m_vecSellCountItem, kDBPacket.m_vecSellCountItem );


    GetDBUpdateData( kDBPacket.m_kData );
    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_BUNDLE_SELL_ITEM_REQ, kDBPacket );
    
    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = GetBundleSelItemErrNo( NetError::GetLastNetError() );
    
    _LOG_SUCCESS( kPacket.m_nOK == 0, L"Name : " << m_strName << L", OK : " << kPacket.m_nOK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_vecEraseList.size() )
        << BUILD_LOG( kPacket_.m_vecSellCountItem.size() )
        << BUILD_LOG( kPacket_.m_vecSellNormalItem.size() )
        << BUILD_LOG( kPacket.m_mapFailInfo.size() )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;
    
    if ( kPacket.m_nOK != 0 ) {
        std::map<GCITEMUID,int>::iterator mit;
        for ( mit = kPacket.m_mapFailInfo.begin() ; mit != kPacket.m_mapFailInfo.end() ; ++mit ) {
            mit->second = GetBundleSelItemErrNo( mit->second );
            if ( mit->second != 0 ) {
                START_LOG( cerr, L"아이템 묶어팔기 에러값 출력 : " << mit->second ) << END_LOG;
            }

        }
        SEND_PACKET( EVENT_BUNDLE_SELL_ITEM_ACK );
    }
}

IMPL_ON_FUNC( EVENT_BUNDLE_SELL_ITEM_ACK )
{
    std::map<GCITEMUID,int>::iterator mit;
    std::vector<KItem>::iterator vit;
    JIF( nFrom_ == KUserEvent::FT_INNER );
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    if ( kPacket_.m_vecEraseList.empty() && kPacket_.m_vecNormalItem.empty() && kPacket_.m_vecCountItem.empty() ) {
        // 빈 패킷.
        SET_ERR_GOTO( ERR_BUNDLE_SELL_07, END_PROC );
    }

    RemoveItemFromInven( kPacket_.m_vecEraseList );
    RemoveItemFromInven( kPacket_.m_vecNormalItem );
    // 수량 동기화
    for ( vit = kPacket_.m_vecCountItem.begin() ; vit != kPacket_.m_vecCountItem.end() ; ++vit ) {
        KItemPtr spItem = m_kInventory.FindDurationItemByItemUID( vit->m_ItemUID );
        if ( !spItem )
            continue;
        spItem->m_nCount = std::max<int>( 0, vit->m_nCount );
        spItem->m_nInitCount = std::max<int>( 0, vit->m_nInitCount );
        if ( spItem->m_nCount <= 0 ) {
            m_kInventory.RemoveItem( spItem->m_ItemUID );
        }
    }

    // 획득 아이템 넣기
    m_kInventory.AddItemList( kPacket_.m_vecRewardItem );

    if ( kPacket_.m_nGamePoint > 0 ) {
        SetGP( kPacket_.m_nGamePoint );
        SetInitGP( GetGP() );
    }

END_PROC:
    kPacket_.m_nOK = GetBundleSelItemErrNo( NetError::GetLastNetError() );
    // 오류 값 설정.
    for ( mit = kPacket_.m_mapFailInfo.begin() ; mit != kPacket_.m_mapFailInfo.end() ; ++mit ) {
        mit->second = GetBundleSelItemErrNo( mit->second );
    }
    kPacket_.m_nGamePoint = GetGP();

    SEND_RECEIVED_PACKET( EVENT_BUNDLE_SELL_ITEM_ACK );
    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"묶어 팔기 응답. Name : " << GetName()
        << L", OK : " << kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nGamePoint )
        << BUILD_LOG( kPacket_.m_vecRewardItem.size() )
        << BUILD_LOG( kPacket_.m_vecEraseList.size() )
        << BUILD_LOG( kPacket_.m_vecNormalItem.size() )
        << BUILD_LOG( kPacket_.m_vecCountItem.size() )
        << BUILD_LOG( kPacket_.m_mapFailInfo.size() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_BUY_FOR_GAMBLE_REQ )
{
    _VERIFY_STATE(( 2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL ));

    KEVENT_BUY_FOR_GAMBLE_ACK kPacket;
    KDB_EVENT_BUY_FOR_GAMBLE_REQ kDBPacket;
    std::vector<GCITEMID>::iterator vit;

    SET_ERROR( ERR_UNKNOWN );

    if ( GetGP() <= 0 ) {
        SET_ERR_GOTO( ERR_BUY_GAMBLE_00, END_PROC );
    }

    GetDBUpdateData( kDBPacket.m_kData );
    kDBPacket.m_vecBuyInfo.reserve( kPacket_.m_vecItemID.size() );

    for ( vit = kPacket_.m_vecItemID.begin() ; vit != kPacket_.m_vecItemID.end() ; ++vit ) {
        KBuySellItemReq kItem;
        kItem.m_ItemID = *vit;
        kItem.m_nCount = kPacket_.m_nDuration;
        kItem.m_nPeriod = kPacket_.m_nPeriod;
        FillBuyerData( kItem );

        kDBPacket.m_vecBuyInfo.push_back( kItem );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_BUY_FOR_GAMBLE_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_BUY_GAMBLE_00,        kPacket.m_nOK, 1 );    // GP부족
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );  // 아직 처리중인 작업입니다.
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_vecItemID.size() )
        << BUILD_LOG( kPacket_.m_nDuration )
        << BUILD_LOG( kPacket_.m_nPeriod ) << END_LOG;

    if ( !IS_CORRECT( NET_OK ) ) {
        SEND_PACKET( EVENT_BUY_FOR_GAMBLE_ACK );
    }
}

IMPL_ON_FUNC( EVENT_BUY_FOR_GAMBLE_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) && kPacket_.m_vecItems.empty() ) {
        goto END_PROC;
    }

    __LIF( kPacket_.m_nUseGP > 0 );
    bool bDBUpdate = false; // DB에서 GP 갱신 처리 하지 않았음.
    DecreaseGP( KGpStatInfo::GDT_GAMEBLE_BUY, bDBUpdate, kPacket_.m_nUseGP );
    kPacket_.m_nUseGP = GetGP();

    m_kInventory.AddItemList( kPacket_.m_vecItems );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,            kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_BUY_GAMBLE_00, kPacket_.m_nOK, 1 ); // GP부족
        ERR_CASE( ERR_BUY_GAMBLE_01, kPacket_.m_nOK, 2 ); // 필수데이터 동기화 실패.
        ERR_CASE( ERR_BUY_GAMBLE_02, kPacket_.m_nOK, 3 ); // GP아이템이 아니다.
        ERR_CASE( ERR_BUY_GAMBLE_03, kPacket_.m_nOK, 4 ); // DB아이템 넣기 작업 실패.
    default:
        kPacket_.m_nOK = 99;
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nUseGP )
        << BUILD_LOG( GetGP() )
        << BUILD_LOG( kPacket_.m_vecItems.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_BUY_FOR_GAMBLE_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_COST_RATE_FOR_GAMBLE_BUY_REQ )
{
    float fCostRate = SiKGambleBuyManager()->GetCostRate();
    SEND_DATA( EVENT_COST_RATE_FOR_GAMBLE_BUY_ACK, fCostRate );
}

IMPL_ON_FUNC( EVENT_STAT_END_GAME_INFO )
{
    KBossDungeonStat kData;
    std::vector<KBossDungeonStat>::iterator vit;
    for( vit = kPacket_.m_vecDungeonStat.begin(); vit != kPacket_.m_vecDungeonStat.end(); ++vit ) {
        KStatDungeonPtr spStat = GET_STAT( KStatDungeon, SI_STAT_DUNGEON );

        kData.m_dwDungeonID = vit->m_dwDungeonID;
        kData.m_dwDifficult = vit->m_dwDifficult;
        kData.m_dwClearType = vit->m_dwClearType;

        if( spStat != NULL )
        {
            spStat->AddBossStat( kData );
        }
    }
}

IMPL_ON_FUNC_NOPARAM( EVENT_TR_SERVER_INFO_REQ )
{
    static int siRelayKey = 0;
    siRelayKey++;

    PAIR_DWORD_USHORT kPacket;

    std::vector<KSimpleServerInfo> vecUseTCPServerInfo;
    std::vector<KSimpleServerInfo> vecNoUseTCPServerInfo;
    SiKGameServer()->GetTCPServerList( vecUseTCPServerInfo, vecNoUseTCPServerInfo );
    SiKGSNetLayer()->GetTRServerAddress( siRelayKey,
        vecUseTCPServerInfo,
        vecNoUseTCPServerInfo,
        kPacket.first,
        kPacket.second );
    SEND_DATA( EVENT_TR_SERVER_INFO_ACK, kPacket );
}

_IMPL_ON_FUNC( EVENT_CHANNELING_REWARD_NOT, std::vector<KItem> )
{
    JIF( PacketHackCheck( nFrom_ ) );
    // 획득한 아이템이 일반 (非 수량) 아이템인 경우
    m_kInventory.AddItemList( kPacket_ );

    SEND_RECEIVED_PACKET( EVENT_CHANNELING_REWARD_NOT );

    START_LOG( clog, L"클라이언트에게 채널링 이벤트 아이템 전달" << m_strName )
        << BUILD_LOG( kPacket_.size() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_CHAR_PROMOTION_UPDATE_REQ )
{
    KEVENT_CHAR_PROMOTION_UPDATE_ACK kPacket;
    kPacket.m_nOK = -99;
    KDB_EVENT_CHAR_PROMOTION_UPDATE_REQ kDBPacket;
    KItemPtr pkItem;
    KMissionInfo kMissionInfo;
    KMissionData* pkMissionData = NULL;
    std::vector<GCITEMID>::iterator vit;

    SET_ERROR( ERR_UNKNOWN );

    // 해당 아이템이 존재 하는가?
    pkItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_dwItemID );
    if ( !pkItem ) {
        SET_ERR_GOTO( ERR_CHAR_PROMOTION_UPDATE_00, END_PROC );
    }

    // 해당 아이템이 전직 가능 아이템인지 체크.
    int nOK = 0;
    if( !SiKCharPromotionSystem()->CheckEnableItem( kPacket_.m_dwItemID, kPacket_.m_cCharPromotion, nOK ) ) {
        if( 1 == nOK ) SET_ERR_GOTO( ERR_CHAR_PROMOTION_UPDATE_01, END_PROC );
        if( 2 == nOK ) SET_ERR_GOTO( ERR_CHAR_PROMOTION_UPDATE_05, END_PROC );
    }

    // 해당 전직이 현재 클라이언트가 보내준 전직 정보에서 가능한지 체크.
    const KCharacterInfo* pkCharacterInfo = GetCharacterInfo( kPacket_.m_cCharType );
    if( !pkCharacterInfo ) {
        SET_ERR_GOTO( ERR_CHAR_PROMOTION_UPDATE_04, END_PROC );
    } else {
        if( (int)pkCharacterInfo->m_cPromotion >= (int)kPacket_.m_cCharPromotion ) {
            SET_ERR_GOTO( ERR_CHAR_PROMOTION_UPDATE_05, END_PROC );
        }

        // 해당 캐릭터가 이전 전직조건을 만족하는지.
        int nPrevPromotion = (int)kPacket_.m_cCharPromotion - 1;
        if ( (int)pkCharacterInfo->m_cPromotion < nPrevPromotion ) {
            SET_ERR_GOTO( ERR_CHAR_PROMOTION_UPDATE_09, END_PROC );
        }
    }

    // 해당 캐릭터가 전직 수행 레벨이 되는지 체크.
    if( !SiKCharPromotionSystem()->CheckCharLevel( kPacket_.m_dwItemID, kPacket_.m_cCharType, pkCharacterInfo->m_dwLevel ) ) {
        SET_ERR_GOTO( ERR_CHAR_PROMOTION_UPDATE_09, END_PROC );
    }

    // DB로 넘기기 위한 데이터 세팅.
    kDBPacket.m_cCharType = kPacket_.m_cCharType;
    kDBPacket.m_cCharPromotion = kPacket_.m_cCharPromotion;
    kDBPacket.m_dwItemID = kPacket_.m_dwItemID;
    // 해당 캐릭터의 전직 수행 MissionID를 가져온다.
    if( !SiKCharPromotionSystem()->GetMissionID( kPacket_.m_dwItemID, kPacket_.m_cCharType, kDBPacket.m_dwMissionID ) ) {
        SET_ERR_GOTO( ERR_CHAR_PROMOTION_UPDATE_02, END_PROC );
    }

    // 스크립트에 등록된 미션이 존재하는가?
    if( !SiKMissionManager()->GetMissionInfo( kMissionInfo, kDBPacket.m_dwMissionID ) ) {
        SET_ERR_GOTO( ERR_CHAR_PROMOTION_UPDATE_06, END_PROC ); // 존재하는 미션이 아님.
    }

    //해당 미션이 나의 미션슬롯에 있는가?
    pkMissionData = GetMissionDataFromSlot( kDBPacket.m_dwMissionID );
    if( pkMissionData == NULL ) kDBPacket.m_nMissionState = 0; // 미션 슬롯에 없음.
    else kDBPacket.m_nMissionState = 1; // 미션 슬롯에 현재 등록되어 있음.

    // 보상 아이템이 있는지 체크.
    if( SiKCharPromotionSystem()->GetPromotionReward( kPacket_.m_dwItemID, kPacket_.m_cCharType, kDBPacket.m_vecRewards ) ) {
        START_LOG( clog, L"캐릭터 획득 시스템 해당 캐릭터 보상 아이템 존재하지 않음. CharType : " << kPacket_.m_cCharType ) << END_LOG;
    }

    SET_ERROR( NET_OK );
    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_CHAR_PROMOTION_UPDATE_REQ, kDBPacket );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_CHAR_PROMOTION_UPDATE_00, kPacket.m_nOK, 1 ); // 전직 마법서 아이템 가져오기 실패.
        ERR_CASE( ERR_CHAR_PROMOTION_UPDATE_01, kPacket.m_nOK, 2 ); // 해당 전직 변경 불가 아이템.
        ERR_CASE( ERR_CHAR_PROMOTION_UPDATE_02, kPacket.m_nOK, 3 ); // 캐릭터 미션정보가 없음.
        ERR_CASE( ERR_CHAR_PROMOTION_UPDATE_03, kPacket.m_nOK, 4 ); // 캐릭터 전직변경 DB 처리실패.
        ERR_CASE( ERR_CHAR_PROMOTION_UPDATE_04, kPacket.m_nOK, 5 ); // 캐릭터 정보 가져오기 실패.
        ERR_CASE( ERR_CHAR_PROMOTION_UPDATE_05, kPacket.m_nOK, 6 ); // 해당 전직으로 변경 불가.
        ERR_CASE( ERR_CHAR_PROMOTION_UPDATE_06, kPacket.m_nOK, 7 ); // 등록되지 않은 미션.
        ERR_CASE( ERR_CHAR_PROMOTION_UPDATE_07, kPacket.m_nOK, 8 ); // 유저가 존재하지 않음.
        ERR_CASE( ERR_CHAR_PROMOTION_UPDATE_08, kPacket.m_nOK, 9 ); // 미션이 등록되어 있지 않음.
        ERR_CASE( ERR_CHAR_PROMOTION_UPDATE_09, kPacket.m_nOK, 10 ); // 전직 수행레벨이 되지 않음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"전직 마법서 아이템 사용.. Name : " << m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_dwItemID )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOGc( kPacket_.m_cCharPromotion ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_CHAR_PROMOTION_UPDATE_ACK );
    }
}

IMPL_ON_FUNC( EVENT_CHAR_PROMOTION_UPDATE_ACK )
{
    KItemPtr pkItem;
    JIF( PacketHackCheck( nFrom_ ) );

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 해당 아이템을 보유 하고 있는가?
    pkItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_dwItemID );
    if( NULL == pkItem ) {
        SET_ERR_GOTO( ERR_CHAR_PROMOTION_UPDATE_00, END_PROC );
    }
    pkItem->m_nCount = std::max<int>( 0, pkItem->m_nCount - 1 ); // 해당 아이템 갯수 차감.

    kPacket_.m_vecCountItem.push_back( *pkItem );

    // 전직된 캐릭터에 대한 스킬정보 갱신.
    KCharacterInfo* pkCharacterInfo = GetCharacterInfo( kPacket_.m_cCharType );
    if( pkCharacterInfo ) {
        pkCharacterInfo->m_cPromotion = kPacket_.m_cPromotionLevel;
        SiKSkillManager()->GetSkillPointInfo( kPacket_.m_cCharType, kPacket_.m_cPromotionLevel, pkCharacterInfo->m_dwLevel, pkCharacterInfo->m_kSkillInfo.m_nLvSPPoint );
        kPacket_.m_kSkillInfo = pkCharacterInfo->m_kSkillInfo;
    }

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_CHAR_PROMOTION_UPDATE_00, kPacket_.m_nOK, 1 ); // 전직 마법서 아이템 가져오기 실패.
        ERR_CASE( ERR_CHAR_PROMOTION_UPDATE_01, kPacket_.m_nOK, 2 ); // 해당 전직 변경 불가 아이템.
        ERR_CASE( ERR_CHAR_PROMOTION_UPDATE_02, kPacket_.m_nOK, 3 ); // 캐릭터 미션정보가 없음.
        ERR_CASE( ERR_CHAR_PROMOTION_UPDATE_03, kPacket_.m_nOK, 4 ); // 캐릭터 전직변경 DB 처리실패.
        ERR_CASE( ERR_CHAR_PROMOTION_UPDATE_04, kPacket_.m_nOK, 5 ); // 캐릭터 정보 가져오기 실패.
        ERR_CASE( ERR_CHAR_PROMOTION_UPDATE_05, kPacket_.m_nOK, 6 ); // 해당 전직으로 변경 불가.
        ERR_CASE( ERR_CHAR_PROMOTION_UPDATE_06, kPacket_.m_nOK, 7 ); // 등록되지 않은 미션.
        ERR_CASE( ERR_CHAR_PROMOTION_UPDATE_07, kPacket_.m_nOK, 8 ); // 유저가 존재하지 않음.
        ERR_CASE( ERR_CHAR_PROMOTION_UPDATE_08, kPacket_.m_nOK, 9 ); // 미션이 등록되어 있지 않음.
        ERR_CASE( ERR_CHAR_PROMOTION_UPDATE_09, kPacket_.m_nOK, 10 ); // 전직 수행레벨이 되지 않음.
        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket_.m_nOK = 99;
    }

    LOG_CONDITION( kPacket_.m_nOK == 0 || kPacket_.m_nOK == -98, clog, cerr )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwItemID )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOGc( kPacket_.m_cPromotionLevel )
        << BUILD_LOG( kPacket_.m_dwMissionID )
        << BUILD_LOG( kPacket_.m_vecCountItem.size() )
        << BUILD_LOG( kPacket_.m_vecRewards.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CHAR_PROMOTION_UPDATE_ACK );
}

IMPL_ON_FUNC( EVENT_USER_HERO_DUNGEON_INFO_ACK )
{
    SEND_RECEIVED_PACKET( EVENT_USER_HERO_DUNGEON_INFO_ACK );
    m_mapHeroDungeonClearInfo.swap( kPacket_ );
    START_LOG( clog, L"Set User Hero Dungeon Info. size : " << m_mapHeroDungeonClearInfo.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_UPDATEPLAN_HERO_DUNGEON_INFO_NOT, DWORD )
{
    SendHeroDungeonList();

    START_LOG( clog, L"영웅던전 스크립트 변경시 접속중인 유저에게 데이터 전달: " << m_strName )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_HERO_ITEM_CATALOG_REQ )
{
    std::map<PAIR_USHORT_DWORD,KManufactureItem> mapItemList;
    SiKHeroDungeonManager()->m_kHeroPointItems.GetCatalogs( mapItemList );
    SEND_COMPRESS_PACKET( EVENT_HERO_ITEM_CATALOG_ACK, mapItemList );

    START_LOG( clog, L"영웅 아이템 리스트 전달. size : " << mapItemList.size() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_HERO_ITEM_MATERIAL_REQ )
{
    std::map<PAIR_USHORT_DWORD,std::vector<KDropItemInfo> > mapMaterialList;

    std::set<PAIR_USHORT_DWORD>::iterator sit;
    for ( sit = kPacket_.begin() ; sit != kPacket_.end() ; ++sit ) {
        std::vector<KDropItemInfo> vecMaterial;
        if ( false == SiKHeroDungeonManager()->m_kHeroPointItems.GetMaterials( *sit, vecMaterial ) ) {
            START_LOG( cerr, L"해당 아이템의 구매재료가 등록되어있지 않음. ItemID : " << (*sit).second ) << END_LOG;
            continue;
        }

        mapMaterialList[ *sit ].swap( vecMaterial );
    }

    SEND_COMPRESS_PACKET( EVENT_HERO_ITEM_MATERIAL_ACK, mapMaterialList );

    _LOG_SUCCESS( !mapMaterialList.empty(), L"영웅 아이템 구매재료 리스트 전달." )
        << BUILD_LOG( kPacket_.size() )
        << BUILD_LOG( mapMaterialList.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_BUY_FOR_HERO_REQ, PAIR_USHORT_PAIR_DWORD_INT )
{
    std::vector<KDropItemInfo> vecMaterials;
    std::vector<KDropItemInfo>::iterator vit;

    KDB_EVENT_BUY_FOR_HERO_REQ kDBPacket;
    kDBPacket.m_kCatalog.m_Index = kPacket_.first;
    kDBPacket.m_kCatalog.m_ItemID = kPacket_.second.first;

    KEVENT_BUY_FOR_HERO_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nGP = GetGP();

    SET_ERROR( ERR_UNKNOWN );

    // 구매재료 아이템 정보가 없음
    if ( false == SiKHeroDungeonManager()->m_kHeroPointItems.GetCatalog( kPacket_, kDBPacket.m_kCatalog ) ) {
        SET_ERR_GOTO( ERR_HERO_POINT_00, END_PROC );
    }

    // 코디아이템일 경우, 충분한 인벤 공간이 없음.
    bool bLookItem = kDBPacket.m_kCatalog.CheckItemFlag( KManufactureItem::IF_LOOK );
    if ( bLookItem ) {
        if ( m_kInventory.GetLookItemNum() >= GetTrueLookInvenCapacity() ) {
            SET_ERR_GOTO( ERR_HERO_POINT_06, END_PROC );
        }
    }

    if ( false == SiKHeroDungeonManager()->m_kHeroPointItems.GetMaterials( kPacket_, vecMaterials ) ) {
        SET_ERR_GOTO( ERR_HERO_POINT_00, END_PROC );
    }

    for ( vit = vecMaterials.begin() ; vit != vecMaterials.end() ; ++vit ) {
        // 구매재료 아이템이 없음.
        KItemPtr pkItem = m_kInventory.FindItemByItemID( vit->m_ItemID );
        if ( pkItem == NULL ) {
            SET_ERR_GOTO( ERR_HERO_POINT_01, END_PROC );
        }

        if ( vit->m_nDuration == KItem::UNLIMITED_ITEM ) {
            // 구매재료 영구아이템이 없음.
            if ( pkItem->m_nCount != KItem::UNLIMITED_ITEM ||
                pkItem->m_nInitCount != KItem::UNLIMITED_ITEM ) {
                    SET_ERR_GOTO( ERR_HERO_POINT_02, END_PROC );
            }

            kDBPacket.m_vecDelMaterials.push_back( pkItem->m_ItemUID );
        }
        else {
            // 구매재료 수량아이템의 수량이 부족함.
            if ( pkItem->m_nCount < vit->m_nDuration ) {
                SET_ERR_GOTO( ERR_HERO_POINT_03, END_PROC );
            }
            // 수량 아이템에 대해서만 넣자.
            kDBPacket.m_mapKItem[pkItem->m_ItemID] = *pkItem;
        }
    }

    kDBPacket.m_vecMaterials = vecMaterials;

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_BUY_FOR_HERO_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,            kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_HERO_POINT_00, kPacket.m_nOK, 1 ); // 구매재료 아이템 정보가 없음
        ERR_CASE( ERR_HERO_POINT_01, kPacket.m_nOK, 2 ); // 구매재료 아이템이 없음.
        ERR_CASE( ERR_HERO_POINT_02, kPacket.m_nOK, 3 ); // 구매재료 영구아이템이 없음.
        ERR_CASE( ERR_HERO_POINT_03, kPacket.m_nOK, 4 ); // 구매재료 수량아이템의 수량이 부족함.
        ERR_CASE( ERR_HERO_POINT_04, kPacket.m_nOK, 5 ); // DB작업중 아이템 넣기 실패.
        ERR_CASE( ERR_HERO_POINT_05, kPacket.m_nOK, 6 ); // DB작업중 룩 아이템 변환 실패.
        ERR_CASE( ERR_HERO_POINT_06, kPacket.m_nOK, 7 ); // 코디아이템 구매시 충분한 인벤 공간이 없음.
        ERR_CASE( ERR_HERO_POINT_07, kPacket.m_nOK, 8 ); // 일반 아이템 구매시 충분한 인벤 공간이 없음.

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
        << BUILD_LOG( kPacket_.second.first )
        << BUILD_LOG( kPacket_.second.second )
        << BUILD_LOG( vecMaterials.size() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nGP )
        << BUILD_LOG( kPacket.m_vecRewards.size() )
        << BUILD_LOG( kPacket.m_vecCountMaterialItems.size() )
        << BUILD_LOG( kPacket.m_vecPeriodMaterialItems.size() )
        << BUILD_LOG( kPacket.m_vecMaterials.size() )
        << BUILD_LOG( kPacket.m_mapKItem.size() ) << END_LOG;

    if ( false == IS_CORRECT( NET_OK ) ) {
        SEND_PACKET( EVENT_BUY_FOR_HERO_ACK );
    }
}

IMPL_ON_FUNC( EVENT_BUY_FOR_HERO_ACK )
{
    std::vector< GCITEMUID >::iterator vit;
    std::map< GCITEMID, KItem >::iterator mit;
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

        std::vector<KDropItemInfo>::iterator vitCount;
        for ( vitCount = kPacket_.m_vecMaterials.begin() ; vitCount != kPacket_.m_vecMaterials.end() ; ++vitCount ) {
            KItemPtr pkItem = m_kInventory.FindDurationItemByItemID( vitCount->m_ItemID );
            if ( !pkItem ) {
                START_LOG( cwarn, L"구매재료 수량아이템이 인벤에 없음. ItemID : " << vitCount->m_ItemID ) << END_LOG;
                continue;
            }
            mit = kPacket_.m_mapKItem.find( pkItem->m_ItemID );
            if( mit == kPacket_.m_mapKItem.end() ) {
                START_LOG( cwarn, L"DB에서 넘어온 수량아이템 리스트에 없음. ItemID : " << vitCount->m_ItemID ) << END_LOG;
                continue;
            }
            // 구매재료 수량 아이템 삭감
            pkItem->m_nCount = std::max<int>( 0, mit->second.m_nCount );
            pkItem->m_nInitCount = std::max<int>( 0, mit->second.m_nCount );
            kPacket_.m_vecCountMaterialItems.push_back( *pkItem );

            if ( pkItem->m_nCount <= 0 ) {
                m_kInventory.RemoveItem( pkItem->m_ItemID, pkItem->m_ItemUID );
            }
        }
    }

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,            kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_HERO_POINT_00, kPacket_.m_nOK, 1 ); // 구매재료 아이템 정보가 없음
        ERR_CASE( ERR_HERO_POINT_01, kPacket_.m_nOK, 2 ); // 구매재료 아이템이 없음.
        ERR_CASE( ERR_HERO_POINT_02, kPacket_.m_nOK, 3 ); // 구매재료 영구아이템이 없음.
        ERR_CASE( ERR_HERO_POINT_03, kPacket_.m_nOK, 4 ); // 구매재료 수량아이템의 수량이 부족함.
        ERR_CASE( ERR_HERO_POINT_04, kPacket_.m_nOK, 5 ); // DB작업중 아이템 넣기 실패.
        ERR_CASE( ERR_HERO_POINT_05, kPacket_.m_nOK, 6 ); // DB작업중 룩 아이템 변환 실패.
        ERR_CASE( ERR_HERO_POINT_06, kPacket_.m_nOK, 7 ); // 코디아이템 구매시 충분한 인벤 공간이 없음.
        ERR_CASE( ERR_HERO_POINT_07, kPacket_.m_nOK, 8 ); // 일반 아이템 구매시 충분한 인벤 공간이 없음.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 아직 처리중인 작업
        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nGP )
        << BUILD_LOG( kPacket_.m_prKey.first )
        << BUILD_LOG( kPacket_.m_prKey.second )
        << BUILD_LOG( kPacket_.m_vecRewards.size() )
        << BUILD_LOG( kPacket_.m_vecCountMaterialItems.size() )
        << BUILD_LOG( kPacket_.m_vecPeriodMaterialItems.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_BUY_FOR_HERO_ACK );
}

_IMPL_ON_FUNC( EVENT_GAWIBAWIBO_INFO_REQ, int )
{
    SET_ERROR( ERR_UNKNOWN );
    // 접속 후 최초 요청은 -1 을 보내주시오.
    KEVENT_GAWIBAWIBO_INFO_ACK kPacket;

    if ( !SiKGawibawibo()->IsRun() ) { // 이벤트 기간인지 체크
        SET_ERR_GOTO( ERR_GAWIBAWIBO_00, END_PROC );
    }

    // 버전 정보가 다르다. 새 정보를 보내주자.
    if ( kPacket_ != SiKGawibawibo()->GetVersion() ) {
        SiKGawibawibo()->GetRewardList( kPacket.m_mapWinReward );
        kPacket.m_dwRetryMark = SiKGawibawibo()->MarkRetryPosition(); // 재도전 표시값
        kPacket.m_dwPointTimeGap = SiKGawibawibo()->GetPointTimeGap(); // 포인트 지급 시간 간격
    }
    
    SiKGawibawibo()->GetNumberOfBoss( m_kGawibawiboInfo.m_dwTurnCount, m_kGawibawiboInfo.m_usNumBoss );

    kPacket.m_dwDefaultRewardStage = SiKGawibawibo()->GetDefaultRewardStage();
    // 유저의 가위바위보 정보
    kPacket.m_kUserGawibawiboInfo = m_kGawibawiboInfo;

    SET_ERROR( NET_OK );
END_PROC:
    switch ( NetError::GetLastNetError() ) 
    {
        ERR_CASE( NET_OK,   kPacket.m_nOK, 0 );          // 성공
        ERR_CASE( ERR_GAWIBAWIBO_00, kPacket.m_nOK, 1 ); // 가위바위보 이벤트 기간이 아님.
    default:
        break;
    }

    START_LOG( clog, L"가위바위보 정보 요청 : " << m_strName )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( kPacket.m_mapWinReward.size() )
        << BUILD_LOG( kPacket.m_kUserGawibawiboInfo.m_dwCurrentPosition )
        << BUILD_LOG( kPacket.m_kUserGawibawiboInfo.m_dwTurnCount )
        << BUILD_LOG( kPacket.m_kUserGawibawiboInfo.m_usReceiveCount )
        << BUILD_LOG( kPacket.m_kUserGawibawiboInfo.m_dwRemainTime )
        << BUILD_LOG( kPacket.m_kUserGawibawiboInfo.m_dwPlayCount )
        << BUILD_LOG( kPacket.m_kUserGawibawiboInfo.m_dwTryPoint )
        << BUILD_LOG( kPacket.m_kUserGawibawiboInfo.m_dwRetryPoint )
        << BUILD_LOG( kPacket.m_kUserGawibawiboInfo.m_usVersion )
        << BUILD_LOG( kPacket.m_kUserGawibawiboInfo.m_vecReward.size() ) << END_LOG;

    SEND_PACKET( EVENT_GAWIBAWIBO_INFO_ACK );
}

_IMPL_ON_FUNC( DB_EVENT_GAWIBAWIBO_INFO_ACK, KUserGawibawiboInfo ) // 접속시 디비 갔다옴
{

    // 유저의 가위바위보 정보를 디비로부터 받아서 유저 객체에 저장한다.
    // 클라이언트로부터 정보 요청이 오면 보내주는 것으로 한다.
    m_kGawibawiboInfo = kPacket_;

    START_LOG( clog, L"가위바위보 유저 데이터 DB에서 로드" )
        << BUILD_LOG( m_kGawibawiboInfo.m_dwTryPoint )
        << BUILD_LOG( m_kGawibawiboInfo.m_dwRetryPoint )
        << BUILD_LOG( m_kGawibawiboInfo.m_dwPlayCount )
        << BUILD_LOG( m_kGawibawiboInfo.m_dwCurrentPosition )
        << BUILD_LOG( m_kGawibawiboInfo.m_dwTurnCount )
        << BUILD_LOG( m_kGawibawiboInfo.m_dwRemainTime )
        << BUILD_LOG( m_kGawibawiboInfo.m_usReceiveCount ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_GAWIBAWIBO_REQ )
{
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    
    KEVENT_GAWIBAWIBO_ACK kPacket;

    SET_ERROR( ERR_UNKNOWN );

    if ( !SiKGawibawibo()->IsRun() ) { // 이벤트 기간인지 체크
        SET_ERR_GOTO( ERR_GAWIBAWIBO_00, END_PROC );
    }

    // 도전 포인트 있는지 확인
    if ( SiKGawibawibo()->CheckTryPoint( m_kGawibawiboInfo ) == false ) {
        SET_ERR_GOTO( ERR_GAWIBAWIBO_01, END_PROC );
    }

    // 확률 계산 -> 승/패/무승부를 서버에서 결정해서 클라이언트로 전달한다.
    USHORT usResult;
    usResult = SiKGawibawibo()->DoGawibawibo( m_kGawibawiboInfo.m_dwTurnCount, m_kGawibawiboInfo.m_dwCurrentPosition );

    if ( usResult == NetError::ERR_GAWIBAWIBO_02 ) {
        usResult = 0;
        SET_ERR_GOTO( ERR_GAWIBAWIBO_02, END_PROC );
    }

    kPacket.m_kUserGawibawiboInfo.m_usResult = usResult;
    m_kGawibawiboInfo.m_usResult = usResult;

    m_kGawibawiboInfo.m_vecReward.clear();

    switch ( usResult )
    {
    case KEVENT_GAWIBAWIBO_ACK::LOSE: // 졌다면 포지션 값에 특정값(128) 을 더해놓는다. 재도전할 수 있다는 표시를 하기 위해서이다.
        SetRetryPosition();
        AddGawibawiboPlayCount(); // 졌을 때 , 플레이횟수 증가
        START_LOG( clog, L"가위바위보 졌음 : " << m_strName ) << END_LOG;
        
        SiKGawibawibo()->GetLoseReward( m_kGawibawiboInfo.m_vecReward );
        AddGawibawiboRetryPoint();
        break;
    case KEVENT_GAWIBAWIBO_ACK::WIN:
        START_LOG( clog, L"가위바위보 이김 : " << m_strName ) << END_LOG;

        // 이겼을 때는  도전 포인트 차감.
        SiKGawibawibo()->UsePoint( KUserGawibawiboInfo::TRY_POINT, m_kGawibawiboInfo );

        // 보상 리스트 가져오기
        SiKGawibawibo()->GetBossKairoReward( m_kGawibawiboInfo.m_dwTurnCount, m_kGawibawiboInfo.m_dwCurrentPosition, m_kGawibawiboInfo.m_vecReward );

        // 말의 위치 이동, 승리
        if ( SiKGawibawibo()->CheckFinalBoss( m_kGawibawiboInfo.m_dwTurnCount, m_kGawibawiboInfo.m_dwCurrentPosition ) ) {
            SetInitGawibawiboPosition();
            AddGawibawiboTurnCount();
        }
        else {
            MoveGawibawiboCurrentPosition();
        }

        AddGawibawiboPlayCount(); // 이겼을 때, 플레이횟수 증가
        AddGawibawiboRetryPoint();
        break;

    case KEVENT_GAWIBAWIBO_ACK::DRAW:
        START_LOG( clog, L"가위바위보 비겼음 : " << m_strName ) << END_LOG;
        break;

    default:
        break;
    }

    // 위의 KGawibawibo  함수 사용 로직을 Gawibawibo 클래스 내에서 처리하면 이쪽 코드가 짧아질 것이다. // 20101009 20:04 woosh
    
    // 디비에 결과 저장
    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_GAWIBAWIBO_INFO_UPDATE, m_kGawibawiboInfo );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) 
    {
        ERR_CASE( NET_OK,   kPacket.m_nOK, 0 );          // 성공
        ERR_CASE( ERR_GAWIBAWIBO_00, kPacket.m_nOK, 1 ); // 가위바위보 이벤트 기간이 아님.
        ERR_CASE( ERR_GAWIBAWIBO_01, kPacket.m_nOK, 2 ); // 가위바위보 도전 혹은 재도전 포인트 부족.
        ERR_CASE( ERR_GAWIBAWIBO_02, kPacket.m_nOK, 3 ); // 가위바위보 스크립트에 없는 스테이지 보스.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );    // 아직 처리중인 작업입니다.

    default:
        START_LOG( cerr, L"가위바위보 알 수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_GAWIBAWIBO_ACK );
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_kGawibawiboInfo.m_dwTryPoint )
        << BUILD_LOG( m_kGawibawiboInfo.m_dwRetryPoint )
        << BUILD_LOG( m_kGawibawiboInfo.m_usResult )
        << BUILD_LOG( m_kGawibawiboInfo.m_vecReward.size() )
        << BUILD_LOG( m_kGawibawiboInfo.m_dwPlayCount )
        << BUILD_LOG( m_kGawibawiboInfo.m_dwCurrentPosition )
        << BUILD_LOG( m_kGawibawiboInfo.m_dwTurnCount )
        << BUILD_LOG( m_kGawibawiboInfo.m_dwRemainTime )
        << BUILD_LOG( m_kGawibawiboInfo.m_usReceiveCount ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_GAWIBAWIBO_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    // 이긴 경우에 스테이지, 보스에 따른 보상 아이템 지급.
    m_kInventory.AddItemList( kPacket_.m_vecItem );

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,            kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GAWIBAWIBO_03, kPacket_.m_nOK, 4 ); // 가위바위보 정보 DB 기록 실패.
        ERR_CASE( ERR_GAWIBAWIBO_04, kPacket_.m_nOK, 5 ); // 가위바위보 아이템 DB에 넣기 실패

    default:
        START_LOG( cerr, L"가위바위보 알 수 없는 오류 ..  Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"가위바위보 아이템 지급로그 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_GAWIBAWIBO_ACK );
}

_IMPL_ON_FUNC( EVENT_GAWIBAWIBO_GIVE_UP_REQ, int )
{ // 재도전 버튼:재도전 포인트 차감 , 확인 버튼:도전 포인트 차감

    KEVENT_GAWIBAWIBO_GIVE_UP_ACK kPacket;

    if ( m_kGawibawiboInfo.m_dwCurrentPosition < SiKGawibawibo()->MarkRetryPosition() ) {
        SET_ERR_GOTO( ERR_GAWIBAWIBO_07, END_PROC );
    }

    // 확인(포기)이면  도전 포인트 확인 및 차감, 말 위치 초기화( = 0 )
    if ( kPacket_ == KUserGawibawiboInfo::GIVE_UP ) { // 확인버튼(포기)

        SetInitGawibawiboPosition(); // 말 위치 초기화
        // 도전 포인트를 확인(포기) 에서만 차감한다. 조근용의 아이디어
        if ( !SiKGawibawibo()->UsePoint( KUserGawibawiboInfo::TRY_POINT, m_kGawibawiboInfo ) ) {
            SET_ERR_GOTO( ERR_GAWIBAWIBO_01, END_PROC );
        }
    }
    else if ( kPacket_ == KUserGawibawiboInfo::RETRY_POINT ) { // 재도전
        if ( m_kGawibawiboInfo.m_dwCurrentPosition == SiKGawibawibo()->MarkRetryPosition() ) { // 포지션 0 에서는 재도전 못함
            SET_ERR_GOTO( ERR_GAWIBAWIBO_07, END_PROC );
        }
    // 재도전이면   재도전 포인트 확인 및  차감,  그리고  재도전 위치 삭제 ( += -128 )    
        if ( !SiKGawibawibo()->UsePoint( KUserGawibawiboInfo::RETRY_POINT, m_kGawibawiboInfo ) ) {
            SET_ERR_GOTO( ERR_GAWIBAWIBO_01, END_PROC );
        }
    }
    
    // 재도전 가능 표시제거
    DeleteRetryPosition();

    SET_ERROR( NET_OK );

END_PROC:
    
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GAWIBAWIBO_01, kPacket.m_nOK, 2 ); // 가위바위보 도전 혹은 재도전 포인트 부족.
        ERR_CASE( ERR_GAWIBAWIBO_07, kPacket.m_nOK, 8 ); // 가위바위보 재도전 할 수 없는 상태
    default:
        START_LOG( cerr, L"가위바위보 알 수 없는 오류 ..  Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;        
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 || kPacket.m_nOK == 8, L"가위바위보 재도전 혹은 확인 버튼: " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( m_kGawibawiboInfo.m_dwCurrentPosition ) << END_LOG;

    
    kPacket.m_kUserGawibawiboInfo = m_kGawibawiboInfo;
    SEND_PACKET( EVENT_GAWIBAWIBO_GIVE_UP_ACK );
}

_IMPL_ON_FUNC( EVENT_GAWIBAWIBO_ITEM_TRADE_REQ, GCITEMID )
{
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_GAWIBAWIBO_ITEM_TRADE_ACK kPacket;
    KItemPtr pkItem;

    if ( !SiKGawibawibo()->IsRun() ) { // 이벤트 기간인지 체크
        SET_ERR_GOTO( ERR_GAWIBAWIBO_00, END_PROC );
    }

    // 해당 아이템이 존재 하는가?
    pkItem = m_kInventory.FindDurationItemByItemID( kPacket_ );
    if ( !pkItem ) {
        SET_ERR_GOTO( ERR_GAWIBAWIBO_05, END_PROC );
    }

    if( SiKGawibawibo()->TradeItem( kPacket_, pkItem->m_nCount, m_kGawibawiboInfo ) == false ) {
        START_LOG( cerr, L"해당 아이템의 교환 정보 없음 ..  Name : " << m_strName )
            << BUILD_LOG( kPacket_ ) << END_LOG;
        SET_ERR_GOTO( ERR_GAWIBAWIBO_06, END_PROC );
    }

    // 주사위 아이템 갯수를 0개로 세팅하고 해당 아이템 Vector를 클라이언트에게 전송.
    pkItem->m_nCount = 0; // 위에서 개수만큼 포인트 적용했기 때문에 0개로 세팅
    kPacket.m_vecUseItem.push_back( *pkItem );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_GAWIBAWIBO_00, kPacket.m_nOK, 1 ); // 가위바위보 이벤트 기간이 아님.
        ERR_CASE( ERR_GAWIBAWIBO_05, kPacket.m_nOK, 6 ); // 가위바위보 포인트 아이템이 인벤토리에 없음.
        ERR_CASE( ERR_GAWIBAWIBO_06, kPacket.m_nOK, 7 ); // 가위바위보 포인트 아이템 교환정보 없음.

        default: break;
    }

    _LOG_SUCCESS( IS_CORRECT( NET_OK ), L"가위바위보 포인트 아이템 교환 요청: " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    SEND_PACKET( EVENT_GAWIBAWIBO_ITEM_TRADE_ACK );

}

IMPL_ON_FUNC_NOPARAM( EVENT_GAWIBAWIBO_TRYPOINT_INFO_REQ )
{
    if ( !SiKGawibawibo()->IsRun() ) { // 이벤트 기간인지 체크
        return;
    }

    DWORD kPacket;
    kPacket = m_kGawibawiboInfo.m_dwTryPoint;

    START_LOG( clog, L"가위바위보 도전 포인트 요청 : " << m_strName )
        << BUILD_LOG( kPacket ) << END_LOG;

    SEND_PACKET( EVENT_GAWIBAWIBO_TRYPOINT_INFO_ACK );
}

_IMPL_ON_FUNC( EVENT_GAWIBAWIBO_INIT_INFO_REQ, DWORD )
{
    START_LOG( clog, L"가위바위보 스크립트 변경시 접속중인 유저 데이터 요청: " << m_strName )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    QUEUEING_ID_TO_DB( DB_EVENT_GAWIBAWIBO_INFO_REQ );
}

_IMPL_ON_FUNC( EVENT_SPECIAL_REWARD_REQ, KRewardInfoList )
{
    MAP_CHAR_JOB mapPromotionInfo;
    std::map<DWORD,int> mapResultGP;
    std::vector<USHORT> vecBoxType;
    std::pair<int,int> prModeKey;
    std::vector<KRewardInfo>::iterator vitUser;
    int nBoxID = 0;
    KEVENT_SPECIAL_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    // 패킷이 비어있다.
    if ( kPacket_.empty() ) {
        SET_ERR_GOTO( ERR_SPECIAL_REWARD_05, END_PROC );
    }

    // 룸정보 얻어오기
    if ( !m_pkRoom ) {
        SET_ERR_GOTO( ERR_SPECIAL_REWARD_00, END_PROC );
    }
    m_pkRoom->GetResultGP( mapResultGP );
    m_pkRoom->ClearResultGP();

    prModeKey = std::make_pair( m_pkRoom->m_iGameMode, m_pkRoom->m_nDifficulty );

    // 선택한 캐릭터 정보 설정.
    m_pkRoom->GetPromotionInfo( mapPromotionInfo );
    GetCurruntCharType( mapPromotionInfo, kPacket_ );
    kPacket.m_vecSpecialReward = kPacket_;

    if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
        //START_LOG( cwarn, L"--- Dump Get User Result GP (size:" << mapResultGP.size() << L")---"  ) << END_LOG;
        //std::map<DWORD,int>::const_iterator cmit;
        //for ( cmit = mapResultGP.begin() ; cmit != mapResultGP.end() ; ++cmit ) {
        //    START_LOG( cwarn, L"UserUID : " << cmit->first << L",ResultGP : " << cmit->second  ) << END_LOG;
        //}
        //START_LOG( cwarn, L"--- End Dump ---" << dbg::endl  ) << END_LOG;

        //START_LOG( cwarn, L"--- Dump User Select Index (size:" << kPacket_.size() << L")---"  ) << END_LOG;
        //std::vector<KRewardInfo>::const_iterator cvit;
        //for ( cvit = kPacket_.begin() ; cvit != kPacket_.end() ; ++cvit ) {
        //    std::Warn << L"UserUID : " << cvit->m_dwUID << L", CharType : " << (int)cvit->m_cCharType << L", Select size : " << cvit->m_setSelectIndex.size() << L"(";

        //    std::set<USHORT>::const_iterator csit;
        //    for ( csit = cvit->m_setSelectIndex.begin() ; csit != cvit->m_setSelectIndex.end() ; ++csit ) {
        //        std::Warn << *csit << L",";
        //    }
        //    START_LOG( cwarn, L")"  ) << END_LOG;
        //}
        //START_LOG( cwarn, L"--- End Dump ---" << dbg::endl  ) << END_LOG;
    }

    // 던전모드가 아닌데 보물상자 보상 요청함.
    if ( false == KRoom::IsDungeonMode( m_pkRoom->m_iGameMode ) ) {
        SET_ERR_GOTO( ERR_SPECIAL_REWARD_01, END_PROC );
    }

    // 던전난이도에 따른 보물상자 ID 얻어오기
    nBoxID = SiKSpecialReward()->GetDifficultBoxID( m_pkRoom->m_nDifficulty );
    if ( nBoxID == -1 ) {
        SET_ERR_GOTO( ERR_SPECIAL_REWARD_02, END_PROC );
    }

    // 보물상자 ID별 보물상자 Type 얻어오기
    if ( false == SiKSpecialReward()->GetBoxInfo( nBoxID, vecBoxType ) ) {
        SET_ERR_GOTO( ERR_SPECIAL_REWARD_03, END_PROC );
    }

    // 랜덤섞기
    std::random_shuffle( vecBoxType.begin(), vecBoxType.end() );

    // 유저별 획득 아이템 확인
    for ( vitUser = kPacket_.begin() ; vitUser != kPacket_.end() ; ++vitUser ) {

        // 해당 유저가 서버에 없으면 넘어가자.
        KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vitUser->m_dwUID ) );
        if ( !spUser ) {
            continue;
        }

        if ( m_pkRoom->GetFatigueRatio( vitUser->m_dwUID ) < 100 ) {
            continue;
        }

        // 박스 타입별
        std::vector<USHORT>::iterator vitBox;
        for ( vitBox = vecBoxType.begin() ; vitBox != vecBoxType.end() ; ++vitBox ) {

            // 캐릭터별 보상 아이템 체크
            int nCharType = ( *vitBox == KRewardInfo::RT_CHARACTER ? (int)vitUser->m_cCharType : -1 );
            std::pair<USHORT,int> prBoxKey = std::make_pair( *vitBox, nCharType );
            KRewardBox kDrop;
            kDrop.m_usBoxType = *vitBox;

            // 상자 보상일 경우,
            if ( SiKSpecialReward()->IsItemBox( prBoxKey.first ) ) {
                __LIF( SiKSpecialReward()->GetBoxDropItem( prModeKey, prBoxKey, kDrop.m_kRewardItem ) );

                // 보상 아이템이 있는 경우,
                if ( kDrop.m_kRewardItem.m_ItemID > 0 ) {
                    // 유저가 선택한 보상인가?
                    std::set<USHORT>::const_iterator csit = vitUser->m_setSelectIndex.find( *vitBox );
                    if ( csit != vitUser->m_setSelectIndex.end() ) {
                        if( spUser->GetTrueInvenCapacity() <= spUser->m_kInventory.GetTotalEnableSize() ) {
                            START_LOG( clog, L"최대 인벤사이즈를 초과해서 스페셜 아이템 실패..꽝처리. UserUID : " << vitUser->m_dwUID ) << END_LOG;
                            // 보상 초기화
                            *vitBox = 0;
                            kDrop.m_kRewardItem.m_ItemID = 0;
                            kDrop.m_kRewardItem.m_nDuration = -1;
                            kDrop.m_kRewardItem.m_nPeriod = -1;
                        }
                    }
                }
            }

            // GP 보상일 경우,
            kDrop.m_nRewardGP = 0;
            if ( SiKSpecialReward()->IsGPBox( prBoxKey.first ) ) {
                // 상자 GP처리
                float fGPRate = SiKSpecialReward()->GetBoxGPRate( *vitBox );
                if ( fGPRate > 0.f ) {
                    std::map<DWORD,int>::iterator mitGP = mapResultGP.find( vitUser->m_dwUID );
                    if ( mitGP == mapResultGP.end() ) {
                        START_LOG( cwarn, L"해당 유저의 결과GP 정보가 없음. UserUID : " << vitUser->m_dwUID ) << END_LOG;
                    }
                    else {
                        kDrop.m_nRewardGP = std::max<int>( 0, (int)(mitGP->second * fGPRate) );
                        START_LOG( clog, L"GP박스 설정됨. 보상GP : " << kDrop.m_nRewardGP ) << END_LOG;
                    }
                }
                else {
                    START_LOG( cwarn, L"GP박스인데, GP보너스률이 0 이하. BoxType : " << *vitBox << L", GP rate : " << fGPRate ) << END_LOG;
                }
            }

            vitUser->m_vecItemList.push_back( kDrop );
            vitUser->m_nDropGP = 0;

            if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
                START_LOG( cwarn, L"Special Reward Log" )
                    << BUILD_LOG( vitUser->m_dwUID )
                    << BUILD_LOG( m_pkRoom->m_iGameMode )
                    << BUILD_LOG( m_pkRoom->m_nDifficulty )
                    << BUILD_LOGc( vitUser->m_cCharType )
                    << BUILD_LOG( nCharType )
                    << BUILD_LOG( kDrop.m_usBoxType )
                    << BUILD_LOG( kDrop.m_kRewardItem.m_ItemID )
                    << BUILD_LOG( kDrop.m_kRewardItem.m_nDuration )
                    << BUILD_LOG( kDrop.m_kRewardItem.m_nPeriod )
                    << BUILD_LOG( kDrop.m_nRewardGP )
                    << BUILD_LOG( vitUser->m_nDropGP )
                    << END_LOG;
            }
        }
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_SPECIAL_REWARD_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_SPECIAL_REWARD_00, kPacket.m_nOK, 1 ); // 방 정보가 없음.
        ERR_CASE( ERR_SPECIAL_REWARD_01, kPacket.m_nOK, 2 ); // 던전모드가 아닌데 보물상자 보상 요청함.
        ERR_CASE( ERR_SPECIAL_REWARD_02, kPacket.m_nOK, 3 ); // 해당 난이도의 보물상자 정보가 없음
        ERR_CASE( ERR_SPECIAL_REWARD_03, kPacket.m_nOK, 4 ); // 해당 보물상자의 Type 정보가 없음.
        ERR_CASE( ERR_SPECIAL_REWARD_04, kPacket.m_nOK, 5 ); // DB처리 실패.
        ERR_CASE( ERR_SPECIAL_REWARD_05, kPacket.m_nOK, 6 ); // 보상요청 패킷이 비어있음

        ERR_CASE( ERR_NOT_FINISHED_YET,  kPacket.m_nOK, -98 ); // 아직 처리중인 작업
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.size() ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        // 실패시 방원들에게 빈 보상으로 알리자.
        if ( m_pkRoom ) {
            m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_SPECIAL_REWARD_BROAD, kPacket_, true );
        }
    }

}

IMPL_ON_FUNC( EVENT_SPECIAL_REWARD_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    // 유저별
    std::vector<KRewardInfo>::iterator vitUser;
    for ( vitUser = kPacket_.m_vecSpecialReward.begin() ; vitUser != kPacket_.m_vecSpecialReward.end() ; ++vitUser ) {

        KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vitUser->m_dwUID ) );
        if( !spUser ) {
            START_LOG( cwarn, L"보상을 받을 유저가 접속중이 아니다. UserUID : " << vitUser->m_dwUID ) << END_LOG;
            continue;
        }

        // 획득 아이템 넣기
        if ( !vitUser->m_vecRewardItem.empty() ) {
            spUser->m_kInventory.AddItemList( vitUser->m_vecRewardItem );
        }

        // 획득 GP 넣기
        spUser->IncreaseGP( KGpStatInfo::GIT_SPECIAL_REWARD, false, vitUser->m_nDropGP );
        vitUser->m_nCurrentGP = spUser->GetGP();

        START_LOG( clog, L"Special Reward.. UserUID : " << vitUser->m_dwUID )
            << BUILD_LOG( vitUser->m_vecRewardItem.size() )
            << BUILD_LOG( vitUser->m_nDropGP )
            << BUILD_LOG( vitUser->m_nCurrentGP ) << END_LOG;
    }

    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_SPECIAL_REWARD_00, kPacket_.m_nOK, 1 ); // 방 정보가 없음.
        ERR_CASE( ERR_SPECIAL_REWARD_01, kPacket_.m_nOK, 2 ); // 던전모드가 아닌데 보물상자 보상 요청함.
        ERR_CASE( ERR_SPECIAL_REWARD_02, kPacket_.m_nOK, 3 ); // 해당 난이도의 보물상자 정보가 없음
        ERR_CASE( ERR_SPECIAL_REWARD_03, kPacket_.m_nOK, 4 ); // 해당 보물상자의 Type 정보가 없음.
        ERR_CASE( ERR_SPECIAL_REWARD_04, kPacket_.m_nOK, 5 ); // DB처리 실패.
        ERR_CASE( ERR_SPECIAL_REWARD_05, kPacket_.m_nOK, 6 ); // 보상요청 패킷이 비어있음

        ERR_CASE( ERR_NOT_FINISHED_YET,  kPacket_.m_nOK, -98 ); // 아직 처리중인 작업
        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket_.m_nOK = 99;
    }

    _LOG_CONDITION( kPacket_.m_nOK == 0, clog, cwarn, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecSpecialReward.size() ) << END_LOG;

    if ( m_pkRoom ) {
        m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_SPECIAL_REWARD_BROAD, kPacket_.m_vecSpecialReward, true );
    }
}

_IMPL_ON_FUNC( EVENT_DUNGEON_REWARD_GP_NOT, std::set<DWORD> )
{
    // kPacket_ : 획득한 GP UID
    // 스테이지 이동시에 방장이 정보를 보낸다.
    _VERIFY_STATE( ( 1, KUserFSM::STATE_PLAYING ) );
    JIF( m_pkRoom );

    JIF( KRoom::IsDungeonMode( m_pkRoom->m_iGameMode ) );

    KDropData kDropInfo;
    m_pkRoom->GetDropList( kDropInfo );
    std::vector<KGameDropElement>& vecGpDrop = kDropInfo.m_vecGpDrop;

    std::vector<KEventMonster> vecEventMonInfo;
    std::vector<sTriggerInfo> vecTriggers;
    m_pkRoom->GetEventMonInfo( vecEventMonInfo );
    SiKGameDrop()->GetTriggerInfo( m_pkRoom->m_iGameMode, m_pkRoom->m_nDifficulty, vecTriggers );
    AddEventMonsterTrigger( vecEventMonInfo, vecTriggers );

    float fStageMonGP = 0.f; // 스테이지에서 획득한 전체 GP

    std::set<DWORD>::iterator sit;
    for ( sit = kPacket_.begin() ; sit != kPacket_.end() ; ++sit ) {

        std::vector<KGameDropElement>::iterator vitGP;
        vitGP = std::find_if( vecGpDrop.begin(), vecGpDrop.end(),
            boost::bind( &KGameDropElement::m_dwUID, _1 ) == *sit );
        if ( vitGP == vecGpDrop.end() ) {
            START_LOG( cwarn, L"GP Drop 정보에 없는 UID : " << *sit ) << END_LOG;
            continue;
        }

        std::vector<sTriggerInfo>::iterator vitMon;
        vitMon = std::find_if( vecTriggers.begin(), vecTriggers.end(),
            boost::bind( &sTriggerInfo::m_nMonsterID, _1 ) == vitGP->m_nMonID );
        if ( vitMon == vecTriggers.end() ) {
            START_LOG( cwarn, L"Trigger 정보에 없는 MonsterID : " << vitGP->m_nMonID ) << END_LOG;
            continue;
        }

        // 획득 Monster GP = 몬스터 레벨에 따른 보상GP * 먹은 동전 개수 * 동전등급에 따른 지급비율
        float fMonGP = SiKGameDrop()->GetMonGp( vitGP->m_nMonID, vitMon->m_nMonLv );
        float fCurruntMonGp = fMonGP * vitGP->m_fRatio;

        fCurruntMonGp = std::max<float>( 0.f, fCurruntMonGp );
        fStageMonGP += fCurruntMonGp;

        // 통계용
        m_pkRoom->AddMonDropGP( vitGP->m_nMonID, vitGP->m_nTriggerID, vitGP->m_dwItemID, static_cast<DWORD>(fCurruntMonGp) );
    }

    std::map<DWORD,int> mapResultGP; // UserUID, GP
    m_pkRoom->GetResultGP( mapResultGP );

    // 파티원 수로 나눈 비율로 GP 지급
    float fPartyRatio = static_cast<float>( 1.f / std::max<float>( 1.f, static_cast<float>( m_pkRoom->GetStartUserNum() ) ) );
    fStageMonGP *= fPartyRatio;

    int nDeductVitality = 0;
    SiKVitalityManager()->GetDeductVitality( m_pkRoom->m_iGameMode, m_pkRoom->m_nDifficulty, nDeductVitality );

    // 유저별로 획득한 GP 적용
    std::vector<DWORD> vecUserUIDs;
    m_pkRoom->GetUserUIDList( vecUserUIDs );
    std::vector<DWORD>::iterator vitUser;
    for ( vitUser = vecUserUIDs.begin() ; vitUser != vecUserUIDs.end() ; ++vitUser ) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( *vitUser );
        if ( !spUser ) continue;

        int nCurruntGP = static_cast<int>( fStageMonGP );

        if ( true == SiKFatigueSystem()->IsRun() && false == spUser->IsAdult() ) { // 중국 몰입방지 시스템. 20120904 luke
            int nRatio = m_pkRoom->GetFatigueRatio( spUser->GetUID() );
            nCurruntGP = nCurruntGP * nRatio / 100;
        }

        // 활력이 0인 상태로 활력을 소모하는 던전을 플레이한 경우에 획득 GP를 조정함
        int nRemainVitality = 0;
        if ( true == m_pkRoom->GetRemainVitalityOnStartGame( spUser->GetUID(), static_cast< int >( spUser->GetCurrentCharType() ), nRemainVitality ) ) {
            if ( nRemainVitality < nDeductVitality ) {
                int nGpGainRatioNoVitality = SiKVitalityManager()->GetGpGainRatioNoVitality( m_pkRoom->m_iGameMode, m_pkRoom->m_nDifficulty );
                nCurruntGP = nCurruntGP * nGpGainRatioNoVitality / 100;
            }
        }

        spUser->IncreaseGP( KGpStatInfo::GIT_DUNGEON_REWARD_GP, false, nCurruntGP );
        mapResultGP[*vitUser] += nCurruntGP;

        START_LOG( clog, L"유저 획득 GP 적용. UserUID : " << *vitUser )
            << BUILD_LOG( spUser->GetGP() )
            << BUILD_LOG( nCurruntGP )
            << BUILD_LOG( fStageMonGP )
            << BUILD_LOG( fPartyRatio )
            << BUILD_LOG( m_pkRoom->GetStartUserNum() )
            << END_LOG;
    }

    // 총 획득한 GP량 갱신
    m_pkRoom->SetDropList( kDropInfo );
    m_pkRoom->SetResultGP( mapResultGP );
}

IMPL_ON_FUNC( EVENT_DUNGEON_REWARD_ITEM_REQ )
{
    // kPacket_.m_prDropList : DropUID, UserUID (std::pair<DWORD,DWORD>)
    // kPacket_.m_prRouletteList : DropUID, UserUID List (std::pair<DWORD,std::vector<DWORD>>)
    // 아이템을 먹을때마다 방장이 보낸다.
    _VERIFY_STATE( ( 1, KUserFSM::STATE_PLAYING ) );

    KDB_EVENT_DUNGEON_REWARD_ITEM_REQ kDBPacket;
    kDBPacket.m_prRouletteList = kPacket_.m_prRouletteList;
    kDBPacket.m_kDubleCrystalCard.m_ItemID = 0;
    kDBPacket.m_prUserDrops.first = 0; // 실제 아이템 획득유저 UserUID
    kDBPacket.m_prUserDrops.second.m_ItemID = 0;
    kDBPacket.m_vecPrPartyDrops.clear();

    KEVENT_DUNGEON_REWARD_ITEM_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_vecDropItem.clear();

    std::vector<DWORD> vecUserUIDs;
    KDropData kDropInfo; // 드랍 정보
    std::map<DWORD, int> mapFatigueUserList; // 중국. 과몰입 대상 유저
    KUserPtr spUser;

    std::vector<KDropItemInfo> vecPartyDrop;
    vecPartyDrop.clear();

    if ( kPacket_.m_prDropList.second == 0 && kPacket_.m_prRouletteList.second.empty() ) {
        START_LOG( cwarn, L"아이템 획득 대상 UID정보가 비어있음. Login : " << GetName() ) << END_LOG;
        return;
    }

    SET_ERROR( ERR_UNKNOWN );

    // 방 정보가 없음.
    if ( !m_pkRoom ) {
        SET_ERR_GOTO( ERR_REAL_TIME_REWARD_00, END_PROC );
    }

    m_pkRoom->GetUserUIDList( vecUserUIDs );
    m_pkRoom->GetDropList( kDropInfo );

    if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
        DumpDungeonDropInfo( kDropInfo.m_vecMonsterDrop );
        DumpDungeonDropInfo( kDropInfo.m_vecMissionDrop );
    }

    m_pkRoom->GetFatigueUserList( mapFatigueUserList );

    // 일반(미션)드랍 처리하기
    __LIF( SiKGameDrop()->DistributeDrop( kDropInfo, vecUserUIDs, kPacket_.m_prDropList.second, kPacket_.m_prDropList.first, kDBPacket.m_prUserDrops, mapFatigueUserList ) );
    // 파티 전체 드랍 아이템 정보 얻기
    m_pkRoom->GetPartyRewardItem( vecPartyDrop );
    // 레어드랍 처리하기
    __LIF( SiKGameDrop()->DistributeRareDrop( kDropInfo, vecUserUIDs, kDBPacket.m_prRouletteList.second, kDBPacket.m_prRouletteList.first,
        kDBPacket.m_prUserDrops, vecPartyDrop, kDBPacket.m_vecPrPartyDrops, mapFatigueUserList ) );

    // 처리후 룸 드랍정보 갱신
    m_pkRoom->SetDropList( kDropInfo );

    // 드랍할 아이템이 없음.
    if ( kDBPacket.m_prUserDrops.second.m_ItemID <= 0 ) {
        SET_ERR_GOTO( ERR_REAL_TIME_REWARD_01, END_PROC );
    }

    // 더블 크리스탈 카드 처리
    if ( kDBPacket.m_prUserDrops.second.m_ItemID == CRYSTAL_GOODS_ID ) {
        KItemPtr pkItem;
        pkItem = m_kInventory.FindItemByItemID( DOUBLE_CRYSTAL_CARD_GOODS_ID );
        if ( pkItem == NULL ) {
            pkItem = m_kInventory.FindItemByItemID( DC30_DOUBLE_CRYSTAL_CARD_GOODS_ID );
        }

        // 획득 크리스탈 개수를 우선 2배 해주자.
        if ( pkItem != NULL && pkItem->m_nCount > 0 ) {
            kDBPacket.m_prUserDrops.second.m_nDuration *= 2;
            --pkItem->m_nCount; // 사용한 카드는 감소
            kDBPacket.m_kDubleCrystalCard = *pkItem;
        }
    }

    spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kDBPacket.m_prUserDrops.first );
    if( spUser ) {
        kDBPacket.m_cReceiveCharType = spUser->GetCurrentCharType();
    } else {
        kDBPacket.m_cReceiveCharType = -1;
    }

    // DB로 넣어주자
    QUEUEING_EVENT_TO_DB( EVENT_DUNGEON_REWARD_ITEM_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                  kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_REAL_TIME_REWARD_00, kPacket.m_nOK, 1 ); // 방 정보가 없음.
        ERR_CASE( ERR_REAL_TIME_REWARD_01, kPacket.m_nOK, 2 ); // 최대인벤 초과하거나 드랍할 아이템이 없음
        ERR_CASE( ERR_REAL_TIME_REWARD_02, kPacket.m_nOK, 3 ); // DB지급 실패.

        ERR_CASE( ERR_NOT_FINISHED_YET,    kPacket.m_nOK, -98 ); // 아직 처리중인 작업
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_CONDITION( kPacket.m_nOK == 0 || kPacket.m_nOK == -98, clog, cwarn, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_prDropList.first )
        << BUILD_LOG( kPacket_.m_prDropList.second )
        << BUILD_LOG( kPacket_.m_prRouletteList.first )
        << BUILD_LOG( kPacket_.m_prRouletteList.second.size() )
        << BUILD_LOG( kDBPacket.m_prUserDrops.first )
        << BUILD_LOG( kDBPacket.m_prUserDrops.second.m_ItemID )
        << BUILD_LOG( kDBPacket.m_kDubleCrystalCard.m_ItemID ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        if ( m_pkRoom ) {
            m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_DUNGEON_REWARD_ITEM_ACK, kPacket, true );
        }
    }
}

IMPL_ON_FUNC( EVENT_DUNGEON_REWARD_ITEM_ACK )
{
    // DB에서 아이템 넣고 온다.
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( false == IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    if ( m_pkRoom ) {
        KStatItemGetPtr spStat = GET_STAT( KStatItemGet, SI_STAT_ITEM_GET );
        if ( spStat ) {
            spStat->AddStat( m_pkRoom->m_iGameMode, kPacket_.m_vecDropItem );
        }
    }

    if ( !kPacket_.m_vecDropItem.empty() ) {

        KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwRecvUserUID ) );
        if( !spUser ) {
            START_LOG( cwarn, L"보상을 받을 유저가 접속중이 아니다. UserUID : " << kPacket_.m_dwRecvUserUID ) << END_LOG;
            goto END_PROC;
        }

        // 획득 아이템 넣기
        spUser->m_kInventory.AddItemList( kPacket_.m_vecDropItem );

        // 드랍받는 아이템중에 칭호 아이템이 있으면  전체 채팅 메세지로 보내기
        std::vector<KItem>::iterator vit;
        for ( vit = kPacket_.m_vecDropItem.begin() ; vit != kPacket_.m_vecDropItem.end() ; ++vit ) {

            if ( false == SiKCollectionManager()->IsTitleItem( vit->m_ItemID ) ) {
                continue;
            }

            if ( false == SiKCollectionManager()->GetBroadcastCheck() ) {
                continue;
            }

            // 닉네임  님께서   '던전이름' 의 '칭호 이름' 칭호를 획득하셨습니다. ['획득 방법']
            std::wstring strMsg;
            std::wstring strDungeon;// = L"어떤던전"; KAIRO , 펫 획득 메세지처럼 DB에 있음
            std::wstring strMethod;// = L"던전클리어보상"; config_server.ini에 설정
            strMethod = SiKCollectionManager()->HowGetTitle( L"str_01" ); // 어떻게 얻은 칭호인지, 이닛드랍
            SiKLoudMessage()->GetItemName( vit->m_ItemID, 1, strDungeon );
            if ( SiKLoudMessage()->GetLoudMsg( vit->m_ItemID, 0,  spUser->m_strNickName, strMsg, strDungeon, strMethod, GetLanguageCode() ) ) {
                ChatNotice( strMsg );
            }
        }

        // 올림픽 이벤트 보스게이트 드랍템 확인
        CheckOlympicBossGateDrop( spUser, kPacket_.m_vecDropItem );
    }

    if ( !kPacket_.m_vecPartyDropItem.empty() ) {
        std::vector< std::pair< DWORD, std::vector<KItem> > >::iterator vitPartyRewardInfo;
        for ( vitPartyRewardInfo = kPacket_.m_vecPartyDropItem.begin(); vitPartyRewardInfo != kPacket_.m_vecPartyDropItem.end(); vitPartyRewardInfo++ ) {
            KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vitPartyRewardInfo->first ) );
            if( !spUser ) {
                START_LOG( cwarn, L"보상을 받을 유저가 접속중이 아니다. UserUID : " << kPacket_.m_dwRecvUserUID ) << END_LOG;
                goto END_PROC;
            }

            // 획득 아이템 넣기
            spUser->m_kInventory.AddItemList( vitPartyRewardInfo->second );

            // 클라 디스플레이용 정보 세팅 (아랫 쪽에서 kPacket_.m_vecDropItem 사용하면 안됨)
//             std::vector<KItem>::iterator vitItem;
//             for ( vitItem = vitPartyRewardInfo->second.begin(); vitItem != vitPartyRewardInfo->second.end(); ++vitItem ) {
//                 kPacket_.m_vecDropItem.push_back( *vitItem );
//             }
        }
    }

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                  kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_REAL_TIME_REWARD_00, kPacket_.m_nOK, 1 ); // 방 정보가 없음.
        ERR_CASE( ERR_REAL_TIME_REWARD_01, kPacket_.m_nOK, 2 ); // 최대인벤 초과하거나 드랍할 아이템이 없음
        ERR_CASE( ERR_REAL_TIME_REWARD_02, kPacket_.m_nOK, 3 ); // DB지급 실패.

        ERR_CASE( ERR_NOT_FINISHED_YET,    kPacket_.m_nOK, -98 ); // 아직 처리중인 작업
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    _LOG_CONDITION( kPacket_.m_nOK == 0 || kPacket_.m_nOK == -98, clog, cwarn, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwRecvUserUID )
        << BUILD_LOG( kPacket_.m_vecDropItem.size() )
        << BUILD_LOG( kPacket_.m_kDubleCrystalCard.m_ItemID )
        << BUILD_LOG( kPacket_.m_kDubleCrystalCard.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kDubleCrystalCard.m_nCount ) << END_LOG;

    if ( m_pkRoom ) {
        m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_DUNGEON_REWARD_ITEM_ACK, kPacket_, true );
    }
}

_IMPL_ON_FUNC( EVENT_DUNGEON_REWARD_EXP_REQ, int )
{
    // kPacket_ : 죽인 몬스터 TriggerID
    // 몬스터를 죽일때마다 방장이 보낸다.
    _VERIFY_STATE( ( 1, KUserFSM::STATE_PLAYING ) );
    JIF( m_pkRoom );

    KGameInfo kGameInfo;
    m_pkRoom->GetGameInfo( kGameInfo );

    MAP_CHAR_JOB mapPromotionInfo;
    m_pkRoom->GetPromotionInfo( mapPromotionInfo );

    std::vector<DWORD> vecUserUIDs;
    m_pkRoom->GetUserUIDList( vecUserUIDs );

    std::vector<DWORD>::iterator vitUser;
    std::vector<sTriggerInfo>::iterator vitMon;

    std::vector<KEventMonster> vecEventMonInfo;
    std::vector<sTriggerInfo> vecTriggers;
    m_pkRoom->GetEventMonInfo( vecEventMonInfo );
    SiKGameDrop()->GetTriggerInfo( m_pkRoom->m_iGameMode, m_pkRoom->m_nDifficulty, vecTriggers );
    AddEventMonsterTrigger( vecEventMonInfo, vecTriggers );

    KEVENT_DUNGEON_REWARD_EXP_ACK kPacket;
    GetCharExpReward( vecUserUIDs, mapPromotionInfo, kPacket );

    // 룸유저 누적 Exp값. [UserUID,CharType], Exp
    std::map< std::pair<DWORD,char>, __int64 > mapResultExp;
    m_pkRoom->GetResultExp( mapResultExp );

    // 유저의 레벨업 정보 [UserUID, <CharType, Level>]
    std::map<DWORD, std::pair<int,DWORD>> mapLevelUpUser;
    mapLevelUpUser.clear();
    m_pkRoom->GetLevelUpUser( mapLevelUpUser );

    KDB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ kDBPacket;

    __int64 biMaxExp = SiKResultManager()->GetExpByLv( SiKResultManager()->GetMaxLevel() );

    bool bDungeonMonsterHackCheck = SiKPreHackingCheckManager()->CheckOption( KPreHackingCheckManager::GHT_DUNGEON_MONSTER_HACK, KPreHackingCheckManager::OPT_CHECK_ENABLE );
    if ( true == bDungeonMonsterHackCheck ) {
        if ( false == m_pkRoom->IncreaseMonsterKillCount() ) {
            START_LOG( cwarn, L"몬스터 최대 갯수보다 많이 죽이면 게임종료. MapID : " << kGameInfo.m_iGameMode ) << END_LOG;
            DungeonMonsterHackDisconnectCheck( kGameInfo.m_iGameMode );
            goto END_PROC;
        }
    }

    if ( false == KRoom::IsDungeonMode( kGameInfo.m_iGameMode ) ) {
        START_LOG( cwarn, L"던전 모드가 아닌데, 던전 경험치 처리 요청. ModeID : " << kGameInfo.m_iGameMode ) << END_LOG;
        goto END_PROC;
    }

    vitMon = std::find_if( vecTriggers.begin(), vecTriggers.end(),
        boost::bind( &sTriggerInfo::m_nTriggerID, _1 ) == kPacket_ );

    if ( vitMon == vecTriggers.end() ) {
        START_LOG( cwarn, L"해당 몬스터가 트리거 정보에 없음. TriggerID : " << kPacket_ )
            << BUILD_LOG( m_pkRoom->m_iGameMode )
            << BUILD_LOG( m_pkRoom->m_nDifficulty )
            << END_LOG;
        goto END_PROC;
    }

    // Stage별 몬스터 Kill정보 통계 추가.
    m_pkRoom->AddMonsterKillInfo( vitMon->m_nStage, vitMon->m_nMonsterID );

    float fMonExp = static_cast<float>( SiKGameDrop()->GetMonExp( vitMon->m_nMonsterID, vitMon->m_nMonLv ) );
    if ( fMonExp < 0.f ) {
        START_LOG( cwarn, L"해당 몬스터의 경험치가 0 미만. TriggerID : " << kPacket_ )
            << BUILD_LOG( m_pkRoom->m_iGameMode )
            << BUILD_LOG( m_pkRoom->m_nDifficulty )
            << END_LOG;
        goto END_PROC;
    }

    int nDeductVitality = 0;
    SiKVitalityManager()->GetDeductVitality( m_pkRoom->m_iGameMode, m_pkRoom->m_nDifficulty, nDeductVitality );

    for ( vitUser = vecUserUIDs.begin() ; vitUser != vecUserUIDs.end() ; ++vitUser ) {

        KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( *vitUser ) );
        if( !spUser ) {
            START_LOG( cwarn, L"서버에 유저 정보가 없음. UserUID : " << *vitUser ) << END_LOG;
            continue;
        }

        std::map<DWORD, std::vector<std::pair<char,char> > >::const_iterator cmitChar;
        cmitChar = mapPromotionInfo.find( *vitUser );
        if ( cmitChar == mapPromotionInfo.end() ) {
            START_LOG( cwarn, L"룸에 유저 정보가 없음. LoginID : " << spUser->GetName() ) << END_LOG;
            continue;
        }

        if ( cmitChar->second.empty() ) {
            START_LOG( cwarn, L"룸에 캐릭터 정보가 없음. LoginID : " << spUser->GetName() ) << END_LOG;
            continue;
        }

        char cCharType = cmitChar->second.begin()->first; // 유저의 캐릭터 타입
        KCharacterInfo* pkCharInfo = spUser->GetCharacterInfo( cCharType );
        if ( !pkCharInfo ) {
            START_LOG( cwarn, L"유저 캐릭터 정보가 없음. LoginID : " << spUser->GetName() ) << END_LOG;
            continue;
        }
        DWORD dwOldLevel = pkCharInfo->m_dwLevel;
        float fCurrentMonExp = fMonExp;

        if ( false == SiKHeroDungeonManager()->IsHeroDungeon( m_pkRoom->m_iGameMode ) ) {
            // 일반던전 레벨차 보정 (영던은 레벨차 보정 없음)
            SiKResultManager()->GetApplyLvDiffRatio( pkCharInfo->m_dwLevel, vitMon->m_nMonLv, fCurrentMonExp );
        }

        // 중국. 2012903. woosh.
        // 게임 시작시 저장된 누적온라인 시간을 구해서 과몰입 시스템에 있는 적용 결과에 따라 경험치 조정.
        if ( true == SiKFatigueSystem()->IsRun() && false == spUser->IsAdult() ) {
            int nRatio = m_pkRoom->GetFatigueRatio( spUser->GetUID() );
            fCurrentMonExp = fCurrentMonExp * nRatio / 100.f;
        }

        // 활력이 0인 상태로 활력을 소모하는 던전을 플레이한 경우에 획득 경험치를 조정함
        int nRemainVitality = 0;
        if ( true == m_pkRoom->GetRemainVitalityOnStartGame( spUser->GetUID(), static_cast< int >( spUser->GetCurrentCharType() ), nRemainVitality ) ) {
            if ( nRemainVitality < nDeductVitality ) {
                int nExpGainRatioNoVitality = SiKVitalityManager()->GetExpGainRatioNoVitality( m_pkRoom->m_iGameMode, m_pkRoom->m_nDifficulty );
                fCurrentMonExp = fCurrentMonExp * nExpGainRatioNoVitality / 100.f;
            }
        }

        fCurrentMonExp = std::max<float>( 0.f, fCurrentMonExp );

        pkCharInfo->m_biExp += static_cast<__int64>( fCurrentMonExp );
        pkCharInfo->m_biTotalExp += static_cast<__int64>( fCurrentMonExp );
        mapResultExp[ std::make_pair( *vitUser, cCharType ) ] += static_cast<__int64>( fCurrentMonExp );

        if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
            START_LOG( cerr , L"Exp Reward : " << fCurrentMonExp
                << L",ModeID : " << m_pkRoom->m_iGameMode
                << L",Difficulty : " << m_pkRoom->m_nDifficulty
                << L",TriggerID : " << kPacket_
                << L",MonsterID : " << vitMon->m_nMonsterID
                << L",MonsterLv : " << vitMon->m_nMonLv ) << END_LOG;
        }

        // 캐릭터에 적용.
        // EXP
        pkCharInfo->m_biExp = std::min<__int64>( pkCharInfo->m_biExp, biMaxExp );
        pkCharInfo->m_biExp = std::max<__int64>( pkCharInfo->m_biExp, USER_MIN_EXP );
        // TotalExp( 누적 경험치 )
        pkCharInfo->m_biTotalExp = std::min<__int64>( pkCharInfo->m_biTotalExp, _I64_MAX );
        pkCharInfo->m_biTotalExp = std::max<__int64>( pkCharInfo->m_biTotalExp, USER_MIN_EXP );
        // 레벨 계산.
        pkCharInfo->m_dwLevel = SiKResultManager()->Exp_2_Level( pkCharInfo->m_biExp );

        // 캐릭터의 Levelup을 달성했을 경우에 체크하자.
        if( dwOldLevel < pkCharInfo->m_dwLevel ) {
            // SPX 계산
            SiKSkillManager()->GetSkillPointInfo( cCharType, pkCharInfo->m_cPromotion, pkCharInfo->m_dwLevel, pkCharInfo->m_kSkillInfo.m_nLvSPPoint );
            // 상하이 드랍 체크
            LevelUpDrop( spUser, cCharType, static_cast<char>(dwOldLevel), static_cast<char>(pkCharInfo->m_dwLevel) );
            // 유저 레벨업 정보 저장.
            SetUserLevelUpInfo( spUser->GetUID(), (int)cCharType, pkCharInfo->m_dwLevel, mapLevelUpUser );
            // 통계용
            spUser->UpdateCharLvPlayTime( static_cast<int>(cCharType), dwOldLevel );

            if( SiKGCHelper()->IsCharLevelEvent() ) { // 레벨업을 했을 경우에..,
                if( SiKGCHelper()->GetCharLevelRewardItem( pkCharInfo->m_cCharType, pkCharInfo->m_dwLevel, m_setCharLevelEvent, kDBPacket.m_vecRewardInfo ) ) {
                    kDBPacket.m_setCharInfo.insert( pkCharInfo->m_cCharType );
                    _QUEUEING_EVENT_TO_DB( DB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ, kDBPacket, spUser );
                }
            }
        }

        // 클라이언트 전달용 패킷
        KCharExpReward kExpInfo;
        kExpInfo.m_cCharType = cCharType;
        kExpInfo.m_biExp = pkCharInfo->m_biExp;
        kExpInfo.m_biTotalExp = pkCharInfo->m_biTotalExp;
        kExpInfo.m_dwOldLevel = dwOldLevel;
        kExpInfo.m_dwLevel = pkCharInfo->m_dwLevel;
        kExpInfo.m_fRewardExp = fCurrentMonExp;
        kExpInfo.m_kSkillInfo = pkCharInfo->m_kSkillInfo;
        kPacket[ *vitUser ] = kExpInfo;

        // 각 유저별 미션 조건 체크 (몬스터ID,Lv 킬)
        SiKMissionManager()->MonsterKillSubMission( *spUser, cCharType, m_pkRoom->GetStartUserNum(), m_pkRoom->m_iGameMode, m_pkRoom->m_nDifficulty, vitMon->m_nMonsterID, vitMon->m_nMonLv );

        // 각 유저별 모험 이벤트 미션 체크
        CheckAdventureCollectingMission( spUser, kPacket_ );
    }

    // Exp 처리결과 룸에 갱신
    m_pkRoom->SetResultExp( mapResultExp );

    // 유저의 레벨업 정보 갱신.
    m_pkRoom->SetLevelUpUser( mapLevelUpUser );

END_PROC:
    if ( m_pkRoom ) {
        m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_DUNGEON_REWARD_EXP_ACK, kPacket, true );
    }

    START_LOG( clog, L"던전 exp 보상 처리. size : " << kPacket.size() )
        << BUILD_LOG( kPacket_ ) << END_LOG;
}


IMPL_ON_FUNC( EVENT_PVP_REWARD_EXP_GP_REQ )
{
    _VERIFY_STATE( ( 2, KUserFSM::STATE_PLAYING, KUserFSM::STATE_ROOM ) );
    JIF( m_pkRoom );

    MAP_CHAR_JOB mapPromotionInfo;
    m_pkRoom->GetPromotionInfo( mapPromotionInfo );

    std::vector<DWORD> vecUserUIDs;
    m_pkRoom->GetUserUIDList( vecUserUIDs );

    std::vector<std::pair<DWORD,int> > vecUIDTeam;
    m_pkRoom->GetTeamInfo( vecUIDTeam );

    if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
        //START_LOG( cwarn, L"--- Dump PvP Room Info ---" 
        //    << L"Category : " << (int)kGameInfo.m_cGameCategory << dbg::endl
        //    << L"GameMode : " << kGameInfo.m_iGameMode << dbg::endl
        //    << L"SubGameMode : " << kGameInfo.m_iSubGameMode << dbg::endl
        //    << L"kGameInfo.m_vecUserUIDs.size() : " << kGameInfo.m_vecUserUIDs.size() )
        //    << END_LOG;
        //std::Warn << L" [";
        //std::vector<DWORD>::const_iterator cvit;
        //for ( cvit = kGameInfo.m_vecUserUIDs.begin() ; cvit != kGameInfo.m_vecUserUIDs.end() ; ++cvit ) {
        //    std::Warn << *cvit << L",";
        //}
        //START_LOG( cwarn, L"]"  ) << END_LOG;

        //std::Warn << L"kGameInfo.m_vecUIDTeam.size() : " << kGameInfo.m_vecUIDTeam.size();
        //std::Warn << L" [";
        //std::vector<std::pair<DWORD,int> >::const_iterator cvitTeam;
        //for ( cvitTeam = kGameInfo.m_vecUIDTeam.begin() ; cvitTeam != kGameInfo.m_vecUIDTeam.end() ; ++cvitTeam ) {
        //    std::Warn << cvitTeam->first << L"(" << cvitTeam->second << L"),";
        //}
        //START_LOG( cwarn, L"]"  ) << END_LOG;
        //START_LOG( cwarn, L"--- End Dump ---"  ) << END_LOG;
    }

    KUserPtr spKillUser;
    KUserPtr spDeathUser;
    std::vector<std::pair<DWORD,int> >::const_iterator cvitKill;
    std::vector<std::pair<DWORD,int> >::const_iterator cvit;

    KEVENT_PVP_REWARD_EXP_GP_ACK kPacket;
    GetCharExpReward( vecUserUIDs, mapPromotionInfo, kPacket );

    // 룸유저 누적 Exp값. [UserUID,CharType], Exp
    std::map< std::pair<DWORD,char>, __int64 > mapResultExp;
    m_pkRoom->GetResultExp( mapResultExp );

    // 룸유저 누적 GP 값
    std::map<DWORD,int> mapResultGP;
    m_pkRoom->GetResultGP( mapResultGP );

    // 유저의 레벨업 정보 [UserUID, <CharType, Level>]
    std::map<DWORD, std::pair<int,DWORD>> mapLevelUpUser;
    mapLevelUpUser.clear();
    m_pkRoom->GetLevelUpUser( mapLevelUpUser );

    int nTeamCount = 0;
    __int64 biMaxExp = SiKResultManager()->GetExpByLv( SiKResultManager()->GetMaxLevel() );
    bool bPvpHackCheck = SiKPreHackingCheckManager()->CheckOption( KPreHackingCheckManager::GHT_PVP_CHECK, KPreHackingCheckManager::OPT_CHECK_ENABLE );

    if ( bPvpHackCheck ) {
        if ( false == m_pkRoom->DecreasePvpLife( kPacket_.m_prDeathUserInfo.first ) ) {
            START_LOG( cwarn, L"최대 목숨수보다 많이 죽으면 경험치 없음. MapID : " << m_pkRoom->m_iMapID ) << END_LOG;
            PvpHackDisconnectCheck( m_pkRoom->m_cGameCategory );
            goto END_PROC;
        }
    }

    if ( m_pkRoom->m_iGameMode == GC_GM_ANGELS_EGG ) {
        START_LOG( clog, L"천사의 알은 실시간 경험치 없음. ModeID : " << m_pkRoom->m_iGameMode ) << END_LOG;
        goto END_PROC;
    }

    if ( false == CheckCharacterType( kPacket_.m_prKillUserInfo.first, mapPromotionInfo, kPacket_.m_prKillUserInfo.second ) ||
         false == CheckCharacterType( kPacket_.m_prDeathUserInfo.first, mapPromotionInfo, kPacket_.m_prDeathUserInfo.second ) ) {
        START_LOG( cwarn, L"해당 캐릭터 정보가 없음." )
            << BUILD_LOG( kPacket_.m_prKillUserInfo.first )
            << BUILD_LOGc( kPacket_.m_prKillUserInfo.second )
            << BUILD_LOG( kPacket_.m_prDeathUserInfo.first )
            << BUILD_LOGc( kPacket_.m_prDeathUserInfo.second ) << END_LOG;
        goto END_PROC;
    }

    spKillUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_prKillUserInfo.first );
    spDeathUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_prDeathUserInfo.first );
    if ( !spKillUser || !spDeathUser ) {
        START_LOG( cwarn, L"대상이 접속중이 아니다." )
            << BUILD_LOG( kPacket_.m_prKillUserInfo.first )
            << BUILD_LOG( kPacket_.m_prDeathUserInfo.first ) << END_LOG;
        goto END_PROC;
    }

    KCharacterInfo* pkDeathCharInfo = spDeathUser->GetCharacterInfo( kPacket_.m_prDeathUserInfo.second );
    if ( !pkDeathCharInfo ) {
        START_LOG( cwarn, L"캐릭터 정보 가져오기 실패." )
            << BUILD_LOGc( kPacket_.m_prDeathUserInfo.second ) << END_LOG;
        goto END_PROC;
    }

    // 경험치 계산( 같은 팀원에게도 동일하게 적용되어야 한다. )
    // 캐릭터Lv에 대한 경험치 * 경험치 가중치( BaseEXPRatio )

    // 레벨값 보정공식 
    // GetMatchApplyLvDiffRatio( IN const DWORD dwWinCharLevel, IN const DWORD dwLoseCharLevel, OUT float& fExp );
    // dwWinCharLevel : 승리한 캐릭터 Lv
    // dwLoseCharLevel : 죽은 캐릭터 Lv
    // fExp : 보정전의 경험치.
    cvitKill = std::find_if( vecUIDTeam.begin(), vecUIDTeam.end(),
                             boost::bind( &std::pair<DWORD,int>::first, _1) == kPacket_.m_prKillUserInfo.first );

    if( cvitKill == vecUIDTeam.end() ) {
        START_LOG( cwarn, L"전체 팀 정보에 죽인 유저의 정보가 없다. Kill UserUID : " << kPacket_.m_prKillUserInfo.first << L",size : " << vecUIDTeam.size() ) << END_LOG;
        goto END_PROC;
    }

    // 팀원 수 체크
    for ( cvit = vecUIDTeam.begin(); cvit != vecUIDTeam.end(); ++cvit ) {
        if ( cvitKill->second != cvit->second ) continue;

        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( cvit->first );
        if( !spUser ) continue;

        ++nTeamCount;
    }

    for( cvit = vecUIDTeam.begin(); cvit != vecUIDTeam.end(); ++cvit ) {
        if( cvitKill->second == cvit->second ) {
            KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( cvit->first );
            if( !spUser ) {
                continue;
            }

            std::map<DWORD, std::vector<std::pair<char,char> > >::const_iterator cmitChar;
            cmitChar = mapPromotionInfo.find( cvit->first );
            if ( cmitChar == mapPromotionInfo.end() ) {
                continue;
            }
            
            if ( cmitChar->second.empty() ) {
                continue;
            }

            char cCharType = cmitChar->second.begin()->first; // 유저의 캐릭터 타입
            KCharacterInfo* pkCharInfo = spUser->GetCharacterInfo( cCharType );
            if ( !pkCharInfo ) {
                continue;
            }
            DWORD dwOldLevel = pkCharInfo->m_dwLevel;

            // Exp 계산
            float fExp = 0.f;
            SiKResultManager()->CalcPvpExp( pkCharInfo->m_dwLevel, pkDeathCharInfo->m_dwLevel, fExp );

            //중국 몰입방지 적용
            if ( SiKFatigueSystem()->IsRun() && !spUser->IsAdult() ) {
                int nRatio = m_pkRoom->GetFatigueRatio( spUser->GetUID() );
                fExp = fExp * nRatio / 100.f;
            }
            fExp = std::max<float>( 0.f, fExp );

            __int64 biExpReward = static_cast<__int64>(fExp);
            __int64 biExpRewardSum = mapResultExp[ std::make_pair( cvit->first, cCharType ) ];

            if ( bPvpHackCheck ) {
                if ( false == PvpHackCheck( cCharType, pkCharInfo->m_dwLevel, biExpRewardSum, biExpReward ) ) {
                    START_LOG( cwarn, L"더이상 실시간 대전 경험치를 획득할 수 없음. Login : " << spUser->GetName() ) << END_LOG;
                    PvpHackDisconnectCheck( m_pkRoom->m_cGameCategory );
                    continue;
                }
            }

            pkCharInfo->m_biExp += biExpReward;
            pkCharInfo->m_biTotalExp += biExpReward;
            mapResultExp[ std::make_pair( cvit->first, cCharType ) ] += biExpReward;

            // 캐릭터에 적용.
            // EXP
            pkCharInfo->m_biExp = std::min<__int64>( pkCharInfo->m_biExp, biMaxExp );
            pkCharInfo->m_biExp = std::max<__int64>( pkCharInfo->m_biExp, USER_MIN_EXP );
            // TotalExp( 누적 경험치 )
            pkCharInfo->m_biTotalExp = std::min<__int64>( pkCharInfo->m_biTotalExp, _I64_MAX );
            pkCharInfo->m_biTotalExp = std::max<__int64>( pkCharInfo->m_biTotalExp, USER_MIN_EXP );
            // 레벨 계산.
            pkCharInfo->m_dwLevel = SiKResultManager()->Exp_2_Level( pkCharInfo->m_biExp );

            // 캐릭터의 Levelup을 달성했을 경우에 체크하자.
            if( dwOldLevel < pkCharInfo->m_dwLevel ) {
                // SPX 계산
                SiKSkillManager()->GetSkillPointInfo( pkCharInfo->m_cCharType, pkCharInfo->m_cPromotion, pkCharInfo->m_dwLevel, pkCharInfo->m_kSkillInfo.m_nLvSPPoint );
                // 상하이 드랍 체크
                LevelUpDrop( spUser, cCharType, static_cast<char>(dwOldLevel), static_cast<char>(pkCharInfo->m_dwLevel) );
                // 유저 레벨업 정보 저장.
                SetUserLevelUpInfo( spUser->GetUID(), (int)cCharType, pkCharInfo->m_dwLevel, mapLevelUpUser );
                // 통계용
                spUser->UpdateCharLvPlayTime( static_cast<int>(cCharType), dwOldLevel );

                if( SiKGCHelper()->IsCharLevelEvent() ) { // 레벨업을 했을 경우에..,
                    KDB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ kDBPacket;
                    if( SiKGCHelper()->GetCharLevelRewardItem( pkCharInfo->m_cCharType, pkCharInfo->m_dwLevel, m_setCharLevelEvent, kDBPacket.m_vecRewardInfo ) ) {
                        kDBPacket.m_setCharInfo.insert( pkCharInfo->m_cCharType );
                        _QUEUEING_EVENT_TO_DB( DB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ, kDBPacket, spUser );
                    }
                }
            }

            // GP 계산
            // 획득GP = ( 몬스터GP테이블[ 죽인 캐릭터 Lv ] * 1.0(무조건 상수) / 팀원수 )
            float fGP = 0.f;
            SiKResultManager()->CalcPvpGP( pkDeathCharInfo->m_dwLevel, nTeamCount, fGP );

            //몰입방지.
            if ( SiKFatigueSystem()->IsRun() && !spUser->IsAdult() ) {
                int nRatio = m_pkRoom->GetFatigueRatio( spUser->GetUID() );
                fGP = fGP * nRatio / 100.f;
            }
            int nCurruntGP = static_cast<int>( fGP );
            spUser->IncreaseGP( KGpStatInfo::GIT_PVP_REWARD_GP, false, nCurruntGP );

            mapResultGP[ cvit->first ] += static_cast<int>( fGP );

            // 클라이언트 전달용 패킷
            KCharExpReward kExpInfo;
            kExpInfo.m_cCharType = cCharType;
            kExpInfo.m_biExp = pkCharInfo->m_biExp;
            kExpInfo.m_biTotalExp = pkCharInfo->m_biTotalExp;
            kExpInfo.m_dwOldLevel = dwOldLevel;
            kExpInfo.m_dwLevel = pkCharInfo->m_dwLevel;
            kExpInfo.m_fRewardExp = fExp;
            kExpInfo.m_kSkillInfo = pkCharInfo->m_kSkillInfo;
            kPacket[ cvit->first ] = kExpInfo;

            if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
                START_LOG( cwarn, L"PvP kill Exp / Gp Info. Login : " << spUser->GetName() )
                    << BUILD_LOG( spKillUser->GetName() )
                    << BUILD_LOG( spDeathUser->GetName() )
                    << BUILD_LOG( pkCharInfo->m_dwLevel )
                    << BUILD_LOG( pkDeathCharInfo->m_dwLevel )
                    << BUILD_LOG( fExp )
                    << BUILD_LOG( biExpReward )
                    << BUILD_LOG( nTeamCount )
                    << BUILD_LOG( fGP )
                    << END_LOG;
            }
        }
    }

    // Kill유저의 미션 정보 갱신(kill)
    SiKMissionManager()->PvpKillSubmission( *spKillUser, kPacket_.m_prKillUserInfo.second, m_pkRoom->GetStartUserNum(), m_pkRoom->m_iGameMode, m_pkRoom->m_iMapID );

    // Exp/GP 처리결과 룸에 갱신
    m_pkRoom->SetResultExp( mapResultExp );
    m_pkRoom->SetResultGP( mapResultGP );

    // 유저의 레벨업 정보 갱신.
    m_pkRoom->SetLevelUpUser( mapLevelUpUser );

END_PROC:
    if ( m_pkRoom ) {
        m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_PVP_REWARD_EXP_GP_ACK, kPacket, true );

        START_LOG( clog, L"대전 exp/gp 보상 처리. size : " << kPacket.size() )
            << BUILD_LOG( kPacket_.m_prKillUserInfo.first )
            << BUILD_LOGc( kPacket_.m_prKillUserInfo.second )
            << BUILD_LOG( kPacket_.m_prDeathUserInfo.first )
            << BUILD_LOGc( kPacket_.m_prDeathUserInfo.second )
            << BUILD_LOGc( m_pkRoom->m_cGameCategory )
            << BUILD_LOG( m_pkRoom->m_iGameMode )
            << BUILD_LOG( m_pkRoom->m_iMapID )
            << BUILD_LOG( bPvpHackCheck )
            << END_LOG;
    }
}

IMPL_ON_FUNC( EVENT_PVP_TAG_REWARD_EXP_GP_REQ )
{
    _VERIFY_STATE( ( 1, KUserFSM::STATE_PLAYING ) );
    JIF( m_pkRoom );

    MAP_CHAR_JOB mapPromotionInfo;
    m_pkRoom->GetPromotionInfo( mapPromotionInfo );

    std::vector<DWORD> vecUserUIDs;
    m_pkRoom->GetUserUIDList( vecUserUIDs );

    KUserPtr spDeathUser;
    KUserPtr spKillUser;
    std::vector<std::pair<DWORD,char> >::const_iterator cvitKillTeam;
    char cKillUserChar = -1;

    __int64 biMaxExp = SiKResultManager()->GetExpByLv( SiKResultManager()->GetMaxLevel() );
    bool bPvpHackCheck = SiKPreHackingCheckManager()->CheckOption( KPreHackingCheckManager::GHT_PVP_CHECK, KPreHackingCheckManager::OPT_CHECK_ENABLE );

    KEVENT_PVP_TAG_REWARD_EXP_GP_ACK kPacket;
    GetCharExpReward( vecUserUIDs, mapPromotionInfo, kPacket );

    // 룸유저 누적 Exp값. [UserUID,CharType], Exp
    std::map< std::pair<DWORD,char>, __int64 > mapResultExp;
    m_pkRoom->GetResultExp( mapResultExp );

    // 룸유저 누적 GP 값
    std::map<DWORD,int> mapResultGP;
    m_pkRoom->GetResultGP( mapResultGP );

    // 유저의 레벨업 정보 [UserUID, <CharType, Level>]
    std::map<DWORD, std::pair<int,DWORD>> mapLevelUpUser;
    mapLevelUpUser.clear();
    m_pkRoom->GetLevelUpUser( mapLevelUpUser );

    if ( false == CheckCharacterType( kPacket_.m_prDeathUserInfo.first, mapPromotionInfo, kPacket_.m_prDeathUserInfo.second ) ) {
        // 해당 캐릭터 정보가 없음.
        goto END_PROC;
    }

    spKillUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwKillUserUID );
    spDeathUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_prDeathUserInfo.first );
    if ( !spKillUser || !spDeathUser ) {
        START_LOG( cwarn, L"대상이 접속중이 아니다." )
            << BUILD_LOG( kPacket_.m_dwKillUserUID )
            << BUILD_LOG( kPacket_.m_prDeathUserInfo.first ) << END_LOG;
        goto END_PROC;
    }

    KCharacterInfo* pkDeathCharInfo = spDeathUser->GetCharacterInfo( kPacket_.m_prDeathUserInfo.second );
    if ( !pkDeathCharInfo ) {
        START_LOG( cwarn, L"캐릭터 정보 가져오기 실패." )
            << BUILD_LOGc( kPacket_.m_prDeathUserInfo.second ) << END_LOG;
        goto END_PROC;
    }

    // 경험치 계산( 같은 팀원에게도 동일하게 적용되어야 한다. )
    // 캐릭터Lv에 대한 경험치 * 경험치 가중치( BaseEXPRatio )

    // 레벨값 보정공식 
    // GetMatchApplyLvDiffRatio( IN const DWORD dwWinCharLevel, IN const DWORD dwLoseCharLevel, OUT float& fExp );
    // dwWinCharLevel : 승리한 캐릭터 Lv
    // dwLoseCharLevel : 죽은 캐릭터 Lv
    // fExp : 보정전의 경험치.
    for( cvitKillTeam = kPacket_.m_vecKillTeamInfo.begin(); cvitKillTeam != kPacket_.m_vecKillTeamInfo.end(); ++cvitKillTeam ) {

        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( cvitKillTeam->first );
        if( !spUser ) {
            continue;
        }

        std::map<DWORD, std::vector<std::pair<char,char> > >::const_iterator cmitChar;
        cmitChar = mapPromotionInfo.find( cvitKillTeam->first );
        if ( cmitChar == mapPromotionInfo.end() ) {
            continue;
        }

        if ( cmitChar->second.empty() ) {
            continue;
        }

        char cCharType = cvitKillTeam->second; // 유저의 캐릭터 타입
        KCharacterInfo* pkCharInfo = spUser->GetCharacterInfo( cCharType );
        if ( !pkCharInfo ) {
            continue;
        }
        DWORD dwOldLevel = pkCharInfo->m_dwLevel;

        if ( kPacket_.m_dwKillUserUID == cvitKillTeam->first ) {
            cKillUserChar = cCharType;
        }

        // Exp 계산
        float fExp = 0.f;
        SiKResultManager()->CalcPvpExp( pkCharInfo->m_dwLevel, pkDeathCharInfo->m_dwLevel, fExp );
        fExp = std::max<float>( 0.f, fExp );

        __int64 biExpReward = static_cast<__int64>(fExp);
        __int64 biExpRewardSum = mapResultExp[ std::make_pair( cvitKillTeam->first, cCharType ) ];

        if ( bPvpHackCheck ) {
            if ( false == PvpHackCheck( cCharType, pkCharInfo->m_dwLevel, biExpRewardSum, biExpReward ) ) {
                START_LOG( cwarn, L"더이상 실시간 태크매치 경험치를 획득할 수 없음. Login : " << spUser->GetName() ) << END_LOG;
                PvpHackDisconnectCheck( m_pkRoom->m_cGameCategory );
                continue;
            }
        }

        pkCharInfo->m_biExp += biExpReward;
        pkCharInfo->m_biTotalExp += biExpReward;
        mapResultExp[ std::make_pair(cvitKillTeam->first,cCharType) ] += biExpReward;

        // EXP 캐릭터에 적용.
        pkCharInfo->m_biExp = std::min<__int64>( pkCharInfo->m_biExp, biMaxExp );
        pkCharInfo->m_biExp = std::max<__int64>( pkCharInfo->m_biExp, USER_MIN_EXP );
        // TotalExp( 누적 경험치 )
        pkCharInfo->m_biTotalExp = std::min<__int64>( pkCharInfo->m_biTotalExp, _I64_MAX );
        pkCharInfo->m_biTotalExp = std::max<__int64>( pkCharInfo->m_biTotalExp, USER_MIN_EXP );
        // 레벨 계산.
        pkCharInfo->m_dwLevel = SiKResultManager()->Exp_2_Level( pkCharInfo->m_biExp );

        // 캐릭터의 Levelup을 달성했을 경우에 체크하자.
        if( dwOldLevel < pkCharInfo->m_dwLevel ) {
            // SPX 계산
            SiKSkillManager()->GetSkillPointInfo( pkCharInfo->m_cCharType, pkCharInfo->m_cPromotion, pkCharInfo->m_dwLevel, pkCharInfo->m_kSkillInfo.m_nLvSPPoint );
            // 상하이 드랍 체크
            LevelUpDrop( spUser, cCharType, static_cast<char>(dwOldLevel), static_cast<char>(pkCharInfo->m_dwLevel) );
            // 유저 레벨업 정보 저장.
            SetUserLevelUpInfo( spUser->GetUID(), (int)cCharType, pkCharInfo->m_dwLevel, mapLevelUpUser );
            // 통계용
            spUser->UpdateCharLvPlayTime( static_cast<int>(cCharType), dwOldLevel );

            if( SiKGCHelper()->IsCharLevelEvent() ) { // 레벨업을 했을 경우에..,
                KDB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ kDBPacket;
                if( SiKGCHelper()->GetCharLevelRewardItem( pkCharInfo->m_cCharType, pkCharInfo->m_dwLevel, m_setCharLevelEvent, kDBPacket.m_vecRewardInfo ) ) {
                    kDBPacket.m_setCharInfo.insert( pkCharInfo->m_cCharType );
                    _QUEUEING_EVENT_TO_DB( DB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ, kDBPacket, spUser );
                }
            }
        }

        // GP 계산
        // 획득GP = ( 몬스터GP테이블[ 죽인 캐릭터 Lv ] * 1.0(무조건 상수) / 팀원수 )
        float fGP = 0.f;
        SiKResultManager()->CalcPvpGP( pkDeathCharInfo->m_dwLevel, kPacket_.m_vecKillTeamInfo.size(), fGP );
        fGP = std::max<float>( 0.f, fGP );

        mapResultGP[ cvitKillTeam->first ] += static_cast<int>( fGP );

        // 클라이언트 전달용 패킷
        KCharExpReward kExpInfo;
        kExpInfo.m_cCharType = cCharType;
        kExpInfo.m_biExp = pkCharInfo->m_biExp;
        kExpInfo.m_biTotalExp = pkCharInfo->m_biTotalExp;
        kExpInfo.m_dwOldLevel = dwOldLevel;
        kExpInfo.m_dwLevel = pkCharInfo->m_dwLevel;
        kExpInfo.m_fRewardExp = fExp;
        kExpInfo.m_kSkillInfo = pkCharInfo->m_kSkillInfo;
        kPacket[ cvitKillTeam->first ] = kExpInfo;
    }

    // Kill유저의 미션 정보 갱신(kill)
    SiKMissionManager()->PvpKillSubmission( *spKillUser, cKillUserChar, m_pkRoom->GetStartUserNum(), m_pkRoom->m_iGameMode, m_pkRoom->m_iMapID );

    // 룸정보에 누적 Exp/GP 값 갱신
    m_pkRoom->SetResultExp( mapResultExp );
    m_pkRoom->SetResultGP( mapResultGP );

    // 유저의 레벨업 정보 갱신.
    m_pkRoom->SetLevelUpUser( mapLevelUpUser );

END_PROC:
    if ( m_pkRoom ) {
        m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_PVP_TAG_REWARD_EXP_GP_ACK, kPacket, true );
    }

    START_LOG( clog, L"태그매치 exp/gp 보상 처리. size : " << kPacket.size() )
        << BUILD_LOGc( kPacket_.m_vecKillTeamInfo.size() )
        << BUILD_LOG( kPacket_.m_prDeathUserInfo.first )
        << BUILD_LOGc( kPacket_.m_prDeathUserInfo.second ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_STAGE_LOAD_COMPLETE_NOT )
{
    _VERIFY_STATE(( 1,KUserFSM::STATE_PLAYING ));
    
    if ( GetStageLoadComplete() ) {
        return;
    }

    JIF( m_pkRoom );

    if ( m_pkRoom->FindStageLoadFailUser( GetUID() ) ) {
        return;
    }

    m_pkRoom->CheckFirstStageLoadComplete();

    SetStageLoadComplete( true );
    m_pkRoom->CheckStageLoadCompleteUser( GetName(), GetUID() );
}

IMPL_ON_FUNC( EVENT_INIT_ITEM_ATTRIBUTE_REQ )
{
    KEVENT_INIT_ITEM_ATTRIBUTE_ACK kPacket;
    kPacket.m_nOK = -99;
    KItemPtr pkUseItem;
    KItemPtr pkInvenItem;

    SET_ERROR( ERR_UNKNOWN );

    pkUseItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_ItemID );
    pkInvenItem = m_kInventory.FindItemByItemUID( kPacket_.m_ItemUID );
    if( !pkInvenItem || !pkUseItem )
    {
        SET_ERR_GOTO( ERR_ATTRIBUTE_INIT_01, END_PROC );
    }

    // 속성 초기화 아이템인지 체크.
    if( !SiKItemManager()->m_kAttributeDecider.CheckInitAttributeItem( pkInvenItem->m_cGradeID, kPacket_.m_ItemID ) ) {
        SET_ERR_GOTO( ERR_ATTRIBUTE_INIT_05, END_PROC );
    }

    // 코디 아이템인지 체크.
    if( pkInvenItem->CheckItemType(KItem::TYPE_LOOK) ) {
        SET_ERR_GOTO( ERR_ATTRIBUTE_INIT_03, END_PROC );
    }

    kPacket_.m_kItem = *pkInvenItem;
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_INIT_ITEM_ATTRIBUTE_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_ATTRIBUTE_INIT_01, kPacket.m_nOK, 1 );   // 속성을 초기화 할 아이템이 인벤에 없음.
        ERR_CASE( ERR_ATTRIBUTE_INIT_02, kPacket.m_nOK, 2 );   // 속성 초기화할 아이템이 에픽 등급 이상이 아님.
        ERR_CASE( ERR_ATTRIBUTE_INIT_03, kPacket.m_nOK, 3 );   // 속성 초기화할 아이템이 코디아이템임.
        ERR_CASE( ERR_ATTRIBUTE_INIT_04, kPacket.m_nOK, 4 );   // 속성초기화 DB에 기록 하는데 오류 발생.
        ERR_CASE( ERR_ATTRIBUTE_INIT_05, kPacket.m_nOK, 5 );   // 속성 초기화를 할수 있는 아이템이 아님.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99; // unknown error
        break;
    }

    if ( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_INIT_ITEM_ATTRIBUTE_ACK );

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"속성 초기화 요청중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_ItemID ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_INIT_ITEM_ATTRIBUTE_ACK )
{
    KItemPtr pkInvenItem;
    KItemPtr pkUseItem;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    pkInvenItem = m_kInventory.FindItemByItemUID( kPacket_.m_kItem.m_ItemUID );
    pkUseItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_ItemID );
    if( !pkInvenItem || !pkUseItem ) {
        SET_ERR_GOTO( ERR_ATTRIBUTE_SELECT_01, END_PROC );
    }

    // 속성 초기화한 아이템 리스트.
    pkInvenItem->m_vecAttribute = kPacket_.m_kItem.m_vecAttribute;
    kPacket_.m_kItem = *pkInvenItem;

    // 사용한 속성 초기화 아이템 갯수 차감.
    pkUseItem->m_nCount -= 1;
    kPacket_.m_vecItem.push_back( *pkUseItem );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_ATTRIBUTE_INIT_01, kPacket_.m_nOK, 1 );   // 속성을 초기화 할 아이템이 인벤에 없음.
        ERR_CASE( ERR_ATTRIBUTE_INIT_02, kPacket_.m_nOK, 2 );   // 속성 초기화할 아이템이 에픽 등급 이상이 아님.
        ERR_CASE( ERR_ATTRIBUTE_INIT_03, kPacket_.m_nOK, 3 );   // 속성 초기화할 아이템이 코디아이템임.
        ERR_CASE( ERR_ATTRIBUTE_INIT_04, kPacket_.m_nOK, 4 );   // 속성초기화 DB에 기록 하는데 오류 발생.
        ERR_CASE( ERR_ATTRIBUTE_INIT_05, kPacket_.m_nOK, 5 );   // 속성 초기화를 할수 있는 아이템이 아님.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99; // unknown error
        break;
    }

    SEND_RECEIVED_PACKET( EVENT_INIT_ITEM_ATTRIBUTE_ACK );
    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"속성 초기화 응답 MSG :" << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName)
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kItem.m_vecAttribute.size() ) << END_LOG;
}

_IMPL_ON_FUNC( DB_EVENT_SOCKS_INFO_ACK, KUserSocksInfo )
{
    SiKSocks()->GetSocksScript( m_mapSocksInfo );

    std::map< GCITEMID, KSocksInfo >::iterator mit;
    for ( mit = kPacket_.begin(); mit != kPacket_.end(); ++mit ) {

        m_mapSocksInfo[mit->first].m_nIsHangup = mit->second.m_nIsHangup;
        m_mapSocksInfo[mit->first].m_dwRemainSec = mit->second.m_dwRemainSec;
    }

    kPacket_ = m_mapSocksInfo;

    START_LOG( clog, L"DB에서 가져온 양말 정보 사이즈, Name: " << GetName() )
        << BUILD_LOG( kPacket_.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SOCKS_INFO_NOT );
}

_IMPL_ON_FUNC( EVENT_SOCKS_HANGUP_REQ, std::vector< GCITEMID > )
{
    VERIFY_STATE(( 4, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM, KUserFSM::STATE_SQUARE ));
    SET_ERROR( ERR_UNKNOWN );
    KSocksResult kPacket;
    kPacket.m_kSocksInfo = m_mapSocksInfo; // 에러난 경우 서버의 데이터를 다시 보내주기
    //std::map< GCITEMID, KSocksInfo > mapSocksInfoCopy = m_mapSocksInfo; // 복사본을 넘기기, 성공할 때까지 데이터 변경 미루기
    //std::map< GCITEMID, KSocksInfo > mapSocksInfoDB; // 걸 양말만 디비로 보내기

    std::vector< GCITEMID >::const_iterator vit;

    //CHECK_EVENT_RECORD( KUserEvent::DB_EVENT_SOCKS_HANGUP_REQ, ERR_PROC );


    if ( SiKSocks()->IsRun() == false ) {
        SET_ERR_GOTO( ERR_SOCKS_00, END_PROC );
    }

    // 양말 아이템 있는지 확인
    for ( vit = kPacket_.begin(); vit != kPacket_.end(); ++vit ) {

        KItemPtr spItem = m_kInventory.FindDurationItemByItemID( *vit );
        if ( !spItem ) {
            SET_ERR_GOTO( ERR_SOCKS_01, END_PROC );
        }

        std::map< GCITEMID, KSocksInfo >::iterator mit = m_mapSocksInfo.find( *vit );

        if ( mit == m_mapSocksInfo.end() ) {
            KSocksInfo kInitData;
            kInitData.Clear();
            kInitData.m_Socks.m_ItemID = *vit;
            kInitData.m_nIsHangup = KSocksInfo::HANGUP;
            kInitData.m_dwRemainSec = SiKSocks()->GetSocksSecond( *vit );
            kInitData.m_dwPeriodSec = SiKSocks()->GetSocksSecond( *vit );

            m_mapSocksInfo.insert( std::make_pair( *vit, kInitData ) );            
        }

        // 걸 수 있는 상태인지 확인, 이미 걸려있는지 확인
        else if ( mit->second.m_nIsHangup == KSocksInfo::HANGUP ) {
            SET_ERR_GOTO( ERR_SOCKS_02, END_PROC );
        }

        if ( mit != m_mapSocksInfo.end() ) {
            mit->second.m_nIsHangup = KSocksInfo::HANGUP;
            mit->second.m_dwRemainSec = SiKSocks()->GetSocksSecond( mit->first );
            mit->second.m_dwPeriodSec = SiKSocks()->GetSocksSecond( mit->first );

            //m_mapSocksInfo[mit->first] =  mit->second;
        }

        // 성공인 경우의 처리
        if ( spItem && spItem->m_nCount > 0 ) {
            spItem->m_nCount -= 1; // 걸기 성공 양말아이템 수량 감소
            kPacket.m_vecUseItem.push_back( *spItem );    
        }

    }
    // 양말 타이머 작동시작 , 아래 주석을 따라가세요, Ctrl + 클릭
    // file:./UserHelper3.cpp#UpdateSocksTime

    //QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_SOCKS_HANGUP_REQ, mapSocksInfoDB );
    SET_ERROR( NET_OK );
    kPacket.m_kSocksInfo = m_mapSocksInfo;

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_SOCKS_00, kPacket.m_nOK, 1 );    // 양말 이벤트 기간이 아님.
        ERR_CASE( ERR_SOCKS_01, kPacket.m_nOK, 2 );    // 양말 아이템이 없는데 걸기 요청함. )
        ERR_CASE( ERR_SOCKS_02, kPacket.m_nOK, 3 );    // 이미 걸려있는 양말 아이템을 걸기 요청함.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );

        default: 
            START_LOG_WITH_NAME( cerr )
                << BUILD_LOG( NetError::GetLastNetErrMsg() )
                << END_LOG;
            break;
    }

    //if ( !IS_CORRECT( NET_OK ) ) {
    SEND_DATA( EVENT_SOCKS_HANGUP_ACK, kPacket );
    //}

    _LOG_SUCCESS( IS_CORRECT( NET_OK ) || NetError::GetLastNetError() == NetError::ERR_NOT_FINISHED_YET, L"양말 걸기 요청: " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( m_mapSocksInfo.size() )
        << BUILD_LOG( kPacket_.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_SOCKS_HANGUP_ACK, KSocksResult )
{
    std::map< GCITEMID, KSocksInfo >::iterator mit;
    for ( mit = kPacket_.m_kSocksInfo.begin(); mit != kPacket_.m_kSocksInfo.end() ; ++mit ) {
        
        if ( mit->second.m_nIsHangup == KSocksInfo::HANGUP ) { // 해당 양말이 ok 면 nIsHangup = HANGUP 이다
            m_mapSocksInfo[mit->first] = mit->second; // ok 인 경우 덮어쓰겠다

            KItemPtr spItem = m_kInventory.FindDurationItemByItemID( mit->first ); 
            if ( spItem && spItem->m_nCount > 0 ) {
                spItem->m_nCount -= 1; // 걸기 성공 양말아이템 수량 감소
                kPacket_.m_vecUseItem.push_back( *spItem );            
            }
        }
        _LOG_SUCCESS( mit->second.m_nIsHangup == KSocksInfo::HANGUP, L"양말 걸기 결과 Name: " << GetName() )
            << BUILD_LOG( mit->first )
            << BUILD_LOG( mit->second.m_dwRemainSec )
            << BUILD_LOG( mit->second.m_vecItemList.size() )
            << BUILD_LOG( mit->second.m_prMaterialID.first ) << END_LOG;
    }

    kPacket_.m_kSocksInfo = m_mapSocksInfo;

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"양말 걸기 응답, Name : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kSocksInfo.size() )
        << BUILD_LOG( kPacket_.m_vecUseItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SOCKS_HANGUP_ACK );
}

_IMPL_ON_FUNC( EVENT_SOCKS_COLLECT_REQ, std::vector< GCITEMID > )
{
    VERIFY_STATE(( 4, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM, KUserFSM::STATE_SQUARE ));
    SET_ERROR( ERR_UNKNOWN );

    KSocksResult kPacket;
    kPacket.m_kSocksInfo = m_mapSocksInfo; // 에러난 경우 서버의 데이터를 다시 보내주기
    std::map< GCITEMID, KSocksInfo > mapSocksInfoCopy = m_mapSocksInfo; // 복사본을 넘기기, 성공할 때까지 데이터 변경 미루기
    std::map< GCITEMID, KSocksInfo > mapSocksInfoDB; // 내릴 양말만 디비로 보내기(sp호출최소화)
    std::vector< GCITEMID >::const_iterator vit;

    if ( SiKSocks()->IsRun() == false ) {
        SET_ERR_GOTO( ERR_SOCKS_00, END_PROC );
    }

    for ( vit = kPacket_.begin(); vit != kPacket_.end(); ++vit ) {

        std::map< GCITEMID, KSocksInfo >::iterator mit = mapSocksInfoCopy.find( *vit );

        if ( mit == mapSocksInfoCopy.end() ) {
            SET_ERR_GOTO( ERR_SOCKS_01, END_PROC );         
        }
        // 걷을 수 있는 상태인지 확인
        if ( mit->second.m_nIsHangup == KSocksInfo::NOHANG ) {
            SET_ERR_GOTO( ERR_SOCKS_04, END_PROC );
        }

        if ( mit->second.m_dwRemainSec > 0 ) {
            SET_ERR_GOTO( ERR_SOCKS_05, END_PROC );
        }

        mit->second.m_nIsHangup = KSocksInfo::NOHANG;
        SiKSocks()->GetRewardItem( mit->first, mit->second.m_vecItemList );

        mapSocksInfoDB[mit->first] = mit->second;
    }

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_SOCKS_COLLECT_REQ, mapSocksInfoDB );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );       // 성공.
        ERR_CASE( ERR_SOCKS_01, kPacket.m_nOK, 2 ); // 양말 아이템이 없는데 걸기 요청함.
        ERR_CASE( ERR_SOCKS_04, kPacket.m_nOK, 5 ); // 안걸려있는 양말 아이템을 받기 요청함.
        ERR_CASE( ERR_SOCKS_05, kPacket.m_nOK, 6 ); // 남은 시간이 0 이 아닌 양말을 받기 요청함.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
        default: 
            START_LOG_WITH_NAME( cerr )
                << BUILD_LOG( NetError::GetLastNetErrMsg() )
                << END_LOG;
            break;
    }
    if ( kPacket.m_nOK != 0 ) {
        SEND_DATA( EVENT_SOCKS_COLLECT_ACK, kPacket );
    }
    _LOG_SUCCESS( IS_CORRECT( NET_OK ) || NetError::GetLastNetError() == NetError::ERR_NOT_FINISHED_YET, L"양말 획득 요청: " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( mapSocksInfoCopy.size() )
        << BUILD_LOG( kPacket_.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_SOCKS_COLLECT_ACK, KSocksResult )
{
    std::map< GCITEMID, KSocksInfo >::iterator mit;

    for ( mit = kPacket_.m_kSocksInfo.begin(); mit != kPacket_.m_kSocksInfo.end() ; ++mit ) {

        if ( mit->second.m_nIsHangup == KSocksInfo::NOHANG ) { // 해당 양말이 ok면 nIsHangup = NOHANG 이다
            m_mapSocksInfo[mit->first] = mit->second; // ok 인 경우 덮어쓰겠다
        }
    }

    if ( kPacket_.m_nOK == 0 ) {
        m_kInventory.AddItemList( kPacket_.m_vecRewardItem );
    }

    kPacket_.m_kSocksInfo = m_mapSocksInfo;

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"양말 내리기  Name: " << GetName() )
        << BUILD_LOG( kPacket_.m_vecRewardItem.size() )
        << BUILD_LOG( kPacket_.m_vecUseItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SOCKS_COLLECT_ACK );
}

IMPL_ON_FUNC( EVENT_SOCKS_MATERIAL_EXCHANGE_NOT )
{
    KEVENT_SOCKS_MATERIAL_EXCHANGE_NOT::iterator mit;
    for ( mit = kPacket_.begin(); mit != kPacket_.end(); ++mit ) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( mit->first );
        if ( !spUser ) {
            START_LOG( cwarn, L"양말 아이템 교환중 유저가 게임서버에 존재하지 않음.. UID : " << mit->first ) << END_LOG;
            continue;
        }
        spUser->m_kInventory.AddItemList( mit->second.m_vecReward );
        spUser->SendPacket( EVENT_TYPE::EVENT_SOCKS_MATERIAL_EXCHANGE_NOT, mit->second );
    }
}

_IMPL_ON_FUNC( EVENT_SOCKS_HANGUP_COMPLETE_NOT, KSocksResult )
{
    kPacket_.m_kSocksInfo = m_mapSocksInfo;

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( kPacket_.m_kSocksInfo.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SOCKS_HANGUP_COMPLETE_NOT );
}

IMPL_ON_FUNC_NOPARAM( EVENT_SOCKS_MATERIAL_EXCHANGE_REQ )
{
    SET_ERROR( ERR_UNKNOWN );
    KSocksExchangeData kPacket;

    std::map< GCITEMID, std::pair< GCITEMID, int > > mapMaterial;
    SiKSocks()->GetSocksMaterialList( mapMaterial );

    KSocksExchangeData kSocksExchangeData;

    std::map< GCITEMID, std::pair< GCITEMID, int > >::iterator mit;
    for ( mit = mapMaterial.begin(); mit != mapMaterial.end(); ++mit ) {
        KItemPtr spItem = m_kInventory.FindItemByItemID( mit->second.first );
        if ( !spItem ) {
            continue;
        }

        int MaterialCount = std::max<int>( mit->second.second, 1 ); // 나누기 0 방지
        int MultipleCount = std::max<int>( (spItem->m_nCount / MaterialCount), 1 ); // 곱하기 0 방지
        int TotalUseCount = MaterialCount * MultipleCount;

        if ( TotalUseCount <= spItem->m_nCount ) {
            spItem->m_nCount -= TotalUseCount; // 재료 아이템 소모

            KSocksInfo kSocksInfo;
            SiKSocks()->GetSocksInfo( mit->first, kSocksInfo );

            kSocksInfo.m_Socks.m_nDuration *= MultipleCount;

            kSocksExchangeData.m_vecExchangeSocks.push_back( kSocksInfo.m_Socks );
            kSocksExchangeData.m_vecUseItem.push_back( *spItem );
        }
    } // end of for ( spUser ...

    if ( false == kSocksExchangeData.m_vecExchangeSocks.empty() ) {
        kSocksExchangeData.m_nOK = 0;
        SET_ERROR( NET_OK );
        QUEUEING_EVENT_TO_DB( DB_EVENT_SOCKS_INSERT_ITEM_NEW, kSocksExchangeData );
    }
    else {
        SET_ERROR( ERR_SOCKS_06 );
    }

    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );       // 성공.
        ERR_CASE( ERR_SOCKS_01, kPacket.m_nOK, 2 ); // 양말 아이템이 없는데 걸기 요청함.
        ERR_CASE( ERR_SOCKS_04, kPacket.m_nOK, 5 ); // 안걸려있는 양말 아이템을 받기 요청함.
        ERR_CASE( ERR_SOCKS_05, kPacket.m_nOK, 6 ); // 남은 시간이 0 이 아닌 양말을 받기 요청함.
        ERR_CASE( ERR_SOCKS_06, kPacket.m_nOK, 7 ); // 양말 재료 수량이 없는데 바꾸기 요청함.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
        default: 
            START_LOG_WITH_NAME( cerr )
                << BUILD_LOG( NetError::GetLastNetErrMsg() )
                << END_LOG;
            break;
    }
    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_DATA( EVENT_SOCKS_MATERIAL_EXCHANGE_ACK, kPacket );
    }

}

_IMPL_ON_FUNC( EVENT_SOCKS_MATERIAL_EXCHANGE_ACK, KSocksExchangeData )
{
    if ( kPacket_.m_nOK == 0 ) {
        m_kInventory.AddItemList( kPacket_.m_vecReward );
    }
    
    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"양말 재료를 양말로 교환" << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecExchangeSocks.size() )
        << BUILD_LOG( kPacket_.m_vecUseItem.size() )
        << BUILD_LOG( kPacket_.m_vecReward.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SOCKS_MATERIAL_EXCHANGE_ACK );
}

_IMPL_ON_FUNC( EVENT_UNLOCK_CHANGE_WEAPON_REQ, char )
{
    // 무기체인지 슬롯 언락
    SET_ERROR( ERR_UNKNOWN );
    std::map<GCITEMID,int> mapUnlockItem;
    std::map<GCITEMID,int>::const_iterator cmit;
    KItemPtr pkItem;
    bool bExistUnlockItem = false;
    int nNeedUnlockItemCount = 0;
    int nCharType = static_cast<int>(kPacket_);

    KEVENT_UNLOCK_CHANGE_WEAPON_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_cCharType = kPacket_;
    kPacket.m_vecItem.clear();

    KCharacterInfo* pkCharInfo = GetCharacterInfo( nCharType );
    if ( !pkCharInfo ) {
        SET_ERR_GOTO( ERR_CHANGE_WEAPON_00, END_PROC );
    }

    if ( false == SiKChangeWeaponManager()->IsPossibleLv( nCharType, static_cast<int>(pkCharInfo->m_dwLevel) ) ) {
        SET_ERR_GOTO( ERR_CHANGE_WEAPON_01, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_UNLOCK_CHANGE_WEAPON_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_CHANGE_WEAPON_00, kPacket.m_nOK, 1 ); // 무기교체 언락할 캐릭터를 보유하지 않음.
        ERR_CASE( ERR_CHANGE_WEAPON_01, kPacket.m_nOK, 2 ); // 무기교체 레벨 제한에 걸림.
        ERR_CASE( ERR_CHANGE_WEAPON_02, kPacket.m_nOK, 3 ); // 무기교체 언락 아이템 수량 부족.
        ERR_CASE( ERR_CHANGE_WEAPON_03, kPacket.m_nOK, 4 ); // 무기교체 언락할 캐릭터 슬롯이 이미 열려있음.
        ERR_CASE( ERR_CHANGE_WEAPON_04, kPacket.m_nOK, 5 ); // 무기교체 DB기록 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );

        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOGc( kPacket.m_cCharType )
        << BUILD_LOG( kPacket.m_vecItem.size() ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_UNLOCK_CHANGE_WEAPON_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_END_GAME_BROAD, KEndGameAck )
{
    std::vector<KGameResultOut>::iterator vitResult;
    for ( vitResult = kPacket_.m_vecGameResult.begin() ; vitResult != kPacket_.m_vecGameResult.end() ; ++vitResult ) {
        if ( vitResult->m_dwUID != GetUID() ) {
            vitResult->m_vecMissionSlot.clear();
            START_LOG( clog, L"다른 파티원의 미션 정보는 패킷에서 제외함.. Login : " << GetName() ) << END_LOG;
        }
    }

    SEND_COMPRESS_PACKET( EVENT_END_GAME_BROAD, kPacket_ );

    // 자신의 정보일 경우 모험 이벤트 전리품 정보 전달
    if ( SiKAdventure()->IsRun() ) {
        std::vector< KGameResultOut >::iterator vit;
        vit = std::find_if( kPacket_.m_vecGameResult.begin(), kPacket_.m_vecGameResult.end(),
            boost::bind( &KGameResultOut::m_dwUID, _1 ) == GetUID() );

        SEND_DATA( EVENT_ADVENTURE_USER_COLLECT_DATA_NOT, m_kAdventureUserData.m_mapCollectData );
        START_LOG( clog, L"유저의 모험 이벤트 전리품 정보 전달. Login : " << GetName() ) << END_LOG;
    }
}

IMPL_ON_FUNC( EVENT_ITEM_ATTRIBUTE_RANDOM_SELECT_REQ )
{
    KEVENT_ITEM_ATTRIBUTE_RANDOM_SELECT_ACK kPacket;
    KDB_EVENT_ITEM_ATTRIBUTE_RANDOM_SELECT_REQ kDBPacket;
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
    if( !pkInvenItem  ) { // 랜덤 속성 선택한 아이템 존재하지 않음.
        SET_ERR_GOTO( ERR_RANDOM_ATTRIBUTE_SELECT_00, END_PROC );
    }

    // 구매후 처음 한번은 한번 랜덤 속성 선택이 가능해야 한다.
    // 랜덤 속성 아이템 없어도 되고.., 있어도 차감되지 않아야 한다.
    // 에픽 등급 이상인지 체크.
    int nCount = 0;
    for( vit = pkInvenItem->m_vecAttribute.begin() ; vit != pkInvenItem->m_vecAttribute.end() ; ++vit ) {
        if( vit->m_cState == KAttributeInfo::ATS_OPENED ) {
            nCount++;
        }
    }

    // 아이템 구매후 첫번째 선택시..,
    if( nCount == pkInvenItem->m_vecAttribute.size() ) {
        kDBPacket.m_bInitItem = true;
    } else {
        pkUseItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_ItemID );
        if( !pkUseItem  ) { // 랜덤 속성 아이템이 존재하지 않음.
            SET_ERR_GOTO( ERR_RANDOM_ATTRIBUTE_SELECT_00, END_PROC );
        }
        kDBPacket.m_bInitItem = false;
        kDBPacket.m_kUseItem = *pkUseItem;
    }

    // 속성 초기화 아이템인지 체크.
    if( !kDBPacket.m_bInitItem && !SiKItemManager()->m_kAttributeDecider.CheckInitAttributeItem( pkInvenItem->m_cGradeID, kPacket_.m_ItemID ) ) {
        SET_ERR_GOTO( ERR_RANDOM_ATTRIBUTE_SELECT_04, END_PROC );
    }

    // 코디 아이템인지 체크.
    if( pkInvenItem->CheckItemType(KItem::TYPE_LOOK) ) {
        SET_ERR_GOTO( ERR_RANDOM_ATTRIBUTE_SELECT_02, END_PROC );
    }

    kDBPacket.m_ItemID = kPacket_.m_ItemID;
    kDBPacket.m_kItem = *pkInvenItem;

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
    SiKItemManager()->m_kAttributeDecider.GetSelectiveAttributeList( nAttributeTableID, nAttributeCount, (int)pkInvenItem->m_cGradeID, kDBPacket.m_listAttributeList );

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_ITEM_ATTRIBUTE_RANDOM_SELECT_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_00, kPacket.m_nOK, 1 );   // 랜덤 속성을 적용할 아이템이 인벤에 없음.
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_01, kPacket.m_nOK, 2 );   // 랜덤 속성 적용할 아이템이 에픽 등급 이상이 아님.
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_02, kPacket.m_nOK, 3 );   // 랜덤 속성 적용할 아이템이 코디아이템임.
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_03, kPacket.m_nOK, 4 );   // 랜덤 속성 DB에 기록 하는데 오류 발생.
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_04, kPacket.m_nOK, 5 );   // 랜덤 속성을 할수 있는 아이템이 아님.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99; // unknown error
        break;
    }

    if ( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_ITEM_ATTRIBUTE_RANDOM_SELECT_ACK );

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"랜덤 속성 적용 요청 MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_ItemID ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_ITEM_ATTRIBUTE_RANDOM_SELECT_ACK )
{
    KItemPtr pkInvenItem;
    KItemPtr pkUseItem;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) && !IS_CORRECT( ERR_RANDOM_ATTRIBUTE_SELECT_05 ) ) {
        goto END_PROC;
    }

    pkInvenItem = m_kInventory.FindItemByItemUID( kPacket_.m_kItem.m_ItemUID );
    if( !pkInvenItem ) {
        SET_ERR_GOTO( ERR_RANDOM_ATTRIBUTE_SELECT_00, END_PROC );
    }

    // 속성 초기화한 아이템 리스트.
    pkInvenItem->m_vecAttribute = kPacket_.m_kItem.m_vecAttribute;
    kPacket_.m_kItem = *pkInvenItem;

    // 사용한 속성 초기화 아이템 갯수 차감.
    if( !kPacket_.m_bInitItem ) {
        pkUseItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_ItemID );
        if( !pkUseItem ) {
            SET_ERR_GOTO( ERR_RANDOM_ATTRIBUTE_SELECT_00, END_PROC );
        }
        pkUseItem->m_nCount = kPacket_.m_kUseItem.m_nCount;
        pkUseItem->m_nInitCount = kPacket_.m_kUseItem.m_nCount;
        kPacket_.m_vecItem.push_back( *pkUseItem );

        if ( pkUseItem && pkUseItem->m_nCount <= 0 ) {
            m_kInventory.RemoveItem( kPacket_.m_kUseItem.m_ItemID, kPacket_.m_kUseItem.m_ItemUID );
        }
    }

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_00, kPacket_.m_nOK, 1 );   // 랜덤 속성을 적용할 아이템이 인벤에 없음.
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_01, kPacket_.m_nOK, 2 );   // 랜덤 속성 적용할 아이템이 에픽 등급 이상이 아님.
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_02, kPacket_.m_nOK, 3 );   // 랜덤 속성 적용할 아이템이 코디아이템임.
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_03, kPacket_.m_nOK, 4 );   // 랜덤 속성 DB에 기록 하는데 오류 발생.
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_04, kPacket_.m_nOK, 5 );   // 랜덤 속성을 할수 있는 아이템이 아님.
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_05, kPacket_.m_nOK, 6 );   // 랜덤 속성 초기화 아이템 DB 동기화 실패
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99; // unknown error
        break;
    }

    SEND_RECEIVED_PACKET( EVENT_ITEM_ATTRIBUTE_RANDOM_SELECT_ACK );
    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"랜덤 속성 적용 응답 MSG :" << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName)
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kItem.m_vecAttribute.size() ) << END_LOG;
}

//IMPL_ON_FUNC_NOPARAM( EVENT_ONEADAY_INFO_REQ )
//{
//    std::vector<KOneadayItem> kPacket;
//    SiKOneaday()->GetInfo( kPacket );
//
//    SEND_PACKET( EVENT_ONEADAY_INFO_ACK );
//}

_IMPL_ON_FUNC( EVENT_USER_SERVER_ROOM_REQ, PAIR_DWORD_WSTRING )
{
    // kPacket_.first : 대상 유저의 UserUID.
    // kPacket_.second : 대상 유저의 NickName.
    if ( false == SiKGSSimLayer()->GetServerRoomInfoEnable() ) {
        return;
    }

    KUserPtr spUser;
    DWORD& dwUserUID = kPacket_.first;
    std::wstring& strNickName = kPacket_.second;

    KEVENT_USER_SERVER_ROOM_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_kServerRoomInfo.m_dwUserUID = dwUserUID;
    kPacket.m_kServerRoomInfo.m_dwServerUID = SiKGameServer()->GetUID();

    SET_ERROR( ERR_UNKNOWN );

    if ( ::GetTickCount() - GetTick( USER_WHERE ) < SiKGSSimLayer()->GetServerRoomInfoTick() ) {
        SET_ERR_GOTO( ERR_SERVER_ROOM_00, END_PROC );
    }

    // UID로 입력안한 경우,
    if ( dwUserUID == 0 ) {
        if ( strNickName.empty() ) {
            START_LOG( cwarn, L"닉네임이 비어있음." ) << END_LOG;
            SET_ERR_GOTO( ERR_SERVER_ROOM_03, END_PROC );
        }

        spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( SiKGSSimLayer()->TranslateNickToLogin( strNickName ) );
        if ( !spUser ) {
            // 서버에 없는 경우, DB에 UID 문의
            QUEUEING_EVENT_TO_DB( EVENT_USER_SERVER_ROOM_REQ, strNickName );
            SET_ERR_GOTO( ERR_SERVER_ROOM_04, END_PROC );
        }

        kPacket.m_kServerRoomInfo.m_dwUserUID = spUser->GetUID();
    }

    kPacket.m_nOK = NetError::NET_OK;
    QueueingEvent( KUserEvent::EVENT_USER_SERVER_ROOM_ACK, kPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,             kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_SERVER_ROOM_00, kPacket.m_nOK, 1 ); // 요청 주기가 짧습니다.
        ERR_CASE( ERR_SERVER_ROOM_01, kPacket.m_nOK, 2 ); // 대상이 채널에 없습니다.
        ERR_CASE( ERR_SERVER_ROOM_02, kPacket.m_nOK, 3 ); // 대상이 방에 없습니다.
        ERR_CASE( ERR_SERVER_ROOM_03, kPacket.m_nOK, 4 ); // 대상 유저를 찾을 수 없습니다.
        ERR_CASE( ERR_SERVER_ROOM_04, kPacket.m_nOK, 5 ); // 대상이 같은 서버에 없습니다.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = -99; // unknown error
        break;
    }

    _LOG_CONDITION( kPacket.m_nOK == 0 || kPacket.m_nOK == 5, clog, cwarn, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kServerRoomInfo.m_dwUserUID )
        << BUILD_LOG( kPacket.m_kServerRoomInfo.m_dwServerUID )
        << BUILD_LOG( kPacket.m_kServerRoomInfo.m_dwChannelUID )
        << BUILD_LOG( kPacket.m_kServerRoomInfo.m_usRoomID )
        << BUILD_LOG( kPacket.m_kServerRoomInfo.m_bPublic )
        << BUILD_LOG( dwUserUID )
        << BUILD_LOG( strNickName ) << END_LOG;

    if ( !IS_CORRECT( NET_OK ) && !IS_CORRECT( ERR_SERVER_ROOM_04 ) ) {
        SEND_PACKET( EVENT_USER_SERVER_ROOM_ACK );
    }
}

IMPL_ON_FUNC( EVENT_USER_SERVER_ROOM_ACK )
{
    KUserPtr spUser;
    KChannel* pkChannel;
    KRoom* pkRoom;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_kServerRoomInfo.m_dwUserUID );
    if( !spUser ) {
        // 같은 서버에 없으면 센터서버로 문의
        KECN_USER_SERVER_ROOM_REQ kCnPacket;
        kCnPacket.m_kSendUser.m_dwUserUID = GetUID();
        kCnPacket.m_kSendUser.m_dwServerUID = SiKGameServer()->GetUID();
        kCnPacket.m_kTargetUser.m_dwUserUID = kPacket_.m_kServerRoomInfo.m_dwUserUID;
        SiKCnConnector()->SendPacket( KCenterEvent::ECN_USER_SERVER_ROOM_REQ, kCnPacket );
        SET_ERR_GOTO( ERR_SERVER_ROOM_04, END_PROC );
    }

    // 채널 확인
    pkChannel = spUser->GetChannel();
    if ( pkChannel == NULL ) {
        SET_ERR_GOTO( ERR_SERVER_ROOM_01, END_PROC );
    }

    // 룸 확인
    pkRoom = spUser->GetRoom();
    if ( pkRoom == NULL ) {
        SET_ERR_GOTO( ERR_SERVER_ROOM_02, END_PROC );
    }

    kPacket_.m_kServerRoomInfo.m_dwChannelUID = pkChannel->GetUID();
    kPacket_.m_kServerRoomInfo.m_usRoomID = pkRoom->GetRoomID();
    kPacket_.m_kServerRoomInfo.m_iGameMode = pkRoom->m_iGameMode;
    kPacket_.m_kServerRoomInfo.m_bPublic = pkRoom->m_bPublic;

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,             kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_SERVER_ROOM_00, kPacket_.m_nOK, 1 ); // 요청 주기가 짧습니다.
        ERR_CASE( ERR_SERVER_ROOM_01, kPacket_.m_nOK, 2 ); // 대상이 채널에 없습니다.
        ERR_CASE( ERR_SERVER_ROOM_02, kPacket_.m_nOK, 3 ); // 대상이 방에 없습니다.
        ERR_CASE( ERR_SERVER_ROOM_03, kPacket_.m_nOK, 4 ); // 대상 유저를 찾을 수 없습니다.
        ERR_CASE( ERR_SERVER_ROOM_04, kPacket_.m_nOK, 5 ); // 대상이 같은 서버에 없습니다.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = -99; // unknown error
        break;
    }

    _LOG_CONDITION( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 5, clog, cwarn, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kServerRoomInfo.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_kServerRoomInfo.m_dwServerUID )
        << BUILD_LOG( kPacket_.m_kServerRoomInfo.m_dwChannelUID )
        << BUILD_LOG( kPacket_.m_kServerRoomInfo.m_usRoomID )
        << BUILD_LOG( kPacket_.m_kServerRoomInfo.m_iGameMode )
        << BUILD_LOG( kPacket_.m_kServerRoomInfo.m_bPublic ) << END_LOG;

    // 같은 서버에 없는 경우는 센터서버에서 다시 응답을 준다.
    if ( !IS_CORRECT( ERR_SERVER_ROOM_04 ) ) {
        SEND_RECEIVED_PACKET( EVENT_USER_SERVER_ROOM_ACK );
    }

    SetTick( USER_WHERE );
}

_IMPL_ON_FUNC( EVENT_TODAYS_POPUP_INFO_ACK, int )
{
    START_LOG( clog, L"Login : " << GetName() << L", Popup Enable : " << (kPacket_ > 0 ? L"false" : L"true" ) ) << END_LOG;
    SEND_RECEIVED_PACKET( EVENT_TODAYS_POPUP_INFO_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_DISABLE_TODAYS_POPUP_NOT )
{
    START_LOG( clog, L"일일 팝업창 끄기 요청. Login : " << GetName() ) << END_LOG;
    QUEUEING_ID_TO_DB( EVENT_DISABLE_TODAYS_POPUP_NOT );
}

IMPL_ON_FUNC( EVENT_RESET_SKILL_REQ )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));

    KEVENT_RESET_SKILL_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_cCharType = kPacket_.m_cCharType;
    KItemPtr pkItem;

    if ( GetCurrentCharType() != kPacket_.m_cCharType ) {
        START_LOG( cerr, L"서버-클라이언트의 현재 캐릭터가 다르다, User: " << GetName() )
            << END_LOG;
        SET_ERR_GOTO( ERR_SKILL_03, END_PROC );
    }

    // 스킬전체초기화 아이템이 아님.
    if ( false == SiKSkillManager()->IsResetSkillItem( kPacket_.m_ItemID ) ) {
        SET_ERR_GOTO( ERR_SKILL_27, END_PROC );
    }

    // 스킬전체초기화 아이템 수량이 부족함.
    if ( GetDurItemCount( kPacket_.m_ItemID ) <= 0 ) {
        SET_ERR_GOTO( ERR_SKILL_28, END_PROC );
    }

    // 보유하지 않은 캐릭터를 스킬전체 초기화하려고 함
    if ( false == IsCharExist( kPacket_.m_cCharType ) ) {
        SET_ERR_GOTO( ERR_SKILL_29, END_PROC );
    }

    // 스킬 초기화
    ResetEquipSkill( kPacket_.m_cCharType );
    LIF( ResetTrainedSkill( kPacket_.m_cCharType ) );
    kPacket.m_mapTrainedSkill = m_mapTrainedSkill; // 수정된 배운 스킬정보 가져오기.

    // 아이템 삭감
    pkItem = m_kInventory.FindItemByItemID( kPacket_.m_ItemID );
    if ( pkItem ) {
        --pkItem->m_nCount;
        pkItem->m_nCount = std::max<int>( pkItem->m_nCount, 0 );
        kPacket.m_kItem = *pkItem;
    }
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,       kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_SKILL_27, kPacket.m_nOK, 1 ); // 스킬전체초기화 아이템이 아님.
        ERR_CASE( ERR_SKILL_28, kPacket.m_nOK, 2 ); // 스킬전체초기화 아이템 수량이 부족함.
        ERR_CASE( ERR_SKILL_29, kPacket.m_nOK, 3 ); // 보유하지 않은 캐릭터를 스킬전체 초기화하려고 함.
        ERR_CASE( ERR_SKILL_03, kPacket.m_nOK, 4 ); // 해당 캐릭터가 존재하지 않음.

    default:
        START_LOG( cerr, L"스킬전체초기화, 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket.m_nOK ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    SEND_PACKET( EVENT_RESET_SKILL_ACK );

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"스킬전체 초기화 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kItem.m_ItemID )
        << BUILD_LOG( kPacket.m_kItem.m_nCount ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_FASHION_CATALOG_REQ )
{
    QUEUEING_ID_TO_DB( EVENT_FASHION_CATALOG_REQ );
    START_LOG( clog, L"패션 카탈로그 요청.. Login : " << GetName() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_FASHION_CATALOG_ACK )
{
    SEND_RECEIVED_PACKET( EVENT_FASHION_CATALOG_ACK );
    START_LOG( clog, L"패션 카탈로그 전달.. Login : " << GetName() << L", size : " << kPacket_.size() ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_USER_RECOM_TYPE_REQ )
{
    int nUserTpe = 0;
    if ( SiKRecomHelper()->IsNewBeginTerm( KncUtil::TimeToInt(m_tmFirstLoginTime) ) ) {
        nUserTpe = 1; // 신규유저
    }
    else if ( IsReturnRecomUser() ) {
        nUserTpe = 2; // 복귀유저
    }
    else {
        nUserTpe = 4; // 기존 유저
    }

    QUEUEING_EVENT_TO_DB( EVENT_USER_RECOM_TYPE_REQ, nUserTpe );
    START_LOG( clog, L"반갑다친구야 유저타입 문의.. Login : " << GetName() << L",Type : " << nUserTpe ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_USER_RECOM_TYPE_ACK, int )
{
    // kPacket_ : UserType
    // 1 ; 신규유저
    // 2 ; 복귀유저
    // 4 ; 활성유저
    SEND_RECEIVED_PACKET( EVENT_USER_RECOM_TYPE_ACK );
    START_LOG( clog, L"반갑다친구야 유저타입.. Login : " << GetName() << L", UserType : " << kPacket_ ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_PLANT_TREE_INFO_REQ )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL ));
    KEVENT_PLANT_TREE_INFO_ACK kPacket;
    SET_ERROR( ERR_UNKNOWN );

    // 나무나무 이벤트 기간인가?
    if ( !SiKPlantTree()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_PLANT_TREE_00, END_PROC );
    }

    if ( m_kPlantTreeInfo.IsLoaded() ) { // 이미 DB에서 데이터를 가져왔으면..,
        m_kPlantTreeInfo.GetPlantTreeUserInfo( kPacket.m_dwContinentType, kPacket.m_dwCurrentPosition, kPacket.m_dwTreeStatus );
        SET_ERR_GOTO( ERR_PLANT_TREE_01, END_PROC );
    }

    SET_ERROR( NET_OK );
    QUEUEING_ID_TO_DB_CHECK( EVENT_PLANT_TREE_INFO_REQ );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_PLANT_TREE_01,    kPacket.m_nOK, 1 );     // DB에서 초기 데이터 이미 가져왔음.
        ERR_CASE( ERR_PLANT_TREE_00,    kPacket.m_nOK, 2 );     // 나무나무 이벤트 기간이 아님.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"나무나무 정보 요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 || kPacket.m_nOK == 1, L"나무나무 정보 요청 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_PLANT_TREE_INFO_ACK );
    }
}

IMPL_ON_FUNC( EVENT_PLANT_TREE_INFO_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    m_kPlantTreeInfo.SetPlantTreeData( kPacket_ );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_PLANT_TREE_01,    kPacket_.m_nOK, 1 );     // DB에서 초기 데이터 이미 가져왔음.
        ERR_CASE( ERR_PLANT_TREE_00,    kPacket_.m_nOK, 2 );     // 나무나무 이벤트 기간이 아님.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"나무나무 응답 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 1, L"나무나무 유저 데이터 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwContinentType )
        << BUILD_LOG( kPacket_.m_dwCurrentPosition )
        << BUILD_LOG( kPacket_.m_dwTreeStatus ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_PLANT_TREE_INFO_ACK );
}

IMPL_ON_FUNC( EVENT_PLANT_TREE_ACTION_REQ )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL ));
    KEVENT_PLANT_TREE_ACTION_ACK kPacket;
    KItemPtr pkUseItem;
    SET_ERROR( ERR_UNKNOWN );

    // 나무나무 이벤트 기간인가?
    if ( !SiKPlantTree()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_PLANT_TREE_00, END_PROC );
    }

    // 일반 나무 심기 이외일때만 인벤토리 아이템 가지고 있는지 체크.
    if( kPacket_.m_dwActionType != KEVENT_PLANT_TREE_ACTION_REQ::AT_PLANT_TREE ) {
        pkUseItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_dwItemID ); // 해당 아이템이 인벤토리에 있는지 체크.
        if( !pkUseItem ) {
            SET_ERR_GOTO( ERR_PLANT_TREE_02, END_PROC );
        }
        if ( pkUseItem->m_nCount <= 0 ) {
            SET_ERR_GOTO( ERR_PLANT_TREE_02, END_PROC );
        }
        kPacket_.m_kCoinItem = *pkUseItem;
    }
    // 모든 에러값에 대한 체크는 PlantTreeInfo Class내에서 처리.
    if( !m_kPlantTreeInfo.ApplyUserAction( kPacket_.m_dwActionType, kPacket_.m_dwItemID, kPacket.m_nOK ) ) {
        switch( kPacket.m_nOK ) {
        case KPlantTreeUserData::ET_ERROR_PLANT_TREE:
            SET_ERR_GOTO( ERR_PLANT_TREE_03, END_PROC );
            break;
        case KPlantTreeUserData::ET_ERROR_ITEM:
            SET_ERR_GOTO( ERR_PLANT_TREE_04, END_PROC );
            break;
        case KPlantTreeUserData::ET_ERROR_CONTINENT_TYPE:
            SET_ERR_GOTO( ERR_PLANT_TREE_05, END_PROC );
            break;
        case KPlantTreeUserData::ET_ERROR_WATER_TREE:
            SET_ERR_GOTO( ERR_PLANT_TREE_06, END_PROC );
            break;
        case KPlantTreeUserData::ET_ERROR_MANURE_TREE:
            SET_ERR_GOTO( ERR_PLANT_TREE_07, END_PROC );
            break;
        case KPlantTreeUserData::ET_ERROR_MANURE_TREE_KAIRO:
            SET_ERR_GOTO( ERR_PLANT_TREE_08, END_PROC );
            break;
        case KPlantTreeUserData::ET_UNDEFIND_ACTION_TYPE:
            SET_ERR_GOTO( ERR_PLANT_TREE_09, END_PROC );
            break;
        }
    }
    // 유저의 정보 가져오기.
    m_kPlantTreeInfo.GetPlantTreeUserInfo( kPacket_.m_dwContinentType, kPacket_.m_dwCurrentPosition, kPacket_.m_dwTreeStatus );

    SET_ERROR( NET_OK );
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_PLANT_TREE_ACTION_REQ, kPacket_ );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_PLANT_TREE_00,    kPacket.m_nOK, 1 );     // 나무나무 이벤트 기간이 아님.
        ERR_CASE( ERR_PLANT_TREE_02,    kPacket.m_nOK, 2 );     // 해당 아이템 인벤토리에 없음.
        ERR_CASE( ERR_PLANT_TREE_03,    kPacket.m_nOK, 3 );     // 나무를 심을수 없는 상태.
        ERR_CASE( ERR_PLANT_TREE_04,    kPacket.m_nOK, 4 );     // 잘못된 ItemID.
        ERR_CASE( ERR_PLANT_TREE_05,    kPacket.m_nOK, 5 );     // 잘못된 대륙 타입.
        ERR_CASE( ERR_PLANT_TREE_06,    kPacket.m_nOK, 6 );     // 물을 줄수 없는 상태.
        ERR_CASE( ERR_PLANT_TREE_07,    kPacket.m_nOK, 7 );     // 거름을 줄수 없는 상태.
        ERR_CASE( ERR_PLANT_TREE_08,    kPacket.m_nOK, 8 );     // 로터리에 등록되어 있지 않음.
        ERR_CASE( ERR_PLANT_TREE_09,    kPacket.m_nOK, 9 );     // 정의되지 않은 행동 Type.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"나무나무 이벤트 유저 행동중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 , L"나무나무 이벤트 유저 행동  : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_dwContinentType )
        << BUILD_LOG( kPacket.m_dwCurrentPosition )
        << BUILD_LOG( kPacket.m_dwTreeStatus ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_PLANT_TREE_ACTION_ACK );
    }
}

IMPL_ON_FUNC( EVENT_PLANT_TREE_ACTION_ACK )
{
    KItemPtr pkUseItem;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK )  && !IS_CORRECT( ERR_PLANT_TREE_12 ) ) {
        goto END_PROC;
    }

    // 일반 나무 심기 이외일때만 인벤토리 아이템 가지고 있는지 체크.
    if( kPacket_.m_dwActionType != KEVENT_PLANT_TREE_ACTION_REQ::AT_PLANT_TREE ) {
        pkUseItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_dwItemID );
        if( !pkUseItem ) {
            SET_ERR_GOTO( ERR_PLANT_TREE_02, END_PROC );
        }

        // 사용한 아이템 갯수 차감.
        if( pkUseItem->m_nCount > 0 ) {
            pkUseItem->m_nCount = kPacket_.m_kCoinItem.m_nCount;
            pkUseItem->m_nInitCount = kPacket_.m_kCoinItem.m_nCount;
            kPacket_.m_vecItem.push_back( *pkUseItem );
        }

        if ( pkUseItem && pkUseItem->m_nCount <= 0 ) {
            m_kInventory.RemoveItem( kPacket_.m_kCoinItem.m_ItemID, kPacket_.m_kCoinItem.m_ItemUID );
        }
    }

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_PLANT_TREE_00,    kPacket_.m_nOK, 1 );     // 나무나무 이벤트 기간이 아님.
        ERR_CASE( ERR_PLANT_TREE_02,    kPacket_.m_nOK, 2 );     // 해당 아이템 인벤토리에 없음.
        ERR_CASE( ERR_PLANT_TREE_03,    kPacket_.m_nOK, 3 );     // 나무를 심을수 없는 상태.
        ERR_CASE( ERR_PLANT_TREE_04,    kPacket_.m_nOK, 4 );     // 잘못된 ItemID.
        ERR_CASE( ERR_PLANT_TREE_05,    kPacket_.m_nOK, 5 );     // 잘못된 대륙 타입.
        ERR_CASE( ERR_PLANT_TREE_06,    kPacket_.m_nOK, 6 );     // 물을 줄수 없는 상태.
        ERR_CASE( ERR_PLANT_TREE_07,    kPacket_.m_nOK, 7 );     // 거름을 줄수 없는 상태.
        ERR_CASE( ERR_PLANT_TREE_08,    kPacket_.m_nOK, 8 );     // 로터리에 등록되어 있지 않음.
        ERR_CASE( ERR_PLANT_TREE_09,    kPacket_.m_nOK, 9 );     // 정의되지 않은 행동 Type.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"나무나무 이벤트 유저 행동중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 1, L"나무나무 유저 데이터 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwActionType )
        << BUILD_LOG( kPacket_.m_dwItemID )
        << BUILD_LOG( kPacket_.m_dwContinentType )
        << BUILD_LOG( kPacket_.m_dwCurrentPosition )
        << BUILD_LOG( kPacket_.m_dwTreeStatus )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_PLANT_TREE_ACTION_ACK );
}

_IMPL_ON_FUNC( EVENT_PLANT_TREE_REWARD_REQ, DWORD )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL ));
    KEVENT_PLANT_TREE_INFO_ACK kPacket;
    KDB_EVENT_PLANT_TREE_REWARD_REQ kDBPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    // 나무나무 이벤트 기간인가?
    if ( !SiKPlantTree()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_PLANT_TREE_00, END_PROC );
    }

    // 보상을 받을수 있는지 체크.
    if( !m_kPlantTreeInfo.CheckUserStatus( kPacket_ ) ) {
        SET_ERR_GOTO( ERR_PLANT_TREE_10, END_PROC );
    }

    // 보상정보 가져오기.
    if( !m_kPlantTreeInfo.GetPlantTreeReward( kPacket_, kDBPacket.m_vecGoalReward ) ) {
        SET_ERR_GOTO( ERR_PLANT_TREE_11, END_PROC );
    }

    kDBPacket.m_dwRewardType = kPacket_;

    // 보상 획득후의 유저 나무나무 말판정보 갱신.
    m_kPlantTreeInfo.ApplyRewardPosition( kPacket_ );

    // 변경된 계정정보 가져오기.
    m_kPlantTreeInfo.GetPlantTreeUserInfo( kDBPacket.m_dwContinentType, kDBPacket.m_dwCurrentPosition, kDBPacket.m_dwTreeStatus );

    SET_ERROR( NET_OK );
    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_PLANT_TREE_REWARD_REQ, kDBPacket );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_PLANT_TREE_00,    kPacket.m_nOK, 1 );     // 나무나무 이벤트 기간이 아님.
        ERR_CASE( ERR_PLANT_TREE_10,    kPacket.m_nOK, 2 );     // 보상을 받을수 있는 나무상태가 아님.
        ERR_CASE( ERR_PLANT_TREE_11,    kPacket.m_nOK, 3 );     // 보상정보 가져오기 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"나무나무 보상 정보 요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 , L"나무나무 보상 정보 요청 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_PLANT_TREE_REWARD_ACK );
    }
}

IMPL_ON_FUNC( EVENT_PLANT_TREE_REWARD_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }
    // 유저 인벤토리 갱신.
    m_kInventory.AddItemList( kPacket_.m_vecRewardItem );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_PLANT_TREE_00,    kPacket_.m_nOK, 1 );     // 나무나무 이벤트 기간이 아님.
        ERR_CASE( ERR_PLANT_TREE_10,    kPacket_.m_nOK, 2 );     // 보상을 받을수 있는 나무상태가 아님.
        ERR_CASE( ERR_PLANT_TREE_11,    kPacket_.m_nOK, 3 );     // 보상정보 가져오기 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"나무나무 보상 정보 요청중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 1, L"나무나무 유저 데이터 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwContinentType )
        << BUILD_LOG( kPacket_.m_dwCurrentPosition )
        << BUILD_LOG( kPacket_.m_dwTreeStatus ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_PLANT_TREE_REWARD_ACK );
}


IMPL_ON_FUNC_NOPARAM( EVENT_GUILD_STORE_CATALOG_REQ )
{
    std::map< int, std::vector<KManufactureItem> > mapItemList;
    SiKGuildManager()->m_kGuildStoreItems.GetCatalogs( mapItemList );

    std::vector<int> vecTabIDs;
    SiKGuildManager()->m_kGuildStoreItems.GetTabIDs( vecTabIDs );

    // 길드레벨에 따른 탭 이용 제한
    KGuildPtr spGuild = SiKGuildManager()->GetGuild( m_kGuildUserInfo.m_dwGuildUID );
    if ( spGuild == NULL ) {
        mapItemList.clear();
        START_LOG( clog, L"길드정보가 없는데, 길드상점 정보를 요청함. Login : " << GetName() ) << END_LOG;
    }
    else {
        // 길드레벨 확인
        KNGuildInfo kGuild = spGuild->GetNGuildInfo();
        USHORT usGuildLv = SiKGuildManager()->GetGuildLv( kGuild.m_dwPoint );

        std::vector<int>::iterator vit;
        for ( vit = vecTabIDs.begin() ; vit != vecTabIDs.end() ; ++vit ) {
            // 길드레벨조건이 안됨.

            if ( false == SiKGuildManager()->CheckStoreTabByGuildLv( usGuildLv, *vit ) ) {
                mapItemList.erase( *vit );

                START_LOG( clog, L"이용할 수 없는 길드상점 탭. Login : " << GetName() )
                    << BUILD_LOG( kGuild.m_dwUID )
                    << BUILD_LOG( usGuildLv )
                    << BUILD_LOG( *vit ) << END_LOG;
            }
        }
    }

    SEND_COMPRESS_PACKET( EVENT_GUILD_STORE_CATALOG_ACK, mapItemList );

    START_LOG( clog, L"길드상점 아이템 리스트 전달. Login : " << GetName() )
        << BUILD_LOG( mapItemList.size() )
        << BUILD_LOG( vecTabIDs.size() )
        << BUILD_LOG( m_kGuildUserInfo.m_dwGuildUID ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_BUY_FOR_GUILD_REQ )
{
    // 길드상점 아이템구매 요청
    KGuildPtr spGuild;
    KItemPtr pkItem;
    bool bLookItem = false;
    USHORT usGuildLv = 0;
    USHORT usUserContributeGuildLv = 0;
    PAIR_USHORT_DWORD prCatalogKey( kPacket_.m_usIndex, kPacket_.m_ItemID );
    KNGuildInfo kGuild;

    KDB_EVENT_BUY_FOR_GUILD_REQ kDBPacket;
    kDBPacket.m_nTabID = kPacket_.m_nTabID;
    kDBPacket.m_kCatalog.m_Index = kPacket_.m_usIndex;
    kDBPacket.m_kCatalog.m_ItemID = kPacket_.m_ItemID;

    KEVENT_BUY_FOR_GUILD_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nGP = GetGP();

    SET_ERROR( ERR_UNKNOWN );

    // 유저의 길드 정보가 없음.
    if ( m_kGuildUserInfo.m_dwGuildUID == 0 ) {
        SET_ERR_GOTO( ERR_GUILD_STORE_00, END_PROC );
    }

    // 유저의 길드 정보가 없음
    spGuild = SiKGuildManager()->GetGuild( m_kGuildUserInfo.m_dwGuildUID );
    if ( spGuild == NULL ) {
        SET_ERR_GOTO( ERR_GUILD_STORE_00, END_PROC );
    }
    // 길드레벨 확인
    kGuild = spGuild->GetNGuildInfo();
    usGuildLv = SiKGuildManager()->GetGuildLv( kGuild.m_dwPoint );

    // 길드레벨조건이 안됨. 없는 구매탭 정보
    if ( false == SiKGuildManager()->CheckStoreTabByGuildLv( usGuildLv, kPacket_.m_nTabID ) ) {
        SET_ERR_GOTO( ERR_GUILD_STORE_06, END_PROC );
    }

    // 유저기여도에 따른 길드 혜택레벨
    usUserContributeGuildLv = SiKGuildManager()->GetUserContributeGuildLv( m_kGuildUserInfo.m_dwContributePoint );

    // 개인기여도가 부족함.
    if ( false == SiKGuildManager()->CheckStoreTabByGuildLv( usUserContributeGuildLv, kPacket_.m_nTabID ) ) {
        SET_ERR_GOTO( ERR_GUILD_STORE_07, END_PROC );
    }

    // 구매 아이템 정보가 없음
    if ( false == SiKGuildManager()->m_kGuildStoreItems.GetCatalog( prCatalogKey, kDBPacket.m_kCatalog ) ) {
        SET_ERR_GOTO( ERR_GUILD_STORE_01, END_PROC );
    }

    // GP가격이 설정되지 않음.
    if ( kDBPacket.m_kCatalog.m_nPrice <= 0 ) {
        SET_ERR_GOTO( ERR_GUILD_STORE_02, END_PROC );
    }

    // GP가 부족함.
    if ( GetGP() < kDBPacket.m_kCatalog.m_nPrice ) {
        SET_ERR_GOTO( ERR_GUILD_STORE_03, END_PROC );
    }

    // 충분한 코디 인벤 공간이 없음.
    bLookItem = kDBPacket.m_kCatalog.CheckItemFlag( KManufactureItem::IF_LOOK );
    if ( bLookItem ) {
        if ( m_kInventory.GetLookItemNum() >= GetTrueLookInvenCapacity() ) {
            SET_ERR_GOTO( ERR_GUILD_STORE_04, END_PROC );
        }
    }

    // 충분한 인벤 공간이 없음.
    pkItem = m_kInventory.FindDurationItemByItemID( kDBPacket.m_kCatalog.m_ItemID );
    if ( !pkItem ) {
        if (GetTrueInvenCapacity() <= m_kInventory.GetTotalEnableSize() ) {
            SET_ERR_GOTO( ERR_GUILD_STORE_05, END_PROC );
        }
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_BUY_FOR_GUILD_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,             kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_STORE_00, kPacket.m_nOK, 1 ); // 유저의 길드정보가 없음.
        ERR_CASE( ERR_GUILD_STORE_01, kPacket.m_nOK, 2 ); // 구매 아이템 정보가 없음.
        ERR_CASE( ERR_GUILD_STORE_02, kPacket.m_nOK, 3 ); // GP가격이 설정되지 않음.
        ERR_CASE( ERR_GUILD_STORE_03, kPacket.m_nOK, 4 ); // GP가 부족함.
        ERR_CASE( ERR_GUILD_STORE_04, kPacket.m_nOK, 5 ); // 충분한 코디 인벤 공간이 없음.
        ERR_CASE( ERR_GUILD_STORE_05, kPacket.m_nOK, 6 ); // 충분한 인벤 공간이 없음.
        ERR_CASE( ERR_GUILD_STORE_06, kPacket.m_nOK, 7 ); // 길드레벨조건이 안됨.
        ERR_CASE( ERR_GUILD_STORE_07, kPacket.m_nOK, 8 ); // 개인기여도가 부족함.
        ERR_CASE( ERR_GUILD_STORE_08, kPacket.m_nOK, 9 ); // 일일구매제한.
        ERR_CASE( ERR_GUILD_STORE_09, kPacket.m_nOK, 10 ); // 없는 구매탭 정보
        ERR_CASE( ERR_GUILD_STORE_10, kPacket.m_nOK, 11 ); // DB처리 실패.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 || NetError::GetLastNetError() == NetError::ERR_GUILD_STORE_07, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( bLookItem )
        << BUILD_LOG( kDBPacket.m_nTabID )
        << BUILD_LOG( kDBPacket.m_kCatalog.m_Index )
        << BUILD_LOG( kDBPacket.m_kCatalog.m_ItemID )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nGP )
        << BUILD_LOG( usGuildLv )
        << BUILD_LOG( usUserContributeGuildLv )
        << BUILD_LOG( kPacket_.m_usIndex )
        << BUILD_LOG( kPacket_.m_ItemID ) << END_LOG;

    if ( false == IS_CORRECT( NET_OK ) ) {
        SEND_PACKET( EVENT_BUY_FOR_GUILD_ACK );
    }
}

IMPL_ON_FUNC( EVENT_BUY_FOR_GUILD_ACK )
{
    std::vector< GCITEMUID >::iterator vit;
    NetError::SetLastNetError( kPacket_.m_nOK );

    kPacket_.m_nGP = GetGP();

    if ( !IS_CORRECT( NET_OK ) && !IS_CORRECT( ERR_GUILD_STORE_11 ) ) {
        goto END_PROC;
    }

    if ( !kPacket_.m_vecRewards.empty() ) {
        // 획득 아이템 넣기
        m_kInventory.AddItemList( kPacket_.m_vecRewards );

        // GP차감
        KManufactureItem kCatalog;
        if ( SiKGuildManager()->m_kGuildStoreItems.GetCatalog( kPacket_.m_prKey, kCatalog ) ) {
            bool bDBUpdate = false; // DB에서 GP 갱신 처리 하지 않았음.
            DecreaseGP( KGpStatInfo::GDT_GUILD_STORE_BUY, bDBUpdate, kCatalog.m_nPrice );
            kPacket_.m_nGP = GetGP();
        }
    }

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,             kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GUILD_STORE_00, kPacket_.m_nOK, 1 ); // 유저의 길드정보가 없음.
        ERR_CASE( ERR_GUILD_STORE_01, kPacket_.m_nOK, 2 ); // 구매 아이템 정보가 없음.
        ERR_CASE( ERR_GUILD_STORE_02, kPacket_.m_nOK, 3 ); // GP가격이 설정되지 않음.
        ERR_CASE( ERR_GUILD_STORE_03, kPacket_.m_nOK, 4 ); // GP가 부족함.
        ERR_CASE( ERR_GUILD_STORE_04, kPacket_.m_nOK, 5 ); // 충분한 코디 인벤 공간이 없음.
        ERR_CASE( ERR_GUILD_STORE_05, kPacket_.m_nOK, 6 ); // 충분한 인벤 공간이 없음.
        ERR_CASE( ERR_GUILD_STORE_06, kPacket_.m_nOK, 7 ); // 길드레벨조건이 안됨.
        ERR_CASE( ERR_GUILD_STORE_07, kPacket_.m_nOK, 8 ); // 개인기여도가 부족함.
        ERR_CASE( ERR_GUILD_STORE_08, kPacket_.m_nOK, 9 ); // 일일구매제한.
        ERR_CASE( ERR_GUILD_STORE_09, kPacket_.m_nOK, 10 ); // 없는 구매탭 정보
        ERR_CASE( ERR_GUILD_STORE_10, kPacket_.m_nOK, 11 ); // DB처리 실패.
        ERR_CASE( ERR_GUILD_STORE_11, kPacket_.m_nOK, 12 ); // 룩아이템 변환 실패

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 아직 처리중인 작업
        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 || NetError::GetLastNetError() == NetError::ERR_GUILD_STORE_08, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nGP )
        << BUILD_LOG( kPacket_.m_prKey.first )
        << BUILD_LOG( kPacket_.m_prKey.second )
        << BUILD_LOG( kPacket_.m_vecRewards.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_BUY_FOR_GUILD_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_CHOICE_BOX_LIST_REQ )
{
    KEVENT_CHOICE_BOX_LIST_ACK kPacket;
    SiKChoiceDrop()->GetBoxList( kPacket );
    SEND_PACKET( EVENT_CHOICE_BOX_LIST_ACK );
    START_LOG( clog, L"초이스 드랍박스 리스트 요청. size : " << kPacket.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_CHOICE_BOX_INFO_REQ, GCITEMID )
{
    const GCITEMID& BoxItemID = kPacket_;
    KChoiceBox kPacket;

    if ( false == SiKChoiceDrop()->GetBoxInfo( BoxItemID, kPacket ) ) {
        START_LOG( cwarn, L"없는 ChoiceDropBox ItemID : " << BoxItemID ) << END_LOG;
    }

    SEND_PACKET( EVENT_CHOICE_BOX_INFO_ACK );

    START_LOG( clog, L"초이스 드랍박스 정보 요청.. Login : " << GetName() )
        << BUILD_LOG( kPacket.m_BoxItemID )
        << BUILD_LOG( kPacket.m_usDropCount )
        << BUILD_LOG( kPacket.m_mapMaterial.size() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_CHOICE_BOX_OPEN_REQ )
{
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));

    KDB_EVENT_CHOICE_BOX_OPEN_REQ kDBPacket;
    kDBPacket.m_BoxItemID = kPacket_.m_BoxItemID;
    kDBPacket.m_vecReward.clear();
    KEVENT_CHOICE_BOX_OPEN_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_kBoxItem.m_ItemID = kPacket_.m_BoxItemID;

    std::set<USHORT>::const_iterator csit;
    KChoiceBox kBoxInfo;
    KItemPtr pkItem;

    USHORT usNormalItemCount = 0; // 보상 일반 아이템 가지수
    USHORT usLookItemCount = 0; // 보상  룩 아이템 가지수
    USHORT usLookInvenFreeSize = GetCharLookInvenCapacity() - m_kInventory.GetLookItemNum(); // 사용가능한 룩인벤 크기
    USHORT usNormalInvenFreeSize = m_kInventory.GetTotalEnableSize() - GetTrueInvenCapacity(); // 사용가능한 일반인벤 크기

    SET_ERROR( ERR_UNKNOWN );

    // 해당 박스 정보가 없다.
    if ( false == SiKChoiceDrop()->GetBoxInfo( kPacket_.m_BoxItemID, kBoxInfo ) ) {
        SET_ERR_GOTO( ERR_CHOICE_DROP_00, END_PROC );
    }

    // 선택한 보상이 없다.
    if ( kPacket_.m_setSelectMaterialID.empty() ) {
        SET_ERR_GOTO( ERR_CHOICE_DROP_01, END_PROC );
    }

    // 선택한 보상개수가 전체 보상개수보다 많다.
    if ( kPacket_.m_setSelectMaterialID.size() > kBoxInfo.m_mapMaterial.size() ) {
        SET_ERR_GOTO( ERR_CHOICE_DROP_02, END_PROC );
    }

    // 선택한 보상개수가 박스의 보상가능 개수와 다르다.
    if ( kPacket_.m_setSelectMaterialID.size() != kBoxInfo.m_usDropCount ) {
        SET_ERR_GOTO( ERR_CHOICE_DROP_03, END_PROC );
    }

    // 인벤토리에서 해당 박스를 찾을 수 없다.
    pkItem = m_kInventory.FindItemByItemID( kPacket_.m_BoxItemID );
    if ( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_CHOICE_DROP_04, END_PROC );
    }

    if ( pkItem->m_nCount <= 0 ) {
        SET_ERR_GOTO( ERR_CHOICE_DROP_04, END_PROC );
    }

    // 선택한 보상이 보상 리스트에 없다.
    for ( csit = kPacket_.m_setSelectMaterialID.begin() ; csit != kPacket_.m_setSelectMaterialID.end() ; ++csit ) {
        const USHORT& usIndex = *csit;
        std::map<USHORT, KDropItemInfo>:: const_iterator cmit;
        cmit = kBoxInfo.m_mapMaterial.find( usIndex );
        if ( cmit == kBoxInfo.m_mapMaterial.end() ) {
            SET_ERR_GOTO( ERR_CHOICE_DROP_05, END_PROC );
        }

        // 보상이 코디인지 여부 확인.
        if ( SiKLookEquipment()->IsLookItemID( cmit->second.m_ItemID ) ) {
            ++usLookItemCount;
        }
        else {
            ++usNormalItemCount;
        }

        kDBPacket.m_vecReward.push_back( cmit->second );
    }

    // 룩인벤토리가 부족함.
    if ( usLookItemCount > usLookInvenFreeSize ) {
        SET_ERR_GOTO( ERR_CHOICE_DROP_06, END_PROC );
    }

    // 일반인벤토리가 부족함.
    if ( usNormalItemCount > usNormalInvenFreeSize ) {
        SET_ERR_GOTO( ERR_CHOICE_DROP_07, END_PROC );
    }

    // 지급할 아이템 리스트가 없음.
    if ( kDBPacket.m_vecReward.empty() ) {
        SET_ERR_GOTO( ERR_CHOICE_DROP_08, END_PROC );
    }

    kDBPacket.m_kBoxItem = *pkItem;

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_CHOICE_BOX_OPEN_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );   // 성공
        ERR_CASE( ERR_CHOICE_DROP_00,   kPacket.m_nOK, 1 );   // 해당 박스 정보가 없다.
        ERR_CASE( ERR_CHOICE_DROP_01,   kPacket.m_nOK, 2 );   // 선택한 보상이 없다.
        ERR_CASE( ERR_CHOICE_DROP_02,   kPacket.m_nOK, 3 );   // 선택한 보상개수가 전체 보상개수보다 많다.
        ERR_CASE( ERR_CHOICE_DROP_03,   kPacket.m_nOK, 4 );   // 선택한 보상개수가 박스의 보상가능 개수와 다르다.
        ERR_CASE( ERR_CHOICE_DROP_04,   kPacket.m_nOK, 5 );   // 인벤토리에서 해당 박스를 찾을 수 없다.
        ERR_CASE( ERR_CHOICE_DROP_05,   kPacket.m_nOK, 6 );   // 선택한 보상이 보상 리스트에 없다.
        ERR_CASE( ERR_CHOICE_DROP_06,   kPacket.m_nOK, 7 );   // 룩인벤토리가 부족함.
        ERR_CASE( ERR_CHOICE_DROP_07,   kPacket.m_nOK, 8 );   // 일반인벤토리가 부족함.
        ERR_CASE( ERR_CHOICE_DROP_08,   kPacket.m_nOK, 9 );   // 지급할 아이템 리스트가 없음.
        ERR_CASE( ERR_CHOICE_DROP_09,   kPacket.m_nOK, 10 );  // DB 아이템 지급 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업.
    default:
        START_LOG( cerr, L"초이스 드랍박스 열기 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 , L"초이스 드랍박스 열기 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( usNormalItemCount )
        << BUILD_LOG( usLookItemCount )
        << BUILD_LOG( usLookInvenFreeSize )
        << BUILD_LOG( usNormalInvenFreeSize )
        << BUILD_LOG( kBoxInfo.m_BoxItemID )
        << BUILD_LOG( kBoxInfo.m_usDropCount )
        << BUILD_LOG( kBoxInfo.m_mapMaterial.size() ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_CHOICE_BOX_OPEN_ACK );
    }
}

IMPL_ON_FUNC( EVENT_CHOICE_BOX_OPEN_ACK )
{
    // 초이스 드랍박스 보상
    KItemPtr pkItem;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) && !IS_CORRECT( ERR_CHOICE_DROP_09 ) ) {
        goto END_PROC;
    }

    pkItem = m_kInventory.FindItemByItemID( kPacket_.m_kBoxItem.m_ItemID );
    if( pkItem && pkItem->m_nCount > 0 ) { // 사용한 코인 아이템 차감.
        // 사용한 아이템 갯수 차감.
        pkItem->m_nCount = kPacket_.m_kBoxItem.m_nCount;
        pkItem->m_nInitCount = kPacket_.m_kBoxItem.m_nCount;
    }

    if ( pkItem && pkItem->m_nCount <= 0 ) {
        m_kInventory.RemoveItem( kPacket_.m_kBoxItem.m_ItemID, kPacket_.m_kBoxItem.m_ItemUID );
    }

    // 획득 아이템 넣기
    if ( false == kPacket_.m_vecItem.empty() ) {
        m_kInventory.AddItemList( kPacket_.m_vecItem );
    }

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );   // 성공
        ERR_CASE( ERR_CHOICE_DROP_00,   kPacket_.m_nOK, 1 );   // 해당 박스 정보가 없다.
        ERR_CASE( ERR_CHOICE_DROP_01,   kPacket_.m_nOK, 2 );   // 선택한 보상이 없다.
        ERR_CASE( ERR_CHOICE_DROP_02,   kPacket_.m_nOK, 3 );   // 선택한 보상개수가 전체 보상개수보다 많다.
        ERR_CASE( ERR_CHOICE_DROP_03,   kPacket_.m_nOK, 4 );   // 선택한 보상개수가 박스의 보상가능 개수와 다르다.
        ERR_CASE( ERR_CHOICE_DROP_04,   kPacket_.m_nOK, 5 );   // 인벤토리에서 해당 박스를 찾을 수 없다.
        ERR_CASE( ERR_CHOICE_DROP_05,   kPacket_.m_nOK, 6 );   // 선택한 보상이 보상 리스트에 없다.
        ERR_CASE( ERR_CHOICE_DROP_06,   kPacket_.m_nOK, 7 );   // 룩인벤토리가 부족함.
        ERR_CASE( ERR_CHOICE_DROP_07,   kPacket_.m_nOK, 8 );   // 일반인벤토리가 부족함.
        ERR_CASE( ERR_CHOICE_DROP_08,   kPacket_.m_nOK, 9 );   // 지급할 아이템 리스트가 없음.
        ERR_CASE( ERR_CHOICE_DROP_09,   kPacket_.m_nOK, 10 );  // DB 아이템 지급 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 아직 처리중인 작업.
    default:
        START_LOG( cerr, L"초이스 드랍박스 열기 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kBoxItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kBoxItem.m_nCount )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CHOICE_BOX_OPEN_ACK );
}

IMPL_ON_FUNC( EVENT_UNLOCK_CHANGE_WEAPON_ACK )
{
    KItemPtr pkItem;
    std::map<GCITEMID,int> mapUnlockItem;
    std::map<GCITEMID,int>::const_iterator cmit;
    bool bExistUnlockItem = false;
    int nNeedUnlockItemCount = 0;

    int nCharType = static_cast<int>( kPacket_.m_cCharType );

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    if ( false == SiKChangeWeaponManager()->GetUnlockItemList( mapUnlockItem ) ) {
        SET_ERR_GOTO( ERR_CHANGE_WEAPON_02, END_PROC );
    }

    for ( cmit = mapUnlockItem.begin() ; cmit != mapUnlockItem.end() ; ++cmit ) {
        pkItem = m_kInventory.FindDurationItemByItemID( cmit->first );
        if ( pkItem == NULL ) continue;
        if ( pkItem->m_nCount < cmit->second ) continue;

        nNeedUnlockItemCount = cmit->second;
        bExistUnlockItem = true;
        break;
    }

    if ( false == bExistUnlockItem || pkItem == NULL ) {
        SET_ERR_GOTO( ERR_CHANGE_WEAPON_02, END_PROC );
    }

    if ( true == m_mapChangeWeapon[nCharType].first ) {
        SET_ERR_GOTO( ERR_CHANGE_WEAPON_03, END_PROC );
    }

    m_mapChangeWeapon[nCharType].first = true;

    if ( pkItem ) {
        pkItem->m_nCount = std::max<int>( pkItem->m_nCount - nNeedUnlockItemCount, 0 );
        kPacket_.m_vecItem.push_back( *pkItem );

        START_LOG( clog, L"사용한 무기교체 언락 아이템 수량감소. LoginID : " << GetName() << L", CharType : " << nCharType )
            << BUILD_LOG( pkItem->m_ItemID )
            << BUILD_LOG( pkItem->m_ItemUID )
            << BUILD_LOG( pkItem->m_nCount )
            << BUILD_LOG( nNeedUnlockItemCount ) << END_LOG;
    }

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_CHANGE_WEAPON_00, kPacket_.m_nOK, 1 ); // 무기교체 언락할 캐릭터를 보유하지 않음.
        ERR_CASE( ERR_CHANGE_WEAPON_01, kPacket_.m_nOK, 2 ); // 무기교체 레벨 제한에 걸림.
        ERR_CASE( ERR_CHANGE_WEAPON_02, kPacket_.m_nOK, 3 ); // 무기교체 언락 아이템 수량 부족.
        ERR_CASE( ERR_CHANGE_WEAPON_03, kPacket_.m_nOK, 4 ); // 무기교체 언락할 캐릭터 슬롯이 이미 열려있음.
        ERR_CASE( ERR_CHANGE_WEAPON_04, kPacket_.m_nOK, 5 ); // 무기교체 DB기록 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );

        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_UNLOCK_CHANGE_WEAPON_ACK );
}

IMPL_ON_FUNC( EVENT_USER_AUTH_CHECK_REQ )
{
    KEVENT_USER_AUTH_CHECK_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    // 남미 미국 유저 인증 체크 이벤트 기간인가?
    if ( !SiKGCHelper()->IsUserAuthEventTerm() ) {
        SET_ERR_GOTO( ERR_USER_AUTH_00, END_PROC );
    }

    SET_ERROR( NET_OK );
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_USER_AUTH_CHECK_REQ, kPacket_ );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_USER_AUTH_00,     kPacket.m_nOK, 1 );     // 남미 미국 유저 인증 체크 기간이 아님.
        ERR_CASE( ERR_USER_AUTH_01,     kPacket.m_nOK, 2 );     // 대상 유저가 아님.
        ERR_CASE( ERR_USER_AUTH_02,     kPacket.m_nOK, 3 );     // 이미 지급 받았음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"남미 미국 유저 인증 체크 이벤트 유저 행동중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 , L"남미 미국 유저 인증 체크 이벤트 유저 행동  : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_USER_AUTH_CHECK_ACK );
    }
}

IMPL_ON_FUNC( EVENT_USER_AUTH_CHECK_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_USER_AUTH_00,     kPacket_.m_nOK, 1 );     // 남미 미국 유저 인증 체크 기간이 아님.
        ERR_CASE( ERR_USER_AUTH_01,     kPacket_.m_nOK, 2 );     // 대상 유저가 아님.
        ERR_CASE( ERR_USER_AUTH_02,     kPacket_.m_nOK, 3 );     // 이미 지급 받았음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"남미 미국 유저 인증 체크 이벤트 유저 행동중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 1, L"남미 미국 유저 인증 체크 데이터 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kVirtualCashInfo.m_dwCashPoint )
        << BUILD_LOG( kPacket_.m_vecRewardItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_USER_AUTH_CHECK_ACK );
}

_IMPL_ON_FUNC( EVENT_ITEM_LEVEL_LIST_REQ, GCITEMUID )
{
    KEVENT_ITEM_LEVEL_LIST_ACK kPacket;
    kPacket.Clear();

    KItemPtr pkInvenItem = m_kInventory.FindItemByItemUID( kPacket_ );
    std::set<int> setItemLevelList;

    if( pkInvenItem == NULL ) {
        SET_ERR_GOTO( ERR_ITEMLEVEL_SELECT_00, END_PROC );
    }

    int nItemLevelTableID = SiKItemManager()->GetLevelTableID( pkInvenItem->m_ItemID );
    if( !SiKItemManager()->m_kItemLevelDecider.GetItemLevelList( nItemLevelTableID, setItemLevelList ) ) {
        SET_ERR_GOTO( ERR_ITEMLEVEL_SELECT_01, END_PROC );
    }

    kPacket.m_ItemID = pkInvenItem->m_ItemID;
    kPacket.m_setItemLevelList.swap( setItemLevelList );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_ITEMLEVEL_SELECT_00, kPacket.m_nOK, 1 );   // 레벨설정할 아이템이 인벤에 없음.
        ERR_CASE( ERR_ITEMLEVEL_SELECT_01, kPacket.m_nOK, 2 );   // 요청한 아이템의 레벨 리스트 없음.
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99; // unknown error
        break;
    }
    SEND_PACKET( EVENT_ITEM_LEVEL_LIST_ACK );
}

IMPL_ON_FUNC( EVENT_ITEM_LEVEL_SELECT_REQ )
{
    KEVENT_ITEM_LEVEL_LIST_ACK kPacket;
    kPacket.m_nOK = -99;
    KItemPtr pkInvenItem;

    std::set<int>::iterator sit;

    SET_ERROR( ERR_UNKNOWN );

    pkInvenItem = m_kInventory.FindItemByItemUID( kPacket_.m_ItemUID );
    if( !pkInvenItem  ) { // 선택한 아이템 존재하지 않음.
        SET_ERR_GOTO( ERR_ITEMLEVEL_SELECT_00, END_PROC );
    }

    int nLevelTableID = SiKItemManager()->GetLevelTableID( pkInvenItem->m_ItemID );
    if( !SiKItemManager()->m_kItemLevelDecider.IsSelectiveType( nLevelTableID ) ) {
        SET_ERR_GOTO( ERR_ITEMLEVEL_SELECT_02, END_PROC );
    }

    if( !SiKItemManager()->m_kItemLevelDecider.CheckItemLevel( nLevelTableID, kPacket_.m_nLevel ) ) {
        SET_ERR_GOTO( ERR_ITEMLEVEL_SELECT_03, END_PROC );
    }

    kPacket_.m_kItem = *pkInvenItem;

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_ITEM_LEVEL_SELECT_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_ITEMLEVEL_SELECT_00, kPacket.m_nOK, 1 );   // 레벨설정할 아이템이 인벤에 없음.
        ERR_CASE( ERR_ITEMLEVEL_SELECT_02, kPacket.m_nOK, 2 );   // 레벨선택 가능한 아이템이 아님.
        ERR_CASE( ERR_ITEMLEVEL_SELECT_03, kPacket.m_nOK, 3 );   // 선택한 레벨이 리스트에 없음.
        ERR_CASE( ERR_ITEMLEVEL_SELECT_04, kPacket.m_nOK, 4 );   // 레벨을 DB에 기록하는데 오류 발생.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99; // unknown error
        break;
    }

    if ( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_ITEM_LEVEL_SELECT_ACK );

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"아이템 레벨 설정 요청 MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_nLevel ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_ITEM_LEVEL_SELECT_ACK )
{
    KItemPtr pkInvenItem;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    pkInvenItem = m_kInventory.FindItemByItemUID( kPacket_.m_kItem.m_ItemUID );
    if( !pkInvenItem ) {
        SET_ERR_GOTO( ERR_ITEMLEVEL_SELECT_00, END_PROC );
    }

    // 설정된 레벨값 저장.
    pkInvenItem->m_sEquipLevel = kPacket_.m_kItem.m_sEquipLevel;
    kPacket_.m_kItem = *pkInvenItem;

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_ITEMLEVEL_SELECT_00, kPacket_.m_nOK, 1 );   // 레벨설정할 아이템이 인벤에 없음.
        ERR_CASE( ERR_ITEMLEVEL_SELECT_02, kPacket_.m_nOK, 2 );   // 레벨선택 가능한 아이템이 아님.
        ERR_CASE( ERR_ITEMLEVEL_SELECT_03, kPacket_.m_nOK, 3 );   // 선택한 레벨이 리스트에 없음.
        ERR_CASE( ERR_ITEMLEVEL_SELECT_04, kPacket_.m_nOK, 4 );   // 레벨을 DB에 기록하는데 오류 발생.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99; // unknown error
        break;
    }

    SEND_RECEIVED_PACKET( EVENT_ITEM_LEVEL_SELECT_ACK );
    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"아이템 레벨 설정 요청 MSG :" << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName)
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kItem.m_sEquipLevel ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_ITEM_ATTRIBUTE_TABLE_ID_REQ, GCITEMUID )
{
    GCITEMID ItemID = 0;
    int nItemLevel = 0;
    int nAttributeTableID = -1;
    int nItemType = -1;
    int nLevelTableID = -1;

    KItemPtr pkInvenItem;
    pkInvenItem = m_kInventory.FindItemByItemUID( kPacket_ );
    if( pkInvenItem == NULL ) {
        START_LOG( cwarn, L"보유하지 않은 아이템의 속성정보를 요청함.. Login : " << GetName() << L",ItemUID : " << kPacket_ ) << END_LOG;
        goto END_PROC;
    }

    ItemID = pkInvenItem->m_ItemID;
    nItemLevel = static_cast<int>( pkInvenItem->m_sEquipLevel );

    nAttributeTableID = SiKItemManager()->GetAttributeTableID( pkInvenItem->m_ItemID );
    nLevelTableID = SiKItemManager()->GetLevelTableID( pkInvenItem->m_ItemID );

    nItemType = SiKItemManager()->m_kItemLevelDecider.GetTableType( nLevelTableID );
    SiKItemManager()->m_kItemLevelDecider.CheckItemLevelAttribute( nLevelTableID, nAttributeTableID, nItemLevel, nItemType );

END_PROC:
    START_LOG( clog, L"아이템 정보.. Login : " << GetName() )
        << BUILD_LOG( ItemID )
        << BUILD_LOG( nItemLevel )
        << BUILD_LOG( nAttributeTableID ) << END_LOG;

    SEND_DATA( EVENT_ITEM_ATTRIBUTE_TABLE_ID_ACK, nAttributeTableID );
}

IMPL_ON_FUNC_NOPARAM( EVENT_VIPEVENT_LIST_REQ )
{
    KEVENT_VIPEVENT_LIST_ACK kPacket;
    SiKVipEventManager()->GetEventList( kPacket.m_mapEventByVer );
    __LIF( SiKVipEventManager()->GetRewardItemByType( KVipEventManager::NEWBIE_REWARD, kPacket.m_mapNewbieRewardItem ) );
    LIF( SiKVipEventManager()->GetRewardItemByType( KVipEventManager::VIP_REWARD, kPacket.m_mapVipRewardItem ) );
    SiKVipEventManager()->GetPayTable( kPacket.m_mapPayTable );
    SiKVipEventManager()->GetRewardByVer( kPacket.m_mapRewardByVer );

    SEND_PACKET( EVENT_VIPEVENT_LIST_ACK );

    START_LOG( clog, L"VIP 이벤트 리스트 정보 요청.. Login : " << GetName() )
        << BUILD_LOG( kPacket.m_mapEventByVer.size() )
        << BUILD_LOG( kPacket.m_mapNewbieRewardItem.size() )
        << BUILD_LOG( kPacket.m_mapVipRewardItem.size() )
        << BUILD_LOG( kPacket.m_mapPayTable.size() )
        << BUILD_LOG( kPacket.m_mapRewardByVer.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_VIPEVENT_USER_INFO_REQ, USHORT )
{
    QUEUEING_EVENT_TO_DB( EVENT_VIPEVENT_USER_INFO_REQ, kPacket_ );
    START_LOG( clog, L"유저 VIP이벤트 정보 요청. Login : " << GetName() << L", ver : " << kPacket_ ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_VIPEVENT_USER_INFO_ACK )
{
    START_LOG( clog, L"VIP Event UserInfo.. Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_usVersion )
        << BUILD_LOG( kPacket_.m_nPoint )
        << BUILD_LOG( kPacket_.m_mapReward.size() ) << END_LOG;

    //std::map<int,bool>::const_iterator cmit;
    //for ( cmit = kPacket_.m_mapReward.begin() ; cmit != kPacket_.m_mapReward.end() ; ++cmit ) {
    //    std::Info << L"PayTableID : " << cmit->first
    //        << L", Complete : " << ( cmit->second ? L"True" : L"False" ) << dbg::endl;
    //}

    SEND_RECEIVED_PACKET( EVENT_VIPEVENT_USER_INFO_ACK );
}

IMPL_ON_FUNC( EVENT_VIPEVENT_USER_REWARD_UPDATE_NOT )
{
    START_LOG( clog, L"VIP-event 지급받은 아이템 리스트 전달. Login : " << GetName() )
        << BUILD_LOG( kPacket_.size() ) << END_LOG;

    JIF( !kPacket_.empty() );

    // 획득 아이템 지급
    m_kInventory.AddItemList( kPacket_ );
    SEND_RECEIVED_PACKET( EVENT_VIPEVENT_USER_REWARD_UPDATE_NOT );
}

IMPL_ON_FUNC_NOPARAM( EVENT_MAGIC_BOX_LIST_REQ )
{
    if ( false == SiKMagicBox()->IsEventTerm() ) {
        return;
    }

    QUEUEING_ID_TO_DB( EVENT_MAGIC_BOX_LIST_REQ );
    START_LOG( clog, L"유저 두근두근 마법상자 정보 요청. Login : " << GetName() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_MAGIC_BOX_LIST_ACK )
{
    START_LOG( clog, L"두근두근 마법상자.. 유저 아이템 리스트 전달  Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_selectItemID )
        << BUILD_LOG( kPacket_.m_mapItemInfo.size() ) << END_LOG;

    // 유저 정보 설정
    m_mapMagicBoxInfo = kPacket_.m_mapItemInfo;

    SEND_RECEIVED_PACKET( EVENT_MAGIC_BOX_LIST_ACK );
}

_IMPL_ON_FUNC( EVENT_SET_MAGIC_BOX_TARGET_REQ, GCITEMID )
{
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_SET_MAGIC_BOX_TARGET_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_ItemID = kPacket_;

    // 이벤트 기간인지?
    if ( false == SiKMagicBox()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_MAGIC_BOX_00, END_PROC );
    }

    // 해당 아이템이 아이템 리스트 중에 있는지?
    if ( false == SiKMagicBox()->IsExistItem( kPacket_ ) ) {
        SET_ERR_GOTO( ERR_MAGIC_BOX_01, END_PROC );
    }

    SET_ERROR( NET_OK );
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_SET_MAGIC_BOX_TARGET_REQ, kPacket_ );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );   // 성공
        ERR_CASE( ERR_MAGIC_BOX_00,     kPacket.m_nOK, 1 );   // 이벤트 기간이 아니다.
        ERR_CASE( ERR_MAGIC_BOX_01,     kPacket.m_nOK, 2 );   // 해당 아이템이 아이템 리스트에 없음
        ERR_CASE( ERR_MAGIC_BOX_02,     kPacket.m_nOK, 3 );   // DB처리 실패
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"두근두근 마법상자.. 타겟 아이템 지정 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 , L"두근두근 마법상자.. 타겟 아이템 지정 요청: " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_ItemID ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_SET_MAGIC_BOX_TARGET_ACK );
    }
}

IMPL_ON_FUNC( EVENT_SET_MAGIC_BOX_TARGET_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );   // 성공
        ERR_CASE( ERR_MAGIC_BOX_00,     kPacket_.m_nOK, 1 );   // 이벤트 기간이 아니다.
        ERR_CASE( ERR_MAGIC_BOX_01,     kPacket_.m_nOK, 2 );   // 해당 아이템이 아이템 리스트에 없음
        ERR_CASE( ERR_MAGIC_BOX_02,     kPacket_.m_nOK, 3 );   // DB처리 실패
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"두근두근 마법상자.. 타겟 아이템 지정 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"두근두근 마법상자.. 타겟 아이템 지정 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_ItemID ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SET_MAGIC_BOX_TARGET_ACK );
}

IMPL_ON_FUNC( EVENT_USE_MAGIC_BOX_REQ )
{
    SET_ERROR( ERR_UNKNOWN );

    GCITEMID normalKeyItemID = SiKMagicBox()->GetNormalKeyItemID();
    GCITEMID specialKeyItemID = SiKMagicBox()->GetSpecialKeyItemID();

    KItemPtr pkItem;
    std::map<GCITEMID, std::pair<int,int> >::iterator mit;

    kPacket_.m_bIsSpecialKey = false;
    kPacket_.m_nLeftKeyCount = -1;
    kPacket_.m_nClearCount = -1;

    KEVENT_USE_MAGIC_BOX_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nLeftKeyCount = -1;

    KDB_EVENT_USE_MAGIC_BOX_REQ kDBPacket;

    // 이벤트 기간인지?
    if ( false == SiKMagicBox()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_MAGIC_BOX_00, END_PROC );
    }

    // 해당 아이템이 아이템 리스트 중에 있는지?
    if ( false == SiKMagicBox()->IsExistItem( kPacket_.m_TargetItemID ) ) {
        SET_ERR_GOTO( ERR_MAGIC_BOX_01, END_PROC );
    }

    // 키 아이템이 맞는가?
    if ( kPacket_.m_KeyItemID != normalKeyItemID && kPacket_.m_KeyItemID != specialKeyItemID ) {
        SET_ERR_GOTO( ERR_MAGIC_BOX_03, END_PROC );
    }

    // 스페셜 키인가?
    if ( kPacket_.m_KeyItemID == specialKeyItemID ) {
        kPacket_.m_bIsSpecialKey = true;
    }

    // 사용하려는 키 아이템을 보유하고 있는지?
    pkItem = m_kInventory.FindItemByItemID( kPacket_.m_KeyItemID );
    if ( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_MAGIC_BOX_04, END_PROC );
    }

    kPacket.m_kKeyItem = *pkItem;
    kDBPacket.m_kKeyItem = *pkItem;

    // 수량은 충분한가?
    if ( pkItem->m_nCount <= 0 ) {
        SET_ERR_GOTO( ERR_MAGIC_BOX_05, END_PROC );
    }

    // 해당 아이템이 아이템 리스트 중에 없다.
    mit = m_mapMagicBoxInfo.find( kPacket_.m_TargetItemID );
    if ( mit == m_mapMagicBoxInfo.end() ) {
        SET_ERR_GOTO( ERR_MAGIC_BOX_01, END_PROC );
    }

    // 소지한도 체크
    if ( false == CheckItemCount( kPacket_.m_TargetItemID ) ) {
        SET_ERR_GOTO( ERR_MAGIC_BOX_09, END_PROC );
    }

    // 인벤토리 체크
    if ( false == CheckItemInvenLimit( kPacket_.m_TargetItemID ) ) {
        SET_ERR_GOTO( ERR_MAGIC_BOX_08, END_PROC );
    }

    kPacket_.m_nLeftKeyCount = mit->second.first; // 남은 열쇠 개수
    kPacket_.m_nClearCount = mit->second.second; // 아이템 획득 회수

    kDBPacket.m_TargetItemID = kPacket_.m_TargetItemID;
    kDBPacket.m_nLeftKeyCount = kPacket_.m_nLeftKeyCount;
    kDBPacket.m_KeyItemID = kPacket_.m_KeyItemID;
    kDBPacket.m_nClearCount = kPacket_.m_nClearCount;
    kDBPacket.m_bIsSpecialKey = kPacket_.m_bIsSpecialKey;


    QUEUEING_EVENT_TO_DB_CHECK( EVENT_USE_MAGIC_BOX_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_MAGIC_BOX_00, kPacket.m_nOK, 1 ); // 이벤트 기간이 아니다.
        ERR_CASE( ERR_MAGIC_BOX_01, kPacket.m_nOK, 2 ); // 해당 아이템이 아이템 리스트 중에 없다.
        ERR_CASE( ERR_MAGIC_BOX_02, kPacket.m_nOK, 3 ); // DB처리 실패
        ERR_CASE( ERR_MAGIC_BOX_03, kPacket.m_nOK, 4 ); // 키 아이템이 아님
        ERR_CASE( ERR_MAGIC_BOX_04, kPacket.m_nOK, 5 ); // 키 아이템이 없음
        ERR_CASE( ERR_MAGIC_BOX_05, kPacket.m_nOK, 6 ); // 키 아이템 수량부족.
        ERR_CASE( ERR_MAGIC_BOX_07, kPacket.m_nOK, 7 ); // 키 아이템 DB 동기화 실패
        ERR_CASE( ERR_MAGIC_BOX_08, kPacket.m_nOK, 8 ); // 인벤토리가 가득 찼음.
        ERR_CASE( ERR_MAGIC_BOX_09, kPacket.m_nOK, 9 ); // 인벤토리 보유제한 초과

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"두근두근 마법상자.. 키 아이템 사용 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 , L"두근두근 마법상자.. 키 아이템 사용 요청: " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_TargetItemID )
        << BUILD_LOG( kPacket_.m_KeyItemID )
        << BUILD_LOG( kPacket_.m_bIsSpecialKey )
        << BUILD_LOG( kPacket_.m_nLeftKeyCount ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_USE_MAGIC_BOX_ACK );
    }
}

IMPL_ON_FUNC( EVENT_USE_MAGIC_BOX_ACK )
{
    KItemPtr pkItem;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 사용한 키 아이템 차감
    pkItem = m_kInventory.FindItemByItemID( kPacket_.m_kKeyItem.m_ItemID );
    if ( pkItem == NULL ) {
        START_LOG( cerr, L"키 아이템을 찾을 수 없음.. Login : " << GetName() )
            << BUILD_LOG( kPacket_.m_kKeyItem.m_ItemID ) << END_LOG;
        SET_ERR_GOTO( ERR_MAGIC_BOX_04, END_PROC );
    }
    else {
        pkItem->m_nCount = kPacket_.m_kKeyItem.m_nCount;
        pkItem->m_nInitCount = kPacket_.m_kKeyItem.m_nCount;

        kPacket_.m_kKeyItem = *pkItem;
    }

    if ( pkItem && pkItem->m_nCount <= 0 ) {
        m_kInventory.RemoveItem( kPacket_.m_kKeyItem.m_ItemID, kPacket_.m_kKeyItem.m_ItemUID );
    }

    // 획득 아이템 넣기
    m_kInventory.AddItemList( kPacket_.m_vecItem );

    // 마법상자 정보갱신
    m_mapMagicBoxInfo[ kPacket_.m_TargetItemID ] = std::make_pair( kPacket_.m_nLeftKeyCount, kPacket_.m_nClearCount );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,           kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_MAGIC_BOX_00, kPacket_.m_nOK, 1 ); // 이벤트 기간이 아니다.
        ERR_CASE( ERR_MAGIC_BOX_01, kPacket_.m_nOK, 2 ); // 해당 아이템이 아이템 리스트 중에 없다.
        ERR_CASE( ERR_MAGIC_BOX_02, kPacket_.m_nOK, 3 ); // DB처리 실패
        ERR_CASE( ERR_MAGIC_BOX_03, kPacket_.m_nOK, 4 ); // 키 아이템이 아님
        ERR_CASE( ERR_MAGIC_BOX_04, kPacket_.m_nOK, 5 ); // 키 아이템이 없음
        ERR_CASE( ERR_MAGIC_BOX_05, kPacket_.m_nOK, 6 ); // 키 아이템 수량부족.
        ERR_CASE( ERR_MAGIC_BOX_07, kPacket_.m_nOK, 7 ); // 키 아이템 DB 동기화 실패

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"두근두근 마법상자.. 키 아이템 사용 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"두근두근 마법상자.. 키 아이템 사용 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_TargetItemID )
        << BUILD_LOG( kPacket_.m_nLeftKeyCount )
        << BUILD_LOG( kPacket_.m_kKeyItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_nClearCount )
        << BUILD_LOG( kPacket_.m_bClear )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_USE_MAGIC_BOX_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_PACKAGE_INFO_REQ )
{
    KEVENT_PACKAGE_INFO_ACK kPacket;
    std::vector<std::map<GCITEMID,std::vector<KDropItemInfo>>> vecList;
    std::vector<std::map<GCITEMID,std::vector<KDropItemInfo>>>::iterator vit;
    SiKItemManager()->GetPackageInfo( vecList );
    int nIndex = 1;

    kPacket.m_nMaxIndex = vecList.size();
    kPacket.m_nStartIndex = nIndex;

    SEND_PACKET( EVENT_PACKAGE_INFO_ACK );

    START_LOG( clog, L"패키지아이템 인덱스 리스트 전달.. Login : " << GetName() )
        << BUILD_LOG( kPacket.m_nMaxIndex )
        << BUILD_LOG( kPacket.m_nStartIndex )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_RKTORNADO_ACTION_REQ )
{
    KEVENT_RKTORNADO_ACTION_ACK kPacket;
    kPacket.m_nOK = -99;
    KDropItemInfo kRewardItem;
    std::map<int,GCITEMID> mapNotExistList;
    KItemPtr pkCoinItem;
    KItemPtr pkLuckyCoinItem;

    SET_ERROR( ERR_UNKNOWN );

    // 릭쿤 토네이도 이벤트 기간인지 체크.
    if( !SiKRKTornado()->IsRun() ) {
        SET_ERR_GOTO( ERR_RKTORNODO_00, END_PROC );
    }

    pkCoinItem = m_kInventory.FindItemByItemID( kPacket_.m_CoinItemID );
    if( !pkCoinItem  ) { // 선택한 아이템 존재하지 않음.
        SET_ERR_GOTO( ERR_RKTORNODO_01, END_PROC );
    }
    if ( pkCoinItem->m_nCount <= 0 ) {
        SET_ERR_GOTO( ERR_RKTORNODO_01, END_PROC );
    }

    if( KRKTornado::AT_NONFAIL == kPacket_.m_dwType )  { // 럭키 Coin 사용시에 체크.
        pkLuckyCoinItem = m_kInventory.FindItemByItemID( kPacket_.m_LuckyCoinItemID );
        if( pkLuckyCoinItem ) {
            kPacket_.m_LuckyCoinItem = *pkLuckyCoinItem;
            int nCount = static_cast<int>( SiKRKTornado()->GetFailItemCount() );            

            if( pkLuckyCoinItem->m_nCount < nCount ) {
                SET_ERR_GOTO( ERR_RKTORNODO_02, END_PROC );
            }
        } else {
            SET_ERR_GOTO( ERR_RKTORNODO_02, END_PROC );
        }
    }

    // 아이템 확률로 얻기.
    if( !SiKRKTornado()->DoGamble( kPacket_.m_dwCharType, kPacket_.m_dwType, kRewardItem ) ) {
        SET_ERR_GOTO( ERR_RKTORNODO_05, END_PROC );
    }

    // 해당 아이템 Type정보 받아오기.
    SiKRKTornado()->GetItemType( kRewardItem.m_ItemID, kPacket_.m_dwRewardType );

    if( KRKTornado::IT_EQUIPMENT == kPacket_.m_dwRewardType ) { // 장비 아이템일 경우 없는 아이템 넣어주기.
        GetRKTornadoNotExistList( kPacket_.m_dwCharType, mapNotExistList );
        if( !mapNotExistList.empty() ) {
			float fRate = (float)rand() / (float)RAND_MAX;
			int nIdx = (int)( mapNotExistList.size() * fRate );
			if( nIdx >= (int)mapNotExistList.size() || nIdx < 0 ) {
				nIdx = 0; // 범위 안넘어가도록 보정(안전코드)
			}

			SiKRKTornado()->GetEquipemntItem( kPacket_.m_dwCharType, mapNotExistList[nIdx], kRewardItem );
        } else {
            if( !SiKRKTornado()->DoGambleFail( kPacket_.m_dwCharType, kRewardItem ) ) {
                SET_ERR_GOTO( ERR_RKTORNODO_05, END_PROC );
            }
            // 해당 아이템 Type정보 받아오기.
            SiKRKTornado()->GetItemType( kRewardItem.m_ItemID, kPacket_.m_dwRewardType );
        }
    }

    if( KRKTornado::IT_FAIL == kPacket_.m_dwRewardType ) { // 꽝일경우에 추가 아이템 넣어주기.
        if( !SiKRKTornado()->GetFailItemList( kPacket_.m_dwCharType, kPacket_.m_dwRewardType, kPacket_.m_vecReward ) ) {
            SET_ERR_GOTO( ERR_RKTORNODO_04, END_PROC );
        }
    }

    // 확률 아이템을 vector넣어주기.
    kPacket_.m_vecReward.push_back( kRewardItem );
    kPacket_.m_dwRewardItemID = kRewardItem.m_ItemID; // 클라이언트 Display용 GoodsID 전달.
    kPacket_.m_CoinItem = *pkCoinItem;

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_RKTORNADO_ACTION_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_RKTORNODO_00, kPacket.m_nOK, 1 );   // 릭쿤 토네이도 이벤트 기간이 아님.
        ERR_CASE( ERR_RKTORNODO_01, kPacket.m_nOK, 2 );   // 릭쿤 토네이도 코인 아이템이 없음.
        ERR_CASE( ERR_RKTORNODO_02, kPacket.m_nOK, 3 );   // 릭쿤 토네이도 럭키 세븐 아이템 수량 부족.
        ERR_CASE( ERR_RKTORNODO_03, kPacket.m_nOK, 4 );   // 시스템 오류입니다.
        ERR_CASE( ERR_RKTORNODO_04, kPacket.m_nOK, 5 );   // 꽝 보상 아이템 가져오기 실패.
        ERR_CASE( ERR_RKTORNODO_05, kPacket.m_nOK, 6 );   // 보상 아이템 가져오기 실패.
        ERR_CASE( ERR_RKTORNODO_06, kPacket.m_nOK, 7 );   // 지급할 아이템 리스트 없음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99; // unknown error
        break;
    }

    if ( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_RKTORNADO_ACTION_ACK );

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"릭쿤 토네이도 이벤트 유저 액션 MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_dwType )
        << BUILD_LOG( kPacket_.m_dwCharType )
        << BUILD_LOG( kPacket_.m_CoinItemID )
        << BUILD_LOG( kPacket_.m_LuckyCoinItemID )
        << BUILD_LOG( kPacket_.m_dwRewardType ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_RKTORNADO_ACTION_ACK )
{
    KItemPtr pkUseCoinItem;
    KItemPtr pkUseLuckyCoinItem;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) && !IS_CORRECT( ERR_RKTORNODO_07 ) && !IS_CORRECT( ERR_RKTORNODO_08 ) ) {
        goto END_PROC;
    }

    pkUseCoinItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_CoinItemID );
    if( pkUseCoinItem && pkUseCoinItem->m_nCount > 0 ) { // 사용한 코인 아이템 차감.
        // 사용한 아이템 갯수 차감.
        pkUseCoinItem->m_nCount = kPacket_.m_CoinItem.m_nCount;
        pkUseCoinItem->m_nInitCount = kPacket_.m_CoinItem.m_nCount;
        kPacket_.m_vecUseItem.push_back( *pkUseCoinItem );
    }

    if ( pkUseCoinItem && pkUseCoinItem->m_nCount <= 0 ) {
        m_kInventory.RemoveItem( kPacket_.m_CoinItem.m_ItemID, kPacket_.m_CoinItem.m_ItemUID );
    }

    if( KRKTornado::AT_NONFAIL == kPacket_.m_dwType ) {
        pkUseLuckyCoinItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_LuckyCoinItemID );
        if( pkUseLuckyCoinItem ) { // 사용한 코인 아이템 차감.
            // 사용한 아이템 갯수 차감.
            pkUseLuckyCoinItem->m_nCount = kPacket_.m_LuckyCoinItem.m_nCount;
            pkUseLuckyCoinItem->m_nInitCount = kPacket_.m_LuckyCoinItem.m_nCount;
            kPacket_.m_vecUseItem.push_back( *pkUseLuckyCoinItem );

            if ( pkUseLuckyCoinItem->m_nCount <= 0 ) {
                m_kInventory.RemoveItem( kPacket_.m_LuckyCoinItem.m_ItemID, kPacket_.m_LuckyCoinItem.m_ItemUID );
            }
        }
    }
    // 유저 인벤토리 갱신.
    m_kInventory.AddItemList( kPacket_.m_vecItem );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_RKTORNODO_00, kPacket_.m_nOK, 1 );   // 릭쿤 토네이도 이벤트 기간이 아님.
        ERR_CASE( ERR_RKTORNODO_01, kPacket_.m_nOK, 2 );   // 릭쿤 토네이도 코인 아이템이 없음.
        ERR_CASE( ERR_RKTORNODO_02, kPacket_.m_nOK, 3 );   // 릭쿤 토네이도 럭키 세븐 아이템 수량 부족.
        ERR_CASE( ERR_RKTORNODO_03, kPacket_.m_nOK, 4 );   // 시스템 오류입니다.
        ERR_CASE( ERR_RKTORNODO_04, kPacket_.m_nOK, 5 );   // 꽝 보상 아이템 가져오기 실패.
        ERR_CASE( ERR_RKTORNODO_05, kPacket_.m_nOK, 6 );   // 보상 아이템 가져오기 실패.
        ERR_CASE( ERR_RKTORNODO_06, kPacket_.m_nOK, 7 );   // 지급할 아이템 리스트 없음.
        ERR_CASE( ERR_RKTORNODO_07, kPacket_.m_nOK, 8 );   // 릭쿤 토네이도 코인 갯수 동기화 실패.
        ERR_CASE( ERR_RKTORNODO_08, kPacket_.m_nOK, 9 );   // 릭쿤 토네이도 럭키 코인 갯수 동기화 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );

    default:
        START_LOG( cerr, L"릭쿤 토네이도 이벤트 유저 행동중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 1, L"릭쿤 토네이도 유저 데이터 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwType )
        << BUILD_LOG( kPacket_.m_CoinItemID )
        << BUILD_LOG( kPacket_.m_LuckyCoinItemID )
        << BUILD_LOG( kPacket_.m_dwRewardType )
        << BUILD_LOG( kPacket_.m_vecUseItem.size() )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_RKTORNADO_ACTION_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_OPEN_CALENDAR_BONUS_POINT_REQ )
{
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_OPEN_CALENDAR_BONUS_POINT_REQ kDBPacket;
    kDBPacket.m_nGCPoint = m_kGCPoint.GetGCPoint();
    kDBPacket.m_nInitGCPoint = m_kGCPoint.GetInitGCPoint();
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    kDBPacket.m_nAddGCPoint = SiKGCPoint()->GetGCPointOpenCalendar( tmCurrent );
    kDBPacket.m_nAccTime = m_kCalendar.GetAccTime();

    KEVENT_OPEN_CALENDAR_BONUS_POINT_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nGCPoint = kDBPacket.m_nGCPoint;
    kPacket.m_nInitGCPoint = kDBPacket.m_nInitGCPoint;
    kPacket.m_nAddGCPoint = kDBPacket.m_nAddGCPoint;

    // 출석부열기 보너스포인트가 설정되어 있는가?
    if ( kDBPacket.m_nAddGCPoint <= 0 ) {
        SET_ERR_GOTO( ERR_GCPOINT_17, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_OPEN_CALENDAR_BONUS_POINT_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,         kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_GCPOINT_17, kPacket.m_nOK, 1 ); // 출석부 열기 보너스 포인트가 없음 (실패아님)
    default:
        START_LOG( cerr, L"일일 1회 달력 오픈시 보너스 출석포인트 지급 요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 || kPacket.m_nOK == 1, L"일일 1회 달력 오픈시 보너스 출석포인트 지급 요청 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nGCPoint )
        << BUILD_LOG( kPacket.m_nInitGCPoint )
        << BUILD_LOG( kPacket.m_nAddGCPoint ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_OPEN_CALENDAR_BONUS_POINT_ACK );
    }
}

IMPL_ON_FUNC( EVENT_OPEN_CALENDAR_BONUS_POINT_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 출석포인트 보너스 지급 적용.
    m_kGCPoint.SetGCPoint( kPacket_.m_nGCPoint );
    m_kGCPoint.SetInitGCPoint( kPacket_.m_nInitGCPoint );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,         kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_GCPOINT_17, kPacket_.m_nOK, 1 ); // 출석부 열기 보너스 포인트가 없음 (실패아님)
        ERR_CASE( ERR_GCPOINT_18, kPacket_.m_nOK, 2 ); // 출석부열기 보너스 오늘분 이미 지급받음 (실패아님)
        ERR_CASE( ERR_GCPOINT_19, kPacket_.m_nOK, 3 ); // 출석부열기 보너스 DB저장 실패.
    default:
        START_LOG( cerr, L"일일1회 달력 오픈시 보너스 출석포인트 지급 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 1 || kPacket_.m_nOK == 2, L"일일1회 달력 오픈시 보너스 출석포인트 지급 결과 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nAddGCPoint )
        << BUILD_LOG( kPacket_.m_nGCPoint )
        << BUILD_LOG( kPacket_.m_nInitGCPoint ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_OPEN_CALENDAR_BONUS_POINT_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_USER_SPHINX_DATA_REQ )
{
    KEVENT_USER_SPHINX_DATA_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    // 스핑크스 OX 이벤트 기간인가?
    if ( !SiKSphinx()->IsRun() ) {
        SET_ERR_GOTO( ERR_SPHINX_00, END_PROC );
    }

    if ( m_kSphinxUserData.IsLoaded() ) { // 이미 DB에서 데이터를 가져왔으면..,
        kPacket.m_nRightAnswerCnt = m_kSphinxUserData.GetRightAnswerCount();
        SET_ERR_GOTO( ERR_SPHINX_01, END_PROC );
    }

    SET_ERROR( NET_OK );
    QUEUEING_ID_TO_DB_CHECK( EVENT_USER_SPHINX_DATA_REQ );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_SPHINX_01,        kPacket.m_nOK, 1 );     // DB에서 초기 데이터 이미 가져왔음.
        ERR_CASE( ERR_SPHINX_00,        kPacket.m_nOK, 2 );     // 스핑크스 OX 이벤트 기간이 아님.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"스핑크스 OX 이벤트 정보 요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 || kPacket.m_nOK == 1, L"스핑크스 OX 이벤트 정보 요청 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_USER_SPHINX_DATA_ACK );
    }
}

IMPL_ON_FUNC( EVENT_USER_SPHINX_DATA_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    int nMaxRigthAnswerCnt = SiKSphinx()->GetTotalQuestionNum();
    m_kSphinxUserData.SetUserSphinxData( kPacket_.m_nRightAnswerCnt, kPacket_.m_vecCorrectQuestions, nMaxRigthAnswerCnt );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_SPHINX_01,        kPacket_.m_nOK, 1 );     // DB에서 초기 데이터 이미 가져왔음.
        ERR_CASE( ERR_SPHINX_00,        kPacket_.m_nOK, 2 );     // 스핑크스 OX 이벤트 기간이 아님.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"스핑크스 OX 이벤트 응답 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 1, L"스핑크스 OX 이벤트 유저 데이터 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nRightAnswerCnt ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_USER_SPHINX_DATA_ACK );
}

IMPL_ON_FUNC( EVENT_SPHINX_QUESTION_REQ )
{
    KEVENT_SPHINX_QUESTION_ACK kPacket;
    kPacket.m_nOK = 99;
    KItemPtr pkCoinItem;
    std::vector<int> vecCorrectQuestions;
    vecCorrectQuestions.clear();

    SET_ERROR( ERR_UNKNOWN );

    // 스핑크스 OX 이벤트 기간인가?
    if ( !SiKSphinx()->IsRun() ) {
        SET_ERR_GOTO( ERR_SPHINX_00, END_PROC );
    }

    // 스핑크스 코인 아이템을 보유 하고 있는가?
    GCITEMID dwSphinxItemID = 0;
    dwSphinxItemID = SiKSphinx()->GetSphinxCoinID();
    pkCoinItem = m_kInventory.FindDurationItemByItemID( dwSphinxItemID );

    if ( NULL == pkCoinItem ) {
        SET_ERR_GOTO( ERR_SPHINX_02, END_PROC );
    }

    if ( pkCoinItem->m_nCount <= 0 ) {
        SET_ERR_GOTO( ERR_SPHINX_02, END_PROC );
    }

    // 문제 가져오기.
    vecCorrectQuestions = m_kSphinxUserData.GetCorrectQuestions();
    if ( !SiKSphinx()->GetRandomQuestionID( vecCorrectQuestions, kPacket_.m_nQuestionID ) ) {
        SET_ERR_GOTO( ERR_SPHINX_03, END_PROC );
    }

    if( !SiKSphinx()->GetSphinxTimeValue( kPacket_.m_nQuestionID, kPacket_.m_nTimeValue ) ) {
        SET_ERR_GOTO( ERR_SPHINX_04, END_PROC );
    }

    kPacket_.m_kCoinItem = *pkCoinItem;

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_SPHINX_QUESTION_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_SPHINX_00,        kPacket.m_nOK, 1 );     // 스핑크스 OX 이벤트 기간이 아님.
        ERR_CASE( ERR_SPHINX_02,        kPacket.m_nOK, 2 );     // 스핑크스 OX 코인 아이템이 없음.
        ERR_CASE( ERR_SPHINX_03,        kPacket.m_nOK, 3 );     // 질문을 가져오지 못함.
        ERR_CASE( ERR_SPHINX_04,        kPacket.m_nOK, 4 );     // 시간정보 가져오지 못함.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"스핑크스 OX 문제 요청중 알수 없는 오류 ..  Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_SPHINX_QUESTION_ACK );
    }

    LOG_CONDITION( kPacket.m_nOK == 0 || kPacket.m_nOK == -98 , clog, cerr )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_nTimeValue )
        << BUILD_LOG( kPacket_.m_nQuestionID ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_SPHINX_QUESTION_ACK )
{
    KItemPtr pkUseCoinItem;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) && !IS_CORRECT( ERR_SPHINX_08 ) ) {
        goto END_PROC;
    }

    pkUseCoinItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_kCoinItem.m_ItemID );
    if( pkUseCoinItem && pkUseCoinItem->m_nCount > 0 ) { // 사용한 코인 아이템 차감.
        // 사용한 아이템 갯수 차감.
        pkUseCoinItem->m_nCount = kPacket_.m_kCoinItem.m_nCount;
        pkUseCoinItem->m_nInitCount = kPacket_.m_kCoinItem.m_nCount;
        kPacket_.m_vecUseItem.push_back( *pkUseCoinItem );
    }

    if ( pkUseCoinItem && pkUseCoinItem->m_nCount <= 0 ) {
        m_kInventory.RemoveItem( kPacket_.m_kCoinItem.m_ItemID, kPacket_.m_kCoinItem.m_ItemUID );
    }

    if ( !SiKSphinx()->GetCompressedQuestion( kPacket_.m_nQuestionID, kPacket_.m_kQuestionBuff ) ) {
        LIF( SiKSphinx()->GetRandomQuestion( m_kQuizBingo.GetCorrectQuestions(), kPacket_.m_kQuestionBuff, kPacket_.m_nQuestionID ) );
    }

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_SPHINX_00,        kPacket_.m_nOK, 1 );     // 스핑크스 OX 이벤트 기간이 아님.
        ERR_CASE( ERR_SPHINX_02,        kPacket_.m_nOK, 2 );     // 스핑크스 OX 코인 아이템이 없음.
        ERR_CASE( ERR_SPHINX_03,        kPacket_.m_nOK, 3 );     // 질문을 가져오지 못함.
        ERR_CASE( ERR_SPHINX_04,        kPacket_.m_nOK, 4 );     // 시간정보 가져오지 못함.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"스핑크스 OX 문제 요청 응답 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 1, L"스핑크스 OX 이벤트 유저 데이터 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nTimeValue )
        << BUILD_LOG( kPacket_.m_nQuestionID ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SPHINX_QUESTION_ACK );
}

IMPL_ON_FUNC( EVENT_SPHINX_ANSWER_REQ )
{
    KEVENT_SPHINX_ANSWER_ACK kPacket;
    KDB_EVENT_SPHINX_ANSWER_REQ kDBPacket;
    kPacket.m_nOK = 99;
    KItemPtr pkCoinItem;
    KSphinxQuestionInfo kQuestion;
    KDropItemInfo kRewardItem;
    SET_ERROR( ERR_UNKNOWN );

    // 스핑크스 OX 이벤트 기간인가?
    if ( !SiKSphinx()->IsRun() ) {
        SET_ERR_GOTO( ERR_SPHINX_00, END_PROC );
    }

    if ( !m_kSphinxUserData.IsLoaded() ) { // 데이터 로드 되었는지 체크.
        SET_ERR_GOTO( ERR_SPHINX_12, END_PROC );
    }

    // 존재 하는 문제 인가?
    if ( !SiKSphinx()->GetQuestion( kPacket_.m_nQuestionID, kQuestion) ) {
        SET_ERR_GOTO( ERR_SPHINX_09, END_PROC );
    }

    // 정답인가?
    if( kPacket_.m_nAnswerNum == kQuestion.m_nAnswer && kPacket_.m_nAnswerNum != KSphinx::AT_TIMEOVER ) {
        int nMaxRigthAnswerCnt = SiKSphinx()->GetTotalQuestionNum();
        m_kSphinxUserData.AddCorrectQuestion( kPacket_.m_nQuestionID, nMaxRigthAnswerCnt );
        int nRigthAnswerCnt = m_kSphinxUserData.GetRightAnswerCount();
        if( !SiKSphinx()->GetKairoLevelReward( kQuestion.m_nLevel, kRewardItem ) ) {
            SET_ERR_GOTO( ERR_SPHINX_11, END_PROC );
        }
        SiKSphinx()->CheckAccumulateReward( nRigthAnswerCnt, kDBPacket.m_vecReward );
        kDBPacket.m_bCorrectCheck = true;
    } else {
        if( !SiKSphinx()->GetKairoLevelReward( KSphinx::RT_FAIL, kRewardItem ) ) {
            SET_ERR_GOTO( ERR_SPHINX_11, END_PROC );
        }
        kDBPacket.m_bCorrectCheck = false;
    }

    kDBPacket.m_vecReward.push_back( kRewardItem );
    kDBPacket.m_nQuestionID = kPacket_.m_nQuestionID;
    kDBPacket.m_nQuestionLevel = kQuestion.m_nLevel;
    kDBPacket.m_nCorrectCount = m_kSphinxUserData.GetRightAnswerCount();

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_SPHINX_ANSWER_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_SPHINX_00,        kPacket.m_nOK, 1 );     // 스핑크스 OX 이벤트 기간이 아님.
        ERR_CASE( ERR_SPHINX_09,        kPacket.m_nOK, 2 );     // 존재하지 않는 문제.
        ERR_CASE( ERR_SPHINX_10,        kPacket.m_nOK, 3 );     // 정답을 맞추지 못했음.
        ERR_CASE( ERR_SPHINX_11,        kPacket.m_nOK, 4 );     // 보상을 가져오지 못했음.
        ERR_CASE( ERR_SPHINX_12,        kPacket.m_nOK, 5 );     // 유저의 스핑크스 정보를 불러오지 못했음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"스핑크스 정답 요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"스핑크스 정답 요청 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nQuestionID )
        << BUILD_LOG( kPacket_.m_nAnswerNum )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_SPHINX_ANSWER_ACK );
    }
}

IMPL_ON_FUNC( EVENT_SPHINX_ANSWER_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }
    // 유저 인벤토리 갱신.
    m_kInventory.AddItemList( kPacket_.m_vecItem );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_SPHINX_00,        kPacket_.m_nOK, 1 );     // 스핑크스 OX 이벤트 기간이 아님.
        ERR_CASE( ERR_SPHINX_09,        kPacket_.m_nOK, 2 );     // 존재하지 않는 문제.
        ERR_CASE( ERR_SPHINX_10,        kPacket_.m_nOK, 3 );     // 정답을 맞추지 못했음.
        ERR_CASE( ERR_SPHINX_11,        kPacket_.m_nOK, 4 );     // 보상을 가져오지 못했음.
        ERR_CASE( ERR_SPHINX_12,        kPacket_.m_nOK, 5 );     // 유저의 스핑크스 정보를 불러오지 못했음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"스핑크스 정답 요청중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"스핑크스 유저 정답 요청 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SPHINX_ANSWER_ACK );
}

IMPL_ON_FUNC( EVENT_CREATE_PARTY_REQ ) // 파티를 맺을 유저의 UID
{
    //_VERIFY_STATE((2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_AGIT )); // STATE_AGIT

    KPartyData kData;
    KPartyMember kPartyMaster; // inviter, host
    kPartyMaster.m_dwUID = GetUID();
    kPartyMaster.m_strNickName = GetNickName();
    kPartyMaster.m_IsHost = true;
    kPartyMaster.m_ServerUID = SiKGameServer()->GetServerID();
    kPartyMaster.m_dwState = KPartyMember::US_READY;

    kData.m_vecPartyPeople.push_back( kPartyMaster ); // 방장 넣기, 초대한 사람

    KPartyMember kPartyMember; // invitee
    kPartyMember.m_dwUID = kPacket_.m_dwInviteUserUID;

    kData.m_vecPartyPeople.push_back( kPartyMember ); // 방원 넣기, 초대받는 사람    
    kData.m_kInvitee.m_dwUID = kPacket_.m_dwInviteUserUID;
    kData.m_dwServerUID = SiKGameServer()->GetServerID(); // 방장의 ServerUID를 넣어준다.
    kData.m_dwPartyType = kPacket_.m_dwPartyType;

    // 파티 타입이 매칭일 경우에 대한 체크.
    if( kPacket_.m_dwPartyType == KEVENT_CREATE_PARTY_REQ::PT_MATCHING ) {
        KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwInviteUserUID ) );
        if( !spUser ) {
            START_LOG( cwarn, L"파티 요청할 유저가 게임서버에 없음. UserUID : " << kPacket_.m_dwInviteUserUID ) 
                << END_LOG;
            kData.m_nOK = KPartyData::RT_REJECT; // 초대유저가 서버에 없을때 거절패킷 전달.
            SEND_DATA( EVENT_CREATE_PARTY_ACK, kData );
            return;
        }
    }

    SiKCnConnector()->SendPacket( KCenterEvent::ECN_CREATE_PARTY_REQ, kData ); // 센터에 파티생성 요청

    START_LOG( clog, L"파티 생성 요청, User UID : " << GetUID() << L", Name : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwPartyType ) 
        << BUILD_LOG( kPacket_.m_dwInviteUserUID ) 
        << END_LOG;
}

_IMPL_ON_FUNC( EVENT_CREATE_PARTY_ASK, KPartyData )
{
    //_VERIFY_STATE( (2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_AGIT ));

    if ( m_kParty.m_dwPartyUID != 0 ) { // 이미 어떤 파티에 소속이므로 no 응답을 보낸다
        KPartyMember kMember;
        kMember.m_strNickName = GetNickName();
        kMember.m_dwUID = GetUID();
        kMember.m_IsHost = false;

        kPacket_.m_bAnswer = false;
        kPacket_.m_kInvitee = kMember;

        SiKCnConnector()->SendPacket( KCenterEvent::ECN_CREATE_PARTY_ANSWER, kPacket_ ); // 센터에 파티생성 요청

        START_LOG( cwarn, L"기존 파티 있음, Party UID : " << m_kParty.m_dwPartyUID )
            << BUILD_LOG( m_kParty.m_vecPartyPeople.size() ) << END_LOG;

        return;
    }

    SEND_RECEIVED_PACKET( EVENT_CREATE_PARTY_ASK );
}

_IMPL_ON_FUNC( EVENT_CREATE_PARTY_ANSWER, KPartyData ) // EVENT_CREATE_PARTY_ASK 에 대한 답
{ // kPacket_ 은 파티신청자(파장) 의 UID
    //_VERIFY_STATE((2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_AGIT ));

    if ( kPacket_.m_bAnswer == true ) {

        KPartyMember kMember;
        kMember.m_strNickName = GetNickName();
        kMember.m_dwUID = GetUID();
        kMember.m_IsHost = false;
        kMember.m_ServerUID = SiKGameServer()->GetServerID();

        kPacket_.m_kInvitee = kMember;
    }
    SiKCnConnector()->SendPacket( KCenterEvent::ECN_CREATE_PARTY_ANSWER, kPacket_ ); // 센터에 파티생성 요청

    START_LOG( clog, L"파티 생성 요청 응답, Party UID : " << kPacket_.m_dwPartyUID )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_bAnswer )
        << BUILD_LOG( kPacket_.m_vecPartyPeople.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_CREATE_PARTY_ACK, KPartyData )
{
    if ( kPacket_.m_nOK == KPartyData::RT_OK ) { // 성공, 0
        m_kParty = kPacket_;
    }

    START_LOG( clog, L"파티 생성 결과, Party UID : " << kPacket_.m_dwPartyUID )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecPartyPeople.size() )
        << BUILD_LOG( m_kParty.m_dwPartyUID ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CREATE_PARTY_ACK );
}

IMPL_ON_FUNC( EVENT_INVITE_PARTY_REQ )
{
    //_VERIFY_STATE( (2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_AGIT ));
    KPartyData kPacket;
    int nMatchMaxPartyNum;

    if ( m_kParty.m_vecPartyPeople.empty() ) {
        SET_ERR_GOTO( ERR_PARTY_05, END_PROC );
    }

    if ( m_kParty.m_vecPartyPeople.front().m_dwUID != GetUID() ) {
        SET_ERR_GOTO( ERR_PARTY_06, END_PROC );
    }

    kPacket = m_kParty;
    kPacket.m_kInvitee.m_dwUID = kPacket_.m_dwInviteUserUID ;

    // 파티 타입이 매칭일 경우에 대한 체크.
    if( kPacket_.m_dwPartyType == KEVENT_CREATE_PARTY_REQ::PT_MATCHING ) {
        KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwInviteUserUID ) );
        if( !spUser ) {
            START_LOG( cwarn, L"파티 요청할 유저가 게임서버에 없음. UserUID : " << kPacket_.m_dwInviteUserUID ) 
                << END_LOG;
            SET_ERR_GOTO( ERR_PARTY_05, END_PROC );
        }
        nMatchMaxPartyNum = SiKPVPMatchManager()->GetMatchPartyMaxNum();
        int nPartySize = m_kParty.m_vecPartyPeople.size();
        if( nPartySize >= nMatchMaxPartyNum ) {
            SET_ERR_GOTO( ERR_PARTY_10, END_PROC );
        }
    }

    SiKCnConnector()->SendPacket( KCenterEvent::ECN_INVITE_PARTY_REQ, kPacket );
    return;

END_PROC:

    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_PARTY_05,         kPacket.m_nOK, 2 );     // 파티원 없음
        ERR_CASE( ERR_PARTY_06,         kPacket.m_nOK, 3 );     // 파티장이 아닌데 초대 요청
        ERR_CASE( ERR_PARTY_10,         kPacket.m_nOK, 4 );     // 파티 최대 인원 초과
    default:
        START_LOG( cerr, L"파티 초대 요청중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    SEND_PACKET( EVENT_INVITE_PARTY_BROAD );
}

_IMPL_ON_FUNC( EVENT_INVITE_PARTY_ASK, KPartyData )
{
    if ( m_kParty.m_dwPartyUID != 0 ) { // 이미 어떤 파티에 소속이므로 no 응답을 보낸다
        KPartyMember kMember;
        kMember.m_strNickName = GetNickName();
        kMember.m_dwUID = GetUID();
        kMember.m_IsHost = false;

        kPacket_.m_bAnswer = false;
        kPacket_.m_kInvitee = kMember;

        SiKCnConnector()->SendPacket( KCenterEvent::ECN_INVITE_PARTY_ANSWER, kPacket_ ); // 센터에 파티생성 요청
        return;
    }

    SEND_RECEIVED_PACKET( EVENT_INVITE_PARTY_ASK );
}

_IMPL_ON_FUNC( EVENT_INVITE_PARTY_ANSWER, KPartyData )
{
    //_VERIFY_STATE( (2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_AGIT ));

    if ( kPacket_.m_bAnswer == true ) {

        KPartyMember kMember;
        kMember.m_strNickName = GetNickName();
        kMember.m_dwUID = GetUID();
        kMember.m_IsHost = false;
        kMember.m_ServerUID = SiKGameServer()->GetServerID();                

        kPacket_.m_kInvitee = kMember;
    }

    SiKCnConnector()->SendPacket( KCenterEvent::ECN_INVITE_PARTY_ANSWER, kPacket_ );
}

_IMPL_ON_FUNC( EVENT_INVITE_PARTY_BROAD, KPartyData )
{
    if ( kPacket_.m_nOK == 0 ) {
        m_kParty = kPacket_;
    }

    SEND_RECEIVED_PACKET( EVENT_INVITE_PARTY_BROAD );    
}

_IMPL_ON_FUNC( EVENT_CHANGE_PARTY_HOST_REQ, DWORD )
{
    //_VERIFY_STATE( (2, KUserFSM::STATE_SQUARE, KUserFSM::STATE_AGIT ));

    KPartyData kPacket;
    KPartyMember kMember;
    KPartyData kData;

    if ( m_kParty.m_dwPartyUID == 0 || m_kParty.m_vecPartyPeople.empty() ) {
        START_LOG( cerr, L"파티가 없거나 파티원 없는데 파티장 교체 패킷 보냄 User : " << GetName() ) << END_LOG;
        SET_ERR_GOTO( ERR_PARTY_05, END_PROC );
    }

    if ( GetUID() != m_kParty.m_vecPartyPeople.front().m_dwUID ) {
        START_LOG( cerr, L"파티장이 아닌데 파티장 교체 패킷 보냄 User : " << GetName() ) << END_LOG;
        SET_ERR_GOTO( ERR_PARTY_08, END_PROC );
    }

    if ( GetUID() == kPacket_ ) {
        START_LOG( cerr, L"파티장 본인 UID로 파티장 위임 패킷 보냄 User : " << GetName() ) << END_LOG;
        SET_ERR_GOTO( ERR_PARTY_09, END_PROC );
    }

    kData = m_kParty;
    kMember.m_dwUID = kPacket_;
    kData.m_vecPartyPeople.clear();
    kData.m_vecPartyPeople.push_back( kMember );

    SiKCnConnector()->SendPacket( KCenterEvent::ECN_CHANGE_PARTY_HOST_REQ, kData );
    return;

END_PROC:

    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_PARTY_05,         kPacket.m_nOK, 1 );     // 파티원 없음
        ERR_CASE( ERR_PARTY_08,         kPacket.m_nOK, 2 );     // 파티장이 아닌데 파티장 위임 요청 )
        ERR_CASE( ERR_PARTY_09,         kPacket.m_nOK, 3 );     // 파티장 본인에게 파티장 위임 요청 )
    default:
        START_LOG( cerr, L"파티장 위임 요청중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    SEND_PACKET( EVENT_CHANGE_PARTY_HOST_BROAD );

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"파티장 위임 요청 MSG :" << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_kParty.m_dwPartyUID ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_CHANGE_PARTY_HOST_BROAD, KPartyData )
{
    if ( kPacket_.m_dwPartyUID == 0 || kPacket_.m_vecPartyPeople.empty() ) {
        START_LOG( cerr, L"파티가 없거나 파티원 없는데 파티장 교체 패킷 보냄 User : " << GetName() ) << END_LOG;
        return;
    }

    START_LOG( clog, L"파티장 변경 전" )
        << BUILD_LOG( kPacket_.m_vecPartyPeople.front().m_strNickName ) << END_LOG;

    m_kParty = kPacket_;    
    SEND_RECEIVED_PACKET( EVENT_CHANGE_PARTY_HOST_BROAD );

    START_LOG( clog, L"파티장 변경 후 알림" )
        << BUILD_LOG( kPacket_.m_vecPartyPeople.front().m_strNickName ) << END_LOG;

}

_IMPL_ON_FUNC( EVENT_LEAVE_PARTY_USER_REQ, DWORD )
{
    // 자신이 나갈 때    
    int kPacket = -99;
    KPartyData kData;
    KPartyMember kMember;
    kMember.m_dwUID = kPacket_;


    kData.m_dwPartyUID = m_kParty.m_dwPartyUID;
    kData.m_vecPartyPeople.push_back( kMember );

    SEND_ID( EVENT_LEAVE_PARTY_USER_ACK );
    m_kParty.Init();

    SiKCnConnector()->SendPacket( KCenterEvent::ECN_LEAVE_PARTY_REQ, kData );

    SET_ERROR( NET_OK );

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket, 0 );
        //ERR_CASE( ERR_NOT_FINISHED_YET, kPacket, -98 );
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;        
    }

    _LOG_SUCCESS( kPacket == 0, L"파티 멤버 나가기 요청 MSG :" << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket )
        << BUILD_LOG( kData.m_dwPartyUID ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_LEAVE_PARTY_BROAD, KPartyData )
{
    m_kParty = kPacket_;
    SEND_RECEIVED_PACKET( EVENT_LEAVE_PARTY_BROAD );
}


IMPL_ON_FUNC_NOPARAM( EVENT_INVITE_PARTY_ROOM_REQ )
{
    VERIFY_STATE( ( 1, KUserFSM::STATE_ROOM ) );

    if ( m_kParty.m_dwPartyUID == 0 ) {
        START_LOG( cerr, L"파티가 없는데 파티방 생성 후 초대 요청 보냄" << GetName() ) << END_LOG;
        return;
    }

    KPartyRoomData kPacket;
    kPacket.m_kPartyData = m_kParty;
    
    // 파티 방의 패스워드를 보내줘야 함.
    JIF( m_pkRoom );

    kPacket.m_strRoomPassword = m_pkRoom->GetPassword();

    SiKCnConnector()->SendPacket( KCenterEvent::ECN_INVITE_PARTY_ROOM_REQ, kPacket );
    
    START_LOG( clog, L"파티방 생성 후 초대하기, User : " << GetName() )
        << BUILD_LOG( m_kParty.m_dwPartyUID )
        << BUILD_LOG( m_kParty.m_vecPartyPeople.size() )
        << BUILD_LOG( kPacket.m_strRoomPassword ) << END_LOG;
}


_IMPL_ON_FUNC( EVENT_BREAK_PARTY_NOT, KPartyData )
{

    SEND_RECEIVED_PACKET( EVENT_BREAK_PARTY_NOT ); 

    START_LOG( clog, L"파티 해체됨, Party UID : " << kPacket_.m_dwPartyUID ) << END_LOG;

    m_kParty.Init();
}

_IMPL_ON_FUNC( EVENT_BAN_PARTY_USER_REQ, DWORD )
{
    KPartyData kData;
    KPartyMember kMember;
    int kPacket = -99;

    if ( m_kParty.m_vecPartyPeople.empty() ) {
        START_LOG( cerr, L"파티 멤버가 비었음" << m_kParty.m_dwPartyUID ) << END_LOG;
        SET_ERR_GOTO( ERR_PARTY_05, END_PROC );        
    }

    if ( m_kParty.m_vecPartyPeople.front().m_dwUID != GetUID() ) {
        SET_ERR_GOTO( ERR_PARTY_01, END_PROC );
    }

    kMember.m_dwUID = kPacket_;
    kData.m_dwPartyUID = m_kParty.m_dwPartyUID;
    kData.m_vecPartyPeople.push_back( kMember );

    SiKCnConnector()->SendPacket( KCenterEvent::ECN_BAN_PARTY_USER_REQ, kData );

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket, 0 );
        ERR_CASE( ERR_PARTY_01, kPacket, 1 ); // 파티장이 아닌데 강퇴 요청
        ERR_CASE( ERR_PARTY_05, kPacket, 2 );
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket, -98 );
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;   
        break;
    }

    _LOG_SUCCESS( kPacket == 0, L"파티 멤버 퇴장 요청 MSG :" << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket )
        << BUILD_LOG( m_kParty.m_dwPartyUID ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_BAN_PARTY_USER_NOT, KPartyData )
{

    SEND_RECEIVED_PACKET( EVENT_BAN_PARTY_USER_NOT );

    START_LOG( clog, L"파티 강퇴 당함, Party UID : " << kPacket_.m_dwPartyUID )
        << BUILD_LOG( GetUID() ) << END_LOG;

    m_kParty.Init();
}

IMPL_ON_FUNC( EVENT_STRENGTH_EQUIP_REQ )
{
    KEVENT_STRENGTH_EQUIP_ACK kPacket;
    kPacket.m_nOK = 99;
    KItemPtr pkStrengthItem; // 강화석 아이템.
    KItemPtr pkEquipItem;    // 강화석 장착할 아이템.
    std::list<std::pair<int,float>> AttributeList;
    int nAttributeCount = 0;
    SET_ERROR( ERR_UNKNOWN );

    pkStrengthItem = m_kInventory.FindItemByItemUID( kPacket_.m_StrengthItemUID );
    pkEquipItem = m_kInventory.FindItemByItemUID( kPacket_.m_EquipItemUID );
    if( !pkStrengthItem || !pkEquipItem ) { // 아이템을 가지고 있는지 체크.
        SET_ERR_GOTO( ERR_STRENGTH_00, END_PROC );
    }

    if( pkStrengthItem->m_EnchantEquipItemUID > 0 ) { // 강화석이 사용중인지 체크.
        SET_ERR_GOTO( ERR_STRENGTH_01, END_PROC );
    }

    if( pkEquipItem->m_EnchantEquipItemUID > 0 ) { // 강화석 장착할 아이템이 사용중인지 체크.
        SET_ERR_GOTO( ERR_STRENGTH_01, END_PROC );
    }

    // 강화석은 영구아이템에만 장착 가능하다.
    if( pkEquipItem->m_nCount != KItem::UNLIMITED_ITEM || pkEquipItem->m_nPeriod != KItem::UNLIMITED_ITEM ) {
        SET_ERR_GOTO( ERR_STRENGTH_20, END_PROC );
    }

    // PC방 아이템은 장착하지 못한다.
    if( SiKIPAdvantage()->GetMaximumPCBangItemUID() >= pkEquipItem->m_ItemUID ) {
        SET_ERR_GOTO( ERR_STRENGTH_21, END_PROC );
    }

    // 강화석 장착 가능한 아이템인지 확인.
    if( !SiKStrengthManager()->IsUseStrengthEquipItem( pkEquipItem->m_ItemID ) ) {
        SET_ERR_GOTO( ERR_STRENGTH_23, END_PROC );
    }

    if( pkStrengthItem->m_cGradeID > KItem::GRADE_NORMAL) {
        if( !SiKStrengthManager()->GetStrengthAttributeInfo( static_cast<DWORD>( pkStrengthItem->m_cGradeID ), kPacket_.m_dwEquipItemType, kPacket_.m_listAttributeList ) ) {
            goto END_PROC;
        }
    }

    kPacket_.m_kStrengthItem = *pkStrengthItem;     // 강화석 장비 넣어주기.
    kPacket_.m_kEquipItem = *pkEquipItem;           // 강화석 장착할 장비 넣어주기.

    kPacket_.m_kStrengthItem.m_EnchantEquipItemUID = pkEquipItem->m_ItemUID;
    kPacket_.m_kStrengthItem.m_cEnchantEquipGradeID = pkStrengthItem->m_cGradeID; // 강화석은 자신의 등급정보 세팅.
    // 강화석 장착된 아이템에 정보 갱신.
    kPacket_.m_kEquipItem.m_EnchantEquipItemUID = pkStrengthItem->m_ItemUID;
    kPacket_.m_kEquipItem.m_cEnchantLevel = pkStrengthItem->m_cEnchantLevel;
    kPacket_.m_kEquipItem.m_cEnchantEquipGradeID = pkStrengthItem->m_cGradeID;

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_STRENGTH_EQUIP_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );    // 성공
        ERR_CASE( ERR_STRENGTH_00,      kPacket.m_nOK, 1 );     // 해당 아이템 인벤토리에 없음.
        ERR_CASE( ERR_STRENGTH_01,      kPacket.m_nOK, 2 );     // 강화석이 이미 사용중임.
        ERR_CASE( ERR_STRENGTH_19,      kPacket.m_nOK, 3 );     // 스킬 갯수가 정의되지 않았음.
        ERR_CASE( ERR_STRENGTH_20,      kPacket.m_nOK, 4 );     // 강화석 장착할 아이템이 영구 아이템이 아니다.
        ERR_CASE( ERR_STRENGTH_21,      kPacket.m_nOK, 5 );     // pc방 아이템은 이용할 수 없습니다.
        ERR_CASE( ERR_STRENGTH_22,      kPacket.m_nOK, 6 );     // 스킬 가져오기 실패.
        ERR_CASE( ERR_STRENGTH_23,      kPacket.m_nOK, 7 );     // 강화석 장착못하는 아이템.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );    // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"강화석 장착 요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"강화석 장착 요청 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_StrengthItemUID )
        << BUILD_LOG( kPacket_.m_EquipItemUID )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_STRENGTH_EQUIP_ACK );
    }
}

IMPL_ON_FUNC( EVENT_STRENGTH_EQUIP_ACK )
{
    KItemPtr pkStrengthItem; // 강화석 아이템.
    KItemPtr pkEquipItem;    // 강화석 장착할 아이템.
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    pkStrengthItem = m_kInventory.FindItemByItemUID( kPacket_.m_kStrengthItem.m_ItemUID );
    if( pkStrengthItem ) { // 강화석에 장착된 아이템 정보, 스킬정보 갱신.
        pkStrengthItem->m_EnchantEquipItemUID = kPacket_.m_kStrengthItem.m_EnchantEquipItemUID;
        pkStrengthItem->m_cEnchantEquipGradeID = kPacket_.m_kStrengthItem.m_cEnchantEquipGradeID;
        pkStrengthItem->m_vecAttribute = kPacket_.m_kStrengthItem.m_vecAttribute;
        pkStrengthItem->m_cCharType = kPacket_.m_kStrengthItem.m_cCharType;
    }

    pkEquipItem = m_kInventory.FindItemByItemUID( kPacket_.m_kEquipItem.m_ItemUID );
    if( pkEquipItem ) { // 강화석에 장착된 아이템 정보, 스킬정보 갱신.
        pkEquipItem->m_EnchantEquipItemUID = kPacket_.m_kEquipItem.m_EnchantEquipItemUID;
        pkEquipItem->m_cEnchantLevel = kPacket_.m_kEquipItem.m_cEnchantLevel;
        pkEquipItem->m_cEnchantEquipGradeID = kPacket_.m_kEquipItem.m_cEnchantEquipGradeID;
    }

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );    // 성공
        ERR_CASE( ERR_STRENGTH_00,      kPacket_.m_nOK, 1 );     // 해당 아이템 인벤토리에 없음.
        ERR_CASE( ERR_STRENGTH_01,      kPacket_.m_nOK, 2 );     // 강화석이 이미 사용중임.
        ERR_CASE( ERR_STRENGTH_19,      kPacket_.m_nOK, 3 );     // 스킬 갯수가 정의되지 않았음.
        ERR_CASE( ERR_STRENGTH_20,      kPacket_.m_nOK, 4 );     // 강화석 장착할 아이템이 영구 아이템이 아니다.
        ERR_CASE( ERR_STRENGTH_21,      kPacket_.m_nOK, 5 );     // pc방 아이템은 이용할 수 없습니다.
        ERR_CASE( ERR_STRENGTH_22,      kPacket_.m_nOK, 6 );     // 스킬 가져오기 실패.
        ERR_CASE( ERR_STRENGTH_23,      kPacket_.m_nOK, 7 );     // 강화석 장착못하는 아이템.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );    // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"강화석 장착 요청 응답 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"강화석 장착 유저 데이터 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_STRENGTH_EQUIP_ACK );
}

IMPL_ON_FUNC( EVENT_STRENGTH_ACTION_REQ )
{
    KEVENT_STRENGTH_ACTION_ACK kPacket;
    kPacket.m_nOK = 99;
    KItemPtr pkStrengthItem;    // 강화석 아이템.
    KItemPtr pkEquipItem;       // 강화석 장착된 아이템.
    KItemPtr pkAssistItem;      // 강화 보조 아이템.
    DWORD dwStrengthGrade = 0;   // 강화석 Grade( NORMAL & EPIC )
    float fAddRatio = 0.f;      // 강화 보조 아이템 배율.
    bool bBreakUpProtect = false; // 아이템 깨짐 하락 방지 아이템 사용.
    int nDefaultMaterialCnt = 0; // 강화에 필요한 기본 아이템 갯수.( 강화 레벨 )
    DWORD dwSpecificItemLevel = 0; // 특정 강화 아이템 사용한 아이템 레벨.
    int nCurrentStrengthLevel = 0; // 강화석 현재 레벨.
    int nStrengthLevel = 0; // 강화석 다음 레벨.
    int nAttributeTableID = 0;
    int nAttributeCount = 0;
    std::list<std::pair<int,float>> AttributeList; // 에픽 강화석 업그레이드시 스킬 정보.
    std::vector< KAttributeInfo >::iterator vit;
    KRainBowData kRainbow;

    SET_ERROR( ERR_UNKNOWN );

    pkStrengthItem = m_kInventory.FindItemByItemUID( kPacket_.m_StrengthItemUID );
    if( !pkStrengthItem ) { // 아이템을 가지고 있는지 체크.
        SET_ERR_GOTO( ERR_STRENGTH_00, END_PROC );
    }
    pkEquipItem = m_kInventory.FindItemByItemUID( pkStrengthItem->m_EnchantEquipItemUID );
    if( !pkEquipItem ) {
        SET_ERR_GOTO( ERR_STRENGTH_00, END_PROC );
    }

    if( pkEquipItem->m_nPeriod > 0 ) { // 기간제 아이템이면..,
        SET_ERR_GOTO( ERR_STRENGTH_25, END_PROC );
    }

    // 강화 보조 Type과 강화 보조 아이템 ID가 잘못 설정된 경우 에러 처리
    if ( false == CheckStrengthActionAssistType( kPacket_.m_dwAssistType, kPacket_.m_AssistItemID ) ) {
        SET_ERR_GOTO( ERR_STRENGTH_24, END_PROC );
    }

    // 강화 보조 아이템 Type이 설정되지 않았는데 Item값이 있을 경우 Error 처리.
    if( kPacket_.m_dwAssistType == 0 && kPacket_.m_AssistItemID != 0 ) {
        SET_ERR_GOTO( ERR_STRENGTH_24, END_PROC );
    }

    // 강화하는 아이템의 장착부위 유효성 체크
    if ( KStrengthManager::EISP_HELMET > kPacket_.m_dwEquipItemType || KStrengthManager::EISP_WEAPON < kPacket_.m_dwEquipItemType ) {
        SET_ERR_GOTO( ERR_STRENGTH_27, END_PROC );
    }

    DWORD dwItemLevel = 0;
    SiKItemManager()->GetItemEquipLevel( pkEquipItem->m_ItemID, dwItemLevel );
    if ( false == SiKStrengthManager()->GetStrengthGPCost( dwItemLevel, kPacket_.m_dwEquipItemType, kPacket_.m_nGPCost ) ) {
        SET_ERR_GOTO( ERR_STRENGTH_07, END_PROC );
    }

    // 현재 보유한 GP로 강화가 가능한지 확인
    int nCurrentGP = GetGP();
    if ( nCurrentGP <= 0 || nCurrentGP < kPacket_.m_nGPCost ) {
        SET_ERR_GOTO( ERR_STRENGTH_26, END_PROC );
    }

    // 강화석의 종류 가져오기.( Normal & Epic )
    dwStrengthGrade = static_cast<DWORD>( pkStrengthItem->m_cGradeID );


    if( kPacket_.m_dwAssistType > KStrengthManager::AT_NOUSE && kPacket_.m_AssistItemID > 0 ) { // 보조 아이템 사용시.
        pkAssistItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_AssistItemID );
        if( !pkAssistItem ) { // 강화 보조 아이템을 가지고 있는지 체크.
            SET_ERR_GOTO( ERR_STRENGTH_00, END_PROC );
        }

        switch( kPacket_.m_dwAssistType ) { // 보조 아이템에 대한 구분.
        case KEVENT_STRENGTH_ACTION_REQ::AT_UPGRADE:
            if( !SiKStrengthManager()->GetUpgradeAssistRatio( kPacket_.m_AssistItemID, fAddRatio ) ) {
                SET_ERR_GOTO( ERR_STRENGTH_06, END_PROC );
            }
            break;
        case KEVENT_STRENGTH_ACTION_REQ::AT_NOBREAK:
            if( !SiKStrengthManager()->CheckProtectionLevel( kPacket_.m_AssistItemID, pkStrengthItem->m_cEnchantLevel ) ) {
                SET_ERR_GOTO( ERR_STRENGTH_05, END_PROC );
            }
            break;
        case KEVENT_STRENGTH_ACTION_REQ::AT_SPECIFIC:
            if( !SiKStrengthManager()->UpdateSpecificLevel( kPacket_.m_AssistItemID, dwSpecificItemLevel ) ) {
                SET_ERR_GOTO( ERR_STRENGTH_09, END_PROC );
            }
            if( static_cast<DWORD>( pkStrengthItem->m_cEnchantLevel ) >= dwSpecificItemLevel ) {
                SET_ERR_GOTO( ERR_STRENGTH_10, END_PROC );
            }
            break;
        case KEVENT_STRENGTH_ACTION_REQ::AT_UPGRADE_NOBREAK:
            if( !SiKStrengthManager()->CheckUpgradeProtectionLevel( kPacket_.m_AssistItemID, pkStrengthItem->m_cEnchantLevel, fAddRatio ) ) {
                SET_ERR_GOTO( ERR_STRENGTH_05, END_PROC );
            }
            break;
        }
    }
    // 강화석 스킬에 대한 확률 추가( 강화 확률 상승, 강화 파괴 방지 )
    if( pkStrengthItem->m_cGradeID > KItem::GRADE_NORMAL) {
        for( vit = pkStrengthItem->m_vecAttribute.begin(); vit != pkStrengthItem->m_vecAttribute.end(); ++vit ) {
            if( vit->m_cType == KStrengthManager::ATTRTYPE_FATAL_ENCHANT_PROBABILITY_UP || vit->m_cType == KStrengthManager::ATTRTYPE_FATAL_ENCHANT_DESTORY_RESIST ) {
                float fValue = std::max<float>( 1, vit->m_fValue ); // 나누기 몫 0 방지
                fValue = fValue / 10;
                fAddRatio += fValue;
            }
        }
    }

    // 길드 보너스
    if ( m_kGuildUserInfo.m_dwGuildUID > 0 ) {
        float fGuildBonus = GetGuildBonus( KGuildConfig::GA_RUNE_ENCHANT_BONUS );
        fAddRatio += fGuildBonus;
    }

    // 레인 보우 이벤트 보너스.
    bool bRainbow = SiKRainbow()->GetEventByID( KRainbow::RE_LUNE_STRONG, kRainbow );
    if( bRainbow ) {
        fAddRatio += kRainbow.m_fPrimaryRatio;
    }

    // 시간제한이벤트 보너스
    fAddRatio += SiKTimeEventManager()->GetStrengthBonus();

    nCurrentStrengthLevel = static_cast<int>( pkStrengthItem->m_cEnchantLevel );
    nStrengthLevel = nCurrentStrengthLevel + 1;
    
    if( !SiKStrengthManager()->StrengthAction( dwStrengthGrade, nStrengthLevel, fAddRatio, kPacket_.m_dwAssistType, kPacket_.m_dwStrengthResult ) ) {
        SET_ERR_GOTO( ERR_STRENGTH_08, END_PROC );
    }

    kPacket_.m_kStrengthItem = *pkStrengthItem;
    kPacket_.m_kEquipItem = *pkEquipItem;
    if( kPacket_.m_dwAssistType > KStrengthManager::AT_NOUSE && pkAssistItem ) {
        kPacket_.m_kAssistItem = *pkAssistItem;
    }

    // 강화에 대한 결과를 아이템 구조체에 적용.
    switch( kPacket_.m_dwStrengthResult ) { // 보조 아이템에 대한 구분.
        case KStrengthManager::SR_SUCCESS: // 성공.
            kPacket_.m_kStrengthItem.m_cEnchantLevel += 1;
            break;
        case KStrengthManager::SR_WEAKNESS: // 하락
            kPacket_.m_kStrengthItem.m_cEnchantLevel -= 1;
            break;
        case KStrengthManager::SR_BREAK: // 깨짐
            kPacket_.m_kStrengthItem.m_cEnchantLevel = 0;
            kPacket_.m_kStrengthItem.m_EnchantEquipItemUID = 0;

            kPacket_.m_kEquipItem.m_EnchantEquipItemUID = 0;
            kPacket_.m_kEquipItem.m_cEnchantEquipGradeID = 0;
            break;
        case KStrengthManager::SR_DROP: // 초기화
            kPacket_.m_kStrengthItem.m_cEnchantLevel = 0;
            break;
        case KStrengthManager::SR_UPGRADE: // 에픽 강화석으로 업그레이드
            kPacket_.m_kStrengthItem.m_cGradeID = KItem::GRADE_EPIC;
            kPacket_.m_kStrengthItem.m_cEnchantEquipGradeID = KItem::GRADE_EPIC;
            kPacket_.m_kEquipItem.m_cEnchantEquipGradeID = kPacket_.m_kStrengthItem.m_cGradeID;

            if( !SiKStrengthManager()->GetStrengthAttributeInfo( static_cast<DWORD>( kPacket_.m_kStrengthItem.m_cGradeID ), kPacket_.m_dwEquipItemType, kPacket_.m_listAttributeList ) ) {
                goto END_PROC;
            }
            break;
        case KStrengthManager::SR_SPECIFIC: // 특정 레벨 변화
            kPacket_.m_kStrengthItem.m_cEnchantLevel = static_cast<char>( dwSpecificItemLevel );
            break;
    }
    kPacket_.m_kEquipItem.m_cEnchantLevel = kPacket_.m_kStrengthItem.m_cEnchantLevel;

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_STRENGTH_ACTION_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_STRENGTH_00,      kPacket.m_nOK, 1 );     // 해당 아이템 인벤토리에 없음.
        ERR_CASE( ERR_STRENGTH_03,      kPacket.m_nOK, 2 );     // 잘못된 강화석 ItemID.
        ERR_CASE( ERR_STRENGTH_04,      kPacket.m_nOK, 3 );     // 강화 재료의 레벨이 낮음.
        ERR_CASE( ERR_STRENGTH_05,      kPacket.m_nOK, 4 );     // 정의되지 않은 강화률 상승 아이템.
        ERR_CASE( ERR_STRENGTH_07,      kPacket.m_nOK, 5 );     // 스크립트에 정의 되지 않은 강화 아이템의 레벨
        ERR_CASE( ERR_STRENGTH_08,      kPacket.m_nOK, 6 );     // 강화할려는 레벨의 확률 설정 에러.
        ERR_CASE( ERR_STRENGTH_09,      kPacket.m_nOK, 7 );     // 설정되지 않은 특정 레벨 강화 부적.
        ERR_CASE( ERR_STRENGTH_10,      kPacket.m_nOK, 8 );     // 특정 레벨 강화보다 현재 아이템 레벨이 같거나 높음.
        ERR_CASE( ERR_STRENGTH_11,      kPacket.m_nOK, 9 );     // 강화석 DB 강화 에러.
        ERR_CASE( ERR_STRENGTH_12,      kPacket.m_nOK, 10);     // 강화석 재료 아이템 DB 삭제중 에러.
        ERR_CASE( ERR_STRENGTH_13,      kPacket.m_nOK, 11);     // 강화석 보조 아이템 DB 삭제중 에러.
        ERR_CASE( ERR_STRENGTH_21,      kPacket.m_nOK, 12 );    // pc방 아이템은 재료로 사용할 수 없습니다.
        ERR_CASE( ERR_STRENGTH_19,      kPacket.m_nOK, 13 );    // 스킬 갯수가 정의되지 않았음.
        ERR_CASE( ERR_STRENGTH_22,      kPacket.m_nOK, 14 );    // 스킬 가져오기 실패.
        ERR_CASE( ERR_STRENGTH_24,      kPacket.m_nOK, 15 );    // 강화석 보조 아이템 Type 설정 에러.
        ERR_CASE( ERR_STRENGTH_25,      kPacket.m_nOK, 16 );    // 기간제 아이템이면 안됨.
        ERR_CASE( ERR_STRENGTH_26,      kPacket.m_nOK, 17 );    // 보유 GP가 부족함.
        ERR_CASE( ERR_STRENGTH_27,      kPacket.m_nOK, 18 );    // 강화로 요청한 아이템의 타입이 비정상적임.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"강화석 강화 요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"강화석 강화 요청 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_StrengthItemUID )
        << BUILD_LOG( kPacket_.m_dwAssistType )
        << BUILD_LOG( kPacket_.m_AssistItemID )
        << BUILD_LOG( kPacket_.m_dwStrengthResult )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( nCurrentStrengthLevel )
        << BUILD_LOGc( kPacket_.m_kStrengthItem.m_ItemID )
        << BUILD_LOGc( kPacket_.m_kStrengthItem.m_ItemUID )
        << BUILD_LOGc( kPacket_.m_kStrengthItem.m_cEnchantLevel )
        << BUILD_LOGc( kPacket_.m_kStrengthItem.m_cGradeID )
        << BUILD_LOG( kPacket_.m_dwEquipItemType )
        << BUILD_LOG( kPacket_.m_kEquipItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kEquipItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kAssistItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kAssistItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_dwEquipItemType )
        << BUILD_LOG( dwItemLevel )
        << BUILD_LOG( kPacket_.m_nGPCost )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_STRENGTH_ACTION_ACK );
    }
}

IMPL_ON_FUNC( EVENT_STRENGTH_ACTION_ACK )
{
    KItemPtr pkStrengthItem;    // 강화석 아이템.
    KItemPtr pkEquipItem;       // 강화석 장착된 아이템.
    KItemPtr pkAssistItem;      // 강화 보조 아이템.

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) && !IS_CORRECT( ERR_STRENGTH_13 ) ) {
        goto END_PROC;
    }

    // 2013.12.31 nodefeat. 강화 편의성 개편으로 잼소모에서 GP소모로 변경
    bool bDBUpdate = false; // DB에서 GP 갱신 처리 하지 않았음.
    DecreaseGP( KGpStatInfo::GDT_STRENGTH_ACTION, bDBUpdate, kPacket_.m_nGPCost );
    kPacket_.m_nGP = GetGP();

    pkStrengthItem = m_kInventory.FindItemByItemUID( kPacket_.m_kStrengthItem.m_ItemUID );
    pkEquipItem = m_kInventory.FindItemByItemUID( kPacket_.m_kEquipItem.m_ItemUID );

    if( pkStrengthItem ) { // 강화석의 강화된 레벨 정보 갱신.
        pkStrengthItem->m_cEnchantLevel = kPacket_.m_kStrengthItem.m_cEnchantLevel;
        pkStrengthItem->m_cGradeID = kPacket_.m_kStrengthItem.m_cGradeID;
        if( kPacket_.m_dwStrengthResult == KStrengthManager::SR_UPGRADE ) { // 에픽 강화석 업그레이드시 스킬 정보 갱신.
            pkStrengthItem->m_vecAttribute = kPacket_.m_kStrengthItem.m_vecAttribute;
            pkStrengthItem->m_cEnchantEquipGradeID = kPacket_.m_kStrengthItem.m_cEnchantEquipGradeID;
        }
    }

    if( kPacket_.m_dwStrengthResult ==  KStrengthManager::SR_BREAK && pkStrengthItem ) { // 깨짐시 강화석 제거.
        m_kInventory.RemoveItem( pkStrengthItem->m_ItemUID );
        kPacket_.m_vecUsePeriodItems.push_back( pkStrengthItem->m_ItemUID );
    }

    if( pkEquipItem ) {
        pkEquipItem->m_EnchantEquipItemUID = kPacket_.m_kEquipItem.m_EnchantEquipItemUID;
        pkEquipItem->m_cEnchantLevel = kPacket_.m_kEquipItem.m_cEnchantLevel;
        pkEquipItem->m_cEnchantEquipGradeID = kPacket_.m_kEquipItem.m_cEnchantEquipGradeID;

        RefreshEquipItem( *pkEquipItem );
    }

    if( kPacket_.m_dwAssistType > KStrengthManager::AT_NOUSE ) { // 강화 보조 아이템 수량 깍기.
        pkAssistItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_kAssistItem.m_ItemID );
        if( pkAssistItem && pkAssistItem->m_nCount > 0 ) {
            pkAssistItem->m_nCount = kPacket_.m_kAssistItem.m_nCount;
            pkAssistItem->m_nInitCount = kPacket_.m_kAssistItem.m_nCount;
            kPacket_.m_vecUseCountItem.push_back( *pkAssistItem );
        }
        if ( pkAssistItem && pkAssistItem->m_nCount <= 0 ) { // 주문서 수량 0이면 아이템 삭제.
            m_kInventory.RemoveItem( kPacket_.m_kAssistItem.m_ItemID, kPacket_.m_kAssistItem.m_ItemUID );
        }
    }

    // 강화 성공 공지 출력.
    if( SiKStrengthManager()->GetStrengthNoticeEnable() && 
            ( kPacket_.m_dwStrengthResult ==  KStrengthManager::SR_SUCCESS ||
                kPacket_.m_dwStrengthResult ==  KStrengthManager::SR_SPECIFIC ) ) {
        DWORD dwStrengthMinLevel = SiKStrengthManager()->GetStrengthNoticeMinLevel();
        DWORD dwCurrentStrengthLevel = static_cast<DWORD>( kPacket_.m_kStrengthItem.m_cEnchantLevel );
        if( dwStrengthMinLevel <= dwCurrentStrengthLevel ) {
            SiKStrengthManager()->StrengthNotify( dwCurrentStrengthLevel, m_strNickName, GetLanguageCode() );
        }
    }

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_STRENGTH_00,      kPacket_.m_nOK, 1 );     // 해당 아이템 인벤토리에 없음.
        ERR_CASE( ERR_STRENGTH_03,      kPacket_.m_nOK, 2 );     // 잘못된 강화석 ItemID.
        ERR_CASE( ERR_STRENGTH_04,      kPacket_.m_nOK, 3 );     // 강화 재료의 레벨이 낮음.
        ERR_CASE( ERR_STRENGTH_05,      kPacket_.m_nOK, 4 );     // 정의되지 않은 강화률 상승 아이템.
        ERR_CASE( ERR_STRENGTH_07,      kPacket_.m_nOK, 5 );     // 스크립트에 정의 되지 않은 강화 아이템의 레벨
        ERR_CASE( ERR_STRENGTH_08,      kPacket_.m_nOK, 6 );     // 강화할려는 레벨의 확률 설정 에러.
        ERR_CASE( ERR_STRENGTH_09,      kPacket_.m_nOK, 7 );     // 설정되지 않은 특정 레벨 강화 부적.
        ERR_CASE( ERR_STRENGTH_10,      kPacket_.m_nOK, 8 );     // 특정 레벨 강화보다 현재 아이템 레벨이 같거나 높음.
        ERR_CASE( ERR_STRENGTH_11,      kPacket_.m_nOK, 9 );     // 강화석 DB 강화 에러.
        ERR_CASE( ERR_STRENGTH_12,      kPacket_.m_nOK, 10);     // 강화석 재료 아이템 DB 삭제중 에러.
        ERR_CASE( ERR_STRENGTH_13,      kPacket_.m_nOK, 11);     // 강화석 보조 아이템 DB 삭제중 에러.
        ERR_CASE( ERR_STRENGTH_21,      kPacket_.m_nOK, 12 );    // pc방 아이템은 재료로 사용할 수 없습니다.
        ERR_CASE( ERR_STRENGTH_19,      kPacket_.m_nOK, 13 );    // 스킬 갯수가 정의되지 않았음.
        ERR_CASE( ERR_STRENGTH_22,      kPacket_.m_nOK, 14 );    // 스킬 가져오기 실패.
        ERR_CASE( ERR_STRENGTH_24,      kPacket_.m_nOK, 15 );    // 강화석 보조 아이템 Type 설정 에러.
        ERR_CASE( ERR_STRENGTH_26,      kPacket_.m_nOK, 17 );     // 보유 GP가 부족함.
        ERR_CASE( ERR_STRENGTH_27,      kPacket_.m_nOK, 18 );     // 강화로 요청한 아이템의 타입이 비정상적임.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"강화석 강화 요청 응답 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"강화석 강화 유저 데이터 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAssistType )
        << BUILD_LOG( kPacket_.m_dwStrengthResult )
        << BUILD_LOG( kPacket_.m_vecUseCountItem.size() )
        << BUILD_LOG( kPacket_.m_vecUsePeriodItems.size() )
        << BUILD_LOG( kPacket_.m_nGP )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_STRENGTH_ACTION_ACK );
}

IMPL_ON_FUNC( EVENT_STRENGTH_BREAKUP_REQ )
{
    KEVENT_STRENGTH_BREAKUP_ACK kPacket;
    kPacket.m_nOK = 99;
    KItemPtr pkStrengthItem; // 강화석 아이템.
    KItemPtr pkEquipItem;       // 강화석 장착된 아이템.
    KItemPtr pkMaterialItem;    // 해체 주문서 아이템.
    SET_ERROR( ERR_UNKNOWN );

    pkStrengthItem = m_kInventory.FindItemByItemUID( kPacket_.m_StrengthItemUID );
    pkMaterialItem = m_kInventory.FindDurationItemByItemUID( kPacket_.m_BreakUpMaterialUID );
    if( !pkStrengthItem || !pkMaterialItem ) { // 아이템을 가지고 있는지 체크.
        SET_ERR_GOTO( ERR_STRENGTH_00, END_PROC );
    }
    pkEquipItem = m_kInventory.FindItemByItemUID( pkStrengthItem->m_EnchantEquipItemUID );
    if( !pkEquipItem ) { // 아이템을 가지고 있는지 체크.
        SET_ERR_GOTO( ERR_STRENGTH_00, END_PROC );
    }

    if( pkEquipItem->m_nPeriod > 0 ) { // 기간제 아이템이면..,
        SET_ERR_GOTO( ERR_STRENGTH_25, END_PROC );
    }

    if( !SiKStrengthManager()->CheckBreakUpMaterial( pkMaterialItem->m_ItemID ) ) {
        SET_ERR_GOTO( ERR_STRENGTH_14, END_PROC );
    }

    if( !SiKStrengthManager()->BreakUpAction( pkMaterialItem->m_ItemID, kPacket_.m_dwBreakUpResult ) ) {
        SET_ERR_GOTO( ERR_STRENGTH_15, END_PROC );
    }

    kPacket_.m_kStrengthItem = *pkStrengthItem;     // 강화석 장비 넣어주기.
    kPacket_.m_kMaterialItem = *pkMaterialItem;     // 해체 재료 아이템 넣어주기.
    kPacket_.m_kEquipItem = *pkEquipItem;

    // 강화석 해체 정보 갱신.
    switch( kPacket_.m_dwBreakUpResult ) {
        case KStrengthManager::SR_SUCCESS: // 성공.
            kPacket_.m_kEquipItem.m_EnchantEquipItemUID = 0;
            kPacket_.m_kEquipItem.m_cEnchantLevel = 0;
            kPacket_.m_kEquipItem.m_cEnchantEquipGradeID = 0;

            kPacket_.m_kStrengthItem.m_EnchantEquipItemUID = 0;
            break;
        case KStrengthManager::SR_BREAK: // 깨짐
            kPacket_.m_kEquipItem.m_EnchantEquipItemUID = 0;
            kPacket_.m_kEquipItem.m_cEnchantLevel = 0;
            kPacket_.m_kEquipItem.m_cEnchantEquipGradeID = 0;

            kPacket_.m_kStrengthItem.m_cEnchantLevel = 0;
            kPacket_.m_kStrengthItem.m_EnchantEquipItemUID = 0;
            break;
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_STRENGTH_BREAKUP_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_STRENGTH_00,      kPacket.m_nOK, 1 );     // 해당 아이템 인벤토리에 없음.
        ERR_CASE( ERR_STRENGTH_14,      kPacket.m_nOK, 2 );     // 등록되지 않은 아이템 해체 주문서.
        ERR_CASE( ERR_STRENGTH_15,      kPacket.m_nOK, 4 );     // 아이템 제작해제 실패.
        ERR_CASE( ERR_STRENGTH_11,      kPacket.m_nOK, 5 );     // 강화석 DB 강화 에러.
        ERR_CASE( ERR_STRENGTH_12,      kPacket.m_nOK, 6);      // 강화석 재료 아이템 DB 삭제중 에러.
        ERR_CASE( ERR_STRENGTH_13,      kPacket.m_nOK, 7);      // 강화석 보조 아이템 DB 삭제중 에러.
        ERR_CASE( ERR_STRENGTH_25,      kPacket.m_nOK, 8);      // 기간제 아이템이면 해체 안됨.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"강화석 해체 요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"강화석 해체 요청 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_StrengthItemUID )
        << BUILD_LOG( kPacket_.m_BreakUpMaterialUID )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_STRENGTH_BREAKUP_ACK );
    }
}

IMPL_ON_FUNC( EVENT_STRENGTH_BREAKUP_ACK )
{
    KItemPtr pkStrengthItem;    // 강화석 아이템.
    KItemPtr pkEquipItem;       // 강화석 장착된 아이템.
    KItemPtr pkMaterialItem;    // 해체 주문서 아이템.

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) && !IS_CORRECT( ERR_STRENGTH_13 ) ) {
        goto END_PROC;
    }

    pkStrengthItem = m_kInventory.FindItemByItemUID( kPacket_.m_kStrengthItem.m_ItemUID );
    pkEquipItem = m_kInventory.FindItemByItemUID( kPacket_.m_kEquipItem.m_ItemUID );
    pkMaterialItem = m_kInventory.FindDurationItemByItemUID( kPacket_.m_kMaterialItem.m_ItemUID );

    if( pkStrengthItem ) { // 강화석의 해체 결과 정보 갱신.
        if( kPacket_.m_dwBreakUpResult != KStrengthManager::SR_NONE ) {
            pkStrengthItem->m_cEnchantLevel = kPacket_.m_kStrengthItem.m_cEnchantLevel;
            pkStrengthItem->m_EnchantEquipItemUID = kPacket_.m_kStrengthItem.m_EnchantEquipItemUID;
            pkStrengthItem->m_cCharType = kPacket_.m_kStrengthItem.m_cCharType;
        }
    }

    if( kPacket_.m_dwBreakUpResult ==  KStrengthManager::SR_BREAK && pkStrengthItem ) { // 깨짐시 강화석 제거.
        m_kInventory.RemoveItem( pkStrengthItem->m_ItemUID );
        kPacket_.m_vecUsePeriodItems.push_back( pkStrengthItem->m_ItemUID );
    }

    if( pkEquipItem ) {
        pkEquipItem->m_EnchantEquipItemUID = kPacket_.m_kEquipItem.m_EnchantEquipItemUID;
        pkEquipItem->m_cEnchantLevel = kPacket_.m_kEquipItem.m_cEnchantLevel;
        pkEquipItem->m_cEnchantEquipGradeID = kPacket_.m_kEquipItem.m_cEnchantEquipGradeID;
    }

    // 해체 주문서 아이템 수량 감소 처리.
    if( pkMaterialItem && pkMaterialItem->m_nCount > 0 ) {
        pkMaterialItem->m_nCount = kPacket_.m_kMaterialItem.m_nCount;
        pkMaterialItem->m_nInitCount = kPacket_.m_kMaterialItem.m_nCount;
        kPacket_.m_vecUseCountItem.push_back( *pkMaterialItem );
    }
    if ( pkMaterialItem && pkMaterialItem->m_nCount <= 0 ) { // 수량다 사용한 수량아이템 깍기.
        m_kInventory.RemoveItem( kPacket_.m_kMaterialItem.m_ItemID, kPacket_.m_kMaterialItem.m_ItemUID );
    }

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_STRENGTH_00,      kPacket_.m_nOK, 1 );     // 해당 아이템 인벤토리에 없음.
        ERR_CASE( ERR_STRENGTH_14,      kPacket_.m_nOK, 2 );     // 등록되지 않은 아이템 해체 주문서.
        ERR_CASE( ERR_STRENGTH_07,      kPacket_.m_nOK, 3 );     // 스크립트에 정의 되지 않은 강화 아이템의 레벨
        ERR_CASE( ERR_STRENGTH_15,      kPacket_.m_nOK, 4 );     // 아이템 제작해제 실패.
        ERR_CASE( ERR_STRENGTH_11,      kPacket_.m_nOK, 5 );     // 강화석 DB 강화 에러.
        ERR_CASE( ERR_STRENGTH_12,      kPacket_.m_nOK, 6);      // 강화석 재료 아이템 DB 삭제중 에러.
        ERR_CASE( ERR_STRENGTH_13,      kPacket_.m_nOK, 7);      // 강화석 보조 아이템 DB 삭제중 에러.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"강화석 해체 요청 응답 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"강화석 해체 유저 데이터 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwBreakUpResult )
        << BUILD_LOG( kPacket_.m_vecUseCountItem.size() )
        << BUILD_LOG( kPacket_.m_vecUsePeriodItems.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_STRENGTH_BREAKUP_ACK );
}

IMPL_ON_FUNC( EVENT_STRENGTH_COMPOSE_REQ )
{
    KEVENT_STRENGTH_COMPOSE_ACK kPacket;
    kPacket.m_nOK = 99;
    KItemPtr pkStrengthItemFirst;  // 강화석 합성 첫번째 아이템.
    KItemPtr pkStrengthItemSecond; // 강화석 합성 두번째 아이템.
    KItemPtr pkFirstItem;          // 첫번째 강화석 박힌 아이템.
    KItemPtr pkSecondItem;         // 두번째 강화석 박힌 아이템.
    KItemPtr pkMaterialItem;       // 합성 주문서 아이템.
    DWORD dwComposeType;
    int nComposeResultLevel;
    int nComposeResultGrade;
    SET_ERROR( ERR_UNKNOWN );

    pkStrengthItemFirst = m_kInventory.FindItemByItemUID( kPacket_.m_prItemInfo.first );
    pkStrengthItemSecond = m_kInventory.FindItemByItemUID( kPacket_.m_prItemInfo.second );
    pkMaterialItem = m_kInventory.FindDurationItemByItemUID( kPacket_.m_dwComposeAssistUID );
    if( !pkStrengthItemFirst || !pkStrengthItemSecond || !pkMaterialItem ) { // 아이템을 가지고 있는지 체크.
        SET_ERR_GOTO( ERR_STRENGTH_00, END_PROC );
    }

    pkFirstItem = m_kInventory.FindItemByItemUID( pkStrengthItemFirst->m_EnchantEquipItemUID );
    pkSecondItem = m_kInventory.FindItemByItemUID( pkStrengthItemSecond->m_EnchantEquipItemUID );

    if( !pkFirstItem || !pkSecondItem ) { // 아이템을 가지고 있는지 체크.
        SET_ERR_GOTO( ERR_STRENGTH_00, END_PROC );
    }

    if( pkFirstItem->m_nPeriod > 0 || pkSecondItem->m_nPeriod > 0 ) { // 기간제 아이템이면..,
        SET_ERR_GOTO( ERR_STRENGTH_25, END_PROC );
    }

    if( !SiKStrengthManager()->GetComposeGradeType( pkStrengthItemFirst->m_cGradeID, pkStrengthItemSecond->m_cGradeID, dwComposeType ) ) {
        SET_ERR_GOTO( ERR_STRENGTH_17, END_PROC );
    }

    if( !SiKStrengthManager()->ComposeAction( dwComposeType, pkMaterialItem->m_ItemID, kPacket_.m_dwComposeResult ) ) {
        SET_ERR_GOTO( ERR_STRENGTH_17, END_PROC );
    }

    // 강화석 normal, epic 합성시 항상 epic 강화석을 살려둔다.
    if( pkStrengthItemFirst->m_cGradeID == KStrengthManager::GRADE_NORMAL && 
            pkStrengthItemSecond->m_cGradeID > KStrengthManager::GRADE_NORMAL ) {
                kPacket_.m_kFirstStrengthItem = *pkStrengthItemSecond; // 강화석 장비 넣어주기.
                kPacket_.m_kSecondStrengthItem = *pkStrengthItemFirst; // 강화석 장비 넣어주기.
                kPacket_.m_kMaterialItem = *pkMaterialItem;     // 합성 재료 아이템 넣어주기.
                kPacket_.m_kFirstEquipItem = *pkSecondItem;       // 강화석 장착 아이템 정보 넣어주기.
                kPacket_.m_kSecondEquipItem = *pkFirstItem;       // 강화석 장착 아이템 정보 넣어주기.
    } else {
        kPacket_.m_kFirstStrengthItem = *pkStrengthItemFirst; // 강화석 장비 넣어주기.
        kPacket_.m_kSecondStrengthItem = *pkStrengthItemSecond; // 강화석 장비 넣어주기.
        kPacket_.m_kMaterialItem = *pkMaterialItem;     // 합성 재료 아이템 넣어주기.
        kPacket_.m_kFirstEquipItem = *pkFirstItem;       // 강화석 장착 아이템 정보 넣어주기.
        kPacket_.m_kSecondEquipItem = *pkSecondItem;       // 강화석 장착 아이템 정보 넣어주기.
    }

    if( !SiKStrengthManager()->ComposeResult( kPacket_.m_dwComposeResult, (int)pkStrengthItemFirst->m_cEnchantLevel, (int)pkStrengthItemSecond->m_cEnchantLevel, nComposeResultLevel, nComposeResultGrade ) ) {
        SET_ERR_GOTO( ERR_STRENGTH_18, END_PROC );
    }

    // 강화석 합성 정보 갱신.
    kPacket_.m_kFirstStrengthItem.m_cEnchantLevel = nComposeResultLevel;
    if( kPacket_.m_kFirstStrengthItem.m_cGradeID > nComposeResultGrade ) {
        kPacket_.m_bAttributeInit = true;
    }
    kPacket_.m_kFirstStrengthItem.m_cGradeID = nComposeResultGrade;

    kPacket_.m_kSecondStrengthItem.m_cEnchantLevel = 0;

    kPacket_.m_kMaterialItem = *pkMaterialItem;

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_STRENGTH_COMPOSE_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_STRENGTH_00,      kPacket.m_nOK, 1 );     // 해당 아이템 인벤토리에 없음.
        ERR_CASE( ERR_STRENGTH_16,      kPacket.m_nOK, 2 );     // 정의되지 않은 합성 Type.
        ERR_CASE( ERR_STRENGTH_17,      kPacket.m_nOK, 3 );     // 아이템 합성 실패.
        ERR_CASE( ERR_STRENGTH_18,      kPacket.m_nOK, 4 );     // 아이템 합성 결과 적용 실패.
        ERR_CASE( ERR_STRENGTH_11,      kPacket.m_nOK, 5 );     // 강화석 DB 강화 에러.
        ERR_CASE( ERR_STRENGTH_12,      kPacket.m_nOK, 6);      // 강화석 재료 아이템 DB 삭제중 에러.
        ERR_CASE( ERR_STRENGTH_13,      kPacket.m_nOK, 7);      // 강화석 보조 아이템 DB 삭제중 에러.
        ERR_CASE( ERR_STRENGTH_25,      kPacket.m_nOK, 8);      // 기간제 아이템이면 안됨.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"강화석 합성 요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"강화석 합성 요청 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_prItemInfo.first )
        << BUILD_LOG( kPacket_.m_prItemInfo.second )
        << BUILD_LOG( kPacket_.m_dwComposeAssistUID )
        << BUILD_LOG( kPacket_.m_dwComposeResult )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_STRENGTH_COMPOSE_ACK );
    }
}

IMPL_ON_FUNC( EVENT_STRENGTH_COMPOSE_ACK )
{
    KItemPtr pkFirstStrengthItem;    // 강화석 첫번째 아이템.
    KItemPtr pkSecondStrengthItem;   // 강화석 두번째 아이템.
    KItemPtr pkFirstItem;            // 첫번째 강화석 박힌 아이템.
    KItemPtr pkSecondItem;           // 두번째 강화석 박힌 아이템.
    KItemPtr pkMaterialItem;         // 합성 주문서 아이템.

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) && !IS_CORRECT( ERR_STRENGTH_13 ) ) {
        goto END_PROC;
    }

    pkFirstStrengthItem = m_kInventory.FindItemByItemUID( kPacket_.m_kFirstStrengthItem.m_ItemUID );
    pkSecondStrengthItem = m_kInventory.FindItemByItemUID( kPacket_.m_kSecondStrengthItem.m_ItemUID );
    pkFirstItem = m_kInventory.FindItemByItemUID( kPacket_.m_kFirstStrengthItem.m_EnchantEquipItemUID );
    pkSecondItem = m_kInventory.FindItemByItemUID( kPacket_.m_kSecondStrengthItem.m_EnchantEquipItemUID );
    pkMaterialItem = m_kInventory.FindDurationItemByItemUID( kPacket_.m_kMaterialItem.m_ItemUID );

    if( pkFirstStrengthItem ) { // 강화석 합성 결과 정보 갱신.
        pkFirstStrengthItem->m_cGradeID = kPacket_.m_kFirstStrengthItem.m_cGradeID;
        pkFirstStrengthItem->m_cEnchantLevel = kPacket_.m_kFirstStrengthItem.m_cEnchantLevel;
        pkFirstStrengthItem->m_cEnchantEquipGradeID = kPacket_.m_kFirstStrengthItem.m_cGradeID;
        pkFirstStrengthItem->m_EnchantEquipItemUID = 0;
        pkFirstStrengthItem->m_cCharType = kPacket_.m_kFirstStrengthItem.m_cCharType;

        kPacket_.m_kFirstStrengthItem.m_EnchantEquipItemUID = 0;
        kPacket_.m_kFirstStrengthItem.m_cEnchantEquipGradeID = kPacket_.m_kFirstStrengthItem.m_cGradeID;
        if( kPacket_.m_bAttributeInit ) {
            pkFirstStrengthItem->m_vecAttribute.clear();
        }
    }

    // 첫번째 두번째 강화석 박힌 아이템 삭제.
    if( pkFirstItem ) {
        m_kInventory.RemoveItem( pkFirstItem->m_ItemUID );
        kPacket_.m_vecUsePeriodItems.push_back( pkFirstItem->m_ItemUID );
    }
    if( pkSecondItem ) {
        m_kInventory.RemoveItem( pkSecondItem->m_ItemUID );
        kPacket_.m_vecUsePeriodItems.push_back( pkSecondItem->m_ItemUID );
    }

    if( pkSecondStrengthItem ) { // 두번째 강화석 합성후 삭제.
        m_kInventory.RemoveItem( pkSecondStrengthItem->m_ItemUID );
        kPacket_.m_vecUsePeriodItems.push_back( pkSecondStrengthItem->m_ItemUID );
    }

    // 합성 주문서 아이템 수량 감소 처리.
    if( pkMaterialItem && pkMaterialItem->m_nCount > 0 ) {
        pkMaterialItem->m_nCount = kPacket_.m_kMaterialItem.m_nCount;
        pkMaterialItem->m_nInitCount = kPacket_.m_kMaterialItem.m_nCount;
        kPacket_.m_vecUseCountItem.push_back( *pkMaterialItem );
    }
    if ( pkMaterialItem && pkMaterialItem->m_nCount <= 0 ) { // 수량다 사용한 수량아이템 깍기.
        m_kInventory.RemoveItem( kPacket_.m_kMaterialItem.m_ItemID, kPacket_.m_kMaterialItem.m_ItemUID );
    }

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_STRENGTH_00,      kPacket_.m_nOK, 1 );     // 해당 아이템 인벤토리에 없음.
        ERR_CASE( ERR_STRENGTH_16,      kPacket_.m_nOK, 2 );     // 정의되지 않은 합성 Type.
        ERR_CASE( ERR_STRENGTH_17,      kPacket_.m_nOK, 3 );     // 아이템 합성 실패.
        ERR_CASE( ERR_STRENGTH_18,      kPacket_.m_nOK, 4 );     // 아이템 합성 결과 적용 실패.
        ERR_CASE( ERR_STRENGTH_11,      kPacket_.m_nOK, 5 );     // 강화석 DB 강화 에러.
        ERR_CASE( ERR_STRENGTH_12,      kPacket_.m_nOK, 6);      // 강화석 재료 아이템 DB 삭제중 에러.
        ERR_CASE( ERR_STRENGTH_13,      kPacket_.m_nOK, 7);      // 강화석 보조 아이템 DB 삭제중 에러.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
	    default:        START_LOG( cerr, L"강화석 합성 요청 응답 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"강화석 합성 유저 데이터 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwComposeResult )
        << BUILD_LOG( kPacket_.m_vecUseCountItem.size() )
        << BUILD_LOG( kPacket_.m_vecUsePeriodItems.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_STRENGTH_COMPOSE_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_INVEN_BUFF_ITEM_LIST_REQ )
{
    std::vector<KInvenBuffItem> vecItemList;
    SiKItemManager()->GetInvenBuffItemList( vecItemList );
    SEND_COMPRESS_PACKET( EVENT_INVEN_BUFF_ITEM_LIST_ACK, vecItemList );
}

IMPL_ON_FUNC( EVENT_SURVEY_REWARD_REQ )
{
    KEVENT_SURVEY_REWARD_ACK kPacket;
    KDB_EVENT_SURVEY_ANSWER_REQ kDBPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    // 설문 기간인가?
    if ( !SiKSurvey()->IsRun() ) {
        SET_ERR_GOTO( ERR_SURVEY_00, END_PROC );
    }

    int nLanguageCode = GetLanguageCode();

    // 존재 하는 설문 인가 확인
    if ( !SiKSurvey()->CheckValidSurvey( nLanguageCode, kPacket_.m_mapNumberAnswer, kPacket_.m_mapStringAnswer ) ) {
        SET_ERR_GOTO( ERR_SURVEY_01, END_PROC );
    }

    // 보상 리스트 존재하는 지 체크.
    if ( !SiKSurvey()->GetTypeRewardList( kPacket_.m_nType, kDBPacket.m_vecRewardItem ) ) {
        SET_ERR_GOTO( ERR_SURVEY_02, END_PROC );
    }

    if( !m_kSurveyUserData.UpdateSurveyStatus( kPacket_.m_nType, kPacket_.m_nDataFirst, kPacket_.m_nDataSecond ) ) {
        SET_ERR_GOTO( ERR_SURVEY_03, END_PROC );
    }

    kDBPacket.m_mapNumberAnswer = kPacket_.m_mapNumberAnswer;
    kDBPacket.m_mapStringAnswer = kPacket_.m_mapStringAnswer;
    kDBPacket.m_nLanguageCode   = nLanguageCode;
    kDBPacket.m_nType           = kPacket_.m_nType;
    kDBPacket.m_nDataFirst      = kPacket_.m_nDataFirst;
    kDBPacket.m_nDataSecond     = kPacket_.m_nDataSecond;

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_SURVEY_ANSWER_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_SURVEY_00,        kPacket.m_nOK, 1 );     // 설문 기간이 아님.
        ERR_CASE( ERR_SURVEY_01,        kPacket.m_nOK, 2 );     // 존재하지 않는 질문.
        ERR_CASE( ERR_SURVEY_02,        kPacket.m_nOK, 3 );     // 보상 리스트 가져오지 못함.
        ERR_CASE( ERR_SURVEY_03,        kPacket.m_nOK, 4 );     // 이미 응답한 설문임.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"설문 답변 등록 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"설문 답변 등록 요청 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nType )
        << BUILD_LOG( kPacket_.m_nDataFirst )
        << BUILD_LOG( kPacket_.m_nDataSecond )
        << BUILD_LOG( kPacket_.m_mapNumberAnswer.size() )
        << BUILD_LOG( kPacket_.m_mapStringAnswer.size() )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_SURVEY_REWARD_ACK );
    }
}

IMPL_ON_FUNC( EVENT_SURVEY_REWARD_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 유저 인벤토리 갱신.
    m_kInventory.AddItemList( kPacket_.m_vecReward );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_SURVEY_00,        kPacket_.m_nOK, 1 );     // 설문 기간이 아님.
        ERR_CASE( ERR_SURVEY_01,        kPacket_.m_nOK, 2 );     // 존재하지 않는 질문.
        ERR_CASE( ERR_SURVEY_02,        kPacket_.m_nOK, 3 );     // 보상 리스트 가져오지 못함.
        ERR_CASE( ERR_SURVEY_03,        kPacket_.m_nOK, 4 );     // 이미 응답한 설문임.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"설문 답변 등록 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"설문 답변 등록 요청 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecReward.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SURVEY_REWARD_ACK );
}

IMPL_ON_FUNC( EVENT_SURVEY_LIST_NOT )
{
    SEND_RECEIVED_PACKET( EVENT_SURVEY_LIST_NOT );
}