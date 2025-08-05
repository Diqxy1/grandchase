#include "stdafx.h"
#include "GCNotifyMessage.h"

#include "KGCAlertWnd.h"

IMPLEMENT_CLASSNAME( KGCNotifyMessage );
IMPLEMENT_WND_FACTORY( KGCNotifyMessage );
IMPLEMENT_WND_FACTORY_NAME( KGCNotifyMessage, "gc_notifymessage" );

KGCNotifyMessage::KGCNotifyMessage( void )
{
    m_pkNewSkillAdd = NULL;
    m_pkNewSkillAddMsg = NULL;

    LINK_CONTROL( "new_skill_add",				m_pkNewSkillAdd );
    LINK_CONTROL( "new_skill_add_msg",			m_pkNewSkillAddMsg );

	m_iToggleFrame = ALERT_TOGGLE_FRAME;
}

KGCNotifyMessage::~KGCNotifyMessage( void )
{
}

void KGCNotifyMessage::OnCreate()
{
    if( m_pkNewSkillAdd)
        m_pkNewSkillAdd->InitState(false , true, this);

    if( m_pkNewSkillAddMsg )
    {
        m_pkNewSkillAddMsg->InitState(false, true, this );
        m_pkNewSkillAddMsg->SetFontColor( 0xff000000 );
        m_pkNewSkillAddMsg->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_NEW_SKILL_ADD) );
    }
}

void KGCNotifyMessage::ShowNewSkillAddMessage(bool bShow)
{
    if( m_pkNewSkillAdd )
        m_pkNewSkillAdd->ToggleRender( bShow );

    if( m_pkNewSkillAddMsg )
        m_pkNewSkillAddMsg->ToggleRender( bShow );
}

void KGCNotifyMessage::SetMsgColor(BYTE byteAlpha)
{
    D3DCOLOR dwFontColor = D3DCOLOR_ARGB( byteAlpha, 0x00, 0x00, 0x00 );
    D3DCOLOR dwBackgroundColor = D3DCOLOR_ARGB( byteAlpha, 0xFF, 0xFF, 0xFF );

    if( m_pkNewSkillAddMsg )
        m_pkNewSkillAddMsg->SetFontColor( dwFontColor );

    if( m_pkNewSkillAdd )
        m_pkNewSkillAdd->SetAllWindowColor( dwBackgroundColor );

}

void KGCNotifyMessage::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkNewSkillAdd || event.m_pWnd == m_pkNewSkillAddMsg)
    {
        SiGCSKT()->SetNewSkillMsgRender( false );
        ShowNewSkillAddMessage( false );
    }
}

void KGCNotifyMessage::FrameMoveInEnabledState( void )
{

#ifdef SKILL_TREE
    //  보여지는 곳이 아니면 가린다.
    if ( IsRenderState() )
    {
        ShowNewSkillAddMessage( SiGCSKT()->GetNewSkillNum() > 0  && SiGCSKT()->IsNewSkillMsgRender() );
    }
    else
    {
        ShowNewSkillAddMessage( false );
    }
#endif

    int iOpacity = ALERT_TOGGLE_FRAME;
    switch( m_iToggleFrame / ALERT_TOGGLE_FRAME )
    {
    case 0: // 0 ~ ( BUBBLE_MSG_TOGGLE_FRAME - 1 )
        iOpacity = m_iToggleFrame;
        break;
    case 1: // ( BUBBLE_MSG_TOGGLE_FRAME ) ~ ( BUBBLE_MSG_TOGGLE_FRAME * 2 - 1 )
        iOpacity = ALERT_TOGGLE_FRAME;
        break;
    case 2: // ( BUBBLE_MSG_TOGGLE_FRAME * 2 ) ~ ( BUBBLE_MSG_TOGGLE_FRAME * 3 - 1 )
        iOpacity = ALERT_TOGGLE_FRAME * 3 - m_iToggleFrame;
        break;
    }
    BYTE alpha = static_cast<BYTE>( 255.0f * static_cast<float>( iOpacity ) / static_cast<float>( ALERT_TOGGLE_FRAME ) );

    SetMsgColor( alpha ); 

    m_iToggleFrame = ( m_iToggleFrame + 1 ) % ( ALERT_TOGGLE_FRAME * 3 );
}

bool KGCNotifyMessage::IsRenderState()
{
    GAME_STATE emState = g_MyD3D->m_pStateMachine->GetState();

    switch( emState )
    {
    case GS_SHOP_CASH:
    case GS_SHOP_GP:
    case GS_ROOM:
    case GS_MY_INFO:
    case GS_MY_INFO_FROM_ROOM:
    case GS_WORLDMAP:
    case GS_MATCH:
#if defined USE_COORDI_SHOP
    case GS_COORDI_SHOP:
#endif
        return true;
    default:
        return false;
    }

    return false;
}