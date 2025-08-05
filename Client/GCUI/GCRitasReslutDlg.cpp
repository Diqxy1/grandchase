#include "stdafx.h"
#include "GCRitasReslutDlg.h"

IMPLEMENT_CLASSNAME( KGCRitasResultDlg );
IMPLEMENT_WND_FACTORY( KGCRitasResultDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCRitasResultDlg, "gc_ritasresultdlg" );

const DWORD dwAllClearReslut_DelayTime = 4000;

KGCRitasResultDlg::KGCRitasResultDlg( void ):
m_pkTitle(NULL),
m_pkStaticSoce(NULL),
m_pkStaticSoceValue(NULL)
{
	LINK_CONTROL( "title", m_pkTitle );
	LINK_CONTROL( "static_mybest_score", m_pkStaticSoce );
	LINK_CONTROL( "static_mybest_score_value", m_pkStaticSoceValue );

	m_pkGameScore = NULL;
	LINK_CONTROL( "static_game_score", m_pkGameScore );

	m_pkStaticSpecialBoxValue = NULL;
	LINK_CONTROL( "static_specialbox_value", m_pkStaticSpecialBoxValue );

	m_pkBtnStop = NULL;
	LINK_CONTROL( "btn_stop", m_pkBtnStop );


	m_pkBoxOpenFrame = NULL;
	LINK_CONTROL( "boxopen_frame", m_pkBoxOpenFrame );

	m_pkBtnOpenBox = NULL;
	LINK_CONTROL( "btn_openbox", m_pkBtnOpenBox );

	m_pkBtnClose = NULL;
	LINK_CONTROL( "btn_close", m_pkBtnClose );

	m_pkStaticLeft = NULL;
	LINK_CONTROL( "static_left", m_pkStaticLeft );
	m_pkStaticLeftValue = NULL;
	LINK_CONTROL( "static_left_value", m_pkStaticLeftValue );
	m_pkLeftIcon = NULL;
	LINK_CONTROL( "left_icon", m_pkLeftIcon );

	m_eDlgState = DLG_STATE_RESULT;

}

KGCRitasResultDlg::~KGCRitasResultDlg( void )
{
	
}

void KGCRitasResultDlg::OnCreate( void )
{
	m_pkTitle->InitState(true);
	m_pkStaticSoce->InitState(true);
	m_pkStaticSoce->SetAlign(DT_LEFT);
	m_pkStaticSoce->SetText(g_pkStrLoader->GetString(STR_ID_RITASCHRISTMAS_BESTSCORE));
	m_pkStaticSoceValue->InitState(true);
	m_pkStaticSpecialBoxValue->SetAlign(DT_RIGHT);

	m_pkStaticLeft->InitState(false);
	m_pkStaticLeft->SetAlign(DT_LEFT);
	m_pkStaticLeft->SetText(g_pkStrLoader->GetString(STR_ID_RITASCHRISTMAS_LEFT_COIN));

	m_pkStaticLeftValue->InitState(false);
	m_pkStaticLeftValue->SetAlign(DT_RIGHT);
	m_pkLeftIcon->InitState(false);

	m_pkGameScore->InitState(true);
	m_pkGameScore->SetStringType( "9th_event_num" );
	m_pkGameScore->SetWriteToRight(false);
	m_pkGameScore->SetNumber( 0 );

	m_pkStaticSpecialBoxValue->InitState(true);
	m_pkStaticSpecialBoxValue->SetAlign(DT_CENTER);

	m_pkBtnClose->InitState(true,true,this);

	m_pkBtnStop->InitState(true,true,this);

	m_pkBoxOpenFrame->InitState(false,true);
	m_pkBtnOpenBox->InitState(false,true,this);

}

void KGCRitasResultDlg::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkBtnStop, KD3DWnd::D3DWE_BUTTON_CLICK, OnStop );
	GCWND_MSG_MAP( m_pkBtnOpenBox, KD3DWnd::D3DWE_BUTTON_CLICK, OnOpneBox );
	GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );

}

void KGCRitasResultDlg::SetDlgState( eRitasResultState eState_ )
{
	m_eDlgState = eState_;
	switch(m_eDlgState)
	{
	case DLG_STATE_RESULT:
		m_pkTitle->SetWndMode(D3DWM_DEFAULT);
		m_pkBoxOpenFrame->ToggleRender(false);
		m_pkBtnOpenBox->ToggleRender(false);
		m_pkStaticLeft->SetText(g_pkStrLoader->GetString(STR_ID_RITASCHRISTMAS_LEFT_COIN));

		m_pkBtnStop->ToggleRender(true);
		m_pkLeftIcon->SetWndMode(D3DWM_DEFAULT);
		UpDateLeftPrimiumBoxNum();
		UpDateResultInfo();
		UpDateLeftCoin();
		break;
	case DLG_STATE_PRIMIUMBOX:
		m_pkTitle->SetWndMode(D3DWM_HOVER);
		m_pkBoxOpenFrame->ToggleRender(true);
		m_pkBtnOpenBox->ToggleRender(true);
		m_pkBoxOpenFrame->SetFrameType(DLG_STATE_PRIMIUMBOX);

		m_pkBtnStop->ToggleRender(false);
		m_pkLeftIcon->SetWndMode(D3DWM_HOVER);
		m_pkStaticLeft->SetText(g_pkStrLoader->GetString(STR_ID_RITASCHRISTMAS_LEFT_BOX));
		UpDateLeftPrimiumBoxNum();
		break;
	case DLG_STATE_SPECIALMBOX:
		m_pkTitle->SetWndMode(D3DWM_SELECT);
		m_pkBoxOpenFrame->ToggleRender(true);
		m_pkBtnOpenBox->ToggleRender(true);
		m_pkBoxOpenFrame->SetFrameType(DLG_STATE_SPECIALMBOX);

		m_pkBtnStop->ToggleRender(false);
		m_pkLeftIcon->SetWndMode(D3DWM_HOVER);
		m_pkStaticLeft->SetText(g_pkStrLoader->GetString(STR_ID_RITASCHRISTMAS_LEFT_BOX));
		UpDateLeftSpecialBoxNum();
		break;
	case  DLG_STATE_ALLCLEAR_RESULT:
		m_pkTitle->SetWndMode(D3DWM_DEFAULT);
		m_pkBoxOpenFrame->ToggleRender(false);
		m_pkBtnOpenBox->ToggleRender(false);

		m_pkBtnStop->ToggleRender(false);
		m_pkLeftIcon->SetWndMode(D3DWM_DEFAULT);
		m_pkStaticLeft->SetText(g_pkStrLoader->GetString(STR_ID_RITASCHRISTMAS_LEFT_COIN));
		UpDateLeftCoin();

		break;
	}
}

void KGCRitasResultDlg::OnOpneBox()
{
	switch(m_eDlgState)
	{
	case DLG_STATE_PRIMIUMBOX:
		{
			g_KDSound.Play( "Ritas_Reward_Open" );
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_MagicBox_01", 0.0f, 0.0f, 0.5f ), GC_LAYER_UI);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_MagicBox_02", 0.0f, 0.0f, 0.5f ), GC_LAYER_UI);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_MagicBox_03", 0.0f, 0.0f, 0.5f ), GC_LAYER_UI);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_MagicBox_04", 0.0f, 0.0f, 0.5f ), GC_LAYER_UI);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_MagicBox_05", 0.0f, 0.0f, 0.5f ), GC_LAYER_UI);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("BoxCrash_01", 0.0f, 0.0f, 0.5f ), GC_LAYER_UI);

			KEVENT_RITAS_CHRISTMAS_GET_REWARD_REQ kPakcet;
			kPakcet.m_nOpenBoxType = KEVENT_RITAS_CHRISTMAS_GET_REWARD_REQ::BT_PREMIUM;
			Result_ReceiveChristmasGetReward = INT_MAX;
			KP2P::GetInstance()->Send_RitasChristmas_GetReward_Req(kPakcet);
			g_MyD3D->WaitForServerAck(Result_ReceiveChristmasGetReward,INT_MAX,3000,TIME_OUT_VALUE);

			g_pkUIScene->m_pkRitasChristmasDlg->GetRankingDlg()->UpDatePrimiumInfo();
			UpDateLeftPrimiumBoxNum();

		}
		break;
	case DLG_STATE_SPECIALMBOX:
		{
			g_KDSound.Play( "Ritas_Reward_Open" );
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Box_Open_01", 0.0f, 0.0f, 0.5f ), GC_LAYER_UI);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Box_Open_02", 0.0f, 0.0f, 0.5f ), GC_LAYER_UI);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Box_Open_03", 0.0f, 0.0f, 0.5f ), GC_LAYER_UI);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Box_Open_04", 0.0f, 0.0f, 0.5f ), GC_LAYER_UI);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Box_Open_05", 0.0f, 0.0f, 0.5f ), GC_LAYER_UI);

			KEVENT_RITAS_CHRISTMAS_GET_REWARD_REQ kPakcet;
			kPakcet.m_nOpenBoxType = KEVENT_RITAS_CHRISTMAS_GET_REWARD_REQ::BT_SPECIAL;
			Result_ReceiveChristmasGetReward = INT_MAX;
			KP2P::GetInstance()->Send_RitasChristmas_GetReward_Req(kPakcet);
			g_MyD3D->WaitForServerAck(Result_ReceiveChristmasGetReward,INT_MAX,3000,TIME_OUT_VALUE);

			g_pkUIScene->m_pkRitasChristmasDlg->GetRankingDlg()->UpDatePrimiumInfo();
			UpDateLeftSpecialBoxNum();
		}
		break;
	}

}

void KGCRitasResultDlg::OnClose()
{
	ToggleRender(false);
	
	g_pkUIScene->m_pkRitasChristmasDlg->GetRankingDlg()->UpDateRankInfo();
	g_pkUIScene->m_pkRitasChristmasDlg->GetRankingDlg()->ToggleRender(true);
}

void KGCRitasResultDlg::OnContinue()
{
	ToggleRender(false);
	g_pkUIScene->m_pkRitasChristmasDlg->GetRitasPanel()->SetStartType(KEVENT_RITAS_CHRISTMAS_STAGE_START_REQ::ST_RETRY);
	g_pkUIScene->m_pkRitasChristmasDlg->GetRitasPanel()->OnStageStart();
	
}

void KGCRitasResultDlg::OnStop()
{
	g_pkUIScene->m_pkRitasChristmasDlg->GetRitasPanel()->InitGameInfo();
	if ( SiGCRitasChrismasManager()->GetSpecialNum() > 0)
	{
		SetDlgState(DLG_STATE_SPECIALMBOX);
	}
	else
	{
		OnClose();
	}
}

void KGCRitasResultDlg::UpDateResultInfo()
{
	int iBestScore = SiGCRitasChrismasManager()->GetMaxPoint();
	int iSpecialBoxNum = SiGCRitasChrismasManager()->GetSpecialNum();
	m_pkStaticSoceValue->SetText(iBestScore);

	m_pkStaticSpecialBoxValue->SetTextAutoMultiline( 
		g_pkStrLoader->GetReplacedString(STR_ID_RITASCHRISTMAS_GAIN_SPECIALBOX, "i", iSpecialBoxNum ) );

	m_pkGameScore->SetNumber(SiGCRitasChrismasManager()->GetCurrentPoint());
}

void KGCRitasResultDlg::FrameMoveInEnabledState()
{
	switch(m_eDlgState)
	{
	case DLG_STATE_PRIMIUMBOX:
		{
			int iPrimiumNum = SiGCRitasChrismasManager()->GetPrimiumNum();
			if ( iPrimiumNum <= 0 )
				ToggleRender(false);

			KItem* pkItem = g_pItemMgr->m_kInventory.FindItemByItemID(ITEM_CHRISTMAS_PRIMIUM_KEY);

			if (pkItem == NULL)
			{
				ToggleRender(false);
				return;
			}

			if (pkItem->m_nCount <= 0)
				ToggleRender(false);
		}
		break;
	case DLG_STATE_SPECIALMBOX:
		{
			if ( SiGCRitasChrismasManager()->GetSpecialNum() <= 0)
				OnClose();
		}
		break;
	}

}

void KGCRitasResultDlg::UpDateLeftCoin()
{
	m_pkStaticLeftValue->SetText(SiGCRitasChrismasManager()->GetContinuePoint() );

}

void KGCRitasResultDlg::UpDateLeftPrimiumBoxNum()
{
	m_pkStaticLeftValue->SetText(SiGCRitasChrismasManager()->GetPrimiumNum());
}

void KGCRitasResultDlg::UpDateLeftSpecialBoxNum()
{
	m_pkStaticLeftValue->SetText(SiGCRitasChrismasManager()->GetSpecialNum());
}

