#pragma once

#include "stdafx.h"
#include "KLayerManager.h"
//#include "../ParticleLib/GCParticleEvent.h"
//#include "KQSLayerManager.h"
#include "KQSLayer.h"
#include "KQSLayerPart.h"
#include "QSFantasticMap.h"

class KQSLayerManager : public KLayerManager<KQSLayerPart, KQSLayer, KQSFantasticMap>
{
    DeclareSingleton(KQSLayerManager);
public:
    KQSLayerManager(void);
    ~KQSLayerManager(void);

    virtual void        UnLoad();
    virtual void        LoadParts( KLuaManager& luaMgr );
    virtual void        SetPartsCategoryList();
    virtual bool        LoadFromLua( KLayerPartData& kData );
    virtual void        RegisterLuabind();
    virtual void        FrameMove( float fElapsedTime );
    virtual bool        LoadScript( std::string Script );
    virtual void        SetLayer();
    virtual void        SetLayerSize( KLuaManager& luaMgr );

    void                AlphaRender();
    void                NoAlphaRender();

    void                SetColor( DWORD dwColorNormal, DWORD dwColorSky );
    //void                SetColor( DWORD dwColor, int iKeepFrame );

    D3DXMATRIX          m_matCamera;
    std::string         m_strScriptDir;
    void                SetCameraMatrix(D3DXMATRIX matCamera);
    void                SetCameraRectToQSLayer(float fCMLeft, float fCMRight, float fCMTop, float fCMBottom);
    void                SetCameraPosToQSLayer(float fLookX, float LookY);
    void                SetScriptDirectory(std::string strScriptDir);
    void                AddLayerPart( KQSLayerPart* pPart );

    GCDeviceShader      *g_pRenderState;    
    void                SetRenderState(void);
private:
    //int                         m_iColorFrame;
    //D3DXCOLOR                   m_CurrColor;
    //D3DXCOLOR                   m_DefaultColor;
    //KGCParticleEvent<D3DXCOLOR> m_ColorEvent;
};

DefSingletonInline(KQSLayerManager);
