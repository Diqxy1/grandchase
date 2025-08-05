#include "stdafx.h"
#include "GCBonusBoxS6.h"


IMPLEMENT_CLASSNAME( KGCBonusItemS6 );
IMPLEMENT_WND_FACTORY( KGCBonusItemS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCBonusItemS6, "gc_bonus_item_s6" );

KGCBonusItemS6::KGCBonusItemS6( void )
: m_pkItemImage(NULL)
, m_pkItemName(NULL)
{
	LINK_CONTROL( "item_image", m_pkItemImage );
	LINK_CONTROL( "static_name", m_pkItemName );


}

KGCBonusItemS6::~KGCBonusItemS6( void )
{
}

void KGCBonusItemS6::ActionPerformed( const KActionEvent& event )
{

}

void KGCBonusItemS6::OnCreate( void )
{	
	m_pkItemImage->InitState(false,false,this);
	m_pkItemName->InitState(false,false,this);
}

void KGCBonusItemS6::SetItemInfo( GCITEMID iItemID )
{
	std::vector<GCITEMID> vecTemp = SiKGCRoomListManager()->GetCurrentModeAndLevelRewardItem();

	m_pkItemImage->ToggleRender(false);
	m_pkItemImage->SetItemInfo(0);
	m_pkItemName->ToggleRender(false);
	GCItem *pkItem = g_pItemMgr->GetItemData( iItemID );

	if ( pkItem == NULL ) return;

	m_pkItemImage->SetItemInfo(iItemID , -1, pkItem->dwSlotPosition & ESP_SET_ITEM , pkItem->iSetItemNumber);
	m_pkItemImage->ToggleRender(true);
	m_pkItemImage->ShowItem(true);
	m_pkItemName->SetText(pkItem->strItemName.c_str() );
	m_pkItemName->SetAlign(DT_CENTER);
	m_pkItemName->ToggleRender(true);

}




IMPLEMENT_CLASSNAME( KGCBonusBoxUserInfoS6 );
IMPLEMENT_WND_FACTORY( KGCBonusBoxUserInfoS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCBonusBoxUserInfoS6, "gc_bonus_box_user_info_s6" );

KGCBonusBoxUserInfoS6::KGCBonusBoxUserInfoS6( void )
: m_pkUserNick(NULL)
{
	for ( int i = 0 ; i < GC_CHAR_NUM ; ++i ) {
		m_pkChar[i] = NULL;
		char strTemp[MAX_PATH];
		sprintf( strTemp, "headup_display_char%d", i);
		LINK_CONTROL( strTemp, m_pkChar[i] );
	}

	LINK_CONTROL( "static_name", m_pkUserNick );
}

KGCBonusBoxUserInfoS6::~KGCBonusBoxUserInfoS6( void )
{
}

void KGCBonusBoxUserInfoS6::ActionPerformed( const KActionEvent& event )
{

}

void KGCBonusBoxUserInfoS6::OnCreate( void )
{
	for ( int i = 0 ; i < GC_CHAR_NUM ; ++i ) {
		m_pkChar[i]->InitState(false,false,NULL);
	}

	m_pkUserNick->InitState(true,false,NULL);

}

void KGCBonusBoxUserInfoS6::SetUserInfo( int iChar, std::wstring strNick )
{
	for ( int i = 0 ; i < GC_CHAR_NUM ; ++i ) {
		m_pkChar[i]->ToggleRender(iChar == i);
	}
	m_pkUserNick->SetText(strNick.c_str());
	m_pkUserNick->SetAlign(DT_CENTER);

}



IMPLEMENT_CLASSNAME( KGCBonusBoxS6 );
IMPLEMENT_WND_FACTORY( KGCBonusBoxS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCBonusBoxS6, "gc_bonus_box_s6" );

KGCBonusBoxS6::KGCBonusBoxS6( void )
: m_pkBack(NULL)
, m_pkUserInfo(NULL)
, m_pkBaseItem(NULL)
, m_pkAddItem(NULL)
, m_bComplete(false)
, m_tmStartTime(0)
{
	LINK_CONTROL( "background_main", m_pkBack );
	LINK_CONTROL( "bonus_box_user_info_s6", m_pkUserInfo );
	LINK_CONTROL( "item_box", m_pkBaseItem );
	LINK_CONTROL( "add_item_box", m_pkAddItem );

}

KGCBonusBoxS6::~KGCBonusBoxS6( void )
{
}

void KGCBonusBoxS6::ActionPerformed( const KActionEvent& event )
{

}

void KGCBonusBoxS6::OnCreate( void )
{
	m_pkBack->InitState(true,false,NULL);
	m_pkUserInfo->InitState(true,false,NULL);
	m_pkBaseItem->InitState(true,false,NULL);
	m_pkAddItem->InitState(false,false,NULL);
	m_bComplete = false;
}

void KGCBonusBoxS6::SetRewardItem( std::map<int, KDropItemInfo> mapRewardItem, int iChar, std::wstring strNickName )
{
	m_bComplete = false;

	m_pkAddItem->ToggleRender(false);	
	m_pkBack->SetSize( m_pkBack->GetWidth(), 258 );

	for ( std::map<int, KDropItemInfo>::iterator mit = mapRewardItem.begin() ; mit != mapRewardItem.end() ; ++mit ) {
		if ( mit->first == KGameResultOut::DEFAULT_REWARD ) {
			m_pkBaseItem->SetItemInfo( mit->second.m_ItemID/10 );
		}
		else if ( mit->first == KGameResultOut::ADDITION_REWARD ) {
			m_pkAddItem->SetItemInfo( mit->second.m_ItemID/10 );
			m_pkAddItem->ToggleRender(true);
			m_pkBack->SetSize( m_pkBack->GetWidth(), 372 );
		}
	}

	m_pkUserInfo->SetUserInfo(iChar, strNickName);
	
}

void KGCBonusBoxS6::FrameMoveInEnabledState( void )
{
	if ( m_bComplete == true ) return;

	if ( (m_tmStartTime + 10) <=  GCUTIL_TIME::GetCurTime() ) {
		m_bComplete = true;
	}
}