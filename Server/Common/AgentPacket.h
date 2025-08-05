#pragma once

#include "CommonPacket.h"

#pragma pack( push, 1 )

DECL_PACKET(EAG_VERIFY_ACCOUNT_ACK)
{
	int                 m_nOK;
	KSimpleServerInfo   m_kServerInfo;
	int                 m_nSID; // AgentServerID
	bool                m_bEnterance; // 입장가능한지
};

DECL_PACKET(EAG_AGIT_LOADING_COMPLETE_REQ)
{
	DWORD               m_dwAgitUID;
	KAgitUserInfo       m_kUserInfo;
};

DECL_PACKET(EAG_ENTER_AGIT_BROAD)
{
	DWORD                               m_dwAgitUID;    // 아지트UID
	KAgitUserInfo                       m_kUserInfo;    // 입장한 유저 정보
	std::map<DWORD, std::vector<DWORD> > m_mapServerUser; // 패킷 전달할 유저정보 map[ServerUID, vector[UserUID]]
};

DECL_PACKET(EAG_LEAVE_AGIT_REQ)
{
	DWORD               m_dwAgitUID;    // 아지트UID
	DWORD               m_dwUserUID;    // 유저UID
};

typedef KEAG_LEAVE_AGIT_REQ     KEAG_LEAVE_AGIT_ACK;

DECL_PACKET(EAG_LEAVE_AGIT_BROAD)
{
	DWORD                               m_dwAgitUID;    // 아지트UID
	DWORD                               m_dwUserUID;    // 떠나는 유저UID
	std::map<DWORD, std::vector<DWORD> > m_mapServerUser; // 패킷 전달할 유저정보 map[ServerUID, vector[UserUID]]
};

DECL_PACKET(EAG_AGIT_MAP_CATALOGUE_ACK)
{
	DWORD                       m_dwUserUID;    // 아지트주인 유저UID
	GCITEMID                    m_PayItemID;    // 맵구매시 화폐로 쓰이는 아이템ItemID (민들레)
	std::map<DWORD, KAgitMap>    m_mapMapList;   // 맵 상품 리스트
};

DECL_PACKET(EAG_BUY_AGIT_MAP_REQ)
{
	DWORD               m_dwUserUID;    // 아지트주인 유저UID
	DWORD               m_dwMapID;      // 구매할 MapID
	KItem               m_kPayItem;     // 화폐 아이템 보유정보
	int                 m_nPrice;       // 필요한 화폐 개수(서버에서 설정)
};

DECL_PACKET(EAG_CHANGE_AGIT_MAP_BROAD)
{
	DWORD                               m_dwAgitUID;    // 아지트UID
	DWORD                               m_dwMapID;      // 변경된 MapID
	std::map<DWORD, std::vector<DWORD> > m_mapServerUser; // 패킷 전달할 유저정보 map[ServerUID, vector[UserUID]]
};

DECL_PACKET(EAG_CHANGE_FLOWER_POT_STATE_BROAD)
{
	DWORD                               m_dwAgitUID; // 아지트UID
	KFlowerPot                          m_kFlowerPotInfo; // 변경된 화분정보
	std::map<DWORD, std::vector<DWORD> > m_mapServerUser; // 패킷 전달할 유저정보 map[ServerUID, vector[UserUID]]
};

DECL_PACKET(EAG_AGIT_CHAT_BROAD)
{
	DWORD                               m_dwAgitUID; // 아지트UID
	KChatData                           m_kChatData; // 채팅 데이터
	std::map<DWORD, std::vector<DWORD> > m_mapServerUser; // 패킷 전달할 유저정보 map[ServerUID, vector[UserUID]]
};

DECL_PACKET(EAG_CHANGE_AGIT_OBJECT_POS_BROAD)
{
	DWORD                               m_dwAgitUID; // 아지트UID
	std::vector<KAgitObjectPos>         m_vecObject; // 변경된 오브젝트 정보
	std::map<DWORD, std::vector<DWORD> > m_mapServerUser; // 패킷 전달할 유저정보 map[ServerUID, vector[UserUID]]
};

DECL_PACKET(DB_EAG_REGISTER_AGIT_PAGE_REQ)
{
	DWORD               m_dwAgitUID;    // 아지트UID
	std::wstring        m_strNickName;  // 주인 닉네임
	KAgitOption         m_kAgitOption;  // 변경되는 아지트 설정
};

DECL_PACKET(EAG_CHANGE_BROWNIE_BROAD)
{
	DWORD                               m_dwAgitUID; // 아지트UID
	KBrownieInfo                        m_kBrownieInfo; // 변경된 도우미 정보
	std::map<DWORD, std::vector<DWORD> > m_mapServerUser; // 패킷 전달할 유저정보 map[ServerUID, vector[UserUID]]
};

DECL_PACKET(EAG_CHANGE_FAIRY_TREE_BROAD)
{
	DWORD                               m_dwAgitUID; // 아지트UID
	KFairyTree                          m_kFairyTree; // 변경된 요정의나무 정보
	std::map<DWORD, std::vector<DWORD> > m_mapServerUser; // 패킷 전달할 유저정보 map[ServerUID, vector[UserUID]]
};

DECL_PACKET(EAG_UPDATE_FAIRY_TREE_REQ)
{
	DWORD       m_dwAgitUID; // 아지트UID
	KFairyTree  m_kFairyTree; // 요정의나무
};

DECL_DATA(KRankAgit)
{
	enum RANK_TYPE {
		RT_TODAY = 0, // 일일 최고방문자수
		RT_TOTAL = 1, // 전체 최고방문자수
	};

	DWORD       m_dwAgitUID;
	int         m_nType; // RANK_TYPE
	int         m_nRank; // 랭킹순위
	DWORD       m_dwVisitorCount; // 기록된 방문자수
};

DECL_PACKET(EAG_UPDATE_AGIT_VISITER_COUNT_NOT)
{
	DWORD       m_dwAgitUID;
	DWORD       m_nTodayVisiterDiff; // 금일 추가 방문자수
};

DECL_PACKET(EAG_AGIT_VISITER_COUNT_BROAD)
{
	DWORD                               m_dwAgitUID; // 아지트UID
	DWORD                               m_nTodayVisiterCount; // 금일 방문자수
	DWORD                               m_nTotalVisiterCount; // 전체 방문자수
	std::map<DWORD, std::vector<DWORD> > m_mapServerUser; // 패킷 전달할 유저정보 map[ServerUID, vector[UserUID]]
};

DECL_PACKET(EAG_WATERING_FAIRY_TREE_REQ)
{
	KWateringFairyTreeReq   m_kPacket;
	bool                    m_bFruitChange; // 레벨업으로 인해 열매가 바뀌었는지?
};

DECL_PACKET(EAG_CHANGE_AGIT_OPTION_BROAD)
{
	DWORD                               m_dwAgitUID; // 아지트UID
	KAgitOption                         m_kOption; // 아지트 변경된 옵션
	std::map<DWORD, std::vector<DWORD> > m_mapServerUser; // 패킷 전달할 유저정보 map[ServerUID, vector[UserUID]]
};

DECL_PACKET(EAG_LOAD_AGIT_INFO_NOT)
{
	KAgitInfo                           m_kInfo; // 아지트정보
	std::wstring                        m_strNick; // 주인 닉네임
	std::map<GCITEMUID, KFlowerPot>      m_mapFlowerPot; // 화분
	std::map<DWORD, KAgitPageInfo>       m_mapFavoritesInfo; // 즐겨찾기
	std::map<GCITEMUID, KBrownieInfo>    m_mapBrownie; // 도우미
	KFairyTree                          m_kTree; // 요정의나무
	std::map<int, KGuestMsg>             m_mapGuestbook; // 낙서장
	std::map<GCITEMUID, KTrainingObj>    m_mapTrainingObj; // 훈련소
};

DECL_PACKET(EAG_ENTER_AGIT_ACK)
{
	KEnterAgitReq                       m_kEnterAgitReq; // 입장요청 패킷
	KEAG_LOAD_AGIT_INFO_NOT             m_kAgitLoadInfo;
};

DECL_PACKET(EAG_ENTER_AGIT_STAT_NOT)
{
	DWORD       m_dwAgitUID; // 아지트UID
	DWORD       m_dwUserUID; // 유저UID
	DWORD       m_dwPlayTime; // 플레이시간(초)
};

DECL_PACKET(EAG_DELETE_FOR_AGIT_NOT)
{
	DWORD                   m_dwAgitUID; // 아지트UID
	std::vector<GCITEMUID>  m_vecDeleteObject; // 삭제된 아지트 오브젝트
};

DECL_PACKET(EAG_GUESTBOOK_WRITE_BROAD)
{
	DWORD                               m_dwAgitUID; // 아지트UID
	int                                 m_nNo; // 글번호
	KGuestMsg                           m_kGuestMsg; // 글정보
	std::map<DWORD, std::vector<DWORD> > m_mapServerUser; // 패킷 전달할 유저정보 map[ServerUID, vector[UserUID]]
};

DECL_PACKET(EAG_GUESTBOOK_DELETE_BROAD)
{
	DWORD                               m_dwAgitUID; // 아지트UID
	std::set<int>                       m_setDeleteNo; // 지워진 글번호
	std::map<DWORD, std::vector<DWORD> > m_mapServerUser; // 패킷 전달할 유저정보 map[ServerUID, vector[UserUID]]
};

DECL_PACKET(EAG_GUESTBOOK_WRITE_ACK)
{
	int                     m_nOK; // 성공유무
	KGuestBookWriteReq      m_kGuestbookWriteReq; // 글쓰기 요청패킷
	int                     m_nNo; // DB에서 할당받은 글번호
	time_t                  m_tmDate; // 글쓴 시간
};

DECL_PACKET(EAG_GUESTBOOK_DELETE_ACK)
{
	int                     m_nOK; // 성공유무
	KGuestBookDeleteReq     m_kGuestBookDeleteReq; // 글삭제 요청패킷
};

DECL_PACKET(EAG_UPDATE_TRAINING_OBJ_BROAD)
{
	DWORD                               m_dwAgitUID; // 아지트UID
	KTrainingObj                        m_kTrainingObj; // 훈련소 정보
	std::map<DWORD, std::vector<DWORD> > m_mapServerUser; // 패킷 전달할 유저정보 map[ServerUID, vector[UserUID]]
};

DECL_PACKET(EAG_USE_TRAINING_OBJ_REQ)
{
	KUseTrainingObjReq      m_kUseTrainingObjReq; // 아지트 훈련 요청패킷
	KTrainingObj            m_kTrainingObj; // 훈련소 정보
};

DECL_PACKET(EAG_UPDATE_TRAINING_OBJ_REQ)
{
	DWORD                               m_dwAgitUID; // 아지트UID
	std::map<GCITEMUID, KTrainingObj>    m_mapTrainingObj; // 훈련소ItemUID
};

typedef KEAG_UPDATE_TRAINING_OBJ_REQ    KEAG_UPDATE_TRAINING_OBJ_ACK;

DECL_PACKET(EAG_UPDATE_AGIT_CHARACTER_BROAD)
{
	DWORD                               m_dwAgitUID; // 아지트UID
	KAgitCharacterPos                   m_kCharInfo; // 변경된 배치캐릭터 정보
	std::map<DWORD, std::vector<DWORD> > m_mapServerUser; // 패킷 전달할 유저정보 map[ServerUID, vector[UserUID]]
};

DECL_PACKET(EAG_UPDATE_AGIT_CHARACTER_POS_BROAD)
{
	DWORD                               m_dwAgitUID; // 아지트UID
	KSetAgitCharPosReq                  m_kCharPosInfo; // 캐릭터 위치정보
	std::map<DWORD, std::vector<DWORD> > m_mapServerUser; // 패킷 전달할 유저정보 map[ServerUID, vector[UserUID]]
};

DECL_DATA(KUserConnectStatus)
{
	DWORD                   m_dwAgitUID;
	std::map<DWORD, bool>    m_mapUserConnect; // map[UserUID,IsConnect]
};

DECL_PACKET(EAG_USE_FAIRY_TREE_FRUIT_REQ)
{
	DWORD                   m_dwAgitUID;
	int                     m_nFruitType; // 먹은 과일
};

#pragma pack( pop )