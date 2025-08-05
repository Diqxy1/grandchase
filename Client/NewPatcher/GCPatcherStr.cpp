#include "stdafx.h"
#include ".\gcpatcherstr.h"

std::map< DWORD, DWORD> KGCPatcherStr::m_mapErr;


KGCPatcherStr::KGCPatcherStr(void)
{   
}

KGCPatcherStr::~KGCPatcherStr(void)
{
}

CString KGCPatcherStr::GetString( DWORD dwID_ )
{
    return NetError::GetErrStr( dwID_ );
}

void KGCPatcherStr::InitString()
{   
	KGCPatcherStr::m_mapErr[PE_PROGRAMPATH] = NetError::PS_PROGRAMPATH_ERR;
    KGCPatcherStr::m_mapErr[PE_WEBOPEN] = NetError::PS_WEBOPEN_ERR;
    KGCPatcherStr::m_mapErr[PE_PATCHSRC] = NetError::PS_PATCHSRC_ERR;
    KGCPatcherStr::m_mapErr[PE_DOWN_PATCHPATH] = NetError::PS_DOWN_PATCHPATH_ERR;
    KGCPatcherStr::m_mapErr[PE_PATCHPATH_READ] = NetError::PS_PATCHPATH_READ_ERR;
    KGCPatcherStr::m_mapErr[PE_NEWVER_DOWN] = NetError::PS_NEWVER_DOWN_ERR;
    KGCPatcherStr::m_mapErr[PE_VER_READ] = NetError::PS_VER_READ_ERR;
    KGCPatcherStr::m_mapErr[PE_PATCHSTR] = NetError::PS_PATCHSTR;
    KGCPatcherStr::m_mapErr[PE_IMAGE_NO_EXIST] = NetError::PS_IMAGE_NO_EXIST;
    KGCPatcherStr::m_mapErr[PE_IMAGE_NO_LOAD] = NetError::PS_IMAGE_NO_LOAD;
    KGCPatcherStr::m_mapErr[PE_PATCHTIME_MSG] = NetError::PS_PATCHTIME_MSG;    
    KGCPatcherStr::m_mapErr[PE_EXECUTE_IN_SYSTEM_FOLDER] = NetError::PS_EXECUTE_IN_SYSTEM_FOLDER;    
}

void KGCPatcherStr::ShowErrorMsgBox( DWORD dwErrCode_ )
{
    InitString();
    if( m_mapErr.find( dwErrCode_) == m_mapErr.end() )
    {
        //이럴리가 없지만 일단 크래쉬 안나게 예외처리..
    }
    else
    {
        CString strErrCode;
        strErrCode.Format( L"%s0x%x", L"Error ", dwErrCode_ );
        // 에러 표시 해줄때, 패치 타임이라 패쳐패쳐 못받아서 발생한 경우는 패쳐 못받았다고 알려준다.
        // 하나하나 예외처리 해주기에는 므흣한 부분이 있어서 여기세어 처리해줘 버림..-_-;
        ::MessageBox( NULL, KGCPatcherStr::GetString( m_mapErr[dwErrCode_] ), strErrCode, 0 );
    }
}
