#include "stdafx.h"
#include "KGCSingleImage.h"

IMPLEMENT_CLASSNAME( KGCSingleImage );
IMPLEMENT_WND_FACTORY( KGCSingleImage );
IMPLEMENT_WND_FACTORY_NAME( KGCSingleImage, "gc_single_image" );

KGCSingleImage::KGCSingleImage( void )
: m_iTotalNumInTexture(0)
, m_pTexture(NULL)
, m_vBaseTexStartAxis(1.f,0.f)
, m_vTexSize(197.f, 68.f)
, m_vTextureFullSize(512.f,512.f)
, m_vSecondTexStartAxis(203.f, 71.f)
, m_iCurrentIndex(0)
, m_iFileNum(0)
{
    m_strFileName = "";
}

KGCSingleImage::~KGCSingleImage( void )
{
}

void KGCSingleImage::OnCreate()
{

}

void KGCSingleImage::ActionPerformed(const KActionEvent& event )
{

}

void KGCSingleImage::SetInfomation( D3DXVECTOR2 vTexSize, D3DXVECTOR2 vSecondTexStartAxis, std::string strFileName, int iIndex, int iTotlaIndex )
{
    m_vTexSize = vTexSize;
    m_vSecondTexStartAxis = vSecondTexStartAxis;
    m_iCurrentIndex = iIndex;
    m_iTotalNumInTexture = iTotlaIndex;
    m_strFileName = strFileName;

    DrawImage();
}

void KGCSingleImage::DrawImage()
{
    //if ( !bOK ) 
    //{
    //    SAFE_RELEASE( m_pTexture );
    //    return;
    //}

    char cBuf[256];
    memset(cBuf,'\0',256);

    sprintf(cBuf, m_strFileName.c_str() );
    SAFE_RELEASE( m_pTexture );

    m_pTexture = g_pGCDeviceManager2->CreateTexture(cBuf);

    int nX = m_iCurrentIndex / m_iTotalNumInTexture;
    int nY = m_iCurrentIndex % m_iTotalNumInTexture;

    m_vImageTexStartAxis.x = ( m_vBaseTexStartAxis.x + ((m_vSecondTexStartAxis.x - m_vBaseTexStartAxis.x) *static_cast<float>(nX) ) ) / m_vTextureFullSize.x;
    m_vImageTexStartAxis.y = ( m_vBaseTexStartAxis.y + ((m_vSecondTexStartAxis.y - m_vBaseTexStartAxis.y) *static_cast<float>(nY) ) ) / m_vTextureFullSize.y;
    m_vImageTexEndtAxis.x = ( m_vBaseTexStartAxis.x + m_vTexSize.x + ((m_vSecondTexStartAxis.x - m_vBaseTexStartAxis.x) *static_cast<float>(nX) ) ) / m_vTextureFullSize.x;
    m_vImageTexEndtAxis.y = ( m_vBaseTexStartAxis.y + m_vTexSize.y + ((m_vSecondTexStartAxis.y - m_vBaseTexStartAxis.y) *static_cast<float>(nY) ) ) / m_vTextureFullSize.y;
}

void KGCSingleImage::PostDraw()
{
    if(m_pTexture == NULL) return;

    D3DXVECTOR2 vPos( GetRelocatedWindowPos() );

    //UI표시할 위치 정하기
    m_vImagePos.x = vPos.x;
    m_vImagePos.y = vPos.y;

    int nWidth = GetWidth();
    int nHeight = GetHeight();

    g_pGCDeviceManager2->PushState();
    g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    g_pGCDeviceManager2->DrawInScreen(m_pTexture,
        m_vImagePos.x, 
        m_vImagePos.y,
        m_vImagePos.x + nWidth * m_fWindowScaleX, 
        m_vImagePos.y + nHeight * m_fWindowScaleY,
        m_vImageTexStartAxis.x,
        m_vImageTexStartAxis.y, 
        m_vImageTexEndtAxis.x,
        m_vImageTexEndtAxis.y,
        false,10.0f,D3DCOLOR_ARGB(255,255,255,255), false);
    g_pGCDeviceManager2->PopState();
}
