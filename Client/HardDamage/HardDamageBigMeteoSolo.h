#pragma once
#include "../Damage.h"

class CHardDamageBigMeteoSolo : public CDamage
{
private:
    int m_iFrameCount;
    int m_iTargetCount;
    bool bRight;
    int m_iWho;
public:
    CHardDamageBigMeteoSolo(void);
    ~CHardDamageBigMeteoSolo(void);
    
    virtual void BeginWithLocate( CDamageInstance* pDamageInstance, float fStartX , float fStartY );
    virtual void FrameMove( CDamageInstance* pDamageInstance );
};
