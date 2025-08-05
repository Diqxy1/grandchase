#include "stdafx.h"
#include "KGCMath.h"

#ifndef DISABLE_GACHA_PON

#include "KGCGachaPongDlg.h"
#include "procedure.h"
#include "GCItemInformationBoxPair.h"
#include "KGCTreasureItemBox.h"

IMPLEMENT_CLASSNAME( KGCGachaPongDlg );
IMPLEMENT_WND_FACTORY( KGCGachaPongDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCGachaPongDlg, "gc_gachapongdlg" );

KGCGachaPongDlg::KGCGachaPongDlg(void)
{
	m_btnOK = NULL;
	m_btnClose = NULL;
	m_pkTooltipImg = NULL;
	m_pkTooltipText = NULL;
	m_pkTotalCapsule = NULL;
	m_pkRewardNumber = NULL;
	m_pkRewardImg = NULL;
	m_pkCoinImg = NULL;
	m_pkCoinNumber = NULL;
	m_btnBuy = NULL;
	m_cbRewardList = NULL;
	m_dwCurMachine = 0;
	m_dwParticleLife = 0;
	m_dwFramesAfterTurnLever = 0;
	m_iCapsuleControl = 0;

	for(int i = 0; i < MAX_MACHINE_COLORS; i++)
	{
		m_arrMachine[i] = NULL;
		LINK_CONTROL_STM( "back_machine" << i, m_arrMachine[i]);
	}

	for(int i = 0; i < MAX_BUTTON_OK_STATE; i++)
	{
		m_arrOKBtn[i] = NULL;
		LINK_CONTROL_STM( "ok_btn_img" << i, m_arrOKBtn[i]);
	}

	LINK_CONTROL("close_btn",			m_btnClose);
	LINK_CONTROL("reward_item_img",		m_pkRewardImg);
	LINK_CONTROL("coin_item_img",		m_pkCoinImg);
	LINK_CONTROL("tooltip_img",			m_pkTooltipImg);
	LINK_CONTROL("static_total_num",	m_pkTotalCapsule);
	LINK_CONTROL("static_reward_num",	m_pkRewardNumber);
	LINK_CONTROL("static_coin_num",		m_pkCoinNumber);
	LINK_CONTROL("static_tooltip",		m_pkTooltipText);
	LINK_CONTROL("ok_btn",				m_btnOK);
	LINK_CONTROL("buy_btn",				m_btnBuy);
	LINK_CONTROL("reward_list",			m_cbRewardList);
	LINK_CONTROL("checkbox_auto", m_pkCheckBoxAutoClear);

	m_bAutoMode		 = false;
	m_pkGetItemImage = new KGC2DObject();
	m_iFrameCount_ItemImage = 73;
	m_bGachaProccessed = false;
}

KGCGachaPongDlg::~KGCGachaPongDlg(void)
{
	if ( m_pkGetItemImage )
	{
		SAFE_DELETE( m_pkGetItemImage );
	}
}

void KGCGachaPongDlg::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP(m_cbRewardList,	D3DWE_COMBO_SELCHANGE,	OnMachineChange);
	GCWND_MSG_MAP(m_btnClose,		D3DWE_BUTTON_CLICK,		OnClose);
	GCWND_MSG_MAP(m_btnOK,			D3DWE_BUTTON_CLICK,		OnOK);
	GCWND_MSG_MAP(m_btnBuy,			D3DWE_BUTTON_CLICK,		OnBuy);
}

void KGCGachaPongDlg::OnCreate()
{
	for(int i = 0; i < MAX_MACHINE_COLORS; i++)
	{
		if( m_arrMachine[i] )
		{
			m_arrMachine[i]->InitState(true, true);
		}
	}

	for(int i = 0; i < MAX_BUTTON_OK_STATE; i++)
	{
		if( m_arrOKBtn[i] )
		{
			m_arrOKBtn[i]->InitState(false, true);
		}
	}

	if( m_btnClose )
	{
		m_btnClose->InitState(true, true, this);
		m_btnClose->SetHotKey( DIK_ESCAPE );
	}

	if( m_pkRewardImg )
	{
		m_pkRewardImg->InitState(true, true, this);
	}

	if( m_pkTooltipImg )
	{
		m_pkTooltipImg->InitState(true, true, this);
	}

	if( m_pkTotalCapsule )
	{
		m_pkTotalCapsule->InitState(true, true, this);
		m_pkTotalCapsule->SetAlign(DT_RIGHT);
	}

	if( m_pkRewardNumber )
	{
		m_pkRewardNumber->InitState(true, true, this);
		m_pkRewardNumber->SetAlign(DT_RIGHT);
	}

	if( m_pkCoinNumber )
	{
		m_pkCoinNumber->InitState(true, true, this);
		m_pkCoinNumber->SetAlign(DT_RIGHT);
	}

	if( m_pkTooltipText )
	{
		m_pkTooltipText->InitState(true, true, this);
		m_pkTooltipText->SetFontColor( D3DCOLOR_ARGB(255, 0, 0, 0) );
		m_pkTooltipText->SetText(g_pkStrLoader->GetString(STR_ID_GACHA_PONG_TURN_LEVER));
	}

	if( m_btnOK )
	{
		m_btnOK->InitState(false, true, this);
		m_btnOK->SetHotKey( DIK_RETURN );
		m_btnOK->SetHotKey( DIK_SPACE );
	}

	if( m_btnBuy )
	{
		m_btnBuy->InitState(true, true, this);
	}

	if( m_cbRewardList )
	{
		m_cbRewardList->InitState(true, true, this);
	}

	int nMachine = g_kGlobalValue.GetGachaPonMachineCnt();
	m_mapMachineIds.clear();
	m_cbRewardList->DeleteAllItem();

	for( int i = 0; i < nMachine; i++ )
	{
		KGachaPongMachineInfo info;
		if( g_kGlobalValue.GetGachaPonMachineInfo(i, info) )
		{
			m_cbRewardList->AddString(g_pItemMgr->GetItemName(info.m_kGachaPongReward.m_ItemID / 10));
			m_mapMachineIds.insert(std::make_pair(i, info.m_dwMachineNumber));
		}
	}

	m_pkCheckBoxAutoClear->InitState( true, true, this );
	m_pkCheckBoxAutoClear->SetCheck( m_bAutoMode );
	m_pkCheckBoxAutoClear->DisconnectAllEventProcedure();
	m_pkCheckBoxAutoClear->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCGachaPongDlg::OnClickAuto );

	m_cbRewardList->SetCurSel(0);
	UpdateMachine(m_mapMachineIds.begin()->second);
}

bool KGCGachaPongDlg::InitDialog( IN const KDialogInfo& kInfo_ )
{
	if( m_dwCurMachine == 0 )
	{
		m_dwCurMachine = m_mapMachineIds.begin()->second;
	}

	m_eMachineState = STATE_INIT;
	return true;
}

void KGCGachaPongDlg::PostChildDraw(void)
{
	if ( m_pkGetItemImage != NULL )
	{
		m_pkGetItemImage->Render();
	}
}

void KGCGachaPongDlg::FrameMoveInEnabledState()
{
	D3DXVECTOR2 vWndPos;
	const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

	if( m_pkCoinImg->CheckPosInWindowBound(vMousePos) )
	{
		vWndPos = m_pkCoinImg->GetCurrentWindowPos();
		RenderItemInfoBox(vWndPos, m_pkCoinImg->GetItemID());
	}
	else if( m_pkRewardImg->CheckPosInWindowBound(vMousePos) )
	{
		vWndPos = m_pkRewardImg->GetCurrentWindowPos();
		RenderItemInfoBox(vWndPos, m_pkRewardImg->GetItemID());
	}

	switch( m_eMachineState )
	{
	case STATE_TURN_LEVER:
		{
			if(m_dwFramesAfterTurnLever == 0)
			{
				m_arrOKBtn[0]->ToggleRender(true);
				m_arrOKBtn[1]->ToggleRender(false);
				m_arrOKBtn[2]->ToggleRender(false);
			}
			else if(m_dwFramesAfterTurnLever == 20)
			{
				m_arrOKBtn[0]->ToggleRender(false);
				m_arrOKBtn[1]->ToggleRender(true);
				m_arrOKBtn[2]->ToggleRender(false);
			}
			else if(m_dwFramesAfterTurnLever == 40)
			{
				m_arrOKBtn[0]->ToggleRender(false);
				m_arrOKBtn[1]->ToggleRender(false);
				m_arrOKBtn[2]->ToggleRender(true);
			}
#ifndef DISABLE_GACHA_PON
			else if( m_dwFramesAfterTurnLever == 41 )
			{
				SiKP2P()->Send_GachaPongPlayInfoActionReq(m_dwCurMachine, m_pkRewardImg->GetItemID() * 10);
			}
#endif
			m_dwFramesAfterTurnLever++;
		}
		break;

	case STATE_SHOW_EFFECT:
		{
			m_cbRewardList->Lock( true );
			m_btnClose->Lock(true);
			m_btnBuy->Lock( true );
			
			if( ++m_dwFramesAfterTurnLever > GC_FPS_LIMIT )
			{
				m_eMachineState = STATE_END;
			}
		}
		break;

	case STATE_END:
		{
			if( !m_bGachaProccessed )
				Init_ItemImage();

			if ( !m_bAutoMode )
			{
				Init_ItemImage();
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_TREASURE_ITEM_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
			}
			
			m_pkCoinNumber->SetText( g_pItemMgr->GetInventoryItemDuration( m_pkCoinImg->GetItemID() ) );

			m_dwFramesAfterTurnLever = 40;

			if ( !m_bAutoMode )
			{
				m_btnOK->ToggleRender( true );
				m_pkTooltipImg->ToggleRender( true );
				m_pkTooltipText->ToggleRender( true );
				m_cbRewardList->Lock( false );
				m_btnClose->Lock( false );
				m_btnBuy->Lock( false );
				m_dwFramesAfterTurnLever = 0;
			}

			if ( !m_bAutoMode )
				m_eMachineState = STATE_INIT;
			else
				FrameMove_ItemImage();
		}
		break;
	}
}

void KGCGachaPongDlg::RestartMachine()
{
	m_bAutoMode = false;
	m_pkCheckBoxAutoClear->SetCheck( m_bAutoMode );
	m_eMachineState = STATE_END;
}

void KGCGachaPongDlg::OnClose()
{
	m_pkGetItemImage->RemoveAllTexture();
	m_bAutoMode = false;
	m_pkCheckBoxAutoClear->SetCheck( m_bAutoMode );
	g_KDSound.Play( "31" );
	SiKP2P()->Send_GachaPongPlayInfoUnsubscribeReq();
	CloseDlg();
}

void KGCGachaPongDlg::OnOK()
{
	if( !m_bAutoMode )
		g_KDSound.Play( "31" );

	if( g_pItemMgr->GetInventoryItemDuration(m_pkCoinImg->GetItemID()) <= 0 )
	{
		m_bAutoMode = false;
		m_pkCheckBoxAutoClear->SetCheck( m_bAutoMode );
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_GACHA_PONG_INSUFFICIENT_COIN ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
		return;
	}

	m_pkTooltipText->ToggleRender( false );
	m_pkTooltipImg->ToggleRender( false );
	m_btnOK->ToggleRender( false );
	m_eMachineState = STATE_TURN_LEVER;
}

void KGCGachaPongDlg::OnBuy()
{
	g_KDSound.Play( "31" );

	GCItem* item = g_pItemMgr->GetItemData( m_pkCoinImg->GetItemID() );

	switch ( item->dwGoodsID )
	{
		case 206081:
			g_pItemMgr->BuyCashItemQuantity( item->dwGoodsID, true );
			break;
		case 155375:
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_NEW_COMBO_BOX_SHOP, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, item->dwGoodsID, KBuySellItemReq::EMT_GAMEPOINT, true, true );
			break;
		case 155374:
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_NEW_COMBO_BOX_SHOP, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, item->dwGoodsID, KBuySellItemReq::EMT_CRYSTAL, true, true );
			break;
		default:
			break;
	}
}

void KGCGachaPongDlg::OnMachineChange()
{
	int nCurSel = m_cbRewardList->GetCurSel();
	if(nCurSel == -1)
	{
		return;
	}

	std::map<int, DWORD>::iterator mit = m_mapMachineIds.find(nCurSel);

	if( mit != m_mapMachineIds.end() )
	{
		if(m_dwCurMachine != mit->second )
		{
			m_dwCurMachine = mit->second;
			m_iCapsuleControl = 0;
			UpdateMachine(m_dwCurMachine);
		}
	}
	else
	{
		m_dwCurMachine = 0;
		m_iCapsuleControl = 0;
		UpdateMachine(m_dwCurMachine);
	}
}

void KGCGachaPongDlg::UpdateCapsuleNumber()
{
	if( m_iCapsuleControl != 0 && m_iCapsuleControl < g_kGlobalValue.m_stGachaPonMachine.m_dwLeftCapsule )
	{
		m_bAutoMode = false;
		m_pkCheckBoxAutoClear->SetCheck( m_bAutoMode );
		m_iCapsuleControl = 0;
	}

	m_iCapsuleControl = g_kGlobalValue.m_stGachaPonMachine.m_dwLeftCapsule;

	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Gachapon_Count_01", 0.39f, 0.018f,0.5f ), GC_LAYER_UI );
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Gachapon_Count_02", 0.39f, 0.018f,0.5f ), GC_LAYER_UI );
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Gachapon_Count_03", 0.39f, 0.018f,0.5f ), GC_LAYER_UI );
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Gachapon_Count_01", 0.39f,-0.02f,0.5f ), GC_LAYER_UI );
	m_pkRewardNumber->SetText( g_kGlobalValue.m_stGachaPonMachine.m_dwRewardCnt );
	m_pkTotalCapsule->SetText( g_kGlobalValue.m_stGachaPonMachine.m_dwLeftCapsule );
}

void KGCGachaPongDlg::UpdateMachine(DWORD dwMachineNumber)
{
	KGachaPongMachineInfo machineInfo;

	SiKP2P()->Send_GachaPongPlayInfoReq( dwMachineNumber );
	Result_GachaPongReq = INT_MAX;
	g_MyD3D->WaitForServerAck( Result_GachaPongReq, INT_MAX, 250, TIME_OUT_VALUE );
	
	if( !g_kGlobalValue.GetGachaPonMachineInfo(m_dwCurMachine, machineInfo) )
	{
		m_bAutoMode = false;
		m_pkCheckBoxAutoClear->SetCheck( m_bAutoMode );
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_GACHA_PONG_INFO_ERROR), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
		OnClose();
	}

	m_arrOKBtn[0]->ToggleRender( true );
	if( machineInfo.m_dwState == STATE_INIT )
	{
		m_eMachineState = STATE_INIT;
		m_btnOK->ToggleRender( true );
	}

	for( int i = 0; i < MAX_MACHINE_COLORS; i++ )
	{
		m_arrMachine[i]->ToggleRender(false);
	}

	int iMachineColor = m_iLastMachineColor;
	do 
	{
		iMachineColor = rand() % MAX_MACHINE_COLORS;
	} while (iMachineColor == m_iLastMachineColor);

	m_iLastMachineColor = iMachineColor;
	m_arrMachine[m_iLastMachineColor]->ToggleRender(true);

	m_pkCoinImg->SetItemInfo(machineInfo.m_dwGachaPonCoin / 10);
	m_pkCoinNumber->SetText(g_pItemMgr->GetInventoryItemDuration(m_pkCoinImg->GetItemID()));
	m_pkRewardImg->SetItemInfo(machineInfo.m_kGachaPongReward.m_ItemID / 10);
	UpdateCapsuleNumber();
	m_pkTooltipText->ToggleRender( true );
	m_pkTooltipImg->ToggleRender( true );
}

void KGCGachaPongDlg::ShowBallRollingEffect()
{
	m_eMachineState = STATE_SHOW_EFFECT;
	m_dwFramesAfterTurnLever = 0;

	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Gacha_Pong_Open_01", 0.f, -0.05f, 0.5f), GC_LAYER_UI);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Gacha_Pong_Open_02", 0.f, -0.05f, 0.5f), GC_LAYER_UI);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Gacha_Pong_Open_03", 0.f, -0.05f, 0.5f), GC_LAYER_UI);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Gacha_Pong_Open_04", 0.f, -0.05f, 0.5f), GC_LAYER_UI);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Gacha_Pong_Open_05", 0.f, -0.05f, 0.5f), GC_LAYER_UI);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Gacha_Pong_Open_06", 0.f, -0.05f, 0.5f), GC_LAYER_UI);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Gacha_Pong_Open_08", 0.f, -0.05f, 0.5f), GC_LAYER_UI);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Gacha_Pong_Open_09", 0.f, -0.05f, 0.5f), GC_LAYER_UI);

	int iCapsuleColor = rand() % MAX_MACHINE_COLORS;

	switch(iCapsuleColor)
	{
	case RED_CAPSULE:
		{
			g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Gacha_Pong_Open_07", 0.f, -0.05f, 0.5f), GC_LAYER_UI);
		}
		break;
	case BLUE_CAPSULE:
		{
			g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Gacha_Pong_Open_07_Blue", 0.f, -0.05f, 0.5f), GC_LAYER_UI);
		}
		break;
	case YELLOW_CAPSULE:
		{
			g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Gacha_Pong_Open_07_Yellow", 0.f, -0.05f, 0.5f), GC_LAYER_UI);
		}
		break;
	}

	UpdateCapsuleNumber();
}

void KGCGachaPongDlg::RenderItemInfoBox( D3DXVECTOR2& vPos_, GCITEMID dwItemID_ )
{
	vPos_.x *= m_fWindowScaleX;
	vPos_.y *= m_fWindowScaleY;
	vPos_.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth() * g_pkUIScene->m_pkItemInformationBox->GetWindowScaleX();

	GCItem* pItem = g_pItemMgr->GetItemData(dwItemID_);
	KItem* pKItem =g_pItemMgr->m_kInventory.FindItemByItemID(dwItemID_);
	g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem, -1, NULL, NULL, NULL, -1, 0 );
	g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
	g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
	g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vPos_ );
	g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
}

void KGCGachaPongDlg::OnClickAuto()
{
	if (m_pkCheckBoxAutoClear == NULL) {
		return;
	}

	m_pkCheckBoxAutoClear->SetCheck( !m_pkCheckBoxAutoClear->GetCheck() );
	m_bAutoMode = m_pkCheckBoxAutoClear->GetCheck();
}

void KGCGachaPongDlg::Init_ItemImage()
{
	m_bGachaProccessed = true;
	m_pkGetItemImage->RemoveAllTexture();
	int iItemID = g_pkUIScene->m_pkTreaureItemBox->GetItemID();
	if (iItemID == 0)
		return;

	m_pkGetItemImage->AddTexture(g_pItemMgr->CreateShopItemTexture(iItemID));

	m_pkGetItemImage->SetScale(0.0f, 0.0f);
	m_pkGetItemImage->SetAxisOffset(-0.5f, -0.5f, 0.0f);
	m_pkGetItemImage->SetPosition(1.005f, 0.68f);
	m_pkGetItemImage->SetAlpha(255);
	m_pkGetItemImage->CameraMatrixOn();
	m_iFrameCount_ItemImage = 73;
}

void KGCGachaPongDlg::FrameMove_ItemImage()
{
	if ( g_pkUIScene->m_pkTreaureItemBox->GetItemID() == 0 )
		return;

	++m_iFrameCount_ItemImage;

	if (m_iFrameCount_ItemImage > 72)
	{
		int iFrame = m_iFrameCount_ItemImage - 72;
		if (iFrame > 0 && iFrame < 50)
		{
			float delta = SiKGCMath()->f(5, (float)iFrame / 20.0f) / 8.0f;
			m_pkGetItemImage->SetScale(
				m_pkGetItemImage->GetWidth() + delta,
				m_pkGetItemImage->GetHeight() + delta
			);
		}
		else if (iFrame > 50 && iFrame < 110)
		{
			m_pkGetItemImage->SetScale(
				m_pkGetItemImage->GetWidth() + 0.002f,
				m_pkGetItemImage->GetHeight() + 0.002f
			);
			m_pkGetItemImage->SetAlpha(255 - (iFrame - 50) * 4);
		}
		else if (iFrame == 110)
		{
			m_pkGetItemImage->SetScale(0.0f, 0.0f);
			m_bGachaProccessed = false;
			OnOK();
		}
	}
}

#endif