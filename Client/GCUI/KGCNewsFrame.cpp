#include "stdafx.h"
//
#include "GCUI/KGCNewsFrame.h"
IMPLEMENT_CLASSNAME( KGCNewsFrame );
IMPLEMENT_WND_FACTORY( KGCNewsFrame );
IMPLEMENT_WND_FACTORY_NAME( KGCNewsFrame, "gc_news_frame" );

KGCNewsFrame::KGCNewsFrame(void)
{
	for (int i=0 ; i<MAX_NEWS ; i++)
	{
		m_pkServerNews[i] = NULL;
		char temp[MAX_PATH];
		sprintf( temp, "bnt_news%d", i );
		LINK_CONTROL( temp, m_pkbntNews[i] );
	}

	LINK_CONTROL( "news_focus", m_pkNewsFocus );

	m_iCurrentFocusIndex = -1;
	m_iNextFocusIndex = -1;

	m_iCntLotationFrame = 0;
	m_iCntAniFrame = 0;

	m_eAniState = NOTHING;
	m_bCatchFocus = false;

	LoadWebLink();

}

void KGCNewsFrame::OnCreate( void )
{
    NewsBtnInit();
	m_pkNewsFocus->InitState( true, true, this );
}

KGCNewsFrame::~KGCNewsFrame( void )
{
	for (int i=0 ; i<MAX_NEWS ; i++)
	{
		SAFE_RELEASE(m_pkServerNews[i]);
	}
}

void KGCNewsFrame::ActionPerformed( const KActionEvent& event )
{
	int i;
	for( i=0 ; i<MAX_NEWS ; i++ ) 
	{ 
		if (!m_pkbntNews[i]->IsRenderOn())
			continue;

		if( event.m_pWnd == m_pkbntNews[i] && event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER ) { 
			OnCatchFocus( i );
			break;
		}
	}

	if (i==MAX_NEWS)
		OffCatchFocus();
#if defined( NATION_KOREA )
    if( event.m_pWnd == m_pkNewsFocus && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) { 
        OnClickNews();
    }
#elif defined( NATION_USA )
    if( event.m_pWnd == m_pkNewsFocus && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) { 
        OnClickNewsToDefaultBrowser();
    }
#elif defined( NATION_CHINA )
	if( event.m_pWnd == m_pkNewsFocus && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) { 
		OnClickNewsLink();
	}
	
#endif
}

void KGCNewsFrame::OnCreateComplete( void )
{
}

void KGCNewsFrame::FrameMoveInEnabledState()
{
	UpDateFrameTime();
	
}

void KGCNewsFrame::OnInitialize()
{
	m_vecPossibleIndex.clear();
    m_vecNewNum.clear();

	m_iMaxNews = g_kGlobalValue.GetNewFileNum().size()<MAX_NEWS ?g_kGlobalValue.GetNewFileNum().size():MAX_NEWS;
	m_vecNewNum = g_kGlobalValue.GetNewFileNum();
	for (int i=0 ; i<m_iMaxNews ; i++)
	{
		SAFE_RELEASE(m_pkServerNews[i]);
#if defined(NATION_CHINA)
        std::wstring strTemp = CHANNEL_IMG_FOLDER;
        char temp[MAX_PATH];
        sprintf( temp, "%d.png", m_vecNewNum[i] );
        m_pkServerNews[i] = (GCDeviceTexture*)g_pGCDeviceManager2->FindDevice(KncUtil::toNarrowString(strTemp) + temp);

        if ( m_pkServerNews[i] == NULL )
            m_pkServerNews[i] = (GCDeviceTexture*)g_pGCDeviceManager2->CreateTexture( "defaultnews.png" );

#else
		char temp[MAX_PATH];
		sprintf( temp, "news%d.png", m_vecNewNum[i] );
		m_pkServerNews[i] = g_pGCDeviceManager2->CreateTexture(temp);
#endif
		m_pkbntNews[i]->ToggleRender(true);
		m_vecPossibleIndex.push_back(i);

	}
// 
// 	for (int i=0 ; i<MAX_NEWS ; i++)
// 	{
// 		SAFE_RELEASE(m_pkServerNews[i]);
// 		char temp[MAX_PATH];
// 		sprintf( temp, "news%d.png", i );
// 
// 		GCMemoryFile memfile(g_pGCDeviceManager->GetMassFileManager(), temp );
//		if( memfile.IsLoaded())
//		{
//			m_pkServerNews[i] = g_pGCDeviceManager2->CreateTexture(temp);
//			m_pkbntNews[i]->ToggleRender(true);
// 			m_vecPossibleIndex.push_back(i);	
//		}
// 		else
// 		{
// 			m_pkbntNews[i]->ToggleRender(false);
// 		}
// 	}
	if(m_vecPossibleIndex.empty())
	{
		m_iCurrentFocusIndex = -1;
		m_iNextFocusIndex = -1;
	}
	else
	{
		std::vector<UINT>::iterator iterIndex = m_vecPossibleIndex.begin();
		m_iCurrentFocusIndex = *iterIndex;
		++iterIndex;
		if (iterIndex != m_vecPossibleIndex.end())
			m_iNextFocusIndex = *iterIndex;
		else
			m_iNextFocusIndex = -1;

	}

	m_iCntLotationFrame = 0;
	m_iCntAniFrame = 1;
	m_vTextureCoord.w = 0.0f;
	m_vTextureCoord.x = 0.0f;
	m_vTextureCoord.y = 1.0f;
	m_vTextureCoord.z =1.0f;
}

void KGCNewsFrame::LoadWebLink() {
	KLuaManager luaMgr;
	GCFUNC::LoadLuaScript( luaMgr, "EventBannerInfo.stg" );
	float comboskillpenalty = 0.0f;

	LUA_BEGIN_TABLE("EVENT_BANNER_LINK", return)
	{
		for ( int i = 1 ;  ; ++i ) {
			LUA_BEGIN_TABLE(i,break)
			{
				int iIndex;
				std::string strWebLink;
				LUA_GET_VALUE( 1, iIndex, break );    
				LUA_GET_VALUE( 2, strWebLink,break );
				m_mapWebLink.insert(std::pair<int,std::string>(iIndex,strWebLink));
			
			}LUA_END_TABLE(break)
		}
	
	} LUA_END_TABLE(return)

}

bool KGCNewsFrame::SetNewsPage()
{
	if (m_iNextFocusIndex == -1)
	{
		return false;
	}
	else
	{
		m_iCurrentFocusIndex = m_iNextFocusIndex;
		std::vector<UINT>::iterator iterIndex = m_vecPossibleIndex.begin();
		for (; iterIndex != m_vecPossibleIndex.end() ; ++iterIndex)
		{
			if ( *iterIndex == m_iNextFocusIndex )
			{
				iterIndex++;
				if (iterIndex == m_vecPossibleIndex.end())
					m_iNextFocusIndex = *m_vecPossibleIndex.begin();
				else
					m_iNextFocusIndex = *iterIndex;

				break;
			}
		}
		
		return true;
	}	
}

void KGCNewsFrame::UpDateFrameTime()
{
	if (m_iCurrentFocusIndex == -1)
		return;

	switch(m_eAniState)
	{
	case NOTHING:
		if (m_iCntLotationFrame < LOTATION_TIME)
		{
			m_iCntLotationFrame++;
		}
		else
		{
			if (SetNewsPage())
			{
				m_eAniState = PROCESSING;
				m_iCntLotationFrame = 0;
				m_iCntAniFrame = 0;
			}
			
		}
		break;
	case PROCESSING:
		if (m_iCntAniFrame < ANI_FRAME_TIME)
		{
			m_iCntAniFrame++;
		}
		else
		{
			if (m_bCatchFocus)
				m_eAniState = CATCHFOCUS;
			else
				m_eAniState = NOTHING;		
		}
		break;
	case CATCHFOCUS:
		break;
	}
}

void KGCNewsFrame::PostChildDraw()
{
	D3DXVECTOR2 vPos( GetRelocatedWindowPos() );
	D3DXVECTOR2 vLocalPos(m_pkNewsFocus->GetCurrentWindowLocalPos());

	g_pGCDeviceManager2->PushState();
	if (m_iCurrentFocusIndex >= 0 && m_iCurrentFocusIndex < m_iMaxNews)
		g_pGCDeviceManager2->DrawInScreen(m_pkServerNews[m_iCurrentFocusIndex],
		(vPos.x+ vLocalPos.x  * m_fWindowScaleX) , 
		(vPos.y+ vLocalPos.y  * m_fWindowScaleY) ,
		(vPos.x+ vLocalPos.x  * m_fWindowScaleX) + (m_pkNewsFocus->GetWidth() * m_fWindowScaleX) , 
		(vPos.y+ vLocalPos.y  * m_fWindowScaleY) + (m_pkNewsFocus->GetHeight() * m_fWindowScaleY) ,
		m_vTextureCoord.w, m_vTextureCoord.x, m_vTextureCoord.y, m_vTextureCoord.z,false,10.0f,D3DCOLOR_ARGB(255 - m_iCntAniFrame*255/ANI_FRAME_TIME ,255,255,255));

	if (m_iNextFocusIndex >=0 && m_iNextFocusIndex < m_iMaxNews)
		g_pGCDeviceManager2->DrawInScreen(m_pkServerNews[m_iNextFocusIndex],
        (vPos.x+ vLocalPos.x  * m_fWindowScaleX) , 
        (vPos.y+ vLocalPos.y  * m_fWindowScaleY) ,
        (vPos.x+ vLocalPos.x  * m_fWindowScaleX) + (m_pkNewsFocus->GetWidth() * m_fWindowScaleX) , 
        (vPos.y+ vLocalPos.y  * m_fWindowScaleY) + (m_pkNewsFocus->GetHeight() * m_fWindowScaleY) ,
		m_vTextureCoord.w, m_vTextureCoord.x, m_vTextureCoord.y, m_vTextureCoord.z,false,10.0f,D3DCOLOR_ARGB( m_iCntAniFrame*255/ANI_FRAME_TIME ,255,255,255));

	for (int i=0 ; i<m_iMaxNews ; i++)
	{
		if (m_pkbntNews[i]->IsRenderOn())
		{
			if (i == m_iNextFocusIndex)
				m_pkbntNews[i]->SetWndMode(D3DWM_HOVER);
			else
				m_pkbntNews[i]->SetWndMode(D3DWM_DEFAULT);

			m_pkbntNews[i]->Render();
		}
	}

	g_pGCDeviceManager2->PopState();
}

void CloseNewsPage( void )
{
    if(g_pkUIScene->IsMsgBoxModal() == false)
        g_pkUIScene->Stop( false );

    ::SetFocus( g_hUIMainWnd );
}

void KGCNewsFrame::OnClickNews( void )
{
    if (m_iCurrentFocusIndex < 0 || m_iCurrentFocusIndex >= m_iMaxNews ) { 
        return;
    }

    // 웹브라우저를 띄우기 전에 버퍼 카운트를 맞춰준다.
    int cnt;
    for ( cnt = 0; !g_MyD3D->IsPrimaryBuffer && cnt < 100; ++cnt )
    {
        if ( D3D_OK == g_MyD3D->m_pd3dDevice->Present( NULL, NULL, NULL, NULL ) )
        {
            g_MyD3D->IsPrimaryBuffer = !g_MyD3D->IsPrimaryBuffer;
        }
    }
    if ( 100 == cnt ) {
        return;
    }

    RECT rc =
    {
        W_Width / 42, 
        W_Height / 32,
        W_Width - ( W_Width / 42 ),
        W_Height - ( W_Height / 32 ),
    };

    if ( false == g_pBrowser->Create( rc, g_MyD3D->m_hWnd ) ) {
        return;
    }
    
    std::string strWebPage( "http://chase.netmarble.net/news/event/bbsListView.asp" );
    
    g_pBrowser->Navigate( strWebPage.c_str(), "" );
    g_pBrowser->SetEndFunction( CloseNewsPage );
    g_pkUIScene->Stop( true );
}

void KGCNewsFrame::OnClickNewsLink( void )
{
    if (m_iCurrentFocusIndex < 0 || m_iCurrentFocusIndex >= m_iMaxNews ) { 
        return;
    }

    // 웹브라우저를 띄우기 전에 버퍼 카운트를 맞춰준다.
    int cnt;
    for ( cnt = 0; !g_MyD3D->IsPrimaryBuffer && cnt < 100; ++cnt )
    {
        if ( D3D_OK == g_MyD3D->m_pd3dDevice->Present( NULL, NULL, NULL, NULL ) )
        {
            g_MyD3D->IsPrimaryBuffer = !g_MyD3D->IsPrimaryBuffer;
        }
    }
    if ( 100 == cnt ) {
        return;
    }

    RECT rc =
    {
        W_Width / 42, 
        W_Height / 32,
        W_Width - ( W_Width / 42 ),
        W_Height - ( W_Height / 32 ),
    };

    if ( false == g_pBrowser->Create( rc, g_MyD3D->m_hWnd ) ) {
        return;
    }

//     std::map<int,std::string>::iterator mit = m_mapWebLink.find(m_iNextFocusIndex);
     std::string strWebPage;
//     if ( mit != m_mapWebLink.end() ) {
//         strWebPage = mit->second;
//     } 
//     else {
//         strWebPage = "http://www.cyou.com";
//     }
    char temp[MAX_PATH];
    sprintf( temp, "%d.shtml", m_vecNewNum[m_iNextFocusIndex] );

    strWebPage = g_kGlobalValue.GetChannerImgClickURL() + temp;
    g_pBrowser->Navigate( strWebPage.c_str(), "" );
    g_pBrowser->SetEndFunction( CloseNewsPage );
    g_pkUIScene->Stop( true );
}

void KGCNewsFrame::OnClickNewsToDefaultBrowser( void )
{
    if ( m_iCurrentFocusIndex < 0 || m_iCurrentFocusIndex >= m_iMaxNews ) { 
        return;
    }

    PopUpWebPage( L"http://grandchase.gamerage.com/List/0000003010-111_0001_0001.aspx" );

    //RECT rc =
    //{
    //    0, 0, 0, 0
    //};

    //if ( false == g_pBrowser->Create( rc, g_MyD3D->m_hWnd ) ) {
    //    return;
    //}

    //std::ostringstream strmWebPage;
    //strmWebPage << "http://grandchase.gamerage.com/List/0000003010-111_0001_0001.aspx";

    //g_pBrowser->Navigate( strmWebPage.str().c_str(), "", L"_blank" );

    //g_pBrowser->DestroyWin();
    //g_pkUIScene->SetAllStop(false);

    //// 외부 브라우저를 실행한 뒤 GC 창을 최소화 시킨다
    //::SendMessage( g_MyD3D->m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0 );
}
void KGCNewsFrame::NewsBtnInit()
{
    for (int i=0 ; i<MAX_NEWS ; i++)
    {
        if (m_pkbntNews[i])
            m_pkbntNews[i]->InitState(false,true,this);
    }
}
