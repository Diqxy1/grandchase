#include "stdafx.h"
#include "KGCGachaBuyBox.h"
#include "KGCGachaBuyItemSlot.h"

IMPLEMENT_CLASSNAME( KGCGachaBuyItemBox );
IMPLEMENT_WND_FACTORY( KGCGachaBuyItemBox );
IMPLEMENT_WND_FACTORY_NAME( KGCGachaBuyItemBox, "gc_gachaitembuybox" );

KGCGachaBuyItemBox::KGCGachaBuyItemBox()
: m_pkBtnClose( NULL )
{
	LINK_CONTROL( "btn_close",		m_pkBtnClose );

	char szTemp[50];
	for(int i = 0; i < 4; i++)
	{
		m_saBuyItemSlot[i] = NULL;
		sprintf(szTemp, "buy_item_slot%d", i);
		LINK_CONTROL(szTemp, m_saBuyItemSlot[i]);
	}
}

KGCGachaBuyItemBox::~KGCGachaBuyItemBox()
{

}

void KGCGachaBuyItemBox::ActionPerformed( const KActionEvent& event)
{
	if(event.m_pWnd == m_pkBtnClose && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
	{
		this->ToggleRender(false);
	}
}

void KGCGachaBuyItemBox::OnCreate()
{
	if( m_pkBtnClose )
	{
		m_pkBtnClose->InitState(true, true, this);
	}

	for( int i = 0; i < 4; i++ )
	{
		m_saBuyItemSlot[i]->InitState(false, true, this);
	}
}

void KGCGachaBuyItemBox::SetItems(const std::vector<DWORD>& vecItems)
{
	for( int i = 0; i < vecItems.size(); i++ )
	{
		m_saBuyItemSlot[i]->SetItem(vecItems[i]);
		m_saBuyItemSlot[i]->ToggleRender(true);
	}
}