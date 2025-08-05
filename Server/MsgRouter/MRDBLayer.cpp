#include "MRDBLayer.h"
#include "MRDBThread.h"
#include "odbc/Odbc.h"

ImplOstmOperatorW2A( KMRDBLayer );
NiImplementRTTI( KMRDBLayer, KDBLayer );
ImplementSingleton( KMRDBLayer );

KMRDBLayer::KMRDBLayer(void)
{
}

KMRDBLayer::~KMRDBLayer(void)
{
}

ImplToStringW( KMRDBLayer )
{
    return START_TOSTRING_PARENTW( KDBLayer );
}

std::shared_ptr<KThread> KMRDBLayer::CreateThread()
{
    std::shared_ptr<KMRDBThread> spThread( new KMRDBThread );
    spThread->SetThreadManager( this );
    return spThread;
}

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

void KMRDBLayer::RegToLua( std::shared_ptr<lua_State> sptlua )
{
    KDBLayer::RegToLua( sptlua );

    luabind::module( sptlua.get() )
    [
        luabind::def( "Init_MSDB", KMRDBThread::InitFileDSN ),
        luabind::def( "GetDBLayer", KMRDBLayer::GetInstance ),
        luabind::class_<KMRDBLayer, KThreadManager>( "KMRDBLayer" )
            .def( luabind::tostring(luabind::self) )
    ];
}