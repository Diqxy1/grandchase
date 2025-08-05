#pragma once

#include "User.h"
#include "SobManager.h"

SmartPointer(KGuild);

class KGuild

{
public:
	// constructor, destructor
	KGuild();
	virtual ~KGuild();

	typedef std::map<DWORD, KNGuildUserInfo> GuildMemberList;


	bool AddNewUser(IN const KNGuildUserInfo& kUserInfo_); // 길드 새 가입자 정보 등록
	void DeleteUser(IN const DWORD dwUserUID, IN const char cOutType); // 탈퇴,강퇴로 인한 유저정보 삭제

	bool Enter(KUserPtr spUser);
	void Leave(KUserPtr spUser); // 나간 유저의 포인터 삭제
	void LogOffUser(IN const DWORD dwUserUID);


	KNGuildInfo& GetGuildInfo() { return m_kNGuildInfo; }
	DWORD GetGuildUID() { return m_kNGuildInfo.m_dwUID; }
	//void SetInfo( IN const KGuildInfo& kGuildInfo );
	void SetInfo(IN const KNGuildInfo& kInfo_);
	void SetGuildNotice(IN std::vector<KNGuildNotice>& vecNotice);
	void ModifyGuildNotice(IN std::vector<KNGuildNotice>& vecNotice);
	bool ModifyMemberComment(IN const DWORD dwUserUID, IN const std::wstring& strMyComment);
	void ModifyGuildName(IN std::wstring& strGuildName);

	void ChangeURL(IN std::wstring& strURL_);

	bool IsInGuild(IN DWORD dwUserUID);
	bool IsEmpty() { return m_mapUsers.empty(); }


	template< class T > void SendToAll(const unsigned short usEventID, const T& data);
	template< class T > void SendToAllexJoiner(const unsigned short usEventID, const T& data);
	template< class T > void SendPacket(const DWORD dwUserUID, const unsigned short usEventID, const T& data);
	template< class T > void QueueingToAll(const unsigned short usEventID, const T& data);
	template< class T > void QueueingToUser(const DWORD dwUserUID, const unsigned short usEventID, const T& data);

	static void SetBroadCastingTime(unsigned int uiSendUserListTick, unsigned int uiRecvDBInfoTick)
	{
		ms_uiSendUserListGap = uiSendUserListTick;
		ms_uiRecvDBInfoGap = uiRecvDBInfoTick;
	}

	KNGuildInfo& GetNGuildInfo() { return m_kNGuildInfo; }
	void GetNGuildNotice(OUT std::vector<KNGuildNotice>& vecNotice_) { vecNotice_ = m_vecNGuildNotice; }
	void SetNGuildMemberList(IN std::map<DWORD, KNGuildUserInfo>& mapList_) { KLocker lock(m_csGuildMemberList); m_mapNGuildMemberList = mapList_; }
	void GetNGuildMemberList(OUT std::map<DWORD, KNGuildUserInfo>& mapList_) { KLocker lock(m_csGuildMemberList); mapList_ = m_mapNGuildMemberList; }
	bool GetNGuildUserInfo(IN const DWORD dwUserUID, OUT KNGuildUserInfo& kInfo);
	bool SetGuildMarkStatus(IN const KNGuildInfo::MARK_STATUS eMarkStatus);
	UCHAR GetGuildMarkStatus() { return m_kNGuildInfo.m_ucMarkStatus; }

	void ChangeMemberLevel(IN const char cMemberLevel, IN const std::set<DWORD>& setUID);
	bool UpdateGuildMarkInfo(IN const KNGuildMarkInfo& kGuildMarkInfo);
	void ChangeJoinPolicy(KNGuildJoinPolicy& kPacket_);
	void ChangeGrade(IN const UCHAR ucGrade);

	size_t GetGuildMemberListSize() { KLocker lock(m_csGuildMemberList); return m_mapNGuildMemberList.size(); }
	size_t GetOnlineUserCount();
	size_t GetGuildMemeberSizeExJoiner();
	void MemberListClearCheck();
	bool IsMemberListEmpty() { KLocker lock(m_csGuildMemberList); return m_mapNGuildMemberList.empty(); }
	bool ChangeMemberNick(IN const DWORD dwUserUID, IN const std::wstring& strNickName, IN const std::wstring& strNickColor);
	DWORD GetBattlePoint() { KLocker lock(m_csGuildInfo); return m_kNGuildInfo.m_GuildBattlePoint; }
	void SetBattlePoint(IN DWORD dwPoint_) { KLocker lock(m_csGuildInfo); m_kNGuildInfo.m_GuildBattlePoint = dwPoint_; }

	void UpdateBattlePoint(IN const int nBattlePoint_);
	void AddGuildPoint(IN const int nGuildPoint_) { KLocker lock(m_csGuildInfo); m_kNGuildInfo.m_dwPoint += nGuildPoint_; }
	void SetGuildPoint(IN const DWORD guildPoint_) { KLocker lock(m_csGuildInfo); m_kNGuildInfo.m_dwPoint = guildPoint_; }
	DWORD GetGuildPoint() { KLocker lock(m_csGuildInfo); return m_kNGuildInfo.m_dwPoint; }
	UCHAR GetGuildGrade() { KLocker lock(m_csGuildInfo); return m_kNGuildInfo.m_ucGrade; }

	void UpdateMemberGuildPoint(const DWORD dwUserUID_, IN const DWORD dwPoint_);
	void SetMemberGuildPoint(const DWORD dwUserUID_, IN const DWORD dwPoint_);
	DWORD GetMemberGuildPoint(const DWORD dwUserUID_);

protected:
	void Send_ChatNot(const std::wstring& strName, const std::wstring& strMsg);
	void Send_NotifyMsgNot(const std::wstring& strName, const std::wstring& strMsg);

protected:
	unsigned int            m_uiLastSendUserList;   // 길드 존에 있는 유저들에게 접속중인 총 길드원 정보를 전송.
	unsigned int            m_uiLastRecvDBInfo;     // 일정 시간마다 DB의 정보를 가져와서 Update한다.

	static unsigned int     ms_uiSendUserListGap;   // 060428. kkurrung. 시간 종속적인 이벤트에 사용.
	static unsigned int     ms_uiRecvDBInfoGap;

	// tslayer.
	mutable KncCriticalSection  m_csGuildInfo;
	KNGuildInfo                 m_kNGuildInfo;          // 길드정보
	std::vector<KNGuildNotice>  m_vecNGuildNotice;      // 길드공지사항

	mutable KncCriticalSection      m_csGuildMemberList;
	std::map<DWORD, KNGuildUserInfo> m_mapNGuildMemberList;  // 길드원리스트

	mutable KncCriticalSection  m_csMemberList;
	std::map<DWORD, KUserPtr>   m_mapUsers; // <UserUID,KUserPtr>
};


template< class T >
void KGuild::SendToAll(const unsigned short usEventID_, const T& data_)
{
	KEventPtr spEvent(new KUserEvent);
	spEvent->m_usEventID = usEventID_;

	// make event object
	KSerializer         ks;

	// serialize - only data
	ks.BeginWriting(&spEvent->m_kbuff);
	ks.Put(data_);
	ks.EndWriting();
	spEvent->m_kbuff.Compress();

	// serialize - full event object
	KSerBuffer          kbuff;
	ks.BeginWriting(&kbuff);
	ks.Put(*spEvent);
	ks.EndWriting();

	KLocker lock(m_csGuildMemberList);
	std::map<DWORD, KUserPtr>::iterator mit;
	for (mit = m_mapUsers.begin(); mit != m_mapUsers.end(); ++mit)
	{
		mit->second->SendPacket(kbuff, spEvent->GetEventIDString());
	}

	START_LOG(clog, L"GuildMember size : " << m_mapUsers.size() << L",Event : " << spEvent->GetEventIDString()) << END_LOG;
}


template< class T >
void KGuild::SendToAllexJoiner(const unsigned short usEventID_, const T& data_)
{
	KEventPtr spEvent(new KUserEvent);
	spEvent->m_usEventID = usEventID_;

	// make event object
	KSerializer         ks;

	// serialize - only data
	ks.BeginWriting(&spEvent->m_kbuff);
	ks.Put(data_);
	ks.EndWriting();
	spEvent->m_kbuff.Compress();

	// serialize - full event object
	KSerBuffer          kbuff;
	ks.BeginWriting(&kbuff);
	ks.Put(*spEvent);
	ks.EndWriting();

	KLocker lock(m_csGuildMemberList);
	std::map<DWORD, KUserPtr>::iterator mit;
	for (mit = m_mapUsers.begin(); mit != m_mapUsers.end(); ++mit)
	{
		if (mit->second->m_kGuildUserInfo.m_cMemberLevel <= KNGuildUserInfo::GL_JOINER) {
			continue;
		}

		mit->second->SendPacket(kbuff, spEvent->GetEventIDString());
	}

	START_LOG(clog, L"GuildMember size : " << m_mapUsers.size() << L",Event : " << spEvent->GetEventIDString()) << END_LOG;
}


template< class T >
void KGuild::SendPacket(const DWORD dwUserUID_, const unsigned short usEventID_, const T& data_)
{
	KEventPtr spEvent(new KUserEvent);
	spEvent->m_usEventID = usEventID_;

	// make event object
	KSerializer         ks;

	// serialize - only data
	ks.BeginWriting(&spEvent->m_kbuff);
	ks.Put(data_);
	ks.EndWriting();
	spEvent->m_kbuff.Compress();

	// serialize - full event object
	KSerBuffer          kbuff;
	ks.BeginWriting(&kbuff);
	ks.Put(*spEvent);
	ks.EndWriting();


	KLocker lock(m_csGuildMemberList);
	std::map<DWORD, KUserPtr>::iterator mit;
	mit = m_mapUsers.find(dwUserUID_);
	if (mit == m_mapUsers.end()) {
		START_LOG(cwarn, L"SendPacket 대상유저가 유저포인트 컨테이너에 없음. UserUID : " << dwUserUID_) << END_LOG;
		return;
	}

	mit->second->SendPacket(kbuff, spEvent->GetEventIDString());
}


template< class T >
void KGuild::QueueingToAll(const unsigned short usEventID_, const T& data_)
{
	KLocker lock(m_csGuildMemberList);

	std::map<DWORD, KUserPtr>::iterator mit;
	for (mit = m_mapUsers.begin(); mit != m_mapUsers.end(); ++mit)
	{
		mit->second->QueueingEvent(usEventID_, data_);
	}
}


template< class T >
void KGuild::QueueingToUser(const DWORD dwUserUID_, const unsigned short usEventID_, const T& data_)
{
	KLocker lock(m_csGuildMemberList);

	std::map<DWORD, KUserPtr>::iterator mit;
	mit = m_mapUsers.find(dwUserUID_);
	if (mit == m_mapUsers.end()) {
		START_LOG(cwarn, L"Queueing 대상유저가 유저포인트 컨테이너에 없음. UserUID : " << dwUserUID_) << END_LOG;
		return;
	}

	mit->second->QueueingEvent(usEventID_, data_);
}