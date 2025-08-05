#pragma once

#include "KLayer.h"
#include "KGCLayerPart.h"

class KGCLayerManager;
//class KGCObj;
//class KGCCamera;

class KGCLayer : public KLayer<KGCLayerPart, KGCLayerManager>
{
public:
    KGCLayer( KGCLayerManager* pManager );
    virtual ~KGCLayer(void);

    virtual void AddParts( KLuaManager& luaMgr );
    virtual bool IsAlpha();

    virtual void        FrameMove( float fElapsedTime );
    virtual HRESULT     Render(D3DXMATRIX * matrix);

    virtual void        Process();

    //KGCCamera*          GetCurCamera();

    void                SetColor( DWORD dwColorNormal, DWORD dwColorSky );
    //void                SetColor( D3DXCOLOR& Color );

    //void                SetPlayerBack( KGCLayerPart* pPart, int iPlayerIndex );
    void                CreateBuffer();
    void                SetStaticCamera(bool bStatic) { m_bStaticCamera = bStatic; }

private:
    bool                m_bStaticCamera;
};
