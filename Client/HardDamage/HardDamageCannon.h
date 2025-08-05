#pragma once
#include "damage.h"

class CHardDamageCannon :
	public CDamage
{
public:
	CHardDamageCannon(void);
	virtual ~CHardDamageCannon(void);
    virtual void Begin( CDamageInstance* pDamageInstance );
    virtual void CrashMonster( CDamageInstance* pDamageInstance, int Monsteri );

	//float m_fStartY;	//처음 시작할 때의 Y좌표이다.
	//float m_iTime;
};
