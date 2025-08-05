#include "User.h" // 
#include "SquareManager.h"
#include <KncUtil.h>
#include <dbg/dbg.hpp>
#include <boost/mem_fn.hpp>
#include "Square.h"
#include "GameServer.h"
#include "TimeEventManager.h"
#include "GSSimLayer.h"
#include <boost/bind.hpp>

//FILE_NAME_FOR_LOG

ImplementSingleton( KSquareManager );
NiImplementRootRTTI( KSquareManager );
ImplOstmOperatorW2A( KSquareManager );
ImplToStringW( KSquareManager )
{
    KLocker lock( m_csSquares );
    KLocker lock1( m_csSquareInfo );
    START_TOSTRINGW
        << TOSTRINGW( m_mapSquares.size() )
        << TOSTRINGW( m_vecSquares.size() )
        << TOSTRINGW( m_vecSquareInfo.size() );

    //std::vector<KSquarePtr>::const_iterator vit;
    //for ( vit = m_vecSquares.begin() ; vit != m_vecSquares.end() ; ++vit ) {
    //    stm_ << L"ID : " << (*vit)->GetUID()
    //         << L", Name : " << (*vit)->GetName()
    //         << L"(" << (*vit)->GetSize() << L"/" << (*vit)->GetMiniGameUserSize() << L"/" << (*vit)->GetMaxUser() << L")"
    //         << L" { " << (*vit)->GetAddress() << L":" << (*vit)->GetPortNum() << L" }" << std::endl;
    //}
    return stm_;

}

KSquareManager::KSquareManager(void)
:m_kTimer( 10 * 60 * 1000 )
{
    for ( int i = 0 ; i < TI_MAX ; ++i ) {
        SetTick( i );
    }

    SetTickGap( TI_BUILD_LIST, 10 * 1000 );
    SetTickGap( TI_TIME_EVENT_LIST, 60 * 1000 );
}

KSquareManager::~KSquareManager(void)
{
}

void KSquareManager::Tick()
{
    {
        KLocker lock( m_csSquares );
        std::for_each( m_vecSquares.begin(), m_vecSquares.end(), boost::mem_fn(&KSquare::Tick) );
    }

    if ( ::GetTickCount() - GetTick( TI_BUILD_LIST ) > GetTickGap( TI_BUILD_LIST ) ) {
        BuildSquareList();
        SetTick( TI_BUILD_LIST );
    }

    if ( ::GetTickCount() - GetTick( TI_TIME_EVENT_LIST ) > GetTickGap( TI_TIME_EVENT_LIST ) ) {
        std::set<int> setSimpleTimeEvent;
        SiKTimeEventManager()->GetSimpleTimeEvents( setSimpleTimeEvent );
        // m_setSimpleTimeEvent.empty() 일때, 이벤트가 끝난 사실을 보내주자.
        SendToAll( KUserEvent::EVENT_TIME_EVENT_NOT, setSimpleTimeEvent );

        SetTick( TI_TIME_EVENT_LIST );
    }

    if ( m_kTimer.CheckTime() ) {
        ClearEmptySquare();

    }
}

void KSquareManager::GetSquareList( std::vector<KSquareInfo>& vecSquareList_ )
{
    vecSquareList_.clear();
    KLocker lock( m_csSquareInfo );
    vecSquareList_ = m_vecSquareInfo;

    if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
        in_addr    address; // KSquare::GetAddress 참고.
        START_LOG( clog, L"광장 리스트 내용 확인 ======"  ) << END_LOG;
        std::vector<KSquareInfo>::iterator vitSquareInfo;
        for ( vitSquareInfo = vecSquareList_.begin() ; vitSquareInfo != vecSquareList_.end() ; ++vitSquareInfo ) {
            
            address.s_addr = vitSquareInfo->m_dwTRServerIP;
            START_LOG( clog, L"Square UID : " << vitSquareInfo->m_dwID
                << L", nType : " << vitSquareInfo->m_nType 
                << L", IP : " << KncUtil::toWideString( ::inet_ntoa( address ) )  ) << END_LOG;
        }
    }
}

void KSquareManager::GetGuildSquareList( IN DWORD dwGuildUID_, std::vector<KSquareInfo>& vecSquareList_ )
{
    //vecSquareList_.clear(); // 길드 광장을 추가할 것이라, 클리어 안함.
    KLocker lock( m_csSquareInfo );
    //vecSquareList_ = m_vecSquareInfo;

    std::vector<KSquareInfo>::iterator vitSquareInfo;
    std::vector<KSquareInfo>::iterator vitInSquareList;

    for ( vitSquareInfo = m_vecSquareInfo.begin() ; vitSquareInfo != m_vecSquareInfo.end() ; ++vitSquareInfo ) {
        if ( vitSquareInfo->m_dwGuildUID == dwGuildUID_ ) {
            //vecSquareList_.push_back( *vitSquareInfo );
            vitInSquareList = std::find_if( vecSquareList_.begin(), vecSquareList_.end(),
                boost::bind( &KSquareInfo::m_nGuildSquareIndex, _1 ) == vitSquareInfo->m_nGuildSquareIndex );

            if ( vitInSquareList != vecSquareList_.end() ) {
                vitInSquareList = vitSquareInfo;
            }
        }
    }

    if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
        in_addr    address; // KSquare::GetAddress 참고.
        START_LOG( clog, L"광장 리스트 내용 확인 ======"  ) << END_LOG;
        std::vector<KSquareInfo>::iterator vitSquareInfo;
        for ( vitSquareInfo = vecSquareList_.begin() ; vitSquareInfo != vecSquareList_.end() ; ++vitSquareInfo ) {

            address.s_addr = vitSquareInfo->m_dwTRServerIP;
            START_LOG( clog, L"Square UID : " << vitSquareInfo->m_dwID
                << L", nType : " << vitSquareInfo->m_nType 
                << L", IP : " << KncUtil::toWideString( ::inet_ntoa( address ) ) ) << END_LOG;
        }
    }
}

void KSquareManager::BuildSquareList()
{
    std::vector<KSquareInfo> vecSquareInfo;

    KLocker lock( m_csSquareInfo );
        
    std::vector<KSquarePtr>::iterator vit;

    for ( vit = m_vecSquares.begin() ; vit != m_vecSquares.end() ; ++vit ) {

        KSquareInfo kInfo;
        (*vit)->GetInfo( kInfo );
        vecSquareInfo.push_back( kInfo );
        (*vit)->BuildUserList(); // 유저 리스트 만들기.
    }

    m_vecSquareInfo = vecSquareInfo;
}

void KSquareManager::EnterSquareReserve( IN OUT KUserPtr kUser_, IN DWORD dwSquareID_ )
{
    std::map<DWORD,KSquarePtr>::iterator mit;
    mit = m_mapSquares.find( dwSquareID_ );
    // 찾는 광장이 존재 하는가?
    _JIF( mit != m_mapSquares.end(), SET_ERROR( ERR_SQUARE_01 ); return );
    mit->second->EnterSquareReserve( kUser_ ); // KSquare 안에 있는 광장입장예약 호출
}

bool KSquareManager::AddSquare( IN KSquareInfo& kInfo_ )
{
    DWORD   dwIPAddress = kInfo_.m_dwTRServerIP;
    USHORT  usPort = kInfo_.m_usTRPort;

    KSquarePtr spSquare = KSquarePtr( new KSquare );

    spSquare->SetName( kInfo_.m_strName );
    spSquare->SetUID( kInfo_.m_dwID );
    spSquare->SetMaxUser( kInfo_.m_dwMaxUserNum );
    spSquare->SetType( kInfo_.m_nType );
    spSquare->SetGuildUID( kInfo_.m_dwGuildUID );
    spSquare->SetGuildSquareIndex( kInfo_.m_nGuildSquareIndex );
    //_JIF( spSquare->LoadInfo( "SquareInfo.lua" ), return false );
    _JIF( spSquare->SetTRAddress( dwIPAddress, usPort ), return false );

    KLocker lock( m_csSquares );
    std::pair< std::map<DWORD,KSquarePtr>::iterator, bool> prRet;
    prRet = m_mapSquares.insert( std::make_pair(spSquare->GetUID(), spSquare) );
    _JIF( prRet.second, return false );
    m_vecSquares.push_back( spSquare );

    BuildSquareList();
    return true;
}

bool KSquareManager::RemoveSquare( IN KSquarePtr spSquare_ )
{
    std::map<DWORD,KSquarePtr>::iterator mit;
    mit = m_mapSquares.find( spSquare_->GetUID() );

    if ( mit == m_mapSquares.end() ) {
        START_LOG( cerr, L"삭제하려는 광장이 없음, Square UID: " << spSquare_->GetUID() ) << END_LOG;
    }
    else {
        m_mapSquares.erase( mit );
    }

    std::vector<KSquarePtr>::iterator vit;
    vit = std::find( m_vecSquares.begin(), m_vecSquares.end(), spSquare_ );

    if ( vit != m_vecSquares.end() ) {
        m_vecSquares.erase( vit );
    }

    BuildSquareList();
    return true;
}

void KSquareManager::Send_NotMsg( std::wstring& strMsg_ )
{
    KLocker lock( m_csSquares );
    std::vector<KSquarePtr>::iterator vit;
    for( vit = m_vecSquares.begin() ; vit != m_vecSquares.end() ; ++vit )
    {
        (*vit)->Send_NotMsg( strMsg_ );
    }
}

void KSquareManager::Send_NotMsgA( const char* szMessage_ )
{
    if ( szMessage_ == NULL ) { 
        return;
    }
    std::wstring strNot = KncUtil::toWideString( szMessage_ );
    Send_NotMsg( strNot );
}

KSquarePtr KSquareManager::GetSquare( DWORD dwID_ )
{
    KLocker lock( m_csSquares );
    std::map<DWORD,KSquarePtr>::iterator mit;
    mit = m_mapSquares.find( dwID_ );
    if ( mit == m_mapSquares.end() ) {
        return KSquarePtr();
    }
    return mit->second;
}

bool KSquareManager::AddSquareList( IN std::vector<KSquareInfo>& vecSquares_ )
{
    std::vector<KSquareInfo>::iterator vit = vecSquares_.begin();

    for( ; vit != vecSquares_.end() ; ++vit )
    {
        if( AddSquare( *vit ) == false )
            return false;
    }

    return true;
}

void KSquareManager::LeaveSquare( IN KECN_LEAVE_SQUARE_BROAD kPacket_ )
{
    KSquarePtr spSquare = GetSquare( kPacket_.m_dwSquareUID );

    if( !spSquare )
        return;

    spSquare->LeaveSquare( kPacket_.m_dwUID );

    if( spSquare->IsEmpty() && spSquare->GetType() == KSquareInfo::ST_GUILD )
    {
        RemoveSquare( spSquare );
        _LOG_CONDITION( spSquare.use_count() == 1, clog, cerr, L"Smart Pointer of Square Ref Count : " << spSquare.use_count() ) << END_LOG;
    }
    QueueingToSquare( kPacket_.m_dwSquareUID, KUserEvent::EVENT_LEAVE_SQUARE_USER_NOT, kPacket_.m_dwUID );
}

void KSquareManager::ClearEmptySquare()
{
    std::vector<KSquarePtr>    vecRemoveSquares;
    std::vector<KSquarePtr>::iterator vit;
    std::vector<KSquarePtr>::iterator vitRemove;


    KLocker lock( m_csSquares );

    for ( vit = m_vecSquares.begin() ; vit != m_vecSquares.end() ; ++vit ) {
        if ( (*vit)->IsEmpty() && (*vit)->GetType() == KSquareInfo::ST_GUILD ) {
            vecRemoveSquares.push_back( *vit );
        }
    }

    for ( vitRemove = vecRemoveSquares.begin() ; vitRemove != vecRemoveSquares.end() ; ++vitRemove ) {
        RemoveSquare( *vitRemove );
    }
}