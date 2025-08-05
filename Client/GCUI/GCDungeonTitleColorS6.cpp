#include "stdafx.h"
#include "GCDungeonTitleColorS6.h"
#include "GCItemManager.h"
#include "GCTextColorManager.h"

IMPLEMENT_CLASSNAME( KGCDungeonTitleColorS6 );
IMPLEMENT_WND_FACTORY( KGCDungeonTitleColorS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCDungeonTitleColorS6, "gc_dungeon_title_color_s6" );

KGCDungeonTitleColorS6::KGCDungeonTitleColorS6( void )
: m_pkCheck(NULL)
, currentIndex(0)
{
	char szTemp[128];
	memset(szTemp, 0, sizeof(char) * 128);

	for ( int i = 0 ; i < KDTC_COLOR_MAX ; i++ ) {
		sprintf(szTemp, "paint_%d", i);
		m_saColorBtn[i] = NULL;
		LINK_CONTROL(szTemp,m_saColorBtn[i]);
	}

	LINK_CONTROL("check",m_pkCheck);

#if defined(NATION_PHILIPPINE)
	LINK_CONTROL("paint_GM",m_pkGMColorBtn);
#endif
}

KGCDungeonTitleColorS6::~KGCDungeonTitleColorS6( void )
{
}

void KGCDungeonTitleColorS6::ActionPerformed( const KActionEvent& event )
{

}

void KGCDungeonTitleColorS6::OnCreate( void )
{
#if defined(NATION_PHILIPPINE)
    if( g_kGlobalValue.m_kUserInfo.eGCUserLevel != USERLEVEL_ADMIN ) {
        m_pkGMColorBtn->InitState(false);
    } else {
        m_pkGMColorBtn->InitState(true);
    }
#endif

	for ( int i = 0 ; i < KDTC_COLOR_MAX ; i++ ) {
		if (m_saColorBtn[i]) {
			m_saColorBtn[i]->InitState(true, true, this);
			m_saColorBtn[i]->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCDungeonTitleColorS6::OnClickColorBtn);
		}
	}

	m_pkCheck->InitState(false);

	if (true == g_kGlobalValue.isNickColorChangeOn) {
		SiGCTextColorManager()->SetColorNum(GCTextColorManager::CASH_ITEM_COLOR_NUM + 1);
	}
	else {
		if (g_pItemMgr->IsCashPaintItem()) {
			SiGCTextColorManager()->SetColorNum(GCTextColorManager::CASH_ITEM_COLOR_NUM + 1);
		}
		else if (g_pItemMgr->IsPaintItem()) {
			SiGCTextColorManager()->SetColorNum(GCTextColorManager::GP_ITEM_COLOR_NUM + 1);
		}
		else {
			SiGCTextColorManager()->SetColorNum(0);
		}
	}

	SetColorBtn();
}

void KGCDungeonTitleColorS6::OnClickColorBtn( const KActionEvent& event )
{
	for ( int i = 0 ; i < KDTC_COLOR_MAX ; i++ ) {
		if ( event.m_pWnd == m_saColorBtn[i] ) {
			ClickColorBtn(i);
			return;
		}
	}
	
	ClickColorBtn(-1);
}

void KGCDungeonTitleColorS6::ClickColorBtn( int iIndex )
{
	if (iIndex == -1 || SiKGCRoomListManager()->GetCreateRoomTitleColorIndex() == iIndex + 1) {
		SiKGCRoomListManager()->SetCreateRoomTitleColor(-1);
		m_pkCheck->ToggleRender(false);
		return;
	}

	SiKGCRoomListManager()->SetCreateRoomTitleColor(iIndex + 1);
	SetCheckPos(iIndex);
	currentIndex = iIndex;
}

void KGCDungeonTitleColorS6::SetCheckPos( int iIndex )
{
	float fXpos = -2.f + 18.f * iIndex;

	m_pkCheck->SetWindowPosXDirect(fXpos);
	m_pkCheck->ToggleRender(true);
}

void KGCDungeonTitleColorS6::SetColorBtn( )
{
	for ( int i = 0 ; i < KDTC_COLOR_MAX ; i++ ) {
		bool bItem = g_pItemMgr->IsPaintItem(); 	
		bool bCashItem = (SiGCTextColorManager()->GetMaxColor() - 1) > i;

		m_saColorBtn[i]->Lock( (!bItem) || (!bCashItem) );
	}
}

