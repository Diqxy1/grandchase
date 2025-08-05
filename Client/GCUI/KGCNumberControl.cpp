#include "stdafx.h"
#include ".\KGCNumberControl.h"

IMPLEMENT_CLASSNAME( KGCNumberControl );
IMPLEMENT_WND_FACTORY( KGCNumberControl );
IMPLEMENT_WND_FACTORY_NAME( KGCNumberControl, "gc_number_control" );

KGCNumberControl::KGCNumberControl(void)
{
    for( int i = 0; i < 10; ++i )
    {
        m_apkNumber[i] = NULL;
    }
   
    LINK_CONTROL( "number0", m_apkNumber[0] );
    LINK_CONTROL( "number1", m_apkNumber[1] );
    LINK_CONTROL( "number2", m_apkNumber[2] );
    LINK_CONTROL( "number3", m_apkNumber[3] );
    LINK_CONTROL( "number4", m_apkNumber[4] );
    LINK_CONTROL( "number5", m_apkNumber[5] );
    LINK_CONTROL( "number6", m_apkNumber[6] );
    LINK_CONTROL( "number7", m_apkNumber[7] );
    LINK_CONTROL( "number8", m_apkNumber[8] );
    LINK_CONTROL( "number9", m_apkNumber[9] );
}

KGCNumberControl::~KGCNumberControl(void)
{
}

void KGCNumberControl::OnCreate( void )
{
    for( int i = 0; i < 10; ++i )
    {
        m_apkNumber[i]->InitState( false );
    }
}

void KGCNumberControl::AllRenderOff()
{
    for( int i = 0; i < 10; ++i )
    {
        m_apkNumber[i]->ToggleRender( false );
    }
}

void KGCNumberControl::SetNumber( int iNumber )
{
    if( iNumber > 9 )
        iNumber = 9;
    if( iNumber < 0 )
        iNumber = 0;

    AllRenderOff();
    m_apkNumber[iNumber]->ToggleRender( true );
}