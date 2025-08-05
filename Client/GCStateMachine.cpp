#include "stdafx.h"
//
#include <KNC/ClientErr.h>

#include "MyD3D.h"
#include "GCStateMachine.h"
#include "GCStateInitLoading.h"
#include "GCStateLogo.h"
#include "GCStateGuild.h"
#include "GCStateRoom.h"
#include "GCStateGame.h"
#include "GCStateCashShop.h"
#include "GCStateGPShop.h"
#include "GCStateMyInfo.h"
#include "GCStateOption.h"
#include "GCStateLoading.h"
#include "GCStateLogin.h"
#include "GCStateRegister.h"
#include "GCStateMyInfoFromRoom.h"
#include "GCStateTutorial.h"
#include "GCStateReplay.h"
#include "KGCStateEndGame.h"
#include "GCStateServer.h"
#include "GCStateMatch.h"
#include "GCStateWorldMap.h"
#include "GCStateSquareLoading.h"
#include "GCStateSquare.h"
#include "GCStateSquareEnd.h"
#include "GCStateExitGame.h"
#include "GCStateCoordiShop.h"
#include "GCStateCharSelect.h"
#include "GCStateCharGain.h"
#include "GCStateIntro.h"
#include "KStats.h"
#include "GCNetwork.h"
#include "Controls.h"
#include "KDInput.h"
#include "KSingleton.h"
#include "GCUtil/EnumString.h"
#include "Procedure.h"
#include "KSingleton.h"
#include "gcui/KGCNewTutorialOption.h"
#include "KGCChannelManager.h"
#include "KGCRoomManager.h"
#include "KGCWorldMapManager.h"
#include "GCIconImageManager.h"
#include "Square/GCSquare.h"
#include "BuddySystem/KFriendShip.h"
#include "KGCQALogManager.h"
#include "GCClientErrorManager.h"
#include "GCDeviceLib/TextureLoadManager.h"
#include "KSingleton.h"
#include "KncP2PLib/KncP2P.h"
#include "KGCRoomManager.h"


KGCStateMachine::KGCStateMachine( void )
{
    m_mapState[GS_INIT_LOADING]         = new KGCStateInitLoading;
    m_mapState[GS_LOGO]                 = new KGCStateLogo;
	m_mapState[GS_INTRO]                = new KGCStateIntro;
    m_mapState[GS_LOGIN]                = new KGCStateLogin;
    m_mapState[GS_GAME]                 = new KGCStateGame;
    m_mapState[GS_ROOM]                 = new KGCStateRoom;
    m_mapState[GS_GUILD]                = new KGCStateGuild;
    m_mapState[GS_SHOP_CASH]            = new KGCStateCashShop;
#ifndef DISABLE_MAP_SHOP // Kawan>
    m_mapState[GS_SHOP_GP]              = new KGCStateGPShop;
#endif
    m_mapState[GS_MY_INFO]              = new KGCStateMyInfo;
    m_mapState[GS_OPTION]               = new KGCStateOption;
    m_mapState[GS_MY_INFO_FROM_ROOM]    = new GCStateMyInfoFromRoom;
    m_mapState[GS_GAME_LOADING]         = new KGCStateLoading;
    m_mapState[GS_GAME_END]             = new KGCStateEndGame;
    m_mapState[GS_TUTORIAL]             = new KGCStateTutorial;
    m_mapState[GS_REPLAY]               = new KGCStateReplay;
    m_mapState[GS_SERVER]               = new KGCStateServer;
	m_mapState[GS_CHAR_SELECT]          = new KGCStateCharSelcet;
	m_mapState[GS_CHAR_GAIN]			= new KGCStateCharGain;
    m_mapState[GS_WORLDMAP]             = new KGCStateWorldMap;
    m_mapState[GS_MATCH]                = new KGCStateMatch;
    m_mapState[GS_SQUARE_LOADING]       = new KGCStateSquareLoading;
    m_mapState[GS_SQUARE]               = new KGCStateSquare;
    m_mapState[GS_SQUARE_END]           = new KGCStateSquareEnd;
    m_mapState[GS_EXITE_GAME]           = new KGCStateExitGame;

#if defined USE_COORDI_SHOP
    m_mapState[GS_COORDI_SHOP]          = new KGCStateCoordiShop;
#endif

	m_mapState[GS_CHAR_SELECT_FROM_ROOM] = new KGCStateCharSelcet;

    m_mapState[GS_REGISTER]              = new KGCStateRegister;

    ::InitializeCriticalSection( &m_csForGameState );

    std::map<DWORD, KGCState*>::iterator mit;
    for ( mit = m_mapState.begin(); mit != m_mapState.end(); mit++ )
    {
        mit->second->OnSetupFSM();
    }
    m_lSavedSate.clear();
    m_dwStayTime = timeGetTime();
    m_ePrevGameState = (GAME_STATE)-1;
    m_ePrevChattingState = (GAME_STATE)-1;

    SetGameState(GS_INIT_LOADING);
    SetCurrentState(GetState());
    g_MyD3D->m_bKeepFPS60 = FALSE;  //·Îµù°ú LOGO¿¡¼­´Â 55FrameÀ» ¸ÂÃâ ÇÊ¿ä°¡ ¾ø±â¿¡ Ã³À½¿¡´Â FALSE·Î ÇÑ´Ù.

    m_iCurrRoomNum = -1;
    m_bJoinedChannel = false;
    m_bJoinedGuild = false;

    // 2007/3/28. iridology. ÀÌºÎºÐ ¿Ö 
#ifndef  STATE_ENUM
#define STATE_ENUM( ID ) KSingleton<EnumString>::GetInstance()->SetEnumName( ID, L#ID );
    STATE_ENUM( GS_INIT_LOADING      )
    STATE_ENUM( GS_LOGO              )
    STATE_ENUM( GS_LOGIN             )
    STATE_ENUM( GS_GUILD             )
    STATE_ENUM( GS_SHOP_CASH         )  // Ä³½¬ »óÁ¡
    STATE_ENUM( GS_ROOM              )
    STATE_ENUM( GS_GAME              )
    STATE_ENUM( GS_MY_INFO           )
    STATE_ENUM( GS_OPTION            )
    STATE_ENUM( GS_MY_INFO_FROM_ROOM )
    STATE_ENUM( GS_GAME_LOADING      )
    STATE_ENUM( GS_GAME_END          )
    STATE_ENUM( GS_TUTORIAL          )
    STATE_ENUM( GS_REPLAY            )
    STATE_ENUM( GS_SERVER            )
	STATE_ENUM( GS_CHAR_SELECT       )
	STATE_ENUM( GS_CHAR_GAIN         )
	STATE_ENUM( GS_INTRO         )
    STATE_ENUM( GS_WORLDMAP          )
    STATE_ENUM( GS_MATCH             )
    STATE_ENUM( GS_SQUARE_LOADING    )
    STATE_ENUM( GS_SQUARE            )
    STATE_ENUM( GS_SQUARE_END        )
    STATE_ENUM( FAIL_N_SHUTDOWN      ) //½ºÅ×ÀÌÆ® º¯°æ ½ÇÆÐ½Ã Å¬¶óÀÌ¾ðÆ® Á¾·á°¡ ÇÊ¿äÇÒ ¶§...
    STATE_ENUM( GS_EXITE_GAME        ) //exit(0)/terminate()/WM_CLOSE µîÀ¸·Î °ÔÀÓÀÌ Á¾·á µÉ ¶§ 
    
#if defined USE_COORDI_SHOP
    STATE_ENUM( GS_COORDI_SHOP       ) //ÄÚµð¼¥
#endif
#ifndef DISABLE_MAP_SHOP // Kawan>
    STATE_ENUM( GS_SHOP_GP           ) // GP »óÁ¡(´ë·úº° »óÁ¡)
#endif
	STATE_ENUM( GS_CHAR_SELECT_FROM_ROOM )
    STATE_ENUM( GS_REGISTER          )
    STATE_ENUM( NUM_GAME_STATE       )
#undef  STATE_ENUM
#endif
}

KGCStateMachine::~KGCStateMachine( void )
{
    for( int i=0 ; i<NUM_GAME_STATE ; ++i )
    {
        SAFE_DELETE( m_mapState[i] );
    }
    m_mapState.clear();

    m_lSavedSate.clear();

    ::DeleteCriticalSection( &m_csForGameState );
}

GAME_STATE KGCStateMachine::GetState( void ) const
{
    return m_eGameState;
}

std::string KGCStateMachine::GetStateString()
{
    return GetStateString( m_eGameState );
}

std::string KGCStateMachine::GetStateString( GAME_STATE eState )
{
    std::string strRet = GCUTIL_STR::GCStrWideToChar( ENUM_STR_GETNAME( eState ).c_str() );
    return strRet;
}

bool KGCStateMachine::GoState( GAME_STATE eState_, bool bValidState_ )
{
    bool bRet = false;

	g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_PRE_ENTER_SCENE, eState_ ) );

	//ÁöÁ¤ÇÑ ½ºÅ×ÀÌÆ®°¡ ¸Ê¿¡ Á¸ÀçÇÏ´ÂÁö È®ÀÎÇÕ´Ï´Ù.
    if( m_mapState.find( eState_) != m_mapState.end() )
    {
        // Áö±Ý ÀÌµ¿ÇÏ·Á°í ÇÏ´Â State°¡ ÇöÀç State¿¡¼­ ÁøÇàÀÌ °¡´ÉÇÑ StateÀÎÁö¸¦ Ã¼Å© ÇÕ´Ï´Ù.
        // bValidState_ °¡ false ÀÌ¸é °­Á¦ ÁøÇàÀÇ »óÈ²ÀÌ±â ¶§¹®¿¡ °¥¼ö ÀÖ´Â State Ã¼Å©¸¦ ÇÏÁö ¾Ê½À´Ï´Ù.
        if( m_mapState[m_eGameState]->OnCheckNextState( eState_ ) || !bValidState_ )
        {
            //¼ÂÆÃµÉ ½ºÅ×ÀÌ½º·Î ¹Ù²î±â Àü ½ºÅ×ÀÌÆ® ÀÌµ¿¿¡ ´ëÇÑ °ËÁõÀÌ ½ÇÆÐÇÏ¸é ½ÇÆÐÇßÀ» ¶§ ÁøÇàÇØ¾ßÇÒ ½ºÅ×ÀÌÆ®·Î ÁøÇàÇÕ´Ï´Ù.
            if(  m_mapState[eState_]->OnInitBeforeStateChange( m_eGameState ) )
            {    
                bRet = true;
                // À§ÀÇ OnInitBeforeStateChange¿¡¼­ ÀÌµ¿¿¡ ´ëÇÑ °ËÁõÀÌ ³¡³µ±â ¶§¹®¿¡ ½ÇÁúÀûÀ¸·Î State ÀÌÀüÀ» ½Ç½Ã ÇÕ´Ï´Ù.
	
				//°ÔÀÓ ³¡³­ °æ¿ì.. ÇØ»óµµ º¹±Í
				/*if (eState_ != GS_GAME && ( GetState()==GS_GAME || GetState() == GS_GAME_LOADING ) &&
					g_MyD3D->m_KGCOption.GetCurrentMode() != g_MyD3D->m_KGCOption.GetBasicMode() )
				{
					g_MyD3D->SetResolution(FALSE);
				}*/


                SetCurrentState( eState_ );

                if( KP2P::GetInstance()->m_kMsgServer != NULL )
                    KP2P::GetInstance()->m_kMsgServer->UpdateLocation();
            }
            else
            {
                m_mapState[eState_]->OnDestroyBeforeStateChange();

                // ½ÇÆÐÇßÀ»¶§ °¡¾ßÇÒ State·ÎÀÇ ÀüÈ¯ ¸¶Àúµµ ½ÇÆÐ ÇÏ°Ô µÈ´Ù¸é °­Á¦·Î Æ¯Á¤ State·Î ÀüÈ¯.
                // °­Á¦·Î ÀüÈ¯µÉ State´Â °¡Àå ¾ÈÀüÇÑ State¿©¾ß ÇÕ´Ï´Ù.
                if ( bValidState_ == false )
                {
                    int iSafeState = GetSafetyState( eState_ );

                    //State ÀüÈ¯ Áß ½ÇÆÐ¿¡ ÀÇÇÑ Á¾·á ¸Þ½ÃÁö´Â °¢ ½ºÅ×ÀÌÆ®ÀÇ ½ÇÆÐ »çÀ¯¿¡ µû¶ó ´Þ¶óÁö±â ¶§¹®¿¡ ¿©±â¼­´Â ±×³É Á¾·á¸¸ ÇØ ÁÖ°Ú½À´Ï´Ù.
                    if( iSafeState == FAIL_N_SHUTDOWN )
                        PostMessage( g_MyD3D->m_hWnd, WM_CLOSE, 0, 0 );
                    else
                        bRet = JumpToSafeState( eState_ );
                    
                }
                else
                {
                    GAME_STATE eStateForFail = m_mapState[m_eGameState]->GetStateForFail( eState_ );

                    if( eStateForFail != FAIL_N_SHUTDOWN )
						GoState( eStateForFail, false );

                    // ½ÇÆÐÇØ¼­ ´Ù¸¥ State·Î ÀÌµ¿Çß±â ¶§¹®¿¡ false
                    bRet = false;
                }
            }
        }
    }

    return bRet;
}

bool KGCStateMachine::JumpToSafeState( GAME_STATE eState_ )
{
    int iSafeState = GetSafetyState( eState_ );
    bool bRet = false;
    //¿©±â¼­ °­Á¦ ÀüÈ¯¿¡ ´ëÇÑ »ç½ÇÀ» ¼­¹ö¿¡ ¾Ë·ÁÁà¾ß ÇÕ´Ï´Ù.
    Result_ForceTransition = INT_MAX;
    KP2P::GetInstance()->Send_ForceTransitionState( iSafeState );
    g_MyD3D->WaitForServerAck( Result_ForceTransition, INT_MAX, 5000, TIME_OUT_VALUE );

    // °­Á¦ÀüÈ¯Àº ¼­¹ö¿¡¼­ Çã°¡ ÇßÀ» ¶§ ¼öÇàÇØ¾ßÇÏ°í, ¼­¹ö¿¡¼­ false¸¦ ¹ÝÈ¯ÇÏ¸é Å¬¶óÀÌ¾ðÆ®¸¦ Àç½ÃÀÛ ÇÒ ¼ö ÀÖµµ·Ï À¯µµÇÕ´Ï´Ù.
    if( Result_ForceTransition == 0 )
    {
        m_mapState[eState_]->OnInitBeforeStateChange( m_eGameState );
        SetCurrentState( eState_ );

        //°­Á¦·Î¶óµµ ÀüÈ¯¿¡ ¼º°øÇß±â ¶§¹®¿¡
        bRet = true;
    }
    else if( Result_ForceTransition == TIME_OUT_VALUE )
    {
        //Áö¿¬À¸·Î ÀÎÇÑ ½ÇÆÐ´Â Æ¯¼öÇÑ »óÈ²ÀÔ´Ï´Ù.
        SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_STATE_DELAY );
#if !defined(RETRY_LOGIN_ON_CLOSE_CONNECTION)
        PostMessage( g_MyD3D->m_hWnd, WM_CLOSE, 0, 0 );
#endif
    }
    else
    {
        //¼­¹ö¿¡¼­ ½ÇÆÐ¸¦ ¸®ÅÏÇß½À´Ï´Ù. Å¬¶óÀÌ¾ðÆ®´Â ¾Ë¾Æ¼­ Á×¾îÁÝ½Ã´Ù.
		PostMessage( g_MyD3D->m_hWnd, WM_CLOSE, 0, 0 );
    }
    return bRet;
}

//¿ä±âºÎÅÍ///////////////////////////////////////////////////////////
// DATE: 07-03-13    milennium9@kogstudios.com
/////////////////////////////////////////////////////////////////////
// Go ÇÔ¼öµé¿¡ ÃÊ±âÈ­ °ü·Ã ¼ÂÆÃÀ» ¸ðµÎ °¢ ½ºÅ×ÀÌÆ® Å¬·¡½º³»ÀÇ Init°ú 
// InitBeforeChange ÇÔ¼ö·Î ÀÌµ¿Çß´Ù. 
//  ÀÌÈÄ ºÎÅÍ´Â KGCState Å¬·¡½º¿¡¼­ Á¦°øÇÏÁö ¾Ê´Â ¼ÂÆÃÀÌ ÇÊ¿äÇÒ ¶§¸¸ 
// Go ÇÔ¼öµéÀ» ÀÌ¿ëÇØ¼­ ÃÊ±âÈ­ ÇØ¾ß ÇÑ´Ù.
/////////////////////////////////////////////////////////////////////
bool KGCStateMachine::GoLogo( void )
{
    return GoState( GS_LOGO );
}

bool KGCStateMachine::GoLogin( void )
{
    return GoState( GS_LOGIN );
}

bool KGCStateMachine::GoRegister( void )
{
    return GoState( GS_REGISTER );
}

bool KGCStateMachine::GoGuild( void )
{
    if( m_eGameState == GS_GUILD )
        return true;

    return GoState( GS_GUILD );
}

bool KGCStateMachine::GoCashShop( void )
{
    return GoState( GS_SHOP_CASH );
}
#ifndef DISABLE_MAP_SHOP // Kawan>
bool KGCStateMachine::GoGPShop( void )
{
    return GoState( GS_SHOP_GP );
}
#endif
bool KGCStateMachine::GoMyInfo( void )
{   
    return GoState( GS_MY_INFO );
}

bool KGCStateMachine::GoMyInfoFromRoom( void )
{   
    return GoState( GS_MY_INFO_FROM_ROOM );
}

bool KGCStateMachine::GoCharSelectFromRoom( void )
{
	return GoState( GS_CHAR_SELECT_FROM_ROOM );
}

bool KGCStateMachine::GoRoom( int iRoomNum, const std::wstring& strRoomName, const std::wstring& strRoomPassword )
{
    m_iCurrRoomNum = iRoomNum;
    ((KGCStateRoom*)m_mapState[GS_ROOM])->SetRoomInfo( iRoomNum, strRoomName, strRoomPassword );
    return GoState( GS_ROOM );
}

bool KGCStateMachine::GoRoom( void )
{
	g_kGlobalValue.m_kUserMission.RefreshMissionItemCount();
    return  GoState( GS_ROOM );
}

bool KGCStateMachine::GoGame( void )
{
    //Á» ÀÌ»óÇÏÁö¸¸ ~ ¾î¶² °ÔÀÓÀÌµç °ÔÀÓ ½ÃÀÛ½Ã¸¶´Ù ÃÊ±âÈ­ ½ÃÄÑÁÖ´Â º¯¼ö -¤µ-;
    g_MyD3D->bEverSpecialCheck = false;

	if (g_pkGameOverlayUI->m_pkAddtionGuide)
	{
		if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
		{
			if (SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST0 ) 
			{
				g_pkGameOverlayUI->m_pkAddtionGuide->SetPassTutorial(false);
				g_pkGameOverlayUI->ShowAddtionGuide(318);
			}
			else
			{
				g_pkGameOverlayUI->m_pkAddtionGuide->ToggleRender(false);
			}
// 			if (SiKGCWorldMapManager()->GetGameMode() == GC_GM_QUEST0)
// 				g_pkGameOverlayUI->m_pkAddtionGuide->SetPassTutorial(false);
// 			else
// 				g_pkGameOverlayUI->m_pkAddtionGuide->SetPassTutorial(true);
// 			g_pkGameOverlayUI->ShowAddtionGuide(318);
		}
	}
    return GoState( GS_GAME );
}

bool KGCStateMachine::GoLoading( void )
{
    return GoState( GS_GAME_LOADING );
}

bool KGCStateMachine::GoTutorial( int iChar_ )
{
    ((KGCStateTutorial*)m_mapState[GS_TUTORIAL])->SetCharacter( 0 );
    if (iChar_ != -1)
    {
        //((KGCStateTutorial*)m_mapState[GS_TUTORIAL])->SetCharacter( iChar_ );
        //g_pkUIScene->m_pkTutorialOption->SetMyCharIndex( iChar_ );
        //g_pkUIScene->m_pkTutorialOption->SetTutorialCharIndex( iChar_ );
    }

	//°ÔÀÓ ½ÃÀÛÇÏ¶ó°í ÇÏ¸é.. ÇØ»óµµ ºÎÅÍ Å°¿î´Ù.
	/*if ( g_MyD3D->m_KGCOption.GetCurrentMode() != g_MyD3D->m_KGCOption.GetBasicMode() )
	{
		g_MyD3D->SetResolution(TRUE);
	}*/

    return GoState( GS_TUTORIAL );
}

bool KGCStateMachine::GoReplay()
{
    return GoState( GS_REPLAY );
}

bool KGCStateMachine::GoServer()
{
    g_KMci.Play( GC_BGM_MAIN_THEME, false );
    return GoState( GS_SERVER );
}

bool KGCStateMachine::GoCharSelect()
{
	return GoState( GS_CHAR_SELECT );
}

bool KGCStateMachine::GoMatch( DWORD dwChannelUID )
{
    ( (KGCStateMatch*)m_mapState[GS_MATCH])->SetChannelUID( dwChannelUID );

    return GoState( GS_MATCH );
}

bool KGCStateMachine::GoWorldMap()
{
	g_kGlobalValue.m_kUserMission.RefreshMissionItemCount();
	if (g_pkGameOverlayUI->m_pkAddtionGuide)
		g_pkGameOverlayUI->m_pkAddtionGuide->IinitAtGoWorldMap();
    return GoState( GS_WORLDMAP );
}

bool KGCStateMachine::GoPrev( void )
{
    if( m_lSavedSate.empty() )
        return false;

    std::list<GAME_STATE>::iterator itor = m_lSavedSate.begin();
    GAME_STATE eSavedState = *itor;
    m_lSavedSate.erase( itor );

    SetCurrentState(eSavedState);
    return true;
}

HRESULT KGCStateMachine::FrameMove( float fElapsedTime)
{
    g_MyD3D->MyCtrl->Into_Packet();
	
	static DWORD dwMsnConnectTime = ::timeGetTime();

    if (dwMsnConnectTime + 60000 <= ::timeGetTime() && !SiKFriendShip()->IsEmptyFriendShipData()) {
        if (GS_TUTORIAL != m_eGameState &&
            GS_GAME != m_eGameState &&
            GS_GAME_LOADING != m_eGameState &&
            GS_GAME_END != m_eGameState)
        {
            if (KP2P::GetInstance()->m_kMsgServer)
                KP2P::GetInstance()->m_kMsgServer->ConnectToMsgServerAgain();
        }
        dwMsnConnectTime = ::timeGetTime();
    }

	//¹é±×¶ó¿îµå ´Ù¿î·Îµå¿¡¼­ ´Ù¿î·Îµå ¿Ï·áµÈ ÆÄÀÏÀÌ ÀÖÀ¸¸é ·Îµå ÇØÁØ´Ù.
    //FileDownloadManager::EDOWN_DATA_TYPE etype;
    //std::wstring strFileName;
    //if (g_MyD3D->m_fileDownloadManager.GetCompleteFileData( etype, strFileName ))
    //{
    //    if ( etype == FileDownloadManager::EDOWN_GUILD_MARK )
    //        SiKGCIconImageManager()->InsertImageIcon(strFileName,GUILD_MARK_FOLDER+strFileName);
    //    else if ( etype == FileDownloadManager::EDOWN_CHANNEL_IMG )
    //    {
    //        g_pGCDeviceManager2->CreateTexture(GCUTIL_STR::GCStrWideToChar( (CHANNEL_IMG_FOLDER+strFileName).c_str() ) ,false , true);
    //    }
    //}

    //µ¿¿µ»ó Ä¸Ã³ ÄÚµå ´ÜÃàÅ°
#if defined(AVI_RECODER)
    bool bToggleVideoCaptureStart   =   ( diks[DIK_LCONTROL] || diks[DIK_RCONTROL] ) && diks[DIK_2] && !Old_diks[DIK_2];
    bool bToggleVideoCaptureEnd     =   ( diks[DIK_LCONTROL] || diks[DIK_RCONTROL] ) && diks[DIK_3] && !Old_diks[DIK_3];
    BOOL bIsCapturing               =   g_kGlobalValue.m_bandiCaptureLibrary.IsCapturing();
    bool bStartCapture = false;

    if( bToggleVideoCaptureStart && !bIsCapturing ){ bStartCapture = true; }    // Ä¸Ã³ ÁßÀÌ ¾Æ´Ò¶§¸¸ Ä¸Ã³ÀÔ·ÂÀÌ ¿ÔÀ»¶§ Åä±ÛÇØ¾ß ÇÑ´Ù.
    if( bToggleVideoCaptureEnd && bIsCapturing ){ bStartCapture = true; }       // Ä¸Ã³ÁßÀÏ¶§¸¸ Ä¸Ã³ÁßÁöÀÔ·ÂÀÌ ¿ÔÀ»¶§ Åä±ÛÇØ¾ß ÇÑ´Ù.

    if( bStartCapture ){
        g_kGlobalValue.ToggleVideoCapture();
    }
#endif

    CheckPing();

    KGCPC_BEGIN("LuaGarbageCollect");
    KGCLuabinder::getInstance().CollectGarbage( LUA_GCSTEP, 7 );
    KGCPC_END();
    
    if ( g_kGlobalValue.m_bShutDown ) 
    {
        if ( IsEnableExitClient() )
        {            
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SHUTDOWN, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 5, g_kGlobalValue.m_iShutDownType, true, true );
            
            g_kGlobalValue.m_bShutDown = false;
        }
    }

#ifdef FEATURE_2021
    SiGCClientErrorManager()->FrameMove( fElapsedTime );
#endif
    //SiKTextureLoadManager()->FrameMove();

    return m_pCurrState->FrameMove( fElapsedTime );
}

HRESULT KGCStateMachine::Render( void )
{

    HRESULT hr = m_pCurrState->Render();
    
    if( BeginScene() )
    {

	    if(m_eGameState != GS_GAME_LOADING && m_eGameState!= GS_INIT_LOADING)
        {
		    g_MyD3D->MyCtrl->Mouse_Render();
        }

	    g_pGCDeviceManager2->Flush();

#ifndef __PATH__    
	    g_kStatsManager.Render();
	    g_pGCDeviceManager2->Flush();
#endif;

        EndScene();
    }

    return hr;
}

void KGCStateMachine::OnExitGame()
{
    g_pStateGame->OnExitGame();
    if( KP2P::GetInstance()->m_kMsgServer != NULL )
		KP2P::GetInstance()->m_kMsgServer->UpdateLocation();
}

void KGCStateMachine::ExitRoom( bool bSendToServer, UINT uiError_, bool bSubtractRP, bool bStateChange )
{
    g_kGlobalValue.m_kUserInfo.bHost = false;
    if ( bSendToServer )
    {
        KEVENT_LEAVE_ROOM_REQ kPacket;
        kPacket.m_iP2PError = uiError_;
        // ¿î¿µÀÚ Ä³¸¯Àº RP±ðÁö ¸»°Í
        if( g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN )
            bSubtractRP = false;
        kPacket.m_bSubtractRP = bSubtractRP;
        //====================================================================================
        // 2006.10.02 : Asirion
        // Sync·Î ¹Ù²Ù¾ú´Ù. 
        Result_LeaveRoom = INT_MAX;

        kPacket.m_bGuild = SiKGCRoomManager()->IsChannelRoomList();
        KP2P::GetInstance()->Send_LeaveRoomReq( kPacket );
        g_MyD3D->WaitForServerAck( Result_LeaveRoom, INT_MAX, 10000, 1 );
        switch( Result_LeaveRoom )
        {
            case 1:
            {
                //====================================================================================
                // 2006.10.02 : Asirion
                // Server¿¡¼­ ÀÀ´äÀÌ ¾ø´Â°ÍÀÌ´Ù.
                g_MyD3D->ExitWithMsgDlg( STR_ID_APPMSG_ERR_DISCONNECTED1, STR_ID_APPMSG_ERR_DISCONNECTED2 );
                return;
            }
            default:
            {
                break;
            }
        }
    }

    std::vector<DWORD> vecPeer = KSingleton<KncP2P>::GetInstance()->GetPeerUID();
    for( int i = 0; i < (int) vecPeer.size(); ++i ) 
    {
        if( vecPeer[i] == g_kGlobalValue.m_kUserInfo.dwUID ) 
            continue;

        DWORD dwPing = KSingleton<KncP2P>::GetInstance()->GetAvgPingTime( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID );
        bool bRelay = KSingleton<KncP2P>::GetInstance()->IsRelay( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID );
        SiGCClientErrorManager()->Send_PingTimeReport( bRelay, dwPing ); 
    }


    KP2P::GetInstance()->DisConnectTCPRelay();

    ShutDown( false );

	// ¹æ³ª¿Íµµ Ä«Å×°í¸® ¼³Á¤ ±×´ë·Î º¸À¯ÇÏ´Â ¹®Á¦ ÀÎÇÏ¿© ¹ß»ýµÇ´Â ¹®Á¦ ¿¹¿Ü Ã³¸®
	// ÀÌ·± ±¸Á¶°¡ ¾ÈµÇµµ·Ï º¯°æÀÌ ÇÊ¿äÇÔ.
	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_CAPTAIN )
	{
		SiKGCRoomManager()->SetGameModeCategory( GC_GMC_MATCH );
	}

    if( bStateChange == false )
        return;

    //¼±º°ÀûÀ¸·Î ÀÌÀü»óÅÂ·Î µ¹·ÁÁà¾ßÇÔ..
    switch( m_ePrevRoomState )
    {
        case GS_GUILD:
        {
            GoGuild();
            break;
        }
        case GS_MATCH:
        case GS_GAME_LOADING:           //µ¥½º¸ÅÄ¡¿¡¼­ °ÔÀÓ ·ÎµùÁß¿¡ °ÔÀÓÀÌ ³¡³¯ °æ¿ì¿¡ ÀÌ¸® µé¾î¿Ã ¼ö ÀÖ´Ù
        {
            GoMatch();
            break;
        }
        case GS_WORLDMAP:
        {
            GoWorldMap();
            break;
        }
        case GS_SQUARE:
        {
            SetJoinChannel( true );
            g_MyD3D->m_pStateMachine->GoSquareLoading();
            break;
        }
        case GS_SHOP_CASH:
#ifndef DISABLE_MAP_SHOP // Kawan>
        case GS_SHOP_GP:
#endif
        case GS_MY_INFO:
        
#if defined USE_COORDI_SHOP
        case GS_COORDI_SHOP:
#endif
        
        {
            switch( SiKGCChannelManager()->GetCurrentChannelType() )
            {
            case CT_DUNGEON:
                {
                    GoWorldMap();
                    break;
                }
            case CT_GUILD:
                {
                    GoGuild();
                    break;
                }
            default:
                {
                    GoMatch();
                    break;
                }
            }
            break;
        }
        default:
        {
            GoServer();
            break;
        }
    }

    for ( int i = 0 ; i < MAX_PLAYER_NUM; i++ )
    {
        g_MyD3D->MyPlayer[i]->m_LoadingState.Init();
    }

    return;
}


void KGCStateMachine::SetCurrentState( GAME_STATE eGameState )
{
    m_mapState[m_eGameState]->OnDestroy();

    // ½ÇÆÐ ÇßÀ»¶§¸¦ À§ÇÔ..
    GAME_STATE etempState = m_eGameState;

    // 2007/3/30. iridology. ÀÏ´Ü ¼­¹ö¿¡¼­ ¹®Á¦ ¾ø°Ô ÇØµÒ. ÀÚ¹æÇÏ¼À
    // ¹æÀ¸·Î µé¾î°£´Ù¸é.. ¹æÀ¸·Î µé¾î°¡±â Àü »óÅÂ¸¦ ÀúÀåÇÑ´Ù.
    // ´Ü °ÔÀÓ¿¡¼­ ´Ù½Ã ¹æÀ¸·Î °¡¸é ÀúÀåÇÏÁö ¾Ê´Â´Ù.
    if( ( m_eGameState != GS_GAME &&
          m_eGameState != GS_GAME_END &&
          m_eGameState != GS_MY_INFO_FROM_ROOM ) && 
		  m_eGameState != GS_CHAR_SELECT_FROM_ROOM &&
          eGameState == GS_ROOM )
    {
        // ¹æ¿¡ µé¾î¿À±â ÀÌÀüÀÇ ½ºÅ×ÀÌÆ®¸¦ ÀúÀåÇÑ´Ù
        // °ÔÀÓ -> ¹æ
        // °ÔÀÓ¿£µå -> ¹æ
        // ¹æÀÚ±âÁ¤º¸ -> ¹æ ¼¼ °æ¿ì´Â ÀúÀåÇÏÁö ¾Ê´Â´Ù!
        m_ePrevRoomState = m_eGameState;
    }

    //ÀÌÀü »óÅÂ¸¦ ÀúÀåÇØ µÓ´Ï´Ù. ÀÌº¯¼ö´Â ÂüÁ¶¸¸ ÇÏ°í »ç¿ëÇÏ¸é ¾ÈµË´Ï´Ù.
    m_ePrevGameState = m_eGameState;

    // Ã¤ÆÃ¹Ù°¡ ÀÖ´ø ÀÌÀü »óÅÂ¸¦ ÀúÀåÇØµÐ´Ù ( ·Îºñ³ª, ±æµå¸¸ ÇÑÇØ¼­ )
#if defined USE_COORDI_SHOP
    if( m_ePrevGameState != GS_SHOP_CASH && m_ePrevGameState != GS_SHOP_GP && m_ePrevGameState != GS_MY_INFO && m_ePrevGameState != GS_COORDI_SHOP && 
        ( m_ePrevGameState == GS_MATCH || m_ePrevGameState == GS_GUILD ) )
#else
#ifndef DISABLE_MAP_SHOP // Kawan>
    if( m_ePrevGameState != GS_SHOP_CASH && m_ePrevGameState != GS_SHOP_GP && m_ePrevGameState != GS_MY_INFO && 
#else
    if (m_ePrevGameState != GS_SHOP_CASH && m_ePrevGameState != GS_MY_INFO &&
#endif
        ( m_ePrevGameState == GS_MATCH || m_ePrevGameState == GS_GUILD ) )
#endif
    {
        m_ePrevChattingState = m_ePrevGameState;
    }

    //¸®½ºÆ®¿¡ ÇöÀç »óÅÂ°¡ µé¾îÀÖÀ¸¸é »èÁ¦ÇÏ¶ó
    std::list<GAME_STATE>::iterator itor = m_lSavedSate.begin();
    for(itor ; itor != m_lSavedSate.end() ; itor++)
    {
        if( *itor == eGameState )
        {
            m_lSavedSate.erase( itor );
            break;
        }
    }

    SetGameState(eGameState);
    m_pCurrState = m_mapState[m_eGameState];

	// À¯¾ÆÀÌ ÅØ½ºÃÄ Ä³½Ã Á¤¸®
	g_pkUIMgr->ClearUnusedTexture();

    m_pCurrState->OnInit();
	//g_MyD3D->m_kGuideMgr.SetCurrentState( m_eGameState );

    //ÇØ´ç ½ºÅ×ÀÌÆ®¿¡ ¸Â´Â »ç¿îµå¸¦ ·ÎµùÇØ º¾½Ã´Ù.
    g_KDSound.LoadFromPositionList( m_eGameState , SiKGCRoomManager()->GetGameMode() );

    // À¯ÀúÀÇ ÇàÀûÀ» ±â·ÏÇÕ´Ï´Ù.
    SaveUserPath();
}

std::string KGCStateMachine::GetCurrentLocationIcon()
{
    std::stringstream strReturn;

    int iNowState = GetState();

    switch (iNowState)
    {
        default:
        case GS_LOGO:
        case GS_LOGIN:
        case GS_REGISTER:
        case GS_CHAR_SELECT:
        case GS_SERVER:
        case GS_MY_INFO:
        case GS_OPTION:
        case GS_SHOP_CASH:
#ifndef DISABLE_MAP_SHOP // Kawan>
        case GS_SHOP_GP:
#endif
#ifdef USE_COORDI_SHOP
        case GS_COORDI_SHOP:
#endif
        {
            strReturn.str("");
            strReturn << "origin_start";
        }
        break;
        case GS_ROOM:
        case GS_GAME_END:
        case GS_GAME_LOADING:
        case GS_MY_INFO_FROM_ROOM:
        case GS_CHAR_SELECT_FROM_ROOM:
        {
            strReturn.str("");
            if (SiKGCWorldMapManager()->IsInfinityDungeon())
            {
                strReturn << "dungeon_infinity";
            }
            else
            {
                if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
                {
                    strReturn << "dungeon_" << SiKGCRoomManager()->GetGameMode();
                }
                else
                {
                    if (SiKGCAgitGameManager()->IsAgitEntered())
                    {
                        strReturn << "residencia";
                    }
                    else
                    {
                        strReturn << "pvp_base";
                    }
                }
            }
        }
        break;
        case GS_MATCH:
        {
            strReturn.str("");
            strReturn << "pvp_base";
        }
        break;
        case GS_WORLDMAP:
        {
            int iContinentStrID = SiKGCWorldMapManager()->GetRegionStringNumber(SiKGCWorldMapManager()->GetCurrentWorldMap());
            strReturn.str("");
            strReturn << "continente_" << iContinentStrID;
        }
        break;
        case GS_SQUARE_LOADING:
        case GS_SQUARE:
        {
            strReturn.str("");
            strReturn << "plaza";
        }
        break;
        case GS_GAME:
        {
            if (SiKGCWorldMapManager()->IsInfinityDungeon())
            {
                strReturn.str("");
                strReturn << "dungeon_infinity";
            }
            else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
            {
                strReturn.str("");
                strReturn << "dungeon_" << SiKGCRoomManager()->GetGameMode();
            }
            else if (SiKGCAgitGameManager()->IsAgitEntered())
            {
                strReturn.str("");
                strReturn << "residencia";
            }
            else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL)
            {
                strReturn.str("");
                strReturn << "origin_start";
            }
            else
            {
                strReturn.str("");
                strReturn << "pvp_base";
            }
        }
        break;
    }
    return strReturn.str();
}

std::wstring KGCStateMachine::GetCurrentLocation()
{
    std::wstring strReturn;
    WCHAR strTemp[MAX_PATH];

    int iNowState = GetState();

    switch( iNowState )
    {
        default:
        case GS_LOGO:
        case GS_LOGIN:
        case GS_REGISTER:
        {
            strReturn = g_pkStrLoader->GetString(STR_ID_MESSENGER_USER_LOCATION1);
        }
        break;
        case GS_GUILD:
        {
            strReturn = g_pkStrLoader->GetString(STR_ID_MESSENGER_USER_LOCATION2);
        }
        break;
        case GS_GAME_LOADING:
        {
            strReturn = g_pkStrLoader->GetString(STR_ID_MESSENGER_USER_LOCATION4);
        }
        break;
        case GS_SHOP_CASH:
    #ifndef DISABLE_MAP_SHOP // Kawan>
	    case GS_SHOP_GP:
    #endif
    #ifdef USE_COORDI_SHOP
	    case GS_COORDI_SHOP:
    #endif
        {
            strReturn = g_pkStrLoader->GetString(STR_ID_MESSENGER_USER_LOCATION9);
        }
        break;
	    case GS_SERVER:
        {
            strReturn = g_pkStrLoader->GetString(STR_ID_MESSENGER_USER_LOCATION5);
        }
	    break;
        case GS_ROOM:
        case GS_GAME_END:
	    {
		    if ( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON )
		    {
			    swprintf( strTemp, MAX_PATH, L"%s [%s%s%s]", SiKGCRoomManager()->GetGameModeName(SiKGCRoomManager()->GetGameMode()).data(), (SiKGCRoomManager()->GetRoomInfo().m_bDisablePetAttackPvP == false ? "😸 " : "😿 "), (SiKGCRoomManager()->GetRoomInfo().m_bEnablePvPBallance == false ? "" : "⚖️ "), (SiKGCRoomManager()->GetRoomInfo().m_bDisableSP4SkillsPvP == false ? "✨" : ""));
		    }
		    else
		    {
			    if (SiKGCWorldMapManager()->IsInfinityDungeon())
			    {
				    int iContinentStrID = SiKGCWorldMapManager()->GetRegionStringNumber(SiKGCWorldMapManager()->GetCurrentWorldMap());
                    swprintf(strTemp, MAX_PATH, L"%s", g_pkStrLoader->GetString(iContinentStrID).data());
			    }
			    else
			    {
                    if ( SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST52 )
                    {
                        std::wstringstream towerString;
                        towerString << SiKGCRoomManager()->GetRoomInfo().nDungeonLevel + 1;
                        swprintf(strTemp, MAX_PATH, L"%s [%s]", SiKGCWorldMapManager()->GetCurrentDungeonName().c_str(), towerString.str().c_str());
                    }
                    else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL)
                    {
                        swprintf(strTemp, MAX_PATH, L"%s", g_pkStrLoader->GetString(STR_ID_MESSENGER_USER_LOCATION13));
                    }
                    else
                    {
                        swprintf(strTemp, MAX_PATH, L"%s [%s]", SiKGCWorldMapManager()->GetCurrentDungeonName().c_str(), g_pkStrLoader->GetString(SiKGCWorldMapManager()->GetBasicLevelString(SiKGCRoomManager()->GetRoomInfo().nDungeonLevel)).data());
                    }
			    }
		    }
		    strReturn = strTemp;
	    }
        break;
        case GS_MY_INFO_FROM_ROOM:
        case GS_MY_INFO:
        {
            strReturn = g_pkStrLoader->GetString(STR_ID_MESSENGER_USER_LOCATION11);
        }
        break;
        case GS_WORLDMAP:
        {
		    int iContinentStrID = SiKGCWorldMapManager()->GetRegionStringNumber(SiKGCWorldMapManager()->GetCurrentWorldMap());

		    std::wstring strWorldMap = g_pkStrLoader->GetString(STR_ID_WORLDMAP);

		    if (iContinentStrID == 0)
		    {
			    strReturn = strWorldMap;
		    }
		    else
		    {
			    std::wstring  strContinent  = g_pkStrLoader->GetString(iContinentStrID);
			    GCUTIL_STR::Trim(strContinent);

			    swprintf(strTemp, MAX_PATH, L"%s [%s]", strWorldMap.c_str(), strContinent.c_str());
			    strReturn = strTemp;
		    }
	    }
           break;
        case GS_SQUARE:
        {
            KSquareInfo& kInfo = SiGCSquare()->GetCurrentSquareInfo();
            strReturn = kInfo.m_strName;
        }
        break;
        case GS_GAME:
        {
            if (GetPrevGameState() == GS_GUILD)
            {
                swprintf(strTemp, MAX_PATH, L"%s -%s%s%s", SiKGCRoomManager()->GetGameModeName(SiKGCRoomManager()->GetGameMode()).data(), (SiKGCRoomManager()->GetRoomInfo().m_bDisablePetAttackPvP == false ? L"😸" : L"😿"), (SiKGCRoomManager()->GetRoomInfo().m_bEnablePvPBallance == false ? L"" : L"⚖️"), (SiKGCRoomManager()->GetRoomInfo().m_bDisableSP4SkillsPvP == false ? L"✨" : L""));
            }
            else
            {
                if (SiKGCAgitGameManager()->IsAgitEntered())
                {
                    std::wstring strAgitName = SiKGCAgitGameManager()->GetAgitName();
                    if (strAgitName == L"")
                    {
                        swprintf(strTemp, MAX_PATH, L"%s", g_pkStrLoader->GetString(STR_ID_AGIT_USER_MENU02).c_str());
                    }
                    else
                    {
                        swprintf(strTemp, MAX_PATH, L"%s", strAgitName.c_str());
                    }
                }
                else if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON)
                {
                    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL)
                    {
                        swprintf(strTemp, MAX_PATH, L"%s", g_pkStrLoader->GetString(STR_ID_MESSENGER_USER_LOCATION13));
                    }
                    else
                    {
                        swprintf(strTemp, MAX_PATH, L"%s", SiKGCChannelManager()->GetCurrentChannelName().c_str());
                    }
                }
                else
                {
                    if (SiKGCWorldMapManager()->IsInfinityDungeon())
                    {
                        int iContinentStrID = SiKGCWorldMapManager()->GetRegionStringNumber(SiKGCWorldMapManager()->GetCurrentWorldMap());
                        swprintf(strTemp, MAX_PATH, L"%s", g_pkStrLoader->GetString(iContinentStrID).data());
                    }
                    else
                    {
                        if (SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST52)
                        {
                            std::wstringstream towerStringIn;
                            towerStringIn << SiKGCRoomManager()->GetRoomInfo().nDungeonLevel + 1;
                            swprintf(strTemp, MAX_PATH, L"%s [%s]", SiKGCWorldMapManager()->GetCurrentDungeonName().c_str(), towerStringIn.str().c_str());
                        }
                        else
                        {
                            swprintf(strTemp, MAX_PATH, L"%s [%s]", SiKGCWorldMapManager()->GetCurrentDungeonName().c_str(), g_pkStrLoader->GetString(SiKGCWorldMapManager()->GetBasicLevelString(SiKGCRoomManager()->GetRoomInfo().nDungeonLevel)).data());
                        }
                    }
                }
            }
            strReturn = strTemp;
        }
        break;
        case GS_MATCH:
        {
            strReturn = SiKGCChannelManager()->GetCurrentChannelName();
        }
        break;
        case GS_OPTION:
        case GS_CHAR_SELECT:
        case GS_CHAR_SELECT_FROM_ROOM:
            strReturn = L"";
        break;
    }   
    return strReturn;
}

void KGCStateMachine::SaveUserPath()
{
    switch( m_eGameState )
    {
    case GS_OPTION:
    case GS_INIT_LOADING:
    case GS_LOGO:
    case GS_LOGIN:
    case GS_REGISTER:
    case GS_REPLAY:
    case GS_GAME_LOADING:
    case GS_GAME_END:
    case GS_SQUARE_LOADING:
    case GS_SQUARE_END:
        return;
    }

    if( m_vecSaveUserPath.size() > 200 )
        return;

    DWORD dwStayEnd = timeGetTime();
    std::pair<char, unsigned short> pairUserPath;
    pairUserPath.first = (char) m_eGameState;
    pairUserPath.second = (unsigned short)(( dwStayEnd - m_dwStayTime ) / 60000);    //ºÐ´ÜÀ§·Î ±â·ÏÇÕ´Ï´Ù.
    m_dwStayTime = dwStayEnd;
    m_vecSaveUserPath.push_back( pairUserPath );
}

ESafeServerState KGCStateMachine::GetSafetyState( GAME_STATE eState_ )
{
    switch( eState_ )
    {
        case GS_INIT_LOADING:
        case GS_LOGO:
        case GS_LOGIN:
        case GS_REGISTER:
        case GS_OPTION:
        case GS_SERVER:
            return (ESafeServerState)SSS_CHANNEL_LOBBY;


		case GS_CHAR_SELECT:
		case GS_CHAR_GAIN:
		case GS_INTRO:
        case GS_GUILD:
        case GS_SHOP_CASH:
#ifndef DISABLE_MAP_SHOP // Kawan>
        case GS_SHOP_GP:
#endif
        case GS_GAME:
        case GS_MY_INFO:
        case GS_GAME_LOADING:
        case GS_TUTORIAL:
        case GS_REPLAY:
        case GS_SQUARE:
        case GS_SQUARE_LOADING:
        case GS_SQUARE_END:
            return SSS_CHANNEL_LOBBY;

        case GS_GAME_END:
            //return SSS_ROOM;

        case GS_MY_INFO_FROM_ROOM:
        case GS_ROOM:
		case GS_CHAR_SELECT_FROM_ROOM:
            return SSS_CHANNEL_LOBBY;

        

        case FAIL_N_SHUTDOWN:
            return (ESafeServerState)FAIL_N_SHUTDOWN;
        default:
            return SSS_CHANNEL_LOBBY;
    }
}

std::string KGCStateMachine::Dump()
{
    std::vector< std::pair<char, unsigned short> >& vecUserPath = GetUserHistory();
    std::stringstream stm;
    stm << "[User History]" << std::endl;

	std::vector< std::pair<char, unsigned short> >::iterator vit;
    for( vit = vecUserPath.begin() ; vit != vecUserPath.end() ; vit++ )
    {
        stm << GetStateString( (GAME_STATE)(vit->first) ) << std::endl;
    }
    stm << std::endl;

    return stm.str();
}

bool KGCStateMachine::GoSquareLoading( void )
{
    return GoState( GS_SQUARE_LOADING );
}

bool KGCStateMachine::GoSquare( void )
{
    return GoState( GS_SQUARE );
}

void KGCStateMachine::BindToLua()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KGCStateMachine>( L, "KGCStateMachine" );
    lua_tinker::class_def<KGCStateMachine>( L, "GetState",  &KGCStateMachine::GetState );
    lua_tinker::class_def<KGCStateMachine>( L, "GetPrevState",  &KGCStateMachine::GetPrevGameState );

    lua_tinker::decl( L, "StateMachine", this );

}

void KGCStateMachine::GoExitGame( KGCGlobal::GAME_EXIT_TYPE eType, int nSockError )
{
    g_pStateExitGame->SetExitType( eType, nSockError );
    GoState( GS_EXITE_GAME, false );
}


#if defined USE_COORDI_SHOP
bool KGCStateMachine::GoCoordiShop( void )
{
    return GoState( GS_COORDI_SHOP );
}
#endif

bool KGCStateMachine::IsEnableExitClient()
{
    if( g_MyD3D->m_pStateMachine->GetState() != GS_GAME ) 
        return true;

    if ( !IsDetermineWin ) 
        return true;

    if ( IsResultComplete ) 
        return true;

    if ( !g_kGlobalValue.m_kUserInfo.bHost ) 
        return true;
    
    return false;
}

void KGCStateMachine::GoExitClient()
{
    g_kGlobalValue.SendSavedCharType();

    if( GetState() == GS_GAME_LOADING )
        KP2P::GetInstance()->Send_ClientFailedGameStartNot(KFailRate::USER_QUIT_GAME);

    // Á¾·á ½Ã, ¾ÆÀÌÅÛ ¸®½ºÆ®°¡ ÀúÀåµÇ·Ï ÇØÁÜ.
    if( IsStateMyInfo() )
    {
        g_MyD3D->SendEquipItemList(&g_MyD3D->m_TempPlayer);
        g_MyD3D->GetMyShortCutSlot()->SendQuickSlotinfo();
    }


    if ( !IsEnableExitClient() ) 
        return;

    //  Á¾·á Ã³¸®
    KGameResultIn kgr;
    MakeGameResult( kgr, g_MyD3D->Get_MyPlayer(), true );

    if( GetState() == GS_GAME )
    {
        Result_LeaveGame = 0;
#if defined(PING_STATISTICS)
        g_pStateGame->CountStatPing();
#endif
        KP2P::GetInstance()->Send_LeaveGameReq( kgr );
        g_MyD3D->WaitForServerAck(Result_LeaveGame, 0, 1000 );     
        //#endif
    }

    g_MyD3D->m_KGCOption.SetLastState(0);
    g_MyD3D->m_KGCOption.SetLastWorldMap((BYTE)WORLDMAP_UPPERMAP);
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON ||
        GetState() == GS_WORLDMAP )
    {
        g_MyD3D->m_KGCOption.SetLastState( (BYTE)GS_WORLDMAP );
        g_MyD3D->m_KGCOption.SetLastWorldMap((BYTE)SiKGCWorldMapManager()->GetCurrentWorldMap());
    }

    if( g_kGlobalValue.m_bShutDown ) { 
        g_kGlobalValue.m_bShutDown = false;
        GoExitGame( KGCGlobal::EXIT_GAME_STATE_SHUTDOWN, 0 );
    }
    else { 
        ::PostMessage( g_MyD3D->m_hWnd, WM_CLOSE, 0, 0 );
    }
}

bool KGCStateMachine::BeginScene()
{
    if( SUCCEEDED( g_MyD3D->m_pd3dDevice->BeginScene() ) )
        return true;
    return false;
}

void KGCStateMachine::EndScene()
{
    g_pGCDeviceManager2->Flush();
    g_MyD3D->m_pd3dDevice->EndScene();
}

bool KGCStateMachine::GoCharGain()
{
	return GoState( GS_CHAR_GAIN );

}

bool KGCStateMachine::GoIntro( void )
{
	return GoState( GS_INTRO );
}

bool KGCStateMachine::IsStateRoom()
{
    return ( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM || g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM || g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT_FROM_ROOM );
}

bool KGCStateMachine::IsStateMyInfo()
{
    return ( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO || g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM );
}

bool KGCStateMachine::IsStateCharSelect()
{
	return ( g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT || g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT_FROM_ROOM );
}