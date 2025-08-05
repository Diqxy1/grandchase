#include "stdafx.h"
#include ".\KGCCheckButton.h"
//


IMPLEMENT_CLASSNAME( KGCCheckButton );
IMPLEMENT_WND_FACTORY( KGCCheckButton );
IMPLEMENT_WND_FACTORY_NAME( KGCCheckButton, "gc_checkbutton" );

KGCCheckButton::KGCCheckButton(void)
{

    m_pCheckBtn = NULL;
    m_bChecked = false;
    LINK_CONTROL("checked",		    m_pCheckBtn);    
}

KGCCheckButton::~KGCCheckButton(void)
{

}

void KGCCheckButton::OnCreate( void )
{   
   m_pCheckBtn->SetSelfInputCheck( true );
   m_pCheckBtn->AddActionListener( this );
}

void KGCCheckButton::OnCreateComplete( void )
{
    m_pCheckBtn->ToggleRender(true);
    m_pCheckBtn->SetNotDraw(m_bChecked);
}

void KGCCheckButton::OnDestroyComplete( void )
{

}

void KGCCheckButton::ActionPerformed( const KActionEvent& event )
{
    if ( KD3DWnd::D3DWE_BUTTON_CLICK == event.m_dwCode )
    {
        //체크박스 클릭시
        {
            Click();
            SpeakToActionListener(KActionEvent(this,KD3DWnd::D3DWE_BUTTON_CLICK,KGCCheckButton::GC_CBUTTON_CHK));
        }
    }
}

void KGCCheckButton::FrameMoveInEnabledState( void )
{

}

void KGCCheckButton::Check(bool bChk)
{
    m_bChecked=bChk;
    m_pCheckBtn->SetNotDraw(m_bChecked);   
}

void KGCCheckButton::Click()
{
    m_bChecked=!m_bChecked;
    m_pCheckBtn->SetNotDraw(m_bChecked);   
}