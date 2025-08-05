#include "stdafx.h"
#include "GCPlantTreeRewardItemListView.h"

IMPLEMENT_CLASSNAME( KGCPlantTreeRewardItemListView );
IMPLEMENT_WND_FACTORY( KGCPlantTreeRewardItemListView );
IMPLEMENT_WND_FACTORY_NAME( KGCPlantTreeRewardItemListView, "gc_reward_list_view_box" );

KGCPlantTreeRewardItemListView::KGCPlantTreeRewardItemListView( void )
: m_pkClose(NULL)
, m_iBtnState(0)
, m_iWorldBtnState(PTRLV_BERMESIAH)
{
    m_mapBtn.insert(std::pair<int,KD3DWnd*>(PTRLV_GOLD_TREE,NULL));
    m_mapBtn.insert(std::pair<int,KD3DWnd*>(PTRLV_GREEN_TREE,NULL));
    m_mapBtn.insert(std::pair<int,KD3DWnd*>(PTRLV_WORLD,NULL));
    m_mapBtn.insert(std::pair<int,KD3DWnd*>(PTRLV_BERMESIAH,NULL));
    m_mapBtn.insert(std::pair<int,KD3DWnd*>(PTRLV_SILVERLAND,NULL));
    m_mapBtn.insert(std::pair<int,KD3DWnd*>(PTRLV_ELLIA,NULL));
    m_mapBtn.insert(std::pair<int,KD3DWnd*>(PTRLV_ARCHIMEDIA,NULL));

    LINK_CONTROL("btn_move_2",m_mapBtn[PTRLV_GREEN_TREE]);
    LINK_CONTROL("btn_move_1",m_mapBtn[PTRLV_GOLD_TREE]);
    LINK_CONTROL("btn_move_3",m_mapBtn[PTRLV_WORLD]);
    LINK_CONTROL("btn_bermesiah",m_mapBtn[PTRLV_BERMESIAH]);
    LINK_CONTROL("btn_silverland",m_mapBtn[PTRLV_SILVERLAND]);
    LINK_CONTROL("btn_ellia",m_mapBtn[PTRLV_ELLIA]);
    LINK_CONTROL("btn_archimedia",m_mapBtn[PTRLV_ARCHIMEDIA]);

    LINK_CONTROL("btn_close",m_pkClose);
}

KGCPlantTreeRewardItemListView::~KGCPlantTreeRewardItemListView( void )
{
}

void KGCPlantTreeRewardItemListView::ActionPerformed( const KActionEvent& event )
{
    if(event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
    {
        if(event.m_pWnd == m_pkClose)
        {
            m_vecRewardItem.clear();
            ClearItemImage();
            SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
            return;
        }
        if(OnClickBtn(event))
            return;
    }
}

void KGCPlantTreeRewardItemListView::OnCreate( void )
{
    m_pkClose->SetHotKey(DIK_ESCAPE);

    for(std::map<int,KD3DWnd*>::iterator mIter = m_mapBtn.begin();mIter != m_mapBtn.end();mIter++)
    {
        mIter->second->InitState(true,true,this);
        mIter->second->Lock(false);
    }
    m_pkClose->InitState(true,true,this);
    m_iBtnState = 0;
    m_iWorldBtnState =PTRLV_BERMESIAH;

    m_mapBtn[PTRLV_GREEN_TREE]->Lock(true);
    m_mapBtn[PTRLV_BERMESIAH]->Lock(true);
}

bool KGCPlantTreeRewardItemListView::OnClickBtn( const KActionEvent& event )
{
    for(std::map<int,KD3DWnd*>::iterator mIter = m_mapBtn.begin();mIter != m_mapBtn.end();mIter++)
    {
        if(mIter->second == event.m_pWnd)
        {
            if(mIter->first <PTRLV_BERMESIAH)
                m_iBtnState = mIter->first;
            else
                m_iWorldBtnState = mIter->first;

            SetRewardSetting();
            return true;
        }
    }
    return false;
}

void KGCPlantTreeRewardItemListView::ClearItemImage()
{
    std::vector<KGC2DObject*>::iterator vIter =	m_vecItemImage.begin();

    for(;vIter != m_vecItemImage.end();vIter++)
    {
        (*vIter)->RemoveAllTexture();
        SAFE_DELETE(*vIter);
    }

    m_vecItemImage.clear();
}

void KGCPlantTreeRewardItemListView::SetRewardSetting(KEVENT_PLANT_TREE_MAP_NOT mapInfo)
{
    m_mapInfo = mapInfo;
}

void KGCPlantTreeRewardItemListView::SetRewardSetting()
{
    for(std::map<int,KD3DWnd*>::iterator mIter = m_mapBtn.begin();mIter != m_mapBtn.end();mIter++)
    {
        mIter->second->Lock(false);
        if(m_iBtnState == mIter->first || m_iWorldBtnState == mIter->first )
            mIter->second->Lock(true);
    }

    KEVENT_PLANT_TREE_MAP_NOT::iterator mapWorldIter = m_mapInfo.find(m_iWorldBtnState - PTRLV_WORLD);
    if(m_mapInfo.end() == mapWorldIter) return;

    std::map< DWORD, std::vector< KDropItemInfo > >::iterator maprewardList = mapWorldIter->second.m_mapRewardList.find(m_iBtnState);

    if(maprewardList == mapWorldIter->second.m_mapRewardList.end()) return;
    if(maprewardList->second.empty()) return;

    m_vecRewardItem.clear();
    ClearItemImage();
    
    VEC_REWARD::iterator vIter =  maprewardList->second.begin();
    for(int i = 0;vIter !=  maprewardList->second.end();vIter++, i++)
    {
        D3DXVECTOR2 pos(0.0f,0.0f);
        pos.x = static_cast<float>( IMG_START_X ) + static_cast<float>((i)%6) * 64.0f ;
        pos.y = static_cast<float>( IMG_START_Y ) - static_cast<float>(static_cast<int>((i)/6)) * 64.0f;

        KGC2DObject* pWnd = new KGC2DObject();

        pWnd->AddTexture(g_pItemMgr->CreateItemTexture(vIter->m_ItemID/10));

        pWnd->SetAxisOffset( 0.0f, 0.0f,0.0f );
        pWnd->SetSize( static_cast<float>( IMG_SIZE ) * GC_SCREEN_DIV_WIDTH, static_cast<float>( IMG_SIZE ) * GC_SCREEN_DIV_WIDTH );
        pWnd->CameraMatrixOn();
        pWnd->SetPositionPixel( pos.x * GC_SCREEN_DIV_WIDTH, pos.y * GC_SCREEN_DIV_WIDTH  );

        m_vecRewardItem.push_back(std::pair<D3DXVECTOR2,KDropItemInfo>(pos,*vIter));

        m_vecItemImage.push_back(pWnd);
    }
}

void KGCPlantTreeRewardItemListView::PostChildDraw()
{
    std::vector<KGC2DObject*>::iterator vIter =	m_vecItemImage.begin();

    for(;vIter != m_vecItemImage.end();vIter++)
    {
        (*vIter)->Render();
    }
    DrawItemInfo();

}

void KGCPlantTreeRewardItemListView::DrawItemInfo()
{
    D3DXVECTOR2 pos(0.0f,0.0f);
    GCItem* pItem = NULL;
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 mousePos(static_cast<float>(pt.x),static_cast<float>(pt.y));
    std::vector< std::pair<D3DXVECTOR2,KDropItemInfo> >::iterator vIt = m_vecRewardItem.begin();
    for(int i = 0;vIt != m_vecRewardItem.end();vIt++, i++)
    {
        pos = vIt->first;
        if(pos.x*m_fWindowScaleX <mousePos.x && pos.x*m_fWindowScaleX +60.f*m_fWindowScaleX >mousePos.x && pos.y*m_fWindowScaleY < mousePos.y && pos.y*m_fWindowScaleY+60.f*m_fWindowScaleY>mousePos.y)
        {
            pItem = g_pItemMgr->GetItemData(vIt->second.m_ItemID/10);
            pos.x += 70.f;
            break;
        }
    }


    if( pItem != NULL )
    {
        g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
        g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( pos );
        g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(D3DWZ_TOPMOST);
    }
    else
    {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
    }
}

void KGCPlantTreeRewardItemListView::InitBtnState()
{
    m_iBtnState = PTRLV_GREEN_TREE;
    m_iWorldBtnState = PTRLV_BERMESIAH;
}