#include "stdafx.h"
#include "gcui/KGCQuickSkillSlot.h"
#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCQuickSkillSlot );
IMPLEMENT_WND_FACTORY( KGCQuickSkillSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCQuickSkillSlot, "gc_quick_skill_slot" );

KGCQuickSkillSlot::KGCQuickSkillSlot(void)
: m_pkSkillIcon(NULL)
, m_pkWndSelect( NULL )
, m_pkWndSelectLine( NULL )
, m_eSkillID( ( EGCSkillTree )-1 )
{
    LINK_CONTROL( "img_skill_icon",     m_pkSkillIcon );
    LINK_CONTROL( "wnd_select",         m_pkWndSelect );
    LINK_CONTROL( "wnd_select_line",    m_pkWndSelectLine );
}

KGCQuickSkillSlot::~KGCQuickSkillSlot(void)
{
}

void KGCQuickSkillSlot::OnCreate( void )
{
    m_pkSkillIcon->InitState( true );
    m_pkWndSelect->InitState( false );
    m_pkWndSelectLine->InitState( false );
}

void KGCQuickSkillSlot::SetSkillImage( EGCSkillTree eSkillID_ )
{
    m_eSkillID = eSkillID_;

    const GCSkill* pSkill = SiKGCSkillTree()->GetSkill( eSkillID_ );

    if( pSkill == NULL ) {
        m_pkSkillIcon->SetTexNameUseMassFile( L"Special75.dds" );
    } else {
        m_pkSkillIcon->SetTexNameUseMassFile( GCUTIL_STR::GCStrCharToWide( pSkill->GetTextureFileName().c_str() ) );
    }

    m_pkSkillIcon->ToggleRender( true );
}

void KGCQuickSkillSlot::InitSkillImage()
{
    m_pkSkillIcon->SetTexNameUseMassFile( L"Special75.dds" );        
    m_eSkillID = ( EGCSkillTree )SID_DIO1_EMPTY_SLOT;
}

void KGCQuickSkillSlot::UnEquipSlot()
{
    SiKGCSkillTree()->UnequipSkill( SiKGCSkillTree()->GetSkill( (EGCSkillTree)m_eSkillID )->GetGroupID() );
    InitSkillImage();
}

void KGCQuickSkillSlot::EquipSlot( EGCSkillTree eSkillID_ )
{
    SiKGCSkillTree()->EquipSkill( eSkillID_ );
    SetSkillImage( eSkillID_ );
}

void KGCQuickSkillSlot::SetSelectSlot( bool bSelect_ )
{
    m_pkWndSelect->ToggleRender( bSelect_ );
}

bool KGCQuickSkillSlot::IsSelectedSlot()
{
    return m_pkWndSelect->IsRenderOn();
}

void KGCQuickSkillSlot::HoverQuickSlot( bool bHover_ )
{
    m_pkWndSelectLine->ToggleRender( bHover_ );
}
