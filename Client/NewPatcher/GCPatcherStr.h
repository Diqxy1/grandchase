#pragma once
#include <map>
#include <StdAfx.h>
#include <Windows.h>

#include "NetError.h"
using namespace NetError;

class KGCPatcherStr
{
private:
    static std::map< DWORD, DWORD> m_mapErr;
    static void InitString();
public:
    KGCPatcherStr(void);
    ~KGCPatcherStr(void);

    enum PATCHERERROR
    {
        PE_PROGRAMPATH,
        PE_WEBOPEN,
        PE_PATCHSRC,
        // PatchPath.dat없는 경우 말고, 패치 시간 파일 없는 경우도 따로 찍어 주고 싶었소만,
        // 다시 번역가져오고 할만한 시기가 안되서, 난중에 에러메시지 추가할일 있으면 추가하세요.
        PE_DOWN_PATCHPATH,
        PE_PATCHPATH_READ,
        PE_NEWVER_DOWN,
        PE_VER_READ,
        PE_PATCHSTR,
        PE_IMAGE_NO_EXIST,
        PE_IMAGE_NO_LOAD,
        PE_PATCHTIME_MSG,  
        PE_EXECUTE_IN_SYSTEM_FOLDER,      
        PE_NUM,
		PE_DOWN_LANGINFO = 27,
    };

    static CString GetString( DWORD dwID_ );
    static void ShowErrorMsgBox( DWORD dwErrCode_ );
};
