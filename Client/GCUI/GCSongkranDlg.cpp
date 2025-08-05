#include "stdafx.h"
#include "GCSongkranDlg.h"

IMPLEMENT_CLASSNAME( KGCSongkranDlg );
IMPLEMENT_WND_FACTORY( KGCSongkranDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCSongkranDlg, "gc_songkrandlg" );

const DWORD MAX_BAR_LENGTH = 187;

// 끝지점 - 시작지점 = 넓이
// ÷?/ (등분간격-1) = 등분별 간격
//	6등분, 11등분

const float fTARGET_GAP_SIZE = 40.0f;
const int nTARGET_POS_NUM = 11;
const float fTARGET_START_POSX = 53.0f;
const float fTARGET_END_POSX = fTARGET_START_POSX + ( fTARGET_GAP_SIZE * (nTARGET_POS_NUM-1)  );

const float fPLAYER_START_POSX = 23.0f;
const float fPLAYER_END_POSX = 420.0f;

const float fBOMB_START_POSY = 292.0f;
const float fBOMB_END_POSY = 130.0f;

const float fBOMB_START_POSY2 = 271.0f;

const float fGAP_BOMB = 72.0f;
const float fGAP_BOMB2 = 67.0f;

const int nBOMB_EXPLODE_FRAME = 55;
const int nCHAR_ANIMATION_FRAME = 10;
const int nTARGET_DEATH_BLINK_FRAME = 5;

const int nPOINT_BACK_RANDOM = 10;






KGCSongkranDlg::KGCSongkranDlg( void )
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
{	

	for ( int i = 0; i < GCSongkranEventManager::CHAR_SIZE; ++i ) 
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
	m_pkRewardImg = NULL;
	m_pkPointBar = NULL;
	m_pkItemName = NULL;
	m_pkBtnLeftReward = NULL;
	m_pkBtnRightReward = NULL;
	m_pkRewardDesc = NULL;
	m_pkRemainTime = NULL;

	char szControl[MAX_PATH];

	for ( int i = 0; i < GCSongkranEventManager::CHAR_SIZE; ++i ) 
	{
		m_apkTarget[i] = NULL;

		sprintf( szControl, "char_target%02d", i );
		LINK_CONTROL( szControl, m_apkTarget[i] );
	}

	for ( int i = 0; i < NUM_SPECIAL_REWARD_SIZE; ++i ) 
	{
		m_apkSpecialRewardImg[i] = NULL;
		m_pkSpecialRewardScore[i] = NULL;

		sprintf( szControl, "sp_reward_item%d", i );
		LINK_CONTROL( szControl, m_apkSpecialRewardImg[i] );

		sprintf( szControl, "static_sp_reward_point%d", i );
		LINK_CONTROL( szControl, m_pkSpecialRewardScore[i] );
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

	LINK_CONTROL( "reward_item", m_pkRewardImg );
	LINK_CONTROL( "cur_point", m_pkPointBar );
	LINK_CONTROL( "static_reward_name", m_pkItemName );

	LINK_CONTROL( "btn_reward_left", m_pkBtnLeftReward );
	LINK_CONTROL( "btn_reward_right", m_pkBtnRightReward );
	LINK_CONTROL( "static_reward_desc", m_pkRewardDesc );
	LINK_CONTROL( "static_remain_time", m_pkRemainTime );
}

KGCSongkranDlg::~KGCSongkranDlg( void )
{
}

void KGCSongkranDlg::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
	GCWND_MSG_MAP( m_pkBtnClose2, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
	
	GCWND_MSG_MAP( m_pkBtnLeftReward, KD3DWnd::D3DWE_BUTTON_CLICK, OnLeftReward );
	GCWND_MSG_MAP( m_pkBtnRightReward, KD3DWnd::D3DWE_BUTTON_CLICK, OnRightReward );

}

void KGCSongkranDlg::OnCreate( void )
{
	m_pkBtnClose->InitState( true, true, this );
	m_pkBtnClose->SetHotKey( DIK_ESCAPE );

	m_pkBtnClose2->InitState( true, true, this );

	m_pkBtnLeftReward->InitState( true, true, this );
	m_pkBtnRightReward->InitState( true, true, this );

	m_apkPlayer->InitState( true );
	m_apkPlayerMark->InitState( true );
	m_apkBomb->InitState( true );
	m_apkScope->InitState( true );

	for ( int i = 0; i < GCSongkranEventManager::CHAR_SIZE; ++i ) 
	{
		m_apkTarget[i]->InitState( true );
	}

	m_pkGameScore->InitState( true, true, this );
	m_pkGameScore->SetStringType( "orange_gradation_num2" );
	m_pkGameScore->SetNumber( 0 );

	m_pkBombCount->InitState( true, true, this );
	m_pkBombCount->SetStringType( "orange_gradation_num4" );
	m_pkBombCount->SetNumber( 0 );

	m_pkRewardImg->InitState( true );
	m_pkRewardImg->ShowItem( false );

	for ( int i = 0; i < NUM_SPECIAL_REWARD_SIZE; ++i ) 
	{
		m_apkSpecialRewardImg[i]->InitState( true );
		m_apkSpecialRewardImg[i]->ShowItem( false );

		m_pkSpecialRewardScore[i]->InitState( true );
		m_pkSpecialRewardScore[i]->SetAlign( DT_CENTER );
	}
	
	m_pkPointBar->InitState( true, true, this );
	m_pkItemName->InitState( true );
	
	m_pkRemainTime->InitState( true );
	m_pkRemainTime->SetAlign(DT_CENTER);
	
	m_pkRewardDesc->InitState( true );
	m_pkRewardDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_SONGKRAN_REWARD_DESC ) );

	for ( int i = 0; i < EM_HIT_POINT_SIZE; ++i ) 
	{
		m_apkPointBack[i]->InitState( true );

		m_apkPointNum[i]->InitState( true );
		m_apkPointNum[i]->SetNumber( 0 );
	}

	m_apkPointNum[EM_ONE_HIT_POINT]->SetStringType( "small_orenge" );
	m_apkPointNum[EM_DOUBLE_HIT_POINT]->SetStringType( "small_purple" );
}

void KGCSongkranDlg::OnCreateComplete( void )
{
	if ( !SiGCSongkranEventManager()->IsGetInfo() )
		OnClose();

	Init();
	CalcBombSpeed();

	m_backMusic = static_cast<EGCBackgroundMusic>(g_KMci.GetTrack());

	if( m_backMusic != GC_BGM_SONGKRAN_EVENT )
		g_KMci.Play( GC_BGM_SONGKRAN_EVENT, true );
}

void KGCSongkranDlg::CalcBombSpeed()
{
	//	폭탄이 도착지까지 도착하는 프레임 계산해서 거기에 맞춰서 x축도 이동해주자.
	D3DXVECTOR2 vBomb = m_apkBomb->GetFixedWindowLocalPos();
	D3DXVECTOR2 vScope = m_apkScope->GetFixedWindowLocalPos();

	float fDiffY = fBOMB_START_POSY2 - fBOMB_END_POSY;
	float fDiffX = vBomb.x - vScope.x;
	fDiffX -= (fGAP_BOMB - fGAP_BOMB2);

	//	Y축 이동 프레임 측정
	int nFrame = (int)( fDiffY / SiGCSongkranEventManager()->GetBombSpeed() ) + 1;
	m_fBombXSpeed = fDiffX / (float)nFrame;

}

void KGCSongkranDlg::FrameMoveInEnabledState( void )
{
	UpdateBomb();
	UpdateTarget();
	UpdateKeyCheck();	
	UpdateItemInfomation();
	UpdateRemainTime();
}

void KGCSongkranDlg::PostChildDraw( void )
{
	g_ParticleManager->Render( GC_LAYER_UPPER_SQUARE_UI_0 );
}

void KGCSongkranDlg::UpdateRemainTime()
{
	int nPrevTime = SiGCSongkranEventManager()->GetRemainTime();

	if ( m_nPrevTime != nPrevTime ) 
	{
		m_nPrevTime = nPrevTime;

		std::wostringstream strm;

		WCHAR strMTemp[20];
		WCHAR strSTemp[20];
		wcsnset(strMTemp,'\0',20);
		wcsnset(strSTemp,'\0',20);

		nPrevTime = nPrevTime % ( 60 * 60 );
		swprintf( strMTemp, 20, L"%02d", (int)( nPrevTime / 60 ) );

		nPrevTime = nPrevTime % 60;
		swprintf( strSTemp, 20, L"%02d", nPrevTime );
		strm<<strMTemp<<L" : "<<strSTemp;
		m_pkRemainTime->SetText(strm.str().c_str());
	}

}

void KGCSongkranDlg::UpdateItemInfomation()
{
	const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

	bool bCheck = false;
	if( m_pkRewardImg->CheckPosInWindowBound( vMousePos ) && m_pkRewardImg->IsRenderOn() )
	{
		// 아이템 정보 표시창 위치 보정.
		D3DXVECTOR2 vecPos = m_pkRewardImg->GetCurrentWindowPos();
		GCITEMID itemID = m_pkRewardImg->GetItemID();
		RenderItemInfoBox( vecPos, itemID );
		bCheck = true;
	}

	for ( int i = 0; i < NUM_SPECIAL_REWARD_SIZE; ++i ) 
	{
		if( m_apkSpecialRewardImg[i]->CheckPosInWindowBound( vMousePos ) && m_apkSpecialRewardImg[i]->IsRenderOn() )
		{
			// 아이템 정보 표시창 위치 보정.
			D3DXVECTOR2 vecPos = m_apkSpecialRewardImg[i]->GetCurrentWindowPos();
			GCITEMID itemID = m_apkSpecialRewardImg[i]->GetItemID();
			RenderItemInfoBox( vecPos, itemID );
			bCheck = true;
		}
	}

	if( !bCheck )
	{
		g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
	} 
}

void KGCSongkranDlg::RenderItemInfoBox( D3DXVECTOR2& vPos_, GCITEMID _itemID )
{
	vPos_.x *= m_fWindowScaleX;
	vPos_.y *= m_fWindowScaleY;
	vPos_.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth() * g_pkUIScene->m_pkItemInformationBox->GetWindowScaleX();

	GCItem* pItem = g_pItemMgr->GetItemData(_itemID);
	if ( !pItem ) 
		return;

	g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
	g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
	g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

	g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vPos_ );
	g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
}

void KGCSongkranDlg::OnClose()
{
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
	g_KMci.Play( m_backMusic, true );
}

void KGCSongkranDlg::OnLeftReward()
{
	if ( m_dwIndex <= 0 ) 
		return;

	m_dwIndex--;

	UpdateSpecialReward();
}

void KGCSongkranDlg::OnRightReward()
{
	KSongkranEventRewardInfo* pReward = SiGCSongkranEventManager()->GetSpecialRewardItem( m_dwIndex + 1 );

	if ( !pReward )
		return;

	if ( pReward->m_vecReward.empty() )
		return;

	m_dwIndex++;

	UpdateSpecialReward();
}



void KGCSongkranDlg::Init()
{
	m_nCurFrame = 0;
	m_nCharAnimationFrame = 0;
	m_bResultShot = false;
	m_dwIndex = 0;
	m_nBombState = BOMB_STATE_READY;

	m_fMoveSpeed[GCSongkranEventManager::CHAR_ELESIS] = SiGCSongkranEventManager()->GetTargetSpeed( GCSongkranEventManager::CHAR_ELESIS );
	m_fMoveSpeed[GCSongkranEventManager::CHAR_ZERO] = SiGCSongkranEventManager()->GetTargetSpeed( GCSongkranEventManager::CHAR_ZERO );
	m_fMoveSpeed[GCSongkranEventManager::CHAR_LAS] = SiGCSongkranEventManager()->GetTargetSpeed( GCSongkranEventManager::CHAR_LAS );

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
	UpdateReward();
	UpdateRewardReq();
	UpdateProgress();
}

void KGCSongkranDlg::UpdateBombCount()
{
	int nCount = SiGCSongkranEventManager()->GetBombCount();

	m_pkBombCount->SetNumber( nCount );
}

void KGCSongkranDlg::UpdateScore()
{
	m_pkGameScore->SetNumber( SiGCSongkranEventManager()->GetGameScore() );
}

void KGCSongkranDlg::UpdateReward()
{
	GCITEMID itemID = SiGCSongkranEventManager()->GetRewardItem();

	if ( itemID != 0 ) 
	{
		m_pkRewardImg->ToggleRender( true );
		m_pkRewardImg->SetItemInfo( itemID / 10  );
		m_pkRewardImg->ShowItem( true );

		GCItem* pItem = g_pItemMgr->GetItemData( itemID / 10 );

        if ( pItem ) {
			m_pkItemName->SetForceWordWrapText( pItem->strItemName , true);
            m_pkItemName->SetToolTip( pItem->strItemName );
        }
	}
	else
	{
		m_pkRewardImg->ToggleRender( false );
		m_pkRewardImg->ShowItem( false );

		m_pkItemName->SetText( L"" );
	}

	UpdateSpecialReward();
}

void KGCSongkranDlg::UpdateSpecialReward()
{
	KSongkranEventRewardInfo* pReward = SiGCSongkranEventManager()->GetSpecialRewardItem( m_dwIndex );

	if ( pReward && !pReward->m_vecReward.empty() ) 
	{
		KDropItemInfo kDrop = *(pReward->m_vecReward.begin());

		m_apkSpecialRewardImg[0]->ToggleRender( true );
		m_apkSpecialRewardImg[0]->SetItemInfo( kDrop.m_ItemID / 10  );
		m_apkSpecialRewardImg[0]->ShowItem( true );

		m_pkSpecialRewardScore[0]->SetForceWordWrapText( g_pkStrLoader->GetReplacedString( STR_ID_SONGKRAN_REWARD_POINT, "i", pReward->m_dwGradeEndScore ) , true );
        m_pkSpecialRewardScore[0]->SetToolTip( g_pkStrLoader->GetReplacedString( STR_ID_SONGKRAN_REWARD_POINT, "i", pReward->m_dwGradeEndScore ) );
	}
	else
	{
		m_apkSpecialRewardImg[0]->ToggleRender( false );
		m_apkSpecialRewardImg[0]->ShowItem( false );
		
		m_pkSpecialRewardScore[0]->SetText( L"" );
	}


	pReward = SiGCSongkranEventManager()->GetSpecialRewardItem( m_dwIndex + 1 );

	if ( pReward && !pReward->m_vecReward.empty() ) 
	{
		KDropItemInfo kDrop = *(pReward->m_vecReward.begin());

		m_apkSpecialRewardImg[1]->ToggleRender( true );
		m_apkSpecialRewardImg[1]->SetItemInfo( kDrop.m_ItemID / 10  );
		m_apkSpecialRewardImg[1]->ShowItem( true );

		m_pkSpecialRewardScore[1]->SetForceWordWrapText( g_pkStrLoader->GetReplacedString( STR_ID_SONGKRAN_REWARD_POINT, "i", pReward->m_dwGradeEndScore ) , true );
        m_pkSpecialRewardScore[1]->SetToolTip( g_pkStrLoader->GetReplacedString( STR_ID_SONGKRAN_REWARD_POINT, "i", pReward->m_dwGradeEndScore ) );
	}
	else
	{
		m_apkSpecialRewardImg[1]->ToggleRender( false );
		m_apkSpecialRewardImg[1]->ShowItem( false );

		m_pkSpecialRewardScore[1]->SetText( L"" );
	}
}

void KGCSongkranDlg::UpdateRewardReq()
{
	bool bHaveReward = SiGCSongkranEventManager()->IsHaveReward();

	if ( bHaveReward ) 
	{
		SiGCSongkranEventManager()->ReqRewardItem();
	}
}

void KGCSongkranDlg::UpdateProgress()
{
	float fRate = SiGCSongkranEventManager()->GetRewardRate();

	float fLength = MAX_BAR_LENGTH * fRate;
	m_pkPointBar->SetWidth( static_cast<DWORD>( fLength ) );

	
	DWORD dwScore = SiGCSongkranEventManager()->GetGameScore();
	DWORD dwEndScore = SiGCSongkranEventManager()->GetEndPoint();

	if ( dwEndScore != 0 ) 
		m_pkPointBar->SetToolTip( g_pkStrLoader->GetReplacedString( STR_ID_SONGKRAN_POINT_TOOLTIP, "ii", dwScore, dwEndScore )); 

}


void KGCSongkranDlg::UpdateBomb()
{
	//	ResetTargeting();

	switch ( m_nBombState )
	{
	case BOMB_STATE_READY:
		{
			//	현재 타겟에 적중인 아이들 찾는다.
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
				UpdateRewardReq();

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

void KGCSongkranDlg::ResetTargeting()
{
	//	반경 범위내 CharCheck 
	for ( int i = 0; i < GCSongkranEventManager::CHAR_SIZE; ++i ) 
	{
		if ( m_nTargetState[i] != TARGET_STATE_MOVE ) 
			continue;

		m_apkTarget[i]->SetWndMode( D3DWM_DEFAULT );
	}
}

void KGCSongkranDlg::UpdateTargeting()
{
	D3DXVECTOR2 vtScopePos = m_apkScope->GetFixedWindowLocalPos();
	DWORD dwScopeWidth = m_apkScope->GetWidth() / 2;

	float fBombRange = SiGCSongkranEventManager()->GetBombRange();

	float fLeft = (vtScopePos.x+dwScopeWidth) - fBombRange;
	float fRight = (vtScopePos.x+dwScopeWidth) + fBombRange;




	//	반경 범위내 CharCheck 
	for ( int i = 0; i < GCSongkranEventManager::CHAR_SIZE; ++i ) 
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

void KGCSongkranDlg::InitPlayerPos()
{
	D3DXVECTOR2 vtFixedWindowLocalPos = m_apkPlayer->GetFixedWindowLocalPos();

	vtFixedWindowLocalPos.x = fTARGET_START_POSX + ( fTARGET_GAP_SIZE * ( nTARGET_POS_NUM / 2 ) );

	m_apkPlayer->SetWindowPos( vtFixedWindowLocalPos );
	m_apkPlayer->SetWndMode( D3DWM_DEFAULT );
}

void KGCSongkranDlg::InitBombPos()
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

void KGCSongkranDlg::InitTargetPos()
{
	std::vector<int> vecShuffle;

	for ( int i = 0; i < nTARGET_POS_NUM; i+=2 )
	{
		vecShuffle.push_back( i ) ;
	}

	std::random_shuffle( vecShuffle.begin(), vecShuffle.end() );

	for ( int i = 0; i < GCSongkranEventManager::CHAR_SIZE; ++i ) 
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

void KGCSongkranDlg::UpdateKeyCheck() 
{
	//	오른쪽 
	if ( g_pkInput->IsPressed( DIK_RIGHT ) ) 
	{
		D3DXVECTOR2 vtFixedWindowLocalPos = m_apkPlayer->GetFixedWindowLocalPos();

		float fPosX = vtFixedWindowLocalPos.x;
		fPosX += SiGCSongkranEventManager()->GetPlayerSpeed();


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

		fPosX -= SiGCSongkranEventManager()->GetPlayerSpeed();

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

void KGCSongkranDlg::UpdatePlayerPos( D3DXVECTOR2 vPos )
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

void KGCSongkranDlg::UpdateBombPos()
{
	D3DXVECTOR2 vtFixedWindowLocalPos = m_apkBomb->GetFixedWindowLocalPos();

	vtFixedWindowLocalPos.y -= SiGCSongkranEventManager()->GetBombSpeed();
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

void KGCSongkranDlg::SetBombStateExplode()
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

	g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Waterboom_01", PARTICLEPOS_X(vtScopePos.x), PARTICLEPOS_Y(vtScopePos.y), 0.5f ), GC_LAYER_UPPER_SQUARE_UI_0);
	g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Waterboom_02", PARTICLEPOS_X(vtScopePos.x), PARTICLEPOS_Y(vtScopePos.y), 0.5f ), GC_LAYER_UPPER_SQUARE_UI_0);
	g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Waterboom_03", PARTICLEPOS_X(vtScopePos.x), PARTICLEPOS_Y(vtScopePos.y), 0.5f ), GC_LAYER_UPPER_SQUARE_UI_0);

}

void KGCSongkranDlg::SearchBombTarget( float fPosX )
{
	float fBombRange = SiGCSongkranEventManager()->GetBombRange();

	float fLeft = fPosX;
	float fRight = fPosX + ( fBombRange * 2 );


	int nGetScore = 0;
	int nKillNum = 0;
	int nIndex = 0;

	//	반경 범위내 CharCheck 
	for ( int i = 0; i < GCSongkranEventManager::CHAR_SIZE; ++i ) 
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
			int nScore = SiGCSongkranEventManager()->GetTargetScore( i );
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
			int nScore = SiGCSongkranEventManager()->GetTargetScore( i );
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

void KGCSongkranDlg::ReqGetScore( int nScore )
{
	KP2P::GetInstance()->Send_SongkranEventWaterbombUseReq( nScore );
}

void KGCSongkranDlg::AddScore( int nScore, bool _bDouble, int _nIndex )
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

void KGCSongkranDlg::UpdateTarget()
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


	for ( int i = 0; i < GCSongkranEventManager::CHAR_SIZE; ++i ) 
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




				if ( m_nTargetDeathFrame[i] >= SiGCSongkranEventManager()->GetResetFrame( i ) ) 
				{
					m_nTargetDeathFrame[i] = 0;
					m_nTargetState[i] = TARGET_STATE_MOVE;
					m_apkTarget[i]->Lock( false );
					m_apkTarget[i]->ToggleRender( true );

					if ( i == GCSongkranEventManager::CHAR_ARME || i == GCSongkranEventManager::CHAR_REY )
					{					
						int nTelIndex = TEL_CHAR_ARME + ( i - GCSongkranEventManager::CHAR_ARME );
						m_nTargetTelFrame[nTelIndex] = SiGCSongkranEventManager()->GetTeleportFrame( i );
					}
				}
			}
			break;
		}
	}

	if ( m_nTargetState[GCSongkranEventManager::CHAR_ARME] == TARGET_STATE_MOVE ) 
	{
		m_nTargetTelFrame[TEL_CHAR_ARME]++;
	}

	if ( m_nTargetState[GCSongkranEventManager::CHAR_REY] == TARGET_STATE_MOVE ) 
	{
		m_nTargetTelFrame[TEL_CHAR_REY]++;
	}


	UpdateTargetPos();
}

void KGCSongkranDlg::UpdateTargetPos()
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

	if ( m_nTargetState[GCSongkranEventManager::CHAR_ARME] == TARGET_STATE_MOVE &&
		m_nTargetTelFrame[TEL_CHAR_ARME] >= SiGCSongkranEventManager()->GetTeleportFrame( GCSongkranEventManager::CHAR_ARME ) ) 
	{
		m_nTargetTelFrame[TEL_CHAR_ARME] = 0;

		// 아르메
		int nRand = rand()%11;

		D3DXVECTOR2 vtFixedWindowLocalPos = m_apkTarget[GCSongkranEventManager::CHAR_ARME]->GetFixedWindowLocalPos();
		vtFixedWindowLocalPos.x = fTARGET_START_POSX + ( nRand * fTARGET_GAP_SIZE );
		m_apkTarget[GCSongkranEventManager::CHAR_ARME]->SetWindowPos( vtFixedWindowLocalPos );
	}

	if ( m_nTargetState[GCSongkranEventManager::CHAR_REY] == TARGET_STATE_MOVE &&
		m_nTargetTelFrame[TEL_CHAR_REY] >= SiGCSongkranEventManager()->GetTeleportFrame( GCSongkranEventManager::CHAR_REY ) ) 
	{

		m_nTargetTelFrame[TEL_CHAR_REY] = 0;
		// 레이
		D3DXVECTOR2 vtFixedWindowLocalPos = m_apkTarget[GCSongkranEventManager::CHAR_REY]->GetFixedWindowLocalPos();
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

		m_apkTarget[GCSongkranEventManager::CHAR_REY]->SetWindowPos( vtFixedWindowLocalPos );
	}
}

bool KGCSongkranDlg::IsBombShot()
{
	//	물폭탄이 없다.
	if ( SiGCSongkranEventManager()->GetBombCount() <= 0 ) 
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_SONGKRAN_EVENT_ERROR01),
								 L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
		return false;
	}

	//	보상을 받아야지 던질수 있습니다.
 	if ( SiGCSongkranEventManager()->IsHaveReward() )
 	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_SONGKRAN_EVENT_ERROR02),
								 L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
 		return false;
 	}

	return true;
}

void KGCSongkranDlg::UpdateResultScore()
{
	m_bResultShot = true;
	//	물폭탄 갯수 업데이트
	UpdateBombCount();
	
	//	점수 업데이트
	UpdateScore();

	//	보상 받을수 있는지 업데이트
	UpdateReward();
	UpdateProgress();
}

void KGCSongkranDlg::UpdateRewardData()
{
	//	보상 받을수 있는지 업데이트
	m_dwIndex = 0;

	UpdateReward();
	UpdateRewardReq();
	UpdateProgress();
}

void KGCSongkranDlg::ThrowBomb()
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

void KGCSongkranDlg::InitCharMarkPos()
{
	D3DXVECTOR2 vtFixedWindowLocalPos = m_apkPlayerMark->GetFixedWindowLocalPos();

	vtFixedWindowLocalPos.y = -1000;

	m_apkPlayerMark->SetWindowPos( vtFixedWindowLocalPos );

	m_bRenderCharMark = false;
}

void KGCSongkranDlg::InitGetPointBack()
{
	for ( int i = 0; i < EM_HIT_POINT_SIZE; ++i ) 
	{
		D3DXVECTOR2 vtFixedWindowLocalPos = m_apkPointBack[i]->GetFixedWindowLocalPos();

		vtFixedWindowLocalPos.y = -1000;

		m_apkPointBack[i]->SetWindowPos( vtFixedWindowLocalPos );
		m_apkPointNum[i]->SetWindowPos( vtFixedWindowLocalPos );
	}
}

void KGCSongkranDlg::FailScore()
{
	D3DXVECTOR2 vtCharPos = m_apkPlayer->GetFixedWindowLocalPos();

	m_apkPlayerMark->SetWindowPos( vtCharPos );

	g_KDSound.Play( "songkran_4" );

	m_bRenderCharMark = true;
}

void KGCSongkranDlg::GetRewardItem()
{
	g_KDSound.Play( "songkran_6" );
}

void KGCSongkranDlg::Close()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
    KGCHotEventBtn::SetHotStateOff();
}
