#include "stdafx.h"
#include "./GCVirtualDepotDlg.h"
#include "./KGCVirtualDepotManager.h"
#include "./GCVirtualDepotChar.h"
#include "./DragManager.h"

IMPLEMENT_CLASSNAME( KGCVirtualDepotDlg );
IMPLEMENT_WND_FACTORY( KGCVirtualDepotDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCVirtualDepotDlg, "gc_virtualdepot_dlg " );

#define VIRTUAL_DEPOT_SLOT_BACK_SIZE 66.0f
#define VIRTUAL_NOTMOVE_IMG 66.0f

KGCVirtualDepotDlg::KGCVirtualDepotDlg( void )
: m_pkCloseBtn( NULL )
, m_pkCharacterInfoVD( NULL )
, m_pkCharacterBtn( NULL )
, m_pkClipRangeVD( NULL )
, m_iDepotScrollPos( 0 )
, m_iInvenScrollPos( 0 )
, m_iCurrentTab( 0 )
, m_pkDepotScrollbar( NULL )
, m_pkDepotScrollTaget( NULL )
, m_pkInvenScrollbar( NULL )
, m_pkInvenScrollTaget( NULL )
, m_pkDepotCommonTabBtn( NULL )
, m_pkDepotExclusiveTabBtn( NULL )
, m_pkCurrentDragImage( NULL )
, m_pkCurrentDragItem( NULL )
, m_pkInven_Back( NULL )
, m_pkVDepot_Back( NULL )
, m_pkMoveComplate( NULL )
, m_pkMent( NULL )
, m_pkNpcMent( NULL )
, m_pkMoveITems( NULL )
, m_pkCheck( NULL )
, m_pkWhiteBackground( NULL )
{
    LINK_CONTROL("vd_character_info", m_pkCharacterInfoVD );
    LINK_CONTROL("vd_character_sel_btn",m_pkCharacterBtn );
    LINK_CONTROL("vd_end_btn",m_pkCloseBtn);
    LINK_CONTROL("vd_clip_range",m_pkClipRangeVD);
    LINK_CONTROL("vd_depot_scroll",m_pkDepotScrollbar);
    LINK_CONTROL("vd_inven_scroll",m_pkInvenScrollbar);
    LINK_CONTROL("depot_scroll_target", m_pkDepotScrollTaget );
    LINK_CONTROL("inven_scroll_target", m_pkInvenScrollTaget );
    LINK_CONTROL("vd_depot_common", m_pkDepotCommonTabBtn );
    LINK_CONTROL("vd_depot_exclusive", m_pkDepotExclusiveTabBtn );
    LINK_CONTROL("inven_slot_background", m_pkInven_Back );
    LINK_CONTROL("depot_slot_background", m_pkVDepot_Back );
    LINK_CONTROL("vd_depot_complate", m_pkMoveComplate );
    LINK_CONTROL("depot_desc_static", m_pkMent );
    LINK_CONTROL("npc_static", m_pkNpcMent );
    LINK_CONTROL("vd_move_items", m_pkMoveITems );
    LINK_CONTROL("check", m_pkCheck );
    LINK_CONTROL( "background_white" , m_pkWhiteBackground );

    m_vecDepotBackBoxVD.clear();
    m_vecInvenBackBoxVD.clear();

    m_vecDepotItemImgListVD.clear();
    m_vecInvenItemImgListVD.clear();
    m_vecShowDepotCommonItemList.clear();
    m_vecShowDepotExclusiveItemList.clear();
    m_NotMoveImg.clear();

    m_iCurrentTab = EVDEPOT_COMMONTAB;
    SiKGCVirtualDepotManager()->SetCurrentTab( m_iCurrentTab );

    m_bMovesItem = true;
}

KGCVirtualDepotDlg::~KGCVirtualDepotDlg( void )
{
}

void KGCVirtualDepotDlg::OnCreate( void )
{
    m_pkWhiteBackground->InitState( false, false, NULL );

    m_pkCharacterInfoVD->InitState(true,false,this);
    m_pkCharacterBtn->InitState( true, true, this );
    m_pkClipRangeVD->InitState(true,true,this);

    m_pkDepotCommonTabBtn->InitState( true, true, this );
    m_pkDepotExclusiveTabBtn->InitState( true, true, this );

    m_pkInven_Back->InitState(true,false,NULL);
    m_pkVDepot_Back->InitState( true, false, NULL );

    m_pkMoveComplate->InitState( true, true, this );
    m_pkCheck->InitState(true, false, NULL);

    g_pkUIMgr->DisconnectAllEventProcedure(GetUID());
    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_DRAG_ITEM_END, this, &KGCVirtualDepotDlg::EndlDrag );

    if( m_pkCloseBtn )
    {
        m_pkCloseBtn->InitState(true,true,this);
        m_pkCloseBtn->DisconnectAllEventProcedure();
        m_pkCloseBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK,this ,&KGCVirtualDepotDlg::OnClickClose );
        m_pkCloseBtn->SetHotKey(DIK_ESCAPE);
    }

    if( m_pkMoveITems )
    {
        m_pkMoveITems->InitState(true, true, this );
        m_pkMoveITems->SetToolTip( g_pkStrLoader->GetString( STR_ID_VIRTUAL_DEPOT_TIP ), 0.0f, 0.0f, WNDUIAT_EXPANTION, ED3DWND_TOOLTIP_ALIGN_RIGHT );
        m_pkMoveITems->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK,this ,&KGCVirtualDepotDlg::OnClickMoveItems );
    }

    if( m_pkCharacterBtn )
        m_pkCharacterBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK,this ,&KGCVirtualDepotDlg::OnClickCharacterChange );

    if( m_pkMoveComplate )
        m_pkMoveComplate->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK,this ,&KGCVirtualDepotDlg::OnMoveComplate );

    InitScroll();

    m_pkMent->InitState( true );
    m_pkNpcMent->InitState( true );
    m_pkNpcMent->SetFontColor( D3DCOLOR_ARGB(255, 0, 0, 0));

    m_bMovesItem = true;
}

void KGCVirtualDepotDlg::OnCreateComplete( void )
{
    CreateVDepotUI();
    ClearDepotItemImgListVD();
    ClearInvenItemImgListVD();

    ActiveCommonTab( true );
    OnDepotScrollPos();
    OnInvenScrollPos();
    g_pkUIScene->m_pkItemInformationBox->ToggleRender(false);

    int m_iCharType = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType;
    m_pkCharacterInfoVD->SetChar( m_iCharType );
    SiKGCVirtualDepotManager()->SetCurrentChar(m_iCharType);

    m_pkMent->SetText( g_pkStrLoader->GetString( STR_ID_VIRTUAL_DEPOT_MSG4 ) );

    BackGroundEnable( true );
}

void KGCVirtualDepotDlg::FrameMoveInEnabledState( void )
{
    if( SiKGCVirtualDepotManager()->GetUpdateUI() )
    {
        UpdateShowList();
        SiKGCVirtualDepotManager()->SetUpdateUI( false );
    }

    if(SiKGCDragManager()->GetDragState() != KGCDragManager::KGDM_DRAG_PROGRESS){
        CheckDrowItemInfo();
    }
    else{
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
    }
}
void KGCVirtualDepotDlg::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == D3DWE_BUTTON_CLICK)
    {
        if( m_pkDepotCommonTabBtn == event.m_pWnd)
        {
            ActiveCommonTab( true );
        }
        else if( m_pkDepotExclusiveTabBtn == event.m_pWnd )
        {
            ActiveCommonTab( false );
        }
    }

    if( event.m_dwCode == D3DWE_RBUTTON_CLICK)
    {
        for(std::vector<std::pair<GCITEMUID,KGCCommonItemWnd*>>::iterator vit = m_vecInvenItemImgListVD.begin();vit!=m_vecInvenItemImgListVD.end();vit++)
        {
            if( vit->second == event.m_pWnd )
            {
                ActionMoveInvenToDepotRbtnClick(vit->first);
                return;
            }
        }
        for(std::vector<std::pair<GCITEMUID,KGCCommonItemWnd*>>::iterator vit = m_vecDepotItemImgListVD.begin();vit!=m_vecDepotItemImgListVD.end();vit++)
        {
            if( vit->second == event.m_pWnd )
            {
                ActionMoveDepotToInvenRbtnClick(vit->first);
                return;
            }
        }
    }

    if( event.m_dwCode == D3DWE_BUTTON_DOWN)
    {
        for(std::vector<std::pair<GCITEMUID,KGCCommonItemWnd*>>::iterator vit = m_vecInvenItemImgListVD.begin();vit!=m_vecInvenItemImgListVD.end();vit++)
        {
            if( vit->second == event.m_pWnd ){
                ActionMoveInvenToDepotDrag(vit->first, vit->second );
                return;
            }
        }
        for(std::vector<std::pair<GCITEMUID,KGCCommonItemWnd*>>::iterator vit = m_vecDepotItemImgListVD.begin();vit!=m_vecDepotItemImgListVD.end();vit++)
        {
            if( vit->second == event.m_pWnd ){
                ActionMoveDepotToInvenDrag(vit->first, vit->second );
                return;
            }
        }
    }
}
void KGCVirtualDepotDlg::InitScroll( void )
{
    if( m_pkDepotScrollbar )
    {
        m_pkDepotScrollbar->InitState(true,true,this);
        m_pkDepotScrollbar->SetScrollPos(0);
        m_pkDepotScrollbar->SetScrollRangeMin(0);
        m_pkDepotScrollbar->SetScrollRangeMax( 1 );
        m_pkDepotScrollbar->SetScrollPageSize( SiKGCVirtualDepotManager()->GetDepotHeightVD() );
        m_pkDepotScrollbar->SetTargetWnd( m_pkDepotScrollTaget );

        m_pkDepotScrollbar->ConnectEventProcedure( KD3DWnd::D3DWE_SCROLLBAR_WHEELUP, this, &KGCVirtualDepotDlg::OnDepotScrollPos );
        m_pkDepotScrollbar->ConnectEventProcedure( KD3DWnd::D3DWE_SCROLLBAR_WHEELDOWN, this, &KGCVirtualDepotDlg::OnDepotScrollPos );
        m_pkDepotScrollbar->ConnectEventProcedure( KD3DWnd::D3DWE_SCROLLBAR_DRAG, this, &KGCVirtualDepotDlg::OnDepotScrollPos );
        m_pkDepotScrollbar->ConnectEventProcedure( KD3DWnd::D3DWE_SCROLLBAR_UPBTN, this, &KGCVirtualDepotDlg::OnDepotScrollPos );
        m_pkDepotScrollbar->ConnectEventProcedure( KD3DWnd::D3DWE_SCROLLBAR_DOWNBTN, this, &KGCVirtualDepotDlg::OnDepotScrollPos );
        m_pkDepotScrollbar->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCVirtualDepotDlg::OnDepotScrollPos );
    }

    if( m_pkInvenScrollbar )
    {
        m_pkInvenScrollbar->InitState(true,false,NULL);
        m_pkInvenScrollbar->SetScrollPos(0);
        m_pkInvenScrollbar->SetScrollRangeMin(0);
        m_pkInvenScrollbar->SetScrollRangeMax(1);
        m_pkInvenScrollbar->SetScrollPageSize( SiKGCVirtualDepotManager()->GetInvenHeightVD() );
        m_pkInvenScrollbar->SetTargetWnd( m_pkInvenScrollTaget );

        m_pkInvenScrollbar->ConnectEventProcedure( KD3DWnd::D3DWE_SCROLLBAR_WHEELUP, this, &KGCVirtualDepotDlg::OnInvenScrollPos );
        m_pkInvenScrollbar->ConnectEventProcedure( KD3DWnd::D3DWE_SCROLLBAR_WHEELDOWN, this, &KGCVirtualDepotDlg::OnInvenScrollPos );
        m_pkInvenScrollbar->ConnectEventProcedure( KD3DWnd::D3DWE_SCROLLBAR_DRAG, this, &KGCVirtualDepotDlg::OnInvenScrollPos );
        m_pkInvenScrollbar->ConnectEventProcedure( KD3DWnd::D3DWE_SCROLLBAR_UPBTN, this, &KGCVirtualDepotDlg::OnInvenScrollPos );
        m_pkInvenScrollbar->ConnectEventProcedure( KD3DWnd::D3DWE_SCROLLBAR_DOWNBTN, this, &KGCVirtualDepotDlg::OnInvenScrollPos );
        m_pkInvenScrollbar->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCVirtualDepotDlg::OnInvenScrollPos );
    }
}

void KGCVirtualDepotDlg::OnClickClose( void )
{
    g_KDSound.Play( "31" );
    if( !SiKGCVirtualDepotManager()->IsEmptyMyInvenItemList() )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_VIRTUAL_DEPOT_MSG7 ) , L"", KGCUIScene::GC_MBOX_USE_VIRTUAL_INVEN_MOVEN_COMPLATE , 0, 0, false, true );
        SiKGCVirtualDepotManager()->SetCloseDlg( true );
    }
    else
        OnClose();
}

void KGCVirtualDepotDlg::OnClickCharacterChange( void )
{
    if(g_pkUIScene->m_pkCharacterSelectBox == NULL)
        return;
    if(g_pkUIScene->m_pkCharacterSelectBox->IsRenderOn())
        return;

    D3DXVECTOR2 vPos = m_pkCharacterBtn->GetFixedWindowLocalPos();

    vPos.x += 30.0f * GC_SCREEN_DIV_WIDTH;
    vPos.y += 110.0f * GC_SCREEN_DIV_WIDTH;

    g_pkUIScene->m_pkCharacterSelectBox->InitCharacterSelectBox(g_MyD3D->m_pStateMachine->GetState(), 
        boost::bind(&KGCVirtualDepotDlg::SetCharacter,this,_1), 
        boost::bind(&KGCVirtualDepotDlg::UpdateCharacterSelect,this), vPos, 
        KGCCharacterSelectBox::STATE_POSITION_DOWN,
        KGCCharacterSelectBox::EXPERT_SKILL_VIRTUAL_DEPOT,
        0, 0, SiKGCVirtualDepotManager()->GetCurrentChar() );

    g_pkUIScene->m_pkCharacterSelectBox->SetWindowPosDirect(vPos);
    g_pkUIScene->m_pkCharacterSelectBox->SetWindowZOrder(D3DWZ_TOPMOST);
    g_pkUIScene->m_pkCharacterSelectBox->ToggleRender(true);
    m_pkCharacterBtn->SetWndMode(D3DWM_LOCK);

}

void KGCVirtualDepotDlg::OnDepotScrollPos( void )
{
    int iPos = m_pkDepotScrollbar->GetScrollPos();

    if ( m_iDepotScrollPos != iPos )
    {
        m_iDepotScrollPos = iPos;
        g_KDSound.Play( "73" );

        if( m_iCurrentTab == EVDEPOT_COMMONTAB )
            SetDepotCommonItemList();
        else
            SetDepotExclusiveItemList();
    }
}

void KGCVirtualDepotDlg::OnInvenScrollPos( void )
{
    int iPos = m_pkInvenScrollbar->GetScrollPos();

    if ( m_iInvenScrollPos != iPos )
    {
        m_iInvenScrollPos = iPos;
        g_KDSound.Play( "73" );
        SetInvenItemList( );
    }
}

void KGCVirtualDepotDlg::CreateVDepotUI( void )
{
    if( m_vecDepotBackBoxVD.empty() ){
        m_vecDepotBackBoxVD = g_pkUIMgr->CreateTemplateUIListByFindName( m_pkClipRangeVD, "ui_virtualdepot.stg", "",
            "virtual_depot_dlg\\vd_depot_slot_box",true,  SiKGCVirtualDepotManager()->GetDepotMaxSlotVD());

        SetItemBoxPosition(m_vecDepotBackBoxVD,SiKGCVirtualDepotManager()->GetDepotWidthVD(), D3DXVECTOR2(4.0f, 4.0f));
    }

    if( m_vecInvenBackBoxVD.empty() ){
        m_vecInvenBackBoxVD = g_pkUIMgr->CreateTemplateUIListByFindName(m_pkClipRangeVD, "ui_virtualdepot.stg", "",
            "virtual_depot_dlg\\vd_inven_slot_box",true,  SiKGCVirtualDepotManager()->GetInvenMaxSlotVD());

        SetItemBoxPosition( m_vecInvenBackBoxVD, SiKGCVirtualDepotManager()->GetInvenWidthVD(), D3DXVECTOR2(413.0f, 4.0f));
    }

    if( m_vecDepotItemImgListVD.empty() ){
        std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName(m_pkClipRangeVD, "ui_virtualdepot.stg", "",
            "virtual_depot_dlg\\box_item",true, SiKGCVirtualDepotManager()->GetDepotMaxSlotVD() );
        SetItemBoxPosition( vecTemp,SiKGCVirtualDepotManager()->GetDepotWidthVD(),D3DXVECTOR2(5.0f, 5.0f));

        for(std::vector<KD3DWnd*>::iterator vIt = vecTemp.begin(); vIt != vecTemp.end();vIt++){
            m_vecDepotItemImgListVD.push_back(std::pair<GCITEMUID,KGCCommonItemWnd*>(0,static_cast<KGCCommonItemWnd*>(*vIt)));
        }
    }

    if( m_NotMoveImg.empty() ) {
        m_NotMoveImg = g_pkUIMgr->CreateTemplateUIListByFindName(m_pkClipRangeVD, "ui_virtualdepot.stg", "",
            "virtual_depot_dlg\\vd_depot_notmove_img",true,  SiKGCVirtualDepotManager()->GetDepotMaxSlotVD());

        SetNotMoveImg( m_NotMoveImg, SiKGCVirtualDepotManager()->GetDepotWidthVD(), D3DXVECTOR2(15.0f, 15.0f));
    }

    if(m_vecInvenItemImgListVD.empty()){
        std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName(m_pkClipRangeVD, "ui_virtualdepot.stg", "",
            "virtual_depot_dlg\\box_item",true, SiKGCVirtualDepotManager()->GetInvenMaxSlotVD() );
        SetItemBoxPosition(vecTemp,SiKGCVirtualDepotManager()->GetInvenWidthVD(),D3DXVECTOR2(414.0f, 5.0f));

        for(std::vector<KD3DWnd*>::iterator vIt = vecTemp.begin(); vIt != vecTemp.end();vIt++){
            m_vecInvenItemImgListVD.push_back(std::pair<GCITEMUID,KGCCommonItemWnd*>(0,static_cast<KGCCommonItemWnd*>(*vIt)));
        }
    }

    
    m_pkMoveComplate->ToggleRender( false );
}

void KGCVirtualDepotDlg::SetItemBoxPosition( std::vector<KD3DWnd*> &vecBox, int With,D3DXVECTOR2 pos_ )
{
    if(With == 0 ) return;

    for( int i = 0; i < (int)vecBox.size(); ++i )
    {
        D3DXVECTOR2 pos = pos_;
        pos.x += VIRTUAL_DEPOT_SLOT_BACK_SIZE * ( i % With );
        pos.y += VIRTUAL_DEPOT_SLOT_BACK_SIZE * ( i / With );
        vecBox[i]->SetWindowPosDirect(pos);
        vecBox[i]->InitState(true,true,this);
    }
}

void KGCVirtualDepotDlg::SetNotMoveImg( std::vector<KD3DWnd*> &vecBox, int With,D3DXVECTOR2 pos_ )
{
    if(With == 0 ) return;


    for( int i = 0; i < (int)vecBox.size(); ++i )
    {
        D3DXVECTOR2 pos = pos_;
        pos.x += VIRTUAL_NOTMOVE_IMG * ( i % With );
        pos.y += VIRTUAL_NOTMOVE_IMG * ( i / With );
        vecBox[i]->SetWindowPosDirect(pos);
        vecBox[i]->InitState(false,true,this);
    }
}

void KGCVirtualDepotDlg::ClearDepotItemImgListVD()
{
    for(std::vector<std::pair<GCITEMUID,KGCCommonItemWnd*>>::iterator vecIter = m_vecDepotItemImgListVD.begin();vecIter != m_vecDepotItemImgListVD.end();vecIter++)
    {
        vecIter->first = 0;
        vecIter->second->ResetItem();
    } 
}

void KGCVirtualDepotDlg::ClearInvenItemImgListVD()
{
    for(std::vector<std::pair<GCITEMUID,KGCCommonItemWnd*>>::iterator vecIter = m_vecInvenItemImgListVD.begin();vecIter != m_vecInvenItemImgListVD.end();vecIter++)
    {
        vecIter->first = 0;
        vecIter->second->ResetItem();
    } 
}


void KGCVirtualDepotDlg::ActiveCommonTab( bool bActive )
{
    m_iDepotScrollPos = 0;
    m_iInvenScrollPos = 0;
    ClearDepotItemImgListVD();

    if( bActive )
    {
        m_pkDepotCommonTabBtn->SetWndMode( D3DWM_LOCK );
        m_pkDepotExclusiveTabBtn->SetWndMode( D3DWM_DEFAULT );
        m_iCurrentTab = EVDEPOT_COMMONTAB;
        m_pkNpcMent->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_VIRTUAL_DEPOT_NEW_STR1 ) );
        m_pkNpcMent->SetFontColor( D3DCOLOR_ARGB(255, 0, 0, 0) );
        SiKGCVirtualDepotManager()->SetCurrentTab( m_iCurrentTab );
        SetDepotCommonItemList();
        SetCheckMark( m_pkDepotCommonTabBtn );

        if( m_bMovesItem == true )
        {
            m_pkMoveITems->ToggleRender( true );
            m_pkMoveITems->Activate( true );
        }
    }
    else
    {
        m_pkDepotCommonTabBtn->SetWndMode( D3DWM_DEFAULT );
        m_pkDepotExclusiveTabBtn->SetWndMode( D3DWM_LOCK );
        m_iCurrentTab = EVDEPOT_EXCLUSIVETAB;
        m_pkNpcMent->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_VIRTUAL_DEPOT_NEW_STR2 ) );
        m_pkNpcMent->SetFontColor( D3DCOLOR_ARGB(255, 255, 0, 0) );
        SiKGCVirtualDepotManager()->SetCurrentTab( m_iCurrentTab );
        SetDepotExclusiveItemList();
        SetCheckMark( m_pkDepotExclusiveTabBtn );
        m_pkMoveITems->ToggleRender( false );
        m_pkMoveITems->Activate( false );
    }
}

void KGCVirtualDepotDlg::SetDepotCommonItemList( void )
{
    m_vecShowDepotCommonItemList = SiKGCVirtualDepotManager()->GetDepotCommonItemList();

    int iScrollMaxSize = ( ( (m_vecShowDepotCommonItemList.size()-1) ) / SiKGCVirtualDepotManager()->GetDepotWidthVD() ) + 1;

    if( iScrollMaxSize < 0 || m_vecShowDepotCommonItemList.empty() )
    {
        iScrollMaxSize = 1;
    }

    if( m_iDepotScrollPos > iScrollMaxSize )
    {
        m_iDepotScrollPos = iScrollMaxSize - 1;
    }

    m_pkDepotScrollbar->SetScrollPos( m_iDepotScrollPos );
    m_pkDepotScrollbar->SetScrollRangeMax( iScrollMaxSize );
    SetShowDepotCommonItemList();

}

void KGCVirtualDepotDlg::SetShowDepotCommonItemList( void )
{
    int iInComplateSize = m_vecShowDepotCommonItemList.size();
    int iCommonSize = m_vecShowDepotCommonItemList.size();

    std::vector<GCITEMUID> vecRealShowCommonList;

    if( iInComplateSize + iCommonSize < SiKGCVirtualDepotManager()->GetDepotMaxSlotVD() )
    {
        vecRealShowCommonList = m_vecShowDepotCommonItemList;
    }
    else
    {
        std::vector<GCITEMUID>::iterator iterBeg = ( m_vecShowDepotCommonItemList.begin() + ( m_iDepotScrollPos * SiKGCVirtualDepotManager()->GetDepotWidthVD() ) );
        std::vector<GCITEMUID>::iterator iterEnd = m_vecShowDepotCommonItemList.end();

        if ( iterEnd - iterBeg >= SiKGCVirtualDepotManager()->GetDepotMaxSlotVD() )
            iterEnd = iterBeg + SiKGCVirtualDepotManager()->GetDepotMaxSlotVD();

        std::copy( iterBeg, iterEnd, std::back_inserter(vecRealShowCommonList));
    }

    std::vector<GCITEMUID>::iterator vitTemp = vecRealShowCommonList.begin();
    int showSize = m_vecDepotItemImgListVD.size();

    for( int i = 0; i < showSize; i++ )
    {
        if( vitTemp == vecRealShowCommonList.end() )
        {
            m_vecDepotItemImgListVD[i].first = 0;
            m_vecDepotItemImgListVD[i].second->ResetItem();
            m_NotMoveImg[i]->ToggleRender(false);
            continue;
        }
        else
        {
            KItem* pItem = SiKGCVirtualDepotManager()->CommonFindGetItem( *vitTemp );

            if( pItem != NULL )
            {
                m_vecDepotItemImgListVD[i].first = *vitTemp;
                SetItemImg( pItem, m_vecDepotItemImgListVD[i].second );
                m_NotMoveImg[i]->ToggleRender(false);

                bool bFind = SiKGCVirtualDepotManager()->FindDepotInComplateCommonItem( *vitTemp );
                if( bFind )
                {
                    m_vecDepotItemImgListVD[i].second->SetColor( 0x991e1e1e );
                    m_NotMoveImg[i]->ToggleRender(true);
                }
            }
        }
        vitTemp++;
    }
}



void KGCVirtualDepotDlg::SetDepotExclusiveItemList( void )
{
    m_vecShowDepotExclusiveItemList.clear();

    m_vecShowDepotExclusiveItemList = SiKGCVirtualDepotManager()->DepotExclusiveItemList();

    int iScrollMaxSize = ( (m_vecShowDepotExclusiveItemList.size()-1) / SiKGCVirtualDepotManager()->GetDepotWidthVD() ) + 1;
    if( iScrollMaxSize < 0 || m_vecShowDepotExclusiveItemList.empty() )
    {
        iScrollMaxSize = 1;
    }

    if( m_iDepotScrollPos >= iScrollMaxSize )
    {
        m_iDepotScrollPos = iScrollMaxSize - 1;
    }

    m_pkDepotScrollbar->SetScrollPos( m_iDepotScrollPos );
    m_pkDepotScrollbar->SetScrollRangeMax( iScrollMaxSize );
    SetShowDepotExclusiveItemList();
}

void KGCVirtualDepotDlg::SetShowDepotExclusiveItemList( void )
{
    int size = m_vecShowDepotExclusiveItemList.size();

    std::vector<GCITEMUID> vecRealShowCommonList;
    if( size < SiKGCVirtualDepotManager()->GetDepotMaxSlotVD() )
    {
        vecRealShowCommonList = m_vecShowDepotExclusiveItemList;
    }
    else
    {
        std::vector<GCITEMUID>::iterator iterBeg = ( m_vecShowDepotExclusiveItemList.begin() + ( m_iDepotScrollPos * SiKGCVirtualDepotManager()->GetDepotWidthVD() ) );
        std::vector<GCITEMUID>::iterator iterEnd = m_vecShowDepotExclusiveItemList.end();

        if ( iterEnd - iterBeg >= SiKGCVirtualDepotManager()->GetDepotMaxSlotVD() )
            iterEnd = iterBeg + SiKGCVirtualDepotManager()->GetDepotMaxSlotVD();

        std::copy( iterBeg, iterEnd, std::back_inserter(vecRealShowCommonList));
    }

    std::vector<GCITEMUID>::iterator vitTemp = vecRealShowCommonList.begin();
    int showSize = m_vecDepotItemImgListVD.size();

    for( int i = 0; i < showSize; i++ )
    {
        if( vitTemp == vecRealShowCommonList.end() )
        {
            m_vecDepotItemImgListVD[i].first = 0;
            m_vecDepotItemImgListVD[i].second->ResetItem();
            m_NotMoveImg[i]->ToggleRender(false);
            continue;
        }
        else
        {
            KItem* pItem = SiKGCVirtualDepotManager()->ExclusiveFindGetItem( *vitTemp );

            if( pItem != NULL )
            {
                m_vecDepotItemImgListVD[i].first = *vitTemp;
                SetItemImg( pItem, m_vecDepotItemImgListVD[i].second );
            }

        }
        vitTemp++;
    }
}


void KGCVirtualDepotDlg::SetInvenItemList( void )
{
    std::vector< KItem > vecCurrenInven = SiKGCVirtualDepotManager()->GetCurrentInvenList( );

    int iScrollMaxSize = ( ( vecCurrenInven.size()-1) / SiKGCVirtualDepotManager()->GetInvenWidthVD() ) + 1;

    if( iScrollMaxSize < 0 || vecCurrenInven.empty() )
    {
        iScrollMaxSize = 1;
    }

    if( m_iInvenScrollPos >= iScrollMaxSize )
    {
        m_iInvenScrollPos = iScrollMaxSize - 1;
    }

    m_pkInvenScrollbar->SetScrollPos( m_iInvenScrollPos );
    m_pkInvenScrollbar->SetScrollRangeMax( iScrollMaxSize );

    SetShowInvenItemList( );
}


void KGCVirtualDepotDlg::SetShowInvenItemList( void )
{

    std::vector< KItem > vecCurrenInven = SiKGCVirtualDepotManager()->GetCurrentInvenList( );

    int iCharInvenSize = vecCurrenInven.size();
    std::vector<KItem> vecRealShowInvenList;

    if( iCharInvenSize < SiKGCVirtualDepotManager()->GetInvenMaxSlotVD() )
    {
        vecRealShowInvenList = vecCurrenInven;
    }
    else
    {
        std::vector<KItem>::iterator iterBeg = ( vecCurrenInven.begin() + ( m_iInvenScrollPos * SiKGCVirtualDepotManager()->GetInvenWidthVD() ) );
        std::vector<KItem>::iterator iterEnd = vecCurrenInven.end();

        if ( iterEnd - iterBeg >= SiKGCVirtualDepotManager()->GetInvenMaxSlotVD() )
            iterEnd = iterBeg + SiKGCVirtualDepotManager()->GetInvenMaxSlotVD();

        std::copy( iterBeg, iterEnd, std::back_inserter(vecRealShowInvenList));
    }

    std::vector<KItem>::iterator vitTemp = vecRealShowInvenList.begin();
    int showSize = m_vecInvenItemImgListVD.size();

    for( int i = 0; i < showSize; i++ )
    {
        if( vitTemp == vecRealShowInvenList.end() )
        {
            m_vecInvenItemImgListVD[i].first = 0;
            m_vecInvenItemImgListVD[i].second->ResetItem();
            continue;
        }
        else
        {
            if( vitTemp != vecRealShowInvenList.end() /*!= NULL*/ )
            {
                m_vecInvenItemImgListVD[i].first = (*vitTemp).m_ItemUID;
                SetItemImg( &(*vitTemp), m_vecInvenItemImgListVD[i].second );
            }
        }
        vitTemp++;
    }
}

void KGCVirtualDepotDlg::SetCharacter( int iChar )
{
    if(SiKGCVirtualDepotManager()->GetCurrentChar() == iChar) 
        return;

    SiKGCVirtualDepotManager()->SetCurrentChar(iChar);
    m_pkCharacterBtn->SetWndMode(D3DWM_DEFAULT);

    g_pkUIScene->m_pkCharacterSelectBox->SpeakToActionListener( KActionEvent(g_pkUIScene->m_pkCharacterSelectBox, EWNDMESSAGE_CLOSE) );
}

void KGCVirtualDepotDlg::UpdateCharacterSelect( void )
{
    m_pkCharacterInfoVD->SetChar( SiKGCVirtualDepotManager()->GetCurrentChar() );
    SetInvenItemList();
    UpdateShowList();
    m_pkCharacterBtn->SetWndMode( D3DWM_DEFAULT );
}

void KGCVirtualDepotDlg::SetItemImg( KItem* kItem, KGCCommonItemWnd* pkItemImg)
{
    if( kItem == NULL )
        return;

    GCItem* pItemInfo = g_MyD3D->m_kItemMgr.GetItemData( kItem->m_ItemID );

    KPetInfo *pPet = SiKGCVirtualDepotManager()->GetPetInfo(kItem);	
    pkItemImg->SetItem( kItem, pPet );
}

void KGCVirtualDepotDlg::ActionMoveDepotToInvenRbtnClick( GCITEMUID dwItemUID )
{
    if( SiKGCVirtualDepotManager()->InvenLimitSize() >= MAX_MOVE_INVEN_MAX_SIZE )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_VIRTUAL_DEPOT_MOVE_STR), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true, true);
        return;
    }

    if(dwItemUID == 0)
        return;

    if( SiKGCVirtualDepotManager()->FindDepotInComplateCommonItem(dwItemUID) )
        return;

    KItem* pItem = NULL;
    if( m_iCurrentTab == EVDEPOT_COMMONTAB )
    {
        pItem = SiKGCVirtualDepotManager()->CommonFindGetItem( dwItemUID );
        if( NULL == pItem ) {
            return;
        }
    }
    else
    {
        pItem = SiKGCVirtualDepotManager()->ExclusiveFindGetItem( dwItemUID );
        if( NULL == pItem ) {
            return;
        }
    }


    GCItem* pItemData = g_pItemMgr->GetItemData( pItem->m_ItemID );
    if( pItemData == NULL )
        return;

    if( pItem->m_nCount > 0 ) { 
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SELL_COUNT_ITEM, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, dwItemUID, KGCSellCountItemDlg::EM_MODE_MOVE_VDEPOT_TO_INVEN, true, true );
    }
    else
    {
        SiKGCVirtualDepotManager()->MoveDepotToInvenItem( *pItem, -1, false );
    }
}

void KGCVirtualDepotDlg::ActionMoveInvenToDepotRbtnClick( GCITEMUID dwItemUID )
{
    if(dwItemUID == 0)
        return;

    KItem* pItem = SiKGCVirtualDepotManager()->InvenFindGetItem( dwItemUID );
    if( NULL == pItem ) {
        return;
    }

    GCItem* pItemData = g_pItemMgr->GetItemData( pItem->m_ItemID );
    if( pItemData == NULL )
        return;

    if( pItem->m_nCount > 0 ) { 
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SELL_COUNT_ITEM, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, dwItemUID, KGCSellCountItemDlg::EM_MODE_MOVE_INVEN_TO_VDEPOT, true, true );
    }
    else
    {
        if( m_iCurrentTab == EVDEPOT_COMMONTAB )
            SiKGCVirtualDepotManager()->MoveInvenToDepotCommon( *pItem, -1, false );
        else
            SiKGCVirtualDepotManager()->MoveInvenToDepotCharItem( *pItem, -1, false );
    }
}

void KGCVirtualDepotDlg::ActionMoveDepotToInvenDrag( GCITEMUID dwItemUID, KGCCommonItemWnd* pImage )
{
    if( SiKGCVirtualDepotManager()->InvenLimitSize() >= MAX_MOVE_INVEN_MAX_SIZE )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_VIRTUAL_DEPOT_MOVE_STR), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true, true);
        return;
    }

    if(dwItemUID == 0)
        return;

    if( SiKGCVirtualDepotManager()->FindDepotInComplateCommonItem(dwItemUID) )
        return;

    KItem* pItem = NULL;
    if( m_iCurrentTab == EVDEPOT_COMMONTAB )
    {
        pItem = SiKGCVirtualDepotManager()->CommonFindGetItem( dwItemUID );
        if( NULL == pItem ) {
            return;
        }
    }
    else
    {
        pItem = SiKGCVirtualDepotManager()->ExclusiveFindGetItem( dwItemUID );
        if( NULL == pItem ) {
            return;
        }
    }

    m_pkCurrentDragImage = pImage;
    m_pkCurrentDragImage->SetColor(0x991e1e1e);
    m_pkCurrentDragItem = pItem;

    SiKGCDragManager()->StartDrag(pItem,49.f * GC_SCREEN_DIV_WIDTH,49.f * GC_SCREEN_DIV_WIDTH, boost::bind(&KGCVirtualDepotDlg::ProgressDepotToInvenDrag,this,_1), SiKGCVirtualDepotManager()->GetPetInfo(pItem));
}

void KGCVirtualDepotDlg::ActionMoveInvenToDepotDrag( GCITEMUID dwItemUID, KGCCommonItemWnd* pImage )
{
    if(dwItemUID == 0)
        return;

    KItem* pItem = SiKGCVirtualDepotManager()->InvenFindGetItem( dwItemUID );
    if( NULL == pItem ) {
        return;
    }

    m_pkCurrentDragImage = pImage;
    m_pkCurrentDragImage->SetColor(0x991e1e1e);
    m_pkCurrentDragItem = pItem;

    SiKGCDragManager()->StartDrag(pItem,49.f * GC_SCREEN_DIV_WIDTH,49.f * GC_SCREEN_DIV_WIDTH, boost::bind(&KGCVirtualDepotDlg::ProgressInvenToDepotDrag,this,_1), SiKGCVirtualDepotManager()->GetPetInfo(pItem));
}

int KGCVirtualDepotDlg::ProgressDepotToInvenDrag( bool btnUp )
{
    if( btnUp )
    {
       if( m_pkInven_Back->CheckMousePosInWindowBound() )
       {
           if(m_pkCurrentDragItem == 0) { 
               return KGCDragManager::KGDM_DRAG_END;
           }

           if ( m_pkCurrentDragItem->CheckItemType( KItem::TYPE_LOOK )  == true && g_pItemMgr->GetCoordiInventorySize() >= g_pItemMgr->GetCoordiInventoryMaxSize() )
           {
               g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_COORDI_INVENTORY_FULL_ERROR) , L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true, false);
               return KGCDragManager::KGDM_DRAG_END;
           }

           GCItem* pItemData = g_pItemMgr->GetItemData( m_pkCurrentDragItem->m_ItemID );
           if( NULL == pItemData ) { 
               return KGCDragManager::KGDM_DRAG_END;
           }

           if( m_pkCurrentDragItem->m_nCount > 0 ) { 
               g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SELL_COUNT_ITEM, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_pkCurrentDragItem->m_ItemUID, KGCSellCountItemDlg::EM_MODE_MOVE_VDEPOT_TO_INVEN, true, true );
           }
           else
           {
               SiKGCVirtualDepotManager()->MoveDepotToInvenItem( *m_pkCurrentDragItem, -1, false );
           }
       }
       return KGCDragManager::KGDM_DRAG_END;
    }

    return KGCDragManager::KGDM_DRAG_PROGRESS;

}

int KGCVirtualDepotDlg::ProgressInvenToDepotDrag( bool btnUp )
{
    if( btnUp )
    {
        if( m_pkVDepot_Back->CheckMousePosInWindowBound() )
        {
            GCItem* pItemData = g_pItemMgr->GetItemData( m_pkCurrentDragItem->m_ItemID );
            if( pItemData == NULL )
                return KGCDragManager::KGDM_DRAG_END;

            if( m_pkCurrentDragItem->m_nCount > 0 )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SELL_COUNT_ITEM, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_pkCurrentDragItem->m_ItemUID, KGCSellCountItemDlg::EM_MODE_MOVE_INVEN_TO_VDEPOT, true, true );
            }
            else
            {
                if( m_iCurrentTab == EVDEPOT_COMMONTAB )
                    SiKGCVirtualDepotManager()->MoveInvenToDepotCommon( *m_pkCurrentDragItem, -1, false );
                else
                    SiKGCVirtualDepotManager()->MoveInvenToDepotCharItem( *m_pkCurrentDragItem, -1, false );
            }
        }
        return KGCDragManager::KGDM_DRAG_END;
    }
    return KGCDragManager::KGDM_DRAG_PROGRESS;
}

void KGCVirtualDepotDlg::EndlDrag( void )
{
    if(m_pkCurrentDragImage!=NULL)
        m_pkCurrentDragImage->SetColor(0xffffffff);
    m_pkCurrentDragImage = NULL;
    m_pkCurrentDragItem = NULL;
}

void KGCVirtualDepotDlg::UpdateShowList( void )
{
    if( m_iCurrentTab == EVDEPOT_COMMONTAB )
        SetDepotCommonItemList();
    else
        SetDepotExclusiveItemList();

    SetInvenItemList();

    bool bEmptyInven = SiKGCVirtualDepotManager()->IsEmptyMyInvenItemList();

    if( bEmptyInven )
    {
        m_pkMoveComplate->ToggleRender( false );
        m_pkMoveComplate->Activate( false );
    }
    else
    {
        m_pkMoveComplate->ToggleRender( true );
        m_pkMoveComplate->Activate( true );
    }

    if( m_bMovesItem == true )
    {
        if( m_iCurrentTab == EVDEPOT_COMMONTAB )
        {
            m_pkMoveITems->ToggleRender( true );
            m_pkMoveITems->Activate( true );
        }
    }
}

void KGCVirtualDepotDlg::OnMoveComplate( void )
{
    g_KDSound.Play( "31" );
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_VIRTUAL_DEPOT_MSG6 ) , L"", KGCUIScene::GC_MBOX_USE_VIRTUAL_INVEN_MOVEN_COMPLATE , 0, 0, false, true );

    m_bMovesItem = true;
}

void KGCVirtualDepotDlg::CheckDrowItemInfo( void )
{
    for ( int i = 0; i < (int)m_vecDepotItemImgListVD.size(); ++i )
    {
        m_vecDepotItemImgListVD[i].second->SelectedItem( false );
    }

    for ( int i = 0; i < (int)m_vecInvenItemImgListVD.size(); ++i )
    {
        m_vecInvenItemImgListVD[i].second->SelectedItem( false );
    }


    for(int i = 0; i <SiKGCVirtualDepotManager()->GetDepotMaxSlotVD();++i){
        if(m_vecDepotItemImgListVD[i].first != 0){
            if(m_vecDepotBackBoxVD[i]->CheckMousePosInWindowBound())
            {
                if( m_iCurrentTab == EVDEPOT_COMMONTAB )
                    DrawItemInfo(SiKGCVirtualDepotManager()->CommonFindGetItem(m_vecDepotItemImgListVD[i].first)
                    ,m_vecDepotBackBoxVD[i]->GetCurrentWindowPos());
                else
                    DrawItemInfo(SiKGCVirtualDepotManager()->ExclusiveFindGetItem(m_vecDepotItemImgListVD[i].first)
                    ,m_vecDepotBackBoxVD[i]->GetCurrentWindowPos());

                m_vecDepotItemImgListVD[i].second->SelectedItem( true );
                return;
            }
        }
    }

    for(int i = 0; i <SiKGCVirtualDepotManager()->GetInvenMaxSlotVD();++i){
        if(m_vecInvenItemImgListVD[i].first != 0){
            if(m_vecInvenBackBoxVD[i]->CheckMousePosInWindowBound())
            {
                DrawItemInfo( SiKGCVirtualDepotManager()->InvenFindGetItem(m_vecInvenItemImgListVD[i].first)
                    ,m_vecInvenBackBoxVD[i]->GetCurrentWindowPos());

                m_vecInvenItemImgListVD[i].second->SelectedItem( true );
                return;
            }
        }
    }

    g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
}

void KGCVirtualDepotDlg::DrawItemInfo(KItem* pItem ,D3DXVECTOR2 vPos)
{
    if( pItem != NULL )
    {
        vPos.x += 49.f * GC_SCREEN_DIV_WIDTH;
        vPos.y += 20.f * GC_SCREEN_DIV_WIDTH;
        vPos.x *= g_pGCDeviceManager2->GetWindowScaleX();
        vPos.y *= g_pGCDeviceManager2->GetWindowScaleY();
        GCItem* pGCItem = g_pItemMgr->GetItemData(pItem->m_ItemID);

        g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pGCItem, -1, pItem );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
        g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vPos );
        g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(D3DWZ_TOPMOST);
    }
    else
    {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
    }
}

void KGCVirtualDepotDlg::OnClose( void )
{
    if( g_pkUIScene->m_pkCharacterSelectBox->IsRenderOn()){
        g_pkUIScene->m_pkCharacterSelectBox->SpeakToActionListener( KActionEvent( g_pkUIScene->m_pkCharacterSelectBox, KD3DWnd::EWNDMESSAGE_CLOSE ) );
    }

    SiKGCVirtualDepotManager()->ClearMyInvenItem();
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
}

void KGCVirtualDepotDlg::OnClickMoveItems( void )
{
    m_iDepotScrollPos = 0;
    m_iDepotScrollPos = 0;
    SiKGCVirtualDepotManager()->LimitMoveItems();
    m_pkMoveITems->ToggleRender( false );
    m_pkMoveITems->Activate( false );
    m_bMovesItem = false;
}


void KGCVirtualDepotDlg::SetCheckMark( KD3DWnd *pkBaseWnd ) 
{
    float xpos = pkBaseWnd->GetFixedWindowLocalPos().x + 2.f;
    xpos += static_cast<float>(pkBaseWnd->GetWidth()) / 2.f - static_cast<float>(m_pkCheck->GetWidth()) / 2.f;
    m_pkCheck->SetWindowPosXDirect(xpos);
}

void KGCVirtualDepotDlg::BackGroundEnable( bool bRender /*= true */ )
{
    m_pkWhiteBackground->ForceSetWindowColor( D3DCOLOR_ARGB( 180, 0x00, 0x00, 0x00 ) );
    m_pkWhiteBackground->ToggleRender( bRender );
}

void KGCVirtualDepotDlg::OnDestroy()
{
    BackGroundEnable( false );
}