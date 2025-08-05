#pragma once
#include "damage.h"

class CHardDamageHolyPole:
	public CDamage
{
public:
	CHardDamageHolyPole(void);
	virtual ~CHardDamageHolyPole(void);
	virtual void Begin( CDamageInstance* pDamageInstance );
	virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate );
	virtual void CrashMonster(CDamageInstance* pDamageInstance, int Monsteri );
};


class CHardDamageDivineTreeSonicBomb:
	public CDamage
{
public:
	CHardDamageDivineTreeSonicBomb(void);
	virtual ~CHardDamageDivineTreeSonicBomb(void);
	virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate );
};

// HardDamage 제거작업 시작
// class MONSTER;
// class CHardDamageDivineTreeAcidRain:
// 	public CDamage
// {
// public:
// 	CHardDamageDivineTreeAcidRain(void);
// 	virtual ~CHardDamageDivineTreeAcidRain(void);
// 	virtual void Begin( CDamageInstance* pDamageInstance );
// 	void NotAngleFrameMove( CDamageInstance* pDamageInstance );
// 	void End( CDamageInstance* pDamageInstance );
// 
// private:
// 	MONSTER* pMonster;
// };
// HardDamage 제거작업 끝

class CHardDamageSunkenArea:
	public CDamage
{
public:
	CHardDamageSunkenArea(void);
	virtual ~CHardDamageSunkenArea(void);
	virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate );
};