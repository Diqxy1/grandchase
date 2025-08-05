#pragma once


#include "GCDeviceTexture.h"
#include "KD3DDeviceState.h"



class GCObject;

class KGCAfterImageRenderer
{
public:
    KGCAfterImageRenderer(void);
    ~KGCAfterImageRenderer(void);

    //D3DApplication 관련 함수 
    void    InitDeviceObjects();
    void    InvalidateDeviceObjects(void);
    void    DeleteDeviceObjects(void);
    void    RestoreDeviceObjects();

    bool    Init();
    void    RegisterObject( GCObject* pObj, int iMaxNotDrawFrame = 0 );
    void    UnRegisterObject( GCObject* pObj );
    void    Render();
    void    RenderAfterImage();
    bool    IsRegisterObject( GCObject* pObj );

private:
    //짠상 텍스쳐
    LPDIRECT3DSURFACE9  m_pAfterImageDepthSurface[2];
    LPDIRECT3DSURFACE9  m_pAfterImageTargetSurface[2];
    LPDIRECT3DTEXTURE9  m_pAfterImageTexture[2];

    BLURVERTEX          m_aTextureVertex[4];
    BLURVERTEX          m_aRenderVertex[4];

    std::set<GCObject*> m_setObject;

    bool    m_bCapable;
    int     m_DrawFrame;
    int     m_iMaxNotDrawFrame;
    int     m_iRenderCnt;
    
#if defined( USE_DYNAMIC_VERTEX_BUFFER )
    BLURVERTEX m_vRenderVertex[4];
#else    
    static GCDeviceVertexBuffer*	m_pRenderVertex;
#endif
    static GCDeviceVertexBuffer*	m_pTextureVertex;

};
