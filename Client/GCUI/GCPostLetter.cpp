#include "stdafx.h"
#include "GCPostLetter.h"

#include "KGCPostLetterManager.h"
#include "GCRecvLetterSlot.h"
#include "GCRecvLetterUI.h"

IMPLEMENT_CLASSNAME( KGCPostLetter );
IMPLEMENT_WND_FACTORY( KGCPostLetter );
IMPLEMENT_WND_FACTORY_NAME( KGCPostLetter, "gc_postletter" );

#define TEXT_COLOR D3DCOLOR_ARGB( 255, 213, 210, 175 )
#define TEXT_OUTLINE_COLOR D3DCOLOR_ARGB( 255, 12, 29, 47 )


KGCPostLetter::KGCPostLetter( void )
{
    m_pkWndReceive = NULL;
    m_pkWndSend = NULL;
    m_pkBtnClose = NULL;
    LINK_CONTROL( "wndReceive", m_pkWndReceive );
    LINK_CONTROL( "wndSend",    m_pkWndSend );
    LINK_CONTROL( "btnClose",   m_pkBtnClose );

    m_pkBtnSelectAllDefault = NULL;
    m_pkBtnSelectAllActive = NULL;
    LINK_CONTROL( "btnAllDefault",    m_pkBtnSelectAllDefault );
    LINK_CONTROL( "btnAllActive",     m_pkBtnSelectAllActive );

    m_pkBtnDelete = NULL;
    m_pkBtnDeleteAll = NULL;
    LINK_CONTROL( "btnDelete",     m_pkBtnDelete );
    LINK_CONTROL( "btnDeleteAll",  m_pkBtnDeleteAll );

    m_pkBtnPrev = NULL;
    m_pkBtnNext = NULL;
    LINK_CONTROL( "btnPrev", m_pkBtnPrev );
    LINK_CONTROL( "btnNext", m_pkBtnNext );

    m_pkStaticSystem = NULL;
    m_pkStaticNormal = NULL;
    m_pkStaticSelectAll = NULL;
    m_pkStaticCapacity = NULL;
    m_pkStaticPage = NULL;
    LINK_CONTROL( "staticSystem",   m_pkStaticSystem );
    LINK_CONTROL( "staticNormal",   m_pkStaticNormal );
    LINK_CONTROL( "staticCapacity", m_pkStaticCapacity );
    LINK_CONTROL( "staticAll",      m_pkStaticSelectAll );
    LINK_CONTROL( "staticPage",     m_pkStaticPage );

    m_pkRecvUI = NULL;
    LINK_CONTROL( "recvLetterUI", m_pkRecvUI );

    for( int i=0 ; i<NUM_LETTER_TYPE ; ++i ) { 
        m_apkBtnLetterTypeDefault[i] = NULL;
        m_apkBtnLetterTypeActive[i] = NULL;
    }
    LINK_CONTROL( "btnSystemDefault", m_apkBtnLetterTypeDefault[ LETTER_SYSTEM ] );
    LINK_CONTROL( "btnNormalDefault", m_apkBtnLetterTypeDefault[ LETTER_NORMAL ] );
    LINK_CONTROL( "btnSystemActive",  m_apkBtnLetterTypeActive[ LETTER_SYSTEM ] );
    LINK_CONTROL( "btnNormalActive",  m_apkBtnLetterTypeActive[ LETTER_NORMAL ] );

    for( int i=0 ; i<NUM_TAB ; ++i ) { 
        m_apkTabDefault[i] = NULL;
        m_apkTabActive[i] = NULL;
    }
    LINK_CONTROL( "tabRecvDefault", m_apkTabDefault[ TAB_RECV ] );
    LINK_CONTROL( "tabSendDefault", m_apkTabDefault[ TAB_SEND ] );
    LINK_CONTROL( "tabRecvActive",  m_apkTabActive[ TAB_RECV ] );
    LINK_CONTROL( "tabSendActive",  m_apkTabActive[ TAB_SEND ] );

    for( int i=0 ; i<NUM_RECV_SLOT ; ++i ) { 
        char szTemp[ 128 ] = { 0, };
        sprintf_s( szTemp, 127, "recvSlot%d", i );

        m_apkRecvSlot[i] = NULL;
        LINK_CONTROL( szTemp, m_apkRecvSlot[i] );
    }

    m_iCurTab = TAB_RECV;
    m_cLetterType = KEVENT_GET_POST_LETTER_LIST_REQ::LL_ALL;

    m_iCurLetterCount = 0;
    m_iTotalLetterCount = 0;
    m_iCurPage = 1;
    m_iTotalPage = 0;
}

KGCPostLetter::~KGCPostLetter( void )
{
}

void KGCPostLetter::OnCreate( void )
{
    m_pkWndReceive->InitState( true );
    m_pkWndSend->InitState( false );

    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->DisconnectAllEventProcedure();
    m_pkBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPostLetter::OnClickClose );
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );

    m_pkBtnSelectAllDefault->InitState( true, true, this );
    m_pkBtnSelectAllDefault->DisconnectAllEventProcedure();
    m_pkBtnSelectAllDefault->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPostLetter::OnClickSelectAll );
    m_pkBtnSelectAllActive->InitState( false, true, this );
    m_pkBtnSelectAllActive->DisconnectAllEventProcedure();
    m_pkBtnSelectAllActive->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPostLetter::OnClickSelectAll );

    m_pkBtnDelete->InitState( true, true, this );
    m_pkBtnDelete->DisconnectAllEventProcedure();
    m_pkBtnDelete->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPostLetter::OnClickDelete );
    
    m_pkBtnDeleteAll->InitState( false, true, this );
    m_pkBtnDeleteAll->DisconnectAllEventProcedure();
    m_pkBtnDeleteAll->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPostLetter::OnClickDelete );

    m_pkBtnPrev->InitState( true, true, this );
    m_pkBtnPrev->DisconnectAllEventProcedure();
    m_pkBtnPrev->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPostLetter::OnClickPrev );
    m_pkBtnNext->InitState( true, true, this );
    m_pkBtnNext->DisconnectAllEventProcedure();
    m_pkBtnNext->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPostLetter::OnClickNext );

    // 우? 종류 구분 일단 빼 둡니다. 
    //m_pkStaticSystem->InitState( true );
    //m_pkStaticNormal->InitState( true );
    //m_pkStaticSystem->SetText( g_pkStrLoader->GetString( STR_ID_SYSTEM_POST_LETTER ) );
    //m_pkStaticNormal->SetText( g_pkStrLoader->GetString( STR_ID_NORMAL_POST_LETTER ) );
    m_pkStaticSystem->InitState( false );
    m_pkStaticNormal->InitState( false );

    m_pkStaticSelectAll->InitState( true );
    m_pkStaticCapacity->InitState( true );
    m_pkStaticPage->InitState( true );

#if defined( NATION_PHILIPPINE )
    m_pkStaticCapacity->SetAlign( DT_CENTER );
    m_pkStaticPage->SetAlign( DT_CENTER );
#endif

    m_pkStaticCapacity->SetFontColor( TEXT_COLOR );
    m_pkStaticCapacity->SetFontOutline( true, TEXT_OUTLINE_COLOR );
    m_pkStaticSelectAll->SetText( g_pkStrLoader->GetString( STR_ID_SELECT_ALL ) );
    m_pkStaticSystem->SetFontOutline( true, TEXT_OUTLINE_COLOR );
    m_pkStaticNormal->SetFontOutline( true, TEXT_OUTLINE_COLOR );
    m_pkStaticSelectAll->SetFontOutline( true, TEXT_OUTLINE_COLOR );

    m_pkRecvUI->InitState( false, true, this );

    // 우편 종류 구분 일단 빼둡니다. 
    //for( int i=0 ; i<NUM_LETTER_TYPE ; ++i ) {
    //    m_apkBtnLetterTypeDefault[i]->InitState( false, true, this );
    //    m_apkBtnLetterTypeDefault[i]->DisconnectAllEventProcedure();
    //    m_apkBtnLetterTypeActive[i]->InitState( true, true, this );
    //    m_apkBtnLetterTypeActive[i]->DisconnectAllEventProcedure();

    //    if( i == LETTER_SYSTEM ) {
    //        m_apkBtnLetterTypeDefault[i]->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPostLetter::OnClickSystemLetter );
    //        m_apkBtnLetterTypeActive[i]->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPostLetter::OnClickSystemLetter );
    //    }

    //    if( i == LETTER_NORMAL ) { 
    //        m_apkBtnLetterTypeDefault[i]->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPostLetter::OnClickNormalLetter );
    //        m_apkBtnLetterTypeActive[i]->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPostLetter::OnClickNormalLetter );
    //    }
    //}
    for( int i=0 ; i<NUM_LETTER_TYPE ; ++i ) { 
        m_apkBtnLetterTypeDefault[i]->InitState( false );
        m_apkBtnLetterTypeActive[i]->InitState( false );
    }

    // 우편 보내기 탭 관련 일단 빼둡니다. 
    //for( int i=0 ; i<NUM_TAB ; ++i ) { 
    //    m_apkTabDefault[i]->InitState( true, true, this );
    //    m_apkTabActive[i]->InitState( true, true, this );
    //}
    //OnClickTab( TAB_RECV );
    m_apkTabActive[ TAB_RECV ]->InitState( true );

    for( int i=0 ; i<NUM_RECV_SLOT ; ++i ) {
        m_apkRecvSlot[i]->InitState( true, true, this );
    }

    m_iCurPage = 1;
}

bool KGCPostLetter::InitDialog( IN const KDialogInfo& kInfo_ )
{
    m_cLetterType = KEVENT_GET_POST_LETTER_LIST_REQ::LL_ALL;
    SiKGCPostLetterManager()->GetPostLetterPageInfoReq( m_iCurPage, m_cLetterType );
    SetCapacityAndPage();
    SetRecvLetterSlot();
    return true;
}

void KGCPostLetter::ActionPerformed( const KActionEvent& event )
{
    // 우편 보내기 탭 관련 일단 빼둡니다. 
    //for( int i=0 ; i<NUM_TAB ; ++i ) { 
    //    if( event.m_pWnd == m_apkTabDefault[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) { 
    //        OnClickTab( i );
    //        break;
    //    }
    //}

    for( int i=0 ; i<NUM_RECV_SLOT ; ++i ) { 
        if( event.m_pWnd == m_apkRecvSlot[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) { 
            OnClickRecvSlot( i );
            break;
        }
    }
}

void KGCPostLetter::OnClickClose( void )
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCPostLetter::OnClickTab( int iTab_ )
{
    if( iTab_ < TAB_RECV || iTab_ > TAB_SEND ) { 
        return;
    }

    for( int i=0 ; i<NUM_TAB ; ++i ) { 
        m_apkTabDefault[i]->ToggleRender( true );
        m_apkTabActive[i]->ToggleRender( false );
    }

    m_apkTabActive[ iTab_ ]->ToggleRender( true );
    m_apkTabDefault[ iTab_ ]->ToggleRender( false );
    m_iCurTab = iTab_;
}

void KGCPostLetter::SetLetterType( void )
{
    if( m_apkBtnLetterTypeActive[ LETTER_SYSTEM ]->IsRenderOn() && 
        m_apkBtnLetterTypeActive[ LETTER_NORMAL ]->IsRenderOn() ) { 
            m_cLetterType = KEVENT_GET_POST_LETTER_LIST_REQ::LL_ALL;
    }
    else if( m_apkBtnLetterTypeActive[ LETTER_SYSTEM ]->IsRenderOn() ) { 
        m_cLetterType = KEVENT_GET_POST_LETTER_LIST_REQ::LL_SYSTEM;
    }
    else if( m_apkBtnLetterTypeActive[ LETTER_NORMAL ]->IsRenderOn() ) { 
        m_cLetterType = KEVENT_GET_POST_LETTER_LIST_REQ::LL_USER;
    }
}

void KGCPostLetter::OnClickSystemLetter( void )
{
    if( !m_apkBtnLetterTypeActive[ LETTER_NORMAL ]->IsRenderOn() && m_apkBtnLetterTypeActive[ LETTER_SYSTEM ]->IsRenderOn() ) { 
        return;
    }

    m_apkBtnLetterTypeDefault[ LETTER_SYSTEM ]->ToggleRender( !m_apkBtnLetterTypeDefault[ LETTER_SYSTEM ]->IsRenderOn() );
    m_apkBtnLetterTypeActive[ LETTER_SYSTEM ]->ToggleRender( !m_apkBtnLetterTypeActive[ LETTER_SYSTEM ]->IsRenderOn() );

    SetLetterType();
    GetLetterPageInfo();

    if( m_pkBtnSelectAllActive->IsRenderOn() ) { 
        OnClickSelectAll();
    }
}

void KGCPostLetter::OnClickNormalLetter( void )
{
    if( !m_apkBtnLetterTypeActive[ LETTER_SYSTEM ]->IsRenderOn() && m_apkBtnLetterTypeActive[ LETTER_NORMAL ]->IsRenderOn() ) { 
        return;
    }

    m_apkBtnLetterTypeDefault[ LETTER_NORMAL ]->ToggleRender( !m_apkBtnLetterTypeDefault[ LETTER_NORMAL ]->IsRenderOn() );
    m_apkBtnLetterTypeActive[ LETTER_NORMAL ]->ToggleRender( !m_apkBtnLetterTypeActive[ LETTER_NORMAL ]->IsRenderOn() );

    SetLetterType();
    GetLetterPageInfo();

    if( m_pkBtnSelectAllActive->IsRenderOn() ) { 
        OnClickSelectAll();
    }
}

void KGCPostLetter::OnClickRecvSlot( int iIndex_ )
{
    if( iIndex_ < 0 || iIndex_ >= NUM_RECV_SLOT ) { 
        return;
    }

    if( m_apkRecvSlot[ iIndex_ ]->GetPostUID() == 0 ) { 
        return;
    }

    ShowRecvLetterUI( m_apkRecvSlot[ iIndex_ ]->GetPostUID() );


    KPostItemTitleInfo Letterlnfo;
    if( SiKGCPostLetterManager()->GetPageLetterInfo( m_iCurPage, iIndex_, Letterlnfo ) ) { 
        Letterlnfo.m_bRead = true;
        SiKGCPostLetterManager()->SetPageLetterInfo( m_iCurPage, iIndex_, Letterlnfo );
        m_apkRecvSlot[ iIndex_ ]->SetLetterInfo( Letterlnfo );
    }
}

void KGCPostLetter::SetCapacityAndPage( void )
{
    m_iCurLetterCount = SiKGCPostLetterManager()->GetTotalLetterCount();
    m_iTotalLetterCount = NUM_TOTAL_LETTER;
    m_iCurPage = SiKGCPostLetterManager()->GetCurPageNum();
    m_iTotalPage = SiKGCPostLetterManager()->GetTotalPageNum();

    std::wostringstream stmCapacity;
    std::wostringstream stmPage;

    stmCapacity << m_iCurLetterCount << L"/" << m_iTotalLetterCount;
    stmPage << m_iCurPage << L" / " << m_iTotalPage;

    m_pkStaticCapacity->SetText( stmCapacity.str() );
    m_pkStaticPage->SetText( stmPage.str() );

    // 총 편지수가 0개면 1/1 페이지로 표시해 주세영
    if( 0 == m_iCurLetterCount ) { 
        m_pkStaticPage->SetText( L"1 / 1" );
    }
}

void KGCPostLetter::GetLetterPageInfo( void )
{
    SiKGCPostLetterManager()->GetPostLetterPageInfoReq( m_iCurPage, m_cLetterType );
    SetCapacityAndPage();
    SetRecvLetterSlot();
}

void KGCPostLetter::SetRecvLetterSlot( void )
{
    m_pkBtnSelectAllActive->ToggleRender( false );
    m_pkBtnSelectAllDefault->ToggleRender( true );

    m_pkBtnDelete->ToggleRender( true );
    m_pkBtnDeleteAll->ToggleRender( false );

    for( int i=0 ; i<NUM_RECV_SLOT ; ++i ) { 
        m_apkRecvSlot[i]->Clear();

        KPostItemTitleInfo Letterlnfo;
        if( SiKGCPostLetterManager()->GetPageLetterInfo( m_iCurPage, i, Letterlnfo ) ) { 
            if( IsNowLetterType( Letterlnfo.m_cScriptType ) )  {
                m_apkRecvSlot[i]->SetLetterInfo( Letterlnfo );
            }
        }
    }
}

void KGCPostLetter::ShowRecvLetterUI( POSTUID postUID_ )
{
    m_pkRecvUI->SetLetterInfo( postUID_ );;
    m_pkRecvUI->ToggleRender( true );
}

void KGCPostLetter::OnClickSelectAll( void )
{
    m_pkBtnSelectAllActive->ToggleRender( !m_pkBtnSelectAllActive->IsRenderOn() );
    m_pkBtnSelectAllDefault->ToggleRender( !m_pkBtnSelectAllDefault->IsRenderOn() );

    if( m_pkBtnSelectAllActive->IsRenderOn() ) { 
        for( int i=0 ; i<NUM_RECV_SLOT ; ++i ) { 
            if( m_apkRecvSlot[i]->GetPostUID() != 0 ) { 
                m_apkRecvSlot[i]->SetCheck( true );
            }
        }

        m_pkBtnDelete->ToggleRender( false );
        m_pkBtnDeleteAll->ToggleRender( true );
    }
    else {
        for( int i=0 ; i<NUM_RECV_SLOT ; ++i ) { 
            m_apkRecvSlot[i]->SetCheck( false );
        }

        m_pkBtnDelete->ToggleRender( true );
        m_pkBtnDeleteAll->ToggleRender( false );
    }
}

void KGCPostLetter::OnClickPrev( void )
{
    if( m_iCurPage > 1 ) { 
        --m_iCurPage;
        GetLetterPageInfo();
    }
}

void KGCPostLetter::OnClickNext( void )
{
    if( m_iCurPage < m_iTotalPage ) { 
        ++m_iCurPage;
        GetLetterPageInfo();
    }
}

void KGCPostLetter::SetItemRecvInfo( IN const POSTUID postUID_ )
{
    SiKGCPostLetterManager()->ClearItemInfo( m_iCurPage, postUID_ );    
    SetRecvLetterSlot();

    m_pkRecvUI->Clear();
    m_pkRecvUI->SetLetterInfo( postUID_);
}

void KGCPostLetter::OnClickDelete( void )
{
    int  iCount = 0;
    bool bWithItem = false;
    for( int i=0 ; i<NUM_RECV_SLOT ; ++i ) { 
        if( m_apkRecvSlot[i]->IsCheck() ) { //슬롯이 체크 상태일때, 이 해당 슬롯에 있는 우편이 안읽은 편지인지 체크해본다.
            ++iCount;
            
            if( m_apkRecvSlot[i]->IsWithItem() ) { 
                bWithItem = true;
            }
        }
    }

    if( 0 == iCount ) {     // 체크된거 없음
        return;
    }

    if( false == bWithItem ) {  // 아이템 없는 편지만 있음
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_DELETE_LETTER_NO_ITEM ), L"", 
            KGCUIScene::GC_MBOX_USE_DELETE_LETTER, 0, 0, true, true );
    }
    else {                      // 아이템 있는 편지도 있음 
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_DELETE_LETTER_WITH_ITEM ), L"", 
            KGCUIScene::GC_MBOX_USE_DELETE_LETTER, 0, 0, true, true );
    }
}

bool KGCPostLetter::IsNowLetterType( char cType_ )
{
    if( cType_ == KPostItemInfo::LT_USER || cType_ == KPostItemInfo::LT_SYSTEM ) { 
        if( m_cLetterType == KEVENT_GET_POST_LETTER_LIST_REQ::LL_ALL ) { 
            return true;
        }
        else if( cType_ == m_cLetterType ) {
            return true;
        }
        else { 
            return false;
        }
    }

    return false;
}

void KGCPostLetter::RefreshPageInfo( void )
{
    SiKGCPostLetterManager()->RefreshPostLetterPageInfo( m_iCurPage, m_cLetterType );
    SetRecvLetterSlot();
}

void KGCPostLetter::DeleteLetter( void )
{
    std::vector< POSTUID > vecDeleteList;
    for( int i=0 ; i<NUM_RECV_SLOT ; ++i ) { 
        if( m_apkRecvSlot[ i ]->IsCheck() ) { 
            vecDeleteList.push_back( m_apkRecvSlot[ i ]->GetPostUID() );
            if(!m_apkRecvSlot[i]->GetReadMail())
            {
                //안 읽은 편지 수가 1개 이상일 때, 한개씩 깎는다. 
                if( SiKGCPostLetterManager()->GetTotalNotReadLetterCount() > 0 )
                    SiKGCPostLetterManager()->SetTotalNotReadLetterCount(SiKGCPostLetterManager()->GetTotalNotReadLetterCount() -1 );
            }
            if(!m_apkRecvSlot[i]->GetReadMail())
            {
                //안 읽은 편지 수가 1개 이상일 때, 한개씩 깎는다. 
                if( SiKGCPostLetterManager()->GetTotalNotReadLetterCount() > 0 )
                    SiKGCPostLetterManager()->SetTotalNotReadLetterCount(SiKGCPostLetterManager()->GetTotalNotReadLetterCount() -1 );
            }
            // 지우려는 우편이 열려 있으면 닫아 줍니다. 
            if( m_pkRecvUI->IsRenderOn() ) { 
                if( m_apkRecvSlot[i]->GetPostUID() == m_pkRecvUI->GetPostUID() ) {
                    m_pkRecvUI->OnClickClose();
                }
            }
        }
    }

    if( vecDeleteList.empty() ) { 
        return;
    }

    Result_PostLetter = INT_MAX;
    KP2P::GetInstance()->Send_DeleteLetterReq( vecDeleteList );
    g_MyD3D->WaitForServerAck( Result_PostLetter, INT_MAX, 1000, TIME_OUT_VALUE );

    if( 0 == Result_PostLetter ) { 
        RefreshPageInfo();
    }

    // 전체 선택 상태라면 풀어준다 
    if( m_pkBtnSelectAllActive->IsRenderOn() ) { 
        OnClickSelectAll();
    }
}

void KGCPostLetter::DeleteRecvUILetter( void )
{
    m_pkRecvUI->OnDelete();
}

void KGCPostLetter::ToggleSelectAll( bool bToggle_ )
{
    m_pkBtnSelectAllActive->ToggleRender( false );
    m_pkBtnSelectAllDefault->ToggleRender( true );

    if( bToggle_ ) { 
        OnClickSelectAll();
    }
    else { 
        m_pkBtnDelete->ToggleRender( true );
        m_pkBtnDeleteAll->ToggleRender( false );
    }
}

void KGCPostLetter::ClearLetterInfo( void )
{
    m_iCurTab = TAB_RECV;
    m_cLetterType = KEVENT_GET_POST_LETTER_LIST_REQ::LL_ALL;

    m_iCurLetterCount = 0;
    m_iTotalLetterCount = 0;
    m_iCurPage = 1;
    m_iTotalPage = 0;
}