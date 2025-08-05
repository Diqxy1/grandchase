#include "User.h"
#include <dbg/dbg.hpp>
#include "UserEvent.h"
#include "UserFSM.h"
#include "GSSimLayer.h"
#include "Channel.h"
#include "ChannelManager.h"
#include "FailRate.h"
#include "GSDBLayer.h"
#include "GameServer.h"
#include "IndigoUtil.h"
#include "ResultManager.h"
#include "GameDrop.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "DungeonEntrance.h"
#include "support_User.h"
#include "GCHelper.h"
#include "CenterPacket.h"
#include "CnConnector.h"
#include "HeroDungeonManager.h"
#include "StatRoomJoin.h"
#include "Survey.h"
#include "Escort.h"
#include "SlangFilter.h"
#include "VitalityManager.h"
#include "MissionManager.h"
#include "MissionInfo.h"
#include "DungeonRankManager.h"

#define CLASS_TYPE  KUser

_IMPL_ON_FUNC(EVENT_REGISTER_NICKNAME_REQ, std::wstring)
{
	VERIFY_STATE((1, KUserFSM::STATE_CHANNELLOBBY));

	int nRet = SiKSlangFilter()->IsValidateNickNameString(kPacket_, m_strName, SiKGSSimLayer()->m_uiNickNameAvailable);
	if (nRet != 0) {

		KPacketNameOK kPacket;
		kPacket.m_nOK = nRet;
		SEND_PACKET(EVENT_REGISTER_NICKNAME_ACK);
		return;
	}

	QUEUEING_EVENT_TO_DB(EVENT_REGISTER_NICKNAME_REQ, kPacket_);

}

_IMPL_ON_FUNC(EVENT_REGISTER_NICKNAME_ACK, KPacketNameOK)
{
	if (kPacket_.m_nOK == 0) {
		m_strNickName = kPacket_.m_strName;

		SiKGSSimLayer()->AddNickToLogin(m_strName, m_strNickName);

		START_LOG(clog, L"닉네임이 성공적으로 등록되었습니다.")
			<< BUILD_LOG(m_strName)
			<< BUILD_LOG(kPacket_.m_strName)
			<< BUILD_LOG(m_strNickName) << END_LOG;
	}
	else
	{
		START_LOG(cwarn, L"닉네임 등록이 실패했습니다. OK값을 확인하세요.")
			<< BUILD_LOG(kPacket_.m_nOK)
			<< BUILD_LOG(m_strName)
			<< BUILD_LOG(kPacket_.m_strName)
			<< END_LOG;
	}

	//  0 : 성공
	// -1 : 아이디와 기존닉네임으로 해당 유저를 찾을 수 없음
	// -2 : 신규닉네임이 이미 사용중임
	// -3 : 닉네임변경아이템의 UID 또는 ID값이 다르거나 아이템이 존재하지 않음
	// -4 : 닉네임 중에 사용할 수 없는 단어나 문자가 있음.
	// -5 : 닉네임 길이가 조건에 맞지 않음.
	// -101이하 : DB처리 실패

	SEND_RECEIVED_PACKET(EVENT_REGISTER_NICKNAME_ACK);
}

_IMPL_ON_FUNC(EVENT_ENTER_CHANNEL_REQ, DWORD)
{
	_VERIFY_STATE((1, KUserFSM::STATE_CHANNELLOBBY));

	if (m_strNickName.empty()) return; //아직 닉네임을 등록하지 않았음.

	KEVENT_ENTER_CHANNEL_ACK kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_cType = -1;
	CTime tmTmp = CTime::GetCurrentTime();
	kPacket.m_tOpenTime = KncUtil::TimeToInt(CTime(tmTmp.GetYear(), tmTmp.GetMonth(), tmTmp.GetDay(), 0, 0, 0));
	kPacket.m_tCloseTime = KncUtil::TimeToInt(CTime(tmTmp.GetYear(), tmTmp.GetMonth(), tmTmp.GetDay(), 23, 59, 59));

	SET_ERROR(ERR_UNKNOWN);

	// indigo 데이터가 없다면 db작업을 한번 거쳐야 한다.
	if (SiKGSSimLayer()->CheckTypeFlag(ST_INDIGO) && !m_bIndigoDataExist) {
		if (!GetBestCharacter()) {
			SET_ERROR(ERR_CHANNEL_04);
		}
		else if (!IsAdmin() && GetBestCharacter()->m_dwLevel < SiKGSSimLayer()->m_dwIndigoLevel) {     // 최고 캐릭터 레벨이 30을 못넘으면  
			// note : 여기서 last error만을 지정해주면, 계속 밑으로 진행해 ack 전송 처리로 들어간다.
			SET_ERROR(ERR_CHANNEL_04);
		}
		else if (!IsAdmin() && SiKGSSimLayer()->m_bCheckIndigoTicket && !CheckIfAllItemExist(KUser::INDIGO_TICKET_ID, -1)) {   // 입장권을 가지고 있지 않거나   
			// note : 여기서 last error만을 지정해주면, 계속 밑으로 진행해 ack 전송 처리로 들어간다.
			SET_ERROR(ERR_CHANNEL_06);
		}
		else if (IsExpAccount()) {
			// note. 체험 계정이면 접속못하게 처리.
			SET_ERROR(ERR_CHANNEL_11);
		}
		else
		{
			KDB_EVENT_CREATE_INDIGO_REQ kPacket;
			kPacket.m_dwUserUID = m_dwUID;
			kPacket.m_strLogin = m_strName;
			kPacket.m_dwChannelUID = kPacket_;
			QUEUEING_EVENT_TO_DB_CHECK(DB_EVENT_CREATE_INDIGO_REQ, kPacket);
			return;     // db에 데이터를 보냈다면 채널 입장처리를 지금 하지 않는다.
		}
	}
	else    // 채널 입장 처리 진행
	{
		if (SiKChannelManager()->EnterChannel(kPacket_, this) && m_pkChannel != NULL) {  // NULL 체크는 심했나?  
			kPacket.m_cType = m_pkChannel->GetType();
		}
	}

END_PROC:
	switch (NetError::GetLastNetError())
	{
		ERR_CASE(NET_OK, kPacket.m_nOK, 0);
		ERR_CASE(ERR_CHANNEL_02, kPacket.m_nOK, -2);  // 현재 채널의 정원이 모두 찼습니다.
		ERR_CASE(ERR_CHANNEL_03, kPacket.m_nOK, -3);  // 가입된 길드가 있어야만 길드채널에 입장할 수 있습니다.
		ERR_CASE(ERR_CHANNEL_04, kPacket.m_nOK, -4);  // Indigo입장 불가. 레벨이 낮음
		ERR_CASE(ERR_CHANNEL_05, kPacket.m_nOK, -5);  // Indigo입장 불가. RP가 0임.
		ERR_CASE(ERR_CHANNEL_06, kPacket.m_nOK, -6);  // Indigo입장 불가. 입장권이 없음
		ERR_CASE(ERR_CHANNEL_07, kPacket.m_nOK, -7);  // Indigo입장 불가능. 입장 가능한 시간이 아님
		ERR_CASE(ERR_CHANNEL_08, kPacket.m_nOK, -8);  // 대전 채널 입장 가능한 레벨이 아님
		ERR_CASE(ERR_CHANNEL_09, kPacket.m_nOK, -9);  // 캐릭터가 존재하지 않음.
		ERR_CASE(ERR_CHANNEL_10, kPacket.m_nOK, -10); // 동일한 유저가 채널안에 있음.
		ERR_CASE(ERR_CHANNEL_11, kPacket.m_nOK, -11); // 체험 계정으로 용사의 섬 입장 불가.
		ERR_CASE(ERR_CHANNEL_12, kPacket.m_nOK, -12); // 체험 계정으로 선택 채널 입장 불가.(대전 자유채널)
		ERR_CASE(ERR_CHANNEL_13, kPacket.m_nOK, -13); // IP 대역이 옳지 않음.
		ERR_CASE(ERR_INTERNAL_00, kPacket.m_nOK, -14); // 채널 객체를 얻어올 수 없음.
		ERR_CASE(ERR_CHANNEL_14, kPacket.m_nOK, -15); // 길드 레벨에 맞지 않는 채널.
	default:
		START_LOG(cerr, L"undefined error code : " << NetError::GetLastNetError()) << END_LOG;
	}

	// Indigo면, 금일 오픈시간을 전달.
	if (SiKGSSimLayer()->CheckTypeFlag(ST_INDIGO)) {
		kPacket.m_tOpenTime = KncUtil::TimeToInt(SiKServerOpenTime()->GetToadysOpenTime());
		kPacket.m_tCloseTime = KncUtil::TimeToInt(SiKServerOpenTime()->GetToadysCloseTime());
	}

	LOG_CONDITION(kPacket.m_nOK == 0, clog, cwarn)
		<< BUILD_LOG(m_strName)
		<< BUILD_LOG(kPacket_)
		<< BUILD_LOGc(kPacket.m_cType)
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(NetError::GetLastNetErrMsg())
		<< BUILD_LOG((LPCTSTR)CTime(kPacket.m_tOpenTime).Format(KNC_TIME_FORMAT))
		<< BUILD_LOG((LPCTSTR)CTime(kPacket.m_tCloseTime).Format(KNC_TIME_FORMAT)) << END_LOG;

	SEND_PACKET(EVENT_ENTER_CHANNEL_ACK);
}

// EVENT_LEAVE_CHANNEL_NOT
IMPL_ON_FUNC_NOPARAM(EVENT_LEAVE_CHANNEL_NOT)
{
	if (GetStateID() == KUserFSM::STATE_CHANNELLOBBY) return;

	_VERIFY_STATE((1, KUserFSM::STATE_CHANNEL));

	LeaveAgit();

	JIF(m_pkChannel);

	START_LOG(clog, L"Leave Channel.. UserState : " << GetStateIDString())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(m_pkChannel->GetChannelName()) << END_LOG;

	if (!m_pkChannel->Leave(*this)) {
		START_LOG(cerr, L"LEAVE CHANNEL 실패!!")
			<< BUILD_LOG(GetName())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		return;
	}
}

IMPL_ON_FUNC(EVENT_CREATE_ROOM_REQ)
{
	_VERIFY_STATE((1, KUserFSM::STATE_CHANNEL));

	KEVENT_CREATE_ROOM_ACK kPacket;
	KChatEventInfo kChatEventInfo;
	kChatEventInfo.m_nUID = -1;
	std::map<int, KDungeonUserInfo> mapDifficulty;
	GetUserDifficultInfo(mapDifficulty);

	JIF(m_pkChannel);
	JIF(m_pkChannel->m_spRoomManager.get());

	if (GetCurrentCharType() != kPacket_.m_kInDoorUserInfo.m_cCharIndex) {
		START_LOG(cerr, L"서버-클라이언트의 현재 캐릭터가 다르다, User: " << GetName())
			<< BUILD_LOGc(GetCurrentCharType())
			<< BUILD_LOGc(kPacket_.m_kInDoorUserInfo.m_cCharIndex)
			<< END_LOG;
		SET_ERR_GOTO(ERR_ROOM_35, END_PROC);
	}

	if (!SiKGSSimLayer()->m_bUserCreateRoomCompeteServer) {
		if (SiKGSSimLayer()->CheckTypeFlag(ST_ENTER_LIMIT) || SiKGSSimLayer()->CheckTypeFlag(ST_CREATE_LIMIT)) {
			if (!IsObserver()) {
				SET_ERR_GOTO(ERR_ROOM_59, END_PROC);
			}
		}
		else if (!SiKGSSimLayer()->CheckTypeFlag(ST_OBSERVER_ROOM)) {
			if (IsObserver()) {
				SET_ERR_GOTO(ERR_ROOM_31, END_PROC);
			}
		}
	}

	if (!CheckServerTypeEnable(kPacket_.m_kRoomInfo.m_iGameMode)) {
		SET_ERR_GOTO(ERR_ROOM_38, END_PROC);
	}

	if (SiKGSSimLayer()->CheckTypeFlag(ST_TOURNAMENT)) {
		if (SiKGCHelper()->IsTournamentCheck() == false) {
			SET_ERR_GOTO(ERR_ROOM_68, END_PROC);
		}
	}

	if (!CheckGameMode(kPacket_.m_kRoomInfo.m_iGameMode)) {
		SET_ERR_GOTO(ERR_ROOM_38, END_PROC);
	}

	if (m_pkChannel->GetType() == KChannel::CT_DUNGEON) {
		if (!SiKDungeonEntrance()->CheckDungeonEntrance(mapDifficulty, kPacket_.m_kRoomInfo.m_iGameMode, kPacket_.m_kRoomInfo.m_nDifficulty) &&
			!m_kInventory.FindItemByItemID(SiKGCHelper()->GetDungeonFreePassID())) {
			SET_ERR_GOTO(ERR_ROOM_47, END_PROC);
		}

		if (!HellModeCheck(kPacket_.m_kRoomInfo.m_iGameMode, kPacket_.m_kRoomInfo.m_nDifficulty)) {
			SET_ERR_GOTO(ERR_ROOM_50, END_PROC);
		}

		if (false == DungeonTicketCheck(kPacket_.m_kRoomInfo.m_iGameMode)) {
			SET_ERR_GOTO(ERR_ROOM_69, END_PROC);
		}

		if (SiKHeroDungeonManager()->IsHeroDungeon(kPacket_.m_kRoomInfo.m_iGameMode)) 
		{
			if (false == HeroDungeonCheck(kPacket_.m_kRoomInfo.m_iGameMode, kPacket_.m_kRoomInfo.m_nDifficulty)) {
				goto END_PROC;
			}

			if (false == SetHeroDungeonCharIndex(kPacket_.m_kRoomInfo.m_iGameMode, kPacket_.m_kInDoorUserInfo)) {
				goto END_PROC;
			}
		}
	}

	MakeCorrectInDoorInfo(kPacket_.m_kInDoorUserInfo);

	m_pkChannel->m_spRoomManager->CreateRoom(this, kPacket_, kPacket);
	if (m_pkRoom == NULL) {
		SET_ERR_GOTO(ERR_ROOM_49, END_PROC);
	}

END_PROC:
	switch (NetError::GetLastNetError())
	{
		ERR_CASE(NET_OK, kPacket.m_nOK, 0);
		ERR_CASE(ERR_ROOM_00, kPacket.m_nOK, 1);
		ERR_CASE(ERR_ROOM_35, kPacket.m_nOK, 2);
		ERR_CASE(ERR_ROOM_36, kPacket.m_nOK, 3);
		ERR_CASE(ERR_ROOM_38, kPacket.m_nOK, 4);
		ERR_CASE(ERR_ROOM_47, kPacket.m_nOK, 5);
		ERR_CASE(ERR_ROOM_31, kPacket.m_nOK, 6);
		ERR_CASE(ERR_ROOM_49, kPacket.m_nOK, 7);
		ERR_CASE(ERR_ROOM_50, kPacket.m_nOK, 8);
		ERR_CASE(ERR_ROOM_61, kPacket.m_nOK, 9);
		ERR_CASE(ERR_ROOM_62, kPacket.m_nOK, 10);
		ERR_CASE(ERR_ROOM_63, kPacket.m_nOK, 11);
		ERR_CASE(ERR_ROOM_64, kPacket.m_nOK, 12);
		ERR_CASE(ERR_ROOM_65, kPacket.m_nOK, 13);
		ERR_CASE(ERR_ROOM_66, kPacket.m_nOK, 14);
		ERR_CASE(ERR_ROOM_68, kPacket.m_nOK, 15);
		ERR_CASE(ERR_ROOM_69, kPacket.m_nOK, 16);

	default:
		START_LOG_WITH_NAME(cerr)
			<< BUILD_LOG(NetError::GetLastNetErrMsg())
			<< END_LOG;
		kPacket.m_nOK = -10; // unknown error
		break;
	}

	LOG_CONDITION(kPacket.m_nOK == 0, clog, cwarn)
		<< L" 방생성 결과 : " << NetError::GetLastNetErrMsg()
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket_.m_kRoomInfo.m_iP2PVersion)
		<< BUILD_LOG(kPacket_.m_kRoomInfo.m_bPublic)
		<< BUILD_LOG(kPacket_.m_kRoomInfo.m_usMaxUsers)
		<< BUILD_LOG(kPacket_.m_kRoomInfo.m_iGameMode)
		<< BUILD_LOG(kPacket_.m_kRoomInfo.m_nDifficulty)
		<< BUILD_LOGc(kPacket_.m_kRoomInfo.m_cGameCategory)
		<< BUILD_LOG(kPacket.m_kRoomInfo.m_bGuild)
		<< BUILD_LOG(kPacket.m_kRoomInfo.m_iGameMode)
		<< BUILD_LOG(kPacket.m_kRoomInfo.m_nDifficulty)
		<< BUILD_LOG(kPacket.m_kInDoorUserInfo.m_vecCharInfo.size()) << END_LOG;

	//SEND_PACKET( EVENT_CREATE_ROOM_ACK );
	SEND_COMPRESS_PACKET(EVENT_CREATE_ROOM_ACK, kPacket);

	// Ã¤ÆÃ ÀÌº¥Æ®¸¦ À¯Àú ÇÑÅ× Àü´Þ ÇÑ´Ù.
	if (kPacket.m_nOK == 0 && m_pkRoom->GetChatEvent(kChatEventInfo)) {
		SEND_DATA(EVENT_CHATTING_EVENT_MSG_NOT, kChatEventInfo);
		START_LOG(clog, L"채팅 이벤트 등록됨 Name : " << m_strName)
			<< BUILD_LOG(kChatEventInfo.m_nUID) << END_LOG;
	}

	if (kPacket_.m_kRoomInfo.m_iGameMode == GC_GM_AUTOMATCH_TEAM) {
		if (kPacket.m_nOK == 0) {
			JoinRoomMatchPartyMember(kPacket.m_kRoomInfo.m_usRoomID);
		}
		else { 
			CreateRoomFailMatchPartyMember();
		}
	}
}

//#EVENT_LEAVE_ROOM_REQ
IMPL_ON_FUNC(EVENT_LEAVE_ROOM_REQ) // 자유의지, 피치못할 사정 
{
	// 051007. florist. 방접속 실패 원인 카운트 관련 - 네트웍 통신 시간상 차이로 STATE_CHANNEL에서
	// LEAVE_ROOM_NOT를 처리하는게 올바른 경우라도, 이미 서버에서 다른 이유로 실패, 상태변경이 된 것이기 때문에
	// 실패 원인이 달라질 뿐 1회 실패가 카운트 되는 것은 똑같다. 따라서 그냥 무시해도 좋다.
	// 문제가 되는 것은 정말 상태오류로 날아든 유효할지도 모를 LEAVE_ROOM_NOT 이다. 
	// 이문제는 상태오류를 없애야만 해결 가능하다.

	int kPacket = 0;

	if (GetStateID() == KUserFSM::STATE_CHANNEL) {
		kPacket = -1; //invalid State
		SEND_PACKET(EVENT_LEAVE_ROOM_ACK);
		return;
	}

	if (!m_pkCurrentState->CheckValidState(2, KUserFSM::STATE_ROOM, KUserFSM::STATE_PLAYING)) {
		START_LOG(cerr, L"상태 오류. Name : " << m_strName)
			<< L"    현재 상태 : " << GetStateIDString() << dbg::endl
			<< L"    허용 상태 : 2, KUserFSM::STATE_ROOM, KUserFSM::STATE_PLAYING " << END_LOG;

		kPacket = -1; //invalid State
		SEND_PACKET(EVENT_LEAVE_ROOM_ACK);
		return;
	}

	int nReasonWhy = KRoom::LEAVE_SELF_DECISION; // 방을 떠나는 이유는 정상으로 셋팅

	// P2P 처리 중 방접속에 실패한 경우, 실패 이유를 수집한다.
	if (GetStateID() == KUserFSM::STATE_PLAYING) { //원래는 Room, Join상태이다.  
		switch (kPacket_.m_iP2PError)
		{
		case KFailRate::FAIL_START_USER:
		case KFailRate::P2P_STANDBY_NO_ACK:
		case KFailRate::POOR_P2P_BAN:
			SiKFailRate()->IncreaseCount(kPacket_.m_iP2PError);
			break;

		case KFailRate::P2P_NO_ERROR:                       // 에러 없음
			// doing nothing.
			break;
		default:
			START_LOG(cerr, L"방접속 실패 원인이 올바르지 못하게 보고됨. Name : " << m_strName)
				<< BUILD_LOG(kPacket_.m_iP2PError)
				<< END_LOG;
		}
	}
	else if (kPacket_.m_iP2PError != KFailRate::P2P_NO_ERROR) {  // 접속중 상태가 아니라면 항상 이유가 P2P_NO_ERROR 여야 한다.  
		// 현재 통신 시간상 차이로 인해 P2P_CAN_NOT_RECV_FIRST_ROOM_INFO_*가 경기중에 날아올 수 있다.
		START_LOG(cwarn, L"경기중 상태에서 접속 실패 보고. Name : " << m_strName)
			<< L"    Reason : " << KFailRate::IDToString(kPacket_.m_iP2PError) << dbg::endl
			<< L"    User State : " << GetStateIDString() << dbg::endl
			<< L"    Room Name : " << (m_pkRoom ? m_pkRoom->GetName() : L"(방 객체 포인터 무효)")
			<< END_LOG;

		SiKFailRate()->IncreaseCount(KFailRate::P2P_NO_ERROR);
	}

	JIF(m_pkRoom);
	JIF(m_pkChannel);

	if (GetStateID() == KUserFSM::STATE_PLAYING) {  //플레이 중에 나가게 될때 또는 P2P 연결이 제대로 되지 않으면  
		m_pkRoom->LeaveGame(*this); //룸 상태로 변경
	}

	START_LOG_WITH_NAME(clog)
		<< BUILD_LOG(m_pkRoom->GetName())
		<< BUILD_LOG(m_bGoToGuildZone) << END_LOG;

	if (kPacket_.m_iP2PError == KFailRate::FAIL_START_USER ||
		kPacket_.m_iP2PError == KFailRate::POOR_P2P_BAN ||
		kPacket_.m_iP2PError == KFailRate::P2P_STANDBY_NO_ACK) // 시작 못했을경우 사유를 적는다.
		nReasonWhy = KRoom::LEAVE_LOADING_COMPLETE_FAILED;


	if (!m_pkRoom->Leave(*this, nReasonWhy)) {
		START_LOG(cerr, L"LEAVE ROOM 실패!!")
			<< BUILD_LOG(GetName())
			<< BUILD_LOG(m_bGoToGuildZone)
			<< END_LOG;
		kPacket = -2; //invalid State
		SEND_PACKET(EVENT_LEAVE_ROOM_ACK);
	}
	else if (nReasonWhy != KRoom::LEAVE_LOADING_COMPLETE_FAILED) {
		kPacket = 0; //invalid State
		SEND_PACKET(EVENT_LEAVE_ROOM_ACK);
	}

	// Indigo일때 방에서 유저가 나가기 버튼을 눌러서 나가면 RP를 차감한다.
	if (m_pkChannel->GetType() == KChannel::CT_INDIGO && kPacket_.m_bSubtractRP) {
		std::pair<int, int> prAvgRP(m_iRP, m_iRP);

		// param : (자신의 RP, 양팀 평균RP, 팀 구분, 승패여부, iNumDies)
		m_iRP += IndigoUtil::GetRPChange(m_iRP, prAvgRP, true, false, 0);

		int kPacket = m_iRP;    // 어차피 int로 캐스팅 해야 하니, local 변수를 만들어 매크로 사용이 가능하게 한다.
		SEND_PACKET(EVENT_RP_CHANGED_NOT);
	}

	ExitParty(); // 파티에서 나가기
	m_mapMatchParty.clear();
}

_IMPL_ON_FUNC(EVENT_JOIN_ROOM_REQ, KJoinRoomReqInfo)
{
	_VERIFY_STATE((1, KUserFSM::STATE_CHANNEL));

	JIF(m_pkChannel);

	KChatEventInfo kChatEventInfo;
	kChatEventInfo.m_nUID = -1;

	KJoinRoomAckInfo kJoinRoomAckInfo;
	kJoinRoomAckInfo.m_nOK = NetError::ERR_UNKNOWN;
	kJoinRoomAckInfo.m_veckInDoorUserInfo.clear();
	KEVENT_JOIN_ROOM_INFO_DIVIDE_ACK kPacket;
	std::vector<KInDoorUserInfo>::iterator vit;
	SET_ERROR(ERR_UNKNOWN);

	if (GetCurrentCharType() != kPacket_.m_kInDoorUserInfo.m_cCharIndex) { // 이경우는 발생하면 안된다.
		START_LOG(cerr, L"서버-클라이언트의 현재 캐릭터가 다르다, User: " << GetName())
			<< END_LOG;
		SET_ERR_GOTO(ERR_ROOM_35, END_PROC);
	}

	MakeCorrectInDoorInfo(kPacket_.m_kInDoorUserInfo);
	SiKFailRate()->IncreaseCount(KFailRate::TOTAL_JOIN_COUNT);    // 방접속률 통계자료 수집.

	START_LOG(clog, kPacket_.m_uiJoinType) << END_LOG;

	switch (kPacket_.m_uiJoinType)
	{
	case KJoinRoomReqInfo::JOIN_NORMAL:
	{
		if (m_pkChannel->m_spRoomManager->JoinRoom(this, kPacket_, kJoinRoomAckInfo)) {
			JIF(m_pkRoom);
		}
	}
	break;
	case KJoinRoomReqInfo::JOIN_QUICK:
	{
		if (m_pkChannel->m_spRoomManager->QuickJoinRoom(this, kPacket_, kJoinRoomAckInfo)) {
			JIF(m_pkRoom);
		}
	}
	break;

	default:
	{
		START_LOG(cerr, L"알수 없는 방접속 타입을 보내왔음") << END_LOG;
		goto __Fail__;
	}
	}//end switch

END_PROC:
	int nFailIndex = KFailRate::LOGICAL_ERROR_01;
	switch (NetError::GetLastNetError())
	{
		ERR_CASE_ACTION(NET_OK, kJoinRoomAckInfo.m_nOK, 0, nFailIndex = KFailRate::P2P_JOIN_COUNT); // 성공
		ERR_CASE_ACTION(ERR_ROOM_01, kJoinRoomAckInfo.m_nOK, 1, nFailIndex = KFailRate::CLOSED_ROOM); // 방 접속 불가능. 정원이 모두 찾습니다.
		ERR_CASE_ACTION(ERR_ROOM_02, kJoinRoomAckInfo.m_nOK, 2, nFailIndex = KFailRate::LOGICAL_ERROR_00); // 방 접속 불가능. 현재 방에 동시에 다른 사용자가 접속중 임.
		ERR_CASE(ERR_ROOM_03, kJoinRoomAckInfo.m_nOK, 3); // 방 접속 불가능. 방장 부재중
		ERR_CASE(ERR_ROOM_04, kJoinRoomAckInfo.m_nOK, 4); // 방 참여 실패. 잘못된 정보로 접속을 시도함. RoomID가 올바르지 않음
		ERR_CASE_ACTION(ERR_ROOM_05, kJoinRoomAckInfo.m_nOK, 5, nFailIndex = KFailRate::CLOSED_ROOM); // 방 참여 실패. 현재 방이 대기상태가 아님.
		ERR_CASE_ACTION(ERR_ROOM_06, kJoinRoomAckInfo.m_nOK, 6, nFailIndex = KFailRate::WRONG_PASSWD); // 방 참여 실패. 비밀번호가 다름.
		ERR_CASE_ACTION(ERR_ROOM_07, kJoinRoomAckInfo.m_nOK, 7, nFailIndex = KFailRate::QUICKJOIN_NO_AVAILABLE); // 퀵조인 실패. 대기중인 공개방을 찾을 수 없음.
		ERR_CASE_ACTION(ERR_ROOM_13, kJoinRoomAckInfo.m_nOK, 8, nFailIndex = KFailRate::ALREADY_HAVE_ITEM); // 방 참여 실패. 운영자 이벤트 기간중 이벤트 아이템을 이미 가지고 있음.    
		ERR_CASE_ACTION(ERR_ROOM_17, kJoinRoomAckInfo.m_nOK, 9, nFailIndex = KFailRate::HOST_NOTIFY_FAILED); // 방 참여 실패. 방장에게 접속 알림 패킷을 보내지 못함.    
		ERR_CASE_ACTION(ERR_ROOM_19, kJoinRoomAckInfo.m_nOK, 10, nFailIndex = KFailRate::WRONG_P2P_PROTOCOL_VERSION); // 방 참여 실패. P2P 버전이 맞지 않음.
		ERR_CASE(ERR_ROOM_30, kJoinRoomAckInfo.m_nOK, 11); // 옵저버가 이미 존재하는 방에 옵저버가 들어갈려고 했다.
		ERR_CASE(ERR_ROOM_33, kJoinRoomAckInfo.m_nOK, 12); // 더이상 다른 길드는 입장할수 없습니다.
		ERR_CASE(ERR_ROOM_34, kJoinRoomAckInfo.m_nOK, 13); // 같은 길드 유저가 슬롯의 반을 채웠음
		ERR_CASE(ERR_ROOM_35, kJoinRoomAckInfo.m_nOK, 14); // 존재하지 않는 캐릭터가 선택되었습니다.
		ERR_CASE(ERR_ROOM_36, kJoinRoomAckInfo.m_nOK, 15); // 입장할수 있는 올바른 레벨이 아닙니다.
		ERR_CASE(ERR_ROOM_50, kJoinRoomAckInfo.m_nOK, 16); // 헬모드 입장 조건이 되지 않습니다.
		ERR_CASE(ERR_ROOM_57, kJoinRoomAckInfo.m_nOK, 17); // 이번 게임에 Leave 했던 유저임.
		ERR_CASE(ERR_ROOM_61, kJoinRoomAckInfo.m_nOK, 18); // 영웅던전 모드가 비활성화 상태.
		ERR_CASE(ERR_ROOM_62, kJoinRoomAckInfo.m_nOK, 19); // 영웅던전 정보가 없음.
		ERR_CASE(ERR_ROOM_63, kJoinRoomAckInfo.m_nOK, 20); // 영웅던전 최소 레벨조건을 만족하는 캐릭터가 없음.
		ERR_CASE(ERR_ROOM_64, kJoinRoomAckInfo.m_nOK, 21); // 영웅던전 입장 시간이 아닙니다.
		ERR_CASE(ERR_ROOM_65, kJoinRoomAckInfo.m_nOK, 22); // 해당 시간대 영웅던전을 이미 플레이했음.
		ERR_CASE(ERR_ROOM_66, kJoinRoomAckInfo.m_nOK, 23); // 영웅던전 일일 클리어할 수 있는 최대 회수를 플레이했음.
		ERR_CASE(ERR_ROOM_69, kJoinRoomAckInfo.m_nOK, 24); // 던전에 입장가능한 티켓 아이템이 없음.
		ERR_CASE(ERR_ROOM_23, kJoinRoomAckInfo.m_nOK, 25); // 현재 게임모드 클리어 조건 단계가 유저의 클리어 단계보다 높다.
		ERR_CASE(ERR_ROOM_38, kPacket.m_nOK, 26); // 해당모드의 방을 생성할수 없습니다.

	default:
		START_LOG(cwarn, L"예상하지 못한 방접속 결과 발생.")
			<< BUILD_LOG(NetError::GetLastNetErrMsg())
			<< BUILD_LOG(kJoinRoomAckInfo.m_nOK) << END_LOG;
		break;
	}
	SiKFailRate()->IncreaseCount(nFailIndex);

	{ // 방 입장 종류 카운트 통계
		KStatRoomJoinPtr spStat = GET_STAT(KStatRoomJoin, SI_STAT_ROOMJOIN);
		if (spStat) {
			spStat->AddStat(kPacket_.m_uiJoinType);
		}
	}

__Fail__:
	// 룸 정보를 우선 전달하도록 수정.
	// 아래 작업을 진행하면서 전체 패킷이 룸 정보를 계속 전달하면서 패킷 전체 사이즈가 증가했음.
	if (kJoinRoomAckInfo.m_nOK == 0) {
		SEND_DATA(EVENT_JOIN_ROOM_INFO_ACK, kJoinRoomAckInfo.m_kRoomInfo);
	}

	// 첫번째 유저의 정보만 받는다.
	kPacket.m_nOK = kJoinRoomAckInfo.m_nOK;
	kPacket.m_MaxIndex = kJoinRoomAckInfo.m_veckInDoorUserInfo.size();
	int nIndex = 0; // vector size로 Max Index를 체크하기 때문에..,
	if (kPacket.m_nOK == 0 && !kJoinRoomAckInfo.m_veckInDoorUserInfo.empty()) {
		kPacket.m_nCurrentIndex = nIndex;
		std::vector<KInDoorUserInfo>::iterator vit = kJoinRoomAckInfo.m_veckInDoorUserInfo.begin();
		kPacket.m_kInDoorUserInfo = *vit;
		SEND_COMPRESS_PACKET(EVENT_JOIN_ROOM_INFO_DIVIDE_ACK, kPacket);
	}
	else {
		SEND_COMPRESS_PACKET(EVENT_JOIN_ROOM_INFO_DIVIDE_ACK, kPacket);
	}

	if (kPacket.m_nOK == 0) { // 성공일 경우에만 룸 유저 정보를 가지고 있는다.
		m_vecIndoorUserTemp = kJoinRoomAckInfo.m_veckInDoorUserInfo;
	}

	// SEND_COMPRESS_PACKET( EVENT_JOIN_ROOM_ACK, kPacket );  //성공 실패 모두 Req유저에게 응답을 해준다.
	if (kPacket.m_nOK == 0 && m_pkRoom->GetChatEvent(kChatEventInfo)) {
		SEND_COMPRESS_PACKET(EVENT_CHATTING_EVENT_MSG_NOT, kChatEventInfo);
		START_LOG(clog, L"채팅 이벤트 등록됨 Name : " << m_strName)
			<< BUILD_LOG(kChatEventInfo.m_nUID) << END_LOG;
	}

	if (kPacket.m_nOK != 0) {
		m_pkChannel->m_spRoomManager->IncreaseMatchFailCount(kPacket_.m_usRoomID);
		m_mapMatchParty.clear();
		ExitParty();
	}

	if (SiKGSSimLayer()->CheckTypeFlag(ST_DP_LOG)) {
		DumpIndoorUserInfo(kJoinRoomAckInfo.m_veckInDoorUserInfo);
	}

	START_LOG_WITH_NAME(clog)
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(m_iP2PVersion)
		<< BUILD_LOG(kPacket_.m_uiJoinType)
		<< BUILD_LOG(kPacket_.m_usRoomID)
		<< BUILD_LOG(kPacket_.m_strRoomPasswd)
		<< BUILD_LOGc(kPacket_.m_cQuickJoinCategory)
		<< BUILD_LOG(kPacket_.m_nQuickJoinModeID)
		<< BUILD_LOG(kPacket_.m_vecKickedRoomID.size())
		<< BUILD_LOG(kPacket_.m_nDifficult)
		<< BUILD_LOG(kJoinRoomAckInfo.m_kRoomInfo.m_strRoomName)
		<< BUILD_LOG(kJoinRoomAckInfo.m_kRoomInfo.m_bGuild)
		<< BUILD_LOG(kJoinRoomAckInfo.m_veckInDoorUserInfo.size())
		<< BUILD_LOG(GetStateIDString()) << END_LOG;
}

_IMPL_ON_FUNC(EVENT_START_GAME_REQ, KStartGameReq)
{
	KEVENT_START_GAME_BROAD kPacket;
	kPacket.m_nOK = -10;
	kPacket.m_dwUniqueNumber = 0;
	kPacket.m_dwHostUID = 0;
	kPacket.m_nStartStage = kPacket_.m_nStartStage;

	SET_ERROR(ERR_UNKNOWN);

	if (m_pkRoom == NULL) {
		SET_ERR_GOTO(ERR_ROOM_49, END_PROC);
	}

	START_LOG_WITH_NAME(clog)
		<< BUILD_LOG(m_pkRoom->m_iGameMode)
		<< BUILD_LOG(m_pkRoom->GetName())
		<< BUILD_LOG(m_pkRoom->GetCount()) << END_LOG;

	if (SiKGSSimLayer()->CheckTypeFlag(ST_INDIGO)) {
		// 용섬인 경우 선행 처리 할게 있다..
		std::map<std::wstring, KInDoorUserInfo>::iterator mit;
		for (mit = m_pkRoom->m_mapInDoorUserInfo.begin(); mit != m_pkRoom->m_mapInDoorUserInfo.end(); ++mit)
			PreStartIndigo(mit->second);
	}

	if (SiKGSSimLayer()->CheckTypeFlag(ST_TOURNAMENT)) { // 점령전 대회서버 모드일 경우..,
		if (SiKGCHelper()->IsTournamentCheck() == false) {
			SET_ERR_GOTO(ERR_ROOM_68, END_PROC);
		}
	}

	if (GetStateID() != KUserFSM::STATE_ROOM) {
		SET_ERR_GOTO(ERR_USER_00, END_PROC);
	}

	if (!CheckRequireItem()) {
		SET_ERR_GOTO(ERR_ROOM_21, END_PROC);
	}

	if (m_pkRoom->m_iGameMode == GC_GM_AUTOMATCH_TEAM) { // 대전매칭 모드일 경우..,
		m_pkRoom->SetMapID(kPacket_.m_nMapID);
		SetMatchRoomEnable(false);
	}

	if (m_pkRoom->StartGame(*this) == false) {  // 내부에서 에러세팅  
		goto END_PROC;
	}

	kPacket.m_dwRamdomSeed = ::GetTickCount(); // 렌덤 씨드
	m_pkRoom->SetRandomSeed(kPacket.m_dwRamdomSeed);
	kPacket.m_dwUniqueNumber = m_pkRoom->GetUniqueNumber();

	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError())
	{
		ERR_CASE(NET_OK, kPacket.m_nOK, 0);  // 성공
		ERR_CASE(ERR_USER_00, kPacket.m_nOK, -1);  // 해당 유저의 상태가 올바르지 않음.
		ERR_CASE(ERR_ROOM_48, kPacket.m_nOK, -2);  // 길드존에서 게임을 시작하려고 했음.
		ERR_CASE(ERR_ROOM_49, kPacket.m_nOK, -3);  // 룸 포인터가 NULL이다
		ERR_CASE(ERR_ROOM_08, kPacket.m_nOK, -4);  // 현재 방의 상태가 WAITING이나 FULL이 아님
		ERR_CASE(ERR_ROOM_20, kPacket.m_nOK, -5);  // 요청자가 방장 또는 운영자가 아님 
		ERR_CASE(ERR_ROOM_21, kPacket.m_nOK, -6);  // 요청자가 필요한 아이템을 가지고 있지 않음
		ERR_CASE(ERR_ROOM_22, kPacket.m_nOK, -7);  // 혼자서 게임을 시작하려고 시도함.
		ERR_CASE(ERR_ROOM_23, kPacket.m_nOK, -8);  // 현재 게임모드 클리어 조건 단계가 유저의 클리어 단계보다 높다.
		ERR_CASE(ERR_ROOM_24, kPacket.m_nOK, -9);  // 지원 되지 않는 Mode Level 입니다.
		ERR_CASE(ERR_ROOM_29, kPacket.m_nOK, -10);  // 게임을 시작할 수 없음. 모든 유저가 Ready상태가 아님.
		ERR_CASE(ERR_ROOM_61, kPacket.m_nOK, -11);  // 영웅던전 모드가 비활성화 상태.
		ERR_CASE(ERR_ROOM_62, kPacket.m_nOK, -12);  // 영웅던전 정보가 없음.
		ERR_CASE(ERR_ROOM_63, kPacket.m_nOK, -13);  // 영웅던전 최소 레벨조건을 만족하는 캐릭터가 없음.
		ERR_CASE(ERR_ROOM_64, kPacket.m_nOK, -14);  // 영웅던전 입장 시간이 아닙니다.
		ERR_CASE(ERR_ROOM_65, kPacket.m_nOK, -15);  // 해당 시간대 영웅던전을 이미 플레이했음.
		ERR_CASE(ERR_ROOM_66, kPacket.m_nOK, -16);  // 영웅던전 일일 클리어할 수 있는 최대 회수를 플레이했음.
		ERR_CASE(ERR_ROOM_68, kPacket.m_nOK, -17);  // 게임을 진행할 수 있는 시간이 아닙니다.
	default:
		START_LOG_WITH_NAME(cerr)
			<< BUILD_LOG(NetError::GetLastNetErrMsg())
			<< END_LOG;
		kPacket.m_nOK = -99; // unknown error
		break;
	}

	if (kPacket.m_nOK != 0) {  //실패 했으면 패킷 날리고 종료.  
		SEND_PACKET(EVENT_START_GAME_ACK);
		JIF(m_pkRoom);
		if (m_pkRoom->m_iGameMode == GC_GM_AUTOMATCH_TEAM) { // 대전매칭 모드일 경우..,
			SendToStartFailMatchMemberNot();
		}
		START_LOG(clog, L" 게임 시작 요청 , Name : " << m_strName)
			<< BUILD_LOG(NetError::GetLastNetErrMsg())
			<< BUILD_LOG(GetStateIDString())
			<< BUILD_LOGc(m_cAuthLevel)
			<< BUILD_LOG(kPacket.m_nOK)
			<< BUILD_LOG(kPacket.m_dwRamdomSeed)
			<< BUILD_LOG(kPacket.m_mapEvents.size())
			<< BUILD_LOG(kPacket.m_dropData.m_vecMissionDrop.size())
			<< BUILD_LOG(kPacket.m_dropData.m_vecMonsterDrop.size())
			<< BUILD_LOG(kPacket.m_vecChampions.size()) << END_LOG;
		return;
	}

	// 성공 했으면 드랍 관련 처리를 해서 패킷을 만든다.
	m_pkRoom->GetStartingUser(kPacket.m_vecStartingUsers);
	m_pkRoom->InitIDPlayTime(kPacket.m_vecStartingUsers); // 무한던전용 플레이시간 정보를 시작 멤버로 초기화(시작시 파티원 수 정보로 활용할 수 있다)

	CheckDrop(kPacket); // 내부에서 룸에 드랍세팅을 저장한다
	CheckEvents(kPacket);

	// 유저들의 인벤 정보를 보내줘야 한다.
	m_pkRoom->GetUserInvenInfo(kPacket.m_mapUserInvenInfo);

	m_pkRoom->ClearPlayingHost();
	if (SiKGSSimLayer()->IsPingEnable()) {

		m_pkRoom->MigrationForPlaying(); // 플레이 방장을 교체 한다.
	}
	else {
		m_pkRoom->SameMigrationForPlaying(); // 플레이 방장을 교체 안한다.
	}

	kPacket.m_dwHostUID = m_pkRoom->GetPlayingHostUID();
	m_pkRoom->GetChangeRoomInfo(kPacket.m_kRoomInfo);

	// 점령전 모드 추가에 따른 해킹 방지를 위한 정보 전달.
	CheckGameInfo(kPacket);

	// 호위 이벤트 활성 여부 패킷에 세팅
	CheckEscortInfo(kPacket);

	// 모험 이벤트 처리
	m_pkRoom->SetAdventureUserCollectDataPrev();  // 모험 이벤트 유저 전리품 정보 저장
	CheckAdventureEvent();  // 모험 이벤트 전리품 드랍하는 몬스터 정보 전달

	m_pkRoom->GetFatigueUserList(kPacket.m_mapUserFatigueRatio);
	m_pkRoom->BroadPacket(KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_START_GAME_BROAD, kPacket, true);
	m_pkRoom->AddPromotionCount(kPacket_.m_vecPromotionCount);

	AddStatisticsMap();
	AddStatisticsDeathMatch();
	AddStatisticsAvgPing();
	AddStatisticsFatalDeathMatch();
	AddStatisticsPvPPlayCharacters();

	START_LOG(clog, L" 게임 시작 요청 , Name : " << m_strName)
		<< BUILD_LOG(NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetStateIDString())
		<< BUILD_LOGc(m_cAuthLevel)
		<< BUILD_LOG(m_pkRoom->GetName()) // 룸 포인터가 널이면 위에서 리턴 된다..
		<< BUILD_LOG(m_pkRoom->GetCount())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket.m_dwRamdomSeed)
		<< BUILD_LOG(kPacket.m_dwUniqueNumber)
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_dwHostUID)
		<< BUILD_LOG(kPacket.m_mapEvents.size())
		<< BUILD_LOG(kPacket.m_dropData.m_vecMissionDrop.size())
		<< BUILD_LOG(kPacket.m_dropData.m_vecMonsterDrop.size())
		<< BUILD_LOG(kPacket.m_dropData.m_vecGpDrop.size())
		<< BUILD_LOG(kPacket.m_vecChampions.size())
		<< BUILD_LOG(kPacket.m_vecEventMonster.size())
		<< BUILD_LOG(kPacket.m_kRoomInfo.m_usRoomID)
		<< BUILD_LOGc(kPacket.m_kRoomInfo.m_cGameCategory)
		<< BUILD_LOG(kPacket.m_kRoomInfo.m_iGameMode)
		<< BUILD_LOG(kPacket.m_kRoomInfo.m_iSubGameMode)
		<< BUILD_LOG(kPacket.m_kRoomInfo.m_iMapID)
		<< BUILD_LOG(kPacket.m_kRoomInfo.m_usMaxUsers)
		<< BUILD_LOG(kPacket.m_kRoomInfo.m_nDifficulty)
		<< BUILD_LOG(kPacket.m_kRoomInfo.m_iMapID)
		<< BUILD_LOG(kPacket.m_nStartStage)
		<< END_LOG;

	DumpDropData(kPacket.m_dropData.m_vecMissionDrop);
	DumpDropData(kPacket.m_dropData.m_vecMonsterDrop);
	DumpDropData(kPacket.m_dropData.m_vecGpDrop);
	DumpEventMonsterInfo(kPacket.m_vecEventMonster);
}

_IMPL_ON_FUNC(EVENT_LEAVE_GAME_REQ, KGameResultIn)
{
	_VERIFY_STATE((2, KUserFSM::STATE_PLAYING, KUserFSM::STATE_ROOM));

	// 060622. florist. 방장의 게임종료 처리상 시간차로 인해 다른 팀원이 LEAVE_GAME_REQ를
	// 전송할 수도 있다. 이미 경기가 끝난 유저가 보내는 패킷은 무시한다.
	if (GetStateID() == KUserFSM::STATE_ROOM) return;

	KEndGameReq kReq;
	KGameInfo kInfo;
	KEndGameAck kAck;
	std::vector<float> vecAtkData;
	std::vector<float> vecIncAPRatio;
	bool bUseAllKill;
	bool bMonsterHPHack;
	std::map< int, KDungeonPersonalRecord > mapDungeonPersonalRecord;
	int nCharType;
	DWORD dwCharLevel;

	// 해킹체크용 데이터 swap
	vecAtkData.swap(kPacket_.m_kHactkData.m_vecAtkData);
	vecIncAPRatio.swap(kPacket_.m_kHactkData.m_vecIncAPRatio);
	bUseAllKill = kPacket_.m_kHactkData.m_bUseAllKill;
	bMonsterHPHack = kPacket_.m_kHactkData.m_bMonsterHPHack;
	mapDungeonPersonalRecord.swap(kPacket_.m_mapDungeonPersonalRecord);

	JIF(m_pkRoom);
	m_pkRoom->InsertLeftUser(GetUID());

	AddStatLeaveGame(kPacket_);
	AddStatDungeon(kPacket_, KEVENT_STAT_DUNGEON_STAGE_NOT::LEAVE_GAME, 1); // 던전 통계

	bool bIsBossStage = kPacket_.m_bIsBossStage;
	kReq.Clear();
	kPacket_.Clear();
	// 해킹체크용 데이터 swap
	kPacket_.m_kHactkData.m_vecAtkData.swap(vecAtkData);
	kPacket_.m_kHactkData.m_vecIncAPRatio.swap(vecIncAPRatio);
	kPacket_.m_kHactkData.m_bUseAllKill = bUseAllKill;
	kPacket_.m_kHactkData.m_bMonsterHPHack = bMonsterHPHack;

	kReq.m_vecGameResult.push_back(kPacket_);
	m_pkRoom->GetGameInfo(kInfo);
	LIF(AdjustExpGP(KRoom::PT_LEAVE_GAME, kInfo, kReq, kAck));

	// 현재 레벨업한 정보가 있는지 확인.
	if (m_pkRoom->CheckLevelUpEnable(GetUID(), nCharType, dwCharLevel)) {
		if (SiKSurvey()->IsRun()) {
			LeaveGameCheckSurveyInfo(nCharType, dwCharLevel, GetName());
		}
		CheckJobMissionGuide(GetUID(), static_cast<char>(nCharType), static_cast<int>(dwCharLevel));

		// 봉인 캐릭터 정보 갱신
		UpdateUserSealedCharInfo(GetThisPtr(), static_cast<char>(nCharType), static_cast<int>(dwCharLevel));
	}

	// 헬모드 게임포기시.
	if (m_pkRoom->IsHellMode() == true) {
		// 태국 정책 - 보스 스테이지이면 감소
		if (SiKGSSimLayer()->CheckAuthTypeFlag(KGSSimLayer::AT_FUNBOX_TH) == false || kPacket_.m_bIsBossStage == true) {

			// 게임종료시 입장권 삭제하는 경우.
			if (SiKGCHelper()->HellItemCheckEnable(m_pkRoom->m_iGameMode) == true &&
				SiKGCHelper()->IsEndGameHellTicketDec() == true)
			{
				if (m_kInventory.FindItemByItemID(SiKGCHelper()->GetDungeonFreePassID())) {
					START_LOG(clog, L"던전 프리패스 소유자라서 헬모드 티켓 감소 안함 Name : " << GetName()) << END_LOG;
				}
				//헬모드 입장권 깎기.
				else if (!CheckIfDurItemExist(SiKGCHelper()->GetHellItemID(), 1, true)) {
					START_LOG(cerr, L"헬모드 시작시 체크 했는데 아이템이 없음.. Name : " << GetName()) << END_LOG;
				}
			}
		}
	}

	{ // 던전 개인 최고 기록, 던전 랭킹
		std::vector< KDungeonRankInfo > vecDungeonRankInfo;
		vecDungeonRankInfo.clear();

		if (false == mapDungeonPersonalRecord.empty()) {
			UpdateDungeonPersonalRecordData(kInfo.m_iGameMode, static_cast<int>(GetCurrentCharType()), mapDungeonPersonalRecord); // 던전 개인 최고 기록 갱신

			KDungeonPersonalRecord& kDungeonPersonalRecord = mapDungeonPersonalRecord.rbegin()->second; // 던전 개인 최고 기록 중 마지막 스테이지 정보를 현재 시즌 랭크에 사용한다
			KDungeonRankInfo kDungeonRankInfo;
			kDungeonRankInfo.m_dwLoginUID = GetUID();
			kDungeonRankInfo.m_nModeID = kInfo.m_iGameMode;
			kDungeonRankInfo.m_nStageNum = kDungeonPersonalRecord.m_nStageNum;
			kDungeonRankInfo.m_dwStageClearTime = kDungeonPersonalRecord.m_dwStageClearTime;
			kDungeonRankInfo.m_wstrNickName = GetNickName();
			kDungeonRankInfo.m_cCharType = GetCurrentCharType();
			kDungeonRankInfo.m_tmDungeonStart = KncUtil::TimeToInt(m_pkRoom->m_tmGameStartTime);

			// 운영자 계정의 기록은 테섭일때만 랭킹에 반영되도록 한다
			if (true == IsAdmin()) {
				if (true == SiKGSSimLayer()->CheckTypeFlag(ST_INTERNAL_TESOP)) {
					vecDungeonRankInfo.push_back(kDungeonRankInfo);
				}
			}
			else {
				vecDungeonRankInfo.push_back(kDungeonRankInfo);
			}
		}
		// 현재 시즌 랭크 추가
		SiKDungeonRankManager()->AddDungeonRankCurrentSeason(kInfo.m_iGameMode, vecDungeonRankInfo);
	}

	if (!m_pkRoom->LeaveGame(*this)) {     // 게임 포기
		PRINT_LASTERROR;
	}

	if (m_pkRoom->IsRoomLastUser()) {
		QUEUEING_EVENT_TO_DB(DB_EVENT_USER_DUNGEON_LOADING_INFO, kPacket_.m_kUserLoadingStat);

		if (false == kPacket_.m_kMonsterTypeStat.m_mapMonsterTypeCount.empty()) {
			kPacket_.m_kMonsterTypeStat.m_dwUserUID = GetUID();
			kPacket_.m_kMonsterTypeStat.m_bClear = kPacket_.m_bWin;
			kPacket_.m_kMonsterTypeStat.m_dwPlayTime = m_pkRoom->GetLatestPlayTime() / (1000);
			QUEUEING_EVENT_TO_DB(DB_EVENT_MONSTER_TYPE_STAT_INFO, kPacket_.m_kMonsterTypeStat);
		}

		// 무한던전 통계 기록(모두 leavegame시)
		AddStatInfinityDungeon(KDB_EVENT_INFINITY_DUNGEON_STAT_NOT::LEAVE_GAME);
	}

	// PlayTime 저장. 룸객체의 LeaveGame 처리 이후 작업.
	KUserPtr spUser(SiKGSSimLayer()->m_kActorManager.GetByName<KUser>(kPacket_.m_strLogin));
	DWORD dwModeType;
	m_pkRoom->GetModeType(dwModeType);
	if (spUser) {
		spUser->AddPlayTime(m_pkRoom->GetLeaveGamePlayTime(), dwModeType); // User객체가 유효할 때 PlayTime 저장.
		// 게임 나갈때의 학습미션 처리 체크.
		if (!SiKMissionManager()->CheckControlMission(*spUser, kPacket_, m_pkRoom->m_iGameMode, m_pkRoom->m_nDifficulty)) {
			START_LOG(clog, L"??? ?? ??? ??")
				<< END_LOG;
		}
	}

	// 길드전 중간에 나가는 경우  길드 ID 저장
	if (false == m_pkRoom->FindInDoorUserSameGuild(GetUID(), GetGuildUID())) {
		m_pkRoom->SetLeaveGuildWhilePlaying(GetGuildUID());
	}

	// gc는 경기도중 게임을 나가면 방에서도 바로 빠져나가도록 되어있다.
	if (!m_pkRoom->Leave(*this, KRoom::LEAVE_SELF_DECISION)) {
		START_LOG(cerr, L"LEAVE ROOM ??!!")
			<< BUILD_LOG(GetName())
			<< BUILD_LOG(m_bGoToGuildZone)
			<< END_LOG;
	}

	START_LOG_WITH_NAME(clog)
		<< BUILD_LOG(kPacket_.m_strLogin)
		<< BUILD_LOG(kPacket_.m_vecCharExp.size())
		<< BUILD_LOG(kPacket_.m_nBaseGP)
		<< BUILD_LOG(kPacket_.m_nTotalRecvGP)
		<< BUILD_LOG(kPacket_.m_iContPoint)
		<< BUILD_LOG(kPacket_.m_bWin)
		<< BUILD_LOG(kPacket_.m_iNumKills)
		<< BUILD_LOG(kPacket_.m_iNumDies)
		<< BUILD_LOG(kPacket_.m_nScore)
		<< BUILD_LOG(kPacket_.m_kHactkData.m_vecAtkData.size())
		<< BUILD_LOG(kPacket_.m_kHactkData.m_vecIncAPRatio.size())
		<< BUILD_LOG(kPacket_.m_kHactkData.m_bUseAllKill)
		<< BUILD_LOG(kPacket_.m_kHactkData.m_bMonsterHPHack)
		<< BUILD_LOG(GetStateIDString())
		<< BUILD_LOG(GetStateIDString())
		<< BUILD_LOG(kPacket_.m_nClearStageCount)
		<< BUILD_LOG(kPacket_.m_nLastStageID)
		<< BUILD_LOG(kPacket_.m_nLifeCount)
		<< BUILD_LOG(kPacket_.m_nRegenCount)
		<< BUILD_LOG(kPacket_.m_nResult)
		<< BUILD_LOG(kPacket_.m_bIsBossStage) << END_LOG;

	// 게임도중 나가도, 게임내에서 레벨업시 획득한 보상을 받도록.
	KEVENT_SHANGHAI_DROP_REQ kShDrop;
	kShDrop.m_cCharType = GetCurrentCharType();
	CheckLevelUpDrop(kShDrop);

	KGameResultOut kPacket;
	if (!kAck.m_vecGameResult.empty()) {
		kPacket = *kAck.m_vecGameResult.begin();
		// 미션 정보 갱신.
		UpdateMissionInfo(kPacket.m_vecMissionSlot);
		UpdateCollectionInfo(kPacket.m_vecCollectionSlot);
	}

	SEND_PACKET(EVENT_LEAVE_GAME_ACK);

	// 유저의 모험 이벤트 전리품 정보를 DB에 저장하고 유저에게 전달
	if (SiKAdventure()->IsRun()) {
		// 유저 전리품 수가 MaxCount를 넘지 않도록 보정 처리
		if (false == SiKAdventure()->AdjustCollectData(m_kAdventureUserData.m_mapCollectData)) {
			START_LOG(cerr, L"모험 이벤트 게임 나가기에서 유저 전리품 수 MaxCount로 보정됨") << END_LOG;
		}

		std::map< int, std::map< DWORD, int > > mapCollectData = m_kAdventureUserData.m_mapCollectData;
		SiKAdventure()->GetCollectDataDiff(m_kAdventureUserData.m_mapCollectDataPrev, mapCollectData);
		if (m_kEventCheck.EventIn(KUserEvent::EVENT_ADVENTURE_USER_COLLECT_DATA_NOT)) {
			QUEUEING_EVENT_TO_DB(EVENT_ADVENTURE_USER_COLLECT_DATA_NOT, mapCollectData);
		}
		else {
			START_LOG(cerr, L"모험 이벤트 전리품 획득 패킷 중복처리에서 걸림(나가기)") << END_LOG;
		}

		// DB 요청 한번 날린 후 prev 데이터를 현재 정보로 보정함
		m_kAdventureUserData.m_mapCollectDataPrev = m_kAdventureUserData.m_mapCollectData;

		SEND_DATA(EVENT_ADVENTURE_USER_COLLECT_DATA_NOT, m_kAdventureUserData.m_mapCollectData);
		START_LOG(clog, L"유저의 모험 이벤트 전리품 정보 전달. Login : " << GetName()) << END_LOG;
	}

	ExitParty();
	m_mapMatchParty.clear();
}

_IMPL_ON_FUNC(EVENT_END_GAME_REQ, KEndGameReq)
{
	_VERIFY_STATE((1, KUserFSM::STATE_PLAYING));

	JIF(m_pkRoom);
	if (this != m_pkRoom->GetPlayingHost() && !IsAdmin()) {
		START_LOG(cwarn, L"방장 또는 운영자가 아닌데 게임을 종료하려 하였다. LoginID : " << m_strName) << END_LOG;
	}

	START_LOG(clog, L"END_GAME_REQ 처리 시작.") << END_LOG;

	// 던전 통계
	for (std::vector<KGameResultIn>::iterator vit = kPacket_.m_vecGameResult.begin(); vit != kPacket_.m_vecGameResult.end(); ++vit) {
		// 해당 유저를 찾을 수 없음.
		KUserPtr spUser(SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>(vit->m_dwUID));
		if (!spUser) continue;
		spUser->AddStatDungeonAdd(*vit);
		spUser->AddStatPVPBalance(*vit);
	}

	KInsertDropItem kInsertDropItem;
	kInsertDropItem.m_mapEachUserDrops.clear();
	kInsertDropItem.m_kEndGameAck.m_vecGuildBPoint.clear();

	{ // 클라이언트에 필요한 정보, REQ에서 받아서 ACK 에 바로 할당함.(서버는 전달만 함)
		kInsertDropItem.m_kEndGameAck.m_uiMVP = kPacket_.m_uiMVP;
		kInsertDropItem.m_kEndGameAck.m_bIsBlueWin = kPacket_.m_bIsBlueWin;
		kInsertDropItem.m_kEndGameAck.m_vecIsPlayerWin = kPacket_.m_vecIsPlayerWin;
	}

	KGameInfo kGameInfo;
	m_pkRoom->GetGameInfo(kGameInfo);

	if (SiKGSSimLayer()->CheckTypeFlag(ST_DP_LOG)) {
		DumpEndGameInfo(kGameInfo, kPacket_);
	}

	// 상태 체크, 유효성 검사.(게임시간이 짧은가? 등등등.. )
	if (!m_pkRoom->EndGame(kPacket_)) {
		PRINT_LASTERROR;
	}

	// PlayTime 저장을 위해서 던전 통계 남기는 부분을 Room의 EndGame처리 이후에 작업하게 순서 수정.
	// 던전 통계 & PlayTime 저장
	int nTotalUsers = kPacket_.m_vecGameResult.size();
	std::vector< KDungeonRankInfo > vecDungeonRankInfo;
	vecDungeonRankInfo.clear();
	for (std::vector<KGameResultIn>::iterator vit = kPacket_.m_vecGameResult.begin(); vit != kPacket_.m_vecGameResult.end(); ++vit) {
		// 해당 유저를 찾을 수 없음.
		KUserPtr spUser(SiKGSSimLayer()->m_kActorManager.GetByName<KUser>(vit->m_strLogin));
		if (NULL == spUser) continue;

		AddStatDungeon(*vit, KEVENT_STAT_DUNGEON_STAGE_NOT::END_GAME, nTotalUsers);
		DWORD dwModeType;
		m_pkRoom->GetModeType(dwModeType);
		spUser->AddPlayTime(m_pkRoom->GetEndGamePlayTime(), dwModeType); // User객체가 유효할 때 PlayTime 저장.
		if (vit->m_dwUID == GetUID()) { // 방장의 던전 로딩 통계 정보, 몬스터 Stat정보.
			QUEUEING_EVENT_TO_DB(DB_EVENT_USER_DUNGEON_LOADING_INFO, vit->m_kUserLoadingStat);
			if (!vit->m_kMonsterTypeStat.m_mapMonsterTypeCount.empty()) {
				vit->m_kMonsterTypeStat.m_dwUserUID = GetUID();
				vit->m_kMonsterTypeStat.m_bClear = vit->m_bWin;
				vit->m_kMonsterTypeStat.m_dwPlayTime = m_pkRoom->GetLatestPlayTime() / (1000);
				QUEUEING_EVENT_TO_DB(DB_EVENT_MONSTER_TYPE_STAT_INFO, vit->m_kMonsterTypeStat);
			}
		}

		// 각 유저의 던전 난이도별 최고 기록 저장 (컨트롤 피드백 시스템), 던전 개인 최고 기록 갱신, 현재 시즌 던전 랭크 갱신
		int nCharType = static_cast<int>(vit->m_vecCharExp.begin()->m_cCharType);
		if (true == vit->m_bWin) {
			spUser->UpdateDungeonBestRecordData(*spUser, kGameInfo.m_iGameMode, kGameInfo.m_nDifficulty, vit->m_kUserControlInfoData.m_nRank, kPacket_.m_dwElapsedSec); // 컨트롤 피드백 시스템
		}

		if (false == vit->m_mapDungeonPersonalRecord.empty()) {
			spUser->UpdateDungeonPersonalRecordData(kGameInfo.m_iGameMode, nCharType, vit->m_mapDungeonPersonalRecord); // 던전 개인 최고 기록 갱신

			KDungeonPersonalRecord& kDungeonPersonalRecord = vit->m_mapDungeonPersonalRecord.rbegin()->second; // 던전 개인 최고 기록 중 마지막 스테이지 정보를 현재 시즌 랭크에 사용한다
			KDungeonRankInfo kDungeonRankInfo;
			kDungeonRankInfo.m_dwLoginUID = vit->m_dwUID;
			kDungeonRankInfo.m_nModeID = kGameInfo.m_iGameMode;
			kDungeonRankInfo.m_nStageNum = kDungeonPersonalRecord.m_nStageNum;
			kDungeonRankInfo.m_dwStageClearTime = kDungeonPersonalRecord.m_dwStageClearTime;
			kDungeonRankInfo.m_wstrNickName = spUser->GetNickName();
			kDungeonRankInfo.m_cCharType = vit->m_vecCharExp.begin()->m_cCharType;
			kDungeonRankInfo.m_tmDungeonStart = KncUtil::TimeToInt(m_pkRoom->m_tmGameStartTime);

			// 운영자 계정의 기록은 테섭일때만 랭킹에 반영되도록 한다
			if (true == spUser->IsAdmin()) {
				if (true == SiKGSSimLayer()->CheckTypeFlag(ST_INTERNAL_TESOP)) {
					vecDungeonRankInfo.push_back(kDungeonRankInfo);
				}
			}
			else {
				vecDungeonRankInfo.push_back(kDungeonRankInfo);
			}
		}
	}
	// 현재 시즌 랭크 추가
	SiKDungeonRankManager()->AddDungeonRankCurrentSeason(kGameInfo.m_iGameMode, vecDungeonRankInfo);

	// 무한던전 통계 기록(endgame시)
	AddStatInfinityDungeon(KDB_EVENT_INFINITY_DUNGEON_STAT_NOT::END_GAME);

	//유효성 검사.(게임시간이 짧은가? 등등등.. )
	DWORD dwUniq = m_pkRoom->GetUniqueNumber();
	m_pkRoom->BroadPacket(KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_END_GAME_NOT, dwUniq);

	KDropData dropData; // 드랍 정보를 여기서 써야 하기 때문에 위로 올림.
	m_pkRoom->GetDropList(dropData);
	m_pkRoom->ClearDropInfo();

	std::vector<KEventMonster> vecEventMonInfo;
	m_pkRoom->GetEventMonInfo(vecEventMonInfo);
	m_pkRoom->ClearEventMonInfo();

	std::map< std::pair<DWORD, char>, __int64 > mapResultExp;
	mapResultExp.clear();
	m_pkRoom->GetResultExp(mapResultExp);
	m_pkRoom->ClearResultExp();

	// 유저의 레벨업 정보 [UserUID, <CharType, Level>]
	std::map<DWORD, std::pair<int, DWORD>> mapLevelUpUser;
	mapLevelUpUser.clear();
	m_pkRoom->GetLevelUpUser(mapLevelUpUser);

	std::map<DWORD, int> mapResultGp;
	mapResultGp.clear();
	m_pkRoom->GetResultGP(mapResultGp);
	if (false == m_pkRoom->IsDungeonMode(kGameInfo.m_iGameMode)) {
		m_pkRoom->ClearResultGP();
	}

	std::map<std::pair<int, int>, std::map<PAIR_DWORD, USHORT> > mapMonDropGP;
	m_pkRoom->GetMonDropGP(mapMonDropGP);
	m_pkRoom->ClearMonDropGP();

	std::map<std::pair<int, int>, int> mapStageMonKillCount;
	mapStageMonKillCount.clear();
	m_pkRoom->GetMonsterKillInfo(mapStageMonKillCount);
	m_pkRoom->ClearMonsterKillInfo();

	// 게임 결과 처리.
	SiKResultManager()->CalcExpGP(kGameInfo, mapResultExp, mapResultGp, kPacket_); // 기본적으로 주어지는 경험치 계산은 끝났다. 
	if (SiKGSSimLayer()->CheckTypeFlag(ST_DP_LOG)) {
		DumpEndGameInfo(kGameInfo, kPacket_);
		DumpEventMonsterInfo(vecEventMonInfo);
		DumpMonsterDropGP(mapMonDropGP);
	}

	// 게임획득 기본GP값 저장
	SetEndGameResultGPInfo(kPacket_.m_vecGameResult);

	//  080604  woosh. 대전 5킬 이벤트
	std::vector< std::wstring > vecStrLogin;
	PVP_NKillEvent(kPacket_.m_vecGameResult, vecStrLogin);

	// 해당 플레이에 할당된 이벤트가 존재하면 가져와서 처리 한다.
	std::map< std::pair<DWORD, char>, std::vector<sTimeEvent> > mapActiveEventList;
	if (m_pkRoom->GetTimeEvent(mapActiveEventList)) {  // 룸에서 End Game시에 클리어 않하기 때문에 사용가능하다.  
		SiKTimeEventManager()->CalcBoostInfo(kPacket_, mapActiveEventList, kGameInfo, vecStrLogin); //  080603  woosh.  CheckPVPEvent() 대전5킬 이벤트는 이 안에
		if (SiKGSSimLayer()->CheckTypeFlag(ST_DP_LOG))
			DumpEndGameInfo(kGameInfo, kPacket_);

		SiKTimeEventManager()->CalcLevelBonus(kPacket_, mapActiveEventList); // 레벨별 이벤트
		SiKTimeEventManager()->CalcFriendEvent(kPacket_, mapActiveEventList); // 친구함께 이벤트.(온라인 메신저 친구)
	}
	AddStatistics(kGameInfo, kPacket_);

	// 게임에서 실시간으로 받은 보상을처리하자.
	// Exp/Gp도 처리하므로 약간 위에 위치한다. ㅜㅜ
	std::map< DWORD, DROP_ITEMS > mapEachUserDrops; // 게임중 드랍 받은것.
	SiKGameDrop()->DistributeDrops(dropData, kPacket_, mapEachUserDrops);
	SiKGameDrop()->DistributeRareDrops(dropData, kPacket_, mapEachUserDrops);

	std::map<DWORD, int> mapFatigueUserList; // 피로도 해당 유저
	m_pkRoom->GetFatigueUserList(mapFatigueUserList);
	SiKGameDrop()->DeleteFatigueUserDistributeItem(mapEachUserDrops, mapFatigueUserList); // 중국 피로 유저의 아이템 제거

	//BR.헬모드 끝냈다.
	DecreaseHellModeTicket(kPacket_.m_vecGameResult);

	// 현재 게임 클리어한 정보에 대하여 설문조사 정보가 있는지 확인 및 전달.
	if (SiKSurvey()->IsRun()) {
		CheckSurveyInfo(kGameInfo.m_iGameMode, kGameInfo.m_nDifficulty, kPacket_.m_vecGameResult, mapLevelUpUser);
	}

	// 레벨업한 정보 있는지 확인하고 전직 미션 가이드 체크
	CheckJobMissionInfo(kPacket_.m_vecGameResult);

	// 레벨업한 정보 있는지 확인하고 봉인 캐릭터 정보 갱신
	CheckUserSealedCharInfo(kPacket_.m_vecGameResult);

	// AdjustExpGP 는 룸이 유효하지 않거나 Short Game이면 실패한다, 그말은 그러하다.. 실패하면 드랍 해주지 말자.
	if (!AdjustExpGP(KRoom::PT_END_GAME, kGameInfo, kPacket_, kInsertDropItem.m_kEndGameAck)) {
		// 060630. jseop. 사용한 아이템 기록 지움.
		ClearUsedQuantityItem(kPacket_.m_vecGameResult);
		CalcRewardType(kGameInfo.m_iGameMode, kInsertDropItem.m_kEndGameAck.m_nRewardType, kInsertDropItem.m_kEndGameAck.m_vecGameResult); // 던전 보상상자 키 계산

		// 유저 프리미엄 정보 갱신
		for (std::vector<KGameResultIn>::iterator vitUser = kPacket_.m_vecGameResult.begin(); vitUser != kPacket_.m_vecGameResult.end(); ++vitUser) {
			KUserPtr spUser(SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>(vitUser->m_dwUID));
			if (spUser == NULL) continue;

			KInDoorUserInfo * pkInDoorUserInfo = m_pkRoom->FindInDoorUserInfo(m_strName);
			if (pkInDoorUserInfo != NULL) {
				CheckInventoryPremiumItem(pkInDoorUserInfo->m_kPremiumInfo);
				m_pkRoom->BroadPacket(KRoom::BROAD_TO_ALL_EX_ME, spUser->GetName(), KUserEvent::EVENT_CHANGE_INDOOR_USERINFO_BROAD, (*pkInDoorUserInfo), true);
			}
		}

		if (SiKGSSimLayer()->CheckTypeFlag(ST_CREATE_LIMIT)) { // 대회서버는 옵저버에게 종료 패킷을 보내야 함.
			std::vector<DWORD> vecRoomUserUIDs;
			m_pkRoom->GetTotalUserUIDList(vecRoomUserUIDs); // 대회서버의 경우 옵저버 포함 유저 전체 정보를 가져와야 한다.

			for (std::vector<DWORD>::iterator vitUserUID = vecRoomUserUIDs.begin(); vitUserUID != vecRoomUserUIDs.end(); ++vitUserUID) {
				// 해당 유저를 찾을 수 없음.
				KUserPtr spUser(SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>(*vitUserUID));
				if (!spUser) continue;

				spUser->QueueingEvent(KUserEvent::EVENT_END_GAME_BROAD, kInsertDropItem.m_kEndGameAck);
			}
			//m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_END_GAME_BROAD, kInsertDropItem.m_kEndGameAck, true );
		}
		else {
			for (std::vector<KGameResultIn>::iterator vitUser = kPacket_.m_vecGameResult.begin(); vitUser != kPacket_.m_vecGameResult.end(); ++vitUser) {
				// 해당 유저를 찾을 수 없음.
				KUserPtr spUser(SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>(vitUser->m_dwUID));
				if (!spUser) continue;

				spUser->QueueingEvent(KUserEvent::EVENT_END_GAME_BROAD, kInsertDropItem.m_kEndGameAck);
				// 결과 보상 시스템 문제 확인 로그.
				if (m_pkRoom->m_iGameMode == 76) {
					START_LOG(cerr, L"RAID Dungeon Hack Check")
						<< BUILD_LOG(vitUser->m_strLogin)
						<< END_LOG;
				}
			}
		}
		return;
	}
	if (SiKGSSimLayer()->CheckTypeFlag(ST_DP_LOG))
		DumpEndGameInfo(kGameInfo, kPacket_);

	AddStatistics2(kGameInfo, kInsertDropItem.m_kEndGameAck, mapStageMonKillCount);
	NotifyPremiumToRoomUsers();

	bool bShDrop = false;
	bool bResultDrop = false;

	bool bItemDropped = DivideEachShareOfDropItem(kPacket_, kInsertDropItem, bShDrop, bResultDrop);
	bool bPCBangDungeonDrop = PCBangDungeonItemDrop(kPacket_, kInsertDropItem);
	CheckGetLimit(bItemDropped, kInsertDropItem.m_mapEachUserDrops);  // GetLimit 이벤트 진행시 InitDrop으로 받을 아이템 리스트의 후처리를 한다
	bool bAdditionalDrop = AdditionalDrop(kPacket_, kInsertDropItem.m_mapEachUserDrops); // 미션 관련 처리가 이 안에 있음, 호위 이벤트 보상도 있음
	bool bGameDrop = SiKGameDrop()->AdjustDrops(mapEachUserDrops, kInsertDropItem.m_mapEachUserDrops); // 이름 하고는 ㅡㅡ;;; 앞에서 미리 뽀아놓은 드랍을 사용한다. 

	UpdateMissionInfo(kInsertDropItem.m_kEndGameAck);
	UpdateCollectionInfo(kInsertDropItem.m_kEndGameAck); //  080528  woosh. 칭호
	CalcRewardType(kGameInfo.m_iGameMode, kInsertDropItem.m_kEndGameAck.m_nRewardType, kInsertDropItem.m_kEndGameAck.m_vecGameResult); // 던전 보상상자 키 계산
	kInsertDropItem.m_kEndGameAck.m_bIsSpecialDropCharBox = IsSpecialDropCharacterBox(kGameInfo.m_nDifficulty); // 던전 보상상자 타입 전달

	// 유저의 모험 이벤트 전리품 정보를 DB에 저장
	if (SiKAdventure()->IsRun()) {
		std::vector< KGameResultIn >::iterator vit = kPacket_.m_vecGameResult.begin();
		for (; vit != kPacket_.m_vecGameResult.end(); ++vit) {
			KUserPtr spUser(SiKGSSimLayer()->m_kActorManager.GetByUID< KUser >(vit->m_dwUID));
			if (!spUser) continue;

			// 유저 전리품 수가 MaxCount를 넘지 않도록 보정 처리
			if (false == SiKAdventure()->AdjustCollectData(spUser->m_kAdventureUserData.m_mapCollectData)) {
				START_LOG(cerr, L"모험 이벤트 게임 종료 처리에서 유저 전리품 수 MaxCount로 보정됨") << END_LOG;
			}

			std::map< int, std::map< DWORD, int > > mapCollectData = spUser->m_kAdventureUserData.m_mapCollectData;
			SiKAdventure()->GetCollectDataDiff(spUser->m_kAdventureUserData.m_mapCollectDataPrev, mapCollectData);
			if (spUser->m_kEventCheck.EventIn(KUserEvent::EVENT_ADVENTURE_USER_COLLECT_DATA_NOT)) {
				_QUEUEING_EVENT_TO_DB(EVENT_ADVENTURE_USER_COLLECT_DATA_NOT, mapCollectData, spUser);
			}
			else {
				START_LOG(cerr, L"모험 이벤트 전리품 획득 패킷 중복처리에서 걸림(클리어)") << END_LOG;
			}

			// DB 요청 한번 날린 후 prev 데이터를 현재 정보로 보정함
			m_kAdventureUserData.m_mapCollectDataPrev = m_kAdventureUserData.m_mapCollectData;
		}
	}

	// 060630. jseop. 사용한 아이템 기록 지움.
	ClearUsedQuantityItem(kPacket_.m_vecGameResult);

	ApplyGuildBPoint(kPacket_, kInsertDropItem.m_kEndGameAck);

	DivideChannelByBPoint(kInsertDropItem.m_kEndGameAck);

	kInsertDropItem.m_kEndGameAck.m_bShDrop = bShDrop;

	if ((bGameDrop || bItemDropped || bAdditionalDrop || bPCBangDungeonDrop) && !kInsertDropItem.m_mapEachUserDrops.empty()) {
		ProcessDoubleCristalCard(kInsertDropItem);
		// 드랍된 아이템이 있으므로 DB를 업데이트 하고 결과를 받아와야한다.
		QUEUEING_EVENT_TO_DB(DB_EVENT_INSERT_DROP_ITEM_REQ, kInsertDropItem);
		return;
	}

	m_pkRoom->RollbackMigrationFromPlaying(kInsertDropItem.m_kEndGameAck.m_dwHostUID);

	if (SiKGSSimLayer()->CheckTypeFlag(ST_CREATE_LIMIT)) { // 대회서버는 옵저버에게 종료 패킷을 보내야 함.
		std::vector<DWORD> vecRoomUserUIDs;
		m_pkRoom->GetTotalUserUIDList(vecRoomUserUIDs); // 대회서버의 경우 옵저버 포함 유저 전체 정보를 가져와야 한다.

		for (std::vector<DWORD>::iterator vitUserUID = vecRoomUserUIDs.begin(); vitUserUID != vecRoomUserUIDs.end(); ++vitUserUID) {
			// 해당 유저를 찾을 수 없음.
			KUserPtr spUser(SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>(*vitUserUID));
			if (!spUser) continue;

			spUser->QueueingEvent(KUserEvent::EVENT_END_GAME_BROAD, kInsertDropItem.m_kEndGameAck);
		}
		//m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_END_GAME_BROAD, kInsertDropItem.m_kEndGameAck, true );
	}
	else {
		for (std::vector<KGameResultIn>::iterator vitUser = kPacket_.m_vecGameResult.begin(); vitUser != kPacket_.m_vecGameResult.end(); ++vitUser) {
			// 해당 유저를 찾을 수 없음.
			KUserPtr spUser(SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>(vitUser->m_dwUID));
			if (!spUser) continue;

			spUser->QueueingEvent(KUserEvent::EVENT_END_GAME_BROAD, kInsertDropItem.m_kEndGameAck);

			spUser->CheckCharacterGraduate(KEVENT_GRADUATE_CHARACTER_USER_INFO_NOT::EGCUI_ON_GRADUATE);
		}
	}
}

_IMPL_ON_FUNC(EVENT_END_GAME_ACK, KEndGameAck)
{
	SEND_COMPRESS_PACKET(EVENT_END_GAME_ACK, kPacket_);
}

// 060630. jseop. 사용한 아이템 찾기.
IMPL_ON_FUNC_NOPARAM(EVENT_END_RESULT_NOT) // 게임 결과가 나오고 결과마무리 되는 그 순간. 방장이 보낸다.
{
	VERIFY_STATE((1, KUserFSM::STATE_ROOM));

	JIF(m_pkChannel);
	JIF(m_pkRoom);

	m_pkRoom->EndResult(*this); // 로그가 많이 남아서 JIF없앰..

}

_IMPL_ON_FUNC(EVENT_CHANGE_ROOM_INFO_REQ, KChangeRoomInfo)
{
	SET_ERROR(ERR_UNKNOWN);

	if (GetStateID() != KUserFSM::STATE_ROOM || m_pkRoom == NULL || m_pkChannel == NULL) {
		kPacket_.m_nOK = 1;

		SET_ERROR(ERR_USER_00);
		SEND_RECEIVED_PACKET(EVENT_CHANGE_ROOM_INFO_BROAD);
		START_LOG(cerr, NetError::GetLastNetErrMsg())
			<< BUILD_LOG((GetStateID() != KUserFSM::STATE_ROOM))
			<< BUILD_LOG((m_pkRoom == NULL)) << END_LOG;
		return;
	}

	m_pkRoom->ChangeRoomInfo(this, kPacket_); //이 안에서 방장인지 어드민인지 확인한다.

//END_PROC:
	switch (NetError::GetLastNetError())
	{
		ERR_CASE(NET_OK, kPacket_.m_nOK, 0);
		ERR_CASE(ERR_ROOM_39, kPacket_.m_nOK, -1); // 방정보 변경 실패. 방장 부재
		ERR_CASE(ERR_ROOM_40, kPacket_.m_nOK, -2); // 방정보 변경 실패. 방장 또는 운영자가 아님
		ERR_CASE(ERR_ROOM_43, kPacket_.m_nOK, -3); // 방정보 변경 실패. 방 최대 유저 변경이 올바르지 않음.
		ERR_CASE(ERR_ROOM_44, kPacket_.m_nOK, -4); // 방정보 변경 실패. 닫으려는 슬롯에 유저가 존재함.
		ERR_CASE(ERR_ROOM_42, kPacket_.m_nOK, -5); // 방정보 변경 실패. 슬롯 변경 정보가 올바르지 않음
		ERR_CASE(ERR_ROOM_45, kPacket_.m_nOK, -6); // 방정보 변경 실패. 존재하지 않는 변경 정보를 보내옴.
		ERR_CASE(ERR_ROOM_41, kPacket_.m_nOK, -7); // 방정보 변경 실패. 선택할수 있는 게임 모드가 아님
		ERR_CASE(ERR_ROOM_47, kPacket_.m_nOK, -8); // 아직 선행 던전을 클리어 하지 못했음.
		ERR_CASE(ERR_ROOM_51, kPacket_.m_nOK, -9); // 헬모드에서. 헬모드로 방정보 변경 불가
		ERR_CASE(ERR_ROOM_67, kPacket_.m_nOK, -10);// 영웅던전 방정보 변경불가
		ERR_CASE(ERR_ROOM_70, kPacket_.m_nOK, -11);// 방정보 변경 실패. 게임모드 변경 가능한 모드가 아님
	default:
		START_LOG(cerr, L"알수없는 오류 정보")
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg())
			<< BUILD_LOG(GetName()) << END_LOG;
	}

	if (kPacket_.m_nOK != 0) {
		m_pkRoom->GetChangeRoomInfo(kPacket_);
	}
	else
	{
		m_pkRoom->BroadPacket(KRoom::BROAD_TO_ALL_EX_ME, m_strName, KUserEvent::EVENT_CHANGE_ROOM_INFO_BROAD, kPacket_);
	}

	SEND_RECEIVED_PACKET(EVENT_CHANGE_ROOM_INFO_BROAD);

	LOG_SUCCESS(kPacket_.m_nOK == 0 || kPacket_.m_nOK == -4 || kPacket_.m_nOK == -7)
		<< BUILD_LOG(NetError::GetLastNetErrMsg())
		<< BUILD_LOG(m_strName)
		<< BUILD_LOG(kPacket_.m_nOK)
		<< BUILD_LOG(kPacket_.m_usMaxUsers)
		<< BUILD_LOGc(kPacket_.m_cGameCategory)
		<< BUILD_LOG(kPacket_.m_iGameMode)
		<< BUILD_LOG(kPacket_.m_iSubGameMode)
		<< BUILD_LOG(kPacket_.m_bRandomableMap)
		<< BUILD_LOG(kPacket_.m_iMapID)
		<< BUILD_LOG(kPacket_.m_nMonsterID)
		<< BUILD_LOG(kPacket_.m_nMonsterCount)
		<< BUILD_LOGc(kPacket_.m_cRoutingMethod)
		<< BUILD_LOG(kPacket_.m_nDifficulty) << END_LOG;
}

_IMPL_ON_FUNC(EVENT_NMNETCAFE_LOGOUT_ACK, UINT)
{
	//if( kPacket_ == 0 )
	{
		m_bServerMigrationReserved = true; //정상적으로 DBup이 된경우 true
		SetTick(MIGRATION_TICK);
	}
	SetDisconnReason(DRW_SERVER_MIGRATION);

	int kPacket = 0;
	SEND_PACKET(EVENT_SERVER_MIGRATION_ACK);

	_LOG_SUCCESS(kPacket_ == 0, L"넷마블 로그아웃 처리 결과 : " << kPacket_)
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< END_LOG;
}

IMPL_ON_FUNC(EVENT_VITALITY_INFO_NOT)
{
	NetError::SetLastNetError(kPacket_.m_nOK);

	if (false == IS_CORRECT(NET_OK)) {
		goto END_PROC;
	}

	SET_ERROR(ERR_UNKNOWN);

	// 유저 객체에 활력 정보를 갱신한다
	m_kVitalityUserData.m_nRechargePoint = kPacket_.m_nRechargePoint;
	m_kVitalityUserData.m_nRechargePointCounter = kPacket_.m_nRechargePointCounter;
	m_kVitalityUserData.m_mapRemainVitality = kPacket_.m_mapRemainVitality;

	// 패킷에 나머지 스크립트 정보를 채운다
	kPacket_.m_nMaxVitalityPerDay = SiKVitalityManager()->GetMaxVitalityPerDay();
	kPacket_.m_nMaxVitalityForFirstCharacter = SiKVitalityManager()->GetVitalityForFirstCharacterCreation();
	kPacket_.m_nVitalityPerRechargePoint = SiKVitalityManager()->GetVitalityPerRechargePoint();

	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket_.m_nOK, NetError::NET_OK); // 성공
		ERR_CASE(ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98); // 아직 처리중인 작업
	default:
		kPacket_.m_nOK = 99;
		START_LOG(cerr, L"유저 활력 정보 조회 시 알수 없는 오류, Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
	}

	SEND_RECEIVED_PACKET(EVENT_VITALITY_INFO_NOT);

	START_LOG(clog, L"유저 활력 정보 알림 전송")
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(kPacket_.m_nOK)
		<< BUILD_LOG(kPacket_.m_nMaxVitalityPerDay)
		<< BUILD_LOG(kPacket_.m_nMaxVitalityForFirstCharacter)
		<< BUILD_LOG(kPacket_.m_nVitalityPerRechargePoint)
		<< BUILD_LOG(kPacket_.m_nRechargePoint)
		<< BUILD_LOG(kPacket_.m_nRechargePointCounter)
		<< BUILD_LOG(kPacket_.m_mapRemainVitality.size())
		<< END_LOG;
}

_IMPL_ON_FUNC(EVENT_VITALITY_RECHARGE_REQ, int)
{
	KEVENT_VITALITY_INFO_NOT kPacket;
	kPacket.m_nOK = -99;

	KDB_EVENT_VITALITY_RECHARGE_NOT kDBPacket;

	SET_ERROR(ERR_UNKNOWN);

	KCharacterInfo* pkCharacter = GetCharacterInfo(kPacket_);
	if (NULL == pkCharacter) {
		SET_ERR_GOTO(ERR_VITALITY_02, END_PROC);
	}

	// 해당 캐릭터의 활력을 재계산(충전)한다
	// 모든 보유중인 캐릭터들의 활력 정보를 유지하고 있다고 가정한다
	if (0 < m_kVitalityUserData.m_nRechargePoint) {
		if (false == m_kVitalityUserData.RechargeVitality(kPacket_)) {
			SET_ERROR(ERR_VITALITY_05);
		}
	}
	else {
		SET_ERROR(ERR_VITALITY_01);
	}

	// 패킷에 현재 활력 정보를 채운다
	kPacket.m_nRechargePoint = m_kVitalityUserData.m_nRechargePoint;
	kPacket.m_nRechargePointCounter = m_kVitalityUserData.m_nRechargePointCounter;
	kPacket.m_mapRemainVitality = m_kVitalityUserData.m_mapRemainVitality;

	// 패킷에 나머지 스크립트 정보를 채운다
	kPacket.m_nMaxVitalityPerDay = SiKVitalityManager()->GetMaxVitalityPerDay();
	kPacket.m_nMaxVitalityForFirstCharacter = SiKVitalityManager()->GetVitalityForFirstCharacterCreation();
	kPacket.m_nVitalityPerRechargePoint = SiKVitalityManager()->GetVitalityPerRechargePoint();

	if (false == IS_CORRECT(ERR_VITALITY_01) && false == IS_CORRECT(ERR_VITALITY_05)) {
		// 활력 충전 로그용 정보를 DB에 기록한다
		kDBPacket.m_nVitalityUpdateType = KVitalityManager::EVUT_RECHARGE;
		kDBPacket.m_nRechargePoint = m_kVitalityUserData.m_nRechargePoint;
		kDBPacket.m_nRechargePointCounter = m_kVitalityUserData.m_nRechargePointCounter;
		kDBPacket.m_nRemainVitality = m_kVitalityUserData.GetVitality(kPacket_);
		kDBPacket.m_kCharacterInfo = *pkCharacter;

		QUEUEING_EVENT_TO_DB(DB_EVENT_VITALITY_RECHARGE_NOT, kDBPacket);

		SET_ERROR(ERR_VITALITY_03);
	}

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, NetError::NET_OK); // 성공.
		ERR_CASE(ERR_VITALITY_01, kPacket.m_nOK, NetError::ERR_VITALITY_01); // 활력 충전 실패(사용할 수 있는 재충전 포인트가 없음).
		ERR_CASE(ERR_VITALITY_02, kPacket.m_nOK, NetError::ERR_VITALITY_02); // 활력 충전 실패(요청한 캐릭터 정보가 없음).
		ERR_CASE(ERR_VITALITY_03, kPacket.m_nOK, NetError::ERR_VITALITY_03); // 활력 충전 성공.
		ERR_CASE(ERR_VITALITY_05, kPacket.m_nOK, NetError::ERR_VITALITY_05); // 활력 충전 실패(이미 최대 활력 포인트를 갖고 있음).
		ERR_CASE(ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98); // 아직 처리중인 작업입니다.
	default:
		START_LOG(cerr, L"유저 활력 충전 요청 시 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	START_LOG(clog, L"유저 활력 충전 요청 처리, User : " << GetName())
		<< BUILD_LOG(NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket.m_nMaxVitalityPerDay)
		<< BUILD_LOG(kPacket.m_nMaxVitalityForFirstCharacter)
		<< BUILD_LOG(kPacket.m_nVitalityPerRechargePoint)
		<< BUILD_LOG(kPacket.m_nRechargePoint)
		<< BUILD_LOG(kPacket.m_nRechargePointCounter)
		<< BUILD_LOG(kPacket.m_mapRemainVitality.size())
		<< BUILD_LOG(kDBPacket.m_nRemainVitality)
		<< END_LOG;

	SEND_PACKET(EVENT_VITALITY_INFO_NOT);
}

_IMPL_ON_FUNC(DB_EVENT_VITALITY_INFO_REQ, bool)
{
	START_LOG(clog, L"접속중인 유저들의 활력 및 재충전 포인트 유저 데이터 요청: " << m_strName)
		<< BUILD_LOG(kPacket_) << END_LOG;

	KDB_EVENT_VITALITY_INFO_REQ kPacket;
	kPacket.m_bNeedForceInit = kPacket_; // true: 활력 초기화 시간에 요청, false: 그 외 상황에서 활력 정보 필요시
	kPacket.m_mapCharacterInfo = m_mapCharacterInfo;
	kPacket.m_bVitalityInfoLoaded = false;
	kPacket.m_kVitalityUpdateData.m_nRechargePoint = m_kVitalityUserData.m_nRechargePoint;
	kPacket.m_kVitalityUpdateData.m_nRechargePointCounter = m_kVitalityUserData.m_nRechargePointCounter;
	kPacket.m_kVitalityUpdateData.m_mapRemainVitality = m_kVitalityUserData.m_mapRemainVitality;

	if (true == kPacket_) {
		// 초기화 시간일 경우에만 유저의 활력 정보가 캐싱되어 있다
		kPacket.m_bVitalityInfoLoaded = true;
	}

	QUEUEING_EVENT_TO_DB(DB_EVENT_VITALITY_INFO_REQ, kPacket);
}
