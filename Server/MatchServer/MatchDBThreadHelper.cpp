#include <winsock2.h>
#include "MatchDBThread.h"
#include "NetError.h"
#include "MatchServer.h"
#include "MatchSimLayer.h"
#include "MatchDBLayer.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

#define CLASS_TYPE KMatchDBThread

void KMatchDBThread::SetServerListFromDB( IN const int& nProtocol_ )
{
    std::vector<KServerInfo> vecServerList;
    LIF( GetServerList( nProtocol_, vecServerList ) );
    SiKMatchServer()->SetServerList( vecServerList );
}

bool KMatchDBThread::GetServerList( IN const int& nProtocol_, OUT std::vector<KServerInfo>& vecServerList_ )
{
    /*
    { call dbo.CUGAConcurrentUser_select_All_by_ProtocolVersion ( @1 ) }
    @1 ; @iProtocolVersion_ int

    n select ( @1, @2, @3, @4, @5, @6, @7 )
    @1  ; ServerPart smallint
    @2  ; ServerName nvarchar( 20 )
    @3  ; ServerIP   varchar ( 25 ) // 인증용 ip.
    @4  ; ServerPort int
    @5  ; UserNum    int
    @6  ; MaxNum     int
    @7  ; NO         int
    @8  ; ServerIP2  varchar ( 25 ) // 전달용 ip.
    */
    vecServerList_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.CUGAConcurrentUser_select_All_by_ProtocolVersion ( %d ) }",
        nProtocol_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KServerInfo kServerInfo;
        kServerInfo.m_nUserProtocolVer = nProtocol_;

        VERIFY_QUERY( m_kODBC >> kServerInfo.m_iServerPart 
            >> kServerInfo.m_strName
            >> kServerInfo.m_strIP
            >> kServerInfo.m_usPort
            >> kServerInfo.m_iUserNum
            >> kServerInfo.m_iMaxUserNum
            >> kServerInfo.m_dwUID
            >> kServerInfo.m_strConnIP );

        vecServerList_.push_back( kServerInfo );
    }
    m_kODBC.EndFetch();

    return true;
}