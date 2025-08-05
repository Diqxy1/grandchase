#include "stdafx.h"
#include "KGCNewForgeDlg.h"
#include "GCSearchBar.h"

IMPLEMENT_CLASSNAME( KGCNewForgeDlg );
IMPLEMENT_WND_FACTORY( KGCNewForgeDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCNewForgeDlg, "gc_new_forge_dlg" );

KGCNewForgeDlg::KGCNewForgeDlg(void)
{
	m_pkCreateBtn = NULL;
	m_pkSelectedCreateBtn = NULL;
	m_pkBreakUpBtn = NULL;
	m_pkSelectedBreakUpBtn = NULL;
	m_pkCloseBtn = NULL;
	m_pkItemCreation = NULL;
	m_pkCraftMachine = NULL;
	m_bFirstOpen = true;
	m_pkSearchBar = nullptr;

	LINK_CONTROL( "create_btn",						m_pkCreateBtn );
	LINK_CONTROL( "close_btn",						m_pkCloseBtn );
	LINK_CONTROL( "breakup_btn",					m_pkBreakUpBtn );
	LINK_CONTROL( "sel_breakup_btn",				m_pkSelectedBreakUpBtn );
	LINK_CONTROL( "sel_create_btn",					m_pkSelectedCreateBtn );
	LINK_CONTROL( "new_forge_item_creation_dlg",	m_pkItemCreation );
	LINK_CONTROL( "new_forge_item_breakup_dlg",		m_pkItemBreakUp );
	LINK_CONTROL( "new_forge_item_crafting_dlg",	m_pkCraftMachine );
	LINK_CONTROL( "search_bar",	m_pkSearchBar );


	m_eCurState = EKGCNEWFORGE_STATE::EKGCFOGE_CREATE;
}

KGCNewForgeDlg::~KGCNewForgeDlg(void)
{
}

void KGCNewForgeDlg::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP(m_pkCloseBtn, D3DWE_BUTTON_CLICK, OnClickCloseBtn);
	GCWND_MSG_MAP(m_pkCreateBtn, D3DWE_BUTTON_CLICK, OnClickCreateBtn);
	GCWND_MSG_MAP(m_pkBreakUpBtn, D3DWE_BUTTON_CLICK, OnClickBrakeUpBtn);
}

void KGCNewForgeDlg::OnCreate()
{
	if(m_pkCreateBtn)
	{
		m_pkCreateBtn->InitState( false, true, this);
	}

	if(m_pkBreakUpBtn)
	{
		m_pkBreakUpBtn->InitState( false, true, this);
	}

	if(m_pkSelectedCreateBtn)
	{
		m_pkSelectedCreateBtn->InitState( false, true);
	}

	if(m_pkSelectedBreakUpBtn)
	{
		m_pkSelectedBreakUpBtn->InitState( false, true);
	}

	if(m_pkCloseBtn)
	{
		m_pkCloseBtn->InitState( true, true, this);
		m_pkCloseBtn->SetHotKey(DIK_ESCAPE);
	}

	if( m_pkItemCreation )
	{
		m_pkItemCreation->InitState(true, true, this);
	}

	if( m_pkItemBreakUp )
	{
		m_pkItemBreakUp->InitState( false , false );
	}

	if( m_pkCraftMachine )
	{
		m_pkCraftMachine->InitState( true, true, this );
	}

	if (m_pkSearchBar != nullptr)
	{
		m_pkSearchBar->InitState(true, true, this);
	}

	OnClickCreateBtn();
	SiGCForgeManager()->InitWaitItem();

}

void KGCNewForgeDlg::OnClickBrakeUpBtn()
{
	g_KDSound.Play( "31" );
	ChangeState(EKGCNEWFORGE_STATE::EKGCFOGE_BREAKUP);

	m_pkSelectedCreateBtn->ToggleRender( false );
	m_pkCreateBtn->ToggleRender( true );
	m_pkSelectedBreakUpBtn->ToggleRender( true );
	m_pkBreakUpBtn->ToggleRender( false );

	m_pkItemBreakUp->ToggleRender( true );
	m_pkItemCreation->ToggleRender(false);

	m_pkItemBreakUp->OnInitialize();
}

void KGCNewForgeDlg::OnClickCreateBtn()
{
	g_KDSound.Play( "31" );
	ChangeState(EKGCNEWFORGE_STATE::EKGCFOGE_CREATE);

	m_pkSelectedCreateBtn->ToggleRender( true );
	m_pkCreateBtn->ToggleRender( false );
	m_pkSelectedBreakUpBtn->ToggleRender( false );
	m_pkBreakUpBtn->ToggleRender( true );

	m_pkItemBreakUp->ToggleRender( false );
	m_pkItemCreation->ToggleRender(true);
}

void KGCNewForgeDlg::OnClickCloseBtn()
{
	g_KDSound.Play( "31" );

	if( GetForgeCraftState() != KGCNewForgeItemCraftMachine::EFMS_NORMAL )
	{ 
		return;
	}

	m_pkCraftMachine->InitCraftMachine();

	m_bFirstOpen = true;

	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

bool KGCNewForgeDlg::InitDialog( IN const KDialogInfo& kInfo_ )
{
	int iCharType = static_cast<int>( kInfo_.m_lParam );

	if( iCharType < GC_CHAR_ELESIS || iCharType >= GC_CHAR_NUM ) 
	{ 
		return false;
	}

	SiGCForgeManager()->SetCharacter( iCharType );

	if( m_bFirstOpen == true )
	{	
		ChangeState(EKGCNEWFORGE_STATE::EKGCFOGE_CREATE);
		m_bFirstOpen = false;
	}

	return true;
}

void KGCNewForgeDlg::ChangeState(EKGCNEWFORGE_STATE iState )
{
	m_pkSelectedCreateBtn->ToggleRender(iState == EKGCNEWFORGE_STATE::EKGCFOGE_CREATE);
	m_pkSelectedBreakUpBtn->ToggleRender(iState == EKGCNEWFORGE_STATE::EKGCFOGE_BREAKUP);

	m_pkCreateBtn->ToggleRender(iState != EKGCNEWFORGE_STATE::EKGCFOGE_CREATE);
	m_pkBreakUpBtn->ToggleRender(iState != EKGCNEWFORGE_STATE::EKGCFOGE_BREAKUP);

	m_pkCraftMachine->SetForgeState(iState);

	m_eCurState = iState;

	SiGCForgeManager()->SetEnableBreakUp(iState == EKGCNEWFORGE_STATE::EKGCFOGE_BREAKUP );
}

bool KGCNewForgeDlg::IsEnabledRecipeBar( GCITEMID itemID )
{
	return m_pkItemCreation->IsEnabledRecipeBar( itemID );
}

void KGCNewForgeDlg::SetSelectedMaterial( IN const GCITEMUID itemUID_ )
{
	m_pkCraftMachine->SetSelectedMaterial(itemUID_);
}

int KGCNewForgeDlg::GetForgeCraftState()
{
	return m_pkCraftMachine->GetForgeCraftState();
}

void KGCNewForgeDlg::UpdateRecipeUI()
{
	m_pkItemCreation->SetInfobarList();
	m_pkItemCreation->OnInitInfobar();
}

void KGCNewForgeDlg::FindItemFromName(const WCHAR* strName_)
{
	if (strName_)
	{
		m_strFilter = strName_;
	}
	else
	{
		m_strFilter.clear();
	}

	if (m_eCurState == EKGCNEWFORGE_STATE::EKGCFOGE_CREATE)
	{
		m_pkItemCreation->FilterInfoBar(m_strFilter);
	}
	else
	{
		m_pkItemBreakUp->FilterInventoryList(m_strFilter);
	}
}