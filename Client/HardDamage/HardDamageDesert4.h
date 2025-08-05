#pragma once
#include "damage.h"

class CHardDamageDesert4DungFly:
	public CDamage
{
public:
	CHardDamageDesert4DungFly(void);
	virtual ~CHardDamageDesert4DungFly(void);
	virtual void BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY );
	virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate );
	virtual void CrashMonster(CDamageInstance* pDamageInstance, int Monsteri );
};

class CHardDamageDesert4Pot:
    public CDamage
{
public:
    CHardDamageDesert4Pot(void);
    virtual ~CHardDamageDesert4Pot(void);
    virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate );
    virtual void CrashMonster(CDamageInstance* pDamageInstance, int Monsteri );
};