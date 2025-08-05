#pragma once
#include "damage.h"

class CHardDamageSinFly :
	public CDamage
{
public:
	CHardDamageSinFly(void);
	virtual ~CHardDamageSinFly(void);
	virtual void NotAngleFrameMove(CDamageInstance* pDamageInstance );

	float m_fStartY;	//처음 시작할 때의 Y좌표이다.
	float m_iTime;		

	float m_fCyclopseY;	//싸이클롭스 돌이 땅에 떨어졌을 때의 Y좌표값. 이 외에는 쓰지마세요.
};
