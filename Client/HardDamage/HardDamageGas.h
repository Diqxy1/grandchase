#pragma once
#include "damage.h"

class CHardDamageGas :
	public CDamage
{
public:
	CHardDamageGas(void);
	virtual ~CHardDamageGas(void);
	virtual void BeginWithLocate(CDamageInstance* pDamageInstance , float fStartX , float fStartY );
	virtual void NotAngleFrameMove(CDamageInstance* pDamageInstance );
	virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );

	float m_fOrgAngle;
};
