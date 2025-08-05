#include "stdafx.h"
#include "gcui/KGCSkillSelectWnd.h"
#include "../MyD3D.h"

// 스킬 선택 아이콘 UI 창
IMPLEMENT_CLASSNAME( KGCSkillSelectWnd );
IMPLEMENT_WND_FACTORY( KGCSkillSelectWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCSkillSelectWnd, "gc_skill_select_wnd" );

KGCSkillSelectWnd::KGCSkillSelectWnd(void)
: m_pkScrollBar( NULL )
{
    LINK_CONTROL( "scroll",         m_pkScrollBar );
}

KGCSkillSelectWnd::~KGCSkillSelectWnd(void)
{
}

void KGCSkillSelectWnd::OnCreate( void )
{
    m_pkScrollBar->InitState( true, true, this );
    CreateUI();
    UpdateSkillIcons();
}

void KGCSkillSelectWnd::CreateUI()
{
    if( m_vecSkillIcon.empty() ){
        std::vector< KD3DWnd* > vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName( this, "ui_skill_tree_main_dlg.stg", "",
            "skill_tree_main_dlg\\skill_select_wnd\\skill_select_icon",true, ( int )EMAX_SKILL_ICON );

        for( std::vector< KD3DWnd* >::iterator vit = vecTemp.begin(); vit != vecTemp.end(); ++vit ) {
            m_vecSkillIcon.push_back( static_cast< KGCSkillSelectIcon* >(*vit) );
        }

        int iRow = 0;
        int iLine = 0;
        for( std::vector< KGCSkillSelectIcon* >::iterator vit = m_vecSkillIcon.begin(); vit != m_vecSkillIcon.end(); ++vit ) {

            (*vit)->SetWindowPos( D3DXVECTOR2( static_cast< float >( EICON_START_X + ( iRow * EICON_OFFSET_X ) ), 
                static_cast< float >( EICON_START_Y + ( iLine * EICON_OFFSET_Y ) ) ) );
            (*vit)->InitState(true,true,this);
            iRow++;
            if( iRow == EICON_ROW_COUNT ) {
                iRow = 0;
                iLine++;
            }
        }
    }
}

void KGCSkillSelectWnd::ActionPerformed( const KActionEvent& event )
{
    GCWND_MAP( m_pkScrollBar, OnScroll );    

    for( int i = 0; i < static_cast< int >( m_vecSkillIcon.size() ); i++ ) {
        if( event.m_pWnd == m_vecSkillIcon[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK && m_vecSkillIcon[i]->GetSkillID() != -1) {            
            this->ToggleRender( false );
            SpeakToActionListener( KActionEvent( this, static_cast< DWORD >( m_vecSkillIcon[i]->GetSkillID() ), ECLICK_SKILL_ICON ) );            
            return;
        }
    }
}

void KGCSkillSelectWnd::PostChildDraw( void )
{

}

void KGCSkillSelectWnd::FrameMoveInEnabledState( void )
{
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x , (float)pt.y );	

    if( this->CheckPosInWindowBound(vMousePos) ) {
        if( g_pkInput->ISWheelUp() ) {
            if( m_pkScrollBar->GetScrollPos() >= 0 ) {
                m_pkScrollBar->SetScrollPos( m_pkScrollBar->GetScrollPos() );
                OnScroll();
                g_KDSound.Play( "73" );
            }
        } else if( g_pkInput->ISWheelDown() ) {
            if( m_pkScrollBar->GetScrollPos() <= m_pkScrollBar->GetScrollRangeMax() ) {
                m_pkScrollBar->SetScrollPos( m_pkScrollBar->GetScrollPos() );
                OnScroll();
                g_KDSound.Play( "73" );
            }
        }	
    }

    for( int i = 0; i < static_cast< int >( m_vecSkillIcon.size() ); i++ ) {
        if( m_vecSkillIcon[i]->CheckPosInWindowBound( vMousePos ) && m_vecSkillIcon[i]->GetSkillID() != -1 ) {
            m_vecSkillIcon[i]->RenderSelectBack( true );
        } else {
            m_vecSkillIcon[i]->RenderSelectBack( false );
        }
    }
}

void KGCSkillSelectWnd::OnScroll()
{
    int iRealindex = m_pkScrollBar->GetScrollPos();
    int iMaxListView = GetMaxLineCount() - iRealindex;

    if( iMaxListView >= EICON_LINE_COUNT) {
        iMaxListView = EICON_LINE_COUNT;
    }

    for(int i = 0; i < iMaxListView; ++i ) {
        int iIndex = ( i + iRealindex ) * EICON_ROW_COUNT;
        int iStartRowIndex = i * EICON_ROW_COUNT;
        if( 0 <= iIndex && iIndex < static_cast< int >( m_vecSkillList.size() ) ) {		
            for( int j = 0; j < EICON_ROW_COUNT; j++ ) {
                m_vecSkillIcon[ iStartRowIndex + j ]->ToggleRender( true );
                if( static_cast<int>( m_vecSkillIcon.size() ) > iIndex + j ) {
                    m_vecSkillIcon[ iStartRowIndex + j ]->SetSkillIcon( m_vecSkillList[ iIndex + j ] );
                }
                else {
                    m_vecSkillIcon[ iStartRowIndex + j ]->InitSkillIcon();
                }
            }            
        }
    }

    for( int i = static_cast< int >( m_vecSkillList.size() ); i < static_cast< int >( m_vecSkillIcon.size() ); ++i ) {
        m_vecSkillIcon[i]->InitSkillIcon();
    }
}

void KGCSkillSelectWnd::UpdateSkillIcons()
{
    for( int i = 0; i < static_cast< int >( m_vecSkillIcon.size() ); i++ ) {
        m_vecSkillIcon[i]->InitSkillIcon();
    }


    std::vector< EGCSkillTree > vecSkillList;
    m_vecSkillList.clear();
    vecSkillList = SiKGCSkillTree()->GetSkillsPerClass( SiKGCSkillTree()->GetCurUICharType(), SiKGCSkillTree()->GetCurUIPromotion() );
    
    for( int i = 0; i < static_cast< int >( vecSkillList.size() ); i++ ) {
        // 스킬 아이콘 개수 넘어서면 빠져 나오자.
        if( i >= static_cast< int >( m_vecSkillIcon.size() ) ) {
            break;
        }

        // 배우지 않은 스킬은 담을 필요 없다.
        if( SiKGCSkillTree()->IsLearnedSkill( vecSkillList[i] ) == false ) {
            continue;
        }

        const GCSkill* pSkill = SiKGCSkillTree()->GetSkill( vecSkillList[i] );

        if( pSkill && pSkill->m_pHotKeySkill ) {
            m_vecSkillIcon[ i ]->SetSkillIcon( vecSkillList[i] );
            m_vecSkillList.push_back( vecSkillList[i] );
        }        
    }

    int iSkillCount = GetMaxLineCount();
    int iScrollMax = 0;
    int iCount =  iSkillCount - EICON_LINE_COUNT;
    if( iCount > 0 ) {
        iScrollMax = iCount;
    }

    m_pkScrollBar->InitScroll( 0, 1, 0, iScrollMax + 1, 2, 0, 1, true );
    OnScroll();
}

int KGCSkillSelectWnd::GetMaxLineCount()
{
    int iSkillCount = static_cast<int>( m_vecSkillList.size() / EICON_ROW_COUNT );
    if( m_vecSkillList.size() > EICON_ROW_COUNT && m_vecSkillList.size() % EICON_ROW_COUNT != 0 ) {
        iSkillCount += 1;
    }
    return iSkillCount;
}


// 스킬 선택 아이콘 UI 아이콘
IMPLEMENT_CLASSNAME( KGCSkillSelectIcon );
IMPLEMENT_WND_FACTORY( KGCSkillSelectIcon );
IMPLEMENT_WND_FACTORY_NAME( KGCSkillSelectIcon, "gc_skill_select_icon" );

KGCSkillSelectIcon::KGCSkillSelectIcon(void)
: m_pkSkillIcon( NULL )
, m_pkWndSpecial( NULL )
, m_pkBack( NULL )
, m_eSkillID( ( EGCSkillTree )-1 )
{
    LINK_CONTROL( "img_skill_icon",     m_pkSkillIcon );
    LINK_CONTROL( "type_special",       m_pkWndSpecial );
    LINK_CONTROL( "back",               m_pkBack );
}

KGCSkillSelectIcon::~KGCSkillSelectIcon(void)
{
}

void KGCSkillSelectIcon::OnCreate( void )
{
    m_pkSkillIcon->InitState( true );
    m_pkWndSpecial->InitState( false );
    m_pkBack->InitState( false );
}

void KGCSkillSelectIcon::SetSkillIcon( EGCSkillTree eSkillID_ )
{
    m_eSkillID = eSkillID_;

    const GCSkill* pSkill = SiKGCSkillTree()->GetSkill( eSkillID_ );

    m_pkWndSpecial->ToggleRender( false );

    if( pSkill == NULL ) {
        m_pkSkillIcon->SetTexNameUseMassFile( L"Special75.dds" );
    } else {
        m_pkSkillIcon->SetTexNameUseMassFile( GCUTIL_STR::GCStrCharToWide( pSkill->GetTextureFileName().c_str() ) );
        if( pSkill->m_pHotKeySkill && pSkill->m_pHotKeySkill->m_bSpecialSkill ) {
            m_pkWndSpecial->ToggleRender( true );
        } else {
            m_pkWndSpecial->ToggleRender( false );
        }
    }

    m_pkSkillIcon->ToggleRender( true );
}

void KGCSkillSelectIcon::InitSkillIcon()
{
    m_pkSkillIcon->SetTexNameUseMassFile( L"Special75.dds" );
    m_pkWndSpecial->ToggleRender( false );
    m_eSkillID = ( EGCSkillTree )-1;
}

void KGCSkillSelectIcon::RenderSelectBack( bool bRender_ )
{
    m_pkBack->ToggleRender( bRender_ );
}


