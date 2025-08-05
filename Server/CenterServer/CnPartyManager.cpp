#include "Center.h"
#include "CnPartyManager.h"
#include <dbg/dbg.hpp>

#include "CenterEvent.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCnPartyManager );
NiImplementRootRTTI( KCnPartyManager );
ImplOstmOperatorW2A( KCnPartyManager );

ImplToStringW( KCnPartyManager )
{
    START_TOSTRINGW 
        << TOSTRINGW( m_mapParty.size() )
        << TOSTRINGW( m_dwUID );

    return stm_;
}

KCnPartyManager::KCnPartyManager(void)
:m_dwUID(0)
{
    
    
}

KCnPartyManager::~KCnPartyManager(void)
{

}

bool KCnPartyManager::CreateParty( IN OUT KPartyData& kData_, IN KCenterPtr spCenter_, IN KCenterPtr spCenterPartyHost_ )
{
    KCnPartyPtr spParty;

    ++m_dwUID;

    spParty.reset( new KCnParty );
    spParty->SetUID( m_dwUID );
    spParty->SetPartyUID();

    spParty->AddMember( kData_.m_vecPartyPeople );
    spParty->SetNextHost(); // 방장 정하기, 없으면 첫번째 유저가 방장
    
    spParty->AddServer( spCenter_ ); // 파티 초대 수락한 사람의 서버 포인터
    spParty->AddServer( spCenterPartyHost_ ); // 파티장의 서버 포인터

    spParty->SetMemberNickName( kData_.m_kInvitee );
    spParty->SetMemberServerUID( kData_.m_kInvitee ); // 파티원의 서버UID 설정
    spParty->SetPartyType( kData_.m_dwPartyType );
    spParty->GetPartyInfo( kData_ );
    
    if ( m_mapParty.insert( std::make_pair( m_dwUID , spParty ) ).second == false ) {
        return false;
    }

    return true;
}

bool KCnPartyManager::CreatePartyAnswer( IN OUT KPartyData& kData_, IN KCenterPtr spCenter_, bool bBroadcast_ )
{
    KCnPartyPtr spParty = GetParty( kData_.m_dwPartyUID );
    if ( spParty ) {
        spParty->AddServer( spCenter_ );
        kData_.m_dwPartyUID = spParty->GetUID();
        spParty->SetMemberNickName( kData_.m_kInvitee );
        spParty->GetPartyInfo( kData_ );
    }
    else {
        return false;
    }

    return true;
}

KCnPartyPtr KCnPartyManager::GetParty( DWORD dwPartyUID_ )
{
    std::map< DWORD, KCnPartyPtr >::iterator mit;
    KLocker lock( m_csParty );
    mit = m_mapParty.find( dwPartyUID_ );

    if ( mit == m_mapParty.end() ) {
        return KCnPartyPtr();
    }

    return mit->second;
}

bool KCnPartyManager::BreakParty( IN KPartyData& kData_ )
{
    std::map< DWORD, KCnPartyPtr >::iterator mit;
    KLocker lock( m_csParty );
    mit = m_mapParty.find( kData_.m_dwPartyUID );

    if ( mit != m_mapParty.end() ) {
        m_mapParty.erase( mit );
    }
    else {
        return false;
    }

    START_LOG( clog, L"파티 해체됨, Party UID: " << kData_.m_dwPartyUID ) << END_LOG;

    return true;
}

bool KCnPartyManager::JoinParty( IN OUT KPartyData& kData_, IN KCenterPtr spCenter_ )
{
    KCnPartyPtr spParty = GetParty( kData_.m_dwPartyUID );
    if ( !spParty ) {
        START_LOG( cerr, L"파티 포인터 없음, Party UID: " << kData_.m_dwPartyUID ) << END_LOG;
        SET_ERROR( ERR_PARTY_07 );
        return false;
    }

    if ( spParty->IsExistMember( kData_.m_kInvitee ) ) {
        START_LOG( cerr, L"이미 있는 파티원을 초대, Party UID: " << kData_.m_dwPartyUID )
            << BUILD_LOG( kData_.m_kInvitee.m_dwUID )
            << BUILD_LOG( kData_.m_kInvitee.m_strNickName ) << END_LOG;
        SET_ERROR( ERR_PARTY_02 );
        return false;
    }

    if ( spParty->IsFull() ) {
        START_LOG( cwarn, L"파티 제한 인원을 초과, Party UID: " << kData_.m_dwPartyUID )
            << BUILD_LOG( kData_.m_vecPartyPeople.size() ) << END_LOG;

        SET_ERROR( ERR_PARTY_03 );
        return false;
    }

    spParty->AddMember( kData_.m_kInvitee, spCenter_ );
    kData_.m_dwPartyUID = spParty->GetUID();
    spParty->SetMemberServerUID( kData_.m_kInvitee ); // 파티원의 서버UID 설정
    spParty->GetPartyInfo( kData_ );

    return true;
}


bool KCnPartyManager::SetNextHost( IN KPartyData& kData_ )
{
    if ( kData_.m_vecPartyPeople.empty() ) {
        return false;
    }
    
    KCnPartyPtr spParty = GetParty( kData_.m_dwPartyUID );
    if ( spParty ) {
        spParty->SetNextHost( kData_.m_vecPartyPeople.front().m_dwUID );
        
    }
    else {
        START_LOG( cerr, L"파티장 위임할 파티 없음, Party UID : " << kData_.m_dwPartyUID ) << END_LOG;
        return false;
    }

    return true;
}

bool KCnPartyManager::LeaveParty( IN KPartyData& kData_, IN KCenterPtr spCenter_ )
{
    if ( kData_.m_vecPartyPeople.empty() ) {
        return false;
    }

    KCnPartyPtr spParty = GetParty( kData_.m_dwPartyUID );
    if ( spParty ) {
        spParty->DeleteUser( kData_.m_vecPartyPeople.front() ); // 내부에 방장 교체 함수호출

        if ( spParty->GetCount() <= 1 ) { // 유저 1명 파티->소멸
            spParty->GetPartyInfo( kData_ );        
            BreakParty( kData_ );
        }

    }
    else {
        START_LOG( cerr, L"파티 나가기 실패, Party UID : " << kData_.m_dwPartyUID )
            << BUILD_LOG( kData_.m_vecPartyPeople.front().m_dwUID ) << END_LOG;
    }

    return true;
}

bool KCnPartyManager::BanPartyUser( IN KPartyData& kData_ )
{
    if ( kData_.m_vecPartyPeople.empty() ) {
        return false;
    }

    KCnPartyPtr spParty = GetParty( kData_.m_dwPartyUID );
    if ( spParty ) {
        spParty->DeleteUser( kData_.m_vecPartyPeople.front()  ); // 내부에 방장 교체 함수호출

        if ( spParty->GetCount() <= 1 ) { // 유저 1명 파티->소멸
            spParty->GetPartyInfo( kData_ );        
            BreakParty( kData_ );
        }

    }
    else {
        START_LOG( cerr, L"파티 나가기 실패, Party UID : " << kData_.m_dwPartyUID )
            << BUILD_LOG( kData_.m_vecPartyPeople.front().m_dwUID ) << END_LOG;
    }

    return true;
}

void KCnPartyManager::PrintParty( DWORD dwPartyUID_ )
{
    KCnPartyPtr spParty = GetParty( dwPartyUID_ );

    if ( spParty ) {
        spParty->PrintPartyUserList();
    }
    else {
        START_LOG( cerr, L"없는 파티 UID: " << dwPartyUID_ ) << END_LOG;
    }
}

void KCnPartyManager::PrintAll()
{
    std::map< DWORD, KCnPartyPtr >::iterator mit;
    KLocker lock( m_csParty );
    
    START_LOG( cerr, L"총 파티 개수 : " << m_mapParty.size() ) << END_LOG;

    for ( mit = m_mapParty.begin(); mit != m_mapParty.end(); ++mit ) {
        mit->second->PrintPartyUserList();
    }

}

bool KCnPartyManager::ChangeUserState( IN KPartyData& kData_, IN KCenterPtr spCenter_ )
{
    if ( kData_.m_vecPartyPeople.empty() ) {
        return false;
    }

    KCnPartyPtr spParty = GetParty( kData_.m_dwPartyUID );
    if ( spParty ) {
        spParty->ChangeUserState( kData_.m_vecPartyPeople.front() ); // 내부에 방장 교체 함수호출
    }
    else {
        START_LOG( cerr, L"파티원 상태값 변경 실패, Party UID : " << kData_.m_dwPartyUID )
            << BUILD_LOG( kData_.m_vecPartyPeople.front().m_dwUID ) 
            << END_LOG;
    }
    return true;
}