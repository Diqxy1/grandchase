#include "stdafx.h"
#include "GCOlympicSongkranDlg.h"

IMPLEMENT_CLASSNAME( KGCOlympicSongkranDlg );
IMPLEMENT_WND_FACTORY( KGCOlympicSongkranDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCOlympicSongkranDlg, "gc_olympic_songkrandlg" );

const int nFRAME_COUNT = 55;

KGCOlympicSongkranDlg::KGCOlympicSongkranDlg( void )
: m_nCurFrame ( 0 )
, m_nBombState( BOMB_STATE_READY )
, m_nBombFrame( 0 )
, m_bResultShot( false )
, m_fBombXSpeed( 0.1f )
, m_dwIndex( 0 )
, m_nCharAnimationFrame( 0 )
, m_backMusic( GC_BGM_MAIN_THEME )
, m_bRenderCharMark( false )
, m_nPrevTime( -1 )
, m_fPlayerCharMoveSpeed( 0 )
, m_fWaterBombFlyingSpeed( 0 )
, m_fWaterBombRange(0)
, m_dwScore(0)
, m_dwWaterBombDuration(0)
, m_tmTodayDate(0)
, m_nGameType(0)  
, m_nTeamType(0)
{	

	for ( int i = 0; i < CHAR_SIZE; ++i ) 
	{
		m_nTargetState[i] = TARGET_STATE_MOVE;
		m_nTargetDeathFrame[i] = 0;
	}

	for ( int i = 0; i < TEL_CHAR_SIZE; ++i ) 
	{
		m_nTargetTelFrame[i] = 0;
	}

	for ( int i = 0; i < MOVE_TYPE_RUN_SIZE; ++i ) 
	{
		m_fMoveSpeed[i] = 1.0f;
	}

	m_pkBtnClose = NULL;
	m_apkPlayer = NULL;
	m_apkPlayerMark = NULL;
	m_apkBomb = NULL;
	m_pkGameScore = NULL;
	m_apkScope = NULL;
	m_pkBombCount = NULL;

	char szControl[MAX_PATH];

	for ( int i = 0; i < CHAR_SIZE; ++i ) 
	{
		m_apkTarget[i] = NULL;

		sprintf( szControl, "char_target%02d", i );
		LINK_CONTROL( szControl, m_apkTarget[i] );
	}

	for ( int i = 0; i < EM_HIT_POINT_SIZE; ++i ) 
	{
		m_apkPointBack[i] = NULL;
		m_apkPointNum[i] = NULL;

		sprintf( szControl, "PointBack%d", i );
		LINK_CONTROL( szControl, m_apkPointBack[i] );

		sprintf( szControl, "score_num%d", i );
		LINK_CONTROL( szControl, m_apkPointNum[i] );		
	}

	LINK_CONTROL( "char_player", m_apkPlayer );
	LINK_CONTROL( "char_player_mark", m_apkPlayerMark );

	LINK_CONTROL( "char_bomb", m_apkBomb );
	LINK_CONTROL( "bomb_scope", m_apkScope );

	LINK_CONTROL( "btn_close", m_pkBtnClose );
	LINK_CONTROL( "btn_close2", m_pkBtnClose2 );
	
	
	LINK_CONTROL( "static_game_score", m_pkGameScore );
	LINK_CONTROL( "static_bomb_num", m_pkBombCount );

}

KGCOlympicSongkranDlg::~KGCOlympicSongkranDlg( void )
{
}

void KGCOlympicSongkranDlg::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
	GCWND_MSG_MAP( m_pkBtnClose2, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
}

void KGCOlympicSongkranDlg::OnCreate( void )
{
	m_pkBtnClose->InitState( true, true, this );
	m_pkBtnClose->SetHotKey( DIK_ESCAPE );

	m_pkBtnClose2->InitState( true, true, this );

	m_apkPlayer->InitState( true );
	m_apkPlayerMark->InitState( true );
	m_apkBomb->InitState( true );
	m_apkScope->InitState( true );

	for ( int i = 0; i < CHAR_SIZE; ++i ) 
	{
		m_apkTarget[i]->InitState( true );
	}

	m_pkGameScore->InitState( true, true, this );
	m_pkGameScore->SetStringType( "orange_gradation_num2" );
	m_pkGameScore->SetNumber( 0 );

	m_pkBombCount->InitState( true, true, this );
	m_pkBombCount->SetStringType( "orange_gradation_num4" );
	m_pkBombCount->SetNumber( 0 );
	
	for ( int i = 0; i < EM_HIT_POINT_SIZE; ++i ) 
	{
		m_apkPointBack[i]->InitState( true );

		m_apkPointNum[i]->InitState( true );
		m_apkPointNum[i]->SetNumber( 0 );
	}

	m_apkPointNum[EM_ONE_HIT_POINT]->SetStringType( "small_orenge" );
	m_apkPointNum[EM_DOUBLE_HIT_POINT]->SetStringType( "small_purple" );

    LoadTargetInfo();

}

void KGCOlympicSongkranDlg::OnCreateComplete( void )
{

	Init();
	CalcBombSpeed();

	m_backMusic = static_cast<EGCBackgroundMusic>(g_KMci.GetTrack());

	if( m_backMusic != GC_BGM_SONGKRAN_EVENT )
		g_KMci.Play( GC_BGM_SONGKRAN_EVENT, true );
}

void KGCOlympicSongkranDlg::CalcBombSpeed()
{
	//	폭탄이 도착지까지 도착하는 프레임 계산해서 거기에 맞춰서 x축도 이동해주자.
	D3DXVECTOR2 vBomb = m_apkBomb->GetFixedWindowLocalPos();
	D3DXVECTOR2 vScope = m_apkScope->GetFixedWindowLocalPos();

	float fDiffY = fBOMB_START_POSY2 - fBOMB_END_POSY;
	float fDiffX = vBomb.x - vScope.x;
	fDiffX -= (fGAP_BOMB - fGAP_BOMB2);

	//	Y축 이동 프레임 측정
	int nFrame = (int)( fDiffY / m_fWaterBombFlyingSpeed ) + 1;
	m_fBombXSpeed = fDiffX / (float)nFrame;

}

void KGCOlympicSongkranDlg::FrameMoveInEnabledState( void )
{
	UpdateBomb();
	UpdateTarget();
	UpdateKeyCheck();	
}

void KGCOlympicSongkranDlg::PostChildDraw( void )
{
	g_ParticleManager->Render( GC_LAYER_UPPER_SQUARE_UI_0 );
}

void KGCOlympicSongkranDlg::OnClose()
{
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
	g_KMci.Play( m_backMusic, true );
}


void KGCOlympicSongkranDlg::Init()
{
	m_nCurFrame = 0;
	m_nCharAnimationFrame = 0;
	m_bResultShot = false;
	m_dwIndex = 0;
	m_nBombState = BOMB_STATE_READY;

	m_fMoveSpeed[CHAR_ELESIS] = GetTargetSpeed(CHAR_ELESIS );
	m_fMoveSpeed[CHAR_ZERO] = GetTargetSpeed( CHAR_ZERO );
	m_fMoveSpeed[CHAR_LAS] = GetTargetSpeed( CHAR_LAS );

	// 좌우 랜덤 적용
	for ( int i = 0; i < MOVE_TYPE_RUN_SIZE; ++i ) 
	{
		int nRand = rand()%2;
		if ( nRand == 0 ) 
		{
			m_fMoveSpeed[i] *= -1;
		}
	}

	InitTargetPos();
	InitPlayerPos();
	InitBombPos();
	InitCharMarkPos();
	InitGetPointBack();

	UpdateBombCount();
	UpdateScore();
}

void KGCOlympicSongkranDlg::UpdateBombCount()
{
	int nCount = m_dwWaterBombDuration;

	m_pkBombCount->SetNumber( nCount );
}

void KGCOlympicSongkranDlg::UpdateScore()
{
	m_pkGameScore->SetNumber( m_dwScore );
}

void KGCOlympicSongkranDlg::UpdateBomb()
{
	switch ( m_nBombState )
	{
	case BOMB_STATE_READY:
		{
			UpdateTargeting();
		}
		break;
	case BOMB_STATE_SHOT:
		{
			UpdateBombPos();
		}
		break;
	case BOMB_STATE_EXPLODE:
		{
			if ( m_apkBomb->IsRenderOn() ) 
			{
				m_apkBomb->ToggleRender( false );
				m_apkScope->ToggleRender( false );
			}

			m_nBombFrame++;

			if ( m_nBombFrame > nBOMB_EXPLODE_FRAME && m_bResultShot )
			{
				InitBombPos();
				InitGetPointBack();
				InitCharMarkPos();

				m_apkBomb->ToggleRender( true );
				m_apkScope->ToggleRender( true );
				m_nBombFrame = 0;
				m_nBombState = BOMB_STATE_READY;
				m_bResultShot = false;

				m_apkPlayer->SetWndMode( D3DWM_DEFAULT );
				
			}
		}
		break;
	}
}

void KGCOlympicSongkranDlg::ResetTargeting()
{
	for ( int i = 0; i < CHAR_SIZE; ++i ) 
	{
		if ( m_nTargetState[i] != TARGET_STATE_MOVE ) 
			continue;

		m_apkTarget[i]->SetWndMode( D3DWM_DEFAULT );
	}
}

void KGCOlympicSongkranDlg::UpdateTargeting()
{
	D3DXVECTOR2 vtScopePos = m_apkScope->GetFixedWindowLocalPos();
	DWORD dwScopeWidth = m_apkScope->GetWidth() / 2;

	float fBombRange = m_fWaterBombRange;

	float fLeft = (vtScopePos.x+dwScopeWidth) - fBombRange;
	float fRight = (vtScopePos.x+dwScopeWidth) + fBombRange;




	//	반경 범위내 CharCheck 
	for ( int i = 0; i < CHAR_SIZE; ++i ) 
	{
		DWORD dwWidth = m_apkTarget[i]->GetWidth();

		D3DXVECTOR2 vtFixedWindowLocalPos = m_apkTarget[i]->GetFixedWindowLocalPos();
		float fCharLeft = vtFixedWindowLocalPos.x;
		float fCharRight = vtFixedWindowLocalPos.x + dwWidth;

		//	상태 체크
		if ( m_nTargetState[i] != TARGET_STATE_MOVE ) 
			continue;

		m_apkTarget[i]->SetWndMode( D3DWM_DEFAULT );

		//	내부점에 있으면
		if ( ( fCharLeft >= fLeft && fCharLeft <= fRight ) || 
			( fCharRight >= fLeft && fCharRight <= fRight ) )
		{
			m_apkTarget[i]->SetWndMode( D3DWM_HOVER );
		}
		//	타겟 안에 폭탄 범위가 들어가면
		else if ( ( fLeft >= fCharLeft && fLeft <= fCharRight ) || 
			( fRight >= fCharLeft && fRight <= fCharRight ) )
		{
			m_apkTarget[i]->SetWndMode( D3DWM_HOVER );
		}
	}
}

void KGCOlympicSongkranDlg::InitPlayerPos()
{
	D3DXVECTOR2 vtFixedWindowLocalPos = m_apkPlayer->GetFixedWindowLocalPos();

	vtFixedWindowLocalPos.x = fTARGET_START_POSX + ( fTARGET_GAP_SIZE * ( nTARGET_POS_NUM / 2 ) );

	m_apkPlayer->SetWindowPos( vtFixedWindowLocalPos );
	m_apkPlayer->SetWndMode( D3DWM_DEFAULT );
}

void KGCOlympicSongkranDlg::InitBombPos()
{
	D3DXVECTOR2 vtFixedWindowLocalPos = m_apkPlayer->GetFixedWindowLocalPos();

	// 폭탄 위치 갱신
	D3DXVECTOR2 vBombPos = vtFixedWindowLocalPos;

	// 폭탄 위치 갱신
	vBombPos.x += fGAP_BOMB;
	vBombPos.y = fBOMB_START_POSY;

	m_apkBomb->SetWindowPos( vBombPos );


	//	Scope 위치 갱신
	D3DXVECTOR2 vScopePos = vtFixedWindowLocalPos;
	DWORD dwGap = m_apkPlayer->GetWidth() - m_apkScope->GetWidth();

	vScopePos.x += dwGap / 2;
	vScopePos.y = fBOMB_END_POSY;
	m_apkScope->SetWindowPos( vScopePos );		

}

void KGCOlympicSongkranDlg::InitTargetPos()
{
	std::vector<int> vecShuffle;

	for ( int i = 0; i < nTARGET_POS_NUM; i+=2 )
	{
		vecShuffle.push_back( i ) ;
	}

	std::random_shuffle( vecShuffle.begin(), vecShuffle.end() );

	for ( int i = 0; i < CHAR_SIZE; ++i ) 
	{
		D3DXVECTOR2 vtFixedWindowLocalPos = m_apkTarget[i]->GetFixedWindowLocalPos();

		vtFixedWindowLocalPos.x = fTARGET_START_POSX + ( fTARGET_GAP_SIZE * vecShuffle[i] );

		m_apkTarget[i]->SetWindowPos( vtFixedWindowLocalPos );

		m_apkTarget[i]->Lock( false );

		m_nTargetDeathFrame[i] = 0;
		m_nTargetState[i] = TARGET_STATE_MOVE;
	}

	for ( int i = 0; i < TEL_CHAR_SIZE; ++i ) 
	{
		m_nTargetTelFrame[i] = 0;
	}

}

void KGCOlympicSongkranDlg::UpdateKeyCheck() 
{
	//	오른쪽 
	if ( g_pkInput->IsPressed( DIK_RIGHT ) ) 
	{
		D3DXVECTOR2 vtFixedWindowLocalPos = m_apkPlayer->GetFixedWindowLocalPos();

		float fPosX = vtFixedWindowLocalPos.x;
		fPosX += m_fPlayerCharMoveSpeed;


		if ( fPosX > fPLAYER_END_POSX ) 
		{
			fPosX = fPLAYER_END_POSX;
		}

		vtFixedWindowLocalPos.x = fPosX;

		UpdatePlayerPos( vtFixedWindowLocalPos );
	}

	if ( g_pkInput->IsPressed( DIK_LEFT ) ) 
	{
		D3DXVECTOR2 vtFixedWindowLocalPos = m_apkPlayer->GetFixedWindowLocalPos();

		float fPosX = vtFixedWindowLocalPos.x;

		fPosX -= m_fPlayerCharMoveSpeed;

		if ( fPosX < fPLAYER_START_POSX ) 
		{
			fPosX = fPLAYER_START_POSX;
		}

		vtFixedWindowLocalPos.x = fPosX;

		UpdatePlayerPos( vtFixedWindowLocalPos );
	}

	if ( g_pkInput->IsUp( DIK_SPACE ) && m_nBombState == BOMB_STATE_READY )
	{
		// shot Bomb
		if ( IsBombShot() ) 
		{
			ThrowBomb();
		}
	}
}

void KGCOlympicSongkranDlg::UpdatePlayerPos( D3DXVECTOR2 vPos )
{
	m_apkPlayer->SetWindowPos( vPos );

	if ( m_bRenderCharMark )
	{
		m_apkPlayerMark->SetWindowPos( vPos );
	}


	//	폭탄을 들고 있을 때만.
	if ( m_nBombState == BOMB_STATE_READY ) 
	{
		//	Scope 위치 갱신
		D3DXVECTOR2 vScopePos = vPos;
		DWORD dwGap = m_apkPlayer->GetWidth() - m_apkScope->GetWidth();

		vScopePos.x += dwGap / 2;
		vScopePos.y = fBOMB_END_POSY;
		m_apkScope->SetWindowPos( vScopePos );		

		D3DXVECTOR2 vBombPos = vPos;

		// 폭탄 위치 갱신
		vBombPos.x += fGAP_BOMB;
		vBombPos.y = fBOMB_START_POSY;

		m_apkBomb->SetWindowPos( vBombPos );

	}
}

void KGCOlympicSongkranDlg::UpdateBombPos()
{
	D3DXVECTOR2 vtFixedWindowLocalPos = m_apkBomb->GetFixedWindowLocalPos();

	vtFixedWindowLocalPos.y -= m_fWaterBombFlyingSpeed;
	vtFixedWindowLocalPos.x -= m_fBombXSpeed;


	m_apkBomb->SetWindowPos( vtFixedWindowLocalPos );

	//	터져라.
	if ( vtFixedWindowLocalPos.y < fBOMB_END_POSY ) 
	{
		//	맞은 타겟 검색
		SearchBombTarget( vtFixedWindowLocalPos.x );

		//	물폭탄 State변경
		SetBombStateExplode();		
	}

}

void KGCOlympicSongkranDlg::SetBombStateExplode()
{
	//	위치를 안보이는곳으로 옮긴다.

	D3DXVECTOR2 vtFixedWindowLocalPos = m_apkBomb->GetFixedWindowLocalPos();

	vtFixedWindowLocalPos.y = -1000;

	m_apkBomb->SetWindowPos( vtFixedWindowLocalPos );
	
	D3DXVECTOR2 vtScopePos = m_apkScope->GetFixedWindowPos();	
	m_apkScope->SetWindowPos( vtFixedWindowLocalPos );

	m_nBombState = BOMB_STATE_EXPLODE;

	g_KDSound.Play( "songkran_3" );

	m_pkBtnClose->Lock( false );
	m_pkBtnClose2->Lock( false );

	//	파티클 출력	
	vtScopePos.x += ( m_apkScope->GetWidth() / 2 ) + 1;
	vtScopePos.y += ( m_apkScope->GetHeight() / 2 ) + 1;


	g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Waterboom_01",PARTICLEPOS_X(vtScopePos.x) ,PARTICLEPOS_Y(vtScopePos.y) , 0.2f ), GC_LAYER_UPPER_SQUARE_UI_0);
	g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Waterboom_02", PARTICLEPOS_X(vtScopePos.x), PARTICLEPOS_Y(vtScopePos.y), 0.2f ), GC_LAYER_UPPER_SQUARE_UI_0);
	g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Waterboom_03", PARTICLEPOS_X(vtScopePos.x), PARTICLEPOS_Y(vtScopePos.y), 0.2f ), GC_LAYER_UPPER_SQUARE_UI_0);

}

void KGCOlympicSongkranDlg::SearchBombTarget( float fPosX )
{
	float fBombRange = m_fWaterBombRange;

	float fLeft = fPosX;
	float fRight = fPosX + ( fBombRange * 2 );


	int nGetScore = 0;
	int nKillNum = 0;
	int nIndex = 0;

	//	반경 범위내 CharCheck 
	for ( int i = 0; i < CHAR_SIZE; ++i ) 
	{
		DWORD dwWidth = m_apkTarget[i]->GetWidth();

		D3DXVECTOR2 vtFixedWindowLocalPos = m_apkTarget[i]->GetFixedWindowLocalPos();
		float fCharLeft = vtFixedWindowLocalPos.x;
		float fCharRight = vtFixedWindowLocalPos.x + dwWidth;

		//		상태 체크
		if ( m_nTargetState[i] != TARGET_STATE_MOVE ) 
			continue;

		//	폭탄 범위안에 타겟이 있으면
		if ( ( fCharLeft >= fLeft && fCharLeft <= fRight ) || 
			( fCharRight >= fLeft && fCharRight <= fRight ) )
		{
			nKillNum++;
			int nScore = GetTargetScore( i );
			nGetScore += nScore;

			nIndex = i;
			

			//	Lock 걸자.
			m_nTargetState[i] = TARGET_STATE_DEATH;
			m_apkTarget[i]->Lock( true );
		}
		//	타겟 안에 폭탄 범위가 들어가면
		else if ( ( fLeft >= fCharLeft && fLeft <= fCharRight ) || 
				( fRight >= fCharLeft && fRight <= fCharRight ) )
		{
			nKillNum++;
			int nScore = GetTargetScore( i );
			nGetScore += nScore;

			nIndex = i;


			//	Lock 걸자.
			m_nTargetState[i] = TARGET_STATE_DEATH;
			m_apkTarget[i]->Lock( true );

		}
	}

	bool bDouble = ( nKillNum >= 2 );

	if ( bDouble )
	{
		nGetScore *= 2;
	}

	ReqGetScore( nGetScore );

	if ( nGetScore > 0 ) 
		AddScore( nGetScore, bDouble, nIndex );
	else
		FailScore();
}

void KGCOlympicSongkranDlg::ReqGetScore( int nScore )
{
	KP2P::GetInstance()->Send_OlympicWaterbombReq( nScore, m_tmTodayDate, m_nGameType, m_nTeamType );
}

void KGCOlympicSongkranDlg::AddScore( int nScore, bool _bDouble, int _nIndex )
{
	if ( !_bDouble ) 
	{
		D3DXVECTOR2 vecPos = m_apkTarget[_nIndex]->GetFixedWindowLocalPos();
		
		vecPos.y -= ( m_apkPointBack[EM_ONE_HIT_POINT]->GetHeight() - 8.0f );

		m_apkPointBack[EM_ONE_HIT_POINT]->SetWindowPos( vecPos );

		vecPos.x += 16.0f;
		vecPos.y += 12.0f;

		m_apkPointNum[EM_ONE_HIT_POINT]->SetWindowPos( vecPos );
		m_apkPointNum[EM_ONE_HIT_POINT]->SetNumber( nScore );
			
		g_KDSound.Play( "songkran_5" );
	}
	else
	{
		D3DXVECTOR2 vecPos = m_apkScope->GetFixedWindowLocalPos();

		vecPos.y -= m_apkPointBack[EM_DOUBLE_HIT_POINT]->GetHeight() + 2.0f;

		m_apkPointBack[EM_DOUBLE_HIT_POINT]->SetWindowPos( vecPos );

		vecPos.x += 16.0f;
		vecPos.y += 14.0f;

		m_apkPointNum[EM_DOUBLE_HIT_POINT]->SetWindowPos( vecPos );
		m_apkPointNum[EM_DOUBLE_HIT_POINT]->SetNumber( nScore );

		g_KDSound.Play( "songkran_1" );
	}
}

void KGCOlympicSongkranDlg::UpdateTarget()
{
	ED3DWndMode wndMode = D3DWM_DEFAULT;

	m_nCharAnimationFrame++;
	if ( m_nCharAnimationFrame > nCHAR_ANIMATION_FRAME )
	{
		wndMode = D3DWM_ACTIVE;
	}

	if ( m_nCharAnimationFrame > nCHAR_ANIMATION_FRAME * 2 )
	{
		m_nCharAnimationFrame = 0;
	}


	for ( int i = 0; i < CHAR_SIZE; ++i ) 
	{
		switch( m_nTargetState[i] ) 
		{
		case TARGET_STATE_MOVE:
			{
				//	타켓팅중이 아닐때만 변경
				if( !(m_apkTarget[i]->GetWndMode() & D3DWM_HOVER) )
					m_apkTarget[i]->SetWndMode( wndMode );
			}
			break;
		case TARGET_STATE_DEATH:
			{
				m_nTargetDeathFrame[i]++;

				int nFrame = m_nTargetDeathFrame[i] % (nTARGET_DEATH_BLINK_FRAME * 2);

				if ( nFrame < nTARGET_DEATH_BLINK_FRAME ) 
				{
					m_apkTarget[i]->ToggleRender( true );
				}
				else
				{
					m_apkTarget[i]->ToggleRender( false );
				}

				if ( m_nTargetDeathFrame[i] >= GetResetFrame( i ) ) 
				{
					m_nTargetDeathFrame[i] = 0;
					m_nTargetState[i] = TARGET_STATE_MOVE;
					m_apkTarget[i]->Lock( false );
					m_apkTarget[i]->ToggleRender( true );

					if ( i == CHAR_ARME || i == CHAR_REY )
					{					
						int nTelIndex = TEL_CHAR_ARME + ( i - CHAR_ARME );
						m_nTargetTelFrame[nTelIndex] = GetTeleportFrame( i );
					}
				}
			}
			break;
		}
	}

	if ( m_nTargetState[CHAR_ARME] == TARGET_STATE_MOVE ) 
	{
		m_nTargetTelFrame[TEL_CHAR_ARME]++;
	}

	if ( m_nTargetState[CHAR_REY] == TARGET_STATE_MOVE ) 
	{
		m_nTargetTelFrame[TEL_CHAR_REY]++;
	}

	UpdateTargetPos();
}

void KGCOlympicSongkranDlg::UpdateTargetPos()
{
	// 걸어 댕기는 애들( 엘리, 제로, 라스 ) 
	{		
		for ( int i = 0; i < MOVE_TYPE_RUN_SIZE; ++i ) 
		{
			if ( m_nTargetState[i] != TARGET_STATE_MOVE )
				continue;

			D3DXVECTOR2 vtFixedWindowLocalPos = m_apkTarget[i]->GetFixedWindowLocalPos();

			float fPosX = vtFixedWindowLocalPos.x;

			fPosX += m_fMoveSpeed[i];

			//	오른쪽 벽면 체크
			if ( m_fMoveSpeed[i] > 0 ) 
			{
				if ( fPosX > fTARGET_END_POSX )
				{
					fPosX = fTARGET_END_POSX;
					m_fMoveSpeed[i] *= -1;
				}
			}
			else
			{
				if ( fPosX < fTARGET_START_POSX ) 
				{
					fPosX = fTARGET_START_POSX;
					m_fMoveSpeed[i] *= -1;
				}
			}

			vtFixedWindowLocalPos.x = fPosX;

			m_apkTarget[i]->SetWindowPos( vtFixedWindowLocalPos );
		}
	}

	if ( m_nTargetState[CHAR_ARME] == TARGET_STATE_MOVE &&
		m_nTargetTelFrame[TEL_CHAR_ARME] >= GetTeleportFrame( CHAR_ARME ) ) 
	{
		m_nTargetTelFrame[TEL_CHAR_ARME] = 0;

		// 아르메
		int nRand = rand()%11;

		D3DXVECTOR2 vtFixedWindowLocalPos = m_apkTarget[CHAR_ARME]->GetFixedWindowLocalPos();
		vtFixedWindowLocalPos.x = fTARGET_START_POSX + ( nRand * fTARGET_GAP_SIZE );
		m_apkTarget[CHAR_ARME]->SetWindowPos( vtFixedWindowLocalPos );
	}

	if ( m_nTargetState[CHAR_REY] == TARGET_STATE_MOVE &&
		m_nTargetTelFrame[TEL_CHAR_REY] >= GetTeleportFrame( CHAR_REY ) ) 
	{

		m_nTargetTelFrame[TEL_CHAR_REY] = 0;
		// 레이
		D3DXVECTOR2 vtFixedWindowLocalPos = m_apkTarget[CHAR_REY]->GetFixedWindowLocalPos();
		float fPosX = vtFixedWindowLocalPos.x;

		// 한쪽 벽면일때 체크
		if ( fPosX - fTARGET_GAP_SIZE < fTARGET_START_POSX ) 
		{		
			fPosX += fTARGET_GAP_SIZE;
		}
		else if ( fPosX + fTARGET_GAP_SIZE > fTARGET_END_POSX ) 
		{
			fPosX -= fTARGET_GAP_SIZE;
		}
		else
		{
			int nRight = rand()%2;

			if ( nRight == 0 ) 
			{
				fPosX -= fTARGET_GAP_SIZE;
			}
			else
			{
				fPosX += fTARGET_GAP_SIZE;
			}
		}

		vtFixedWindowLocalPos.x = fPosX;

		m_apkTarget[CHAR_REY]->SetWindowPos( vtFixedWindowLocalPos );
	}
}

bool KGCOlympicSongkranDlg::IsBombShot()
{
	//	물폭탄이 없다.
	if ( m_dwWaterBombDuration <= 0 ) 
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_SONGKRAN_EVENT_ERROR01),
								 L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);      
                 
		return false;
	}

	return true;
}

void KGCOlympicSongkranDlg::UpdateResultScore()
{
	m_bResultShot = true;
	//	물폭탄 갯수 업데이트
	UpdateBombCount();
	
	//	점수 업데이트
	UpdateScore();
}

void KGCOlympicSongkranDlg::ThrowBomb()
{
	m_nBombState = BOMB_STATE_SHOT;

	m_apkPlayer->SetWndMode( D3DWM_ACTIVE );

	D3DXVECTOR2 vtFixedWindowLocalPos = m_apkPlayer->GetFixedWindowLocalPos();

	// 폭탄 위치 갱신
	D3DXVECTOR2 vBombPos = vtFixedWindowLocalPos;

	// 폭탄 위치 갱신
	vBombPos.x += fGAP_BOMB2;
	vBombPos.y = fBOMB_START_POSY2;

	m_apkBomb->SetWindowPos( vBombPos );

	g_KDSound.Play( "songkran_2" );

	m_pkBtnClose->Lock( true );
	m_pkBtnClose2->Lock( true );
}

void KGCOlympicSongkranDlg::InitCharMarkPos()
{
	D3DXVECTOR2 vtFixedWindowLocalPos = m_apkPlayerMark->GetFixedWindowLocalPos();

	vtFixedWindowLocalPos.y = -1000;

	m_apkPlayerMark->SetWindowPos( vtFixedWindowLocalPos );

	m_bRenderCharMark = false;
}

void KGCOlympicSongkranDlg::InitGetPointBack()
{
	for ( int i = 0; i < EM_HIT_POINT_SIZE; ++i ) 
	{
		D3DXVECTOR2 vtFixedWindowLocalPos = m_apkPointBack[i]->GetFixedWindowLocalPos();

		vtFixedWindowLocalPos.y = -1000;

		m_apkPointBack[i]->SetWindowPos( vtFixedWindowLocalPos );
		m_apkPointNum[i]->SetWindowPos( vtFixedWindowLocalPos );
	}
}

void KGCOlympicSongkranDlg::FailScore()
{
	D3DXVECTOR2 vtCharPos = m_apkPlayer->GetFixedWindowLocalPos();

	m_apkPlayerMark->SetWindowPos( vtCharPos );

	g_KDSound.Play( "songkran_4" );

	m_bRenderCharMark = true;
}

int	KGCOlympicSongkranDlg::GetTeleportFrame( int nChar )
{
    OlympicSongTargetInfo* pTarget = GetTargetInfo( nChar );

    if ( !pTarget ) 
        return nFRAME_COUNT;

    return static_cast<int>( pTarget->m_fTeleportPeriod * nFRAME_COUNT );
}

int KGCOlympicSongkranDlg::GetResetFrame( int nChar )
{
    OlympicSongTargetInfo* pTarget = GetTargetInfo( nChar );

    if ( !pTarget ) 
        return nFRAME_COUNT;

    return static_cast<int>( pTarget->m_fHitTargetAfterResetTime * nFRAME_COUNT );
}


DWORD KGCOlympicSongkranDlg::GetTargetScore( int nChar )
{
    OlympicSongTargetInfo* pTarget = GetTargetInfo( nChar );

    if ( !pTarget ) 
        return 0;

    return pTarget->m_dwTargetScore;
}

float KGCOlympicSongkranDlg::GetTargetSpeed( int nChar )
{
    OlympicSongTargetInfo* pTarget = GetTargetInfo( nChar );

    if ( !pTarget ) 
        return 0.0f;

    return pTarget->m_fTargetMoveSpeed;
}

OlympicSongTargetInfo* KGCOlympicSongkranDlg::GetTargetInfo( int nChar )
{	
    if ( nChar < CHAR_ELESIS || nChar >= CHAR_SIZE )
        return NULL;

    std::map< USHORT, OlympicSongTargetInfo >::iterator pos = m_mapTargetInfoList.find( nChar );

    if ( pos == m_mapTargetInfoList.end() )
        return NULL;

    return &pos->second;
}


void KGCOlympicSongkranDlg::SetUserInfo( KEVENT_OLYMPIC_JOIN_GAME_ACK kRecv )
{
    m_dwScore = kRecv.m_nMyAccumulateData;            
	m_dwWaterBombDuration = kRecv.m_nDefaultCount;

    m_tmTodayDate       = kRecv.m_tmTodayDate;
    m_nGameType         = kRecv.m_nGameType;  
    m_nTeamType         = kRecv.m_nTeamType;

}

void KGCOlympicSongkranDlg::ResultSongkranInfo( KEVENT_OLYMPIC_WATERBOMB_RESULT_ACK kRecv )
{
    m_dwScore = kRecv.m_nMyAccumulateData;            
    m_dwWaterBombDuration = kRecv.m_nRemainTryCount;

    m_tmTodayDate       = kRecv.m_tmTodayDate;
}
bool KGCOlympicSongkranDlg::LoadTargetInfo( )
{
    KLuaManager luaMgr;

    m_mapTargetInfoList.clear();

    if( GCFUNC::LoadLuaScript( luaMgr , "InitSongkranOlympic.lua" ) == false )
        return false;

    LUA_BEGIN_TABLE("UserInfo", return false )
    {
        LUA_GET_VALUE_DEF( "PlayerMoveSpeed", m_fPlayerCharMoveSpeed, 0.0f )
        LUA_GET_VALUE_DEF( "WaterBombFylingSpeed", m_fWaterBombFlyingSpeed, 0.0f )
        LUA_GET_VALUE_DEF( "WaterBombRange", m_fWaterBombRange, 0.0f )
    }
    LUA_END_TABLE( return false )


    LUA_BEGIN_TABLE("TargetInfoList", return false );
    {
         for( int i = 1; ; ++i ) 
         {
             int nCharType;
             OlympicSongTargetInfo TargetInfo;

             LUA_BEGIN_TABLE(i, break);
             {
                 LUA_GET_VALUE_DEF( "CharType", nCharType, 0 );
                 LUA_GET_VALUE_DEF( "MoveSpeed", TargetInfo.m_fTargetMoveSpeed, 0.0f );
                 LUA_GET_VALUE_DEF( "Score", TargetInfo.m_dwTargetScore, 0);
                 LUA_GET_VALUE_DEF( "TelpoPeriod", TargetInfo.m_fTeleportPeriod, 0.0f );
                 LUA_GET_VALUE_DEF( "ResetTime", TargetInfo.m_fHitTargetAfterResetTime, 0.0f );

                 m_mapTargetInfoList.insert( std::make_pair( nCharType, TargetInfo ) ).second;
             }
             LUA_END_TABLE( return false );
         }
     }
     LUA_END_TABLE(return false );

     return true;
}
