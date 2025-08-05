#pragma once
#include "damage.h"

class CHardDamageGuardianBall :
	public CDamage
{
public:
	CHardDamageGuardianBall(void);
	virtual ~CHardDamageGuardianBall(void);
	virtual void AngleFrameMove(CDamageInstance* pDamageInstance );
	virtual void BeginWithLocate(CDamageInstance* pDamageInstance , float fStartX , float fStartY );

	float m_fStart_X;
	float m_fStart_Y;
};
