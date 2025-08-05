#include "StdAfx.h"
#include "KGCJungSu.h"

#include "MyD3D.h"
#include "LatencyTransfer.h"
#include "Player.h"
#include "GCCameraOrtho.h"
#include "Headup Display.h"
#include "Monster.h"
#include "Buff/KGCBuffManager.h"
#include "DamageManager.h"

KGCJungSu::KGCJungSu(void)
{
    m_pTexBase = NULL;
    m_JungSuItemID = GEM_ITEM_ID;
}

KGCJungSu::~KGCJungSu(void)
{
    SAFE_RELEASE( m_pTexBase );
}

void KGCJungSu::Create( void )
{
    m_RemainCoolTime = 0;
	m_pTexBase = g_pGCDeviceManager2->CreateTexture( "base_jungsu.dds" );
}

void KGCJungSu::Destroy( void )
{

    for( int i = 0 ; i < (int)m_vecParticleActive.size(); i++ )
    {
        g_ParticleManager->DeleteSequence( m_vecParticleActive[i] );
    }

    for( int i = 0 ; i < (int)m_vecParticleCoolTime.size(); i++ )
    {
        g_ParticleManager->DeleteSequence( m_vecParticleCoolTime[i] );
    }

    for( int i = 0 ; i < (int)m_vecParticleHover.size(); i++ )
    {
        g_ParticleManager->DeleteSequence( m_vecParticleHover[i] );
    }

    m_vecParticleActive.clear();
    m_vecParticleCoolTime.clear();
    m_vecParticleHover.clear();
	m_vecParticleStart.clear();
}

void KGCJungSu::Enable( void )
{

}

void KGCJungSu::FrameMove( float fElapsedTime /*= GC_ELAPSED_TIME */ )
{
    if( 0 < m_RemainCoolTime )
    {
        m_RemainCoolTime--;
        return;
    }
    else
    {
        EnableLive( true );
    }

    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();

    GCCollisionRect<float> rtColl;
    bool IsHover = pPlayer->GetCollisionRect().CheckCollision( GetCollisionRect() , &rtColl );    

    for( int i = 0 ; i < (int)m_vecParticleHover.size(); i++ )
    {
        m_vecParticleHover[i]->SetShow( IsHover );
    }

    if( !IsHover )
        return;

    if ( g_MyD3D->MyCtrl->k_Fresh_Fire )
    {
        if( g_MyD3D->m_kItemMgr.FindInventoryForItemID( m_JungSuItemID ) )
        {
            //ㅂ라동!!!!
            LTP_JUNGSU_REQ JungSuReqPacket;
            JungSuReqPacket.iCharIndex = pPlayer->m_iPlayerIndex;
            JungSuReqPacket.iJungSuType = m_JungSuType;
            JungSuReqPacket.iSlotIndex = m_iIndex;
            g_LatencyTransfer.PushPacket( &JungSuReqPacket );
        }
    }
}

void KGCJungSu::Render( void )
{
    g_pGCDeviceManager2->PushState();
    g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
    g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

     g_pGCDeviceManager2->DrawInWorld(m_pTexBase,&g_kCamera.m_matCamera,
         D3DXVECTOR3(m_fPosX - 1.2f, m_fPosY - 0.57f, 3.0f), 
         D3DXVECTOR3(m_fPosX - 1.2f, m_fPosY+m_fHeight - 0.57f, 3.0f), 
         D3DXVECTOR3(m_fPosX+m_fWidth - 1.2f, m_fPosY - 0.57f, 3.0f), 
         D3DXVECTOR3(m_fPosX+m_fWidth - 1.2f, m_fPosY+m_fHeight - 0.57f, 3.0f));





    g_pGCDeviceManager2->PopState();

}

bool KGCJungSu::IsCrashCheck( void )
{
    return false;
}

GCCollisionRect<float> KGCJungSu::GetCollisionRect( void )
{
    GCCollisionRect<float> rtRect;
    rtRect.m_Left       = m_fPosX+ (m_fWidth / 5)*2 - 1.2f;
    rtRect.m_Right      = m_fPosX+ (m_fWidth / 5)*3 - 1.2f;
    rtRect.m_Top        = m_fPosY+ m_fHeight/2 - 0.25f;
    rtRect.m_Bottom     = m_fPosY - 0.25f;

    return rtRect;
}

void KGCJungSu::Activate( int CharIndex )
{
    if( CharIndex == g_MyD3D->Get_MyPlayer() )
        g_MyD3D->m_kItemMgr.DecInventoryDurationItemForItemID( m_JungSuItemID );

	for ( int i = 0; i < (int)m_vecParticleStart.size(); ++i )
	{
		g_ParticleManager->CreateSequence( m_vecParticleStart[i], m_fPosX - 1.073f, m_fPosY - 0.4f, 0.5f );
	}

    ActiveAction( CharIndex );
    ResetCoolTime();

}

void KGCJungSu::ResetCoolTime()
{
    m_RemainCoolTime = m_CoolTime;
    EnableLive( false );
}

void KGCJungSu::EnableLive( bool bLive )
{
    KGCGameObject::EnableLive( bLive );

    for( int i = 0 ; i < (int)m_vecParticleActive.size(); i++ )
    {
        m_vecParticleActive[i]->SetShow( bLive );
    }

    for( int i = 0 ; i < (int)m_vecParticleCoolTime.size(); i++ )
    {
        m_vecParticleCoolTime[i]->SetShow( bLive );
    }

    for( int i = 0 ; i < (int)m_vecParticleHover.size(); i++ )
    {
        m_vecParticleHover[i]->SetShow( bLive );
    }


}
KGCJungSuCirculation::KGCJungSuCirculation( void )
{
    m_CoolTime = 275;
    m_JungSuItemID = 18291;
}

KGCJungSuCirculation::~KGCJungSuCirculation( void )
{

}

void KGCJungSuCirculation::ActiveAction( int CharIndex )
{
    if( CharIndex == g_MyD3D->Get_MyPlayer() )
    {
        g_MyD3D->MyHead->Level = g_kGlobalValue.GetMaxMP();
    }
}

void KGCJungSuCirculation::Create( void )
{
    KGCJungSu::Create();

	CParticleEventSeqPTR pParticle;
    float fPosY;

#if defined(NATION_CHINA)
    fPosY = 0.08f;
#else
    fPosY = 0.55f;
#endif

    pParticle = g_ParticleManager->CreateSequence( "Circulation_Integer_Effect_01", m_fPosX - 1.073f, m_fPosY - 0.4f, 0.5f );
    m_vecParticleActive.push_back(pParticle);
    pParticle = g_ParticleManager->CreateSequence( "Circulation_Integer_Effect_02", m_fPosX - 1.073f, m_fPosY - 0.4f, 0.5f );
    m_vecParticleActive.push_back(pParticle);

    pParticle = g_ParticleManager->CreateSequence( "cursor03", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);
#if defined(NATION_CHINA)
    pParticle = g_ParticleManager->CreateSequence( "cursor05", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);
#else
    pParticle = g_ParticleManager->CreateSequence( "cursor04", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);
#endif

    pParticle = g_ParticleManager->CreateSequence( "Cursor_Z_01", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);
    pParticle = g_ParticleManager->CreateSequence( "Cursor_Z_02", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);

	char str[256] = "";
	for ( int i = 0; i < 3; ++i )
	{
		sprintf( str, "Circulation_Integer_Run_0%d", i+1 );
		m_vecParticleStart.push_back( str );
	}
}
KGCJungSuInferno::KGCJungSuInferno( void )
{
    m_CoolTime = 550;
    m_JungSuItemID = 18292;
}

KGCJungSuInferno::~KGCJungSuInferno( void )
{

}

void KGCJungSuInferno::ActiveAction( int CharIndex )
{
    if( CharIndex == g_MyD3D->Get_MyPlayer() )
    {

        g_MyD3D->MyPlayer[CharIndex]->SetHP( 0.f );
        g_MyD3D->MyPlayer[CharIndex]->Change_Motion( MID_COMMON_DOWN_AND_STANDUP );

    }

    if( g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost )
    {

		for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
			MONSTER *pMonster = mit->second;
			if( pMonster->IsLive() == false ) continue;
			
			float fDamage = 15000.f;

			if( pMonster->GetHP() - fDamage < 0.0f )
				pMonster->SetHP( 0.0f );
			else
				pMonster->SetHP( pMonster->GetHP() - fDamage );

		}
    }
}

void KGCJungSuInferno::Create( void )
{
    KGCJungSu::Create();

	CParticleEventSeqPTR pParticle;
    float fPosY;

#if defined(NATION_CHINA)
    fPosY = 0.08f;
#else
    fPosY = 0.55f;
#endif

    pParticle = g_ParticleManager->CreateSequence( "Fire_Integer_Effect_01", m_fPosX - 1.073f, m_fPosY - 0.4f, 0.5f );
    m_vecParticleActive.push_back(pParticle);
    pParticle = g_ParticleManager->CreateSequence( "Fire_Integer_Effect_02", m_fPosX - 1.073f, m_fPosY - 0.4f, 0.5f );
    m_vecParticleActive.push_back(pParticle);

    pParticle = g_ParticleManager->CreateSequence( "cursor03", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);
#if defined(NATION_CHINA)
    pParticle = g_ParticleManager->CreateSequence( "cursor05", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);
#else
    pParticle = g_ParticleManager->CreateSequence( "cursor04", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);
#endif

    pParticle = g_ParticleManager->CreateSequence( "Cursor_Z_01", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);
    pParticle = g_ParticleManager->CreateSequence( "Cursor_Z_02", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);

	char str[256] = "";
	for ( int i = 0; i < 3; ++i )
	{
		sprintf( str, "Fire_Integer_Run_0%d", i+1 );
		m_vecParticleStart.push_back( str );
	}
}
KGCJungSuControl::KGCJungSuControl( void )
{
    m_CoolTime = 110;
    m_JungSuItemID = 18293;
}

KGCJungSuControl::~KGCJungSuControl( void )
{

}

void KGCJungSuControl::ActiveAction( int CharIndex )
{
    for( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
		if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.strLogin != L"" &&
			 ( g_MyD3D->MyPlayer[i]->m_ePlayerState == PS_ALIVE || g_MyD3D->MyPlayer[i]->m_ePlayerState == PS_FATAL ) )
		{
			g_pMagicEffect->SetMagicEffect( i, EGC_EFFECT_JUNGSU_CONTROL, 60.0f, 1, -1, true, true );
		}
    }
}

void KGCJungSuControl::Create( void )
{
    KGCJungSu::Create();

	CParticleEventSeqPTR pParticle;
    float fPosY;

#if defined(NATION_CHINA)
    fPosY = 0.08f;
#else
    fPosY = 0.55f;
#endif

    pParticle = g_ParticleManager->CreateSequence( "Tuning_Integer_Effect_01", m_fPosX - 1.073f, m_fPosY - 0.4f, 0.5f );
    m_vecParticleActive.push_back(pParticle);
    pParticle = g_ParticleManager->CreateSequence( "Tuning_Integer_Effect_02", m_fPosX - 1.073f, m_fPosY - 0.4f, 0.5f );
    m_vecParticleActive.push_back(pParticle);

    pParticle = g_ParticleManager->CreateSequence( "cursor03", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);
#if defined(NATION_CHINA)
    pParticle = g_ParticleManager->CreateSequence( "cursor05", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);
#else
    pParticle = g_ParticleManager->CreateSequence( "cursor04", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);
#endif

    pParticle = g_ParticleManager->CreateSequence( "Cursor_Z_01", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);
    pParticle = g_ParticleManager->CreateSequence( "Cursor_Z_02", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);

	char str[256] = "";
	for ( int i = 0; i < 3; ++i )
	{
		sprintf( str, "Tuning_Integer_Run_0%d", i+1 );
		m_vecParticleStart.push_back( str );
	}
}
KGCJungSuDestruction::KGCJungSuDestruction( void )
{
    m_CoolTime = 825;
    m_JungSuItemID = 18466;
}

KGCJungSuDestruction::~KGCJungSuDestruction( void )
{

}

void KGCJungSuDestruction::ActiveAction( int CharIndex )
{
	for( int i = 0; i < MAX_PLAYER_NUM; i++ )
	{
		if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.strLogin != L"" &&
			( g_MyD3D->MyPlayer[i]->m_ePlayerState == PS_ALIVE || g_MyD3D->MyPlayer[i]->m_ePlayerState == PS_FATAL ) )
		{
			g_MyD3D->MyPlayer[i]->Item_Remain_Time[GC_GAME_ITEM_SUPER] = 550;
		}
	}
}

void KGCJungSuDestruction::Create( void )
{
    KGCJungSu::Create();

	CParticleEventSeqPTR pParticle;
    float fPosY;

#if defined(NATION_CHINA)
    fPosY = 0.08f;
#else
    fPosY = 0.55f;
#endif

    pParticle = g_ParticleManager->CreateSequence( "Ruin_Integer_Effect_01", m_fPosX - 1.073f, m_fPosY - 0.4f, 0.5f );
    m_vecParticleActive.push_back(pParticle);
    pParticle = g_ParticleManager->CreateSequence( "Ruin_Integer_Effect_02", m_fPosX - 1.073f, m_fPosY - 0.4f, 0.5f );
    m_vecParticleActive.push_back(pParticle);

    pParticle = g_ParticleManager->CreateSequence( "cursor03", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);
#if defined(NATION_CHINA)
    pParticle = g_ParticleManager->CreateSequence( "cursor05", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);
#else
    pParticle = g_ParticleManager->CreateSequence( "cursor04", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);
#endif

    pParticle = g_ParticleManager->CreateSequence( "Cursor_Z_01", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);
    pParticle = g_ParticleManager->CreateSequence( "Cursor_Z_02", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);

	char str[256] = "";
	for ( int i = 0; i < 3; ++i )
	{
		sprintf( str, "Ruin_Integer_Run_0%d", i+1 );
		m_vecParticleStart.push_back( str );
	}
}

KGCJungSuResurrection::KGCJungSuResurrection( void )
{
    m_CoolTime = 275;
    m_JungSuItemID = 18856;
}

KGCJungSuResurrection::~KGCJungSuResurrection( void )
{

}

void KGCJungSuResurrection::ActiveAction( int CharIndex )
{
    for( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
		if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.strLogin != L"" &&
			( g_MyD3D->MyPlayer[i]->m_ePlayerState == PS_ALIVE || g_MyD3D->MyPlayer[i]->m_ePlayerState == PS_FATAL ) )
		{
			g_MyD3D->MyPlayer[i]->SetHPFull();
		}
    }
}

void KGCJungSuResurrection::Create( void )
{
    KGCJungSu::Create();

	CParticleEventSeqPTR pParticle;
    float fPosY;

#if defined(NATION_CHINA)
    fPosY = 0.08f;
#else
    fPosY = 0.55f;
#endif

    pParticle = g_ParticleManager->CreateSequence( "Life_Integer_Effect_01", m_fPosX - 1.073f, m_fPosY - 0.4f, 0.5f );
    m_vecParticleActive.push_back(pParticle);
    pParticle = g_ParticleManager->CreateSequence( "Life_Integer_Effect_02", m_fPosX - 1.073f, m_fPosY - 0.4f, 0.5f );
    m_vecParticleActive.push_back(pParticle);

    pParticle = g_ParticleManager->CreateSequence( "cursor03", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);
#if defined(NATION_CHINA)
    pParticle = g_ParticleManager->CreateSequence( "cursor05", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);
#else
    pParticle = g_ParticleManager->CreateSequence( "cursor04", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);
#endif
    
    pParticle = g_ParticleManager->CreateSequence( "Cursor_Z_01", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);
    pParticle = g_ParticleManager->CreateSequence( "Cursor_Z_02", m_fPosX - 1.073f, m_fPosY - fPosY, 0.5f );
    m_vecParticleHover.push_back(pParticle);

    char str[256] = "";
	for ( int i = 0; i < 3; ++i )
	{
		sprintf( str, "Life_Integer_Run_0%d", i+1 );
		m_vecParticleStart.push_back( str );
	}
}