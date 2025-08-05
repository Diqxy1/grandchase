#pragma once
#include "damage.h"

class CHardDamageBermesiaFinalEarthQuake :
	public CDamage
{
public:
	CHardDamageBermesiaFinalEarthQuake(void);
	virtual ~CHardDamageBermesiaFinalEarthQuake(void);
    virtual void FrameMove( CDamageInstance* pDamageInstance );
    virtual void BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY );

    void AddParticle( CDamageInstance* pDamageInstance , char* strSeqeunce, float fX , float fY );

    bool GetParent() const { return m_bParent; }
    void SetParent(bool val) { m_bParent = val; }
    float GetStartX() const { return m_fStartX; }
    float GetStartY() const { return m_fStartY; }

private:
    bool            m_bParent;

    float           m_fStartX;
    float           m_fStartY;
    short           m_sDelay;
};
