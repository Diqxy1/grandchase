#include "StdAfx.h"
#include "KGCEventDungeonSeason5.h"

IMPLEMENT_CLASSNAME( KGCEventDungeonSeason5 );
IMPLEMENT_WND_FACTORY( KGCEventDungeonSeason5 );
IMPLEMENT_WND_FACTORY_NAME( KGCEventDungeonSeason5, "gc_event_dungeon_season5_dlg" );

KGCEventDungeonSeason5::KGCEventDungeonSeason5(void)
{
    m_pkCloseBtn = NULL;
    m_pkHeroInfoBtn = NULL;
    m_bAtOnce = true;
    char strWndName[MAX_PATH] = {0,};

    for( int i = 0; i < NUM_EVENT_DUNGEON; ++i )
    {
        m_pkDungeonIconList[i] = NULL;
        sprintf( strWndName, "event_dun_%02d", i );
        LINK_CONTROL(strWndName, m_pkDungeonIconList[i]);
    }
    
    LINK_CONTROL("close_btn", m_pkCloseBtn );
    LINK_CONTROL("hero_info_btn", m_pkHeroInfoBtn);
    m_vecEventDunID.clear();
}

KGCEventDungeonSeason5::~KGCEventDungeonSeason5(void)
{
}

void KGCEventDungeonSeason5::OnCreate( void )
{
    m_pkCloseBtn->InitState( true, true, this );
    m_pkCloseBtn->SetHotKey( DIK_ESCAPE );

    if (m_pkHeroInfoBtn)
    {
        m_pkHeroInfoBtn->InitState(true, true, this);
        m_pkHeroInfoBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCEventDungeonSeason5::OnInformation);
    }

    for( int i = 0; i < NUM_EVENT_DUNGEON; ++i )
    {
        m_pkDungeonIconList[i]->InitState( true, true, this );
		m_pkDungeonIconList[i]->SetWndState(KD3DWnd::D3DWS_DISABLED);
    }

    SetEventDungeonID();
}

void KGCEventDungeonSeason5::OnInformation()
{
    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_KGCHeroDungeonInfoS6, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
}

void KGCEventDungeonSeason5::InitEventDungeonTexCoord()
{
}

void KGCEventDungeonSeason5::GetEventDungeonTexInfo(ED3DWndMode eWndMode, int pos)
{
	if( pos >= NUM_EVENT_DUNGEON )
		return;

	m_pkDungeonIconList[pos]->SetEventDungeonIconTexInfo(eWndMode,m_pkDungeonIconList[pos]->GetEventDunIconTexInfo(eWndMode));
	m_pkDungeonIconList[pos]->SetEventDungeonNameTexInfo(eWndMode,m_pkDungeonIconList[pos]->GetEventDunNameTexInfo(eWndMode));
	m_pkDungeonIconList[pos]->SetEventDungeonBackTexInfo(eWndMode,m_pkDungeonIconList[pos]->GetEventDunBackTexInfo(eWndMode));
}

void KGCEventDungeonSeason5::SetEventDungeonID()
{
    m_vecEventDunID.push_back( std::make_pair(GC_GM_QUEST51,m_pkDungeonIconList[EVENT_NIGHTMARECIRCUS] ));
    m_vecEventDunID.push_back( std::make_pair(GC_GM_QUEST53,m_pkDungeonIconList[EVENT_GHOSTTRAIN] ));
    m_vecEventDunID.push_back( std::make_pair(GC_GM_QUEST54,m_pkDungeonIconList[EVENT_HUNGRYPET] ));
    m_vecEventDunID.push_back( std::make_pair(GC_GM_QUEST57,m_pkDungeonIconList[EVENT_MOONLIGHTYOKAIVILLAGE] ));
    m_vecEventDunID.push_back( std::make_pair(GC_GM_QUEST50,m_pkDungeonIconList[EVENT_ECLIPSESOLAR] ));
    m_vecEventDunID.push_back( std::make_pair(GC_GM_QUEST64,m_pkDungeonIconList[EVENT_REVENGE_OF_TALIN] ));
	m_vecEventDunID.push_back( std::make_pair(GC_GM_QUEST83,m_pkDungeonIconList[EVENT_JUDGEMENT_OF_KING_SLIME] ));
	m_vecEventDunID.push_back( std::make_pair(GC_GM_QUEST92,m_pkDungeonIconList[EVENT_OLDDUNGEON] ));
	m_vecEventDunID.push_back( std::make_pair(GC_GM_QUEST93,m_pkDungeonIconList[EVENT_ATONEMENT] ));
	m_vecEventDunID.push_back( std::make_pair(GC_GM_QUEST94,m_pkDungeonIconList[EVENT_HALLOWEENMANSION] ));
	m_vecEventDunID.push_back( std::make_pair(GC_GM_QUEST95,m_pkDungeonIconList[EVENT_PROJECTMARK] ));
	m_vecEventDunID.push_back( std::make_pair(GC_GM_QUEST97,m_pkDungeonIconList[EVENT_PIXELWORLD] ));
    m_vecEventDunID.push_back(std::make_pair(GC_GM_QUEST101, m_pkDungeonIconList[EVENT_COLISEU]));
    m_vecEventDunID.push_back(std::make_pair(GC_GM_QUEST103, m_pkDungeonIconList[EVENT_MEMORY]));
}

void KGCEventDungeonSeason5::OnEnterScene()
{}
void KGCEventDungeonSeason5::OnCreateComplete( void )
{
	std::vector< EGCGameMode > vecEventDungeon = g_kGlobalValue.GetOpenedEventDungeonInfo();
	std::vector< EGCGameMode >::iterator it = vecEventDungeon.begin();
	std::vector< EGCGameMode >::iterator end = vecEventDungeon.end();

	while( it != end )
	{
		for( int i =  0; i < NUM_EVENT_DUNGEON; i++ )
		{
			if(  m_vecEventDunID[i].first == *it )
			{
				GetEventDungeonTexInfo(D3DWM_DEFAULT, i);
				m_pkDungeonIconList[i]->SetLock( false );
				m_pkDungeonIconList[i]->SetModeID( *it );
			}
			else
			{
				if( m_pkDungeonIconList[i]->GetModeID() == -1 )
				{
					GetEventDungeonTexInfo(D3DWM_LOCK, i);
					m_pkDungeonIconList[i]->SetLock( true );
				}
			}
		}

		++it;
	}
}

void KGCEventDungeonSeason5::OnInitialize()
{
    InitEventDungeonTexCoord();
}
void KGCEventDungeonSeason5::FrameMoveInEnabledState( void )
{
	if( this->IsRenderOn() )
	{
        for( int i = 0; i < NUM_EVENT_DUNGEON; ++i )    
            SetEventDungeonMouseHoverState( (GC_EVENT_DUNGEON)i );

        POINT pt = g_pkInput->GetMousePos();
        D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

		for( int i =  0; i < NUM_EVENT_DUNGEON; i++ )
		{
			GetEventDungeonTexInfo(KD3DWnd::D3DWM_DEFAULT, i);
			
            if ( m_pkDungeonIconList[i]->CheckPosInWindowBound( vMousePos ) && !m_pkDungeonIconList[i]->IsLock() )
            {
                SetEventDungeonMouseHoverState( (GC_EVENT_DUNGEON)i );
                m_pkDungeonIconList[i]->SetWndHover();
            }

			if( m_pkDungeonIconList[i]->IsLock() || !m_pkDungeonIconList[i]->GetAvaliableDay() )
			{
				GetEventDungeonTexInfo(KD3DWnd::D3DWM_LOCK, i);
			}
		}
	}
}

void KGCEventDungeonSeason5::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkCloseBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnExit );
	if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE )
	{
		OnExit();
	}
}
void KGCEventDungeonSeason5::OnExit()
{
    this->ToggleRender( false );
    g_pkUIScene->EndMessageBox();
}

void KGCEventDungeonSeason5::SetEventDungeonMouseHoverState( GC_EVENT_DUNGEON iSelectedDungeon )
{
    m_eSelectedEventDungeon = iSelectedDungeon;
    SetEnterInfoToolTip( m_vecEventDunID[m_eSelectedEventDungeon].first );
}

void KGCEventDungeonSeason5::SetEnterInfoToolTip( EGCGameMode _eGameMode )
{
    if( m_eSelectedEventDungeon == NUM_EVENT_DUNGEON )
        return;

    if ( !m_pkDungeonIconList[m_eSelectedEventDungeon] )
        return;

    if( g_kGlobalValue.IsOpenDayEventDungeon( _eGameMode ) )
    {
        m_pkDungeonIconList[m_eSelectedEventDungeon]->ClearToolTip();
        m_pkDungeonIconList[ m_eSelectedEventDungeon ]->SetAvaliableDay( true );
    }
    else
    {
        std::wstring tooltipText = g_kGlobalValue.GetEventDungeonOpenDayString( _eGameMode );
        if( tooltipText != L"" )
            m_pkDungeonIconList[ m_eSelectedEventDungeon ]->SetToolTip( tooltipText );
        m_pkDungeonIconList[ m_eSelectedEventDungeon ]->SetAvaliableDay( false );
    }
}

IMPLEMENT_CLASSNAME( KGCEventDungeonSeason5Icon );
IMPLEMENT_WND_FACTORY( KGCEventDungeonSeason5Icon );
IMPLEMENT_WND_FACTORY_NAME( KGCEventDungeonSeason5Icon, "gc_event_dungeon_icon_dlg" );

KGCEventDungeonSeason5Icon::KGCEventDungeonSeason5Icon(void)
{
    m_pkDunback = NULL;
    m_pkDunIcon = NULL;
    m_pkDunName = NULL;

    m_iModeID = -1;
	m_bLock = false;

    m_sEventDunIconTexInfo.clear();
    m_sEventDunNameTexInfo.clear();
    m_sEventDunBackTexInfo.clear();

    LINK_CONTROL( "dun_back",        m_pkDunback );
    LINK_CONTROL( "dun_icon",        m_pkDunIcon );
    LINK_CONTROL( "dun_name",        m_pkDunName );
}

KGCEventDungeonSeason5Icon::~KGCEventDungeonSeason5Icon(void)
{
}

void KGCEventDungeonSeason5Icon::OnCreate( void )
{
    //m_iModeID = ; //���⼭ ������ ��� ���̵� �� �� �ִ� ����� ã�ƺ���.
    m_pkDunback->InitState( true, true, this);
    m_pkDunIcon->InitState( true, true, this);
    m_pkDunName->InitState( true, true, this);
    
    SetEventDungeonTexInfo();
}

void KGCEventDungeonSeason5Icon::SetAvaliableDay(bool dun)
{
    m_bAvailableDay = dun;
}

bool KGCEventDungeonSeason5Icon::GetAvaliableDay()
{
    return m_bAvailableDay;
}

void KGCEventDungeonSeason5Icon::SetEventDungeonTexInfo()
{
    sEventDungeonTexInfo sEventDunTexInfo;
    sEventDunTexInfo.clear();
    
    //������ �ؽ��� ������ �޾ƿ�
    sEventDunTexInfo.iTexNum        = m_pkDunIcon->GetWndTexture(D3DWM_DEFAULT);
    sEventDunTexInfo.dxVecTexCoord  = m_pkDunIcon->GetWndTexCoords(D3DWM_DEFAULT);
    m_sEventDunIconTexInfo.push_back( std::make_pair(D3DWM_DEFAULT,sEventDunTexInfo ) );
    sEventDunTexInfo.clear();

    sEventDunTexInfo.iTexNum        = m_pkDunIcon->GetWndTexture(D3DWM_HOVER);
    sEventDunTexInfo.dxVecTexCoord  = m_pkDunIcon->GetWndTexCoords(D3DWM_HOVER);
    m_sEventDunIconTexInfo.push_back( std::make_pair(D3DWM_HOVER,sEventDunTexInfo ) );
    sEventDunTexInfo.clear();
    
    sEventDunTexInfo.iTexNum        = m_pkDunIcon->GetWndTexture(D3DWM_LOCK);
    sEventDunTexInfo.dxVecTexCoord  = m_pkDunIcon->GetWndTexCoords(D3DWM_LOCK);
    m_sEventDunIconTexInfo.push_back( std::make_pair(D3DWM_LOCK,sEventDunTexInfo ) );
    sEventDunTexInfo.clear();



    //��� �ؽ��� ������ �޾ƿ�
    sEventDunTexInfo.iTexNum        = m_pkDunback->GetWndTexture(D3DWM_DEFAULT);
    sEventDunTexInfo.dxVecTexCoord  = m_pkDunback->GetWndTexCoords(D3DWM_DEFAULT);
    m_sEventDunBackTexInfo.push_back( std::make_pair(D3DWM_DEFAULT,sEventDunTexInfo ) );
    sEventDunTexInfo.clear();

    sEventDunTexInfo.iTexNum        = m_pkDunback->GetWndTexture(D3DWM_HOVER);
    sEventDunTexInfo.dxVecTexCoord  = m_pkDunback->GetWndTexCoords(D3DWM_HOVER);
    m_sEventDunBackTexInfo.push_back( std::make_pair(D3DWM_HOVER,sEventDunTexInfo ) );
    sEventDunTexInfo.clear();

    sEventDunTexInfo.iTexNum        = m_pkDunback->GetWndTexture(D3DWM_LOCK);
    sEventDunTexInfo.dxVecTexCoord  = m_pkDunback->GetWndTexCoords(D3DWM_LOCK);
    m_sEventDunBackTexInfo.push_back( std::make_pair(D3DWM_LOCK,sEventDunTexInfo ) );
    sEventDunTexInfo.clear();

    //�̸� �ؽ��� ������ �޾ƿ�
    sEventDunTexInfo.iTexNum        = m_pkDunName->GetWndTexture(D3DWM_DEFAULT);
    sEventDunTexInfo.dxVecTexCoord  = m_pkDunName->GetWndTexCoords(D3DWM_DEFAULT);
    m_sEventDunNameTexInfo.push_back( std::make_pair(D3DWM_DEFAULT,sEventDunTexInfo ) );
    sEventDunTexInfo.clear();
    
    sEventDunTexInfo.iTexNum        = m_pkDunName->GetWndTexture(D3DWM_HOVER);
    sEventDunTexInfo.dxVecTexCoord  = m_pkDunName->GetWndTexCoords(D3DWM_HOVER);
    m_sEventDunNameTexInfo.push_back( std::make_pair(D3DWM_HOVER,sEventDunTexInfo ) );
    sEventDunTexInfo.clear();

    sEventDunTexInfo.iTexNum        = m_pkDunName->GetWndTexture(D3DWM_LOCK);
    sEventDunTexInfo.dxVecTexCoord  = m_pkDunName->GetWndTexCoords(D3DWM_LOCK);
    m_sEventDunNameTexInfo.push_back( std::make_pair(D3DWM_LOCK,sEventDunTexInfo ) );
    sEventDunTexInfo.clear();


}
void KGCEventDungeonSeason5Icon::SetModeID( int imodeid)
{
    m_iModeID = imodeid;
}
int KGCEventDungeonSeason5Icon::GetModeID( )
{
    return m_iModeID; 
}

void KGCEventDungeonSeason5Icon::SetEventDungeonIconTexInfo( ED3DWndMode eWndMode, sEventDungeonTexInfo sTexinfo )
{
	m_pkDunIcon->SetWndTex(eWndMode,sTexinfo.iTexNum);
	m_pkDunIcon->SetWndTexCoords(eWndMode,sTexinfo.dxVecTexCoord);
}

void KGCEventDungeonSeason5Icon::SetEventDungeonNameTexInfo( ED3DWndMode eWndMode, sEventDungeonTexInfo sTexinfo )
{
	m_pkDunName->SetWndTex(eWndMode,sTexinfo.iTexNum);
    m_pkDunName->SetWndTexCoords(eWndMode,sTexinfo.dxVecTexCoord);
}

void KGCEventDungeonSeason5Icon::SetEventDungeonBackTexInfo(ED3DWndMode eWndMode, sEventDungeonTexInfo sTexinfo )
{
    m_pkDunback->SetWndTex(eWndMode,sTexinfo.iTexNum);
    m_pkDunback->SetWndTexCoords(eWndMode,sTexinfo.dxVecTexCoord);
}

sEventDungeonTexInfo KGCEventDungeonSeason5Icon::GetEventDunIconTexInfo( ED3DWndMode eWndMode )
{
    sEventDungeonTexInfo sTexInfo;
    std::vector<std::pair<int,sEventDungeonTexInfo>>::iterator itr = m_sEventDunIconTexInfo.begin();
    
    for( ; itr != m_sEventDunIconTexInfo.end(); ++itr )
    {
        if( (ED3DWndMode)(*itr).first == eWndMode )
        {
            sTexInfo = (*itr).second;
        }
    }
    return sTexInfo;

}
sEventDungeonTexInfo KGCEventDungeonSeason5Icon::GetEventDunNameTexInfo(ED3DWndMode eWndMode )
{
    sEventDungeonTexInfo sTexInfo;
    std::vector<std::pair<int,sEventDungeonTexInfo>>::iterator itr = m_sEventDunNameTexInfo.begin();

    for( ; itr != m_sEventDunNameTexInfo.end(); ++itr )
    {
        if( (ED3DWndMode)(*itr).first == eWndMode )
        {
            sTexInfo = (*itr).second;
        }
    }
    return sTexInfo;
}

sEventDungeonTexInfo KGCEventDungeonSeason5Icon::GetEventDunBackTexInfo(ED3DWndMode eWndMode )
{
    sEventDungeonTexInfo sTexInfo;
    std::vector<std::pair<int,sEventDungeonTexInfo>>::iterator itr = m_sEventDunBackTexInfo.begin();

    for( ; itr != m_sEventDunBackTexInfo.end(); ++itr )
    {
        if( (ED3DWndMode)(*itr).first == eWndMode )
        {
            sTexInfo = (*itr).second;
        }
    }

    return sTexInfo;
}

void KGCEventDungeonSeason5Icon::OnCreateComplete( void )
{
}

void KGCEventDungeonSeason5Icon::SetWndHover()
{
    m_pkDunIcon->SetWndMode(D3DWM_HOVER);
    m_pkDunName->SetWndMode(D3DWM_HOVER);
    m_pkDunback->SetWndMode(D3DWM_HOVER);
}

void KGCEventDungeonSeason5Icon::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == D3DWE_BUTTON_CLICK )
	{
		if( m_bLock == false && GetAvaliableDay() )
		{
			SiKGCWorldMapManager()->SelectIcon(GetModeID());
			g_KDSound.Play( "31" );
			SpeakToActionListener( KActionEvent( g_pkUIScene->m_pkEventDungeonSeason5, KD3DWnd::EWNDMESSAGE_CLOSE ) );
		}
	}
}
void KGCEventDungeonSeason5Icon::FrameMoveInEnabledState( void )
{
}
