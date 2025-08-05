
#ifdef __NEVER_DEFINED_MACRO__
#define NET_ERROR(VAL, VAL2) VAL,
enum
{
#endif

    NET_ERROR( NET_OK = 0,              Sukses							    )
    NET_ERROR( ERR_UNKNOWN,             Error tidak diketahui				)
    NET_ERROR( PS_INIT_PATCHER,         Inisialisasi Patch			        )
    NET_ERROR( PS_CHECK_PATCHER,        Mengecek Upgrade Patcher		    )
    NET_ERROR( PS_PATCHER_PATCH,        Upgrade Patcher		                )
    NET_ERROR( PS_PROGRAMPATH_ERR,      Folder GC tidak ditemukan		    )
    NET_ERROR( PS_WEBOPEN_ERR,          Error pada jaringan		            )
    NET_ERROR( PS_PATCHSRC_ERR,         Server Patch tidak tersedia   	    )
    NET_ERROR( PS_DOWN_PATCHPATH_ERR,   Download Patch tidak tersedia	    )
    NET_ERROR( PS_PATCHPATH_READ_ERR,   Tidak dapat membaca URL file patch  )
    NET_ERROR( PS_NEWVER_DOWN_ERR,      Versi file download salah     	    )
    NET_ERROR( PS_VER_READ_ERR,         Versi client salah   	            )
    NET_ERROR( PS_PATCHSTR,             Tidak dapat membaca string     	    )
    NET_ERROR( PS_IMAGE_NO_EXIST,       Error pada IMAGE.KOM 				)
    NET_ERROR( PS_IMAGE_NO_LOAD,        Tidak dapat membuka IMAGE.KOM		)
    NET_ERROR( PS_PATCHTIME_MSG,        Maintenance         )
	NET_ERROR( PS_EXECUTE_IN_SYSTEM_FOLDER,	잘못된 경로에 게임이 설치되어 있으니 반드시 최신버전으로 재설치를 해주십시오.)
    NET_ERROR( ERR_SENTINEL,    ERR_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef NET_ERROR
#endif







