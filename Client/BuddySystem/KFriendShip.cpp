#include "stdafx.h"
//
#include ".\kfriendship.h"
//

//
#include "../gcui/GCNateOnAddFriend.h"
#include "../gcui/KGCRecommendList.h"
#include "../MyD3D.h"
#include "../GCStateMachine.h"
#include "GCCoupleSystem.h"
#include "GCAchieveManager.h"

ImplementSingleton(KFriendShip);

KFriendShip::KFriendShip(void)
	: m_bNewPaperAlarm(false)
	, m_dwEmoticon(KFriend::DEFAULT_EMOTICON_ID) // 기본아이디 받아야함
	, DEFGROUPID(10)
{
	m_mapBuddyGroup.clear();
	m_vecFriendReq.clear();
	m_mapFriendShipData.clear();
	m_vecContactFriendList.clear();
	m_vecWaitRecommenders.clear();
	m_iAttendedRecommender = 0;
}

KFriendShip::~KFriendShip(void)
{
	m_mapBuddyGroup.clear();
	m_vecFriendReq.clear();
	m_mapFriendShipData.clear();
	m_vecContactFriendList.clear();
	m_vecWaitRecommenders.clear();
}

void KFriendShip::SetData(const KEMS_S2_VERIFY_ACCOUNT_ACK& kPacket)
{
	std::map<int, KGroup>::const_iterator mit;
	for (mit = kPacket.m_mapFriendGroup.begin(); mit != kPacket.m_mapFriendGroup.end(); ++mit)
	{
		if (mit->first != KGroup::GUILD_BASE)
			m_mapBuddyGroup[mit->first].SetData(mit->second);
	}

	std::map<int, KGroup>::const_iterator mitGuild = kPacket.m_mapFriendGroup.find(KGroup::GUILD_BASE);

	m_kBlockGroup.SetData(kPacket.m_kBlockGroup);
	if (mitGuild != kPacket.m_mapFriendGroup.end())
		m_kGuildGroup.SetData(mitGuild->second);

	m_bNewPaperAlarm = kPacket.m_bNewPaperAlarm;

	m_dwEmoticon = kPacket.m_dwEmoticon;

	if (KFriend::DEFAULT_EMOTICON_ID != m_dwEmoticon)
		m_dwEmoticon /= 10;

	UpdateFriendShipData();
}

void KFriendShip::AddBuddy(const KFriend& kFriendInfo)
{
	if (false == m_mapBuddyGroup[DEFGROUPID].AddBuddy(kFriendInfo))
	{
		// 뭔가 잘못됨.
		assert(!"false == m_mapBuddyGroup[DEFGROUPID].AddBuddy( kFriendInfo )");
	}
	UpdateFriendShipData();

	SiKGCAchieveManager()->OccurAction(SAchieve::MSN_ADD_FRIEND, GC_CHAR_INVALID, GetDataUserCount(GCNateOn::FRIEND_MODE));
}

void KFriendShip::AddBuddy(const KEMS_S2_ADD_FRIEND_ACK& kPacket)
{
	// 기본그룹에다가 넣으면 됨.
	AddBuddy(kPacket.m_kFriendInfo);
}

void KFriendShip::RegisterFriendRequest(const KEMS_S2_ADD_FRIEND_NOT& kPacket)
{
	for (iterFriendReq vit = m_vecFriendReq.begin(); vit != m_vecFriendReq.end(); ++vit)
	{
		KEMS_S2_ADD_FRIEND_NOT& kReq = (*vit);
		if (kReq.m_dwSenderUID == kPacket.m_dwSenderUID)
		{
			// 이미 요청한 상태이다 ( 서버에서 한번 걸러주지만 여기서도 체크 )
			return;
		}
	}
	m_vecFriendReq.push_back(kPacket);
}

void KFriendShip::DeleteBuddy(const KEMS_S2_DELETE_FRIEND_ACK& kPacket)
{
	if (kPacket.m_nGroupID == m_kBlockGroup.GetGroupID())
	{
		if (false == m_kBlockGroup.DeleteBuddy(kPacket.m_dwFriendUID))
		{
			// 실패
			assert(!"false == m_kBlockGroup.DeleteBuddy( kPacket.m_dwFriendUID )");
		}
	}
	else
	{
		iterBuddyGroup mit = m_mapBuddyGroup.find(kPacket.m_nGroupID);
		if (mit == m_mapBuddyGroup.end())
		{
			assert(!"mit == m_mapBuddyGroup.end()");
		}
		KBuddyGroup& kGroup = mit->second;
		if (false == kGroup.DeleteBuddy(kPacket.m_dwFriendUID))
		{
			assert(!"false == kGroup.DeleteBuddy( kPacket.m_dwFriendUID )");
		}
	}
	UpdateFriendShipData();
}

void KFriendShip::RejectMyRequest(DWORD dwRejectUID)
{
	// 내 요청을 거절한 건방진 친구
	if (false == m_mapBuddyGroup[DEFGROUPID].DeleteBuddy(dwRejectUID))
	{
		// 내 요청을 거절한 건방진 친구 삭제 실패
		assert(!"false == m_mapBuddyGroup[DEFGROUPID].DeleteBuddy( dwRejectUID )");
	}
	UpdateFriendShipData();
}

void KFriendShip::UpdateBuddy(const KEMS_S2_UPDATE_FRIENDS_NOT& kPacket)
{
	std::map< int, std::map<DWORD, KFriend> >::const_iterator mit;
	for (mit = kPacket.begin(); mit != kPacket.end(); ++mit)
	{
		if (mit->first == m_kBlockGroup.GetGroupID())
		{
			m_kBlockGroup.UpdateBuddy(mit->second);
		}
		else if (mit->first == m_kGuildGroup.GetGroupID())
		{
			m_kGuildGroup.UpdateBuddy(mit->second);
		}
		else
		{
			iterBuddyGroup mitGroup = m_mapBuddyGroup.find(mit->first);
			if (mitGroup != m_mapBuddyGroup.end())
			{
				KBuddyGroup& kGroup = mitGroup->second;
				kGroup.UpdateBuddy(mit->second);
			}
		}
	}
	UpdateFriendShipData();
}

void KFriendShip::UpdateBuddy(const KFriend& kPacket)
{
	iterBuddyGroup mit;
	for (mit = m_mapBuddyGroup.begin(); mit != m_mapBuddyGroup.end(); ++mit)
	{
		KBuddyGroup& kGroup = mit->second;
		kGroup.UpdateBuddy(kPacket);
	}
	m_kBlockGroup.UpdateBuddy(kPacket); // block된 유저도 update 해줘야하나?
	m_kGuildGroup.UpdateBuddy(kPacket);
	UpdateFriendShipData();
}

// 내 친구블럭
void KFriendShip::BlockBuddy(const KEMS_S2_BLOCK_MOVE_FRIEND_ACK& kPacket)
{
	iterBuddyGroup mit = m_mapBuddyGroup.find(kPacket.m_nGroupID);
	if (mit != m_mapBuddyGroup.end())
	{
		KBuddyGroup& kGroup = mit->second;
		KBuddy kBlockBuddy;
		kGroup.GetBuddy(kPacket.m_dwFriendUID, kBlockBuddy);
		kGroup.DeleteBuddy(kPacket.m_dwFriendUID);

		KFriend kBlockFriend;
		kBlockBuddy.GetData(kBlockFriend);
		kBlockFriend.m_bIsOnline = false;
		kBlockBuddy.SetData(kBlockFriend);
		m_kBlockGroup.AddBuddy(kBlockBuddy);
	}
	UpdateFriendShipData();

	SiKGCAchieveManager()->OccurAction(SAchieve::MSN_BLOCK_FRIEND, GC_CHAR_INVALID, GetDataUserCount(GCNateOn::BLOCK_MODE));
}

// 쌩블럭
void KFriendShip::BlockBuddy(const KEMS_S2_BLOCK_FRIEND_ACK& kPacket)
{
	for (iterBuddyGroup mit = m_mapBuddyGroup.begin(); mit != m_mapBuddyGroup.end(); ++mit)
	{
		KBuddyGroup& kGroup = mit->second;
		kGroup.DeleteBuddy(kPacket.m_dwBlockUID);
	}

	KFriend kBlockFriend;
	kBlockFriend.m_dwUserUID = kPacket.m_dwBlockUID;
	kBlockFriend.m_strNickName = kPacket.m_strBlockNick;
	kBlockFriend.m_strCharNickName = kPacket.m_strBlockCharNick;
	kBlockFriend.m_bIsOnline = false;
	KBuddy kBlockBuddy;
	kBlockBuddy.SetData(kBlockFriend);
	m_kBlockGroup.AddBuddy(kBlockBuddy);
	UpdateFriendShipData();

	SiKGCAchieveManager()->OccurAction(SAchieve::MSN_BLOCK_FRIEND, GC_CHAR_INVALID, GetDataUserCount(GCNateOn::BLOCK_MODE));
}

bool KFriendShip::UnBlockBuddy(const DWORD dwUID_)
{
	return m_kBlockGroup.DeleteBuddy(dwUID_);
}

void KFriendShip::UnBlockBuddy(const KEMS_S2_UNBLOCK_FRIEND_ACK& kPacket)
{
	if (false == m_kBlockGroup.DeleteBuddy(kPacket.m_dwBlockUID))
	{
		// 차단목록에 없음
		assert(!"false == m_kBlockGroup.DeleteBuddy( kPacket.m_dwBlockUID )");
	}
	UpdateFriendShipData();
}

void KFriendShip::MoveBuddy_BlockToFriend(const KEMS_S2_UNBLOCK_MOVE_FRIEND_ACK& kPacket)
{
	KBuddy kUnBlockBuddy;
	if (true == m_kBlockGroup.GetBuddy(kPacket.m_dwFriendUID, kUnBlockBuddy))
	{
		m_mapBuddyGroup[DEFGROUPID].AddBuddy(kUnBlockBuddy);
		m_kBlockGroup.DeleteBuddy(kPacket.m_dwFriendUID);
	}
	else
	{
		// 차단 목록에 없음
		assert(!"true == m_kBlockGroup.GetBuddy( kPacket.m_dwFriendUID, kUnBlockBuddy )");
	}
	UpdateFriendShipData();
}

void KFriendShip::UnBlockBuddy(const KEMS_S2_UNBLOCK_MOVE_FRIEND_ACK& kPacket)
{
	KBuddy kUnBlockBuddy;
	if (true == m_kBlockGroup.GetBuddy(kPacket.m_dwFriendUID, kUnBlockBuddy))
	{
		m_kBlockGroup.DeleteBuddy(kPacket.m_dwFriendUID);
	}
	else
	{
		// 차단 목록에 없음
		assert(!"true == m_kBlockGroup.GetBuddy( kPacket.m_dwFriendUID, kUnBlockBuddy )");
	}
	UpdateFriendShipData();
}

void KFriendShip::AddBuddyGroup(const KEMS_S2_ADD_GROUP_ACK& kPacket)
{
	if (kPacket.m_nGroupID == m_kBlockGroup.GetGroupID() ||
		kPacket.m_nGroupID == m_kGuildGroup.GetGroupID())
	{
		assert(!"AddBuddyGroup : ???");
		return;
	}
	m_mapBuddyGroup[kPacket.m_nGroupID].SetGroupID(kPacket.m_nGroupID);
	m_mapBuddyGroup[kPacket.m_nGroupID].SetGroupName(kPacket.m_strGroupName);
	UpdateFriendShipData();
}

void KFriendShip::DeleteBuddyGroup(const KEMS_S2_DELETE_GROUP_ACK& kPacket)
{
	iterBuddyGroup mit = m_mapBuddyGroup.find(kPacket.m_nGroupID);
	if (mit != m_mapBuddyGroup.end())
	{
		m_mapBuddyGroup.erase(mit);
	}
	UpdateFriendShipData();
}

void KFriendShip::RenameBuddyGroup(const KEMS_S2_RENAME_GROUP_ACK& kPacket)
{
	iterBuddyGroup mit = m_mapBuddyGroup.find(kPacket.m_nGroupID);
	if (mit != m_mapBuddyGroup.end())
	{
		KBuddyGroup& kGroup = mit->second;
		kGroup.SetGroupName(kPacket.m_strGroupName);
	}
	UpdateFriendShipData();
}

void KFriendShip::MoveBuddy(const KEMS_S2_MOVE_FRIEND_TO_GROUP_ACK& kPacket)
{
	iterBuddyGroup mit = m_mapBuddyGroup.find(kPacket.m_nFromGroupID);
	if (mit == m_mapBuddyGroup.end())
	{
		assert(!"MoveBuddy : ???");
		return;
	}
	KBuddyGroup& kFromGroup = mit->second;
	KBuddy kMoveBuddy;
	if (false == kFromGroup.GetBuddy(kPacket.m_dwFriendUID, kMoveBuddy))
	{
		assert(!"MoveBuddy : ???");
		return;
	}
	mit = m_mapBuddyGroup.find(kPacket.m_nToGroupID);
	if (mit == m_mapBuddyGroup.end())
	{
		assert(!"MoveBuddy : ???");
		return;
	}
	kFromGroup.DeleteBuddy(kPacket.m_dwFriendUID);
	KBuddyGroup& kToGroup = mit->second;
	kToGroup.AddBuddy(kMoveBuddy);
	UpdateFriendShipData();
}

void KFriendShip::SetMyEmoticon(const KEMS_S2_SET_EMOTICON_ACK& kPacket)
{
	m_dwEmoticon = kPacket.m_dwEmoticonID;

	if (KFriend::DEFAULT_EMOTICON_ID != m_dwEmoticon)
		m_dwEmoticon /= 10;
}

void KFriendShip::UpdateFriendShipData()
{
	m_mapFriendShipData.clear();
	iterBuddyGroup itGroup;
	for (itGroup = m_mapBuddyGroup.begin(); itGroup != m_mapBuddyGroup.end(); ++itGroup)
	{
		KBuddyGroup& kGroup = itGroup->second;
		kGroup.GetFriendShipData(m_mapFriendShipData[GCNateOn::FRIEND_MODE]);
	}
	m_kBlockGroup.GetFriendShipData(m_mapFriendShipData[GCNateOn::BLOCK_MODE]);
	m_kGuildGroup.GetFriendShipData(m_mapFriendShipData[GCNateOn::GUILD_MODE]);

	// 내 커플 봐주자
#if defined( COUPLE_SYSTEM )
	JIF(KP2P::GetInstance()->m_kMsgServer != NULL);
	if (SiKGCCoupleSystem()->Am_I_Solo() == false)
	{
		// 아 귀찮아...친구 목록에 없으면 친구 추가시켜주고 블럭이면 돌려주고
		DWORD myHoney = SiKGCCoupleSystem()->GetUID_of_my_honey();
		KBuddy kData;
		bool bFriend = GetFriendBuddyByUID(myHoney, kData);
		bool bBlock = GetBlockBuddy(myHoney, kData);

		if (bFriend == false)
		{
			KP2P::GetInstance()->m_kMsgServer->Send_ForceMakeFriend(g_kGlobalValue.m_kUserInfo.dwUID, g_kGlobalValue.m_kUserInfo.strNickName,
				myHoney, SiKGCCoupleSystem()->GetNickName_of_my_honey());
		}
		else if (bBlock == true)
		{
			KP2P::GetInstance()->m_kMsgServer->Send_UnBlockMoveFriendReq_S2(myHoney);
		}
	} // 끝
#endif
}

void KFriendShip::GetFriendShipData(GCNateOn::E_NATEON_MODE eMode, IN int iTopIdx, OUT std::vector< FRIEND_UI_DATA >& vecRet, const int iChildSize /*= MAX_CHILD_NUM*/, bool bExcludeHoney /*= true*/)
{
	assert(eMode >= GCNateOn::FRIEND_MODE && eMode <= GCNateOn::BLOCK_MODE);

	VEC_FRIEND_UI_DATA& vecData = m_mapFriendShipData[eMode];
	VEC_FRIEND_UI_DATA::iterator vit;
	int iCount = 0;
	for (vit = vecData.begin(); vit != vecData.end(); ++vit)
	{
		if (bExcludeHoney && (eMode != GCNateOn::BLOCK_MODE))
		{
			if (SiKGCCoupleSystem()->Are_you_My_Honey((*vit).m_dwUserUID))
				continue;
		}

		if (iCount < iTopIdx)
		{
			iCount++;
		}
		else
		{
			vecRet.push_back((*vit));
			if (iChildSize == vecRet.size())
			{
				return;
			}
		}
	}
}

void KFriendShip::ToggleGroupOpen(FS_GROUPID iId)
{
	assert(iId != -1);
	if (m_kBlockGroup.GetGroupID() == iId)
	{
		m_kBlockGroup.SetOpenStatus(!m_kBlockGroup.GetOpenStatus());
	}
	else if (m_kGuildGroup.GetGroupID() == iId)
	{
		m_kGuildGroup.SetOpenStatus(!m_kGuildGroup.GetOpenStatus());
	}
	else
	{
		iterBuddyGroup mit = m_mapBuddyGroup.find(iId);
		assert(mit != m_mapBuddyGroup.end());
		mit->second.SetOpenStatus(!mit->second.GetOpenStatus());
	}
	UpdateFriendShipData();
}

int KFriendShip::GetDataCount(GCNateOn::E_NATEON_MODE eMode)
{
	std::map< GCNateOn::E_NATEON_MODE, VEC_FRIEND_UI_DATA >::iterator mit;
	mit = m_mapFriendShipData.find(eMode);
	assert(mit != m_mapFriendShipData.end());
	return (int)mit->second.size();
}

void KFriendShip::AddBuddyGroupReq(std::wstring strGroupName)
{
	JIF(KP2P::GetInstance()->m_kMsgServer != NULL);
	KP2P::GetInstance()->m_kMsgServer->Send_AddGroupReq_S2(strGroupName);
}

void KFriendShip::GetFriendGroupVector(OUT std::vector< KBuddyGroup >& vecRet)
{
	for (iterBuddyGroup mit = m_mapBuddyGroup.begin(); mit != m_mapBuddyGroup.end(); ++mit)
	{
		vecRet.push_back(mit->second);

		if (MAX_GROUP_NUM == (int)vecRet.size())
		{
			return;
		}
	}
}

void KFriendShip::RenameGroupReq(int nGroupID, std::wstring strGroupName)
{
	JIF(KP2P::GetInstance()->m_kMsgServer != NULL);
	KP2P::GetInstance()->m_kMsgServer->Send_RenameGroupReq_S2(nGroupID, strGroupName);
}

void KFriendShip::GetGroupName(IN int nGroupID, OUT std::wstring& strGroupName)
{
	iterBuddyGroup mit = m_mapBuddyGroup.find(nGroupID);
	if (mit == m_mapBuddyGroup.end())
	{
		assert(!"GetGroupName : ???");
		return;
	}

	strGroupName = mit->second.GetGroupName();
}

void KFriendShip::DeleteGroupReq(int nGroupID)
{
	JIF(KP2P::GetInstance()->m_kMsgServer != NULL);
	KP2P::GetInstance()->m_kMsgServer->Send_DeleteGroupReq_S2(nGroupID);
}

void KFriendShip::MoveBuddyReq(int nFrom, int nTo, UINT dwUID)
{
	JIF(KP2P::GetInstance()->m_kMsgServer != NULL);
	KP2P::GetInstance()->m_kMsgServer->Send_MoveFriendGroupReq_S2(nFrom, nTo, dwUID);
}

void KFriendShip::BlockMyFriendReq(int nGroupID, DWORD dwUID)
{
	JIF(KP2P::GetInstance()->m_kMsgServer != NULL);
	KP2P::GetInstance()->m_kMsgServer->Send_BlockMoveFriendReq_S2(nGroupID, dwUID);
}

void KFriendShip::UnBlockMyFriendReq(DWORD dwUID)
{
	JIF(KP2P::GetInstance()->m_kMsgServer != NULL);

	KBuddy kUnBlockBuddy;
	if (false == m_kBlockGroup.GetBuddy(dwUID, kUnBlockBuddy))
	{
		assert(!"UnBlockMyFriendReq : ???");
		return;
	}

	// 이놈은 친구도 아니여
	if (false == kUnBlockBuddy.IsFriend())
	{
		KP2P::GetInstance()->m_kMsgServer->Send_UnBlockFriendReq_S2(dwUID);
	}
	else
	{
		KP2P::GetInstance()->m_kMsgServer->Send_UnBlockMoveFriendReq_S2(dwUID);
	}
}

void KFriendShip::DeleteBuddyReq(int nGroupID, DWORD dwUID)
{
	JIF(KP2P::GetInstance()->m_kMsgServer != NULL);
	KP2P::GetInstance()->m_kMsgServer->Send_DeleteFriendReq_S2(dwUID, nGroupID);
}

void KFriendShip::BlockNoFriendReq(std::wstring strNickName)
{
	JIF(KP2P::GetInstance()->m_kMsgServer != NULL);

	for (iterBuddyGroup mit = m_mapBuddyGroup.begin(); mit != m_mapBuddyGroup.end(); ++mit)
	{
		KBuddyGroup& kGroup = mit->second;
		KBuddy kBuddy;
		if (true == kGroup.GetBuddyByNickName(strNickName, kBuddy))
		{
			// 내 친구 블럭
			KP2P::GetInstance()->m_kMsgServer->Send_BlockMoveFriendReq_S2(kGroup.GetGroupID(), kBuddy.GetUID());
			return;
		}
	}
	//쌩블럭
	KP2P::GetInstance()->m_kMsgServer->Send_BlockFriendReq_S2(strNickName);
}

bool KFriendShip::SetEmoticon(DWORD dwEmoticon)
{
	if (m_dwEmoticon == dwEmoticon || KFriend::DEFAULT_EMOTICON_ID == dwEmoticon)
		return false;
	m_dwEmoticon = dwEmoticon;
	_JIF(KP2P::GetInstance()->m_kMsgServer != NULL, return false);

	if (KFriend::DEFAULT_EMOTICON_ID != m_dwEmoticon)
		m_dwEmoticon *= 10;

	KP2P::GetInstance()->m_kMsgServer->Send_SetEmoticonReq_S2(m_dwEmoticon);
	return true;
}

bool KFriendShip::IsOpenAddFriendNot()
{
	GAME_STATE emState = g_MyD3D->m_pStateMachine->GetState();
	switch (emState)
	{
	case GS_GUILD:
	case GS_SHOP_CASH:
	case GS_SHOP_GP:
	case GS_ROOM:
	case GS_MY_INFO:
	case GS_MY_INFO_FROM_ROOM:
	case GS_WORLDMAP:
	case GS_MATCH:
	case GS_SQUARE:
		return true;

	case GS_GAME:
	{
		if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT)
			return true;
	}
	break;

	default:
		return false;
	}

	return false;
}

void KFriendShip::CheckAlramAddFriend(void)
{
	if (GC_RUS_READY == g_kGlobalValue.m_kUserInfo.nUserState)
		return;

	if (m_vecFriendReq.size() > 0)
	{
		if (IsOpenAddFriendNot())
		{
			if (false == g_pkUIScene->m_pkNateOnAddFriend->IsCreate())
			{
				KEMS_S2_ADD_FRIEND_NOT pkAddFriendNot = FirstPopFriendReq();

				g_pkUIScene->m_pkNateOnAddFriend->SetRequesterUID(pkAddFriendNot.m_dwSenderUID);
				if (g_pkUIScene->IsRenderOn(KGCUIScene::GC_MBOX_DEPOT)) {
					g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NATEON_ADD_BUDDY, pkAddFriendNot.m_strSenderCharNickName, pkAddFriendNot.m_strMessage
						, KGCUIScene::GC_MBOX_USE_NATEON_ADD_BUDDY_ALRAM, 0, 0, true);
					return;
				}
				std::wstring strSenderName = pkAddFriendNot.m_strSenderCharNickName;
				strSenderName += L"[";
				strSenderName += L"#cffff00";
				strSenderName += pkAddFriendNot.m_strSenderNick;
				strSenderName += L"#cX";
				strSenderName += L"]";

				g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NATEON_ADD_BUDDY, strSenderName, pkAddFriendNot.m_strMessage
					, KGCUIScene::GC_MBOX_USE_NATEON_ADD_BUDDY_ALRAM, 0, 0, false, false);
			}
		}
	}
}

KEMS_S2_ADD_FRIEND_NOT KFriendShip::FirstPopFriendReq(void)
{
	KEMS_S2_ADD_FRIEND_NOT temp;

	if ((int)m_vecFriendReq.size() > 0)
	{
		temp = m_vecFriendReq[0];
		m_vecFriendReq.erase(m_vecFriendReq.begin());
	}

	return temp;
}

int KFriendShip::GetDataUserCount(GCNateOn::E_NATEON_MODE eMode, bool bOnline /*= false*/)
{
	if (m_mapFriendShipData.end() == m_mapFriendShipData.find(eMode))
		return 0;

	int iCnt = 0;
	for (int i = 0; i < (int)m_mapFriendShipData[eMode].size(); ++i)
	{
		if (m_mapFriendShipData[eMode][i].m_bUser)
		{
			if (bOnline)
			{
				if (m_mapFriendShipData[eMode][i].m_bOnline)
					++iCnt;
			}
			else
			{
				++iCnt;
			}
		}
	}

	return iCnt;
}

bool KFriendShip::FindUserInfo(const DWORD dwUserUID, FRIEND_UI_DATA& FUData)
{
	std::map< FS_GROUPID, KBuddyGroup >::iterator itBG = m_mapBuddyGroup.begin();
	for (; m_mapBuddyGroup.end() != itBG; ++itBG)
	{
		std::vector< FRIEND_UI_DATA > vFriendData;
		itBG->second.GetFriendShipData(vFriendData);

		for (int i = 0; i < (int)vFriendData.size(); ++i)
		{
			if (dwUserUID == vFriendData[i].m_dwUserUID)
			{
				FUData = vFriendData[i];
				return true;
			}
		}
	}

	return false;
}

int KFriendShip::GetFriendBuddyGroupID(const DWORD dwUserUID)
{
	std::map< FS_GROUPID, KBuddyGroup >::iterator itBG = m_mapBuddyGroup.begin();
	for (; m_mapBuddyGroup.end() != itBG; ++itBG)
	{
		KBuddy Buddy;
		if (itBG->second.GetBuddy(dwUserUID, Buddy))
		{
			return itBG->first;
		}
	}

	return -1;
}

bool KFriendShip::GetFriendBuddyByNickName(const std::wstring strNickName, KBuddy& FData)
{
	std::map< FS_GROUPID, KBuddyGroup >::iterator itBG = m_mapBuddyGroup.begin();
	for (; m_mapBuddyGroup.end() != itBG; ++itBG)
	{
		if (itBG->second.GetBuddyByNickName(strNickName, FData))
		{
			return true;
		}
	}

	return false;
}

bool KFriendShip::GetFriendBuddyByUID(const DWORD dwUserUID, KBuddy& FData)
{
	std::map< FS_GROUPID, KBuddyGroup >::iterator itBG = m_mapBuddyGroup.begin();
	for (; m_mapBuddyGroup.end() != itBG; ++itBG)
	{
		if (itBG->second.GetBuddy(dwUserUID, FData))
		{
			return true;
		}
	}

	return false;
}

bool KFriendShip::GetBlockBuddy(const DWORD dwUserUID, KBuddy& FData)
{
	return m_kBlockGroup.GetBuddy(dwUserUID, FData);
}

bool KFriendShip::GetBlockBuddy(const std::wstring wstrNickName, KBuddy& FData)
{
	return m_kBlockGroup.GetBuddy(wstrNickName, FData);
}

bool KFriendShip::GetGuildBuddy(const DWORD dwUserUID, KBuddy& FData)
{
	return m_kGuildGroup.GetBuddy(dwUserUID, FData);
}

void KFriendShip::ClearFriendShipData()
{
	m_mapBuddyGroup.clear();
	m_vecFriendReq.clear();
	m_mapFriendShipData.clear();
	m_kBlockGroup.InitData();
	m_kGuildGroup.InitData();
}

bool KFriendShip::IsEmptyFriendShipData()
{
	if (m_mapBuddyGroup.empty() && m_vecFriendReq.empty() && m_mapFriendShipData.empty())
		return true;
	return false;
}

void KFriendShip::SetRecommender(KRecommenderInfo kMyInfo, std::vector< KRecommenderInfo> vecRecommenders)
{
	m_myInfo = kMyInfo;
	m_vecRecommenders = vecRecommenders;

	MakeContactFriendList();
}

void KFriendShip::SetRecommender(std::vector< KRecommenderInfo> vecRecommenders)
{
	m_vecRecommenders = vecRecommenders;

	MakeContactFriendList();
}

void KFriendShip::MakeContactFriendList()
{
	m_vecContactFriendList.clear();
	m_vecWaitRecommenders.clear();
	m_iAttendedRecommender = 0;

	time_t tCurTime;
	time(&tCurTime);

	VEC_FRIEND_UI_DATA& vecFriendData = m_mapFriendShipData[GCNateOn::FRIEND_MODE];
	VEC_FRIEND_UI_DATA& vecBlockData = m_mapFriendShipData[GCNateOn::BLOCK_MODE];

	if ((MRS_ACCEPTED == m_myInfo.m_nState || MRS_ATTENDANCE_FINISH == m_myInfo.m_nState) && tCurTime < m_myInfo.m_tmEndDate)
	{
		m_vecContactFriendList.push_back(m_myInfo.m_dwLoginUID);

		FindAndMakeFriend(vecFriendData, vecBlockData, m_myInfo); // 연계榮쨉?친구 아니면 자동 추가
	}

	std::vector< KRecommenderInfo>::iterator itor;

	for (itor = m_vecRecommenders.begin(); itor != m_vecRecommenders.end(); ++itor)
	{
		if (ORS_ACCEPTED == itor->m_nState && tCurTime < itor->m_tmEndDate)    // 오늘 날짜가 이벤트 끝나는 날보다 앞이어야함
		{
			m_vecContactFriendList.push_back(itor->m_dwLoginUID);
			FindAndMakeFriend(vecFriendData, vecBlockData, *itor);
		}
		if ((ORS_ATTENDANCE_FINISH == itor->m_nState || ORS_ITEM_RECEIVED == itor->m_nState) && tCurTime < itor->m_tmEndDate)
		{
			m_vecContactFriendList.push_back(itor->m_dwLoginUID);
			m_iAttendedRecommender++;
			FindAndMakeFriend(vecFriendData, vecBlockData, *itor);
		}
		else if (itor->m_nState == ORS_WAIT_ACCEPT)
			m_vecWaitRecommenders.push_back(*itor);

#if defined(WELCOMEBACK_FRIEND)
		if (ORS_WAIT_ACCEPT == itor->m_nState)
		{
			g_pkUIScene->m_pkNateOn->SetNew(true);
			g_pkUIScene->m_pkRecommendList->SetNew(true);
		}
#endif
	}

#if defined(WELCOMEBACK_FRIEND)
	KP2P::GetInstance()->Send_UserRecomTypeReq();
	if (m_vecWaitRecommenders.size() == 0)
	{
		g_pkUIScene->m_pkNateOn->SetNew(false);
		g_pkUIScene->m_pkRecommendList->SetNew(false);
	}
#endif
}

bool KFriendShip::FindUIDInContactFriendList(DWORD dwUID_)
{
	return (m_vecContactFriendList.end() != std::find(m_vecContactFriendList.begin(), m_vecContactFriendList.end(), dwUID_));
}

void KFriendShip::FindAndMakeFriend(VEC_FRIEND_UI_DATA& vecFriendData, VEC_FRIEND_UI_DATA& vecBlockData, KRecommenderInfo kFriendInfo)
{
	if ((vecFriendData.end() == std::find(vecFriendData.begin(), vecFriendData.end(), kFriendInfo)) ||
		(vecBlockData.end() != std::find(vecBlockData.begin(), vecBlockData.end(), kFriendInfo)))   // 연계榮쨉?친구 아니면 자동 추가
	{
		if (KP2P::GetInstance()->m_kMsgServer != NULL)
		{
			KP2P::GetInstance()->m_kMsgServer->Send_ForceMakeFriend(g_kGlobalValue.m_kUserInfo.dwUID, g_kGlobalValue.m_kUserInfo.strNickName,
				kFriendInfo.m_dwLoginUID, kFriendInfo.m_strNick);
		}
	}
}

int KFriendShip::GetMyRecommendState()
{
	if (m_myInfo.m_nState >= KFriendShip::MRS_ACCEPTED)    // 추천인 있음
		return 1;
	// 원래는 추천인 있으면 1 없으면 0 해서 리턴값 받아서 - 하는데다 썼는데 승인대기중일때도 보내야된데서 2 리턴하는거 만들면서 망했음.
	// 따로 빼려면 빼삼 난 얼른 고쳐달래서 시간이 없었음..
	else if (m_myInfo.m_nState == KFriendShip::MRS_WAIT_ACCEPT)
		return 2;
	else
		return 0;
}

void KFriendShip::GetOnlienFriendList(GCNateOn::E_NATEON_MODE _eMode, OUT std::vector< DWORD>& _vecOnlineFirendList)
{
	std::map< GCNateOn::E_NATEON_MODE, VEC_FRIEND_UI_DATA >::iterator mit = m_mapFriendShipData.find(_eMode);

	if (m_mapFriendShipData.end() == mit)
		return;

	VEC_FRIEND_UI_DATA::iterator itor;
	for (itor = mit->second.begin(); itor != mit->second.end(); ++itor)
	{
		if ((*itor).m_bUser && (*itor).m_bOnline)
		{
			_vecOnlineFirendList.push_back((*itor).m_dwUserUID);
		}
	}
}

void KFriendShip::GetOffLineFriendList(GCNateOn::E_NATEON_MODE _eMode, OUT std::vector< DWORD>& _vecFirendList)
{
	std::map< GCNateOn::E_NATEON_MODE, VEC_FRIEND_UI_DATA >::iterator mit = m_mapFriendShipData.find(_eMode);

	if (m_mapFriendShipData.end() == mit)
		return;

	VEC_FRIEND_UI_DATA::iterator itor;
	for (itor = mit->second.begin(); itor != mit->second.end(); ++itor)
	{
		if ((*itor).m_bUser && (*itor).m_bOnline == false)
		{
			_vecFirendList.push_back((*itor).m_dwUserUID);
		}
	}
}

int KFriendShip::GetOnlineGuildMember()
{
	return m_kGuildGroup.GetOnlineBuddyCount();
}

int KFriendShip::GetTotalGuildMember()
{
	return m_kGuildGroup.GetTotalBuddyCount();
}

void KFriendShip::ClearGuildMemberData()
{
	std::map< GCNateOn::E_NATEON_MODE, VEC_FRIEND_UI_DATA >::iterator mit = m_mapFriendShipData.find(GCNateOn::GUILD_MODE);

	if (mit != m_mapFriendShipData.end())
	{
		mit->second.clear();
	}
}

void KFriendShip::ModifyGuildMemberData(std::map<int, KGroup>& mapFriendGroup_, int iGroupType_, bool bAdd_)
{
	VEC_FRIEND_UI_DATA vecGuildAdd;

	std::map<int, KGroup>::iterator mitGuild = mapFriendGroup_.find(iGroupType_);
	if (mitGuild != mapFriendGroup_.end())
	{
		std::map<DWORD, KFriend>::iterator mitFriend;

		for (mitFriend = mitGuild->second.m_mapMembers.begin(); mitFriend != mitGuild->second.m_mapMembers.end(); ++mitFriend)
		{
			if (bAdd_)
				m_kGuildGroup.AddBuddy(mitFriend->second);
			else
				m_kGuildGroup.DeleteBuddy(mitFriend->second.m_dwUserUID);
		}
	}

	UpdateFriendShipData();
}

void KFriendShip::SetAllGuildMemberData(const KGroup& kPacket)
{
	m_kGuildGroup.SetData(kPacket);
}

bool KFriendShip::IsBlockFriend(const std::wstring& strNickName_)
{
	return m_kBlockGroup.IsInNickName(strNickName_);
}

void KFriendShip::GetFriendListData(OUT std::vector< KFriendListData >& vecRet)
{
	std::vector< KBuddyGroup > vecBuddyGroup;
	vecBuddyGroup.clear();
	GetFriendGroupVector(vecBuddyGroup);

	std::vector< KBuddyGroup >::iterator vit = vecBuddyGroup.begin();
	for (; vit != vecBuddyGroup.end(); ++vit)
	{
		vit->GetFriendListData(vecRet);
	}
}