#include "stdafx.h"
#include "GCAttendanceStampDlg.h"
#include "GCAttendanceStampManager.h"
#include "KGCStampTitle.h"

IMPLEMENT_CLASSNAME( KGCAttendanceStampDlg );
IMPLEMENT_WND_FACTORY( KGCAttendanceStampDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCAttendanceStampDlg, "gc_attendance_stamp" );

KGCAttendanceStampDlg::KGCAttendanceStampDlg( void )
{
    m_StampTitle = NULL;
    LINK_CONTROL ("title", m_StampTitle );
    m_pkBtnClose = NULL;
    LINK_CONTROL( "btn_close", m_pkBtnClose );

    m_pkSlot = NULL;
    LINK_CONTROL( "stamp", m_pkSlot );

    for( int i = 0 ; i < (int)m_apStampSlot.size() ; ++i )
    {
        m_apStampSlot[i] = NULL;
    }

    m_pkBackOuter = NULL;
    LINK_CONTROL( "background0", m_pkBackOuter );

    m_pkBackInner = NULL;
    LINK_CONTROL( "background1", m_pkBackInner );

    m_dwOriginalOuterHeight = 0;
    m_dwOriginalInnerHeight = 0;
    m_dwSlotHeight = 0;
}

KGCAttendanceStampDlg::~KGCAttendanceStampDlg( void )
{
}

void KGCAttendanceStampDlg::OnCreate( void  )
{
    m_StampTitle->InitState(false);
    m_pkBackOuter->InitState( true );
    m_pkBackInner->InitState( true );

    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->SetHotKey (DIK_ESCAPE );

    m_pkSlot->InitState( false );
    float fStartPosX = m_pkSlot->GetFixedWindowLocalPos().x;
    float fStartPosY = m_pkSlot->GetFixedWindowLocalPos().y;

    int nEventSize = SiGCAttendanceStampManager()->GetDayCount();
    if( nEventSize > MAX_STAMP_SLOT )
        nEventSize = MAX_STAMP_SLOT;

    int nRow = nEventSize / SLOT_PER_ROW;
    if( (nEventSize % SLOT_PER_ROW) != 0 )
        nRow += 1;

    std::vector<KD3DWnd*> vecSlotList = g_pkUIMgr->CreateTemplateUIListByFindName( this, "ui_attendance_stamp.stg", "", "attendance_stamp_main\\stamp", true, nRow * SLOT_PER_ROW );
    if (!vecSlotList.empty())
    {
        std::vector<KD3DWnd*>::iterator iterSlot = vecSlotList.begin();
        DWORD dwWidth = (*iterSlot)->GetWidth();
        DWORD dwHeight = (*iterSlot)->GetHeight();
        for ( int i = 0; iterSlot != vecSlotList.end(); ++iterSlot, ++i )
        {
            m_apStampSlot[i] = static_cast<KGCAttendanceStampSlot*>(*iterSlot);
            m_apStampSlot[i]->InitState( true, true, this );

            D3DXVECTOR2 vPos;
            vPos.x = fStartPosX + static_cast<float>( (i%SLOT_PER_ROW)*(dwWidth+SLOT_GAP) );
            vPos.y = fStartPosY + static_cast<float>( (i/SLOT_PER_ROW)*(dwHeight+SLOT_GAP) );
            m_apStampSlot[i]->SetFixedWindowLocalPos(vPos);
            m_apStampSlot[i]->SetSlotInfo( i + 1 );
        }
    }

    if( m_dwOriginalOuterHeight == 0 )
        m_dwOriginalOuterHeight = m_pkBackOuter->GetHeight();

    if( m_dwOriginalInnerHeight == 0 )
        m_dwOriginalInnerHeight = m_pkBackInner->GetHeight();

    if( m_dwSlotHeight == 0 )
        m_dwSlotHeight = m_pkSlot->GetHeight() + SLOT_GAP;

    UIResizing( nRow );
}

void KGCAttendanceStampDlg::OnCreateComplete()
{
    UpdateSlotInfo();
    UpdateStampTitle();
}

void KGCAttendanceStampDlg::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickClose );
}

void KGCAttendanceStampDlg::FrameMoveInEnabledState( void )
{
}

void KGCAttendanceStampDlg::OnClickClose()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCAttendanceStampDlg::UpdateSlotInfo()
{
    int nSlotUsed = SiGCAttendanceStampManager()->GetDayCount();
    if( nSlotUsed > MAX_STAMP_SLOT )
        nSlotUsed = MAX_STAMP_SLOT;

    for ( int i = 0; i < nSlotUsed; ++i )
    {
        if( m_apStampSlot[i] != NULL )
            m_apStampSlot[i]->SetSlotInfo( i + 1 );
    }
}

void KGCAttendanceStampDlg::UIResizing( IN int nRow )
{
    DWORD dwNewOuterHeight = m_dwOriginalOuterHeight + ( m_dwSlotHeight * ( nRow - 3 ) );
    DWORD dwNewInnerHeight = m_dwOriginalInnerHeight + ( m_dwSlotHeight * ( nRow - 3 ) );
    m_pkBackOuter->SetHeight( dwNewOuterHeight );
    m_pkBackInner->SetHeight( dwNewInnerHeight );
}

void KGCAttendanceStampDlg::UpdateStampTitle( void )
{
    m_StampTitle->SetUiType( SiGCAttendanceStampManager()->GetUIType() );
    m_StampTitle->ToggleRender(true );
}