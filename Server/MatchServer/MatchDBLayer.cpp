#include "MatchDBLayer.h"
#include "MatchDBThread.h"
#include "odbc/Odbc.h"

ImplOstmOperatorW2A( KMatchDBLayer );
NiImplementRTTI( KMatchDBLayer, KDBLayer );
ImplementSingleton( KMatchDBLayer );

KMatchDBLayer::KMatchDBLayer(void)
{
}

KMatchDBLayer::~KMatchDBLayer(void)
{
}

ImplToStringW( KMatchDBLayer )
{
    return START_TOSTRING_PARENTW( KDBLayer );
}

boost::shared_ptr<KThread> KMatchDBLayer::CreateThread()
{
    boost::shared_ptr<KMatchDBThread> spThread( new KMatchDBThread );
    spThread->SetThreadManager( this );
    return spThread;
}

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

void KMatchDBLayer::RegToLua( boost::shared_ptr<lua_State> sptlua )
{
    KDBLayer::RegToLua( sptlua );

    luabind::module( sptlua.get() )
    [
        luabind::def( "GetDBLayer", KMatchDBLayer::GetInstance ),
        luabind::class_<KMatchDBLayer, KThreadManager>( "KMatchDBLayer" )
            .def( luabind::tostring(luabind::self) )
            .def( "Init_ODBC", &KMatchDBLayer::Init_ODBC )
    ];
}

void KMatchDBLayer::Init_ODBC( const char* szMainODBC_, const char* szStatODBC_ )
{
    KMatchDBThread::InitFileDSN( szMainODBC_ );
    KMatchDBThread::InitFileDSNStat( szStatODBC_ );
}