#include "TRUserManager.h"
#include "TRUserFSM.h"
#include <boost/bind.hpp>
#include <dbg/dbg.hpp>
#include <boost/shared_ptr.hpp>
#include "TRServer.h"
#include "TRDBLayer.h"

#define EVENT_TYPE KTREvent

KTRUserManager::KTRUserManager(void)
:   
m_bCreateSessionEnable(true),
m_spFSM( new KTRUserFSM )
{
    m_dwLastTick = ::GetTickCount();
    m_dwTickGap = 1000 * 60;
    m_mapTypeStat.clear();
    SetDefaultType();
}

KTRUserManager::~KTRUserManager(void)
{

}

//void KTRUserManager::DelUser( KTRUser trUser_ )
//{
//    trUser_.~KTRUser();
//}

KTRUserPtr KTRUserManager::CreateTRUser()
{
    if( !m_bCreateSessionEnable ) return KTRUserPtr();

    std::shared_ptr<KTRUser> spTRUser( new KTRUser(false) );

    spTRUser->Init( true );

    if( spTRUser->GetUID() != 0 )
    {
        START_LOG( cerr, L"Fail Add to m_listSessionTable, UID is Not Zero : " << spTRUser->GetUID() ) << END_LOG;
        return KTRUserPtr();
    }

    spTRUser->SetFSM( m_spFSM );
    spTRUser->StateTransition( KTRUserFSM::INPUT_CONNECT );

    {
        KLocker lock( m_cslist );
        m_listSessionTable.push_back( spTRUser );
    }

    return spTRUser;
}

KTRUserPtr KTRUserManager::GetTRUser( const DWORD dwUID_ )
{
    KLocker lock( m_csmap );
    std::map< DWORD, KTRUserPtr >::iterator mit;
    mit = m_mapSessionTable.find( dwUID_ );
    if( mit != m_mapSessionTable.end() )
    {
        return mit->second;
    }

    START_LOG( cerr, L"Not Found TRUser : " << dwUID_ ) << END_LOG;

    return KTRUserPtr();
}

void KTRUserManager::Relay( KRelayDataPtr& spData )
{
    KTRUserPtr spTRUser;
    std::vector< DWORD >::iterator vit;
    for( vit = spData->m_vecUID.begin() ; vit != spData->m_vecUID.end() ; ++vit ) {
        spTRUser = GetTRUser( *vit );
        if( spTRUser ) {
            // Send Packet fail Log.
            if( !spTRUser->Send( spData->m_buff ) ) {
                IncreaseTypeStatCount( KTRUserManager::STAT_SEND_PACKET_FAIL, 1 );
            }
        }
        else {
            START_LOG( cwarn, L" Can't Find TRUser. UID : " << *vit )
                << BUILD_LOG( spData->m_vecUID.size() )
                << BUILD_LOG( spData->m_buff.size() ) << END_LOG;
        }
    }
}

void KTRUserManager::Tick()
{
    int nSize = 0;
    {
        KLocker lock( m_cslist );

        std::list< KTRUserPtr >::iterator lit;
        lit = m_listSessionTable.begin();
        nSize = m_listSessionTable.size();

        while( lit != m_listSessionTable.end() )
        {
            (*lit)->Tick();

            if( (*lit)->GetStateID() == KTRUserFSM::STATE_EXIT )
            {
                if( (*lit)->GetUID() != 0 ) // 유효한 UID가 등록 된 경우는 map에서도 지운다.
                {
                    KLocker lock(m_csmap);
                    m_mapSessionTable.erase( (*lit)->GetUID() );
                    START_LOG( clog, L"Delete User : " << (*lit)->GetUID() )
                        //<< BUILD_LOG( (*lit)->GetRefCount() )
                        << END_LOG;
                }
                //(*lit)->ResetTRUser();
                lit = m_listSessionTable.erase( lit );
            }
            else
            {
                ++lit;
            }
        }
    }

    // 통계 추가.
    if( ::GetTickCount() - m_dwLastTick > m_dwTickGap ) {
        KDB_ETR_TYPE_COUNT_STAT kDBPacket;
        kDBPacket.m_nSID = SiKTRServer()->GetServerID();
        IncreaseTypeStatCount( KTRUserManager::STAT_CURRENT_USER , nSize );
        GetTypeStatInfo( kDBPacket.m_mapTypeStat );
        SetDefaultType(); // type에 대한 초기화.
        if( !kDBPacket.m_mapTypeStat.empty() ) {
            SiKTRDBLayer()->QueueingEvent( KTREvent::DB_ETR_TYPE_COUNT_STAT, L"", 0, -1, kDBPacket );
        }
        m_dwLastTick = ::GetTickCount();
    }
}

bool KTRUserManager::Regist( KTRUserPtr& pkTRUser_ )
{
    //KActorPtr spActor = std::static_pointer_cast<KActor>( pkTRUser_.shared_from_this() );

    if( pkTRUser_->GetUID() == 0 )
    {
        START_LOG( cerr, L"Can't Regist User UID : 0 ") << END_LOG;
        return false;
    }

    KLocker lock( m_csmap );
    std::map< DWORD, KTRUserPtr >::iterator mit = m_mapSessionTable.find( pkTRUser_->GetUID() );
    if( mit != m_mapSessionTable.end() ) // 이미 등록된 UID에 대한 처리.
    {
        START_LOG( cerr, L"Already Exist")
            << BUILD_LOG( pkTRUser_->GetUID() ) << END_LOG;
        return false;
    }

    m_mapSessionTable.insert( std::make_pair( pkTRUser_->GetUID(), pkTRUser_ ) );

    return true;
}

void KTRUserManager::IncreaseTypeStatCount( IN const int& nType_, IN const int& nCount_ )
{
    KLocker lock( m_csStat );
    m_mapTypeStat[nType_] += nCount_;
}

void KTRUserManager::GetTypeStatInfo( OUT std::map<int,int>& mapTypeStat_ )
{
    mapTypeStat_.clear();

    KLocker lock( m_csStat );
    std::map<int,int>::iterator mit;
    mapTypeStat_.swap( m_mapTypeStat );
}

void KTRUserManager::SetLimitPacketSendCount( IN int nLimitPacketSendCount_ )
{
    KTRUser::SetLimitPacketSendCount( nLimitPacketSendCount_ );
}

void KTRUserManager::SetDefaultType()
{
    KLocker lock( m_csStat );
    for( int i = KTRUserManager::STAT_RECV_COUNT; i <= KTRUserManager::STAT_USER_ZOMBIE_STATE; ++i ) {
        m_mapTypeStat[i] = 0;
    }
}