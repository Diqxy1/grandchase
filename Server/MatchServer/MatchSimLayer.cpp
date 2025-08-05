#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

#include "MatchSimLayer.h"
#include <iomanip>  // std::setiosflags, std::resetiosflags
#include "defaultFSM.h"
#include "MatchDBLayer.h"
#include "MatchDBThread.h"
#include "MatchServer.h"
#include "MatchUser.h"
#include "DBLayer.h"
#include "SlangFilter.h"

ImplementSingleton( KMatchSimLayer );
NiImplementRTTI( KMatchSimLayer, KSimLayer );

KMatchSimLayer::KMatchSimLayer(void) 
:
m_dwShutdownTimeLimit( 30000 ),
m_dwSendTimeLimit( 10000 )
{
}

KMatchSimLayer::~KMatchSimLayer(void)
{
}

ImplToStringW( KMatchSimLayer )
{
    START_TOSTRING_PARENTW( KSimLayer )
        << TOSTRINGW( m_dwShutdownTimeLimit )
        << TOSTRINGW( m_dwSendTimeLimit );
    return stm_;
}

ImplOstmOperatorW2A( KMatchUser );
ImplOstmOperatorW2A( KMatchSimLayer );

void KMatchSimLayer::RegToLua( boost::shared_ptr<lua_State> L )
{
    KSimLayer::RegToLua( L );

    luabind::module( L.get() )
    [
        luabind::def( "GetSimLayer", &KMatchSimLayer::GetInstance ),
        luabind::class_<KMatchSimLayer, KSimLayer>( "KMatchSimLayer" )
            .def( luabind::tostring(luabind::self) )
            .def_readwrite( "m_dwShutdownTimeLimit", &KMatchSimLayer::m_dwShutdownTimeLimit )
            .def_readwrite( "m_dwSendTimeLimit",     &KMatchSimLayer::m_dwSendTimeLimit )
    ];
}

void KMatchSimLayer::Init()
{
    KSimLayer::Init();

    // UserFactory 생성. KUser가 아닌 다른 객체군을 생성한다면, 코드 구현 후 이곳 한줄만 변경하면 됨.
    // Abstract Factory Patern.
    m_kActorManager.SetActorFactory( IActorFactoryPtr( new KActorFactory<KMatchUser, KDefaultFSM, KMatchUser::MatchUserPoolTag> ) );

}

void KMatchSimLayer::Tick()
{
    BEGIN_TRY_BLOCK;

    KSimLayer::Tick();

    SiKMatchServer()->Tick();

    END_TRY_BLOCK( "" );
}

void KMatchSimLayer::ShutDown()
{
    KSimLayer::ShutDown();
    KSlangFilter::ReleaseInstance();
}

bool KMatchSimLayer::DestroyAllUsers()
{
    m_kActorManager.SetCreateActorEnable( false );
    m_kActorManager.ForEach( boost::bind( &KActor::ReserveDestroy, _1, 0 ) );
    return false;
}