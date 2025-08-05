#include "stdafx.h"
#include "GCAgitObjectSlot.h"
#include "GCAgitOverlayUI.h"

IMPLEMENT_CLASSNAME( KGCAgitObjectSlot );
IMPLEMENT_WND_FACTORY( KGCAgitObjectSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitObjectSlot, "gc_object_slot" );

KGCAgitObjectSlot::KGCAgitObjectSlot( void )
{
    m_pkDefaultFrame = NULL;
    m_pkActiveFrame = NULL;
    m_pkBtnSell = NULL;
    m_pkBtnSetting = NULL;
    LINK_CONTROL( "defaultFrame", m_pkDefaultFrame );
    LINK_CONTROL( "activeFrame", m_pkActiveFrame );
    LINK_CONTROL( "btnSell", m_pkBtnSell );
    LINK_CONTROL( "btnSetting", m_pkBtnSetting );

    m_ItemID = KItem::INVALID_ITEM;
    m_iCount = 0;

    m_iCharType = GC_CHAR_INVALID;
    m_bLock  = false;

    m_iSlotType  = TYPE_OBJ;
    m_pkItemTex = NULL;

    m_pkCharIcon = NULL;
    m_pkLockIcon = NULL;
    LINK_CONTROL( "charIcon", m_pkCharIcon );
    LINK_CONTROL( "lockIcon", m_pkLockIcon );
}

KGCAgitObjectSlot::~KGCAgitObjectSlot( void )
{
    SAFE_RELEASE( m_pkItemTex );
}

void KGCAgitObjectSlot::OnCreate( void )
{
    m_pkDefaultFrame->InitState( true, true, this );
    m_pkActiveFrame->InitState( false, true, this );
    m_pkBtnSell->InitState( false, true, this );
    m_pkBtnSetting->InitState( false, true, this );

    m_pkCharIcon->InitState( false, true, this  );
    m_pkLockIcon->InitState( false, true, this  );
}

void KGCAgitObjectSlot::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkDefaultFrame,  KD3DWnd::D3DWE_BUTTON_DOWN,    OnClick );
    GCWND_MSG_MAP( m_pkActiveFrame,   KD3DWnd::D3DWE_BUTTON_DOWN,    OnClick );
    GCWND_MSG_MAP( m_pkCharIcon,      KD3DWnd::D3DWE_BUTTON_DOWN,    OnClick );
    GCWND_MSG_MAP( m_pkLockIcon,      KD3DWnd::D3DWE_BUTTON_DOWN,    OnClick );
    GCWND_MSG_MAP( m_pkBtnSell,       KD3DWnd::D3DWE_BUTTON_CLICK,   OnSell );
    GCWND_MSG_MAP( m_pkBtnSetting,    KD3DWnd::D3DWE_BUTTON_CLICK,   OnSetting );
}

void KGCAgitObjectSlot::PostChildDraw( void )
{
    if( m_pkItemTex == NULL )
        return;

    // 슬롯 이미지 
    float fX = 3.0f * GC_SCREEN_DIV_WIDTH;
    float fY = 1.0f * GC_SCREEN_DIV_WIDTH;
    float fH = 56.0f * GC_SCREEN_DIV_WIDTH;
    float fW = 56.0f * GC_SCREEN_DIV_WIDTH;
    float fNX = 40.0f * GC_SCREEN_DIV_WIDTH;
    float fNY = 40.0f * GC_SCREEN_DIV_WIDTH;

    float fScaleX = g_pGCDeviceManager2->GetWindowScaleX();
    float fScaleY = g_pGCDeviceManager2->GetWindowScaleY();
    
    D3DXVECTOR2 dxvDrawPos = m_pkDefaultFrame->GetRelocatedWindowPos();
    dxvDrawPos.x += ( fX * fScaleX );
    dxvDrawPos.y += ( fY * fScaleY );

    g_pItemMgr->Draw_ShopImage( dxvDrawPos, fW * fScaleX, fH * fScaleY, m_pkItemTex, false, -1, 0xffffffff );

    // 갯수 표시 
    dxvDrawPos.x += ( fNX * fScaleX );
    dxvDrawPos.y += ( fNY * fScaleY );
    g_pkUIMgr->RenderNumber( "very_small_num", dxvDrawPos, m_iCount, D3DXCOLOR( 0.95f, 0.89f, 0.64f, 1.0f ) );

    // 팔기 버튼 
    if( IsActive() && m_iSlotType == TYPE_OBJ ) {
        m_pkBtnSell->Render();
    }

    if( IsActive() && !m_bLock && m_iSlotType == TYPE_CHAR ) 
    {
        m_pkBtnSetting->Render();
    }
}

void KGCAgitObjectSlot::SetObjectSlot( GCITEMID itemID_, int iCount_ )
{
    // 아이템 슷롯정보 세팅
    m_ItemID = itemID_;
    m_iCount = iCount_;

    m_pkItemTex = g_pItemMgr->CreateShopItemTexture( m_ItemID / 10 );

    m_pkCharIcon->ToggleRender( false );
    m_pkLockIcon->ToggleRender( false );

    m_iSlotType = TYPE_OBJ;
}

void KGCAgitObjectSlot::OnSell( void )
{
    if( m_iCount <= 0 )
        return;

    GCITEMUID dwItemUID = SiKGCAgitGameManager()->GetHideObjectUID(m_ItemID);
    g_pkAgitOverlayUI->ShowAgitSellBox( true, std::make_pair(m_ItemID, dwItemUID));
}

void KGCAgitObjectSlot::Clear( void )
{
    m_ItemID = KItem::INVALID_ITEM;
    m_iCount = 0;

    m_iCharType = GC_CHAR_INVALID;
    m_bLock  = false;

    m_iSlotType  = TYPE_OBJ;

    m_pkLockIcon->ToggleRender( false );
    m_pkCharIcon->ToggleRender( false );

    SAFE_RELEASE( m_pkItemTex );
}

void KGCAgitObjectSlot::OnClick( void )
{
    SpeakToActionListener( KActionEvent(this,D3DWE_BUTTON_DOWN) );  
}

void KGCAgitObjectSlot::SetActive( bool bActive_ )
{
    m_pkDefaultFrame->ToggleRender( !bActive_ );
    m_pkActiveFrame->ToggleRender( bActive_ );
    
    m_pkBtnSell->ToggleRender( bActive_ && m_iSlotType == TYPE_OBJ );

    m_pkBtnSetting->ToggleRender( bActive_ && 
                                  !m_bLock && 
                                  m_iSlotType == TYPE_CHAR );
}

void KGCAgitObjectSlot::SetCharSlot( int iCharType_, bool bLock_ )
{
    if( iCharType_ == GC_CHAR_INVALID )
        return;

    if( iCharType_ >= GC_CHAR_NUM )
        return;

    m_iCharType = iCharType_;
    m_bLock = bLock_;

    m_pkCharIcon->ToggleRender( true );
    m_pkCharIcon->ShowCharIcon( m_iCharType, m_bLock );

    if( m_bLock )
        m_pkLockIcon->ToggleRender( true );

    m_iSlotType = TYPE_CHAR;
}

void KGCAgitObjectSlot::OnSetting( void )
{
    g_pkAgitOverlayUI->ShowCharSettingUI( true, m_iCharType );
}