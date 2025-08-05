#include "stdafx.h"
#include "GCRitasPanel.h"

IMPLEMENT_CLASSNAME( KGCRitasPanel );
IMPLEMENT_WND_FACTORY( KGCRitasPanel );
IMPLEMENT_WND_FACTORY_NAME( KGCRitasPanel, "gc_ritaspanel" );



const DWORD dwEndStage_LmitDelayTime = 4000;

KGCRitasPanel::KGCRitasPanel( void )
{

	m_pkPlayer = NULL;
	LINK_CONTROL( "player", m_pkPlayer );

	m_pkGoalBasket = NULL;
	LINK_CONTROL( "goal_basket", m_pkGoalBasket );

	m_pkStageRect = NULL;
	LINK_CONTROL( "stage_rect", m_pkStageRect );

	m_pkGoalRect = NULL;
	LINK_CONTROL( "goal_rect", m_pkGoalRect );


	m_pkStaticStageNum = NULL;
	LINK_CONTROL( "static_stage_num", m_pkStaticStageNum );

	m_pkStaticGoalObjNum = NULL;
	LINK_CONTROL( "static_goal_obj", m_pkStaticGoalObjNum );

	m_pkGameScore = NULL;
	LINK_CONTROL( "static_game_score", m_pkGameScore );

	m_pkObjScore=NULL;
	LINK_CONTROL( "static_obj_score", m_pkObjScore );

	m_pkProgressBar = NULL;
	LINK_CONTROL( "progressbar", m_pkProgressBar );
	m_pkProgressBarRect= NULL;
	LINK_CONTROL( "progressbar_rect", m_pkProgressBarRect );

	m_pkImgGameover = NULL;
	LINK_CONTROL( "gameover_img", m_pkImgGameover );
	m_pkImgGmaeClear = NULL;
	LINK_CONTROL( "gameclear_img", m_pkImgGmaeClear );

	m_pkProgressBlinder = NULL;
	LINK_CONTROL( "progress_blinder", m_pkProgressBlinder );

	m_pkStaticFeverObjNum = NULL;
	LINK_CONTROL( "static_fever_obj_num", m_pkStaticFeverObjNum );
	m_pkStaticGotBoxInStage = NULL;
	LINK_CONTROL( "static_box_num", m_pkStaticGotBoxInStage );

	m_fCatchZoneTop = 0.0f;
	m_fCatchZoneBottom = 0.0f;

	m_mapObjPool.clear();
	m_vecBasket.clear();
	m_mapSlotPos.clear();
	m_vecClearStage.clear();
	m_vecNoamlObjID.clear();
	m_vecFeverObjID.clear();

	m_vecFeverEffect.clear();
	m_vecBackEffect.clear();
	m_vecAlramEffect.clear();
	m_vecSpaceEffect.clear();

	m_eNowState = BEFORE_GAME;
	m_ePlayType = PLAYTYPE_NOMAL;

	m_bSetPlayInfo = false;
	m_iGotObj = 0;
	m_iGotFeverObj = 0;
	m_iTempFeverScore = 0;

	char szName[MAX_PATH];
	for (int i=0 ; i<MAX_CNT_NUM ; i++)
	{
		m_pkCnt[i] = NULL;
		sprintf( szName, "cnt%d", i );
		LINK_CONTROL( szName, m_pkCnt[i] );

		m_bCountDownFlag[i] = false;
	}

	for (int i=0 ; i<MAX_OBJICON_NUM ; i++)
	{
		m_pkObjIcon[i] = NULL;
		sprintf( szName, "ritas_obj_icon%d", i );
		LINK_CONTROL( szName, m_pkObjIcon[i] );
	}

	m_dwTickCntForCount = 0;
	m_dwTickCntForPlay = 0;
	m_dwTickCntForGenerate = 0;

	m_iStartType = KEVENT_RITAS_CHRISTMAS_STAGE_START_REQ::ST_FIRST;

	m_kCurrenStageInfo.ClearInfo();

	m_fLeftTime = 0.0f;
	m_fValueXPerSec = 0.0f;

	m_fLeftFeverTime = 0.0f;

	m_dwEndStageDelayTime = 0;
	m_dwRevisionAtClearBasket = 0;

	m_bClear = false;
	
}

KGCRitasPanel::~KGCRitasPanel( void )
{
}
void KGCRitasPanel::OnCreate( void )
{
	m_pkPlayer->InitState(true);

	m_pkGoalBasket->InitState(true);

	m_pkStageRect->InitState(false);

	m_pkGoalRect->InitState(false);

	m_pkGameScore->InitState(true);
	m_pkGameScore->SetStringType( "ritas_game_score" );
	m_pkGameScore->SetNumber( 0 );

	m_pkObjScore->InitState(true);
	m_pkObjScore->SetStringType( "ritas_obj_score" );
	m_pkObjScore->SetNumber( 0 );
	//m_pkObjScore->SetFontSize(SiKGCMultipleLanguages()->GetLargeFontSize());

	m_pkProgressBar->InitState(true);
	m_pkProgressBarRect->InitState(false);

	m_pkStaticStageNum->InitState(true);
	m_pkStaticStageNum->SetAlign(DT_CENTER);
	m_pkStaticGoalObjNum->InitState(true);
	m_pkStaticGoalObjNum->SetAlign(DT_LEFT);

	m_pkImgGameover->InitState(false);
	m_pkImgGmaeClear->InitState(false);

	m_pkProgressBlinder->InitState(false);

	m_pkStaticFeverObjNum->InitState(true);
	m_pkStaticFeverObjNum->SetAlign(DT_LEFT);
	m_pkStaticGotBoxInStage->InitState(true);
	m_pkStaticGotBoxInStage->SetAlign(DT_LEFT);

	for (int i=0 ; i<MAX_CNT_NUM ; i++)
	{
		m_pkCnt[i]->InitState(false);
		m_bCountDownFlag[i] = false;
	}

	for (int i=0 ; i<MAX_OBJICON_NUM ; i++)
		m_pkObjIcon[i]->InitState(false);

	InitStageInfo();
	m_eNowState = BEFORE_GAME;
	
}

void KGCRitasPanel::ActionPerformed( const KActionEvent& event )
{


}

void KGCRitasPanel::FrameMoveInEnabledState()
{

	switch(m_eNowState)
	{
	case BEFORE_GAME:
		break;
	case SET_STAGE:
		SetStageFrameMove();
		break;
	case COUNT:
		CountFrameMove();
		break;
	case PLAY:
		PlayFrameMove();
		break;
	case END_STAGE:
		EndStageFramdMove();
		break;
	}	
}

void KGCRitasPanel::UpdatePlayer()
{

	m_dwRevisionAtClearBasket = -1;

	if ( g_pkInput->IsPressed( DIK_RIGHT ) ) 
	{
		MoveRight();
	
	}
	else if ( g_pkInput->IsPressed( DIK_LEFT ) )
	{
		MoveLeft();
	}


	if ( CheckGoalZone() )
	{
		if ( true != m_vecBasket.empty() ) 
		{

			SetShowSpaceParticle(true);

			if (g_pkInput->IsUp( DIK_SPACE ))
			{
				KEVENT_RITAS_CHRISTMAS_EMPTY_BASKET_REQ kPacket;
				kPacket = m_vecBasket;
				Result_ReceiveChristmasBasketEmpty = INT_MAX;
				KP2P::GetInstance()->Send_RitasChristmas_EmptyBasket_Req(kPacket);

				DWORD dwStart = ::GetTickCount();
				g_MyD3D->WaitForServerAck(Result_ReceiveChristmasBasketEmpty,INT_MAX,3000,TIME_OUT_VALUE);
				DWORD dwEndTime = ::GetTickCount();
				m_dwRevisionAtClearBasket = dwEndTime - dwStart;

				m_iGotObj += m_vecBasket.size();

				ClearBasketEffect();

				UpDateScore();

				SiGCRitasChrismasManager()->UpdateComboInfo(m_vecBasket);
				m_pkGoalBasket->UpDateComboInfo();

				if ( m_ePlayType == PLAYTYPE_NOMAL && !EndStageCheck() )
				{
					if ( SiGCRitasChrismasManager()->IsClearedAllCombo())
					{
						m_ePlayType = PLAYTYPE_FEVER;
						m_fLeftFeverTime = m_kCurrenStageInfo.m_fFeverTimeDuration;
						g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Lita_Fever_Start_01", 0.0f, 0.0f, PARTICLEPOS_Z ), GC_LAYER_UI);
						g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Lita_Fever_Start_02", 0.0f, 0.0f, PARTICLEPOS_Z ), GC_LAYER_UI);
						g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Lita_Fever_Start_03", 0.0f, 0.0f, PARTICLEPOS_Z ), GC_LAYER_UI);
						g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Lita_Fever_Start_04", 0.0f, 0.0f, PARTICLEPOS_Z ), GC_LAYER_UI);
						g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Lita_Fever_Start_05", 0.0f, 0.0f, PARTICLEPOS_Z ), GC_LAYER_UI);
						g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Lita_Fever_Start_06", 0.0f, 0.0f, PARTICLEPOS_Z ), GC_LAYER_UI);
						g_KDSound.Play( "Ritas_Bonus_Time_Start" );

						SetShowFeverParticle(true);
						SetShowBackParticle(true);

					}

				}
				ClearBasket();	
				g_KDSound.Play( "Ritas_Empty_Basket" );
			}
		}
		else
		{
			SetShowSpaceParticle(false);
		}
	}
	else
	{
		if (m_vecBasket.size() == SiGCRitasChrismasManager()->GetPlayInfo()->m_nBasketSize)
			SetShowSpaceParticle(true);
		else
			SetShowSpaceParticle(false);
	}

	if (g_pkInput->IsUp( DIK_UP ))
	{
		PopTopObj();
		g_KDSound.Play( "Ritas_Empty_Basket_Top" );

	}
}

void KGCRitasPanel::MoveRight()
{
	D3DXVECTOR2 vPos = m_pkPlayer->GetBasketPos();
	float fTargetPosX = vPos.x + SiGCRitasChrismasManager()->GetPlayInfo()->m_fPlayerSpeed;

	if ( (fTargetPosX + m_pkPlayer->GetBasketWidth()) < (m_pkStageRect->GetFixedWindowLocalPos().x + m_pkStageRect->GetWidth()) )
	{
		m_pkPlayer->SetDirectionRight(true);
		D3DXVECTOR2 vTargetPos= m_pkPlayer->GetFixedWindowLocalPos();
		vTargetPos.x += SiGCRitasChrismasManager()->GetPlayInfo()->m_fPlayerSpeed;
		m_pkPlayer->SetFixedWindowLocalPos(vTargetPos);
		
	}

}

void KGCRitasPanel::MoveLeft()
{
	D3DXVECTOR2 vPos = m_pkPlayer->GetBasketPos();
	float fTargetPosX = vPos.x - SiGCRitasChrismasManager()->GetPlayInfo()->m_fPlayerSpeed;

	if ( fTargetPosX > m_pkStageRect->GetFixedWindowLocalPos().x )
	{
		m_pkPlayer->SetDirectionRight(false);
		D3DXVECTOR2 vTargetPos = m_pkPlayer->GetFixedWindowLocalPos();
		vTargetPos.x -= SiGCRitasChrismasManager()->GetPlayInfo()->m_fPlayerSpeed;
		m_pkPlayer->SetFixedWindowLocalPos(vTargetPos);
		
	}

}

void KGCRitasPanel::SetCatchZone()
{
	D3DXVECTOR2 vPos = m_pkStageRect->GetFixedWindowLocalPos();
	m_fCatchZoneTop = vPos.y + (m_pkStageRect->GetHeight() * SiGCRitasChrismasManager()->GetPlayInfo()->m_fCatchZoneTop);
	m_fCatchZoneBottom = vPos.y + (m_pkStageRect->GetHeight() * SiGCRitasChrismasManager()->GetPlayInfo()->m_fCatchZoneBottom);
}

void KGCRitasPanel::InitGameInfo()
{

	m_eNowState = BEFORE_GAME;
 	if (!m_bSetPlayInfo)
 	{
		//여기다 PlayInfo 세팅해준다.
		SetCatchZone();

		D3DXVECTOR2 vRecPos = m_pkStageRect->GetFixedWindowLocalPos();
		DWORD dwStageWidth = m_pkStageRect->GetWidth();
		DWORD dwWidthTerm = dwStageWidth/SiGCRitasChrismasManager()->GetPlayInfo()->m_nSlotNum;

		for (int i=0 ; i<SiGCRitasChrismasManager()->GetPlayInfo()->m_nSlotNum ; i++)
		{
			D3DXVECTOR2 vSlotPos;
			vSlotPos.x = vRecPos.x + static_cast<float>(i*dwWidthTerm);
			vSlotPos.y = vRecPos.y;
			m_mapSlotPos.insert(std::make_pair(i,vSlotPos));
		}
		
		std::map< int, KRitasChristmasObjectInfo > mapObjContainer = SiGCRitasChrismasManager()->GetObjContainer();
		std::map< int, KRitasChristmasObjectInfo >::iterator mit = mapObjContainer.begin();
		for (; mit != mapObjContainer.end() ; ++mit)
		{
			if (mit->second.m_nObjectType == KRitasChristmasObjectInfo::OT_NORMAL)
				m_vecNoamlObjID.push_back(mit->second.m_nObjectID);
			else if (mit->second.m_nObjectType == KRitasChristmasObjectInfo::OT_FEVER)
				m_vecFeverObjID.push_back(mit->second.m_nObjectID);
		}
		m_bSetPlayInfo = true;
		CreateParticle();
	}

	m_vecClearStage.clear();
	m_iStartType = KEVENT_RITAS_CHRISTMAS_STAGE_START_REQ::ST_FIRST;
	InitStageInfo();

}

void KGCRitasPanel::CheckCatchObj(KGCRitasObj* pkObj_ , int nObjID_)
{
	if (!pkObj_->GetCanCatch())
		return;

	if ( m_pkPlayer->CheckCatchObj(pkObj_) )
	{
		if (pkObj_->GetObjType() == KRitasChristmasObjectInfo::OT_FEVER)
		{

			//보여주기식 연산! 서버에 매번 점수 확인은 부화가 있기때문에 로컬 자체에서 연산 하여 보여주기만 한다.
			std::map< int, KRitasChristmasObjectInfo > mapObjContainer = SiGCRitasChrismasManager()->GetObjContainer();
			std::map< int, KRitasChristmasObjectInfo >::iterator mit = mapObjContainer.find(nObjID_);

			if ( mit != mapObjContainer.end() )
			{

				int iAddPoint = mit->second.m_nPoint;
				m_iTempFeverScore += iAddPoint;
				m_pkGameScore->SetNumber( SiGCRitasChrismasManager()->GetCurrentPoint() + m_iTempFeverScore );
			}
			m_iGotFeverObj++;

			UpDateFeverObjAndBoxNum();
			
		}
		else
		{
			if (static_cast<int>(m_vecBasket.size()) >= SiGCRitasChrismasManager()->GetPlayInfo()->m_nBasketSize)
				return;
			m_vecBasket.push_back(nObjID_);
		}

// 		D3DXVECTOR2 vPos(GetRelocatedWindowPos());
		D3DXVECTOR2 vPos = pkObj_->GetCurrentWindowPos();
// 		vPos += pkObj_->GetFixedWindowLocalPos();
 		vPos.x += pkObj_->GetWidth()/2.0f;
 		vPos.y += pkObj_->GetHeight()/2.0f;

		g_KDSound.Play( "Ritas_Get_Object" );

		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Lita_Get_01", PARTICLEPOS_X(vPos.x) , PARTICLEPOS_Y(vPos.y), PARTICLEPOS_Z ), GC_LAYER_UI);
		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Lita_Get_02", PARTICLEPOS_X(vPos.x) , PARTICLEPOS_Y(vPos.y), PARTICLEPOS_Z ), GC_LAYER_UI);

		pkObj_->InitInfo(D3DXVECTOR2(-1000.0f,-1000.0f) , pkObj_->GetObjType() , false);

		UpdateBasket();
	}

}

void KGCRitasPanel::UpdateRitasObj()
{
	//Null이라면 SetStage 실패를 했다는 뜻
// 	if (m_kCurrenStageInfo == NULL)
// 		return;

	//m_vecNoamlObjID , m_vecFeverObjID 가 비어 있다면 오브젝트 정보를 받아오지 못했다는 뜻
	if (m_vecNoamlObjID.empty() || m_vecFeverObjID.empty())
		return;

	float fGenerateSec = 1.0f;
	if (m_ePlayType == PLAYTYPE_NOMAL)
		fGenerateSec = m_kCurrenStageInfo.m_fGenerateSec;
	else if(m_ePlayType == PLAYTYPE_FEVER)
		fGenerateSec = m_kCurrenStageInfo.m_fFeverGenerateSec;

	DWORD dwTmpGenerateTime = ::GetTickCount();
	if (static_cast<float>(dwTmpGenerateTime - m_dwTickCntForGenerate)/1000.0f >= fGenerateSec)
	{
		GenerateObj();
		m_dwTickCntForGenerate = ::GetTickCount();
	}
	
	MoveObj();
}

void KGCRitasPanel::GenerateObj()
{
	int iGenerateNum = 0;
	
	if (m_ePlayType == PLAYTYPE_NOMAL)
		iGenerateNum = m_kCurrenStageInfo.m_nGenerateNum;
	else if(m_ePlayType == PLAYTYPE_FEVER)
		iGenerateNum = m_kCurrenStageInfo.m_nFeverGenerateNum;

	for (int i=0 ; i<iGenerateNum ; i++)
	{
		int iObjID;
		int iContainerIndex = 0;
		int iObjType;
		switch(m_ePlayType)
		{
		case PLAYTYPE_NOMAL:
			iContainerIndex = ::rand()%m_vecNoamlObjID.size();
			iObjType = KRitasChristmasObjectInfo::OT_NORMAL;
			iObjID = m_vecNoamlObjID[iContainerIndex];
			break;
		case PLAYTYPE_FEVER:
			iContainerIndex = ::rand()%m_vecFeverObjID.size();
			iObjType = KRitasChristmasObjectInfo::OT_FEVER;
			iObjID = m_vecFeverObjID[iContainerIndex];
			break;
		}
		

		D3DXVECTOR2 vPos(0.0f,0.0f);
		int iSlotNum = ::rand()%SiGCRitasChrismasManager()->GetPlayInfo()->m_nSlotNum;
		std::map<int,D3DXVECTOR2>::iterator mitPos = m_mapSlotPos.find(iSlotNum);
		if ( mitPos == m_mapSlotPos.end())
		{
			return;
		}
		else
		{
			vPos = mitPos->second;
		}


		std::map< int , std::vector<KGCRitasObj*> >::iterator mit = m_mapObjPool.find(iObjID);
		if ( mit == m_mapObjPool.end() )
		{
			CreateObj(iObjID , vPos , iObjType);
		}
		else
		{
			std::vector<KGCRitasObj*>::iterator vit = mit->second.begin();
			for (;vit != mit->second.end() ; ++vit)
			{
				if ( (*vit) )
				{
					if ( (*vit)->GetLive() )
					{
						continue;
					}
					else
					{
						(*vit)->InitInfo(vPos , iObjType);
						return;
					}
				}
			}
			//여기까지 왔다면 Pool에 가용 Obj가 없다는 뜻..만들어야지.
			CreateObj(iObjID , vPos , iObjType);
		}
	}
}


bool KGCRitasPanel::CheckCatchZone( float fPosY_ )
{
	if ( fPosY_ > m_fCatchZoneTop 
		&& fPosY_ < m_fCatchZoneBottom )
		return true;

	return false;
}

void KGCRitasPanel::CheckPos( KGCRitasObj* pkObj_ , int nObjID_ , float fMoveRate_)
{
	//이미 이곳에 들어오기전에 pkObj==Null 이 되 있으므로 하지 않는다.

	if (pkObj_->GetOnBottom())
		return;

	D3DXVECTOR2 vObjPos = pkObj_->GetFixedWindowLocalPos();
	std::map< int, KRitasChristmasObjectInfo > mapObjectContainer = SiGCRitasChrismasManager()->GetObjContainer();
	std::map< int, KRitasChristmasObjectInfo >::iterator mit = mapObjectContainer.find(nObjID_);

	float fSpeed = 0.0f;
	float fDecreaseTime = 0.0f;
	if (mit != mapObjectContainer.end())
	{
		fSpeed = mit->second.m_fDownSpeed;
		fDecreaseTime = mit->second.m_fDecreaseTime;
	}

	vObjPos.y += (fSpeed * fMoveRate_ * m_kCurrenStageInfo.m_fDownSpeedIncreaseRatio);

	//스테이지보다 아래로 떨어지면 안되기때문에
	//오브젝트를 바로 없애지 않고 바닥에 남김
	D3DXVECTOR2 vStagePos = m_pkStageRect->GetFixedWindowLocalPos();
	if ( (vObjPos.y + static_cast<float>(pkObj_->GetHeight()))
		>= (vStagePos.y + static_cast<float>(m_pkStageRect->GetHeight())) )
	{
		vObjPos.y = vStagePos.y + static_cast<float>(m_pkStageRect->GetHeight()) - static_cast<float>(pkObj_->GetHeight());
		pkObj_->SetOnBottom(true);
		if (pkObj_->GetObjType() == KRitasChristmasObjectInfo::OT_NORMAL)
		{
			DecreasLeftTime(fDecreaseTime);
		}
	}

	pkObj_->SetCanCatch( CheckCatchZone(vObjPos.y) );
	pkObj_->SetFixedWindowLocalPos(vObjPos);

	CheckCatchObj(pkObj_ ,nObjID_ );
}

void KGCRitasPanel::MoveObj()
{
	DWORD dwTmpPlayTime = ::GetTickCount() - m_dwTickCntForPlay;

	//스페이스바 에 의 한 렉이 발생시에 흐르는 시간 방지
	if (m_dwRevisionAtClearBasket != -1)
		dwTmpPlayTime += m_dwRevisionAtClearBasket;


	float fTime = static_cast<float>(dwTmpPlayTime)/1000.f;

	std::map< int , std::vector<KGCRitasObj*>>::iterator mit = m_mapObjPool.begin();
	for (;mit != m_mapObjPool.end() ; ++mit)
	{
		std::vector<KGCRitasObj*>::iterator vit = mit->second.begin();
		for (;vit != mit->second.end() ; ++vit)
		{
			if ( *vit )
			{
				if ((*vit)->GetLive())
				{
					CheckPos( (*vit) , mit->first , fTime );
				}
			}
		}
	}

	switch(m_ePlayType)
	{
	case PLAYTYPE_NOMAL:
		DecreasLeftTime(fTime);
		break;
	case PLAYTYPE_FEVER:
		DecreasFeverLeftTime(fTime);
		break;
	}
		

	m_dwTickCntForPlay = ::GetTickCount();

}

void KGCRitasPanel::CreateObj(int iObjID_ , D3DXVECTOR2 vPos_ , int iType )
{

	KD3DWnd* pWnd = NULL;
	char strWndName[MAX_PATH] = {0,};
	sprintf( strWndName, "ritaschristmas_dlg\\ritaspanel\\obj%d", iObjID_ );
	pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_RitasChristmas.stg", "ritasobj",strWndName );
	if (pWnd == NULL)
		return;

	KGCRitasObj* pRitasObj = static_cast<KGCRitasObj*>(pWnd);
	pRitasObj->InitInfo(vPos_ , iType);
	pRitasObj->ToggleRender(true);

	std::map< int , std::vector<KGCRitasObj*>>::iterator mit = m_mapObjPool.find(iObjID_);
	if (mit == m_mapObjPool.end())
	{
		std::vector<KGCRitasObj*> vecTmp;
		vecTmp.push_back(pRitasObj);
		m_mapObjPool.insert( std::make_pair(iObjID_,vecTmp) );
	}
	else
	{
		mit->second.push_back(pRitasObj);
	}
}

void KGCRitasPanel::ResSetObj()
{
	std::map< int , std::vector<KGCRitasObj*>>::iterator mit = m_mapObjPool.begin();
	for (;mit != m_mapObjPool.end() ; ++mit)
	{
		std::vector<KGCRitasObj*>::iterator vit = mit->second.begin();
		for (;vit != mit->second.end() ; ++vit)
		{
			if ( *vit )
			{
				(*vit)->InitInfo(D3DXVECTOR2(-1000.0f,-1000.0f) ,(*vit)->GetObjType() , false);
			}
		}
	}
}

void KGCRitasPanel::SetStageInfo(int iStage_ )
{
	InitStageInfo();

	m_kCurrenStageInfo.ClearInfo();


	std::map< int, KRitasChristmasStageInfo > mapStageContainer = SiGCRitasChrismasManager()->GetStageContainer();
	std::map< int, KRitasChristmasStageInfo >::iterator mit = mapStageContainer.find(iStage_);
	if ( mit == mapStageContainer.end() )
		return;

	m_kCurrenStageInfo = mit->second;

	m_pkStaticStageNum->SetText( g_pkStrLoader->GetReplacedString(STR_ID_RITASCHRISTMAS_STAGE, "i", m_kCurrenStageInfo.m_nStageNum ) );

	m_fLeftTime = m_kCurrenStageInfo.m_fTimeLimit;
	m_fLeftFeverTime = m_kCurrenStageInfo.m_fFeverTimeDuration;

	char szName[MAX_PATH];
	sprintf( szName, "/%d", m_kCurrenStageInfo.m_nGoalCount );

	m_pkStaticGoalObjNum->SetText( GCUTIL_STR::GCStrCharToWide(szName) );

	m_pkGameScore->SetNumber( SiGCRitasChrismasManager()->GetCurrentPoint() );
	m_pkObjScore->SetNumber( m_iGotObj );

	D3DXVECTOR2 vProgressBar = m_pkProgressBarRect->GetFixedWindowLocalPos();
	DWORD dwTotalWidth = m_pkProgressBarRect->GetWidth();
	vProgressBar.x += static_cast<float>(dwTotalWidth);
	m_pkProgressBar->SetFixedWindowLocalPos(vProgressBar);
	m_fValueXPerSec = static_cast<float>(dwTotalWidth)/m_kCurrenStageInfo.m_fTimeLimit;
	m_pkProgressBar->SetTime(m_fLeftTime);

	SiGCRitasChrismasManager()->GenerateCombo(m_kCurrenStageInfo.m_nStageNum);
	m_pkGoalBasket->UpDateComboInfo();
}

void KGCRitasPanel::CountFrameMove()
{
	DWORD dwTmpTickCnt = ::GetTickCount() - m_dwTickCntForCount;

	if (dwTmpTickCnt < 1000 )
	{
		if (false == m_bCountDownFlag[3])
		{
			g_KDSound.Play("Ritas_CountDown");
			SetNumber(3);

			m_bCountDownFlag[3] = true;
		}	
	}
	else if (dwTmpTickCnt > 1000 && dwTmpTickCnt<2000)
	{
		if (false == m_bCountDownFlag[2])
		{
			g_KDSound.Play("Ritas_CountDown");
			SetNumber(2);
			m_bCountDownFlag[2] = true;
		}
		
	}
	else if (dwTmpTickCnt > 2000 && dwTmpTickCnt<3000)
	{
		if (false == m_bCountDownFlag[1])
		{
			g_KDSound.Play("Ritas_CountDown");
			SetNumber(1);
			m_bCountDownFlag[1] = true;
		}
		
	}
	else if (dwTmpTickCnt > 3000 && dwTmpTickCnt<4000)
	{
		if (false == m_bCountDownFlag[0])
		{
			g_KDSound.Play("Ritas_CountDown_Start");
			//Start UI 출력 부분
			SetNumber(0);
			m_bCountDownFlag[0] = true;
		}
		
	}
	else if (dwTmpTickCnt > 4000)
	{	
		SetNumber(-1);
		
		SetRitasState(PLAY);
		m_dwTickCntForCount = 0;

		m_dwTickCntForPlay = ::GetTickCount();
		m_dwTickCntForGenerate =::GetTickCount();		
	}
}

void KGCRitasPanel::SetTickCntForCount( DWORD dwTickCnt_ )
{
	m_dwTickCntForCount = dwTickCnt_;
}

void KGCRitasPanel::SetNumber( int iNum_ )
{
	//편의때문에..ㅡ,.ㅡ ㅎ -1 넣으면 넘버 다 지우는 식으로
	if (iNum_ == -1)
	{
		for (int i=0 ; i<MAX_CNT_NUM ; i++)
			m_pkCnt[i]->ToggleRender(false);
		return;
	}

	if (iNum_ < 0 || iNum_>5 )
		return;

	for (int i=0 ; i<MAX_CNT_NUM ; i++)
		m_pkCnt[i]->ToggleRender(false);

	m_pkCnt[iNum_]->ToggleRender(true);

}

void KGCRitasPanel::PopTopObj()
{
	if (m_vecBasket.empty())
		return;

	m_vecBasket.pop_back();
	UpdateBasket();
	
}

void KGCRitasPanel::UpdateBasket()
{
	for (int i=0 ; i<MAX_OBJICON_NUM ; i++)
		m_pkObjIcon[i]->ToggleRender(false);

	if (m_vecBasket.empty())
		return;

	std::vector<int>::iterator vit = m_vecBasket.begin();
	for (int i = 0 ; vit != m_vecBasket.end() ; ++vit , i++)
	{
		//안전코드.. 하지만 이 경우가 가쳐질 경우가 생기면 안된다.
		if ( (*vit) < 0 || (*vit) > KGCRitasGoalBasket::OBJ_KIND_NUM )
			continue;
		if ( i>10 )
			continue;

		m_pkObjIcon[i]->SetWndMode(static_cast<ED3DWndMode>(*vit));
		m_pkObjIcon[i]->ToggleRender(true);
	}

}

void KGCRitasPanel::ClearBasket()
{
	for (int i=0 ; i<MAX_OBJICON_NUM ; i++)
		m_pkObjIcon[i]->ToggleRender(false);

	m_vecBasket.clear();
}

bool KGCRitasPanel::CheckGoalZone()
{
	D3DXVECTOR2 vPlayerPos = m_pkPlayer->GetFixedWindowLocalPos();
	D3DXVECTOR2 vGoalZonePos = m_pkGoalRect->GetFixedWindowLocalPos();

	//플레이어 오브젝트의 반 이상이 골 존에 있을시에 true
	float fCheckPosX = vPlayerPos.x + static_cast<float>(m_pkPlayer->GetWidth());

	if ( fCheckPosX > vGoalZonePos.x && 
		fCheckPosX <( vGoalZonePos.x + static_cast<float>(m_pkGoalRect->GetWidth())) )
		return true;
	else
		return false;
	
}

bool KGCRitasPanel::IsCleared( int iStageNum_ )
{
	if (m_vecClearStage.empty())
		return false;
	
	std::vector<int>::iterator vit = m_vecClearStage.begin();
	for (; vit != m_vecClearStage.end() ; ++vit)
	{
		if (iStageNum_ == (*vit))
			return true;
	}

	return false;
	
}

bool KGCRitasPanel::EndStageCheck()
{
	if (m_iGotObj >= m_kCurrenStageInfo.m_nGoalCount)
	{
		g_KDSound.Play( "Ritas_StageClear" );
		m_bClear = true;

		m_vecClearStage.push_back(m_kCurrenStageInfo.m_nStageNum);

		KEVENT_RITAS_CHRISTMAS_STAGE_END_REQ kPacket;
		kPacket.m_nEndType = KEVENT_RITAS_CHRISTMAS_STAGE_END_REQ::ET_STAGE_CLEAR;
		kPacket.m_nGetFeverObjectCount = m_iGotFeverObj;
		kPacket.m_nStageNum = m_kCurrenStageInfo.m_nStageNum;
		kPacket.m_fRemainTime = m_fLeftTime;
		Result_ReceiveChristmasStageClear = INT_MAX;
		KP2P::GetInstance()->Send_RitasChristmas_StageEnd_Req(kPacket);
		g_MyD3D->WaitForServerAck(Result_ReceiveChristmasStageClear , INT_MAX , 3000 , TIME_OUT_VALUE);

// 		//Clear effect
// 		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Lita_Stage_Clear_01", 0.0f, 0.0f, 0.5f ), GC_LAYER_UI);
// 		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Lita_Stage_Clear_02", 0.0f, 0.0f, 0.5f ), GC_LAYER_UI);
		m_pkImgGmaeClear->InitState(true);

		m_eNowState = END_STAGE;
		m_dwEndStageDelayTime = ::GetTickCount();

		return true;
	}
	return false;
}

void KGCRitasPanel::DecreasLeftTime( float fTime_ )
{
	if (m_fLeftTime - fTime_ < 0)
		m_fLeftTime = 0;
	else
		m_fLeftTime -= fTime_;


	UpDateTimeProgressBar();
}

void KGCRitasPanel::InitStageInfo()
{
	ClearBasket();
	ResSetObj();

	m_pkPlayer->SetFixedWindowLocalPos(SiGCRitasChrismasManager()->GetInitPlayerPos());
	m_pkPlayer->OnCreate();

	m_ePlayType = PLAYTYPE_NOMAL;

	m_dwTickCntForCount = 0;
	m_iGotObj = 0;
	m_iGotFeverObj = 0;
	m_iTempFeverScore = 0;

	m_fLeftTime = 0.0f;
	m_fLeftFeverTime = 0.0f;

	m_pkProgressBar->Init(m_pkProgressBarRect->GetFixedWindowLocalPos() , 0 );

	m_fValueXPerSec = 0.0f;

	SiGCRitasChrismasManager()->ClearComboInfo();
	AllSetParticle(false);

	for (int i=0 ; i<MAX_CNT_NUM ; i++)
		m_bCountDownFlag[i] = false;

	m_pkProgressBlinder->ToggleRender(false);

	m_pkStaticFeverObjNum->SetText(L"x 0");
	m_pkStaticGotBoxInStage->SetText(L"x 0");
}

void KGCRitasPanel::UpDateScore()
{
	m_pkObjScore->SetNumber( m_iGotObj );

	m_pkGameScore->SetNumber( SiGCRitasChrismasManager()->GetCurrentPoint() + m_iTempFeverScore);
}

void KGCRitasPanel::UpDateTimeProgressBar()
{
	if (m_fLeftTime <= 0.0f)
	{
		m_pkProgressBar->SetTime(0.0f);
		return;
	}

	float fpassedTime = m_kCurrenStageInfo.m_fTimeLimit - m_fLeftTime;
	float fValueX = fpassedTime*m_fValueXPerSec;

	D3DXVECTOR2 vPos = m_pkProgressBarRect->GetFixedWindowLocalPos();
	DWORD dwWidthRect = m_pkProgressBarRect->GetWidth();

	D3DXVECTOR2 vNewPosProgressBar(vPos);

	vNewPosProgressBar.x += static_cast<float>(dwWidthRect);
	vNewPosProgressBar.x -= fValueX;

	m_pkProgressBar->SetFixedWindowLocalPos(vNewPosProgressBar);

	D3DXVECTOR2 vTmpPos = m_pkProgressBarRect->GetFixedWindowLocalPos();
	vTmpPos.x += static_cast<float>(dwWidthRect);
	vTmpPos.x -= vNewPosProgressBar.x;

	m_pkProgressBar->SetBlindWidth(static_cast<DWORD>(vTmpPos.x));
	m_pkProgressBar->SetTime(m_fLeftTime);

	D3DXVECTOR2 vPosProgressBar = m_pkProgressBar->GetFixedWindowLocalPos();
	float fCheckX = vPosProgressBar.x + static_cast<float>(m_pkProgressBar->GetWidth());


	if ( static_cast<float>(fCheckX) >= m_pkProgressBlinder->GetFixedWindowLocalPos().x )
		m_pkProgressBlinder->ToggleRender(false);
	else
		m_pkProgressBlinder->ToggleRender(true);
}

void KGCRitasPanel::SetStageFrameMove()
{
	
}

void KGCRitasPanel::DecreasFeverLeftTime( float fTime_ )
{
	if (m_fLeftFeverTime - fTime_ < 0)
	{
		m_fLeftFeverTime = 0;
		m_ePlayType = PLAYTYPE_NOMAL;
		SiGCRitasChrismasManager()->GenerateCombo(m_kCurrenStageInfo.m_nStageNum);
		SetShowFeverParticle(false);
		SetShowBackParticle(false);
		m_pkGoalBasket->UpDateComboInfo();
	}
	else
	{
		g_KDSound.Play("Ritas_Bonus_Time");
		m_fLeftFeverTime -= fTime_;
	}

}

void KGCRitasPanel::EndStageFramdMove()
{
	DWORD dwTmpTime = ::GetTickCount();

	if ( (dwTmpTime - m_dwEndStageDelayTime) < dwEndStage_LmitDelayTime )
		return;

	SetShowAlramParticle(false);

	m_pkImgGameover->ToggleRender(false);
	m_pkImgGmaeClear->ToggleRender(false);
		
	bool bCheckLastStage = SiGCRitasChrismasManager()->GetClearLastStage();
	if (bCheckLastStage)
	{
		
		g_pkUIScene->m_pkRitasChristmasDlg->GetResultDlg()->SetDlgState(KGCRitasResultDlg::DLG_STATE_RESULT);
		g_pkUIScene->m_pkRitasChristmasDlg->GetResultDlg()->ToggleRender(true);
		m_eNowState = BEFORE_GAME;
		SiGCRitasChrismasManager()->InitTmepInfo();
		return;
	}

	if ( m_bClear )
	{
		SiGCRitasChrismasManager()->InitTmepInfo();
		m_iStartType = KEVENT_RITAS_CHRISTMAS_STAGE_START_REQ::ST_NEXT;
		OnStageStart();
	}
	else
	{
		
		g_pkUIScene->m_pkRitasChristmasDlg->GetResultDlg()->SetDlgState(KGCRitasResultDlg::DLG_STATE_RESULT);
		g_pkUIScene->m_pkRitasChristmasDlg->GetResultDlg()->ToggleRender(true);
		SiGCRitasChrismasManager()->InitTmepInfo();
		m_eNowState = BEFORE_GAME;
	}
}

void KGCRitasPanel::CreateParticle()
{
	m_vecFeverEffect.clear();
	m_vecBackEffect.clear();
	m_vecAlramEffect.clear();
	m_vecSpaceEffect.clear();

	CParticleEventSeqPTR pFeverPar1  = NULL;
	pFeverPar1 = g_ParticleManager->CreateSequence("UI_Lita_Fever_01", 0.0f, 0.0f, PARTICLEPOS_Z );
	CParticleEventSeqPTR pFeverPar2 = NULL;
	pFeverPar2 = g_ParticleManager->CreateSequence("UI_Lita_Fever_02", 0.0f, 0.0f, PARTICLEPOS_Z );

	if (pFeverPar1)
	{
		g_ParticleManager->SetLayer(pFeverPar1,GC_LAYER_UI);
		pFeverPar1->SetShow(false);
		m_vecFeverEffect.push_back(pFeverPar1);
	}

	if (pFeverPar2)
	{
		g_ParticleManager->SetLayer(pFeverPar2,GC_LAYER_UI);
		pFeverPar2->SetShow(false);
		m_vecFeverEffect.push_back(pFeverPar2);
	}
	
	

	CParticleEventSeqPTR pBackPar[8];
	for (int i=0 ; i<8 ; i++)
		pBackPar[i]=NULL;

	pBackPar[0] = g_ParticleManager->CreateSequence("UI_Lita_Back_01", 0.0f, 0.0f, PARTICLEPOS_Z );
	pBackPar[1] = g_ParticleManager->CreateSequence("UI_Lita_Back_02", 0.0f, 0.0f, PARTICLEPOS_Z );
	pBackPar[2] = g_ParticleManager->CreateSequence("UI_Lita_Back_03", 0.0f, 0.0f, PARTICLEPOS_Z );
	pBackPar[3] = g_ParticleManager->CreateSequence("UI_Lita_Back_04", 0.0f, 0.0f, PARTICLEPOS_Z );
	pBackPar[4] = g_ParticleManager->CreateSequence("UI_Lita_Back_05", 0.0f, 0.0f, PARTICLEPOS_Z );
	pBackPar[5] = g_ParticleManager->CreateSequence("UI_Lita_Back_06", 0.0f, 0.0f, PARTICLEPOS_Z );
	pBackPar[6] = g_ParticleManager->CreateSequence("UI_Lita_Back_07", 0.0f, 0.0f, PARTICLEPOS_Z );
	pBackPar[7] = g_ParticleManager->CreateSequence("UI_Lita_Back_08", 0.0f, 0.0f, PARTICLEPOS_Z );

	for (int i=0 ; i<8 ; i++)
	{
		if (pBackPar[i])
		{
			g_ParticleManager->SetLayer(pBackPar[i],GC_LAYER_UI);
			pBackPar[i]->SetShow(true);
			m_vecBackEffect.push_back(pBackPar[i]);

		}
	}

	CParticleEventSeqPTR pAlram = NULL;

	D3DXVECTOR2 vPos = m_pkProgressBarRect->GetCurrentWindowPos();
	vPos.x += m_pkProgressBarRect->GetWidth()/2.0f;
	vPos.y += m_pkProgressBarRect->GetHeight()/2.0f;

	pAlram = g_ParticleManager->CreateSequence("UI_Lita_Alarm", PARTICLEPOS_X(vPos.x) , PARTICLEPOS_Y(vPos.y), PARTICLEPOS_Z );

	if (pAlram)
	{
		g_ParticleManager->SetLayer(pAlram,GC_LAYER_UI);
		pAlram->SetShow(false);
		m_vecAlramEffect.push_back(pAlram);
	}


	vPos = m_pkGoalBasket->GetCurrentWindowPos();
	vPos.x += m_pkGoalBasket->GetWidth()/3.0f;
	vPos.y += m_pkGoalBasket->GetHeight();
	//CParticleEventSeqPTR pSpaceBar1 = g_ParticleManager->CreateSequence("Cursor_Space_01", PARTICLEPOS_X(vPos.x) , PARTICLEPOS_Y(vPos.y), PARTICLEPOS_Z );
	CParticleEventSeqPTR pSpaceBar2 = g_ParticleManager->CreateSequence("Cursor_Space_02", PARTICLEPOS_X(vPos.x) , PARTICLEPOS_Y(vPos.y), PARTICLEPOS_Z );
	CParticleEventSeqPTR pSpaceBar3 = g_ParticleManager->CreateSequence("Cursor_Space_03", PARTICLEPOS_X(vPos.x) , PARTICLEPOS_Y(vPos.y), PARTICLEPOS_Z );
// 	if ( pSpaceBar1 )
// 	{
// 		g_ParticleManager->SetLayer(pSpaceBar1,GC_LAYER_UI);
// 		pSpaceBar1->SetShow(false);
// 		m_vecSpaceEffect.push_back(pSpaceBar1);
// 	}
	if (pSpaceBar2)
	{
		g_ParticleManager->SetLayer(pSpaceBar2,GC_LAYER_UI);
		pSpaceBar2->SetShow(false);
		m_vecSpaceEffect.push_back(pSpaceBar2);
	}
	if (pSpaceBar3)
	{
		g_ParticleManager->SetLayer(pSpaceBar3,GC_LAYER_UI);
		pSpaceBar3->SetShow(false);
		m_vecSpaceEffect.push_back(pSpaceBar3);
	}


}

void KGCRitasPanel::SetShowFeverParticle( bool bShow_ )
{
	std::vector< CParticleEventSeqPTR >::iterator vit =  m_vecFeverEffect.begin();
	while( vit != m_vecFeverEffect.end() )
	{
		if ((*vit))
		{
			(*vit)->SetShow(bShow_);
		}
		++vit;
	}

}

void KGCRitasPanel::SetShowBackParticle( bool bShow_ )
{
	std::vector< CParticleEventSeqPTR >::iterator vit =  m_vecBackEffect.begin();
	while( vit != m_vecBackEffect.end() )
	{
		if ((*vit))
		{
			(*vit)->SetShow(bShow_);
		}
		++vit;
	}
}

void KGCRitasPanel::GameOverCheck()
{
	if( m_iGotObj >= m_kCurrenStageInfo.m_nGoalCount )
		return;

	if (m_fLeftTime <= 10.0f && m_fLeftTime > 0.0f)
	{
		SetShowAlramParticle(true);
	}

	if (m_fLeftTime <= 0.0f)
	{
		m_bClear = false;

		g_KDSound.Play( "Ritas_GameOver" );

		KEVENT_RITAS_CHRISTMAS_STAGE_END_REQ kPacket;
		kPacket.m_nEndType = KEVENT_RITAS_CHRISTMAS_STAGE_END_REQ::ET_GAME_OVER;
		kPacket.m_nGetFeverObjectCount = m_iGotFeverObj;
		kPacket.m_nStageNum = m_kCurrenStageInfo.m_nStageNum;
		kPacket.m_fRemainTime = 0.0f;
		Result_ReceiveChristmasStageClear = INT_MAX;
		KP2P::GetInstance()->Send_RitasChristmas_StageEnd_Req(kPacket);
		g_MyD3D->WaitForServerAck(Result_ReceiveChristmasStageClear , INT_MAX , 3000 , TIME_OUT_VALUE);

		//Gameover Effect
// 		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Lita_Game_Over_01", 0.0f, 0.0f, 0.5f ), GC_LAYER_UI);
// 		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Lita_Game_Over_02", 0.0f, 0.0f, 0.5f ), GC_LAYER_UI);
		m_pkImgGameover->ToggleRender(true);

		m_eNowState = END_STAGE;

		m_dwEndStageDelayTime = ::GetTickCount();
	}

}

void KGCRitasPanel::PlayFrameMove()
{
	UpdatePlayer();
	UpdateRitasObj();
	GameOverCheck();
}

bool KGCRitasPanel::OnStageStart()
{
	KEVENT_RITAS_CHRISTMAS_STAGE_START_REQ kPacket;
	kPacket.m_nStartType = m_iStartType;
	std::map< int, KRitasChristmasStageInfo > mapStageInfo = SiGCRitasChrismasManager()->GetStageContainer();
	std::map< int, KRitasChristmasStageInfo >::iterator mit = mapStageInfo.begin();
	for (; mit != mapStageInfo.end() ; ++mit)
	{
		if ( !IsCleared(mit->second.m_nStageNum) )
		{
			kPacket.m_nStageNum = mit->second.m_nStageNum;
			break;
		}
	}
	//만약 모든 스테이지를 다 클리어 했다면어쩌지... 일단 이 루틴에는 절대 들어오면 안된다.
	if (mit == mapStageInfo.end())
	{
		return false;
	}

	Result_ReceiveChristmasStageStart = INT_MAX;
	KP2P::GetInstance()->Send_RitasChristmas_StageStart_Req(kPacket);
	g_MyD3D->WaitForServerAck(Result_ReceiveChristmasStageStart,INT_MAX,3000,TIME_OUT_VALUE);


	if (Result_ReceiveChristmasStageStart == 0)
	{
		SetStageInfo(kPacket.m_nStageNum);

		SetTickCntForCount(::GetTickCount());

		m_eNowState = COUNT;

		return true;
	}

	return false;

}

void KGCRitasPanel::ClearBasketEffect()
{

	std::vector<int>::iterator vit = m_vecBasket.begin();
	int i=0;
	while( vit != m_vecBasket.end() && i<MAX_OBJICON_NUM)
	{
		D3DXVECTOR2 vPos = m_pkObjIcon[i]->GetCurrentWindowPos();
		vPos.x += m_pkObjIcon[i]->GetWidth()/2.0f;
		vPos.y += m_pkObjIcon[i]->GetHeight()/2.0f;

		g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Lita_Tab", PARTICLEPOS_X(vPos.x) , PARTICLEPOS_Y(vPos.y), PARTICLEPOS_Z ), GC_LAYER_UI);

		i++;
		++vit;
	}

}

void KGCRitasPanel::SetShowAlramParticle( bool bShow_ )
{
	std::vector< CParticleEventSeqPTR >::iterator vit =  m_vecAlramEffect.begin();
	while( vit != m_vecAlramEffect.end() )
	{
		if ((*vit))
		{
			(*vit)->SetShow(bShow_);
		}
		++vit;
	}
}

void KGCRitasPanel::AllSetParticle(bool bShow_)
{
	SetShowFeverParticle(bShow_);
	SetShowBackParticle(bShow_);
	SetShowAlramParticle(bShow_);
	SetShowSpaceParticle(bShow_);

}

void KGCRitasPanel::SetShowSpaceParticle( bool bShow_ )
{
	std::vector< CParticleEventSeqPTR >::iterator vit =  m_vecSpaceEffect.begin();
	while( vit != m_vecSpaceEffect.end() )
	{
		if ((*vit))
		{
			(*vit)->SetShow(bShow_);
		}
		++vit;
	}
}

void KGCRitasPanel::UpDateFeverObjAndBoxNum()
{

	char szName[MAX_PATH];
	sprintf( szName, "x %d", m_iGotFeverObj );

	m_pkStaticFeverObjNum->SetText( GCUTIL_STR::GCStrCharToWide(szName) );

	sprintf( szName, "x %d", m_iGotFeverObj/10 );

	m_pkStaticGotBoxInStage->SetText( GCUTIL_STR::GCStrCharToWide(szName) );

}







