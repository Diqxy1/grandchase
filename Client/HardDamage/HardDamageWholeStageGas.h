#pragma once
#include "damage.h"

class CHardDamageWholeStageGas :
	public CDamage
{
public:
	CHardDamageWholeStageGas(void);
	virtual ~CHardDamageWholeStageGas(void);
    virtual void BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY );
};
