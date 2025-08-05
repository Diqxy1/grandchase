#include "stdafx.h"
#include "GCRoomCreateDlgS6.h"
#include "GCRoomListBtnSetS6.h"
#include "GCDungeonTitleColorS6.h"
#include "GCTextColorManager.h"

IMPLEMENT_CLASSNAME( KGCRoomCreateDlgS6 );
IMPLEMENT_WND_FACTORY( KGCRoomCreateDlgS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomCreateDlgS6, "gc_room_create_dlg_s6" );

KGCRoomCreateDlgS6::KGCRoomCreateDlgS6( void )
: m_pkCreateRoom(NULL)
, m_pkBossInfo(NULL)
, m_pkRoomName(NULL)
, m_pkRoomPassword(NULL)
, m_pkClose(NULL)
, m_pkColorBtnSet(NULL)
, m_pkLevelBackGround(NULL)
, m_pkLevelTitle(NULL)
, m_pkPaintTitle(NULL)
{
	char szTemp[128];
	memset(szTemp, 0, sizeof(char) * 128);

	for ( int i = KDSI_BTN_SET_0 ; i < KDSI_BTN_SET_NUM ; i++ ) {
		sprintf(szTemp, "btn_set_%d", i);
		m_saBtnSet[i] = NULL;
		LINK_CONTROL(szTemp,m_saBtnSet[i]);
	}

	LINK_CONTROL( "dungeon_boss_info_s6" ,m_pkBossInfo);
	LINK_CONTROL( "paint_btn_set" ,m_pkColorBtnSet);
	
	LINK_CONTROL("create_room_btn",m_pkCreateRoom);
	LINK_CONTROL("static_room_name",m_pkRoomName);
	LINK_CONTROL("static_room_pass",m_pkRoomPassword);
	LINK_CONTROL("end_btn", m_pkClose);

	LINK_CONTROL("level_background", m_pkLevelBackGround);
	LINK_CONTROL("level_title", m_pkLevelTitle);

	LINK_CONTROL("paint_title", m_pkPaintTitle);
}

KGCRoomCreateDlgS6::~KGCRoomCreateDlgS6( void )
{
}

void KGCRoomCreateDlgS6::ActionPerformed( const KActionEvent& event )
{

}

void KGCRoomCreateDlgS6::OnCreate( void )
{
	g_pkUIMgr->DisconnectAllEventProcedure(GetUID());

	m_pkBossInfo->InitState(true);

	for ( int i = KDSI_BTN_SET_0 ; i < KDSI_BTN_SET_NUM ; i++ ) {
		m_saBtnSet[i]->InitState(false,true,this);
	}

	UpdateState();
	
	m_pkBossInfo->SetBoss();
	m_pkBossInfo->InitState(true,false,NULL);

	m_pkColorBtnSet->InitState(true,true,this);

	m_pkCreateRoom->InitState(true,true,this);
	m_pkCreateRoom->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomCreateDlgS6::OnCreateRoom);
	m_pkCreateRoom->SetHotKey(DIK_RETURN);

	m_pkRoomName->InitState(true,true,this);
	m_pkRoomName->SetLimitText(MAX_EDIT_LEN,m_pkRoomName->GetWidth(),12);
	m_pkRoomName->SetFontColor(0xffffffff);
	m_pkRoomName->SetText( KGCRoomManager::m_vecDungeonRoomNameList[rand() % KGCRoomManager::m_vecDungeonRoomNameList.size()].c_str(), true );

	m_pkRoomPassword->SetText(L"");
	m_pkRoomPassword->SetLimitTextByByte(16);
	m_pkClose->InitState(true,true,this);
	m_pkClose->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomCreateDlgS6::OnClickBtnClose);
	m_pkClose->SetHotKey(DIK_ESCAPE);

    g_pkUIMgr->DisconnectGlobalEventProcedure(m_pkBossInfo->GetUID(),KGCUIScene::D3DWE_DUNGEON_ROOM_SELECT_LEVEL);
    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_DUNGEON_ROOM_SELECT_LEVEL,m_pkBossInfo, &KGCDungeonBossInfoS6::SetBoss);	

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_DUNGEON_CLICK_TITLE_COLOR_BTN);
	g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_DUNGEON_CLICK_TITLE_COLOR_BTN, this, &KGCRoomCreateDlgS6::OnClickColorBtn );


	m_pkPaintTitle->InitState(true);

	CheckPaint();
}

void KGCRoomCreateDlgS6::OnClickColorBtn( )
{
	m_pkRoomName->SetFontColor(SiKGCRoomListManager()->GetCreateRoomTitleColor());
}


void KGCRoomCreateDlgS6::OnClickBtnClose( )
{
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}
//던전 변경시에만 불려야 한다.
void KGCRoomCreateDlgS6::UpdateState( void )
{
	for ( int i = KDSI_BTN_SET_0 ; i < KDSI_BTN_SET_NUM ; i++ ) {
		m_saBtnSet[i]->ToggleRender(false);
	}
	
	m_pkLevelBackGround->ToggleRender(false);
	m_pkLevelTitle->ToggleRender(false);

	if(SiKGCWorldMapManager()->IsHeroDungeonMode()) 
		return;

	m_pkLevelBackGround->ToggleRender(true);
	m_pkLevelTitle->ToggleRender(true);

	bool bHell = SiKGCWorldMapManager()->IsHellMode();
	bool bBreak = SiKGCWorldMapManager()->IsBreakMode();
	std::pair<int, int> pairLevel = SiKGCWorldMapManager()->GetCurrentDungeonPlayLevelAndLastLevel();

    if( bHell == false ) {
		RenderBtnSet(KDSI_BTN_SET_0);
	}
	else if (bBreak == true) {
		if (pairLevel.second < 2) {
			RenderBtnSet(KDSI_BTN_SET_5);
		}
		else {
			RenderBtnSet(KDSI_BTN_SET_4);
		}
	}
	else {
		RenderBtnSet(pairLevel.second);
	}
}

void KGCRoomCreateDlgS6::RenderBtnSet( int index )
{
	if(index < KDSI_BTN_SET_0 || index > KDSI_BTN_SET_5) return;
	m_saBtnSet[index]->SetWndState();
	m_saBtnSet[index]->SetBtnState();
	m_saBtnSet[index]->ToggleRender(true);
}

void KGCRoomCreateDlgS6::OnCreateRoom( void )
{
	WCHAR szRoomName[MAX_EDIT_LEN + 1] = { 0, };
	StringCchCopyW( szRoomName, MAX_EDIT_LEN + 1, m_pkRoomName->GetText() );
	SiKSlangFilter()->Filter( szRoomName, KSlangFilter::SLANG_FILTER_CHAT );
	//g_kCurseFilter.ConvertString( szRoomName, MAX_EDIT_LEN + 1 );
	std::wstring RoomName = szRoomName;

	g_pkStrLoader->Replacer(RoomName, L"#c", L"");
	g_pkStrLoader->Replacer(RoomName, L"#C", L"");

	std::wstring strRoomName = RoomName;
	std::wstring strRoomPassword = m_pkRoomPassword->GetText();

//#if defined( NATION_KOREA___ )
//	if ( g_kGlobalValue.m_kLoginInfo.bNewUser == true && !g_MyD3D->m_kGuideMgr.IsCompleteGuideNewbie())
//	{
//		if(strRoomPassword == L"")
//		{
//			int iTempPassWord1 = rand() % 18;
//			int iTempPassWord2 = rand() % 18;
//
//			WCHAR szTempPassWord[64] = {0,};
//
//			strRoomPassword = _itow(iTempPassWord1, szTempPassWord, 10);
//			strRoomPassword += _itow(iTempPassWord2, szTempPassWord, 10);
//		}
//	}
//#endif
	if(SiKGCRoomListManager()->GetCreateRoomTitleColorIndex() > 0 ) {
		strRoomName = L"#c" + SiGCTextColorManager()->GetColorString(SiKGCRoomListManager()->GetCreateRoomTitleColorIndex()) + strRoomName + L"#cX";
	}

	KRoomOptions options;
	HRESULT hr = SiKGCRoomManager()->CreateRoom(strRoomName, strRoomPassword, MAX_PLAYER_NUM, false, options);

    if ( !( SUCCEEDED( hr ) || hr == E_ABORT ) ) {
        return;
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

    if (SUCCEEDED( hr )) {
        // 룸리스트 및 팝업창 꺼줌
        OnClickBtnClose();
        g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_DUNGEON_CREATE ) );
    }

}

void KGCRoomCreateDlgS6::CheckPaint()
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

IMPLEMENT_CLASSNAME( KGCRoomQuickJoinDlgS6 );
IMPLEMENT_WND_FACTORY( KGCRoomQuickJoinDlgS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomQuickJoinDlgS6, "gc_quick_join_s6" );

KGCRoomQuickJoinDlgS6::KGCRoomQuickJoinDlgS6( void )
: m_pkJoinRoomBtn(NULL)
, m_pkBossInfo(NULL)
, m_pkUserCombo(NULL)
, m_pkCloseBtn(NULL)
{
	m_mapOption.clear();
	LINK_CONTROL( "dungeon_boss_info_s6" ,m_pkBossInfo);
	LINK_CONTROL( "join_room_btn" ,m_pkJoinRoomBtn);
	LINK_CONTROL( "user_num_combobox", m_pkUserCombo );
	LINK_CONTROL( "end_btn", m_pkCloseBtn );
	
}

KGCRoomQuickJoinDlgS6::~KGCRoomQuickJoinDlgS6( void )
{
}


void KGCRoomQuickJoinDlgS6::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkJoinRoomBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickQuickJoin );
	GCWND_MSG_MAP( m_pkCloseBtn, KD3DWnd::D3DWE_BUTTON_CLICK, Close );
}

void KGCRoomQuickJoinDlgS6::OnCreate( void )
{
	m_pkCloseBtn->InitState(true,true,this);
    m_pkCloseBtn->SetHotKey( DIK_ESCAPE );
	m_pkJoinRoomBtn->InitState(true,true,this);
	m_pkUserCombo->InitState(true,true,this);
	m_pkBossInfo->SetBoss();
	m_pkBossInfo->InitState(true,false,NULL);
	m_mapOption.clear();
	m_mapOption.insert(std::make_pair(static_cast<DWORD>(0),static_cast<DWORD>(4)));

	int iMaxUserNum = SiKGCWorldMapManager()->GetDungeonMaxPlayerNum();

	m_pkUserCombo->DeleteAllItem();

	for ( int i = 1 ; i < (iMaxUserNum + 1) ; ++i ) {
		if ( i == 1) {
			m_pkUserCombo->AddString(g_pkStrLoader->GetString(STR_ID_RAID_NON_OPTION));
			continue;
		}
		WCHAR szCharNum[10];
		_itow(i,szCharNum,10);
		m_pkUserCombo->AddString(szCharNum);
	} 

	m_pkUserCombo->SetComboStatic( g_pkStrLoader->GetString(STR_ID_RAID_NON_OPTION) );
	m_pkUserCombo->SetCurSel( 0 );
}

void KGCRoomQuickJoinDlgS6::Close( )
{
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCRoomQuickJoinDlgS6::OnClickQuickJoin( void )
{
	if ( m_pkUserCombo->GetCurSel() <= 0){
		m_mapOption.clear();
	}
	else {
		m_mapOption[OT_USER_NUM] = m_pkUserCombo->GetCurSel() + 1;
	}

	if( FAILED( SiKGCRoomManager()->JoinRoom( KJoinRoomReqInfo::JOIN_QUICK, m_mapOption, -1, L"", SiKGCRoomManager()->IsChannelRoomList() ) ) && Result_Join != EPWET_QUICK_JOIN_TIME_OVER )
	{
		KRoomOptions options;
		if (FAILED(SiKGCRoomManager()->CreateRoom(KGCRoomManager::m_vecDungeonRoomNameList[rand() % KGCRoomManager::m_vecDungeonRoomNameList.size()].c_str(), L"", MAX_PLAYER_NUM, true, options)))
		{
			Close();
			return;
		}
	}
	Close();
	g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_DUNGEON_ROOM_JOIN ) );


}

