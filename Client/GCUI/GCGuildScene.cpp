#include "stdafx.h"
#include ".\gcguildscene.h"
//

//

#include "gcui/KGCWorldMapContainer.h"
#include "GCUI/GCGuildUserList.h"
#include "GCUI/GCChatBox.h"
#include "GCUI/GCGuildInfo.h"
#include "GCUI/GCGuildMemo.h"
#include "../BuddySystem/KFriendShip.h"


IMPLEMENT_CLASSNAME( KGCGuildScene );
IMPLEMENT_WND_FACTORY( KGCGuildScene );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildScene, "gc_guild_scene" );

KGCGuildScene::KGCGuildScene(void)
{
	LINK_CONTROL("guild_info" ,		m_pkGuildInfo );
	LINK_CONTROL("user_list" ,		m_pkUserList );
	LINK_CONTROL("guild_chat_box" , m_pkChatBox );
	LINK_CONTROL("guild_notify",	m_pkGuildMemo);
}

KGCGuildScene::~KGCGuildScene(void)
{
}

void KGCGuildScene::OnCreate()
{
	KGCScene::OnCreate();

	m_pkGuildInfo->SetSelfInputCheck(true);
	m_pkUserList->SetSelfInputCheck(true);
	m_pkChatBox->SetSelfInputCheck(true);
	m_pkGuildMemo->SetSelfInputCheck(true);

	m_pkGuildInfo->AddActionListener(this);
	m_pkUserList->AddActionListener(this);
	m_pkChatBox->AddActionListener(this);
	m_pkGuildMemo->AddActionListener(this);

}
void KGCGuildScene::ActionPerformed( const KActionEvent& event )
{
	KGCScene::ActionPerformed( event );
}

void KGCGuildScene::FrameMoveInEnabledState( void )
{	
	KGCScene::FrameMoveInEnabledState();
	SiKFriendShip()->CheckAlramAddFriend();
}

void KGCGuildScene::OnEnterScene()
{
    g_pkUIScene->m_pkWorldMapContainer->ToggleRender( false );
    g_pkUIScene->EnableMessageBox( true );
}