#include <winsock2.h>
#include "AGDBThread.h"
#include "NetError.h"
#include "AgentServer.h"
#include "AGSimLayer.h"
#include "AGDBLayer.h"
#include "AgitManager.h"
#include "../GameServer/EventType.h"
#include "Log4.h"

#define CLASS_TYPE KAGDBThread

void KAGDBThread::SetServerListFromDB(IN const int& nProtocol_)
{
	std::vector<KServerInfo> vecServerList;
	LIF(GetServerList(nProtocol_, vecServerList));
	SiKAgentServer()->SetServerList(vecServerList);
}

bool KAGDBThread::GetServerList(IN const int& nProtocol_, OUT std::vector<KServerInfo>& vecServerList_)
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

	DBJIF2(m_kODBC.Query(L"{ call dbo.CUGAConcurrentUser_select_All_by_ProtocolVersion ( %d ) }",
		nProtocol_));

	while (m_kODBC.Fetch() != SQL_NO_DATA)
	{
		KServerInfo kServerInfo;
		kServerInfo.m_nUserProtocolVer = nProtocol_;

		VERIFY_QUERY(m_kODBC >> kServerInfo.m_iServerPart
			>> kServerInfo.m_strName
			>> kServerInfo.m_strIP
			>> kServerInfo.m_usPort
			>> kServerInfo.m_iUserNum
			>> kServerInfo.m_iMaxUserNum
			>> kServerInfo.m_dwUID
			>> kServerInfo.m_strConnIP);

		vecServerList_.push_back(kServerInfo);
	}
	m_kODBC.EndFetch();

	return true;
}

bool KAGDBThread::UpdateCountItem(IN const DWORD dwUID_, IN const GCITEMUID ItemUID, IN const int nDellFactor_)
{
	//수량아이템 개수 변경 획득 및 사용, 삭제
	/*
	L"{ call dbo.UIGAUserItemDuration_merge_20131028 ( %d, %I64d, %d ) }"

	{ call dbo.UIGAUserItemDuration_merge_20131028 ( @01, @02, @03 ) }
		@01iLoginUID_input      int
	,   @02iItemUID_input       bigint
	,   @03iDuration_diff_input int

	1 return ( @01, @02 )
		@01iOK       int
			0        : 성공
			-1       : 유저 정보 없음
			-2       : 메인 아이템 정보 없음
			-801~-899: 무시가능 에러
			-901~-999: 치명적인 에러
			-1000    : 사용 중지된 sp인데 사용하려 함
		@02iDuration int
	*/
	//!!주의 : nDellFactor_ 가 음수이면 수량 아이템을 소모합니다. 양수이면 수량 증가합니다.
	int nRet = -99;
	int nCount = 0;
	DBJIF2(m_kODBC.Query(L"{ call dbo.UIGAUserItemDuration_merge_20131028 ( %d, %I64d, %d ) }",
		dwUID_, ItemUID, nDellFactor_));

	if (m_kODBC.BeginFetch())
	{
		VERIFY_QUERY(m_kODBC >> nRet >> nCount);
		m_kODBC.EndFetch();
	}

	bool bSuccess = (nRet == 0) || (-899 <= nRet && nRet <= -801);
	_LOG_SUCCESS(bSuccess, L"Count Item Update, OK : " << nRet << L", count : " << nCount << L", Query : " << m_kODBC.m_strLastQuery) << END_LOG;

	return bSuccess;
}

bool KAGDBThread::UseCountItem(IN const DWORD dwUID_, IN const GCITEMUID ItemUID_, IN const int nDellFactor_)
{
	return UpdateCountItem(dwUID_, ItemUID_, -nDellFactor_);
}

bool KAGDBThread::AddCountItem(IN const DWORD dwUID_, IN const GCITEMUID ItemUID_, IN const int nDellFactor_)
{
	return UpdateCountItem(dwUID_, ItemUID_, nDellFactor_);
}

bool KAGDBThread::AgitInfo_agittype_update(IN const DWORD& dwUserUID_, IN const DWORD& dwMapID_)
{
	/*
	mk8253. 20110415.
	아지트의 타입을 변경.

	{ call dbo.ASGAAgitInfo_agittype_update ( @1, @2 ) }
	@1 ; @iLoginUID_ [int]
	@2 ; @iAgitType_ [tinyint]

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 아지트 정보가 없거나 변경하려는 아지트 타입이 작음
	-101이하 ; 트랜젝션 에러
	*/
	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitInfo_agittype_update ( %d, %d ) }",
		dwUserUID_, dwMapID_));

	int nRet = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nRet == 0, L"아지트의 타입을 변경.. Result : " << nRet)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return (nRet == 0);
}

bool KAGDBThread::AgitInfo_visitor_update(IN const DWORD& dwUserUID_, IN const int& nDiffCount_)
{
	/*
	mk8253. 20110418.
	방문자 수 갱신.

	{ call dbo.ASGAAgitInfo_visitor_update ( @1, @2 ) }
	@1 ; @iLoginUID_    [int]
	@2 ; @iVisitorDiff_ [int]

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 아지트 정보가 없음
	-101이하 ; 트랜젝션 에러
	*/
	if (nDiffCount_ <= 0) return true;

	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitInfo_visitor_update ( %d, %d ) }",
		dwUserUID_, nDiffCount_));

	int nRet = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nRet == 0, L"방문자 수 갱신.. Result : " << nRet)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return (nRet == 0);
}

bool KAGDBThread::AgitInfo_select(IN const DWORD& dwUserUID_, IN OUT KAgitInfo& kInfo_)
{
	/*
	mk8253. 20110418.
	아지트 정보 리턴.

	{ call dbo. ASGAAgitInfo_select( @1 ) }
	@1 ; @iLoginUID_ [int]

	1 return ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10 )
	@1  ; AgitType       tinyint
	@2  ; AgitName       nvarchar(20)
	@3  ; EnterUserCount tinyint
	@4  ; OpenType       tinyint
	@5  ; passwd         nvarchar(20)
	@6  ; Invitation     tinyint
	@7  ; Introduce      nvarchar(100)
	@8  ; TodayVisitor   int
	@9  ; TotalVisitor   int
	@10 ; LastCleaning   smalldatetime
	*/
	kInfo_.m_dwUID = dwUserUID_;

	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitInfo_select ( %d ) }",
		dwUserUID_));

	if (m_kODBC.BeginFetch()) {
		kInfo_.m_dwUID = dwUserUID_;

		VERIFY_QUERY(m_kODBC >> kInfo_.m_dwMapID
			>> kInfo_.m_strName
			>> kInfo_.m_ucMaxUser
			>> kInfo_.m_ucOpenType
			>> kInfo_.m_strPassword
			>> kInfo_.m_ucInviteType
			>> kInfo_.m_kProfile.m_strProfile
			>> kInfo_.m_nInitTodayVisiter
			>> kInfo_.m_nTotalVisiter
			>> kInfo_.m_tmLastCleaning);

		kInfo_.m_nInitTodayVisiter = std::max<int>(kInfo_.m_nInitTodayVisiter, 0);
		kInfo_.m_nTodayVisiter = kInfo_.m_nInitTodayVisiter;
		m_kODBC.EndFetch();
	}

	START_LOG(clog, L"DB 아지트 정보 받아옴.")
		<< BUILD_LOGc(kInfo_.m_dwUID)
		<< BUILD_LOGc(kInfo_.m_dwMapID)
		<< BUILD_LOG(kInfo_.m_strName)
		<< BUILD_LOGc(kInfo_.m_ucMaxUser)
		<< BUILD_LOGc(kInfo_.m_ucOpenType)
		<< BUILD_LOG(kInfo_.m_strPassword)
		<< BUILD_LOGc(kInfo_.m_ucInviteType)
		<< BUILD_LOG(kInfo_.m_kProfile.m_strProfile)
		<< BUILD_LOG(kInfo_.m_nInitTodayVisiter)
		<< BUILD_LOG(kInfo_.m_nTodayVisiter)
		<< BUILD_LOG(kInfo_.m_nTotalVisiter)
		<< BUILD_LOGtm(CTime(kInfo_.m_tmLastCleaning)) << END_LOG;

	return true;
}

bool KAGDBThread::AgiteObjPos_merge(IN const DWORD& dwUserUID_, IN const GCITEMUID& ItemUID_, IN const int& nPosX_, IN const int& nPosY_)
{
	/*
	mk8253. 20110419.
	오브젝트 배치.

	{ call dbo.ASGAAgitObjPos_merge ( @1, @2, @3, @4 ) }
	@1 ; @iLoginUID_ [int]
	@2 ; @iItemUID_  [bigint]
	@4 ; @iPosX_     [smallint]
	@5 ; @iPosY_     [smallint]

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 아이템 정보가 없음
	-101이하 ; 트랜젝션 에러
	*/
	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitObjPos_merge ( %d, %I64d, %d, %d ) }",
		dwUserUID_, ItemUID_, nPosX_, nPosY_));

	int nRet = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nRet == 0, L"오브젝트 배치.. Result : " << nRet)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return (nRet == 0);
}

bool KAGDBThread::AgiteObjPos_select(IN const DWORD& dwUserUID_, OUT std::vector<KAgitObjectPos>& vecObjectPos_)
{
	/*
	mk8253. 20110419.
	오브젝트 위치 리턴.

	{ call dbo.ASGAAgitObjPos_select ( @1 ) }
	@1 ; @iLoginUID_     [int]

	n return ( @1, @2, @3, @4 )
	@1 ; ItemUID bigint
	@2 ; ItemID  int
	@3 ; PosX    smallint
	@4 ; PosY    smallint
	*/
	std::set<GCITEMUID> setUID; // 중복체크용
	setUID.clear();
	vecObjectPos_.clear();

	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitObjPos_select ( %d ) }",
		dwUserUID_));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		KAgitObjectPos kItem;

		VERIFY_QUERY(m_kODBC >> kItem.m_UID
			>> kItem.m_ID
			>> kItem.m_nPosX
			>> kItem.m_nPosY);

		// 좌표 보정
		kItem.m_nPosX = std::max<int>(kItem.m_nPosX, -1);
		kItem.m_nPosY = std::max<int>(kItem.m_nPosY, -1);

		if (false == setUID.insert(kItem.m_UID).second) {
			START_LOG(cwarn, L"중복된 오브젝트 정보. AgitUID : " << dwUserUID_)
				<< BUILD_LOG(kItem.m_UID)
				<< BUILD_LOG(kItem.m_ID)
				<< BUILD_LOG(kItem.m_nPosX)
				<< BUILD_LOG(kItem.m_nPosY) << END_LOG;
			continue;
		}

		vecObjectPos_.push_back(kItem);
	}

	START_LOG(clog, L"오브젝트 위치 리턴. size : " << vecObjectPos_.size()) << END_LOG;
	return true;
}

bool KAGDBThread::AgitInfo_lastcleaning_update(IN const DWORD& dwUserUID_)
{
	/*
	mk8253. 20110419.
	아지트의 마지막 청소시간을 변경.

	{ call dbo.ASGAAgitInfo_lastcleaning_update ( @1 ) }
	@1 ; @iLoginUID_   [int]

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 아지트 정보가 없음
	-101이하 ; 트랜젝션 에러
	*/
	DBJIF2(m_kODBC.Query(L"{ call ASGAAgitInfo_lastcleaning_update ( %d ) }",
		dwUserUID_));

	int nRet = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nRet == 0, L"아지트의 마지막 청소시간을 변경.. Result : " << nRet)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return (nRet == 0);
}

bool KAGDBThread::AgitInfo_option_update(IN const DWORD& dwUserUID_, IN const KAgitOption& kInfo_)
{
	/*
	mk8253. 20110419.
	아지트의 옵션 설정을 변경.

	{ call dbo.ASGAAgitInfo_option_update ( @1, @2, @3, @4, @5, @6, @7 ) }
	@1 ; @iLoginUID_       [int]
	@2 ; @strAgitName_     [nvarchar](20)
	@3 ; @iEnterUserCount_ [tinyint]
	@4 ; @iOpenType_       [tinyint]
	@5 ; @strPasswd_       [nvarchar](10)
	@6 ; @iInvitation_     [tinyint]
	@7 ; @strIntroduce_    [nvarchar](100)

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 아지트 정보가 없음
	-101이하 ; 트랜젝션 에러
	*/
	// 게임서버 user8.cpp EVENT_SET_AGIT_OPTION_REQ 처리 부분에서 ' 를 ''로 변경하게 수정. 2012. 05. 14 lucidust
	//ReplaceAgitOptionInfo( kInfo_ );

	DBJIF2(m_kODBC.Query(L"{ call ASGAAgitInfo_option_update ( %d, N'%s', %d, %d, N'%s', %d, N'%s' ) }",
		dwUserUID_,
		kInfo_.m_strName.c_str(),
		static_cast<int>(kInfo_.m_ucMaxUser),
		static_cast<int>(kInfo_.m_ucOpenType),
		kInfo_.m_strPassword.c_str(),
		static_cast<int>(kInfo_.m_ucInviteType),
		kInfo_.m_strProfile.c_str()));

	int nRet = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nRet == 0, L"아지트의 옵션 설정을 변경.. Result : " << nRet)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
	return (nRet == 0);
}

bool KAGDBThread::AgiteFlowerpot_init(IN const DWORD& dwUserUID_, IN const GCITEMUID& ItemUID_)
{
	/*
	mk8253. 20110419.
	화분을 처음 획득할 경우와 민들레를 회수할 경우 초기화

	{ call dbo.ASGAAgitFlowerpot_init ( @1, @2 ) }
	@1 ; @iLoginUID_ [int]
	@2 ; @iPotUID_   [bigint]

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 화분 정보가 없음
	-101이하 ; 트랜젝션 에러
	*/
	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitFlowerpot_init ( %d, %I64d ) }",
		dwUserUID_, ItemUID_));

	int nRet = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nRet == 0, L"화분을 처음 획득할 경우와 민들레를 회수할 경우 초기화.. Result : " << nRet)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return (nRet == 0);
}

bool KAGDBThread::AgiteFlowerpot_seed(IN const DWORD& dwUserUID_, IN const GCITEMUID& ItemUID_, IN const int& nSeedType_, OUT time_t& tmTime_)
{
	/*
	mk8253. 20110419.
	씨앗 심기.

	{ call dbo.ASGAAgitFlowerpot_seed ( @1, @2, @3, @4 ) }
	@1 ; @iLoginUID_  [int]
	@2 ; @iPotUID_     [bigint]
	@3 ; @iPlantType_ [int]
	@4 ; @sdtSeedTime_ [smalldatetime]

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 화분 정보가 없음
	-101이하 ; 트랜젝션 에러
	*/
	tmTime_ = KncUtil::TimeToInt(CTime::GetCurrentTime());
	std::wstring strDate = (LPCTSTR)CTime(tmTime_).Format(_T("%Y%m%d %H:%M"));

	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitFlowerpot_seed ( %d, %I64d, %d, N'%s' ) }",
		dwUserUID_, ItemUID_, nSeedType_, strDate.c_str()));

	int nRet = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nRet == 0, L"씨앗 심기.. Result : " << nRet)
		<< BUILD_LOG(strDate)
		<< BUILD_LOGtm(CTime(tmTime_))
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return (nRet == 0);
}

bool KAGDBThread::AgiteFlowerpot_select(IN const DWORD& dwUserUID_, OUT std::map<GCITEMUID, KFlowerPot>& mapList_)
{
	/*
	mk8253. 20110421.
	화분 정보.

	{ call dbo.ASGAAgitFlowerpot_select ( @1 ) }
	@1 ; @iLoginUID_     [int]

	n return ( @1, @2, @3, @4, @5 )
	@1 ; PotUID    bigint
	@2 ; ItemID    int
	@3 ; PlantType smallint
	@5 ; RegDate   smalldatetime
	*/
	mapList_.clear();

	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitFlowerpot_select ( %d ) }",
		dwUserUID_));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		KFlowerPot kPot;
		kPot.m_nSeedType = -1;
		kPot.m_usFlowerCount = 0;

		VERIFY_QUERY(m_kODBC >> kPot.m_UID
			>> kPot.m_ItemID
			>> kPot.m_nSeedType
			>> kPot.m_tmSeedTime);

		// 꽃이 안심어져있는 상태가 아니면 꽃 상태 체크
		if (kPot.m_nSeedType != -1) {
			KFlowerPotOption kOption;
			if (false == SiKAgitManager()->GetFlowerPotOption(kPot.m_nSeedType, kOption)) {
				START_LOG(cwarn, L"정의되지않은 SeedType : " << kPot.m_nSeedType << L", UserUID : " << dwUserUID_)
					<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
				continue;
			}
			// 꽃피는 시간
			CTime tmFlowerTime = CTime(kPot.m_tmSeedTime) + CTimeSpan(0, 0, kOption.m_nGrowTime, 0);
			kPot.m_tmFlowerTime = KncUtil::TimeToInt(tmFlowerTime);
			// 꽃피는 개수
			kPot.m_usFlowerCount = kOption.m_usFlowerCount;
		}

		LIF(mapList_.insert(std::map<GCITEMUID, KFlowerPot>::value_type(kPot.m_UID, kPot)).second);
	}

	START_LOG(clog, L"화분 정보. size : " << mapList_.size()) << END_LOG;
	return true;
}

bool KAGDBThread::AgitTownAuction_init(IN const DWORD& dwUserUID_, IN const std::wstring& strLogin_)
{
	/*
	mk8253. 20110427.
	월드맵 아지트 초기화

	{ call dbo.ASGAAgitTownAuction_init ( @1 ) }
	@1 ; @iLoginUID_     [int]
	@2 ; @strBuyerLogin_ [nvarchar](24)

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 유저 정보가 없음
	-101이하 ; 트랜젝션 에러
	*/
	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitTownAuction_init ( %d, N'%s' ) }",
		dwUserUID_, strLogin_.c_str()));

	int nRet = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nRet == 0, L"월드맵 아지트 초기화.. Result : " << nRet)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
	return (nRet == 0);
}

bool KAGDBThread::AgitTown_select(OUT std::map<USHORT, std::map<USHORT, DWORD> >& mapList_)
{
	/*
	mk8253. 20110427.
	등록된 월드맵 아지트 보기

	{ call dbo.ASGAAgitTown_select }

	n return ( @1, @2, @3 )
	@1 ; TownID
	@2 ; PosID
	@3 ; LoginUID
	*/
	mapList_.clear();

	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitTown_select }"));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		USHORT usWorldID = 0;
		USHORT usPosID = 0;
		DWORD dwAgitUID = 0;

		VERIFY_QUERY(m_kODBC >> usWorldID
			>> usPosID
			>> dwAgitUID);

		if (usWorldID == 0) continue;
		if (usPosID == 0) continue;
		if (dwAgitUID == 0) continue;

		LIF(mapList_[usWorldID].insert(std::map<USHORT, DWORD>::value_type(usPosID, dwAgitUID)).second);
	}

	START_LOG(clog, L"등록된 월드맵 아지트 보기. size : " << mapList_.size()) << END_LOG;
	return true;
}

bool KAGDBThread::AgitTown_update(IN const USHORT& usTownID_, IN const USHORT& usPosID_, IN const DWORD& dwAgitUID_)
{
	/*
	mk8253. 20110427.
	월드맵 아지트 등록

	{ call dbo.ASGAAgitTown_update ( @1, @2, @3 ) }
	@1 ; @iTownID_ [tinyint]
	@2 ; @iPosID_       [tinyint]
	@3 ; @iLoginUID_    [int]

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 아지트 정보가 없음
	-101이하 ; 트랜젝션 에러
	*/
	DBJIF2(m_kODBC.Query(L"{ call ASGAAgitTown_update ( %d, %d, %d ) }",
		usTownID_, usPosID_, dwAgitUID_));

	int nRet = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nRet == 0, L"월드맵 아지트 등록.. Result : " << nRet)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
	return (nRet == 0);
}

bool KAGDBThread::AgitTownAuction_select(OUT std::map< USHORT, std::map<USHORT, KAuctionInfo> >& mapList_)
{
	/*
	mk8253. 20110502.
	월드맵 아지트 경매현황

	{ call dbo.ASGAAgitTownAuction_select }

	n return ( @1, @2, @3, @4, @5 )
	@1 ; TownID
	@2 ; PosID
	@3 ; LoginUID
	@4 ; BID
	@5 ; RegDate
	*/
	mapList_.clear();

	DBJIF2(m_kODBC.Query(L"{ call ASGAAgitTownAuction_select }"));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		KAuctionInfo kInfo;
		USHORT usTownID = 0;
		USHORT usPosID = 0;

		VERIFY_QUERY(m_kODBC >> usTownID
			>> usPosID
			>> kInfo.m_dwUserUID
			>> kInfo.m_nPayPoint
			>> kInfo.m_tmDate);

		mapList_[usTownID][usPosID] = kInfo;
	}

	START_LOG(clog, L"월드맵 아지트 경매현황. size : " << mapList_.size()) << END_LOG;
	return true;
}

bool KAGDBThread::AgitTownAuction_merge(IN const USHORT& usTownID_, IN const USHORT& usPosID_, IN const DWORD& dwUserUID_, IN const int& nPayPoint_)
{
	/*
	mk8253. 20110502.
	경매 입찰자 변경

	{ call dbo.ASGAAgitTownAuction_merge ( @1, @2, @3, @4 ) }
	@1 ; @iTownID_   [tinyint]
	@2 ; @iPosID_    [tinyint]
	@3 ; @iLoginUID_ [int]
	@4 ; @iBID_      [int]

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 유저 정보가 없음
	-101이하 ; 트랜젝션 에러
	*/
	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitTownAuction_merge ( %d, %d, %d, %d ) }",
		usTownID_, usPosID_, dwUserUID_, nPayPoint_));

	int nRet = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nRet == 0, L"경매 입찰자 변경.. Result : " << nRet)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
	return (nRet == 0);
}

bool KAGDBThread::GetAgitInfoFromDB(IN const DWORD& dwUserUID_, OUT KAgitInfo& kInfo_, OUT std::wstring& strNick_)
{
	_JIF(AgitInfo_select(dwUserUID_, kInfo_), return false);
	_JIF(AgiteObjPos_select(dwUserUID_, kInfo_.m_vecObjectPos), return false);
	_JIF(AgiteCharInfo_select(dwUserUID_, kInfo_.m_mapCharacterPos), return false);
	_JIF(AgiteCharCoordi_select(dwUserUID_, kInfo_.m_mapCharacterPos), return false);
	LIF(AgitCharDesignCoordi_select(dwUserUID_, kInfo_.m_mapCharacterPos));
	LIF(LoginUIDtoNickname_select(dwUserUID_, strNick_));
	return true;
}

bool KAGDBThread::AgitFavorites_insert(IN const DWORD& dwUserUID_, IN const DWORD& dwAgitUID_)
{
	/*
	mk8253. 20110418.즐겨찾기 정보 추가.

	{ call dbo.ASGAAgitFavorites_insert( @1, @2 ) }
	@1 ; @iLoginUID_  [int]
	@1 ; @iLoginUIDE_ [int]

	1 return ( @1 )
	@1 ; OK int
	-1       ; 유저 정보가 없음
	-101이하 ; 트랜젝션 에러
	*/

	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitFavorites_insert ( %d, %d ) }",
		dwUserUID_, dwAgitUID_));

	int nRet = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nRet == 0, L"즐겨찾기 정보 추가.. Result : " << nRet)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
	return (nRet == 0);
}

bool KAGDBThread::AgitFavorites_delete(IN const DWORD& dwUserUID_, IN const DWORD& dwAgitUID_)
{
	/*
	mk8253. 20110418.즐겨찾기 정보 삭제.

	{ call dbo.ASGAAgitFavorites_delete( @1, @2 ) }
	@1 ; @iLoginUID_  [int]
	@1 ; @iLoginUIDE_ [int]

	1 return ( @1 )
	@1 ; OK int
	-1       ; 유저 정보가 없음
	-101이하 ; 트랜젝션 에러
	*/

	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitFavorites_delete ( %d, %d ) }",
		dwUserUID_, dwAgitUID_));

	int nRet = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nRet == 0, L"즐겨찾기 정보 삭제.. Result : " << nRet)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
	return (nRet == 0);
}

bool KAGDBThread::AgitFavorites_select(IN const DWORD& dwUserUID_, OUT std::map<DWORD, KFavoritesAgit>& mapList_)
{
	/*
	mk8253. 20110418.즐겨찾기 정보 리턴.

	{ call dbo.ASGAAgitFavorites_select( @1 ) }
	@1 ; @iLoginUID_ [int]

	n return ( @1, @2, @3, @4  )
	@1  ; LoginUIDE      int
	@2  ; Nickname       nvarchar(20)
	@3  ; AgitName       nvarchar(20)
	@4  ; OpenType       tinyint
	*/

	mapList_.clear();

	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitFavorites_select ( %d ) }",
		dwUserUID_));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		KFavoritesAgit kInfo;

		VERIFY_QUERY(m_kODBC >> kInfo.m_dwAgitUID
			>> kInfo.m_strNickName
			>> kInfo.m_strAgitName
			>> kInfo.m_ucOpenType);

		mapList_[kInfo.m_dwAgitUID] = kInfo;
	}

	START_LOG(clog, L"즐겨찾기 정보 리턴. size : " << mapList_.size()) << END_LOG;
	return true;
}

bool KAGDBThread::AgitRank_select(OUT std::map<int, std::vector<KRankAgit> >& mapList_)
{
	/*
	아지트 랭킹
	{ call dbo. ASGAAgitRank_select }

	10 return ( @1, @2, @3, @4 )
	@1  ; Type     int
	Type 0 전일 Today 인기 아지트
	Type 1 전일 Total  인기 아지트
	@2  ; Rank     int
	@3  ; LoginUID int
	@4  ; Visitor  int
	*/
	mapList_.clear();

	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitRank_select }"));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		KRankAgit kInfo;

		VERIFY_QUERY(m_kODBC >> kInfo.m_nType
			>> kInfo.m_nRank
			>> kInfo.m_dwAgitUID
			>> kInfo.m_dwVisitorCount);

		mapList_[kInfo.m_nType].push_back(kInfo);
	}

	START_LOG(clog, L"아지트 랭크 정보 리턴.. size : " << mapList_.size()) << END_LOG;
	return true;
}

bool KAGDBThread::GetFavoritesInfoFromDB(IN const DWORD& dwUserUID_, OUT std::map<DWORD, KAgitPageInfo>& mapPage_)
{
	KAgitInfo kInfo;
	std::map<DWORD, KFavoritesAgit> mapList;
	_JIF(AgitFavorites_select(dwUserUID_, mapList), return false);

	std::map<DWORD, KFavoritesAgit>::iterator mit;
	for (mit = mapList.begin(); mit != mapList.end(); ++mit) {
		// 없는 아지트 정보는 DB에서 받아오기
		if (false == AgitInfo_select(mit->first, kInfo)) {
			continue;
		}

		KAgitPageInfo kPage;
		kPage.m_dwUID = mit->second.m_dwAgitUID;
		kPage.m_strName = mit->second.m_strAgitName;
		kPage.m_strOwnerNick = mit->second.m_strNickName;
		kPage.m_ucMaxUser = kInfo.m_ucMaxUser;
		kPage.m_ucOpenType = mit->second.m_ucOpenType;

		mapPage_[kPage.m_dwUID] = kPage;
	}

	START_LOG(clog, L"아지트 즐겨찾기 리스트 등록.. UserUID : " << dwUserUID_)
		<< BUILD_LOG(mapList.size())
		<< BUILD_LOG(mapPage_.size()) << END_LOG;
	return true;
}

bool KAGDBThread::AgitHelper_merge(IN const DWORD& dwUserUID_, IN const KBrownieInfo& kInfo_)
{
	/*
	mk8253. 20110419.아지트 도우미 갱신.

	{ call dbo.ASGAAgitHelper_merge ( @1, @2, @3, @4 ) }
	@1 ; @iLoginUID_   [int]
	@2 ; @iHelperType_ [tinyint]
	@3 ; @iItemUID_    [bigint]
	@4 ; @bUsed_       [bit]

	1 return ( @1 )
	@1 ; OK int
	-101이하 : 트랜잭션 에러
	*/
	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitHelper_merge ( %d, %d, %I64d, %d ) }",
		dwUserUID_, kInfo_.m_dwUID, kInfo_.m_EquipItemUID, kInfo_.m_bIsUsed));

	int nRet = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nRet == 0, L"아지트 도우미 갱신.. Result : " << nRet)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
	return (nRet == 0);
}

bool KAGDBThread::AgitHelper_select(IN const DWORD& dwUserUID_, OUT std::map<GCITEMUID, KBrownieInfo>& mapList_)
{
	/*
	mk8253. 20110419.아지트 도우미 정보 리턴.

	{ call dbo.ASGAAgitHelper_select ( @1 ) }
	@1 ; @iLoginUID_ [int]

	n return ( @1, @2, @3, @4 )
	@1 ; HelperType   int
	@2 ; ItemUID      bigint
	@3 ; ItemID       int
	@4 ; IsUsed       bit
	*/
	mapList_.clear();

	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitHelper_select ( %d ) }",
		dwUserUID_));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		KBrownieInfo kInfo;

		VERIFY_QUERY(m_kODBC >> kInfo.m_dwUID
			>> kInfo.m_EquipItemUID
			>> kInfo.m_EquipItemID
			>> kInfo.m_bIsUsed);

		mapList_[kInfo.m_dwUID] = kInfo;
	}

	START_LOG(clog, L"아지트 도우미 정보 리턴.. size : " << mapList_.size()) << END_LOG;
	return true;
}

bool KAGDBThread::GetFairyTreeInfoFromDB(IN const DWORD& dwUserUID_, OUT KFairyTree& kInfo_)
{
	_JIF(AgitFairyTree_select(dwUserUID_, kInfo_), return false);
	_JIF(AgitFairyTreeFruit_select(dwUserUID_, kInfo_.m_tmUpdateFruit, kInfo_.m_nFruitType), return false);

	CTime tmUpdateFruit = CTime(kInfo_.m_tmUpdateFruit);
	CTime tmCurrent = CTime::GetCurrentTime();
	time_t tmCurrentTime = KncUtil::TimeToInt(tmCurrent);

	// 요정의나무 성장치 감소 체크
	if (SiKAgitManager()->FairyTreeGrowChange(tmCurrentTime, kInfo_)) {
		AgitFairyTree_merge(dwUserUID_, kInfo_);
	}

	// 날짜 지났으면, 열매 새로 설정
	if (tmUpdateFruit.GetYear() < tmCurrent.GetYear() ||
		(tmUpdateFruit.GetYear() == tmCurrent.GetYear() && tmUpdateFruit.GetMonth() < tmCurrent.GetMonth()) ||
		(tmUpdateFruit.GetYear() == tmCurrent.GetYear() && tmUpdateFruit.GetMonth() == tmCurrent.GetMonth() && tmUpdateFruit.GetDay() < tmCurrent.GetDay())) {
		kInfo_.m_nFruitType = SiKAgitManager()->GetTodayFruitType(kInfo_.m_nLv);
		kInfo_.m_tmUpdateFruit = tmCurrentTime;
		LIF(AgitFairyTreeFruit_merge(dwUserUID_, kInfo_.m_nFruitType, kInfo_.m_tmUpdateFruit));
	}

	int nRet = GetTodayUseFruit(dwUserUID_);
	kInfo_.m_bUseFruit = (nRet > 0 ? true : false);

	// 열매 사용안했을때 열매가 설정안된 경우라면 열매 한번더 확인
	if (false == kInfo_.m_bUseFruit) {
		kInfo_.m_nFruitType = SiKAgitManager()->GetTodayFruitType(kInfo_.m_nLv);
		kInfo_.m_tmUpdateFruit = tmCurrentTime;
		LIF(AgitFairyTreeFruit_merge(dwUserUID_, kInfo_.m_nFruitType, kInfo_.m_tmUpdateFruit));
	}
	return true;
}

bool KAGDBThread::AgitFairyTree_select(IN const DWORD& dwUserUID_, OUT KFairyTree& kInfo_)
{
	/*
	20110419. mk8253. 요정의 나무 정보 리턴.

	{ call dbo.ASGAAgitFairyTree_select ( @1 ) }
	@1 ; @iLoginUID_ [int]

	1 return ( @1, @2, @3 )
	@1 ; TreeLevel      tinyint
	@2 ; TreeExp        smallint
	@3 ; LastGrowthTime smalldatetime
	*/
	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitFairyTree_select ( %d ) }",
		dwUserUID_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> kInfo_.m_nLv
			>> kInfo_.m_dwExp
			>> kInfo_.m_tmLastGrowTime
			>> kInfo_.m_tmLastDecreaseTime);
		m_kODBC.EndFetch();
	}

	// 최소값 보정
	time_t tmDefault = KncUtil::TimeToInt(CTime(2000, 1, 1, 0, 0, 0, 0));
	if (kInfo_.m_tmLastGrowTime < tmDefault) {
		kInfo_.m_tmLastGrowTime = tmDefault;
	}

	// 최대값 보정
	time_t tmToday = KncUtil::TimeToInt(CTime::GetCurrentTime());
	if (kInfo_.m_tmLastGrowTime > tmToday) {
		kInfo_.m_tmLastGrowTime = tmToday;
	}

	if (kInfo_.m_tmLastDecreaseTime < tmDefault) {
		kInfo_.m_tmLastDecreaseTime = tmDefault;
	}

	START_LOG(clog, L"요정의 나무 정보 리턴.. LoginUID : " << dwUserUID_)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< BUILD_LOG(kInfo_.m_nLv)
		<< BUILD_LOG(kInfo_.m_dwExp)
		<< BUILD_LOGtm(CTime(kInfo_.m_tmLastGrowTime)) << END_LOG;

	return true;
}

bool KAGDBThread::AgitFairyTree_merge(IN const DWORD& dwUserUID_, IN const KFairyTree& kInfo_)
{
	/*
	20110419. mk8253. 요정의 나무 갱신.

	{ call dbo.ASGAAgitFairyTree_merge ( @1, @2, @3, @4 ) }
	@1 ; @iLoginUID_       [int]
	@2 ; @iTreeLevel_      [tinyint]
	@3 ; @iTreeExp_        [int]
	@4 ; @sdtLastGrowTime_ [smalldatetime]
	@5 ; @sdtReducedTime_  [smalldatetime]

	1 return ( @1 )
	@1 ; OK int
	-1       ; 유저 정보가 없음
	-101이하 ; 트랜젝션 에러
	*/
	time_t tmTimeDate = kInfo_.m_tmLastGrowTime;
	time_t tmTimeDate2 = kInfo_.m_tmLastDecreaseTime;
	time_t tmToday = KncUtil::TimeToInt(CTime::GetCurrentTime());

	// 물준 시간 체크
	if (kInfo_.m_tmLastGrowTime > tmToday) {
		tmTimeDate = tmToday;
		START_LOG(cerr, L"마지막으로 물준 시간이 현재시간보다 미래 시간이다.. UserUID : " << dwUserUID_)
			<< BUILD_LOGtm(CTime(tmToday))
			<< BUILD_LOGtm(CTime(kInfo_.m_tmLastGrowTime)) << END_LOG;
	}

	// 성장 감소 시간 체크
	if (kInfo_.m_tmLastDecreaseTime > tmToday) {
		tmTimeDate2 = tmToday;
		START_LOG(cerr, L"감소 시간이 현재시간보다 미래 시간이다.. UserUID : " << dwUserUID_)
			<< BUILD_LOGtm(CTime(tmToday))
			<< BUILD_LOGtm(CTime(kInfo_.m_tmLastDecreaseTime)) << END_LOG;
	}

	std::wstring strDate = (LPCTSTR)CTime(tmTimeDate).Format(_T("%Y%m%d %H:%M"));
	std::wstring strDate2 = (LPCTSTR)CTime(tmTimeDate2).Format(_T("%Y%m%d %H:%M"));

	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitFairyTree_merge ( %d, %d, %d, N'%s', N'%s' ) }",
		dwUserUID_, kInfo_.m_nLv, kInfo_.m_dwExp, strDate.c_str(), strDate2.c_str()));

	int nRet = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nRet == 0, L"요정의 나무 정보 갱신.. Result : " << nRet)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
	return (nRet == 0);
}

bool KAGDBThread::AgitFairyTreeFruit_merge(IN const DWORD& dwUserUID_, IN const int& nFruitType_, IN const time_t& tmUpdate_)
{
	/*
	20110419. mk8253. 요정의 나무 열매 갱신.

	{ call dbo.ASGAAgitFairyTreeFruit_merge ( @1, @2 , @3) }
	@1 ; @iLoginUID_  [int]
	@2 ; @sdtToday_   [smalldatetime]
	@3 ; @iFruitType_ [int]

	1 return ( @1 )
	@1 ; OK int
	-1       ; 유저 정보가 없음
	-101이하 ; 트랜젝션 에러
	*/
	std::wstring strDate = (LPCTSTR)CTime(tmUpdate_).Format(_T("%Y%m%d %H:%M"));

	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitFairyTreeFruit_merge ( %d, N'%s', %d ) }",
		dwUserUID_, strDate.c_str(), nFruitType_));

	int nRet = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nRet == 0, L"요정의 나무 열매 갱신.. Result : " << nRet)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
	return (nRet == 0);
}

bool KAGDBThread::AgitFairyTreeFruit_select(IN const DWORD& dwUserUID_, OUT time_t& tmDate_, OUT int& nFruitType_)
{
	/*
	20110419. mk8253. 요정의 나무 열매정보 리턴.

	{ call dbo.ASGAAgitFairyTreeFruit_select ( @1 ) }
	@1 ; @iLoginUID_ [int]

	1 return ( @1, @2 )
	@1 ; Today     smalldatetime
	@2 ; FruitType tinyint
	*/
	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitFairyTreeFruit_select ( %d ) }",
		dwUserUID_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> tmDate_
			>> nFruitType_);
		m_kODBC.EndFetch();
	}

	START_LOG(clog, L"요정의 나무 열매정보 리턴.. LoginUID : " << dwUserUID_)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< BUILD_LOGtm(CTime(tmDate_))
		<< BUILD_LOG(nFruitType_) << END_LOG;
	return true;
}

int KAGDBThread::GetTodayUseFruit(IN const DWORD& dwUserUID_)
{
	//{ call dbo.SDGA_server_data_select_integer_user ( @1, @2, @3 ) }
	//@1 ; @iLoginUID_ [int]
	//@2 ; @sdtRegDateD_ [smalldatetime]  ; 일단위까지만 인식함
	//@3 ; @iType_ [tinyint]              ; 서버에서 원하는 숫자별로 분류됨
	//
	//1 return ( @1 )
	//@1 ; Data int

	DBJIF3(m_kODBC.Query(L"{ call dbo.SDGA_server_data_select_integer_user (  %d, N'%s', %d ) }",
		dwUserUID_, (LPCTSTR)CTime::GetCurrentTime().Format(L"%Y%m%d"), UDT_TODAY_USE_FRUIT), return -1);

	int nRet = 0;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
	}
	m_kODBC.EndFetch();

	START_LOG(clog, L"일일 요정의나무 과일 사용여부 확인.. LoginUID : " << dwUserUID_)
		<< BUILD_LOG(nRet)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return nRet;
}

bool KAGDBThread::SetTodayUseFruit(IN const DWORD& dwUserUID_)
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

	DBJIF2(m_kODBC.Query(L"{ call dbo.SDGAServerDataIntegerUser_merge_diff (  %d, N'%s', %d, 1 ) }",
		dwUserUID_, (LPCTSTR)CTime::GetCurrentTime().Format(L"%Y%m%d"), UDT_TODAY_USE_FRUIT));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	_LOG_SUCCESS(nOK == 0, L"일일 요정의나무 과일 사용 DB기록. Ret : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return (nOK == 0);
}

bool KAGDBThread::AgitSystemCollect_insert(IN const DWORD& dwAgitUID_, IN const DWORD& dwEnterUserUID_, IN const DWORD& dwTimeSec_)
{
	/*
	아지트 통계(statDB)

	{ call dbo.ASCOAgitSystemCollect_insert ( @1, @2, @3 ) }
	@1 ; @iLoginUID_  int
	@2 ; @iLoginUIDE_ int
	@3 ; @iDurationTime_ int (초)

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-101이하 ; 트랜젝션 에러
	*/

	DBJIF2(m_kODBCStat.Query(L"{ call ASCOAgitSystemCollect_insert ( %d, %d, %d ) }",
		dwAgitUID_, dwEnterUserUID_, dwTimeSec_));

	int nOK = -99;
	if (m_kODBCStat.BeginFetch()) {
		VERIFY_QUERY(m_kODBCStat >> nOK);
	}
	m_kODBCStat.EndFetch();

	_LOG_SUCCESS(nOK == 0, L"아지트 방문통계 DB기록. Ret : " << nOK)
		<< BUILD_LOG(m_kODBCStat.m_strLastQuery) << END_LOG;

	return (nOK == 0);
}

bool KAGDBThread::AgitCharInfo_insert(IN const DWORD& dwAgitUID_, IN const int& nCharType_)
{
	/*
	mk8253. 20110504. 캐릭터정보 최초 획득.

	{ call dbo.ASGAAgitCharInfo_insert ( @1, @2 ) }
	@1 ; @iLoginUID_ [int]
	@2 ; @iCharType_ [tinyint]

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 유저 정보가 없음
	-2       ; 캐릭터 정보가 이미 있음
	-101이하 ; 트랜젝션 에러
	*/
	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitCharInfo_insert ( %d, %d ) }",
		dwAgitUID_, nCharType_));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	_LOG_SUCCESS(nOK == 0, L"캐릭터정보 최초 획득.. Ret : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return (nOK == 0);
}

bool KAGDBThread::AgitCharInfo_pos_update(IN const DWORD& dwAgitUID_, IN const int& nCharType_, IN const int& nPosX_, IN const int& nPosY_)
{
	/*
	mk8253. 20110426.아지트 캐릭터 배치.

	{ call dbo.ASGAAgitCharInfo_pos_update ( @1, @2, @3, @4 ) }
	@1 ; @iLoginUID_ [int]
	@2 ; @iCharType_ [tinyint]
	@3 ; @iPosX_     [int]
	@4 ; @iPosY_     [int]

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 아지트 캐릭터 정보가 없음
	-101이하 ; 트랜젝션 에러
	*/
	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitCharInfo_pos_update ( %d, %d, %d, %d ) }",
		dwAgitUID_, nCharType_, nPosX_, nPosY_));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	_LOG_SUCCESS(nOK == 0, L"아지트 캐릭터 배치.. Ret : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return (nOK == 0);
}

bool KAGDBThread::AgitCharInfo_dialogic_update(IN const DWORD& dwAgitUID_, IN const int& nCharType_, IN const std::wstring& strNormalDialogic_, IN const std::wstring& strNearDialogic_, IN const std::wstring& strClickDialogic_)
{
	/*
	mk8253. 20110426.캐릭터 대사 변경.

	{ call dbo.ASGAAgitCharInfo_dialogic_update ( @1, @2, @3, @4, @5 ) }
	@1 ; @iLoginUID_         [int]
	@2 ; @iCharType_         [tinyint]
	@3 ; @strNormalDialogic_ [nvarchar](50)
	@4 ; @strNearDialogic_   [nvarchar](50)
	@5 ; @strClickDialogic_  [nvarchar](50)

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 아이템 정보가 없음
	-101이하 ; 트랜젝션 에러
	*/
	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitCharInfo_dialogic_update ( %d, %d, N'%s', N'%s', N'%s' ) }",
		dwAgitUID_, nCharType_, strNormalDialogic_.c_str(), strNearDialogic_.c_str(), strClickDialogic_.c_str()));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	_LOG_SUCCESS(nOK == 0, L"캐릭터 대사 변경.. Ret : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return (nOK == 0);
}

bool KAGDBThread::ASGAAgitCharInfo_motion_update(IN const DWORD& dwAgitUID_, IN const int& nCharType_, IN const int& nMotionID_)
{
	/*
	mk8253. 20110419.캐릭터 모션 변경.

	{ call dbo.ASGAAgitCharInfo_motion_update ( @1, @2, @3 ) }
	@1 ; @iLoginUID_ [int]
	@2 ; @iCharType_ [tinyint]
	@3 ; @iMotionID_ [int]

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 아이템 정보가 없음
	-101이하 ; 트랜젝션 에러
	*/
	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitCharInfo_motion_update ( %d, %d, %d ) }",
		dwAgitUID_, nCharType_, nMotionID_));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	_LOG_SUCCESS(nOK == 0, L"캐릭터 모션 변경.. Ret : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return (nOK == 0);
}

bool KAGDBThread::AgiteCharInfo_select(IN const DWORD& dwAgitUID_, OUT std::map<int, KAgitCharacterPos>& mapList_)
{
	/*
	mk8253. 20110419.캐릭터 정보 리턴.

	{ call dbo.ASGAAgitCharInfo_select ( @1 ) }
	@1 ; @iLoginUID_ [int]

	1 return ( @1, @2, @3, @4, @5, @6, @7 )
	@1 ; CharType tinyint
	@1 ; MotionID int
	@3 ; PosX smallint
	@4 ; PosY smallint
	@5 ; NormalDialogic nvarchar(50)
	@6 ; NearDialogic nvarchar(50)
	@7 ; ClickDialogic nvarchar(50)
	*/
	mapList_.clear();

	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitCharInfo_select ( %d ) }",
		dwAgitUID_));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		KAgitCharacterPos kInfo;
		std::wstring strNormalDialogic;
		std::wstring strNearDialogic;
		std::wstring strClickDialogic;

		VERIFY_QUERY(m_kODBC >> kInfo.m_nCharType
			>> kInfo.m_nMotionID
			>> kInfo.m_nPosX
			>> kInfo.m_nPosY
			>> strNormalDialogic
			>> strNearDialogic
			>> strClickDialogic);

		kInfo.m_mapSpeech[KAgitCharacterPos::SPT_DEFAULT] = strNormalDialogic;
		kInfo.m_mapSpeech[KAgitCharacterPos::SPT_CLOSE] = strNearDialogic;
		kInfo.m_mapSpeech[KAgitCharacterPos::SPT_CLICK] = strClickDialogic;

		mapList_[kInfo.m_nCharType] = kInfo;
	}

	START_LOG(clog, L"캐릭터 정보 리턴.. size : " << mapList_.size()) << END_LOG;
	return true;
}

int KAGDBThread::AgiteCharCoordi_merge(IN const DWORD& dwAgitUID_, IN const int& nCharType_, IN const DWORD& dwSlotID_, IN const GCITEMUID& ItemUID_, IN const bool& bEquip_)
{
	/*
	mk8253. 20110419.캐릭터 코디 갱신.

	{ call dbo.ASGAAgitCharCoordi_merge ( @1, @2, @3, @4, @5 ) }
	@1 ; @iLoginUID_ [int]
	@2 ; @iCharType_ [tinyint]
	@3 ; @iSlotID_   [int]
	@4 ; @iItemUID_  [bigint]
	@5 ; @bIsEquip_  [bit]
		0 ; 장착해제
		1 ; 장착

	1 return ( @1 )
	@1 ; OK int
	- 1 : 보유하지않은 아이템을 장착해제. DB에서 정보 다시 읽어올것
	-101이하 : 트랜잭션 에러
	*/
	DBJIF3(m_kODBC.Query(L"{ call dbo.ASGAAgitCharCoordi_merge ( %d, %d, %d, %I64d, %d ) }",
		dwAgitUID_, nCharType_, dwSlotID_, ItemUID_, bEquip_), return -99);

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	_LOG_SUCCESS(nOK == 0 || nOK == -1, L"캐릭터 코디 갱신.. Ret : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return nOK;
}

bool KAGDBThread::AgiteCharCoordi_select(IN const DWORD& dwAgitUID_, IN OUT std::map<int, KAgitCharacterPos>& mapList_)
{
	/*
	mk8253. 20110419.캐릭터 코디 정보 리턴.

	{ call dbo.ASGAAgitCharCoordi_select ( @1 ) }
	@1 ; @iLoginUID_ [int]

	n return ( @1, @2, @3, @4 )
	@1 ; CharType int
	@2 ; SlotID   tinyint
	@3 ; ItemUID  bigint
	@4 ; ItemID   int
	@5 ; IsEquip
	*/

	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitCharCoordi_select ( %d ) }",
		dwAgitUID_));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		int nCharType = -1;
		DWORD dwSlotID = 0;
		KSimpleCoordiItem kSimpleCoordiItem;
		bool bEquip = false;

		VERIFY_QUERY(m_kODBC >> nCharType
			>> dwSlotID
			>> kSimpleCoordiItem.m_ItemUID
			>> kSimpleCoordiItem.m_ItemID
			>> bEquip);

		if (bEquip) {
			mapList_[nCharType].m_mapCoordi[dwSlotID] = kSimpleCoordiItem;
		}
	}

	START_LOG(clog, L"캐릭터 코디 정보 리턴.. size : " << mapList_.size()) << END_LOG;
	return true;
}

bool KAGDBThread::LoginUIDtoNickname_select(IN const DWORD& dwUserUID_, OUT std::wstring& strNick_)
{
	/*
	20110720. mk8253. LoginUID로 닉네임을 찾자

	{ call dbo.LNGALoginUIDtoNickname_select ( @1 ) }
	@1 ; @iLoginUID_ [int]

	1 return ( @1 )
	@1 ; Nickname
	*/
	DBJIF2(m_kODBC.Query(L"{ call dbo.LNGALoginUIDtoNickname_select ( %d ) }",
		dwUserUID_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> strNick_);
	}
	m_kODBC.EndFetch();

	START_LOG(clog, L"LoginUID : " << dwUserUID_ << L" Nickname : " << strNick_)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
	return true;
}

bool KAGDBThread::AgitGuestBook_insert(IN const DWORD& dwAgitUID_, IN const DWORD& dwUserUID_, IN const std::wstring& strMsg_, OUT int& nNo_)
{
	/*
	낙서장 글쓰기

	{ call dbo.ASGAAgitGuestBook_insert( @1, @2, @3 ) }
	@1 ; @iLoginUID_  int
	@2 ; @iLoginUIDE_ int
	@3 ; @strContent_ nvarchar(60)

	1 return ( @1, @2 )
	@1 ; OK int
	-101이하 ; 트랜젝션 에러
	@2 ; No
	*/
	nNo_ = -1;

	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitGuestBook_insert ( %d, %d, N'%s') }",
		dwAgitUID_, dwUserUID_, strMsg_.c_str()));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK
			>> nNo_);
	}
	m_kODBC.EndFetch();

	_LOG_SUCCESS(nOK == 0, L"낙서장 글쓰기.. Ret : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< BUILD_LOG(nNo_) << END_LOG;

	return (nOK == 0);
}

bool KAGDBThread::AgitGuestBook_delete(IN const DWORD& dwAgitUID_, IN const DWORD& dwUserUID_, IN const int& nNo_)
{
	/*
	낙서장 글삭제

	{ call dbo.ASGAAgitGuestBook_delete( @1, @2, @3 ) }
	@1 ; @iLoginUID_  int
	@2 ; @iNo_        int
	@3 ; @iLoginUIDE_ int

	1 return ( @1 )
	@1 ; OK int
	-101이하 ; 트랜젝션 에러
	*/
	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitGuestBook_delete ( %d, %d, %d) }",
		dwAgitUID_, nNo_, dwUserUID_));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	_LOG_SUCCESS(nOK == 0, L"낙서장 글삭제.. Ret : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return (nOK == 0);
}

bool KAGDBThread::AgitGuestBook_select(IN const DWORD& dwAgitUID_, OUT std::map<int, KGuestMsg>& mapList_)
{
	/*
	낙서장 보기.

	{ call dbo.ASGAAgitGuestBook_select ( @1 ) }
	@1 ; @iLoginUID_ [int]

	n return ( @1, @2, @3, @4 )
	@1  ; LoginUIDE int
	@2  ; No        int
	@3  ; RegDate   smalldatetime
	@4  ; Content   nvarchar(60)
	*/
	mapList_.clear();

	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitGuestBook_select ( %d ) }",
		dwAgitUID_));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		int nNo = -1;
		KGuestMsg kInfo;

		VERIFY_QUERY(m_kODBC >> kInfo.m_dwUserUID
			>> nNo
			>> kInfo.m_tmDate
			>> kInfo.m_strMsg);

		mapList_[nNo] = kInfo;
	}

	std::map<DWORD, std::wstring> mapUserNick;
	mapUserNick.clear();

	// 닉네임 설정
	std::map<int, KGuestMsg>::iterator mit;
	for (mit = mapList_.begin(); mit != mapList_.end(); ++mit) {
		if (mapUserNick.find(mit->second.m_dwUserUID) != mapUserNick.end()) {
			mit->second.m_strNick = mapUserNick[mit->second.m_dwUserUID];
			continue;
		}

		LIF(LoginUIDtoNickname_select(mit->second.m_dwUserUID, mit->second.m_strNick));
		mapUserNick[mit->second.m_dwUserUID] = mit->second.m_strNick;
	}

	START_LOG(clog, L"낙서장 보기.. size : " << mapList_.size()) << END_LOG;
	return true;
}

bool KAGDBThread::AgitTrainingOBJ_merge(IN const DWORD& dwAgitUID_, IN const GCITEMUID& ItemUID_, IN const int& nDiff_, IN const time_t& tmUpdate_)
{
	/*
	훈련소 등록, 회수

	{ call dbo.ASGAAgitTrainingOBJ_merge ( @1, @2, @3, @4 ) }
	@1 ; @iLoginUID_           int
	@2 ; @iItemUID_            bigint
	@3 ; @iAccCountDiff_       smallint
	@4 ; @sdtLastTrainingTime_ smalldatetime

	1 return ( @1 )
	@1 ; OK int
	-101이하 ; 트랜젝션 에러
	*/
	std::wstring strDate = (LPCTSTR)CTime(tmUpdate_).Format(_T("%Y%m%d %H:%M"));

	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitTrainingOBJ_merge ( %d, %I64d, %d, N'%s') }",
		dwAgitUID_, ItemUID_, nDiff_, strDate.c_str()));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	_LOG_SUCCESS(nOK == 0, L"훈련소 등록, 회수.. Ret : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return (nOK == 0);
}

bool KAGDBThread::AgitTrainingOBJ_select(IN const DWORD& dwAgitUID_, OUT std::map<GCITEMUID, KTrainingObj>& mapList_)
{
	/*
	훈련소 내용 보기

	{ call dbo.ASGAAgitTrainingOBJ_select ( @1 ) }
	@1 ; @iLoginUID_ [int]

	n return ( @1, @2, @3 )
	@1  ; ItemUID          bigint
	@2  ; AccCount         int
	@3  ; LastTrainingTime smalldatetime
	*/
	mapList_.clear();

	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitTrainingOBJ_select ( %d ) }",
		dwAgitUID_));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		KTrainingObj kInfo;

		VERIFY_QUERY(m_kODBC >> kInfo.m_ItemUID
			>> kInfo.m_nInitAccCount
			>> kInfo.m_tmDate);

		kInfo.m_nAccCount = kInfo.m_nInitAccCount;
		mapList_[kInfo.m_ItemUID] = kInfo;
		START_LOG(clog, L"AgitUID : " << dwAgitUID_ << L", TrainingObj UID : " << kInfo.m_ItemUID << L", Count : " << kInfo.m_nAccCount) << END_LOG;
	}

	USHORT usMaxCount = SiKAgitManager()->GetTrainingMaterialLimit();
	std::map<GCITEMUID, KTrainingObj>::iterator mit;
	for (mit = mapList_.begin(); mit != mapList_.end(); ++mit) {
		int nDiffTemp = 0;

		// 최소,최대값 보장
		if (mit->second.m_nAccCount < 0) {
			nDiffTemp -= mit->second.m_nAccCount;
		}
		else if (mit->second.m_nAccCount > usMaxCount) {
			nDiffTemp -= (mit->second.m_nAccCount - usMaxCount);
		}
		else {
			continue; // 정상
		}

		LIF(AgitTrainingOBJ_merge(dwAgitUID_, mit->second.m_ItemUID, nDiffTemp, mit->second.m_tmDate));

		START_LOG(cwarn, L"잘못된 훈련소 DB데이터 초기화 진행.. AgitUID : " << dwAgitUID_)
			<< BUILD_LOG(nDiffTemp)
			<< BUILD_LOG(mit->second.m_nAccCount)
			<< BUILD_LOGtm(CTime(mit->second.m_tmDate)) << END_LOG;

		mit->second.m_nAccCount += nDiffTemp;
		mit->second.m_nInitAccCount = mit->second.m_nAccCount;
	}

	START_LOG(clog, L"훈련소 내용 보기.. size : " << mapList_.size()) << END_LOG;
	return true;
}

bool KAGDBThread::AgitCharDesignCoordi_select(IN const DWORD& dwUserUID_, IN OUT std::map<int, KAgitCharacterPos>& mapCharacterPos_)
{
	/*
	L"{ call dbo.UIGAUserItemCoordi2_select_1_20131008 ( %d, %I64d ) }"

	{ call dbo.UIGAUserItemCoordi2_select_1_20131008 ( @01, @02 ) }
	@01iLoginUID_input int
   ,@02iItemUID_input  bigint

	1 return ( @01 )
	@01iItemID int
	*/

	std::map<int, KAgitCharacterPos>::iterator mit;
	std::map<DWORD, KSimpleCoordiItem>::iterator mitCoordi;
	for (mit = mapCharacterPos_.begin(); mit != mapCharacterPos_.end(); ++mit) {
		for (mitCoordi = mit->second.m_mapCoordi.begin(); mitCoordi != mit->second.m_mapCoordi.end(); ++mitCoordi) {
			DBJIF2(m_kODBC.Query(L"{ call dbo.UIGAUserItemCoordi2_select_1_20131008 ( %d, %I64d ) }",
				dwUserUID_, mitCoordi->second.m_ItemUID));

			GCITEMID ItemID = 0;
			if (m_kODBC.BeginFetch()) {
				VERIFY_QUERY(m_kODBC >> ItemID);
			}
			m_kODBC.EndFetch();

			if (ItemID != 0) {
				mitCoordi->second.m_DesignCoordiID = ItemID;
			}

			START_LOG(clog, L"유저 코디 외형 정보 가져오기")
				<< BUILD_LOG(m_kODBC.m_strLastQuery)
				<< END_LOG;
		}
	}

	return true;
}