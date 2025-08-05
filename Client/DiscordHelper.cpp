#include "stdafx.h"
#ifndef DEV_BUILD
#define DISCORD_DISABLE_IO_THREAD
#include <discord_rpc.h>
#include <clocale>
#include <locale>
#include <codecvt>
#include "DiscordHelper.h"
#include "MyD3D.h"
#include "KGCStringLoader.h"
#include "KGCAgitGameManager.h"
#include "KGCRoomSceneS6.h"
#include "KGCWorldMapManager.h"
#include "GCUI/GCRoomTitleS6.h"
#include "GCUI/KGCNewTutorialOption.h"

const char* ClientID = "1160381328567578634";

static time_t StartTime;

static void HandleDiscordReady(const DiscordUser* connectedUser)
{
}

static void HandleDiscordDisconnected(int errcode, const char* message)
{
}

static void HandleDiscordError(int errcode, const char* message)
{
}

static void HandleDiscordJoin(const char* secret)
{
	// Not implemented
}

static void HandleDiscordSpectate(const char* secret)
{
	// Not implemented
}

static void HandleDiscordJoinRequest(const DiscordUser* request)
{
	// Not implemented
}

std::string DiscordHelper::to_string(const std::wstring& wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(wstr);
}

DiscordHelper::DiscordHelper()
{
	StartTime = time(0);
	DiscordEventHandlers handlers;
	ZeroMemory(&handlers, sizeof(DiscordEventHandlers));
	handlers.ready = HandleDiscordReady;
	handlers.disconnected = HandleDiscordDisconnected;
	handlers.errored = HandleDiscordError;
	handlers.joinGame = HandleDiscordJoin;
	handlers.spectateGame = HandleDiscordSpectate;
	handlers.joinRequest = HandleDiscordJoinRequest;
	Discord_Initialize(ClientID, &handlers, 1, nullptr);

	m_bStartedPlaying = false;
}

DiscordHelper::~DiscordHelper()
{
	Discord_Shutdown();
}

DiscordHelper* DiscordHelper::GetInstance()
{
	static DiscordHelper discordHelper;
	return &discordHelper;
}

void DiscordHelper::Refresh()
{
	Discord_RunCallbacks();
}

void DiscordHelper::UpdatePresence()
{
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	std::stringstream roomInfo;
	std::wstringstream imageText;
	std::string strImageText, strCharName, strLocationIcon, strLocation, actualState;

	switch (g_MyD3D->m_pStateMachine->GetState())
	{
		case GS_ROOM:
		case GS_GAME_END:
			{
				roomInfo.str("");
				roomInfo << KncUtil::toNarrowString(g_pkStrLoader->GetString(STR_ID_MESSENGER_USER_LOCATION10)) << " " << g_MyD3D->m_pStateMachine->GetRoomNumber() + 1 << "(" << KncUtil::toNarrowString(g_pkStrLoader->GetString(STR_ID_WAIT)) << ")";
				if (SiKGCAgitGameManager()->IsAgitEntered())
				{
					actualState = "";
				}
				else
				{
					actualState = roomInfo.str();
				}
				discordPresence.state = actualState.c_str();
				m_bStartedPlaying = false;
			}
			break;
		case GS_GAME:
			{
				if (m_bStartedPlaying == false)
				{
					m_tmStartTime = time(nullptr);
					m_bStartedPlaying = true;
				}
				if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TUTORIAL)
				{
					if (!SiKGCAgitGameManager()->IsAgitEntered())
					{
						roomInfo.str("");
						roomInfo << KncUtil::toNarrowString(g_pkStrLoader->GetString(STR_ID_MESSENGER_USER_LOCATION10)) << " " << g_MyD3D->m_pStateMachine->GetRoomNumber() + 1 << "(" << KncUtil::toNarrowString(g_pkStrLoader->GetString(STR_ID_MESSENGER_USER_LOCATION12)) << ")";
						actualState = roomInfo.str();
						discordPresence.partyId = std::to_string(SiKGCRoomManager()->GetRoomInfo().dwHostUID).data();
						discordPresence.partySize = g_MyD3D->GetPlayerNum();
						discordPresence.partyMax = SiKGCWorldMapManager()->GetDungeonMaxPlayerNum(SiKGCRoomManager()->GetGameMode());
						discordPresence.startTimestamp = m_tmStartTime;
					}
					else
					{
						actualState = "";
					}
				}
				else
				{
					strCharName = KncUtil::toNarrowString(g_kGlobalValue.GetCharName(g_pkUIScene->m_pkNewTutorialOption->GetMySelectCharID()));
					GCUTIL_STR::ToLower(strCharName);
					discordPresence.smallImageKey = strCharName.c_str();
					discordPresence.smallImageText = "";
					actualState = KncUtil::toNarrowString(g_kGlobalValue.GetJopName(g_pkUIScene->m_pkNewTutorialOption->GetMySelectCharID(), g_pkUIScene->m_pkNewTutorialOption->GetMySelectJobID()));
					discordPresence.startTimestamp = m_tmStartTime;
				}
				discordPresence.state = actualState.c_str();
			}
			break;
	}

	if ( ( g_MyD3D->m_pStateMachine->GetState() == GS_GAME && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TUTORIAL ) || g_MyD3D->m_pStateMachine->GetState() != GS_GAME )
	{
		imageText.str(L"");
		imageText << g_kGlobalValue.m_kUserInfo.strNickName << L" (Nv. " << (int)g_kGlobalValue.m_kUserInfo.GetCurrentChar().iLevel << L")";
		strCharName = KncUtil::toNarrowString(g_kGlobalValue.GetCharName(g_kGlobalValue.m_kUserInfo.GetCurrentCharType()));
		GCUTIL_STR::ToLower(strCharName);
		strImageText = to_string(imageText.str());
		if (strCharName.empty())
		{
			discordPresence.smallImageText = "";
			discordPresence.smallImageKey = "todos";
		}
		else
		{
			discordPresence.smallImageText = strImageText.c_str();
			discordPresence.smallImageKey = strCharName.c_str();
		}
	}

	strLocation = to_string(g_MyD3D->m_pStateMachine->GetCurrentLocation());
	strLocationIcon = g_MyD3D->m_pStateMachine->GetCurrentLocationIcon();
	discordPresence.largeImageText = strLocation.c_str();
	discordPresence.largeImageKey = strLocationIcon.c_str();
	discordPresence.details = strLocation.data();

	discordPresence.instance = 0;
	Discord_UpdatePresence(&discordPresence);
}
#endif