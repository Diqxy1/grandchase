#pragma once

#include "stdafx.h"
//#include "../ParticleLib/IGCEffectTrace.h"
//#include "../ParticleLib/GCEffectManager.h"
//#include "../GCDeviceLib/GCDeviceTexture.h"
//#include "../GCDeviceLib/GCDeviceManager.h"
//#include "../Vertex.h"

// [8/23/2007 breadceo] for Map Rendering
#define GC_ZORDER_NUM 20

/************************************************************************/
/*  KLayerPart가 해야하는 일들

1. 어떤 텍스처의 
2. 어떤 좌표에
3. 어떤 크기로
4. Layer 상의 어디에
5. 어떤 크기로
6. 어떤 모드로 그려줄 것인가? (O)

Ext.
7. LayerPart의 각각에 움직임을 주자. (0)
8. Layer의 끝에 다다르면? Position Reset하는 기능을 넣자. (0)
9. Effect
/************************************************************************/

template< typename T>
struct KLayerRect
{
    T m_X;
    T m_Y;
    T m_Width;
    T m_Height;

    KLayerRect()
    {
        m_X = 0;
        m_Y = 0;
        m_Width = 0;
        m_Height = 0;
    }
    KLayerRect( T fX, T fY, T fWidth, T fHeight )
    {
        m_X = fX;
        m_Y = fY;
        m_Width = fWidth;
        m_Height = fHeight;
    }

    bool operator != ( const KLayerRect& _Right )
    {
        return !(m_X == _Right.m_X && m_Y == _Right.m_Y &&
            m_Width == _Right.m_Width && m_Height == _Right.m_Height);
    }
};

struct KLayerParticleData
{
    std::string strParticleName;
    float fOffsetX;
    float fOffsetY;

    KLayerParticleData()
    {
        strParticleName = "";
        fOffsetX = 0;
        fOffsetY = 0;
    }

    void SetName( char* strName ) { strParticleName = strName; }
    void Init()
    {
        strParticleName = "";
        fOffsetX = 0;
        fOffsetY = 0;
    }
};

template <class T>
class KLayerPart// : public IGCEffectTrace
{
public:
    KLayerPart(void);
    virtual ~KLayerPart(void);

    virtual void        Render() = 0;
    virtual void        FrameMove( float fElapsedTime ) = 0;
    //virtual void        RenderEffect() = 0;
    virtual void        Flush() = 0;

    virtual T*          Clone() = 0;
    virtual void        CopyMember( T* pSrc ) = 0;

    virtual void        SetTexturePart( std::string TexFileName, const KLayerRect<int>& kRect );
    virtual void        SetTextureRedirect( const KLayerRect<int>& kRect );
    virtual void        SetLayerPos( const KLayerRect<float>& kRect )
    { 
        m_fLayerX = kRect.m_X;
        m_fLayerY = kRect.m_Y;
        m_fWidth = kRect.m_Width;
        m_fHeight = kRect.m_Height;
    }
    //virtual void        CreateChildEffect( std::string EffectName, int XOffset, int YOffset );

    virtual void        SetMoveProp( bool bMoveable, float fSpeedX, float fSpeedY );
    virtual void        GetMoveProp( bool &bMoveable, float &fSpeedX, float &fSpeedY )
    {
        bMoveable = m_bMoveable;
        fSpeedX = m_fSpeedX;
        fSpeedY = m_fSpeedY;
    }

    virtual bool        GetMoveable() const { return m_bMoveable; }

    virtual int         GetLayer(){ return m_iLayerNum; }
    virtual void        SetLayer( int iLayer ){ m_iLayerNum = iLayer; }
    virtual void        SetZOrder( int ZOrder ) { m_iZOrder = ZOrder; }
    virtual int         GetZOrder() const { return m_iZOrder; }

    virtual void        SetRepeat_X( bool Repeat ) { m_bPositionReset_X = Repeat; }
    virtual bool        GetRepeat_X() { return m_bPositionReset_X; }
    virtual void        SetRepeat_Y( bool Repeat ) { m_bPositionReset_Y = Repeat; }
    virtual bool        GetRepeat_Y() { return m_bPositionReset_Y; }

    virtual void        SetScrollPos( float fScrollX, float fScrollY )
    {
        m_fScrollX = fScrollX;
        m_fScrollY = fScrollY;
    }
    virtual void        GetScrollPos( float& fScrollX, float& fScrollY )
    {
        fScrollX = m_fScrollX;
        fScrollY = m_fScrollY;
    }

    virtual void        SetPartName( std::string PartName ) { m_strPartName = PartName; }
    virtual std::string GetPartName() { return m_strPartName; }

    virtual void        SetVertex();

    virtual void        GetLayerRect( KLayerRect<float>& kRect )
    {
        kRect.m_X = m_fLayerX;
        kRect.m_Y = m_fLayerY;
        kRect.m_Width = m_fWidth;
        kRect.m_Height = m_fHeight;
    }
    virtual void        GetTexRect( KLayerRect<int>& kRect )
    {
        kRect.m_X = m_iTexX;
        kRect.m_Y = m_iTexY;
        kRect.m_Width = m_iTexWidth;
        kRect.m_Height = m_iTexHeight;
    }

    virtual void        SetPartCategory( std::string Category ) { m_strCategory = Category; }
    virtual std::string GetPartCategory() { return m_strCategory; }

    virtual void        SetFileName( std::string fileName ) { m_strFileName = fileName; }
    virtual std::string GetFileName() { return m_strFileName; }

    virtual GCDeviceTexture*    GetTexture() { return m_pTexture; }

    virtual void        SetObjectName( std::string ObjName ) { m_strUniqueName = ObjName; }
    virtual std::string GetObjectName() { return m_strUniqueName; }

    virtual void        SetUserValue( int iValue ) { m_iUserValue = iValue; }
    virtual int         GetUserValue() { return m_iUserValue; }

    virtual void        SetUserString( std::string strUserString ) { m_strUserString = strUserString; }
    virtual std::string GetUserString() { return m_strUserString; }

    virtual bool        IsNeedToPositionReset_X( float LayerX, float LayerY );
    virtual void        ResetPosition_X( float LayerX, float LayerY );
    virtual bool        IsNeedToPositionReset_Y( float LayerX, float LayerY );
    virtual void        ResetPosition_Y( float LayerX, float LayerY );

    virtual void        ProcessRepeat( float LayerX, float LayerY );

    virtual void        SetShow( bool Show ) { m_bShow = Show; }
    virtual bool        GetShow() { return m_bShow; }

    virtual void        SetMoveable( bool Moveable ) { m_bMoveable = Moveable; }

    //virtual void        SetUseEffect( bool UseEffect ) { m_bUseEffect = UseEffect; }
    //virtual bool        GetUseEffect() { return m_bUseEffect; }

    //virtual std::string GetEffectName() { return m_strEffectName; }
    //virtual void        GetEffectPos( int& X, int& Y ) { X = m_iXOffset; Y = m_iYOffset; }

    virtual void        SetTexel();

    //virtual void        OnEffectTrace( KGCEffect* pEffect, void* pTraceData ){}
    //virtual void        OnEffectDie();

    virtual void        SetPlayerBack( bool PlayerBack ) { m_bPlayerIsBack = PlayerBack; }
    virtual bool        GetPlayerBack() { return m_bPlayerIsBack; }

    virtual float       GetZoffset() const { return m_fZoffset; }
    virtual void        SetZoffset(float val) { m_fZoffset = val; }

    const SLVERTEX*     GetRenderVertex() const
    {
        return ScrolledVertex;
    }

    virtual bool        CheckCulling() { return true; }

    void                SetParticle( std::vector<KLayerParticleData> vecParticle_ );
    void                AddParticle( std::string strName_, float fOffsetX_, float fOffsetY_ );
    void                DeleteParticle( DWORD dwIndex_ );

    DWORD               GetParticleCount() { return m_vecParticle.size(); }
    KLayerParticleData  GetParticleData( int iIndex_ ) 
    { 
        if ( (int)m_vecParticle.size() <= iIndex_ || iIndex_ < 0 )
        {
            return KLayerParticleData();
        }
        return m_vecParticle[iIndex_];
    }


protected:
    // PartName
    std::string         m_strPartName;
    std::string         m_strCategory;
    std::string         m_strUniqueName;

    // Texture
    std::string         m_strFileName;
    GCDeviceTexture*    m_pTexture;
    int                 m_iTexX;
    int                 m_iTexY;
    int                 m_iTexWidth;
    int                 m_iTexHeight;

    // uv, Vertex
    SLVERTEX            m_Vertex[4];
    float               m_fUV[4];

    // Layer
    float               m_fLayerX;
    float               m_fLayerY;
    float               m_fWidth;
    float               m_fHeight;

    // Move
    bool                m_bMoveable;
    float               m_fSpeedX;
    float               m_fSpeedY;

    int                 m_iLayerNum;
    int                 m_iZOrder;
    bool                m_bPositionReset_X;
    bool                m_bPositionReset_Y;

    // Scroll
    float               m_fScrollX;
    float               m_fScrollY;

    // Effect
    //std::string         m_strEffectName;
    //bool                m_bUseEffect;
    //KGCEffect*          m_pEffect;
    //int                 m_iXOffset;
    //int                 m_iYOffset;

    bool                m_bShow;
    int                 m_iUserValue;
    std::string         m_strUserString;

    bool                m_bPlayerIsBack;

    SLVERTEX            ScrolledVertex[4];
    float               m_fZoffset;

    std::vector< KLayerParticleData >   m_vecParticle;

public:
    // [8/23/2007 breadceo] for Map Rendering
    static float        ms_fZOrder[GC_ZORDER_NUM];
};

#include "KLayerPart.inl"
