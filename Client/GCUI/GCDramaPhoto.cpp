#include "stdafx.h"
#include "GCDramaPhoto.h"

IMPLEMENT_CLASSNAME( KGCDramaPhoto );
IMPLEMENT_WND_FACTORY( KGCDramaPhoto );
IMPLEMENT_WND_FACTORY_NAME( KGCDramaPhoto, "gc_dramaphoto" );

KGCDramaPhoto::KGCDramaPhoto( void )
{
    m_pDramaTexture = NULL;
}

KGCDramaPhoto::~KGCDramaPhoto( void )
{
    SAFE_RELEASE( m_pDramaTexture );
}

void KGCDramaPhoto::ActionPerformed( const KActionEvent& event )
{
}

void KGCDramaPhoto::OnCreate( void )
{
}

void KGCDramaPhoto::PostDraw( void )
{
    if(m_pDramaTexture == NULL) return;

    D3DXVECTOR2 vPos( GetRelocatedWindowPos() );

    g_pGCDeviceManager2->PushState();
    g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    g_pGCDeviceManager2->DrawInScreen(m_pDramaTexture,
        vPos.x, 
        vPos.y,
        vPos.x + nXSize * m_fWindowScaleX, 
        vPos.y + nYSize * m_fWindowScaleY,
        m_vImageTexStartAxis.x,
        m_vImageTexStartAxis.y, 
        m_vImageTexEndtAxis.x,
        m_vImageTexEndtAxis.y,
        false,10.0f,D3DCOLOR_ARGB(255,255,255,255), false);
    g_pGCDeviceManager2->PopState();
}

void KGCDramaPhoto::SetDramaPhoto( SDramaPhoto& sDramaPhoto )
{
    SAFE_RELEASE( m_pDramaTexture );

    if ( sDramaPhoto.strTexName.empty() )
        return;

    m_pDramaTexture = g_pGCDeviceManager2->CreateTexture(sDramaPhoto.strTexName);

    D3DXVECTOR2 vecTextureFullSize( 512.0f, 512.0f );

    m_vImageTexStartAxis.x = static_cast<float>(sDramaPhoto.nTexTU1)/vecTextureFullSize.x;
    m_vImageTexStartAxis.y = static_cast<float>(sDramaPhoto.nTexTV1)/vecTextureFullSize.y;

    m_vImageTexEndtAxis.x = static_cast<float>(sDramaPhoto.nTexTU2)/vecTextureFullSize.x;
    m_vImageTexEndtAxis.y = static_cast<float>(sDramaPhoto.nTexTV2)/vecTextureFullSize.y;

    nXSize = sDramaPhoto.nXSize;
    nYSize = sDramaPhoto.nYSize;
}


