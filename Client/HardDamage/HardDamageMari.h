#pragma once
#include "damage.h"


class CHardDamageTeleporter : public CDamage
{
public:
    CHardDamageTeleporter(void){};
    virtual ~CHardDamageTeleporter(void){};

    virtual void Begin( CDamageInstance* pDamageInstance );
    virtual void BeginWithAngle( CDamageInstance* pDamageInstance , float fStartAngle );
    virtual void BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY );
private:
    void Teleport( CDamageInstance* pDamageInstance );
};

class CHardDamagePortal : public CDamage
{
public:
    CHardDamagePortal(void){};
    virtual ~CHardDamagePortal(void){};

    virtual void End( CDamageInstance* pDamageInstance );

    static int iIndex;
};




