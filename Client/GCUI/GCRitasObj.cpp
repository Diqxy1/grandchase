#include "stdafx.h"
#include "GCRitasObj.h"

IMPLEMENT_CLASSNAME( KGCRitasObj );
IMPLEMENT_WND_FACTORY( KGCRitasObj );
IMPLEMENT_WND_FACTORY_NAME( KGCRitasObj, "gc_ritasobj" );


const int LEFTTIME_DEATH_OBJ = 10;


KGCRitasObj::KGCRitasObj( void )
{
	m_bLive = false;
	m_bCanCatch = false;
	m_bOnBottom = false;
	m_iFrameCnt = 0;
	m_iObjType = 0;
}

KGCRitasObj::~KGCRitasObj( void )
{
}

void KGCRitasObj::ActionPerformed( const KActionEvent& event )
{

}

void KGCRitasObj::OnCreate( void )
{
	m_bLive = true;
}

void KGCRitasObj::InitInfo(D3DXVECTOR2 vPos_ ,int iType , bool bLive_ )
{
	SetLive(bLive_);
	m_vWindowPos = vPos_;
	m_bCanCatch = false;
	m_bOnBottom = false;
	m_iFrameCnt = 0;
	m_iObjType = iType;
}

void KGCRitasObj::FrameMoveInEnabledState()
{
	if (false == m_bLive)
		return;

	if (m_bOnBottom)
	{
		if (m_iFrameCnt > LEFTTIME_DEATH_OBJ)
		{
			SetLive(false);
			SetFixedWindowLocalPos(D3DXVECTOR2(-1000.0f,-1000.0f));
		}
		else
		{
			m_iFrameCnt++;
		}
	}
}

void KGCRitasObj::SetLive( bool bLive_ )
{
	m_bLive = bLive_; 
}
