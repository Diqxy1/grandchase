#pragma once
#include "damage.h"

class CHardDamageRonan4MonoVolt :
	public CDamage
{
public:
	CHardDamageRonan4MonoVolt(void);
	virtual ~CHardDamageRonan4MonoVolt(void);
	//virtual void Begin( CDamageInstance* pDamageInstance );
	virtual void AngleFrameMove( CDamageInstance* pDamageInstance );
	//virtual void End( CDamageInstance* pDamageInstance );
public:
	float m_fPosX;
	float m_fPosY;
	float m_fStartAngle;
	bool m_isOn;
};
