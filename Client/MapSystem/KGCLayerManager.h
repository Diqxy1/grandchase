#pragma once

#include <d3dx9.h>
#include "KLayerManager.h"
//#include "../ParticleLib/GCParticleEvent.h"
#include "KGCLayerPart.h"
#include "KGCLayer.h"
#include "GCFantasticMap.h"

class KGCLayerManager : public KLayerManager<KGCLayerPart, KGCLayer, KGCFantasticMap>
{
    DeclareSingleton(KGCLayerManager);
public:
    KGCLayerManager(void);
    virtual ~KGCLayerManager(void);

    virtual void        UnLoad();
    virtual void        LoadParts( KLuaManager& luaMgr );
    virtual void        SetPartsCategoryList();
    virtual bool        LoadFromLua( KLayerPartData& kData );
    virtual void        RegisterLuabind();
    virtual void        FrameMove( float fElapsedTime );
    virtual bool        LoadScript( std::string& Script );
    virtual void        SetLayer();
    virtual void        SetLayerSize( KLuaManager& luaMgr );

    void                AlphaRender();
	void NoAlphaRender( int iLayerNum_ );

    void                SetColor( DWORD dwColorNormal, DWORD dwColorSky );
    //void                SetColor( DWORD dwColor, int iKeepFrame );

public:
    void SetLayerMoveSpeed( int iLayer, char *szPartName, float fSpeedX, float fSpeedY );
    float GetLayerMoveSpeedX( int iLayer, char *szPartName );
    float GetLayerMoveSpeedY( int iLayer, char *szPartName );

private:
    //int                         m_iColorFrame;
    //D3DXCOLOR                   m_CurrColor;
    //D3DXCOLOR                   m_DefaultColor;
    //KGCParticleEvent<D3DXCOLOR> m_ColorEvent;
};

DefSingletonInline(KGCLayerManager);
