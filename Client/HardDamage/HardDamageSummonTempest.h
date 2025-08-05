#pragma once
#include "../Damage.h"

class CHardDamageSummonTempest : public CDamage
{
private:
    bool bRight;
    bool bRenderParticle;
    D3DXVECTOR3 m_vPlayerPos;
    D3DXVECTOR3 m_vHead;
    D3DXVECTOR3 m_vTail;
    D3DXVECTOR3 m_vWaist;
    D3DXVECTOR3 m_vArticulationL;
    D3DXVECTOR3 m_vArticulationR;
    D3DXVECTOR3 m_vLance;

    KSafeArray<CParticleEventSeqPTR, 14> vecTemp;

public:
    CHardDamageSummonTempest(void);
    ~CHardDamageSummonTempest(void);

    virtual void Begin( CDamageInstance* pDamageInstance );
    virtual void FrameMove( CDamageInstance* pDamageInstance );
    virtual void End( CDamageInstance* pDamageInstance );
};
