#include "stdafx.h"
#include "GCChatBoxDlgS6.h"

IMPLEMENT_CLASSNAME( KGCChatBoxDlgS6 );
IMPLEMENT_WND_FACTORY( KGCChatBoxDlgS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCChatBoxDlgS6, "gc_chat_dlg_s6" );

KGCChatBoxDlgS6::KGCChatBoxDlgS6( void )
{
	
}

KGCChatBoxDlgS6::~KGCChatBoxDlgS6( void )
{
}

void KGCChatBoxDlgS6::ActionPerformed( const KActionEvent& event )
{

}

void KGCChatBoxDlgS6::OnCreate( void )
{
	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_CHAT_CHANGE_SIZE_BTN_CLICK);
	g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_CHAT_CHANGE_SIZE_BTN_CLICK, this, &KGCChatBoxDlgS6::OnClickChangeSize );

}


void KGCChatBoxDlgS6::OnClickChangeSize( const KActionEvent& event )
{
	const int ciTotalPosY = 570;
	const int ciTotalHeghit = 160;

	int iPosY = 0;
	int iHeghit = 0;

	switch (event.m_dwlParam) {
		case KGCChatBox::CTSIZE_MINIMUM :
			iHeghit = -130;
			iPosY = 130;
			break;
		case KGCChatBox::CTSIZE_NOMAL :
			break;
		case KGCChatBox::CTSIZE_MAXIMUM :
			iHeghit = 130;
			iPosY = -130;
			break;
		default:
			break;
	}

	SetHeightDirect( ciTotalHeghit + iHeghit );
	SetWindowPosYDirect( static_cast<float>(ciTotalPosY + iPosY) );
}
//
//void KGCChatBoxDlgS6::FrameMoveInEnabledState()
//{
//	//if (  g_pkWorldmapChatBox->GetCurrentSize() == KGCChatBox::CTSIZE_MINIMUM && IsRenderOn() ) {
//	//	if (  g_pkInput->IsUp( DIK_A )  )
//	//		g_pkWorldmapChatBox->ChangeChatSize(KGCChatBox::CTSIZE_NOMAL);
//	//}
//}

void KGCChatBoxDlgS6::RenderOn()
{
	ToggleRender(true);
	g_pkWorldmapChatBox->ToggleRender(true);
}

void KGCChatBoxDlgS6::RenderOff()
{
	ToggleRender(false);
	g_pkWorldmapChatBox->ToggleRender(false);
}
