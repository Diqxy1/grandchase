#include "stdafx.h"
//
#include ".\kgcinvitemanager.h"
#include "../Square/GCSquare.h"
//
#include "GCUI/GCInviteRequestBox.h"
#include "GCUI/GCInviteUserList.h"
//
#include "MyD3D.h"
#include "BuddySystem/KFriendShip.h"

ImplementSingleton(KGCInviteManager);

KGCInviteManager::KGCInviteManager(void)
	: m_nCurPage(0)
	, m_nMaxPage(0)
	, m_eInviteType(KGCInviteManager::INVITE_TYPE_INVALID)
	, m_dwSquareID(0)
	, m_strSelectSquareName(L"")
	, m_bMessageboxOpen(false)
	, m_dwMessageBoxOpenTime(0)
	, m_bDenyInviteMsg(false)
	, m_bInviteAccept(false)
{
	m_vecInviteUserList.clear();
	m_vecInviteRequest.clear();
}

KGCInviteManager::~KGCInviteManager(void)
{
}

void KGCInviteManager::UpdateUserList()
{
	KP2P::GetInstance()->Send_UserListReq(m_nCurPage);
	if (g_pkUIScene->m_pkInviteRequestBox)
		g_pkUIScene->m_pkInviteRequestBox->SetUserListTitle(g_pkStrLoader->GetString(STR_ID_WAITING_FOR_USER_LIST));
}

void KGCInviteManager::UpdateSquareUserList(DWORD dwSquareID, const std::wstring& strSquareName)
{
	m_strSelectSquareName = strSquareName;
	m_dwSquareID = dwSquareID;
	m_nCurPage = 0;
	KP2P::GetInstance()->Send_SquareUserListReq(dwSquareID, m_nCurPage);
	if (g_pkUIScene->m_pkInviteRequestBox)
		g_pkUIScene->m_pkInviteRequestBox->SetUserListTitle(g_pkStrLoader->GetString(STR_ID_WAITING_FOR_USER_LIST));
}

void KGCInviteManager::SetCurrentPage(int nPage)
{
	if (nPage >= 0 && nPage < m_nMaxPage)
	{
		m_nCurPage = nPage;
	}
}

void KGCInviteManager::AddInviteRequest(const KInviteInfo& kRequest)
{
	if (m_bDenyInviteMsg == true)
		return;

	m_vecInviteRequest.push_back(kRequest);
}

void KGCInviteManager::FrameMove()
{
	PROFILE_SET("KGCInviteManager::FrameMove");

	if (true == m_bMessageboxOpen || m_vecInviteRequest.empty())
	{
		return;
	}

	if (g_MyD3D->m_pStateMachine->GetState() == GS_ROOM || g_MyD3D->m_kGuideMgr.IsGuideOngoing())
	{
		m_vecInviteRequest.clear();
		return;
	}

	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_SEASON2_INVITE);
	SetMessageBoxOpen(true);
}

const KInviteInfo& KGCInviteManager::GetInviteInfo() const
{
	ASSERT(m_vecInviteRequest.empty() == false);
	return (*m_vecInviteRequest.begin());
}

void KGCInviteManager::PopInviteInfo()
{
	ASSERT(m_vecInviteRequest.empty() == false);
	m_vecInviteRequest.erase(m_vecInviteRequest.begin());
}

void KGCInviteManager::SetDenyInviteMsg(bool bDeny, bool bForceSend/* = false*/)
{
	if (m_bDenyInviteMsg != bDeny || true == bForceSend)
	{
		// send
		if (KP2P::GetInstance()->IsConnectedToGameServer())
		{
			KP2P::GetInstance()->Send_Invite_Deny_Not(bDeny);
		}
	}
	m_bDenyInviteMsg = bDeny;
}

void KGCInviteManager::ClearInviteList()
{
	ASSERT(m_bMessageboxOpen != true);
	m_vecInviteRequest.clear();
}

void KGCInviteManager::SetInviteFriendUserList(const std::vector<KInviteUserInfo>& vecInviteUserList, int iMode_, bool bAddOffline_)
{
	std::vector<DWORD> vecFriendList;

	//채널 검사 루틴.

	//여기 수정 일어나면 ChannelListBox.cpp쪽도 수정해 주시오.
	int iUserGrade = 0;
	bool bEnableChannel = false;
	if (g_kGlobalValue.GetServerInfo()->CheckServerType(ST_GUILD))
	{
		iUserGrade = static_cast<int>(g_kGlobalValue.m_kGuildUserInfo.m_ChannelGrade);
	}
	else
	{
		iUserGrade = static_cast<int>(g_kGlobalValue.m_kUserInfo.GetUserGrade());
	}
	EChannelType eChannelType = g_MyD3D->GradeToChannelType(iUserGrade);

	m_vecInviteFriendUserList.clear();
	std::vector<KInviteFriendUserInfo> veckInviteDisablefriList;
	veckInviteDisablefriList.clear();

	//일단 목록에 있는 유저들 중에... 채널에 초대 가능한 사람부터 백터에 집어 넣는다.
	std::set<DWORD> setEnableUsers;
	for (int i = 0; i < (int)vecInviteUserList.size(); i++)
	{
		bool bEnableChannel = false;
		KInviteFriendUserInfo kInviteFriendUserinfo;
		kInviteFriendUserinfo.m_cBestCharGrade = vecInviteUserList[i].m_cBestCharGrade;
		kInviteFriendUserinfo.m_cGrade = vecInviteUserList[i].m_cGrade;
		kInviteFriendUserinfo.m_strNickName = vecInviteUserList[i].m_strNickName;
		kInviteFriendUserinfo.m_dwUserUID = vecInviteUserList[i].m_dwUserUID;

		KBuddy fdata;
		SiKFriendShip()->GetFriendBuddyByUID(vecInviteUserList[i].m_dwUserUID, fdata);
		kInviteFriendUserinfo.m_strLocation = fdata.GetServerName() + L"/" + fdata.GetLocation();

		if (g_MyD3D->GradeToChannelType(vecInviteUserList[i].m_cGrade) == eChannelType || // 방장과 동일한 채널 타입이거나..
			SiKGCChannelManager()->GetCurrentChannelType() == CT_UNLIMITED ||
			SiKGCChannelManager()->GetCurrentChannelType() == CT_DUNGEON ||
			SiKGCChannelManager()->GetCurrentChannelType() == CT_GUILD_UNLIMITED
			)  // 자유 채널이나 던젼이면..
			bEnableChannel = true; // 들어갈 수 있는 채널임

		//길드 채널 예외
		if (iMode_ == GCNateOn::GUILD_MODE)
		{
			bEnableChannel = true;
		}

		if (bEnableChannel == true)
		{
			kInviteFriendUserinfo.m_bInviteOK = true;
			m_vecInviteFriendUserList.push_back(kInviteFriendUserinfo);
			setEnableUsers.insert(kInviteFriendUserinfo.m_dwUserUID);
		}
	}

	vecFriendList.clear();
	SiKFriendShip()->GetOnlienFriendList((GCNateOn::E_NATEON_MODE)iMode_, vecFriendList);

	//온라인 유저 중에 목록에 없는 분들 싹 넣음..
	for (int i = 0; i < (int)vecFriendList.size(); i++)
	{
		bool bCheck = setEnableUsers.find(vecFriendList[i]) != setEnableUsers.end();

		if (bCheck == false)
		{
			KInviteFriendUserInfo kInviteFriendUserinfo;
			KBuddy fdata;
			SiKFriendShip()->GetFriendBuddyByUID(vecFriendList[i], fdata);
			kInviteFriendUserinfo.m_cGrade = (char)-1;
			kInviteFriendUserinfo.m_cBestCharGrade = (char)-1;
			kInviteFriendUserinfo.m_dwUserUID = vecFriendList[i];
			kInviteFriendUserinfo.m_strNickName = fdata.GetNickName();
			kInviteFriendUserinfo.m_strLocation = fdata.GetServerName() + L"/" + fdata.GetLocation();
			kInviteFriendUserinfo.m_bInviteOK = false;
			m_vecInviteFriendUserList.push_back(kInviteFriendUserinfo);
		}
	}

	if (bAddOffline_)
	{
		vecFriendList.clear();
		SiKFriendShip()->GetOffLineFriendList((GCNateOn::E_NATEON_MODE)iMode_, vecFriendList);

		//오프라인인 녀석들 넣어준다.
		for (int i = 0; i < (int)vecFriendList.size(); i++)
		{
			KInviteFriendUserInfo kInviteFriendUserinfo;
			KBuddy fdata;
			SiKFriendShip()->GetFriendBuddyByUID(vecFriendList[i], fdata);
			kInviteFriendUserinfo.m_cGrade = (char)-1;
			kInviteFriendUserinfo.m_cBestCharGrade = (char)-1;
			kInviteFriendUserinfo.m_dwUserUID = vecFriendList[i];
			kInviteFriendUserinfo.m_strNickName = fdata.GetNickName();
			kInviteFriendUserinfo.m_strLocation = g_pkStrLoader->GetString(STR_ID_OFFLINE);
			kInviteFriendUserinfo.m_bInviteOK = false;
			m_vecInviteFriendUserList.push_back(kInviteFriendUserinfo);
		}
	}

	SiKGCInviteManager()->SetMaxPage(m_vecInviteFriendUserList.size() / INVITE_USER_NUM + 1);
}

bool KGCInviteManager::JoinToInviteRoom()
{
	const KInviteInfo& kInfo = GetInviteInfo();

	// 입장 티켓조건이 걸린 던전인지 확인
	if (KGCRoomManager::ENTER_IMPOSSIBLE == SiKGCRoomManager()->CheckTicketToDungeonEnter(true, kInfo.m_nGameMode))
		return false;

	// 나중에 채널에 들어가기 위해서.

	int nPlayType = SiKGCWorldMapManager()->GetHeroDungeonPlayType(static_cast<EGCGameMode>(kInfo.m_nGameMode));

	switch (nPlayType)
	{
	case KGCWorldMapManager::GRPT_NOT_INFO:
	case KGCWorldMapManager::GRPT_ADMISSION:
		break;
	case KGCWorldMapManager::GRPT_IMPOSSIBLE_ADMISSION:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR4),
			L"",
			KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		return false;
	}
	case KGCWorldMapManager::GRPT_CONDITIONS_BELOW:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR8),
			L"",
			KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		return false;
	}
	case KGCWorldMapManager::GRPT_IMPOSSIBLE_TIME:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR2),
			L"",
			KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		return false;
	}
	default:
		return false;
	}

	if (!SiKGCPrivateDungeonManager()->IsClearPrivateDungeon(g_kGlobalValue.m_kUserInfo.GetCurrentCharType()))
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetString(STR_ID_NO_CLEAR_PRIVATE_DUNGEON),
			L"",
			KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		return false;
	}

	SetCurrentInviteInfo(kInfo);

	if (kInfo.m_bCenterRoom)
	{
		bool bPreState = SiKGCRoomManager()->IsChannelRoomList();
		SiKGCRoomManager()->SetChannelRoomList(true);
		EChannelType eType = (EChannelType)SiKGCChannelManager()->GetChannel(kInfo.m_dwChannelUID)->m_cType;
		SiKGCRoomManager()->UpdateShowGameModeCategory(false, eType);
		//그냥 초대
		if (FAILED(SiKGCRoomManager()->JoinRoom(KJoinRoomReqInfo::JOIN_NORMAL, kInfo.m_usRoomID, kInfo.m_strRoomPasswd, true)))
		{
			SiKGCRoomManager()->SetChannelRoomList(bPreState);
			EChannelType eType = SiKGCChannelManager()->GetCurrentChannelType();
			SiKGCRoomManager()->UpdateShowGameModeCategory(false, eType);
			// 근데 실패하면 아무처리 안해도 된다. 이미 JoinRoom 안에서 에러 메세지 박스는 불러줌.
			return false;
		}
	}
	else
	{
		if (g_MyD3D->m_pStateMachine->GetState() != GS_SQUARE)
		{
			//채널이 같은 경우..
			if (kInfo.m_dwChannelUID == SiKGCChannelManager()->GetCurrentChannel())
			{
				//그냥 초대
				if (FAILED(SiKGCRoomManager()->JoinRoom(KJoinRoomReqInfo::JOIN_NORMAL, kInfo.m_usRoomID, kInfo.m_strRoomPasswd, false)))
				{
					// 근데 실패하면 아무처리 안해도 된다. 이미 JoinRoom 안에서 에러 메세지 박스는 불러줌.
					return false;
				}
			}
			else
			{
				//다른 채널에 있는 사람 초대하는 경우...
				KInDoorUserInfo kIndoorUserInfo;
				g_kGlobalValue.m_kUserInfo.SetIndoorUserInfo(&kIndoorUserInfo);
				KP2P::GetInstance()->Send_JoinRoomFromChannelReq(kInfo.m_dwChannelUID, kInfo.m_usRoomID, kInfo.m_dwHostUID, kInfo.m_strRoomPasswd, kIndoorUserInfo);
				Result_Join_Other_Channel = -1;
				if (false == g_MyD3D->WaitForServerAck(Result_Join_Other_Channel, -1, 5000, INT_MAX))
				{
					// timeout
					g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1));
					return false;
				}
			}
		}
		else
		{
			//광장에서 초대
			KInDoorUserInfo kIndoorUserInfo;
			g_kGlobalValue.m_kUserInfo.SetIndoorUserInfo(&kIndoorUserInfo);
			KP2P::GetInstance()->Send_InvitedRoomFromSquareReq(kInfo.m_dwChannelUID, kInfo.m_usRoomID, kInfo.m_dwHostUID, kInfo.m_strRoomPasswd, kIndoorUserInfo);
			Result_Join_From_Square = -1;
			if (false == g_MyD3D->WaitForServerAck(Result_Join_From_Square, -1, 5000, INT_MAX))
			{
				// timeout
				g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1));
				return false;
			}
		}
	}
	return true;
}

ImplementSingleton(KGCMatchInviteManager);

KGCMatchInviteManager::KGCMatchInviteManager(void)
{
}

KGCMatchInviteManager::~KGCMatchInviteManager(void)
{
}

bool KGCMatchInviteManager::GetCurrentPageUserList(int iCurrent, int iMax, OUT std::vector<KMatchInviteUserInfo>& vecResult)
{
	if (iCurrent < 0) return false;

	int iUserNum = static_cast<int>(m_vecMatchInviteList.size());

	if (iUserNum < iMax) {
		vecResult = m_vecMatchInviteList;
		return true;
	}

	if (iCurrent > (iUserNum - 1) / iMax)
		return false;

	int iStartIndex = iMax * iCurrent;
	int iEndIndex = iStartIndex + iMax - 1;

	if (iEndIndex > iUserNum)
		iEndIndex = iUserNum;

	std::copy(m_vecMatchInviteList.begin() + iStartIndex, m_vecMatchInviteList.begin() + iEndIndex, std::back_inserter(vecResult));

	return true;
}