#include "stdafx.h"
#include "KGCNewForgeDlg.h"
#include "KGCNewForgeItemCraft.h"

IMPLEMENT_CLASSNAME( KGCNewForgeItemCraftMachine );
IMPLEMENT_WND_FACTORY( KGCNewForgeItemCraftMachine );
IMPLEMENT_WND_FACTORY_NAME( KGCNewForgeItemCraftMachine, "gc_new_forge_item_crafting_dlg" );

KGCNewForgeItemCraftMachine::KGCNewForgeItemCraftMachine( void )
: m_iForgeState(EKGCNEWFORGE_STATE::EKGCFOGE_CREATE)
, m_pkGP(NULL)
, m_pkCostTitle(NULL)
, m_pkCost(NULL)
, m_pPrekResultItem(NULL)
, m_pkStartBtn(NULL)
, m_pkStopBtn(NULL)
, m_pkStaticPreResultItemNum(NULL)
, m_pkCountMinBtn(NULL)
, m_pkCountMaxBtn(NULL)
, m_pkCountPreBtn(NULL)
, m_pkCountNextBtn(NULL)
, m_pkCountEditBG(NULL)
, m_pkCountEdit(NULL)
, m_iCreateCount(1)
, m_iMachineState(EFMS_NORMAL)
, m_iMachineDelayCnt(0)
, m_bBypassForgeVerify(false)
{
	//Forge money information
	LINK_CONTROL("static_gp_title", m_pkGPTitle);
	LINK_CONTROL("static_gp",m_pkGP);
	LINK_CONTROL("static_cost_title",m_pkCostTitle);
	LINK_CONTROL("static_cost",m_pkCost);

	//Forge operation
	LINK_CONTROL("start_btn",m_pkStartBtn);
	LINK_CONTROL("stop_btn",m_pkStopBtn);
	
	//Prize information
	for( int i = 0; i < (int)m_apkItemGradeBack.size(); i++ )
	{
		m_apkItemGradeBack[i] = NULL;
		LINK_CONTROL_STM( "create_item_back" << i, m_apkItemGradeBack[i] );
	}

	LINK_CONTROL( "create_item_img", m_pPrekResultItem );
	LINK_CONTROL("static_create_count", m_pkStaticPreResultItemNum);
	LINK_CONTROL("item_count_min",m_pkCountMinBtn);
	LINK_CONTROL("item_count_max",m_pkCountMaxBtn);
	LINK_CONTROL("item_count_del",m_pkCountPreBtn);
	LINK_CONTROL("item_count_add",m_pkCountNextBtn);
	LINK_CONTROL("count_btn_back", m_pkCountItemBack);
	LINK_CONTROL("create_count_back", m_pkCountEditBG);
	LINK_CONTROL("count_edit", m_pkCountEdit);

	//Forge State
	for( int i = 0; i < EMCS_MAX; i++ )
	{
		m_apkForgeState[i] = NULL;
		LINK_CONTROL_STM( "state" << i, m_apkForgeState[i] );
	}
}

KGCNewForgeItemCraftMachine::~KGCNewForgeItemCraftMachine( void )
{
}

void KGCNewForgeItemCraftMachine::ActionPerformed( const KActionEvent& event )
{
	switch ( event.m_dwCode )
	{
		case KD3DWnd::D3DWE_CURSOR_ENTER:
			if( m_pkCountEdit )
				if ( event.m_pWnd == m_pkCountEdit )
					m_pkCountEdit->SetSelection( 0, m_pkCountEdit->GetTextSize() );
		break;
	}
}

void KGCNewForgeItemCraftMachine::OnCreate( void )
{
	OnCreateItemList();
	InitItemList();

	m_pkCostTitle->SetTextFromID( STR_ID_ITEM_CREATION0 );
	m_pkCostTitle->SetAlign( DT_RIGHT );

	m_pkGPTitle->SetTextFromID( STR_ID_GP_PLUS_RATE );
	m_pkGPTitle->SetAlign( DT_RIGHT );

	SetUserGPText();
	m_pkGP->SetAlign( DT_RIGHT );

	m_pkCost->SetAlign( DT_RIGHT );

	m_pkStartBtn->InitState(true,true,this);
	m_pkStopBtn->InitState(true,true,this);

	m_pkCountMinBtn->InitState(false,true,this);
	m_pkCountMaxBtn->InitState(false,true,this);
	m_pkCountPreBtn->InitState(false,true,this);
	m_pkCountNextBtn->InitState(false,true,this);
	m_pkCountItemBack->InitState( false );
	m_pkCountEditBG->InitState( false );
	m_pkCountEdit->InitState( false, true, this );
	m_pkCountEdit->SetFontColor( D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
	m_pkCountEdit->SetEditType( KD3DEdit::EDIT_NUMBER );
	m_pkCountEdit->SetText( L"1" );

	m_pkCountMinBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCNewForgeItemCraftMachine::OnClickCountBtn);
	m_pkCountMaxBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCNewForgeItemCraftMachine::OnClickCountBtn);
	m_pkCountPreBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCNewForgeItemCraftMachine::OnClickCountBtn);
	m_pkCountNextBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCNewForgeItemCraftMachine::OnClickCountBtn);

	m_pkStartBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCNewForgeItemCraftMachine::OnClickStartBtn );
	m_pkStopBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCNewForgeItemCraftMachine::OnClickStopBtn );

	g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_FORGE_CLICK_CREATION_ITEM_BAR, this, &KGCNewForgeItemCraftMachine::OnClickCreationItem );
	g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_FORGE_CLICK_BREAKEUP_ITEM, this, &KGCNewForgeItemCraftMachine::OnClickBreakUpItem );

	g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_FORGE_COMPLETE_RESULT_ITEM, this, &KGCNewForgeItemCraftMachine::OnCompleteItem );

	for( int i = 0; i < KItem::GRADE_NUM; i++ )
	{
		if( m_apkItemGradeBack[i] )
		{
			m_apkItemGradeBack[i]->ToggleRender( i == KItem::GRADE_NORMAL );
		}
	}
	m_bBypassForgeVerify = false;
}

void KGCNewForgeItemCraftMachine::OnClickCountBtn( const KActionEvent& event )
{
	if(event.m_pWnd == m_pkCountMinBtn)
	{
		ClickCountBtn(-3);
	}
	else if(event.m_pWnd == m_pkCountPreBtn)
	{
		ClickCountBtn(-1);
	}
	else if(event.m_pWnd == m_pkCountNextBtn)
	{
		ClickCountBtn(1);
	}
	else if(event.m_pWnd == m_pkCountMaxBtn)
	{
		ClickCountBtn(-2);
	}
}

void KGCNewForgeItemCraftMachine::ClickCountBtn( int iIncCount, bool isFreeType )
{
	int iMax = SiGCForgeManager()->GetMaxCreateCount( m_kCreateItemInfo.usIndex, m_kCreateItemInfo.ItemID );
	if ( !isFreeType )
	{
		if ( iIncCount == -3 )
		{
			m_iCreateCount = 1;
		}
		else if ( iIncCount == -2 )
		{
			m_iCreateCount = iMax;
		}
		else
		{
			if ( iIncCount + m_iCreateCount < 1 )
				m_iCreateCount = 1;
			else if ( iIncCount + m_iCreateCount > iMax )
				m_iCreateCount = iMax;
			else
				m_iCreateCount += iIncCount;
		}
	}
	else
	{
		if( iIncCount < 0 )
			iIncCount = 1;
		if ( iIncCount > iMax )
			iIncCount = iMax;
		m_iCreateCount = iIncCount;
	}
	SetUseNum( m_iCreateCount );
	std::vector<SMaterialInfo> vInfo;
	SiGCForgeManager()->GetMaterialList( m_kCreateItemInfo.usIndex, m_kCreateItemInfo.ItemID, vInfo );
	SetCreationItem( vInfo, m_kCreateItemInfo.ItemID, m_kCreateItemInfo.usIndex );
}

void KGCNewForgeItemCraftMachine::OnCompleteItem( const KActionEvent& event )
{
	const std::vector<KItem>& vecResultItem =  SiGCForgeManager()->GetResultItem();

	int size = vecResultItem.size();
	int i = 0;

	if( m_iForgeState == EKGCNEWFORGE_STATE::EKGCFOGE_CREATE  && size > 0)
	{
		g_KDSound.Play("219");
		m_apkForgeState[EMCS_SUCCESS]->ToggleRender( true );
	}

	std::vector<KGCMaterialItemInfo*>::iterator vIt = m_vecResultItemList.begin();

	for(;vIt != m_vecResultItemList.end();vIt++)
	{
		if(i < size)
		{
			(*vIt)->SetItem(vecResultItem[i].m_ItemID);
			(*vIt)->SetItemUID(vecResultItem[i].m_ItemUID);
			(*vIt)->ItemSetCount(vecResultItem[i].m_nCount);
		}
		else
		{
			(*vIt)->SetItemUID(0);
			(*vIt)->SetItem(0);
		}

		i++;
	}

	SetUserGPText();
}


void KGCNewForgeItemCraftMachine::OnClickStartBtn( const KActionEvent& event )
{
	m_bBypassForgeVerify = false;

	if(m_iForgeState == EKGCNEWFORGE_STATE::EKGCFOGE_CREATE)
	{
		MakeMaterialUIDList();

		for ( auto& element : m_vecMaterialUIDList )
		{
			KItem* tempItemForge = g_pItemMgr->m_kInventory.FindItemByItemUID( element );
			if ( tempItemForge->m_cEnchantLevel > 0 )
			{
				m_bBypassForgeVerify = true;
				break;
			}
			if ( !tempItemForge->m_vecSocket.empty() )
			{
				for ( auto& socket : tempItemForge->m_vecSocket )
				{
					if ( socket.m_cState == KSocketInfo::STS_USING )
					{
						m_bBypassForgeVerify = true;
						break;
					}
				}
			}
		}

		if ( m_bBypassForgeVerify )
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString(STR_FORGE_CONFIRM_CONFIRM), L"", KGCUIScene::GC_MBOX_USE_FORGE_CONFIRM, 0, 0, false, true);
		}
		else
		{
			OnClickStartBtnCreate();
		}
	}
	else
	{
		OnClickStartBtnBreakUp();
	}
}

void KGCNewForgeItemCraftMachine::OnClickStopBtn( const KActionEvent& event )
{
	m_pkStopBtn->Lock(true);
	m_pkStartBtn->Lock(false);

	m_iMachineState = EFMS_STOP;
}

void KGCNewForgeItemCraftMachine::OnClickStartBtnCreate( void )
{
	if(m_iMachineState == EFMS_NORMAL)
	{
		m_iMachineState = EFMS_START;
		m_pkStartBtn->Lock(true);
		m_pkStopBtn->Lock(false);
		g_KDSound.Play("Machine_Cylinder");
		m_apkForgeState[EMCS_CREATING]->ToggleRender( true );
	}
	else if(m_iMachineState == EFMS_CONTINUE )
	{
		m_bBypassForgeVerify = false;

		SiGCForgeManager()->CreateItem(m_kCreateItemInfo, m_iCreateCount, m_vecMaterialUIDList);

		std::vector<SMaterialInfo> vInfo;
		SiGCForgeManager()->GetMaterialList(m_kCreateItemInfo.usIndex, m_kCreateItemInfo.ItemID, vInfo);

		m_pkStartBtn->Lock(false);
		m_pkStopBtn->Lock(true);

		SetCreationItem(vInfo, m_kCreateItemInfo.ItemID, m_kCreateItemInfo.usIndex);
	}
}

void KGCNewForgeItemCraftMachine::OnClickStartBtnBreakUp( void )
{
	if(m_iMachineState == EFMS_NORMAL)
	{
		m_iMachineState = EFMS_START;
		m_pkStartBtn->Lock( true );
		m_pkStopBtn->Lock( false );
		m_apkForgeState[EMCS_SUCCESS]->ToggleRender(false);
		m_apkForgeState[EMCS_BREAKING]->ToggleRender( true );
		return;
	}

	std::vector<std::pair<GCITEMUID, GCITEMID>>& vInfo = SiGCForgeManager()->GetWaitItemList();
	std::vector<std::pair<GCITEMUID, GCITEMID>>::iterator vit = vInfo.begin();

	if(vit == vInfo.end())
	{
		m_iMachineState = EFMS_NORMAL;
		m_pkStartBtn->Lock(true);
		m_pkStopBtn->Lock(true);
	}
	else if(m_iMachineState == EFMS_CONTINUE )
	{
		m_pPrekResultItem->SetItemInfo(vit->second);
		m_pPrekResultItem->ToggleRender( true );
		if( SiGCForgeManager()->BreakUpItem(vit->first) )
		{
			g_KDSound.Play("Machine_Cylinder");
		}

		SiGCForgeManager()->MakeAllBreakUpItemList();
		SpeakToActionListener( KActionEvent( NULL, KGCUIScene::D3DWE_FORGE_SEL_CHARACTER));
		SetBreakItemList();

		if(vInfo.begin() != vInfo.end())
		{
			m_iMachineState = EFMS_START;
			m_pkStartBtn->Lock(true);
			m_pkStopBtn->Lock(false);
		}
		else
		{
			m_apkForgeState[EMCS_BREAKING]->ToggleRender( false );
			m_apkForgeState[EMCS_SUCCESS]->ToggleRender( true );
			m_pPrekResultItem->ToggleRender( false );
			m_iMachineState = EFMS_START;
			m_pkStartBtn->Lock(true);
			m_pkStopBtn->Lock(true);
		}
	}
}

void KGCNewForgeItemCraftMachine::InitCountBtn()
{
	GCItem *pItem =  g_pItemMgr->GetItemData(m_kCreateItemInfo.ItemID);
	bool bPeriod = true;
	if(pItem != NULL)
		bPeriod = pItem->eItemType == 1;

	bool bCanIncCount = (m_iForgeState == EKGCNEWFORGE_STATE::EKGCFOGE_CREATE) &&
		(SiGCForgeManager()->GetCurrentSlot() == GCForgeManager::ESFB_ETC ||
		SiGCForgeManager()->GetCurrentSlot() == GCForgeManager::ESFB_EVENT ||
		SiGCForgeManager()->GetCurrentSlot() == GCForgeManager::ESFB_VIP || SiGCForgeManager()->GetCurrentSlot() == GCForgeManager::ESFB_ALL ) && m_kCreateItemInfo.ItemID != 0 && bPeriod; 

	m_pkCountMinBtn->ToggleRender(bCanIncCount);
	m_pkCountMaxBtn->ToggleRender(bCanIncCount);
	m_pkCountPreBtn->ToggleRender(bCanIncCount);
	m_pkCountNextBtn->ToggleRender(bCanIncCount);
	m_pkCountItemBack->ToggleRender(bCanIncCount);
	m_pkCountEditBG->ToggleRender(bCanIncCount);
	m_pkCountEdit->ToggleRender(bCanIncCount);
}

void KGCNewForgeItemCraftMachine::ShowPrizeRarity( const IN char cGradeID )
{
	for( int i = 0; i < KItem::GRADE_NUM; i++ )
	{
		if (m_apkItemGradeBack[i] != nullptr)
		{
			m_apkItemGradeBack[i]->ToggleRender(false);
		}
	}

	m_apkItemGradeBack[cGradeID]->ToggleRender(true);
}

void KGCNewForgeItemCraftMachine::SetCreationItem(std::vector<SMaterialInfo> vInfo, GCITEMID giItemID, int iIndex)
{
	int size = vInfo.size();
	int i = 0;

	if ( m_pPrekResultItem->GetItemID() != giItemID )
	{
		m_pPrekResultItem->SetItemInfo( giItemID );
		m_pPrekResultItem->ToggleRender( true );
	}
	
	m_pkCost->SetNumber(0);
	m_pkStartBtn->Lock(false);

	const std::vector<RecipeSimpleInfo>& vecInfoList = SiGCForgeManager()->GetShowRecipeList();

	for(std::vector<RecipeSimpleInfo>::const_iterator cvIt = vecInfoList.begin();cvIt!=vecInfoList.end();cvIt++)
	{
		if(cvIt->ItemID == giItemID && static_cast<int>(cvIt->usIndex) == iIndex)
		{
			m_pkCost->SetNumber(cvIt->iPrice * m_iCreateCount);
			SetUserGPText( cvIt->iPrice * m_iCreateCount );

			m_kCreateItemInfo = *cvIt;
			m_pkStaticPreResultItemNum->SetNumber(m_kCreateItemInfo.cFactor* m_iCreateCount);
			GCItem *pItem =  g_pItemMgr->GetItemData(m_kCreateItemInfo.ItemID);
			bool bPeriod = true;
			if(pItem != NULL)
				bPeriod = pItem->eItemType == 1;

			ShowPrizeRarity(m_kCreateItemInfo.cGrade);

			if( m_kCreateItemInfo.cFactor <= 0 || bPeriod == false ) 
			{
				m_pkStaticPreResultItemNum->ToggleRender(false);
			}
			else 
			{
				m_pkStaticPreResultItemNum->ToggleRender(true);
			}
			
			break;
		}
	}

	std::vector<KGCMaterialItemInfo*>::iterator vIt = m_vecMaterialItemList.begin();

	for(; vIt != m_vecMaterialItemList.end(); ++vIt)
	{
		(*vIt)->SetItemUID(0);
		(*vIt)->SetItem(0);

		if( i < size )
		{
			GCITEMUID itemUID = 0;
			if( GetNotCoordiItemUID( vInfo[i].m_ItemID, itemUID ) ) 
			{ 
				(*vIt)->SetItemUID(itemUID);
			}

			DWORD fontColor = 0;
			DWORD imgColor = 0;

			if(SiGCForgeManager()->GetMaterialColorByUID( itemUID, vInfo[i].m_nDuration * m_iCreateCount, vInfo[i].m_nLevel, imgColor, fontColor ) == false)
			{
				m_pkStartBtn->Lock(true);
				(*vIt)->EnableSelection( false );
			}
			else 
			{ 
				if( vInfo[i].m_nDuration == -1  ) 
				{ 
					GCItem* pItemData = g_pItemMgr->GetItemData( vInfo[i].m_ItemID );
					if( pItemData->eItemKind != GIK_ENCHANT_ENABLE_ITEM ) 
					{
						(*vIt)->EnableSelection( true );
					}
					else
					{
						(*vIt)->EnableSelection( false );
					}
				}
				else 
				{ 
					(*vIt)->EnableSelection( false );
				}
			}

			if( false == g_pkUIScene->m_pkForgeDlg->IsEnabledRecipeBar( giItemID ) ) 
			{ 
				m_pkStartBtn->Lock(true);
			}

			(*vIt)->SetItem(vInfo[i].m_ItemID, imgColor, fontColor);
			(*vIt)->ItemSetCount( vInfo[i].m_nDuration );
			(*vIt)->ItemSetLevel( vInfo[i].m_nLevel );
		}

		(*vIt)->ToggleRender(true);
		++i;
	}

	InitCountBtn();
}

void KGCNewForgeItemCraftMachine::OnClickCreationItem( const KActionEvent& event )
{
	m_iCreateCount = 1;
	int iIndex = static_cast<int>(event.m_dwlParam);
	GCITEMID giItemID = static_cast<GCITEMID>(event.m_dwlParam2);
	std::vector<SMaterialInfo> vInfo;
	if(SiGCForgeManager()->GetMaterialList(iIndex,giItemID, vInfo) == false) return;

	m_kCreateItemInfo.Clear();
	SetCreationItem(vInfo, giItemID, iIndex);
	ClearResultItemList();
}


void KGCNewForgeItemCraftMachine::OnClickBreakUpItem( const KActionEvent& event )
{
	SetBreakItemList();
	ClearResultItemList();
}

void KGCNewForgeItemCraftMachine::OnClickWaitBreakUpItem( const KActionEvent& event )
{
	if(m_iForgeState == EKGCNEWFORGE_STATE::EKGCFOGE_CREATE) return;
	if(m_iMachineState != EFMS_NORMAL ) return;
	GCITEMUID guItemUID = 0;


	for(int i = 0;i<static_cast<int>(m_vecMaterialItemList.size());i++)
	{
		if(m_vecMaterialItemList[i] == event.m_pWnd)
		{
			guItemUID = m_vecMaterialItemList[i]->GetItemUID();
			break;
		}
	}

	SiGCForgeManager()->EraseWaitItem(guItemUID);
	SpeakToActionListener( KActionEvent( NULL, KGCUIScene::D3DWE_FORGE_SEL_CHARACTER));
	SetBreakItemList();
	ClearResultItemList();
}



void KGCNewForgeItemCraftMachine::SetBreakItemList( )
{
	std::vector<std::pair<GCITEMUID, GCITEMID>>& vInfo = SiGCForgeManager()->GetWaitItemList();
	int size = vInfo.size();
	int i = 0;

	for(std::vector<KGCMaterialItemInfo*>::iterator vIt = m_vecMaterialItemList.begin(); vIt != m_vecMaterialItemList.end();vIt++)
	{
		KGCMaterialItemInfo* materialInfo = (*vIt);
		if(i < size)
		{
			materialInfo->SetItemUID(vInfo[i].first);
			materialInfo->SetItem(vInfo[i].second);
			materialInfo->ItemSetCount( -1 );
		}
		else
		{
			materialInfo->SetItemUID(0);
			materialInfo->SetItem(0);
		}
		++i;
	}

	if(vInfo.empty())
		m_pkStartBtn->Lock(true);
	else
		m_pkStartBtn->Lock(false);
}

void KGCNewForgeItemCraftMachine::ClearResultItemList( void )
{
	for(std::vector<KGCMaterialItemInfo*>::iterator vIt = m_vecResultItemList.begin(); vIt != m_vecResultItemList.end();vIt++)
	{
		(*vIt)->SetItemUID(0);
		(*vIt)->SetItem(0);
	}
}

void KGCNewForgeItemCraftMachine::ClearItemList( void )
{
	for(std::vector<KGCMaterialItemInfo*>::iterator vIt = m_vecMaterialItemList.begin(); vIt != m_vecMaterialItemList.end();vIt++)
	{
		SiGCForgeManager()->EraseWaitItem((*vIt)->GetItemUID());
		(*vIt)->SetItemUID(0);
		(*vIt)->SetItem(0);
	}
}

void KGCNewForgeItemCraftMachine::OnCreateItemList( void )
{
	if(m_vecMaterialItemList.empty() == true){
		std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_new_forge.stg", "","new_forge_dlg\\new_forge_item_crafting_dlg\\material_item",true,m_iMAX_ITEM_LIST_WIDTH_COUNT * m_iMAX_ITEM_LIST_HEGHIT_COUNT );

		for(std::vector<KD3DWnd*>::iterator vIt = vecTemp.begin(); vIt != vecTemp.end();vIt++)
		{
			m_vecMaterialItemList.push_back(static_cast<KGCMaterialItemInfo*>(*vIt));
		}
	}

	if(m_vecResultItemList.empty() == true)
	{
		std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_new_forge.stg", "","new_forge_dlg\\new_forge_item_crafting_dlg\\material_item",true,m_iMAX_RESULT_ITEM_LIST_WIDTH * m_iMAX_RESULT_ITEM_LIST_HEGHIT );

		for(std::vector<KD3DWnd*>::iterator vIt = vecTemp.begin(); vIt != vecTemp.end();vIt++)
		{
			m_vecResultItemList.push_back(static_cast<KGCMaterialItemInfo*>(*vIt));
		}
	}
}

void KGCNewForgeItemCraftMachine::InitItemList( void )
{
	for(int i = 0 ; i < m_iMAX_ITEM_LIST_HEGHIT_COUNT; i++)
	{
		for(int j = 0; j < m_iMAX_ITEM_LIST_WIDTH_COUNT; j++)
		{
			int iIndex =  ( (m_iMAX_ITEM_LIST_WIDTH_COUNT * m_iMAX_ITEM_LIST_HEGHIT_COUNT) - 1 ) - ( i * m_iMAX_ITEM_LIST_WIDTH_COUNT + j );

			float x = static_cast<float>(222.f * GC_SCREEN_DIV_WIDTH - (j * 71.f * GC_SCREEN_DIV_WIDTH));
			float y = static_cast<float>(160.f * GC_SCREEN_DIV_WIDTH - (i * 71.f * GC_SCREEN_DIV_WIDTH));
			
			D3DXVECTOR2 vNewPos(x,y);
			m_vecMaterialItemList[iIndex]->InitState(true, true,this);
			m_vecMaterialItemList[iIndex]->SetWindowPos( vNewPos );
			m_vecMaterialItemList[iIndex]->SetWidthDirect( GC_SCREEN_DIV_SIZE_INT(70) );
			m_vecMaterialItemList[iIndex]->SetHeightDirect( GC_SCREEN_DIV_SIZE_INT(70) );

			m_vecMaterialItemList[iIndex]->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCNewForgeItemCraftMachine::OnClickWaitBreakUpItem);
		}
	}

	for( int i = 0; i < m_iMAX_RESULT_ITEM_LIST_WIDTH; i++ )
	{
		int iIndex = (m_iMAX_RESULT_ITEM_LIST_WIDTH - 1) - i;
		float x = static_cast<float>(185.f * GC_SCREEN_DIV_WIDTH - (i * 71.f * GC_SCREEN_DIV_WIDTH));

		D3DXVECTOR2 vNewPos(x, 560.f);
		m_vecResultItemList[iIndex]->InitState(true, true,this);
		m_vecResultItemList[iIndex]->SetWindowPos( vNewPos );
		m_vecResultItemList[iIndex]->SetWidthDirect( GC_SCREEN_DIV_SIZE_INT(70) );
		m_vecResultItemList[iIndex]->SetHeightDirect( GC_SCREEN_DIV_SIZE_INT(70) );
	}

	m_pPrekResultItem->InitState(true, false,NULL);
	m_pkStaticPreResultItemNum->InitState(true,false,NULL);
	m_pkStaticPreResultItemNum->SetAlign( DT_RIGHT );
}

void KGCNewForgeItemCraftMachine::InitCraftMachine()
{
	if(m_pPrekResultItem)
	{
		m_pPrekResultItem->ToggleRender( false );
	}

	if( m_pkStaticPreResultItemNum )
	{
		m_pkStaticPreResultItemNum->ToggleRender( false );
	}

	for(int i = 0; i < KItem::GRADE_NUM; i++)
	{
		if( m_apkItemGradeBack[i] )
		{
			m_apkItemGradeBack[i]->ToggleRender( false );
		}
	}

	m_apkItemGradeBack[KItem::GRADE_NORMAL]->ToggleRender( true );

	for( int i = 0; i < EMCS_MAX; i++ )
	{
		m_apkForgeState[i]->ToggleRender( false );
	}

	if(m_pkCost)
	{
		m_pkCost->SetNumber(0);
	}

	SetUserGPText();

	if(m_pkStartBtn && m_pkStopBtn)
	{
		m_pkStartBtn->Lock(true);
		m_pkStopBtn->Lock(true);
	}

	m_iMachineState = EFMS_NORMAL;
	m_kCreateItemInfo.Clear();

	InitCountBtn();
	ClearItemList();
	ClearResultItemList();

}

void KGCNewForgeItemCraftMachine::SetForgeState( EKGCNEWFORGE_STATE iState )
{
	m_iForgeState = iState;
	InitCraftMachine();

	m_pkCost->SetNumber(0);
	SetUserGPText();
}

void KGCNewForgeItemCraftMachine::FrameMoveInEnabledState( )
{
	MoveCraftMachine();
	OnTextEnter();
}

void KGCNewForgeItemCraftMachine::BoundItemList( )
{
	if( CheckBoundItem(m_vecMaterialItemList) ) return;
	else if(CheckBoundItem(m_vecResultItemList)) return;
}

bool KGCNewForgeItemCraftMachine::GetItemInfo( KItem** pInven, GCItem** pItem, KGCMaterialItemInfo* iter_)
{
	if( m_iForgeState == EKGCNEWFORGE_STATE::EKGCFOGE_CREATE )
	{ 
		g_pItemMgr->FindInventory(iter_->GetItemUID(), pInven, pItem);

		if( *pInven == NULL ) 
		{ 
			*pItem = g_pItemMgr->GetItemData( iter_->GetItemID() );
		}

		return (*pInven == NULL && *pItem == NULL) == true;
	}

	return g_pItemMgr->FindInventory(iter_->GetItemUID(), pInven, pItem);
}


bool KGCNewForgeItemCraftMachine::CheckBoundItem(std::vector<KGCMaterialItemInfo*>& vecItemList)
{
	bool bCheck = false;
	for(std::vector<KGCMaterialItemInfo*>::iterator vIt = vecItemList.begin(); vIt != vecItemList.end(); vIt++)
	{
		if((*vIt)->CheckMousePosInWindowBound())
		{
			KItem* pInventory = NULL;
			GCItem* pItem = NULL;

			if( false == GetItemInfo(&pInventory, &pItem,*vIt))
			{
				continue;
			}

			g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem , -1, pInventory);
			D3DXVECTOR2 vec = (*vIt)->GetCurrentWindowLocalPos();

			vec.x += 49.f * GC_SCREEN_DIV_WIDTH;
			vec.y += 20.f * GC_SCREEN_DIV_WIDTH;
			//vec.x += (*vIt)->GetWidth();
			vec.x *= m_fWindowScaleX;
			vec.y *= m_fWindowScaleY;

			g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vec );
			g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(KD3DWnd::D3DWZ_TOPMOST);
			bCheck = true;
			break;
		}
	}

	g_pkUIScene->m_pkItemInformationBox->ToggleRender( bCheck );
	return bCheck;
}

void KGCNewForgeItemCraftMachine::MoveCraftMachine( )
{
	if(m_iMachineState == EFMS_NORMAL) 
	{
		return;
	}

	if(m_iMachineDelayCnt == -1)
	{
		m_iMachineDelayCnt = 0;
		m_iMachineState = EFMS_NORMAL;
		m_apkForgeState[EMCS_SUCCESS]->ToggleRender( false );
		return;
	}
	else if(m_iMachineDelayCnt == 60 && m_iForgeState == EKGCNEWFORGE_STATE::EKGCFOGE_CREATE)
	{
		m_iMachineState = EFMS_CONTINUE;
		OnClickStartBtnCreate();
		m_apkForgeState[EMCS_CREATING]->ToggleRender( false );
		m_iMachineDelayCnt = 40;
		return;
	}
	else if(m_iMachineDelayCnt == 60 && m_iForgeState == EKGCNEWFORGE_STATE::EKGCFOGE_BREAKUP)
	{
		m_iMachineState = EFMS_CONTINUE;
		OnClickStartBtnBreakUp();
		m_iMachineDelayCnt = 0;
		return;
	}

	m_iMachineDelayCnt += ( (m_iMachineState == EFMS_START) ? 1 : -1 );
}


void KGCNewForgeItemCraftMachine::MakeMaterialUIDList( void )
{
	m_vecMaterialUIDList.clear();

	std::vector<KGCMaterialItemInfo*>::const_iterator vitList = m_vecMaterialItemList.begin();
	for( ; vitList != m_vecMaterialItemList.end() ; ++vitList ) 
	{
		if( (*vitList) == NULL || (*vitList)->GetItemID() == KItem::INVALID_ITEM || (*vitList)->GetItemUID() == 0 ) 
		{ 
			continue;
		}

		m_vecMaterialUIDList.push_back( (*vitList)->GetItemUID() );
	}
}

void KGCNewForgeItemCraftMachine::SetSelectedMaterial( IN const GCITEMUID itemUID_ )
{
	KItem* pSelItem = NULL;
	if( false == g_pItemMgr->FindInventory( itemUID_ , &pSelItem ) ) 
	{
		return;
	}

	if( NULL == pSelItem ) 
	{ 
		return;
	}

	std::vector<KGCMaterialItemInfo*>::iterator itemIter = m_vecMaterialItemList.begin();
	for( ; itemIter != m_vecMaterialItemList.end() ; ++itemIter ) 
	{ 
		KItem* pBeforeItem = NULL;

		if( false == g_pItemMgr->FindInventory( (*itemIter)->GetItemUID(), &pBeforeItem ) ) 
		{ 
			continue;
		}

		if( NULL == pBeforeItem ) 
		{ 
			return;
		}

		if( pSelItem->m_ItemID == pBeforeItem->m_ItemID ) 
		{ 
			if( pSelItem->m_ItemUID != pBeforeItem->m_ItemUID)
			{
				(*itemIter)->SetItem(pSelItem->m_ItemID);
				(*itemIter)->SetItemUID(itemUID_);
			}
			break;
		}
	}
}

bool KGCNewForgeItemCraftMachine::GetNotCoordiItemUID( const IN GCITEMID itemID_, OUT GCITEMUID& itemUID_ )
{
	int iGrade = -1;
	itemUID_ = 0;

	std::vector< KItem* > vecInvenItem;
	if( false == g_pItemMgr->GetInventoryItemListFromItemID( itemID_, vecInvenItem ) ) 
	{ 
		return false;
	}

	GCItem* pItemData = g_pItemMgr->GetItemData( itemID_ );
	if( NULL == pItemData ) 
	{ 
		return false;
	}

	std::vector< KItem* >::const_iterator itItem = vecInvenItem.begin();
	for( ; itItem != vecInvenItem.end() ; ++itItem ) 
	{ 
		GCItem* pItem = g_pItemMgr->GetItemData( (*itItem)->m_ItemID );
		CONTINUE_NIL( pItem );

		if( pItem->eItemKind == GIK_ENCHANT_ENABLE_ITEM ) { // 강화석 처리도 여기서 합니다...
			if( (*itItem)->m_EnchantEquipItemUID != 0 || // 장착된 강화석이거나 
				(*itItem)->m_cGradeID != KItem::GRADE_NORMAL || // 노멀 등급이 아니거나 
				(*itItem)->m_cEnchantLevel != 0 ) { // 0강이 아니거나 
					continue;
			}
		}

		if( pItem->eItemKind == GIK_COORDI_ITEM && (*itItem)->m_cItemType == KItem::TYPE_NORMAL ) 
		{ 
			continue;
		}

		if( pItem->eItemKind != GIK_COORDI_ITEM && (*itItem)->m_cItemType == KItem::TYPE_LOOK ) 
		{ 
			continue;
		}

		// 장창중인 아이템은 제외 
		if( g_pItemMgr->CheckEquipItemByItemUID( &g_kGlobalValue.m_kUserInfo.GetCurrentChar(), (*itItem)->m_ItemUID ) ) 
		{
			continue;
		}

		// 기간제 코디 제외 
		if( pItem->eItemKind == GIK_COORDI_ITEM && (*itItem)->m_nPeriod != KItem::UNLIMITED_ITEM ) { 
			continue;
		}

		//  동일한 UID가 나오면 안된다.
		if( IsSelectedMaterialItem( (*itItem)->m_ItemUID )  )
			continue;

		itemUID_ = (*itItem)->m_ItemUID;
		break;
	}

	return true;
}

bool KGCNewForgeItemCraftMachine::IsSelectedMaterialItem( const IN GCITEMUID itemUID_ )
{
	std::vector<KGCMaterialItemInfo*>::iterator vIt = m_vecMaterialItemList.begin();

	for ( ; vIt != m_vecMaterialItemList.end(); ++vIt ) 
	{
		if ( (*vIt)->GetItemUID() == itemUID_ )
		{
			return true;
		}
	}

	return false;
}

void KGCNewForgeItemCraftMachine::SetUserGPText( const int iConst_ )
{
	if(m_pkGP ) 
	{ 
		int iGPValue = g_kGlobalValue.GetCurrentCharacterGP();
		m_pkGP->SetFontColor( (iGPValue - iConst_ >= 0) ? 0xffffffff : 0xffff0000 );
		m_pkGP->SetNumber( iGPValue );
	}
}

void KGCNewForgeItemCraftMachine::OnTextEnter()
{
	int count = static_cast<int>( _wtoi( m_pkCountEdit->GetText() ) );
	if ( count == m_iCreateCount )
		return;
	if ( count < 1 )
		count = 1;
	ClickCountBtn( count, true );
}

void KGCNewForgeItemCraftMachine::SetUseNum( int iNum )
{
	std::wostringstream strmItemNum;
	strmItemNum << iNum;
	m_pkCountEdit->SetText( strmItemNum.str().data(), false );
}