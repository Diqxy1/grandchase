#include "TRUserManager.h"
#include "TRUserFSM.h"
#include <boost/bind.hpp>
#include <dbg/dbg_log4.hpp>
#include <boost/shared_ptr.hpp>

KTRUserManager::KTRUserManager(void)
:   
    m_bCreateSessionEnable(true),
    m_spFSM( new KTRUserFSM )
{
    
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

    boost::shared_ptr<KTRUser> spTRUser( new KTRUser(false) );
    
    spTRUser->Init( true );

    if( spTRUser->GetUID() != 0 )
    {
        START_LOG( Error, L"Fail Add to m_listSessionTable, UID is Not Zero : " << spTRUser->GetUID() ) << END_LOG;
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

    START_LOG( Error, L"Not Found TRUser : " << dwUID_ ) << END_LOG;

    return KTRUserPtr();
}

void KTRUserManager::Relay( KRelayDataPtr& spData )
{
    KTRUserPtr spTRUser;
    std::vector< DWORD >::iterator vit;
    for( vit = spData->m_vecUID.begin() ; vit != spData->m_vecUID.end() ; ++vit )
    {
        
        spTRUser = GetTRUser( *vit );
        if( spTRUser )
        {
            spTRUser->Send( spData->m_buff );
        }
        else
        {
            START_LOG( Warn, L" Can't Find TRUser. UID : " << *vit )
                << BUILD_LOG( spData->m_vecUID.size() )
                << BUILD_LOG( spData->m_buff.size() ) << END_LOG;
        }
    }
    
}

void KTRUserManager::Tick()
{
    KLocker lock( m_cslist );

    std::list< KTRUserPtr >::iterator lit;
    lit = m_listSessionTable.begin();

    while( lit != m_listSessionTable.end() )
    {
        (*lit)->Tick();

        if( (*lit)->GetStateID() == KTRUserFSM::STATE_EXIT )
        {
            if( (*lit)->GetUID() != 0 ) // 유효한 UID가 등록 된 경우는 map에서도 지운다.
            {
                KLocker lock(m_csmap);
                m_mapSessionTable.erase( (*lit)->GetUID() );
                //START_LOG( Info, L"Delete User : " << (*lit)->GetUID() )
                    //<< BUILD_LOG( (*lit)->GetRefCount() )
                    //<< END_LOG;
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

bool KTRUserManager::Regist( KTRUserPtr& pkTRUser_ )
{

    //KActorPtr spActor = boost::static_pointer_cast<KActor>( pkTRUser_.shared_from_this() );

    if( pkTRUser_->GetUID() == 0 )
    {
        //START_LOG( Error, L"Can't Regist User UID : 0 ") << END_LOG;
        return false;
    }

    KLocker lock( m_csmap );
    std::map< DWORD, KTRUserPtr >::iterator mit = m_mapSessionTable.find( pkTRUser_->GetUID() );
    if( mit != m_mapSessionTable.end() ) // 이미 등록된 UID에 대한 처리.
    {
        /*
        START_LOG( Error, L"Already Exist")
            << BUILD_LOG( pkTRUser_->GetUID() ) << END_LOG;
        */
        return false;
    }

    m_mapSessionTable.insert( std::make_pair( pkTRUser_->GetUID(), pkTRUser_ ) );

    return true;
}

