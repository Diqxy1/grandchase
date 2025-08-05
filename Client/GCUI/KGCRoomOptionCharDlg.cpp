#include "stdafx.h"
#include "GCCharIcon.h"
#include "KGCRoomOptionCharDlg.h"

IMPLEMENT_CLASSNAME( KGCRoomOptionCharDlg );
IMPLEMENT_WND_FACTORY( KGCRoomOptionCharDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomOptionCharDlg, "gc_roomoptionchardlg" );

KGCRoomOptionCharDlg::KGCRoomOptionCharDlg(void)
: m_pkCloseBtn( NULL )
, m_pkOkBtn( NULL )
, m_dwAllowedCharacters( 0 )
{
	LINK_CONTROL( "close_btn", m_pkCloseBtn );
	LINK_CONTROL( "ok_btn", m_pkOkBtn );

	for( int i=0 ; i<GC_CHAR_NUM ; ++i )
	{
		m_saAllowedCharacters[i] = NULL;

		char szTemp[128] = { 0, };
		sprintf_s( szTemp, 127, "char%d", i );
		LINK_CONTROL( szTemp, m_saAllowedCharacters[i] );
	}
}

KGCRoomOptionCharDlg::~KGCRoomOptionCharDlg(void)
{
}

void KGCRoomOptionCharDlg::ActionPerformed( const KActionEvent& kEvent )
{

}

bool KGCRoomOptionCharDlg::CheckCondition( IN const KDialogInfo& kInfo_ )
{
	m_dwAllowedCharacters = static_cast<DWORD>(kInfo_.m_lParam);
	return true;
}

void KGCRoomOptionCharDlg::OnCreate()
{
	if( m_pkOkBtn )
	{
		m_pkOkBtn->InitState( true, true, this );
		m_pkOkBtn->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCRoomOptionCharDlg::OnClickBtn );
	}

	if( m_pkCloseBtn )
	{
		m_pkCloseBtn->InitState( true, true, this );
		m_pkCloseBtn->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCRoomOptionCharDlg::OnClickBtn );
	}

	for( int i = 0; i < GC_CHAR_NUM; i++ )
	{
		if( m_saAllowedCharacters[i] )
		{
			m_saAllowedCharacters[i]->InitState( true, true, this );
			//m_saAllowedCharacters[i]->SetLockIcon(true);
			//m_saAllowedCharacters[i]->Lock( true );
		}
	}
}

void KGCRoomOptionCharDlg::OnCreateComplete()
{
	UpdateCharSlot();
}

void KGCRoomOptionCharDlg::OnClickBtn()
{
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

const int PanelWidthSize = 289;
const int PanelIconGap = 3;
const float PanelStartPosX = 18.0f;
const float PanelStartPosY = 48.0f;

void KGCRoomOptionCharDlg::UpdateCharSlot()
{
	DWORD dwIconStandardWidth = m_saAllowedCharacters[0]->GetWidth();
	DWORD dwIconStandardHeight = m_saAllowedCharacters[0]->GetHeight();
	D3DXVECTOR2 vStartPos( PanelStartPosX, PanelStartPosY );

	int nColumn = (PanelWidthSize + PanelIconGap) / (dwIconStandardWidth + PanelIconGap);
	int cntColumn = 0;
	int cntRow = 0;
	int nIndex = 0;

	for( int i = 0; i < GC_CHAR_NUM; ++i ) 
	{
		DWORD dwCharExponential = 1 << i;
		cntColumn = nIndex % nColumn;
		cntRow = nIndex / nColumn;

		float fPosX = vStartPos.x + (cntColumn * (dwIconStandardWidth + PanelIconGap));
		float fPosY = vStartPos.y + (cntRow * (dwIconStandardHeight + PanelIconGap));

		m_saAllowedCharacters[i]->SetWindowPosDirect(D3DXVECTOR2(fPosX,fPosY));
		m_saAllowedCharacters[i]->ToggleRender( true );
		
		nIndex++;
		if( (m_dwAllowedCharacters & dwCharExponential ) == dwCharExponential )
		{
			m_saAllowedCharacters[i]->SetLockIcon( true );
		}
	}
}