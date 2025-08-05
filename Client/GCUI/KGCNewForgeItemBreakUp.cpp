#include "stdafx.h"
#include "KGCNewForgeItemBreakUp.h"

IMPLEMENT_CLASSNAME( KGCNewForgeItemBreakUp );
IMPLEMENT_WND_FACTORY( KGCNewForgeItemBreakUp );
IMPLEMENT_WND_FACTORY_NAME( KGCNewForgeItemBreakUp, "gc_new_forge_item_breakup_dlg" );

KGCNewForgeItemBreakUp::KGCNewForgeItemBreakUp( void )
: m_pkScrollbar( NULL )
, m_iScrollPos(0)
{
	LINK_CONTROL("scroll", m_pkScrollbar);
}

KGCNewForgeItemBreakUp::~KGCNewForgeItemBreakUp( void )
{
}

void KGCNewForgeItemBreakUp::ActionPerformed( const KActionEvent& event )
{
	GCWND_MAP( m_pkScrollbar, OnScrollPos );
}

void KGCNewForgeItemBreakUp::OnCreate( void )
{
	OnCreateItemImgList();
	OnInitItemImgList();

	if( m_pkScrollbar )
	{
		m_pkScrollbar->InitState( true, true, this );
		m_pkScrollbar->InitScroll(0, 1, 0, 1, 0, 0, 1, true);
	}

	g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_FORGE_SEL_CHARACTER, this, &KGCNewForgeItemBreakUp::OnInitialize );

	g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( NULL );
}

void KGCNewForgeItemBreakUp::OnInitialize()
{
	m_iScrollPos = 0;
	m_strFilter = L"";
	SiGCForgeManager()->MakeAllBreakUpItemList();
	RefreshItemList();
	SetItemList();
}

void KGCNewForgeItemBreakUp::OnScrollPos()
{
	int iPos = m_pkScrollbar->GetScrollPos();

	if( m_iScrollPos != iPos )
	{
		m_iScrollPos = iPos;
		g_KDSound.Play( "73" );

		RefreshItemList();
		SetItemList();
	}
}

void KGCNewForgeItemBreakUp::FilterInventoryList(const std::wstring& strFilter)
{
	m_strFilter = strFilter;
	SetItemList();
}

void KGCNewForgeItemBreakUp::SetItemList()
{
	m_vecBreakableUIDItemList.clear();

	const auto vecBreakableUIDItemList = SiGCForgeManager()->GetShowBreakUpItemList();


	if (m_strFilter.empty() == false && vecBreakableUIDItemList.size() > 0)
	{
		for (auto breakableItem : vecBreakableUIDItemList)
		{
			KItem* kItem = g_pItemMgr->GetInventoryItem(breakableItem);
			GCItem* pItem = g_pItemMgr->GetItemData(kItem->m_ItemID);
			
			if (pItem)
			{
				wstring strLowerItemName = pItem->strItemName;
				GCUTIL_STR::ToLower(strLowerItemName);
				GCUTIL_STR::ToLower(m_strFilter);

				if (strLowerItemName.find(m_strFilter) != std::wstring::npos)
				{
					m_vecBreakableUIDItemList.push_back(breakableItem);
				}
			}
		}
	}
	else
	{
		m_vecBreakableUIDItemList = vecBreakableUIDItemList;
	}

	int iScrollMaxSize = 1 + ( ( m_vecBreakableUIDItemList.size() ) / m_iMAX_ITEM_VIEW_WIDTH_COUNT ) - (m_iMAX_ITEM_VIEW_HEGHIT_COUNT - 1); 

	if( iScrollMaxSize < 0 || m_vecBreakableUIDItemList.empty() )
	{
		iScrollMaxSize = 1;
	}

	if(m_iScrollPos > iScrollMaxSize)
	{
		m_iScrollPos = iScrollMaxSize;
	}

	m_pkScrollbar->SetScrollRangeMax(iScrollMaxSize);

	std::vector<GCITEMUID> vecRealPage;

	if( m_vecBreakableUIDItemList.size() < m_iMAX_ITEM_VIEW_WIDTH_COUNT * m_iMAX_ITEM_VIEW_HEGHIT_COUNT )
	{
		vecRealPage = m_vecBreakableUIDItemList;
	}
	else
	{
		vecRealPage.reserve( m_iMAX_ITEM_VIEW_WIDTH_COUNT * m_iMAX_ITEM_VIEW_HEGHIT_COUNT );
		std::vector<GCITEMUID>::const_iterator begin = m_vecBreakableUIDItemList.begin();
		std::vector<GCITEMUID>::const_iterator end = m_vecBreakableUIDItemList.end();
		if(m_iScrollPos == 0)
		{
			std::copy( begin, begin + (m_iMAX_ITEM_VIEW_WIDTH_COUNT * m_iMAX_ITEM_VIEW_HEGHIT_COUNT), std::back_inserter(vecRealPage));
		}
		else
		{
			std::copy(begin + (m_iScrollPos * m_iMAX_ITEM_VIEW_WIDTH_COUNT), end, std::back_inserter(vecRealPage));
		}
	}

	std::vector<GCITEMUID>::iterator vitTemp = vecRealPage.begin();
	int iShowSize = m_vecBreakableItemList.size();

	for( int i = 0; i < iShowSize; i++ )
	{
		if( vitTemp == vecRealPage.end() )
		{
			m_vecBreakableItemList[i].first = 0;
			m_vecBreakableItemList[i].second->ResetItem();
			m_vecBreakableItemList[i].second->ToggleRender( false );
			continue;
		}

		KItem* pItem = g_pItemMgr->GetInventoryItem(*vitTemp);

		if( pItem != NULL )
		{
			m_vecBreakableItemList[i].first = *vitTemp;
			m_vecBreakableItemList[i].second->SetItem(pItem);
			m_vecBreakableItemList[i].second->ToggleRender( true );
		}

		++vitTemp;
	}
}

void KGCNewForgeItemBreakUp::RefreshItemList( void )
{
	if(SiGCForgeManager()->IsEnableBreakUp() == false )
	{
		return;
	}

	SiGCForgeManager()->SetShowBreakUpItemList();
}

void KGCNewForgeItemBreakUp::OnCreateItemImgList( void )
{
	if(m_vecBreakableItemList.empty() == false) 
	{
		return;
	}

	std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_new_forge.stg", "","new_forge_dlg\\new_forge_item_breakup_dlg\\box_item",true,m_iMAX_ITEM_VIEW_HEGHIT_COUNT * m_iMAX_ITEM_VIEW_WIDTH_COUNT );

	for(std::vector<KD3DWnd*>::iterator vIt = vecTemp.begin(); vIt != vecTemp.end();vIt++)
	{
		m_vecBreakableItemList.push_back(std::pair<GCITEMID,KGCCommonItemWnd*>(0,static_cast<KGCCommonItemWnd*>(*vIt)));
	}
}

void KGCNewForgeItemBreakUp::OnInitItemImgList( void )
{
	for(int i = 0 ; i < m_iMAX_ITEM_VIEW_HEGHIT_COUNT ; i++)
	{
		for(int j = 0; j < m_iMAX_ITEM_VIEW_WIDTH_COUNT ; j++)
		{
			int iIndex = i * m_iMAX_ITEM_VIEW_WIDTH_COUNT + j;
			float x = static_cast<float>(j * 50 * GC_SCREEN_DIV_WIDTH + 23 * GC_SCREEN_DIV_WIDTH);
			float y = static_cast<float>(i * 50 * GC_SCREEN_DIV_WIDTH + 66 * GC_SCREEN_DIV_WIDTH);
			D3DXVECTOR2 vNewPos(x,y);
			m_vecBreakableItemList[iIndex].second->InitState(false, true,this);
			m_vecBreakableItemList[iIndex].second->SetWindowPos( vNewPos );
			m_vecBreakableItemList[iIndex].second->SetWidthDirect( GC_SCREEN_DIV_SIZE_INT(49) );
			m_vecBreakableItemList[iIndex].second->SetHeightDirect( GC_SCREEN_DIV_SIZE_INT(49) );
			m_vecBreakableItemList[iIndex].second->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCNewForgeItemBreakUp::OnClickItem);
		}
	}
}

void KGCNewForgeItemBreakUp::OnClickItem( const KActionEvent& event )
{
	for( std::vector<std::pair<GCITEMUID,KGCCommonItemWnd*>>::iterator vIt = m_vecBreakableItemList.begin(); vIt != m_vecBreakableItemList.end(); vIt++ )
	{
		if( vIt->second == event.m_pWnd )
		{
			GCITEMUID giuItemUID = vIt->first;
			vIt->first = 0;
			ClickItem( giuItemUID, vIt->second );
			break;
		}
	}
}

void KGCNewForgeItemBreakUp::ClickItem( GCITEMUID giItem, KGCCommonItemWnd* pWnd )
{
	if (SiGCForgeManager()->AddWaitItem(giItem, pWnd->GetItemID()))
	{
		RefreshItemList();
		SetItemList();
		SpeakToActionListener(KActionEvent(NULL, KGCUIScene::D3DWE_FORGE_CLICK_BREAKEUP_ITEM, giItem, pWnd->GetItemID()));
	}
}

void KGCNewForgeItemBreakUp::FrameMoveInEnabledState( void )
{
	bool bCheck = false;
	for(std::vector<std::pair<GCITEMUID,KGCCommonItemWnd*>>::iterator vIt = m_vecBreakableItemList.begin(); vIt != m_vecBreakableItemList.end(); vIt++)
	{
		if(vIt->second->CheckMousePosInWindowBound())
		{
			D3DXVECTOR2 vec = vIt->second->GetCurrentWindowLocalPos();

			KItem* pInventory = NULL;
			GCItem* pItem = NULL;

			if( false == g_pItemMgr->FindInventory(vIt->first, &pInventory, &pItem) || NULL == pInventory || NULL == pItem || vIt->first == 0 )
			{
				continue;
			}

			g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem , -1, pInventory);

			const std::vector<RecipeSimpleInfo>& vecShowRecipe = SiGCForgeManager()->GetShowRecipeList();

			vec.x += vIt->second->GetWidth();
			vec.x *= m_fWindowScaleX;
			vec.y *= m_fWindowScaleY;

			g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vec );
			g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(KD3DWnd::D3DWZ_TOPMOST);
			bCheck = true;
			break;
		}
	}

	g_pkUIScene->m_pkItemInformationBox->ToggleRender( bCheck );

}