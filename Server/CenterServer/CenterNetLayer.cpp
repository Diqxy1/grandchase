#include "Socket/Session.h"
#include "CenterNetLayer.h"
#include "CenterSimLayer.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCenterNetLayer );
NiImplementRTTI( KCenterNetLayer, KNetLayer );

KCenterNetLayer::KCenterNetLayer(void)
{
}

KCenterNetLayer::~KCenterNetLayer(void)
{
    // accepter의 delete는 NetLayer에서 한다.
}

ImplToStringW( KCenterNetLayer )
{
    return START_TOSTRING_PARENTW( KNetLayer )
        << L"    Nothing." << std::endl;
}

ImplOstmOperatorW2A( KCenterNetLayer );

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

void KCenterNetLayer::ResetMaxData()
{
    std::wostringstream strstm;

    KSession::DumpMaxSendData( strstm );
    KNetLayer::ResetMaxData();
    START_LOG( cerr, strstm.str() ) << END_LOG;
    KSession::ResetMaxSendData();
}
void KCenterNetLayer::RegToLua( std::shared_ptr<lua_State> L )
{
    KNetLayer::RegToLua( L );

    luabind::module( L.get() )
    [
        luabind::class_<KCenterNetLayer, KNetLayer>( "KCenterNetLayer" )
            .def( luabind::tostring(luabind::self) )
            .def( "ResetMaxData", &KCenterNetLayer::ResetMaxData ),
        luabind::def( "GetCenterNetLayer", &KCenterNetLayer::GetInstance )
    ];
}

bool KCenterNetLayer::Init()
{
    m_vecAccepter.push_back( IAccepterPtr( new KAccepter<KCenter>( m_usPortMaster ) ) );
    m_vecAccepter.push_back( IAccepterPtr( new KAccepter<KNUser, false>( m_usPortSlave ) ) );

    _JIF( KNetLayer::Init(), return false );

    return true;
}