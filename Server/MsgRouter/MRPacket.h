#pragma once

#include "CommonPacket.h"
#include "Event.h"

#pragma pack( push, 1 )


DECL_PACKET( EMR_CLIENT_CONNECT_REPORT_REQ )
{
    DWORD                               m_dwLogonUID;
    std::map< int, std::set<DWORD> >    m_mapFriendGroup;
};

typedef KEMR_CLIENT_CONNECT_REPORT_REQ KEMR_LOGINNED_FRIEND_INFO_REQ;

DECL_PACKET( EMR_REMOTE_QUEUEING_REQ )
{
    DWORD           m_dwSenderUID;
    DWORD           m_dwRecverUID;
    KSerBuffer      m_kData;
    bool            m_bCheckRecv;
    bool            m_bDirectSend;
};

typedef KEMR_REMOTE_QUEUEING_REQ KEMR_REMOTE_QUEUEING_ACK;

DECL_PACKET( EMR_REMOTE_QUEUEING_PACK_REQ )
{
    DWORD               m_dwSenderUID;
    std::vector<DWORD>  m_vecRecverUIDList;
    KSerBuffer          m_kData;
    bool                m_bCheckRecv;
    bool                m_bDirectSend;
};
typedef KEMR_REMOTE_QUEUEING_PACK_REQ KEMR_REMOTE_QUEUEING_PACK_ACK;

DECL_PACKET( EMR_CLIENT_CONNECT_REPORT_ACK )
{
    int             m_nOK;
    DWORD           m_dwLogonUID;
};

DECL_PACKET( EMR_KICK_CLIENT_NOT )
{
    int             m_nOK;
    DWORD           m_dwUserUID;
};

#pragma pack( pop )