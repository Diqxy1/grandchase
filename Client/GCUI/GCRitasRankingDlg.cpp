#include "stdafx.h"
#include "GCRitasRankingDlg.h"


IMPLEMENT_CLASSNAME( KGCRitasRankingDlg );
IMPLEMENT_WND_FACTORY( KGCRitasRankingDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCRitasRankingDlg, "gc_ritasrankingdlg" );

KGCRitasRankingDlg::KGCRitasRankingDlg( void ):
m_pkBtnStart(NULL),
m_pkBtnClose(NULL),
m_pkBtnHelp(NULL),
m_pkHelpPlanCard(NULL),
m_pkBtnMyRank(NULL),
m_pkMyRankOver(NULL),
m_pkBtnAllRank(NULL),
m_pkAllRankOver(NULL),
m_pkBtnOpenBox(NULL),
m_pkStaticTodayScore(NULL),
m_pkStaticTodayScoreValue(NULL),
m_pkStaticOutOfRank(NULL),
m_pkStaticPrimium(NULL),
m_pkStaticSpecialKey(NULL),
m_pkStaticPrimiumValue(NULL),
m_pkStaticSpecialKeyValue(NULL),
m_pkRankList(NULL),
m_pkMyRank(NULL),
m_pkLevelImg(NULL),
m_pkStaticLeftCoin(NULL),
m_pkStaticLeftCoinValue(NULL),
m_pkStaticRankGuide(NULL)
{
	LINK_CONTROL( "btn_start", m_pkBtnStart );
	LINK_CONTROL( "btn_close", m_pkBtnClose );
	LINK_CONTROL( "btn_help", m_pkBtnHelp );
	LINK_CONTROL( "help_plancard", m_pkHelpPlanCard );

	LINK_CONTROL( "btn_myrank", m_pkBtnMyRank );
	LINK_CONTROL( "myrank_over", m_pkMyRankOver );
	LINK_CONTROL( "btn_allrank", m_pkBtnAllRank );
	LINK_CONTROL( "allrank_over", m_pkAllRankOver );
	LINK_CONTROL( "btn_openbox", m_pkBtnOpenBox );

	LINK_CONTROL( "static_today_score", m_pkStaticTodayScore );
	LINK_CONTROL( "static_today_score_value", m_pkStaticTodayScoreValue );

	LINK_CONTROL( "static_outofrank_comment", m_pkStaticOutOfRank );


	LINK_CONTROL( "static_primium", m_pkStaticPrimium );
	LINK_CONTROL( "static_specialkey", m_pkStaticSpecialKey );
	LINK_CONTROL( "static_primium_value", m_pkStaticPrimiumValue );
	LINK_CONTROL( "static_specialkey_value", m_pkStaticSpecialKeyValue );

	LINK_CONTROL( "rank_list", m_pkRankList );

	LINK_CONTROL( "static_myrank", m_pkMyRank );
	LINK_CONTROL( "level_img", m_pkLevelImg );

	LINK_CONTROL( "static_left_coin", m_pkStaticLeftCoin );
	LINK_CONTROL( "static_left_coin_value", m_pkStaticLeftCoinValue );

	LINK_CONTROL( "static_rank_guide", m_pkStaticRankGuide );

}

KGCRitasRankingDlg::~KGCRitasRankingDlg( void )
{
}

void KGCRitasRankingDlg::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
	GCWND_MSG_MAP( m_pkBtnStart, KD3DWnd::D3DWE_BUTTON_CLICK, OnStart );
	GCWND_MSG_MAP( m_pkBtnHelp, KD3DWnd::D3DWE_BUTTON_CLICK, OnHelp );

	GCWND_MSG_MAP( m_pkBtnMyRank, KD3DWnd::D3DWE_BUTTON_CLICK, OnMyRank );
	GCWND_MSG_MAP( m_pkBtnAllRank, KD3DWnd::D3DWE_BUTTON_CLICK, OnAllRnak );
	GCWND_MSG_MAP( m_pkBtnOpenBox, KD3DWnd::D3DWE_BUTTON_CLICK, OnOpenBox );
}

void KGCRitasRankingDlg::OnCreate( void )
{
	m_pkBtnStart->InitState(true,true,this);

	m_pkBtnClose->InitState( true, true, this );
	//m_pkBtnClose->SetHotKey( DIK_ESCAPE );

	m_pkBtnHelp->InitState(true,true,this);
	m_pkHelpPlanCard->InitState(false,true,this);
	m_pkRankList->InitState(false,true,this);

	m_pkBtnMyRank->InitState(true,true,this);
	m_pkMyRankOver->InitState(true);
	m_pkBtnAllRank->InitState(true,true,this);
	m_pkAllRankOver->InitState(false);

	m_pkBtnOpenBox->InitState(true,true,this);

	m_pkStaticTodayScore->InitState(true);
	m_pkStaticTodayScoreValue->InitState(true);

	m_pkStaticOutOfRank->InitState(false);

	m_pkStaticPrimium->InitState(true);
	m_pkStaticSpecialKey->InitState(true);
	m_pkStaticPrimiumValue->InitState(true);
	m_pkStaticSpecialKeyValue->InitState(true);
		

	m_pkMyRank->InitState(true);
	m_pkMyRank->SetStringType("9th_event_num");
	m_pkMyRank->SetNumber(0);
	m_pkMyRank->SetWriteToRight(false);
	m_pkLevelImg->InitState(true);

	m_pkStaticLeftCoin->InitState(true);
	m_pkStaticLeftCoin->SetAlign(DT_LEFT);
	m_pkStaticLeftCoin->SetText(g_pkStrLoader->GetString(STR_ID_RITASCHRISTMAS_LEFT_COIN).c_str());

	m_pkStaticLeftCoinValue->InitState(true);
	m_pkStaticLeftCoinValue->SetAlign(DT_RIGHT);

	m_pkStaticRankGuide->InitState(true);
	m_pkStaticRankGuide->SetAlign(DT_CENTER);
	m_pkStaticRankGuide->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_RITASCHRISTMAS_RANK_GUIDE).c_str());

}

void KGCRitasRankingDlg::OnClose()
{
	g_KDSound.Play( "31" );
	
	g_pkUIScene->m_pkRitasChristmasDlg->OnClose();
}

void KGCRitasRankingDlg::OnStart()
{
	if (SiGCRitasChrismasManager()->GetStartCoinCnt() <= 0)
		return;

	g_pkUIScene->m_pkRitasChristmasDlg->GetRitasPanel()->InitGameInfo();

	if (g_pkUIScene->m_pkRitasChristmasDlg->GetRitasPanel()->OnStageStart())
		ToggleRender(false);
	
	SetStartCoin();

}

void KGCRitasRankingDlg::OnHelp()
{
	m_pkHelpPlanCard->ToggleRender(true);
}

void KGCRitasRankingDlg::FrameMoveInEnabledState()
{
	if (m_pkHelpPlanCard->IsRenderOn())
		m_pkBtnClose->Lock(true);
	else
		m_pkBtnClose->Lock(false);

	KItem* pkItem = g_pItemMgr->m_kInventory.FindItemByItemID(ITEM_CHRISTMAS_PRIMIUM_KEY);
	if (pkItem != NULL && pkItem->m_nCount >0)
	{
		if ( SiGCRitasChrismasManager()->GetPrimiumNum() > 0 )
		{
			m_pkBtnOpenBox->Lock(false);
		}
		else
		{
			m_pkBtnOpenBox->Lock(true);
		}
	}
	else
	{	
		m_pkBtnOpenBox->Lock(true);
	}

		

}

void KGCRitasRankingDlg::OnCreateComplete()
{
	m_pkStaticOutOfRank->SetAlign(DT_CENTER);
	m_pkStaticOutOfRank->SetTextAutoMultiline (g_pkStrLoader->GetString( STR_ID_RITASCHRISTMAS_OUTOFRANK ).c_str() );

	m_pkStaticTodayScore->SetAlign(DT_LEFT);
	m_pkStaticTodayScore->SetText(g_pkStrLoader->GetString( STR_ID_RITASCHRISTMAS_BESTSCORE ).c_str() );
	m_pkStaticTodayScoreValue->SetAlign(DT_RIGHT);
	m_pkStaticTodayScoreValue->SetText(SiGCRitasChrismasManager()->GetMaxPoint());



	m_pkStaticPrimium->SetAlign(DT_LEFT);
	m_pkStaticPrimium->SetText( g_pkStrLoader->GetString( STR_ID_RITASCHRISTMAS_PRIMIUMBOX ).c_str() );
	m_pkStaticPrimiumValue->SetAlign(DT_RIGHT);
	m_pkStaticPrimiumValue->SetText(SiGCRitasChrismasManager()->GetPrimiumNum());

	m_pkStaticSpecialKey->SetAlign(DT_LEFT);
	m_pkStaticSpecialKey->SetText( g_pkStrLoader->GetString( STR_ID_RITASCHRISTMAS_SPECIALKEY ).c_str() );
	m_pkStaticSpecialKeyValue->SetAlign(DT_RIGHT);

	KItem* pkItem = g_pItemMgr->m_kInventory.FindItemByItemID(ITEM_CHRISTMAS_PRIMIUM_KEY);

	if (pkItem == NULL)
		m_pkStaticSpecialKeyValue->SetText(0);
	else
		m_pkStaticSpecialKeyValue->SetText(pkItem->m_nCount);

	SetStartCoin();

	//내 랭킹정보를 초기 화면으로 세팅한다.
	OnMyRank();



}

void KGCRitasRankingDlg::OnMyRank()
{
	m_pkAllRankOver->ToggleRender(false);
	m_pkMyRankOver->ToggleRender(true);

	m_pkRankList->ToggleRender(false);


	int iMyRank = SiGCRitasChrismasManager()->GetMyRank();
	if ( iMyRank<0 || iMyRank > MAX_NUM_RANK)
	{
		m_pkStaticOutOfRank->ToggleRender(true);
		m_pkLevelImg->ToggleRender(false);
		m_pkMyRank->ToggleRender(false);
	}
	else
	{
		m_pkMyRank->ToggleRender(true);
		m_pkMyRank->SetNumber(iMyRank);
		m_pkLevelImg->ToggleRender(true);
		m_pkStaticOutOfRank->ToggleRender(false);
	}
		
}

void KGCRitasRankingDlg::OnAllRnak()
{
	m_pkAllRankOver->ToggleRender(true);
	m_pkMyRankOver->ToggleRender(false);

	m_pkRankList->ToggleRender(true);
	m_pkRankList->InitRankList();
}

void KGCRitasRankingDlg::OnOpenBox()
{
	KItem* pkItem = g_pItemMgr->m_kInventory.FindItemByItemID(ITEM_CHRISTMAS_PRIMIUM_KEY);
	if (pkItem != NULL && pkItem->m_nCount >0)
	{
		if ( SiGCRitasChrismasManager()->GetPrimiumNum() > 0 )
		{
			g_pkUIScene->m_pkRitasChristmasDlg->GetResultDlg()->SetDlgState(KGCRitasResultDlg::DLG_STATE_PRIMIUMBOX);
			g_pkUIScene->m_pkRitasChristmasDlg->GetResultDlg()->ToggleRender(true);
		}
	}
	else
	{
		std::wstring strTemp = g_pkStrLoader->GetString(STR_ID_RITASCHRISTMAS_PRIMIUM_ERR);
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, strTemp, L"",KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true );

	}

}

void KGCRitasRankingDlg::SetStartCoin()
{
	if (SiGCRitasChrismasManager()->GetStartCoinCnt() <= 0)
	{
		m_pkBtnStart->Lock(true);
		m_pkStaticLeftCoinValue->SetText(0);
	}
	else
	{
		m_pkBtnStart->Lock(false);
		m_pkStaticLeftCoinValue->SetText(SiGCRitasChrismasManager()->GetStartCoinCnt());
	}

}

void KGCRitasRankingDlg::UpDatePrimiumInfo()
{
	m_pkStaticPrimiumValue->SetText(SiGCRitasChrismasManager()->GetPrimiumNum());
	KItem* pkItem = g_pItemMgr->m_kInventory.FindItemByItemID(ITEM_CHRISTMAS_PRIMIUM_KEY);

	if (pkItem == NULL)
		m_pkStaticSpecialKeyValue->SetText(0);
	else
		m_pkStaticSpecialKeyValue->SetText(pkItem->m_nCount);

}

void KGCRitasRankingDlg::UpDateRankInfo()
{
	m_pkStaticTodayScoreValue->SetText(SiGCRitasChrismasManager()->GetMaxPoint());

	Result_ReceiveChristmasRankInfo = INT_MAX;
	KP2P::GetInstance()->Send_RitasChristmas_RankInfo_Req();
	g_MyD3D->WaitForServerAck( Result_ReceiveChristmasRankInfo, INT_MAX, 3000, TIME_OUT_VALUE );

	OnMyRank();
}
