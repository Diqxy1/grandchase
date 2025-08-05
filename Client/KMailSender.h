#pragma once


//#include "define.h"
#include <tchar.h>
//#include <string>
//#include <Windows.h>

class KMailSender
{
public:
    KMailSender(void);
    ~KMailSender(void);    

    void SendMail( std::string strSMTP, std::string strFrom , std::string strTo, std::string strTitle, std::string strBody, HWND hWnd );

	void SendCurrentGameGuardInfo( std::string strAddInfo );
	void SendHttpPost( std::string strTo, std::string strTitle, std::string strBody, std::string strFileName="" );    
};
