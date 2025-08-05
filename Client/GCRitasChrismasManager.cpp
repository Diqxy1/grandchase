#include "GCRitasChrismasManager.h"
#include "GCUI/GCMyInfoScene.h"

ImplementSingleton( GCRitasChrismasManager )

GCRitasChrismasManager::GCRitasChrismasManager(void):
m_bGetInfo(false)
{

    m_mapObjectInfo.clear();
    m_mapStageInfo.clear();
    m_vecRankInfo.clear();

	m_nStartCoinCount = 0;
	m_nContinueCoinCount = 0;
	m_nPremiumRewardCount = 0;
	m_nMaxPoint = 0;
	InitTmepInfo();

    m_vInitPlayerPos = D3DXVECTOR2(40.0f,349.0f);

	m_vecRewardItemID.clear();

}

GCRitasChrismasManager::~GCRitasChrismasManager(void)
{

}

void GCRitasChrismasManager::SetBaseInfo( const KEVENT_RITAS_CHRISTMAS_USER_INFO_ACK& kRecv_ )
{
    switch(kRecv_.m_nOK)
    {
    case 0:
        m_nStartCoinCount = kRecv_.m_nStartCoinCount;
        m_nContinueCoinCount = kRecv_.m_nContinueCoinCount;
        m_nPremiumRewardCount = kRecv_.m_nPremiumRewardCount;
        m_nMaxPoint = kRecv_.m_nMaxPoint;
        break;

    default:
        m_nStartCoinCount = 0;
        m_nContinueCoinCount = 0;
        m_nPremiumRewardCount = 0;
        m_nMaxPoint = 0;

        std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", NULL, kRecv_.m_nOK );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_UNKWON_ERROR), strTemp,
            KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true );
        break;
    }
}

void GCRitasChrismasManager::SetPlayInfo( const KEVENT_RITAS_CHRISTMAS_PLAY_INFO_ACK& kRecv_ )
{
    switch(kRecv_.m_nOK)
    {
    case 0:
        m_kPlayInfo = kRecv_.m_kPlayInfo;
        m_mapObjectInfo = kRecv_.m_mapObjectInfo;
        m_mapStageInfo = kRecv_.m_mapStageInfo;
        m_bGetInfo = true;
        break;

    default:
        std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", NULL, kRecv_.m_nOK );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_UNKWON_ERROR), strTemp,
            KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true );
        break;
    }
}

void GCRitasChrismasManager::SetRankInfo( const KEVENT_RITAS_CHRISTMAS_RANK_INFO_ACK& kRecv_ )
{
    switch(kRecv_.m_nOK)
    {
    case 0:
        m_vecRankInfo = kRecv_.m_vecRankInfo;

        ///테스트 코드//
// 		for (int i=0; i <15 ; i++)
// 		{
// 			char szName[MAX_PATH];
// 			sprintf( szName, "aaa%d", i );
// 			KRitasChristmasRankInfo tmpData;
// 			tmpData.m_nMaxPoint = 1000+ ::rand()%1000;
// 			tmpData.m_wsNickName =  GCUTIL_STR::GCStrCharToWide(szName);
// 			m_vecRankInfo.push_back(tmpData);
// 		}
        FindMyRank();
        break;

    default:
        std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", NULL, kRecv_.m_nOK );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_UNKWON_ERROR), strTemp,
            KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true );
        break;
    }
}

void GCRitasChrismasManager::FindMyRank()
{
    std::vector< KRitasChristmasRankInfo >::iterator vit = m_vecRankInfo.begin();
    for (int i=1; vit != m_vecRankInfo.end() ; ++vit , i++)
    {
        if ( vit->m_wsNickName == g_kGlobalValue.m_kUserInfo.strNickName)
        {
            m_nMyRank = i;
            return;
        }
    }

    m_nMyRank = -1;
}

void GCRitasChrismasManager::SetStageStartInfo( const KEVENT_RITAS_CHRISTMAS_STAGE_START_ACK& kRecv_ )
{
    // m_nCurrentPoint; // 현재 점수
    // kPacket.m_nOK : 1 // 리타의 크리스마스 이벤트 기간이 아님.
    // kPacket.m_nOK : 15 // 바구니에 담긴 오브젝트 수가 최대 바구니 크기보다 큼.
    // kPacket.m_nOK : 16 // 오브젝트 정보가 없음.

    switch(kRecv_.m_nOK)
    {
    case 0:
        m_nStartCoinCount = kRecv_.m_nStartCoinCount;
        m_nCurrentPoint = kRecv_.m_nCurrentPoint;
        m_nCurrentStageNum = kRecv_.m_nCurrentStageNum;
        break;

    default:
        std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", NULL, kRecv_.m_nOK );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_UNKWON_ERROR), strTemp,
            KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true );
        break;
    }
}

void GCRitasChrismasManager::SetEmptyBasket( const KEVENT_RITAS_CHRISTMAS_EMPTY_BASKET_ACK& kRecv_ )
{
    switch(kRecv_.m_nOK)
    {
    case 0:
        m_nCurrentPoint = kRecv_.m_nCurrentPoint;
        break;

    default:
        std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", NULL, kRecv_.m_nOK );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_UNKWON_ERROR), strTemp,
            KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true );
        break;
    }
}

void GCRitasChrismasManager::GenerateCombo(int iStageNum_)
{
    if (!m_bGetInfo)
        return;

    if (m_mapObjectInfo.empty())
        return;

    m_vecCombo.clear();

	int nRandValue = m_kPlayInfo.m_nBasketSize-1;
	std::map< int, KRitasChristmasStageInfo >::iterator mitStage =  m_mapStageInfo.find(iStageNum_);
	if (mitStage != m_mapStageInfo.end())
	{
		nRandValue = mitStage->second.m_nComboLimit -1;
	}
    
    std::map< int, KRitasChristmasObjectInfo >::iterator mit = m_mapObjectInfo.begin();
    int i=0;
    while(mit != m_mapObjectInfo.end() && i<Max_COMBO_INFO)
    {
        if (mit->second.m_nObjectType == KRitasChristmasObjectInfo::OT_FEVER)
            continue;

		

		RitasComboInfo kTmpInfo;
		kTmpInfo.m_iObjID = mit->second.m_nObjectID;
		//kTmpInfo.m_iNum = 1;
		kTmpInfo.m_iNum = ::rand()%nRandValue+1;
		kTmpInfo.m_bClear = false;

        m_vecCombo.push_back(kTmpInfo);
        ++mit;
        i++;
    }

}

void GCRitasChrismasManager::UpdateComboInfo(std::vector< int >& vecBasket_ )
{
    if (m_vecCombo.empty())
        return;
    
    std::vector<RitasComboInfo>::iterator vitComboInfo = m_vecCombo.begin();

    while( vitComboInfo != m_vecCombo.end() )
    {
        int iComboCnt = 0;

        std::vector< int >::iterator vit = vecBasket_.begin();
        while( vit != vecBasket_.end() )
        {
            if ( (*vit) == vitComboInfo->m_iObjID )
                iComboCnt++;
            else
                iComboCnt = 0;

            if (iComboCnt == vitComboInfo->m_iNum)
            {
                vitComboInfo->m_bClear = true;
                break;
            }
            ++vit;
        }
        ++vitComboInfo;
    }
}

bool GCRitasChrismasManager::IsClearedAllCombo()
{
    if (m_vecCombo.empty())
        return false;

    std::vector<RitasComboInfo>::iterator vit = m_vecCombo.begin();
    while(vit != m_vecCombo.end())
    {
        if (vit->m_bClear == false)
            return false;
        ++vit;
    }

    return true;

}

void GCRitasChrismasManager::SetStageClearInfo( const KEVENT_RITAS_CHRISTMAS_STAGE_END_ACK& kRecv_ )
{
    // m_nCurrentPoint; // 현재 점수
    // m_nStartCoinCount; // 시작 코인 수
    // m_nContinueCoinCount; // 재도전 코인 수
    // m_bContinuable; // 재도전 가능 여부
    // m_nSpecialRewardCount; // 스페셜 상자 수
    // m_nPremiumRewardCount; // 프리미엄 상자 수
    // m_nMaxPoint; // 내 최고 점수
    // m_bGotNewRecord; // 기록을 갱신했는지 여부
    // m_bClearLastStage; // 마지막 스테이지를 클리어했는지 여부

    // kPacket.m_nOK : 1 // 리타의 크리스마스 이벤트 기간이 아님.
    // kPacket.m_nOK : 9 // 스테이지 정보가 없음.
    // kPacket.m_nOK : 17 // 유효하지 않은 종료타입.

    switch(kRecv_.m_nOK)
    {
    case 0:
        m_nCurrentPoint = kRecv_.m_nCurrentPoint;
        m_nStartCoinCount = kRecv_.m_nStartCoinCount;
        m_nContinueCoinCount = kRecv_.m_nContinueCoinCount;
        m_nSpecialRewardCount = kRecv_.m_nSpecialRewardCount;
        m_nPremiumRewardCount = kRecv_.m_nPremiumRewardCount;
        m_nMaxPoint = kRecv_.m_nMaxPoint;
        m_bContinuable	=	kRecv_.m_bContinuable;
        m_bClearLastStage = kRecv_.m_bClearLastStage;

        break;

    default:
        std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", NULL, kRecv_.m_nOK );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_UNKWON_ERROR), strTemp,
            KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true );
        break;
    }
}

void GCRitasChrismasManager::InitTmepInfo()
{
    m_vecCombo.clear();

    m_nCurrentPoint = 0;
    m_nCurrentStageNum = 0;

    m_bContinuable = false;
    m_bClearLastStage = false;

    m_nMyRank = -1;
}

void GCRitasChrismasManager::SetContinueCoin( const KEVENT_RITAS_CHRISTMAS_EXCHANGE_CONTINUE_COIN_ACK& kRecv_ )
{
    // m_nAfterContinueCoinCount : 아이템 사용 후 충전 된 재도전 코인 수(누적)
    // m_kConsumeItem : 사용한 교환 아이템 정보(수량감소)
    // kRecv.m_nOK :
    // 0 : 성공.
    // 1 : 리타의 크리스마스 이벤트 기간이 아님.
    // 19 : 도전 코인 교환 아이템이 없음.
    // 20 : 재도전 코인 교환 아이템의 수량이 부족함(DB).
    // 21 : 재도전 코인 교환 중 재도전 코인(포인트) DB 갱신 실패.
    // 22 : 재도전 코인 교환 중 교환 아이템의 수량 DB 갱신 실패.
    // -98 : 아직 처리중인 작업입니다.

    switch( kRecv_.m_nOK ) 
    {
    case 0:
        g_pItemMgr->m_kInventory.AddItem( kRecv_.m_kConsumeItem, true );

        if ( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO ||
            g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM ) {
                g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
                g_pkUIScene->m_pkMyInfoScene->UpdateData();
        }

        m_nContinueCoinCount = kRecv_.m_nAfterContinueCoinCount;

        // 여기에 재도전 코인이 충전되었습니다 라는 메시지 박스 출력이 필요할 듯

        break;
    default:
        std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", NULL, kRecv_.m_nOK );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_UNKWON_ERROR), strTemp,
            KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true );
        break;
    }
}

void GCRitasChrismasManager::SetRewardInfo( KEVENT_RITAS_CHRISTMAS_GET_REWARD_ACK& kRecv_ )
{
	switch( kRecv_.m_nOK ) 
	{
	case 0:
		{
			g_pItemMgr->m_kInventory.AddItem(kRecv_.m_kPremiumKey,true);

			std::vector< KItem >::iterator vit = kRecv_.m_vecGetItems.begin();
			while( vit != kRecv_.m_vecGetItems.end() )
			{
				GCItem* pItem = g_pItemMgr->GetItemData( vit->m_ItemID / 10 );
				if( NULL == pItem ) { 
					continue;
				}
				g_pItemMgr->m_kInventory.AddItem( *vit, true );
				std::wstring strMsg = g_pkStrLoader->GetReplacedString( STR_ID_ITEM_RECEIVE1, "l", pItem->strItemName );
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPETITION_EVENT_BOX, strMsg, L"", KGCUIScene::GC_MBOX_USE_NORMAL, pItem->dwGoodsID * 10, vit->m_ItemUID, true, true);
				++vit;
			}

			//g_pItemMgr->m_kInventory.AddItemList( kRecv_.m_vecGetItems, true );
			
			m_nPremiumRewardCount = kRecv_.m_nAfterPremiumRewardCount;
			m_nSpecialRewardCount = kRecv_.m_nAfterSpecialRewardCount;
			m_nStartCoinCount = kRecv_.m_nStartCoinCount;
			m_nContinueCoinCount = m_nContinueCoinCount;
		}
		break;
	case  -98:
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PACKET_DELAY ));
		break;
	default:
		std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", NULL, kRecv_.m_nOK );
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_UNKWON_ERROR), strTemp,
			KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true );
		break;
	}

}

void GCRitasChrismasManager::SetCurrentPoint( int iValue_ )
{
	m_nCurrentPoint = iValue_;

}

void GCRitasChrismasManager::SetRewarItemList( const KEVENT_RITAS_CHRISTMAS_REWARD_VIEW_INFO_ACK& kRecv_ )
{
	switch( kRecv_.m_nOK ) 
	{
	case 0:
		m_vecRewardItemID = kRecv_.m_vecRewardViewListInfo;
		break;
	default:

		break;
	}

}
