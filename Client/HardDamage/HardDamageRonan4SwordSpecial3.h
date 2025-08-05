#pragma once
#include "damage.h"

class CHardDamageRonan4SwordSpecial3 :
	public CDamage
{
private:
	KSafeArray<D3DXVECTOR3,10> m_vSwordPos;
	KSafeArray<CParticleEventSeqPTR,9> pTemp1;
	KSafeArray<CParticleEventSeqPTR,9> pTemp2;
	CParticleEventSeqPTR pSwordCenter;
	int m_elapsedTime;
	KSafeArray<bool,3> m_isOnParticle;
public:
	CHardDamageRonan4SwordSpecial3(void);
	virtual ~CHardDamageRonan4SwordSpecial3(void);
	virtual void Begin( CDamageInstance* pDamageInstance );
	virtual void FrameMove( CDamageInstance* pDamageInstance );
	virtual void End( CDamageInstance* pDamageInstance );	
	virtual void CountBonePosWithTrace( CDamageInstance* pDamageInstance, float _x = 0.0f, float _y = 0.0f, float _z = 0.0f );	
	virtual void CreateParticle( CDamageInstance* pDamageInstance, std::string name, KSafeArray<CParticleEventSeqPTR,9>& pTemp );
	virtual void DeleteParticle();	
};