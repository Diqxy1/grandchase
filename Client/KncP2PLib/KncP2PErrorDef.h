#ifdef __NEVER_DEFINED_MACRO__
#define KNC_P2P_ERROR(VAL, VAL2) VAL,
enum
{
#endif
KNC_P2P_ERROR( KNC_OK,      SUCCESS )
KNC_P2P_ERROR( KNC_FAIL,    FAIL )
//////////////////////////////////////////////////////////////////////////
//                      KNCMSGPROC_FUNC_ERROR
KNC_P2P_ERROR( KNC_MSG_PROC_ADDCONNECTQ , Try To Add existing UID In ConnectQ )
KNC_P2P_ERROR( KNC_MSG_PROC_REMOVECONNECTQ , Try To Remove not existing UID In ConnectQ )
KNC_P2P_ERROR( KNC_MSG_PROC_RESETCONNECTINFO, Try To ResetConnectInfo not existing UID In ConnectQ )
KNC_P2P_ERROR( KNC_MSG_PROC_CLOSECONNECTION, Try To CloseConnection not existing UID In PeerManager )
KNC_P2P_ERROR( KNC_MSG_PROC_RECV_STATICDATA, Recv StaticData existing UID In PeerManager Peer is not using relay )
KNC_P2P_ERROR( KNC_MSG_PROC_RECV_PID_CONNECT_REQ1, Recv PID_CONNECT_REQ existing UID In PeerManager Peer is using relay )
KNC_P2P_ERROR( KNC_MSG_PROC_RECV_PID_CONNECT_REQ2, Recv PID_CONNECT_REQ From Not RelayServer Address )
KNC_P2P_ERROR( KNC_MSG_PROC_RECV_PID_CONNECT_REQ3, Recv PID_CONNECT_REQ From Already Exist Peer )
KNC_P2P_ERROR( KNC_MSG_PROC_RECV_PID_CONNECT_ACK1, Recv PID_CONNECT_ACK existing UID In PeerManager )
KNC_P2P_ERROR( KNC_MSG_PROC_RECV_PID_CONNECT_ACK2, Recv PID_CONNECT_ACK From Not RelayServer Address )
KNC_P2P_ERROR( KNC_MSG_PROC_RECV_PID_CONNECT_ACK3, Recv PID_CONNECT_ACK From Already Exist Peer )
KNC_P2P_ERROR( KNC_MSG_PROC_REMOVECONNECTUID , Try To Remove not existing UID In ConnectUID )
KNC_P2P_ERROR( KNC_MSG_PROC_REMOVECONNECTREQUID , Try To Remove not existing UID In ConnectReqUID )
//////////////////////////////////////////////////////////////////////////
//                      KNCP2P상의 ERROR
KNC_P2P_ERROR( KNC_KNCP2P_CLOSE, m_pRakPeer is Invalid Pointer )
KNC_P2P_ERROR( KNC_KNCP2P_GETVALIDPORT, Can Not Get Vaild Port )
KNC_P2P_ERROR( KNC_KNCP2P_INIT_RAKNET, Fail To Initialize RakNet )
KNC_P2P_ERROR( KNC_KNCP2P_INVALID_PTR, Exist Invalid Pointer In KncP2P )
//////////////////////////////////////////////////////////////////////////
//                      THREADRECV상의 ERROR
KNC_P2P_ERROR( KNC_THREADRECV_INVALID_RAKNET_PACKET, Recv Invalid RakNet Packet )
KNC_P2P_ERROR( KNC_THREADRECV_INVALID_P2P_PACKET, Recv Invalid P2P Packet )
KNC_P2P_ERROR( KNC_THREADRECV_INVALID_PTR, Exist Invalid Pointer In ThreadRecv)
KNC_P2P_ERROR( KNC_THREADRECV_EMPTY_CONNECTQ, ConnectQ is Empty )

KNC_P2P_ERROR( KNC_P2P_ERROR_END, KNC_P2P_ERROR_END )
#ifdef __NEVER_DEFINED_MACRO__
};
#undef KNC_P2P_ERROR
#endif
