#include <WinSock2.h>
#pragma warning(push, 2)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)

#include "odbc/Odbc.h"
#include <dbg/dbg.hpp>
#include "Log4.h"

//FILE_NAME_FOR_LOG

void TestODBC2()
{
    std::wstring        m_strIP;            // IP 서버에서 기록.
    std::wstring        m_strOSType;        // OS 종류
    std::wstring        m_strCPU;           // cpu 이름
    int                 m_iMainMemory;      // Main Memory Size
    std::wstring        m_strGraphicChip;   // Graphic Chipset Name
    int                 m_iVMemory;         // Video Memory Size
    bool                m_bjoystick;        // Joystick Usable
    std::wstring        m_strDXversion;     // DX Version

    m_strIP = L"116.120.238.33";
    m_strOSType = L"WinXP HomeEdition";     
    m_strCPU = L"Intel Core2Duo";        
    m_iMainMemory = 1024;   
    m_strGraphicChip = L"Voodoo 3s";    
    m_iVMemory = 256;      
    m_bjoystick = false;     
    m_strDXversion = L"9.0c";  

    KODBC m_kODBC;
    m_kODBC.DriverConnect( L"odbc_stat.dsn" );

    int nOK;
    std::wstring strwLogin  = L"tgkwon";
    std::wstring strwLogin1  = L"tgkwon1";
    std::wstring strwPasswd = L"tksno1";
    std::string strPasswd = "tksno1";
    std::string strLogin;
    std::string strPass;
    int nGameCode           = 2;
    std::string strFirstName = " ";
    std::string strLastName = " ";
    char cGender[2] = " ";
    char cTemp[2] = "M";
    char cTemp1[2] = "F";
    std::string strEmail = " ";
    std::string strBirthDay = " ";
    time_t tBirthday = time_t(NULL);
    CTime cTime;
    SYSTEMTIME sTime;
//    int i;

//    std::string strTemp;
//    strTemp = KncUtil::GetMD5( strPasswd );

/*
    DBJIF( m_kODBC.Query( L"{ call dbo.GetUserLoginInfo_Test( N'%s',N'%s', %d) }", 
        strwLogin.c_str(),
        strwPasswd.c_str(),
        nGameCode ) );
*/

    for( int i = 0; i < 10; i++)
    {
        DBJIF( m_kODBC.Query( L"Select firstLogin From users Where Login = N'%s' ", 
            strwLogin.c_str() ) );

        if( m_kODBC.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBC 
                >> sTime ); // 여긴 SystemTime 구조로 받아보자

            m_kODBC.EndFetch();
            nOK = 0;
/*
            CString temp;
            temp.Format(_T("%d-%d-%d %d-%d-%d"), sTime.wYear, sTime.wMonth, 
                                            sTime.wDay, sTime.wHour, sTime.wMinute, sTime.wSecond );
*/

//            2079-06-06일 날짜까지는 smalldatatime 형식에서 저장가능
/*
            if( sTime.wYear <= 1970 || sTime.wYear >= 2079 ) 
            {
                sTime.wYear     = 1970;
                sTime.wMonth    = 2;
                sTime.wDay      = 1;
            }
*/

            START_LOG( clog , L" NUSAUTH ENU_VERIFY_ACCOUNT_REQ " )
                      << BUILD_LOGc( sTime.wYear ) << END_LOG;
//                      << BUILD_LOGst( sTime );
//                    << BUILD_LOG( temp.GetString() );
//                    << BUILD_LOGtm( CTime(sTime) );
        }
    }
/*
    DBJIF( m_kODBC.Query( L"Select firstLogin From users Where Login = N'%s' ", 
        strwLogin1.c_str() ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC 
            >> sTime ); // 여긴 SystemTime 구조로 받아보자

        m_kODBC.EndFetch();
        nOK = 0;

        START_LOG(  Info , L" NUSAUTH ENU_VERIFY_ACCOUNT_REQ " )
            << BUILD_LOGtm( CTime(sTime) );
    }
*/

/*
    strwLogin  = L"benitas";
    strwPasswd = L"qazwsxedc";

    DBJIF( m_kODBC.Query( L"{ call dbo.GetUserLoginInfo_Test( N'%s',N'%s', %d) }", 
        strwLogin.c_str(),
        strwPasswd.c_str(),
        nGameCode ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC 
            >> strLogin
            >> strPass
            >> strFirstName
            >> strLastName
            >> cGender
            >> strEmail
            >> strBirthDay ); // 여기는 std::string 으로 받아보자.

        m_kODBC.EndFetch();
        nOK = 0;

        START_LOG(  Info , L" NUSAUTH ENU_VERIFY_ACCOUNT_REQ " )
            << BUILD_LOG( strLogin )
            << BUILD_LOG( strPass )
            << BUILD_LOG( strFirstName )
            << BUILD_LOG( strLastName )
            << BUILD_LOG( cGender )
            << BUILD_LOG( strEmail )
            << BUILD_LOGtm( CTime(tBirthday) );

        // Ntreev에서 받아온 cGender값을 가지고 성별 구분을 체크해야 한다.
        // Ntreev Web DB의 Gender구분인 M, F 값을 신뢰하지만 M, F 이외의 값에 
        // 대한 에러 처리에 대해서는 FALSE로 처리하고 로그를 남긴다.
        //        if( strcmp(cGender, cTemp) == 0 )       kPacket_.m_bMale = true;
        //        else if( strcmp(cGender, cTemp1) == 0 ) kPacket_.m_bMale = false;
        //        else
        //        {
        //            kPacket_.m_bMale = false;
        //            START_LOG( clog , L" Entreev Wrong Gender Data " )
        //                << BUILD_LOG( kPacket_.m_bMale );
        //        }
    }
    else
    {
        // NtreevUS 웹 인증 실패
        //        kPacket.m_nOK = NetError::ERR_NUSER_VERIFY_17;
        START_LOG( clog , L" NTreev Wrong Gender Data " );
    }
*/
/*
    DWORD dwItemUID = 141710;
//    int nOK = 0;

    DBJIF( m_kODBC.Query( L"{ call dbo.up_buy_cash_item_rollback( %d) }", 
      dwItemUID ) );
    // BeginFecth()함수를 사용안하공..,

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC 
            >> nOK
            ); 

        m_kODBC.EndFetch();
    }

    DWORD dwUserUID = 12623;
    int dwUserCash = 0;

    DBJIF( m_kODBC.Query( L"{ call dbo.VCGA_virtual_cash_select( %d) }", 
        dwUserUID ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC 
            >> dwUserCash
                     ); 

        m_kODBC.EndFetch();
    }
*/

/*
    /////////////  Normal condition
    if( !m_kODBC.Query( L"{ call dbo.sup_insert_client_info ( N'%s', N'%s', N'%s', %d, N'%s', %d, %d, N'%s' ) }",

        m_strIP.c_str(), m_strOSType.c_str(), m_strCPU.c_str(),
        m_iMainMemory, m_strGraphicChip.c_str(), m_iVMemory,
        (m_bjoystick)?(1):(0), m_strDXversion.c_str() ) )
    {
        START_LOG( cerr, L"Client Stat Info Query failed(1)..." ) 
            << BUILD_LOG( (&m_kODBC)->m_szDiagMsg ) 
            << BUILD_LOG( (wchar_t*)((&m_kODBC)->m_sqlState) ) 
            << BUILD_LOG( (&m_kODBC)->m_strLastQuery ); 
            
    }
    else
        std::cout <<" Success First Test" << std::endl;


    int nOK = -99;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    //////////////////////////   ① N\'%s\' : 문자열 전체 감싸는 quote에 \ 넣기
    m_strGraphicChip = L"Voodoo 3's";
    if( !m_kODBC.Query( L"{ call dbo.sup_insert_client_info ( N'%s', N'%s', N'%s', %d, N\'%s\', %d, %d, N'%s' ) }",

        m_strIP.c_str(), m_strOSType.c_str(), m_strCPU.c_str(),
        m_iMainMemory, m_strGraphicChip.c_str(), m_iVMemory,
        (m_bjoystick)?(1):(0), m_strDXversion.c_str() ) )
    {
        START_LOG( cerr, L"Client Stat Info Query failed(2)..." ) 
            << BUILD_LOG( (&m_kODBC)->m_szDiagMsg ) 
            << BUILD_LOG( (wchar_t*)((&m_kODBC)->m_sqlState) ) 
            << BUILD_LOG( (&m_kODBC)->m_strLastQuery ); 

    }
    else
        std::cout <<" Success Second Test" << std::endl;

    nOK = -99;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    /////////////////////   ② ~~\'s      : 문자열 내부 quote에 \ 넣기
    m_strGraphicChip = L"Voodoo 3\'s";    
    if( !m_kODBC.Query( L"{ call dbo.sup_insert_client_info ( N'%s', N'%s', N'%s', %d, N'%s', %d, %d, N'%s' ) }",

        m_strIP.c_str(), m_strOSType.c_str(), m_strCPU.c_str(),
        m_iMainMemory, m_strGraphicChip.c_str(), m_iVMemory,
        (m_bjoystick)?(1):(0), m_strDXversion.c_str() ) )
    {
        START_LOG( cerr, L"Client Stat Info Query failed(3)..." ) 
            << BUILD_LOG( (&m_kODBC)->m_szDiagMsg ) 
            << BUILD_LOG( (wchar_t*)((&m_kODBC)->m_sqlState) ) 
            << BUILD_LOG( (&m_kODBC)->m_strLastQuery ); 

    }
    else
        std::cout <<" Success Third Test" << std::endl;

    nOK = -99;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    ////////////////////   ③ ~~\\'s     : 문자열 내부 quote에 \\ 넣기
    m_strGraphicChip = L"Voodoo 3's";
    std::wstring m_strReplace = L"''";
    wchar_t strProhibition = L'\'';
    for( size_t i=0; i < m_strGraphicChip.size() ; ++i )    
    {
        if( m_strGraphicChip[i] == strProhibition )
            m_strGraphicChip.replace( i, 1, L"\''" );
    }
    std::wcout << L"m_strGraphicChip" << m_strGraphicChip << std::endl;

    if( !m_kODBC.Query( L"{ call dbo.sup_insert_client_info ( N'%s', N'%s', N'%s', %d, N'%s', %d, %d, N'%s' ) }",

        m_strIP.c_str(), m_strOSType.c_str(), m_strCPU.c_str(),
        m_iMainMemory, m_strGraphicChip.c_str(), m_iVMemory,
        (m_bjoystick)?(1):(0), m_strDXversion.c_str() ) )
    {
        START_LOG( cerr, L"Client Stat Info Query failed(3)..." ) 
            << BUILD_LOG( (&m_kODBC)->m_szDiagMsg ) 
            << BUILD_LOG( (wchar_t*)((&m_kODBC)->m_sqlState) ) 
            << BUILD_LOG( (&m_kODBC)->m_strLastQuery ); 

    }
    else
    {
        START_LOG( cout, L"Success Third Test( replace ' to '' " ) 
            //<< BUILD_LOG( (&m_kODBC)->m_szDiagMsg ) 
            //<< BUILD_LOG( (wchar_t*)((&m_kODBC)->m_sqlState) ) 
            << BUILD_LOG( (&m_kODBC)->m_strLastQuery ); 
    }
    nOK = -99;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();
    //////////////////////////  double ' test ('')
    m_strGraphicChip = L"Voodoo 3''s";
    if( !m_kODBC.Query( L"{ call dbo.sup_insert_client_info ( N'%s', N'%s', N'%s', %d, N'%s', %d, %d, N'%s' ) }",

        m_strIP.c_str(), m_strOSType.c_str(), m_strCPU.c_str(),
        m_iMainMemory, m_strGraphicChip.c_str(), m_iVMemory,
        (m_bjoystick)?(1):(0), m_strDXversion.c_str() ) )
    {
        START_LOG( cerr, L"Client Stat Info Query failed(3)..." ) 
            << BUILD_LOG( (&m_kODBC)->m_szDiagMsg ) 
            << BUILD_LOG( (wchar_t*)((&m_kODBC)->m_sqlState) ) 
            << BUILD_LOG( (&m_kODBC)->m_strLastQuery ); 

    }
    else
        std::cout <<" Success Fourth Test (double ') " << std::endl;

    nOK = -99;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();
*/


}