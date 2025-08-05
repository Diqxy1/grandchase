
#include <WinSock2.h>
#pragma warning(push, 2)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)
#include <dbg/dbg.hpp>
#include "Log4.h"


#include "Socket/NetCommon.h"
#include "FTP/FTPClient.h"
#include "FTPFactory.h"

//FILE_NAME_FOR_LOG

bool GetFile( const std::string& strFile, std::vector<char>& buf )
{
    std::ifstream infile ( strFile.c_str() ,std::ifstream::binary);
    if( !infile.is_open() ) return false;

    infile.seekg(0,std::ifstream::end);
    long size = infile.tellg();
    infile.seekg(0);
    buf.resize( size );
    infile.read( &buf[0], size );
    infile.close();
    return true;
}

bool FileDiff( const std::string& strlFile, const std::string& strrFile )
{
    std::vector<char> lbuff, rbuff;
    if( !GetFile( strlFile, lbuff ) ) return false;
    if( !GetFile( strrFile, rbuff ) ) return false;

    return lbuff == rbuff;
}

void TestUseFTPFactory()
{

    std::wcout << L"Test Factory.." << std::endl;

    SiKFTPFactory()->AddConnectionInfo( std::wstring(L"Test"), std::wstring(L"116.120.238.39"), 21, std::wstring(L"grandchase"), std::wstring(L"gpfrpdlxm310TT^^") );
    CFTPClientPtr spFTP = SiKFTPFactory()->GetConnection( std::wstring(L"Test") );
    KFTPFactory::ReleaseInstance(); // 더이상 쓰지 않기 때문에.그냥 미리 릴리즈 해놓는다.

    BOOST_CHECK_EQUAL( spFTP.get() != NULL, true );
    BOOST_CHECK_EQUAL( spFTP->IsConnected(), true );

    std::wcout << L"List Remote Files.." << std::endl;
    nsFTP::TSpFTPFileStatusVector list;
    BOOST_CHECK_EQUAL( spFTP->List(L"/", list), true );

    for( nsFTP::TSpFTPFileStatusVector::iterator it = list.begin() ; it!=list.end() ; ++it )
        std::wcout << L" -- " << (*it)->Name().c_str() << std::endl;

    std::wcout << L"Image File Upload..." << std::endl;
    BOOST_CHECK_EQUAL( spFTP->UploadFile( L".\\kill.gif", L"/kill.gif"), true );
    BOOST_CHECK_EQUAL( spFTP->DownloadFile( L"/kill.gif", L".\\kill_.gif"), true );
    BOOST_CHECK_EQUAL( spFTP->Delete(L"/kill.gif"), nsFTP::FTP_OK );
    BOOST_CHECK_EQUAL( ::_waccess_s( L".\\kill_.gif", 0 ), 0 );
    BOOST_CHECK_EQUAL( FileDiff( std::string(".\\kill.gif"), std::string(".\\kill_.gif") ), true );
    BOOST_CHECK_EQUAL( ::_wremove( L".\\kill_.gif" ), 0 );

    std::wcout << L"Image Memory Upload..." << std::endl;
    std::vector<char> memBuff;
    BOOST_CHECK_EQUAL( GetFile( std::string(".\\kill.gif"), memBuff ), true );
    BOOST_CHECK_EQUAL( memBuff.empty(), false );
    BOOST_CHECK_EQUAL( spFTP->UploadMem( &memBuff[0], memBuff.size(),L"kill.gif"), true );
    BOOST_CHECK_EQUAL( spFTP->DownloadFile( L"/kill.gif", L".\\kill_.gif"), true );
    BOOST_CHECK_EQUAL( spFTP->Delete(L"/kill.gif"), nsFTP::FTP_OK );
    BOOST_CHECK_EQUAL( ::_waccess_s( L".\\kill_.gif", 0 ), 0 );
    BOOST_CHECK_EQUAL( FileDiff( std::string(".\\kill.gif"), std::string(".\\kill_.gif") ), true );
    BOOST_CHECK_EQUAL( ::_wremove( L".\\kill_.gif" ), 0 );


}

void TestFTP()
{
    std::wcout << L"Start FTP Test .. " << std::endl;
    JIF( NetCommon::InitWinSock() );

    nsFTP::CFTPClient ftpClient;
    nsFTP::CLogonInfo logonInfo( L"116.120.238.39", 21, L"grandchase", L"gpfrpdlxm310TT^^");

    BOOST_CHECK_EQUAL( ftpClient.Login(logonInfo), true );
    BOOST_CHECK_EQUAL( ftpClient.IsConnected(), true );

    std::wcout << L"List Remote Files.." << std::endl;
    nsFTP::TSpFTPFileStatusVector list;
    BOOST_CHECK_EQUAL( ftpClient.List(L"/", list), true );

    for( nsFTP::TSpFTPFileStatusVector::iterator it = list.begin() ; it!=list.end() ; ++it )
        std::wcout << L" -- " << (*it)->Name().c_str() << std::endl;

    std::wcout << L"Image File Upload..." << std::endl;
    BOOST_CHECK_EQUAL( ftpClient.UploadFile( L".\\kill.gif", L"/kill.gif"), true );
    BOOST_CHECK_EQUAL( ftpClient.DownloadFile( L"/kill.gif", L".\\kill_.gif"), true );
    BOOST_CHECK_EQUAL( ftpClient.Delete(L"/kill.gif"), nsFTP::FTP_OK );
    BOOST_CHECK_EQUAL( ::_waccess_s( L".\\kill_.gif", 0 ), 0 );
    BOOST_CHECK_EQUAL( FileDiff( std::string(".\\kill.gif"), std::string(".\\kill_.gif") ), true );
    BOOST_CHECK_EQUAL( ::_wremove( L".\\kill_.gif" ), 0 );

    std::wcout << L"Image Memory Upload..." << std::endl;
    std::vector<char> memBuff;
    BOOST_CHECK_EQUAL( GetFile( std::string(".\\kill.gif"), memBuff ), true );
    BOOST_CHECK_EQUAL( memBuff.empty(), false );
    BOOST_CHECK_EQUAL( ftpClient.UploadMem( &memBuff[0], memBuff.size(),L"kill.gif"), true );
    BOOST_CHECK_EQUAL( ftpClient.DownloadFile( L"/kill.gif", L".\\kill_.gif"), true );
    BOOST_CHECK_EQUAL( ftpClient.Delete(L"/kill.gif"), nsFTP::FTP_OK );
    BOOST_CHECK_EQUAL( ::_waccess_s( L".\\kill_.gif", 0 ), 0 );
    BOOST_CHECK_EQUAL( FileDiff( std::string(".\\kill.gif"), std::string(".\\kill_.gif") ), true );
    BOOST_CHECK_EQUAL( ::_wremove( L".\\kill_.gif" ), 0 );

    std::wcout << L"Disconnect FTP" << std::endl;

    ftpClient.Logout();

    TestUseFTPFactory();
    ::WSACleanup();

    std::wcout << L"Finish.." << std::endl;
}