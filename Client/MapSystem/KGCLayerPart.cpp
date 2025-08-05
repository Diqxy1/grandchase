#include "stdafx.h"
#include ".\kgclayerpart.h"

#include "GCCameraOrtho.h"

//
//
//

KGCLayerPart::KGCLayerPart(void) : KLayerPart<KGCLayerPart>()
{
}

KGCLayerPart::~KGCLayerPart(void)
{
    std::vector< CParticleEventSeqPTR >::iterator it;
    for ( it = m_vecParticleSeq.begin(); it != m_vecParticleSeq.end(); it++ )
    {
        g_ParticleManager->DeleteSequence( *it );
    }
    m_vecParticleSeq.clear();
    
}

KGCLayerPart* KGCLayerPart::Clone()
{
    KGCLayerPart* pTemp = new KGCLayerPart();
    pTemp->CopyMember( this );
    return pTemp;
}

void KGCLayerPart::CopyMember( KGCLayerPart* pSrc )
{
    m_strPartName = pSrc->m_strPartName;
    m_strFileName = pSrc->m_strFileName;
    m_pTexture = pSrc->m_pTexture;
    if( m_pTexture )
        m_pTexture->AddRefCount();
    m_iTexX = pSrc->m_iTexX;
    m_iTexY = pSrc->m_iTexY;
    m_iTexWidth = pSrc->m_iTexWidth;
    m_iTexHeight = pSrc->m_iTexHeight;

    for( int i = 0; i < 4; i++ )
    {
        m_Vertex[i] = pSrc->m_Vertex[i];
        ScrolledVertex[i] = pSrc->ScrolledVertex[i];
    }

    m_fLayerX = pSrc->m_fLayerX;
    m_fLayerY = pSrc->m_fLayerY;
    m_fWidth = pSrc->m_fWidth;
    m_fHeight = pSrc->m_fHeight;
    m_bMoveable = pSrc->m_bMoveable;
    m_fSpeedX = pSrc->m_fSpeedX;
    m_fSpeedY = pSrc->m_fSpeedY;
    m_iZOrder = pSrc->m_iZOrder;
    m_bPositionReset_X = pSrc->m_bPositionReset_X;
    m_bPositionReset_Y = pSrc->m_bPositionReset_Y;
    m_fScrollX = pSrc->m_fScrollX;
    m_fScrollY = pSrc->m_fScrollY;

    // Clone 한 Effect는 Effect가 있으면 따로 생성하므로 항상 NULL
    //m_strEffectName = "";
    //m_pEffect = NULL;
    //m_iXOffset = pSrc->m_iXOffset;
    //m_iYOffset = pSrc->m_iYOffset;

    m_strCategory = pSrc->m_strCategory;
    m_strUniqueName = pSrc->m_strUniqueName;

    m_iUserValue    = pSrc->m_iUserValue;
    m_strUserString = pSrc->m_strUserString;

    m_vecParticle   = pSrc->m_vecParticle;

    SetTexel();
}

//void KGCLayerPart::OnEffectTrace( KGCEffect* pEffect )
//{
//    if( m_pEffect == NULL )
//        return ;
//
//    D3DXVECTOR3 AddMatrix;
//    AddMatrix.x = m_Vertex[2].m_vPos.x + m_fScrollX;
//    AddMatrix.y = m_Vertex[2].m_vPos.y + m_fScrollY;
//    AddMatrix.z = 0.5f;
//    pEffect->SetPosition(AddMatrix.x, AddMatrix.y, true);
//}

void KGCLayerPart::FrameMove( float fElapsedTime )
{
    for( int i = 0; i < 4; i++ )
    {
        if( m_bMoveable )
        {
            m_Vertex[i].Pos.x += (float)m_fSpeedX;
            m_Vertex[i].Pos.y += (float)m_fSpeedY;
        }
    }

    m_fLayerX = m_Vertex[0].Pos.x;
    m_fLayerY = m_Vertex[2].Pos.y;

    //if( m_bMoveable )
    //{
    //    if( m_pEffect != NULL )
    //        m_pEffect->SetPosition( m_Vertex[2].m_vPos.x, m_Vertex[2].m_vPos.y, true );
    //}

    if( m_bShow == false )
        return;

    Flush();
}

void KGCLayerPart::Flush()
{
    if( m_bShow == false )
        return;

    SetVertex();

    if( m_pTexture != NULL )
    {
        for( int i = 0; i < 4; i++ )
        {
            ScrolledVertex[i] = m_Vertex[i];
            // Scale = Zoom이 달라지면 점들도 그에 맞게 달라져야한다.
            ScrolledVertex[i].Pos.x += m_fScrollX;
            ScrolledVertex[i].Pos.y += m_fScrollY;

            // 알파가 차츰 빠지게 하는 부분
            DWORD dwAlpha = ScrolledVertex[i].Diff;
            if( m_bPlayerIsBack )
            {
                if( dwAlpha > (0x81000000) )
                    dwAlpha -= (0x03000000);
                else
                    dwAlpha = (0x80000000);
            }
            else
            {
                if( dwAlpha < (0xfC000000) )
                    dwAlpha += (0x03000000);
                else
                    dwAlpha = (0xff000000);
            }
            ScrolledVertex[i].Diff = (m_Vertex[i].Diff & 0x00ffffff) | dwAlpha;
            m_Vertex[i].Diff = ScrolledVertex[i].Diff;
        }
    }
}

void KGCLayerPart::Render(D3DXMATRIX *mat)
{
    if( m_bShow == false )
        return;

	g_pGCDeviceManager2->DrawInWorld(m_pTexture,mat,ScrolledVertex.as_array() );	
}

bool KGCLayerPart::CheckCulling()
{
    if( m_iLayerNum == LAST_LAYER )
        return true;

    // 배경중심점이 카메라 앵글안에 들어오는지 확인하는 컬링은
    // 이글사이트 등으로 카메라 Wide값이 변할때 배경이 잘리는 위험이 있으므로
    // 주석처리합니다.
    // 아래 Rect와 Rect비교로 하는 컬링만으로도 충분하다고 판단됨!!

    //float fX = ( ScrolledVertex[0].Pos.x + ScrolledVertex[1].Pos.x ) * 0.5f - 1.0f;
    //float fY = ( ScrolledVertex[0].Pos.y + ScrolledVertex[2].Pos.y ) * 0.5f - g_kCamera.m_fLookTermY;

    //fX += fX * GetCMRadio_X();
    //fY += fY * GetCMRadio_Y();

    //if( g_kCamera.m_rtCamera.PtInRect( fX, fY ) == true )
    //    return true;

    float fMinX = ScrolledVertex[0].Pos.x - 1.0f;
    float fMaxX = ScrolledVertex[1].Pos.x - 1.0f;
    float fMinY = ScrolledVertex[2].Pos.y - g_kCamera.m_fLookTermY - 0.5f;
    float fMaxY = ScrolledVertex[1].Pos.y - g_kCamera.m_fLookTermY + 0.5f;

    fMinX -= fMinX * GetCMRadio_X();
    fMaxX += fMaxX * GetCMRadio_X();
    fMinY -= fMinY * GetCMRadio_Y();
    fMaxY += fMaxY * GetCMRadio_Y();

    return g_kCamera.m_rtCamera.RectInRect( fMinX, fMinY, fMaxX, fMaxY );
}

void KGCLayerPart::SetColor( DWORD dwColor )
{
    for( int i=0 ; i<4 ; ++i )
    {
        m_Vertex[i].Diff = (m_Vertex[i].Diff & 0xff000000) | dwColor;
    }
}

//void KGCLayerPart::SetColor( D3DXCOLOR& Color )
//{
//    for( int i=0 ; i<4 ; ++i )
//    {
//        m_Vertex[i].Diff = D3DCOLOR_RGBA( (BYTE)Color.r, (BYTE)Color.g, (BYTE)Color.b, 255 );
//    }
//}

//void KGCLayerPart::RenderEffect()
//{
//    if( m_pEffect != NULL )
//    {
//        g_pRenderState->EndPass();
//        SiKGCEffectManager()->Render( m_pEffect );
//        g_pRenderState->BeginPass(EGCRENDER_STATE_FAR_MAP_LINEAR);
//    }
//}
