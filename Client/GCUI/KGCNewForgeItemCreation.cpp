#include "stdafx.h"
#include "KGCNewForgeItemCreation.h"
#include "KGCNewForgeDlg.h"
#include "GCRecipeInfoBar.h"
#include "GCItemInformationBoxPair.h"

IMPLEMENT_CLASSNAME( KGCNewForgeItemCreation );
IMPLEMENT_WND_FACTORY( KGCNewForgeItemCreation );
IMPLEMENT_WND_FACTORY_NAME( KGCNewForgeItemCreation, "gc_new_forge_item_creation_dlg" );

KGCNewForgeItemCreation::KGCNewForgeItemCreation( void )
: m_iSelectIndex(0)
, m_iScrollpos(0)
{

	for(int i =0;i<GCForgeManager::ESFB_MAX;i++)
	{
		char strTemp[ MAX_PATH ] = "";
		m_saSlotFilterBtnList[i] = NULL;
		sprintf_s( strTemp, MAX_PATH, "slot_filter_btn%d", i);
		LINK_CONTROL(strTemp,m_saSlotFilterBtnList[i]);

		m_saSlotFilterSelectedBtnList[i] = NULL;
		sprintf_s( strTemp, MAX_PATH, "slot_filter_selected%d", i);
		LINK_CONTROL(strTemp,m_saSlotFilterSelectedBtnList[i]);
	}

	for(int i =0;i<GCForgeManager::ERSB_MAX;i++)
	{
		char strTemp[ MAX_PATH ] = "";
		m_saUpSortList[i] = NULL;
		sprintf_s( strTemp, MAX_PATH, "recipe_asc_sort%d", i);
		LINK_CONTROL(strTemp,m_saUpSortList[i]);

		m_saDownSortList[i] = NULL;
		sprintf_s( strTemp, MAX_PATH, "recipe_desc_sort%d", i);
		LINK_CONTROL(strTemp,m_saDownSortList[i]);

		m_saSortBtnList[i] = NULL;
		sprintf_s( strTemp, MAX_PATH, "recipe_sort_btn%d", i);
		LINK_CONTROL(strTemp,m_saSortBtnList[i]);
	}

	m_pkScrollbar = NULL;
	LINK_CONTROL( "scroll", m_pkScrollbar );

	m_vecRecipeInfoBar.reserve( mc_iMaxRecipeViewHeightCount );

	m_strFilter = L"";
}


KGCNewForgeItemCreation::~KGCNewForgeItemCreation( void )
{
}

void KGCNewForgeItemCreation::ActionPerformed( const KActionEvent& event )
{
	GCWND_MAP( m_pkScrollbar, OnScrollPos );
}

void KGCNewForgeItemCreation::OnCreate( void )
{
	if(SiKP2P()->IsConnectedToGameServer() == true )
	{
		SiGCForgeManager()->Send_GetItemCatalog();
		SiGCForgeManager()->Send_GetItemBreakupInfo();
	}

	InitSlotBtnList();
	InitSortBtnList();
	CreateInfobarList();
	InitInfobarList();
	
	g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_FORGE_COMPLETE_RESULT_ITEM, this, &KGCNewForgeItemCreation::SetInfobarList );

	if( m_pkScrollbar )
	{
		m_pkScrollbar->InitState( true, true, this );
		m_pkScrollbar->SetScrollPos(0);
		m_pkScrollbar->SetScrollRangeMin(0);
		m_pkScrollbar->SetScrollRangeMax(1);
		m_pkScrollbar->SetScrollPageSize(1);
	}

	InitItemCreation();
	g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( NULL );
}

void KGCNewForgeItemCreation::InitItemCreation( void )
{
	SelectSlotBtn(GCForgeManager::ESFB_ALL);
	m_strFilter = L"";
	SiGCForgeManager()->SetShowRecipeList();
	SetInfobarList();
}

void KGCNewForgeItemCreation::InitSlotBtnList( void )
{
	for(int i =0;i<GCForgeManager::ESFB_MAX;i++)
	{
		m_saSlotFilterBtnList[i]->InitState(false,true,this);
		m_saSlotFilterBtnList[i]->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCNewForgeItemCreation::OnSelectSlotBtnList);
		m_saSlotFilterSelectedBtnList[i]->InitState(false,false,NULL);
	}
}

void KGCNewForgeItemCreation::InitSortBtnList( void )
{
	for(int i =0;i<GCForgeManager::ERSB_MAX;i++)
	{
		m_saUpSortList[i]->InitState(false,false,NULL);
		m_saDownSortList[i]->InitState(false,false,NULL);
		m_saSortBtnList[i]->InitState(true,true,this);
		m_saSortBtnList[i]->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCNewForgeItemCreation::OnSelectSortBtnList);
	}

	m_saUpSortList[GCForgeManager::ERSB_ITEMLEVEL]->ToggleRender(true);
	SiGCForgeManager()->InitSort();
}

void KGCNewForgeItemCreation::CreateInfobarList( void )
{
	if(m_iMaxRecipeViewCount == m_vecRecipeInfoBar.size() && !m_vecRecipeInfoBar.empty())
	{
		return;
	}

	int iMaxRecipeViewHeightCount = static_cast<int>( mc_iDefaultRecipeViewHeightCount * m_fWindowScaleY );

	if( iMaxRecipeViewHeightCount > mc_iMaxRecipeViewHeightCount)
	{
		iMaxRecipeViewHeightCount = mc_iMaxRecipeViewHeightCount;
	}

	DWORD dwHeight = static_cast<DWORD>( mc_iMaxRecipeViewRealHeight / iMaxRecipeViewHeightCount );
	float fMaxHeightInterval = static_cast<float>( mc_iMaxRecipeViewRealHeight - ( dwHeight * iMaxRecipeViewHeightCount ) + mc_iMaxRecipeViewIntervalHeight );

	int iHeightIntervalCount = iMaxRecipeViewHeightCount - 1;
	float fHeightInterval = 0.0f;

	if( iHeightIntervalCount > 0 )
	{
		fHeightInterval = fMaxHeightInterval / iHeightIntervalCount;
	}

	float fOffsetY = ( fMaxHeightInterval - ( fHeightInterval * iHeightIntervalCount) ) / 2.0f;

	D3DXVECTOR2 vStartPos( 27.0f * GC_SCREEN_DIV_WIDTH, 113.f * GC_SCREEN_DIV_WIDTH + fOffsetY );

	int iRecipeInfoBarCount = static_cast<int>( m_vecRecipeInfoBar.size() );
	m_iMaxRecipeViewCount = iMaxRecipeViewHeightCount;

	for(int i = 0 ; i < m_iMaxRecipeViewCount; ++i )
	{
		if( i >= iRecipeInfoBarCount )
		{
			char szMakeWndName[MAX_PATH] ="";
			sprintf_s( szMakeWndName, "recipe_info_bar%d", i );
			KGCRecipeInfoBar* pkRecipeBar = static_cast<KGCRecipeInfoBar*>( g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_new_forge.stg", szMakeWndName, "new_forge_dlg\\new_forge_item_creation_dlg\\creation_recipe_bar" ) );
			m_vecRecipeInfoBar.push_back( pkRecipeBar );
		}

		float fHeightSize = static_cast<float>( dwHeight ) + fHeightInterval;
		D3DXVECTOR2 vNewPos( vStartPos.x, vStartPos.y + i * fHeightSize );

		m_vecRecipeInfoBar[i]->InitState( false, true, this );
		m_vecRecipeInfoBar[i]->SetWindowPosDirect( vNewPos );
		m_vecRecipeInfoBar[i]->SetHeightDirect( dwHeight );
	}
}

void KGCNewForgeItemCreation::InitInfobarList( void )
{
	for(std::vector< KGCRecipeInfoBar* >::iterator vIt = m_vecRecipeInfoBar.begin();vIt !=m_vecRecipeInfoBar.end();vIt++)
	{
		(*vIt)->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCNewForgeItemCreation::OnSelectInfobar);
	}
}

std::wstring KGCNewForgeItemCreation::GetGradeString( char cGradeID )
{
	switch( cGradeID )
	{
		case KItem::GRADE_NORMAL:
			{
				return g_pkStrLoader->GetString( STR_ID_ITEM_GRADE_NORMAL + 4 );
			}
		case KItem::GRADE_RARE:
			{
				return g_pkStrLoader->GetString( STR_ID_ITEM_GRADE_MAGIC + 3 );
			}
		case KItem::GRADE_EPIC:
			{
				return g_pkStrLoader->GetString( STR_ID_ITEM_GRADE_EPIC + 2 );
			}
		case KItem::GRADE_LEGEND:
			{
				return g_pkStrLoader->GetString( STR_ID_ITEM_GRADE_LEGEND + 1);
			}
		case KItem::GRADE_MYSTIC:
			{
				return g_pkStrLoader->GetString(STR_ID_ITEM_GRADE_MYSTIC);
			}
	}

	return std::wstring();
}

void KGCNewForgeItemCreation::SetInfobarString(GCItem* pItem, KGCRecipeInfoBar* pBar,const RecipeSimpleInfo& csInfo )
{
	if(pItem == NULL || pBar == NULL) return;

	WCHAR strTemp[ MAX_PATH ] = L"";
	_itow_s(csInfo.iItemLevel, strTemp, MAX_PATH, 10 );
	std::wstring strItemLevel = strTemp;
	std::wstring strItemName = pItem->strItemName;
	_itow_s( csInfo.iPrice, strTemp, MAX_PATH, 10 ); 
	std::wstring strPrice = strTemp;
	std::wstring strGrade = GetGradeString( csInfo.cGrade );

	pBar->SetTextRecipeInfoBar( strItemLevel, strItemName, strPrice, strGrade );
	pBar->SetRecipe(csInfo);
}

void KGCNewForgeItemCreation::SetInfobarStringColor(GCItem* pItem, KGCRecipeInfoBar* pBar,const RecipeSimpleInfo& csInfo )
{
	if( NULL == pItem || NULL == pBar ) { 
		return;
	}

	DWORD dwFontColor = 0xff6f6f6f;
	if( csInfo.iPrice <= g_kGlobalValue.m_kUserInfo.GetCurrentChar().GetCurrentGP() ) 
	{ 
		//Forge Discount
		int totalDiscount = 0;

		g_kGlobalValue.currentTotalForgeDiscount = 0;

		if ( csInfo.bHasDiscount )
		{
			if ( g_pItemMgr->FindInventoryForItemID( ITEM_BLACKSMITH_DISCOUNT ) )
				g_kGlobalValue.currentTotalForgeDiscount = 0.15;
			if ( g_pItemMgr->FindInventoryForItemID( ITEM_VIP_TEMPORARY_ITEM ) )
				g_kGlobalValue.currentTotalForgeDiscount = 0.30;
		}

		GCForgeManager::MAP_FM_MATERIAL_LIST::const_iterator mit = SiGCForgeManager()->GetMeterialList().find(PAIR_USHORT_DWORD(csInfo.usIndex, csInfo.ItemID));

		std::map< GCITEMID, int > mapEquipMatNeed; 
		std::map< GCITEMID, int > mapEquipMatHave; 
		std::map< GCITEMID, int > mapCountMatNeed; 
		std::map< GCITEMID, int > mapCountMatHave; 

		if( mit != SiGCForgeManager()->GetMeterialList().end() ) 
		{ 
			std::vector<KDropItemInfo>::const_iterator vit = mit->second.begin();
			for( ; vit != mit->second.end(); ++vit )
			{
				if( (*vit).m_nDuration < 0 )
				{
					std::vector< KItem* > vecItemList;
					if( g_pItemMgr->GetInventoryItemListFromItemID( (*vit).m_ItemID, vecItemList ) ) 
					{
						for( std::vector< KItem* >::iterator matVit = vecItemList.begin() ; matVit != vecItemList.end() ; ) 
						{
							if( (*matVit)->m_ItemID == KGCItemManager::ITEM_ENCHANT_STONE )  // 강화석입니다. 
							{ 
								if( (*matVit)->m_cGradeID == 0 && (*matVit)->m_cEnchantLevel == 0 && (*matVit)->m_EnchantEquipItemUID == 0 ) 
								{
									++matVit;
								}
								else
								{ 
									matVit = vecItemList.erase( matVit );
								}
							}
							else 
							{ 
								++matVit;
							}
						}

						mapEquipMatHave.insert( std::make_pair( (*vit).m_ItemID, static_cast<int>( vecItemList.size() ) ) );    
					}
 
					std::map< GCITEMID, int >::iterator matIter = mapEquipMatNeed.find( (*vit).m_ItemID );
					if( mapEquipMatNeed.end() == matIter ) 
					{ 
						mapEquipMatNeed.insert( std::make_pair( (*vit).m_ItemID, 1 ) );
					}
					else 
					{ 
						matIter->second++;
					}
				}
				else
				{
					mapCountMatHave.insert( std::make_pair( (*vit).m_ItemID, g_pItemMgr->GetInventoryItemDuration( (*vit).m_ItemID ) ) );   // 보유 갯수 저장 

					if ( g_kGlobalValue.currentTotalForgeDiscount > 0 && (*vit).m_nDuration > 0 )
						totalDiscount = (int)ceil( (*vit).m_nDuration - ( (*vit).m_nDuration * g_kGlobalValue.currentTotalForgeDiscount ) );
					else
						totalDiscount = (*vit).m_nDuration;

					mapCountMatNeed.insert( std::make_pair( (*vit).m_ItemID, totalDiscount) );                                        // 필요갯수 거장
				}
			}
		}

		if( false == mapEquipMatNeed.empty() && false == mapCountMatNeed.empty() ) 
		{
			bool bEquipMatEnable = false;
			std::map< GCITEMID, int >::iterator equipMatIter = mapEquipMatNeed.begin();

			for( ; equipMatIter != mapEquipMatNeed.end() ; ++equipMatIter ) 
			{ 
				std::map< GCITEMID, int >::iterator matIter = mapEquipMatHave.find( equipMatIter->first );
				if( matIter == mapEquipMatHave.end() ) 
				{
					bEquipMatEnable = false;
					break;
				}
				else if ( equipMatIter->second > matIter->second ) 
				{
					bEquipMatEnable = false;
					break;
				}

				bEquipMatEnable = true;
			}
 
			bool bCountMatEnable = false;
			std::map< GCITEMID, int >::iterator countMatIter = mapCountMatNeed.begin();

			for( ; countMatIter != mapCountMatNeed.end() ; ++countMatIter ) 
			{ 
				std::map< GCITEMID, int >::iterator matIter = mapCountMatHave.find( countMatIter->first );
				if( matIter == mapCountMatHave.end() ) 
				{
					bCountMatEnable = false;
					break;
				}
				else 
				{
					if ( countMatIter->second > matIter->second )
					{
						bCountMatEnable = false;
						break;
					}
				}

				bCountMatEnable = true;
			}

			if( bEquipMatEnable && bCountMatEnable ) 
			{ 
				dwFontColor = g_pItemMgr->GetItemGradeColorRGB(csInfo.cGrade);
				//dwFontColor = 0xffffffff;
			}
			else 
			{ 
				dwFontColor = 0xff6f6f6f;
			}
		}
		else if( false == mapEquipMatNeed.empty() && mapCountMatNeed.empty() ) 
		{
			bool bEquipMatEnable = false;
			std::map< GCITEMID, int >::iterator equipMatIter = mapEquipMatNeed.begin();

			for( ; equipMatIter != mapEquipMatNeed.end() ; ++equipMatIter ) 
			{ 
				std::map< GCITEMID, int >::iterator matIter = mapEquipMatHave.find( equipMatIter->first );
				if( matIter == mapEquipMatHave.end() ) 
				{
					bEquipMatEnable = false;
					break;
				}
				else if ( equipMatIter->second > matIter->second ) 
				{
					bEquipMatEnable = false;
					break;
				}
				bEquipMatEnable = true;
			}

			if( bEquipMatEnable ) 
			{ 
				dwFontColor = g_pItemMgr->GetItemGradeColorRGB(csInfo.cGrade);
				//dwFontColor = 0xffffffff;
			}
			else 
			{ 
				dwFontColor = 0xff6f6f6f;
			}
		}
		else if( mapEquipMatNeed.empty() && false == mapCountMatNeed.empty() ) 
		{
			bool bCountMatEnable = false;
			std::map< GCITEMID, int >::iterator countMatIter = mapCountMatNeed.begin();

			for( ; countMatIter != mapCountMatNeed.end() ; ++countMatIter ) 
			{ 
				std::map< GCITEMID, int >::iterator matIter = mapCountMatHave.find( countMatIter->first );
				if( matIter == mapCountMatHave.end() ) 
				{
					bCountMatEnable = false;
					break;
				}
				else if( countMatIter->second > matIter->second )  
				{
					bCountMatEnable = false;
					break;
				}

				bCountMatEnable = true;
			}

			if( bCountMatEnable ) 
			{ 
				dwFontColor = g_pItemMgr->GetItemGradeColorRGB(csInfo.cGrade);
				//dwFontColor = 0xffffffff;
			}
			else 
			{ 
				dwFontColor = 0xff6f6f6f;
			}
		}
	}

	pBar->SetFontColorRecipeInfoBar( dwFontColor );
}

void KGCNewForgeItemCreation::FilterInfoBar(const std::wstring& strFilter)
{
	m_strFilter = strFilter;
	SetInfobarList();
	OnInitInfobar();
}

void KGCNewForgeItemCreation::SetInfobarList( )
{
	const auto vecRecipeList = SiGCForgeManager()->GetShowRecipeList();
	std::vector<RecipeSimpleInfo> vecInfoList;

	if ( m_strFilter.empty() == false && vecRecipeList.size() > 0 )
	{
		for (auto recipe : vecRecipeList)
		{
			GCItem* pItem = g_pItemMgr->GetItemData(recipe.ItemID);

			if (pItem)
			{
				wstring strLowerItemName = pItem->strItemName;
				GCUTIL_STR::ToLower(strLowerItemName);
				GCUTIL_STR::ToLower(m_strFilter);

				if (strLowerItemName.find(m_strFilter) != std::wstring::npos)
				{
					vecInfoList.push_back(recipe);
				}
			}
		}
	}
	else
	{
		vecInfoList = vecRecipeList;
	}

	if(vecInfoList.empty())
	{
		m_iSelectIndex = -1;
	}
	else
	{
		m_iSelectIndex = 0;
	}

	int iScrollMaxSize = (vecInfoList.size() - (m_iMaxRecipeViewCount)) + 1;// / mc_iMaxRecipeViewHeightCount;

	if(iScrollMaxSize < 0 || vecInfoList.empty())
	{
		iScrollMaxSize = 1;
	}

	if(m_iScrollpos > iScrollMaxSize)
	{
		m_iScrollpos = iScrollMaxSize;
	}

	m_pkScrollbar->SetScrollRangeMax( iScrollMaxSize );

	std::vector<RecipeSimpleInfo> vecPage;

	if( (int)vecInfoList.size() < m_iMaxRecipeViewCount )
	{
		vecPage = vecInfoList;
	}
	else
	{
		vecPage.reserve(m_iMaxRecipeViewCount);
		std::vector<RecipeSimpleInfo>::const_iterator begin = vecInfoList.begin();
		std::vector<RecipeSimpleInfo>::const_iterator end = vecInfoList.end();
		if( m_iScrollpos == 0 )
		{
			std::copy( begin, begin + m_iMaxRecipeViewCount, std::back_inserter(vecPage));
		}
		else
		{
			if( m_iScrollpos + m_iMaxRecipeViewCount >= (int)vecInfoList.size() )
			{
				std::copy( begin + m_iScrollpos, end, std::back_inserter(vecPage));
			}
			else
			{
				std::copy( begin + m_iScrollpos, begin + m_iScrollpos + m_iMaxRecipeViewCount, std::back_inserter(vecPage));
			}
		}
	}

	std::vector<KGCRecipeInfoBar*>::iterator vShowListIter = m_vecRecipeInfoBar.begin();

	for(; vShowListIter != m_vecRecipeInfoBar.end(); ++vShowListIter)
	{
		(*vShowListIter)->ToggleRender(false);
	}

	int iSize = vecPage.size();
	std::vector<RecipeSimpleInfo>::iterator vit = vecPage.begin();

	for( int i = 0; i < iSize; i++ )
	{
		if( vit == vecPage.end() )
		{
			m_vecRecipeInfoBar[i]->ToggleRender( false );
			continue;
		}

		GCItem* pItem = g_pItemMgr->GetItemData( vecPage[i].ItemID );
		SetInfobarString(pItem, m_vecRecipeInfoBar[i],vecPage[i]);
		SetInfobarStringColor(pItem, m_vecRecipeInfoBar[i],vecPage[i]);

		m_vecRecipeInfoBar[i]->ToggleRender( true );
		++vit;
	}
}

void KGCNewForgeItemCreation::OnSelectSortBtnList( const KActionEvent& event )
{
	for(int i =0;i<GCForgeManager::ERSB_MAX;i++)
	{
		if(m_saSortBtnList[i] == event.m_pWnd)
		{
			SelectSortBtn(i);
			break;
		}
	}
}

void KGCNewForgeItemCreation::OnInitInfobar( void )
{
	for(std::vector< KGCRecipeInfoBar* >::iterator vit = m_vecRecipeInfoBar.begin(); vit != m_vecRecipeInfoBar.end(); vit++)
	{
		(*vit)->LockAll(false);
	}
}

void KGCNewForgeItemCreation::OnSelectInfobar( const KActionEvent& event )
{
	if( g_pkUIScene->m_pkForgeDlg->GetForgeCraftState() == KGCNewForgeItemCraftMachine::EFMS_START || g_pkUIScene->m_pkForgeDlg->GetForgeCraftState() == KGCNewForgeItemCraftMachine::EFMS_CONTINUE )
	{
		return;
	}

	for(std::vector< KGCRecipeInfoBar* >::iterator vit = m_vecRecipeInfoBar.begin(); vit != m_vecRecipeInfoBar.end(); vit++)
	{
		if( (*vit) == event.m_pWnd)
		{
			SelectInfoBar((*vit)->GetIndex(), (*vit)->GetItemID());
			(*vit)->LockAll(true);
		}
		else
		{
			(*vit)->LockAll(false);
		}
	}
}

void KGCNewForgeItemCreation::SelectInfoBar( unsigned short iIndex ,GCITEMID giItemID )
{
	SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_FORGE_CLICK_CREATION_ITEM_BAR,iIndex,giItemID));
}

void KGCNewForgeItemCreation::SelectSortBtn( int iIndex )
{
	for(int i =0;i<GCForgeManager::ERSB_MAX;i++)
	{
		if(m_saDownSortList[i]->IsRenderOn() && iIndex == i)
		{
			m_saUpSortList[i]->ToggleRender(true );
			m_saDownSortList[i]->ToggleRender(false );
			SiGCForgeManager()->SetSort(i, true);
			SetInfobarList();
			OnInitInfobar();
		}
		else if(iIndex == i)
		{
			m_saUpSortList[i]->ToggleRender(false );
			m_saDownSortList[i]->ToggleRender(true );
			SiGCForgeManager()->SetSort(i, false);
			SetInfobarList();
			OnInitInfobar();
		}
		else
		{
			m_saUpSortList[i]->ToggleRender(false );
			m_saDownSortList[i]->ToggleRender(false );
		}
	}
}

void KGCNewForgeItemCreation::OnSelectSlotBtnList( const KActionEvent& event )
{
	for(int i =0;i<GCForgeManager::ESFB_MAX;i++)
	{
		if(m_saSlotFilterBtnList[i] == event.m_pWnd)
		{
			SelectSlotBtn(i);
			break;
		}
	}
}

void KGCNewForgeItemCreation::SelectSlotBtn( int iIndex )
{
	for(int i =0;i<GCForgeManager::ESFB_MAX;i++)
	{
		m_saSlotFilterBtnList[i]->ToggleRender(iIndex != i);
		m_saSlotFilterSelectedBtnList[i]->ToggleRender(iIndex == i);
	}


	m_iScrollpos = 0;
	SiGCForgeManager()->SetSlot(iIndex);	
	SiGCForgeManager()->SetShowRecipeList();
	SetInfobarList();
	OnInitInfobar();
}

void KGCNewForgeItemCreation::OnScrollPos( )
{
	int iPos = m_pkScrollbar->GetScrollPos();

	if( m_iScrollpos != iPos )
	{
		m_iScrollpos = iPos;
		g_KDSound.Play( "73" );

		SetInfobarList();
		OnInitInfobar();
	}
}

void KGCNewForgeItemCreation::FrameMoveInEnabledState( void )
{
// 	POINT ptMouse = g_pkInput->GetMousePos();
// 	D3DXVECTOR2 vMousePos( (float)ptMouse.x, (float)ptMouse.y );
// 
// 	bool bCheck = false;
// 	//int iStartIndex = (m_iScrollpos + m_iMaxRecipeViewCount);
// 
// 	for( int i = 0;i < m_iMaxRecipeViewCount; ++i )
// 	{
// 		if( m_vecRecipeInfoBar[i]->IsRenderOn() && m_vecRecipeInfoBar[i]->CheckPosInWindowBound( vMousePos ) )
// 		{
// 			int iRealIndex = m_iScrollpos + i;
// 
// 			GCItem* pItem = g_pItemMgr->GetItemData( m_vecRecipeInfoBar[i]->GetItemID() );
// 			if( NULL == pItem )
// 			{
// 				continue;
// 			}
// 
// 			const std::vector<RecipeSimpleInfo>& vecShowRecipe = SiGCForgeManager()->GetShowRecipeList();
// 			
// 			if( iRealIndex > (int)vecShowRecipe.size())
// 			{
// 				continue;
// 			}
// 
// 			g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem, -1, NULL, NULL, NULL, vecShowRecipe[ iRealIndex ].cGrade, vecShowRecipe[ iRealIndex ].iItemLevel );
// 			g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
// 
// 			D3DXVECTOR2 vecPos( m_vecRecipeInfoBar[i]->GetCurrentWindowPos() );
// 			vecPos.x += m_vecRecipeInfoBar[i]->GetWidth();
// 			vecPos.x *= m_fWindowScaleX;
// 			vecPos.y *= m_fWindowScaleY;
// 
// 			g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );            
// 			g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(KD3DWnd::D3DWZ_TOPMOST);
// 
// 			bCheck = true;
// 			break;
// 		}
// 	}
// 
// 	g_pkUIScene->m_pkItemInformationBox->ToggleRender( bCheck );
}

bool KGCNewForgeItemCreation::IsEnabledRecipeBar( GCITEMID itemID_ )
{
	bool bEnable = false;
	for( std::vector< KGCRecipeInfoBar* >::iterator it = m_vecRecipeInfoBar.begin() ; it != m_vecRecipeInfoBar.end() ; ++it ) 
	{ 
		if( (*it)->GetItemID() == itemID_ && (*it)->IsEnable() ) 
		{
			bEnable = true;
			break;
		}
	}

	return bEnable;
}