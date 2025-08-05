#pragma once
#include "damage.h"

class CHardDamageJin4Ball :
	public CDamage
{
public:
    CHardDamageJin4Ball(void){};
    virtual ~CHardDamageJin4Ball(void){};
	virtual void FrameMove(CDamageInstance* pDamageInstance );
};

class CHardDamageJin4Sucker :
    public CDamage
{
public:
    CHardDamageJin4Sucker(void){};
    virtual ~CHardDamageJin4Sucker(void){};

    virtual bool CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate );
    virtual void CrashMonster( CDamageInstance* pDamageInstance, int Monsteri );

};

class CHardDamageJin4BurnningSucker :
    public CDamage
{
public:
    CHardDamageJin4BurnningSucker(void){};
    virtual ~CHardDamageJin4BurnningSucker(void){};

};

