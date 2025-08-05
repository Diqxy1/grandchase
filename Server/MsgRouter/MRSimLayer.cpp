#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

#include "MRSimLayer.h"
#include <iomanip>  // std::setiosflags, std::resetiosflags
#include "defaultFSM.h"
#include "MRDBLayer.h"
#include "MRDBThread.h"
#include "MsgRouter.h"
#include "MRUser.h"
#include "MRPacket.h"
#include "DBLayer.h"
#include "NetError.h"

ImplementSingleton( KMRSimLayer );
NiImplementRTTI( KMRSimLayer, KSimLayer );

KMRSimLayer::KMRSimLayer(void) 
:
    m_dwShutdownTimeLimit( 30000 )
{
}

KMRSimLayer::~KMRSimLayer(void)
{
}

ImplToStringW( KMRSimLayer )
{

    return START_TOSTRING_PARENTW( KSimLayer )
        << TOSTRINGW( m_dwShutdownTimeLimit )
        << TOSTRINGW( m_mapMSUser.size() );
}

ImplOstmOperatorW2A( KMRUser );
ImplOstmOperatorW2A( KMRSimLayer );

void KMRSimLayer::RegToLua( std::shared_ptr<lua_State> L )
{
    KSimLayer::RegToLua( L );

    luabind::module( L.get() )
    [
        luabind::def( "GetSimLayer", &KMRSimLayer::GetInstance ),
        luabind::class_<KMRSimLayer, KSimLayer>( "KMRSimLayer" )
            .def( luabind::tostring(luabind::self) )
            .def_readwrite( "m_dwShutdownTimeLimit", &KMRSimLayer::m_dwShutdownTimeLimit )
    ];
}

void KMRSimLayer::Init()
{
    KSimLayer::Init();

    // UserFactory 생성. KUser가 아닌 다른 객체군을 생성한다면, 코드 구현 후 이곳 한줄만 변경하면 됨.
    // Abstract Factory Patern.
    m_kActorManager.SetActorFactory( IActorFactoryPtr( new KActorFactory<KMRUser, KDefaultFSM, KMRUser::MRUserPoolTag> ) );

}

void KMRSimLayer::Tick()
{
    BEGIN_TRY_BLOCK;

    KSimLayer::Tick();

    SiKMsgRouter()->Tick();

    END_TRY_BLOCK( "" );
}

void KMRSimLayer::ShutDown()
{
    KSimLayer::ShutDown();
}

bool KMRSimLayer::DestroyAllUsers()
{
    m_kActorManager.SetCreateActorEnable( false );

    START_LOG( cerr, L"서버 종료. 현재 유저 : " << m_kActorManager.GetCount() << L" 명" ) << END_LOG;

    m_kActorManager.ForEach( boost::bind( &KActor::ReserveDestroy, _1, 0 ) );

    DWORD dwBeginTick = ::GetTickCount();

    // 모든 유저 객체가 소멸되어야 하고 DB 작업이 모두 완료되어야 함
    while( ( m_kActorManager.GetCount() != 0 ) &&
           ::GetTickCount() - dwBeginTick < m_dwShutdownTimeLimit )   // 하지만 일정 시간이 지나면 그냥 통과.
    {
        ::Sleep( 500 );
    }

    if( m_kActorManager.GetCount() == 0 )
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

bool KMRSimLayer::GetMSUserInfo( IN DWORD dwUID_, OUT KMSUserInfo& kInfo_ )
{
    std::map<DWORD,KMSUserInfo>::iterator mit;
    mit = m_mapMSUser.find( dwUID_ );

    if( mit == m_mapMSUser.end() ) return false;

    kInfo_ = mit->second;

    return true;
}

bool KMRSimLayer::AddMSUserInfo( IN const KMSUserInfo& kInfo_ )
{
    std::map<DWORD,KMSUserInfo>::iterator mit;
    mit = m_mapMSUser.find( kInfo_.m_dwUserUID );

    if( mit != m_mapMSUser.end() )  // 이미 동일한 UID의 데이터가 존재한다.
    {
        if( mit->second.m_strMsgServer == kInfo_.m_strMsgServer )   // 기존 정보의 ms가 현재 ms와 동일
        {
            mit->second = kInfo_;
            return true;
        }
        else    // 현재 접속된 ms와 다른 곳에 접속되어 있다면 이중접속으로 인식한다.
        {
            KActorPtr spActor = m_kActorManager.GetByName( mit->second.m_strMsgServer );
            if( spActor )   // 타서버간 이중접속
            {
                // 이전에 접속했던 유저를 튕겨내도록 한다.
                KEMR_KICK_CLIENT_NOT kPacket;
                kPacket.m_dwUserUID     = mit->second.m_dwUserUID;
                kPacket.m_nOK           = 2; // 타서버간 이중접속은 2, 동일서버 이중접속은 1

                spActor->SendPacket( KMREvent::EMR_KICK_CLIENT_NOT, kPacket );

                // 현재 추가하고자 하는 정보로 덮어씌운다.
                mit->second = kInfo_;

                return true;
            }
            else    // 서버 다운 등으로 잘못된 데이터가 남아있던 경우.
            {
                mit->second = kInfo_;
                return true;
            }
        }
    }

    m_mapMSUser.insert( std::make_pair( kInfo_.m_dwUserUID, kInfo_ ) );

    return true;
}

bool KMRSimLayer::DeleteMSUserInfo( IN DWORD dwUID_ )
{
    std::map<DWORD,KMSUserInfo>::iterator mit;
    mit = m_mapMSUser.find( dwUID_ );

    if( mit == m_mapMSUser.end() )
    {
        START_LOG( cwarn, L"삭제하고하 하는 유저를 map에서 찾을 수 없음." )
            << BUILD_LOG( m_mapMSUser.size() )
            << BUILD_LOG( dwUID_ ) << END_LOG;

        return false;
    }

    START_LOG( clog, L"" )
        << BUILD_LOG( mit->first )
        << BUILD_LOG( mit->second.m_dwUserUID ) << END_LOG;

    m_mapMSUser.erase( mit );

    return true;
}