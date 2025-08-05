#include "stdafx.h"
#include "KGCStampTitle.h"

IMPLEMENT_CLASSNAME( KGCStampTitle );
IMPLEMENT_WND_FACTORY( KGCStampTitle );
IMPLEMENT_WND_FACTORY_NAME( KGCStampTitle, "gc_stamp_title" );

const int iTitleNumInTexture  = 14;

KGCStampTitle::KGCStampTitle( void )
: m_iUitype(-1)
, m_pStampTexture(NULL)
, m_vBaseTexStartAxis(1.f,0.f)
, m_vTexSize(197.f, 68.f)
, m_vTextureFullSize(512.f,512.f)
, m_vSecondTexStartAxis(203.f, 71.f)
, m_iIndex(0)
{
}

KGCStampTitle::~KGCStampTitle( void )
{
}

void KGCStampTitle::OnCreate()
{

}

void KGCStampTitle::ActionPerformed(const KActionEvent& event )
{

}

void KGCStampTitle::SetUiType( int iID )
{
    m_iUitype = iID;
    SetStampImage();
}

void KGCStampTitle::SetStampImage()
{
    int iFileNum =0;
    int iIndexNum = 0;

    bool bOK = false;
    //bOK = SiGCAttendanceStampManager()->GetStampImageInfo(iFileNum, iIndexNum, m_iUitype);
    bOK = GetStampImageInfo(iFileNum, iIndexNum, m_iUitype);

    if ( !bOK ) 
    {
        SAFE_RELEASE( m_pStampTexture );
        return;
    }


    char cBuf[256];
    memset(cBuf,'\0',256);

    sprintf(cBuf,"ui_event_stamp_title_text_%d.dds",iFileNum);

    SAFE_RELEASE( m_pStampTexture );

    m_pStampTexture = g_pGCDeviceManager2->CreateTexture(cBuf);

    m_vTexSize = D3DXVECTOR2(197.f, 68.f);
    m_vBaseTexStartAxis = D3DXVECTOR2(1.f,0.f);
    m_vTextureFullSize = D3DXVECTOR2(512.f,512.f);
    m_vSecondTexStartAxis= D3DXVECTOR2(203.f, 71.f);

    int nX = iIndexNum / 7;
    int nY = iIndexNum % 7;
    
    m_vImageTexStartAxis.x = ( m_vBaseTexStartAxis.x + ((m_vSecondTexStartAxis.x - m_vBaseTexStartAxis.x) *static_cast<float>(nX) ) ) / m_vTextureFullSize.x;
    m_vImageTexStartAxis.y = ( m_vBaseTexStartAxis.y + ((m_vSecondTexStartAxis.y - m_vBaseTexStartAxis.y) *static_cast<float>(nY) ) ) / m_vTextureFullSize.y;
    m_vImageTexEndtAxis.x = ( m_vBaseTexStartAxis.x + m_vTexSize.x + ((m_vSecondTexStartAxis.x - m_vBaseTexStartAxis.x) *static_cast<float>(nX) ) ) / m_vTextureFullSize.x;
    m_vImageTexEndtAxis.y = ( m_vBaseTexStartAxis.y + m_vTexSize.y + ((m_vSecondTexStartAxis.y - m_vBaseTexStartAxis.y) *static_cast<float>(nY) ) ) / m_vTextureFullSize.y;
}

void KGCStampTitle::PostDraw()
{
    if(m_pStampTexture == NULL) return;

    D3DXVECTOR2 vPos( GetRelocatedWindowPos() );

    //UI표시할 위치 정하기
    m_vImagePos.x = vPos.x;
    m_vImagePos.y = vPos.y;

    int nWidth = GetWidth();
    int nHeight = GetHeight();

    g_pGCDeviceManager2->PushState();
    g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    g_pGCDeviceManager2->DrawInScreen(m_pStampTexture,
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

bool KGCStampTitle::GetStampImageInfo( OUT int &iFileNum, OUT int &iIndexNum , IN int iUitype )
{
    if ( iUitype < 0 )
        return false;

    if ( iUitype == 0 ) {
        iFileNum = 0;
        iIndexNum = 0;
    }
    else {
        iFileNum = iUitype / iTitleNumInTexture;
        iIndexNum = iUitype % iTitleNumInTexture;
    }

    return true;
}