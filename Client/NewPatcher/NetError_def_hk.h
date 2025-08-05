癤?#ifdef __NEVER_DEFINED_MACRO__
#define NET_ERROR(VAL, VAL2) VAL,
enum
{
#endif

    NET_ERROR( NET_OK,                  ??                        )
    NET_ERROR( ERR_UNKNOWN,             ?ょη?沃?                 )
    NET_ERROR( PS_INIT_PATCHER,         閭ｅⓨ冶?.                 )
    NET_ERROR( PS_CHECK_PATCHER,        patcher upgrade閻븃?訝.      )
    NET_ERROR( PS_PATCHER_PATCH,        閭ｅ?atcher upgrade.        )
    NET_ERROR( PS_PROGRAMPATH_ERR,      葉凉瓮孃?孃鸚길.           )
    NET_ERROR( PS_WEBOPEN_ERR,          泳꿱러???沃?!              )
    NET_ERROR( PS_PATCHSRC_ERR,         patch server瓮孃?孃鸚길!!  )
    NET_ERROR( PS_DOWN_PATCHPATH_ERR,   patch server瓮孃訝雍鸚길!!  )
    NET_ERROR( PS_PATCHPATH_READ_ERR,   patch server瓮孃溫?鸚길!!  )
    NET_ERROR( PS_NEWVER_DOWN_ERR,      ??ф?旅訝雍鸚길!!!         )
    NET_ERROR( PS_VER_READ_ERR,         葉凉??ф?旅?沃?!!         )
    NET_ERROR( PS_PATCHSTR,             ?耶?溫??沃?!!           )
    NET_ERROR( PS_IMAGE_NO_EXIST,       IMAGE.KOM訝耶??!!          )
    NET_ERROR( PS_IMAGE_NO_LOAD,        IMAGE.KOM load鸚길!!!       )
    NET_ERROR( PS_PATCHTIME_MSG,        ??鴉뷸?ⓩｅ①땟岳訝     )
    NET_ERROR( PS_EXECUTE_IN_SYSTEM_FOLDER,        ??꿨獒瓮孃?沃ㅿ?恙?若?당㎉?ㅴ말葉凉竊??겼獒??곈?꿰??     )
NET_ERROR( ERR_SENTINEL,    ERR_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef NET_ERROR
#endif
