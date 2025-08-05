#include "stdafx.h"
#include "GCIntroMissionFrame.h"

IMPLEMENT_CLASSNAME( KGCIntroMissionFrame );
IMPLEMENT_WND_FACTORY( KGCIntroMissionFrame );
IMPLEMENT_WND_FACTORY_NAME( KGCIntroMissionFrame, "gc_intro_mission_frame" );

KGCIntroMissionFrame::KGCIntroMissionFrame( void )
{


	m_pkStaticDesc = NULL;
	LINK_CONTROL( "static_desc", m_pkStaticDesc );

	m_pkImgTypeCash = NULL;
	LINK_CONTROL( "type_cash", m_pkImgTypeCash );
	m_pkImgTypeEvent = NULL;
	LINK_CONTROL( "type_event", m_pkImgTypeEvent );

	m_pkTitle = NULL;
	LINK_CONTROL( "title", m_pkTitle );

}

KGCIntroMissionFrame::~KGCIntroMissionFrame( void )
{

	
}

void KGCIntroMissionFrame::ActionPerformed( const KActionEvent& event )
{


}

void KGCIntroMissionFrame::OnCreate( void )
{
	m_pkStaticDesc->InitState(true);

	m_pkTitle->InitState(true);
	m_pkImgTypeCash->InitState(false);
	m_pkImgTypeEvent->InitState(false);
}

void KGCIntroMissionFrame::SetMissionFrame( const int iCharType_ )
{

	switch( g_kGlobalValue.GetCharOpenType(iCharType_) )
	{
	case KGCSelectedCharFrame::OPEN_SALE:
	case KGCSelectedCharFrame::OPEN_CASH:
		m_pkTitle->SetWndMode(D3DWM_HOVER);
		m_pkImgTypeCash->ToggleRender(true);
		m_pkImgTypeEvent->ToggleRender(false);
		m_pkStaticDesc->SetTextAutoMultiline(g_pkStrLoader->GetString( STR_ID_CHAR_CASH_DESC ));
		break;
	case KGCSelectedCharFrame::OPEN_EVENT:
    case KGCSelectedCharFrame::OPEN_SEALED:
		m_pkTitle->SetWndMode(D3DWM_DEFAULT);
		m_pkImgTypeCash->ToggleRender(false);
		m_pkImgTypeEvent->ToggleRender(true);
		m_pkStaticDesc->SetTextAutoMultiline(g_pkStrLoader->GetString( STR_ID_CHAR_EVENT_DESC ));
		break;
	}

}


void KGCIntroMissionFrame::PostChildDraw()
{

}
