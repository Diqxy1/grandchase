#include "Socket/Session.h"
#include "MatchNetLayer.h"
#include "MatchSimLayer.h"
#include <sstream>
#include "MatchUser.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KMatchNetLayer );
NiImplementRTTI( KMatchNetLayer, KNetLayer );

KMatchNetLayer::KMatchNetLayer(void)
{
}

KMatchNetLayer::~KMatchNetLayer(void)
{
}

ImplToStringW( KMatchNetLayer )
{
    return START_TOSTRING_PARENTW( KNetLayer ) << std::endl;

}

ImplOstmOperatorW2A( KMatchNetLayer );

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

bool KMatchNetLayer::Init()
{
    m_vecAccepter.push_back( IAccepterPtr( new KAccepter<KMatchUser>( m_usPortMaster ) ) );

    _JIF( KNetLayer::Init(), false );

    return true;
}

void KMatchNetLayer::ResetMaxData()
{
    std::wostringstream strstm;

    KSession::DumpMaxSendData( strstm );
    KNetLayer::ResetMaxData();
    START_LOG( cerr, L"" << strstm.str() ) << END_LOG;
    KSession::ResetMaxSendData();
}

void KMatchNetLayer::RegToLua( boost::shared_ptr<lua_State> L )
{
    KNetLayer::RegToLua( L );

    luabind::module( L.get() )
    [
        luabind::def( "GetNetLayer", KMatchNetLayer::GetInstance ),

        luabind::class_<KMatchNetLayer, KNetLayer>( "KMatchNetLayer" )
            .def( luabind::tostring(luabind::self) )
            .def( "ResetMaxData", &KMatchNetLayer::ResetMaxData ),
        luabind::def( "GetMatchNetLayer", &KMatchNetLayer::GetInstance )
    ];
}

void KMatchNetLayer::ShutDown()
{
    KNetLayer::ShutDown();
}
