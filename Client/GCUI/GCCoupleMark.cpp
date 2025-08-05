#include "StdAfx.h"
#include "GCCoupleMark.h"


#include "GCCoupleSystem.h"


IMPLEMENT_CLASSNAME( KGCCoupleMark );
IMPLEMENT_WND_FACTORY( KGCCoupleMark );
IMPLEMENT_WND_FACTORY_NAME( KGCCoupleMark, "gc_couple_mark" );

KGCCoupleMark::ECoupleMarkKind KGCCoupleMark::ms_eLastUsedMark;
std::map<DWORD, KGCCoupleMark::ECoupleMarkKind> KGCCoupleMark::ms_mapCouple;

KGCCoupleMark::KGCCoupleMark(void)
: m_bForceMode( false )
, m_dwMyUID( 0 )
, m_dwPartnerUID( 0 )
, m_eCoupleState( GCCM_LOCKED )
, m_eSize( GCCM_MINI_SIZE )
, m_eKind( GCCM_MARK_YELLOW )
{
	for( int i = 0 ; i < GCCM_MARK_NUM ; ++i )
	{
		m_pkMark[i] = NULL;
		char temp[MAX_PATH];
		sprintf( temp, "couple%d", i + 1);
		LINK_CONTROL( temp, m_pkMark[i] );
	}
	m_pkLocked = NULL;;
	LINK_CONTROL( "couple_lock", m_pkLocked );
}

KGCCoupleMark::~KGCCoupleMark(void)
{
}

void KGCCoupleMark::OnCreate( void )
{
	m_pkLocked->InitState( true );

	for( int i = 0 ; i < GCCM_MARK_NUM ; ++i )
	{
		m_pkMark[i]->InitState( false );
	}
}

void KGCCoupleMark::FrameMoveInEnabledState( void )
{
	if( m_bForceMode )
		return;

	//m_pExpEvent1 = g_ParticleManager->CreateSequence( strSequenceName1, 0.0f, -0.15f, 1.0f );
	//m_pExpEvent2 = g_ParticleManager->CreateSequence( strSequenceName2, 0.0f, -0.15f, 1.0f );
	if( this->IsRenderOn() == false )
	{
		for( int i = 0 ; i < (int)m_vecParticle.size() ; ++i )
		{
			if( m_vecParticle[i] == NULL )
				continue;

			g_ParticleManager->DeleteSequence( m_vecParticle[i] );
		}
	}
	else{
		if( m_dwPartnerUID == 0 ) return;

		this->ToggleRender( !SiKGCCoupleSystem()->Are_you_Solo( m_dwMyUID ) );

        if( SiKGCCoupleSystem()->GetCoupleUID( m_dwPartnerUID ) == 0 )
        {
            ms_mapCouple.erase( m_dwPartnerUID );
            DeleteParticle();
        }

        int nNum = 0;
        nNum += (ms_mapCouple.find( m_dwMyUID ) != ms_mapCouple.end() ? 1:0); 
        nNum += (ms_mapCouple.find( m_dwPartnerUID ) != ms_mapCouple.end() ? 1:0);

		//bool bCoupleExist = ms_mapCouple.find( m_dwMyUID ) != ms_mapCouple.end() &&
		//					ms_mapCouple.find( m_dwPartnerUID ) != ms_mapCouple.end();

		//bool bOneperson = !bCoupleExist &&
		//	(SiKGCCoupleSystem()->GetCoupleUID( m_dwMyUID ) != 0 ||
		//	SiKGCCoupleSystem()->GetCoupleUID( m_dwPartnerUID ) != 0 );

        switch( nNum )
        {
        case 2:
            if(  m_eCoupleState == GCCM_LOCKED )
            {
                ToggleMark( ms_mapCouple[m_dwMyUID] );
                m_eCoupleState = GCCM_ACTIVE;
                CreateParticle( m_eSize );
            }
            break;
        case 1:
            if( m_eCoupleState == GCCM_ACTIVE)
            {
                ToggleMark();
                m_eCoupleState = GCCM_LOCKED;
                DeleteParticle();
            }
        }

	}
}

void KGCCoupleMark::FrameMoveInDisabledState( void )
{
    DeleteParticle();
}

void KGCCoupleMark::SetCoupleUID( DWORD dwMyUID, DWORD dwCoupleUID )
{
	m_bForceMode = false;
	MarkDisable();
	m_dwMyUID = dwMyUID;
	m_dwPartnerUID = dwCoupleUID;

	if( m_dwPartnerUID == 0 )
	{
		this->ToggleRender( false );
		return;
	}

	m_eCoupleState = GCCM_LOCKED;
	this->ToggleRender( true );
	if( ms_mapCouple.find( dwCoupleUID ) != ms_mapCouple.end() )
	{
		ms_mapCouple[m_dwMyUID] = ms_mapCouple[dwCoupleUID];
	}
	else if( ms_mapCouple.find( m_dwMyUID ) == ms_mapCouple.end() )
	{
		ms_eLastUsedMark = (ECoupleMarkKind)( ((int)ms_eLastUsedMark + 1) % (int)GCCM_MARK_NUM);
		ms_mapCouple.insert( std::map<DWORD, KGCCoupleMark::ECoupleMarkKind>::value_type(m_dwMyUID, ms_eLastUsedMark ));
	}
	ToggleMark();
}

void KGCCoupleMark::SetSize( ECoupleMarkSize eSize )
{
	m_eSize = eSize;
}

void KGCCoupleMark::MarkDisable()
{
	//ms_mapCouple.erase( m_dwMyUID );
    DeleteParticle();

}

void KGCCoupleMark::ToggleMark( ECoupleMarkKind eKind /*= GCCM_MARK_NUM */ )
{
	for( int i = 0 ; i < GCCM_MARK_NUM ; ++i )
	{
		m_pkMark[i]->ToggleRender( (int)eKind == i );
	}
	m_pkLocked->ToggleRender( (int)eKind == GCCM_MARK_NUM );
    if( (int)eKind == GCCM_MARK_NUM )
        DeleteParticle();

}

KGCCoupleMark::ECoupleMarkKind KGCCoupleMark::GetCurrentMark()
{
	for( int i = 0 ; i < GCCM_MARK_NUM ; ++i )
	{
		if( m_pkMark[i]->IsRenderOn() )
			return (ECoupleMarkKind)i;
	}
	return GCCM_MARK_NUM;
}

void KGCCoupleMark::CreateParticle( ECoupleMarkSize eSize )
{
    m_vecParticle.clear();

#if defined( _DEBUG )
	KLuaManager luaMgr;
	GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );

	float Px1, Py1;
	LUA_GET_VALUE_DEF( "Px1", Px1, 375.0f );
	LUA_GET_VALUE_DEF( "Py1", Py1, 630.0f );

	float Px2, Py2;
	LUA_GET_VALUE_DEF( "Px2", Px2, 0.0f );
	LUA_GET_VALUE_DEF( "Py2", Py2, 0.0f );
#endif

	D3DXVECTOR2 vPos = this->GetFixedWindowPos();
	
	float fSmallSizeX = 24.f * GC_SCREEN_DIV_WIDTH;
	float fSmallSizeY = 7.f * GC_SCREEN_DIV_WIDTH;

	float fMiddleSizeX = 27.f * GC_SCREEN_DIV_WIDTH;
	float fMiddleSizeY = 13.f * GC_SCREEN_DIV_WIDTH;

	switch( eSize )
	{
	case GCCM_MINI_SIZE:
		m_vecParticle.push_back( GCFUNC::CreateSequenceToUI( this, "Couple_Light_small",  fSmallSizeX, fSmallSizeY ) );
		m_vecParticle.push_back( GCFUNC::CreateSequenceToUI( this, "Couple_Line_small_1", fSmallSizeX, fSmallSizeY ) );
		m_vecParticle.push_back( GCFUNC::CreateSequenceToUI( this, "Couple_Line_small_2", fSmallSizeX, fSmallSizeY ) );
		break;
	case GCCM_NORMAL_SIZE:
		m_vecParticle.push_back( GCFUNC::CreateSequenceToUI( this, "Couple_Light_middle", fMiddleSizeX, fMiddleSizeY ) );
		m_vecParticle.push_back( GCFUNC::CreateSequenceToUI( this, "Couple_Line_middle_1", fMiddleSizeX, fMiddleSizeY ) );
		m_vecParticle.push_back( GCFUNC::CreateSequenceToUI( this, "Couple_Line_middle_2", fMiddleSizeX, fMiddleSizeY ) );
        break;
	}
}

void KGCCoupleMark::DeleteParticle()
{
    if( m_vecParticle.empty() ) return;
    std::vector< CParticleEventSeqPTR >::iterator vit;
    for( vit = m_vecParticle.begin() ; vit != m_vecParticle.end() ; ++vit )
    {
        g_ParticleManager->DeleteSequence( *vit );
    }

}

void KGCCoupleMark::ForceSetHeart( ECoupleMarkKind eKind )
{
	m_bForceMode = true;
	ToggleMark( eKind );
}

void KGCCoupleMark::ForceShowParticle( ECoupleMarkSize eSize )
{
	CreateParticle( eSize );
}