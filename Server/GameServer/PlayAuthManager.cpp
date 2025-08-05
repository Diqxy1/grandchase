#include "User.h"
#include "GSSimLayer.h"
#include "PlayAuthManager.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "UserEvent.h"
#include "Log4.h"
#include "StatDisconnect.h"

ImplementSingleton( KPlayAuthManager );
ImplOstmOperatorW2A( KPlayAuthManager );
NiImplementRootRTTI( KPlayAuthManager );

KPlayAuthManager::KPlayAuthManager( void )
{
    m_hCon = NULL;
    m_mapOnlineUserInfo.clear();

    m_bIsConnected = false;
    m_dwAutoReconnectLastTick = 0;

    m_bUsePlayAuthSystem = false;
    m_bAutoReconnect = true;
    m_dwAutoReconnectGap = 60000;
}

KPlayAuthManager::~KPlayAuthManager( void )
{
}

ImplToStringW( KPlayAuthManager )
{
    KLocker lock( m_csOnlineUserInfo );

    return START_TOSTRINGW
        << TOSTRINGW( m_mapOnlineUserInfo.size() )
        << TOSTRINGAb( m_bIsConnected )
        << TOSTRINGW( m_dwAutoReconnectLastTick )
        << TOSTRINGAb( m_bUsePlayAuthSystem )
        << TOSTRINGAb( m_bAutoReconnect )
        << TOSTRINGW( m_dwAutoReconnectGap );
}

void __stdcall	_OnConnectedCallback( void* context, const char* ip, unsigned short port )
{
    if ( NULL == context )
    {
        return;
    }
    SiKPlayAuthManager()->DoOnConnectedCallback( std::string( ip ), port );
}

void __stdcall	_OnDisconnectedCallback( void* context, const char* ip, unsigned short port )
{
    if ( NULL == context )
    {
        return;
    }
    SiKPlayAuthManager()->DoOnDisconnectedCallback( std::string( ip ), port );
}

void __stdcall	_OnUserStateNotiCallback( void* context, const int nNotiType, const PlayAuth::UserNotiInfo& rUserNotiInfo )
{
    if ( NULL == context )
    {
        return;
    }
    SiKPlayAuthManager()->DoOnUserStateNotiCallback( nNotiType, rUserNotiInfo );
}

void __stdcall	_OnUserAliveCheckCallback( void* context, const char* szUniqueID, const char* szTicket, bool& bIsOnline )
{
    if ( NULL == context )
    {
        return;
    }
    SiKPlayAuthManager()->DoOnUserAliveCheckCallback( std::string( szUniqueID ), std::string( szTicket ), bIsOnline );
}

void __stdcall	_OnCheckInCallback(void* context, int errorCode, char const* szUniqueID, unsigned long ulUserIP, const char* szTicket, PlayAuth::UserCheckInInfo const & rCheckInInfo )
{
    if ( NULL == context )
    {
        return;
    }
    SiKPlayAuthManager()->DoOnCheckInCallback( errorCode, std::string( szUniqueID ), ulUserIP, std::string( szTicket ), rCheckInInfo );
}

void __stdcall	_OnCheckOutCallback(void* context, int errorCode, const char* szUniqueID, const char* szTicket)
{
    if ( NULL == context )
    {
        return;
    }
    SiKPlayAuthManager()->DoOnCheckOutCallback( errorCode, std::string( szUniqueID ), std::string( szTicket ) );
}

bool KPlayAuthManager::Initialize()
{
    // Connector handle생성
    m_hCon = PlayAuth::Connector::CreateHandle();
    if ( NULL == m_hCon )
    {
        int nErrorCode = PlayAuth::Connector::GetLastError();
        START_LOG( cerr, L"셧다운 시스템 핸들 생성 실패. errorCode: " << nErrorCode ) << END_LOG;

        return false;
    }

    // 셧다운시스템으로 connect/disconnect시에 callback설정
    if ( false == PlayAuth::Connector::SetOnConnected( m_hCon, _OnConnectedCallback, this ) )
    {
        START_LOG( cerr, L"셧다운 시스템 connected 콜백 등록 실패" ) << END_LOG;
        return false;
    }
    if ( false == PlayAuth::Connector::SetOnDisconnected( m_hCon, _OnDisconnectedCallback, this) )
    {
        START_LOG( cerr, L"셧다운 시스템 disconnected 콜백 등록 실패" ) << END_LOG;
        return false;
    }
    // 셧다운시스템으로부터의 노티(유저 셧다운요청, 잔여시간정보)처리 callback등록
    if ( false == PlayAuth::Connector::SetOnUserStateNoti( m_hCon, _OnUserStateNotiCallback, this) )
    {
        START_LOG( cerr, L"셧다운 시스템 user state noti 콜백 등록 실패" ) << END_LOG;
        return false;
    }
    // 유저가 실제로 게임서버에 있는지 체크 callback등록
    if ( false == PlayAuth::Connector::SetOnUserAliveCheck( m_hCon, _OnUserAliveCheckCallback, this) )
    {
        START_LOG( cerr, L"셧다운 시스템 user alive check 콜백 등록 실패" ) << END_LOG;
        return false;
    }

    m_dwAutoReconnectLastTick = ::GetTickCount();

    // 셧다운 시스템에 정상적인 연결성공시 true
    if ( false == PlayAuth::Connector::Start( m_hCon ) )
    {
        START_LOG( cerr, L"셧다운 시스템 연결 실패" ) << END_LOG;
        return false;
    }
    START_LOG( cerr, L"셧다운 시스템 시작 성공" ) << END_LOG;

    return true;
}

void KPlayAuthManager::Uninitialize()
{
    if ( NULL != m_hCon )
    {
        PlayAuth::Connector::Stop( m_hCon );
        PlayAuth::Connector::DestroyHandle( m_hCon );
        m_hCon = NULL;

        START_LOG( cerr, L"셧다운 시스템 종료 성공" ) << END_LOG;
    }
}

bool KPlayAuthManager::CheckIn( IN const std::string& strUniqueID_, IN const unsigned long& ulUserIP_, IN const DWORD& dwUID_, IN const int& nAge_ )
{
    KOnlineUserInfo kOnlineUserInfo;
    kOnlineUserInfo.m_strUniqueID = strUniqueID_;
    kOnlineUserInfo.m_dwUID = dwUID_;
    kOnlineUserInfo.m_strTicket = "";

    AddToOnlineUser( kOnlineUserInfo );

    START_LOG( clog, L"셧다운 시스템 체크인 요청" )
        << BUILD_LOG( strUniqueID_.c_str() )
        << BUILD_LOG( dwUID_ )
        << END_LOG;

    return PlayAuth::Connector::CheckIn( m_hCon, strUniqueID_.c_str(), ulUserIP_, nAge_, _OnCheckInCallback, this );
}

bool KPlayAuthManager::CheckOut( IN const std::string& strUniqueID_, IN const std::string& strTicket_ )
{
    START_LOG( clog, L"셧다운 시스템 체크아웃 요청" ) << END_LOG;

    return PlayAuth::Connector::CheckOut( m_hCon, strUniqueID_.c_str(), strTicket_.c_str(), _OnCheckOutCallback, this );
}

void KPlayAuthManager::DoOnConnectedCallback( IN const std::string& strIP_, IN const unsigned short& usPort_ )
{
    m_bIsConnected = true;
    START_LOG( cerr, L"셧다운 시스템 연결 성공. IP: " << KncUtil::toWideString( strIP_ ) << ", PORT: " << usPort_ ) << END_LOG;
}

void KPlayAuthManager::DoOnDisconnectedCallback( IN const std::string& strIP_, IN const unsigned short& usPort_ )
{
    m_bIsConnected = false;
    START_LOG( cerr, L"셧다운 시스템 연결 해제 성공. IP: " << KncUtil::toWideString( strIP_ ) << ", PORT: " << usPort_ ) << END_LOG;
}

void KPlayAuthManager::DoOnCheckInCallback( IN const int& nErrorCode_, IN const std::string& strUniqueID_, IN const unsigned long& ulUserIP_, IN const std::string& strTicket_, IN const PlayAuth::UserCheckInInfo& kUserCheckInInfo_ )
{
    // CheckIn에 대한 로직처리
    if ( PLAY_AUTH_SUCCESS != nErrorCode_ )
    {
        // 시스템오류에 의한 checkin 실패
        // 자세한 원인은 errorCode코드값으로 확인해봐야함
         START_LOG( clog, L"셧다운 시스템 오류로 체크인 실패" ) << END_LOG;
    }

    if ( false == kUserCheckInInfo_.m_bCheckInSuccess )
    {
        // 셧다운제에 걸려서 checkin 실패
        // 유저 접속 해제 처리를 한다
        KOnlineUserInfo kOnlineUserInfo;
        if ( true == GetOnlineUserInfo( strUniqueID_, kOnlineUserInfo ) ) {
            KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kOnlineUserInfo.m_dwUID ) );
            if ( spUser == NULL ) {
                START_LOG( clog, L"셧다운 시스템 게임 서버에서 이미 접속 해제된 유저" ) << END_LOG;
            }
            else {
                // 셧다운제 체크인 결과 패킷 전송
                KEVENT_SHUTDOWN_CHECKIN_INFO_NOT kPacket;
                GetCheckInInfo( kUserCheckInInfo_, kPacket );
                // 유저에게 패킷 보내고
                SiKGSSimLayer()->m_kActorManager.SendTo( kOnlineUserInfo.m_dwUID, KUserEvent::EVENT_SHUTDOWN_CHECKIN_INFO_NOT, kPacket );
                // 체크인 실패니 내부 리스트에서도 제거
                RemoveFromOnlineUser( strUniqueID_ );

                spUser->SetDisconnReason( KStatDisconnect::DRW_SHUTDOWN_USER );
            }
            START_LOG( clog, L"셧다운 시스템 체크인 실패(셧다운제에 걸려 접속 못함)" ) << END_LOG;
        }
    }
    else
    {
        // checkin 성공처리
        // 발급된 ticket은 저장해두었다가 차후 checkout 할때 입력해줘야함
        // 유저의 티켓 정보를 저장 해둔다
        if ( true == UpdateOnlineUserTicket( strUniqueID_, strTicket_ ) ) {
            START_LOG( clog, L"셧다운 시스템 체크인 성공" ) << END_LOG;
        }
    }

    START_LOG( clog, L"셧다운 시스템 체크인 콜백 처리 완료" )
        << BUILD_LOG( nErrorCode_ )
        << BUILD_LOG( kUserCheckInInfo_.m_bCheckInSuccess )
        << BUILD_LOG( strUniqueID_.c_str() )
        << BUILD_LOG( ulUserIP_ )
        << BUILD_LOG( strTicket_.c_str() )
        << END_LOG;
}

void KPlayAuthManager::DoOnCheckOutCallback( IN const int& nErrorCode_, IN const std::string& strUniqueID_, IN const std::string& strTicket_ )
{
    // checkout의 경우에는 별다른 에러체크는 필요없을듯
    if ( PLAY_AUTH_SUCCESS != nErrorCode_ )
    {
        START_LOG( clog, L"셧다운 시스템 체크아웃 콜백 처리 실패" ) << END_LOG;
        // 시스템오류에 의한 checkout 실패
        // errorCode 에대한 로그를 남겨서 원인파악은 해야할듯
    }

    START_LOG( clog, L"셧다운 시스템 체크아웃 콜백 처리 완료" )
        << BUILD_LOG( nErrorCode_ )
        << BUILD_LOG( strUniqueID_.c_str() )
        << BUILD_LOG( strTicket_.c_str() )
        << END_LOG;
}

void KPlayAuthManager::DoOnUserStateNotiCallback( IN const int& nNotiType_, IN const PlayAuth::UserNotiInfo& kUserNotiInfo_ )
{
    START_LOG( clog, L"셧다운 시스템 유저 상태 노티 정보 받음" )
        << BUILD_LOG( nNotiType_ )
        << BUILD_LOG( kUserNotiInfo_.m_nNotiType )
        << BUILD_LOG( kUserNotiInfo_.m_szUniqueID )
        << BUILD_LOG( kUserNotiInfo_.m_szGameCode )
        << BUILD_LOG( kUserNotiInfo_.m_szTicket )
        << BUILD_LOG( kUserNotiInfo_.m_szShutdownName )
        << BUILD_LOG( kUserNotiInfo_.m_nRemainTime )
        << END_LOG;

    KOnlineUserInfo kOnlineUserInfo;
    GetOnlineUserInfo( std::string( kUserNotiInfo_.m_szUniqueID ), kOnlineUserInfo );

    KEVENT_SHUTDOWN_CHECKOUT_INFO_NOT kPacket;
    kPacket.m_uiNotiType = kUserNotiInfo_.m_nNotiType;
    kPacket.m_uiRemainTime = kUserNotiInfo_.m_nRemainTime;

    if ( strcmp( "ForcedShutdownSystem", kUserNotiInfo_.m_szShutdownName ) == 0 ) {
        kPacket.m_uiShutdownKind = KShutdownInfo::FORCED_SHUTDOWN_SYSTEM;
    }
    else if ( strcmp( "SelectiveShutdownSystem", kUserNotiInfo_.m_szShutdownName ) == 0 ) {
        kPacket.m_uiShutdownKind = KShutdownInfo::SELECTIVE_SHUTDOWN_SYSTEM;
    }
    else if ( strcmp( "CoolingOffSystem", kUserNotiInfo_.m_szShutdownName ) == 0 ) {
        kPacket.m_uiShutdownKind = KShutdownInfo::COOLING_OFF_SYSTEM;
    }

    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kOnlineUserInfo.m_dwUID ) );
    if ( spUser == NULL ) {
        START_LOG( clog, L"셧다운 시스템 게임 서버에서 이미 접속 해제된 유저" ) << END_LOG;
    }
    else {
        // 셧다운제 체크아웃 정보 패킷 전송
        SiKGSSimLayer()->m_kActorManager.SendTo( kOnlineUserInfo.m_dwUID, KUserEvent::EVENT_SHUTDOWN_CHECKOUT_INFO_NOT, kPacket );
        spUser->SetDisconnReason( KStatDisconnect::DRW_SHUTDOWN_USER );
    }

    //switch ( nNotiType_ ) {
    //    case KEVENT_SHUTDOWN_CHECKOUT_INFO_NOT::CHECKOUT_REQ:
    //        // 유저 강제 체크아웃 처리
    //        break;

    //    case KEVENT_SHUTDOWN_CHECKOUT_INFO_NOT::REMAIN_TIME_NOT:
    //        // 유저 잔여시간 알림
    //        // kUserNotiInfo_.m_nRemainTime : 잔여시간
    //        // kUserNotiInfo_.m_strShutdownName : 셧다운제이름
    //        break;
    //    default:
    //        START_LOG( cerr, L"셧다운 시스템 정의되지 않은 NotiType: " ) << nNotiType_
    //            << END_LOG;
    //        break;
    //}
}

void KPlayAuthManager::DoOnUserAliveCheckCallback( IN const std::string& strUniqueID_, IN const std::string& strTicket_, OUT bool& bIsOnline )
{
    // uniqueid와 ticket으로 해당 유저가 현재게임서버에 존재하는지 체크
    // 만약 존재하지않는 유저라면, bIsOnline 에 false를 할당해야함.
    // bIsOnline에 false를 할당하면, 셧다운제시스템에서 해당유저를 더미유저로 체크하여서 시스템에서 체크아웃처리함.

    KOnlineUserInfo kOnlineUserInfo;
    GetOnlineUserInfo( strUniqueID_, kOnlineUserInfo );

    if ( strUniqueID_ == kOnlineUserInfo.m_strUniqueID && strTicket_ == kOnlineUserInfo.m_strTicket ) {
        bIsOnline = true;
        START_LOG( clog, L"셧다운 시스템 접속중인지 확인 콜백: 접속중임" ) << END_LOG;
    }
    else {
        bIsOnline = false;
        START_LOG( clog, L"셧다운 시스템 접속중인지 확인 콜백: 접속중이지 않음" ) << END_LOG;
    }
    START_LOG( clog, L"셧다운 시스템 접속중인지 확인 정보" )
        << BUILD_LOG( strUniqueID_.c_str() )
        << BUILD_LOG( kOnlineUserInfo.m_strUniqueID.c_str() )
        << BUILD_LOG( strTicket_.c_str() )
        << BUILD_LOG( kOnlineUserInfo.m_strTicket.c_str() )
        << END_LOG;
}

bool KPlayAuthManager::AddToOnlineUser( IN const KOnlineUserInfo& kOnlineUserInfo_ )
{
    KLocker lock( m_csOnlineUserInfo );

    if ( false == m_mapOnlineUserInfo.insert( std::make_pair( kOnlineUserInfo_.m_strUniqueID, kOnlineUserInfo_ ) ).second ) {
        START_LOG( cerr, L"셧다운 시스템 온라인 유저 등록 실패(uniqueID가 중복됨)" ) << END_LOG;
        return false;
    }
    return true;
}

bool KPlayAuthManager::RemoveFromOnlineUser( IN const std::string& strUniqueID_ )
{
    KLocker lock( m_csOnlineUserInfo );

    std::map< std::string, KOnlineUserInfo >::iterator mit;
    mit = m_mapOnlineUserInfo.find( strUniqueID_ );
    if ( mit == m_mapOnlineUserInfo.end() ) {
        START_LOG( cerr, L"셧다운 시스템 온라인 유저 제거 실패(uniqueID를 찾을 수 없음)" ) << END_LOG;
        return false;
    }

    // 셧다운 시스템에 체크아웃을 하고
    if ( mit->second.m_strTicket != "" ) {
        // 티켓 정보가 없으면 자체 리스트에서만 제거한다
        CheckOut( strUniqueID_, mit->second.m_strTicket );
    }

    // 별도로 유지하는 유저 리스트에서 제거한다
    m_mapOnlineUserInfo.erase( mit );

    return true;
}

bool KPlayAuthManager::UpdateOnlineUserTicket( IN const std::string& strUniqueID_, IN const std::string& strTicket_ )
{
    KLocker lock( m_csOnlineUserInfo );

    std::map< std::string, KOnlineUserInfo >::iterator mit;
    mit = m_mapOnlineUserInfo.find( strUniqueID_ );
    if ( mit == m_mapOnlineUserInfo.end() ) {
        START_LOG( cerr, L"셧다운 시스템 온라인 유저 티켓 갱신 실패(uniqueID를 찾을 수 없음)" ) << END_LOG;
        return false;
    }

    mit->second.m_strTicket = strTicket_;

    return true;
}

bool KPlayAuthManager::GetOnlineUserInfo( IN const std::string& strUniqueID_, OUT KOnlineUserInfo& kOnlineUserInfo_ )
{
    KLocker lock( m_csOnlineUserInfo );

    std::map< std::string, KOnlineUserInfo >::iterator mit;
    mit = m_mapOnlineUserInfo.find( strUniqueID_ );
    if ( mit == m_mapOnlineUserInfo.end() ) {
        START_LOG( cerr, L"셧다운 시스템 온라인 유저 검색 실패(uniqueID를 찾을 수 없음)" ) << END_LOG;
        return false;
    }

    kOnlineUserInfo_ = mit->second;

    return true;
}

void KPlayAuthManager::GetCheckInInfo( IN const PlayAuth::UserCheckInInfo& kUserCheckInInfo_, OUT KEVENT_SHUTDOWN_CHECKIN_INFO_NOT& kPacket_ )
{
    kPacket_.m_bCheckInSuccess = kUserCheckInInfo_.m_bCheckInSuccess;

    for ( unsigned int i = 0; i < kUserCheckInInfo_.m_nShutdownInfoListSize; ++i ) {
        KShutdownInfo kShutdownInfo;
        if ( strcmp( "ForcedShutdownSystem", kUserCheckInInfo_.m_listShutdownInfoList[ i ].m_szShutdownName ) == 0 ) {
            kShutdownInfo.m_uiShutdownKind = KShutdownInfo::FORCED_SHUTDOWN_SYSTEM;
        }
        else if ( strcmp( "SelectiveShutdownSystem", kUserCheckInInfo_.m_listShutdownInfoList[ i ].m_szShutdownName ) == 0 ) {
            kShutdownInfo.m_uiShutdownKind = KShutdownInfo::SELECTIVE_SHUTDOWN_SYSTEM;
        }
        else if ( strcmp( "CoolingOffSystem", kUserCheckInInfo_.m_listShutdownInfoList[ i ].m_szShutdownName ) == 0 ) {
            kShutdownInfo.m_uiShutdownKind = KShutdownInfo::COOLING_OFF_SYSTEM;
        }
        else {
            START_LOG( cerr, L"셧다운 시스템 잘못된 셧다운제 정보" )
                << BUILD_LOG( kUserCheckInInfo_.m_listShutdownInfoList[ i ].m_szShutdownName )
                << END_LOG;
        }

        kShutdownInfo.m_nShutdownCheckResult = kUserCheckInInfo_.m_listShutdownInfoList[ i ].m_nShutdownCheckResult;

        kPacket_.m_vecShutdownInfo.push_back( kShutdownInfo );
    }
}

void KPlayAuthManager::Tick()
{
    if ( false == m_bUsePlayAuthSystem || false == m_bAutoReconnect || true == m_bIsConnected ) {
        return;
    }

    if ( ::GetTickCount() - m_dwAutoReconnectLastTick <= m_dwAutoReconnectGap ) {
        return;
    }
    m_dwAutoReconnectLastTick = ::GetTickCount();

    // 셧다운 시스템에 정상적인 연결성공시 true
    if ( false == PlayAuth::Connector::Start( m_hCon ) )
    {
        START_LOG( cerr, L"셧다운 시스템 연결 실패(Tick)" ) << END_LOG;
        return;
    }
    START_LOG( cerr, L"셧다운 시스템 시작 성공(Tick)" ) << END_LOG;
}
