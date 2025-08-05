#include "stdafx.h"
#include "GCForgeItemBreakUp.h"

IMPLEMENT_CLASSNAME( KGCForgeItemBreakUp );
IMPLEMENT_WND_FACTORY( KGCForgeItemBreakUp );
IMPLEMENT_WND_FACTORY_NAME( KGCForgeItemBreakUp, "gc_forge_item_breakup_dlg" );

KGCForgeItemBreakUp::KGCForgeItemBreakUp( void )
: m_pkItemImgEdge(NULL)
, m_bCheckBox(false)
,m_pkScrollbar(NULL)
{
    LINK_CONTROL( "scroll", m_pkScrollbar );
    m_iScrollpos = 0;
    m_vecInvenItemImgList.clear();
    m_vecItemUIDList.clear();   
}

KGCForgeItemBreakUp::~KGCForgeItemBreakUp( void )
{
}

void KGCForgeItemBreakUp::ActionPerformed( const KActionEvent& event )
{
    GCWND_MAP(m_pkScrollbar, OnScrollPos);
}

void KGCForgeItemBreakUp::OnCreate( void )
{
    OnCreateItemBackList();
    OnInitItemBackList();

    OnCreateItemImgList();
    OnInitItemImgList();

    SiGCForgeManager()->MakeAllBreakUpItemList();
    RefreshItemList();
    OnCreateItemImgEdge();
    
    m_pkScrollbar->InitState(true,true,this);
    m_iScrollpos = 0;
    m_pkScrollbar->InitScroll( 0, 5, 0, 1, 1, -2, 1, true );
    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_FORGE_SEL_CHARACTER, this, &KGCForgeItemBreakUp::RefreshItemList );
    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_FORGE_CHANGE_PAGE, this, &KGCForgeItemBreakUp::RefreshPage );
    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_FORGE_BOUND_ITEM, this, &KGCForgeItemBreakUp::OnBoundItem );

    g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( NULL );
}


void KGCForgeItemBreakUp::OnBoundItem( const KActionEvent& event )
{
    m_bCheckBox = event.m_dwlParam != 0;
}

void KGCForgeItemBreakUp::OnScrollPos( void )
{
    const std::vector<GCITEMUID>& vecTemp =  SiGCForgeManager()->GetShowBreakUpItemList();

    int iIndex = m_pkScrollbar->GetScrollPos();
    int iMaxSize = vecTemp.size();
    int iScrollSize = (vecTemp.size());
    std::vector<std::pair<GCITEMUID,KGCItemImgWnd*>>::iterator vItImg = m_vecItemImgList.begin(); 
    m_pkScrollbar->SetScrollPageSize( 1 );
    m_pkScrollbar->SetScrollRangeMin( 0 );

    if (vecTemp.size() <= EM_INVEN_HEIGHT_SIZE)
        iScrollSize = 0;
    if (vecTemp.size() % EM_INVEN_HEIGHT_SIZE > 0)
        iScrollSize += 5;
    if (iScrollSize >= EM_INVEN_HEIGHT_SIZE)
        iScrollSize -= EM_INVEN_HEIGHT_SIZE;
    m_pkScrollbar->SetScrollRangeMax(iScrollSize);

    while(vItImg != m_vecItemImgList.end())
    {
        KItem* pInven = NULL;
        if(iIndex < iMaxSize ){
            if( g_pItemMgr->FindInventory( vecTemp[iIndex], &pInven) && pInven ){
                vItImg->first = pInven->m_ItemUID;
                vItImg->second->SetItemInfo(pInven->m_ItemID,-1,false,-1,pInven->m_ItemUID);
                vItImg->second->ToggleRender( true );
                ++iIndex;
            }
        }  
        else
        {
            vItImg->second->ToggleRender( false );
        }
        vItImg++;
    }
}

void KGCForgeItemBreakUp::UpdateInvenItemList()
{
}

void KGCForgeItemBreakUp::RefreshItemList( void )
{
    if(SiGCForgeManager()->IsEnableBreakUp() == false ) return;
    SiGCForgeManager()->SetShowBreakUpItemList(m_iMAX_ITEM_VIEW_HEGHIT_COUNT*m_iMAX_ITEM_VIEW_WIDTH_COUNT);
    SpeakToActionListener( KActionEvent( NULL, KGCUIScene::D3DWE_FORGE_CHANGE_PAGE ));
}

void KGCForgeItemBreakUp::RefreshPage( void )
{
  const std::vector<GCITEMUID>& vecTemp =  SiGCForgeManager()->GetShowBreakUpItemList();

    int iIndex = m_pkScrollbar->GetScrollPos();
    int iMaxSize = vecTemp.size();
    int iScrollSize = (vecTemp.size());
    std::vector<std::pair<GCITEMUID,KGCItemImgWnd*>>::iterator vItImg = m_vecItemImgList.begin(); 
    m_pkScrollbar->SetScrollPageSize( 1 );
    m_pkScrollbar->SetScrollRangeMin( 0 );

    if (vecTemp.size() <= EM_INVEN_HEIGHT_SIZE)
        iScrollSize = 0;
    if (vecTemp.size() % EM_INVEN_HEIGHT_SIZE > 0)
        iScrollSize += 5;
    if (iScrollSize >= EM_INVEN_HEIGHT_SIZE)
        iScrollSize -= EM_INVEN_HEIGHT_SIZE;
    m_pkScrollbar->SetScrollRangeMax(iScrollSize);

    while(vItImg != m_vecItemImgList.end())
    {
        KItem* pInven = NULL;
        if(iIndex < iMaxSize ){
            if( g_pItemMgr->FindInventory( vecTemp[iIndex], &pInven) && pInven ){
                vItImg->first = pInven->m_ItemUID;
                vItImg->second->SetItemInfo(pInven->m_ItemID,-1,false,-1,pInven->m_ItemUID);
                vItImg->second->ToggleRender( true );
                ++iIndex;
            }
        }  
        else
        {
            vItImg->second->ToggleRender( false );
        }
        vItImg++;
    }
}

void KGCForgeItemBreakUp::OnCreateItemBackList( void )
{
    if(m_vecItemImgBackList.empty() == false) return;

    m_vecItemImgBackList = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_new_forge.stg", "","new_forge_dlg\\forge_item_breakup_dlg\\box_item\\item_frame",true,m_iMAX_ITEM_VIEW_HEGHIT_COUNT * m_iMAX_ITEM_VIEW_WIDTH_COUNT );
}

void KGCForgeItemBreakUp::OnInitItemBackList( void )
{
    for(int i = 0 ; i < m_iMAX_ITEM_VIEW_HEGHIT_COUNT ; i++){
        for(int j = 0; j < m_iMAX_ITEM_VIEW_WIDTH_COUNT ; j++){
            int iIndex = i * m_iMAX_ITEM_VIEW_WIDTH_COUNT + j;
            float x = static_cast<float>(j * 50 * GC_SCREEN_DIV_WIDTH + 27 * GC_SCREEN_DIV_WIDTH);
            float y = static_cast<float>(i * 50 * GC_SCREEN_DIV_WIDTH + 65 * GC_SCREEN_DIV_WIDTH);
            D3DXVECTOR2 vNewPos(x,y);
            m_vecItemImgBackList[iIndex]->InitState(true, false,NULL);
            m_vecItemImgBackList[iIndex]->SetWindowPos( vNewPos );
            m_vecItemImgBackList[iIndex]->SetWidthDirect( GC_SCREEN_DIV_SIZE_INT(49) );
            m_vecItemImgBackList[iIndex]->SetHeightDirect( GC_SCREEN_DIV_SIZE_INT(49) );
        }
    }
}

void KGCForgeItemBreakUp::OnCreateItemImgList( void )
{
    if(m_vecItemImgList.empty() == false) return;
    std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_forge.stg", "","forge_dlg\\forge_item_breakup_dlg\\inven_item_img_view_temp",true,m_iMAX_ITEM_VIEW_HEGHIT_COUNT * m_iMAX_ITEM_VIEW_WIDTH_COUNT );

    for(std::vector<KD3DWnd*>::iterator vIt = vecTemp.begin(); vIt != vecTemp.end();vIt++){
        m_vecItemImgList.push_back(std::pair<GCITEMID,KGCItemImgWnd*>(0,static_cast<KGCItemImgWnd*>(*vIt)));
    }

}

void KGCForgeItemBreakUp::OnInitItemImgList( void )
{
    for(int i = 0 ; i < m_iMAX_ITEM_VIEW_HEGHIT_COUNT ; i++){
        for(int j = 0; j < m_iMAX_ITEM_VIEW_WIDTH_COUNT ; j++){
            int iIndex = i * m_iMAX_ITEM_VIEW_WIDTH_COUNT + j;
            float x = static_cast<float>(j * 50 * GC_SCREEN_DIV_WIDTH + 27 * GC_SCREEN_DIV_WIDTH);
            float y = static_cast<float>(i * 50 * GC_SCREEN_DIV_WIDTH + 65 * GC_SCREEN_DIV_WIDTH);
            D3DXVECTOR2 vNewPos(x,y);
            m_vecItemImgList[iIndex].second->InitState(false, true,this);
            m_vecItemImgList[iIndex].second->SetWindowPos( vNewPos );
            m_vecItemImgList[iIndex].second->SetWidthDirect( GC_SCREEN_DIV_SIZE_INT(49) );
            m_vecItemImgList[iIndex].second->SetHeightDirect( GC_SCREEN_DIV_SIZE_INT(49) );
            m_vecItemImgList[iIndex].second->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCForgeItemBreakUp::OnClickItem);
        }
    }
}

void KGCForgeItemBreakUp::OnCreateItemImgEdge( void )
{
    if(m_pkItemImgEdge != NULL) return;
    std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_forge.stg", "","forge_dlg\\forge_item_breakup_dlg\\item_img_edge",true );
    if(vecTemp.begin() != vecTemp.end()){
        m_pkItemImgEdge = *vecTemp.begin();
        m_pkItemImgEdge->InitState(false,false,NULL);
        m_pkItemImgEdge->SetWidthDirect( GC_SCREEN_DIV_SIZE_INT(50) );
        m_pkItemImgEdge->SetHeightDirect( GC_SCREEN_DIV_SIZE_INT(50) );
    }
}

void KGCForgeItemBreakUp::OnClickItem( const KActionEvent& event )
{
    for(std::vector<std::pair<GCITEMUID,KGCItemImgWnd*>>::iterator vIt = m_vecItemImgList.begin(); vIt != m_vecItemImgList.end(); vIt++){
        if(vIt->second == event.m_pWnd)
        {
            GCITEMUID giuItemUID = vIt->first;
            vIt->first = 0;
            ClickItem(giuItemUID, vIt->second);
            break;
        }
    }
}

void KGCForgeItemBreakUp::ClickItem( GCITEMUID giItem, KGCItemImgWnd* pWnd )
{
    SiGCForgeManager()->AddWaitItem(giItem, pWnd->GetItemID());
    RefreshItemList();
    SpeakToActionListener( KActionEvent( NULL, KGCUIScene::D3DWE_FORGE_CLICK_BREAKEUP_ITEM,giItem, pWnd->GetItemID() ));
}

void KGCForgeItemBreakUp::FrameMoveInEnabledState( void )
{
    if(m_bCheckBox) return;

    bool bCheck = false;
    for(std::vector<std::pair<GCITEMUID,KGCItemImgWnd*>>::iterator vIt = m_vecItemImgList.begin(); vIt != m_vecItemImgList.end(); vIt++){
        if(vIt->second->CheckMousePosInWindowBound())
        {
            D3DXVECTOR2 vec = vIt->second->GetCurrentWindowLocalPos();
            m_pkItemImgEdge->SetWindowPos(vec);

            KItem* pInventory = NULL;
            GCItem* pItem = NULL;

            if( false == g_pItemMgr->FindInventory(vIt->first, &pInventory, &pItem) || NULL == pInventory || NULL == pItem || vIt->first == 0 )
                continue;

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
    
     if( g_pkInput->ISWheelUp() )
    {
        if( m_pkScrollbar->GetScrollPos() > 0 )
        {
            m_pkScrollbar->SetScrollPos(m_pkScrollbar->GetScrollPos());
             OnScrollPos();
            g_KDSound.Play( "73" );
        }
    }
    else if( g_pkInput->ISWheelDown() )
    {
        if( m_pkScrollbar->GetScrollPos() < m_pkScrollbar->GetScrollRangeMax() )
        {
            m_pkScrollbar->SetScrollPos(m_pkScrollbar->GetScrollPos());
             OnScrollPos();
            g_KDSound.Play( "73" );
        }
    }	
    m_pkItemImgEdge->ToggleRender( bCheck );
    g_pkUIScene->m_pkItemInformationBox->ToggleRender( bCheck );

}
	