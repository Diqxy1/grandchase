#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

#include "TRSimLayer.h"
#include <iomanip>  // std::setiosflags, std::resetiosflags
#include "defaultFSM.h"
#include "TRDBLayer.h"
#include "TRDBThread.h"
#include "TRServer.h"
#include "TRUser.h"
#include "DBLayer.h"
#include "SlangFilter.h"

ImplementSingleton( KTRSimLayer );
NiImplementRTTI( KTRSimLayer, KSimLayer );

KTRSimLayer::KTRSimLayer(void) 
:
m_dwShutdownTimeLimit( 30000 ),
m_dwSendTimeLimit( 10000 )
{
}

KTRSimLayer::~KTRSimLayer(void)
{
}

ImplToStringW( KTRSimLayer )
{
    START_TOSTRING_PARENTW( KSimLayer )
        << TOSTRINGW( m_dwShutdownTimeLimit )
        << TOSTRINGW( m_dwSendTimeLimit );
    return stm_;
}

ImplOstmOperatorW2A( KTRUser );
ImplOstmOperatorW2A( KTRSimLayer );

void KTRSimLayer::RegToLua( std::shared_ptr<lua_State> L )
{
    KSimLayer::RegToLua( L );

    luabind::module( L.get() )
    [
        luabind::def( "GetSimLayer", &KTRSimLayer::GetInstance ),
        luabind::class_<KTRSimLayer, KSimLayer>( "KTRSimLayer" )
            .def( luabind::tostring(luabind::self) )
            .def_readwrite( "m_dwShutdownTimeLimit", &KTRSimLayer::m_dwShutdownTimeLimit )
            .def_readwrite( "m_dwSendTimeLimit",     &KTRSimLayer::m_dwSendTimeLimit )
    ];
}

void KTRSimLayer::Init()
{
    KSimLayer::Init();

    // UserFactory 생성. KUser가 아닌 다른 객체군을 생성한다면, 코드 구현 후 이곳 한줄만 변경하면 됨.
    // Abstract Factory Patern.
    //m_kActorManager.SetActorFactory( IActorFactoryPtr( new KActorFactory<KTRUser, KDefaultFSM, KTRUser::TRUserPoolTag> ) );

}

void KTRSimLayer::Tick()
{
    BEGIN_TRY_BLOCK;

    KSimLayer::Tick();

    SiKTRServer()->Tick();

    END_TRY_BLOCK( "" );
}

void KTRSimLayer::ShutDown()
{
    KSimLayer::ShutDown();
    KSlangFilter::ReleaseInstance();
}

bool KTRSimLayer::DestroyAllUsers()
{
    m_kActorManager.SetCreateActorEnable( false );
    m_kActorManager.ForEach( boost::bind( &KActor::ReserveDestroy, _1, 0 ) );
    return false;
}