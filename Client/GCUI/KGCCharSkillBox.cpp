#include "stdafx.h"
#include "gcui/KGCCharSkillBox.h"
#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCCharSkillBox );
IMPLEMENT_WND_FACTORY( KGCCharSkillBox );
IMPLEMENT_WND_FACTORY_NAME( KGCCharSkillBox, "gc_char_skill_box" );

KGCCharSkillBox::KGCCharSkillBox(void)
: m_pkSkillIcon( NULL )
, m_pkStaticSkillName( NULL )
, m_pkStaticCurSkillLevel( NULL )
, m_pkWndMaster( NULL )
, m_pkWndLock( NULL )
, m_pkBtnPlus( NULL )
, m_pkBtnMinus( NULL )
, m_pkStaticOpenLv( NULL )
, m_pkHoverFrame( NULL )
, m_eSkillID( ( EGCSkillTree )-1 )
{   
    LINK_CONTROL( "img_skill_icon",         m_pkSkillIcon );
    LINK_CONTROL( "static_skill_name",      m_pkStaticSkillName );
    LINK_CONTROL( "static_current_level",   m_pkStaticCurSkillLevel );
    LINK_CONTROL( "wnd_master",             m_pkWndMaster );
    LINK_CONTROL( "wnd_lock",               m_pkWndLock );
    LINK_CONTROL( "btn_plus",               m_pkBtnPlus );
    LINK_CONTROL( "btn_minus",              m_pkBtnMinus );
    LINK_CONTROL( "static_trainable_lv",    m_pkStaticOpenLv );
    LINK_CONTROL( "hover_frame",            m_pkHoverFrame );
}

KGCCharSkillBox::~KGCCharSkillBox(void)
{
}

void KGCCharSkillBox::OnCreate( void )
{
    m_pkSkillIcon->InitState( true );

    m_pkStaticSkillName->InitState( true );

    m_pkStaticCurSkillLevel->InitState( true );
    m_pkStaticCurSkillLevel->SetAlign( DT_CENTER );

    m_pkWndMaster->InitState( false );
    m_pkWndLock->InitState( false );

    m_pkBtnPlus->InitState( false, true, this );
    m_pkBtnMinus->InitState( false, true, this );

    m_pkStaticOpenLv->InitState( true );
    m_pkStaticOpenLv->SetFontColor( D3DCOLOR_ARGB( 255, 52, 78, 109 ) );
    m_pkStaticOpenLv->SetAlign( DT_CENTER );

    m_pkHoverFrame->ToggleRender( false );
}

void KGCCharSkillBox::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkBtnPlus && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) {
        const GCSkill* pSkill = SiKGCSkillTree()->GetSkill( m_eSkillID );
        if( pSkill ) {
            if( pSkill->GetLearnSP() <= SiKGCSkillTree()->GetCurrentSP( SiKGCSkillTree()->GetCurUICharType() ) ) {
                SpeakToActionListener( KActionEvent( this, static_cast< DWORD >( m_eSkillID ), ECLICK_TRAIN_BTN ) );            
            } else {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NOMORE_SKILL_POINT ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            }
        } 
        return;
    }

    if( event.m_pWnd == m_pkBtnMinus && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) {
        if( SiKGCSkillTree()->IsExistSkillUntrainItem().first == false  ) {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_SKILL_UNTRAIN_CONDITION_ERROR2 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        } else {
            const GCSkill* pSkill = SiKGCSkillTree()->GetSkill( m_eSkillID );

            if( pSkill ) {
                const SkillSet& setNextID = pSkill->GetNextSet();
                for( std::set< EGCSkillTree >::const_iterator sit = setNextID.begin(); sit != setNextID.end(); ++sit ) {
                    const GCSkill* pNextSkill = SiGCSKT()->GetSkill( *sit );
                    if( pNextSkill && pNextSkill->m_bOrCheckPreList && SiGCSKT()->IsLearnedSkill( *sit ) ) {
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_SKILL_UNTRAIN_CONDITION_ERROR1 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
                        return;
                    }
                }
            }            

            SpeakToActionListener( KActionEvent( this, static_cast< DWORD >( m_eSkillID ), ECLICK_UNTRAIN_BTN ) );
        }        
        return;
    }
}

void KGCCharSkillBox::SetCharSkillInfo( EGCSkillTree eSkillID_ )
{
    const GCSkill* pSkill = SiKGCSkillTree()->GetSkill( eSkillID_ );

    if( pSkill == NULL )
        return;

    m_eSkillID = eSkillID_;
    m_pkSkillIcon->SetTexNameUseMassFile( GCUTIL_STR::GCStrCharToWide( pSkill->GetTextureFileName().c_str() ) );
    m_pkStaticSkillName->SetText( SiKGCSkillTree()->GetString( pSkill->GetTitleID() ) );
    SetBoxState( pSkill );
    
}

void KGCCharSkillBox::SetBoxState( const GCSkill* pSkill_ )
{   
    if( pSkill_ == NULL ) {
        m_pkStaticCurSkillLevel->SetText( L"(0/0)" );
        return;
    }

    // 스킬을 배울 수 있는 상태라면
    if( SiKGCSkillTree()->IsSkillTrainable( pSkill_->GetID() ) ) {        
        m_pkWndLock->ToggleRender( false );
        m_pkStaticOpenLv->SetText( L"" );
        m_pkStaticSkillName->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

        // 스킬 레벨이 있는 스킬과 없는 스킬을 구분해서 출력한다.
        if( pSkill_->m_iCurSkillLv == -1 ) {
            SetMasterState( false );
            m_pkStaticCurSkillLevel->ToggleRender( false );
        } else {
            SetMasterState( false );

            // 처음 레벨일 때 -버튼 처리
            if( pSkill_->m_iCurSkillLv == 1 ) {
                if( !SiKGCSkillTree()->IsLearnedSkill( pSkill_->GetID() ) ) {
                    m_pkBtnMinus->Lock( true );
                } else {
                    m_pkBtnMinus->Lock( false );
                }
            } else {
                m_pkBtnMinus->Lock( false );
            }

            std::wstringstream wstr;
            if( !SiKGCSkillTree()->IsLearnedSkill( pSkill_->GetID() ) ) {
                wstr << "(" << pSkill_->m_iCurSkillLv - 1 << "/" << pSkill_->m_iMaxSkillLv << ")";
            }

            m_pkStaticCurSkillLevel->ToggleRender( true );
            m_pkStaticCurSkillLevel->SetText( wstr.str() );
        }
    } else if( SiKGCSkillTree()->IsLearnedSkill( pSkill_->GetID() ) ) {        
        m_pkWndLock->ToggleRender( false );
        m_pkStaticOpenLv->SetText( L"" );
        m_pkStaticSkillName->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
        if( pSkill_->m_iCurSkillLv == -1 ) {
            SetMasterState( true );
            m_pkStaticCurSkillLevel->ToggleRender( false );
        } else {
            // 현재 스킬레벨이 맥스레벨이고 익힌 상태라면 Master를 출력한다.
            if( SiKGCSkillTree()->IsLearnedSkill( pSkill_->GetID() ) && pSkill_->m_iCurSkillLv == pSkill_->m_iMaxSkillLv ) {
                SetMasterState( true );
                m_pkStaticCurSkillLevel->ToggleRender( false );
            }
        }
    } else {
        // 스킬 배울 수 없는 상태 일때 셋팅
        m_pkStaticSkillName->SetFontColor( D3DCOLOR_ARGB( 255, 52, 78, 109 ) );
        m_pkStaticCurSkillLevel->ToggleRender( false );
        m_pkWndMaster->ToggleRender( false );
        m_pkBtnPlus->ToggleRender( false );
        m_pkBtnMinus->ToggleRender( false );
        m_pkWndLock->ToggleRender( true );
        WCHAR strTemp[MAX_PATH];
        swprintf( strTemp, MAX_PATH, L"LV.%d", pSkill_->GetLv() );
        m_pkStaticOpenLv->SetText( strTemp );
    }
}

void KGCCharSkillBox::SetMasterState( bool bMaster_ )
{
    m_pkWndMaster->ToggleRender( bMaster_ );
    m_pkBtnPlus->ToggleRender( true );
    m_pkBtnPlus->Lock( bMaster_ );
    m_pkBtnMinus->ToggleRender( true );
    m_pkBtnMinus->Lock( !bMaster_ );    
}

void KGCCharSkillBox::EmptyBoxState()
{
    m_pkSkillIcon->SetTexNameUseMassFile( L"Special75.dds" );;
    m_pkStaticSkillName->SetText( L"" );
    m_pkStaticCurSkillLevel->SetText( L"" );
    m_pkWndMaster->ToggleRender( false );
    m_pkWndLock->ToggleRender( false );
    m_pkBtnPlus->ToggleRender( false );
    m_pkBtnMinus->ToggleRender( false );
    m_pkStaticOpenLv->SetText( L"" );
}

void KGCCharSkillBox::RenderHoverFrame( bool bRender_ )
{
    m_pkHoverFrame->ToggleRender( bRender_ );
}


