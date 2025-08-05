#include "stdafx.h"
#include "GCNeonsignMessage.h"
//
//
//
//
//

IMPLEMENT_CLASSNAME( KGCNeonsignMessage );
IMPLEMENT_WND_FACTORY( KGCNeonsignMessage );
IMPLEMENT_WND_FACTORY_NAME( KGCNeonsignMessage, "gc_neonsign_message" );

#define MAX_MESSAGE_BYTES (50)

KGCNeonsignMessage::KGCNeonsignMessage(void)
: m_pkColor(NULL)
, m_pkMessage(NULL)
, m_pkBytes(NULL)
, m_bDefaultMsg(true)
{
    LINK_CONTROL( "color_palette",  m_pkColor );
    LINK_CONTROL( "edit_message",   m_pkMessage );
    LINK_CONTROL( "static_bytes",   m_pkBytes );
}

KGCNeonsignMessage::~KGCNeonsignMessage(void)
{
}

void KGCNeonsignMessage::OnCreate()
{
    m_pkColor->InitState( true );
    m_pkMessage->InitState( true );
    m_pkMessage->SetLimitText( MAX_MESSAGE_BYTES );
    SetEditText( true );
    m_pkBytes->InitState( true );
    m_pkBytes->SetAlign( DT_RIGHT );
    SetBytes();
}

void KGCNeonsignMessage::FrameMoveInEnabledState()
{
    if( SetBytes() == true )
    {
        m_pkMessage->SetFontColor( D3DCOLOR_ARGB(255,255,0,0) );
        m_pkBytes->SetFontColor( D3DCOLOR_ARGB(255,255,0,0) );
    }
    else
    {
        if( m_pkColor->GetColor() != m_pkMessage->GetFontColor() )
        {
            m_pkMessage->SetFontColor( m_pkColor->GetColor() );
        }
        m_pkBytes->SetFontColor( D3DCOLOR_ARGB(255,255,255,255) );
    }

    if( true == m_pkMessage->HasFocus() && true == m_bDefaultMsg )
    {
        SetEditText( false );
    }
    else if( m_pkMessage->HasFocus() == false && wcscmp( m_pkMessage->GetText(), L"" ) == 0 )
    {
        SetEditText( true );
    }
}

void KGCNeonsignMessage::OnCreateComplete()
{
    m_pkColor->SetColorNum( KD3DColorPalette::CASH_ITEM_COLOR_NUM + 1 );
    m_pkColor->CheckPaletteButtons( true );
    m_pkColor->EnablePaletteButtons( true );
}

DWORD KGCNeonsignMessage::GetMessageColor() const
{
    return m_pkColor->GetColor();
}

bool KGCNeonsignMessage::SetBytes()
{
    int nbytes = (int)GCUTIL_STR::GetStringBytes( m_pkMessage->GetText() );

    if( m_bDefaultMsg )
    {
        nbytes = 0;
    }

    m_pkBytes->SetText( g_pkStrLoader->GetReplacedString( STR_ID_BYTES, "ii", nbytes, MAX_MESSAGE_BYTES ) );

    if( MAX_MESSAGE_BYTES < nbytes )
    {
        return true;
    }
    return false;
}

void KGCNeonsignMessage::GetMessage( OUT std::wstring& strMessage )
{
    strMessage = m_pkMessage->GetText();
}

bool KGCNeonsignMessage::IsValidate()
{
    if( wcscmp( m_pkMessage->GetText(), L"" ) == 0 )
    {
        return false;
    }
    else if( m_pkMessage->GetText() == g_pkStrLoader->GetString( STR_ID_ENTER_MESSAGE ) )
    {
        return false;
    }
    return true;
}

void KGCNeonsignMessage::SetEditText( bool bDefault )
{
    m_bDefaultMsg = bDefault;
    if( m_bDefaultMsg )
    {
        m_pkMessage->SetText( g_pkStrLoader->GetString( STR_ID_ENTER_MESSAGE ).c_str() );
    }
    else
    {
        m_pkMessage->SetText( L"" );
    }
}