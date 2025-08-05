#include "MRPacket.h"

SERIALIZE_DEFINE_PUT( KEMR_CLIENT_CONNECT_REPORT_REQ, obj, ks )
{
    return  PUT( m_dwLogonUID ) &&
            PUT( m_mapFriendGroup );
}

SERIALIZE_DEFINE_GET( KEMR_CLIENT_CONNECT_REPORT_REQ, obj, ks )
{
    return  GET( m_dwLogonUID ) &&
            GET( m_mapFriendGroup );
}

SERIALIZE_DEFINE_PUT( KEMR_REMOTE_QUEUEING_REQ, obj, ks )
{
    return  PUT( m_dwSenderUID ) &&
            PUT( m_dwRecverUID ) &&
            PUT( m_kData ) &&
            PUT( m_bCheckRecv ) &&
            PUT( m_bDirectSend );
}

SERIALIZE_DEFINE_GET( KEMR_REMOTE_QUEUEING_REQ, obj, ks )
{
    return  GET( m_dwSenderUID ) &&
            GET( m_dwRecverUID ) &&
            GET( m_kData ) &&
            GET( m_bCheckRecv ) &&
            GET( m_bDirectSend );
}

SERIALIZE_DEFINE_PUT( KEMR_KICK_CLIENT_NOT, obj, ks )
{
    return  PUT( m_nOK ) &&
            PUT( m_dwUserUID );
}

SERIALIZE_DEFINE_GET( KEMR_KICK_CLIENT_NOT, obj, ks )
{
    return  GET( m_nOK ) &&
            GET( m_dwUserUID );
}

SERIALIZE_DEFINE_PUT( KEMR_CLIENT_CONNECT_REPORT_ACK, obj, ks )
{
    return  PUT( m_nOK ) &&
            PUT( m_dwLogonUID );
}

SERIALIZE_DEFINE_GET( KEMR_CLIENT_CONNECT_REPORT_ACK, obj, ks )
{
    return  GET( m_nOK ) &&
            GET( m_dwLogonUID );
}

SERIALIZE_DEFINE_GET( KEMR_REMOTE_QUEUEING_PACK_REQ, obj, ks )
{
    return  GET( m_dwSenderUID ) &&
            GET( m_vecRecverUIDList ) &&
            GET( m_kData ) &&
            GET( m_bCheckRecv ) &&
            GET( m_bDirectSend );
}

SERIALIZE_DEFINE_PUT( KEMR_REMOTE_QUEUEING_PACK_REQ, obj, ks )
{
    return  PUT( m_dwSenderUID ) &&
            PUT( m_vecRecverUIDList ) &&
            PUT( m_kData ) &&
            PUT( m_bCheckRecv ) &&
            PUT( m_bDirectSend );
}
