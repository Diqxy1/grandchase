#pragma once
#include "damage.h"

class CHardDamageStoneCurse :
	public CDamage
{
public:
	CHardDamageStoneCurse(void);
	virtual ~CHardDamageStoneCurse(void);

	virtual void Begin( CDamageInstance* pDamageInstance );
	virtual void FrameMove(CDamageInstance* pDamageInstance );
	virtual bool CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );
    virtual void CrashMonster( CDamageInstance* pDamageInstance, int Monsteri );

	int m_iMonsterIndex;
	int m_bIsCrashMonster;
};

class CHardDamageStoneBody :
	public CDamage
{
public:
	CHardDamageStoneBody(void){}
	virtual ~CHardDamageStoneBody(void){}

	virtual void FrameMove( CDamageInstance* pDamageInstance );
};
