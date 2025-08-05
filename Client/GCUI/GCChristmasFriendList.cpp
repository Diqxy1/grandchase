#include "stdafx.h"
#include "GCChristmasFriendList.h"

IMPLEMENT_CLASSNAME( KGCChristmasFriendList );
IMPLEMENT_WND_FACTORY( KGCChristmasFriendList );
IMPLEMENT_WND_FACTORY_NAME( KGCChristmasFriendList, "gc_christmas_friend_list" );

KGCChristmasFriendList::KGCChristmasFriendList( void )
{
    m_pkBtnClose = NULL;
    LINK_CONTROL( "btn_close", m_pkBtnClose );

    m_pkStaticFriendList = NULL;
    LINK_CONTROL( "static_friend_list", m_pkStaticFriendList );

    for( int i=0 ; i<FRIEND_LIST_SIZE ; ++i )
    {
        m_apkStaticFriendsName[i] = NULL;

        char szWndName[100] = {0, };
        sprintf_s( szWndName, 99, "static_friend_name_%d", i );
        LINK_CONTROL( szWndName, m_apkStaticFriendsName[i] );
    }

    for( int i=0 ; i<FRIEND_LIST_SIZE ; ++i )
    {
        m_apkBtnFriendsNameSelected[i] = NULL;

        char szWndName[100] = {0, };
        sprintf_s( szWndName, 99, "btn_selected_back_%d", i );
        LINK_CONTROL( szWndName, m_apkBtnFriendsNameSelected[i] );
    }

    m_pkScrollBar = NULL;
    LINK_CONTROL( "scroll_friend_list", m_pkScrollBar );

    m_pkOfflineCheckBox = NULL;
    LINK_CONTROL( "offline_checkbox", m_pkOfflineCheckBox );

    m_pkBtnOfflineCheckBox = NULL;
    LINK_CONTROL( "btn_offline_back", m_pkBtnOfflineCheckBox );

    m_pkBackScroll = NULL;
    LINK_CONTROL( "back_scroll_rect", m_pkBackScroll );

}

KGCChristmasFriendList::~KGCChristmasFriendList( void )
{
}

void KGCChristmasFriendList::OnCreate( void )
{
    m_pkBtnClose->InitState( true, true, this );

    m_pkStaticFriendList->InitState( true );
    m_pkStaticFriendList->SetAlign( DT_CENTER );;
    m_pkStaticFriendList->SetFontOutline( true );
    m_pkStaticFriendList->SetText( g_pkStrLoader->GetString( STR_ID_FRIEND_LIST ) );

    for( int i=0 ; i<FRIEND_LIST_SIZE ; ++i )
    {
        m_apkStaticFriendsName[i]->InitState( false, true, this );
        m_apkStaticFriendsName[i]->SetAlign( DT_CENTER );

        m_apkBtnFriendsNameSelected[i]->InitState( false, true, this );
    }

    m_pkScrollBar->InitState( true, true, this );

#if defined ( USE_POST_LETTER_SYSTEM )
    if ( m_pkOfflineCheckBox != NULL ) {
        m_pkOfflineCheckBox->InitState( true, true, this );
        m_pkOfflineCheckBox->SetText( g_pkStrLoader->GetString( STR_ID_OFFLINE ) );
        m_pkOfflineCheckBox->SetCheck( false );
    }

    if ( m_pkBtnOfflineCheckBox != NULL ) {
        m_pkBtnOfflineCheckBox->InitState( true, true, this );
    }
#endif

    m_iRealIndex = 0;
    m_vecFriendList.clear();

    InitFriendList();
    InitScroll();
    OnScroll();

}

void KGCChristmasFriendList::ActionPerformed( const KActionEvent& event )
{
    GCWND_MAP( m_pkScrollBar, OnScroll );
    _GCWND_MSG_MAP( m_pkBtnClose,          KD3DWnd::D3DWE_BUTTON_CLICK, OnClickClose );

#if defined ( USE_POST_LETTER_SYSTEM )
    _GCWND_MSG_MAP( m_pkBtnOfflineCheckBox,   KD3DWnd::D3DWE_BUTTON_CLICK, OnToggleOfflineList );
#endif

    for( int i = 0; i < FRIEND_LIST_SIZE; ++i )
    {
        if( event.m_pWnd == m_apkStaticFriendsName[i] && event.m_dwCode == D3DWE_BUTTON_CLICK ){
            m_apkBtnFriendsNameSelected[i]->ToggleRender( true );
            wstring strName = m_apkStaticFriendsName[i]->GetText();
            g_pkUIScene->m_pkChristmasGiftDlg->GetSendGiftItemDlg()->GetNameFromFriendList( strName );
        } else {
            m_apkBtnFriendsNameSelected[i]->ToggleRender( false );
        }
    }
}

void KGCChristmasFriendList::OnCreateComplete( void )
{
#if defined ( USE_POST_LETTER_SYSTEM )
    if ( m_pkOfflineCheckBox != NULL ) {
        m_pkOfflineCheckBox->SetText( g_pkStrLoader->GetString( STR_ID_OFFLINE ) );
    }
    InitScroll();
#endif

    OnScroll();
}

void KGCChristmasFriendList::OnClickClose()
{
    this->ToggleRender( false );
}

void KGCChristmasFriendList::FrameMoveInEnabledState( void )
{
    if( m_vecFriendList.empty() )
        return;

    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x , (float)pt.y );	

    if( m_pkBackScroll->CheckPosInWindowBound(vMousePos) )
    {
        if( g_pkInput->ISWheelUp() )
        {
            if( m_pkScrollBar->GetScrollPos() > 0 )
            {
                m_pkScrollBar->SetScrollPos(m_pkScrollBar->GetScrollPos() - 1);
                OnScroll();
                g_KDSound.Play( "73" );
            }
        }
        else if( g_pkInput->ISWheelDown() )
        {
            if( m_pkScrollBar->GetScrollPos() < m_pkScrollBar->GetScrollRangeMax() )
            {
                m_pkScrollBar->SetScrollPos(m_pkScrollBar->GetScrollPos() + 1);
                OnScroll();
                g_KDSound.Play( "73" );
            }
        }	
    }
}

void KGCChristmasFriendList::InitScroll( void )
{
    m_pkScrollBar->SetScrollPos( 0 );
    m_pkScrollBar->SetScrollRangeMin( 0 );
    m_pkScrollBar->SetScrollPageSize( 1 );

    int iFileCount = static_cast<int>( m_vecFriendList.size() );
    int iScrollMax = 0;
    int iCount =  iFileCount - FRIEND_LIST_SIZE;

    if(iCount > 0)
    {
        iScrollMax = iCount;
    }

    m_pkScrollBar->SetScrollRangeMax( iScrollMax + 1 );
}

void KGCChristmasFriendList::OnScroll()
{
    m_iRealIndex = m_pkScrollBar->GetScrollPos();
    int iMaxBuddyView = GetViewCount( m_iRealIndex );

    for(int i = 0; i < iMaxBuddyView; ++i )
    {
        int iIndex = i + m_iRealIndex;

        if( 0 <= iIndex && iIndex < (int)m_vecFriendList.size() )
        {		
            m_apkStaticFriendsName[i]->SetText( m_vecFriendList[iIndex].m_strNickName );
            m_apkStaticFriendsName[i]->ToggleRender( true );
        }
    }

    for(int i = iMaxBuddyView; i < FRIEND_LIST_SIZE; ++i)
    {
        m_apkStaticFriendsName[i]->ToggleRender( false );
    }	
}


void KGCChristmasFriendList::InitFriendList()
{
    m_vecFriendList.clear();

    for( int i = 0; i < FRIEND_LIST_SIZE; ++i )
    {
        m_apkStaticFriendsName[i]->SetText(L"");
    }

    std::vector<DWORD> vecFriendList;
    vecFriendList.clear();
    SiKFriendShip()->GetOnlienFriendList(GCNateOn::FRIEND_MODE, vecFriendList);

#if defined ( USE_POST_LETTER_SYSTEM )
    if ( IsShowOfflineList() ) {    // 우편 있을 경우 오프라인 유저도 출력하게
        SiKFriendShip()->GetOffLineFriendList(GCNateOn::FRIEND_MODE, vecFriendList);
    }
#endif

    //온라인 유저 중에 목록
    for (int i=0;i<(int)vecFriendList.size();i++)
    {
        KInviteFriendUserInfo kInviteFriendUserinfo;
        KBuddy fdata;
        SiKFriendShip()->GetFriendBuddyByUID(vecFriendList[i],fdata);
        kInviteFriendUserinfo.m_cGrade = (char)-1;
        kInviteFriendUserinfo.m_cBestCharGrade = (char)-1;
        kInviteFriendUserinfo.m_dwUserUID = vecFriendList[i];
        kInviteFriendUserinfo.m_strNickName = fdata.GetNickName();
        kInviteFriendUserinfo.m_strNickColor = fdata.GetNickColor();
        kInviteFriendUserinfo.m_strLocation = fdata.GetServerName() +L"/" + fdata.GetLocation();
        kInviteFriendUserinfo.m_bInviteOK = false;
        m_vecFriendList.push_back(kInviteFriendUserinfo);
    }
}

int KGCChristmasFriendList::GetViewCount(int iMemberIndex_)
{
    int iViewCount = static_cast<int>(m_vecFriendList.size() ) - iMemberIndex_;

    if(iViewCount < FRIEND_LIST_SIZE)
        return iViewCount;

    return FRIEND_LIST_SIZE;
}

void KGCChristmasFriendList::OnToggleOfflineList()
{
    m_pkOfflineCheckBox->ToggleCheck();
    InitFriendList();
    InitScroll();
    OnScroll();
}

bool KGCChristmasFriendList::IsShowOfflineList()
{
    return m_pkOfflineCheckBox->GetCheck();
}

void KGCChristmasFriendList::ToggleFriendList()
{
    InitFriendList();
    InitScroll();
    OnScroll();
    ToggleRender( !IsRenderOn() );
}
