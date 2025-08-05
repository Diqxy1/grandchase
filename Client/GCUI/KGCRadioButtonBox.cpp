#include "stdafx.h"
#include ".\KGCRadioButtonBox.h"

//

//
//
//



IMPLEMENT_CLASSNAME( KGCRadioButtonBox );
IMPLEMENT_WND_FACTORY( KGCRadioButtonBox );
IMPLEMENT_WND_FACTORY_NAME( KGCRadioButtonBox, "gc_radio_button_box" );

KGCRadioButtonBox::KGCRadioButtonBox(void)
{
    m_pkOriginalCheckBox = NULL;
    m_pkOriginalCheck = NULL;
    m_pkOriginalPetName = NULL;

    m_pkNewCheckBox = NULL;
    m_pkNewCheck = NULL;
    m_pkNewPetName = NULL;

    LINK_CONTROL( "original_check_box",     m_pkOriginalCheckBox );
    LINK_CONTROL( "original_check",         m_pkOriginalCheck );
    LINK_CONTROL( "original_name_static",   m_pkOriginalPetName );
    LINK_CONTROL( "new_check_box",          m_pkNewCheckBox );
    LINK_CONTROL( "new_check",              m_pkNewCheck );
    LINK_CONTROL( "new_name_static",        m_pkNewPetName );

    m_bCheckFirst = true;
}

KGCRadioButtonBox::~KGCRadioButtonBox(void)
{
}

void KGCRadioButtonBox::OnCreate( void )
{
    m_pkOriginalCheckBox->InitState( true, true, this );
    m_pkOriginalCheck->InitState( true, true, this );
    m_pkOriginalPetName->InitState( true, true, this );

    m_pkNewCheckBox->InitState( true, true, this );
    m_pkNewCheck->InitState( true, true, this );
    m_pkNewPetName->InitState( true, true, this );
}

void KGCRadioButtonBox::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
        g_KDSound.Play( "30" );

    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if( event.m_pWnd == m_pkOriginalCheckBox || event.m_pWnd == m_pkOriginalCheck || event.m_pWnd == m_pkOriginalPetName )
        {
            g_KDSound.Play( "31" );
            ChangeCheck( true );
        }
        else if( event.m_pWnd == m_pkNewCheckBox || event.m_pWnd == m_pkNewCheck || event.m_pWnd == m_pkNewPetName )
        {
            g_KDSound.Play( "31" );
            ChangeCheck( false );
        }
    }
}

void KGCRadioButtonBox::SetPetName( std::wstring strOriginalPetName, int iOriginalPetLevel, std::wstring strNewPetName, int iNewPetLevel, bool bCheckFirst )
{
    m_strOriginalPetName = strOriginalPetName;
    m_strNewPetName = strNewPetName;

    m_pkOriginalPetName->SetText( m_strOriginalPetName );
    m_pkNewPetName->SetText( m_strNewPetName );

    ChangeCheck( bCheckFirst );
}

void KGCRadioButtonBox::ChangeCheck( bool bCheckFirst )
{
    m_bCheckFirst = bCheckFirst;

    m_pkOriginalCheck->ToggleRender( m_bCheckFirst );
    m_pkNewCheck->ToggleRender( !m_bCheckFirst );
}