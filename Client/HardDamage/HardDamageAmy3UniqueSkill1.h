#pragma once
#include "damage.h"

class CHardDamageAmy3UniqueSkill1 :
	public CDamage
{
public:
	CHardDamageAmy3UniqueSkill1(void);
	virtual ~CHardDamageAmy3UniqueSkill1(void);
	virtual void AngleFrameMove( CDamageInstance* pDamageInstance );
public:
	float m_fPosX;
	float m_fPosY;
	float m_fStartAngle;
	float m_fPrevPosY;
	//int m_iTime;
};
