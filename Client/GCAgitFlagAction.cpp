#include "stdafx.h"
#include "GCAgitFlagAction.h"

#include "LatencyTransfer.h"

const float F_FLAG_ONE_TIME = 3.0f;

const int nFRAME_TICK_COUNT = 5;


KGCAgitFlagAction::KGCAgitFlagAction(void)
{
    m_eObjType = EM_AGIT_OBJECT_FLAG_ACTION;
    m_nSound = -1;
    m_dwUserID = -1;
    m_dwItemUID = UINT_MAX;
    m_bActionFlag = false;
    m_fFlagTime = 0.0f;
    m_pFlagParticle = NULL;
    m_dwTickCount = 0;
}

KGCAgitFlagAction::~KGCAgitFlagAction(void)
{
    Destroy();
}

void KGCAgitFlagAction::Create( void )
{
    if ( m_nSound != -1 )
    {
        g_KDSound.LoadFromIndex(m_nSound);
    }

    m_dwUserID = -1;    
    
    m_bActionFlag = true;
    m_fFlagTime = 0.0f;
}

void KGCAgitFlagAction::Destroy( void )
{	
    if ( m_nSound != -1 )
    {
        g_KDSound.Free(m_nSound);
    }

    ReSetFlagEffect();
    m_vecEffect.clear();
}

void KGCAgitFlagAction::FrameMove( float fElapsedTime /*= GC_ELAPSED_TIME */ )
{
    if ( !IsLive() )
        return;

    UpdateCheckFlag( fElapsedTime );

    UpdateFlag();

}

void KGCAgitFlagAction::UpdateFlag()
{
    if ( !g_kGlobalValue.m_kUserInfo.bHost ) 
        return;

    m_dwTickCount++;

    if ( m_dwTickCount > nFRAME_TICK_COUNT )
    {
        if ( m_dwUserID != UINT_MAX ) 
        {
            LTP_AGIT_FLAG_OBJECT_STATE kPacket;
            kPacket.m_dwItemUID = m_dwItemUID;
            kPacket.m_dwUserUID = m_dwUserID;
            kPacket.m_bUpdate = true;

            g_LatencyTransfer.PushPacket( &kPacket );
        }
        
        m_dwTickCount = 0;
    }
}

//  ��� ������ ���� üũ
void KGCAgitFlagAction::UpdateCheckFlag( float fElapsedTime )
{
    //  ���üũ�� ���常 �Ѵ�.
    if ( !g_kGlobalValue.m_kUserInfo.bHost ) 
        return;

    //  3�ʿ� �ѹ����� ������ �ִ�.
    if ( m_bActionFlag ) 
    {
        m_fFlagTime += fElapsedTime;

        if ( m_fFlagTime > F_FLAG_ONE_TIME ) 
        {
            m_bActionFlag = false;
            m_fFlagTime = 0.0f;
        }

        return;
    }

    bool bHaveFlag = (m_dwUserID == -1) ? true : false;

    GCCollisionRect<float> rcFlagRect;

    if ( bHaveFlag ) 
    {
        rcFlagRect = GetCollisionRect();
    }
    else
    {
        PLAYER* pPlayer = g_MyD3D->GetPlayerByUID( m_dwUserID );
        if ( !pPlayer )
        {
            //  ��� �����ڰ� ��������.
            //  ����� ������� ��������.
            SetFlagState( UINT_MAX );

            return;            
        }

        //  ����� ���������� ���� �ε����� �ٽ� �ž� ���´�.
        rcFlagRect = pPlayer->GetCollisionRect();

        GCCollisionRect<float> rect;
        if ( rcFlagRect.CheckCollision( GetCollisionRect(), &rect ) )
        {
            SetFlagState( UINT_MAX );
            return;
        }
    }


    // ��� üũ
    for(int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if ( !g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive ) 
            continue;

        if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID == m_dwUserID ) 
            continue;


        GCCollisionRect<float> rect;
        if( g_MyD3D->MyPlayer[i]->GetCollisionRect().CheckCollision(rcFlagRect,&rect) )
        {
            // �̹� ����� ������ ������ �ǳʶڴ�. ���϶����� ���⼭ üũ�Ѵ�.
            if ( SiKGCAgitGameManager()->IsHaveFlag( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID ) )
                continue;

            SetFlagState( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID );

            return;
        }
    }
}

void KGCAgitFlagAction::SetFlagState( DWORD dwUserID )
{
    //  ��߿� ���� �������� �����Ѵ�.
    LTP_AGIT_FLAG_OBJECT_STATE kPacket;
    kPacket.m_dwItemUID = m_dwItemUID;
    kPacket.m_dwUserUID = dwUserID;

    g_LatencyTransfer.PushPacket( &kPacket );

    m_bActionFlag = true;
    m_fFlagTime = 0.0;

    m_dwUserID = dwUserID;

    SetFlagEffect();
}

GCCollisionRect<float> KGCAgitFlagAction::GetCollisionRect( void )
{
    GCCollisionRect<float> kCollisionRect;

    //  Size
    float fHeight = GetHeight();
    float fWidth = GetWidth();

    //  Left ��ǥ 
    float fX = GetPosX();
    float fY = GetPosY();   

    //  Width�� �����ϴ� ������ ĳ���� �ٿ�尡 �ʹ� ũ��.
    if ( fWidth > 0.15f ) 
        fWidth -= 0.15f;
    else
        fWidth = 0.02f;

    fX += fWidth / 2.0f;    


    kCollisionRect.SetRect( fX - 1.0f, fY - 0.25f, fX + fWidth - 1.0f, fY + fHeight * 0.95f - 0.25f );

    return kCollisionRect;
}

void KGCAgitFlagAction::SetEffectName( const std::vector<std::string>& vecEffect )
{
    m_vecEffect = vecEffect;
}

void KGCAgitFlagAction::DoAction( const std::vector<DWORD>& vecParam )
{
    if ( vecParam.size() < 2 ) 
        return;

    bool bUpdate = vecParam[0] == 1 ? true : false;

    //  �׳� ��߸� �����϶��� �������� �ʴ´�.
    if ( !bUpdate ) 
    {
        m_bActionFlag = true;
        m_fFlagTime = 0.0;
    }

    DWORD dwBack = m_dwUserID;
    m_dwUserID = vecParam[1];

    if ( dwBack != m_dwUserID )
    {
        // ��� ��ġ
        SetFlagEffect();
    }
}

void KGCAgitFlagAction::EnableLive( bool bLive )
{
    KGCAgitObjectAction::EnableLive( bLive );

    if ( bLive )
    {
        //  ��� ��ġ ������
        SetFlagEffect();        
    }
    else
    {
        m_dwUserID = -1;
        //  ��� ����
        ReSetFlagEffect();
    }
}

void KGCAgitFlagAction::SetFlagEffect()
{
    if ( m_vecEffect.empty() ) 
        return;

    if ( m_pFlagParticle ) 
    {
        g_ParticleManager->DeleteSequence( m_pFlagParticle );
    }

    float fPosX = 0.0f;
    float fPosY = 0.0f;

    //  ��뿡 ��ġ
    if ( m_dwUserID == -1 )
    {
        fPosX = GetPosX() - 1.0f + 0.02f;
        fPosY = GetPosY() + GetHeight() - 0.4f;

        m_pFlagParticle = g_ParticleManager->CreateSequence( m_vecEffect[0], fPosX, fPosY, 0.5f );
    }
    // �������� ��ġ
    else
    {
        PLAYER* pPlayer = g_MyD3D->GetPlayerByUID( m_dwUserID );
        if ( !pPlayer ) 
            return;

        m_pFlagParticle = pPlayer->AddTraceParticle( m_vecEffect[0], -0.15f, 0.1f, 5.0f, static_cast<int>( EPT_BODY_CENTER ) );
    }
}

void KGCAgitFlagAction::ReSetFlagEffect()
{
    if ( !m_pFlagParticle )
        return;
   
    g_ParticleManager->DeleteSequence( m_pFlagParticle );
    m_pFlagParticle = NULL;
}



