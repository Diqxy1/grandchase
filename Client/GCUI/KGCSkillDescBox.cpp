#include "stdafx.h"
#include "gcui/KGCSkillDescBox.h"
#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCSkillDescBox );
IMPLEMENT_WND_FACTORY( KGCSkillDescBox );
IMPLEMENT_WND_FACTORY_NAME( KGCSkillDescBox, "gc_skill_desc_box" );

KGCSkillDescBox::KGCSkillDescBox(void)
: m_pkBackGroundBox( NULL )
, m_pkStaticName( NULL )
, m_pkStaticsubInfo1( NULL )
, m_pkStaticsubInfo2( NULL )
, m_pkWndCommandSkill( NULL )
, m_pkWndSlotSkill( NULL )
, m_pkStaticSkillType( NULL )
, m_pkStaticUseDesc( NULL )
, m_pkStaticSkillDesc( NULL )
, m_pkWndSpecial( NULL )
{   
    LINK_CONTROL( "back",                       m_pkBackGroundBox );
    LINK_CONTROL( "static_name",                m_pkStaticName );
    LINK_CONTROL( "static_sub_info1",           m_pkStaticsubInfo1 );
    LINK_CONTROL( "static_sub_info2",           m_pkStaticsubInfo2 );
    LINK_CONTROL( "type_command_skill",         m_pkWndCommandSkill );
    LINK_CONTROL( "type_slot_skill",            m_pkWndSlotSkill );
    LINK_CONTROL( "static_skill_type",          m_pkStaticSkillType );
    LINK_CONTROL( "static_use_desc",            m_pkStaticUseDesc );
    LINK_CONTROL( "static_skill_desc",          m_pkStaticSkillDesc );
    LINK_CONTROL( "type_special",               m_pkWndSpecial );
}

KGCSkillDescBox::~KGCSkillDescBox(void)
{
}

void KGCSkillDescBox::OnCreate( void )
{
    m_pkBackGroundBox->InitState( true );
    m_pkStaticName->InitState( true );
    m_pkStaticsubInfo1->InitState( true );
    m_pkStaticsubInfo2->InitState( true );
    m_pkWndCommandSkill->InitState( false );
    m_pkWndSlotSkill->InitState( false );
    m_pkStaticSkillType->InitState( true );
    m_pkStaticUseDesc->InitState( true );
    m_pkStaticUseDesc->SetFontColor( D3DCOLOR_ARGB( 255, 138, 255, 0 ) );
    m_pkStaticSkillDesc->InitState( true );    
    m_pkWndSpecial->InitState( false );
}

void KGCSkillDescBox::ActionPerformed( const KActionEvent& event )
{
}


void KGCSkillDescBox::FrameMoveInEnabledState( void )
{
}

void KGCSkillDescBox::PostChildDraw( void )
{
}

void KGCSkillDescBox::SetSkillDesc( int iType_, EGCSkillTree eSkillID_ )
{
    const GCSkill* pSkill = SiKGCSkillTree()->GetSkill( eSkillID_ );
    if( pSkill == NULL ) {
        return;
    }

    std::wstring strColorKey = L"#c49d8db";
    std::wstring strColorKeyEnd = L"#cX";
    std::wstringstream wstrm;

    switch( iType_ )
    {
    case EBOX_TYPE_A:
        {
            m_pkWndSpecial->ToggleRender( false );
            m_pkBackGroundBox->OnSetHeight( EBOX_HEIGHT_TYPE_A );
            RenderInfoTypeB( false );
            m_pkStaticName->SetText( SiKGCSkillTree()->GetString( pSkill->GetTitleID() ) );
            wstrm << strColorKey << g_pkStrLoader->GetString( STR_ID_SKILL_DESC_BOX_LEVEL ) << strColorKeyEnd << L" Lv." << pSkill->GetLv();
            m_pkStaticsubInfo1->SetText( wstrm.str() );
            m_pkStaticsubInfo2->SetText(L"");
            m_pkStaticSkillDesc->SetTextAutoMultiline( SiKGCSkillTree()->GetString( pSkill->GetDescID() ) );            
        }
        break; 
    case EBOX_TYPE_B:
        {
            m_pkWndSpecial->ToggleRender( false );
            m_pkBackGroundBox->OnSetHeight( EBOX_HEIGHT_TYPE_B );
            RenderInfoTypeB( true );
            // 스킬이름
            m_pkStaticName->SetText( SiKGCSkillTree()->GetString( pSkill->GetTitleID() ) );            
            // 필요레벨
            wstrm << strColorKey << g_pkStrLoader->GetString( STR_ID_SKILL_DESC_BOX_LEVEL ) << strColorKeyEnd << L" Lv." << pSkill->GetLv();
            m_pkStaticsubInfo1->SetText( wstrm.str() );
            
            if( pSkill->m_pHotKeySkill ) {
                // 쿨타임
                wstrm.str( L"" );
                wstrm << strColorKey << g_pkStrLoader->GetString( STR_ID_SKILL_DESC_BOX_COOLTIME ) << strColorKeyEnd << L" " << pSkill->m_pHotKeySkill->m_fCoolTime << L"초";
                m_pkStaticsubInfo2->SetText( wstrm.str() );

                // 단축키 및 커맨드 스킬 설명
                m_pkWndCommandSkill->ToggleRender( false );
                m_pkStaticSkillType->SetText( g_pkStrLoader->GetString( STR_ID_SKILL_DESC_BOX_SLOT_SKILL ) );

                if( pSkill->m_pHotKeySkill->m_bSpecialSkill ) {
                    m_pkWndSpecial->ToggleRender( true );
                }
            } else {
                m_pkWndSlotSkill->ToggleRender( false );
                m_pkStaticSkillType->SetText( g_pkStrLoader->GetString( STR_ID_SKILL_DESC_BOX_COMMAND_SKILL ) );
            }

            // 스킬 설명
            m_pkStaticSkillDesc->SetTextAutoMultiline( SiKGCSkillTree()->GetString( pSkill->GetDescID() ) );

            // 조작 설명
            m_pkStaticUseDesc->SetTextAutoMultiline( SiKGCSkillTree()->GetString( pSkill->GetControlDescID() ) );
        }
        break;
    default:
        break;
    }
}

void KGCSkillDescBox::RenderInfoTypeB( bool bRender_ )
{
    m_pkWndCommandSkill->ToggleRender( bRender_ );
    m_pkWndSlotSkill->ToggleRender( bRender_ );
    m_pkStaticSkillType->ToggleRender( bRender_ );
    m_pkStaticUseDesc->ToggleRender( bRender_ );
}

void KGCSkillDescBox::SetPosition( D3DXVECTOR2& vPos_ )
{
    this->SetWindowPosDirect( vPos_ );
}


