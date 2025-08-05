#include "StdAfx.h"
#include "KGCContributionManager.h"

ImplementSingleton(KGCContributionManager);

KGCContributionManager::KGCContributionManager(void)
	: m_dwDonationItemID(0)
	, m_dwDonationUnit(0)
	, m_dwClientUIType(0)
	, m_nUserAmount(0)
	, m_nTotalAmount(0)
	, m_nCurrentNPCType(0)
{
	m_vecDonationRewards.clear();
	m_vecGoalInfo.clear();
}

KGCContributionManager::~KGCContributionManager(void)
{
}

//void KGCContributionManager::LoadContributionEvent()
//{
//    std::vector< EventBYContributedGP* >::iterator itor;
//    for( itor =  m_vecContributionEvent.begin(); itor != m_vecContributionEvent.end(); ++itor)
//    {
//        SAFE_DELETE( *itor );
//    }
//    m_vecContributionEvent.clear();
//
//    KLuaManager     kLuaMgr;
//
//    if( GCFUNC::LoadLuaScript( kLuaMgr, "ContributionEvent.stg" ) == false )
//        return;
//
//    kLuaMgr.BeginTable( "EventList" );
//    {
//        for(int i=1 ; ; ++i )
//        {
//            if( kLuaMgr.BeginTable( i ) == E_FAIL )
//                break;
//
//			EventBYContributedGP*	pContributionEvent = new EventBYContributedGP;
//
//            int iEventState = CES_NOTYET;
//            kLuaMgr.GetValue( "EventState", iEventState)   ;
//            pContributionEvent->EventState = (CONTRIBUTION_EVENT_STATE)iEventState;
//
//            DWORD dwTemp;
//            kLuaMgr.GetValue( "GoalGP",	dwTemp );
//
//#ifdef NATION_BRAZIL
//            pContributionEvent->dwGoalGP = ( (__int64)i ) * 1000000000 ;
//#else
//            pContributionEvent->dwGoalGP = dwTemp;
//#endif
//
//            int iEventName = 0;
//            kLuaMgr.GetValue( "EventName", iEventName);
//            pContributionEvent->strEventName = g_pkStrLoader->GetReplacedString(iEventName, "l", ConvertNumToCommaString(pContributionEvent->dwGoalGP) );
//
//            int iEventDesc = 0;
//            kLuaMgr.GetValue( "EventDesc", iEventDesc);
//            pContributionEvent->strEventDesc = g_pkStrLoader->GetString(iEventDesc);
//
//            m_vecContributionEvent.push_back( pContributionEvent );
//
//            kLuaMgr.EndTable();
//        }
//    }
//    kLuaMgr.EndTable();
//
//    std::sort(m_vecContributionEvent.begin(), m_vecContributionEvent.end(), KContributionEventSortByGP() );
//}
//
//bool KGCContributionManager::IsExistRanker(int iIndex)
//{
//    if(iIndex >= (int)m_vecContributionRanker.size())
//        return false;
//
//    return !m_vecContributionRanker[iIndex].m_strNickName.empty();
//}
//
//bool KGCContributionManager::IsExistEvent(int iIndex)
//{
//    if(iIndex >= (int)m_vecContributionEvent.size())
//        return false;
//
//    return !m_vecContributionEvent[iIndex]->strEventName.empty();
//}
//
//bool KGCContributionManager::IsCompleteContributionEvent(int iIndex)
//{
//    if(iIndex >= (int)m_vecContributionEvent.size())
//        return false;
//
//    return CES_ACTIVE == m_vecContributionEvent[iIndex]->EventState;
//}
//
//bool KGCContributionManager::IsCompleteContributionEventLocker(int iIndex)
//{
//    if(iIndex >= (int)m_vecContributionEvent.size())
//        return false;
//
//    return CES_GETITEM == m_vecContributionEvent[iIndex]->EventState;
//}
//
//void KGCContributionManager::SetContributionEventState()
//{
//    std::vector< EventBYContributedGP* >::iterator itor;
//
//    for(itor = m_vecContributionEvent.begin(); itor != m_vecContributionEvent.end(); ++itor)
//    {
//        if( (*itor)->dwGoalGP <= m_dwTotalContributionGP )
//        {
//            (*itor)->EventState = CES_ACTIVE;
//        }
//    }
//}
//
//void KGCContributionManager::SetContributionEventState(std::vector<KItemDonationInfo> m_Info)
//{
//    std::vector<KItemDonationInfo>::iterator itor;
//
//    for(itor= m_Info.begin(); itor != m_Info.end(); ++itor)
//    {
//        if(itor->m_dwGradeState > 0)
//           m_vecContributionEvent[itor->m_dwGrade - 1]->EventState = CES_ACTIVE;
//    }
//}
//
//EventBYContributedGP* KGCContributionManager::GetContributionEvent(int iIndex )
//{
//    if(iIndex >= (int)m_vecContributionEvent.size())
//        iIndex = NULL;
//
//    return m_vecContributionEvent[iIndex];
//}
//
//KDotantionRank* KGCContributionManager::GetContributionRanker(int iIndex)
//{
//    if(iIndex >= (int)m_vecContributionRanker.size())
//        return NULL;
//
//    return &m_vecContributionRanker[iIndex];
//}
//
std::wstring KGCContributionManager::ConvertNumToCommaString(__int64 dwNumber)
{
	WCHAR strTemp[MAX_PATH] = { 0, };
	std::wstring strNumber = _i64tow(dwNumber, strTemp, 10);
	std::wstring strResult = L"";

	int iLength = strNumber.length();
	int iCommaPoint = (iLength - 1) % 3 + 1;
	int iCommaCount = (iLength - 1) / 3;

	for (int i = 0; i < iLength; ++i)
	{
		for (int j = 0; j < iCommaCount; ++j)
		{
			if (i == iCommaPoint + (j * 3))
				strResult += L',';
		}
		strResult += strNumber[i];
	}

	return strResult;
}

std::string KGCContributionManager::ConvertNumToCommaUIString(__int64 dwNumber)
{
	char strTemp[MAX_PATH] = { 0, };
	std::string strNumber = _i64toa(dwNumber, strTemp, 10);
	std::string strResult = "";

	int iLength = strNumber.length();
	int iCommaPoint = (iLength - 1) % 3 + 1;
	int iCommaCount = (iLength - 1) / 3;

	for (int i = 0; i < iLength; ++i)
	{
		for (int j = 0; j < iCommaCount; ++j)
		{
			if (i == iCommaPoint + (j * 3))
				strResult += '0' + 11;
		}
		strResult += strNumber[i];
	}

	return strResult;
}

//void KGCContributionManager::SetContributionLocker(std::set<int> setSuccessGrade)
//{
//    std::set<int>::iterator Sit = setSuccessGrade.begin();
//    for(; Sit != setSuccessGrade.end(); ++Sit)
//    {
//        m_vecContributionEvent[*Sit - 1]->EventState = CES_GETITEM;
//    }
//
//}
//
//void KGCContributionManager::SetNewYearContributionInfo( std::map< DWORD, std::vector< std::pair<DWORD, int> > > pNewYearContributionInfo )
//{
//	m_pkNewYearContributionGoal = pNewYearContributionInfo;
//	LoadNewYearStringScript();
//	SetNewYearString();
//}
//
//void KGCContributionManager::LoadNewYearStringScript()
//{
//	KLuaManager     kLuaMgr;
//
//	if( GCFUNC::LoadLuaScript( kLuaMgr, "NewYearContributionEvent.stg" ) == false )
//		return;
//
//	kLuaMgr.BeginTable( "EventList" );
//	{
//		for(int i=1 ; ; ++i )
//		{
//			if( kLuaMgr.BeginTable( i ) == E_FAIL )
//				break;
//			int iEventValue = 0;
//			kLuaMgr.GetValue( "EventValue", iEventValue);
//			m_EventDescValue.push_back(iEventValue);
//
//			int iEventDesc = 0;
//			kLuaMgr.GetValue( "EventDesc", iEventDesc);
//			m_EventDescID.push_back(iEventDesc);
//			kLuaMgr.EndTable();
//		}
//	}
//	kLuaMgr.EndTable();
//}
//
//void KGCContributionManager::SetNewYearString()GET
//{
//    for( int i = 0 ; i < MAX_CONTRIBUTION_EVENT; ++i )
//    {
//        for( int j = 0; j < MAX_CONTRIBUTION_EVENT; ++j )
//        {
//            m_EventNamestring[i][j] = L"";
//            m_EventDescstring[i][j] = L"";
//        }
//    }
//
//    std::map< DWORD, std::vector< std::pair<DWORD, int> > >::iterator mit = m_pkNewYearContributionGoal.begin();
//
//    int iNpcType = 0;
//    for( ; mit != m_pkNewYearContributionGoal.end(); ++mit, ++iNpcType )
//    {
//        if( iNpcType >= MAX_CONTRIBUTION_EVENT )
//            break;
//
//        for( std::vector< std::pair<DWORD, int> >::iterator vit = mit->second.begin(); vit != mit->second.end(); ++vit )
//        {
//            int iStrOrder = vit - mit->second.begin();
//            if( iStrOrder >= MAX_CONTRIBUTION_EVENT )
//                break;
//
//            m_EventNamestring[iNpcType][iStrOrder] = g_pkStrLoader->GetReplacedString(STR_ID_DURATION_COUNT,"i", (*vit).second );
//
//
//            if( iNpcType < static_cast<int>( m_EventDescID.size() ) )
//                m_EventDescstring[iNpcType][iStrOrder] = g_pkStrLoader->GetReplacedString(m_EventDescID[iNpcType], "i", m_EventDescValue[iNpcType] * (iStrOrder+1) );
//            else
//                m_EventDescstring[iNpcType][iStrOrder] = L"";
//        }
//    }
//}
//
//void KGCContributionManager::SetNewContributionEvent(DWORD dwType)
//{
//	std::vector< std::pair<DWORD, int> >::iterator vit = m_pkNewYearContributionGoal[dwType].begin();
//	for(; vit != m_pkNewYearContributionGoal[dwType].end(); ++vit)
//	{
//		if( vit->second <=m_dwNewYearTotalContributionGP[dwType-1])
//		{
//			m_EventState[dwType-1][vit->first - 1] = CES_ACTIVE;
//		}
//		else
//		{
//			m_EventState[dwType-1][vit->first - 1] = CES_NOTYET;
//		}
//	}
//}

void KGCContributionManager::OnEventTongDonationInfoAck(IN KEVENT_TONG_DONATION_INFO_ACK& kRecv_)
{
	m_dwDonationItemID = kRecv_.m_dwDonationItemID;
	m_dwDonationUnit = kRecv_.m_dwDonationUnit;
	m_vecDonationRewards = kRecv_.m_vecDonationRewards;
	m_dwClientUIType = kRecv_.m_dwClientUIType;
	m_vecGoalInfo = kRecv_.m_vecGoalInfo;
	m_nUserAmount = kRecv_.m_nUserAmount;
	m_nTotalAmount = kRecv_.m_nTotalAmount;
}

void KGCContributionManager::OnEventTongDonationDonateAck(IN KEVENT_TONG_DONATION_DONATE_ACK& kRecv_)
{
	// 기부에 사용한 아이템 갱신
	KItem* pkInvenItem = NULL;
	g_pItemMgr->FindInventory(kRecv_.m_kDonationItem.m_ItemUID, &pkInvenItem);

	if (pkInvenItem) {
		pkInvenItem->m_nCount = kRecv_.m_kDonationItem.m_nCount;
	}

	if (pkInvenItem->m_nCount <= 0) {
		g_pItemMgr->m_kInventory.RemoveItem(pkInvenItem->m_ItemUID);
		g_pItemMgr->RemoveInventoryItemList(pkInvenItem->m_ItemUID);
	}

	// 교환된 아이템 처리
	g_pItemMgr->m_kInventory.AddItemList(kRecv_.m_vecDonationRewards, true);

	for (std::vector<KItem>::iterator it = kRecv_.m_vecDonationRewards.begin(); it != kRecv_.m_vecDonationRewards.end(); ++it) {
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_REPETITION_EVENT_BOX,
			L"", g_pkStrLoader->GetString(STR_ID_MISSION_DLG3),
			KGCUIScene::GC_MBOX_USE_NORMAL, (*it).m_ItemID * 10, (*it).m_ItemUID, false, true);
	}

	// 인벤토리 갱신
	if (g_MyD3D->m_pStateMachine->IsStateMyInfo())
		g_pkUIScene->m_pkMyInfoScene->UpdateGameData();

	// UI 갱신 코드는 아래에
	//
}

int KGCContributionManager::GetDonationMyItemCount()
{
	KItem* pInventoryItem = NULL;

	if (g_pItemMgr->FindInventoryForItemID(m_dwDonationItemID / 10, &pInventoryItem)) {
		return pInventoryItem->m_nCount;
	}
	else
		return 0;
}

void KGCContributionManager::Donating(int nCount)
{
	if (nCount <= 0)
		return;

	// 최소 기부단위를 충족하지 못하면 안됨
	if ((nCount / m_dwDonationUnit) == 0)
		return;

	// 기부
	KEVENT_TONG_DONATION_DONATE_REQ kPacket;

	kPacket.m_dwDonateCount = nCount - (nCount % m_dwDonationUnit);
	kPacket.m_dwNPCType = m_nCurrentNPCType;

	KP2P::GetInstance()->Send_TongDonationDonateReq(kPacket);
}