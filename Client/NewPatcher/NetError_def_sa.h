
#ifdef __NEVER_DEFINED_MACRO__
#define NET_ERROR(VAL, VAL2) VAL,
enum
{
#endif

    NET_ERROR( NET_OK = 0,              Satisfactorio					                    )
    NET_ERROR( ERR_UNKNOWN,             Error desconocido   				                )
    NET_ERROR( PS_INIT_PATCHER,         reinicia la configuracion de actualizaciones        )
    NET_ERROR( PS_CHECK_PATCHER,        Revisando la actualizacion del Patcher  	        )
    NET_ERROR( PS_PATCHER_PATCH,        Actualizando el Patcher                             )
    NET_ERROR( PS_PROGRAMPATH_ERR,      Fallo el programa de busqueda del Patcher	        )
    NET_ERROR( PS_WEBOPEN_ERR,          Error de conexion           		                )
    NET_ERROR( PS_PATCHSRC_ERR,         Fallo el programa de busqueda del Servidor Patcher  )
    NET_ERROR( PS_DOWN_PATCHPATH_ERR,   Fallo la descarga de la direccion del Patcher	    )
    NET_ERROR( PS_PATCHPATH_READ_ERR,   Fallo la lectura de direcciones de descarga         )
    NET_ERROR( PS_NEWVER_DOWN_ERR,      Fallo la descarga de la version              	    )
    NET_ERROR( PS_VER_READ_ERR,         Error en la version del cliente        	            )
    NET_ERROR( PS_PATCHSTR,             Error en la lectura de strings              	    )
    NET_ERROR( PS_IMAGE_NO_EXIST,       No existe IMAGE.KOM                  				)
    NET_ERROR( PS_IMAGE_NO_LOAD,        Fallo la carga de IMAGE.KOM                 		)
    NET_ERROR( PS_PATCHTIME_MSG,        El servidor se encuentra en mantenimiento           )
	NET_ERROR( PS_EXECUTE_IN_SYSTEM_FOLDER,	El cliente se encuentra instalado en una localizacion erronea, por favor reinstala la version actual.)
    NET_ERROR( ERR_SENTINEL,    ERR_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef NET_ERROR
#endif







