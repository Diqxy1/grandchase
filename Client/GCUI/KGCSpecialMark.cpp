#include "stdafx.h"
#include "KGCSpecialMark.h"
#include "../MyD3D.h"
#include "GCEventManager.h"

IMPLEMENT_CLASSNAME( KGCSpecialMark );
IMPLEMENT_WND_FACTORY( KGCSpecialMark );
IMPLEMENT_WND_FACTORY_NAME( KGCSpecialMark, "gc_player_special_mark" );

KGCSpecialMark::KGCSpecialMark(void)
{
	m_ppMark[0] = NULL;
	m_ppMark[1] = NULL;
    m_ppMark[2] = NULL;
    m_ppMark[3] = NULL;

	LINK_CONTROL( "heart_gp",   m_ppMark[0] );
	LINK_CONTROL( "heart_cash", m_ppMark[1] );
    LINK_CONTROL( "friend2", m_ppMark[2] );
    LINK_CONTROL( "friend1", m_ppMark[3] );
}

KGCSpecialMark::~KGCSpecialMark(void)
{
}

void KGCSpecialMark::OnCreate( void )
{
	for( int i=0; i<4; i++ )
	{
		m_ppMark[i]->ToggleRender( false );
	}
}

void KGCSpecialMark::UpdateMark( int iIndex )
{
	for( int i=0; i<4; i++ )
	{
		m_ppMark[i]->ToggleRender( false );
	}

	//CheckEvent( iIndex );
}

void KGCSpecialMark::CheckEvent( int iIndex )
{
	SCharInfo &STemp = g_MyD3D->MyPlayer[iIndex]->GetCurrentChar();

	if ( SiKGCEventManager()->IsActiveEvent( EVENT_ID_VALENTINE ) == true )
	{
		if( g_pItemMgr->CheckEquipItemForItemID( &STemp, ITEM_ID_HEART_GP) )
		{
			m_ppMark[0]->ToggleRender( true );
		}
		else if( g_pItemMgr->CheckEquipItemForItemID( &STemp, ITEM_ID_HEART_CASH) )
		{
			m_ppMark[1]->ToggleRender( true );
		}	
	}	

	if ( SiKGCEventManager()->IsActiveEvent( EVENT_ID_FRIENDSHIP ) == true )
	{
		if( g_pItemMgr->CheckEquipItemForItemID( &STemp, ITEM_ID_FRIENDRING1) )
		{
			m_ppMark[2]->ToggleRender( true );
		}
		else if( g_pItemMgr->CheckEquipItemForItemID( &STemp, ITEM_ID_FRIENDRING2) )
		{
			m_ppMark[3]->ToggleRender( true );
		}	
	}
}
