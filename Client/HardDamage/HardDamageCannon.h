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

	//float m_fStartY;	//ó�� ������ ���� Y��ǥ�̴�.
	//float m_iTime;
};
