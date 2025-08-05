#include "stdafx.h"
#include "KGCRoomOptionRankDlg.h"

IMPLEMENT_CLASSNAME( KGCRoomOptionRankDlg );
IMPLEMENT_WND_FACTORY( KGCRoomOptionRankDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomOptionRankDlg, "gc_roomoptionrankdlg" );

KGCRoomOptionRankDlg::KGCRoomOptionRankDlg(void)
: m_pkCloseBtn( NULL )
, m_pkOkBtn( NULL )
, m_dwAllowedRanks( 0xFFFFFFFF )
{
	LINK_CONTROL( "close_btn", m_pkCloseBtn );
	LINK_CONTROL( "ok_btn", m_pkOkBtn );

	for( int i = 0; i < GC_BG_GRADE_MAX; i++ )
	{
		m_saGrades[i] = NULL;
		LINK_CONTROL_STM("rank_icon" << i, m_saGrades[i]);
	}
}

KGCRoomOptionRankDlg::~KGCRoomOptionRankDlg(void)
{

}

void KGCRoomOptionRankDlg::ActionPerformed( const KActionEvent& kEvent )
{

}

bool KGCRoomOptionRankDlg::CheckCondition( IN const KDialogInfo& kInfo_ )
{
	m_dwAllowedRanks = static_cast<DWORD>(kInfo_.m_lParam);
	return true;
}

void KGCRoomOptionRankDlg::OnCreate()
{
	if( m_pkOkBtn )
	{
		m_pkOkBtn->InitState( true, true, this );
		m_pkOkBtn->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCRoomOptionRankDlg::OnClickBtn );
	}

	if( m_pkCloseBtn )
	{
		m_pkCloseBtn->InitState( true, true, this );
		m_pkCloseBtn->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCRoomOptionRankDlg::OnClickBtn );
	}

	for( int i = 0; i < GC_BG_GRADE_MAX; i++ )
	{
		if( m_saGrades[i] )
		{
			m_saGrades[i]->InitState( false, true );
		}
	}
}

void KGCRoomOptionRankDlg::OnCreateComplete()
{
	UpdateGradeSlot();
}

void KGCRoomOptionRankDlg::OnClickBtn()
{
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCRoomOptionRankDlg::UpdateGradeSlot()
{
	DWORD dwIconStandardWidth = m_saGrades[0]->GetWidth();
	DWORD dwIconStandardHeight = m_saGrades[0]->GetHeight();
	D3DXVECTOR2 vStartPos( 18.0f, 48.0f );

	int nColumn = (289 + 3) / (dwIconStandardWidth + 3);
	int cntColumn = 0;
	int cntRow = 0;
	int nIndex = 0;

	for( int i = 0; i < GC_BG_GRADE_MAX; ++i ) 
	{
		int iGradeIdx = (GC_BG_GRADE_SS - i);
		if( (m_dwAllowedRanks & (1 << iGradeIdx) ) == (1 << iGradeIdx) )
		{
			cntColumn = nIndex % nColumn;
			cntRow = nIndex / nColumn;

			float fPosX = vStartPos.x + (cntColumn * (dwIconStandardWidth + 3));
			float fPosY = vStartPos.y + (cntRow * (dwIconStandardHeight + 3));

			m_saGrades[iGradeIdx]->SetWindowPosDirect(D3DXVECTOR2(fPosX,fPosY));
			m_saGrades[iGradeIdx]->ToggleRender( true );

			nIndex++;
		}
		else
		{
			m_saGrades[iGradeIdx]->ToggleRender( false );
		}
	}
}