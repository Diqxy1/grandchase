#include "stdafx.h"
#include "GCItemManager.h"

#include "GCInventoryFullMsg.h"
//
//
//

IMPLEMENT_CLASSNAME( KGCInventoryFullMsg );
IMPLEMENT_WND_FACTORY( KGCInventoryFullMsg );
IMPLEMENT_WND_FACTORY_NAME( KGCInventoryFullMsg, "gc_inventory_full_msg" );

KGCInventoryFullMsg::KGCInventoryFullMsg(void)
: m_pkBackground( NULL )
, m_pkMsg( NULL )
, m_iToggleFrame( ALERT_TOGGLE_FRAME_MAX )
, m_iToggleCount( 0 )
, m_eMsg_Type( INVEN_FULL )
, m_iVal1( 0 )
, m_iVal2( 0 )
{
    LINK_CONTROL( "background", m_pkBackground );
    LINK_CONTROL( "msg", m_pkMsg );
}

KGCInventoryFullMsg::~KGCInventoryFullMsg(void)
{
}

void KGCInventoryFullMsg::OnCreate( void )
{
    m_pkMsg->SetAlign( DT_CENTER );
}

void KGCInventoryFullMsg::FrameMoveInEnabledState()
{
    if( m_eMsg_Type == EVNET_CREATE_INFO )
    {
        SetMsgColor( 200 );
        return;
    }

    int iOpacity = ALERT_TOGGLE_FRAME_MAX;
    switch( m_iToggleFrame / ALERT_TOGGLE_FRAME_MAX )
    {
    case 0: // 0 ~ ( BUBBLE_MSG_TOGGLE_FRAME - 1 )
        iOpacity = m_iToggleFrame;
        break;
    case 1: // ( BUBBLE_MSG_TOGGLE_FRAME ) ~ ( BUBBLE_MSG_TOGGLE_FRAME * 2 - 1 )
        iOpacity = ALERT_TOGGLE_FRAME_MAX;
        break;
    case 2: // ( BUBBLE_MSG_TOGGLE_FRAME * 2 ) ~ ( BUBBLE_MSG_TOGGLE_FRAME * 3 - 1 )
        iOpacity = ALERT_TOGGLE_FRAME_MAX * 3 - m_iToggleFrame;
        break;
    }

    BYTE alpha = static_cast<BYTE>( 255.0f * static_cast<float>( iOpacity ) / static_cast<float>( ALERT_TOGGLE_FRAME_MAX ) );

    SetMsgColor( alpha ); 

    m_iToggleFrame = ( m_iToggleFrame + 1 ) % ( ALERT_TOGGLE_FRAME_MAX * 3 );

    if ( m_iToggleFrame == 0 ) 
    {
        m_iToggleCount++;
        
        if ( m_iToggleCount > ALERT_TOGGLE_COUNT_MAX )
        {
            m_iToggleCount = 0;
            ToggleRender( false );
        }

    }
}

void KGCInventoryFullMsg::ActionPerformed( const KActionEvent& event )
{

}

void KGCInventoryFullMsg::UpdateMsg()
{
    bool bRender = true;

    switch ( m_eMsg_Type )
    {
    case INVEN_FULL:
        {
            int iInventoryCapacity = g_kGlobalValue.m_iInvenCapacity;
            int iUserItemSize = g_pItemMgr->GetTotalInventorySize();

            if( iInventoryCapacity <= iUserItemSize )
                m_pkMsg->SetText( g_pkStrLoader->GetString( STR_ID_INVENTORY_FULL_ERROR2) );
            else if( iInventoryCapacity <= iUserItemSize + INVENTORY_FULL_WARNING_COUNT )
                m_pkMsg->SetText( g_pkStrLoader->GetReplacedString( STR_ID_INVENTORY_FULL_ERROR3, "i", INVENTORY_FULL_WARNING_COUNT ) );
            else
                bRender = false;
        }
        break;
    case CALENDAR_INFO:
        {
            // m_iVal1 은 플레이 타임으로 사용한다.
            // m_iVal2 는 오늘 출석부를 찍었는지 여부에 사용한다.
            //         0 이면 아직 출석부를 찍지 않았다. 
            //         1 이면 오늘 출석부를 찍었다.
            if ( m_iVal1 < g_kGlobalValue.m_iAttendTime )
                m_pkMsg->SetTextAutoMultiline( g_pkStrLoader->GetReplacedString( STR_ID_ATTENDANCE_RECOMMAND0, "ii", m_iVal1, g_kGlobalValue.m_iAttendTime - m_iVal1));
            else
            {
                if ( m_iVal2 == 0 )
                    m_pkMsg->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_ATTENDANCE_RECOMMAND3 ) );
                else
                    bRender = false;
            }
        }
        break;
    case EVNET_CREATE_INFO:
        // m_iVal1 == 1 이면 첫 접속시 알려주는 메시지
        // m_iVal1 == 2 이면 제작 가능할 때 알려주는 메시지 
        if( m_iVal1 == 1 )
        {
            if( g_kGlobalValue.m_bEventCreateFirst )
            {
                m_pkMsg->SetMultiLine( true );
                m_pkMsg->SetText( g_pkStrLoader->GetString(STR_ID_EVENT_CREATE_INFO) );
            }
        }
        else if( m_iVal1 == 2 )
        {
            if( g_kGlobalValue.m_bEventCreateEnable )
            {
                m_pkMsg->SetMultiLine( true );
                m_pkMsg->SetText( g_pkStrLoader->GetString(STR_ID_EVENT_CREATE_ENABLE ) );
            }
        }
        else    
            bRender = false;

        break;
    case NEW_SKILL_MSG :
        m_pkMsg->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_SKILLTREE_MSG ) );
        break;
    default:
        break;
    }

    m_pkBackground->ToggleRender( bRender );
    m_pkMsg->ToggleRender( bRender );
}

void KGCInventoryFullMsg::SetMsgColor(BYTE byteAlpha_)
{
    D3DCOLOR dwFontColor = D3DCOLOR_ARGB( byteAlpha_, 0x00, 0x00, 0x00 );
    D3DCOLOR dwBackgroundColor = D3DCOLOR_ARGB( byteAlpha_, 0xFF, 0xFF, 0xFF );

    m_pkMsg->SetFontColor( dwFontColor );
    m_pkBackground->SetAllWindowColor( dwBackgroundColor );
}
