#include "stdafx.h"
#include "gcui/KGCAwakeningSkillBox.h"
#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCAwakeningSkillBox );
IMPLEMENT_WND_FACTORY( KGCAwakeningSkillBox );
IMPLEMENT_WND_FACTORY_NAME( KGCAwakeningSkillBox, "gc_awakening_skill_box" );

KGCAwakeningSkillBox::KGCAwakeningSkillBox(void)
: m_pkAwakeningSkillIcon( NULL )
, m_pkDescAwakening( NULL )
, m_pkBtnPlus( NULL )
, m_eSkillID( (EGCSkillTree)-1 )
{
    char str[MAX_PATH] = {0,};
    for( int i = 0; i < EMAX_AWAKENING_CLASS; ++i )
    {
        m_pkAwakeningClass[i] = NULL;
        sprintf( str, "awakening_class_%d", i );  
        LINK_CONTROL( str, m_pkAwakeningClass[i] ); 
    }

    LINK_CONTROL( "img_skill_icon",         m_pkAwakeningSkillIcon );
    LINK_CONTROL( "static_awakening_desc",  m_pkDescAwakening );
    LINK_CONTROL( "btn_plus",               m_pkBtnPlus );
}

KGCAwakeningSkillBox::~KGCAwakeningSkillBox(void)
{
}

void KGCAwakeningSkillBox::OnCreate( void )
{
    InitAwakeningSkillBox();
}

void KGCAwakeningSkillBox::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkBtnPlus && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) {        
        SpeakToActionListener( KActionEvent( this, static_cast< DWORD >( m_eSkillID ), ECLICK_TRAINNIG_BTN ) );
    }
}

void KGCAwakeningSkillBox::SetAwakeningSkill( EGCSkillTree eSkillID_ )
{
    const GCSkill* pSkill = SiKGCSkillTree()->GetSkill( eSkillID_ );

    if( pSkill == NULL ) {
        InitAwakeningSkillBox();
        return;
    }

    if( pSkill->m_iAwakeningLv <= 0 || pSkill->m_iAwakeningLv > EMAX_AWAKENING_CLASS ) {
        return;
    }

    m_eSkillID = eSkillID_;

    for( int i = 0; i < EMAX_AWAKENING_CLASS; i++ ) {
        m_pkAwakeningClass[i]->ToggleRender( false );
    }
    int iRealIndex = pSkill->m_iAwakeningLv - 1;
    m_pkAwakeningClass[ iRealIndex ]->ToggleRender( true );

    if( SiKGCSkillTree()->IsSkillTrainable( pSkill->GetID() ) ) {
        m_pkDescAwakening->SetText( SiKGCSkillTree()->GetString( pSkill->GetTitleID() ) );
        m_pkBtnPlus->Lock( false );
        m_pkAwakeningSkillIcon->SetTexNameUseMassFile( L"Special75.dds" );
        m_pkDescAwakening->SetFontColor( D3DCOLOR_ARGB( 255, 52, 78, 109 ) );
        m_pkBtnPlus->ToggleRender( true );                        
        m_pkAwakeningClass[ iRealIndex ]->Lock( true );
    } else if( SiKGCSkillTree()->IsLearnedSkill( pSkill->GetID() ) ) {
        m_pkDescAwakening->SetText( SiKGCSkillTree()->GetString( pSkill->GetTitleID() ) );
        m_pkBtnPlus->Lock( false );
        m_pkAwakeningSkillIcon->SetTexNameUseMassFile( GCUTIL_STR::GCStrCharToWide( pSkill->GetTextureFileName().c_str() ) );
        m_pkDescAwakening->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
        m_pkBtnPlus->ToggleRender( false );
        m_pkAwakeningClass[ iRealIndex ]->Lock( false );
    } else {
        m_pkAwakeningSkillIcon->SetTexNameUseMassFile( L"Special75.dds" );
        m_pkDescAwakening->SetText( SiKGCSkillTree()->GetString( STR_ID_AWAKENING_SKILL_CLOSE ) );
        m_pkDescAwakening->SetFontColor( D3DCOLOR_ARGB( 255, 52, 78, 109 ) );
        m_pkBtnPlus->ToggleRender( true );
        m_pkBtnPlus->Lock( true );
        m_pkAwakeningClass[ iRealIndex ]->Lock( true );
    }
}

void KGCAwakeningSkillBox::InitAwakeningSkillBox()
{
    m_pkAwakeningSkillIcon->InitState( true );
    m_pkAwakeningSkillIcon->SetTexNameUseMassFile( L"Special75.dds" );

    m_pkDescAwakening->InitState( true );
    m_pkDescAwakening->SetAlign( DT_CENTER );
    m_pkDescAwakening->SetText( L"" );

    for( int i = 0; i < EMAX_AWAKENING_CLASS; i++ ) {
        m_pkAwakeningClass[i]->InitState( false );
    }

    m_pkBtnPlus->InitState( false, true, this );

    m_eSkillID = ( EGCSkillTree )-1;
}
