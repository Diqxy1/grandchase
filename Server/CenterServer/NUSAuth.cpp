#include "CenterSimLayer.h"
#include "NUSAuth.h"
#include "NUserPacket.h"
#include <dbg/dbg.hpp>
#include "ThreadManager.h"
#include <KncUtil.h>
#include "CenterDBThread.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG
ImplementDBThread(KNUSAuth);
#define CLASS_TYPE KNUSAuth

KNUSAuth::KNUSAuth(void)
{
	m_kODBC.DriverConnect(ms_wstrFileDSN.c_str()); // 유저용( NtreevUS 웹 인증시 사용 )
}

KNUSAuth::~KNUSAuth(void)
{
}

int KNUSAuth::CheckUSUserLoginInfo(IN const std::string strLogin_,
	IN const std::string strPasswd_,
	IN const int nGameCode_,
	IN const std::wstring wstrIP_,
	OUT bool& bMale_,
	OUT int& nUniqueKey_,
	OUT std::wstring& wstrFailString_)
{
	/*
	{ call dbo.USP_GAME_User_Login ( N'%s', N'%s', %d, N'%s' ) }

	{ call dbo.USP_GAME_User_Login ( @1, @2, @3, @4 ) }

	@uid  ; varchar(12)
	@pwd  ; varchar(32)
	@GAME ; tinyint
	@IP   ; binary(4)
	1 : 트릭스타
	2 : 그랜드체이스
	3 : 팡야

	Return Value의 경우 많은 값을 리턴하지만 기존에 DB에 필요한 값들 이외의 값은 저장하지 않는다.
	Return Value
	varchar(12)     UserID
	varchar(32)     Password(md5)
	varchar(12)     FirstName
	varchar(12)     LastName
	char(1)         Gender
	varchar(50)     EMail
	smalldatetime   Birthday
	int             UniqueKey
	varchar(255)    Custom1
	int             @Result
	*/

	int nOK = -1;
	std::wstring strwLogin = KncUtil::toWideString(strLogin_);
	std::wstring strwPasswd = KncUtil::toWideString(strPasswd_);
	// wstring -> string으로 변환해서 인자에 맞는 형태로 변환.
	std::string strIP = KncUtil::toNarrowString(wstrIP_);
	DWORD dwIP = ::ntohl(::inet_addr(strIP.c_str()));

	// db의 리턴되는 값을 전부 사용하진 않지만 일단 받아와야 하기때문에 해당 값들에 대한 지역변수 선언.
	//int nGameCode           = 2;
	std::string strFirstName = " ";
	std::string strLastName = " ";
	char cGender[2] = " ";
	char cTemp[2] = "M";
	char cTemp1[2] = "F";
	std::string strEmail = " ";
	std::string strLogin = " ";
	std::string strPasswd = " ";
	std::string strTemp1 = "";
	std::string strTemp2 = "";
	std::string strTemp3 = "";

	// NTreev WebDB? ?? birthday ??? ???? ??? SYSTEMTIME ??? ??? ????.
	// time_t ?? ?? ? ??? ?? ???? 1970? 1? 1? 00: 00: 00 ?? ?? 
	// ?? ??? 2038??? ?? ???. 1970? ??? Birthday ??? ?? ?????
	// ?? ??? ???? ??? ?? ????? ??? ? ?? ?? ?? ?? ?? ??.
	SYSTEMTIME sysTime;
	nUniqueKey_ = 0;
	DBJIF3(m_kODBC.Query(L"{ call dbo.USP_GAME_User_Login ( N'%s',N'%s', %d, %u, %d ) }",
		strwLogin.c_str(), strwPasswd.c_str(), nGameCode_, dwIP, 0), return nOK);

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC
			>> strLogin
			>> strPasswd
			>> strFirstName
			>> strLastName
			>> cGender
			>> strEmail
			>> sysTime
			>> nUniqueKey_
			>> strTemp1
			>> strTemp2
			>> strTemp3);

		m_kODBC.EndFetch();

		if (strLogin == " " || nUniqueKey_ == 0) { // 값에 대한 보정 처리.
			nOK = -99;
			wstrFailString_ = KncUtil::toWideString(strTemp1);
		}
		else {
			nOK = 0;
			wstrFailString_ = KncUtil::toWideString(strTemp1);
		}

		// systime thing
		char sysTimeStr[32];
		sprintf_s(sysTimeStr, "%02d-%02d-%d %02d:%02d:%02d", sysTime.wMonth, sysTime.wDay, sysTime.wYear, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

		START_LOG(clog, L"CheckUserLoginInfo")
			<< BUILD_LOG(KncUtil::toWideString(strLogin))
			<< BUILD_LOG(KncUtil::toWideString(strPasswd))
			<< BUILD_LOG(KncUtil::toWideString(strFirstName))
			<< BUILD_LOG(KncUtil::toWideString(strLastName))
			<< BUILD_LOG(cGender)
			<< BUILD_LOG(KncUtil::toWideString(strEmail))
			<< BUILD_LOG(KncUtil::toWideString(sysTimeStr))
			<< BUILD_LOG(nUniqueKey_)
			<< BUILD_LOG(wstrIP_)
			<< BUILD_LOG(dwIP)
			<< END_LOG;

		// Ntreev에서 받아온 cGender값을 가지고 성별 구분을 체크해야 한다.
		// Ntreev Web DB의 Gender구분인 M, F 값을 신뢰하지만 M, F 이외의 값에 
		// 대한 에러 처리에 대해서는 FALSE로 처리하고 로그를 남긴다.
		if (strcmp(cGender, cTemp) == 0) {
			bMale_ = true;
		}
		else if (strcmp(cGender, cTemp1) == 0) {
			bMale_ = false;
		}
		else {
			bMale_ = false;
			START_LOG(cerr, L" NTreev Wrong Gender Data ")
				<< BUILD_LOG(bMale_)
				<< END_LOG;
		}
	}
	else {
		// NtreevUS 웹 인증 실패
		// Ntreev에서 제공한 프로시저에서 자세한 리턴값을 제공하지 않기 때문에
		// User정보를 못가져 오면 클라이언트는 웹 인증 실패 메세지를 띄운다.
		START_LOG(cerr, L" NTreev can't Get User Data ") << END_LOG;
	}

	return nOK;
}

int KNUSAuth::CheckUserLoginInfo(IN const std::string strLogin_,
	IN const std::string strPasswd_,
	IN const int nGameCode_,
	IN const std::wstring wstrIP_,
	OUT bool& bMale_,
	OUT __int64& biUniqueKey_,
	OUT std::wstring& wstrFailString_)
{
	/*
	{ call dbo.USP_GAME_User_Login ( N'%s', N'%s', %d, N'%s' ) }

	{ call dbo.USP_GAME_User_Login ( @1, @2, @3, @4 ) }

	@uid  ; varchar(12)
	@pwd  ; varchar(32)
	@GAME ; tinyint
	@IP   ; binary(4)
	1 : 트릭스타
	2 : 그랜드체이스
	3 : 팡야

	Return Value의 경우 많은 값을 리턴하지만 기존에 DB에 필요한 값들 이외의 값은 저장하지 않는다.
	Return Value
	varchar(12)     UserID
	varchar(32)     Password(md5)
	varchar(120)    FirstName
	varchar(120)    LastName
	char(1)         Gender
	varchar(50)     EMail
	smalldatetime   Birthday
	bigint          UniqueKey
	varchar(255)    Custom1
	int             @Result
	*/

	int nOK = -1;
	std::wstring strwLogin = KncUtil::toWideString(strLogin_);
	std::wstring strwPasswd = KncUtil::toWideString(strPasswd_);
	// wstring -> string으로 변환해서 인자에 맞는 형태로 변환.
	std::string strIP = KncUtil::toNarrowString(wstrIP_);
	DWORD dwIP = ::ntohl(::inet_addr(strIP.c_str()));

	// db의 리턴되는 값을 전부 사용하진 않지만 일단 받아와야 하기때문에 해당 값들에 대한 지역변수 선언.
	//int nGameCode           = 2;
	std::string strFirstName = " ";
	std::string strLastName = " ";
	char cGender[2] = " ";
	char cTemp[2] = "M";
	char cTemp1[2] = "F";
	std::string strEmail = " ";
	std::string strLogin = " ";
	std::string strPasswd = " ";
	std::string strTemp1 = "";
	std::string strTemp2 = "";
	std::string strTemp3 = "";

	// NTreev WebDB에 있는 birthday 정보를 받아오는 부분을 SYSTEMTIME 구조의 변수로 받아온다.
	// time_t 형의 경우 이 함수는 협정 세계시간 1970년 1월 1일 00: 00: 00 시간 이후 
	// 최대 연도는 2038년으로 제한 됩니다. 1970년 이전의 Birthday 정보를 가진 유저때문에
	// 해당 처리를 진행했고 추후에 특정 시간값으로 체크할 때 날짜 값에 대한 예외 처리 필요.
	SYSTEMTIME sysTime;
	biUniqueKey_ = 0;

	DBJIF3(m_kODBC.Query(L"{ call dbo.USP_GAME_User_Login_GCE ( N'%s',N'%s', %d, %u, %d ) }",
		strwLogin.c_str(), strwPasswd.c_str(), nGameCode_, dwIP, 0), return nOK);

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC
			>> strLogin
			>> strPasswd
			>> strFirstName
			>> strLastName
			>> cGender
			>> strEmail
			>> sysTime
			>> biUniqueKey_
			>> strTemp1
			>> strTemp2
			>> strTemp3);

		m_kODBC.EndFetch();

		if (strLogin == " " || biUniqueKey_ == 0) { // 값에 대한 보정 처리.
			nOK = -99;
			wstrFailString_ = KncUtil::toWideString(strFirstName);
		}
		else {
			nOK = 0;
			wstrFailString_ = KncUtil::toWideString(strTemp1);
		}

		// systime thing
		char sysTimeStr[32];
		sprintf_s(sysTimeStr, "%02d-%02d-%d %02d:%02d:%02d", sysTime.wMonth, sysTime.wDay, sysTime.wYear, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

		START_LOG(clog, L"CheckUserLoginInfo")
			<< BUILD_LOG(KncUtil::toWideString(strLogin))
			<< BUILD_LOG(KncUtil::toWideString(strPasswd))
			<< BUILD_LOG(KncUtil::toWideString(strFirstName))
			<< BUILD_LOG(KncUtil::toWideString(strLastName))
			<< BUILD_LOG(cGender)
			<< BUILD_LOG(KncUtil::toWideString(strEmail))
			<< BUILD_LOG/*st*/(KncUtil::toWideString(sysTimeStr))
			<< BUILD_LOG(biUniqueKey_)
			<< BUILD_LOG(nGameCode_)
			<< BUILD_LOG(wstrIP_)
			<< BUILD_LOG(dwIP)
			<< END_LOG;

		// Ntreev에서 받아온 cGender값을 가지고 성별 구분을 체크해야 한다.
		// Ntreev Web DB의 Gender구분인 M, F 값을 신뢰하지만 M, F 이외의 값에 
		// 대한 에러 처리에 대해서는 FALSE로 처리하고 로그를 남긴다.
		if (strcmp(cGender, cTemp) == 0) {
			bMale_ = true;
		}
		else if (strcmp(cGender, cTemp1) == 0) {
			bMale_ = false;
		}
		else {
			bMale_ = false;
			START_LOG(cerr, L" NTreev Wrong Gender Data ")
				<< BUILD_LOG(bMale_) << END_LOG;
		}
	}
	else {
		// NtreevUS 웹 인증 실패
		// Ntreev에서 제공한 프로시저에서 자세한 리턴값을 제공하지 않기 때문에
		// User정보를 못가져 오면 클라이언트는 웹 인증 실패 메세지를 띄운다.
		START_LOG(cerr, L" NTreev can't Get User Data ") << END_LOG;
	}

	return nOK;
}

int KNUSAuth::InsertUserLogoutInfo(IN const std::wstring wstrLogin_,
	IN const int nGameCode_,
	IN const std::wstring wstrIP_,
	IN const __int64 biUniqueKey_,
	IN const int nUniqueKey_)
{
	/*
	{ call dbo.USP_GAME_User_Logout ( N'%s', %d, N'%s', %d ) }

	{ call dbo.USP_GAME_User_Login ( @1, @2, @3, @4 ) }

	@uid      ; varchar(12)
	@GAME     ; tinyint
	@IP       ; binary(4)
	@UniquKey ; int

	Return Value
	int             @Result
	*/

	int nOK = 0;
	// wstring -> string으로 변환해서 인자에 맞는 형태로 변환.
	std::string strIP = KncUtil::toNarrowString(wstrIP_);
	DWORD dwIP = ::ntohl(::inet_addr(strIP.c_str()));

	if (nGameCode_ == KENU_VERIFY_ACCOUNT_REQ::GAMECODE_GC_EU) {
		DBJIF3(m_kODBC.Query(L"{ call dbo.USP_GAME_User_Logout ( N'%s', %d, %u, %I64d, %d ) }",
			wstrLogin_.c_str(), nGameCode_, dwIP, biUniqueKey_, 0), return nOK);
	}
	else {
		DBJIF3(m_kODBC.Query(L"{ call dbo.USP_GAME_User_Logout ( N'%s', %d, %u, %d, %d ) }",
			wstrLogin_.c_str(), nGameCode_, dwIP, nUniqueKey_, 0), return nOK);
	}

	LOG_SUCCESS(nOK == 0)
		<< BUILD_LOG(nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return nOK;
}

void KNUSAuth::ProcessIntEvent(const KIntEvent & kIntEvent_)
{
	KSerializer ks;
	ks.BeginReading(const_cast<KSerBuffer*>(&kIntEvent_.m_kbuff));

	switch (kIntEvent_.m_usEventID)
	{
		INT_CASE(ENU_VERIFY_ACCOUNT_REQ);
		_INT_CASE(ENU_LOG_OUT_NOT, KECN_USER_EXIT_GAME_NOT);
	default:
		START_LOG(cerr, L"이벤트 핸들러가 정의되지 않았음. "
			<< IDVenderType::GetEventIDString(kIntEvent_.m_usEventID)) << END_LOG;
	}
}

INT_IMPL_ON_FUNC(ENU_VERIFY_ACCOUNT_REQ)
{
	KENU_VERIFY_ACCOUNT_ACK kPacket;
	int nOK = -99;

	if (kPacket_.m_nSGIGamecode == KENU_VERIFY_ACCOUNT_REQ::GAMECODE_GC_EU) {
		nOK = CheckUserLoginInfo(kPacket_.m_strLogin,
			kPacket_.m_strPasswd,
			kPacket_.m_nSGIGamecode,
			kPacket_.m_strIP,
			kPacket_.m_bMale,
			kPacket_.m_biUniqueKey,
			kPacket.m_wstrFailString);
	}
	else if (kPacket_.m_nSGIGamecode == KENU_VERIFY_ACCOUNT_REQ::GAMECODE_GC_US) {
		nOK = CheckUSUserLoginInfo(kPacket_.m_strLogin,
			kPacket_.m_strPasswd,
			kPacket_.m_nSGIGamecode,
			kPacket_.m_strIP,
			kPacket_.m_bMale,
			kPacket_.m_nUniqueKey,
			kPacket.m_wstrFailString);
	}

	if (nOK == 0) {
		kPacket.m_nOK = NetError::NET_OK;
	}
	else {
		kPacket.m_nOK = NetError::ERR_NUSER_VERIFY_20;
	}

	if (nOK == 0) {
		SiKCenterDBMgr()->QueueingEvent(KCenterEvent::DB_ENU_VERIFY_ACCOUNT_REQ, strLogin_.c_str(), 0, 0, kPacket_);
	}
	else {
		SiKCenterSimLayer()->m_kActorManager2.QueueingEventTo(strLogin_, KNUserEvent::ENU_VERIFY_ACCOUNT_ACK, kPacket);
	}

	// NtreevUS Web에서 인증처리 성공/실패여부에 대한 로그 출력
	_LOG_SUCCESS(nOK == 0, L"NUS Web 로그인 인증 처리 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(nOK)
		<< BUILD_LOG(KncUtil::toWideString(kPacket_.m_strLogin))
		<< BUILD_LOG(KncUtil::toWideString(kPacket_.m_strPasswd))
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< BUILD_LOG(kPacket_.m_nSGIGamecode)
		<< BUILD_LOG(kPacket_.m_strIP)
		<< END_LOG;
}

_INT_IMPL_ON_FUNC(ENU_LOG_OUT_NOT, KECN_USER_EXIT_GAME_NOT)
{
	int nOK = InsertUserLogoutInfo(kPacket_.m_wstrLogin,
		kPacket_.m_nGameCode,
		kPacket_.m_wstrIP,
		kPacket_.m_biUniqueKey,
		kPacket_.m_nUniqueKey);

	// 유저 로그아웃 기록에 대한 부분 성공 실패 처리.
	_LOG_SUCCESS(nOK == 0, L"NUS Web 로그인 인증 처리 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(nOK)
		<< BUILD_LOG(kPacket_.m_wstrLogin)
		<< BUILD_LOG(kPacket_.m_wstrIP)
		<< BUILD_LOG(kPacket_.m_nGameCode)
		<< BUILD_LOG(kPacket_.m_biUniqueKey)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
}