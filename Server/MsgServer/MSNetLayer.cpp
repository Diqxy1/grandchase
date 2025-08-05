#include "Socket/Session.h"
#include "MSNetLayer.h"
#include "MSSimLayer.h"
#include <sstream>
#include "MSUser.h"

//FILE_NAME_FOR_LOG

ImplementSingleton( KMSNetLayer );
NiImplementRTTI( KMSNetLayer, KNetLayer );

KMSNetLayer::KMSNetLayer(void)
{
}

KMSNetLayer::~KMSNetLayer(void)
{
}

ImplToStringW( KMSNetLayer )
{
    return START_TOSTRING_PARENTW( KNetLayer )
        << L"   nothing to dump." << std::endl;
}

ImplOstmOperatorW2A( KMSNetLayer );

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

bool KMSNetLayer::Init()
{
    m_vecAccepter.push_back( IAccepterPtr( new KAccepter<KMSUser>( m_usPortMaster ) ) );

    _JIF( KNetLayer::Init(), false );

    return true;
}

void KMSNetLayer::ResetMaxData()
{
    std::wostringstream strstm;

    KSession::DumpMaxSendData( strstm );
    KNetLayer::ResetMaxData();
    START_LOG( cerr, strstm.str() ) << END_LOG;
    KSession::ResetMaxSendData();
}

void KMSNetLayer::RegToLua( std::shared_ptr<lua_State> L )
{
    KNetLayer::RegToLua( L );

    luabind::module( L.get() )
    [
        luabind::def( "GetNetLayer", KMSNetLayer::GetInstance ),

        luabind::class_<KMSNetLayer, KNetLayer>( "KMSNetLayer" )
            .def( luabind::tostring(luabind::self) )
            .def( "ResetMaxData", &KMSNetLayer::ResetMaxData ),
        luabind::def( "GetMSNetLayer", &KMSNetLayer::GetInstance )
    ];
}