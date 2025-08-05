#include "stdafx.h"
#include ".\gcjumpzone.h"
//









#include "DamageManager.h"


KGCJumpZone::KGCJumpZone(void)
{
	m_eObjType=GC_OBJ_JUMPZONE;
	m_bCrashCheck	= false;	
	m_pJumpParticle1 = NULL;
	m_pJumpEffectParticle1 = NULL;
	m_pJumpZone=NULL;
}

KGCJumpZone::~KGCJumpZone(void)
{
	Destroy();
}

void KGCJumpZone::Create( void )
{
	m_pJumpZone = g_MyD3D->m_pMapAnim->CreateInst( g_MyD3D->m_pMapAnim->GetSequence( "GameObjectBase" ) );

	D3DXVECTOR2 pos = D3DXVECTOR2(GetPosX(), GetPosY());	

	m_pJumpZone->GetMatrix()->Move( pos.x - 1.15f, pos.y-0.48f, 42.0f );
	m_pJumpZone->GetMatrix()->Scale( 0.0025f, 0.0025f, 1.0f );	
	m_pJumpZone->Start( CKTDGAnim::PT_LOOP );
	m_pJumpZone->SetNowFrame(0);
	m_pJumpZone->Wait();	
	m_pJumpZone->SetShow( true );

	m_pJumpParticle1 = g_ParticleManager->CreateSequence( "Jump_Zone01", 
		m_pJumpZone->GetMatrix()->GetXPos(),
		m_pJumpZone->GetMatrix()->GetYPos()+0.02f, 1.9f );
	m_pJumpParticle2 = g_ParticleManager->CreateSequence( "Jump_Zone02", 
		m_pJumpZone->GetMatrix()->GetXPos(),
		m_pJumpZone->GetMatrix()->GetYPos()+0.02f, 1.9f );
	m_pJumpParticle3 = g_ParticleManager->CreateSequence( "Jump_Zone03", 
		m_pJumpZone->GetMatrix()->GetXPos(),
		m_pJumpZone->GetMatrix()->GetYPos()+0.02f, 1.9f );
	
	m_pJumpParticle1->SetShow( true );        
	m_pJumpParticle2->SetShow( true );    
	m_pJumpParticle3->SetShow( true );	

	g_KDSound.LoadFromIndex(608);
}

void KGCJumpZone::Destroy( void )
{	
	if( g_MyD3D->m_pMapAnim )
	{
		g_MyD3D->m_pMapAnim->DeleteInst( m_pJumpZone );		
	}
	if( g_ParticleManager )
	{
		g_ParticleManager->DeleteSequence(m_pJumpParticle1);        	
		g_ParticleManager->DeleteSequence(m_pJumpParticle2);        	
		g_ParticleManager->DeleteSequence(m_pJumpParticle3);        	
      
	}
	m_pJumpZone			=NULL;
	m_pJumpParticle1     = NULL;
	m_pJumpParticle2     = NULL;
	m_pJumpParticle3     = NULL;	

	g_KDSound.Free(608);

}

void KGCJumpZone::Enable( void )
{

}

void KGCJumpZone::FrameMove( float fElapsedTime /*= GC_ELAPSED_TIME */ )
{
	for(int i = 0; i < MAX_PLAYER_NUM; i++ )
	{
		if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive  && 0!=g_MyD3D->MyPlayer[i]->m_cLife )
		{			
			GCCollisionRect<float> rect;
			if(g_MyD3D->MyPlayer[i]->IsContact &&
				g_MyD3D->MyPlayer[i]->GetCollisionRect().CheckCollision(GetCollision(),&rect))
			{
				if(g_MyD3D->MyPlayer[i]->y_Speed<m_fJump)
					g_MyD3D->MyPlayer[i]->y_Speed=m_fJump;

				// Sound & Effect 
				g_KDSound.Play( "608" );

				g_ParticleManager->CreateSequence( "Jump_Zone_effect01", 
					m_pJumpZone->GetMatrix()->GetXPos(),
					m_pJumpZone->GetMatrix()->GetYPos()+0.02f, 1.9f );
				g_ParticleManager->CreateSequence( "Jump_Zone_effect02", 
					m_pJumpZone->GetMatrix()->GetXPos(),
					m_pJumpZone->GetMatrix()->GetYPos()+0.02f, 1.9f );

			}		
		}
	}

	
}



void KGCJumpZone::Render( void )
{	
}

GCCollisionRect<float> KGCJumpZone::GetCollision( void )
{
	GCCollisionRect<float> kCollisionRect;

	float fHeight = m_pJumpZone->GetRealHeight()*0.01f;
	float fWidth = m_pJumpZone->GetRealWidth()*0.01f;

	float fX = m_pJumpZone->GetMatrix()->GetPos().x;
	float fY = m_pJumpZone->GetMatrix()->GetPos().y;

	kCollisionRect.m_Top = fY + fHeight + 0.2f;
	kCollisionRect.m_Bottom = fY+ 0.2f;

	kCollisionRect.m_Left = fX - ( fWidth);
	kCollisionRect.m_Right = fX + ( fWidth);

	return kCollisionRect;
}
