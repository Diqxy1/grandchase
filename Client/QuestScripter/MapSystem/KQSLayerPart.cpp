//#include "../stdafx.h"
#include "stdafx.h"
#include ".\KQSLayerPart.h"
#include "../DlgMapToObject.h"
//#include "../MyD3D.h"
//#include "GCCameraOrtho.h"
//#include <dbg/dbg.hpp>
//#include "GCGlobalValue.h"
//#include "../ParticleLib/GCEffectManager.h"
//#include "Camera/GCCamera.h"


KQSLayerPart::KQSLayerPart(void) : KLayerPart<KQSLayerPart>()
{
}

KQSLayerPart::~KQSLayerPart(void)
{
}

KQSLayerPart* KQSLayerPart::Clone()
{
    KQSLayerPart* pTemp = new KQSLayerPart();
    pTemp->CopyMember( this );
    return pTemp;
}

void KQSLayerPart::CopyMember( KQSLayerPart* pSrc )
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

    SetTexel();
}

//void KQSLayerPart::OnEffectTrace( KGCEffect* pEffect )
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

void KQSLayerPart::FrameMove( float fElapsedTime )
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

void KQSLayerPart::Flush()
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

void KQSLayerPart::Render()
{
    if( m_bShow == false )
        return;

    m_pTexture->SetDeviceTexture();
    g_pd3dDevice->SetFVF( SLVERTEX::FVF );
    g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP , 2, ScrolledVertex, sizeof( SLVERTEX ) );
}

bool KQSLayerPart::CheckCulling()
{
    return true;

    //if( m_iLayerNum == LAST_LAYER )
    //    return true;

    //float fX = ( ScrolledVertex[0].Pos.x + ScrolledVertex[1].Pos.x ) * 0.5f - 1.0f;
    //float fY = ( ScrolledVertex[0].Pos.y + ScrolledVertex[2].Pos.y ) * 0.5f - 0.5f;

    //if( m_rtCamera.PtInRect( fX, fY ) == true )
    //    return true;

    //float fMinX = ScrolledVertex[0].Pos.x - 1.0f;
    //float fMaxX = ScrolledVertex[1].Pos.x - 1.0f;
    //float fMinY = ScrolledVertex[2].Pos.y - 0.5f;
    //float fMaxY = ScrolledVertex[1].Pos.y - 0.5f;

    //return m_rtCamera.RectInRect( fMinX, fMinY, fMaxX, fMaxY );
}

void KQSLayerPart::SetColor( DWORD dwColor )
{
    for( int i=0 ; i<4 ; ++i )
    {
        m_Vertex[i].Diff = (m_Vertex[i].Diff & 0xff000000) | dwColor;
    }
}

//void KQSLayerPart::setcolor( d3dxcolor& color )
//{
//    for( int i=0 ; i<4 ; ++i )
//    {
//        m_vertex[i].diff = d3dcolor_rgba( (byte)color.r, (byte)color.g, (byte)color.b, 255 );
//    }
//}

//void kqslayerpart::rendereffect()
//{
//    if( m_peffect != null )
//    {
//        g_prenderstate->endpass();
//        sikgceffectmanager()->render( m_peffect );
//        g_prenderstate->beginpass(EGCRENDER_STATE_FAR_MAP_LINEAR);
//    }
//}

void KQSLayerPart::SetCameraRect( FRect_Quest rtCamera )
{
    m_rtCamera = rtCamera;
}

void KQSLayerPart::Inverse( bool bLeftRight )
{
    if( bLeftRight )
    {
        // 좌우
        m_iTexX = m_iTexX + m_iTexWidth;
        m_iTexWidth = -m_iTexWidth;
    }
    else
    {
        // 상하
        m_iTexY = m_iTexY + m_iTexHeight;
        m_iTexHeight = -m_iTexHeight;
    }

    SetTexel();
    SetVertex();
}

bool KQSLayerPart::IsMouseIn( D3DVECTOR MPos )
{
    if( (int)MPos.x >= m_fLayerX + m_fScrollX && (int)MPos.x <= m_fLayerX + m_fWidth + m_fScrollX )
    {
        if( m_fLayerY + m_fScrollY <= (int)MPos.y && m_fLayerY + m_fHeight + m_fScrollY >= (int)MPos.y )
        {
            return true;
        }
    }

    return false;
}
