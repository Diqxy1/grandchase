#include "stdafx.h"
#include "GCAgitMapSlot.h"

IMPLEMENT_CLASSNAME( KGCAgitMapSlot );
IMPLEMENT_WND_FACTORY( KGCAgitMapSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitMapSlot, "gc_agit_map_slot" );

KGCAgitMapSlot::KGCAgitMapSlot( void )
{
    m_pkMainFrame = NULL;
    m_pkPriceFrame = NULL;
    LINK_CONTROL( "mainFrame", m_pkMainFrame );
    LINK_CONTROL( "priceBack", m_pkPriceFrame );

    m_pMapTex = NULL;
}

KGCAgitMapSlot::~KGCAgitMapSlot( void )
{
    SAFE_RELEASE( m_pMapTex );
}

void KGCAgitMapSlot::OnCreate( void )
{
    m_pkMainFrame->InitState( true, true, this );
    m_pkPriceFrame->InitState( true, false, NULL );
}

void KGCAgitMapSlot::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkMainFrame,   KD3DWnd::D3DWE_BUTTON_CLICK,    OnClick );
}

void KGCAgitMapSlot::PostChildDraw( void )
{
    float fScaleX = g_pGCDeviceManager2->GetWindowScaleX();
    float fScaleY = g_pGCDeviceManager2->GetWindowScaleY();
    
    if( m_MapInfo.m_usMaxFlowerPot != 0 )
    {
        // 이미지 그리기 
        if( m_pMapTex )
        {
            float fX = 5.0f;
            float fY = 6.0f;
            float fW = 102.0f;
            float fH = 69.0f;

            D3DXVECTOR2 dxvMapPos = m_pkMainFrame->GetRelocatedWindowPos();
            dxvMapPos.x += ( fX * fScaleX );
            dxvMapPos.y += ( fY * fScaleY );

            g_pGCDeviceManager2->Draw_ShopImage( dxvMapPos, fW, fH, m_pMapTex, false );
        }

        // 가격 그리기 
        D3DXVECTOR2 dxvPricePos = m_pkPriceFrame->GetRelocatedWindowPos();
        float fNumOffsetX = 69.0f * GC_SCREEN_DIV_WIDTH;
        float fNumOffsetY = 7.0f * GC_SCREEN_DIV_WIDTH;
        float fGap = 8.0f * GC_SCREEN_DIV_WIDTH;  // very_small_num

        fNumOffsetX -= g_kGlobalValue.GetCiphers( m_MapInfo.m_nPrice ) * fGap;
        dxvPricePos.x += ( fNumOffsetX * fScaleX );
        dxvPricePos.y += ( fNumOffsetY * fScaleY );

        g_pkUIMgr->RenderNumber( "very_small_num", dxvPricePos, m_MapInfo.m_nPrice,  D3DXCOLOR( 0.95f, 0.89f, 0.64f, 1.0f ) );
    }
}

void KGCAgitMapSlot::SetMapInfo( KAgitMap& info_ )
{
    m_MapInfo = info_;

    // 맵 텍스처도 로드하세영 
    std::string strFileName = m_MapInfo.m_strFileName;
    std::string strExt = ".dds";
    std::string::size_type stdPos = strFileName.find( "." );
    if( std::string::npos != stdPos )
        strFileName.replace( stdPos, 4, strExt );

    SAFE_RELEASE( m_pMapTex );
    m_pMapTex = g_pGCDeviceManager2->CreateTexture( strFileName );
}

void KGCAgitMapSlot::Clear( void )
{
    m_MapInfo.m_dwMapID = 0;
    m_MapInfo.m_nPrice = 0;
    m_MapInfo.m_usMaxFlowerPot = 0;
}

void KGCAgitMapSlot::OnClick( void )
{
    SpeakToActionListener( KActionEvent(this,KD3DWnd::D3DWE_BUTTON_CLICK) );
}

