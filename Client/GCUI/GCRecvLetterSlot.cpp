#include "stdafx.h"
#include "GCRecvLetterSlot.h"

IMPLEMENT_CLASSNAME( KGCRecvLetterSlot );
IMPLEMENT_WND_FACTORY( KGCRecvLetterSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCRecvLetterSlot, "gc_recvletterslot" );

#define READ_LETTER_COLOR D3DCOLOR_ARGB( 255, 122, 158, 180 )
#define NON_READ_LETTER_COLOR D3DCOLOR_ARGB( 255,255,255,255 )

KGCRecvLetterSlot::KGCRecvLetterSlot( void )
{
    m_pkFrame = NULL;
    m_pkActiveFrame = NULL;
    m_pkItemImg = NULL;
    LINK_CONTROL( "frame",       m_pkFrame );
    LINK_CONTROL( "activeFrame", m_pkActiveFrame );
    LINK_CONTROL( "itemImg",     m_pkItemImg );

    m_pkIconBeforeRead = NULL;
    m_pkIconAfterRead = NULL;
    m_pkIconSystem = NULL;
    m_pkIconNormal = NULL;
    LINK_CONTROL( "iconBeforeRead", m_pkIconBeforeRead );
    LINK_CONTROL( "iconAfterRead",  m_pkIconAfterRead );
    LINK_CONTROL( "iconSystem",     m_pkIconSystem );
    LINK_CONTROL( "iconNormal",     m_pkIconNormal );

    m_pkBtnCheckDefault = NULL;
    m_pkBtnCheckActive = NULL;
    LINK_CONTROL( "btnCheckDefault", m_pkBtnCheckDefault );
    LINK_CONTROL( "btnCheckActive",  m_pkBtnCheckActive );

    m_pkStaticDate = NULL;
    m_pkStaticTitle = NULL;
    m_pkStaticGoods = NULL;
    m_pkStaticRemain = NULL;
    LINK_CONTROL( "staticDate",     m_pkStaticDate );
    LINK_CONTROL( "staticTitle",    m_pkStaticTitle );
    LINK_CONTROL( "staticGoods",    m_pkStaticGoods );
    LINK_CONTROL( "staticRemain",   m_pkStaticRemain );
}

KGCRecvLetterSlot::~KGCRecvLetterSlot( void )
{
}

void KGCRecvLetterSlot::OnCreate( void )
{
    m_pkFrame->InitState( true, true, this );
    m_pkActiveFrame->InitState( false );
    m_pkItemImg->InitState( true );

    m_pkIconBeforeRead->InitState( false );
    m_pkIconAfterRead->InitState( false );
    
    // 우편 종류 관련 일단 빼둡니다. 
    m_pkIconSystem->InitState( false );
    m_pkIconNormal->InitState( false );

    m_pkBtnCheckDefault->InitState( true, true, this );
    m_pkBtnCheckDefault->DisconnectAllEventProcedure();
    m_pkBtnCheckDefault->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRecvLetterSlot::OnClickCheck );
    m_pkBtnCheckActive->InitState( false, true, this );
    m_pkBtnCheckActive->DisconnectAllEventProcedure();
    m_pkBtnCheckActive->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRecvLetterSlot::OnClickCheck );

    m_pkStaticDate->InitState( true );
    m_pkStaticTitle->InitState( true );
    m_pkStaticGoods->InitState( true );
    m_pkStaticRemain->InitState( true );
    m_pkStaticRemain->SetAlign( DT_RIGHT );
}

void KGCRecvLetterSlot::ActionPerformed( const KActionEvent& event )
{
    if( m_LetterInfo.m_PostUID == 0 ) { 
        return;
    }

    if( event.m_pWnd == m_pkFrame && event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER ) { 
        m_pkActiveFrame->ToggleRender( true );
    }

    if( event.m_pWnd == m_pkFrame && event.m_dwCode == KD3DWnd::D3DWE_CURSOR_LEAVE ) { 
        m_pkActiveFrame->ToggleRender( false );
    }

    if( event.m_pWnd == m_pkFrame && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) { 
        OnClickSlot();
    }
}

void KGCRecvLetterSlot::OnClickCheck( void )
{
    if( m_LetterInfo.m_PostUID == 0 ) { 
        return;
    }

    m_pkBtnCheckDefault->ToggleRender( !m_pkBtnCheckDefault->IsRenderOn() );
    m_pkBtnCheckActive->ToggleRender( !m_pkBtnCheckActive->IsRenderOn() );

    // 전체 선택 항태에서는 하나라도 체크 풀면 전체 선택도 풀리도록 
    if( m_pkBtnCheckDefault->IsRenderOn() && g_pkUIScene->m_pkPostLetterDlg->IsSelectAllLetter() ) { 
        g_pkUIScene->m_pkPostLetterDlg->ToggleSelectAll( false );
    }
}

void KGCRecvLetterSlot::OnClickSlot( void )
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::D3DWE_BUTTON_CLICK ) );
}

bool KGCRecvLetterSlot::IsCheck( void )
{
    return m_pkBtnCheckActive->IsRenderOn();
}

void KGCRecvLetterSlot::SetCheck( bool bCheck_ )
{
    m_pkBtnCheckActive->ToggleRender( bCheck_ );
    m_pkBtnCheckDefault->ToggleRender( !bCheck_ );
}
void KGCRecvLetterSlot::SetReadMail( bool bRead )
{
    m_bRead =  bRead;
}

bool KGCRecvLetterSlot::GetReadMail()
{
    return m_bRead;
}
void KGCRecvLetterSlot::SetLetterInfo( KPostItemTitleInfo& Info_ )
{
    m_LetterInfo = Info_;

    
    SetReadMail( m_LetterInfo.m_bRead );
    // 아이템 정보 
    if( m_LetterInfo.m_vecItemID.empty() ) {    // 첨부 아이템 없음 
        if( m_LetterInfo.m_bRead ) { 
            m_pkIconAfterRead->ToggleRender( true );
            m_pkIconBeforeRead->ToggleRender( false );
        }
        else {
            m_pkIconAfterRead->ToggleRender( true );
            m_pkIconBeforeRead->ToggleRender( true );
        }
        m_pkStaticGoods->SetText( g_pkStrLoader->GetReplacedString( STR_ID_LETTER_GOODS, "l", std::wstring(L"") ) );
    }
    else {                                      // 첨부 아이템 있음 
        std::vector< GCITEMID >::iterator it = m_LetterInfo.m_vecItemID.begin();
        m_pkItemImg->SetItemInfo( *it / 10 );
        m_pkItemImg->ShowItem( true );

        GCItem* pItem = g_pItemMgr->GetItemData( *it / 10 );
        if( NULL != pItem ) { 
            m_pkStaticGoods->SetForceWordWrapText( g_pkStrLoader->GetReplacedString( STR_ID_LETTER_GOODS, "l", pItem->strItemName ), true );
        }
        else { 
            m_pkStaticGoods->SetForceWordWrapText( g_pkStrLoader->GetReplacedString( STR_ID_LETTER_GOODS, "l", std::wstring(L"") ), true );
        }
    }

    //// 시스템 / 일반 메일 구분  - 우편 종류 관련 일단 빼둡니다. 
    //if( m_LetterInfo.m_cScriptType == KPostItemInfo::LT_SYSTEM ) { 
    //    m_pkIconSystem->ToggleRender( true );
    //    m_pkIconNormal->ToggleRender( false );
    //}
    //else { 
    //    m_pkIconSystem->ToggleRender( false );
    //    m_pkIconNormal->ToggleRender( true );
    //}

    // 날짜 / 제목 / 첨부 물품 
    std::wostringstream stmDate;
    struct tm* timeInfo = localtime( &m_LetterInfo.m_tmRegDate );
    stmDate << timeInfo->tm_year + 1900 << L"." << timeInfo->tm_mon + 1 << L"." << timeInfo->tm_mday << L" ";   // 날짜 
    stmDate << m_LetterInfo.m_strFromNickName;  // 보낸 사람 
    
    m_pkStaticDate->SetForceWordWrapText( stmDate.str(), true );
    m_pkStaticTitle->SetForceWordWrapText( m_LetterInfo.m_strTitle, true );

    // 읽었으면 글자색 바꿔줌 
    if( m_LetterInfo.m_bRead ) { 
        m_pkStaticDate->SetFontColor( READ_LETTER_COLOR );
        m_pkStaticTitle->SetFontColor( READ_LETTER_COLOR );
        m_pkStaticGoods->SetFontColor( READ_LETTER_COLOR );
    }
	else
	{
		m_pkStaticDate->SetFontColor( NON_READ_LETTER_COLOR );
		m_pkStaticTitle->SetFontColor( NON_READ_LETTER_COLOR );
		m_pkStaticGoods->SetFontColor( NON_READ_LETTER_COLOR );

	}
}

void KGCRecvLetterSlot::Clear( void )
{
    m_LetterInfo.SetData( KPostItemInfo() );

    m_pkActiveFrame->ToggleRender( false );
    m_pkItemImg->SetItemInfo( KItem::INVALID_ITEM );

    m_pkIconBeforeRead->ToggleRender( false );
    m_pkIconAfterRead->ToggleRender( false );
    m_pkIconSystem->ToggleRender( false );
    m_pkIconNormal->ToggleRender( false );

    m_pkBtnCheckActive->ToggleRender( false );
    m_pkBtnCheckDefault->ToggleRender( true );

    m_pkStaticDate->SetText( L"" );
    m_pkStaticTitle->SetText( L"" );
    m_pkStaticGoods->SetText( L"" );
    m_pkStaticRemain->SetText( L"" );
}

bool KGCRecvLetterSlot::IsWithItem( void )
{
    return ( !m_LetterInfo.m_vecItemID.empty() );
}
