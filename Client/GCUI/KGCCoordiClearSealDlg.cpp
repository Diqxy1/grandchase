#include "stdafx.h"
#include "gcui/KGCCoordiClearSealDlg.h"
#include "KGCFaceLift.h"
#include "KGCGachaBuyBox.h"
#include "KGCDepotManager.h"
#include "KGCMath.h"
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

IMPLEMENT_CLASSNAME( KGCCoordiClearSealDlg );
IMPLEMENT_WND_FACTORY( KGCCoordiClearSealDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCCoordiClearSealDlg, "gc_clear_seal_bead_coordi" );

KGCCoordiClearSealDlg::KGCCoordiClearSealDlg()
: m_iCurCharType( 0 )
, m_iCurBuyItem( 0 )
, m_iCurCharJob( 0 )
, m_iCurBeadItemNum( 0 )
, m_iCurSpellItemNum( 0 )
, m_iParticleLife( 0 )
, m_iFrameCount_ItemImage( 0 )
, m_iGachaKeyBuyCount( 0 )

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

    LINK_CONTROL( "img_item_bead", m_pkImgBeadItem );
    LINK_CONTROL( "img_item_seal", m_pkImgSealItem );
	LINK_CONTROL( "static_bead_num", m_pkStaticBeadItem );
	LINK_CONTROL( "static_seal_num", m_pkStaticSealItem );
	LINK_CONTROL("state_speech", m_pkStaticClearSealDesc );   

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
    
    LINK_CONTROL( "wnd_take_equipover", m_pkWndTakeEquipOver );
    LINK_CONTROL( "wnd_take_accover", m_pkWndTakeAccOver );

	LINK_CONTROL( "combo_promotion", m_pkJobCombo );
	LINK_CONTROL("checkbox_auto", m_pkCheckBoxAutoClear );
    
	LINK_CONTROL( "btn_buy", m_pkBtnBuy );
	LINK_CONTROL( "buy_item_box", m_pkGachaBuyItemBox);
    LINK_CONTROL( "btn_pet_view", m_pkBtnPetView );
    
    m_mapSealBead.clear();
    m_mapBuyItemList.clear();
    m_vecCurBuyItemList.clear();
    m_setObtainedBeadList.clear();
    m_vecFinalRewardEuipItems.clear();

#if defined( NATION_KOREA )
    LoadGachaKeyList();
#endif

    m_pkGetItemImage = new KGC2DObject();

    IsProccessedGacha = false;
}

KGCCoordiClearSealDlg::~KGCCoordiClearSealDlg()
{
    if( m_pkGetItemImage )
    {
        SAFE_DELETE( m_pkGetItemImage );
    }
}

void KGCCoordiClearSealDlg::OnCreateComplete( void )
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
        m_pkBtnPetView->ConnectEventProcedure<KGCCoordiClearSealDlg, void>( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCCoordiClearSealDlg::OnClickPetView );
        m_pkBtnPetView->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCCoordiClearSealDlg::OnEnterCursor );
    }

    if( m_pkBtnClose )
    {
        m_pkBtnClose->InitState( true, true, this );
        m_pkBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCCoordiClearSealDlg::OnExit );
        m_pkBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCCoordiClearSealDlg::OnEnterCursor );
    }

    if( m_pkBtnTakeEquip )
    {
        m_pkBtnTakeEquip->InitState( true, true, this );
		m_pkBtnTakeEquip->Lock( true );
        m_pkBtnTakeEquip->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCCoordiClearSealDlg::OnRecvFinalReward );
    }

    if( m_pkBtnTakeAcc )
    {
        m_pkBtnTakeAcc->InitState( true, true, this );
		m_pkBtnTakeAcc->Lock( true );
        m_pkBtnTakeAcc->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCCoordiClearSealDlg::OnRecvFinalTotalReward );
    }

    if( m_pkBtnClearSealEquip )
    {
        m_pkBtnClearSealEquip->InitState( true, true, this );
        m_pkBtnClearSealEquip->ConnectEventProcedure<KGCCoordiClearSealDlg, void>( KD3DWnd::D3DWE_BUTTON_CLICK, this, boost::bind( &KGCCoordiClearSealDlg::OnClickClearSealButton, _1, static_cast<DWORD>(ETYPE_EQUIP_ITEM) ) );
        m_pkBtnClearSealEquip->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCCoordiClearSealDlg::OnEnterCursor );
    }

    if( m_pkBtnClearSealAcc )
    {
        m_pkBtnClearSealAcc->InitState( true, true, this );
        m_pkBtnClearSealAcc->ConnectEventProcedure<KGCCoordiClearSealDlg, void>( KD3DWnd::D3DWE_BUTTON_CLICK, this, boost::bind( &KGCCoordiClearSealDlg::OnClickClearSealButton, _1, static_cast<DWORD>(ETYPE_ACCESSORY_ITEM) ) );
        m_pkBtnClearSealAcc->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCCoordiClearSealDlg::OnEnterCursor );
    }

    if( m_pkStaticClearSealDesc )
    {
        m_pkStaticClearSealDesc->InitState(true);
        m_pkStaticClearSealDesc->SetFontColor(D3DCOLOR_RGBA(255,255,255,255));
		m_pkStaticClearSealDesc->SetAlign(DT_CENTER);
        m_pkStaticClearSealDesc->SetTextAutoMultiline(g_pkStrLoader->GetString( STR_ID_COORDI_GACHA_SYSTEM_MESSAGE01 )); //������� ���� ��Ʈ��
    }

	if( m_pkBtnBuy )
	{
		m_pkBtnBuy->InitState(true, true, this);
		m_pkBtnBuy->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCCoordiClearSealDlg::OnClickBuy );
	}

	if( m_pkGachaBuyItemBox )
	{
		m_pkGachaBuyItemBox->InitState( false, true, this);
	}

    m_pkCheckBoxAutoClear->InitState( true, true, this );
    m_pkCheckBoxAutoClear->SetCheck( m_bAutoMode );
    m_pkCheckBoxAutoClear->DisconnectAllEventProcedure();
    m_pkCheckBoxAutoClear->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCCoordiClearSealDlg::OnClickAuto );

    InitializeData();
}

void KGCCoordiClearSealDlg::OnDestroy( void )
{
    ClearPaticle();
    m_iCurCharType = 0;
    DeleteMeshObject();
    SAFE_RELEASE( m_pkBackTexture );
}

void KGCCoordiClearSealDlg::PostDraw( void )
{
    RenderPreview();
}

void KGCCoordiClearSealDlg::PostChildDraw( void )
{
    g_ParticleManager->Render(GC_LAYER_GACHA);
    if( m_pkGetItemImage != NULL )
    {
        m_pkGetItemImage->Render();
    }
}

void KGCCoordiClearSealDlg::FrameMoveInEnabledState( void )
{
    if ( g_pkInput->IsDown( DIK_ESCAPE ) && g_pkUIScene->IsCashBoxRenderOn() == false)
    {
        if( 0 >= m_iParticleLife )
            OnExit();

        g_pkInput->IgnoreDown(DIK_ESCAPE,30);
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

    // ������ ����Box ó��
    bool bCheck = false;
    const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );
    for(int iLoop=0; iLoop<EREWORD_ITEM_NUM; ++iLoop )
    {
        // ��������� ����Box ó��
        if( m_pkImgRewordEquip[iLoop]->CheckPosInWindowBound( vMousePos ) && int(m_vecRewardEuipItems.size()) > iLoop )
        {
            D3DXVECTOR2 vecPos =m_pkImgRewordEquip[iLoop]->GetCurrentWindowPos();
            RenderItemInfoBox( vecPos, m_vecRewardEuipItems[iLoop] );
            bCheck = true;
            break;
        }

        // �׼����� ������ ����Box ó��
        else if( iLoop < EREWORD_ACC_NUM && m_pkImgRewordAcc[iLoop]->CheckPosInWindowBound( vMousePos ) && int(m_vecRewardAccItems.size()) > iLoop )
        {
            D3DXVECTOR2 vecPos =m_pkImgRewordAcc[iLoop]->GetCurrentWindowPos();
            RenderItemInfoBox( vecPos, m_vecRewardAccItems[iLoop] );
            bCheck = true;
            break;
        }
    }    

    // ����� ������ ����Box ó��
    if( m_pkImgTakeEquip->CheckPosInWindowBound( vMousePos ) )
    {
        if( m_iCurCharType >= 0 && m_iCurCharType < GC_CHAR_NUM )
        {
            D3DXVECTOR2 vecPos = m_pkImgTakeEquip->GetCurrentWindowPos();
            RenderItemInfoBox( vecPos, m_pkImgTakeEquip->GetItemID() );
            bCheck = true;
        }        
    }

    // �������� ������ ����Box ó��
    if( m_pkImgTakeAcc->CheckPosInWindowBound( vMousePos ) )
    {
        D3DXVECTOR2 vecPos = m_pkImgTakeAcc->GetCurrentWindowPos();
        RenderItemInfoBox(vecPos, m_pkImgTakeAcc->GetItemID() );
        bCheck = true;
    }

    if( !bCheck )
    {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
    }

    FrameMove_Preview();
}

void KGCCoordiClearSealDlg::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == D3DWE_COMBO_SELCHANGE )
    {
        UpdatePromotion( m_pkJobCombo->GetCurSel() );
    }
}

void KGCCoordiClearSealDlg::RenderItemInfoBox( D3DXVECTOR2& vPos_, GCITEMID dwItemID_ )
{
	if( m_pkGachaBuyItemBox->IsRenderOn() )
		return;

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

void KGCCoordiClearSealDlg::OnExit()
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

void KGCCoordiClearSealDlg::OnClickBuy()
{
	if( 0 < m_iParticleLife )
		return;

	m_pkGachaBuyItemBox->SetItems(m_vecCurBuyItemList);
	m_pkGachaBuyItemBox->ToggleRender(true);
	m_pkGachaBuyItemBox->SetOrder_Top();
}

void KGCCoordiClearSealDlg::OnClickPetView()
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
	g_pkUIScene->m_pkPetPreview->SetNowPet( iPetID, 0 ); // ������ �Ĵ³༮�̴� ��ȭ������ 0 �̰���..
}


void KGCCoordiClearSealDlg::OnEnterCursor()
{
    g_KDSound.Play( "30" );
}

void KGCCoordiClearSealDlg::OnClickClearSealButton( DWORD dwType )
{
//#if !defined(__PATH__)

	if( m_pkGachaBuyItemBox->IsRenderOn() )
		return;

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

//         if( m_dwGachaItemType == 0 ) {   // ��� �� ���� 
//             m_pkJobCombo->SetCurSel( 0 );
//         }

        m_dwGachaItemType = dwType;
        RollGacha();
    }
}

void KGCCoordiClearSealDlg::RollGacha()
{
    IsProccessedGacha = false;
    bool bCheckAvailable = true;
    if( m_iCurBeadItemNum <= 0 )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NOT_ENOUGH_BEAD_ITEM ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        bCheckAvailable = false;
    }
    else if (ENEED_SPELLITEM_NUM > m_iCurSpellItemNum )
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

    KP2P::GetInstance()->Send_GachaActionReq(m_pkImgBeadItem->GetItemID(), 0, m_dwGachaItemType, static_cast<char>(m_iCurCharType), m_iGachaVer );
}

bool KGCCoordiClearSealDlg::IsOpenUI()
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

void KGCCoordiClearSealDlg::InitializeData()
{
    static bool bFirstLoad = true;
    if( bFirstLoad )
    {
        InitPreview();

        // ��í ������ ���� ������
        m_mapSealBead.clear();
        std::map<DWORD, int> mapGachaList = g_pItemMgr->GetGachaList();
        for(std::map<DWORD, int>::iterator mapCachaIter = mapGachaList.begin(); mapCachaIter != mapGachaList.end(); ++mapCachaIter)
        {
            m_mapSealBead.insert( std::pair<int,DWORD>(mapCachaIter->second, mapCachaIter->first) );
        }

        // ��ũ��Ʈ���� �Ǹ��� ������ ����Ʈ ������
        if( false == LoadScriptBuyItemList() )
        {
            ASSERT( !"Gacha Script Load Error!!" );
        }
    }

    if( m_setObtainedBeadList.empty() )
        KP2P::GetInstance()->Send_ObtainedGachaSetRewardList();

    // ĳ���� ComboBox�� String���( ���α����� �ִ� ����鸸 )
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

    //GachaStartTextAlert();
}

void KGCCoordiClearSealDlg::InitPromotion( int iCharType )
{
    // ����ComboBox�� String���
    m_pkJobCombo->DeleteAllItem();
    m_pkJobCombo->AddString( g_pkStrLoader->GetString(STR_ID_PROMOTION_4), DT_CENTER );
    std::set< char > setPromotion;
    g_kGlobalValue.GetOpenedPromotion( iCharType, setPromotion );
    for( std::set< char >::iterator sit = setPromotion.begin(); sit != setPromotion.end(); ++sit ) {
        m_pkJobCombo->AddString( g_pkStrLoader->GetString(STR_ID_PROMOTION_0+static_cast< int >( *sit ) ), DT_CENTER );
    }
}

void KGCCoordiClearSealDlg::UpdateCharacter(int iIndex)
{
    if( 0 < m_iParticleLife )
        return;

    // ���ȹ������ �ʱ�ȭ
    m_bGetAll = false;
    m_bGetEquipAll = false;
    m_bGetAccessoriesAll= false;

    // ĳ���� ����
    SelCharToCharType( iIndex );

    if( 0 > m_iCurCharType || m_iCurCharType > GC_CHAR_NUM )
        return;

    // ���� ������ ����
    if( false == m_mapSealBead.empty() )
        UpdateOwnItemList();

    // �Ǹ� ������ ����
    if( false == m_mapBuyItemList.empty() )
        UpdateBuyItemList();

    // �����ʱ�ȭ
    InitPromotion( m_iCurCharType );

    // ���⺸���� ȹ���ߴٸ� ȹ���� �������� ����������
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

void KGCCoordiClearSealDlg::SelCharToCharType(int iCharType)
{
    m_iCurCharType = iCharType;
}

void KGCCoordiClearSealDlg::UpdatePromotion(int iIndex)
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
            // ���������� ȹ���ߴٸ� Lock
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

void KGCCoordiClearSealDlg::UpdateOwnItemList()
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
            m_pkStaticBeadItem->SetText( 0 );

        // ������� ����
        UpdateRewardList( mapIter->second );

        // �������� ����
        UpdateFinalRewardList( mapIter->second );
    } 

	if (g_kGlobalValue.m_iCurrentGachaType == 2) {
		m_pkImgSealItem->SetItemInfo(KGCItemManager::ITEM_DIAMOND_CLEAR_SPELL_ITEM);
		if (g_pItemMgr->FindInventoryForItemID(KGCItemManager::ITEM_DIAMOND_CLEAR_SPELL_ITEM, &pkInvenItem))
		{
			m_pkStaticSealItem->SetText(pkInvenItem->m_nCount);
			m_iCurSpellItemNum = pkInvenItem->m_nCount;
		}
		else
		{
			m_pkStaticSealItem->SetText(0);
		}
	}
	else
	{
		m_pkImgSealItem->SetItemInfo(KGCItemManager::ITEM_CASH_CLEAR_SPELL_ITEM);
		if (g_pItemMgr->FindInventoryForItemID(KGCItemManager::ITEM_CASH_CLEAR_SPELL_ITEM, &pkInvenItem))
		{
			m_pkStaticSealItem->SetText(pkInvenItem->m_nCount);
			m_iCurSpellItemNum = pkInvenItem->m_nCount;
		}
		else
		{
			m_pkStaticSealItem->SetText(0);
		}
	}
}

void KGCCoordiClearSealDlg::SetMapGachaFinalRewardList( GCITEMID ItemID_, std::map<int, std::vector<GCITEMID>>& mapRewardItemList_ )
{
    m_mapStoredFinalRewardInfo[ItemID_] = mapRewardItemList_;
}

void KGCCoordiClearSealDlg::UpdateFinalRewardList(GCITEMID ItemID_)
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

void KGCCoordiClearSealDlg::UpdateBuyItemList()
{
    DWORD dwBeadItemID = m_pkImgBeadItem->GetItemID();
    std::map<DWORD, std::vector<DWORD> >::iterator mapBeadItemIter = m_mapBuyItemList.find( dwBeadItemID );
    if( mapBeadItemIter != m_mapBuyItemList.end() )
    {
        m_vecCurBuyItemList = mapBeadItemIter->second;
    }
}

void KGCCoordiClearSealDlg::UpdateRewardList(GCITEMID ItemID_)
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

void KGCCoordiClearSealDlg::SetMapGachaRewardList( GCITEMID ItemID_, std::map<int, std::set<GCITEMID>>& mapRewardItemList_)
{
    m_mapStoredRewardInfo[ItemID_] = mapRewardItemList_;
}

void KGCCoordiClearSealDlg::SetRewardItemList( std::map< int, std::set<GCITEMID> >& mapRewardItemList_ )
{
    // UI �ʱ�ȭ
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
            // �����ϰ� �ִ� ������ ó�� �� ���� ������ �̹��� ����
            std::set<GCITEMID>::iterator setIter = mapIter->second.begin();
            for(int iLoop=0; setIter != mapIter->second.end(); ++setIter, ++iLoop )
            {
                if ( g_pItemMgr->FindInventoryForItemIDCoordi( *setIter ) || SiKGCDepotManager()->CheckDepotItem( *setIter ) )
                {
                    iEquipItemInInven++;
                    m_pkWndRewordEquipOver[iLoop]->ToggleRender(false);
                }
                else
                {
                    m_pkWndRewordEquipOver[iLoop]->ToggleRender(true);
                }

                m_pkImgRewordEquip[iLoop]->ToggleRender( true );
                m_pkImgRewordEquip[iLoop]->SetItemInfo( *setIter );
                m_vecRewardEuipItems.push_back( *setIter );
            }

            // ��� �����ϰ� �ִ� ������ �̶��...
            if( iEquipItemInInven == int(mapIter->second.size()) )
            {
                // ����������ư ����
                m_pkBtnClearSealEquip->ToggleRender(false);
                m_pkBtnClearSealEquip->Lock(true);

                // ����ȹ���ư ����ó��
                //m_pkBtnTakeEquip->ToggleRender(true);

                // ���⺸���� �޾Ҵٸ� ������ Lock
                // ���⺸���� �ȹ޾Ҵµ� ���������� �ȵǾ��ٸ� Lock
                // �ƴϸ� Active
                bool bGetWeapon = false;
                DWORD dwBeadItemID = m_pkImgBeadItem->GetItemID();
                if( m_setObtainedBeadList.end() == m_setObtainedBeadList.find( std::make_pair(dwBeadItemID, ETYPE_EQUIP_ITEM) ) )
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
                m_pkStaticClearSealDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_COORDI_GACHA_SYSTEM_MESSAGE01));
            }
        }
        else if( mapIter->first == ETYPE_ACCESSORY_ITEM )
        {
            std::set<GCITEMID>::iterator setIter = mapIter->second.begin();

            for(int iLoop=0; setIter != mapIter->second.end(); ++setIter, ++iLoop )
            {
                if (g_pItemMgr->FindInventoryForItemIDCoordi(*setIter) || SiKGCDepotManager()->CheckDepotItem(*setIter))
                {
                    iAccItemInInven++;
                    m_pkWndRewordAccOver[iLoop]->ToggleRender(false);
                }
                else
                {
                    m_pkWndRewordAccOver[iLoop]->ToggleRender(true);
                }

                m_pkImgRewordAcc[iLoop]->ToggleRender( true );
                m_pkImgRewordAcc[iLoop]->SetItemInfo( *setIter );
                m_vecRewardAccItems.push_back( *setIter );
            }   

            // ��� �����ϰ� �ִ� ������ �̶��...
            if( iAccItemInInven == int(mapIter->second.size()) )
            {
                // ����������ư ����
                m_pkBtnClearSealAcc->ToggleRender(false);
                m_pkBtnClearSealAcc->Lock(true);

                // ����ȹ���ư ����ó��
                //m_pkBtnTakeAcc->ToggleRender(true);

                // ��� ������ �ȹ޾Ұų� �׼������� �޾����� Lock �ƴϸ� Active
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
                    m_pkStaticClearSealDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_COORDI_GACHA_SYSTEM_MESSAGE01));
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
}

void KGCCoordiClearSealDlg::SetFinalRewardtItemList( std::map<int, std::vector<GCITEMID>>& mapRewardItemList_ )
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

                if( g_pItemMgr->FindInventoryForItemID(*vecIter) ){
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

bool KGCCoordiClearSealDlg::LoadScriptBuyItemList()
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

void KGCCoordiClearSealDlg::LoadGachaKeyList()
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

//void KGCCoordiClearSealDlg::GachaStartTextAlert()
//{
//    m_pEffTextAlert = g_ParticleManager->CreateSequence( "Gacha_Text_Alert", 0, 0, 0.5f );
//    g_ParticleManager->SetLayer( m_pEffTextAlert, GC_LAYER_UI);
//}

void KGCCoordiClearSealDlg::ClearPaticle()
{
    m_bPaticleInit = false;
    if(m_pEffTextAlert)
        g_ParticleManager->DeleteSequence( m_pEffTextAlert );
}

//void KGCCoordiClearSealDlg::GachaStartBtnAlertAndStopTextAlert()
//{
//    g_ParticleManager->DeleteSequence(m_pEffTextAlert);
//    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Gacha_Btn1_Alert", 0, 0, 0.5f ), GC_LAYER_UI);
//    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Gacha_Btn2_Alert", 0, 0, 0.5f ), GC_LAYER_UI);
//}

void KGCCoordiClearSealDlg::OnRecvFinalReward()
{ 
    if( 0 < m_iParticleLife )
        return;

        KP2P::GetInstance()->Send_GachaSetRewardReq( m_pkImgBeadItem->GetItemID(), static_cast<DWORD>(ETYPE_EQUIP_ITEM), 0, m_pkImgTakeEquip->GetItemID(), static_cast<char>(m_iCurCharType), m_iGachaVer );
}

void KGCCoordiClearSealDlg::OnRecvFinalTotalReward()
{
    if( 0 < m_iParticleLife )
        return;

    KP2P::GetInstance()->Send_GachaSetRewardReq( m_pkImgBeadItem->GetItemID(), static_cast<DWORD>(ETYPE_ACCESSORY_ITEM), 0, m_pkImgTakeAcc->GetItemID(), static_cast<char>(m_iCurCharType), m_iGachaVer );
}

void KGCCoordiClearSealDlg::OnRecvGachaAck()
{
    ShowGachaEffect();
    Init_ItemImage();
}

void KGCCoordiClearSealDlg::Init_ItemImage()
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

void KGCCoordiClearSealDlg::FrameMove_ItemImage()
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

void KGCCoordiClearSealDlg::ShowGachaEffect()
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

void KGCCoordiClearSealDlg::AddObtainedBeadList( std::pair<DWORD,DWORD>& prItemInfo_ )
{
    m_setObtainedBeadList.insert( prItemInfo_ );
    InitRewardBtn();
    InitUseSealBtn();
    UpdateOwnItemList();
}

void KGCCoordiClearSealDlg::SetObtainedBeadList( std::set<std::pair<DWORD,DWORD>>& setObtainedBeadList_ )
{
    m_setObtainedBeadList = setObtainedBeadList_;
    InitRewardBtn();
    InitUseSealBtn();
    UpdateOwnItemList();
}

void KGCCoordiClearSealDlg::StartAuto()
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

void KGCCoordiClearSealDlg::OnStopAuto( void )
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

void KGCCoordiClearSealDlg::InitRewardBtn()
{
    //m_pkBtnTakeEquip->ToggleRender(m_bGetEquipAll);
    //m_pkBtnTakeAcc->ToggleRender(m_bGetAll);
}

void KGCCoordiClearSealDlg::InitUseSealBtn()
{
    m_pkBtnClearSealEquip->ToggleRender(!m_bGetEquipAll);
    if( m_bGetEquipAll )
        m_pkBtnClearSealAcc->ToggleRender(!m_bGetAccessoriesAll);
}

bool KGCCoordiClearSealDlg::IsGachaKeyItem(DWORD dwItemID)
{
    return ( m_vecGachaKeyItemList.end() != std::find(m_vecGachaKeyItemList.begin(),m_vecGachaKeyItemList.end(),dwItemID ) );
}

void KGCCoordiClearSealDlg::SetBeadSpellItemNum( int iBeadNum_, int iSpellNum_ )
{
    m_iCurBeadItemNum = iBeadNum_;
    m_iCurSpellItemNum = iSpellNum_;

    m_pkStaticBeadItem->SetText(iBeadNum_);
    m_pkStaticSealItem->SetText(iSpellNum_);
}

void KGCCoordiClearSealDlg::OnItemEquipPreview()
{
    std::vector<GCItem*> vecItem;
    vecItem.clear();

    // ���
    for( int i = 0; i < static_cast<int>(m_vecRewardEuipItems.size()); i++ )
    {
        vecItem.push_back( g_pItemMgr->GetItemData(m_vecRewardEuipItems[i]) );
    }

    // �׼�
    for( int i = 0; i < static_cast<int>(m_vecRewardAccItems.size()); i++ )
    {
        vecItem.push_back( g_pItemMgr->GetItemData(m_vecRewardAccItems[i]) );
    }

    // ���⺸��
    for( int i = 0; i < static_cast<int>(m_vecFinalRewardEuipItems.size()); i++ )
    {
        if( i == m_iCurCharJob )
            vecItem.push_back( g_pItemMgr->GetItemData(m_vecFinalRewardEuipItems[i]) );
    }
    
    UpdateMeshObject( &vecItem );
}

void KGCCoordiClearSealDlg::InitPreview()
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
    ::D3DXMatrixScaling( &matScale, 1.0f, 1.0f, 0.01f );
    ::D3DXMatrixTranslation( &matTran, 0.0f, 0.172f, 0.5f );
    m_matProject = matLook * matScale * matTran;

	::D3DXMatrixOrthoOffCenterLH(&m_matScreenProj, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT, 0, 0.f, 100.f);

    // ��� �ε� 
    if( m_pkBackTexture )
        SAFE_RELEASE( m_pkBackTexture );
    m_pkBackTexture = g_pGCDeviceManager2->CreateTexture( "showroom.dds" );
}

void KGCCoordiClearSealDlg::DeleteMeshObject( void )
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

void KGCCoordiClearSealDlg::RenderPreview( void )
{
    // ��� ���ϴ� �� 
    g_pGCDeviceManager2->Flush();
    D3DVIEWPORT9 orgViewport;

    D3DXMATRIX matRot,matTrans,matScale,matWorld;
    g_pd3dDevice->GetViewport( &orgViewport );
    g_pd3dDevice->SetViewport( &m_viewport );

    //g_pd3dDevice->SetFVF( SKINVERTEX::FVF );
    //g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );
    //g_MyD3D->m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER|D3DCLEAR_TARGET, 0, 1.0f, 0L );

    // ��� ���
//     g_pGCDeviceManager2->DrawInScreen( m_pkBackTexture,
//         105.0f * GC_SCREEN_DIV_WIDTH * g_pGCDeviceManager2->GetWindowScaleX(), 
//         245.0f * GC_SCREEN_DIV_WIDTH * g_pGCDeviceManager2->GetWindowScaleY(), 
//         285.0f * GC_SCREEN_DIV_WIDTH * g_pGCDeviceManager2->GetWindowScaleX(), 
//         490.0f * GC_SCREEN_DIV_WIDTH * g_pGCDeviceManager2->GetWindowScaleY(),
//         0.0f, 0.0f, 1.0f, 1.0f, false, 0.1f, 0xffffffff );

	g_pGCDeviceManager2->DrawInScreen(m_pkBackTexture,
		0,
		0,
		VIEWPORT_WIDTH,
		VIEWPORT_HEIGHT,
		0.0f, 0.0f, 1.0f, 1.0f, false, 0.1f, 0xffffffff);

    // ���� ���  
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
                    // ����� �ٲ����
                    m_pShopMotion = m_pActionMotion[m_iExtraCharNum];
                    m_iMotionCount = 0;
                    m_fCharX = 0.0f;// X�� �ʱ�ȭ

                    SiKGCFaceLift()->SetCurMotionName( m_pActionMotion[m_iExtraCharNum]->GetDeviceID() );
                }
            }
            else
            {
                if( m_pWaitMotion[m_iExtraCharNum] != NULL )
                {
                    // ����� �ٲ����
                    m_pShopMotion = m_pWaitMotion[m_iExtraCharNum];
                    m_fCharX = 0.0f;// X�� �ʱ�ȭ

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

void KGCCoordiClearSealDlg::FrameMove_Preview( void )
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

void KGCCoordiClearSealDlg::ChangeCharWndZoom( bool bZoom )
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

void KGCCoordiClearSealDlg::ProcessCharWndAngle( void )
{
    POINT CurrentMousePoint = g_pkInput->GetMousePos();

    m_fAngleMove = m_fStartAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * DELTAZOOM;
    m_fStageAngleMove = m_fStartStageAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * DELTAZOOM;
}

void KGCCoordiClearSealDlg::SetCoordiView( int iCharType_ )
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

void KGCCoordiClearSealDlg::UpdateMeshObject(  std::vector<GCItem*> *pvecItem )
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
        m_pMeshFootStage->m_strName = "�ڵ� ����";
#endif
        m_pMeshFootStage->AddMesh( g_pGCDeviceManager2->CreateMeshP3M( "footstage.p3m" ), g_pGCDeviceManager2->CreateTexture( "footstage.dds" ) );
    }
}

void KGCCoordiClearSealDlg::ResetPlayer( void )
{
    m_Player = g_MyD3D->m_TempPlayer;
    m_Player.m_kUserInfo = g_kGlobalValue.m_kUserInfo;
    SetChar( m_iCurCharType );

    OnItemEquipPreview();
}

void KGCCoordiClearSealDlg::SetChar( int iCharType )
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

void KGCCoordiClearSealDlg::InitCharacterInfo()
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

bool KGCCoordiClearSealDlg::CheckCondition( IN const KDialogInfo& kInfo_ )
{
    m_iGachaVer = static_cast<int>( kInfo_.m_lParam2 );
    if( m_iGachaVer == 0 ) { 
        return false;
    }
    
    return true;
}

void KGCCoordiClearSealDlg::OnClickAuto( void )
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

