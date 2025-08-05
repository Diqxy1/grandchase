
#include <boost/test/unit_test.hpp>
#include "CPSSOLib.h"
#include <iostream>
//#pragma comment( lib, "../GameServer/CPSSOLib" )  // 넷마블 인증 라이브러리

void TestNetmarbleAuth()
{
    //std::string strPasswd =
    //    "UserID=4B12701A0F9D68DF3F2203D32C70666F&"
    //    "Age=AC56EBF8340400BC&"
    //    "Name=9F36BC0D3940085D&"
    //    "UniID=A44ED6AC0985F1CEFBD1044A82BBAFDC&"
    //    "Sex=F1553A2F8BD18A59&"
    //    "Certificate=F1553A2F8BD18A59&"
    //    "IDState=3F01788A2482BA3A&"
    //    "UniqueNick=4B12701A0F9D68DF3F2203D32C70666F&"
    //    "YAge=1FAF0BEB36FC7BAD";
/*
    std::string strPasswd =
    "UserID=4B12701A0F9D68DF3F2203D32C70666F&Age=AC56EBF8340400BC&Name=9F36BC0D3940085D&UniID=A44ED6AC0985F1CEFBD1044A82BBAFDC&Sex=F1553A2F8BD18A59&Certificate=F1553A2F8BD18A59&IDState=3F01788A2482BA3A&UniqueNick=4B12701A0F9D68DF3F2203D32C70666F&YAge=1FAF0BEB36FC7BAD";
*/

std::string strPasswd =
"Certificate=418bfccd6bd6f944cfaae551dec08044&Sex=418bfccd6bd6f944cfaae551dec08044&IDState=ab8cc1b487a3a6408c4021cc630a8afd&Age=1de2197acef3a376ff938f80fda27571&Name=6f44e63367c11525b1d26c95e513c3e5&UniID=ddbc5eb8945faceb143a246b9105d8e6&UserID=93ae2a51ac231ac7d28e7115ff4bff43&Site=a192f1eeee0a70f44c12dd7bca843436&CPTS=0e390b405ffd2921193da31b5e931955878b359e3d2609c0937ea2e4e1fb1a20";
    //std::string strPasswd =
    //"Certificate=F1553A2F8BD18A59&"
    //"Sex=F1553A2F8BD18A59&"
    //"Name=567C127DB96FA866&"
    //"UniID=2221F0C2A5C537DDC268A9BAD6DFAE66&"
    //"Age=81362FA0EA7AE52C&"
    //"YAge=81362FA0EA7AE52C&"
    //"UniqueNick=153341236EF0BE3CB724E31B914AE7B6&"
    //"IDState=3F01788A2482BA3A&"
    //"UserID=153341236EF0BE3CB724E31B914AE7B6&"
    //"CPTS=feea93bc48c54842041ed66e6c6ec76b2078dfc9021f73af";

    //std::string strPasswd =
    //"Certificate=F1553A2F8BD18A59&Sex=F1553A2F8BD18A59&Name=3915B05F2BDF8EF1&UniID=63D2AF92D1E4132E69A6D99D980DD515&Age=88C6256D8595890C&YAge=88C6256D8595890C&UniqueNick=85430955039CF2DD&IDState=0994E2CB626D7449&UserID=85430955039CF2DD";



    //std::string strPasswd = "CPUInfo=UserID=8226326690679F179EDF2FAFB5119073&Age=1489FA5CE12AEAB7&Name=9E33F0A9A5B6513D&UniID=7F737D49FAA3B4EF0B973190F9051D41&Sex=F1553A2F8BD18A59&Certificate=F1553A2F8BD18A59&IDState=3F01788A2482BA3A&UniqueNick=8226326690679F179EDF2FAFB5119073&YAge=1489FA5CE12AEAB7";

    char szUserID[256]          = {0};
    char szUniID[256]           = {0};
    char szSex[256]             = {0};
    char szAge[256]             = {0};
    char szYAge[256]            = {0};
    char szName[256]            = {0};
    char szSite[256]            = {0};

    char szCertificate[256]     = {0};
    char szCPTS[256]     = {0};
    char szIDState[256]         = {0};
    char szUniqueNick[256]      = {0};

    std::string strUniID;

    if( strPasswd.find("UserID=") != std::string::npos &&
        GetCPCookieValue( strPasswd.c_str(), "UserID", szUserID) == true && 
        GetCPCookieValue( strPasswd.c_str(), "Age", szAge) == true &&
        GetCPCookieValue( strPasswd.c_str(), "Name", szName) == true &&
        GetCPCookieValue( strPasswd.c_str(), "UniID", szUniID) == true )
//        GetCPCookieValue( strPasswd.c_str(), "Sex", szSex) == true &&
//        GetCPCookieValue( strPasswd.c_str(), "Certificate", szCertificate) == true &&
//        GetCPCookieValue( strPasswd.c_str(), "IDState", szIDState) == true &&
//        GetCPCookieValue( strPasswd.c_str(), "UniqueNick", szUniqueNick) == true &&
//        GetCPCookieValue( strPasswd.c_str(), "YAge", szYAge) == true )

/*
    if( strPasswd.find("UserID=") != std::string::npos &&
        GetCPCookieValue( strPasswd.c_str(), "Certificate", szCertificate) == true &&
        GetCPCookieValue( strPasswd.c_str(), "UserID", szUserID) == true )

        GetCPCookieValue( strPasswd.c_str(), "Sex", szSex) == true &&
        GetCPCookieValue( strPasswd.c_str(), "IDState", szIDState) == true &&
        GetCPCookieValue( strPasswd.c_str(), "Age", szAge) == true &&
        GetCPCookieValue( strPasswd.c_str(), "Name", szName) == true &&
        GetCPCookieValue( strPasswd.c_str(), "UniID", szUniID) == true &&
        GetCPCookieValue( strPasswd.c_str(), "UserID", szUserID) == true &&
        GetCPCookieValue( strPasswd.c_str(), "Site", szSite) == true &&
        GetCPCookieValue( strPasswd.c_str(), "CPTS", szCPTS) == true )
*/
    {
        strUniID = szUniID;

        if( strUniID.find("EX") != std::string::npos )
        {
            std::cout << "이것은 체험 계정이군요 " << std::endl;
        }

        std::cout << "UserID : " << szUserID << std::endl
            << "Age : " << szAge << std::endl
            << "Name : " << szName << std::endl
            << "UniID : " << szUniID << std::endl
            << "Sex : " << szSex << std::endl
            << "Certificate : " << szCertificate << std::endl
            << "IDState : " << szIDState << std::endl
            << "UniqueNick : " << szUniqueNick << std::endl
            << "YAge : " << szYAge;
    }

}