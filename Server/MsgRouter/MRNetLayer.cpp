#include "Socket/Session.h"
#include "MRNetLayer.h"
#include "MRSimLayer.h"
#include <sstream>
#include "MRUser.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KMRNetLayer );
NiImplementRTTI( KMRNetLayer, KNetLayer );

KMRNetLayer::KMRNetLayer(void)
{
}

KMRNetLayer::~KMRNetLayer(void)
{
}

ImplToStringW( KMRNetLayer )
{
    return START_TOSTRING_PARENTW( KNetLayer )
        << L"   nothing to dump.";
}

ImplOstmOperatorW2A( KMRNetLayer );

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

bool KMRNetLayer::Init()
{
    m_vecAccepter.push_back( IAccepterPtr( new KAccepter<KMRUser>( m_usPortMaster ) ) );

    _JIF( KNetLayer::Init(), false );

    return true;
}

void KMRNetLayer::ResetMaxData()
{
    std::wostringstream strstm;

    KSession::DumpMaxSendData( strstm );
    KNetLayer::ResetMaxData();
    START_LOG( cerr, L"" << strstm.str() ) << END_LOG;
    KSession::ResetMaxSendData();
}

void KMRNetLayer::RegToLua( std::shared_ptr<lua_State> L )
{
    KNetLayer::RegToLua( L );

    luabind::module( L.get() )
    [
        luabind::def( "GetNetLayer", &KMRNetLayer::GetInstance ),

        luabind::class_<KMRNetLayer, KNetLayer>( "KMRNetLayer" )
            .def( luabind::tostring(luabind::self) )
            .def( "ResetMaxData", &KMRNetLayer::ResetMaxData ),
        luabind::def( "GetMSNetLayer", &KMRNetLayer::GetInstance )
    ];
}