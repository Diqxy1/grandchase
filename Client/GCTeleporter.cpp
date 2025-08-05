#include "stdafx.h"
#include ".\gcteleporter.h"
#include "MyD3D.h"
#include "Damage.h"

std::set<KGCTeleporter*> KGCTeleporter::set_teleporter;

KGCTeleporter::KGCTeleporter(void)
{
	m_pArrow			= NULL;
	m_pTeleporter		= NULL;
	m_eObjType          = GC_TELEPORTER;
	set_teleporter.insert(this);
	pair=-1;	

	m_pDamage = NULL;

    m_pMariTelepoter[0] = NULL;
    m_pMariTelepoter[1] = NULL;

    m_bIsMariTeleport = false;
    m_iDotaStage = -1;

    m_pCursorParticle1 = NULL;
    m_pCursorParticle2 = NULL;
    m_pCursorParticle3 = NULL;
    m_pCursorParticle4 = NULL;
}

KGCTeleporter::~KGCTeleporter(void)
{
	set_teleporter.erase(this);
	Destroy();
}

void KGCTeleporter::Create( void )
{
	m_pTeleporter = g_MyD3D->m_pMapAnim->CreateInst( g_MyD3D->m_pMapAnim->GetSequence( "GameObjectBase" ) );
	m_pArrow = g_MyD3D->m_pMapAnim->CreateInst( g_MyD3D->m_pMapAnim->GetSequence( "PotalArrow" ));    

	D3DXVECTOR2 pos = D3DXVECTOR2(GetPosX(), GetPosY());	
   
    if( SiKGCRoomManager()->IsMonsterGameMode() )
        m_pTeleporter->GetMatrix()->Move( pos.x - 1.15f, pos.y-0.45f, 42.0f );
    else
        m_pTeleporter->GetMatrix()->Move( pos.x - 1.15f, pos.y-0.45f, 1.0f );
    m_pTeleporter->GetMatrix()->Scale( 0.0025f, 0.0025f, 1.0f );	
    m_pTeleporter->Start( CKTDGAnim::PT_LOOP );
    m_pTeleporter->SetNowFrame(1);
    m_pTeleporter->Wait();	
    m_pTeleporter->SetShow( true );	

    if( SiKGCRoomManager()->IsMonsterGameMode() )
        m_pArrow->GetMatrix()->Move( pos.x - 1.16f, pos.y-0.4f, 10.0f );
    else
        m_pArrow->GetMatrix()->Move( pos.x - 1.16f, pos.y-0.4f, 0.5f );
    m_pArrow->GetMatrix()->Scale( 0.0025f, 0.0025f, 1.0f );	
    m_pArrow->Start( CKTDGAnim::PT_LOOP );	
    m_pTeleporter->SetShow( true );	

    if( GC_GM_DOTA == SiKGCRoomManager()->GetGameMode() )
        g_KDSound.LoadFromIndex(985022);
    else
    g_KDSound.LoadFromIndex(609);

    // 마리 텔레포트 활성화 파티클
    char str[MAX_PATH] = {0,};
    for ( int i = 0; i < MARI_TELEPORT_PARTICEL_NUM; ++i )
    {
        sprintf_s( str, "Mari03_Portal_Effect_0%d", i + 1 );
        m_pMariTelepoter[i] = g_ParticleManager->CreateSequence( str, 
            m_pTeleporter->GetMatrix()->GetXPos(),
            m_pTeleporter->GetMatrix()->GetYPos(), 1.9f );
    }

    for( int i = 0; i < MARI_TELEPORT_PARTICEL_NUM; i++ )
    {
        if( m_pMariTelepoter[i] )
            m_pMariTelepoter[i]->SetShow(false);
    }
#if defined(NATION_CHINA)
    m_pCursorParticle1 = g_ParticleManager->CreateSequence( "cursor01", 
        m_pTeleporter->GetMatrix()->GetXPos(),
        m_pTeleporter->GetMatrix()->GetYPos()+0.35f, 1.9f );
    m_pCursorParticle2 = g_ParticleManager->CreateSequence( "cursor02", 
        m_pTeleporter->GetMatrix()->GetXPos(),
        m_pTeleporter->GetMatrix()->GetYPos()+0.35f, 1.9f );
    m_pCursorParticle3 = g_ParticleManager->CreateSequence( "cursor03", 
        m_pTeleporter->GetMatrix()->GetXPos(),
        m_pTeleporter->GetMatrix()->GetYPos()+0.35f, 1.9f );
    m_pCursorParticle4 = g_ParticleManager->CreateSequence( "cursor04", 
        m_pTeleporter->GetMatrix()->GetXPos(),
        m_pTeleporter->GetMatrix()->GetYPos()+0.35f, 1.9f );

    m_pCursorParticle1->SetShow(false);
    m_pCursorParticle2->SetShow(false);
    m_pCursorParticle3->SetShow(false);
    m_pCursorParticle4->SetShow(false);
#endif
}

void KGCTeleporter::Destroy( void )
{	
	if( g_MyD3D->m_pMapAnim )
	{
		g_MyD3D->m_pMapAnim->DeleteInst( m_pTeleporter );		
        g_MyD3D->m_pMapAnim->DeleteInst( m_pArrow );		
	}

    if( GC_GM_DOTA == SiKGCRoomManager()->GetGameMode() )
        g_KDSound.Free(985022);
    else
	g_KDSound.Free(609);

	m_pTeleporter= NULL;
    m_pArrow = NULL;

	
    if( g_MyD3D->m_pDamageManager->IsExistDamage( m_pDamage ) )
    {
        if( 2 < m_pDamage->m_Life )
            m_pDamage->m_Life = 2;
    }

    m_pDamage = NULL;

    for( int i = 0; i < MARI_TELEPORT_PARTICEL_NUM; i++ ) 
    {
        g_ParticleManager->DeleteSequence( m_pMariTelepoter[i] );
        m_pMariTelepoter[i] = NULL;
    }   

    m_bIsMariTeleport = false;

    if( g_ParticleManager )
    {
        g_ParticleManager->DeleteSequence(m_pCursorParticle1);
        g_ParticleManager->DeleteSequence(m_pCursorParticle2);
        g_ParticleManager->DeleteSequence(m_pCursorParticle3);
        g_ParticleManager->DeleteSequence(m_pCursorParticle4);
    }

    m_pCursorParticle1 = NULL;
    m_pCursorParticle2 = NULL;
    m_pCursorParticle3 = NULL;
    m_pCursorParticle4 = NULL;
}

void KGCTeleporter::FrameMove( float fElapsedTime /*= GC_ELAPSED_TIME */ )
{
    if ( false == KGCGameObject::IsLive() )
    {
        m_pArrow->SetShow(false);
#if defined(NATION_CHINA)
        m_pCursorParticle1->SetShow(false);
        m_pCursorParticle2->SetShow(false);
        m_pCursorParticle3->SetShow(false);
        m_pCursorParticle4->SetShow(false);
#endif
        if( m_pDamage )
            m_pTeleporter->SetShow( false );

        return;
    }
    
	m_pArrow->SetShow(false);
#if defined(NATION_CHINA)
    m_pCursorParticle1->SetShow(true);
    m_pCursorParticle2->SetShow(true);
    m_pCursorParticle3->SetShow(true);
    m_pCursorParticle4->SetShow(true);
#endif

    if( m_pDamage )
        m_pTeleporter->SetShow( false );

    if( !GetNextTeleporter( this ) )
        return;

    for( int i = 0; i < MARI_TELEPORT_PARTICEL_NUM; i++ )
    {
        if( m_pMariTelepoter[i] )
            m_pMariTelepoter[i]->SetShow(m_bIsMariTeleport);
    }    


	for(int i = 0; i < MAX_PLAYER_NUM; i++ )
	{
		if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive  && 0!=g_MyD3D->MyPlayer[i]->m_cLife )
		{			
			GCCollisionRect<float> rect;
			if(g_MyD3D->MyPlayer[i]->GetCollisionRect().CheckCollision(GetCollision(),&rect))
			{
				m_pArrow->SetShow(!m_bIsMariTeleport);

                if( m_bIsMariTeleport )
                {
                    g_MyD3D->MyPlayer[i]->GetOwnMotion(MID_COMMON_INTO_THE_TELEPORT);
                    if(g_MyD3D->MyPlayer[i]->uiMotion == MID_COMMON_INTO_TELEPORT &&
                       (g_MyD3D->MyPlayer[i]->cFrame == 1 || g_MyD3D->MyPlayer[i]->cFrame==70))
                    {
                        // Sound & Effect 
                        if( GC_GM_DOTA == SiKGCRoomManager()->GetGameMode() )
                            g_KDSound.Play( "985022" );
                        else
                            g_KDSound.Play( "609" );
                        ShowEffect();
                    }
                }
                else
                {
                    if(g_MyD3D->MyPlayer[i]->uiMotion == g_MyD3D->MyPlayer[i]->GetOwnMotion(MID_COMMON_INTO_TELEPORT) && 
                        (g_MyD3D->MyPlayer[i]->cFrame == 1 || g_MyD3D->MyPlayer[i]->cFrame==70))
                    {
                        // Sound & Effect 
                        if( GC_GM_DOTA == SiKGCRoomManager()->GetGameMode() )
                            g_KDSound.Play( "985022" );
                        else
                        g_KDSound.Play( "609" );				
                        ShowEffect();
                    }
                }				
			}		
		}
	}


}

void KGCTeleporter::Render( void )
{
    if ( false == KGCGameObject::IsLive() )
        return;

    m_pArrow->Render();
    if( m_pDamage )
        return;

	m_pTeleporter->SetShow(true);
	m_pTeleporter->Render();
	m_pTeleporter->SetShow(false);	
}

GCCollisionRect<float> KGCTeleporter::GetCollision( void )
{
	GCCollisionRect<float> kCollisionRect;

	float fHeight = m_pTeleporter->GetRealHeight()*0.01f;
	float fWidth = m_pTeleporter->GetRealWidth()*0.01f;

	float fX = m_pTeleporter->GetMatrix()->GetPos().x;
	float fY = m_pTeleporter->GetMatrix()->GetPos().y;

	kCollisionRect.m_Top = fY + fHeight + 0.3f;
	kCollisionRect.m_Bottom = fY+ 0.3f;

	kCollisionRect.m_Left = fX - ( fWidth);
	kCollisionRect.m_Right = fX + ( fWidth);


	return kCollisionRect;
}

KGCTeleporter* KGCTeleporter::CheckTeleporter( PLAYER * player )
{

	KGCTeleporter * cur_teleporter=NULL;	
	for(std::set<KGCTeleporter*>::iterator i=set_teleporter.begin();i!=set_teleporter.end();i++)
	{
		GCCollisionRect<float> rect;	
		if( player->GetCollisionRect().CheckCollision((*i)->GetCollision(),&rect))
		{
            if ( (*i)->IsLive() )
            {
                cur_teleporter=(*i);
                break;
            }
		}
	}	

    if( GetNextTeleporter( cur_teleporter ) )
    {
        return cur_teleporter;
    }
    return NULL;
	
}

KGCTeleporter* KGCTeleporter::GetNextTeleporter( KGCTeleporter* cur_teleporter )
{	
	KGCTeleporter * next_teleporter=NULL;
	if(cur_teleporter && cur_teleporter->pair>-1)
	{
		for(std::set<KGCTeleporter*>::iterator i=set_teleporter.begin();i!=set_teleporter.end();i++)
		{				
			if(cur_teleporter!=(*i) && cur_teleporter->pair==(*i)->pair)
			{
				next_teleporter=(*i);
				return next_teleporter;					
			}
		}
	}
	return NULL;	
}

KGCTeleporter* KGCTeleporter::GetDotaStageInTeleporter( int iDotaStage )
{	
    KGCTeleporter * next_teleporter=NULL;

    for(std::set<KGCTeleporter*>::iterator i=set_teleporter.begin();i!=set_teleporter.end();i++)
    {				
        if( (*i)->GetDotaStage() == iDotaStage )
        {
            next_teleporter=(*i);
            return next_teleporter;
        }
    }
    return NULL;	
}

void KGCTeleporter::SetPair( int i )
{
	this->pair=i;
}

void KGCTeleporter::Enable()
{

}

D3DXVECTOR2 KGCTeleporter::GetPos()
{
	D3DXVECTOR2 vec;
	vec.x =  m_pTeleporter->GetMatrix()->GetPos().x;
	vec.y =  m_pTeleporter->GetMatrix()->GetPos().y;
	return vec;
}



void KGCTeleporter::ShowEffect()
{
	g_ParticleManager->CreateSequence( "Portal_Zone_Effect01", 
		m_pTeleporter->GetMatrix()->GetXPos(),
		m_pTeleporter->GetMatrix()->GetYPos(), 1.9f );
	g_ParticleManager->CreateSequence( "Portal_Zone_Effect02", 
		m_pTeleporter->GetMatrix()->GetXPos(),
		m_pTeleporter->GetMatrix()->GetYPos(), 1.9f );
}
