#include "stdafx.h"
#include "gcui/KGCClassSkillBox.h"
#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCClassSkillBox );
IMPLEMENT_WND_FACTORY( KGCClassSkillBox );
IMPLEMENT_WND_FACTORY_NAME( KGCClassSkillBox, "gc_class_skill_box" );

KGCClassSkillBox::KGCClassSkillBox(void)
: m_pkSkillIcon( NULL )
, m_pkStaticSkillName( NULL )
, m_pkWndMaster( NULL )
, m_pkWndLock( NULL )
, m_pkBtnPlus( NULL )
, m_pkTypeHotKey( NULL )
, m_pkTypeCommand( NULL )
, m_pkStaticOpenLv( NULL )
, m_pkBtnUse( NULL )
, m_pkBtnNoUse( NULL )
, m_eSkillID( ( EGCSkillTree )-1 )
{

    char str[MAX_PATH] = {0,};
    for( int i = 0; i < EMAX_QUICK_KEY_TYPE; ++i )
    {
        m_pkQuickKeyType[i] = NULL;
        sprintf( str, "quick_key_%d", i );  
        LINK_CONTROL( str, m_pkQuickKeyType[i] ); 
    }

    LINK_CONTROL( "img_skill_icon",         m_pkSkillIcon );
    LINK_CONTROL( "static_skill_name",      m_pkStaticSkillName );    
    LINK_CONTROL( "wnd_master",             m_pkWndMaster );
    LINK_CONTROL( "wnd_lock",               m_pkWndLock );
    LINK_CONTROL( "btn_plus",               m_pkBtnPlus );
    LINK_CONTROL( "type_hot_key",           m_pkTypeHotKey );
    LINK_CONTROL( "type_command",           m_pkTypeCommand );
    LINK_CONTROL( "btn_use",                m_pkBtnUse );
    LINK_CONTROL( "btn_nouse",              m_pkBtnNoUse );
    LINK_CONTROL( "static_trainable_lv",    m_pkStaticOpenLv );
    LINK_CONTROL( "hover_frame",            m_pkHoverFrame );
}

KGCClassSkillBox::~KGCClassSkillBox(void)
{
}

void KGCClassSkillBox::OnCreate( void )
{
    m_pkSkillIcon->InitState( true );
    m_pkStaticSkillName->InitState( true );
    m_pkWndMaster->InitState( false );
    m_pkWndLock->InitState( false );
    m_pkBtnPlus->InitState( false, true, this );
    m_pkTypeHotKey->InitState( false );
    m_pkTypeCommand->InitState( false );    
    m_pkStaticOpenLv->InitState( true );
    m_pkStaticOpenLv->SetFontColor( D3DCOLOR_ARGB( 255, 52, 78, 109 ) );
    m_pkStaticOpenLv->SetAlign( DT_CENTER );

    for( int i = 0; i < EMAX_QUICK_KEY_TYPE; ++i ) {
        m_pkQuickKeyType[i]->InitState( false );
    }

    m_pkBtnUse->InitState( false, true, this );
    m_pkBtnNoUse->InitState( false, true, this );
}

void KGCClassSkillBox::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkBtnPlus && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) {        
        SpeakToActionListener( KActionEvent( this, static_cast< DWORD >( m_eSkillID ), ECLICK_TRAIN_SKILL ) );
    }

    if( event.m_pWnd == m_pkBtnUse && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) {
        OnUseSkill();
        SpeakToActionListener( KActionEvent( this, static_cast< DWORD >( m_eSkillID ), ECLICK_USE_NOUSE_BTN ) );            
        return;
    }

    if( event.m_pWnd == m_pkBtnNoUse && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) {                    
        OnNoUseSkill();
        SpeakToActionListener( KActionEvent( this, static_cast< DWORD >( m_eSkillID ), ECLICK_USE_NOUSE_BTN ) );            
        return;
    }
}

void KGCClassSkillBox::SetClassSkillInfo( EGCSkillTree eSkillID_ )
{
    const GCSkill* pSkill = SiKGCSkillTree()->GetSkill( eSkillID_ );

    if( pSkill == NULL )
        return;    

    m_eSkillID = eSkillID_;
    m_pkSkillIcon->SetTexNameUseMassFile( GCUTIL_STR::GCStrCharToWide( pSkill->GetTextureFileName().c_str() ) );
    m_pkStaticSkillName->SetText( SiKGCSkillTree()->GetString( pSkill->GetTitleID() ) );
    SetBoxState( pSkill );
}

void KGCClassSkillBox::SetBoxState( const GCSkill* pSkill_ )
{
    if( pSkill_ == NULL ) {
        return;
    }
    
    if( SiKGCSkillTree()->IsSkillTrainable( pSkill_->GetID() ) ) { // 스킬을 배울 수 있는 상태
        m_pkStaticSkillName->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
        m_pkWndLock->ToggleRender( false );
        m_pkStaticOpenLv->SetText( L"" );         
        m_pkWndMaster->ToggleRender( false );
        m_pkBtnPlus->ToggleRender( true );
        m_pkBtnUse->ToggleRender( false );
        m_pkBtnNoUse->ToggleRender( false );
    } else if( SiKGCSkillTree()->IsLearnedSkill( pSkill_->GetID() ) ) { // 스킬을 익힌 상태
        m_pkStaticSkillName->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
        m_pkWndLock->ToggleRender( false );
        m_pkStaticOpenLv->SetText( L"" );         
        m_pkWndMaster->ToggleRender( true );
        m_pkBtnPlus->ToggleRender( false );
        if( SiKGCSkillTree()->IsEquipedSkill( pSkill_->GetID(), SiKGCSkillTree()->GetCurUICharType(), SiKGCSkillTree()->GetCurUIPromotion() ) ) {
            m_pkBtnUse->ToggleRender( true );
            m_pkBtnNoUse->ToggleRender( false );
        } else {
            m_pkBtnUse->ToggleRender( false );
            m_pkBtnNoUse->ToggleRender( true );
        }
    } else {
        m_pkStaticSkillName->SetFontColor( D3DCOLOR_ARGB( 255, 52, 78, 109 ) );
        m_pkWndLock->ToggleRender( true );
        m_pkWndMaster->ToggleRender( false );
        m_pkBtnPlus->ToggleRender( false );
        m_pkBtnUse->ToggleRender( false );
        m_pkBtnNoUse->ToggleRender( false );
        WCHAR strTemp[MAX_PATH];
        swprintf( strTemp, MAX_PATH, L"LV.%d", pSkill_->GetLv() );
        m_pkStaticOpenLv->SetText( strTemp );
    }

    SetHotKeyState( pSkill_->GetID() );
    SetSkillTypeState( pSkill_ );
}

void KGCClassSkillBox::SetHotKeyState( EGCSkillTree eSkillID_ )
{
    for( int i = 0; i < EMAX_QUICK_KEY_TYPE; i++ ) {
        m_pkQuickKeyType[ i ]->ToggleRender( false );
    }

    std::vector<EGCSkillTree> set = SiKGCSkillTree()->GetHotKeySkillVector( SiKGCSkillTree()->GetCurUICharType(), SiKGCSkillTree()->GetCurUIPromotion() );
    std::vector<EGCSkillTree>::iterator sit = set.begin();

    int iIndex = 0;
    while( sit != set.end() && iIndex < EMAX_QUICK_KEY_TYPE ) {
        if( *sit == eSkillID_ ) {
            m_pkQuickKeyType[ iIndex ]->ToggleRender( true );
            break;
        }
        ++sit;
        ++iIndex;
    }
}

void KGCClassSkillBox::SetSkillTypeState( const GCSkill* pSkill_ )
{
    if( pSkill_ == NULL ) {
        return;
    }

    if( pSkill_->m_pHotKeySkill ) {
        m_pkTypeHotKey->ToggleRender( true );
        if( SiKGCSkillTree()->IsSkillTrainable( pSkill_->GetID() ) || SiKGCSkillTree()->IsLearnedSkill( pSkill_->GetID() ) ) {
            m_pkTypeHotKey->Lock( false );
        } else {
            m_pkTypeHotKey->Lock( true );
        }
        m_pkTypeCommand->ToggleRender( false );
        m_pkBtnUse->ToggleRender( false );
        m_pkBtnNoUse->ToggleRender( false );
    } else {
        m_pkTypeCommand->ToggleRender( true );
        if( SiKGCSkillTree()->IsSkillTrainable( pSkill_->GetID() ) || SiKGCSkillTree()->IsLearnedSkill( pSkill_->GetID() ) ) {
            m_pkTypeCommand->Lock( false );
        } else {
            m_pkTypeCommand->Lock( true );
        }
        m_pkTypeHotKey->ToggleRender( false );        
    }
}

void KGCClassSkillBox::EmptyBoxState()
{
    m_pkSkillIcon->SetTexNameUseMassFile( L"Special75.dds" );;
    m_pkStaticSkillName->SetText( L"" );
    m_pkWndMaster->ToggleRender( false );
    m_pkWndLock->ToggleRender( false );
    m_pkBtnPlus->ToggleRender( false );
    m_pkBtnUse->ToggleRender( false );
    m_pkBtnNoUse->ToggleRender( false );
    m_pkTypeCommand->ToggleRender( false );
    m_pkStaticOpenLv->SetText( L"" );

    for( int i = 0; i < EMAX_QUICK_KEY_TYPE; i++ ) {
        m_pkQuickKeyType[i]->ToggleRender( false );
    }
}

void KGCClassSkillBox::OnUseSkill()
{
    m_pkBtnUse->ToggleRender( false );
    m_pkBtnNoUse->ToggleRender( true );
    const GCSkill* pSkill = SiKGCSkillTree()->GetSkill( m_eSkillID );
    if( pSkill ) {
        SiKGCSkillTree()->UnequipSkill( pSkill->GetGroupID() );
    }
}

void KGCClassSkillBox::OnNoUseSkill()
{
    m_pkBtnUse->ToggleRender( true );
    m_pkBtnNoUse->ToggleRender( false );
    SiKGCSkillTree()->EquipSkill( m_eSkillID );    
}

void KGCClassSkillBox::RenderHoverFrame( bool bRender_ )
{
    m_pkHoverFrame->ToggleRender( bRender_ );
}