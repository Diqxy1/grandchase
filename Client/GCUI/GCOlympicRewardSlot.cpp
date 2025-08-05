#include "stdafx.h"
#include "GCOlympicRewardSlot.h"

IMPLEMENT_CLASSNAME( KGCOlympicRewardSlot );
IMPLEMENT_WND_FACTORY( KGCOlympicRewardSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCOlympicRewardSlot, "gc_olympic_reward_slot" );

#define ACTIVE_COLOR    D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f )
#define LOCK_COLOR      D3DXCOLOR( 0.8f, 0.8f, 0.8f, 0.8f )

KGCOlympicRewardSlot::KGCOlympicRewardSlot( void )
: m_dwScore( 0 )
, m_bActive( false )
, m_bCanRecv( false )
, m_bShowItemData( false )
{
    m_pkItemBack = NULL;
    m_pkItemRecvFrame = NULL;
    m_pkBtnRecv = NULL;
    m_pkStaticScore = NULL;
    m_pkItemImg = NULL;
    m_pkShadow = NULL;

    LINK_CONTROL( "item_back",      m_pkItemBack );
    LINK_CONTROL( "recv_frame",     m_pkItemRecvFrame );
    LINK_CONTROL( "item_img",       m_pkItemImg );
    LINK_CONTROL( "static_score",   m_pkStaticScore );
    LINK_CONTROL( "btn_recv",       m_pkBtnRecv );
    LINK_CONTROL( "shadow",         m_pkShadow );

    m_vecItemInfo.clear();
}

KGCOlympicRewardSlot::~KGCOlympicRewardSlot( void )
{
}

void KGCOlympicRewardSlot::OnCreate( void )
{
    m_pkItemBack->InitState( false );
    m_pkItemRecvFrame->InitState( false );
    m_pkItemImg->InitState( true, true, this );
    
    m_pkStaticScore->InitState( false );
    m_pkStaticScore->SetAlign( DT_CENTER );

    m_pkBtnRecv->InitState( false, true, this );
    m_pkBtnRecv->DisconnectAllEventProcedure();
    m_pkBtnRecv->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCOlympicRewardSlot::OnClickRecv );

    m_pkShadow->InitState( false );

    m_dwScore = 0;
    m_bActive = false;
    m_bCanRecv = false;
    m_vecItemInfo.clear();
}

void KGCOlympicRewardSlot::ActionPerformed( const KActionEvent& event ) 
{
    if( m_vecItemInfo.empty() ) { 
        return;
    }

    if( event.m_pWnd == m_pkItemImg && event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER ) { 
        m_bShowItemData = true;
    }

    if( event.m_pWnd == m_pkItemImg && event.m_dwCode == KD3DWnd::D3DWE_CURSOR_LEAVE ) { 
        m_bShowItemData = false;
    }
}

void KGCOlympicRewardSlot::FrameMoveInEnabledState( void ) {}

void KGCOlympicRewardSlot::ClearRewadSlot( void )
{
    m_dwScore = 0;
    m_bActive = false;
    m_bShowItemData = false;
    m_vecItemInfo.clear();

    m_pkItemBack->ToggleRender( false );
    m_pkItemRecvFrame->ToggleRender( false );
    m_pkItemImg->SetItemInfo( KItem::INVALID_ITEM );
    m_pkItemImg->ToggleRender( false );
    m_pkStaticScore->ToggleRender( false );
    m_pkStaticScore->ToggleRender( false );
    m_pkShadow->ToggleRender( false );
}

void KGCOlympicRewardSlot::SetRewardSlot( const IN std::vector< KDropItemInfo >& vecReward_, const DWORD dwScore_, const bool bActive_, const bool bCanRecv_ )
{
    ClearRewadSlot();

    if( vecReward_.empty() ) { 
        return;
    }

    GCItem* pItemData = g_pItemMgr->GetItemData( vecReward_[0].m_ItemID / 10 );
    if( pItemData == NULL ) { 
        return;
    }

    m_vecItemInfo = vecReward_;

    m_dwScore = dwScore_;
    m_bActive = bActive_;
    m_bCanRecv = bCanRecv_;

    m_pkItemBack->ToggleRender( true );
    //if( m_bActive ) { 
    //    m_pkItemBack->SetWindowColor( ACTIVE_COLOR );
    //}
    //else {
    //    m_pkItemBack->SetWindowColor( LOCK_COLOR );
    //}

    m_pkShadow->ToggleRender( !m_bActive );

    m_pkItemImg->ToggleRender( true );
    //m_pkItemImg->SetItemInfo( pItemData->dwGoodsID, m_bActive ? ACTIVE_COLOR : LOCK_COLOR );
    m_pkItemImg->SetItemInfo( pItemData->dwGoodsID );
    m_pkItemImg->ShowItem( true );

    m_pkStaticScore->ToggleRender( true );
    m_pkStaticScore->SetNumber( m_dwScore );

    m_pkItemRecvFrame->ToggleRender( m_bCanRecv );
    m_pkBtnRecv->ToggleRender( m_bCanRecv );
}

GCITEMID KGCOlympicRewardSlot::GetRewardItemID( void )
{
    if( m_vecItemInfo.empty() ) { 
        return 0;
    }
    else { 
        return m_vecItemInfo[0].m_ItemID;
    }
}

void KGCOlympicRewardSlot::OnClickRecv( void )
{
    if( !m_bCanRecv ) { 
        return;
    }

    KEVENT_OLYMPIC_REWARD_REQ kPacket_;

    DWORD dwRewardRank = 0;
    if ( SiGCOlympicManager()->GetRewardGradeFromScore( m_dwScore, dwRewardRank ) ){

        kPacket_.m_dwGrade = dwRewardRank;
        kPacket_.m_vecReward = m_vecItemInfo;

        KP2P::GetInstance()->Send_OlympicGradeRewardReq( kPacket_ );
    }
}

void KGCOlympicRewardSlot::PostChildDraw( void )
{
    if( false == m_bShowItemData ) {
        return;
    }

    ShowItemData();
}

void KGCOlympicRewardSlot::ShowItemData( void )
{
    if( m_vecItemInfo.empty() ) { 
        return;
    }

    GCItem* pItemData = g_pItemMgr->GetItemData( m_vecItemInfo[0].m_ItemID / 10 );
    if( pItemData == NULL ) { 
        return;
    }

    D3DXVECTOR2 dxvPos;
    dxvPos = GetCurrentWindowLocalPos();

    //float fX = 152.0f;
    //float fY = 78.0f;

    //KLuaManager luaMgr;
    //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
    //LUA_GET_VALUE_DEF( "fX", fX, 0.0f );
    //LUA_GET_VALUE_DEF( "fY", fY, 0.0f );

    dxvPos.x += ( 152.0f * g_pGCDeviceManager->GetWindowScaleX() );
    dxvPos.y += ( 78.0f * g_pGCDeviceManager->GetWindowScaleX() );

    g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
    g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItemData );
    g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
    g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( dxvPos );
    g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( D3DWZ_TOPMOST );
}