#include "CommonSP.h"
#include "odbc/Odbc.h"
#include <dbg/dbg.hpp>
#include "Log4.h"

KCommonSP::KCommonSP(void)
	: m_kODBC(KODBC())
{
}

KCommonSP::~KCommonSP(void)
{
}

int KCommonSP::InsertToWaitItemJob(IN const DWORD dwUID_, IN const char cCharType_, IN const GCITEMID itemID_,
	IN const int nCount_, IN const int nPeriod_, IN const int nGetType_, OUT __int64& waitUID_, IN int nGrade_/*=-1*/, IN const int nItemLevel_/*=-1*/, IN const int nEnchantLevel_/*=0*/, IN const int nStatus_/*2*/)
{
	/*
	구매외 아이템 획득
	 L"{ call dbo.WIGAWaitItem_insert_20130402 ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }"
	{ call dbo.WIGAWaitItem_insert_20130402 ( @01, @02, @03, @04, @05, @06, @07, @08, @09, @10 ) }
	@01: @01iLoginUID_input      int
	@02: @02iCharType_input      int
	@03: @03iItemID_input        int
	@04: @04iGradeID_input       smallint
	@05: @05iPeriod_input        int
	@06: @06iDuration_input      smallint
	@07: @07iGetType_input       tinyint
	@08: @08iStatus_input        tinyint
	@09: @09iItemLevel_input     smallint
	@10: @10iStrengthLevel_input int

	1 return ( @1 )
	@1: OK      int
		0       : 성공
		-1      : 유저 정보가 없음
		-2      : 아이템ID가 음수로 들어옴
		-3      : 기간이 500일을 초과함
		-4      : 수량이 2012개를 초과함
		-101이하: 트랜젝션 에러
	@2: WIGAUID bigint
	*/

	int nOK = -99;
	DBJIF3(m_kODBC.Query(L"{ call dbo.WIGAWaitItem_insert_20130402 ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }",
		dwUID_, cCharType_, itemID_, nGrade_, nPeriod_, nCount_, nGetType_, nStatus_, nItemLevel_, nEnchantLevel_), return nOK);

	if (m_kODBC.BeginFetch() == true) {
		VERIFY_QUERY(m_kODBC
			>> nOK
			>> waitUID_);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nOK == 0, L"대기 아이템으로 추가. OK : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< BUILD_LOG(waitUID_) << END_LOG;
	return nOK;
}

void KCommonSP::GetSALT(IN const std::wstring& strLogin_, OUT std::wstring& strSALT_)
{
	/*
	L"{ call dbo.USGAUserSalt_select ( %s ) }"

	{ call dbo.USGAUserSalt_select ( @1 ) }
	@1 ; @iLogin_

	1 return ( @1 )
	@1  ; Salt [nvarchar](40)
	*/

	DBJIF(m_kODBC.Query(L"{ call dbo.USGAUserSalt_select ( N'%s' ) }", strLogin_.c_str()));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> strSALT_);
		m_kODBC.EndFetch();
	}

	START_LOG(clog, L"SALT : " << strSALT_)
		<< BUILD_LOG(strLogin_)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
}

bool KCommonSP::UpdateChannelCCU(IN const int nType_, IN const int nChannelType_, IN const int nServerID_, IN const int nCurUserNum_)
{
	/*
	L"{ call dbo.MSGAMinuteStat_merge_20101101 ( %d, %d, %d, %d ) }"

	{ call dbo.MSGAMinuteStat_merge_20101101 ( @1, @2, @3, @4 ) }
	@1 ; @iType_         tinyint
	1: 분당 동접
	2: 분당 Login수
	3: 분당 Logout수
	4: 분당 Migrationin수
	5: 분당 Migrationout수
	6: 분당 Zombie수
	@2 ; @iChannelingID_ tinyint
	0 ; 전체
	1 ; 채널링국가1
	2 ; 채널링국가2
	차후 국가가 늘어날시 숫자를 증가시켜주면 됨
	국가별 구분이 필요없는 경우 0으로 줌(단, 덮쳐쓰므로 주의)
	@3 ; @iServerID_     int
	서버의 고유한 ID값
	@4 ; @iData_         int

	1 select ( @1 )
	@1  ; OK int
	0        ; 성공
	-1       ; ServerID가 유효하지 않음
	-101이하 ; 트랜잭션 에러
	*/

	DBJIF2(m_kODBC.Query(L"{ call dbo.MSGAMinuteStat_merge_20101101 ( %d, %d, %d, %d ) }",
		nType_, nChannelType_, nServerID_, nCurUserNum_));

	if (m_kODBC.BeginFetch()) {
		int nOK = -99;
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();
		if (nOK != 0) {
			START_LOG(cerr, L"서버 채널링 동접 갱신 실패 .. OK : " << nOK << L" Query : " << m_kODBC.m_strLastQuery) << END_LOG;
		}
	}

	return true;
}

void KCommonSP::Merge_AccOnlineTime(IN const DWORD dwLoginUID_, IN const int nTime_)
{
	/*
	L"{ call dbo.IPGAImmersionPrevention_merge_20121128 ( %d, %d ) }"

	{ call dbo.IPGAImmersionPrevention_merge_20121128 ( @1, @2 ) }
	@1: @iLoginUID_input             int
	@2: @iOnlineCumulationTime_input int

	1 return ( @1 )
	@1: OK int
		0       : 성공
		-101이하: 트랜잭션 에러
	*/
	DBJIF(m_kODBC.Query(L"{ call dbo.IPGAImmersionPrevention_merge_20121128 ( %d, %d ) }", dwLoginUID_, nTime_));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();
	}

	START_LOG(clog, L" : ")
		<< BUILD_LOG(dwLoginUID_)
		<< BUILD_LOG(nTime_)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
}

void KCommonSP::Select_AccOnlineTime(IN const DWORD dwLoginUID_, OUT int& nTime_)
{
	/*
	L"{ call dbo.IPGAImmersionPrevention_select_20121128 ( %d ) }"

	{ call dbo.IPGAImmersionPrevention_select_20121128 ( @1 ) }
	@1: @iLoginUID_input int

	1 select ( @1 )
	@1: OnlineCumulationTime int
	*/

	DBJIF(m_kODBC.Query(L"{ call dbo.IPGAImmersionPrevention_select_20121128 ( %d ) }", dwLoginUID_));

	nTime_ = 0;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nTime_);
		m_kODBC.EndFetch();
	}

	START_LOG(clog, L" : ")
		<< BUILD_LOG(dwLoginUID_)
		<< BUILD_LOG(nTime_)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
}

int KCommonSP::GetTodayEventCount(IN const DWORD dwUID_, IN const int nEventType_, IN const time_t tmDate_)
{
	//{ call dbo.SDGA_server_data_select_integer_user ( @1, @2, @3 ) }
	//@1 ; @iLoginUID_ [int]
	//@2 ; @sdtRegDateD_ [smalldatetime]  ; 일단위까지만 인식함
	//@3 ; @iType_ [tinyint]              ; 서버에서 원하는 숫자별로 분류됨
	//
	//1 return ( @1 )
	//@1 ; Data int

	int nRes = 0;
	DBJIF3(m_kODBC.Query(L"{ call dbo.SDGA_server_data_select_integer_user (  %d, N'%s', %d ) }",
		dwUID_, (LPCTSTR)CTime(tmDate_).Format(L"%Y%m%d"), nEventType_), return -1);

	if (m_kODBC.BeginFetch())
	{
		VERIFY_QUERY(m_kODBC >> nRes);
	}
	m_kODBC.EndFetch();

	START_LOG(clog, m_kODBC.m_strLastQuery)
		<< BUILD_LOG(nRes) << END_LOG;
	return nRes;
}

bool KCommonSP::SetTodayEventDoneDiff(IN const DWORD dwUID_, IN const int nEventType_, IN const time_t tmDate_, IN const int nData_)
{
	/*
		L"{ call dbo.SDGAServerDataIntegerUser_merge_diff ( %d, N'%s', %d, %d ) }"

		{ call dbo.SDGAServerDataIntegerUser_merge_diff ( @1, @2, @3, @4 ) }
		@1 ; @iLoginUID_   int
		@2 ; @sdtRegDateD_ smalldatetime ; 일단위까지만 인식함
		@3 ; @iType_       tinyint       ; 서버에서 원하는 숫자별로 분류됨
		@4 ; @iDataDiff_   int           ; 차이값을 주쇼

		1 return ( @1 )
		@1 ; OK int
		0        ; 성공
		-1       ; 유저 정보가 없음
		-101이하 ; 트랜잭션 에러
		*/

	int nOK = -99;
	DBJIF2(m_kODBC.Query(L"{ call dbo.SDGAServerDataIntegerUser_merge_diff (  %d, N'%s', %d, %d ) }",
		dwUID_, (LPCTSTR)CTime(tmDate_).Format(L"%Y%m%d"), nEventType_, nData_));

	if (m_kODBC.BeginFetch())
	{
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	_LOG_SUCCESS(nOK == 0, L"일일 이벤트??? 셋팅 할때 오류.., UID : " << dwUID_)
		<< BUILD_LOG(nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return (nOK == 0);
}

bool KCommonSP::SetTodayEventDone(IN const DWORD dwUID_, IN const int nEventType_, IN const time_t tmDate_, IN const int nData_)
{
	/*
	L"{ call dbo.SDGAServerDataIntegerUser_merge ( %d, N'%s', %d, %d ) }"

	{ call dbo.SDGAServerDataIntegerUser_merge ( @1, @2, @3, @4 ) }
	@1 ; @iLoginUID_   int
	@2 ; @sdtRegDateD_ smalldatetime
	@3 ; @iType_       tinyint
	@4 ; @iData_       int

	1 return ( @1 )
	@1 ; OK int
	0        ; 성공
	-1       ; 유저 정보가 없음
	-101이하 ; 트랜잭션 에러
	*/

	int nOK = -99;
	DBJIF2(m_kODBC.Query(L"{ call dbo.SDGAServerDataIntegerUser_merge (  %d, N'%s', %d, %d ) }",
		dwUID_, (LPCTSTR)CTime(tmDate_).Format(L"%Y%m%d"), nEventType_, nData_));

	if (m_kODBC.BeginFetch())
	{
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	_LOG_SUCCESS(nOK == 0, L"일일 이벤트??? 셋팅 할때 오류.., UID : " << dwUID_)
		<< BUILD_LOG(nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return (nOK == 0);
}

bool KCommonSP::UpdateSellStateFlag_SocialCommerce(IN const DWORD& dwEventUID_, IN int& nSuccessFlag_)
{
	/*
		발렌타인 보부상 이벤트 상태정보 변경

		L"{ call dbo.SCGASocialCommerceEventInfo_SuccessFlag_update ( %d, %d ) }"

		{ call dbo.SCGASocialCommerceEventInfo_SuccessFlag_update ( @1, @2) }
		@1: @iEventUID_    int
		@2: @iSuccessFlag_ int  -- 목표 달성 성공 여부 ( -1 : 실패, 0 : 진행중, 1 : 성공, 2 : 보상 지급까지 완료 )

		1 return ( @1 )
		@1: OK       int
			0       : 성공
			-1      : 해당 이벤트가 존재하지 않음
			-101이하: 트랜젝션 에러
	*/

	DBJIF2(m_kODBC.Query(L"{ call dbo.SCGASocialCommerceEventInfo_SuccessFlag_update ( %d, %d ) }", dwEventUID_, nSuccessFlag_));

	int nOK = -99;
	if (m_kODBC.BeginFetch())
	{
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	_LOG_SUCCESS(nOK == 0, L"[소셜커머스] 공동구매 이벤트 진행 상태 변경 nOK : " << nOK)
		<< BUILD_LOG(dwEventUID_)
		<< BUILD_LOG(nSuccessFlag_)
		<< END_LOG;

	return (nOK == 0);
}

void KCommonSP::GetGroupPurchaseFlag_SocialCommerce(IN const DWORD& dwEventUID_, OUT int& nSuccessFlag_)
{
	/*
		발렌타인 보부상 이벤트 상태정보 변경

		L"{ call dbo.SCGASocialCommerceEventInfo_SuccessFlag_update ( %d, %d ) }"

		{ call dbo.SCGASocialCommerceEventInfo_SuccessFlag_update ( @1, @2) }
		@1: @iEventUID_    int
		@2: @iSuccessFlag_ int  -- 목표 달성 성공 여부 ( -1 : 실패, 0 : 진행중, 1 : 성공, 2 : 보상 지급까지 완료 )

		1 return ( @1 )
		@1: OK       int
			0       : 성공
			-1      : 해당 이벤트가 존재하지 않음
			-101이하: 트랜젝션 에러
	*/

	DBJIF(m_kODBC.Query(L"{ call dbo.SCGASocialCommerceEventInfo_SuccessFlag_update ( %d, %d ) }", dwEventUID_));

	if (m_kODBC.BeginFetch())
	{
		VERIFY_QUERY(m_kODBC >> nSuccessFlag_);
	}
	m_kODBC.EndFetch();
}

bool KCommonSP::User_auth_select(IN const DWORD& dwUserUID_, OUT char& cLastGrade_, OUT char& cCurrentGrade_, OUT int& nPeriodType_, OUT time_t& tmLimit_, OUT char& cCause_)
{
	/*
	L"{ call dbo.UAG_user_auth_select_20130705 ( %d ) }"

	{ call dbo.UAG_user_auth_select_20130705 ( @01 ) }
	@01iLoginUID_input int

	1 select ( @01, @02, @03, @04, @05 )
	@01iAuthLevelA  int
	@02iAuthLevelB  int
	@03iStatus      int
	@04iIsUnlimited int
		0: 기간 존재함
		1: 무제한
	@05dtRegDateA   datetime
		@04가 0인 경우에는 데이터가 유효
	*/

	cLastGrade_ = 0;
	cCurrentGrade_ = 0;
	nPeriodType_ = 0;
	cCause_ = 0;

	DBJIF2(m_kODBC.Query(L"{ call dbo.UAG_user_auth_select_20130705 ( %d ) }",
		dwUserUID_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> cLastGrade_
			>> cCurrentGrade_
			>> cCause_
			>> nPeriodType_
			>> tmLimit_);
		m_kODBC.EndFetch();
	}

	START_LOG(clog, L"유저 등급레벨.. UserUID : " << dwUserUID_)
		<< BUILD_LOGc(cLastGrade_)
		<< BUILD_LOGc(cCurrentGrade_)
		<< BUILD_LOG(nPeriodType_)
		<< BUILD_LOGtm(CTime(tmLimit_))
		<< BUILD_LOGc(cCause_) << END_LOG;

	return true;
}

bool KCommonSP::IgnorableErrorProcessingAfterCallSP(IN OUT int& nReturnValue_)
{
	// -801~-899: 무시가능 에러
	if (-899 <= nReturnValue_ && -801 >= nReturnValue_) {
		nReturnValue_ = 0;

		return false;
	}

	return true;
}