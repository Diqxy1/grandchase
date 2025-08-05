#include "stdafx.h"
#include "GCForgeDlg.h"

IMPLEMENT_CLASSNAME( KGCForgeDlg );
IMPLEMENT_WND_FACTORY( KGCForgeDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCForgeDlg, "gc_new_forge_dlg" );

KGCForgeDlg::KGCForgeDlg( void )
:   m_pkCreateBtn(NULL)
,   m_pkSelectedCreateBtn(NULL)
,   m_pkBreakUpBtn(NULL)
,   m_pkSelectedBreakUpBtn(NULL)
,   m_pkCloseBtn(NULL)
,   m_pkItemCreation(NULL)
,   m_pkItemBreakUp(NULL)
,   m_pkPageChange(NULL)
,   m_pkCraftMachine(NULL)
{
    LINK_CONTROL("create_btn",m_pkCreateBtn);
    LINK_CONTROL("sel_create_btn",m_pkSelectedCreateBtn);
    LINK_CONTROL("breakup_btn",m_pkBreakUpBtn);
    LINK_CONTROL("sel_breakup_btn",m_pkSelectedBreakUpBtn);
    LINK_CONTROL("close_btn",m_pkCloseBtn);
    LINK_CONTROL("forge_item_creation_dlg",m_pkItemCreation);
    LINK_CONTROL("forge_item_breakup_dlg",m_pkItemBreakUp);
    LINK_CONTROL("forge_item_crafting_dlg", m_pkCraftMachine);

    
}

KGCForgeDlg::~KGCForgeDlg( void )
{
}

void KGCForgeDlg::ActionPerformed( const KActionEvent& event )
{

}

void KGCForgeDlg::OnCreate( void )
{
    m_pkCreateBtn->InitState(false, true, this);
    m_pkItemBreakUp->InitState(false,true,this);
    //m_pkPageChange->InitState(true,true,this);

    m_pkSelectedCreateBtn->InitState(true, false, NULL);
    m_pkBreakUpBtn->InitState(true, true, this);
    m_pkSelectedBreakUpBtn->InitState(false, false, NULL);
    m_pkCloseBtn->InitState(true, true, this);
    m_pkCloseBtn->SetHotKey(DIK_ESCAPE);

    m_pkItemCreation->InitState(true, true, this);

    m_pkCraftMachine->InitState(true,true,this);

    m_pkCreateBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCForgeDlg::OnClickCreateBtn);
    m_pkBreakUpBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCForgeDlg::OnClickBrakeUpBtn);
    m_pkCloseBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCForgeDlg::OnClickCloseBtn);

    OnClickCreateBtn();

    SiGCForgeManager()->InitWaitItem();

    
}

bool KGCForgeDlg::InitDialog( IN const KDialogInfo& kInfo_ )
{
    int iCharType = static_cast<int>( kInfo_.m_lParam );
    if( iCharType < GC_CHAR_ELESIS || iCharType >= GC_CHAR_NUM ) { 
        return false;
    }

    SiGCForgeManager()->SetCharacter( iCharType );
    return true;
}

void KGCForgeDlg::OnClickCreateBtn()
{
    ChangeState(EKGCFOGE_CREATE);
    m_pkItemCreation->ToggleRender(true);
    m_pkItemBreakUp->ToggleRender(false);
    m_pkItemCreation->SetMaxPage();

}

void KGCForgeDlg::OnClickBrakeUpBtn()
{
    ChangeState(EKGCFOGE_BREAKUP);
    m_pkItemCreation->ToggleRender(false);
    m_pkItemBreakUp->ToggleRender(true);
    m_pkItemBreakUp->RefreshItemList();

}

void KGCForgeDlg::OnClickCloseBtn()
{
    if( GetForgeCraftState() != KGCForgeItemCraftMachine::EFMS_NORMAL ) { 
        return;
    }

    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCForgeDlg::ChangeState( int iState )
{
    m_pkSelectedCreateBtn->ToggleRender(iState == EKGCFOGE_CREATE);
    m_pkSelectedBreakUpBtn->ToggleRender(iState == EKGCFOGE_BREAKUP);

    m_pkCreateBtn->ToggleRender(iState != EKGCFOGE_CREATE);
    m_pkBreakUpBtn->ToggleRender(iState != EKGCFOGE_BREAKUP);

    m_pkCraftMachine->SetForgeState(iState);
    SiGCForgeManager()->SetEnableBreakUp(iState == EKGCFOGE_BREAKUP );



}

bool KGCForgeDlg::IsEnabledRecipeBar( GCITEMID itemID )
{
    return m_pkItemCreation->IsEnabledRecipeBar( itemID );
}

void KGCForgeDlg::SetSelectedMatertial( IN const GCITEMUID itemUID_ )
{
    m_pkCraftMachine->SetSelectedMaterial( itemUID_ );
}

void KGCForgeDlg::UpdateRecipeUI()
{
    m_pkItemCreation->SetInfobarList();
    m_pkItemCreation->OnInitInfobar();
}
