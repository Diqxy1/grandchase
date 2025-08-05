#include "stdafx.h"
#include "GCScene.h"
#include "GCGameOverlayUI.h"
#include "GCOnGameDlg.h"
#include "GCBossHPBar.h"
#include "GCMyHeadup.h"
#include "GCHeadupDisplay.h"
#include "GCPlayerO2Bar.h"
#include "../Message.h"
#include "../Headup Display.h"
#include "../Monster.h"
#include "Replay.h"
#include "../KncP2PLib/kncP2P.h"
#include "QuestGameManager.h"
#include "LatencyTransfer.h"
#include "GCQuestTileMap.h"
#include "KGCRoomManager.h"
#include "KGCWorldMapManager.h"
#include "GCStateGame.h"
#include "GCQuestRankScore.h"
#include "KGCElvis.h"
#include "GCUI/GCColordlg.h"
#include "KGCGameBoard.h"
#include "KGCDeathMatchScoreBoard.h"
#include "gcui/KGCMiniGameResult.h"
#include "gcui/KGCMiniGameScoreBoard.h"
#include "GCGameResultUI.h"
#include "../GCStateMachine.h"
#include "GCEventManager.h"
#include "../gcui/KGCCheckButton.h"
#include "gcui/KGCQuestResult_S3.h"
#include "KGCMissionProgressBox.h"
#include "gcui/gcgameskillbox.h"
#include "KGCLoadingStateBar.h"
#include "KGCStageLoadCompleteSign.h"
#include "KGCMultipleLanguages.h"
#include "GCEscortMerryDesc.h"
#include "GCEscortMonHPBar.h"
#include "./gcui/GCSystemGateCount.h"
#include "GCAdventureManager.h"
#include "GCAddtionGuide.h"
#include "GCAlertInDungeon.h"
#include "KGCRoomListManager.h"
#include "GCDramaPhoto.h"
#include "KGCInGameMessage.h"
#include "KGCSubjectList.h"
#include "KGCCommandGuid.h"
#include "../KGCSubjectManager.h"
#include "GCUI/GCTutorialDlg.h"
#include "GCBossAlram.h"
#include "KGCDotaOverLay.h"

IMPLEMENT_CLASSNAME( KGCGameOverlayUI );
IMPLEMENT_WND_FACTORY( KGCGameOverlayUI );
IMPLEMENT_WND_FACTORY_NAME( KGCGameOverlayUI, "gc_game_overlay_ui" );

KGCGameOverlayUI* g_pkGameOverlayUI = NULL;

#define SHORTCUT_BOX_ALPHA          (0xCCFFFFFF)
#define SHORTCUT_BOX_ALPHA_ZERO     (0x00000000)
#define SHORTCUT_BOX_ALPHA_CHANGE   (0x08080808)
#define PIXELTOTEXEL(p) ( (p)/512.0f )

KGCGameOverlayUI::KGCGameOverlayUI( void )
: m_dwFullScreenColor( 0x00ffffff )
, m_bShowPlayTime( true )
, m_pkSystemGateCount(NULL)
{
    g_pkGameOverlayUI           = this;
    m_dwMissionProcessShowCount = 0;
    for( int i = 0 ; i < MISSION_PROCESS_LIST_NUM ; ++i )
    {
        m_pkMissionProcessList[i] = NULL;
        m_dwMissionProcessShowTime[i] = 0;

        char strMission[MAX_PATH];
        sprintf( strMission, "mission_process%d", i + 1 );
        LINK_CONTROL( strMission,       m_pkMissionProcessList[i] );
    }

    m_iCountNumber				= -1;
    m_iCountFrame				= 0;
    m_iCharType                 = -1;
    m_iCurStage                 = 0;
    m_bTimerEnable              = false;
    m_bChatEnable               = false;
    // [12/17/2007] breadceo. ���� ��� �޼��� ����
    //m_bRebirthEnable    = false;
    m_bHealEnable               = false;
    m_bShowQuestTitle           = false;
    m_bShowChatList             = true;
    m_fLimitedTime              = 0.0f;
    m_fLocalTime                = 0.0f;
    m_fLimitedTimePacketTime    = 0.0f;
    m_fChatTime                 = 0.0f;
    m_bHandleChatEvent          = false;
    m_bDialogueEnable           = false;
    m_bRightSideDialogue        = false;

    m_pkChatList                = NULL;
    m_pkOtherChatList           = NULL;
    m_pkChatEdit                = NULL;
    m_pkChatToggle              = NULL;
    m_pkPaint_Btn               = NULL;
    m_dwEditColor               = 0xFFFFFFFF;
    m_bIsChatColorEnabled       = false;
    m_bIsChatColorUpdated       = false;
    m_pkQuestLoadFlash          = NULL;
    m_pkPlayerO2Bar             = NULL;
    m_pkHeadUpDisplay           = NULL;
    m_pkGameResult              = NULL;
    //����3 �������â
    m_pkQuestResultS3           = NULL;
	m_pkFullScreenWhite			= NULL;
    m_pkFullScreenEclipse   	= NULL;

    // dialogue
    m_pkDialogueBoxLeft         = NULL;
    m_pkDialogueBoxRight        = NULL;
    m_pkDialogueListLeft        = NULL;
    m_pkDialogueListRight       = NULL;

    for ( int i = 0; i < MAX_BOSS_NUM; ++i )
    {
        m_pkBossHPBar[i]				= NULL;
    }

    m_pkMinimap                 = NULL;
    m_bIsWaiting				= false;


	m_bUseTeamChat				= FALSE;

    m_v2OrgChatEditPos.x        = 210.0f;// * m_fWindowScaleX;
    m_v2OrgChatEditPos.y        = 15.0f;// * m_fWindowScaleY;

    m_mapPhoto.clear();

    m_bReplayChatEntered        = false;

    m_vecTitleList.clear();

    m_pkShortCutBox				= NULL;

    m_pkMoveStageBlind          = NULL;

    m_pHotEquipParticle         = NULL;
    m_pkLeftDramaPhoto          = NULL;
    m_pkRightDramaPhoto         = NULL;
    m_pkInGameMessage           = NULL;
    m_pkSubjectList             = NULL;
    m_pkCommandGuid             = NULL;

    LINK_CONTROL( "game_chat_list",             m_pkChatList );
    LINK_CONTROL( "game_other_chat_list",       m_pkOtherChatList );
    LINK_CONTROL( "game_chat_edit",             m_pkChatEdit );
    LINK_CONTROL( "chat_render_toggle",         m_pkChatToggle );
    LINK_CONTROL( "color_palette",              m_pkPaint_Btn );
    LINK_CONTROL( "quest_loading_flasher",      m_pkQuestLoadFlash );

    LINK_CONTROL( "player_o2_bar",              m_pkPlayerO2Bar );
    LINK_CONTROL( "headup_display",             m_pkHeadUpDisplay );
    for ( int i = 0; i < MAX_BOSS_NUM; ++i )
    {
        char strTemp[MAX_PATH] = {0,};
        sprintf_s( strTemp, MAX_PATH, "boss_hp_bar%d", i);
        LINK_CONTROL( strTemp, m_pkBossHPBar[i] );
    }

    LINK_CONTROL( "game_dialogue_box_left",     m_pkDialogueBoxLeft );
    LINK_CONTROL( "game_dialogue_box_right",    m_pkDialogueBoxRight );
    LINK_CONTROL( "game_dialogue_list_left",    m_pkDialogueListLeft );
    LINK_CONTROL( "game_dialogue_list_right",   m_pkDialogueListRight );
  
    LINK_CONTROL( "short_cut_slot",             m_pkShortCutBox );
    LINK_CONTROL( "minimap",                    m_pkMinimap );
	
    LINK_CONTROL( "game_result",                m_pkGameResult );
    LINK_CONTROL( "quest_result_s3",            m_pkQuestResultS3 );

    m_pLoadingTexture = NULL;

    m_bInitValue            = false;
    m_bTimerStart           = false;

    LINK_CONTROL( "quest_count_down_0",      m_pkCountDown[0] );
    LINK_CONTROL( "quest_count_down_1",      m_pkCountDown[1] );
    LINK_CONTROL( "quest_count_down_2",      m_pkCountDown[2] );
    LINK_CONTROL( "quest_count_down_3",      m_pkCountDown[3] );
    LINK_CONTROL( "quest_count_down_4",      m_pkCountDown[4] );
    LINK_CONTROL( "quest_count_down_5",      m_pkCountDown[5] );

    m_pkContinueBox = NULL;
    m_pkContinueHelp = NULL;
    m_pkContinueWait = NULL;
    m_pkBonusMark = NULL;
    LINK_CONTROL( "continue_box",           m_pkContinueBox );

#if defined(RING_OF_FRIENDSHIP_UNLIMIT_REBIRTH)
	LINK_CONTROL( "continue_box2",          m_pkContinueBox2 );
#endif

    m_pkNowLoading = NULL;
    m_pkWaiting = NULL;
    LINK_CONTROL( "now_loading",				m_pkNowLoading );
    LINK_CONTROL( "waiting",				m_pkWaiting );

    //m_pkColorButton = NULL;
    //m_pkColorButtonFFFFFF = NULL;
    //LINK_CONTROL( "color_button",       m_pkColorButton );
    //LINK_CONTROL( "color_button2",      m_pkColorButtonFFFFFF );

    //m_Color = 0xFFFFFFFF;

    //m_pkColorDlg=NULL;
    //LINK_CONTROL( "color_box",      m_pkColorDlg );

    m_pkDeathMatchScoreBoard = NULL;
    LINK_CONTROL("deathmatch_scoreboard", m_pkDeathMatchScoreBoard);

    m_pkMinigameResult = NULL;
    LINK_CONTROL( "minigame_result_box", m_pkMinigameResult );

    m_pkMinigameScoreBoard = NULL;
    LINK_CONTROL( "minigame_scoreboard", m_pkMinigameScoreBoard );

    //TODO: parte do character creation em que aparece o video preview do personagem selecionado. Por algum motivo esta crashando ao trocar de personagem (trocar de video)
	//LINK_CONTROL( "background_white", m_pkFullScreenWhite );
    //LINK_CONTROL( "background_eclipse", m_pkFullScreenEclipse );

    m_pkAllBtn		= NULL;
    m_pkWhisperBtn	= NULL;
    m_pkGuildBtn	= NULL;
    m_pkGeneralBtn	= NULL;
    m_pkShowBtn     = NULL;

    char strName[MAX_PATH] = {0,};
    for ( int i = 0; i < MAX_DUNGEON_PLAYER_NUM; i++ )
    {
        m_pkDungeonLoadingUserState[i] = NULL;
        sprintf( strName, "loading_state_bar%d", i );
        LINK_CONTROL( strName,		    m_pkDungeonLoadingUserState[i] );
    }

    LINK_CONTROL( "move_stage_blind", m_pkMoveStageBlind);

    m_pkStageLoadCompleteSign = NULL;
    LINK_CONTROL( "stage_load_complete_sign", m_pkStageLoadCompleteSign );

    // ������ �̴ϸ�
    m_pkDotaTileMap = NULL;
    LINK_CONTROL( "dota_tilemap", m_pkDotaTileMap );

    // ������ Point���
    m_pkDotaPointOverLay = NULL;
    LINK_CONTROL( "dota_point", m_pkDotaPointOverLay );

    // ������ �ھ� HP���� ���
    m_pkDotaCoreHpBar = NULL;
    LINK_CONTROL( "dota_corehpbar", m_pkDotaCoreHpBar );

    // ������ ī��Ʈ UI
    m_pkDotaCount = NULL;
    LINK_CONTROL( "dota_count", m_pkDotaCount );

    // ������ ���� ����Ʈ UI
    m_pkDotaItemImg = NULL;
    LINK_CONTROL( "dota_itemimg", m_pkDotaItemImg );

    // ������ ���̵� UI
    m_pkDotaGuide = NULL;
    m_pkDotaKeyMark = NULL;
    LINK_CONTROL( "dota_guide", m_pkDotaGuide );
    LINK_CONTROL( "dota_guide_keymark", m_pkDotaKeyMark );

#if defined( USE_ONGAME_INVENTORY )
    m_pkInventoryKey = NULL;
    LINK_CONTROL( "inventory_key", m_pkInventoryKey );
#endif

    // �޸� ȣ�� �̺�Ʈ ����
    m_pkEscortMerryDesc = NULL;
    m_pkEscortMonHPBar = NULL;
    LINK_CONTROL( "escort_merry_desc", m_pkEscortMerryDesc );
    LINK_CONTROL( "escort_mon_hp_bar", m_pkEscortMonHPBar );

	m_pkAddtionGuide = NULL;
	LINK_CONTROL( "addtion_guide", m_pkAddtionGuide );

    // ���̾�α� UI
    for(int iLoop=0; iLoop<NUM_DIALOG_UI; ++iLoop)
    {
        m_ppDialogWnd[iLoop] = NULL;
    }
    
    LINK_CONTROL( "dialog_ui_71_0", m_ppDialogWnd[0] );
    LINK_CONTROL( "dialog_ui_71_1", m_ppDialogWnd[1] );
    LINK_CONTROL( "dialog_ui_71_2", m_ppDialogWnd[2] );
    LINK_CONTROL( "dialog_ui_71_3", m_ppDialogWnd[3] );
    LINK_CONTROL( "dialog_ui_71_4", m_ppDialogWnd[4] );
    LINK_CONTROL( "dialog_ui_71_5", m_ppDialogWnd[5] );
    LINK_CONTROL( "dialog_ui_71_6", m_ppDialogWnd[6] );
    LINK_CONTROL( "dialog_ui_71_7", m_ppDialogWnd[7] );
    LINK_CONTROL( "dialog_ui_07_1", m_ppDialogWnd[8] );
    LINK_CONTROL( "dialog_ui_07_2", m_ppDialogWnd[9] );
    LINK_CONTROL( "dialog_ui_07_3", m_ppDialogWnd[10] );
    LINK_CONTROL( "dialog_ui_07_4", m_ppDialogWnd[11] );
    LINK_CONTROL( "dialog_ui_07_5", m_ppDialogWnd[12] );
    LINK_CONTROL( "dialog_ui_07_6", m_ppDialogWnd[13] );
    LINK_CONTROL( "dialog_ui_07_7", m_ppDialogWnd[14] );
    LINK_CONTROL( "dialog_ui_07_8", m_ppDialogWnd[15] );
    LINK_CONTROL( "dialog_ui_07_9", m_ppDialogWnd[16] );
    LINK_CONTROL( "dialog_ui_07_10", m_ppDialogWnd[17] );
    LINK_CONTROL( "dialog_ui_07_11", m_ppDialogWnd[18] );
    LINK_CONTROL( "dialog_ui_07_12", m_ppDialogWnd[19] );
    LINK_CONTROL( "dialog_ui_07_13", m_ppDialogWnd[20] );
    LINK_CONTROL( "dialog_ui_07_14", m_ppDialogWnd[21] );
    LINK_CONTROL( "dialog_ui_07_15", m_ppDialogWnd[22] );
    LINK_CONTROL( "dialog_ui_07_16", m_ppDialogWnd[23] );
    LINK_CONTROL( "dialog_ui_81_0", m_ppDialogWnd[24] );
    LINK_CONTROL( "dialog_ui_81_1", m_ppDialogWnd[25] );
    LINK_CONTROL( "dialog_ui_81_2", m_ppDialogWnd[26] );
    LINK_CONTROL( "dialog_ui_95_01", m_ppDialogWnd[27] );
    LINK_CONTROL( "dialog_ui_95_02", m_ppDialogWnd[28] );
    LINK_CONTROL( "dialog_ui_95_03", m_ppDialogWnd[29] );
    LINK_CONTROL( "dialog_ui_95_04", m_ppDialogWnd[30] );
    LINK_CONTROL( "dialog_ui_95_05", m_ppDialogWnd[31] );
    LINK_CONTROL( "dialog_ui_95_06", m_ppDialogWnd[32] );
    LINK_CONTROL( "dialog_ui_95_07", m_ppDialogWnd[33] );
    LINK_CONTROL( "dialog_ui_95_08", m_ppDialogWnd[34] );
    LINK_CONTROL( "dialog_ui_123_01", m_ppDialogWnd[35] );
    LINK_CONTROL( "dialog_ui_123_02", m_ppDialogWnd[36] );
    LINK_CONTROL( "dialog_ui_123_03", m_ppDialogWnd[37] );
    LINK_CONTROL( "dialog_ui_123_04", m_ppDialogWnd[38] );
    LINK_CONTROL( "dialog_ui_123_05", m_ppDialogWnd[39] );
    LINK_CONTROL( "dialog_ui_123_06", m_ppDialogWnd[40] );
    LINK_CONTROL( "dialog_ui_123_07", m_ppDialogWnd[41] );
    LINK_CONTROL( "dialog_ui_origin_01", m_ppDialogWnd[42]);
    LINK_CONTROL( "dialog_ui_origin_02", m_ppDialogWnd[43]);
    LINK_CONTROL( "dialog_ui_origin_03", m_ppDialogWnd[44]);
    LINK_CONTROL( "dialog_ui_origin_04", m_ppDialogWnd[45]);
    LINK_CONTROL( "dialog_ui_origin_05", m_ppDialogWnd[46]);
    LINK_CONTROL( "dialog_ui_origin_06", m_ppDialogWnd[47]);
	LINK_CONTROL( "system_monster_gate_count", m_pkSystemGateCount );

	m_pkAlerInDungeon = NULL;
	LINK_CONTROL( "alertindungeon", m_pkAlerInDungeon );
    m_pkZoomDisc = NULL;
    LINK_CONTROL( "zoom_disc", m_pkZoomDisc );

    m_pkZoomScroll = NULL;
    LINK_CONTROL( "zoom_scroll", m_pkZoomScroll );

    m_pkRealMiniMap = NULL;
    LINK_CONTROL( "real_minimap", m_pkRealMiniMap );

    m_pkLimitedTime = NULL;
    LINK_CONTROL( "limited_time", m_pkLimitedTime );

    m_pkFoodGauge = NULL;
    LINK_CONTROL( "food_gauge", m_pkFoodGauge );

    m_pkSayBalloon = NULL;
    LINK_CONTROL( "game_say_ballon", m_pkSayBalloon );

    LINK_CONTROL( "left_drama_photo", m_pkLeftDramaPhoto );
    LINK_CONTROL( "right_drama_photo", m_pkRightDramaPhoto );
    LINK_CONTROL( "ingame_message", m_pkInGameMessage );
    LINK_CONTROL( "subject_list", m_pkSubjectList );
    LINK_CONTROL( "command_guid", m_pkCommandGuid );

    RegisterLuaBind();

    //  ��� �̹��� ���� �ε�
    if ( !LoadDramaPhoto() )
    {
        MessageBox( NULL, L"File Load Error", L"DramaPhotoInfo.lua", MB_OK );
    }

    // ���� �ε����� 
    for( int i=0 ; i<4 ; ++i ) { 
        m_pkLoadingEdge[i] = NULL;
        char temp[128];
        sprintf_s( temp, 127, "dun_loading_egde%d", i );

        LINK_CONTROL( temp, m_pkLoadingEdge[i] );
    }
    m_pkLoadingTipIcon = NULL;
    m_pkLoadingTipBack = NULL;
    m_pkLoadingTipStatic = NULL;
    LINK_CONTROL( "loading_tip_icon", m_pkLoadingTipIcon );
    LINK_CONTROL( "loading_tip_back", m_pkLoadingTipBack );
    LINK_CONTROL( "loading_tip_static", m_pkLoadingTipStatic );

    m_pkLoadingStateBack = NULL;
    LINK_CONTROL( "loading_state_back", m_pkLoadingStateBack );

    m_pkBtnSkillList = NULL;
    LINK_CONTROL( "btn_skilllist", m_pkBtnSkillList );

    m_pkStaticInternalTest = NULL;
    LINK_CONTROL( "static_internalTest", m_pkStaticInternalTest );

    m_pkBossAlram = NULL;
    LINK_CONTROL( "boss_alram", m_pkBossAlram );

	m_pkWorldBillboardChatBoard = NULL;
	LINK_CONTROL( "worldbillboardchat", m_pkWorldBillboardChatBoard );
}

KGCGameOverlayUI::~KGCGameOverlayUI( void )
{
    g_pkGameOverlayUI = NULL;
    m_mapPhoto.clear();

    SAFE_RELEASE( m_pLoadingTexture );
}

void KGCGameOverlayUI::Clear()
{
    m_iCharType         = -1;
    m_iCurStage         = 0;
    m_bTimerEnable      = false;
    m_bChatEnable       = false;
    // [12/17/2007] breadceo. ���� ��� �޼��� ����
    //m_bRebirthEnable    = false;
    m_bHealEnable		= false;
    m_bShowChatList     = true;
    m_fLimitedTime      = 0.0f;
    m_fLocalTime        = 0.0f;
    m_fLimitedTimePacketTime = 0.0f;
    m_fChatTime         = 0.0f;
    m_bHandleChatEvent  = false;
    m_bDialogueEnable   = false;
    m_bRightSideDialogue= false;

	//All ��ư �����༭ ��ġ�� ���ڸ��� ������ �Ѵ�.]
	m_pkChatEdit->SetText(L"");
    m_dwEditColor = 0xFFFFFFFF;
    m_bIsChatColorEnabled = false;
    m_bIsChatColorUpdated = false;
	OnChatBtn(KActionEvent(m_pkAllBtn,KD3DWnd::D3DWE_BUTTON_CLICK));

    SetTimer( false );
    EnableDialogueBox( false );
    EnableO2Bar( false );

    m_mapPreMark.clear();

    if( m_pkStageLoadCompleteSign )
        m_pkStageLoadCompleteSign->InitInfo();
}


//bool KGCGameOverlayUI::IsUseColorItemCash()
//{
//    return g_pColorDlg->IsCashColor(m_strSelColor);
//}

//void KGCGameOverlayUI::OffColorBtn()
//{
//	m_pkColorButtonFFFFFF->ToggleRender(false);
//	m_pkColorButton->ToggleRender(false);
//}

//void KGCGameOverlayUI::SetColor( std::wstring strColor )
//{
//    m_strSelColor = strColor;
//
//    int iCol;
//    WCHAR strTemp[7] = { 0, };
//    wcsncpy(strTemp,strColor.c_str(),6);
//    swscanf(strTemp,L"%x",&iCol);
//    m_Color = 0xFF000000 | iCol;
//
//    bool bWhite = ( m_strSelColor == L"0xFFFFFF" );
//
//    {
//        if (g_MyD3D->MyReplay->m_iCurrState != RP_VIEW
//            && g_MyD3D->MyReplay->m_iCurrState != RP_PREPARE )
//        {
//            m_pkColorButtonFFFFFF->ToggleRender(bWhite);
//            m_pkColorButton->ToggleRender(!bWhite);
//
//            m_pkColorButton->InitState(!bWhite,true,!bWhite ? this : NULL);
//            m_pkColorButtonFFFFFF->InitState(bWhite,true,bWhite ? this : NULL);
//        }
//        else
//        {
//            m_pkColorButtonFFFFFF->ToggleRender(false);
//            m_pkColorButton->ToggleRender(false);
//        }
//    }
//
//    if( !bWhite )
//        m_pkColorButton->SetColor( m_Color );
//}

void KGCGameOverlayUI::OnCreate( void )
{
    m_pkChatList->SetSelfInputCheck( false );
    m_pkChatList->SetFontSize( SiKGCMultipleLanguages()->GetDefualtFontSize() );
    m_pkChatList->SetLineSpace( 1.1f );
    m_pkChatList->Clear();
    m_pkChatList->ToggleRender( m_bShowChatList );   

    m_pkOtherChatList->SetSelfInputCheck( false );
    m_pkOtherChatList->SetFontSize( SiKGCMultipleLanguages()->GetDefualtFontSize() );
    m_pkOtherChatList->SetLineSpace( 1.1f );
    m_pkOtherChatList->Clear();
    m_pkOtherChatList->ToggleRender( m_bShowChatList );

    for( int i = 0 ; i < MISSION_PROCESS_LIST_NUM ; ++i )
    {
        m_pkMissionProcessList[i]->InitState( false );
        m_pkMissionProcessList[i]->SetAlign( DT_CENTER );
        m_pkMissionProcessList[i]->SetText();
    }

    SiGCPairPlaySystem()->ClearChatLog( GCPairPlaySystem::PPSCT_GAME );

    m_pkChatEdit->SetSelfInputCheck( false );
    m_pkChatEdit->SetFontSize( SiKGCMultipleLanguages()->GetLargeFontSize() );
    m_pkChatEdit->SetFontColor( 0xffffffff );
    m_pkChatEdit->AddActionListener( this );
    m_pkChatEdit->EnableSelfFocusCheck( false );

    m_pkChatToggle->SetSelfInputCheck( false );
    m_pkChatToggle->SetFontColor( 0xffffff00 );
    m_pkChatToggle->SetShadow( true );
    m_pkChatToggle->SetMultiLine( false );
    m_pkChatToggle->SetLineSpace( 1.2f );
    m_pkChatToggle->ToggleRender( true );
    m_pkChatToggle->SetText( g_pkStrLoader->GetString( STR_ID_CHAT_MSG_ON_OFF ) );

    m_pkQuestLoadFlash->SetSelfInputCheck( false );
    m_pkQuestLoadFlash->ToggleRender( false );

    m_pkPlayerO2Bar->SetSelfInputCheck( false );
    m_pkPlayerO2Bar->ToggleRender( false );

    for ( int i = 0; i < MAX_BOSS_NUM; ++i )
    {
        m_pkBossHPBar[i]->SetSelfInputCheck( false );
        m_pkBossHPBar[i]->ToggleRender(false);
    }

    m_pkHeadUpDisplay->SetSelfInputCheck( false );
    m_pkHeadUpDisplay->ToggleRender( true );

    m_pkPaint_Btn->SetSelfInputCheck(false);
    m_pkPaint_Btn->ToggleRender(false);
    m_pkPaint_Btn->SetWindowZOrder(D3DWZ_TOPMOST);

    m_pkGameResult->InitState( true, true, this );
    m_pkNowLoading->InitState( false );
    m_pkWaiting->InitState( false );

	SetTimer( false );

    // dialogue
    m_pkDialogueBoxLeft->SetSelfInputCheck( false );
    m_pkDialogueBoxRight->SetSelfInputCheck( false );
    m_pkDialogueListLeft->SetSelfInputCheck( false );
    m_pkDialogueListRight->SetSelfInputCheck( false );

    m_pkDialogueListLeft->SetLineSpace( 1.2f );
    m_pkDialogueListRight->SetLineSpace( 1.2f );

    m_vecTitleList.clear();

    m_pkMinimap->InitState(false, false, NULL);
    m_pkDeathMatchScoreBoard->InitState(false, false, NULL);
    m_pkMinigameResult->InitState( false );
    m_pkMinigameScoreBoard->InitState( false );

    //����3 �������â
    m_pkQuestResultS3->InitState( false, true, this );

    for(int i=0;i<REBIRTH_COUNT_DOWN;++i)
    {
        m_pkCountDown[i]->InitState( false, false, this );
        InitShowNumberEffect(&m_stCountDown[i], m_pkCountDown[i]);      // �ʱ� ��ġ ����
    }
    m_bInitValue            = true;

    // ShortCutSlot
    // Display Setting
    m_iShortCutShowTime	= 0;

    EGCGameModeCategory eGameCategory = SiKGCRoomManager()->GetGameModeCategory();

    m_pkContinueBox->InitState( false );
    m_pkContinueHelp = m_pkContinueBox->GetChildByName( "continue_help" );
    ASSERT( m_pkContinueHelp != NULL );
    m_pkContinueWait = m_pkContinueBox->GetChildByName( "continue_wait" );
    ASSERT( m_pkContinueWait != NULL );
	m_pkContinueWait->ToggleRender( false );
    m_pkBonusMark = m_pkContinueBox->GetChildByName( "user_info_bonus" );
    ASSERT( m_pkBonusMark != NULL );

#if defined(RING_OF_FRIENDSHIP_UNLIMIT_REBIRTH)
	m_pkContinueBox2->InitState( false );
#endif

    // [12/18/2007] breadceo. �������� �κ� ����
    m_pkBonusMark->InitState( true );
    m_pkBonus = static_cast<KGCQuestRankScore*>( m_pkBonusMark->GetChildByName( "bonus" ) );
    ASSERT( m_pkBonus != NULL );
    m_pkBonus->SetScore( 0 );
    m_pkBonus->SetNumPos( D3DXVECTOR2( 12.0f, 2.0f ) );
    m_pkBonus->SetNumWidth( 15.36f );
    m_pkBonus->SetNumHeight( 19.2f );
    m_pkBonus->SetNumInterval( 11.52f );
    m_pkBonus->SetNumOfNumber( 2 );
    m_pkBonus->InitState( true );

    // ä�� �ڽ�!
    KD3DWnd* pChatBox = GetChildByName( "chat_btn_list" );
    ASSERT( pChatBox != NULL );
    m_pkAllBtn		= static_cast<KGCCheckButton*>(pChatBox->GetChildByName( "chat_btn_all" ));
    m_pkWhisperBtn	= static_cast<KGCCheckButton*>(pChatBox->GetChildByName( "chat_btn_whisper" ));
    m_pkGuildBtn	= static_cast<KGCCheckButton*>(pChatBox->GetChildByName( "chat_btn_guild" ));
    m_pkGeneralBtn	= static_cast<KGCCheckButton*>(pChatBox->GetChildByName( "chat_btn_general" ));

    D3DXVECTOR2 vPos(140, 0);
    if( GC_GM_QUEST64 == SiKGCRoomManager()->GetGameMode() )
        vPos = D3DXVECTOR2( 20, 0 );

    pChatBox->SetFixedWindowLocalPos( vPos );

    m_pkShowBtn     = m_pkAllBtn;

    m_mapPreMark.insert(std::map< std::string, std::wstring>::value_type("chat_btn_all", L"") );
    m_mapPreMark.insert(std::map< std::string, std::wstring>::value_type("chat_btn_general", L"") );
    m_mapPreMark.insert(std::map< std::string, std::wstring>::value_type("chat_btn_whisper", L">") );
    m_mapPreMark.insert(std::map< std::string, std::wstring>::value_type("chat_btn_guild", L"@") );

    m_pkAllBtn->InitState( true, true, this );
    m_pkWhisperBtn->InitState( true, true, this );
    m_pkGuildBtn->InitState( true, true, this );
    m_pkGeneralBtn->InitState( true, true, this );

    m_bChatBtnFold = true;
    FoldingChatBox( m_bChatBtnFold );
    m_pkShowBtn->ToggleRender( true );


    for( int i = 0; i < MAX_DUNGEON_PLAYER_NUM; i++ )
        m_pkDungeonLoadingUserState[i]->InitState(false);    

    if( m_pkMoveStageBlind )
        m_pkMoveStageBlind->InitState(false);

    if( m_pkDotaTileMap )
        m_pkDotaTileMap->InitState(false);

    if( m_pkDotaPointOverLay )
        m_pkDotaPointOverLay->InitState( GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory(), GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory() );

    if( m_pkDotaCoreHpBar )
        m_pkDotaCoreHpBar->InitState( GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory(), GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory() );

    if( m_pkDotaCount )
        m_pkDotaCount->InitState( false, false );

    if( m_pkDotaItemImg )
        m_pkDotaItemImg->InitState( false, false );

    if( m_pkDotaGuide )
        m_pkDotaGuide->InitState( GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory(), false );

    if( m_pkDotaKeyMark )
        m_pkDotaKeyMark->InitState( false );

#if defined( USE_ONGAME_INVENTORY )
    if( m_pkInventoryKey ) { 
        m_pkInventoryKey->InitState( false );
    }
#endif

    // �޸� ȣ�� �̺�Ʈ ����
    if( m_pkEscortMerryDesc ) { 
        m_pkEscortMerryDesc->InitState( false, false );
    }

    if( m_pkEscortMonHPBar ) { 
        m_pkEscortMonHPBar->InitState( false, false );
    }

	if (m_pkAddtionGuide)
		m_pkAddtionGuide->InitState(false,true,this);

    // ���̾�α� UI
    for(int iLoop=0; iLoop<NUM_DIALOG_UI; ++iLoop)
    {
        if( m_ppDialogWnd[iLoop] )
        {
            m_ppDialogWnd[iLoop]->InitState( false );
        }
    }

    m_mapDialogUI.clear();
    if( m_ppDialogWnd[0] )    m_mapDialogUI["dialog_ui_71_0"] = m_ppDialogWnd[0];
    if( m_ppDialogWnd[1] )    m_mapDialogUI["dialog_ui_71_1"] = m_ppDialogWnd[1];
    if( m_ppDialogWnd[2] )    m_mapDialogUI["dialog_ui_71_2"] = m_ppDialogWnd[2];
    if( m_ppDialogWnd[3] )    m_mapDialogUI["dialog_ui_71_3"] = m_ppDialogWnd[3];
    if( m_ppDialogWnd[4] )    m_mapDialogUI["dialog_ui_71_4"] = m_ppDialogWnd[4];
    if( m_ppDialogWnd[5] )    m_mapDialogUI["dialog_ui_71_5"] = m_ppDialogWnd[5];
    if( m_ppDialogWnd[6] )    m_mapDialogUI["dialog_ui_71_6"] = m_ppDialogWnd[6];
    if( m_ppDialogWnd[7] )    m_mapDialogUI["dialog_ui_71_7"] = m_ppDialogWnd[7];
    if( m_ppDialogWnd[8] )    m_mapDialogUI["dialog_ui_07_1"] = m_ppDialogWnd[8];
    if( m_ppDialogWnd[9] )    m_mapDialogUI["dialog_ui_07_2"] = m_ppDialogWnd[9];
    if( m_ppDialogWnd[10] )    m_mapDialogUI["dialog_ui_07_3"] = m_ppDialogWnd[10];
    if( m_ppDialogWnd[11] )    m_mapDialogUI["dialog_ui_07_4"] = m_ppDialogWnd[11];
    if( m_ppDialogWnd[12] )    m_mapDialogUI["dialog_ui_07_5"] = m_ppDialogWnd[12];
    if( m_ppDialogWnd[13] )    m_mapDialogUI["dialog_ui_07_6"] = m_ppDialogWnd[13];
    if( m_ppDialogWnd[14] )    m_mapDialogUI["dialog_ui_07_7"] = m_ppDialogWnd[14];
    if( m_ppDialogWnd[15] )    m_mapDialogUI["dialog_ui_07_8"] = m_ppDialogWnd[15];

    if( m_ppDialogWnd[16] )    m_mapDialogUI["dialog_ui_07_9"] = m_ppDialogWnd[16];
    if( m_ppDialogWnd[17] )    m_mapDialogUI["dialog_ui_07_10"] = m_ppDialogWnd[17];
    if( m_ppDialogWnd[18] )    m_mapDialogUI["dialog_ui_07_11"] = m_ppDialogWnd[18];
    if( m_ppDialogWnd[19] )    m_mapDialogUI["dialog_ui_07_12"] = m_ppDialogWnd[19];
    if( m_ppDialogWnd[20] )    m_mapDialogUI["dialog_ui_07_13"] = m_ppDialogWnd[20];
    if( m_ppDialogWnd[21] )    m_mapDialogUI["dialog_ui_07_14"] = m_ppDialogWnd[21];
    if( m_ppDialogWnd[22] )    m_mapDialogUI["dialog_ui_07_15"] = m_ppDialogWnd[22];
    if( m_ppDialogWnd[23] )    m_mapDialogUI["dialog_ui_07_16"] = m_ppDialogWnd[23];

    if( m_ppDialogWnd[24] )    m_mapDialogUI["dialog_ui_81_0"] = m_ppDialogWnd[24];
    if( m_ppDialogWnd[25] )    m_mapDialogUI["dialog_ui_81_1"] = m_ppDialogWnd[25];
    if( m_ppDialogWnd[26] )    m_mapDialogUI["dialog_ui_81_2"] = m_ppDialogWnd[26];

    if( m_ppDialogWnd[27] )    m_mapDialogUI["dialog_ui_95_01"] = m_ppDialogWnd[27];
    if( m_ppDialogWnd[28] )    m_mapDialogUI["dialog_ui_95_02"] = m_ppDialogWnd[28];
    if( m_ppDialogWnd[29] )    m_mapDialogUI["dialog_ui_95_03"] = m_ppDialogWnd[29];
    if( m_ppDialogWnd[30] )    m_mapDialogUI["dialog_ui_95_04"] = m_ppDialogWnd[30];
    if( m_ppDialogWnd[31] )    m_mapDialogUI["dialog_ui_95_05"] = m_ppDialogWnd[31];
    if( m_ppDialogWnd[32] )    m_mapDialogUI["dialog_ui_95_06"] = m_ppDialogWnd[32];
    if( m_ppDialogWnd[33] )    m_mapDialogUI["dialog_ui_95_07"] = m_ppDialogWnd[33];
    if( m_ppDialogWnd[34] )    m_mapDialogUI["dialog_ui_95_08"] = m_ppDialogWnd[34];

    if (m_ppDialogWnd[35])    m_mapDialogUI["dialog_ui_123_01"] = m_ppDialogWnd[35];
    if (m_ppDialogWnd[36])    m_mapDialogUI["dialog_ui_123_02"] = m_ppDialogWnd[36];
    if (m_ppDialogWnd[37])    m_mapDialogUI["dialog_ui_123_03"] = m_ppDialogWnd[37];
    if (m_ppDialogWnd[38])    m_mapDialogUI["dialog_ui_123_04"] = m_ppDialogWnd[38];
    if (m_ppDialogWnd[39])    m_mapDialogUI["dialog_ui_123_05"] = m_ppDialogWnd[39];
    if (m_ppDialogWnd[40])    m_mapDialogUI["dialog_ui_123_06"] = m_ppDialogWnd[40];
    if (m_ppDialogWnd[41])    m_mapDialogUI["dialog_ui_123_07"] = m_ppDialogWnd[41];

    if (m_ppDialogWnd[42])    m_mapDialogUI["dialog_ui_origin_01"] = m_ppDialogWnd[42];
    if (m_ppDialogWnd[43])    m_mapDialogUI["dialog_ui_origin_02"] = m_ppDialogWnd[43];
    if (m_ppDialogWnd[44])    m_mapDialogUI["dialog_ui_origin_03"] = m_ppDialogWnd[44];
    if (m_ppDialogWnd[45])    m_mapDialogUI["dialog_ui_origin_04"] = m_ppDialogWnd[45];
    if (m_ppDialogWnd[46])    m_mapDialogUI["dialog_ui_origin_05"] = m_ppDialogWnd[46];
    if (m_ppDialogWnd[47])    m_mapDialogUI["dialog_ui_origin_06"] = m_ppDialogWnd[47];

    for(int iLoop=0; iLoop<NUM_DIALOG_UI; ++iLoop)
    {
        if( m_ppDialogWnd[iLoop] )
        {
            m_ppDialogWnd[iLoop]->ToggleRender( false );
        }
    }

    m_mapDialogUIAnimPos.clear();
    m_mapDialogUIAnimAlpha.clear();
	if(m_pkSystemGateCount)
		m_pkSystemGateCount->InitState(false,true,this);
	if (m_pkAlerInDungeon)
		m_pkAlerInDungeon->InitState(false,true,this);

    if( m_pkZoomScroll && m_pkZoomDisc )
    {
        SCameraZoom* pCameraZoom = SiKGCWorldMapManager()->GetCameraZoomInfo( SiKGCRoomManager()->GetGameMode() );
        if( pCameraZoom && 0 < int(pCameraZoom->m_vecZoomStep.size()) )
        {
            m_pkZoomScroll->InitScroll(0, 1, 0, int(pCameraZoom->m_vecZoomStep.size()), 2, 0, 1, true );
            m_pkZoomScroll->SetSelfInputCheck(true);
            m_pkZoomScroll->AddActionListener(this);
			m_pkZoomDisc->ToggleRender(pCameraZoom->m_bShowDesc);
			if (pCameraZoom->m_bShowDesc)
			{
				m_pkZoomDisc->SetWindowPosX( static_cast<float>(pCameraZoom->m_iPosUI_X - (m_pkZoomDisc->GetWidth() * 0.25f)) );
				m_pkZoomDisc->SetWindowPosY( static_cast<float>((pCameraZoom->m_iPosUI_Y - m_pkZoomDisc->GetHeight() )) );
			}
			else
			{//���� Ű�� ������ m_pkZoomDisc�� ���� ���ְ� �Ǿ� �ִ�..��,.��....(�ٸ������� �������..)..��ǥ�� �� �ָ� ������������
				m_pkZoomDisc->SetWindowPosX( -1000.0f);
				m_pkZoomDisc->SetWindowPosY( -1000.0f);
			}
            m_pkZoomScroll->SetWindowPosX( static_cast<float>(pCameraZoom->m_iPosUI_X) );
            m_pkZoomScroll->SetWindowPosY( static_cast<float>((pCameraZoom->m_iPosUI_Y)) );
        }
        else
            m_pkZoomScroll->InitScroll(0, 1, 0, 1, 2, 0, 1, true );

        ShowZoomUI( false );
    }

    if( m_pkLimitedTime )
        m_pkLimitedTime->InitState( false );

    if( m_pkFoodGauge )
        m_pkFoodGauge->InitState( false );

    if( m_pkSayBalloon )
    {
        std::vector<DWORD> vecUIDList = m_pkSayBalloon->GetUserUIDList();
        if( false == vecUIDList.empty() )
            m_pkSayBalloon->DeleteAllBalloon();
        m_pkSayBalloon->InitState( false );
    }

    if ( m_pkLeftDramaPhoto )
        m_pkLeftDramaPhoto->InitState( false );

    if ( m_pkRightDramaPhoto )
        m_pkRightDramaPhoto->InitState( false );

    if( m_pkInGameMessage )
        m_pkInGameMessage->InitState( false );

    if( m_pkSubjectList )
        m_pkSubjectList->InitState( false );

    if( m_pkCommandGuid )
        m_pkCommandGuid->InitState( false );

    if( m_pkLoadingTipIcon ) { 
        m_pkLoadingTipIcon->InitState( false );
    }

    if( m_pkLoadingTipBack ) { 
        m_pkLoadingTipBack->InitState( false );
    }

    if( m_pkLoadingTipStatic ) { 
        m_pkLoadingTipStatic->InitState( false );
        m_pkLoadingTipStatic->SetAutoMultiLineLimit( 2 );
    }

    for( int i=0 ; i<4 ; ++i ) { 
        if( m_pkLoadingEdge[i] ) { 
            m_pkLoadingEdge[i]->InitState( false ); 
        } 
    }

    if( m_pkLoadingStateBack ) { 
        m_pkLoadingStateBack->InitState( false );
    }

    if ( m_pkBtnSkillList )
    {
        m_pkBtnSkillList->InitState(false, true, this);
    }

    if( m_pkStaticInternalTest ) { 
        m_pkStaticInternalTest->InitState( false, true, this );
        m_pkStaticInternalTest->SetAlign( DT_LEFT );
    }

    if( m_pkBossAlram ) { 
        m_pkBossAlram->InitState( false, false, this );
    }

	if( m_pkWorldBillboardChatBoard )
	{
		m_pkWorldBillboardChatBoard->InitState( true, true, this );
		m_pkWorldBillboardChatBoard->ToggleButton( false );
		m_pkWorldBillboardChatBoard->SetBlink( false );
	}
}

void KGCGameOverlayUI::ShowZoomUI( bool bShow )
{
    if( m_pkZoomScroll && m_pkZoomDisc )
    {
        m_pkZoomDisc->ToggleRender( bShow );
        m_pkZoomScroll->ToggleRender( bShow );
    }
}

void KGCGameOverlayUI::SetZoomScrollPos( int iPos )
{
    if( m_pkZoomScroll )
    {
        m_pkZoomScroll->SetScrollPos( iPos );
    }
}

float KGCGameOverlayUI::GetZoomValue()
{

    if( m_pkZoomScroll )
    {
        int iPos = m_pkZoomScroll->GetScrollPos();
        SCameraZoom* pCameraZoom = SiKGCWorldMapManager()->GetCameraZoomInfo( SiKGCRoomManager()->GetGameMode() );
        if( pCameraZoom && 0 < int(pCameraZoom->m_vecZoomStep.size()) && iPos < int(pCameraZoom->m_vecZoomStep.size()) )
        {
            return pCameraZoom->m_vecZoomStep[iPos];
        }
        else if( pCameraZoom ) { 
            return pCameraZoom->m_fFristZoom;
        }
        else { 
            return 0.0f;
        }
    }

    return 0.0f;
}

void KGCGameOverlayUI::ShowRealMiniMap( bool bShow )
{
    if( m_pkRealMiniMap )
    {
        m_pkRealMiniMap->ToggleRender( bShow );
    }
}

void KGCGameOverlayUI::SetRealMiniMapInfo( SMiniMapInfo *pMiniMapInfo )
{
    if( m_pkRealMiniMap )
    {
        m_pkRealMiniMap->SetRealMiniMapInfo( pMiniMapInfo );
    }
}

void KGCGameOverlayUI::OnInitialize()
{

    EGCGameModeCategory eGameCategory = SiKGCRoomManager()->GetGameModeCategory();
    if (g_MyD3D->GetGameCategoryKind(eGameCategory) == GC_GMK_MONSTER)
    {
        m_pkShortCutBox->SetShortCutMode(SCB_MODE_ALL);
        ToggleShortCutBox(true);
    }
    else
    {
        m_pkShortCutBox->SetShortCutMode(SCB_MODE_EMOT);
        if (g_MyD3D->m_TempPlayer.NotUseShortCutItem())
        {
            ToggleShortCutBox(false);
        }
    }

	D3DXVECTOR2 dxv2Scale (1.0f,1.0f);
	// ���ӳ� UIũ���  1024x768���̽��� �����. 
	if ( g_pGCDeviceManager2->GetWindowScaleX() >= 1024.0f ) 
		dxv2Scale.x = g_pGCDeviceManager2->GetWindowScaleX() / (1024.0f);

	// ���ӳ� UIũ��� 1024x768���̽��� �����. 
	if (g_pGCDeviceManager2->GetWindowScaleY() >= 768.0f ) 
		dxv2Scale.y = g_pGCDeviceManager2->GetWindowScaleY() / (768.0f);

    for ( int i = 0; i < MAX_BOSS_NUM; ++i )
    {
        D3DXVECTOR2 dxv2Scale_ForBoss( g_pGCDeviceManager2->GetWindowScaleX(), g_pGCDeviceManager2->GetWindowScaleY() );
        m_pkBossHPBar[i]->SetAlignType(WNDUIAT_POSBOTTOMBASE);
        m_pkBossHPBar[i]->SetWindowScale( dxv2Scale_ForBoss );
    }

    D3DXVECTOR2 vPos(210, 15);
	m_pkChatToggle->SetAlignType(WNDUIAT_POSZEROBASE);
	m_pkChatToggle->SetWindowScale(dxv2Scale);
    if( GC_GM_QUEST64 == SiKGCRoomManager()->GetGameMode() )
        vPos = D3DXVECTOR2(90, 15);
    m_pkChatToggle->SetFixedWindowLocalPos( vPos );

    vPos = D3DXVECTOR2(210, 15);
	m_pkChatEdit->SetAlignType(WNDUIAT_POSZEROBASE);
	m_pkChatEdit->SetWindowScale(dxv2Scale);
    if( GC_GM_QUEST64 == SiKGCRoomManager()->GetGameMode() )
        vPos = D3DXVECTOR2(90, 15);
    m_pkChatEdit->SetFixedWindowLocalPos( vPos );

    vPos = D3DXVECTOR2(407, 35);
    m_pkPaint_Btn->SetAlignType(WNDUIAT_POSZEROBASE);
    m_pkPaint_Btn->SetWindowScale(dxv2Scale);
    if (GC_GM_QUEST64 == SiKGCRoomManager()->GetGameMode())
        vPos = D3DXVECTOR2(287, 35);
    m_pkPaint_Btn->SetFixedWindowLocalPos(vPos);
    m_pkPaint_Btn->SetWindowZOrder(D3DWZ_TOPMOST);

    vPos = D3DXVECTOR2(30, 220);
	m_pkChatList->SetAlignType(WNDUIAT_POSZEROBASE);
	m_pkChatList->SetWindowScale(dxv2Scale);
    if( GC_GM_QUEST64 == SiKGCRoomManager()->GetGameMode() )
        vPos = D3DXVECTOR2(15.0f, 205.0f);
    m_pkChatList->SetFixedWindowLocalPos( vPos );

    vPos = D3DXVECTOR2(30, 322);
    if (GC_GM_QUEST64 == SiKGCRoomManager()->GetGameMode())
        vPos = D3DXVECTOR2(15.0f, 307.0f);
    m_pkOtherChatList->SetAlignType(WNDUIAT_POSZEROBASE);
    m_pkOtherChatList->SetWindowScale(dxv2Scale);
    m_pkOtherChatList->SetFixedWindowLocalPos(vPos);

	m_pkAllBtn->SetAlignType(WNDUIAT_POSZEROBASE);
	m_pkAllBtn->SetWindowScale(dxv2Scale);

	m_pkWhisperBtn->SetAlignType(WNDUIAT_POSZEROBASE);
	m_pkWhisperBtn->SetWindowScale(dxv2Scale);

	m_pkGuildBtn->SetAlignType(WNDUIAT_POSZEROBASE);
	m_pkGuildBtn->SetWindowScale(dxv2Scale);

	m_pkGeneralBtn->SetAlignType(WNDUIAT_POSZEROBASE);
	m_pkGeneralBtn->SetWindowScale(dxv2Scale);

	m_pkShowBtn->SetAlignType(WNDUIAT_POSZEROBASE);	
	m_pkShowBtn->SetWindowScale(dxv2Scale);

    m_pkShortCutBox->SetAlignType(WNDUIAT_POSRIGHTBASE);
	m_pkShortCutBox->SetWindowScale(dxv2Scale);

    m_pkBonus->SetAlignType(WNDUIAT_POSZEROBASE);
	m_pkBonus->SetWindowScale(dxv2Scale);

    m_pkMinimap->SetAlignType(WNDUIAT_POSZEROBASE);
	m_pkMinimap->SetWindowScale(dxv2Scale);	
	

	m_pkMinimap->SetWindowPos(D3DXVECTOR2(50 , 50) );
	m_pkMinimap->SetWidth(GC_SCREEN_WIDTH - 50*2);
	m_pkMinimap->SetHeight(GC_SCREEN_HEIGHT - 50*2);


    m_pkMinigameScoreBoard->SetAlignType(WNDUIAT_POSZEROBASE);
	m_pkMinigameScoreBoard->SetWindowScale(dxv2Scale);

	m_dv2GameUIScale = dxv2Scale;
    ReCalcReLocatedRect();

	m_pkShortCutBox->SetAlignGap(D3DXVECTOR2(m_pkShortCutBox->GetCurrentWindowPos().x,
		m_pkShortCutBox->GetCurrentWindowPos().y+m_pkShortCutBox->GetHeight()/2));

	m_fWindowScaleX=g_pGCDeviceManager2->GetWindowScaleX();
	m_fWindowScaleY=g_pGCDeviceManager2->GetWindowScaleY();	

    InitMinimap();
    m_bChatBtnFold = true;
    FoldingChatBox( m_bChatBtnFold );
    m_pkShowBtn->ToggleRender( true );

    if( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() )
        m_pkMoveStageBlind->InitState(true);
    else
        m_pkMoveStageBlind->InitState(false);

    m_pkMoveStageBlind->SetAllWindowAlpha(0);

    m_bFadeIn = false;
    m_iFadeAlpha = 0;

    if( m_pkStageLoadCompleteSign )
        m_pkStageLoadCompleteSign->InitState(false);
    m_kStageLoadCompleteStopWatch.Init();

    if( m_pkRealMiniMap )
    {
        ShowRealMiniMap( false );
    }

    m_pkSubjectList->SetAlignType(WNDUIAT_POSRIGHTBASE);
    m_pkSubjectList->SetWindowScale(dxv2Scale);
    m_pkSubjectList->SetFixedWindowLocalPos( D3DXVECTOR2(m_pkSubjectList->GetFixedWindowLocalPos().x, (120*m_fWindowScaleY)+65) );

    if ( m_pkBtnSkillList )
    {
        m_pkBtnSkillList->ToggleRender(false);
    }

    m_pkWorldBillboardChatBoard->SetIngamePosition();

}


void KGCGameOverlayUI::OnDestroyComplete( void )
{
    SetTimer( false );
    ShortCutSlotClear();

    //m_pkChatList->Clear();
}

void KGCGameOverlayUI::AnimationEndBackground( int iWeight )
{   
    m_iFadeAlpha += iWeight;

    if( m_iFadeAlpha > 255 )
    {
        m_iFadeAlpha = 255;        
    }
    else if( m_iFadeAlpha < 0 )
    {
        m_iFadeAlpha = 0;           
    }    

    m_pkMoveStageBlind->SetAllWindowAlpha(m_iFadeAlpha);
}
#pragma message("[KGCGameOverlayUI::FrameMoveInEnabledState] verificar certas coisas aqui")
void KGCGameOverlayUI::FrameMoveInEnabledState( void )
{
    if( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() )
    {
        if( m_bFadeIn )
        {        
            if( m_iFadeAlpha > 0 )
                AnimationEndBackground(-10);
        }
        else
        {
            if( m_iFadeAlpha < 255 )
            {
                AnimationEndBackground(10);                
                if ( !IsRenderSkillList() )
                    g_pkTutorialDlg->Disable();
        }
        }
#if defined(USE_STAGE_LOAD_COMPLETE_SIGN)
        if( m_pkStageLoadCompleteSign->IsRenderOn() && m_kStageLoadCompleteStopWatch.IsStop() )
        {
            m_pkStageLoadCompleteSign->ToggleRender(false);
        }
#endif
    }

    FrameMove_MissionProcess();

    if( m_bShowQuestTitle && o10 < 10 )
        m_pkQuestLoadFlash->ToggleRender( true );
    else
        m_pkQuestLoadFlash->ToggleRender( false );

    if ( m_bTimerEnable )
        ProcessTimer();

    if( m_fLocalTime > 0 ) {
        ProcessLocalTimer();
    }

    if ( m_bChatEnable )
        ProcessChatList();

	if (m_pkAlerInDungeon)
		m_pkAlerInDungeon->ProcessTitle();
	else
	{
		if ( m_vecTitleList.size() > 0 )
			ProcessTitle();
	}
    
    // ��� �������� ������
    if( m_pkPlayerO2Bar->IsRenderOn() )
        m_pkPlayerO2Bar->SetNowO2( g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->m_fO2 );

    for ( int i = 0; i < MAX_BOSS_NUM; ++i )
    {
        // ���� ���� HP�� �����Ѵ�.
        if ( m_pkBossHPBar[i]->IsRenderOn() && m_vecBossList.size() != 0 )
            ProcessBossHP(i);
    }

    if ( m_pkChatEdit->HasFocus() ) {
        if(!m_bIsChatColorUpdated)
            OnColorButton();

        if ( !m_bIsChatColorEnabled )
            m_dwEditColor = 0xFFFFFFFF;

        if (m_pkPaint_Btn->GetColor() != m_pkChatEdit->GetFontColor() && m_bIsChatColorEnabled)
        {
            m_dwEditColor = m_pkPaint_Btn->GetColor();
            m_pkChatEdit->SetFontColor(m_dwEditColor);
        }

        ProcessChatEdit();
    }
    else
        OffColorButton();

    if ( SiKGCWorldMapManager()->IsSkillListGuideDungeon( SiKGCRoomManager()->GetGameMode() ) )
    {
    if( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() 
            && g_pkInput->IsUp(DIK_H) 
            && !g_pkQuestManager->m_bLoading 
            && !g_MyD3D->m_pkQuestGameMgr->m_bEndGame
            )
        {
            if ( g_pkTutorialDlg->IsEnable() == true )
            {
                g_pkTutorialDlg->Disable();
                m_pkBtnSkillList->ToggleRender(true);
            }
            else
            {
                g_pkTutorialDlg->Enable();
                m_pkBtnSkillList->ToggleRender(false);
            }
        }

        g_pkTutorialDlg->FrameMove();
    }

    if( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() 
        && g_pkInput->IsUp(DIK_TAB) 
        && !g_pkQuestManager->m_bLoading 
        && !g_MyD3D->m_pkQuestGameMgr->m_bEndGame
        )
    {
        if( m_bFadeIn ) {
            m_pkMinimap->ToggleRender(!m_pkMinimap->IsRenderOn());
            m_pkMinimap->SetPlayerRankInfo();
        }
    }

    if( GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory() 
        && g_pkInput->IsUp(DIK_TAB) 
        && !g_pkQuestManager->m_bLoading 
        && !g_MyD3D->m_pkQuestGameMgr->m_bEndGame
        )
    {
        // ������ �̴ϸ� Toggle
        if( m_pkDotaTileMap )
            ToggleDotaTileMap( !m_pkDotaTileMap->IsRenderOn() );
    }

    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ||
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH )
    {
        m_pkDeathMatchScoreBoard->ToggleRender( (bool)g_pkInput->IsPressed(DIK_TAB) );
    }

    FrameMove_Minimap();

    if( g_MyD3D->GetGameCategoryKind(SiKGCRoomManager()->GetGameModeCategory()) == GC_GMK_MONSTER || 
		SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ||
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH ||
		SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_CAPTAIN )
    {
        KGCGameModeInterface* pQuest = g_pStateGame->GetCurGameModePtr();
        bool bGameEnd = false;
        bool bRebirthProcessing = pQuest->m_bRebirthProcessing;
        bool bDeathMatch = false;
        int iRebirthTimer = pQuest->m_nRebirthTimer;

        switch( SiKGCRoomManager()->GetGameModeCategory() )
        {
        case GC_GMC_DUNGEON:
            {
                if( SiKGCWorldMapManager()->IsNotRebirth( SiKGCRoomManager()->GetGameMode(), g_MyD3D->GetDungeonLevel() ) )
                {
					bRebirthProcessing = false;
					iRebirthTimer = 0;
					bDeathMatch = false;
					bGameEnd = true;
					g_pkQuestManager->m_dwEndGameDelay = 0;
                }
				else
				{
					bGameEnd = g_pkQuestManager->m_bEndGame;
				}
            }
            break;
        case GC_GMC_MONSTER_CRUSADER:
            bGameEnd = pQuest->CheckGameEnd();
            break;
        case GC_GMC_DEATHMATCH:
			bRebirthProcessing = g_MyD3D->GetMyPlayer()->m_iArenaCoolTime > 0;
			iRebirthTimer = g_MyD3D->GetMyPlayer()->m_iArenaCoolTime;
			bDeathMatch = true;
			break;
        case GC_GMC_FATAL_DEATHMATCH:
            bRebirthProcessing = g_MyD3D->GetMyPlayer()->m_iArenaCoolTime > 0;
            iRebirthTimer = g_MyD3D->GetMyPlayer()->m_iArenaCoolTime;
            bDeathMatch = true;
            break;
		case GC_GMC_CAPTAIN:
            bRebirthProcessing = g_MyD3D->GetMyPlayer()->m_iArenaCoolTime > 0;
            iRebirthTimer = g_MyD3D->GetMyPlayer()->m_iArenaCoolTime;
            bDeathMatch = true;
			bGameEnd = pQuest->CheckGameEnd();
			break;
        case GC_GMC_DOTA:
            bRebirthProcessing = g_MyD3D->GetMyPlayer()->m_iArenaCoolTime > 0;
            iRebirthTimer = g_MyD3D->GetMyPlayer()->m_iArenaCoolTime;
            break;
        case GC_GMC_AGIT:
            bRebirthProcessing = false;
            bDeathMatch = false;
            break;
        }


		
        if( bRebirthProcessing == true && m_bTimerStart == false && bGameEnd == false )
        {
            m_bTimerStart = true;
            m_stTimer.SetInterpolate( iRebirthTimer, (float)((iRebirthTimer/55)+1)*1000, 0);

            RenderContinueBox( !bDeathMatch );

        }
        else if( (m_bTimerStart == true && IsEndRouletteDiceTimer()) || bRebirthProcessing == false || bGameEnd == true )
        {
            m_bTimerStart = false;

            for(int i=0;i<REBIRTH_COUNT_DOWN;++i)
            {
                m_pkCountDown[i]->ToggleRender( false );
            }
        }
        if( bRebirthProcessing == false )
        {
            RenderContinueBox( false );
        }
        // ��Ȱ�� ��û�� �����̸� continue ������� �Ҵ�
        if( true == pQuest->GetRequestRebirth() )
        {
            RenderContinueBox( !bDeathMatch, !bDeathMatch );

        }
        else if( g_MyD3D->GetMyPlayer()->m_cLife <= 0 )
        {

            RenderContinueBox( !bDeathMatch );

        }

        if( bGameEnd )
        {
            RenderContinueBox( false );
        }
    }
    // ���ཽ��
    DecShortCutShowTime();

    // ������ ���̵� ����
    if( m_pkDotaGuide && GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory() )
    {
        if( 0 < m_pkDotaGuide->GetAddGuideNum() )
        {
            D3DXVECTOR2 vTemp = m_pkDotaGuide->GetFixedWindowPos();
            if( m_pkDotaGuide->GetShowGuide() )
            {
                if( vTemp.x > (-KGCDotaGuide::ANI_SHOW_GAP) )
                    vTemp.x -= float(KGCDotaGuide::ANI_SPEED);
                else
                    vTemp.x = (-KGCDotaGuide::ANI_SHOW_GAP);
            }
            else
            {
                if( vTemp.x < 0.0f )
                    vTemp.x += float(KGCDotaGuide::ANI_SPEED);
                else
                    vTemp.x =  0.0f;
            }
            m_pkDotaGuide->SetFixedWindowLocalPos( vTemp );
            m_pkDotaGuide->ToggleRender( true );
            if( m_pkDotaKeyMark )
                m_pkDotaKeyMark->ToggleRender( true );
        }
        else
        {
            m_pkDotaGuide->ToggleRender( false );
            if( m_pkDotaKeyMark )
                m_pkDotaKeyMark->ToggleRender( false );
        }
    }

    // ȭ�� �߾� ī����
    if( m_iCountNumber >= 0 )
        ++m_iCountFrame;

    if( m_iCountFrame == GC_FPS_LIMIT )
    {
        m_iCountFrame = 0;
        --m_iCountNumber;
    }

    if( IsRenderOnDramaDlg() || 
        SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON ||
        (false == SiKGCWorldMapManager()->IsShowRank(SiKGCRoomManager()->GetGameMode())) ) 
    {
        if ( m_pkEscortMonHPBar != NULL ) {
            m_pkEscortMonHPBar->ShowHPBar( false );
        }
    }
    else 
    {
        if ( m_pkEscortMonHPBar != NULL ) {
            m_pkEscortMonHPBar->ShowHPBar( true );
        }
    }

	//m_pkMissionBox->ToggleRender( m_pkMinimap->IsRenderOn() );

    // ���̾�α� UI : Pos�ִϸ��̼�
    DialogAnimationPos();

    // ���̾�α� UI : Alpha�ִϸ��̼�
    DialogAnimationAlpha();

    if( m_pkSayBalloon )
    {
        m_pkSayBalloon->TimeCheck();
        ReLocateSayBallonMsg();
    }

    // ����UI�� �����ְ�, ������ �ְ�, ���� �� �޽����ڽ��� ������
    if( m_pkShortCutBox->IsRenderOn() && (0 < SiKGCSubjectManager()->GetSubjectNum()) && (false == m_pkInGameMessage->IsRender()) )
        m_pkSubjectList->ToggleRender( true );
    else
        m_pkSubjectList->ToggleRender( false );
}

void KGCGameOverlayUI::OnColorButton()
{
    if (m_pkPaint_Btn)
    {
        KItem* chatPaintItem = g_pItemMgr->GetInventoryItemFromID(ITEM_VIP_TEMPORARY_ITEM);
        if (chatPaintItem != NULL)
        {
            if ( !m_bIsChatColorUpdated ) {
                m_pkPaint_Btn->ToggleRender(true);
                m_bIsChatColorUpdated = true;
            }
            if ( !m_bIsChatColorEnabled ) {
                m_bIsChatColorEnabled = true;
                SiGCTextColorManager()->SetColorNum(GCTextColorManager::CASH_ITEM_COLOR_NUM + 1);
                m_pkPaint_Btn->CheckPaletteButtons(true);
                m_pkPaint_Btn->EnablePaletteButtons(true);
            }
        }
        else
        {
            m_pkPaint_Btn->ToggleRender(false);
            m_bIsChatColorEnabled = false;
            m_bIsChatColorUpdated = false;
        }
    }
}

void KGCGameOverlayUI::OffColorButton()
{
    if ( m_bIsChatColorUpdated ) {
        m_bIsChatColorUpdated = false;
        if ( m_pkPaint_Btn )
            m_pkPaint_Btn->ToggleRender( false );
    }
}

void KGCGameOverlayUI::DialogAnimationPos()
{
    bool bErase = false;
    std::map<KD3DWnd*, DIALOG_ANIM_POS>::iterator mapPosIter = m_mapDialogUIAnimPos.begin();
    for(; mapPosIter != m_mapDialogUIAnimPos.end(); )
    {
        bErase = false;
        if( mapPosIter->first )
        {
            mapPosIter->first->SetWindowPosDirect( D3DXVECTOR2(mapPosIter->second.fStartX, mapPosIter->second.fStartY) );

            // X�� �̵�
            if( 0 > mapPosIter->second.fSpeedX )
            {
                if( mapPosIter->second.fStartX > mapPosIter->second.fEndX )
                    mapPosIter->second.fStartX += mapPosIter->second.fSpeedX;
                else
                    mapPosIter->second.fStartX = mapPosIter->second.fEndX;
            }
            else
            {
                if( mapPosIter->second.fStartX < mapPosIter->second.fEndX )
                    mapPosIter->second.fStartX += mapPosIter->second.fSpeedX;
                else
                    mapPosIter->second.fStartX = mapPosIter->second.fEndX;
            }
            // Y�� �̵�
            if( 0 > mapPosIter->second.fSpeedY )
            {
                if( mapPosIter->second.fStartY > mapPosIter->second.fEndY )
                    mapPosIter->second.fStartY += mapPosIter->second.fSpeedY;
                else
                    mapPosIter->second.fStartY = mapPosIter->second.fEndY;
            }
            else
            {
                if( mapPosIter->second.fStartY < mapPosIter->second.fEndY )
                    mapPosIter->second.fStartY += mapPosIter->second.fSpeedY;
                else
                    mapPosIter->second.fStartY = mapPosIter->second.fEndY;
            }

            // -X��� -Y��
            if( 0 >= mapPosIter->second.fSpeedX && 0 >= mapPosIter->second.fSpeedY )
            {
                if( mapPosIter->second.fStartX <= mapPosIter->second.fEndX && 
                    mapPosIter->second.fStartY <= mapPosIter->second.fEndY )
                    bErase = true;
            }
            // -X��� +Y��
            else if( 0 >= mapPosIter->second.fSpeedX && 0 <= mapPosIter->second.fSpeedY )
            {
                if( mapPosIter->second.fStartX <= mapPosIter->second.fEndX && 
                    mapPosIter->second.fStartY >= mapPosIter->second.fEndY )
                    bErase = true;
            }
            // +X��� -Y��
            else if( 0 <= mapPosIter->second.fSpeedX && 0 >= mapPosIter->second.fSpeedY )
            {
                if( mapPosIter->second.fStartX >= mapPosIter->second.fEndX && 
                    mapPosIter->second.fStartY <= mapPosIter->second.fEndY )
                    bErase = true;
            }
            // +X��� +Y��
            else if( 0 <= mapPosIter->second.fSpeedX && 0 <= mapPosIter->second.fSpeedY )
            {
                if( mapPosIter->second.fStartX >= mapPosIter->second.fEndX && 
                    mapPosIter->second.fStartY >= mapPosIter->second.fEndY )
                    bErase = true;
            }
        }

        if( bErase )
            mapPosIter = m_mapDialogUIAnimPos.erase( mapPosIter );
        else
            ++mapPosIter;
    }
}

void KGCGameOverlayUI::DialogAnimationAlpha()
{
    bool bErase = false;
    std::map<KD3DWnd*, DIALOG_ANIM_ALPHA>::iterator mapAlphaIter = m_mapDialogUIAnimAlpha.begin();
    for(; mapAlphaIter != m_mapDialogUIAnimAlpha.end(); )
    {
        bErase = false;
        if( mapAlphaIter->first )
        {
            mapAlphaIter->first->SetAllWindowAlpha( mapAlphaIter->second.iStartAlpha );

            // ���Ļ���
            if( 0 > mapAlphaIter->second.iSpeed )
            {
                if( mapAlphaIter->second.iStartAlpha > mapAlphaIter->second.iEndAlpha )
                    mapAlphaIter->second.iStartAlpha += mapAlphaIter->second.iSpeed;
                else
                    mapAlphaIter->second.iStartAlpha = mapAlphaIter->second.iEndAlpha;

                if( mapAlphaIter->second.iStartAlpha <= mapAlphaIter->second.iEndAlpha )
                    bErase = true;
            }
            // ���ĳֱ�
            else
            {
                if( mapAlphaIter->second.iStartAlpha < mapAlphaIter->second.iEndAlpha )
                    mapAlphaIter->second.iStartAlpha += mapAlphaIter->second.iSpeed;
                else
                    mapAlphaIter->second.iStartAlpha = mapAlphaIter->second.iEndAlpha;

                if( mapAlphaIter->second.iStartAlpha >= mapAlphaIter->second.iEndAlpha )
                    bErase = true;
            }
        }

        if( bErase )
            mapAlphaIter = m_mapDialogUIAnimAlpha.erase( mapAlphaIter );
        else
            ++mapAlphaIter;
    }
}

void KGCGameOverlayUI::PreDraw( void )
{
    // NOTE : �������� UI�� ��ü������ Linear Filtering���� �׸��� ����.
    // Linear Filtering ����
	 g_pGCDeviceManager2->PushState();

	 // �������̵� UI�� �� ���� ���� �ϴ��� �𸣰���
	 //g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	 //g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );       
	 /*g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_NONE );
	 g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_NONE );       */
    // PostChildDraw���� Pop�ȴ�.
}

void KGCGameOverlayUI::PostChildDraw( void )
{

    // ����Ʈ Ÿ��Ʋ ȭ���� ������?
    if( m_bShowQuestTitle && m_pLoadingTexture )
    {
        D3DXVECTOR2 vPos( GetFixedWindowPos() );

		g_pGCDeviceManager2->DrawInScreen(m_pLoadingTexture,
			0, 0, GC_SCREEN_FLOAT_WIDTH*m_fWindowScaleX, GC_SCREEN_FLOAT_HEIGHT*m_fWindowScaleY, 0.0f, 0.0f, 1.0f, 1.0f,false,0.1f,0xffffffff);

        ShowDungeonLoadingTip( true );

        m_pkNowLoading->ToggleRender( !m_bIsWaiting );
        m_pkNowLoading->Render();
        m_pkNowLoading->ToggleRender( false );
        m_pkWaiting->ToggleRender( m_bIsWaiting );
        m_pkWaiting->Render();
        m_pkWaiting->ToggleRender( false );

        m_pkLoadingStateBack->ToggleRender( true );
        m_pkLoadingStateBack->Render();

        for( int i = 0; i < MAX_DUNGEON_PLAYER_NUM; i++ )
        {
            m_pkDungeonLoadingUserState[i]->Render();
        }        
    }

    if ( m_bTimerEnable && NULL == m_pkLimitedTime )
    {
		D3DXVECTOR2 vPos;

        SETVECTOR2( vPos, 10.0f, GC_SCREEN_FLOAT_HEIGHT*m_fWindowScaleY - 70.0f * m_dv2GameUIScale.y);
		//SETVECTOR2( vPos, 10.0f, 530.0f *m_fWindowScale);
		vPos+=GetRelocatedWindowPos();

        g_pkUIMgr->RenderNumber( "white_num",vPos, static_cast<int>( m_fLimitedTime ),true,true,-1,false );
    }

    if ( false == m_bShowQuestTitle)
    {

		if (m_pkAlerInDungeon)
		{
			int a = static_cast<int>( 255.0f * TitleParameterFunc( m_pkAlerInDungeon->m_fTime ) );
			m_pkAlerInDungeon->SetAllWindowAlpha(a);
		}

		else 
		{
			if (m_vecTitleList.size() > 0 )
			{
				const KTitleTextInfo& rkInfo = m_vecTitleList.front();
				BYTE a = static_cast<BYTE>( 255.0f * TitleParameterFunc( rkInfo.fTimeParam ) );        

				g_pkFontMgrTitle->OutTextMultiline( (int)rkInfo.vTitlePos.x, (int)rkInfo.vTitlePos.y, rkInfo.strTitleText.c_str(), D3DCOLOR_ARGB( a, 255, 255, 255 ) );

			}	
		}  
    }

    if ( g_kGlobalValue.m_kUserInfo.bHost )
    {
        if ( false == m_bShowQuestTitle && m_bDialogueEnable )
        {   
            if ( m_bRightSideDialogue )
            {
                if ( 0 != m_pkDialogueListRight->GetCount() )
                {
                    /*if ( g_pkQuestManager->m_bDramaStage )
                    {
                        g_pkFontMgr->OutTextXY( 480, 560, g_pkStrLoader->GetString( STR_ID_OVERAY_UI_TEXT5 ).c_str(), 0xffff0000 );
                    }
                    else
                    {*/
#if defined (NATION_BRAZIL)
                    if (g_MyD3D->GetMyPlayer()->GetCharType() != GC_CHAR_LIME || SiKGCRoomManager()->GetGameMode() != GC_GM_QUEST0 )
#endif
					 g_pkFontMgr->OutTextXY( static_cast<int>(480 * m_fWindowScaleX * GC_SCREEN_DIV_WIDTH), static_cast<int>(560 * m_fWindowScaleY* GC_SCREEN_DIV_WIDTH), g_pkStrLoader->GetString( STR_ID_OVERAY_UI_TEXT1 ).c_str(), 0xffff0000 );

                    //}

                }
            }
            else
            {
                if ( 0 != m_pkDialogueListLeft->GetCount() )
                {
                    /*if ( g_pkQuestManager->m_bDramaStage )
                    {
                        g_pkFontMgr->OutTextXY( 50, 560, g_pkStrLoader->GetString( STR_ID_OVERAY_UI_TEXT5 ).c_str(),0xffff0000 );
                    }
                    else
                    {*/
#if defined (NATION_BRAZIL)
                    if (g_MyD3D->GetMyPlayer()->GetCharType() != GC_CHAR_LIME || SiKGCRoomManager()->GetGameMode() != GC_GM_QUEST0 )
#endif
                        g_pkFontMgr->OutTextXY( static_cast<int>(50 * m_fWindowScaleX* GC_SCREEN_DIV_WIDTH), static_cast<int>(560 * m_fWindowScaleY* GC_SCREEN_DIV_WIDTH), g_pkStrLoader->GetString( STR_ID_OVERAY_UI_TEXT1 ).c_str(),0xffff0000 );
                    //}
                }
            }
        }
    }


    if ( false == m_bShowQuestTitle && false == m_bDialogueEnable )	// ��Ȱ �޼��� ���
    {
        if( m_bHealEnable )
        {
			D3DXVECTOR2 vPos;

			vPos = GetRelocatedWindowPos();
			vPos.x += 200.0f * m_fWindowScaleX;
			vPos.y += 540.0f * m_fWindowScaleY;
            g_pkFontMgr->OutTextXY( (int)vPos.x, (int)( vPos.y + 15.0f * m_fWindowScaleY ), g_pkStrLoader->GetString( STR_ID_OVERAY_UI_TEXT4 ).c_str(), 0xffff0000 );
        }		
    }	

	// �÷��� �ð� ǥ��
	if( 	(false == m_bShowQuestTitle &&
            m_bFadeIn &&
            false ==m_pkQuestResultS3->IsRenderOn() &&

			SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON) ||
			( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MINIGAME )
		)
	{		
        if( m_bShowPlayTime && SiKGCWorldMapManager()->IsShowPlayTime( SiKGCRoomManager()->GetGameMode() ) )
        {

            float fXGap = 200.0f;
            float fYGap = 120.0f;

//             KLuaManager luaMgr;
//             GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
//                  
//             LUA_GET_VALUE_DEF( "x", fXGap, 0.0f );
//             LUA_GET_VALUE_DEF( "y", fYGap, 0.0f );                


            DWORD sec=g_pGameBoard->GetPlayTimeInFrame()/GC_FPS_LIMIT%60;
            DWORD minute=g_pGameBoard->GetPlayTimeInFrame()/GC_FPS_LIMIT/60;
            if(minute>60)
            {		
                if(!(minute<61 && sec < 10 && g_pGameBoard->GetPlayTimeInFrame()%GC_FPS_LIMIT<27))
                    g_MyD3D->m_pkUIMgr->RenderNumber("overtime",D3DXVECTOR2(GC_SCREEN_FLOAT_WIDTH*m_fWindowScaleX-fXGap * m_dv2GameUIScale.x,fYGap*m_fWindowScaleY),0,true,true,-1,false);
            }
            else 
            {
                char str_min[3];
                char str_sec[3];

                sprintf(str_min,"%02d",minute);
                sprintf(str_sec,"%02d",sec);
                //���� �ð� ǥ��

                g_MyD3D->m_pkUIMgr->RenderNumber("dungeon_clock_num",D3DXVECTOR2(GC_SCREEN_FLOAT_WIDTH*m_fWindowScaleX-fXGap * m_dv2GameUIScale.x,fYGap*m_fWindowScaleY),str_min,D3DXCOLOR( 0xffffffff ),true,true,-1.0f,false);
                g_MyD3D->m_pkUIMgr->RenderNumber("dungeon_clock_dash",D3DXVECTOR2(GC_SCREEN_FLOAT_WIDTH*m_fWindowScaleX-(fXGap-55) * m_dv2GameUIScale.x,fYGap*m_fWindowScaleY),0,true,true,-1,false);
                g_MyD3D->m_pkUIMgr->RenderNumber("dungeon_clock_num",D3DXVECTOR2(GC_SCREEN_FLOAT_WIDTH*m_fWindowScaleX-(fXGap-70) * m_dv2GameUIScale.x,fYGap*m_fWindowScaleY),str_sec,D3DXCOLOR( 0xffffffff ),true,true,-1.0f,false);
            }

            RenderCountNumber();
        }
    }

    // NOTE : �������� UI�� ��ü������ Linear Filtering���� �׸��� ����.
    // PreDraw���� Push�ȴ�.
	
	g_pGCDeviceManager2->PopState();	
    // Linear Filtering ��
    if(m_pkMinimap->IsRenderOn())
        m_pkMinimap->Render();
}

bool KGCGameOverlayUI::CheckIfIsOtherChar(std::wstring OriginString) {
    if (OriginString.find(L"#c43f4a7") != std::wstring::npos)
        return true;
    if (OriginString.find(L"#cff0000") != std::wstring::npos)
        return true;
    if (OriginString.find(L"#cX#cff28da") != std::wstring::npos)
        return true;
    if (OriginString.find(L"#cX#c43f4a7") != std::wstring::npos)
        return true;
    return false;
}

void KGCGameOverlayUI::AddChat( const std::wstring& strChat, int iChatType/* = KGCChatManager::CHAT_TYPE_NORMAL*/, DWORD dwColor/* = 0xFFFFFFFF*/ )
{
    static CString strPrev = _T("");
    static int iNumSameInput = 0;

    CString strNow = _T("");	
    strNow = strChat.c_str();
    //�Էµ� ���ڰ� �����Ͱ� ���ٸ� ����Ѵ�.
    if ( strNow == strPrev )
        iNumSameInput++;
    else
        strPrev = strNow;

    //3���̳� ��ϵǸ� ����� �����Ͽ� ġ�� �ʰ� �Ѵ�. 
    if (iNumSameInput == 3)
    {
        iNumSameInput = 0;
        return;
    }

    dwColor = m_dwEditColor;

    ASSERT( g_pkChatManager != NULL );

    if (true == CheckIfIsOtherChar(strChat)) {
        m_pkOtherChatList->AddString(strChat, dwColor, false, 0, true, L"", iChatType);
    }
    else {
        m_pkChatList->AddString(strChat, dwColor, false, 0, true, L"", iChatType);
    }

    //���÷��̿��� ä�� �������� ���.
    if ( g_MyD3D->m_pStateMachine->GetState() == GS_GAME && g_MyD3D->MyReplay->m_iCurrState != RP_VIEW)
    {
        m_bReplayChatEntered = true; //ä���� ���Դٰ� ǥ��,
        m_strChatMsg.clear();
        m_strChatMsg = strChat;	
    }

    m_fChatTime = 0.0f;

    m_bChatEnable = m_bShowChatList;
}

void KGCGameOverlayUI::AddMacroCommunityChat( const std::wstring& strChat, int iChatType/* = KGCChatManager::CHAT_TYPE_NORMAL*/, DWORD dwColor/* = 0xFFFFFFFF*/ )
{
    CString strNow = _T("");	
    strNow = strChat.c_str();

    ASSERT( g_pkChatManager != NULL );

    if (true == CheckIfIsOtherChar(strChat)) {
        m_pkOtherChatList->AddString(strChat, dwColor, false, 0, true, L"", iChatType);
    }
    else {
        m_pkChatList->AddString(strChat, dwColor, false, 0, true, L"", iChatType);
    }

    //���÷��̿��� ä�� �������� ���.
    if ( g_MyD3D->m_pStateMachine->GetState() == GS_GAME && g_MyD3D->MyReplay->m_iCurrState != RP_VIEW)
    {
        m_bReplayChatEntered = true;
        m_strChatMsg.clear();
        m_strChatMsg = strChat;	
    }

    m_fChatTime = 0.0f;
    m_bChatEnable = m_bShowChatList;
}

void KGCGameOverlayUI::ShowQuestTitle( int iQuest, int iStage, bool bShow )
{
    // Next Stage ���� �׽�Ʈ
    // ���� ù ��������...�ٵ�;; �ٽ� ���ư���? ����
    if ( iStage == 0 )
    {
        m_bShowQuestTitle = bShow;
        // ����Ʈ �ε�ȭ���� �ε��Ѵ�!!
        SAFE_RELEASE( m_pLoadingTexture );
        m_pLoadingTexture = g_pGCDeviceManager2->CreateTexture( SiKGCWorldMapManager()->GetDungeonLoadingImgName( SiKGCRoomManager()->GetGameMode() ) );

        g_MyD3D->MyHead->ComboClear();
        g_kCamera.InitCamera();
    }
    else
    {
        FadeInOrOut( false );
    }

    m_iCurStage = iStage;

    //m_pkColorDlg->CancelDlg();

	//m_dwFullScreenColor = D3DCOLOR_ARGB( 0x00, 0xff, 0xff, 0xff );
	//m_pkFullScreenWhite->ForceSetWindowColor( m_dwFullScreenColor );
}

void KGCGameOverlayUI::HideAllQuestTitle(void)
{
    // �ؽ��ĸ� ��ε� ����
    m_bShowQuestTitle = false;
    SAFE_RELEASE( m_pLoadingTexture );
    ShowDungeonLoadingTip( false );
    m_pkLoadingStateBack->ToggleRender( false );
}

void KGCGameOverlayUI::SetSpecialMonsterTimer( int iCount)
{
	if(m_pkSystemGateCount)
		m_pkSystemGateCount->OnSetCount(iCount);
}

bool KGCGameOverlayUI::IsChapionMonsterTimerOn()
{
	if(m_pkSystemGateCount){
		return m_pkSystemGateCount->IsRenderOn();
	}
	return false;
}

void KGCGameOverlayUI::SetTimer( bool bEnable, float fLimitedTime /* = 0.0f */, bool bLocal_ /*= false*/ )
{
    m_bTimerEnable = bEnable;
    m_fLimitedTime = fLimitedTime;
    m_fLimitedTimePacketTime = 1.0f;
    m_bLocalTimer = bLocal_;
}

void KGCGameOverlayUI::SetLocalTimer( float fLocalTime )
{
    m_fLocalTime = fLocalTime;
}

void KGCGameOverlayUI::EnableDialogueBox( bool bEnable )
{
    m_pkDialogueBoxLeft->ToggleRender( bEnable );
    m_pkDialogueBoxRight->ToggleRender( bEnable );
    m_pkDialogueListLeft->ToggleRender( bEnable );
    m_pkDialogueListRight->ToggleRender( bEnable );

    if ( m_pkLeftDramaPhoto && m_pkRightDramaPhoto )
    {
        m_pkLeftDramaPhoto->ToggleRender( false );
        m_pkRightDramaPhoto->ToggleRender( false );
    }

    m_pkDialogueListLeft->Clear();
    m_pkDialogueListRight->Clear();

    if( false == bEnable )
    {
        for(int iLoop=0; iLoop<NUM_DIALOG_UI; ++iLoop)
        {
            if( m_ppDialogWnd[iLoop] )
            {
                m_ppDialogWnd[iLoop]->ToggleRender( false );
            }
        }
    }

    m_bDialogueEnable = bEnable;

    if( false == bEnable )
        m_bShowPlayTime = true;
}

void KGCGameOverlayUI::SetDialogue( bool bRight, int nStrSpeaker, int nStrSpeech )
{
    if ( m_bDialogueEnable )
    {
        m_pkDialogueListRight->Clear();
        m_pkDialogueListLeft->Clear();

        //���⼭ ��� �Ⱥ��̰� �մϴ�.
        m_pkLeftDramaPhoto->ToggleRender( false );
        m_pkRightDramaPhoto->ToggleRender( false );

        std::wstring strBlank = g_pkStrLoader->GetString(STR_ID_BLANK);
        std::wstring strSpeech = g_pkStrLoader->GetString( nStrSpeech );
        std::wstring strSpeaker = g_pkStrLoader->GetString( nStrSpeaker );
        int iStrLength = strSpeech.length();
        
        if( -1 == nStrSpeech || strBlank == strSpeech )
        {
            m_pkDialogueBoxLeft->ToggleRender( false );
            m_pkDialogueBoxRight->ToggleRender( false );
        }
        else
        {
            m_pkDialogueBoxLeft->ToggleRender( true );
            m_pkDialogueBoxRight->ToggleRender( true );
        }

        
        if ( bRight )
        {
            m_pkDialogueListRight->AddString( strSpeaker, 0xffffff00 );
            m_pkDialogueListRight->AddString( strSpeech );
        }
        else
        {
            m_pkDialogueListLeft->AddString( strSpeaker, 0xffffff00 );
            m_pkDialogueListLeft->AddString( strSpeech );
        }

        //  -1�̸� ��� ���� �׸��� �ʴ´�.
        if( nStrSpeaker != -1 )
        {
            std::map< int, SDramaPhoto >::iterator mit = m_mapPhoto.find( nStrSpeaker );
            if( mit != m_mapPhoto.end() )
            {
                if ( bRight ) 
                {
                    m_pkRightDramaPhoto->SetDramaPhoto( mit->second );
                    m_pkRightDramaPhoto->ToggleRender( true );
                }
                else
                {
                    m_pkLeftDramaPhoto->SetDramaPhoto( mit->second );
                    m_pkLeftDramaPhoto->ToggleRender( true );
                }
            }
        }

        m_bRightSideDialogue = bRight;
    }
}


void KGCGameOverlayUI::SetEditText( const WCHAR* szText )
{
    if ( m_pkChatEdit != NULL )
    {
        m_pkChatEdit->SetText( szText );
    }
}

const WCHAR* KGCGameOverlayUI::GetEditText( void ) const
{
    if ( m_pkChatEdit != NULL )
    {
        return m_pkChatEdit->GetText();
    }
    return NULL;
}

std::wstring KGCGameOverlayUI::GetChatMsg( void )
{
    std::wstring strChatmsg = L"";
    strChatmsg = m_strChatMsg;
    m_strChatMsg.clear();
    m_bReplayChatEntered = false;
    return strChatmsg;
}

void KGCGameOverlayUI::SetEditFocus( void )
{
    if ( m_pkChatEdit != NULL )
    {
        //���÷��� ��� ���� ���� ä���� ���ϰ� �Ѵ�.
        if ( g_MyD3D->MyReplay->m_iCurrState != RP_VIEW ) 
        {
            m_pkChatEdit->SetFocus();

			if(m_bUseTeamChat && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DOTA ) //��ä�� ����̸�
			{
				SetEditText( L"%" );
				m_pkChatEdit->SetSelection(1, 1);
			}
        }
    }
}

bool KGCGameOverlayUI::HasEditFocus( void ) const
{
    if ( m_pkChatEdit != NULL )
    {
        return m_pkChatEdit->HasFocus();
    }
    return false;
}

void KGCGameOverlayUI::SetTitleText( const std::wstring& strTitleText )
{
	if (m_pkAlerInDungeon)
	{
		m_pkAlerInDungeon->SetStaticDesc(strTitleText);
	}
    else
    {
		if ( strTitleText.size() <= 0 )
			return;

        // '\n'�� ���ڿ��� �и��Ѵ�.
        std::wstring strText = strTitleText;
        while ( strText.size() > 0 )
        {
            std::wstring::size_type delimiter = strText.find( L'\n' );
            std::wstring strTitle = strText.substr( 0, delimiter );

            KTitleTextInfo kInfo;
            kInfo.vTitlePos.x  = 0.5f * ( GC_SCREEN_WIDTH * m_fWindowScaleX - g_pkFontMgrTitle->GetWidth( strTitle.c_str() ) );
            kInfo.vTitlePos.y  = GC_SCREEN_HEIGHT* m_fWindowScaleY  / 3.0f;
            kInfo.strTitleText = strTitle;
            kInfo.fTimeParam   = 0.0f;

            strText.erase( 0, std::string::npos == delimiter ? delimiter : delimiter + 1 );

            m_vecTitleList.push_back( kInfo );
        }
    }
}

void KGCGameOverlayUI::SetTitleTextAutoLine( const std::wstring& strTitleText, float fTitlePosX, float fTitlePosY)
{
	if (m_pkAlerInDungeon)
	{
		m_pkAlerInDungeon->SetStaticDesc(strTitleText);
	}
	else 
	{
		if ( strTitleText.size() <= 0 )
			return;
		// '\n'�� ���ڿ��� �и��Ѵ�.
		std::wstring strText = strTitleText;
		
		std::wstring::size_type delimiter = strText.find( L'\n' );
		std::wstring strTitle = strText.substr( 0, delimiter );

		KTitleTextInfo kInfo;
		kInfo.vTitlePos.x  = fTitlePosX;
		kInfo.vTitlePos.y  = fTitlePosY;
		kInfo.strTitleText = strTitleText;
		kInfo.fTimeParam   = -1.0f;

		m_vecTitleList.push_back( kInfo );
	}
}



//void KGCGameOverlayUI::EnableRebirthMessage( bool bEnable )
//{
//    m_bRebirthEnable = bEnable;
//}

void KGCGameOverlayUI::EnableHealMessage( bool bEnable )
{
    m_bHealEnable = bEnable;
}

void KGCGameOverlayUI::ShowChatToggleMsg(bool bShow)
{ 
    //���÷��� �ÿ��� �׻� �� �ֵ���.
    if (g_MyD3D->MyReplay->m_iCurrState == RP_VIEW )
        bShow = true;

    m_pkChatToggle->ToggleRender(bShow); 
}
void KGCGameOverlayUI::ToggleChatList( void )
{
    std::map<int, SMacroCommunity> mapMacroCommunity = SiKGCWorldMapManager()->GetMacroCommunity( SiKGCRoomManager()->GetGameMode() );
    if( false == mapMacroCommunity.empty() )
        m_bShowChatList = true;
    else
        m_bShowChatList = !m_bShowChatList;
    m_pkChatList->ToggleRender(m_bShowChatList);
    m_pkOtherChatList->ToggleRender(m_bShowChatList);
}

void KGCGameOverlayUI::EnableO2Bar( bool bEnable )
{
    m_pkPlayerO2Bar->ToggleRender(bEnable);
    m_pkPlayerO2Bar->EnableO2Bar(bEnable);

    int m_iPlayerIdx = g_MyD3D->Get_MyPlayer();
    m_pkPlayerO2Bar->SetMaxO2( g_MyD3D->MyPlayer[m_iPlayerIdx]->m_fO2 );
    m_pkPlayerO2Bar->SetNowO2( g_MyD3D->MyPlayer[m_iPlayerIdx]->m_fO2 );
}
void KGCGameOverlayUI::ActionPerformed( const KActionEvent& event )
{
    // NOTE : �Ϻ��� �Է½� ���� ��ȯ ��带 �̿��� ��� �ܼ��� ����Ű �Է� Ȯ�����θ� üũ�� �ϸ� ������ �����.
    // �׷��� ����Ʈ ��Ʈ���� �̺�Ʈ�� ó���ϵ��� �Ѵ�.
    if ( event.m_pWnd == m_pkChatEdit ) // ���� �� ä�� �Է��� �޴´�.
    {
        if( event.m_dwCode == D3DWE_EDIT_ESCAPE )
            ClearChatEvent();
        else
            HandleChatEvent();

        SetChatEventHandling( true );
    }
	if ( event.m_pWnd == m_pkAllBtn ||
              event.m_pWnd == m_pkWhisperBtn ||
              event.m_pWnd == m_pkGuildBtn ||
              event.m_pWnd == m_pkGeneralBtn )
    {
        OnChatBtn( event );
    }

    if( m_pkZoomScroll && (event.m_pWnd == m_pkZoomScroll) )
    {
        int iTemp = m_pkZoomScroll->GetScrollPos();
        
        SCameraZoom* pCameraZoom = SiKGCWorldMapManager()->GetCameraZoomInfo( SiKGCRoomManager()->GetGameMode() );
        if( pCameraZoom && (iTemp < int(pCameraZoom->m_vecZoomStep.size())) )
        {
#if defined( USE_CAMERA_ZOOM_STATE )
            int iPlayerIndex = g_MyD3D->Get_MyPlayer();
            if( g_MyD3D->IsPlayerIndex(iPlayerIndex) )
            {
                g_MyD3D->MyPlayer[iPlayerIndex]->AddCameraZoomState( g_kCamera.m_fWideMode, pCameraZoom->m_vecZoomStep[iTemp] );
            }
#endif

            g_kCamera.m_fWideMode = pCameraZoom->m_vecZoomStep[iTemp];
            g_kCamera.SavePrevState();
        }
    }

    if ( event.m_pWnd == m_pkBtnSkillList && event.m_dwCode == D3DWE_BUTTON_CLICK)
    {
        g_pkTutorialDlg->Enable();
        m_pkBtnSkillList->ToggleRender(false);
}
}

void KGCGameOverlayUI::ClearChatEvent( void )
{
    ShowChatToggleMsg( true );
    g_MyD3D->MyHead->IsCheatMode = false;

    g_KDSound.Play( "72" );

    SetEditText( NULL );
    ::SetFocus( g_hUIMainWnd );
}

void KGCGameOverlayUI::HandleChatEvent( void )
{
    m_bUseTeamChat = FALSE;

	KGCChatManager::EChatCommand chattype; //ä�� Ÿ��

    ShowChatToggleMsg( true );
    g_MyD3D->MyHead->IsCheatMode = false;

    const WCHAR* szText = GetEditText();
	if (szText[0] == L'/') {
		KGCChatManager::EChatCommand  eChatCmd=g_pkChatManager->AnalyzeChatCommand( szText );
	}

    if ( szText != NULL && ::wcslen( szText ) != 0 )
    {
        static DWORD dwLastSendChat = 0;
        static CString strPrev = _T("");
        static int iNumSameInput = 0;

		chattype = static_cast<KGCChatManager::EChatCommand>(g_pkChatManager->GetCommend( ));

        // ���������� ��üä���� ������ ��ä������ ó���Ѵ�.
        if( KGCChatManager::CHAT_CMD_NORMAL == chattype &&
            SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA )
            chattype = KGCChatManager::CHAT_CMD_TEAM;

        if ( !wcscmp( szText , 
            g_pkStrLoader->GetString(STR_ID_WHISPER_FILTER_COMMAND).c_str() ) )
        {
            if( g_pkChatManager->GetWhisperFilter() )
                g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString(STR_ID_NOTIFY_WHISPER_FILTER_MODE_OFF), KGCChatManager::CHAT_TYPE_ALERT );
            else
                g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString(STR_ID_NOTIFY_WHISPER_FILTER_MODE_ON), KGCChatManager::CHAT_TYPE_ALERT );

            g_pkChatManager->SetWhisperFilter( !g_pkChatManager->GetWhisperFilter() );
            m_bUseTeamChat = FALSE;
        }
        else if ( KGCChatManager::CHAT_CMD_NORMAL == chattype)
        {
            // ������ �Է½ð����� ���� �ð��̻� ������?
            if( timeGetTime() - dwLastSendChat > 1000 * 3/*��*/ )
                iNumSameInput = 0; // ��� �ʱ�ȭ �����ش�
            // �� �ð��� �ΰ� ������ �� ������ �ƴٰ� ����� �ƴϴϱ�..

            //�Էµ� ���ڰ� �����Ͱ� ���ٸ� ����Ѵ�.
            if ( strPrev == szText )
                iNumSameInput++;
            else
            {
                strPrev = szText;
                iNumSameInput = 0;
            }

			if( !g_kGlobalValue.m_kLoginInfo.bNewUser )
			{
				//3���̳� ��ϵǸ� ����� �����Ͽ� ġ�� �ʰ� �Ѵ�. 
				if (iNumSameInput >= 3)
				{		
					g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_TOO_MANY_CHAT ), KGCChatManager::CHAT_TYPE_ALERT );				
				}
				else if( timeGetTime() - dwLastSendChat < 1000 )
				{				
					g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_TOO_MANY_CHAT ), KGCChatManager::CHAT_TYPE_ALERT );				
				}
				else
				{
					std::wstring IllegalColorChat = szText;
					
					g_pkStrLoader->Replacer(IllegalColorChat, L"#c", L"");
					g_pkStrLoader->Replacer(IllegalColorChat, L"#C", L"");
	                
					StringCchCopyW( (WCHAR*)szText, MAX_CHAT_STR_BUF_SIZE, IllegalColorChat.c_str() );

					std::wstring strContent = szText;
					SiKD3DEmoticonString()->CheckForSend( strContent );

                    D3DCOLOR chatColor = m_dwEditColor;         

                    KGC_PID_CHAT_PACKET kPacket(g_kGlobalValue.m_kUserInfo.dwUID, g_kGlobalValue.m_kUserInfo.strNickName, g_kGlobalValue.m_kUserInfo.strNickColor, strContent.c_str(), chatColor, KGCChatManager::CHAT_CMD_NORMAL);

					SERIALIZE_P2P_PACKET( kPacket );

					SendP2PPacket( (void*)ksBuff.GetData(), ksBuff.GetLength(),KNC_BROADCAST, _RELIABLE );  
					KGCPC_MEMO( "KGC_PID_CHAT_PACKET" );              
					dwLastSendChat = timeGetTime();
				}
			}
            m_bUseTeamChat = FALSE;
        }
		else if( KGCChatManager::CHAT_CMD_TEAM == chattype ) //��ä���� ���
		{
			if( SiKGCRoomManager()->GetGameMode() == GC_GM_TEAM_MODE || 
                SiKGCRoomManager()->GetGameMode() == GC_GM_TAG_TEAM || 
                SiKGCRoomManager()->GetGameMode() == GC_GM_DEATH_TEAM || 
                SiKGCRoomManager()->GetGameMode() == GC_GM_DOTA || 
                SiKGCRoomManager()->GetGameMode() == GC_GM_AUTOMATCH_TEAM ||
                SiKGCRoomManager()->GetGameMode() == GC_GM_FATAL_DEATH_TEAM )
			{
				// ������ �Է½ð����� ���� �ð��̻� ������?
				if( timeGetTime() - dwLastSendChat > 1000 * 3/*��*/ )
					iNumSameInput = 0; // ��� �ʱ�ȭ �����ش�
				// �� �ð��� �ΰ� ������ �� ������ �ƴٰ� ����� �ƴϴϱ�..

				//�Էµ� ���ڰ� �����Ͱ� ���ٸ� ����Ѵ�.
				if ( strPrev == szText )
					iNumSameInput++;
				else
				{
					strPrev = szText;
					iNumSameInput = 0;
				}

				//3���̳� ��ϵǸ� ����� �����Ͽ� ġ�� �ʰ� �Ѵ�. 
				if (iNumSameInput >= 3)
				{		
					g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_TOO_MANY_CHAT ), KGCChatManager::CHAT_TYPE_ALERT );				
				}
				else if( timeGetTime() - dwLastSendChat < 1000 )
				{				
					g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_TOO_MANY_CHAT ), KGCChatManager::CHAT_TYPE_ALERT );				
				}
				else
				{
					CString IllegalColorChat = szText;
					CString Sharpc = "#c";
					CString SharpC = "#C";
					CString NullStr = "";
					while( IllegalColorChat.Find( Sharpc ) != -1 || IllegalColorChat.Find( SharpC ) != -1 )
					{
						IllegalColorChat.Replace( SharpC, NullStr );
						IllegalColorChat.Replace( Sharpc, NullStr );
					}
					StringCchCopyW( (WCHAR*)szText, MAX_CHAT_STR_BUF_SIZE, IllegalColorChat.GetBuffer() );

					std::wstring strContent = szText;

					std::wstring team;
                    if( szText[0] == '%' )
					strContent = strContent.substr(1);
                    else
                        strContent = strContent.substr(0);


                    if( TEAM_SERDIN == g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->m_kUserInfo.iTeam )
					    team = g_pkStrLoader->GetString(STR_TEAM_SERDIN);
                    else
                        team = g_pkStrLoader->GetString(STR_TEAM_CANABAN);

                    std::wstring strNickName = L"[" + team + L"] " + g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->m_kUserInfo.strNickName;
                    std::wstring strNickColor = g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->m_kUserInfo.strNickColor;

					SiKD3DEmoticonString()->CheckForSend( strContent );

                    D3DCOLOR chatColor = m_dwEditColor;

                    KGC_PID_CHAT_PACKET kPacket(g_kGlobalValue.m_kUserInfo.dwUID, strNickName, strNickColor, strContent, chatColor, KGCChatManager::CHAT_CMD_TEAM);

					SERIALIZE_P2P_PACKET( kPacket );

					//�� ä�� ���� �ڵ�
					for(int i = 0; i < MAX_PLAYER_NUM; i++)
					{
						if(g_MyD3D->IsMyTeam(i) && g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
						{
							SendP2PPacket( (void*)ksBuff.GetData(), ksBuff.GetLength(),g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID, _RELIABLE );
						}
					}

					dwLastSendChat = timeGetTime();
				}
				m_bUseTeamChat = TRUE;
			}
			else
			{
				g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString(STR_ID_NOT_IN_TEAM), KGCChatManager::CHAT_TYPE_ALERT );
                m_bUseTeamChat = FALSE;
			}
		}
        else if(KGCChatManager::CHAT_CMD_GUILD  == chattype ) //��� ä���� ���
        {
            std::wstring strChatMsg;
            if ( false == g_pkGuildMemo->FilterGuildMemoCommand(szText ) )
            {
                strChatMsg = szText;
                SiKD3DEmoticonString()->CheckForSend( strChatMsg );
                //KP2P::GetInstance()->Send_GuildChatReq( L"Server", strChatMsg.c_str() );
                KP2P::GetInstance()->Send_ChatReq( L"Server", strChatMsg.c_str(), KChatData::MSG_GUILD );
            }
        }
		else if(KGCChatManager::CHAT_CMD_WHISPER  == chattype ) //�ӼӸ�
		{
			std::wstring strChatMsg;
			//g_pkChatManager->AddChatMsg(szText,chattype );
			KGCChatManager::EChatCommand  eChatCmd=g_pkChatManager->AnalyzeChatCommand( szText );
		}
        g_KDSound.Play( "72" );
        m_bUseTeamChat = FALSE;
    }

    SetEditText( NULL );
    ::SetFocus( g_hUIMainWnd );
}

void KGCGameOverlayUI::ProcessTimer()
{
    if( m_bLocalTimer ) {
        m_fLimitedTime -= GC_ELAPSED_TIME;        
    } else {
        if( g_kGlobalValue.m_kUserInfo.bHost == true )
        {
            m_fLimitedTime -= GC_ELAPSED_TIME;
            m_fLimitedTimePacketTime -= GC_ELAPSED_TIME;
            if( m_fLimitedTimePacketTime <= 0.0f )
            {
                LTP_QUEST_TIME Packet;
                ZeroMemory( &Packet, sizeof(LTP_QUEST_TIME) );
                Packet.packetType   = ELTP_QUEST_TIME;
                Packet.size         = sizeof(LTP_QUEST_TIME);
                Packet.latencyTime  = 0;

                if ( m_fLimitedTime <= 1.0f )
                {
                    Packet.bEnd = true;
                }
                Packet.fTime = m_fLimitedTime;

                g_LatencyTransfer.PushPacket( &Packet );

                m_fLimitedTimePacketTime = 1.0f;
            }
        }
    }    

    // RP_SLOW ����
    static int ox=0;
    ox++;

    if(ox>1)
        ox=0;

    if ( g_MyD3D->MyReplay->m_iCurrState == RP_VIEW )
    {
        switch(g_MyD3D->MyReplay->m_eReplay_Speed)
        {
        case RS_SLOW:
            if(!ox)
                m_fLimitedTime -= GC_ELAPSED_TIME;
            break;
        case RS_NORMAL:
            m_fLimitedTime -= GC_ELAPSED_TIME;
            break;
        case RS_FAST:
            m_fLimitedTime -= GC_ELAPSED_TIME * 2;
            break;
        case RS_TOP:
            m_fLimitedTime -= GC_ELAPSED_TIME * 4;
            break;
        }

    }

    CheckToToggleOfTimerUI();
}

void KGCGameOverlayUI::ProcessLocalTimer()
{
    m_fLocalTime -= GC_ELAPSED_TIME;
    
    if ( m_fLocalTime <= 0.0f )
    {
        m_fLocalTime = 0.0f;
    }

    if ( 0.01f < m_fLocalTime )
    {
        m_pkLimitedTime->ToggleRender( true );
        m_pkLimitedTime->SetTime( m_fLocalTime );
    }
    else if( m_pkLimitedTime )
    {
        m_pkLimitedTime->ToggleRender( false );
    }
}

void KGCGameOverlayUI::CheckToToggleOfTimerUI()
{
    // Ÿ�̸� ����
    if ( m_fLimitedTime <= 0.0f )
    {
        m_fLimitedTime = 0.0f;
        m_fLimitedTimePacketTime = 0.0f;
        m_bTimerEnable = false;
        m_bLocalTimer = false;
    }

    if ( m_bTimerEnable && m_pkLimitedTime && 0.01f < m_fLimitedTime )
    {
        m_pkLimitedTime->ToggleRender( true );
        m_pkLimitedTime->SetTime( m_fLimitedTime );
    }
    else if( m_pkLimitedTime )
    {
        m_pkLimitedTime->ToggleRender( false );
    }
}

void KGCGameOverlayUI::ProcessChatList()
{
#if !defined( NATION_BRAZIL )
	if(g_kGlobalValue.m_kLoginInfo.bNewUser && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL )
		return;
#endif

    m_fChatTime += GC_ELAPSED_TIME;

    float t = m_fChatTime / CHAT_VIEW_TIME;
    if ( t > 1.0f ) t = 1.0f;
    BYTE a = static_cast<BYTE>( 255.0f * ChatParameterFunc( t ) );

    m_pkChatList->SetTransparent( a );
    m_pkOtherChatList->SetTransparent(a);

    if ( m_fChatTime >= static_cast<float>( CHAT_VIEW_TIME ) )
    {
        m_fChatTime = 0.0f;
        m_bChatEnable = false;
    }
}

void KGCGameOverlayUI::ProcessTitle()
{
    KTitleTextInfo& rkInfo = m_vecTitleList.front();
    rkInfo.fTimeParam += 0.2f * GC_ELAPSED_TIME;
    if ( rkInfo.fTimeParam > 0.7f )
    {
        m_vecTitleList.erase( m_vecTitleList.begin() );
    }
}

void KGCGameOverlayUI::ProcessChatEdit()
{
	KGCChatManager::EChatCommand  eChatCmd=g_pkChatManager->FindChatType( m_pkChatEdit->GetText() );
	std::wstring wStr = m_pkChatEdit->GetText();
	if ( eChatCmd == KGCChatManager::CHAT_CMD_ERROR || wStr.empty() ) {
		eChatCmd = static_cast<KGCChatManager::EChatCommand>(g_pkChatManager->GetCommend());
	} else {
		KGCCheckButton* chkBtn = m_pkGeneralBtn;
		switch (eChatCmd)
		{
		case KGCChatManager::CHAT_CMD_NORMAL:             
			chkBtn = m_pkGeneralBtn;
			break;
		case KGCChatManager::CHAT_CMD_GUILD:
			chkBtn = m_pkGuildBtn;
			break;                
		case KGCChatManager::CHAT_CMD_WHISPER:                            
			chkBtn = m_pkWhisperBtn;
			break;                

		}

		if ( chkBtn != m_pkShowBtn )
		{
			// ��ġ �ٲ��� ��
			D3DXVECTOR2 vecTemp = chkBtn->GetFixedWindowLocalPos();
			chkBtn->SetWindowPosDirect( m_pkShowBtn->GetFixedWindowLocalPos() );
			m_pkShowBtn->SetWindowPosDirect( vecTemp );

			// �ٲ��� �������� �߿���
			m_pkShowBtn = static_cast<KGCCheckButton*>(chkBtn);

			SetEditFocus();
			//������ ��ȣ�� �־��ش�.
			m_pkChatEdit->SetText( eChatCmd == KGCChatManager::CHAT_CMD_WHISPER ? L">":L"",false );
			SetChatEventHandling(true);
			ShowChatToggleMsg(false);
			g_MyD3D->MyHead->IsCheatMode = true;

			//HandleChatEvent();
		}

		m_bChatBtnFold = true;
		FoldingChatBox( m_bChatBtnFold );

		// �Ф���
		m_pkShowBtn->ToggleRender( true );

		g_KDSound.Play("72");
		g_pkChatManager->SetCommend(eChatCmd);
		return;
	}
	// Ű�� ������ �Ҹ�
	for ( int i = 0; i < 256; i++ )
	{
		if ( g_pkInput->IsDown( i ) ) 
			g_KDSound.Play( "71" );
	}

    m_pkChatEdit->SetFontColor(m_dwEditColor);

    // �ӼӸ� ó��
    std::wstring strCmd;
    bool bIsInput_GreaterThanSign = (g_pkInput->IsPressed( DIK_LSHIFT ) || g_pkInput->IsPressed( DIK_RSHIFT )) && g_pkInput->IsDown( DIK_PERIOD );
    //  �Էµ� ������ ���̰� 1,                  �׸���,    �װ��� '>' �϶�
    if( (1 == ::wcslen( m_pkChatEdit->GetText())) && bIsInput_GreaterThanSign ){
        strCmd = g_pkChatManager->OnWhisperInputStart();
        //  �� ȭ��ǥ�� ������ ��
    }else if ( g_pkChatManager->IsWhisperMode() && g_pkInput->IsDown( DIK_UP ) ){
        strCmd = g_pkChatManager->OnWhisperScrollUp();
        //  �Ʒ� ȭ��ǥ�� ������ ��
    }else if ( g_pkChatManager->IsWhisperMode() && g_pkInput->IsDown( DIK_DOWN ) ){
        strCmd = g_pkChatManager->OnWhisperScrollDown();
    }

    int strCmdSize = static_cast<int>( strCmd.size() );
    if( strCmdSize > 0 ){
            m_pkChatEdit->SetText( strCmd.c_str() );
        m_pkChatEdit->SetSelection( strCmdSize + 1, strCmdSize + 1 );
    }
}

void KGCGameOverlayUI::DeleteBossList()
{
    for ( int i = 0; i < MAX_BOSS_NUM; ++i )
    {
        if (m_pkBossHPBar[i] == NULL)
            continue;

        m_pkBossHPBar[i]->HideBossHPBar();
        m_pkBossHPBar[i]->ToggleRender( false );
        
        m_pkBossHPBar[i]->SetMaxHP( 0.0f );
        m_pkBossHPBar[i]->SetNowHP( 0.0f );
    }

    m_vecBossList.clear();
}

void KGCGameOverlayUI::RenderOffHPBar( bool bForceOff )
{
	for ( int i = 0; i < MAX_BOSS_NUM; ++i )
	{
		if (m_pkBossHPBar[i] == NULL)
			continue;

        //  ������ ����.
        if ( bForceOff ) 
        {
            m_pkBossHPBar[i]->HideBossHPBar();
            m_pkBossHPBar[i]->ToggleRender( false );
        }
        else
        {
    		m_pkBossHPBar[i]->RenderOffHPBar();
        }
	}
}


void KGCGameOverlayUI::AddBossMonster(MONSTER *boss )
{
    m_vecBossList.push_back(boss);
    UpdateBossHPBar(boss->m_iMonsterType);
}

void KGCGameOverlayUI::UpdateBossHPBar( int iMonsterType )
{
    if ( iMonsterType == MON_PATUSEI_LEG || 
        iMonsterType == MON_PATUSEI_BODY  ||
        iMonsterType == MON_VS_PATUSEI_BODY || 
        iMonsterType == MON_VS_PATUSEI_LEG 

        )
        return;

    //TotalHP
    float fTotalHP = 0.0f;
    float fTotalMaxHP = 0.0f;
	 
    for(int i = 0; i < (int)m_vecBossList.size(); i++)
    {
        if ( m_vecBossList[i]->m_iMonsterType == MON_PATUSEI_HEAD || 
            m_vecBossList[i]->m_iMonsterType == MON_PATUSEI_BODY || 
            m_vecBossList[i]->m_iMonsterType == MON_PATUSEI_LEG ||
            m_vecBossList[i]->m_iMonsterType == MON_VS_PATUSEI_HEAD || 
            m_vecBossList[i]->m_iMonsterType == MON_VS_PATUSEI_BODY || 
            m_vecBossList[i]->m_iMonsterType == MON_VS_PATUSEI_LEG
            )
        {
            fTotalMaxHP += m_vecBossList[i]->m_fHPPoint;
            fTotalHP += m_vecBossList[i]->m_fHP;

		    if ( m_vecBossList[i]->m_iMonsterType == MON_PATUSEI_HEAD ||
                m_vecBossList[i]->m_iMonsterType == MON_VS_PATUSEI_HEAD
                )
		    {
                if( fTotalHP > 0 )
                {
                    int iIndex = g_pkGameOverlayUI->EnableBossHPBar(true, iMonsterType);
                    if ( iIndex != -1 )
                    {
                        if ( iIndex == 1 )
                        {
                            m_pkBossHPBar[iIndex]->SetAnimationPos( -70.0f, 40.0f );
                        }
                        m_pkBossHPBar[iIndex]->ToggleRender(true);
                        m_pkBossHPBar[iIndex]->SetMaxHP( fTotalMaxHP);
                        m_pkBossHPBar[iIndex]->SetNowHP( fTotalHP );
                    }
                }
		    }
        }
		else
		{
			if ( m_vecBossList[i]->m_iMonsterType == iMonsterType )
			{
				fTotalMaxHP = m_vecBossList[i]->m_fHPPoint;
				fTotalHP = m_vecBossList[i]->m_fHP;

                if( fTotalHP > 0 )
                {
                    int iIndex = g_pkGameOverlayUI->EnableBossHPBar(true, iMonsterType);
                    if ( iIndex != -1 )
                    {
                        if ( iIndex == 1 )
                        {
                            m_pkBossHPBar[iIndex]->SetAnimationPos( -70.0f, 40.0f );
                        }
                        m_pkBossHPBar[iIndex]->ToggleRender(true);
                        m_pkBossHPBar[iIndex]->SetMaxHP( fTotalMaxHP );
                        m_pkBossHPBar[iIndex]->SetNowHP( fTotalHP );
                    }
                }
			}
		}
    }
}
void KGCGameOverlayUI::BindBossHPUpdate( int iMonsterType )
{
    std::vector<MONSTER*>::iterator vecIter = m_vecBossList.begin();
    for(int iLoop=0; vecIter != m_vecBossList.end(); ++iLoop, ++vecIter)
    {
        if( (*vecIter)->m_iMonsterType == iMonsterType )
        {
            m_pkBossHPBar[iLoop]->SetNowHP( (*vecIter)->m_fHP );
        }
    }
}
void KGCGameOverlayUI::ProcessBossHP( int iIndex_ )
{
    float fTotalHP = 0.0f;
    for(int i = 0; i < (int)m_vecBossList.size(); i++)
    {
		if ( m_vecBossList[i]->m_iMonsterType == MON_PATUSEI_HEAD || 
			m_vecBossList[i]->m_iMonsterType == MON_PATUSEI_BODY || 
			m_vecBossList[i]->m_iMonsterType == MON_PATUSEI_LEG ||
            m_vecBossList[i]->m_iMonsterType == MON_VS_PATUSEI_HEAD || 
            m_vecBossList[i]->m_iMonsterType == MON_VS_PATUSEI_BODY || 
            m_vecBossList[i]->m_iMonsterType == MON_VS_PATUSEI_LEG
            )
		{
			fTotalHP += m_vecBossList[i]->m_fHP;

            if ( m_vecBossList[i]->m_iMonsterType == MON_PATUSEI_HEAD ||
                m_vecBossList[i]->m_iMonsterType == MON_VS_PATUSEI_HEAD
                )
                m_pkBossHPBar[iIndex_]->SetNowHP( fTotalHP );
		}
		else
		{
            if ( i >= MAX_BOSS_NUM ) 
                continue;

			if ( m_vecBossList[i]->m_bRender && m_vecBossList[i]->m_bLive )
			{
				fTotalHP = m_vecBossList[i]->m_fHP;
                m_pkBossHPBar[i]->SetNowHP( fTotalHP );
			}
		}
    }
}

bool KGCGameOverlayUI::IsAddBossMonster(MONSTER *pMonster)
{
    std::vector<MONSTER*>::iterator vecIter = m_vecBossList.begin();
    for(; vecIter != m_vecBossList.end(); ++vecIter)
    {
        if( (*vecIter) == pMonster )
            break;
    }

    if( vecIter != m_vecBossList.end() )
        return true;
    return false;
}

bool KGCGameOverlayUI::IsRenderBossHPBar( MONSTER *pMonster )
{
    if ( (*pMonster).m_iMonsterType == MON_PATUSEI_LEG     ||
         (*pMonster).m_iMonsterType == MON_PATUSEI_BODY    ||
         (*pMonster).m_iMonsterType == MON_VS_PATUSEI_BODY ||
         (*pMonster).m_iMonsterType == MON_VS_PATUSEI_LEG  )
        return true;

    int iLoop = -1;
    std::vector<MONSTER*>::iterator vecIter = m_vecBossList.begin();
    for( ; vecIter != m_vecBossList.end(); ++vecIter )
    {
        if ( (*vecIter)->m_iMonsterType != MON_PATUSEI_LEG     &&
             (*vecIter)->m_iMonsterType != MON_PATUSEI_BODY    &&
             (*vecIter)->m_iMonsterType != MON_VS_PATUSEI_BODY &&
             (*vecIter)->m_iMonsterType != MON_VS_PATUSEI_LEG )
            ++iLoop;

        if( (*vecIter) == pMonster )
            break;
    }

    if( iLoop != -1 && iLoop < MAX_BOSS_NUM )
	{
        return m_pkBossHPBar[iLoop]->IsRenderOn();
	}

    return false;
}

int KGCGameOverlayUI::EnableBossHPBar( bool bEnable, int iMonsterType )
{
    for ( int i = 0; i < MAX_BOSS_NUM; ++i )
    {
        if ( bEnable != m_pkBossHPBar[i]->IsEnable() )
        {
            m_pkBossHPBar[i]->EnableHPBar( bEnable && !m_vecBossList.empty(), iMonsterType );
            return i;
        }
    }

    return -1;
}

void KGCGameOverlayUI::HideBossHPBar()
{
    for ( int i = 0; i < MAX_BOSS_NUM; ++i )
    {
        m_pkBossHPBar[i]->HideBossHPBar();
    }
}

void KGCGameOverlayUI::EnableHUD( bool bEnable, bool bShowStatusBar )
{
    m_pkHeadUpDisplay->ShowHUD( bEnable, bShowStatusBar);
    ShowChatToggleMsg( bEnable );

	if(  !g_MyD3D->IsLocalPlayerObserver() )
	{
		m_pkShortCutBox->ToggleRender( false );
	}

    m_bHandleChatEvent = false;

    m_pkChatEdit->SetFixedWindowLocalPos( m_v2OrgChatEditPos );
    m_pkChatToggle->SetFixedWindowLocalPos( m_v2OrgChatEditPos );
}

void KGCGameOverlayUI::OnStartGame()
{
    // �� �ڽ��� �������̰ų� �ü� ��ȸ ��� �÷��� ���� ��, �� ���÷��� ��� �߿��� HUD�� �׸��� �ʴ´�.
    bool bShowHUD = !( g_MyD3D->IsLocalPlayerObserver() );
    EnableHUD( bShowHUD );

   // //Į��â �ʱ�ȭ
   // if ( g_MyD3D->MyReplay->m_iCurrState != RP_VIEW
   //     && g_MyD3D->MyReplay->m_iCurrState != RP_PREPARE )
   // {
   //     if( m_Color == 0xFFFFFFFF )
   //     {
   //         m_pkColorButton->ToggleRender( false );
   //         m_pkColorButton->SetSelfInputCheck( false );
   //         m_pkColorButtonFFFFFF->ToggleRender( false );
   //         m_pkColorButtonFFFFFF->SetSelfInputCheck( false );

			//if((g_kGlobalValue.m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCCLUB || g_kGlobalValue.m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCBLOG || g_kGlobalValue.m_kUserInfo.cIsNetmarblePCRoom ))
   //         {
			//	m_pkColorButton->InitState(true,true,NULL);
			//	m_pkColorButtonFFFFFF->InitState(true,true,this);
			//	//         m_pkColorButton->SetColor(0xFFFFFFFF);
			//	m_pkColorButtonFFFFFF->SetColor(0xFFFFFFFF);
			//	if (g_pkUIScene->m_pkColorDlg != NULL)
			//	{
			//		SetColor(g_pkUIScene->m_pkColorDlg->IndexToColorString(g_MyD3D->m_KGCOption.GetColorIndex()));
			//	}
			//	else
			//	{
			//		SetColor(L"0xFFFFFFFF");
			//	}
   //         }
   //     }
   // }
   // else
   // {
   //     m_pkColorButton->ToggleRender( false );
   //     m_pkColorButtonFFFFFF->ToggleRender( false );		
   // }

    // �ʱ�ȭ
    ShowChatToggleMsg( true );
    g_MyD3D->MyHead->IsCheatMode = false;
    SetEditText( NULL );
    ::SetFocus( g_hUIMainWnd );
}

void KGCGameOverlayUI::InitMinimap()
{
    m_pkMinimap->InitMinimap();
    m_pkMinimap->UpdateDescPos();
}

void KGCGameOverlayUI::EndMinimap()
{
    m_pkMinimap->ToggleRender(false);
}

void KGCGameOverlayUI::SetMiniMapArrowColor(int iRow, int iCol, int iDirection, bool bBright)
{
    if( !m_pkMinimap ) return;
    if( iDirection < 1 || iDirection > 4 ) return;

    for(int i=0;i<(int)m_pkMinimap->m_vtMinimapInfo.size();++i)
    {
        if( iRow == m_pkMinimap->m_vtMinimapInfo[i].iRow 
            && iCol == m_pkMinimap->m_vtMinimapInfo[i].iCol 
            )
        {
            m_pkMinimap->m_vtMinimapInfo[i].bArrowColor[iDirection-1] = bBright;
            break;
        }
    }
}

void KGCGameOverlayUI::StartQuestResult(void)
{
    m_pkSubjectList->ToggleRender(false);
    m_pkShortCutBox->ToggleRender(false);

    m_pkQuestResultS3->SetMoveBackground(GC_SCREEN_DIV_SIZE_INT(800), 0, GC_SCREEN_DIV_SIZE_INT(395), 0, 10, KGCQuestResult_S3::EANIM_SCENE_BACKGROUND);
    m_pkQuestResultS3->SetQuestResult();
    m_pkQuestResultS3->ToggleRender(true);    

    ShowZoomUI( false );
    ShowRealMiniMap( false );

    g_pkTutorialDlg->Disable();

    if( m_pkLimitedTime )
    {
        SetTimerEnable( false );
        m_pkLimitedTime->ToggleRender( false );
    }

    if( m_pkFoodGauge )
        m_pkFoodGauge->ToggleRender( false );
}

void KGCGameOverlayUI::EndQuestResult(void)
{
    m_pkQuestResultS3->ToggleRender(false);

    if ( m_pkEscortMonHPBar != NULL ) {
        m_pkEscortMonHPBar->ClearHPBar();
    }
}

void KGCGameOverlayUI::InitShowNumberEffect(KGCD3DWndEffect *pEffectInfo, KD3DWnd *pWindow)
{
    if( !pEffectInfo || !pWindow ) return;

    pWindow->SetSelfInputCheck(false);
    pWindow->ToggleRender(false);
    if( !m_bInitValue )
    {
        pEffectInfo->InitValue( (int)pWindow->GetFixedWindowLocalPos().x, (int)pWindow->GetFixedWindowLocalPos().y
            , (int)pWindow->GetWidth(), (int)pWindow->GetHeight() );
    }
    else
    {
        pWindow->SetFixedWindowLocalPos( pEffectInfo->GetOriginalPos() );
        pWindow->SetWidthDirect(pEffectInfo->GetOriginalWidth());
        pWindow->SetHeightDirect(pEffectInfo->GetOriginalHeight());
    }
}

void KGCGameOverlayUI::OnDestroy( void )
{
    ShortCutSlotClear();
    m_bTimerStart = false;
}

bool KGCGameOverlayUI::IsEndRouletteDiceTimer( void )
{
    for(int i=0;i<REBIRTH_COUNT_DOWN;++i)
        m_pkCountDown[i]->ToggleRender(false);

    if( m_stTimer.IsComplete() )
        return true;

    const int iCurCnt = (int)(m_stTimer.GetCurValue() / 1000);

    if( iCurCnt >= 0 && iCurCnt < REBIRTH_COUNT_DOWN )
    {
        if( m_stCountDown[iCurCnt].IsEndOfShow() )
        {
            for(int i=0;i<REBIRTH_COUNT_DOWN;++i) m_stCountDown[i].SetComplete();
            SetShowNumberEffect(iCurCnt, (int)(GC_FPS_LIMIT/2.0f), GC_FPS_LIMIT, 300, 100);
        }
        FrameMove_NumberEffect(&m_stCountDown[iCurCnt], m_pkCountDown[iCurCnt]);
    }

    m_stTimer.Step();

    return false;
}

void KGCGameOverlayUI::SetShowNumberEffect( int iNumber, int iScaleFrame, int iShowFrame, int iStartScalePercent, int iDestScalePercent )
{
    KGCD3DWndEffect     *pEffect        = NULL;
    KD3DWnd             *pkWindow       = NULL;

    if( iNumber >= 0 && iNumber < REBIRTH_COUNT_DOWN )
    {
        pEffect                 = &m_stCountDown[iNumber];
        pkWindow                = m_pkCountDown[iNumber];
    }

    if( pEffect && pkWindow )
    {
        pEffect->SetFramePerAlterationScale(iScaleFrame, iShowFrame, iStartScalePercent, iDestScalePercent);
        pEffect->SetFramePerAlterationBlend(iScaleFrame, iShowFrame, 255, 0);

        // ó�� ���� �� ����
        pkWindow->SetWidthDirect((DWORD)pEffect->GetCurrentWidth());
        pkWindow->SetHeightDirect((DWORD)pEffect->GetCurrentHeight());

        // �������� ����ʿ� ���� UI�� ��ġ�� ���ݾ� �����Ǿ� ��... 
        const float fChangeX = (float)(pEffect->GetCurrentWidth()/2 - pEffect->GetOriginalWidth()/2);
        const float fChangeY = (float)(pEffect->GetCurrentHeight()/2 - pEffect->GetOriginalHeight()/2);
        pkWindow->SetWindowPosDirect(pEffect->GetOriginalPos() - D3DXVECTOR2(fChangeX, fChangeY));
    }
}

void KGCGameOverlayUI::FrameMove_NumberEffect( KGCD3DWndEffect *pEffectInfo, KD3DWnd *pWindow )
{
    if( !pEffectInfo || !pWindow ) return;

    // ǥ�� ���θ� Ȯ���ϴ� �κ�
    if( !pEffectInfo->IsEndOfShow() )
    {
        pEffectInfo->ShowStep();

        // �������� �����ϴ� �κ�
        if( !pEffectInfo->IsEndOfScale() )
        {
            pEffectInfo->ScaleStep();

            // �������� ����ʿ� ���� UI�� ��ġ�� ���ݾ� �����Ǿ� ��... 
            const float fChangeX = (float)(pEffectInfo->GetCurrentWidth()/2 - pEffectInfo->GetOriginalWidth()/2);
            const float fChangeY = (float)(pEffectInfo->GetCurrentHeight()/2 - pEffectInfo->GetOriginalHeight()/2);

            pWindow->SetWindowPosDirect(pEffectInfo->GetOriginalPos() - D3DXVECTOR2(fChangeX, fChangeY));
            pWindow->SetWidthDirect((int)pEffectInfo->GetCurrentWidth());
            pWindow->SetHeightDirect((int)pEffectInfo->GetCurrentHeight());
            pWindow->SetWindowColor(D3DCOLOR_ARGB( 123, 0xff, 0xff, 0xff));
        }
        else
        {
            pWindow->SetWindowColor(D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff));
        }
        pWindow->ToggleRender( (pEffectInfo->GetCurrentWidth() < 1000) );
    }
    else if( !pEffectInfo->IsEndOfBlend() )
    {
        pEffectInfo->BlendStep();
    }

    if( pEffectInfo->IsEndOfShow() )
    {
        pWindow->ToggleRender(true);
    }
}

void KGCGameOverlayUI::RenderContinueBox( bool bRender, bool bShowWait /*= false */ )
{
#if defined(RING_OF_FRIENDSHIP_UNLIMIT_REBIRTH)
	m_pkContinueBox->ToggleRender( false );
	m_pkContinueBox2->ToggleRender( false );

	if( g_MyD3D->GetMyPlayer()->m_kUserInfo.bLive == true && (g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING ) 
		|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_ELESIS )
		|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_LIR )
		|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_ARME )
		|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_LAS )
		|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_RYAN )
		|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_RONAN )
		|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_AMY ) )
		)
	{
		m_pkContinueBox2->ToggleRender( bRender );
	}
	else
#endif
	{
		m_pkContinueBox->ToggleRender( bRender );
		m_pkContinueHelp->ToggleRender( bRender && !bShowWait );
		//m_pkContinueWait->ToggleRender( bRender && bShowWait );

		if( bRender == true )
		{
			// [12/18/2007] breadceo. ���� ���ʽ� ������ �ͼ� Set
            int nBonus = SiKGCElvis()->GetCurrentBonusPoint();
            if ( nBonus > MAX_BONUS_NUM )
            {
                nBonus = MAX_BONUS_NUM;
            }

			m_pkBonus->SetScore( nBonus );
		}
	}
}

void KGCGameOverlayUI::ChangeShortCutSlotState( bool _bHide )
{
    m_pkShortCutBox->ToggleShowState();
}
void KGCGameOverlayUI::ShortCutSlotClear()
{
    m_pkShortCutBox->ResetBoxPos();
    m_pkShortCutBox->ResetEmotPos();
}
void KGCGameOverlayUI::DecShortCutShowTime()
{
    if( m_iShortCutShowTime > 0 )
    {
        m_iShortCutShowTime--;

        if( m_iShortCutShowTime == 0 )
            ChangeShortCutSlotState();
    }
}

void KGCGameOverlayUI::FrameMove_Minimap(void)
{
    if( m_dwRemainFogTime > 0 )
    {
        --m_dwRemainFogTime;
        if( m_dwRemainFogTime <= 0 )
        {
            m_pkMinimap->ToggleRender(false);
        }
    }
}

void KGCGameOverlayUI::SetTimerMinimap(DWORD dwFrame)
{
    if( m_pkMinimap->IsRenderOn() || m_pkDialogueBoxRight->IsRenderOn() ) return;

    m_pkMinimap->ToggleRender(true);
    m_pkMinimap->SetPlayerRankInfo();
    m_dwRemainFogTime = dwFrame;
}

void KGCGameOverlayUI::SetWaiting( bool bIsWaiting )
{
    m_bIsWaiting = bIsWaiting;
}

void KGCGameOverlayUI::SetFocus( void )
{
    // [8/12/2007 breadceo] ������ �����ؼ� �ƹ��͵� ����.
}

void KGCGameOverlayUI::RenderCountNumber()
{
    D3DXMATRIX mat;
    if( m_fCountNumberScale != 1.0f) 
    {
        D3DXMatrixIdentity( &mat );
        D3DXMatrixScaling ( &mat,m_fCountNumberScale,m_fCountNumberScale,m_fCountNumberScale);
        g_pd3dDevice->SetTransform( D3DTS_WORLD, &mat);
    }

    if( m_iCountNumber >= 0 )
        g_pkUIMgr->RenderNumber("white_num", D3DXVECTOR2(380.0f * g_pGCDeviceManager->GetWindowScaleX() * GC_SCREEN_DIV_WIDTH, 280.0f * g_pGCDeviceManager->GetWindowScaleY() * GC_SCREEN_DIV_HEIGHT ), m_iCountNumber , D3DCOLOR_ARGB( 0xff,0xff,0xff,0xff), true, true, -1.0f,true );

    if( m_fCountNumberScale != 1.0f) 
    {
        D3DXMatrixIdentity( &mat );
        g_pd3dDevice->SetTransform( D3DTS_WORLD, &mat );
    }
}

void KGCGameOverlayUI::StartNumberCount( int iNumber, float fScale )
{
    m_iCountNumber = iNumber;
    m_iCountFrame  = 0;
    m_fCountNumberScale = fScale;
}

bool KGCGameOverlayUI::IsNumberCountEnd()
{
    return m_iCountNumber <= -1;
}

void KGCGameOverlayUI::ToggleShortCutBox( bool bToggle_ )
{
    m_pkShortCutBox->SetShowState(bToggle_?SFS_FADE_IN:SFS_FADE_OUT);
}


void KGCGameOverlayUI::UpdateBossList()
{
    // ���� ���� �ֳ� Ȯ���� ���� 
    DeleteBossList();
    // ���� HP �������� 
	
    for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER *pMonster = mit->second;
		if( !pMonster->m_bDie && pMonster->m_bLive && 1 == pMonster->m_iBossMonster && pMonster->m_fHP > 0.0f)
		{
			g_pkGameOverlayUI->AddBossMonster(pMonster);
		}	
	}

}

MONSTER * KGCGameOverlayUI::GetBoss()
{
    if(this->m_vecBossList.empty())
        return NULL;
    return m_vecBossList[0];
}

void KGCGameOverlayUI::SetLoadingTextureColor( D3DCOLOR color )
{
	//m_pkFullScreenWhite->ForceSetWindowColor( color );
}

void KGCGameOverlayUI::SetEclipseTextureColor( D3DCOLOR color )
{
    /*if( m_pkFullScreenEclipse ) {
        m_pkFullScreenEclipse->ForceSetWindowColor( color );
    }    */
}

void KGCGameOverlayUI::OnChatBtn( const KActionEvent& event_ )
{
    if ( event_.m_dwlParam == KGCCheckButton::GC_CBUTTON_CHK )
    {
        if ( event_.m_pWnd == m_pkWhisperBtn )
        {
            m_pkChatList->SetFilterInfo(KGCChatManager::CHAT_TYPE_WHISPER, !m_pkWhisperBtn->IsChecked());
        }
        else if ( event_.m_pWnd == m_pkGuildBtn )
        {
            m_pkChatList->SetFilterInfo(KGCChatManager::CHAT_TYPE_GUILD, !m_pkGuildBtn->IsChecked());
        }
        else if ( event_.m_pWnd == m_pkGeneralBtn )
        {
            m_pkChatList->SetFilterInfo(KGCChatManager::CHAT_TYPE_NORMAL, !m_pkGeneralBtn->IsChecked());
        }
        else if ( event_.m_pWnd == m_pkAllBtn )
        {
            m_pkWhisperBtn->Check(m_pkAllBtn->IsChecked());
            m_pkGeneralBtn->Check(m_pkAllBtn->IsChecked());
            if( g_kGlobalValue.m_kGuildUserInfo.m_dwGuildUID != 0 )
                m_pkGuildBtn->Check(m_pkAllBtn->IsChecked());

            m_pkChatList->SetFilterInfo(KGCChatManager::CHAT_TYPE_WHISPER,!m_pkAllBtn->IsChecked());
            if( g_kGlobalValue.m_kGuildUserInfo.m_dwGuildUID != 0 )
                m_pkChatList->SetFilterInfo(KGCChatManager::CHAT_TYPE_GUILD, !m_pkAllBtn->IsChecked());
            m_pkChatList->SetFilterInfo(KGCChatManager::CHAT_TYPE_NORMAL, !m_pkAllBtn->IsChecked());
        }
    }
    else
    {        
        if ( event_.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        {
			KGCChatManager::EChatCommand  eChatCmd = KGCChatManager::CHAT_CMD_ERROR;

			if ( event_.m_pWnd == m_pkWhisperBtn )
			{
				eChatCmd = KGCChatManager::CHAT_CMD_WHISPER;
			}
			else if ( event_.m_pWnd == m_pkGuildBtn )
			{
				eChatCmd = KGCChatManager::CHAT_CMD_GUILD;
			}
			else if ( event_.m_pWnd == m_pkGeneralBtn )
			{
				eChatCmd = KGCChatManager::CHAT_CMD_NORMAL;
			}
			else if ( event_.m_pWnd == m_pkAllBtn )
			{
				eChatCmd = KGCChatManager::CHAT_CMD_NORMAL;
			}


			KGCCheckButton* chkBtn = static_cast<KGCCheckButton*>(event_.m_pWnd);

			if ( chkBtn != m_pkShowBtn && eChatCmd != KGCChatManager::CHAT_CMD_ERROR )
			{
				// ��ġ �ٲ��� ��
				D3DXVECTOR2 vecTemp = chkBtn->GetFixedWindowLocalPos();
				chkBtn->SetWindowPosDirect( m_pkShowBtn->GetFixedWindowLocalPos() );
				m_pkShowBtn->SetWindowPosDirect( vecTemp );

				// �ٲ��� �������� �߿���
				m_pkShowBtn = static_cast<KGCCheckButton*>(chkBtn);

				SetEditFocus();
				//������ ��ȣ�� �־��ش�.
				m_pkChatEdit->SetText( eChatCmd == KGCChatManager::CHAT_CMD_WHISPER ? L">":L"",false );
				SetChatEventHandling(true);
				ShowChatToggleMsg(false);
				g_MyD3D->MyHead->IsCheatMode = true;

				//HandleChatEvent();
			}

			// ������ ����!
			m_bChatBtnFold = !m_bChatBtnFold;
			FoldingChatBox( m_bChatBtnFold );
			// �Ф���
			m_pkShowBtn->ToggleRender( true );

			g_KDSound.Play("72");
			g_pkChatManager->SetCommend(eChatCmd);
			return;

        }
    }
}

void KGCGameOverlayUI::FoldingChatBox( bool bFold_ )
{
    m_pkAllBtn->ToggleRender( !bFold_ );
    m_pkWhisperBtn->ToggleRender( !bFold_ );

    if( g_kGlobalValue.m_kGuildUserInfo.m_dwGuildUID == 0 )
        m_pkGuildBtn->Lock(true);
    else
        m_pkGuildBtn->Lock(false);
    m_pkGuildBtn->ToggleRender( !bFold_ );

    m_pkGeneralBtn->ToggleRender( !bFold_ );
}

std::wstring KGCGameOverlayUI::GetCurrentPreMark()
{
    std::map< std::string, std::wstring>::iterator mit = m_mapPreMark.find( m_pkShowBtn->GetWindowName() );
    std::wstring strReturn = L"";

    if ( m_mapPreMark.end() != mit )
    {
        strReturn = mit->second;
    }

    return strReturn;
}

void KGCGameOverlayUI::FadeInOrOut( bool bFadein_ )
{    
#if defined(USE_STAGE_LOAD_COMPLETE_SIGN)
    m_bFadeIn = bFadein_;

    if( m_bFadeIn == false )
    {
        m_pkMinimap->ToggleRender(false);

        // ȥ�� �÷����� ���� ������ �ʿ䰡 ����.
        if( g_MyD3D->GetPlayerNum() > 1 )
        {
            m_kStageLoadCompleteStopWatch.SetStopWatch(600000);
            m_pkStageLoadCompleteSign->ToggleRender(true);
        }
        else
            m_pkStageLoadCompleteSign->ToggleRender(false);
    }    
#else        
    m_bFadeIn = bFadein_;
#endif
}

bool KGCGameOverlayUI::IsRenderOnDramaDlg()
{
    if( m_pkDialogueBoxLeft->IsRenderOn() || m_pkDialogueBoxRight->IsRenderOn() )
        return true;
    else
        return false;
}

void KGCGameOverlayUI::PushMissionProcessStr( const std::wstring& strMissionProcess )
{
    if( m_dwMissionProcessShowCount == MISSION_PROCESS_LIST_NUM )
    {
        PopMissionProcessStr();
    }
    m_dwMissionProcessShowTime[m_dwMissionProcessShowCount] = 110;

    std::wstringstream stm;
    if ( SiGCAdventureManager()->isValidEventPeriod() && SiGCAdventureManager()->GetPossibleObtainStr() ) {
        stm<< "#CFF0000"<<strMissionProcess<<L"#CX";
        SiGCAdventureManager()->SetPrintObtainStr( false );
    }
    else
    stm<< "#CFFFF00"<<strMissionProcess<<L"#CX";

#if defined( NATION_USA ) || defined( NATION_EU )
    m_pkMissionProcessList[m_dwMissionProcessShowCount]->SetFontSize( SiKGCMultipleLanguages()->GetLargeFontSize() );
#else
	m_pkMissionProcessList[m_dwMissionProcessShowCount]->SetFontSize( SiKGCMultipleLanguages()->GetTitleFontSize() );
#endif
	m_pkMissionProcessList[m_dwMissionProcessShowCount]->SetFontOutline( true );
    m_pkMissionProcessList[m_dwMissionProcessShowCount]->SetText( stm.str() );
    m_pkMissionProcessList[m_dwMissionProcessShowCount]->ToggleRender(true);
    ++m_dwMissionProcessShowCount;
}

void KGCGameOverlayUI::FrameMove_MissionProcess()
{
    for( int i = 0 ; i < MISSION_PROCESS_LIST_NUM ; ++i )
    {
        if( m_dwMissionProcessShowCount == 0 )
            break;

        if( m_dwMissionProcessShowCount == i )
            break;

        if( m_dwMissionProcessShowTime[i] == 0 )
        {
            PopMissionProcessStr();
        }
        else
            continue;
    }
    for( int i = 0 ; i < MISSION_PROCESS_LIST_NUM ; ++i )
    {
        if( m_dwMissionProcessShowTime[i] > 0 )
            --m_dwMissionProcessShowTime[i];
    }
}

void KGCGameOverlayUI::PopMissionProcessStr()
{
    for( int i = 0 ; i < (int)m_dwMissionProcessShowCount && i < MISSION_PROCESS_LIST_NUM -1 ; ++i )
    {
        m_dwMissionProcessShowTime[i] = m_dwMissionProcessShowTime[i+1];
        m_pkMissionProcessList[i]->SetText( m_pkMissionProcessList[i+1]->GetOriginalText() );
    }

    m_pkMissionProcessList[m_dwMissionProcessShowCount-1]->ToggleRender( false );
    m_pkMissionProcessList[m_dwMissionProcessShowCount-1]->SetText();
    m_dwMissionProcessShowTime[m_dwMissionProcessShowCount-1] = 0;

    if( m_dwMissionProcessShowCount > 0 )
        --m_dwMissionProcessShowCount;
}

void KGCGameOverlayUI::OnEnterScene()
{
    g_pkUIScene->m_pkOnGameDlg->Create();
    g_pkUIScene->m_pkOnGameDlg->ToggleRender( true );
    g_pkUIScene->m_pkOnGameDlg->Initialize();
    g_pkUIScene->EnableMessageBox( false );
}

// ���� ���ӽ��� �� �ڽ��� �ε�������¸� ����
void KGCGameOverlayUI::DungeonMyLoadingState( int iLoadingStete_ )
{
    m_pkDungeonLoadingUserState[0]->SetLoadingState( g_kGlobalValue.m_kUserInfo.dwUID, iLoadingStete_ );
    m_pkDungeonLoadingUserState[0]->ToggleRender(true);

    if( iLoadingStete_ == 1 )
    {
        int cnt = 1;
        std::map<DWORD,int>::iterator mit  = m_mapLoadingMembers.begin();
        for( ; mit != m_mapLoadingMembers.end(); ++mit )   
        {
            if( cnt > MAX_DUNGEON_PLAYER_NUM - 1 )
                break;

            if( SiKGCRoomManager()->IsPlayerObserver(mit->first) )
                continue;

            m_pkDungeonLoadingUserState[cnt]->ToggleRender(true);
            m_pkDungeonLoadingUserState[cnt]->SetLoadingState( mit->first, mit->second );
            cnt++;
        }
    }    
}

// �������ӽ��� �� ������� �� ������¸� ����
void KGCGameOverlayUI::DungeonMembersLoadingState( std::pair<DWORD,int> prMemberInfo_ )
{ 
    std::map<DWORD, int>::iterator mit = m_mapLoadingMembers.find(prMemberInfo_.first);

    if( mit != m_mapLoadingMembers.end() )
        m_mapLoadingMembers[prMemberInfo_.first] = prMemberInfo_.second;


    mit = m_mapLoadingMembers.begin();
    int cnt = 1;

    for(; mit != m_mapLoadingMembers.end(); ++mit )
    {        
        if( cnt >= MAX_DUNGEON_PLAYER_NUM )
            break;

        if( SiKGCRoomManager()->IsPlayerObserver(mit->first) )
            continue;

        m_pkDungeonLoadingUserState[cnt]->SetLoadingState( mit->first, mit->second );
        cnt++;
    }
}

void KGCGameOverlayUI::SetDungeonLoadingMembers( std::vector<DWORD> vecUserUID_ )
{
    m_mapLoadingMembers.clear();
    for( int i = 0; i < (int)vecUserUID_.size(); i++ )
    {
        if( vecUserUID_[i] == g_kGlobalValue.m_kUserInfo.dwUID )
            continue;
        m_mapLoadingMembers.insert(std::map<DWORD,int>::value_type(vecUserUID_[i], 1));        
    }

    int iPlayerNum = m_mapLoadingMembers.empty() ? 0 : static_cast<int>( m_mapLoadingMembers.size() );
    m_pkLoadingStateBack->ToggleRender( true );
    m_pkLoadingStateBack->SetHeight( LOADING_STATE_BACK_MIN + ( LOADING_STATE_BACK_OFFSET * iPlayerNum ) );
}   

void KGCGameOverlayUI::RenderDungeonLoadingState( bool bRender_ )
{
    for( int i = 0; i < MAX_DUNGEON_PLAYER_NUM; i++ )
    {
        m_pkDungeonLoadingUserState[i]->ToggleRender(bRender_);
    }
}

void KGCGameOverlayUI::SetPlayingGameUserPing( std::vector<std::pair<DWORD,DWORD>>& vecRoomMemberPingInfo_ )
{
    m_pkHeadUpDisplay->SetRoomMemberPingInfo(vecRoomMemberPingInfo_);
}

void KGCGameOverlayUI::CheckStageLoadComplete( DWORD dwUID_ )
{
    m_pkStageLoadCompleteSign->CheckPlayerLoadComplete(dwUID_);    
}

void KGCGameOverlayUI::CheckAllPlayerLoadComplete()
{
    m_pkStageLoadCompleteSign->CheckAllPlayerLoadComplete();
    m_kStageLoadCompleteStopWatch.SetStopWatch(300);
}

void KGCGameOverlayUI::InitStageLoadCompleteInfo()
{
    m_pkStageLoadCompleteSign->InitInfo();
}

void KGCGameOverlayUI::SetLoadCompleteUserUID( DWORD dwUID_ )
{
    m_pkStageLoadCompleteSign->SetUserInfo(dwUID_);
}

void KGCGameOverlayUI::RenderStageLoadComplete( bool bRender_ )
{
    m_pkStageLoadCompleteSign->ToggleRender(bRender_);
}

void KGCGameOverlayUI::ShowInvenKey( bool bShow_ )
{
#if defined( USE_ONGAME_INVENTORY )
    std::string strParticle = "UI_NewEquip_02";

    bool bRender = m_pkInventoryKey->IsRenderOn();

    m_pkInventoryKey->ToggleRender( bShow_ );

    if ( bShow_ && !bRender ) 
    {
        //  ��ƼŬ�� �ѷ�����.
        D3DXVECTOR2 vPos = m_pkInventoryKey->GetFixedWindowPos();

        vPos.x += ( m_pkInventoryKey->GetWidth() / 2 ) + 1;
        vPos.y += ( m_pkInventoryKey->GetHeight() / 2 ) + 1;

        vPos.x = (vPos.x - 400.0f)/ 400.0f;
        vPos.y = (-vPos.y + 300.0f)/ 400.0f;

        if ( m_pHotEquipParticle ) 
        {
            g_ParticleManager->DeleteSequence( m_pHotEquipParticle );
            m_pHotEquipParticle = NULL;
        }


        m_pHotEquipParticle = g_ParticleManager->CreateSequence( strParticle, vPos.x, vPos.y, 0.5f );
        g_ParticleManager->SetLayer ( m_pHotEquipParticle, GC_LAYER_UI );   
    }
    else if ( !bShow_ )
    {
        g_ParticleManager->DeleteSequence( m_pHotEquipParticle );
        m_pHotEquipParticle = NULL;
    }

#endif
}

void KGCGameOverlayUI::ResetComboSkill()
{
    if( m_pkHeadUpDisplay && m_pkHeadUpDisplay->m_pMyHeadup && m_pkHeadUpDisplay->m_pMyHeadup->m_pGameSkillBox )
    {
        m_pkHeadUpDisplay->m_pMyHeadup->m_pGameSkillBox->ResetComboSkill();
    }
}

void KGCGameOverlayUI::ShowEscortMerryDesc( int iStringID_, int iShowFrame_ )
{
    if( m_pkEscortMerryDesc ) {
        m_pkEscortMerryDesc->ShowEscortMerryDesc( iStringID_, iShowFrame_ );
    }
}

void KGCGameOverlayUI::ShowEscortMonHPBar( bool bShow_ )
{
    m_pkEscortMonHPBar->ShowHPBar( bShow_ );    
}

void KGCGameOverlayUI::SetEscortMonHPBar( float fMaxHP_, float fCurHP_ )
{
    if( m_pkEscortMonHPBar ) {
        m_pkEscortMonHPBar->ShowHPBar( true );
        m_pkEscortMonHPBar->SetHPBar( fMaxHP_, fCurHP_ );
    }
}

void KGCGameOverlayUI::SetEscortMonHPBarWarning( void )
{
    if( m_pkEscortMonHPBar ) { 
        m_pkEscortMonHPBar->SetWarning();
    }
}

void KGCGameOverlayUI::ShowAddtionGuide(int iShowFrame_ /* = 275 */)
{
	if (m_pkAddtionGuide)
		m_pkAddtionGuide->ShowAddtionGuide(iShowFrame_);
}

void KGCGameOverlayUI::RegisterLuaBind()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KGCGameOverlayUI>( L, "KGCGameOverlayUIClass" );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "ToggleDialogUI", 		    &KGCGameOverlayUI::ToggleDialogUI  );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "SetPosDialogUI", 		    &KGCGameOverlayUI::SetPosDialogUI  );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "SetAlphaDialogUI", 	    &KGCGameOverlayUI::SetAlphaDialogUI  );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "SetPosAnimDialogUI", 	    &KGCGameOverlayUI::SetPosAnimDialogUI  );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "SetAlphaAnimDialogUI", 	&KGCGameOverlayUI::SetAlphaAnimDialogUI  );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "ClearAnimUI",          	&KGCGameOverlayUI::ClearAnimUI  );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "SetShowPlayTime", 	    &KGCGameOverlayUI::SetShowPlayTime  );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "SetFoodGauge", 	        &KGCGameOverlayUI::SetFoodGauge  );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "UpdateFoodGauge", 	    &KGCGameOverlayUI::UpdateFoodGauge  );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "SetSayBallonMsg", 	    &KGCGameOverlayUI::SetSayBallonMsg  );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "DeleteSayBallonMsg", 	    &KGCGameOverlayUI::DeleteSayBallonMsg  );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "IsShowDialogue", 	        &KGCGameOverlayUI::IsShowDialogue  );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "SetTimer", 	            &KGCGameOverlayUI::SetTimer  );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "IsTimerEnable", 	        &KGCGameOverlayUI::IsTimerEnable );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "GetLimitedTime", 	        &KGCGameOverlayUI::GetLimitedTime );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "GetInGameMessage", 	    &KGCGameOverlayUI::GetInGameMessage );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "GetCommandGuid", 	        &KGCGameOverlayUI::GetCommandGuid );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "BindBossHPUpdate", 	    &KGCGameOverlayUI::BindBossHPUpdate );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "DeleteBossList", 	        &KGCGameOverlayUI::DeleteBossList );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "RenderOffHPBar", 	        &KGCGameOverlayUI::RenderOffHPBar );
    lua_tinker::class_def<KGCGameOverlayUI>( L,  "StartBossAlram", 	        &KGCGameOverlayUI::StartBossAlram );
	lua_tinker::class_def<KGCGameOverlayUI>( L,  "EnableHUD",				&KGCGameOverlayUI::EnableHUD);

    lua_tinker::decl( L, "KGCGameOverlayUI", this );

    m_pkInGameMessage->RegisterLuaBind();
    m_pkCommandGuid->RegisterLuaBind();

    GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), "DungeonDramaFunc.lua", true );
}

void KGCGameOverlayUI::ToggleDialogUI( char *strUI, bool bShow )
{
    std::map<std::string, KD3DWnd*>::iterator mapIter = m_mapDialogUI.find( strUI );
    if( mapIter != m_mapDialogUI.end() )
    {
        if( mapIter->second )
        {
            mapIter->second->ToggleRender( bShow );
        }
    }
}

void KGCGameOverlayUI::SetPosDialogUI( char *strUI, float fX, float fY )
{
    std::map<std::string, KD3DWnd*>::iterator mapIter = m_mapDialogUI.find( strUI );
    if( mapIter != m_mapDialogUI.end() )
    {
        if( mapIter->second )
        {
            mapIter->second->SetWindowPosDirect( D3DXVECTOR2(fX * GC_SCREEN_DIV_WIDTH , fY * GC_SCREEN_DIV_HEIGHT) );
        }
    }
}

void KGCGameOverlayUI::SetAlphaDialogUI( char *strUI, int iAlpha )
{
    std::map<std::string, KD3DWnd*>::iterator mapIter = m_mapDialogUI.find( strUI );
    if( mapIter != m_mapDialogUI.end() )
    {
        if( mapIter->second )
        {
            mapIter->second->SetAllWindowAlpha( iAlpha );
        }
    }
}

void KGCGameOverlayUI::SetPosAnimDialogUI( char *strUI, float fStartX, float fStartY, float fEndX, float fEndY, float fSpeedX, float fSpeedY )
{
    std::map<std::string, KD3DWnd*>::iterator mapIter = m_mapDialogUI.find( strUI );
    if( mapIter != m_mapDialogUI.end() )
    {
        if( mapIter->second )
        {
            DIALOG_ANIM_POS sDialogAnimPos;
            sDialogAnimPos.fStartX = fStartX * GC_SCREEN_DIV_WIDTH;
            sDialogAnimPos.fStartY = fStartY * GC_SCREEN_DIV_HEIGHT;
            sDialogAnimPos.fEndX = fEndX * GC_SCREEN_DIV_WIDTH;
            sDialogAnimPos.fEndY = fEndY * GC_SCREEN_DIV_HEIGHT ;
            sDialogAnimPos.fSpeedX = fSpeedX;
            sDialogAnimPos.fSpeedY = fSpeedY;

            m_mapDialogUIAnimPos.insert( std::make_pair(mapIter->second, sDialogAnimPos) );
        }
    }
}

void KGCGameOverlayUI::SetAlphaAnimDialogUI( char *strUI, int iStartAlpha, int iEndAlpha, int iSpeed )
{
    std::map<std::string, KD3DWnd*>::iterator mapIter = m_mapDialogUI.find( strUI );
    if( mapIter != m_mapDialogUI.end() )
    {
        if( mapIter->second )
        {
            if( 0 > iStartAlpha )   iStartAlpha = 0;
            if( 255 < iStartAlpha ) iStartAlpha = 255;
            if( 0 > iEndAlpha )     iEndAlpha = 0;
            if( 255 < iEndAlpha )   iEndAlpha = 255;

            DIALOG_ANIM_ALPHA sDialogAnimAlpha;
            sDialogAnimAlpha.iStartAlpha = iStartAlpha;
            sDialogAnimAlpha.iEndAlpha = iEndAlpha;
            sDialogAnimAlpha.iSpeed = iSpeed;

            m_mapDialogUIAnimAlpha.insert( std::make_pair(mapIter->second, sDialogAnimAlpha) );
        }
    }
}

void KGCGameOverlayUI::ClearAnimUI( char *strUI )
{
    std::map<std::string, KD3DWnd*>::iterator mapIter = m_mapDialogUI.find( strUI );
    if( mapIter != m_mapDialogUI.end() )
    {
        if( mapIter->second )
        {
            std::map<KD3DWnd*, DIALOG_ANIM_ALPHA>::iterator mapAlphaIter = m_mapDialogUIAnimAlpha.find( mapIter->second );
            if( mapAlphaIter != m_mapDialogUIAnimAlpha.end() )
                m_mapDialogUIAnimAlpha.erase( mapAlphaIter );

            std::map<KD3DWnd*, DIALOG_ANIM_POS>::iterator mapPosIter = m_mapDialogUIAnimPos.find( mapIter->second );
            if( mapPosIter != m_mapDialogUIAnimPos.end() )
                m_mapDialogUIAnimPos.erase( mapPosIter );
        }
    }
}

bool KGCGameOverlayUI::IsRenderCartoonDrama()
{
    std::map<std::string, KD3DWnd*>::iterator mit = m_mapDialogUI.begin();
    for( mit; mit != m_mapDialogUI.end(); ++mit ) {
        if( mit->second->IsRenderOn() ) {
            return true;
        }
    }

    return false;
}

void KGCGameOverlayUI::SetFoodGauge( int iMaxFood, int iFoodEnum, int iFoodType, int iGetFoodEnum, int iGetFoodType )
{
    if( m_pkFoodGauge )
    {
        m_pkFoodGauge->SetFoodGauge( iMaxFood, iFoodEnum, iFoodType, iGetFoodEnum, iGetFoodType );
        m_pkFoodGauge->ToggleRender( true );
    }
}

void KGCGameOverlayUI::UpdateFoodGauge()
{
    if( m_pkFoodGauge )
    {
        m_pkFoodGauge->UpDateGauge();
    }
}

void KGCGameOverlayUI::SetSayBallonMsg( int iStringID, int iUnitIndex, float fOffsetX, float fOffsetY, int iArrowType, UCHAR cR, UCHAR cG, UCHAR cB )
{
    if( NULL == m_pkSayBalloon )
        return;

    DWORD dwColor = D3DXCOLOR( cR/255.0f, cG/255.0f, cB/255.0f, 1.0f);
    m_pkSayBalloon->NewUserMsg( g_pkStrLoader->GetString( iStringID ), iUnitIndex, KChatData::MSG_COMMON, dwColor );
    m_pkSayBalloon->SetOffset( iUnitIndex, fOffsetX, fOffsetY );
    m_pkSayBalloon->SetArrowDist( iUnitIndex, iArrowType );
}

// ī�޶� �� �����ϸ� ��ǳ�� ��ġ �̻��� ���ϴ�.
// �������߽��ϴ�. �ʿ��ϸ� �����ϼ���~
void KGCGameOverlayUI::ReLocateSayBallonMsg()
{
    if( NULL == m_pkSayBalloon )
        return;

    bool bToggle = false;
    std::vector<DWORD> vecDeleteList;
    std::vector<DWORD> vecUIDList = m_pkSayBalloon->GetUserUIDList();
    std::vector<DWORD>::iterator vecIter = vecUIDList.begin();
    for(; vecIter != vecUIDList.end(); ++vecIter)
    {
        D3DXVECTOR2 vPos(0.0f, 0.0f);
        if( g_MyD3D->IsPlayerIndex( (*vecIter) ) )
        {
            PLAYER* pPlayer = g_MyD3D->MyPlayer[ (*vecIter) ];
            if( pPlayer )
            {
                if( pPlayer->IsDie() )
                    vecDeleteList.push_back( (*vecIter) );
                else
                {
                    D3DXVECTOR3 v3Pos(0.0f, 0.0f, 0.0f);
                    D3DXMATRIX matBasic;
                    pPlayer->Get_BasicMatrix( &matBasic, true, true, false );
                    D3DXVec3TransformCoord( &v3Pos, &D3DXVECTOR3(0,0,0), &matBasic );

                    D3DXVECTOR2 vOffset = m_pkSayBalloon->GetOffset( (*vecIter) );

                    v3Pos.x += vOffset.x;
                    v3Pos.y += vOffset.x;

                    vPos.x = ( v3Pos.x + 1.0f ) * 400.0f + 0.5f;
                    vPos.y = ( -v3Pos.y + 0.75f ) * 400.0f + 0.5f;
                }
            }
        }
        else if( g_kMonsterManager.IsMonsterIndex( (*vecIter) - MAX_PLAYER_NUM ) )
        {
            MONSTER *pMonster = g_kMonsterManager.GetMonster( (*vecIter) - MAX_PLAYER_NUM );
            if( pMonster )
            {
                if( pMonster->IsDie() )
                    vecDeleteList.push_back( (*vecIter) );
                else
                {
                    int iIndex = 0;
                    D3DXVECTOR3 v3Pos( 0.0f, 0.0f, 0.0f );
                    D3DXVECTOR2 vOffset = m_pkSayBalloon->GetOffset( (*vecIter) );
                    v3Pos.x = (pMonster->m_afX[iIndex] + vOffset.x) - g_kCamera.m_fLookX * (g_kCamera.m_fWideMode/10.0f);
                    v3Pos.y = (pMonster->m_afY[iIndex] + vOffset.y) - (g_kCamera.m_fLookY * (g_kCamera.m_fWideMode/10.0f));

                    // world coordinates -> screen coordinates
                    vPos.x = int( (( v3Pos.x + 1.0f ) * 400.f) + 0.5f ) * GC_SCREEN_DIV_WIDTH;
                    vPos.y = int( (( -v3Pos.y + 0.75f ) * 400.f) + 0.5f ) * GC_SCREEN_DIV_HEIGHT;
                }
            }
        }

        if( vPos != D3DXVECTOR2(0.0f, 0.0f) )
        {
            bToggle = true;
            m_pkSayBalloon->SetBalloonPos(vPos, (*vecIter), static_cast<KGCSayBalloonSquare::EArrowPos>(m_pkSayBalloon->GetArrowDist( (*vecIter) )) );
        }
    }

    m_pkSayBalloon->ToggleRender( bToggle );

    vecIter = vecDeleteList.begin();
    for(; vecIter != vecDeleteList.end(); ++vecIter)
    {
        DeleteSayBallonMsg( (*vecIter) );
    }
}

void KGCGameOverlayUI::DeleteSayBallonMsg( DWORD dwUID )
{
    m_pkSayBalloon->DeleteUserMsg( dwUID );
}

bool KGCGameOverlayUI::LoadDramaPhoto()
{
    m_mapPhoto.clear();
    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) == false )
        return false;

    if( GCFUNC::LoadLuaScript( luaMgr, "etc.stg" ) == false )
        return false;

    if( GCFUNC::LoadLuaScript( luaMgr , "DramaPhotoInfo.lua" ) == false )
    {
        return false;        
    }


    LUA_BEGIN_TABLE("DramaPhoto",return false);
    {
        for(int i = 1; ; i++)
        {
            SDramaPhoto sDramaPhoto;
            LUA_BEGIN_TABLE(i,break)
            {
                LUA_GET_VALUE_DEF( "DRAMA_SPEAKER", sDramaPhoto.nStringNumber, 0 );
                
                LUA_GET_VALUE_DEF( "X_SIZE", sDramaPhoto.nXSize, 224 );
                LUA_GET_VALUE_DEF( "Y_SIZE", sDramaPhoto.nYSize, 326 );
                
                LUA_GET_VALUE_DEF( "TEX_NAME", sDramaPhoto.strTexName, "" );
                LUA_GET_VALUE_DEF( "TEX_TU1", sDramaPhoto.nTexTU1, 0 );
                LUA_GET_VALUE_DEF( "TEX_TV1", sDramaPhoto.nTexTV1, 0 );
                LUA_GET_VALUE_DEF( "TEX_TU2", sDramaPhoto.nTexTU2, 1 );
                LUA_GET_VALUE_DEF( "TEX_TV2", sDramaPhoto.nTexTV2, 1 );

                std::map<int, SDramaPhoto>::iterator iter = m_mapPhoto.find( sDramaPhoto.nStringNumber );

                if ( iter != m_mapPhoto.end() )
                {
                    std::wstring strTemp = boost::str(boost::wformat(L"Duplication Map Photo stringNumber %1% - %2%")%sDramaPhoto.nStringNumber%iter->second.nStringNumber);
                    MessageBox( NULL, strTemp.c_str(), L"", MB_OK );
                }

                m_mapPhoto.insert( std::pair<int,SDramaPhoto>( sDramaPhoto.nStringNumber, sDramaPhoto ) );
                

            }
            LUA_END_TABLE(return false)
        }
    }
    LUA_END_TABLE(return false)


    return true;

}

void KGCGameOverlayUI::SetDrama( int nDramaSpeaker )
{
    std::map<int, SDramaPhoto>::iterator iter = m_mapPhoto.find( nDramaSpeaker );
    if ( iter == m_mapPhoto.end() )
        return;

    m_pkLeftDramaPhoto->SetDramaPhoto( iter->second );
    m_pkLeftDramaPhoto->ToggleRender( true );   
}

KGCInGameMessage* KGCGameOverlayUI::GetInGameMessage()
{
    return m_pkInGameMessage;
}

KGCCommandGuid* KGCGameOverlayUI::GetCommandGuid()
{
    return m_pkCommandGuid;
}

void KGCGameOverlayUI::ShowDungeonLoadingTip( const bool bShow_ )
{
    static bool bSetText = false;

    if( bShow_ ) {
        for( int i=0 ; i<4 ; ++i ) { 
            m_pkLoadingEdge[i]->ToggleRender( true );
            m_pkLoadingEdge[i]->Render();
        }

        m_pkLoadingTipIcon->ToggleRender( true );
        m_pkLoadingTipBack->ToggleRender( true );
        m_pkLoadingTipStatic->ToggleRender( true );

        m_pkLoadingTipBack->Render();
        m_pkLoadingTipIcon->Render();
        m_pkLoadingTipStatic->Render();

        if( !bSetText ) { 
            int iTipID = SiKGCWorldMapManager()->GetDungeonLoadingTip( SiKGCRoomManager()->GetGameMode() );
            m_pkLoadingTipStatic->SetTextAutoMultiline(g_pkStrLoader->GetString( iTipID ));
            bSetText = true;
        }
    }
    else { 
        for( int i=0 ; i<4 ; ++i ) { 
            m_pkLoadingEdge[i]->ToggleRender( false );
        }

        m_pkLoadingTipIcon->ToggleRender( false );
        m_pkLoadingTipBack->ToggleRender( false );
        m_pkLoadingTipStatic->ToggleRender( false );

        bSetText = false;
    }
}

bool KGCGameOverlayUI::IsRenderSkillList()
{
    return m_pkBtnSkillList->IsRenderOn();
}
void KGCGameOverlayUI::SetPlayerRankInfo( float fDamage )
{
    m_pkHeadUpDisplay->SetPlayerRank(fDamage);
}

void KGCGameOverlayUI::UpdatePlayerRankInfo()
{
    if ( m_pkMinimap->IsRenderOn() )
    {
        m_pkMinimap->SetPlayerRankInfo();
    }
}

bool KGCGameOverlayUI::IsRenderOnStaticInternalTest( void )
{
    if( m_pkStaticInternalTest ) { 
        return m_pkStaticInternalTest->IsRenderOn();
    }

    return false;
}

void KGCGameOverlayUI::ToggleStaticInternalTest( void )
{
    if( m_pkStaticInternalTest ) { 
        m_pkStaticInternalTest->ToggleRender( !m_pkStaticInternalTest->IsRenderOn() );
    }
}

void KGCGameOverlayUI::SetStaticInternalTest( std::wstring& strText_ )
{
    if( m_pkStaticInternalTest ) { 
        m_pkStaticInternalTest->SetTextAutoMultiline( strText_ );
    }
}

void KGCGameOverlayUI::StartBossAlram( const int iBlinkFrame_, const int iBlinkCount_ )
{
    if( m_pkBossAlram ) { 
        m_pkBossAlram->StartAlram( iBlinkFrame_, iBlinkCount_ );
    }
}