
#include <boost/test/unit_test.hpp>
//#include <WTypes.h>
//#include "CPSSOLib.h"
//#include <iostream>
//#include "nmcred.h"
//#include "KncUtil.h"
//#include "winerror.h"
//
////#pragma comment( lib, "../GameServer/CPSSOLib" )  // 넷마블 인증 라이브러리
//#pragma comment( lib, "../GameServer/nmcred" ) // 넷마블 체험 계정 인증

void TestNetmarbleExpAuth()
{
   // nmcred cred;
   // nmcred_entry entry1;
   // nmcred_entry entry2;
   // nmcred_entry entry3;
   // double seconds;

   // nmcred_init(
   //     &cred,
   //     _T("+JqAh6ng7pd73wekIbgeAhboc5jf4GiSSro3Ymd/0tQ6oJ5b+mmPBZHrQMs/boP0ag9u4/mdHReBownsDmPToUXQWY8pp571IIHhE3QpMu+qR31u4aMUZJnW0BAetMUNJo2u0rUsL4toblqgFknCdXcLqc0UXRJbjR2mtj3Sn6E6cit7lE3Fk7BXh6h0g5qHBiaWOssr0RiL49UUldvIfi9CRvFpF18zyXsRtPF699aZtvLaQnZOSIz2IQKaN5id1UwsUiH8HfDq7XdD0Nm112eGjraAZY8miYdmUsoqbGA="),
   //     (BYTE*)"spt!ak#qmf^_cpgja@",
   //     18);
   // seconds = nmcred_elapsed_seconds(&cred);
   // nmcred_find_entry(&cred, _T("EUserID"), &entry1);
   // nmcred_find_entry(&cred, _T("EUniID"), &entry2);
   // nmcred_find_entry(&cred, _T("EIDState"), &entry3);


   // _tprintf(_T("elapsed seconds = %d\n"), (int)seconds);
   // printf("%s = %s\n", entry1.name, entry1.value);
   // printf("%s = %s\n", entry2.name, entry2.value);
   // printf("%s = %s\n", entry3.name, entry3.value);


   // //nmcred nmObject;
   // //nmcred nmObject2;
   // nmcred_entry nmEntry;

   // //ZeroMemory(&cred,sizeof(nmObject));
   // //ZeroMemory(&nmObject2,sizeof(nmObject2));


   // BYTE key[] = {"WOOSH"};

   // //const WCHAR wstrPasswd[] =L"+JqAh6ng7pd73wekIbgeAhboc5jf4GiSSro3Ymd/0tQ6oJ5b+mmPBZHrQMs/boP0ag9u4/mdHReBownsDmPToUXQWY8pp571IIHhE3QpMu+qR31u4aMUZJnW0BAetMUNJo2u0rUsL4toblqgFknCdXcLqc0UXRJbjR2mtj3Sn6E6cit7lE3Fk7BXh6h0g5qHBiaWOssr0RiL49UUldvIfi9CRvFpF18zyXsRtPF699aZtvLaQnZOSIz2IQKaN5id1UwsUiH8HfDq7XdD0Nm112eGjraAZY8miYdmUsoqbGA=";
   //
   // //int nResult =  nmcred_init( &nmObject, wstrPasswd, key, sizeof( key ) / sizeof(BYTE)) ;
   // nmcred_init(&cred, NULL, NULL, 0);

   // int re =  nmcred_add_entry(&cred,L"WOOSH",L"SERVER");
   // WCHAR dest[2048];
   // size_t destz = 2048;
   // int re2 =nmcred_issue(&cred,key,5,dest,&destz);

   // std::wcout << dest << std::endl;


   // int re3 = nmcred_init(&cred, dest, key, 5);
   // 
   // nmcred_find_entry(&cred, L"WOOSH", &nmEntry);
   // printf("%s = %s\n", nmEntry.name, nmEntry.value);

   // //std::cout << nmEntry.name << ", " << nmEntry.value << std::endl;

   // //char szUserID[1024]          = {0};
   // //char szUniID[256]           = {0};
   // //char szSex[256]             = {0};
   // //char szAge[256]             = {0};
   // //char szYAge[256]            = {0};
   // //char szName[256]            = {0};

   // //char szCertificate[256]     = {0};
   // //char szIDState[256]         = {0};
   // //char szUniqueNick[256]      = {0};

   // //TCHAR* chPasswd = strPasswd.c_str();
   // //wchar_t* chPass = 

   // /*f( strPasswd.find("EXPUInfo=") != std::string::npos )
   // {
   //     nmcred_find_entry( &nmObject, L"EUserID", &nmEntry );

   //     //std::cout << nmEntry.name << std::endl;

   //     if( GetCPCookieValue( strPasswd.c_str(), "EXPUInfo", szUserID) == true )/*&&
   //                                                                         GetCPCookieValue( strPasswd.c_str(), "Age", szAge) == true &&
   //                                                                         GetCPCookieValue( strPasswd.c_str(), "Name", szName) == true &&
   //                                                                         GetCPCookieValue( strPasswd.c_str(), "UniID", szUniID) == true &&
   //                                                                         GetCPCookieValue( strPasswd.c_str(), "Sex", szSex) == true &&
   //                                                                         GetCPCookieValue( strPasswd.c_str(), "Certificate", szCertificate) == true &&
   //                                                                         GetCPCookieValue( strPasswd.c_str(), "IDState", szIDState) == true &&
   //                                                                         GetCPCookieValue( strPasswd.c_str(), "UniqueNick", szUniqueNick) == true &&
   //                                                                         GetCPCookieValue( strPasswd.c_str(), "YAge", szYAge) == true )*/
   //     //{
   //       //  std::cout << "UserID : " << szUserID << std::endl;
   //         //<< "Age : " << szAge << std::endl
   //         //<< "Name : " << szName << std::endl
   //         //<< "UniID : " << szUniID << std::endl
   //         //<< "Sex : " << szSex << std::endl
   //         //<< "Certificate : " << szCertificate << std::endl
   //         //<< "IDState : " << szIDState << std::endl
   //         //<< "UniqueNick : " << szUniqueNick << std::endl
   //         //<< "YAge : " << szYAge;
   //    // }
   //// }
 

}