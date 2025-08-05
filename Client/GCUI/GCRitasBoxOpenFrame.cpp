#include "stdafx.h"
#include "GCRitasBoxOpenFrame.h"

IMPLEMENT_CLASSNAME( KGCRitasBoxOpenFrame );
IMPLEMENT_WND_FACTORY( KGCRitasBoxOpenFrame );
IMPLEMENT_WND_FACTORY_NAME( KGCRitasBoxOpenFrame, "gc_ritasboxopenframe" );

KGCRitasBoxOpenFrame::KGCRitasBoxOpenFrame( void ):
m_pkBoxImg(NULL),
m_pkStaticExplain(NULL)
{
	LINK_CONTROL("box_img",m_pkBoxImg);
	LINK_CONTROL("static_explain",m_pkStaticExplain);

}

KGCRitasBoxOpenFrame::~KGCRitasBoxOpenFrame( void )
{
}

void KGCRitasBoxOpenFrame::OnCreate( void )
{
	m_pkBoxImg->InitState(true);
	m_pkStaticExplain->InitState(true);
	m_pkStaticExplain->SetAlign(DT_CENTER);
	m_pkStaticExplain->SetTextAutoMultiline(g_pkStrLoader->GetString( STR_ID_RITASCHRISTMAS_OPENEXPLAIN ).c_str());
}

void KGCRitasBoxOpenFrame::ActionPerformed( const KActionEvent& event )
{

}

void KGCRitasBoxOpenFrame::SetFrameType(int iType_)
{
	switch(iType_)
	{
	case KGCRitasResultDlg::DLG_STATE_SPECIALMBOX:
		m_pkBoxImg->SetWndMode(D3DWM_DEFAULT);
		break;
	case KGCRitasResultDlg::DLG_STATE_PRIMIUMBOX:
		m_pkBoxImg->SetWndMode(D3DWM_HOVER);
		break;
	}
}
