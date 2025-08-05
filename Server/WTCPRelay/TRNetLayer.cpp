#include "Socket/Session.h"
#include "TRNetLayer.h"
#include "TRSimLayer.h"
#include <sstream>
#include "TRUser.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KTRNetLayer );
NiImplementRTTI( KTRNetLayer, KNetLayer );

KTRNetLayer::KTRNetLayer(void)
{
}

KTRNetLayer::~KTRNetLayer(void)
{
}

ImplToStringW( KTRNetLayer )
{
    return START_TOSTRING_PARENTW( KNetLayer ) << std::endl;

}

ImplOstmOperatorW2A( KTRNetLayer );

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

bool KTRNetLayer::Init()
{
    m_vecAccepter.push_back( IAccepterPtr( new KAccepter<KTRUser>( m_usPortMaster ) ) );

    _JIF( KNetLayer::Init(), false );

    return true;
}

void KTRNetLayer::ResetMaxData()
{
    std::wostringstream strstm;

    KSession::DumpMaxSendData( strstm );
    KNetLayer::ResetMaxData();
    START_LOG( cerr, L"" << strstm.str() ) << END_LOG;
    KSession::ResetMaxSendData();
}

void KTRNetLayer::RegToLua( std::shared_ptr<lua_State> L )
{
    KNetLayer::RegToLua( L );

    luabind::module( L.get() )
    [
        luabind::def( "GetNetLayer", KTRNetLayer::GetInstance ),

        luabind::class_<KTRNetLayer, KNetLayer>( "KTRNetLayer" )
            .def( luabind::tostring(luabind::self) )
            .def( "ResetMaxData", &KTRNetLayer::ResetMaxData ),
        luabind::def( "GetTRNetLayer", &KTRNetLayer::GetInstance )
    ];
}

void KTRNetLayer::ShutDown()
{
    KNetLayer::ShutDown();
}
