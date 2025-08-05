#include "stdafx.h"
#include "GCLimitedTime.h"


IMPLEMENT_CLASSNAME( KGCLimitedTime );
IMPLEMENT_WND_FACTORY( KGCLimitedTime );
IMPLEMENT_WND_FACTORY_NAME( KGCLimitedTime, "gc_limited_time" );

KGCLimitedTime::KGCLimitedTime( void )
: m_pkClock( NULL )
{
    char szTemp[1024];
    for(int iLoop=0; iLoop<6; ++iLoop)
    {
        m_pkMitNumber10[iLoop] = NULL;
        m_pkSecNumber10[iLoop] = NULL;

        sprintf(szTemp, "mit_num_%02d", iLoop*10);
        LINK_CONTROL( szTemp, m_pkMitNumber10[iLoop] );
        sprintf(szTemp, "sec_num_%02d", iLoop*10);
        LINK_CONTROL( szTemp, m_pkSecNumber10[iLoop] );
    }

    for(int iLoop=0; iLoop<10; ++iLoop)
    {
        m_pkMitNumber1[iLoop] = NULL;
        m_pkSecNumber1[iLoop] = NULL;

        sprintf(szTemp, "mit_num_%d", iLoop);
        LINK_CONTROL( szTemp, m_pkMitNumber1[iLoop] );
        sprintf(szTemp, "sec_num_%d", iLoop);
        LINK_CONTROL( szTemp, m_pkSecNumber1[iLoop] );
    }

    LINK_CONTROL( "clock", m_pkClock );
}

KGCLimitedTime::~KGCLimitedTime( void )
{

}

void KGCLimitedTime::OnCreate( void )
{
    if( m_pkClock )
        m_pkClock->InitState( true );

    HiddenNumber();

    m_iTremorFrame = 0;
    m_bShowClockAni = false;
}

void KGCLimitedTime::FrameMoveInEnabledState( void )
{
    if( m_pkClock )
    {
        if( m_bShowClockAni )
        {
            if ( m_iTremorFrame < CLOCK_ANI_DELAY_TIME )
                m_pkClock->SetWndMode( D3DWM_HOVER );
            else if ( m_iTremorFrame < CLOCK_ANI_DELAY_TIME * 2 )
                m_pkClock->SetWndMode( D3DWM_DEFAULT );
            else if ( m_iTremorFrame < CLOCK_ANI_DELAY_TIME * 3 )
                m_pkClock->SetWndMode( D3DWM_SELECT );
            else if ( m_iTremorFrame < CLOCK_ANI_DELAY_TIME * 4 ) 
            {
                m_pkClock->SetWndMode( D3DWM_DEFAULT );
                m_iTremorFrame = 0;
            }

            ++m_iTremorFrame;
        }
        else
        {
            m_pkClock->SetWndMode( D3DWM_DEFAULT );
        }
    }
}

void KGCLimitedTime::SetTime( float fTime )
{
    int iMit = int(fTime)/60;
    int iSec = int(fTime)%60;
    m_bShowClockAni = false;

    SLimitedTimeInfo* pLimitedTimeInfo = SiKGCWorldMapManager()->GetLimitedTimeInfo( SiKGCRoomManager()->GetGameMode() );

    if( pLimitedTimeInfo )
    {
        std::vector<SAlarmInfo>::iterator vecIter = pLimitedTimeInfo->m_vecAlarmInfo.begin();
        for(; vecIter != pLimitedTimeInfo->m_vecAlarmInfo.end(); ++vecIter)
        {
            if( (vecIter->iMitMin <= iMit && vecIter->iSecMin <= iSec) &&  
                (vecIter->iMitMax >= iMit && vecIter->iSecMax >= iSec) )
            {
                if( "" != vecIter->strSound )
                    g_KDSound.PlayNoOverlapping( vecIter->strSound.c_str(), 0, 0 );
                m_bShowClockAni = true;
                break;
            }
        }
    }

    if( iMit > 60 ) iMit = 0;

    HiddenNumber();

    if( m_pkMitNumber10[(iMit / 10)] )
        m_pkMitNumber10[(iMit / 10)]->ToggleRender( true );

    if( m_pkMitNumber1[(iMit % 10)] )
        m_pkMitNumber1[(iMit % 10)]->ToggleRender( true );

    if( m_pkSecNumber10[(iSec / 10)] )
        m_pkSecNumber10[(iSec / 10)]->ToggleRender( true );

    if( m_pkSecNumber1[(iSec % 10)] )
        m_pkSecNumber1[(iSec % 10)]->ToggleRender( true );
}

void KGCLimitedTime::HiddenNumber()
{
    for(int iLoop=0; iLoop<6; ++iLoop)
    {
        if( m_pkMitNumber10[iLoop] )
            m_pkMitNumber10[iLoop]->ToggleRender( false );
        if( m_pkSecNumber10[iLoop] )
            m_pkSecNumber10[iLoop]->ToggleRender( false );
    }

    for(int iLoop=0; iLoop<10; ++iLoop)
    {
        if( m_pkMitNumber1[iLoop] )
            m_pkMitNumber1[iLoop]->ToggleRender( false );
        if( m_pkSecNumber1[iLoop] )
            m_pkSecNumber1[iLoop]->ToggleRender( false );
    }
}