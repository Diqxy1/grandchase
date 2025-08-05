#include "stdafx.h"
#include ".\GCGambleBox.h"
#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCGambleBox );
IMPLEMENT_WND_FACTORY( KGCGambleBox );
IMPLEMENT_WND_FACTORY_NAME( KGCGambleBox, "gc_gamble_box" );

KGCGambleBox::KGCGambleBox(void)
{

	char temp[MAX_PATH];
	for( int i = 0; i < MAX_WANNA_DO_WND; ++i )
	{
		sprintf( temp, "wanna_do_%d", i );
		LINK_CONTROL( temp, m_itemwndwannado[i] );
	}

	for( int i = 0; i < MAX_I_GOT_WND; ++i )
	{
		sprintf( temp, "iGot_%d", i );
		LINK_CONTROL( temp, m_itemwndGot[i] );
	}

	LINK_CONTROL( "btn_buy", m_pkBtnBuy );
	
	m_fCostRate = 1.0f;
	m_dwTotalPrice = 0;
}

KGCGambleBox::~KGCGambleBox(void)
{

}

void KGCGambleBox::OnCreate( void )
{
	GetChildByName("btn_end2")->InitState( true, true ,this );
	GetChildByName("btn_end")->InitState( true, true ,this );
	GetChildByName("btn_end")->SetHotKey( DIK_ESCAPE );

	GetChildByName("background")->SetPredraw( true );
	GetChildByName("background")->ToggleRender( true );
	GetChildByName("bg1")->SetPredraw( true );
	GetChildByName("bg2")->SetPredraw( true );


	GetChildByName("btn_filter")->InitState( true, true ,this );
	GetChildByName("btn_pre")->InitState( true, true ,this );
	GetChildByName("btn_ne")->InitState( true, true ,this );
	((KD3DStatic*)GetChildByName("static_gp"))->SetAlign(DT_CENTER|DT_VCENTER);
	((KD3DStatic*)GetChildByName("static_ost"))->SetAlign(DT_CENTER|DT_VCENTER);
	((KD3DStatic*)GetChildByName("static_page"))->SetAlign(DT_CENTER|DT_VCENTER);

	((KD3DStatic*)GetChildByName("static_gp"))->SetOffset( D3DXVECTOR2( 0.0f, 4.0f ) );
	((KD3DStatic*)GetChildByName("static_ost"))->SetOffset( D3DXVECTOR2( 0.0f, 4.0f ) );
	((KD3DStatic*)GetChildByName("static_page"))->SetOffset( D3DXVECTOR2( 0.0f, 4.0f ) );

	((KD3DStatic*)GetChildByName("static_info1"))->SetMultiLine( true );
	((KD3DStatic*)GetChildByName("static_info2"))->SetMultiLine( true );

	((KD3DStatic*)GetChildByName("static_info1"))->SetTextAutoMultiline( g_pkStrLoader->GetString( 6449 ) );
	((KD3DStatic*)GetChildByName("static_info2"))->SetTextAutoMultiline( g_pkStrLoader->GetString( 6452 ) );

	for( int i = 0; i < MAX_WANNA_DO_WND; ++i )
	{		
		m_itemwndwannado[i]->InitState( true, true, this );
	}

	for( int i = 0; i < MAX_I_GOT_WND; ++i )
	{
		m_itemwndGot[i]->InitState( true, true, this );
	}

	m_pkBtnBuy->InitState( true, true ,this );

	m_iCurrentPage = 0;
	

	//m_vecGot = g_MyD3D->m_kItemMgr.GetInvenItemFromKind( GIK_NORMAL );	
}

void KGCGambleBox::ActionPerformed( const KActionEvent& event )
{
    switch ( event.m_dwCode )
    {
    case KD3DWnd::D3DWE_CURSOR_ENTER:
        {
            g_KDSound.Play( "30" );
            break;
        }
    case KD3DWnd::D3DWE_BUTTON_DOWN:
        {
            g_KDSound.Play( "31" );
            break;
        }
    }

	if ( event.m_dwCode != KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		return;
	}

	if( event.m_pWnd == GetChildByName("btn_end") || event.m_pWnd == GetChildByName("btn_end2") )
	{
		Destroy();
		return;
	}
	else if( event.m_pWnd == m_pkBtnBuy )
	{
		if ((int) m_dwTotalPrice > g_kGlobalValue.m_kUserInfo.GetCurrentChar().GetCurrentGP() )
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_BUY_GP_ITEM_FAILED ), L"",
				KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0,
				true, true  );
			return;
		}
		std::vector<GCITEMID> vec;
		for ( unsigned int i = 0; i < m_vecWanna.size(); i++ )
		{
			vec.push_back( m_vecWanna[i]->dwGoodsID * 10 );
		}
		
		int iInventoryCapacity = g_kGlobalValue.m_iInvenCapacity;
		if ( g_MyD3D->m_kItemMgr.GetTotalInventorySize() + (int)vec.size() > iInventoryCapacity )
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
				g_pkStrLoader->GetString( STR_ID_INVENTORY_FULL1 ) +
				g_pkStrLoader->GetReplacedString( STR_ID_INVENTORY_FULL2, "i", iInventoryCapacity ),
				g_pkStrLoader->GetString( STR_ID_INVENTORY_FULL3 ),KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true );
			return;
		}
		
		KP2P::GetInstance()->Send_BuyForGambleReq( vec, -1, -1 ); // EVENT_BUY_FOR_GAMBLE_REQ
		Result_Buy = INT_MAX;
		g_MyD3D->WaitForServerAck( Result_Buy, INT_MAX, 10000, 100 );		
	}
	else if( event.m_pWnd == GetChildByName( "btn_ne" ) )
	{
		m_iCurrentPage++; 
		OnUpdate();
	}	
	else if( event.m_pWnd == GetChildByName( "btn_pre" ) )
	{
		m_iCurrentPage--;
		OnUpdate();
	}
	else if( event.m_pWnd == GetChildByName( "btn_filter" ) )
	{
		SellEtcItem();		
		OnUpdate();
	}

	

	for ( unsigned int i = 0 ; i < MAX_I_GOT_WND; i++ )
	{
		if ( event.m_pWnd == m_itemwndGot[i] && m_itemwndGot[i]->IsShowItem() )
		{			
			KItem* kitem = g_MyD3D->m_kItemMgr.GetInventoryItem( m_vecGot[ i + ( MAX_I_GOT_WND * m_iCurrentPage )  ] );;
			GCItem* gcItem = g_MyD3D->m_kItemMgr.GetItemData( kitem->m_ItemID );
			std::wstring strItemColorName = g_pItemMgr->GetItemGradeColorKey( kitem->m_cGradeID )+ gcItem->strItemName + L"#cX";
			std::wostringstream strmText1;
			std::wostringstream strmText2;

#if defined ( _PORTUGUESE ) || defined( _ENGLISH ) || defined(_THAI_LANG) || defined( _SPANISH )
			strmText1 << strItemColorName;
#else
			strmText1 << strItemColorName << g_pkStrLoader->GetString( STR_ID_OBJECT_POSTPOSITION );
#endif
			strmText2 << g_pkStrLoader->GetReplacedString( STR_ID_SELL_PRICE, "i", gcItem->dwPrice / SELL_DIV_VALUE );

			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, strmText1.str(), strmText2.str(), KGCUIScene::GC_MBOX_USE_SELL, kitem->m_ItemUID,0, true, true );
		}
		
	}


	for ( unsigned int i = 0 ; i < MAX_WANNA_DO_WND; i++ )
	{
		if ( event.m_pWnd == m_itemwndwannado[i] && m_itemwndwannado[i]->IsShowItem() )
		{
			if( m_vecWanna.size() == 1 )
			{
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(6451), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true );
				return;
			}
			if( m_vecWanna.size() <= i )
			{
				return;
			}

			GCItem* gcItem = m_vecWanna[ i ];
			std::wstring strItemColorName = g_pItemMgr->GetItemGradeColorKey( gcItem->cItemGrade )+ gcItem->strItemName + L"#cX";
			std::wostringstream strmText1;
			std::wostringstream strmText2;

#if defined ( _PORTUGUESE ) || defined( _ENGLISH ) || defined(_THAI_LANG) || defined( _SPANISH )
			strmText1 << strItemColorName;
#else
			strmText1 << strItemColorName << g_pkStrLoader->GetString( STR_ID_OBJECT_POSTPOSITION );
#endif
			strmText2 << g_pkStrLoader->GetString( STR_ID_DO_YOU_WANTTO_DELETE );

			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, strmText1.str(), strmText2.str(), KGCUIScene::GC_MBOX_USE_GAMBLE_DELETE, gcItem->dwGoodsID ,0, true, true );
		}

	}

}


void KGCGambleBox::OnDestroy()
{
	m_vecWanna.clear();
	m_vecGot.clear();
    
}

bool KGCGambleBox::CheckCondition( IN const KDialogInfo& kInfo_ )
{
	if( m_vecWanna.empty() )
	{

		if( kInfo_.m_lParam > 0)
		{
			m_vecWanna.push_back( g_MyD3D->m_kItemMgr.GetItemData( static_cast<GCITEMID>(kInfo_.m_lParam )) );
		}

		if ( kInfo_.m_lParam2 > 0)
		{
			std::vector<int> vec = g_MyD3D->m_kItemMgr.GetSetItemList( static_cast<int>(kInfo_.m_lParam2) );

			for( unsigned int i = 0; i < vec.size(); i++ )
			{
				if( g_MyD3D->m_kItemMgr.GetItemData( vec[i] )->dwSlotPosition != ESP_SET_ITEM &&
					g_MyD3D->m_kItemMgr.GetItemData( vec[i] )->dwSlotPosition != ESP_WEAPON && 
                    g_MyD3D->m_kItemMgr.GetItemData( vec[i] )->eMoneyType != EMT_CASH )
					m_vecWanna.push_back( g_MyD3D->m_kItemMgr.GetItemData( vec[i] ) );
			}
		}
	}
	
	OnUpdate();
    return true;
}

void KGCGambleBox::OnUpdate( void )
{
	if ( m_vecGot.empty() )
	{
		m_iTotalPage = 0;
	}
	else
	{
		m_iTotalPage = ( m_vecGot.size() - 1) / MAX_I_GOT_WND;
	}

	//SortGot();

	if ( m_iCurrentPage > m_iTotalPage )
	{
		m_iCurrentPage = m_iTotalPage;
	}
	
	if ( m_iCurrentPage < 0 )
	{
		m_iCurrentPage = 0;
	}
	
	for( unsigned int i = 0 ; i < MAX_I_GOT_WND ; i++ )
	{
        KD3DSizingBox* pkRareBox = ( KD3DSizingBox* )m_itemwndGot[i]->GetChildByName("rare");
        if( pkRareBox != NULL ) {
		    pkRareBox->ToggleRender( false );
        }

        KD3DSizingBox* pkLegendBox = ( KD3DSizingBox* )m_itemwndGot[i]->GetChildByName("legend");
        if( pkLegendBox != NULL ) {
		    pkLegendBox->ToggleRender( false );
        }

        KD3DSizingBox* pkEpicBox = ( KD3DSizingBox* )m_itemwndGot[i]->GetChildByName("epi");
        if( pkEpicBox != NULL ) {
		    pkEpicBox->ToggleRender( false );
        }

		if ( m_vecGot.size() > i + ( MAX_I_GOT_WND * m_iCurrentPage ) )
		{
			KItem* kitem = g_MyD3D->m_kItemMgr.GetInventoryItem( m_vecGot[ i + ( MAX_I_GOT_WND * m_iCurrentPage )  ] );
			if( !kitem )
			{
				m_itemwndGot[i]->ShowItem( false );
				continue;
			}
			m_itemwndGot[i]->SetItemInfo( kitem->m_ItemID );
			
			m_itemwndGot[i]->ShowItem( true );
			if ( kitem->m_cGradeID == KItem::GRADE_RARE && pkLegendBox != NULL )
			{
				pkLegendBox->ToggleRender( true );
			}
			else if ( kitem->m_cGradeID == KItem::GRADE_EPIC && pkRareBox != NULL )
			{
				pkRareBox->ToggleRender( true );
			}
			else if ( kitem->m_cGradeID == KItem::GRADE_LEGEND && pkEpicBox != NULL )
			{
				pkEpicBox->ToggleRender( true );
			}
		}
		else
		{
			m_itemwndGot[i]->ShowItem( false );
		}
	}

	for( unsigned int i = 0 ; i < MAX_WANNA_DO_WND ; i++ )
	{
		if ( m_vecWanna.size() > i )
		{
			m_itemwndwannado[i]->SetItemInfo( m_vecWanna[i]->dwGoodsID );
			m_itemwndwannado[i]->ShowItem( true );
		}
		else
		{
			m_itemwndwannado[i]->ShowItem( false );
		}
	}	

    KD3DStatic* pkStatic = (KD3DStatic*)GetChildByName("static_gp");
    if( pkStatic != NULL ) {
	    pkStatic->SetText( g_kGlobalValue.m_kUserInfo.GetCurrentChar().GetCurrentGP() );
    }

	WCHAR temp[MAX_PATH];

	wsprintf(temp, L"%d / %d", m_iCurrentPage+1, m_iTotalPage+1 );	


    pkStatic = (KD3DStatic*)GetChildByName("static_page");
    if( pkStatic != NULL ) {
	    pkStatic->SetText( temp );
    }

	m_dwTotalPrice = 0;
	for( unsigned int i = 0 ; i < m_vecWanna.size() ; i++ )
	{
		m_dwTotalPrice += m_vecWanna[i]->dwPrice;
	}

	m_dwTotalPrice = (DWORD) ( (float)m_dwTotalPrice * m_fCostRate);

    pkStatic = (KD3DStatic*)GetChildByName("static_ost");
    if( pkStatic != NULL ) {
	    pkStatic->SetText( (int) m_dwTotalPrice );
    }
}

void KGCGambleBox::FrameMoveInEnabledState( void )
{
	const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );
	
	g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );

	for(int i=0;i< MAX_WANNA_DO_WND;++i)
	{
		if( m_itemwndwannado[i]->IsShowItem() && m_itemwndwannado[i]->CheckPosInWindowBound( vMousePos ) )
		{
			// 아이템 정보 표시창 위치 보정.
			D3DXVECTOR2 vecPos =m_itemwndwannado[i]->GetCurrentWindowPos();

			vecPos.x *= m_fWindowScaleX;
			vecPos.y *= m_fWindowScaleY;
			vecPos.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth() * g_pkUIScene->m_pkItemInformationBox->GetWindowScaleX();
			//vecPos.y += m_pkRewardItemList[i]->GetHeight() / 2.0f * m_pkRewardItemList[i]->GetWindowScaleY();

			GCItem* pItem = g_pItemMgr->GetItemData(m_itemwndwannado[i]->GetItemID());

			g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
			g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
			g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

			g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
            g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
			break;
		}
	}

	for(int i=0;i< MAX_I_GOT_WND;++i)
	{
		if( m_itemwndGot[i]->IsShowItem() && m_itemwndGot[i]->CheckPosInWindowBound( vMousePos ) )
		{
			// 아이템 정보 표시창 위치 보정.
			D3DXVECTOR2 vecPos = m_itemwndGot[i]->GetCurrentWindowPos();
			vecPos.x *= m_fWindowScaleX;
			vecPos.y *= m_fWindowScaleY;
			vecPos.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth() * g_pkUIScene->m_pkItemInformationBox->GetWindowScaleX();
			//vecPos.y += m_pkReceiveItem[i]->GetHeight() / 2.0f * m_pkReceiveItem[i]->GetWindowScaleY();

			GCItem* pItem = g_pItemMgr->GetItemData(m_itemwndGot[i]->GetItemID());
			KItem* kitem = g_MyD3D->m_kItemMgr.GetInventoryItem( m_vecGot[ i + ( MAX_I_GOT_WND * m_iCurrentPage )  ] );;


			g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem,-1, kitem);
			g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
			g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

			g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
            g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
			break;
		}
	}

	

	//((KD3DStatic*)GetChildByName("static_ost"))->SetText( dwTotalPrice );
}

void KGCGambleBox::RemoveItem( GCITEMUID uid )
{
	std::vector<GCITEMUID>::iterator vit = m_vecGot.begin();
	for( ; vit != m_vecGot.end(); vit++ )
	{
		if( *vit == uid )
		{
			m_vecGot.erase( vit );
			OnUpdate();
			return;
		}
	}

	
}

void KGCGambleBox::DeleteItem( DWORD goodsID )
{
	std::vector<GCItem*>::iterator vit = m_vecWanna.begin();
	for( ; vit != m_vecWanna.end(); vit++ )
	{
		if( (*vit)->dwGoodsID == goodsID )
		{
			m_vecWanna.erase( vit );
			OnUpdate();
			return;
		}
	}
}

void KGCGambleBox::SortGot()
{
	if (m_vecGot.empty())
	{
		return;
	}
	
	class ItemGradeCmp
	{
	public:
		bool operator()(const GCITEMUID& p, const GCITEMUID& q) const
		{
			KItem* itemp = g_MyD3D->m_kItemMgr.GetInventoryItem( p );
			KItem* itemq = g_MyD3D->m_kItemMgr.GetInventoryItem( q );
			return itemp->m_cGradeID > itemq->m_cGradeID;
		}
	};

	sort(m_vecGot.begin(), m_vecGot.end(), ItemGradeCmp());
}

void KGCGambleBox::SellEtcItem()
{
	
	std::vector<KItem> vecUselessItem;
	std::vector<GCITEMUID> vecRemainItem = m_vecGot;

	for ( unsigned int i = 0; i < m_vecGot.size() ; i++ )
	{
		KItem* pItem = g_MyD3D->m_kItemMgr.GetInventoryItem( m_vecGot[i] );

		if ( !pItem )
		{
			continue;
		}
		
		if ( pItem->m_cGradeID == 0 )
		{
			vecUselessItem.push_back( *pItem );
			vecRemainItem[i] = -1;
		}
	}
	
	if ( vecUselessItem.empty() )
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( 6454 ), L"",
			KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0,
			true, true  );
		return;
	}
	else
	{
		g_MyD3D->m_kItemMgr.SellItem( vecUselessItem );
	}

	Result_Drop = INT_MAX;
	g_MyD3D->WaitForServerAck( Result_Drop, INT_MAX, 10000, 100 );	
	if( Result_Drop == 0 )
	{
		std::vector<GCITEMUID>::iterator vit = vecRemainItem.begin();
		while( vit != vecRemainItem.end() )
		{
			if ( (*vit) == -1 )
			{
				vit = vecRemainItem.erase( vit );
			}
			else
			{
				vit++;
			}
		}

		m_vecGot = vecRemainItem;
	}

	
}

void KGCGambleBox::InsertGambledItem( std::vector<KItem>& vecItem )
{
	m_iCurrentPage = INT_MAX;

	bool bSuccess = false;
	for( unsigned int i = 0 ; i < vecItem.size(); i++ )
	{
		if ( vecItem[i].m_cGradeID )
		{
			bSuccess = true;
		}
		g_pkUIScene->m_pkGambleBox->m_vecGot.push_back( vecItem[i].m_ItemUID );
	}

	if( bSuccess )
		g_KDSound.Play( "991027" );
	else
		g_KDSound.Play( "868" );

	OnUpdate();

	unsigned int iSize = vecItem.size();

	for( int i = 8 ; i >= 0 ; i-- )
	{
		if ( m_itemwndGot[i]->IsShowItem() )
		{
			D3DXVECTOR2 vPos = m_itemwndGot[i]->GetFixedWindowPos();
			vPos.x += (float)(m_itemwndGot[i]->GetWidth() / 2);
			vPos.y += (float)(m_itemwndGot[i]->GetHeight() / 2);

			g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Gemble_01",(vPos.x - 400.0f)/ 400.0f,(-vPos.y + 300.0f)/ 400.0f, 0.5f ), GC_LAYER_UI); 
			g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Gemble_02",(vPos.x - 400.0f)/ 400.0f,(-vPos.y + 300.0f)/ 400.0f, 0.5f ), GC_LAYER_UI); 
			//이펙트 띄워드림
            KD3DSizingBox* pkRareBox = ( KD3DSizingBox* )m_itemwndGot[i]->GetChildByName( "rare" );
			KD3DSizingBox* pkLegendBox = (KD3DSizingBox*)m_itemwndGot[i]->GetChildByName("legend");
            KD3DSizingBox* pkEpicBox = ( KD3DSizingBox* )m_itemwndGot[i]->GetChildByName( "epi" );

            if( ( pkRareBox != NULL && pkRareBox->IsRenderOn() ) ||
				(pkLegendBox != NULL && pkLegendBox->IsRenderOn()) ||
                ( pkEpicBox != NULL && pkEpicBox->IsRenderOn() ) ) {
                g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Gemble_03",(vPos.x - 400.0f)/ 400.0f,(-vPos.y + 300.0f)/ 400.0f, 0.5f ), GC_LAYER_UI); 
                g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Gemble_04",(vPos.x - 400.0f)/ 400.0f,(-vPos.y + 300.0f)/ 400.0f, 0.5f ), GC_LAYER_UI); 
                g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Gemble_05",(vPos.x - 400.0f)/ 400.0f,(-vPos.y + 300.0f)/ 400.0f, 0.5f ), GC_LAYER_UI); 
                g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Gemble_06",(vPos.x - 400.0f)/ 400.0f,(-vPos.y + 300.0f)/ 400.0f, 0.5f ), GC_LAYER_UI); 
            } 
			iSize--;
		}

		if ( iSize == 0 )
		{
			break;
		}
		
	}

}

