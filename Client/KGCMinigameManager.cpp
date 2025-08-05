#include "stdafx.h"
#include ".\kgcminigamemanager.h"
#include "MyD3D.h"

//
#include "KGCRoomManager.h"
#include "GCStateMachine.h"



#include "GCCameraOrtho.h"
#include "GCStateGame.h"

#include "MapSystem/KGCLayerManager.h"

//
//
//

#include "GCUtil/GCRandomObj.h"
#include "Headup Display.h"
#include "gcui/GCGameOverlayUI.h"
//

#include "KGCGameBoard.h"
#include "gcui/KGCMiniGameScoreBoard.h"
#include "gcui/GCUIScene.h"
#include "gcui/GCOlympicEventDlg.h"
#include "gcui/KGCWorldMapContainer.h"
#include "gcui/GCEventMinigameList.h"


ImplementSingleton(KGCMinigameManager);

KGCMinigameManager::KGCMinigameManager(void)
: m_bPushCheck( true )
, m_bContact( false )
, m_bTutorialEnd( false )
, m_fCharPosX( 1.12f )
, m_fCharPosY( 1.12f )
, m_eMiniGameMode( GC_GM_INVALID )
, m_bExitGame( false )
, m_iLevel( 0 )
, m_ePrevState( GS_SERVER )
, m_iScore( 0 )
, m_iAccumulatedScore( 0 )
{
    m_mapRankInfo.clear();
    m_vecMyRankInfo.clear();
}

KGCMinigameManager::~KGCMinigameManager(void)
{
    m_mapRankInfo.clear();
    m_vecMyRankInfo.clear();
}

bool KGCMinigameManager::StartMiniGame()
{
	//게임 시작하라고 하면.. 해상도 부터 키운다.
	if ( g_MyD3D->m_KGCOption.GetBasicWindowScale() != g_MyD3D->m_KGCOption.GetWindowScale() )
	{
		g_MyD3D->SetResolution(TRUE);
	}

	if( m_eMiniGameMode == GC_GM_INVALID )
	{
		ASSERT( !"You must set minigamemode before call start function!!" );
		return false;
	}


	//미니게임에 대한 속성을 읽어오기
	if( LoadMinigameAttribute( m_eMiniGameMode ) == false )
		return false;

	InitValue();
	m_bTutorialEnd = false;
	g_MyD3D->SetEnableKeyInput( false );
	g_pGameBoard->InitPlayTime();

	// 방정보 셋팅
	InitRoomInfo();

	// 케릭터 셋팅
	InitPlayer();

	GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), m_strScript );

    // 로딩화면으로!!
	g_MyD3D->m_pStateMachine->GoLoading();

	return true;
}

void KGCMinigameManager::SetPlayerPosition()
{
	// milennium9 test code
	// 일단 화면 가운데 나타납시다.

	g_MyD3D->MyPlayer[0]->vPos.x = m_fCharPosX;
	g_MyD3D->MyPlayer[0]->vPos.y = m_fCharPosY;

	// end // milennium9 test code
}

bool KGCMinigameManager::LoadStage()
{
	// 2008.6.23 milennium9
	// 미니게임에 사용되는 맵이 하나니까 코드에 둡니다.
	// 나중에 스크립트로 빼세요. 일정땜시 스크립트로 컨트롤하기 힘듦.
	
	//초기화
	SiKGCFantasticMap()->ClearWater();
	g_pStateGame->ClearStageResource();

	// 스크립트 로드
	//SiKGCFantasticMap()->UnLoad();
	if( SiKGCLayerManager()->LoadScript( m_strMap ) == false )
		return false;
	
	// 카메라 셋팅
	InitCamara(); 

	return true;
}

bool KGCMinigameManager::InitCamara()
{
	g_kCamera.SetTargetMonster(-1);                         //혹시나 몬스터를 가리키고 있었다면 취소한다    
	g_kCamera.SetTargetPlayer( g_MyD3D->Get_MyPlayer() );   // Drama에서 다른 플레이어를 가리키고 있다면 나를 가리키도록 수정함. 
	g_kCamera.InitCamera();
	g_kCamera.m_fWideMode = 7.51f;	//미니게임은 언제나 맵 전체가 보이도록 해 줍니다.

	g_kCamera.InitCameraPosition();

	return true;
}

bool KGCMinigameManager::SetMiniGameModeID( EGCGameMode eGameMode )
{
	if( eGameMode != GC_GM_MINIGAME_TREEDROP &&
		eGameMode != GC_GM_MINIGAME_BUTTERFRY &&
		eGameMode != GC_GM_MINIGAME_BALLOON )
	{
		ASSERT( !"This is not minigame modeID!!!" );
		return false;
	}

	m_eMiniGameMode = eGameMode;

	return true;
}

bool KGCMinigameManager::EndMiniGame()
{

    g_MyD3D->MyPlayer[0]->m_setSkill = m_vecTempSkill;

	//m_eMiniGameMode = GC_GM_INVALID;
	g_MyD3D->MyPlayer[0]->ClearPlayerData();
	g_MyD3D->MyPlayer[0]->EndGame();
    g_MyD3D->SetEnableKeyInput( true );
	ClearObject();
	GCFUNC::CallLuaFunction( KGCLuabinder::getInstance(), "MiniGame_End()" );

	return true;
}

void KGCMinigameManager::InitPlayer()
{
	//////////////////////////////////////////////////////////////////////////
	// 모션과 모델을 모두 지운다. 아니면 메모리가 캐릭터 수만큼(모델 및 모션만큼) 늘어난다.
	// 메모리가 허용범위가 높다면 궂이 안해도 된다.
	g_pGraphicsHelper->UnLoadPlayerActionMotion();
	g_pGraphicsHelper->UnLoadPlayerModel();
	//////////////////////////////////////////////////////////////////////////

	// 기본복장을 입히자.
	for( int i = 0 ; i < (int)g_MyD3D->MyPlayer[0]->m_kUserInfo.vecCharInfo.size() ; ++i )
	{
		g_pItemMgr->EquipBaseItem( &g_MyD3D->MyPlayer[0]->m_kUserInfo.vecCharInfo[i] );                                
	}

	g_MyD3D->MyPlayer[0]->PresetTutorialChars( "MinigameEquip.lua" );
	g_MyD3D->MyPlayer[0]->InitTutorialChar( m_iMiniGameChar );

	g_MyD3D->MyPlayer[0]->SetPlayerName(g_kGlobalValue.m_kUserInfo.strNickName.c_str(), g_kGlobalValue.m_kUserInfo.strNickColor);
	g_MyD3D->MyPlayer[0]->m_cLife = 1;
	g_MyD3D->MyPlayer[0]->m_kUserInfo.iTeam = 0;
	g_MyD3D->MyPlayer[0]->Item_Remain_Time[GC_GAME_ITEM_SUPER] = 100;
	g_MyD3D->MyPlayer[0]->m_pObject->SetOutlineColor( EDGE_COLOR_FRIEND );

	for(int i=0;i<FORM_NUM;++i)
	{
		if( g_MyD3D->MyPlayer[0]->m_pObjMetaForm[i] )
			g_MyD3D->MyPlayer[0]->m_pObjMetaForm[i]->SetOutlineColor( EDGE_COLOR_FRIEND );
	}

	//	스킬을 백업해두고 지운다.
	m_vecTempSkill = g_MyD3D->MyPlayer[0]->m_setSkill;
	g_MyD3D->MyPlayer[0]->m_setSkill.clear();

    

}

void KGCMinigameManager::InitRoomInfo()
{
	SiKGCRoomManager()->InitGameMode();
	SiKGCRoomManager()->SetGameModeCategory( GC_GMC_MINIGAME );
	SiKGCRoomManager()->SetGameMode( m_eMiniGameMode );
	SiKGCRoomManager()->SetGameModeStage( (EGCGameStage)-1 );
	SiKGCRoomManager()->SetSubGameMode( -1 );
}

void KGCMinigameManager::Render()
{
	std::list< KGC2DObject* >::iterator lit;
	for( lit = m_listObjects.begin() ; lit != m_listObjects.end() ; ++lit )
	{
		(*lit)->Render();
	}
}

void KGCMinigameManager::FrameMove()
{
	// 부스트 힐 같은 기술 쓰면 피가 찹니다.
	// 그래서 매 프레임 false
	g_MyD3D->MyPlayer[0]->m_bIncHP = false;

	if( g_MyD3D->EnableKeyInput() == false )
		g_MyD3D->SetEnableKeyInput( m_bTutorialEnd );

	std::list< KGC2DObject* >::iterator lit;
	for( lit = m_listObjects.begin() ; lit != m_listObjects.end() ; ++lit )
	{
		(*lit)->FrameMove_Origin();
	}
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
	lua_checkstack( L, 10 );
	lua_pushstring( L, "MiniGame_FrameMove" );
	lua_rawget( L, LUA_GLOBALSINDEX );
	if( lua_type( L, -1 ) == LUA_TFUNCTION )
	{
		lua_pcall( L, 0, 0, 0 );
	}
	else
	{
		lua_pop( L, 1 );
	}
}

bool KGCMinigameManager::BindLua()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KGCMinigameManager>( L, "KGCMinigameManagerClass" );
    lua_tinker::class_mem<KGCMinigameManager>( L, "ReadyTimeEnd", 	&KGCMinigameManager::m_bTutorialEnd  );
    lua_tinker::class_mem<KGCMinigameManager>( L, "ContactCheck", 	&KGCMinigameManager::m_bContact  );
    lua_tinker::class_mem<KGCMinigameManager>( L, "Level", 			&KGCMinigameManager::m_iLevel  );
    lua_tinker::class_def<KGCMinigameManager>( L, "CreateObject", 			&KGCMinigameManager::CreateObject  );
    lua_tinker::class_def<KGCMinigameManager>( L, "RemoveObject", 			&KGCMinigameManager::RemoveObject  );
    lua_tinker::class_def<KGCMinigameManager>( L, "GetMapWidth", 				&KGCMinigameManager::GetMapWidth  );
    lua_tinker::class_def<KGCMinigameManager>( L, "GetRandomValue", 			&KGCMinigameManager::GetRandomValue  );
    lua_tinker::class_def<KGCMinigameManager>( L, "ClearObject", 				&KGCMinigameManager::ClearObject  );
    lua_tinker::class_def<KGCMinigameManager>( L, "ShowText", 				&KGCMinigameManager::ShowText  );
    lua_tinker::class_def<KGCMinigameManager>( L, "AddScore", 				&KGCMinigameManager::AddScore  );
    lua_tinker::class_def<KGCMinigameManager>( L, "ParticleEffect", 			&KGCMinigameManager::ParticleEffect  );
    lua_tinker::class_def<KGCMinigameManager>( L, "RestartMinigame", 			&KGCMinigameManager::RestartMinigame  );;

	lua_tinker::decl( L, "KGCMinigameManager", this );

	return true;
}

KGC2DObject* KGCMinigameManager::CreateObject()
{
	KGC2DObject* pObj = new KGC2DObject();
	m_listObjects.push_back( pObj );
	return pObj;
}

void KGCMinigameManager::InitLua()
{
#if defined( __PATH__ )
	GCFUNC::CallLuaFunction( KGCLuabinder::getInstance(), "MiniGame_Initialize()" );
#else
	GCFUNC::CallLuaFunction( KGCLuabinder::getInstance(), "state, msg = pcall( MiniGame_Initialize ); if state == false then print( msg ) end" );
#endif
}

std::pair< bool, GCCollisionRect<float> > KGCMinigameManager::HitObject( GCCollisionRect<float>* rtWeapon, bool bOneHit, int iDamageType )
{
	bool bCrash = false;
	GCCollisionRect<float> rtObjectRect;
	std::list< KGC2DObject* >::iterator lit;

	for( lit = m_listObjects.begin() ; lit != m_listObjects.end() ; ++lit )
	{
		if( (*lit)->Damaged( *rtWeapon, 0.0f, iDamageType, -1, false ) )
		{
			bCrash = true;
			rtObjectRect = (*lit)->GetCollisionRect();
		}

		if( bCrash == true && bOneHit == true )
			break;
	}

	return std::make_pair( bCrash, rtObjectRect );
}

void KGCMinigameManager::RemoveObject( KGC2DObject* obj )
{
	std::list< KGC2DObject* >::iterator lit = std::find( m_listObjects.begin(), m_listObjects.end(), obj );

	if( lit == m_listObjects.end() )
		return;

	SAFE_DELETE( *lit );
	m_listObjects.erase( lit );
}

void KGCMinigameManager::ParticleEffect( const char* strEffectName, float fx, float fy )
{
	g_ParticleManager->CreateSequence( strEffectName, fx - 1.0f, fy - 0.5f, 0.5f );
}

void KGCMinigameManager::PushCheck( bool bTeleport )
{
	if( m_bPushCheck == false )
		return;

	std::list< KGC2DObject* >::iterator lit;

	for( lit = m_listObjects.begin() ; lit != m_listObjects.end() ; ++lit )
	{
		(*lit)->PushCheck( g_MyD3D->MyPlayer[0]->vPos.x, g_MyD3D->MyPlayer[0]->vPos.y + g_MyD3D->MyPlayer[0]->y_Ex, bTeleport );
	}
}

float KGCMinigameManager::GetMapWidth()
{
	return SiKGCFantasticMap()->GetWorldWidth();
}

float KGCMinigameManager::GetRandomValue( float begin, float end )
{
	GCUTIL::GCRand_float kRand;
	kRand.SetRange( begin, end );

	return kRand.rand();
}

void KGCMinigameManager::ClearObject()
{
	std::list< KGC2DObject* >::iterator lit;

	for( lit = m_listObjects.begin() ; lit != m_listObjects.end() ; ++lit )
	{
		SAFE_DELETE( *lit );
	}
	m_listObjects.clear();
}

void KGCMinigameManager::ShowText( const char* str )
{
	g_MyD3D->MyHead->Start_Text( str );
}

bool KGCMinigameManager::LoadMinigameAttribute( EGCGameMode eGameMode )
{
	KLuaManager luaMgr;

	if( GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) == false )
		return false;

	if( GCFUNC::LoadLuaScript( luaMgr, "MinigameData.lua" ) == false )
		return false;

	LUA_BEGIN_TABLE("MiniGameData", return false)
	{
		LUA_BEGIN_TABLE( eGameMode, return false )
		{
			LUA_GET_VALUE( "Script", m_strScript, return false );
			LUA_GET_VALUE( "Map", m_strMap, return false );
			LUA_GET_VALUE( "Char", m_iMiniGameChar, return false );
			LUA_GET_VALUE_DEF( "Char_Pos_X", m_fCharPosX, 1.3f );
			LUA_GET_VALUE_DEF( "Char_Pos_Y", m_fCharPosY, 1.3f );
		}
		LUA_END_TABLE( return false );
	}
	LUA_END_TABLE(return false)

	return true;
}

bool KGCMinigameManager::CheckContact( KGCContectInfo& kInfo )
{
	if( false == m_bContact )
		return false;

	std::list< KGC2DObject* >::iterator lit;

	for( lit = m_listObjects.begin() ; lit != m_listObjects.end() ; ++lit )
	{
		if( kInfo.m_ySpeed > 0.0f )
			break;

		//g_MyD3D->MyPlayer[0]->vPos.x, g_MyD3D->MyPlayer[0]->vPos.y + g_MyD3D->MyPlayer[0]->y_Ex
		if( (*lit)->GetPos().x < g_MyD3D->MyPlayer[0]->vPos.x &&
			(*lit)->GetPos().x + (*lit)->GetWidth() > g_MyD3D->MyPlayer[0]->vPos.x )
		{
			if( (*lit)->GetPos().y + (*lit)->GetHeight() > g_MyD3D->MyPlayer[0]->vPos.y + g_MyD3D->MyPlayer[0]->y_Ex &&
				(*lit)->GetPos().y + (*lit)->GetHeight() * 0.9f < g_MyD3D->MyPlayer[0]->vPos.y + g_MyD3D->MyPlayer[0]->y_Ex)
			{
				kInfo.m_ySpeed = (*lit)->GetSpeed().y;
				kInfo.m_y = (*lit)->GetPos().y + (*lit)->GetHeight() * 0.95f - 0.1f;
				return true;
			}
		}
	}

	return false;
}

bool KGCMinigameManager::RestartMinigame()
{
	InitValue();
	g_pGameBoard->InitPlayTime();
    g_pkGameOverlayUI->GetMinigameScoreBoard()->SetHighScore( GetMyHighScore( GetMinigameModeID() - GC_GM_MINIGAME_TREEDROP ) );
	InitPlayer();
	SetPlayerPosition();
	return GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), m_strScript );
}

int KGCMinigameManager::GetCurrentScore()
{
	return m_iScore + m_iAccumulatedScore;
}

void KGCMinigameManager::SetRankingInfo( int iMode, std::vector<KMiniGameRankInfo>& vecRankinfo )
{
    m_mapRankInfo[iMode] = vecRankinfo;
}

void KGCMinigameManager::GetRankingInfo( int iMode, std::vector<KMiniGameRankInfo>& vecRankInfo )
{
	vecRankInfo.clear();
    std::map< int, std::vector<KMiniGameRankInfo> >::iterator mit;
    mit = m_mapRankInfo.find( iMode );
	if( mit != m_mapRankInfo.end() ) vecRankInfo = mit->second;		
}

int KGCMinigameManager::GetMyHighScore( int iMode )
{
    std::vector<KMiniGameRankInfo>::iterator vit = m_vecMyRankInfo.begin();

    for ( ; vit != m_vecMyRankInfo.end(); ++vit )
    {
        if ( vit->m_cGameMode == iMode )
            return vit->m_dwBestScore;
    }

    return -1;
}

int KGCMinigameManager::GetMyRanking( int iMode )
{
    std::vector<KMiniGameRankInfo>::iterator vit = m_vecMyRankInfo.begin();

    for ( ; vit != m_vecMyRankInfo.end(); ++vit )
    {
        if ( vit->m_cGameMode == iMode )
            return vit->m_nRank;
    }

    return -1;
}

void KGCMinigameManager::SetMyHighScore( int iMode, DWORD dwScore )
{
    std::vector<KMiniGameRankInfo>::iterator vit = m_vecMyRankInfo.begin();

    for ( ; vit != m_vecMyRankInfo.end(); ++vit )
    {
        if ( vit->m_cGameMode == iMode )
            vit->m_dwBestScore = dwScore;
    }
}

void KGCMinigameManager::SetMyRanking( int iMode, DWORD dwRank )
{
    std::vector<KMiniGameRankInfo>::iterator vit = m_vecMyRankInfo.begin();

    for ( ; vit != m_vecMyRankInfo.end(); ++vit )
    {
        if ( vit->m_cGameMode == iMode )
            vit->m_nRank = dwRank;
    }
}

int KGCMinigameManager::GetCurrentLevel()
{
	return m_iLevel;
}

void KGCMinigameManager::InitValue()
{
	m_iScore = 0;
	m_iLevel = 0;
}

std::wstring KGCMinigameManager::GetRankerName( int iMode, int iRank )
{
    std::map< int, std::vector<KMiniGameRankInfo> >::iterator mit;
    mit = m_mapRankInfo.find( iMode );

    if ( mit == m_mapRankInfo.end() )
        return L"";

    std::vector<KMiniGameRankInfo>::iterator vit = mit->second.begin();
    for ( ; vit != mit->second.end(); ++vit )
    {
        if ( vit->m_nRank == iRank )
            return vit->m_strNick;
    }

    return L"";
}

bool KGCMinigameManager::IsInRanker( std::wstring strNick )
{
    std::map< int, std::vector<KMiniGameRankInfo> >::iterator mit;
    std::vector<KMiniGameRankInfo>::iterator vit;
    for ( mit = m_mapRankInfo.begin(); mit != m_mapRankInfo.end(); ++mit )
    {
        for ( vit = mit->second.begin(); vit != mit->second.end(); ++vit )
        {
            if ( vit->m_strNick == strNick )
                return true;
        }
    }

    return false;
}

void KGCMinigameManager::SetScore( int iScore )
{
	if( iScore >= 1000 )
	{
		m_iScore = 1000;
		return;
	}

	m_iScore = iScore;
}

// 미니게임을 하기 전 스테이트로
void KGCMinigameManager::GoPrevState()
{
    switch ( m_ePrevState )
    {
    case GS_GAME :
    case GS_WORLDMAP :
        g_MyD3D->m_pStateMachine->GoWorldMap();
        break;
    case GS_MY_INFO :
        //g_MyD3D->m_pStateMachine->GoMyInfo();
        //break;
    case GS_SHOP_CASH :
    case GS_SHOP_GP :
        //g_MyD3D->m_pStateMachine->GoShop();
        //break;
#if defined USE_COORDI_SHOP
    case GS_COORDI_SHOP :
#endif
        //g_MyD3D->m_pStateMachine->GoCoordiShop();
        //break;
    case GS_MATCH :
        g_MyD3D->OnMatchButton();
        break;
    default:
        g_MyD3D->m_KGCOption.SetLastState( (BYTE)(g_MyD3D->m_pStateMachine->GetState()) );
        g_MyD3D->m_KGCOption.SetLastWorldMap((BYTE)SiKGCWorldMapManager()->GetCurrentWorldMap());
        g_MyD3D->m_pStateMachine->GoServer();
    }
}

void KGCMinigameManager::SetPrevState()
{
    m_ePrevState = g_MyD3D->m_pStateMachine->GetState();
}

void KGCMinigameManager::SendResult()
{
    if( m_eMiniGameMode == GC_GM_MINIGAME_TREEDROP )
        SiGCOlympicManager()->SendOlympicTreeDropResult( m_iScore );
    else if( m_eMiniGameMode == GC_GM_MINIGAME_BALLOON )
        SiGCOlympicManager()->SendOlympicBalloonResult( m_iScore );
}

void KGCMinigameManager::StartGame( EGCGameMode eGameMode )
{
    SiKGCMinigameManager()->SetPrevState();
    SiKGCChannelManager()->SetCurrentChannelType( CT_SQUARE );
    SiKGCRoomManager()->UpdateShowGameModeCategory( false, SiKGCChannelManager()->GetCurrentChannelType() );
    SiKGCRoomManager()->InitGameMode();
    SetUserInfo( g_kGlobalValue.m_kUserInfo );

    // 불필요한 UI들을 꺼주자
    g_pkUIScene->m_pkWorldMapContainer->ToggleRender( false );
    //g_pkUIScene->m_pkOlympicDlg->Onclose();
    //g_pkUIScene->m_pkEventMinigameList->OnEndBtn();

    SiKGCMinigameManager()->SetMiniGameModeID( eGameMode );
    SiKGCMinigameManager()->StartMiniGame();
    SiKGCMinigameManager()->SetExitGame( false );
}

void KGCMinigameManager::QuitCurrentGame()
{
    SiKGCMinigameManager()->SetExitGame( true );
}

void KGCMinigameManager::SetAccumulatedScore( IN const int nScore )
{
    m_iAccumulatedScore = nScore;
}