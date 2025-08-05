#pragma once
#include "damage.h"

class CHardDamageRoseArrow : public CDamage
{
public:
    CHardDamageRoseArrow(void);
    virtual ~CHardDamageRoseArrow(void);

	virtual void CustomizeBeforeCreate( CDamageInstance* pDamageInstance );
};
