#pragma once
#include "damage.h"

class CHardDamageRonan4SummonValkyrie:
	public CDamage
{
public:
	CHardDamageRonan4SummonValkyrie(void);
	virtual ~CHardDamageRonan4SummonValkyrie(void);
	virtual void Begin( CDamageInstance* pDamageInstance );
	virtual void FrameMove( CDamageInstance* pDamageInstance );
public:
	bool m_bIsLastMotion;
};