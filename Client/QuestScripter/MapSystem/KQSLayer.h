#pragma once
#include "KLayer.h"
#include "KQSLayerPart.h"



class KQSLayerManager;
//class KGCObj;
//class KGCCamera;

class KQSLayer : public KLayer<KQSLayerPart, KQSLayerManager>
{
public:
    KQSLayer( KQSLayerManager* pManager );
    virtual ~KQSLayer(void);

    virtual void AddParts( KLuaManager& luaMgr );
    virtual bool IsAlpha();

    virtual void        FrameMove( float fElapsedTime );
    virtual HRESULT     Render();

    virtual void        Process();

    float m_fLookX, m_fLookY;
    float m_fCMLeft, m_fCMRight, m_fCMTop, m_fCMBottom;

    //KGCCamera*          GetCurCamera();
    void                SetCameraPos(float fLookX, float fLookY);
    void                SetShowRect(float fCMLeft, float fCMRight, float fCMTop, float fCMBottom);

    void                SetColor( DWORD dwColorNormal, DWORD dwColorSky );
    //void                SetColor( D3DXCOLOR& Color );

    //void                SetPlayerBack( KQSLayerPart* pPart, int iPlayerIndex );
    void                CreateBuffer();
};
