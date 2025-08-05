#include "stdafx.h"
#include "GCGuildMemo.h"
//
//
//

//

//
//
//

IMPLEMENT_CLASSNAME( KGCGuildMemo );
IMPLEMENT_WND_FACTORY( KGCGuildMemo );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildMemo, "gc_guild_memo" );

KGCGuildMemo*   g_pkGuildMemo = NULL;

class KNoticeInfoLess
{
public:
    bool operator()( const KGuildNoticeInfo& p, const KGuildNoticeInfo& q ) const
    { return p.m_strRegDate < q.m_strRegDate; }
};

KGCGuildMemo::KGCGuildMemo( void )
{
    g_pkGuildMemo   = this;
    m_pkMemoList    = NULL;
    m_pkScrollbar   = NULL;
    m_vecNoticeInfo.clear();

    LINK_CONTROL( "guild_memo_list_box", m_pkMemoList );
    LINK_CONTROL( "guild_memo_scroll",   m_pkScrollbar );
}

KGCGuildMemo::~KGCGuildMemo( void )
{
    m_vecNoticeInfo.clear();
    g_pkGuildMemo   = NULL;
}

void KGCGuildMemo::OnCreate( void )
{
    SetSelfInputCheck( true );

    m_pkMemoList->SetSelfInputCheck( false );
    m_pkScrollbar->SetSelfInputCheck( false );

    m_pkMemoList->AddActionListener( this );
    m_pkScrollbar->AddActionListener( this );

    m_pkMemoList->SetLineSpace( 1.2f );
}

void KGCGuildMemo::ActionPerformed( const KActionEvent& event )
{
    if ( event.m_pWnd == m_pkScrollbar )
    {
        int iTop = m_pkScrollbar->GetScrollPos();
        if ( m_pkMemoList->GetTopIndex() != iTop )
        {
            g_KDSound.Play( "73" );
            m_pkMemoList->SetTopIndex( iTop );
        }
    }
}

void KGCGuildMemo::ResetScrollBar( void )
{
    m_pkScrollbar->SetScrollPos( 0 );
    m_pkMemoList->SetTopIndex( 0 );
}

void KGCGuildMemo::AddMemo( const std::wstring& strMemo )
{
    m_pkMemoList->AddString( strMemo );
    m_pkScrollbar->SetScrollPageSize( m_pkMemoList->GetDisplayLimitLine() );
    m_pkScrollbar->SetScrollRangeMax( m_pkMemoList->GetCount() );
    m_pkScrollbar->SetScrollPos( m_pkMemoList->GetTopIndex() );
}

void KGCGuildMemo::MemoGuildNotice( std::map<int, KGuildNoticeInfo>& rmapInfo )
{
    m_vecNoticeInfo.clear();

    for ( int i = 0; i < MAX_NUM_MEMO; ++i )
    {
        m_vecNoticeInfo.push_back( rmapInfo[i] );
    }

    std::sort( m_vecNoticeInfo.begin(), m_vecNoticeInfo.end(), KNoticeInfoLess() );

    m_pkMemoList->Clear();

    int cnt = 1;
		
    for ( int i = 0; i < (int)m_vecNoticeInfo.size(); i++ )
    {
        if ( false == m_vecNoticeInfo[i].m_bValid ) continue;

		std::wstringstream strMemo;
		strMemo << cnt++;
		strMemo << L". " << m_vecNoticeInfo[i].m_strRegDate << L"\n" << m_vecNoticeInfo[i].m_strMsg << L"\n\n";		
        AddMemo( strMemo.str().c_str() );		
    }
}

bool KGCGuildMemo::FilterGuildMemoCommand( const std::wstring& strMessage )
{
    if ( 0 == strMessage.find( g_pkStrLoader->GetString( STR_ID_GUILD_MEMOCMD_NOTICE ) ) ) // "/게시 "
    {
        std::string::size_type cmdSize = g_pkStrLoader->GetString( STR_ID_GUILD_MEMOCMD_NOTICE ).size();

        if ( cmdSize < strMessage.size() ) // 뒷부분에 게시할 메세지가 있는지 확인한다.
        {
			int slot, i;
            for ( slot = 0, i = 0; i < (int)m_vecNoticeInfo.size(); ++i )
            {
                if ( false == m_vecNoticeInfo[i].m_bValid && 0 == slot ) slot = i;
            }

            KGuildNoticeInfo kNoticeInfo;
            kNoticeInfo.m_iMsgId = m_vecNoticeInfo[slot].m_iMsgId;			
			kNoticeInfo.m_strMsg = strMessage.substr( cmdSize );
            kNoticeInfo.m_bValid = true;

            KP2P::GetInstance()->Send_GuildEditNoticeReq( "Server", kNoticeInfo );
        }
        return true;
    }
    else if ( 0 == strMessage.find( g_pkStrLoader->GetString( STR_ID_GUILD_MEMOCMD_DELETE ) ) ) // "/삭제 "
    {
        std::string::size_type cmdSize = g_pkStrLoader->GetString( STR_ID_GUILD_MEMOCMD_DELETE ).size();

        // 0에서 5까지의 숫자만 허용한다.
        if ( cmdSize + 1 == strMessage.size() && strMessage[cmdSize] > '0' && strMessage[cmdSize] < '6' )
        {
            std::vector<KGuildNoticeInfo>::size_type index = strMessage[cmdSize] - '1';
			int cnt = 0, i = 0;
            for ( cnt ; i < (int)m_vecNoticeInfo.size(); ++i )
            {
                if ( true == m_vecNoticeInfo[i].m_bValid )
                {
                    if ( index == cnt++ ) break;
                }
            }

            KGuildNoticeInfo kNoticeInfo;
            kNoticeInfo.m_iMsgId = m_vecNoticeInfo[i].m_iMsgId;
            kNoticeInfo.m_bValid = false;

            KP2P::GetInstance()->Send_GuildEditNoticeReq( "Server", kNoticeInfo );
        }
        return true;
    }
    else if ( 0 == strMessage.find( g_pkStrLoader->GetString( STR_ID_GUILD_MEMOCMD_NOTIFY ) ) ) // "/통보 "
    {
        std::string::size_type cmdSize = g_pkStrLoader->GetString( STR_ID_GUILD_MEMOCMD_NOTIFY ).size();

        if ( cmdSize < strMessage.size() ) // 뒷부분에 통보할 메세지가 있는지 확인한다.
        {
            std::wstring strMsg = strMessage.substr( cmdSize );

			KP2P::GetInstance()->Send_GuildNotifyMsgReq( L"Server", strMsg.c_str() );
        }
        return true;
    }

    return false;
}