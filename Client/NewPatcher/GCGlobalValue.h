#pragma once
#include "stdafx.h"
#include <Registry.h>
#include "define.h"
#include <IPHlpApi.h>                       // for GetAdaptersInfo()

struct SServerSelectInfo {
	std::wstring wstrServerName;
	int iServiceType;
	int iServerState;
	bool bRecomServer;
	bool bNewServer;
	std::wstring strServerIP;
	std::wstring strServerPort;
	std::wstring strDownloadLink;
	std::wstring strDownloadLinkFile;
};

class KGCGlobalValue
{
private:
    static std::string m_strProgramPath;   //게임이 설치된 폴더 위치    
    static std::string m_strSrcPath;       //패치서버의 위치

#if defined( _KOREAN )
    static bool GetProgramPath_Korea();
#elif defined( _LEVELUP_BRAZIL )
    static bool GetProgramPath_Brazil();
#elif defined( _TAIWAN )
    static bool GetProgramPath_Taiwan();
#elif defined( _HONGKONG )
    static bool GetProgramPath_Hongkong();
#elif defined( _USA )
	static bool GetProgramPath_USA();
#elif defined(_THAILAND)
	static bool GetProgramPatch_Thailand();
#elif defined( _INDONESIA )
    static bool GetProgramPath_Indonesia();
#elif defined( _PHILIPPINE )
    static bool GetProgramPath_Philippine();
#elif defined(_LATINAMERICA )
    static bool GetProgramPath_LATINAMERICA();
#elif defined(_EU )
    static bool GetProgramPath_EU();
#elif defined(_CHINA )
	static bool GetProgramPath_CN();
#else
    static bool GetProgramPath_Test();
#endif
    static bool GetProgramPath_Nation();
public:

    static bool IsGameInstalled( KRegistry* kReg_, const std::string& strRegKey_ );
    static bool GetGamePathFromReg( KRegistry* kReg_ , std::string& strPath_ );

    static std::string GetProgramPath();
    static std::string GetSrcPath();
    static bool GetPatcherPath( std::string& strPath_ );
    static bool GetPatchPathFileName( std::string& strFileName_, bool bDirectFTP = false );
    static bool GetPatchTimeFileName( std::string& strFileName_ );


	static std::string GetPatchPathFileName( bool bDirectFTP = false );
	static std::string GetPatchTimeFileName();

    static std::string GetBGWeb();
    static std::string GetBGPWeb();
	static void PopUpWebPage( const WCHAR* strURL_ );

#if defined( _TEST )
    static std::string GetTestSrvIP();
    static std::string GetTestSrvPort();
    static std::string GetDataFromINI( std::string _strData );
#endif

    /// Read patch path file.
    static bool ReadPatchPathFile( char* szSrcPath , std::string& strPatchPathFileName_ );
    static bool ReadPatchPathFile( std::string& strSrcPath , std::string& strPatchPathFileName_ );

	// XTrap Http url Key
	static std::string GetXTrapHttpUrlKey();


    static void Init();

    KGCGlobalValue(void);
    ~KGCGlobalValue(void);

	static DWORD m_dwChannelType;

	static int m_iPatchMode;
    static int m_iGridDownState;
    static char m_szCPCookie[20480];
    static DWORD m_dwPatcherStartTime;

    static bool m_bPreTestPath;
    enum    {   GRID_OK, GRID_FAIL, };


	// FTP DirectAdress;
	static bool m_bReDirectFTP;		// 50메가 이상시 새로 연결할지 여부( FTP에서만 사용 ) 
	static bool m_bFirstRedirectFTP;		// 바로 ReDirectFTP로 붙는다.
	static std::string m_strReDirectFTP;
	static std::string m_strPatchPathReDirect;
	static bool m_bLastUpdateLog;
	static bool m_bCheckDownSpeed;


	//	다국어 패쳐 여부
	static bool m_bMultilingual;
	static int m_nLanguage;

	// 서버 선택
	static bool m_bServerSelectMode;
	static bool IsServerSelectMode(){return m_bServerSelectMode;}
	
	static std::map<int,SServerSelectInfo> m_mapServerInfo; //버튼 인덱스에 복사된 정보.
	static std::vector<SServerSelectInfo> m_vecServerInfo; //원본 정보
	static std::string m_strSelectIP;
	static std::string m_strSelectPort;
	static int m_iSelectServerIndex;
	static std::wstring m_wstrSelectServerName;
	static int m_iSelectServerState;
	static std::string m_strDownLoadLink;
	static std::string m_strDownLoadLinkFileName;
	static bool m_bVersionInfo;
	static std::string m_strVersion;

	static bool LoadServerSelectInfo();
	static bool m_bIsStoped;
	static bool IsStoped();
	static void InitServerSelectInfo();
	static void AddServerInfo(std::wstring strInfo);
	static std::wstring LoadVersionInfo();

	static std::string m_strMacAddress;
	static void GetMacAddress( );
};
