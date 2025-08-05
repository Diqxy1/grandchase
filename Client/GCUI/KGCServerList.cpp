#include "stdafx.h"
//
#include "GCUI/KGCServerList.h"

#include "GCUI/KGCServerBar.h"
//
//
//
#include "../MyD3D.h"


#include "../gcui/GCGuideNewbie.h"

#include "GCHighLight.h"

IMPLEMENT_CLASSNAME( KGCServerList );
IMPLEMENT_WND_FACTORY( KGCServerList );
IMPLEMENT_WND_FACTORY_NAME( KGCServerList, "gc_server_list" );

KGCServerList::KGCServerList(void)
{
    m_iCurrentPage = 0;
    m_iViewMaxList = 0;
    m_iTopIndex = 0;
    m_pkServerBar = NULL;
	m_pkBar_draw_rect = NULL;
  
    i_delay = 55;

	m_pkClipRange = NULL;
    LINK_CONTROL("server_bar00",m_pkServerBar);
    LINK_CONTROL("clip_range",m_pkClipRange);
	LINK_CONTROL("bar_draw_rect",m_pkBar_draw_rect);

	m_pkBtnConnection = NULL;
	LINK_CONTROL("bnt_connection",m_pkBtnConnection);


	m_pkServerFocus = NULL;


	m_pkHighLight = NULL;
	LINK_CONTROL( "highlight", m_pkHighLight );

    m_pkBtnLeftPage = NULL;
    LINK_CONTROL( "btn_leftpage", m_pkBtnLeftPage );

    m_pkBtnRightPage = NULL;
    LINK_CONTROL( "btn_rightpage", m_pkBtnRightPage );

}

void KGCServerList::OnCreate( void )
{
    if ( m_pkServerBar )
		m_pkServerBar->InitState(false,true,this);

	if ( m_pkClipRange ) {
		m_pkClipRange->InitState( true, true );
		m_pkClipRange->SetClipChild( true );
	}
	if ( m_pkBar_draw_rect)
		m_pkBar_draw_rect->InitState(false);

    if( m_pkBtnLeftPage )
        m_pkBtnLeftPage->InitState( true, true,this );

    if( m_pkBtnRightPage )
        m_pkBtnRightPage->InitState( true, true,this);

	m_pkBtnConnection->InitState(true,true,this);

	m_pkHighLight->InitState(false);
	m_pkHighLight->SetDarkReginon(m_pkBar_draw_rect->GetWidth() , m_pkBar_draw_rect->GetHeight());
	m_pkHighLight->SetDarkReginonStartPos(m_pkBar_draw_rect->GetFixedWindowLocalPos());


	m_vecViewServerBar.clear();
	
    m_iViewMaxList  = MAX_SERVERLIST_ON_ONE_PAGE;
	m_vecServerBarBack = g_pkUIMgr->CreateTemplateUIListByFindName( this, "ui_scene_server.stg", "", "server_scene\\server_list\\server_bar_back", true, m_iViewMaxList );
	std::vector<KD3DWnd*>::iterator vit = m_vecServerBarBack.begin();
	for (;vit != m_vecServerBarBack.end() ; ++vit)
	{
		(*vit)->InitState(false);
	}
	
}

KGCServerList::~KGCServerList( void )
{

	
}

void KGCServerList::ActionPerformed( const KActionEvent& event )
{
	switch ( event.m_dwCode )
	{
	case KD3DWnd::D3DWE_CURSOR_ENTER: 
		g_KDSound.Play( "32" ); 
		return;
	case  KD3DWnd::D3DWE_BUTTON_CLICK:
		{
			if (event.m_pWnd == m_pkBtnConnection)
			{
				if (m_pkServerFocus)
				{
					m_pkServerFocus->OnSelectServer();
					return;
				}
			}
			for(std::vector<KGCServerBar*>::iterator vIt = m_vecViewServerBar.begin();vIt != m_vecViewServerBar.end();vIt++)
			{
				if((*vIt) == event.m_pWnd)
				{

					m_pkServerFocus = *vIt;
					break;
					
				}
			}
            if (event.m_pWnd == m_pkBtnLeftPage )
            {
                if( m_iCurrentPage >= 1 ) 
                    m_iCurrentPage--;
                SetServerList();
            }
            if (event.m_pWnd == m_pkBtnRightPage )
            {
                int iMaxPage = m_vecServerBar.size() / MAX_SERVERLIST_ON_ONE_PAGE - 1; //페이지가 0부터 시작이므로 -1을 해준다.

                if( (m_vecServerBar.size() % MAX_SERVERLIST_ON_ONE_PAGE) > 0 )
                    iMaxPage++;

                if( iMaxPage > m_iCurrentPage )
                    m_iCurrentPage++;
                SetServerList();
            }
		}	
		break;
	case KD3DWnd::D3DWE_DOUBLE_CLICK:
		{
			for(std::vector<KGCServerBar*>::iterator vIt = m_vecViewServerBar.begin();vIt != m_vecViewServerBar.end();vIt++)
			{
				if((*vIt) == event.m_pWnd)
				{

					(*vIt)->OnSelectServer();
					return;
					
				}
			}

		}
		
	}
}

void KGCServerList::OnCreateComplete( void )
{
		
}

void KGCServerList::FrameMoveInEnabledState()
{
	std::vector<KD3DWnd*>::iterator vit = m_vecServerBarBack.begin();
	for (;vit != m_vecServerBarBack.end() ; ++vit)
	{
		(*vit)->SetWndMode(D3DWM_DEFAULT);
	}

	std::vector<KGCServerBar*>::iterator vitServerBar = m_vecViewServerBar.begin();
	for (int i=0 ;vitServerBar != m_vecViewServerBar.end() ; ++vitServerBar , i++) 
	{
		if ((*vitServerBar) == m_pkServerFocus)
		{
			m_vecServerBarBack[i]->SetWndMode(D3DWM_HOVER);
			break;
		}
	}

    // 서버리스트 요청한다!
    // 매 프레임 부르지만 일정시간이 지나야 패킷을 보낸다( 딜레이가 있음 )
    if(i_delay > 0)
		i_delay--;
    KP2P::GetInstance()->Send_ServerListReq();

	//SetFocusingHilight();


}

std::string KGCServerList::GetChildFile()
{
    return m_pkServerBar->GetFileName();
}

std::string KGCServerList::GetChildPath()
{
    return m_pkServerBar->GetPath();
}



void KGCServerList::SetServerList()
{
    if( g_kGlobalValue.ServerList.size() > MAX_SERVERLIST_ON_ONE_PAGE )
    {
        m_pkBtnLeftPage->ToggleRender( true );
        m_pkBtnRightPage->ToggleRender( true );
    }
    else
    {
        m_pkBtnLeftPage->ToggleRender( false );
        m_pkBtnRightPage->ToggleRender( false );
        m_iCurrentPage = 0;
    }

    if(m_ServerList.size() != g_kGlobalValue.ServerList.size())
    {
        int iNumServer = (int)g_kGlobalValue.ServerList.size() - (int)m_ServerList.size();

        if(iNumServer > 0){
            std::vector<KD3DWnd*> vecCreatedList = g_pkUIMgr->CreateTemplateUIListByFindName( m_pkClipRange, GetChildFile().c_str(), "", GetChildPath().c_str(), true, iNumServer );

            m_ServerList = g_kGlobalValue.ServerList;
            for(std::vector<KD3DWnd*>::iterator vit = vecCreatedList.begin() ; vit != vecCreatedList.end() ; ++vit ) {
                m_vecServerBar.push_back( static_cast<KGCServerBar*>(*vit) );
            }
        }
    }
	
	for(std::vector<KGCServerBar*>::iterator vit = m_vecServerBar.begin() ; vit != m_vecServerBar.end() ; ++vit ) {
		(*vit)->InitState(false,true,this);
	}

    
    int i = 0;
    for(std::vector<KGCServerBar*>::iterator vit = m_vecServerBar.begin();vit != m_vecServerBar.end();vit++)
    {
		(*vit)->ReSetInfo();
        if(static_cast<int>(g_kGlobalValue.ServerList.size()) > i)
            (*vit)->SetServerInfo(&g_kGlobalValue.ServerList[i++]);
    }

	SetViewServerList();
}

void KGCServerList::SetViewServerList()
{
	m_vecViewServerBar.clear();
    unsigned int uiStartValue = m_iCurrentPage * MAX_SERVERLIST_ON_ONE_PAGE;
    for (unsigned int iLoop = uiStartValue; iLoop <= uiStartValue + MAX_SERVERLIST_ON_ONE_PAGE - 1; ++iLoop )
    {
		if ( m_vecServerBar.begin() == m_vecServerBar.end() )
			break;

        if( iLoop > m_vecServerBar.size() -1 )
            break;

        if( m_vecServerBar.at(iLoop)->GetServerInfo().m_dwUID == 0 )
            continue;
        
        m_vecViewServerBar.push_back(m_vecServerBar.at(iLoop));
    }
}


void KGCServerList::PostChildDraw( void )
{
	std::vector<KD3DWnd*>::iterator vit = m_vecServerBarBack.begin();
	for (; vit != m_vecServerBarBack.end() ; ++vit)
	{
		(*vit)->ToggleRender(false);
	}

    int iViewSize = static_cast<int>( m_vecViewServerBar.size() );

    if (m_iViewMaxList != iViewSize)
        m_iViewMaxList = iViewSize;

	for(int i = 0; i < m_iViewMaxList ; i++)
	{
		D3DXVECTOR2 vPos = m_vecServerBarBack[i]->GetCurrentWindowLocalPos();
		m_vecServerBarBack[i]->ToggleRender(true);

		m_vecViewServerBar[i]->ToggleRender(true);
		m_vecViewServerBar[i]->SetWidth(m_vecServerBarBack[i]->GetWidth());
		m_vecViewServerBar[i]->SetHeight(m_vecServerBarBack[i]->GetHeight());
		m_vecViewServerBar[i]->SetFixedWindowLocalPos(vPos);
		m_vecViewServerBar[i]->OptimizationPos();
		m_vecViewServerBar[i]->Render();
	}
}

KGCServerBar* KGCServerList::FindServerBar(DWORD dwServerUID)
{
    for(std::vector<KGCServerBar*>::iterator vIter = m_vecServerBar.begin();vIter != m_vecServerBar.end();vIter++)
    {
        if((*vIter)->IsServerUID(dwServerUID))
            return (*vIter);
    }
    return NULL;
}

void KGCServerList::OnInitialize()
{
    m_iCurrentPage = 0;
    //서버 개수가 7(한 페이지에 그릴 수 있는 개수)이하면 버튼을 그리지 않는다.
    if( m_vecServerBar.size() <= MAX_SERVERLIST_ON_ONE_PAGE )
    {
        m_pkBtnLeftPage->ToggleRender( false );
        m_pkBtnRightPage->ToggleRender( false );
    }
    else
    {
        m_pkBtnLeftPage->ToggleRender( true );
        m_pkBtnRightPage->ToggleRender( true );
    }
	float fRectWidth = static_cast<float>(m_pkBar_draw_rect->GetWidth());
	float fRectHeight = static_cast<float>(m_pkBar_draw_rect->GetHeight());

	D3DXVECTOR2 vStarLocalPos = m_pkBar_draw_rect->GetFixedWindowLocalPos();

    for (int i=0 ; i< MAX_SERVERLIST_ON_ONE_PAGE ; i++)
    {
        float fPosY = m_vecServerBarBack[i]->GetHeight() * i * 1.f + 2; //vStarLocalPos.y + i*(fHeightGap * (ROW_GAP_TIMES +1));

        m_vecServerBarBack[i]->SetFixedWindowLocalPos(D3DXVECTOR2(vStarLocalPos.x,vStarLocalPos.y + fPosY));
        m_vecServerBarBack[i]->ToggleRender(false);
    }
    SetServerList();
}

void KGCServerList::SetFocusingHilight()
{
	if (!m_pkServerFocus)
	{
		m_pkHighLight->ToggleRender(false);
		return;
	}
	m_pkHighLight->ToggleRender(true);

	DWORD dwWidth = m_pkServerFocus->GetWidth()+ m_pkServerFocus->GetWidth()/30;
	DWORD dwHeight = m_pkServerFocus->GetHeight()+ m_pkServerFocus->GetHeight()/10;
	m_pkHighLight->SetHighLightRegion(m_pkServerFocus->GetFixedWindowLocalPos().x , m_pkServerFocus->GetFixedWindowLocalPos().y , dwHeight , dwWidth);
	
}



