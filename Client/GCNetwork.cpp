#include "stdafx.h"
//
//
#include "KncP2PLib/KncP2P.h"



#include "GCNetwork.h"
#include "Message.h"


#include "KSingleton.h"

//




char                Max_Player          = MAX_PLAYER_NUM;

// 접속 대화창의 프로시져
bool Init_TCPIP( void )
{

    std::cout << "=== INICIANDO CONEXÃO ===" << std::endl;
    std::cout << "UID do usuário: " << g_kGlobalValue.m_kUserInfo.dwUID << std::endl;
    std::cout << "Servidor IP: " << g_kGlobalValue.ServerInfo.dwServerIP << std::endl;
    std::cout << "Porta UDP: " << g_kGlobalValue.ServerInfo.usUdpPort << std::endl;

	// 메시지 핸들러 지정 및 Heart bit 시간 설정
    KncP2P* pKncP2P = KSingleton<KncP2P>::GetInstance();
    pKncP2P->SetDefaultLocalPort( ( DEFAULT_PORT + ( rand() % DEFAULT_PORT_RANGE ) ) );
    std::cout << "Porta local aleatória: " << pKncP2P->GetDefaultLocalPort() << std::endl;

	if ( pKncP2P->InitKncP2P( g_kGlobalValue.m_kUserInfo.dwUID,
                              KncP2PMessageHandler,
                              32,
                              g_kGlobalValue.ServerInfo.dwServerIP,
                              g_kGlobalValue.ServerInfo.usUdpPort,
                              pKncP2P->GetDefaultLocalPort() ) == false )
    {
        std::cerr << "ERRO: Falha na inicialização do P2P" << std::endl;
        return false;
    }
    g_kGlobalValue.m_kUserInfo.vecIP   = pKncP2P->GetLocalIP();
    g_kGlobalValue.m_kUserInfo.vecPort = pKncP2P->GetLocalPort();
    
	if( std::find( g_kGlobalValue.m_kUserInfo.vecIP.begin(), g_kGlobalValue.m_kUserInfo.vecIP.end(), 
        g_kGlobalValue.ServerInfo.dwPublicIP ) == g_kGlobalValue.m_kUserInfo.vecIP.end() )
	{
        g_kGlobalValue.m_kUserInfo.vecIP.push_back( g_kGlobalValue.ServerInfo.dwPublicIP );
	}
#if defined( USE_ONLY_TCP_RELAY )
	KSingleton<KncP2P>::GetInstance()->SetRelayOnly( true);
#endif
    std::cout << "Conexão P2P inicializada com sucesso!" << std::endl;
	return true;
}

void ShutDown( bool Perfact_Clear )
{	
    // close and reinitialize
	{
        KncP2P* pKncP2P = KSingleton<KncP2P>::GetInstance();
        pKncP2P->Close();

        if ( false == Perfact_Clear )
        {
            pKncP2P->InitKncP2P( g_kGlobalValue.m_kUserInfo.dwUID,
                                 KncP2PMessageHandler,
                                 32,
                                 g_kGlobalValue.ServerInfo.dwServerIP,
                                 g_kGlobalValue.ServerInfo.usUdpPort,
                                 pKncP2P->GetDefaultLocalPort() );
		}        
	}
}
