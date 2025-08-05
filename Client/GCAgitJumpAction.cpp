#include "stdafx.h"
#include "GCAgitJumpAction.h"



KGCAgitJumpAction::KGCAgitJumpAction(void)
{
    m_eObjType=EM_AGIT_OBJECT_JUMP_ACTION;
    m_nSound = -1;
    m_nBasicEffectNum = 0;
}

KGCAgitJumpAction::~KGCAgitJumpAction(void)
{
    Destroy();
}

void KGCAgitJumpAction::Create( void )
{
    if ( m_nSound != -1 )
    {
        g_KDSound.LoadFromIndex(m_nSound);
    }
}

void KGCAgitJumpAction::Destroy( void )
{	
    if ( m_nSound != -1 )
    {
        g_KDSound.Free(m_nSound);
    }
    
    m_vecEffect.clear();

    for ( size_t i = 0; i < m_vecBasicParticle.size(); ++i ) 
    {
        CParticleEventSeqPTR pParticle = m_vecBasicParticle[i];
        g_ParticleManager->DeleteSequence( pParticle );        
    }

    m_vecBasicParticle.clear();

}

void KGCAgitJumpAction::FrameMove( float fElapsedTime /*= GC_ELAPSED_TIME */ )
{
    if ( !IsLive() )
        return;

    for(int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive  && 0!=g_MyD3D->MyPlayer[i]->m_cLife )
        {			
            GCCollisionRect<float> rect;
            if(g_MyD3D->MyPlayer[i]->IsContact && 
                GetPlayerCollisionRect(i).CheckCollision(GetCollisionRect(),&rect))
            {
//              g_MyD3D->MyPlayer[i]->x_Speed=m_fJumpX;

                if(g_MyD3D->MyPlayer[i]->y_Speed<m_fJumpY)
                    g_MyD3D->MyPlayer[i]->y_Speed=m_fJumpY;

                // Sound & Effect 
                if ( m_nSound != -1 )
                {
                    g_KDSound.Play( m_nSound, 0  );
                }

                float fPosX = GetPosX() - 1.0f + ( GetWidth() / 2.0f );
                float fPosY = GetPosY() - 0.45f;

                for ( size_t i = m_nBasicEffectNum; i < m_vecEffect.size(); ++i )
                {
                    g_ParticleManager->CreateSequence( m_vecEffect[i], 
                        fPosX, fPosY, 0.5f );
                }
            }		
        }
    }
}


void KGCAgitJumpAction::SetBasicEffect()
{
    m_vecBasicParticle.clear();

    float fPosX = GetPosX() - 1.0f + ( GetWidth() / 2.0f );
    float fPosY = GetPosY() - 0.45f;

    for ( int i = 0; i < m_nBasicEffectNum; ++i ) 
    {
        if ( i >= static_cast<int>(m_vecEffect.size()) ) 
            break;

        CParticleEventSeqPTR pParticle = g_ParticleManager->CreateSequence( m_vecEffect[i], fPosX, fPosY, 0.5f ); 
        m_vecBasicParticle.push_back( pParticle );
    }   
}

void KGCAgitJumpAction::ResetBasicEffect()
{
    for ( size_t i = 0; i < m_vecBasicParticle.size(); ++i ) 
    {
        CParticleEventSeqPTR pParticle = m_vecBasicParticle[i];
        g_ParticleManager->DeleteSequence( pParticle );        
    }

    m_vecBasicParticle.clear();
}

void KGCAgitJumpAction::EnableLive( bool bLive )
{
    if ( bLive )
    {
        ResetBasicEffect();
        SetBasicEffect();
    }
    else
    {
        ResetBasicEffect();
    }
}

GCCollisionRect<float> KGCAgitJumpAction::GetPlayerCollisionRect( int i )
{
    GCCollisionRect<float> kCollisionRect;
    if ( g_MyD3D->IsPlayerIndex( i )  )
    {
        kCollisionRect = g_MyD3D->MyPlayer[i]->GetCollisionRect();

        //  플레이어 바운드 박스를 하단만 적용하게 수정한다.
        kCollisionRect.m_Bottom += 0.045f;
        kCollisionRect.m_Top = kCollisionRect.m_Bottom + 0.005f; 
    }

    return kCollisionRect;

}

GCCollisionRect<float> KGCAgitJumpAction::GetCollisionRect( void )
{
    GCCollisionRect<float> kCollisionRect;

    //  Size
    float fHeight = GetHeight();
    float fWidth = GetWidth();

    //  Left 좌표 
    float fX = GetPosX();
    float fY = GetPosY();   

    //  Width를 변경하는 이유는 캐릭터 바운드가 너무 크다.

    fX += fWidth / 2.0f;

    if ( fWidth > 0.24f ) 
        fWidth -= 0.24f;
    else
        fWidth = 0.004f;

    fX -= fWidth / 2.0f;

    kCollisionRect.SetRect( fX - 1.0f, fY - 0.25f, fX + fWidth - 1.0f, fY + fHeight * 0.95f - 0.25f );

    return kCollisionRect;
}

void KGCAgitJumpAction::SetEffectName( const std::vector<std::string>& vecEffect )
{
    m_vecEffect = vecEffect;
}

