#pragma once
#include "Damage.h"

class CHardDamagePhoenixArrow :
	public CDamage
{
public:
	CHardDamagePhoenixArrow(void);
	virtual ~CHardDamagePhoenixArrow(void);

	virtual void Begin( CDamageInstance* pDamageInstance );
	virtual void NotAngleFrameMove( CDamageInstance* pDamageInstance );
};



class CHardDamageLire2Drill :
	public CDamage
{
public:
	CHardDamageLire2Drill(void);
	virtual ~CHardDamageLire2Drill(void);

	virtual void Begin( CDamageInstance* pDamageInstance );
	virtual void FrameMove( CDamageInstance* pDamageInstance );

	virtual bool CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );
	virtual void CrashMonster( CDamageInstance* pDamageInstance, int Monsteri );
	virtual void CrashObject( CDamageInstance* pDamageInstance, int iObjIndex );
	virtual void CrashEffect( CDamageInstance* pDamageInstance );

	float m_fOriginalSpeedX;
	float m_fOriginalSpeedY;
	bool m_bHitted;
    bool m_bFirst;

    int m_iHittedMonsterID;
};


