#include "stdafx.h"
#include "GCVirtualKey.h"

IMPLEMENT_CLASSNAME( KGCVirtualKey );
IMPLEMENT_WND_FACTORY( KGCVirtualKey );
IMPLEMENT_WND_FACTORY_NAME( KGCVirtualKey, "gc_virtualkey" );

KGCVirtualKey::KGCVirtualKey( void ) :
m_bUpperCase(false)
{
    m_pkShowData = new KD3DStatic;

    m_pkShowData->SetSelfInputCheck( true );
    m_pkShowData->AddActionListener( this );
    m_pkShowData->ToggleRender( true );
    m_pkShowData->SetAlign( DT_CENTER );
    m_pkShowData->SetWndState(D3DWS_ENABLED);

    m_vecChild.push_back(m_pkShowData);
}

KGCVirtualKey::~KGCVirtualKey( void )
{
}

void KGCVirtualKey::OnCreate( void )
{
    m_pkShowData->SetWidth( GetWidth() );
    m_pkShowData->SetHeight( GetHeight() );
}

void KGCVirtualKey::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        SpeakToActionListener( KActionEvent(this,D3DWE_BUTTON_CLICK) );
        return;
    }
}

void KGCVirtualKey::OnInitialize()
{
    D3DXVECTOR2 vPos = GetFixedWindowPos();

    vPos.y += 8.f;
    m_pkShowData->SetWindowPos(vPos);
}

void KGCVirtualKey::FrameMoveInEnabledState( void )
{
}

void KGCVirtualKey::SetKeyData( const std::wstring& strKeyData_ )
{
    m_pkShowData->SetText(strKeyData_);
}

std::wstring KGCVirtualKey::GetKeyData()
{
    return m_pkShowData->GetText();
}
