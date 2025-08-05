#include "stdafx.h"
#include ".\KGCChatScrollBackground.h"
//


//담에 하시는분 콤포넌트로 빼세요. 또 이거 쓸일있을려나 몰라서 그냥 씀

IMPLEMENT_CLASSNAME( KGCChatScrollBackground );
IMPLEMENT_WND_FACTORY( KGCChatScrollBackground );
IMPLEMENT_WND_FACTORY_NAME( KGCChatScrollBackground, "gc_scroll_background_bar" );

KGCChatScrollBackground::KGCChatScrollBackground(void)
{

    for( int i = 0; i < CSB_COMPONENT_NUM; i++ )
        m_pBackgroundComponent[i] = NULL;
    
    LINK_CONTROL("scroll_background_up",		    m_pBackgroundComponent[CSB_UP] );
    LINK_CONTROL("scroll_background_middle",	    m_pBackgroundComponent[CSB_MIDDLE] );
    LINK_CONTROL("scroll_background_down",	    	m_pBackgroundComponent[CSB_DOWN] ); 
}

KGCChatScrollBackground::~KGCChatScrollBackground(void)
{
}

void KGCChatScrollBackground::OnCreate( void )
{

}
void KGCChatScrollBackground::SetMiddleHeight(DWORD dwHeight)
{
    D3DXVECTOR2 vPos;
    m_pBackgroundComponent[CSB_MIDDLE]->SetHeight(dwHeight);    
    vPos.x=0;
    vPos.y=(float)dwHeight;
    m_pBackgroundComponent[CSB_DOWN]->SetWindowPos(vPos);

}

DWORD KGCChatScrollBackground::GetMiddleHeight()
{
    DWORD dwResult = 0 ;
    
    if (m_pBackgroundComponent[CSB_MIDDLE]!=NULL)
    {
        return m_pBackgroundComponent[CSB_MIDDLE]->GetHeight();
    }
    return dwResult;
}

void KGCChatScrollBackground::OnCreateComplete( void )
{
    SetMiddleHeight(GetMiddleHeight());
}

void KGCChatScrollBackground::OnDestroyComplete( void )
{
}

void KGCChatScrollBackground::FrameMoveInEnabledState( void )
{

}