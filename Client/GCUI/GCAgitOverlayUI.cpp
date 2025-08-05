#include "stdafx.h"
#include "GCAgitOverlayUI.h"

#include "GCAgitInfoBar.h"
#include "GCChatBox.h"
#include "GCAgitPrePlace.h"
#include "GCAgitOption.h"
#include "GCAgitObjectPlaceCollectUI.h"
#include "GCAgitInviteVisit.h"
#include "GCSeedPlant.h"
#include "GCHarvestGauge.h"
#include "GCAgitBrownie.h"
#include "GCAgitMove.h"
#include "GCAgitMoneyInfo.h"
#include "GCFairyTreeLevelGauge.h"

#include "KGCAgitGameManager.h"
#include "GCSquareSayBalloon.h"

#include "KDInput.h"

IMPLEMENT_CLASSNAME( KGCAgitOverlayUI );
IMPLEMENT_WND_FACTORY( KGCAgitOverlayUI );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitOverlayUI, "gc_agit_overlay" );

KGCAgitOverlayUI* g_pkAgitOverlayUI = NULL;

KGCAgitOverlayUI::KGCAgitOverlayUI( void )
{
    g_pkAgitOverlayUI = this;

    m_pkChatBox = NULL;
    m_pkChatBoxBackGround = NULL;
    m_pkChatScrollBackground = NULL;   
    m_pkInputBack = NULL;
    m_pkChangeViewBtn = NULL;
    LINK_CONTROL( "agit_chat_box", m_pkChatBox );
    LINK_CONTROL( "chat_list_back", m_pkChatBoxBackGround );
    LINK_CONTROL( "scroll_background_bar", m_pkChatScrollBackground );
    LINK_CONTROL( "chat_input_back", m_pkInputBack );
    LINK_CONTROL( "chatbox_downBtn", m_pkChangeViewBtn );

    m_vBeforePos = D3DXVECTOR2(1,1);
    m_dwBeforeScrollHeight = 0;
    m_dwBeforeListHeight = 0;    
    m_dwBeforScrollBackground = 0;
    m_bDragWindowCheck = false;

    m_pkInfoBar = NULL;
    m_pkPrePlaceUI = NULL;
    m_pkAgitOption = NULL;
    m_pkPlaceCollectUI = NULL;
    m_pkInviteVisitUI = NULL;
    m_pkAgitList = NULL;
	m_pkAgitEnterPass = NULL;
    m_pkSeedPlantUI = NULL;
    m_pkHarvestGauge = NULL;
    m_pkAgitBrownieUI = NULL;
    m_pkAgitMoveUI = NULL;
    m_pkAgitMoneyInfo = NULL;
    m_pkInviteAgitReqDlg  =NULL;
    m_pkInviteAgitSellBox = NULL;
    m_pkGuestBook = NULL;
    m_pkTrainingUI = NULL;
    m_pkAgitCharSetting = NULL;
    m_pkFairyTreeLevelGauge = NULL;
    LINK_CONTROL( "agit_info_bar", m_pkInfoBar );
    LINK_CONTROL( "object_pre_place", m_pkPrePlaceUI );
    LINK_CONTROL( "agit_option", m_pkAgitOption );
    LINK_CONTROL( "object_place_collect", m_pkPlaceCollectUI );
    LINK_CONTROL( "agit_invite_visit", m_pkInviteVisitUI );
    LINK_CONTROL( "agit_list", m_pkAgitList );
    LINK_CONTROL( "seed_plant", m_pkSeedPlantUI );
    LINK_CONTROL( "harvest_gauge", m_pkHarvestGauge );
    LINK_CONTROL( "aigt_brownie", m_pkAgitBrownieUI );
    LINK_CONTROL( "agit_move", m_pkAgitMoveUI );
    LINK_CONTROL( "agit_money_info", m_pkAgitMoneyInfo );
    LINK_CONTROL( "invite_agit_req_dlg", m_pkInviteAgitReqDlg );
    LINK_CONTROL( "agit_sell_box", m_pkInviteAgitSellBox );
	LINK_CONTROL( "agitenterpass", m_pkAgitEnterPass );
    LINK_CONTROL( "agit_guestbook", m_pkGuestBook );
    LINK_CONTROL( "agit_training", m_pkTrainingUI );
    LINK_CONTROL( "agit_char_setting", m_pkAgitCharSetting );
    LINK_CONTROL( "fairyTree_gauge", m_pkFairyTreeLevelGauge );

	m_pkSayBalloon = NULL;
	m_pkBalloonTalkBtn1 = NULL;
	m_pkBalloonTalkBtn2 = NULL;
    m_pkBtnBrownie = NULL;	

	LINK_CONTROL( "say_balloon", m_pkSayBalloon );
	LINK_CONTROL( "balloon_talk_Btn1",           m_pkBalloonTalkBtn1 );
	LINK_CONTROL( "balloon_talk_Btn2",           m_pkBalloonTalkBtn2 );
    LINK_CONTROL( "btnBrownie", m_pkBtnBrownie );

    m_pkPlaceButton = NULL;
    m_pkShopButton = NULL;
    m_pkInviteButton = NULL;
    m_pkExitButton = NULL;
    m_pkAddFavoriteButton = NULL;
    m_pkMsnButton = NULL;
    m_pkPartyButton = NULL;
    m_pkReturnButton = NULL;
    LINK_CONTROL( "place_button",   m_pkPlaceButton );
    LINK_CONTROL( "shop_button",    m_pkShopButton );
    LINK_CONTROL( "invite_button",  m_pkInviteButton );
    LINK_CONTROL( "exit_button",    m_pkExitButton );
    LINK_CONTROL( "addFavorite_button", m_pkAddFavoriteButton );
    LINK_CONTROL( "msn_button", m_pkMsnButton );
    LINK_CONTROL( "party_button", m_pkPartyButton );
    LINK_CONTROL( "return_button", m_pkReturnButton );

	m_emHandleChatEvent = EM_CHAT_NONE;


	m_pkWorldBillboardChatBoard = NULL;
	LINK_CONTROL( "worldbillboardchat", m_pkWorldBillboardChatBoard );
}   

KGCAgitOverlayUI::~KGCAgitOverlayUI( void )
{
    g_pkAgitOverlayUI = NULL;
}

void KGCAgitOverlayUI::OnCreate( void )
{
    m_pkChatBox->InitState( true, true, this );
    m_pkChangeViewBtn->InitState( true, true, this );
    m_pkInputBack->InitState( true, false, NULL );
    m_pkChatBox->Clear();

    m_pkInfoBar->InitState( true, true, this );
    m_pkPrePlaceUI->InitState( false, true, this );
    m_pkAgitOption->InitState( false, true, this );
    if (m_pkPlaceCollectUI)
        m_pkPlaceCollectUI->InitState( false, true, this );
    m_pkInviteVisitUI->InitState( false, true, this );
    m_pkAgitList->InitState( false, true, this );
	m_pkAgitEnterPass->InitState( false, true, this );
    m_pkSeedPlantUI->InitState( false, true, this );
    m_pkHarvestGauge->InitState( false, false, NULL );
    m_pkAgitBrownieUI->InitState( false, true, this );
    m_pkAgitMoveUI->InitState( false, true, this );
    m_pkAgitMoneyInfo->InitState( true, false, NULL );
    m_pkInviteAgitReqDlg->InitState( false, true, this );
    m_pkInviteAgitSellBox->InitState( false, true, this );
    m_pkGuestBook->InitState( false, true, this );
    m_pkTrainingUI->InitState( false, true, this );
    m_pkAgitCharSetting->InitState( false, true, this );
    m_pkFairyTreeLevelGauge->InitState( true );

    m_pkPlaceButton->InitState( true, true, this );
    m_pkShopButton->InitState( true, true, this );
    m_pkInviteButton->InitState( true, true, this );
    m_pkAddFavoriteButton->InitState( true, true, this );
    m_pkMsnButton->InitState( true, true, this );
    m_pkPartyButton->InitState( true, true, this );
    m_pkReturnButton->InitState( false, true, this );
    m_pkExitButton->InitState( true, true, this );
    
    m_pkPlaceButton->SetToolTip( g_pkStrLoader->GetString( STR_ID_AGIT_MENU_PLACE ) ,0.0f,100.0f);
    m_pkShopButton->SetToolTip( g_pkStrLoader->GetString( STR_ID_AGIT_MENU_SHOP ) ,0.0f,100.0f);
    m_pkInviteButton->SetToolTip( g_pkStrLoader->GetString( STR_ID_AGIT_MENU_INVITE_VISIT ) ,0.0f,100.0f);
    m_pkAddFavoriteButton->SetToolTip( g_pkStrLoader->GetString( STR_ID_AGIT_MENE_ADD_FAVORITE ) ,0.0f,100.0f);
    m_pkMsnButton->SetToolTip( g_pkStrLoader->GetString( STR_ID_AGIT_MENU_MSN ) ,0.0f,100.0f);
    m_pkPartyButton->SetToolTip( g_pkStrLoader->GetString( STR_ID_AGIT_MENU_PARTY ) ,0.0f,100.0f);
    m_pkReturnButton->SetToolTip( g_pkStrLoader->GetString( STR_ID_AGIT_MENU_RETURN ) ,0.0f,100.0f);
    m_pkExitButton->SetToolTip( g_pkStrLoader->GetString( STR_ID_SQUARE_LEAVE ) ,0.0f,100.0f);
    
    m_pkExitButton->SetHotKey( DIK_ESCAPE );

    m_pkBtnBrownie->InitState( true, true, this );

	ToggleRender(false);
	m_pkChatBox->Clear();
	ReLocate();

	if( m_pkWorldBillboardChatBoard )
	{
		m_pkWorldBillboardChatBoard->InitState( true, true, this );
		m_pkWorldBillboardChatBoard->ToggleButton( false );
		m_pkWorldBillboardChatBoard->SetBlink( false );
	}
}

void KGCAgitOverlayUI::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkChangeViewBtn ) 
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
	
	GCWND_MSG_MAP( m_pkBtnBrownie,      KD3DWnd::D3DWE_BUTTON_CLICK,    OnBtnBrownie );
    GCWND_MSG_MAP( m_pkAgitBrownieUI,   KD3DWnd::D3DWE_BUTTON_CLICK,    OnCloseBrownie );

    GCWND_MSG_MAP( m_pkPlaceButton,     KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickPlaceBtn );
    GCWND_MSG_MAP( m_pkShopButton,      KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickShopBtn );
    GCWND_MSG_MAP( m_pkInviteButton,    KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickInviteBtn );
    GCWND_MSG_MAP( m_pkAddFavoriteButton, KD3DWnd::D3DWE_BUTTON_CLICK,  OnClickAddFavorite );
    GCWND_MSG_MAP( m_pkMsnButton,       KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickMsnButton );
    GCWND_MSG_MAP( m_pkPartyButton,     KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickParty );
    GCWND_MSG_MAP( m_pkReturnButton,    KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickReturnButton );

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
		else if( event.m_pWnd == m_pkExitButton )
		{
            g_KDSound.Play( "984038" );
            if( false == SiKGCAgitGameManager()->GetWatchState() && false == IsRenderOnAgitUI() )
			    ModalExitMsgBox();
            else
            {
                if( SiKGCAgitGameManager()->GetWatchState() )
                    SiKGCAgitGameManager()->LeaveWatchAgit();

                m_pkBtnBrownie->ToggleRender( true );
                m_pkPlaceButton->ToggleRender( true );
                m_pkShopButton->ToggleRender( true );
                m_pkInviteButton->ToggleRender( true );
                m_pkExitButton->ToggleRender( true );
                m_pkMsnButton->ToggleRender( true );
                m_pkPartyButton->ToggleRender( true );
            }
		}
	}

	if (event.m_dwCode==KD3DWnd::EWNDMESSAGE_CLOSE || event.m_dwCode==KGCUIScene::GC_MBOX_RETURN_OK)
	{
		EndMessageBox();
	}
}

void KGCAgitOverlayUI::FrameMoveInEnabledState( void )
{
    if( g_MyD3D->m_pStateMachine->GetState() != GS_GAME )
    {
        CloseAllUI();
        this->ToggleRender( false );
    }
	TimeCheck();

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
			
        }
    }
	ReLocate();



	if (ECF_MAINWND==GetCursorFocus() )
	{
		if ( GetChatEventHandling() == EM_CHAT_RETURN )
		{
			SetChatEventHandling( EM_CHAT_NONE );
		}
		else
		{
			//메인 모드일때 아닌데, 엔터친 경우
			if ( (diks[DIK_RETURN] || diks[DIK_NUMPADENTER]) && !(Old_diks[DIK_RETURN] || Old_diks[DIK_NUMPADENTER] ))
			{
				SetEditFocus();
			}			
		}

	}

	POINT       pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

	// 유저리스트의 팝업창 이외에 딴거 클릭하면 꺼지쟈!
	if ( g_pkInput->BtnUp( KInput::MBLEFT ) && !m_pkInfoBar->CheckPosPopupBoxBound( vMousePos ) )
		m_pkInfoBar->SetPopupBoxRender( false );

}


void KGCAgitOverlayUI::Init( DWORD dwAgitUID )
{
    m_pkInfoBar->SetAgitInfo();

	m_pkBalloonTalkBtn1->InitState(true,true,this);
	m_pkBalloonTalkBtn2->InitState(false,false,NULL);
	m_pkSayBalloon->InitState(true,true,NULL);

    ToggleMainUI( true );
    SetMenuButton( dwAgitUID );
    ClearChatList();
	ReLocate();
}

void KGCAgitOverlayUI::ToggleMainUI( bool bShow_ )
{
    m_pkInfoBar->ToggleRender( bShow_ );
    m_pkChatBox->ToggleRender( bShow_ );
    m_pkAgitMoneyInfo->ToggleRender( bShow_ );
    m_pkChatBoxBackGround->ToggleRender( bShow_ );
    m_pkChatScrollBackground->ToggleRender( bShow_ );
    m_pkChangeViewBtn->ToggleRender( bShow_ );
    m_pkInputBack->ToggleRender( bShow_ );

    m_pkBalloonTalkBtn1->ToggleRender( bShow_ );
    m_pkBalloonTalkBtn2->ToggleRender( bShow_ );

    m_pkPlaceButton->ToggleRender( bShow_ );
    m_pkShopButton->ToggleRender( bShow_ );
    m_pkInviteButton->ToggleRender( bShow_ );
    m_pkExitButton->ToggleRender( bShow_ );
    m_pkMsnButton->ToggleRender( bShow_ );
    m_pkPartyButton->ToggleRender( bShow_ );
    m_pkReturnButton->ToggleRender( bShow_ );    

    if( bShow_ )
    {
        if( SiKGCAgitGameManager()->GetAgitUID() == g_kGlobalValue.m_kUserInfo.dwUID )
        {
            m_pkAddFavoriteButton->ToggleRender( false );
            m_pkReturnButton->ToggleRender( false );
        }
        else
        {
            m_pkBtnBrownie->ToggleRender( false );
            m_pkPlaceButton->ToggleRender( false );
        }
    }

    m_pkChatBox->InitColorButton();
}

void KGCAgitOverlayUI::CloseAllUI( void )
{
    ShowAgitOption( false );

    if( m_pkPlaceCollectUI->IsRenderOn() )
        ShowAgitPlaceCollectUI( false );

    ShowAgitInviteVisitUI( false );
    CloseInviteVisitUI();
    ShowSeedPlant( false );
    OnCloseBrownie();
    ShowAgitMove( false );
    ShowTrainingUI( false, KItem::INVALID_ITEM );
    ShowGuestBook( false );
    g_pkUIScene->m_pkAgitShop->OnClose();
	m_pkAgitEnterPass->ToggleRender( false );

    m_pkPrePlaceUI->Close();

    if( SiKGCAgitGameManager()->GetWatchState() )
    {
        m_pkBtnBrownie->ToggleRender( false );
        m_pkAddFavoriteButton->ToggleRender( false );
        m_pkPlaceButton->ToggleRender( false );
        m_pkShopButton->ToggleRender( false );
        m_pkInviteButton->ToggleRender( false );
        m_pkAddFavoriteButton->ToggleRender( false );
        m_pkMsnButton->ToggleRender( false );
        m_pkPartyButton->ToggleRender( false );
    }

    if( SiKGCAgitGameManager()->GetAgitUID() != g_kGlobalValue.m_kUserInfo.dwUID )
        m_pkBtnBrownie->ToggleRender( false );
}

void KGCAgitOverlayUI::ShowPrePlaceUI( bool bShow_, GCItem* pItem_, int iMoneyType_, int iPrice_, PAIR_USHORT_DWORD pairIndex_ ) 
{
    if( bShow_ )
        m_pkPrePlaceUI->SetPrePlaceInfo( pItem_, iMoneyType_, iPrice_, pairIndex_ );
    else
    {
        m_pkPrePlaceUI->ToggleRender( false );
        m_pkAgitCharSetting->ToggleRender( false );
    }
}

void KGCAgitOverlayUI::ShowAgitOption( bool bShow_, int iType_ )
{
    if( bShow_ )
        m_pkAgitOption->InitAgitOption( SiKGCAgitGameManager()->GetAgitInfo(), iType_ );
    m_pkAgitOption->ToggleRender( bShow_ );
}

void KGCAgitOverlayUI::UpdateAgitInfo( void )
{
    m_pkInfoBar->SetAgitInfo();
}

void KGCAgitOverlayUI::ShowAgitPlaceCollectUI( bool bShow_ )
{
    if( bShow_ )
    {
        CloseAllUI();
        ToggleMainUI( false );
        m_pkPlaceCollectUI->InitUI();
        m_pkPlaceCollectUI->ToggleRender( true );
        m_pkBtnBrownie->ToggleRender( false );
    }
    else
    {
        m_pkPlaceCollectUI->OnClose();
    }
}

void KGCAgitOverlayUI::ShowAgitInviteVisitUI( bool bShow_ )
{
    m_pkInviteVisitUI->ToggleRender( bShow_ );
}

void KGCAgitOverlayUI::ShowAgitList( bool bShow_ )
{
	m_pkAgitList->ToggleRender( bShow_ );
}

void KGCAgitOverlayUI::ShowAgitEnterPass ( DWORD dwAgitID )
{
	m_pkAgitEnterPass->VisibleUI( dwAgitID );
}

void KGCAgitOverlayUI::SetMaxChannel( int nMaxChannel_ )
{
	m_pkAgitList->SetMaxChannel( nMaxChannel_ );
}

void KGCAgitOverlayUI::ReqUpdateFavorityPage()
{
	m_pkAgitList->ReqUpdateFavoritePage();
}


void KGCAgitOverlayUI::SetRegisterAgitList( KAgitPageList& PageInfo_, std::vector< KAgitPageInfo >& vecPageList_ )
{
	m_pkAgitList->SetRegisterAgitList( PageInfo_, vecPageList_ );
}

void KGCAgitOverlayUI::SetFriendAgitList( std::vector< KAgitPageInfo >& vecPageList_ )
{
	m_pkAgitList->SetFriendAgitList( vecPageList_ );
}

void KGCAgitOverlayUI::SetFavoriteAgitList( std::vector< KAgitPageInfo >& vecPageList_ )
{
	m_pkAgitList->SetFavoriteAgitList( vecPageList_ );
}

void KGCAgitOverlayUI::SetRankAgitList( std::vector< KAgitPageInfo >& vecPageList_ )
{
	m_pkAgitList->SetRankAgitList( vecPageList_ );
}

void KGCAgitOverlayUI::ShowSeedPlant( bool bshow_ )
{
    if( bshow_ ){
        CloseAllUI();
        m_pkSeedPlantUI->InitUI();
        g_pkUIScene->m_pkGuideAzit->SetCompleateActionNum(3);
    }
    m_pkSeedPlantUI->ToggleRender( bshow_ );
}

bool KGCAgitOverlayUI::MouseInUI( D3DXVECTOR2& dxvPos_ )
{
    if( m_pkPrePlaceUI->IsRenderOn() && m_pkPrePlaceUI->CheckPosInWindowBoundWithChild( dxvPos_, true ) )
        return true;
    else if( m_pkPlaceCollectUI->IsRenderOn() && m_pkPlaceCollectUI->CheckPosInWindowBoundWithChild( dxvPos_, true ) )
        return true;
    else if( m_pkAgitCharSetting->IsRenderOn() && m_pkAgitCharSetting->CheckPosInWindowBoundWithChild( dxvPos_, true ) )
        return true;
    else if( m_pkAgitCharSetting->IsRenderOn() && m_pkAgitCharSetting->CheckPosInWindowBoundWithChild( dxvPos_, true ) )
        return true;
    else
    {
        KD3DWnd* pCurMsgBox = g_pkUIScene->GetCurMsgBox();
        if( pCurMsgBox->GetWndState() == KD3DWnd::D3DWS_ENABLED && pCurMsgBox->IsRenderOn() && pCurMsgBox->CheckPosInWindowBoundWithChild( dxvPos_ ) )
            return true;
    }

    return false;
}

void KGCAgitOverlayUI::UpdatePlaceCollectUI( void )
{
    if (m_pkPlaceCollectUI != NULL)
        return;

    if( false == m_pkPlaceCollectUI->IsRenderOn() )
        return;

    m_pkPlaceCollectUI->UpdateData();
    m_pkPlaceCollectUI->SetSlot();
}

void KGCAgitOverlayUI::OnExitAgit( void )
{
    ToggleMainUI( false );
    CloseAllUI();
}

void KGCAgitOverlayUI::CloseInviteVisitUI( void )
{
    ShowAgitInviteVisitUI( false );
    ShowAgitList( false );
}

bool KGCAgitOverlayUI::IsUseKeyCheck( void )
{
	if ( m_pkAgitOption->IsRenderOn() )
		return true;
    
	if ( GetCursorFocus() != ECF_MAINWND )
		return true;

	return false;
}

void KGCAgitOverlayUI::ShowHarvestGauge( bool bShow_, float fPosX_ /* = -1.0f */, float fPosY_ /* = -1.0f */ )
{
    if( bShow_ )
        m_pkHarvestGauge->Init( D3DXVECTOR2( fPosX_, fPosY_ ) );
    
    m_pkHarvestGauge->ToggleRender( bShow_ );
}

void KGCAgitOverlayUI::ShowBrownie( bool bShow_ )
{
    if( bShow_ )
    {
        m_pkAgitBrownieUI->InitUI();
        m_pkBtnBrownie->ToggleRender( false );
    }

    m_pkAgitBrownieUI->ToggleRender( bShow_ );
}

void KGCAgitOverlayUI::ShowAgitMove( bool bshow_ )
{
    if( bshow_ )
        m_pkAgitMoveUI->InitUI( SiKGCAgitGameManager()->GetMapCatalog() );

    m_pkAgitMoveUI->ToggleRender( bshow_ );
}

bool KGCAgitOverlayUI::IsRenderOnHarvestUI( void )
{
    return m_pkHarvestGauge->IsRenderOn();
}

bool KGCAgitOverlayUI::HarvestStop( void )
{
    if( m_pkHarvestGauge->IsRenderOn() )
        return m_pkHarvestGauge->Stop();

    return false;
}



KGCAgitOverlayUI::ECursorFocus KGCAgitOverlayUI::GetCursorFocus( void ) const
{
	ECursorFocus eRet;
	if (g_hUIMainWnd==::GetFocus())
	{
		eRet=KGCAgitOverlayUI::ECF_MAINWND;
	}
	else if (m_pkChatBox->HasEditFocus()==true)
	{
		eRet=KGCAgitOverlayUI::ECF_CHATBOX;
	}
	else
	{
		eRet=KGCAgitOverlayUI::ECF_UNKNOWN;
	}     
	return eRet;
}

void KGCAgitOverlayUI::TimeCheck( void ) const
{
	m_pkSayBalloon->TimeCheck(); //말풍선 타임체크
}

void KGCAgitOverlayUI::ClearFocus( void )
{	    
	::SetFocus( g_hUIMainWnd );
}

void KGCAgitOverlayUI::ClearChatEvent( void )
{
	g_KDSound.Play( "72" );
	m_pkChatBox->m_pkEdit->SetText(L"");
	::SetFocus( g_hUIMainWnd );
}

void KGCAgitOverlayUI::SetEditFocus( void )
{
	if ( m_pkChatBox != NULL )
	{
		m_pkChatBox->SetFocus();
	}
}

void KGCAgitOverlayUI::ReLocate()
{
	D3DXVECTOR2 vPos;
	vPos.x = m_pkChangeViewBtn->GetFixedWindowPos().x;
	vPos.y = m_pkChatBox->GetScrollPosY() + static_cast<FLOAT>(m_pkChatBox->GetScrollHeight() + m_pkChatBox->GetScrollBtnGap()*3);
	m_pkChangeViewBtn->SetAllWindowPos(vPos);    
}

void KGCAgitOverlayUI::EndMessageBox(void)
{
	g_pkInput->IgnoreDown(DIK_ESCAPE,30);
	m_pkChatBox->SetChatListClick(true);
	::SetFocus( g_hUIMainWnd );
}


void KGCAgitOverlayUI::ReLocateBalloon( D3DXVECTOR2 vPos , DWORD dwUID, int BalloonDistance)
{	        
	m_pkSayBalloon->SetBalloonPos(vPos,dwUID, KGCSayBalloonSquare::GCSB_ARROW_POS_AUTO, BalloonDistance);
}

void KGCAgitOverlayUI::SetBalloonMsg( std::wstring strMsg ,DWORD dwCharUID, KChatData::ENUM_CHAT_TYPE eMsgType, DWORD dwChatColor )
{
    m_pkSayBalloon->NewUserMsg(strMsg, dwCharUID, eMsgType, dwChatColor);
}

void KGCAgitOverlayUI::DeleteBalloonMsg( DWORD dwCharUID)
{
	m_pkSayBalloon->DeleteUserMsg(dwCharUID);
}

void KGCAgitOverlayUI::DeleteAllSayBalloon()
{
	m_pkSayBalloon->DeleteAllBalloon();
}


void KGCAgitOverlayUI::ModalExitMsgBox( void )
{
	if( KGCAgitOverlayUI::ECF_CHATBOX==GetCursorFocus() )
	{
		ClearChatEvent();
		SetChatEventHandling( EM_CHAT_ESC );
	}
	else
	{
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EXIT_GAME,
        g_pkStrLoader->GetString( STR_ID_EXIT_AGIT ), 
        L"", KGCUIScene::GC_MBOX_USE_EXIT );

		SetChatEventHandling(EM_CHAT_NONE);
	}
}


void KGCAgitOverlayUI::OnBtnBrownie( void )
{
    CloseAllUI();
    ShowBrownie( true );
}

void KGCAgitOverlayUI::OnCloseBrownie( void )
{
    m_pkAgitBrownieUI->ToggleRender( false );
    m_pkAgitBrownieUI->ClearAllParticle();

    m_pkAgitOption->ToggleRender( false );
    m_pkAgitMoveUI->ToggleRender( false );

    m_pkBtnBrownie->ToggleRender( true );
}

void KGCAgitOverlayUI::UpdateMoneyInfo( void )
{
    if( m_pkAgitMoneyInfo )
        m_pkAgitMoneyInfo->UpdateAgitMoney();
}

void KGCAgitOverlayUI::UpdatShopPage( void )
{
	if ( g_pkUIScene->m_pkAgitShop ) 
		g_pkUIScene->m_pkAgitShop->UpdatePage();
}

void KGCAgitOverlayUI::OnClickPlaceBtn( void )
{
    if( false == m_pkPlaceCollectUI->IsRenderOn() )
    {
        g_KDSound.Play( "984038" );
        ShowAgitPlaceCollectUI( true );
    }
}

void KGCAgitOverlayUI::OnClickShopBtn( void )
{
    // 구경중 상점 안되게 
    if( SiKGCAgitGameManager()->GetWatchState() ) 
        return;

    CloseAllUI();
    g_pkUIScene->m_pkAgitShop->InitAgitShop( true );
    g_KDSound.Play( "984038" );
    m_pkBtnBrownie->ToggleRender( false );
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_AGIT_SHOP, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);//, true );
}

void KGCAgitOverlayUI::OnClickInviteBtn( void )
{
    // 구경중에 초대방문 안되게 
    if( SiKGCAgitGameManager()->GetWatchState() )
        return;

    CloseAllUI();
    if( false == m_pkInviteVisitUI->IsRenderOn() )
    {
        g_KDSound.Play( "984038" );
        ShowAgitInviteVisitUI( true );
        m_pkBtnBrownie->ToggleRender( false );
    }
}

void KGCAgitOverlayUI::UpdatePrePlaceUI( void )
{
    m_pkPrePlaceUI->UpdateData();
}

void KGCAgitOverlayUI::ShowInviteAgitReqDlg( bool bShow_ )
{
    if( bShow_ )
        m_pkInviteAgitReqDlg->InitInfo();

    m_pkInviteAgitReqDlg->ToggleRender(bShow_);
}

void KGCAgitOverlayUI::ShowAgitSellBox( bool bShow_, std::pair<GCITEMID, GCITEMUID> prItemInfo_ )
{
    if( bShow_ )
        m_pkInviteAgitSellBox->SetSellItem( prItemInfo_ );

    m_pkInviteAgitSellBox->ToggleRender(bShow_);
}

void KGCAgitOverlayUI::OnClickAddFavorite( void )
{
    if( SiKGCAgitGameManager()->GetAgitUID() == g_kGlobalValue.m_kUserInfo.dwUID )
        return;

    g_KDSound.Play( "984038" );
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_AGIT_FACVORITE_ADD ), L"", KGCUIScene::GC_MBOX_USE_ADD_FAVORITE_AGIT, 0, 0, true, true);
}

bool KGCAgitOverlayUI::IsRenderOnMoveUI( void )
{
    return m_pkAgitMoveUI->IsRenderOn();
}

bool KGCAgitOverlayUI::IsRenderOnOptionUI( void )
{
    return m_pkAgitOption->IsRenderOn();
}

bool KGCAgitOverlayUI::IsRenderOnAgitUI( void )
{
    return ( m_pkPrePlaceUI->IsRenderOn() ||
             m_pkAgitOption->IsRenderOn() ||
             m_pkPlaceCollectUI->IsRenderOn() ||
             m_pkInviteVisitUI->IsRenderOn() ||
             m_pkAgitList->IsRenderOn() ||
             m_pkSeedPlantUI->IsRenderOn() ||
             m_pkAgitBrownieUI->IsRenderOn() ||
             m_pkAgitMoveUI->IsRenderOn() ||
             m_pkInviteAgitSellBox->IsRenderOn() || 
             m_pkHarvestGauge->IsRenderOn() ||
             m_pkGuestBook->IsRenderOn() ||
             m_pkTrainingUI->IsRenderOn() 
            );
}

void KGCAgitOverlayUI::OnClickMsnButton( void )
{
    g_MyD3D->OnMSNButton();
    m_pkBtnBrownie->ToggleRender( false );
}

void KGCAgitOverlayUI::ClearChatList()
{
    m_pkChatBox->Clear();
}

void KGCAgitOverlayUI::OnClickParty( void )
{
    if (!SiKGCPartyManager()->IsParty())
        return;

    if ( g_pkUIScene->m_pkPartyWnd->IsRenderOn() )
    {
        g_pkUIScene->m_pkPartyWnd->OffPartyDlg();
    }
    else
    {
        g_pkUIScene->m_pkPartyWnd->OnPartyDlg();
    }
}

void KGCAgitOverlayUI::ToggleRenderBrownieBtn( bool bRender_ )
{
    if( SiKGCAgitGameManager()->GetAgitUID() != g_kGlobalValue.m_kUserInfo.dwUID ) { 
        if( m_pkBtnBrownie ) 
            m_pkBtnBrownie->ToggleRender( false );

        return;
    }

    if( m_pkBtnBrownie )
        m_pkBtnBrownie->ToggleRender( bRender_ );
}

void KGCAgitOverlayUI::ShowGuestBook( bool bShow_ )
{
    if( bShow_ )
    {
        CloseAllUI();
        m_pkGuestBook->InitUI();
    }

    m_pkGuestBook->ToggleRender( bShow_ );
}

void KGCAgitOverlayUI::SetGuestBookInfo( std::map<int, KGuestMsg>& info_ )
{
    m_pkGuestBook->SetGuestbookInfo( info_ );
}

void KGCAgitOverlayUI::AddNewGuestbookMsg( int iMsgNum_, KGuestMsg Msg_ )
{
    if( m_pkGuestBook->IsRenderOn() )
        m_pkGuestBook->AddNewMessage( iMsgNum_, Msg_ );
}

void KGCAgitOverlayUI::DeleteGuestBookMsg( std::set<int>& steDeleteID )
{
    if( m_pkGuestBook->IsRenderOn() )
        m_pkGuestBook->DeleteMessage( steDeleteID );
}

void KGCAgitOverlayUI::ShowTrainingUI( bool bshow_,GCITEMUID itemUID )
{
    if( bshow_ )
    {
        CloseAllUI();
        m_pkTrainingUI->InitUI(itemUID);
    }

    m_pkTrainingUI->ToggleRender( bshow_ );
}

void KGCAgitOverlayUI::OnClickReturnButton( void )
{
    // 나가고 
    int iPlayer = g_MyD3D->Get_MyPlayer();
    if( g_MyD3D->IsPlayerIndex( iPlayer ) )
        g_MyD3D->MyPlayer[iPlayer]->SetSummonMonsterHP_SendHost( 0.0f, -1, true );

    while( g_pkUIScene->RemoveWaitMessageBox( KGCUIScene::GC_MBOX_DOTA_SHOP ) );

    g_MyD3D->m_pStateMachine->OnExitGame();

    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT )
    {
        g_pkAgitOverlayUI->OnExitAgit();

        if ( SiKGCPartyManager()->IsParty() )
            SiKGCPartyManager()->LeaveParty();
    }

    // 내 아지트로 
    SiKGCAgitGameManager()->EnterMyAgit();
}

void KGCAgitOverlayUI::SetMenuButton( DWORD dwAgitUID )
{
    // 주인이냐 손님이냐 따라 뜨는 버튼들 세팅 
    if( dwAgitUID == g_kGlobalValue.m_kUserInfo.dwUID ) // 주인
    {
        m_pkPlaceButton->SetFixedWindowLocalPos( D3DXVECTOR2( 579.0f * GC_SCREEN_DIV_WIDTH, 2.0f ) );
        m_pkShopButton->SetFixedWindowLocalPos( D3DXVECTOR2( 615.0f* GC_SCREEN_DIV_WIDTH, 2.0f ) );
        m_pkInviteButton->SetFixedWindowLocalPos( D3DXVECTOR2( 652.0f* GC_SCREEN_DIV_WIDTH, 2.0f ) );
        m_pkPartyButton->SetFixedWindowLocalPos( D3DXVECTOR2( 688.0f* GC_SCREEN_DIV_WIDTH, 3.0f ) );
        m_pkMsnButton->SetFixedWindowLocalPos( D3DXVECTOR2( 726.0f* GC_SCREEN_DIV_WIDTH, 3.0f ) );
        
        m_pkPlaceButton->ToggleRender( true );
        m_pkShopButton->ToggleRender( true );
        m_pkBtnBrownie->ToggleRender( true );
        m_pkInviteButton->ToggleRender( true );
        m_pkMsnButton->ToggleRender( true );
        m_pkPartyButton->ToggleRender( true );
        m_pkAddFavoriteButton->ToggleRender( false );
        m_pkReturnButton->ToggleRender( false );
    }   
    else    // 손님
    {
        m_pkAddFavoriteButton->SetFixedWindowLocalPos( D3DXVECTOR2( 581.0f* GC_SCREEN_DIV_WIDTH, 2.0f ) );
        m_pkInviteButton->SetFixedWindowLocalPos( D3DXVECTOR2( 616.0f* GC_SCREEN_DIV_WIDTH, 2.0f ) );
        m_pkPartyButton->SetFixedWindowLocalPos( D3DXVECTOR2( 652.0f* GC_SCREEN_DIV_WIDTH, 2.0f ) );
        m_pkMsnButton->SetFixedWindowLocalPos( D3DXVECTOR2( 690.0f* GC_SCREEN_DIV_WIDTH, 3.0f ) );
        m_pkReturnButton->SetFixedWindowLocalPos( D3DXVECTOR2( 724.0f* GC_SCREEN_DIV_WIDTH, 2.0f ) );

        m_pkPlaceButton->ToggleRender( false );
        m_pkShopButton->ToggleRender( false );
        m_pkBtnBrownie->ToggleRender( false );
        m_pkInviteButton->ToggleRender( true );
        m_pkMsnButton->ToggleRender( true );
        m_pkPartyButton->ToggleRender( true );
        m_pkAddFavoriteButton->ToggleRender( true );
        m_pkReturnButton->ToggleRender( true );
    }
}

GCITEMUID KGCAgitOverlayUI::GetTraingObjUID( void )
{
    if( m_pkTrainingUI )
        return m_pkTrainingUI->GetTrainigObjUID();

    return KItem::INVALID_ITEM;
}

void KGCAgitOverlayUI::ShowCharSettingUI( bool bShow_, int iCharType_ /*= GC_CHAR_ELESIS */ )
{
    if( bShow_ )
        m_pkAgitCharSetting->InitUI(iCharType_);

    m_pkAgitCharSetting->ToggleRender( bShow_ );
}

void KGCAgitOverlayUI::SetFariyTreeLevelGauge( bool bShow_, int iLevel_ /*= 0*/, DWORD dwCurExp_ /*= 0*/, DWORD dwNextLevelExp_ /*= 0*/, D3DXVECTOR2& dxvPos_ /*= D3DXVECTOR2( 0.0f, 0.0f )*/, D3DXVECTOR2& dxvOffset_ /*= D3DXVECTOR2( 0.0f, 0.0f ) */ )
{
    m_pkFairyTreeLevelGauge->ToggleRender( bShow_ );
    m_pkFairyTreeLevelGauge->SetFairyTreeLevelGauge( iLevel_, dwCurExp_, dwNextLevelExp_, dxvPos_, dxvOffset_ );
}

void KGCAgitOverlayUI::UpdateFairyTreeLevelGaugePos( void )
{
    if( m_pkFairyTreeLevelGauge->IsRenderOn() ) { 
        m_pkFairyTreeLevelGauge->UpdateFairyTreeLevelGaugePos();
    }
}

void KGCAgitOverlayUI::RefreshCharacterTab( bool bIsTrue )
{
    m_pkPlaceCollectUI->RefreshCharTab( bIsTrue );
}
