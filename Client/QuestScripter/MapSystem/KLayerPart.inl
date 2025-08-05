//#include "stdafx.h"

#define PIXELTOTEXEL_X(p) ( (float)((p) - 0.5f ) / (float) m_pTexture->GetWidth() )
#define PIXELTOTEXEL_Y(p) ( (float)((p) - 0.5f ) / (float) m_pTexture->GetHeight() )

// [8/23/2007 breadceo] for Map Rendering
// Layer 0 - 19
template <class T>
float KLayerPart<T>::ms_fZOrder[] = { 1.0f, 5.0f, 10.0f, 15.0f, 20.0f, 25.0f, 30.0f/*Player*/, 35.0f/*Monster,Npc*/, 40.0f, 45.0f, 50.0f, 55.0f, 60.0f, 65.0f, 70.0f, 75.0f, 80.0f, 85.0f, 90.0f, 95.0f };

template <class T>
KLayerPart<T>::KLayerPart(void) : m_strPartName(""), m_pTexture(NULL), m_fLayerX(0), m_fLayerY(0),
                               m_fWidth(0), m_fHeight(0), m_fSpeedX(0), m_fSpeedY(0), m_bMoveable(false),
                               m_iLayerNum(0), m_iZOrder(0), m_bPositionReset_X(false), m_bPositionReset_Y(false),
                               m_fScrollX(0), m_fScrollY(0),// m_pEffect(NULL), m_iXOffset(0), m_iYOffset(0),
                               m_iTexX(0), m_iTexY(0), m_iTexWidth(0), m_iTexHeight(0), m_iUserValue(-1),
                               m_strCategory(""), m_strUniqueName(""), 
                               //m_strEffectName(""),
                               m_bShow(true),
                               //m_bUseEffect(false), 
                               m_fZoffset(0), m_bPlayerIsBack(false)
{
    ZeroMemory( m_fUV, sizeof(float)*4 );
}

template <class T>
KLayerPart<T>::~KLayerPart(void)
{
    SAFE_RELEASE( m_pTexture );
    //if( m_pEffect != NULL )
    //{
    //    SiKGCEffectManager()->ClearTraceAll( (IGCEffectTrace*)this );
    //    m_pEffect = NULL;
    //}
}

//template <class T>
//void KLayerPart<T>::OnEffectDie()
//{
//    m_pEffect = NULL;
//}

template <class T>
void KLayerPart<T>::SetTexturePart( std::string TexFileName, const KLayerRect<int>& kRect )
{
    m_pTexture = g_pGCDeviceManager->CreateTexture( TexFileName );
    if( m_pTexture == NULL )
        return;
    m_strFileName = TexFileName;

    SetTextureRedirect( kRect );
}

template <class T>
void KLayerPart<T>::SetTextureRedirect( const KLayerRect<int>& kRect )
{
    m_iTexX = kRect.m_X;
    m_iTexY = kRect.m_Y;
    m_iTexWidth = kRect.m_Width;
    m_iTexHeight = kRect.m_Height;

    SetTexel();
}

//template <class T>
//void KLayerPart<T>::CreateChildEffect( std::string EffectName, int XOffset, int YOffset )
//{
//    if( EffectName != "" )
//    {
//        m_strEffectName = EffectName;
//        m_bUseEffect = true;
//
//        m_pEffect = SiKGCEffectManager()->Create( EffectName, (float)(m_iLayerX + XOffset), (float)(m_iLayerY + YOffset) );
//
//        if( m_pEffect != NULL )
//        {
//            m_pEffect->SetLayer( m_iLayerNum );
//            m_pEffect->SetTrace( (IGCEffectTrace*)this );
//        }
//    }
//}

template <class T>
void KLayerPart<T>::SetTexel()
{
    // [11/23/2007] breadceo. m_pTexture == NULL 일 수 있음
    if( m_pTexture == NULL )
        return;

    m_fUV[0] = PIXELTOTEXEL_X(m_iTexX);
    m_fUV[1] = PIXELTOTEXEL_Y(m_iTexY);
    m_fUV[2] = PIXELTOTEXEL_X(m_iTexX + m_iTexWidth);
    m_fUV[3] = PIXELTOTEXEL_Y(m_iTexY + m_iTexHeight);
}

template <class T>
void KLayerPart<T>::SetVertex()
{
    if( m_pTexture == NULL )
        return;

    float fZ = KLayerPart<T>::ms_fZOrder[m_iLayerNum] + m_fZoffset;

    m_Vertex[0].SetSLVER( (float)m_fLayerX,                 (float)(m_fLayerY + m_fHeight),    fZ,  m_fUV[0],  m_fUV[1], m_Vertex[0].Diff );
    m_Vertex[1].SetSLVER( (float)(m_fLayerX + m_fWidth),    (float)(m_fLayerY + m_fHeight),    fZ,  m_fUV[2],  m_fUV[1], m_Vertex[1].Diff );
    m_Vertex[2].SetSLVER( (float)m_fLayerX,                 (float)m_fLayerY,                  fZ,  m_fUV[0],  m_fUV[3], m_Vertex[2].Diff );
    m_Vertex[3].SetSLVER( (float)(m_fLayerX + m_fWidth),    (float)m_fLayerY,                  fZ,  m_fUV[2],  m_fUV[3], m_Vertex[3].Diff );
}

template <class T>
void KLayerPart<T>::SetMoveProp( bool bMoveable, float fSpeedX, float fSpeedY )
{
    m_bMoveable = bMoveable;
    m_fSpeedX = fSpeedX;
    m_fSpeedY = fSpeedY;
}

template <class T>
bool KLayerPart<T>::IsNeedToPositionReset_X( float LayerX, float LayerY )
{
    switch( (bool)(m_fSpeedX > 0) )
    {
    case FALSE:
        if( m_Vertex[2].Pos.x + m_fWidth < 0 )
            return true;
        break;
    case TRUE:
        if( m_Vertex[2].Pos.x > LayerX )
            return true;
        break;
    }
    return false;
}

template <class T>
void KLayerPart<T>::ResetPosition_X( float LayerX, float LayerY )
{
    switch( (bool)(m_fSpeedX > 0) )
    {
    case FALSE:
        SetLayerPos( KLayerRect<float>( LayerX - m_fWidth, m_fLayerY, m_fWidth, m_fHeight ) );
        SetVertex();
        break;
    case TRUE:
        SetLayerPos( KLayerRect<float>( 0, m_fLayerY, m_fWidth, m_fHeight ) );
        SetVertex();
        break;
    }	
}

template <class T>
bool KLayerPart<T>::IsNeedToPositionReset_Y( float LayerX, float LayerY )
{
    switch( (bool)(m_fSpeedY > 0) )
    {
    case FALSE:
        if( m_Vertex[2].Pos.y + m_fHeight < 0 )
            return true;
        break;
    case TRUE:
        if( m_Vertex[2].Pos.y > LayerY )
            return true;
        break;
    }
    return false;
}

template <class T>
void KLayerPart<T>::ResetPosition_Y( float LayerX, float LayerY )
{
    switch( (bool)(m_fSpeedY > 0) )
    {
    case FALSE:
        SetLayerPos( KLayerRect<float>( m_fLayerX, LayerY - m_fHeight, m_fWidth, m_fHeight ) );
        SetVertex();
        break;
    case TRUE:
        SetLayerPos( KLayerRect<float>( m_fLayerX, 0, m_fWidth, m_fHeight ) );
        SetVertex();
        break;
    }	
}

template <class T>
void KLayerPart<T>::ProcessRepeat( float LayerX, float LayerY )
{
    if( GetRepeat_X() )
    {
        if( IsNeedToPositionReset_X( LayerX, LayerY ) )
            ResetPosition_X( LayerX, LayerY );
    }

    if( GetRepeat_Y() )
    {
        if( IsNeedToPositionReset_Y( LayerX, LayerY ) )
            ResetPosition_Y( LayerX, LayerY );
    }
}

template <class T>
void KLayerPart<T>::SetParticle( std::vector<KLayerParticleData> vecParticle_ )
{
    m_vecParticle = vecParticle_;
}

template <class T>
void KLayerPart<T>::AddParticle( std::string strName_, float fOffsetX_, float fOffsetY_ )
{
    KLayerParticleData partcle;
    partcle.strParticleName = strName_;
    partcle.fOffsetX = fOffsetX_;
    partcle.fOffsetY = fOffsetY_;

    m_vecParticle.push_back( partcle );
}

template <class T>
void KLayerPart<T>::DeleteParticle( DWORD dwIndex_ )
{
    if ( dwIndex_ >= m_vecParticle.size() )
    {
        return;
    }
    m_vecParticle.erase( m_vecParticle.begin() + dwIndex_ );
}
