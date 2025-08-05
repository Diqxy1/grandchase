#include "StdAfx.h"
#include "KGCCharDependentGauge.h"


IMPLEMENT_CLASSNAME( KGCCharDependentGauge );
IMPLEMENT_WND_FACTORY( KGCCharDependentGauge );
IMPLEMENT_WND_FACTORY_NAME( KGCCharDependentGauge, "GCCharDependentGauge" );

KGCCharDependentGauge::KGCCharDependentGauge(void) :
m_vBurnningGaugeCoords( 0.0f, 0.0f, 0.0f, 0.0f ),
m_vBurnningActiveCoords( 0.0f, 0.0f, 0.0f, 0.0f )
{
    m_pkBurnningBar			= NULL;
    m_pkBurnningGauge		= NULL;
    m_pkBurnningGaugeLight	= NULL;
    m_pkBurnningGaugeLight2	= NULL;
    m_pkBurnningGaugeLight3	= NULL;
    m_pkBurnningActive		= NULL;
    m_pkBurnningActiveLight = NULL;
    m_pkBurnningActiveLight2= NULL;
    m_pkBurnningActiveLight3= NULL;
    m_pkBurnningLightLine	= NULL;
    m_pkBurnningBlackLine	= NULL;
    m_pkBurnningEdge		= NULL; 
    m_pkBurnningIcon		= NULL;
    m_pkBurnningIconLight1	= NULL;
    m_pkBurnningIconLight2	= NULL;
    m_pkBurnningIconActive1 = NULL;
    m_pkBurnningIconActive2 = NULL;

    LINK_CONTROL( "headup_burnning_bar",			m_pkBurnningBar );
    LINK_CONTROL( "headup_burnning_gauge",			m_pkBurnningGauge );
    LINK_CONTROL( "headup_burnning_gauge_light",	m_pkBurnningGaugeLight );
    LINK_CONTROL( "headup_burnning_gauge_light2",	m_pkBurnningGaugeLight2 );
    LINK_CONTROL( "headup_burnning_gauge_light3",	m_pkBurnningGaugeLight3 );
    LINK_CONTROL( "headup_burnning_active",			m_pkBurnningActive );
    LINK_CONTROL( "headup_burnning_active_light",	m_pkBurnningActiveLight );
    LINK_CONTROL( "headup_burnning_active_light2",	m_pkBurnningActiveLight2 );
    LINK_CONTROL( "headup_burnning_active_light3",	m_pkBurnningActiveLight3 );
    LINK_CONTROL( "headup_burnning_light_line",		m_pkBurnningLightLine );
    LINK_CONTROL( "headup_burnning_black_line",		m_pkBurnningBlackLine );
    LINK_CONTROL( "headup_burnning_edge",			m_pkBurnningEdge );
    LINK_CONTROL( "headup_burnning_icon",			m_pkBurnningIcon );
    LINK_CONTROL( "headup_burnning_icon_light1",	m_pkBurnningIconLight1 );
    LINK_CONTROL( "headup_burnning_icon_light2",	m_pkBurnningIconLight2 );
    LINK_CONTROL( "headup_burnning_icon_active1",	m_pkBurnningIconActive1 );
    LINK_CONTROL( "headup_burnning_icon_active2",	m_pkBurnningIconActive2 );

    m_bBlink = false;
}

KGCCharDependentGauge::~KGCCharDependentGauge(void)
{
}

void KGCCharDependentGauge::OnCreate( void )
{
    m_vBurnningGaugeCoords		= m_pkBurnningGauge->GetWndTexCoords();
    m_vBurnningGaugeLightCoords	= m_pkBurnningGaugeLight3->GetWndTexCoords();
    m_vBurnningActiveCoords		= m_pkBurnningActive->GetWndTexCoords();
    m_vBurnningActiveLightCoords= m_pkBurnningActiveLight3->GetWndTexCoords();
    m_pkBurnningBar->SetSelfInputCheck( false );
    m_pkBurnningGauge->SetSelfInputCheck( false );
    m_pkBurnningGaugeLight->SetSelfInputCheck( false );
    m_pkBurnningGaugeLight2->SetSelfInputCheck( false );
    m_pkBurnningGaugeLight3->SetSelfInputCheck( false );
    m_pkBurnningActive->SetSelfInputCheck( false );
    m_pkBurnningActiveLight->SetSelfInputCheck( false );
    m_pkBurnningActiveLight2->SetSelfInputCheck( false );
    m_pkBurnningActiveLight3->SetSelfInputCheck( false );
    m_pkBurnningIconLight1->SetSelfInputCheck( false );
    m_pkBurnningIconLight2->SetSelfInputCheck( false );
    m_pkBurnningIconActive1->SetSelfInputCheck( false );
    m_pkBurnningIconActive2->SetSelfInputCheck( false );

    m_pkBurnningBar->ToggleRender( false );
}

void KGCCharDependentGauge::SetAmount( float fAmount )
{
    float fTU2		= m_vBurnningGaugeCoords.x + fAmount * ( m_vBurnningGaugeCoords.z - m_vBurnningGaugeCoords.x );
    float fTU2_A	= m_vBurnningActiveCoords.x + fAmount * ( m_vBurnningActiveCoords.z - m_vBurnningActiveCoords.x );

    m_pkBurnningGauge->SetWndTexCoords( D3DXVECTOR4( m_vBurnningGaugeCoords.x,
        m_vBurnningGaugeCoords.y, fTU2,
        m_vBurnningGaugeCoords.w ) );

    m_pkBurnningActive->SetWndTexCoords( D3DXVECTOR4( m_vBurnningActiveCoords.x,
        m_vBurnningActiveCoords.y, fTU2_A,
        m_vBurnningActiveCoords.w ) );

    DWORD dwPrevWidth = m_pkBurnningGauge->GetWidth();
    m_pkBurnningGauge->SetWidthDirect( static_cast<int>( fAmount * MAX_LEN_BURNNING_GAUGE ) );	
    m_pkBurnningActive->SetWidthDirect( static_cast<int>( fAmount * MAX_LEN_BURNNING_GAUGE ) );
    m_pkBurnningGaugeLight->ToggleRender( false );
    m_pkBurnningGaugeLight2->ToggleRender( false );
    m_pkBurnningGaugeLight3->ToggleRender( false );
    m_pkBurnningActiveLight->ToggleRender( false );
    m_pkBurnningActiveLight2->ToggleRender( false );
    m_pkBurnningActiveLight3->ToggleRender( false );
    m_pkBurnningIconLight1->ToggleRender( false );
    m_pkBurnningIconLight2->ToggleRender( false );
    m_pkBurnningIconActive1->ToggleRender( false );
    m_pkBurnningIconActive2->ToggleRender( false );

    const float afBlinkTable[] = { 0.35f,0.67f,1.0f   };

    if( dwPrevWidth < m_pkBurnningGauge->GetWidth() || fAmount == 1.0f )
    {
        m_pkBurnningBlackLine->ToggleRender( false );
        m_pkBurnningGauge->ToggleRender( true );
        m_pkBurnningActive->ToggleRender( false );
        float fWidth = 0.0f;
        for( int i = 0 ; i < sizeof(afBlinkTable) / sizeof( float ) ; i++ )
        {            
            if( m_bLevel )
            {
                if( fAmount >= afBlinkTable[i] )
                {
                    fWidth = afBlinkTable[i];
                }                
            }
            else
            {                
                fWidth = fAmount;
            }
            fTU2	= m_vBurnningGaugeLightCoords.x + fWidth * ( m_vBurnningGaugeLightCoords.z - m_vBurnningGaugeLightCoords.x );            

            m_pkBurnningGaugeLight3->SetWidthDirect( static_cast<int>( fWidth * MAX_LEN_BURNNING_GAUGE ) );

            m_pkBurnningGaugeLight3->SetWndTexCoords( D3DXVECTOR4( m_vBurnningGaugeLightCoords.x,
                m_vBurnningGaugeLightCoords.y, fTU2,
                m_vBurnningGaugeLightCoords.w ) );
        }
        
        if( m_bLevel || fAmount == 1.0f )
        {
            m_pkBurnningLightLine->ToggleRender( true );
            m_pkBurnningIconLight1->ToggleRender( true );
            m_pkBurnningIconLight2->ToggleRender( o10 < 10 );
            m_pkBurnningGaugeLight3->ToggleRender( o10 < 10 );
        }
    }
    else if( m_bBlink )
    {
        m_pkBurnningBlackLine->ToggleRender( false );
        m_pkBurnningActive->ToggleRender( true );

        float fWidth = 0.0f;
        for( int i = 0 ; i < sizeof(afBlinkTable) / sizeof( float ) ; i++ )
        {
            if( m_bLevel )
            {
                if( fAmount >= afBlinkTable[i] )
                {
                    fWidth = afBlinkTable[i];
                }               
            }
            else
            {
                fWidth = fAmount;
            }
            fTU2_A	= m_vBurnningActiveLightCoords.x + fWidth * ( m_vBurnningActiveLightCoords.z - m_vBurnningActiveLightCoords.x );

            m_pkBurnningActiveLight3->SetWidthDirect( static_cast<int>( fWidth * MAX_LEN_BURNNING_GAUGE ) );

            m_pkBurnningActiveLight3->SetWndTexCoords( D3DXVECTOR4( m_vBurnningActiveLightCoords.x,
                m_vBurnningActiveLightCoords.y, fTU2_A,
                m_vBurnningActiveLightCoords.w ) );

        }
        m_pkBurnningActiveLight3->ToggleRender( o10 < 10 );        
        m_pkBurnningLightLine->ToggleRender( true );
        m_pkBurnningIconActive1->ToggleRender( true );
        m_pkBurnningIconActive2->ToggleRender( o10 < 10 );
    }
    else
    {
        m_pkBurnningBar->ToggleRender( true );
        m_pkBurnningEdge->ToggleRender( true );
        m_pkBurnningIcon->ToggleRender( true );
        m_pkBurnningGauge->ToggleRender( true );    
        m_pkBurnningBlackLine->ToggleRender( true );
        m_pkBurnningLightLine->ToggleRender( false );
    }
}

void KGCCharDependentGauge::FrameMoveInEnabledState( void )
{
    
}

IMPLEMENT_CLASSNAME( KGCQPGauge );
IMPLEMENT_WND_FACTORY( KGCQPGauge );
IMPLEMENT_WND_FACTORY_NAME( KGCQPGauge, "GCQPGauge" );

KGCQPGauge::KGCQPGauge( void )
{
    char str[MAX_PATH] = {0,};
    for( int i = 0; i < MAX_QP_GAUGE; ++i ) {
        m_pkQPGaugeActive[i] = NULL;
        sprintf( str, "qp_gauge_active%d", i );
        LINK_CONTROL( str, m_pkQPGaugeActive[i] );
    }
    m_bInit = false;
}

KGCQPGauge::~KGCQPGauge( void )
{
}

void KGCQPGauge::OnCreate( void )
{
    InitQPGauge();
}

void KGCQPGauge::InitQPGauge()
{
    for( int i = 0; i < MAX_QP_GAUGE; ++i ) {
        if( m_pkQPGaugeActive[i] ) {
            m_pkQPGaugeActive[i]->InitState( true );
            m_pkQPGaugeActive[i]->SetHeightDirectInverse( MAX_HEIGHT_QP_GAUGE, MAX_HEIGHT_QP_GAUGE );

            if( m_bInit == false ) {                
                m_vQPOriginalCoords[i] = m_pkQPGaugeActive[i]->GetWndTexCoords();
            }
            m_vQPGaugeActiveCoords[i] = m_vQPOriginalCoords[i];
            m_pkQPGaugeActive[i]->SetWndTexCoords( m_vQPOriginalCoords[i] );
        }
    }
    m_bInit = true;
}

void KGCQPGauge::SetAmount( float fAmount )
{
    int iCurIdx = g_MyD3D->GetMyPlayer()->GetQuantityPoint();
    if( iCurIdx < 0 || 
        iCurIdx >= MAX_QP_GAUGE ||                 
        m_pkQPGaugeActive[ iCurIdx ] == NULL ||
        g_MyD3D->GetMyPlayer()->GetQuantityPoint() >= g_MyD3D->GetMyPlayer()->GetCurFormTemplate().iMaxQP ) {
        return;
    }    
    for( int i = iCurIdx; i < MAX_QP_GAUGE; i++  ) {
        if( m_pkQPGaugeActive[ i ]->GetHeight() == MAX_HEIGHT_QP_GAUGE  ) {
            m_pkQPGaugeActive[ i ]->SetHeightDirectInverse(0, MAX_HEIGHT_QP_GAUGE);
        }
        if( i + 1 < MAX_QP_GAUGE ) {
            m_pkQPGaugeActive[ i + 1 ]->SetHeightDirectInverse(0, MAX_HEIGHT_QP_GAUGE);
        }
    }           
    m_pkQPGaugeActive[iCurIdx]->SetHeightDirectInverse( static_cast< int >( fAmount * static_cast<float>( MAX_HEIGHT_QP_GAUGE )  ), MAX_HEIGHT_QP_GAUGE ); 
    float fTU2 = m_vQPGaugeActiveCoords[ iCurIdx ].w - fAmount * ( m_vQPGaugeActiveCoords[ iCurIdx ].w - m_vQPGaugeActiveCoords[ iCurIdx ].y );
    m_pkQPGaugeActive[ iCurIdx ]->SetWndTexCoords( D3DXVECTOR4( m_vQPGaugeActiveCoords[ iCurIdx ].x, fTU2, m_vQPGaugeActiveCoords[ iCurIdx ].z, m_vQPGaugeActiveCoords[ iCurIdx ].w ) );   

    if( fAmount >= 1.0f) {
        float x = -0.633f;
        float y = 0.523f;
        float fRate = 0.0685f;

        if( g_pGCDeviceManager2->GetWindowScaleX() == 1.f &&
            g_pGCDeviceManager2->GetWindowScaleY() == 1.f ) {
            x = -0.529f;
            y = 0.46f;
            fRate = 0.088f;
        }
    }
}

void KGCQPGauge::PostChildDraw( void )
{
}

IMPLEMENT_CLASSNAME( KGCBloodGauge );
IMPLEMENT_WND_FACTORY( KGCBloodGauge );
IMPLEMENT_WND_FACTORY_NAME( KGCBloodGauge, "GCBloodGauge " );

KGCBloodGauge::KGCBloodGauge( void )
{
	char str[MAX_PATH] = {0,};
	for( int i = 0; i < MAX_BLOOD_GAUGE; ++i ) {
		m_pkBloodGaugeActive[i] = NULL;
		sprintf( str, "blood_gauge_active%d", i );
		LINK_CONTROL( str, m_pkBloodGaugeActive[i] );
	}
	m_bInit = false;
}

KGCBloodGauge::~KGCBloodGauge( void )
{
}

void KGCBloodGauge::OnCreate( void )
{
	InitBloodGauge();
}

void KGCBloodGauge::InitBloodGauge()
{
	for( int i = 0; i < MAX_BLOOD_GAUGE; ++i ) 
	{
		if( m_pkBloodGaugeActive[i])
			m_pkBloodGaugeActive[i]->InitState(true);
	}
}

void KGCBloodGauge::SetAmount( float fAmount )
{
	unsigned int iBloodPoint = g_MyD3D->GetMyPlayer()->GetBloodPoint();
	if(iBloodPoint > MAX_BLOOD_GAUGE )
		return;
	for(int i = 0; i < MAX_BLOOD_GAUGE; i++)
	{
		if(i < (int)iBloodPoint) {
			m_pkBloodGaugeActive[i]->ToggleRender(true);
		}
		else {
			m_pkBloodGaugeActive[i]->ToggleRender(false);
		}
	}
}

void KGCBloodGauge::PostChildDraw( void )
{
}

IMPLEMENT_CLASSNAME( GCSPGauge );
IMPLEMENT_WND_FACTORY( GCSPGauge );
IMPLEMENT_WND_FACTORY_NAME( GCSPGauge, "GCSPGauge" );

GCSPGauge::GCSPGauge( void )
{
    m_pkSieg = NULL;
    m_pkJin = NULL;
    m_pkMari = NULL;
    m_pkQPGauge = NULL;
    m_pkMarbleGauge = NULL;
    m_pkMarbleGaugeRin = NULL;
    m_pkMarbleGaugeZero = NULL;
    m_pkMarbleGaugeRonan = NULL;
	m_pkBloodGauge = NULL;
#ifdef DECANEE
    m_pkMarbleGaugeDecanee = NULL;
#endif

    LINK_CONTROL( "SiegGauge",			m_pkSieg );	
    LINK_CONTROL( "JinGauge",			m_pkJin );	
    LINK_CONTROL( "MariGauge",			m_pkMari );
    LINK_CONTROL( "qp_gauge",			m_pkQPGauge );
    LINK_CONTROL( "marble_gauge",		m_pkMarbleGauge );
    LINK_CONTROL( "marble_gauge_rin",	m_pkMarbleGaugeRin );
    LINK_CONTROL( "marble_gauge_zero",	m_pkMarbleGaugeZero );
    LINK_CONTROL( "marble_gauge_ronan", m_pkMarbleGaugeRonan );
	LINK_CONTROL( "Blood_Gauge",		m_pkBloodGauge );
#ifdef DECANEE
    LINK_CONTROL( "DecaneeGauge", m_pkMarbleGaugeDecanee );
#endif
}

GCSPGauge::~GCSPGauge( void )
{
}

void GCSPGauge::OnCreate( void )
{
}

void GCSPGauge::ToggleRender( bool bRender )
{
    KD3DWnd::ToggleRender( bRender );

    if (bRender && m_pkJin && m_pkSieg && m_pkMari)
    {
        m_pkJin->ToggleRender( false );
        m_pkJin->m_bLevel = true;
        m_pkSieg->ToggleRender( false );        
        m_pkSieg->m_bLevel = false;
        m_pkMari->ToggleRender( false );
        m_pkMari->m_bLevel = false;

        m_pkJin->ToggleRender( g_MyD3D->GetMyPlayer()->GetCurFormTemplate().iSubAbilityType == SUB_ABILITY_BURNNING_MODE );
        m_pkSieg->ToggleRender( g_MyD3D->GetMyPlayer()->GetCurFormTemplate().iSubAbilityType == SUB_ABILITY_FURY_MODE &&  bRender && g_MyD3D->GetMyPlayer()->GetCurrentChar().iCharType != GC_CHAR_RONAN );
        m_pkMari->ToggleRender( g_MyD3D->GetMyPlayer()->GetCurFormTemplate().iSubAbilityType == SUB_ABILITY_HITTING_POINT );
    }
    if( bRender && m_pkQPGauge ) {
        if( g_MyD3D->GetMyPlayer() && g_MyD3D->GetMyPlayer()->IsLearnedSkill( SID_RIN2_SELECT_EVIL ) && g_MyD3D->GetMyPlayer()->GetCurrentChar().iPromotionLevel > 0 )
        {
            m_pkQPGauge->ToggleRender( false );
        } 
        else 
        {
            m_pkQPGauge->ToggleRender( g_MyD3D->GetMyPlayer()->GetCurFormTemplate().iSubAbilityType == SUB_ABILITY_QUANTITY_POINT );
        }        
    }
	if( bRender && m_pkBloodGauge) 
    {
		m_pkBloodGauge->ToggleRender( g_MyD3D->GetMyPlayer()->GetCurFormTemplate().iSubAbilityType == SUB_ABILITY_BLOOD_POINT );
	}
    if( m_pkMarbleGauge ) 
    {
        m_pkMarbleGauge->ToggleRender( false );
    }
    if( m_pkMarbleGaugeRin ) 
    {
        m_pkMarbleGaugeRin->ToggleRender( false );
    }
    if( m_pkMarbleGaugeZero ) 
    {
        m_pkMarbleGaugeZero->ToggleRender( false );
    }
    if ( m_pkMarbleGaugeRonan )
    {
        m_pkMarbleGaugeRonan->ToggleRender( false );
    }
#ifdef DECANEE
    if ( m_pkMarbleGaugeDecanee )
    {
        m_pkMarbleGaugeDecanee->ToggleRender( false );
    }
#endif
    if( bRender && g_MyD3D->GetMyPlayer()->GetCurrentChar().iCharType == GC_CHAR_RONAN && m_pkMarbleGaugeRonan) 
    {
        m_pkMarbleGaugeRonan->m_bLevel = false;
        m_pkMarbleGaugeRonan->ToggleRender( g_MyD3D->GetMyPlayer()->GetCurFormTemplate().iSubAbilityType == SUB_ABILITY_FURY_MODE );
    }
    else if( g_MyD3D->GetMyPlayer()->GetCurrentChar().iCharType == GC_CHAR_RIN ) 
    {
        if( bRender && m_pkMarbleGaugeRin ) 
        {
            m_pkMarbleGaugeRin->m_bLevel = false;     
            m_pkMarbleGaugeRin->ToggleRender( g_MyD3D->GetMyPlayer()->IsUsableMarbleState() );
        }
    } 
    else if( g_MyD3D->GetMyPlayer()->GetCurrentChar().iCharType == GC_CHAR_ZERO ) 
    {
        if( bRender && m_pkMarbleGaugeZero ) 
        {
            m_pkMarbleGaugeZero->m_bLevel = false;     
            m_pkMarbleGaugeZero->ToggleRender( g_MyD3D->GetMyPlayer()->IsUsableMarbleState() );
        }
    } 
#ifdef DECANEE
    else if ( g_MyD3D->GetMyPlayer()->GetCurrentChar().iCharType == GC_CHAR_DECANEE )
    {
        if ( bRender && m_pkMarbleGaugeDecanee)
        {
            m_pkMarbleGaugeDecanee->m_bLevel = false;
            m_pkMarbleGaugeDecanee->ToggleRender( g_MyD3D->GetMyPlayer()->IsUsableMarbleState() );
        }
    }
#endif
    else {
        if( bRender && m_pkMarbleGauge ) 
        {
            m_pkMarbleGauge->m_bLevel = false;     
            m_pkMarbleGauge->ToggleRender( g_MyD3D->GetMyPlayer()->IsUsableMarbleState() );
        } 
    }    
}

void GCSPGauge::SetAmount( float fAmount )
{
    m_pkJin->SetAmount( fAmount );
    m_pkSieg->SetAmount( fAmount );
    m_pkMari->SetAmount( fAmount );
    if( m_pkQPGauge ) 
    {
        m_pkQPGauge->SetAmount( fAmount );
    }    
    if( m_pkMarbleGauge ) 
    {
        m_pkMarbleGauge->SetAmount( fAmount );
    }
    if( m_pkMarbleGaugeRin ) 
    {
        m_pkMarbleGaugeRin->SetAmount( fAmount );
    }
    if (m_pkMarbleGaugeZero)
    {
        m_pkMarbleGaugeZero->SetAmount(fAmount);
    }
    if ( m_pkMarbleGaugeRonan) 
    {
        m_pkMarbleGaugeRonan->SetAmount( fAmount );
    }
	if( m_pkBloodGauge ) 
    {
		m_pkBloodGauge->SetAmount( fAmount );
	}
#ifdef DECANEE
    if ( m_pkMarbleGaugeDecanee )
    {
        m_pkMarbleGaugeDecanee->SetAmount( fAmount );
    }
#endif
}

void GCSPGauge::InitQPGauge()
{
    if( m_pkQPGauge ) 
    {
        m_pkQPGauge->InitQPGauge();
    }    
}

void GCSPGauge::InitBloodGauge()
{
	if( m_pkBloodGauge ) 
    {
		m_pkBloodGauge->InitBloodGauge();
	}    
}
