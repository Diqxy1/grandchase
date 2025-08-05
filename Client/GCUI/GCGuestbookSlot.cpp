#include "stdafx.h"
#include "GCGuestbookSlot.h"

IMPLEMENT_CLASSNAME( KGCGuestbookSlot );
IMPLEMENT_WND_FACTORY( KGCGuestbookSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCGuestbookSlot, "gc_guestbook_slot" );

KGCGuestbookSlot::KGCGuestbookSlot( void )
{
    m_pkStaticNumber = NULL;
    m_pkStaticNickname = NULL;
    m_pkstaticDate = NULL;
    m_pkStaticMent = NULL;
    m_pkBtnDelete = NULL;

    LINK_CONTROL( "staticNumber", m_pkStaticNumber );
    LINK_CONTROL( "staticName",   m_pkStaticNickname );
    LINK_CONTROL( "staticDate",   m_pkstaticDate );
    LINK_CONTROL( "staticMent",   m_pkStaticMent );
    LINK_CONTROL( "btnDelete",    m_pkBtnDelete );

    m_iNum = -1;
    m_dwUserUID = 0;
}

KGCGuestbookSlot::~KGCGuestbookSlot( void )
{
}

void KGCGuestbookSlot::OnCreate( void )
{
    m_pkStaticNumber->InitState( true );
    m_pkStaticNickname->InitState( true );
    m_pkstaticDate->InitState( true );
    m_pkStaticMent->InitState( true );
    m_pkBtnDelete->InitState( true, true, this );

    m_pkstaticDate->SetAlign( DT_RIGHT );
    m_pkStaticMent->SetMultiLine( true );
    m_pkStaticMent->SetLineSpace( 1.0f );
}

void KGCGuestbookSlot::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnDelete,   KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickDelete );    
}

void KGCGuestbookSlot::SetSlot( int iNum_, DWORD dwUserUID_, std::wstring& strNick_, time_t tmDate_, std::wstring& strMent_ )
{
    m_pkStaticNumber->SetForceWordWrapText( g_pkStrLoader->GetReplacedString( STR_ID_AGIT_GUESTBOOK_NUMBER, "i", iNum_+1 ), true );
    m_pkStaticNickname->SetForceWordWrapText( GetColorNickName( strNick_ ), true );
    m_pkstaticDate->SetText( GetFormatDate( tmDate_ ) );
    m_pkStaticMent->SetText( strMent_ );

    m_iNum = iNum_;
    m_dwUserUID = dwUserUID_;
}

void KGCGuestbookSlot::Clear( void )
{
    m_pkStaticNumber->SetText( L"" );
    m_pkStaticNickname->SetText( L"" );
    m_pkstaticDate->SetText( L"" );
    m_pkStaticMent->SetText( L"" );

    m_iNum = -1;
    m_dwUserUID = 0;
}

std::wstring KGCGuestbookSlot::GetColorNickName( std::wstring& strNick_ )
{
    std::wostringstream stmNick;
    stmNick << L"#cFEBA00" << strNick_ << L"#cX";
    return stmNick.str();
}

std::wstring KGCGuestbookSlot::GetFormatDate( time_t tmDate_ )
{
    struct tm* time = localtime( &tmDate_ );
    std::wostringstream stmDate;
    stmDate << time->tm_year + 1900 << L"/" << time->tm_mon + 1 << L"/" << time->tm_mday << L" ";

    int iHour = time->tm_hour;
    if( iHour < 12 )
        stmDate << g_pkStrLoader->GetString( STR_ID_AGIT_GUESTBOOK_TIME_AM ) << L" " << iHour << L":" << time->tm_min;
    else
        stmDate << g_pkStrLoader->GetString( STR_ID_AGIT_GUESTBOOK_TIME_PM ) << L" " << iHour - 12 << L":" << time->tm_min;

    return stmDate.str();
}

void KGCGuestbookSlot::OnClickDelete( void )
{
    // 빈칸
    if( m_iNum == -1 )
        return;


    // 주인이거나 내가 쓴 것만 지울 수 있다. 
    if( SiKGCAgitGameManager()->GetAgitUID() != g_kGlobalValue.m_kUserInfo.dwUID )
    {
        if( m_dwUserUID != g_kGlobalValue.m_kUserInfo.dwUID )
            return;
    }

    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_DELETE_GUESTBOOK_MSG ), L"",
        KGCUIScene::GC_MBOX_USE_DELETE_GUESTBOOK_MSG, m_iNum, 0, true, true );
}

