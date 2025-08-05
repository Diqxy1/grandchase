#include "stdafx.h"
#include "GCForgeItemCraftMachine.h"
#include <string>

IMPLEMENT_CLASSNAME( KGCForgeItemCraftMachine );
IMPLEMENT_WND_FACTORY( KGCForgeItemCraftMachine );
IMPLEMENT_WND_FACTORY_NAME( KGCForgeItemCraftMachine, "gc_forge_item_crafting_dlg" );

KGCForgeItemCraftMachine::KGCForgeItemCraftMachine( void )
: m_iForgeState(0)
, m_pkGPTitle(NULL)
, m_pkCostTitle(NULL)
, m_pkGP(NULL)
, m_pkCost(NULL)
, m_pPrekResultItem(NULL)
, m_pkStartBtn(NULL)
, m_pkStopBtn(NULL)
, m_pkStaticPreResultItemNum(NULL)
, m_pkBtnBack(NULL)
, m_pkCountMinBtn(NULL)
, m_pkCountMaxBtn(NULL)
, m_pkCountPreBtn(NULL)
, m_pkCountNextBtn(NULL)
, m_iCreateCount(1)
, m_pkCraftMachineObject(NULL)
, m_iMachineState(EFMS_NORMAL)
, m_iMachineDelayCnt(0)
, m_fMachineSpeed(0.0f)
, m_pkGaugeCenter(NULL)
, m_pkGaugeRight(NULL)
{
    LINK_CONTROL("static_gp_title",m_pkGPTitle);
    LINK_CONTROL("static_cost_title",m_pkCostTitle);
    LINK_CONTROL("static_gp",m_pkGP);
    LINK_CONTROL("static_cost",m_pkCost);
    LINK_CONTROL("start_btn",m_pkStartBtn);
    LINK_CONTROL("stop_btn",m_pkStopBtn);
    for(int i = 0; i < 5; i++){
        char strTemp[ MAX_PATH ] = "";
        m_pkItemBack[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "create_item_back%d", i);
        LINK_CONTROL(strTemp,m_pkItemBack[i]);
    }

    for(int i = 0; i < 3; i++){
        char strTemp[ MAX_PATH ] = "";
        m_pkItemBack[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "state%d", i);
        LINK_CONTROL(strTemp,m_pkState[i]);
    }
    
    LINK_CONTROL("item_count_min",m_pkCountMinBtn);
    LINK_CONTROL("item_count_max",m_pkCountMaxBtn);
    LINK_CONTROL("item_count_del",m_pkCountPreBtn);
    LINK_CONTROL("item_count_add",m_pkCountNextBtn);

    m_bIsRotation[0] = true;
    m_bIsRotation[1] = true;
    m_bIsRotation[2] = true;
    m_bIsRotation[3] = false;
    m_bIsRotation[4] = false;
    m_bIsRotation[5] = false;
    m_bIsRotation[6] = false;

    m_bIsMove[0] = false;
    m_bIsMove[1] = false;
    m_bIsMove[2] = false;
    m_bIsMove[3] = false;
    m_bIsMove[4] = true;
    m_bIsMove[5] = true;
    m_bIsMove[6] = true;

    m_fMoveORROtationValue[0] = 0.02f;
    m_fMoveORROtationValue[1] = -0.03f;
    m_fMoveORROtationValue[2] = 0.03f;
    m_fMoveORROtationValue[3] = 0.0f;
    m_fMoveORROtationValue[4] = 1.0f;
    m_fMoveORROtationValue[5] = -1.0f;
    m_fMoveORROtationValue[6] = 1.0f;

    m_fPosXValue[0] = 0.0f* GC_SCREEN_DIV_WIDTH;
    m_fPosXValue[1] = 0.0f* GC_SCREEN_DIV_WIDTH;
    m_fPosXValue[2] = 0.0f* GC_SCREEN_DIV_WIDTH;
    m_fPosXValue[3] = 0.0f* GC_SCREEN_DIV_WIDTH;
    m_fPosXValue[4] = 27.0f* GC_SCREEN_DIV_WIDTH;
    m_fPosXValue[5] = 58.0f* GC_SCREEN_DIV_WIDTH;
    m_fPosXValue[6] = 88.0f* GC_SCREEN_DIV_WIDTH;
    m_vecMatieralUIDList.clear();
    m_vecBoxImg.reserve( mc_iMaxRecipeViewHeightCount );
}

KGCForgeItemCraftMachine::~KGCForgeItemCraftMachine( void )
{
}

void KGCForgeItemCraftMachine::ActionPerformed( const KActionEvent& event )
{

}


void KGCForgeItemCraftMachine::OnCreate( void )
{
    OnCreateItemBackList();
    OnInitItemBackList();
    OnCreateItemList();
    InitItemList();

  //  OnCreateCraftMachine();
  
    m_pkCostTitle->SetText(g_pkStrLoader->GetString( STR_ID_ITEM_CREATION0 ));
    m_pkCostTitle->SetAlign(DT_RIGHT);

    m_pkGPTitle->SetText("GP");
    m_pkGPTitle->SetAlign(DT_RIGHT);


    SetUserGPText();

    m_pkStartBtn->InitState(true,true,this);
    m_pkStopBtn->InitState(true,true,this);

    for(int i = 0; i < 5; i++){
       m_pkItemBack[i]->InitState(false,true,this);
    }

    m_pkCountMinBtn->InitState(false,true,this);
    m_pkCountMaxBtn->InitState(false,true,this);
    m_pkCountPreBtn->InitState(false,true,this);
    m_pkCountNextBtn->InitState(false,true,this);

    SetItemHeaterAndBtnSetting();

    for ( int i = 0; i < 3; ++i ) 
	{
		m_pkState[i]->ToggleRender( false );
	}


    m_pkCountMinBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCForgeItemCraftMachine::OnClickCountBtn);
    m_pkCountMaxBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCForgeItemCraftMachine::OnClickCountBtn);
    m_pkCountPreBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCForgeItemCraftMachine::OnClickCountBtn);
    m_pkCountNextBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCForgeItemCraftMachine::OnClickCountBtn);


    m_pkStartBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCForgeItemCraftMachine::OnClickStartBtn);
    m_pkStopBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCForgeItemCraftMachine::OnClickStopBtn);

    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_FORGE_CLICK_BREAKEUP_ITEM, this, &KGCForgeItemCraftMachine::OnClickBreakUpItem );

    
    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_FORGE_CLICK_CREATION_ITEM_BAR, this, &KGCForgeItemCraftMachine::OnClickCreationItem );

    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_FORGE_CHANGE_PAGE, this, &KGCForgeItemCraftMachine::ChangeCharacter );
    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_FORGE_COMPLETE_RESULT_ITEM, this, &KGCForgeItemCraftMachine::OnCompleteItem );
 
    RefreshGauge();
}

void KGCForgeItemCraftMachine::CreateCraftMachine2DObject( std::string strFileName, float sizeX, float sizeY )
{
    if(m_pkCraftMachineObject == NULL) return;

    KGC2DObject *pTemp = m_pkCraftMachineObject->Create2DObject();

    pTemp->AddTexture(g_pGCDeviceManager2->CreateTexture(strFileName.c_str()));
    pTemp->SetAxisOffset( -0.5f,-0.5f ,0.0f);
    pTemp->SetSize(sizeX * GC_SCREEN_DIV_WIDTH,sizeY * GC_SCREEN_DIV_WIDTH);
    pTemp->CameraMatrixOn();
}


void KGCForgeItemCraftMachine::ToggleStateImage( int nStateType )
{

	for ( int i = 0; i < 3; ++i ) 
	{
        if ( nStateType == -1 )
        {
            m_pkState[i]->ToggleRender( false );
        }
        else
        {
            if ( i == nStateType ) 
			    m_pkState[i]->ToggleRender( true );
		    else
			    m_pkState[i]->ToggleRender( false );
        }
	}
}


void KGCForgeItemCraftMachine::OnCreateCraftMachine( void )
{
    if(m_pkCraftMachineObject != NULL) return;

    m_pkCraftMachineObject = (KGCObjectUI*)g_pkUIMgr->CreateUI( "gc_kgcobjectui" );
    m_pkCraftMachineObject->SetWindowNamePtr( "craft_machine_object" );

    CreateCraftMachine2DObject("Item_Break_M_04.dds",128.0f,128.0f);
    CreateCraftMachine2DObject("Item_Break_M_02.dds",256.0f,256.0f);
    CreateCraftMachine2DObject("Item_Break_M_03.dds",128.0f,128.0f);
    CreateCraftMachine2DObject("Item_Break_M_01.dds",256.0f,256.0f);
    CreateCraftMachine2DObject("Item_Break_M_05.dds",64.0f,64.0f);
    CreateCraftMachine2DObject("Item_Break_M_05.dds",64.0f,64.0f);
    CreateCraftMachine2DObject("Item_Break_M_05.dds",64.0f,64.0f);

    float pos[7][2] = {
        { 121.0f * GC_SCREEN_DIV_WIDTH, -23.0f * GC_SCREEN_DIV_WIDTH },
        { -50.0f * GC_SCREEN_DIV_WIDTH, 60.0f * GC_SCREEN_DIV_WIDTH },
        { -115.0f * GC_SCREEN_DIV_WIDTH, 30.0f * GC_SCREEN_DIV_WIDTH },
        { 0.0f * GC_SCREEN_DIV_WIDTH, 0.0f * GC_SCREEN_DIV_WIDTH },
        { 27.0f * GC_SCREEN_DIV_WIDTH, 73.0f * GC_SCREEN_DIV_WIDTH },
        { 58.0f * GC_SCREEN_DIV_WIDTH, 73.0f * GC_SCREEN_DIV_WIDTH },
        { 88.0f * GC_SCREEN_DIV_WIDTH, 73.0f * GC_SCREEN_DIV_WIDTH }
    };

    for(int i = 0;true;i++){
        KGC2DObject *pTemp = m_pkCraftMachineObject->Get2DObject(i);
        if(pTemp == NULL) break;
        pTemp->SetPositionPixel(static_cast<float>(m_iMACHINE_BASE_POS_X) + pos[i][0],static_cast<float>(m_iMACHINE_BASE_POS_Y) +pos[i][1] );
    }
    m_pkCraftMachineObject->ToggleRender(false);
    
//    m_pkCraftMachineObject->setpos
}

void KGCForgeItemCraftMachine::OnClickCountBtn( const KActionEvent& event )
{
    if(event.m_pWnd == m_pkCountMinBtn){
        ClickCountBtn(-2);
    }
    else if(event.m_pWnd == m_pkCountPreBtn){
        ClickCountBtn(-1);
    }
    else if(event.m_pWnd == m_pkCountNextBtn){
        ClickCountBtn(1);
    }
    else if(event.m_pWnd == m_pkCountMaxBtn){
        ClickCountBtn(2);
    }
}

void KGCForgeItemCraftMachine::ClickCountBtn( int iIncCount )
{
    int iMax = SiGCForgeManager()->GetMaxCreateCount(m_kCreateItemInfo.usIndex, m_kCreateItemInfo.ItemID);

    if(iIncCount == -2){
        m_iCreateCount = 1;
    }
    else if(iIncCount == 2){
        m_iCreateCount = iMax;
    }
    else
    {
        if(iIncCount + m_iCreateCount < 1)
            m_iCreateCount = 1;
        else if(iIncCount + m_iCreateCount > iMax)
            m_iCreateCount = iMax;
        else
            m_iCreateCount += iIncCount;
    }
    std::vector<SMaterialInfo> vInfo;
    SiGCForgeManager()->GetMaterialList(m_kCreateItemInfo.usIndex, m_kCreateItemInfo.ItemID, vInfo);
    SetCreationItem(vInfo, m_kCreateItemInfo.ItemID, m_kCreateItemInfo.usIndex);


}

void KGCForgeItemCraftMachine::SetItemHeaterAndBtnSetting( )
{
    GCItem *pItem =  g_pItemMgr->GetItemData(m_kCreateItemInfo.ItemID);
    bool bPeriod = true;
    int j = 0;
    if(pItem != NULL)
        bPeriod = pItem->eItemType == 1;

    bool bCanIncCount = (m_iForgeState == KGCForgeDlg::EKGCFOGE_CREATE) && m_kCreateItemInfo.ItemID != 0; 
  
   // m_pkBtnBack->ToggleRender(bCanIncCount);
   if(m_iForgeState != KGCForgeDlg::EKGCFOGE_CREATE || m_kCreateItemInfo.ItemID == 0)
   {
       m_pkItemBack[0]->ToggleRender(true);
   }

   for (int i = 0; i < 5; i++) {
       m_pkItemBack[i]->ToggleRender(pItem->cItemGrade == i);
   }
 
    m_pkCountMinBtn->ToggleRender(bCanIncCount);
    m_pkCountMaxBtn->ToggleRender(bCanIncCount);
    m_pkCountPreBtn->ToggleRender(bCanIncCount);
    m_pkCountNextBtn->ToggleRender(bCanIncCount);

}

void KGCForgeItemCraftMachine::OnCompleteItem( const KActionEvent& event )
{
    const std::vector<KItem>& vecResultItem =  SiGCForgeManager()->GetResultItem();

    int size = vecResultItem.size();
    int i = 0;
    std::vector<std::pair<GCITEMUID,KGCItemImgWnd*>>::iterator vIt = m_vecResultItemList.begin();
    std::vector<KD3DStatic*>::iterator vItStatic = m_vecStaticResultItemNumList.begin();

    for(;vIt != m_vecResultItemList.end();vIt++, vItStatic++){
        if(i < size){
            vIt->second->SetItemInfo(vecResultItem[i].m_ItemID);
            vIt->first = vecResultItem[i].m_ItemUID;
            vIt->second->ToggleRender(true);
            if(vecResultItem[i].m_nCount > 0){
                (*vItStatic)->SetNumber(vecResultItem[i].m_nCount);
                (*vItStatic)->ToggleRender(true);
                (*vItStatic)->SetFontSize(5);
            }
            else{
                (*vItStatic)->ToggleRender(false);
            }
        }
        else{
            vIt->first = 0;
            vIt->second->ToggleRender(false);
            (*vItStatic)->ToggleRender(false);
        }
        i++;
    }
    ToggleStateImage(2);
    SetUserGPText();
}


void KGCForgeItemCraftMachine::OnClickStartBtn( const KActionEvent& event )
{
    m_bBypassForgeVerify = false;

    if (m_iForgeState == KGCForgeDlg::EKGCFOGE_CREATE)
    {
        MakeMaterialUIDList();

        for (auto& element : m_vecMatieralUIDList)
        {
            KItem* tempItemForge = g_pItemMgr->m_kInventory.FindItemByItemUID(element);
            if (tempItemForge->m_cEnchantLevel > 0)
            {
                m_bBypassForgeVerify = true;
                break;
            }
            if (!tempItemForge->m_vecSocket.empty())
            {
                for (auto& socket : tempItemForge->m_vecSocket)
                {
                    if (socket.m_cState == KSocketInfo::STS_USING)
                    {
                        m_bBypassForgeVerify = true;
                        break;
                    }
                }
            }
        }

        if (m_bBypassForgeVerify)
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

void KGCForgeItemCraftMachine::OnClickStopBtn( const KActionEvent& event )
{
    m_pkStopBtn->Lock(true);
    m_pkStartBtn->Lock(false);

    m_iMachineState = EFMS_STOP;
}

void KGCForgeItemCraftMachine::OnClickStartBtnCreate( void )
{
    if (m_iMachineState == EFMS_NORMAL)
    {
        m_iMachineState = EFMS_START;
        m_pkStartBtn->Lock(true);
        m_pkStopBtn->Lock(false);
        g_KDSound.Play("Machine_Cylinder");
        ToggleStateImage(0);
    }
    else if (m_iMachineState == EFMS_CONTINUE)
    {
        m_bBypassForgeVerify = false;

        SiGCForgeManager()->CreateItem(m_kCreateItemInfo, m_iCreateCount, m_vecMatieralUIDList);

        std::vector<SMaterialInfo> vInfo;
        SiGCForgeManager()->GetMaterialList(m_kCreateItemInfo.usIndex, m_kCreateItemInfo.ItemID, vInfo);

        m_pkStartBtn->Lock(false);
        m_pkStopBtn->Lock(true);

        SetCreationItem(vInfo, m_kCreateItemInfo.ItemID, m_kCreateItemInfo.usIndex);
    }
}

void KGCForgeItemCraftMachine::OnClickStartBtnBreakUp( void )
{
    if (m_iMachineState == EFMS_NORMAL)
    {
        m_iMachineState = EFMS_START;
        m_pkStartBtn->Lock(true);
        m_pkStopBtn->Lock(false);
        ToggleStateImage(1);
        return;
    }

    std::vector<std::pair<GCITEMUID, GCITEMID>>& vInfo = SiGCForgeManager()->GetWaitItemList();
    std::vector<std::pair<GCITEMUID, GCITEMID>>::iterator vit = vInfo.begin();

    if (vit == vInfo.end())
    {
        m_iMachineState = EFMS_NORMAL;
        m_pkStartBtn->Lock(true);
        m_pkStopBtn->Lock(true);
        ToggleStateImage(2);
    }
    else if (m_iMachineState == EFMS_CONTINUE)
    {
        m_pPrekResultItem->SetItemInfo(vit->second);
        m_pPrekResultItem->ToggleRender(true);
        if (SiGCForgeManager()->BreakUpItem(vit->first))
        {
            g_KDSound.Play("Machine_Cylinder");
        }

        SiGCForgeManager()->MakeAllBreakUpItemList();
        SpeakToActionListener(KActionEvent(NULL, KGCUIScene::D3DWE_FORGE_SEL_CHARACTER));
        SetBreakItemList();

        if (vInfo.begin() != vInfo.end())
        {
            m_iMachineState = EFMS_START;
            m_pkStartBtn->Lock(true);
            m_pkStopBtn->Lock(false);
        }
        else
        {
            ToggleStateImage(2);
            m_pPrekResultItem->ToggleRender(false);
            m_iMachineState = EFMS_START;
            m_pkStartBtn->Lock(true);
            m_pkStopBtn->Lock(true);
        }
    }
}

void KGCForgeItemCraftMachine::SetCreationItem(std::vector<SMaterialInfo> vInfo, GCITEMID giItemID, int iIndex)
{
    int size = vInfo.size();
    int i = 0;

    //제작해지에서는 세트 아이템은 활용하지 못하게 되어 있음..
    m_pPrekResultItem->SetItemInfo(giItemID);
    m_pPrekResultItem->ToggleRender(true);

    m_pkCost->SetNumber(0);
    m_pkCost->SetAlign(DT_RIGHT);
    m_pkStartBtn->Lock(false);

    const std::vector<RecipeSimpleInfo>& vecInfoList = SiGCForgeManager()->GetShowRecipeList();

    for(std::vector<RecipeSimpleInfo>::const_iterator cvIt = vecInfoList.begin();cvIt!=vecInfoList.end();cvIt++){
        if(cvIt->ItemID == giItemID && static_cast<int>(cvIt->usIndex) == iIndex){
            m_pkCost->SetNumber(cvIt->iPrice * m_iCreateCount);
            m_pkCost->SetAlign(DT_RIGHT);
            SetUserGPText( cvIt->iPrice * m_iCreateCount );

            m_kCreateItemInfo = *cvIt;
            m_pkStaticPreResultItemNum->SetNumber(m_kCreateItemInfo.cFactor* m_iCreateCount);
            GCItem *pItem =  g_pItemMgr->GetItemData(m_kCreateItemInfo.ItemID);
            bool bPeriod = true;
            if(pItem != NULL)
                bPeriod = pItem->eItemType == 1;

            if( m_kCreateItemInfo.cFactor <= 0 || bPeriod == false ) m_pkStaticPreResultItemNum->ToggleRender(false);
            else m_pkStaticPreResultItemNum->ToggleRender(true);
            break;
        }
    }

    std::vector<std::pair<GCITEMUID,KGCItemImgWnd* >>::iterator vIt = m_vecItemList.begin();
    std::vector<std::pair<GCITEMUID,KD3DWnd*>>::iterator vitSelect = m_vecItemSelectBtnList.begin();
    std::vector<KD3DStatic*>::iterator vItStatic = m_vecStaticItemNumList.begin();

    for(; vIt != m_vecItemList.end();vIt++, vItStatic++, ++vitSelect){
        vIt->first = 0;
        if( i < size ){

            GCITEMUID itemUID = 0;
            if( GetNotCoordiItemUID( vInfo[i].m_ItemID, itemUID ) ) { 
                vIt->first = itemUID;
                vitSelect->first = itemUID;
            }

            DWORD fontColor = 0;
            DWORD imgColor = 0;

            if(SiGCForgeManager()->GetMaterialColorByUID( itemUID, vInfo[i].m_nDuration * m_iCreateCount, imgColor, fontColor ) == false){
                m_pkStartBtn->Lock(true);
                vitSelect->second->ToggleRender( false );
            }
            else { 
                if( vInfo[i].m_nDuration == -1 && vitSelect->first != 0  ) { 
                    GCItem* pItemData = g_pItemMgr->GetItemData( vInfo[i].m_ItemID );
                    if( pItemData->eItemKind != GIK_ENCHANT_ENABLE_ITEM ) {
                        vitSelect->second->ToggleRender( true );
                    }
                    else
                    {
                        vitSelect->second->ToggleRender( false );
                    }
                }
                else { 
                    vitSelect->second->ToggleRender( false );
                }
            }

            if( false == g_pkUIScene->m_pkForgeDlg->IsEnabledRecipeBar( giItemID ) ) { 
                m_pkStartBtn->Lock(true);
            }

            vIt->second->SetItemInfo(vInfo[i].m_ItemID,imgColor);
            vIt->second->ToggleRender(true);
            int Duration = g_MyD3D->m_kItemMgr.GetInventoryItemDuration(vInfo[i].m_ItemID);
            std::wostringstream strmDurations;
 	        strmDurations << Duration << "/" << vInfo[i].m_nDuration *m_iCreateCount;
	           
                
            if(vInfo[i].m_nDuration >= 0){
                (*vItStatic)->SetText(strmDurations.str());
                (*vItStatic)->SetFontColor(fontColor);
                (*vItStatic)->SetFontSize(5);
                (*vItStatic)->ToggleRender(true);
            }
            else{
                (*vItStatic)->ToggleRender(false);
            }
        }
        else{
            vIt->second->ToggleRender(false);
            (*vItStatic)->ToggleRender(false);
            vitSelect->second->ToggleRender( false );
        }
        ++i;
    }
}

void KGCForgeItemCraftMachine::OnClickCreationItem( const KActionEvent& event )
{
    m_iCreateCount = 1;
    int iIndex = static_cast<int>(event.m_dwlParam);
    GCITEMID giItemID = static_cast<GCITEMID>(event.m_dwlParam2);
    std::vector<SMaterialInfo> vInfo;
    if(SiGCForgeManager()->GetMaterialList(iIndex,giItemID, vInfo) == false) return;

    m_kCreateItemInfo.Clear();
    SetCreationItem(vInfo, giItemID, iIndex);
    ClearResultItemList();
    SetItemHeaterAndBtnSetting();

}


void KGCForgeItemCraftMachine::OnClickBreakUpItem( const KActionEvent& event )
{
    SetBreakItemList();
    ClearResultItemList();
    SetItemHeaterAndBtnSetting();
}

void KGCForgeItemCraftMachine::OnClickWaitBreakUpItem( const KActionEvent& event )
{
    if(m_iForgeState == KGCForgeDlg::EKGCFOGE_CREATE) return;
    if(m_iMachineState != EFMS_NORMAL ) return;
    GCITEMUID guItemUID = 0;


    for(int i = 0;i<static_cast<int>(m_vecItemList.size());i++){
        if(m_vecItemList[i].second == event.m_pWnd){
            guItemUID = m_vecItemList[i].first;
            break;
        }
    }
    SiGCForgeManager()->EraseWaitItem(guItemUID);
    SpeakToActionListener( KActionEvent( NULL, KGCUIScene::D3DWE_FORGE_SEL_CHARACTER));
    SetBreakItemList();
    ClearResultItemList();
    SetItemHeaterAndBtnSetting();
}



void KGCForgeItemCraftMachine::SetBreakItemList( )
{
    std::vector<std::pair<GCITEMUID, GCITEMID>>& vInfo = SiGCForgeManager()->GetWaitItemList();
    int size = vInfo.size();
    int i = 0;

    for(std::vector<std::pair<GCITEMUID,KGCItemImgWnd* >>::iterator vIt = m_vecItemList.begin(); vIt != m_vecItemList.end();vIt++){
        if(i < size){
            vIt->first = vInfo[i].first;
            if( vIt->second ) {
                vIt->second->SetItemInfo(vInfo[i].second,-1,false,-1,vIt->first);
                vIt->second->ToggleRender(true);
            }            
        }
        else
        {
            vIt->first = 0;
            if( vIt->second ) {
                vIt->second->ToggleRender(false);
            }
        }
        ++i;
    }

    if(vInfo.empty())
        m_pkStartBtn->Lock(true);
    else
        m_pkStartBtn->Lock(false);

}

void KGCForgeItemCraftMachine::ClearResultItemList( void )
{
    for(std::vector<std::pair<GCITEMUID,KGCItemImgWnd* >>::iterator vIt = m_vecResultItemList.begin(); vIt != m_vecResultItemList.end();vIt++){
        vIt->first = 0;
        vIt->second->ToggleRender(false);
        ToggleStateImage(4);
    }

    for(std::vector<KD3DStatic*>::iterator vIt = m_vecStaticResultItemNumList.begin(); vIt != m_vecStaticResultItemNumList.end();vIt++){
        (*vIt)->ToggleRender(false);
    }


}

void KGCForgeItemCraftMachine::ClearItemList( void )
{
    for(std::vector<std::pair<GCITEMUID,KGCItemImgWnd* >>::iterator vIt = m_vecItemList.begin(); vIt != m_vecItemList.end();vIt++){
        // WaitList에 있으면 그것도 같이 지워 주자
        SiGCForgeManager()->EraseWaitItem(vIt->first);

        vIt->first = 0;
        vIt->second->ToggleRender(false);
    }

    for( std::vector<std::pair<GCITEMUID, KD3DWnd*>>::iterator vit = m_vecItemSelectBtnList.begin() ; vit != m_vecItemSelectBtnList.end() ; ++vit ) { 
        vit->first = 0;
        vit->second->ToggleRender( false );
    }

    for(std::vector<KD3DStatic*>::iterator vIt = m_vecStaticItemNumList.begin(); vIt != m_vecStaticItemNumList.end();vIt++){
        (*vIt)->ToggleRender(false);
    }
}

void KGCForgeItemCraftMachine::OnCreateItemBackList( void )
{
    if(m_vecBoxImg.empty() == false) return;

    m_vecBoxImg = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_new_forge.stg", "","new_forge_dlg\\forge_item_crafting_dlg\\material_item\\back",true,m_iMAX_ITEM_LIST_HEGHIT_COUNT * m_iMAX_ITEM_LIST_WIDTH_COUNT );
}

void KGCForgeItemCraftMachine::OnCreateItemList( void )
{
    if(m_vecItemList.empty() == true){
        
        std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_new_forge.stg", "","new_forge_dlg\\forge_item_crafting_dlg\\material_item\\item_img",true,m_iMAX_ITEM_LIST_WIDTH_COUNT * m_iMAX_ITEM_LIST_HEGHIT_COUNT );
        std::vector<KD3DWnd*> vecTempSelect = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_new_forge.stg", "","new_forge_dlg\\forge_item_crafting_dlg\\material_item\\item_select_btn",true,m_iMAX_ITEM_LIST_WIDTH_COUNT * m_iMAX_ITEM_LIST_HEGHIT_COUNT );

        for(std::vector<KD3DWnd*>::iterator vIt = vecTemp.begin(); vIt != vecTemp.end();vIt++){
            m_vecItemList.push_back(std::pair<GCITEMUID,KGCItemImgWnd*>(0,static_cast<KGCItemImgWnd*>(*vIt)));
        }

        for( std::vector< KD3DWnd* >::iterator it = vecTempSelect.begin() ; it != vecTempSelect.end() ; ++it ) { 
            m_vecItemSelectBtnList.push_back( std::make_pair( 0, *it ) );
        }
    }

    if(m_vecResultItemList.empty() == true){
        std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_new_forge.stg", "","new_forge_dlg\\forge_item_crafting_dlg\\create_item_img",true,m_iMAX_RESULT_ITEM_LIST_WIDTH * m_iMAX_RESULT_ITEM_LIST_HEGHIT );

        for(std::vector<KD3DWnd*>::iterator vIt = vecTemp.begin(); vIt != vecTemp.end();vIt++){
            m_vecResultItemList.push_back(std::pair<GCITEMID,KGCItemImgWnd*>(0,static_cast<KGCItemImgWnd*>(*vIt)));
        }
    }

    if(m_pPrekResultItem == NULL)
    {
        std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_new_forge.stg", "","new_forge_dlg\\forge_item_crafting_dlg\\create_item_img",true);
        if(vecTemp.begin() != vecTemp.end()){
            m_pPrekResultItem = static_cast<KGCItemImgWnd*>(*vecTemp.begin());
        }
    }

    if(m_vecStaticItemNumList.empty() == true){
        std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_new_forge.stg", "","new_forge_dlg\\forge_item_crafting_dlg\\static_create_count",true,m_iMAX_ITEM_LIST_WIDTH_COUNT * m_iMAX_ITEM_LIST_HEGHIT_COUNT );

        for(std::vector<KD3DWnd*>::iterator vIt = vecTemp.begin(); vIt != vecTemp.end();vIt++){
            m_vecStaticItemNumList.push_back(static_cast<KD3DStatic*>(*vIt));
        }
    }

    if(m_vecStaticResultItemNumList.empty() == true){
        std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_new_forge.stg", "","new_forge_dlg\\forge_item_crafting_dlg\\static_create_count",true,m_iMAX_RESULT_ITEM_LIST_WIDTH * m_iMAX_RESULT_ITEM_LIST_HEGHIT );

        for(std::vector<KD3DWnd*>::iterator vIt = vecTemp.begin(); vIt != vecTemp.end();vIt++){
            m_vecStaticResultItemNumList.push_back(static_cast<KD3DStatic*>(*vIt));
        }
    }

    if(m_pkStaticPreResultItemNum == NULL)
    {
        std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_new_forge.stg", "","new_forge_dlg\\forge_item_crafting_dlg\\static_create_count",true);
        if(vecTemp.begin() != vecTemp.end()){
            m_pkStaticPreResultItemNum = static_cast<KD3DStatic*>(*vecTemp.begin());
        }
    }

}

void KGCForgeItemCraftMachine::OnInitItemBackList( void )
{
    for(int i = 0 ; i < m_iMAX_ITEM_LIST_HEGHIT_COUNT ; i++){
        for(int j = 0; j < m_iMAX_ITEM_LIST_WIDTH_COUNT ; j++){
            int iIndex = i * m_iMAX_ITEM_LIST_WIDTH_COUNT + j;
            float x = static_cast<float>(j * 72.f * GC_SCREEN_DIV_WIDTH + 6.f * GC_SCREEN_DIV_WIDTH);
            float y = static_cast<float>(i * 72.f * GC_SCREEN_DIV_WIDTH + 90.f * GC_SCREEN_DIV_WIDTH);
            D3DXVECTOR2 vNewPos(x,y);
            m_vecBoxImg[iIndex]->InitState(true, false,NULL);
            m_vecBoxImg[iIndex]->SetWindowPos( vNewPos );
        }
    }
}

void KGCForgeItemCraftMachine::InitItemList( void )
{

    for(int i = 0 ; i < m_iMAX_ITEM_LIST_HEGHIT_COUNT ; i++){
        for(int j = 0; j < m_iMAX_ITEM_LIST_WIDTH_COUNT ; j++){
            int iIndex = i * m_iMAX_ITEM_LIST_WIDTH_COUNT + j;
            float x = static_cast<float>(j * 72.f * GC_SCREEN_DIV_WIDTH + 6.f * GC_SCREEN_DIV_WIDTH);
            float y = static_cast<float>(i * 72.f * GC_SCREEN_DIV_WIDTH + 90.f * GC_SCREEN_DIV_WIDTH);
            D3DXVECTOR2 vNewPos(x,y);
            m_vecItemList[iIndex].second->InitState(false, true,this);
            m_vecItemList[iIndex].second->SetWindowPos( vNewPos );
            m_vecItemList[iIndex].second->SetWidthDirect( GC_SCREEN_DIV_SIZE_INT(70) );
            m_vecItemList[iIndex].second->SetHeightDirect( GC_SCREEN_DIV_SIZE_INT(70) );

            vNewPos.y += 70; 
            m_vecStaticItemNumList[iIndex]->InitState(false,false,NULL);
            m_vecStaticItemNumList[iIndex]->SetWindowPos(vNewPos);
            m_vecStaticItemNumList[iIndex]->SetAlign( DT_RIGHT );

            m_vecItemList[iIndex].second->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCForgeItemCraftMachine::OnClickWaitBreakUpItem);
        }
    }

    for(int i = 0 ; i < m_iMAX_ITEM_LIST_HEGHIT_COUNT ; i++){
        for(int j = 0; j < m_iMAX_ITEM_LIST_WIDTH_COUNT ; j++){
            int iIndex = i * m_iMAX_ITEM_LIST_WIDTH_COUNT + j;
            float x = static_cast<float>(j * 72.f * GC_SCREEN_DIV_WIDTH + 6.f * GC_SCREEN_DIV_WIDTH);
            float y = static_cast<float>(i * 72.f * GC_SCREEN_DIV_WIDTH + 90.f * GC_SCREEN_DIV_WIDTH);
            D3DXVECTOR2 vNewPos(x,y);
            m_vecItemSelectBtnList[iIndex].second->InitState(false, true,this);
            m_vecItemSelectBtnList[iIndex].second->SetWindowPos( vNewPos );

            m_vecItemSelectBtnList[iIndex].second->DisconnectAllEventProcedure();
            m_vecItemSelectBtnList[iIndex].second->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCForgeItemCraftMachine::OnCliclItemSelect);
        }
    }

    for(int i = 0 ; i < m_iMAX_RESULT_ITEM_LIST_HEGHIT ; i++){
        for(int j = 0; j < m_iMAX_RESULT_ITEM_LIST_WIDTH ; j++){
            int iIndex = i * m_iMAX_RESULT_ITEM_LIST_WIDTH + j;
            float x = static_cast<float>(j * 72.f * GC_SCREEN_DIV_WIDTH + 45.f * GC_SCREEN_DIV_WIDTH);
            float y = static_cast<float>(i * 72.f * GC_SCREEN_DIV_WIDTH + 435.f * GC_SCREEN_DIV_WIDTH);
            D3DXVECTOR2 vNewPos(x,y);
            m_vecResultItemList[iIndex].second->InitState(false, false,NULL);
            m_vecResultItemList[iIndex].second->SetWindowPos( vNewPos );
            m_vecResultItemList[iIndex].second->SetWidthDirect( GC_SCREEN_DIV_SIZE_INT(70) );
            m_vecResultItemList[iIndex].second->SetHeightDirect( GC_SCREEN_DIV_SIZE_INT(70) );

            vNewPos.y += 70; 
            m_vecStaticResultItemNumList[iIndex]->InitState(false,false,NULL);
            m_vecStaticResultItemNumList[iIndex]->SetWindowPos(vNewPos);
            m_vecStaticResultItemNumList[iIndex]->SetAlign( DT_RIGHT );

        }
    }

    m_pPrekResultItem->InitState(false, false,NULL);
   // m_pPrekResultItem->SetWindowPos( D3DXVECTOR2(247.0f* GC_SCREEN_DIV_WIDTH,75.0f* GC_SCREEN_DIV_WIDTH) );
    m_pPrekResultItem->SetWidthDirect( GC_SCREEN_DIV_SIZE_INT(120) );
    m_pPrekResultItem->SetHeightDirect( GC_SCREEN_DIV_SIZE_INT(120) );
    m_pkStaticPreResultItemNum->InitState(false,false,NULL);
    m_pkStaticPreResultItemNum->SetAlign( DT_RIGHT );

}


void KGCForgeItemCraftMachine::ChangeCharacter()
{
    if(m_iForgeState == KGCForgeDlg::EKGCFOGE_CREATE)
        InitCraftMachine();

}

void KGCForgeItemCraftMachine::InitCraftMachine()
{

    if(m_pPrekResultItem)
        m_pPrekResultItem->ToggleRender(false);

    if(m_pkCost)
        m_pkCost->SetNumber(0);
        m_pkCost->SetAlign(DT_RIGHT);

    SetUserGPText();

    if(m_pkStartBtn && m_pkStopBtn){
        m_pkStartBtn->Lock(true);
        m_pkStopBtn->Lock(true);
    }

    m_iMachineDelayCnt = 0;
    m_iMachineState = EFMS_NORMAL;
    m_fMachineSpeed = 0.0f;

    if(m_pkStaticPreResultItemNum)
        m_pkStaticPreResultItemNum->ToggleRender(false);
    m_kCreateItemInfo.Clear();

    ClearItemList();
    ClearResultItemList();
    SetItemHeaterAndBtnSetting();
}

void KGCForgeItemCraftMachine::SetForgeState( int iState )
{
    m_iForgeState = iState;
    InitCraftMachine();
    SetItemHeaterAndBtnSetting();

    m_pkCost->SetNumber(0);
    SetUserGPText();
}

void KGCForgeItemCraftMachine::PostChildDraw( )
{
    for(int i = 0;m_pkCraftMachineObject != NULL;i++){
        KGC2DObject *pTemp = m_pkCraftMachineObject->Get2DObject(i);
        if(pTemp == NULL) break;

        pTemp->Render();
    }
    for ( int i = 0; i < 3; ++i ) 
	{
		m_pkState[i]->Render();
	}
}

void KGCForgeItemCraftMachine::FrameMoveInEnabledState( )
{    
    MoveCraftMachine();
    BoundItemList();
}

void KGCForgeItemCraftMachine::BoundItemList()
{
    if (CheckBoundItem(m_vecItemList)) return;
    else if (CheckBoundItem(m_vecResultItemList)) return;
}


bool KGCForgeItemCraftMachine::GetItemInfo( KItem** pInven, GCItem** pItem, std::pair<GCITEMUID,KGCItemImgWnd*> iter_)
{
    if( m_iForgeState == KGCForgeDlg::EKGCFOGE_CREATE ) { 
        g_pItemMgr->FindInventory(iter_.first, pInven, pItem);
        
        if( *pInven == NULL ) { 
            *pItem = g_pItemMgr->GetItemData( iter_.second->GetItemID() );
        }

        if( *pInven == NULL && *pItem == NULL ) { 
            return false;
        }
        else { 
            return true;
        }
    }
    else { 
        return g_pItemMgr->FindInventory(iter_.first, pInven, pItem);
    }
    return true;
}


bool KGCForgeItemCraftMachine::CheckBoundItem(std::vector<std::pair<GCITEMUID,KGCItemImgWnd*>>& vecItemList)
{
    bool bCheck = false;
    for(std::vector<std::pair<GCITEMUID,KGCItemImgWnd*>>::iterator vIt = vecItemList.begin(); vIt != vecItemList.end(); vIt++){
        if(vIt->second->CheckMousePosInWindowBound())
        {
            KItem* pInventory = NULL;
            GCItem* pItem = NULL;

            if( false == GetItemInfo(&pInventory, &pItem,*vIt))
                continue;

            g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem , -1, pInventory);

            const std::vector<RecipeSimpleInfo>& vecShowRecipe = SiGCForgeManager()->GetShowRecipeList();

            D3DXVECTOR2 vec = vIt->second->GetCurrentWindowPos();
            vec.x *= m_fWindowScaleX;
            vec.y *= m_fWindowScaleY;
            vec.x += -200.0f;
            vec.y += 60.0f;

            g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vec );
            g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(KD3DWnd::D3DWZ_TOPMOST);
            bCheck = true;
            break;
        }
    }
    g_pkUIScene->m_pkItemInformationBox->ToggleRender( bCheck );
    SpeakToActionListener( KActionEvent( NULL, KGCUIScene::D3DWE_FORGE_BOUND_ITEM, static_cast<KEVETPARM>(bCheck) ) );

    return bCheck;
}

void KGCForgeItemCraftMachine::RefreshGauge( )
{
    //DWORD gauge = static_cast<DWORD>( (static_cast<float>(m_iMachineDelayCnt) / 270.f) *142.f* GC_SCREEN_DIV_WIDTH );
    //m_pkGaugeCenter->SetWidth(gauge);
    //m_pkGaugeRight->SetWindowPosX(static_cast<float>(gauge+25.f* GC_SCREEN_DIV_WIDTH));

}

void KGCForgeItemCraftMachine::MoveCraftMachine( )
{
    if (m_iMachineState == EFMS_NORMAL)
    {
        return;
    }

    if (m_iMachineDelayCnt == -1)
    {
        m_iMachineDelayCnt = 0;
        m_iMachineState = EFMS_NORMAL;
        ToggleStateImage(-1);
        return;
    }
    else if (m_iMachineDelayCnt == 60 && m_iForgeState == KGCForgeDlg::EKGCFOGE_CREATE)
    {
        m_iMachineState = EFMS_CONTINUE;
        OnClickStartBtnCreate();
        ToggleStateImage(-1);
        m_iMachineDelayCnt = 40;
        return;
    }
    else if (m_iMachineDelayCnt == 60 && m_iForgeState == KGCForgeDlg::EKGCFOGE_BREAKUP)
    {
        m_iMachineState = EFMS_CONTINUE;
        OnClickStartBtnBreakUp();
        m_iMachineDelayCnt = 0;
        return;
    }

    m_iMachineDelayCnt += ((m_iMachineState == EFMS_START) ? 1 : -1);
}

void KGCForgeItemCraftMachine::OnCliclItemSelect( const KActionEvent& event )
{
    if( m_iForgeState == KGCForgeDlg::EKGCFOGE_BREAKUP ) {
        return;
    }
    
    if(m_iMachineState != EFMS_NORMAL ) { 
        return;
    }
    
    GCITEMUID selUID = 0;
    for( std::vector<std::pair<GCITEMUID, KD3DWnd*>>::iterator vit = m_vecItemSelectBtnList.begin() ; vit != m_vecItemSelectBtnList.end() ; ++vit ) { 
        if( vit->second == event.m_pWnd ) { 
            selUID = vit->first;
            break;
        }
    }

    GCItem* pItemData = NULL;
    g_pItemMgr->FindInventory( selUID, &pItemData );
    if( pItemData == NULL ) { 
        return;
    }

    D3DXVECTOR2 dxvPos = event.m_pWnd->GetCurrentWindowPos();

    //float fX, fY;
    //KLuaManager luaMgr;
    //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
    //LUA_GET_VALUE_DEF( "X", fX, 0.0f );
    //LUA_GET_VALUE_DEF( "Y", fY, 0.0f );

    //dxvPos.x += fX;
    dxvPos.y += 63.0f;

    g_pkUIScene->m_pkItemCreationSelectBox->SetPos( dxvPos );
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ITEM_CREATION_SELECT_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL , pItemData->dwGoodsID, 0, true, true, false);
}

void KGCForgeItemCraftMachine::MakeMaterialUIDList( void )
{
    m_vecMatieralUIDList.clear();

    std::vector<std::pair<GCITEMUID,KGCItemImgWnd*>>::const_iterator vitList = m_vecItemList.begin();
    for( ; vitList != m_vecItemList.end() ; ++vitList ) {
        if( vitList->second == NULL || vitList->second->GetItemID() == KItem::INVALID_ITEM || vitList->first == 0 ) { 
            continue;
        }
        m_vecMatieralUIDList.push_back( vitList->first );
    }
}

void KGCForgeItemCraftMachine::SetSelectedMaterial( IN const GCITEMUID itemUID_ )
{
    KItem* pSelItem = NULL;
    if( false == g_pItemMgr->FindInventory( itemUID_ , &pSelItem ) ) {
        return;
    }

    if( NULL == pSelItem ) { 
        return;
    }

    std::vector<std::pair<GCITEMUID,KGCItemImgWnd*>>::iterator itemIter = m_vecItemList.begin();
    for( ; itemIter != m_vecItemList.end() ; ++itemIter ) { 
        KItem* pBeforeItem = NULL;
        if( false == g_pItemMgr->FindInventory( itemIter->first, &pBeforeItem ) ) { 
            continue;
        }

        if( NULL == pBeforeItem ) { 
            return;
        }

        if( pSelItem->m_ItemID == pBeforeItem->m_ItemID ) { 
            itemIter->first = itemUID_;
            break;
        }
    }
}

bool KGCForgeItemCraftMachine::GetNotCoordiItemUID( const IN GCITEMID itemID_, OUT GCITEMUID& itemUID_ )
{
    int iGrade = -1;
    itemUID_ = 0;

    std::vector< KItem* > vecInvenItem;
    if( false == g_pItemMgr->GetInventoryItemListFromItemID( itemID_, vecInvenItem ) ) { 
        return false;
    }

    GCItem* pItemData = g_pItemMgr->GetItemData( itemID_ );
    if( NULL == pItemData ) { 
        return false;
    }

    std::vector< KItem* >::const_iterator itItem = vecInvenItem.begin();
    for( ; itItem != vecInvenItem.end() ; ++itItem ) { 
        GCItem* pItem = g_pItemMgr->GetItemData( (*itItem)->m_ItemID );
        CONTINUE_NIL( pItem );

        if( pItem->eItemKind == GIK_ENCHANT_ENABLE_ITEM ) { // 강화석 처리도 여기서 합니다...
            if( (*itItem)->m_EnchantEquipItemUID != 0 || // 장착된 강화석이거나 
                (*itItem)->m_cGradeID != KItem::GRADE_NORMAL || // 노멀 등급이 아니거나 
                (*itItem)->m_cEnchantLevel != 0 ) { // 0강이 아니거나 
                    continue;
            }
        }

        if( pItem->eItemKind == GIK_COORDI_ITEM && (*itItem)->m_cItemType == KItem::TYPE_NORMAL ) { 
            continue;
        }

        if( pItem->eItemKind != GIK_COORDI_ITEM && (*itItem)->m_cItemType == KItem::TYPE_LOOK ) { 
            continue;
        }

        // 장창중인 아이템은 제외 
        if( g_pItemMgr->CheckEquipItemByItemUID( &g_kGlobalValue.m_kUserInfo.GetCurrentChar(), (*itItem)->m_ItemUID ) ) {
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

bool KGCForgeItemCraftMachine::IsSelectedMaterialItem( const IN GCITEMUID itemUID_ )
{
    std::vector<std::pair<GCITEMUID,KGCItemImgWnd* >>::iterator vIt = m_vecItemList.begin();

    for ( ; vIt != m_vecItemList.end(); ++vIt ) 
    {
        if ( vIt->first == itemUID_ )
        {
            return true;
        }
    }

    return false;
}

void KGCForgeItemCraftMachine::SetUserGPText( const int iConst_ /*= -1*/ )
{
    if( NULL == m_pkGP ) { 
        return;
    }

    int iGPValue = g_kGlobalValue.GetCurrentCharacterGP();
    bool bEnableGP = iGPValue - iConst_ >= 0 ? true : false;
    if( false == bEnableGP ) { 
        m_pkGP->SetFontColor( 0xffff0000 );
    }
    else {
        m_pkGP->SetFontColor( 0xffffffff );
    }

    m_pkGP->SetNumber( iGPValue );
    m_pkGP->SetAlign(DT_RIGHT);
}