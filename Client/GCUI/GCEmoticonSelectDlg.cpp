#include "stdafx.h"
#include ".\GCEmoticonSelectDlg.h"
//
#include "KGCEmoticonSlotRow.h"

//


IMPLEMENT_CLASSNAME( GCEmoticonSelectDlg );
IMPLEMENT_WND_FACTORY( GCEmoticonSelectDlg );
IMPLEMENT_WND_FACTORY_NAME( GCEmoticonSelectDlg, "gc_emoticon_select" );

GCEmoticonSelectDlg::GCEmoticonSelectDlg(void)
: m_pkClose(NULL)
, m_iRowIdx(-1)
, m_iColumnIdx(-1)
, m_pData(NULL)
, m_eUse(GCEmoticonSelectDlg::INIT)
, m_dwEmoticonID(KFriend::DEFAULT_EMOTICON_ID)
{
    LINK_CONTROL( "close_button", m_pkClose );

    char str[MAX_PATH] = "";
    for ( int i = 0; i < EMOTICON_ROW_MAX; ++i )
    {
        m_pkEmoticonRow[i] = NULL;
        sprintf( str, "emoticon_slot_row%d", i+1 );
        LINK_CONTROL( str, m_pkEmoticonRow[i] );
    }
}

GCEmoticonSelectDlg::~GCEmoticonSelectDlg(void)
{
}

void GCEmoticonSelectDlg::OnCreate()
{
    m_pkClose->InitState( true, true, this );
    m_pkClose->SetHotKey( DIK_ESCAPE );
    m_pkClose->SetHotKey( DIK_RETURN );

    for ( int i = 0; i < EMOTICON_ROW_MAX; ++i )
    {
        m_pkEmoticonRow[i]->InitState( true, true, this );
        m_pkEmoticonRow[i]->SetRowIndex( i );
    }
}

void GCEmoticonSelectDlg::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );

    for ( int i = 0; i < EMOTICON_ROW_MAX; ++i )
    {
        if ( event.m_pWnd == m_pkEmoticonRow[i] && m_pkEmoticonRow[i]->GetIsRowClick() && g_pkInput->BtnUp( KInput::MBLEFT ) )
        {
            if( m_iRowIdx != -1 && m_iColumnIdx != -1 && m_pData != NULL )
            {
                m_pkEmoticonRow[m_iRowIdx]->TurnOffSlotEdge( m_iColumnIdx );
                m_pkEmoticonRow[m_iRowIdx]->InitOverColumnIndex();
            }

            m_iRowIdx = i;
            m_iColumnIdx = m_pkEmoticonRow[i]->GetColumnIndex();
            m_pkEmoticonRow[i]->SetIsRowClick( false );
            m_pData = m_pkEmoticonRow[i]->GetEmoticonData();
            return;
        }
    }

    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if( m_iRowIdx != -1 && m_iColumnIdx != -1 && m_pData != NULL )
        {
            m_pkEmoticonRow[m_iRowIdx]->TurnOffSlotEdge( m_iColumnIdx );
            m_pkEmoticonRow[m_iRowIdx]->InitOverColumnIndex();
        }
        InitData();
    }
}

void GCEmoticonSelectDlg::OnClose()
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void GCEmoticonSelectDlg::OnDestroyComplete()
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );
    m_dwEmoticonID = KFriend::DEFAULT_EMOTICON_ID;
}

void GCEmoticonSelectDlg::InitData()
{
    m_iRowIdx = -1;
    m_iColumnIdx = -1;
    m_pData = NULL;
    m_dwEmoticonID = KFriend::DEFAULT_EMOTICON_ID;
}

void GCEmoticonSelectDlg::SetEmoticon( DWORD dwID )
{
    m_dwEmoticonID = dwID;
}

void GCEmoticonSelectDlg::OnChildCreateComplete()
{
    if( m_dwEmoticonID == KFriend::DEFAULT_EMOTICON_ID )
        return;

    for ( int i = 0; i < EMOTICON_ROW_MAX; ++i )
    {
        int iColumn = m_pkEmoticonRow[i]->FindElement( (int)m_dwEmoticonID );
        if( iColumn != -1 )
        {
            m_iRowIdx = i;
            m_iColumnIdx = iColumn;
            m_pkEmoticonRow[i]->SetColumnIndex( iColumn );
            m_pkEmoticonRow[i]->TurnOnSlotEdge( iColumn );
            m_pData = m_pkEmoticonRow[i]->GetEmoticonData();
            return;
        }
        else
        {
            m_pkEmoticonRow[i]->SetColumnIndex( -1 );
        }
    }
}

bool GCEmoticonSelectDlg::CheckCondition( IN const KDialogInfo& kInfo_ )
{
    switch( kInfo_.m_nMsgBoxUse )
    {
    case KGCUIScene::GC_MBOX_USE_NATEON_SELECT_EMOTICON:
        {
            this->SetUse( GCEmoticonSelectDlg::NATEON );
        }
        break;
    case KGCUIScene::GC_MBOX_USE_NATEON_INPUT_EMOTICON:
        {
            this->SetUse( GCEmoticonSelectDlg::INPUT );
        }
        break;
    }
    return true;
}

bool GCEmoticonSelectDlg::InitDialog( IN const KDialogInfo& kInfo_ )
{
    switch( kInfo_.m_nMsgBoxUse )
    {
    case KGCUIScene::GC_MBOX_USE_NATEON_SELECT_EMOTICON:
        {
            DWORD dwEmoticon = SiKFriendShip()->GetEmoticon();
            if( dwEmoticon != 0 )
            {
                this->SetEmoticon( dwEmoticon );
            }
        }
        break;
    case KGCUIScene::GC_MBOX_USE_NATEON_INPUT_EMOTICON:
        {
        }
        break;
    }
    return true;
}