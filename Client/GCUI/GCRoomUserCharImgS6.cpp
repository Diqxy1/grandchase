#include "stdafx.h"
#include "GCRoomUserCharImgS6.h"

IMPLEMENT_CLASSNAME( KGCRoomUserCharImgS6 );
IMPLEMENT_WND_FACTORY( KGCRoomUserCharImgS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomUserCharImgS6, "gc_room_user_charimg_s6" );

KGCRoomUserCharImgS6::KGCRoomUserCharImgS6( void )
: m_pkLineSel( NULL ) 
{
    char str[MAX_PATH] = {0,};

    for ( int i = 0; i < GC_CHAR_NUM; ++i ) 
    {
        m_apkCharImg[i] = NULL;

        sprintf( str, "char%d", i );          
        LINK_CONTROL( str,  m_apkCharImg[i] );
    }

    LINK_CONTROL( "linesel",  m_pkLineSel );
}

KGCRoomUserCharImgS6::~KGCRoomUserCharImgS6( void )
{
}


void KGCRoomUserCharImgS6::ActionPerformed( const KActionEvent& event )
{
}


void KGCRoomUserCharImgS6::OnCreate( void )
{
    for ( int i = 0; i < GC_CHAR_NUM; ++i ) 
    {
        if ( m_apkCharImg[i] ) 
        {
            m_apkCharImg[i]->InitState( true, true, this );
        }
    }
    
    if ( m_pkLineSel ) 
        m_pkLineSel->InitState( true, true, this );
}

void KGCRoomUserCharImgS6::SetChar( int iCharType )
{
    for ( int i = 0; i < GC_CHAR_NUM; ++i ) 
    {
        if ( m_apkCharImg[i] ) 
        {
            m_apkCharImg[i]->ToggleRender( false );
        }
    }

    if ( m_apkCharImg[iCharType] ) 
        m_apkCharImg[iCharType]->ToggleRender( true );
}

void KGCRoomUserCharImgS6::UpdateLineSel( bool bRender )
{
    if ( m_pkLineSel ) 
        m_pkLineSel->ToggleRender( bRender );
}