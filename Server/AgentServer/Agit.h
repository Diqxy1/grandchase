#pragma once
#include <RTTI.h>
#include "KncSmartPtr.h"
#include "Performer.h"
#include "AgentEvent.h"
#include "AgentPacket.h"
#include "KncTimer.h"

SmartPointer(KAgit);
SmartPointer(KAgent);
class KAgit : public KPerformer
{
	NiDeclareRTTI;
	DeclToStringW;

public:
	KAgit(void);
	virtual ~KAgit(void);

	virtual void Tick();

	bool IsPageRegister() { KLocker lock(m_csPage); return m_bPageRegister; }
	void SetPageRegister(IN const bool& bVal) { KLocker lock(m_csPage); m_bPageRegister = bVal; }
	void GetPageInfo(OUT KAgitPageInfo& kPageInfo);

	void SetRoomID(IN const USHORT& usRoomID) { KLocker lock(m_csPage); m_usRoomID = usRoomID; }
	USHORT GetRoomID() { KLocker lock(m_csPage); return m_usRoomID; }

	void SetInfo(IN const KAgitInfo& kInfo);
	void GetInfo(OUT KAgitInfo& kInfo);
	void SetName(IN const std::wstring& strName) { KLocker lock(m_csInfo); m_kInfo.m_strName = strName; }
	void SetOwnerNickname(IN const std::wstring& strNick) { KLocker lock(m_csInfo); m_strOwnerNickname = strNick; }
	void GetOwnerNickname(OUT std::wstring& strNick) { KLocker lock(m_csInfo); strNick = m_strOwnerNickname; }

	void IncVisiter();
	void GetActiveObjectPos(IN const GCITEMID& ItemID, OUT std::vector<KAgitObjectPos>& vecInfo);
	void SetObjectPos(IN const std::vector<KAgitObjectPos>& vecInfo);

	bool AddFavorites(IN const KAgitPageInfo& kInfo);
	void SetFavorites(IN const std::map<DWORD, KAgitPageInfo>& mapList) { KLocker lock(m_csFavorites); m_mapFavorites = mapList; }
	void GetFavorites(OUT std::map<DWORD, KAgitPageInfo>& mapList) { KLocker lock(m_csFavorites); mapList = m_mapFavorites; }
	void RemoveFavorites(IN const DWORD& dwAgitUID);

	void SetTRServerIPPort(IN const DWORD& dwIP, IN const USHORT& usPort);
	void GetTRServerIPPort(OUT std::pair<DWORD, USHORT>& prIPPort);
	void SetURServerIPPort(IN const DWORD& dwIP, IN const USHORT& usPort);
	void GetURServerIPPort(OUT std::pair<DWORD, USHORT>& prIPPort);

	size_t GetUserNum();
	bool IsFull();
	bool IsExistUser(IN const DWORD& dwUserUID);
	void SetUserInfo(IN const KAgitUserInfo& kUserInfo);
	bool GetUserInfo(IN const DWORD& dwUserUID, OUT KAgitUserInfo& kUserInfo);
	void GetUserList(OUT std::map<DWORD, KAgitUserInfo>& mapUser);
	void SetUserList(IN const std::map<DWORD, KAgitUserInfo>& mapUser); // For TestFunc
	void DeleteUserInfo(IN const DWORD& dwUserUID);
	void SetUserServer(IN const DWORD& dwUserUID, IN const DWORD& dwServerUID, IN KAgentPtr& spAgent);
	void GetUserServerList(OUT std::map<DWORD, std::vector<DWORD> >& mapUserServer);
	void RemoveUserServer(IN const DWORD& dwUserUID);
	void SetOption(IN const KAgitOption& kOption);
	UCHAR GetOpenType() { KLocker lock(m_csInfo); return m_kInfo.m_ucOpenType; }

	bool Enter(IN OUT KAgitUserInfo& kUserInfo, IN KAgentPtr& spAgent);
	void Leave(IN const DWORD& dwUserUID);
	USHORT GetUserSlot(IN const DWORD& dwUserUID);

	void SetFlowerPot(IN std::map<GCITEMUID, KFlowerPot>& mapList);
	void GetFlowerPot(OUT std::map<GCITEMUID, KFlowerPot>& mapList);
	USHORT GetActiveCashPotCount();
	USHORT GetActiveNormalPotCount();
	void UpdateFlowerPot(IN const KFlowerPot& kPotInfo);

	void SetBrownieInfo(IN const std::map<GCITEMUID, KBrownieInfo>& mapList) { KLocker lock(m_csBrownie); m_mapBrownieList = mapList; }
	void GetBrownieInfo(OUT std::map<GCITEMUID, KBrownieInfo>& mapList) { KLocker lock(m_csBrownie); mapList = m_mapBrownieList; }
	void UpdateBrownieInfo(IN const KBrownieInfo& kInfo) { KLocker lock(m_csBrownie); m_mapBrownieList[kInfo.m_dwUID] = kInfo; }

	void SetFairyTree(IN const KFairyTree& kInfo) { KLocker lock(m_csFairyTree); m_kFairyTree = kInfo; }
	void GetFairyTree(OUT KFairyTree& kInfo) { KLocker lock(m_csFairyTree); kInfo = m_kFairyTree; }

	void UpdateCashingData();
	void SendAgitVisiterCountBroad();

	void SetOwnerConnect(IN const bool& bVal, IN const time_t& tmRecvTime);
	bool IsOwnerConnect() { KLocker lock(m_csUser); return m_bOwnerConnect; }
	bool ClearCheck();

	void SetReserveDestroy(IN const bool& bVal) { KLocker lock(m_csDestroy); m_bReserveDestroy = bVal; }
	bool IsReserveDestroy() { KLocker lock(m_csDestroy); return m_bReserveDestroy; }

	void SetGuestbook(IN const std::map<int, KGuestMsg>& mapMsg);
	void GetGuestbook(OUT std::map<int, KGuestMsg>& mapMsg);
	void AddGuestMsg(IN const int& nNo, IN const KGuestMsg& kMsg);
	void DeleteGuestMsg(IN const DWORD& dwUserUID, IN OUT std::set<int>& setNo);

	void SetTrainingObj(IN const std::map<GCITEMUID, KTrainingObj>& mapList);
	void GetTrainingObjUpdate(OUT std::map<GCITEMUID, KTrainingObj>& mapList);
	void GetTrainingObj(OUT std::map<GCITEMUID, KTrainingObj>& mapList);
	bool GetCurrentTrainingObj(IN GCITEMUID& ItemUID, OUT KTrainingObj& kObject);
	void UpdateTrainingObj(IN const KTrainingObj& kObject);

	void GetCharacterPos(OUT std::map<int, KAgitCharacterPos>& mapList);
	bool GetCurrentCharacterPos(IN const int& nCharType, OUT KAgitCharacterPos& kInfo);
	void UpdateCharacterPos(IN const int& nCharType, IN const KAgitCharacterPos& kInfo);
	bool IsCharacterPosOpen(IN const int& nCharType);

	void GetUserUID(OUT std::map<DWORD, bool>& mapList);
	void UpdateUserConnectStatus(IN const std::map<DWORD, bool>& mapList);
	void UpdateTotalCharacterInfo(IN const std::map<int, KAgitCharacterPos>& mapCharacterPos);

	template< class T >
	void SendTo(const unsigned short usEventID, const T& data, const DWORD dwUserUID, DWORD dwServerUID);
	template< class T >
	void SendToAll(const unsigned short usEventID, const T& data);
	template< class T >
	void QueueingToAll(const unsigned short usEventID, const T& data);

private:
	virtual void ProcessEvent(const KEventPtr& spEvent);

private:
	KncTimer                            m_kConnectCheckTimer; // 방원 접속체크 주기

	mutable KncCriticalSection          m_csDestroy;
	bool                                m_bReserveDestroy; // 삭제 예약

	mutable KncCriticalSection          m_csPage;
	USHORT                              m_usRoomID; // 페이지 번호
	bool                                m_bPageRegister; // 페이지 등록여부

	mutable KncCriticalSection          m_csServer;
	std::map<DWORD, KAgentPtr>           m_mapServer; // map[ ServerUID, ServerPtr ]
	std::map<DWORD, std::vector<DWORD> > m_mapServerUser; // map[ ServerUID, vector[UserUID] ]

	mutable KncCriticalSection          m_csUser;
	time_t                              m_tmPacketRecvTime; // 주인접속유무 패킷 마지막으로 받은 시간
	bool                                m_bOwnerConnect; // 주인이 접속중인지.
	std::map<DWORD, KAgitUserInfo>       m_mapUserInfo; // map[ UserUID, UserInfo ]
	std::map<DWORD, DWORD>               m_mapUserPlayTime; // map[ UserUID, Time(sec) ]
	std::map<USHORT, DWORD>              m_mapUserSlot; // map[ SlotID, UserUID ]

	mutable KncCriticalSection          m_csInfo;
	KAgitInfo                           m_kInfo; // 아지트 정보
	CTime                               m_tmVisiterCountDate; // 방문자수 날짜
	std::wstring                        m_strOwnerNickname; // 주인 닉네임

	mutable KncCriticalSection          m_csFavorites;
	std::map<DWORD, KAgitPageInfo>       m_mapFavorites; // 즐겨찾기 정보 map[AgitUID,정보]

	mutable KncCriticalSection          m_csRelayServer;
	std::pair<DWORD, USHORT>             m_prTRServerIPPort; // TCP-Relay Server [IP,Port]
	std::pair<DWORD, USHORT>             m_prURServerIPPort; // UDP-Relay Server [IP,Port]

	mutable KncCriticalSection          m_csFlowerPot;
	std::map<GCITEMUID, KFlowerPot>      m_mapFlowerPot; // map[화분ItemUID,화분정보]

	mutable KncCriticalSection          m_csBrownie;
	std::map<GCITEMUID, KBrownieInfo>    m_mapBrownieList; // map[도우미UID,도우미정보]

	mutable KncCriticalSection          m_csFairyTree;
	KFairyTree                          m_kFairyTree; // 요정의 나무

	mutable KncCriticalSection          m_csGuestbook;
	std::map<int, KGuestMsg>             m_mapGuestbook; // 낙서장

	mutable KncCriticalSection          m_csTrainingObj;
	std::map<GCITEMUID, KTrainingObj>    m_mapTrainingObj; // 훈련소

protected:
	_DECL_ON_FUNC(EAG_ENTER_AGIT_BROAD, KAgitUserInfo);
	_DECL_ON_FUNC(EAG_LEAVE_AGIT_BROAD, DWORD);
	_DECL_ON_FUNC(EAG_CHANGE_AGIT_MAP_BROAD, DWORD);
	_DECL_ON_FUNC(EAG_CHANGE_FLOWER_POT_STATE_BROAD, KFlowerPot);
	_DECL_ON_FUNC(EAG_AGIT_CHAT_BROAD, KChatData);
	_DECL_ON_FUNC(EAG_CHANGE_AGIT_OBJECT_POS_BROAD, VEC_OBJECT_POS);
	_DECL_ON_FUNC(EAG_CHANGE_BROWNIE_BROAD, KBrownieInfo);
	_DECL_ON_FUNC(EAG_CHANGE_FAIRY_TREE_BROAD, KFairyTree);
	_DECL_ON_FUNC(EAG_UPDATE_FAIRY_TREE_REQ, time_t);
	_DECL_ON_FUNC(EAG_UPDATE_FAIRY_TREE_ACK, KEAG_UPDATE_FAIRY_TREE_REQ);
	_DECL_ON_FUNC(EAG_UPDATE_AGIT_VISITER_COUNT_NOT, time_t);
	_DECL_ON_FUNC(EAG_CHANGE_AGIT_OPTION_BROAD, KAgitOption);
	_DECL_ON_FUNC(EAG_GUESTBOOK_WRITE_BROAD, KGuestBookWriteBroad);
	_DECL_ON_FUNC(EAG_GUESTBOOK_DELETE_BROAD, KGuestBookDeleteReq);
	_DECL_ON_FUNC(EAG_UPDATE_TRAINING_OBJ_BROAD, KTrainingObj);
	_DECL_ON_FUNC(EAG_CHECK_TRAINING_OBJ_NOT, GCITEMID);
	_DECL_ON_FUNC(EAG_UPDATE_AGIT_CHARACTER_BROAD, KAgitCharacterPos);
	_DECL_ON_FUNC(EAG_UPDATE_AGIT_CHARACTER_POS_BROAD, KSetAgitCharPosAck);
};

template< class T >
void KAgit::SendTo(const unsigned short usEventID_, const T& data_, const DWORD dwUserUID_, DWORD dwServerUID_)
{
	KLocker lock(m_csServer);

	std::vector<DWORD>::iterator vit;
	vit = std::find(m_mapServerUser[dwServerUID_].begin(), m_mapServerUser[dwServerUID_].end(), dwUserUID_);
	__JIF(vit != m_mapServerUser[dwServerUID_].end(), return);

	std::map<DWORD, KAgentPtr>::iterator mit;
	mit = m_mapServer.find(dwServerUID_);
	__JIF(mit != m_mapServer.end(), return);

	mit->second->SendPacket(usEventID_, data_);
}

template< class T >
void KAgit::SendToAll(const unsigned short usEventID_, const T& data_)
{
	KLocker lock(m_csServer);

	std::map<DWORD, KAgentPtr>::iterator mit;
	for (mit = m_mapServer.begin(); mit != m_mapServer.end(); ++mit) {
		if (mit->second == NULL) continue;
		if (m_mapServerUser[mit->first].empty()) continue;

		mit->second->SendPacket(usEventID_, data_);
	}
}

template< class T >
void KAgit::QueueingToAll(const unsigned short usEventID_, const T& data_)
{
	KLocker lock(m_csServer);

	std::map<DWORD, KAgentPtr>::iterator mit;
	for (mit = m_mapServer.begin(); mit != m_mapServer.end(); ++mit) {
		if (mit->second == NULL) continue;
		if (m_mapServerUser[mit->first].empty()) continue;

		mit->second->QueueingEvent(usEventID_, data_);
	}
}