#pragma once
#include "damage.h"

class CHardDamageAmyNoteSinFly :
	public CDamage
{
public:
	CHardDamageAmyNoteSinFly(void);
	virtual ~CHardDamageAmyNoteSinFly(void);
	virtual void NotAngleFrameMove(CDamageInstance* pDamageInstance );
	float m_fStartY;	//ó�� ������ ���� Y��ǥ
	float m_iTime;		
    float m_fOffsetY;
};
