#include "stdafx.h"
#include "gcdoor.h"
#include "KGCRoomManager.h"

KGCDoor::KGCDoor(void)
{
	m_enDoorState   = IDS_NORMAL;
	m_pDirectionArrow	= NULL;
	m_iGateDelay        = 0;
	m_iDirection        = 1;
	m_bLockEnable       = false;
	m_iCurHit			= 0;
	m_iMaxHit           = 0;	
	m_iDamageHit        = 0;
	m_iCrashHit         = 0;

	m_eObjType          = GC_PORTAL;

	EnableLive(true);
}

KGCDoor::~KGCDoor(void)
{
	Destroy();
}

void KGCDoor::Create( void )
{
	m_pDirectionArrow = new KGCDirectionArrow();
	if ( false == IsInitEnable() )
	{		
		m_bLockEnable = true;
	}
	else
	{	
		m_bLockEnable = false;
	}
}

void KGCDoor::Destroy( void )
{
	SAFE_DELETE( m_pDirectionArrow );
}

void KGCDoor::Enable( void )
{
	m_bLockEnable = false;
	m_pDirectionArrow->ShowArrowParticle(m_iDirection,true);
}

void KGCDoor::FrameMove( float fElapsedTime /*= GC_ELAPSED_TIME */ )
{
	m_pDirectionArrow->ShowArrowParticle(m_iDirection,!(!IsLive() && (IDS_NORMAL == m_enDoorState)) );    
}

void KGCDoor::Render( void )
{

}

KGCDoor::IRON_DOOR_STATE KGCDoor::GetDoorState( void ) const
{
	return m_enDoorState;
}

void KGCDoor::SetDoorState( IRON_DOOR_STATE enState )
{
	switch( enState )
	{
	case IDS_NORMAL:
		m_pDirectionArrow->AllHideArrowParticle();		
		m_enDoorState = IDS_NORMAL;
		break;
	case IDS_DAMAGE:		
		m_enDoorState = IDS_DAMAGE;
		break;
	case IDS_OPEN:
		m_enDoorState = IDS_OPEN;
		EnableLive(false);		
		break;
	}
}

void KGCDoor::SendPacketCurDoorState( void )
{	
	if( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory()
		&& g_kGlobalValue.m_kUserInfo.bHost
		&& !g_kGateStage->IsUsed() )
	{
		g_kGateStage->SetGateInfo(GetIndex(), GetCurHit(), (int)m_enDoorState);
		g_kTGPacketPacker.Pack( g_kGateStage, sizeof(KGC_PID_GATE_STATE) );

		if (IDS_OPEN != m_enDoorState) g_kGateStage->Init();
	}
}