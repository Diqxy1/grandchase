#include "stdafx.h"
#include "KGCCharacterName.h"

IMPLEMENT_CLASSNAME( KGCCharacterName );
IMPLEMENT_WND_FACTORY( KGCCharacterName );
IMPLEMENT_WND_FACTORY_NAME( KGCCharacterName, "gc_character_name" );

KGCCharacterName::KGCCharacterName( void )
: m_pNameTexture(NULL)
, m_vBaseTexStartAxis(1.f,1.f)
, m_vTexSize(254.f, 41.f)
, m_vTextureFullSize(512.f,512.f)
, m_vSecondTexStartAxis(257.f, 43.f)
{
}

KGCCharacterName::~KGCCharacterName( void )
{
}

void KGCCharacterName::OnCreate()
{

}

void KGCCharacterName::ActionPerformed(const KActionEvent& event )
{

}

void KGCCharacterName::SetCharacterInfo( int iCharType, int iPromotion )
{
    int iFileNum = 0;
    int iIndexNum = 0;
    int iMax = 0;
    bool bOk = false;

    iMax = GC_CHAR_NUM * NUM_JOB_LEVEL;
    
    if ( ( iCharType + 1 ) * (iPromotion + 1 ) > iMax || iCharType >= GC_CHAR_NUM || iPromotion >= NUM_JOB_LEVEL ) {
        SAFE_RELEASE( m_pNameTexture );
        return;
    }
    
    if ( GetCharacterTextureInfo( iIndexNum , iFileNum, iCharType, iPromotion ) == false )
        return;

    char cBuf[256];
    memset(cBuf,'\0',256);

    sprintf(cBuf,"ui_tutorial_JopName%d.dds",iFileNum);

    SAFE_RELEASE( m_pNameTexture );

    m_pNameTexture = g_pGCDeviceManager2->CreateTexture(cBuf);

    m_vTexSize = D3DXVECTOR2(254.f, 41.f);
    m_vBaseTexStartAxis = D3DXVECTOR2(1.f,1.f);
    m_vTextureFullSize = D3DXVECTOR2(512.f,512.f);
    m_vSecondTexStartAxis= D3DXVECTOR2(257.f, 43.f);

    int nX = iIndexNum / 12;
    int nY = iIndexNum % 12;

    m_vImageTexStartAxis.x = ( m_vBaseTexStartAxis.x + ((m_vSecondTexStartAxis.x - m_vBaseTexStartAxis.x) *static_cast<float>(nX) ) ) / m_vTextureFullSize.x;
    m_vImageTexStartAxis.y = ( m_vBaseTexStartAxis.y + ((m_vSecondTexStartAxis.y - m_vBaseTexStartAxis.y) *static_cast<float>(nY) ) ) / m_vTextureFullSize.y;
    m_vImageTexEndtAxis.x = ( m_vBaseTexStartAxis.x + m_vTexSize.x + ((m_vSecondTexStartAxis.x - m_vBaseTexStartAxis.x) *static_cast<float>(nX) ) ) / m_vTextureFullSize.x;
    m_vImageTexEndtAxis.y = ( m_vBaseTexStartAxis.y + m_vTexSize.y + ((m_vSecondTexStartAxis.y - m_vBaseTexStartAxis.y) *static_cast<float>(nY) ) ) / m_vTextureFullSize.y;
}

void KGCCharacterName::PostChildDraw()
{
    if(m_pNameTexture == NULL) return;

    D3DXVECTOR2 vPos( GetRelocatedWindowPos() );

    //UI표시할 위치 정하기
    m_vImagePos.x = vPos.x;
    m_vImagePos.y = vPos.y;

    int nWidth = GetWidth();
    int nHeight = GetHeight();

    g_pGCDeviceManager2->PushState();
    g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    g_pGCDeviceManager2->DrawInScreen(m_pNameTexture,
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

bool KGCCharacterName::GetCharacterTextureInfo( OUT int& iIndexNum , OUT int& iFileNum , IN int iCharType, IN int iPromotion )
{
    //tutorial_jop_name에 들어가 있는 캐릭터 전직 별 마다 6개의 캐릭터가 들어간다.
    int iLimitJob = 6;
    int iTexNum = GC_CHAR_NUM / iLimitJob;
    if( GC_CHAR_NUM % iLimitJob != 0 ) {
        iTexNum += 1;
    }    
    bool bValid = false;
    for( int i = 0; i < iTexNum; i++ ) {
        if ( iCharType >= ( i * iLimitJob ) && iCharType < ( ( i + 1 ) * iLimitJob ) ) {
            iFileNum = i + 1;
            bValid = true;
            break;
        }
    }

    if( bValid == false ) {
        return false;
    }

    //인덱스 구하기
    if ( iCharType == 0 && iPromotion == 0 ) {
        iIndexNum = 0;
    }
    else
        iIndexNum =  (  ( iCharType - ( (iFileNum -1 ) * 6) )  * NUM_JOB_LEVEL ) + iPromotion;

    return true;
}