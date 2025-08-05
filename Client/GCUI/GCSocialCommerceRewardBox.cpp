#include "stdafx.h"
#include "GCSocialCommerceRewardBox.h"
#include "GCSocialCommerceManager.h"


IMPLEMENT_CLASSNAME( KGCSocialCommerceRewardBox );
IMPLEMENT_WND_FACTORY( KGCSocialCommerceRewardBox );
IMPLEMENT_WND_FACTORY_NAME( KGCSocialCommerceRewardBox, "gc_social_commerce_reward_box" );

KGCSocialCommerceRewardBox::KGCSocialCommerceRewardBox( void )
{
    m_pkBtnClose = NULL;

    m_pkItemImgGoods = NULL;
    m_pkItemImgReward = NULL;

    m_pkStaticItemNameGoods = NULL;
    m_pkStaticItemNameReward = NULL;
    m_pkStaticTitleDesc = NULL;
    m_pkStaticDesc = NULL;

    LINK_CONTROL( "btn_close", m_pkBtnClose );

    LINK_CONTROL( "item_img_goods", m_pkItemImgGoods );
    LINK_CONTROL( "item_img_reward", m_pkItemImgReward );

    LINK_CONTROL( "static_name_goods", m_pkStaticItemNameGoods );
    LINK_CONTROL( "static_name_reward", m_pkStaticItemNameReward );
    LINK_CONTROL( "static_title_desc", m_pkStaticTitleDesc );
    LINK_CONTROL( "static_desc", m_pkStaticDesc );

}

KGCSocialCommerceRewardBox::~KGCSocialCommerceRewardBox( void )
{
}


void KGCSocialCommerceRewardBox::OnCreate( void )
{
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );

    m_pkItemImgGoods->InitState( true );
    m_pkItemImgReward->InitState( true );

    m_pkStaticItemNameGoods->InitState( true );
    m_pkStaticItemNameGoods->SetFontColor(D3DCOLOR_ARGB( 255, 56, 41, 100));
    m_pkStaticItemNameGoods->SetAlign( DT_CENTER );
    m_pkStaticItemNameReward->InitState( true );
    m_pkStaticItemNameReward->SetFontColor(D3DCOLOR_ARGB( 255, 56, 41, 100));
    m_pkStaticItemNameReward->SetAlign( DT_CENTER );
    m_pkStaticTitleDesc->InitState( true );
    m_pkStaticTitleDesc->SetFontColor(D3DCOLOR_ARGB( 255, 200, 26, 83));
    m_pkStaticDesc->InitState( true );
    m_pkStaticDesc->SetMultiLine( true );
}

void KGCSocialCommerceRewardBox::OnCreateComplete( void )
{
}

void KGCSocialCommerceRewardBox::FrameMoveInEnabledState( void )
{
    //
}


void KGCSocialCommerceRewardBox::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnClose,          KD3DWnd::D3DWE_BUTTON_CLICK, OnClickClose );
}

void KGCSocialCommerceRewardBox::OnClickClose()
{
    ToggleRender( false );
}

void KGCSocialCommerceRewardBox::SetRewardInfo( std::pair<KSCSellInfo, VEC_REWARD>& prInfo )
{
    VEC_REWARD::iterator iter = prInfo.second.begin();

    // 상품
    m_pkItemImgGoods->SetItemInfo( prInfo.first.m_dwItemID / 10 );
    GCItem* pkItem = g_MyD3D->m_kItemMgr.GetItemData( prInfo.first.m_dwItemID / 10 );
    m_pkStaticItemNameGoods->SetText( pkItem->strItemName );

    // 보상
    m_pkItemImgReward->SetItemInfo( (*iter).m_ItemID / 10 );
    pkItem = g_MyD3D->m_kItemMgr.GetItemData( (*iter).m_ItemID / 10 );
    m_pkStaticItemNameReward->SetText( pkItem->strItemName );

    // 안내문
    m_pkStaticTitleDesc->SetText( g_pkStrLoader->GetString( STR_ID_SOCIAL_COMMERCE_4 ) );
    m_pkStaticDesc->SetText( g_pkStrLoader->GetString( STR_ID_SOCIAL_COMMERCE_5 ) );
}