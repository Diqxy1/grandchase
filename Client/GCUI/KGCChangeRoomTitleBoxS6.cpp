#include "stdafx.h"
#include "GCRoomCreateDlgS6.h"
#include "GCRoomListBtnSetS6.h"
#include "GCDungeonTitleColorS6.h"
#include "GCTextColorManager.h"
#include "gcui/KGCChangeRoomTitleBoxS6.h"

IMPLEMENT_CLASSNAME( KGCChangeRoomTitleBoxS6 );
IMPLEMENT_WND_FACTORY( KGCChangeRoomTitleBoxS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCChangeRoomTitleBoxS6, "gc_change_room_title_box_s6" );

KGCChangeRoomTitleBoxS6::KGCChangeRoomTitleBoxS6( void )
: m_pkCreateRoom(NULL)
, m_pkRoomName(NULL)
, m_pkRoomPassword(NULL)
, m_pkClose(NULL)
, m_pkColorBtnSet(NULL)
, m_pkPaintTitle(NULL)
, m_pkBackgroundExtended(NULL)
, m_pkOptionCloseBtn(NULL)
, m_pkOptionOpenBtn(NULL)
, m_pkUsePetCheck(NULL)
, m_pkUse4SPCheck(NULL)
, m_pkUseBallanceCheck(NULL)
, m_pkRankSetBtn(NULL)
, m_pkRankModifyBtn(NULL)
, m_pkCharSetBtn(NULL)
, m_pkCharModifyBtn(NULL)
, m_pkRankPanel(NULL)
{
    LINK_CONTROL("background_extend", m_pkBackgroundExtended);
    LINK_CONTROL("paint_btn_set" ,m_pkColorBtnSet);
    LINK_CONTROL("create_room_btn",m_pkCreateRoom);
    LINK_CONTROL("static_room_name",m_pkRoomName);
    LINK_CONTROL("static_room_pass",m_pkRoomPassword);
    LINK_CONTROL("end_btn", m_pkClose);
    LINK_CONTROL("paint_title", m_pkPaintTitle);

    LINK_CONTROL("close_option_btn", m_pkOptionCloseBtn);
    LINK_CONTROL("open_option_btn", m_pkOptionOpenBtn);
    LINK_CONTROL("check_use_pet", m_pkUsePetCheck);
    LINK_CONTROL("check_use_4sp", m_pkUse4SPCheck);
    LINK_CONTROL("check_use_ballance", m_pkUseBallanceCheck);
    LINK_CONTROL("rank_set_btn", m_pkRankSetBtn);
    LINK_CONTROL("rank_modify_btn", m_pkRankModifyBtn);
    LINK_CONTROL("char_set_btn", m_pkCharSetBtn);
    LINK_CONTROL("char_modify_btn", m_pkCharModifyBtn);

    for (int i = 0; i < MAX_ROOM_OPTION; i++)
    {
        m_saExtraOptions[i] = NULL;
        LINK_CONTROL_STM("static_extra_option" << i, m_saExtraOptions[i]);
    }

    LINK_CONTROL("rank_option_panel", m_pkRankPanel);
    LINK_CONTROL("char_option_panel", m_pkCharPanel);
}

KGCChangeRoomTitleBoxS6::~KGCChangeRoomTitleBoxS6( void )
{
}

void KGCChangeRoomTitleBoxS6::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP(m_pkUsePetCheck, KD3DWnd::D3DWE_BUTTON_CLICK, m_pkUsePetCheck->ToggleCheck);
    GCWND_MSG_MAP(m_pkUse4SPCheck, KD3DWnd::D3DWE_BUTTON_CLICK, m_pkUse4SPCheck->ToggleCheck);
    GCWND_MSG_MAP(m_pkUseBallanceCheck, KD3DWnd::D3DWE_BUTTON_CLICK, m_pkUseBallanceCheck->ToggleCheck);
}

void KGCChangeRoomTitleBoxS6::OnCreate( void )
{
    g_pkUIMgr->DisconnectAllEventProcedure(GetUID());

    m_pkColorBtnSet->InitState(true,true,this);

    m_pkCreateRoom->InitState(true,true,this);
    m_pkCreateRoom->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCChangeRoomTitleBoxS6::OnChangeRoomInfo);
    m_pkCreateRoom->SetHotKey(DIK_RETURN);

    m_pkRoomName->InitState(true,true,this);
    m_pkRoomName->SetLimitText(MAX_EDIT_LEN,m_pkRoomName->GetWidth(),12);
    m_pkRoomName->SetText(L"");
    m_pkRoomName->SetFontColor( 0xffffffff );
    SiKGCRoomListManager()->SetCreateRoomTitleColor( -1 );

    m_pkRoomPassword->SetText( g_pStateRoom->GetRoomPassword().c_str() );
    m_pkRoomPassword->SetLimitTextByByte(16);

    m_pkClose->InitState(true,true,this);
    m_pkClose->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCChangeRoomTitleBoxS6::OnClickBtnClose);
    m_pkClose->SetHotKey(DIK_ESCAPE);

    g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_DUNGEON_CLICK_TITLE_COLOR_BTN);
    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_DUNGEON_CLICK_TITLE_COLOR_BTN, this, &KGCChangeRoomTitleBoxS6::OnClickColorBtn );


    m_pkPaintTitle->InitState(true);

    CheckPaint();

    if (m_pkBackgroundExtended)
    {
        m_pkBackgroundExtended->InitState(false, true, this);
    }

    if (m_pkOptionCloseBtn)
    {
        m_pkOptionCloseBtn->InitState(false, true, this);
        m_pkOptionCloseBtn->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCChangeRoomTitleBoxS6::OnClickCloseOptionsBtn);
    }

    if (m_pkOptionOpenBtn)
    {
        m_pkOptionOpenBtn->InitState(true, true, this);
        if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON)
        {
            m_pkOptionOpenBtn->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCChangeRoomTitleBoxS6::OnClickOpenOptionsBtn);
        }
    }

    if (m_pkUsePetCheck)
    {
        m_pkUsePetCheck->InitState(false, true, this);
        m_pkUsePetCheck->SetCheck(false);
    }

    if (m_pkUse4SPCheck)
    {
        m_pkUse4SPCheck->InitState(false, true, this);
        m_pkUse4SPCheck->SetCheck(false);
    }

    if (m_pkUseBallanceCheck)
    {
        m_pkUseBallanceCheck->InitState(false, true, this);
        m_pkUseBallanceCheck->SetCheck(false);
    }

    if (m_pkRankSetBtn)
    {
        m_pkRankSetBtn->InitState(false, true, this);
        m_pkRankSetBtn->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCChangeRoomTitleBoxS6::OnClickRankPanelSet);
    }

    if (m_pkRankModifyBtn)
    {
        m_pkRankModifyBtn->InitState(false, true, this);
        m_pkRankModifyBtn->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCChangeRoomTitleBoxS6::OnClickRankPanelSet);
    }

    if (m_pkCharSetBtn)
    {
        m_pkCharSetBtn->InitState(false, true, this);
        m_pkCharSetBtn->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCChangeRoomTitleBoxS6::OnClickCharPanelSet);
    }

    if (m_pkCharModifyBtn)
    {
        m_pkCharModifyBtn->InitState(false, true, this);
        m_pkCharModifyBtn->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCChangeRoomTitleBoxS6::OnClickCharPanelSet);
    }

    for (int i = 0; i < MAX_ROOM_OPTION; i++)
    {
        if (m_saExtraOptions[i])
        {
            m_saExtraOptions[i]->InitState(false, true, this);
            m_saExtraOptions[i]->SetText(g_pkStrLoader->GetString(STR_ID_ROOM_EXTRA_OPTION_BLOCKED_PET + i));
        }
    }

    if (m_pkRankPanel)
    {
        m_pkRankPanel->IsCreatingRoom(true);
        m_pkRankPanel->InitState(false, true, this);
    }

    if (m_pkCharPanel)
    {
        m_pkCharPanel->IsCreatingRoom(true);
        m_pkCharPanel->InitState(false, true, this);
    }

    m_pkCreateRoom->SetWindowPosYDirect(241.0f);
}

void KGCChangeRoomTitleBoxS6::OnClickOpenOptionsBtn()
{
    EnableOptionInfo(true);
    m_pkCreateRoom->SetWindowPosYDirect(373.0f);

}

void KGCChangeRoomTitleBoxS6::OnClickCloseOptionsBtn()
{
    EnableOptionInfo(false);
    m_pkCreateRoom->SetWindowPosYDirect(241.0f);
}

void KGCChangeRoomTitleBoxS6::EnableOptionInfo(bool bEnable)
{
    m_pkBackgroundExtended->ToggleRender(bEnable);
    m_pkOptionOpenBtn->ToggleRender(!bEnable);
    m_pkOptionCloseBtn->ToggleRender(bEnable);
    m_pkUsePetCheck->ToggleRender(bEnable);
    m_pkUse4SPCheck->ToggleRender(bEnable);
    m_pkUseBallanceCheck->ToggleRender(bEnable);

    KRoomOptions options = SiKGCRoomListManager()->GetCreateRoomOptions();

    if (options.m_dwAllowedRankings != 0xFFFFFFFF)
    {
        m_pkRankModifyBtn->ToggleRender(bEnable);
    }
    else
    {
        m_pkRankSetBtn->ToggleRender(bEnable);
    }

    if (options.m_dwDisabledChars != 0)
    {
        m_pkCharModifyBtn->ToggleRender(bEnable);
    }
    else
    {
        m_pkCharSetBtn->ToggleRender(bEnable);
    }

    if ((options.m_dwEnabledOptions & KRoomOptions::BLOCK_PET_OPTION) == KRoomOptions::BLOCK_PET_OPTION)
    {
        m_pkUsePetCheck->SetCheck(true);
    }

    if ((options.m_dwEnabledOptions & KRoomOptions::BLOCK_SUPREME_SKILL) == KRoomOptions::BLOCK_SUPREME_SKILL)
    {
        m_pkUse4SPCheck->SetCheck(true);
    }

    if ((options.m_dwEnabledOptions & KRoomOptions::ATTRIBUTE_CORRECTION) == KRoomOptions::ATTRIBUTE_CORRECTION)
    {
        m_pkUseBallanceCheck->SetCheck(true);
    }

    for (int i = 0; i < MAX_ROOM_OPTION; i++)
    {
        m_saExtraOptions[i]->ToggleRender(bEnable);
    }
}

void KGCChangeRoomTitleBoxS6::OnClickRankPanelSet()
{
    if (m_pkCharPanel->IsRenderOn())
    {
        m_pkCharPanel->ToggleRender(false);
    }

    m_pkRankPanel->ToggleRender(!m_pkRankPanel->IsRenderOn());
}

void KGCChangeRoomTitleBoxS6::OnClickCharPanelSet()
{
    if (m_pkRankPanel->IsRenderOn())
    {
        m_pkRankPanel->ToggleRender(false);
    }

    m_pkCharPanel->ToggleRender(!m_pkCharPanel->IsRenderOn());
}

void KGCChangeRoomTitleBoxS6::OnClickColorBtn( )
{
    m_pkRoomName->SetFontColor(SiKGCRoomListManager()->GetCreateRoomTitleColor());
}


void KGCChangeRoomTitleBoxS6::OnClickBtnClose( )
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCChangeRoomTitleBoxS6::OnChangeRoomInfo( void )
{
    WCHAR szRoomName[MAX_EDIT_LEN + 1] = { 0, };
    StringCchCopyW( szRoomName, MAX_EDIT_LEN + 1, m_pkRoomName->GetText() );
    SiKSlangFilter()->Filter( szRoomName, KSlangFilter::SLANG_FILTER_CHAT );
    //g_kCurseFilter.ConvertString( szRoomName, MAX_EDIT_LEN + 1 );
    std::wstring RoomName = szRoomName;

    g_pkStrLoader->Replacer(RoomName, L"#c", L"");
    g_pkStrLoader->Replacer(RoomName, L"#C", L"");

    m_strRoomName = RoomName;
    m_strRoomPasswd = m_pkRoomPassword->GetText();
    

    if(SiKGCRoomListManager()->GetCreateRoomTitleColorIndex() > 0 ) {
        m_strRoomName = L"#c" + SiGCTextColorManager()->GetColorString(SiKGCRoomListManager()->GetCreateRoomTitleColorIndex()) + m_strRoomName + L"#cX";
    }

    KRoomOption opt;
    opt.m_strPassWord = m_strRoomPasswd;
    opt.m_strRoomTitle = m_strRoomName;
    opt.m_bDeathMatchBalancing =  SiKGCRoomManager()->GetRoomInfo().bDeathMatchBalancing;
    opt.m_bDeathMatchIntrudable = true;
    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ||
         SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH )
    {
        KGCDeathMatch* pGameMode = (KGCDeathMatch*)g_pStateGame->GetCurGameModePtr();
        opt.m_nDeathKillCount = pGameMode->m_iGoal;
        opt.m_nDeathLimitTime = (int)pGameMode->m_fTime;
    }
    else
    {
        opt.m_nDeathKillCount = 20;
        opt.m_nDeathLimitTime = 600;
    }
    KP2P::GetInstance()->Send_Change_Room_Option_Req( opt );

    if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON)
    {
        KRoomOptions options = SiKGCRoomListManager()->GetCreateRoomOptions();
        options.m_dwEnabledOptions = 0;
        if (m_pkUsePetCheck->GetCheck())
        {
            options.m_dwEnabledOptions |= KRoomOptions::BLOCK_PET_OPTION;
        }

        if (m_pkUse4SPCheck->GetCheck())
        {
            options.m_dwEnabledOptions |= KRoomOptions::BLOCK_SUPREME_SKILL;
        }

        if (m_pkUseBallanceCheck->GetCheck())
        {
            options.m_dwEnabledOptions |= KRoomOptions::ATTRIBUTE_CORRECTION;
        }

        SiKGCRoomListManager()->SetCreateRoomOptions(options);
        KP2P::GetInstance()->Send_PvpRoomExtraOptionReq(options);
        SiKGCRoomListManager()->SaveRoomOptionsFile();
    }

    // 방제 색변화 아이템 수량 깍아주고
    if ( g_pItemMgr->IsPaintItem() && !g_pItemMgr->IsFreePaintItem() ) {    // 페인트 아이템 중 GC 클럽, GC 블로그, 운영자 계정이면 수량을 깎지 않는다
        KItem* Info = NULL;
        if ( SiKGCRoomListManager()->GetCreateRoomTitleColorIndex() != -1 ) {
            if( SiGCTextColorManager()->IsCashColor( SiKGCRoomListManager()->GetCreateRoomTitleColorIndex() ) ) {
                if ( g_pItemMgr->FindInventoryForItemID( KGCItemManager::ITEM_ROOM_NAME_COLORING_CASH, &Info ) ) {
                    g_pItemMgr->DecDurationItem( Info->m_ItemUID );
                }
            }
            else {
                if ( g_pItemMgr->FindInventoryForItemID( KGCItemManager::ITEM_ROOM_NAME_COLORING_GP, &Info ) ) {
                    g_pItemMgr->DecDurationItem( Info->m_ItemUID );
                }
                else if ( g_pItemMgr->FindInventoryForItemID( KGCItemManager::ITEM_ROOM_NAME_COLORING_CASH, &Info ) ) { // GP템이 없으면 캐쉬템 깎아야함
                    g_pItemMgr->DecDurationItem( Info->m_ItemUID );
                }
            }
        }
    }

    // 룸리스트 및 팝업창 꺼줌
    OnClickBtnClose();
}

void KGCChangeRoomTitleBoxS6::CheckPaint()
{
    if (g_pItemMgr->IsCashPaintItem() || g_pItemMgr->IsPaintItem())
    {
        m_pkPaintTitle->ToggleRender(true);
        m_pkColorBtnSet->ToggleRender(true);
    }
    else
    {
        m_pkPaintTitle->ToggleRender(false);
        m_pkColorBtnSet->ToggleRender(false);
    }
}

const std::wstring KGCChangeRoomTitleBoxS6::GetRoomName( void ) const
{
    return m_strRoomName;
}

const std::wstring& KGCChangeRoomTitleBoxS6::GetRoomPasswd( void ) const
{
    return m_strRoomPasswd;
}