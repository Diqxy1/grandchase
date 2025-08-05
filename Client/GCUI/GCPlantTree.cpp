#include "stdafx.h"
#include "GCPlantTree.h"
#include "GCPlantTreeMap.h"
#include "GCPlantTreeRewardItemListView.h"

IMPLEMENT_CLASSNAME( KGCPlantTree );
IMPLEMENT_WND_FACTORY( KGCPlantTree );
IMPLEMENT_WND_FACTORY_NAME( KGCPlantTree, "gc_event_plant_tree_box" );

#define MAX_WORLD_TREE_PROGRESS_BAR (487)
#define MAX_WATER_PROGRESS_BAR (188)
#define MAX_PLANT_TREE_NUM (10)

KGCPlantTree::KGCPlantTree( void )
: m_pkWoldMapList(NULL)
, m_pkTotalProgressbarL(NULL)
, m_pkTotalProgressbarR(NULL)
, m_pkTotalProgressbarC(NULL)
, m_pkWaterProgressbarL(NULL)
, m_pkWaterProgressbarR(NULL)
, m_pkWaterProgressbarC(NULL)
, m_iContinentType(-1)
, m_pkBtnViewRewardItem(NULL)
, m_pkClose(NULL)
, m_pkBkWorldBtn(NULL)
, m_pkBtnMoveWorld(NULL)
, m_pkBtnBuyWaterItem(NULL)
, m_pkBtnBuyGoldTreeItem(NULL) 
, m_pkBtnBuyFertilizersItem(NULL)
, m_iRealContinentType(-1)
, m_iRealCurrentPos(-1)
, m_iRealCurrentState(-1)
, m_bRenderProgressBar( false )
//, m_pkReardItemListView(NULL)
{
    m_vecTreeList.clear();
    m_vecEarthList.clear();
    m_vecTreeStateList.clear();
    for(int i = 0;i < MAX_PLANT_TREE_NUM;i++){
        m_vecTreeList.push_back(NULL);
        m_vecEarthList.push_back(NULL);
    }

    for(int i = 0;i <EPTS_NUM;i++)
    {
        m_vecTreeStateList.push_back(NULL);
    }

    for(int i = 0;i<EPTB_NUM;i++)
    {
        m_vecBtnList.push_back(NULL);
    }

    for(int i = 0;i<EPTSTR_NUM;i++)
    {
        m_vecStaticList.push_back(NULL);
    }
    for(int i = 0;i<KGCPlantTreeMap::EPTW_NUM;i++)
    {

    }
    m_mapBtnWorldList.insert(std::pair<int,KD3DWnd*>(KGCPlantTreeMap::EPTW_BERMESIAH,NULL));
    m_mapBtnWorldList.insert(std::pair<int,KD3DWnd*>(KGCPlantTreeMap::EPTW_SILVER_LAND,NULL));
    m_mapBtnWorldList.insert(std::pair<int,KD3DWnd*>(KGCPlantTreeMap::EPTW_ELLIA,NULL));
    m_mapBtnWorldList.insert(std::pair<int,KD3DWnd*>(KGCPlantTreeMap::EPTW_ARCHIMEDIA,NULL));

    for(int i = 0;i<MAX_PLANT_TREE_NUM;i++)
    {
        char buf[256] = "";
        sprintf(buf,"tree_%d",i+1);
        LINK_CONTROL(buf,m_vecTreeList[i]);

        memset(buf,0,256);
        sprintf(buf,"earth_%d",i+1);
        LINK_CONTROL(buf,m_vecEarthList[i]);
    }

    for(int i = 0;i <EPTS_NUM;i++)
    {
        char buf[256] = "";
        sprintf(buf,"tree_state_%d",i);
        LINK_CONTROL(buf,m_vecTreeStateList[i]);
    }

    

    LINK_CONTROL("btn_water_item",m_vecBtnList[EPTB_WATER_ITEM]);
    LINK_CONTROL("btn_fertilizers_item",m_vecBtnList[EPTB_FERTILIZERS_ITEM]);
    LINK_CONTROL("btn_green_tree_item",m_vecBtnList[EPTB_GREEN_TREE_ITEM]);
    LINK_CONTROL("btn_gold_tree_item",m_vecBtnList[EPTB_GOLD_TREE_ITEM]);
    LINK_CONTROL("btn_tree_reward_item",m_vecBtnList[EPTB_GET_TREE_ITEM]);
    LINK_CONTROL("btn_world_reward_item",m_vecBtnList[EPTB_GET_WORLD_ITEM]);

    LINK_CONTROL("static_water",m_vecStaticList[EPTSTR_WATER_ITEM_TITLE]);
    LINK_CONTROL("static_fertilizers",m_vecStaticList[EPTSTR_FERTILIZERS_ITEM_TITLE]);
    LINK_CONTROL("static_green_tree",m_vecStaticList[EPTSTR_GREEN_TREE_ITEM_TITLE]);
    LINK_CONTROL("static_gold_tree",m_vecStaticList[EPTSTR_GOLD_TREE_ITEM_TITLE]);
    LINK_CONTROL("static_water_value",m_vecStaticList[EPTSTR_WATER_ITEM]);
    LINK_CONTROL("static_fertilizers_value",m_vecStaticList[EPTSTR_FERTILIZERS_ITEM]);
    LINK_CONTROL("static_green_tree_value",m_vecStaticList[EPTSTR_GREEN_TREE_ITEM]);
    LINK_CONTROL("static_gold_tree_value",m_vecStaticList[EPTSTR_GOLD_TREE_ITEM]);
    LINK_CONTROL("static_description",m_vecStaticList[EPTSTR_DESCRIPTION]);

    LINK_CONTROL("worldmap_list",m_pkWoldMapList);

    LINK_CONTROL("progress_bar_l",m_pkTotalProgressbarL);
    LINK_CONTROL("progress_bar_r",m_pkTotalProgressbarR);
    LINK_CONTROL("progress_bar_c",m_pkTotalProgressbarC);

    LINK_CONTROL("water_progress_bar_l",m_pkWaterProgressbarL);
    LINK_CONTROL("water_progress_bar_r",m_pkWaterProgressbarR);
    LINK_CONTROL("water_progress_bar_c",m_pkWaterProgressbarC);

    LINK_CONTROL("btn_view_item",m_pkBtnViewRewardItem);

    LINK_CONTROL("btn_close",m_pkClose);

    LINK_CONTROL("btn_bermesiah",m_mapBtnWorldList[KGCPlantTreeMap::EPTW_BERMESIAH]);
    LINK_CONTROL("btn_silverland",m_mapBtnWorldList[KGCPlantTreeMap::EPTW_SILVER_LAND]);
    LINK_CONTROL("btn_ellia",m_mapBtnWorldList[KGCPlantTreeMap::EPTW_ELLIA]);
    LINK_CONTROL("btn_archimedia",m_mapBtnWorldList[KGCPlantTreeMap::EPTW_ARCHIMEDIA]);

    LINK_CONTROL("bk_world_btn",m_pkBkWorldBtn);
    LINK_CONTROL("btn_move_world",m_pkBtnMoveWorld);

    LINK_CONTROL("btn_buy_water_item",m_pkBtnBuyWaterItem);
    LINK_CONTROL("btn_buy_goldtree_item",m_pkBtnBuyGoldTreeItem);
    LINK_CONTROL("btn_buy_fertilizer_item",m_pkBtnBuyFertilizersItem);

//    LINK_CONTROL("reward_list_view_box",m_pkReardItemListView);
    std::vector<std::string> vecStrParticle;
    vecStrParticle.push_back("Tree_Water_01");
    vecStrParticle.push_back("Tree_Water_02");
    m_mapParticle.insert(std::pair<int, std::vector<std::string>>(EPTB_WATER_ITEM,vecStrParticle));
    vecStrParticle.clear();
    vecStrParticle.push_back("Tree_Biryo");
    m_mapParticle.insert(std::pair<int, std::vector<std::string>>(EPTB_FERTILIZERS_ITEM,vecStrParticle));
    
    vecStrParticle.clear();
    vecStrParticle.push_back("MZP_Win_ef_01");
    vecStrParticle.push_back("MZP_Win_ef_02");
    vecStrParticle.push_back("MZP_Win_ef_07");
    vecStrParticle.push_back("Namunamu_Com_01");
    vecStrParticle.push_back("Namunamu_Com_02");
    vecStrParticle.push_back("Namunamu_Com_03");
    m_mapParticle.insert(std::pair<int, std::vector<std::string>>(100,vecStrParticle));

    vecStrParticle.clear();
    vecStrParticle.push_back("MZP_Win_ef_01");
    vecStrParticle.push_back("MZP_Win_ef_02");
    vecStrParticle.push_back("Mission_Com_02");
    vecStrParticle.push_back("Namunamu_Golden_Com_01");
    vecStrParticle.push_back("Namunamu_Golden_Com_02");
    vecStrParticle.push_back("Namunamu_Golden_Com_03");
    m_mapParticle.insert(std::pair<int, std::vector<std::string>>(200,vecStrParticle));



}

KGCPlantTree::~KGCPlantTree( void )
{
}

void KGCPlantTree::ActionPerformed( const KActionEvent& event )
{
//    if(m_pkReardItemListView->IsRenderOn()) return;

    if(event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
    {
        if(m_pkClose == event.m_pWnd)
        {
            SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
            return;
        }
        if(Result_PlantTreeInfo == INT_MAX) return;
        if(m_pkBtnViewRewardItem == event.m_pWnd)
        {
            g_pkUIScene->m_pkPlantTreeRewardItemListView->InitBtnState();
            g_pkUIScene->m_pkPlantTreeRewardItemListView->SetRewardSetting();

            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_PLANT_TREE_REWARD_LIST_VIEW_DLG,L"",L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,true);
        }
        OnClickBtn(event);
    }
}

void KGCPlantTree::BuyItem( DWORD& dwBuyItemID )
{
    GCItem* pItem = g_pItemMgr->GetItemData( dwBuyItemID );
    if( pItem->eMoneyType == EMT_CASH )
    {
#if defined ( USE_VIP_POINT )
        g_pItemMgr->BuyCashItemS6( dwBuyItemID );
#else
    #ifdef GAME_CASH
        g_pkUIScene->EnableBuyCashItemWnd( dwBuyItemID, false, true );
    #else    
        g_MyD3D->m_kItemMgr.BuyItem( dwBuyItemID, pItem->iItemTypeValue  );
    #endif
#endif
    }
    else
    {
        std::wostringstream strmText1;
        std::wostringstream strmText2;
#if defined ( _PORTUGUESE ) || defined( _SPANISH )
        strmText1 << pItem->strItemName;
        strmText2 << g_pkStrLoader->GetReplacedString( STR_ID_BUY_PRICE, "i", (int)pItem->dwPrice );
#elif defined( _ENGLISH )
        strmText1<<g_pkStrLoader->GetReplacedString( STR_ID_BUY_PRICE, "li", pItem->strItemName, (int)pItem->dwPrice );
#else
        strmText1 << pItem->strItemName << g_pkStrLoader->GetString( STR_ID_OBJECT_POSTPOSITION );
        strmText2 << (int)pItem->dwPrice << ' ' << g_pkStrLoader->GetString( STR_ID_BUY_PRICE );
#endif
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO,
            strmText1.str(), strmText2.str(),
            KGCUIScene::GC_MBOX_USE_BUY, dwBuyItemID, pItem->iItemTypeValue, true, true );
    }
}

void KGCPlantTree::OnCreate( void )
{
    m_pkClose->SetHotKey(DIK_ESCAPE);
    for(std::vector<KD3DWnd*>::iterator vIter = m_vecBtnList.begin();vIter != m_vecBtnList.end();vIter++)
    {
        (*vIter)->InitState(false,true,this);
    }
    m_pkBtnViewRewardItem->InitState(true,true,this);

    m_pkWoldMapList->InitState(true,true,NULL);

    for(std::vector<KD3DStatic*>::iterator vIter = m_vecStaticList.begin();vIter != m_vecStaticList.end();vIter++)
    {
        (*vIter)->InitState(true,false,NULL);
        (*vIter)->SetAlign(DT_CENTER);
    }

    m_vecStaticList[EPTSTR_WATER_ITEM_TITLE]->SetText(g_pkStrLoader->GetString( STR_ID_PLANT_TREE_23 ));    
    m_vecStaticList[EPTSTR_FERTILIZERS_ITEM_TITLE]->SetText(g_pkStrLoader->GetString( STR_ID_PLANT_TREE_24 ));  
    m_vecStaticList[EPTSTR_GREEN_TREE_ITEM_TITLE]->SetText(g_pkStrLoader->GetString( STR_ID_PLANT_TREE_21 )); 
    m_vecStaticList[EPTSTR_GREEN_TREE_ITEM]->SetText(L"∞");
    m_vecStaticList[EPTSTR_GOLD_TREE_ITEM_TITLE]->SetText(g_pkStrLoader->GetString( STR_ID_PLANT_TREE_22 ));  
    
    std::wostringstream strmDesc;
    strmDesc << g_pkStrLoader->GetString( STR_ID_PLANT_TREE_18 ) << std::endl << g_pkStrLoader->GetString( STR_ID_PLANT_TREE_19 ) << std::endl << g_pkStrLoader->GetString( STR_ID_PLANT_TREE_20 );
    m_vecStaticList[EPTSTR_DESCRIPTION]->SetAlign( DT_LEFT );
    m_vecStaticList[EPTSTR_DESCRIPTION]->SetMultiLine( true );
    m_vecStaticList[EPTSTR_DESCRIPTION]->SetText( strmDesc.str() );
    
    m_pkClose->InitState(true,true,this);
    m_pkBkWorldBtn->InitState(false,false,NULL);
    m_pkBtnMoveWorld->InitState(true,true,this);

    m_pkBtnBuyWaterItem->InitState(true,true,this);  
    m_pkBtnBuyGoldTreeItem->InitState(true,true,this);
    m_pkBtnBuyFertilizersItem->InitState(true,true,this);

	m_pkTotalProgressbarL->ToggleRender(true);
	m_pkTotalProgressbarC->ToggleRender(true);
	m_pkTotalProgressbarR->ToggleRender(true);

    for(std::map<int, KD3DWnd*>::iterator mIter = m_mapBtnWorldList.begin(); mIter != m_mapBtnWorldList.end(); mIter++)
    {
        mIter->second->InitState(false,true,this);
    }
//    m_pkReardItemListView->InitState(false,false,NULL);
    InitPlantTree();

}

void KGCPlantTree::FrameMoveInEnabledState()
{
    if( m_bRenderProgressBar ) { 
        m_pkTotalProgressbarL->ToggleRender(true);
        m_pkTotalProgressbarC->ToggleRender(true);
        m_pkTotalProgressbarR->ToggleRender(true);
        
        m_bRenderProgressBar = false;
    }
}

void KGCPlantTree::InitPlantTree( void )
{
    Result_PlantTreeInfo = INT_MAX;
    KP2P::GetInstance()->Send_PlantTreeInfoReq();
    g_MyD3D->WaitForServerAck(Result_PlantTreeInfo , INT_MAX, 3000, TIME_OUT_VALUE );

}

void KGCPlantTree::SetPlantTreeRewardInfo( KEVENT_PLANT_TREE_MAP_NOT &kRecv )
{
    m_mapPlantTreeInfo.clear();
    m_mapPlantTreeInfo = kRecv;
    m_iContinentType = 0;
}

void KGCPlantTree::SetPlantTreeInfo( DWORD dwContinentType, DWORD dwCurrentPosition, DWORD dwTreeStatus)
{
    m_iRealContinentType = dwContinentType;
    m_iRealCurrentPos = dwCurrentPosition;
    m_iRealCurrentState = dwTreeStatus;
    m_pkWoldMapList->SetWorldMapIndex(dwContinentType);

    SetTree(dwContinentType,dwCurrentPosition,dwTreeStatus);
    SetBtn(dwTreeStatus);
    SetInfoWindow();
    SetTreeProgressBar(dwContinentType,dwCurrentPosition);
    SetWaterProgressBar(dwContinentType,dwTreeStatus);
}


void KGCPlantTree::On_ActionAck( KEVENT_PLANT_TREE_ACTION_ACK &kRecv )
{
    RecvItemList(kRecv.m_vecItem, false);
    SetPlantTreeInfo(kRecv.m_dwContinentType,kRecv.m_dwCurrentPosition,kRecv.m_dwTreeStatus );
}

void KGCPlantTree::On_RewardAck( KEVENT_PLANT_TREE_REWARD_ACK &kRecv )
{
    SetPlantTreeInfo(kRecv.m_dwContinentType,kRecv.m_dwCurrentPosition,kRecv.m_dwTreeStatus );
    RecvItemList(kRecv.m_vecRewardItem, true);
}

void KGCPlantTree::SetInfoWinowStatic( int itemID, int staticIndex )
{
    KItem *pItem =NULL;
    WCHAR wchar[256] = L""; 
    pItem = g_pItemMgr->m_kInventory.FindItemByItemID( itemID );
    if(pItem == NULL)
        wsprintf(wchar,L"%d",0);
    else
        wsprintf(wchar,L"%d",pItem->m_nCount);
    m_vecStaticList[staticIndex]->SetText(wchar);
}


void KGCPlantTree::SetInfoWindow( )
{
    SetInfoWinowStatic(KGCItemManager::ITEM_PLANTTREE_GOLDTREE,EPTSTR_GOLD_TREE_ITEM);
    SetInfoWinowStatic(KGCItemManager::ITEM_PLANTTREE_WATER,EPTSTR_WATER_ITEM);
    SetInfoWinowStatic(KGCItemManager::ITEM_PLANTTREE_MANURE,EPTSTR_FERTILIZERS_ITEM);
}

void KGCPlantTree::SetBtn( int iTreeStatus)
{
    std::vector<KD3DWnd*>::iterator vIter = m_vecBtnList.begin();
    for(;vIter != m_vecBtnList.end();vIter++)
    {
        (*vIter)->ToggleRender(false);
        (*vIter)->Lock(false);
    }
    if( iTreeStatus == 0 || iTreeStatus == 400 )
    {
        m_vecBtnList[EPTB_GREEN_TREE_ITEM]->ToggleRender(true);
        m_vecBtnList[EPTB_GOLD_TREE_ITEM]->ToggleRender(true);
    }
    else if( iTreeStatus >= 10 && iTreeStatus < 100  )
    {
        m_vecBtnList[EPTB_WATER_ITEM]->ToggleRender(true);
        m_vecBtnList[EPTB_FERTILIZERS_ITEM]->ToggleRender(true);
        if(iTreeStatus>=50)
            m_vecBtnList[EPTB_FERTILIZERS_ITEM]->Lock(true);
    }
    else if( iTreeStatus == 100 || iTreeStatus == 200 || iTreeStatus == 300)
    {
        m_vecBtnList[EPTB_GET_TREE_ITEM]->ToggleRender(true);
        m_vecBtnList[EPTB_GET_WORLD_ITEM]->ToggleRender(true);
        if(iTreeStatus != 300)
            m_vecBtnList[EPTB_GET_WORLD_ITEM]->Lock(true);
        else
            m_vecBtnList[EPTB_GET_TREE_ITEM]->Lock(true);
    }
}


void KGCPlantTree::ClearTree( )
{
    std::vector<KD3DWnd*>::iterator vIter = m_vecTreeList.begin();
    for(;vIter != m_vecTreeList.end();vIter++)
    {
        (*vIter)->ToggleRender(false);
    }
}

void KGCPlantTree::ClearEarth( )
{
    std::vector<KD3DWnd*>::iterator vIter = m_vecEarthList.begin();
    for(;vIter != m_vecEarthList.end();vIter++)
    {
        (*vIter)->ToggleRender(false);
    }
}

void KGCPlantTree::ClearTreeState( )
{
    std::vector<KD3DWnd*>::iterator vIter = m_vecTreeStateList.begin();
    for(;vIter != m_vecTreeStateList.end();vIter++)
    {
        (*vIter)->ToggleRender(false);
    }
}

std::vector<D3DXVECTOR2> KGCPlantTree::GetObjectPos( int iContinentType )
{
    std::vector<D3DXVECTOR2> vecPos;

    KLuaManager luaMgr;
    if( GCFUNC::LoadLuaScript( luaMgr , "EventScript.stg" ) == false )
        return vecPos;


    LUA_BEGIN_TABLE( "PLANT_TREE_OBJECT_POSITION", return vecPos;)
        for( int i = 1; ; i++ ){
            LUA_BEGIN_TABLE(i,break)
                int iType = 0;
                LUA_GET_VALUE_NOASSERT("CONTINENT_TYPE",iType,break);
                if(iType != iContinentType){
                    LUA_END_TABLE(/*i*/;);
                    continue;
                }
                LUA_BEGIN_TABLE("POSITION",;)
                    for(int j=1;;j++)
                    {
                        LUA_BEGIN_TABLE(j,;)
                            D3DXVECTOR2 pos;
                            LUA_GET_VALUE_NOASSERT(1,pos.x,break);
                            LUA_GET_VALUE_NOASSERT(2,pos.y,break);
                            vecPos.push_back(pos);
                        LUA_END_TABLE(/*POSITION*/;);
                    }
                LUA_END_TABLE(/*POSITION*/;);
            LUA_END_TABLE(/*i*/;);
            break;
        }
    LUA_END_TABLE(;);

    return vecPos;
}


void KGCPlantTree::SetObjectPos( int iContinentType , int iCurrentPos ,int iTreeStatus)
{
    std::vector<D3DXVECTOR2> vPos = GetObjectPos(iContinentType);
    std::vector<D3DXVECTOR2>::const_iterator vIter = vPos.begin();

    for(int i = 0;vIter != vPos.end();vIter++, i++)
    {
        D3DXVECTOR2 pos; 
        if(iCurrentPos-1 == i)
        {
            std::vector<KD3DWnd*>::iterator vIterState = m_vecTreeStateList.begin();
            for(;vIterState != m_vecTreeStateList.end();vIterState++)
            {
                pos = *vIter;
                pos.x -=  (*vIterState)->GetWidth()/2.0f;
                pos.y -=  (*vIterState)->GetHeight();
                (*vIterState)->SetWindowPosDirect(pos);
            }
        }

        pos = *vIter;
        pos.x -=  m_vecTreeList[i]->GetWidth()/2.0f;
        pos.y -=  m_vecTreeList[i]->GetHeight();
        m_vecTreeList[i]->SetWindowPosDirect(pos);

        pos = *vIter;
        pos.x -=  m_vecEarthList[i]->GetWidth()/2.0f;
        pos.y -=  m_vecEarthList[i]->GetHeight();
        m_vecEarthList[i]->SetWindowPosDirect(pos);
    }
}

int KGCPlantTree::TransferStateInClient( int iTreeStatus )
{

    if( iTreeStatus == 0 )
        return 0;
    else if( iTreeStatus == 10 )
        return 1;
    else if( iTreeStatus == 11 )
        return 2;
    else if( iTreeStatus > 11 && iTreeStatus < 50 )
        return 3;
    else if( iTreeStatus == 50 )
        return 5;
    else if( iTreeStatus == 51 )
        return 6;
    else if( iTreeStatus > 51 && iTreeStatus < 100 )
        return 7;
    else if( iTreeStatus == 100 )
    {
        ShowRewardParticle(iTreeStatus);
        return 4;
    }
    else if( iTreeStatus == 200 )
    {
        ShowRewardParticle(iTreeStatus);
        return 8;
    }
    else if( iTreeStatus == 300 )
        return 3;
    else if( iTreeStatus == 400 )
        return 9;

    return -1;
}



void KGCPlantTree::SetTree(int iContinentType, int iCurrentPos, int iTreeStatus)
{
    m_mapPlantTreeInfo[iContinentType].m_dwTotalTreeCount;

    KD3DWnd *wnd  = NULL;

    if(iContinentType !=  m_iContinentType || iCurrentPos!= m_iCurrentPos){
        ClearTree();
        ClearEarth();
        m_iContinentType = iContinentType;
        m_iCurrentPos = iCurrentPos;
        SetObjectPos(iContinentType,iCurrentPos, iTreeStatus);
    }

    for(int i = 0;i < static_cast<int>( m_mapPlantTreeInfo[iContinentType].m_dwTotalTreeCount); i++)
    {
        if(i < iCurrentPos-1){
            m_vecTreeList[i]->ToggleRender(true);
        }
        else if(i >=iCurrentPos){
            m_vecEarthList[i]->ToggleRender(true);
        }
    }
    ClearTreeState();
    int iTreeMode = TransferStateInClient(iTreeStatus);
    if(iTreeMode != -1)
        m_vecTreeStateList[iTreeMode]->ToggleRender(true);
}

void KGCPlantTree::SetTreeProgressBar(int iContinentType, int iCurrentPos)
{
    static int iBeforeWidth = 0;
	if(iCurrentPos == 1 || m_mapPlantTreeInfo[iContinentType].m_dwTotalTreeCount == 0) {
		m_pkTotalProgressbarL->SetWidth(0);
		m_pkTotalProgressbarC->SetWidth(0);
		m_pkTotalProgressbarR->SetWidth(0);
		return;
	}

	int width = MAX_WORLD_TREE_PROGRESS_BAR/m_mapPlantTreeInfo[iContinentType].m_dwTotalTreeCount;

	width *= iCurrentPos - 1;
	if ( m_pkTotalProgressbarR->GetWidth() == 0 )
	{
		m_pkTotalProgressbarL->SetWidth(20);
		m_pkTotalProgressbarR->SetWidth(20);
	}	

	m_pkTotalProgressbarC->SetWidth(width);
	m_pkTotalProgressbarR->SetWindowPosX(static_cast<float>(width + 120));

    if( iBeforeWidth != width ) { 
        iBeforeWidth = width;
        m_bRenderProgressBar = true;
    }
}

void KGCPlantTree::SetWaterProgressBar(int iContinentType, int iNum)
{
    if( iNum > 10 && iNum < 50 )
        iNum -= 10;
    else if( iNum > 50 && iNum < 100 )
        iNum -= 50;
    else
        iNum = 0;

    m_pkWaterProgressbarL->ToggleRender(false);
    m_pkWaterProgressbarC->ToggleRender(false);
    m_pkWaterProgressbarR->ToggleRender(false);
    if(iNum == 0) return;
    else if(m_mapPlantTreeInfo[iContinentType].m_dwWaterCount == 0) return;

    m_pkWaterProgressbarL->ToggleRender(true);
    m_pkWaterProgressbarC->ToggleRender(true);
    m_pkWaterProgressbarR->ToggleRender(true);


    int width = MAX_WATER_PROGRESS_BAR/m_mapPlantTreeInfo[iContinentType].m_dwWaterCount;

    width *= iNum;

    m_pkWaterProgressbarC->SetWidth(width);
    m_pkWaterProgressbarR->SetWindowPosX(static_cast<float>(width + 387));
}


bool KGCPlantTree::OnClickUseItemBtn( const KActionEvent &event,int iBtnType, int iActionType, DWORD dwItemID )
{
    if(event.m_pWnd == m_vecBtnList[iBtnType])
    {
        if(dwItemID != 0)
        {
            KItem *pItem =NULL;
            pItem = g_pItemMgr->m_kInventory.FindItemByItemID( dwItemID );
            if(pItem == NULL){
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PLANT_TREE_02 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
                return true;
            }
            else if(pItem->m_nCount == 0){
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PLANT_TREE_02 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
                return true;
            }

        }
        std::map<int,std::vector<std::string>>::iterator mIter = m_mapParticle.find(iBtnType);
        if(mIter != m_mapParticle.end()){

            std::vector<D3DXVECTOR2> vecPos = GetObjectPos(m_iRealContinentType);
            D3DXVECTOR2 pos = vecPos[m_iRealCurrentPos-1];

            pos.x -= m_vecTreeStateList[ TransferStateInClient(m_iRealCurrentState) ]->GetWidth() / 2.0f;
            pos.y -= m_vecTreeStateList[ TransferStateInClient(m_iRealCurrentState) ]->GetHeight();

            pos.x = PARTICLEPOS_X( pos.x );
            pos.y = PARTICLEPOS_Y( pos.y );

            pos.x += 0.41f;
            pos.y -= 0.33f;

            std::vector< std::string >::const_iterator vIter = mIter->second.begin();
            for( ; vIter != mIter->second.end() ; ++vIter ) { 
                g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( (*vIter).c_str(), pos.x, pos.y + 0.15f, PARTICLEPOS_Z ), GC_LAYER_UI );
            }
            g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Tree_Grow", pos.x, pos.y, PARTICLEPOS_Z ), GC_LAYER_UI );
        }

        Result_PlantTreeInfo = INT_MAX;
        KP2P::GetInstance()->Send_PlantTreeActionReq(iActionType,dwItemID);
       
        if( iBtnType == EPTB_WATER_ITEM || iBtnType == EPTB_FERTILIZERS_ITEM )
            g_KDSound.Play( "Enchant_Start" );

        return true;
    }
    return false;
}

bool KGCPlantTree::OnClickGetItemBtn( const KActionEvent &event,int iBtnType)
{
    if(event.m_pWnd == m_vecBtnList[iBtnType])
    {
        Result_PlantTreeInfo = INT_MAX;

        KP2P::GetInstance()->Send_PlantTreeRewardReq(iBtnType == EPTB_GET_TREE_ITEM ? (1) : (2));
        return true;
    }
    return false;
}


bool KGCPlantTree::OnClickOpenWorldBtn( const KActionEvent &event)
{
    if(event.m_pWnd == m_pkBtnMoveWorld)
    {
        ToggleWorldmapBtn();
        return true;
    }
    return false;
}

bool KGCPlantTree::OnClickMoveWorldBtn( const KActionEvent &event)
{
    for(std::map<int,KD3DWnd*>::iterator mIt = m_mapBtnWorldList.begin();mIt != m_mapBtnWorldList.end();mIt++)
    {
        if(event.m_pWnd == mIt->second)
        {
            g_pkUIScene->m_pkPlantTreeRewardItemListView->SetWorldBtnState(mIt->first);
            m_pkWoldMapList->SetWorldMapIndex(mIt->first);
            KEVENT_PLANT_TREE_MAP_NOT::iterator it = m_mapPlantTreeInfo.find(mIt->first);

            if(m_iRealContinentType > mIt->first)
            {
                if(it != m_mapPlantTreeInfo.end())
                {
                    SetTree(mIt->first,it->second.m_dwTotalTreeCount+1,-1);
                }
                else
                {
                    SetTree(mIt->first,0,-1);
                }
                SetBtn(-1);

            }
            else if(m_iRealContinentType == mIt->first)
            {
                SetTree(mIt->first,m_iRealCurrentPos,m_iRealCurrentState);
                SetBtn(m_iRealCurrentState);
            }
            else
            {
                SetTree(mIt->first,0,-1);
                SetBtn(-1);
            }
            ToggleWorldmapBtn();
            return true;
        }
    }
    return false;
}

void KGCPlantTree::OnClickBtn( const KActionEvent &event )
{
    if(OnClickUseItemBtn(event,EPTB_WATER_ITEM,KEVENT_PLANT_TREE_ACTION_REQ::AT_WATER_THE_TREE,KGCItemManager::ITEM_PLANTTREE_WATER))
        return;
    if(OnClickUseItemBtn(event,EPTB_FERTILIZERS_ITEM,KEVENT_PLANT_TREE_ACTION_REQ::AT_MANURE_THE_TREE,KGCItemManager::ITEM_PLANTTREE_MANURE))
        return;
    if(OnClickUseItemBtn(event,EPTB_GREEN_TREE_ITEM,KEVENT_PLANT_TREE_ACTION_REQ::AT_PLANT_TREE,0))
        return;
    if(OnClickUseItemBtn(event,EPTB_GOLD_TREE_ITEM,KEVENT_PLANT_TREE_ACTION_REQ::AT_PLANT_GOLD_TREE,KGCItemManager::ITEM_PLANTTREE_GOLDTREE))
        return;

    if(OnClickGetItemBtn(event,EPTB_GET_TREE_ITEM))
        return;
    if(OnClickGetItemBtn(event,EPTB_GET_WORLD_ITEM))
        return;

    if(OnClickOpenWorldBtn(event))
        return;
    if(OnClickMoveWorldBtn(event))
        return;

    if(OnClickBuyBtn(event))
        return;

}

bool KGCPlantTree::OnClickBuyBtn( const KActionEvent &event )
{
    if( m_pkBtnBuyWaterItem == event.m_pWnd ) {
        DWORD dwItemID = ITEM_ID_TREE_WATER;
        BuyItem( dwItemID );
        return true;
    }
    else if ( m_pkBtnBuyGoldTreeItem == event.m_pWnd ) {
        DWORD dwItemID = ITEM_ID_TREE_GOLDTREE;
        BuyItem( dwItemID ); 
        return true; 
    }
    else if ( m_pkBtnBuyFertilizersItem == event.m_pWnd ) {
        DWORD dwItemID = ITEM_ID_TREE_FERTILIZERS;
        BuyItem( dwItemID ); 
        return true;    
    }
    return false;
}

void KGCPlantTree::RecvItemList( std::vector<KItem> &kRecvItem , bool bViewItem)
{
    if(!kRecvItem.empty()){
        g_pItemMgr->m_kInventory.AddItemList( kRecvItem , true);
        if(bViewItem){
            std::vector<KItem>::iterator vIter = kRecvItem.begin();
            for(;vIter !=kRecvItem.end();vIter++)
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_TREASURE_ITEM_BOX, L"", L"",
                    KGCUIScene::GC_MBOX_USE_NORMAL, vIter->m_ItemID,0 ,false, true);
            }
        }
        g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
    }
}

void KGCPlantTree::ShowRewardParticle( int iTreeStatus )
{
    std::map<int,std::vector<std::string>>::iterator mIter = m_mapParticle.find(iTreeStatus);
    if(mIter != m_mapParticle.end()){
        std::vector<std::string>::iterator vIter = mIter->second.begin();
        for(;vIter != mIter->second.end();vIter++)
            g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( (*vIter).c_str(), 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);

        g_KDSound.Play( "SoulUp" );
    }
}

void KGCPlantTree::ToggleWorldmapBtn( void )
{
    m_pkBkWorldBtn->ToggleRender(!m_pkBkWorldBtn->IsRenderOn());
    for(std::map<int,KD3DWnd*>::iterator mIt = m_mapBtnWorldList.begin();mIt != m_mapBtnWorldList.end();mIt++)
    {
        mIt->second->ToggleRender(!mIt->second->IsRenderOn());
    }  
}