#include "stdafx.h"
#include ".\gccharacterui.h"

IMPLEMENT_CLASSNAME( KGCCharacterUI );
IMPLEMENT_WND_FACTORY( KGCCharacterUI );
IMPLEMENT_WND_FACTORY_NAME( KGCCharacterUI, "ui_character" );

KGCCharacterUI::KGCCharacterUI(void)
{
    m_iEnableChar = 0;

    for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
    {
        m_pkChar[i] = NULL;
        char strControlName[MAX_PATH] = {0,};
        sprintf( strControlName, "char%d", i );
        LINK_CONTROL( strControlName, m_pkChar[i] );
    }
    m_pkAllChar = NULL;
    LINK_CONTROL( "char_all",   m_pkAllChar );
}

KGCCharacterUI::~KGCCharacterUI(void)
{
}

void KGCCharacterUI::OnCreate( void )
{
    for( int i = 0 ;i < GC_CHAR_NUM ; ++i )
    {
        if( m_pkChar[i] != NULL )
        {
            m_pkChar[i]->InitState( m_iEnableChar == i , true, NULL );
            m_pkChar[i]->Activate( false );
        }
    }

    if ( m_pkAllChar )
    {
        m_pkAllChar->InitState( false, true, NULL );
        m_pkAllChar->Activate( false );
    }
}

void KGCCharacterUI::SetToggleRender( bool bRender /*= false*/ )
{
    for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
    {
        if( m_pkChar[i] != NULL )
            m_pkChar[i]->ToggleRender( bRender );
    }
    if( m_pkAllChar != NULL )
        m_pkAllChar->ToggleRender( bRender );
}

void KGCCharacterUI::Active()
{
	if( m_iEnableChar == -1 )
	{
        if ( m_pkAllChar )
		    m_pkAllChar->Lock( true );
	}
	else
	{
		m_pkChar[m_iEnableChar]->Lock( true );
	}
}

void KGCCharacterUI::DeActive()
{
	if( m_iEnableChar == -1 )
	{
        if ( m_pkAllChar )
		    m_pkAllChar->Lock( false );
	}
	else
	{
		m_pkChar[m_iEnableChar]->Lock( false );
	}
}
