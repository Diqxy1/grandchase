#ifndef _KNCFIREWALL_H_
#define _KNCFIREWALL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#undef _DLLAPI

#if defined(_USRDLL)    // DLL library compile 중
#   define _DLLAPI __declspec(dllexport)
#else
#   define _DLLAPI __declspec(dllimport)
#endif

#pragma comment( lib, "KncFirewall" )

extern "C"
{
    namespace KncFirewall 
    {
        enum ENUM_RETURN 
        {
            KNC_OK = 0,                 // ㅇㅋㅂㄹ.
            __INIT_COM_FAILED,          // COM 초기화 실패. 과연 이런 경우도 있을 것인가? // 050222. 내부 COM 초기화 삭제.
            NOT_WINXP_SP2,              // Windows XP SP2 가 아님.
            NO_EXCEPTION_ALLOWED,       // '모든 예외 허용 안함' 모드 동작중 - 게임 실행 불가능
            NOT_SYSADMIN,               // 예외 항목을 변경하려 했으나 시스템 ADMIN 권한이 없음
        };

        // 050222. 아래 세 함수 모두 사용하기 전에 반드시 CoInitialize 가 된 상태여야 한다.
        // 그렇지 않으면 WindowXP sp2 에서도 WindowXP sp2 가 아니라고 인식하게 된다.
        _DLLAPI bool OnInstallApplication( char* szFullFilePath, char* szName );
        _DLLAPI bool OnUninstallApplication( char* szFullFilePath );
        _DLLAPI int GuarantedFireWallException( char* szFullFilePath, char* szName );
    }
}

#endif // _KNCFIREWALL_H_