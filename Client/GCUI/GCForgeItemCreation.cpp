#include "stdafx.h"
#include "GCForgeItemCreation.h"
#include "GCRecipeInfoBar.h"

IMPLEMENT_CLASSNAME( KGCForgeItemCreation );
IMPLEMENT_WND_FACTORY( KGCForgeItemCreation );
IMPLEMENT_WND_FACTORY_NAME( KGCForgeItemCreation, "gc_forge_item_creation_dlg" );

KGCForgeItemCreation::KGCForgeItemCreation( void )
: m_iSelectIndex(0)
, m_bCheckBox(false)
, m_pkScrollbar(false)
{
    for(int i =0;i<GCForgeManager::ESFB_MAX;i++){
        char strTemp[ MAX_PATH ] = "";
        m_saSloatFilterBtnList[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "slot_filter_btn%d", i);
        LINK_CONTROL(strTemp,m_saSloatFilterBtnList[i]);

        m_saSloatFilterSelectedBtnList[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "slot_filter_selected%d", i);
        LINK_CONTROL(strTemp,m_saSloatFilterSelectedBtnList[i]);
    }

    for(int i =0;i<4;i++){
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
    LINK_CONTROL( "background", m_pkBackground );
    LINK_CONTROL( "scroll", m_pkScrollbar );

    m_vecRecipeInfoBar.reserve( mc_iMaxRecipeViewHeightCount );
}

KGCForgeItemCreation::~KGCForgeItemCreation( void )
{
    SiGCForgeManager()->isForgeSearchActive = false;
}

void KGCForgeItemCreation::ActionPerformed( const KActionEvent& event )
{
    GCWND_MAP( m_pkScrollbar, OnScrollPos );
}

void KGCForgeItemCreation::OnCreate(void)
{
    if (SiKP2P()->IsConnectedToGameServer() == true)
    {
        SiGCForgeManager()->Send_GetItemCatalog();
        // 해제정보 요청
        SiGCForgeManager()->Send_GetItemBreakupInfo();
    }

    InitSloatBtnList();
    InitSortBtnList();
    CreateInfobarList();
    InitInfobarList();
    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_FORGE_CHANGE_PAGE, this, &KGCForgeItemCreation::OnChangePage);
    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_FORGE_SEL_CHARACTER, this, &KGCForgeItemCreation::OnChangeCharacter);
    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_FORGE_COMPLETE_RESULT_ITEM, this, &KGCForgeItemCreation::SetInfobarList);
    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_FORGE_BOUND_ITEM, this, &KGCForgeItemCreation::OnBoundItem);

    if (m_pkScrollbar)
    {
        m_pkScrollbar->InitState(true, true, this);
        m_pkScrollbar->SetScrollPos(0);
        m_pkScrollbar->SetScrollRangeMin(0);
        m_pkScrollbar->SetScrollRangeMax(1);
        m_pkScrollbar->SetScrollPageSize(1);
    }

    InitItemCreation();
    g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation(NULL);
    SiGCForgeManager()->isForgeSearchActive = true;

    SiGCForgeManager()->UpdateForgeSearch(L"");
    SpeakToActionListener(KActionEvent(this, KGCUIScene::D3DWE_FORGE_CHANGE_PAGE));
}

void KGCForgeItemCreation::OnScrollPos()
{
    int iPos = m_pkScrollbar->GetScrollPos();

    if (m_iScrollpos != iPos)
    {
        m_iScrollpos = iPos;
        g_KDSound.Play("73");

        SetInfobarList();
        OnInitInfobar();
    }
}

void KGCForgeItemCreation::OnBoundItem( const KActionEvent& event )
{
    m_bCheckBox = event.m_dwlParam != 0 ;
}

void KGCForgeItemCreation::InitItemCreation( void )
{
    SelectGradeBtn(GCForgeManager::EGFB_ALL);
    SelectSloatBtn(GCForgeManager::ESFB_ALL);

}

void KGCForgeItemCreation::InitGradeBtnList( void )
{
}

void KGCForgeItemCreation::InitSloatBtnList( void )
{
    for(int i =0;i<GCForgeManager::ESFB_MAX;i++){

        m_saSloatFilterBtnList[i]->InitState(false,true,this);
        m_saSloatFilterBtnList[i]->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCForgeItemCreation::OnSelectSloatBtnList);
        m_saSloatFilterSelectedBtnList[i]->InitState(false,false,NULL);
    }
}

void KGCForgeItemCreation::InitSortBtnList( void )
{
    for(int i =0;i<GCForgeManager::ERSB_MAX;i++){

        m_saUpSortList[i]->InitState(false,false,NULL);
        m_saDownSortList[i]->InitState(false,false,NULL);

        m_saSortBtnList[i]->InitState(true,true,this);
        m_saSortBtnList[i]->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCForgeItemCreation::OnSelectSortBtnList);
    }

    m_saDownSortList[GCForgeManager::ERSB_ITEMLEVEL]->ToggleRender(true);
    SiGCForgeManager()->InitSort();
}

void KGCForgeItemCreation::CreateInfobarList(void)
{
    if (m_iMaxRecipeViewCount == m_vecRecipeInfoBar.size() && !m_vecRecipeInfoBar.empty())
    {
        return;
    }

    int iMaxRecipeViewHeightCount = static_cast<int>(mc_iDefaultRecipeViewHeightCount * m_fWindowScaleY);

    if (iMaxRecipeViewHeightCount > mc_iMaxRecipeViewHeightCount)
    {
        iMaxRecipeViewHeightCount = mc_iMaxRecipeViewHeightCount;
    }

    DWORD dwHeight = static_cast<DWORD>(mc_iMaxRecipeViewRealHeight / iMaxRecipeViewHeightCount);
    float fMaxHeightInterval = static_cast<float>(mc_iMaxRecipeViewRealHeight - (dwHeight * iMaxRecipeViewHeightCount) + mc_iMaxRecipeViewIntervalHeight);

    int iHeightIntervalCount = iMaxRecipeViewHeightCount - 1;
    float fHeightInterval = 0.0f;

    if (iHeightIntervalCount > 0)
    {
        fHeightInterval = fMaxHeightInterval / iHeightIntervalCount;
    }

    float fOffsetY = (fMaxHeightInterval - (fHeightInterval * iHeightIntervalCount)) / 2.0f;

    D3DXVECTOR2 vStartPos(28.0f * GC_SCREEN_DIV_WIDTH, (130.0f * GC_SCREEN_DIV_WIDTH + fOffsetY) + 10);

    int iRecipeInfoBarCount = static_cast<int>(m_vecRecipeInfoBar.size());
    m_iMaxRecipeViewCount = iMaxRecipeViewHeightCount;

    for (int i = 0; i < m_iMaxRecipeViewCount; ++i)
    {
        if (i >= iRecipeInfoBarCount)
        {
            char szMakeWndName[MAX_PATH] = "";
            sprintf_s(szMakeWndName, "recipe_info_bar%d", i);
            KGCRecipeInfoBar* pkRecipeBar = static_cast<KGCRecipeInfoBar*>(g_pkUIMgr->CreateTemplateUIByFindName(this, "ui_new_forge.stg", szMakeWndName, "new_forge_dlg\\forge_item_creation_dlg\\creation_recipe_bar_temp"));
            m_vecRecipeInfoBar.push_back(pkRecipeBar);
        }

        float fHeightSize = static_cast<float>(dwHeight) + fHeightInterval;
        D3DXVECTOR2 vNewPos(vStartPos.x, vStartPos.y + i * fHeightSize);

        m_vecRecipeInfoBar[i]->InitState(false, true, this);
        m_vecRecipeInfoBar[i]->SetWindowPosDirect(vNewPos);
        m_vecRecipeInfoBar[i]->SetHeightDirect(dwHeight);
    }
}

void KGCForgeItemCreation::InitInfobarList( void )
{
    for(std::vector< KGCRecipeInfoBar* >::iterator vIt = m_vecRecipeInfoBar.begin();vIt !=m_vecRecipeInfoBar.end();vIt++){
        (*vIt)->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCForgeItemCreation::OnSelectInfobar);
    }

}

void KGCForgeItemCreation::SetInfobarString(GCItem* pItem, KGCRecipeInfoBar* pBar,const RecipeSimpleInfo& csInfo )
{
    if(pItem == NULL || pBar == NULL) return;

    WCHAR strTemp[ MAX_PATH ] = L"";
    _itow_s(csInfo.iItemLevel, strTemp, MAX_PATH, 10 );
    std::wstring strItemLevel = strTemp;

    std::wstring strItemName = pItem->strItemName;

    _itow_s( csInfo.iPrice, strTemp, MAX_PATH, 10 ); 
    std::wstring strPrice = strTemp;

    wstring strGradeString = g_pItemMgr->GetItemGradeForgeString( pItem->cItemGrade );
    pBar->SetTextRecipeInfoBar( strItemLevel, strItemName, strGradeString, strPrice );

    pBar->SetIndex( csInfo.usIndex );
    pBar->SetItemID( csInfo.ItemID );


}

void KGCForgeItemCreation::SetInfobarStringColor(GCItem* pItem, KGCRecipeInfoBar* pBar,const RecipeSimpleInfo& csInfo )
{
    if( NULL == pItem || NULL == pBar ) { 
        return;
    }

    //Forge Discount
    int totalDiscount = 0;

    g_kGlobalValue.currentTotalForgeDiscount = 0;

    if (g_pItemMgr->FindInventoryForItemID(ITEM_BLACKSMITH_DISCOUNT))
        g_kGlobalValue.currentTotalForgeDiscount = 0.15;

    if (g_pItemMgr->FindInventoryForItemID(ITEM_VIP_TEMPORARY_ITEM))
        g_kGlobalValue.currentTotalForgeDiscount = 0.30;

    DWORD dwFontColor = 0xff6f6f6f;
    if( csInfo.iPrice <= g_kGlobalValue.m_kUserInfo.GetCurrentChar().GetCurrentGP() ) { 
        GCForgeManager::MAP_FM_MATERIAL_LIST::const_iterator mit = SiGCForgeManager()->GetMeterialList().find(PAIR_USHORT_DWORD(csInfo.usIndex, csInfo.ItemID));

        std::map< GCITEMID, int > mapEquipMatNeed;  // 영구 아이템 필요 갯수 
        std::map< GCITEMID, int > mapEquipMatHave;  // 영구 아이템 보유 갯수 
        std::map< GCITEMID, int > mapCountMatNeed;  // 수량 아이템 필요 갯수 
        std::map< GCITEMID, int > mapCountMatHave;  // 수량 아이템 보유 갯수 

        if( mit != SiGCForgeManager()->GetMeterialList().end() ) { 
            std::vector<KDropItemInfo>::const_iterator vit = mit->second.begin();
            for( ; vit != mit->second.end(); ++vit )
            {
                if( (*vit).m_nDuration < 0 )    // 영구 아이템 재료 
                {
                    // 보유 갯수 저장
                    std::vector< KItem* > vecItemList;
                    if( g_pItemMgr->GetInventoryItemListFromItemID( (*vit).m_ItemID, vecItemList ) ) { 
                            
                        // 재료가 강화석일 경우 노말0강만 넣어줍시다. 
                        for( std::vector< KItem* >::iterator matVit = vecItemList.begin() ; matVit != vecItemList.end() ; ) { 

                            if( (*matVit)->m_ItemID == KGCItemManager::ITEM_ENCHANT_STONE ) { // 강화석입니다. 
                                if( (*matVit)->m_cGradeID == 0 && (*matVit)->m_cEnchantLevel == 0 && (*matVit)->m_EnchantEquipItemUID == 0 ) {
                                    ++matVit;
                                }
                                else { // 노말 0강 아님
                                    matVit = vecItemList.erase( matVit );
                                }
                            }
                            else { 
                                ++matVit;
                            }

                        }

                        mapEquipMatHave.insert( std::make_pair( (*vit).m_ItemID, static_cast<int>( vecItemList.size() ) ) );    
                    }

                    // 필요 갯수 저장 
                    std::map< GCITEMID, int >::iterator matIter = mapEquipMatNeed.find( (*vit).m_ItemID );
                    if( mapEquipMatNeed.end() == matIter ) { 
                        mapEquipMatNeed.insert( std::make_pair( (*vit).m_ItemID, 1 ) );
                    }
                    else { 
                        matIter->second++;
                    }
                }
                else
                {
                    if ( g_kGlobalValue.currentTotalForgeDiscount > 0 && (*vit).m_nDuration > 0 )
                        totalDiscount = (int)ceil( (*vit).m_nDuration - ( (*vit).m_nDuration * g_kGlobalValue.currentTotalForgeDiscount ) );
                    else
                        totalDiscount = (*vit).m_nDuration;

                    mapCountMatHave.insert( std::make_pair( (*vit).m_ItemID, g_pItemMgr->GetInventoryItemDuration( (*vit).m_ItemID ) ) );   // 보유 갯수 저장 
                    mapCountMatNeed.insert( std::make_pair( (*vit).m_ItemID, totalDiscount ) );                                        // 필요갯수 거장
                }
            }
        }


        if( false == mapEquipMatNeed.empty() && false == mapCountMatNeed.empty() ) {    // 영구, 수량 재료 모두 필요한 경우 
            // 영구 아이템 조건 맞는지 체크 
            bool bEquipMatEnable = false;
            std::map< GCITEMID, int >::iterator equipMatIter = mapEquipMatNeed.begin();
            for( ; equipMatIter != mapEquipMatNeed.end() ; ++equipMatIter ) { 
                std::map< GCITEMID, int >::iterator matIter = mapEquipMatHave.find( equipMatIter->first );
                if( matIter == mapEquipMatHave.end() ) {                // 보유 아이템 없음
                    bEquipMatEnable = false;
                    break;
                }
                else if ( equipMatIter->second > matIter->second ) {    // 갯수 모자람 
                    bEquipMatEnable = false;
                    break;
                }
                bEquipMatEnable = true;
            }

            // 수량 아이템 조건 맞는지 체크 
            bool bCountMatEnable = false;
            std::map< GCITEMID, int >::iterator countMatIter = mapCountMatNeed.begin();
            for( ; countMatIter != mapCountMatNeed.end() ; ++countMatIter ) { 
                std::map< GCITEMID, int >::iterator matIter = mapCountMatHave.find( countMatIter->first );
                if( matIter == mapCountMatHave.end() ) {                    //  보유 아이템 없음 
                    bCountMatEnable = false;
                    break;
                }
                else if( countMatIter->second > matIter->second )  {        // 갯수 모자람 
                    bCountMatEnable = false;
                    break;
                }
                bCountMatEnable = true;
            }

            // 글자색 
            if( bEquipMatEnable && bCountMatEnable ) { 
                dwFontColor = 0xffffffff;
            }
            else { 
                dwFontColor = 0xff6f6f6f;
            }
        }
        else if( false == mapEquipMatNeed.empty() && mapCountMatNeed.empty() ) {        // 영구 재료 아이템만 필요한 경우 
            bool bEquipMatEnable = false;
            std::map< GCITEMID, int >::iterator equipMatIter = mapEquipMatNeed.begin();
            for( ; equipMatIter != mapEquipMatNeed.end() ; ++equipMatIter ) { 
                std::map< GCITEMID, int >::iterator matIter = mapEquipMatHave.find( equipMatIter->first );
                if( matIter == mapEquipMatHave.end() ) {                // 보유 아이템 없음
                    bEquipMatEnable = false;
                    break;
                }
                else if ( equipMatIter->second > matIter->second ) {    // 갯수 모자람 
                    bEquipMatEnable = false;
                    break;
                }
                bEquipMatEnable = true;
            }

            // 글자색 
            if( bEquipMatEnable ) { 
                dwFontColor = 0xffffffff;
            }
            else { 
                dwFontColor = 0xff6f6f6f;
            }
        }
        else if( mapEquipMatNeed.empty() && false == mapCountMatNeed.empty() ) {        // 수량 재료 아이템만 필요한 경우 
            // 수량 아이템 조건 맞는지 체크 
            bool bCountMatEnable = false;
            std::map< GCITEMID, int >::iterator countMatIter = mapCountMatNeed.begin();
            for( ; countMatIter != mapCountMatNeed.end() ; ++countMatIter ) { 
                std::map< GCITEMID, int >::iterator matIter = mapCountMatHave.find( countMatIter->first );
                if( matIter == mapCountMatHave.end() ) {                    //  보유 아이템 없음 
                    bCountMatEnable = false;
                    break;
                }
                else if( countMatIter->second > matIter->second )  {        // 갯수 모자람 
                    bCountMatEnable = false;
                    break;
                }
                bCountMatEnable = true;
            }

            // 글자색 
            if( bCountMatEnable ) { 
                dwFontColor = 0xffffffff;
            }
            else { 
                dwFontColor = 0xff6f6f6f;
            }
        }
    }
    pBar->SetFontColorRecipeInfoBar( dwFontColor );
}

void KGCForgeItemCreation::SetInfobarList()
{
    const std::vector<RecipeSimpleInfo> vecInfoList = SiGCForgeManager()->GetShowRecipeList();

    if (vecInfoList.empty())
    {
        m_iSelectIndex = -1;
    }
    else
    {
        m_iSelectIndex = 0;
    }

    int iScrollMaxSize = (vecInfoList.size() - (m_iMaxRecipeViewCount)) + 1;// / mc_iMaxRecipeViewHeightCount;

    if (iScrollMaxSize < 0 || vecInfoList.empty())
    {
        iScrollMaxSize = 1;
    }

    if (m_iScrollpos > iScrollMaxSize)
    {
        m_iScrollpos = iScrollMaxSize;
    }

    m_pkScrollbar->SetScrollRangeMax(iScrollMaxSize);

    std::vector<RecipeSimpleInfo> vecPage;

    if ((int)vecInfoList.size() < m_iMaxRecipeViewCount)
    {
        vecPage = vecInfoList;
    }
    else
    {
        vecPage.reserve(m_iMaxRecipeViewCount);
        std::vector<RecipeSimpleInfo>::const_iterator begin = vecInfoList.begin();
        std::vector<RecipeSimpleInfo>::const_iterator end = vecInfoList.end();
        if (m_iScrollpos == 0)
        {
            std::copy(begin, begin + m_iMaxRecipeViewCount, std::back_inserter(vecPage));
        }
        else
        {
            if (m_iScrollpos + m_iMaxRecipeViewCount >= (int)vecInfoList.size())
            {
                std::copy(begin + m_iScrollpos, end, std::back_inserter(vecPage));
            }
            else
            {
                std::copy(begin + m_iScrollpos, begin + m_iScrollpos + m_iMaxRecipeViewCount, std::back_inserter(vecPage));
            }
        }
    }

    std::vector<KGCRecipeInfoBar*>::iterator vShowListIter = m_vecRecipeInfoBar.begin();

    for (; vShowListIter != m_vecRecipeInfoBar.end(); ++vShowListIter)
    {
        (*vShowListIter)->ToggleRender(false);
    }

    int iSize = vecPage.size();
    std::vector<RecipeSimpleInfo>::iterator vit = vecPage.begin();

    for (int i = 0; i < iSize; i++)
    {
        if (vit == vecPage.end())
        {
            m_vecRecipeInfoBar[i]->ToggleRender(false);
            continue;
        }

        GCItem* pItem = g_pItemMgr->GetItemData(vecPage[i].ItemID);
        SetInfobarString(pItem, m_vecRecipeInfoBar[i], vecPage[i]);
        SetInfobarStringColor(pItem, m_vecRecipeInfoBar[i], vecPage[i]);

        m_vecRecipeInfoBar[i]->ToggleRender(true);
        ++vit;
    }
}

void KGCForgeItemCreation::OnChangePage( const KActionEvent& event )
{
    SetInfobarList();
    OnInitInfobar();

}

void KGCForgeItemCreation::OnSelectSortBtnList( const KActionEvent& event )
{
    for(int i =0;i<GCForgeManager::ERSB_MAX;i++){
        if(m_saSortBtnList[i] == event.m_pWnd){
            SelectSortBtn(i);
            break;
        }
    }
}

void KGCForgeItemCreation::OnInitInfobar( void )
{
    for(std::vector< KGCRecipeInfoBar* >::iterator vit = m_vecRecipeInfoBar.begin(); vit != m_vecRecipeInfoBar.end(); vit++){
        (*vit)->LockAll(false);
    }
}

void KGCForgeItemCreation::OnSelectInfobar( const KActionEvent& event )
{
    if( g_pkUIScene->m_pkForgeDlg->GetForgeCraftState() == KGCForgeItemCraftMachine::EFMS_START || 
        g_pkUIScene->m_pkForgeDlg->GetForgeCraftState() == KGCForgeItemCraftMachine::EFMS_CONTINUE ) { 
            return;
    }

    for(std::vector< KGCRecipeInfoBar* >::iterator vit = m_vecRecipeInfoBar.begin(); vit != m_vecRecipeInfoBar.end(); vit++){
        if( (*vit) == event.m_pWnd){
            SelectInfoBar((*vit)->GetIndex(), (*vit)->GetItemID());
            (*vit)->LockAll(true);
        }
        else{
            (*vit)->LockAll(false);
        }
    }
}

void KGCForgeItemCreation::SelectInfoBar( unsigned short iIndex ,GCITEMID giItemID )
{
    SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_FORGE_CLICK_CREATION_ITEM_BAR,iIndex,giItemID));
}

void KGCForgeItemCreation::SelectSortBtn(int iIndex)
{
    for (int i = 0; i < GCForgeManager::ERSB_MAX; i++)
    {
        if (m_saDownSortList[i]->IsRenderOn() && iIndex == i)
        {
            m_saUpSortList[i]->ToggleRender(true);
            m_saDownSortList[i]->ToggleRender(false);
            SiGCForgeManager()->SetSort(i, true);
            SetInfobarList();
            OnInitInfobar();
        }
        else if (iIndex == i)
        {
            m_saUpSortList[i]->ToggleRender(false);
            m_saDownSortList[i]->ToggleRender(true);
            SiGCForgeManager()->SetSort(i, false);
            SetInfobarList();
            OnInitInfobar();
        }
        else
        {
            m_saUpSortList[i]->ToggleRender(false);
            m_saDownSortList[i]->ToggleRender(false);
        }
    }
}

void KGCForgeItemCreation::OnSelectSloatBtnList( const KActionEvent& event )
{
    for(int i =0;i<GCForgeManager::ESFB_MAX;i++)
    {
        if(m_saSloatFilterBtnList[i] == event.m_pWnd)
        {
            SelectSloatBtn(i);
            break;
        }
    }
}

void KGCForgeItemCreation::SelectSloatBtn( int iIndex )
{
    for(int i =0;i<GCForgeManager::ESFB_MAX;i++)
    {
        m_saSloatFilterBtnList[i]->ToggleRender(iIndex != i);
        m_saSloatFilterSelectedBtnList[i]->ToggleRender(iIndex == i);
    }
    SiGCForgeManager()->SetSloat(iIndex);
    SelectGradeBtn(-1);
}

void KGCForgeItemCreation::OnSelectGradeBtnList( const KActionEvent& event )
{
}

void KGCForgeItemCreation::SelectGradeBtn( int iIndex )
{
    SiGCForgeManager()->SetGrade(iIndex);

    SiGCForgeManager()->SetShowRecipeList();

    SetMaxPage();


}

void KGCForgeItemCreation::SetMaxPage( )
{
    SiGCForgeManager()->RefreshMaxPage(mc_iMaxRecipeViewHeightCount);
    SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_FORGE_CHANGE_PAGE));
}

void KGCForgeItemCreation::OnChangeCharacter( const KActionEvent& event )
{
    if(SiGCForgeManager()->IsEnableBreakUp() == true) return;
    SiGCForgeManager()->SetShowRecipeList();
    SetMaxPage();
}

void KGCForgeItemCreation::RefreshAllRecipe() {
    if (m_bCheckBox) return;
    POINT ptMouse = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos((float)ptMouse.x, (float)ptMouse.y);

    bool bCheck = false;

    for (int i = 0; i < m_iMaxRecipeViewCount; ++i)
    {
        if (m_vecRecipeInfoBar[i]->IsRenderOn() && m_vecRecipeInfoBar[i]->CheckPosInWindowBound(vMousePos))
        {
            int iRealIndex = m_iScrollpos + i;

            GCItem* pItem = g_pItemMgr->GetItemData(m_vecRecipeInfoBar[i]->GetItemID());
            if (NULL == pItem)
            {
                continue;
            }

            const std::vector<RecipeSimpleInfo>& vecShowRecipe = SiGCForgeManager()->GetShowRecipeList();

            if (iRealIndex > (int)vecShowRecipe.size())
            {
                continue;
            }

            g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation(pItem, -1, NULL, NULL, NULL, vecShowRecipe[iRealIndex].cGrade, vecShowRecipe[iRealIndex].iItemLevel);
            g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

            D3DXVECTOR2 vecPos(m_vecRecipeInfoBar[i]->GetCurrentWindowPos());
            vecPos.x += m_vecRecipeInfoBar[i]->GetWidth();
            vecPos.x *= m_fWindowScaleX;
            vecPos.y *= m_fWindowScaleY;

            g_pkUIScene->m_pkItemInformationBox->SetOriginPosition(vecPos);
            g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(KD3DWnd::D3DWZ_TOPMOST);

            bCheck = true;
            break;
        }
    }

    g_pkUIScene->m_pkItemInformationBox->ToggleRender(bCheck);
}

void KGCForgeItemCreation::FrameMoveInEnabledState( void )
{
    RefreshAllRecipe();
}

bool KGCForgeItemCreation::IsEnabledRecipeBar( GCITEMID itemID_ )
{
    bool bEnable = false;
    for( std::vector< KGCRecipeInfoBar* >::iterator it = m_vecRecipeInfoBar.begin() ; it != m_vecRecipeInfoBar.end() ; ++it ) { 
        if( (*it)->GetItemID() == itemID_ && (*it)->IsEnable() ) {
            bEnable = true;
            break;
        }
    }

    return bEnable;
}
