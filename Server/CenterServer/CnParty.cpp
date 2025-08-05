#include "Center.h"
#include "CnParty.h"
#include "CnPartyManager.h"
#include <algorithm>
#include <boost/bind.hpp>
#include "Log4.h"
NiImplementRTTI( KCnParty, KPerformer );
#define EVENT_TYPE KCenterEvent
#define CLASS_TYPE KCnParty

//FILE_NAME_FOR_LOG

namespace
{
    class KSortHost
    {
    public:
        bool operator()( KPartyMember member_ ) {
            return member_.m_IsHost;
        }
    };
}

ImplToStringW( KCnParty )
{
    return START_TOSTRING_PARENTW( KPerformer );
}

KCnParty::KCnParty(void)
{
}

KCnParty::~KCnParty(void)
{
}

void KCnParty::SetHost( IN KPartyMember& kMaster_, IN KCenterPtr spCenter_ )
{

    m_kPartyInfo.m_vecPartyPeople.push_back( kMaster_ );
    AddServer( spCenter_ );

    START_LOG( clog, L"파티장 설정 , User UID : " << kMaster_.m_dwUID )
        << BUILD_LOG( kMaster_.m_strNickName )
        << BUILD_LOG( GetUID() ) << END_LOG;
}

void KCnParty::ProcessEvent( const KEventPtr& spEvent_ )
{

}

void KCnParty::Tick()
{

}

void KCnParty::AddServer( IN KCenterPtr spCenter_ )
{
    KLocker lock( m_csServers );

    std::map<DWORD,std::pair<KCenterPtr,int>>::iterator mitServer = m_mapServers.find( spCenter_->GetUID() );
    // 유저 접속한 서버의 길드원 카운트값 증가
    if ( mitServer == m_mapServers.end() ) {
        mitServer = m_mapServers.insert(std::map<DWORD,std::pair<KCenterPtr,int>>::value_type(spCenter_->GetUID(),std::pair<KCenterPtr,int>(spCenter_,0)) ).first;
    }

    ++(mitServer->second.second);
    START_LOG( clog, L"서버 파티원 수 증가. Party UID : " << m_kPartyInfo.m_dwPartyUID << L",ServerUID : " << mitServer->first << L",UserNum : " << mitServer->second.second ) << END_LOG;
}

void KCnParty::RemoveServer( IN const DWORD dwUID_ )
{
    KLocker lock( m_csServers );

    std::map<DWORD,std::pair<KCenterPtr,int>>::iterator mitServer = m_mapServers.find( dwUID_ );
    if ( mitServer == m_mapServers.end() ) {
        // 이미 처리된 경우.
        START_LOG( cwarn, L"없는 서버정보(=해당 서버UID의 서버 포인터 없음). ServerUID : " << dwUID_ ) << END_LOG;
        return;
    }

    // 나간 유저가 있던 서버의 길드 맴버수 카운트를 감소
    --(mitServer->second.second);
    START_LOG( clog, L"서버 파티원 수 감소. Party UID : " << m_kPartyInfo.m_dwPartyUID << L",ServerUID : " << dwUID_ << L",Member Num :" << mitServer->second.second ) << END_LOG;

    // 모든 유저가 나간 서버는 제거
    if ( mitServer->second.second <= 0 ) {
        m_mapServers.erase( mitServer );
        START_LOG( clog, L"파티원이 없는 서버정보 제거. PartyUID : " << m_kPartyInfo.m_dwPartyUID  << L",ServerUID : " << dwUID_ ) << END_LOG;
    }
}


//void KCnParty::AddMember( IN DWORD dwUserUID_ )
//{
//    KLocker lock( m_csPartyInfo );
//
//    KPartyMember kMember;
//    kMember.m_dwUID = dwUserUID_;
//    m_kPartyInfo.m_vecPartyPeople.push_back( kMember );
//
//    START_LOG( clog, L"AddMember in Party, UserUID : " << dwUserUID_ )
//        << BUILD_LOG( m_kPartyInfo.m_vecPartyPeople.size() ) << END_LOG;
//}
//
//void KCnParty::AddMember( IN DWORD dwUserUID_, IN KCenterPtr spCenter_ )
//{
//    KLocker lock( m_csPartyInfo );
//
//    KPartyMember kMember;
//    kMember.m_dwUID = dwUserUID_;
//
//    m_kPartyInfo.m_vecPartyPeople.push_back( kMember );
//
//    AddServer( spCenter_ );
//
//    START_LOG( clog, L"AddMember in Party, UserUID : " << dwUserUID_ )
//        << BUILD_LOG( m_kPartyInfo.m_vecPartyPeople.size() ) << END_LOG;
//}

void KCnParty::AddMember( IN std::vector<KPartyMember>& vecUser_ )
{
    if ( vecUser_.empty() ) {
        return;
    }

    KLocker lock( m_csPartyInfo );

    m_kPartyInfo.m_vecPartyPeople.insert( m_kPartyInfo.m_vecPartyPeople.end(), vecUser_.begin(), vecUser_.end() );
 
    START_LOG( clog, L"AddMembers in Party " )
        << BUILD_LOG( m_kPartyInfo.m_vecPartyPeople.size() ) << END_LOG;
}

void KCnParty::AddMember( IN KPartyMember& kUser_, IN KCenterPtr spCenter_ )
{
    KLocker lock( m_csPartyInfo );

    m_kPartyInfo.m_vecPartyPeople.push_back( kUser_ );

    AddServer( spCenter_ );

    START_LOG( clog, L"AddMember in Party, UserUID : " << kUser_.m_dwUID )
        << BUILD_LOG( m_kPartyInfo.m_vecPartyPeople.size() ) << END_LOG;
}


bool KCnParty::SetNextHost( IN DWORD dwUID_ )
{
    KLocker lock( m_csPartyInfo );

    if ( m_kPartyInfo.m_vecPartyPeople.empty() ) {
        START_LOG( cerr, L"파티원이 없음, UserUID : " << dwUID_ ) << END_LOG;
        return false;
    }

    START_LOG( clog, L"파티장 변경 전, Party UID : " << m_kPartyInfo.m_dwPartyUID )
        << BUILD_LOG( m_kPartyInfo.m_vecPartyPeople.front().m_IsHost )
        << BUILD_LOG( m_kPartyInfo.m_vecPartyPeople.front().m_strNickName ) << END_LOG;

    m_kPartyInfo.m_vecPartyPeople.front().m_IsHost = false; // 기존 파티장 -> 직위 해제

    START_LOG( clog, L"파티장 변경 후, Party UID : " << m_kPartyInfo.m_dwPartyUID )
        << BUILD_LOG( m_kPartyInfo.m_vecPartyPeople.front().m_IsHost )
        << BUILD_LOG( m_kPartyInfo.m_vecPartyPeople.front().m_strNickName ) << END_LOG;

    std::vector<KPartyMember>::iterator vit;
    vit = std::find_if( m_kPartyInfo.m_vecPartyPeople.begin(), m_kPartyInfo.m_vecPartyPeople.end(),
        boost::bind( &KPartyMember::m_dwUID, _1 ) == dwUID_ );

    if ( vit != m_kPartyInfo.m_vecPartyPeople.end() ) {
        vit->m_IsHost = true;

        START_LOG( clog, L"새로운 파티장 , UserUID : " << vit->m_strNickName )
            << BUILD_LOG( vit->m_IsHost ) << END_LOG;
    }
    else {
        START_LOG( cerr, L"파티장 위임할 파티원 없음, Party UID : " << dwUID_ ) << END_LOG;
        return false;
    }

    std::stable_partition( m_kPartyInfo.m_vecPartyPeople.begin() ,m_kPartyInfo.m_vecPartyPeople.end(), KSortHost() );

    SendToAll( KCenterEvent::ECN_CHANGE_PARTY_HOST_BROAD, m_kPartyInfo );

    return true;
}

bool KCnParty::DeleteUser( IN KPartyMember& kData_ )
{
    KLocker lock( m_csPartyInfo );

    std::vector<KPartyMember>::iterator vit;
    vit = std::find_if( m_kPartyInfo.m_vecPartyPeople.begin(), m_kPartyInfo.m_vecPartyPeople.end(),
        boost::bind( &KPartyMember::m_dwUID, _1 ) == kData_.m_dwUID );

    if ( vit == m_kPartyInfo.m_vecPartyPeople.end() ) {
        START_LOG( cerr, L"파티에 삭제할 유저 없음, UserUID : " << kData_.m_dwUID )
            << BUILD_LOG( m_kPartyInfo.m_dwPartyUID ) << END_LOG;
        return false;
    }

    KPartyData kPacket;
    kPacket = m_kPartyInfo;
    kPacket.m_nOK = 0;

    DWORD dwServerUID = vit->m_ServerUID; // m_kPartyInfo.m_vecPartyPeople.erase( vit );  아래 코드보다 위에 있어야 함.
    m_kPartyInfo.m_vecPartyPeople.erase( vit );

    if ( m_kPartyInfo.m_vecPartyPeople.size() >= MINIMUM_PARTY_MEMBER ) {
        SendToAll( KCenterEvent::ECN_LEAVE_PARTY_BROAD, m_kPartyInfo );
        SetNextHost();
    }
    else {

        SendToAll( KCenterEvent::ECN_BREAK_PARTY_NOT, kPacket );
    }

    RemoveServer( dwServerUID );

    START_LOG( clog, L"파티 유저 삭제, UserUID : " << kData_.m_dwUID )
        << BUILD_LOG( kData_.m_strNickName )
        << BUILD_LOG( dwServerUID )
        << BUILD_LOG( m_kPartyInfo.m_dwPartyUID )
        << BUILD_LOG( m_kPartyInfo.m_vecPartyPeople.size() ) << END_LOG;

    return true;
}


bool KCnParty::SetNextHost()
{
    if ( m_kPartyInfo.m_vecPartyPeople.empty() ) {
        return false;
    }
    
    // lock 은 DeleteUser에서 걸고 들어옴.
    std::vector<KPartyMember>::iterator vit;
    vit = std::find_if( m_kPartyInfo.m_vecPartyPeople.begin(), m_kPartyInfo.m_vecPartyPeople.end(),
        boost::bind( &KPartyMember::m_IsHost, _1 ) == true );

    if ( vit == m_kPartyInfo.m_vecPartyPeople.end() ) { // 방장이 없으면 가장 앞에 있는 유저가 방장이 됨
        m_kPartyInfo.m_vecPartyPeople.front().m_IsHost = true;
        m_kPartyInfo.m_vecPartyPeople.front().m_dwState = KPartyMember::US_READY;
    }
    else {
        return false;
    }

    std::stable_partition( m_kPartyInfo.m_vecPartyPeople.begin() , m_kPartyInfo.m_vecPartyPeople.end(), KSortHost() );
    
    SendToAll( KCenterEvent::ECN_CHANGE_PARTY_HOST_BROAD, m_kPartyInfo );
    
    return true;
}

void KCnParty::PrintPartyUserList()
{
    KLocker lock( m_csPartyInfo );

    START_LOG( cerr, L"Party User List, Party UID : " << GetUID()  ) << END_LOG;
    
    std::vector<KPartyMember>::iterator vit;
    
    for ( vit = m_kPartyInfo.m_vecPartyPeople.begin(); vit != m_kPartyInfo.m_vecPartyPeople.end(); ++vit ) {
        START_LOG( cerr, L"UserUID : " << vit->m_dwUID << L", Nick : " << vit->m_strNickName 
            << L", Host : " << vit->m_IsHost << L", Server UID : " << vit->m_ServerUID ) << END_LOG;
    }

    std::map<DWORD,std::pair<KCenterPtr,int> >::const_iterator mitServer;
    
    for ( mitServer = m_mapServers.begin(); mitServer != m_mapServers.end(); ++mitServer ) {
        START_LOG( cerr, L"Server UID : " << mitServer->first << L" , UserNum : " << mitServer->second.second  ) << END_LOG;
    }
    
}

void KCnParty::SetMemberNickName( IN KPartyMember& kData_ )
{
    std::vector<KPartyMember>::iterator vit;

    vit = std::find_if( m_kPartyInfo.m_vecPartyPeople.begin(), m_kPartyInfo.m_vecPartyPeople.end(),
        boost::bind( &KPartyMember::m_dwUID, _1 ) == kData_.m_dwUID );

    if ( vit != m_kPartyInfo.m_vecPartyPeople.end() ) {
        vit->m_strNickName = kData_.m_strNickName;
        START_LOG( clog, L"파티 초대받은 유저의 닉네임 설정 성공" << kData_.m_strNickName )
            << BUILD_LOG( GetUID() ) << END_LOG;
    }
    else {
        START_LOG( cerr, L"파티 초대받은 유저의 닉네임 설정 실패" << kData_.m_strNickName )
            << BUILD_LOG( GetUID() ) << END_LOG;
    }

}

void KCnParty::SetMemberServerUID( IN KPartyMember& kData_ )
{
    std::vector<KPartyMember>::iterator vit;

    vit = std::find_if( m_kPartyInfo.m_vecPartyPeople.begin(), m_kPartyInfo.m_vecPartyPeople.end(),
        boost::bind( &KPartyMember::m_dwUID, _1 ) == kData_.m_dwUID );

    if ( vit != m_kPartyInfo.m_vecPartyPeople.end() ) {
        vit->m_ServerUID = kData_.m_ServerUID;
        START_LOG( clog, L"파티 초대받은 유저의 서버UID 설정 성공" << kData_.m_ServerUID )
            << BUILD_LOG( GetUID() ) << END_LOG;
    }
    else {
        START_LOG( cerr, L"파티 초대받은 유저의 서버UID 설정 실패" << kData_.m_ServerUID )
            << BUILD_LOG( GetUID() ) << END_LOG;
    }
}

bool KCnParty::IsExistMember( IN KPartyMember& kData_ )
{
    bool ret = false;
    std::vector<KPartyMember>::iterator vit;

    vit = std::find_if( m_kPartyInfo.m_vecPartyPeople.begin(), m_kPartyInfo.m_vecPartyPeople.end(),
        boost::bind( &KPartyMember::m_dwUID, _1 ) == kData_.m_dwUID );

    if ( vit != m_kPartyInfo.m_vecPartyPeople.end() ) {
        ret = true; // 이미 유저가 존재
    }
    else {
        ret = false; // 유저 없음
    }

    return ret;
}

bool KCnParty::ChangeUserState( IN KPartyMember& kData_ )
{
    KLocker lock( m_csPartyInfo );

    std::vector<KPartyMember>::iterator vit;
    vit = std::find_if( m_kPartyInfo.m_vecPartyPeople.begin(), m_kPartyInfo.m_vecPartyPeople.end(),
        boost::bind( &KPartyMember::m_dwUID, _1 ) == kData_.m_dwUID );

    if ( vit == m_kPartyInfo.m_vecPartyPeople.end() ) {
        START_LOG( cerr, L"파티에 상태 변경할 유저 없음, UserUID : " << kData_.m_dwUID )
            << BUILD_LOG( m_kPartyInfo.m_dwPartyUID ) << END_LOG;
        return false;
    }

    // 상태값 변경.
    vit->m_dwState = kData_.m_dwState;
    vit->m_cCharType = kData_.m_cCharType;

    KPartyData kPacket;
    kPacket = m_kPartyInfo;
    kPacket.m_nOK = 0;
    SendToAll( KCenterEvent::ECN_PARTY_USER_READY_BROAD, m_kPartyInfo );

    START_LOG( clog, L"파티 유저 상태 변경, UserUID : " << kData_.m_dwUID )
        << BUILD_LOG( kData_.m_strNickName )
        << BUILD_LOG( m_kPartyInfo.m_dwPartyUID )
        << BUILD_LOG( kData_.m_dwState )
        << BUILD_LOGc( kData_.m_cCharType )
        << BUILD_LOG( m_kPartyInfo.m_vecPartyPeople.size() )
        << END_LOG;

    return true;
}

void KCnParty::SetPartyType( IN const DWORD& dwPartyType_ )
{
    KLocker lock( m_csPartyInfo );

    m_kPartyInfo.m_dwPartyType = dwPartyType_;
}