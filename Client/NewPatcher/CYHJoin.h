/*-- game launch join CYHall need code as follow:

	int ret = join::CheckCYHallA("씽땋눈綱", "JDCS", "1.0.0.1", "MMORPG");
	if (ret & join::RUN_GAME) {
	// enter into game
	}

	if (ret & join::LAUNCH_EXIT) {
	// launch exit 
	}

	if (ret & join::DOWNLOAD_HALL) {
	// download hall (notice:you should save the hall installation package to the launch's directory, and with the name "CYouGame.exe")
	}
*/





#ifndef CYH_JOIN_H_
#define CYH_JOIN_H_


#pragma warning (disable: 4786)

#include <Windows.h>

#ifdef CYHJOIN_EXPORTS
	#define		CYHAPI		__declspec(dllexport)
#else
	#define		CYHAPI
#endif


#ifdef __cplusplus
extern "C" {
#endif

namespace join
{
	enum CYHJoinState
	{
		LAUNCH_EXIT = 1,							//!< launch exit
		RUN_GAME = 2,								//!< run game
		DOWNLOAD_HALL = 4,							//!< download hall
	};
	
	/*!
	@brief		check CYHall status
	@param		name			IN			game name
	@param		keyword			IN			game keyword(privide by hall)
	@param		version			IN			game version(not launch version)
	@param		group			IN			game group	(eg:MMORPG)
	@return		what launch should do
	************************************************************************/
	CYHAPI int		CheckCYHallA(const char* name, const char* keyword, const char* version, const char* group);
	CYHAPI int		CheckCYHallW(const wchar_t* name, const wchar_t* keyword, const wchar_t* version, const wchar_t* group);
}

	

#ifdef __cplusplus
}
#endif	



#endif