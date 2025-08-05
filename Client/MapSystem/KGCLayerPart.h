#pragma once

//#include <string>
#include "KLayerPart.h"
#include "Particle/KGCParticleManager.h"

/************************************************************************/
/*  KGCLayerPart�� �ؾ��ϴ� �ϵ�

1. � �ؽ�ó�� 
2. � ��ǥ��
3. � ũ���
4. Layer ���� ���
5. � ũ���
6. � ���� �׷��� ���ΰ�? (O)

Ext.
7. LayerPart�� ������ �������� ����. (0)
8. Layer�� ���� �ٴٸ���? Position Reset�ϴ� ����� ����. (0)
9. Effect
/************************************************************************/
//class KGCEffect;
#define LAST_LAYER (19)


class KGCLayerPart : public KLayerPart<KGCLayerPart>
{
public:
    KGCLayerPart(void);
    virtual ~KGCLayerPart(void);

    virtual KGCLayerPart*           Clone();
    virtual void                    CopyMember( KGCLayerPart* pSrc );

    //virtual void                    OnEffectTrace( KGCEffect* pEffect );
    
    virtual void                    Render(D3DXMATRIX *mat);
	virtual void                    Render(){}
    virtual void                    FrameMove( float fElapsedTime );
    //virtual void                    RenderEffect();

    virtual void                    Flush();

    virtual bool                    CheckCulling();

    void                            SetColor( DWORD dwColor );
    //void                            SetColor( D3DXCOLOR& Color );

    std::vector< CParticleEventSeqPTR > m_vecParticleSeq;


};
