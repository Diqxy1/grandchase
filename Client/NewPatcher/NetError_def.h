
#ifdef __NEVER_DEFINED_MACRO__
#define NET_ERROR(VAL, VAL2) VAL,
enum
{
#endif

NET_ERROR( NET_OK = 0,              성공                            )
NET_ERROR( ERR_UNKNOWN,             알 수 없는 에러                 )
NET_ERROR( PS_INIT_PATCHER,         패쳐 설정을 초기화 중입니다.    )
NET_ERROR( PS_CHECK_PATCHER,        패쳐 업그레이드를 확인합니다.   )
NET_ERROR( PS_PATCHER_PATCH,        패쳐를 업그레이드 합니다.       )
NET_ERROR( PS_PROGRAMPATH_ERR,      프로그램 경로 구하기 실패!!     )
NET_ERROR( PS_WEBOPEN_ERR,          네트워크 상태 오류!!            )
NET_ERROR( PS_PATCHSRC_ERR,         패치서버 경로 구하기 실패!!     )
NET_ERROR( PS_DOWN_PATCHPATH_ERR,   패치서버 경로 다운로드 실패!!   )
NET_ERROR( PS_PATCHPATH_READ_ERR,   패치서버 경로 읽기 실패!!       )
NET_ERROR( PS_NEWVER_DOWN_ERR,      버젼 파일 다운로드 실패!!       )
NET_ERROR( PS_VER_READ_ERR,         클라이언트 버젼 파일 에러!!!    )
NET_ERROR( PS_PATCHSTR,             문자열 읽기 오류!!!             )
NET_ERROR( PS_IMAGE_NO_EXIST,       IMAGE.KOM이 존재하지 않음!!!    )
NET_ERROR( PS_IMAGE_NO_LOAD,        IMAGE.KOM 로드 실패!!!          )
NET_ERROR( PS_PATCHTIME_MSG,        서버 점검 중입니다.            )
NET_ERROR( PS_EXECUTE_IN_SYSTEM_FOLDER,        잘못된 경로에 게임이 설치되어 있으니 반드시 최신버전으로 재설치를 해주십시오. )
NET_ERROR( ERR_SENTINEL,    ERR_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef NET_ERROR
#endif
