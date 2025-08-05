#include "stdafx.h"
//#include "P2P.h"
//#include "define.h"
//#include "GCGlobal.h"
#include "MyD3D.h"
#include "Procedure.h"
#include "KDlgPrivateInfo.h"
#include "VersionDef.h"
#include "KncFirewall.h"
#include <cmath>
#include "KncUtil.h"
#include "NProtect.h"
#include "NetError.h"
#include "SystemAnalysis/SystemAnalysis.h"
#include "KDInput.h"
#include "KSingleton.h"
#include <MMSystem.h>
#include "KncP2PLib/RakNetSrc/SHA1.h"

// 인증 라이브러리 헤더 포함
#ifdef NATION_KOREA
#   include "NMCrypt.h"                      // 한국 - 넷마블
#elif defined( NATION_JAPAN )
#   include "./Support/MD5/MD5Checksum.h"    // 일본 - 넷마블
#   include "NMCOClientObject.h"             // 일본 - 넥슨재팬
#endif

#define MAX_FILE_SHA1_BUFFER 1024

// pow를 사용하므로, #include <cmath> 해주어야 한다.
inline long hstol(char* str) // 넷마블 옛 인증방식에 쓰이던 함수. hex 문자열을 숫자로 바꿔준다.
{
	if (str == NULL) return 0L;

	int     len = (int)::strlen(str);
	long    ret = 0L;
	long    p;
	long    index;
	int     i, j;

	if (len >= 3)
	{
		if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
		{
			len -= 2;
			memmove(str, &str[2], len);
		}
	}

	for (i = len - 1, j = 0; i >= 0; --i, ++j)
	{
		p = (long)pow(float(16), j);
		{
			if ('0' <= str[i] && str[i] <= '9')
				index = str[i] - '0';
			else if ('a' <= str[i] && str[i] <= 'f')
				index = str[i] - 'a' + 10;
			else if ('A' <= str[i] && str[i] <= 'F')
				index = str[i] - 'A' + 10;
			else
				index = 0;
		}
		ret += p * index;
	}
	return ret;
}

DWORD GetFileCheckSum(char* szFileName = NULL)
{
	if (szFileName == NULL)
	{
		// 051027. jseop. 실행 파일 체크섬 검사.
		static char __szFileName[MAX_PATH];
		if (::GetModuleFileNameA(NULL, __szFileName, MAX_PATH) == 0)
			return 0;

		szFileName = __szFileName;
	}

	char szExeCheckSum[16];
	DWORD dwExeCheckSum = 0;

	lstrcpyA(szExeCheckSum, CRC.GetCRCFromFile(szFileName).c_str());
	dwExeCheckSum = (DWORD)hstol(szExeCheckSum);

	return dwExeCheckSum;
}

std::wstring GetSHA1FromFile(char* strFilePath_)
{
	char strSHACheckSum[MAX_FILE_SHA1_BUFFER] = { 0, };
	CSHA1 cSHA1;
	cSHA1.HashFile(strFilePath_);
	cSHA1.Final();
	cSHA1.ReportHash(strSHACheckSum, CSHA1::REPORT_HEX, true);

	return GCUTIL_STR::GCStrCharToWide(strSHACheckSum);
}

bool RecheckFileSHA()
{
	CRITICAL_SECTION cs;
	InitializeCriticalSection(&cs);

	KLocker csLock(cs);

	//사용할 백터들
	//std::vector< std::string >					m_vecSHAFilePathList;
	//std::vector< DWORD >							m_vecSHAFileSizeList;
	//std::vector< std::string >					m_vecSHAFileSHA;

	if (g_kGlobalValue.m_mapModifiedFile.size() > 0)
	{
		//목록 있던거 다시 돌면거 검색한다.
		for (int i = 0; i < (int)g_kGlobalValue.m_vecSHAFileSHA.size(); i++)
		{
			char* chFilePath_ = (LPSTR)(LPCSTR)GCUTIL_STR::GCStrWideToChar(g_kGlobalValue.m_vecSHAFilePathList[i].c_str());
			std::wstring strSHAChecksum = GetSHA1FromFile(chFilePath_);
			DWORD dwFileSize = (DWORD)GCFUNC::GetRealFileSize(GCUTIL_STR::GCStrWideToChar(g_kGlobalValue.m_vecSHAFilePathList[i].c_str()));

			if (g_kGlobalValue.m_vecSHAFileSHA[i] != strSHAChecksum || g_kGlobalValue.m_vecSHAFileSizeList[i] != dwFileSize)
				return false;
		}

		return true;
	}
	return true;
}

void CheckFileSizeAndSHA(KENU_VERIFY_ACCOUNT_REQ* kReq_, std::wstring strFileName_, std::wstring strFilePath_)
{
	KScriptCheckInfo kScriptCheck;

	char* chFilePath_ = (LPSTR)(LPCSTR)GCUTIL_STR::GCStrWideToChar(strFilePath_.c_str());
	std::wstring strSHAChecksum = kScriptCheck.m_strHash = GetSHA1FromFile(chFilePath_);
	DWORD dwFileSize = (DWORD)GCFUNC::GetRealFileSize(GCUTIL_STR::GCStrWideToChar(strFilePath_.c_str()));

	kScriptCheck.m_strHash = strSHAChecksum;
	kScriptCheck.m_dwFileSize = dwFileSize;

	g_kGlobalValue.m_vecSHAFilePathList.push_back(strFilePath_);
	g_kGlobalValue.m_vecSHAFileSizeList.push_back(kScriptCheck.m_dwFileSize);
	g_kGlobalValue.m_vecSHAFileSHA.push_back(kScriptCheck.m_strHash);

	kReq_->m_mapScriptCheckInfo.insert(std::map<std::wstring, KScriptCheckInfo>::value_type(strFileName_, kScriptCheck));

	g_kGlobalValue.m_mapModifiedFile.insert(std::map<std::wstring, std::string>::value_type(strFileName_, GCUTIL_STR::GCStrWideToChar(strFilePath_.c_str())));
}

void SetExtendSHAList(KENU_VERIFY_ACCOUNT_REQ* kReq_)
{
	if (g_kGlobalValue.m_vecExtendSHAList.empty())
		return;
	std::vector<std::wstring>::iterator Iter = g_kGlobalValue.m_vecExtendSHAList.begin();

	for (Iter; Iter != g_kGlobalValue.m_vecExtendSHAList.end(); ++Iter) {
		KScriptCheckInfo kScriptCheck;

		char* chFilePath_ = (LPSTR)(LPCSTR)GCUTIL_STR::GCStrWideToChar(Iter->c_str());
		std::wstring strSHAChecksum = kScriptCheck.m_strHash = GetSHA1FromFile(chFilePath_);
		DWORD dwFileSize = (DWORD)GCFUNC::GetRealFileSize(GCUTIL_STR::GCStrWideToChar(Iter->c_str()));

		kScriptCheck.m_strHash = strSHAChecksum;
		kScriptCheck.m_dwFileSize = dwFileSize;

		std::wstring strTmp(Iter->c_str());

		std::wstring strFileName;

		size_t pos = strTmp.rfind(L"/");

		strFileName = strTmp.substr(pos + 1);

		kReq_->m_mapScriptCheckInfo.insert(std::map<std::wstring, KScriptCheckInfo>::value_type(strFileName, kScriptCheck));
		g_kGlobalValue.m_mapModifiedFile.insert(std::map<std::wstring, std::string>::value_type(strFileName, GCUTIL_STR::GCStrWideToChar(Iter->c_str())));
	}
}

void SendCheckFileSizeAndSHA(KENU_VERIFY_ACCOUNT_REQ* kReq_)
{
	/*시간 체크를 위한 코드
	//  1초당클럭혹은진동수
	LARGE_INTEGER cpuFrequency;
	::QueryPerformanceFrequency( &cpuFrequency );
	//  부팅후누적된클럭혹은진동수
	LARGE_INTEGER start;
	::QueryPerformanceCounter( &start );
	LARGE_INTEGER end;
	double Gap;

	DWORD dwStart;
	DWORD dwEnd;
	DWORD dwGap;
	dwStart = GetTickCount();
	*/

	int virtualval = 0;

	g_setSHAFileList.clear();

	if (!(KP2P::GetInstance()->m_spNUserProxy->SendPacket(KNUserEvent::ENU_SHAFILENAME_LIST_REQ, virtualval)))
	{
		g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_LOGINSERVER_CONNECT_FAIL1).c_str(),
			g_pkStrLoader->GetString(STR_ID_LOGINSERVER_CONNECT_FAIL2).c_str());
		return;
	}

	g_MyD3D->WaitForServerAck(Result_SHAFileList, INT_MAX, Wait_Time_LoginServer, TIME_OUT_VALUE);
	Result_SHAFileList = INT_MAX;

	g_kGlobalValue.m_vecSHAFilePathList.clear();
	g_kGlobalValue.m_vecSHAFileSizeList.clear();
	g_kGlobalValue.m_vecSHAFileSHA.clear();

	std::map<std::wstring, std::wstring> bufFilePath;

	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"script.first", L"data/stage/script.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"char_script.first", L"data/stage/char_script.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"ai.first", L"data/stage/ai/ai.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"fan_map.first", L"data/stage/fan_map/fan_map.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"ui.first", L"data/stage/ui/ui.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"playertemplate.first", L"data/stage/playertemplate.first"));

	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"pettemplate.first", L"data/stage/pettemplate.first"));

	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"aipet.first", L"data/stage/aipet.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"minigame.first", L"data/stage/MiniGame/minigame.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"map.first", L"data/stage/map.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"main.exe", L"main.exe"));

	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"chaosland1.first", L"data/resset/chaosland1.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"chaosland2.first", L"data/resset/chaosland2.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"chaosland3.first", L"data/resset/chaosland3.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"chaosland4.first", L"data/resset/chaosland4.first"));

	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"char_amy4.first", L"data/resset/char_amy4.first"));

	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"char_dio1.first", L"data/resset/char_dio1.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"char_dio2.first", L"data/resset/char_dio2.first"));

	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"char_jin2.first", L"data/resset/char_jin2.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"char_jin4.first", L"data/resset/char_jin4.first"));

	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"char_ley1.first", L"data/resset/char_ley1.first"));

	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"char_mari.first", L"data/resset/char_mari.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"char_mari1.first", L"data/resset/char_mari1.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"char_mari2.first", L"data/resset/char_mari2.first"));

	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"char_sieg2.first", L"data/resset/char_sieg2.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"char_sieg3.first", L"data/resset/char_sieg3.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"char_sieg4.first", L"data/resset/char_sieg4.first"));

	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"char_zero1.first", L"data/resset/char_zero1.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"char_zero2.first", L"data/resset/char_zero2.first"));

	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"desert1.first", L"data/resset/desert1.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"desert2.first", L"data/resset/desert2.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"desert3.first", L"data/resset/desert3.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"desert4.first", L"data/resset/desert4.first"));

	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"dungeonzig.first", L"dungeonzig.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"dunrefectoring.first", L"dunrefectoring.first"));

	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"hero1.first", L"data/resset/hero1.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"hero2.first", L"data/resset/hero2.first"));

	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"nightswamp.first", L"data/resset/nightswamp.first"));

	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"renew_arme.first", L"data/resset/renew_arme.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"renew_elesis.first", L"data/resset/renew_elesis.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"renew_lire2.first", L"data/resset/renew_lire2.first"));

	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"rockpaperscissors.first", L"data/resset/rockpaperscissors.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"silverland5.first", L"data/resset/silverland5.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"silverland6.first", L"data/resset/silverland6.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"silverland7.first", L"data/resset/silverland7.first"));

	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"skill_amy1.first", L"data/resset/skill_amy1.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"skill_jin1.first", L"data/resset/skill_jin1.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"skill_ronan1.first", L"data/resset/skill_ronan1.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"skill_ryan0.first", L"data/resset/skill_ryan0.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"skill_sieg1.first", L"data/resset/skill_sieg1.first"));

	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"waterdungeon.first", L"data/resset/waterdungeon.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"xenia5.first", L"data/resset/xenia5.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"xenia6.first", L"data/resset/xenia6.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"xenia7.first", L"data/resset/xenia7.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"xenia8.first", L"data/resset/xenia8.first"));
	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"skilltree.first", L"data/stage/skilltree.first"));

	bufFilePath.insert(std::map<std::wstring, std::wstring>::value_type(L"skilltree.first", L"data/stage/skilltree.first"));

	std::map<std::wstring, std::wstring>::iterator mapIter;
	std::set<std::wstring>::iterator sIter = g_setSHAFileList.begin();

	for (; sIter != g_setSHAFileList.end(); sIter++)
	{
		mapIter = bufFilePath.find((*sIter).c_str());

		if (mapIter == bufFilePath.end()) continue;

		CheckFileSizeAndSHA(kReq_, mapIter->first, mapIter->second);
	}

	SetExtendSHAList(kReq_);

	/*시간 체크를 위한 코드
	dwEnd = GetTickCount();
	dwGap = dwEnd - dwStart;
	//  부팅후누적된클럭혹은진동수
	::QueryPerformanceCounter( &end );
	Gap = ((double)(end.QuadPart - start.QuadPart))/((double)cpuFrequency.QuadPart);
	*/
}

//만약 다른 곳에서도 부를거면...적절한 처리를 하도록...헤더파일을 만든다던지...
void InstallFirewallModulre()
{
	// 방화벽 처리
	char pathbuff[MAX_PATH] = { 0 };
	::GetModuleFileNameA(NULL, pathbuff, MAX_PATH);

	char szTitle[MAX_PATH] = { 0 };
#if defined( NATION_KOREA )
	{
		::strcpy(szTitle, "GrandChase New Era S5");
	}
#else // !NATION_KOREA
	{
		::strcpy(szTitle, "First Chase");
	}
#endif // end NATION_KOREA

	int nFirewallRet = KncFirewall::GuarantedFireWallException(pathbuff, szTitle);

#ifdef NATION_CHINA
	return;
#endif

	switch (nFirewallRet)
	{
	case KncFirewall::KNC_OK:
	case KncFirewall::NOT_WINXP_SP2:        // Windows XP SP2 가 아님.
	{
		break;
	}
	case KncFirewall::NO_EXCEPTION_ALLOWED: // '모든 예외 허용 안함' 모드 동작중 - 게임 실행 불가능
	{
		std::wstring strMsg = g_pkStrLoader->GetString(STR_ID_XP_SP2_NO_EXCEPTION_ALLOWED0) + L'\n' +
			g_pkStrLoader->GetString(STR_ID_XP_SP2_NO_EXCEPTION_ALLOWED1) + L'\n' +
			g_pkStrLoader->GetString(STR_ID_XP_SP2_NO_EXCEPTION_ALLOWED2) + L'\n' +
			g_pkStrLoader->GetString(STR_ID_XP_SP2_NO_EXCEPTION_ALLOWED3) + L'\n' +
			g_pkStrLoader->GetString(STR_ID_XP_SP2_NO_EXCEPTION_ALLOWED4);
		//::MessageBoxW( NULL, strMsg.c_str(), g_pkStrLoader->GetString( STR_ID_GRANDCHASE_TITLE ).c_str(), MB_OK );
		break;
	}
	case KncFirewall::NOT_SYSADMIN:
	{
		std::wstring strMsg = g_pkStrLoader->GetString(STR_ID_XP_SP2_NOT_SYSADMIN0) + L'\n' +
			g_pkStrLoader->GetString(STR_ID_XP_SP2_NOT_SYSADMIN1) + L'\n' +
			g_pkStrLoader->GetString(STR_ID_XP_SP2_NOT_SYSADMIN2) + L'\n' +
			g_pkStrLoader->GetString(STR_ID_XP_SP2_NOT_SYSADMIN3) + L'\n' +
			g_pkStrLoader->GetString(STR_ID_XP_SP2_NOT_SYSADMIN4);
		//::MessageBoxW( NULL, strMsg.c_str(), g_pkStrLoader->GetString( STR_ID_GRANDCHASE_TITLE ).c_str(), MB_OK );
		break;
	}
	} // end switch
}

HRESULT ProcessResultConnectLoginServer(int iResultConnectLoginServer)
{
	// 0 - 성공
	// 1 - Protocol Version이 틀림
	// 2 - 아직 행동이 정의되지 않은 인증방식
	// 3 - 인증 실패
	// 4 - rename 실패
	// 5 - 이중접속 감지
	// 6 - Center Gash 에서 Login 을 얻을수 없다
	// 7 - ID or Password에 이상한 문자들어가있음
	// 8 - 요청한 유저와 반환 유저가 다름_Gash_
	// 9 - Login 길이가 0 이다
	// 10 - 틀린 Password
	// 11 - 존재하지 않는 계정
	// 12 - 신규 유저 등록 실패
	// 13 - 연령 제한
	// 14 -
	// 15

	if (iResultConnectLoginServer == 0)
	{
		return S_OK;
	}
	else
	{
		switch (iResultConnectLoginServer)
		{
		case 1: // 프로토콜 버젼이 다름
		case 15: // 스크립트 crc가 다름
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_CLIENT_VERSION1).c_str(),
				g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_CLIENT_VERSION2).c_str());
			break;
		}
		case 4: // rename 실패
		case 6: // Center Gash 에서 Login 을 얻을수 없다
		case 8: // 요청한 유저와 반환 유저가 다름_Gash
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_GASH_CERTIFY_FAIL1).c_str(),
				g_pkStrLoader->GetString(STR_ID_INQUIRE_TO_CUSTOMER_SUPPORT).c_str());
			break;
		}
		case 5: // 이중접속 감지
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_DOUBLE_CONNECT1).c_str(),
				g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_DOUBLE_CONNECT2).c_str());
			break;
		}
		case 7: // ID or Password에 이상한 문자들어가있음
		case 9: // Login 길이가 0 이다
		case 11: // 존재하지 않는 계정
		case 12: // 신규 유저 등록 실패
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_INVALID_ID_OR_PW1).c_str(),
				g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_INVALID_ID_OR_PW2).c_str());
			break;
		}
#if !defined( BEAN_FUN )
		case 10: // 틀린 Password
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_INVALID_ID_OR_PW1).c_str(),
				g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_INVALID_ID_OR_PW2).c_str());
			break;
		}
#else
		case 10: // 틀린 Password
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_INVALID_ID_OR_PW1).c_str(),
				g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE2).c_str());
			break;
		}
#endif
		case 13: // 연령 제한
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_YOUNGER_BOY_CONNECT1).c_str(),
				g_pkStrLoader->GetString(STR_ID_YOUNGER_BOY_CONNECT2).c_str());
			break;
		}
		case TIME_OUT_VALUE:
		default:
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_CONNECT_TIMEOUT1).c_str(),
				g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_CONNECT_TIMEOUT2).c_str());
			break;
		}
		case 2: // 아직 행동이 정의되지 않은 인증방식
		case 3: // 인증 실패
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_INVALID_PARAM1).c_str(),
				g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_INVALID_PARAM2).c_str());
			return E_FAIL;
		}
		case 16:
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_LOGIN_ERROR_MESSAGE).c_str(), L"");
			return E_FAIL;
		}
		case 18:	//Wrong OTP : Bean Fun
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_BEANFUN_ERROR1).c_str(), L"");
			return E_FAIL;
		}
		case 19:    //빈펀으로 전환한 유저가 빈펀이 아닌 패쳐실행을 통해 접속함
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_BEANFUN_ERROR0).c_str(),
				L"");
			return E_FAIL;
		}
		}
		return E_INVALIDARG; // 인증 실패;
	}
}

// 사내테스트 서버의 파라메터형식일때
// 파라메터를 분석해서 게임서버에 접속한다.
HRESULT CertifyTestServer()
{
	// Test parameter 설명
	//
	// 예시 : Main.exe 218.235.99.238,94001,pch413,pch41301628EDF4D __kogstudios_default_test__
	// 설명 : Main.exe (서버아이피),(서버포트)(성별),(아이디),(아이디)(유니크아이디) __kogstudios_default_test__
	//
	// __kogstudios_default_test__는 테스트 모드임을 명시하는 파라미터이다.

	std::string     strServerIP;        // 서버 아이피
	USHORT          usServerPort;       // 서서 포트
	bool            bFail = false;

	std::string strParam = __argv[1];

	char strTemp[MAX_PATH];
	const char seps[] = ",";
	char* token;

	strcpy(strTemp, __argv[1]);
	token = strtok(strTemp, seps);

	if (token != NULL)
	{
		strServerIP = token;
	}
	else
	{
		// 잘못된 파라메터가 들어왔음

		KMsgBox::Open(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_INVALID_PARAM1).c_str(),
			g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_INVALID_PARAM2).c_str(),
			KMsgBox::MBOX_OK);

		return E_FAIL;
	}

	token = strtok(NULL, seps);
	if (token != NULL)
	{
		int iTemp = atoi(token);
		usServerPort = (USHORT)(iTemp / 10);
		g_kGlobalValue.m_kLoginInfo.bMale = (iTemp % 10 == 1);
	}

	token = strtok(NULL, seps);
	if (token != NULL)
	{
		g_kGlobalValue.m_kLoginInfo.strID = token;
	}

	// 유니크 아이디 - 사용하진 않고 있음
	token = strtok(NULL, seps);

	// NOTE : 테스트 인증이 변경됨.
	// 패스워드가 아이디랑 같아지도록 수정. (2006-04-12)
	// 자세한 사항은 성기군에게 물어보시면 됩니다.
	g_kGlobalValue.m_kLoginInfo.strPass = g_kGlobalValue.m_kLoginInfo.strID;

	g_pBrowser->Init(g_MyD3D->m_hWnd, g_kGlobalValue.m_kLoginInfo.strID.c_str(), NULL, g_kGlobalValue.m_kLoginInfo.strPass.c_str());

	// 방화벽 처리
	InstallFirewallModulre();

	// 전역 서버 정보에 서버 아이피 기록
	g_kGlobalValue.ServerInfo.dwServerIP = ::inet_addr(strServerIP.c_str());
	g_kGlobalValue.ServerInfo.usServerPort = usServerPort;

	if (KP2P::GetInstance()->Init(g_MyD3D->m_hWnd) == false)
	{
		return E_FAIL;
	}

#if defined( NATION_KOREA )
	{
		// 한국이면 아이디는 소문자로 바꿔서 게임 플레이시 사용한다.
		// 왜냐고? 몰라! 원래 그랬으니깐 그냥 쓸 수 밖에... 선대 창시자가 왜 이렇게 했는지는 알 수 없다.
		//char szLoginID[MAX_PATH] = { 0, };
		//::strcpy( szLoginID, g_kGlobalValue.m_kLoginInfo.strID.c_str() );
		//g_kGlobalValue.m_kLoginInfo.strID = ::CharLowerA( szLoginID );
		// 모른다고 이렇게 싸질러 놓으면 쓰나..
		std::transform(g_kGlobalValue.m_kLoginInfo.strID.begin(), g_kGlobalValue.m_kLoginInfo.strID.end(), g_kGlobalValue.m_kLoginInfo.strID.begin(), tolower);
	}
#endif // end NATION_KOREA

	g_kGlobalValue.m_kUserInfo.strLogin = GCUTIL_STR::GCStrCharToWide(g_kGlobalValue.m_kLoginInfo.strID.c_str());
	g_kGlobalValue.m_kUserInfo.strNickName = L"";

	KServerInfo info;
	info.m_strIP = strServerIP;
	info.m_usPort = usServerPort;

	// 사내테스트의 경우 보통 바로 게임서버로 접속한다
	// VERIFY_ACCOUNT_REQ를 날린다.
	Result_Connect_GameServer = INT_MAX;

	KEVENT_VERIFY_ACCOUNT_REQ kReq;
	kReq.m_strLogin = g_kGlobalValue.m_kLoginInfo.strID;
	kReq.m_strPasswd = g_kGlobalValue.m_kLoginInfo.strPass;
	kReq.m_wstrCCode = g_kGlobalValue.m_kUserInfo.strNation;
	kReq.m_bMale = g_kGlobalValue.m_kLoginInfo.bMale;
	kReq.m_iVersion = SERVER_VERSION;
	kReq.m_iP2PVersion = CLIENT_P2P_VER;
	kReq.m_dwChecksum = GetFileCheckSum();
	kReq.m_dwAuthType = g_kGlobalValue.m_dwAuthType;
	kReq.m_dwAuthTick = g_kGlobalValue.m_dwAuthTick;
	kReq.m_bExpAccount = g_kGlobalValue.m_bExpAccount;
	kReq.m_nFunBoxBonus = g_kGlobalValue.m_kLoginInfo.nFunBoxBonus;
	kReq.m_nLinBonus = g_kGlobalValue.m_kLoginInfo.nLinBonus;

	bool bFirstLogin = KP2P::GetInstance()->IsConnectedToGameServer();

	if (KP2P::GetInstance()->ConnectToGameServer(info, kReq, 100000) < 0)
	{
		return E_FAIL;
	}

	g_MyD3D->WaitForServerAck(Result_Connect_GameServer, INT_MAX, 100000, TIME_OUT_VALUE);

	// 0 ( 인증 성공 )
	// 1 ( 잘못된 패스워드 )
	// 2 ( 이중 접속 )
	// 3 ( 길드정보 잘못됨 )
	// 4 ( 프로토콜 버전 다름 )
	// 5 ( 불량 유저 )
	// 7 ( login 크기가 0이거나 인증 실패 )
	// 8 ( 신규 유저 추가 실패 )
	// 9 ( esports 예선 신청자가 아님. )
	// 10 ( 실행파일 checksum이 무효함 )
	// 11 ( 알 수 없는 오류. 대만 인증 실패)
	// 12 ( 존재하지 않는 login )
	// 13 ( 인증 tick Count가 다르다. )
	// 14   대회서버 입장 실패.
	// 15   미국 대회서버 입장 시간이 아님.
	// 16   길드 소속 유저가 아님 (길드전 서버)
	// 17   정규 길드가 아님(길드전 서버 입장불가)
	// 20   최대 동접 초과
	// 21   대회서버 입장권이 없음
	// 22   셧다운제 대상유저

	if (Result_Connect_GameServer != 0) // 인증 실패
	{
		switch (Result_Connect_GameServer)
		{
		case 1: // 존재하지 않는 아이디 또는 패스워드가 틀림
		case 7: // login 크기가 0이거나 인증 실패
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_INVALID_ID_OR_PW1).c_str(),
				g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_INVALID_ID_OR_PW2).c_str());
			break;
		}
		case 2: // 이중접속 시도
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_DOUBLE_CONNECT1).c_str(),
				g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_DOUBLE_CONNECT2).c_str());
			break;
		}
		case 3: // 잘못된 길드
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_GUILD_INFO1).c_str(),
				g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_GUILD_INFO2).c_str());
			break;
		}
		case 4: // 패치가 되지 않았음 ( 클라이언트 체크섬이 디비에 등록되어 있지 않다 )
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_CLIENT_VERSION1).c_str(),
				g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_CLIENT_VERSION2).c_str());
			break;
		}
		case 5: // 계정 중지
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_ACCOUNT_BLOCK1).c_str(),
				g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_ACCOUNT_BLOCK2).c_str());
			break;
		}
		case 9: // 대회서버인데 대회참가자가 아님
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_CONNECT_CONTEST1).c_str(),
				g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_CONNECT_CONTEST2).c_str());
			break;
		}
		case 10: // 실행파일 체크섬이 유효하지 않음.
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_INVALID_FILE1).c_str(),
				g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_INVALID_FILE2).c_str());
			break;
		}
		case 11: // Gash 오류
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_GASH).c_str(),
				g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_NOMATCH_SERVER).c_str());
			break;
		}
		case 13: // 12세 미만의 사용자 제한.
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_YOUNGER_BOY_CONNECT1).c_str(),
				g_pkStrLoader->GetString(STR_ID_YOUNGER_BOY_CONNECT2).c_str());
			break;
		}
		case 15:
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ERR_GAME_START_TIME), L"");
			break;
		}
		case 16: // 길드 소속 유저가 아님 (길드전 서버)
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NOT_GUILD_USER));
			break;
		}
		case 18:	//Wrong OTP : Bean Fun
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_BEANFUN_ERROR1).c_str(), L"");
			return E_FAIL;
		}
		case 19:    //빈펀으로 전환한 유저가 빈펀이 아닌 패쳐실행을 통해 접속함
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_BEANFUN_ERROR0).c_str(),
				L"");
			return E_FAIL;
		}
		case 20:
		{
			break;
		}
		case 22:
		{
			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_SHUTDOWN_MSG_3).c_str(),
				g_pkStrLoader->GetString(STR_ID_SHUTDOWN_MSG_4).c_str());
			break;
		}
		case 8:
		case TIME_OUT_VALUE: // 타임아웃 또는 알 수 없는 에러
		default:
		{
			std::wstring strTemp = g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_CONNECT_TIMEOUT2).c_str();
			strTemp += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii",
				KUserEvent::EVENT_VERIFY_ACCOUNT_ACK, Result_Connect_GameServer);

			g_MyD3D->ExitWithMsgDlg(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_CONNECT_TIMEOUT1).c_str(),
				strTemp.c_str());
			break;
		}
		}
		return E_INVALIDARG; // 인증 실패
	}

	// 060227. jseop. nProtect Send.
	NPGameGuardSend(g_kGlobalValue.m_kUserInfo.strLogin.c_str());

	// 개인정보 동의를 했는가
	if (g_bAgreePrivateInfo == false)
	{
		// 한국에서만 개인 정보 동의 창을 띄워서 확인하도록 한다. 그 외의 국가에서는 스킵한다.
#if defined( NATION_KOREA )
		{
			//ok -> send packet
			//cancel -> shutdown game
			if (KDlgPrivateInfo::DoModal() == IDOK)
			{
				KP2P::GetInstance()->Send_AgreePrivateInfoNot("Server");
			}
			else
			{
				return E_FAIL;
			}
		}
#else // !NATION_KOREA
		{
			KP2P::GetInstance()->Send_AgreePrivateInfoNot("Server");
		}
#endif // end NATION_KOREA
	}

	if (KP2P::GetInstance()->m_kMsgServer != NULL)
	{
		// 만약 메시지 서버에 접속되어있지 않다면 메시지 서버에 접속시도
		if (KP2P::GetInstance()->m_kMsgServer->IsConnectedToMsgServer() == false)
		{
			KP2P::GetInstance()->m_kMsgServer->ConnectToMsgServer();
		}
		else
		{
			g_MyD3D->GetMyShortCutSlot()->Clear();	// 단축슬롯을 클리어
			KP2P::GetInstance()->m_kMsgServer->Send_ChangeGameServer();
		}
	}
	g_kGlobalValue.SendLoadingTime();
	g_kGlobalValue.SendLoginServerConTime();

	// 최초 접속이면 클라이언트 사양 정보 보낸다.
	if (!bFirstLogin)
	{
		KEVENT_STAT_CLIENT_INFO     kStatClientInfo;
		g_MyD3D->GetClientHarwareInfo(kStatClientInfo);
		KP2P::GetInstance()->SendClientHardwareInfo(kStatClientInfo);
	}

	return S_OK;
}

HRESULT TestCertifyExpIDServer() //exp_id - 체험계정 테스트용
{
	return 0L;
}

// 대만 홍콩 본섭 - 첫번째 파라메터가 __kogstudios_original_service__ 인지만 확인하면 된다
// 대만 홍콩은 게임에서 로그인을 하기 때문..
HRESULT CertifyLoginFirst()
{
	if (0 == ::strcmp(__argv[1], MY_MAIN_PARAM))
	{
		// 방화벽 처리
		InstallFirewallModulre();
		g_pBrowser->Init(g_MyD3D->m_hWnd, g_kGlobalValue.m_kLoginInfo.strID.c_str(), NULL, g_kGlobalValue.m_kLoginInfo.strPass.c_str());
		return S_OK;
	}
	else
	{
		KMsgBox::Open(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_INVALID_PARAM1).c_str(),
			g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_INVALID_PARAM2).c_str(),
			KMsgBox::MBOX_OK);
		return E_FAIL;
	}
}

/***************중국의 Parameter처리 과정******************
2006-04-18 프로그래머 권희재

중국 같은 경우 세가 런처가 Parameter를 Client로 넘겨준다.
처음에는 GrandChase.exe가 Parameter를 넘겨 받는데
GrandChase.exe는 패치만 하고 그 받은 Parameter를 그대로
Main.exe로 넘겨준다. Parameter의 형식은 다음과 같다.

-k [KDC address and KDC port] -s [service address and service port] -i [service id]

여기서 service address는 접속할 로그인 서버의 주소이다.
이 Parameter를 이용해서

1.Service Ticket을 발급받는다.(KDC로부터)
2.로그인 서버에 ST를 날려서 검증을 받고, 접속 때 쓰일 ID와 PWD를 받는다.
3.바로 서버리스트에서 접속을 한다.

이런 식으로 접속한다. 여기서 알 수 있듯이 중국에서는 로그인 서버를 쓰지만
로그인 인터페이스를 쓰지 않는다. 세가 런처에서 ID와 PWD를 입력하면 그 이후로는
또 로그인을 할 필요가 없다. 대신 런처가 클라이언트에 암호화된 Ticket을 넘겨준다.
이것이 Parameter이고, 아래 코드는 그 Parameter를 해독하여 접속하는 코드이다.

***********************************************************/
void DecodeParameter(const char* szArgument, char* szAddress, int& iPort)
{
	//다음 Code는 szArg Parameter를 원래 모습으로 변환하는 코드이다.
	char* Addr[4];              //Address를 담을 변수이다.
	char* Port[2];              //Port번호이다.

	char* szArg = new char[strlen(szArgument) + 1];

	strncpy(szArg, szArgument, strlen(szArgument));

	Addr[0] = strtok((char*)szArg, ",");

	for (int i = 1; i < 6; ++i)
	{
		if (i < 4)
		{
			Addr[i] = strtok(NULL, ",");
		}
		else
		{
			Port[i - 4] = strtok(NULL, ",");
		}
	}

	sprintf(szAddress, "%s.%s.%s.%s", Addr[0], Addr[1], Addr[2], Addr[3]);

	//release모드로 하면 이상하게 0이 뒤에 하나 더 붙은 숫자가 나온다..왜 그럴까? 조사해봐야...
	int PortNumber = atoi(Port[1]);
	if (PortNumber > 256)
	{
		PortNumber /= 10;
	}

	iPort = 256 * atoi(Port[0]) + PortNumber;

	SAFE_DELETE_ARRAY(szArg);
	return;
}

HRESULT TestCertifyTaiwan(void)
{
	return S_OK;
}

HRESULT CertifyKoreaNetmarble()
{
	return S_OK;
}

HRESULT TestCertifyKoreaNetmarble()
{
	return 0L;
}

HRESULT StartUpServer(void)
{
	g_kGlobalValue.m_dwAuthType = AT_LEVELUP;
	if (g_kGlobalValue.m_dwAuthType == AT_LEVELUP)
	{
		SAFE_DELETE(g_pBrowser);
		g_pBrowser = new KBrowserWrapper();
	}
	::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	return CertifyLoginFirst();
}