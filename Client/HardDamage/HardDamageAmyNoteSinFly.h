#pragma once
#include "damage.h"

class CHardDamageAmyNoteSinFly :
	public CDamage
{
public:
	CHardDamageAmyNoteSinFly(void);
	virtual ~CHardDamageAmyNoteSinFly(void);
	virtual void NotAngleFrameMove(CDamageInstance* pDamageInstance );
	float m_fStartY;	//처음 시작할 때의 Y좌표
	float m_iTime;		
    float m_fOffsetY;
};
