#pragma once
#include "damage.h"

class CHardDamageAmy1DivineMsg:
	public CDamage
{
public:
	CHardDamageAmy1DivineMsg(void){};
	virtual ~CHardDamageAmy1DivineMsg(void){};	

    virtual void BeginParticle( CDamageInstance* pDamageInstance );
    virtual bool CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );
};


class CHardDamageAmy1Abracadabra:
    public CDamage
{
public:
    CHardDamageAmy1Abracadabra(void){};
    virtual ~CHardDamageAmy1Abracadabra(void){};	
    virtual void End( CDamageInstance* pDamageInstance );
};

class CHardDamageAmy1AbracadabraDun:
    public CDamage
{
public:
    CHardDamageAmy1AbracadabraDun(void){};
    virtual ~CHardDamageAmy1AbracadabraDun(void){};
    virtual void End( CDamageInstance* pDamageInstance );
};