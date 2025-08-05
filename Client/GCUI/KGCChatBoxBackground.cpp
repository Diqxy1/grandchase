#include "stdafx.h"
#include ".\KGCChatBoxBackground.h"
//


//담에 하시는분 콤포넌트로 빼세요. 또 이거 쓸일있을려나 몰라서 그냥 씀

IMPLEMENT_CLASSNAME( KGCChatBoxBackground );
IMPLEMENT_WND_FACTORY( KGCChatBoxBackground );
IMPLEMENT_WND_FACTORY_NAME( KGCChatBoxBackground, "gc_chatbox_background" );

KGCChatBoxBackground::KGCChatBoxBackground(void)
{

    for( int i = 0; i < CBB_COMPONENT_NUM; i++ )
        m_pBackgroundComponent[i] = NULL;
  
    LINK_CONTROL("chat_list_back_left_top",		        m_pBackgroundComponent[CBB_LEFT_UP] );
    LINK_CONTROL("chat_list_back_left_center",	        m_pBackgroundComponent[CBB_LEFT_MIDDLE] );
    LINK_CONTROL("chat_list_back_left_bottom",	        m_pBackgroundComponent[CBB_LEFT_DOWN] ); 
    LINK_CONTROL("chat_list_back_top",		            m_pBackgroundComponent[CBB_MIDDLE_UP] );
    LINK_CONTROL("chat_list_back_center",	            m_pBackgroundComponent[CBB_MIDDLE_MIDDLE] );
    LINK_CONTROL("chat_list_back_center_bottom",	    m_pBackgroundComponent[CBB_MIDDLE_DOWN] ); 
    LINK_CONTROL("chat_list_back_right_top",	        m_pBackgroundComponent[CBB_RIGHT_UP] );
    LINK_CONTROL("chat_list_back_right_center",	        m_pBackgroundComponent[CBB_RIGHT_MIDDLE] );
    LINK_CONTROL("chat_list_edit_back_right_bottom",	m_pBackgroundComponent[CBB_RIGHT_DOWN] ); 
}

KGCChatBoxBackground::~KGCChatBoxBackground(void)
{
}

void KGCChatBoxBackground::OnCreate( void )
{

}
void KGCChatBoxBackground::SetMiddleHeight(DWORD dwHeight)
{
    D3DXVECTOR2 vPos;
    m_pBackgroundComponent[CBB_MIDDLE_MIDDLE]->SetHeight(dwHeight);
    m_pBackgroundComponent[CBB_RIGHT_MIDDLE]->SetHeight(dwHeight);
    m_pBackgroundComponent[CBB_LEFT_MIDDLE]->SetHeight(dwHeight);
    vPos.y = (float)m_pBackgroundComponent[CBB_MIDDLE_DOWN]->GetHeight()+dwHeight;

    vPos.x=m_pBackgroundComponent[CBB_LEFT_DOWN]->GetCurrentWindowLocalPos().x;    
    m_pBackgroundComponent[CBB_LEFT_DOWN]->SetWindowPos(vPos);
    vPos.x=m_pBackgroundComponent[CBB_MIDDLE_DOWN]->GetCurrentWindowLocalPos().x;
    m_pBackgroundComponent[CBB_MIDDLE_DOWN]->SetWindowPos(vPos);
    vPos.x=m_pBackgroundComponent[CBB_RIGHT_DOWN]->GetCurrentWindowLocalPos().x;
    m_pBackgroundComponent[CBB_RIGHT_DOWN]->SetWindowPos(vPos);

}

DWORD KGCChatBoxBackground::GetMiddleHeight()
{
    DWORD dwResult = 0 ;
    
    if (m_pBackgroundComponent[CBB_MIDDLE_MIDDLE]!=NULL)
    {
        return m_pBackgroundComponent[CBB_MIDDLE_MIDDLE]->GetHeight();
    }
    return dwResult;
}

void KGCChatBoxBackground::OnCreateComplete( void )
{
    SetMiddleHeight(GetMiddleHeight());
}

void KGCChatBoxBackground::OnDestroyComplete( void )
{
}

void KGCChatBoxBackground::FrameMoveInEnabledState( void )
{

}