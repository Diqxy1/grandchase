#include "stdafx.h"
#include "GCRoomDungeonSelectDlgS6.h"
#include "GCRoomDungeonBtnS6.h"
#include "KGCWorldMapManager.h"

IMPLEMENT_CLASSNAME( KGCRoomDungeonSelectDlgS6 );
IMPLEMENT_WND_FACTORY( KGCRoomDungeonSelectDlgS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomDungeonSelectDlgS6, "gc_room_dungeon_sel_dlg_s6" );

KGCRoomDungeonSelectDlgS6::KGCRoomDungeonSelectDlgS6( void )
: m_pkScrollbar(NULL)
, m_iScrollpos(0)
, m_pkCloseBtn(NULL)
, m_iSelectMode(0)
{
	LINK_CONTROL("scroll",m_pkScrollbar);
	LINK_CONTROL("end_btn", m_pkCloseBtn);
}

KGCRoomDungeonSelectDlgS6::~KGCRoomDungeonSelectDlgS6( void )
{
}

void KGCRoomDungeonSelectDlgS6::ActionPerformed( const KActionEvent& event )
{

}

void KGCRoomDungeonSelectDlgS6::OnCreate( void )
{
	CreateRoomListUI();
	SetMapInfo();

	m_iScrollpos = 0;
	m_iSelectMode = 0;


	m_pkScrollbar->InitState(true,true,this);
	m_pkScrollbar->SetScrollPos(0);
	m_pkScrollbar->SetScrollRangeMin(0);
	m_pkScrollbar->SetScrollRangeMax(1);
	m_pkScrollbar->SetScrollPageSize( 2 );

	InitStateBtn();

	SetBtnList();
	
	m_pkCloseBtn->InitState(true,true,this);
	m_pkCloseBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomDungeonSelectDlgS6::OnClickBtnClose);
	m_pkCloseBtn->SetHotKey(DIK_ESCAPE);

	g_pkUIMgr->ConnectGlobalEventProcedure( KD3DWnd::D3DWE_SCROLLBAR_WHEELUP, this, &KGCRoomDungeonSelectDlgS6::OnScrollPos );
	g_pkUIMgr->ConnectGlobalEventProcedure( KD3DWnd::D3DWE_SCROLLBAR_WHEELDOWN, this, &KGCRoomDungeonSelectDlgS6::OnScrollPos );
	g_pkUIMgr->ConnectGlobalEventProcedure( KD3DWnd::D3DWE_SCROLLBAR_DRAG, this, &KGCRoomDungeonSelectDlgS6::OnScrollPos );
	g_pkUIMgr->ConnectGlobalEventProcedure( KD3DWnd::D3DWE_SCROLLBAR_UPBTN, this, &KGCRoomDungeonSelectDlgS6::OnScrollPos );
	g_pkUIMgr->ConnectGlobalEventProcedure( KD3DWnd::D3DWE_SCROLLBAR_DOWNBTN, this, &KGCRoomDungeonSelectDlgS6::OnScrollPos );

}

void KGCRoomDungeonSelectDlgS6::OnScrollPos( void )
{
	int iPos = m_pkScrollbar->GetScrollPos();

	if ( m_iScrollpos != iPos )
	{
		m_iScrollpos = iPos;
		g_KDSound.Play( "73" );
		SetBtnList();
	}
}

void KGCRoomDungeonSelectDlgS6::InitBtnMode()
{
	m_iScrollpos = 0;
	m_pkScrollbar->SetScrollPos(m_iScrollpos);

	SetBtnList();
}

void KGCRoomDungeonSelectDlgS6::OnClickBtnClose( void )
{
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCRoomDungeonSelectDlgS6::CreateRoomListUI( void )
{
	if(m_vecBtnList.empty() == false) return;

	std::vector<KD3DWnd*> &vecRoomList = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_dungeon_room_list_dlg_s6.stg", "","room_dungeon_sel_dlg_s6\\dungeon_btn",true,KRDSD_LIST_MAX );

	for ( std::vector<KD3DWnd*>::iterator vit = vecRoomList.begin() ; vit != vecRoomList.end() ; ++vit ) {
		m_vecBtnList.push_back(static_cast<KGCRoomDungeonBtnS6*>(*vit));
		(*vit)->InitState(false,true,this);
	}

	SetBtnPos();
}

void KGCRoomDungeonSelectDlgS6::InitStateBtn( void )
{
	for (  std::vector<KGCRoomDungeonBtnS6*>::iterator vit = m_vecBtnList.begin() ; vit != m_vecBtnList.end() ; vit++ ) {
		(*vit)->InitState(false,true,this);
		(*vit)->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomDungeonSelectDlgS6::OnClickBtnBar);
	}
}

void KGCRoomDungeonSelectDlgS6::OnClickBtnBar( const KActionEvent& event )
{
	static_cast<KGCRoomDungeonBtnS6*>(event.m_pWnd)->OnClick();
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}



void KGCRoomDungeonSelectDlgS6::SetBtnPos( void )
{
	int i = 0;

	float fGapY = 39.f;

	float fStartPosY = 0.f;

	if ( m_vecBtnList.begin() != m_vecBtnList.end() ) {
		fStartPosY = m_vecBtnList[0]->GetFixedWindowLocalPos().y;
	}

	for (  std::vector<KGCRoomDungeonBtnS6*>::iterator vit = m_vecBtnList.begin() ; vit != m_vecBtnList.end() ; vit++, i++ ) {
		(*vit)->SetWindowPosY( fStartPosY + ( fGapY * i ));
	}
}

void KGCRoomDungeonSelectDlgS6::SetMapInfo( void )
{
	m_mapBtnInfoList.clear();
	std::map<int,std::vector<EGCGameMode>> &mapGameMode = SiKGCWorldMapManager()->GetAllGameMode();
	for ( std::map<int,std::vector<EGCGameMode>>::iterator mit = mapGameMode.begin(); mit != mapGameMode.end() ; ++mit ) {
		if( mit->second.empty() ) continue;
		for ( std::vector<EGCGameMode>::iterator vit = mit->second.begin() ; vit != mit->second.end() ; ++vit ) {
			if ( SiKGCRoomManager()->IsAvailableGameMode(GC_GMC_DUNGEON, *vit) == false )
				continue;

			m_mapBtnInfoList[mit->first].push_back(*vit);
		}
	}

}

void KGCRoomDungeonSelectDlgS6::SetBtnList( void )
{
	std::map<int,  std::vector<int>>::iterator mit = m_mapBtnInfoList.find(SiKGCWorldMapManager()->GetCurrentWorldMap());

	if ( mit == m_mapBtnInfoList.end() ) return;
	std::vector<int> vecCurrentModeList = mit->second;

	int scrollMaxSize = static_cast<int>(vecCurrentModeList.size()) - 3;

	if( vecCurrentModeList.empty() || scrollMaxSize < 1){
		scrollMaxSize = 1;
	}

	if(m_iScrollpos > scrollMaxSize){
		m_iScrollpos = scrollMaxSize;
	}

	m_pkScrollbar->SetScrollRangeMax(scrollMaxSize);

	SetShowBtnList();
}

void KGCRoomDungeonSelectDlgS6::SetShowBtnList( )
{
	std::map<int,  std::vector<int>>::iterator mit = m_mapBtnInfoList.find(SiKGCWorldMapManager()->GetCurrentWorldMap());

	if ( mit == m_mapBtnInfoList.end() ) return;

	std::vector<int> vecCurrentModeList = mit->second;
	
	int size = vecCurrentModeList.size();

	std::vector<int> vecBtnInfoList;

	if ( size < KRDSD_LIST_MAX ) {
		vecBtnInfoList = vecCurrentModeList;
	}
	else {
		std::copy(vecCurrentModeList.begin() + (m_iScrollpos),
			vecCurrentModeList.end(),std::back_inserter(vecBtnInfoList));
	}

	std::vector<int>::iterator vitTemp = vecBtnInfoList.begin();

	int showSize = m_vecBtnList.size();
	for(int i = 0;i<showSize;++i){
		if(vitTemp == vecBtnInfoList.end()){
			m_vecBtnList[i]->ToggleRender(false);
			continue;
		}

		m_vecBtnList[i]->SetDungeonID(*vitTemp);
		m_vecBtnList[i]->ToggleRender(true);
		vitTemp++;
	}
}

