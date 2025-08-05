#pragma once
#include "damage.h"

class CHardDamageArmeSkillTest :
	public CDamage
{
public:
	CHardDamageArmeSkillTest(void);
	virtual ~CHardDamageArmeSkillTest(void);
	virtual void NotAngleFrameMove(CDamageInstance* pDamageInstance );
	virtual int MostClosedPlayerIndex( CDamageInstance* pDamageInstance);
	virtual int MostClosedMonsterIndex( CDamageInstance* pDamageInstance);
	virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate );
	//virtual void CrashMonster(CDamageInstance* pDamageInstance, int Monsteri );

public:
	int m_iTime;
	int m_xRate;
	int m_yRate;
    int m_iTargetNum;

	D3DXVECTOR3 m_vPos;
};
