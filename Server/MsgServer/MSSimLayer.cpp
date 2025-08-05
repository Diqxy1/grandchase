#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

#include "MSSimLayer.h"
#include <iomanip>  // std::setiosflags, std::resetiosflags
#include "defaultFSM.h"
#include "MSDBLayer.h"
#include "MSDBThread.h"
#include "MsgServer.h"
#include "MSUser.h"
#include "DBLayer.h"
#include "SlangFilter.h"
ImplementSingleton( KMSSimLayer );
NiImplementRTTI( KMSSimLayer, KSimLayer );

KMSSimLayer::KMSSimLayer(void) 
:
m_dwShutdownTimeLimit( 30000 ),
m_dwSendTimeLimit( 10000 ),
m_usMaxFriendLimit( 100 ),
m_usMaxBlockLimit( 100 ),
m_usMaxGroupLimit( 10 ),
m_usMaxGroupName( 12 ),
m_usMaxChatRoom( 10 ),
m_usMaxChatMember( 20 ),
m_dwChatRoomUID( 0 )
{
}

KMSSimLayer::~KMSSimLayer(void)
{
}

ImplToStringW( KMSSimLayer )
{
    START_TOSTRING_PARENTW( KSimLayer )
        << TOSTRINGW( m_dwShutdownTimeLimit )
        << TOSTRINGW( m_dwSendTimeLimit )
        << TOSTRINGW( m_usMaxFriendLimit )
        << TOSTRINGW( m_usMaxBlockLimit )
        << TOSTRINGW( m_usMaxGroupLimit )
        << TOSTRINGW( m_usMaxGroupName )
        << TOSTRINGW( m_usMaxChatRoom )
        << TOSTRINGW( m_usMaxChatMember )
        << TOSTRINGW( m_dwChatRoomUID );

    SiKMRConnector()->ToString( stm_ );

    return stm_;
}

ImplOstmOperatorW2A( KMSUser );
ImplOstmOperatorW2A( KMSSimLayer );

void KMSSimLayer::RegToLua( std::shared_ptr<lua_State> L )
{
    KSimLayer::RegToLua( L );

    luabind::module( L.get() )
    [
        luabind::def( "GetSimLayer", &KMSSimLayer::GetInstance ),
        luabind::class_<KMSSimLayer, KSimLayer>( "KMSSimLayer" )
            .def( luabind::tostring(luabind::self) )
            .def_readwrite( "m_dwShutdownTimeLimit", &KMSSimLayer::m_dwShutdownTimeLimit )
            .def_readwrite( "m_dwSendTimeLimit",     &KMSSimLayer::m_dwSendTimeLimit )
            .def_readwrite( "m_usMaxFriendLimit",    &KMSSimLayer::m_usMaxFriendLimit )
            .def_readwrite( "m_usMaxBlockLimit",     &KMSSimLayer::m_usMaxBlockLimit )
            .def_readwrite( "m_usMaxGroupLimit",     &KMSSimLayer::m_usMaxGroupLimit )
            .def_readwrite( "m_usMaxGroupName",      &KMSSimLayer::m_usMaxGroupName )
            .def_readwrite( "m_usMaxChatRoom",       &KMSSimLayer::m_usMaxChatRoom )
            .def_readwrite( "m_usMaxChatMember",     &KMSSimLayer::m_usMaxChatMember )
    ];
}

void KMSSimLayer::Init()
{
    KSimLayer::Init();

    // UserFactory 생성. KUser가 아닌 다른 객체군을 생성한다면, 코드 구현 후 이곳 한줄만 변경하면 됨.
    // Abstract Factory Patern.
    m_kActorManager.SetActorFactory( IActorFactoryPtr( new KActorFactory<KMSUser, KDefaultFSM, KMSUser::MSUserPoolTag> ) );

}

void KMSSimLayer::Tick()
{
    BEGIN_TRY_BLOCK;

    KSimLayer::Tick();

    SiKMsgServer()->Tick();

    //m_kMRConnector.Tick();

    END_TRY_BLOCK( "" );
}

void KMSSimLayer::ShutDown()
{
    KSimLayer::ShutDown();
    KSlangFilter::ReleaseInstance();
}

bool KMSSimLayer::DestroyAllUsers()
{
    m_kActorManager.SetCreateActorEnable( false );

    START_LOG( cerr, L"서버 종료. 현재 유저 : " << m_kActorManager.GetCount() << L" 명" ) << END_LOG;

    m_kActorManager.ForEach( boost::bind( &KActor::ReserveDestroy, _1, 0 ) );    
    SiKMRConnector()->StartShutdown();
    DWORD dwBeginTick = ::GetTickCount();

    // 모든 유저 객체가 소멸되어야 하고 Router로 송신한 데이터가 모두 처리되어야 함
    while( ( m_kActorManager.GetCount() != 0 || !SiKMRConnector()->ShutdownFinished() ) &&
           ::GetTickCount() - dwBeginTick < m_dwShutdownTimeLimit )   // 하지만 일정 시간이 지나면 그냥 통과.
    {
        ::Sleep( 500 );
    }

    if( m_kActorManager.GetCount() == 0 && SiKMRConnector()->ShutdownFinished() )
    {
        START_LOG( cerr, L"모든 유저 정상 종료" )
            << L"Elapsed Time : " << ::GetTickCount() - dwBeginTick << dbg::endl
            << L"Time Limit : " << m_dwShutdownTimeLimit << dbg::endl
            << L"non-safed release count : " << m_kActorManager.GetCount() << END_LOG;
        return true;
    }

    START_LOG( cerr, L"캐싱 정보를 모두 저장하지 못하고 종료" )
        << L"Elapsed Time : " << ::GetTickCount() - dwBeginTick << dbg::endl
        << L"Time Limit : " << m_dwShutdownTimeLimit << dbg::endl
        << L"non-safed release count : " << m_kActorManager.GetCount() << END_LOG;

    return false;

}

void KMSSimLayer::GetLoginnedFriendInfo( KEMR_LOGINNED_FRIEND_INFO_REQ& kReq_ )
{
//     std::vector<KFriendInfo> vecFriendList;
// 
//     std::vector<DWORD>::iterator vit;
//     for( vit = kReq_.m_vecFriendUID.begin(); vit != kReq_.m_vecFriendUID.end(); vit++ )
//     {
//         KMSUserPtr spUser( m_kActorManager.GetByUID<KMSUser>( *vit ) );
// 
//         if( spUser )
//         {
//             vecFriendList.push_back( spUser->GetMyFriendInfo( kReq_.m_dwLogonUID ) );
//         }
//     }
// 
//     if( !vecFriendList.empty() )
//     {
//         SiKMRConnector()->RemoteQueueing( 0, kReq_.m_dwLogonUID, 
//             KMSEvent::EMS_LOGINNED_FRIEND_LIST_NOT, 
//             vecFriendList, false, false );
//     }
}

void KMSSimLayer::GetLoginnedFriendGroupInfo_S2( IN OUT KEMR_LOGINNED_FRIEND_INFO_REQ& kReq_ )
{
    std::map< int, std::map<DWORD,KFriend> > mapFriendGroup;
    std::map< int, std::set<DWORD> >::iterator mit;

    // 친구그룹 돌면서
    for( mit = kReq_.m_mapFriendGroup.begin() ; mit != kReq_.m_mapFriendGroup.end() ; ++mit )
    {
        std::map<DWORD,KFriend> mapMembers;

        //그룹 맴버 정보 세팅(내UID, 그룹맴버들 UID 리스트, 정보가 저장될 컨테이터)
        GetLoginnedFriendInfo_S2( kReq_.m_dwLogonUID, mit->second, mapMembers );
        START_LOG( clog, L"그룹 맴버 정보 싣기")
            << BUILD_LOG( kReq_.m_dwLogonUID )
            << BUILD_LOG( mit->second.size() )
            << BUILD_LOG( mapMembers.size() ) << END_LOG;

        //그룹별 정보 싣기(그룹ID,그룹맴버들정보)
        mapFriendGroup.insert( std::make_pair( mit->first, mapMembers ) );
    }

    START_LOG( clog, L"그룹별 정보 싣기끝")
        << BUILD_LOG( mapFriendGroup.size() ) << END_LOG;

    // 친구그룹정보가 들어있으면
    if( !mapFriendGroup.empty() )
    {
        SiKMRConnector()->QueueingPacket( 0, kReq_.m_dwLogonUID,
            KMSEvent::EMS_S2_LOGINNED_FRIEND_LIST_NOT,
            mapFriendGroup, false, false );
    }
}

void KMSSimLayer::GetLoginnedFriendInfo_S2( IN DWORD dwLogonUID_, IN std::set<DWORD>& mapFriendUID_, OUT std::map<DWORD,KFriend>& mapMembers_ )
{
    std::set<DWORD>::iterator sit;

    // 그룹 맴버 리스트 돌면서
    for( sit = mapFriendUID_.begin() ; sit != mapFriendUID_.end() ; ++sit )
    {
        // 맴버 UID로 유저 포인트 얻기
        KMSUserPtr spUser( m_kActorManager.GetByUID<KMSUser>( *sit ) );

        if( spUser ) // 유효하면
        {
            // 컨테이너에 실어주자( 유저UID, 유저 정보(KFriend) )
            mapMembers_.insert( std::make_pair( *sit, spUser->GetMyFriendInfo_S2( dwLogonUID_ ) ));
        }
    }
}

void KMSSimLayer::GetChatRoomUID( OUT std::pair<DWORD,DWORD>& prRoomKey_ )
{
    KLocker lock( m_csChatRoomUID );

    ++m_dwChatRoomUID;
    // 1부터 시작.
    prRoomKey_ = std::make_pair( SiKMsgServer()->GetUID(), m_dwChatRoomUID );
}
