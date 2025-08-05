#include "stdafx.h"
#include ".\kgcsquareoverlayui.h"
#include "GCUI/GCChatBox.h"


//


#include "Square/GCSquare.h"
#include "Square/GCSquarePeople.h"
#include "Controls.h"
#include "GCNeonsignSelectBox.h"

//
#include "GCUI/KGCChatScrollBackground.h"
#include "GCUI/KGCChatBoxBackground.h"
#include "GCUI/KGCCheckButton.h"
#include "GCUI/KGCAlertWnd.h"
#include "GCUI/GCSquareUserList.h"
#include "GCUI/GCSquarePickUserList.h"
#include "gcui/GCSquareSayBalloon.h"
#include "KGCShortCutBox.h"
#include "GCUI/KGCCooperationEventDlg.h"
#include "GCSquareSayBalloon.h"
#include "gcui/KGCGuildSquareMenu.h"
#include "gcui/KGCGuildSquareNotice.h"

#include "KDInput.h"
#include "GCInventoryFullMsg.h"

IMPLEMENT_CLASSNAME( KGCSquareOverlayUI );
IMPLEMENT_WND_FACTORY( KGCSquareOverlayUI );
IMPLEMENT_WND_FACTORY_NAME( KGCSquareOverlayUI, "gc_square_overlay_ui" );

KGCSquareOverlayUI* g_pkSquareOverlayUI = NULL;

KGCSquareOverlayUI::KGCSquareOverlayUI(void)
: m_pkSquareInfo(NULL)
, m_pkSquareInfoBackCenter(NULL)
, m_pkSquareInfoBackTopRight(NULL)
, m_pkSquareInfoBackRight(NULL)
, m_pkSquareInfoBackBottomRight(NULL)
, m_pkSquareUserList(NULL)
, m_pkSquarePickUserList(NULL)
, m_pkButtonBackground(NULL)
, m_pkStarBtn(NULL)
, m_pkCooperationEventBtn(NULL)
, m_pkGuildSquareMenu(NULL)
, m_iStartTime(0)
{
    g_pkSquareOverlayUI           = this;
    m_bHandleChatEvent            = false;
    m_bDragWindowCheck            = false;
    m_bCheckChatTypeBtns          = false;
    m_bChatButtonOff              = false;
	m_bRallyEvent				  = false;

    m_vBeforePos = D3DXVECTOR2(1,1);
    m_pkChatBox = NULL;
    m_pkChangeViewBtn = NULL;
    m_pkChatScrollBackground = NULL;
    m_pkChatBoxBackGround = NULL;

    m_dwBeforeScrollHeight = NULL;
    m_dwBeforeListHeight = NULL;    
    m_dwBeforScrollBackground = NULL;

    m_pkMsnButton   = NULL;
    m_pkAttendanceCalendar = NULL;
    m_pkMissionButton = NULL;
    m_pkOptionButton = NULL;
    m_pkExitButton = NULL;
    m_pkPartyButton = NULL;
    m_pkSayBalloon = NULL;
    m_pkBalloonTalkBtn1 = NULL;
    m_pkBalloonTalkBtn2 = NULL;
    m_pkSquareUserListButton = NULL;
    m_bUserListRender = false;

    m_pkShortCutBox = NULL;
	m_pkCoupleButton = NULL;


    LINK_CONTROL("square_chat_box" , m_pkChatBox );
    LINK_CONTROL("chatbox_downBtn" , m_pkChangeViewBtn );    
    LINK_CONTROL("scroll_background_bar" , m_pkChatScrollBackground );        
    LINK_CONTROL("chat_list_back" , m_pkChatBoxBackGround );
    LINK_CONTROL("msn_button" , m_pkMsnButton );
    LINK_CONTROL( "attendance_calendar_btn",    m_pkAttendanceCalendar );
    LINK_CONTROL( "mission_button",             m_pkMissionButton );
    LINK_CONTROL( "option_button",              m_pkOptionButton );
    LINK_CONTROL( "exit_button",                m_pkExitButton );
    LINK_CONTROL( "party_button",               m_pkPartyButton );
    LINK_CONTROL( "backgrounds",                m_pkButtonBackground );
    LINK_CONTROL( "say_balloon",                m_pkSayBalloon );
    LINK_CONTROL( "balloon_talk_Btn1",           m_pkBalloonTalkBtn1 );
    LINK_CONTROL( "balloon_talk_Btn2",           m_pkBalloonTalkBtn2 );
    LINK_CONTROL( "static_square_info",             m_pkSquareInfo );
    LINK_CONTROL( "square_info_back_center",        m_pkSquareInfoBackCenter );
    LINK_CONTROL( "square_info_back_top_right",     m_pkSquareInfoBackTopRight );
    LINK_CONTROL( "square_info_back_bottom_right",  m_pkSquareInfoBackRight );
    LINK_CONTROL( "square_info_back_right",         m_pkSquareInfoBackBottomRight );
    LINK_CONTROL( "square_btn_userlist",            m_pkSquareUserListButton );
    LINK_CONTROL( "square_userlist",                m_pkSquareUserList );
    LINK_CONTROL( "square_pickuserlist",            m_pkSquarePickUserList );
    LINK_CONTROL( "star_Btn",                       m_pkStarBtn );
	LINK_CONTROL( "short_cut_slot",             m_pkShortCutBox );
	LINK_CONTROL( "couplewnd_btn",				m_pkCoupleButton );
	//LINK_CONTROL( "dummy_alert",             m_pkMyInfoSceneAlert );
	LINK_CONTROL( "cooperation_event_btn",		m_pkCooperationEventBtn);
    LINK_CONTROL( "guild_button",				m_pkGuildBtn);
	LINK_CONTROL( "guild_sqaure_menu",			m_pkGuildSquareMenu);
	LINK_CONTROL( "worldbillboardchat", m_pkBillBoardChatBoard);
}

KGCSquareOverlayUI::~KGCSquareOverlayUI(void)
{
}


void KGCSquareOverlayUI::ActionPerformed( const KActionEvent& event )
{
    if(event.m_dwCode== KD3DWnd::D3DWE_BUTTON_DOWN)
    {
        g_MyD3D->MyCtrl->Mouse_Power = 255; //일단 클릭이벤트 발생되면 무조껀, 마우스 보이도록 처리
    }

    if (event.m_pWnd == m_pkChangeViewBtn) 
    {        
        if(event.m_dwCode== KD3DWnd::D3DWE_BUTTON_DOWN)
        {
            POINT       pt = g_pkInput->GetMousePos();
            D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
            //나중에 X축 처리할때 쓰삼...
			vMousePos.x/=m_fWindowScaleX;
			vMousePos.y/=m_fWindowScaleY;
            m_vBeforePos=vMousePos;
            m_dwBeforeScrollHeight=m_pkChatBox->GetScrollHeight();
            m_dwBeforeListHeight=m_pkChatBox->GetListHeight();
            m_dwBeforScrollBackground=m_pkChatScrollBackground->GetMiddleHeight();
            m_dwBeforChatBoxBackground=m_pkChatBoxBackGround->GetMiddleHeight();            
            m_bDragWindowCheck=true;            
            m_pkChangeViewBtn->Lock(true); 
            m_pkChatBox->SetChatListClick(false); //드래그 되는 동안 채팅창은 클릭안되게 처리
        }
        else if(event.m_dwCode==D3DWE_DRAG_END) //그냥 BUTTON_UP으로 해주면 해당 UI위에서만 마우스 놓았을때 이벤트가 들어온다-_-;
        {
            m_bDragWindowCheck=false;
            m_pkChangeViewBtn->Lock(false);
            m_pkChatBox->SetChatListClick(true); 
        }
    }
    else if ( event.m_pWnd == m_pkSquareUserList && m_pkSquareUserList->IsLClickUserList() )
    {
        SetNameOnUserlistForWhisper( m_pkSquareUserList->GetClickedUserName() );
        m_pkSquareUserList->SetLClickUserlist( false );
    }
#if !defined(CLOSED_BETA)
    else if( event.m_pWnd == m_pkChatBox && event.m_dwCode == D3DWE_BUTTON_CLICK )
    {
        POINT       pt = g_pkInput->GetMousePos();
        D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
        
        if( m_pkStarBtn->CheckPosInWindowBound( vMousePos ) == true )
        {
            g_pkUIScene->m_pkNeonsignSelectBox->SetSignBoardType( KSignBoardData::SBT_SQUARE );
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_NEONSIGN_SELECT );
        }
    }
#endif

    {        
        if(event.m_dwCode==KD3DWnd::D3DWE_BUTTON_CLICK)
        {
            if( event.m_pWnd == m_pkBalloonTalkBtn1)
            {   
                m_pkSayBalloon->ToggleRender(false);
                m_pkBalloonTalkBtn1->InitState(false,false,NULL);
                m_pkBalloonTalkBtn2->InitState(true,true,this);
            }
            else if( event.m_pWnd == m_pkBalloonTalkBtn2)
            {
                m_pkSayBalloon->ToggleRender(true);
                m_pkBalloonTalkBtn2->InitState(false,false,NULL);
                m_pkBalloonTalkBtn1->InitState(true,true,this);
            }
            else if( event.m_pWnd == m_pkMsnButton )
            {
                g_MyD3D->OnMSNButton();
                m_pkChatBox->SetChatListClick(false); //창이 뜨면 뒤에 채팅창은 클릭안되게 처리                
            }
#if defined( ENABLE_POINT_SYSTEM )
            else if( event.m_pWnd == m_pkAttendanceCalendar  )
            {
                g_MyD3D->OnCalendarButton();
                m_pkChatBox->SetChatListClick(false);
            }
#endif
#if defined( COUPLE_SYSTEM )
			else if( event.m_pWnd == m_pkCoupleButton  )
			{
				g_MyD3D->OnCoupleButton();
				m_pkChatBox->SetChatListClick(false);
			}
#endif
            else if( event.m_pWnd == m_pkMissionButton )
            {
                g_MyD3D->OnMissionButton();
                m_pkChatBox->SetChatListClick(false);
            }
            else if( event.m_pWnd == m_pkOptionButton )
            {
                g_MyD3D->OnOptionButton();
                m_pkChatBox->SetChatListClick(false);
            }
            else if( event.m_pWnd == m_pkExitButton )
            {
                ModalExitMsgBox();
            }
            else if( event.m_pWnd == m_pkPartyButton )
            {
                //파티 버튼 클릭시...?
                //m_pkChatBox->SetChatListClick(false);               
            }
            else if ( event.m_pWnd == m_pkSquareUserListButton )
            {
                if ( false == m_bUserListRender)
                {
                    // 꺼져 있다면 켜야지 암!
                    m_bUserListRender = true;
                    m_pkSquareUserList->SetRenderUserList( true );
                    //m_pkSquareUserList->InitState( m_bUserListRender, true, this );
                    m_pkSquareUserList->ToggleRender( m_bUserListRender );
                }
                else if ( m_bUserListRender && !m_pkSquareUserList->GetRenderUserList() )
                {
                    // 꺼져 있는데 이제 꺼라카믄? -_- 켜야지 ㅎㅎ
                    m_bUserListRender = true;
                    m_pkSquareUserList->SetRenderUserList( true );
                    m_pkSquareUserList->ToggleRender( m_bUserListRender );
                }
                else
                {
                    // 켜져 있으면 꺼! 
                    m_bUserListRender = false;
                    m_pkSquareUserList->SetRenderUserList( false );
                    m_pkSquareUserList->ToggleRender( m_bUserListRender );
                }
            }
            else if( event.m_pWnd == m_pkCooperationEventBtn )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_COOPERATION_EVENT_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
            }

            g_KDSound.Play("72");
        }
    }

    if (event.m_dwCode==KD3DWnd::EWNDMESSAGE_CLOSE || event.m_dwCode==KGCUIScene::GC_MBOX_RETURN_OK)
    {
        /*if (event.m_pWnd == m_pkAttendanceCalendar)
        {
            EndMessageBox();
        }*/
        EndMessageBox();
    }

	if( event.m_pWnd == m_pkGuildBtn && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		g_KDSound.Play( "31" );
		if( m_pkGuildSquareMenu->IsRenderOn() )
			m_pkGuildSquareMenu->ToggleRender(false);
		else
			m_pkGuildSquareMenu->ToggleRender(true);
	}

    return;
}

void KGCSquareOverlayUI::EndMessageBox(void)
{
    g_pkInput->IgnoreDown(DIK_ESCAPE,30);
    m_pkChatBox->SetChatListClick(true);
    ::SetFocus( g_hUIMainWnd );
}

void KGCSquareOverlayUI::ExitSquare( void )
{
	SiKGCFantasticMap()->ClearWater();
    ToggleRender(false);
    g_MyD3D->m_pStateMachine->GoState( GS_SQUARE_END );
	//g_pkSquareChatBox->OffColorButton();

    g_pkUIScene->EndMessageBox();
    g_pkUIScene->RemoveWaitMessageBox( KGCUIScene::GC_MBOX_EXIT_SQUARE);
    
	ClearParticle();
}


void KGCSquareOverlayUI::ClearChatEvent( void )
{
    g_KDSound.Play( "72" );
    m_pkChatBox->m_pkEdit->SetText(L"");
    ::SetFocus( g_hUIMainWnd );
}

//void KGCSquareOverlayUI::LockMessageBoxBtn(bool bChk)
//{
//    m_pkAttendanceCalendar->Lock(bChk);
//    m_pkMissionButton->Lock(bChk);
//    m_pkOptionButton->Lock(bChk);
//    m_pkExitButton->Lock(bChk);
//    //m_pkPartyButton->Lock(bChk);
//    m_pkMsnButton->Lock(bChk);
//}




void KGCSquareOverlayUI::OnCreate( )
{
	//this->SetAlignType(WNDUIAT_POSZEROBASE);
    m_pkChatBox->SetSelfInputCheck(false);
    m_pkChatBox->AddActionListener(this);
    m_pkChatBox->ToggleRender(true);

    m_pkChangeViewBtn->SetSelfInputCheck( true );
    m_pkChangeViewBtn->AddActionListener(this);
    m_pkChangeViewBtn->ToggleRender(true);

    m_pkExitButton->SetHotKey( DIK_ESCAPE );
    
	//m_pkMsnButton->SetAlignType(WNDUIAT_POSRIGHTTOPBASE);
    m_pkMsnButton->InitState( true, true, this );
    m_pkMsnButton->SetToolTip( g_pkStrLoader->GetString( STR_ID_FRIEND ),0.0f,75.0f );
    m_pkMsnButton->SetAlertButtonType( KGCAlertWnd::MSN );

#if defined( ENABLE_POINT_SYSTEM )
    m_pkAttendanceCalendar->InitState( true, true, this );
    m_pkAttendanceCalendar->SetToolTip( g_pkStrLoader->GetString( STR_ID_ATTENDANCE_CALENDAR ), 0.0f, 75.0f );
    m_pkAttendanceCalendar->SetAlertButtonType( KGCAlertWnd::CALENDAR );
	//m_pkAttendanceCalendar->SetAlignType(WNDUIAT_POSRIGHTTOPBASE);
#else
    m_pkAttendanceCalendar->InitState( true, true );
    m_pkAttendanceCalendar->Lock( true );
    m_pkAttendanceCalendar->SetToolTip( g_pkStrLoader->GetString( STR_ID_ATTENDANCE_CALENDAR ), 0.0f, 50.0f );
    //m_pkAttendanceCalendar->SetToolTip( g_pkStrLoader->GetString( STR_ID_NOW_WE_MAKE_IT_TOT ), 0.0f, 50.0f );
#endif

#ifdef CALENDAR_RECOMMAND
    g_pkUIScene->m_pkAttendanceInfoMsg->SetWindowPosDirect( D3DXVECTOR2( 550.0f * GC_SCREEN_DIV_WIDTH, 70.0f * GC_SCREEN_DIV_WIDTH ) );
    g_pkUIScene->m_pkAttendanceInfoMsg->SetMsgType( KGCInventoryFullMsg::CALENDAR_INFO );
#endif

#if defined( COUPLE_SYSTEM )
	m_pkCoupleButton->InitState( true, true, this );
	m_pkCoupleButton->SetToolTip( g_pkStrLoader->GetString( STR_ID_COUPLE_BTN_TOOLTIP ), 0.0f, 75.0f );
	m_pkCoupleButton->SetAlertButtonType( KGCAlertWnd::COUPLE );
	//m_pkCoupleButton->SetAlignType(WNDUIAT_POSRIGHTTOPBASE);
#else
	//m_pkCoupleButton->Lock( true );
	m_pkCoupleButton->InitState( false );
	//m_pkCoupleButton->SetToolTip( g_pkStrLoader->GetString( STR_ID_NOW_WE_MAKE_IT_TOT ), 0.0f, 50.0f );
#endif

    m_pkMissionButton->InitState( true, true, this );
    m_pkMissionButton->SetToolTip( g_pkStrLoader->GetString( STR_ID_MISSION ) ,0.0f,75.0f);
    m_pkMissionButton->SetAlertButtonType( KGCAlertWnd::MISSION);
	//m_pkMissionButton->SetAlignType(WNDUIAT_POSRIGHTTOPBASE);

    m_pkOptionButton->InitState( true, true, this );
    m_pkOptionButton->SetToolTip( g_pkStrLoader->GetString( STR_ID_OPTION ) ,0.0f,75.0f);
	//m_pkOptionButton->SetAlignType(WNDUIAT_POSRIGHTTOPBASE);
  

    m_pkExitButton->InitState( true, true, this );
    m_pkExitButton->SetToolTip( g_pkStrLoader->GetString( STR_ID_SQUARE_LEAVE ) ,0.0f,75.0f);
	//m_pkExitButton->SetAlignType(WNDUIAT_POSRIGHTTOPBASE);

    m_pkButtonBackground->InitState( true );
	//m_pkButtonBackground->SetAlignType(WNDUIAT_POSRIGHTTOPBASE);

    m_pkSquareInfo->InitState( false );
    m_pkSquareInfo->SetFontSize( SiKGCMultipleLanguages()->GetLargeFontSize() );
    m_pkSquareInfoBackCenter->InitState( false );
    m_pkSquareInfoBackTopRight->InitState( false );
    m_pkSquareInfoBackRight->InitState( false );
    m_pkSquareInfoBackBottomRight->InitState( false );
    
    m_pkPartyButton->InitState( true, true, this );
    m_pkPartyButton->SetToolTip( g_pkStrLoader->GetString( STR_ID_SQUARE_PARTY_TIP ) ,0.0f,75.0f);
    m_pkPartyButton->Lock(true);
	//m_pkPartyButton->SetAlignType(WNDUIAT_POSRIGHTTOPBASE);
    //파티 기능은 추후 지원때 까지 막음

    m_pkSquareUserList->InitState( false, true, this );
    m_pkSquareUserListButton->InitState( true, true, this );
    m_bUserListRender = false;

    m_pkSquarePickUserList->InitState( false );

    ToggleRender(false);
    m_pkChatBox->Clear();
    ReLocate();

#if defined(CLOSED_BETA)
    m_pkStarBtn->InitState( true, false, NULL );
#else
    m_pkStarBtn->InitState( true, true, this );
    m_pkStarBtn->SetToolTip( g_pkStrLoader->GetString( STR_ID_NEONSIGN ), 0.0f, 75.0f );
#endif


#if defined(EMOTICON_SYSTEM)
	m_pkShortCutBox->InitState( true, true, this );	
#else
	m_pkShortCutBox->InitState( false, true, this );	
#endif
#if defined(COOPORATION_EVENT)
    m_pkCooperationEventBtn->InitState( true, true ,this);
#else
    m_pkCooperationEventBtn->InitState( false, false ,NULL);
#endif
	m_pkGuildBtn->InitState( false, true, this );
	m_pkGuildBtn->SetToolTip( g_pkStrLoader->GetString(STR_ID_NEW_GUILD_SQUARE_GUILD_MENU), 0.0f, 75.0f );
	m_pkGuildSquareMenu->InitState(false, true, this);

	DisableMenuButtons();

	m_pkBillBoardChatBoard->InitState( true, true, this );
}

void KGCSquareOverlayUI::OnCreateComplete( void )
{	
	m_iStartTime = 0;
}

void KGCSquareOverlayUI::OnInitialize()
{
    m_pkShortCutBox->SetShortCutMode( SCB_MODE_EMOT );
#if !defined(EMOTICON_SYSTEM)
	m_pkShortCutBox->SetShowState( SFS_HIDE );
	m_pkShortCutBox->ToggleRender(false);
#endif

	if(SiKGCEventManager()->IsActiveEvent(EVENT_ID_CAMPFIRE))
	{
		SiGCSquare()->CreateCampFire();
	}
	if(SiKGCEventManager()->IsActiveEvent(EVENT_ID_FIREWORKS))
	{
		SiGCSquare()->CreateFireWorks();
	}

	if(SiKGCEventManager()->IsActiveEvent(EVENT_ID_WATER_IN_SQUARE) )
	{
#if defined(NATION_THAILAND) || defined( NATION_KOREA )
		SiKGCFantasticMap()->SetWaterHeight(0.35f);
#else
		SiKGCFantasticMap()->SetWaterHeight(0.65f);
#endif
		SiKGCFantasticMap()->CreateWater();

		g_ParticleManager->CreateSequence( "O2Zone", 1.2f , -0.2f, 0.5f );
		g_ParticleManager->CreateSequence( "O2Zone", 3.7f , -0.2f, 0.5f );
		g_ParticleManager->CreateSequence( "O2Zone", 5.2f , -0.2f, 0.5f );
		g_ParticleManager->CreateSequence( "O2Zone", 6.7f , -0.2f, 0.5f );
		g_ParticleManager->CreateSequence( "O2Zone", 8.2f , -0.2f, 0.5f );
		g_ParticleManager->CreateSequence( "O2Zone", 9.7f , -0.2f, 0.5f );
	}	

	KP2P::GetInstance()->Send_MyDonationInfoReq();
}

void KGCSquareOverlayUI::ReLocate()
{
    D3DXVECTOR2 vPos;
    vPos.x = m_pkChangeViewBtn->GetFixedWindowPos().x;
    vPos.y = static_cast<FLOAT>(m_pkChatBox->GetScrollHeight() + m_pkChatBox->GetScrollBtnGap()*3);
    m_pkChangeViewBtn->SetAllWindowPos(vPos);    
}

void KGCSquareOverlayUI::StartSquare()
{
	m_bRallyEvent = false;

	//광장 시작하라고 하면.. 해상도 부터 키운다. 태국서 작업하다 사고칠꺼 같아 일단 닫아둠
	/*if ( g_MyD3D->m_KGCOption.GetBasicMode() != g_MyD3D->m_KGCOption.GetCurrentMode() )
	{
		g_MyD3D->SetResolution(TRUE);
	}*/

#if !defined( ENABLE_POINT_SYSTEM )
	m_pkAttendanceCalendar->Lock( true );
#endif

    m_pkBalloonTalkBtn1->InitState(true,true,this);
    m_pkBalloonTalkBtn2->InitState(false,false,NULL);
    m_pkSayBalloon->InitState(true,true,NULL);

    m_bHandleChatEvent          = false;
    m_bDragWindowCheck          = false;
    m_bCheckChatTypeBtns        = false;

    m_pkChatBox->Clear();
    ReLocate();
    HandleChatEvent();

    //테스트 데이터
    /*for (int i=0;i<200;i++)
    {
        g_pkChatManager->AddChatMsg(L"일반::조금은서툴러도지금은연습일뿐야하이어웨이웨잇폴조금은서툴러도지금은연습일뿐야하이어웨이웨잇폴미웨잇폴미언젠가는강미웨잇폴미언젠가는강" , KGCChatManager::CHAT_TYPE_NORMAL ,false,false,L"NORMAL" );
        g_pkChatManager->AddChatMsg(L"파티::조금은서툴러조금은서툴러도지금은연습일뿐야하이어웨이웨잇폴미웨잇폴미언젠가는강도지금은연습일뿐야하이어웨이웨잇폴미웨잇폴미언젠가는강" , KGCChatManager::CHAT_TYPE_PARTY ,false,false,L"CHAT_TYPE_PARTY");
        g_pkChatManager->AddChatMsg(L"외치기::조금은서툴러도지금은" , KGCChatManager::CHAT_TYPE_SHOUT ,false,false,L"CHAT_TYPE_SHOUT");
        g_pkChatManager->AddChatMsg(L"길드::조금은서툴러도지금은연습일뿐조금은서툴러도지금은연습일뿐야하이어웨이웨잇폴미웨잇폴미언젠가는강야하이어웨이웨잇폴미웨잇폴미언젠가는강" , KGCChatManager::CHAT_TYPE_GUILD ,false,false,L"CHAT_TYPE_GUILD");
        g_pkChatManager->AddChatMsg(L"운영자::조금은서툴러도지금은연습일뿐야하이어웨이웨잇폴미웨잇폴미언젠가는강" , KGCChatManager::CHAT_TYPE_ADMIN ,false,false,L"CHAT_TYPE_ADMIN");
        g_pkChatManager->AddChatMsg(L"귓말::조금은서툴러도지금은연습조금은서툴러도지금은연습일뿐야하이어웨이웨잇폴미웨잇폴미언젠가는강일뿐야하이어웨이웨잇폴미웨잇폴미언젠가는강" , KGCChatManager::CHAT_TYPE_WHISPER ,false,false,L"CHAT_TYPE_WHISPER");
    }*/

    g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_SQUARE_PERFORMANCE_CONTROL ), KGCChatManager::CHAT_TYPE_ADMIN, false, false );
    m_pkShortCutBox->Initialize();
    UpdateSquareInfo();
	g_pkSquareChatBox->InitColorButton();

#if defined(COOPORATION_EVENT)
    m_pkCooperationEventBtn->ToggleRender( 0 != g_pkUIScene->m_pkCooperationEventDlg->GetEventID() );
#endif
	if( SiGCSquare()->GetSquareType() == KSquareInfo::ST_GUILD )
	{
		m_pkGuildBtn->ToggleRender(true);
		m_pkPartyButton->ToggleRender(false);
	}
	else	
	{
		m_pkGuildSquareMenu->ToggleRender(false);
		m_pkGuildBtn->ToggleRender(false);
		m_pkPartyButton->ToggleRender(false);
	}

	LoadSquareParticleList();
}

void KGCSquareOverlayUI::OnDestroy()
{
	g_pkUIScene->m_pkAttendanceCalendar->ToggleRender( false );
}

void KGCSquareOverlayUI::SetEditFocus( void )
{
    if ( m_pkChatBox != NULL )
    {
        m_pkChatBox->SetFocus();
    }
}

void KGCSquareOverlayUI::OnDestroyComplete()
{

}

void KGCSquareOverlayUI::FrameMoveInEnabledState()
{    
//     if (m_pkAllBtn->GetWndMode()==D3DWM_HOVER ||
//         m_pkWhisperBtn->GetWndMode()==D3DWM_HOVER ||
//         m_pkShoutBtn->GetWndMode()==D3DWM_HOVER ||
//         m_pkGeneralBtn->GetWndMode()==D3DWM_HOVER ||
//         m_pkGuildBtn->GetWndMode()==D3DWM_HOVER ||
//         m_pkPartyBtn->GetWndMode()==D3DWM_HOVER)
//     {
//         g_MyD3D->MyCtrl->Mouse_Power=255;        
//     }

    //마우스 버튼 업시, 창 드레그 중인 경우 중지
    if(g_pkInput->BtnUp() && m_bDragWindowCheck==true)
    {
        m_pkChangeViewBtn->SpeakToActionListener(KActionEvent(m_pkChangeViewBtn,D3DWE_DRAG_END));  
    }

    if (m_bDragWindowCheck==true)
    {
        //OnMove일때 처리 계속 움직여 줍니다.
        POINT       pt = g_pkInput->GetMousePos();
		pt.x = (LONG)( pt.x / m_fWindowScaleX );
		pt.y = (LONG)( pt.y / m_fWindowScaleY );

        float fGap=m_dwBeforeScrollHeight-m_vBeforePos.y+pt.y;
        if (fGap >= HEIGHT_MINUMUM && fGap <= HEIGHT_MAXIMUM)  //범위초과시 더이상안 한다. Scroll을 기준으로 처리함
        {
            m_pkChatBox->SetListHeight(static_cast<DWORD>(m_dwBeforeListHeight-m_vBeforePos.y+pt.y));            
            m_pkChatBox->SetScrollHeight(static_cast<DWORD>(m_dwBeforeScrollHeight-m_vBeforePos.y+pt.y));
            m_pkChatScrollBackground->SetMiddleHeight(static_cast<DWORD>(m_dwBeforScrollBackground-m_vBeforePos.y+pt.y));
            m_pkChatBoxBackGround->SetMiddleHeight(static_cast<DWORD>(m_dwBeforChatBoxBackground-m_vBeforePos.y+pt.y));            
            ReLocate();
        }
    }

    POINT       pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
    if ( g_pkInput->BtnUp( KInput::MBLEFT ) && !m_pkSquarePickUserList->CheckPosInWindowBoundWithChild( vMousePos ) )
        m_pkSquarePickUserList->ToggleRender( false );

    // 유저리스트의 팝업창 이외에 딴거 클릭하면 꺼지쟈!
    if ( g_pkInput->BtnUp( KInput::MBLEFT ) && !m_pkSquareUserList->CheckPosPopupBoxBound( vMousePos ) )
        m_pkSquareUserList->SetPopupBoxRender( false );

    // 픽킹해서 뜨는 팝업창은 내 캐릭 움직이면 사라져염!
    if ( g_MyD3D->MyCtrl->k_Fresh_Down || g_MyD3D->MyCtrl->k_Fresh_Up 
        || g_MyD3D->MyCtrl->k_Left || g_MyD3D->MyCtrl->k_Right )
    {
        m_pkSquarePickUserList->ToggleRender( false );
    }

	// 광장 높이 올라가기 대회
	if( g_kGlobalValue.IsAdmin() )
	{
		if( diks[DIK_LCONTROL] )
		{
			if( diks[DIK_1] && !Old_diks[DIK_1] )
			{
				KP2P::GetInstance()->Send_StartJumpEvent();
			} 
			else if( diks[DIK_5] && !Old_diks[DIK_5] )
			{
				KP2P::GetInstance()->Send_FinishJumpEvent();
			}
		}
	}
		
	if( SiGCSquare()->GetSquareType() == KSquareInfo::ST_GUILD )
	{
		if( m_iStartTime < GC_FPS_LIMIT * 3 )
			m_iStartTime++;

		if( m_iStartTime ==  (int)(GC_FPS_LIMIT / 2) )
		{
			g_pkUIScene->m_pkGuildSquareNotice->Create();
			g_pkUIScene->m_pkGuildSquareNotice->ToggleRender(true);
		}

		if( m_iStartTime == (int)(GC_FPS_LIMIT * 2.5f) )
			g_pkUIScene->m_pkGuildSquareNotice->Close();			
	}
}
void KGCSquareOverlayUI::LoadSquareParticleList( void ) 
{

	m_vecSquareParticle.clear();

	KLuaManager luaMgr;
	 
	if( GCFUNC::LoadLuaScript( luaMgr , "emblem.lua" ) == false )
	  return ;
	 
	LUA_BEGIN_TABLE( "EVENT_SQUARE_PARTICLE", ;)
		for( int i = 1; ; i++ ){
		  LUA_BEGIN_TABLE(i,break)
			bool b_OK = false;
			bool bGameMode = false;
			bool bOnly = false;
			int	   iEventID = -1;
			int	   GameMode = -1;

			LUA_BEGIN_TABLE("IS_ACTIVE_EVENT",;)
			  for(int i=1;;i++)
			  {
				  LUA_GET_VALUE_NOASSERT(i,iEventID,/*IS_ACTIVE_EVENT[i]*/break);
				  if( SiKGCEventManager()->IsActiveEvent(iEventID)){
					  b_OK = TRUE;
					  break;
				  }
			  }
			LUA_END_TABLE(/*IS_ACTIVE_EVENT*/;);
			if(!b_OK){	LUA_END_TABLE(/* EVENT_EMBLEM[i] */;) continue;}

			LUA_GET_VALUE_DEF("B_ONLY",bOnly,false);
			if(bOnly){
				m_vecSquareParticle.clear();
			}
			LUA_BEGIN_TABLE("PARTICLE",;)
				for(int i=1;;i++)
				{
					SSquareParticleInfo sParticleInfo = SSquareParticleInfo();
					sParticleInfo.iSeveralTimes = 1000;
					LUA_BEGIN_TABLE(i,break;);
						LUA_GET_VALUE_NOASSERT( "TIME_INTERVAL", sParticleInfo.iTimeInterval,; );
						LUA_GET_VALUE_NOASSERT( "STR_PARTICLE_NAME", sParticleInfo.strParticleName, ; );
						LUA_GET_VALUE_NOASSERT( "SEVERAL_TIMES", sParticleInfo.iSeveralTimes, ; );
						LUA_GET_VALUE_NOASSERT( "IS_TRACE_CAMERA", sParticleInfo.bIsTraceCamera, ; );
						LUA_GET_VALUE_NOASSERT( "IS_CREATE_POS_CAMERA", sParticleInfo.bIsCreatePosCamera, ; );
						LUA_GET_VALUE_NOASSERT( "POS_X", sParticleInfo.fPosX, ; );
						LUA_GET_VALUE_NOASSERT( "POS_Y", sParticleInfo.fPosY, ; );
					LUA_END_TABLE(/*i*/;);

					m_vecSquareParticle.push_back(sParticleInfo);
				}
			LUA_END_TABLE(/*STR_PARTICLE_NAME*/;);

		  LUA_END_TABLE(/*  EVENT_EMBLEM[i] */;);
		  if(bOnly)
			break;
		}
	LUA_END_TABLE(/* EVENT_EMBLEM */;);


}
void KGCSquareOverlayUI::CreateParticle( void )
{

	if(m_vecSquareParticle.empty() == false)
	{
		if (g_MyD3D->m_KGCOption.GetSquareQuality()==true)
		{	


			m_vCameraLookPos.x = g_kCamera.m_fLookX;
			m_vCameraLookPos.y = g_kCamera.m_fLookY;
			m_vCameraLookPos.z = 0.5f;

			int iNowTime = static_cast<int>(timeGetTime());

			std::vector<SSquareParticleInfo>::iterator vIter = m_vecSquareParticle.begin();
			
			for(; vIter != m_vecSquareParticle.end();vIter++)
			{			

		//		START_LOG(cout,"광장파티클"<< static_cast<int>(iNowTime) % vIter->iTimeInterval<<iNowTime);
				if( static_cast<int>(iNowTime) % vIter->iTimeInterval != 0 || iNowTime == vIter->iSquereParticleTime  )
					continue;
				
				if(vIter->iSeveralTimes == 0)
					continue;

				if(vIter->iSeveralTimes != -1)
					vIter->iSeveralTimes--;

				vIter->iSquereParticleTime = iNowTime;

				CParticleEventSeqPTR pTemp = NULL;
				TRACE_INFO stTrace;
				stTrace.m_pvPos = &m_vCameraLookPos;
				stTrace.m_fParticleTraceTime = 5.0f;
				pTemp = g_ParticleManager->CreateSequence( vIter->strParticleName.c_str(), m_vCameraLookPos.x, m_vCameraLookPos.y, m_vCameraLookPos.z );
				g_ParticleManager->SetLayer( pTemp, GC_LAYER_CUSTOM2);
				if(vIter->bIsTraceCamera){
                    if (pTemp)
					    pTemp->SetTrace( &stTrace );
				}
				else
				{
					if(!vIter->bIsCreatePosCamera)
                        if (pTemp)
						    pTemp->SetPosition(vIter->fPosX,vIter->fPosY,0.5f);
				}
				if(vIter->iSeveralTimes != -1)
					m_vecSquareParticleSequence.push_back(pTemp);
			}
		}
	}
}

void KGCSquareOverlayUI::ClearParticle()
{
	std::vector<CParticleEventSeqPTR>::iterator vIter = m_vecSquareParticleSequence.begin();
	
	for(; vIter != m_vecSquareParticleSequence.end();vIter++)
	{
		g_ParticleManager->DeleteSequence( *vIter );
		(*vIter) = NULL;	
	}

	m_vecSquareParticleSequence.clear();

}

void KGCSquareOverlayUI::TimeCheck( void ) const
{
	//마우스 가만히 놔뚜면 서서히 연해지는거
	if ( g_MyD3D->MyCtrl->Mouse_Power>4 ) 
		g_MyD3D->MyCtrl->Mouse_Power-=2;

	m_pkSayBalloon->TimeCheck(); //말풍선 타임체크


	D3DXVECTOR2 vPos;

	//눈뿌리기 옵션.
	if(SiKGCEventManager()->IsActiveEvent(EVENT_ID_SNOW_IN_SQUARE) == true )
	{
	  //눈뿌리기...
	  if (g_MyD3D->m_KGCOption.GetSquareQuality()==true)
	  {
		  vPos.x = static_cast<float>(rand() % 15);
		  vPos.y = static_cast<float>(rand() % 10);

		  g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "snowman_big", vPos.x, vPos.y, 0.5f ) , GC_LAYER_CUSTOM2);
		  g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "snowman_small", vPos.x, vPos.y, 0.5f ) , GC_LAYER_CUSTOM2);
	  }
	}



	


}

KGCSquareOverlayUI::ECursorFocus KGCSquareOverlayUI::GetCursorFocus( void ) const
{
    ECursorFocus eRet;
    if (g_hUIMainWnd==::GetFocus())
    {
        eRet=KGCSquareOverlayUI::ECF_MAINWND;
    }
    else if (m_pkChatBox->HasEditFocus()==true)
    {
        eRet=KGCSquareOverlayUI::ECF_CHATBOX;
    }
    else
    {
        eRet=KGCSquareOverlayUI::ECF_UNKNOWN;
    }     
    return eRet;
}

void KGCSquareOverlayUI::HandleChatEvent( void )
{	    
    ::SetFocus( g_hUIMainWnd );
}

void KGCSquareOverlayUI::DeleteAllSayBalloon()
{
	m_iStartTime = 0;
    m_pkSayBalloon->DeleteAllBalloon();
}

void KGCSquareOverlayUI::DeleteBalloonMsg( DWORD dwCharUID)
{
    m_pkSayBalloon->DeleteUserMsg(dwCharUID);
}

void KGCSquareOverlayUI::SetBalloonMsg( std::wstring strMsg ,DWORD dwCharUID, KChatData::ENUM_CHAT_TYPE eMsgType, DWORD dwChatColor)
{
    m_pkSayBalloon->NewUserMsg(strMsg, dwCharUID,eMsgType, dwChatColor);
}

void KGCSquareOverlayUI::SetNPCBalloonMsg( std::wstring strMsg ,DWORD dwCharUID, KChatData::ENUM_CHAT_TYPE eMsgType, DWORD dwChatColor )
{
	m_pkSayBalloon->NewNPCMsg(strMsg, dwCharUID,eMsgType, dwChatColor);
}

void KGCSquareOverlayUI::ReLocateBalloon( D3DXVECTOR2 vPos , DWORD dwUID, int BalloonDistance)
{	        
	m_pkSayBalloon->SetBalloonPos(vPos,dwUID, KGCSayBalloonSquare::GCSB_ARROW_POS_AUTO, BalloonDistance);
}

void KGCSquareOverlayUI::UpdateSquareInfo()
{
    KSquareInfo kInfo = SiGCSquare()->GetCurrentSquareInfo();
    std::wstringstream stm;
    stm << kInfo.m_strName << " " << kInfo.m_dwCurrentUserNum << " / " << kInfo.m_dwMaxUserNum;
    m_pkSquareInfo->SetText( stm.str() );

#if defined(NATION_TAIWAN)
	int iWidth = g_pkFontMgrOutline16_original->GetWidth( stm.str().c_str() ) + static_cast<int>( m_pkSquareUserListButton->GetWidth() ) ;
#else
	int iWidth = g_pkFontMgrOutline16_original->GetWidth( stm.str().c_str() ) + static_cast<int>( m_pkSquareUserListButton->GetWidth() ) - 10 ;
#endif
//    int iWidth = g_pkFontMgrOutline16->GetWidth( stm.str().c_str() ) + static_cast<int>( m_pkSquareUserListButton->GetWidth() ) - 10;
	m_pkSquareInfo->SetWidth(iWidth);
    m_pkSquareInfoBackCenter->SetWidth( iWidth );

    float fXpos = m_pkSquareInfoBackCenter->GetFixedWindowPos().x
        + static_cast<float>( m_pkSquareInfoBackCenter->GetWidth() );

    D3DXVECTOR2 vTempPos;
    vTempPos = m_pkSquareUserListButton->GetFixedWindowPos();
    vTempPos.x = fXpos - static_cast<float>(m_pkSquareUserListButton->GetWidth());
    m_pkSquareUserListButton->SetAllWindowPos( vTempPos );

    vTempPos = m_pkSquareInfoBackTopRight->GetFixedWindowPos();
    vTempPos.x = fXpos;
    m_pkSquareInfoBackTopRight->SetWindowPos( vTempPos );

    vTempPos = m_pkSquareInfoBackRight->GetFixedWindowPos();
    vTempPos.x = fXpos;
    m_pkSquareInfoBackRight->SetWindowPos( vTempPos );

    vTempPos = m_pkSquareInfoBackBottomRight->GetFixedWindowPos();
    vTempPos.x = fXpos;
    m_pkSquareInfoBackBottomRight->SetWindowPos( vTempPos );

    m_pkSquareUserListButton->ToggleRender( true );
    m_pkSquareInfo->ToggleRender( true );
    m_pkSquareInfoBackCenter->ToggleRender( true );
    m_pkSquareInfoBackTopRight->ToggleRender( true );
    m_pkSquareInfoBackRight->ToggleRender( true );
    m_pkSquareInfoBackBottomRight->ToggleRender( true );
}

void KGCSquareOverlayUI::ModalExitMsgBox( void )
{
    if( KGCSquareOverlayUI::ECF_CHATBOX==GetCursorFocus() )
    {
        HandleChatEvent();
    }
    else
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EXIT_SQUARE,
            g_pkStrLoader->GetString( STR_ID_EXIT_SQUARE ),
            L"",        
            KGCUIScene::GC_MBOX_USE_EXIT );
    }
}

void KGCSquareOverlayUI::AddSquareUserListOrder( const KSquareUserInfo &userInfo )
{
    ASSERT( NULL != m_pkSquareUserList );
    m_pkSquareUserList->PushUserListVector( userInfo );
}

void KGCSquareOverlayUI::RemoveSquareUserListOrder( DWORD dwPeopleUID )
{
    ASSERT( NULL != m_pkSquareUserList );
    m_pkSquareUserList->PopUserListVector( dwPeopleUID );
}

void KGCSquareOverlayUI::ClearSquareUserListOrder()
{
    ASSERT( NULL != m_pkSquareUserList );
	if( m_pkSquareUserList )
		m_pkSquareUserList->ClearUserListVector();
}

bool KGCSquareOverlayUI::IsMouseControlAvailable( DWORD dwWhere ) const
{
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

    // 여기에 박스 등록
    switch ( dwWhere )
    {
        case SQUARE_FOCUS_WHEEL:
            if ( m_pkChatBox->CheckPosInWindowBound( vMousePos ) == true )
                return false;
            if ( ( m_pkSquareUserList->CheckPosInWindowBound( vMousePos ) && m_pkSquareUserList->GetRenderUserList() ) == true )
                return false;
            if( g_pkUIScene->m_pkNeonsignSelectBox->GetWndState() == KD3DWnd::D3DWS_ENABLED && 
                g_pkUIScene->m_pkNeonsignSelectBox->CheckPosInWindowBound( vMousePos ) == true )
                return false;
            break;

        case SQUARE_FOCUS_PICKING:
            if ( m_pkChatBoxBackGround->CheckPosInWindowBoundWithChild( vMousePos ) == true )
                return false;
            if ( ( m_pkSquareUserList->CheckPosInWindowBound( vMousePos ) && m_pkSquareUserList->GetRenderUserList() ) == true )
                return false;
            if ( m_pkButtonBackground->CheckPosInWindowBoundWithChild( vMousePos ) == true )
                return false;
            break;
    }

    return true;
}

void KGCSquareOverlayUI::Picking( int iMouseBtn )
{
    std::vector< KSquareUserInfo > vecUserInfo;
    vecUserInfo.clear();

	// 마우스 오른쪽 클릭했을때 선택된 아이들이 모두 vecUserInfo에 들어있다.
	if( iMouseBtn == KInput::MBRIGHT 
		|| false == g_pkSquareOverlayUI->m_pkSquarePickUserList->IsRenderOn() )
	{
		SiGCSquare()->Picking( vecUserInfo, iMouseBtn );
	}

	// 오른쪽 클릭된 유저에 대한 처리는 오른쪽 클릭을 했을 때만!!
	if( iMouseBtn == KInput::MBRIGHT )
	{
		m_pkSquarePickUserList->DeletePeople();

		// 아무도 없네!
		if ( vecUserInfo.empty() )
		{
			m_pkSquarePickUserList->ToggleRender( false );
			return;
		}

		// 누군가 클릭 됐다면~!
		// 유저 목록 UI 5명으로 제한 -_- 5명이 넘는다면 찍을때마다 랜덤으로 5명 추출해서 보여주쟈!
		std::vector< KSquareUserInfo >::iterator vit;
		if ( 6 > vecUserInfo.size() )
		{
			for ( vit = vecUserInfo.begin(); vit < vecUserInfo.end(); ++vit )
			{
				std::wstringstream strNick;
				int iRank = 0;
				if( iRank = g_kGlobalValue.m_kRanking.GetRankingFromNick( KGCRanking::RT_MONTHLY, vit->m_cCharType, vit->m_dwUID )  != -1 )
				{
					strNick<<vit->m_strNick<<L"#cFFFF00("<<g_pkStrLoader->GetReplacedString(STR_ID_RANKING10, "i", iRank )<<L")#cX";
				}
				else
					strNick<<vit->m_strNick;

				m_pkSquarePickUserList->AddPeople( strNick.str(), vit->m_dwUID, DT_LEFT );
			}
		}
		else
		{
			int iCount = 0;

			std::random_shuffle( vecUserInfo.begin(), vecUserInfo.end() );

			for ( vit = vecUserInfo.begin(); (vit < vecUserInfo.end()) && iCount < 5; ++vit, ++iCount )
			{
				std::wstringstream strNick;
				int iRank = 0;
				if( iRank = g_kGlobalValue.m_kRanking.GetRankingFromNick( KGCRanking::RT_MONTHLY, vit->m_cCharType, vit->m_dwUID )  != -1 )
				{
					strNick<<vit->m_strNick<<L"#cFFFF00("<<g_pkStrLoader->GetReplacedString(STR_ID_RANKING10, "i", iRank )<<L")#cX";
				}
				else
				{
					strNick<<vit->m_strNick;
				}
				m_pkSquarePickUserList->AddPeople( vit->m_strNick, vit->m_dwUID, DT_LEFT );
			}
		}

		// 사람들 이름 띄울 위치 잡고 랜더링~!
		// 기본적으로 마우스 클릭 위치에 띄우고, 인간 움직여도 가만히 놔두겠다!
		const POINT& ptMouse = g_pkInput->GetMousePos();
		D3DXVECTOR2 vMousePos( (float)ptMouse.x / m_fWindowScaleX, (float)ptMouse.y / m_fWindowScaleY);

		// 창 밖으로 넘어가는 리스트 조정하쟈! popup과 함께 처리했습니다.
		float fTmpX = vMousePos.x + m_pkSquarePickUserList->GetListWidth();
		float fTmpY = vMousePos.y + m_pkSquarePickUserList->GetListHeight();
		if ( GC_SCREEN_WIDTH < fTmpX)
			vMousePos.x -= ( fTmpX - GC_SCREEN_WIDTH + 10);
		if ( GC_SCREEN_HEIGHT < fTmpY )
			vMousePos.y -= ( fTmpY - GC_SCREEN_HEIGHT );

		m_pkSquarePickUserList->SetWindowPosDirect( vMousePos );
		m_pkSquarePickUserList->ToggleRender( true );
	}
}

void KGCSquareOverlayUI::SetNameOnUserlistForWhisper( const std::wstring& strNickName )
{
    m_pkChatBox->OnUserList( strNickName );
}

D3DXVECTOR2 KGCSquareOverlayUI::GetMissionButtonPos(void) const
{ 
	return m_pkMissionButton->GetCurrentWindowPos(); 
}

void KGCSquareOverlayUI::DisableMenuButtons()
{
    m_pkCoupleButton->ToggleRender( false );
    m_pkAttendanceCalendar->ToggleRender( false );
    m_pkMissionButton->ToggleRender( false );
    m_pkGuildBtn->ToggleRender( false );
    m_pkPartyButton->ToggleRender( false );
    m_pkMsnButton->ToggleRender( false );
    m_pkOptionButton->ToggleRender( false );
}