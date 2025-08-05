// #include "stdafx.h"
// //
// #include "KGCTutorialOption.h"
// 
// 
// //
// #include "../KGCModeTutorial.h"
// #include "../GCStateGame.h"
// 
// #include "../GCStateMachine.h"
// 
// #include "GCCameraOrtho.h"
// #include "Headup Display.h"
// 
// #include "Stage.h"
// 
// 
// 
// 
// 
// 
// #include "KGCGameBoard.h"
// #include "GCGameOverlayUI.h"
// #include "KGCPlayerCommon.h"
// #include "KGCRoomManager.h"
// 
// 
// #include "./MapSystem/KGCLayerManager.h"
// #include "gcui/GCSkillTreeDlg.h"
// 
// 
// //
// 
// IMPLEMENT_CLASSNAME( KGCTutorialOption );
// IMPLEMENT_WND_FACTORY( KGCTutorialOption );
// IMPLEMENT_WND_FACTORY_NAME( KGCTutorialOption, "gc_tutorialoptoin_select_char" );
// 
// KGCTutorialOption::KGCTutorialOption( void )
// : m_bFirstTutorial( true )
// {
//     m_pkBackGround          = NULL; 
//     m_pkBtnOk               = NULL;
//     m_pkStaticTip           = NULL;
//     m_pkChangeBox           = NULL;
//     m_pkMySelBox            = NULL;
//     m_pkTutorialSelBox      = NULL;
//     m_pktabMyChar           = NULL;
//     m_pktabTutorialChar     = NULL;
//     m_pkExit                = NULL;
// 
//     // 맵의 실제 인덱스
//     m_iMapRealIndex[0]     = GC_GS_FOREST_OF_ELF_REVERS,
//     m_iMapRealIndex[1]     = GC_GS_VALLEY_OF_OATH_REVERS,
//     m_iMapRealIndex[2]     = GC_GS_FOREST_OF_ELF;
//     m_iMapRealIndex[3]     = GC_GS_SWAMP_OF_OBLIVION;
//     m_iMapRealIndex[4]     = GC_GS_FLYING_SHIP;
//     m_iMapRealIndex[5]     = GC_GS_VALLEY_OF_OATH;
//     m_iMapRealIndex[6]     = GC_GS_FOGOTTEN_CITY;
//     m_iMapRealIndex[7]     = GC_GS_SHOOTING_FIELD;
//     m_iMapRealIndex[8]     = GC_GS_HELL_BRIDGE;
//     m_iMapRealIndex[9]     = GC_GS_TEMPLE_OF_ORC;
//     m_iMapRealIndex[10]    = GC_GS_GUILD;
//     m_iMapRealIndex[11]    = GC_GS_CARRIBEACH;
//     m_iMapRealIndex[12]    = GC_GS_FOREST_OF_TRIAL;
//     m_iMapRealIndex[13]    = GC_GS_GRAVEYARD_OF_DECEASED;
//     m_iMapRealIndex[14]    = GC_GS_AMAZON_FOREST;
//     m_iMapRealIndex[15]    = GC_GS_MARKET_WATER;
// 
//     // 맵의 글자 인덱스
//     m_iMapStringIndex[0]     = STR_ID_GS_FOREST_OF_ELF_REVERS,
//     m_iMapStringIndex[1]     = STR_ID_GS_VALLEY_OF_OATH_REVERS,
//     m_iMapStringIndex[2]     = STR_ID_GS_FOREST_OF_ELF;
//     m_iMapStringIndex[3]     = STR_ID_GS_SWAMP_OF_OBLIVION;
//     m_iMapStringIndex[4]     = STR_ID_GS_FLYING_SHIP;
//     m_iMapStringIndex[5]     = STR_ID_GS_VALLEY_OF_OATH;
//     m_iMapStringIndex[6]     = STR_ID_GS_FOGOTTEN_CITY;
//     m_iMapStringIndex[7]     = STR_ID_GS_SHOOTING_FIELD;
//     m_iMapStringIndex[8]     = STR_ID_GS_HELL_BRIDGE;
//     m_iMapStringIndex[9]     = STR_ID_GS_TEMPLE_OF_ORC;
//     m_iMapStringIndex[10]    = STR_ID_GS_GUILD;
//     m_iMapStringIndex[11]    = STR_ID_GS_CARRIBEACH;
//     m_iMapStringIndex[12]    = STR_ID_GS_TRIAL;
//     m_iMapStringIndex[13]    = STR_ID_GS_GRAVEYARD_OF_DECEASED;
//     m_iMapStringIndex[14]    = STR_ID_GS_AMAZON_FOREST;
//     m_iMapStringIndex[15]    = STR_ID_GS_MARKET_WATER;

    //// 맵의 실제 인덱스
    //m_iMapRealIndex[0]      = GC_GS_AMAZON_FOREST;
    //m_iMapRealIndex[1]      = GC_GS_MARKET_WATER;
    //m_iMapRealIndex[2]      = GC_GS_FOREST_OF_ELF_REVERS;
    //m_iMapRealIndex[3]      = GC_GS_VALLEY_OF_OATH_REVERS;
    //m_iMapRealIndex[4]      = GC_GS_FOREST_OF_ELF;
    //m_iMapRealIndex[5]      = GC_GS_SWAMP_OF_OBLIVION;
    //m_iMapRealIndex[6]      = GC_GS_FLYING_SHIP;
    //m_iMapRealIndex[7]      = GC_GS_VALLEY_OF_OATH;
    //m_iMapRealIndex[8]      = GC_GS_FOGOTTEN_CITY;
    //m_iMapRealIndex[9]      = GC_GS_SHOOTING_FIELD;
    //m_iMapRealIndex[10]     = GC_GS_HELL_BRIDGE;
    //m_iMapRealIndex[11]     = GC_GS_TEMPLE_OF_ORC;
    //m_iMapRealIndex[12]     = GC_GS_GUILD;
    //m_iMapRealIndex[13]     = GC_GS_CARRIBEACH;
    //m_iMapRealIndex[14]     = GC_GS_FOREST_OF_TRIAL;
    //m_iMapRealIndex[15]     = GC_GS_GRAVEYARD_OF_DECEASED;

    //// 맵의 글자 인덱스
    //m_iMapStringIndex[0]        = STR_ID_GS_AMAZON_FOREST;
    //m_iMapStringIndex[1]        = STR_ID_GS_MARKET_WATER;
    //m_iMapStringIndex[2]        = STR_ID_GS_FOREST_OF_ELF_REVERS,
    //m_iMapStringIndex[3]        = STR_ID_GS_VALLEY_OF_OATH_REVERS,
    //m_iMapStringIndex[4]        = STR_ID_GS_FOREST_OF_ELF;
    //m_iMapStringIndex[5]        = STR_ID_GS_SWAMP_OF_OBLIVION;
    //m_iMapStringIndex[6]        = STR_ID_GS_FLYING_SHIP;
    //m_iMapStringIndex[7]        = STR_ID_GS_VALLEY_OF_OATH;
    //m_iMapStringIndex[8]        = STR_ID_GS_FOGOTTEN_CITY;
    //m_iMapStringIndex[9]        = STR_ID_GS_SHOOTING_FIELD;
    //m_iMapStringIndex[10]       = STR_ID_GS_HELL_BRIDGE;
    //m_iMapStringIndex[11]       = STR_ID_GS_TEMPLE_OF_ORC;
    //m_iMapStringIndex[12]       = STR_ID_GS_GUILD;
    //m_iMapStringIndex[13]       = STR_ID_GS_CARRIBEACH;
    //m_iMapStringIndex[14]       = STR_ID_GS_TRIAL;
    //m_iMapStringIndex[15]       = STR_ID_GS_GRAVEYARD_OF_DECEASED;

// #if defined( NATION_BRAZIL )
// 	m_iMapRealIndex[12] = GC_GS_BAHIA;
// 	m_iMapStringIndex[12] = STR_ID_GS_BAHIA;
// 
//     m_iMapRealIndex[13] = GC_GS_AMAZON_FOREST;
//    m_iMapStringIndex[13] = STR_ID_GS_AMAZON_FOREST;
//#endif
// 
// #if defined(NATION_THAILAND) //태국 수상시장맵 연습모드 추가
// 	m_iMapRealIndex[12] = GC_GS_MARKET_WATER;
// 	m_iMapStringIndex[12] = STR_ID_GS_MARKET_WATER;
// #endif
// 
// 
//     m_bVisible = false;
// 
//     // 컨트롤 설정
//     char strTemp[40];
//     for( int i = 0; i < GC_CHAR_NUM * NUM_JOB_LEVEL; ++i )
//     {
//         m_pkChar[i] = NULL;
//         sprintf( strTemp, "tutorial_char%d_%d", i / NUM_JOB_LEVEL, i % NUM_JOB_LEVEL );
//         LINK_CONTROL( strTemp, m_pkChar[i] );
//     }
// 
//     for( int i = 0; i < NUM_TUTORIAL_MAP ; ++i )
//     {
//         m_pkMap[i] = NULL;
//         sprintf( strTemp, "map_tutorial_stage%d", i );
//         LINK_CONTROL( strTemp, m_pkMap[i] );
//     }
// 
//     m_iTopIndex = 0;
//     m_pkScrollbar = NULL;
// 
//     LINK_CONTROL( "background_type1",           m_pkBackGround );
//     LINK_CONTROL( "background_type2",           m_pkCharBackGround );
//     LINK_CONTROL( "frame_my_char1",             m_pkMySelBG_Sel );
//     LINK_CONTROL( "frame_my_char2",             m_pkMySelBG_Def );
//     LINK_CONTROL( "frame_tutorial_char1",       m_pkTutorialSelBG_Sel );
//     LINK_CONTROL( "frame_tutorial_char2",       m_pkTutorialSelBG_Def );
//     LINK_CONTROL( "background_type3",           m_pkTipBG );
//     LINK_CONTROL( "frame_map_sel",              m_pkMapSelBG );
//     LINK_CONTROL( "ok_btn",                     m_pkBtnOk );
//     LINK_CONTROL( "close_btn",                  m_pkCloseBtn );
//     LINK_CONTROL( "map_change_box",             m_pkChangeBox );
//     LINK_CONTROL( "tutorial_select_box_tip",    m_pkStaticTip );
//     LINK_CONTROL( "char_sel_box_my",            m_pkMySelBox );
//     LINK_CONTROL( "char_sel_box_tutorial",      m_pkTutorialSelBox );
//     LINK_CONTROL( "sel_my_char",                m_pktabMyChar );
//     LINK_CONTROL( "sel_tutorial_char",          m_pktabTutorialChar );
//     LINK_CONTROL( "scroll",                     m_pkScrollbar );
//     LINK_CONTROL( "exit_button",                m_pkExit );
// 	LINK_CONTROL( "btn_background",				m_pkOkCancelBtnBackGround );
// 
//     m_iCurMapIndex = 0;
//     m_bMyCharSelMode = true;
// 
//     // 디버그 모드나 사내테섭일때만.. 연습모드에서 몬스터를 등장시킬 수 있다!
//    // m_pkMonsterSetting = NULL;
//    // LINK_CONTROL( "tutorial_monster_setting", m_pkMonsterSetting );
// }
// 
// KGCTutorialOption::~KGCTutorialOption( void )
// {
// }
// 
// void KGCTutorialOption::OnCreate( void )
// {
//     SetAlignType( WNDUIAT_EXPANTION );
//     static bool bFirst = true;
// #ifdef USE_MARI1
// 	fScaleFactor = 0.9f;
// #else
// 	fScaleFactor = 1.0f;
// #endif
// 
// 	SetAlignType(WNDUIAT_EXPANTION);    
// 
//     if( bFirst )
//     {
//         bFirst = false;
// #ifdef NEWBIE_GUIDE
// 		if( g_kGlobalValue.m_kLoginInfo.bNewUser == true)
// 		{
// #if defined(NATION_THAILAND)
// 
// 			//태국은 신규유저를 막아 놓은 상태로 보여 줍니다. 먼가 개지저분 하다.. 난중에 한번 정리 필요 할 것 같다.
// 			if( g_kGlobalValue.IsCharacterOpenFromServer() )
// 			{
// 				for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
// 				{
// 					if( g_kGlobalValue.IsOpenedChar( i ) == false )
// 					{
// 						AddNotPatchedChar( i );
// 						continue;
// 					}
// 					else
// 					{
// 						AddExistEnableChar( i );
// 					}
// 
// 					for( int j = 0 ; j < NUM_JOB_LEVEL ; ++j )
// 					{
// 						if( g_kGlobalValue.GetOpenedPromotion(i) >= j )
// 						{
// 							AddExistEnableExtraChar( i * NUM_JOB_LEVEL + j );
// 							continue;
// 						}
// 
// 						AddNotPatchedExtraChar( i * NUM_JOB_LEVEL + j );
// 					}
// 				}
// 			}
// 			else //풀린 케릭터 정보를 못 받는 경우.. 이딴일이 생기면 안되지만.. 일단 대충 선정해줌.
// 			{
// 				AddExistEnableChar( GC_CHAR_LAS );
// 				AddExistEnableChar( GC_CHAR_RYAN );
// 				AddExistEnableChar( GC_CHAR_RONAN );
// 				AddExistEnableChar( GC_CHAR_AMY );
// 				AddExistEnableChar( GC_CHAR_JIN );
// 				AddNotPatchedChar( GC_CHAR_SIEG );
// 				AddNotPatchedChar( GC_CHAR_MARI );
// 			}			
// #else
// 			//신규유저는 기본 케릭터만 보입니다.
// 			AddNotPatchedChar( GC_CHAR_LAS );
// 			AddNotPatchedChar( GC_CHAR_RYAN );
// 			AddNotPatchedChar( GC_CHAR_RONAN );
//             AddNotPatchedChar( GC_CHAR_AMY );
// 			AddNotPatchedChar( GC_CHAR_JIN );
// 			AddNotPatchedChar( GC_CHAR_SIEG );
// 			AddNotPatchedChar( GC_CHAR_MARI );
//             AddNotPatchedChar( GC_CHAR_DIO );
//             AddNotPatchedChar( GC_CHAR_ZERO );
//             AddNotPatchedChar( GC_CHAR_LEY );
//             AddNotPatchedChar( GC_CHAR_LUPUS );
// 			AddNotPatchedExtraChar( GC_CHAR_ELESIS * NUM_JOB_LEVEL + 1 );
// 			AddNotPatchedExtraChar( GC_CHAR_LIRE * NUM_JOB_LEVEL + 1 );
// 			AddNotPatchedExtraChar( GC_CHAR_ARME * NUM_JOB_LEVEL + 1 );
// #endif
// 		}
// 		else
// 		{
// 			SetTutorialCharList();
// 		}
// #else
// 		SetTutorialCharList();
// #endif
// 		vSingleBoxScale.x = (FLOAT)m_pkChar[0]->GetWidth();
// 		vSingleBoxScale.y = (FLOAT)m_pkChar[0]->GetHeight();		
// 		vSingleBoxScale *= fScaleFactor;
// 		vSingleBoxScale.x = vSingleBoxScale.x;
// 		vSingleBoxScale.y = vSingleBoxScale.y;
// 		D3DXVECTOR2 v기준 = m_pkChar[0]->GetFixedWindowLocalPos();
//         // 창에 표시될 18개 위치를 저장해놓는다
//         int iShown = 0;
//         for( int i = 0; i < GC_CHAR_NUM; ++i )
//         {
//             for( int j = 0; j < NUM_JOB_LEVEL; ++j )
//             {
//                 if( j >= SHOWWND_CHAR_JOB )
//                     continue;
//                 if (m_pkChar[ i * NUM_JOB_LEVEL + j ])
//                 {
// 					D3DXVECTOR2 vpos;
// 					vpos.x = v기준.x + i * vSingleBoxScale.x;
// 					vpos.y = v기준.y + j * vSingleBoxScale.y;
// 
//                     m_vecPos[ iShown++ ] = vpos;
//                 }                
//             }
//         }
// 	}
// 	else
// 	{
// 		SetTutorialCharList();
// 	}
// 
//     m_pkBackGround->InitState( true, false, this );
//     m_pkBtnOk->InitState( true, true, this );
//     m_pkStaticTip->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_TUTORIAL_SELECT_CHAR_POS ) );
// 
//     m_pkMySelBox->InitState( true, true, this );
//     m_pkTutorialSelBox->InitState( true, true, this );
//     m_pktabMyChar->InitState( true, true, this );
//     m_pktabTutorialChar->InitState( true, true, this );
// 
//     m_pkMySelBG_Sel->InitState( false, true, this );
//     m_pkMySelBG_Def->InitState( false, true, this );
//     m_pkTutorialSelBG_Sel->InitState( false, true, this );
//     m_pkTutorialSelBG_Def->InitState( false, true, this );
// 
//     m_pkCharBackGround->InitState( true, true, this );
// 
// 	m_pkOkCancelBtnBackGround->InitState( true, true, this );
// 
//     m_pkCloseBtn->InitState( true, true, this );
// 	//신규유저가 처음으로 튜토리얼을 할때는 가려줍니다.
// #ifdef NEWBIE_GUIDE
// 	m_pkExit->InitState( !(g_kGlobalValue.m_kLoginInfo.bNewUser && m_bFirstTutorial ), true, this );
// 	m_pkCloseBtn->Lock( g_kGlobalValue.m_kLoginInfo.bNewUser && m_bFirstTutorial  );
// #else
// 	m_pkExit->InitState( true, true, this );
// #endif
// 
//     UpdateTutorialCharBtn();
// 
//     for( int i = 0; i < NUM_TUTORIAL_MAP ; ++i )
//         m_pkMap[i]->InitState( true, true, this );
// 
// 
// 
//     for( int i = 0; i < GC_CHAR_NUM * NUM_JOB_LEVEL; ++i )
//     {
//         if ( m_pkChar[i] )
//         {
//             m_pkChar[i]->InitState( false );
//         }        
//     }
// 
//     m_pkChangeBox->SetSelfInputCheck( true );
//     m_pkChangeBox->AddActionListener( this );
// 
//     m_pkScrollbar->AddActionListener( this );
//     m_pkScrollbar->SetSelfInputCheck( false );
//     m_pkScrollbar->SetScrollPageSize( SHOWWND_CHAR_JOB );
//     // 3차 전직 캐릭터가 있다면 Range는 4 그렇지 않으면 3
// #if defined( USE_ELESIS4 ) || defined( USE_LIRE4 ) || defined( USE_ARME4 ) || defined( USE_LAS4 )
//     m_pkScrollbar->SetScrollRangeMax( 4 );
// #else
// 	m_pkScrollbar->ToggleRender( false );
//     m_pkScrollbar->SetScrollRangeMax( 3 );
// #endif
//     m_pkScrollbar->SetScrollPos( m_iTopIndex );
// 
//     SetSelfInputCheck( false );
// 
//     m_pkBtnOk->SetHotKey( DIK_RETURN );
//     m_pkBtnOk->SetHotKey( DIK_NUMPADENTER );
//     m_pkCloseBtn->SetHotKey( DIK_ESCAPE );
// 
//     SelectCharTab( 1 );
//     SelectMap( m_iCurMapIndex );
// 
//     // 현재 선택된 캐릭터가 보이도록 스크롤을 조절
//     int iTopIndex = 0;
//     if( m_bMyCharSelMode )
//         iTopIndex = m_pkMySelBox->GetCurCharIndex() % NUM_JOB_LEVEL;
//     else
//         iTopIndex =  m_pkTutorialSelBox->GetCurCharIndex() % NUM_JOB_LEVEL;
//     iTopIndex -= 2;
// 
//     if( iTopIndex < 0 )
//         iTopIndex = 0;
//     if( iTopIndex > m_pkScrollbar->GetScrollRangeMax() )
//         iTopIndex = m_pkScrollbar->GetScrollRangeMax();
// 
//     m_iTopIndex = iTopIndex;
// 
//     bool bCloseTest = false;
// #if defined( _DEBUG ) || !defined( __PATH__ )
//     // 이 변수를 true로 만들면 연습모드 창에서..
//     // 몬스터를 선택하는 UI를 등장시킬 수 있다..
//     // 근데.. 추가 작업이 좀 필요함..
//     // 현재 캐릭터 vs 캐릭터 일때는 예전 맵시스템을 사용하기 때문에 몬스터가 등장 할 수 없고..
//     // 몬스터 등장시킬 때는 예전 맵시스템이 아닌 FantasticMap을 로드해서 사용하도록 작업을
//     // 해야 한다..
//     //bCloseTest = true;
// #endif
// 
//     //m_pkMonsterSetting->InitState( bCloseTest );
// 
//     if( bCloseTest )
//         SetAllWindowPos( D3DXVECTOR2( 10, 50 ) );
// 
//     UpdateData();
// 
// 	if( g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT )
// 		g_MyD3D->MyPlayer[0]->PresetTutorialChars();
// 
//     //RefreshRelocatedPos();
// }
// 
// void KGCTutorialOption::UpdateTutorialCharBtn()
// {
//     // 내가 현재 연습모드에서 플레이 할 수 있는 캐릭터들을 설정한다
//     SetEnableChar();
// 
//     std::vector<int> vecRenderOn;
//     for( int i = m_iTopIndex; i < m_iTopIndex + SHOWWND_CHAR_JOB; ++i )
//     {
//         for( int j = 0; j < GC_CHAR_NUM; ++j )
//         {
//             if( m_setNotPatched.find( NUM_JOB_LEVEL * j ) != m_setNotPatched.end() )
//                 continue;
// 
//             vecRenderOn.push_back( NUM_JOB_LEVEL * j + i );
//         }
//     }
// 
// 
//     ResetWndSize();
// 
//     // 스크롤 올리거나 내리면 재배열 해야함
//     int iShown = 0;
//     for( int i = 0; i < GC_CHAR_NUM * NUM_JOB_LEVEL; ++i )
//     {
//         if( m_pkChar[i] == NULL )
//             continue;
// 
//         m_pkChar[i]->InitState( false );
//         std::vector<int>::iterator vitIsRender = std::find( vecRenderOn.begin(), vecRenderOn.end(), i );
// 
//         // 꺼야되는 윈도우 리스트에 없으면
//         if( vitIsRender != vecRenderOn.end() && iShown < GC_CHAR_NUM * SHOWWND_CHAR_JOB )
//         {
//             if( m_setNotPatched.count( i ) != 0 )
// 			{
// 				m_pkChar[i]->InitState( false );
// 				m_pkChar[i]->Lock( true );
// 				m_pkChar[i]->SetWndMode( D3DWM_LOCK );
// 				m_pkChar[i]->SetWindowColor( 0xFFFFFFFF );
//             }
// 			else if( m_setIfExistThenEnable.count( i ) != 0 )
// 			{
// 				m_pkChar[i]->InitState( true );
// 				m_pkChar[i]->Lock( false );
// 				m_pkChar[i]->SetWindowColor( 0xFF888888 );
// 			}
//             else
// 			{
// 				m_pkChar[i]->InitState( true, true, this );
// 				m_pkChar[i]->Lock( false );
// 				m_pkChar[i]->SetWindowColor( 0xFFFFFFFF );
//             }
// 
//             // 윈도우 위치 맞추기
//             m_pkChar[i]->SetAllWindowPos( m_vecPos[iShown++] );
// 
// 			m_pkChar[i]->SetWidth( (DWORD)vSingleBoxScale.x );
// 			m_pkChar[i]->SetHeight( (DWORD)vSingleBoxScale.y );
// 
//         }
//     }
// 
// 
// 	//게임중에 생성되었다면.. 가운데 작은 박스라는 셋팅해준다.
// 	if( GS_GAME == g_MyD3D->m_pStateMachine->GetState() )
// 	{
// 		SetAlignType(WNDUIAT_POSZEROBASE);
// 		SetAlignGap(D3DXVECTOR2(GetWidth()/2.0f,GetHeight()/2.0f));
// 	}
// 	else
// 	{
// 		SetAlignType(WNDUIAT_EXPANTION);
// 		SetAlignGap(D3DXVECTOR2(0,0));
// 	}
// 
//     if( GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() )
//     {
//         KGCModeTutorial* pMode = static_cast<KGCModeTutorial*>(g_pStateGame->GetCurGameModePtr());
//         if( pMode )
//             SelectMap( pMode->m_iTutorialStageIndex );
//     }
// 
//     if( m_bMyCharSelMode )
//         SetCharHover( m_pkMySelBox->GetCurCharIndex() );
//     else
//         SetCharHover( m_pkTutorialSelBox->GetCurCharIndex() );
// }
// 
// void KGCTutorialOption::FrameMoveInEnabledState( void )
// {
//     POINT       pt = g_pkInput->GetMousePos();
//     D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
// 
//     // 마우스 휠 돌릴때 스크롤 처리
//     if ( 0 != g_pkInput->GetZMov() )
//     {
//         if( g_pkInput->GetZMov() > 0 )
//         {
//             if( --m_iTopIndex < 0 )
//                 ++m_iTopIndex;
//             else
//                 g_KDSound.Play( "73" );
// 
//             UpdateData();
//         }
//         else 
//         {
//             if( ++m_iTopIndex > m_pkScrollbar->GetScrollRangeMax() )
//                 --m_iTopIndex;
//             else
//                 g_KDSound.Play( "73" );
// 
//             UpdateData();
//         }
//     }
// 
//     // 마우스 클릭시 처리
//     if( g_pkInput->BtnDown( KInput::MBLEFT ) )
//     {
//         D3DXVECTOR2 vCurPos = D3DXVECTOR2( static_cast<float>(g_pkInput->GetMousePos().x), static_cast<float>(g_pkInput->GetMousePos().y));
// 
//         // 우측 상단에 선택된 캐릭터를 골랐을 때
//         // 내캐릭터 혹은 연습캐릭터 선택 모드로 변경한다.
//         if( g_pkInput->BtnDown( KInput::MBLEFT ) )
//         {
//             if( m_pkMySelBG_Def->CheckPosInWindowBound(D3DXVECTOR2(static_cast<float>(g_pkInput->GetMousePos().x), static_cast<float>(g_pkInput->GetMousePos().y)))
//                 || m_pktabMyChar->CheckPosInWindowBound(D3DXVECTOR2(static_cast<float>(g_pkInput->GetMousePos().x), static_cast<float>(g_pkInput->GetMousePos().y)))
//                 )
//             {
//                 SelectCharTab(true);
//             }
//             else if( m_pkTutorialSelBG_Def->CheckPosInWindowBound(D3DXVECTOR2(static_cast<float>(g_pkInput->GetMousePos().x), static_cast<float>(g_pkInput->GetMousePos().y))) 
//                 || m_pktabTutorialChar->CheckPosInWindowBound(D3DXVECTOR2(static_cast<float>(g_pkInput->GetMousePos().x), static_cast<float>(g_pkInput->GetMousePos().y)))
//                 )
//             {
//                 SelectCharTab(false);
//             }
//         }
// 
//         // 캐릭터 선택 하는 부분.
//         if( m_pkCharBackGround->CheckPosInWindowBound( vCurPos ) )
//         {
//             for( int i = 0; i < GC_CHAR_NUM * NUM_JOB_LEVEL ; ++i )
//             {
//                 if ( !m_pkChar[i] )
//                 {
//                     continue;
//                 }
//                 // 보유하지 않으면 못하는 캐릭터 목록에 있는지 찾아본다
//                 if( m_setIfExistThenEnable.count(i) == 0 && !m_pkChar[i]->IsLocked() && m_pkChar[i]->IsRenderOn() )
//                 {
//                     if( m_pkChar[i]->CheckPosInWindowBound( vCurPos ) )
//                     {
//                         if( m_bMyCharSelMode )
//                             m_pkMySelBox->SelectCharicter( i );
//                         else
//                             m_pkTutorialSelBox->SelectCharicter( i );
// 
//                         SetCharHover( i );
//                         break;
//                     }
//                 }
//             }
//         }
//     }
//     else if( g_pkInput->BtnDown( KInput::MBRIGHT ) )
//     {
//         if( m_bMyCharSelMode )
//             SelectCharTab( false );
//         else
//             SelectCharTab( true );
// 
//         int iTopIndex = 0;
//         if( m_bMyCharSelMode )
//             iTopIndex = m_pkMySelBox->GetCurCharIndex() % NUM_JOB_LEVEL;
//         else
//             iTopIndex =  m_pkTutorialSelBox->GetCurCharIndex() % NUM_JOB_LEVEL;
//         iTopIndex -= 2;
// 
//         if( iTopIndex < 0 )
//             iTopIndex = 0;
//         if( iTopIndex > m_pkScrollbar->GetScrollRangeMax() )
//             iTopIndex = m_pkScrollbar->GetScrollRangeMax();
// 
//         m_iTopIndex = iTopIndex;
//         UpdateData();
//     }
// 
// 
// }
// 
// void KGCTutorialOption::ActionPerformed( const KActionEvent& event )
// {
//     if( event.m_pWnd == this )
//         return;
// 
//     //소리를 낸다
//     if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
//         g_KDSound.Play( "30" );
// 
//     //여기서는 클릭소리를 낸다
//     if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_DOWN ||
//         event.m_dwCode == KD3DWnd::D3DWE_RBUTTON_DOWN )
//         g_KDSound.Play( "31" );
// 
//     // 맵 바꾸기 박스 이벤트라면
//     if ( event.m_pWnd == m_pkChangeBox )
//     {
//         switch ( event.m_dwCode )
//         {
//             case KGCChangeBox::RIGHT_BUTTON: // 오른쪽 버튼을 클릭했을 때
//             {
//                 m_iCurMapIndex++;
//                 if( m_iCurMapIndex >= NUM_TUTORIAL_MAP )
//                     m_iCurMapIndex = 0;
// 
//                 SelectMap(m_iCurMapIndex);
//                 g_KDSound.Play( "31" );
//                 break;
//             }
//             case KGCChangeBox::LEFT_BUTTON: // 왼쪽 버튼을 클릭했을 때
//             {
//                 m_iCurMapIndex--;
//                 if( m_iCurMapIndex < 0 )
//                     m_iCurMapIndex = NUM_TUTORIAL_MAP-1;
// 
//                 SelectMap( m_iCurMapIndex );
//                 g_KDSound.Play( "31" );
//                 break;
//             }
//         }
//     }
// 
//     if( event.m_pWnd == m_pkScrollbar )
//     {
//         int iTop = m_iTopIndex;
//         switch( event.m_dwCode )
//         {
//             case 0:
//             case D3DWE_SCROLLBAR_DOWNBTN:
//             case D3DWE_SCROLLBAR_UPBTN:
//             case D3DWE_SCROLLBAR_DRAG:
//                 m_iTopIndex = m_pkScrollbar->GetScrollPos();
//                 break;
//         }
// 
//         if( iTop != m_iTopIndex )
//         {
//             UpdateData();
//             g_KDSound.Play( "73" );
//         }
//     }
// 
//     GCWND_MSG_MAP( m_pkBtnOk,                           KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickOk );
//     GCWND_MSG_MAP( m_pkCloseBtn,                        KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickClose );
//     GCWND_MSG_MAP( m_pkExit,                            KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickClose );
// }
// 
// void KGCTutorialOption::UpdateData()
// {
//     m_pkScrollbar->SetScrollPos( m_iTopIndex );
//     UpdateTutorialCharBtn();
// 
//    // m_pkMonsterSetting->UpdateMonsterPanel();
// }
// 
// void KGCTutorialOption::OnDestroyComplete( void )
// {
//     SpeakToActionListener( KActionEvent( this, DESTROYED ) );
// }
// 
// void KGCTutorialOption::OnClickOk(void)
// {
// 	// 게임 실행후 처음으로 시작하는 튜토리얼임을 알 수 있는 변수
// 	m_bFirstTutorial = false;
// 
//     // GCUIScene에 메시지를 보내서 ActionPerformed 함수에서 처리할 수도 있다.
//     // 여기서 얻어낸 값을 이용하여 게임에 적용하여야한다.
// 
//     // 채팅 에디트박스에 포커스가 가지 않도록 설정
//     ::SetFocus( g_hUIMainWnd );
//     g_pkGameOverlayUI->ShowChatToggleMsg(true);
//     KGCPlayerCommon::MakeSoundLoadList();
//     //////////////////////////////////////////////////////////////////////////
//     // 튜토리얼 중인 상태가 아니라면(로비에서 튜토리얼 옵션창을 클릭 한 상태라면)
//     // 튜토리얼 모드를 실행하고 캐릭터의 위치를 잡아준다.
//     if( GS_GAME != g_MyD3D->m_pStateMachine->GetState() )
//     {
//         SpeakToActionListener( KActionEvent( this, STARTTUTORIAL ) );
//         g_MyD3D->m_pStateMachine->GoTutorial();
// 
//         if( GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() )
//         {
//             KGCModeTutorial* pMode = static_cast<KGCModeTutorial*>(g_pStateGame->GetCurGameModePtr());
//             if( pMode )
//             {
//                 pMode->m_iTutorialStageIndex = m_iCurMapIndex;
// 
//                 g_MyD3D->MyPlayer[0]->vPos.x = pMode->GetCurStageMyCharPos(m_iCurMapIndex).x;
//                 g_MyD3D->MyPlayer[0]->vPos.y = pMode->GetCurStageMyCharPos(m_iCurMapIndex).y;
//                 g_MyD3D->MyPlayer[1]->vPos.x = pMode->GetCurStageTutorialCharPos(m_iCurMapIndex).x;
//                 g_MyD3D->MyPlayer[1]->vPos.y = pMode->GetCurStageTutorialCharPos(m_iCurMapIndex).y;
//                 
//                 pMode->CheckTutCharDirection();
//             }
// 
// 			if(g_kGlobalValue.m_kLoginInfo.bNewUser  && !g_MyD3D->m_kGuideMgr.IsCompleteGuideNewbie() )
// 			{
// 				g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_TUTORIAL_GUIDE_STRING1 ), KGCChatManager::CHAT_TYPE_ADMIN );
// 				g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_TUTORIAL_GUIDE_STRING2 ), KGCChatManager::CHAT_TYPE_ADMIN );
// 				g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_TUTORIAL_TOGGLE ), KGCChatManager::CHAT_TYPE_ADMIN);
// 			}
//         }
// 
//         return;
//     }
//     //////////////////////////////////////////////////////////////////////////
// 
// 
//     //////////////////////////////////////////////////////////////////////////
//     // 튜토리얼이 이미 실행중이라면 튜토리얼을 다시 로드한다.
//     LoadTutorial();
// 
//     KGCModeTutorial* pMode = static_cast<KGCModeTutorial*>(g_pStateGame->GetCurGameModePtr());
//     if( pMode )
//         pMode->InitAtGame();        // 기술표를 업데이트 한다.
//     //////////////////////////////////////////////////////////////////////////
// 
//     //D3DEditProc( WM_KILLFOCUS, 0 ,0 );
//     SpeakToActionListener( KActionEvent( this, EXIT ) );
// 
//     m_iTopIndex = 0;
//     UpdateData();
// }
// 
// void KGCTutorialOption::InitTutorialChar(void)
// {
//     bool bTutorialChar = true;    
// 
// 	for( int i = 0; i < MAX_PLAYER_NUM; ++i )
// 	{
// 		g_MyD3D->MyPlayer[i]->ReleasePlayerResource();
// 		g_MyD3D->MyPlayer[i]->MetamorphosisForm = FORM_NORMAL;
// 	}
// 
//     //////////////////////////////////////////////////////////////////////////
//     // 모션과 모델을 모두 지운다. 아니면 메모리가 캐릭터 수만큼(모델 및 모션만큼) 늘어난다.
//     // 메모리가 허용범위가 높다면 궂이 안해도 된다.
//     g_pGraphicsHelper->UnLoadPlayerActionMotion();
//     g_pGraphicsHelper->UnLoadPlayerModel();
//     //////////////////////////////////////////////////////////////////////////
// 
//     // 캐릭터 생성
//     g_MyD3D->MyPlayer[0]->InitTutorialChar( m_pkMySelBox->GetCurCharIndex() );	    
// 	g_MyD3D->MyPlayer[0]->SetPlayerName(GCFUNC::GetCharName( (m_pkMySelBox->GetCurCharIndex() - (m_pkMySelBox->GetCurCharIndex() % NUM_JOB_LEVEL )) / NUM_JOB_LEVEL ).c_str());
//     g_MyD3D->MyPlayer[0]->m_cLife = 1;
//     g_MyD3D->MyPlayer[0]->m_kUserInfo.iTeam = 0;
//     g_MyD3D->MyPlayer[0]->Item_Remain_Time[GC_GAME_ITEM_SUPER] = 100;
//     g_MyD3D->MyPlayer[0]->m_pObject->SetOutlineColor( EDGE_COLOR_FRIEND );
// 
//     for(int i=0;i<FORM_NUM;++i)
//     {
//         if( g_MyD3D->MyPlayer[0]->m_pObjMetaForm[i] )
//             g_MyD3D->MyPlayer[0]->m_pObjMetaForm[i]->SetOutlineColor( EDGE_COLOR_FRIEND );
//     }
// 
//     if( bTutorialChar )
//     {
// 		g_MyD3D->MyPlayer[1]->PresetTutorialChars();
//         g_MyD3D->MyPlayer[1]->InitTutorialChar(m_pkTutorialSelBox->GetCurCharIndex());
// #ifdef NATION_KOREA
// 		g_MyD3D->MyPlayer[1]->SetPlayerName( L"연습용 캐릭터");        
// #else
// 		g_MyD3D->MyPlayer[1]->SetPlayerName( L"GrandChase");        
// #endif
// 
//         g_MyD3D->MyPlayer[1]->m_kUserInfo.iTeam = 1;
//         g_MyD3D->MyPlayer[1]->m_kUserInfo.dwUID = g_MyD3D->MyPlayer[0]->m_kUserInfo.dwUID+1;
//         g_MyD3D->MyPlayer[1]->m_cLife = 1;
//         g_MyD3D->MyPlayer[1]->Item_Remain_Time[GC_GAME_ITEM_SUPER] = 100;
//         g_MyD3D->MyPlayer[1]->m_pObject->SetOutlineColor( EDGE_COLOR_ENEMY );
//         for(int i=0;i<FORM_NUM;++i)
//         {
//             if( g_MyD3D->MyPlayer[1]->m_pObjMetaForm[i] )
//                 g_MyD3D->MyPlayer[1]->m_pObjMetaForm[i]->SetOutlineColor( EDGE_COLOR_FRIEND );
//         }
//         ////////////////////////////////////////////////////////////////////////////
//     }
// 
//     // 캐릭터 상태바를 표시해주기 위해 접속한놈인척 한다.
//     g_pGameBoard->m_pStartingMember[0].dwUID = g_MyD3D->MyPlayer[0]->m_kUserInfo.dwUID;
//     g_pGameBoard->m_pStartingMember[0].IsLive = true;
// 
// 
//     g_pGameBoard->m_pStartingMember[1].dwUID = g_MyD3D->MyPlayer[1]->m_kUserInfo.dwUID;  
//     g_pGameBoard->m_pStartingMember[1].IsLive = bTutorialChar;
// 
//     for(int i=0;i<LATENCY*2;i++)
//     {
//         g_kLocalPlayerLatency[0].kRenderData[i].usMotion = g_MyD3D->MyPlayer[0]->GetCurFormTemplate().WAIT;
//         g_kLocalPlayerLatency[0].kRenderData[i].cFrame = i;
//     }
// 
//     //////////////////////////////////////////////////////////////////////////
//     // 카메라 타겟 설정
//     g_kCamera.SetTargetPlayer(0);
//     //////////////////////////////////////////////////////////////////////////
// 
//     //////////////////////////////////////////////////////////////////////////
//     // 현재의 게임 모드가 튜토리얼 모드인지 확인하고 GameModeInterface의 ptr을 받아서 쓴다.
//     if( GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() )
//     {
//         KGCModeTutorial* pMode = static_cast<KGCModeTutorial*>(g_pStateGame->GetCurGameModePtr());
//         if( pMode )
//         {
//             pMode->m_iTutorialStageIndex = m_iCurMapIndex;
// 
//             // 같은 스테이지면 이전위치를 그대로 넣어준다.
//             g_MyD3D->MyPlayer[0]->vPos.x = pMode->GetCurStageMyCharPos(m_iCurMapIndex).x;
//             g_MyD3D->MyPlayer[0]->vPos.y = pMode->GetCurStageMyCharPos(m_iCurMapIndex).y;
// 
//             if( bTutorialChar )
//             {
//                 g_MyD3D->MyPlayer[1]->vPos.x = pMode->GetCurStageTutorialCharPos(m_iCurMapIndex).x;
//                 g_MyD3D->MyPlayer[1]->vPos.y = pMode->GetCurStageTutorialCharPos(m_iCurMapIndex).y;
//             }
// 
//             // 캐릭터 인덱스 설정변경 및 캐릭터 기술표 변경
//             pMode->SetCharType(g_MyD3D->MyPlayer[0]->m_kUserInfo.cCharIndex );
//             pMode->CheckTutCharDirection();
//         }
//     }
//     //////////////////////////////////////////////////////////////////////////
// 
//     //케릭터 사운드 로딩 2007.8.30 milennium9
//     g_MyD3D->MyPlayer[0]->MakeSoundLoadList();
//     if( bTutorialChar )
//     {
//         g_MyD3D->MyPlayer[1]->MakeSoundLoadList();
//     }
// }
// 
// void KGCTutorialOption::LoadTutorial(void)
// {
//     ////////////////////////////////////////////////////////////////////////////
//     // 이전 스테이지 초기화
//     g_pStateGame->ClearStageResource();    
//     ////////////////////////////////////////////////////////////////////////////
// 
// 
//     //////////////////////////////////////////////////////////////////////////
//     // 스테이지 로드
//     const bool bSameStage = (g_MyD3D->MyStg->GetStage() == m_iMapRealIndex[m_iCurMapIndex]);
//     g_MyD3D->MyStg->SetStage( m_iMapRealIndex[m_iCurMapIndex] ); // Stage 초기화
//     g_MyD3D->MyStg->Load_Stage(/*m_strMapFileName[m_iCurMapIndex]*/);
//     //SiKGCLayerManager()->LoadScript(m_strMapFileName[m_iCurMapIndex]);
//     //////////////////////////////////////////////////////////////////////////
// 
// 
//     //////////////////////////////////////////////////////////////////////////
//     // 캐릭터들 초기화
//     g_MyD3D->MyHead->InitDataForTutorial();
// 
//     // 같은 스테이지라면 이전 위치를 그대로 사용한다.
//     const D3DXVECTOR3 vMyBeforePos = g_MyD3D->MyPlayer[0]->vPos;
//     const D3DXVECTOR3 vTutorialBeforePos = g_MyD3D->MyPlayer[1]->vPos;
// 
//     InitTutorialChar();
//     if( bSameStage )
//     {
//         g_MyD3D->MyPlayer[0]->vPos = vMyBeforePos;
//         g_MyD3D->MyPlayer[0]->vPos.y += 0.3f;
//         g_MyD3D->MyPlayer[1]->vPos = vTutorialBeforePos;
//         g_MyD3D->MyPlayer[1]->vPos.y += 0.3f;
//     }
// 
//     // 캐릭터를 서로 마주보게 설정한다.
//     if( g_MyD3D->MyPlayer[0]->vPos.x <= g_MyD3D->MyPlayer[1]->vPos.x )
//     {
//         g_MyD3D->MyPlayer[0]->bIsRight = true;
//         g_MyD3D->MyPlayer[0]->x_Speed = 0.001f;
//         g_MyD3D->MyPlayer[1]->bIsRight = false;
//         g_MyD3D->MyPlayer[1]->x_Speed = -0.001f;
//     }
//     else
//     {
//         g_MyD3D->MyPlayer[0]->bIsRight = false;
//         g_MyD3D->MyPlayer[0]->x_Speed = -0.001f;
//         g_MyD3D->MyPlayer[1]->bIsRight = true;
//         g_MyD3D->MyPlayer[1]->x_Speed = 0.001f;
//     }
// 
//     g_KDSound.FullLoadfromLoadList();
// }
// 
// void KGCTutorialOption::OnClickClose(void)
// {
// #ifdef NEWBIE_GUIDE
// 	if( g_kGlobalValue.m_kLoginInfo.bNewUser == true && m_bFirstTutorial == true )
// 		return;
// #endif
//     // GCUIScene에 메시지를 보내서 ActionPerformed 함수에서 처리할 수도 있다.
//     // 여기서 얻어낸 값을 이용하여 게임에 적용하여야한다.
//     SpeakToActionListener( KActionEvent( this, EXIT ) );
// 
//     m_iTopIndex = 0;
//     UpdateData();
// }
// 
// void KGCTutorialOption::SelectMap(int index)
// {
//     if( index < 0 || index >= NUM_TUTORIAL_MAP ) return;
//     m_iCurMapIndex = index;
//     m_pkChangeBox->SetText( g_pkStrLoader->GetString( m_iMapStringIndex[index] ) );
// 
// 
//     for(int i=0;i<NUM_TUTORIAL_MAP;i++)
//         m_pkMap[i]->ToggleRender( (index == i) );
// 
// 
// }
// 
// void KGCTutorialOption::SelectCharTab(bool bMyChar)
// {
//     if( bMyChar != m_bMyCharSelMode )
//     {
//         m_iTopIndex = 0;
//         UpdateData();
//     }
// 
//     if( bMyChar )
//     {
//         m_pktabMyChar->SetWndMode( D3DWM_HOVER );
//         m_pktabMyChar->SetSelfInputCheck( false );
//         m_pktabTutorialChar->SetSelfInputCheck( true );
// 
//         SetCharHover( m_pkMySelBox->GetCurCharIndex() );
// 
//         m_pkMySelBG_Def->ToggleRender( false );
//         m_pkMySelBG_Sel->ToggleRender( true );
//         m_pkTutorialSelBG_Def->ToggleRender( true );
//         m_pkTutorialSelBG_Sel->ToggleRender( false );
// 
//         m_bMyCharSelMode = true;
//     }
//     else
//     {
//         m_pktabTutorialChar->SetWndMode( D3DWM_HOVER );
//         m_pktabTutorialChar->SetSelfInputCheck( false );
//         m_pktabMyChar->SetSelfInputCheck( true );
// 
//         SetCharHover( m_pkTutorialSelBox->GetCurCharIndex() );
// 
//         m_pkMySelBG_Def->ToggleRender( true );
//         m_pkMySelBG_Sel->ToggleRender( false );
//         m_pkTutorialSelBG_Def->ToggleRender( false );
//         m_pkTutorialSelBG_Sel->ToggleRender( true );
// 
//         m_bMyCharSelMode = false;
//     }
// 
//     // 2007.06.20 Jaeho
//     // 이부분을 제거하면 SelfinputCheck가 false인 경우 더이상 버튼 클릭을 안받는다.
//     // 마우스 포커스를 가지고 있다가 Selfinputcheck를 하지 않으면 먹통이 되는거 같다.
//     m_pktabMyChar->ParentRendeOff();
//     m_pktabTutorialChar->ParentRendeOff();
// }
// 
// void KGCTutorialOption::SetCharHover( int index )
// {
//     if( index >= (GC_CHAR_NUM * NUM_JOB_LEVEL-1) || index < 0 ) 
//         return;
//     if( m_pkChar[index]->IsLocked() ) 
//         return;
// 
//     for( int i = 0; i < GC_CHAR_NUM * NUM_JOB_LEVEL; ++i )
//     {
//         if( m_pkChar[i] == NULL )
//             continue;
// 
//         if( index == i )
//         {
//             m_pkChar[i]->SetWndMode( D3DWM_HOVER );
//             m_pkChar[i]->SetSelfInputCheck( false );
//         }
//         else if( m_setNotPatched.count(i) == 0 )
//         {
//             //m_pkChar[i]->SetWndMode( D3DWM_DEFAULT );
//             m_pkChar[i]->SetSelfInputCheck( true );
//         }
// 
//         // 2007.06.20 Jaeho
//         // 이부분을 제거하면 SelfinputCheck가 false인 경우 더이상 버튼 클릭을 안받는다.
//         // 마우스 포커스를 가지고 있다가 Selfinputcheck를 하지 않으면 먹토이 되는거 같다.
//         m_pkChar[i]->ParentRendeOff();
//     }
// }
// 
// void KGCTutorialOption::SetEnableChar()
// {
// 	// 초기화 시키면서 위치를 정해 줍시다.
//     for( int i = 0; i < GC_CHAR_NUM * NUM_JOB_LEVEL; ++i )
//     {
//         if( m_pkChar[i] == NULL )
//             continue;
// 
// 		if( m_setNotPatched.count( i ) != 0 )
// 		{
// 			m_pkChar[i]->ToggleRender(false);
// 			continue;
// 		}
// 
// 		if( m_setIfExistThenEnable.count( i ) != 0 )
// 		{
// 			m_pkChar[i]->InitState( true );
// 			m_pkChar[i]->Lock( true );
// 			m_pkChar[i]->SetWindowColor( 0xFF888888 );
// 			continue;
// 		}
// 
// 		if( m_pkChar[i] != NULL )
// 		{
// 			m_pkChar[i]->InitState( true, true, this );
// 			m_pkChar[i]->Lock( false );
// 			m_pkChar[i]->SetWndMode( D3DWM_DEFAULT );
// 		}
// 	}
// 
//     KCharInfoVector& vecCharInfo = g_kGlobalValue.m_kUserInfo.vecCharInfo;
// 
//     // 2007/1/31. iridology. 안된다 이거!
//     if( (int)vecCharInfo.size() < 3 ) // 캐릭터 숫자가 셋보다 적을순 없음
//         return;
// 
//     // 내가 플레이 할 수 있는 캐릭터들을 벡터에 추가..
//     int iIndex = -1;
//     KCharInfoVector::iterator vit;
// 
//     for( vit = vecCharInfo.begin(); vit != vecCharInfo.end(); ++vit )
//     {
//         for( int iPl = 0; iPl <= vit->iPromotionLevel; ++iPl )
//         {
//             iIndex = NUM_JOB_LEVEL * vit->iCharType + iPl;
// 
//             if( m_setIfExistThenEnable.count( iIndex ) != 0 &&
// 				m_pkChar[iIndex] != NULL )
//             {
// 				m_pkChar[iIndex]->InitState( true, true, this );
// 				m_pkChar[iIndex]->Lock( false );
// 				m_pkChar[iIndex]->SetWndMode( D3DWM_DEFAULT );
// 
// 				if( m_setIfExistThenEnable.count( iIndex ) != 0 )
// 				{
// 					m_setIfExistThenEnable.erase(iIndex);
// 				}
//             }
//         }
//     }
// 
// 	//창의 크기를 조절해 줍시다...
// 	ResetWndSize();
// }
// 
// void KGCTutorialOption::AddExistEnableChar( int iCharType )
// {
//     // 미국과 태국은 얻은 캐릭터만 사용.
// #if !(defined(NATION_USA) || defined(NATION_THAILAND) || defined( NATION_EU )) 
//     return;
// #endif
// 
// 	if( iCharType < 0 ) return;
// 	if( iCharType >= GC_CHAR_NUM ) return;
// 
// 	for( int i = 0 ; i < NUM_JOB_LEVEL ; ++i )
// 	{
// 		m_setIfExistThenEnable.insert( i + iCharType * NUM_JOB_LEVEL );
// 	}
// }
// 
// void KGCTutorialOption::AddExistEnableExtraChar( int iExtraCharType )
// {
//     // 미국과 태국 얻은 캐릭터만 사용.
// #if !(defined(NATION_USA) || defined(NATION_THAILAND) || defined( NATION_EU ))
//     return;
// #endif
// 
// 	if( iExtraCharType < 0 ) return;
// 
// 	m_setIfExistThenEnable.insert( iExtraCharType );
// }
// 
// void KGCTutorialOption::AddNotPatchedChar( int iCharType )
// {
// 	if( iCharType < 0 ) return;
// 	if( iCharType >= GC_CHAR_NUM ) return;
// 
// 	for( int i = 0 ; i < NUM_JOB_LEVEL ; ++i )
// 	{
// 		m_setNotPatched.insert( i + iCharType * NUM_JOB_LEVEL );
// 	}
// }
// 
// void KGCTutorialOption::AddNotPatchedExtraChar( int iExtraCharType )
// {
// 	if( iExtraCharType < 0 ) return;
// 
// 	// 생각해 보니 1차 전직이 패치되지 않았으면 2차 3차도 없네?
// 	int iCharType = (iExtraCharType - (iExtraCharType % NUM_JOB_LEVEL )) / NUM_JOB_LEVEL;
// 	for( int i = iExtraCharType % NUM_JOB_LEVEL ; i < NUM_JOB_LEVEL ; ++i )
// 	{
// 		m_setNotPatched.insert( i + iCharType * NUM_JOB_LEVEL );
// 	}
// }
// 
// void KGCTutorialOption::UpdateDialogSize()
// {
// 
// }
// 
// void KGCTutorialOption::ResetWndSize()
// {
// 	int iWidthCount = 0;
// 	int iHeightCount = 0;
// 	//가로길이를 위해 활성화된 케릭터 수를 셉니다.
// 	for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
// 	{
// 		int iExtraType = i * NUM_JOB_LEVEL;
// 
// 		if( m_pkChar[iExtraType] == NULL )
// 			continue;
// 
// 		if( m_pkChar[iExtraType]->IsRenderOn() )
// 			++iWidthCount;
// 	}
// 
// 	if( iWidthCount <= 3 )
// 		iWidthCount = 4;
// 
// 	//세로길이를 위해 최대로 전직된 케릭터의 전직 상태를 알아봅니다.
// 	for( int i = 0 ; i < NUM_JOB_LEVEL ; ++i )
// 	{
// 		for( int j = 0 ; j < GC_CHAR_NUM ; ++j )
// 		{
// 			int iExtraType = i + j * NUM_JOB_LEVEL;
// 
// 			if( m_pkChar[iExtraType] == NULL &&
// 				m_pkChar[iExtraType]->IsRenderOn() == false)
// 				continue;
// 
// 			iHeightCount = i;
// 			break;
// 		}
// 	}
// 
// 	//케릭터 선택창의 사이즈를 조절합니다.
// 	m_pkCharBackGround->SetSizeWithoutEdge( (int)(vSingleBoxScale.x * iWidthCount), (int)(vSingleBoxScale.y * iHeightCount) );
// 
// 	//케릭터 선택창의 사이즈에 따라서 다른 창들의 위치가 변경됩니다.
// 	//여기서 나오는 마법의 숫자는 UI 사이의 갭입니다. 알아서 조절하세요~
// 	D3DXVECTOR2 vSelCharPos = m_pkCharBackGround->GetFixedWindowLocalPos();
// 	D3DXVECTOR2 vScrollBarPos = m_pkScrollbar->GetFixedWindowLocalPos();
// 	vScrollBarPos.x = vSelCharPos.x + m_pkCharBackGround->GetWidth();
// 	m_pkScrollbar->SetWindowPosDirect( vScrollBarPos );
// 	m_pkScrollbar->SetHeight( m_pkCharBackGround->GetHeight() );
// 
// 	D3DXVECTOR2 vMyCharPos = m_pkMySelBG_Def->GetFixedWindowLocalPos();
// 	vMyCharPos.x = vSelCharPos.x + m_pkCharBackGround->GetWidth() + 28;
// 	m_pkMySelBG_Def->SetWindowPosDirect( vMyCharPos );
// 	m_pkMySelBG_Sel->SetWindowPosDirect( vMyCharPos );
// 	D3DXVECTOR2 vMyCharPos2 = m_pkMySelBox->GetFixedWindowLocalPos();
// 	vMyCharPos2.x = vMyCharPos.x + 7;
// 	m_pkMySelBox->SetWindowPosDirect( vMyCharPos2 );
// 
// 	D3DXVECTOR2 vTutoCharPos = m_pkTutorialSelBG_Def->GetFixedWindowLocalPos();
// 	vTutoCharPos.x = vSelCharPos.x + m_pkCharBackGround->GetWidth() + m_pkMySelBG_Def->GetWidth() + 40/*28 + 12*/;
// 	m_pkTutorialSelBG_Def->SetWindowPosDirect( vTutoCharPos );
// 	m_pkTutorialSelBG_Sel->SetWindowPosDirect( vTutoCharPos );
// 	D3DXVECTOR2 vTutoCharPos2 = m_pkTutorialSelBox->GetFixedWindowLocalPos();
// 	vTutoCharPos2.x = vTutoCharPos.x + 7;
// 	m_pkTutorialSelBox->SetWindowPosDirect( vTutoCharPos2 );
// 
// 	D3DXVECTOR2 vMapPos = m_pkMap[0]->GetFixedWindowLocalPos();
// 	vMapPos.x = vSelCharPos.x + m_pkCharBackGround->GetWidth() + 28;
// 	for( int i = 0 ; i < NUM_TUTORIAL_MAP ; ++i )
// 	{
// 		m_pkMap[i]->SetWindowPosDirect( vMapPos );
// 	}
// 	D3DXVECTOR2 vMapChangePos = m_pkChangeBox->GetFixedWindowLocalPos();
// 	vMapChangePos.x = vSelCharPos.x + m_pkCharBackGround->GetWidth() + 28;
// 	m_pkChangeBox->SetWindowPosDirect( vMapChangePos );
// 	D3DXVECTOR2 vMapBGPos = m_pkMapSelBG->GetFixedWindowLocalPos();
// 	vMapBGPos.x = vSelCharPos.x + m_pkCharBackGround->GetWidth() + 28;
// 	m_pkMapSelBG->SetWindowPosDirect( vMapBGPos );
// 
// 	D3DXVECTOR2 vTipPos = m_pkTipBG->GetFixedWindowLocalPos();
// 	vTipPos.x = vSelCharPos.x + m_pkCharBackGround->GetWidth() + 28;
// 	m_pkTipBG->SetWindowPosDirect( vTipPos );
// 
// 	D3DXVECTOR2 vTipStaticPos = m_pkStaticTip->GetFixedWindowLocalPos();
// 	vTipStaticPos.x = vTipPos.x + 12;
// 	m_pkStaticTip->SetWindowPosDirect( vTipStaticPos );
// 
// 	//전체창의 크기를 줄여줍시다.
// 	int ySize = m_pkCharBackGround->GetHeight() + 113;
// 	if( ySize < 505 )
// 		ySize = 505;
// 	m_pkBackGround->SetSize(m_pkCharBackGround->GetWidth() + 227, ySize );
// 
// 	//확인 취소 종료 버튼의 위치를 잡아줍시다.
// 	D3DXVECTOR2 vOkPos = m_pkBtnOk->GetFixedWindowLocalPos();
// 	vOkPos.x = m_pkBackGround->GetFixedWindowLocalPos().x + ( m_pkBackGround->GetWidth() / 2 ) - m_pkBtnOk->GetWidth() - 5;
// 	vOkPos.y = m_pkBackGround->GetFixedWindowLocalPos().y + m_pkBackGround->GetHeight() - 50;
// 	m_pkBtnOk->SetWindowPosDirect( vOkPos );
// 
// 	D3DXVECTOR2 vClosePos = m_pkCloseBtn->GetFixedWindowLocalPos();
// 	vClosePos.x = m_pkBackGround->GetFixedWindowLocalPos().x + ( m_pkBackGround->GetWidth() / 2 );
// 	vClosePos.y = m_pkBackGround->GetFixedWindowLocalPos().y + m_pkBackGround->GetHeight() - 50;
// 	m_pkCloseBtn->SetWindowPosDirect( vClosePos );
// 
// 	D3DXVECTOR2 vExitPos = m_pkExit->GetFixedWindowLocalPos();
// 	vExitPos.x = m_pkBackGround->GetFixedWindowLocalPos().x + m_pkBackGround->GetWidth() - m_pkExit->GetWidth() - 10;
// 	
// 	m_pkExit->SetWindowPosDirect( vExitPos );
// 
// 	D3DXVECTOR2 vOkCancelBtnBackGroundPos = m_pkOkCancelBtnBackGround->GetFixedWindowLocalPos();
// 	vOkCancelBtnBackGroundPos.x = m_pkBackGround->GetFixedWindowLocalPos().x + (m_pkBackGround->GetWidth() / 2) - (m_pkOkCancelBtnBackGround->GetWidth() / 2 ) - 3;
// 	vOkCancelBtnBackGroundPos.y = m_pkBackGround->GetFixedWindowLocalPos().y + m_pkBackGround->GetHeight() - 55;
// 	m_pkOkCancelBtnBackGround->SetWindowPos( vOkCancelBtnBackGroundPos );
// //#endif
// 
//     SetWindowPosDirect( D3DXVECTOR2( ( 800.0f - (float)m_pkBackGround->GetWidth() ) / 2.0f , ( 600.0f - (float)m_pkBackGround->GetHeight() ) / 2.0f ) );
// }
// 
// void KGCTutorialOption::SetTutorialCharList()
// {
// 	m_setIfExistThenEnable.clear();	// 획득해야만 활성화 되는 케릭터;
// 	m_setNotPatched.clear();	// 아직 패치되지 않은 케릭터; 비활성화 창마저 가리자..
// 
//     // 얻지 않으면 해볼 수 없는 케릭터들 리스트를 만努척?
//     /*AddExistEnableChar( GC_CHAR_LAS );
//     AddExistEnableChar( GC_CHAR_RYAN );
//     AddExistEnableChar( GC_CHAR_RONAN );*/
//     //AddExistEnableChar( GC_CHAR_AMY );
// 
//     if( g_kGlobalValue.IsCharacterOpenFromServer() )
//     {
//         for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
//         {
//             if( g_kGlobalValue.IsOpenedChar( i ) == false )
//             {
//                 AddNotPatchedChar( i );
//                 continue;
//             }
// 
//             for( int j = 0 ; j < NUM_JOB_LEVEL ; ++j )
//             {
//                 if( g_kGlobalValue.GetOpenedPromotion(i) >= j )
//                     continue;
// 
//                 AddNotPatchedExtraChar( i * NUM_JOB_LEVEL + j );
//             }
//         }
//     }
//     else
//     {
//         // 아직 패치되지 않은 케릭터들 리스트를 만努척?
// #if !defined( USE_LAS1 )
//         AddNotPatchedChar( GC_CHAR_LAS );
// #endif
// #if !defined( USE_RYAN1 )
//         AddNotPatchedChar( GC_CHAR_RYAN );
// #endif
// #if !defined( USE_RONAN )
//         AddNotPatchedChar( GC_CHAR_RONAN );
// #endif
// #if !defined( USE_AMY1 )
//         AddNotPatchedChar( GC_CHAR_AMY );
// #endif
// #if !defined( USE_JIN1 )
//         AddNotPatchedChar( GC_CHAR_JIN );
// #endif
// 
// #if !defined( USE_SIEG1 )
//         AddNotPatchedChar( GC_CHAR_SIEG );
// #endif
// 
// #if !defined( USE_MARI1 )
// 		AddNotPatchedChar( GC_CHAR_MARI );
// #endif
// 
// #if !defined( USE_DIO1 )
// 		AddNotPatchedChar( GC_CHAR_DIO );
// #endif
// 
// #if !defined( USE_ELESIS2 )
//         AddNotPatchedExtraChar( GC_CHAR_ELESIS * NUM_JOB_LEVEL + 1 );
// #elif !defined( USE_ELESIS3 )
//         AddNotPatchedExtraChar( GC_CHAR_ELESIS * NUM_JOB_LEVEL + 2 );
// #elif !defined( USE_ELESIS4 )
//         AddNotPatchedExtraChar( GC_CHAR_ELESIS * NUM_JOB_LEVEL + 3 );
// #endif
// 
// 
// #if !defined( USE_LIRE2 )
//         AddNotPatchedExtraChar( GC_CHAR_LIRE * NUM_JOB_LEVEL + 1 );
// #elif !defined( USE_LIRE3 )
//         AddNotPatchedExtraChar( GC_CHAR_LIRE * NUM_JOB_LEVEL + 2 );
// #elif !defined( USE_LIRE4 )
//         AddNotPatchedExtraChar( GC_CHAR_LIRE * NUM_JOB_LEVEL + 3 );
// #endif
// 
// 
// #if !defined( USE_ARME2 )
//         AddNotPatchedExtraChar( GC_CHAR_ARME * NUM_JOB_LEVEL + 1 );
// #elif !defined( USE_ARME3 )
//         AddNotPatchedExtraChar( GC_CHAR_ARME * NUM_JOB_LEVEL + 2 );
// #elif !defined( USE_ARME4 )
//         AddNotPatchedExtraChar( GC_CHAR_ARME * NUM_JOB_LEVEL + 3 );
// #endif
// 
// 
// #if !defined( USE_LAS2 )
//         AddNotPatchedExtraChar( GC_CHAR_LAS * NUM_JOB_LEVEL + 1 );
// #elif !defined( USE_LAS3 )
//         AddNotPatchedExtraChar( GC_CHAR_LAS * NUM_JOB_LEVEL + 2 );
// #elif !defined( USE_LAS4 )
//         AddNotPatchedExtraChar( GC_CHAR_LAS * NUM_JOB_LEVEL + 3 );
// #endif
// 
// 
// #if !defined( USE_RYAN2 )
//         AddNotPatchedExtraChar( GC_CHAR_RYAN * NUM_JOB_LEVEL + 1 );
// #elif !defined( USE_RYAN3 )
//         AddNotPatchedExtraChar( GC_CHAR_RYAN * NUM_JOB_LEVEL + 2 );
// #elif !defined( USE_RYAN4 )
//         AddNotPatchedExtraChar( GC_CHAR_RYAN * NUM_JOB_LEVEL + 3 );
// #endif
// 
// #if !defined( USE_RONAN2 )
//         AddNotPatchedExtraChar( GC_CHAR_RONAN * NUM_JOB_LEVEL + 1 );
// #elif !defined( USE_RONAN3 )
//         AddNotPatchedExtraChar( GC_CHAR_RONAN * NUM_JOB_LEVEL + 2 );
// #elif !defined( USE_RONAN4 )
//         AddNotPatchedExtraChar( GC_CHAR_RONAN * NUM_JOB_LEVEL + 3 );
// #endif
// 
// #if !defined( USE_AMY2 )
//         AddNotPatchedExtraChar( GC_CHAR_AMY * NUM_JOB_LEVEL + 1 );
// #elif !defined( USE_AMY3 )
//         AddNotPatchedExtraChar( GC_CHAR_AMY * NUM_JOB_LEVEL + 2 );
// #elif !defined( USE_AMY4 )
//         AddNotPatchedExtraChar( GC_CHAR_AMY * NUM_JOB_LEVEL + 3 );
// #endif    
// 
// #if !defined( USE_JIN2 )
//         AddNotPatchedExtraChar( GC_CHAR_JIN * NUM_JOB_LEVEL + 1 );
// #elif !defined( USE_JIN3 )
//         AddNotPatchedExtraChar( GC_CHAR_JIN * NUM_JOB_LEVEL + 2 );
// #elif !defined( USE_JIN4 )
//         AddNotPatchedExtraChar( GC_CHAR_JIN * NUM_JOB_LEVEL + 3 );
// #endif    
// 
// #if !defined( USE_SIEG2 )
//         AddNotPatchedExtraChar( GC_CHAR_SIEG * NUM_JOB_LEVEL + 1 );
// #elif !defined( USE_SIEG3 )
//         AddNotPatchedExtraChar( GC_CHAR_SIEG * NUM_JOB_LEVEL + 2 );
// #elif !defined( USE_SIEG4 )
//         AddNotPatchedExtraChar( GC_CHAR_SIEG * NUM_JOB_LEVEL + 3 );
// #endif    
// 
// 
// #if !defined( USE_MARI2 )
// 		AddNotPatchedExtraChar( GC_CHAR_MARI * NUM_JOB_LEVEL + 1 );
// #elif !defined( USE_MARI3 )
// 		AddNotPatchedExtraChar( GC_CHAR_MARI * NUM_JOB_LEVEL + 2 );
// #elif !defined( USE_MARI4 )
// 		AddNotPatchedExtraChar( GC_CHAR_MARI * NUM_JOB_LEVEL + 3 );
// #endif    
// 
// 
// #if !defined( USE_DIO2 )
// 		AddNotPatchedExtraChar( GC_CHAR_DIO * NUM_JOB_LEVEL + 1 );
// #elif !defined( USE_DIO3 )
// 		AddNotPatchedExtraChar( GC_CHAR_DIO * NUM_JOB_LEVEL + 2 );
// #elif !defined( USE_DIO4 )
// 		AddNotPatchedExtraChar( GC_CHAR_DIO * NUM_JOB_LEVEL + 3 );
// #endif    
//     }
// }
// 
// bool KGCTutorialOption::InitDialog( IN const KDialogInfo& kInfo_ )
// {
//     this->UpdateTutorialCharBtn();
//     return true;
// }
