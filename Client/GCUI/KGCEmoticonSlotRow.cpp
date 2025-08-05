#include "stdafx.h"
#include "KGCEmoticonSlotRow.h"
#include "KGCEmoticonElement.h"
#include "../GCItemManager.h"
//
#include "KGCEmoticon.h"
//

IMPLEMENT_CLASSNAME( KGCEmoticonSlotRow );
IMPLEMENT_WND_FACTORY( KGCEmoticonSlotRow );
IMPLEMENT_WND_FACTORY_NAME( KGCEmoticonSlotRow, "gc_emoticon_window_row" );


KGCEmoticonSlotRow::KGCEmoticonSlotRow( void )
: m_iRowIndex( 0 )
, m_iClickIndex( 0 )
, m_iOverClickIndex( 0 )
, m_bIsClick( false )
, m_bClickable( true )
{
    char str[256] = "";
    for ( int i = 0; i < EMOTICON_SLOT_MAX; ++i )
    {
        m_pkEmoticonSlot[i] = NULL;
        sprintf( str, "slot%d", i+1 );
        LINK_CONTROL( str, m_pkEmoticonSlot[i] ) ;
    }
}

KGCEmoticonSlotRow::~KGCEmoticonSlotRow( void )
{
}

void KGCEmoticonSlotRow::OnCreateComplete( void )
{
    UpdateSlotInfo();
}

void KGCEmoticonSlotRow::ActionPerformed( const KActionEvent& event )
{
    for ( int i = 0; i < EMOTICON_SLOT_MAX; ++i )
    {
        if ( event.m_pWnd == m_pkEmoticonSlot[i] && m_pkEmoticonSlot[i]->IsExistEmoticon() )
        {
            if ( event.m_dwCode == D3DWE_BUTTON_CLICK )
            {
                if ( false == m_pkEmoticonSlot[i]->GetIsClick() && m_bClickable )
                {
                    m_pkEmoticonSlot[i]->SetIsClick( true );
                    m_bIsClick = true;
                    m_iClickIndex = i;
                }
                else if ( false == m_pkEmoticonSlot[i]->GetIsClick() && !m_bClickable )
                {
                    // 3개 다 채워놓고 찍은 놈들!
                    m_pkEmoticonSlot[i]->SetIsClick( true );
                    m_bIsClick = true;
                    m_iOverClickIndex = i;
                }
            }
        }
    }
}

void KGCEmoticonSlotRow::SetRowIndex( int index )
{
    m_iRowIndex = index;
}

KGCEmoticon* KGCEmoticonSlotRow::GetEmoticonData()
{
    return m_pkEmoticonSlot[m_iClickIndex]->GetEmoticonData();
}

void KGCEmoticonSlotRow::TurnOffSlotEdge( int iColumn )
{
    if ( -1 == iColumn )
        return;

    m_pkEmoticonSlot[iColumn]->SetIsClick( false );
}
void KGCEmoticonSlotRow::TurnOnSlotEdge( int iColumn )
{
    if ( -1 == iColumn )
        return;

    m_pkEmoticonSlot[iColumn]->SetIsClick( true );
}

void KGCEmoticonSlotRow::UpdateSlotInfo( void )
{
    for ( int i = 0; i < EMOTICON_SLOT_MAX; ++i )
    {
        int iTmpItemId = SiKGCEmoticonManager()->GetEmoticonDataUsable( (i + m_iRowIndex * 8) );
        if ( - 1 != iTmpItemId )
        {
            GCItem* pTmpItem = g_pItemMgr->GetItemData( iTmpItemId );

            m_pkEmoticonSlot[i]->InitState( true, true, this );
            m_pkEmoticonSlot[i]->SetItemID( iTmpItemId );

            int iTmpItemNum = g_pItemMgr->GetVolumeInventoryItem( iTmpItemId );
            if ( iTmpItemNum > 0 )
            {
                m_pkEmoticonSlot[i]->SetToolTip( pTmpItem->strItemDesc );
                m_pkEmoticonSlot[i]->SetVolumn( iTmpItemNum );
            }
            else
            {
                m_pkEmoticonSlot[i]->SetVolumn( 0 );
                m_pkEmoticonSlot[i]->SetToolTip( L"" );
                m_pkEmoticonSlot[i]->SetIsClick( false );
                m_bIsClick = false;
            }
        }
        else
        {
            m_pkEmoticonSlot[i]->InitState( true );
            m_pkEmoticonSlot[i]->SetVolumn( 0 );
            m_pkEmoticonSlot[i]->SetIsExistEmoticon( false );
        }
    }
}

bool KGCEmoticonSlotRow::GetIsRowClick( void ) const
{
    return m_bIsClick;
}

KGCEmoticon* KGCEmoticonSlotRow::GetOverEmoticonData()
{
    return m_pkEmoticonSlot[m_iOverClickIndex]->GetEmoticonData();
}

int KGCEmoticonSlotRow::FindElement( int iItemID )
{
    for( int i = 0; i < 8; ++i )
    {
        if( m_pkEmoticonSlot[i]->GetItemId() == iItemID )
        {
            return i;
        }
    }
    return -1;
}
