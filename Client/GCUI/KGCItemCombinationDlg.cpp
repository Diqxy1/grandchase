#include "stdafx.h"
#include "KGCItemCombinationDlg.h"
#include "GCItemCombinationManager.h"
#include "gcui/KGCItemCombinationStuff.h"

IMPLEMENT_CLASSNAME( KGCItemCombinationDlg );
IMPLEMENT_WND_FACTORY( KGCItemCombinationDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCItemCombinationDlg, "gc_item_combination_dlg" );

#define DLG_SLOT_SIZE (51.0f * GC_SCREEN_DIV_WIDTH)

KGCItemCombinationDlg::KGCItemCombinationDlg( void )
: m_pkBtnClose(NULL)
, m_pkAssembleBtn(NULL)
, m_pkDescStatic(NULL)
, m_pkBtnRewardPre(NULL)
, m_pkBtnRewardNext(NULL)
, m_pkBtnStuffPre(NULL)
, m_pkBtnStuffNext(NULL)
, m_pkBaloonDesc(NULL)
, m_nIndex(0)
, m_RewardItemIndex(0)
, m_iParticleTime(0)
{
    m_vecRewardItemList.clear();
    m_vecStuffItemList.clear();
    m_vecStuffSlot.clear();
    m_mapStuffItemUIDList.clear();

    LINK_CONTROL( "btnColse", m_pkBtnClose );
    LINK_CONTROL( "assemble_btn", m_pkAssembleBtn );
    LINK_CONTROL( "btn_reward_previous" , m_pkBtnRewardPre );
    LINK_CONTROL( "btn_reward_next" , m_pkBtnRewardNext );
    LINK_CONTROL( "btn_stuff_previous" , m_pkBtnStuffPre );
    LINK_CONTROL( "btn_stuff_next" , m_pkBtnStuffNext );
    LINK_CONTROL( "desc_static", m_pkDescStatic );
    LINK_CONTROL( "ballon_desc_static", m_pkBaloonDesc );

    char str[MAX_PATH] = {0,};

    for ( int i = 0; i < EM_STUFF_SLOT_SIZE; ++i ) 
    {
        sprintf( str, "item_stuff_img%02d", i );  
        LINK_CONTROL( str, m_pkStuffSlot[i] );
    }

    for ( int i = 0; i < EM_STUFF_LIST_SLOT_SIZE; ++i ) 
    {
        sprintf( str, "item_stuff_list_img_%02d", i );
        LINK_CONTROL( str, m_pkStuffListSlot[i] );
    }

    for ( int i = 0; i < EM_REWARD_SLOT_SIZE; ++i ) 
    {
        sprintf( str, "item_reward_img%02d", i );
        LINK_CONTROL( str, m_pkRewardSlot[i] );
    }

}

KGCItemCombinationDlg::~KGCItemCombinationDlg( void )
{
}

void KGCItemCombinationDlg::OnCreate( void )
{
    m_pkBtnClose->SetHotKey(DIK_ESCAPE);
    m_pkBtnClose->InitState( true, true, this);
    m_pkBtnClose->DisconnectAllEventProcedure();
    m_pkBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCItemCombinationDlg::OnClickClose );

    m_pkAssembleBtn->InitState( true, true, this );
    m_pkAssembleBtn->DisconnectAllEventProcedure();
    m_pkAssembleBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCItemCombinationDlg::OnClickAssembleBtn );

    m_pkDescStatic->InitState( true, true, this );
    m_pkBaloonDesc->InitState( true, true, this );
    m_pkBaloonDesc->SetFontColor(D3DCOLOR_ARGB( 255, 0, 0, 0));
    m_pkDescStatic->SetAlign( DT_CENTER );
    m_pkDescStatic->SetText(L"");
    m_pkBaloonDesc->SetText(L"");

    m_pkBtnRewardPre->InitState( true, true, this );
    m_pkBtnRewardNext->InitState( true, true, this );
    m_pkBtnStuffPre->InitState( true, true, this );
    m_pkBtnStuffNext->InitState( true, true, this );

    for ( int i = 0; i < EM_STUFF_SLOT_SIZE; ++i ) 
    {
        m_pkStuffSlot[i]->InitState( true, true, this );
    }

    for ( int i = 0; i < EM_STUFF_LIST_SLOT_SIZE; ++i ) 
    {
        m_pkStuffListSlot[i]->InitState( true, true, this );
        m_pkStuffListSlot[i]->SetSelect( false );
    }

    for ( int i = 0; i < EM_REWARD_SLOT_SIZE; ++i ) 
    {
        m_pkRewardSlot[i]->InitState( true, true, this );
    }
}

void KGCItemCombinationDlg::FrameMoveInEnabledState( void )
{
    CheckParticleTime();

    if ( SiGCItemCombinationManager()->GetEndParticleTime() )
    {
        SiGCItemCombinationManager()->Send_ItemComposeReq();
        SiGCItemCombinationManager()->SetEndParticleTime( false );
    }

    const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

    bool bCheck = false;
    for(int i = 0; i < EM_REWARD_SLOT_SIZE; ++i)
    {
        if( m_pkRewardSlot[i]->CheckPosInWindowBound( vMousePos ) && (int)m_pkRewardSlot.size() > i && (i + m_RewardItemIndex) < static_cast<int> ( m_vecRewardItemList.size()) )
        {

            D3DXVECTOR2 vecPos =m_pkRewardSlot[i]->GetCurrentWindowPos();

            vecPos.x *= m_fWindowScaleX;
            vecPos.y *= m_fWindowScaleY;
            vecPos.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth() * g_pkUIScene->m_pkItemInformationBox->GetWindowScaleX();

            GCItem* pItem = g_pItemMgr->GetItemData( m_pkRewardSlot[i]->GetItemID() );

            g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
            g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
            g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

            g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
            bCheck = true;
            break;
        }
    }

    for(int i = 0; i < EM_STUFF_LIST_SLOT_SIZE; ++i)
    {
        if( m_pkStuffListSlot[i]->CheckPosInWindowBound( vMousePos ) && (int)m_pkStuffListSlot.size() > i && (i + m_nIndex) < static_cast<int> ( m_vecStuffItemList.size()) )
        {

            D3DXVECTOR2 vecPos =m_pkStuffListSlot[i]->GetCurrentWindowPos();

            vecPos.x *= m_fWindowScaleX;
            vecPos.y *= m_fWindowScaleY;
            vecPos.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth() * g_pkUIScene->m_pkItemInformationBox->GetWindowScaleX();

            GCItem* pItem = g_pItemMgr->GetItemData( m_pkStuffListSlot[i]->GetItemID() );
            KItem* pInvenItem = NULL;
            g_pItemMgr->FindInventory( m_pkStuffListSlot[i]->GetItemUID(), &pInvenItem );

            g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem, -1, pInvenItem );
            g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
            g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

            g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
            bCheck = true;
            break;
        }
    }

    if( !bCheck )
    {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
    }

}

void KGCItemCombinationDlg::InitData( void )
{
    m_nIndex = 0;
    m_RewardItemIndex = 0;
    m_iParticleTime = 0;
    m_vecStuffItemList.clear();
    m_vecStuffSlot.clear();
    m_vecRewardItemList.clear();
    m_mapStuffItemUIDList.clear();
    m_pkAssembleBtn->Lock( true );
    ButtonLock( false );
    SiGCItemCombinationManager()->SetStuffSelect( true );

    m_pkBaloonDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_ITEM_COMBINATION_BALOON_STR3 ) );
    std::vector<std::pair< GCITEMID, GCITEMUID>> vecTemp;
    vecTemp.clear();

    SiGCItemCombinationManager()->GetRewardItemList( m_vecRewardItemList );
    SiGCItemCombinationManager()->MakeMaterialList( vecTemp );

    std::vector<std::pair<GCITEMID, GCITEMUID>>::iterator vit = vecTemp.begin();
    int i =0;
    for( ; vit != vecTemp.end() ; vit++ )
    {
        m_vecStuffItemList.push_back(std::make_pair( std::make_pair( (*vit).first, (*vit).second ) , false ));
        m_mapStuffItemUIDList.insert( std::make_pair( i, (*vit).second ) );
        i++;
    }
    
    UpdateStuffList();
    UpdateStuff();
    UpdateRewardItemList();
}

void KGCItemCombinationDlg::OnCreateComplete( void )
{
    InitData();
    m_pkDescStatic->SetText( g_pkStrLoader->GetString(STR_ID_ITEM_COMBINATION_REWARD_DESC) );
}

void KGCItemCombinationDlg::UpdateStuffList( void )
{
    if ( m_nIndex >= (int)m_vecStuffItemList.size() )
    {
        m_nIndex = 0;
    }

    for ( int i = 0; i < EM_STUFF_LIST_SLOT_SIZE; ++i )
    {
        m_pkStuffListSlot[i]->SetShowItem( false );
        m_pkStuffListSlot[i]->SetSelect( false );
    }

    int nSlotIndex = 0;
    for ( int i = m_nIndex; i < m_nIndex + EM_STUFF_LIST_SLOT_SIZE; ++i ) 
    {
        if ( i >= (int)m_vecStuffItemList.size() ) 
            break;

        m_pkStuffListSlot[nSlotIndex]->SetItemInfo( m_vecStuffItemList[i].first.first, m_vecStuffItemList[i].first.second );
        m_pkStuffListSlot[nSlotIndex]->SetSelect( m_vecStuffItemList[i].second );
        nSlotIndex++;
    }
}

void KGCItemCombinationDlg::UpdateStuff()
{
    for ( int i = 0; i < EM_STUFF_SLOT_SIZE; ++i )
    {
        m_pkStuffSlot[i]->SetShowItem( false );
        if ( i >= static_cast<int> ( SiGCItemCombinationManager()->GetMaterialListNum() ) )
            m_pkStuffSlot[i]->SetBlockWnd( true );
    }

    for ( int i = 0; i < (int)m_vecStuffSlot.size(); ++i )
    {
        if ( i >= static_cast<int> ( SiGCItemCombinationManager()->GetMaterialListNum() ) ) 
            break;

        m_pkStuffSlot[i]->SetItemInfo( m_vecStuffSlot[i].first );		
    }

    if ( static_cast<int> ( SiGCItemCombinationManager()->GetMaterialListNum() ) != static_cast<int> ( m_vecStuffSlot.size() ) )
    {
        m_pkAssembleBtn->Lock( true );
    }
    else
    {
        m_pkAssembleBtn->Lock( false );
    }
}

void KGCItemCombinationDlg::UpdateRewardItemList( void )
{
    if ( m_RewardItemIndex >= (int)m_vecRewardItemList.size() )
    {
        m_RewardItemIndex = 0;
    }

    for ( int i = 0; i < EM_REWARD_SLOT_SIZE; ++i )
    {
        m_pkRewardSlot[i]->ShowItem( false );
    }

    int nSlotIndex = 0;
    for ( int i = m_RewardItemIndex; i < m_RewardItemIndex + EM_REWARD_SLOT_SIZE; ++i ) 
    {
        if ( i >= (int)m_vecRewardItemList.size() ) 
            break;

        m_pkRewardSlot[nSlotIndex]->SetItemInfo( m_vecRewardItemList[i] / 10 );
        m_pkRewardSlot[nSlotIndex]->ShowItem( true );
        nSlotIndex++;
    }
}

void KGCItemCombinationDlg::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnRewardPre,		KD3DWnd::D3DWE_BUTTON_CLICK, OnRewawrdPrevMove );
    _GCWND_MSG_MAP( m_pkBtnRewardNext,		KD3DWnd::D3DWE_BUTTON_CLICK, OnRewardNextMove );
    _GCWND_MSG_MAP( m_pkBtnStuffPre,		KD3DWnd::D3DWE_BUTTON_CLICK, OnStuffPrevMove );
    _GCWND_MSG_MAP( m_pkBtnStuffNext,		KD3DWnd::D3DWE_BUTTON_CLICK, OnStuffNextMove );

    for ( int i = 0; i < EM_STUFF_SLOT_SIZE; ++i ) 
    {
        if( event.m_pWnd == m_pkStuffSlot[i] && event.m_dwCode == D3DWE_BUTTON_CLICK && SiGCItemCombinationManager()->GetStuffSelect() )
        {
            OnClickStuffSlot(i);
        }
    }


    for ( int i = 0; i < EM_STUFF_LIST_SLOT_SIZE; ++i ) 
    {
        if( event.m_pWnd == m_pkStuffListSlot[i] && event.m_dwCode == D3DWE_BUTTON_CLICK && SiGCItemCombinationManager()->GetStuffSelect()  )
        {
            OnClickStuffListSlot(i);
        }
    }
}

void KGCItemCombinationDlg::OnClickStuffListSlot( int nSlot )
{
    m_pkBaloonDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_ITEM_COMBINATION_BALOON_STR1 ) );

    int nIndex = m_nIndex + nSlot;

    if ( nIndex >= static_cast<int>( m_vecStuffItemList.size() ) ||
        m_vecStuffSlot.size() >= EM_STUFF_SLOT_SIZE ||
        m_vecStuffItemList[nIndex].second )
        return;

    if (static_cast<int> ( m_vecStuffSlot.size()) >= SiGCItemCombinationManager()->GetMaterialListNum())
        return;

    m_vecStuffItemList[ nIndex ].second = true;
    m_vecStuffSlot.push_back( std::make_pair ( m_vecStuffItemList[nIndex].first.first, nIndex ) );

    UpdateStuffList();
    UpdateStuff();
}

void KGCItemCombinationDlg::OnClickStuffSlot( int nSlot )
{
    m_pkBaloonDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_ITEM_COMBINATION_BALOON_STR2 ) );
    
    if ( nSlot >= (int)m_vecStuffSlot.size() )
        return;
    
    int iStuffItemListIndex = m_vecStuffSlot[nSlot].second;

    if ( iStuffItemListIndex >= static_cast<int>(m_vecStuffItemList.size() ))
        return;

    m_vecStuffItemList[ iStuffItemListIndex ].second = false;

    m_vecStuffSlot.erase( m_vecStuffSlot.begin() + nSlot );

    UpdateStuffList();
    UpdateStuff();
}

void KGCItemCombinationDlg::OnRewawrdPrevMove()
{
    if( m_RewardItemIndex < EM_REWARD_SLOT_SIZE )
        m_RewardItemIndex = ( static_cast<int>(m_vecRewardItemList.size()) - 1 ) / EM_REWARD_SLOT_SIZE * EM_REWARD_SLOT_SIZE;
    else
        m_RewardItemIndex-=EM_REWARD_SLOT_SIZE;

    UpdateRewardItemList();
}

void KGCItemCombinationDlg::OnRewardNextMove()
{
    if( m_RewardItemIndex + EM_REWARD_SLOT_SIZE >= static_cast<int>(m_vecRewardItemList.size()) ||
        static_cast<int>(m_vecRewardItemList.size()) < EM_REWARD_SLOT_SIZE )
        m_RewardItemIndex = 0;
    else
        m_RewardItemIndex+=EM_REWARD_SLOT_SIZE;

    UpdateRewardItemList();
}

void KGCItemCombinationDlg::OnStuffPrevMove()
{
    if( m_nIndex < EM_STUFF_LIST_SLOT_SIZE )
        m_nIndex = ( static_cast<int>(m_vecStuffItemList.size()) - 1 ) / EM_STUFF_LIST_SLOT_SIZE * EM_STUFF_LIST_SLOT_SIZE;
    else
        m_nIndex-=EM_STUFF_LIST_SLOT_SIZE;

    UpdateStuffList();
}

void KGCItemCombinationDlg::OnStuffNextMove()
{
    if( m_nIndex + EM_STUFF_LIST_SLOT_SIZE >= static_cast<int>(m_vecStuffItemList.size()) ||
        static_cast<int>(m_vecStuffItemList.size()) < EM_STUFF_LIST_SLOT_SIZE )
        m_nIndex = 0;
    else
        m_nIndex+=EM_STUFF_LIST_SLOT_SIZE;

    UpdateStuffList();
}

void KGCItemCombinationDlg::OnClickClose()
{
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCItemCombinationDlg::OnClickAssembleBtn()
{
    std::vector<GCITEMUID> vecItemUID;
    vecItemUID.clear();

    std::vector<PAIR_STUFF_AND_INDEX>::iterator vit = m_vecStuffSlot.begin();
    for ( ; vit != m_vecStuffSlot.end() ; vit++)
    {
        std::map<int , GCITEMUID>::iterator mit = m_mapStuffItemUIDList.find( (*vit).second);
        if ( mit != m_mapStuffItemUIDList.end() )
            vecItemUID.push_back( (*mit).second );
    }

    if ( !vecItemUID.empty() ) {
        ShowEffect();
        m_pkAssembleBtn->Lock( true );
        ButtonLock( true );
        SiGCItemCombinationManager()->SetResultItemUID( vecItemUID );
    }
}

void KGCItemCombinationDlg::ButtonLock(bool bBtnLock)
{
    m_pkBtnRewardNext->Lock( bBtnLock );
    m_pkBtnRewardPre->Lock( bBtnLock );
    m_pkBtnStuffNext->Lock( bBtnLock );
    m_pkBtnStuffPre->Lock( bBtnLock );
}

void KGCItemCombinationDlg::ShowEffect()
{
    m_iParticleTime = 50;

    g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Ui_MariCard_01", 0.015f, 0.0f, 0.5f), GC_LAYER_UI);
    g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Ui_MariCard_02", 0.015f, 0.0f, 0.5f), GC_LAYER_UI);
    g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Ui_MariCard_03", 0.015f, 0.0f, 0.5f), GC_LAYER_UI);
    g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Ui_MariCard_04", 0.015f, 0.0f, 0.5f), GC_LAYER_UI);
    g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Ui_MariCard_05", 0.015f, 0.0f, 0.5f), GC_LAYER_UI);
    g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Ui_MariCard_06", 0.015f, 0.0f, 0.5f), GC_LAYER_UI);
    g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Ui_MariCard_07", 0.015f, 0.0f, 0.5f), GC_LAYER_UI);
    g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Ui_MariCard_08", 0.015f, 0.0f, 0.5f), GC_LAYER_UI);
    g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Ui_MariCard_09", 0.015f, 0.0f, 0.5f), GC_LAYER_UI);
}

void KGCItemCombinationDlg::CheckParticleTime()
{
    if( m_iParticleTime == 0)
        return;

    SiGCItemCombinationManager()->SetStuffSelect( false );
    m_iParticleTime--;

    if( m_iParticleTime == 0 )
    {
        SiGCItemCombinationManager()->SetEndParticleTime( true );
    }
}