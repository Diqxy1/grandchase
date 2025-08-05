#include "MSDBLayer.h"
#include "MSDBThread.h"
#include "odbc/Odbc.h"

ImplOstmOperatorW2A( KMSDBLayer );
NiImplementRTTI( KMSDBLayer, KDBLayer );
ImplementSingleton( KMSDBLayer );

KMSDBLayer::KMSDBLayer(void)
{
}

KMSDBLayer::~KMSDBLayer(void)
{
}

ImplToStringW( KMSDBLayer )
{
    return START_TOSTRING_PARENTW( KDBLayer );
}

std::shared_ptr<KThread> KMSDBLayer::CreateThread()
{
    std::shared_ptr<KMSDBThread> spThread( new KMSDBThread );
    spThread->SetThreadManager( this );
    return spThread;
}

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

void KMSDBLayer::RegToLua( std::shared_ptr<lua_State> sptlua )
{
    KDBLayer::RegToLua( sptlua );

    luabind::module( sptlua.get() )
    [
        luabind::def( "Init_MSDB", KMSDBThread::InitFileDSN ),
        luabind::def( "Init_MSMainDB", KMSDBThread::InitFileDSNMain ),
        luabind::def( "GetDBLayer", KMSDBLayer::GetInstance ),
        luabind::class_<KMSDBLayer, KThreadManager>( "KMSDBLayer" )
            .def( luabind::tostring(luabind::self) )
    ];
}