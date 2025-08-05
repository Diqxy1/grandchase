#pragma once
#include "damage.h"

class CHardDamageRyanPoisonSpore :
	public CDamage
{
public:
	CHardDamageRyanPoisonSpore(void);
	virtual ~CHardDamageRyanPoisonSpore(void);

	virtual bool CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate /* = 1.0f */ );
	virtual void CrashMonster( CDamageInstance* pDamageInstance, int Monsteri );
	virtual void FrameMove( CDamageInstance* pDamageInstance );
	virtual void AngleFrameMove( CDamageInstance* pDamageInstance );
	virtual void End( CDamageInstance* pDamageInstance );
	virtual void BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY );

private:
	float m_fStartAngle;
	bool m_isOn;
};
