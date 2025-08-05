
#ifdef __NEVER_DEFINED_MACRO__
#define NET_ERROR(VAL, VAL2) VAL,
enum
{
#endif

	NET_ERROR( NET_OK = 0,              Success							)
	NET_ERROR( ERR_UNKNOWN,             Unknown Error					)
	NET_ERROR( PS_INIT_PATCHER,         Patcher is Initializing			)
	NET_ERROR( PS_CHECK_PATCHER,        Checking Patcher Upgrade		)
	NET_ERROR( PS_PATCHER_PATCH,        Upgrading Patcher Now			)
	NET_ERROR( PS_PROGRAMPATH_ERR,      GC Folder Doesn’t Exist		)
	NET_ERROR( PS_WEBOPEN_ERR,          Network Error          			)
	NET_ERROR( PS_PATCHSRC_ERR,         Patch Server Unavailable    	)
	NET_ERROR( PS_DOWN_PATCHPATH_ERR,   Patch Download Unavailable 		)
	NET_ERROR( PS_PATCHPATH_READ_ERR,   Unable to Obtain Patch File URL	)
	NET_ERROR( PS_NEWVER_DOWN_ERR,      Version File Download Fail     	)
	NET_ERROR( PS_VER_READ_ERR,         Client Version File Error   	)
	NET_ERROR( PS_PATCHSTR,             Unable to Read String Code     	)
	NET_ERROR( PS_IMAGE_NO_EXIST,       IMAGE.KOM Error  				)
	NET_ERROR( PS_IMAGE_NO_LOAD,        Unable   to Load IMAGE.KOM		)
    NET_ERROR( PS_PATCHTIME_MSG,        Server Maintenance           )
    NET_ERROR( PS_EXECUTE_IN_SYSTEM_FOLDER,        잘못된 경로에 게임이 설치되어 있으니 반드시 최신버전으로 재설치를 해주십시오. )
    NET_ERROR( ERR_SENTINEL,    ERR_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef NET_ERROR
#endif
