#include "MatchSvrConnector.h"
#include "MatchServer.h"
#include "MatchSimLayer.h"
#include "MatchEvent.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG
ImplementSingleton( KMatchSvrConnector );

KMatchSvrConnector::KMatchSvrConnector(void)
{
}

KMatchSvrConnector::~KMatchSvrConnector(void)
{
}

ImplToStringW( KMatchSvrConnector )
{
    return stm_;
}

void KMatchSvrConnector::Run()
{
    DWORD ret;

    while( true )
    {
        ret = ::WaitForSingleObject( m_hKillEvent, 1 );

        if( ret == WAIT_OBJECT_0 ) break;

        if( ret == WAIT_TIMEOUT ) Tick();

        else    START_LOG( cerr, L"*** WaitForSingleObject() - return : " << ret ) << END_LOG;
    }
}

void KMatchSvrConnector::Tick()
{

}