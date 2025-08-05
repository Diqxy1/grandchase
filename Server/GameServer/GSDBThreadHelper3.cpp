#include <WinSock2.h>
#include "GSDBThread.h"
#include <dbg/dbg.hpp>
#include <boost/bind.hpp>
#include "GSDBLayer.h"
#include "ItemManager.h"
#include "NetError.h"
#include "GameCouponManager.h"
#include "ResultManager.h"
#include "SkillManager.h"
#include "LogManager.h"
#include "MissionManager.h"
#include "MissionInfo.h"

#include "GSSimLayer.h"
#include "GameServer.h"
#include "IPAdvantage.h"
#include "GCHelper.h"
#include "DicePlay.h"
#include "AgitHelper.h"
#include "EventType.h"
#include "PostConfig.h"
#include "StrengthManager.h"
#include "GuildManager.h"
#include "SealedChar.h"
#include "PVPMatchManager.h"

#define CLASS_TYPE KGSDBThread

void KGSDBThread::GetItemListFromDB(IN const DWORD dwUID_, IN const char cCharType_, OUT std::vector<KItem>& vecInven_, OUT std::vector<std::pair<GCITEMUID, GCITEMID>>& vecDesignCoordi_)
{
	std::vector< KItem > vecItem;
	vecItem.clear();

	// 아이템 기본 정보 조회
	m_kHelper.UIGAUserItem_select(dwUID_, cCharType_, vecItem);

	// 아이템 수량 정보 조회
	m_kHelper.UIGAUserItemDuration_select(dwUID_, cCharType_, vecItem);

	// 아이템 기간 정보 조회
	m_kHelper.UIGAUserItemPeriod_select(dwUID_, cCharType_, vecItem);

	// 아이템 속성값 조회
	m_kHelper.UIGAUserItemAttribute_select(dwUID_, cCharType_, vecItem);

	// 아이템 소켓값 조회
	m_kHelper.UIGAUserItemSocket_select(dwUID_, cCharType_, vecItem);

	LIF(m_kHelper.UserItemLevel_select_20120103(dwUID_, vecItem, cCharType_));

	std::vector<GCITEMUID> vecWareHouseItem; // 유저 창고 전체 아이템 UID List.
	std::vector<GCITEMUID> vecRemoveItem; // 삭제 필요한 강화석 아이템 정보.
	m_kHelper.UserWareHouseTotalselect(dwUID_, vecWareHouseItem);

	// 아이템 강화 정보 조회
	m_kHelper.UserItemStrengthInfo(dwUID_, cCharType_, vecWareHouseItem, vecItem, vecRemoveItem);

	if (!vecRemoveItem.empty()) {
		DeleteRemoveStrengthItem(L"", cCharType_, dwUID_, vecRemoveItem);
	}

	/*
	아이템에 지정된 본래의 캐릭터 타입으로 보정하는 역할의 함수지만
	함수 내부 구현도 잘못되어 있고(5번째 인자를 사용하지 않는다)
	구현상 목적이 불분명하여 사용하지 않는다(8880같은 공용 아이템의 itemID가 캐릭터별로 분리되어 있지 않다)
	혹시나 해서 코드는 남겨둔다
	*/
	//m_kHelper.UserItem_LoginUID_CharType_select( dwUID_, cCharType_, vecItem );

	// 유저의 아이템 획득날짜 가져오기.
	m_kHelper.GetUserItemReceiveDate(dwUID_, vecItem);
	m_kHelper.GetUserItemCooriCompose(dwUID_, vecItem, vecDesignCoordi_);

	vecInven_.swap(vecItem);
}

void KGSDBThread::GetPetInfo(IN const DWORD dwUID_, IN const char& cCharType_, IN const std::vector<KItem>& vecInven_, OUT std::map<GCITEMUID, KPetInfo>& mapPetInfo_)
{
	/*
	L"{ call dbo.UIGAUserItemPetEquip_select_20130510 ( %d, %d ) }"

	{ call dbo.UIGAUserItemPetEquip_select_20130510 ( @01, @02 ) }
	@01: @01iLoginUID_input int
	@02: @02iCharType_input int

	n return ( @01, @02 )
	@01: PetUID  bigint
	@02: ItemUID bigint
	*/

	DBJIF(m_kODBC.Query(L"{ call dbo.UIGAUserItemPetEquip_select_20130510 ( %d, %d ) }", dwUID_, cCharType_));

	while (m_kODBC.Fetch() != SQL_NO_DATA)
	{
		GCITEMUID dwPetUID;
		KSimpleItem kItem;
		VERIFY_QUERY(m_kODBC >> dwPetUID
			>> kItem.m_dwUID);

		//인벤에 해당 아이템이 있으면
		std::vector<KItem>::const_iterator vit;
		vit = std::find_if(vecInven_.begin(), vecInven_.end(), boost::bind(&KItem::m_ItemUID, _1) == kItem.m_dwUID);
		if (vit == vecInven_.end()) {

			START_LOG(cwarn, L"펫에 장착하려는 아이템이 내 인벤토리에 없음")
				<< BUILD_LOG(dwUID_)
				<< BUILD_LOG(dwPetUID)
				<< BUILD_LOG(kItem.m_dwUID) << END_LOG;

			continue;
		}

		//해당 펫을 찾아 장착 시켜 준다.
		std::map<GCITEMUID, KPetInfo>::iterator mit;
		mit = mapPetInfo_.find(dwPetUID);
		if (mit == mapPetInfo_.end()) {
			START_LOG(cerr, L"장비 장착하려는 펫이 없음")
				<< BUILD_LOG(dwUID_)
				<< BUILD_LOG(dwPetUID)
				<< BUILD_LOG(vit->m_ItemID)
				<< BUILD_LOG(kItem.m_dwUID) << END_LOG;
			continue;
		}

		kItem.m_dwID = vit->m_ItemID;
		mit->second.m_vecEquipItem.push_back(kItem);
		mit->second.m_vecInitEquipItem.push_back(kItem);
	}

}

void KGSDBThread::UpdateItemEquipLevelList()
{
	/*
	mk8253. 20100118.
	아이템 장착 정보 리턴

	L"{ call dbo.IIGAItemInfoEquipLevel_select }"

	{ call dbo.IIGAItemInfoEquipLevel_select }

	n return ( @1, @2 )
	@1 ; GoodsID
	@2 ; GoodsClass
	*/
	std::map< DWORD, DWORD > mapItemEquipLevel;
	DBJIF(m_kODBC.Query(L"{ call dbo.IIGAItemInfoEquipLevel_select }"));
	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		GCITEMID ItemID;
		DWORD Level;
		VERIFY_QUERY(m_kODBC >> ItemID >> Level);

		mapItemEquipLevel.insert(std::make_pair(ItemID, Level));
	}

	SiKItemManager()->UpdateItemEquipLevelList(mapItemEquipLevel);

}

bool KGSDBThread::GetStrongItemLevel(IN const GCITEMID dwItemID_, OUT int& nItemKind_, OUT int& nNowLevel_, OUT int& nCharacterType_)
{
	//{ call dbo.SIG_strong_item_level ( @1 ) }
	//@1 : ItemID int
	//
	//return 1 loop select
	//( Kind tinyint ) ; iKindItem
	//( Level tinyint ) ; iNextLevel이 아님 -ㅅ- ( 현재 아이템의 레벨임 )
	//( CharType tinyint ) ; iCharType
	//해당 입력된 아이템 ID에 대한 정보임
	//아무 정보도 리턴되지 않으면 강화 아이템이 아님
	nItemKind_ = -1;
	nNowLevel_ = -1;
	nCharacterType_ = -1;

	DBJIF2(m_kODBC.Query(L"{ call dbo.SIG_strong_item_level ( %d ) }",
		dwItemID_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nItemKind_ >> nNowLevel_ >> nCharacterType_);
		m_kODBC.EndFetch();
	}

	START_LOG(clog, m_kODBC.m_strLastQuery)
		<< BUILD_LOG(nItemKind_)
		<< BUILD_LOG(nNowLevel_)
		<< BUILD_LOG(nCharacterType_) << END_LOG;

	return true;
}

int KGSDBThread::UpdateEnchantStrongItem(IN const DWORD dwUID_, IN const char& cCharType_, IN const GCITEMUID dwItemUID_, IN const GCITEMID dwItemID_, IN const int nDecision_, OUT GCITEMID & dwNextItemID_)
{
	/*
	L"{ call dbo.UIGAUserItem_update_Enchant_ItemID_20130514 ( %d, %d, %I64d, %d, %d ) }"

	{ call dbo.UIGAUserItem_update_Enchant_ItemID_20130514 ( @01, @02, @03, @04, @05 ) }
	@01: @01iLoginUID_input int
	@02: @02iCharType_input int
	@03: @03iItemUID_input  bigint
	@04: @04iItemID_input   int
	@05: @05iDecision_input tinyint
	0: 1단계 상승
	1: 1단계 하락
	2: 깨짐
	3: 변화없음

	1 select ( @01, @02 )
	@01: OK     int
	0       : 성공
	-1      : 유저 정보 없음
	-2      : 메인 아이템 정보 없음
	-3      : 해당 캐릭터가 보유하고 있는 아이템이 아님
	-4      : 이 아이템은 강화할 수 있는 아이템이 아님
	-5      : 다음 단계의 강화 정보를 찾을 수 없음
	-801~-899: 무시가능 에러
	-901~-999: 치명적인 에러
	@02: ItemID int
	*/

	int nRet = -99;
	dwNextItemID_ = 0;

	DBJIF3(m_kODBC.Query(L"{ call dbo.UIGAUserItem_update_Enchant_ItemID_20130514 ( %d, %d, %I64d, %d, %d ) }",
		dwUID_, cCharType_, dwItemUID_, dwItemID_, nDecision_),
		return nRet);
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet >> dwNextItemID_);
		m_kODBC.EndFetch();
	}

	LIF(m_kHelper.IgnorableErrorProcessingAfterCallSP(nRet));

	_LOG_SUCCESS(nRet == 0, L"Result : " << nRet)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< BUILD_LOG(dwNextItemID_) << END_LOG;

	return nRet;
}

void KGSDBThread::PresentBoxInsertLog(IN DWORD dwUID_, IN DWORD dwVersion_, IN GCITEMID dwItemID_, IN int nDuration_, IN int nPeriod_)
{
	/*
	microcat. 20100225
	선물 상자 이벤트 아이템 획득 로그.

	L"{ call dbo.PBGAPresentBox_insert ( %d, %d, %d, %d, %d, %d ) }"

	{ call dbo.PBGAPresentBox_insert ( @1, @2, @3, @4, @5, @6 ) }
	@1 ; @iLoginUID_ int
	@2 ; @iVersion_  tinyint
	@3 ; @iItemID_   int
	@4 ; @iGradeID_  tinyint
	@5 ; @iPeriod_   int
	@6 ; @iDuration_ int

	1 select ( @1 )
	@1 ; OK int
	0        ; 성공
	-1       ; 유저 정보가 없음
	-101이하 ; 트랜잭션 에러
	*/

	// 아이템의 Grade정보는 그냥 일괄적으로 0으로 넣어준다.
	int nOK = -99;
	DBJIF(m_kODBC.Query(L"{ call dbo.PBGAPresentBox_insert ( %d, %d, %d, %d, %d, %d ) }",
		dwUID_, dwVersion_, dwItemID_, 0, nPeriod_, nDuration_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();
	if (nOK != 0) {
		START_LOG(cerr, L"선물 상자 이벤트 아이템 지급 로그 남기기 실패. ")
			<< BUILD_LOG(nOK)
			<< BUILD_LOG(dwUID_)
			<< BUILD_LOG(dwVersion_)
			<< BUILD_LOG(dwItemID_)
			<< BUILD_LOG(nDuration_)
			<< BUILD_LOG(nPeriod_)
			<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
	}
}

void KGSDBThread::GetGuildMemeberInfoFromDB(IN DWORD dwGuildUID_, IN DWORD dwUserUID_, OUT KNGuildUserInfo & kGuildUserInfo_, OUT time_t & tmJoinDate_, OUT time_t & tmLastLogin_)
{
	/*
	20091109. microcat.
	*/
	/*
	L"{ call dbo.GSGAGuildSystemMember_select_1_member ( %d, %d ) }"

	{ call dbo.GSGAGuildSystemMember_select_1_member ( @1, @2 ) }
	@1 ; @iNID_      int
	@2 ; @iLoginUID_ int

	1 select ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10 )
	@1  ; 호출된 sp이름 nvarchar( 4000 )
	@2  ; NID           int
	@3  ; LoginUID      int
	@4  ; RegDateA      datetime
	@5  ; Grade         tinyint
	0 ; 가입 대기 유저
	1 ; 일반 길드원
	3 ; 제너럴
	5 ; 길드마스터
	@6  ; Contents      nvarchar( 100  )
	@7  ; GPoint        bigint
	@8  ; Login         nvarchar( 20   )
	@9  ; Nickname      nvarchar( 24   )
	@10 ; LastLogin     smalldatetime
	*/
	if (dwGuildUID_ == 0) return;

	std::wstring strSP;
	DBJIF(m_kODBC.Query(L"{ call dbo.GSGAGuildSystemMember_select_1_member_color ( %d, %d ) }",
		dwGuildUID_, dwUserUID_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> strSP
			>> kGuildUserInfo_.m_dwGuildUID
			>> kGuildUserInfo_.m_dwUserUID
			>> tmJoinDate_
			>> kGuildUserInfo_.m_cMemberLevel
			>> kGuildUserInfo_.m_strMyComment
			>> kGuildUserInfo_.m_dwContributePoint
			>> kGuildUserInfo_.m_strLogin
			>> kGuildUserInfo_.m_strNickName
			>> kGuildUserInfo_.m_strNickColor
			>> tmLastLogin_);
	}
	m_kODBC.EndFetch();
}

void KGSDBThread::GBGAGuideBook_merge(IN const DWORD dwUID_, IN const std::pair<short, short> & prData_)
{
	/*
	L"{ call dbo.GBGAGuideBook_merge_20130703( %d, %d ) }"

	{ call dbo.GBGAGuideBook_merge_20130703 ( @01, @02 ) }
	@01iLoginUID_input int
   ,@02iCharType_input int

	1 return ( @01 )
	@01iOK int
		0        : 성공
		-1       : 유저 정보 없음
		-2       : 캐릭터 정보 없음
		-801~-899: 무시가능 에러
		-901~-999: 치명적인 에러
	*/

	DBJIF(m_kODBC.Query(L"{ call dbo.GBGAGuideBook_merge_20130703( %d, %d ) }",
		dwUID_, prData_.second));

	int nOK = -99;

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	LIF(m_kHelper.IgnorableErrorProcessingAfterCallSP(nOK));

	_LOG_SUCCESS(nOK == 0, L"가이드북 기록 저장 OK : " << nOK) << END_LOG;
}

void KGSDBThread::GetGameCouponInfoFromDB(IN const int nSet_)
{
	/*
	20090306. microcat.
	L"{ call dbo.GCGA_game_coupon_select_info ( %d ) }"

	{ call dbo.GCGA_game_coupon_select_info ( @1 ) }
	@1 ; @bAllDisplay_ [bit] = 0

	n return ( @1, @2, @3, @4, @5, @6 )
	@1 ; EventUID int
	@2 ; New bit
	@3 ; Name nvarchar(20)
	@4 ; StartDateB smalldatetime
	@5 ; EndDateB smalldatetime
	@6 ; Contents nvarchar(25)
	*/

	// nSet_ 값
	// 1 (IT_ALL_INFO)    : 모든 정보를 준다.(서버실행시 최초1회)
	// 0 (IT_UPDATE_INFO) : 갱신된 정보만 준다.(주기적으로 실행)

	KEVENT_GAME_COUPON_LIST_ACK vecGameCouponInfo;
	time_t tmCurrent = KncUtil::TimeToInt(CTime::GetCurrentTime()); // 현재시간 체크

	DBJIF(m_kODBC.Query(L"{ call dbo.GCGA_game_coupon_select_info ( %d ) }", nSet_));
	while (m_kODBC.Fetch() != SQL_NO_DATA)
	{
		KGameCouponInfo kData;
		kData.m_nEventUID = -1;

		VERIFY_QUERY(m_kODBC >> kData.m_nEventUID
			>> kData.m_bNew
			>> kData.m_strEventName
			>> kData.m_tStartDate
			>> kData.m_tEndDate
			>> kData.m_strContents);

		// 현재 동작중인 게임쿠폰 이벤트인지 체크.
		if (kData.m_tStartDate <= tmCurrent && kData.m_tEndDate >= tmCurrent)
		{
			kData.m_bActive = true;
		}
		else
		{
			kData.m_bActive = false;
		}

		vecGameCouponInfo.push_back(kData);
	}

	// 정보 갱신
	if (nSet_ == SiKGameCouponManager()->IT_ALL_INFO)
	{
		SiKGameCouponManager()->SetGameCouponInfo(vecGameCouponInfo);
	}
	else if (nSet_ == SiKGameCouponManager()->IT_UPDATE_INFO)
	{
		SiKGameCouponManager()->UpdateGameCouponInfo(vecGameCouponInfo);
	}
	else
	{
		START_LOG(cerr, L"설정에 없는 게임쿠폰 정보유형입니다. nSet_ : " << nSet_) << END_LOG;
	}

	START_LOG(clog, L"GameCoupon Info.")
		<< BUILD_LOG(nSet_)
		<< BUILD_LOG(vecGameCouponInfo.size())
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
}

int KGSDBThread::CheckGameCoupon(IN const DWORD dwUID_, IN const int nEventUID_, IN const std::wstring & strCouponID_, OUT DWORD & dwCouponUID_)
{
	/*
	20090306. microcat.
	L"{ call dbo.GCGA_game_coupon_check ( %d, %d, N'%s' ) }"

	{ call dbo.GCGA_game_coupon_check ( @1, @2, @3 ) }
	@1 ; @iLoginUID_ [int],
	@2 ; @iEventUID_ [int],
	@3 ; @strCouponID_ [nvarchar](20),

	1 return ( @1, @2 )
	@1 ; OK int
	0         ; 성공
	-1        ; 유저 정보가 없음
	-2        ; 해당 이벤트가 없거나 이벤트 기간이 아님
	-3        ; 발행된 쿠폰 번호가 아님.
	-4        ; 이미 사용된 쿠폰임
	@2 ; CouponUID ; 쿠폰ID와 1:1 매칭이 되는 고유 번호
	*/
	int nRet = -99;
	dwCouponUID_ = -99;

	DBJIF2(m_kODBC.Query(L"{ call dbo.GCGA_game_coupon_check ( %d, %d, N'%s' ) }",
		dwUID_, nEventUID_, strCouponID_.c_str()));

	if (m_kODBC.BeginFetch())
	{
		VERIFY_QUERY(m_kODBC >> nRet >> dwCouponUID_);
		m_kODBC.EndFetch();

		// 에러값 처리, 리턴값 어떨거냐?
		if (nRet != 0)
		{
			START_LOG(cerr, L"게임쿠폰 체크 DB실패.. OK : " << nRet << L" Query : " << m_kODBC.m_strLastQuery) << END_LOG;
		}
	}

	return nRet;
}

void KGSDBThread::GetGameCouponItem(IN const int dwEventUID_, OUT std::vector<KDropItemInfo> & vecItems_)
{
	/*
	20090306. microcat.
	L"{ call dbo.GCGA_game_coupon_select_item ( %d ) }"

	{ call dbo.GCGA_game_coupon_select_item ( @1 ) }
	@1 ; @iEventUID_ [int],

	n return ( @1, @2 )
	@1 ; ItemID int
	@2 ; Period   int
	@3 ; Duration int
	*/
	DBJIF(m_kODBC.Query(L"{ call dbo.GCGA_game_coupon_select_item ( %d ) }", dwEventUID_));
	while (m_kODBC.Fetch() != SQL_NO_DATA)
	{
		KDropItemInfo kData;
		kData.m_nPeriod = 0;

		VERIFY_QUERY(m_kODBC >> kData.m_ItemID
			>> kData.m_nPeriod
			>> kData.m_nDuration);

		vecItems_.push_back(kData);
	}

	START_LOG(clog, L"게임쿠폰아이템 목록 얻어오기 완료. Size:" << vecItems_.size())
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
}

int KGSDBThread::UpdateGameCoupon(IN const DWORD dwUID_, IN const int nEventUID_, IN const DWORD dwCouponUID_)
{
	/*
	20090306. microcat.

	L"{ call dbo.GCGA_game_coupon_update ( %d, %d, %d ) }"

	{ call dbo.GCGA_game_coupon_update ( @1, @2, @3 ) }
	@1 ; @iLoginUID_ [int],
	@2 ; @iEventUID_ [int],
	@3 ; @iCouponUID_ [int],

	1 return ( @1 )
	@1 ; OK int
	0         ; 성공
	-1        ; 유저 정보가 없음
	-2        ; 해당 이벤트가 없거나 이벤트 기간이 아님
	-3        ; 발행된 쿠폰 번호가 아님
	-4        ; 이미 사용된 쿠폰임
	-101 이하 ; 트랜잭션 에러
	*/
	int nRet = -99;

	DBJIF2(m_kODBC.Query(L"{ call dbo.GCGA_game_coupon_update ( %d, %d, %d ) }",
		dwUID_, nEventUID_, dwCouponUID_));

	if (m_kODBC.BeginFetch())
	{
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();

		// 에러값 처리, 리턴값 어떨거냐?
		if (nRet != 0)
		{
			START_LOG(cerr, L"사용한 게임쿠폰정보 갱신 DB실패.. OK : " << nRet << L" Query : " << m_kODBC.m_strLastQuery) << END_LOG;
		}
	}

	return nRet;
}

bool KGSDBThread::CheckGameCouponErr(IN const DWORD dwUID_)
{
	/*
	mk8253. 20091005
	쿠폰 오류 정보
	{ call dbo.GCGA_game_coupon_error_select ( @1, @2 ) }
	@1 ; iLoginUID_ int
	@2 ; sdtRegDate_ smalldatetime

	1 return ( @1 )
	@1 ; Error bit

	Error
	0 ; 최초
	1 ; 두번째
	*/
	_DBJIF(m_kODBC.Query(L"{ call dbo.GCGA_game_coupon_error_select ( %d, N'%s' ) }",
		dwUID_, (LPCTSTR)CTime::GetCurrentTime().Format(L"%Y%m%d"))
		, return false, (&m_kODBC));

	int nRet = -1;
	if (m_kODBC.BeginFetch())
	{
		VERIFY_QUERY(m_kODBC >> nRet);
	}
	m_kODBC.EndFetch();

	START_LOG(clog, L"dwUID_ : " << dwUID_ << L", nRet : " << nRet) << END_LOG;
	return (nRet != 1);
}

bool KGSDBThread::UpdateGameCouponFailErr(IN const DWORD dwUID_)
{
	/*
	mk8253. 20091005
	쿠폰 오류 기록
	{ call dbo.GCGA_game_coupon_error_merge ( @1, @2 ) }
	@1 ; iLoginUID_ int
	@2 ; sdtRegDate_ smalldatetime

	1 return ( @1 )
	@1 ; OK bit

	OK
	0 ; 성공
	-1 ; 유저 정보가 존재하지 않음
	-101이하 ; 트랜잭션 에러
	*/
	_DBJIF(m_kODBC.Query(L"{ call dbo.GCGA_game_coupon_error_merge ( %d, N'%s' ) }",
		dwUID_, (LPCTSTR)CTime::GetCurrentTime().Format(L"%Y%m%d"))
		, return false, (&m_kODBC));

	int nRet = -1;
	if (m_kODBC.BeginFetch())
	{
		VERIFY_QUERY(m_kODBC >> nRet);
	}
	m_kODBC.EndFetch();

	START_LOG(clog, L"dwUID_ : " << dwUID_ << L", nRet : " << nRet) << END_LOG;
	return (nRet == 0);
}

bool KGSDBThread::UpdateAddSkillTreeSet(IN const DWORD dwUID_, IN int nChar_, IN int nPromotion_, IN int nSetID_, IN int nSlotIndex_, IN int nSlotID_, IN int nSkillID_)
{
	// 빈 슬롯은 DB 저장 안하고 성공.
	if (SiKSkillManager()->CheckSkillExceptionID(nChar_, nSkillID_)) {
		return true;
	}

	/*
	L"{ call dbo.STGASkillTreeSet_insert_20120305 ( %d, %d, %d, %d, %d, %d, %d ) }"

	{ call dbo.STGASkillTreeSet_insert_20120305 ( @1, @2, @3, @4, @5, @6, @7 ) }
	@1: @iLoginUID_  int
	@2: @iCharType_  tinyint
	@3: @iPromotion_ tinyint
	@4: @iSetID_     tinyint
	@5: @iSlotIndex_ int
	@6: @iSlotID_    int
	@7: @iSkillID_   int

	1 return ( @1 )
	@1 ; OK int
		0       : 성공
		-1      : 계정이 존재하지 않음
		-2      : 스킬 장착 정보가 이미 존재함
		-101이하: 트랜잭션 에러
	*/
	int nOK = -99;

	DBJIF2(m_kODBC.Query(L"{ call dbo.STGASkillTreeSet_insert_20120305 ( %d, %d, %d, %d, %d, %d, %d ) }", dwUID_, nChar_, nPromotion_, nSetID_, nSlotIndex_, nSlotID_, nSkillID_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(0 == nOK || -2 == nOK, L"Result : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return (0 == nOK || -2 == nOK);
}

bool KGSDBThread::UpdateDeleteSkillTreeSet(IN const DWORD dwUID_, IN int nChar_, IN int nPromotion_, IN int nSetID_, IN int nSlotIndex_, IN int nSlotID_, IN int nSkillID_)
{

	/*
	L"{ call dbo.STGASkillTreeSet_delete_20120305 ( %d, %d, %d, %d, %d, %d, %d ) }"

	{ call dbo.STGASkillTreeSet_delete_20120305 ( @1, @2, @3, @4, @5, @6, @7 ) }
	@1: @iLoginUID_  int
	@2: @iCharType_  tinyint
	@3: @iPromotion_ tinyint
	@4: @iSetID_     tinyint
	@5: @iSlotIndex_ int
	@6: @iSlotID_    int
	@7: @iSkillID_   int

	1 return ( @1 )
	@1 ; OK int
		0       : 성공
		-1      : 계정이 존재하지 않음
		-2      : 스킬 장착 정보가 없음
		-101이하: 트랜잭션 에러
	*/

	int nOK = -99;

	DBJIF2(m_kODBC.Query(L"{ call dbo.STGASkillTreeSet_delete_20120305 ( %d, %d, %d, %d, %d, %d, %d ) }", dwUID_, nChar_, nPromotion_, nSetID_, nSlotIndex_, nSlotID_, nSkillID_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(0 == nOK || -2 == nOK, L"Result : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return (0 == nOK || -2 == nOK);
}

bool KGSDBThread::UpdatePointbagCalender(IN const DWORD & dwUID_, IN const char& cCharType_, IN std::vector<KDailyInfo> & vecDailyInfo_)
{
	/*
	L"{ call dbo.PBGAPointBagCalender_merge_20130523 ( %d, %d, N'%s', %d ) }"

	{ call dbo.PBGAPointBagCalender_merge_20130523 ( @01, @02, @03, @04 ) }
	@01: @01iLoginUID_input   int
	@02: @02iCharType_input   int
	@03: @03sdtRegDateD_input smalldatetime
	@04: @04iType_input       int

	1 return ( @01 )
	@01: OK int
	0        : 성공
	-1       : 유저 정보 없음
	-2       : 캐릭터 정보 없음
	-3       : 출석부 기본 정보 없음
	-801~-899: 무시가능 에러
	-901~-999: 치명적인 에러
	*/
	std::vector<KDailyInfo>::iterator vit;
	for (vit = vecDailyInfo_.begin(); vit != vecDailyInfo_.end(); ++vit) {
		CTime tmTime(static_cast<int>(vit->m_kDate.m_sYear),
			static_cast<int>(vit->m_kDate.m_cMonth),
			static_cast<int>(vit->m_kDate.m_cDay), 0, 0, 0);
		std::wstring strDate = (LPCTSTR)tmTime.Format(_T("%y%m%d"));

		if (!m_kODBC.Query(L"{ call dbo.PBGAPointBagCalender_merge_20130523 ( %d, %d, N'%s', %d ) }",
			dwUID_, cCharType_, strDate.c_str(), static_cast<int>(vit->m_cSealType)))
		{
			START_LOG(cerr, L"현재 달력 DB에 쓰기 실패")
				<< BUILD_LOG(dwUID_)
				<< BUILD_LOG((&m_kODBC)->m_szDiagMsg)
				<< BUILD_LOG((wchar_t*)((&m_kODBC)->m_sqlState))
				<< BUILD_LOG((&m_kODBC)->m_strLastQuery) << END_LOG;

			SET_ERROR(ERR_CALENDAR_09);//달력 정보 로드 실패(클라이언트에서는 달력 로드 실패로 간주함)
		}
		int nOK = 0;
		if (m_kODBC.BeginFetch())
		{
			VERIFY_QUERY(m_kODBC >> nOK);
		}
		m_kODBC.EndFetch();

		LIF(m_kHelper.IgnorableErrorProcessingAfterCallSP(nOK));

		LOG_SUCCESS(nOK == 0)
			<< L"달력 정보 DB 업데이트 " << dbg::endl
			<< BUILD_LOG(dwUID_)
			<< BUILD_LOG(nOK)
			<< BUILD_LOG(vecDailyInfo_.size()) << END_LOG;
	}
	return true;
}

bool KGSDBThread::VerifyCharExpForStat(IN const std::wstring & strLogin_, IN const DWORD dwLoginUID_, IN const KCharacterInfo & kCharInfo_)
{
	// 캐릭터 정보를 가져 온다.
	KCharacterInfo kCharInfo;
	if (!m_kHelper.GetCharacterInfo_1(dwLoginUID_, kCharInfo_.m_cCharType, kCharInfo)) {
		START_LOG(cerr, L"데이터 검증을 위해 캐릭터 정보 가져 오기 실패, LoginUID : " << dwLoginUID_
			<< L", CharType : " << (int)kCharInfo_.m_cCharType) << END_LOG;
		return false;
	}

	// 가져온 정보와 현재 통계로 남길려는 데이터가 다르면 로그를 남기자.
	if (kCharInfo.m_biExp > kCharInfo_.m_biExp) {
		std::wstringstream stm;
		stm << ::GetCurrentThreadId()
			<< L" | " << KNC_TIME_STRING
			<< L" | " << dwLoginUID_
			<< L" | " << strLogin_
			<< L" | " << (int)kCharInfo_.m_cCharType
			<< L" | " << kCharInfo.m_biExp
			<< L" | " << kCharInfo_.m_biExp
			<< L" | " << kCharInfo_.m_dwLevel;

		SiKLogManager()->Get(KLogManager::LT_EXP_STAT_ERR) << stm.str() << GLOG::endl;
		return false;
	}

	return true;
}

bool KGSDBThread::GetCharacterInfo_1(IN const DWORD dwLoginUID_, IN const char cCharType_, OUT KCharacterInfo & kCharInfo_)
{
	bool bret = m_kHelper.GetCharacterInfo_1(dwLoginUID_, cCharType_, kCharInfo_);
	if (bret) {

		kCharInfo_.m_dwLevel = SiKResultManager()->Exp_2_Level(kCharInfo_.m_biExp);
		SiKSkillManager()->InitSkillInfo(kCharInfo_.m_kSkillInfo, cCharType_, kCharInfo_.m_cCurrentPromotion, kCharInfo_.m_dwLevel); // 스킬 정보 얻어 와야 한다.
		SiKSkillManager()->InitSkillSetting(kCharInfo_.m_cPromotion, kCharInfo_.m_mapEquipSkillSet);
		// 캐릭터 ELO 정보 세팅.
		int nSeasonYear = 0;
		int nSeasonVersion = 0;
		SiKPVPMatchManager()->GetSeasonInfo(nSeasonYear, nSeasonVersion);
		GetPVPELOInfo(dwLoginUID_, nSeasonYear, nSeasonVersion, cCharType_, kCharInfo_);
	}

	LOG_SUCCESS(bret == true)
		<< BUILD_LOG(dwLoginUID_)
		<< BUILD_LOGc(cCharType_)
		<< BUILD_LOG(bret)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return bret;
}

bool KGSDBThread::UserLevelupCheck(IN const DWORD dwUID_)
{
	/*
	mk8253. 20100729.
		레벨업이벤트

	L"{ call dbo.LUEVLevelUp_check ( %d ) }"

	{ call dbo.LUEVLevelUp_check ( @1 ) }
	@1 ; @iLoginUID_ [int]

	1 return ( @1 )
		@1 ; OK
		0         ; 성공
		-1        ; 유저 정보가 없음
		-101이하  ; 트랜젝션 에러
	*/
	int nOK = -99;

	DBJIF2(m_kODBC.Query(L"{ call dbo.LUEVLevelUp_check ( %d ) }", dwUID_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nOK == 0, L"Result : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	if (nOK != 0) {
		return false;
	}
	else {
		return true;
	}
}

void KGSDBThread::AdjustLookItemInfo(IN const DWORD dwLoginUID_, IN OUT std::vector<KItem> & vecItems_, IN const char cCharType_)
{
	std::vector<GCITEMUID> vecLookItemUIDs; // 미리 reserve 해야 하나??
	m_kHelper.UIGAUserItemCoordi_select(dwLoginUID_, vecLookItemUIDs, cCharType_);

	std::vector<GCITEMUID>::iterator vit;
	for (vit = vecLookItemUIDs.begin(); vit != vecLookItemUIDs.end(); ++vit) {

		std::vector<KItem>::iterator vitItem;
		vitItem = std::find_if(vecItems_.begin(), vecItems_.end(),
			boost::bind(&KItem::m_ItemUID, _1) == *vit);

		if (vitItem == vecItems_.end())
			continue;

		vitItem->AddItemType(KItem::TYPE_LOOK);
	}
}

void KGSDBThread::LoadFullLookInfo(IN const DWORD dwLoginUID_, OUT KLookFullInfo & kFullInfo_)
{
	std::vector< boost::tuple<char, int, GCITEMUID> > vecLookEquip;
	std::vector< boost::tuple<char, int, std::wstring> > vecCoordiName;
	m_kHelper.UIGAUserItemCoordiEquip_select(dwLoginUID_, vecLookEquip);
	m_kHelper.UIGAUserItemCoordiName_select(dwLoginUID_, vecCoordiName);

	std::vector< boost::tuple<char, int, GCITEMUID> >::iterator vitEquip;
	for (vitEquip = vecLookEquip.begin(); vitEquip != vecLookEquip.end(); ++vitEquip) {
		std::map<int, KCoordiInfo>& mapCharEquip = kFullInfo_.m_mapLookFullInfo[vitEquip->get<0>()];

		std::map<int, KCoordiInfo>::iterator mit;
		mit = mapCharEquip.find(vitEquip->get<1>());
		if (mit == mapCharEquip.end()) {
			KCoordiInfo kInfo;
			kInfo.m_nCoordiID = vitEquip->get<1>();
			kInfo.m_setEquip.insert(vitEquip->get<2>());
			mapCharEquip.insert(std::map<int, KCoordiInfo>::value_type(kInfo.m_nCoordiID, kInfo));
		}
		else {
			mit->second.m_setEquip.insert(vitEquip->get<2>());
		}
	}

	std::vector< boost::tuple<char, int, std::wstring> >::iterator vitName;
	for (vitName = vecCoordiName.begin(); vitName != vecCoordiName.end(); ++vitName) {

		std::map<int, KCoordiInfo>& mapCharEquip = kFullInfo_.m_mapLookFullInfo[vitName->get<0>()];

		std::map<int, KCoordiInfo>::iterator mit;
		mit = mapCharEquip.find(vitName->get<1>());
		if (mit == mapCharEquip.end()) {
			KCoordiInfo kInfo;
			kInfo.m_nCoordiID = vitName->get<1>();
			kInfo.m_strCoordiName = vitName->get<2>();
			mapCharEquip.insert(std::map<int, KCoordiInfo>::value_type(kInfo.m_nCoordiID, kInfo));
		}
		else {
			mit->second.m_strCoordiName = vitName->get<2>();
		}
	}
}

void KGSDBThread::UpdateLookEquipInfo(IN const DWORD dwLoginUID_, IN const std::map<char, std::map<int, KCoordiDiffInfo> > & mapLookEquips_)
{
	std::map<char, std::map<int, KCoordiDiffInfo> >::const_iterator mit;
	for (mit = mapLookEquips_.begin(); mit != mapLookEquips_.end(); ++mit) {

		std::map<int, KCoordiDiffInfo>::const_iterator mit1;
		for (mit1 = mit->second.begin(); mit1 != mit->second.end(); ++mit1) {

			// 이름 저장
			m_kHelper.UIGAUserItemCoordiName_merge(dwLoginUID_, mit->first, mit1->second.m_nCoordiID, mit1->second.m_strCoordiName);

			std::set<GCITEMUID>::const_iterator sit;
			// 해제
			for (sit = mit1->second.m_setRelease.begin(); sit != mit1->second.m_setRelease.end(); ++sit) {
				m_kHelper.UIGAUserItemCoordiEquip_delete(dwLoginUID_, mit->first, mit1->second.m_nCoordiID, *sit);
			}

			// 장착
			for (sit = mit1->second.m_setEquip.begin(); sit != mit1->second.m_setEquip.end(); ++sit) {
				m_kHelper.UIGAUserItemCoordiEquip_insert(dwLoginUID_, mit->first, mit1->second.m_nCoordiID, *sit);
			}

		}
	}
}

void KGSDBThread::BundleSellCountItemSync(IN const DWORD dwLoginUID_, IN const char cCharType_, IN OUT std::vector<std::pair<KItem, int> > & vecCountItems_, IN OUT std::map<GCITEMUID, int> & mapFailInfo_)
{
	std::vector<std::pair<KItem, int> >::iterator vit;
	for (vit = vecCountItems_.begin(); vit != vecCountItems_.end(); ) {

		if (!UpdateCountItem(dwLoginUID_, vit->first.m_ItemUID, vit->first.m_nCount - vit->first.m_nInitCount)) {
			mapFailInfo_.insert(std::map<GCITEMUID, int>::value_type(vit->first.m_ItemUID, NetError::ERR_BUNDLE_SELL_10));
			vit = vecCountItems_.erase(vit);
			continue;
		}
		vit->first.m_nInitCount = vit->first.m_nCount;
		++vit;
	}
}

void KGSDBThread::BundleSellEraseItems(IN const DWORD dwLoginUID_, IN OUT std::vector<GCITEMUID> & vecEraseList_, IN OUT std::map<GCITEMUID, int> & mapFailInfo_, IN const char cCharType_)
{
	std::wstring str;
	std::vector<GCITEMUID>::iterator vit;
	for (vit = vecEraseList_.begin(); vit != vecEraseList_.end(); ) {
		if (!PureEraseItem(str, dwLoginUID_, *vit, cCharType_)) {
			mapFailInfo_.insert(std::map<GCITEMUID, int>::value_type(*vit, NetError::ERR_BUNDLE_SELL_11));
			vit = vecEraseList_.erase(vit);
			continue;
		}
		++vit;
	}
}

void KGSDBThread::BundleSellCountItems(IN const DWORD dwLoginUID_, IN const char cCharType_, IN OUT std::vector<std::pair<KItem, int> > & vecSellCountItem_, IN OUT std::map<GCITEMUID, int> & mapFailInfo_, IN OUT int& nGamePoint_, IN OUT std::map<GCITEMUID, KItem> & mapRewardItem_, OUT std::vector<KItem> & vecSellSucceed_,
	IN const DWORD & dwSlotNum_)
{
	vecSellSucceed_.clear();
	std::vector<std::pair<KItem, int> >::iterator vit;
	for (vit = vecSellCountItem_.begin(); vit != vecSellCountItem_.end(); ) {

		// 판매 하려는 아이템이 출석 아이템인지 확인 한다.
		if (m_kHelper.IsGCPointItem(vit->first.m_ItemID)) {
			mapFailInfo_.insert(std::map<GCITEMUID, int>::value_type(vit->first.m_ItemUID, NetError::ERR_BUNDLE_SELL_14));
			vit = vecSellCountItem_.erase(vit);
			continue;
		}

		__int64 dwWaitUID = 0;
		int nInitGamePoint = 0;
		GCITEMID ItemID = vit->first.m_ItemID;
		// DivideSellItem 함수는 GP 반환이 아니면 GP 를 0을 반환 한다.주의 하자.
		if (!m_kHelper.DivideSellItem(dwLoginUID_, vit->first.m_ItemUID, vit->first.m_ItemID, vit->second, nInitGamePoint, dwWaitUID, cCharType_)) {
			mapFailInfo_.insert(std::map<GCITEMUID, int>::value_type(vit->first.m_ItemUID, NetError::ERR_BUNDLE_SELL_12));
			vit = vecSellCountItem_.erase(vit);
			continue;
		}

		if (nInitGamePoint > 0) {

			if (nInitGamePoint < nGamePoint_) {
				START_LOG(cerr, L"수량성 아이템 판매후, 전체GP가 현재GP보다 적다.. LoginUID : " << dwLoginUID_)
					<< BUILD_LOG(m_kODBC.m_strLastQuery)
					<< BUILD_LOG(ItemID)
					<< BUILD_LOG(nGamePoint_)
					<< BUILD_LOG(nInitGamePoint) << END_LOG;

				// 현재값보다 적을 경우, 현재값만큼 GP 복구한다
				int nOK = -99;
				int nCurrentGP = 0;
				UpdateUserGamePoint(dwLoginUID_, cCharType_, dwSlotNum_, nGamePoint_ - nInitGamePoint, nOK, nCurrentGP);
				if (nOK == 0) {
					if (nInitGamePoint != nGamePoint_) {
						START_LOG(cerr, L"GP 차이값 복구했는데도 DB랑 차이가 있음..LoginUID : " << dwLoginUID_)
							<< BUILD_LOG(ItemID)
							<< BUILD_LOG(nGamePoint_)
							<< BUILD_LOG(nInitGamePoint)
							<< BUILD_LOG(nCurrentGP)
							<< END_LOG;
					}
					nInitGamePoint = nCurrentGP;
				}
			}

			nGamePoint_ = nInitGamePoint;
		}

		vit->first.m_nCount -= vit->second;
		vit->first.m_nInitCount = vit->first.m_nCount;
		vecSellSucceed_.push_back(vit->first);

		// 대기 아이템 정보가 유효 하다면 그것도 가져 온다.
		if (dwWaitUID != KItem::INVALID_ITEM) {
			std::vector<KItem> vecRewardItem;
			std::vector<KItem>::iterator vit1;
			LIF(m_kHelper.InsertSingleWaitItemToInvenOrVDepot(dwLoginUID_, cCharType_, dwWaitUID, KItemManager::EWIT_ITEM_SELL, vecRewardItem));
			for (vit1 = vecRewardItem.begin(); vit1 != vecRewardItem.end(); ++vit1) {
				std::map<GCITEMUID, KItem>::iterator mit;
				mit = mapRewardItem_.find(vit1->m_ItemUID);
				if (mit == mapRewardItem_.end()) {
					mapRewardItem_.insert(std::map<GCITEMUID, KItem>::value_type(vit1->m_ItemUID, *vit1));
				}
				else {
					mit->second.m_nCount += vit1->m_nCount;
				}
			}
		}
		++vit;
	}
}

void KGSDBThread::BundleSellNormalItems(IN const DWORD dwLoginUID_, IN const char cCharType_, IN OUT std::vector<KItem> & vecSellNormalItem_, IN OUT std::map<GCITEMUID, int> & mapFailInfo_, IN OUT int& nGamePoint_, IN OUT std::map<GCITEMUID, KItem> & mapRewardItem_, OUT std::vector<GCITEMUID> & vecSellSucceed_,
	IN const DWORD & dwSlotNum_)
{
	vecSellSucceed_.clear();
	std::vector<KItem>::iterator vit;
	for (vit = vecSellNormalItem_.begin(); vit != vecSellNormalItem_.end(); ) {
		GCITEMID SellItemID = vit->m_ItemID;

		//판매 하려고 한것이 출석 아이템인지 확인.
		bool bGCPointItem = false;
		if (SiKGSSimLayer()->CheckAuthTypeFlag(KGSSimLayer::AT_NETMARBLE)) {
			bGCPointItem = m_kHelper.IsGCPointItem_CashCheck(vit->m_ItemID);
		}
		else {
			bGCPointItem = m_kHelper.IsGCPointItem(vit->m_ItemID);
		}

		if (bGCPointItem) {
			mapFailInfo_.insert(std::map<GCITEMUID, int>::value_type(vit->m_ItemUID, NetError::ERR_BUNDLE_SELL_14));
			vit = vecSellNormalItem_.erase(vit);
			continue;
		}

		__int64 dwWaitUID = 0;
		int nDBGamePoint = 0;
		if (!m_kHelper.SellItem(dwLoginUID_, vit->m_ItemID, vit->m_ItemUID, nDBGamePoint, dwWaitUID, cCharType_)) {
			mapFailInfo_.insert(std::map<GCITEMUID, int>::value_type(vit->m_ItemUID, NetError::ERR_BUNDLE_SELL_13));
			vit = vecSellNormalItem_.erase(vit);
			continue;
		}

		if (nDBGamePoint > 0) {
			if (nDBGamePoint < nGamePoint_) {
				START_LOG(cerr, L"일반 아이템 판매후, 전체GP가 현재GP보다 적다.. LoginUID : " << dwLoginUID_)
					<< BUILD_LOG(m_kODBC.m_strLastQuery)
					<< BUILD_LOG(SellItemID)
					<< BUILD_LOG(nGamePoint_)
					<< BUILD_LOG(nDBGamePoint) << END_LOG;

				// 현재값보다 적을 경우, 현재값만큼 GP 복구한다
				int nOK = -99;
				int nCurrentGP = 0;
				UpdateUserGamePoint(dwLoginUID_, cCharType_, dwSlotNum_, nGamePoint_ - nDBGamePoint, nOK, nCurrentGP);
				if (nOK == 0) {
					if (nCurrentGP != nGamePoint_) {
						START_LOG(cerr, L"GP 차이값 복구했는데도 DB랑 차이가 있음..LoginUID : " << dwLoginUID_)
							<< BUILD_LOG(SellItemID)
							<< BUILD_LOG(nGamePoint_)
							<< BUILD_LOG(nDBGamePoint)
							<< BUILD_LOG(nCurrentGP)
							<< END_LOG;
					}
					nDBGamePoint = nCurrentGP;
				}
			}

			nGamePoint_ = nDBGamePoint;
		}

		vecSellSucceed_.push_back(vit->m_ItemUID);
		// 대기 아이템 정보가 유효 하다면 그것도 가져 온다.
		if (dwWaitUID != KItem::INVALID_ITEM) {
			std::vector<KItem> vecRewardItem;
			std::vector<KItem>::iterator vit1;
			LIF(m_kHelper.InsertSingleWaitItemToInvenOrVDepot(dwLoginUID_, cCharType_, dwWaitUID, KItemManager::EWIT_ITEM_SELL, vecRewardItem));
			for (vit1 = vecRewardItem.begin(); vit1 != vecRewardItem.end(); ++vit1) {
				std::map<GCITEMUID, KItem>::iterator mit;
				mit = mapRewardItem_.find(vit1->m_ItemUID);
				if (mit == mapRewardItem_.end()) {
					mapRewardItem_.insert(std::map<GCITEMUID, KItem>::value_type(vit1->m_ItemUID, *vit1));
				}
				else {
					mit->second.m_nCount += vit1->m_nCount;
				}
			}
		}

		++vit;
	}
}

bool KGSDBThread::UpdateBossDungeonStat(IN std::vector<KBossDungeonStat> & vecBossDungeonStat_)
{
	/*
	mk8253. 20100803.

	L"{ call dbo.BDSTBossDoor_merge ( %d, %d, %d ) }"

	{ call dbo.BDSTBossDoor_merge ( @1, @2, @3 ) }
	@1 ; @iModeID_    [int]
	@2 ; @iDifficult_ [int]
	@3 ; @iType_      [int]

	1 return ( @1 )
	@1 ; OK
	0         ; 성공
	-101이하  ; 트랜젝션 에러
	*/
	int nOK = -99;
	std::vector<KBossDungeonStat>::iterator vit;
	for (vit = vecBossDungeonStat_.begin(); vit != vecBossDungeonStat_.end(); ++vit) {
		DBJIF2(m_kODBCStat.Query(L"{ call dbo.BDSTBossDoor_merge ( %d, %d, %d, ) }",
			vit->m_dwDungeonID,
			vit->m_dwDifficult,
			vit->m_dwClearType));

		if (m_kODBCStat.BeginFetch()) {
			VERIFY_QUERY(m_kODBCStat >> nOK);
			m_kODBCStat.EndFetch();
		}// end if

		_LOG_SUCCESS(nOK == 0, L"nOK : " << nOK)
			<< BUILD_LOG(m_kODBCStat.m_strLastQuery) << END_LOG;
	}

	return true;
}

bool KGSDBThread::CompleteMissionInfo(IN const DWORD dwUID_, IN const char& cCharType_, IN const DWORD dwMissionID_, IN bool bUnique_, IN const int nDeleteType_, OUT int& nRet_)
{
	if (false == RemoveMission(dwUID_, cCharType_, dwMissionID_, nDeleteType_)) {
		nRet_ = NetError::GetLastNetError();
		return false;
	}

	if (true == bUnique_) {
		if (false == PromotionCompleteMission(dwUID_, cCharType_, dwMissionID_, nRet_)) {
			nRet_ = NetError::GetLastNetError();
			return false;
		}
	}

	return true;
}

bool KGSDBThread::UpdateCharPromotion(IN std::wstring strLogin_, IN const char cCharType_, IN const char cCharPromotion_, OUT int& nRet_)
{
	char cOK = -99;
	char m_cPromotionLevel = -99;

	DBJIF2(m_kODBC.Query(L"{ call dbo.up_character_promote ( N'%s', %d, %d ) }",
		strLogin_.c_str(),
		cCharType_,
		cCharPromotion_))

		if (m_kODBC.BeginFetch()) {
			VERIFY_QUERY(m_kODBC >> cOK);

			if (0 == cOK) // 성공적으로 전직된 경우, 전직된 단계를 추가로 읽어낸다.
			{
				VERIFY_QUERY(m_kODBC >> m_cPromotionLevel);
			}
			m_kODBC.EndFetch();
		}

	LOG_SUCCESS(cOK == 0)
		<< BUILD_LOG(strLogin_)
		<< BUILD_LOGc(cCharType_)
		<< BUILD_LOGc(m_cPromotionLevel)
		<< BUILD_LOGc(cOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return true;
}

bool KGSDBThread::PromotionCompleteMission(IN const DWORD dwUID_, IN const char& cCharType_, IN const DWORD dwMissionID_, OUT int& nRet_)
{
	/*
	L"{ call dbo.MSGAMissionSystemTag_insert_20130527 ( %d, %d, %d ) }"

	{ call dbo.MSGAMissionSystemTag_insert_20130527 ( @01, @02, @03 ) }
	@01iLoginUID_input int
   ,@02iCharType_input int
   ,@03iMainID_input   int

	1 return ( @01 )
	@01: OK int
	0        : 성공
	-1       : 유저 정보 없음
	-2       : 캐릭터 정보 없음
	-3       : 이미 완료된 미션
	-801~-899: 무시가능 에러
	-901~-999: 치명적인 에러
	*/

	int nOK = -99;
	DBJIF2(m_kODBC.Query(L"{ call dbo.MSGAMissionSystemTag_insert_20130527 ( %d, %d, %d ) }",
		dwUID_, cCharType_, dwMissionID_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();
	}

	LIF(m_kHelper.IgnorableErrorProcessingAfterCallSP(nOK));

	if (0 != nOK) {
		switch (nOK)
		{
		case 0: SET_ERROR(NET_OK); break;
		case -1: SET_ERROR(ERR_MISSION_00); break;
		case -2: SET_ERROR(ERR_MISSION_12); break;
		case -3: SET_ERROR(ERR_MISSION_02); break;
		default:
			START_LOG(cerr, L"미션 완료 요청중 트랜젝션 에러 발생, UserUID : " << dwUID_)
				<< BUILD_LOG(nOK)
				<< BUILD_LOG(dwMissionID_)
				<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
			SET_ERROR(ERR_MISSION_21);
		}
		// 실패 이유를 알기 위해서.
		nRet_ = NetError::GetLastNetError();

		return false;
	}

	return true;
}

void KGSDBThread::GetMainMissionInfo(IN const DWORD dwUID_, IN const char& cCharType_, OUT std::vector<KMissionData> & vecMissionSlot_)
{
	/*
	L"{ call dbo.MSGAMissionSystemMain_select_20130527 ( %d, %d ) }"

	{ call dbo.MSGAMissionSystemMain_select_20130527 ( @01, @02 ) }
	@01iLoginUID_input int
   ,@02iCharType_input int

	n return ( @01, @02, @03, @04 )
	@01: MainID       int
	@02: RegDateA     datetime
	@03: ExpiredDateA datetime
	@04: isdeletion  bit
	0 ; 유효
	1 ; 만기됨
	*/

	std::set<DWORD> setDelMissionID;
	std::set<DWORD>::iterator sit;
	setDelMissionID.clear();

	DBJIF(m_kODBC.Query(L"{ call dbo.MSGAMissionSystemMain_select_20130527 ( %d, %d ) }",
		dwUID_, cCharType_));
	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		bool bDel = false;
		KMissionData kMissionSlot;
		VERIFY_QUERY(m_kODBC >> kMissionSlot.m_dwMissionID
			>> kMissionSlot.m_tmRegDate
			>> kMissionSlot.m_tmEndDate
			>> bDel);

		if (bDel) {
			setDelMissionID.insert(kMissionSlot.m_dwMissionID);
			continue;
		}

		if (!SiKMissionManager()->IsInMissions(kMissionSlot.m_dwMissionID))
		{
			START_LOG(cerr, L"DB에서 가져온 미션이 스크립트에 없음")
				<< BUILD_LOG(dwUID_)
				<< BUILD_LOG(kMissionSlot.m_dwMissionID)
				<< END_LOG;
			continue;
		}

		vecMissionSlot_.push_back(kMissionSlot);
	}

	if (!setDelMissionID.empty()) {
		for (sit = setDelMissionID.begin(); sit != setDelMissionID.end(); ++sit) {
			LIF(RemoveMission(dwUID_, cCharType_, *sit, KMissionInfo::DEL_TYPE_EXPIRY_DEL));
		}
	}
}

bool KGSDBThread::UpdateLoadingInfoStat(IN int nDateType_, IN int nLoadingType_, IN int nDateID_, IN int nCount_)
{
	/*
	L"{ call dbo.GLCOGameLoading_merge ( %d, %d, %d, %d ) }"

	{ call dbo.GLCOGameLoading_merge ( @1, @2, @3, @4 ) }
	@1 ; @iDataType_    int
	1 ; 시간
	2 ; 맵
	3 ; 방인원수
	@2 ; @iLoadingType_ int
	1 ; 게임 시작시 로딩 성공
	2 ; 게임 시작시 로딩 실패
	3 ; 스테이지 변환시 로딩 성공
	4 ; 스테이지 변환시 로딩 실패
	@3 ; @iDataID_      int
	DataType이 시간인 경우 초
	DataType이 맵인 경우 던전ID
	DataType이 방인원수면 사람수
	@4 ; @iCNT_         int
	해당 조건의 카운트

	1 return ( @1 )
	@1 ; OK int
	0        ; 성공
	-101이하 ; 트랜잭션 에러
	*/
	int nOK = -99;
	DBJIF2(m_kODBCStat.Query(L"{ call dbo.GLCOGameLoading_merge ( %d, %d, %d, %d ) }",
		nDateType_,
		nLoadingType_,
		nDateID_,
		nCount_));

	if (m_kODBCStat.BeginFetch()) {
		VERIFY_QUERY(m_kODBCStat >> nOK);
		m_kODBCStat.EndFetch();
	}// end if

	_LOG_SUCCESS(nOK == 0, L"nOK : " << nOK)
		<< BUILD_LOG(m_kODBCStat.m_strLastQuery) << END_LOG;

	return true;
}

void KGSDBThread::GetUserGawibawiboInfo(IN const DWORD dwUID_, IN DWORD dwVersion_, OUT KUserGawibawiboInfo & kUserGawibawibo_)
{
	/*mk8253. 20100928.
	가위바위보 말판 정보.

	L"{ call dbo.GPGAGawibawiboPlay_select ( %d, %d ) }"

	{ call dbo.GPGAGawibawiboPlay_select ( @1, @2 ) }
	@1 ; @iEventUID_     [int]
	@2 ; @iLoginUID_     [int]

	1 return ( @1, @2, @3, @4, @5, @6, @7)
	@1 ; TurnCount    -- 바퀴 수
	@2 ; Position     -- 현재 도전중인 보스몬스터
	@3 ; PlayCount    -- 플레이 횟수
	@4 ; TryCount     -- 도전가능 횟수
	@5 ; RetryCount    -- 재도전가능 횟수
	@6 ; ReceiveCount -- 오늘 받아간 코인 수
	@7 ; RestSec      -- 코인받기까지 남은 시간
	*/
	DBJIF(m_kODBC.Query(L"{ call dbo.GPGAGawibawiboPlay_select ( %d, %d ) }",
		dwVersion_, dwUID_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> kUserGawibawibo_.m_dwTurnCount
			>> kUserGawibawibo_.m_dwCurrentPosition
			>> kUserGawibawibo_.m_dwPlayCount
			>> kUserGawibawibo_.m_dwTryPoint
			>> kUserGawibawibo_.m_dwRetryPoint
			>> kUserGawibawibo_.m_usReceiveCount
			>> kUserGawibawibo_.m_dwRemainTime);

		m_kODBC.EndFetch();
	}

}

int KGSDBThread::CheckGawibawiboInit(IN const DWORD dwUID_)
{
	/*mk8253. 20100928.
	오늘 초기화 기록이 있는지 체크
	L"{ call dbo.GPGAGawibawiboPlay_check ( %d ) }"
	{ call dbo.GPGAGawibawiboPlay_check ( @1 ) }
	@1 ; @iLoginUID_ int

	1 return ( @1 )
	@1 ; Data */
	int nData = 0;
	DBJIF2(m_kODBC.Query(L"{ call dbo.GPGAGawibawiboPlay_check ( %d ) }",
		dwUID_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nData);
		m_kODBC.EndFetch();
	}
	return nData;
}


void KGSDBThread::InitGawibawiboPlayLog(IN const DWORD dwUID_)
{
	/* mk8253. 20100928.
	오늘 초기화 기록 추가
	L"{ call dbo.GPGAGawibawiboPlayInit_insert ( %d ) }"

	{ call dbo.GPGAGawibawiboPlayInit_insert ( @1 )
	@1 ; @iLoginUID_ int

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 유저 정보가 없음
	-101이하 ; 트랜젝션 에러
	*/
	int nOK = -99;
	DBJIF(m_kODBC.Query(L"{ call dbo.GPGAGawibawiboPlayInit_insert ( %d ) }",
		dwUID_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();
	if (nOK != 0) {
		START_LOG(cerr, L"가위바위보 초기화 기록 남기기 실패. ")
			<< BUILD_LOG(nOK)
			<< BUILD_LOG(dwUID_)
			<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
	}
}

void KGSDBThread::GawibawiboItemInsertLog(IN DWORD dwUID_, IN GCITEMID dwItemID_, IN int nDuration_, IN int nPeriod_, IN DWORD dwVersion_)
{
	/* mk8253. 20100928.
	아이템 획득 로그
	L"{ call dbo.GPGAGawibawiboPlay_item_insert ( %d, %d, %d, %d, %d ) }"
	{ call dbo.GPGAGawibawiboPlay_item_insert ( @1, @2, @3, @4, @5 ) }
	@1 ; @iEventUID_ [int]
	@2 ; @iLoginUID_ [int]
	@3 ; @iItemID_   [int]
	@4 ; @iPeriod_   [smallint]
	@5 ; @iDuration_ [smallint]

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 유저 정보가 없음
	-101이하 ; 트랜젝션 에러
	*/
	DBJIF(m_kODBC.Query(L"{ call dbo.GPGAGawibawiboPlay_item_insert ( %d, %d, %d, %d, %d ) }",
		dwVersion_, dwUID_, dwItemID_, nDuration_, nPeriod_));

	int nOK = -99;
	if (m_kODBC.BeginFetch())
	{
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	LOG_SUCCESS(nOK == 0)
		<< BUILD_LOG(nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
}

int KGSDBThread::UpdateUserGawibawiboInfo(IN DWORD dwUID_, IN KUserGawibawiboInfo kUserGawibawiboInfo_)
{
	/*
	mk8253. 201010014.
	보드게임 정보 변경

	L"{ call dbo.GPGAGawibawiboPlay_update ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }"
	{ call dbo.GPGAGawibawiboPlay_update ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10 ) }
	@1  ; @iEventUID_     [int]
	@2  ; @iLoginUID_     [int]
	@3  ; @iResult_       [tinyint]
	@4  ; @iTurnCount_    [tinyint]
	@5  ; @iPosition_     [tinyint]
	@6  ; @iPlayCount_    [smallint]
	@7  ; @iTryCount_     [smallint]
	@8  ; @iRetryCoint_   [smallint]
	@9  ; @iReceiveCount_ [smallint]
	@10 ; @iRestSec_      [smallint]

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 유저 정보가 없음
	-101이하 ; 트랜젝션 에러

	*/

	int nOK = -99;

	DBJIF3(m_kODBC.Query(L"{ call dbo.GPGAGawibawiboPlay_update ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }",
		kUserGawibawiboInfo_.m_usVersion,
		dwUID_,
		kUserGawibawiboInfo_.m_usResult,
		kUserGawibawiboInfo_.m_dwTurnCount,
		kUserGawibawiboInfo_.m_dwCurrentPosition,
		kUserGawibawiboInfo_.m_dwPlayCount,
		kUserGawibawiboInfo_.m_dwTryPoint,
		kUserGawibawiboInfo_.m_dwRetryPoint,
		kUserGawibawiboInfo_.m_usReceiveCount,
		kUserGawibawiboInfo_.m_dwRemainTime
	), return nOK);

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	// 로그성 데이터이므로 에러 처리를 진행하진 않고
	// 실패한 부분에 대해서 로그는 남기자.
	if (nOK != 0) {
		START_LOG(cerr, L"가위바위보 로그 남기기 실패. ")
			<< BUILD_LOG(nOK)
			<< BUILD_LOG(dwUID_)
			<< BUILD_LOG(kUserGawibawiboInfo_.m_dwTurnCount)
			<< BUILD_LOG(kUserGawibawiboInfo_.m_dwCurrentPosition)
			<< BUILD_LOG(kUserGawibawiboInfo_.m_usReceiveCount)
			<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
	}

	return nOK;
}

void KGSDBThread::InsertPlayTimeStat(IN const DWORD dwUID_, IN const DWORD dwDTTime_, IN DWORD dwPTTime_)
{
	/*
	L"{ call dbo.DPGADurationPlayTime_merge_20101102 ( %d, %d, %d ) }"

	{ call dbo.DPGADurationPlayTime_merge_20101102 ( @1, @2, @3 ) }
	@1 ; @iLoginUID_     int
	@2 ; @iDurationTime_ int
	@3 ; @iPlayTime_     int

	1 select ( @1 )
	@1 ; OK int
	0        ; 성공
	-1       ; 유저UID가 없음
	-101이하 ; 트랜잭션 에러
	*/

	// DB에 인자값을 분으로 넣기 위해서 보정.
	dwPTTime_ = std::max<DWORD>(0, dwPTTime_ / (60 * 1000));
	DBJIF(m_kODBC.Query(L"{ call dbo.DPGADurationPlayTime_merge_20101102 ( %d, %d, %d ) }",
		dwUID_, dwDTTime_, dwPTTime_));

	int nOK = -99;
	if (m_kODBC.BeginFetch())
	{
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	LOG_SUCCESS(nOK == 0)
		<< BUILD_LOG(nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
}

void KGSDBThread::SelectSocksInfo(IN const DWORD dwUID_,
	IN const GCITEMID ItemID_,
	OUT int& nIsHangup_,
	OUT DWORD & dwRemainSec_)
{
	/*    mk8253. 20101122.
	양말 정보.
	L"{ call dbo.CMGAChristmas_201010_select ( %d, %d ) }"
	{ call dbo.CMGAChristmas_201010_select ( @1, @2 ) }
	@1 ; @iLoginUID_  [int]
	@2 ; @iSocksKind_ [int]

	1 return ( @1, @2 )
	@1 ; IsHangup    0이면 안걸려있음 1이면 걸려있고
	@2 ; RemainSec   RemainSec만큼 시간이 남음    */

	DBJIF(m_kODBC.Query(L"{ call dbo.CMGAChristmas_201010_select ( %d, %d ) }",
		dwUID_, ItemID_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nIsHangup_ >> dwRemainSec_);
	}
	m_kODBC.EndFetch();

	START_LOG(clog, L"양말 정보 가져오기 UserUID: " << dwUID_)
		<< BUILD_LOG(ItemID_)
		<< BUILD_LOG(nIsHangup_)
		<< BUILD_LOG(dwRemainSec_) << END_LOG;
}

int KGSDBThread::SocksHangup(IN const DWORD dwUID_, IN const GCITEMID ItemID_, IN const DWORD dwRemainSec_)
{
	// 클라이언트 종료시에  걸려있는 양말에 한해서 아래 sp 로 남은 시간을 수정한다
	/*    mk8253. 20101122.
	크리스마스 이벤트 양말 걸기.
	L"{ call dbo.CMGAChristmas_201010_Hangup ( %d, %d, %d ) }"

	{ call dbo.CMGAChristmas_201010_Hangup ( @1, @2 ) }
	@1 ; @iLoginUID_  int 누가
	@2 ; @iSocksKind_ int 어떤양말을
	@3 ; @iRemainSec_ int 남은시간
	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 유저 정보가 없음
	-101이하 ; 트랜젝션 에러  */

	int nOK = -99;

	DBJIF3(m_kODBC.Query(L"{ call dbo.CMGAChristmas_201010_Hangup ( %d, %d, %d ) }",
		dwUID_, ItemID_, dwRemainSec_), return nOK);

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}

	m_kODBC.EndFetch();

	LOG_SUCCESS(nOK == 0)
		<< BUILD_LOG(nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return nOK;
}

int KGSDBThread::SocksCollect(IN const DWORD dwUID_, IN const GCITEMID ItemID_, IN const DWORD dwRemainSec_)
{
	/* mk8253. 20101122.
	크리스마스 이벤트 양말 수거.

	L"{ call dbo.CMGAChristmas_201010_Collect ( %d, %d ) }"

	{ call dbo.CMGAChristmas_201010_Collect ( @1, @2 ) }
	@1 ; @iLoginUID_  [int]
	@2 ; @iSocksKind_ [int]

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 유저 정보가 없음
	-101이하 ; 트랜젝션 에러  */
	int nOK = -99;

	DBJIF3(m_kODBC.Query(L"{ call dbo.CMGAChristmas_201010_Collect ( %d, %d ) }",
		dwUID_, ItemID_), return nOK);

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	LOG_SUCCESS(nOK == 0)
		<< BUILD_LOG(nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return nOK;
}

void KGSDBThread::SetSkillInfo(IN DWORD dwUID_, IN OUT std::map<char, KCharacterInfo> & mapCharacterInfo_)
{
	// mapCharacterInfo_ 에 이미 초기화 되어 있음..
	//--유저의 캐릭터별 스킬 기본 정보
	std::map<char, KCharacterInfo>::iterator mit;
	for (mit = mapCharacterInfo_.begin(); mit != mapCharacterInfo_.end(); ++mit) {
		// 캐릭터 현재 레벨의 SPPoint 가져오기.
		SiKSkillManager()->GetSkillPointInfo(mit->first, mit->second.m_cPromotion, mit->second.m_dwLevel, mit->second.m_kSkillInfo.m_nLvSPPoint);
		// 해당 캐릭터 최대 SPPoint 가져오기.
		SiKSkillManager()->GetMaxSPX(mit->first, mit->second.m_cPromotion, mit->second.m_kSkillInfo.m_nMaxSPPoint);
	}
}

void KGSDBThread::InitChangeWeapon(IN const std::map<char, KCharacterInfo> & mapCharInfo_, OUT std::map<char, std::pair<bool, GCITEMUID> > & mapWeaponInfo)
{
	std::map<char, KCharacterInfo>::const_iterator cmit;
	for (cmit = mapCharInfo_.begin(); cmit != mapCharInfo_.end(); ++cmit) {
		std::pair<bool, GCITEMUID> prTemp = std::make_pair(false, 0);
		mapWeaponInfo[cmit->first] = prTemp;
	}
}

bool KGSDBThread::CheckClientChecksum(IN const KDB_EVENT_VERIFY_ACCOUNT_REQ & kReq_)
{
	KSerBuffer  buff;   // goto를 사용하기 위해 변수 선언을 앞으로 당겼다.

	// 051027. florist. 클라이언트 실행파일 checksum이 올바른지 확인한다. 사내테섭은 그냥 통과.
	if (SiKGSSimLayer()->CheckTypeFlag(ST_INTERNAL_TESOP) ||
		SiKGameServer()->IsRegisteredChecksum(kReq_.m_kPacket.m_dwChecksum)) {
		return true;
	}

	// 이미 무효한 체크섬으로 등록되어있는 경우
	if (SiKGameServer()->IsRegisteredInvalidChecksum(kReq_.m_kPacket.m_dwChecksum)) {
		goto invalid_checksum;
	}

	// 유효/무효 리스트 어느쪽에도 등록되어있지 않다면 DB에 물어본다.
	DBJIF2(m_kODBC.Query(L"{ call dbo.sp_executesql ( "
		L"N'select [CheckSum] from dbo.ClientCheckSum with (nolock) where ProtocolVer = @1 and CheckSum = @2 option (maxdop 1)', "
		L"N'@1 int, @2 binary(4)', %d, 0x%08x ) }",
		SiKGameServer()->m_nProtocolVer, kReq_.m_kPacket.m_dwChecksum));

	if (!m_kODBC.BeginFetch()) {  // 얻어온 내용이 없음 - 잘못된 checksum.
		goto invalid_checksum;
	}
	// 쿼리 성공했으면 문의한 체크섬은 성공한 것이다.
	VERIFY_QUERY(m_kODBC >> buff);
	m_kODBC.EndFetch();

	SiKGameServer()->AddChecksum(kReq_.m_kPacket.m_dwChecksum);

	return true;

invalid_checksum:
	START_LOG(cerr, L"invalid checksum : " << kReq_.m_kPacket.m_dwChecksum)
		<< BUILD_LOG(KncUtil::toWideString(kReq_.m_kPacket.m_strLogin))
		<< END_LOG;

	SiKGameServer()->AddInvalidChecksum(kReq_.m_kPacket.m_dwChecksum, kReq_.m_kPacket.m_strLogin);
	return false;
}

char KGSDBThread::CheckPCBangUser(IN const std::wstring & strIP_)
{
	char cRet = KIPAdvantage::PCBANG_NONE;

	std::vector< std::pair<std::string, int> > vecODBCKey;

	SiKIPAdvantage()->GetODBCKey(vecODBCKey);

	if (SiKIPAdvantage()->CheckTestIP(strIP_) == true) {
		cRet = KIPAdvantage::PCBANG_PUBLISHER;
		return cRet;
	}

	__JIF(!vecODBCKey.empty(), return cRet);
	//if ( vecODBCKey.empty() ) { return cRet; }

	std::vector< std::pair<std::string, int> >::iterator vit;

	for (vit = vecODBCKey.begin(); vit != vecODBCKey.end(); ++vit) {
		KODBC* pkODBCPCBang = SiKGSDBLayer()->GetODBC(vit->first.c_str());

		if (pkODBCPCBang == NULL) { continue; }

		if (m_kHelper.QueryPCBangUser(strIP_, *pkODBCPCBang)) {
			cRet = (char)vit->second;
			break;
		}
	}

	START_LOG(clog, L"Check PCBang User.. IP : " << strIP_ << L",type : " << (int)cRet) << END_LOG;
	return cRet;
}

char KGSDBThread::CheckTH3BBUser(IN const std::wstring & strIP_, IN const DWORD dwUserType_)
{
	char cRet = KIPAdvantage::PCBANG_NONE;

	std::vector< std::pair<std::string, int> > vecODBCKey;

	SiKIPAdvantage()->GetODBCKey(vecODBCKey);

	if (SiKIPAdvantage()->CheckTestIP(strIP_) == true) {
		cRet = (char)dwUserType_;
		return cRet;
	}

	if (vecODBCKey.empty()) { return cRet; }

	std::vector< std::pair<std::string, int> >::iterator vit;

	for (vit = vecODBCKey.begin(); vit != vecODBCKey.end(); ++vit) {
		KODBC* pkODBCPCBang = SiKGSDBLayer()->GetODBC(vit->first.c_str());

		if (pkODBCPCBang == NULL) { continue; }

		if (vit->second == dwUserType_) {
			if (m_kHelper.QueryUserBenfitCheck(strIP_, *pkODBCPCBang)) {
				cRet = (char)vit->second;
				break;
			}
		}
	}

	return cRet;
}

void KGSDBThread::UpdateIndigoRank()
{
	/*
	20081114. microcat.

	{ call dbo.gii_indigo_island_rank ( %d ) }

	{ call dbo.gii_indigo_island_rank ( @1 ) }
	@1 ; Rank int ; 255를 초과할 경우 255로 고정됨

	n return ( @1, @2, @3, @4, @5, @6, @7, @8 )
	@1 ; Rank tinyint
	@2 ; Nick nvarchar(24)
	@3 ; CharType tinyint
	@4 ; RP int
	@5 ; Win int
	@6 ; Lose int
	@7 ; WinRate float
	@8 ; Change int
	*/

	std::vector<KIndigoRankInfo>    vecIndigoRank;
	DBJIF(m_kODBC.Query(L"{ call dbo.gii_indigo_island_rank ( 100 ) }"));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		KIndigoRankInfo kInfo;
		VERIFY_QUERY(m_kODBC >> kInfo.m_nRank
			>> kInfo.m_strNick
			>> kInfo.m_cCharType
			>> kInfo.m_nRP
			>> kInfo.m_nWin
			>> kInfo.m_nLose
			>> kInfo.m_fWinRate
			>> kInfo.m_nChange);

		vecIndigoRank.push_back(kInfo);
	}
	JIF(!vecIndigoRank.empty());

	KEventPtr spEvent(new KUserEvent);
	spEvent->m_usEventID = KUserEvent::EVENT_INDIGO_RANK_NOT;
	KSerializer         ks;

	ks.BeginWriting(&spEvent->m_kbuff);
	ks.Put(vecIndigoRank);
	ks.EndWriting();

	spEvent->m_kbuff.Compress();

	//KLocker lock( m_csIndigoRank );
	KSerBuffer serBuffIndigoRank;
	serBuffIndigoRank.Clear();
	ks.BeginWriting(&serBuffIndigoRank);
	ks.Put(*spEvent);
	ks.EndWriting();

	SiKGCHelper()->SetIndigoRank(serBuffIndigoRank);
}

void KGSDBThread::SyncChangeWeapon(IN const std::vector<KItem> & vecInven_, IN const std::map< char, std::pair<bool, GCITEMUID> > & mapChangeWeapon_, IN OUT std::map<char, KCharacterInfo> & mapCharacterInfo_)
{
	std::map< char, std::pair<bool, GCITEMUID> >::const_iterator cmitEquip;
	for (cmitEquip = mapChangeWeapon_.begin(); cmitEquip != mapChangeWeapon_.end(); ++cmitEquip) {

		// ItemUID로 인벤토리내 Item정보 찾기
		std::vector<KItem>::const_iterator cvit;
		cvit = std::find_if(vecInven_.begin(), vecInven_.end(),
			boost::bind(&KItem::m_ItemUID, _1) == cmitEquip->second.second);

		std::map<char, KCharacterInfo>::iterator mitChar;
		// 캐릭터 타입 체크
		mitChar = mapCharacterInfo_.find(cmitEquip->first);
		if (mitChar == mapCharacterInfo_.end()) continue;

		if (cvit != vecInven_.end()) {
			mitChar->second.m_kChangeWeaponItem.m_dwID = cvit->m_ItemID;
			mitChar->second.m_kChangeWeaponItem.m_dwUID = cvit->m_ItemUID;
			mitChar->second.m_kChangeWeaponItem.m_cEnchantLevel = cvit->m_cEnchantLevel;
			mitChar->second.m_kChangeWeaponItem.m_cGradeID = cvit->m_cGradeID;
			mitChar->second.m_kChangeWeaponItem.m_nEquipLevel = static_cast<int>(cvit->m_sEquipLevel);
			mitChar->second.m_kChangeWeaponItem.m_vecSocket = cvit->m_vecSocket;
			mitChar->second.m_kChangeWeaponItem.m_vecAttribute = cvit->m_vecAttribute;
			mitChar->second.m_kChangeWeaponItem.m_cEnchantEquipGradeID = cvit->m_cEnchantEquipGradeID;
		}
		else {
			mitChar->second.m_kChangeWeaponItem.m_dwUID = cmitEquip->second.second;
			mitChar->second.m_bChangeWeaponLock = cmitEquip->second.first;
		}
	}
}

void KGSDBThread::CheckFreeReceiveCount(IN OUT DWORD & dwGiveCount)
{
	CTime tmCurrent = CTime::GetCurrentTime();
	DWORD dwInitHour = SiKDicePlay()->GetInitHour();
	DWORD dwDailyFree = SiKDicePlay()->GetDailyFree();

	// 초기화 시간에 주사위 무료 지급 갯수가 DB저장된 값이 0이다.
	// 해당 부분에 대한 보정처리를 진행한다.
	if (tmCurrent.GetHour() == dwInitHour) {
		if (dwGiveCount == 0) {
			dwGiveCount = dwDailyFree;
		}
	}
}

void KGSDBThread::InitDiceDailyFree(IN const int dwEventType_)
{
	/*
	mk8253. 20110110.
		각종 이벤트들 시간 초기화.

	L"{ call dbo.ETGAEventTime_Init ( %d, %d, %d ) }"

	{ call dbo.ETGAEventTime_Init ( @1, @2, @3 ) }
	@1 ; @iEventType_ [int] 이벤트 타입( 주사위 이벤트, 가위바위보 )
	@2 ; @iVersion_   [int] 주사위 이벤트 버전 정보.
	@3 ; @iSetCount_  [int] 무료 지급 횟수.
	1 return ( @1 )
		@1 ; OK
		0        ; 성공
		-101이하 ; 트랜젝션 에러
	*/

	DWORD dwVersion = SiKDicePlay()->GetDiceEventVersion();
	DWORD dwDailyFree = SiKDicePlay()->GetDailyFree();

	DBJIF(m_kODBC.Query(L"{ call dbo.ETGAEventTime_Init ( %d, %d, %d ) }",
		dwEventType_, dwVersion, dwDailyFree));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	START_LOG(clog, L"주사위 무료 지급횟수 초기화 ")
		<< BUILD_LOG(dwEventType_)
		<< BUILD_LOG(dwVersion)
		<< BUILD_LOG(dwDailyFree) << END_LOG;
}

void KGSDBThread::SetRandomDiceNum(IN const DWORD & dwRecvUserUID_, IN const std::vector<DWORD> & vecUserUID_, OUT std::map<DWORD, int> & mapUserDice_)
{
	std::set<int> setDice;
	setDice.clear();
	mapUserDice_.clear();

	if (vecUserUID_.empty()) return;

	// 주사위값 결정
	while (setDice.size() < vecUserUID_.size()) {
		int nNum = (rand() % 100) + 1;
		setDice.insert(nNum);
	}

	JIF(!setDice.empty());

	std::set<int>::iterator sit;
	sit = setDice.begin();

	// 유저별 대입
	std::vector<DWORD>::const_iterator cvit;
	for (cvit = vecUserUID_.begin(); cvit != vecUserUID_.end(); ++cvit) {
		const DWORD& dwUserUID = *cvit;

		// 실제 아이템 받을 유저의 주사위 설정은 마지막에 한다.
		if (dwUserUID == dwRecvUserUID_) {
			continue;
		}

		mapUserDice_[dwUserUID] = *sit;

		if (sit != setDice.end()) {
			++sit;
		}
	}

	// 실유저는 가장 큰 주사위값 대입
	mapUserDice_[dwRecvUserUID_] = *setDice.rbegin();
}

void KGSDBThread::GetPlantTreePosition(IN const DWORD & dwUID_, IN const DWORD & dwVersion_, OUT DWORD & dwContinentType_, OUT DWORD & dwCurrentPosition_, OUT DWORD & dwTreeStatus_)
{
	/*
	L"{ call dbo.NNGANamuNamu_select_20110208 ( %d, %d )  }"

	{ call dbo.NNGANamuNamu_select_20110208 ( @1, @2 ) }
	@1: @iLoginUID_ int
	@2: @iEventNo_  int
		차후 새롭게 진행시 해당 번호가 바뀌어야 한다

	n return ( @1, @2, @3 )
	@1: ContinentID int 대륙번호
	1번부터 시작
	@2: NamuNo      int 최종나무번호
	0번부터 시작
	@3: Status      int 상태
	0    : 나무를 심을 수 있는 상태
	1    : 애기나무(물 준적없음)
	11~99: 애기나무(물을 1회이상 주었음)
	100  : 완소나무(열매열렸음)
	200  : 황금나무(열매열렸음)
	300  : 죽었음
	*/

	DBJIF(m_kODBC.Query(L"{ call dbo.NNGANamuNamu_select_20110208 ( %d, %d ) }",
		dwUID_, dwVersion_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> dwContinentType_
			>> dwCurrentPosition_
			>> dwTreeStatus_);
	}
	m_kODBC.EndFetch();

	START_LOG(clog, L"나무나무 유저 정보 가져오기 UserUID: " << dwUID_)
		<< BUILD_LOG(dwContinentType_)
		<< BUILD_LOG(dwCurrentPosition_)
		<< BUILD_LOG(dwTreeStatus_) << END_LOG;
}

int KGSDBThread::SetPlantTreePosition(IN const DWORD & dwUID_, IN const DWORD & dwVersion_, IN DWORD & dwContinentType_, IN DWORD & dwCurrentPosition_, IN DWORD & dwTreeStatus_)
{
	/*
	L"{ call dbo.NNGANamuNamu_merge_20110208 ( %d, %d, %d, %d, %d )  }"

	{ call dbo.NNGANamuNamu_merge_20110208 ( @1, @2, @3, @4, @5 ) }
	@1: @iLoginUID_    int
	@2: @iEventNo_     int
	@3: @iContinentID_ int
	@4: @iNamuNo_      int
	@5: @iStatus_      int

	1 return ( @1 )
	@1: OK int
	0       : 성공
	-1      : 유저정보가 없음
	-101이하: 트랜잭션 에러
	*/

	int nOK = -99;
	DBJIF3(m_kODBC.Query(L"{ call dbo.NNGANamuNamu_merge_20110208 ( %d, %d, %d, %d, %d ) }",
		dwUID_, dwVersion_, dwContinentType_, dwCurrentPosition_, dwTreeStatus_), return nOK);

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	LOG_SUCCESS(nOK == 0)
		<< BUILD_LOG(nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return nOK;
}

int KGSDBThread::CheckUserAuthCheck(IN const DWORD & dwType_,
	IN const std::wstring & wstrLogin_,
	IN const std::wstring & wstrNickName_,
	IN const std::wstring & wstrBirthDay_,
	IN const std::wstring & wstrEmail_)
{
	/*
		mk8253. 20110503.
		미국 접속 보상자 체크.

		L"{ call dbo.UUGAUSAUser_check ( N'%s', N'%s', N'%s', N'%s', %d ) }"

		{ call dbo.UUGAUSAUser_check ( @1, @2 ) }
		@1 ; @strAccount_  [nvarchar](24)
		@2 ; @strNickname_ [nvarchar](24)
		@3 ; @sdtBirthday_ [smalldatetime]
		@4 ; @strEmail_    [nvarchar](100)
		@5 ; @bRewardType_ [bit]
		0 : 아이템 보상자
		1 : 가상캐시 보상자

		1 return ( @1 )
		@1 ; @iVCash
		0        ; 대상아님
		1        ; 아이템 받을 유저
		2       ; 이미 받아간 유저
		10000이상; 지급할 가상캐시 금액
	*/

	int nOK = -99;
	DBJIF3(m_kODBC.Query(L"{ call dbo.UUGAUSAUser_check ( N'%s', N'%s', N'%s', N'%s', %d ) }",
		wstrLogin_.c_str(),
		wstrNickName_.c_str(),
		wstrBirthDay_.c_str(),
		wstrEmail_.c_str(),
		dwType_), return nOK);

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	LOG_SUCCESS(nOK == 0)
		<< BUILD_LOG(nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return nOK;
}

void KGSDBThread::InsertVirtualCashLog(IN const DWORD & dwUID_, IN const std::wstring & strAccount_, IN const int& nRewardType_)
{
	/*
	mk8253. 20110503.
	가상캐시 받아간 날짜를 기록.

	L"{ call dbo.VCGAVirtualCashEndDate_merge ( %d, N'%s' ) }"

	{ call dbo.VCGAVirtualCashEndDate_insert ( @1, @2, @3 ) }
	@1 ; @iLoginUID_   [int]
	@2 ; @strAccount_  [nvarchar](24)
	@3 ; @bRewardType_ [bit]

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 유저 정보가 없음
	-2       ; 미국쪽 유저 정보 없음
	-101이하 ; 트랜젝션 에러
	*/
	DBJIF(m_kODBC.Query(L"{ call dbo.VCGAVirtualCashEndDate_merge ( %d, N'%s', %d ) }",
		dwUID_, strAccount_.c_str(), nRewardType_)); // 가상캐시 받아간 로그 추가.

	if (m_kODBC.BeginFetch()) {
		int nOK = 0;
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();

		if (nOK != 0) {
			START_LOG(cerr, L"가상캐쉬 받아간 로그 추가 에러")
				<< BUILD_LOG(nOK)
				<< BUILD_LOG(m_kODBC.m_strLastQuery)
				<< END_LOG;
		}
	}
}

void KGSDBThread::UpdateVirtualCash(IN const DWORD & dwUID_, IN const DWORD & dwVirtualCash_)
{
	// 가상캐시 금액을 갱신한다.
	//{ call dbo.VCGA_virtual_cash_merge ( @1, @2 ) }
	//@1 ; @iLoginUID_ int
	//@2 ; @iVCPointDiff_ int
	//
	//1 return ( @1 )
	//@1 ; OK int
	//0        ; 성공
	//-1       ; 유저 정보가 없음
	//-101이하 ; 트랜잭션 에러
	DBJIF(m_kODBC.Query(L"{ call dbo.VCGA_virtual_cash_merge ( %d, %d ) }",
		dwUID_, dwVirtualCash_)); // 가상캐시 갱신.

	if (m_kODBC.BeginFetch()) {
		int nOK = 0;
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();

		if (nOK != 0) {
			START_LOG(cerr, L"가상캐쉬 쿼리 에러")
				<< BUILD_LOG(nOK)
				<< BUILD_LOG(m_kODBC.m_strLastQuery)
				<< END_LOG;
		}
	}
}

void KGSDBThread::GetVirtualCashLog(IN const DWORD & dwUID_, OUT time_t & tmVirtualEnableDate_)
{
	/*
	mk8253. 110503.
	가상캐시 사용 종료일 리턴.

	L"{ call dbo.VCGAVirtualCashEndDate_select ( %d ) }"

	{ call dbo.VCGAVirtualCashEndDate_select ( @1 ) }
	@1 ; @iLoginUID_ [int]

	1 return ( @1 )
	@1 ; EndDate
	*/
	DBJIF(m_kODBC.Query(L"{ call dbo.VCGAVirtualCashEndDate_select ( %d ) }",
		dwUID_)); // 가상캐시 사용 종료일 리턴.

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> tmVirtualEnableDate_);
		m_kODBC.EndFetch();
	}
}

int KGSDBThread::InsertGachaSetReward(IN const DWORD dwUID_, IN const GCITEMID dwGachaItemID_, IN const int nGachaVersion_, IN const DWORD dwType_)
{
	/*
	20090724. microcat.
	20110201. microcat.
	20110209. mk8253.

	L"{ call dbo.GGGA_game_game_user_insert ( %d, %d, %d, %d ) }"

	{ call dbo.GGGA_game_game_user_insert ( @1, @2, @3, @4 ) }
	@1 ; LoginUID_  int
	@2 ; Version_   int
	@3 ; ItemID int
	@4 ; Type_    tinyint = 200,

	1 return ( @1 )
	@1 ; OK int
	*/
	int nOK = -99;
	DBJIF3(m_kODBC.Query(L"{ call dbo.GGGA_game_game_user_insert ( %d, %d, %d, %d ) }",
		dwUID_, nGachaVersion_, dwGachaItemID_, dwType_), return nOK);
	if (m_kODBC.BeginFetch())
	{
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	LOG_SUCCESS(nOK == 0)
		<< BUILD_LOG(nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return nOK;
}

int KGSDBThread::InsertGachaSetReward(IN const DWORD dwUID_,
	IN const DWORD dwGachaItemID_,
	IN const int nGachaVersion_,
	IN const DWORD dwType_,
	IN const int nItemLevel_)
{
	/*
		L"{ call dbo.GGGAGameGameUser_insert ( %d, %d, %d, %d, %d ) }"
		가차 아이템 획득 로그

		{ call dbo.GGGAGameGameUser_insert ( @1, @2, @3, @4, @5 ) }
		@1 ; LoginUID_  int
		@2 ; Version_   int
		@3 ; ItemID_  int
		@4 ; Type_      tinyint = 200
		@5 ; ItemLevel_ tinyint = 1

		1 return ( @1 )
		@1 ; OK int
			0         ; 성공
			-1        ; 유저 정보가 없음
			-2        ; 이미 획득했음
			-101 이하 ; 트랜잭션 에러
	*/

	int nOK = -99;
	DBJIF3(m_kODBC.Query(L"{ call dbo.GGGAGameGameUser_insert ( %d, %d, %d, %d, %d ) }",
		dwUID_, nGachaVersion_, dwGachaItemID_, dwType_, nItemLevel_), return nOK);
	if (m_kODBC.BeginFetch())
	{
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	LOG_SUCCESS(nOK == 0)
		<< BUILD_LOG(nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return nOK;
}

bool KGSDBThread::CheckEventNewbieReward(IN KVipEvent & kInfo_, IN const int& nPoint_, IN const time_t & tmFirstLogin_, IN const std::map<int, bool> & mapRewardCheck_)
{
	if (false == kInfo_.IsEventEnable(KVipEventManager::NEWBIE_REWARD)) {
		START_LOG(clog, L"신규구매 이벤트 비활성화") << END_LOG;
		return false;
	}

	if (false == SiKVipEventManager()->IsNewbieUser(tmFirstLogin_, kInfo_.m_prEventRange)) {
		START_LOG(clog, L"신규유저가 아니다.")
			<< BUILD_LOGtm(CTime(tmFirstLogin_))
			<< BUILD_LOGtm(CTime(kInfo_.m_prEventRange.first))
			<< BUILD_LOGtm(CTime(kInfo_.m_prEventRange.second)) << END_LOG;
		return false;
	}

	if (nPoint_ <= 0) {
		START_LOG(clog, L"신규구매 보상 대상이 아니다.") << END_LOG;
		return false;
	}

	std::map<int, bool>::const_iterator cmit;
	cmit = mapRewardCheck_.find(KVipEventManager::NEWBIE_REWARD);
	if (cmit != mapRewardCheck_.end()) {

		if (true == cmit->second) {
			START_LOG(clog, L"이미 신규보상을 받았다.") << END_LOG;
			return false;
		}
	}

	return true;
}

bool KGSDBThread::CheckEventVipReward(IN KVipEvent & kInfo_, IN const int& nPoint_, IN const std::map<int, bool> & mapRewardCheck_, OUT std::set<USHORT> & setPayTableID_)
{
	if (false == kInfo_.IsEventEnable(KVipEventManager::VIP_REWARD)) {
		START_LOG(clog, L"VIP구매 이벤트 비활성화") << END_LOG;
		return false;
	}

	std::map<PAY_INDEX, PAIR_INT> mapPayTable;
	SiKVipEventManager()->GetPayTable(mapPayTable);

	// 금액별 체크
	std::map<int, bool>::const_iterator cmit;
	for (cmit = mapRewardCheck_.begin(); cmit != mapRewardCheck_.end(); ++cmit) {
		const int& nPayIndex = cmit->first;

		if (true == cmit->second) {
			START_LOG(clog, L"이미 보상을 받았다. Index : " << nPayIndex) << END_LOG;
			continue;
		}

		// 구매Index별 금액범위
		std::map<PAY_INDEX, PAIR_INT>::const_iterator cmitPay;
		cmitPay = mapPayTable.find((USHORT)nPayIndex);
		if (cmitPay == mapPayTable.end()) {
			START_LOG(cwarn, L"없는 구매금액Index : " << nPayIndex) << END_LOG;
			continue;
		}

		if (nPoint_ < cmitPay->second.first) {
			START_LOG(clog, L"사용한 구매금액이 보상범위보다 적다. Index : " << nPayIndex)
				<< BUILD_LOG(nPoint_)
				<< BUILD_LOG(cmitPay->second.first)
				<< BUILD_LOG(cmitPay->second.second) << END_LOG;
			continue;
		}

		// 보상 대상이 되는 금액ID를 넣어주자.
		setPayTableID_.insert(nPayIndex);
	}

	if (setPayTableID_.empty()) {
		START_LOG(clog, L"보상 대상이 아니다.") << END_LOG;
		return false;
	}

	return true;
}

void KGSDBThread::InsertVipEventRewardItemToDB(IN const DWORD & dwUserUID_, IN const char cCharType_, IN const USHORT & usVer_, IN const std::map<USHORT, std::vector<KDropItemInfo> > & mapReward_, OUT std::vector<KItem> & vecItem_)
{
	std::map<USHORT, std::vector<KDropItemInfo> >::const_iterator cmit;
	for (cmit = mapReward_.begin(); cmit != mapReward_.end(); ++cmit) {

		// 아이템 넣어주기.
		std::vector<KDropItemInfo>::const_iterator cvit;
		for (cvit = cmit->second.begin(); cvit != cmit->second.end(); ++cvit) {
			LIF(m_kHelper.InsertSingleItemJob(dwUserUID_, cCharType_, cvit->m_ItemID, cvit->m_nDuration, cvit->m_nPeriod, KItemManager::EWIT_VIP_EVENT, vecItem_));
		}

		// 지급기록 DB저장
		LIF(m_kHelper.VIPCustomerReward_update(dwUserUID_, usVer_, cmit->first));
	}
}

void KGSDBThread::GetSphinxUserData(IN const DWORD & dwUID_, IN const int& nVersion_, OUT int& nRigthAnswerCnt_, OUT std::vector<int> & vecQuestionIDs_)
{
	/*
	L"{ call dbo.SQGASphinxQuizMain_select_20110517 ( %d, %d ) }"

	{ call dbo.SQGASphinxQuizMain_select_20110517 ( @1, @2 ) }
	@1: @iLoginUID_ int
	@2: @iVersion_  int

	1 return ( @1, @2 )
	@1: Level            int
	@2: CorrectAnswerCNT int
	*/

	int nLevel = 0;
	vecQuestionIDs_.clear();
	DBJIF(m_kODBC.Query(L"{ call dbo.SQGASphinxQuizMain_select_20110517 ( %d, %d ) }",
		dwUID_, nVersion_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nLevel
			>> nRigthAnswerCnt_);
	}
	m_kODBC.EndFetch();

	/*
	L"{ call dbo.SQGASphinxQuizSub_select_20110517 ( %d, %d ) }"

	{ call dbo.SQGASphinxQuizSub_select_20110517 ( @1, @2 ) }
	@1: @iLoginUID_ int
	@2: @iVersion_  int

	n return ( @1, @2 )
	@1: Level int
	@2: UID   int
	*/

	DBJIF(m_kODBC.Query(L"{ call dbo.SQGASphinxQuizSub_select_20110517 ( %d, %d ) }",
		dwUID_, nVersion_));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		int nUID = 0;
		VERIFY_QUERY(m_kODBC >> nLevel
			>> nUID);
		vecQuestionIDs_.push_back(nUID);
	}

	START_LOG(clog, L"스핑크스 OX 유저 정보 가져오기 UserUID: " << dwUID_)
		<< BUILD_LOG(nRigthAnswerCnt_)
		<< BUILD_LOG(vecQuestionIDs_.size()) << END_LOG;
}

void KGSDBThread::UpdateUserSphinxAnswerCnt(IN const DWORD dwUID_, IN const int nVersion_, IN const int nLevel_, IN const int nRigthAnswerCnt_)
{
	/*
	L"{ call dbo.SQGASphinxQuizMain_merge_20110517 ( %d, %d, %d, %d ) }"

	{ call dbo.SQGASphinxQuizMain_merge_20110517 ( @1, @2, @3, @4 ) }
	@1: @iLoginUID_         int
	@2: @iVersion_          int
	@3: @iLevel_            int
	@4: @iCorrectAnswerCNT_ int

	1 return ( @1 )
	@1 ; OK
	0       : 성공
	-1      : 유저정보가 없음
	-101이하: 트랜젝션 에러
	*/

	int nOK = -99;
	DBJIF(m_kODBC.Query(L"{ call dbo.SQGASphinxQuizMain_merge_20110517 ( %d, %d, %d, %d ) }",
		dwUID_, nVersion_, nLevel_, nRigthAnswerCnt_));
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	if (nOK != 0) {
		START_LOG(cerr, L"스핑크스 데이터 갱신 에러")
			<< BUILD_LOG(nOK)
			<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
	}
}

void KGSDBThread::InsertUserSphinxQuizInfo(IN const DWORD dwUID_, IN const int nVersion_, IN const int nLevel_, IN const int nQuestionID_)
{
	/*
	L"{ call dbo.SQGASphinxQuizSub_insert_20110517 ( %d, %d, %d, %d ) }"

	{ call dbo.SQGASphinxQuizSub_insert_20110517 ( @1, @2, @3, @4 ) }
	@1: @iLoginUID_ int
	@2: @iVersion_  int
	@3: @iLevel_    int
	@4: @iUID_      int

	1 return ( @1 )
	@1 ; OK
	0       : 성공
	-1      : 유저정보가 없음
	-2      : 스핑크스퀴즈메인정보가 없음
	-3      : 해당문제를 이미 풀었음
	-101이하: 트랜젝션 에러
	*/

	int nOK = -99;
	DBJIF(m_kODBC.Query(L"{ call dbo.SQGASphinxQuizSub_insert_20110517 ( %d, %d, %d, %d ) }",
		dwUID_, nVersion_, nLevel_, nQuestionID_));
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	if (nOK != 0) {
		START_LOG(cerr, L"스핑크스 유저 문제푼 정보 삽입 에러")
			<< BUILD_LOG(nOK)
			<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
	}
}

void KGSDBThread::InsertDungeonUseItemStat(IN const int nModeID_, IN const int nDifficult_, IN std::map<GCITEMID, int> mapItemUseCount_, IN const DWORD dwLoginUID_, IN time_t tmDate_)
{
	/*
	20110713. mk8253.
	던전 통계 아이템 사용.

	L"{ call dbo.MSSTModeStatisticsItem_insert ( N'%s', %d, %d, %d, %d, %d ) }"

	{ call dbo.MSSTModeStatisticsItem_insert ( @1, @2, @3, @4, @5, @6 ) }
	@1 ; @strRegDate_    [smalldatetime]
	@2 ; @iLoginUID_     [int]
	@3 ; @iModeID_       [int]
	@4 ; @iDifficult_    [int]
	@5 ; @iItemID_       [int]
	@6 ; @iItemUseCount_ [int]

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-101이하 ; 트랜젝션 에러
	*/
	std::map<GCITEMID, int>::iterator mit;
	CTime tmTime(tmDate_);
	for (mit = mapItemUseCount_.begin(); mit != mapItemUseCount_.end(); ++mit) {
		DBJIF(m_kODBCStat.Query(L"{ call dbo.MSSTModeStatisticsItem_insert ( N'%s', %d, %d, %d, %d, %d ) }",
			(LPCTSTR)tmTime.Format(L"%Y%m%d %H:%M:%S"), dwLoginUID_, nModeID_, nDifficult_, mit->first, mit->second));

		if (m_kODBCStat.BeginFetch()) {
			int nOK = -99;
			VERIFY_QUERY(m_kODBCStat >> nOK);
			m_kODBCStat.EndFetch();

			if (nOK != 0) {
				START_LOG(cerr, L"던전 아이템 사용 통계 에러")
					<< BUILD_LOG(nOK)
					<< BUILD_LOG(m_kODBCStat.m_strLastQuery)
					<< END_LOG;
			}
		}
	}
}

void KGSDBThread::InsertDungeonBuyItemStat(IN const int nModeID_, IN const int nDifficult_, IN std::map<std::pair<DWORD, GCITEMID>, int> mapItemBuyCount_, IN const DWORD dwLoginUID_, IN time_t tmDate_)
{
	/*
	20110713. mk8253.
	던전 통계 아이템 구매.

	L"{ call dbo.MSSTModeStatisticsItemBuyCount_insert ( N'%s', %d, %d, %d, %d, %d, %d ) }"

	{ call dbo.MSSTModeStatisticsItemBuyCount_insert ( @1, @2, @3, @4, @5, @6, @7 ) }
	@1 ; @strRegDate_    [smalldatetime]
	@2 ; @iLoginUID_     [int]
	@3 ; @iModeID_       [int]
	@4 ; @iDifficult_    [int]
	@5 ; @iLevel_        [int]
	@6 ; @iItemID_       [int]
	@7 ; @iItemBuyCount_ [int]

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-101이하 ; 트랜젝션 에러
	*/

	std::map<std::pair<DWORD, GCITEMID>, int>::iterator mit;
	CTime tmTime(tmDate_);

	for (mit = mapItemBuyCount_.begin(); mit != mapItemBuyCount_.end(); ++mit) {
		DBJIF(m_kODBCStat.Query(L"{ call dbo.MSSTModeStatisticsItemBuyCount_insert ( N'%s', %d, %d, %d, %d, %d, %d ) }",
			(LPCTSTR)tmTime.Format(L"%Y%m%d %H:%M:%S"), dwLoginUID_, nModeID_, nDifficult_, mit->first.first, mit->first.second, mit->second));

		if (m_kODBCStat.BeginFetch()) {
			int nOK = -99;
			VERIFY_QUERY(m_kODBCStat >> nOK);
			m_kODBCStat.EndFetch();

			if (nOK != 0) {
				START_LOG(cerr, L"던전 아이템 구매 통계 에러")
					<< BUILD_LOG(nOK)
					<< BUILD_LOG(m_kODBCStat.m_strLastQuery)
					<< END_LOG;
			}
		}
	}
}

void KGSDBThread::InsertDungeonZoneDeathStat(IN const int nModeID_, IN const int nDifficult_, IN std::map<DWORD, int> mapZoneDeathCount_, IN const DWORD dwLoginUID_, IN time_t tmDate_)
{
	/*
	20110713. mk8253.
	던전 통계 존 사망.

	L"{ call dbo.MSSTModeStatisticsZone_insert ( N'%s', %d, %d, %d, %d, %d ) }"

	{ call dbo.MSSTModeStatisticsZone_insert ( @1, @2, @3, @4, @5, @6 ) }
	@1 ; @strRegDate_      [smalldatetime]
	@2 ; @iLoginUID_       [int]
	@3 ; @iModeID_         [int]
	@4 ; @iDifficult_      [int]
	@5 ; @iZoneID_         [int]
	@6 ; @iZoneDeathCount_ [int]

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-101이하 ; 트랜젝션 에러
	*/

	std::map<DWORD, int>::iterator mit;
	CTime tmTime(tmDate_);

	for (mit = mapZoneDeathCount_.begin(); mit != mapZoneDeathCount_.end(); ++mit) {
		DBJIF(m_kODBCStat.Query(L"{ call dbo.MSSTModeStatisticsZone_insert ( N'%s', %d, %d, %d, %d, %d ) }",
			(LPCTSTR)tmTime.Format(L"%Y%m%d %H:%M:%S"), dwLoginUID_, nModeID_, nDifficult_, mit->first, mit->second));

		if (m_kODBCStat.BeginFetch()) {
			int nOK = -99;
			VERIFY_QUERY(m_kODBCStat >> nOK);
			m_kODBCStat.EndFetch();

			if (nOK != 0) {
				START_LOG(cerr, L"던전 존 사망 통계 에러")
					<< BUILD_LOG(nOK)
					<< BUILD_LOG(m_kODBCStat.m_strLastQuery)
					<< END_LOG;
			}
		}
	}
}

void KGSDBThread::InsertDungeonDeathTypeStat(IN const int nModeID_, IN const int nDifficult_, IN std::map<std::pair<DWORD, DWORD>, int> mapDeathType_, IN const DWORD dwLoginUID_, IN time_t tmDate_)
{
	/*
	20110713. mk8253.
	던전 통계 사망 타입.

	L"{ call dbo.MSSTModeStatisticsType_insert ( N'%s', %d, %d, %d, %d, %d, %d ) }"

	{ call dbo.MSSTModeStatisticsType_insert ( @1, @2, @3, @4, @5, @6, @7 ) }
	@1 ; @strRegDate_      [smalldatetime]
	@2 ; @iLoginUID_       [int]
	@3 ; @iModeID_         [int]
	@4 ; @iDifficult_      [int]
	@5 ; @iType_           [int]
	@6 ; @iIndex_          [int]
	@7 ; @iTypeDeathCount_ [int]

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-101이하 ; 트랜젝션 에러
	*/

	std::map<std::pair<DWORD, DWORD>, int>::iterator mit;
	CTime tmTime(tmDate_);

	for (mit = mapDeathType_.begin(); mit != mapDeathType_.end(); ++mit) {
		DBJIF(m_kODBCStat.Query(L"{ call dbo.MSSTModeStatisticsType_insert ( N'%s', %d, %d, %d, %d, %d, %d ) }",
			(LPCTSTR)tmTime.Format(L"%Y%m%d %H:%M:%S"), dwLoginUID_, nModeID_, nDifficult_, mit->first.first, mit->first.second, mit->second));

		if (m_kODBCStat.BeginFetch()) {
			int nOK = -99;
			VERIFY_QUERY(m_kODBCStat >> nOK);
			m_kODBCStat.EndFetch();

			if (nOK != 0) {
				START_LOG(cerr, L"던전 사망 Type 통계 에러")
					<< BUILD_LOG(nOK)
					<< BUILD_LOG(m_kODBCStat.m_strLastQuery)
					<< END_LOG;
			}
		}
	}
}

void KGSDBThread::UpdateUserGamePoint(IN const DWORD dwUID_, IN const char cCharType_, IN const int& nSlotID_, IN const int nGamePointDiff_, OUT int& nOK_, OUT int& nGP_)
{
	/*
	L"{ call dbo.CIGACharacterInfo_update_GamePoint_20130620 ( %d, %d, %d, %I64d ) }"

	{ call dbo.CIGACharacterInfo_update_GamePoint_20130620 ( @01, @02, @03, @04 ) }
		@01iLoginUID_input       int
	,   @02iCharType_input       int
	,   @03iSlotNo_input         int
	,   @04iGamePoint_diff_input bigint

	1 return ( @01, @02 )
	@01: OK        int
		0        : 캐릭터 생성 성공
		-1       : 캐릭터가 존재하지 않음
		-801~-899: 무시가능 에러
		-901~-999: 치명적인 에러
	@02: GamePoint bigint
	*/
	DBJIF(m_kODBC.Query(L"{ call dbo.CIGACharacterInfo_update_GamePoint_20130620 ( %d, %d, %d, %d ) }",
		dwUID_, cCharType_, nSlotID_, nGamePointDiff_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK_ >> nGP_);
		m_kODBC.EndFetch();
	}

	LIF(m_kHelper.IgnorableErrorProcessingAfterCallSP(nOK_));

	_LOG_SUCCESS(nOK_ == 0, L"유저 GP 기록.. Ret : " << nOK_)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< BUILD_LOG(nGP_) << END_LOG;
}

void KGSDBThread::GetSubMissionInfo(IN const DWORD dwUID_, IN const char& cCharType_, OUT std::vector<KMissionData> & vecMissionSlot_)
{
	/*
	L"{ call dbo.MSGAMissionSystemSub_select_20130527 ( %d, %d ) }"

	{ call dbo.MSGAMissionSystemSub_select_20130527 ( @01, @02 ) }
	@01iLoginUID_input int
	,   @02iCharType_input int

	n return ( @01, @02, @03 )
	@01: MainID int
	@02: SubID  int
	@03: CNT    int
	*/

	DBJIF(m_kODBC.Query(L"{ call dbo.MSGAMissionSystemSub_select_20130527 ( %d, %d ) }", dwUID_, cCharType_));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		DWORD dwMissionID = 0;
		std::pair<int, int> prComplete;
		VERIFY_QUERY(m_kODBC >> dwMissionID
			>> prComplete.first
			>> prComplete.second);

		std::vector<KMissionData>::iterator vitMission;
		vitMission = std::find_if(vecMissionSlot_.begin(), vecMissionSlot_.end(),
			boost::bind(&KMissionData::m_dwMissionID, _1) == dwMissionID);
		if (vitMission != vecMissionSlot_.end()) {
			vitMission->m_mapCompletionInfo.insert(prComplete);
			vitMission->m_nInitSubCnt = prComplete.second; // DB 초기값 저장.
		}
	}
}

bool KGSDBThread::CheckUpdateCountItem(IN const DWORD dwUID_, IN const GCITEMID ItemID_, IN const char cGrade_, IN const int nDellFactor_, IN const char cCharType_)
{
	/*
	mk8253. 20091124.
	수량아이템 개수 변경 획득 및 사용, 삭제
	L"{ call dbo.UIGAUserItemDuration_select_1_20130415 ( %d, %d, %d, %d ) }"

	{ call dbo.UIGAUserItemDuration_select_1_20130415 ( @01, @02, @03, @04 ) }
		@01iLoginUID_input int
	,   @02iCharType_input int
	,   @03iItemID_input   int
	,   @04iGrade_input    int

	1 return ( @01 )
	@01: ItemUID bigint
	*/

	GCITEMUID itemUID = 0;
	DBJIF2(m_kODBC.Query(L"{ call dbo.UIGAUserItemDuration_select_1_20130415 (%d,%d,%d,%d) }",
		dwUID_, cCharType_, ItemID_, cGrade_));

	if (m_kODBC.BeginFetch())
	{
		VERIFY_QUERY(m_kODBC >> itemUID);
		m_kODBC.EndFetch();
	}

	_LOG_CONDITION(itemUID > 0, clog, cwarn, L"Count Item Check, OK. ItemUID : " << itemUID)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return (itemUID > 0);
}


void KGSDBThread::GetSantaItemInfo(IN const DWORD dwEventUID_, IN const DWORD dwUserType_, OUT std::vector<KDropItemInfo> & vecData_)
{
	/*
	L"{ call dbo.SAGASantaItem_select_201210718 ( %d, %d ) }"

	{ call dbo.SAGASantaItem_select_201210718 ( @1, @2 ) }
	@1: @iEventUID_ int
	@2: @iUserType_ tinyint

	n return ( @1, @2, @3, @4, @5 )
	@1: ItemID        int
	@2: GradeID       int
	@3: Period        int
	@4: Duration      duration
	@5: StrengthLevel int
	*/
	vecData_.clear();
	DBJIF(m_kODBC.Query(L"{ call dbo.SAGASantaItem_select_201210718 ( %d, %d ) }",
		dwEventUID_, dwUserType_));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		KDropItemInfo kData;

		VERIFY_QUERY(m_kODBC >> kData.m_ItemID
			>> kData.m_cGradeID
			>> kData.m_nPeriod
			>> kData.m_nDuration
			>> kData.m_cEnchantLevel);

		vecData_.push_back(kData);
	}
}

int KGSDBThread::UpdateSantaInfo(IN const DWORD dwEventUID_, IN const DWORD dwUserUID_, IN const char& cCharType_)
{
	/*
	L"{ call dbo.SSGASantaSystemCharacter_insert_20130521 ( %d, %d, %d ) }"

	{ call dbo.SSGASantaSystemCharacter_insert_20130521 ( @01, @02, @03 ) }
	@01: @01iLoginUID_input int
	@02: @02iCharType_input int
	@03: @03iEventUID_input int

	1 return ( @01 )
	@01: OK int
	0        : 성공
	-1       : 유저 정보가 없음
	-2       : 캐릭터 정보가 없음
	-3       : 등록된 이벤트가 아님
	-4       : 이미 이벤트를 진행함
	-801~-899: 무시가능 에러
	-901~-999: 치명적인 에러
	*/

	int nOK = -99;
	DBJIF3(m_kODBC.Query(L"{ call dbo.SSGASantaSystemCharacter_insert_20130521 ( %d, %d, %d ) }",
		dwUserUID_, cCharType_, dwEventUID_), return nOK);
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	LIF(m_kHelper.IgnorableErrorProcessingAfterCallSP(nOK));

	LOG_SUCCESS(nOK == 0)
		<< BUILD_LOG(nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return nOK;
}

void KGSDBThread::InsertStrengthStat(IN const DWORD dwUID_,
	IN const GCITEMID StrengthItemID_,
	IN const GCITEMUID StrengthItemUID_,
	IN const int nStrengthLevel_,
	IN const GCITEMID EquipItemID_,
	IN const GCITEMUID EquipItemUID_,
	IN DWORD dwType_,
	IN DWORD dwResult_,
	IN GCITEMID AssistItemID_)
{
	/*
	L"{ call dbo.UIGAUserItemStrengthDetail_insert_20110731 ( %d, %I64d, %d, %I64d, %d, %d, %d, %d, %d ) }"

	{ call dbo.UIGAUserItemStrengthDetail_insert_20110731 ( @1, @2, @3, @4, @5, @6, @7, @8, @9 ) }
	@1: @iLoginUID_        int
	@2: @iEquippedItemUID_ bigint
	@3: @iEquippedItemID_  int
	@4: @iStrengthItemUID_ bigint
	@5: @iStrengthItemID_  int
	@6: @iStrengthLevel_   int
	@7: @iActionType1_     int
	@8: @iActionType2_     int
	@9: @iUseItemID_       int

	1 return ( @1 )
	@1: OK int
	0       : 성공
	-101이하: 트랜잭션에러
	*/

	int nOK = -99;
	DBJIF(m_kODBC.Query(L"{ call dbo.UIGAUserItemStrengthDetail_insert_20110731 ( %d, %I64d, %d, %I64d, %d, %d, %d, %d, %d ) }",
		dwUID_, EquipItemUID_, EquipItemID_, StrengthItemUID_, StrengthItemID_, nStrengthLevel_, dwType_, dwResult_, AssistItemID_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	_LOG_SUCCESS(nOK == 0, L"강화 통계 남기기 결과.. " << nOK)
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
}

void KGSDBThread::ItemStrengthMigration(IN const DWORD & dwUserUID_, IN const char cCharacter_)
{
	std::vector< std::pair<GCITEMUID, int> >::iterator vit;
	std::vector< std::pair<GCITEMUID, int> > vecItemEnchantLv;
	vecItemEnchantLv.clear();

	// 1
	for (int i = 1; i <= 117; ++i) {
		m_kHelper.UserItemStrength_migration011(dwUserUID_, i, vecItemEnchantLv, cCharacter_);
		for (vit = vecItemEnchantLv.begin(); vit != vecItemEnchantLv.end(); ++vit) {
			m_kHelper.UserItemStrength_migration012(dwUserUID_, cCharacter_, vit->second);
			m_kHelper.UserItemStrength_migration099(dwUserUID_, vit->first, vit->second, cCharacter_);
		}
	}

	// 2
	for (int i = 1; i <= 39; ++i) {
		m_kHelper.UserItemStrength_migration021(dwUserUID_, i, vecItemEnchantLv, cCharacter_);
		for (vit = vecItemEnchantLv.begin(); vit != vecItemEnchantLv.end(); ++vit) {
			m_kHelper.UserItemStrength_migration022(dwUserUID_, cCharacter_, vit->second);
			m_kHelper.UserItemStrength_migration099(dwUserUID_, vit->first, vit->second, cCharacter_);
		}
	}

	// 3
	m_kHelper.UserItemStrength_migration031(dwUserUID_, vecItemEnchantLv, cCharacter_);
	for (vit = vecItemEnchantLv.begin(); vit != vecItemEnchantLv.end(); ++vit) {
		m_kHelper.UserItemStrength_migration032(dwUserUID_, cCharacter_, vit->second);
		m_kHelper.UserItemStrength_migration099(dwUserUID_, vit->first, vit->second, cCharacter_);
	}

	// 4
	m_kHelper.UserItemStrength_migration041(dwUserUID_, vecItemEnchantLv, cCharacter_);
	for (vit = vecItemEnchantLv.begin(); vit != vecItemEnchantLv.end(); ++vit) {
		m_kHelper.UserItemStrength_migration042(dwUserUID_, cCharacter_, vit->second);
		m_kHelper.UserItemStrength_migration099(dwUserUID_, vit->first, vit->second, cCharacter_);
	}

	// 5
	m_kHelper.UserItemStrength_migration051(dwUserUID_, vecItemEnchantLv, cCharacter_);
	for (vit = vecItemEnchantLv.begin(); vit != vecItemEnchantLv.end(); ++vit) {
		m_kHelper.UserItemStrength_migration052(dwUserUID_, cCharacter_, vit->second);
		m_kHelper.UserItemStrength_migration099(dwUserUID_, vit->first, vit->second, cCharacter_);
	}

	// 6
	m_kHelper.UserItemStrength_migration061(dwUserUID_, vecItemEnchantLv, cCharacter_);
	for (vit = vecItemEnchantLv.begin(); vit != vecItemEnchantLv.end(); ++vit) {
		m_kHelper.UserItemStrength_migration062(dwUserUID_, cCharacter_, vit->second);
		m_kHelper.UserItemStrength_migration099(dwUserUID_, vit->first, vit->second, cCharacter_);
	}

	// 7
	for (int i = 1; i <= 346; ++i) {
		m_kHelper.UserItemStrength_migration071(dwUserUID_, i, vecItemEnchantLv, cCharacter_);
		for (vit = vecItemEnchantLv.begin(); vit != vecItemEnchantLv.end(); ++vit) {
			m_kHelper.UserItemStrength_migration099(dwUserUID_, vit->first, vit->second, cCharacter_);
		}
	}
}

bool KGSDBThread::SetTypeEraseItem(IN const std::wstring strLogin_, IN const DWORD dwLoginUID_, IN const GCITEMUID IntemUID_, IN const DWORD dwDelType_, IN const char cCharType_)
{
	/*
	L"{ call dbo.UIGAUserItem_DelState_update_20131005 ( %d, %I64d, %d ) }"

	{ call dbo.UIGAUserItem_DelState_update_20131005 ( @01, @02, @03 ) }
	@01: @01iLoginUID_input int
	@02: @02iItemUID_input  bigint
	@04: @03iDelstate_input tinyint

	1 return ( @01 )
	@01: OK
		0        : 성공
		-1       : 메인 아이템 정보 없음
		-801~-899: 무시가능 에러
		-901~-999: 치명적인 에러
		-1000    : 사용 중지된 sp인데 사용하려 함
	*/
	int nOK = -99;
	DBJIF2(m_kODBC.Query(L"{ call dbo.UIGAUserItem_DelState_update_20131005 ( %d, %I64d, %d ) }", dwLoginUID_, IntemUID_, dwDelType_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	LIF(m_kHelper.IgnorableErrorProcessingAfterCallSP(nOK));

	if (nOK != 0) {
		START_LOG(cerr, L"아이템 삭제 실패. Name : " << strLogin_)
			<< BUILD_LOGc(nOK)
			<< BUILD_LOG(IntemUID_)
			<< BUILD_LOG(dwDelType_)
			<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
	}

	return (nOK == 0);
}

void KGSDBThread::AdjustObjectItemInfo(IN const DWORD & dwUserUID_, IN OUT std::vector<KItem> & vecItems_)
{
	std::vector<KAgitObjectPos> vecObjectPos;
	LIF(m_kHelper.AgiteObjPos_select(dwUserUID_, vecObjectPos));

	{
		std::vector<KAgitObjectPos>::iterator vit;
		for (vit = vecObjectPos.begin(); vit != vecObjectPos.end(); ++vit) {

			std::vector<KItem>::iterator vitItem;
			vitItem = std::find_if(vecItems_.begin(), vecItems_.end(),
				boost::bind(&KItem::m_ItemUID, _1) == vit->m_UID);

			if (vitItem == vecItems_.end()) {
				START_LOG(cwarn, L"아지트 오브젝트인데, 인벤토리에 없음. ItemID : " << vit->m_ID << L", ItemUID : " << vit->m_UID << L", Usem_UID : " << dwUserUID_) << END_LOG;
				continue;
			}

			vitItem->AddItemType(KItem::TYPE_AGIT_OBJ);
			START_LOG(clog, L"아지트 아이템 설정.. ItemID : " << vit->m_ID << L", ItemUID : " << vit->m_UID << L", Usem_UID : " << dwUserUID_) << END_LOG;
		}
	}

	{
		std::set<GCITEMID> setList;
		SiKAgitHelper()->GetAgitObjectItemIDList(setList);
		std::set<GCITEMID>::iterator sit;
		for (sit = setList.begin(); sit != setList.end(); ++sit) {
			std::vector<KItem>::iterator vitItem;
			vitItem = std::find_if(vecItems_.begin(), vecItems_.end(),
				boost::bind(&KItem::m_ItemID, _1) == *sit);

			if (vitItem == vecItems_.end()) {
				continue;
			}

			if (false == vitItem->CheckItemType(KItem::TYPE_AGIT_OBJ)) {
				vitItem->AddItemType(KItem::TYPE_AGIT_OBJ);
				START_LOG(clog, L"아지트 아이템 설정2.. ItemID : " << vitItem->m_ItemID << L", ItemUID : " << vitItem->m_ItemUID << L", Usem_UID : " << dwUserUID_) << END_LOG;
			}
		}
	}

	START_LOG(clog, L"아지트 아이템 설정됨.. UserUID : " << dwUserUID_)
		<< BUILD_LOG(vecObjectPos.size())
		<< BUILD_LOG(vecItems_.size()) << END_LOG;
}

bool KGSDBThread::AgitInfo_select(IN const DWORD & dwUserUID_, IN OUT KAgitInfo & kInfo_)
{
	/*
	mk8253. 20110418.아지트 정보 리턴.

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

		kInfo_.m_nTodayVisiter = kInfo_.m_nInitTodayVisiter;
		m_kODBC.EndFetch();
	}

	kInfo_.m_ucInviteType = KAgitInfo::IT_ACCEPT;
	START_LOG(clog, L"아지트 초대 거부 설정 미구현으로 인해 초대 무조건 수락 m_ucAgitInviteOption = KAgitInfo::IT_ACCEPT" << dwUserUID_)
		<< END_LOG;

	START_LOG(clog, L"DB 아지트 정보 받아옴.. AgitUID : " << kInfo_.m_dwUID)
		<< END_LOG;
	return true;
}

bool KGSDBThread::AgitFairyTree_select(IN const DWORD & dwUserUID_, OUT KFairyTree & kInfo_)
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

	if (kInfo_.m_tmLastDecreaseTime < tmDefault) {
		kInfo_.m_tmLastDecreaseTime = tmDefault;
	}

	START_LOG(clog, L"요정의 나무 정보 리턴.. LoginUID : " << dwUserUID_) << END_LOG;
	return true;
}

bool KGSDBThread::AgitFairyTreeFruit_select(IN const DWORD & dwUserUID_, OUT time_t & tmDate_, OUT int& nFruitType_)
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

	START_LOG(clog, L"요정의 나무 열매정보 리턴.. LoginUID : " << dwUserUID_) << END_LOG;
	return true;
}

bool KGSDBThread::GetTodayUseFruit(IN const DWORD & dwUserUID_)
{
	//{ call dbo.SDGA_server_data_select_integer_user ( @1, @2, @3 ) }
	//@1 ; @iLoginUID_ [int]
	//@2 ; @sdtRegDateD_ [smalldatetime]  ; 일단위까지만 인식함
	//@3 ; @iType_ [tinyint]              ; 서버에서 원하는 숫자별로 분류됨
	//
	//1 return ( @1 )
	//@1 ; Data int

	DBJIF2(m_kODBC.Query(L"{ call dbo.SDGA_server_data_select_integer_user (  %d, N'%s', %d ) }",
		dwUserUID_, (LPCTSTR)CTime::GetCurrentTime().Format(L"%Y%m%d"), UDT_TODAY_USE_FRUIT));

	int nRet = 0;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
	}
	m_kODBC.EndFetch();

	START_LOG(clog, L"일일 요정의나무 과일 사용여부 확인.. LoginUID : " << dwUserUID_) << END_LOG;
	return (nRet > 0);
}

bool KGSDBThread::AgiteFlowerpot_init(IN const DWORD & dwUserUID_, IN const GCITEMUID & ItemUID_)
{
	/*
	mk8253. 20110419. 화분을 처음 획득할 경우 초기화

	{ call dbo.ASGAAgitFlowerpot_init ( @1, @2 ) }
	@1 ; @iLoginUID_ [int]
	@2 ; @iPotUID_   [bigint]

	1 return ( @1 )
	@1 ; OK
	0        ; 성공
	-1       ; 화분 정보가 없음
	-101이하 ; 트랜젝션 에러
	*/
	DBJIF2(m_kODBC.Query(L"{ call dbo.ASGAAgitFlowerpot_init ( %d, %d ) }",
		dwUserUID_, ItemUID_));

	int nRet = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nRet == 0, L"화분을 처음 획득할 경우 초기화.. Result : " << nRet)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return (nRet == 0);
}

bool KGSDBThread::UpdateGuildMarkInfo(IN const DWORD dwGuildUID_)
{
	// 3,4일 경우 이 SP를 불러서 갱신 Center
	/*
	{ call dbo.GSGAGuildSystem_update_Mark_Broadcast ( @1 ) }
	@1 ; @iNID_      int

	1 select ( @1, @2 )
	@1  ; 호출된 sp이름 nvarchar( 4000 )
	@2  ; OK            int
	0        ; 성공
	-1       ; 길드 정보 없음
	-2       ; 길드마크가 승인성공실패상태가 아님
	-101이하 ; 트랜잭션 에러
	*/
	std::wstring strSP;
	int nOK = -99;

	DBJIF2(m_kODBC.Query(L"{ call dbo.GSGAGuildSystem_update_Mark_Broadcast ( %d ) }",
		dwGuildUID_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> strSP >> nOK);
	}
	m_kODBC.EndFetch();

	_LOG_SUCCESS(nOK == 0, L"nOK : " << nOK)
		<< BUILD_LOG(dwGuildUID_)
		<< BUILD_LOG(strSP) << END_LOG;

	return (nOK == 0);
}

bool KGSDBThread::DailyStageCollect_insert(IN const KEVENT_STAT_DUNGEON_STAGE_NOT & kPacket_)
{
	/*
	{ call dbo.DSCODailyStageCollect_insert_20120605 ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }

	{ call dbo.DSCODailyStageCollect_insert_20120605 ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10, @11, @12 ) }
	@1 : @iModeID_        int
	@2 : @iDifficulty_    int
	@3 : @iStageID_       int
	@4 : @iClearTime_     int
	@5 : @iDeath_         int
	@6 : @iLeave_         int
	@7 : @iSpecial_       int
	@8 : @iPet_           int
	@9 : @iReceiveDamage_ int
	@10: @iSendDamage_    int
	@11: @iLevel_         int
	@12: @iType_          int

	1 return ( @1 )
	@1: OK int
		0       : 성공
		-101이하: 트랜잭션 에러
	*/

	std::map< KDungeonDetail, KDungeonPlayState >::const_iterator cmit;
	for (cmit = kPacket_.m_mapDungeonPlayState.begin(); cmit != kPacket_.m_mapDungeonPlayState.end(); ++cmit) {
		DBJIF2(m_kODBCStat.Query(L"{ call dbo.DSCODailyStageCollect_insert_20120605 ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }",
			cmit->first.m_dwDungeonID,
			cmit->first.m_dwDifficult,
			cmit->first.m_dwStageID,
			cmit->second.m_dwClearTime,
			cmit->second.m_usDeath_Cnt,
			cmit->second.m_usLeave_Cnt,
			cmit->second.m_usSpecial_Cnt,
			cmit->second.m_usPetSpecial_Cnt,
			cmit->second.m_dwReceiveDamage,
			cmit->second.m_dwSendDamage,
			kPacket_.m_dwCharLevel,
			kPacket_.m_nType));

		int nRet = -99;
		if (m_kODBCStat.BeginFetch()) {
			VERIFY_QUERY(m_kODBCStat >> nRet);
			m_kODBCStat.EndFetch();
		}

		_LOG_SUCCESS(nRet == 0, L"던전 스테이지 통계.. Result : " << nRet)
			<< BUILD_LOG(m_kODBCStat.m_strLastQuery) << END_LOG;
	}
	return true;
}

bool KGSDBThread::DailyCPLItemCollect_insert(IN const KEVENT_STAT_CHAR_EQUIP_NOT & mapList_)
{
	/*
	장착아이템 통계

	{ call dbo.DSCODailyCPLItemCollect_insert ( @1, @2, @3, @4, @5 ) }
	@1 ; @iCharType_    int
	@2 ; @iPromotion_   int
	@3 ; @iLevel_       int
	@4 ; @iItemID_      int
	@5 ; @iEquipCNT_    int

	1 return ( @1 )
	@1 ; OK int
	0        ; 성공
	-101이하 ; 트랜잭션 에러
	*/

	KEVENT_STAT_CHAR_EQUIP_NOT::const_iterator cmit;
	for (cmit = mapList_.begin(); cmit != mapList_.end(); ++cmit) {

		std::map<GCITEMID, DWORD>::const_iterator cmitItem;
		for (cmitItem = cmit->second.begin(); cmitItem != cmit->second.end(); ++cmitItem) {
			DBJIF2(m_kODBCStat.Query(L"{ call dbo.DSCODailyCPLItemCollect_insert ( %d, %d, %d, %d, %d ) }",
				static_cast<int>(cmit->first.m_cCharacterType),
				static_cast<int>(cmit->first.m_cPromotion),
				cmit->first.m_dwLv,
				cmitItem->first,
				cmitItem->second));

			int nRet = -99;
			if (m_kODBCStat.BeginFetch()) {
				VERIFY_QUERY(m_kODBCStat >> nRet);
				m_kODBCStat.EndFetch();
			}

			_LOG_SUCCESS(nRet == 0, L"장착아이템 통계.. Result : " << nRet)
				<< BUILD_LOG(m_kODBCStat.m_strLastQuery) << END_LOG;
		}
	}
	return true;
}

bool KGSDBThread::DailyCPLPTCollect_insert(IN const KEVENT_STAT_CHAR_LV_PT_NOT & vecList_)
{
	/*
	캐릭터 레벨별 플레이타임 통계

	{ call dbo. DSCODailyCPLPTCollect_insert ( @1, @2, @3, @4 ) }
	@1 ; @iCharType_  int
	@2 ; @iPromotion_ int
	@3 ; @iLevel_     int
	@4 ; @iPT_        int


	1 return ( @1 )
	@1 ; OK int
	0        ; 성공
	-101이하 ; 트랜잭션 에러
	*/

	KEVENT_STAT_CHAR_LV_PT_NOT::const_iterator cvit;
	for (cvit = vecList_.begin(); cvit != vecList_.end(); ++cvit) {
		DBJIF2(m_kODBCStat.Query(L"{ call dbo.DSCODailyCPLPTCollect_insert ( %d, %d, %d, %d ) }",
			static_cast<int>(cvit->first.m_cCharacterType),
			static_cast<int>(cvit->first.m_cPromotion),
			cvit->first.m_dwLv,
			cvit->second));

		int nRet = -99;
		if (m_kODBCStat.BeginFetch()) {
			VERIFY_QUERY(m_kODBCStat >> nRet);
			m_kODBCStat.EndFetch();
		}

		_LOG_SUCCESS(nRet == 0, L"캐릭터 레벨별 플레이타임 통계.. Result : " << nRet)
			<< BUILD_LOG(m_kODBCStat.m_strLastQuery) << END_LOG;
	}
	return true;
}

bool KGSDBThread::GetUserSurveyInfo(IN const int& nSurveyVersion_,
	IN const DWORD & dwUID_,
	OUT std::map<int, std::set<std::pair<int, int>>> & mapUserSurveyInfo_)
{
	mapUserSurveyInfo_.clear();
	/*
	서버에서 유저 답변 유/무 파악 하기 위해 사용

	{ call dbo.SEGASurveyEventAnswer_select ( %d, %d ) }
사용 : 서버에서 유저 답변 유/무 파악 하기 위해사용

	 { call dbo.SEGASurveyEventAnswer_select ( @1, @2 ) }
		 @1 ; @iVersion_  smallint
		 @2 ; @iLoginUID_ int

		 n return ( @1, @2, @3, @4 )
		 @1 ; SituationType tinyint
		 @2 ; Value1        int
		 @3 ; Value2        int
	*/
	int nOK = -99;
	DBJIF2(m_kODBC.Query(L"{ call dbo.SEGASurveyEventAnswer_select ( %d, %d ) }",
		nSurveyVersion_, dwUID_));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		int nType;
		std::pair<int, int> prData;
		std::map<int, std::set<std::pair<int, int>>>::iterator mit;

		VERIFY_QUERY(m_kODBC >> nType
			>> prData.first
			>> prData.second);

		mit = mapUserSurveyInfo_.find(nType);
		if (mit == mapUserSurveyInfo_.end()) {
			std::set<std::pair<int, int>> setData;
			setData.clear();
			setData.insert(prData);
			mapUserSurveyInfo_[nType] = setData;
		}
		else {
			mit->second.insert(prData);
		}
	} // end while
	return true;
}

void KGSDBThread::InsertUserSurveyAnswer(IN const DWORD & dwUID_,
	IN const int& nVersion_,
	IN const int& nType_,
	IN const int& nQuestionUID_,
	IN const int& nDataFirst_,
	IN const int& nDataSecond_,
	IN const std::wstring & wstrAnswer_,
	IN const int& nLanguageID_)
{
	/*
	{ call dbo.SEGASurveyEventAnswer_insert ( %d, %d, %d, %d, %d, %d, %s, %d ) }
	사용 : 설문 조사 시스템 유저 답 등록

	{ call dbo.SEGASurveyEventAnswer_insert ( @1, @2, @3, @4, @5, @6, @7, @8 ) }
	@1 ; @iLoginUID_      int
	@2 ; @iVersion_       smallint
	@3 ; @iSituationType_ tinyint
	@4 ; @iQuestionUID_   int
	@5 ; @iValue1_        int
	@6 ; @iValue2_        int
	@7 ; @strAnswer_      nvarchar(400)
	@8 ; @iLanguageID_    tinyint

	1 return ( @1 )
	@1 ; OK int
	0        ; 성공
	-101이하 ; 트랜잭션 에러
	*/

	int nOK = -99;
	DBJIF(m_kODBC.Query(L"{ call dbo.SEGASurveyEventAnswer_insert ( %d, %d, %d, %d, %d, %d, N'%s', %d ) }",
		dwUID_, nVersion_, nType_, nQuestionUID_, nDataFirst_, nDataSecond_, wstrAnswer_.c_str(), nLanguageID_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	if (nOK != 0) {
		START_LOG(cerr, L"설문 정보 남기기 실패. ")
			<< BUILD_LOG(nOK)
			<< BUILD_LOG(dwUID_)
			<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
	}
}

void KGSDBThread::GetCharismasUserData(IN const DWORD & dwUID_, IN const DWORD & dwVersion_, OUT DWORD & dwGrade_, OUT DWORD & dwBonusPoint_)
{
	/*
	L"{ call dbo.CMGAChristMas_select_20111109 ( %d, %d ) }"

	{ call dbo.CMGAChristMas_select_20111109 ( @1, @2 ) }
	@1: @i유저_ int
	@2: @i버전_ int

	1 return ( @1, @2 )
	@1: 착한일포인트 bigint
	@2: 보상받은단계 tinyint
	*/

	DBJIF(m_kODBC.Query(L"{ call dbo.CMGAChristMas_select_20111109 ( %d, %d ) }",
		dwUID_, dwVersion_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> dwBonusPoint_
			>> dwGrade_);
	}
	m_kODBC.EndFetch();

	if (dwGrade_ == 0) { // 등급정보는 1부터 시작된다.
		dwGrade_ = 1;
	}

	START_LOG(clog, L"크리스마스 이벤트 유저 정보 가져오기 UserUID: " << dwUID_)
		<< BUILD_LOG(dwGrade_)
		<< BUILD_LOG(dwBonusPoint_)
		<< END_LOG;
}

bool KGSDBThread::InsertCharismasGift(IN const DWORD & dwUID_, IN const DWORD & dwVersion_, IN const DWORD & dwReceiveUID_, IN const DWORD & dwItemID_)
{
	/*
	L"{ call dbo.CMGAChristMasGift_insert_20111109 ( %d, %d, %d, %d ) }"

	{ call dbo.CMGAChristMasGift_insert_20111109 ( @1, @2, @3, @4 ) }
	@1: @i유저_           int
	@2: @i버전_           int
	@3: @i누구_           int
	@4: @i선물한아이템ID_ int

	1 return ( @1 )
	@1: OK int
		0       : 성공
		-101이하: 트랜잭션 에러
	*/
	DBJIF2(m_kODBC.Query(L"{ call dbo.CMGAChristMasGift_insert_20111109 ( %d, %d, %d, %d ) }",
		dwUID_,
		dwVersion_,
		dwReceiveUID_,
		dwItemID_));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nOK == 0, L"크리스마스 이벤트 선물 주기 Result : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< END_LOG;

	return true;
}

bool KGSDBThread::UpdateCharismasUserData(IN const DWORD & dwUID_, IN const DWORD & dwVersion_, IN const DWORD & dwGrade_, IN const int& nBonusPoint_)
{
	/*
	 L"{ call dbo.CMGAChristMas_merge_20111109 ( %d, %d, %I64d, %d ) }"

	 { call dbo.CMGAChristMas_merge_20111109 ( @1, @2, @3, @4 ) }
	 @1: @i유저_             int
	 @2: @i버전_             int
	 @3: @i착한일포인트Diff_ bigint
	 @4: @i보상받은단계_     tinyint

	 1 return ( @1 )
	 @1: OK int
		 0       : 성공
		 -1      : 유저 정보가 없음
		 -101이하: 트랜잭션 에러
	 */

	DBJIF2(m_kODBC.Query(L"{ call dbo.CMGAChristMas_merge_20111109 ( %d, %d, %d, %d ) }",
		dwUID_,
		dwVersion_,
		nBonusPoint_,
		dwGrade_));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nOK == 0, L"크리스마스 이벤트 유저 정보 갱신 Result : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< END_LOG;

	return true;
}

bool KGSDBThread::UpdateCharismasDonationInfo(IN const DWORD & dwUID_, IN const DWORD & dwVersion_, IN const DWORD & dwDonationCnt_)
{
	/*
	L"{ call dbo.CMGAChristMasDonation_insert_20111109 ( %d, %d, %d ) }"

	{ call dbo.CMGAChristMasDonation_insert_20111109 ( @1, @2, @3 ) }
	@1: @i유저_     int
	@2: @i버전_     int
	@3: @i기부개수_ int

	1 return ( @1 )
	@1: OK int
		0       : 성공
		-101이하: 트랜잭션 에러
	*/

	DBJIF2(m_kODBC.Query(L"{ call dbo.CMGAChristMasDonation_insert_20111109 ( %d, %d, %d ) }",
		dwUID_,
		dwVersion_,
		dwDonationCnt_));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nOK == 0, L"크리스마스 이벤트 기부 정보 갱신 Result : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< END_LOG;

	return true;
}

void KGSDBThread::InsertHotkeyUsedInfoStat(IN const DWORD & dwUID_,
	IN const int& nGameMode_,
	IN const char& cCharType_,
	IN const char& nUsedType_,
	IN const char& cSkillLevel_,
	IN const int& nSkillCount_)
{
	/*
	MP 캐릭터의 필살기를 모아쓰기 또는 단축키로 사용하는 통계 기록

	L"{ call dbo.SHGAShortcuts_merge_20111111 ( %d, %d, %d, %d, %d, %d ) }"

	{ call dbo.SHGAShortcuts_merge_20111111 ( @1, @2, @3, @4, @5, @6 ) }
	@1: @iLoginUID_               int
	@2: @i게임모드_               int
	@3: @i캐릭터종류_             int
	@4: @i모아쓰기단축키사용여부_ int
	@5: @i사용한스킬레벨_         int
	@6: @i스킬사용횟수Diff_       int

	1 return ( @1 )
	@1: OK int
		0       : 성공
		-101이하: 트랜잭션 에러
	*/

	DBJIF(m_kODBC.Query(L"{ call dbo.SHGAShortcuts_merge_20111111 ( %d, %d, %d, %d, %d, %d ) }",
		dwUID_,
		nGameMode_,
		static_cast<int>(cCharType_),
		static_cast<int>(nUsedType_),
		static_cast<int>(cSkillLevel_),
		nSkillCount_));

	int nRet = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nRet == 0, L"DB에 Hotkey 시스템 정보 남기기 실패." << nRet)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
}

bool KGSDBThread::InsertSystemPost(IN OUT KPostItemInfo & kInfo_)
{
	// 편지보내기
	{
		int nSystemPostStorageTime = SiKPostConfig()->GetSystemPostStorageTime();
		DWORD dwFromUserUID = 0;
		if (kInfo_.m_dwFromUserUID != 0) {
			dwFromUserUID = kInfo_.m_dwFromUserUID;
		}
		int nOK = m_kHelper.Post_insert(kInfo_.m_dwToUserUID, kInfo_.m_cCharType, KPostItemInfo::LT_SYSTEM, dwFromUserUID, kInfo_.m_strTitle, kInfo_.m_strMessage, kInfo_.m_PostUID, kInfo_.m_nGamePoint, nSystemPostStorageTime);
		if (nOK != 0) {
			START_LOG(cerr, L"시스템 우편 보내기 DB처리 실패.. ToUserUID : " << kInfo_.m_dwToUserUID)
				<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
			return false;
		}
	}

	if (false == kInfo_.m_vecItem.empty()) {
		std::vector<KItem>::iterator vit;
		for (vit = kInfo_.m_vecItem.begin(); vit != kInfo_.m_vecItem.end(); ++vit) {
			// 우편 대기아이템 등록
			{
				int nOK = m_kHelper.WaitItem_insert_20111028(kInfo_.m_dwToUserUID, kInfo_.m_cCharType, vit->m_ItemID, vit->m_cGradeID, vit->m_nPeriod, vit->m_nCount,
					KItemManager::EWIT_POST_SYSTEM, vit->m_ItemUID, (int)vit->m_sEquipLevel, vit->m_cEnchantLevel);

				if (nOK != 0) {
					START_LOG(cerr, L"시스템우편 대기아이템 등록 DB처리 실패. ToUserUID : " << kInfo_.m_dwToUserUID)
						<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
					continue;
				}
			}

			// 우편 아이템 첨부
			{
				int nOK = m_kHelper.PostItem_insert(kInfo_.m_dwToUserUID, kInfo_.m_cCharType, kInfo_.m_PostUID, vit->m_ItemUID, vit->m_nCount);
				if (nOK != 0) {
					START_LOG(cerr, L"시스템우편 대기아이템 첨부 DB처리 실패.. ToUserUID : " << kInfo_.m_dwToUserUID)
						<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
					continue;
				}
			}

			START_LOG(clog, L"시스템우편 첨부아이템 첨부.. ToUserUID : " << kInfo_.m_dwToUserUID)
				<< BUILD_LOGc(kInfo_.m_cCharType)
				<< BUILD_LOG(vit->m_ItemID)
				<< BUILD_LOG(vit->m_cGradeID)
				<< BUILD_LOG(vit->m_nPeriod)
				<< BUILD_LOG(vit->m_nCount)
				<< BUILD_LOG(vit->m_ItemUID)
				<< BUILD_LOG(vit->m_sEquipLevel)
				<< BUILD_LOGc(vit->m_cEnchantLevel) << END_LOG;
		}
	}

	START_LOG(clog, L"시스템 우편 보내기 완료.. ToUserUID : " << kInfo_.m_dwToUserUID)
		<< BUILD_LOGc(kInfo_.m_cCharType)
		<< BUILD_LOG(kInfo_.m_strTitle)
		<< BUILD_LOG(kInfo_.m_strMessage)
		<< BUILD_LOG(kInfo_.m_PostUID)
		<< BUILD_LOG(kInfo_.m_nGamePoint)
		<< BUILD_LOG(kInfo_.m_vecItem.size()) << END_LOG;

	return true;
}

int KGSDBThread::InsertUserTraceMainInfo(IN const DWORD & dwUID_,
	IN const std::wstring & strStartDate_,
	IN const std::wstring & strEndDate_,
	IN const int& nGrade_,
	IN const DWORD & dwPlayTime_,
	IN const bool& bPCBang_,
	IN const bool& bGuildServer_,
	OUT int& nUID_)
{
	/*
	유저 추적 메인 정보 입력
	L"{ call dbo.UTGAUserTrace_insert_20120103 ( %d, N'%s', N'%s', %d, %d, %d, %d ) }"

	{ call dbo.UTGAUserTrace_insert_20120103 ( @1, @2, @3, @4, @5, @6, @7 ) }
	@1: @iLoginUID_     int
	@2: @sdtStartDateB_ smalldatetime
	@3: @sdtEndDateB_   smalldatetime
	@4: @iGrade_        tinyint
	@5: @iPlayTime_     int
	@6: @bPCBang_       bit
	@7: @bGuildSserver_ bit

	1 return ( @1, @2 )
	@1: OK  int
		0       : 성공
		-1      : 유저가 없음
		-101이하: 트랜젝션 에러
	@2: UID int
	*/

	int nOK = -99;
	DBJIF2(m_kODBC.Query(L"{ call dbo.UTGAUserTrace_insert_20120103 ( %d, N'%s', N'%s', %d, %d, %d, %d ) }",
		dwUID_,
		strStartDate_.c_str(),
		strEndDate_.c_str(),
		nGrade_,
		dwPlayTime_,
		(bPCBang_ ? 1 : 0),
		(bGuildServer_ ? 1 : 0)));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK >> nUID_);
		m_kODBC.EndFetch();

		LOG_SUCCESS(nOK == 0)
			<< L" 통계 기록..." << dbg::endl
			<< BUILD_LOG(nOK)
			<< BUILD_LOG(nUID_)
			<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
	}
	return nOK;
}

int KGSDBThread::InsertUserTraceCharacterInfo(IN const DWORD & dwUID_,
	IN const int& nUID_,
	IN const int& nCharType_,
	IN const __int64& biExp_,
	IN const DWORD & dwLevel_,
	IN const DWORD & dwPlayCount_,
	IN const bool& bTurtleBack_,
	IN const __int64& biTotalExp_)
{
	/*
	유저 추적 캐릭터 정보 입력
	L"{ call dbo.UTGAUserTraceCharacter_insert_20120823 ( %d, %d, %d, %I64d, %d, %d, %d, %I64d ) }"

	{ call dbo.UTGAUserTraceCharacter_insert_20120823 ( @1, @2, @3, @4, @5, @6, @7, @8 ) }
	@1: @iLoginUID_  int
	@2: @iUID_       int
	@3: @iCharType_  tinyint
	@4: @iExp_       bigint
	@5: @iLevel_     int
	@6: @iPlayCount_ int
	@7: @bTurtle_    bit
	@8: @iDiffEXP_   bigint

	1 return ( @1 )
	@1: OK  int
	0       : 성공
	-1      : 메인 정보가 없음
	-2      : 이미 정보가 있음
	-101이하: 트랜젝션 에러
	*/

	int nOK = -99;
	DBJIF2(m_kODBC.Query(L"{ call dbo.UTGAUserTraceCharacter_insert_20120823 ( %d, %d, %d, %I64d, %d, %d, %d, %I64d ) }",
		dwUID_,
		nUID_,
		nCharType_,
		biExp_,
		dwLevel_,
		dwPlayCount_,
		(bTurtleBack_ ? 1 : 0),
		biTotalExp_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();

		LOG_SUCCESS(nOK == 0)
			<< L" 통계 기록..." << dbg::endl
			<< BUILD_LOG(nOK)
			<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
	}
	return nOK;
}

int KGSDBThread::InsertPetInfo(IN const DWORD & dwUserUID_, IN const char& cCharType_, IN const __int64& biPetUID_, IN const int& nPetID_, IN const std::wstring & wstrPetName_, OUT char& cPromotion_, OUT int& nSatiation_)
{
	/*
	L"{ call dbo.UIGAUserItemPet_insert_20130510 ( %d, %d, %I64d, %d, N'%s' ) }"

	{ call dbo.UIGAUserItemPet_insert_20130510 ( @01, @02, @03, @04, @05 ) }
	@01: @01iLoginUID_input  int
	@02: @02iCharType_input  int
	@03: @03iItemUID_input   bigint
	@04: @04iPetID_input     int
	@05: @05strPetName_input nvarchar( 20 )

	1 return ( @01, @02, @03 )
	@01: OK        int
	0        : 성공
	-1       : 유저 정보 없음
	-2       : 인벤 아이템 정보 없음
	-3       : 해당 캐릭터가 보유하고 있는 아이템이 아님
	-4       : 이미 펫으로 등록되어 있음
	-801~-899: 무시가능 에러
	-901~-999: 치명적인 에러
	@02: Promotion tinyint
	@03: FullTime  int
	*/

	int nOK = -99;
	DBJIF2(m_kODBC.Query(L"{ call dbo.UIGAUserItemPet_insert_20130510 ( %d, %d, %I64d, %d, N'%s' ) }",
		dwUserUID_,
		cCharType_,
		biPetUID_,
		nPetID_,
		wstrPetName_.c_str()));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK >> cPromotion_ >> nSatiation_);
		m_kODBC.EndFetch();
	}

	LIF(m_kHelper.IgnorableErrorProcessingAfterCallSP(nOK));

	LOG_SUCCESS(nOK == 0)
		<< L" 펫 생성 정보 기록." << dbg::endl
		<< BUILD_LOG(nOK)
		<< BUILD_LOGc(cPromotion_)
		<< BUILD_LOG(nSatiation_)
		<< BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

	return nOK;
}

void KGSDBThread::GetMainCollectionInfo(IN const DWORD dwUID_, IN const char& cCharType_, OUT std::vector<KCollectionData> & vecCollection_)
{
	//유저 접속시 칭호 미션 메인 정보 리턴 
	/*
	L"{ call dbo.TCGATitleCollectionMain_select_20130516 ( %d, %d ) }"

	{ call dbo.TCGATitleCollectionMain_select_20130516 ( @01, @02 ) }
	@01: @01iLoginUID_input int
	@02: @02iCharType_input int

	n return ( @01, @02 )
	@01: MainMissionID int
	@02: [Status]      int
	*/

	DBJIF(m_kODBC.Query(L"{ call dbo.TCGATitleCollectionMain_select_20130516 ( %d, %d ) }", dwUID_, cCharType_));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		KCollectionData kCollectionSlot;
		VERIFY_QUERY(m_kODBC >> kCollectionSlot.m_dwMissionID
			>> kCollectionSlot.m_dwState);

		vecCollection_.push_back(kCollectionSlot);
	}
}

void KGSDBThread::GetSubCollectionInfo(IN const DWORD dwUID_, IN const char& cCharType_, OUT std::vector<KCollectionData> & vecCollection_)
{
	//유저 접속시 칭호 미션 서브 정보 리턴
	/*
	L"{ call dbo.TCGATitleCollectionSub_select_20130516 ( %d, %d ) }"

	{ call dbo.TCGATitleCollectionSub_select_20130516 ( @01, @02 ) }
	@01: @01iLoginUID_input int
	@02: @02iCharType_input int

	n return ( @01, @02, @03 )
	@01: MainMissionID int
	@02: SubMissionID  int
	@03: CNT           int
	*/

	DBJIF(m_kODBC.Query(L"{ call dbo.TCGATitleCollectionSub_select_20130516 ( %d, %d ) }", dwUID_, cCharType_));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		DWORD dwMissionID = 0;
		std::pair<int, int> prComplete;
		VERIFY_QUERY(m_kODBC >> dwMissionID
			>> prComplete.first
			>> prComplete.second);

		std::vector<KCollectionData>::iterator vitCollection;
		vitCollection = std::find_if(vecCollection_.begin(), vecCollection_.end(), boost::bind(&KCollectionData::m_dwMissionID, _1) == dwMissionID);
		if (vitCollection != vecCollection_.end()) {
			vitCollection->m_mapCompletionInfo.insert(prComplete);
			vitCollection->m_mapSubInitInfo.insert(prComplete);
		}
	}
}

void KGSDBThread::UpdateCollectionData(IN const DWORD dwUID_, IN const char& cCharType_, IN const std::vector<KCollectionData> & vecCollectionSlot_)
{
	std::vector<KCollectionData>::const_iterator vit;
	for (vit = vecCollectionSlot_.begin(); vit != vecCollectionSlot_.end(); ++vit) {
		if (vit->m_dwState == 2) {
			continue;
		}

		std::map<int, int>::const_iterator mit;
		std::map<int, int>::const_iterator cmitInit;
		for (mit = vit->m_mapCompletionInfo.begin(); mit != vit->m_mapCompletionInfo.end(); ++mit) {
			cmitInit = vit->m_mapSubInitInfo.find(mit->first);
			if (cmitInit == vit->m_mapSubInitInfo.end()) {
				UpdateUserCollection(dwUID_, cCharType_, vit->m_dwMissionID, mit->first, mit->second);
				continue;
			}

			if (cmitInit->second >= mit->second) {
				continue;
			}

			int nDiffCount = 0;
			if (mit->second > 0) {
				nDiffCount = mit->second - cmitInit->second;
			}

			UpdateUserCollection(dwUID_, cCharType_, vit->m_dwMissionID, mit->first, nDiffCount);
		} // vit->m_mapCompletionInfo for문
	}
}

bool KGSDBThread::UpdateStrengthItemReward(IN const DWORD & dwUID_,
	IN OUT KItem & StrengthItem_,
	IN OUT KItem & EquipItem_,
	IN std::list<std::pair<int, float>> & listAttributeList_,
	IN const char cCharType_)
{
	std::list<std::pair<int, float>>::iterator litAtt;
	int nOK = m_kHelper.UpdateStrengthItemInfo(dwUID_, StrengthItem_.m_ItemUID, StrengthItem_.m_cEnchantLevel, EquipItem_.m_ItemUID, StrengthItem_.m_cGradeID, cCharType_);

	if (nOK != 0) {
		START_LOG(cerr, L" 강화석 장착시 UserUID : " << dwUID_)
			<< BUILD_LOG(nOK)
			<< BUILD_LOG(StrengthItem_.m_ItemUID)
			<< BUILD_LOG(EquipItem_.m_ItemUID) << END_LOG;
		SET_ERROR(ERR_KAIRO_06);
		return false;
	}

	// 강화석 랜덤 스킬 적용.
	if (StrengthItem_.m_cGradeID > KItem::GRADE_NORMAL) {
		int i;
		bool bUpdateSuccess = true;
		StrengthItem_.m_vecAttribute.clear();
		for (i = 0, litAtt = listAttributeList_.begin(); litAtt != listAttributeList_.end(); ++litAtt, ++i) {
			LIF(bUpdateSuccess = bUpdateSuccess && m_kHelper.MergeAttribute(KStrengthManager::SU_UPDATE, dwUID_, i, litAtt->first, litAtt->second, StrengthItem_, cCharType_));
		}
		if (!bUpdateSuccess) {
			START_LOG(cerr, L"강화석 스킬 장착 실패") << END_LOG;
		}
	}
	return true;
}

void KGSDBThread::Character_gameinfo_select(IN const DWORD & dwUserUID_, IN OUT std::map<char, KCharacterInfo> & mapCharacterInfo_)
{
	/*
	L"{ call dbo.CIGACharacterInfo_select_20130326 ( %d ) }"

	{ call dbo.CIGACharacterInfo_select_20130326 ( @01 ) }
	@01: @01iLoginUID_input int

	n return ( @01, @02, @03, @04, @05, @06, @07, @08, @09, @10, @11, @12 )
	@01: SlotNo       int
	@02: CharType     tinyint
	@03: Promotion    tinyint
	@04: ExpS4        bigint
	@05: Win          int
	@06: Lose         int
	@07: Level        int
	@08: Exp          bigint
	@09: GamePoint    bigint
	@10: DefaultBonus int
	@11: SpecialBonus int
	@12: InvenSize    int
	@13: CoordiSize   int
	*/

	DBJIF(m_kODBC.Query(L"{ call dbo.CIGACharacterInfo_select_20130326 ( %d ) }", dwUserUID_));

	while (m_kODBC.Fetch() != SQL_NO_DATA)
	{
		KCharacterInfo  kCharacterInfo;
		DWORD dwLevel = 0;

		VERIFY_QUERY(m_kODBC
			>> kCharacterInfo.m_dwSlotNum
			>> kCharacterInfo.m_cCharType
			>> kCharacterInfo.m_cPromotion
			>> kCharacterInfo.m_biExp
			>> kCharacterInfo.m_iWin
			>> kCharacterInfo.m_iLose
			>> dwLevel
			>> kCharacterInfo.m_biTotalExp
			>> kCharacterInfo.m_nGamePoint
			>> kCharacterInfo.m_kBonusPoint.m_nBaseBonus
			>> kCharacterInfo.m_kBonusPoint.m_nSpecialBonus
			>> kCharacterInfo.m_nInvenCapacity
			>> kCharacterInfo.m_nLookInvenCapacity);

		kCharacterInfo.m_biExp = std::min<__int64>(SiKResultManager()->GetMaxExp(), kCharacterInfo.m_biExp);
		kCharacterInfo.m_nGamePoint = std::min<DWORD>(SiKGSSimLayer()->m_iMaxGamePoint, kCharacterInfo.m_nGamePoint);

		//후처리
		kCharacterInfo.m_biInitExp = kCharacterInfo.m_biExp;
		kCharacterInfo.m_iInitWin = kCharacterInfo.m_iWin;
		kCharacterInfo.m_iInitLose = kCharacterInfo.m_iLose;
		kCharacterInfo.m_kEquipPetInfo.Init();
		kCharacterInfo.m_strCharName.clear();
		kCharacterInfo.m_dwLevel = SiKResultManager()->Exp_2_Level(kCharacterInfo.m_biExp);
		kCharacterInfo.m_nInitGP = kCharacterInfo.m_nGamePoint;

		kCharacterInfo.m_biInitTotalExp = kCharacterInfo.m_biTotalExp;
		SiKSkillManager()->InitSkillInfo(kCharacterInfo.m_kSkillInfo, kCharacterInfo.m_cCharType, kCharacterInfo.m_cPromotion, kCharacterInfo.m_dwLevel); // 스킬 정보 얻어 와야 한다.
		SiKSkillManager()->InitSkillSetting(kCharacterInfo.m_cPromotion, kCharacterInfo.m_mapEquipSkillSet);

		//읽어오는 것이 완료 되었다면 ack에 넣어주자.
		mapCharacterInfo_[kCharacterInfo.m_cCharType] = kCharacterInfo;

		START_LOG(clog, L"캐릭터 정보 DB 불러오기.. LoginUID : " << dwUserUID_)
			<< BUILD_LOG(kCharacterInfo.m_dwSlotNum)
			<< BUILD_LOGc(kCharacterInfo.m_cCharType)
			<< BUILD_LOGc(kCharacterInfo.m_cPromotion)
			<< BUILD_LOG(kCharacterInfo.m_biExp)
			<< BUILD_LOG(kCharacterInfo.m_iWin)
			<< BUILD_LOG(kCharacterInfo.m_iLose)
			<< BUILD_LOG(dwLevel)
			<< BUILD_LOG(kCharacterInfo.m_dwLevel)
			<< BUILD_LOG(kCharacterInfo.m_biTotalExp)
			<< BUILD_LOG(kCharacterInfo.m_nGamePoint)
			<< BUILD_LOG(kCharacterInfo.m_nInvenCapacity)
			<< BUILD_LOG(kCharacterInfo.m_nLookInvenCapacity)
			<< BUILD_LOG(m_kODBC.m_strLastQuery)
			<< END_LOG;
	}
}

void KGSDBThread::GetSkillSlotOpenInfo(IN const DWORD & dwUID_, OUT std::map<char, int> & mapSlotOpenInfo_)
{
	/*
	L"{ call dbo.STGASkillTreeSetSlotIndex_select_20120305 ( %d ) }"

	{ call dbo.STGASkillTreeSetSlotIndex_select_20120305 ( @1 ) }
	@1: @iLoginUID_ int

	n return ( @1, @2 )
	@1: CharType     tinyint
	@2: MaxSlotIndex int
	*/
	mapSlotOpenInfo_.clear();
	DBJIF(m_kODBC.Query(L"{ call dbo.STGASkillTreeSetSlotIndex_select_20120305 ( %d ) }", dwUID_));
	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		int nCharType;
		int nSlotIndex;

		VERIFY_QUERY(m_kODBC >> nCharType
			>> nSlotIndex);

		if (!mapSlotOpenInfo_.insert(std::make_pair(nCharType, nSlotIndex)).second) {
			START_LOG(cerr, L"Char Type 존재, Char : " << nCharType)
				<< BUILD_LOG(nSlotIndex)
				<< END_LOG;
		}
	}

	START_LOG(clog, L"유저의 SlotIndex 오픈정보 얻어오기. Size:" << mapSlotOpenInfo_.size())
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< END_LOG;;
}

bool KGSDBThread::UpdateCharSlotIndex(IN const DWORD & dwUID_, IN const char& cCharType_, IN const int& nAddSlotIndex_)
{
	/*
	L"{ call dbo.STGASkillTreeSetSlotIndex_merge_20120305 ( %d, %d, %d ) }"

	{ call dbo.STGASkillTreeSetSlotIndex_merge_20120305 ( @1, @2, @3 ) }
	@1: @iLoginUID_     int
	@2: @iCharType_     tinyint
	@3: @iMaxSlotIndex_ int

	1 return ( @1 )
	@1: OK int
		0       : 성공
		-1      : 계정이 존재하지 않음
		-101이하: 트랜잭션 에러
	*/
	DBJIF2(m_kODBC.Query(L"{ call dbo.STGASkillTreeSetSlotIndex_merge_20120305 ( %d, %d, %d ) }",
		dwUID_,
		(int)cCharType_,
		nAddSlotIndex_));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();
	}

	if (nOK != 0) {
		START_LOG(cerr, L" 유저의 슬롯 오픈 정보 갱신 에러 : " << nOK)
			<< BUILD_LOG(dwUID_)
			<< BUILD_LOGc(cCharType_)
			<< BUILD_LOG(nAddSlotIndex_)
			<< BUILD_LOG(m_kODBC.m_strLastQuery)
			<< END_LOG;
		return false;
	}
	return true;
}

bool KGSDBThread::DungeonCharStat_insert(IN const KEVENT_STAT_DUNGEON_CHARACTER_NOT & vecList_)
{
	/*
	L"{ call dbo.CDGACharacterDungeon_insert_20140123 ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }"

	{ call dbo.CDGACharacterDungeon_insert_20140123 ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10, @11, @12, @13, @14, @15, @16 ) }
	@1 : @iLoginUID_input                   int
	@2 : @iCharType_input                   int
	@3 : @iLevel_input                      int
	@4 : @iDungeonID_input                  int
	@5 : @iDifficulty_input                 int
	@6 : @iPlayTime_input                   int
	@7 : @iNomalAttack_input                int
	@8 : @iDefence_input                    int
	@9 : @iVitality_input                   int
	@10: @iSpecialAttack_input              int
	@11: @iMPRecoveryRate_input             int
		원본값: @11 / 1000
	@12: @iMPRecoveryCNT_input              int
		원본값: @12 / 1000
	@13: @iMPAbsorbRate_input               int
	@14: @iTotalAttackPower_input           int
	@15: @iCriticalProbability_input        int
		원본값: @15 / 1000
	@16: @iCriticalDamageGrowthRate_input   int
		원본값: @16 / 1000

	1 return ( @1 )
	@1: OK
		0       : 성공
		-101이하: 트랜젝션 에러
	*/

	KEVENT_STAT_DUNGEON_CHARACTER_NOT::const_iterator cvit;
	for (cvit = vecList_.begin(); cvit != vecList_.end(); ++cvit) {
		int nMPRecoveryRate = static_cast<int>(cvit->m_fMPRecoveryRate * 1000);
		int nCounterResistRate = static_cast<int>(cvit->m_fCounterResistRate * 1000);
		int nCriticalAttackRate = static_cast<int>(cvit->m_fCriticalAttackRate * 1000);
		int nCriticalDamageRate = static_cast<int>(cvit->m_fCriticalDamageRate * 1000);
		DBJIF2(m_kODBC.Query(L"{ call dbo.CDGACharacterDungeon_insert_20140123 ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }",
			cvit->m_dwLoginUID,
			cvit->m_dwCharType,
			cvit->m_dwLevel,
			cvit->m_dwDungeonID,
			cvit->m_dwDifficult,
			cvit->m_dwPlayTime,
			cvit->m_dwNormalAttack,
			cvit->m_dwDefence,
			cvit->m_dwVitality,
			cvit->m_dwSpecialAttack,
			nMPRecoveryRate,
			nCounterResistRate,
			cvit->m_dwMPDrain,
			cvit->m_dwTotalAttack,
			nCriticalAttackRate,
			nCriticalDamageRate));

		int nRet = -99;
		if (m_kODBC.BeginFetch()) {
			VERIFY_QUERY(m_kODBC >> nRet);
			m_kODBC.EndFetch();
		}

		_LOG_SUCCESS(nRet == 0, L"던전 스테이지 통계.. Result : " << nRet)
			<< BUILD_LOG(m_kODBC.m_strLastQuery)
			<< END_LOG;
	}
	return true;
}

bool KGSDBThread::UserDungeonCollect_insert(IN const KDB_EVENT_USER_DUNGEON_STAT_NOT & vecInfo_)
{
	/*
		특정 던전의 계정 별 로그 데이터
		L"{ call dbo.DCCODungeonCollect_Insert ( %d, %d, %d, %d, %d, %d, %d, %d ) }"

		{ call dbo.DCCODungeonCollect_Insert ( @1, @2, @3, @4, @5, @6, @7, @8 ) }
		@1 ; @iModeID_     int
		@2 ; @iDifficult_  int
		@3 ; @iLoginUID_   int
		@4 ; @iPlayTime_   int
		@5 ; @iParty_      int // 파티원수
		@6 ; @iCharType_   int
		@7 ; @iLevel_      int
		@8 ; @iResult_     int

		1 return ( @1 )
		@1 ; @iOK int
		0        ; 성공
		-101이하: 트랜잭션에러
	*/

	KDB_EVENT_USER_DUNGEON_STAT_NOT::const_iterator cvit;
	for (cvit = vecInfo_.begin(); cvit != vecInfo_.end(); ++cvit) {
		DBJIF2(m_kODBC.Query(L"{ call dbo.DCCODungeonCollect_Insert ( %d, %d, %d, %d, %d, %d, %d, %d ) }",
			cvit->m_nModeID,
			cvit->m_nDifficult,
			cvit->m_dwLoginUID,
			cvit->m_dwPlayTime,
			cvit->m_nTotalUserCount,
			static_cast<int>(cvit->m_cCharType),
			cvit->m_dwLevel,
			cvit->m_dwClearType));

		int nRet = -99;
		if (m_kODBC.BeginFetch()) {
			VERIFY_QUERY(m_kODBC >> nRet);
			m_kODBC.EndFetch();
		}

		_LOG_SUCCESS(nRet == 0, L"유저별 던전 통계.. Result : " << nRet)
			<< BUILD_LOG(m_kODBC.m_strLastQuery)
			<< END_LOG;
	}
	return true;
}

void KGSDBThread::GetRankInfo(IN const DWORD & dwUserUID_, IN const int& nTabType_, OUT KMyRankInfo & kMyRankInfo_)
{
	/*
	L"{ call dbo.RGGARankGrandchaserLoginUID_select_20120404 ( %d, %d ) }"

	{ call dbo.RGGARankGrandchaserLoginUID_select_20120404 ( @1, @2 ) }
	@1: @iLoginUID_ int
	@2: @iTabType_  tinyint = 0

	n return ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10, @11, @12, @13 )
	@1 : Type      int
	1: 일간랭킹
	2: 주간랭킹
	3: 월간랭킹
	4: 경험치누적랭킹
	@2 : TabType   tinyint
	@3 : [Rank]    int
	@4 : LoginUID  int
	@5 : GuildID   int
	@6 : GuildMark int
	@7 : GuildName nvarchar( 12 )
	@8 : CharType  tinyint
	@9 : Nick      nvarchar( 24 )
	@10: Win       int
	Type이 1, 2, 3일 경우 유효
	@11: Lose      int
	Type이 1, 2, 3일 경우 유효
	@12: WinRate   float
	Type이 1, 2, 3일 경우 유효
	@13: [Exp]     bigint
	Type이 4일 경우 유효
	*/

	DBJIF(m_kODBC.Query(L"{ call dbo.RGGARankGrandchaserLoginUID_select_20120404 ( %d, %d ) }",
		dwUserUID_, nTabType_ + 1));
	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		KWinRateRank kRankData;
		KExpRank kExpRankDataExp;

		int nRevision = 0;
		std::wstring strExt;

		VERIFY_QUERY(m_kODBC >> kRankData.m_cRankType
			>> kRankData.m_cTabType
			>> kRankData.m_nRank
			>> kRankData.m_dwLoginUID
			>> kRankData.m_dwGuildUID
			>> kRankData.m_nGuildMark
			>> kRankData.m_strGuildName
			>> kRankData.m_cCharType
			>> kRankData.m_strNick
			>> kRankData.m_dwWin
			>> kRankData.m_dwLose
			>> kRankData.m_fWinRate
			>> kRankData.m_biExp
			>> nRevision
			>> strExt);

		SiKGuildManager()->GetGuildMarkStringName(kRankData.m_dwGuildUID, kRankData.m_strMarkName);

		if (kRankData.m_cTabType != -1) {
			if (kRankData.m_cRankType == KGSDBThread::RT_DAILY) {
				kMyRankInfo_.m_mapDailyRank[nTabType_] = kRankData;
			}
			else if (kRankData.m_cRankType == KGSDBThread::RT_WEEKLY) {
				kMyRankInfo_.m_mapWeeklyRank[nTabType_] = kRankData;
			}
			else if (kRankData.m_cRankType == KGSDBThread::RT_MONTHLY) {
				kMyRankInfo_.m_mapMonthlyRank[nTabType_] = kRankData;
			}
			else if (kRankData.m_cRankType == KGSDBThread::RT_TOTALEXP) {
				kExpRankDataExp.m_nRank = kRankData.m_nRank;
				kExpRankDataExp.m_dwLoginUID = kRankData.m_dwLoginUID;
				kExpRankDataExp.m_nGuildID = static_cast<int>(kRankData.m_dwGuildUID);
				kExpRankDataExp.m_nGuildMark = kRankData.m_nGuildMark;
				kExpRankDataExp.m_strGuildName = kRankData.m_strGuildName;
				kExpRankDataExp.m_cCharType = kRankData.m_cCharType;
				kExpRankDataExp.m_strNick = kRankData.m_strNick;
				kExpRankDataExp.m_biExp = kRankData.m_biExp;
				kExpRankDataExp.m_strMarkName = kRankData.m_strMarkName;
				kMyRankInfo_.m_mapExpRank[nTabType_] = kExpRankDataExp;
			}
		}
	}
	m_kODBC.EndFetch();
}

bool KGSDBThread::DungeonCharSkillCount_insert(IN const DWORD dwUID_, IN const KEVENT_STAT_CHARACTER_SKILL_COUNT & prData_)
{
	/*
	L"{ call dbo.HSGAHybridSkill_insert_20120424 ( %d, %d, %d ) }"

	{ call dbo.HSGAHybridSkill_insert_20120424 ( @1, @2, @3 ) }
	@1 : @iLoginUID_    int
	@2 : @iShortcutCNT_ int --단축키
	@3 : @iChargingCNT_ int --차징

	1 return ( @1 )
	@1: OK
		0       : 성공
		-101이하: 트랜젝션 에러
	*/

	DBJIF2(m_kODBC.Query(L"{ call dbo.HSGAHybridSkill_insert_20120424 ( %d, %d, %d ) }",
		dwUID_,
		prData_.first,
		prData_.second));

	int nRet = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nRet == 0, L"유저 스킬 Cnt 통계.. Result : " << nRet)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< END_LOG;

	return (nRet == 0);
}

void KGSDBThread::GetMultiEventUserData(IN const DWORD & dwUID_, IN const DWORD & dwType_, IN const DWORD & dwVersion_,
	OUT DWORD & dwPoint_, OUT DWORD & dwGrade_, OUT DWORD & dwDuration_, OUT DWORD & dwSuppliedDuration_, OUT int& nAccTime_)
{
	/*
	L"{ call dbo.MEGAMultiEvent_select ( %d, %d, %d ) }"

	{ call dbo.MEGAMultiEvent_select ( @1, @2, @3 ) }
	@1: @i유저_ int
	@2: @i타입_ int
	@3: @i버전_ int

	1 return ( @1, @2 )
	@1: Point        int
	@2: Compensation int
	@3: ItemCount    int
	@4: Provide      int
	@5: CheckTime    int
	*/

	DBJIF(m_kODBC.Query(L"{ call dbo.MEGAMultiEvent_select  ( %d, %d, %d ) }",
		dwUID_, dwType_, dwVersion_));

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> dwPoint_
			>> dwGrade_
			>> dwDuration_
			>> dwSuppliedDuration_
			>> nAccTime_);
	}
	m_kODBC.EndFetch();

	START_LOG(clog, L"멀티 이벤트 유저 정보 가져오기 UserUID: " << dwUID_)
		<< BUILD_LOG(dwPoint_)
		<< BUILD_LOG(dwGrade_)
		<< BUILD_LOG(dwDuration_)
		<< BUILD_LOG(dwSuppliedDuration_)
		<< BUILD_LOG(nAccTime_)
		<< END_LOG;
}

bool KGSDBThread::UpdateMultiEventUserData(IN const DWORD & dwUID_, IN const DWORD & dwType_, IN const DWORD & dwVersion_,
	IN const DWORD & dwPointDiff_, IN const DWORD & dwGradeDiff_, IN const int& nDurationDiff_,
	IN const DWORD dwSuppliedDuration_, IN const int& nAccTime_)
{
	/*
	L"{ call dbo.MEGAMultiEvent_merge ( %d, %d, %d, %d, %d ) }"

	{ call dbo.MEGAMultiEvent_merge ( @1, @2, @3, @4, @5 ) }
	@1: @iLoginuid_         int
	@2: @iType_             int
	@3: @iVersion_          int
	@4: @iPointDiff_        int
	@5: @iCompensationDiff_ int
	@6: @iItemCountDiff_    int
	@7: @iProvide_          int
	@8: @iCheckTime_        int

	1 return ( @1 )
	@1: OK int
		0       : 성공
		-1      : 유저 정보가 없음
		-101이하: 트랜잭션 에러
	*/

	DBJIF2(m_kODBC.Query(L"{ call dbo.MEGAMultiEvent_merge ( %d, %d, %d, %d, %d, %d, %d, %d ) }",
		dwUID_,
		dwType_,
		dwVersion_,
		dwPointDiff_,
		dwGradeDiff_,
		nDurationDiff_,
		dwSuppliedDuration_,
		nAccTime_));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nOK == 0, L"멀티 이벤트 유저 정보 갱신 Result : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< END_LOG;

	return true;
}

bool KGSDBThread::CheckGradeReward(IN const DWORD & dwUID_, IN const DWORD & dwType_, IN const DWORD & dwVersion_, IN const DWORD & dwGrade_)
{
	/*
	L"{ call dbo.MEGAMultiEvent_CheckReward ( %d, %d, %d, %d ) }"

	{ call dbo.MEGAMultiEvent_CheckReward ( @1, @2, @3, @4 ) }
	@1: @i유저_     int
	@2: @i타입_     int
	@3: @i버전_     int
	@4: @i등급_     int

	1 return ( @1 )
	@1: OK int
		0       : 성공
		-101이하: 트랜잭션 에러
	*/

	DBJIF2(m_kODBC.Query(L"{ call dbo.MEGAMultiEvent_CheckReward ( %d, %d, %d, %d ) }",
		dwUID_,
		dwType_,
		dwVersion_,
		dwGrade_));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nOK == 0, L"송크란 이벤트 유저 등급 정보 체크 : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< END_LOG;

	return (nOK == 0);
}

bool KGSDBThread::UpdateWateBombCount(IN const DWORD & dwUID_, IN const DWORD & dwType_, IN const DWORD & dwVersion_, IN const DWORD & dwItemCountDiff_)
{
	// 물풍선 개수 변경.
	/*
	L"{ call dbo.MEGAMultiEvent_ItemCount_update ( %d, %d, %d, %d ) }"

	{ call dbo.MEGAMultiEvent_ItemCount_update ( @1, @2, @3, @4 ) }
	@1: @iLoginuid_         int
	@2: @iType_             int
	@3: @iVersion_          int
	@4: @iItemCountDiff_    int

	1 return ( @1 )
	@1: OK int
		0       : 성공
		-1      : 유저 정보가 없음
		-2      : 유저 이벤트 정보 없음
		-101이하: 트랜잭션 에러
	*/

	DBJIF2(m_kODBC.Query(L"{ call dbo.MEGAMultiEvent_ItemCount_update ( %d, %d, %d, %d ) }",
		dwUID_,
		dwType_,
		dwVersion_,
		dwItemCountDiff_));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nOK == 0, L"송크란 이벤트 유저 풍선 갯수 정보 변경 : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< END_LOG;

	return (nOK == 0);
}

bool KGSDBThread::UpdateSongkranWaterBombUseCount(IN const DWORD & dwUID_, IN const DWORD & dwType_, IN const DWORD & dwVersion_, IN const DWORD & dwUseCount_)
{
	/*
	L"{ call dbo.MEGAMultiEventThrowCount_insert ( %d, %d, %d, %d ) }"

	{ call dbo.MEGAMultiEventThrowCount_insert ( @1, @2, @3, @4 ) }
	@1: @iLoginuid_     int
	@2: @iType_         int
	@3: @iVersion_      int
	@4: @iThrowCount_   int

	1 return ( @1 )
	@1: OK int
		0       : 성공
		-101이하: 트랜잭션 에러
	*/
	DBJIF2(m_kODBC.Query(L"{ call dbo.MEGAMultiEventThrowCount_insert ( %d, %d, %d, %d ) }",
		dwUID_,
		dwType_,
		dwVersion_,
		dwUseCount_));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nOK == 0, L"송크란 이벤트 물풍 사용 횟수 기록 : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< END_LOG;

	return (nOK == 0);
}

bool KGSDBThread::UpdateSongkranWaterBombSuppliedCount(IN const DWORD & dwUID_, IN const DWORD & dwType_, IN const DWORD & dwVersion_, IN const DWORD & dwProvide_)
{
	/*
	L"{ call dbo.MEGAMultiEventTodayProvide_insert ( %d, %d, %d, %d ) }"

	{ call dbo.MEGAMultiEventTodayProvide_insert ( @1, @2, @3, @4 ) }
	@1: @iLoginuid_     int
	@2: @iType_         int
	@3: @iVersion_      int
	@4: @iTodayProvide_ int

	1 return ( @1 )
	@1: OK int
		0       : 성공
		-101이하: 트랜잭션 에러
	*/

	DBJIF2(m_kODBC.Query(L"{ call dbo.MEGAMultiEventTodayProvide_insert ( %d, %d, %d, %d ) }",
		dwUID_,
		dwType_,
		dwVersion_,
		dwProvide_));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nOK == 0, L"송크란 이벤트 물풍 지급 개수 기록 : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< END_LOG;

	return (nOK == 0);
}

bool KGSDBThread::InsertMultiEventGift(IN const DWORD & dwUID_, IN const DWORD & dwType_, IN const DWORD & dwVersion_, IN const DWORD & dwReceiveUID_, IN const DWORD & dwItemID_)
{
	/*
	L"{ call dbo.MEGAMultiEventGift_insert ( %d, %d, %d, %d, %d ) }"

	{ call dbo.MEGAMultiEventGift_insert ( @1, @2, @3, @4, @5 ) }
	@1: @i유저_           int
	@2: @i타입_           int
	@3: @i버전_           int
	@4: @i누구_           int
	@5: @i선물한아이템ID_ int

	1 return ( @1 )
	@1: OK int
		0       : 성공
		-101이하: 트랜잭션 에러
	*/
	DBJIF2(m_kODBC.Query(L"{ call dbo.MEGAMultiEventGift_insert ( %d, %d, %d, %d, %d ) }",
		dwUID_,
		dwType_,
		dwVersion_,
		dwReceiveUID_,
		dwItemID_));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nOK == 0, L"멀티 이벤트 선물 주기 Result : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< END_LOG;

	return true;
}

bool KGSDBThread::UpdateMultiEventDonationInfo(IN const DWORD & dwUID_, IN const DWORD & dwType_, IN const DWORD & dwVersion_, IN const DWORD & dwDonationCnt_)
{
	/*
	L"{ call dbo.MEGAMultiEventDonation_insert ( %d, %d, %d, %d ) }"

	{ call dbo.MEGAMultiEventDonation_insert ( @1, @2, @3, @4 ) }
	@1: @i유저_     int
	@2: @i타입_     int
	@3: @i버전_     int
	@4: @i기부개수_ int

	1 return ( @1 )
	@1: OK int
		0       : 성공
		-101이하: 트랜잭션 에러
	*/

	DBJIF2(m_kODBC.Query(L"{ call dbo.MEGAMultiEventDonation_insert ( %d, %d, %d, %d ) }",
		dwUID_,
		dwType_,
		dwVersion_,
		dwDonationCnt_));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nOK == 0, L"멀티 이벤트 기부 정보 갱신 Result : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< END_LOG;

	return true;
}

bool KGSDBThread::CheckUpdateClearData(IN const KDungeonClearData & kDungeonClearData_)
{
	if (kDungeonClearData_.m_bClear == false && kDungeonClearData_.m_shMaxDifficulty == 0 &&
		kDungeonClearData_.m_shLastDifficulty == 0 && kDungeonClearData_.m_usLeftRewardCount == 0) {
		return false;
	}
	return true;
}


bool KGSDBThread::InsertShanghaiPost(IN const DWORD dwLoginUID_, IN const char& cCharType_, IN const KDropItemInfo & kDropItem_)
{
	// 우편 메세지 만들기
	KPostItemInfo kInfo;
	kInfo.m_dwToUserUID = dwLoginUID_;
	kInfo.m_cScriptType = KPostItemInfo::LT_SYSTEM;
	kInfo.m_nGamePoint = 0;
	kInfo.m_vecItem.clear();

	kInfo.m_strFromNickName = SiKPostConfig()->GetPostString(L"str_00");
	kInfo.m_strTitle = SiKPostConfig()->GetPostString(L"str_03");
	kInfo.m_strMessage = SiKPostConfig()->GetPostString(L"str_04");;
	kInfo.m_cCharType = cCharType_;

	KItem kItem;
	kItem.m_ItemID = kDropItem_.m_ItemID;
	kItem.m_nCount = kDropItem_.m_nDuration;
	kItem.m_nPeriod = kDropItem_.m_nPeriod;
	kItem.m_cEnchantLevel = 0;
	kItem.m_cGradeID = -1;
	kInfo.m_vecItem.push_back(kItem);

	// 우편 지급
	if (false == InsertSystemPost(kInfo)) {
		START_LOG(cerr, L"우편으로 상하이 드랍 DB처리 실패.. LoginID : " << dwLoginUID_ << " ItemID : " << kDropItem_.m_ItemID)
			<< END_LOG;
		return false;
	}

	return true;
}

bool KGSDBThread::ViewGradeTime_insert(IN const KUserViewGradeTime & kUserViewGradeTime_)
{
	/*
	L"{ call dbo.VRGAViewRange_insert_20120409 ( %d, %d, %d, %f, %d ) }"

	{ call dbo.VRGAViewRange_insert_20120409 ( @1, @2, @3, @4, @5 ) }
	@1 : @iLoginUID_  int
	@2 : @iCharType_  int
	@3 : @iDungeonID_ int
	@4 : @iViewRange_ float
	@5 : @iPlayTime_  int

	1 return ( @1 )
	@1: OK
		0       : 성공
		-101이하: 트랜젝션 에러
	*/

	std::map<float, DWORD>::const_iterator cmit;
	for (cmit = kUserViewGradeTime_.m_mapViewGradeTime.begin(); cmit != kUserViewGradeTime_.m_mapViewGradeTime.end(); ++cmit) {
		DBJIF2(m_kODBC.Query(L"{ call dbo.VRGAViewRange_insert_20120409 ( %d, %d, %d, %f, %d ) }",
			kUserViewGradeTime_.m_dwUserUID,
			kUserViewGradeTime_.m_dwCharType,
			kUserViewGradeTime_.m_dwDungeonID,
			cmit->first,
			cmit->second));

		int nRet = -99;
		if (m_kODBC.BeginFetch()) {
			VERIFY_QUERY(m_kODBC >> nRet);
			m_kODBC.EndFetch();
		}

		_LOG_SUCCESS(nRet == 0, L"던전 스테이지 통계.. Result : " << nRet)
			<< BUILD_LOG(m_kODBC.m_strLastQuery)
			<< END_LOG;
	}
	return true;
}


bool KGSDBThread::GetAdventureCollectCountInfo(IN const DWORD & dwUserUID_, IN const int& nVersion_, OUT std::map< int, std::map< DWORD, int > > & mapCollectCountData_)
{
	/*
	GC원정대 던전 전리품 개수 정보 받아오기

	L"{ call dbo.AEGAAdventureEventCollect_Select ( %d, %d ) }"

	{ call dbo.AEGAAdventureEventCollect_Select ( @1, @2 ) }
	@1 ; @iLoginUID_ int
	@2 ; @iVersion_  int

	n return ( @1, @2, @3 )
	@1 ; Continent int
	@2 ; CollectNo int
	@3 ; Count     int
	*/

	mapCollectCountData_.clear();

	DBJIF2(m_kODBC.Query(L"{ call dbo.AEGAAdventureEventCollect_Select ( %d, %d ) }", dwUserUID_, nVersion_));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		int nContinentID = -1;
		PAIR_DWORD_INT prCollectIDnCount;
		prCollectIDnCount.first = 0;
		prCollectIDnCount.second = 0;

		VERIFY_QUERY(m_kODBC >> nContinentID
			>> prCollectIDnCount.first
			>> prCollectIDnCount.second);

		if (!mapCollectCountData_[nContinentID].insert(prCollectIDnCount).second) {
			START_LOG(cerr, L"던전 전리품 개수 정보 가져오기 오류.")
				<< BUILD_LOG(dwUserUID_)
				<< BUILD_LOG(nVersion_)
				<< END_LOG;
			return false;
		}
	}

	START_LOG(clog, L"던전 전리품 개수 정보 가져오기.")
		<< BUILD_LOG(dwUserUID_)
		<< BUILD_LOG(nVersion_)
		<< BUILD_LOG(mapCollectCountData_.size())
		<< END_LOG;

	return true;
}

bool KGSDBThread::GetAdventureContinentRewardInfo(IN const DWORD & dwUserUID_, IN const int& nVersion_, OUT std::map< int, bool > & mapContinentData_)
{
	/*
	GC원정대 대륙 별 보상 정보 조회

	L"{ call dbo.AEGAAdventureEventContinent_Select ( %d, %d) }"

	{ call dbo.AEGAAdventureEventContinent_Select ( @1, @2 ) }
	@1 ; @iLoginUID_  int
	@2 ; @iVersion_   int

	n return ( @1, @2 )
	@1 ; Continent int
	@2 ; Supplied  int
	*/

	mapContinentData_.clear();

	DBJIF2(m_kODBC.Query(L"{ call dbo.AEGAAdventureEventContinent_Select ( %d, %d ) }", dwUserUID_, nVersion_));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {
		std::pair< int, bool > prContinentData;
		prContinentData.first = -1;
		prContinentData.second = false;

		VERIFY_QUERY(m_kODBC >> prContinentData.first
			>> prContinentData.second);

		if (!mapContinentData_.insert(prContinentData).second) {
			START_LOG(cerr, L"대륙 보상 정보 가져오기 오류.")
				<< BUILD_LOG(dwUserUID_)
				<< BUILD_LOG(nVersion_)
				<< END_LOG;
			return false;
		}
	}

	START_LOG(clog, L"대륙 보상 정보 가져오기.")
		<< BUILD_LOG(dwUserUID_)
		<< BUILD_LOG(nVersion_)
		<< BUILD_LOG(mapContinentData_.size())
		<< END_LOG;

	return true;
}
bool KGSDBThread::UpdateContinentUserData(IN const DWORD & dwUserUID_, IN const int& nVersion_, IN const int& nContinentID_, IN const int& nSupplied_)
{
	/*
	GC원정대대륙별보상상태입력/ 수정

	L"{ call dbo.AEGAAdventureEventContinent_merge ( %d, %d, %d, %d ) }"

	{ call dbo.AEGAAdventureEventContinent_merge ( @1, @2, @3, @4 ) }
	@1: @iLoginUID_  int
	@2: @iVersion_   int
	@3: @iContinent_ int
	@4: @iSupplied_  int

	1 return ( @1 )
	@1: OK       int
	0       : 성공
	-1      : 유저가없음
	-101이하: 트랜젝션에러
	*/

	DBJIF2(m_kODBC.Query(L"{ call dbo.AEGAAdventureEventContinent_merge ( %d, %d, %d, %d ) }",
		dwUserUID_,
		nVersion_,
		nContinentID_,
		nSupplied_));

	int nRet = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nRet == 0, L"모험 이벤트 대륙 보상 입력." << nRet)
		<< BUILD_LOG(dwUserUID_)
		<< BUILD_LOG(nVersion_)
		<< BUILD_LOG(nContinentID_)
		<< BUILD_LOG(nSupplied_)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< END_LOG;

	return (nRet == 0);
}

bool KGSDBThread::UpdateModeCountUserData(IN const DWORD & dwUserUID_, IN const int& nVersion_, IN const int& nContinentID_, IN const int& nCollectID_, IN const int& nDiff_)
{
	/*
	GC원정대 던전 전리품 개수 입력/ 수정

	L"{ call dbo.AEGAAdventureEventCollect_merge ( %d, %d, %d, %d, %d ) }"

	{ call dbo.AEGAAdventureEventCollect_merge ( @1, @2, @3, @4, @5 ) }
	@1: @iLoginUID_  int
	@2: @iVersion_   int
	@3: @iContinent_ int
	@4: @iCollectNo_ int
	@5: @iCountDiff_ int

	1 return ( @1, @2 )
	@1: OK       int
	0       : 성공
	-1      : 유저가없음
	-2      : 전리품 개수가 마이너스되므로 취소됨
	-101이하: 트랜젝션에러
	@2: Count int
	*/

	DBJIF2(m_kODBC.Query(L"{ call dbo.AEGAAdventureEventCollect_merge ( %d, %d, %d, %d, %d ) }",
		dwUserUID_,
		nVersion_,
		nContinentID_,
		nCollectID_,
		nDiff_));

	int nRet = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nRet);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(nRet == 0, L"전리품 개수 정보 갱신." << nRet)
		<< BUILD_LOG(dwUserUID_)
		<< BUILD_LOG(nVersion_)
		<< BUILD_LOG(nCollectID_)
		<< BUILD_LOG(nDiff_)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< END_LOG;

	return (nRet == 0);
}

bool KGSDBThread::UpdateAdventureUserData(IN const DWORD & dwUID_, IN const int& nVersion_,
	IN const std::map< int, KContinentRewardInfo > & mapContinentRewardInfo_)
{
	std::map< int, KContinentRewardInfo >::const_iterator mit = mapContinentRewardInfo_.begin();
	for (; mit != mapContinentRewardInfo_.end(); ++mit) {
		UpdateContinentUserData(dwUID_, nVersion_, mit->first, 0);

		std::map< DWORD, KCollectItem >::const_iterator mit2 = mit->second.m_mapCollectItems.begin();
		for (; mit2 != mit->second.m_mapCollectItems.end(); ++mit2) {
			// 결과가 음수가 아닌지 확인이 필요한데...
			int nMaxCount = static_cast<int>(mit2->second.m_dwMaxCount);
			UpdateModeCountUserData(dwUID_, nVersion_, mit->second.m_nContinentID, mit2->second.m_dwCollectID, -nMaxCount);
		}
	}

	return true;
}

void KGSDBThread::GetLevelResetEventUserData(IN const DWORD & dwUID_, IN const DWORD & dwVersion_, IN OUT std::map<char, std::pair<bool, bool>> & mapResetCharInfo_)
{
	/*
	케릭터초기화이벤트특정유저정보조회
	L"{ call dbo.REGACharacterResetEvent_Select ( %d, %d ) }"

	{ call dbo.REGACharacterResetEvent_Select ( @1, @2 ) }
	@1 ; @iVersion_   int
	@2 ; @iLoginUID_ int

	n return ( @1, @2 )
	@1 ; CharType int
	@2 ; Supplied int
	@3 ; ResetType int
	*/


	mapResetCharInfo_.clear();

	DBJIF(m_kODBC.Query(L"{ call dbo.REGACharacterResetEvent_Select ( %d, %d ) }",
		dwVersion_, dwUID_));

	while (m_kODBC.Fetch() != SQL_NO_DATA) {

		int nCharType = 0;
		int nSupplied = 0;
		int nCashType = 0;


		VERIFY_QUERY(m_kODBC >> nCharType
			>> nSupplied >> nCashType);

		bool bSupplied = (nSupplied == 1) ? true : false;
		bool bCashType = (nCashType == 1) ? true : false;

		mapResetCharInfo_.insert(std::make_pair(nCharType, std::make_pair(bCashType, bSupplied)));

	}
	m_kODBC.EndFetch();

	START_LOG(clog, L"레벨 리셋 이벤트 정보 가져오기 UserUID: " << dwUID_)
		<< BUILD_LOG(mapResetCharInfo_.size())
		<< END_LOG;
}


bool KGSDBThread::SetLevelResetEventChar(IN const DWORD & dwUID_, IN const DWORD & dwVersion_, IN const char& cCharType_, IN const bool& bSupplied, IN const bool& bCashType)
{
	/*
	케릭터초기화이벤트특정특정유저정보입력
	L"{ call dbo.REGACharacterResetEvent_Merge ( %d, %d, %d, %d, %d ) }"

	{ call dbo.REGACharacterResetEvent_Merge ( @1, @2, @3, @4, @5 ) }
	@1: @iVersion_   int
	@2: @iLoginUID_ int
	@3: @iCharType_ int
	@4: @iSupplied_ int
	@5: @iResetType_ int -- 0 : GP, 1 : Cash

	1 return ( @1 )
	@1: OK       int
	0       : 성공
	-1      : 유저가없음
	-2      : 중복보상에러
	-901이하: 트랜젝션에러
	*/


	//  bSupplied가 false면 초기화
	//  bSupplied가 true면 보상 받아가는거

	int iCharType = cCharType_;
	int iSupplied = bSupplied ? 1 : 0;
	int iResetType = bCashType ? 1 : 0;

	DBJIF2(m_kODBC.Query(L"{ call dbo.REGACharacterResetEvent_Merge ( %d, %d, %d, %d, %d ) }",
		dwVersion_, dwUID_, iCharType, iSupplied, iResetType));

	int nOK = -99;

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	_LOG_SUCCESS(nOK == 0, L"레벨 초기화 이벤트 적용 OK : " << nOK)
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(iCharType)
		<< BUILD_LOG(iSupplied)
		<< END_LOG;

	return (nOK == 0);
}

bool KGSDBThread::SetLevelResetEventExp(IN const std::wstring & strLogin_, IN const DWORD & dwUID_, IN const char& cCharType_, IN __int64 biDiffExp_)
{
	bool bRet = true;
	// 과다 경험치에 대한 추가코드
	__int64 biExpRewardLimit = SiKResultManager()->GetExpRewardLimit();

	if (biDiffExp_ > biExpRewardLimit) {
		START_LOG(cerr, L"CharExp Over Value. Name : " << strLogin_)
			<< BUILD_LOG(dwUID_)
			<< BUILD_LOG(biDiffExp_)
			<< END_LOG;

		if (SiKResultManager()->IsClippingExpRewardLimit()) {
			biDiffExp_ = biExpRewardLimit;
		}
	}

	int nOK = -99;
	int nWin = 0;
	int nLose = 0;
	__int64 biTotalExp = 0;

	if (biDiffExp_ != 0) { // DB 부하를 줄이기 위해서 값의 변경이 있을때만 해당 sp 호출.
		DBJIF2(m_kODBC.Query(L"{ call dbo.CGGA_character_gameinfo_update (  %d, %d, %I64d, %d, %d, %I64d ) }",
			dwUID_,
			(int)cCharType_,
			biDiffExp_,
			nWin,
			nLose,
			biTotalExp));

		if (m_kODBC.BeginFetch()) {
			VERIFY_QUERY(m_kODBC >> nOK);
			m_kODBC.EndFetch();
			if (nOK != 0) bRet = false;
		}
	}
	else {
		nOK = 0;
	}

	_LOG_SUCCESS(nOK == 0, L"캐릭터 기록 Name : " << strLogin_)
		<< BUILD_LOG(nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< BUILD_LOGc(cCharType_)
		<< BUILD_LOG(biDiffExp_)
		<< END_LOG;

	return bRet;
}

void KGSDBThread::GetSealedCharInfo(IN const DWORD & dwUserUID_, OUT std::map< int, KSealedCharInfo > & kSealedCharInfo_)
{
	std::set< int > setSealedChar;
	CharacterSeal_Select(dwUserUID_, setSealedChar);

	// 보유캐릭터가 아닌 전체 캐릭터의 봉인 정보를 전달하기 위해 빈 데이터를 채워준다
	kSealedCharInfo_.clear();
	for (int i = 0; i < GC_CHAR_NUM; ++i) {
		KSealedCharInfo kSealedCharInfo;
		kSealedCharInfo.m_nCharType = i;
		kSealedCharInfo.m_bIsSealed = false;
		kSealedCharInfo.m_bIsEventTerm = false;
		kSealedCharInfo.m_nObjectLevel = 0;
		kSealedCharInfo.m_tmObjectTime = 0;

		kSealedCharInfo_.insert(std::make_pair(i, kSealedCharInfo));
	}

	// DB에서 가져온 봉인상태인 캐릭터 중
	// 이벤트가 진행중이 아니면 봉인 정보를 true로 전달,
	// 현재 해당 캐릭터의 이벤트가 진행중이면 봉인 정보를 false로 전달한다
	std::set< int >::iterator sit = setSealedChar.begin();
	for (; sit != setSealedChar.end(); ++sit) {

		int nCharType_ = *sit;

		std::map< int, KSealedCharInfo >::iterator mit;
		mit = kSealedCharInfo_.find(nCharType_);
		if (mit == kSealedCharInfo_.end()) {
			START_LOG(cerr, L"?? ??? ?? ?? ? DB? ?? ????? ???? ?? ??? ?? ??. charType: " << nCharType_)
				<< END_LOG;
			continue;
		}

		if (false == SiKSealedChar()->IsEventTerm(nCharType_)) {
			kSealedCharInfo_[nCharType_].m_bIsSealed = true;
		}
		else {
			kSealedCharInfo_[nCharType_].m_bIsEventTerm = true;

			if (false == SiKSealedChar()->GetObjectLevel(nCharType_, kSealedCharInfo_[nCharType_].m_nObjectLevel)) {
				START_LOG(clog, L"?? ??? ?? ?? ?? ? ?? ?? ???? ??" << (nCharType_)) << END_LOG;
			}
			if (false == SiKSealedChar()->GetObjectTime(nCharType_, kSealedCharInfo_[nCharType_].m_tmObjectTime)) {
				START_LOG(clog, L"?? ??? ?? ?? ?? ? ?? ?? ???? ??" << (nCharType_)) << END_LOG;
			}
		}
	}

	START_LOG(clog, L"봉인 캐릭터 정보 패킷 설정 완료") << END_LOG;
}

void KGSDBThread::CharacterSeal_Select(IN const DWORD & dwUserUID_, OUT std::set< int > & setSealedChar_)
{
	/*
	캐릭터 봉인 정보 검색
	L"{ call dbo.CSGACharacterSeal_Select ( %d ) }"

	{ call dbo.CSGACharacterSeal_Select ( @1 ) }
	@1: @iLoginUID_   int

	n return ( @1, @2, @3 )
	@1: LoginUID  int
	@2: CharType  tinyint
	@3: Seal      tinyint
	*/

	setSealedChar_.clear();

	DBJIF(m_kODBC.Query(L"{ call dbo.CSGACharacterSeal_Select ( %d ) }", dwUserUID_));

	while (m_kODBC.Fetch() != SQL_NO_DATA)
	{
		DWORD dwLoginUID = 0;
		int nCharType = 0;
		int nSealed = 0;

		VERIFY_QUERY(m_kODBC
			>> dwLoginUID
			>> nCharType
			>> nSealed);

		if (false == setSealedChar_.insert(nCharType).second) {
			START_LOG(cerr, L"DB에 봉인 캐릭터 정보가 중복되어 있음")
				<< BUILD_LOG(dwUserUID_)
				<< BUILD_LOG(nCharType)
				<< END_LOG;
		}
	}

	START_LOG(clog, L"봉인 캐릭터 정보 가져오기: ")
		<< BUILD_LOG(dwUserUID_)
		<< BUILD_LOG(setSealedChar_.size())
		<< END_LOG;
}

void KGSDBThread::UpdateSealedCharInfo(IN const DWORD & dwUserUID_, IN std::map< char, KCharacterInfo > & mapCharacterInfo_, IN OUT std::map< int, KSealedCharInfo > & kSealedCharInfo_)
{
	// 이벤트 진행중인 캐릭터 번호를 찾아서
	// 보유한 캐릭터의 레벨이 목표 레벨보다 높은지 확인하고
	// 높은 경우 봉인 해제상태로 DB에 갱신한다
	// 패킷에도 갱신된 정보를 저장한다
	std::map< int, KSealedCharInfo >::iterator mitSealedCharInfo = kSealedCharInfo_.begin();
	for (; mitSealedCharInfo != kSealedCharInfo_.end(); ++mitSealedCharInfo) {
		if (mitSealedCharInfo->second.m_bIsSealed) {
			std::map< char, KCharacterInfo >::iterator mitCharInfo;
			mitCharInfo = mapCharacterInfo_.find(mitSealedCharInfo->second.m_nCharType);
			if (mitCharInfo == mapCharacterInfo_.end()) {
				continue;
			}

			int nObjectLevel = 0;
			if (false == SiKSealedChar()->GetObjectLevel(mitSealedCharInfo->second.m_nCharType, nObjectLevel)) {
				continue;
			}

			if (static_cast<int>(mitCharInfo->second.m_dwLevel) >= nObjectLevel) {
				// DB 갱신
				CharacterSeal_Merge(dwUserUID_, mitSealedCharInfo->second.m_nCharType, false);

				// 패킷 갱신
				mitSealedCharInfo->second.m_bIsSealed = false;
				mitSealedCharInfo->second.m_bIsEventTerm = false;
				mitSealedCharInfo->second.m_nObjectLevel = 0;
				mitSealedCharInfo->second.m_tmObjectTime = 0;
			}
		}
	}
}

void KGSDBThread::CharacterSeal_Merge(IN const DWORD & dwUserUID_, IN int& nCharType_, IN bool bSealed_)
{
	/*
	케릭터 봉인 정보 갱신
	L"{ call dbo.CSGACharacterSeal_Merge ( %d, %d, %d  ) }"

	{ call dbo.CSGACharacterSeal_Merge ( @1, @2, @3 ) }
	@1: @iLoginUID_   int
	@2: @iCharType_   tinyint
	@3: @iSeal_       tinyint     --0:해제, 1:봉인

	1 return ( @1 )
	@1: OK       int
		0       : 성공
		-1      : 유저가 없음
		-901이하: 트랜젝션 에러
	*/
	int nSealed = (bSealed_) ? 1 : 0;

	DBJIF(m_kODBC.Query(L"{ call dbo.CSGACharacterSeal_Merge ( %d, %d, %d  ) }",
		dwUserUID_, nCharType_, nSealed));

	int nOK = -99;

	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
	}
	m_kODBC.EndFetch();

	_LOG_SUCCESS(nOK == 0, L"봉인 캐릭터 정보 갱신 완료 : " << nOK)
		<< BUILD_LOG(dwUserUID_)
		<< BUILD_LOG(nCharType_)
		<< BUILD_LOG(nSealed)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< END_LOG;
}


bool KGSDBThread::GetSongkranEventResetTime(IN const DWORD & dwUID_, OUT time_t & tResetTime_)
{
	/*
	송크란 재공 받은 개수 초기화 시간 검색
	L"{ call dbo.MEGAMultiEventResetTime_Select ( %d ) }"

	{ call dbo.MEGAMultiEventResetTime_Select ( @1 ) }
	@1: @iLoginUID_  int


	1 return ( @1, @2 )
	@1: LoginUID  int
	@2: ResetTime datetime
	*/

	time_t tTimeStamp = tResetTime_;
	CTime tmCurrentTime = CTime(tTimeStamp);

	DBJIF2(m_kODBC.Query(L"{ call dbo.MEGAMultiEventResetTime_Select ( %d ) }", dwUID_));

	int nLoginUID = 0;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC
			>> nLoginUID
			>> tResetTime_);
		m_kODBC.EndFetch();
	}

	// DB에서 받은 값이 없을 때
	if (tTimeStamp == tResetTime_) {
		CTime tmYesterDay = CTime(tmCurrentTime.GetYear(), tmCurrentTime.GetMonth(), tmCurrentTime.GetDay(), 0, 0, 0) - CTimeSpan(1, 0, 0, 0);
		tResetTime_ = KncUtil::TimeToInt(tmYesterDay);

		START_LOG(clog, L"DB에서 ResetTime 얻기 실패. 어제 날짜로 설정.")
			<< BUILD_LOG(tResetTime_)
			<< END_LOG;
	}

	START_LOG(clog, L"송크란 데이터 초기화 시점 얻기 성공 : " << dwUID_)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< END_LOG;

	return true;
}

bool KGSDBThread::UpdateSongkranEventResetTime(IN const DWORD & dwUID_, IN const CTime & tmDate_)
{
	/*
	송크란 재공 받은 개수 초기화 시간 갱신
	L"{ call dbo.MEGAMultiEventResetTime_Merge ( %d, N'%s' ) }"

	{ call dbo.MEGAMultiEventResetTime_Merge ( @1, @2 ) }
	@1: @iLoginUID_  int
	@2: @sdtRegDateD_ smalldatetime

	1 return ( @1 )
	@1: OK       int
		0       : 성공
		-1      : 유저가 없음
		-901이하: 트랜젝션 에러
	*/



	DBJIF2(m_kODBC.Query(L"{ call dbo.MEGAMultiEventResetTime_Merge ( %d, N'%s' ) }",
		dwUID_, (LPCTSTR)tmDate_.Format(_T("%Y%m%d %H:%M:%S"))));

	int nOK = -99;
	if (m_kODBC.BeginFetch()) {
		VERIFY_QUERY(m_kODBC >> nOK);
		m_kODBC.EndFetch();
	}

	_LOG_SUCCESS(0 == nOK, L"송크란 데이터 초기화 시점 기록 성공 : " << nOK)
		<< BUILD_LOG(m_kODBC.m_strLastQuery)
		<< END_LOG;

	return true;
}

void KGSDBThread::UpdateFinalLoginTime(IN const DWORD & dwUID_)
{
	/*
	유저 마지막 접속 날짜 기록
	L"{ call dbo.LLGALastLogin_Merge ( %d ) }"

	{ call dbo.LLGALastLogin_Merge ( @1 ) }
	@1: @iLoginUID_  int


	1 return ( @1 )
	@1: OK       int
		0       : 성공
		-1      : 유저가 없음
		-901이하: 트랜젝션 에러
	*/

	DBJIF(m_kODBC.Query(L"{ call dbo.LLGALastLogin_Merge ( %d ) }", dwUID_));

	int nRet = 0;
	if (m_kODBC.BeginFetch())
	{
		VERIFY_QUERY(m_kODBC >> nRet);
	}
	m_kODBC.EndFetch();

	if (0 != nRet) {
		START_LOG(cwarn, L" LastLogin Time 기록 실패. Name : " << nRet) << END_LOG;
	}
}

int KGSDBThread::CheckReserveNewUID(IN const DWORD & dwUID_, IN const char cCharType_, IN const std::vector<KItem> & vecItem_, IN int nType_, OUT std::vector<KItem> & vecNewUIDItem_)
{
	int nFailCount = 0;

	std::vector<KItem>::const_iterator vit;
	for (vit = vecItem_.begin(); vit != vecItem_.end(); ++vit) {
		if (vit->m_bReserveNewUID == true && vit->m_nCount > 0) {
			if (false == m_kHelper.InsertSingleItemJobForDepot(dwUID_, cCharType_, vit->m_ItemID, vit->m_nCount, vit->m_nPeriod, nType_,
				vecNewUIDItem_, vit->m_cGradeID, vit->m_sEquipLevel, vit->m_cEnchantLevel)) {
				++nFailCount;
			}
		}
	}

	return nFailCount;
}

int KGSDBThread::SetNewUID(IN const std::vector<KItem> & vecNewUIDItem_, OUT std::vector<KItem> & vecInvenItem_)
{
	int nSuccessCount = 0;
	std::vector<KItem>::const_iterator vitResult;
	std::vector<KItem>::iterator vitFind;
	for (vitResult = vecNewUIDItem_.begin(); vitResult != vecNewUIDItem_.end(); ++vitResult) {
		vitFind = std::find_if(vecInvenItem_.begin(), vecInvenItem_.end(), boost::bind(&KItem::m_ItemID, _1) == vitResult->m_ItemID);

		if (vitFind != vecInvenItem_.end()) {

			if (vitFind->m_cGradeID == vitResult->m_cGradeID) {
				vitFind->m_ItemUID = vitResult->m_ItemUID;
				++nSuccessCount;
			}
			else {
				START_LOG(cerr, L"같은 ItemID에 Grade 가 다른 경우 : " << vitFind->m_ItemID) << END_LOG;
			}
		}
		else {
			START_LOG(cerr, L"창고 수량아이템 SetNewUID실패")
				<< BUILD_LOG(vitResult->m_ItemID)
				<< BUILD_LOG(vitResult->m_ItemUID)
				<< BUILD_LOGc(vitResult->m_cGradeID)
				<< BUILD_LOG(vitResult->m_nCount)
				<< BUILD_LOG(vitResult->m_nInitCount)
				<< BUILD_LOGc(vitResult->m_cInvenType)
				<< BUILD_LOGc(vitResult->m_cInitInvenType)
				<< BUILD_LOG(vitResult->m_bReserveNewUID)
				<< BUILD_LOGc(vitResult->m_cInitSlotID)
				<< BUILD_LOGc(vitResult->m_cSlotID)
				<< END_LOG;
		}
	}

	return nSuccessCount;
}

void KGSDBThread::GetLastPlayCharNickName(IN const char cLastPlayCharacter_, IN const std::map< char, KCharacterInfo > & mapCharacterInfo_, OUT std::wstring & wstrLastPlayCharNickName_)
{
	wstrLastPlayCharNickName_ = L"";

	std::map< char, KCharacterInfo >::const_iterator mitCharacterInfo;
	mitCharacterInfo = mapCharacterInfo_.find(cLastPlayCharacter_);
	if (mitCharacterInfo != mapCharacterInfo_.end()) {
		wstrLastPlayCharNickName_ = mitCharacterInfo->second.m_strCharName;
	}
}
