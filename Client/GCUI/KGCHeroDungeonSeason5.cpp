#include "StdAfx.h"
#include "KGCHeroDungeonSeason5.h"

IMPLEMENT_CLASSNAME( KGCHeroDungeonSeason5 );
IMPLEMENT_WND_FACTORY( KGCHeroDungeonSeason5 );
IMPLEMENT_WND_FACTORY_NAME( KGCHeroDungeonSeason5, "gc_hero_dungeon_season5_dlg" );

KGCHeroDungeonSeason5::KGCHeroDungeonSeason5(void)
{
    m_pkCloseBtn = NULL;
    m_iRemainNextEnteranceTime = -1;
    m_iRemainCloseTime = -1;
    m_bOpenTime = false; 
    m_bAvailableTime = false;
    m_bAvailableLevel= false;
    m_vecHeroDunID.clear();
    m_eSelectedHeroDungeon = NUM_HERO_DUNGEON;
    char strWndName[MAX_PATH] = {0,};
    
    for( int i = 0; i < NUM_HERO_DUNGEON; ++i )
    {
        m_pkDungeonIconList[i] = NULL;
        sprintf( strWndName, "Hero_dun_%02d", i );
        LINK_CONTROL(strWndName, m_pkDungeonIconList[i]);
    }
    LINK_CONTROL("close_btn", m_pkCloseBtn );
}

KGCHeroDungeonSeason5::~KGCHeroDungeonSeason5(void)
{
}

void KGCHeroDungeonSeason5::OnCreate( void )
{
    m_pkCloseBtn->InitState( true, true, this );
    m_pkCloseBtn->SetHotKey( DIK_ESCAPE );

    for( int i = 0; i < NUM_HERO_DUNGEON; ++i )
    {
        if ( m_pkDungeonIconList[i] )
        {
            m_pkDungeonIconList[i]->InitState( false, true, this );
			m_pkDungeonIconList[i]->SetWndLock();
        }
    }
    SetHeroDungeonID();   
}

void KGCHeroDungeonSeason5::OnEnterScene()
{
}

void KGCHeroDungeonSeason5::OnCreateComplete( void )
{	
	std::vector< EGCGameMode > vecGameMode = SiKGCRoomManager()->GetCheckDungeonGameMode();

	for( int i =  0; i < NUM_HERO_DUNGEON; i++ )
	{
		if( std::find( vecGameMode.begin(), vecGameMode.end(), m_vecHeroDunID[i].second ) != vecGameMode.end())
		{
			m_pkDungeonIconList[i]->ToggleRender( true );
			m_pkDungeonIconList[i]->SetWndDefault();
		}
	}
}

void KGCHeroDungeonSeason5::OnInitialize( void )
{
    for( int i = 0; i < NUM_HERO_DUNGEON; ++i )
    {
        if ( m_pkDungeonIconList[i] )
            m_pkDungeonIconList[i]->SetWndDefault();
        SetHeroDungeonMouseHoverState( (GC_HERO_DUNGEON)i );
    }
}

void KGCHeroDungeonSeason5::FrameMoveInEnabledState( void )
{
    if( g_MyD3D->m_pStateMachine->GetState() != GS_WORLDMAP )
    {
        this->ToggleRender( false );
        return; 
    }
    for( int i = 0; i < NUM_HERO_DUNGEON; ++i )    
        SetHeroDungeonMouseHoverState( (GC_HERO_DUNGEON)i );

    for( int i = 0; i < NUM_HERO_DUNGEON; ++i )
    {
        if ( !m_pkDungeonIconList[i] )
            continue;

        
        if( m_pkDungeonIconList[i]->GetAvaliableTime() && m_pkDungeonIconList[i]->GetAvaliableLevel() )
        {
            m_pkDungeonIconList[i]->SetRenderDungeonLimitLevel(false);
            m_pkDungeonIconList[i]->SetWndDefault();
            //m_pkDungeonIconList[i]->SetWndHover();
            POINT pt = g_pkInput->GetMousePos();
            D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
            if( m_pkDungeonIconList[i]->CheckPosInWindowBound( vMousePos ) )
            {
                m_pkDungeonIconList[i]->SetWndHover();
            }
        }
        else
        {
			m_pkDungeonIconList[i]->SetWndLock();
			if( !m_pkDungeonIconList[i]->GetAvaliableLevel() )
				m_pkDungeonIconList[i]->SetRenderDungeonLimitLevel(true);
			else 
				 m_pkDungeonIconList[i]->SetRenderDungeonLimitLevel(false);
        }
    }

    SetEnterSelectedHeroDungeon();
}

bool KGCHeroDungeonSeason5::IsAvailableHeroDungeon()
{
    return m_bAvailableTime && m_bAvailableLevel;
}

void KGCHeroDungeonSeason5::SetHeroDungeonID()
{
    //시간날때 리팩토링 할 것 스크립트로 영웅던전과 이벤트던전 관련 내용을 스크립트로 빼든지 해야겠음
    m_vecHeroDunID.push_back( std::make_pair(HERO_FURNACE_OF_HELL, GC_GM_QUEST47));
    m_vecHeroDunID.push_back( std::make_pair(HERO_SANCTUM_OF_DESTRUCTION, GC_GM_QUEST46));
    m_vecHeroDunID.push_back( std::make_pair(HERO_PHANTASMICSECTOR, GC_GM_QUEST52));
    m_vecHeroDunID.push_back( std::make_pair(HERO_VERKAS, GC_GM_QUEST58));
    m_vecHeroDunID.push_back( std::make_pair(HERO_EXTINCTION, GC_GM_QUEST81));
	m_vecHeroDunID.push_back( std::make_pair(HERO_HENIR, GC_GM_QUEST98));
    m_vecHeroDunID.push_back(std::make_pair(HER0_BERCO_DA_ORIGEM, GC_GM_QUEST102));
    m_vecHeroDunID.push_back(std::make_pair(HERO_TIME_DISPLACEMENT, GC_GM_QUEST104));
}

void KGCHeroDungeonSeason5::SetEnterSelectedHeroDungeon()
{
    if( g_pkInput->BtnUp(KInput::MBLEFT) )
    {
        for( int i = 0; i < NUM_HERO_DUNGEON; ++i )
        {
            POINT pt = g_pkInput->GetMousePos();
            D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

            if ( !m_pkDungeonIconList[i] || !m_pkDungeonIconList[i]->IsRenderOn() )
                continue;

            //클릭 했을때, 현재 마우스 위치가 영웅던전 안이면
            if( m_pkDungeonIconList[i]->CheckPosInWindowBound( vMousePos ) && ( m_pkDungeonIconList[i]->GetIconMode() != KD3DWnd::D3DWM_LOCK || g_kGlobalValue.IsAdmin() ) )
            {
                std::vector< std::pair< GC_HERO_DUNGEON,EGCGameMode> >::iterator vit;
                for( vit = m_vecHeroDunID.begin(); vit != m_vecHeroDunID.end(); ++vit )
                {
                    if( (*vit).first == i )
                        SiKGCWorldMapManager()->SelectIcon((*vit).second);
                }
                this->ToggleRender( false );
                g_pkUIScene->EndMessageBox();
            }
        }
    }
}

void KGCHeroDungeonSeason5::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkCloseBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnExit );

    for( int i = 0; i < NUM_HERO_DUNGEON; ++i )
    {
        if( event.m_pWnd == m_pkDungeonIconList[i] && event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
        {
            SetHeroDungeonMouseHoverState( (GC_HERO_DUNGEON)i );
            m_pkDungeonIconList[i]->SetWndHover();
        } 
    }
}

void KGCHeroDungeonSeason5::OnExit()
{
    this->ToggleRender( false );
    g_pkUIScene->EndMessageBox();
}

void KGCHeroDungeonSeason5::SetHeroDungeonMouseHoverState( GC_HERO_DUNGEON iSelectedDungeon )
{
    m_eSelectedHeroDungeon = iSelectedDungeon;
    
    std::vector< std::pair< GC_HERO_DUNGEON,EGCGameMode> >::iterator vit;
    
    for( vit = m_vecHeroDunID.begin(); vit != m_vecHeroDunID.end(); ++vit )
    {
        if( (*vit).first == m_eSelectedHeroDungeon )
        {
            SetEnterInfoToolTip((*vit).second);
            SetLimitInfo((*vit).second);
        }
    }
}

void KGCHeroDungeonSeason5::SetEnterInfoToolTip(EGCGameMode _eGameMode)
{
    if( m_eSelectedHeroDungeon == NUM_HERO_DUNGEON )
        return;

    if ( !m_pkDungeonIconList[m_eSelectedHeroDungeon] )
        return;
    
    std::pair<int, int> pairClearInfo = g_kGlobalValue.m_kHeroDungeonInfo.GetClearInfo(_eGameMode);

    std::wstring  strRemainTime;
    if( IsOpenTimeHeroDungeon(_eGameMode) )
    {
        m_pkDungeonIconList[m_eSelectedHeroDungeon]->SetAvaliableTime( true );
        m_bAvailableTime = true;
    }
    else
    {
        strRemainTime = g_pkStrLoader->GetReplacedString(STR_ID_TOOLTIP_HERODUNGEON_REMAIN_STARTTIME,"iii", GetNextEnteranceRemainTime(), pairClearInfo.first, pairClearInfo.second );
        m_pkDungeonIconList[m_eSelectedHeroDungeon]->SetToolTip(strRemainTime);
        m_pkDungeonIconList[m_eSelectedHeroDungeon]->SetAvaliableTime( false );
        m_bAvailableTime = false;
    }
}

void KGCHeroDungeonSeason5::SetLimitInfo(EGCGameMode _eGameMode)
{
    if( m_eSelectedHeroDungeon == NUM_HERO_DUNGEON )
        return;

    if ( !m_pkDungeonIconList[m_eSelectedHeroDungeon] )
        return;

    PLAYER* pPlayer = NULL;
    pPlayer = &g_MyD3D->m_TempPlayer;
    int iCurrentCharLevel = pPlayer->GetCurrentCharLevel();

    QuestFileInfo kQuestFileInfo = g_MyD3D->m_pkQuestGameMgr->GetQuestFileInfoFromGameMode( _eGameMode, 0 );
    if( kQuestFileInfo.iLowLevel <= iCurrentCharLevel )    
    {
        m_bAvailableLevel = true;
        m_pkDungeonIconList[m_eSelectedHeroDungeon]->SetAvaliableLevel( true );
        return;
    }
    else
    {
        m_pkDungeonIconList[m_eSelectedHeroDungeon]->SetRenderDungeonLimitLevel(true);
        m_pkDungeonIconList[m_eSelectedHeroDungeon]->SetAvaliableLevel( false );
        m_bAvailableLevel = false;
    }
    
    m_pkDungeonIconList[m_eSelectedHeroDungeon]->SetRenderDungeonLimitLevel(true);
    m_pkDungeonIconList[m_eSelectedHeroDungeon]->SetAvaliableLevel( false );
    m_bAvailableLevel = false;
}

bool KGCHeroDungeonSeason5::IsOpenTimeHeroDungeon(EGCGameMode _eGameMode )
{
	return true;
}

int KGCHeroDungeonSeason5::GetCloseHeroDungeonRemainTime()
{
    return m_iRemainCloseTime;
}

int KGCHeroDungeonSeason5::GetNextEnteranceRemainTime()
{
   return m_iRemainNextEnteranceTime;
}

IMPLEMENT_CLASSNAME( KGCHeroDungeonSeason5Icon );
IMPLEMENT_WND_FACTORY( KGCHeroDungeonSeason5Icon );
IMPLEMENT_WND_FACTORY_NAME( KGCHeroDungeonSeason5Icon, "gc_hero_dungeon_icon_dlg" );

KGCHeroDungeonSeason5Icon::KGCHeroDungeonSeason5Icon(void)
{
    m_pkDunback = NULL;
    m_pkDunIcon = NULL;
    m_pkDunName = NULL;
    m_pkDunLimitLevel = NULL;
    m_pkDunDummyBound = NULL;
    m_iModeID = -1;
    m_bAvailableTime = false;
    m_bAvailableLevel = false;

    LINK_CONTROL( "dun_back",        m_pkDunback );
    LINK_CONTROL( "dun_icon",        m_pkDunIcon );
    LINK_CONTROL( "dun_name",        m_pkDunName );
    LINK_CONTROL( "dun_limit_level", m_pkDunLimitLevel );
}

KGCHeroDungeonSeason5Icon::~KGCHeroDungeonSeason5Icon(void)
{
}

void KGCHeroDungeonSeason5Icon::SetAvaliableTime(bool dun)
{
    m_bAvailableTime = dun;
}
bool KGCHeroDungeonSeason5Icon::GetAvaliableTime()
{
    return m_bAvailableTime;
}

void KGCHeroDungeonSeason5Icon::SetAvaliableLevel(bool dun)
{
    m_bAvailableLevel = dun;
}

bool KGCHeroDungeonSeason5Icon::GetAvaliableLevel()
{
    return m_bAvailableLevel;
}

void KGCHeroDungeonSeason5Icon::OnCreate( void )
{
    m_pkDunback->InitState( true, true, this);
    m_pkDunIcon->InitState( true, true, this);
    m_pkDunName->InitState( true, true, this);
    m_pkDunLimitLevel->InitState( false, true, this);
}

void KGCHeroDungeonSeason5Icon::SetDungeonNameText( const std::wstring& strText )
{
}

void KGCHeroDungeonSeason5Icon::SetRenderDungeonLimitLevel( bool bRender)
{
    m_pkDunLimitLevel->ToggleRender( bRender );
}

void KGCHeroDungeonSeason5Icon::OnCreateComplete( void )
{
}

void KGCHeroDungeonSeason5Icon::SetWndDefault()
{
    m_pkDunIcon->SetWndMode(D3DWM_DEFAULT);
    m_pkDunback->SetWndMode(D3DWM_DEFAULT);
    m_pkDunName->SetWndMode(D3DWM_DEFAULT);
}

void KGCHeroDungeonSeason5Icon::SetWndHover()
{
    m_pkDunIcon->SetWndMode(D3DWM_HOVER);
    m_pkDunback->SetWndMode(D3DWM_HOVER);
    m_pkDunName->SetWndMode(D3DWM_HOVER);
}

void KGCHeroDungeonSeason5Icon::SetWndLock()
{
    m_pkDunIcon->SetWndMode(D3DWM_LOCK);
    m_pkDunback->SetWndMode(D3DWM_LOCK);
    m_pkDunName->SetWndMode(D3DWM_LOCK);
}

KD3DWnd::ED3DWndMode KGCHeroDungeonSeason5Icon::GetIconMode( void )
{
    if( m_pkDunIcon->GetWndMode() == KD3DWnd::D3DWM_DEFAULT && 
        m_pkDunback->GetWndMode() == KD3DWnd::D3DWM_DEFAULT &&
        m_pkDunName->GetWndMode() == KD3DWnd::D3DWM_DEFAULT ) { 
        return KD3DWnd::D3DWM_DEFAULT;
    }

    if( m_pkDunIcon->GetWndMode() == KD3DWnd::D3DWM_HOVER && 
        m_pkDunback->GetWndMode() == KD3DWnd::D3DWM_HOVER &&
        m_pkDunName->GetWndMode() == KD3DWnd::D3DWM_HOVER ) { 
        return KD3DWnd::D3DWM_HOVER;
    }

    if( m_pkDunIcon->GetWndMode() == KD3DWnd::D3DWM_LOCK && 
        m_pkDunback->GetWndMode() == KD3DWnd::D3DWM_LOCK &&
        m_pkDunName->GetWndMode() == KD3DWnd::D3DWM_LOCK ) { 
        return KD3DWnd::D3DWM_LOCK;
    }

    return KD3DWnd::D3DWM_DEFAULT;
}

void KGCHeroDungeonSeason5Icon::ActionPerformed( const KActionEvent& event )
{
}

void KGCHeroDungeonSeason5Icon::FrameMoveInEnabledState( void )
{
}