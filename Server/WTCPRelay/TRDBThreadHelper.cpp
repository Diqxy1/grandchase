#include <winsock2.h>
#include "TRDBThread.h"
#include "NetError.h"
#include "TRServer.h"
#include "TRSimLayer.h"
#include "TRDBLayer.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

#define CLASS_TYPE KTRDBThread

bool KTRDBThread::UpdateStatType( IN const int nType_, IN const int nServerID_, IN const int nTypeCount_ )
{
    /*
    L"{ call dbo.TRSYTcpRelayServerStat_merge ( %d, %d, %d ) }"

    { call dbo.TRSYTcpRelayServerStat_merge ( @1, @2, @3 ) }
    @1 ; @iServerID_    int
    @2 ; @iType_        tinyint
    @3 ; @iData_        int

    1 select ( @1 )
    @1  ; OK int
    0        ; 성공
    -1       ; ServerID가유효하지않음
    -901     ; Data 값변경실패
    -902     ; Data 값등록실패
    */

    /*
    --gc.dbo.MSCOMinuteStat.Type
    1: 분당동접
    2: 분당Login수
    3: 분당Logout수
    4: 분당Migrationin수
    5: 분당Migrationout수
    6: 분당Zombie수
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.TRSYTcpRelayServerStat_merge ( %d, %d, %d ) }",
        nServerID_, nType_, nTypeCount_ ) );

    if ( m_kODBC.BeginFetch() ) {
        int nOK = -99;
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
        if ( nOK != 0 ) {
            START_LOG( cerr, L"TCP 통계 추가 갱신 실패 .. OK : " << nOK << L" Query : " << m_kODBC.m_strLastQuery ) << END_LOG;
        }
    }

    return true;
}

int KTRDBThread::GetServerUID( IN const std::wstring strIP_, IN const USHORT usPort_, IN const int nType_ )
{
    /*
    L"{ call dbo.SMGAServerManagement_select_1_20130926 ( N'%s', %d, %d ) }"

    { call dbo.SMGAServerManagement_select_1_20130926 ( @01, @02, @03 ) }
        @01strIP nvarchar( 15 )   서버의 IP
    ,   @02iPort int              서버의 Port
    ,   @03iType int              서버의 종류

    1 select ( @01 )
    @01iUID    int            서버의 고유 번호
    */

    int nUID = -1;
    DBJIF2( m_kODBC.Query( L"{ call dbo.SMGAServerManagement_select_1_20130926 ( N'%s', %d, %d ) }",
        strIP_.c_str(), usPort_, nType_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nUID );
        m_kODBC.EndFetch();
    }

    START_LOG( cerr, L"TCP Server UID 정보 가져오기 " << nUID ) << END_LOG;

    return nUID;
}

bool KTRDBThread::UpdateCCU( IN const int nUID_, IN const int nStatus_, IN const int nCnt_ )
{
    /*
    L"{ call dbo.SMGAServerManagement_update_20130926 ( %d, %d, %d ) }"

    { call dbo.SMGAServerManagement_update_20130926 ( @01, @02, @03 ) }
        @01iUID_input    int
    ,   @02iStatus_input int
    ,   @03iCNT_input    int

    1 return ( @01 )
    @01: OK int
    0        : 성공
    -1       : 존재 하지 않는 서버
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.SMGAServerManagement_update_20130926 ( %d, %d, %d ) }",
        nUID_, nStatus_, nCnt_ ) );

    if ( m_kODBC.BeginFetch() ) {
        int nOK = -99;
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
        if ( nOK != 0 ) {
            START_LOG( cerr, L"TCP 동접 갱신 실패 .. OK : " << nOK << L" Query : " << m_kODBC.m_strLastQuery ) << END_LOG;
        }
    }

    return true;
}
