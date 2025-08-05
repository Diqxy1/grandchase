#include "stdafx.h"
#include "gcui/KGCAwakeningSkillDlg.h"
#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCAwakeningSkillDlg );
IMPLEMENT_WND_FACTORY( KGCAwakeningSkillDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCAwakeningSkillDlg, "gc_awakening_skill_dlg" );

KGCAwakeningSkillDlg::KGCAwakeningSkillDlg(void)
: m_pkCheckBoxLock( NULL )
, m_pkBtnClose( NULL )
, m_pkBtnOpen( NULL )
, m_pkScroll( NULL )
, m_bCloseWnd( false )
, m_bAnimation( false )
, m_bFirstOpen( true )
{

    char str[MAX_PATH] = {0,};
    for( int i = 0; i < EMAX_AWAKENING_BOX_LINE; ++i )
    {
        m_pkAwakeningSkillBox[i] = NULL;
        sprintf( str, "awakening_skill_box_%d", i );  
        LINK_CONTROL( str, m_pkAwakeningSkillBox[i] ); 
    }

    LINK_CONTROL( "btn_close",          m_pkBtnClose );
    LINK_CONTROL( "btn_open",           m_pkBtnOpen );
    LINK_CONTROL( "checkbox_lock",      m_pkCheckBoxLock );
    LINK_CONTROL( "scroll",             m_pkScroll );

    m_vStartWndPos = D3DXVECTOR2( 620.f, 10.f );
    m_vEndWndPos = D3DXVECTOR2( 391.f, 10.f );
    m_vAnimWndPos = D3DXVECTOR2( 0.f, 0.f );
    
}

KGCAwakeningSkillDlg::~KGCAwakeningSkillDlg(void)
{
}

void KGCAwakeningSkillDlg::OnCreate( void )
{
    m_bAnimation = false;
    m_pkCheckBoxLock->InitState( true, true, this );
    if( m_bFirstOpen ) {
        m_pkCheckBoxLock->SetCheck( false );
        m_bFirstOpen = false;
    }    

    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnOpen->InitState( false, true, this );

    m_pkScroll->InitState( true, true, this );
    m_pkScroll->InitScroll( 0, 1, 0, 1, 2, 0, 1, true );

    for( int i = 0; i < EMAX_AWAKENING_BOX_LINE; i++ ) {
        m_pkAwakeningSkillBox[i]->InitState( true, true, this );
    }

    if( m_bCloseWnd ) {
        this->SetWindowPosDirect( m_vEndWndPos );
    } else {
        this->SetWindowPosDirect( m_vStartWndPos );
    }
}

void KGCAwakeningSkillDlg::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnClose,			KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
    _GCWND_MSG_MAP( m_pkBtnOpen,			KD3DWnd::D3DWE_BUTTON_CLICK, OnOpen );
    _GCWND_MSG_MAP( m_pkCheckBoxLock,		KD3DWnd::D3DWE_BUTTON_CLICK, OnCheck );
    
    GCWND_MAP( m_pkScroll, OnScroll );   

    for( int i = 0; i < EMAX_AWAKENING_BOX_LINE; i++ ) {
        if( event.m_pWnd == m_pkAwakeningSkillBox[i] && event.m_dwlParam == KGCAwakeningSkillBox::ECLICK_TRAINNIG_BTN ) {
            SiKGCSkillTree()->Server_TrainSkill( static_cast< int >( event.m_dwCode ) );
            SiKGCSkillTree()->EquipSkill( static_cast< EGCSkillTree >( event.m_dwCode ) );
            SiKGCSkillTree()->SaveTrainSkill( static_cast< EGCSkillTree >( event.m_dwCode ) );
            OnScroll();
        }
    }
}


void KGCAwakeningSkillDlg::FrameMoveInEnabledState( void )
{
    if( m_bAnimation ) {
        if( m_bCloseWnd ) {
            m_vAnimWndPos.x -= 10.f;
            if( m_vAnimWndPos.x <= m_vEndWndPos.x ) {
                m_vAnimWndPos.x = m_vEndWndPos.x;
                m_bAnimation = false;
                m_pkBtnClose->InitState( false, true, this );
                m_pkBtnOpen->InitState( true, true, this );
                m_pkScroll->Lock( true );
            }

        } else {
            m_vAnimWndPos.x += 10.f;
            if( m_vAnimWndPos.x >= m_vStartWndPos.x ) {
                m_vAnimWndPos.x = m_vStartWndPos.x;
                m_bAnimation = false;
                m_pkBtnClose->InitState( true, true, this );
                m_pkBtnOpen->InitState( false, true, this );
                m_pkScroll->Lock( false );
            }
        }
        this->SetWindowPosDirect( m_vAnimWndPos );
    }

    // 창이 닫혀있고 애니메이션이 있을 때는 스크롤 묵어버리자.
    if( m_bCloseWnd == false || m_bAnimation ) {
        true;
    }

    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x , (float)pt.y );	

    if( this->CheckPosInWindowBound(vMousePos) ) {
        if( g_pkInput->ISWheelUp() ) {
            if( m_pkScroll->GetScrollPos() >= 0 ) {
                m_pkScroll->SetScrollPos( m_pkScroll->GetScrollPos() );
                OnScroll();
                g_KDSound.Play( "73" );
            }
        } else if( g_pkInput->ISWheelDown() ) {
            if( m_pkScroll->GetScrollPos() <= m_pkScroll->GetScrollRangeMax() ) {
                m_pkScroll->SetScrollPos( m_pkScroll->GetScrollPos() );
                OnScroll();
                g_KDSound.Play( "73" );
            }
        }	
    }
}

void KGCAwakeningSkillDlg::PostChildDraw( void )
{
}

void KGCAwakeningSkillDlg::OnClose()
{
    if( m_pkCheckBoxLock->GetCheck() ) {
        return;
    }

    m_bCloseWnd = true;
    m_vAnimWndPos = m_vStartWndPos;
    m_bAnimation = true;
}

void KGCAwakeningSkillDlg::OnOpen()
{
    m_bCloseWnd = false;
    m_vAnimWndPos = m_vEndWndPos;
    m_bAnimation = true;
}


void KGCAwakeningSkillDlg::OnCheck()
{
    m_pkCheckBoxLock->SetCheck( !m_pkCheckBoxLock->GetCheck() );    
}

void KGCAwakeningSkillDlg::OnScroll()
{
    if( m_vecAwakeningSkill.empty() ) {
        return;
    }

    int iRealindex = m_pkScroll->GetScrollPos();
    int iMaxListView = static_cast<int>( m_vecAwakeningSkill.size() ) - iRealindex;

    if( iMaxListView >= EMAX_AWAKENING_BOX_LINE) {
        iMaxListView = EMAX_AWAKENING_BOX_LINE;
    }

    for(int i = 0; i < iMaxListView; ++i ) {
        int iIndex = i + iRealindex;

        if( 0 <= iIndex && iIndex < static_cast<int>( m_vecAwakeningSkill.size() ) ) {		
            m_pkAwakeningSkillBox[i]->ToggleRender( true );
            if( static_cast<int>( m_vecAwakeningSkill.size() ) > iIndex) {
                m_pkAwakeningSkillBox[i]->SetAwakeningSkill( m_vecAwakeningSkill[iIndex] );
            }
            else {
                m_pkAwakeningSkillBox[i]->InitAwakeningSkillBox();
            }
        }
    }

    for(int i = iMaxListView; i < EMAX_AWAKENING_BOX_LINE; ++i)
    {
        m_pkAwakeningSkillBox[i]->InitAwakeningSkillBox();
    }
}

void KGCAwakeningSkillDlg::UpdateAwakeningSkillDlg( std::vector< EGCSkillTree >& vecSkillID_ )
{
    m_vecAwakeningSkill = vecSkillID_;

    int iFileCount = static_cast<int>( m_vecAwakeningSkill.size() );
    int iScrollMax = 0;
    int iCount =  iFileCount - EMAX_AWAKENING_BOX_LINE;
    if( iCount > 0 ) {
        iScrollMax = iCount;
    }

    m_pkScroll->InitScroll( 0, 1, 0, iScrollMax + 1, 2, 0, 1, true );
    OnScroll();

    if( static_cast<int>( m_vecAwakeningSkill.size() ) == EMAX_AWAKENING_BOX_LINE ) {
        m_pkScroll->ToggleRender( false );
    }
}
