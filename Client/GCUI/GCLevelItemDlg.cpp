#include "stdafx.h"
#include "GCLevelItemDlg.h"

IMPLEMENT_CLASSNAME( KGCLevelItemDlg );
IMPLEMENT_WND_FACTORY( KGCLevelItemDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCLevelItemDlg, "gc_level_item_dlg" );

KGCLevelItemDlg::KGCLevelItemDlg( void )
: m_pkStaticMent0( NULL )
, m_pkStaticMent1( NULL )
, m_pkBtnOK( NULL )
{
    LINK_CONTROL( "static_ment0", m_pkStaticMent0 );
    LINK_CONTROL( "static_ment1", m_pkStaticMent1 );
    LINK_CONTROL( "btn_ok",       m_pkBtnOK );

    for( int i=0 ; i<GC_CHAR_NUM ; ++i ) { 
        m_apkIcon[ i ] = NULL;

        char szTemp[ 128 ] = { 0, };
        sprintf_s( szTemp, 127, "icon_char%d", i );
        LINK_CONTROL( szTemp, m_apkIcon[ i ] );
    }
}

KGCLevelItemDlg::~KGCLevelItemDlg( void )
{
}

void KGCLevelItemDlg::OnCreate( void )
{
    m_pkStaticMent0->InitState( true );
    m_pkStaticMent1->InitState( true );
    m_pkStaticMent0->SetAlign( DT_CENTER );
    m_pkStaticMent1->SetAlign( DT_CENTER );
    m_pkStaticMent1->SetMultiLine( true );

    m_pkBtnOK->InitState( true, true, this );
    m_pkBtnOK->DisconnectAllEventProcedure();
    m_pkBtnOK->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCLevelItemDlg::OnClickBtnOK );
    m_pkBtnOK->SetHotKey( DIK_ESCAPE );
}

void KGCLevelItemDlg::ActionPerformed( const KActionEvent& event )
{

}

void KGCLevelItemDlg::OnClickBtnOK( void )
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCLevelItemDlg::ClearLevelItemDlg( void )
{
    m_pkStaticMent0->SetText( L"" );
    m_pkStaticMent1->SetText( L"" );

    for( int i=0 ; i<GC_CHAR_NUM ; ++i ) { 
        m_apkIcon[ i ]->InitState( false );
    }
}

bool KGCLevelItemDlg::SetIconPosition( std::set< char >& setCharIndex_ )
{
    if( setCharIndex_.empty() ) { 
        return false;
    }

    int iRenderCount = 0;
    for( int iLineIndex = 0 ; iLineIndex < MAX_LINE ; ++iLineIndex ) { 
        for( int iIconIndex = 0 ; iIconIndex < MAX_CHAR_IN_LINE ; ++iIconIndex ) { 

            if( setCharIndex_.empty() ) { 
                break;
            }

            std::set< char >::iterator it = setCharIndex_.begin();
            D3DXVECTOR2 dxvIconPos;
            dxvIconPos.x = static_cast< float >( ( START_POSX + ( ICON_OFFSET * ( iRenderCount % MAX_CHAR_IN_LINE ) ) ) );
            dxvIconPos.y = static_cast< float >( ( START_POSY + ( ICON_OFFSET * iLineIndex ) ) );

            int idx = static_cast<int>( *it );
            m_apkIcon[ idx ]->SetFixedWindowLocalPos( dxvIconPos );
            m_apkIcon[ idx ]->ToggleRender( true );
            ++iRenderCount;

            setCharIndex_.erase( it );
        }
    }

    return true;
}

void KGCLevelItemDlg::SetLevelItemInfo( int iLevel_, std::set< char >& setCharIndex_ )
{
    ClearLevelItemDlg();

    if( SetIconPosition( setCharIndex_ ) == false ) { 
       return;
    }

    m_pkStaticMent0->ToggleRender( true );
    m_pkStaticMent1->ToggleRender( true );
    m_pkStaticMent0->SetText( g_pkStrLoader->GetReplacedString( STR_ID_LEVEL_ITEM_ALRAM0, "i", iLevel_ ) );
    m_pkStaticMent1->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_LEVEL_ITEM_ALRAM1 ) );

}
