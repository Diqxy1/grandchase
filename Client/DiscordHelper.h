#ifndef DEV_BUILD
#ifndef _DISCORDHELPER_H_
#define _DISCORDHELPER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class DiscordHelper
{
private:
	DiscordHelper();
	~DiscordHelper();

public:
	static DiscordHelper* GetInstance();
	void Refresh();
	void UpdatePresence();
private:
	std::string to_string(const std::wstring& wstr);
	bool m_bStartedPlaying;
	time_t m_tmStartTime;
};

#endif // _DISCORDHELPER_H_
#endif