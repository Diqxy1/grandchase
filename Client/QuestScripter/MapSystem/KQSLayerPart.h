#pragma once
//#include <string>
#include "stdafx.h"
#include "KLayerPart.h"
#include "MapHeader.h"




/************************************************************************/
/*  KQSLayerPart가 해야하는 일들

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
//class KGCEffect;
#define LAST_LAYER (19)

class KQSLayerPart : public KLayerPart<KQSLayerPart>
{
public:
    KQSLayerPart(void);
    ~KQSLayerPart(void);

    FRect_Quest       m_rtCamera;

    virtual KQSLayerPart*           Clone();
    virtual void                    CopyMember( KQSLayerPart* pSrc );

    //virtual void                    OnEffectTrace( KGCEffect* pEffect );
    // 이 Render는 부르는 것이 아님
    virtual void                    Render();
    virtual void                    FrameMove( float fElapsedTime );
    //virtual void                    RenderEffect();

    virtual void                    Flush();

    virtual bool                    CheckCulling();

    void                            SetColor( DWORD dwColor );
    //void                            SetColor( D3DXCOLOR& Color );
    
    void                            SetCameraRect( FRect_Quest rtCamera );
    
    void                            Inverse( bool bLeftRight );
    bool                            IsMouseIn( D3DVECTOR MPos );
};
