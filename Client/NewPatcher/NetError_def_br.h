癤?#ifdef __NEVER_DEFINED_MACRO__
#define NET_ERROR(VAL, VAL2) VAL,
enum
{
#endif

    NET_ERROR( NET_OK = 0,              Sucesso	                                 )
    NET_ERROR( ERR_UNKNOWN,             Erro desconhecido	                     )
    NET_ERROR( PS_INIT_PATCHER,         Inicializando	                         )
    NET_ERROR( PS_CHECK_PATCHER,        Confirmando atualiza챌찾o do Patcher	     )
    NET_ERROR( PS_PATCHER_PATCH,        Atualizando Patcher	                     )
    NET_ERROR( PS_PROGRAMPATH_ERR,      N찾o foi poss챠vel encontrar arquivo	     )
    NET_ERROR( PS_WEBOPEN_ERR,          Erro na conex찾o de rede	                 )
    NET_ERROR( PS_PATCHSRC_ERR,         Falha na conex찾o com servidor de patch	 )
    NET_ERROR( PS_DOWN_PATCHPATH_ERR,   Falha no download de arquivo de patch	 )
    NET_ERROR( PS_PATCHPATH_READ_ERR,   Falha na leitura do arquivo de patch	 )
    NET_ERROR( PS_NEWVER_DOWN_ERR,      Falha no donwload de vers찾o	             )
    NET_ERROR( PS_VER_READ_ERR,         Erro na vers찾o de programa	             )
    NET_ERROR( PS_PATCHSTR,             Falha na leitura de texto	             )
    NET_ERROR( PS_IMAGE_NO_EXIST,       IMAGE.KOM n찾o existe	                 )
    NET_ERROR( PS_IMAGE_NO_LOAD,        Falha no carregamento de         	     )
    NET_ERROR( PS_PATCHTIME_MSG,        Servidor fechado para manuten챌찾o            )
    NET_ERROR( PS_EXECUTE_IN_SYSTEM_FOLDER,        "Seu jogo est찼 instalado em um diret처rio inv찼lido. Por favor, reinstale o jogo usando o cliente mais recente dispon챠vel" )
NET_ERROR( ERR_SENTINEL,    ERR_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef NET_ERROR
#endif
