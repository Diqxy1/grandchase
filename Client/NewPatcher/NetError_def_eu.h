
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
    NET_ERROR( PS_EXECUTE_IN_SYSTEM_FOLDER,        Your client is installed in an inaccessible folder. Please re-install in a new folder. )
    NET_ERROR( ERR_SENTINEL,    ERR_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef NET_ERROR
#endif
