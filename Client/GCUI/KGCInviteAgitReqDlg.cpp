#include "stdafx.h"
#include ".\KGCInviteAgitReqDlg.h"

IMPLEMENT_CLASSNAME( KGCInviteAgitReqDlg );
IMPLEMENT_WND_FACTORY( KGCInviteAgitReqDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCInviteAgitReqDlg, "gc_invite_agit_req_dlg" );

KGCInviteAgitReqDlg::KGCInviteAgitReqDlg(void)
: m_pkInviteBtn(NULL)
, m_pkStaticAllCheck(NULL)
, m_pkStaticConnectBuddy(NULL)
, m_pkScrollBar(NULL)
, m_pkCloseBtn(NULL)
, m_pkCheckBoxAll(NULL)
, m_pkBackScroll(NULL)
, m_iRealIndex(0)
{
    char strName[MAX_PATH];
    for( int i = 0; i < EBUDDY_LIST_NUM; ++i )
    {
        m_pkBuddyInfo[i] = NULL;
        sprintf( strName, "user_info%d", i );
        LINK_CONTROL( strName, m_pkBuddyInfo[i] );
    }

    LINK_CONTROL( "btn_invite",             m_pkInviteBtn );
    LINK_CONTROL( "btn_close",              m_pkCloseBtn );
    LINK_CONTROL( "scroll_buddy_list",      m_pkScrollBar );
    LINK_CONTROL( "checkbox_all_buddy",     m_pkCheckBoxAll );
    LINK_CONTROL( "static_connect_buddy",   m_pkStaticConnectBuddy );
    LINK_CONTROL( "static_all",             m_pkStaticAllCheck );
    LINK_CONTROL( "back_scroll",            m_pkBackScroll );

    m_vecBuddyList.clear();
}

KGCInviteAgitReqDlg::~KGCInviteAgitReqDlg(void)
{

}

void KGCInviteAgitReqDlg::OnCreate( void )
{
    m_vecBuddyList.clear();

    for( int i = 0; i < EBUDDY_LIST_NUM; ++i )
    {
        m_pkBuddyInfo[i]->InitState( false, true, this );
        m_pkBuddyInfo[i]->SetCheck( false );
    }

    m_pkInviteBtn->InitState( true, true, this );
    m_pkCloseBtn->InitState( true, true, this );
    m_pkScrollBar->InitState( true, true, this );    
    m_pkBackScroll->InitState(true);

    m_pkCheckBoxAll->InitState( true, true, this );
    m_pkCheckBoxAll->SetCheck(false);

    m_pkStaticAllCheck->InitState( true );
    m_pkStaticAllCheck->SetAlign(DT_CENTER);
    m_pkStaticAllCheck->SetText(L"All");

    m_pkStaticConnectBuddy->InitState( true );
    m_pkStaticConnectBuddy->SetText(L"");
    m_iRealIndex = 0;

    m_pkCloseBtn->SetHotKey( DIK_ESCAPE );

    InitBuddyList();
    InitScroll();        
}

void KGCInviteAgitReqDlg::OnCreateComplete( void )
{
    OnScroll();
}

void KGCInviteAgitReqDlg::InitInfo()
{
    m_vecBuddyList.clear();

    for( int i = 0; i < EBUDDY_LIST_NUM; ++i )
    {
        m_pkBuddyInfo[i]->ToggleRender( false );
        m_pkBuddyInfo[i]->SetCheck( false );
    }

    m_pkCheckBoxAll->SetCheck(false);
    m_iRealIndex = 0;

    InitBuddyList();
    InitScroll();
    OnScroll();

    m_pkStaticConnectBuddy->SetText(g_pkStrLoader->GetReplacedString(STR_ID_NOW_CONNECT_FRIEND, "i", m_vecBuddyList.size()));
}

void KGCInviteAgitReqDlg::ActionPerformed( const KActionEvent& event )
{
    GCWND_MAP( m_pkScrollBar, OnScroll );
    GCWND_MSG_MAP( m_pkCloseBtn,     KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );

    GCWND_MSG_MAP( m_pkInviteBtn, D3DWE_BUTTON_CLICK, OnInviteBtn );

    GCWND_WND_CODE_MAP( m_pkCheckBoxAll, OnCheckAll );    

    for( int i = 0; i < EBUDDY_LIST_NUM; ++i )
    {
        if( event.m_pWnd == m_pkBuddyInfo[i] && event.m_dwCode == D3DWE_BUTTON_CLICK )
        {
            m_pkBuddyInfo[i]->SetCheck( !m_pkBuddyInfo[i]->GetCheck() );

            if( i+m_iRealIndex < static_cast<int>(m_vecBuddyList.size()) )           
            {
                if( m_pkBuddyInfo[i]->GetCheck() )
                    m_vecBuddyList[i+m_iRealIndex].m_bInviteOK = true;
                else
                    m_vecBuddyList[i+m_iRealIndex].m_bInviteOK = false;
            }            
        }
    }
}

void KGCInviteAgitReqDlg::FrameMoveInEnabledState( void )
{
    if( m_vecBuddyList.empty() )
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

void KGCInviteAgitReqDlg::PostChildDraw( void )
{

}

void KGCInviteAgitReqDlg::InitScroll( void )
{
    m_pkScrollBar->SetScrollPos( 0 );
    m_pkScrollBar->SetScrollRangeMin( 0 );
    m_pkScrollBar->SetScrollPageSize( 1 );

    int iFileCount = static_cast<int>( m_vecBuddyList.size() );
    int iScrollMax = 0;
    int iCount =  iFileCount - EBUDDY_LIST_NUM;

    if(iCount > 0)
    {
        iScrollMax = iCount;
    }

    m_pkScrollBar->SetScrollRangeMax( iScrollMax + 1 );
}

void KGCInviteAgitReqDlg::OnScroll()
{
    m_iRealIndex = m_pkScrollBar->GetScrollPos();
    int iMaxBuddyView = GetViewCount( m_iRealIndex );

    for(int i = 0; i < iMaxBuddyView; ++i )
    {
        int iIndex = i + m_iRealIndex;

        if( 0 <= iIndex && iIndex < (int)m_vecBuddyList.size() )
        {		
            m_pkBuddyInfo[i]->SetText( m_vecBuddyList[iIndex].m_strNickName );
            m_pkBuddyInfo[i]->SetCheck( m_vecBuddyList[iIndex].m_bInviteOK );
            m_pkBuddyInfo[i]->ToggleRender( true );
        }
    }

    for(int i = iMaxBuddyView; i < EBUDDY_LIST_NUM; ++i)
    {
        m_pkBuddyInfo[i]->ToggleRender( false );
    }	
}

void KGCInviteAgitReqDlg::OnCheckAll( KD3DWnd* pWnd, DWORD dwCode )
{
    KGCCheckBox* pEventWnd = static_cast<KGCCheckBox*>( pWnd );

    switch( dwCode )
    {
    case D3DWE_BUTTON_CLICK:
        pEventWnd->SetCheck( !pEventWnd->GetCheck() );

        if( pEventWnd->GetCheck() )
        {
            for( int i = 0; i < static_cast<int>(m_vecBuddyList.size()); i++ )
            {
                m_vecBuddyList[i].m_bInviteOK = true;
            }
            OnScroll();
        }
        else
        {
            for( int i = 0; i < static_cast<int>(m_vecBuddyList.size()); i++ )
            {
                m_vecBuddyList[i].m_bInviteOK = false;
            }
            OnScroll();
        }
        break;
    }
}

int KGCInviteAgitReqDlg::GetViewCount(int iMemberIndex_)
{
    int iViewCount = static_cast<int>(m_vecBuddyList.size() ) - iMemberIndex_;

    if(iViewCount < EBUDDY_LIST_NUM)
        return iViewCount;

    return EBUDDY_LIST_NUM;
}

void KGCInviteAgitReqDlg::OnInviteBtn()
{
    std::vector<DWORD> vecInvite;

    for( int i = 0; i < static_cast<int>(m_vecBuddyList.size()); i++ )
    {
        if( m_vecBuddyList[i].m_bInviteOK == true )
            vecInvite.push_back(m_vecBuddyList[i].m_dwUserUID);
    }

    if( vecInvite.empty() )
        return;
    
    KP2P::GetInstance()->Send_InviteAgitReq(vecInvite);

    InitInfo();
    this->ToggleRender(false);
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_AGIT_INVITE_FRIEND_MGS),
        L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true );
}

void KGCInviteAgitReqDlg::InitBuddyList()
{

    for( int i = 0; i < EBUDDY_LIST_NUM; ++i )
    {
        m_pkBuddyInfo[i]->InitInfo();
    }
    
    std::vector<DWORD> vecFriendList;
    vecFriendList.clear();
    SiKFriendShip()->GetOnlienFriendList(GCNateOn::FRIEND_MODE, vecFriendList);

    //온라인 유저 중에 목록에 없는 분들 싹 넣음..
    for (int i=0;i<(int)vecFriendList.size();i++)
    {
        KInviteFriendUserInfo kInviteFriendUserinfo;
        KBuddy fdata;
        SiKFriendShip()->GetFriendBuddyByUID(vecFriendList[i],fdata);
        kInviteFriendUserinfo.m_cGrade = (char)-1;
        kInviteFriendUserinfo.m_cBestCharGrade = (char)-1;
        kInviteFriendUserinfo.m_dwUserUID = vecFriendList[i];
        kInviteFriendUserinfo.m_strNickName = fdata.GetNickName();
        kInviteFriendUserinfo.m_strLocation = fdata.GetServerName() +L"/" + fdata.GetLocation();
        kInviteFriendUserinfo.m_bInviteOK = false;
        m_vecBuddyList.push_back(kInviteFriendUserinfo);
    }

    //테스트 코드
    //for (int i=0;i<(int)15;i++)
    //{
    //    KInviteFriendUserInfo kInviteFriendUserinfo;
    //    //SiKFriendShip()->GetFriendBuddyByUID(vecFriendList[i],fdata);
    //    kInviteFriendUserinfo.m_cGrade = (char)-1;
    //    kInviteFriendUserinfo.m_cBestCharGrade = (char)-1;
    //    kInviteFriendUserinfo.m_dwUserUID = i;
    //    std::wstringstream wstrm;
    //    wstrm << L"Azit_" << i;
    //    kInviteFriendUserinfo.m_strNickName = wstrm.str();
    //    kInviteFriendUserinfo.m_strLocation = L"nmr";
    //    kInviteFriendUserinfo.m_bInviteOK = false;
    //    m_vecBuddyList.push_back(kInviteFriendUserinfo);
    //}
}

void KGCInviteAgitReqDlg::OnClose()
{
    g_KDSound.Play( "31" );
    //SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
    InitInfo();
    this->ToggleRender(false);   
}