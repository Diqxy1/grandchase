#include "stdafx.h"
#include "GCJumpingCharDlg.h"

IMPLEMENT_CLASSNAME( KGCJumpingCharDlg );
IMPLEMENT_WND_FACTORY( KGCJumpingCharDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCJumpingCharDlg, "gc_jumpingchardlg" );

KGCJumpingCharDlg::KGCJumpingCharDlg( void )
{
    m_pkBtnClose = NULL;
    m_pkBtnOk = NULL;
    m_pkBtnItem = NULL;
    m_pkCharFocus = NULL;
    m_cSelectCharType = -1;

    m_pkCharIconRect = NULL;
    

    LINK_CONTROL( "btn_ok",     m_pkBtnOk );
    LINK_CONTROL( "btn_close",  m_pkBtnClose );
    LINK_CONTROL( "btn_item",   m_pkBtnItem );
    LINK_CONTROL( "char_select_focus", m_pkCharFocus );
    LINK_CONTROL( "char_icon_rect", m_pkCharIconRect );

    char str[MAX_PATH] = {0,};
    for ( int i = 0; i < NUM_STATIC_DESC; ++i ) 
    {
        m_pkStaticDesc[i] = NULL;
        
        sprintf( str, "static_desc%d", i );          
        LINK_CONTROL( str,  m_pkStaticDesc[i] );
    }

    for (int i=0 ; i<GC_CHAR_NUM ; i++ )
    {
        m_pkCharIcon[i] = NULL;
        sprintf( str, "char%d", i );
        LINK_CONTROL( str, m_pkCharIcon[i] );
    }
}

KGCJumpingCharDlg::~KGCJumpingCharDlg( void )
{
}

void KGCJumpingCharDlg::OnCreate( void )
{
    m_pkBtnOk->InitState( true, true, this );
    m_pkBtnOk->Lock( false );
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnItem->InitState( true, true, this );
    m_pkBtnItem->Lock( false );
    m_pkCharIconRect->InitState( false );
    m_pkCharFocus->InitState( false );

    for ( int i = 0; i < NUM_STATIC_DESC; ++i ) 
    {
        m_pkStaticDesc[i]->InitState( true, true, this );
    }

    for (int i=0 ; i<GC_CHAR_NUM ; i++ )
    {
        m_pkCharIcon[i]->InitState( true, true, this );
        
    }

    int nJumpingLevel = SiKGCJumpingCharManager()->GetJumpingLevel();
    
    m_pkStaticDesc[0]->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_JUMPING_CHAR_DESC0 ) );
    m_pkStaticDesc[1]->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_JUMPING_CHAR_DESC1 ) );
    m_pkStaticDesc[2]->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_JUMPING_CHAR_DESC2 ) );

    InitCharIconPos();
}

void KGCJumpingCharDlg::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnOk, KD3DWnd::D3DWE_BUTTON_CLICK, OnOK );
    GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
    GCWND_MSG_MAP( m_pkBtnItem, KD3DWnd::D3DWE_BUTTON_CLICK, OnItem );


    for (int i=0 ; i<GC_CHAR_NUM ; i++)
    {
        if (m_pkCharIcon[i] == event.m_pWnd && event.m_dwCode == D3DWE_BUTTON_CLICK )
        {
            OnCharSelect( i );
        }
    }
}

void KGCJumpingCharDlg::InitCharIconPos()
{
    for (int i=0 ; i<GC_CHAR_NUM ; i++)
    {
        m_pkCharIcon[i]->ToggleRender(false);
    }

    DWORD dwIconStandardWidth = m_pkCharIcon[0]->GetWidth();
    DWORD dwIconStandardHeight = m_pkCharIcon[0]->GetHeight();

    DWORD dwIconDrawRecWidth = m_pkCharIconRect->GetWidth();

    std::set<char> setCharList = SiKGCJumpingCharManager()->GetCharList();

    int nColumn =(dwIconDrawRecWidth+ICON_GAP) / (dwIconStandardWidth+ICON_GAP) -1;

    D3DXVECTOR2 vStartPos = m_pkCharIconRect->GetFixedWindowLocalPos();


    int cntColumn = 0;
    int cntRow = 0;
    
    
    std::set<char>::iterator iter = setCharList.begin();
    std::set<char>::iterator iter_end = setCharList.end();
    for( ; iter != iter_end; ++iter)
    {
        int iCharIndex = static_cast<int>(*iter);
        if (m_pkCharIcon[iCharIndex])
        {
            float fPosX = vStartPos.x + (cntColumn * (dwIconStandardWidth + ICON_GAP));
            float fPosY = vStartPos.y + (cntRow * (dwIconStandardHeight + ICON_GAP));

            m_pkCharIcon[iCharIndex]->SetWindowPosDirect(D3DXVECTOR2(fPosX,fPosY));
            m_pkCharIcon[iCharIndex]->ToggleRender(true);

            if (cntColumn < nColumn)
            {
                cntColumn++;
            }
            else
            {
                cntRow++;
                cntColumn = 0;
            }
        }
    }
}

void KGCJumpingCharDlg::OnCharSelect( int nIndex )
{
    m_cSelectCharType = nIndex;

    D3DXVECTOR2 vFocusingPos = m_pkCharIcon[nIndex]->GetFixedWindowLocalPos();
    vFocusingPos.x -= (m_pkCharFocus->GetWidth() - m_pkCharIcon[nIndex]->GetWidth())/2;
    vFocusingPos.y -= (m_pkCharFocus->GetHeight() - m_pkCharIcon[nIndex]->GetHeight())/2;

    m_pkCharFocus->SetFixedWindowLocalPos(vFocusingPos);
    m_pkCharFocus->ToggleRender(true);
}

void KGCJumpingCharDlg::OnOK()
{
    if ( m_cSelectCharType < 0 || m_cSelectCharType >= GC_CHAR_NUM )
        return;

    if ( !SiKGCJumpingCharManager()->CheckEnableJumping( m_cSelectCharType ) )
        return;

    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_JUMPING_CHAR_QUESTION ),
        L"", KGCUIScene::GC_MBOX_USE_JUMPING_CHAR , m_cSelectCharType, 0, false, true );
}

void KGCJumpingCharDlg::OnClose()
{
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCJumpingCharDlg::OnItem()
{
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_JUMPING_CHAR_REWARD_QUESTION ),
        L"", KGCUIScene::GC_MBOX_USE_JUMPING_CHAR_REWARD , 0, 0, false, true );
}

void KGCJumpingCharDlg::SetButtonLock( bool bLock )
{
    m_pkBtnOk->Lock( bLock );
    m_pkBtnItem->Lock( bLock );
}