#include "stdafx.h"
//

#include "GCIronDoor.h"
#include "Monster.h"





KGCIronDoor::KGCIronDoor( void )
{
    m_pIronDoorNormal   = NULL;
    m_pIronDoorDamage   = NULL;
    m_pIronDoorCrash    = NULL;
    m_pDoorLockParticle = NULL;            
    	
}

KGCIronDoor::~KGCIronDoor( void )
{
    // empty
    Destroy();
}

void KGCIronDoor::Create( void )
{
	KGCDoor::Create();

    m_pIronDoorNormal = g_MyD3D->m_pMapAnim->CreateInst( g_MyD3D->m_pMapAnim->GetSequence( "IronDoor0" ) );
    m_pIronDoorDamage = g_MyD3D->m_pMapAnim->CreateInst( g_MyD3D->m_pMapAnim->GetSequence( "IronDoor1" ) );
    m_pIronDoorCrash  = g_MyD3D->m_pMapAnim->CreateInst( g_MyD3D->m_pMapAnim->GetSequence( "IronDoor2" ) );

    D3DXVECTOR2 pos = D3DXVECTOR2(GetPosX(), GetPosY());

    m_pIronDoorNormal->GetMatrix()->Move( pos.x - 1.15f, pos.y - 0.25f, 42.0f );
    m_pIronDoorNormal->GetMatrix()->Scale( 0.002f, 0.002f, 1.0f );
    m_pIronDoorNormal->Start( CKTDGAnim::PT_LOOP );
    m_pIronDoorNormal->SetShow( true );

    m_pIronDoorDamage->GetMatrix()->Move( pos.x - 1.15f, pos.y - 0.25f, 42.0f );
    m_pIronDoorDamage->GetMatrix()->Scale( 0.002f, 0.002f, 1.0f );
    m_pIronDoorDamage->SetShow( false );
    m_pIronDoorDamage->Stop();

    m_pIronDoorCrash->GetMatrix()->Move( pos.x - 1.15f, pos.y - 0.25f, 42.0f );
    m_pIronDoorCrash->GetMatrix()->Scale( 0.002f, 0.002f, 1.0f );
    m_pIronDoorCrash->SetShow( false );
    m_pIronDoorCrash->Stop();

    m_pDoorLockParticle = g_ParticleManager->CreateSequence( "DoorLock", pos.x - 0.9f - m_pIronDoorNormal->GetRealWidth()/2.0f
                                                            , pos.y - 0.04f - m_pIronDoorNormal->GetRealHeight()/2.0f, 1.9f );

	m_pDirectionArrow->SetPosition(pos.x - 0.9f - m_pIronDoorNormal->GetRealWidth()/2.0f,
		pos.y + 0.28f - m_pIronDoorNormal->GetRealHeight()/2.0f, 3.0f);

    if ( false == IsInitEnable() )
    {
        m_pDoorLockParticle->SetShow( true );        
    }
    else
    {
        m_pDoorLockParticle->SetShow( false );        
    }

    SetWidth( m_pIronDoorNormal->GetRealWidth() );
    SetHeight( m_pIronDoorNormal->GetRealHeight() );
}

void KGCIronDoor::Destroy( void )
{
	KGCDoor::Destroy();

    if( g_MyD3D->m_pMapAnim )
    {
        g_MyD3D->m_pMapAnim->DeleteInst( m_pIronDoorNormal );
        g_MyD3D->m_pMapAnim->DeleteInst( m_pIronDoorDamage );
        g_MyD3D->m_pMapAnim->DeleteInst( m_pIronDoorCrash );
    }

    if( g_ParticleManager )
    {
        g_ParticleManager->DeleteSequence(m_pDoorLockParticle);        
    }

    m_pDoorLockParticle     = NULL;
    m_pIronDoorNormal       = NULL;
    m_pIronDoorDamage       = NULL;
    m_pIronDoorCrash        = NULL;
}

void KGCIronDoor::Enable( void )
{   	
    m_pDoorLockParticle->SetShow( false );    
}

void KGCIronDoor::FrameMove( float fElapsedTime /* = GC_ELAPSED_TIME */ )
{
	KGCDoor::FrameMove(fElapsedTime);
    UNREFERENCED_PARAMETER( fElapsedTime );

    if( m_pDoorLockParticle && g_ParticleManager->IsLiveInstance(m_pDoorLockParticle) )
        m_pDoorLockParticle->SetShow( !IsLive() && (IDS_NORMAL == GetDoorState()) );	
}


void KGCIronDoor::Render( void )
{
    switch( GetDoorState())
    {
        case IDS_NORMAL:
            m_pIronDoorNormal->SetShow(true);
            m_pIronDoorNormal->Render();
            m_pIronDoorNormal->SetShow(false);
            break;
        case IDS_DAMAGE:
            m_pIronDoorDamage->SetShow(true);
            m_pIronDoorDamage->Render();
            m_pIronDoorDamage->SetShow(false);
            break;
        case IDS_OPEN:
            m_pIronDoorCrash->SetShow(true);
            m_pIronDoorCrash->Render();
            m_pIronDoorCrash->SetShow(false);
            break;
    }
}

GCCollisionRect<float> KGCIronDoor::GetCollisionRect( void )
{
    GCCollisionRect<float> kCollisionRect;

    float fHeight = m_pIronDoorNormal->GetRealHeight();
    float fWidth = m_pIronDoorNormal->GetRealWidth();
    
    float fX = m_pIronDoorNormal->GetMatrix()->GetPos().x;
    float fY = m_pIronDoorNormal->GetMatrix()->GetPos().y;

    kCollisionRect.m_Top = fY + fHeight;
    kCollisionRect.m_Bottom = fY;

    kCollisionRect.m_Left = fX - ( fWidth * 0.5f );
    kCollisionRect.m_Right = fX + ( fWidth * 0.5f );

    return kCollisionRect;
}

bool KGCIronDoor::OnDamage( int iPlayerIndex, float fAttackPower
                      , EGCAttackType eAttackType, EGCAttackDirection eAttackDirection
                      , float fPushTargetX, float fPushTargetY, bool bSpecialAttack )
{
    SetCurHit( GetCurHit()+1 );
    if( GetCurHit() > GetMaxHit() ) SetCurHit( GetMaxHit() );

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        if( (GetMaxHit() <= GetCurHit() || GetCrashHit() <= GetCurHit()) )
        {
            if( IDS_OPEN != GetDoorState() )
            {
                SetDoorState(IDS_OPEN);
                SendPacketCurDoorState();
            }
        }
        else if( (GetDamageHit() <= GetCurHit()) )
        {
            if( IDS_DAMAGE != GetDoorState() )
            {
                SetDoorState(IDS_DAMAGE);
                SendPacketCurDoorState();
            }
        }
        else
        {
            if( IDS_NORMAL != GetDoorState() )
            {
                SetDoorState(IDS_NORMAL);
                SendPacketCurDoorState();
            }
        }
    }

    return true;
}

bool KGCIronDoor::IsLive( void ) const
{
    return m_bLive;
}

bool KGCIronDoor::IsCrashCheck(void) const
{
    if( m_bLive && IDS_OPEN != GetDoorState() )
        return true;

    return false;
}

void KGCIronDoor::SetDoorState( KGCDoor::IRON_DOOR_STATE enState )
{
	KGCDoor::SetDoorState(enState);
	switch( enState )
	{
	case IDS_NORMAL:
		m_pDirectionArrow->AllHideArrowParticle();
		m_pIronDoorNormal->Stop();
		m_pIronDoorDamage->Stop();
		m_pIronDoorCrash->Stop();
		m_pIronDoorNormal->Start( CKTDGAnim::PT_LOOP );
		m_enDoorState = IDS_NORMAL;
		break;
	case IDS_DAMAGE:
		m_pIronDoorNormal->Stop();
		m_pIronDoorDamage->Stop();
		m_pIronDoorCrash->Stop();
		m_pIronDoorDamage->Start( CKTDGAnim::PT_LOOP );
		m_enDoorState = IDS_DAMAGE;
		break;
	case IDS_OPEN:
		m_pIronDoorNormal->Stop();
		m_pIronDoorDamage->Stop();
		m_pIronDoorCrash->Stop();
		m_pIronDoorCrash->Start( CKTDGAnim::PT_LOOP );
		m_enDoorState = IDS_OPEN;
		EnableLive(false);
		break;
	}
}

