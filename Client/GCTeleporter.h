#pragma once
#include "gcgameobject.h"
#include "animation/KTDGAnim.h"
#include "GCDirectionArrow.h"
#include "GCDoor.h"
//#include <set>

#define MARI_TELEPORT_PARTICEL_NUM 3

class PLAYER;
class CDamageInstance;


class KGCTeleporter :
	public KGCGameObject
{
public:
	KGCTeleporter(void);
	~KGCTeleporter(void);

	virtual void Create( void );
	virtual void Destroy( void );	
	virtual void FrameMove( float fElapsedTime = GC_ELAPSED_TIME );
	virtual void Render( void );
	virtual void Enable();

	static KGCTeleporter* CheckTeleporter(PLAYER * player);	
	static KGCTeleporter* GetNextTeleporter(KGCTeleporter* teleporter);	
    static std::set<KGCTeleporter*>& GetInstanceTeleporter() { return set_teleporter; }
    static KGCTeleporter* GetDotaStageInTeleporter( int iDotaStage );
    
	void SetPair(int i);
	D3DXVECTOR2 GetPos();

	void ShowEffect();

    void SetNowFrame( int iFrameIndex ) { m_pTeleporter->SetNowFrame(iFrameIndex); }
    void SetShowMariTeleportParticle( bool bShow )  { m_bIsMariTeleport = bShow; }

    void SetDotaStage( int iStage ) {   m_iDotaStage = iStage;  }
    int GetDotaStage()              {   return m_iDotaStage;    }

    CDamageInstance*                                 m_pDamage;
private:

	static std::set<KGCTeleporter*> set_teleporter;
	GCCollisionRect<float> GetCollision( void );
	
	CKTDGAnim::CAnimInstance*                        m_pTeleporter;
	CKTDGAnim::CAnimInstance*                        m_pArrow;

    KSafeArray<CParticleEventSeqPTR,MARI_TELEPORT_PARTICEL_NUM> m_pMariTelepoter;
  
	int		pair;

    bool    m_bIsMariTeleport;
    int     m_iDotaStage;

    CParticleEventSeqPTR     m_pCursorParticle1;   
    CParticleEventSeqPTR     m_pCursorParticle2;
    CParticleEventSeqPTR     m_pCursorParticle3;
    CParticleEventSeqPTR     m_pCursorParticle4;
};
