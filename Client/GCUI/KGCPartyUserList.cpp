#include "StdAfx.h"
#include "gcui/KGCPartyUserList.h"

IMPLEMENT_CLASSNAME( KGCPartyUserList );
IMPLEMENT_WND_FACTORY( KGCPartyUserList );
IMPLEMENT_WND_FACTORY_NAME( KGCPartyUserList, "gc_party_user_list" );

KGCPartyUserList::KGCPartyUserList(void)
: m_pkPopupBox ( NULL )
, m_dwSelectedUID(0)
{

    for (int i =0 ; i < PARTYUSER_MAXNUM ; ++ i )
        m_vecPartyUserList.push_back(NULL);

    m_strClickedUserName = L"";

    char str[MAX_PATH];
    for ( int i = 0; i < PARTYUSER_MAXNUM ; ++i )
    {
        sprintf( str, "member_bar%d", i+1 );
        LINK_CONTROL( str, m_vecPartyUserList[i] );
    }
    
    LINK_CONTROL( "popupbox",               m_pkPopupBox );
}

KGCPartyUserList::~KGCPartyUserList(void)
{

}

void KGCPartyUserList::OnCreate()
{
    for(std::vector<KGCPartyUserList_Element*>::iterator vIter = m_vecPartyUserList.begin();vIter != m_vecPartyUserList.end();vIter++)
    {
        (*vIter)->InitState(true,true,this);
        (*vIter)->ConnectEventProcedure(KD3DWnd::D3DWE_RBUTTON_CLICK, this, &KGCPartyUserList::OnRClickMember);
        (*vIter)->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPartyUserList::OffPopupbox);
        (*vIter)->ConnectEventProcedure(KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCPartyUserList::MouseHover);
        (*vIter)->ConnectEventProcedure(KD3DWnd::D3DWE_CURSOR_LEAVE, this, &KGCPartyUserList::MouseHoverLeave);
    }

    m_pkPopupBox->InitState( false, true, this );
}

void KGCPartyUserList::OnRClickMember(const KActionEvent& event)
{
    for(int i = 0; i < PARTYUSER_MAXNUM ; i++)
    {
        if(m_vecPartyUserList[i] == event.m_pWnd)
        {
            SetMenu(i);
            return;
        }
    }
}
void KGCPartyUserList::MouseHover(const KActionEvent& event)
{
    for(int i = 0; i < PARTYUSER_MAXNUM ; i++)
    {
        if(m_vecPartyUserList[i] == event.m_pWnd)
        {
            m_vecPartyUserList[i]->CursorEnter( true );
        }
    }
} 

void KGCPartyUserList::MouseHoverLeave(const KActionEvent& event)
{
    for(int i = 0; i < PARTYUSER_MAXNUM ; i++)
    {
        if(m_vecPartyUserList[i] == event.m_pWnd)
        {
            m_vecPartyUserList[i]->CursorEnter( false );
            
        }
    }
}

void KGCPartyUserList::OffPopupbox(const KActionEvent& event)
{
    m_pkPopupBox->ToggleRender( false );
}

void KGCPartyUserList::OnCreateComplete( void )
{
    m_pkPopupBox->ToggleRender( false );
}

void KGCPartyUserList::ActionPerformed( const KActionEvent& event )
{
    if(event.m_dwCode == KGCMultiMenu::K3DWE_MENU_CLICKED && event.m_pWnd == m_pkPopupBox)
    {
        OnClickMenu(event);
    }
}

void KGCPartyUserList::SetMenu( int index )
{
    m_pkPopupBox->ClearMenu();
    
    D3DXVECTOR2 vTempPos = m_vecPartyUserList[index]->GetFixedWindowLocalPos();
    vTempPos.x = (float)m_vecPartyUserList[index]->GetWidth();
    m_pkPopupBox->SetWindowPosDirect( vTempPos );
    m_pkPopupBox->ToggleRender(true);
    // 위치선정.

    //기능 추가
    m_dwSelectedUID  = m_vecPartyUserList[index]->GetPartyMemberUID();

    if(SiKGCPartyManager()->IsLocalPartyMember(m_vecPartyUserList[index]->GetPartyMemberUID())) // 자기 자신 체크
    {
        m_pkPopupBox->AddMenu(EKPULPM_CHANGE_PARTY_LEAVE,g_pkStrLoader->GetString( STR_ID_PARTY_BREAK ));
    }
    else if(SiKGCPartyManager()->IsLocalPartyMasterUser()) //방장이 체크
    {
        m_pkPopupBox->AddMenu(EKPULPM_CHANGE_PARTY_MASTER,g_pkStrLoader->GetString( STR_ID_PARTY_CHANGE_CAPTAIN ));
        m_pkPopupBox->AddMenu(EKPULPM_CHANGE_PARTY_OUT_MEMBER,g_pkStrLoader->GetString( STR_ID_PARTY_OUT ));
    }
    else //방장 아닌놈이 자기 아닌놈 찍음
    {
        m_pkPopupBox->ToggleRender(false);
    }
}

void KGCPartyUserList::CloseMenu( void )
{
    m_pkPopupBox->ToggleRender(false);

}

void KGCPartyUserList::OnClickMenu( const KActionEvent& event )
{
    switch(event.m_dwlParam)
    {
    case EKPULPM_CHANGE_PARTY_MASTER :
        SiKGCPartyManager()->ChangePartyMasterUser(m_dwSelectedUID);
        break;
    case EKPULPM_CHANGE_PARTY_LEAVE :
        SiKGCPartyManager()->LeaveParty();
        break;
    case EKPULPM_CHANGE_PARTY_OUT_MEMBER :
        SiKGCPartyManager()->OutPartyMember(m_dwSelectedUID);
        break;
    default:
        break;
    }
    CloseMenu();
}


void KGCPartyUserList::FrameMoveInEnabledState( void )
{
    if ( m_pkPopupBox->GetIsRenderMsgBox() )
    {
        m_pkPopupBox->SetIsRenderMsgBox( false );
    }
}

void KGCPartyUserList::SetPartyMember()
{   
    for(int i = 0; i < PARTYUSER_MAXNUM ; i++)
    {
        if(i < static_cast<int>(SiKGCPartyManager()->GetPartyMemberList().size())){
            m_vecPartyUserList[i]->SetPartyMember(SiKGCPartyManager()->GetPartyMemberList()[i]);

            m_vecPartyUserList[i]->Lock(false);
        }
        else{
            m_vecPartyUserList[i]->initElement();
            m_vecPartyUserList[i]->Lock(true);
        }
    }
}