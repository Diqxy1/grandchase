#include "stdafx.h"
#include ".\GCSetItemImgWnd.h"

//


//

IMPLEMENT_CLASSNAME( KGCSetItemImgWnd );
IMPLEMENT_WND_FACTORY( KGCSetItemImgWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCSetItemImgWnd, "gc_setitem_img_wnd" );

KGCSetItemImgWnd::KGCSetItemImgWnd()
{
    m_bShow = false;
    m_pTexture = NULL;
    m_fSizeX = 155.f;
    m_fSizeY = 287.f;
}
KGCSetItemImgWnd::~KGCSetItemImgWnd()
{
    SAFE_RELEASE( m_pTexture );
}

void KGCSetItemImgWnd::OnCreate( void )
{
}
void KGCSetItemImgWnd::FrameMoveInEnabledState( void )
{
}
void KGCSetItemImgWnd::PostDraw( void )
{	
    //if ( m_bShow )
    {
        D3DXVECTOR2 vPos( GetRelocatedWindowPos() );
        D3DXVECTOR4 vTexCoords( GetTextureCoords( m_iSetNumber ) );        

		g_pGCDeviceManager2->PushState();
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
        {
			g_pGCDeviceManager2->DrawInScreen(m_pTexture,vPos.x, vPos.y, vPos.x + m_fSizeX * m_fWindowScaleX, vPos.y + m_fSizeY  * m_fWindowScaleY,
				vTexCoords.x, vTexCoords.y, vTexCoords.z, vTexCoords.w,false,10.0f,0xffffffff);
        }
        g_pGCDeviceManager2->PopState();
    }
}
void KGCSetItemImgWnd::SetItemInfo( int iSetNumber_ , DWORD dwGoodsID_, float fSizeX, float fSizeY )
{
    SAFE_RELEASE( m_pTexture );
    //========ItemResorceLoading====================
    m_pTexture = g_pItemMgr->CreateShopSetItemTexture( iSetNumber_,dwGoodsID_ );
    GCItem *pItem = g_pItemMgr->GetSameImageIndexItemInfo(dwGoodsID_);
    if(pItem != NULL){
        m_iSetNumber = pItem->iSetItemNumber;
    }
    else{
        m_iSetNumber = iSetNumber_;
    }
    //===============================================
	if( fSizeX == -1.0f )
	{
		m_fSizeX = static_cast< float >( GetWidth() ) * m_fWindowScaleX;
		m_fSizeY = static_cast< float >( GetHeight() ) * m_fWindowScaleY;
	}
	else
	{
		m_fSizeX = fSizeX;
		m_fSizeY = fSizeY;
	}
}

void KGCSetItemImgWnd::ShowItem( bool bShow_ )
{
    m_bShow = bShow_;
}

D3DXVECTOR4 KGCSetItemImgWnd::GetTextureCoords( int iSetNum )
{
    D3DXVECTOR4 vTexCoords;

    if ( 0 == iSetNum % 2 )
    {
        vTexCoords.x = 0.0f;
        vTexCoords.y = 0.0f;
        vTexCoords.z = 0.5f;
        vTexCoords.w = 1.0f;
    }
    else
    {
        vTexCoords.x = 0.5f;
        vTexCoords.y = 0.0f;
        vTexCoords.z = 1.0f;
        vTexCoords.w = 1.0f;
    }

    return vTexCoords;
}