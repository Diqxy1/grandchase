#include "stdafx.h"

#include "../MyD3D.h"
#include "GCUI/GCMyInfoScene.h"
#include "KGCMath.h"
#include "gcui/GCItemInfoBox.h"
#include "gcui/GCUINumber.h"
#include "gcui/KGCTreasureItemBox.h"
#include "GCUI/GCItemInformationBoxPair.h"
//#include "GCUI/GCHeroItemPreview.h"
#include "GCUI/KGCNewClearSealJobBtn.h"
#include "gcui/KGCGachaBuyBox.h"
#include "KGCFaceLift.h"
#include "KGCDepotManager.h"
#include "GCRenderManager.h"

#ifdef DELTAZOOM
#undef DELTAZOOM
#endif
#define DELTAZOOM   0.01f

#ifdef MAXZOOM
#undef MAXZOOM
#endif
#define MAXZOOM     0.32f

#ifdef MINZOOM
#undef MINZOOM
#endif
#define MINZOOM     0.25f

#ifdef DEFAULTZOOM
#undef DEFAULTZOOM
#endif
#define DEFAULTZOOM MAXZOOM

IMPLEMENT_CLASSNAME( KGCNewClearSealDlg );
IMPLEMENT_WND_FACTORY( KGCNewClearSealDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCNewClearSealDlg, "gc_clear_seal_dlg_new" );

KGCNewClearSealDlg::KGCNewClearSealDlg()
: m_iCurCharType( 0 )
, m_iCurBuyItem( 0 )
, m_iCurCharJob( 0 )
, m_iCurBeadItemNum( 0 )
, m_iCurSpellItemNum( 0 )
, m_iParticleLife( 0 )
, m_iFrameCount_ItemImage( 0 )
, m_iGachaKeyBuyCount( 0 )

, m_iNeedSpellItemNum( 1 )
, m_iCurItemLevel( 0 )

, m_bSuccess( false )
, m_bGetAll( false )
, m_bGetEquipAll( false )
, m_bGetAccessoriesAll( false )
, m_bPaticleInit( false )
, m_bRewardEquipItem( false )

, m_pEffTextAlert( NULL )
, m_pkGetItemImage( NULL )

, m_pkJobCombo( NULL )
, m_pkImgBeadItem( NULL )
, m_pkImgSealItem( NULL )
, m_pkImgTakeEquip( NULL )
, m_pkImgTakeAcc( NULL )
, m_pkStaticBeadItem( NULL )
, m_pkStaticSealItem( NULL )
, m_pkBtnPetView( NULL )
, m_pkBtnBuy( NULL )
, m_pkBtnClose( NULL )
, m_pkBtnTakeEquip( NULL )
, m_pkBtnTakeAcc( NULL )
, m_pkBtnClearSealEquip( NULL )
, m_pkBtnClearSealAcc( NULL )
, m_pkStaticClearSealDesc( NULL )
, m_pkBackground( NULL )
, m_pkWndTakeEquipOver( NULL )
, m_pkWndTakeAccOver( NULL )

, m_pMeshFootStage( NULL )
, m_pShopMotion( NULL )
, m_pkBackTexture( NULL )
, m_iMotionCount( 0 )
, m_cFrame( 0 )
, m_fCharX( 0.0f )
, m_fAngleMove( 0.0f )
, m_fStageAngleMove( 0.0f )
, m_fZoom( 0.25 )
, m_bAngleChanged( false )
, m_iExtraCharNum( 0 )
, m_iCurrentCharacter( 0 )
, m_bShowDefault( false )
, m_bAutoMode( false )
, m_pkCheckBoxAutoClear( NULL )
, m_pkGachaBuyItemBox( NULL )
, m_iGachaVer( 0 )
{

	LINK_CONTROL( "background", m_pkBackground );

	LINK_CONTROL( "img_item_bead",     m_pkImgBeadItem );
	LINK_CONTROL( "img_item_seal",     m_pkImgSealItem );
	LINK_CONTROL( "static_bead_num",   m_pkStaticBeadItem );
	LINK_CONTROL( "static_seal_num",   m_pkStaticSealItem );
	LINK_CONTROL( "state_speech",      m_pkStaticClearSealDesc );

	//Equipment gacha
	LINK_CONTROL( "static_select_item_level", m_pkStaticSelectedItemLevel );
	LINK_CONTROL( "btn_select_level",           m_pkBtnSelectItemLevel );

	char szTemp[256];
	for(int iLoop=0; iLoop<EREWORD_ITEM_NUM; ++iLoop)
	{
		m_pkImgRewordEquip[iLoop] = NULL;
		sprintf(szTemp, "img_equip%d", iLoop);
		LINK_CONTROL( szTemp, m_pkImgRewordEquip[iLoop] );

		m_pkWndRewordEquipOver[iLoop] = NULL;
		sprintf(szTemp, "wnd_equipover%d", iLoop);
		LINK_CONTROL( szTemp, m_pkWndRewordEquipOver[iLoop] );

		if( iLoop<EREWORD_ACC_NUM )
		{
			m_pkImgRewordAcc[iLoop] = NULL;
			sprintf(szTemp, "img_acc%d", iLoop);
			LINK_CONTROL( szTemp, m_pkImgRewordAcc[iLoop] );

			m_pkWndRewordAccOver[iLoop] = NULL;
			sprintf(szTemp, "wnd_accover%d", iLoop);
			LINK_CONTROL( szTemp, m_pkWndRewordAccOver[iLoop] );
		}
	}

	LINK_CONTROL( "img_take_equip", m_pkImgTakeEquip );
	LINK_CONTROL( "img_take_acc", m_pkImgTakeAcc );
	LINK_CONTROL( "btn_take_equip", m_pkBtnTakeEquip );
	LINK_CONTROL( "btn_take_acc", m_pkBtnTakeAcc );
	LINK_CONTROL( "btn_clear_seal_equip", m_pkBtnClearSealEquip );
	LINK_CONTROL( "btn_clear_seal_acc", m_pkBtnClearSealAcc );
	LINK_CONTROL( "btn_close", m_pkBtnClose );

	LINK_CONTROL( "popupbox_level", m_pkPopupMenuItemLevel );

	LINK_CONTROL( "wnd_take_equipover", m_pkWndTakeEquipOver );
	LINK_CONTROL( "wnd_take_accover", m_pkWndTakeAccOver );

	LINK_CONTROL( "combo_promotion", m_pkJobCombo );
	LINK_CONTROL( "checkbox_auto", m_pkCheckBoxAutoClear );

	LINK_CONTROL( "btn_buy", m_pkBtnBuy );
	LINK_CONTROL( "buy_item_box", m_pkGachaBuyItemBox);
	LINK_CONTROL( "btn_pet_view", m_pkBtnPetView );
	LINK_CONTROL( "static_msg", m_pkStaticMsg );

	m_mapSealBead.clear();
	m_mapBuyItemList.clear();
	m_vecCurBuyItemList.clear();
	m_setObtainedBeadList.clear();
	m_vecFinalRewardEuipItems.clear();
	m_mapObtainedBeadList.clear();

#if defined( NATION_KOREA )
	LoadGachaKeyList();
#endif

	m_pkGetItemImage = new KGC2DObject();

	IsProccessedGacha = false;
}

KGCNewClearSealDlg::~KGCNewClearSealDlg()
{
	if( m_pkGetItemImage )
	{
		SAFE_DELETE( m_pkGetItemImage );
	}
}

void KGCNewClearSealDlg::OnCreateComplete( void )
{
	m_mapSealBead.clear();
	std::map<DWORD,int> mapGachaList = g_pItemMgr->GetGachaList();
	for(std::map<DWORD,int>::iterator mIter = mapGachaList.begin();mIter != mapGachaList.end();mIter++)
	{
		m_mapSealBead.insert(std::pair<int,DWORD>(mIter->second,mIter->first));
	}

	if( m_pkBackground )
	{
		m_pkBackground->InitState( true );
		m_pkBackground->SetPredraw( true );
	}

	if( m_pkJobCombo )
	{
		m_pkJobCombo->InitState( true, true, this );
		m_pkJobCombo->SetAlign( DT_CENTER );
		m_pkJobCombo->SetComboBoxColor( 0xffffda4e, 0xffffffff );
	}

	if( m_pkImgBeadItem )
		m_pkImgBeadItem->InitState( true );
	if( m_pkImgSealItem )
		m_pkImgSealItem->InitState( true );
	if( m_pkImgTakeEquip )
		m_pkImgTakeEquip->InitState( true );
	if( m_pkImgTakeAcc )
		m_pkImgTakeAcc->InitState( true );
	if( m_pkWndTakeEquipOver )
		m_pkWndTakeEquipOver->InitState( true );
	if( m_pkWndTakeAccOver )
		m_pkWndTakeAccOver->InitState( true );

	if( m_pkStaticBeadItem )
	{
		m_pkStaticBeadItem->InitState( true );
		m_pkStaticBeadItem->SetAlign(DT_RIGHT);
		m_pkStaticBeadItem->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
	}

	if( m_pkStaticSealItem )
	{
		m_pkStaticSealItem->InitState( true );
		m_pkStaticSealItem->SetAlign(DT_RIGHT);
		m_pkStaticSealItem->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
	}

	for(int iLoop=0; iLoop<EREWORD_ITEM_NUM; ++iLoop)
	{
		if( m_pkImgRewordEquip[iLoop] )
			m_pkImgRewordEquip[iLoop]->InitState( false );
		if( m_pkWndRewordEquipOver[iLoop] )
			m_pkWndRewordEquipOver[iLoop]->InitState( false );

		if( iLoop<EREWORD_ACC_NUM )
		{
			if( m_pkImgRewordAcc[iLoop] )
				m_pkImgRewordAcc[iLoop]->InitState( false );
			if( m_pkWndRewordAccOver[iLoop] )
				m_pkWndRewordAccOver[iLoop]->InitState( false );
		}
	}

	if( m_pkBtnPetView )
	{
		m_pkBtnPetView->InitState( true, true, this );
		m_pkBtnPetView->ConnectEventProcedure<KGCNewClearSealDlg, void>( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCNewClearSealDlg::OnClickPetView );
		m_pkBtnPetView->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCNewClearSealDlg::OnEnterCursor );
	}

	if( m_pkBtnClose )
	{
		m_pkBtnClose->InitState( true, true, this );
		m_pkBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCNewClearSealDlg::OnExit );
		m_pkBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCNewClearSealDlg::OnEnterCursor );
	}

	if( m_pkBtnTakeEquip )
	{
		m_pkBtnTakeEquip->InitState( true, true, this );
		m_pkBtnTakeEquip->Lock( true );
		m_pkBtnTakeEquip->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCNewClearSealDlg::OnRecvFinalReward );
	}

	if( m_pkBtnTakeAcc )
	{
		m_pkBtnTakeAcc->InitState( true, true, this );
		m_pkBtnTakeAcc->Lock( true );
		m_pkBtnTakeAcc->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCNewClearSealDlg::OnRecvFinalTotalReward );
	}

	if( m_pkBtnClearSealEquip )
	{
		m_pkBtnClearSealEquip->InitState( true, true, this );
		m_pkBtnClearSealEquip->ConnectEventProcedure<KGCNewClearSealDlg, void>( KD3DWnd::D3DWE_BUTTON_CLICK, this, boost::bind( &KGCNewClearSealDlg::OnClickClearSealButton, _1, static_cast<DWORD>(ETYPE_EQUIP_ITEM) ) );
		m_pkBtnClearSealEquip->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCNewClearSealDlg::OnEnterCursor );
	}

	if( m_pkBtnClearSealAcc )
	{
		m_pkBtnClearSealAcc->InitState( true, true, this );
		m_pkBtnClearSealAcc->ConnectEventProcedure<KGCNewClearSealDlg, void>( KD3DWnd::D3DWE_BUTTON_CLICK, this, boost::bind( &KGCNewClearSealDlg::OnClickClearSealButton, _1, static_cast<DWORD>(ETYPE_ACCESSORY_ITEM) ) );
		m_pkBtnClearSealAcc->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCNewClearSealDlg::OnEnterCursor );
	}

	if( m_pkStaticClearSealDesc )
	{
		m_pkStaticClearSealDesc->InitState(true);
		m_pkStaticClearSealDesc->SetFontColor(D3DCOLOR_RGBA(255,255,255,255));
		m_pkStaticClearSealDesc->SetAlign(DT_CENTER);
		m_pkStaticClearSealDesc->SetTextAutoMultiline(g_pkStrLoader->GetString( STR_ID_COORDI_GACHA_SYSTEM_MESSAGE01 )); //구슬까기 설명 스트링
	}

	if( m_pkBtnBuy )
	{
		m_pkBtnBuy->InitState(true, true, this);
		m_pkBtnBuy->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCNewClearSealDlg::OnClickBuy );
	}

	if( m_pkGachaBuyItemBox )
	{
		m_pkGachaBuyItemBox->InitState( false, true, this);
	}

	if( m_pkStaticSelectedItemLevel )
	{
		m_pkStaticSelectedItemLevel->InitState(true, true, this);
	}

	if( m_pkBtnSelectItemLevel )
	{
		m_pkBtnSelectItemLevel->InitState(true, true, this);
	}

	if( m_pkPopupMenuItemLevel )
	{
		m_pkPopupMenuItemLevel->InitState( false, true, this );
	}

	if( m_pkStaticMsg )
	{
		m_pkStaticMsg->InitState(true);
		m_pkStaticMsg->SetFontColor(D3DCOLOR_RGBA(255,255,255,255));
		m_pkStaticMsg->SetAlign(DT_CENTER);
		m_pkStaticMsg->SetTextAutoMultiline(g_pkStrLoader->GetString( STR_ID_COORDI_GACHA_SYSTEM_MESSAGE10 )); //구슬까기 설명 스트링
	}

	m_pkCheckBoxAutoClear->InitState( true, true, this );
	m_pkCheckBoxAutoClear->SetCheck( m_bAutoMode );
	m_pkCheckBoxAutoClear->DisconnectAllEventProcedure();
	m_pkCheckBoxAutoClear->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCNewClearSealDlg::OnClickAuto );

	InitializeData();
}

void KGCNewClearSealDlg::OnDestroy( void )
{
	ClearPaticle();
	m_iCurCharType = 0;
	DeleteMeshObject();
	SAFE_RELEASE( m_pkBackTexture );
}

void KGCNewClearSealDlg::PostDraw( void )
{
	RenderPreview();
}

void KGCNewClearSealDlg::PostChildDraw( void )
{
	g_ParticleManager->Render(GC_LAYER_GACHA);
	if( m_pkGetItemImage != NULL )
	{
		m_pkGetItemImage->Render();
	}
}

void KGCNewClearSealDlg::FrameMoveInEnabledState( void )
{
	if ( g_pkInput->IsDown( DIK_ESCAPE ) && g_pkUIScene->IsCashBoxRenderOn() == false)
	{
		if( 0 >= m_iParticleLife )
			OnExit();

		g_pkInput->IgnoreDown(DIK_ESCAPE,30);
	}

	if( m_pkPopupMenuItemLevel->IsRenderOn() )
	{
		return;
	}

	if( 0 < m_iParticleLife )
	{
		m_pkBtnClearSealEquip->Lock(true);
		m_pkBtnClearSealAcc->Lock(true);
		m_pkJobCombo->Lock(true);

		if (!IsProccessedGacha) {
			m_iParticleLife = 200;
			IsProccessedGacha = true;
		}

		if( 0 == --m_iParticleLife && false == m_wstrMsg.empty() )
		{
			if( m_bSuccess )
				g_KDSound.Play( "GACHA_SUCCESS" );
			else
				g_KDSound.Play( "GACHA_FAILED" );

			if( false == m_bAutoMode ) { 
				g_pkUIScene->m_pkTreaureItemBox->SetContentAutoMultiLine(m_wstrMsg);
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_TREASURE_ITEM_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
			}

			m_wstrMsg.clear();

			if( false == m_bAutoMode ) { 
				Init_ItemImage();
				g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
				g_pkUIScene->m_pkMyInfoScene->UpdateData(); 
				UpdateOwnItemList();
			}
			else { 
				UpdateOwnItemList();
				UpdateBuyItemList();
			}
		}
		else
		{
			if( !m_wstrMsg.empty() )
				FrameMove_ItemImage();
		}

		if( m_bAutoMode && m_iParticleLife == 0 ) { 
			RollGacha();
		}
	}
	else
	{
		if (!m_bAutoMode) {
			m_pkBtnClearSealEquip->Lock(false);
			m_pkBtnClearSealAcc->Lock(false);
			m_pkJobCombo->Lock(false);
		}
	}

	// 아이템 정보Box 처리
	bool bCheck = false;
	const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );
	for(int iLoop=0; iLoop<EREWORD_ITEM_NUM; ++iLoop )
	{
		// 보상아이템 정보Box 처리
		if( m_pkImgRewordEquip[iLoop]->CheckPosInWindowBound( vMousePos ) && int(m_vecRewardEuipItems.size()) > iLoop )
		{
			D3DXVECTOR2 vecPos =m_pkImgRewordEquip[iLoop]->GetCurrentWindowPos();
			RenderItemInfoBox( vecPos, m_vecRewardEuipItems[iLoop], m_iCurItemLevel );
			bCheck = true;
			break;
		}

		// 액세서리 아이템 정보Box 처리
		else if( iLoop < EREWORD_ACC_NUM && m_pkImgRewordAcc[iLoop]->CheckPosInWindowBound( vMousePos ) && int(m_vecRewardAccItems.size()) > iLoop )
		{
			D3DXVECTOR2 vecPos =m_pkImgRewordAcc[iLoop]->GetCurrentWindowPos();
			RenderItemInfoBox( vecPos, m_vecRewardAccItems[iLoop], m_iCurItemLevel );
			bCheck = true;
			break;
		}
	}    

	// 장비선택 아이템 정보Box 처리
	if( m_pkImgTakeEquip->CheckPosInWindowBound( vMousePos ) )
	{
		if( m_iCurCharType >= 0 && m_iCurCharType < GC_CHAR_NUM )
		{
			D3DXVECTOR2 vecPos = m_pkImgTakeEquip->GetCurrentWindowPos();
			RenderItemInfoBox( vecPos, m_pkImgTakeEquip->GetItemID(), m_iCurItemLevel );
			bCheck = true;
		}        
	}

	// 최종보상 아이템 정보Box 처리
	if( m_pkImgTakeAcc->CheckPosInWindowBound( vMousePos ) )
	{
		D3DXVECTOR2 vecPos = m_pkImgTakeAcc->GetCurrentWindowPos();
		RenderItemInfoBox(vecPos, m_pkImgTakeAcc->GetItemID(), m_iCurItemLevel );
		bCheck = true;
	}

	if( !bCheck )
	{
		g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
	}

	FrameMove_Preview();
}

void KGCNewClearSealDlg::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == D3DWE_COMBO_SELCHANGE )
	{
		UpdatePromotion( m_pkJobCombo->GetCurSel() );
	}

	if( event.m_dwCode == KGCMultiMenu::K3DWE_MENU_CLICKED )
	{
		UpdateItemLevel( static_cast<int>(event.m_dwlParam) );
	}

	if( event.m_pWnd == m_pkBtnSelectItemLevel && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ||
		event.m_pWnd == m_pkStaticSelectedItemLevel && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		if(m_pkPopupMenuItemLevel->IsRenderOn() )
		{
			m_pkPopupMenuItemLevel->ToggleRender( false );
		}
		else
		{
			m_pkPopupMenuItemLevel->ToggleRender( true );
		}
		g_KDSound.Play( "31" );
	}
}

void KGCNewClearSealDlg::RenderItemInfoBox( D3DXVECTOR2& vPos_, GCITEMID dwItemID_, int iLevel_ )
{
	if( m_pkGachaBuyItemBox->IsRenderOn() )
	{
		return;
	}

	vPos_.x *= m_fWindowScaleX;
	vPos_.y *= m_fWindowScaleY;
	vPos_.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth() * g_pkUIScene->m_pkItemInformationBox->GetWindowScaleX();

	GCItem* pItem = g_pItemMgr->GetItemData(dwItemID_);
	KItem* pKItem =g_pItemMgr->m_kInventory.FindItemByItemIdAndLevel(dwItemID_, iLevel_);

	if( pKItem != NULL )
	{
		iLevel_ = static_cast<int>(pKItem->m_sEquipLevel);
	}

	g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem, -1, NULL, NULL, NULL, -1, iLevel_ );
	g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
	g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

	g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vPos_ );
	g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
}

void KGCNewClearSealDlg::OnExit()
{
	if( 0 < m_iParticleLife )
		return;

	ClearPaticle();
	m_iCurCharType = 0;
	m_iCurBuyItem = 0;
	m_iCurCharJob = 0;
	m_mapSealBead.clear();
	m_mapBuyItemList.clear();
	m_vecCurBuyItemList.clear();
	m_setObtainedBeadList.clear();
	m_vecFinalRewardEuipItems.clear();

	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCNewClearSealDlg::OnClickBuy()
{
	if( 0 < m_iParticleLife )
		return;

	m_pkGachaBuyItemBox->SetItems(m_vecCurBuyItemList);
	m_pkGachaBuyItemBox->ToggleRender(true);
	m_pkGachaBuyItemBox->SetOrder_Top();
}

void KGCNewClearSealDlg::OnClickPetView()
{
	if( 0 < m_iParticleLife )
		return;

	g_KDSound.Play( "31" );

	int iPetID = m_pkImgTakeAcc->GetItemID();
	if (g_pItemMgr->IsPackageItemID(iPetID * 10))
	{
		std::vector< KDropItemInfo > vecList;
		g_pItemMgr->GetPackageElements(iPetID * 10, vecList);

		for (auto itPackage : vecList)
		{
			GCItem* pPackaceElement = g_pItemMgr->GetItemData(itPackage.m_ItemID / 10);
			if (nullptr == pPackaceElement)
			{
				continue;
			}

			if (pPackaceElement->eItemKind == GIK_PET)
			{
				iPetID = g_kGlobalValue.GetPetIDFromItemID(pPackaceElement->dwGoodsID);
				break;
			}
		}
	}
	else
	{
		iPetID = g_kGlobalValue.GetPetIDFromItemID(m_pkImgTakeAcc->GetItemID());
	}

	g_pkUIScene->MessageBox( KGCUIScene::GC_PET_PREVIEW, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
	g_pkUIScene->m_pkPetPreview->SetGachaPreview();
	g_pkUIScene->m_pkPetPreview->SetDescState( 1 );
	g_pkUIScene->m_pkPetPreview->SetNowPet( iPetID, 0 ); 
}


void KGCNewClearSealDlg::OnEnterCursor()
{
	g_KDSound.Play( "30" );
}

void KGCNewClearSealDlg::OnClickClearSealButton( DWORD dwType )
{
	//#if !defined(__PATH__)

	if( m_pkGachaBuyItemBox->IsRenderOn() )
	{
		return;
	}

	m_dwGachaItemType = dwType;

	if( m_pkCheckBoxAutoClear->GetCheck() )
	{
		if( false ==g_MyD3D->m_kItemMgr.CheckExtraInventoryExist() )
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL_ERROR0), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false,true );
			m_pkCheckBoxAutoClear->SetCheck(false);
			return;
		}
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_GACHA_AUTO3 ), L"", KGCUIScene::GC_MBOX_USE_GACHA_AUTO, 0, 0, true, true );
	}
	else
		//#endif
	{
		if( 0 < m_iParticleLife )
			return;

		//         if( m_dwGachaItemType == 0 ) {   // 장비 일 때만 
		//             m_pkJobCombo->SetCurSel( 0 );
		//         }

		m_dwGachaItemType = dwType;
		RollGacha();
	}
}

void KGCNewClearSealDlg::RollGacha()
{
	IsProccessedGacha = false;
	bool bCheckAvailable = true;
	if( m_iCurBeadItemNum <= 0 )
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NOT_ENOUGH_BEAD_ITEM ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
		bCheckAvailable = false;
	}
	else if (m_iNeedSpellItemNum > m_iCurSpellItemNum )
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NOT_ENOUGH_CLEAR_SPELL_ITEM ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
		bCheckAvailable = false;
	}
	else if( m_bGetAll )
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_GACHA_AUTO4 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
		bCheckAvailable = false;
	}
	else if( false ==g_MyD3D->m_kItemMgr.CheckExtraInventoryExist() )
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL_ERROR0), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false,true );
		bCheckAvailable = false;
	}

	if( bCheckAvailable == false ) { 
		//#if !defined( __PATH__ )
		OnStopAuto();
		//#endif
		return;
	}

	KP2P::GetInstance()->Send_GachaLevelActionReq(m_pkImgBeadItem->GetItemID(), m_iCurItemLevel, m_dwGachaItemType, static_cast<char>(m_iCurCharType), m_iGachaVer );
}

bool KGCNewClearSealDlg::IsOpenUI()
{
	m_mapSealBead.clear();
	std::map<DWORD,int> mapGachaList = g_pItemMgr->GetGachaList();
	for(std::map<DWORD,int>::iterator mIter = mapGachaList.begin();mIter != mapGachaList.end();mIter++)
	{
		m_mapSealBead.insert(std::pair<int,DWORD>(mIter->second,mIter->first));
	}

	std::map<int, DWORD>::iterator mapIter = m_mapSealBead.begin();
	for(int iInsert=0; mapIter != m_mapSealBead.end(); ++mapIter)
	{
		if(g_pItemMgr->FindInventoryForItemID(mapIter->second) == true )
		{
			break;
		}
	}

	if( mapIter != m_mapSealBead.end() )
	{
		return true;
	}
	else
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_COORDI_GACHA_SYSTEM_MESSAGE05 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
		return false;
	}
}

void KGCNewClearSealDlg::InitializeData()
{
	static bool bFirstLoad = true;
	if( bFirstLoad )
	{
		InitPreview();

		m_mapSealBead.clear();
		std::map<DWORD, int> mapGachaList = g_pItemMgr->GetGachaList();
		for(std::map<DWORD, int>::iterator mapCachaIter = mapGachaList.begin(); mapCachaIter != mapGachaList.end(); ++mapCachaIter)
		{
			m_mapSealBead.insert( std::pair<int,DWORD>(mapCachaIter->second, mapCachaIter->first) );
		}

		if( false == LoadScriptBuyItemList() )
		{
			ASSERT( !"Gacha Script Load Error!!" );
		}
	}

	if( m_mapObtainedBeadList.empty() )
		KP2P::GetInstance()->Send_ObtainedGachaLevelSetRewardList();

	int iCharType = g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType;
	std::map<int, DWORD>::iterator mapIter = m_mapSealBead.find( iCharType );

	if ( mapIter != m_mapSealBead.end() )
	{
		if(g_pItemMgr->FindInventoryForItemID(mapIter->second) == false )
		{
			OnExit();
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
				g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_CASH_FAIL_TITLE),
				g_pkStrLoader->GetString(STR_GACHA_NO_SEAL_WARNING), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true);
			return;
		}
	}
	else
	{
		OnExit();
		return;
	}

	UpdateCharacter( iCharType );

	m_wstrMsg.clear();
}

void KGCNewClearSealDlg::InitPromotion( int iCharType )
{
	// 전직ComboBox에 String등록
	m_pkJobCombo->DeleteAllItem();
	m_pkJobCombo->AddString( g_pkStrLoader->GetString(STR_ID_PROMOTION_4), DT_CENTER );
	std::set< char > setPromotion;
	g_kGlobalValue.GetOpenedPromotion( iCharType, setPromotion );
	for( std::set< char >::iterator sit = setPromotion.begin(); sit != setPromotion.end(); ++sit ) {
		m_pkJobCombo->AddString( g_pkStrLoader->GetString(STR_ID_PROMOTION_0+static_cast< int >( *sit ) ), DT_CENTER );
	}
}

void KGCNewClearSealDlg::UpdateCharacter(int iIndex)
{
	if( 0 < m_iParticleLife )
		return;

	// 장비획득정보 초기화
	m_bGetAll = false;
	m_bGetEquipAll = false;
	m_bGetAccessoriesAll= false;

	// 캐릭터 선택
	SelCharToCharType( iIndex );

	if( 0 > m_iCurCharType || m_iCurCharType > GC_CHAR_NUM )
		return;

	// 보유 아이템 갱신
	if( false == m_mapSealBead.empty() )
		UpdateOwnItemList();

	// 판매 아이템 갱신
	if( false == m_mapBuyItemList.empty() )
		UpdateBuyItemList();

	// 전직초기화
	InitPromotion( m_iCurCharType );

	// 무기보상을 획득했다면 획득한 전직으로 선택해주자
	std::vector< DWORD >::iterator vecIter = m_vecFinalRewardEuipItems.begin();
	for(int iLoop=0; vecIter != m_vecFinalRewardEuipItems.end(); ++vecIter, ++iLoop)
	{
		if( g_pItemMgr->FindInventoryForItemID( *vecIter ) )
		{
			m_pkJobCombo->SetCurSel( iLoop+1 );
			break;
		}
	}
	if( vecIter == m_vecFinalRewardEuipItems.end() )
	{
		m_pkJobCombo->SetCurSel( 0 );
	}
}

void KGCNewClearSealDlg::SelCharToCharType(int iCharType)
{
	m_iCurCharType = iCharType;
}

void KGCNewClearSealDlg::UpdatePromotion(int iIndex)
{
	if( 0 < m_iParticleLife )
		return;

	int iJobIndex = (iIndex-1);
	if( 0 > iJobIndex )
	{
		iJobIndex = 0;
	}

	if( iJobIndex >= int(m_vecFinalRewardEuipItems.size()) )
		iJobIndex = int(m_vecFinalRewardEuipItems.size()) - 1;

	m_pkImgTakeEquip->SetItemInfo(m_vecFinalRewardEuipItems[iJobIndex]);
	if( m_bGetEquipAll )
	{
		//m_pkBtnTakeEquip->ToggleRender( true );
		m_pkBtnClearSealEquip->ToggleRender( false );
		if( 0 == iIndex )
		{
			m_pkBtnTakeEquip->Lock( true );
			m_pkBtnTakeEquip->SetToolTip( g_pkStrLoader->GetString( STR_ID_COORDI_GACHA_TOOLTIP01 ) );
			if( m_setObtainedBeadList.end() == m_setObtainedBeadList.find( std::make_pair(m_pkImgBeadItem->GetItemID(), ETYPE_EQUIP_ITEM) ) )
				m_pkStaticClearSealDesc->SetTextAutoMultiline(g_pkStrLoader->GetString( STR_ID_COORDI_GACHA_SYSTEM_MESSAGE02 ));
		}
		else
		{
			// 최종보상을 획득했다면 Lock
			if( m_setObtainedBeadList.end() != m_setObtainedBeadList.find( std::make_pair(m_pkImgBeadItem->GetItemID(), ETYPE_EQUIP_ITEM) ) )
			{
				m_pkBtnTakeEquip->Lock( true );
				m_pkBtnTakeEquip->ClearToolTip();
			}
			else
			{
				m_pkBtnTakeEquip->Lock( false );
				m_pkBtnTakeEquip->ClearToolTip();
				m_pkStaticClearSealDesc->SetTextAutoMultiline(g_pkStrLoader->GetString( STR_ID_COORDI_GACHA_SYSTEM_MESSAGE03 ));
			}
		}
	}
	else
	{
		//m_pkBtnTakeEquip->ToggleRender( false );
		m_pkBtnClearSealEquip->ToggleRender( true );
	}

	m_iCurCharJob = iJobIndex;

	ResetPlayer();

	SiKGCFaceLift()->SetCurMotionName( m_pWaitMotion[m_iExtraCharNum]->GetDeviceID() );
}

void KGCNewClearSealDlg::UpdateOwnItemList()
{
	KItem* pkInvenItem = NULL;
	std::map<int,DWORD>::iterator mapIter = m_mapSealBead.find( m_iCurCharType );
	if( mapIter != m_mapSealBead.end() )
	{
		m_pkImgBeadItem->SetItemInfo( mapIter->second );
		if( g_pItemMgr->FindInventoryForItemID(mapIter->second, &pkInvenItem) )
		{
			m_pkStaticBeadItem->SetText( pkInvenItem->m_nCount );
			m_iCurBeadItemNum = pkInvenItem->m_nCount;
		}
		else
		{
			m_pkStaticBeadItem->SetText( 0 );
		}

		// 보상장비 갱신
		UpdateRewardList( mapIter->second );

		// 최종보상 갱신
		UpdateFinalRewardList( mapIter->second );
	} 

	m_pkImgSealItem->SetItemInfo(KGCItemManager::ITEM_CASH_CLEAR_SPELL_ITEM);
	if(g_pItemMgr->FindInventoryForItemID(KGCItemManager::ITEM_CASH_CLEAR_SPELL_ITEM, &pkInvenItem))
	{
		m_pkStaticSealItem->SetText( pkInvenItem->m_nCount );
		m_iCurSpellItemNum = pkInvenItem->m_nCount;
	}
	else
		m_pkStaticSealItem->SetText( 0 );
}

void KGCNewClearSealDlg::SetMapGachaFinalRewardList( GCITEMID ItemID_, std::map<int, std::vector<GCITEMID>>& mapRewardItemList_ )
{
	m_mapStoredFinalRewardInfo[ItemID_] = mapRewardItemList_;
}

void KGCNewClearSealDlg::UpdateFinalRewardList(GCITEMID ItemID_)
{
	if( g_kGlobalValue.IsSingleGacha() ) { 
		std::map< DWORD, std::map<int, std::vector<GCITEMID>> >::iterator mit = m_mapStoredFinalRewardInfo.find(ItemID_);
		if( m_mapStoredFinalRewardInfo.end() != mit && !( mit->second.empty() ) )
		{
			SetFinalRewardtItemList( mit->second );
		}
		else
		{
			KP2P::GetInstance()->Send_GachaSetRewardListReq( m_iGachaVer, ItemID_ * 10 );

			Result_Gacha_Reward_List = INT_MAX;
			g_MyD3D->WaitForServerAck(Result_Gacha_Reward_List, INT_MAX, 5000, NULL);
		}
	}
	else { 
		if( g_pkUIScene->m_pkSuperGachaDlg ) { 
			std::map< int, std::vector< GCITEMID > > mapRewardSet_;
			if( g_pkUIScene->m_pkSuperGachaDlg->GetRewardSetMap( m_iGachaVer, m_iCurCharType, mapRewardSet_ ) ) { 
				SetMapGachaFinalRewardList( ItemID_, mapRewardSet_ );
				SetFinalRewardtItemList( mapRewardSet_ );
			}
			else { 
				KP2P::GetInstance()->Send_GachaSetRewardListReq( m_iGachaVer, ItemID_ * 10 );

				Result_Gacha_Reward_List = INT_MAX;
				g_MyD3D->WaitForServerAck(Result_Gacha_Reward_List, INT_MAX, 5000, NULL);
			}
		}
		else { 
			KP2P::GetInstance()->Send_GachaSetRewardListReq( m_iGachaVer, ItemID_ * 10 );

			Result_Gacha_Reward_List = INT_MAX;
			g_MyD3D->WaitForServerAck(Result_Gacha_Reward_List, INT_MAX, 5000, NULL);
		}
	}
}

void KGCNewClearSealDlg::UpdateBuyItemList()
{
	DWORD dwBeadItemID = m_pkImgBeadItem->GetItemID();
	std::map<DWORD, std::vector<DWORD> >::iterator mapBeadItemIter = m_mapBuyItemList.find( dwBeadItemID );
	if( mapBeadItemIter != m_mapBuyItemList.end() )
	{
		m_vecCurBuyItemList = mapBeadItemIter->second;
	}
}

void KGCNewClearSealDlg::UpdateRewardList(GCITEMID ItemID_)
{
	if( g_kGlobalValue.IsSingleGacha() ) {
		std::map< DWORD, std::map<int, std::set<GCITEMID>> >::iterator mit = m_mapStoredRewardInfo.find(ItemID_);
		if( m_mapStoredRewardInfo.end() != mit && !( mit->second.empty() ) )
		{
			SetRewardItemList( mit->second );
		}
		else
		{
			KP2P::GetInstance()->Send_GachaRewardListReq( m_iGachaVer, ItemID_ * 10 );

			Result_Gacha_Reward_List = INT_MAX;
			g_MyD3D->WaitForServerAck(Result_Gacha_Reward_List, INT_MAX, 5000, NULL);
		}
	}
	else { 
		if( g_pkUIScene->m_pkSuperGachaDlg ) { 
			std::map< int, std::set< GCITEMID > > mapReward_;
			if( g_pkUIScene->m_pkSuperGachaDlg->GetRewardMap( m_iGachaVer, m_iCurCharType, mapReward_ ) ) { 

				SetMapGachaRewardList( ItemID_, mapReward_ );

				std::map<int, std::set<GCITEMID>>::iterator it = m_mapStoredRewardInfo[ItemID_].begin();

				if( it != m_mapStoredRewardInfo[ItemID_].end() )
				{
					std::set<GCITEMID>::iterator it2 = it->second.begin();
					for(; it2 != it->second.end(); ++it2 )
					{
						KItem* pItem = NULL;
						bool bLockLevel = false;

						//printf( "ItemID = %d\n", *it2);
						if( g_pItemMgr->FindInventoryForItemID(*it2, &pItem) )
						{
							//printf("\tInventory  Level = %d\n", pItem->m_sEquipLevel);
							m_iCurItemLevel = pItem->m_sEquipLevel;
							bLockLevel = true;
						}
						else if( SiKGCDepotManager()->GetDepotItemInfo(*it2))
						{
							//printf("\tDepot  Level = %d\n", pItem->m_sEquipLevel);
							m_iCurItemLevel = pItem->m_sEquipLevel;
							bLockLevel = true;
						}

						if( bLockLevel == true )
						{
							m_pkBtnSelectItemLevel->SetWndState(KD3DWnd::D3DWS_DISABLED);
							m_pkBtnSelectItemLevel->SetSelfInputCheck( false );
							m_pkStaticSelectedItemLevel->SetText(m_iCurItemLevel);
							m_pkStaticSelectedItemLevel->Lock( true );
							std::map<int, std::pair<int, int>>::iterator mit = m_mapMenuItemLevelInfo.find( (m_iCurItemLevel - 10) / 10 );
							m_pkStaticClearSealDesc->SetTextAutoMultiline(g_pkStrLoader->GetReplacedString(STR_ID_NEW_GACHA_SYSTEM_MESSAGE02, "l", to_wstring(mit->second.second)));
							break;
						}
					}

				}

				SetRewardItemList( mapReward_ );
			}
			else { 
				KP2P::GetInstance()->Send_GachaRewardListReq( m_iGachaVer, ItemID_ * 10 );

				Result_Gacha_Reward_List = INT_MAX;
				g_MyD3D->WaitForServerAck(Result_Gacha_Reward_List, INT_MAX, 5000, NULL);
			}
		}
		else {
			KP2P::GetInstance()->Send_GachaRewardListReq( m_iGachaVer, ItemID_ * 10 );

			Result_Gacha_Reward_List = INT_MAX;
			g_MyD3D->WaitForServerAck(Result_Gacha_Reward_List, INT_MAX, 5000, NULL);
		}
	}
}

void KGCNewClearSealDlg::SetMapGachaRewardList( GCITEMID ItemID_, std::map<int, std::set<GCITEMID>>& mapRewardItemList_)
{
	m_mapStoredRewardInfo[ItemID_] = mapRewardItemList_;
}

void KGCNewClearSealDlg::SetRewardItemList( std::map< int, std::set<GCITEMID> >& mapRewardItemList_ )
{
	// UI 초기화
	for(int iLoop=0; iLoop<EREWORD_ITEM_NUM; ++iLoop)
	{
		m_pkImgRewordEquip[iLoop]->ToggleRender( false );
		m_pkWndRewordEquipOver[iLoop]->ToggleRender( false );
		if( iLoop < EREWORD_ACC_NUM )
		{
			m_pkImgRewordAcc[iLoop]->ToggleRender( false );
			m_pkWndRewordAccOver[iLoop]->ToggleRender( false );
		}
	}

	m_pkBtnClearSealEquip->Lock(false);
	m_pkBtnClearSealAcc->Lock( false );
	//m_pkBtnTakeEquip->ToggleRender( false );
	//m_pkBtnTakeAcc->ToggleRender( false );

	int iEquipItemInInven = 0;
	int iAccItemInInven = 0;
	int iRewardItemSize = 0;

	m_bGetEquipAll = false;
	m_bGetAccessoriesAll = false;
	m_bRewardEquipItem = false;
	m_vecRewardEuipItems.clear();
	m_vecRewardAccItems.clear();

	std::map< int, std::set<GCITEMID> >::iterator mapIter = mapRewardItemList_.begin();
	for(; mapIter != mapRewardItemList_.end(); ++mapIter )
	{
		if( mapIter->first == ETYPE_EQUIP_ITEM )
		{
			// 보유하고 있는 아이템 처리 및 보상 아이템 이미지 셋팅
			std::set<GCITEMID>::iterator setIter = mapIter->second.begin();
			for(int iLoop=0; setIter != mapIter->second.end(); ++setIter, ++iLoop )
			{
				//printf("itemid = %d, itemLevel = %d\n", *setIter, m_iCurItemLevel );
				if( g_pItemMgr->FindInventoryForItemIdAndLevel( *setIter, m_iCurItemLevel ) || 
					SiKGCDepotManager()->CheckDepotItemForLevel( *setIter, m_iCurItemLevel ) )
				{
					iEquipItemInInven++;
					m_pkWndRewordEquipOver[iLoop]->ToggleRender( false );
				}
				else
				{
					m_pkWndRewordEquipOver[iLoop]->ToggleRender( true );
				}

				m_pkImgRewordEquip[iLoop]->ToggleRender( true );
				m_pkImgRewordEquip[iLoop]->SetItemInfo( *setIter );
				m_vecRewardEuipItems.push_back( *setIter );
			}

			// 모두 보유하고 있는 아이템 이라면...
			if( iEquipItemInInven == int(mapIter->second.size()) )
			{
				// 봉인해제버튼 제거
				m_pkBtnClearSealEquip->ToggleRender(false);
				m_pkBtnClearSealEquip->Lock(true);

				// 보상획득버튼 상태처리
				//m_pkBtnTakeEquip->ToggleRender(true);

				// 무기보상을 받았다면 무조건 Lock
				// 무기보상을 안받았는데 전직선택이 안되었다면 Lock
				// 아니면 Active
				bool bGetWeapon = false;
				DWORD dwBeadItemID = m_pkImgBeadItem->GetItemID();
				if( !IsObtainedItem( ETYPE_EQUIP_ITEM ) )
				{
					if( 0 < m_pkJobCombo->GetCurSel() )
					{
						m_pkBtnTakeEquip->Lock(false);
						m_pkBtnTakeEquip->ClearToolTip();
						m_pkStaticClearSealDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_COORDI_GACHA_SYSTEM_MESSAGE03));
					}
					else
					{
						ClearPaticle();
						m_pkBtnTakeEquip->Lock(true);
						m_pkBtnTakeEquip->SetToolTip( g_pkStrLoader->GetString( STR_ID_COORDI_GACHA_TOOLTIP01 ) );
						m_pkStaticClearSealDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_COORDI_GACHA_SYSTEM_MESSAGE02));
					}
					m_pkWndTakeEquipOver->ToggleRender(true);
				}
				else
				{
					ClearPaticle();
					m_pkBtnTakeEquip->Lock(true);
					m_pkBtnTakeEquip->ClearToolTip();
					m_bRewardEquipItem = true;
					m_pkWndTakeEquipOver->ToggleRender(false);
					bGetWeapon = true;
					//m_iCurItemLevel = 0;
					//m_pkStaticSelectedItemLevel->SetText(L"0");
				}

				m_bGetEquipAll = true;
				//#if !defined( __PATH__ )
				if( m_bAutoMode && !bGetWeapon ) {
					OnStopAuto();
				}
				//#endif
			}
			else
			{
				//m_pkBtnTakeEquip->ToggleRender(false);
				m_pkWndTakeEquipOver->ToggleRender(true);
				m_pkBtnClearSealEquip->ToggleRender(true);
				m_pkBtnClearSealEquip->Lock(false);
				m_pkStaticClearSealDesc->SetTextAutoMultiline(g_pkStrLoader->GetReplacedString(STR_ID_NEW_GACHA_SYSTEM_MESSAGE02, "i", m_iNeedSpellItemNum));
			}
		}
		else if( mapIter->first == ETYPE_ACCESSORY_ITEM )
		{
			std::set<GCITEMID>::iterator setIter = mapIter->second.begin();
			for(int iLoop=0; setIter != mapIter->second.end(); ++setIter, ++iLoop )
			{
				if( g_pItemMgr->FindInventoryForItemIdAndLevel( *setIter, m_iCurItemLevel ) || 
					SiKGCDepotManager()->CheckDepotItemForLevel( *setIter, m_iCurItemLevel ) )
				{
					iAccItemInInven++;
					m_pkWndRewordAccOver[iLoop]->ToggleRender( false );
				}
				else
					m_pkWndRewordAccOver[iLoop]->ToggleRender( true );

				m_pkImgRewordAcc[iLoop]->ToggleRender( true );
				m_pkImgRewordAcc[iLoop]->SetItemInfo( *setIter );
				m_vecRewardAccItems.push_back( *setIter );
			}

			// 모두 보유하고 있는 아이템 이라면...
			if( iAccItemInInven == int(mapIter->second.size()) )
			{
				// 봉인해제버튼 제거
				m_pkBtnClearSealAcc->ToggleRender(false);
				m_pkBtnClearSealAcc->Lock(true);

				// 보상획득버튼 상태처리
				//m_pkBtnTakeAcc->ToggleRender(true);

				// 장비 보상을 안받았거나 액세보상을 받았으면 Lock 아니면 Active
				DWORD dwBeadItemID = m_pkImgBeadItem->GetItemID();
				bool bGetPet = false;
				if( false == m_bRewardEquipItem || m_setObtainedBeadList.end() != m_setObtainedBeadList.find( std::make_pair(dwBeadItemID, ETYPE_ACCESSORY_ITEM) ) )
				{
					m_pkBtnTakeAcc->Lock(true);
					bGetPet = true;
					if( m_bRewardEquipItem )
						m_pkBtnTakeAcc->ClearToolTip();
					else
						m_pkBtnTakeAcc->SetToolTip( g_pkStrLoader->GetString( STR_ID_COORDI_GACHA_TOOLTIP02 ) );
					m_pkWndTakeAccOver->ToggleRender(false);
				}
				else
				{
					m_pkBtnTakeAcc->Lock(false);
					m_pkBtnTakeAcc->ClearToolTip();
					m_pkWndTakeAccOver->ToggleRender(true);
				}

				m_bGetAccessoriesAll = true;
#if !defined( __PATH__ )
				if( m_bAutoMode && !bGetPet ) { 
					OnStopAuto();
				}
#endif
			}
			else
			{
				if( m_bRewardEquipItem )
					m_pkStaticClearSealDesc->SetTextAutoMultiline(g_pkStrLoader->GetReplacedString(STR_ID_NEW_GACHA_SYSTEM_MESSAGE02, "i", m_iNeedSpellItemNum));
				m_pkBtnClearSealAcc->ToggleRender(true);
				m_pkBtnClearSealAcc->Lock(false);
				//m_pkBtnTakeAcc->ToggleRender(false);
				m_pkWndTakeAccOver->ToggleRender(true);
			}
		}

		iRewardItemSize += int(mapIter->second.size());
	}    

	if( m_bRewardEquipItem && m_bGetAccessoriesAll )
		m_pkStaticClearSealDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_COORDI_GACHA_SYSTEM_MESSAGE04));

	m_bGetAll = ( iRewardItemSize == (iEquipItemInInven + iAccItemInInven) );

	if( IsObtainedItem(ETYPE_ACCESSORY_ITEM) && IsObtainedItem(ETYPE_EQUIP_ITEM) )
	{
		m_pkWndTakeAccOver->ToggleRender( false );
		m_pkBtnTakeAcc->ToggleRender( false );
		m_pkBtnTakeEquip->ToggleRender( false );
	}
}

void KGCNewClearSealDlg::SetFinalRewardtItemList( std::map<int, std::vector<GCITEMID>>& mapRewardItemList_ )
{
	m_vecFinalRewardEuipItems.clear();

	m_pkImgTakeEquip->ToggleRender(false);
	m_pkImgTakeAcc->ToggleRender(false);

	std::map< int, std::vector<GCITEMID> >::iterator mapIter = mapRewardItemList_.begin();

	for(; mapIter != mapRewardItemList_.end(); ++mapIter )
	{        
		if( mapIter->first == ETYPE_EQUIP_ITEM )
		{
			std::vector<GCITEMID>::iterator vecIter = mapIter->second.begin();
			for( int iLoop=0; vecIter != mapIter->second.end(); ++vecIter, ++iLoop )
			{
				if( iLoop < EBUY_ITEM_NUM + 1 )
				{
					m_vecFinalRewardEuipItems.push_back(*vecIter);                    
				}

				if( g_pItemMgr->FindInventoryForItemIdAndLevel( *vecIter, m_iCurItemLevel ) ||
					SiKGCDepotManager()->CheckDepotItemForLevel( *vecIter, m_iCurItemLevel ) ){
						m_pkImgTakeEquip->ToggleRender(true);
						m_pkImgTakeEquip->SetItemInfo(*vecIter);
				}
				else if( iLoop == 0 )
				{
					m_pkImgTakeEquip->ToggleRender(true);
					m_pkImgTakeEquip->SetItemInfo(*vecIter);
				}
			}            
		}
		else if( mapIter->first == ETYPE_ACCESSORY_ITEM )
		{
			std::vector<GCITEMID>::iterator vecIter = mapIter->second.begin();
			for( int iLoop=0 ; vecIter != mapIter->second.end(); ++vecIter, ++iLoop )
			{
				m_pkImgTakeAcc->ToggleRender( true );
				m_pkImgTakeAcc->SetItemInfo( *vecIter );
			}            
		}        
	}
}

bool KGCNewClearSealDlg::LoadScriptBuyItemList()
{
	KLuaManager     luaMgr;
	if(false == GCFUNC::LoadLuaScript(luaMgr, "GachaItemList.stg") )
		return false;

	LUA_BEGIN_TABLE("GachaBuyList", return false )
	{
		int iLoop = 1;
		m_mapBuyItemList.clear();
		while( 1 )
		{
			LUA_BEGIN_TABLE( iLoop++, break ) 
			{
				int iItemID = 0;
				LUA_GET_VALUE( "ID", iItemID, return false );

				std::vector<DWORD> vecItemList;
				LUA_BEGIN_TABLE( "ItemList", return false )
				{
					int iValue, iItemLoop = 1;
					while( SUCCEEDED(luaMgr.GetValue(iItemLoop++, iValue) ) )
					{
						GCItem* pItem = g_pItemMgr->GetItemData( iValue );
						if(pItem == NULL) break;
						if( pItem->bNotBuyItem == false )
							vecItemList.push_back( iValue );
					}
				}
				LUA_END_TABLE( return false )
					m_mapBuyItemList[iItemID] = vecItemList;
			}
			LUA_END_TABLE( return false )
		}
	}
	LUA_END_TABLE( return false )

		return true;
}

void KGCNewClearSealDlg::LoadGachaKeyList()
{
	m_vecGachaKeyItemList.clear();

	KLuaManager     luaMgr;

	if(false == GCFUNC::LoadLuaScript(luaMgr, "GachaItemList.stg") )
		return;

	LUA_BEGIN_TABLE("GachaKeyList", return )
	{
		for(int i = 1; ; ++i)
		{
			int iTemp = 0;
			LUA_GET_VALUE_NOASSERT(i,iTemp,break)
				m_vecGachaKeyItemList.push_back(iTemp); 
		}
	}
	LUA_END_TABLE( return)
}

void KGCNewClearSealDlg::ClearPaticle()
{
	m_bPaticleInit = false;
	if(m_pEffTextAlert)
		g_ParticleManager->DeleteSequence( m_pEffTextAlert );
}

void KGCNewClearSealDlg::OnRecvFinalReward()
{ 
	if( 0 < m_iParticleLife )
		return;

	KP2P::GetInstance()->Send_GachaLevelSetRewardReq( m_pkImgBeadItem->GetItemID(), static_cast<DWORD>(ETYPE_EQUIP_ITEM), m_iCurItemLevel, m_pkImgTakeEquip->GetItemID(), static_cast<char>(m_iCurCharType), m_iGachaVer );
}

void KGCNewClearSealDlg::OnRecvFinalTotalReward()
{
	if( 0 < m_iParticleLife )
		return;

	KP2P::GetInstance()->Send_GachaLevelSetRewardReq( m_pkImgBeadItem->GetItemID(), static_cast<DWORD>(ETYPE_ACCESSORY_ITEM), m_iCurItemLevel, m_pkImgTakeAcc->GetItemID(), static_cast<char>(m_iCurCharType), m_iGachaVer );
}

void KGCNewClearSealDlg::OnRecvGachaAck()
{
	ShowGachaEffect();
	Init_ItemImage();
}

void KGCNewClearSealDlg::Init_ItemImage()
{    
	m_pkGetItemImage->RemoveAllTexture();
	int iItemID = g_pkUIScene->m_pkTreaureItemBox->GetItemID();
	if( iItemID == 0 )
		return;

	m_pkGetItemImage->AddTexture( g_pItemMgr->CreateShopItemTexture(iItemID));

	m_pkGetItemImage->SetScale( 0.0f, 0.0f );
	m_pkGetItemImage->SetAxisOffset( -0.5f, -0.5f, 0.0f );
	m_pkGetItemImage->SetPosition( 1.005f, 0.68f );
	m_pkGetItemImage->SetAlpha( 255 );
	m_pkGetItemImage->CameraMatrixOn();
	m_iFrameCount_ItemImage = 0;
}

void KGCNewClearSealDlg::FrameMove_ItemImage()
{
	if( g_pkUIScene->m_pkTreaureItemBox->GetItemID() == 0 )
		return;

	++m_iFrameCount_ItemImage;

	if( m_iFrameCount_ItemImage > 72 )
	{
		int iFrame = m_iFrameCount_ItemImage - 72;
		if( iFrame > 0 && iFrame < 50 )
		{
			float delta = SiKGCMath()->f(5, (float)iFrame / 20.0f) / 8.0f;
			m_pkGetItemImage->SetScale(
				m_pkGetItemImage->GetWidth() + delta,
				m_pkGetItemImage->GetHeight() + delta
				);
		}
		else if( iFrame > 50 && iFrame < 110 )
		{
			m_pkGetItemImage->SetScale(
				m_pkGetItemImage->GetWidth() + 0.002f,
				m_pkGetItemImage->GetHeight() + 0.002f
				);
			m_pkGetItemImage->SetAlpha( 255 - ( iFrame - 50 ) * 4 );
		}
		else if ( iFrame == 110 )
		{
			m_pkGetItemImage->SetScale( 0.0f, 0.0f );
		}
		else if ( iFrame > 200 )
		{
			Init_ItemImage();
		}
	}
}

void KGCNewClearSealDlg::ShowGachaEffect()
{
	g_KDSound.Play( "GACHA_EFFECT" );
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Gacha_01", 0.0f, 0.16f, 0.5f ) , GC_LAYER_GACHA);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Gacha_02", 0.0f, 0.16f, 0.5f ) , GC_LAYER_GACHA);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Gacha_03", 0.0f, 0.16f, 0.5f ) , GC_LAYER_GACHA);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Gacha_04", 0.0f, 0.16f, 0.5f ) , GC_LAYER_GACHA);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Gacha_05", 0.0f, 0.16f, 0.5f ) , GC_LAYER_GACHA);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Gacha_06", 0.0f, 0.16f, 0.5f ) , GC_LAYER_GACHA);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Gacha_07", 0.0f, 0.16f, 0.5f ) , GC_LAYER_GACHA);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Gacha_08", 0.0f, 0.16f, 0.5f ) , GC_LAYER_GACHA);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Gacha_09", 0.0f, 0.16f, 0.5f ) , GC_LAYER_GACHA);
}

void KGCNewClearSealDlg::AddObtainedBeadList( std::pair<DWORD,DWORD>& prItemInfo_ )
{
	m_setObtainedBeadList.insert( prItemInfo_ );
	//InitRewardBtn();
	InitUseSealBtn();
	UpdateOwnItemList();
}

void KGCNewClearSealDlg::SetObtainedBeadList( std::set<std::pair<DWORD,DWORD>>& setObtainedBeadList_ )
{
	m_setObtainedBeadList = setObtainedBeadList_;
	//InitRewardBtn();
	InitUseSealBtn();
	UpdateOwnItemList();
}

void KGCNewClearSealDlg::StartAuto()
{
	m_bAutoMode = true;

	if( m_dwGachaItemType == ETYPE_EQUIP_ITEM )
	{
		m_pkBtnClearSealAcc->Lock(true);
	}
	else if( m_dwGachaItemType == ETYPE_ACCESSORY_ITEM )
	{
		m_pkBtnClearSealEquip->Lock(true);
	}

	m_pkBtnClose->Lock( true );
}

void KGCNewClearSealDlg::OnStopAuto( void )
{
	OnClickAuto();
	//m_bAutoMode = false;
	m_pkBtnClose->Lock( false );

	if(!m_bGetEquipAll) { 
		m_pkBtnClearSealEquip->Lock(false);
	}
	if( !m_bGetAccessoriesAll) { 
		m_pkBtnClearSealAcc->Lock(false);
	}

	g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
	g_pkUIScene->m_pkMyInfoScene->UpdateData(); 
	Init_ItemImage();
}

void KGCNewClearSealDlg::InitRewardBtn()
{
	//m_pkBtnTakeEquip->ToggleRender(m_bGetEquipAll);
	//m_pkBtnTakeAcc->ToggleRender(m_bGetAll);
}

void KGCNewClearSealDlg::InitUseSealBtn()
{
	m_pkBtnClearSealEquip->ToggleRender(!m_bGetEquipAll);
	if( m_bGetEquipAll )
		m_pkBtnClearSealAcc->ToggleRender(!m_bGetAccessoriesAll);
}

bool KGCNewClearSealDlg::IsGachaKeyItem(DWORD dwItemID)
{
	return ( m_vecGachaKeyItemList.end() != std::find(m_vecGachaKeyItemList.begin(),m_vecGachaKeyItemList.end(),dwItemID ) );
}

void KGCNewClearSealDlg::SetBeadSpellItemNum( int iBeadNum_, int iSpellNum_ )
{
	m_iCurBeadItemNum = iBeadNum_;
	m_iCurSpellItemNum = iSpellNum_;

	m_pkStaticBeadItem->SetText(iBeadNum_);
	m_pkStaticSealItem->SetText(iSpellNum_);
}

void KGCNewClearSealDlg::OnItemEquipPreview()
{
	std::vector<GCItem*> vecItem;
	vecItem.clear();

	// 장비
	for( int i = 0; i < static_cast<int>(m_vecRewardEuipItems.size()); i++ )
	{
		vecItem.push_back( g_pItemMgr->GetItemData(m_vecRewardEuipItems[i]) );
	}

	// 액세
	for( int i = 0; i < static_cast<int>(m_vecRewardAccItems.size()); i++ )
	{
		vecItem.push_back( g_pItemMgr->GetItemData(m_vecRewardAccItems[i]) );
	}

	// 무기보상
	for( int i = 0; i < static_cast<int>(m_vecFinalRewardEuipItems.size()); i++ )
	{
		if( i == m_iCurCharJob )
			vecItem.push_back( g_pItemMgr->GetItemData(m_vecFinalRewardEuipItems[i]) );
	}

	UpdateMeshObject( &vecItem );
}

void KGCNewClearSealDlg::InitPreview()
{
	for( int j = 0; j < NUM_JOB_LEVEL; j++ )
	{
		for( int i = 0; i < GC_CHAR_NUM; i++ )
		{
			char strTemp[MAX_PATH];
			std::string strShopMotion;
			sprintf( strTemp, "shop_%d_%d", i, j );

			strShopMotion = strTemp;

			// i + j * GC_CHAR_NUM == m_iExtraCharNum
			m_pWaitMotion[i + j * GC_CHAR_NUM] = g_pGCDeviceManager2->CreateMotionFRM( strShopMotion + "_0.frm" );
			if( m_pWaitMotion[i + j * GC_CHAR_NUM ]->IsDisable() )
			{
				//assert(0);
				SAFE_RELEASE(m_pWaitMotion[i + j * GC_CHAR_NUM ]);
			}
			m_pActionMotion[i + j * GC_CHAR_NUM] = g_pGCDeviceManager2->CreateMotionFRM( strShopMotion + "_1.frm" );
			if( m_pActionMotion[i + j * GC_CHAR_NUM ]->IsDisable() )
			{
				//assert(0);
				SAFE_RELEASE(m_pActionMotion[i + j * GC_CHAR_NUM ]);
			}
		}
	}

	D3DXMATRIX matLook, matScale, matTran;
	::D3DXMatrixPerspectiveLH( &matLook, float( VIEWPORT_WIDTH ) / float( VIEWPORT_HEIGHT ), 1.0f, 1.0f, 100.0f );
	::D3DXMatrixOrthoOffCenterLH(&m_matScreenProj, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT, 0, 0.f, 100.f);
	::D3DXMatrixScaling( &matScale, 1.0f, 1.0f, 0.01f );
	::D3DXMatrixTranslation( &matTran, 0.0f, 0.172f, 0.5f );
	m_matProject = matLook * matScale * matTran;

	// 배경 로드 
	if( m_pkBackTexture )
		SAFE_RELEASE( m_pkBackTexture );
	m_pkBackTexture = g_pGCDeviceManager2->CreateTexture( "showroom.dds" );
}

void KGCNewClearSealDlg::DeleteMeshObject( void )
{
	m_Player.ReleasePlayerResource();

	if( m_pMeshFootStage )
	{
		g_RenderManager->RemoveObject( m_pMeshFootStage );
		SAFE_DELETE( m_pMeshFootStage );
	}

	for( int j = 0; j < NUM_JOB_LEVEL; j++ )
	{
		for( int i = 0; i < GC_CHAR_NUM; i++ )
		{
			SAFE_RELEASE(m_pWaitMotion[i + j * GC_CHAR_NUM ]);
			SAFE_RELEASE(m_pActionMotion[i + j * GC_CHAR_NUM ]);
		}
	}

}

void KGCNewClearSealDlg::RenderPreview( void )
{
	// 행렬 구하는 곳 
	g_pGCDeviceManager2->Flush();
	D3DVIEWPORT9 orgViewport;

	D3DXMATRIX matRot,matTrans,matScale,matWorld;
	g_pd3dDevice->GetViewport( &orgViewport );
	g_pd3dDevice->SetViewport( &m_viewport );

	//g_pd3dDevice->SetFVF( SKINVERTEX::FVF );
	//g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );
	//g_MyD3D->m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER|D3DCLEAR_TARGET, 0, 1.0f, 0L );

	// 배경 출력
// 	g_pGCDeviceManager2->DrawInScreen( m_pkBackTexture,
// 		105.0f * GC_SCREEN_DIV_WIDTH * g_pGCDeviceManager2->GetWindowScaleX(), 
// 		245.0f * GC_SCREEN_DIV_WIDTH * g_pGCDeviceManager2->GetWindowScaleY(), 
// 		285.0f * GC_SCREEN_DIV_WIDTH * g_pGCDeviceManager2->GetWindowScaleX(), 
// 		490.0f * GC_SCREEN_DIV_WIDTH * g_pGCDeviceManager2->GetWindowScaleY(),
// 		0.0f, 0.0f, 1.0f, 1.0f, false, 0.1f, 0xffffffff );

	g_pGCDeviceManager2->DrawInScreen(m_pkBackTexture,
		0,
		0,
		VIEWPORT_WIDTH,
		VIEWPORT_HEIGHT,
		0.0f, 0.0f, 1.0f, 1.0f, false, 0.1f, 0xffffffff);

	// 발판 출력  
	float fScale = 4.1f;
	float fFootStageY = -0.15f;
	float fFootStageZ = 0.395f;
	float fFootStageMul = 0.46f;
	float fAngle = 0.9f;

	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProject);

	D3DXMatrixScaling( &matScale ,m_fZoom * fScale, m_fZoom * fScale, m_fZoom * fScale );
	D3DXMatrixRotationY( &matRot, m_fStageAngleMove );
	D3DXMatrixTranslation( &matTrans, 0.0f, fFootStageY + ( MAXZOOM - m_fZoom ) * fFootStageMul, fFootStageZ );   

	matWorld = matScale * matRot * matTrans;
	if( m_pMeshFootStage != NULL )
	{
		m_pMeshFootStage->SetWorldMat( &matWorld );
		g_RenderManager->Render( false, m_pMeshFootStage, EGCShaderTechnique::CARTOON_NOEDGE );
	}

	if( m_pShopMotion != NULL )
	{
		if( !m_Player.m_pObject )
		{
			ResetPlayer();
		} 

		if ( m_cFrame > (int)m_pShopMotion->GetNum_Frame() - 1 )
		{
			m_iMotionCount++;
			if( m_iMotionCount == 5 )
			{
				if( m_pActionMotion[m_iExtraCharNum] != NULL )
				{
					// 모션이 바뀌었음
					m_pShopMotion = m_pActionMotion[m_iExtraCharNum];
					m_iMotionCount = 0;
					m_fCharX = 0.0f;// X를 초기화

					SiKGCFaceLift()->SetCurMotionName( m_pActionMotion[m_iExtraCharNum]->GetDeviceID() );
				}
			}
			else
			{
				if( m_pWaitMotion[m_iExtraCharNum] != NULL )
				{
					// 모션이 바뀌었음
					m_pShopMotion = m_pWaitMotion[m_iExtraCharNum];
					m_fCharX = 0.0f;// X를 초기화

					SiKGCFaceLift()->SetCurMotionName( m_pWaitMotion[m_iExtraCharNum]->GetDeviceID() );
				}
			}

			m_cFrame = 0;
		}

		float fPosX = m_fCharX * CHAR_SCALE_RATIO;
		float fPosY = m_pShopMotion->GetFrameData(m_cFrame).Pos_y * 0.1f - 0.03f;
		float fPosZ = 0.37f;

		m_fCharX += m_pShopMotion->GetFrameData(m_cFrame).Plus_x;

		fPosX = fPosX * cos( m_fAngleMove );

		D3DXMatrixScaling( &matScale , m_fZoom, m_fZoom, m_fZoom );
		D3DXMatrixRotationY( &matRot, m_fAngleMove );
		D3DXMatrixTranslation( &matTrans, fPosX, fPosY, fPosZ );

		matWorld = matScale * matRot * matTrans;

		m_Player.m_pObject->SetWorldMat( &matWorld );
		m_Player.m_pObject->SetFrame( &m_pShopMotion->GetFrameData( (int)m_cFrame ) );
		m_Player.SetRenderFaceLift( m_Player.GetDefaultFaceLiftType() );
		SiKGCFaceLift()->SetFaceLiftByMotionName( &m_Player, SiKGCFaceLift()->GetCurMotionName(), (int)m_cFrame );
		g_RenderManager->Render( false ,m_Player.m_pObject, EGCShaderTechnique::TECHNIQUE_NUM);

		m_cFrame++;
	}        

	g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_MyD3D->matProj );
	g_pd3dDevice->SetViewport( &orgViewport );
	g_pd3dDevice->SetTexture( 1, NULL );
}

void KGCNewClearSealDlg::FrameMove_Preview( void )
{
	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
	bool bCheckBound = this->CheckPosInWindowBoundWithChild(vMousePos, true);

	if( !m_bAngleChanged )
	{
		m_fAngleMove += 0.005f;
		m_fStageAngleMove += 0.0035f;
		if ( m_fAngleMove >= 2 * D3DX_PI ) m_fAngleMove = 0.0f;
		if ( m_fStageAngleMove >= 2 * D3DX_PI ) m_fStageAngleMove = 0.0f;
	}

	if( g_pkInput->ISWheelUp() && bCheckBound )
		ChangeCharWndZoom(true);
	else if( g_pkInput->ISWheelDown() && bCheckBound )
		ChangeCharWndZoom(false);

	static bool bInternal = false;

	if( g_pkInput->BtnDown() == true &&  bCheckBound )
	{
		m_bAngleChanged = true;
		m_ptStartMouse = g_pkInput->GetMousePos();
		m_fStartAngle = m_fAngleMove;
		m_fStartStageAngle = m_fStageAngleMove;
		bInternal = true;
	}
	else if( g_pkInput->BtnDown() == true &&  !bCheckBound )
		bInternal = false;

	if( g_pkInput->BtnDown( KInput::MBRIGHT ) == TRUE  )
	{
		m_bAngleChanged = false;
	}

	if( g_pkInput->BtnPressed() && m_bAngleChanged == true && bInternal == true )
		ProcessCharWndAngle();

	if( m_Player.m_pObject )
	{
		m_Player.SetRender( true );
		m_Player.EgoItemFrameMove( UCHAR(m_cFrame) );
		m_Player.m_pObject->ChildFrameMove();
	}
}

void KGCNewClearSealDlg::ChangeCharWndZoom( bool bZoom )
{
	if( bZoom )
		m_fZoom += DELTAZOOM;
	else
		m_fZoom -= DELTAZOOM;

	if( m_fZoom > MAXZOOM )
		m_fZoom = MAXZOOM;

	if( m_fZoom < MINZOOM )
		m_fZoom = MINZOOM;
}

void KGCNewClearSealDlg::ProcessCharWndAngle( void )
{
	POINT CurrentMousePoint = g_pkInput->GetMousePos();

	m_fAngleMove = m_fStartAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * DELTAZOOM;
	m_fStageAngleMove = m_fStartStageAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * DELTAZOOM;
}

void KGCNewClearSealDlg::SetCoordiView( int iCharType_ )
{
	if( 0 > m_iExtraCharNum || m_iExtraCharNum > CID_MAX)
	{
		m_iExtraCharNum = m_iCurCharType;
		m_Player.Extra_Char_Num = m_iCurCharType;
	}

	if( m_pWaitMotion[m_iExtraCharNum] != NULL )
		m_pShopMotion = m_pWaitMotion[m_iExtraCharNum];
	else
	{
		m_iExtraCharNum = m_iCurCharType;
		m_Player.Extra_Char_Num = m_iCurCharType;
		if( m_pWaitMotion[m_iExtraCharNum] != NULL )
			m_pShopMotion = m_pWaitMotion[m_iExtraCharNum];
	}

	D3DXVECTOR2 vPos( GetFixedWindowPos() );
	m_viewport.X = static_cast<DWORD>(( VIEWPORT_OFFSET_X * GC_SCREEN_DIV_WIDTH + vPos.x ) * g_pGCDeviceManager2->GetWindowScaleX());
	m_viewport.Y = static_cast<DWORD>(( VIEWPORT_OFFSET_Y * GC_SCREEN_DIV_WIDTH + vPos.y ) * g_pGCDeviceManager2->GetWindowScaleY());
	m_viewport.Width = static_cast<DWORD>( VIEWPORT_WIDTH * GC_SCREEN_DIV_WIDTH * g_pGCDeviceManager2->GetWindowScaleX() );
	m_viewport.Height = static_cast<DWORD>( VIEWPORT_HEIGHT * GC_SCREEN_DIV_WIDTH * g_pGCDeviceManager2->GetWindowScaleY() );
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
}

void KGCNewClearSealDlg::UpdateMeshObject(  std::vector<GCItem*> *pvecItem )
{
	if( pvecItem )
	{
		SetCoordiView( m_iCurCharType );
		g_pItemMgr->UnequipItemAll( &(m_Player.m_kUserInfo.GetCurrentChar( m_iCurCharType )) );
		std::vector<GCItem*>::iterator vecIterIter = pvecItem->begin();
		for(; vecIterIter != pvecItem->end(); ++vecIterIter )
		{
			if( (*vecIterIter) )
				g_pItemMgr->EquipItem( &( m_Player.m_kUserInfo.GetCurrentChar( m_iCurCharType )), (*vecIterIter) );
		}
	}

	m_Player.SetMyPlayer( -1, true  );
	m_Player.ReleasePlayerResource();
	m_Player.UpdatePlayerResource(true);
	//m_iExtraCharNum = m_Player.Extra_Char_Num;
	m_iExtraCharNum = m_iCurrentCharacter + GC_CHAR_NUM * m_iCurCharJob;

	if( m_Player.m_pObject )
		m_Player.m_pObject->SetShellMatrixScale( 0.25f );  

	if( NULL == m_pMeshFootStage )
	{
		m_pMeshFootStage = new GCObject();
#ifdef _DEBUG
		m_pMeshFootStage->m_strName = "코디샵 발판";
#endif
		m_pMeshFootStage->AddMesh( g_pGCDeviceManager2->CreateMeshP3M( "footstage.p3m" ), g_pGCDeviceManager2->CreateTexture( "footstage.dds" ) );
	}
}

void KGCNewClearSealDlg::ResetPlayer( void )
{
	m_Player = g_MyD3D->m_TempPlayer;
	m_Player.m_kUserInfo = g_kGlobalValue.m_kUserInfo;
	SetChar( m_iCurCharType );

	OnItemEquipPreview();
}

void KGCNewClearSealDlg::SetChar( int iCharType )
{
	InitCharacterInfo();
	m_Player.m_kUserInfo.SetCurrentChar( iCharType );

	m_iCurrentCharacter = iCharType;
	m_iExtraCharNum = iCharType + GC_CHAR_NUM * m_iCurCharJob;
	m_Player.Extra_Char_Num = iCharType + GC_CHAR_NUM * m_iCurCharJob;

	m_cFrame = 0;
	m_fCharX = 0.0f;
	m_iMotionCount = 0;
}

void KGCNewClearSealDlg::InitCharacterInfo()
{
	std::map<int, std::set< char> > mapCharInfo = g_kGlobalValue.GetOpenedCharacterInfo();
	std::map<int, std::set< char> >::iterator mit = mapCharInfo.begin();

	m_Player.m_kUserInfo.Init();

	for( ; mit != mapCharInfo.end(); ++mit )
	{
		SCharInfo charInfo;
		charInfo.iCharType = mit->first;
		if ( charInfo.iCharType == GC_CHAR_DIO )
			charInfo.setPromotion = g_kGlobalValue.m_kUserInfo.GetCurrentChar( GC_CHAR_DIO ).setPromotion;
		else if ( charInfo.iCharType == GC_CHAR_ZERO )
			charInfo.setPromotion = g_kGlobalValue.m_kUserInfo.GetCurrentChar( GC_CHAR_ZERO ).setPromotion;
		else if ( charInfo.iCharType == GC_CHAR_LEY )
			charInfo.setPromotion = g_kGlobalValue.m_kUserInfo.GetCurrentChar( GC_CHAR_LEY ).setPromotion;
		else
			charInfo.setPromotion = mit->second;

		m_Player.m_kUserInfo.AddChar(charInfo);
	}
}

bool KGCNewClearSealDlg::CheckCondition( IN const KDialogInfo& kInfo_ )
{
	m_iGachaVer = static_cast<int>( kInfo_.m_lParam2 );
	if( m_iGachaVer == 0 ) { 
		return false;
	}

	return true;
}

void KGCNewClearSealDlg::OnClickAuto( void )
{
	if( m_pkCheckBoxAutoClear == NULL ) { 
		return;
	}

	m_pkCheckBoxAutoClear->SetCheck( !m_pkCheckBoxAutoClear->GetCheck() );
	m_bAutoMode = m_pkCheckBoxAutoClear->GetCheck();
	if( m_bAutoMode ) {
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_GACHA_AUTO2 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
	}
}

void KGCNewClearSealDlg::InitSetLevel()
{
	m_iCurItemLevel = 0;
	m_pkStaticSelectedItemLevel->SetText(L"0");
}

void KGCNewClearSealDlg::AddObtainedBeadListForGachaLevel( std::pair< GCITEMID, int >& prItemInfo_, DWORD dwType_ )
{
	std::map< std::pair< GCITEMID, int>, std::set< DWORD > >::iterator mit = m_mapObtainedBeadList.find( prItemInfo_ );

	printf( "recv gacha level %d, %d, %d\n", prItemInfo_.first, prItemInfo_.second, dwType_);

	if( mit == m_mapObtainedBeadList.end() ) {
		std::set< DWORD > setType;
		setType.insert( dwType_ );
		m_mapObtainedBeadList.insert( std::make_pair( prItemInfo_, setType ) );
	} else {
		mit->second.insert( dwType_ );
	}

	//InitRewardBtn();
	InitUseSealBtn();
	UpdateOwnItemList();
}    

void KGCNewClearSealDlg::SetObtainedBeadListForGachaLevel( std::map< std::pair< GCITEMID, int >, std::set< GCITEMID > >& mapObtainedBeadList_ )
{
	m_mapObtainedBeadList = mapObtainedBeadList_;

	//InitRewardBtn();
	InitUseSealBtn();
	UpdateOwnItemList();
}

void KGCNewClearSealDlg::UpdateItemLevel( int iCurSelection )
{
	std::map<int, std::pair<int, int>>::iterator mit = m_mapMenuItemLevelInfo.find(iCurSelection);

	if( mit != m_mapMenuItemLevelInfo.end() )
	{
		m_iCurItemLevel     = mit->second.first;
		m_iNeedSpellItemNum = mit->second.second;

		std::wstringstream wstrm;
		wstrm << m_iCurItemLevel;
		m_pkStaticSelectedItemLevel->SetText( wstrm.str() );

		if( m_bGetEquipAll == false )
			m_pkBtnClearSealEquip->Lock( false );

		if( m_bGetAccessoriesAll == false )
			m_pkBtnClearSealAcc->Lock( false );

		m_pkStaticClearSealDesc->SetTextAutoMultiline(g_pkStrLoader->GetReplacedString(STR_ID_NEW_GACHA_SYSTEM_MESSAGE02, "i", m_iNeedSpellItemNum));

		//InitRewardBtn();
		InitUseSealBtn();
		UpdateOwnItemList();
	}
}

void KGCNewClearSealDlg::SetItemLevelList( std::map< int,int >& mapItemLevelList_ )
{
	m_mapMenuItemLevelInfo.clear();
	m_pkPopupMenuItemLevel->ClearMenu();

	std::map< int,int >::iterator itor = mapItemLevelList_.begin();

	for( int i = 1 ; itor != mapItemLevelList_.end(); ++itor )
	{
		m_mapMenuItemLevelInfo.insert(std::make_pair(i, *itor));
		std::wstringstream wstrmLevel;
		wstrmLevel << itor->first;
		m_pkPopupMenuItemLevel->AddMenu(i, wstrmLevel.str());
		i++;
	}
	InitSetLevel();
}

bool KGCNewClearSealDlg::IsObtainedItem( DWORD dwType_ )
{
	std::map< std::pair< DWORD, int >, std::set< DWORD > >::iterator mit = m_mapObtainedBeadList.find( std::make_pair( m_pkImgBeadItem->GetItemID() * 10, m_iCurItemLevel ) );

	if( mit != m_mapObtainedBeadList.end() ) 
	{
		std::set< DWORD >::iterator sit = mit->second.find( dwType_ );
		if( sit != mit->second.end() ) 
		{
			return true;
		}
	}

	return false;
}