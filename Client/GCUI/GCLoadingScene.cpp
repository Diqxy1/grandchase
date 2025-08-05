#include "stdafx.h"
#include "GCLoadingScene.h"
#include "MyD3D.h"

#include "GCUI/GCGameOverlayUI.h"
#include "GCUI/KGCCharLoadWnd.h"
#include "GCEventManager.h"
#include "KGCRoomManager.h"
#include "GCRenderManager.h"
//
#include "GCStateLoading.h"
#include "GCStateMachine.h"
#include "QuestGameManager.h"
#include "GCUI/GCAutoMatchingS6.h"

IMPLEMENT_CLASSNAME( KGCLoadingScene );
IMPLEMENT_WND_FACTORY( KGCLoadingScene );
IMPLEMENT_WND_FACTORY_NAME( KGCLoadingScene, "gc_loading_scene" );

KGCLoadingScene::KGCLoadingScene(void)
{
	m_pMatchLoad = NULL;
	m_pWorldCupLoad = NULL;
	m_pMinigameLoad = NULL;
	m_pNowLoading = NULL;
	m_pWaiting = NULL;
    m_pVS = NULL;
    m_pAgitLoad = NULL;
    m_pTip = NULL;
    m_pTipTitle= NULL;
    m_pTipContents= NULL;

	LINK_CONTROL( "worldcup_loading",	m_pWorldCupLoad );
	LINK_CONTROL( "match_loading",		m_pMatchLoad );
	LINK_CONTROL( "indigo_loading",		m_pIndigoLoad );
	LINK_CONTROL( "minigame_loading",	m_pMinigameLoad );
    LINK_CONTROL( "agit_loading",       m_pAgitLoad );
    
	char strName[MAX_PATH] = {0,};
    for ( int i = 0; i < MAX_LOADING_CHAR; i++ )
    {
        sprintf( strName, "char_wnd%d", i );
        LINK_CONTROL( strName,		    m_apkCharLoadingWnd[i] );
    }
	LINK_CONTROL( "now_loading",	m_pNowLoading );
	LINK_CONTROL( "waiting",		m_pWaiting );

    LINK_CONTROL( "VS",		        m_pVS);

    LINK_CONTROL( "tip",		        m_pTip);
    LINK_CONTROL( "tip_title",		    m_pTipTitle);
    LINK_CONTROL( "tip_contents",		m_pTipContents);

    memset( m_pMotionIntro, NULL, (sizeof(GCDeviceMotionFRM*) * GC_CHAR_NUM * NUM_JOB_LEVEL) );
}

KGCLoadingScene::~KGCLoadingScene(void)
{
}

void KGCLoadingScene::SetGameMode()
{
	EGCGameModeCategory eCurrCategory;
	eCurrCategory = SiKGCRoomManager()->GetGameModeCategory();

	m_pMatchLoad->ToggleRender( false );
	m_pWorldCupLoad->ToggleRender( false );
	m_pIndigoLoad->ToggleRender( false );
	m_pMinigameLoad->ToggleRender( false );
    if (m_pAgitLoad)
        m_pAgitLoad->ToggleRender( false );
    
    m_vecBillBoard.clear();

    if( m_pTipTitle)
    {
        m_pTipTitle->SetText(g_pkStrLoader->GetString(STR_ID_MATCH_LOADINGSCENE_TIP_TITLE));
        m_pTipTitle->SetFontColor(D3DCOLOR_ARGB(255,255,255,0));
    }
    if( m_pTipContents )
        m_pTipContents->SetText(g_pkStrLoader->GetString(STR_ID_MATCH_LOADINGSCENE_TIP_CONTENTS));

    if ( SiKGCRoomManager()->GetGameModeType() == GC_GMT_UNION_FIGHT && !g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) )
    {
        m_pVS->ToggleRender( false );
        
        if( m_pTip )
        m_pTip->ToggleRender( false );
        
        if( m_pTipTitle )
        m_pTipTitle->ToggleRender( false );
        
        if( m_pTipContents )
        m_pTipContents->ToggleRender( false );
    }
    else
    {
        m_pVS->ToggleRender( true );
        if( m_pTip )
            m_pTip->ToggleRender( true );

        if( m_pTipTitle )
            m_pTipTitle->ToggleRender( true );

        if( m_pTipContents )
            m_pTipContents->ToggleRender( true );
    }
    

	switch( eCurrCategory )
	{
	case GC_GMC_DUNGEON:
        // 제일 첫 스테이지 들어갈 때이다!! iCurStage를 0으로 주겠따!
		g_pkGameOverlayUI->ShowQuestTitle( SiKGCRoomManager()->GetGameMode(), 0, true );
		break;
	case GC_GMC_AUTOMATCH:
		OnAutoMatch();
		break;
    case GC_GMC_AGIT:
        OnAgit();
        break;
	default:
        ClearUID();
		if( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) )
        {
            OnIndigo();
        }
		else if( SiKGCEventManager()->IsActiveEvent( EVENT_ID_WORLDCUP ) == true )
        {
            OnMatch();
            m_pMatchLoad->ToggleRender( false );
			m_pWorldCupLoad->ToggleRender( true );
        }
		else if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MINIGAME )
		{
			OnMiniGame();
		}
		else if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_ANGELS_EGG )
		{
			OnMatch();
		}
		else
        {
            OnMatch();
        }
		break;
	}
}

void KGCLoadingScene::SetLoadingState( std::pair<DWORD,int> prLoadingState_ )
{
    for( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if( m_apkCharLoadingWnd[i]->GetUID() == prLoadingState_.first )
        {
            m_apkCharLoadingWnd[i]->SetLoadingState(prLoadingState_);
        }        
    }
}

void KGCLoadingScene::OnMatch()
{
    LoadAdTexture();

	m_pNowLoading->ToggleRender( true );
	m_pWaiting->ToggleRender( false );

    for ( int i = 0; i < MAX_LOADING_CHAR; i++ )
    {
        m_apkCharLoadingWnd[i]->ToggleRender( false );
    }

    int iCount          = 0;
    int iLeftCount      = 0;
    int iRightCount     = 0;

    for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if( ! ( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_READY ) || 
            g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) ||
            g_MyD3D->MyPlayer[i]->m_kUserInfo.bHost ||
            g_MyD3D->MyPlayer[i]->IsLocalPlayer() )  ) 
            continue;
        
        KGCStateLoading* pState = (KGCStateLoading*)g_MyD3D->m_pStateMachine->GetStatePtr( GS_GAME_LOADING );
        if( pState->IsStartingMember( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID ) == false )
        {
            continue;
        }

        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_NONE ) || !g_MyD3D->MyPlayer[i]->m_pObject )
        {
            continue;
        }

        int iRenderSlot = 0;
        int iPetFrame = 0;
        int iFrame = 0;

        if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.IsBlueTeam() )
        {
            iRenderSlot = iLeftCount * 2;                
            ++iLeftCount;
        }
        else
        {                
            iRenderSlot = iRightCount * 2 + 1;                
            ++iRightCount;
        }

        if ( GC_GMT_TEAM_FIGHT != SiKGCRoomManager()->GetGameModeType() )
        {
            iRenderSlot = iCount++;
        }

#if defined( LOADING_CHAR_MOTION )
        SCharInfo& sCharInfo = g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar();
        int iCharType = sCharInfo.iCharType;
        int iPromotion = (g_MyD3D->MyPlayer[i]->Extra_Char_Num - sCharInfo.iCharType) / GC_CHAR_NUM;
        iFrame = 0;
        if ( iRenderSlot % 2 == 1 )
        {
            iFrame = iPetFrame = 1;
        }

        // 예외처리 : 모션이 없을땐 다른모션으로 대체
        int iTemp = 0;
        while( NULL == m_pMotionIntro[iCharType][iPromotion] )
        {
            CreateMotion( iCharType, iPromotion );
            
            if ( !m_pMotionIntro[iCharType][iPromotion] ) 
            {
                if( ++iPromotion >= NUM_JOB_LEVEL )
                {
                    iTemp++;
                    iPromotion=0;
                }
                if( iTemp >= GC_CHAR_NUM )
                {   return;     }

                iCharType = iTemp;
            }
        }
#else
        iFrame = g_MyD3D->MyPlayer[i]->Extra_Char_Num;
        if ( iRenderSlot % 2 == 1 )
        {
            iFrame = g_MyD3D->MyPlayer[i]->Extra_Char_Num + GC_CHAR_NUM * NUM_JOB_LEVEL;
            iPetFrame = 1;
        } 
#endif

        m_apkCharLoadingWnd[iRenderSlot]->SetName( g_MyD3D->MyPlayer[i]->GetStrUserName(),
            g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID, SiGCTextColorManager()->StringToColor(g_MyD3D->MyPlayer[i]->GetStrUserColor()));
        m_apkCharLoadingWnd[iRenderSlot]->InitLoadingStateBar();


        D3DXMATRIX matWorld,matTrans,matScale;

        D3DXMatrixTranslation( &matTrans, ( m_apkCharLoadingWnd[iRenderSlot]->GetFixedWindowPos().x + m_apkCharLoadingWnd[iRenderSlot]->GetWidth() /2 - (400 *GC_SCREEN_DIV_WIDTH)  ) * 0.0021f, 
            ( m_apkCharLoadingWnd[iRenderSlot]->GetFixedWindowPos().y + m_apkCharLoadingWnd[iRenderSlot]->GetHeight() /2 - (300*GC_SCREEN_DIV_WIDTH) ) * (-0.00185f), 10.0f + i * 5.0f  );

        D3DXMatrixScaling( &matScale, 0.3f, 0.3f, 0.3f );

        matWorld = matScale * matTrans;

        g_MyD3D->MyPlayer[i]->m_pObject->SetWorldMat( &matWorld );

#if defined( LOADING_CHAR_MOTION )
        g_MyD3D->MyPlayer[i]->m_pObject->SetFrame( &m_pMotionIntro[iCharType][iPromotion]->GetFrameData(iFrame) );
#else
        g_MyD3D->MyPlayer[i]->m_pObject->SetFrame( &m_pMotionIntro->GetFrameData(iFrame) );
#endif

        //펫 배치
        KPetInfo kPet = g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar().kPetInfo;
        int iPetLevel = kPet.m_iLevel;
        int iPetID = g_kGlobalValue.GetPetIDFromItemID( kPet.m_dwID );
        int iPetEvolutionLevel = (int)kPet.m_cPromotion;

        // 장착하고 있는 펫이 있다
        if( kPet.m_dwUID != 0 && iPetID != -1 )
        {
            D3DXMATRIX matPetOffset;
            D3DXMATRIX matPetScale;
            
            SPetTemplate petTemplate =  g_kGlobalValue.GetPetTemplate(iPetID , iPetEvolutionLevel );               

            float fTemp = ( iPetFrame == 0 )?-1.0f:1.0f;
            D3DXMatrixTranslation( &matPetOffset, petTemplate.fDistX * 0.7f * fTemp,
                                                    petTemplate.fDistY * 0.7f , -2.0f );
            D3DXMatrixScaling( &matPetScale, petTemplate.fScale , petTemplate.fScale, petTemplate.fScale );                
            


            matWorld = matPetScale * matTrans * matPetOffset;
            
            m_aMatPetTransform[i] = matWorld;
            m_aiPetFrame[i] = iPetFrame;

        }
        else
        {
            m_aiPetFrame[i] = 0;
            D3DXMatrixScaling( &m_aMatPetTransform[i], 0.0f, 0.0f, 0.0f );
        }
    }
}

void KGCLoadingScene::OnIndigo()
{
	m_pNowLoading->ToggleRender( true );
	m_pWaiting->ToggleRender( false );

    for ( int i = 0; i < MAX_LOADING_CHAR; i++ )
    {
        m_apkCharLoadingWnd[i]->ToggleRender( false );
    }
    m_apkCharLoadingWnd[6]->ToggleRender( true );
    m_apkCharLoadingWnd[6]->SetLoadingComplete( false );
    m_apkCharLoadingWnd[7]->ToggleRender( true );
    m_apkCharLoadingWnd[7]->SetLoadingComplete( false );
    for( int i=0 ; i<MAX_PLAYER_NUM ; ++i )
    {
        if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState == GC_RUS_NONE )
            continue;
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.IsBlueTeam() )
        {
            D3DCOLOR dwColor = D3DCOLOR_ARGB( 255, 0, 0, 255 );
            m_apkCharLoadingWnd[6]->SetName( g_MyD3D->MyPlayer[i]->GetStrUserName(), 
                                             g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID, dwColor );
        }
        else
        {
            D3DCOLOR dwColor = D3DCOLOR_ARGB( 255, 255, 0, 0 );
            m_apkCharLoadingWnd[7]->SetName( g_MyD3D->MyPlayer[i]->GetStrUserName(), 
                                             g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID, dwColor );
        }
    }
    m_pIndigoLoad->ToggleRender( true );
}

void KGCLoadingScene::SetLoadingComplete( DWORD dwUID_ )
{
    for ( int i = 0; i < MAX_LOADING_CHAR; i++ )
    {
        if ( m_apkCharLoadingWnd[i]->GetUID() == dwUID_ )
        {
            m_apkCharLoadingWnd[i]->SetLoadingComplete( true );
            return;
        }
    }

    
}
void KGCLoadingScene::ClearUID()
{
    for( int i = 0; i < MAX_LOADING_CHAR; i++ )
    {
        m_apkCharLoadingWnd[i]->SetUID( 0 );
    }
}

void KGCLoadingScene::OnCreate()
{
	//for( int i = 3 ; i < 6 ; ++i )
	//{
	//	m_apkCharLoadingWnd[i]->SetNickNameAlign( DT_RIGHT );
	//}

	m_pNowLoading->InitState( true );
	m_pWaiting->InitState( false );

    // 전체파일로드
#if defined( LOADING_CHAR_MOTION )

    if ( !GC_GLOBAL_DEFINE::bRealTimeLoadMotion ) 
    {
        char szTemp[128];
        memset( m_pMotionIntro, 0, (sizeof(GCDeviceMotionFRM*) * GC_CHAR_NUM * NUM_JOB_LEVEL) );
        for(int iCharLoop=0; iCharLoop<GC_CHAR_NUM; ++iCharLoop)
        {
            for(int iJobLoop=0; iJobLoop<NUM_JOB_LEVEL; ++iJobLoop)
            {
                sprintf(szTemp, "%d_%d.frm", iCharLoop, iJobLoop);

                if( g_pGCDeviceManager2->CheckFileExist( szTemp ) ) {
                    m_pMotionIntro[iCharLoop][iJobLoop] = g_pGCDeviceManager2->CreateMotionFRM( szTemp );
                } else {
                    m_pMotionIntro[iCharLoop][iJobLoop] = g_pGCDeviceManager2->CreateMotionFRM( "184.frm" );
                }
            }
        }
    }
#else
    m_pMotionIntro = g_pGCDeviceManager2->CreateMotionFRM( "184.FRM" );
#endif
}

void KGCLoadingScene::SetWaiting( bool bIsWaiting )
{
	m_pNowLoading->ToggleRender( !bIsWaiting );
	m_pWaiting->ToggleRender( bIsWaiting );
}

void KGCLoadingScene::PostDraw( void )
{    
	
    for ( std::vector<KGCBillBoard>::iterator it = m_vecBillBoard.begin() ; it != m_vecBillBoard.end() ; it++ )
    {
        it->Render();
    }
}

void KGCLoadingScene::LoadAdTexture()
{
    int iAdNum;
    KGCLuaManager LuaMgr;   

    if( GCFUNC::LoadLuaScript( LuaMgr , "AdSceneList.lua" ) == false )
    {
        MessageBoxA( NULL, "AdSceneList.lua Load Failed!", NULL, MB_OK );
        return;
    }

    HRESULT hr = LuaMgr.BeginTable( "AdBoard" );
    LuaMgr.GetValue( "AD_NUM", iAdNum );
    int iCurrentAd = rand() % iAdNum;

    LuaMgr.BeginTable( iCurrentAd + 1 );
    {
        int iBillBoardNum;
        LuaMgr.GetValue( "BILLBOARD_NUM" , iBillBoardNum );

		int iPvPMapSize = g_kGlobalValue.GetPvPLoadingImageName().size();

		KPVPLodingImageFileName::iterator mIter = g_kGlobalValue.GetPvPLoadingImageName().find(iCurrentAd);
				
		if ( mIter == g_kGlobalValue.GetPvPLoadingImageName().end() || mIter->second.size() != 3 ) {
			mIter = g_kGlobalValue.GetPvPLoadingImageName().begin();	
		}

		if ( mIter == g_kGlobalValue.GetPvPLoadingImageName().end() || mIter->second.size() != 3 ) {
			std::vector<wstring> vecStr;
			vecStr.push_back(L"ui_match_load1.dds");
			vecStr.push_back(L"ui_match_load2.dds");
			vecStr.push_back(L"ui_match_load3.dds");

			KPVPLodingImageFileName mapPVPLoading;

			mapPVPLoading.insert( std::make_pair(0, vecStr ) );
			g_kGlobalValue.SetPvPLoadingImageName(mapPVPLoading);
			mIter = g_kGlobalValue.GetPvPLoadingImageName().find(0);
		}

        for ( int i = 1; i <= iBillBoardNum; ++i )
        {

            KGCBillBoard board;
            std::string szTextureName;
            LuaMgr.BeginTable( i );            

            LuaMgr.GetValue( "LEFT_TOP_X" , board.m_vPos[0].x );
            LuaMgr.GetValue( "LEFT_TOP_Y" , board.m_vPos[0].y );
            LuaMgr.GetValue( "RIGHT_BOTTOM_X" , board.m_vPos[1].x );
            LuaMgr.GetValue( "RIGHT_BOTTOM_Y" , board.m_vPos[1].y );


            LuaMgr.GetValue( "TEX_LEFT_TOP_X" , board.m_vTexUV[0].x );
            LuaMgr.GetValue( "TEX_LEFT_TOP_Y" , board.m_vTexUV[0].y );
            LuaMgr.GetValue( "TEX_RIGHT_BOTTOM_X" , board.m_vTexUV[1].x );
            LuaMgr.GetValue( "TEX_RIGHT_BOTTOM_Y" , board.m_vTexUV[1].y );
			if(iPvPMapSize == 0 || mIter == g_kGlobalValue.GetPvPLoadingImageName().end())
	            LuaMgr.GetValue( "TEXTURE_NAME", szTextureName );
			else
			{
				if(i == 1)
					szTextureName =  GCUTIL_STR::GCStrWideToChar(mIter->second[0].c_str());
				else if(i == 2)
					szTextureName =  GCUTIL_STR::GCStrWideToChar(mIter->second[1].c_str());
				else
					szTextureName =  GCUTIL_STR::GCStrWideToChar(mIter->second[2].c_str());
			}

            board.m_pTexture = g_pGCDeviceManager2->CreateTexture( szTextureName );

            m_vecBillBoard.push_back( board );

            LuaMgr.EndTable();

        }

    }
    LuaMgr.EndTable();
}

void KGCLoadingScene::ReleaseAdTexture()
{
    for ( std::vector<KGCBillBoard>::iterator it = m_vecBillBoard.begin() ; it != m_vecBillBoard.end() ; it++ )
    {
        it->Release();
    }    
    m_vecBillBoard.clear();
}

void KGCLoadingScene::ReleaseTempResource()
{
    ReleaseAdTexture();
}

void KGCLoadingScene::PostChildDraw()
{
    if( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) ||
		SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MINIGAME ||
		SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AUTOMATCH ||
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT

       )
    {
        return;
    }

    for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if( ! ( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_READY ) || 
            g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) ||
            g_MyD3D->MyPlayer[i]->m_kUserInfo.bHost ||
            g_MyD3D->MyPlayer[i]->IsLocalPlayer() )  ) 
            continue;

        if ( !g_MyD3D->MyPlayer[i]->m_pObject )
        {
            continue;
        }
        
        switch( g_MyD3D->MyPlayer[i]->Extra_Char_Num % GC_CHAR_NUM )
        {
        case GC_CHAR_DIO:
            g_MyD3D->MyPlayer[i]->ToggleExtraMesh( "WEAPON", true );
            break;
        case GC_CHAR_ZERO:
            g_MyD3D->MyPlayer[i]->ToggleExtraMesh( "GRANDARK_2ND", false );
            g_MyD3D->MyPlayer[i]->ToggleExtraMesh( "GRANDARK_2ND_1", false );
            g_MyD3D->MyPlayer[i]->ToggleExtraMesh( "GRANDARK_2ND_2", false );
            g_MyD3D->MyPlayer[i]->ToggleExtraMesh( "DOG_GRANDARK", false );
            break;
        case GC_CHAR_LUPUS:
            g_MyD3D->MyPlayer[i]->ToggleExtraMesh( "WEAPON_0", true );
            g_MyD3D->MyPlayer[i]->ToggleExtraMesh( "WEAPON_1", true );
            break;
        default:
            g_MyD3D->MyPlayer[i]->ToggleExtraMesh( "WEAPON", g_MyD3D->MyPlayer[i]->GetCurFormTemplate().WeaponRenderBasically );
            g_MyD3D->MyPlayer[i]->ToggleExtraMesh( "MERGE_WEAPON", g_MyD3D->MyPlayer[i]->GetCurFormTemplate().LeftWeaponRenderBasically );
            break;
        }
        
        g_MyD3D->MyPlayer[i]->Body_Angle = 0.0f;
        g_RenderManager->Render( false, g_MyD3D->MyPlayer[i]->m_pObject );

        if ( g_MyD3D->m_akPet[i]->m_pObject )
        {
            g_MyD3D->m_akPet[i]->m_pObject->SetWorldMat( &m_aMatPetTransform[i] );
            g_MyD3D->m_akPet[i]->m_pObject->SetFrame( &(g_MyD3D->m_akPet[i]->GetPetMotionFrm(EGCPETMOTION::GAME_LOADING))->GetFrameData( m_aiPetFrame[i] ) );       
            g_RenderManager->Render( false, g_MyD3D->m_akPet[i]->m_pObject );
        }

    }

}

void KGCLoadingScene::OnMiniGame()
{
	m_pWorldCupLoad->ToggleRender( false );
	m_pMatchLoad->ToggleRender( false );
	m_pIndigoLoad->ToggleRender( false );
	m_pMinigameLoad->ToggleRender( true );
    m_pAgitLoad->ToggleRender( false );

	for ( int i = 0; i < MAX_LOADING_CHAR; i++ )
	{
		if( m_apkCharLoadingWnd[i] == NULL )
			continue;

		m_apkCharLoadingWnd[i]->ToggleRender( false );
	}

	m_pNowLoading->ToggleRender( true );
	m_pWaiting->ToggleRender( false );

	m_pVS->ToggleRender( false );
    
    if( m_pTip )
        m_pTip->ToggleRender( false );

    if( m_pTipTitle )
        m_pTipTitle->ToggleRender( false );

    if( m_pTipContents )
        m_pTipContents->ToggleRender( false );
    
}

void KGCLoadingScene::OnAgit()
{
    m_pWorldCupLoad->ToggleRender( false );
    m_pMatchLoad->ToggleRender( false );
    m_pIndigoLoad->ToggleRender( false );
    m_pMinigameLoad->ToggleRender( false );
    m_pAgitLoad->ToggleRender( true );

    for ( int i = 0; i < MAX_LOADING_CHAR; i++ )
    {
        if( m_apkCharLoadingWnd[i] == NULL )
            continue;

        m_apkCharLoadingWnd[i]->ToggleRender( false );
    }

    m_pNowLoading->ToggleRender( true );
    m_pWaiting->ToggleRender( false );

    m_pVS->ToggleRender( false );
    if( m_pTip )
        m_pTip->ToggleRender( false );

    if( m_pTipTitle )
        m_pTipTitle->ToggleRender( false );

    if( m_pTipContents )
        m_pTipContents->ToggleRender( false );

}

void KGCLoadingScene::OnAutoMatch()
{
	m_pWorldCupLoad->ToggleRender( false );
	m_pMatchLoad->ToggleRender( false );
	m_pIndigoLoad->ToggleRender( false );
	m_pMinigameLoad->ToggleRender( false );
	m_pAgitLoad->ToggleRender( false );

	for ( int i = 0; i < MAX_LOADING_CHAR; i++ )
	{
		if( m_apkCharLoadingWnd[i] == NULL )
			continue;

		m_apkCharLoadingWnd[i]->ToggleRender( false );
	}

	m_pNowLoading->ToggleRender( false );
	m_pWaiting->ToggleRender( false );

	m_pVS->ToggleRender( false );
    if( m_pTip )
        m_pTip->ToggleRender( false );

    if( m_pTipTitle )
        m_pTipTitle->ToggleRender( false );

    if( m_pTipContents )
        m_pTipContents->ToggleRender( false );

}



void KGCLoadingScene::ActionPerformed( const KActionEvent& event )
{

}

void KGCLoadingScene::FrameMoveInEnabledState( void )
{

}

void KGCLoadingScene::CreateMotion( int nChar, int nPromotion )
{
#if defined( LOADING_CHAR_MOTION )
    if ( nChar < 0 || nChar >= GC_CHAR_NUM )
        return;

    if ( nPromotion < 0 || nPromotion >= NUM_JOB_LEVEL )
        return;

    CHAR szTemp[128];
    sprintf(szTemp, "%d_%d.frm", nChar, nPromotion);

    if( g_pGCDeviceManager2->CheckFileExist( szTemp ) ) {
        m_pMotionIntro[nChar][nPromotion] = g_pGCDeviceManager2->CreateMotionFRM( szTemp );
    } else {
        m_pMotionIntro[nChar][nPromotion] = g_pGCDeviceManager2->CreateMotionFRM( "184.frm" );
    }
    #endif
}


#ifdef PIXELTOTEXEL
#undef PIXELTOTEXEL
#endif
#define PIXELTOTEXEL(p) ( (p)/512.0f )

KGCBillBoard::KGCBillBoard()
{
    m_pTexture = NULL;
	m_vTexUV[0] = D3DXVECTOR2( 0.0f, 0.0f );
	m_vTexUV[1] = D3DXVECTOR2( 512.0f, 512.0f );

}

KGCBillBoard::~KGCBillBoard()
{
}

void KGCBillBoard::Render()
{
    g_pGCDeviceManager2->PushState();
    {
        // use box-filter
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );

        // enable alpha blend
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );

        g_pGCDeviceManager2->SetRenderState( D3DRS_ZENABLE,          false );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE,     false );	
        g_pGCDeviceManager2->SetRenderState( D3DRS_CULLMODE,         D3DCULL_NONE );

		if (g_pGCDeviceManager->GetWindowScaleX() > 1.0f || g_pGCDeviceManager->GetWindowScaleY() > 1.0f)
		{
			g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
			g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
			g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		}

        //m_pTexture->SetDeviceTexture();*/
        
        bool bRotate = false;
        
        if ( m_vTexUV[0].x > m_vTexUV[1].x || m_vTexUV[0].y > m_vTexUV[1].y )
        {
            bRotate = true;
        }
        
		g_pGCDeviceManager2->DrawInScreen(
			m_pTexture,m_vPos[0].x * g_pGCDeviceManager->GetWindowScaleX(),
			m_vPos[0].y * g_pGCDeviceManager->GetWindowScaleY(),
			m_vPos[1].x * g_pGCDeviceManager->GetWindowScaleX(), 
			m_vPos[1].y * g_pGCDeviceManager->GetWindowScaleY(), 
			PIXELTOTEXEL( m_vTexUV[0].x ), PIXELTOTEXEL( m_vTexUV[0].y ), PIXELTOTEXEL( m_vTexUV[1].x ), PIXELTOTEXEL( m_vTexUV[1].y ), bRotate ,10.0f,0xffffffff);

    }
    g_pGCDeviceManager2->PopState();
}

void KGCBillBoard::Release()
{
    if ( m_pTexture )
    {
        m_pTexture->Release();
    }
}


IMPLEMENT_CLASSNAME( KGCAutoMatchLoadingBarS6 );
IMPLEMENT_WND_FACTORY( KGCAutoMatchLoadingBarS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCAutoMatchLoadingBarS6, "gc_automatch_loading_bar" );

KGCAutoMatchLoadingBarS6::KGCAutoMatchLoadingBarS6( void )
: m_iGageType(0)
, m_pkGauge(NULL)
, m_pkUserNick(NULL)
, m_dwUserUID(0)
{
	LINK_CONTROL("gauge",m_pkGauge);
	LINK_CONTROL("name_static",m_pkUserNick);
}

KGCAutoMatchLoadingBarS6::~KGCAutoMatchLoadingBarS6( void )
{
}

void KGCAutoMatchLoadingBarS6::ActionPerformed( const KActionEvent& event )
{
}


void KGCAutoMatchLoadingBarS6::OnCreate( void )
{
	m_pkGauge->InitState(true);
	m_pkUserNick->InitState(true);
	m_dwUserUID = 0;
}

void KGCAutoMatchLoadingBarS6::SetInfo( DWORD dwUserUID, std::wstring wstrUserNick, int iProgressBarType )
{
	m_pkUserNick->SetText(wstrUserNick);
	m_dwUserUID = dwUserUID;
	m_iGageType = iProgressBarType;

	if ( iProgressBarType == AMLB_GAGE_START_LEFT )
		m_pkUserNick->SetAlign(DT_RIGHT);
}

void KGCAutoMatchLoadingBarS6::SetProgress( int iProgress )
{
	m_pkGauge->SetGauge( iProgress, 18, m_iGageType );
}


IMPLEMENT_CLASSNAME( KGCAutoMatchLoadingS6 );
IMPLEMENT_WND_FACTORY( KGCAutoMatchLoadingS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCAutoMatchLoadingS6, "gc_automatch_loading" );

KGCAutoMatchLoadingS6::KGCAutoMatchLoadingS6( void )
: m_tmCloseCheck(0)
{
	for ( int i = 0 ; i < AML_LOADINGBAR_MAX_CNT ; ++i ) {
		char str[MAX_PATH];
		m_saBar[i] = NULL;
		sprintf( str, "loding_bar_%d", i );
		LINK_CONTROL( str, m_saBar[i] );
	}
}

KGCAutoMatchLoadingS6::~KGCAutoMatchLoadingS6( void )
{
}

void KGCAutoMatchLoadingS6::ActionPerformed( const KActionEvent& event )
{
}


void KGCAutoMatchLoadingS6::OnCreate( void )
{
	for ( int i = 0 ; i < AML_LOADINGBAR_MAX_CNT ; ++i ) {
		m_saBar[i]->InitState(true);
	}
	time_t curTime = GCUTIL_TIME::GetCurTime();

	m_tmCloseCheck = curTime + 15;

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(), KGCUIScene::D3DWE_PRE_ENTER_SCENE);
	g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_PRE_ENTER_SCENE, this, &KGCAutoMatchLoadingS6::OnCloseLoading );
}


void KGCAutoMatchLoadingS6::InitUserInfo( void )
{
	std::map<int, std::vector<KPartyMember> > mapInfo =  SiKGCMatchManager()->GetMatchCompleteUser();

	for ( int i = 0 ; i < AML_LOADINGBAR_MAX_CNT ; ++i ) {
		m_saBar[i]->ToggleRender(false);
		m_saBar[i]->SetInfo(0,L"", KGCAutoMatchLoadingBarS6::AMLB_GAGE_START_LEFT);
	}
}


void KGCAutoMatchLoadingS6::SetUserInfo( )
{
	std::map<int, std::vector<KPartyMember> > mapUser =	SiKGCMatchManager()->GetMatchCompleteUser();

	std::vector<KPartyMember> vecBlueTeam;
	std::vector<KPartyMember> vecRedTeam;

	std::map<int, std::vector<KPartyMember> >::iterator mit = mapUser.find( KGCMatchCompleteDlgS6::KMCD_TEAM_BLUE );
	if ( mit != mapUser.end() ) vecBlueTeam = mit->second;

	mit = mapUser.find( KGCMatchCompleteDlgS6::KMCD_TEAM_RED );
	if ( mit != mapUser.end() ) vecRedTeam = mit->second;

	std::vector<KPartyMember>::iterator vitBlue = vecBlueTeam.begin();
	for ( int i = 0 ; i < 3 ; ++i ) {
		if ( vitBlue == vecBlueTeam.end() ) {
			m_saBar[i]->ToggleRender(false);
			continue;
		}
		m_saBar[i]->SetInfo(vitBlue->m_dwUID,vitBlue->m_strNickName, KGCAutoMatchLoadingBarS6::AMLB_GAGE_START_LEFT);
		m_saBar[i]->ToggleRender(true);
		m_saBar[i]->SetProgress(0);
		++vitBlue;
	}

	std::vector<KPartyMember>::iterator vitRed = vecRedTeam.begin();
	for ( int i = 3 ; i < AML_LOADINGBAR_MAX_CNT ; ++i ) {
		if ( vitRed == vecRedTeam.end() ) {
			m_saBar[i]->ToggleRender(false);
			continue;
		}

		m_saBar[i]->SetInfo(vitRed->m_dwUID,vitRed->m_strNickName, KGCAutoMatchLoadingBarS6::AMLB_GAGE_START_RIGHT);
		m_saBar[i]->ToggleRender(true);
		m_saBar[i]->SetProgress(0);
		++vitRed;
	}


}

void KGCAutoMatchLoadingS6::SetProgress( PAIR_DWORD_INT piUserInfo )
{
	for ( int i = 0 ; i < AML_LOADINGBAR_MAX_CNT ; ++i ) {
		if ( m_saBar[i]->IsUserUID(piUserInfo.first) ) {
			m_saBar[i]->SetProgress(piUserInfo.second);
			break;
		}
	}
	
}

void KGCAutoMatchLoadingS6::OnCloseLoading( const KActionEvent& event)
{
	if ( event.m_dwlParam == GS_ROOM || event.m_dwlParam == GS_GAME_LOADING )
		return;

	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCAutoMatchLoadingS6::FrameMoveInEnabledState( )
{
	time_t curTime = GCUTIL_TIME::GetCurTime();

	if ( curTime > m_tmCloseCheck ) {
		OnCloseLoading(KActionEvent(NULL,0,g_MyD3D->m_pStateMachine->GetState()));
		m_tmCloseCheck = curTime + 5;
	}
}

