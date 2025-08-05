
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include<set>
#include "ParticleLib/KTDGParticleSystem.h"
#include "animation/KTDGAnim.h"
//#include "GCGameObject.h"
#include "GCDirectionArrow.h"
#include "GCDoor.h"

#ifndef GCPortal_h__
#define GCPortal_h__

#define	MAX_PLAYER_IN_PORTAL 4

class KGCPortal : public KGCDoor
{
public:
	KGCPortal(void);
	virtual ~KGCPortal(void);

	static KGCPortal* CheckPortal(PLAYER* player);
	static bool CheckOpen();
	static void UpdatePortal();
	static bool IsPortalInScreen();
	void ProcessTraceIcon();
	void AllPortalTraceIcon();
	void InitTraceIcon();

	virtual void Create(void);
	virtual void Destroy(void);
	virtual void Enable(void);
	virtual void FrameMove(float fElapsedTime = GC_ELAPSED_TIME);
	virtual void Render(void);

	virtual void SetDoorState(KGCDoor::IRON_DOOR_STATE enState);
	virtual bool IsCrashCheck(void);

	void SetType(int type) { m_type = type; }

	void ShowEffect();
	virtual bool IsCrashCheck(void) const { return false; }
	virtual GCCollisionRect<float> GetCollision(void);
	virtual GCCollisionRect<float> GetCollision2(void);


private:

	static std::set<KGCPortal*> portal_set;
	static int			portal_countdown;
	static int			framecount;

	CKTDGAnim::CAnimInstance* m_pPortal;

	CParticleEventSeqPTR     m_pParticle1;
	CParticleEventSeqPTR     m_pParticle2;
	CParticleEventSeqPTR     m_pParticle3;
	CParticleEventSeqPTR     m_pParticle4;
	CParticleEventSeqPTR     m_pParticle5;
	CParticleEventSeqPTR     m_pParticle6;

	CParticleEventSeqPTR     m_pCursorParticle1;
	CParticleEventSeqPTR     m_pCursorParticle2;
	CParticleEventSeqPTR     m_pCursorParticle3;
	CParticleEventSeqPTR     m_pCursorParticle4;


	bool											m_bEnter;
	std::vector<int>								m_vecUser;
	int							m_type;

	std::vector<CParticleEventSeqPTR> m_vecTraceIcon;
	D3DXVECTOR3 m_vTraceIconPos;
};

#endif // GCPortal_h__




