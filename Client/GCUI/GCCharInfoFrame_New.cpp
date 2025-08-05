#include "stdafx.h"

#include "MyD3D.h"

#include "KGCRoomManager.h"
#include "GCIconImageManager.h"

#include "GCUI/GCCharIcon.h"

#include "GCUI/GCAccountNickNameFrame.h"
#include "KGCGiftBoxManager.h"

// 25/04/2021 dragonares: platform view com personagem 


#include "GCCharInfoFrame_New.h"

//
//char temp[MAX_PATH];
//sprintf( temp, "char%d", i );
//LINK_CONTROL( temp, m_pkCharIcon[i] );

IMPLEMENT_CLASSNAME( KGCCharInfoFrame_New );
IMPLEMENT_WND_FACTORY( KGCCharInfoFrame_New );
IMPLEMENT_WND_FACTORY_NAME( KGCCharInfoFrame_New, "gc_char_info_frame_new2" );

KGCCharInfoFrame_New::KGCCharInfoFrame_New( void )
{
	m_pkBtn_Confirm = NULL;
	LINK_CONTROL( "btn_confirm", m_pkBtn_Confirm );

	m_pkCharWnd = NULL;
	LINK_CONTROL( "character_window", m_pkCharWnd );

    m_pkBtnMoveChannel = NULL;
    LINK_CONTROL( "btn_move_channel", m_pkBtnMoveChannel );

    m_pkNickNameFrame = NULL;
    LINK_CONTROL( "nickname_frame", m_pkNickNameFrame );

    m_pkBackGroundWhite = NULL;
    LINK_CONTROL( "background_white", m_pkBackGroundWhite );

    m_pkNameBack = NULL;
    LINK_CONTROL( "name_back", m_pkNameBack );

    m_pkVdepot = NULL;
    LINK_CONTROL( "btn_vdepot", m_pkVdepot );

    m_pkSloatAddIcon = NULL;
    LINK_CONTROL( "add_slot_icon", m_pkSloatAddIcon );

    m_CharActiveBar = NULL;
    LINK_CONTROL("char0_active", m_CharActiveBar);
    m_CharBar = NULL;
    LINK_CONTROL("char0", m_CharBar);
   
    m_TitleMP = NULL;
    LINK_CONTROL("section_title_mp", m_TitleMP);

    m_TitleAP = NULL;
    LINK_CONTROL("section_title_ap", m_TitleAP);
    
    m_TitleHybrid = NULL;
    LINK_CONTROL("section_title_hybrid", m_TitleHybrid);

    m_pkBtn_Make_char = NULL;
    LINK_CONTROL("btn_add_char", m_pkBtn_Make_char);
}

KGCCharInfoFrame_New::~KGCCharInfoFrame_New( void )
{
}

KProtectedType<int>* m_iTotalSlot;

void KGCCharInfoFrame_New::OnCreate(void)
{
    m_CharActiveBar->ToggleRender(false);
    m_TitleMP->ToggleRender(false);
    m_TitleAP->ToggleRender(false);
    m_TitleHybrid->ToggleRender(false);
    m_CharBar->ToggleRender(false);


    m_pkCharWnd->InitState(true,true);
    m_pkCharWnd->SetZoom(0.13f);
    m_pkCharWnd->SetUseRoomObject(false);
    m_pkCharWnd->FreezeRoomRotation(true);

    m_pkNickNameFrame->InitState( false , true , this );
    m_pkBackGroundWhite->InitState( false );
    m_pkBackGroundWhite->ForceSetWindowColor( D3DCOLOR_ARGB( 0xb0, 0x00, 0x00, 0x00 ) );

    m_pkVdepot->InitState( true, true, this );
    m_pkVdepot->SetToolTip( g_pkStrLoader->GetString( STR_ID_VIRTUAL_DEPOT_MSG3 ), 0.0f, 0.0f, WNDUIAT_EXPANTION, ED3DWND_TOOLTIP_ALIGN_RIGHT );


    m_pkBtnMoveChannel->InitState(true, true, this);

    m_pkBtn_Confirm->InitState(true, true, this);
    m_pkBtn_Make_char->InitState(true, true, this);

    //GCWND_MSG_MAP( m_pkSloatAddIcon, KD3DWnd::D3DWE_BUTTON_CLICK, OnAddSlot );

    //m_vecSlotList = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_scene_char_select_new.stg", "", "char_select_scene_new\\char_info_frame_new2\\char0", true, GC_CHAR_NUM);
}


void KGCCharInfoFrame_New::OnMoveChannel()
{
    if( g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT_FROM_ROOM )
    {
        g_MyD3D->m_pStateMachine->GoRoom();
        g_pkUIHelper->SetUILockTime( LOCK_ROOM, BUTTON_LOCK_FRAME );
        KP2P::GetInstance()->Send_Into_Myinfo( g_kGlobalValue.m_kUserInfo.dwUID, g_kGlobalValue.m_kUserInfo.strLogin, GC_RUS_WAIT );
    }
    else
    {
        g_MyD3D->OnServerSceneBtn();
    }
}
void KGCCharInfoFrame_New::GoChannel()
{
    // ´øÀü ¼­¹ö°¡ ¾Æ´Ï¸é 
    if( g_kGlobalValue.m_bSendServerInfoNot == false )
    {
        KP2P::GetInstance()->Send_CharSelectJoinReq();

        //ÃÊÀÌ½ºµå¶ø ¾ÆÀÌÅÛ ¸ñ·Ï ÁÖ¼¼¿ä
        KP2P::GetInstance()->Send_ChoiceBoxListReq();


        //°æÇèÄ¡¾ÆÀÌÅÛ Á¤º¸ ÁÖ¼¼¿ä.
        KP2P::GetInstance()->Send_ExpPotionListReq();
#if defined( USE_AGIT_SYSTEM )
        SiKGCAgitGameManager()->SetAgitShop();              // ¾ÆÁöÆ® ¼¥ Á¤º¸¿Í ¸Ê ±¸¸Å Á¤º¸¸¦ ¹Þ¾Æ¿ÀÀÚ!
        KP2P::GetInstance()->Send_FairyTreeLvTableReq();    // ¿äÁ¤ÀÇ ³ª¹« °æÇèÄ¡ Å×ÀÌºíµµ ¹Þ¾Æ ¿ÀÀÚ! 
#endif

        KP2P::GetInstance()->Send_MyDonationInfoReq();
        KP2P::GetInstance()->Send_RecommendFullInfoReq();

#ifdef  USE_QUIZ_BINGO
        //Result_BingoBoardGame = INT_MAX;
        KP2P::GetInstance()->Send_GetUserBingoDataReq();       
        //g_MyD3D->WaitForServerAck( Result_BingoBoardGame, INT_MAX, 5000, TIME_OUT_VALUE );
#endif

        KP2P::GetInstance()->Send_PetCostumeListReq();
        SiKGCInviteManager()->SetDenyInviteMsg( g_MyD3D->m_KGCOption.IsDenyInviteMsg(), true);

        g_kGlobalValue.m_bSendServerInfoNot = true;
    }

    int iCharType = g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType;

    //  ÀÎº¥ »õ·Î ¹Þ¾Æ¾ß ÇÔ.( ´ë±â ÇÑ´Ù. ) 
    if ( iCharType != g_kGlobalValue.GetSavedCharType() )
    {
        //
        // Send ÀÎº¥Åä¸® ¿äÃ» 
        //
        SiKGCGiftBoxManager()->Reset();
        KP2P::GetInstance()->Send_ChangeCharReq( g_kGlobalValue.GetSavedCharType(), iCharType );

        // ½Å±Ô ÀÎº¥Åä¸® ¿Àµµ·Ï ±â´Ù¸°´Ù.
        Result_ChangeChar = INT_MAX;
        g_MyD3D->WaitForServerAck( Result_ChangeChar, INT_MAX, 30000, TIME_OUT_VALUE );

        if ( Result_ChangeChar != 0 )
        {
            // ÀÎº¥ Åä¸® Á¤º¸ ¸ø ºÒ·¯¿ÔÀ»¶§....
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_GET_INVEN_ERROR ), 
                L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            g_MyD3D->OnServerSceneBtn();
            return;
        }
    }

#if defined(NATION_CHINA)
    KP2P::GetInstance()->Send_CYOU_UserLogin_Req( g_kGlobalValue.GetSavedCharType(), iCharType );
#endif

    //g_kGlobalValue.SetSavedCharType( iCharType );
    //g_kGlobalValue.SendSavedCharType();

    SiKGCPrivateDungeonManager()->MoveStateCondition( EM_CREATE_ROOM_TYPE );

#ifdef AUTO_CITY
    g_MyD3D->m_pStateMachine->GoWorldMap();

    // Kawan>
    //if (SiGCSquare()->GetSquareType() == KSquareInfo::ST_GUILD)
    //    SiGCSquare()->SetGuildSquareIndex(m_iSelectIndex);

    if (g_MyD3D->m_pStateMachine->IsJoinedChannel())
    {
        SiGCSquare()->SetJoinedChannel(true);
        SiGCSquare()->SetChannelID(SiKGCChannelManager()->GetCurrentChannel());
        SiGCSquare()->SetJoinedGuild(false);
    }
    // else if (g_MyD3D->m_pStateMachine->IsJoinedGuild())
    // {
    //     SiGCSquare()->SetJoinedChannel(false);
    //      SiGCSquare()->SetJoinedGuild(true);
    //  }

    SiGCSquare()->SetPrevState(g_MyD3D->m_pStateMachine->GetState());

    std::vector< KSquareInfo > vecSquareInfo = SiGCSquare()->GetSquareInfo();
    if (vecSquareInfo.size() > 0) {
        KSquareInfo     m_kSquareInfo = vecSquareInfo[0]; // default = 0
        SiGCSquare()->SetSquareID(m_kSquareInfo.m_dwID);
        g_MyD3D->m_pStateMachine->GoSquareLoading();
    }
#else
    g_MyD3D->GoChannel();
#endif
}

void KGCCharInfoFrame_New::GoRoom()
{
    int iCharType = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType;

    // Ä³¸¯ÅÍ Àü¿ë ´øÀüÀÌ ÀÖ´Â Ä³¸¯ÅÍ ÀÏ°æ¿ì Á¹¾÷ ¿©ºÎ Ã¼Å©ÇÑ´Ù.
    if ( SiKGCPrivateDungeonManager()->IsHavePrivateDungeon( iCharType ) )
    {
        if ( g_MyD3D->m_TempPlayer.GetCurrentCharLevel() < 18 )
        {   
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NO_CLEAR_PRIVATE_DUNGEON ));
            return;
        }
    }

    if( iCharType != g_kGlobalValue.GetSavedCharType() )
    {
        SiKGCGiftBoxManager()->Reset();
        KP2P::GetInstance()->Send_InRoomChangeCharReq( g_kGlobalValue.GetSavedCharType(), iCharType );

        // ½Å±Ô ÀÎº¥Åä¸® ¿Àµµ·Ï ±â´Ù¸°´Ù.
        Result_ChangeChar = INT_MAX;
        g_MyD3D->WaitForServerAck( Result_ChangeChar, INT_MAX, 30000, TIME_OUT_VALUE );

        if ( Result_ChangeChar != 0 )
        {
            // ÀÎº¥ Åä¸® Á¤º¸ ¸ø ºÒ·¯¿ÔÀ»¶§....
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_GET_INVEN_ERROR ), 
                L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            g_MyD3D->OnServerSceneBtn();
            return;
        }
    }

    if( g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT_FROM_ROOM )
    {
        KP2P::GetInstance()->Send_InRoomChangeIndoorUserInfoReq( &g_kGlobalValue.m_kUserInfo );
        Result_ReceiveInRoomIndoorChangeReq = INT_MAX;
        g_MyD3D->WaitForServerAck( Result_ReceiveInRoomIndoorChangeReq, INT_MAX, 5000, TIME_OUT_VALUE );

        if( Result_ReceiveInRoomIndoorChangeReq != 0 )
        {
            g_MyD3D->m_pStateMachine->GoMatch();
            return;
        }

        g_MyD3D->m_pStateMachine->GoRoom();
        g_pkUIHelper->SetUILockTime(LOCK_ROOM, 0);
        KP2P::GetInstance()->Send_Into_Myinfo( g_kGlobalValue.m_kUserInfo.dwUID, g_kGlobalValue.m_kUserInfo.strLogin, GC_RUS_WAIT );
    }
}


void KGCCharInfoFrame_New::OnInitialize()
{
    m_pkBtn_Confirm->Lock( g_kGlobalValue.m_kUserInfo.vecCharInfo.empty() );
    SetChildOrderTop( m_pkBackGroundWhite );
    SetChildOrderTop( m_pkNickNameFrame );
    m_pkBtn_Make_char->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCCharInfoFrame_New::OnAddSlot ); 
    if ( g_kGlobalValue.m_kUserInfo.strNickName == L"" )
    {
        m_pkNickNameFrame->ToggleRender( true );
        m_pkBackGroundWhite->ToggleRender( true );
        //LockEmptySlot( true );
    }
    if( SiKGCVirtualDepotManager()->IsEmptyItemList() )
    {
        m_pkVdepot->ToggleRender( false );
        m_pkVdepot->SetWndMode( D3DWM_LOCK );
    }
}


void KGCCharInfoFrame_New::OnAddSlot()
{
    int nOpenChar = g_kGlobalValue.GetCharOpenType().size();
    int nHaveChar = g_kGlobalValue.m_kUserInfo.vecCharInfo.size();
    int nSlotCount = g_kGlobalValue.m_kUserInfo.m_nSlotCount;

    if (nSlotCount > 0) {
        g_MyD3D->m_pStateMachine->GoCharGain();
        return;
    }

    if ( g_kGlobalValue.m_kUserInfo.vecCharInfo.empty() ) 
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ADD_SLOT_CHECK_ERROR1 ), 
            L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }

    if ( nOpenChar <= nHaveChar + nSlotCount )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ADD_SLOT_CHECK_ERROR2 ), 
            L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }

    GCITEMID itemID = KGCItemManager::ITEM_ADD_SLOT_ITEM;

    //  ÀÎº¥¿¡ ÀÖ´ÂÁö µÚÁø´Ù.
    if ( g_pItemMgr->GetInventoryItemDuration( itemID ) > 0 ) 
    {
        KP2P::GetInstance()->Send_AddSlotReq( itemID * 10 );    
    }
    //  ÀÏ´Ü »ç°í...
    else
    {       
        BuyAddSlotItem( itemID );
    }
}



void KGCCharInfoFrame_New::BuyAddSlotItem( GCITEMID itemID )
{
    if ( itemID == 0 ) return;

    GCItem* pItem = g_pItemMgr->GetItemData( itemID );
    if ( pItem == NULL ) return;

    if( pItem->eMoneyType == EMT_CASH )
    {
#if defined ( USE_VIP_POINT )
        g_pItemMgr->m_bIsInPresentState = false;
        g_pItemMgr->BuyCashItemS6( itemID );
#else
#ifdef GAME_CASH
        g_pkUIScene->EnableBuyCashItemWnd( itemID, false, false, true );
#else
        g_MyD3D->m_kItemMgr.BuyItem( itemID, pItem->iItemTypeValue );
#endif
#endif
    }
    else
    {
        g_MyD3D->m_kItemMgr.BuyItem( itemID, pItem->iItemTypeValue );
    }
}


void KGCCharInfoFrame_New::CreateNewBar(KCharInfoVector::iterator cit, std::vector<KD3DWnd*>& barList, KD3DWnd*& title)
{
    // Its MP user, adds it
    KD3DWnd* curUser = g_pkUIMgr->CreateTemplateUIByFindName(this, "ui_scene_char_select_new.stg", "", "char_select_scene_new\\char_info_frame_new2\\char0");
    KD3DStatic* curLv = (KD3DStatic*)curUser->GetChildByName("char_lv");
    KD3DStatic* curName = (KD3DStatic*)curUser->GetChildByName("char_name");

    // Make Lv and Charname text
    std::wstring Lvtxt = L"Lv. ";
    Lvtxt.append(std::to_wstring(cit->iLevel));
    curLv->SetText(Lvtxt);
    curName->SetText( g_kGlobalValue.GetCharName( cit->iCharType ));

    D3DXVECTOR2 vc;
    if (barList.size() % 2 == 0) {
        vc = title->GetFixedWindowLocalPos();
        vc.y += 30;
        vc.x -= 10;
    }
    else
        vc = barList.back()->GetFixedWindowLocalPos();

    if(barList.size() > 0 && barList.size() % 2 == 0)
    {
        // ?tima barrinha ?a segunda, ent? aumentar Y
        vc.y += (40 * (barList.size() / 2));
    }

    if (barList.size() > 0 && barList.size() % 2 > 0)
    {
        vc.x += 190;
    }
    
    curUser->InitState(true, true, this);
    curUser->SetFixedWindowLocalPos(vc);
    //curUser->ToggleRender(true);

    //Add click event
    curUser->DisconnectAllEventProcedure();
    curUser->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCCharInfoFrame_New::OnClickChangeCharacter ); 
    curUser->ConnectEventProcedure( KD3DWnd::D3DWE_DOUBLE_CLICK, this, &KGCCharInfoFrame_New::OnDblClickChangeCharacter ); 

    barList.push_back(curUser);
}

bool KGCCharInfoFrame_New::OnDblClickChangeCharacter(const KActionEvent& event) 
{
    OnClickChangeCharacter(event);
    OnClickChannelBtn();
    return true;
}
void KGCCharInfoFrame_New::OnClickChannelBtn()
{
    //here has a code for return guide message box, I should see if we'll put this trash in our main
    if( g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT_FROM_ROOM )
    {
        this->GoRoom();
    }
    else
    {
        this->GoChannel();
    }

    this->m_pkBackGroundWhite->ToggleRender( false );
}


bool KGCCharInfoFrame_New::OnClickChangeCharacter(const KActionEvent& event)
{
    KCharInfoVector::iterator vIter =	g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.begin();
    for(int k= 0;vIter !=g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.end() ;vIter++)
    {
        if(g_kGlobalValue.GetCharName( vIter->iCharType ) == ((KD3DStatic*)event.m_pWnd->GetChildByName("char_name"))->GetText())
        {
           // g_MyD3D->m_TempPlayer.m_kUserInfo.GetCharIndex(vIter->iCharType)
            g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex = static_cast<char>(k);
            g_kGlobalValue.m_kUserInfo.cCharIndex = static_cast<char>(k);
            break;
        }
        k++;
    }

    // dragonares: Plataforma
    ChangeCharacter();

    return true;
}

void KGCCharInfoFrame_New::InitData()
{
    m_TitleMP->ToggleRender(false);
    m_TitleAP->ToggleRender(false);
    m_TitleHybrid->ToggleRender(false);

    m_iTotalSlot = &g_kGlobalValue.m_kUserInfo.m_nSlotCount;

    KCharInfoVector::iterator cit = g_kGlobalValue.m_kUserInfo.vecCharInfo.begin();
    
    std::set<int> mpUsers{ 0,1,2,3,4,5,6,7,8,9,19, 21 };
//    std::set<int> mpUsers{ 0,1,2,3,4,5,6,7,8,9,19 };
    std::set<int> apUsers{ 10,11,12,13,18, 20 };
    std::set<int> hybUsers{ 14,15,16,17,22 }; // Cadela adicionada

    // Print MP section first
    for (; cit != g_kGlobalValue.m_kUserInfo.vecCharInfo.end(); cit++)
    {
        if (mpUsers.find(cit->iCharType) == mpUsers.end())
            continue;
            
        //First time finding the char? create the title
        if(m_vecMPSlotBar.size() == 0)
            m_TitleMP->ToggleRender(true);

        CreateNewBar(cit, m_vecMPSlotBar, m_TitleMP);
    }


    // AP SECTION
    cit = g_kGlobalValue.m_kUserInfo.vecCharInfo.begin();

    for (; cit != g_kGlobalValue.m_kUserInfo.vecCharInfo.end(); cit++)
    {
        if (apUsers.find(cit->iCharType) == apUsers.end())
            continue;

        //First time finding the char? create the title
        if (m_vecAPSlotBar.size() == 0)
        {
            m_TitleAP->ToggleRender(true);

            // Change title position if MP slot was printed
            D3DXVECTOR2 tmpvc = m_TitleAP->GetFixedWindowLocalPos();

            if (m_vecMPSlotBar.size() > 0)
            {
                tmpvc.y += 85;
                tmpvc.y += 40 * (m_vecMPSlotBar.size() / 2);
            }

            if (m_vecHybridSlotBar.size() > 0)
            {
                tmpvc.y += 85;
                tmpvc.y += 40 * (m_vecHybridSlotBar.size() / 2);
            }

            if (tmpvc.y != m_TitleAP->GetFixedWindowLocalPos().y)
            {
                m_TitleAP->SetFixedWindowLocalPos(tmpvc);
            }
        }

        CreateNewBar(cit, m_vecAPSlotBar, m_TitleAP);
    }



    // HYBRID SECTION
    cit = g_kGlobalValue.m_kUserInfo.vecCharInfo.begin();

    for (; cit != g_kGlobalValue.m_kUserInfo.vecCharInfo.end(); cit++)
    {
        if (hybUsers.find(cit->iCharType) == hybUsers.end())
            continue;

        //First time finding the char? create the title
        if (m_vecHybridSlotBar.size() == 0)
        {
            m_TitleHybrid->ToggleRender(true);

            // Change title position if MP slot was printed
            D3DXVECTOR2 tmpvc = m_TitleHybrid->GetFixedWindowLocalPos();

            if (m_vecMPSlotBar.size() > 0)
            {
                tmpvc.y += 85;
                tmpvc.y += 40 * (m_vecMPSlotBar.size() / 2);
            }

            if (m_vecAPSlotBar.size() > 0)
            {
                if (m_vecMPSlotBar.size() > 0)
                {
                    tmpvc.y -= 40;
                }

                tmpvc.y += 85;
                tmpvc.y += 40 * (m_vecAPSlotBar.size() / 2);
            }

            if (tmpvc.y != m_TitleHybrid->GetFixedWindowLocalPos().y)
            {
                m_TitleHybrid->SetFixedWindowLocalPos(tmpvc);
            }
        }

        CreateNewBar(cit, m_vecHybridSlotBar, m_TitleHybrid);
    }

    if (!g_kGlobalValue.m_kUserInfo.vecCharInfo.empty() && g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType != -1)
    {
        ChangeCharacter();
    }
}

void KGCCharInfoFrame_New::ChangeCharacter()
{
    // dragonares: Plataforma
    KGCGlobal::ShortMenuCharInfo m_kCharInfo;

    m_kCharInfo = g_kGlobalValue.GetShortMenuInfoForCharType(g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType);
    m_pkCharWnd->m_pMeshKOGRoom = new GCObject();
    m_pkCharWnd->m_pMeshKOGRoom->AddMesh(g_pGCDeviceManager2->CreateMeshP3M(m_kCharInfo.m_strFootholdModel), g_pGCDeviceManager2->CreateTexture(m_kCharInfo.m_strFootholdTexture));

    m_pkCharWnd->UpdateMeshObject();
    m_pkCharWnd->SetChar( g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType );
}

void KGCCharInfoFrame_New::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnMoveChannel, KD3DWnd::D3DWE_BUTTON_CLICK, OnMoveChannel );
    GCWND_MSG_MAP( m_pkBtn_Confirm, KD3DWnd::D3DWE_BUTTON_CLICK,     OnClickChannelBtn );

    if( m_pkVdepot == event.m_pWnd )
		{
            CreateVDepot();
        }
    //for (int i = 0; i < GC_CHAR_NUM; i++)
    //{
    //    if (m_pkCharIcon[i] == event.m_pWnd)
    //    {
    //        //if (m_pkCharIcon[i]->IsLockChar())
    //        //	continue;
    //        if (event.m_dwCode == D3DWE_BUTTON_CLICK || event.m_dwCode == D3DWE_DOUBLE_CLICK)
    //        {

    //            KCharInfoVector::iterator vIter = g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.begin();
    //            for (int k = 0; vIter != g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.end(); vIter++)
    //            {
    //                if (vIter->iCharType == i)
    //                {
    //                    g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex = static_cast<char>(k);
    //                    g_kGlobalValue.m_kUserInfo.cCharIndex = static_cast<char>(k);
    //                    break;
    //                }
    //                k++;
    //            }
    //            m_pkFocusingIcon = m_pkCharIcon[i];

    //            // 				g_kGlobalValue.m_kUserInfo.SetCurrentChar(i);
    //            // 				g_MyD3D->m_TempPlayer.m_kUserInfo.SetCurrentChar( i );			
    //            SetSelectedChar();
    //        }
    //        if (event.m_dwCode == D3DWE_DOUBLE_CLICK)
    //        {
    //            OnClickChannelBtn();
    //        }
    //    }
    //}
}

void KGCCharInfoFrame_New::DisposeInfos() 
{
    std::vector<KD3DWnd*>::iterator& a = m_vecMPSlotBar.begin();
    for (; a != m_vecMPSlotBar.end(); a++)
    {
        (*a)->Destroy();
    }

    std::vector<KD3DWnd*>::iterator& b = m_vecAPSlotBar.begin();

    for (; b != m_vecAPSlotBar.end(); b++)
    {
        (*b)->Destroy();

    }

    std::vector<KD3DWnd*>::iterator& c = m_vecHybridSlotBar.begin();

    for (; c != m_vecHybridSlotBar.end(); c++)
    {
        (*c)->Destroy();

    }
    
    m_vecMPSlotBar.clear();
    m_vecAPSlotBar.clear();
    m_vecHybridSlotBar.clear();

    m_TitleAP->SetFixedWindowLocalPos(m_TitleMP->GetFixedWindowLocalPos());
    m_TitleHybrid->SetFixedWindowLocalPos(m_TitleMP->GetFixedWindowLocalPos());
}

void KGCCharInfoFrame_New::OnDestroyComplete(void)
{
}
void KGCCharInfoFrame_New::FrameMoveInEnabledState(void)
{
    m_pkCharWnd->SetZoom(0.14f);
    m_pkCharWnd->FreezeRoomRotation(true);
    if( SiKGCVirtualDepotManager()->GetRecvPacket() )
    {
        if( !g_kGlobalValue.m_kUserInfo.vecCharInfo.empty() )
        {
            if( SiKGCVirtualDepotManager()->GetFirstLogOn() )
            {
                CreateVDepot();
                SiKGCVirtualDepotManager()->SetFirstLogOn( false );
            }

            if( SiKGCVirtualDepotManager()->IsEmptyItemList() )
            {
                m_pkVdepot->ToggleRender( false );
                m_pkVdepot->SetWndMode( D3DWM_LOCK );
            }
            else
            {
                m_pkVdepot->ToggleRender( true );
                m_pkVdepot->SetWndMode( D3DWM_DEFAULT );
            }
            SiKGCVirtualDepotManager()->SetRecvPacket( false );
        }
        else
            SiKGCVirtualDepotManager()->SetFirstLogOn( false );

    }	
}
void KGCCharInfoFrame_New::PostChildDraw() 
{
}

void KGCCharInfoFrame_New::CreateEmptySlot()
{
    /*for ( int i = 0; i < (int)m_vecEmptySlot.size(); ++i ) 
    {
        m_vecEmptySlot[i]->ToggleRender( false );
    }

    int nSlotCount = g_kGlobalValue.m_kUserInfo.m_nSlotCount;

    if ( (int)m_vecEmptySlot.size() < nSlotCount )
    {
        int nCount = nSlotCount - m_vecEmptySlot.size();

        for ( int i = 0; i < nCount; ++i ) 
        {
            KD3DWnd* pWnd = NULL;

            pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_scene_char_select.stg", "window", "char_select_scene_new\\char_info_frame\\charempty" );
            if (pWnd == NULL)
                return;

            pWnd->InitState( false, true, this );

            m_vecEmptySlot.push_back( pWnd );

        }    
    }*/
}

void KGCCharInfoFrame_New::UpdateEmptySlot()
{
   /* if( g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT_FROM_ROOM )
    {
        for( int i = 0; i < (int)m_vecEmptySlot.size(); i++ )
        {
            m_vecEmptySlot[i]->ToggleRender( false );
        }
        return;
    }

    DWORD dwIconStandardWidth = m_pkCharIcon[0]->GetWidth();
    DWORD dwIconStandardHeight = m_pkCharIcon[0]->GetHeight();
    DWORD dwIconDrawRecWidth = m_pkCharIconRect->GetWidth();
    D3DXVECTOR2 vStartPos = m_pkCharIconRect->GetFixedWindowLocalPos();

    int nColumn =(dwIconDrawRecWidth+ICON_GAP) / (dwIconStandardWidth+ICON_GAP);
    int nHaveChar = g_kGlobalValue.m_kUserInfo.vecCharInfo.size();
    int cntColumn = nHaveChar % nColumn;
    int cntRow = nHaveChar / nColumn;



    CreateEmptySlot();
    int nSlotCount = g_kGlobalValue.m_kUserInfo.m_nSlotCount;

    for ( int i = 0; i < nSlotCount; ++i ) 
    {
        cntColumn = (i+nHaveChar) % nColumn;
        cntRow = (i+nHaveChar) / nColumn;

        float fPosX = vStartPos.x + (cntColumn * (dwIconStandardWidth + ICON_GAP));
        float fPosY = vStartPos.y + (cntRow * (dwIconStandardHeight + ICON_GAP));

        m_vecEmptySlot[i]->SetWindowPosDirect(D3DXVECTOR2(fPosX,fPosY));
        m_vecEmptySlot[i]->ToggleRender( true );
    }

    if( nSlotCount > 0 )
    {
        m_pkSloatAddIcon->InitState( false, false, NULL );
    }*/

}


void KGCCharInfoFrame_New::LockEmptySlot( bool bRender_ )
{
    if ( true == m_vecEmptySlot.empty() )
        return;

    std::vector<KD3DWnd*>::iterator vit = m_vecEmptySlot.begin();
    for ( ; vit!=m_vecEmptySlot.end() ; ++vit )
        (*vit)->Lock( bRender_ );

}

void KGCCharInfoFrame_New::CreateVDepot()
{
    if( SiKGCVirtualDepotManager()->IsEmptyItemList() )
        return;

    SiKGCVirtualDepotManager()->SetItemCategory();

    g_KDSound.Play( "31" );
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_VIRTUALDEPOT );
}