#pragma once
#include "damage.h"

class CHardDamageRonanMeteo : public CDamage
{
public:
    CHardDamageRonanMeteo(void);
    virtual ~CHardDamageRonanMeteo(void);

    virtual void Begin( CDamageInstance* pDamageInstance );
    virtual void BeginWithLocate( CDamageInstance* pDamageInstance, float fStartX , float fStartY );
};


class CHardDamageRonanMeteorLv3Rune : public CDamage
{
public:
    CHardDamageRonanMeteorLv3Rune(void);
    virtual ~CHardDamageRonanMeteorLv3Rune(void);

    virtual void Begin( CDamageInstance* pDamageInstance );
    virtual void BeginWithLocate( CDamageInstance* pDamageInstance, float fStartX , float fStartY );
    virtual void NotAngleFrameMove( CDamageInstance* pDamageInstance );
};

class CHardDamageRonanEnergyRune : public CDamage
{
public:
    CHardDamageRonanEnergyRune(void){};
    virtual ~CHardDamageRonanEnergyRune(void){};

    virtual void NotAngleFrameMove( CDamageInstance* pDamageInstance );
};


// HardDamage 제거작업 시작
// class CHardDamageRonanEnergyCircle : public CDamage
// {
// public:
//     CHardDamageRonanEnergyCircle(void){};
//     virtual ~CHardDamageRonanEnergyCircle(void){};
// };

// HardDamage 제거작업 끝