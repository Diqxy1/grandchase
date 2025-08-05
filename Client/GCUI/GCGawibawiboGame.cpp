#include "stdafx.h"
#include "GCGawibawiboGame.h"

#include "GCGawibawiboGameInfo.h"
#include "GCGawibawiboGameBossInfo.h"
#include "GCGawibawiboGamePlay.h"
#include "GCGawibawiboGameLose.h"

IMPLEMENT_CLASSNAME( KGCGawibawiboGame );
IMPLEMENT_WND_FACTORY( KGCGawibawiboGame );
IMPLEMENT_WND_FACTORY_NAME( KGCGawibawiboGame, "gc_gawibawibo_game_box" );

KGCGawibawiboGame::KGCGawibawiboGame( void )
:	m_pkBtnBuyItem(NULL)
,	m_pkBtnViewItem(NULL)
,	m_iStateValue(0)
,	m_iVer(-1)
,	m_pkGameInfoBox(NULL)
,	m_pkGameBossInfo(NULL)
,	m_iState(0)
,	m_pkGamePlay(0)
,	m_pkBtnClose(NULL)
,	m_pkGameLose(NULL)
,	m_iCurrentPosition(0)
,	m_iMaxBoss(0)
,	m_pkStaticDescription1(NULL)
,	m_pkStaticDescription2(NULL)
,	m_pkStaticDescription3(NULL)
,	m_pkStaticDescription4(NULL)
{
	LINK_CONTROL("btn_view_item",m_pkBtnViewItem);
	LINK_CONTROL("btn_buy_item",m_pkBtnBuyItem);
	LINK_CONTROL("btn_close",m_pkBtnClose);

	LINK_CONTROL( "static_description1",		m_pkStaticDescription1 );	 
	LINK_CONTROL( "static_description2",		m_pkStaticDescription2 );	 
	LINK_CONTROL( "static_description3",		m_pkStaticDescription3 );	 
	LINK_CONTROL( "static_description4",		m_pkStaticDescription4 );	 

	LINK_CONTROL( "gawibawibo_game_info_box",   m_pkGameInfoBox );	
	LINK_CONTROL( "gawibawibo_game_boss_info",  m_pkGameBossInfo );	
	LINK_CONTROL( "gawibawibo_game_play",		m_pkGamePlay );	 
	LINK_CONTROL( "gawibawibo_game_lose_info_box",		m_pkGameLose );	 
	LINK_CONTROL( "gawibawibo_game_view_item",		m_pkGameItemView );	 


}

KGCGawibawiboGame::~KGCGawibawiboGame( void )
{
}
void KGCGawibawiboGame::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		if( event.m_pWnd == m_pkBtnClose)
		{
            Close();
		}
		else if( m_pkBtnBuyItem == event.m_pWnd && m_pkBtnBuyItem )
		{
#if defined( NATION_USA )|| defined( NATION_EU ) 
			SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
#endif
			DWORD dwBuyItemID = KGCItemManager::ITEM_GAWIBAWIBO_PACKEGE;
#if defined( NATION_USA )
            dwBuyItemID = KGCItemManager::ITEM_GAWIBAWIBO_COIN; // 북미는 가위바위보 UI에서 패키지 아이템이 아니라 도전코인을 판다
#endif
			GCItem* pItem = g_pItemMgr->GetItemData( dwBuyItemID );
			if( pItem->eMoneyType == EMT_CASH )
			{
#if defined ( USE_VIP_POINT )
                g_pItemMgr->BuyCashItemS6( dwBuyItemID );
#else
    #ifdef GAME_CASH
                g_pkUIScene->EnableBuyCashItemWnd( dwBuyItemID, false, true );
    #else    
                g_MyD3D->m_kItemMgr.BuyItem( dwBuyItemID, pItem->iItemTypeValue  );
    #endif
#endif
			}
			
		}
		else if( m_pkBtnViewItem == event.m_pWnd )
		{
			m_pkGameItemView->SetBossIndex((m_iCurrentPosition+1)%m_iStateValue);
			m_pkGameItemView->SetView();
			m_pkGameItemView->ToggleRender(true);
		}
	}
}

void KGCGawibawiboGame::OnCreate( void )
{
    if (m_pkBtnBuyItem)
	    m_pkBtnBuyItem->InitState(true,true,this);
	m_pkBtnViewItem->InitState(true,true,this);
	m_pkBtnClose->InitState(true,true, this);
	m_pkGameInfoBox->InitState(false,true,this);
	m_pkGameBossInfo->InitState(true,true,this);
	m_pkGamePlay->InitState(false,true,this);
	m_pkGameLose->InitState(false,true,this);
	m_pkGameItemView->InitState(false,true,this);

	m_pkGameInfoBox->SetActionFunc(boost::bind(&KGCGawibawiboGame::ClickChallengeBtn,this));
	m_pkGamePlay->SetActionFunc(boost::bind(&KGCGawibawiboGame::ClickGawibawiboBtn,this));
	m_pkGameLose->SetActionFunc(boost::bind(&KGCGawibawiboGame::ClickRetryBtn,this));
	ChangeState();

	m_pkStaticDescription1->InitState(true,false,NULL);
	m_pkStaticDescription2->InitState(true,false,NULL);
	m_pkStaticDescription3->InitState(true,false,NULL);
	m_pkStaticDescription4->InitState(true,false,NULL);

	m_pkStaticDescription1->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_MAIN_1 ));
	m_pkStaticDescription2->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_MAIN_2 ));
	m_pkStaticDescription3->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_MAIN_3 ));
	m_pkStaticDescription4->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_MAIN_4 ));

    m_pkBtnClose->SetHotKey(DIK_ESCAPE);
}

void KGCGawibawiboGame::ChangeState()
{
	//GAWIBAWIBO_STATE_DEFAULT	= 0,	// 도전 대기 상태
	//GAWIBAWIBO_STATE_GAME		= 1,	// 게임 상태
	//GAWIBAWIBO_STATE_CHALLENGE	= 2,	// 재도전 확인 상태

	m_pkGameInfoBox->ToggleRender(GAWIBAWIBO_STATE_DEFAULT == m_iState);
	m_pkGamePlay->ToggleRender(GAWIBAWIBO_STATE_GAME == m_iState);
	m_pkGameLose->ToggleRender(GAWIBAWIBO_STATE_CHALLENGE == m_iState);


}

void KGCGawibawiboGame::SetGawibawiboGameInfo(const KEVENT_GAWIBAWIBO_INFO_ACK &recvInfo_)
{
	if(m_iVer != static_cast<int>(recvInfo_.m_kUserGawibawiboInfo.m_usVersion)){
		m_iVer = static_cast<int>(recvInfo_.m_kUserGawibawiboInfo.m_usVersion);
		m_iStateValue = recvInfo_.m_dwRetryMark;
		m_pkGameItemView->SetReward(recvInfo_.m_mapWinReward);
		// 아이템 받는 부분 작업.
		m_pkGameItemView->SetDefaultRwardItem(recvInfo_.m_dwDefaultRewardStage);
	}
	m_iCurrentPosition = static_cast<int>(recvInfo_.m_kUserGawibawiboInfo.m_dwCurrentPosition);
	m_iMaxBoss		= static_cast<int>(recvInfo_.m_kUserGawibawiboInfo.m_usNumBoss);

	if(m_iStateValue <= m_iCurrentPosition)
	{
		m_iState = GAWIBAWIBO_STATE_CHALLENGE;
	}
	else
	{
		m_iState = GAWIBAWIBO_STATE_DEFAULT;
	}

	m_pkGameBossInfo->SetInfo(m_iCurrentPosition%m_iStateValue,m_iMaxBoss);
	
	m_pkGameInfoBox->SetGameInfo(recvInfo_);	

	m_pkGameLose->SetRetryCoin(recvInfo_.m_kUserGawibawiboInfo.m_dwRetryPoint);
	
	m_pkGameItemView->SetClearCount(static_cast<int>(recvInfo_.m_kUserGawibawiboInfo.m_dwTurnCount));
	m_pkGameItemView->SetMaxBossCnt(m_iMaxBoss);

	ChangeState();

}


void KGCGawibawiboGame::SetGawibawiboTryPointInfo(DWORD &recvInfo_)
{
	m_pkGameInfoBox->SetTryPointInfo(recvInfo_);	
}


void KGCGawibawiboGame::ResultGawiBawiBoGame(const KEVENT_GAWIBAWIBO_ACK &recvInfo_)
{
	m_pkGamePlay->SetClearPaticleEff(m_pkGameInfoBox->GetTurnCount() <static_cast<int>( recvInfo_.m_kUserGawibawiboInfo.m_dwTurnCount ));

	m_vecRevItemList.clear();

	m_pkGamePlay->ResultGawiBawiBoGame(recvInfo_);

	m_iCurrentPosition = static_cast<int>(recvInfo_.m_kUserGawibawiboInfo.m_dwCurrentPosition);
	m_iMaxBoss		= static_cast<int>(recvInfo_.m_kUserGawibawiboInfo.m_usNumBoss);
	m_vecRevItemList = recvInfo_.m_vecItem;
	
	m_pkGameLose->SetResult(static_cast<int>(recvInfo_.m_kUserGawibawiboInfo.m_usResult));

	KEVENT_GAWIBAWIBO_INFO_ACK buf;

	buf.m_kUserGawibawiboInfo = recvInfo_.m_kUserGawibawiboInfo;

	m_pkGameInfoBox->SetGameInfo(buf);
	m_pkGameItemView->SetClearCount(static_cast<int>(recvInfo_.m_kUserGawibawiboInfo.m_dwTurnCount));
	
	m_pkGameLose->SetRetryCoin(recvInfo_.m_kUserGawibawiboInfo.m_dwRetryPoint);


}

void KGCGawibawiboGame::ClickChallengeBtn()
{
	m_iState = GAWIBAWIBO_STATE_GAME;
	m_pkGamePlay->InitGamePlay();
	ChangeState();
	m_pkBtnClose->Lock(true);
	m_pkBtnViewItem->Lock(true);
    if ( m_pkBtnBuyItem )
	    m_pkBtnBuyItem->Lock(true);
}

void KGCGawibawiboGame::ClickGawibawiboBtn()
{

	m_iState = GAWIBAWIBO_STATE_CHALLENGE;

	m_pkGameLose->ToggleRender(true);
	m_pkBtnViewItem->Lock(false);
    if ( m_pkBtnBuyItem )
	    m_pkBtnBuyItem->Lock(false);

}

void KGCGawibawiboGame::ClickRetryBtn()
{
	if(m_iCurrentPosition >= m_iStateValue)
		return;

	m_pkBtnClose->Lock(false);
	if(!m_vecRevItemList.empty()){
		g_pItemMgr->m_kInventory.AddItemList( m_vecRevItemList , true);
		std::vector<KItem>::iterator vIter = m_vecRevItemList.begin();
		for(;vIter !=m_vecRevItemList.end();vIter++)
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_TREASURE_ITEM_BOX, L"", L"",
				KGCUIScene::GC_MBOX_USE_NORMAL, vIter->m_ItemID,0 ,false, true);
		}
		g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
	}
	m_iState = GAWIBAWIBO_STATE_DEFAULT;
	m_pkGameBossInfo->SetInfo(m_iCurrentPosition%m_iStateValue,m_iMaxBoss);

	m_pkGameItemView->SetBossIndex(m_iCurrentPosition%m_iStateValue);

	ChangeState();
}

void KGCGawibawiboGame::Close()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
    KGCHotEventBtn::SetHotStateOff();
}


