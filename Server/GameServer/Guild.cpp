#include "Guild.h"
#include "GSDBLayer.h"
#include <KncUtilLua.h> // boost::bind
#include "Log4.h"
//FILE_NAME_FOR_LOG

unsigned int KGuild::ms_uiSendUserListGap(7 * 1000);
unsigned int KGuild::ms_uiRecvDBInfoGap(60 * 1000);

KGuild::KGuild()
{
	m_uiLastSendUserList = timeGetTime();
	m_uiLastRecvDBInfo = timeGetTime();
}

KGuild::~KGuild()
{
}

bool KGuild::AddNewUser(IN const KNGuildUserInfo& kUserInfo_)
{
	{
		KLocker lock(m_csGuildMemberList);

		if (m_mapNGuildMemberList.insert(std::make_pair(kUserInfo_.m_dwUserUID, kUserInfo_)).second == false) {
			START_LOG(cwarn, L"이미 등록된 길드원. GuildUID : " << kUserInfo_.m_dwGuildUID)
				<< BUILD_LOG(kUserInfo_.m_strLogin)
				<< BUILD_LOGc(kUserInfo_.m_cMemberLevel) << END_LOG;
			// 정보갱신만 해주자.
			m_mapNGuildMemberList[kUserInfo_.m_dwUserUID] = kUserInfo_;
			return false;
		}
	}

	if (kUserInfo_.m_cMemberLevel > KNGuildUserInfo::GL_JOINER) {
		KLocker lock(m_csGuildInfo);
		m_kNGuildInfo.m_nNumMembers = GetGuildMemeberSizeExJoiner();
	}

	// 길드원들에게 길드원 추가 알림.
	SendToAll(KUserEvent::EVENT_ADD_GUILD_USER_NOT, kUserInfo_);
	return true;
}

bool KGuild::Enter(KUserPtr spUser_)
{
	if (IsInGuild(spUser_->GetUID()) == true) {
		//이미 있는 유저임.
		START_LOG(cwarn, L"이미 길드에 속한 유저, GuildUID : " << m_kNGuildInfo.m_dwUID)
			<< BUILD_LOG(spUser_->GetUID())
			<< BUILD_LOG(spUser_->GetName()) << END_LOG;
		return true;
	}

	KLocker lock(m_csGuildMemberList);
	// 길드원 목록에서 정보 갱신
	std::map< DWORD, KNGuildUserInfo >::iterator mit = m_mapNGuildMemberList.find(spUser_->GetUID());
	if (mit == m_mapNGuildMemberList.end()) {
		START_LOG(cerr, L"길드원 목록에 해당 유저가 없음. GuildUID : " << m_kNGuildInfo.m_dwUID)
			<< BUILD_LOG(spUser_->GetUID())
			<< BUILD_LOG(spUser_->GetName()) << END_LOG;
		return false;
	}

	KNSimpleGuildUserInfo kInfo;
	spUser_->GetSimpleGuildUserInfo(kInfo);

	// 접속상태로 설정.
	mit->second.m_bIsConnect = true;
	mit->second.m_cServerUID = kInfo.m_cServerUID;
	CTime tDate(CTime::GetCurrentTime());
	mit->second.m_kLastLoginDate.SetDate(tDate.GetYear(), tDate.GetMonth(), tDate.GetDay(), 0);

	// UserPtr 저장
	m_mapUsers.insert(std::map<DWORD, KUserPtr>::value_type(spUser_->GetUID(), spUser_));

	START_LOG(clog, L"길드에 유저 접속. GuildUID : " << m_kNGuildInfo.m_dwUID)
		<< BUILD_LOG(mit->second.m_dwGuildUID)
		<< BUILD_LOG(mit->second.m_dwUserUID)
		<< BUILD_LOG(mit->second.m_strLogin)
		<< BUILD_LOG(mit->second.m_strNickName)
		<< BUILD_LOG(mit->second.m_strNickColor)
		<< BUILD_LOGc(mit->second.m_cMemberLevel)
		<< BUILD_LOGc(mit->second.m_ucCharacterGrade)
		<< BUILD_LOG(mit->second.m_dwContributePoint)
		<< BUILD_LOG(mit->second.m_nWinCount)
		<< BUILD_LOG(mit->second.m_nLoseCount)
		<< BUILD_LOG(mit->second.m_strMyComment)
		<< BUILD_LOG(mit->second.m_kJoinDate.m_sYear)
		<< BUILD_LOG((int)mit->second.m_kJoinDate.m_cMonth)
		<< BUILD_LOG((int)mit->second.m_kJoinDate.m_cDay)
		<< BUILD_LOG(mit->second.m_bIsConnect)
		<< BUILD_LOGc(mit->second.m_cServerUID)
		<< BUILD_LOG(mit->second.m_strLocation)
		<< BUILD_LOG(mit->second.m_kLastLoginDate.m_sYear)
		<< BUILD_LOG((int)mit->second.m_kLastLoginDate.m_cMonth)
		<< BUILD_LOG((int)mit->second.m_kLastLoginDate.m_cDay)
		<< END_LOG;

	return true;
}

void KGuild::LogOffUser(IN const DWORD dwUserUID_)
{
	KLocker lock(m_csGuildMemberList);
	// 다른 게임서버에서 길드원 로그오프 알림.
	std::map< DWORD, KNGuildUserInfo >::iterator mit = m_mapNGuildMemberList.find(dwUserUID_);
	if (mit == m_mapNGuildMemberList.end()) {
		START_LOG(cerr, L"로그오프 처리대상 유저정보 없음. UserUID : " << dwUserUID_) << END_LOG;
		return;
	}

	// 로그오프 처리
	CTime tDate(CTime::GetCurrentTime());
	mit->second.LogOff(tDate.GetYear(), tDate.GetMonth(), tDate.GetDay(), 0);
	START_LOG(clog, L"길드원 로그오프함. Login : " << mit->second.m_strLogin) << END_LOG;
}

void KGuild::Leave(IN KUserPtr spUser_)
{
	// 길드원 목록에서 정보 갱신
	LogOffUser(spUser_->GetUID());

	KLocker lock(m_csGuildMemberList);
	// 유저포인터 목록에서 삭제
	std::map<DWORD, KUserPtr>::iterator mit = m_mapUsers.find(spUser_->GetUID());
	if (mit != m_mapUsers.end()) {
		m_mapUsers.erase(mit);
	}
}

void KGuild::DeleteUser(IN const DWORD dwUserUID_, IN const char cOutType_)
{
	KLocker lock(m_csGuildMemberList);
	// 길드에서 탈퇴했거나 강퇴당하는 경우 불린다.
	std::map< DWORD, KNGuildUserInfo >::iterator mit;
	KNGuildUserInfo kDeleteUserInfo;
	kDeleteUserInfo.Init();

	// 길드원 목록에서 제거
	mit = m_mapNGuildMemberList.find(dwUserUID_);
	if (mit != m_mapNGuildMemberList.end()) {
		kDeleteUserInfo = mit->second;
		m_mapNGuildMemberList.erase(mit);
		START_LOG(clog, L"길드원 목록에서 해당 유저 제거함. dwUserUID_ : " << dwUserUID_) << END_LOG;
	}

	// 지우기전에 대상 유저에게 탈퇴당했다는 패킷 보내주자.
	KEVENT_GUILD_DRUMOUT_USER_NOT kPacket;
	kPacket.m_cOutType = cOutType_;
	kPacket.m_kOutUserInfo = kDeleteUserInfo;
	QueueingToUser(dwUserUID_, KUserEvent::EVENT_GUILD_DRUMOUT_USER_NOT, kPacket);
	SendPacket(dwUserUID_, KUserEvent::EVENT_GUILD_DRUMOUT_USER_NOT, kPacket);

	// 유저포인터 목록에서 삭제
	{
		std::map<DWORD, KUserPtr>::iterator mitUser;

		KLocker lock(m_csGuildMemberList);
		mitUser = m_mapUsers.find(dwUserUID_);
		if (mitUser != m_mapUsers.end()) {
			// 유저포인터 벡터에서 삭제
			m_mapUsers.erase(mitUser);
			START_LOG(clog, L"길드원 포인터 정보 제거함. dwUserUID_ : " << dwUserUID_) << END_LOG;
		}
	}

	// 길드원들에게 길드원 삭제 알림.
	SendToAll(KUserEvent::EVENT_DEL_GUILD_USER_NOT, kDeleteUserInfo);
}

void KGuild::SetInfo(IN const KNGuildInfo& kInfo_)
{
	m_kNGuildInfo = kInfo_;

	START_LOG(clog, L"길드정보 최초 설정됨. GuildUID : " << kInfo_.m_dwUID)
		<< BUILD_LOG(kInfo_.m_strName)
		<< BUILD_LOGc(kInfo_.m_ucMarkStatus)
		<< BUILD_LOG(kInfo_.m_nMarkRevision)
		<< BUILD_LOG(kInfo_.m_strFileName)
		<< BUILD_LOG(kInfo_.m_strComment)
		<< BUILD_LOGc(kInfo_.m_ucGrade)
		<< BUILD_LOG(kInfo_.m_strMasterNick)
		<< BUILD_LOG(kInfo_.m_strURL)
		<< BUILD_LOG(kInfo_.m_dwExp)
		<< BUILD_LOG(kInfo_.m_dwPoint)
		<< BUILD_LOG(kInfo_.m_nNumMembers)
		<< BUILD_LOG(kInfo_.m_nTotalRank)
		<< BUILD_LOG(kInfo_.m_nMonthlyRank)
		<< BUILD_LOG(kInfo_.m_nWeeklyRank)
		<< BUILD_LOGc(kInfo_.m_ucJoinSetting) << END_LOG;
}

void KGuild::SetGuildNotice(IN std::vector<KNGuildNotice>& vecNotice_)
{
	// 센터서버에서 전달받은 전체공지 설정
	KLocker lock(m_csGuildInfo);

	m_vecNGuildNotice.swap(vecNotice_);
	START_LOG(clog, L"길드공지 추가. GuildUID : " << m_kNGuildInfo.m_dwUID << L", size : " << m_vecNGuildNotice.size()) << END_LOG;

	// 길드정보에 소개말 설정
	std::vector<KNGuildNotice>::iterator vit;
	vit = std::find_if(m_vecNGuildNotice.begin(), m_vecNGuildNotice.end(),
		boost::bind(&KNGuildNotice::m_nMsgID, _1) == KNGuildNotice::GN_COMMENT);

	if (vit == m_vecNGuildNotice.end()) {
		//m_kNGuildInfo.m_strComment.clear(); 초기화하지 말자.
		START_LOG(cwarn, L"공지리스트에서 길드소개가 없음. GuildUID : " << m_kNGuildInfo.m_dwUID) << END_LOG;
		return;
	}

	m_kNGuildInfo.m_strComment = vit->m_strMsg;
	START_LOG(clog, L"길드소개 설정. GuildUID : " << m_kNGuildInfo.m_dwUID << L",Comment : " << m_kNGuildInfo.m_strComment) << END_LOG;
}

void KGuild::ModifyGuildNotice(IN std::vector<KNGuildNotice>& vecNotice_)
{
	KLocker lock(m_csGuildInfo);

	// 로컬서버에서 공지사항 수정시 불린다.
	std::vector<KNGuildNotice>::const_iterator cvit;
	std::vector<KNGuildNotice>::iterator vit;

	for (cvit = vecNotice_.begin(); cvit != vecNotice_.end(); ++cvit) {
		vit = std::find_if(m_vecNGuildNotice.begin(), m_vecNGuildNotice.end(),
			boost::bind(&KNGuildNotice::m_nMsgID, _1) == cvit->m_nMsgID);

		// 공지수정시 없으면 데이터를 집어넣어준다.
		if (vit == m_vecNGuildNotice.end()) {
			m_vecNGuildNotice.push_back(*cvit);

			START_LOG(clog, L"길드 공지 추가. GuildUID : " << m_kNGuildInfo.m_dwUID)
				<< BUILD_LOG(cvit->m_nMsgID)
				<< BUILD_LOG(cvit->m_strMsg) << END_LOG;
			continue;
		}

		// 있는 데이터이면 수정.
		vit->m_strMsg = cvit->m_strMsg;

		START_LOG(clog, L"길드 공지 수정. GuildUID : " << m_kNGuildInfo.m_dwUID)
			<< BUILD_LOG(cvit->m_nMsgID)
			<< BUILD_LOG(cvit->m_strMsg) << END_LOG;

		// 길드 소개이면,
		if (cvit->m_nMsgID == KNGuildNotice::GN_COMMENT) {
			m_kNGuildInfo.m_strComment = cvit->m_strMsg;
			START_LOG(clog, L"길드소개 수정. GuildUID : " << m_kNGuildInfo.m_dwUID)
				<< BUILD_LOG(cvit->m_strMsg) << END_LOG;
		}
	}

	START_LOG(clog, L"수정된 길드 공지 적용됨. GuildUID : " << m_kNGuildInfo.m_dwUID << L",size : " << vecNotice_.size()) << END_LOG;
}


bool KGuild::ModifyMemberComment(IN const DWORD dwUserUID_, IN const std::wstring& strMyComment_)
{
	KLocker lock(m_csGuildMemberList);

	std::map<DWORD, KNGuildUserInfo>::iterator mit = m_mapNGuildMemberList.find(dwUserUID_);
	if (mit == m_mapNGuildMemberList.end()) {
		START_LOG(cerr, L"길드에 유저 정보가 없음. GuildUID : " << m_kNGuildInfo.m_dwUID << L",UserUID : " << dwUserUID_) << END_LOG;
		return false;
	}

	mit->second.m_strMyComment = strMyComment_;
	START_LOG(clog, L"유저 소갯글 변경. GuildUID : " << m_kNGuildInfo.m_dwUID)
		<< BUILD_LOG(dwUserUID_)
		<< BUILD_LOG(strMyComment_) << END_LOG;
	return true;
}


bool KGuild::IsInGuild(IN DWORD dwUserUID_)
{
	KLocker lock(m_csGuildMemberList);
	return m_mapUsers.find(dwUserUID_) != m_mapUsers.end();
}

bool KGuild::GetNGuildUserInfo(IN const DWORD dwUserUID_, OUT KNGuildUserInfo& kInfo_)
{
	{
		KLocker lock(m_csGuildMemberList);
		std::map<DWORD, KNGuildUserInfo>::iterator mit = m_mapNGuildMemberList.find(dwUserUID_);
		if (mit == m_mapNGuildMemberList.end()) {
			START_LOG(cerr, L"길드 길드원목록에 없는 유저. UserUID : " << dwUserUID_) << END_LOG;
			return false;
		}

		if (mit->second.m_dwGuildUID != m_kNGuildInfo.m_dwUID) {
			START_LOG(cerr, L"길드원 정보의 길드UID와 길드정보의 길드UID가 다름. UserUID : " << dwUserUID_)
				<< BUILD_LOG(m_kNGuildInfo.m_dwUID)
				<< BUILD_LOG(mit->second.m_dwGuildUID) << END_LOG;
			// 길드원 정보의 길드UID를 수정해주자.
			mit->second.m_dwGuildUID = m_kNGuildInfo.m_dwUID;
		}

		kInfo_ = mit->second;
	}

	START_LOG(clog, L"길드유저 정보 받아감. UserUID : " << dwUserUID_)
		<< BUILD_LOG(kInfo_.m_dwUserUID)
		<< BUILD_LOG(kInfo_.m_strLogin)
		<< BUILD_LOG(kInfo_.m_strNickName)
		<< BUILD_LOG(kInfo_.m_strNickColor)
		<< BUILD_LOG(kInfo_.m_dwGuildUID)
		<< BUILD_LOGc(kInfo_.m_cMemberLevel)
		<< BUILD_LOGc(kInfo_.m_ucCharacterGrade)
		<< BUILD_LOG(kInfo_.m_dwContributePoint)
		<< BUILD_LOG(kInfo_.m_nWinCount)
		<< BUILD_LOG(kInfo_.m_nLoseCount)
		<< BUILD_LOG(kInfo_.m_strMyComment)
		<< BUILD_LOG(kInfo_.m_bIsConnect)
		<< BUILD_LOGc(kInfo_.m_cServerUID)
		<< BUILD_LOG(kInfo_.m_strLocation) << END_LOG;
	return true;
}

bool KGuild::SetGuildMarkStatus(IN const KNGuildInfo::MARK_STATUS eMarkStatus_)
{
	KLocker lock(m_csGuildInfo);

	_JIF(eMarkStatus_ >= KNGuildInfo::MS_NORMAL && eMarkStatus_ <= KNGuildInfo::MS_REJECT_CHANGE, return false);
	m_kNGuildInfo.m_ucMarkStatus = eMarkStatus_;
	return true;
}


void KGuild::ChangeMemberLevel(IN const char cMemberLevel_, IN const std::set<DWORD>& setUID_)
{
	if (setUID_.empty()) return;
	// 길드원 맴버레벨 수정시 불린다.

	std::map<DWORD, KNGuildUserInfo>::iterator mit;
	std::set<DWORD>::const_iterator sit;

	KEVENT_CHANGE_GUILD_MEMBER_LEVEL_NOT kPacket;
	kPacket.m_dwGuildUID = m_kNGuildInfo.m_dwUID;

	for (sit = setUID_.begin(); sit != setUID_.end(); ++sit) {
		{
			KLocker lock(m_csGuildMemberList);
			mit = m_mapNGuildMemberList.find(*sit);
			if (mit == m_mapNGuildMemberList.end()) {
				START_LOG(cerr, L"멤버레벨 변경대상 유저가 없다. GuildUID : " << m_kNGuildInfo.m_dwUID << L",UserUID : " << *sit) << END_LOG;
				continue;
			}

			// 길드원에게 알림용 정보
			KGuildMemberLevelInfo kInfo;
			kInfo.m_strNickName = mit->second.m_strNickName;
			kInfo.m_prMemberLevel.first = mit->second.m_cMemberLevel; // 변경전 맴버레벨
			kInfo.m_prMemberLevel.second = cMemberLevel_; // 변경후 맴버레벨
			// 패킷 설정
			kPacket.m_mapGuildMemberLevelInfo[mit->second.m_dwUserUID] = kInfo;

			// 맴버레벨 변경
			mit->second.m_cMemberLevel = cMemberLevel_;
		}

		if (cMemberLevel_ == KNGuildUserInfo::GL_MASTER) {
			KLocker lock(m_csGuildInfo);
			m_kNGuildInfo.m_strMasterNick = mit->second.m_strNickName;
			m_kNGuildInfo.m_strMasterNickColor = mit->second.m_strNickColor;
			START_LOG(clog, L"길드마스터 닉네임 변경됨. NickName : " << m_kNGuildInfo.m_strMasterNick) << END_LOG;
		}

		START_LOG(clog, L"멤버레벨 변경됨.")
			<< BUILD_LOG(m_kNGuildInfo.m_dwUID)
			<< BUILD_LOG(*sit)
			<< BUILD_LOGc(cMemberLevel_) << END_LOG;
	}

	START_LOG(clog, L"멤버레벨 변경. GuildUID : " << m_kNGuildInfo.m_dwUID)
		<< BUILD_LOGc(cMemberLevel_)
		<< BUILD_LOGc(setUID_.size()) << END_LOG;

	// 로컬 서버내의 길드원들에게 변경사항을 알려줘야한다.
	QueueingToAll(KUserEvent::EVENT_CHANGE_GUILD_MEMBER_LEVEL_NOT, kPacket);
	SendToAll(KUserEvent::EVENT_CHANGE_GUILD_MEMBER_LEVEL_NOT, kPacket);
}


bool KGuild::UpdateGuildMarkInfo(IN const KNGuildMarkInfo& kGuildMarkInfo_)
{
	KLocker lock(m_csGuildInfo);

	bool bRet = false;
	if (m_kNGuildInfo.m_nMarkRevision != kGuildMarkInfo_.m_nMarkRevision ||
		m_kNGuildInfo.m_ucMarkStatus != kGuildMarkInfo_.m_ucMarkStatus) {
		bRet = true;
	}

	m_kNGuildInfo.m_ucMarkStatus = kGuildMarkInfo_.m_ucMarkStatus;
	m_kNGuildInfo.m_nMarkRevision = kGuildMarkInfo_.m_nMarkRevision;
	m_kNGuildInfo.m_strFileName = kGuildMarkInfo_.m_strFileName;
	return bRet;
}


void KGuild::ModifyGuildName(IN std::wstring& strGuildName_)
{
	KLocker lock(m_csGuildInfo);

	START_LOG(clog, L"길드 이름 변경 전. UID : " << m_kNGuildInfo.m_dwUID)
		<< BUILD_LOG(m_kNGuildInfo.m_strName) << END_LOG;

	m_kNGuildInfo.m_strName = strGuildName_;

	START_LOG(clog, L"길드 이름 변경 후. UID : " << m_kNGuildInfo.m_dwUID)
		<< BUILD_LOG(m_kNGuildInfo.m_strName) << END_LOG;

	// 접속중인 길드원들에게 알려주자.
	KECN_UPDATE_GUILD_NAME_NOT kPacket;
	kPacket.m_dwGuildUID = m_kNGuildInfo.m_dwUID;
	kPacket.m_strGuildName = strGuildName_;
	SendToAll(KUserEvent::EVENT_EDIT_GUILD_NAME_NOT, kPacket);
}


void KGuild::ChangeURL(IN std::wstring& strURL_)
{
	KLocker lock(m_csGuildInfo);

	START_LOG(clog, L"길드 URL 변경 전. UID : " << m_kNGuildInfo.m_dwUID)
		<< BUILD_LOG(m_kNGuildInfo.m_strURL) << END_LOG;

	m_kNGuildInfo.m_strURL = strURL_;

	START_LOG(clog, L"길드 URL 변경 후. UID : " << m_kNGuildInfo.m_dwUID)
		<< BUILD_LOG(m_kNGuildInfo.m_strURL) << END_LOG;

	KNGuildURL kPacket;
	kPacket.m_nOK = 0;
	kPacket.m_dwUID = m_kNGuildInfo.m_dwUID;
	kPacket.m_strURL = strURL_;
	SendToAll(KUserEvent::EVENT_UPDATE_GUILD_URL_NOT, kPacket);
}


void KGuild::ChangeJoinPolicy(KNGuildJoinPolicy& kPacket_)
{
	KLocker lock(m_csGuildInfo);

	START_LOG(clog, L"길드 가입승인 방법 변경 전. UID : " << m_kNGuildInfo.m_dwUID)
		<< BUILD_LOGc(m_kNGuildInfo.m_ucJoinSetting) << END_LOG;

	m_kNGuildInfo.m_ucJoinSetting = kPacket_.m_ucMethod;

	START_LOG(clog, L"길드 가입승인 방법 변경 후. UID : " << m_kNGuildInfo.m_dwUID)
		<< BUILD_LOGc(m_kNGuildInfo.m_ucJoinSetting) << END_LOG;
}


void KGuild::ChangeGrade(IN const UCHAR ucGrade_)
{
	KLocker lock(m_csGuildInfo);

	PAIR_UCHAR kPacket;
	kPacket.first = m_kNGuildInfo.m_ucGrade;
	kPacket.second = ucGrade_;

	m_kNGuildInfo.m_ucGrade = ucGrade_;
	START_LOG(clog, L"길드등급 변경. Grade : " << (int)ucGrade_) << END_LOG;

	QueueingToAll(KUserEvent::EVENT_UPDATE_GUILD_STATE_NOT, kPacket);
}


size_t KGuild::GetOnlineUserCount()
{
	std::map< DWORD, KNGuildUserInfo > mapList;
	std::map< DWORD, KNGuildUserInfo >::iterator mit;
	size_t stOnlineUserCount = 0;

	GetNGuildMemberList(mapList);

	for (mit = mapList.begin(); mit != mapList.end(); ++mit) {
		if (mit->second.m_bIsConnect == true) {
			++stOnlineUserCount;
		}
	}
	return stOnlineUserCount;
}


void KGuild::MemberListClearCheck()
{
	size_t OnlineUserCount = GetOnlineUserCount();
	if (OnlineUserCount > 0) {
		START_LOG(clog, L"접속중인 유저가 있다. GuildUID : " << m_kNGuildInfo.m_dwUID)
			<< BUILD_LOG(GetGuildMemberListSize())
			<< BUILD_LOG(OnlineUserCount) << END_LOG;
		return;
	}

	{
		KLocker lock(m_csGuildMemberList);
		m_mapNGuildMemberList.clear();
		m_mapUsers.clear();
	}

	START_LOG(clog, L"접속중인 유저가 없는 길드의 맴버리스트 초기화. GuildUID : " << m_kNGuildInfo.m_dwUID) << END_LOG;
}


bool KGuild::ChangeMemberNick(IN const DWORD dwUserUID_, IN const std::wstring& strNickName_, IN const std::wstring& strNickColor_)
{
	std::map<DWORD, KNGuildUserInfo>::iterator mit;

	KLocker lock(m_csGuildMemberList);
	const std::wstring nickColorEmpty = L"FFFFFF";

	KGuildUserName kPacket;

	if (strNickColor_ == L"")
		kPacket.m_strNickColor = nickColorEmpty;
	else
		kPacket.m_strNickColor = strNickColor_;

	kPacket.m_dwGuildUID = m_kNGuildInfo.m_dwUID;
	kPacket.m_dwUserUID = dwUserUID_;
	kPacket.m_strNickName = strNickName_;

	mit = m_mapNGuildMemberList.find(dwUserUID_);
	if (mit == m_mapNGuildMemberList.end()) {
		START_LOG(cerr, L"정보 변경할 대상 길드원이 리스트에 없음. dwUserUID_ : " << dwUserUID_) << END_LOG;
		return false;
	}

	// 닉네임 변경.
	mit->second.m_strNickName = kPacket.m_strNickName;
	mit->second.m_strNickColor = kPacket.m_strNickColor;
	START_LOG(clog, L"길드원 닉네임 변경됨. NickName : " << kPacket.m_strNickName) << END_LOG;

	// 길마면 추가처리
	if (mit->second.m_cMemberLevel == KNGuildUserInfo::GL_MASTER) {
		KLocker lock(m_csGuildInfo);
		m_kNGuildInfo.m_strMasterNick = kPacket.m_strNickName;
		m_kNGuildInfo.m_strMasterNickColor = kPacket.m_strNickColor;
		START_LOG(clog, L"길드마스터 닉네임 변경됨. NickName : " << kPacket.m_strNickName) << END_LOG;
	}

	// 길드원들에게 알림.
	SendToAll(KUserEvent::EVENT_UPDATE_GUILD_MEMBER_NICK_NOT, kPacket);
	return true;
}


size_t KGuild::GetGuildMemeberSizeExJoiner()
{
	// 정식 길드원수 리턴 (가입대기자 제외)
	std::map<DWORD, KNGuildUserInfo>::const_iterator cmit;
	size_t uiCount = 0;

	KLocker lock(m_csGuildMemberList);

	for (cmit = m_mapNGuildMemberList.begin(); cmit != m_mapNGuildMemberList.end(); ++cmit) {
		if (cmit->second.m_cMemberLevel > KNGuildUserInfo::GL_JOINER) {
			++uiCount;
		}
	}

	return uiCount;
}


void KGuild::UpdateBattlePoint(IN const int nBattlePoint_)
{
	KLocker lock(m_csGuildInfo);

	m_kNGuildInfo.m_GuildBattlePoint += nBattlePoint_;

	m_kNGuildInfo.m_GuildBattlePoint = std::max<int>(m_kNGuildInfo.m_GuildBattlePoint, 0);
}


void KGuild::UpdateMemberGuildPoint(const DWORD dwUserUID_, IN const DWORD dwPoint_)
{
	std::map<DWORD, KNGuildUserInfo>::iterator mit;

	KLocker lock(m_csGuildMemberList);

	mit = m_mapNGuildMemberList.find(dwUserUID_);

	if (mit != m_mapNGuildMemberList.end()) {
		mit->second.m_dwContributePoint += dwPoint_;
	}
}


void KGuild::SetMemberGuildPoint(const DWORD dwUserUID_, IN const DWORD dwPoint_)
{
	std::map<DWORD, KNGuildUserInfo>::iterator mit;

	KLocker lock(m_csGuildMemberList);

	mit = m_mapNGuildMemberList.find(dwUserUID_);

	if (mit != m_mapNGuildMemberList.end()) {
		mit->second.m_dwContributePoint = dwPoint_;
	}
}


DWORD KGuild::GetMemberGuildPoint(const DWORD dwUserUID_)
{
	std::map<DWORD, KNGuildUserInfo>::iterator mit;

	KLocker lock(m_csGuildMemberList);

	mit = m_mapNGuildMemberList.find(dwUserUID_);

	if (mit != m_mapNGuildMemberList.end()) {
		return mit->second.m_dwContributePoint;
	}
	return 0;
}