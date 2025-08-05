#include "stdafx.h"

//

#include <io.h>

/// 패치 프로그램을 업데이트한다.
/// @return 0(패치할 필요 없음), 1(성공), -1(실패)
int PatchPatcher( void )
{
    // NOTE by parkch 2006-01-13 : 패치 프로그램에 대한 패치는 게임 종료 시에
    // 파일을 바꿔주기만 하고 순환 실행을 하지 않는 것으로 바꾼다.
    // 게임 종료 시에 이루어지므로 바꾸는 도중에 에러가 발생하더라도 메세지를 띄우지 말자.

    // 패치할 패치 프로그램이 있는지 확인한다.
    if ( 0 == ::_access( "GrandChaseTemp.exe", 0 ) && 0 == ::_access( "GrandChase.exe", 0 ) )
    {
        // 기본 파일을 지우기 전 파일 속성을 확인해서 읽기 전용 속성이 있다면 빼버리자.
        DWORD dwErrCount = 0;
        DWORD dwAtt = ::GetFileAttributesA( "GrandChase.exe" );
        if ( 0xffffffff != dwAtt )
        {
            while ( FALSE == ::SetFileAttributesA( "GrandChase.exe", dwAtt & (~FILE_ATTRIBUTE_READONLY) ) )
            {
                if ( ++dwErrCount > 10 )
                {
                    return -1; // fail
                }
                ::Sleep( 300 ); // 안되면 잠시 쉬어줬다가 다시 해보자.
            }
        }

        // 기존의 파일을 제거한다.
        dwErrCount = 0;
        while ( FALSE == ::DeleteFileA( "GrandChase.exe" ) )
        {
            if ( ++dwErrCount > 10 )
            {
                return -1; // fail
            }
            ::Sleep( 300 ); // 안되면 잠시 쉬어줬다가 다시 해보자.
        }

        // 새로 받은 파일을 원본 파일 이름으로 바꾼다.
        if ( FALSE == ::MoveFileA( "GrandChaseTemp.exe", "GrandChase.exe" ) )
        {
            return -1; // fail
        }

        // NOTE : 패치 프로그램을 패치한 후 다시 실행시키는 일은 이제 하지 말자. by parkch 2006-01-13

        //// 새 패치 프로그램을 실행시킨다.
        //PROCESS_INFORMATION pi;
        //STARTUPINFOA si = { 0, };
        //si.cb = sizeof(STARTUPINFO);
        //char szProgram[MAX_PATH] = { 0, };
        //::sprintf(szProgram, "GrandChase.exe %s", __argv[1]);
        //if (FALSE == ::CreateProcessA(NULL,
        //                             szProgram,
        //                             NULL,
        //                             NULL,
        //                             FALSE,
        //                             0,
        //                             NULL,
        //                             NULL,
        //                             &si,
        //                             &pi))
        //{
        //    return -1; // fail
        //}

        return 1; // success
    }

    return 0; // nothing
}