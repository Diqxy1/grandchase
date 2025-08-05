#include "stdafx.h"
#include "KGCTagMatchSelect.h"

//

#include "../MyD3D.h"
//


#include "KGCRoomManager.h"

IMPLEMENT_CLASSNAME( KGCTagMatchSelect );
IMPLEMENT_WND_FACTORY( KGCTagMatchSelect );
IMPLEMENT_WND_FACTORY_NAME( KGCTagMatchSelect, "gc_tag_match_select" );

KGCTagMatchSelect* g_pTagMatchSelect = NULL;

KGCTagMatchSelect::KGCTagMatchSelect(void)
{
	g_pTagMatchSelect = this;

	char strWnd[MAX_PATH];

	for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
	{
		if ( ProhibitChar(i) )
            continue;

        m_ppk1stCharFace[i] = NULL;
        m_ppk2ndCharFace[i] = NULL;
        m_ppkDisableChar[i] = NULL;
		m_ppk1stChar[i] = NULL;
        m_ppk2ndChar[i] = NULL;

		sprintf( strWnd, "char_face%d", i+1 );
		LINK_CONTROL( strWnd, m_ppk1stCharFace[i] );

        sprintf( strWnd, "char_face%d_2nd", i+1 );
        LINK_CONTROL( strWnd, m_ppk2ndCharFace[i] );

        sprintf( strWnd, "cand_char%d_disable", i+1 );
        LINK_CONTROL( strWnd, m_ppkDisableChar[i] );

		sprintf( strWnd, "cand_char%d", i+1 );
		LINK_CONTROL( strWnd, m_ppk1stChar[i] );

        sprintf( strWnd, "cand_char%d_2nd", i+1 );
        LINK_CONTROL( strWnd, m_ppk2ndChar[i] );
	}

	for( int i = 0 ; i < TAG_CHAR_NUM ; ++i )
	{
		m_ppkSelectChar[i] = NULL;	

		sprintf( strWnd, "select_char%d", i+1 );
		LINK_CONTROL( strWnd, m_ppkSelectChar[i] );
	}

    m_bIsReadyAvailable = false;

    m_pkModeTeam = NULL;
    m_pkModeSurvival = NULL;
    LINK_CONTROL("tag_game_mode_team", m_pkModeTeam );
    LINK_CONTROL("tag_game_mode_survival", m_pkModeSurvival );

    m_pkBtnRight = NULL;
    m_pkBtnLeft = NULL;
    LINK_CONTROL( "btn_right", m_pkBtnRight );
    LINK_CONTROL( "btn_left", m_pkBtnLeft );

    InitNowVisible();
}

KGCTagMatchSelect::~KGCTagMatchSelect(void)
{
}

void KGCTagMatchSelect::CandidateVisible()
{
    for( int i = 0; i < GC_CHAR_NUM; ++i )
    {
        if (m_ppk1stChar[i] == NULL || m_ppk2ndChar[i] == NULL || m_ppkDisableChar[i] == NULL)
            continue;
        
        if (  ProhibitChar(i) )
            continue;

        m_ppk1stChar[i]->ToggleRender( false );
        m_ppk2ndChar[i]->ToggleRender( false );
        m_ppkDisableChar[i]->ToggleRender( false );
    }

    for( int i = 0; i < (int)m_vecNowVisible.size(); ++i )
    {
        if ( (int)m_vecChar.size() <= i ) 
            continue;

        if ( ProhibitChar( m_vecChar[m_vecNowVisible[i]].iCharType ))
            continue;

        m_ppkDisableChar[ m_vecChar[m_vecNowVisible[i]].iCharType ]->ToggleRender(true);
        m_ppkDisableChar[ m_vecChar[m_vecNowVisible[i]].iCharType ]->SetAllWindowPos(m_vecPos[i]);
        m_ppk1stChar[ m_vecChar[m_vecNowVisible[i]].iCharType ]->ToggleRender(true);
        m_ppk1stChar[ m_vecChar[m_vecNowVisible[i]].iCharType ]->SetAllWindowPos(m_vecPos[i]);
        m_ppk2ndChar[ m_vecChar[m_vecNowVisible[i]].iCharType ]->ToggleRender(true);
        m_ppk2ndChar[ m_vecChar[m_vecNowVisible[i]].iCharType ]->SetAllWindowPos(m_vecPos[i]);
    }
}

void KGCTagMatchSelect::InitNowVisible()
{
    m_iCurSelectIndex = 0;
    m_vecNowVisible.clear();
    m_vecNowVisible.push_back( 0 );
    m_vecNowVisible.push_back( 1 );
    m_vecNowVisible.push_back( 2 );
}

void KGCTagMatchSelect::Init()
{
    static bool bFirst = true;
    if( bFirst )
    {
        bFirst = false;
        for( int i = 0; i < 3; ++i )
        {
            m_vecPos[i] = m_ppk1stChar[i]->GetFixedWindowLocalPos();
        }
    }

	m_vecChar = g_kGlobalValue.m_kUserInfo.vecCharInfo;
	
    m_iCharNum = (int)g_kGlobalValue.m_kUserInfo.vecCharInfo.size();
    int iCount = 0;
    for (int i =0; i < m_iCharNum ; ++i)
    {
        if ( ProhibitChar( m_vecChar[i].iCharType ) ){
            ++iCount;
        }
    }
    m_iCharNum = m_iCharNum - iCount;

	for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
	{
        if (m_ppk1stCharFace[i] == NULL ||
            m_ppk2ndCharFace[i] == NULL || 
            m_ppk1stChar[i] == NULL || 
            m_ppk2ndChar[i] == NULL)
            continue;
        
        if ( ProhibitChar(i) )
            continue;

		m_ppk1stCharFace[i]->ToggleRender( false );
        m_ppk2ndCharFace[i]->ToggleRender( false );
		m_ppk1stChar[i]->Lock( false );
		m_ppk1stChar[i]->InitState( false, true, this );
        m_ppk2ndChar[i]->Lock( false );
        m_ppk2ndChar[i]->InitState( false, true, this );
	}

    for( int i = 0 ; i < TAG_CHAR_NUM ; ++i )
		m_ppkSelectChar[i]->InitState( false, false, NULL );

    for( int i = 0 ; i < (int)m_pCharSlot.size() ; ++i )
    {
        m_pCharSlot[i] = -1;
    }

    m_pkModeTeam->InitState( true, true, this );
    m_pkModeSurvival->InitState( true, true, this );
    m_pkModeTeam->Activate( true );
    m_pkModeSurvival->Activate( false );

    m_pkBtnRight->InitState( true, true, this );
    m_pkBtnLeft->InitState( true, true, this );

    InitNowVisible();
    CandidateVisible();
}

void KGCTagMatchSelect::OnCreate( void )
{
}

void KGCTagMatchSelect::SelectNowVisible( int iSelectIndex, bool bIncrease )
{
    m_vecNowVisible.clear();
    
    int iCharNum =0;
    int iCount = 0;
    iCharNum = static_cast<int>(m_vecChar.size());
    for (int i =0; i < iCharNum ; ++i)
    {
        if ( ProhibitChar( m_vecChar[i].iCharType ) ) {
            ++iCount;
        }
    }
    iCharNum = iCharNum - iCount;

    if( 3 == (int)m_vecChar.size() )
    {
        iSelectIndex = 0;
    }

    if( bIncrease )
    {
        if( ++iSelectIndex == iCharNum )
            iSelectIndex = 0;
    }
    else
    {
        if( --iSelectIndex == -1 )
            iSelectIndex = iCharNum - 1;
    }
    m_vecNowVisible.push_back( iSelectIndex );

    for( int i = 0; i < 2; ++i )
    {
        if( ++iSelectIndex == iCharNum )
            iSelectIndex = 0;

        m_vecNowVisible.push_back( iSelectIndex );
    }

    CandidateVisible();
}

void KGCTagMatchSelect::ActionPerformed( const KActionEvent& event )
{
    if( g_MyD3D->IsLocalPlayerObserver() )
        return;

	for( int i = 0 ; i < m_iCharNum ; ++i )
	{
        if ( ProhibitChar( i ) )
            continue;

        if( (m_ppk1stChar[m_vecChar[i].iCharType]->IsRenderOn() && event.m_pWnd == m_ppk1stChar[m_vecChar[i].iCharType]) || 
            (m_ppk2ndChar[m_vecChar[i].iCharType]->IsRenderOn() && event.m_pWnd == m_ppk2ndChar[m_vecChar[i].iCharType]) )
		{
			OnCandChar( m_vecChar[i].iCharType, event.m_dwCode );
			break;
		}
	}

	for( int i = 0 ; i < TAG_CHAR_NUM ; ++i )
	{
		if( event.m_pWnd == m_ppkSelectChar[i] )
		{
			OnSelectChar( m_vecChar[i].iCharType, event.m_dwCode );
			break;
		}
	}

    if( event.m_pWnd == m_pkBtnRight && event.m_dwCode == D3DWE_BUTTON_CLICK )
    {
        SelectNowVisible( m_iCurSelectIndex, true );
        if( ++m_iCurSelectIndex == m_iCharNum )
            m_iCurSelectIndex = 0;
    }
    if( event.m_pWnd == m_pkBtnLeft && event.m_dwCode == D3DWE_BUTTON_CLICK )
    {
        SelectNowVisible( m_iCurSelectIndex, false );
        if( --m_iCurSelectIndex == -1 )
            m_iCurSelectIndex = m_iCharNum - 1;
    }

    if ( g_kGlobalValue.m_kUserInfo.bHost ) // 방장인 경우만 조작 가능
    {
#if !defined( NATION_USA ) && !defined( NATION_EU ) && !defined( NATION_KOREA )
        if( g_kGlobalValue.ServerInfo.CheckServerType( ST_PVP_BALANCE ) )
        {
            // 방장인 내가 아닌 옵저버가 있다면 방장 기능을 상실한다.
            if( SiKGCRoomManager()->GetRoomInfo().IsExistObserver() && !g_MyD3D->IsLocalPlayerObserver() )
                return;
        }        
#endif

        if( event.m_pWnd == m_pkModeTeam || event.m_pWnd == m_pkModeSurvival )
        {
            switch ( event.m_dwCode )
            {
                case KD3DWnd::D3DWE_CURSOR_ENTER:
                {
                    g_KDSound.Play( "30" );
                    break;
                }
                case KD3DWnd::D3DWE_BUTTON_DOWN:
                {
                    g_KDSound.Play( "31" );
                    break;
                }
                case KD3DWnd::D3DWE_BUTTON_CLICK:
                {
                    std::vector<std::pair<char,bool> > vecChangeSlot;
                    for ( char c = 0 ; c < MAX_PLAYER_NUM; c++ )
                    {
                        if ( !g_MyD3D->MyPlayer[c]->m_kUserInfo.bLive )
                        {
                            vecChangeSlot.push_back( std::make_pair( c, true ) );
                        }
                    }

                    if( event.m_pWnd == m_pkModeTeam )
                    {
                        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH )
                        {
                            if ( SiKGCRoomManager()->GetGameMode() != GC_GM_TAG_TEAM )
                            {
                                SiKGCRoomManager()->SetGameMode( GC_GM_TAG_TEAM );
                                KP2P::GetInstance()->Send_ChangeRoomInfoReq( vecChangeSlot );
                            }
                        }
                    }
                    if( event.m_pWnd == m_pkModeSurvival )
                    {
                        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH )
                            SiKGCRoomManager()->SetGameMode( GC_GM_TAG_SURVIVAL );
                    }

                    ChangeTagMatchGameMode();
                    SiKGCRoomManager()->SendRoomInfo();
                    break;
                }
            }
        }
    }
}

void KGCTagMatchSelect::PreDraw( void )
{
	// 이놈은 LINEAR로 필터링 합니다. (이미지 축소시 깨짐 현상 때문)
	g_pGCDeviceManager2->PushState();
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
}

void KGCTagMatchSelect::PostChildDraw( void )
{
	g_pGCDeviceManager2->PopState();
}

void KGCTagMatchSelect::OnCandChar( int iIndex, DWORD dwCode )
{
	// 케릭터 선택
	if( dwCode == D3DWE_BUTTON_CLICK )
	{
        if( g_kGlobalValue.m_kUserInfo.nUserState == GC_RUS_READY )
            return;

		int iSlot = FindCharSlot();
		if( iSlot == -1 )
			return;

        if( iSlot == 0 )
        {
		    m_ppk1stChar[iIndex]->Lock( true );
            m_ppk1stCharFace[iIndex]->ToggleRender( true );
            m_ppk1stCharFace[iIndex]->SetFixedWindowLocalPos( m_ppkSelectChar[iSlot]->GetFixedWindowLocalPos() );
        }
        else if( iSlot == 1 )
        {
            m_ppk2ndChar[iIndex]->Lock( true );
            m_ppk2ndCharFace[iIndex]->ToggleRender( true );
            m_ppk2ndCharFace[iIndex]->SetFixedWindowLocalPos( m_ppkSelectChar[iSlot]->GetFixedWindowLocalPos() );
        }

		m_ppkSelectChar[iSlot]->InitState( true, true, this );
		m_pCharSlot[iSlot] = iIndex;
        g_kGlobalValue.m_kUserInfo.aiTagSlot[iSlot] = iIndex;

        KChangeRoomUserInfo kUserInfo;
        g_kGlobalValue.m_kUserInfo.SetChangeRoomUserInfo( &kUserInfo , KChangeRoomUserInfo::RUI_INDIGO_SLOT );
        for ( int i = 0; i < TAG_CHAR_NUM; i++ )
        {
            if ( m_pCharSlot[i] < 0 )
                continue;
            kUserInfo.m_acTagModeInfo[i][1] = g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->GetPromotion( m_pCharSlot[i] );
        }
        kUserInfo.m_acTagModeInfo[2][1] = 0;
        kUserInfo.m_nUserSlot = g_MyD3D->Get_MyPlayer();
        KP2P::GetInstance()->Send_ChangeRoomUserInfoReq( kUserInfo );
	}
}

void KGCTagMatchSelect::OnSelectChar( int iIndex, DWORD dwCode )
{
	// 케릭터 취소
	if( dwCode == D3DWE_BUTTON_CLICK )
	{
        if( g_kGlobalValue.m_kUserInfo.nUserState == GC_RUS_READY )
            return;

		m_ppkSelectChar[iIndex]->InitState( false, false, NULL );

        if( iIndex == 0 )
        {
            m_ppk1stCharFace[m_pCharSlot[iIndex]]->ToggleRender( false );
		    m_ppk1stChar[m_pCharSlot[iIndex]]->Lock( false );
        }
        else if( iIndex == 1 )
        {
            m_ppk2ndCharFace[m_pCharSlot[iIndex]]->ToggleRender( false );
            m_ppk2ndChar[m_pCharSlot[iIndex]]->Lock( false );
        }

		m_pCharSlot[iIndex] = -1;
        g_kGlobalValue.m_kUserInfo.aiTagSlot[iIndex] = -1;
		// 선택한 케릭터 갱신
        KChangeRoomUserInfo kUserInfo;
        g_kGlobalValue.m_kUserInfo.SetChangeRoomUserInfo( &kUserInfo , KChangeRoomUserInfo::RUI_INDIGO_SLOT );
        for ( int i = 0; i < TAG_CHAR_NUM; i++ )
        {
            if ( m_pCharSlot[i] < 0 )
                continue;
            kUserInfo.m_acTagModeInfo[i][1] = g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->GetPromotion( m_pCharSlot[i] );
        }
        kUserInfo.m_acTagModeInfo[2][1] = 0;
        kUserInfo.m_nUserSlot = g_MyD3D->Get_MyPlayer();
        KP2P::GetInstance()->Send_ChangeRoomUserInfoReq( kUserInfo );
	}
}

int KGCTagMatchSelect::FindCharSlot()
{
	for( int i = 0 ; i < TAG_CHAR_NUM ; ++i )
	{
		if( m_pCharSlot[i] == -1 )
			return i;
	}

	return -1;
}

void KGCTagMatchSelect::UpdateSlot()
{
	Init();
    for( int i = 0 ; i < 2 ; ++i )
    {
        m_pCharSlot[i] = g_kGlobalValue.m_kUserInfo.aiTagSlot[i];
    }
    
	for( int i = 0 ; i < TAG_CHAR_NUM ; ++i )
	{
		if( m_pCharSlot[i] != -1 )
		{
			m_ppkSelectChar[i]->InitState( true, true, this );
            if( i == 0 )
            {
			    m_ppk1stCharFace[m_pCharSlot[i]]->ToggleRender( true );
			    m_ppk1stCharFace[m_pCharSlot[i]]->SetFixedWindowLocalPos( m_ppkSelectChar[i]->GetFixedWindowLocalPos() );
                m_ppk1stChar[m_pCharSlot[i]]->Lock( true );
            }
            else if( i == 1 )
            {
                m_ppk2ndCharFace[m_pCharSlot[i]]->ToggleRender( true );
                m_ppk2ndCharFace[m_pCharSlot[i]]->SetFixedWindowLocalPos( m_ppkSelectChar[i]->GetFixedWindowLocalPos() );
                m_ppk2ndChar[m_pCharSlot[i]]->Lock( true );
            }
		}
	}

    for( int i = 0 ; i < 2 ; ++i )
    {
        g_kGlobalValue.m_kUserInfo.aiTagSlot[i] = m_pCharSlot[i];
    }
    g_kGlobalValue.m_kUserInfo.aiTagSlot[2] = -1;
}

int KGCTagMatchSelect::GetUsedSlotCount()
{
    int iUsedSlotCount = 0;
    for ( int i = 0; i < TAG_CHAR_NUM; ++i )
    {
        if ( m_pCharSlot[i] != -1 )
            iUsedSlotCount++;
    }
    return iUsedSlotCount;
}

bool KGCTagMatchSelect::IsReadyAvailable()
{
    m_bIsReadyAvailable = false;
    for( int i = 0; i < TAG_CHAR_NUM; ++i )
    {
        m_bIsReadyAvailable |= (m_pCharSlot[i] != -1);
    }

    return m_bIsReadyAvailable;
}

void KGCTagMatchSelect::ChangeTagMatchGameMode( void )
{
    EGCGameMode eGameMode = SiKGCRoomManager()->GetGameMode();
    m_pkModeTeam->Activate( ( eGameMode == GC_GM_TAG_TEAM ) );
    m_pkModeSurvival->Activate( ( eGameMode == GC_GM_TAG_SURVIVAL ) );
}

void KGCTagMatchSelect::UpdateGameData()
{
    ChangeTagMatchGameMode();

    bool bHost = g_kGlobalValue.m_kUserInfo.bHost;

    m_pkModeTeam->SetSelfInputCheck( bHost );
    m_pkModeSurvival->SetSelfInputCheck( bHost );
}

bool KGCTagMatchSelect::ProhibitChar(int iChar)
{
    switch (iChar)
    {
    case GC_CHAR_ASIN:
        return true;
    case GC_CHAR_LIME:
        return true;
#ifdef DECANEE
    case GC_CHAR_DECANEE:
        return true;
#endif
    default:
        return false;
    }

    return false;
}
