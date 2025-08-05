#include "stdafx.h"
#include "GCNewNecklaceEnchantDlg.h"

IMPLEMENT_CLASSNAME( KGCNewNecklaceEnchantDlg );
IMPLEMENT_WND_FACTORY( KGCNewNecklaceEnchantDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCNewNecklaceEnchantDlg, "gc_newnecklaceenchantdlg" );

int g_iEnchantGemCountResult = 0;

const GCITEMID nMATERIAL_LIST[KGCNewNecklaceEnchantDlg::EM_MATERIAL_SIZE] =
{	GEM_ITEM_ID, CHOCOLATE_ID, ITEM_ID_HERO_COIN };

KGCNewNecklaceEnchantDlg::KGCNewNecklaceEnchantDlg( void )
{
	m_ItemID = 0;
	m_nStrongNum = 0;
	m_nMaterialType = EM_MATERIAL_GEM;
	m_mapEnchantGemCount.clear();
	
	
	m_dwState = EM_NORMAL_STATE;
	m_nResult = EM_SUCCESS_TYPE;
	m_iParticleTime = 0;

	m_fRatio = 1.0f;



	m_pkBtnOK = NULL;
	m_pkBtnCancel = NULL;

	m_pkItemImage = NULL;

	m_pkStaticDesc = NULL;
	m_pkStaticDescTitle = NULL;
	m_pkStaticItemName = NULL;
	m_pkStaticCostMaterialTitle = NULL;
	m_pkStaticHaveMaterialTitle = NULL;
	m_pkStaticCostMaterial = NULL;
	m_pkStaticHaveMaterial = NULL;
	m_pkStrongNum = NULL;
	m_pkAutoText = NULL;


	char str[MAX_PATH] = {0,};

	for ( int i = 0; i < EM_MATERIAL_SIZE; ++i )
	{
		m_pkBtnCheck[i] = NULL;
		m_pkStaticCheck[i] = NULL;

		sprintf( str, "item_check_btn%d", i );  
		LINK_CONTROL( str, m_pkBtnCheck[i] );

		sprintf( str, "static_check_btn%d", i );  
		LINK_CONTROL( str, m_pkStaticCheck[i] );
	}

	for ( int i = 0; i < EM_SIZE_ELENA; ++i ) 
	{
		m_pkElenaImage[i] = NULL;

		sprintf( str, "elena_image%d", i );  
		LINK_CONTROL( str, m_pkElenaImage[i] );
	}


	LINK_CONTROL( "btn_ok", m_pkBtnOK );
	LINK_CONTROL( "btn_cancel", m_pkBtnCancel );
	LINK_CONTROL( "item_img", m_pkItemImage );
	LINK_CONTROL( "static_desc_title", m_pkStaticDescTitle );
	LINK_CONTROL( "static_desc", m_pkStaticDesc );
	LINK_CONTROL( "static_item_name", m_pkStaticItemName );
	LINK_CONTROL( "static_cost_material_title", m_pkStaticCostMaterialTitle );
	LINK_CONTROL( "static_have_material_title", m_pkStaticHaveMaterialTitle );
	LINK_CONTROL( "static_cost_material", m_pkStaticCostMaterial );
	LINK_CONTROL( "static_have_material", m_pkStaticHaveMaterial );
	LINK_CONTROL( "item_strong", m_pkStrongNum );
	LINK_CONTROL( "wnd_auto_enchant", m_pkAutoText );
	LINK_CONTROL("checkbox_auto", m_pkCheckBoxAutoClear);

    m_pkTitleNecklace = NULL;
    m_pkTitleAnkle = NULL;

    LINK_CONTROL( "title_necklace", m_pkTitleNecklace );
    LINK_CONTROL( "title_ankle",    m_pkTitleAnkle );
}

KGCNewNecklaceEnchantDlg::~KGCNewNecklaceEnchantDlg( void )
{
}

void KGCNewNecklaceEnchantDlg::OnCreate( void )
{
	for ( int i = 0; i < EM_MATERIAL_SIZE; ++i )
	{
		m_pkStaticCheck[i]->SetFontColor(0xffE9EEF3);
		m_pkStaticCheck[i]->SetFontOutline(true,0xff14212f);
		m_pkStaticCheck[i]->SetFontSpace( 1.0f );
		m_pkStaticCheck[i]->SetText( g_pkStrLoader->GetString( STR_ID_NECK_ENCHANT_MATERIAL_TYPE1+i ));
	}

	for ( int i = 0; i < EM_SIZE_ELENA; ++i ) 
	{
		m_pkElenaImage[i]->ToggleRender( false );
	}

	m_pkStaticDescTitle->SetFontColor(0xffE9EEF3);
	m_pkStaticDescTitle->SetFontOutline(true,0xff14212f);
	m_pkStaticDescTitle->SetFontSpace( 1.0f );
	m_pkStaticDescTitle->SetText( g_pkStrLoader->GetString( STR_ID_ENCHANT_QUERY1 ) );

	m_pkStaticDesc->SetFontColor(0xffE9EEF3);
	m_pkStaticDesc->SetFontOutline(true,0xff14212f);
	m_pkStaticDesc->SetFontSpace( 1.0f );
	m_pkStaticDesc->SetMultiLine( true );

	m_pkStaticItemName->SetFontColor(0xffffc502);
	m_pkStaticItemName->SetFontOutline(true,0xff14212f);
	m_pkStaticItemName->SetFontSpace( 1.0f );

	m_pkStaticCostMaterialTitle->SetFontColor(0xffE9EEF3);
	m_pkStaticCostMaterialTitle->SetFontOutline(true,0xff14212f);
	m_pkStaticCostMaterialTitle->SetFontSpace( 1.0f );
	m_pkStaticCostMaterialTitle->SetAlign(DT_CENTER);

	m_pkStaticHaveMaterialTitle->SetFontColor(0xffE9EEF3);
	m_pkStaticHaveMaterialTitle->SetFontOutline(true,0xff14212f);
	m_pkStaticHaveMaterialTitle->SetFontSpace( 1.0f );
	m_pkStaticHaveMaterialTitle->SetAlign(DT_CENTER);

	m_pkStaticCostMaterial->SetFontColor(0xffE81919);
	m_pkStaticCostMaterial->SetAlign(DT_CENTER);

	m_pkStaticHaveMaterial->SetFontColor(0xffE9EEF3);
	m_pkStaticHaveMaterial->SetAlign(DT_CENTER);

	m_pkBtnOK->InitState( true, true, this );
	m_pkBtnOK->SetHotKey( DIK_RETURN );
	m_pkBtnCancel->InitState( true, true, this );
	m_pkBtnCancel->SetHotKey( DIK_ESCAPE );

	m_pkItemImage->InitState(true);
	m_pkStrongNum->InitState(true);
	m_pkStrongNum->SetStringType( "yellow_num" );

	m_pkAutoText->InitState(true);

	for ( int i = 0; i < EM_MATERIAL_SIZE; ++i )
	{
		m_pkBtnCheck[i]->InitState( true, true, this );
		m_pkBtnCheck[i]->ConnectEventProcedure<KGCNewNecklaceEnchantDlg,void>( D3DWE_BUTTON_CLICK, this, boost::bind( &KGCNewNecklaceEnchantDlg::OnClickCheckBtn, _1, i ) );
	}

	//	최고급 젬 임시 보류
	m_pkBtnCheck[EM_MATERIAL_HIGH_GEM]->ToggleRender( false );
	m_pkStaticCheck[EM_MATERIAL_HIGH_GEM]->ToggleRender( false );


    m_pkTitleNecklace->InitState( false, false, NULL );
    m_pkTitleAnkle->InitState( false, false, NULL );

	m_pkCheckBoxAutoClear->InitState(true, true, this);
	m_pkCheckBoxAutoClear->SetCheck(m_bAutoMode);
	m_pkCheckBoxAutoClear->DisconnectAllEventProcedure();
	m_pkCheckBoxAutoClear->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCNewNecklaceEnchantDlg::OnClickAuto);
}

void KGCNewNecklaceEnchantDlg::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP(m_pkBtnOK, KD3DWnd::D3DWE_BUTTON_CLICK, OnOK);
	GCWND_MSG_MAP( m_pkBtnCancel, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
}

bool KGCNewNecklaceEnchantDlg::CheckCondition(  IN const KDialogInfo& kInfo_ )
{
	SetItemInfo ( static_cast<GCITEMUID>( kInfo_.m_lParam ) );
	return true;
}

void KGCNewNecklaceEnchantDlg::SetItemInfo( GCITEMUID _itemUID )
{
	m_ItemUID = _itemUID;

	UpdateItemID();
	
	if ( m_mapEnchantGemCount.empty() )
	{
		g_iEnchantGemCountResult = 1;
		KP2P::GetInstance()->m_pkUserProxy->SendID( KUserEvent::EVENT_ENCHANT_GEMCOUNT_REQ );
		g_MyD3D->WaitForServerAck( g_iEnchantGemCountResult,1,5000,2);
	}
}

void KGCNewNecklaceEnchantDlg::LockButton( bool _bLock )
{
	m_pkBtnOK->Lock( _bLock );
	m_pkBtnCancel->Lock( _bLock );

	for ( int i = 0; i < EM_MATERIAL_SIZE; ++i ) 
	{
		m_pkBtnCheck[i]->Lock( _bLock );
	}
}

void KGCNewNecklaceEnchantDlg::SetResult( int nResult )
{
	switch ( nResult )
	{
	case 0:
		{
			RenderResultParticle( EM_SUCCESS_TYPE );
			ToggleElenaImage( EM_SUCCESS_ELENA );
		}
		break;
	case 1:
		{
			RenderResultParticle( EM_FAIL_DOWN_TYPE );
			ToggleElenaImage( EM_FAIL_ELENA );
		}
		break;
	case 2:
		{
			RenderResultParticle( EM_FAIL_BREAK_TYPE );
			ToggleElenaImage( EM_FAIL_ELENA );
		}
		break;
	case 3:
		{
			RenderResultParticle( EM_FAIL_NONE_TYPE );
			ToggleElenaImage( EM_FAIL_ELENA );
		}
		break;
	case -1:
		{
			OnClose();
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_FAIL8 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
			return;
		}
		break;
	case -5: 
		{
			OnClose();
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_FAIL9 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
			return;
		}
		break;
	default:
		{
			OnClose();
            std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_STRONG_LEVELUP_ACK, nResult );	
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_UNKWON_ERROR), strTemp, 
                KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true );
			return;
		}
		break;
	}

	m_iParticleTime = 60;
	m_dwState = EM_RESULT_STATE;
}
	
void KGCNewNecklaceEnchantDlg::RenderResultParticle( int nResultType )
{
	switch ( nResultType ) 
	{
	case EM_SUCCESS_TYPE:
		{
			g_KDSound.Play( "Enchant_Success" );
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("success01", 0.015f, 0.11f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("success02", 0.015f, 0.11f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("success03", 0.015f, 0.11f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Elena_Suc_01", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Elena_Suc_02", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Elena_Suc_03", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);			
		}
		break;
	case EM_FAIL_NONE_TYPE:
		{
			g_KDSound.Play( "Enchant_Fail" );
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("fail01", 0.015f, 0.11f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("fail02", 0.015f, 0.11f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Elena_Fail", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);			
		}
		break;
	case EM_FAIL_DOWN_TYPE:
		{
			g_KDSound.Play( "Enchant_Fail" );
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("fail01", 0.015f, 0.11f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("fail02", 0.015f, 0.11f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("fail05", 0.015f, 0.11f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("fail06", 0.015f, 0.11f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Elena_Fail", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);			
		}
		break;
	case EM_FAIL_BREAK_TYPE:
		{
			g_KDSound.Play( "Enchant_Break" );            
			g_KDSound.Play( "Enchant_Fate" );
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("fail01", 0.015f, 0.11f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("fail02", 0.015f, 0.11f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("fail03", 0.015f, 0.11f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("fail04", 0.015f, 0.11f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Elena_Fail", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);			
		}
		break;
	}

	m_nResult = nResultType;
}

void KGCNewNecklaceEnchantDlg::ToggleElenaImage( int nElenaType )
{
	if ( nElenaType < EM_NORMAL_ELENA || nElenaType >= EM_SIZE_ELENA )
		nElenaType = EM_NORMAL_ELENA;

	for ( int i = 0; i < EM_SIZE_ELENA; ++i ) 
	{
		if ( i == nElenaType ) 
			m_pkElenaImage[i]->ToggleRender( true );
		else
			m_pkElenaImage[i]->ToggleRender( false );
	}
}

void KGCNewNecklaceEnchantDlg::OnCreateComplete( void )
{
	m_dwState = EM_NORMAL_STATE;
	m_nResult = EM_SUCCESS_TYPE;
	m_iParticleTime = 0;

	ToggleElenaImage( EM_NORMAL_ELENA );
	LockButton( false );
	UpdateEnchantDlg();
}

void KGCNewNecklaceEnchantDlg::RenderItemInfoBox( D3DXVECTOR2& vPos_, GCITEMUID _itemUID )
{
	vPos_.x *= m_fWindowScaleX;
	vPos_.y *= m_fWindowScaleY;
	vPos_.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth() * g_pkUIScene->m_pkItemInformationBox->GetWindowScaleX();

	KItem* pInvenItem = g_pItemMgr->GetInventoryItem( _itemUID );
	if ( !pInvenItem ) 
		return;
	
	GCItem* pItem = g_pItemMgr->GetItemData(pInvenItem->m_ItemID);

	g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem, -1, pInvenItem, NULL, NULL, -1 );
	g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
	g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

	g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vPos_ );
	g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
}


void KGCNewNecklaceEnchantDlg::FrameMoveInEnabledState( void )
{
	switch (m_dwState)
	{
		case EM_NORMAL_STATE:
		{
			const D3DXVECTOR2 vMousePos = D3DXVECTOR2((float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y);

			bool bCheck = false;
			if (m_pkItemImage->CheckPosInWindowBound(vMousePos))
			{
				// 아이템 정보 표시창 위치 보정.
				D3DXVECTOR2 vecPos = m_pkItemImage->GetCurrentWindowPos();
				RenderItemInfoBox(vecPos, m_ItemUID);
				bCheck = true;
			}

			if (!bCheck)
			{
				g_pkUIScene->m_pkItemInformationBox->ToggleRender(false);
			}
		}
		break;
		case EM_ENCHANT_STATE:
		{
			m_iParticleTime--;

			if (m_iParticleTime == 60)
			{
				if (m_nStrongNum < 9)
				{
					KP2P::GetInstance()->Send_StrongLevelUpReq(m_ItemID, m_ItemUID, 0, nMATERIAL_LIST[m_nMaterialType]);
				}
				else {
					m_bAutoMode = false;
					m_pkCheckBoxAutoClear->SetCheck(m_bAutoMode);
					OnClose();
					return;
				}
			}
		}
		break;
		case EM_RESULT_STATE:
		{
			m_iParticleTime--;

			if (m_iParticleTime == 1)
			{
				//	깨졌으면 창 닫는다.
				if (m_nResult == EM_FAIL_BREAK_TYPE)
				{
					OnClose();
					return;
				}

				m_dwState = EM_NORMAL_STATE;
				ToggleElenaImage(EM_NORMAL_ELENA);

				if (!m_bAutoMode) {
					LockButton(false);
				}

				UpdateItemID();
				UpdateEnchantDlg();
			}
		}
		break;
	};
	if (m_bAutoMode && m_iParticleTime == 1) {
		OnOK();
	}
}

void KGCNewNecklaceEnchantDlg::PostChildDraw( void )
{
	g_ParticleManager->Render( GC_LAYER_UPPER_SQUARE_UI_0 );
}


void KGCNewNecklaceEnchantDlg::OnOK()
{
	if (m_nStrongNum >= 9) {
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, L"Você não pode fortificar mais este colar!", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
		if (m_bAutoMode) {
			m_bAutoMode = false;
			m_pkCheckBoxAutoClear->SetCheck(m_bAutoMode);
		}
		OnClose();
		return;
	}

	m_dwState = EM_ENCHANT_STATE;
	ToggleElenaImage(EM_ENCHANT_ELENA);

	g_KDSound.Play("Enchant_Start");
	g_KDSound.Play("Enchant_Run");

	if (m_nStrongNum < 6)
	{
		m_iParticleTime = 170; //110;
		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_Elena_01", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_Elena_02", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_Elena_03", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_Elena_04", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_Elena_05", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
	}
	else if (m_nStrongNum < 9)
	{
		m_iParticleTime = 205; //165 ;
		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_Elena_Long_01", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_Elena_Long_02", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_Elena_Long_03", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_Elena_Long_04", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_Elena_Long_05", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
	}
	else
	{
		m_iParticleTime = 205; //220;
		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_Elena_Long_01", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_Elena_Long_02", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_Elena_Long_03", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_Elena_Long_04", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_Elena_Long_05", 0.0f, 0.0f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
	}
}

void KGCNewNecklaceEnchantDlg::OnClose()
{
	g_KDSound.Play( "31" );
	m_bAutoMode = false;
	m_pkCheckBoxAutoClear->SetCheck(m_bAutoMode);
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCNewNecklaceEnchantDlg::OnClickCheckBtn( int nCheck )
{
	if (nCheck < EM_MATERIAL_GEM || nCheck >= EM_MATERIAL_SIZE) {
		m_bAutoMode = false;
		m_pkCheckBoxAutoClear->SetCheck(m_bAutoMode);
		return;
	}

	m_nMaterialType = nCheck;

	UpdateCheckBtn();
	UpdateMaterialText();
	UpdateFailText();
}

void KGCNewNecklaceEnchantDlg::Init()
{
	m_nMaterialType = EM_MATERIAL_GEM;
	m_ItemID = 0;
	m_ItemUID = 0;
	m_nStrongNum = 0;

	m_dwState = EM_NORMAL_STATE;
	m_nResult = EM_SUCCESS_TYPE;
	m_iParticleTime = 0;
	ToggleElenaImage( EM_NORMAL_ELENA );
}

void KGCNewNecklaceEnchantDlg::UpdateEnchantDlg()
{
	UpdateCheckBtn();
	UpdateTextControl();
	UpdateItemImgae();
}

void KGCNewNecklaceEnchantDlg::UpdateItemID()
{
	KItem* kItem = g_pItemMgr->GetInventoryItem( m_ItemUID );
	if ( kItem )
		m_ItemID = kItem->m_ItemID;
	else
		m_ItemID = 0;
}

void KGCNewNecklaceEnchantDlg::UpdateTextControl()
{
	GCItem* pItem = g_pItemMgr->GetItemData( m_ItemID );
	if ( !pItem )
		return;

    m_pkStaticItemName->SetText( pItem->strItemName );

    // 목걸이 / 발찌 따라 UI 제목 바꿔줌
    if( pItem->dwSlotPosition & ESP_A_NECKLET ) { 
        m_pkTitleNecklace->ToggleRender( true );
        m_pkTitleAnkle->ToggleRender( false );
    }
    else { 
        m_pkTitleNecklace->ToggleRender( false );
        m_pkTitleAnkle->ToggleRender( true );
    }
	
	UpdateMaterialText();
	UpdateFailText();
}

void KGCNewNecklaceEnchantDlg::UpdateItemImgae()
{
	GCItem* pItem = g_pItemMgr->GetItemData( m_ItemID );
	if ( !pItem )
		return;

	m_pkItemImage->SetItemInfo( m_ItemID );
	m_pkItemImage->ShowItem( true );

	m_nStrongNum = pItem->iStrongLevel;

	//	강화 수치 표시
	m_pkStrongNum->SetNumber( m_nStrongNum );
}

void KGCNewNecklaceEnchantDlg::UpdateCheckBtn()
{
	for ( int i = 0; i < EM_MATERIAL_SIZE; ++i ) 
	{
		m_pkBtnCheck[i]->SetCheck( false );
	}

	m_pkBtnCheck[m_nMaterialType]->SetCheck( true );

}

void KGCNewNecklaceEnchantDlg::UpdateMaterialText()
{
	std::wstring strMaterial;
	std::wstring strTemp;
	strMaterial = g_pkStrLoader->GetString( STR_ID_NECK_ENCHANT_MATERIAL_TYPE1+m_nMaterialType );

	strTemp = strMaterial + g_pkStrLoader->GetString( STR_ID_NECK_ENCAHNT_COST_MATERIAL_TITLE );		
	m_pkStaticCostMaterialTitle->SetForceWordWrapText( strTemp , true);
    m_pkStaticCostMaterialTitle->SetToolTip(strTemp);
    
	strTemp = strMaterial + g_pkStrLoader->GetString( STR_ID_NECK_ENCAHNT_HAVE_MATERIAL_TITLE );	
	m_pkStaticHaveMaterialTitle->SetForceWordWrapText( strTemp, true );
    m_pkStaticHaveMaterialTitle->SetToolTip(strTemp);

	int nCount = g_pItemMgr->GetInventoryItemDuration( nMATERIAL_LIST[m_nMaterialType] );
	int nCost = GetEnchantCost();

	m_pkStaticCostMaterial->SetText( g_pkStrLoader->GetReplacedString( STR_ID_DURATION_COUNT, "i", nCost ));	
	m_pkStaticHaveMaterial->SetText( g_pkStrLoader->GetReplacedString( STR_ID_DURATION_COUNT, "i", nCount ));

	if ( nCost > nCount || nCost <= 0 ) 
	{
		m_pkBtnOK->Lock( true );
	}
	else
	{
		m_pkBtnOK->Lock( false );
	}
}

void KGCNewNecklaceEnchantDlg::UpdateFailText()
{
	GCItem* pItem = g_pItemMgr->GetItemData( m_ItemID );
	if ( !pItem )
		return;

	int nStrongLevel = pItem->iStrongLevel;
	bool bCash = ( EMT_CASH == pItem->eMoneyType );
	bool bHighGem = ( m_nMaterialType == EM_MATERIAL_HIGH_GEM );

	int nMessageIndex = 0;
	
	if ( nStrongLevel > 5 && !bCash && !bHighGem )
	{
		nMessageIndex = 2;
	}
	else if ( nStrongLevel > 2 && !bHighGem )
	{
		nMessageIndex = 1;
	}

	
	std::wstring strDesc;	
	for ( int i = 0; i <= nMessageIndex; ++i )
	{
		strDesc += g_pkStrLoader->GetReplacedString( STR_ID_ENCHANT_QUERY2 + i, "i", i+1 );
		strDesc += L"\n";
	}

	m_pkStaticDesc->SetText( strDesc );
}

int KGCNewNecklaceEnchantDlg::GetEnchantCost()
{
	if ( m_ItemID == 0 ) 
		return 0;

	GCItem* pItem = g_pItemMgr->GetItemData( m_ItemID );
	if ( !pItem )
		return 0;

	std::map<DWORD, DWORD>::iterator pos = m_mapEnchantGemCount.find( pItem->iStrongLevel + 1 );
	if ( pos == m_mapEnchantGemCount.end() )
		return 0;

	int nCount = pos->second;
	nCount = static_cast<int>(nCount * m_fRatio + 0.5f);

	return nCount;
}

void KGCNewNecklaceEnchantDlg::OnClickAuto(void)
{
	if (m_pkCheckBoxAutoClear == NULL) {
		return;
	}

	m_pkCheckBoxAutoClear->SetCheck(!m_pkCheckBoxAutoClear->GetCheck());
	m_bAutoMode = m_pkCheckBoxAutoClear->GetCheck();
	if (m_bAutoMode) {
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, L"Ao clicar no modo automático, a fortificação irá continuar até os itens de fortificação acabarem, seu acessório chegar ao nível máximo ou se você desmarcar a caixa automático.", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
	}
}