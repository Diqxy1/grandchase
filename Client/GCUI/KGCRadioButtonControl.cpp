#include "stdafx.h"
#include "KGCRadioButtonControl.h"

IMPLEMENT_CLASSNAME( KGCRadioButtonControl );
IMPLEMENT_WND_FACTORY( KGCRadioButtonControl );
IMPLEMENT_WND_FACTORY_NAME( KGCRadioButtonControl, "gc_radio_btn_control" );

KGCRadioButtonControl::KGCRadioButtonControl(void)
: m_pkCheckBack(NULL)
, m_pkCheckSign(NULL)
, m_pkStatic(NULL)
{
    LINK_CONTROL( "check_back", m_pkCheckBack );
    LINK_CONTROL( "check_sign", m_pkCheckSign );
    LINK_CONTROL( "static", m_pkStatic );
}

KGCRadioButtonControl::~KGCRadioButtonControl(void)
{
}

void KGCRadioButtonControl::OnCreate()
{
    m_pkCheckBack->InitState(true);
    m_pkCheckSign->InitState(false);
    m_pkStatic->InitState(true);    
}
                                           
void KGCRadioButtonControl::OnCreateComplete()
{
}

void KGCRadioButtonControl::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkCheckBack && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if( m_pkCheckSign->IsRenderOn() ) {
            m_pkCheckSign->ToggleRender(false);
            SpeakToActionListener( KActionEvent( this, D3DWE_CHECK_DISABLED ) );
        }
        else {
            m_pkCheckSign->ToggleRender(true);
            SpeakToActionListener( KActionEvent( this, D3DWE_CHECK_ENABLED ) );
        }
    }
}

void KGCRadioButtonControl::FrameMoveInEnabledState()
{
}

void KGCRadioButtonControl::PostChildDraw( void )
{
}

void KGCRadioButtonControl::SetStatic(std::wstring& wstr, DWORD dwAlign)
{
    m_pkStatic->SetForceWordWrapText(wstr, true);
    m_pkStatic->SetAlign(dwAlign);
}

bool KGCRadioButtonControl::IsChecked()
{
    return m_pkCheckSign->IsRenderOn();
}

void KGCRadioButtonControl::RenderCheckSign(bool bRender )
{
    m_pkCheckSign->ToggleRender(bRender);
}