#pragma once
#include "Damage.h"

class CHardDamageBigMeteo : public CDamage
{
public:
    CHardDamageBigMeteo(void);
    ~CHardDamageBigMeteo(void);

    virtual void Begin( CDamageInstance* pDamageInstance );
    virtual void FrameMove( CDamageInstance* pDamageInstance );
	virtual void NotAngleFrameMove(CDamageInstance* pDamageInstance );
	virtual void End( CDamageInstance* pDamageInstance );

    void BeginOthers( CDamageInstance* pDamageInstance );
    void CrashMonster( CDamageInstance* pDamageInstance, int Monsteri );
	void Boom( CDamageInstance* pDamageInstance );

};
