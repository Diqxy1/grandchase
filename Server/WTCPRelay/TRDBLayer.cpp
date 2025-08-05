#include "TRDBLayer.h"
#include "TRDBThread.h"
#include "odbc/Odbc.h"

ImplOstmOperatorW2A( KTRDBLayer );
NiImplementRTTI( KTRDBLayer, KDBLayer );
ImplementSingleton( KTRDBLayer );

KTRDBLayer::KTRDBLayer(void)
{
}

KTRDBLayer::~KTRDBLayer(void)
{
}

ImplToStringW( KTRDBLayer )
{
    return START_TOSTRING_PARENTW( KDBLayer );
}

std::shared_ptr<KThread> KTRDBLayer::CreateThread()
{
    std::shared_ptr<KTRDBThread> spThread( new KTRDBThread );
    spThread->SetThreadManager( this );
    return spThread;
}

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

void KTRDBLayer::RegToLua( std::shared_ptr<lua_State> sptlua )
{
    KDBLayer::RegToLua( sptlua );

    luabind::module( sptlua.get() )
    [
        luabind::def( "GetDBLayer", KTRDBLayer::GetInstance ),
        luabind::class_<KTRDBLayer, KThreadManager>( "KTRDBLayer" )
            .def( luabind::tostring(luabind::self) )
            .def( "Init_ODBC", &KTRDBLayer::Init_ODBC )
    ];
}

void KTRDBLayer::Init_ODBC( const char* szMainODBC_, const char* szStatODBC_ )
{
    KTRDBThread::InitFileDSN( szMainODBC_ );
    KTRDBThread::InitFileDSNStat( szStatODBC_ );
}