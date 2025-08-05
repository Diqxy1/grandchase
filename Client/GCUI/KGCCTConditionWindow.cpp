#include "stdafx.h"
#include ".\KGCCTConditionWindow.h"
//
//
//
#include "../gcui/GCSubMissionBar.h"
//

#include "mission/GCCTMissionManager.h"

IMPLEMENT_CLASSNAME( KGCCTConditionWindow );
IMPLEMENT_WND_FACTORY( KGCCTConditionWindow );
IMPLEMENT_WND_FACTORY_NAME( KGCCTConditionWindow, "gc_condition_window" );

KGCCTConditionWindow::KGCCTConditionWindow(void)
{
    char str[256] = "";
    for ( int i = 0; i < NUM_OF_LIST; ++i )
    {
        m_pkConditionList[i] = NULL;

        sprintf( str, "condition_bar%d", i+1 );
        LINK_CONTROL( str,               m_pkConditionList[i] );
    }

    m_pkTitle       = NULL;
    m_pkOk          = NULL;
    m_pkClose       = NULL;
    m_pkScrollBar   = NULL;
    m_pkBack        = NULL;

    LINK_CONTROL( "title",               m_pkTitle );
    LINK_CONTROL( "btn_ok",              m_pkOk );
    LINK_CONTROL( "close_button",        m_pkClose );
    LINK_CONTROL( "replay_box_scroll",   m_pkScrollBar );
    LINK_CONTROL( "back",                m_pkBack );    

	m_iMissionID	= -1;
	m_iTopIndex		= 0;
}

KGCCTConditionWindow::~KGCCTConditionWindow(void)
{
}

void KGCCTConditionWindow::OnCreate( void )
{
    char str[256] = "";
    for ( int i = 0; i < NUM_OF_LIST; ++i )
    {
        m_pkConditionList[i]->InitState( true, false, this );
    }

    m_pkTitle->InitState( true, true, this );
    m_pkOk->InitState( true, true, this );
    m_pkClose->InitState( true, true, this );
    m_pkScrollBar->InitState( true, true, this );
    m_pkBack->InitState( true, true, this );

    m_pkClose->SetHotKey( DIK_RETURN );
    m_pkClose->SetHotKey( DIK_ESCAPE );
}

void KGCCTConditionWindow::ActionPerformed( const KActionEvent& event )
{
    if( KD3DWnd::D3DWE_BUTTON_CLICK == event.m_dwCode )
    {
        if( event.m_pWnd == m_pkClose
            || event.m_pWnd == m_pkOk )
        {
            SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
        }
	}

	if( m_pkScrollBar == event.m_pWnd )
	{
		int iTop = m_iTopIndex;
		switch( event.m_dwCode )
		{
		case 0:
		case D3DWE_SCROLLBAR_DOWNBTN:
		case D3DWE_SCROLLBAR_UPBTN:
		case D3DWE_SCROLLBAR_DRAG:
			m_iTopIndex = m_pkScrollBar->GetScrollPos();
			break;
		}

		if( iTop != m_iTopIndex )
		{
			if( -1 != m_iMissionID )
			{
				SetSubMissionList(m_iMissionID);
			}
		}
	}
}

int KGCCTConditionWindow::GetFirstConditionIndex(void)
{
    return (m_pkScrollBar->GetScrollPos());
}

int KGCCTConditionWindow::GetSlotIndexByIndex(int iIndex)
{
    const int ciFirst_SIndex   = GetFirstConditionIndex();
    const int ciEnd_SIndex     = ciFirst_SIndex + (NUM_OF_LIST);
    
    if( iIndex < ciFirst_SIndex || iIndex >= ciEnd_SIndex )
        return -1;

    return iIndex - GetFirstConditionIndex();
}

void KGCCTConditionWindow::SetMission( int iMissionID )
{
	m_iMissionID = iMissionID;

    SetSubMissionList(iMissionID);
}

void KGCCTConditionWindow::RefreshAll(void)
{
    m_pkScrollBar->SetScrollPos(0);
    m_pkScrollBar->SetScrollRangeMax(0);

	for(int i=0;i<NUM_OF_LIST;++i)
		m_pkConditionList[i]->Reset();
}

void KGCCTConditionWindow::SetSubMissionList(int iMissionID)
{
	const SMissionInfo* sInfo = SiKGCCTMissionManager()->GetMissionInfo( iMissionID );

	if( NULL == sInfo )
		return;

	m_pkTitle->SetText( SiKGCCTMissionManager()->GetString( sInfo->m_iTitleID ) );

	for(int i=0;i<NUM_OF_LIST;++i)
		m_pkConditionList[i]->Reset();

	int iSlotCnt = 0;
	for( int i = GetFirstConditionIndex() ; i < (int)sInfo->m_vecSubMissions.size() ; ++i, ++iSlotCnt )
	{
		if( m_pkConditionList[iSlotCnt] == NULL )
			continue;

		if( (GetFirstConditionIndex() + NUM_OF_LIST) <= i  )
			break;

		const SSubMissionInfo* sSubInfo = SiKGCCTMissionManager()->GetSubMission( sInfo->m_vecSubMissions[i] );
		if( sSubInfo == NULL )
		{
			ASSERT_MBOX( "There is no CollectionMission Infomation! Please Patch CollectionMission Script \n MissionID : " << iMissionID <<
				"\nSubMissionID : "<<sInfo->m_vecSubMissions[i] );
			break;
		}
		//미션의 진행 상황을 보여줍니다.
		m_pkConditionList[iSlotCnt]->SetSubMissionID( sInfo->m_vecSubMissions[i] );
		m_pkConditionList[iSlotCnt]->SetTitleString( SiKGCCTMissionManager()->GetString( sSubInfo->m_iTitleID ) );
		m_pkConditionList[iSlotCnt]->SetCount(g_kGlobalValue.m_kUserCollectionMission.GetCompletionCount( iMissionID, sInfo->m_vecSubMissions[i] ), sSubInfo->m_sCompletion.m_iCount );
	}

	m_pkScrollBar->SetScrollPageSize( NUM_OF_LIST );
	m_pkScrollBar->SetScrollRangeMin( 0 );
	m_pkScrollBar->SetScrollRangeMax( sInfo->m_vecSubMissions.size() );
}
