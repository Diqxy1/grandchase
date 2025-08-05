#pragma once
#include "damage.h"

class CHardDamageDarkTemplarDoubleFrame:
	public CDamage
{
public:
	CHardDamageDarkTemplarDoubleFrame(void);
	virtual ~CHardDamageDarkTemplarDoubleFrame(void);
	virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate );
};

class CHardDamageIblisLaser:
	public CDamage
{
public:
	CHardDamageIblisLaser(void);
	virtual ~CHardDamageIblisLaser(void);
	virtual void BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY );
	virtual void NotAngleFrameMove( CDamageInstance* pDamageInstance );

private:
	int iMaxnum;
	int iPlayerNum;
	std::vector<int> vecLiveIndex;
	MONSTER* pMonster;
};