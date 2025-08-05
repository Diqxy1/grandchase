#include <WinSock2.h>
#include "BaseServer.h"
#include "dbg/dbg.hpp"
#include "NetError.h"
#include "ThreadManager.h"
#include <KncLua.h>
#include <time.h>
#include <KncUtil.h>
#include <sstream>
#include "socket/NetCommon.h"
#include "odbc/Odbc.h"
#include "NetLayer.h"
#include "SimLayer.h"
#include "DBLayer.h"
#include "SubThread.h"
#include <signal.h>
#include <boost/lexical_cast.hpp> // lexical_cast
#include "Log4.h"

#ifdef NDEBUG
#   include "CrashRpt/KncCrashRpt.h"
#   pragma comment(lib, "../Common/Crashrpt/CrashRpt.lib")

inline void terminator()
{
	int*z = 0; *z = 13;
}

inline void signal_handler(int)
{
	terminator();
}

inline void __cdecl invalid_parameter_handler(PCTSTR expression_, PCTSTR function_, PCTSTR file_,
	unsigned int line_, uintptr_t pReserved_)
{
	START_LOG(cerr, L"Exceptions..")
		<< BUILD_LOG(expression_)
		<< BUILD_LOG(function_)
		<< BUILD_LOG(file_)
		<< BUILD_LOG(line_) << END_LOG;
	terminator();
}

BOOL CALLBACK CrashCallback(LPVOID lpvState)
{
	if (GetExcetionType(lpvState) != EXCEPTION_STACK_OVERFLOW) {
		std::ostringstream stm;

		stm << "Local IP : " << NetCommon::GetLocalIP() << std::endl
			<< "GetTickCount : " << ::GetTickCount() << std::endl;

		AppendToMailContent(lpvState, stm.str().c_str());

		// get post-crash execute file's full path.
		char szBuff[MAX_PATH] = { 0 };
		::GetCurrentDirectoryA(MAX_PATH, szBuff);
		::strcat(szBuff, "\\DumpUploader.exe");

		AddFile(lpvState, "odbc_main.dsn", "odbc");

		stm.str("");  // 초기화 - 버퍼의 내용을 인자로 받은 문자열로 대치.

		// build command line parameter
		stm << "/s " << NetCommon::GetLocalIP()             // 서버 이름 (아이피)
			//<< " /f " << std::logfile::GetLogFileName()     // 로그파일 이름
			<< " /f " << GetFullDumpFileName(lpvState);   // 덤프파일 이름
		//std::logfile::CloseFile(); // HTML 로그 파일을 닫는다.

		ShellExecuteA(NULL, "open", szBuff, stm.str().c_str(), NULL, SW_SHOWNORMAL);
	}

	return true;
}

LPTOP_LEVEL_EXCEPTION_FILTER WINAPI MyDummySetUnhandledExceptionFilter(
	LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
{
	// TODO: MiniDumpWriteDump
	return NULL;
}

BOOL PreventSetUnhandledExceptionFilter()
{
	HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));

	if (hKernel32 == NULL)
		return FALSE;
	void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");

	if (pOrgEntry == NULL)
		return FALSE;

	unsigned char newJump[100];
	DWORD dwOrgEntryAddr = (DWORD)pOrgEntry;
	dwOrgEntryAddr += 5; // add 5 for 5 op-codes for jmp far
	void *pNewFunc = &MyDummySetUnhandledExceptionFilter;
	DWORD dwNewEntryAddr = (DWORD)pNewFunc;
	DWORD dwRelativeAddr = dwNewEntryAddr - dwOrgEntryAddr;

	newJump[0] = 0xE9;  // JMP absolute
	memcpy(&newJump[1], &dwRelativeAddr, sizeof(pNewFunc));
	SIZE_T bytesWritten;
	BOOL bRet = WriteProcessMemory(GetCurrentProcess(),
		pOrgEntry, newJump, sizeof(pNewFunc) + 1, &bytesWritten);
	return bRet;
}

#endif

NiImplementRootRTTI(KBaseServer);

// 060206. kkurrung. 서버 리스트 리프레시 간격
DWORD KBaseServer::ms_dwSendServerListGap(60 * 1000);

KBaseServer::KBaseServer(void)
	:m_strTagVersion(L"Tag version undefined.")
	, m_nProtocolVer(0)
	, m_nProtocolVer2(0)
	, m_pkSimLayer(NULL)
	, m_pkNetLayer(NULL)
	, m_pkDBLayer(NULL)
	, m_nServerID(0)
{
#ifdef NDEBUG
	signal(SIGABRT, signal_handler);
	_set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);

	set_terminate(&terminator);
	set_unexpected(&terminator);
	_set_purecall_handler(&terminator);
	_set_invalid_parameter_handler(&invalid_parameter_handler);
#endif
	srand((unsigned)time(NULL));
	m_dwServerInfoUpdateTick = ::GetTickCount();
	//std::profiler::init_profiler();
	//
	m_vecServerInfoList.clear();
	m_vecNoUseTCPServerInfoList.clear();
}

KBaseServer::~KBaseServer(void)
{
	//std::profiler::kill_profiler( "profile_info.htm" );
}

ImplToStringW(KBaseServer)
{
	size_t uiServerList;
	{
		KLocker lock(m_csServerInfo);
		uiServerList = m_vecServerInfoList.size();
	}

	START_TOSTRINGW
		<< TOSTRINGW(m_strServerName)
		<< TOSTRINGW(m_strTagVersion)
		<< TOSTRINGW(uiServerList)
		<< TOSTRINGW(m_nProtocolVer)
		<< TOSTRINGW(m_nProtocolVer2)
		<< TOSTRINGW(m_nServerID)
		<< TOSTRINGA2W(m_locale.name());

	//     m_stringTable.Dump( stm_ );

	return stm_;
}

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

void KBaseServer::RegToLua()
{
	luabind::module(m_sptlua.get())
		[
			//luabind::def( "log", &std::logfile::SetLogLevel ),
			//luabind::def( "changelog", &std::logfile::CloseFile ),
			luabind::def("GetLocalIP", &NetCommon::GetLocalIP),

			luabind::class_<KThreadManager>("KThreadManager")
			.def("Init", &KThreadManager::Init)
		.def("ResetMaxProcessingInfo", &KThreadManager::ResetMaxProcessingInfo)
		.def("SetProcessing", &KThreadManager::SetProcessing)
		.def("SetThreadNum", &KThreadManager::SetThreadNum)
		.def("ClearQueue", &KThreadManager::ClearQueue),

		luabind::def("ResetMaxTimeQuery", &KODBC::ResetMaxTime),
		luabind::class_<KODBC>("KODBC")
		.def(luabind::tostring(luabind::self))
		.def("recon", &KODBC::ReConnect),

		luabind::class_<KBaseServer>("KBaseServer")
		.def("SetProtocolVersion", &KBaseServer::SetProtocolVersion)
		.def("SetProtocolVersion2", &KBaseServer::SetProtocolVersion2)
		.def("ReLoadStringTable", &KBaseServer::ReLoadStringTable)
		.def("InitLog4cxx", &KBaseServer::InitLog4cxx)
		.def("SetLogLevel", &KBaseServer::SetLogLevel)
		];
}

bool KBaseServer::Init(const wchar_t* szFileName_)
{
	m_locale = std::locale("");
	std::cout.imbue(m_locale);
	std::wcout.imbue(m_locale);

	//{{ 050810. jseop. tag 버전 설정.
	// 실행 파일 생성 시각 얻기.
	SetTagVersion();
	//}} 050810. jseop. tag 버전 설정.

	// set locale

	//std::locale::global( m_locale );

	// init log file
	//std::logfile::SetFileNameAtTime( true );

	//Enable_LFH();
	//Enable_LFH_crt();

	// socket init
	_JIF(NetCommon::InitWinSock(), return false);

	m_sptlua = std::shared_ptr<lua_State>( lua_open(), lua_close );

	_JIF(m_sptlua != NULL, return false);

	luaopen_base(m_sptlua.get());  // 루아함수 tonumber를 사용해야 한다.

	luabind::open(m_sptlua.get());

	RegToLua();

	m_pkDBLayer->RegToLua(m_sptlua);
	m_pkSimLayer->RegToLua(m_sptlua);
	m_pkNetLayer->RegToLua(m_sptlua);

	LoadINIFile(szFileName_);
	SetCrashMail();

	//InitLog4cxx();

#ifdef NDEBUG

	BOOL bRet = PreventSetUnhandledExceptionFilter();

	if (!bRet) {
		START_LOG(cout, L"Failed PreventSetUnhandledExceptionFilter") << END_LOG;
	}
#endif
	// 050317. config 파일이 lua, ini 두가지로 늘어남. 확장자는 별도로 붙인다.
	std::string strLuaFileName = KncUtil::toNarrowString(szFileName_);
	strLuaFileName += ".lua";

	_JIF(0 == lua_dofile(m_sptlua.get(), strLuaFileName.c_str()), lua_pop(m_sptlua.get(), -1); return false);

	if (NetError::GetLastNetError() != NetError::NET_OK)
		return false;

	m_pkSimLayer->Init();
	m_pkNetLayer->Init();

	m_pkDBLayer->BeginThread();

	m_pkSimLayer->Begin();

	return true;
}

void KBaseServer::Run(bool bRunInterpreter_)
{
	if (bRunInterpreter_)  lua_interpreter(m_sptlua.get());
}

void KBaseServer::ShutDown()
{
	m_pkSimLayer->ShutDown();
	m_pkNetLayer->ShutDown();
	m_pkDBLayer->EndThread();

	::WSACleanup();
}

void KBaseServer::SetTagVersion()
{
	KncUtil::GetTimeStampByNameW(m_strTagVersion);

	// 제목은 처음 초기화때 출력하므로, 아직 아무 로그도 출력하지 않은 상태여야 한다.
	//std::logfile::SetLogFileTitle( m_strTagVersion.c_str() );
}

bool KBaseServer::SetServerName(IN const std::wstring& strServerName_)
{
	KLocker lock(m_csServerName);

	bool bTemp = false;
	if (m_strServerName != strServerName_) {
		bTemp = true;
	}
	m_strServerName = strServerName_;
	if (bTemp) {
		return true;
	}
	return false;
}

void KBaseServer::GetServerName(OUT std::wstring& strServerName_)
{
	KLocker lock(m_csServerName);

	strServerName_ = m_strServerName;
}

std::wstring KBaseServer::GetServerName()
{
	KLocker lock(m_csServerName);
	return m_strServerName;
}

namespace ConfigINI
{
	/*//// ...in ini file... ////////////////////////////////////////////////////

	[Version]
	Protocol Version 1 = XXX
	Protocol Version 2 = YYY

	[String]
	aaa = AAA
	bbb = BBB
	...

	/////////////////////////////////////////////////////////////////////////*/

	const wchar_t* szSectionVersion = L"Version";
	const wchar_t* szSectionString = L"String";
	const wchar_t* szSectionMail = L"Mail";
	const wchar_t* szKeyVersion1 = L"Protocol Version 1";
	const wchar_t* szKeyVersion2 = L"Protocol Version 2";
}

void KBaseServer::LoadINIFile(const wchar_t* szFileName_)
{
	// 050318. florist. ini 파일은 전체 경로(혹은 상대경로)를 적어서 사용해야 한다.
	// 그렇지 않으면 무조건 윈도우 폴더 안에서만 찾는다. 이왕 해야 할거면 확실하게 절대경로를 쓰자.
	wchar_t buff[MAX_PATH] = { 0 };
	::GetCurrentDirectoryW(MAX_PATH, buff);
	m_strIniFilePath = buff;
	m_strIniFilePath += L"\\";
	m_strIniFilePath += szFileName_;
	m_strIniFilePath += L".ini";

	// 050317. version 정보 얻어오기.
	m_nProtocolVer = ::GetPrivateProfileIntW(ConfigINI::szSectionVersion, ConfigINI::szKeyVersion1, 0, m_strIniFilePath.c_str());
	m_nProtocolVer2 = ::GetPrivateProfileIntW(ConfigINI::szSectionVersion, ConfigINI::szKeyVersion2, 0, m_strIniFilePath.c_str());

	if (m_nProtocolVer == 0 || m_nProtocolVer2 == 0)
	{
		START_LOG(cerr, L"버전 정보를 얻어오지 못했습니다.")
			<< BUILD_LOG(m_strIniFilePath)
			<< BUILD_LOG(m_nProtocolVer)
			<< BUILD_LOG(m_nProtocolVer2)
			<< END_LOG;
	}

	//{{ 060201. kkurrung. 문자열 테이블
	if (!m_stringTable.LoadIni(m_strIniFilePath.c_str()))
	{
		START_LOG(cerr, L"INI 파일을 읽어 오지 못했습니다.")
			<< BUILD_LOG(m_strIniFilePath)
			<< END_LOG;
	}

	m_stringTable.SetCurrentSection(ConfigINI::szSectionString);

	//}} kkurrung.
}

void KBaseServer::ReLoadStringTable()
{
	m_stringTable.ReLoadIni();
}

void KBaseServer::SetProtocolVersion(int nProtocolVer_)
{
	START_LOG(cerr, L"change m_nProtocolVer (" << m_nProtocolVer << L" -> " << nProtocolVer_ << L")") << END_LOG;

	m_nProtocolVer = nProtocolVer_;

	wchar_t buff[32] = { 0 };
	LIF(::WritePrivateProfileStringW(ConfigINI::szSectionVersion, ConfigINI::szKeyVersion1,
		::_itow(m_nProtocolVer, buff, 10), m_strIniFilePath.c_str()));
}

void KBaseServer::SetProtocolVersion2(int nProtocolVer_)
{
	START_LOG(cerr, L"change m_nProtocolVer2 (" << m_nProtocolVer2 << L" -> " << nProtocolVer_ << L")") << END_LOG;

	m_nProtocolVer2 = nProtocolVer_;

	wchar_t buff[32] = { 0 };
	LIF(::WritePrivateProfileStringW(ConfigINI::szSectionVersion, ConfigINI::szKeyVersion2,
		::_itow(m_nProtocolVer2, buff, 10), m_strIniFilePath.c_str()));
}

void KBaseServer::SetServerList(IN const std::vector< KServerInfo >& vecServerList_)
{
	KLocker lock(m_csServerInfo);
	m_vecServerInfoList = vecServerList_;
}

void KBaseServer::GetServerList(OUT std::vector< KServerInfo >& vecServerList_)
{
	KLocker lock(m_csServerInfo);
	vecServerList_ = m_vecServerInfoList;
}

void InvalidParameterHandler(PCTSTR expression_, PCTSTR function_, PCTSTR file_, unsigned int line_, uintptr_t pReserved_)
{
}

void KBaseServer::SetServerID(IN int& nServerID_)
{
	KLocker lock(m_csServerName);
	m_nServerID = nServerID_;
}

int KBaseServer::GetServerID()
{
	KLocker lock(m_csServerName);
	return m_nServerID;
}

void KBaseServer::GetServerPort(OUT std::wstring& strServerPort)
{
	strServerPort = boost::lexical_cast<std::wstring>( m_pkNetLayer->m_usPortMaster ); // DO NOT change it
}

void KBaseServer::SetCrashMail()
{
	// 050716. crash report 설정 부분을 main.cpp에서 simlayer로 옮김 - 태그 제목을 스크립트로 수정하기 위함.
#ifdef NDEBUG
	std::string strTagVersion = KncUtil::toNarrowString(m_strTagVersion);
	std::string strMailSubject = "[CRASH]" + strTagVersion;

	std::string strMail;
	std::vector<std::wstring> vecKeys;
	size_t keySize = m_stringTable.GetAllKeys(ConfigINI::szSectionMail, vecKeys);
	int nLeftSize = vecKeys.size();
	std::vector<std::wstring>::iterator vit;
	for (vit = vecKeys.begin(); vit != vecKeys.end(); ++vit) {
		std::string strTmp = KncUtil::toNarrowString(*vit);
		strMail.insert(strMail.size(), strTmp);

		--nLeftSize;
		if (nLeftSize > 0) {
			strMail.insert(strMail.size(), ", ");
		}
	}

	LPVOID lpvState = Install(CrashCallback, strMail.c_str(), strMailSubject.c_str());
#endif
}

void KBaseServer::DumpServerList()
{
	KLocker lock(m_csServerInfo);

	START_LOG(clog, L"=== Dump ServerList (size:" << m_vecServerInfoList.size() << L") ===") << END_LOG;

	std::vector<KServerInfo>::const_iterator cvit;
	for (cvit = m_vecServerInfoList.begin(); cvit != m_vecServerInfoList.end(); ++cvit) {
		START_LOG(clog, L"" << cvit->Str()) << END_LOG;
	}
	START_LOG(clog, L"=== End Dump ===") << END_LOG;
}

void KBaseServer::InitLog4cxx()
{
	std::string propertyFileName = "./log4j.properties";
	log4cxx::PropertyConfigurator::configure(propertyFileName);

	START_LOG(cerr, L"LOG4CXX_LOAD_COMPLETE") << END_LOG;
}

void KBaseServer::SetLogLevel(IN const int nLogLevel_)
{
	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("GCLog"));

	switch (nLogLevel_)
	{
	case -1:
		logger->setLevel(log4cxx::Level::getOff());
		break;

	case 0:
		logger->setLevel(log4cxx::Level::getFatal());
		break;

	case 1:
		logger->setLevel(log4cxx::Level::getError());
		break;

	case 2:
		logger->setLevel(log4cxx::Level::getWarn());
		break;

	case 3:
		logger->setLevel(log4cxx::Level::getInfo());
		break;

	case 4:
		logger->setLevel(log4cxx::Level::getDebug());
		break;

	case 5:
		logger->setLevel(log4cxx::Level::getAll());
		break;

	default:
		logger->setLevel(log4cxx::Level::getError());
		break;
	}

	START_LOG(cerr, L"Loglevel changed to: " << nLogLevel_) << END_LOG;
}

void KBaseServer::SetTCPServerList(IN const std::vector< KSimpleServerInfo >& vecUseTCPServerList_, IN const std::vector< KSimpleServerInfo >& vecNoUseTCPServerList_)
{
	KLocker lock(m_csServerInfo);
	m_vecUseTCPServerInfoList = vecUseTCPServerList_;
	m_vecNoUseTCPServerInfoList = vecNoUseTCPServerList_;
}

void KBaseServer::GetTCPServerList(OUT std::vector< KSimpleServerInfo >& vecUseTCPServerList_, OUT std::vector< KSimpleServerInfo >& vecNoUseTCPServerList_)
{
	vecUseTCPServerList_.clear();
	vecNoUseTCPServerList_.clear();

	KLocker lock(m_csServerInfo);
	vecUseTCPServerList_ = m_vecUseTCPServerInfoList;
	vecNoUseTCPServerList_ = m_vecNoUseTCPServerInfoList;
}