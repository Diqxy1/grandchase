#include "stdafx.h"
#include "CheckHackTool.h"
//
#include "Registry.h"
//

//
#include "MyD3D.h"
#include "GCStateMachine.h"
//
//
//
//

#ifdef UNICODE
#   undef UNICODE
#   include <tlhelp32.h>
#   define UNICODE
#else
#   include <tlhelp32.h>
#endif

/// @class KHackToolName 해킹툴 이름을 체크하는 클래스.
class KHackToolName
{
public:
    /// Constructor.
    /// @param strHackToolName 해킹툴 이름
    /// @param bCheckFullMatch 이름을 완전 매칭할 것인가?
    /// @param strFirstMenuStr 첫번째 메뉴 문자열
    KHackToolName( std::string strHackToolName, bool bCheckFullMatch, std::string strFirstMenuStr )
    {
        m_strHackToolName = strHackToolName;
        m_bCheckFullMatch = bCheckFullMatch;
        m_strFirstMenuStr = strFirstMenuStr;
    }
    /// Check hack-tool name with parameter name.
    /// @param strHackToolName 체크할 해킹툴 이름
    /// @param strFirstMenuStr 체크할 첫번째 메뉴 문자열
    /// @return 이름이 일치하거나 포함되어 있다면 true, 아니면 false.
    bool CheckHackToolName( std::string& strHackToolName, std::string& strFirstMenuStr ) const
    {
		bool bReg = ( false == m_strFirstMenuStr.empty() && ( strFirstMenuStr == m_strFirstMenuStr ) ) ||
			        ( ( true == m_bCheckFullMatch ) ? ( strHackToolName == m_strHackToolName ) :
		              ( std::string::npos != strHackToolName.find( m_strHackToolName ) ) );
        return bReg;
    }
private:
    std::string m_strHackToolName;  ///< 해킹툴 이름
    bool        m_bCheckFullMatch;  ///< 이름을 완전 매칭할 것인가?
    std::string m_strFirstMenuStr;  ///< 첫번째 메뉴 문자열
};

class KHackToolList
{
public:
    static KHackToolList& GetInstance( void )
    {
        static KHackToolList obj;
        return obj;
    }

private:
    KHackToolList( void )
    {
        m_vecHackToolName.clear();
        m_vecHackToolImageName.clear();

        Load();
    }
    ~KHackToolList( void )
    {
        m_vecHackToolName.clear();
        m_vecHackToolImageName.clear();
    }

    void Load( void )
    {
        KLuaManager luaMgr;

        if ( NULL == g_pGCDeviceManager2 )
            return;

        if( GCFUNC::LoadLuaScript( luaMgr, "HackTool.stg" ) == false )
            return;

        if ( SUCCEEDED( luaMgr.BeginTable( "HackToolName" ) ) )
        {
            for ( int i = 1; ; ++i )
            {
                if ( SUCCEEDED( luaMgr.BeginTable( i ) ) )
                {
                    std::string strHackToolName;
                    bool        bCheckFullMatch;
                    std::string strFirstMenuStr;

                    if ( SUCCEEDED( luaMgr.GetValue( 1, strHackToolName ) ) )
                    {
                        if ( FAILED( luaMgr.GetValue( 2, bCheckFullMatch ) ) )
                            bCheckFullMatch = false;

                        if ( FAILED( luaMgr.GetValue( 3, strFirstMenuStr ) ) )
                            strFirstMenuStr = "";

                        m_vecHackToolName.push_back( KHackToolName( strHackToolName,
                                                                    bCheckFullMatch,
                                                                    strFirstMenuStr ) );
                    }

                    if ( FAILED( luaMgr.EndTable() ) )
                        return;
                }
                else break; // no more table
            }

            if ( FAILED( luaMgr.EndTable() ) )
                return;
        }

        if ( SUCCEEDED( luaMgr.BeginTable( "HackToolImageName" ) ) )
        {
            for ( int i = 1; ; ++i )
            {
                std::string strHackToolImageName;

                if ( SUCCEEDED( luaMgr.GetValue( i, strHackToolImageName ) ) )
                {
                    m_vecHackToolImageName.push_back( strHackToolImageName );
                }
                else break; // no more string
            }

            if ( FAILED( luaMgr.EndTable() ) )
                return;
        }
    }

public:
    std::vector<KHackToolName>  m_vecHackToolName;
    std::vector<std::string>    m_vecHackToolImageName;
};

static bool         g_bExist = false;   ///< Does hack-tool exist?
static std::string  g_strArtMoneyPath;  ///< ArtMoney Program Path
static int          g_iArtMoneyBtnCnt;  ///< ArtMoney Button Count
static int          g_iGameMasterPanel; ///< Game Master Panel Count

/// An application-defined callback function used with the EnumChildWindows function.
/// @param hWnd Handle to child window
/// @param lParam Specifies the application-defined value given in EnumChildWindows
/// @return TRUE to continue enumeration and FALSE to stop enumeration
static BOOL CALLBACK EnumChildProc( HWND hWnd, LPARAM lParam )
{
    UNREFERENCED_PARAMETER( lParam );

    char szWndTitle[1024] = { 0, };
    ::GetWindowTextA( hWnd, szWndTitle, 1024 );
    char szClassName[1024] = { 0, };
    ::GetClassNameA( hWnd, szClassName, 1024 );

    // Check ArtMoney button
    RECT rc = { 0, };
    ::GetClientRect( hWnd, &rc );
    if ( 0 == rc.left && 0 == rc.top && 100 == rc.right && 25 == rc.bottom )
    {
        g_iArtMoneyBtnCnt++;
    }

    // NOTE : 여기 있는 하드코딩들은 어찌 빼내기가 곤란하다. (ㅡㅡ;)a

    // Check GameMaster
    if ( 0 == ::strcmp( "TPanel", szClassName ) && rc.right == 481 )
    {
        g_iGameMasterPanel++;
    }
    // Check TMK
    if ( 0 == ::strcmp( "AfxControlBar42s", szClassName ) && 0 == ::strcmp( "Trainer Window", szWndTitle ) )
    {
        g_bExist = true;
        return FALSE; // stop
    }
    // Check user-developed program
    if ( std::string::npos != std::string( szWndTitle ).find( "옷을 입힐때는" ) ) // 옷입히기 프로그램
    {
        g_bExist = true;
        return FALSE; // stop
    }
    // Check ArtMoney button and string
    if ( g_iArtMoneyBtnCnt > 6 ||
         g_iGameMasterPanel > 6 ||
         0 == ::strcmp( "기력 보충", szWndTitle ) ||                            // next cocoro
         0 == ::strcmp( "체력 보충", szWndTitle ) ||                            // next cocoro
         0 == ::strcmp( "윈핵", szWndTitle ) ||                                 // cocoro
         0 == ::strcmp( "강퇴무한", szWndTitle ) ||                             // cocoro
         0 == ::strcmp( "편가르기", szWndTitle ) ||                             // cocoro
         0 == ::strcmp( "@ Password @", szWndTitle ) ||                         // Free-Hack
         0 == ::strcmp( "In X Zone", szWndTitle ) ||                            // 강제 창 내리기
         0 == ::strcmp( "Use Interrupt function to Run Game", szWndTitle ) ||   // Game Master
         0 == ::strcmp( "Speed Up the Application", szWndTitle ) )              // Game Master
    {
        g_bExist = true;
        return FALSE; // stop
    }

    return TRUE; // continue
}

/// An application-defined callback function used with the EnumWindows function.
/// @param hWnd Handle to a top-level window
/// @param lParam Specifies the application-defined value given in EnumWindows
/// @return TRUE to continue enumeration and FALSE to stop enumeration
static BOOL CALLBACK EnumWindowsProc( HWND hWnd, LPARAM lParam )
{
    UNREFERENCED_PARAMETER( lParam );

    // Check child windows
    g_iArtMoneyBtnCnt = 0;
    g_iGameMasterPanel = 0;
    ::EnumChildWindows( hWnd, EnumChildProc, 0 );
    if ( true == g_bExist )
        return FALSE; // stop

    // Get the text of the specified window's title bar (if it has one) into a buffer
    char szWndTitle[1024] = { 0, };
    ::GetWindowTextA( hWnd, szWndTitle, 1024 );
    char szMenuString[1024] = { 0, };
    HMENU hMenu = ::GetMenu( hWnd );
    if ( hMenu != NULL )
        ::GetMenuStringA( hMenu, 0, szMenuString, 1024, MF_BYPOSITION );

    // Search hack-tool program
    std::string strWndTitle = szWndTitle;
    std::string stdMenu     = szMenuString;
    for ( std::size_t i = 0; i < KHackToolList::GetInstance().m_vecHackToolName.size(); ++i )
    {
        if ( KHackToolList::GetInstance().m_vecHackToolName[i].CheckHackToolName( strWndTitle, stdMenu ) )
        {
            g_bExist = true;
            return FALSE; // stop
        }
    }

    return TRUE; // continue
}

/// Check executable file image name of hack-tool process.
/// @param void
/// @return true if a hack-tool process exist, and false if it do not
static bool CheckProcessImage( void )
{
    // Create process snapshot
    HANDLE          hProcessSnapShot = ::CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    PROCESSENTRY32  pProcessEntry    = { 0, };

    // Set size of struct
    pProcessEntry.dwSize = sizeof( PROCESSENTRY32 );

    // Get first process
    if ( FALSE == ::Process32First( hProcessSnapShot, &pProcessEntry ) )
    {
        // Close process snapshot
        ::CloseHandle( hProcessSnapShot );

        return false; // Not found
    }

    // Search hack-tool process image
    do
    {
        // Check executable file name
        for ( std::size_t i = 0 ; i < KHackToolList::GetInstance().m_vecHackToolImageName.size(); ++i )
        {
            std::string strHackToolImageName = KHackToolList::GetInstance().m_vecHackToolImageName[i];

            // Perform a lowercase comparison of strings
            if ( 0 == ::_stricmp( pProcessEntry.szExeFile, strHackToolImageName.c_str() ) )
            {
                // Close process snapshot
				g_kGlobalValue.m_strDetectedHackTool = GCUTIL_STR::GCStrCharToWide( strHackToolImageName.c_str() );
                ::CloseHandle( hProcessSnapShot );

                return true; // Found
            }
        }

        // If ArtMoney is not installed, find next process
        if ( true == g_strArtMoneyPath.empty() ) continue;

        // Create module snapshot
        HANDLE          hModuleSnapShot = ::CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, pProcessEntry.th32ProcessID );
        MODULEENTRY32   pModuleEntry    = { 0, };

        // Set size of struct
        pModuleEntry.dwSize = sizeof( MODULEENTRY32 );

        // Get first module
        if ( FALSE == ::Module32First( hModuleSnapShot, &pModuleEntry ) )
        {
            // Close module snapshot
            ::CloseHandle( hModuleSnapShot );

            continue; // Find next process
        }

        // Check module location
        do
        {
            // If a module location is equal to install location of ArtMoney
            if ( std::string::npos != std::string( pModuleEntry.szExePath ).find( g_strArtMoneyPath ) )
            {
                // Close snapshot
                ::CloseHandle( hModuleSnapShot );
                ::CloseHandle( hProcessSnapShot );

                return true; // Found
            }
        }
        while ( TRUE == ::Module32Next( hModuleSnapShot, &pModuleEntry ) ); // Find next module

        // Close module snapshot
        ::CloseHandle( hModuleSnapShot );
    }
    while ( TRUE == ::Process32Next( hProcessSnapShot, &pProcessEntry ) ); // Find next process

    // Close process snapshot
    ::CloseHandle( hProcessSnapShot );

    return false; // Not found
}

/// Check existence of hack-tool.
/// @param void
/// @return true if a hack-tool exist, and false if it do not
bool CheckHackTool( void )
{
    // 아직 해킹툴 리스트를 읽을 수 있는 상황이 아니므로 통과한다.
    // 로딩이 다 끝난 후에 해킹툴 리스트를 읽어야 한다.
    if ( NULL == g_pGCDeviceManager2 ||
         NULL == g_MyD3D->m_pStateMachine ||
         GS_INIT_LOADING == g_MyD3D->m_pStateMachine->GetState() )
        return false;

    // Check Process image name
    if ( true == CheckProcessImage() )
        return true;

    g_bExist = false; // initialize

    // Enumerates all top-level windows on the screen
    ::EnumWindows( EnumWindowsProc, NULL );

    return g_bExist; // return result
}

/// Is ArtMoney Installed?
/// @param void
/// @return true if ArtMoney is installed, and false if it is not
bool IsArtMoneyInstalled( void )
{
    KRegistry kReg;

    // Open registry key of ArtMoney
    bool bRet = kReg.OpenKey( KRegistry::localMachine,
                              "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\ArtMoney\\" );

    if ( true == bRet ) // if ArtMoney is installed
    {
        // Get install location of ArtMoney
        kReg.GetValue( "InstallLocation", g_strArtMoneyPath );
    }

    return bRet; // return result
}

/** @example CheckHackTool

#include "CheckHackTool.h"

return_type function_name( argument_list )
{
    // do something...

    if ( true == IsArtMoneyInstalled() )
    {
        // do something...
    }
    else
    {
        // do something...
    }

    // do something...

    if ( true == CheckHackTool() )
    {
        // do something...
    }
    else
    {
        // do something...
    }

    // do something...
}

*/
