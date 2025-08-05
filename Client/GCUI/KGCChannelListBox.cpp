#include "stdafx.h"
//
#include "KGCChannelListBox.h"

#include "KGCChannelBar.h"
//
#include "MyD3D.h"
#include "GCStateMachine.h"
#include "KGCChannelManager.h"
//

IMPLEMENT_CLASSNAME( KGCChannelListBox );
IMPLEMENT_WND_FACTORY( KGCChannelListBox );
IMPLEMENT_WND_FACTORY_NAME( KGCChannelListBox, "gc_channel_list_box" );

KGCChannelListBox::KGCChannelListBox( void )
{
    for( int i = 0; i < NUM_CHANNEL; ++i )
        m_apkChannelBar[i]  = NULL;
    m_pkMainFrame           = NULL;
    m_pkCloseButton         = NULL;

    char strWndName[MAX_PATH] = {0,};
    for( int i = 0; i < NUM_CHANNEL; ++i )
    {
        sprintf( strWndName, "channel_bar%02d", i );
        LINK_CONTROL( strWndName, m_apkChannelBar[i] );
    }

    LINK_CONTROL( "mainframe",      m_pkMainFrame );
    LINK_CONTROL( "close_button",   m_pkCloseButton );

    m_vBasePos = D3DXVECTOR2( 8, 29 );

    m_dwSelectChannelUID = 0xFFFFFFFF;
}

KGCChannelListBox::~KGCChannelListBox( void )
{
}

void KGCChannelListBox::OnCreate( void )
{
    for( int i = 0; i < NUM_CHANNEL; ++i )
        m_apkChannelBar[i]->InitState( false, true, this );

    m_pkMainFrame->InitState( true );
    m_pkCloseButton->InitState( true, true, this );

    m_pkMainFrame->SetAllWindowColor( 0xCCFFFFFF );

    m_pkCloseButton->SetHotKey( DIK_ESCAPE );

    m_dwSelectChannelUID = 0xFFFFFFFF;

	SetChannelListBox();
}

void KGCChannelListBox::FrameMoveInEnabledState( void )
{
    // 매 프레임 부르지만 일정시간이 지나야 패킷을 보낸다( 딜레이가 있음 )
    KP2P::GetInstance()->Send_ChannelInfoListReq();
}

void KGCChannelListBox::ActionPerformed( const KActionEvent& event )
{
    switch ( event.m_dwCode )
    {
        case KD3DWnd::D3DWE_CURSOR_ENTER: g_KDSound.Play( "30" ); return;
        case KD3DWnd::D3DWE_BUTTON_DOWN:  g_KDSound.Play( "31" ); return;
    }

    GCWND_MSG_MAP( m_pkCloseButton, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );

    if( event.m_dwCode != KD3DWnd::D3DWE_BUTTON_CLICK )
        return;

    for( int i = 0; i < NUM_CHANNEL; ++i )
    {
        if( event.m_pWnd == m_apkChannelBar[i] )
        {
            m_dwSelectChannelUID = m_apkChannelBar[i]->GetChannelUID();
            m_strSelectChannelName = m_apkChannelBar[i]->GetChannelName();

            OnClose();
            return;
        }
    }
}

void KGCChannelListBox::OnClose()
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCChannelListBox::OnDestroyComplete()
{
    GoChannel();

    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );
}

void KGCChannelListBox::SetChannelListBox()
{
    // 대전 채널 리스트를 얻어온다
    std::vector<KChannelInfo>& vecChannelInfo = SiKGCChannelManager()->GetMatchChannelList();

    int iChannelSize = (int)vecChannelInfo.size();

    if( iChannelSize > NUM_CHANNEL )
        iChannelSize = NUM_CHANNEL;
	// 나머지 얘들은 보이지 않게 한다!
	for( int i = 0; i < NUM_CHANNEL; ++i )
	{
		m_apkChannelBar[i]->ToggleRender( false );
		D3DXVECTOR2 vWindowPos = m_vBasePos;
		vWindowPos.y += ( CHANNEL_BAR_HEIGHT + 2 ) * i;
		m_apkChannelBar[i]->SetAllWindowPos( vWindowPos );
	}

    // 채널들을 세팅해 준다
    for( int i = 0; i < iChannelSize; ++i )
    {
        D3DCOLOR kChannelNameColor = 0xFFFFFFFF;
        if( SiKGCChannelManager()->GetCurrentChannel() == vecChannelInfo[i].m_dwChannelUID )
            kChannelNameColor = 0xFFFFFF00; // 현재 입장된 상태의 채널이름은 노란색으로 표시한다

		//여기서 체크해주다니-_-;; 일단 복사때린다..
		//여기 수정 일어나면 InviteManager.cpp쪽도 수정해 주시오.
        bool bEnableChannel = false;
        int iUserGrade = 0;
        EChannelType eChannelType = CT_UNLIMITED;
        
        //길드전 서버일 경우 등급을 그에 맞게 받아온다.
        if ( g_kGlobalValue.GetServerInfo()->CheckServerType( ST_GUILD ) ) {
            iUserGrade = static_cast<int> (g_kGlobalValue.m_kGuildUserInfo.m_ChannelGrade);
            eChannelType = (EChannelType)iUserGrade;
        }
        else {
            iUserGrade = static_cast<int>(g_kGlobalValue.m_kUserInfo.GetUserGrade());
            eChannelType = g_MyD3D->GradeToChannelType( iUserGrade );
        }        
        
        if( (EChannelType)vecChannelInfo[i].m_cType == eChannelType || // 자기 계급이거나..
            (EChannelType)vecChannelInfo[i].m_cType == CT_UNLIMITED   // 자유 채널이면..
            ) 
        {
            bEnableChannel = true; // 들어갈 수 있는 채널임
        }
        
        bool bEnableGuildUnlimited = false;
        //길드전 서버에서 자기 채널에 맞는 등급이거나, 자유 채널일 경우 들어갈 수 있는 채널이라고 체크를 해준다.
        if ( g_kGlobalValue.GetServerInfo()->CheckServerType( ST_GUILD ) && 
            ( (EChannelType)vecChannelInfo[i].m_cType == eChannelType || (EChannelType)vecChannelInfo[i].m_cType == CT_GUILD_UNLIMITED )
            )
        {
            bEnableChannel = true;            
        }
        
        if (g_kGlobalValue.m_bIsObserverMode)
        {
            bEnableChannel = true;
        }

        // 내가 들어갈 수 없는 채널이라면 회색으로 표시 해주는게 좋겠지..
        if( !bEnableChannel )
            kChannelNameColor = 0xFF888888;

        m_apkChannelBar[i]->ToggleRender( true );
        m_apkChannelBar[i]->SetSelfInputCheck( bEnableChannel );
        m_apkChannelBar[i]->SetChannelInfo( vecChannelInfo[i], kChannelNameColor );

    }


    // 박스의 크기를 조절해 준다
    int iHeight = ( CHANNEL_BAR_HEIGHT + 2 ) * iChannelSize + 15;
    m_pkMainFrame->SetSize( m_pkMainFrame->GetWidth(), iHeight );

    // 박스의 위치를 적당히 조절한다
    D3DXVECTOR2 vPos;
    vPos.x = CHANNEL_LIST_BOX_POS_X * GC_SCREEN_DIV_WIDTH;
    vPos.y = (float)( ( GC_SCREEN_HEIGHT - ( iHeight + 20 ) ) / 2 );
    SetAllWindowPos( vPos );
}

// 채널로 입장한다.
void KGCChannelListBox::GoChannel()
{
    // 체험계정이면 자유채널 입장이 불가능 하다.        
    if ( m_strSelectChannelName == L"자유마을" && g_kGlobalValue.m_bExpAccount ) //exp_id
    {       
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString(STR_ID_EXPID_NOT_ENTER_FREE_CHANNEL), 
            g_pkStrLoader->GetString(STR_ID_EXPID_IMPOSSIBLE) );
        return;
    }
    
    // 같은 채널을 선택했다면 따로 처리하지 않아도 된다..
    if( SiKGCChannelManager()->GetCurrentChannel() == m_dwSelectChannelUID )
        return;

    // 올바른 채널이 아니면 처리X
    if( SiKGCChannelManager()->GetChannel( m_dwSelectChannelUID ) == NULL )
        return;

    if( g_MyD3D->m_pStateMachine->GoMatch( m_dwSelectChannelUID ) == true )
    {
        StringCchPrintfW( g_Selected_Channel_Name, (int)m_strSelectChannelName.size() + 1, L"%s", m_strSelectChannelName.c_str() );

        g_pkUIScene->RoomButtonSetAutoStart( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) );

    }
}

bool KGCChannelListBox::InitDialog( IN const KDialogInfo& kInfo_ )
{
    this->SetChannelListBox();
    return true;
}