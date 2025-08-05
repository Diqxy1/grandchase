#include "StdAfx.h"
#include "D3DImageIcon.h"
#include "GCIconImageManager.h"

IMPLEMENT_CLASSNAME( KD3DImageIcon );

KD3DImageIcon::KD3DImageIcon(void)
{
	m_strTextureName=L"";
    m_pTexture = NULL;
}

KD3DImageIcon::~KD3DImageIcon(void)
{
    SAFE_RELEASE( m_pTexture );
}

void KD3DImageIcon::Destroy( void )
{
    SAFE_RELEASE( m_pTexture );
}

void KD3DImageIcon::Create( bool bAuto )
{
	KD3DWnd::Create( bAuto );
}

void KD3DImageIcon::PostDraw( void )
{
    
    D3DXVECTOR2 vPos = GetCurrentWindowPos();

    if( m_pTexture ) {
        g_pGCDeviceManager2->PushState();
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
        {
            g_pGCDeviceManager2->DrawInScreen( m_pTexture, vPos.x * m_fWindowScaleX, vPos.y * m_fWindowScaleY, 
                (vPos.x + GetWidth()) * m_fWindowScaleX , (vPos.y + GetHeight())  * m_fWindowScaleY , 0.0f, 0.0f, 1.0f, 1.0f );
        }
        g_pGCDeviceManager2->PopState();
    } else {
        GCImageIconTexture* pTexture;

        if (m_strTextureName.empty()) {
            pTexture = SiKGCIconImageManager()->GetTexture(L"defaultmark.dds"); 
        } 
        else {        
            pTexture = SiKGCIconImageManager()->GetTexture(m_strTextureName); 

            // NULL이면 기본 길드마크를 셋팅한다.
            if( pTexture == NULL ) 
                pTexture = SiKGCIconImageManager()->GetTexture(L"defaultmark.dds"); 
    	    if( pTexture == NULL)
    		    return;

            g_pGCDeviceManager2->DrawInScreen( pTexture, vPos.x * m_fWindowScaleX, vPos.y * m_fWindowScaleY, 
                (vPos.x + GetWidth()) * m_fWindowScaleX , (vPos.y + GetHeight())  * m_fWindowScaleY , 0.0f, 0.0f, 1.0f, 1.0f );
       }	
    }
}

void KD3DImageIcon::SetTextureName( std::wstring strFileName )
{
	m_strTextureName = strFileName;
}

void KD3DImageIcon::SetTexNameUseMassFile( const std::wstring& strFileName_ )
{
    m_strTextureName = strFileName_;

    SAFE_RELEASE( m_pTexture );
    m_pTexture = g_pGCDeviceManager2->CreateTexture( GCUTIL_STR::GCStrWideToChar( strFileName_.c_str() ) );
}