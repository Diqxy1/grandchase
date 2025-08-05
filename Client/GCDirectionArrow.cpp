#include "stdafx.h"
#include"GCDirectionArrow.h"
//



#include "StructOfQuest.h"



KGCDirectionArrow::KGCDirectionArrow()
{
	Create();
}

KGCDirectionArrow::~KGCDirectionArrow()
{
	Destroy();
}

void KGCDirectionArrow::Destroy()
{
	for(int i=0;i<NUM_OF_ARROW;++i)
	{
		g_ParticleManager->DeleteSequence(m_pArrowParticle[i]);
		m_pArrowParticle[i] = NULL;
	}	
}

void KGCDirectionArrow::ShowArrowParticle(int iDirection, bool bShow)
{	
	switch( iDirection )
	{
	case DIRECTION_DOWN:
		if( m_pArrowParticle[ARROW_DOWN] && g_ParticleManager->IsLiveInstance(m_pArrowParticle[ARROW_DOWN]) )
			m_pArrowParticle[ARROW_DOWN]->SetShow( bShow );
		break;
	case DIRECTION_UP:
		if( m_pArrowParticle[ARROW_UP] && g_ParticleManager->IsLiveInstance(m_pArrowParticle[ARROW_UP]) )
			m_pArrowParticle[ARROW_UP]->SetShow( bShow );
		break;
	case DIRECTION_LEFT:
		if( m_pArrowParticle[ARROW_LEFT] && g_ParticleManager->IsLiveInstance(m_pArrowParticle[ARROW_LEFT]) )
			m_pArrowParticle[ARROW_LEFT]->SetShow( bShow );
		break;
	case DIRECTION_RIGHT:
		if( m_pArrowParticle[ARROW_RIGHT] && g_ParticleManager->IsLiveInstance(m_pArrowParticle[ARROW_RIGHT]) )
			m_pArrowParticle[ARROW_RIGHT]->SetShow( bShow );
		break;
	}
}


void KGCDirectionArrow::AllHideArrowParticle(void)
{
	for(int i=0;i<NUM_OF_ARROW;++i)
	{
		if( m_pArrowParticle[i] && g_ParticleManager->IsLiveInstance(m_pArrowParticle[i]) )
			m_pArrowParticle[i]->SetShow( false );
		else
			m_pArrowParticle[i] = NULL;
	}
}

void KGCDirectionArrow::SetPosition( float x,float y,float z )
{
	m_pArrowParticle[ARROW_DOWN]->SetPosition(x,y,z);
	m_pArrowParticle[ARROW_UP]->SetPosition(x,y,z);
	m_pArrowParticle[ARROW_LEFT]->SetPosition(x,y,z);
	m_pArrowParticle[ARROW_RIGHT]->SetPosition(x,y,z);
}

void KGCDirectionArrow::Create()
{
	m_pArrowParticle[ARROW_RIGHT] = g_ParticleManager->CreateSequence( "arrow_right01", m_fPosX , m_fPosY , 3.0f );
	m_pArrowParticle[ARROW_DOWN] = g_ParticleManager->CreateSequence( "arrow_down01", m_fPosX , m_fPosY, 3.0f );
	m_pArrowParticle[ARROW_LEFT] = g_ParticleManager->CreateSequence( "arrow_left01", m_fPosX , m_fPosY , 3.0f );
	m_pArrowParticle[ARROW_UP] = g_ParticleManager->CreateSequence( "arrow_up01", m_fPosX , m_fPosY , 3.0f );

	AllHideArrowParticle();
}