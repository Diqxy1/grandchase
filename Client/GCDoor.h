#pragma once
#include "gcgameobject.h"
//#include "ParticleLib/KTDGParticleSystem.h"
//#include "animation/KTDGAnim.h"
#include "GCGameObject.h"
#include "GCDirectionArrow.h"


class KGCDoor :
	public KGCGameObject
{
public:
	KGCDoor(void);
	virtual ~KGCDoor(void);

	virtual void Create( void );
	virtual void Destroy( void );
	virtual void Enable( void );
	virtual void FrameMove( float fElapsedTime = GC_ELAPSED_TIME );
	virtual void Render( void );
	

	enum IRON_DOOR_STATE { IDS_NORMAL, IDS_DAMAGE, IDS_OPEN ,IDS_MOVE };
	IRON_DOOR_STATE m_enDoorState;
	IRON_DOOR_STATE GetDoorState( void ) const;
	virtual void SetDoorState( IRON_DOOR_STATE enState );
	void SendPacketCurDoorState(void);

	
	void SetGateDelay(int iGateDelay) { m_iGateDelay = iGateDelay; }
	int GetGateDelay(void) { return m_iGateDelay; }
	void SetDirection(int iDirection) { m_iDirection = iDirection; }
	int GetDirection(void) { return m_iDirection; }	

	void SetMaxHit(int iMaxHit) { m_iMaxHit = iMaxHit; }
	int GetMaxHit(void) { return m_iMaxHit; }    
	void SetDamageHit(int iDamageHit) { m_iDamageHit = iDamageHit; }
	int GetDamageHit(void) { return m_iDamageHit; }
	void SetCrashHit(int iCrashHit) { m_iCrashHit = iCrashHit; }
	int GetCrashHit(void) { return m_iCrashHit; }

	void SetCurHit(int iCurHit) { m_iCurHit = iCurHit; }
	int GetCurHit(void) { return m_iCurHit; }
	
protected:
	KGCDirectionArrow*								 m_pDirectionArrow;
	int												 m_iCurHit;
	bool					                        m_bLockEnable;
private:
	
	int                         m_iGateDelay;
	int                         m_iDirection;
	int                         m_iMaxHit;    
	int                         m_iDamageHit;   
	int							m_iCrashHit;	
};
