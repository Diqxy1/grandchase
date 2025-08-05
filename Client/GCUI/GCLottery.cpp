#include "stdafx.h"
#include "KGCMath.h"
#include "GCLottery.h"
#include "GCUI/GCLotteryRewardBox.h"
#include "GCUtil/GCRandomObj.h"


IMPLEMENT_CLASSNAME( KGCLottery );
IMPLEMENT_WND_FACTORY( KGCLottery );
IMPLEMENT_WND_FACTORY_NAME( KGCLottery, "gc_lottery" );

KGCLottery::KGCLottery( void )
{
	for (int i=0 ; i<NUM_SHOW_ITEM_IMG ; i++ )
	{
		m_pkImgRect[i] = NULL;
		m_pTexShowImg[i] = NULL;

		char temp[MAX_PATH];
		sprintf( temp, "img_rect%d", i );
		LINK_CONTROL( temp, m_pkImgRect[i] );
	}
	
	m_pkOverBlindRect = NULL;
	LINK_CONTROL("over_blind_rect",m_pkOverBlindRect);
	m_btnExit = NULL;
	LINK_CONTROL("exit",m_btnExit);
	m_btnAuto = NULL;
	LINK_CONTROL("auto",m_btnAuto);
	m_btnYes = NULL;
	LINK_CONTROL("yes",m_btnYes);
	m_btnLeft = NULL;
	LINK_CONTROL("left",m_btnLeft);
	m_btnRight = NULL;
	LINK_CONTROL("right",m_btnRight);
	m_pTexOverBlind = NULL;
	m_pkCheckRect = NULL;
	LINK_CONTROL("check_rect",m_pkCheckRect);

	m_iCharType = -1;
	m_eLotteryState = STATE_MANUAL;
	m_dwFrameCnt = 1;
	m_fAutoScratchCnt = 0.0f;
	m_iScratchType = -1;

	m_pkCharSelectBox = NULL;
	LINK_CONTROL("char_select_box",m_pkCharSelectBox);
	m_pkTitleQuestion = NULL;
	LINK_CONTROL("title_question",m_pkTitleQuestion);
	m_pkTitleRejection = NULL;
	LINK_CONTROL("title_rejection",m_pkTitleRejection);
	m_pkTitleGuide = NULL;
	LINK_CONTROL("title_guide",m_pkTitleGuide);

	m_bHaveLottery = false;
	m_bNoTouch = true;

	m_pkRewardDisplayRect = NULL;
	LINK_CONTROL("reward_box_rect",m_pkRewardDisplayRect);
	for (int i = 0 ; i <NUM_REWARD_DISPLAY_BOX ; i++)
		m_pkRewardDisplayBox[i] = NULL;
	
	m_pkStaticGuide = NULL;
	LINK_CONTROL("static_guide",m_pkStaticGuide);
	m_pkStaticExplain0 = NULL;
	LINK_CONTROL("static_explain0",m_pkStaticExplain0);
	m_pkStaticExplain1 = NULL;
	LINK_CONTROL("static_explain1",m_pkStaticExplain1);
	m_pkStaticExplain2 = NULL;
	LINK_CONTROL("static_explain2",m_pkStaticExplain2);
	m_pkStaticNumLottery = NULL;
	LINK_CONTROL("static_num_lottery",m_pkStaticNumLottery);
	m_pkCheckBoxAutoClear = NULL;
	LINK_CONTROL("checkbox_auto", m_pkCheckBoxAutoClear);

	m_iFirstIndex = 0;
	m_bSuccescc = false;
	m_dwFrammCntAtShowEffect = 0;
	m_bAutoMode = false;
}

KGCLottery::~KGCLottery( void )
{
	SAFE_RELEASE(m_pTexOverBlind);
}

void KGCLottery::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_btnExit,                  KD3DWnd::D3DWE_BUTTON_CLICK, OnExit );
	GCWND_MSG_MAP( m_btnAuto,                  KD3DWnd::D3DWE_BUTTON_CLICK, OnAuto );
	GCWND_MSG_MAP( m_btnYes,                   KD3DWnd::D3DWE_BUTTON_CLICK, OnYes );
	GCWND_MSG_MAP( m_btnLeft,                  KD3DWnd::D3DWE_BUTTON_CLICK, OnLeft );
	GCWND_MSG_MAP( m_btnRight,                 KD3DWnd::D3DWE_BUTTON_CLICK, OnRight );

	if( event.m_pWnd == m_pkCharSelectBox && event.m_dwCode == D3DWE_COMBO_SELCHANGE )
	{
		if (m_eLotteryState == STATE_INIT)
		{
			std::map<int,char>::iterator mit = m_mapCharType.find(m_pkCharSelectBox->GetCurSel());
			if ( mit == m_mapCharType.end() )
			{
				m_iCharType = -1;
			}
			else
			{
				m_iCharType  = mit->second;
				m_iFirstIndex = 0;
				SetRewardDisplay_();
			}
		}
	}
}

void KGCLottery::OnCreate( void )
{
	SAFE_RELEASE(m_pTexOverBlind);
	m_pTexOverBlind = g_pGCDeviceManager2->CreateTexture( "scratch.dds" );
	m_pkOverBlindRect->InitState(true,true,this);
	m_btnExit->InitState(true,true,this);
	m_btnExit->SetHotKey(DIK_ESCAPE);
	m_btnAuto->InitState(true,true,this);
	m_btnYes->InitState(false,true,this);
	m_btnLeft->InitState(true,true,this);
	m_btnRight->InitState(true,true,this);
	m_pkTitleQuestion->InitState(false);
	m_pkTitleRejection->InitState(false);
	m_pkTitleGuide->InitState(false);
	m_pkStaticGuide->InitState(true);
    m_pkStaticGuide->SetFontColor( D3DCOLOR_ARGB( 255, 0, 0, 0 )  );
	m_pkStaticGuide->SetTextAutoMultiline(g_pkStrLoader->GetString( STR_ID_LOTTERY_GUIDE ));
	m_pkStaticExplain0->InitState(true);
	m_pkStaticExplain0->SetText(g_pkStrLoader->GetString( STR_ID_LOTTERY_EXPLAIN0 ));
	m_pkStaticExplain0->SetAutoScroll(true);
	m_pkStaticExplain1->InitState(true);
	m_pkStaticExplain1->SetAutoScroll(true);
	m_pkStaticExplain1->SetText(g_pkStrLoader->GetString( STR_ID_LOTTERY_EXPLAIN1 ));
	m_pkStaticExplain2->InitState(true);
	m_pkStaticExplain2->SetText(g_pkStrLoader->GetString( STR_ID_LOTTERY_EXPLAIN2 ));
	m_pkStaticExplain2->SetAutoScroll(true);
	m_pkStaticNumLottery->InitState(true);
	m_iCharType = g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType;
	m_pkCheckBoxAutoClear->InitState( true, true, this );
	m_pkCheckBoxAutoClear->SetCheck( m_bAutoMode );
	m_pkCheckBoxAutoClear->DisconnectAllEventProcedure();
	m_pkCheckBoxAutoClear->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCLottery::OnClickAuto );
	
	SetInitVailue();
}


void KGCLottery::PostChildDraw()
{
	g_pGCDeviceManager2->PushState();
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	if (m_eLotteryState != STATE_INIT)
	{
		for (int i=0 ; i<NUM_SHOW_ITEM_IMG ; i++)
		{
			if (m_pTexShowImg[i] == NULL)
				continue;

			D3DXVECTOR2 vPos = GetRelocatedWindowPos();
			vPos.x += m_pkImgRect[i]->GetFixedWindowLocalPos().x * m_fWindowScaleX;
			vPos.y += m_pkImgRect[i]->GetFixedWindowLocalPos().y * m_fWindowScaleY;
			g_pGCDeviceManager2->DrawInScreen( m_pTexShowImg[i], vPos.x, vPos.y, 
				vPos.x + m_pkImgRect[i]->GetWidth() * m_fWindowScaleX , vPos.y + m_pkImgRect[i]->GetHeight() * m_fWindowScaleY , 0.0f, 0.0f, 1.0f, 1.0f );

		}
	}
	if (m_eLotteryState != STATE_SHOW_EFFECT)
	{
		D3DXVECTOR2 vPos = GetRelocatedWindowPos();
		vPos.x += m_pkOverBlindRect->GetFixedWindowLocalPos().x * m_fWindowScaleX;
		vPos.y += m_pkOverBlindRect->GetFixedWindowLocalPos().y * m_fWindowScaleY;
		g_pGCDeviceManager2->DrawInScreen( m_pTexOverBlind, vPos.x, vPos.y, 
			vPos.x + m_pkOverBlindRect->GetWidth() * m_fWindowScaleX , vPos.y + m_pkOverBlindRect->GetHeight() * m_fWindowScaleY , 0.0f, 0.0f, 1.0f, 1.0f );
	}
	g_pGCDeviceManager2->PopState();

	switch(m_eLotteryState)
	{
	case STATE_INIT:
		if (m_bHaveLottery)
		{
			m_pkTitleQuestion->ToggleRender(true);
			m_pkTitleQuestion->Render();
			m_pkTitleQuestion->ToggleRender(false);
			m_btnYes->Render();
		}
		else
		{
			m_pkTitleRejection->ToggleRender(true);
			m_pkTitleRejection->Render();
			m_pkTitleRejection->ToggleRender(false);
		}
		break;
	case STATE_MANUAL:
		m_btnYes->ToggleRender(false);
		if (m_bNoTouch)
		{
			m_pkTitleGuide->ToggleRender(true);
			m_pkTitleGuide->Render();
			m_pkTitleGuide->ToggleRender(false);
		}
		break;
	}
	m_btnAuto->Render();
	m_pkCharSelectBox->Render();
}

void KGCLottery::ScratchOnBoard( POINT ptPoint_ )
{
	if ( m_pTexOverBlind == NULL )
		return;

	D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)ptPoint_.x, (float)ptPoint_.y );

	if( !m_pkOverBlindRect->CheckPosInWindowBound( vMousePos ) )
		return;

	//최초 한번만 플레이
	if (m_bNoTouch)
		g_KDSound.Play( "SoulUp" );

	m_bNoTouch = false;

	LPDIRECT3DSURFACE9 d3dSurface;
	if ( FAILED(m_pTexOverBlind->GetDeviceTexture()->GetSurfaceLevel(0,&d3dSurface)) )
		return;

	D3DSURFACE_DESC d3dDesc;
	if ( FAILED(m_pTexOverBlind->GetDeviceTexture()->GetLevelDesc(0,&d3dDesc)) )
		return;

	D3DLOCKED_RECT d3dRect;
	if ( FAILED(d3dSurface->LockRect(&d3dRect , NULL , D3DLOCK_DISCARD)) )
		return;

	DWORD* dwAccess = static_cast<DWORD*>(d3dRect.pBits);

	D3DXVECTOR2 vPos = GetRelocatedWindowPos();
	vPos.x += m_pkOverBlindRect->GetFixedWindowLocalPos().x * m_fWindowScaleX;
	vPos.y += m_pkOverBlindRect->GetFixedWindowLocalPos().y * m_fWindowScaleY;

	vMousePos -= vPos;

	float fXRate = (static_cast<float>(d3dDesc.Width)/m_pkOverBlindRect->GetWidth()/m_fWindowScaleX);
	float fYRate = (static_cast<float>(d3dDesc.Height)/m_pkOverBlindRect->GetHeight()/m_fWindowScaleY);

	
	POINT ptCenter;
	ptCenter.x = static_cast<long>(vMousePos.x*fXRate);
	ptCenter.y = static_cast<long>(vMousePos.y*fYRate);

	//Range1
	int iStartXIndex = static_cast<int>((vMousePos.x - RAN_RANGE3* m_fWindowScaleX) * fXRate);
	int iStartYIndex = static_cast<int>((vMousePos.y - RAN_RANGE3* m_fWindowScaleY) * fYRate);
	int iEndXIndex = static_cast<int>((vMousePos.x + RAN_RANGE3* m_fWindowScaleX) * fXRate);
	int iEndYIndex = static_cast<int>((vMousePos.y + RAN_RANGE3* m_fWindowScaleY) * fYRate);

	GCFUNC::DrawDotEllips( dwAccess , d3dDesc , iStartXIndex , iEndXIndex , iStartYIndex , iEndYIndex , ptCenter , APHADOT );
	
	m_dwFrameCnt++;

	if ( (m_dwFrameCnt%GC_FPS_LIMIT) == 0 )
	{
		D3DXVECTOR2 vCheckRectPos = m_pkCheckRect->GetFixedWindowLocalPos();
		vCheckRectPos -= m_pkOverBlindRect->GetFixedWindowLocalPos();

		iStartXIndex = static_cast<int>((vCheckRectPos.x) * fXRate);
		iStartYIndex = static_cast<int>((vCheckRectPos.y) * fYRate);
		iEndXIndex = static_cast<int>((vCheckRectPos.x +m_pkCheckRect->GetWidth()* m_fWindowScaleX) * fXRate);
		iEndYIndex = static_cast<int>((vCheckRectPos.y +m_pkCheckRect->GetHeight()* m_fWindowScaleY) * fYRate);

		if ( GCFUNC::CheckColInRect( dwAccess , d3dDesc , iStartXIndex , iEndXIndex , iStartYIndex , iEndYIndex , APHADOT , 0.75f ) )
		{
			GCFUNC::FillTheCol(dwAccess , d3dDesc , APHADOT );
			m_eLotteryState = STATE_SHOW_EFFECT;
			m_btnAuto->Lock(true);
			m_dwFrameCnt = 0;
		}
	}
	
	d3dSurface->UnlockRect();
	d3dSurface->Release();
}

void KGCLottery::FrameMoveInEnabledState()
{
	for (int i=0 ; i<NUM_REWARD_DISPLAY_BOX ; i++)
	{
		if ( m_pkRewardDisplayBox[i] && !m_pkRewardDisplayBox[i]->IsNothing() && m_pkRewardDisplayBox[i]->CheckMousePosInWindowBound() )
		{
			D3DXVECTOR2 vecPos = m_pkRewardDisplayBox[i]->GetCurrentWindowPos();
			vecPos.x *= m_fWindowScaleX;
			vecPos.y *= m_fWindowScaleY;
			vecPos.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth() * g_pkUIScene->m_pkItemInformationBox->GetWindowScaleX();

			GCItem* pItem = g_pItemMgr->GetItemData(m_pkRewardDisplayBox[i]->GetItemID());
			if (pItem == NULL)
				break;
			pItem->iNeedLevelForEquippingItem = m_pkRewardDisplayBox[i]->GetEquipLevel();
			g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
			g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
			g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

			g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
			g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
			break;
		}
	}
	switch(m_eLotteryState)
	{
	case STATE_INIT:
		break;
	case STATE_MANUAL:
		if ( m_btnAuto->CheckMousePosInWindowBound())
			break;
		if ( g_pkInput->BtnPressed( KInput::MBLEFT ) )
			ScratchOnBoard(g_pkInput->GetMousePos());
		break;
	case STATE_AUTO:
		FrameMoveAutoScratch_();
		break;
	case STATE_SHOW_EFFECT:
		FrameMoveShowEffect_();
		break;
	}	
}

void KGCLottery::OnExit()
{
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCLottery::OnAuto()
{
	if (m_mapAutoScratchPos.empty())
		return;

	GCUTIL::GCRand_Int gcRandInt;
	m_iScratchType = gcRandInt.rand()%m_mapAutoScratchPos.size(); 

	if (m_mapAutoScratchPos.find(m_iScratchType) == m_mapAutoScratchPos.end())
	{
		m_eLotteryState = STATE_END;
		return;
	}
	m_eLotteryState = STATE_AUTO;
}

void KGCLottery::MakeAutoScratchPos_()
{
	std::vector<D3DXVECTOR2> vecPos;
	vecPos.push_back(D3DXVECTOR2(0.0f , 0.0f));
	vecPos.push_back(D3DXVECTOR2(1.0f , 0.0f));
	vecPos.push_back(D3DXVECTOR2(1.0f , 1.0f));
	vecPos.push_back(D3DXVECTOR2(0.0f , 1.0f));
	vecPos.push_back(D3DXVECTOR2(0.0f , 0.0f));
	vecPos.push_back(D3DXVECTOR2(1.0f , 1.0f));
	vecPos.push_back(D3DXVECTOR2(0.5f , 0.0f));
	vecPos.push_back(D3DXVECTOR2(0.5f , 1.0f));

	m_mapAutoScratchPos.insert(std::make_pair(0 , vecPos));
}

void KGCLottery::FrameMoveAutoScratch_()
{
	if( true == m_bAutoMode )
		m_btnYes->ToggleRender( false );

	LPDIRECT3DSURFACE9 d3dSurface;
	if ( FAILED(m_pTexOverBlind->GetDeviceTexture()->GetSurfaceLevel(0,&d3dSurface)) )
		return;

	D3DSURFACE_DESC d3dDesc;
	if ( FAILED(m_pTexOverBlind->GetDeviceTexture()->GetLevelDesc(0,&d3dDesc)) )
		return;

	D3DLOCKED_RECT d3dRect;
	if ( FAILED(d3dSurface->LockRect(&d3dRect , NULL , D3DLOCK_DISCARD)) )
		return;

	DWORD* dwAccess = static_cast<DWORD*>(d3dRect.pBits);

	GCFUNC::FillTheCol(dwAccess , d3dDesc , APHADOT );
	m_eLotteryState = STATE_SHOW_EFFECT;
	m_btnAuto->Lock(true);
	m_dwFrameCnt = 0;

	d3dSurface->UnlockRect();
	d3dSurface->Release();
}

void KGCLottery::SetCharSelectBox_()
{
	m_pkCharSelectBox->DeleteAllItem();
    m_mapCharType.clear();

	std::vector<SCharInfo>::iterator vit = g_kGlobalValue.m_kUserInfo.vecCharInfo.begin();

	for (int i=0 ; vit != g_kGlobalValue.m_kUserInfo.vecCharInfo.end() ; ++vit , i++)
	{
		m_pkCharSelectBox->AddString(GCFUNC::GetCharName(vit->iCharType));
		m_mapCharType.insert(std::make_pair(i,vit->iCharType));
	}

	if (m_iCharType == -1)
		return;

	std::map<int,char>::iterator mit = m_mapCharType.begin();
	for (; mit != m_mapCharType.end() ; ++mit)
	{
		if (mit->second == m_iCharType)
		{
			m_pkCharSelectBox->SetCurSel(mit->first);
			break;
		}
	}

}

void KGCLottery::OnYes()
{
	if (m_iCharType < 0 || m_iCharType > GC_CHAR_NUM)
		return;

	m_btnAuto->Lock(false);
	m_btnExit->Lock(true);
	if( false == m_bAutoMode )
		m_eLotteryState = STATE_MANUAL;
	else
		m_eLotteryState = STATE_AUTO;
	Result_BuyForLottery = INT_MAX;
	
	KP2P::GetInstance()->Send_GachaLotteryActionReq(m_iCharType);
	g_MyD3D->WaitForServerAck( Result_BuyForLottery, INT_MAX, 5000, TIME_OUT_VALUE);

	m_pkTitleQuestion->ToggleRender(false);
	m_pkCharSelectBox->Lock(true);
}

void KGCLottery::CreateRewardDisplayBox_()
{
	std::vector<KD3DWnd*> vecCreatedWnd = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_lottery.stg", "","lottery_dlg\\reward_box",true,NUM_REWARD_DISPLAY_BOX );
	std::vector<KD3DWnd*>::iterator vit = vecCreatedWnd.begin();
	for (int i=0 ; vit != vecCreatedWnd.end()&& i< NUM_REWARD_DISPLAY_BOX ; ++vit , ++i)
	{
		m_pkRewardDisplayBox[i] = static_cast<KGCLotteryRewardBox*>(*vit);
		m_pkRewardDisplayBox[i]->ToggleRender(true);
	}

	D3DXVECTOR2 vPos = m_pkRewardDisplayRect->GetFixedWindowLocalPos();
	DWORD dwRectWidth = m_pkRewardDisplayRect->GetWidth();
	DWORD dwBoxWidth = m_pkRewardDisplayBox[0]->GetWidth();

	float fGap = static_cast<float>(dwRectWidth - dwBoxWidth * NUM_REWARD_DISPLAY_BOX)/(NUM_REWARD_DISPLAY_BOX -1);

	for (int i=0 ; i<NUM_REWARD_DISPLAY_BOX ; i++)
	{
		m_pkRewardDisplayBox[i]->SetFixedWindowLocalPos(vPos);
		vPos.x += (fGap+dwBoxWidth);
	}
}

void KGCLottery::SetRewardDisplay_()
{
	m_btnLeft->Lock(true);
	m_btnRight->Lock(true);

	std::map< char, std::vector< std::pair< GCITEMID, int > > >::iterator mit = g_kGlobalValue.m_mapGachaSuccessList.find(m_iCharType);
	if (mit == g_kGlobalValue.m_mapGachaSuccessList.end())
	{
		for (int i=0 ; i<NUM_REWARD_DISPLAY_BOX ; i++)
			if (m_pkRewardDisplayBox[i])
				m_pkRewardDisplayBox[i]->SetNothing();
		return;
	}
	
	if ( mit->second.empty() )
	{
		for (int i=0 ; i<NUM_REWARD_DISPLAY_BOX ; i++)
			if (m_pkRewardDisplayBox[i])
				m_pkRewardDisplayBox[i]->SetNothing();
		return;
	}

	if ( mit->second.size() > NUM_REWARD_DISPLAY_BOX )
	{
		if (m_iFirstIndex != 0)
		{
			m_btnLeft->Lock(false);
		}

		if ( m_iFirstIndex + NUM_REWARD_DISPLAY_BOX < static_cast<int>(mit->second.size()) )
			m_btnRight->Lock(false);
	}

	for (int i=0 ; i<NUM_REWARD_DISPLAY_BOX ; i++)
	{
		int ivecIndex = i + m_iFirstIndex;
		if (m_pkRewardDisplayBox[i])
		{
			if (ivecIndex >= static_cast<int>(mit->second.size()))
				m_pkRewardDisplayBox[i]->SetNothing();
			else
				m_pkRewardDisplayBox[i]->SetGoods(mit->second[ivecIndex].first,mit->second[ivecIndex].second , ivecIndex);
		}
	}
}

void KGCLottery::OnLeft()
{
	if (m_iFirstIndex > 0)
		m_iFirstIndex--;

	SetRewardDisplay_();
}

void KGCLottery::OnRight()
{
	m_iFirstIndex++;

	SetRewardDisplay_();
}

void KGCLottery::SetShowImgAtSuccess( GCITEMID gcItemID_ )
{
	for (int i=0 ; i<NUM_SHOW_ITEM_IMG ; i++)
	{
		SAFE_RELEASE(m_pTexShowImg[i]);
		m_pTexShowImg[i] = g_pItemMgr->CreateShopItemTexture( gcItemID_ / 10 );
	}
}

void KGCLottery::SetShowImgAtFail()
{
	std::vector<KItem> tempPotionReward = m_vecTempRewardItem;

	for (int i=0 ; i<NUM_SHOW_ITEM_IMG ; i++)
		SAFE_RELEASE(m_pTexShowImg[i]);

	if ( !m_bAutoMode || tempPotionReward.empty() )
	{
		std::map< char, std::vector< std::pair< GCITEMID, int > > >::iterator mit = g_kGlobalValue.m_mapGachaSuccessList.find(m_iCharType);
		if (mit == g_kGlobalValue.m_mapGachaSuccessList.end())
		{
			return;
		}
		else
		{
			std::vector< std::pair< GCITEMID, int > > vecGCItem = mit->second;
			GCUTIL::GCRand_Int kRandom;
			std::vector< std::pair< GCITEMID, int > >::iterator vit;
			for (int i = 0; i < NUM_SHOW_ITEM_IMG; i++)
			{
				int iIndex = kRandom.rand() % vecGCItem.size();
				int iCnt = 0;
				for (vit = vecGCItem.begin(); iCnt < iIndex; ++vit)
					iCnt++;
				m_pTexShowImg[i] = g_pItemMgr->CreateShopItemTexture(vit->first / 10);
				vecGCItem.erase(vit);
			}
		}
	}
	else 
	{
		std::vector<KItem>::iterator vit = tempPotionReward.begin();
		for (; vit != tempPotionReward.end(); ++vit)
		{
			GCItem* pItem = g_pItemMgr->GetItemData(vit->m_ItemID / 10);
			if (NULL == pItem) {
				continue;
			}

			for (int i = 0; i < NUM_SHOW_ITEM_IMG; i++)
			{
				m_pTexShowImg[i] = g_pItemMgr->CreateShopItemTexture(pItem->dwGoodsID);
			}
		}
		tempPotionReward.clear();
	}
}

void KGCLottery::SetLotteryDuration( int iDurantion_ )
{
	m_pkStaticNumLottery->SetText(iDurantion_);
}

void KGCLottery::FrameMoveShowEffect_()
{
	if (m_dwFrammCntAtShowEffect == 0)
	{
		if (m_bSuccescc)
		{
			g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("Bingo_01",0.0f,0.0f,0.5f), GC_LAYER_UI);
			g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("Bingo_02",0.0f,0.0f,0.5f), GC_LAYER_UI);
			g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("Bingo_03",0.0f,0.0f,0.5f), GC_LAYER_UI);
			g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("Bingo_05",0.0f,0.0f,0.5f), GC_LAYER_UI);
			g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Gacha_Congratulation",0.0f,0.0f,0.5f), GC_LAYER_CUSTOM);
			g_KDSound.Play( "Enchant_Start" );
		}
		else
		{
			g_KDSound.Play( "Enchant_Fail" );
		}
	}

	m_dwFrammCntAtShowEffect++;

	if ( m_dwFrammCntAtShowEffect > GC_FPS_LIMIT*2)
	{
		if ( !m_vecTempRewardItem.empty())
		{
			std::vector<KItem>::iterator vit = m_vecTempRewardItem.begin();
			for (; vit != m_vecTempRewardItem.end() ; ++vit )
			{
				GCItem* pItem = g_pItemMgr->GetItemData( vit->m_ItemID / 10 );
				if( NULL == pItem ) { 
					continue;
				}

				g_pItemMgr->m_kInventory.AddItem( *vit, true );

				if( !m_bAutoMode )
				{
					std::wstring strMsg = g_pkStrLoader->GetReplacedString( STR_ID_ITEM_RECEIVE1, "l", pItem->strItemName );
					g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPETITION_EVENT_BOX, strMsg, L"", KGCUIScene::GC_MBOX_USE_NORMAL, pItem->dwGoodsID * 10, vit->m_ItemUID, true, true );
				}
			}
			m_vecTempRewardItem.clear();
			g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
			g_pkUIScene->m_pkMyInfoScene->UpdateData();
		}
		SetInitVailue();
	}
}

void KGCLottery::SetTempRewardItem( std::vector<KItem>* vecRewardItem_ )
{
	m_vecTempRewardItem = *vecRewardItem_;
}

void KGCLottery::SetInitVailue()
{
	SAFE_RELEASE(m_pTexOverBlind);
	m_pTexOverBlind = g_pGCDeviceManager2->CreateTexture( "scratch.dds" );

	if (m_iCharType < 0 || m_iCharType > GC_CHAR_NUM)
		m_iCharType = g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType;

	m_bHaveLottery = false;
	KItem* pLotteryItem = NULL;
	if ( g_pItemMgr->FindInventoryForItemID( ITEM_GACHA_LOTTERY , &pLotteryItem) )
	{
		m_pkStaticNumLottery->SetText(pLotteryItem->m_nCount);
		if (pLotteryItem->m_nCount > 0)
			m_bHaveLottery = true;
		else
			OnStopAuto();
	}
	else
	{
		m_pkStaticNumLottery->SetText(0);
		OnStopAuto();
	}

	if ( !m_bAutoMode )
		m_btnExit->Lock(false);

	m_eLotteryState = STATE_INIT;
	m_btnAuto->Lock(true);
	m_dwFrameCnt = 1;
	m_fAutoScratchCnt = 0.0f;

	MakeAutoScratchPos_();

	m_iScratchType = -1;

	m_pkCharSelectBox->InitState(true,true,this);
	m_pkCharSelectBox->Lock(false);
	SetCharSelectBox_();

	m_bNoTouch = true;

	if (m_bHaveLottery)
	{
		m_btnYes->ToggleRender(true);
	}
	else
	{
		m_btnYes->ToggleRender(false);
	}

	CreateRewardDisplayBox_();
	m_iFirstIndex = 0;
	SetRewardDisplay_();	

	m_bSuccescc = false;
	m_dwFrammCntAtShowEffect = 0;

	if ( m_bAutoMode )
		OnYes();
}

void KGCLottery::OnClickAuto(void)
{
	if ( m_pkCheckBoxAutoClear == NULL ) {
		return;
	}

	m_pkCharSelectBox->Lock( true );

	m_pkCheckBoxAutoClear->SetCheck( !m_pkCheckBoxAutoClear->GetCheck() );
	m_bAutoMode = m_pkCheckBoxAutoClear->GetCheck();
	if ( m_bAutoMode ) {
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_GACHA_AUTO2 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
	}
}

void KGCLottery::OnStopAuto(void)
{
	OnClickAuto();
	m_btnExit->Lock( false );

	m_pkCharSelectBox->Lock( false );

	g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
	g_pkUIScene->m_pkMyInfoScene->UpdateData();
}